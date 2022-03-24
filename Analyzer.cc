#include <iostream>
#include "interface/EQ.h"
#include "TCanvas.h"
#include "TDirectory.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TH1D.h"
#include "TKey.h"
#include "TStyle.h"

using namespace std;
const double Earthquake::minK40   = 1.3;
const double Earthquake::maxK40   = 1.5;
const double Earthquake::minRadon = 0.25;
const double Earthquake::maxRadon = 0.8;

void Earthquake::doAnalysis(TH1 *Template, TH1 *h_diff, TDirectory *dir, TFile *ofile)
{

  TCanvas *c   = new TCanvas("canvas", "", 800, 600);
  double   K40 = Template->Integral(Template->GetXaxis()->FindBin(minK40), Template->GetXaxis()->FindBin(maxK40));

  TKey    *keyAsObj, *keyAsObj2;
  TIter    next(dir->GetListOfKeys());
  TH1     *obj;
  Double_t x[4000], y[4000];
  double   cfactor[4000];
  double   cali_peak[4000];
  double   uncali_peak[4000];
  int      N = 0;
  int      h = 0; // # of hour

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
      obj       = setZeroBinContent(obj);            // fill the empty bin with average of adjacent bins

      if (h > 1799 && obj->Integral() != 0) { // start from 9/15 h=1800)

        // set hist name ex. 12/25
        hist_name[N].Form("%s", key->GetName());
        hist_name[N].Remove(0, 4);
        hist_name[N].Insert(2, "/");
        //
        daily_name[N].Form("%s%s", key->GetName(), key2->GetName());
        daily_name[N].Remove(10, 4); // 20211225 22

        // calibrate plots every 2 hours, get the cali factor
        cfactor[N] = calibration(obj, ofile, daily_name[N]);

        // Normalize template to yiels of every hour
        double obj_K40 = obj->Integral(obj->GetXaxis()->FindBin(minK40), obj->GetXaxis()->FindBin(maxK40));

        TH1D *ScaledTemplate = (TH1D *)(Template->Clone("ScaledTemplate"));
        ScaledTemplate->Scale(obj_K40 / K40); // template scale to same as the hourly plot

        // calibrate hourly K40 peak
        //        TH1D *obj_cali = (TH1D *)(obj->Clone("obj_cali"));
        //        for (int j = 0; j < 1024; j++) {
        //          obj_cali->SetBinContent(j + 1, (obj->GetBinContent(j + 1)) * cfactor[N]);
        //          // cout<<obj->GetBinContent(j+1)<<"\t"<< cfactor[N] <<"\t"<< obj_cali->GetBinContent(j+1)<<endl;
        //        }

        // input cfactor and fit K40 to get mean(peak), to see the stability.
        // return peak of K40 hourly
        uncali_peak[N] = stability_K40(obj, ofile, daily_name[N]);
        //        cali_peak[N]   = stability_K40(obj_cali, ofile, daily_name[N]);

        double nTemplateSig = ScaledTemplate->Integral(ScaledTemplate->GetXaxis()->FindBin(minRadon),
                                                       ScaledTemplate->GetXaxis()->FindBin(maxRadon));
        double nDailySig =
          obj->Integral(obj->GetXaxis()->FindBin(minRadon), obj->GetXaxis()->FindBin(maxRadon));
        double diff = nDailySig - nTemplateSig;
        x[N] = (double)(N + 1); // number of 2hour
        y[N] = diff;
        h_diff->Fill(diff);

        delete obj;
        delete ScaledTemplate;
        N++;
      }
      h++;
    }
  }

	h_diff->SetStats(0);
//	h_diff->Fit("gaus");
//	h_diff->GetFunction("gaus")->GetParameter(2);
//	h_diff->Scale(h_diff->GetFunction("gaus")->GetParameter(2));
	h_diff->Draw();
	c->SaveAs("h_diff.pdf");

  TGraph *gr = new TGraph(N, x, y);
  gr->GetXaxis()->SetLimits(0, N);
  for (int i = 0; i <= N / 60; i++) {
    gr->GetXaxis()->SetBinLabel(i * 60 + 1, hist_name[i * 60]);
  //  cout << i << " " << gr->GetXaxis()->GetBinLabel(i * 60 + 1) << endl;
  }
	gr->SetTitle("");
  gr->Draw("AP");
  c->SaveAs("DiffvsTime.pdf");
  delete c;

  // draw corr factor
  TCanvas *c2   = new TCanvas("c2", "", 900, 600);
  TGraph  *corr = new TGraph(N, x, cfactor);
  for (int i = 0; i <= N / 60; i++) {
    corr->GetXaxis()->SetBinLabel(i * 60 + 1, hist_name[i * 60]);
  }
	corr->SetTitle("");
  corr->Draw("AP");
  corr->GetXaxis()->SetLimits(0, N);
	corr->GetYaxis()->SetTitle("calibration factor");
  c2->SetGridy(1);
  c2->SaveAs("cfactor.pdf");
  delete c2;

  // see if K40 is stable around 1.4 MeV (peak)after calibration
  ////  TMultiGraph *mg = new TMultiGraph();
  //
  //  TGraph *stable = new TGraph(N, x, cali_peak);
  //  stable->SetMarkerColor(kRed);
  //  stable->SetMarkerSize(8);
  //  stable->GetXaxis()->SetLimits(0, N);
  //    stable->SetMaximum(1.43);
  //    stable->SetMinimum(1.37);

  TCanvas     *c3 = new TCanvas("c3", "", 900, 600);
  TGraph *stable_uncali = new TGraph(N, x, uncali_peak);
  stable_uncali->SetMarkerColor(kBlue);
  stable_uncali->SetMarkerSize(8);
  stable_uncali->GetXaxis()->SetLimits(0, N);
	stable_uncali->GetYaxis()->SetTitle("K40 peak (MeV)");
  stable_uncali->Draw("AP");
  stable_uncali->SetMaximum(1.43);
  stable_uncali->SetMinimum(1.38);

  for (int i = 0; i <= N / 60; i++) {
    stable_uncali->GetXaxis()->SetBinLabel(i * 60 + 1, hist_name[i * 60]);
  }
  //  mg->Add(stable);
  //  mg->Add(stable_uncali);
  //  mg->Draw("AP");
	
  c3->SetGridy(1);
  c3->SaveAs("K40_stability.pdf");
  delete c3;
  //  for (int i = 0; i < N; i++) {
  //		cout<<stable->GetPointY(i)<<endl;
  //  }
}
int main()
{
  // outputfile
  TFile *ofile = new TFile("oAnalyzer.root", "recreate");
  ofile->mkdir("cali_Hist");
  ofile->mkdir("stability_K40");
  ofile->cd();
  // inputfile
  TFile      *fin1 = new TFile("output.root");
  TDirectory *dir  = (TDirectory *)fin1->Get("HistoCh0");
  dir->cd();

  // get template
  TFile *fin2     = new TFile("template.root");
  TH1   *Template = (TH1 *)fin2->Get("Template");

  Earthquake EQ;

  // do analysis
  TH1D *h_diff = new TH1D("h_diff", "", 100, -10000, 10000);
  EQ.doAnalysis(Template, h_diff, dir, ofile);

  return 0;
}
