#include <iostream>
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

#include "../include/EQ.h"

using namespace RooFit;
using namespace std;

TH1 *Earthquake::AddHistforTemplate(TDirectory *dir)
{
  int count = 0;

  TH1D *Template = new TH1D("Template", "", NBINS, -0.5, 4.5);

  TKey *keyAsObj, *keyAsObj2;
  TIter next(dir->GetListOfKeys());
  TH1  *obj;

  while ((keyAsObj = (TKey *)next())) {
    auto        key  = (TKey *)keyAsObj;
    TDirectory *dir2 = (TDirectory *)dir->Get(key->GetName());
    dir2->cd();
    TIter next2(dir2->GetListOfKeys());
    while ((keyAsObj2 = (TKey *)next2())) {
      count++;
      auto key2 = (TKey *)keyAsObj2;
      obj       = (TH1 *)dir2->Get(key2->GetName()); // copy every th1 histogram to
                                                     // obj
      if ((obj->Integral() != 0)) {
        // if (count > 2375) { // start from 21/10/01
        // if (count > 1799) { // start from 21/9/15
        Template->Add(obj);
      }
      delete obj;
    }
  }
  double factor_a = 1.02257;
  double factor_b = 0.0439089;
  for (int k = 0; k < NBINS; k++) {
    nMoveBin[k] = 0.0;
  }
  for (int k = 0; k < NBINS; k++) {


    nMoveBin[k] = ((Template->GetBinCenter(k + 1) ) * (factor_a-1)+(factor_b)) / energyBin ;
    cout << "BinCenter_uncali " << Template->GetBinCenter(k + 1) << " cali "
         << (Template->GetBinCenter(k + 1) + 0.0439089) * (factor_a) << " nMoveBin " << nMoveBin[k] << endl;
  }

  // calibrate hourly and show K40 peak
  TH1D *Template_cali = (TH1D *)(Template->Clone("Template_cali"));
  for (int j = 0; j < NBINS; j++) {
    // Template_cali->SetBinContent(j+1,(Template->GetBinCenter(k + 1)+0.0439089)*1.02257);
    Template_cali->SetBinContent(j + 1 , Template->GetBinContent(j + 1 -round(nMoveBin[j ]))) ;
    // Template_cali->SetBinContent(j + 1 + 1, Template->GetBinContent(j + 1 + 1) * (1 - nMoveBin[j + 1]) +
    //                                           Template->GetBinContent(j + 1) * (nMoveBin[j]));
    cout << j + 1 << "  template " << Template->GetBinContent(j + 1) << " move " << nMoveBin[j] << " "
         << "cali " << Template_cali->GetBinContent(j + 1) << endl;
  }

  // Template->SetXTitle("Energy (MeV)");
  // Template->SetYTitle("Entries");
  // cout << count << endl;
  // return Template;
  Template_cali->SetXTitle("Energy (MeV)");
  Template_cali->SetYTitle("Entries");
  cout << count << endl;
  return Template_cali;
}
