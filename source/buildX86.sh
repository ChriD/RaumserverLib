#!/bin/bash
./getActualLibs.sh
make arch=X86 clean
make arch=X86
/bin/cp -rf libs/_tmpBuild/raumserver.so libs/linux_x86/raumserver.so
make arch=X86 clean
