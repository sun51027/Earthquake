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

#include "interface/EQ.h"

using namespace RooFit;
using namespace std;

const double Earthquake::minK40   = 1.3;
const double Earthquake::maxK40   = 1.5;
const double Earthquake::minRadon = 0.25;
const double Earthquake::maxRadon = 0.8;
TH1* Earthquake::addHist(TDirectory *dir)
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
      double obj_K40 = obj->Integral();

      if (count > 1800) { // start from 9/15
        Template->Add(obj);
      }
      delete obj;
    }
  }

  Template->SetXTitle("Energy (MeV)");
  Template->SetYTitle("Entries");
  // Template->SetStats(0);
  cout << count << endl;
  return Template;
}


int main()
{

  TFile *ofile = new TFile("template.root", "recreate");
  ofile->mkdir("stability_K40");
  ofile->mkdir("cali_Hist");

  TFile      *file = new TFile("output.root");
  TDirectory *dir  = (TDirectory *)file->Get("HistoCh0");
  dir->cd();
	// init
	Earthquake EQ;

  // make a template
  TH1 *Template;
 	Template	= EQ.addHist(dir);
  Template      = EQ.setZeroBinContent(Template);
	ofile->cd();
	Template->Write("Template");
  EQ.calibration(Template, ofile,"Template_cali");
	EQ.stability_K40(Template,ofile,"Template_stab");
}
