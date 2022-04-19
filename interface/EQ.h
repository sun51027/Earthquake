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
  double FittingGaus(TH1 *h_diff);
  void DoAnalysis(TH1 *Template, TDirectory *dir, TFile *ofile);
  void WriteFile(TH1 *obj, TFile *ofile) { obj->Write(); }
  double PeakforCalibration(TH1 *obj, TFile *ofile, TString hist_name);
  double PeakforK40(TH1 *obj, TFile *ofile, TString hist_name, bool flag);

  static const double minK40; // 1.3
  static const double maxK40; // 1.5
  static const double minRadon;
  static const double maxRadon;
};
#endif
