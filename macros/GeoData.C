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
  for (Long64_t ientry = 0; ientry < nentries; ientry++) {

    fChain->GetEntry(ientry);
    if (ientry % 10000000 == 0)
      std::cout << ientry << " " << std::setprecision(3) << float(ientry) / float(nentries) * 100 << "%" << std::endl;


    if (ientry > 0 && timestamp == timetmp) {
      if (abs(data) > abs(datatmp)) datatmp = data;
    } else {
      // include ientry = 0, that is in first entry we set timetmp = timestamp
  //    cout << "timestamp " << timestamp << " timetmp " << timetmp <<" timestamp_ns "<<timestamp_ns<<endl;
      timetmp = timestamp;
      data_collection.push_back(datatmp); // input data every second
			n++;
			ts_collection.push_back(n);
      //ts_collection.push_back(timestamp+timestamp_ns*1e-9);
      datatmp = 0;
    }

    // if (Cut(ientry) < 0) continue;
  }
  //   TFile* f = new TFile("test.root","RECREATE");
  TCanvas *c = new TCanvas("c","",800,600);
  TGraph *gdata = new TGraph(ts_collection.size(), ts_collection.data(), data_collection.data());
  gdata->Draw("AL");
  gdata->SetMarkerStyle(20);
  gdata->GetXaxis()->SetTimeDisplay(1);
  gdata->GetXaxis()->SetTimeFormat("%m/%d %H %F2022-03-23 00:00:00");
	gdata->GetXaxis()->SetLimits(-10000,n+10000);
  c->SaveAs("EQtimestamp(s)_maxsignal.png");
  cout << "nentries " << nentries << endl;
	cout <<"size "<<data_collection.size()<<endl;
	cout <<"size "<<ts_collection.size()<<endl;
	cout<<"n "<<n<<endl;
  //   gdata->Write();
  //   f->Write();
  //   f->Close();
  //  return;
}
