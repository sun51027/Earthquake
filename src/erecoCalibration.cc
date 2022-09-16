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

  // for(int i = 0;i<factor_a.size();i++){
  //   cout<<"factor_a["<<i<<"] "<<factor_a[i]<<endl;
  // }

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

      if (obj->Integral() != 0) {

        for (int k = 0; k < NBINS; k++) {
          nMoveBin[k] = 0.0;
        }
        for (int k = 0; k < NBINS; k++) {

          // nMoveBin[k] = ( (factor_b[N])) / energyBin;
          // nMoveBin[k] = ((obj->GetBinCenter(k + 1)) * (factor_a[N] - 1)) / energyBin;
          nMoveBin[k] = ((obj->GetBinCenter(k + 1)) * (factor_a[N] - 1) + (factor_b[N])) / energyBin;
          // cout<<"center "<<obj->GetBinCenter(k+1)<<", factor_a["<<N<<"] "<<factor_a[N]<<", factor_b["<<N<<"]"<<
          //   factor_b[N]<< "  nMoveBin["<<k<<"] "<<nMoveBin[k]<<endl;
        }
        TH1D *obj_cali = (TH1D *)(obj->Clone("obj_cali"));
        for (int j = 0; j < NBINS; j++) {
          obj_cali->SetBinContent(j + 1, obj->GetBinContent(j + 1 - round(nMoveBin[j])));
          // if(j+1+1 > NBINS){
          //   cout<<"j "<<j<<" nbins "<<NBINS<<endl;
          //   break;
          // }else{
          // if(j==0){
          //   obj_cali->SetBinContent(j + 1, obj->GetBinContent(j + 1 ) * (1 - nMoveBin[j + 1]/obj->GetBinContent(j + 1
          //   )));
          // }else if (j+1==NBINS){
          //   obj_cali->SetBinContent(j + 1, obj->GetBinContent(j+1) + (nMoveBin[j]));
          // }else{
          // obj_cali->SetBinContent(j + 1, obj->GetBinContent(j + 1 ) + nMoveBin[j] - nMoveBin[j+1]);
          // obj_cali->SetBinContent(j + 1 + 1, obj->GetBinContent(j + 1 + 1) * (1 - nMoveBin[j + 1+1]) +
          //                                    obj->GetBinContent(j + 1) * (nMoveBin[j+1]));
          cout <<"  obj " << obj->GetBinContent(j+1) << " move " << nMoveBin[j] << " "
               << "cali " << obj_cali->GetBinContent(j+1) << endl;
        }
        obj_cali->Write(key2->GetName());

        delete obj;
        N++;
      }
    }
  }
  cout << "N = " << N << endl;
  cout << datetime_Rn.size() << endl;
}
