TH1* addHist( TDirectory *dir){
	int count = 0;
		
	TH1D *Template = new TH1D ("Template","",1024,-0.5,4.5);

	TKey *keyAsObj, *keyAsObj2;
	TIter next(dir->GetListOfKeys());
	TH1* obj;

	while ((keyAsObj = (TKey*) next())){

		auto key = (TKey*) keyAsObj;
		TDirectory* dir2 = (TDirectory*)dir->Get(key->GetName());
		dir2->cd();
		TIter next2(dir2->GetListOfKeys());
		while ((keyAsObj2 = (TKey*)next2())){

			count++;	
			auto key2 = (TKey*) keyAsObj2;
			obj = (TH1*)dir2->Get(key2->GetName()); // copy every th1 histogram to obj
			double obj_K40 = obj->Integral();
			Template->Add(obj);	

			delete obj;
		}
	}

	Template->SetXTitle("Energy (MeV)");
	Template->SetYTitle("Entries");
	//Template->SetStats(0);
	cout<<count<<endl;	
	return Template;
			

}
void writeFile(TH1 *obj,TFile* ofile){

	ofile->cd("AdjustedHist");
	obj->Write();
}
TH1* setZeroBinContent(TH1 *hist){


	for(int i = 0;i<hist->GetNbinsX();i++){
	
		//cout<<i<<" GetBinContent "<<hist->GetBinContent(i);
		if(hist->GetBinContent(i) == 0 && hist->GetBinContent(i+1) == 0 && hist->GetBinContent(i+2) == 0){
			//cout<<"\t\t ,  "<<hist->GetBinContent(i)<<endl;
		    continue;
		}
		else if( hist->GetBinContent(i) == 0){
		
			hist->SetBinContent(i,(hist->GetBinContent(i-1)+hist->GetBinContent(i+1))/2);		

		}
			//cout<<"\t\t ,  "<<hist->GetBinContent(i)<<endl;
	}


	return hist;
}
  const double minK40 = 1.3;
  const double maxK40 = 1.5;
  const double minRadon = 0.25;
  const double maxRadon = 0.8;

TH1* doAnalysis(TH1* Template, TH1 *h_diff,TDirectory *dir, TFile *ofile){

	double K40 = Template->Integral(Template->GetXaxis()->FindBin(minK40),Template->GetXaxis()->FindBin(maxK40));	

	TKey *keyAsObj, *keyAsObj2;
	TIter next(dir->GetListOfKeys());
	TH1* obj;

	while ((keyAsObj = (TKey*) next())){

		auto key = (TKey*) keyAsObj;
		TDirectory* dir2 = (TDirectory*)dir->Get(key->GetName());
		dir2->cd();
		TIter next2(dir2->GetListOfKeys());

		while ((keyAsObj2 = (TKey*)next2())){

		  auto key2 = (TKey*) keyAsObj2;
		  obj = (TH1*)dir2->Get(key2->GetName()); // copy every th1 histogram to obj
  		  obj = setZeroBinContent(obj);
		  double obj_K40 = obj->Integral(obj->GetXaxis()->FindBin(minK40),obj->GetXaxis()->FindBin(maxK40));

		  if(obj_K40 > 0){

			double factor = obj_K40/K40;
			//cout<<"factor "<<factor<<endl;
			TH1D *ScaledTemplate = (TH1D*)(Template->Clone("ScaledTemplate"));
			ScaledTemplate -> Scale(obj_K40/K40); // template scale to same as the hourly plot

			double nTemplateSig = ScaledTemplate->Integral(ScaledTemplate->GetXaxis()->FindBin(minRadon),ScaledTemplate->GetXaxis()->FindBin(maxRadon));
			double nDailySig = obj->Integral(obj->GetXaxis()->FindBin(minRadon),obj->GetXaxis()->FindBin(maxRadon));
			double diff = nDailySig-nTemplateSig;
			//cout<<"diff "<<diff<<endl;
			h_diff->Fill(diff);
			delete obj;
			delete ScaledTemplate;
		  }
		}
	}
	return h_diff;

}
void Analyzer(){

	TCanvas *canvas = new TCanvas("canvas","",800,600);

	TFile *ofile = new TFile("o.root","recreate");
	ofile->mkdir("AdjustedHist");

	TFile *file= new TFile("output.root");
	TDirectory *dir = (TDirectory*)file->Get("HistoCh0");
	dir->cd();
	
	//make a template
	TH1* Template = addHist(dir);
	Template->Draw();
	canvas->SaveAs("template.pdf");

	// check zero entry bin
	Template = setZeroBinContent(Template);
	Template->Draw();
	canvas->SaveAs("template_adjust.pdf");

	//do analysis
	TH1D *h_diff = new TH1D("h_diff","",100,-10000,10000);
	doAnalysis(Template,h_diff,dir,ofile);
	
	//Fitting to get sigma
	
//	TF1 *f1  = new TF1("f1",TMath::Gaus(x,[0]/*mean*/,[1]/*sigma*/),-10000,10000);
//	f1->SetParameter(-17);
//	f1->SetParameter(3000);
	h_diff->Fit("gaus","L");
    h_diff->Draw();	
//	canvas->SaveAs("h_diff.pdf");


}
