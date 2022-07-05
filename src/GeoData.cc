#include <iostream>
#include <fstream>
#include <iomanip>

#include "../include/GeoData.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TDatime.h"
#include "TTimeStamp.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TCanvas.h"
#include <string>
#include <vector>
#include "TStyle.h"

using namespace std;
void GeoData::Init(TTree *fChain)
{

  fChain->SetBranchAddress("data", &data, &b_data);
  fChain->SetBranchAddress("timestamp", &timestamp, &b_timestamp);
  fChain->SetBranchAddress("timestamp_ns", &timestamp_ns, &b_timestamp_ns);
}
//void GeoData::SetDatetime(vector<double> ts_collection, vector<double> data_collection)
//{
//  for (int i = 0; i < ts_collection.size(); i++) {
//    if (abs(data_collection[i]) > 10000) {
//
//      TTimeStamp t((int)ts_collection[i]);
//      TString    twohrstamp;
//
//      if ((t.GetTime() / 10000) != 0) {
//        twohrstamp.Form("%i%i", t.GetDate(), t.GetTime());
//      } else {
//        twohrstamp.Form("%i0%i", t.GetDate(), t.GetTime());
//      }
//      twohrstamp.Remove(10, 4);
//      TString s(twohrstamp(9, 10));
//      if (s == "1") {
//        twohrstamp.Replace(9, 1, "0");
//      } else if (s == "3") {
//        twohrstamp.Replace(9, 1, "2");
//      } else if (s == "5") {
//        twohrstamp.Replace(9, 1, "4");
//      } else if (s == "7") {
//        twohrstamp.Replace(9, 1, "6");
//      } else if (s == "9") {
//        twohrstamp.Replace(9, 1, "8");
//      }
//      //  cout << " : " << twohrstamp << endl;
//      data_binary.push_back(1);
//      ts_binary.push_back(ts_collection[i]);
//      datetime.push_back(twohrstamp);
//    }
//  }
//}
void GeoData::Combine(string infileName, ifstream &timeInput)
{
  TFile *ofile = new TFile("test.root", "update");
  ofile->mkdir((infileName).c_str());
  TFile *infile = new TFile(("Data_Geophysical/output_root/" + infileName + ".root").c_str(), "READ");
  TTree *fChain = (TTree *)infile->Get("GeoData");

  Init(fChain);

  TString name    = infileName;
  TString channel = infileName;
  channel.Remove(0, 7);
  channel.Remove(3, 23);

  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();

  float           datatmp        = 0;
  Long64_t        timetmp        = 0;
  double          starttimestamp = 0;
  for (Long64_t ientry = 0; ientry < nentries; ientry++) {

    fChain->GetEntry(ientry);
    if (ientry % 10000000 == 0)
      cout << ientry << " " << setprecision(3) << float(ientry) / float(nentries) * 100 << "%" << endl;

    if (ientry == 0) starttimestamp = timestamp;
    if (ientry > 0 && timestamp == timetmp) {
      if (abs(data) > abs(datatmp)) datatmp = data;
    } else {
      timetmp = timestamp;
      data_collection.push_back(datatmp); // input data every second
      ts_collection.push_back(timestamp);
      datatmp = 0;
    }
  }

  string column;
  if (!timeInput.is_open()) {
    cout << "Failed to open file" << endl;
  } else {
    while (timeInput >> column) {
      datetime_Rn.push_back(column);
    }
  }
//  SetDatetime(ts_collection, data_collection);

//  double combine[datetime_Rn.size()];
//  double N_[4000]; // for time
//  for (int i = 0; i < datetime_Rn.size(); i++) {
//    combine[i] = 0;
//    N_[i]      = 0;
//  }
//  for (int i = 0; i < datetime_Rn.size(); i++) {
//    // since "combine" came from CWBSN, the time (in x-axis) must +8
//    N_[i] = (double)(i + 1) * 60 * 60 * 2 + 8 * 3600; // CWBSN UTC+8
//    for (int t = 0; t < datetime.size(); t++) {
//      if (datetime_Rn[i] == datetime[t]) {
//        combine[i] = 1;
//      } else
//        continue;
//    }
//  }
//
//  for (int i = 0; i < datetime_Rn.size(); i++) {
//    cout << "combine " << combine[i] << " ";
//    cout << datetime_Rn[i] << endl;
//  }
//
//  g_data       = new TGraph(data_collection.size(), &ts_collection[0], &data_collection[0]);
//  g_binarydata = new TGraph(datetime_Rn.size(), N_, combine);
//  cout << "1st timestamp " << int(ts_collection[0]) << " " << starttimestamp << endl;
//  ofile->cd((infileName).c_str());
//  g_data->SetName("g_data");
//  g_data->Write();
//  g_binarydata->SetName("g_binarydata");
//  g_binarydata->Write();
//  ofile->Close();
//
//  TDatime timeoffset(20210915, 0);
//  DrawGeoData(name, channel, timeoffset);
}
