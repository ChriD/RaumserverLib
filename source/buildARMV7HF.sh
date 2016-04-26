#!/bin/bash
make arch=ARMV7HF clean
make arch=ARMV7HF
/bin/cp -rf libs/_tmpBuild/raumserver.so libs/linux_ARMV7/raumserver.so
make arch=ARMV7HF clean
