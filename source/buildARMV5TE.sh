#!/bin/bash
./getActualLibs.sh
make arch=ARMV5TE clean
make arch=ARMV5TE
/bin/cp -rf libs/_tmpBuild/raumserver.so libs/linux_ARMV5/raumserver.so
make arch=ARMV5TE clean
