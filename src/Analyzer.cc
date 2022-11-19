// STL
#include <iostream>
#include <fstream>
#include <cmath>

// my header
#include "../include/RadonData.h"
#include "../include/Constants.h"

// ROOT include
#include "TDirectory.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TKey.h"
#include "TMath.h"
#include "TDatime.h"
#include "RooMsgService.h"
#include "RooFit.h"

void RadonData::DoAnalysis(TH1 *Template, TDirectory *dir, TFile *ofile)
{

  h_K40_peak_cali   = new TH1D("h_K40_peak_cali", "", 100, 1.25, 1.44);
  h_K40_peak_uncali = new TH1D("h_K40_peak_uncali", "", 100, 1.25, 1.44);
  h_diff            = new TH1D("h_diff", "", 100, -20000, 20000);
  h_diff_K40        = new TH1D("h_diff", "", 100, -20000, 20000);
  h_cfactor         = new TH1D("h_cfactor", "", 100, 0.9, 1.1);
  h_cfactor_cali    = new TH1D("h_cfactor", "", 100, 0.9, 1.1);
  K40_template =
    Template->Integral(Template->GetXaxis()->FindBin(XMINFIT_K40), Template->GetXaxis()->FindBin(XMAXFIT_K40));

  TH1D *Template_cali = new TH1D("Template_cali", "", NBINS, -0.5, 4.5);
  
  RooMsgService::instance().setGlobalKillBelow(RooFit::ERROR);
  RooMsgService::instance().setSilentMode(true);
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
      obj       = (TH1 *)dir2->Get(key2->GetName());
      obj       = RadonData::SetZeroBinContent(obj); // fill the empty bin with average of adjacent bins
      if (h % 100 == 0) cout << "processing " << key->GetName() << endl;
      // if (h > 1799) { // start from 2021/9/15 h=1800, 2021/11/2 h =2376(?))
      // set hist name ex. 12/25;  datetime = 2021122522
      datetime[N].Form("%s%s", key->GetName(), key2->GetName());
      datetime[N].Remove(10, 4);

      Twopoint_uncali[N]   = 0.0;
      Twopoint_cali[N]     = 0.0;
      Radon2peak_cali[N]   = 0.0;
      Radon2peak_uncali[N] = 0.0;
      cfactor[N]           = 0.0;
      cfactor_cali[N]      = 0.0;
      K40peak_uncali[N]    = 0.0;
      K40peak_cali[N]      = 0.0;
      N_[N]                = 0.0;
      diff_[N]             = 0.0;
      diff_K40_[N]         = 0.0;

      if (obj->Integral() != 0) {

        /**********************************************************
            Calibration according to the 2.2 MeV peak
         **********************************************************/
        // get the calibration factor
        Twopoint_uncali[N] = PeakforCalibration(obj, ofile, datetime[N], 0);
        cfactor[N]         = PEAKFORCALI / Twopoint_uncali[N];
        h_cfactor->Fill(cfactor[N]);

        for (int k = 0; k < NBINS; k++) {

          nMoveBin[k] = (cfactor[N] - 1) * obj->GetBinCenter(k + 1) / energyBin;
        }

        // calibrate hourly according to 2.2MeV peak
        // TH1D *obj_cali = new TH1D(key2->GetName(), key2->GetName(), NBINS, -0.5, 4.5);
        TH1D* obj_cali = (TH1D*)(obj->Clone(key2->GetName()));
        
        // for (int j = 0; j < NBINS; j++) {
        //
        //   if (obj->GetBinContent(j + 1) != 0) {
        //     double ratio = 0;
        //     ratio        = nMoveBin[j] - (int)nMoveBin[j];
        //     obj_cali->SetBinContent(j + 1 + (int)nMoveBin[j] + 1,
        //                             obj->GetBinContent(j + 1) * ratio +
        //                               obj_cali->GetBinContent(j + 1 + (int)nMoveBin[j] + 1));
        //     obj_cali->SetBinContent(j + 1 + (int)nMoveBin[j] + 1 - 1,
        //                             obj->GetBinContent(j + 1) * (1 - ratio) +
        //                               obj_cali->GetBinContent(j + 1 + (int)nMoveBin[j] + 1 - 1));
        //
        //     // cout << j + 1 << "  nMoveBin " << nMoveBin[j] << "\t obj[" << j + 1 << "] " << obj->GetBinContent(j + 1)
        //     //      << " \t  obj_cali[" << j + 1 << "] " << obj_cali->GetBinContent(j + 1) << " \t  obj_cali["
        //     //      << j + 1 + (int)nMoveBin[j] << "] " << obj_cali->GetBinContent(j + 1 + (int)nMoveBin[j]) << "\t
        //     //      obj["
        //     //      << j + 1 << "] " << obj->GetBinContent(j + 1) << "*" << ratio << "\t + obj["
        //     //      << j + 1 + (int)nMoveBin[j] << "] " << obj->GetBinContent(j + 1 + (int)nMoveBin[j]) << endl;
        //   }
        // }
        Twopoint_cali[N] = PeakforCalibration(obj_cali, ofile, datetime[N], 1);
        cfactor_cali[N]  = PEAKFORCALI / Twopoint_cali[N];
        h_cfactor_cali->Fill(cfactor_cali[N]);

        K40peak_uncali[N]    = PeakforK40(obj, ofile, datetime[N], 0, N);
        K40peak_cali[N]      = PeakforK40(obj_cali, ofile, datetime[N], 1, N);
        // Radon2peak_uncali[N] = PeakforRadon2(obj, ofile, datetime[N], 0);
        Radon2peak_cali[N]   = PeakforRadon2(obj_cali, ofile, datetime[N], 1);

        h_K40_peak_cali->Fill(K40peak_cali[N]);
        h_K40_peak_uncali->Fill(K40peak_uncali[N]);

        /**********************************************************
            After the calibration, calculate the difference
            between template and each plots.
         **********************************************************/

        // Normalize template to yiels of every hour (use K40)

        double K40_obj_cali =
          obj_cali->Integral(obj_cali->GetXaxis()->FindBin(XMINFIT_K40), obj_cali->GetXaxis()->FindBin(XMAXFIT_K40));

        TH1D *scaledTemplate = (TH1D *)(Template->Clone("scaledTemplate"));
        scaledTemplate->Scale(K40_obj_cali / K40_template); // template scale to same as the hourly plot

        double nTemplateSig = scaledTemplate->Integral(scaledTemplate->GetXaxis()->FindBin(MINRADON),
                                                       scaledTemplate->GetXaxis()->FindBin(MAXRADON));

        double nDailySig =
          obj_cali->Integral(obj_cali->GetXaxis()->FindBin(MINRADON), obj_cali->GetXaxis()->FindBin(MAXRADON));
        double diff = nDailySig - nTemplateSig;

        N_[N]        = (double)(N + 1) * 60 * 60 * 2; // time series -- 2 hrs counting
        diff_[N]     = diff;
        diff_K40_[N] = K40_obj_cali;
        h_diff->Fill(diff);

        Template_cali->Add(obj_cali);

        ofile->cd("obj_cali");
        obj_cali->Write(key->GetName());
      

        delete obj;
        delete obj_cali;
        delete scaledTemplate;
        N++;
      } // obj analysis
      else{
        Twopoint_cali[N] = 0.0;
        cfactor_cali[N]  =0.0; 
        K40peak_uncali[N]    =0.0; 
        K40peak_cali[N]      =0.0; 
        Radon2peak_uncali[N] =0.0; 
        Radon2peak_cali[N]   =0.0; 
        N_[N]        = (double)(N + 1) * 60 * 60 * 2; // time series -- 2 hrs counting
        N++;
      }

      // } // select specific period
      h++;
    } // 2nd loop
  }   // 1st loop

  /**********************************************************
      Calculate the sigma between template and each plots.
   **********************************************************/

  fluct_peak  = RadonData::FittingGausPeak(h_diff);
  fluct_sigma = RadonData::FittingGausSigma(h_diff);

  cout << "Radon fluctuation peak " << fluct_peak << endl;
  cout << "Radon fluctuation sigma " << fluct_sigma << endl;

  for (int i = 0; i < N; i++) {
    if (diff_[i] < fluct_peak) {
      sigma_[i]   = 0;
      p_value_[i] = 1;
    } else {
      sigma_[i]   = (diff_[i] - fluct_peak) / fluct_sigma;
      p_value_[i] = 0.5 * (1 - TMath::Erf(sigma_[i] / sqrt(2)));
    }
  }
  g_sigma_significant = new TGraph(N, N_, sigma_);
  g_pvalue            = new TGraph(N, N_, p_value_);

  /*********************************************************
      Fill TGraph
   *********************************************************/
  double v[40000];
  for (int i = 0; i < N; i++) v[i] = diff_[i] / fluct_sigma;

  //
  double ratio[40000];
  for (int i = 0; i < N; i++) ratio[i] = diff_[i] / nsig_K40[i];

  g_diffvsTime      = new TGraph(N, N_, v);
  g_NofK40          = new TGraph(N, N_, diff_K40_);
  g_cfactor         = new TGraph(N, N_, cfactor);
  g_cfactor_cali    = new TGraph(N, N_, cfactor_cali);
  g_twopoint_uncali = new TGraph(N, N_, Twopoint_uncali);
  g_twopoint_cali   = new TGraph(N, N_, Twopoint_cali);
  g_K40_peak_cali   = new TGraph(N, N_, K40peak_cali);
  g_K40_peak_uncali = new TGraph(N, N_, K40peak_uncali);
  g_Radon2_uncali   = new TGraph(N, N_, Radon2peak_uncali);
  g_Radon2_cali     = new TGraph(N, N_, Radon2peak_cali);

  /**********************************************************
      Write object into output files
   **********************************************************/

  // save time name into datetime.txt
  SetRnDatetime(datetime);


  // write analysis plots into oAnalyzr.root

  ofile->cd("Analysis_plot");

  h_K40_peak_uncali->Write();
  h_K40_peak_cali->Write();
  h_diff->Write();
  h_cfactor->Write();

  Template_cali->SetName("Template_cali");
  Template_cali->Write();

  g_Radon2_uncali->SetName("g_Radon2_uncali");
  g_Radon2_uncali->Write();

  g_Radon2_cali->SetName("g_Radon2_cali");
  g_Radon2_cali->Write();

  g_twopoint_uncali->SetName("g_twopoint_uncali");
  g_twopoint_uncali->Write();

  g_twopoint_cali->SetName("g_twopoint_cali");
  g_twopoint_cali->Write();

  g_cfactor->SetName("g_cfactor");
  g_cfactor->Write();

  g_K40_peak_cali->SetName("g_K40_peak_cali");
  g_K40_peak_cali->Write();

  g_K40_peak_uncali->SetName("g_K40_peak_uncali");
  g_K40_peak_uncali->Write();

  g_diffvsTime->SetName("g_diffvsTime");
  g_diffvsTime->Write();

  g_NofK40->SetName("g_NofK40");
  g_NofK40->Write();

  g_sigma_significant->SetName("g_sigma_significant");
  g_sigma_significant->Write();

  g_pvalue->SetName("g_pvalue");
  g_pvalue->Write();
}
