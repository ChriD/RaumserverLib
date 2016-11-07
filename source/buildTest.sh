#!/bin/bash
ARCH=$1
rm -rf build/testDyn
rm -rf build/testStat
mkdir -p build
mkdir -p build/linux_$ARCH
mkdir -p build/linux_$ARCH/logs
make arch=$ARCH dbg=1 clean -f makefile_test
make arch=$ARCH dbg=1 -f makefile_test
/bin/cp -rf build/testDyn build/linux_$ARCH/testDyn
/bin/cp -rf build/testStat build/linux_$ARCH/testStat
/bin/cp -rf settings.xml build/linux_$ARCH/settings.xml
make arch=$ARCH dbg=1 clean -f makefile_test

