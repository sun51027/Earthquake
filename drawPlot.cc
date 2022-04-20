// STL
using namespace std;
#include <iostream>

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
void drawPlot(){

	TFile *fin = new TFile("oAnalyzer.root");
	TDirectory *dir = (TDirectory*)fin->Get("Analysis_plot");
	dir->cd();
	
	// difference vs time
  TCanvas *c = new TCanvas("canvas", "", 800, 600);
	TGraph *gr = (TGraph*)dir->Get("DiffvsTime");
//  gr->GetXaxis()->SetLimits(0, N);
//  for (int i = 0; i <= N / 60; i++) {
//    gr->GetXaxis()->SetBinLabel(i * 60 + 1, hist_name[i * 60]);
//  }
  gr->SetTitle("");
  gr->Draw("AP");
  c->SaveAs("plots/DiffvsTime.pdf");

	// Correlation factor
//  corr->SetTitle("");
//  corr->Draw("AP");
//  corr->GetXaxis()->SetLimits(0, N);
//  corr->GetYaxis()->SetTitle("Calibration factor");
//  c2->SetGridy(1);
//  c2->SaveAs("plots/cfactor_beforSep.pdf");
}
