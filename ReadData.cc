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

#include "interface/DataReader.h"

void DataReader::Init(ifstream &eqDirInput)
{
  string c1, c2, c13, c15, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c14;

  //  rawdata.resize(64);
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

    TTimeStamp test(date[i].Atoi(), time[i].Atoi(), 0, kTRUE, 8 * 3600);
    if ((test.GetTime() / 100000) != 0) {
      datetime[i].Form("%i%i", test.GetDate(), test.GetTime());
    } else {
      datetime[i].Form("%i0%i", test.GetDate(), test.GetTime());
    }
    datetime[i].Remove(10, 4);
    //						cout<<"datetime["<<i<<"] "<<datetime[i]<<endl;
    // combine date and time into datatime to match Rn data array
    TString s(datetime[i](9, 10));
    if (s == "1") {
      datetime[i].Replace(9, 1, "0");
    } else if (s == "3") {
      datetime[i].Replace(9, 1, "2");
    } else if (s == "5") {
      datetime[i].Replace(9, 1, "4");
    } else if (s == "7") {
      datetime[i].Replace(9, 1, "6");
    } else if (s == "9") {
      datetime[i].Replace(9, 1, "8");
    }

    // rawdata begin with i = 0
    rawdata[i - 1] = DataReader(date[i].Atoi(), time[i].Atoi(), stod(lat_raw[i]), stod(lon_raw[i]), stod(depth_raw[i]),
                                stod(ML_raw[i]), stod(nstn_raw[i]), stod(dmin_raw[i]), stod(gap_raw[i]),
                                stod(trms_raw[i]), stod(ERH_raw[i]), stod(ERZ_raw[i]), stod(nph_raw[i]), datetime[i]);
  }
  return rawdata;
}

void DataReader::ReadEQdata(ifstream &eqDirInput, ifstream &timeInput, TFile *ofile)
{
  // open timeInput
  string column;
  if (!timeInput.is_open()) {
    cout << "Failed to open file" << endl;
  } else {
    while (timeInput >> column) {
      datetime_Rn.push_back(column);
    }
  }
  // Initialize rawdata from EQ directory (CWB) and
  // create newArray to match match the date-time of Rn
  // to the EQ directory
  //
  vector<DataReader> newArray;
  TString            date_Rn[datetime_Rn.size()];
  TString            time_Rn[datetime_Rn.size()];
  newArray.resize(datetime_Rn.size());
  for (int rn = 0; rn < datetime_Rn.size(); rn++) {
    newArray[rn].ML_    = 0;
    newArray[rn].depth_ = 0;
    date_Rn[rn]         = datetime_Rn[rn];
    date_Rn[rn].Remove(8, 2);
    time_Rn[rn] = datetime_Rn[rn];
    time_Rn[rn].Remove(0, 8);
    time_Rn[rn].Insert(2, "0000");
  }


  DataReader reader;
  reader.Init(eqDirInput);
  rawdata = reader.ReadRawData();
  timeoffset.Set(date_Rn[0].Atoi(), time_Rn[0].Atoi());

  //  Match the two arrays

  double N_[4000]; // for time
  double MLtmp = 0;
  double ntmp  = 0;
  for (int rn = 0; rn < datetime_Rn.size(); rn++) {

    N_[rn] = (double)(rn + 1) * 60 * 60 * 2 - 8 * 3600; // number of 2hour

    for (int i = 0; i < rawdata.size(); i++) {

      if (rawdata[i].datetime_ == datetime_Rn[rn]) {
        if (rawdata[i].ML_ > MLtmp) {
          MLtmp = rawdata[i].ML_;
          ntmp  = i;
        }
        newArray[rn] = rawdata[ntmp];
        // cout << ntmp << " " << rawdata[i].datetime_ << " ML[" << i << "] " << rawdata[i].ML_ << " Rndatetime[" << rn
        //      << "] " << datetime_Rn[rn] << " " << endl;
      } else {
        MLtmp = 0;
        continue;
      }
    }
  }
  for (int rn = 0; rn < datetime_Rn.size(); rn++) {
    // cout << "newArray[" << rn << "] " << newArray[rn].datetime_ << " " << newArray[rn].ML_ << " " <<
    // newArray[rn].depth_
    //      << endl;
    if (newArray[rn].ML_ >= 4 && newArray[rn].depth_ <= 50) {
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

