#ifndef READDATA_H
#define READDATA_H

#include "TString.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTimeStamp.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

class DataReader {
public:
  DataReader() {}
  DataReader(int date, int time, double lat, double lon, double depth, double ML, double nstn, double dmin, double gap,
             double trms, double ERH, double ERZ, double nph, const TString datetime)
    : date_(date), time_(time), lat_(lat), lon_(lon), depth_(depth), ML_(ML), nstn_(nstn), dmin_(dmin), gap_(gap),
      trms_(trms), ERH_(ERH), ERZ_(ERZ), nph_(nph), datetime_(datetime)
  {
  }
  DataReader(const DataReader &da)
    : date_(da.date_), time_(da.time_), lat_(da.lat_), lon_(da.lon_), depth_(da.depth_), ML_(da.ML_), nstn_(da.nstn_),
      dmin_(da.dmin_), gap_(da.gap_), trms_(da.trms_), ERH_(da.ERH_), ERZ_(da.ERZ_), nph_(da.nph_),
      datetime_(da.datetime_)
  {
  }
  ~DataReader() {}
  vector<DataReader> ReadRawData();
  void               Init(ifstream &eqDirInput);
  void               SetEQdata(ifstream &eqDirInput, TFile *ofile);
  void    OpenDateTimeDoc(ifstream &timeInput);
  void    EQdirDrawPlots();
  TString SetDatetime(TTimeStamp t);
  TDatime SetTimeOffset();

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

  vector<string> datetime_Rn;

private:
  int     date_;
  int     time_;
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
  vector<TString>    date;
  vector<TString>    time;
  vector<DataReader> rawdata;

  TDatime timeoffset;

  double  ML[4000];
  double  depth[4000];
  TGraph *g_ML;
  TGraph *g_depth;
};

inline void DataReader::OpenDateTimeDoc(ifstream &timeInput)
{
  string column;
  if (!timeInput.is_open()) {
    cout << "Failed to open file" << endl;
  } else {
    while (timeInput >> column) {
      datetime_Rn.push_back(column);
    }
  }
}
inline TString DataReader::SetDatetime(TTimeStamp t)
{
  TString twohrstamp;
  if ((t.GetTime() / 100000) != 0) {
    twohrstamp.Form("%i%i", t.GetDate(), t.GetTime());
  } else if ((t.GetTime() / 10000) == 0) {
    twohrstamp.Form("%i00", t.GetDate());
  } else {
    twohrstamp.Form("%i0%i", t.GetDate(), t.GetTime());
  }
  twohrstamp.Remove(10, 4);
  TString s(twohrstamp(9, 10));
  if (s == "1") {
    twohrstamp.Replace(9, 1, "0");
  } else if (s == "3") {
    twohrstamp.Replace(9, 1, "2");
  } else if (s == "5") {
    twohrstamp.Replace(9, 1, "4");
  } else if (s == "7") {
    twohrstamp.Replace(9, 1, "6");
  } else if (s == "9") {
    twohrstamp.Replace(9, 1, "8");
  }
  return twohrstamp;
}
inline TDatime DataReader::SetTimeOffset()
{
  TString date_Rn[datetime_Rn.size()];
  TString time_Rn[datetime_Rn.size()];
  for (int rn = 0; rn < datetime_Rn.size(); rn++) {
    date_Rn[rn] = datetime_Rn[rn];
    date_Rn[rn].Remove(8, 2);
    time_Rn[rn] = datetime_Rn[rn];
    time_Rn[rn].Remove(0, 8);
    time_Rn[rn].Insert(2, "0000");
  }
  TDatime timeoffset;
  timeoffset.Set(date_Rn[0].Atoi(), time_Rn[0].Atoi());
  timeoffset.Print();
  return timeoffset;
}
#endif
