#include <iostream>
#include "interface/EQ.h"
#include "interface/DataReader.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TH1D.h"
//#define DEBUG
using namespace std;
void   main_doAnalysis();
void   main_eqDir();
string outputFile    = "";
string inputFile     = "";
string inputFile2    = "";
string inputTemplate = "";

int main(int argc, char **argv)
{
  string         arg_str;
  vector<string> arg_list;
  for (int i = 0; i < argc; ++i) {
    arg_str = argv[i];
    arg_list.push_back(arg_str);
    std::cout << arg_list[i] << std::endl;
  }
  int iarg    = 1;
  int anaType = 0;
  // only one arg run...
  while (iarg < argc) {
    string arg = arg_list[iarg];

    if (arg == "-an" || arg == "--analysis") {
      anaType = 1;
      iarg++;
    } else if (arg == "-dir" || arg == "--eqdir") {
      anaType = 2;
      iarg++;
    } else if (arg == "-i" || arg == "--inputFile") {
      iarg++;
      inputFile = argv[iarg];
      iarg++;
    } else if (arg == "-i2" || arg == "--inputFile2") {
      iarg++;
      inputFile2 = argv[iarg];
      iarg++;
    } else if (arg == "-o" || arg == "--outputFile") {
      iarg++; // first is -o ,then is name
      outputFile = argv[iarg];
      iarg++;
    }
  }
  switch (anaType) {
  case 1: main_doAnalysis(); break;
  case 2: main_eqDir(); break;
  }

  return 0;
}
void main_doAnalysis()
{
  cout << "Working for Analysis....." << endl;
  TFile *ofile = new TFile(outputFile.c_str(), "recreate");
  ofile->mkdir("cali_Hist");
  ofile->mkdir("K40_uncali_fit");
  ofile->mkdir("K40_cali_fit");
  ofile->mkdir("Analysis_plot");
  ofile->mkdir("EQ_directory");
  ofile->cd();
  // inputfile
  TFile      *fin1 = new TFile(inputFile.c_str());
  TDirectory *dir  = (TDirectory *)fin1->Get("HistoCh0");
  dir->cd();

  TFile     *fin2     = new TFile(inputTemplate.c_str());
  TH1D      *Template = (TH1D *)fin2->Get("Template");
  Earthquake eqAnalysis;
  eqAnalysis.DoAnalysis(Template, dir, ofile);
  eqAnalysis.DrawPlot();
}

void main_eqDir()
{
  cout << "Working for earthquake directory....." << endl;
  TFile *ofile = new TFile(outputFile.c_str(), "recreate");
  ofile->mkdir("EQ_directory");
  ofile->cd();
  ifstream eqDirInput;
  eqDirInput.open(inputFile.c_str());
  // eqDirInput.open("data/GDMScatalog20210915-1231.txt");
  DataReader eqData;
  // read Eq directory
  ifstream timeInput;
  timeInput.open(inputFile2.c_str());
  eqData.ReadEQdata(eqDirInput, timeInput, ofile);
  ofile->Close();
  // draw plots
  eqData.DrawPlots();
}

