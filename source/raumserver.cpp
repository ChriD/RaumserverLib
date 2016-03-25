
#include <raumserver/raumserver.h>
#include <raumkernel/manager/managerEngineer.h>

namespace Raumserver
{

    Raumserver::Raumserver() : RaumkernelBase()
    {
        // set the current version info of the library
        versionInfo.appName = "Raumserver Library";
        versionInfo.appVersion = "1.0.0";
        versionInfo.isBeta = false;

        isOnline = false;
    }


    Raumserver::~Raumserver()
    {
    }


    void Raumserver::init(Raumkernel::Log::LogType _defaultLogLevel)
    {
        // create a new log object for this library which we will provide to the kernel library too so both libraries use the same logger object
        logObject = std::shared_ptr<Raumkernel::Log::Log>(new Raumkernel::Log::Log());
        logObject->registerAdapter(std::shared_ptr<Raumkernel::Log::LogAdapter>(new Raumkernel::Log::LogAdapter_Console()));
        logObject->registerAdapter(std::shared_ptr<Raumkernel::Log::LogAdapter>(new Raumkernel::Log::LogAdapter_File()));        
        logObject->setLogLevel(_defaultLogLevel);               

        // create the raumkernel object and init the kernel from the settings given in the raumserver.xml
        raumkernel = std::shared_ptr<Raumkernel::Raumkernel>(new Raumkernel::Raumkernel());      
        raumkernel->setLogObject(getLogObject());

        // lets do some subscriptions
        connections.connect(raumkernel->sigRaumfeldSystemOnline, this, &Raumserver::onRaumfeldSystemOnline);
        connections.connect(raumkernel->sigRaumfeldSystemOffline, this, &Raumserver::onRaumfeldSystemOffline);

        raumkernel->init(_defaultLogLevel, "raumserver.xml");

        // after init of the kernel we do have the kernel manager engineer
        managerEngineerKernel = raumkernel->getManagerEngineer();

        logDebug("Preparing Server-Manager-Engineer...", CURRENT_POSITION);

        // create the manager engineer which will hold references to all managers. this engineer will be present in each manager and each class
        // which is inherited from 'RaumfeldBaseMgr', but has to be set explicit
        managerEngineerServer = std::shared_ptr<Manager::ManagerEngineerServer>(new Manager::ManagerEngineerServer());
        managerEngineerServer->setLogObject(logObject);
        managerEngineerServer->createManagers();

        // set links to the manager engineer for all managers (this is a little bit of circular dependencies because the managers have a link to the
        // managerEngineer, which has links to the managers again. But this should be no problem, in this case)
        managerEngineerServer->getRequestActionManager()->setManagerEngineer(managerEngineerKernel);
        managerEngineerServer->getRequestActionManager()->setManagerEngineerServer(managerEngineerServer);
        managerEngineerServer->getRequestActionManager()->init();
 
        logDebug("Raumserver Manager-Engineer is prepared", CURRENT_POSITION);

        std::string serverPort = managerEngineerKernel->getSettingsManager()->getValue(SETTINGS_RAUMSERVER_PORT);
        if (serverPort.empty())
        {
            logWarning("No port is specified for the server! Using port '" + SETTINGS_RAUMSERVER_PORT_DEFAULT  + "'", CURRENT_POSITION);
            serverPort = SETTINGS_RAUMSERVER_PORT_DEFAULT;
        }

        // create the webserver object and try to start it. 
        // If the Webserver can not be startet the lib will throw an error which should be non recoverable
        webserver = std::shared_ptr<Server::Webserver>(new Server::Webserver());
        webserver->setManagerEngineer(managerEngineerKernel); 
        webserver->setManagerEngineerServer(managerEngineerServer);
        webserver->setLogObject(getLogObject());
        webserver->start(std::stoi(serverPort));
    }


    std::shared_ptr<Raumkernel::Raumkernel> Raumserver::getRaumkernelObject()
    {
        return raumkernel;
    }


    Raumkernel::Tools::VersionInfo Raumserver::getVersionInfo()
    {
        return versionInfo;
    }


    void Raumserver::onRaumfeldSystemOnline()
    {
        isOnline = true;
    }


    void Raumserver::onRaumfeldSystemOffline()
    {
        isOnline = false;
    }


    bool Raumserver::isRaumserverOnline()
    {
        return isOnline;
    }

}