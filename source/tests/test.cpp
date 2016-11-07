
#include <raumserver/raumserver.h>
#include <raumkernel/rlutil/rlutil.h>


int main()
{
    Raumserver::Raumserver          raumServer;

    raumServer.initLogObject();
    raumServer.init(); 

    rlutil::getkey();
  
    return 0;
}





