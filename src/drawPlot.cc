// STL
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
// my header
#include "../include/RadonData.h"
#include "../include/DataReader.h"
#include "../include/GeoData.h"
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
#include "../include/rootlogon.h"
using namespace mgr;

void RadonData::DrawPlot()
{

  gStyle->SetTimeOffset(timeoffset.Convert());
  timeoffset.Print();

  //# of K40
  // TCanvas *c10  = new TCanvas("c", "", 10, 10, 1800, 900);
  // g_NofK40->Draw("AP");
  // g_NofK40->SetTitle("");
  // g_NofK40->SetMarkerStyle(20);
  // g_NofK40->SetMarkerColor(kBlue);
  // g_NofK40->GetXaxis()->SetLimits(-30 * 60 * 60 * 2, (N + 29) * 60 * 60 * 2);
  // g_NofK40->GetXaxis()->SetTitle("Time (mm/dd)");
  // g_NofK40->GetYaxis()->SetTitle("Area of K-40");
  // g_NofK40->GetXaxis()->SetTitleOffset(1.6);
  // g_NofK40->GetXaxis()->SetTimeDisplay(1);
  // g_NofK40->GetXaxis()->SetTimeFormat("%m/%d");
  // g_NofK40->GetYaxis()->SetTitleSize(30);
  // g_NofK40->GetYaxis()->SetLabelSize(30);
  //
  // c10->SetGrid(1, 1);
  // c10->Modified();
  // c10->SaveAs("plots/NofK40.pdf");
  // delete c10;
  //
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
  g_diffvsTime->Draw("AP");
  g_diffvsTime->SetTitle("");
  g_diffvsTime->SetMaximum(20000 / fluct_sigma);
  g_diffvsTime->SetMinimum(-20000 / fluct_sigma);
  g_diffvsTime->SetMarkerStyle(20);
  g_diffvsTime->SetMarkerColor(kBlue);
  mgr::SetLeftPlotAxis(g_diffvsTime);
  g_diffvsTime->GetXaxis()->SetLimits(-30 * 60 * 60 * 2, (N + 29) * 60 * 60 * 2);
  g_diffvsTime->GetXaxis()->SetTitle("Time (mm/dd)");
  g_diffvsTime->GetYaxis()->SetTitle("N_{radon} - <N_{radon}> / #sigma");
  g_diffvsTime->GetXaxis()->SetTitleOffset(1.6);
  g_diffvsTime->GetXaxis()->SetTimeDisplay(1);
  g_diffvsTime->GetXaxis()->SetTimeFormat("%m/%d");
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
  c->SaveAs("plots/DiffvsTime.pdf");
  delete c;
  /*----------------------------------------------*/
  //   Radon  2nd peak

  g_Radon2_uncali->SetMarkerColor(kBlue);
  g_Radon2_uncali->SetMarkerStyle(20);
  mgr::SetAxis(g_Radon2_uncali);

  g_Radon2_cali->SetMarkerColor(kRed);
  g_Radon2_cali->SetMarkerStyle(20);
  mgr::SetAxis(g_Radon2_cali);

  TCanvas *c8  = new TCanvas("c8", "", 1800, 900);
  TMultiGraph *mg4 = new TMultiGraph();
  mg4->Add(g_Radon2_uncali);
  mg4->Add(g_Radon2_cali);
  mg4->SetTitle("");
  // mg4->SetMaximum(0.62); // 1.01
  // mg4->SetMinimum(0.55); // 0.98
  mg4->Draw("AP");
  mg4->GetXaxis()->SetLimits(-30 * 60 * 60 * 2, (N + 29) * 60 * 60 * 2);
  mg4->GetYaxis()->SetTitle("Radon 2nd peak (MeV)");
  mg4->GetXaxis()->SetTitle("Time (mm/dd)");
  mg4->GetXaxis()->SetTimeDisplay(1);
  mg4->GetXaxis()->SetTimeFormat("%m/%d");
  TLegend *leg4 = new TLegend(0.65, 0.65, 0.80, 0.80);
  leg4->SetBorderSize(0);
  leg4->SetTextSize(0.04);
  leg4->SetFillColorAlpha(0, 0);
  leg4->AddEntry(g_Radon2_uncali, "un-cali", "p");
  leg4->AddEntry(g_Radon2_cali, "Cali", "p");
  leg4->Draw();
  c8->SetGrid(1,1);
  c8->Modified();
  c8->SaveAs("plots/Radon2.pdf");
  delete c8;

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
  mg->SetMaximum(1.44); // 1.44
  mg->SetMinimum(1.24); // 1.37
  mg->GetYaxis()->SetTitle("K40 peak (MeV)");
  mg->GetXaxis()->SetTitle("Time (mm/dd)");
  mg->GetXaxis()->SetTimeDisplay(1);
  mg->GetXaxis()->SetTimeFormat("%m/%d");

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
  //   twoPoint 

  TCanvas *c9  = new TCanvas("c9", "", 1800, 900);

  g_twopoint_uncali->SetMarkerColor(kBlue);
  g_twopoint_uncali->SetMarkerStyle(20);
  mgr::SetAxis(g_twopoint_uncali);

  g_twopoint_cali->SetMarkerColor(kRed);
  g_twopoint_cali->SetMarkerStyle(20);
  mgr::SetAxis(g_twopoint_cali);

  TMultiGraph *mg3 = new TMultiGraph();
  mg3->Add(g_twopoint_uncali);
  mg3->Add(g_twopoint_cali);
  mg3->SetTitle("");
  mg3->SetMaximum(2.2);
  mg3->SetMinimum(2.09);
  mg3->Draw("AP");
  mg3->GetXaxis()->SetLimits(-30 * 60 * 60 * 2, (N + 29) * 60 * 60 * 2);
  mg3->GetYaxis()->SetTitle("two point peak (MeV)");
  mg3->GetXaxis()->SetTitle("Time (mm/dd)");
  mg3->GetXaxis()->SetTimeDisplay(1);
  mg3->GetXaxis()->SetTimeFormat("%m/%d");
  TLegend *leg9 = new TLegend(0.65, 0.65, 0.80, 0.80);
  leg9->SetBorderSize(0);
  leg9->SetTextSize(0.04);
  leg9->SetFillColorAlpha(0, 0);
  leg9->AddEntry(g_twopoint_uncali, "un-cali", "p");
  leg9->AddEntry(g_twopoint_cali, "Cali", "p");
  leg9->Draw();

  c9->SetGrid(1, 1);
  c9->Modified();
  c9->SaveAs("plots/twoPoint.pdf");
  delete c9;
  
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

  TMultiGraph *mg2 = new TMultiGraph();
  mg2->Add(g_cfactor_cali);
  mg2->Add(g_cfactor);
  mg2->SetTitle("");
  mg2->SetMaximum(1.03); // 1.01
  mg2->SetMinimum(0.97); // 0.98
  mg2->Draw("AP");
  mg2->GetXaxis()->SetLimits(-30 * 60 * 60 * 2, (N + 29) * 60 * 60 * 2);
  mg2->GetYaxis()->SetTitle("calibration factor");
  mg2->GetXaxis()->SetTitle("Time (mm/dd)");
  mg2->GetXaxis()->SetTimeDisplay(1);
  mg2->GetXaxis()->SetTimeFormat("%m/%d");

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
  g_sigma_significant->GetXaxis()->SetLimits(-14 * 60 * 60 * 2, (N + 29) * 60 * 60 * 2);
  g_sigma_significant->GetXaxis()->SetTitle("Time (mm/dd)");
  g_sigma_significant->GetYaxis()->SetTitle("No. of #sigma");
  g_sigma_significant->GetXaxis()->SetTitleOffset(1.6);
  g_sigma_significant->GetXaxis()->SetTimeDisplay(1);
  g_sigma_significant->GetXaxis()->SetTimeFormat("%m/%d");
  c4->SetGrid(1, 1);
  c4->Update();
  c4->SaveAs("plots/Significant_sigma.pdf");
  delete c4;

  /*----------------------------------------------*/
  // p value
  TCanvas *c5 = new TCanvas("c5", "", 1200, 600);
  g_pvalue->SetTitle("");
  g_pvalue->SetMaximum(1);
  g_pvalue->SetMinimum(1e-7);
  //  g_pvalue->SetMinimum(1e-13);
  g_pvalue->Draw("");
  g_pvalue->GetXaxis()->SetLimits(-30 * 60 * 60 * 2, (N + 80) * 60 * 60 * 2);
  g_pvalue->GetXaxis()->SetTitle("Time (mm/dd)");
  g_pvalue->GetYaxis()->SetTitle("p-value");
//  g_pvalue->GetXaxis()->SetLabelSize(0);
//  g_pvalue->GetXaxis()->SetTitleSize(0);
  g_pvalue->GetYaxis()->SetTitleSize(0.05);
  g_pvalue->GetYaxis()->SetTitleOffset(0.7);
  g_pvalue->GetXaxis()->SetTimeDisplay(1);
  g_pvalue->GetXaxis()->SetTimeFormat("%m/%d");
  for (int i = 0; i < 5; i++) {
    double p = 0.5 * (1 - TMath::Erf((i + 1) / sqrt(2)));
    TLine *l = new TLine(-30 * 60 * 60 * 2, p, (N + 80) * 60 * 60 * 2, p);
    l->SetLineStyle(7);
    l->SetLineColor(kRed);
    l->SetLineWidth(1);
    l->Draw();
    TString s;
    s.Form("%i", i + 1);
    TLatex *tv1 = new TLatex((N + 15) * 60 * 60 * 2, p, s + " #sigma");
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

void DataReader::EQdirDrawPlots()
{

  /*  Just draw EQ directory, not */
  gStyle->SetTimeOffset(timeoffset.Convert());
  timeoffset.Print();

  int lowerlimit = 0;
  int upperlimit = 0;
  int limitdays  = 0;
  int N          = datetime_Rn.size();
  cout << "N " << N << endl;
  if (N > 1000) {
    lowerlimit = -30 * 60 * 60 * 2;
    upperlimit = (N + 80) * 60 * 60 * 2;
  } else {
    cout << "There are " << (double)(N / 12) << " days in the graph! " << endl;
    cout << "Input limit days: ";
    cin >> limitdays;

    lowerlimit = -(limitdays)*60 * 60 * 2;
    upperlimit = (N + limitdays) * 60 * 60 * 2;
  }
  // EQ
  TCanvas *c6 = new TCanvas("c5", "", 1200, 200);
  c6->SetBottomMargin(0.3);

  g_ML->SetTitle("");
  g_ML->Draw("AP");
  g_ML->GetXaxis()->SetLimits(lowerlimit, upperlimit);
  g_ML->GetXaxis()->SetTitle("Time (mm/dd)");
  g_ML->GetYaxis()->SetTitle("M_{L}");
  g_ML->SetMinimum(0.5);
  g_ML->SetMaximum(7);
  g_ML->SetMarkerStyle(20);
  g_ML->GetXaxis()->SetTimeDisplay(1);
  g_ML->GetXaxis()->SetTimeFormat("%m/%d-%Hh");
  g_ML->GetXaxis()->SetLabelSize(0.1);
  g_ML->GetXaxis()->SetTitleSize(0.1);

  g_ML->GetYaxis()->SetLabelSize(0.1);
  g_ML->GetYaxis()->SetTitleSize(0.1);
  g_ML->GetYaxis()->SetTitleOffset(0.3);

  c6->SetTicks(1, 1);
  c6->SetGrid(1, 1);
  c6->Modified();
  c6->SaveAs("plots/EQ_ML.pdf");
  delete c6;

  // depth
  TCanvas *c7 = new TCanvas("c5", "", 1200, 200);
  c7->SetBottomMargin(0.3);

  g_depth->SetTitle("");
  g_depth->Draw("AP");
  g_depth->GetXaxis()->SetLimits(lowerlimit, upperlimit);
  g_depth->GetXaxis()->SetTitle("Time (dd/mm)");
  g_depth->GetYaxis()->SetTitle("Depth(km)");
  g_depth->SetMinimum(-40);
  g_depth->SetMaximum(-0.5);
  g_depth->SetMarkerStyle(20);
  g_depth->GetXaxis()->SetTimeDisplay(1);
  g_depth->GetXaxis()->SetTimeFormat("%m/%d-%Hh");
  g_depth->GetXaxis()->SetLabelSize(0.1);
  g_depth->GetXaxis()->SetTitleSize(0.1);
  //  g_depth->GetXaxis()->SetTitleOffset(1.0);

  g_depth->GetYaxis()->SetLabelSize(0.1);
  g_depth->GetYaxis()->SetTitleSize(0.1);
  g_depth->GetYaxis()->SetTitleOffset(0.3);
  c7->SetTicks(1, 1);
  c7->SetGrid(1, 1);
  c7->Modified();
  c7->SaveAs("plots/EQ_depth.pdf");
  delete c7;
}

void drawPvalue_eqdir(TDirectory *dir1, TDirectory *dir2, TDatime timeoffset)
{
  gStyle->SetTimeOffset(timeoffset.Convert());
  timeoffset.Print();
  dir1->cd();
  dir2->cd();

  TCanvas *c  = new TCanvas("c", "", 1200, 800);
  TPad    *pT = new TPad("toppT", "", 0, 0.45, 1., 1.0); // xlow ylow xup yup
  pT->SetTicks(1, 1);
  pT->SetBottomMargin(0.05);
  pT->SetLeftMargin(0.13);
  pT->SetRightMargin(1 - 0.95);
  pT->SetTopMargin((1 - 0.9) / (1 - 0.3));

  TPad *pB = new TPad("botpB", "", 0, 0.45 / 2, 1, 0.45);
  pB->SetTicks(1, 1);
  pB->SetTopMargin(0.025);
  pB->SetLeftMargin(0.13);
  pB->SetRightMargin(1 - 0.95);
  pB->SetBottomMargin((0.105) / (0.3));

  TPad *pB2 = new TPad("botpB2", "", 0, 0., 1, 0.45 / 2);
  pB2->SetTicks(1, 1);
  pB2->SetTopMargin(0.025);
  pB2->SetLeftMargin(0.13);
  pB2->SetRightMargin(1 - 0.95);
  pB2->SetBottomMargin((0.105) / (0.3));

  c->cd();
  pT->Draw();
  pB->Draw();
  pB2->Draw();

  c->cd();
  pT->cd();
  // TGraph
  TGraph *g_pvalue = (TGraph *)dir1->Get("g_pvalue");
  TGraph *g_ML     = (TGraph *)dir2->Get("g_ML");
  TGraph *g_depth  = (TGraph *)dir2->Get("g_depth");

  int N = 0;
  if (g_ML->GetN() != g_pvalue->GetN()) {
    cout << "#bin in g_ML " << g_ML->GetN() << endl;
    cout << "#bin in g_pvalue " << g_pvalue->GetN() << endl;
    cout << "N = 0 " << endl;
  } else
    N = g_ML->GetN();

  int lowerlimit = 0;
  int upperlimit = 0;
  int limitdays  = 0;
  if (N > 1000) {
    lowerlimit = -30 * 60 * 60 * 2;
    upperlimit = (N + 80) * 60 * 60 * 2;
  } else {
    cout << "There are " << (double)(N / 12) << " days in the graph! " << endl;
    cout << "Input limit days: ";
    //					cin<<limitdays<<endl;
    lowerlimit = -(limitdays)*60 * 60 * 2;
    upperlimit = (N + limitdays) * 60 * 60 * 2;
  }

  g_pvalue->SetTitle("");
  g_pvalue->SetMaximum(1);
  g_pvalue->SetMinimum(1e-7);
  g_pvalue->Draw("AL");
  g_pvalue->GetXaxis()->SetLimits(lowerlimit, upperlimit);
  g_pvalue->GetXaxis()->SetTitle("Time (mm/dd)");
  g_pvalue->GetYaxis()->SetTitle("p-value");

  g_pvalue->GetXaxis()->SetLabelSize(0);
  g_pvalue->GetXaxis()->SetTitleSize(0);
  g_pvalue->GetXaxis()->SetTimeDisplay(1);
  g_pvalue->GetXaxis()->SetTimeFormat("%m/%d");
  g_pvalue->GetXaxis()->SetNdivisions(511);

  g_pvalue->GetYaxis()->SetTitleSize(0.05);
  g_pvalue->GetYaxis()->SetTitleOffset(0.7);

  for (int i = 0; i < 5; i++) {
    double p = 0.5 * (1 - TMath::Erf((i + 1) / sqrt(2)));
    TLine *l = new TLine(lowerlimit, p, upperlimit, p);
    l->SetLineStyle(7);
    l->SetLineColor(kRed);
    l->SetLineWidth(1);
    l->Draw();
    TString s;
    s.Form("%i", i + 1);
    TLatex *tv1 = new TLatex((N + 15) * 60 * 60 * 2, p, s + " #sigma");
    tv1->SetTextAlign(11);
    tv1->SetTextColor(kRed);
    tv1->SetTextSize(0.04);
    tv1->SetTextFont(12);
    tv1->Draw();
  }

  c->cd();
  pB->cd();

  g_ML->SetTitle("");
  g_ML->Draw("AP");
  g_ML->GetXaxis()->SetLimits(lowerlimit, upperlimit);
  //  g_ML->GetXaxis()->SetTitle("Time (mm/dd)");
  g_ML->GetYaxis()->SetTitle("M_{L}");
  g_ML->SetMinimum(1);
  g_ML->SetMaximum(7);
  g_ML->SetMarkerStyle(20);
  g_ML->GetXaxis()->SetTimeDisplay(1);
  g_ML->GetXaxis()->SetTimeFormat("%m/%d-%Hh");

  g_ML->GetXaxis()->SetLabelSize(0.0);
  g_ML->GetXaxis()->SetTitleSize(0.0);
  g_ML->GetXaxis()->SetNdivisions(511);

  g_ML->GetYaxis()->SetLabelSize(0.08);
  g_ML->GetYaxis()->SetTitleSize(0.1);
  g_ML->GetYaxis()->SetTitleOffset(0.4);

  c->cd();
  pB2->cd();
  g_depth->SetTitle("");
  g_depth->Draw("AP");
  g_depth->GetXaxis()->SetLimits(lowerlimit, upperlimit);
  g_depth->GetXaxis()->SetTitle("Time (mm/dd)");
  g_depth->GetYaxis()->SetTitle("depth");
  g_depth->SetMinimum(-40);
  g_depth->SetMaximum(-0.5);
  g_depth->SetMarkerStyle(20);

  g_depth->GetXaxis()->SetTimeDisplay(1);
  g_depth->GetXaxis()->SetTimeFormat("%m/%d");
  g_depth->GetXaxis()->SetLabelSize(0.125);
  g_depth->GetXaxis()->SetTitleSize(0.12);
  g_depth->GetXaxis()->SetTitleOffset(1.0);
  g_depth->GetXaxis()->SetNdivisions(511);

  g_depth->GetYaxis()->SetLabelSize(0.08);
  g_depth->GetYaxis()->SetTitleSize(0.1);
  g_depth->GetYaxis()->SetTitleOffset(0.4);

  pT->SetLogy();
  pB->SetGrid(0, 1);
  pB2->SetGrid(0, 1);

  pT->Modified();
  pB->Modified();
  pB2->Modified();
  c->Modified();
  c->SaveAs("plots/Pvalue_ML_depth_test.pdf");
  delete c;
}

void GeoData::DrawGeoData(TString name, TString channel, TDatime timeoffset)
{
  TCanvas *c = new TCanvas("c", "", 1200, 600);
  // TCanvas *c  = new TCanvas("c", "", 10, 10, 1800, 900);
  // TPad    *pL = mgr::NewLeftPad();
  // TPad    *pR = mgr::NewRightPad();

  // c->cd();
  // pL->Draw();
  // pR->Draw();

  // c->cd();
  // pL->cd();
  g_data->Draw("AL");
  g_data->SetTitle("");
  g_data->GetXaxis()->SetTimeDisplay(1);
  g_data->GetXaxis()->SetTimeOffset(0);
  g_data->GetXaxis()->SetTimeFormat("%m/%d %Hh");
  g_data->GetXaxis()->SetTitle("Date time (mm/dd/hh)");
  if (channel.Contains("HN1")||channel.Contains("HN2")||channel.Contains("HNZ")) {
    g_data->GetYaxis()->SetTitle(channel + " (m/s/s)");
  } else {
    g_data->GetYaxis()->SetTitle(channel + " (m/s)");
  }
  //  mgr::SetLeftPlotAxis(g_data);
  // c->cd();
  // pR->cd();
  // h_fitting->Draw("hbar");
  // h_fitting->SetFillColor(kBlue);
  // h_fitting->SetStats(0);
  // mgr::SetRightPlotAxis(h_fitting);

  //  pR->Modified();
  //  pL->Modified();
  //  pL->SetGrid(1, 1);
  c->SetGrid(1, 1);
  c->Modified();
  c->SaveAs("plots/geodata/" + name + ".png");
  delete c;

  TCanvas *c2 = new TCanvas("c2", "", 1200, 600);
  g_nsigma->Draw("AP");
  g_nsigma->SetMarkerStyle(20);
  g_nsigma->SetTitle("");
  g_nsigma->GetXaxis()->SetTimeDisplay(1);
  g_nsigma->GetXaxis()->SetNdivisions(511);
  g_nsigma->GetXaxis()->SetTimeFormat("%m/%d %Hh");
  g_nsigma->GetXaxis()->SetTitle("Date time (mm/dd/hh)");
  g_nsigma->GetYaxis()->SetTitle(channel + " # #sigma");
  c2->SetGrid(1, 0);
  c2->Modified();
  c2->SaveAs("plots/geodata/" + name + "_nsigma.pdf");

  TCanvas *c3 = new TCanvas("c2", "", 1200, 600);
  g_2hrdata->Draw("AP");
  g_2hrdata->SetMarkerStyle(20);
  g_2hrdata->SetTitle("");
  g_2hrdata->GetXaxis()->SetTimeDisplay(1);
  g_2hrdata->GetXaxis()->SetTimeOffset(0);
  g_2hrdata->GetXaxis()->SetTimeFormat("%m/%d %Hh");
  g_2hrdata->GetXaxis()->SetTitle("Date time (mm/dd/hh)");
  if (channel.Contains("HN1")||channel.Contains("HN2")||channel.Contains("HNZ")) {
    g_2hrdata->GetYaxis()->SetTitle(channel + " (m/s/s)");
  } else {
    g_2hrdata->GetYaxis()->SetTitle(channel + " (m/s)");
  }
  c3->SetGrid(1, 0);
  c3->Modified();
  c3->SaveAs("plots/geodata/" + name + "_2hrCombine.pdf");
}
void drawPvalue_geo(TDirectory *dir1, TDirectory *dir2, TDirectory *dir3, TDirectory *dir4, TDatime timeoffset)
{
  gStyle->SetTimeOffset(timeoffset.Convert());
  timeoffset.Print();
  dir1->cd();
  dir2->cd();
  dir3->cd();
  dir4->cd();

  TCanvas *c  = new TCanvas("c", "", 1200, 800);
  TPad    *pT = new TPad("toppT", "", 0, 0.75, 1., 1.0); // xlow ylow xup yup
  pT->SetTicks(1, 1);
  pT->SetBottomMargin(0.035);
  pT->SetLeftMargin(0.13);
  pT->SetRightMargin(1 - 0.95);
  pT->SetTopMargin((1 - 0.9) / (1 - 0.3));

  TPad *pB = new TPad("botpB", "", 0, 0.50, 1, 0.75);
  pB->SetTicks(1, 1);
  pB->SetTopMargin(0.025);
  pB->SetLeftMargin(0.13);
  pB->SetRightMargin(1 - 0.95);
  pB->SetBottomMargin((0.105) / (0.4));

  TPad *pB2 = new TPad("botpB2", "", 0, 0.25, 1, 0.50);
  pB2->SetTicks(1, 1);
  pB2->SetTopMargin(0.025);
  pB2->SetLeftMargin(0.13);
  pB2->SetRightMargin(1 - 0.95);
  pB2->SetBottomMargin((0.105) / (0.4));

  TPad *pB3 = new TPad("botpB3", "", 0, 0., 1, 0.25);
  pB3->SetTicks(1, 1);
  pB3->SetTopMargin(0.025);
  pB3->SetLeftMargin(0.13);
  pB3->SetRightMargin(1 - 0.95);
  pB3->SetBottomMargin((0.105) / (0.4));

  c->cd();
  pT->Draw();
  pB->Draw();
  pB2->Draw();
  pB3->Draw();

  // TGraph
  TGraph *g_pvalue  = (TGraph *)dir1->Get("g_pvalue");
  TGraph *g_nsigma1 = (TGraph *)dir2->Get("g_matchingdata");
  TGraph *g_nsigma2 = (TGraph *)dir3->Get("g_matchingdata");
  TGraph *g_nsigma3 = (TGraph *)dir4->Get("g_matchingdata");

  c->cd();
  pT->cd();

  int N = 0;
  if (g_nsigma1->GetN() != g_pvalue->GetN()) {
    cout << "#bin in g_nsigma " << g_nsigma1->GetN() << endl;
    cout << "#bin in g_pvalue " << g_pvalue->GetN() << endl;
    cout << "N = 0 " << endl;
    N = g_nsigma1->GetN();
  } else {
    N = g_nsigma1->GetN();
  }
  int lowerlimit = 0;
  int upperlimit = 0;
  int limitdays  = 0;
  if (N > 1000) {
    lowerlimit = -30 * 60 * 60 * 2;
    upperlimit = (N + 80) * 60 * 60 * 2;
  } else {
    cout << "There are " << (double)(N / 12) << " days in the graph! " << endl;
    cout << "Input limit days: ";
    //					cin<<limitdays<<endl;
    lowerlimit = -(limitdays)*60 * 60 * 2;
    upperlimit = (N + limitdays) * 60 * 60 * 2;
  }

  g_pvalue->SetTitle("");
  g_pvalue->SetMaximum(1);
  g_pvalue->SetMinimum(1e-7);
  g_pvalue->Draw("AL");
  g_pvalue->GetXaxis()->SetLimits(lowerlimit, upperlimit);
  g_pvalue->GetXaxis()->SetTitle("Time (mm/dd)");
  g_pvalue->GetXaxis()->SetTimeFormat("%m/%d");
  g_pvalue->GetYaxis()->SetTitle("p-value");
  g_pvalue->GetXaxis()->SetTimeDisplay(1);

  g_pvalue->GetXaxis()->SetNdivisions(511);
  g_pvalue->GetXaxis()->SetLabelSize(0.0);   // 0.08
  g_pvalue->GetXaxis()->SetTitleSize(0.0);   // 0.1
  g_pvalue->GetXaxis()->SetTitleOffset(1.0); // 1.0

  g_pvalue->GetYaxis()->SetLabelSize(0.08);
  g_pvalue->GetYaxis()->SetTitleSize(0.09);
  g_pvalue->GetYaxis()->SetTitleOffset(0.5);

  for (int i = 0; i < 5; i++) {
    double p = 0.5 * (1 - TMath::Erf((i + 1) / sqrt(2)));
    TLine *l = new TLine(lowerlimit, p, upperlimit, p);
    l->SetLineStyle(7);
    l->SetLineColor(kRed);
    l->SetLineWidth(1);
    l->Draw();
    TString s;
    s.Form("%i", i + 1);
    TLatex *tv1 = new TLatex((N + 15) * 60 * 60 * 2, p, s + " #sigma");
    tv1->SetTextAlign(11);
    tv1->SetTextColor(kRed);
    tv1->SetTextSize(0.09);
    tv1->SetTextFont(12);
    tv1->Draw();
  }

  c->cd();
  pB->cd();

  g_nsigma1->SetTitle("");
  g_nsigma1->Draw("AP");
  g_nsigma1->GetXaxis()->SetLimits(lowerlimit, upperlimit);
  g_nsigma1->GetXaxis()->SetTitle("Time (mm/dd)");
  g_nsigma1->GetYaxis()->SetTitle("EHE");
  g_nsigma1->SetMinimum(1);
  g_nsigma1->SetMarkerStyle(20);
  g_nsigma1->GetXaxis()->SetTimeDisplay(1);
  g_nsigma1->GetXaxis()->SetTimeFormat("%m/%d");
  g_nsigma1->GetXaxis()->SetNdivisions(511);
  g_nsigma1->GetXaxis()->SetLabelSize(0.08);
  g_nsigma1->GetXaxis()->SetTitleSize(0.1);
  g_nsigma1->GetXaxis()->SetTitleOffset(1.0);

  g_nsigma1->GetYaxis()->SetLabelSize(0.08);
  g_nsigma1->GetYaxis()->SetTitleSize(0.09);
  g_nsigma1->GetYaxis()->SetTitleOffset(0.25);

  c->cd();
  pB2->cd();
  g_nsigma2->SetTitle("");
  g_nsigma2->Draw("AP");
  g_nsigma2->GetXaxis()->SetLimits(lowerlimit, upperlimit);
  g_nsigma2->GetXaxis()->SetTitle("Time (mm/dd)");
  g_nsigma2->GetYaxis()->SetTitle("EHN");
  g_nsigma2->SetMinimum(2);
  g_nsigma2->SetMarkerStyle(20);

  g_nsigma2->GetXaxis()->SetTimeDisplay(1);
  g_nsigma2->GetXaxis()->SetTimeFormat("%m/%d");
  g_nsigma2->GetXaxis()->SetNdivisions(511);
  g_nsigma2->GetXaxis()->SetLabelSize(0.08);
  g_nsigma2->GetXaxis()->SetTitleSize(0.1);
  g_nsigma2->GetXaxis()->SetTitleOffset(1.0);

  g_nsigma2->GetYaxis()->SetLabelSize(0.08);
  g_nsigma2->GetYaxis()->SetTitleSize(0.09);
  g_nsigma2->GetYaxis()->SetTitleOffset(0.25);

  c->cd();
  pB3->cd();
  g_nsigma3->SetTitle("");
  g_nsigma3->Draw("AP");
  g_nsigma3->GetXaxis()->SetLimits(lowerlimit, upperlimit);
  g_nsigma3->GetXaxis()->SetTitle("Time (mm/dd)");
  g_nsigma3->GetYaxis()->SetTitle("EHZ");
  g_nsigma3->SetMinimum(1);
  g_nsigma3->SetMarkerStyle(20);
  // x-axis
  g_nsigma3->GetXaxis()->SetTimeDisplay(1);
  g_nsigma3->GetXaxis()->SetTimeFormat("%m/%d");
  g_nsigma3->GetXaxis()->SetNdivisions(511);
  g_nsigma3->GetXaxis()->SetLabelSize(0.08);
  g_nsigma3->GetXaxis()->SetTitleSize(0.1);
  g_nsigma3->GetXaxis()->SetTitleOffset(1.0);
  // y-axis
  g_nsigma3->GetYaxis()->SetLabelSize(0.08);
  g_nsigma3->GetYaxis()->SetTitleSize(0.09);
  g_nsigma3->GetYaxis()->SetTitleOffset(0.25);

  pT->SetLogy();
  pB->SetGrid(0, 1);
  pB2->SetGrid(0, 1);
  pB3->SetGrid(0, 1);

  pT->Modified();
  pB->Modified();
  pB2->Modified();
  pB3->Modified();
  c->Modified();
  c->SaveAs("plots/Pvalue_geodata.pdf");
  delete c;
}
