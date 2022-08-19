#include <iostream>
#include <fstream>
#include <iomanip>

#include "../include/GeoData.h"
#include "../include/Constants.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TDatime.h"
#include "TTimeStamp.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TF1.h"
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
void GeoData::SetGeoData(string infileName, ifstream &timeInput)
{
  // input files and treat datetime
  TFile *infile = new TFile(("Data_Geophysical/output_root/" + infileName + ".root").c_str(), "READ");
  TTree *fChain = (TTree *)infile->Get("GeoData");

  LoadTree(fChain);
  TString name = infileName;
  name.Remove(3, 7);
  TString channel = infileName;
  channel.Remove(0, 4);
  channel.Remove(6, 18);

  // set up parameters for each sensors
  float const_sys = 0;
  double xbins = 0;
  if (channel.Contains("EHE")) {
    const_sys = constEHE;
    xbins = 0.001;
  } else if (channel.Contains("EHN")) {
    const_sys = constEHN;
    xbins = 0.001;
  } else if (channel.Contains("EHZ")) {
    const_sys = constEHZ;
    xbins = 0.01;
  } else if (channel.Contains("HH1")) {
    const_sys = constHH1;
    xbins = 0.001;
  } else if (channel.Contains("HH2")) {
    const_sys = constHH2;
    xbins = 0.001;
  } else if (channel.Contains("HHZ")) {
    const_sys = constHHZ;
    xbins = 0.001;
  } else if (channel.Contains("HN1")) {
    const_sys = constHN1;
    xbins = 0.001;
  } else if (channel.Contains("HN2")) {
    const_sys = constHN2;
    xbins = 0.001;
  } else if (channel.Contains("HNZ")) {
    const_sys = constHNZ;
    xbins = 0.001;
  }

  // set up datetime base on Rn data
  string column;
  if (!timeInput.is_open()) {
    cout << "Failed to open file" << endl;
  } else {
    while (timeInput >> column) {
      datetime_Rn.push_back(column);
    }
  }

  h_fitting            = new TH1D("h_fitting", "", 100, -xbins, xbins);

  /*---------------------set up basic waveform pattern----------------------*/
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
      data_collection.push_back(datatmp);
      data_avg += datatmp;
      datatmp = 0;
    }
  }
  data_avg = data_avg / data_collection.size();
  cout << "Avg " << data_avg << endl;
  data_collection.clear();
  ts_collection.clear();

  cout << "Processing remove mean..." << endl;
  for (Long64_t ientry = 0; ientry < nentries; ientry++) {
    fChain->GetEntry(ientry);
    if (ientry % 10000000 == 0)
      cout << ientry << " " << setprecision(3) << float(ientry) / float(nentries) * 100 << "%" << endl;
    if (ientry == 0) starttimestamp = timestamp;
    if (ientry > 0 && timestamp == timetmp) {
      if (abs(data) > abs(datatmp)) datatmp = data;
    } else {

      timetmp = timestamp;
      data_collection.push_back((datatmp - data_avg) * const_sys);
      ts_collection.push_back(timestamp);
      // h_fitting->Fill((datatmp - data_avg) * const_sys);
      datatmp = 0;
    }
  }
  timetmp = 0;
  datatmp = 0;

  data_avg = data_avg * const_sys;

  /*---------------------- analysis patterns -----------------------------*/

  // collect max data in 2 hr
  for (int i = 0; i < ts_collection.size(); i++) {
    if (i > 0 && timetmp >= ts_collection[i]) {
      if (abs(data_collection[i]) > abs(datatmp)) datatmp = data_collection[i];
    } else {
      timetmp = ts_collection[i] + 2 * 60 * 60;
      data2hr_collection.push_back(abs(datatmp)); // input data every second
      t2hr_collection.push_back(timetmp);
      h_fitting->Fill(datatmp);
      datatmp = 0.;
    }
  }

  // fitting
  h_fitting->Fit("gaus");
  double peak  = h_fitting->GetFunction("gaus")->GetParameter(1);
  double sigma = h_fitting->GetFunction("gaus")->GetParameter(2);
  cout << "peak " << peak << endl;
  cout << "sigma " << sigma << endl;

  // get # sigma for each sesnors
  for (int i = 0; i < t2hr_collection.size(); i++) {
    nsigma_collection.push_back(abs(data2hr_collection[i] - abs((float)peak)) / (float)sigma);
  }

  //matching time structure to Rn data
  double  N_[4000]; // date-time count
  double  matchingdata[4000];
  TString geodatetime[4000];
  for (int i = 0; i < datetime_Rn.size(); i++) {
    N_[i]           = 0;
    //geodatetime[i]  = "";
    matchingdata[i] = 0;
  }
  for (int j = 0; j < t2hr_collection.size(); j++) {
    geodatetime[j] = SetDatetime(t2hr_collection[j] + 8 * 60 * 60);
  }
  for (int i = 0; i < datetime_Rn.size(); i++) {
    N_[i] = (double)(i + 1) * 60 * 60 * 2 + 8 * 3600; // CWBSN UTC+8
    for (int j = 0; j < t2hr_collection.size(); j++) {
      if (datetime_Rn[i] == geodatetime[j]) {
        matchingdata[i] = nsigma_collection[j];
      } else {
        continue;
      }
    }
  }
  //  cout << "nsigma_collection.size() " << nsigma_collection.size() << endl;
  //  cout << "datetime_Rn.size() " << datetime_Rn.size() << endl;
  //  for (int i = 0; i < datetime_Rn.size(); i++) {
  //    cout << datetime_Rn[i] << " matchingdata " << matchingdata[i] << " data2hr ";
  //    cout << nsigma_collection[i] << " " << (int)t2hr_collection[i] << endl;
  //  }

  g_data         = new TGraph(data_collection.size(), &ts_collection[0], &data_collection[0]);
  g_2hrdata = new TGraph(data2hr_collection.size(), t2hr_collection.data(), data2hr_collection.data());
  g_nsigma = new TGraph(nsigma_collection.size(), t2hr_collection.data(), nsigma_collection.data());
  g_matchingdata = new TGraph(datetime_Rn.size(), N_, matchingdata);

  TFile *ofile = new TFile("Geodata_" + name + ".root", "update");
  ofile->mkdir((channel));
  ofile->cd(channel);
  h_fitting->Write();
  g_data->SetName("g_data");
  g_data->Write();
  g_matchingdata->SetName("g_matchingdata");
  g_matchingdata->Write();
  g_2hrdata->SetName("g_2hrdata");
  g_2hrdata->Write();
  g_nsigma->SetName("g_nsigma");
  g_nsigma->Write();

  ofile->Close();

  TDatime timeoffset = SetTimeOffset();
  DrawGeoData(infileName, channel, timeoffset);
}
