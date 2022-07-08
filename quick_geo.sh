#!/bin/zsh
source ~/root/root_install/bin/thisroot.sh

make
./main -geo -i HWA_00_EHE_20220322_20220325 -i2 doc/time_name_test.txt
./main -geo -i HWA_00_EHN_20220322_20220325 -i2 doc/time_name_test.txt
./main -geo -i HWA_00_EHZ_20220322_20220325 -i2 doc/time_name_test.txt
./main -geo -i HWA_00_HH1_20220322_20220325 -i2 doc/time_name_test.txt
./main -geo -i HWA_00_HH2_20220322_20220325 -i2 doc/time_name_test.txt
./main -geo -i HWA_00_HHZ_20220322_20220325 -i2 doc/time_name_test.txt
./main -geo -i HWA_00_HN1_20220322_20220325 -i2 doc/time_name_test.txt
./main -geo -i HWA_00_HN2_20220322_20220325 -i2 doc/time_name_test.txt
./main -geo -i HWA_00_HNZ_20220322_20220325 -i2 doc/time_name_test.txt
