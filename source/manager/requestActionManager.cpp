
#include <raumserver/manager/requestActionManager.h>

namespace Raumserver
{
    namespace Manager
    {

        RequestActionManager::RequestActionManager() : ManagerBaseServer()
        {    
            stopThreads = false;
        }


        RequestActionManager::~RequestActionManager()
        {    
            stopThreads = true;
            if (doRequestsThreadObject.joinable())
            {
                logDebug("Waiting for RequestWorkerThread thread to finish (This may take some time...)", CURRENT_POSITION);
                doRequestsThreadObject.join();
            }
            logDebug("Destroying RequestAction-Manager", CURRENT_POSITION);
        }
   
        
        void RequestActionManager::init()
        {
            doRequestsThreadObject = std::thread(&RequestActionManager::requestProcessingWorkerThread, this);
        }
        
        
        void RequestActionManager::requestProcessingWorkerThread()
        {
            while (!stopThreads)
            {
                mutexRequestActionQueue.lock();

                try
                {                    
                    if (requestActionQueue.size())
                    {
                        // get the first request in the queue and process it!
                        auto requestAction = requestActionQueue.front();
                        logDebug("Processing Request: " + requestAction->getRequestInfo(), CURRENT_POSITION);
                        requestAction->execute();                        
                        logDebug("Popping Request: " + requestAction->getRequestInfo(), CURRENT_POSITION);
                        requestActionQueue.pop();
                    }

                }
                catch (Raumkernel::Exception::RaumkernelException &e)
                {
                    if (e.type() == Raumkernel::Exception::ExceptionType::EXCEPTIONTYPE_APPCRASH)
                        throw e;
                }
                catch (std::exception &e)
                {
                    logError(e.what(), CURRENT_POSITION);
                }
                catch (std::string &e)
                {
                    logError(e, CURRENT_POSITION);
                }
                catch (OpenHome::Exception &e)
                {
                    logError(e.Message(), CURRENT_POSITION);;
                }
                catch (...)
                {
                    logError("Unknown exception!", CURRENT_POSITION);
                }

                mutexRequestActionQueue.unlock();

                // Keep the cpu load low. the processing of the request is not very critical but should be in time
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        }
        

        void RequestActionManager::addRequestAction(std::shared_ptr<Request::RequestAction> _requestAction)
        {
            std::unique_lock<std::mutex> lock(mutexRequestActionQueue);
            requestActionQueue.push(_requestAction);
        }
       
    }
}