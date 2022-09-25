//
//	read earthquake directory from CWB
//	deal with eq raw data to match the form of Rn data
//	create: Lin Shih
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
#include "TDirectory.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TPad.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TH1D.h"
#include "TKey.h"
#include "TStyle.h"
#include "TTimeStamp.h"

#include "../include/DataReader.h"

void DataReader::Init(ifstream &eqDirInput)
{
  string c1, c2, c13, c15, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c14;

  if (!eqDirInput.is_open()) {
    cout << "Failed to open file" << endl;
  } else {
    cout << "open file " << endl;
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
  rawdata.resize(date_raw.size());
}
vector<DataReader> DataReader::ReadRawData()
{

  date.resize(lat_raw.size());
  time.resize(lat_raw.size());
  datetime.resize(lat_raw.size());

  for (int i = 1; i < lat_raw.size(); i++) {
    time[i] = time_raw[i];
    time[i].Remove(2, 1);
    time[i].Remove(4, 1);
    time[i].Remove(6, 3);
    date[i] = date_raw[i];
    date[i].Remove(4, 1);
    date[i].Remove(6, 1);

    // EQdir time must be UTC+8
    TTimeStamp t(date[i].Atoi(), time[i].Atoi(), 0, kTRUE, 8 * 60 * 60);
    datetime[i] = SetDatetime(t);

    // rawdata begin with i = 0
    rawdata[i - 1] = DataReader(date[i].Atoi(), time[i].Atoi(), stod(lat_raw[i]), stod(lon_raw[i]), stod(depth_raw[i]),
                                stod(ML_raw[i]), stod(nstn_raw[i]), stod(dmin_raw[i]), stod(gap_raw[i]),
                                stod(trms_raw[i]), stod(ERH_raw[i]), stod(ERZ_raw[i]), stod(nph_raw[i]), datetime[i]);
  }
  return rawdata;
}

void DataReader::SetEQdata(ifstream &eqDirInput, TFile *ofile)
{

  // Initialize rawdata from EQ directory (CWB) and
  // create newArray to match match the date-time of Rn
  // to the EQ directory
  //
  vector<DataReader> newArray;
  newArray.resize(datetime_Rn.size());
  for (int rn = 0; rn < datetime_Rn.size(); rn++) {
    newArray[rn].ML_    = 0;
    newArray[rn].depth_ = 0;
  }
  timeoffset = SetTimeOffset();
  timeoffset.Print();

  DataReader reader;
  reader.Init(eqDirInput);
  rawdata = reader.ReadRawData();

  double N_[4000]; // for time
  double MLtmp = 0;
  double ntmp  = 0;
  for (int rn = 0; rn < datetime_Rn.size(); rn++) {

    N_[rn] = (double)(rn + 1) * 60 * 60 * 2; // number of 2hour

    for (int i = 0; i < rawdata.size(); i++) {

      if (rawdata[i].datetime_ == datetime_Rn[rn]) {
        if (rawdata[i].ML_ > MLtmp) {
          MLtmp = rawdata[i].ML_;
          ntmp  = i;
        }
        newArray[rn] = rawdata[ntmp];
      } else {
        MLtmp = 0;
        continue;
      }
    }
  }
  for (int rn = 0; rn < datetime_Rn.size(); rn++) {
    if (newArray[rn].datetime_ != "") {
      cout << "newArray[" << rn << "] " << newArray[rn].datetime_ << " " << newArray[rn].ML_ << " "
           << newArray[rn].depth_ << endl;
    }
    //  if (newArray[rn].ML_ >= 4) {
    if (newArray[rn].ML_ >= 0.5 && newArray[rn].depth_ <= 35) {
      ML[rn]    = newArray[rn].ML_;
      depth[rn] = 0. - newArray[rn].depth_;
    }
  }

  // store into root file
  ofile->cd("EQ_directory");

  g_ML = new TGraph(datetime_Rn.size(), N_, ML);
  g_ML->SetName("g_ML");
  g_ML->Write();

  g_depth = new TGraph(datetime_Rn.size(), N_, depth);
  g_depth->SetName("g_depth");
  g_depth->Write();
}
