
#include <raumserver/raumserver.h>
#include <raumkernel/manager/managerEngineer.h>

namespace Raumserver
{

    Raumserver::Raumserver() : RaumkernelBase()
    {
    }


    Raumserver::~Raumserver()
    {
    }


    void Raumserver::init(Raumkernel::Log::LogType _defaultLogLevel)
    {
        
        logObject = std::shared_ptr<Raumkernel::Log::Log>(new Raumkernel::Log::Log());
        logObject->registerAdapter(std::shared_ptr<Raumkernel::Log::LogAdapter>(new Raumkernel::Log::LogAdapter_Console()));
        logObject->registerAdapter(std::shared_ptr<Raumkernel::Log::LogAdapter>(new Raumkernel::Log::LogAdapter_File()));        
        logObject->setLogLevel(_defaultLogLevel);               

        raumkernel = std::shared_ptr<Raumkernel::Raumkernel>(new Raumkernel::Raumkernel());      
        raumkernel->setLogObject(getLogObject());
        raumkernel->init(_defaultLogLevel, "raumserver.xml");

        // after init of the kernel we do have the manager engineer
        managerEngineer = raumkernel->getManagerEngineer();

        std::string serverPort = managerEngineer->getSettingsManager()->getValue(SETTINGS_RAUMSERVER_PORT);
        if (serverPort.empty())
        {
            logWarning("No port is specified for the server! Using port '" + SETTINGS_RAUMSERVER_PORT_DEFAULT  + "'", CURRENT_POSITION);
            serverPort = SETTINGS_RAUMSERVER_PORT_DEFAULT;
        }

        webserver = std::shared_ptr<Server::Webserver>(new Server::Webserver());
        webserver->setLogObject(getLogObject());
        webserver->start(std::stoi(serverPort));
    }


    std::shared_ptr<Raumkernel::Raumkernel> Raumserver::getRaumkernelObject()
    {
        return raumkernel;
    }
}