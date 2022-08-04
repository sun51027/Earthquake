#!/bin/zsh
source ~/root/root_install/bin/thisroot.sh
rm *HWA*.root
make
./main -geo -i HWA_00_EHE_20220322_20220325 -i2 doc/datetime_220322-0325.txt 
./main -geo -i HWA_00_EHN_20220322_20220325 -i2 doc/datetime_220322-0325.txt 
./main -geo -i HWA_00_EHZ_20220322_20220325 -i2 doc/datetime_220322-0325.txt  
./main -geo -i HWA_00_HH1_20220322_20220325 -i2 doc/datetime_220322-0325.txt 
./main -geo -i HWA_00_HH2_20220322_20220325 -i2 doc/datetime_220322-0325.txt 
./main -geo -i HWA_00_HHZ_20220322_20220325 -i2 doc/datetime_220322-0325.txt 
./main -geo -i HWA_00_HN1_20220322_20220325 -i2 doc/datetime_220322-0325.txt 
./main -geo -i HWA_00_HN2_20220322_20220325 -i2 doc/datetime_220322-0325.txt 
./main -geo -i HWA_00_HNZ_20220322_20220325 -i2 doc/datetime_220322-0325.txt 

#./main -geo -i HWA_00_EHE_20220322_20220325 -i2 doc/datetime_220322-0325.txt -th 10000
#./main -geo -i HWA_00_EHN_20220322_20220325 -i2 doc/datetime_220322-0325.txt -th 50000
#./main -geo -i HWA_00_EHZ_20220322_20220325 -i2 doc/datetime_220322-0325.txt -th 50000  
#./main -geo -i HWA_00_HH1_20220322_20220325 -i2 doc/datetime_220322-0325.txt -th 500000
#./main -geo -i HWA_00_HH2_20220322_20220325 -i2 doc/datetime_220322-0325.txt -th 500000
#./main -geo -i HWA_00_HHZ_20220322_20220325 -i2 doc/datetime_220322-0325.txt -th 500000
#./main -geo -i HWA_00_HN1_20220322_20220325 -i2 doc/datetime_220322-0325.txt -th 40000
#./main -geo -i HWA_00_HN2_20220322_20220325 -i2 doc/datetime_220322-0325.txt -th 40000
#./main -geo -i HWA_00_HNZ_20220322_20220325 -i2 doc/datetime_220322-0325.txt -th 40000
#
#
## 210915-211031
#./main -geo -i HWA_00_EHE_20210914_20211031 -i2 doc/datetime_210915-1031.txt -th 40000
#./main -geo -i HWA_00_EHN_20210914_20211031 -i2 doc/datetime_210915-1031.txt -th 1000000
#./main -geo -i HWA_00_EHZ_20210914_20211031 -i2 doc/datetime_210915-1031.txt -th 100000  
./main -geo -i HWA_00_EHE_20210914_20211031 -i2 doc/datetime_210915-1031.txt 
./main -geo -i HWA_00_EHN_20210914_20211031 -i2 doc/datetime_210915-1031.txt 
./main -geo -i HWA_00_EHZ_20210914_20211031 -i2 doc/datetime_210915-1031.txt 
