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
void GeoData::Cut(int sigma, int peak)
{

  // take the largest data in 2hr
  vector<float> data2hr_collection;
  vector<float> t2hr_collection;
  float         datatmp        = 0;
  float         timetmp        = 0;
  int           starttimestamp = 0;
  for (int i = 0; i < ts_collection.size(); i++) {
    if (i > 0 && timetmp >= ts_collection[i]) {
      if (abs(data_collection[i]) > abs(datatmp)) datatmp = data_collection[i];
    } else {
      timetmp = ts_collection[i] + 2 * 60 * 60;
      data2hr_collection.push_back(abs(datatmp)); // input data every second
      t2hr_collection.push_back(timetmp);
      datatmp = 0.;
    }
  }
  for (int i = 0; i < data2hr_collection.size(); i++) {

    cout << "data2hr_collection " << data2hr_collection[i] << endl;
    cout << "t2hr_collection " << t2hr_collection[i] << endl;
  }
  cout << t2hr_collection.size() << " " << data2hr_collection.size() << endl;
  g_2hrdata = new TGraph(data2hr_collection.size(), t2hr_collection.data(), data2hr_collection.data());

  // take a data > # sigma within 2 hr
  //  double threshold = sigma;
  //    if (abs(data_collection[i] - peak) > threshold) {
  //          cout<<data_collection[i]<<" - "<<peak<<" = "<<abs(data_collection[i]-peak)<<" > ";
  //          cout<<threshold<<endl;
  //      TTimeStamp t((int)ts_collection[i]);
  //      TString    twohrstamp = SetDatetime(t);
  //      geodatetime.push_back(twohrstamp);
  //    }else{
  //          cout<<data_collection[i]<<" - "<<peak<<" = "<<abs(data_collection[i]-peak)<<" < ";
  //          cout<<threshold<<endl;
  //    }
  //  }
}
void GeoData::SetGeoData(string infileName, ifstream &timeInput)
// void GeoData::SetGeoData(string infileName, ifstream &timeInput, int threshold)
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

  float const_sys = 0;
  if (channel.Contains("EHE")) {
    const_sys = constEHE;
  } else if (channel.Contains("EHN")) {
    const_sys = constEHN;
  } else if (channel.Contains("EHZ")) {
    const_sys = constEHZ;
  } else if (channel.Contains("HH1")) {
    const_sys = constHH1;
  } else if (channel.Contains("HH2")) {
    const_sys = constHH2;
  } else if (channel.Contains("HHZ")) {
    const_sys = constHHZ;
  } else if (channel.Contains("HN1")) {
    const_sys = constHN1;
  } else if (channel.Contains("HN2")) {
    const_sys = constHN2;
  } else if (channel.Contains("HNZ")) {
    const_sys = constHNZ;
  }
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

  h_fitting            = new TH1D("h_fitting", "", 100, -100000, 100000);
  float datatmp        = 0;
  Int_t timetmp        = 0;
  int   starttimestamp = 0;
  float data_avg       = 0;
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
                                          //      ts_collection.push_back(timestamp);
                                          //      h_fitting->Fill(datatmp);
      data_avg += datatmp;
      datatmp = 0;
    }
  }
  timetmp  = 0;
  datatmp  = 0;
  data_avg = data_avg / data_collection.size();
  cout << "Avg " << data_avg << endl;
  data_collection.clear();
  ts_collection.clear();

  cout << "remove mean" << endl;
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
      h_fitting->Fill(datatmp);
      datatmp = 0;
    }
  }

  h_fitting->Fit("gaus");
  double peak  = h_fitting->GetFunction("gaus")->GetParameter(1);
  double sigma = h_fitting->GetFunction("gaus")->GetParameter(2);
  cout << "peak " << peak << endl;
  cout << "sigma " << sigma << endl;

  Cut(sigma, peak);

  //  double combineIn2hr[datetime_Rn.size()];
  //  double N_[4000];
  //  for (int i = 0; i < datetime_Rn.size(); i++) {
  //    combineIn2hr[i] = 0;
  //    N_[i]           = 0;
  //  }
  //  for (int i = 0; i < datetime_Rn.size(); i++) {
  //
  //    // since "combineIn2hr" came from CWBSN, the time (in x-axis) must +8
  //    N_[i] = (double)(i + 1) * 60 * 60 * 2 + 8 * 3600; // CWBSN UTC+8
  //    for (int t = 0; t < geodatetime.size(); t++) {
  //      if (datetime_Rn[i] == geodatetime[t]) {
  //        combineIn2hr[i] = 1;
  //      } else
  //        continue;
  //    }
  //  }

  //  for (int i = 0; i < datetime_Rn.size(); i++) {
  //    cout << "combineIn2hr " << combineIn2hr[i] << " ";
  //    cout << datetime_Rn[i] << endl;
  //  }

  g_data = new TGraph(data_collection.size(), &ts_collection[0], &data_collection[0]);
  // g_binarydata = new TGraph(datetime_Rn.size(), N_, combineIn2hr);

  TFile *ofile = new TFile("Geodata_" + name + ".root", "update");
  ofile->mkdir((channel));
  ofile->cd(channel);
  h_fitting->Write();
  g_data->SetName("g_data");
  g_data->Write();
  //  g_binarydata->SetName("g_binarydata");
  //  g_binarydata->Write();
  g_2hrdata->SetName("g_2hrdata");
  g_2hrdata->Write();

  ofile->Close();

  TDatime timeoffset = SetTimeOffset();
  DrawGeoData(infileName, channel, timeoffset);
}
