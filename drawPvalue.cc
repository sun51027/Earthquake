using namespace mgr;
void drawPvalue()
{

  int N = 1285;

  // difference vs time
  // create canvas
  TFile      *fin1 = new TFile("plots_root/oAnalyzer.root");
  TDirectory *dir1 = (TDirectory *)fin1->Get("Analysis_plot");
  dir1->cd();

  // eq dir
  TFile      *fin2 = new TFile("plots_root/EQdiroutput.root");
  TDirectory *dir2 = (TDirectory *)fin2->Get("EQ_directory");
  dir2->cd();

  TCanvas *c  = new TCanvas("c5", "", 1200, 1200);
  TPad    *pT = mgr::NewTopPad();
  TPad    *pB = mgr::NewBottomPad();

  c->cd();
  pT->Draw();
  pB->Draw();

  c->cd();
  pT->cd();
  // TGraph
  TGraph *g_pvalue = (TGraph *)dir1->Get("g_pvalue");
  TGraph *g_ML     = (TGraph *)dir2->Get("g_ML");
	
  g_pvalue->SetTitle("");
  g_pvalue->SetMaximum(1);
  g_pvalue->SetMinimum(1e-7);
  //  g_pvalue->SetMinimum(1e-13);
  g_pvalue->Draw("AL");
  g_pvalue->GetXaxis()->SetLimits(-30 * 60 * 60 * 2, (N + 80) * 60 * 60 * 2);
  g_pvalue->GetXaxis()->SetTitle("Time (mm/dd)");
  g_pvalue->GetYaxis()->SetTitle("p-value");

 // g_pvalue->GetXaxis()->SetLabelSize(0);
 // g_pvalue->GetXaxis()->SetTitleSize(0);
  g_pvalue->GetXaxis()->SetTimeDisplay(1);
//  g_pvalue->GetXaxis()->SetTimeFormat("%m/%d");
  g_pvalue->GetXaxis()->SetTimeFormat("%m/%d %F2021-09-15 00:00:00");
	g_pvalue->GetXaxis()->SetNdivisions(510);

  g_pvalue->GetYaxis()->SetTitleSize(0.05);
  g_pvalue->GetYaxis()->SetTitleOffset(0.7);
	//mgr::SetTopPlotAxis(g_pvalue);
  //  for (int i = 0; i < 5; i++) {
  //    double p = 0.5 * (1 - TMath::Erf((i + 1) / sqrt(2)));
  //    TLine *l = new TLine(-30 * 60 * 60 * 2, p, (N + 80) * 60 * 60 * 2, p);
  //    l->SetLineStyle(7);
  //    l->SetLineColor(kRed);
  //    l->SetLineWidth(1);
  //    l->Draw();
  //    TString s;
  //    s.Form("%i", i + 1);
  //    TLatex *tv1 = new TLatex((N + 15) * 60 * 60 * 2, p, s + " #sigma");
  //    tv1->SetTextAlign(11);
  //    tv1->SetTextColor(kRed);
  //    tv1->SetTextSize(0.04);
  //    tv1->SetTextFont(12);
  //    tv1->Draw();
  //  }

  c->cd();
  pB->cd();

//	TDatime timeoffset(2021,9,15,00,00,00);
//	gStyle->SetTimeOffset(timeoffset.Convert()+8*3600);
  g_ML->SetTitle("");
  g_ML->Draw("AP");
  g_ML->GetXaxis()->SetLimits(-30 * 60 * 60 * 2, (N + 80) * 60 * 60 * 2);
  //  //g_ML->GetXaxis()->SetLimits(-30 * 60 * 60 * 2, (datetime_Rn.size() + 80) * 60 * 60 * 2);
  //	g_ML->GetXaxis()->SetLimits(-10*60*60*2,(datetime_Rn.size()+10)*60*60*2);
  g_ML->GetXaxis()->SetTitle("Time (mm/dd)");
  g_ML->GetYaxis()->SetTitle("M_{L}");
  g_ML->SetMinimum(3.8);
  g_ML->SetMaximum(7);
  g_ML->SetMarkerStyle(20);
  g_ML->GetXaxis()->SetTimeDisplay(1);
  // g_ML->GetXaxis()->SetTimeOffset(0,"local");
 // g_ML->GetXaxis()->SetTimeFormat("%m/%d-%Hh");
  g_ML->GetXaxis()->SetTimeFormat("%m/%d-%Hh %F2021-09-15 00:00:00");
  // g_ML->GetXaxis()->SetLabelSize(0);
  // g_ML->GetXaxis()->SetTitleSize(0);

  g_ML->GetXaxis()->SetLabelSize(0.08);
  g_ML->GetXaxis()->SetTitleSize(0.15);
	g_ML->GetXaxis()->SetNdivisions(511);

  g_ML->GetYaxis()->SetLabelSize(0.12);
  g_ML->GetYaxis()->SetTitleSize(0.17);
  g_ML->GetYaxis()->SetTitleOffset(0.2);
//	mgr::SetBottomPlotAxis(g_ML);	

	pT->SetLogy();
	pT->Modified();
	pB->Modified();
  c->SetTicks(1, 1);
  c->Modified();
  c->SaveAs("p-value.pdf");
  delete c;
}
