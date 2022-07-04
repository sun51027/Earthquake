#include <iostream>
#include <iomanip>

#include "../include/GeoData.h"
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

//int main(string infileName)
void GeoData::Combine(string infileName)
{

  TFile *infile = new TFile(("Data_Geophysical/output_root/"+infileName+".root").c_str(), "READ");
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
  vector<float> binarydata;
  float              datatmp = 0;
  Long64_t           timetmp = 0;
  int                n       = 0;
	

  TString name = infileName;
	TString channel = infileName;
//  name.Remove(0,29);
//  name.Remove(28,5);
	channel.Remove(0,7);
	channel.Remove(3,23);

  if (fChain == 0) return;
  Show(1);
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
      if(abs(datatmp) > 10000) binarydata.push_back(1);
      else binarydata.push_back(0);
      ts_collection.push_back(timestamp);
      datatmp = 0;
    }

    // if (Cut(ientry) < 0) continue;
  }
  TDatime da(1970, 1,1, 00, 00, 00);
  gStyle->SetTimeOffset(da.Convert());
//
  TCanvas *c     = new TCanvas("c", "", 1200, 600);
  TGraph *gdata = new TGraph(data_collection.size(), ts_collection.data(), data_collection.data());
  gdata->SetName("gdata");
  gdata->Draw("AL");
  gdata->SetTitle("");
  gdata->GetXaxis()->SetTimeDisplay(1);
  gdata->GetXaxis()->SetTimeOffset(0);
	gdata->GetXaxis()->SetTimeFormat("%m/%d %Hh");
	gdata->GetXaxis()->SetTitle("Date time (mm/dd/hh)");
 	gdata->GetYaxis()->SetTitle("Data ("+channel+")");	
  c->SetGrid(1,0);
	c->Modified();
  c->SaveAs(name+".png");
  delete c;

  TCanvas *c2     = new TCanvas("c2", "", 1200, 600);
  TGraph *g_binarydata = new TGraph(binarydata.size(), ts_collection.data(), binarydata.data());
  g_binarydata->SetName("g_binarydata");
  g_binarydata->Draw("AP");
  g_binarydata->SetMarkerStyle(20);
  g_binarydata->SetTitle("");
  g_binarydata->GetXaxis()->SetTimeDisplay(1);
  g_binarydata->GetXaxis()->SetTimeOffset(0);
	g_binarydata->GetXaxis()->SetTimeFormat("%m/%d %Hh");
	g_binarydata->GetXaxis()->SetTitle("Date time (mm/dd/hh)");
 	g_binarydata->GetYaxis()->SetTitle("Data ("+channel+")");	
  c2->SetGrid(1,0);
	c2->Modified();
  c2->SaveAs(name+"_binary.pdf");

     TFile* f = new TFile("test.root","RECREATE");
     gdata->Write();
      g_binarydata->Write();
     f->Write();
     f->Close();
}
