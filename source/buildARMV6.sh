#!/bin/bash
./getActualLibs.sh
make arch=ARMV6 clean
make arch=ARMV6
/bin/cp -rf libs/_tmpBuild/raumserver.so libs/linux_ARMV6/raumserver.so
make arch=ARMV6 clean
