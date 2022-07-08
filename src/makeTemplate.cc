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

  TH1D *Template = new TH1D("Template", "", 1024, -0.5, 4.5);

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

      if (count > 1799) { // start from 9/15
        Template->Add(obj);
      }
      delete obj;
    }
  }

  Template->SetXTitle("Energy (MeV)");
  Template->SetYTitle("Entries");
  cout << count << endl;
  return Template;
}

// int main()
// // void makeTemplate()
// {
//
//   TFile *ofile = new TFile("../root_output/template_Apr-Sep.root", "recreate");
//   ofile->mkdir("K40_peak");
//   ofile->mkdir("cali_peak");
//
//   TFile      *file = new TFile("../root_output/output.root");
//   TDirectory *dir  = (TDirectory *)file->Get("HistoCh0");
//   dir->cd();
//   // init
//   Earthquake EQ;
//
//   // make a template
//   TH1 *Template;
//   Template = EQ.AddHist(dir);
//   Template = EQ.SetZeroBinContent(Template);
//   ofile->cd();
//   Template->Write("Template");
//   double caliPeak = EQ.PeakforCalibration(Template, ofile, "cali_peak");
//   double K40Peak  = EQ.PeakforK40(Template, ofile, "K40_peak", 0);
// 	cout<<"\n\n\n";
//   cout << "cali_peak " << caliPeak<< "  K40_peak " << K40Peak << endl;
// 	cout<<"\n\n\n";
//
//   return 0;
// }
