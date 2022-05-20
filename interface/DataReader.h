#ifndef READDATA_H
#define READDATA_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

class DataReader {
public:
  DataReader() {}
  ~DataReader() {}
	void EarthquakeDirectory();

  double         RichterML[4000];
  double         Depth[4000];

private:
  vector<string> date_;
  vector<string> time_;
  vector<string> fixed;
  vector<string> quality;

  vector<string> lat_;
  vector<string> lon_;
  vector<string> depth_;
  vector<string> ML_;
  vector<string> nstn_;
  vector<string> dmin_;
  vector<string> gap_;
  vector<string> trms_;
  vector<string> ERH_;
  vector<string> ERZ_;
  vector<string> nph_;

  vector<double> lat;
  vector<double> lon;
  vector<double> depth;
  vector<double> ML;
  vector<double> nstn;
  vector<double> dmin;
  vector<double> gap;
  vector<double> trms;
  vector<double> ERH;
  vector<double> ERZ;
  vector<int>    nph;
};

#endif
