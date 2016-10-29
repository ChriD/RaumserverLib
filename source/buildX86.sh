#!/bin/bash
mkdir -p libs/_tmpBuild
./getActualLibs.sh
make arch=X86 dbg=1 clean
make arch=X86 dbg=1
/bin/cp -rf libs/_tmpBuild/raumserver.so libs/linux_x86/raumserver.so
make arch=X86 dbg=1 clean
