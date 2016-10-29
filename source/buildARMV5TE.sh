#!/bin/bash
mkdir -p libs/_tmpBuild
./getActualLibs.sh
make arch=ARMV5TE dbg=1 clean
make arch=ARMV5TE dbg=1
/bin/cp -rf libs/_tmpBuild/raumserver.so libs/linux_ARMV5/raumserver.so
make arch=ARMV5TE dbg=1 clean
