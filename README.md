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

|     |inputfile1         | inputfile2        | outputfile      |
| --------- | ---------- | ------------ | --------------- |
|-t   |RadonData.root     |                   | Rn_template.root|
|-an  |RadonData.root     | Rn_template.root  | Rn_analysis.root|
|-dir |doc/GDMScatalog.txt| datetime.txt      | EQdir.root      |
|-geo |(name)HWA_00_    | datetime.txt      | GeoData.root    |
|-pd  |Rn_analysis.root   | EQdir.root        |                 |
|-pg  |Rn_analysis.root   | GeoData.root      |                 |

`./main -t -i root_output/RadonData.root -o root_output/Rn_template_test.root`
`./main -an -i root_output/RadonData.root -i2 root_output/Rn_template_test.root -o root_output/Rn_analysis_test.root`
`./main -dir -i data/GDMScatalog20211101-1231_HWA.txt -i2 doc/datetime_211101-1231.txt -o root_output/EQdir_test.root`
`./main -geo -i HWA_00_EHE_20211101_20211231 -i2 doc/datetime_211101-1231.txt`
`./main -pd -i root_output/Rn_analysis_test.root -i2 root_output/EQdir_test.root`
`./main -pg -i root_output/Rn_analysis_test.root -i2 root_output/Geodata_HWA_20211101_20211231.root`
