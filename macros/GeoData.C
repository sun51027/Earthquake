void GeoData(string infileName)
{

  TFile *infile = new TFile(infileName.c_str(), "READ");
  TTree *fChain = (TTree *)infile->Get("GeoData");
  // Declaration of leaf types
  Float_t data;
  Int_t   timestamp;
  Int_t   timestamp_ns;
  // List of branches
  TBranch *b_data;         //!
  TBranch *b_timestamp;    //!
  TBranch *b_timestamp_ns; //!
  fChain->SetBranchAddress("data", &data, &b_data);
  fChain->SetBranchAddress("timestamp", &timestamp, &b_timestamp);
  fChain->SetBranchAddress("timestamp_ns", &timestamp_ns, &b_timestamp_ns);

  std::vector<float> data_collection;
  std::vector<float> ts_collection;
  float              datatmp = 0;
  Long64_t           timetmp = 0;
  int                n       = 0;

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  double   data_c[10000];
  double   ts_c[10000];
  cout << "nentries " << nentries << endl;
  for (Long64_t ientry = 0; ientry < 10000; ientry++) {

    fChain->GetEntry(ientry);
    data_c[ientry] = data;
    ts_c[ientry]   = (timestamp + timestamp_ns * 1e-9);
    if (ientry % 10000000 == 0)
      std::cout << ientry << " " << std::setprecision(3) << float(ientry) / float(nentries) * 100 << "%" << std::endl;
    //    cout << "timestamp " << timestamp << " timetmp " << timetmp <<" timestamp_ns "<<timestamp_ns<< " data
    //    "<<data<<endl;
    if (ientry > 0 && timestamp == timetmp) {
      n++;
      data_collection.push_back(data); // input data every second
    } else {
      // include ientry = 0, that is in first entry we set timetmp = timestamp
      timetmp = timestamp;
      ts_collection.push_back(timestamp);
      //    cout << "n " << n << endl;
    }

    // if (Cut(ientry) < 0) continue;
  }
  //    for(int i =0;i< data_collection.size();i++){
  //          cout<<" data_collection "<< data_collection[i]<<endl;
  //  }
  for (int i = 0; i < 10000; i++) {
    cout << (Long64_t)ts_c[i] << endl;
  }
  //  cout<<"data_collection size "<<data_collection.size()<<endl;
  //   TFile* f = new TFile("test.root","RECREATE");
  //     TGraph* gdata = new TGraph(data_collection.size(),&ts_collection,&data_collection);
  TGraph *gdata = new TGraph(10000, ts_c, data_c);
  gdata->Draw("AP");
  gdata->SetMarkerStyle(20);
  //   gdata->Write();
  //   f->Write();
  //   f->Close();
  return;
}
