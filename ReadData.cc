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

//#include "interface/DataReader.h"

// void DataReader::EarthquakeDirectory(){

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
  //return 0;
}
