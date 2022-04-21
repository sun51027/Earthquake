// STL
using namespace std;
#include <iostream>
#include <fstream>
// my header
#include "interface/EQ.h"

// ROOT include
#include "TCanvas.h"
#include "TDirectory.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TPad.h"
#include "TLegend.h"
#include "TH1D.h"
#include "TKey.h"
#include "TStyle.h"
//#include "rootlogon.h"
// using namespace mgr;
//#define DEBUG

const double Earthquake::minK40   = 1.3;
const double Earthquake::maxK40   = 1.5;
const double Earthquake::minRadon = 0.25;
const double Earthquake::maxRadon = 0.8;

TH1 *Earthquake::SetZeroBinContent(TH1 *hist)
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
void Earthquake::DoAnalysis(TH1 *Template, TDirectory *dir, TFile *ofile)
{

  h_K40_peak_cali   = new TH1D("h_K40_peak_cali", "", 100, 1.37, 1.44);
  h_K40_peak_uncali = new TH1D("h_K40_peak_uncali", "", 100, 1.37, 1.44);
  h_diff            = new TH1D("h_diff", "", 100, -20000, 20000);
  K40_template      = Template->Integral(Template->GetXaxis()->FindBin(minK40), Template->GetXaxis()->FindBin(maxK40));

  TKey *keyAsObj, *keyAsObj2;
  TIter next(dir->GetListOfKeys());
  TH1  *obj;

  while ((keyAsObj = (TKey *)next())) {
    auto        key  = (TKey *)keyAsObj;
    TDirectory *dir2 = (TDirectory *)dir->Get(key->GetName());
    dir2->cd();
    TIter next2(dir2->GetListOfKeys());

    while ((keyAsObj2 = (TKey *)next2())) {

      auto key2 = (TKey *)keyAsObj2;
      obj       = (TH1 *)dir2->Get(key2->GetName()); // copy every th1 histogram to
      obj       = SetZeroBinContent(obj);            // fill the empty bin with average of adjacent bins

      //      if (h < 720 && obj->Integral() != 0) { // before July
      if (h > 1799 && obj->Integral() != 0) { // start from 9/15 h=1800)

        // set hist name ex. 12/25;  time_name = 2021122522
        time_name[N].Form("%s%s", key->GetName(), key2->GetName());
        time_name[N].Remove(10, 4);

        // get the calibration factor
        peakforCali[N] = PeakforCalibration(obj, ofile, time_name[N]);
        cfactor[N]     = 2.22198 / peakforCali[N]; // After Sep
        //        cfactor[N]     = 2.24337 / peakforCali[N];            // Apr-Aug
        //        cfactor[N]     = 2.25117 / peakforCali[N];             // Apr-Jun
        //
        for (int k = 0; k < 1024; k++) {
          nMoveBin_K40[k] = (1 - cfactor[N]) * obj->GetBinCenter(k + 1) / energyBin;
        }

        // calibrate hourly and show K40 peak
        TH1D *obj_cali = (TH1D *)(obj->Clone("obj_cali"));
        for (int j = 0; j < 1024; j++) {
          obj_cali->SetBinContent(j + 1, obj->GetBinContent(j + 1 + (int)nMoveBin_K40[j]));

          if ((int)nMoveBin_K40[j] != 0) {
            //  cout << "obj " << obj->GetBinContent(j + 1) << "\t" << (int)nMoveBin_K40[j] << "\t obj_cali "
            //       << obj_cali->GetBinContent(j + 1) << endl;
          }
        }
        K40peak_uncali[N] = PeakforK40(obj, ofile, time_name[N], 0);
        K40peak_cali[N]   = PeakforK40(obj_cali, ofile, time_name[N], 1);
        h_K40_peak_cali->Fill(K40peak_cali[N]);
        h_K40_peak_uncali->Fill(K40peak_uncali[N]);

        /**********************************************************
            After the calibration, calculate the difference
            between template and each plots.
         **********************************************************/

        // Normalize template to yiels of every hour (use K40)
        double K40_obj_cali =
          obj_cali->Integral(obj_cali->GetXaxis()->FindBin(minK40), obj_cali->GetXaxis()->FindBin(maxK40));

        TH1D *scaledTemplate = (TH1D *)(Template->Clone("scaledTemplate"));
        scaledTemplate->Scale(K40_obj_cali / K40_template); // template scale to same as the hourly plot

        double nTemplateSig = scaledTemplate->Integral(scaledTemplate->GetXaxis()->FindBin(minRadon),
                                                       scaledTemplate->GetXaxis()->FindBin(maxRadon));
        double nDailySig =
          obj_cali->Integral(obj_cali->GetXaxis()->FindBin(minRadon), obj_cali->GetXaxis()->FindBin(maxRadon));
        double diff = nDailySig - nTemplateSig;
        N_[N]       = (double)(N + 1); // number of 2hour
        diff_[N]    = diff;
        h_diff->Fill(diff);

        // calculate the sigma

        delete obj;
        delete scaledTemplate;
        N++;
      }
      h++;
    }
  }

  /**********************************************************
      Calculate the sigma between template and each plots.
   **********************************************************/

  fluct_peak  = Earthquake::FittingGausPeak(h_diff);
  fluct_sigma = Earthquake::FittingGausSigma(h_diff);
  for (int i = 0; i < N; i++) {
    if (diff_[i] < fluct_peak)
      sigma_[i] = 0;
    else {
      sigma_[i] = (diff_[i] - fluct_peak) / fluct_sigma;
      cout << "diff_[" << i << "] " << diff_[i] << "\t - peak " << fluct_peak << "\t / sigma " << fluct_sigma
           << "  = sigma[" << i << "] = " << sigma_[i] << endl;
    }
  }
	g_sigma_significant = new TGraph(N,N_,sigma_);

	

  /**********************************************************
      Write object into output files
   **********************************************************/

  // save time name into txt
  ofstream ofs;
  ofs.open("time_name.txt");
  if (!ofs.is_open()) {
    cout << "Fail to open txt file" << endl;
  } else {
    for (int i = 0; i < N; i++) {
      ofs << time_name[i] << endl;
    }
  }
  ofs.close();

  // write analysis plots into oAnalyzr.root
  ofile->cd("Analysis_plot");

  h_K40_peak_uncali->Write();
  h_K40_peak_cali->Write();
  h_diff->Write();

	g_sigma_significant->SetName("g_sigma_significant");
	g_sigma_significant->Write();

  TGraph *gr = new TGraph(N, N_, diff_);
  gr->SetName("g_diffvsTime");
  gr->Write();

  TGraph *corr = new TGraph(N, N_, cfactor);
  corr->SetName("g_cfactor");
  corr->Write();

  TGraph *g_K40_peak_cali = new TGraph(N, N_, K40peak_cali);
  g_K40_peak_cali->SetName("g_K40_peak_cali");
  g_K40_peak_cali->Write();

  TGraph *g_K40_peak_uncali = new TGraph(N, N_, K40peak_uncali);
  g_K40_peak_uncali->SetName("g_K40_peak_uncali");
  g_K40_peak_uncali->Write();
}
