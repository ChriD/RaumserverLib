
#include <raumserver/raumserverBaseMgr.h>

namespace Raumserver
{
    RaumserverBaseMgr::RaumserverBaseMgr() : Raumkernel::RaumkernelBaseMgr()
    {        
        managerEngineerServer = nullptr;
    }


    RaumserverBaseMgr::~RaumserverBaseMgr()
    {
    }


    void RaumserverBaseMgr::setManagerEngineerServer(std::shared_ptr<Manager::ManagerEngineerServer> _managerEngineerServer)
    {
        managerEngineerServer = _managerEngineerServer;
    }
  

    std::shared_ptr<Manager::ManagerEngineerServer> RaumserverBaseMgr::getManagerEngineerServer()
    {
        return managerEngineerServer;
    }
}