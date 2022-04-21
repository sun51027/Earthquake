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

// significant values
//const double Earthquake::minK40   = 1.3;
//const double Earthquake::maxK40   = 1.5;
//const double Earthquake::minRadon = 0.25;
//const double Earthquake::maxRadon = 0.8;


double Earthquake::FittingGausSigma(TH1 *h_diff)
{
  h_diff->Fit("gaus");
  double sigma = h_diff->GetFunction("gaus")->GetParameter(2);
  return sigma;
}

double Earthquake::FittingGausPeak(TH1 *h_diff)
{
  h_diff->Fit("gaus");
  double peak = h_diff->GetFunction("gaus")->GetParameter(1);
  return peak;
}

double Earthquake::PeakforCalibration(TH1 *obj, TFile *ofile, TString hist_name)
{

  double xmin = obj->GetXaxis()->FindBin(2.1);
  double xmax = obj->GetXaxis()->FindBin(2.4);

  cout << "# of bins " << xmax - xmin + 1 << endl;
  TH1D *cal = new TH1D("cal", "", xmax - xmin + 1, 2.1, 2.4);
  for (int i = 0; i < xmax - xmin + 1; i++) {
    cal->SetBinContent(i + 1, obj->GetBinContent(xmin + i));
    //    cout << "obj->GetBinContent(" << xmin + i << ") " << obj->GetBinContent(xmin + i) << endl;
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
  RooDataHist data("data", "data", x, RooFit::Import(*cal));
  RooPlot    *frame = x.frame();
  data.plotOn(frame);
  model.fitTo(data);
  model.plotOn(frame);
  frame->Draw();

  ofile->cd("cali_Hist");
  frame->Write(hist_name.Data());
  delete cal;

  double peak = mu.getVal();
  return peak;
}


double Earthquake::PeakforK40(TH1 *obj, TFile *ofile, TString hist_name, bool flag)
{

  RooRealVar x("x", "random variable", minK40, maxK40);

  // Gaussian model
  RooRealVar  mu("mu", "mean parameter", 1.2, minK40, maxK40);
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
  RooDataHist data("data", "data", x, RooFit::Import(*obj));
  RooPlot    *frame = x.frame();
  data.plotOn(frame);
  model.fitTo(data);
  model.plotOn(frame, RooFit::LineColor(kRed));
  frame->Draw();
  if (flag == 0) {
    ofile->cd("K40_uncali_fit");
    frame->Write(hist_name.Data());
  } else {
    ofile->cd("K40_cali_fit");
    frame->Write(hist_name.Data());
  }

  // Get peak of
  double peak = mu.getVal();
  return peak;
}

