// STL
#include <iostream>

// my header
#include "../include/RadonData.h"
#include "../include/Constants.h"

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
#include "RooExtendPdf.h"

double RadonData::FittingGausSigma(TH1 *h_diff)
{
  h_diff->Fit("gaus");
  double sigma = h_diff->GetFunction("gaus")->GetParameter(2);
  return sigma;
}

double RadonData::FittingGausPeak(TH1 *h_diff)
{
  h_diff->Fit("gaus");
  double peak = h_diff->GetFunction("gaus")->GetParameter(1);
  return peak;
}

double RadonData::PeakforCalibration(TH1 *obj, TFile *ofile, TString hist_name, bool flag)
{
  double xmin = obj->GetXaxis()->FindBin(XMINFIT_CALI);
  double xmax = obj->GetXaxis()->FindBin(XMAXFIT_CALI);

  TH1D *cal = new TH1D("cal", "", xmax - xmin + 1, XMINFIT_CALI, XMAXFIT_CALI);
  for (int i = 0; i < xmax - xmin + 1; i++) {
    cal->SetBinContent(i + 1, obj->GetBinContent(xmin + i));
  }
  // Fitting
  // observable
  RooRealVar x("x", "random variable", XMINFIT_CALI, XMAXFIT_CALI);

  // Gaussian model
  RooRealVar  mu("mu", "mean parameter", XCENFIT_CALI, XMINFIT_CALI, XMAXFIT_CALI); // 2.2
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
  model.fitTo(data, RooFit::Verbose(false));
  model.plotOn(frame);
  frame->Draw();

  if (flag == 0) {
    ofile->cd("cali_uncali_fit");
    frame->Write(hist_name.Data());
  } else {
    ofile->cd("cali_cali_fit");
    frame->Write(hist_name.Data());
  }
  delete cal;

  double peak = mu.getVal();
  return peak;
}

double RadonData::PeakforK40(TH1 *obj, TFile *ofile, TString hist_name, bool flag, int count)
// double RadonData::PeakforK40(TH1 *obj, TFile *ofile, TString hist_name, bool flag)
{
  // cout<<"count "<<count<<endl;
  RooRealVar x("x", "random variable", XMINFIT_K40, XMAXFIT_K40);
  RooRealVar nsig("nsig", "number of sig", 30000,0, 500000000);
  RooRealVar nbkg("nbkg", "number of bkg", 30000,0, 500000000);

  // Gaussian model
  RooRealVar  mu("mu", "mean parameter", XCENFIT_K40, XMINFIT_K40, XMAXFIT_K40); // 1.4
  RooRealVar  sigma("sigma", "width parameter", 0.1, 0.0, 0.3);
  RooGaussian gaus("gaus", "Gaussian PDF", x, mu, sigma);
 	RooExtendPdf gaus_ex("gaus_ex","gaus_ex",gaus,nsig);
  // Linear function: 1 + slope*x
  RooRealVar    slope("slope", "slope parameter", -1000, -100000, 100000);
  RooPolynomial linear("linear", "Linear function", x, RooArgSet(slope));
 	RooExtendPdf linear_ex("linear_ex","linear_ex",linear,nbkg);
  // Expo
  RooRealVar     par("par", "", -1, -10, 10);
  RooExponential expo("expo", "", x, par);
 	RooExtendPdf expo_ex("expo_ex","expo_ex",expo,nbkg);
  // add up: Gaussian + linear
  RooRealVar fraction("fraction", "fraction of Gaussian", 0.5, 0., 1.);
  RooAddPdf  model("model", "PDF model", RooArgList(gaus_ex, expo_ex));
  // RooAddPdf  model("model", "PDF model", RooArgList(gaus, expo), RooArgList(fraction));
  // RooAddPdf  model("model", "PDF model", RooArgList(gaus, linear), RooArgList(fraction));
  RooDataHist data("data", "data", x, RooFit::Import(*obj));
  RooPlot    *frame = x.frame();
  data.plotOn(frame);
  model.fitTo(data, RooFit::Verbose(false));
  // model.fitTo(data);
  model.plotOn(frame, RooFit::LineColor(kRed));
  frame->Draw();
  if (flag == 0) {
    ofile->cd("K40_uncali_fit");
    frame->Write(hist_name.Data());
  } else {
    ofile->cd("K40_cali_fit");
    frame->Write(hist_name.Data());
  }
   // cout<<"nsig "<<nsig.getVal()<<endl;;
   // cout<<"nbkg "<<nbkg.getVal()<<endl;;

  nsig_K40[count]=0.0;
  nbkg_K40[count]=0.0;
  nsig_K40[count]=nsig.getVal();
  nbkg_K40[count]=nbkg.getVal();
  // Get peak of
  double peak = mu.getVal();
  return peak;
}

double RadonData::PeakforRadon2(TH1 *obj, TFile *ofile, TString hist_name, bool flag)
{

  RooRealVar x("x", "random variable", XMINFIT_RADON2, XMAXFIT_RADON2);

  // Gaussian model
  RooRealVar  mu("mu", "mean parameter", XCENFIT_RADON2, XMINFIT_RADON2, XMAXFIT_RADON2);
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
  model.fitTo(data, RooFit::Verbose(false));
  // model.fitTo(data);
  model.plotOn(frame, RooFit::LineColor(kRed));
  frame->Draw();
  if (flag == 0) {
    ofile->cd("Radon2_uncali_fit");
    frame->Write(hist_name.Data());
  } else {
    ofile->cd("Radon2_cali_fit");
    frame->Write(hist_name.Data());
  }

  double peak = mu.getVal();
  return peak;
}
