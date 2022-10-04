#ifndef RADONDATA_H
#define RADONDATA_H
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
using namespace std;

class RadonData : DataReader {
public:
  RadonData() {}
  ~RadonData() {}
  TH1   *AddHist(TDirectory *dir);
  TH1   *AddHistforTemplate(TDirectory *dir);
  TH1   *SetZeroBinContent(TH1 *hist);
  double FittingGaus(TH1 *h_diff);
  void   DoAnalysis(TH1 *Template, TDirectory *dir, TFile *ofile);
  void   WriteFile(TH1 *obj, TFile *ofile) { obj->Write(); }
  double FittingGausSigma(TH1 *h_diff);
  double FittingGausPeak(TH1 *h_diff);
  double PeakforRadon2(TH1 *obj, TFile *ofile, TString hist_name, bool flag);
  double PeakforCalibration(TH1 *obj, TFile *ofile, TString hist_name, bool flag);
  double PeakforK40(TH1 *obj, TFile *ofile, TString hist_name, bool flag,int count);
  // double PeakforK40(TH1 *obj, TFile *ofile, TString hist_name, bool flag);
  void   DrawPlot();
  void   ErecoCalibration(TDirectory *dir, TDirectory *odir, ifstream &inputfile);
  void   SetRnDatetime(TString *datetime);

private:
  // Histogram
  TH1D *h_K40_peak_cali;
  TH1D *h_K40_peak_uncali;
  TH1D *h_diff;
  TH1D *h_diff_K40;
  TH1D *h_cfactor;
  TH1D *h_cfactor_cali;

  // Graph
  TGraph *g_diffvsTime;
  TGraph *g_NofK40;
  TGraph *g_Radon2_uncali; // 0.6 MeV
  TGraph *g_Radon2_cali;
  TGraph *g_twopoint_cali; // 2.2 MeV
  TGraph *g_twopoint_uncali;
  TGraph *g_cfactor;
  TGraph *g_cfactor_cali;
  TGraph *g_K40_peak_cali; // 1.4 MeV
  TGraph *g_K40_peak_uncali;
  TGraph *g_sigma_significant;
  TGraph *g_pvalue;

  // variables
  double K40_template = 0;
  double diff_[40000];
  double diff_K40_[40000];
  double cfactor[40000];
  double cfactor_cali[40000];

  double Radon2peak_uncali[40000]; // ~ 0.6 MeV
  double Radon2peak_cali[40000];
  double K40peak_uncali[40000];
  double K40peak_cali[40000];    // ~ 1.4 MeV
  double Twopoint_uncali[40000]; // 2.2MeV, unknown peak
  double Twopoint_cali[40000];
  double nsig_K40[40000];
  double nbkg_K40[40000];

  double nMoveBin[40000];
  double energyBin = 5. / NBINS;
  int    h         = 0; // # of spectrum for 2hrs
  int    N         = 0; // # of spectrum with obj!=0
  double N_[40000];     // time series
  double fluct_peak  = 0;
  double fluct_sigma = 0;
  double sigma_[40000];
  double p_value_[40000];

  TString datetime[25000];
  TDatime timeoffset;
};
#endif

//#ifdef RadonData_cxx
inline TH1 *RadonData::SetZeroBinContent(TH1 *hist)
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
inline void RadonData::SetRnDatetime(TString *datetime)
{
  ofstream ofs;
  ofs.open("doc/datetime.txt");
  if (!ofs.is_open()) {
    cout << "Fail to open txt file" << endl;
  } else {
    for (int i = 0; i < N; i++) {
      ofs << datetime[i] << endl;
    }
  }
  ofs.close();

  TString date_Rn[N];
  TString time_Rn[N];
  double  t[N];
  for (int rn = 0; rn < N; rn++) {
    date_Rn[rn] = datetime[rn];
    date_Rn[rn].Remove(8, 2);
    time_Rn[rn] = datetime[rn];
    time_Rn[rn].Remove(0, 8);
    time_Rn[rn].Insert(2, "0000");
    TTimeStamp timestamp(date_Rn[rn].Atoi(), time_Rn[rn].Atoi(), 0, kFALSE, 0);
    t[rn] = timestamp + 8 * 60 * 60;
  }
  timeoffset.Set(date_Rn[0].Atoi(), time_Rn[0].Atoi());
}
//#endif
