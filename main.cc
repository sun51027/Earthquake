#include <iostream>
#include "interface/EQ.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TH1D.h"
//#define DEBUG
using namespace std;
int main()
{
  // outputfile
  TFile *ofile = new TFile("oAnalyzer.root", "recreate");
  ofile->mkdir("cali_Hist");
  ofile->mkdir("K40_uncali_fit");
  ofile->mkdir("K40_cali_fit");
  ofile->mkdir("Analysis_plot");
  ofile->cd();
  // inputfile
  TFile      *fin1 = new TFile("plots_root/output.root");
  TDirectory *dir  = (TDirectory *)fin1->Get("HistoCh0");
  dir->cd();

  // get template
  TFile *fin2     = new TFile("plots_root/template.root");
  TH1D   *Template = (TH1D*)fin2->Get("Template");

  Earthquake EQ;

  // do analysis
  EQ.DoAnalysis(Template, dir, ofile);

  return 0;
}
