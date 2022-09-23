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

void Earthquake::ErecoCalibration(TDirectory *dir, TDirectory *odir, ifstream &inputPara)
{

  // get parameter for calibration
  string c1;
  double c2, c3;

  vector<double> factor_a;
  vector<double> factor_b;

  if (!inputPara.is_open()) {
    cout << "Failed to open file" << endl;
  } else {
    while (inputPara >> c1 >> c2 >> c3) {
      datetime_Rn.push_back(c1);
      factor_a.push_back(c2);
      factor_b.push_back(c3);
    }
  }

  TKey *keyAsObj, *keyAsObj2;
  TIter next(dir->GetListOfKeys());
  TH1  *obj;

  while ((keyAsObj = (TKey *)next())) {
    auto key = (TKey *)keyAsObj;
    // get second dir from input file
    TDirectory *dir2 = (TDirectory *)dir->Get(key->GetName());
    dir2->cd();
    TIter next2(dir2->GetListOfKeys());
    // create second dir for output file
    odir->mkdir(key->GetName());
    odir->cd(key->GetName());
    while ((keyAsObj2 = (TKey *)next2())) {
      auto key2 = (TKey *)keyAsObj2;
      obj       = (TH1 *)dir2->Get(key2->GetName());
      obj       = Earthquake::SetZeroBinContent(obj); // fill the empty bin with average of adjacent bins

      if (obj->Integral() != 0) {

        for (int k = 0; k <= NBINS; k++) {
          nMoveBin[k] = 0.0;
        }
        for (int k = 0; k < NBINS; k++) {

          nMoveBin[k] = ((obj->GetBinCenter(k + 1)) * (factor_a[N] - 1) + (factor_b[N])) / energyBin;
        }
        TH1D *obj_cali = new TH1D(key2->GetName(), key2->GetName(), NBINS, -0.5, 4.5);
        for (int j = 0; j < NBINS; j++) {

          if (obj->GetBinContent(j + 1) != 0) {
            double ratio = 0;
            ratio        = nMoveBin[j] - (int)nMoveBin[j];
            obj_cali->SetBinContent(j + 1 + (int)nMoveBin[j] + 1,
                                    obj->GetBinContent(j + 1) * ratio +
                                      obj_cali->GetBinContent(j + 1 + (int)nMoveBin[j] + 1));
            obj_cali->SetBinContent(j + 1 + (int)nMoveBin[j] + 1 - 1,
                                    obj->GetBinContent(j + 1) * (1 - ratio) +
                                      obj_cali->GetBinContent(j + 1 + (int)nMoveBin[j] + 1 - 1));
          }
        }
        obj_cali->Write(key2->GetName());

        delete obj;
        N++;
      }
    }
  }
}
