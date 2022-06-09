#ifndef READDATA_H
#define READDATA_H

#include "TString.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TFile.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

//#include "EQ.h"

using namespace std;

class Earthquake;

class DataReader
{
public:
  DataReader() {}
  DataReader(double lat, double lon, double depth, double ML, double nstn, double dmin, double gap, double trms,
             double ERH, double ERZ, double nph, const TString datetime)
    : lat_(lat), lon_(lon), depth_(depth), ML_(ML), nstn_(nstn), dmin_(dmin), gap_(gap), trms_(trms), ERH_(ERH),
      ERZ_(ERZ), nph_(nph), datetime_(datetime)
  {
  }

  ~DataReader() {}
  vector<DataReader> ReadRawData();
  void               EarthquakeDirectory();
  TString            LoadDateTime();
  void               Init(ifstream &eqDirInput);
  void               ReadEQdata(ifstream &eqDirInput, ifstream &timeInput, TFile* ofile);

  // private:
  double  lat_;
  double  lon_;
  double  depth_;
  double  ML_;
  double  nstn_;
  double  dmin_;
  double  gap_;
  double  trms_;
  double  ERH_;
  double  ERZ_;
  double  nph_;
  TString datetime_;

  vector<string> date_raw;
  vector<string> time_raw;
  vector<string> fixed_raw;
  vector<string> quality_raw;

  vector<string> lat_raw;
  vector<string> lon_raw;
  vector<string> depth_raw;
  vector<string> ML_raw;
  vector<string> nstn_raw;
  vector<string> dmin_raw;
  vector<string> gap_raw;
  vector<string> trms_raw;
  vector<string> ERH_raw;
  vector<string> ERZ_raw;
  vector<string> nph_raw;

  vector<string> datetime_Rn;

private:
  vector<DataReader> rawdata;
  vector<TString>    datetime;
  //  vector<double> lat;
  //  vector<double> lon;
  //  vector<double> depth;
  //  vector<double> ML;
  //  vector<double> nstn;
  //  vector<double> dmin;
  //  vector<double> gap;
  //  vector<double> trms;
  //  vector<double> ERH;
  //  vector<double> ERZ;
  //  vector<double> nph;
};
#endif
