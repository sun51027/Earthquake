#ifndef EQ_H
#define EQ_H
#include "Constants.h"
#include "DataReader.h"

#include "TDirectory.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TKey.h"
#include "TDatime.h"
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

class Earthquake : DataReader {
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
  //  void   ReadData();

private:
  // Histogram
  TH1D *h_K40_peak_cali;
  TH1D *h_K40_peak_uncali;
  TH1D *h_diff;
  TH1D *h_cfactor;
  TH1D *h_cfactor_cali;

  // Graph
  TGraph *g_diffvsTime;
  TGraph *g_cfactor;
  TGraph *g_cfactor_cali;
  TGraph *g_K40_peak_cali;
  TGraph *g_K40_peak_uncali;
  TGraph *g_sigma_significant;
  TGraph *g_pvalue;

  // variables
  double K40_template = 0;
  double diff_[4000];
  double cfactor[4000];
  double cfactor_cali[4000];
  double K40peak_cali[4000];
  double K40peak_uncali[4000];
  double peakforCali[4000]; // 2.2MeV, unknown peak
  double nMoveBin[4000];
  double energyBin   = 5. / 1024.;
  int    h           = 0; // # of hour
  double fluct_peak  = 0;
  double fluct_sigma = 0;
  double sigma_[4000];
  double p_value_[4000];
  double N_[4000];
  int    N = 0;

  TString datetime[2500];
  TDatime timeoffset;
};
#endif


//#ifdef Earthquake_cxx
inline TH1 *Earthquake::SetZeroBinContent(TH1 *hist)
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
//#endif
