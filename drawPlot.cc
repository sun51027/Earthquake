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
  // TFile      *fin = new TFile("plots_root/oAnalyzer.root");

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

  /*----------------------------------------------*/

  // difference vs time
  // create canvas
  TCanvas *c  = new TCanvas("c", "", 10, 10, 1800, 900);
  TPad    *pL = mgr::NewLeftPad();
  TPad    *pR = mgr::NewRightPad();

  c->cd();
  pL->Draw();
  pR->Draw();

  c->cd();
  pL->cd();
  g_diffvsTime->SetTitle("");
//  g_diffvsTime->SetMaximum(20000);
//  g_diffvsTime->SetMinimum(-20000);
  g_diffvsTime->GetXaxis()->SetLimits(0, N);
  g_diffvsTime->GetXaxis()->SetTitle("Time (mm/dd)");
  g_diffvsTime->GetYaxis()->SetTitle("N_{radon} - <N_{radon}> ");
  g_diffvsTime->GetXaxis()->SetTitleOffset(1.6);
  g_diffvsTime->SetMarkerStyle(20);
  g_diffvsTime->SetMarkerColor(kBlue);
  //  for (int i = 0; i <= N / 80; i++) {
  //    g_diffvsTime->GetXaxis()->SetBinLabel(i * 80 + 1, time[i * 80]);
  //  }
  mgr::SetLeftPlotAxis(g_diffvsTime);
  g_diffvsTime->GetXaxis()->SetTimeDisplay(1);
  g_diffvsTime->GetXaxis()->SetTimeFormat("%d/%m%F2021-09-015 00:00:00s0");
  g_diffvsTime->GetXaxis()->SetTimeOffset(1, "gmt");
  //	g_diffvsTime->GetYaxis()->SetTitleSize(30);
  //	g_diffvsTime->GetYaxis()->SetLabelSize(30);
  g_diffvsTime->Draw("AP");

  c->cd();
  pR->cd();
  h_diff->Draw("hbar");
  h_diff->SetFillColor(kBlue);
  h_diff->SetStats(0);
  mgr::SetRightPlotAxis(h_diff);

  pR->Modified();
  pL->Modified();
  pL->SetGrid(1, 1);
  c->Modified();
  c->SaveAs("plots/DiffvsTime.pdf");
  delete c;

  /*----------------------------------------------*/
  // K40 stability
  TCanvas     *c2  = new TCanvas("c2", "", 10, 10, 1800, 900);
  TPad        *pL2 = mgr::NewLeftPad();
  TPad        *pR2 = mgr::NewRightPad();
  TMultiGraph *mg  = new TMultiGraph();

  g_K40_peak_cali->SetMarkerColorAlpha(kRed, 1);
  g_K40_peak_cali->SetMarkerStyle(8);
  g_K40_peak_cali->GetXaxis()->SetLimits(0, N);
  mgr::SetLeftPlotAxis(g_K40_peak_cali);

  g_K40_peak_uncali->SetMarkerColor(kBlue);
  g_K40_peak_uncali->SetMarkerStyle(22);
  g_K40_peak_uncali->GetXaxis()->SetLimits(0, N);
  mgr::SetLeftPlotAxis(g_K40_peak_uncali);

  c2->cd();
  pL2->Draw();
  pR2->Draw();

  c2->cd();
  pL2->cd();
  mg->Add(g_K40_peak_uncali);
  mg->Add(g_K40_peak_cali);
  mg->Draw("AP");
  mg->SetMaximum(1.44);
  mg->SetMinimum(1.37);
  mg->GetYaxis()->SetTitle("K40 peak (MeV)");
  mg->GetXaxis()->SetTitle("Time (mm/dd)");
  mg->GetXaxis()->SetTitleOffset(1.6);
  mg->GetXaxis()->SetNdivisions(5);
  for (int i = 0; i <= N / 80; i++) {
    mg->GetXaxis()->SetBinLabel(i * 80 + 1, time[i * 80]);
  }
  TLegend *leg2 = new TLegend(0.65, 0.65, 0.80, 0.80);
  leg2->SetBorderSize(0);
  leg2->SetTextSize(0.04);
  leg2->SetFillColorAlpha(0, 0);
  leg2->AddEntry(g_K40_peak_uncali, "un-cali", "p");
  leg2->AddEntry(g_K40_peak_cali, "Cali", "p");
  leg2->Draw();

  c2->cd();
  pR2->cd();
  h_K40_peak_cali->SetFillColor(kRed);
  h_K40_peak_cali->SetStats(0);
  h_K40_peak_uncali->SetFillColor(kBlue);
  h_K40_peak_uncali->SetStats(0);
  h_K40_peak_uncali->Draw("hbar");
  h_K40_peak_cali->Draw("same hbar");
  mgr::SetRightPlotAxis(h_K40_peak_uncali);

  pR2->Modified();
  pL2->Modified();
  pL2->SetGrid(1, 1);
  c2->Modified();
  c2->SaveAs("plots/K40_cali_vs_uncali.pdf");
  delete c2;

  /*----------------------------------------------*/
  //   Correlation factor
  TCanvas *c3  = new TCanvas("c3", "", 1800, 900);
  TPad    *pL3 = mgr::NewLeftPad();
  TPad    *pR3 = mgr::NewRightPad();

  c3->cd();
  pL3->Draw();
  pR3->Draw();

  c3->cd();
  pL3->cd();

  g_cfactor->SetTitle("");
  g_cfactor->Draw("AP");
  g_cfactor->SetMarkerColor(kBlue);
  g_cfactor->SetMarkerStyle(20);
  g_cfactor->SetMaximum(1.006); // 1.01
  g_cfactor->SetMinimum(0.992); // 0.98
  g_cfactor->GetXaxis()->SetLimits(0, N);
  g_cfactor->GetYaxis()->SetTitle("calibration factor");
  g_cfactor->GetXaxis()->SetTitle("Time (mm/dd)");
  mgr::SetLeftPlotAxis(g_cfactor);
  for (int i = 0; i <= N / 80; i++) {
    g_cfactor->GetXaxis()->SetBinLabel(i * 80 + 1, time[i * 80]);
  }

  c3->cd();
  pR3->cd();

  h_cfactor->Draw("hbar");
  h_cfactor->SetFillColor(kBlue);
  h_cfactor->SetStats(0);
  // h_cfactor->GetXaxis()->SetLimits(0.98,1.01);
  //  mgr::SetRightPlotAxis(h_cfactor);

  pR3->Modified();
  pL3->Modified();
  pL3->SetGrid(1, 1);
  c3->Modified();
  c3->SaveAs("plots/cfactor.pdf");
  delete c3;

  /*----------------------------------------------*/
  // sigma significant
  TCanvas *c4 = new TCanvas("c4", "", 800, 600);
  g_sigma_significant->SetTitle("");
  g_sigma_significant->SetMaximum(8);
  g_sigma_significant->SetMinimum(0);
  g_sigma_significant->GetXaxis()->SetLimits(0, N);
  g_sigma_significant->GetXaxis()->SetTitle("Time (mm/dd)");
  g_sigma_significant->GetYaxis()->SetTitle("#sigma");
  g_sigma_significant->GetXaxis()->SetTitleOffset(1.6);
  //	g_sigma_significant->SetMarkerStyle(20);
  //	g_sigma_significant->SetMarkerColor(kBlue);
  g_sigma_significant->Draw("");
  for (int i = 0; i <= N / 80; i++) {
    g_sigma_significant->GetXaxis()->SetBinLabel(i * 80 + 1, time[i * 80]);
  }
  c4->SetGrid(1, 1);
  c4->Modified();
  c4->SaveAs("plots/Significant_sigma.pdf");
  delete c4;

  /*----------------------------------------------*/
  // p value
  TCanvas *c5 = new TCanvas("c5", "", 800, 600);
  for (int i = 0; i < N; i++) {
    cout << "sigma[" << i << "] " << g_sigma_significant->GetPointY(i) << "\t\t";
    cout << "pvalue[" << i << "] " << g_pvalue->GetPointY(i) << endl;
  }
  g_pvalue->SetTitle("");
  g_pvalue->SetMaximum(1);
  g_pvalue->SetMinimum(1e-13);
  g_pvalue->GetXaxis()->SetLimits(0, N);
  g_pvalue->GetXaxis()->SetTitle("Time (mm/dd)");
  g_pvalue->GetYaxis()->SetTitle("p-value");
  g_pvalue->GetXaxis()->SetTitleOffset(1.6);
  g_pvalue->SetMarkerStyle(20);
  g_pvalue->SetMarkerColor(kBlue);
  g_pvalue->Draw("");
  //  for (int i = 0; i <= N / 80; i++) {
  //    g_pvalue->GetXaxis()->SetBinLabel(i * 80 + 1, time[i * 80]);
  //  }
  c5->SetLogy();
  c5->SetGrid(1, 1);
  c5->Modified();
  c5->SaveAs("plots/p-value.pdf");
  delete c5;
}
