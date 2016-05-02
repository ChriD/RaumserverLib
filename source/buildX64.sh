#!/bin/bash
./getActualLibs.sh
make arch=X64 clean
make arch=X64
/bin/cp -rf libs/_tmpBuild/raumserver.so libs/linux_x64/raumserver.so
make arch=X64 clean
