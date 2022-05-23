//
//	read earthquake data
//
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

#include "TString.h"
#include "TGraph.h"
#include "TCanvas.h"

#include "interface/DataReader.h"

// void DataReader::EarthquakeDirectory(){
int main()
{

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
  vector<double> nph;
  string         c1, c2, c13, c15, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c14;

  ifstream earthqakeDirInput;
  earthqakeDirInput.open("data/GDMScatalog20210915-1231.txt");

  if (!earthqakeDirInput.is_open()) {
    cout << "Failed to open file" << endl;
  } else {
    while (earthqakeDirInput >> c1 >> c2 >> c3 >> c4 >> c5 >> c6 >> c7 >> c8 >> c9 >> c10 >> c11 >> c12 >> c13 >> c14 >>
           c15) {
      date_raw.push_back(c1);
      time_raw.push_back(c2);
      lat_raw.push_back(c3);
      lon_raw.push_back(c4);
      depth_raw.push_back(c5);
      ML_raw.push_back(c6);
      nstn_raw.push_back(c7);
      dmin_raw.push_back(c8);
      gap_raw.push_back(c9);
      trms_raw.push_back(c10);
      ERH_raw.push_back(c11);
      ERZ_raw.push_back(c12);
      fixed_raw.push_back(c13);
      nph_raw.push_back(c14);
      quality_raw.push_back(c15);
    }
  }
  vector<TString> datetime;
  datetime.resize(100);
  for (int i = 1; i < lat_raw.size(); i++) {
    datetime[i - 1].Form("%s%s", date_raw[i].c_str(), time_raw[i].c_str());
    datetime[i - 1].Remove(4, 1);
    datetime[i - 1].Remove(6, 1);
    datetime[i - 1].Remove(10, 9);
    lat.push_back(stod(lat_raw[i]));
    lon.push_back(stod(lon_raw[i]));
    depth.push_back(stod(depth_raw[i]));
    ML.push_back(stod(ML_raw[i]));
    nstn.push_back(stod(nstn_raw[i]));
    dmin.push_back(stod(dmin_raw[i]));
    gap.push_back(stod(gap_raw[i]));
    trms.push_back(stod(trms_raw[i]));
    ERH.push_back(stod(ERH_raw[i]));
    ERZ.push_back(stod(ERZ_raw[i]));
    nph.push_back(stod(nph_raw[i]));
  }
  // odd time -> odd -1 time
  // ex 13 -> 12

  for (int i = 0; i < lat_raw.size(); i++) {
    TString s(datetime[i](9, 10));
    //				cout<<datetime[i]<<" ";
    if (s == "1") {
      datetime[i].Replace(9, 1, "0");
    } else if (s == "3") {
      datetime[i].Replace(9, 1, "2");
    } else if (s == "5") {
      datetime[i].Replace(9, 1, "4");
    } else if (s == "7") {
      datetime[i].Replace(9, 1, "6");
    } else if (s == "9") {
      datetime[i].Replace(9, 1, "8");
    }
    //	cout<<datetime[i]<<endl;
  }
  //------------------------------------------

  ifstream ifs2;
  ifs2.open("time_name.txt");
  vector<string> datetime_Rn;
  string         column;
  double         N[4000];

  while (ifs2 >> column) {
    datetime_Rn.push_back(column);
  }

  vector<DataReader> data(4000);

  // initialize
  //  for (int j = 0; j < datetime_Rn.size(); j++) {
  //					RichterML[j] = 0;
  //	}
  for (int rn = 0; rn < datetime_Rn.size(); rn++) {
    for (int i = 0; i < lat_raw.size() - 1; i++) {

      if (datetime[i] == datetime_Rn[rn]) {
        //        cout << datetime[i] << " " << datetime_Rn[j] << " " << ML[i] << endl;
        //
        //        RichterML[j] = ML[i];
        //				Depth[j] = depth[i];
        //        cout << "RichterML[" << j << "] " << RichterML[j] << endl;
        data[rn] =
          DataReader(lat[i], lon[i], depth[i], ML[i], nstn[i], dmin[i], gap[i], trms[i], ERH[i], ERZ[i], nph[i]);
      }
    }
    N[rn] = rn + 1;
  }
  vector<DataReader> Test;
  for (int rn = 0; rn < datetime_Rn.size(); rn++) {
    DataReader &cand = data[rn];
    Test.push_back(cand);

    //    cout << "RichterML[" << j << "] " << RichterML[j] << endl;
    //		cout<< "N["<<j<<"] "<<N[j]<<endl;
  }
  for (int rn = 0; rn < datetime_Rn.size(); rn++) {
    cout << "Test ML " << Test[rn].ML_ << endl;
  }

  return 0;
}
