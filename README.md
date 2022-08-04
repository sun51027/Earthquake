# Earthquake

## Radon data

Radon data input : plots_root/RadonData.root

+ each histrogram is the energy spectrum (-0.5 ~ 4.5 MeV)  for 2 hours

+ Radon (signal) $\epsilon$  [0.25,0.8] (MeV)

+ K40 ~ 1.4 (MeV)

+ benchmark for normaliz. $\epsilon\$ [2.2,2.5] (MeV)


## Data from CWBSN

**Timezone are UTC**

1. [Event catagory 地震目錄](https://gdmsn.cwb.gov.tw/catalogDownload.php)

+ To match with the datetime of radon data, please download GMT time data:

+ ex. Radon data 9/15 00:00:00 ~ 12/31 23:59:59 -> Event category 9/14 16:00:00 ~ 12/31 15:59:59

+ Longitude : 121.15 - 122 (~76 km)

+ Latitude : 23.30 - 24.30 (~110 km)

2. [Continuous Waveform Data 連續波形資料](https://gdmsn.cwb.gov.tw/eqconDownload.php)

+	Timezone is same as above.

## Code

So far three class have been built for radon data, eq dir, and wavefrom data.	

|           | radon data | EQ directory | waveform data   |
| --------- | ---------- | ------------ | --------------- |
| class     | Earthquake | DataReader   | GeoData         |
| include   | EQ.h       | DataReader.h | GeoData.h       |

**Note that the function in DataReader (ReadData()) and GeoData are already transform the UTC to GMT+8, thus you don't need to change it again when you draw plots!!!**

### command

`./main -h` : open argument list

We deal radon data (-an) and earthquake directory (-dir) separately. They are stored into different root files. At last, to compare the earthquake event with radon data, you need to use -p to draw pvalue. This "drawPvalue" function is indepedent of any class so far. 

example for 20210915~20211231 (1) analysis, (2) eqdir, (3) draw pvalue, (4) draw waveform data:

`./main -an -o plots_root/oAnalyzer.root -i plots_root/RadonData.root -i2 plots_root/template.root`

`./main -dir -i data/GDMScatalog20210915-1231.txt -i2 doc/datetimeOffset.txt -o plots_root/EQdir20210915-1231.root`

`./main -p` (I haven't change the argument...)

`root -l -b  -q 'macros/GeoData.cc("output_root/HWA_00_EHZ_20220322_20220325.root")'` (cd Data_Geophysical first)

