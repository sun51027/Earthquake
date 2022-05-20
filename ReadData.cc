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

#include <TString.h>

// void Earthquake::ReadData()
int main()
{

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

  string c1, c2, c13, c15, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c14;

  ifstream ifs;
  ifs.open("data/GDMScatalog20210915-1231.txt");

  if (!ifs.is_open()) {
    cout << "Failed to open file" << endl;
  } else {
    while (ifs >> c1 >> c2 >> c3 >> c4 >> c5 >> c6 >> c7 >> c8 >> c9 >> c10 >> c11 >> c12 >> c13 >> c14 >> c15) {
      date_.push_back(c1);
      time_.push_back(c2);
      lat_.push_back(c3);
      lon_.push_back(c4);
      depth_.push_back(c5);
      ML_.push_back(c6);
      nstn_.push_back(c7);
      dmin_.push_back(c8);
      gap_.push_back(c9);
      trms_.push_back(c10);
      ERH_.push_back(c11);
      ERZ_.push_back(c12);
      fixed.push_back(c13);
      nph_.push_back(c14);
      quality.push_back(c15);
    }
  }
  vector<TString> datetime;
  datetime.resize(100);
  for (int i = 1; i < lat_.size(); i++) {
    datetime[i - 1].Form("%s%s", date_[i].c_str(), time_[i].c_str());
    datetime[i - 1].Remove(4, 1);
    datetime[i - 1].Remove(6, 1);
    datetime[i - 1].Remove(10, 9);
    lat.push_back(stod(lat_[i]));
    lon.push_back(stod(lon_[i]));
    depth.push_back(stod(depth_[i]));
    ML.push_back(stod(ML_[i]));
    nstn.push_back(stod(nstn_[i]));
    dmin.push_back(stod(dmin_[i]));
    gap.push_back(stod(gap_[i]));
    trms.push_back(stod(trms_[i]));
    ERH.push_back(stod(ERH_[i]));
    ERZ.push_back(stod(ERZ_[i]));
    nph.push_back(stod(nph_[i]));
  }
  // odd time -> odd -1 time
  // ex 13 -> 12

  for (int i = 0; i < lat_.size(); i++) {
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
  double         RichterML[4000];
  string         column;
  int            n = 0;

  while (ifs2 >> column) {
    datetime_Rn.push_back(column);
  }

  for (int j = 0; j < datetime_Rn.size(); j++) {
    for (int i = 0; i < lat_.size() - 1; i++) {

      if (datetime[i] == datetime_Rn[j]) {
        cout << datetime[i] << " " << datetime_Rn[j] << " " << ML[i] << endl;

        RichterML[j] = ML[i];
        cout << "RichterML[" << j << "] " << RichterML[j] << endl;
        n++;
      } else {
        RichterML[j] = 0;
      }
    }
  }

  for (int j = 0; j < datetime_Rn.size(); j++) {
    cout << "RichterML[" << j << "] " << RichterML[j] << endl;
  }
  cout << "n " << n << endl;

  //  TGraph *g_RichterML = new TGraph(, , );

  return 0;
}
