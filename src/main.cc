#include <iostream>
#include "../include/EQ.h"
#include "../include/DataReader.h"
#include "../include/GeoData.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TH1D.h"
//#define DEBUG
using namespace std;
void   main_doAnalysis();
void   main_eqDir();
void main_geodata();
void   main_pvalue_eqDir();
void   drawPvalue(TDirectory *dir1, TDirectory *dir2, TDatime timeoffset);
void   Help();
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
    } else if (arg == "-p" || arg == "--pvalue") {
      anaType = 3;
      iarg++;
    } else if (arg == "-geo" || arg == "--geodata") {
      anaType = 4;
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
      iarg++;
      outputFile = argv[iarg];
      iarg++;
    } else if (arg == "-h" || arg == "--help") {
      anaType = 0;
      break;
    }
  }
  switch (anaType) {
  case 0: Help(); break;
  case 1: main_doAnalysis(); break;
  case 2: main_eqDir(); break;
  case 3: main_pvalue_eqDir(); break;
  case 4: main_geodata(); break;
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
  ofile->cd();

  // inputfile
  TFile      *fin1 = new TFile(inputFile.c_str());
  TDirectory *dir  = (TDirectory *)fin1->Get("HistoCh0");
  dir->cd();

  TFile     *fin2     = new TFile(inputFile2.c_str());
  TH1D      *Template = (TH1D *)fin2->Get("Template");
  Earthquake eqAnalysis;
  eqAnalysis.DoAnalysis(Template, dir, ofile);
  ofile->Close();
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

  DataReader eqData;
  ifstream   timeInput;
  timeInput.open(inputFile2.c_str());
  eqData.ReadEQdata(eqDirInput, timeInput, ofile);
  ofile->Close();

  // draw plots
  eqData.DrawPlots();
}
void main_pvalue_eqDir()
{
	TDatime timeoffset;
//  ifstream datetime;
//  datetime.open("datetime.txt");
  int date = 20210915;
  int time = 80000;
  timeoffset.Set(date, time);
  TFile      *fin1 = new TFile("plots_root/oAnalyzer.root");
  TDirectory *dir1 = (TDirectory *)fin1->Get("Analysis_plot");
  TFile      *fin2 = new TFile("plots_root/EQdiroutput.root");
  TDirectory *dir2 = (TDirectory *)fin2->Get("EQ_directory");
  drawPvalue(dir1, dir2, timeoffset);
}
void main_geodata()
{
  cout<<"Geodata...."<<endl;

  GeoData geodata;
  ifstream   timeInput;
  timeInput.open(inputFile2.c_str());
  geodata.Combine(inputFile.c_str(),timeInput);
  
}
void Help()
{
  cout << endl;
  cout << "Anlysis type  -------------------" << endl;
  cout << "-an  || --analysis \t\t Do Radon analysis" << endl;
  cout << "-dir || --eqdir \t\t Treat EQ directory from CWB and draw plots" << endl;
  cout << "-geo || --geodata \t\t Treat waveform data from CWB and draw plots" << endl;
  cout << "-p   || --pvalue \t\t Draw p-value with eqdir and radon data"<<endl;
  cout << endl;
  cout << "File type  ----------------------" << endl;
  cout << "-i   || --inputFile \t\t 1st inputfile, radon data for -an, GDMScatalog for -dir" << endl;
  cout << "-i2  || --inputFile2 \t\t 2ns inputfile, template for -an, timelist for -dir " << endl;
  cout << "-o   || --outputFile \t\t root file output name " << endl;
}
