#include <iostream>
#include "interface/EQ.h"
#include "interface/DataReader.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TH1D.h"
//#define DEBUG
using namespace std;
int main()
{
  // outputfile
  TFile *ofile = new TFile("plots_root/oAnalyzer.root", "recreate");
  ofile->mkdir("cali_Hist");
  ofile->mkdir("K40_uncali_fit");
  ofile->mkdir("K40_cali_fit");
  ofile->mkdir("Analysis_plot");
  ofile->mkdir("EQ_directory");
  ofile->cd();

  // inputfile
  TFile      *fin1 = new TFile("plots_root/output.root");
  TDirectory *dir  = (TDirectory *)fin1->Get("HistoCh0");
  dir->cd();

  ifstream eqDirInput;
  eqDirInput.open("data/GDMScatalog20210915-1231.txt");

  // get template
  TFile *fin2     = new TFile("plots_root/template.root");
  TH1D   *Template = (TH1D*)fin2->Get("Template");

  Earthquake eqAnalysis;
	DataReader eqData;

  // do analysis
  eqAnalysis.DoAnalysis(Template, dir, ofile);

	// read Eq directory
  ifstream timeInput;
  timeInput.open("time_name.txt");
	eqData.ReadEQdata(eqDirInput,timeInput,ofile);
	ofile->Close();

	// draw plots
	eqAnalysis.DrawPlot();
	eqData.DrawPlots();

  return 0;
}
