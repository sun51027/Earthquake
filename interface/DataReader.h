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

// class Earthquake;

class DataReader {
public:
  DataReader() {}
  DataReader(double lat, double lon, double depth, double ML, double nstn, double dmin, double gap, double trms,
             double ERH, double ERZ, double nph, const TString datetime)
    : lat_(lat), lon_(lon), depth_(depth), ML_(ML), nstn_(nstn), dmin_(dmin), gap_(gap), trms_(trms), ERH_(ERH),
      ERZ_(ERZ), nph_(nph), datetime_(datetime)
  {
  }
  DataReader(const DataReader &da)
     : lat_(da.lat_),
        lon_(da.lon_),
        depth_(da.depth_),
        ML_(da.ML_),
        nstn_(da.nstn_),
        dmin_(da.dmin_),
        gap_(da.gap_),
        trms_(da.trms_),
        ERH_(da.ERH_),
        ERZ_(da.ERZ_),
        nph_(da.nph_),
        datetime_(da.datetime_)
  {
  }
  ~DataReader() {}
  vector<DataReader> ReadRawData();
  void               Init(ifstream &eqDirInput);
  void               ReadEQdata(ifstream &eqDirInput, ifstream &timeInput, TFile *ofile);
  void               DrawPlots();

  bool operator<(const DataReader &da) const
  {
    if (ML_ > da.ML_)
      return true;
    else if (ML_ == da.ML_) {
      if (depth_ > da.depth_)
        return true;
      else
        return false;
    } else
      return false;
  }

private:
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

  vector<TString>    datetime;
  vector<string>     datetime_Rn;
  vector<DataReader> rawdata;

  double  ML[4000];
  double  depth[4000];
  TGraph *g_ML;
  TGraph *g_depth;
};
#endif
