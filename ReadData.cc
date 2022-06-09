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
#include "interface/EQ.h"

// void DataReader::EarthquakeDirectory(){

void DataReader::Init(ifstream &eqDirInput)
{
  string c1, c2, c13, c15, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c14;

  rawdata.resize(64);
  if (!eqDirInput.is_open()) {
    cout << "Failed to open file" << endl;
  } else {
    while (eqDirInput >> c1 >> c2 >> c3 >> c4 >> c5 >> c6 >> c7 >> c8 >> c9 >> c10 >> c11 >> c12 >> c13 >> c14 >> c15) {
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

    // combine date and time into datatime to match Rn data array
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

void DataReader::ReadEQdata(ifstream &eqDirInput, ifstream &timeInput, TFile *ofile)
{
  // Initialize rawdata from EQ directory (CWB)
  vector<DataReader> rawdata;
  DataReader         reader;
  reader.Init(eqDirInput);
  rawdata = reader.ReadRawData();

  // open timeInput
  string column;
  if (!timeInput.is_open()) {
    cout << "Failed to open file" << endl;
  } else {
    while (timeInput >> column) {
      datetime_Rn.push_back(column);
    }
  }

  vector<DataReader> Rndata;
  Rndata.resize(datetime_Rn.size());
  cout << "Rndata " << Rndata.size() << endl;
  cout << "rawdata " << rawdata.size() << endl;

  for (int rn = 0; rn < datetime_Rn.size(); rn++) {
          Rndata[rn].ML_ =0;
          Rndata[rn].depth_ =0;
					cout<<"Rndata["<<rn<<"] "<<Rndata[rn].datetime_<<" "<<Rndata[rn].ML_<<endl;
  }
  cout<<"after equal"<<endl;
  double N_[4000];
  for (int rn = 0; rn < datetime_Rn.size(); rn++) {

    N_[rn] = (double)(rn + 1) * 60 * 60 * 2; // number of 2hour
		
    for (int i = 0; i < rawdata.size(); i++) {

      if (rawdata[i].datetime_ == datetime_Rn[rn]) {
        Rndata[rn] = rawdata[i];
        cout << rawdata[i].datetime_ << " " << datetime_Rn[rn] << " "<<Rndata[rn].datetime_<<endl;
      }
			else continue;
//        cout << rawdata[i].ML_<< " " << datetime_Rn[rn] << " Rndata["<<rn<<"] "<<Rndata[rn].ML_<<endl;
    }
  }

  double ML[4000];
  for (int rn = 0; rn < datetime_Rn.size(); rn++) {
					cout<<"Rndata["<<rn<<"] "<<Rndata[rn].datetime_<<" "<<Rndata[rn].ML_<<" "<<Rndata[rn].depth_<<endl;
    ML[rn] = Rndata[rn].ML_;
  }

  TGraph *g_ML;
  g_ML = new TGraph(datetime_Rn.size(), N_, ML);
  ofile->cd("EQ_directory");
  g_ML->SetName("g_ML");
  g_ML->Write();
  //  return 0;
}
