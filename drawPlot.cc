// STL
using namespace std;
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
// my header
#include "interface/EQ.h"

// ROOT include
#include "TCanvas.h"
#include "TDirectory.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TPad.h"
#include "TLegend.h"
#include "TH1D.h"
#include "TKey.h"
#include "TStyle.h"
#include "rootlogon.h"
// using namespace mgr;

void drawPlot()
{

  // load root
  TFile      *fin = new TFile("oAnalyzer.root");
  TDirectory *dir = (TDirectory *)fin->Get("Analysis_plot");
  dir->cd();

  // load plot_name.txt
  ifstream ifs;
  ifs.open("plot_name.txt", std::ios::in);
  vector<TString> time;
  if (!ifs.is_open()) {
    cout << "fail to open file." << endl;
  } else {
    while (!ifs.eof()) {
      string s;
      getline(ifs, s);
      time.push_back(s);
    }
  }
  ifs.close();

  TCanvas *c = new TCanvas("canvas", "", 800, 600);

  // difference vs time
  TGraph *gr = (TGraph *)dir->Get("g_diffvsTime");
  gr->GetXaxis()->SetLimits(0, time.size());
  for (int i = 0; i <= time.size() / 60; i++) {
    gr->GetXaxis()->SetBinLabel(i * 60 + 1, time[i * 60]);
  }
  gr->SetTitle("");
  gr->Draw("AP");
  c->SaveAs("DiffvsTime.pdf");

  //   Correlation factor
  TGraph *corr = (TGraph *)dir->Get("g_cfactor");
  corr->SetTitle("");
  corr->Draw("AP");
  corr->GetXaxis()->SetLimits(0, time.size());
  corr->GetYaxis()->SetTitle("Calibration factor");
  c->SetGridy(1);
  c->SaveAs("cfactor.pdf");
}
