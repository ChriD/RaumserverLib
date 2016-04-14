
#include <raumserver/webserver/webserver.h>

namespace Raumserver
{
    namespace Server
    {

        Webserver::Webserver() : RaumserverBaseMgr()
        {
            serverObject = nullptr;
            isStarted = false;
        }


        Webserver::~Webserver()
        {
            stop();
        }

     
        void Webserver::start(std::uint32_t _port)
        {
            if (isStarted)
                return;

            std::vector<std::string> serverOptions;

            try
            {
                logDebug("Starting webserver for requests on port: " + std::to_string(_port), CURRENT_POSITION);

                serverOptions.push_back("listening_ports");
                serverOptions.push_back(std::to_string(_port));

                serverObject = std::shared_ptr<CivetServer>(new CivetServer(serverOptions));              

                // add a general handler for the raumserver room and zone action handlings (like removing from zone or add to zone or room volumes, room mutes, aso...)
                serverRequestHandlerController = std::shared_ptr<RequestHandlerController>(new RequestHandlerController());
                serverRequestHandlerController->setManagerEngineerServer(getManagerEngineerServer());
                serverRequestHandlerController->setManagerEngineerKernel(getManagerEngineer());
                serverRequestHandlerController->setLogObject(getLogObject());
                serverObject->addHandler("/raumserver/controller", serverRequestHandlerController.get());

                // add a general handler for fetching data 
                serverRequestHandlerData = std::shared_ptr<RequestHandlerData>(new RequestHandlerData());
                serverRequestHandlerData->setManagerEngineerServer(getManagerEngineerServer());
                serverRequestHandlerData->setManagerEngineerKernel(getManagerEngineer());
                serverRequestHandlerData->setLogObject(getLogObject());
                serverObject->addHandler("/raumserver/data", serverRequestHandlerData.get());

                // add a general handler for wrong path requests
                serverRequestHandlerVoid = std::shared_ptr<RequestHandlerVoid>(new RequestHandlerVoid());
                serverRequestHandlerVoid->setManagerEngineerServer(getManagerEngineerServer());
                serverRequestHandlerVoid->setManagerEngineerKernel(getManagerEngineer());
                serverRequestHandlerVoid->setLogObject(getLogObject());
                serverObject->addHandler("", serverRequestHandlerVoid.get());
                                                         
                logInfo("Webserver for requests started (Port: " + std::to_string(_port) + ")", CURRENT_POSITION);
                isStarted = true;
            }    
            // errors when starting the server should lead to apprash!
            catch (Raumkernel::Exception::RaumkernelException &e)
            {          
                logError(e.what(), CURRENT_FUNCTION);
                throw e;
            }
            catch (CivetException &e)
            {
                logError(e.what(), CURRENT_FUNCTION);
                throw e;
            }
            catch (std::exception &e)
            {
                logError(e.what(), CURRENT_POSITION);
                throw e;
            }
            catch (std::string &e)
            {
                logError(e, CURRENT_POSITION);
                throw e;
            }            
            catch (...)
            {
                logError("Unknown exception!", CURRENT_POSITION);
                throw std::runtime_error("Unknown exception!");
            }         
        }


        void Webserver::stop()
        {            
            if (serverObject && isStarted)
                serverObject->close();
        }

   

        void RequestHandlerBase::setManagerEngineerServer(std::shared_ptr<Manager::ManagerEngineerServer> _managerEngineerServer)
        {
            managerEngineerServer = _managerEngineerServer;
        }

        std::shared_ptr<Manager::ManagerEngineerServer> RequestHandlerBase::getManagerEngineerServer()
        {
            return managerEngineerServer;
        }

        void RequestHandlerBase::setManagerEngineerKernel(std::shared_ptr<Raumkernel::Manager::ManagerEngineer> _managerEngineerKernel)
        {
            managerEngineerKernel = _managerEngineerKernel;
        }

        std::shared_ptr<Raumkernel::Manager::ManagerEngineer> RequestHandlerBase::getManagerEngineerKernel()
        {
            return managerEngineerKernel;
        }

