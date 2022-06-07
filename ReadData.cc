//
//	read earthquake data
//
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

#include "TString.h"
#include "TGraph.h"
#include "TCanvas.h"

#include "interface/DataReader.h"

// void DataReader::EarthquakeDirectory(){

void DataReader::Init()
{
  string c1, c2, c13, c15, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c14;

  ifstream earthqakeDirInput;
  earthqakeDirInput.open("data/GDMScatalog20210915-1231.txt");

  rawdata.resize(64);
  if (!earthqakeDirInput.is_open()) {
    cout << "Failed to open file" << endl;
  } else {
    while (earthqakeDirInput >> c1 >> c2 >> c3 >> c4 >> c5 >> c6 >> c7 >> c8 >> c9 >> c10 >> c11 >> c12 >> c13 >> c14 >>
           c15) {
      date_raw.push_back(c1);
      time_raw.push_back(c2);
      lat_raw.push_back(c3);
      lon_raw.push_back(c4);
      depth_raw.push_back(c5);
      ML_raw.push_back(c6);
      nstn_raw.push_back(c7);
      dmin_raw.push_back(c8);
      gap_raw.push_back(c9);
      trms_raw.push_back(c10);
      ERH_raw.push_back(c11);
      ERZ_raw.push_back(c12);
      fixed_raw.push_back(c13);
      nph_raw.push_back(c14);
      quality_raw.push_back(c15);
    }
  }
}


vector<DataReader> DataReader::ReadRawData()
{

  datetime.resize(64);

  for (int i = 1; i < lat_raw.size(); i++) {

    // date time combine
    datetime[i - 1].Form("%s%s", date_raw[i].c_str(), time_raw[i].c_str());
    datetime[i - 1].Remove(4, 1);
    datetime[i - 1].Remove(6, 1);
    datetime[i - 1].Remove(10, 9);
    TString s(datetime[i - 1](9, 10));
    if (s == "1") {
      datetime[i - 1].Replace(9, 1, "0");
    } else if (s == "3") {
      datetime[i - 1].Replace(9, 1, "2");
    } else if (s == "5") {
      datetime[i - 1].Replace(9, 1, "4");
    } else if (s == "7") {
      datetime[i - 1].Replace(9, 1, "6");
    } else if (s == "9") {
      datetime[i - 1].Replace(9, 1, "8");
    }

    rawdata[i - 1] = DataReader(stod(lat_raw[i]), stod(lon_raw[i]), stod(depth_raw[i]), stod(ML_raw[i]),
                                stod(nstn_raw[i]), stod(dmin_raw[i]), stod(gap_raw[i]), stod(trms_raw[i]),
                                stod(ERH_raw[i]), stod(ERZ_raw[i]), stod(nph_raw[i]), datetime[i - 1]);
  }

  return rawdata;
}


//int main()
void DataReader::ReadEQdata()
{
  vector<DataReader> rawdata;

  DataReader reader;
  reader.Init();
  rawdata = reader.ReadRawData();

  /*--debug--*/
  cout << rawdata.size() << endl;
  vector<DataReader> TypeConverter;

  for (int i = 0; i < rawdata.size(); i++) {
    DataReader &cand = rawdata[i];
    TypeConverter.push_back(cand);
    cout << TypeConverter[i].datetime_ << " ML " << i << " " << TypeConverter[i].ML_ << endl;
  }

	//open time file
    ifstream ifs2;
    ifs2.open("time_name.txt");
    vector<string> datetime_Rn;
    string         column;
    while (ifs2 >> column) {
      datetime_Rn.push_back(column);
    }

    vector<DataReader> data(10);
    data.resize(datetime_Rn.size());

    for (int rn = 0; rn < datetime_Rn.size(); rn++) {
      for (int i = 0; i < rawdata.size(); i++) {
        
        if (TypeConverter[i].datetime_ == datetime_Rn[rn]) {
          data[rn] = rawdata[i];
          cout<<TypeConverter[i].datetime_ <<" "<<datetime_Rn[rn]<<endl;
        }
      }
    }

    vector<DataReader> Test;
    for (int rn = 0; rn < datetime_Rn.size(); rn++) {
      DataReader &cand = data[rn];
      Test.push_back(cand);
    }
    for (int rn = 0; rn < datetime_Rn.size(); rn++) {
      cout << datetime_Rn[rn] << " ";
      cout << "ML " << rn << " " << Test[rn].ML_ << " " << Test[rn].depth_<<endl;
    }
//  return 0;
}
