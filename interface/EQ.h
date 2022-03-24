#ifndef EQ_H
#define EQ_H

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
using namespace std;
class Earthquake {
public:
  Earthquake() {}
  ~Earthquake() {}

  TH1 *addHist(TDirectory *dir);

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

  //  void   calibration(TH1 *Template, TFile *ofile);
  double FittingScale(TH1 *h_diff)
  {
    h_diff->Fit("gaus");
    // h_diff scale yields
//    TH1D *h_scale = new TH1D("h_scale", "", 100, -10000, 10000);
//    for (int i = 0; i < 100; i++) {
//      h_scale->SetBinContent(i + 1, h_diff->GetBinContent(i + 1) / (h_diff->GetFunction("gaus")->GetParameter(2)));
//    }
    double sigma = h_diff->GetFunction("gaus")->GetParameter(2);
//		delete h_scale;
    return sigma;
  }

  // double calibration(TH1 *obj)
  // {

  //   obj->Fit("gaus", "R", "", 2.1, 2.4);
  //   double cfactor = obj->GetFunction("gaus")->GetParameter(1) / 2.22198;

  //   //    cout << cfactor << endl;
  //   return cfactor;
  // }

  void doAnalysis(TH1 *Template, TH1 *h_diff, TDirectory *dir, TFile *ofile);

  void writeFile(TH1 *obj, TFile *ofile) { obj->Write(); }

  double calibration(TH1 *obj, TFile *ofile, TString hist_name)
  {

    double   xmin = obj->GetXaxis()->FindBin(2.1);
    double   xmax = obj->GetXaxis()->FindBin(2.4);
    cout << "xmax " << xmax << endl;

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
    double cfactor = 2.22198 / peak;

    return cfactor;
  }
  double stability_K40(TH1 *obj, TFile *ofile, TString hist_name)
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
//    model.plotOn(frame,LineColor(kBlue));
    frame->Draw();

    ofile->cd("stability_K40");

    frame->Write(hist_name.Data());
		//Get peak of 
    double peak = mu.getVal();
    return peak;
  }

private:
  static const double minK40; // 1.3
  static const double maxK40; // 1.5
  static const double minRadon;
  static const double maxRadon;
};
#endif
