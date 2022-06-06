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
#include "TLatex.h"
#include "TH1D.h"
#include "TKey.h"
#include "TStyle.h"
#include "rootlogon.h"
using namespace mgr;

void Earthquake::DrawPlot()
{
		

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
  g_diffvsTime->SetMaximum(20000/fluct_sigma);
  g_diffvsTime->SetMinimum(-20000/fluct_sigma);
  g_diffvsTime->SetMarkerStyle(20);
  g_diffvsTime->SetMarkerColor(kBlue);
  g_diffvsTime->Draw("AP");
  mgr::SetLeftPlotAxis(g_diffvsTime);
  g_diffvsTime->GetXaxis()->SetLimits(-30 * 60 * 60 * 2, (N + 29) * 60 * 60 * 2);
  g_diffvsTime->GetXaxis()->SetTitle("Time (mm/dd)");
  g_diffvsTime->GetYaxis()->SetTitle("N_{radon} - <N_{radon}> / #sigma");
  g_diffvsTime->GetXaxis()->SetTitleOffset(1.6);
  g_diffvsTime->GetXaxis()->SetTimeDisplay(1);
  g_diffvsTime->GetXaxis()->SetTimeFormat("%d/%m %F2021-09-15 00:00:00");
  g_diffvsTime->GetYaxis()->SetTitleSize(30);
  g_diffvsTime->GetYaxis()->SetLabelSize(30);

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
  c->SaveAs("plots/DiffvsTime2.pdf");
  delete c;

  /*----------------------------------------------*/
  // K40 stability
  TCanvas     *c2  = new TCanvas("c2", "", 10, 10, 1800, 900);
  TPad        *pL2 = mgr::NewLeftPad();
  TPad        *pR2 = mgr::NewRightPad();
  TMultiGraph *mg  = new TMultiGraph();

  g_K40_peak_cali->SetMarkerColorAlpha(kRed, 1);
  g_K40_peak_cali->SetMarkerStyle(8);
  mgr::SetLeftPlotAxis(g_K40_peak_cali);

  g_K40_peak_uncali->SetMarkerColor(kBlue);
  g_K40_peak_uncali->SetMarkerStyle(22);
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
  mg->GetXaxis()->SetTimeDisplay(1);
  mg->GetXaxis()->SetTimeFormat("%d/%m %F2021-09-15 00:00:00");

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

  g_cfactor->SetMarkerColor(kBlue);
  g_cfactor->SetMarkerStyle(20);
  mgr::SetLeftPlotAxis(g_cfactor);

  g_cfactor_cali->SetMarkerColor(kRed);
  g_cfactor_cali->SetMarkerStyle(20);
  mgr::SetLeftPlotAxis(g_cfactor_cali);

  TMultiGraph *mg2  = new TMultiGraph();
	mg2->Add(g_cfactor_cali);
	mg2->Add(g_cfactor);
  mg2->SetTitle("");
  mg2->SetMaximum(1.01); // 1.01
  mg2->SetMinimum(0.99); // 0.98
  mg2->Draw("AP");
  mg2->GetXaxis()->SetLimits(-30 * 60 * 60 * 2, (N + 29) * 60 * 60 * 2);
  mg2->GetYaxis()->SetTitle("calibration factor");
  mg2->GetXaxis()->SetTitle("Time (mm/dd)");
  mg2->GetXaxis()->SetTimeDisplay(1);
  mg2->GetXaxis()->SetTimeFormat("%d/%m %F2021-09-15 00:00:00");

  TLegend *leg3 = new TLegend(0.65, 0.65, 0.80, 0.80);
  leg3->SetBorderSize(0);
  leg3->SetTextSize(0.04);
  leg3->SetFillColorAlpha(0, 0);
  leg3->AddEntry(g_cfactor, "un-cali", "p");
  leg3->AddEntry(g_cfactor_cali, "Cali", "p");
  leg3->Draw();

  c3->cd();
  pR3->cd();

  h_cfactor_cali->Draw("hbar");
  h_cfactor->Draw("hbar same");
  h_cfactor->SetFillColor(kBlue);
  h_cfactor->SetStats(0);
  h_cfactor_cali->SetFillColor(kRed);
  h_cfactor_cali->SetStats(0);
  mgr::SetRightPlotAxis(h_cfactor_cali);


  pR3->Modified();
  pL3->Modified();
  pL3->SetGrid(1, 1);
  c3->Modified();
  c3->SaveAs("plots/cfactor.pdf");
  delete c3;

  /*----------------------------------------------*/
  // sigma significant
  TCanvas *c4 = new TCanvas("c4", "", 1200, 600);
  g_sigma_significant->SetTitle("");
  g_sigma_significant->SetMaximum(9);
  g_sigma_significant->SetMinimum(0);
  g_sigma_significant->Draw("");
  g_sigma_significant->GetXaxis()->SetLimits(-30 * 60 * 60 * 2, (N + 29) * 60 * 60 * 2);
  g_sigma_significant->GetXaxis()->SetTitle("Time (mm/dd)");
  g_sigma_significant->GetYaxis()->SetTitle("No. of #sigma");
  g_sigma_significant->GetXaxis()->SetTitleOffset(1.6);
  g_sigma_significant->GetXaxis()->SetTimeDisplay(1);
  g_sigma_significant->GetXaxis()->SetTimeFormat("%d/%m %F2021-09-15 00:00:00");
  c4->SetGrid(1, 1);
  c4->Modified();
  c4->SaveAs("plots/Significant_sigma.pdf");
  delete c4;

  /*----------------------------------------------*/
  // p value
  TCanvas *c5 = new TCanvas("c5", "", 1200, 600);
  g_pvalue->SetTitle("");
  g_pvalue->SetMaximum(1);
  g_pvalue->SetMinimum(1e-13);
  g_pvalue->Draw("");
  g_pvalue->GetXaxis()->SetLimits(-30 * 60 * 60 * 2, (N + 80) * 60 * 60 * 2);
  g_pvalue->GetXaxis()->SetTitle("Time (mm/dd)");
  g_pvalue->GetYaxis()->SetTitle("p-value");
  //g_pvalue->GetXaxis()->SetTitleOffset(1.6);
  g_pvalue->GetXaxis()->SetTimeDisplay(1);
  g_pvalue->GetXaxis()->SetTimeFormat("%d/%m %F2021-09-015 00:00:00");
  for (int i = 0; i < 7; i++) {
    double p = 0.5 * (1 - TMath::Erf((i + 1) / sqrt(2)));
    TLine *l = new TLine(-30 * 60 * 60 * 2, p, (N + 80) * 60 * 60 * 2, p);
    l->SetLineStyle(7);
    l->SetLineColor(kRed);
    l->SetLineWidth(1);
    l->Draw();
		TString s;
		s.Form("%i",i+1);
    TLatex *tv1 = new TLatex((N+15) * 60 * 60 * 2, p,s+" #sigma");
    tv1->SetTextAlign(11);
		tv1->SetTextColor(kRed);
    tv1->SetTextSize(0.04);
    tv1->SetTextFont(12);
    tv1->Draw();
  }

  c5->SetLogy();
  c5->SetTicks(1, 1);
  c5->Modified();
  c5->SaveAs("plots/p-value.pdf");
  delete c5;
}
