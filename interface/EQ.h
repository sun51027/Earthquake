#ifndef EQ_H
#define EQ_H

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
using namespace std;


class Earthquake {
public:
  Earthquake() {}
  ~Earthquake() {}

  TH1 *AddHist(TDirectory *dir);

  TH1 *SetZeroBinContent(TH1 *hist);
  //{

  //  for (int i = 0; i < hist->GetNbinsX(); i++) {
  //    if (hist->GetBinContent(i) == 0 && hist->GetBinContent(i + 1) == 0 && hist->GetBinContent(i + 2) == 0) {
  //      continue;
  //    } else if (hist->GetBinContent(i) == 0) {
  //      hist->SetBinContent(i, (hist->GetBinContent(i - 1) + hist->GetBinContent(i + 1)) / 2);
  //    }
  //  }

  //  return hist;
  //}

  //  void   calibration(TH1 *Template, TFile *ofile);
  double FittingGaus(TH1 *h_diff);
//  {
//    h_diff->Fit("gaus");
//    double sigma = h_diff->GetFunction("gaus")->GetParameter(2);
//    return sigma;
//  }


  void DoAnalysis(TH1 *Template, TDirectory *dir, TFile *ofile);

  void WriteFile(TH1 *obj, TFile *ofile) { obj->Write(); }

  double PeakforCalibration(TH1 *obj, TFile *ofile, TString hist_name);
//  {
//
//    double   xmin = obj->GetXaxis()->FindBin(2.1);
//    double   xmax = obj->GetXaxis()->FindBin(2.4);
//
//    cout << "# of bins " << xmax - xmin + 1 << endl;
//    TH1D *cal = new TH1D("cal", "", xmax - xmin + 1, 2.1, 2.4);
//    for (int i = 0; i < xmax - xmin + 1; i++) {
//      cal->SetBinContent(i + 1, obj->GetBinContent(xmin + i));
//      //    cout << "obj->GetBinContent(" << xmin + i << ") " << obj->GetBinContent(xmin + i) << endl;
//    }
//    // Fitting
//    // observable
//    RooRealVar x("x", "random variable", 2.1, 2.4);
//
//    // Gaussian model
//    RooRealVar  mu("mu", "mean parameter", 2.2, 2.1, 2.4);
//    RooRealVar  sigma("sigma", "width parameter", 0.1, 0.0, 0.3);
//    RooGaussian gaus("gaus", "Gaussian PDF", x, mu, sigma);
//
//    // Linear function: 1 + slope*x
//    RooRealVar    slope("slope", "slope parameter", -1000, -100000, 100000);
//    RooPolynomial linear("linear", "Linear function", x, RooArgSet(slope));
//    // Expo
//    RooRealVar     par("par", "", -1, -10, 10);
//    RooExponential expo("expo", "", x, par);
//    // add up: Gaussian + linear
//    RooRealVar fraction("fraction", "fraction of Gaussian", 0.5, 0., 1.);
//    RooAddPdf  model("model", "PDF model", RooArgList(gaus, expo), RooArgList(fraction));
//    // RooAddPdf  model("model", "PDF model", RooArgList(gaus, linear), RooArgList(fraction));
//    RooDataHist data("data", "data", x, RooFit::Import(*cal));
//    RooPlot    *frame = x.frame();
//    data.plotOn(frame);
//    model.fitTo(data);
//    model.plotOn(frame);
//    frame->Draw();
//
//    ofile->cd("cali_Hist");
//    frame->Write(hist_name.Data());
//		delete cal;
//
//    double peak = mu.getVal();
//    return peak;
//  }
  double PeakforK40(TH1 *obj, TFile *ofile, TString hist_name, bool flag);
//  {
//
//    RooRealVar x("x", "random variable", minK40, maxK40);
//
//    // Gaussian model
//    RooRealVar  mu("mu", "mean parameter", 1.2, minK40, maxK40);
//    RooRealVar  sigma("sigma", "width parameter", 0.1, 0.0, 0.3);
//    RooGaussian gaus("gaus", "Gaussian PDF", x, mu, sigma);
//
//    // Linear function: 1 + slope*x
//    RooRealVar    slope("slope", "slope parameter", -1000, -100000, 100000);
//    RooPolynomial linear("linear", "Linear function", x, RooArgSet(slope));
//    // Expo
//    RooRealVar     par("par", "", -1, -10, 10);
//    RooExponential expo("expo", "", x, par);
//    // add up: Gaussian + linear
//    RooRealVar fraction("fraction", "fraction of Gaussian", 0.5, 0., 1.);
//    RooAddPdf  model("model", "PDF model", RooArgList(gaus, expo), RooArgList(fraction));
//    // RooAddPdf  model("model", "PDF model", RooArgList(gaus, linear), RooArgList(fraction));
//    RooDataHist data("data", "data", x, RooFit::Import(*obj));
//    RooPlot    *frame = x.frame();
//    data.plotOn(frame);
//    model.fitTo(data);
//	  model.plotOn(frame,RooFit::LineColor(kRed));
//    frame->Draw();
//		if(flag==0){
//	    ofile->cd("K40_uncali_fit");
//    	frame->Write(hist_name.Data());
//		}
//		else{
//			ofile->cd("K40_cali_fit");
//    	frame->Write(hist_name.Data());
//		}
//
//		//Get peak of 
//    double peak = mu.getVal();
//    return peak;
//  }
//
  static const double minK40; // 1.3
  static const double maxK40; // 1.5
  static const double minRadon;
  static const double maxRadon;
};
#endif
