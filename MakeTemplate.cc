#include <iostream>

#include "TCanvas.h"
#include "TDirectory.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TKey.h"
#include "RooFit.h"
#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooPolynomial.h"
#include "RooExponential.h"
#include "RooArgSet.h"
#include "RooAddPdf.h"
#include "RooArgList.h"
#include "RooPlot.h"
#include "RooDataHist.h"
using namespace RooFit;
using namespace std;

TH1 *addHist(TDirectory *dir)
{
  int count = 0;

  TH1D *Template = new TH1D("Template", "", 1024, -0.5, 4.5);

  TKey *keyAsObj, *keyAsObj2;
  TIter next(dir->GetListOfKeys());
  TH1  *obj;

  while ((keyAsObj = (TKey *)next())) {
    auto        key  = (TKey *)keyAsObj;
    TDirectory *dir2 = (TDirectory *)dir->Get(key->GetName());
    dir2->cd();
    TIter next2(dir2->GetListOfKeys());
    while ((keyAsObj2 = (TKey *)next2())) {
      count++;
      auto key2 = (TKey *)keyAsObj2;
      obj       = (TH1 *)dir2->Get(key2->GetName()); // copy every th1 histogram to
                                                     // obj
      double obj_K40 = obj->Integral();

      if (count > 1800) { // start from 9/15
        Template->Add(obj);
      }
      delete obj;
    }
  }

  Template->SetXTitle("Energy (MeV)");
  Template->SetYTitle("Entries");
  // Template->SetStats(0);
  cout << count << endl;
  return Template;
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
void calibration(TH1 *Template, TFile *ofile)
{

  TCanvas *c    = new TCanvas("c", "", 800, 600);
  double   xmin = Template->GetXaxis()->FindBin(2.1);
  double   xmax = Template->GetXaxis()->FindBin(2.4);
  cout << "xmax " << xmax << endl;
  cout << "xmin " << xmin << endl;
  cout << "# of bins " << xmax - xmin + 1 << endl;
  TH1D *cal = new TH1D("cal", "", xmax - xmin + 1, 2.1, 2.4);
  for (int i = 0; i < xmax - xmin + 1; i++) {
    cal->SetBinContent(i + 1, Template->GetBinContent(xmin + i));
    //    cout << "Template->GetBinContent(" << xmin + i << ") " << Template->GetBinContent(xmin + i) << endl;
  }
  // Fitting
  // observable
  RooRealVar x("x", "random variable", 2.1, 2.4);

  // Gaussian model
  RooRealVar  mu("mu", "mean parameter", 2.2, 2.1, 2.4);
  RooRealVar  sigma("sigma", "width parameter", 0.1, 0.0, 0.3);
  RooGaussian gaus("gaus", "Gaussian PDF", x, mu, sigma);

  // Linear function: 1 + slope*x
  RooRealVar    slope("slope", "slope parameter", -1000, -100000, 100000);
  RooPolynomial linear("linear", "Linear function", x, RooArgSet(slope));
  // Expo
  RooRealVar     par("par", "", -1, -10, 10);
  RooExponential expo("expo", "", x, par);
  // add up: Gaussian + linear
  RooRealVar fraction("fraction", "fraction of Gaussian", 0.5, 0., 1.);
  RooAddPdf  model("model", "PDF model", RooArgList(gaus, expo), RooArgList(fraction));
  // RooAddPdf  model("model", "PDF model", RooArgList(gaus, linear), RooArgList(fraction));
  RooDataHist data("data", "data", x, Import(*cal));
  RooPlot    *frame = x.frame();
  data.plotOn(frame);
  model.fitTo(data);
  model.plotOn(frame);
  frame->Draw();
  c->SaveAs("cal_new.pdf");
  //  return cfactor;
}
int main()
{

  TFile *ofile = new TFile("o.root", "recreate");
  ofile->mkdir("AdjustedHist");

  TFile      *file = new TFile("output.root");
  TDirectory *dir  = (TDirectory *)file->Get("HistoCh0");
  dir->cd();

  // make a template
  TH1 *Template = addHist(dir);
  Template      = setZeroBinContent(Template);
  calibration(Template, ofile);
}
