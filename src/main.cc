#include <iostream>
#include "../include/RadonData.h"
#include "../include/DataReader.h"
#include "../include/GeoData.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TH1D.h"
//#define DEBUG
using namespace std;
void   main_doAnalysis();
void   main_eqDir();
void   main_geodata();
void   main_pvalue_eqDir();
void   main_pvalue_geodata();
void   main_makeTemplate();
void   main_calibration();
void   drawPvalue_eqdir(TDirectory *dir1, TDirectory *dir2, TDatime timeoffset);
void   drawPvalue_geo(TDirectory *dir1, TDirectory *dir2, TDirectory *dir3, TDirectory *dir4, TDatime timeoffset);
void   Help();
string outputFile = "";
string inputFile  = "";
string inputFile2 = "";

int main(int argc, char **argv)
{
  string         arg_str;
  vector<string> arg_list;
  for (int i = 0; i < argc; ++i) {
    arg_str = argv[i];
    arg_list.push_back(arg_str);
    //    std::cout << arg_list[i] << std::endl;
  }
  int iarg    = 1;
  int anaType = 0;
  // only one arg run...
  while (iarg < argc) {
    string arg = arg_list[iarg];

    if (arg == "-t" || arg == "--template") {
      anaType = 0;
      iarg++;
    } else if (arg == "-an" || arg == "--analysis") {
      anaType = 1;
      iarg++;
    } else if (arg == "-dir" || arg == "--eqdir") {
      anaType = 2;
      iarg++;
    } else if (arg == "-pd" || arg == "--pvalueEQdir") {
      anaType = 3;
      iarg++;
    } else if (arg == "-pg" || arg == "--pvalueGeodata") {
      anaType = 4;
      iarg++;
    } else if (arg == "-geo" || arg == "--geodata") {
      anaType = 5;
      iarg++;
    } else if (arg == "-cali" || arg == "--calibration") {
      anaType = 6;
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
      anaType = -1;
      break;
    }
  }
  switch (anaType) {
  case -1: Help(); break;
  case 0: main_makeTemplate(); break;
  case 1: main_doAnalysis(); break;
  case 2: main_eqDir(); break;
  case 3: main_pvalue_eqDir(); break;
  case 4: main_pvalue_geodata(); break;
  case 5: main_geodata(); break;
  case 6: main_calibration(); break;
  }

  return 0;
}
void main_calibration()
{

  TFile      *fin1  = new TFile(inputFile.c_str());
  TDirectory *indir = (TDirectory *)fin1->Get("HistoCh0");
  indir->cd();

  TFile *ofile = new TFile(outputFile.c_str(), "recreate");
  ofile->mkdir("HistoCh0");
  TDirectory *odir = (TDirectory *)ofile->Get("HistoCh0");

  ifstream paraInput;
  paraInput.open(inputFile2.c_str());

  RadonData Rndata;
  Rndata.ErecoCalibration(indir, odir, paraInput);
}
void main_makeTemplate()
{
  cout << "Making template for Rn analysis...." << endl;
  TFile *ofile = new TFile(outputFile.c_str(), "recreate");
  ofile->mkdir("K40_uncali_fit");
  ofile->mkdir("cali_uncali_fit");

  TFile      *fin1 = new TFile(inputFile.c_str());
  TDirectory *dir  = (TDirectory *)fin1->Get("HistoCh0");
  dir->cd();
  // init
  RadonData Rndata;

  // make a template
  TH1 *Template;
  Template = Rndata.AddHistforTemplate(dir);
  Template = Rndata.SetZeroBinContent(Template);

  ofile->cd();
  Template->Write("Template");
  double caliPeak = 0;
  caliPeak        = Rndata.PeakforCalibration(Template, ofile, "cali_peak", 0);
  double K40Peak  = 0;
  K40Peak         = Rndata.PeakforK40(Template, ofile, "K40_peak", 0,0);
  cout << "\n\n\n";
  cout << "cali_peak " << caliPeak << "  K40_peak " << K40Peak << endl;
  cout << "\n\n\n";
        // double nTemplateSig = Template->Integral(Template->GetXaxis()->FindBin(MINRADON),
        //                                          Template->GetXaxis()->FindBin(MAXRADON));
        //
        // cout<<"nRadonSig_template " << nTemplateSig<<endl;

  ofile->Close();
}
void main_doAnalysis()
{
  cout << "Processing Radon Analysis....." << endl;
  TFile *ofile = new TFile(outputFile.c_str(), "recreate");
  ofile->mkdir("obj_cali");
  ofile->mkdir("Radon2_uncali_fit");
  ofile->mkdir("Radon2_cali_fit");
  ofile->mkdir("cali_uncali_fit");
  ofile->mkdir("cali_cali_fit");
  ofile->mkdir("K40_uncali_fit");
  ofile->mkdir("K40_cali_fit");
  ofile->mkdir("Analysis_plot");
  ofile->cd();

  // inputfile
  TFile      *fin1 = new TFile(inputFile.c_str());
  TDirectory *dir  = (TDirectory *)fin1->Get("HistoCh0");
  dir->cd();

  TFile    *fin2     = new TFile(inputFile2.c_str());
  TH1D     *Template = (TH1D *)fin2->Get("Template");
  RadonData RnAnalysis;
  RnAnalysis.DoAnalysis(Template, dir, ofile);
  ofile->Close();
  RnAnalysis.DrawPlot();
}

