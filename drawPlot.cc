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
using namespace mgr;

void Earthquake::DrawPlot()
{

  // load root
  TFile      *fin = new TFile("oAnalyzer.root");
  TDirectory *dir = (TDirectory *)fin->Get("Analysis_plot");
  dir->cd();

  // load plot_name.txt
  ifstream ifs;
  ifs.open("time_name.txt", std::ios::in);
  vector<TString> time;
  if (!ifs.is_open()) {
    cout << "fail to open file." << endl;
  } else {
    while (!ifs.eof()) {
      string s;
      getline(ifs, s);
      time.push_back(s); // 2021122500
    }
  }
  ifs.close();
  int N = time.size();
  for (int i = 0; i < N; i++) {
    time[i].Remove(8, 2);
    time[i].Remove(0, 4);
    time[i].Insert(2, "/");
  }

  TCanvas *c = new TCanvas("canvas", "", 800, 600);

  // difference vs time
  TGraph *g_diffvsTime = (TGraph *)dir->Get("g_diffvsTime");
  g_diffvsTime->GetXaxis()->SetLimits(0, N);
  for (int i = 0; i <= N / 60; i++) {
    g_diffvsTime->GetXaxis()->SetBinLabel(i * 60 + 1, time[i * 60]);
  }
  g_diffvsTime->SetTitle("");
  g_diffvsTime->Draw("AP");
  c->SaveAs("DiffvsTime.pdf");

  //   Correlation factor
  TGraph *g_cfactor = (TGraph *)dir->Get("g_cfactor");
  g_cfactor->SetTitle("");
  g_cfactor->Draw("AP");
  g_cfactor->GetXaxis()->SetLimits(0, N);
  g_cfactor->GetYaxis()->SetTitle("Calibration factor");
  c->SetGridy(1);
  c->SaveAs("cfactor.pdf");

  // K40 stability
  TCanvas     *c3 = new TCanvas("c3", "", 10, 10, 1500, 900);
  TPad        *pL = mgr::NewLeftPad();
  TPad        *pR = mgr::NewRightPad();
  TMultiGraph *mg = new TMultiGraph();

  TGraph *g_K40_peak_cali   = (TGraph *)dir->Get("g_K40_peak_cali");
  TGraph *g_K40_peak_uncali = (TGraph *)dir->Get("g_K40_peak_uncali");
  TH1D   *h_K40_peak_cali   = (TH1D *)dir->Get("h_K40_peak_cali");
  TH1D   *h_K40_peak_uncali = (TH1D *)dir->Get("h_K40_peak_uncali");

  g_K40_peak_cali->SetMarkerColorAlpha(kRed, 1);
  g_K40_peak_cali->SetMarkerStyle(8);
  g_K40_peak_cali->GetXaxis()->SetLimits(0, N);

  g_K40_peak_uncali->SetMarkerColor(kBlue);
  g_K40_peak_uncali->SetMarkerStyle(22);
  g_K40_peak_uncali->GetXaxis()->SetLimits(0, N);

  c3->cd();
  pL->Draw();
  pR->Draw();

  c3->cd();
  pL->cd();
  mg->Add(g_K40_peak_uncali);
  mg->Add(g_K40_peak_cali);
  for (int i = 0; i <= N / 80; i++) {
    mg->GetXaxis()->SetBinLabel(i * 80 + 1, time[i * 80]);
  }
  mg->SetMaximum(1.44);
  mg->SetMinimum(1.37);
  mg->GetYaxis()->SetTitle("K40 peak (MeV)");
  mg->GetXaxis()->SetTitle("Time (mm/dd)");
  mg->GetXaxis()->SetTitleOffset(1.6);
  mg->GetXaxis()->SetNdivisions(510);
  mg->Draw("AP");
  TLegend *leg2 = new TLegend(0.65, 0.65, 0.80, 0.80);
  leg2->SetBorderSize(0);
  leg2->SetTextSize(0.04);
  leg2->SetFillColorAlpha(0, 0);
  leg2->AddEntry(g_K40_peak_uncali, "un-cali", "p");
  leg2->AddEntry(g_K40_peak_cali, "Cali", "p");
  leg2->Draw();
  gPad->Update();

  c3->cd();
  pR->cd();
  h_K40_peak_cali->SetFillColor(kRed);
  h_K40_peak_cali->SetStats(0);
  h_K40_peak_uncali->SetFillColor(kBlue);
  h_K40_peak_uncali->SetStats(0);
  h_K40_peak_uncali->Draw("hbar");
  h_K40_peak_cali->Draw("same hbar");

  mgr::SetRightPlotAxis(h_K40_peak_uncali);
  pR->Modified();
  pL->Modified();
  c3->SetGridy(1);
  c3->Modified();
  c3->SaveAs("plots/K40_cali_vs_uncali_beforeSep.pdf");
  delete c3;
}
