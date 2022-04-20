// STL
using namespace std;
#include <iostream>
#include <fstream>
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
//#define DEBUG

const double Earthquake::minK40   = 1.3;
const double Earthquake::maxK40   = 1.5;
const double Earthquake::minRadon = 0.25;
const double Earthquake::maxRadon = 0.8;

TH1 *Earthquake::SetZeroBinContent(TH1 *hist)
{

  for (int i = 0; i < hist->GetNbinsX(); i++) {
    if (hist->GetBinContent(i) == 0 && hist->GetBinContent(i + 1) == 0 && hist->GetBinContent(i + 2) == 0) {
      continue;
    } else if (hist->GetBinContent(i) == 0) {
      hist->SetBinContent(i, (hist->GetBinContent(i - 1) + hist->GetBinContent(i + 1)) / 2);
    }
  }

  return hist;
}
void Earthquake::DoAnalysis(TH1 *Template, TDirectory *dir, TFile *ofile)
{

  TH1D    *h_K40_peak_cali   = new TH1D("h_K40_peak_cali", "", 100, 1.37, 1.47);
  TH1D    *h_K40_peak_uncali = new TH1D("h_K40_peak_uncali", "", 100, 1.37, 1.47);
  TH1D    *h_diff            = new TH1D("h_diff", "", 100, -10000, 10000);

  double K40_template =
    Template->Integral(Template->GetXaxis()->FindBin(minK40), Template->GetXaxis()->FindBin(maxK40));

  TKey    *keyAsObj, *keyAsObj2;
  TIter    next(dir->GetListOfKeys());
  TH1     *obj;
  Double_t x[4000], y[4000];
  double   cfactor[4000];
  double   K40peak_cali[4000];
  double   K40peak_uncali[4000];
  double   peakforCali[4000]; // 2.2MeV, unknown peak
  double   nMoveBin_K40[4000];
  double   energyBin = 5. / 1024.;
  int      N         = 0;
  int      h         = 0; // # of hour

  TString hist_name[2500];
  TString daily_name[2500];

  while ((keyAsObj = (TKey *)next())) {
    auto        key  = (TKey *)keyAsObj;
    TDirectory *dir2 = (TDirectory *)dir->Get(key->GetName());
    dir2->cd();
    TIter next2(dir2->GetListOfKeys());

    while ((keyAsObj2 = (TKey *)next2())) {

      auto key2 = (TKey *)keyAsObj2;
      obj       = (TH1 *)dir2->Get(key2->GetName()); // copy every th1 histogram to
      obj       = SetZeroBinContent(obj);            // fill the empty bin with average of adjacent bins

      //      if (h < 720 && obj->Integral() != 0) { // before July
      if (h > 1799 && obj->Integral() != 0) { // start from 9/15 h=1800)

        // set hist name ex. 12/25;  daily_name = 2021122522
        hist_name[N].Form("%s", key->GetName());
        hist_name[N].Remove(0, 4);
        hist_name[N].Insert(2, "/");
        daily_name[N].Form("%s%s", key->GetName(), key2->GetName());
        daily_name[N].Remove(10, 4);

        // get the calibration factor
        peakforCali[N] = PeakforCalibration(obj, ofile, daily_name[N]);
        cfactor[N]     = 2.22198 / peakforCali[N];                      // After Sep
        //        cfactor[N]     = 2.24337 / peakforCali[N];            // Apr-Aug
        //        cfactor[N]     = 2.25117 / peakforCali[N];             // Apr-Jun
        //
        for (int k = 0; k < 1024; k++) {
          nMoveBin_K40[k] = (1 - cfactor[N]) * obj->GetBinCenter(k + 1) / energyBin;
        }

        // calibrate hourly and show K40 peak
        TH1D *obj_cali = (TH1D *)(obj->Clone("obj_cali"));
        for (int j = 0; j < 1024; j++) {
          obj_cali->SetBinContent(j + 1, obj->GetBinContent(j + 1 + (int)nMoveBin_K40[j]));

          if ((int)nMoveBin_K40[j] != 0) {
          //  cout << "obj " << obj->GetBinContent(j + 1) << "\t" << (int)nMoveBin_K40[j] << "\t obj_cali "
          //       << obj_cali->GetBinContent(j + 1) << endl;
          }
        }
        K40peak_uncali[N] = PeakforK40(obj, ofile, daily_name[N], 0); 
        K40peak_cali[N]   = PeakforK40(obj_cali, ofile, daily_name[N], 1);
        h_K40_peak_cali->Fill(K40peak_cali[N]);
        h_K40_peak_uncali->Fill(K40peak_uncali[N]);

        /*----------------------------------
         *
         *			After calibration
         *
         * --------------------------------*/

        // Normalize template to yiels of every hour (use K40)
        double K40_obj_cali =
          obj_cali->Integral(obj_cali->GetXaxis()->FindBin(minK40), obj_cali->GetXaxis()->FindBin(maxK40));
        TH1D *scaledTemplate = (TH1D *)(Template->Clone("scaledTemplate"));
        scaledTemplate->Scale(K40_obj_cali / K40_template); // template scale to same as the hourly plot

        double nTemplateSig = scaledTemplate->Integral(scaledTemplate->GetXaxis()->FindBin(minRadon),
                                                       scaledTemplate->GetXaxis()->FindBin(maxRadon));
        double nDailySig =
          obj_cali->Integral(obj_cali->GetXaxis()->FindBin(minRadon), obj_cali->GetXaxis()->FindBin(maxRadon));
        double diff = nDailySig - nTemplateSig;
        x[N]        = (double)(N + 1); // number of 2hour
        y[N]        = diff;
        h_diff->Fill(diff);

        delete obj;
        delete scaledTemplate;
        N++;
      }
      h++;
    }
  }

	ofstream ofs;
	ofs.open("plot_name.txt");
	if(!ofs.is_open()){
					cout<<"Fail to open txt file"<<endl;
	}else{
					for(int i=0;i<N;i++){
									ofs<<daily_name[i]<<endl;
					}
	}
	ofs.close();


	ofile->cd("Analysis_plot");

//  h_diff->SetStats(0);
  //	h_diff->Fit("gaus");
  //	h_diff->GetFunction("gaus")->GetParameter(2);
  //	h_diff->Scale(h_diff->GetFunction("gaus")->GetParameter(2));
//  h_diff->Draw();
	h_diff->Write();
  //c->SaveAs("plots/h_diff.pdf");

  TGraph *gr = new TGraph(N, x, y);
	gr->SetName("g_diffvsTime");
	gr->Write();	
//  gr->GetXaxis()->SetLimits(0, N);
//  for (int i = 0; i <= N / 60; i++) {
//    gr->GetXaxis()->SetBinLabel(i * 60 + 1, hist_name[i * 60]);
//  }
//  gr->SetTitle("");
//  gr->Draw("AP");
//  c->SaveAs("plots/DiffvsTime.pdf");
//  delete c;

// draw corr factor
  TGraph  *corr = new TGraph(N, x, cfactor);
//  for (int i = 0; i <= N / 60; i++) {
//    corr->GetXaxis()->SetBinLabel(i * 60 + 1, hist_name[i * 60]);
//  }
//  corr->SetTitle("");
//  corr->Draw("AP");
//  corr->GetXaxis()->SetLimits(0, N);
//  corr->GetYaxis()->SetTitle("Calibration factor");
//  c2->SetGridy(1);
//  c2->SaveAs("plots/cfactor_beforSep.pdf");
	corr->SetName("g_cfactor");
	corr->Write();
//  delete c2;

// see if K40 is K40_peak around 1.4 MeV (peak)after calibration
  TCanvas     *c3 = new TCanvas("c3", "", 10, 10, 1500, 900);
  TPad        *pL = mgr::NewLeftPad();
  TPad        *pR = mgr::NewRightPad();
  TMultiGraph *mg = new TMultiGraph();

  TGraph *g_K40_peak_cali = new TGraph(N, x, K40peak_cali);
//  g_K40_peak_cali->SetMarkerColorAlpha(kRed, 1);
//  g_K40_peak_cali->SetMarkerStyle(8);
//  g_K40_peak_cali->GetXaxis()->SetLimits(0, N);
	g_K40_peak_cali->SetName("g_K40_peak_cali");
	g_K40_peak_cali->Write();

  TGraph *g_K40_peak_uncali = new TGraph(N, x, K40peak_uncali);
//  g_K40_peak_uncali->SetMarkerColor(kBlue);
//  g_K40_peak_uncali->SetMarkerStyle(22);
//  g_K40_peak_uncali->GetXaxis()->SetLimits(0, N);
	g_K40_peak_uncali->SetName("g_K40_peak_cali");
	g_K40_peak_uncali->Write();

//  c3->cd();
//  pL->Draw();
//  pR->Draw();
//
//  c3->cd();
//  pL->cd();
//  mg->Add(g_K40_peak_uncali);
//  mg->Add(g_K40_peak_cali);
//  for (int i = 0; i <= N / 80; i++) {
//    mg->GetXaxis()->SetBinLabel(i * 80 + 1, hist_name[i * 80]);
//  }
//  mg->SetMaximum(1.47);
//  mg->SetMinimum(1.37);
//  mg->GetYaxis()->SetTitle("K40 peak (MeV)");
//  mg->GetXaxis()->SetTitle("Time (mm/dd)");
//  mg->GetXaxis()->SetTitleOffset(1.6);
//  mg->GetXaxis()->SetNdivisions(510);
//  mg->Draw("AP");
//  TLegend *leg2 = new TLegend(0.65, 0.65, 0.80, 0.80);
//  leg2->SetBorderSize(0);
//  leg2->SetTextSize(0.04);
//  leg2->SetFillColorAlpha(0, 0);
//  leg2->AddEntry(g_K40_peak_uncali, "un-cali", "p");
//  leg2->AddEntry(g_K40_peak_cali, "Cali", "p");
//  leg2->Draw();
//  gPad->Update();
//
//  c3->cd();
//  pR->cd();
//  h_K40_peak_cali->SetFillColor(kRed);
//  h_K40_peak_cali->SetStats(0);
//  h_K40_peak_uncali->SetFillColor(kBlue);
//  h_K40_peak_uncali->SetStats(0);
//  h_K40_peak_uncali->Draw("hbar");
//  h_K40_peak_cali->Draw("same hbar");
	h_K40_peak_uncali->Write();
	h_K40_peak_cali->Write();

//  mgr::SetRightPlotAxis(h_K40_peak_uncali);
//  pR->Modified();
//  pL->Modified();
//  c3->SetGridy(1);
//  c3->Modified();
//  c3->SaveAs("plots/K40_cali_vs_uncali_beforeSep.pdf");
  //delete c3;
}
