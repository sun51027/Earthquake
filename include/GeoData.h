//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Apr  8 12:36:12 2022 by ROOT version 6.22/08
// from TTree GeoData/GeoData
// found on file: output_root/HWA_00_EHE_20211001_20211229.root
//////////////////////////////////////////////////////////

#ifndef GeoData_h
#define GeoData_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <string>
#include <vector>
#include "TGraph.h"
#include "TDatime.h"
using namespace std;
// Header file for the classes stored in the TTree if any.

class GeoData {
public:
  TTree *fChain;   //! pointer to the analyzed TTree or TChain
  Int_t  fCurrent; //! current Tree number in a TChain

  // Fixed size dimensions of array or collections stored in the TTree if any.

  // Declaration of leaf types
  Float_t data;
  Int_t   timestamp;
  Int_t   timestamp_ns;

  // List of branches
  TBranch *b_data;         //!
  TBranch *b_timestamp;    //!
  TBranch *b_timestamp_ns; //!
  GeoData(){};
  ~GeoData(){};
  void Combine(string infile, ifstream &timeInput);
  void DrawGeoData(TString name, TString ch, TDatime timeoffset);

  void SetDatetime(vector<double> t, vector<double> data);

  Int_t    Cut(Long64_t entry);
  Int_t    GetEntry(Long64_t entry);
  Long64_t LoadTree(Long64_t entry);
  void     Init(TTree *tree);
  void     Loop();
  Bool_t   Notify();
  void     Show(Long64_t entry = -1);

  vector<TString> datetime;
  TGraph         *g_data;
  TGraph         *g_binarydata;
  vector<TString> datetime_Rn;
  vector<double>  data_binary;
  vector<double>  ts_binary;
  vector<double>  data_collection;
  vector<double>  ts_collection;
};

#endif
