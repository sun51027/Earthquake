// STL
using namespace std;
#include <iostream>
#include <fstream>
#include <cmath>
// my header
#include "interface/EQ.h"
#include "interface/Constants.h"

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
#include "TMath.h"
//#include "rootlogon.h"
// using namespace mgr;
//#define DEBUG

// const double Earthquake::MINK40   = 1.3;//K40 +- sigma
// const double Earthquake::MAXK40   = 1.5;
// const double Earthquake::MINRADON = 0.25;
// const double Earthquake::MAXRADON = 0.8;

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

  h_K40_peak_cali   = new TH1D("h_K40_peak_cali", "", 100, 1.37, 1.44); // before Sep 1.37-1.48
  h_K40_peak_uncali = new TH1D("h_K40_peak_uncali", "", 100, 1.37, 1.44);
  h_diff            = new TH1D("h_diff", "", 100, -20000, 20000);
  h_cfactor         = new TH1D("h_cfactor", "", 100, 0.99, 1.01); // befor Sep 0.98-1.01
  h_cfactor_cali    = new TH1D("h_cfactor", "", 100, 0.99, 1.01); // befor Sep 0.98-1.01
  K40_template      = Template->Integral(Template->GetXaxis()->FindBin(MINK40), Template->GetXaxis()->FindBin(MAXK40));

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
        cfactor[N]     = PEAKFORCALI / peakforCali[N]; // After Sep
        h_cfactor->Fill(cfactor[N]);

        for (int k = 0; k < 1024; k++) {
          nMoveBin_K40[k] = (cfactor[N]-1) * obj->GetBinCenter(k + 1) / energyBin;
        }

        // calibrate hourly and show K40 peak
        TH1D *obj_cali = (TH1D *)(obj->Clone("obj_cali"));
        for (int j = 0; j < 1024; j++) {
            obj_cali->SetBinContent(j + 1+1,
                               obj->GetBinContent(j + 1+1) * (1-nMoveBin_K40[j+1]) + obj->GetBinContent(j+1) *(nMoveBin_K40[j]));

//          if (nMoveBin_K40[j] != 0) {
            cout << "obj " << obj->GetBinContent(j + 1) 
								 << "\t" << nMoveBin_K40[j] //<< "\t obj_cali "
								 <<"\t\t cfactor "<<cfactor[N]
                 <<"\t\tcali_obj "<< obj_cali->GetBinContent(j + 1) << endl;
  //        }
        }

        cfactor_cali[N] = PEAKFORCALI / PeakforCalibration(obj_cali, ofile, time_name[N]);
        h_cfactor_cali->Fill(cfactor_cali[N]);
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
          obj_cali->Integral(obj_cali->GetXaxis()->FindBin(MINK40), obj_cali->GetXaxis()->FindBin(MAXK40));

        TH1D *scaledTemplate = (TH1D *)(Template->Clone("scaledTemplate"));
        scaledTemplate->Scale(K40_obj_cali / K40_template); // template scale to same as the hourly plot

        double nTemplateSig = scaledTemplate->Integral(scaledTemplate->GetXaxis()->FindBin(MINRADON),
                                                       scaledTemplate->GetXaxis()->FindBin(MAXRADON));
        double nDailySig =
          obj_cali->Integral(obj_cali->GetXaxis()->FindBin(MINRADON), obj_cali->GetXaxis()->FindBin(MAXRADON));
        double diff = nDailySig - nTemplateSig;
        // N_[N]       = (double)(N + 1); // number of 2hour
        N_[N]    = (double)(N + 1) * 60 * 60 * 2; // number of 2hour
        diff_[N] = diff;
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
    if (diff_[i] < fluct_peak) {
      sigma_[i]   = 0;
      p_value_[i] = 1;
    } else {
      sigma_[i]   = (diff_[i] - fluct_peak) / fluct_sigma;
      p_value_[i] = 0.5 * (1 - TMath::Erf(sigma_[i] / sqrt(2)));
    }
    //    cout << "diff_[" << i << "] " << diff_[i] << "\t - peak " << fluct_peak << "\t / sigma " << fluct_sigma
    //         << "  = sigma[" << i << "] = " << sigma_[i] << " p-value " << p_value_[i] << endl;
  }
  g_sigma_significant = new TGraph(N, N_, sigma_);
  g_pvalue            = new TGraph(N, N_, p_value_);

  /*********************************************************
      Fill TGraph
   *********************************************************/
  double v[4000];
  for (int i = 0; i < N; i++) v[i] = diff_[i] / fluct_sigma;
  cout << "fluct_peak " << fluct_peak << endl;
  cout << "fluct_sigma " << fluct_sigma << endl;

  g_diffvsTime      = new TGraph(N, N_, v);
  g_cfactor         = new TGraph(N, N_, cfactor);
  g_cfactor_cali    = new TGraph(N, N_, cfactor_cali);
  g_K40_peak_cali   = new TGraph(N, N_, K40peak_cali);
  g_K40_peak_uncali = new TGraph(N, N_, K40peak_uncali);

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
  h_cfactor->Write();

  g_sigma_significant->SetName("g_sigma_significant");
  g_sigma_significant->Write();

  g_pvalue->SetName("g_pvalue");
  g_pvalue->Write();

  g_diffvsTime->SetName("g_diffvsTime");
  g_diffvsTime->Write();

  g_cfactor->SetName("g_cfactor");
  g_cfactor->Write();

  g_K40_peak_cali->SetName("g_K40_peak_cali");
  g_K40_peak_cali->Write();

  g_K40_peak_uncali->SetName("g_K40_peak_uncali");
  g_K40_peak_uncali->Write();
}
