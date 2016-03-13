
#include <raumserver/webserver/webserver.h>

namespace Raumserver
{
    namespace Server
    {

        Webserver::Webserver() : RaumserverBase()
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

                // add a general handler for the raumserver zone action handlings (like zone volume, zone mute, next, prev, aso...)
                serverRequestHandlerZone = std::shared_ptr<RequestHandlerZone>(new RequestHandlerZone());
                serverObject->addHandler("/raumserver/zone", serverRequestHandlerZone.get());

                // add a general handler for the raumserver room action handlings (like removing from zone or add to zone or room volumes, room mutes, aso...)
                serverRequestHandlerRoom = std::shared_ptr<RequestHandlerRoom>(new RequestHandlerRoom());
                serverObject->addHandler("/raumserver/room", serverRequestHandlerRoom.get());
                                                         
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
            if (serverObject && isStarted)
                serverObject->close();
        }


 
        bool RequestHandlerRoom::handleGet(CivetServer *server, struct mg_connection *conn)
        {            
            //logDebug("Webserver for requests started (Port: " + std::to_string(_port) + ")", CURRENT_POSITION);

            mg_printf(conn,
                "HTTP/1.1 200 OK\r\nContent-Type: "
                "text/html\r\nConnection: close\r\n\r\n");
            mg_printf(conn, "<html><body>\r\n");
            mg_printf(conn,
                "<h2>Room Request</h2>\r\n");         
            mg_printf(conn, "</body></html>\r\n");
            return true;                
        }


        bool RequestHandlerZone::handleGet(CivetServer *server, struct mg_connection *conn)
        {

            mg_printf(conn,
                "HTTP/1.1 200 OK\r\nContent-Type: "
                "text/html\r\nConnection: close\r\n\r\n");
            mg_printf(conn, "<html><body>\r\n");
            mg_printf(conn,
                "<h2>Zone Request</h2>\r\n");
            mg_printf(conn, "</body></html>\r\n");
            return true;
        }


       

    }
}