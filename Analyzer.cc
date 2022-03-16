#include <iostream>

#include "TCanvas.h"
#include "TDirectory.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TKey.h"

using namespace std;
void writeFile(TH1 *obj, TFile *ofile)
{
  ofile->cd("AdjustedHist");
  obj->Write();
}

TH1 *setZeroBinContent(TH1 *hist)
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

double FittingScale(TH1 *h_diff)
{
  TCanvas *canvas = new TCanvas("canvas", "", 800, 600);
  h_diff->Fit("gaus");
  h_diff->Draw();
  canvas->SaveAs("h_fitting_gaus.pdf");

  // h_diff scale yields
  TH1D *h_scale = new TH1D("h_scale", "", 100, -10000, 10000);
  for (int i = 0; i < 100; i++) {
    h_scale->SetBinContent(i + 1, h_diff->GetBinContent(i + 1) / (h_diff->GetFunction("gaus")->GetParameter(2)));
  }
  h_scale->Draw();
  h_scale->SetStats(0);
  canvas->SaveAs("h_scale.pdf");

  double sigma = h_diff->GetFunction("gaus")->GetParameter(2);
  return sigma;
}

double calibration(TH1 *obj)
{

  obj->Fit("gaus", "R", "", 2.1, 2.4);
  double cfactor = obj->GetFunction("gaus")->GetParameter(1) / 2.23118;

  cout << cfactor << endl;
  return cfactor;
}

const double minK40   = 1.3;
const double maxK40   = 1.5;
const double minRadon = 0.25;
const double maxRadon = 0.8;

void doAnalysis(TH1 *Template, TH1 *h_diff, TDirectory *dir, TFile *ofile)
{
  TCanvas *c   = new TCanvas("canvas", "", 800, 600);
  double   K40 = Template->Integral(Template->GetXaxis()->FindBin(minK40), Template->GetXaxis()->FindBin(maxK40));

  TKey    *keyAsObj, *keyAsObj2;
  TIter    next(dir->GetListOfKeys());
  TH1     *obj;
  Double_t x[4000], y[4000];
  Double_t cfactor[4000];
  Int_t    n = 2500;
  int      i = 0;
  int      h = 0; // # of hour

  TString hist_name[2500];

  double testx[4000];
  double testy[4000];

  while ((keyAsObj = (TKey *)next())) {
    auto        key  = (TKey *)keyAsObj;
    TDirectory *dir2 = (TDirectory *)dir->Get(key->GetName());
    dir2->cd();
    TIter next2(dir2->GetListOfKeys());

    while ((keyAsObj2 = (TKey *)next2())) {

      if (h > 1210) { // start from 9/14 (h=1211)

        auto key2 = (TKey *)keyAsObj2;
        obj       = (TH1 *)dir2->Get(key2->GetName()); // copy every th1 histogram to
        obj       = setZeroBinContent(obj);            // fill the empty bin with average of adjacent bins

        // calibrate every hour
        cfactor[i] = calibration(obj);

        // Normalize template to yiels of every hour
        double obj_K40        = obj->Integral(obj->GetXaxis()->FindBin(minK40), obj->GetXaxis()->FindBin(maxK40));
        double factor         = obj_K40 / K40;
        TH1D  *ScaledTemplate = (TH1D *)(Template->Clone("ScaledTemplate"));
        ScaledTemplate->Scale(obj_K40 / K40); // template scale to same as the hourly plot
        double nTemplateSig = ScaledTemplate->Integral(ScaledTemplate->GetXaxis()->FindBin(minRadon),
                                                       ScaledTemplate->GetXaxis()->FindBin(maxRadon));
        double nDailySig    = obj->Integral(obj->GetXaxis()->FindBin(minRadon), obj->GetXaxis()->FindBin(maxRadon));
        double diff         = nDailySig - nTemplateSig;

        hist_name[i].Form("%s", key->GetName());
        hist_name[i].Remove(0, 4);
        hist_name[i].Insert(2, "/");

        x[i] = (double)(i + 1); // number of 2hour
        y[i] = diff;

        i++;
        h_diff->Fill(diff);

        delete obj;
        delete ScaledTemplate;
      }
      h++;
    }
  }
	cout<<"h "<<h<<endl;
	cout<<"i "<<i<<endl;

  TGraph *gr = new TGraph(n, x, y);

  for (int i = 0; i < h/100; i++) {
    gr->GetXaxis()->SetBinLabel(i * 100 + 1, hist_name[i * 100]);
  }

  //   for (int i = 0; i < n; i++) {
  //      cout << hist_name[i]<<" "<< i << " " << gr->GetPointX(i) << "\t" << gr->GetPointY(i) << " GetBinLabel "<<
  //      gr->GetXaxis()->GetBinLabel(i+1)<<endl;
  //   }
  gr->GetXaxis()->SetLimits(0,h);

  gr->Draw("AP");
  c->SetGrid();
  c->SaveAs("DiffvsTime.pdf");

  TCanvas *c2 = new TCanvas("c2", "", 800, 600);

  TGraph *corr = new TGraph(n, x, cfactor);
  for (int i = 0; i < 25; i++) {
    corr->GetXaxis()->SetBinLabel(i * 100 + 1, hist_name[i * 100]);
  }
  //  for (int i = 0; i < n; i++) {
  //    cout << hist_name[i] << " " << i << " " << cfactor[i] << "\t" << corr->GetPointY(i) << " GetBinLabel "
  //         << corr->GetXaxis()->GetBinLabel(i + 1) << endl;
  //  }
  //	corr->SetMarkerStyle(20);
  corr->Draw("AP");
  corr->GetXaxis()->SetLimits(0, 2500);
  c2->SetGrid(1, 1);
  c2->SaveAs("Corr_factor.pdf");
  //--------------debug-----------------
  /*
    TGraph *test = new TGraph(t,testy,testx);
  //	test->Draw("AP");
  //	c2->SaveAs("test.pdf");

    TH1D* h_test = new TH1D("h_test","",2500,0,2500);
    for(int i = 0 ; i < n ; i++){
      h_test -> SetBinContent(i+1,cfactor[i]);
    }
  //	h_test->Draw();
  //	c2->SaveAs("h_test.pdf");
  */
  //	FittingScale(h_diff);
}
// void Analyzer(){
int main()
{

  TFile *ofile = new TFile("o.root", "recreate");
  ofile->mkdir("AdjustedHist");

  TFile      *fin1 = new TFile("output.root");
  TDirectory *dir  = (TDirectory *)fin1->Get("HistoCh0");
  dir->cd();

  // get template
  TFile *fin2 = new TFile("template.root");
  fin2->cd();
  TH1 *Template = (TH1 *)fin2->Get("Template");

  // do analysis
  TH1D *h_diff = new TH1D("h_diff", "", 100, -10000, 10000);
  doAnalysis(Template, h_diff, dir, ofile);

  // fitting

  return 0;
}
