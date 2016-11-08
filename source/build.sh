#!/bin/bash
ARCH=$1
./getLinkedLibs.sh $ARCH
rm -rf build/linux_$ARCH/libs
mkdir -p build
mkdir -p build/linux_$ARCH
mkdir -p build/linux_$ARCH/libs
/bin/cp -rf libs/linux_$ARCH/libraumkernel.so build/linux_$ARCH/libs/libraumkernel.so
make arch=$ARCH dbg=1 clean
make arch=$ARCH dbg=1
/bin/cp -rf build/libraumserver.so build/linux_$ARCH/libs/libraumserver.so
/bin/cp -rf build/libraumserver.a build/linux_$ARCH/libs/libraumserver.a
make arch=$ARCH dbg=1 clean

