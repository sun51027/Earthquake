#ifndef EQ_H
#define EQ_H
#include "Constants.h"

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
  TH1   *AddHist(TDirectory *dir);
  TH1   *SetZeroBinContent(TH1 *hist);
  double FittingGaus(TH1 *h_diff);
  void   DoAnalysis(TH1 *Template, TDirectory *dir, TFile *ofile);
  void   WriteFile(TH1 *obj, TFile *ofile) { obj->Write(); }
	double FittingGausSigma(TH1 *h_diff);
	double FittingGausPeak(TH1 *h_diff);
  double PeakforCalibration(TH1 *obj, TFile *ofile, TString hist_name);
  double PeakforK40(TH1 *obj, TFile *ofile, TString hist_name, bool flag);
  void   DrawPlot();

//  static const double minK40; // 1.3
//  static const double maxK40; // 1.5
//  static const double minRadon;
//  static const double maxRadon;

private:

	//Histogram
  TH1D    *h_K40_peak_cali;
  TH1D    *h_K40_peak_uncali;
  TH1D    *h_diff;
  TH1D    *h_cfactor;
  TH1D    *h_cfactor_cali;

	//Graph
	TGraph		*g_diffvsTime;
	TGraph    *g_cfactor;
	TGraph    *g_cfactor_cali;
	TGraph		*g_K40_peak_cali;
	TGraph		*g_K40_peak_uncali;
	TGraph		*g_sigma_significant;
	TGraph		*g_pvalue;

	//variables
  double   K40_template = 0;
  double N_[4000], diff_[4000];
  double   cfactor[4000];
  double   cfactor_cali[4000];
  double   K40peak_cali[4000];
  double   K40peak_uncali[4000];
  double   peakforCali[4000]; // 2.2MeV, unknown peak
  double   nMoveBin_K40[4000];
  double   energyBin   = 5. / 1024.;
  int      N           = 0;
  int      h           = 0; // # of hour
  double   fluct_peak  = 0;
  double   fluct_sigma = 0;
	double 	 sigma_[4000];
	double 	 p_value_[4000];

  TString time_name[2500];
};
#endif
