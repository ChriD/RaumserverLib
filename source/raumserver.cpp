
#include <raumserver/raumserver.h>
#include <raumkernel/manager/managerEngineer.h>

#include <signal.h>
#include <raumkernel/backtrace.hpp>

namespace Raumserver
{

    Raumserver::Raumserver() : RaumkernelBase()
    {        
        settingsFile = "settings.xml";  

        versionInfo.appName = Server_AppName;
        versionInfo.appVersion = Server_VersionNumber;
        versionInfo.appVersionName = Server_VersionName;

        // create a new log object for this library which we will provide to the kernel library too so both libraries use the same logger object
        logObject = std::shared_ptr<Raumkernel::Log::Log>(new Raumkernel::Log::Log());
    }


    Raumserver::~Raumserver()
    {
    }


    void Raumserver::raiseSigsegv()
    {
        raise(SIGSEGV);
    }


    void Raumserver::addSystemSignalHandlers()
    {
        Backtrace::AddSignalHandlers();
    }
   

    void Raumserver::initLogObject(Raumkernel::Log::LogType _defaultLogLevel, const std::string &_logFilePath, const std::vector<std::shared_ptr<Raumkernel::Log::LogAdapter>> &_adapterList)
    {                
        logObject = std::shared_ptr<Raumkernel::Log::Log>(new Raumkernel::Log::Log());
        
        if (_adapterList.empty())
        {
            auto logAdapterConsole = std::shared_ptr<Raumkernel::Log::LogAdapter_Console>(new Raumkernel::Log::LogAdapter_Console());
            logObject->registerAdapter(logAdapterConsole);

            auto logAdapterFile = std::shared_ptr<Raumkernel::Log::LogAdapter_File>(new Raumkernel::Log::LogAdapter_File());
            if (!_logFilePath.empty())
                logAdapterFile->setLogFilePath(_logFilePath);
            logObject->registerAdapter(logAdapterFile);
        }
        else
        {
            for (auto i : _adapterList)
            {
                logObject->registerAdapter(i);
            }
        }

        logObject->setLogLevel(_defaultLogLevel);
    }


    void Raumserver::init()
    {                                      
        // create the raumkernel object and init the kernel from the settings given in the raumserver.xml
        raumkernel = std::shared_ptr<Raumkernel::Raumkernel>(new Raumkernel::Raumkernel());     
        // we do not init the log object we use the log object created in our class
        raumkernel->setLogObject(getLogObject());

        // lets do some subscriptions
        connections.connect(raumkernel->sigRaumfeldSystemOnline, this, &Raumserver::onRaumfeldSystemOnline);
        connections.connect(raumkernel->sigRaumfeldSystemOffline, this, &Raumserver::onRaumfeldSystemOffline);
        connections.connect(raumkernel->getLogObject()->sigLog, this, &Raumserver::onLog);

        raumkernel->init(settingsFile);

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
        managerEngineerServer->getRequestActionManager()->setKernelVersion(raumkernel->getVersionInfo());
        managerEngineerServer->getRequestActionManager()->setServerVersion(versionInfo);
        managerEngineerServer->getRequestActionManager()->init();
 
        logDebug("Raumserver Manager-Engineer is prepared", CURRENT_POSITION);

        std::string serverPort = managerEngineerKernel->getSettingsManager()->getValue(SETTINGS_RAUMSERVER_PORT);
        if (serverPort.empty())
        {
            logWarning("No port is specified for the server! Using port '" + SETTINGS_RAUMSERVER_PORT_DEFAULT  + "'", CURRENT_POSITION);
            serverPort = SETTINGS_RAUMSERVER_PORT_DEFAULT;
        }

        std::string docRoot = managerEngineerKernel->getSettingsManager()->getValue(SETTINGS_RAUMSERVER_DOCROOT);
        if (docRoot.empty())
        {
            logWarning("No docroot is specified for the server! Using ''" + SETTINGS_RAUMSERVER_DOCROOT_DEFAULT + "'", CURRENT_POSITION);
            docRoot = SETTINGS_RAUMSERVER_DOCROOT_DEFAULT;
        }

        // create the webserver object and try to start it. 
        // If the Webserver can not be startet the lib will throw an error which should be non recoverable
        webserver = std::shared_ptr<Server::Webserver>(new Server::Webserver());
        webserver->setManagerEngineer(managerEngineerKernel); 
        webserver->setManagerEngineerServer(managerEngineerServer);
        webserver->setLogObject(getLogObject());
        webserver->setDocumentRoot(docRoot);
        webserver->start(std::stoi(serverPort));
    }


    std::shared_ptr<Raumkernel::Raumkernel> Raumserver::getRaumkernelObject()
    {
        return raumkernel;
    }


    VersionInfo::VersionInfo Raumserver::getVersionInfo()
    {
        return versionInfo;
    }


    void Raumserver::onRaumfeldSystemOnline()
    {
        logInfo("Raumfeld System is now online!", CURRENT_POSITION);        
        managerEngineerServer->setSystemReady(true);
    }


    void Raumserver::onRaumfeldSystemOffline()
    {
        logInfo("Raumfeld System is now offline!", CURRENT_POSITION);        
        managerEngineerServer->setSystemReady(false);
    }


    bool Raumserver::isRaumserverOnline()
    {
        if (managerEngineerServer == nullptr)
            return false;
        return managerEngineerServer->isSystemReady();
    }


    void Raumserver::onLog(Raumkernel::Log::LogData _logData)
    {
        sigLog.fire(_logData);
    }


    void Raumserver::setSettingsFile(const std::string &_settingsFile)
    {
        settingsFile = _settingsFile;
    }

}