void main_eqDir()
{
  cout << "Processing earthquake directory....." << endl;

  TFile *ofile = new TFile(outputFile.c_str(), "recreate");
  ofile->mkdir("EQ_directory");
  ofile->cd();

  // read eqdir.txt
  ifstream eqDirInput;
  eqDirInput.open(inputFile.c_str());

  // read datetime(Rn).txt
  DataReader eqData;
  ifstream   timeInput;
  timeInput.open(inputFile2.c_str());
  eqData.OpenDateTimeDoc(timeInput);

  eqData.SetEQdata(eqDirInput, ofile);
  ofile->Close();

  // draw plots
  eqData.EQdirDrawPlots();
}
void main_pvalue_eqDir()
{
  TFile      *fin1 = new TFile(inputFile.c_str());
  TDirectory *dir1 = (TDirectory *)fin1->Get("Analysis_plot");
  TFile      *fin2 = new TFile(inputFile2.c_str());
  TDirectory *dir2 = (TDirectory *)fin2->Get("EQ_directory");

  TDatime timeoffset;
  int     date = 20211101;
  int     time = 80000;
  timeoffset.Set(date, time);
  drawPvalue_eqdir(dir1, dir2, timeoffset);
}
void main_geodata()
{
  cout << "Processing Geodata...." << endl;

  GeoData geo;

  // read datetime(Rn).txt
  ifstream timeInput;
  timeInput.open(inputFile2.c_str());
  geo.OpenDateTimeDoc(timeInput);

  // input waveform data built inside the function
  geo.GeoDataAnalyzer(inputFile.c_str()); //, timeInput);
}
void main_pvalue_geodata()
{
  TFile      *fin1 = new TFile(inputFile.c_str());
  TDirectory *dir1 = (TDirectory *)fin1->Get("Analysis_plot");

  TFile      *fin2 = new TFile(inputFile2.c_str());
  TDirectory *dir2 = (TDirectory *)fin2->Get("00_EHE");
  TDirectory *dir3 = (TDirectory *)fin2->Get("00_EHN");
  TDirectory *dir4 = (TDirectory *)fin2->Get("00_EHZ");
  TDatime     timeoffset;
  int         date = 20211102;
  int         time = 80000;
  timeoffset.Set(date, time);
  drawPvalue_geo(dir1, dir2, dir3, dir4, timeoffset);
}
void Help()
{
  cout << endl;
  cout << "Anlysis type  -------------------" << endl;
  cout << "-t   || --template \t\t make template" << endl;
  cout << "-an  || --analysis \t\t Do Radon analysis" << endl;
  cout << "-dir || --eqdir \t\t Treat EQ directory from CWB and draw plots" << endl;
  cout << "-geo || --geodata \t\t Treat waveform data from CWB and draw plots" << endl;
  cout << "-pd   || --pvalue \t\t Draw p-value with eqdir and radon data" << endl;
  cout << "-pg   || --pvalue \t\t Draw p-value with geodata and radon data" << endl;
  cout << endl;
  cout << "File type  ----------------------" << endl;
  cout << "-i   || --inputFile \t\t 1st inputfile, radon data for -an, GDMScatalog for -dir, analyzer for -p" << endl;
  cout << "-i2  || --inputFile2 \t\t 2ns inputfile, template for -an, timelist for -dir, EQdir for -p " << endl;
  cout << "-o   || --outputFile \t\t root file output name " << endl;
  cout << endl;
  cout << "other note ----------------------" << endl;
  cout << "To draw pvalue, manual is necessary" << endl;
  cout << "|     |inputfile1         | inputfile2        | outputfile      |" << endl;
  cout << "|-t   |RadonData.root     |                   | Rn_template.root|" << endl;
  cout << "|-an  |RadonData.root     | Rn_template.root  | Rn_analysis.root|" << endl;
  cout << "|-dir |doc/GDMScatalog.txt| datetime.txt      | EQdir.root      |" << endl;
  cout << "|-geo |(name)HWA_00_*_    | datetime.txt      | GeoData.root    |" << endl;
  cout << "|-pd  |Rn_analysis.root   | EQdir.root        |                 |" << endl;
  cout << "|-pg  |Rn_analysis.root   | GeoData.root      |                 |" << endl;
}
