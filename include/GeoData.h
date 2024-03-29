//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Apr  8 12:36:12 2022 by ROOT version 6.22/08
// from TTree GeoData/GeoData
// found on file: output_root/HWA_00_EHE_20211001_20211229.root
//////////////////////////////////////////////////////////

#ifndef GEODATA_h
#define GEODATA_h

#include "../include/DataReader.h"

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <string>
#include <vector>
#include "TGraph.h"
#include "TH1D.h"
#include "TDatime.h"
#include "TTimeStamp.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

class GeoData : public DataReader {
public:
  GeoData() {}
  ~GeoData() {}
  TTree *fChain;
  float  data;
  Int_t  timestamp;
  int    timestamp_ns;

  // List of branches
  TBranch *b_data;
  TBranch *b_timestamp;
  TBranch *b_timestamp_ns;
  void     GeoDataAnalyzer(string infile);
  void     DrawGeoData(TString name, TString ch, TDatime timeoffset);
  void     LoadTree(TTree *tree);
  void     Cut(double sigma, double peak);
  float   *TimeMatching();

private:
  //  vector<TString> geodatetime;
  vector<float> data_collection;
  vector<float> ts_collection;
  vector<float> data2hr_collection;
  vector<float> t2hr_collection;
  vector<float> nsigma_collection;
  TGraph       *g_data;
  TGraph       *g_matchingdata;
  TGraph       *g_2hrdata;
  TGraph       *g_nsigma;
  TH1D         *h_fitting;
  float         data_avg = 0;
  float         matchingdata[4000];
  float         nDays[4000];
};

#endif
