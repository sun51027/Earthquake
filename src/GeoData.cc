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
void GeoData::LoadTree(TTree *fChain)
{
  fChain->SetBranchAddress("data", &data, &b_data);
  fChain->SetBranchAddress("timestamp", &timestamp, &b_timestamp);
  fChain->SetBranchAddress("timestamp_ns", &timestamp_ns, &b_timestamp_ns);
}
void GeoData::Cut()
{
  for (int i = 0; i < ts_collection.size(); i++) {
    if (abs(data_collection[i]) > 10000) {

      TTimeStamp t((int)ts_collection[i]);
      TString    twohrstamp = SetDatetime(t);
      geodatetime.push_back(twohrstamp);
    }
  }
}
void GeoData::SetGeoData(string infileName, ifstream &timeInput)
{
  // input files and treat datetime
  TFile *infile = new TFile(("Data_Geophysical/output_root/" + infileName + ".root").c_str(), "READ");
  TTree *fChain = (TTree *)infile->Get("GeoData");

  LoadTree(fChain);
  TString name    = infileName;
  name.Remove(3,7);
  TString channel = infileName;
  channel.Remove(0, 4);
  channel.Remove(6, 18);

  string column;
  if (!timeInput.is_open()) {
    cout << "Failed to open file" << endl;
  } else {
    while (timeInput >> column) {
      datetime_Rn.push_back(column);
    }
  }

  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();

  float datatmp        = 0;
  Int_t timetmp        = 0;
  int   starttimestamp = 0;
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

  Cut();



  double combineIn2hr[datetime_Rn.size()];
  double N_[4000];
  for (int i = 0; i < datetime_Rn.size(); i++) {
    combineIn2hr[i] = 0;
    N_[i]      = 0;
  }
  for (int i = 0; i < datetime_Rn.size(); i++) {

    // since "combineIn2hr" came from CWBSN, the time (in x-axis) must +8
    N_[i] = (double)(i + 1) * 60 * 60 * 2 + 8 * 3600; // CWBSN UTC+8
    for (int t = 0; t < geodatetime.size(); t++) {
      if (datetime_Rn[i] == geodatetime[t]) {
        combineIn2hr[i] = 1;
      } else
        continue;
    }
  }

//  for (int i = 0; i < datetime_Rn.size(); i++) {
//    cout << "combineIn2hr " << combineIn2hr[i] << " ";
//    cout << datetime_Rn[i] << endl;
//  }

  g_data       = new TGraph(data_collection.size(), &ts_collection[0], &data_collection[0]);
  g_binarydata = new TGraph(datetime_Rn.size(), N_, combineIn2hr);

  TFile *ofile = new TFile("Geodata_"+name+".root", "update");
  ofile->mkdir((channel));
  ofile->cd(channel);
  g_data->SetName("g_data");
  g_data->Write();
  g_binarydata->SetName("g_binarydata");
  g_binarydata->Write();
  ofile->Close();
  
  TDatime timeoffset = SetTimeOffset();
  DrawGeoData(infileName, channel, timeoffset);
}
