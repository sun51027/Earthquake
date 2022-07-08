//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Apr  8 12:36:12 2022 by ROOT version 6.22/08
// from TTree GeoData/GeoData
// found on file: output_root/HWA_00_EHE_20211001_20211229.root
//////////////////////////////////////////////////////////

#ifndef GEODATA_h
#define GEODATA_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <string>
#include <vector>
#include "TGraph.h"
#include "TDatime.h"
#include "TTimeStamp.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
// Header file for the classes stored in the TTree if any.

class GeoData {
public:
  GeoData()
  {
    //    data_collection.clear();
    //    ts_collection.clear();
  }
  ~GeoData()
  {
    //    delete[] & data_collection;
    //    delete[] & ts_collection;
  }
  TTree *fChain; //! pointer to the analyzed TTree or TChain

  // Fixed size dimensions of array or collections stored in the TTree if any.

  // Declaration of leaf types
  float data;
  Int_t timestamp;
  int   timestamp_ns;

  // List of branches
  TBranch *b_data;         //!
  TBranch *b_timestamp;    //!
  TBranch *b_timestamp_ns; //!
  void     SetGeoData(string infile, ifstream &timeInput);
  void     DrawGeoData(TString name, TString ch, TDatime timeoffset);
  TString SetDatetime(TTimeStamp t);
  TDatime SetTimeOffset();
  void    LoadTree(TTree *tree);
  void    Cut();

  vector<TString> datetime;
  vector<TString> datetime_Rn;
  vector<double>  data_binary;
  vector<double>  ts_binary;
  vector<float>   data_collection;
  vector<float>   ts_collection;
  TGraph         *g_data;
  TGraph         *g_binarydata;
};

#endif
