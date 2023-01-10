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

#include "../include/RadonData.h"

using namespace RooFit;
using namespace std;

TH1 *RadonData::AddHistforTemplate(TDirectory *dir)
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
      if ((obj->Integral() != 0)) {
        // if (count > 2375) { // start from 21/10/01
        // if (count > 1799) { // start from 21/9/15
        Template->Add(obj);
        // }
      }
      delete obj;
    }
  }
  Template->SetXTitle("Energy (MeV)");
  Template->SetYTitle("Entries");


  cout << count << endl;
  return Template;
}