        void RequestHandlerBase::setLogObject(std::shared_ptr<Raumkernel::Log::Log> _logObject)
        {
            logObject = _logObject;
        }

        std::shared_ptr<Raumkernel::Log::Log> RequestHandlerBase::getLogObject()
        {
            return logObject;
        }

        void RequestHandlerBase::sendResponse(struct mg_connection *_conn, std::string _string, bool _error)
        {
            mg_printf(_conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n");
            mg_printf(_conn, "<html><body>\r\n");
            mg_printf(_conn, "<h2>Raumserver</h2>\r\n");
            mg_printf(_conn, _string.c_str());
            mg_printf(_conn, "</body></html>\r\n");
        }


        void RequestHandlerBase::sendDataResponse(struct mg_connection *_conn, std::string _string, bool _error)
        {       
            mg_printf(_conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n");
            mg_printf(_conn, _string.c_str());         
        }



        bool RequestHandlerVoid::handleGet(CivetServer *_server, struct mg_connection *_conn)
        {
            if (!getManagerEngineerServer())
                return false;           
            const struct mg_request_info *request_info = mg_get_request_info(_conn);
            sendResponse(_conn, "Wrong request path! Please check to the documentation for the valid path!", true);
            return true;         
        }



        bool RequestHandlerController::handleGet(CivetServer *_server, struct mg_connection *_conn)
        {
            if (!getManagerEngineerServer())
                return false;

            // TODO: Check if system is online, otherwise don't execute!

            const struct mg_request_info *request_info = mg_get_request_info(_conn);   

            // create request action object from url given from the connection
            std::shared_ptr<Request::RequestAction> requestAction = Request::RequestAction::createFromPath(request_info->request_uri, request_info->query_string == nullptr ? "" : request_info->query_string);

            // if there is no reuquest action the path is wrong or not existent!
            if (!requestAction)
            {
                sendResponse(_conn, "Action for request '" + std::string(request_info->request_uri) + "' not found! Please check to the documentation for valid requests!", true);
                return true;                
            }

            requestAction->setManagerEngineer(getManagerEngineerKernel());
            requestAction->setManagerEngineerServer(getManagerEngineerServer());
            requestAction->setLogObject(getLogObject());

            // if we should stack the request we have to add it to the request manager and return the error values of the validate if there are some
            // the Reuest-Manager will take care of the Request from now on
            if (requestAction->isStackable())
            {
                if (requestAction->isValid())
                {
                    getManagerEngineerServer()->getRequestActionManager()->addRequestAction(requestAction);                    
                    sendResponse(_conn, "Request '" + std::string(request_info->request_uri) + "' was added to queue!", false);                    
                }
                else
                {                    
                    sendResponse(_conn, "Error while executing request: '" + requestAction->getErrors(), true);
                }
            }
            // the request is not stackable, that means we have to execute it right now
            // if the request is a returnable item we have to return the data string from the requestAction
            else
            {
                // a returnable request ist always a sync and non stackable request
                if (std::dynamic_pointer_cast<Request::RequestActionReturnable>(requestAction))
                {
                    if (requestAction->execute())
                    {
                        auto requestActionReturnable = std::dynamic_pointer_cast<Request::RequestActionReturnable>(requestAction);
                        sendDataResponse(_conn, requestActionReturnable->getResponseData(), false);                        
                    }
                    else
                    {
                        // TODO: set better response!
                        sendDataResponse(_conn, "ERROR'" + requestAction->getErrors(), true);
                    }
                    
                }
                else
                {
                    requestAction->execute();
                    sendResponse(_conn, "Request '" + std::string(request_info->request_uri) + "' was executed!", false);                    
                }
            }
             
            return true;
        }


        bool RequestHandlerData::handleGet(CivetServer *_server, struct mg_connection *_conn)
        {
            return RequestHandlerController::handleGet(_server, _conn);
        }
   
    }
}