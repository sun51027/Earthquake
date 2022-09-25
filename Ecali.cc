void Ecali()
{

  TFile      *fin1 = new TFile("root_output/Rn_analyzer_220109_220218.root");
  TDirectory *dir1 = (TDirectory *)fin1->Get("Analysis_plot");
  dir1->cd();

  TFile      *fin2 = new TFile("root_output/Rn_analyzer_220501_220820.root");
  TDirectory *dir2 = (TDirectory *)fin2->Get("Analysis_plot");
  dir2->cd();

  // Period 1
  TGraph *g_twopoint_uncali_1 = (TGraph *)dir1->Get("g_twopoint_cali");
  double  theor_mean_two      = 0;
  theor_mean_two              = g_twopoint_uncali_1->GetMean(2);
  cout<<"theor_mean_two "<<theor_mean_two<<endl;

  TGraph *g_K40_peak_uncali_1 = (TGraph *)dir1->Get("g_K40_peak_cali");
  double  theor_mean_K40      = 0;
  theor_mean_K40              = g_K40_peak_uncali_1->GetMean(2);
  cout<<"theor_mean_K40 "<<theor_mean_K40<<endl;

  // TGraph *g_Radon2_uncali_1 = (TGraph *)dir1->Get("g_Radon2_uncali");
  // double  theor_mean_Radon2 = 0;
  // theor_mean_Radon2         = g_Radon2_uncali_1->GetMean(2);

  // Period 2 -----------------------

  TGraph *g_twopoint_uncali_2 = (TGraph *)dir2->Get("g_twopoint_uncali");
  double  exp_mean_two        = 0;
  exp_mean_two                = g_twopoint_uncali_2->GetMean(2);

  TGraph *g_K40_peak_uncali_2 = (TGraph *)dir2->Get("g_K40_peak_uncali");
  double  exp_mean_K40        = 0;
  exp_mean_K40                = g_K40_peak_uncali_2->GetMean(2);

  // TGraph *g_Radon2_uncali_2 = (TGraph *)dir2->Get("g_Radon2_uncali");
  // double  exp_mean_Radon2   = 0;
  // exp_mean_Radon2           = g_Radon2_uncali_2->GetMean(2);

  // open datetime
  string   column;
  ifstream timeInput;
  timeInput.open("doc/datetime_220501-0820.txt");
  // vector<double> datetime_Rn;
  vector<string> datetime_Rn;
  if (!timeInput.is_open()) {
    cout << "Failed to open file" << endl;
  } else {
    while (timeInput >> column) {
      datetime_Rn.push_back(column);
    }
  }

  // output file
  TFile *fout = new TFile("root_output/EnergyCaliPara.root", "recreate");
  fout->mkdir("para");

  // analysis
  vector<double> theor_mean;
  vector<double> exp_value;
  // theor_mean.push_back(theor_mean_Radon2);
  theor_mean.push_back(theor_mean_K40);
  theor_mean.push_back(theor_mean_two);

  //fitting function
  TF1 *f1 = new TF1("f1", "[0]*x+[1]", 0, 3);

  if (datetime_Rn.size() != g_K40_peak_uncali_2->GetN()) {
    cout << "# of days not matched! " << endl;
    cout << "Rn days :" << datetime_Rn.size() << endl;
    cout << "K40 days :" << g_K40_peak_uncali_2->GetN()<< endl;
    return 0;
  } else {
    cout << "matched days :" << datetime_Rn.size() << endl;
  }

  ofstream ofs;
  ofs.open("doc/cali_parameter.txt");
  if (!ofs.is_open()) {
    cout << "Fail to open txt file" << endl;
  }
  vector<double> c1;
  vector<double> c2;
  vector<double> N;
  for (int i = 0; i < g_K40_peak_uncali_2->GetN(); i++) {
    // exp_value.push_back(g_Radon2_uncali_2->GetPointY(i));
    exp_value.push_back(g_K40_peak_uncali_2->GetPointY(i));
    exp_value.push_back(g_twopoint_uncali_2->GetPointY(i));

    TGraph *compare = new TGraph(2, exp_value.data(), theor_mean.data());
    // compare->SetName(datetime_Rn[i]);
    compare->Draw("AP");
    compare->SetMarkerStyle(20);
    compare->Fit("f1");
    compare->GetXaxis()->SetTitle("peak for 2hr (MeV) (Period 2)");
    compare->GetYaxis()->SetTitle("Mean of peak (MeV) (Period 1)");

    ofs << datetime_Rn[i] << " " << f1->GetParameter(0) << " " << f1->GetParameter(1) << endl;
   
    // c1.push_back(f1->GetParameter(0));
    c2.push_back(f1->GetParameter(1));

    fout->cd("para");
    compare->Write();
    exp_value.clear();
    N.push_back(i+1);

  }
  TGraphErrors *gr = new TGraphErrors(c2.size(),N.data(),c2.data());
    gr->Draw("AP");
    gr->SetMarkerStyle(20);
    fout->cd();
    gr->Write();
    // gr->GetYaxis()->SetTitle("c1 (slope)"); 
    cout<<gr->GetMean()<<endl;
  ofs.close();
}
