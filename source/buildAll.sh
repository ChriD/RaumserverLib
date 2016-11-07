#!/bin/bash
./buildX64.sh
./buildARMV7HF.sh
./buildARMV5TE.sh
#
./buildX64_test.sh
./buildARMV7HF_test.sh
./buildARMV5TE_test.sh

