#!/bin/bash
./buildX86.sh
./buildX64.sh
./buildARMV7HF.sh
#
./buildX86_test.sh
./buildX64_test.sh
./buildARMV7HF_test.sh

