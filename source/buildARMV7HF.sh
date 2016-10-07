#!/bin/bash
./getActualLibs.sh
make arch=ARMV7HF dbg=1 clean
make arch=ARMV7HF dbg=1
/bin/cp -rf libs/_tmpBuild/raumserver.so libs/linux_ARMV7/raumserver.so
make arch=ARMV7HF dbg=1 clean
