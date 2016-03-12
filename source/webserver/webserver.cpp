
#include <raumserver/webserver/webserver.h>

namespace Raumserver
{
    namespace Server
    {

        Webserver::Webserver() : RaumkernelBase()
        {
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

            try
            {
                logDebug("Starting webserver for requests on port: " + std::to_string(_port), CURRENT_POSITION);

                server = std::shared_ptr<Mongoose::Server>(new Mongoose::Server(_port));
                requestController = std::shared_ptr<RequestController>(new RequestController());

                server->registerController(requestController.get());
                server->start();

                logInfo("Webserver for requests started (Port: " + std::to_string(_port) + ")", CURRENT_POSITION);
                isStarted = true;
            }    
            // errors when starting the server should lead to apprash!
            catch (Raumkernel::Exception::RaumkernelException &e)
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
            if (server && isStarted)
                server->stop();
        }



        void RequestController::setup()
        {            
            //server->resource["^/raumserver/room/(.*)/(.*)"]["POST"] = std::bind(&RaumkernServer::HandleRoomRequests, this, std::placeholders::_1, std::placeholders::_2);
            //server->resource["^/raumserver/room/(.*)/(.*)"]["GET"] = std::bind(&RaumkernServer::HandleRoomRequests, this, std::placeholders::_1, std::placeholders::_2);
            //server->resource["^/raumserver/zone/(.*)/(.*)"]["POST"] = std::bind(&RaumkernServer::HandleZoneRequests, this, std::placeholders::_1, std::placeholders::_2);
            //server->resource["^/raumserver/zone/(.*)/(.*)"]["GET"] = std::bind(&RaumkernServer::HandleZoneRequests, this, std::placeholders::_1, std::placeholders::_2);

            //registerRoute("GET", "^/raumserver/room/(.*)/(.*)", new Mongoose::RequestHandler<RequestController, Mongoose::StreamResponse>(this, &RequestController::handleRequest));
            // http://10.0.0.4:8080/raumserver/request?zone=Wohnzimmer&action=play           
            // http://10.0.0.4:8080/raumserver/request?zone=Wohnzimmer&action=stop
            // http://10.0.0.4:8080/raumserver/request?room=Wohnzimmer&action=volume&value=-10
            registerRoute("GET", "/request", new Mongoose::RequestHandler<RequestController, Mongoose::StreamResponse>(this, &RequestController::handleRequest));
            //registerRoute("GET", "/request/play", new Mongoose::RequestHandler<RequestController, Mongoose::StreamResponse>(this, &RequestController::handleRequest));
            //registerRoute("GET", "/request/stop", new Mongoose::RequestHandler<RequestController, Mongoose::StreamResponse>(this, &RequestController::handleRequest));
        }


        void RequestController::handleRequest(Mongoose::Request &_request, Mongoose::StreamResponse &_response)
        {
             std::string method = _request.getMethod();
            //_response << "Raumserver is not finished yet! " << Mongoose::Utils::htmlEntities(_request.get("name", "... what's your name ?")) << endl;
             _response << "Raumserver is not finished yet! " << std::endl;
        }
       

    }
}