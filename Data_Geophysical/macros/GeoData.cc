#include "GeoData.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TDatime.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TCanvas.h"
#include <string>
#include <vector>
#include "TStyle.h"

using namespace std;

int main(string infileName)
//void GeoData(string infileName)
{

  TFile *infile = new TFile(infileName.c_str(), "READ");
  TTree *fChain = (TTree *)infile->Get("GeoData");
  // Declaration of leaf types
  Float_t data;
  Int_t   timestamp;
  Int_t   timestamp_ns;
  // List of branches
  TBranch *b_data;         //!
  TBranch *b_timestamp;    //!
  TBranch *b_timestamp_ns; //!
  fChain->SetBranchAddress("data", &data, &b_data);
  fChain->SetBranchAddress("timestamp", &timestamp, &b_timestamp);
  fChain->SetBranchAddress("timestamp_ns", &timestamp_ns, &b_timestamp_ns);

  vector<float> data_collection;
  vector<float> ts_collection;
  float              datatmp = 0;
  Long64_t           timetmp = 0;
  int                n       = 0;
	

  TString name = infileName;
	TString channel = infileName;
  name.Remove(0,12);
  name.Remove(28,5);
	channel.Remove(0,19);
	channel.Remove(3,23);

  if (fChain == 0) return;
GeoData geodata;
  geodata.Show(1);
  Long64_t nentries = fChain->GetEntriesFast();

  for (Long64_t ientry = 0; ientry < nentries; ientry++) {

    fChain->GetEntry(ientry);
    if (ientry % 10000000 == 0)
      cout << ientry << " " << setprecision(3) << float(ientry) / float(nentries) * 100 << "%" << endl;


    if (ientry > 0 && timestamp == timetmp) {
      if (abs(data) > abs(datatmp)) datatmp = data;
    } else {

      // include ientry = 0, that is in first entry we set timetmp = timestamp
      timetmp = timestamp;
      data_collection.push_back(datatmp); // input data every second
      ts_collection.push_back(timestamp);
      datatmp = 0;
    }

    // if (Cut(ientry) < 0) continue;
  }
  TDatime da(1970, 1,1, 00, 00, 00);
  gStyle->SetTimeOffset(da.Convert());
//
//  TCanvas *c     = new TCanvas("c", "", 1200, 600);
//  TGraph *gdata = new TGraph(data_collection.size(), ts_collection.data(), data_collection.data());
//  gdata->Draw("AL");
//  gdata->SetTitle("");
//  gdata->GetXaxis()->SetTimeDisplay(1);
//  gdata->GetXaxis()->SetTimeOffset(0);
//	gdata->GetXaxis()->SetTimeFormat("%m/%d %Hh");
//	gdata->GetXaxis()->SetTitle("Date time (mm/dd/hh)");
// 	gdata->GetYaxis()->SetTitle("Data ("+channel+")");	
//  c->SetGrid(1,0);
//	c->Modified();
//  c->SaveAs("plot/"+name+".png");

  //   TFile* f = new TFile("test.root","RECREATE");
  //   gdata->Write();
  //   f->Write();
  //   f->Close();
    return;
}
