TH1* addHist( TDirectory *dir){
	int count = 0;
	
	TH1D *Template = new TH1D ("Template","",1024,-0.5,4.5);

	TKey *keyAsObj, *keyAsObj2;
	TIter next(dir->GetListOfKeys());
	TH1* obj;

	while ((keyAsObj = (TKey*) next())){
		auto key = (TKey*) keyAsObj;
	//	cout << key->GetName() << " " << key->GetClassName() << endl;
		TDirectory* dir2 = (TDirectory*)dir->Get(key->GetName());
		dir2->cd();
		TIter next2(dir2->GetListOfKeys());
		while ((keyAsObj2 = (TKey*)next2())){
			count++;	
			auto key2 = (TKey*) keyAsObj2;
			obj = (TH1*)dir2->Get(key2->GetName()); // copy every th1 histogram to obj
			double obj_K40 = obj->Integral();
	//		cout<<"count "<<count<<", obj_Integral() "<<obj_K40<<endl;
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

	ofile->cd("ScaleHist");
	obj->Write();
}
void scale(TH1* Template,TFile *ofile, TDirectory *dir ){

	double K40 = Template->Integral(Template->GetXaxis()->FindBin(1.3),Template->GetXaxis()->FindBin(1.5));
	cout<<"Integral K40 "<<K40<<endl;
		
	TKey *keyAsObj, *keyAsObj2;
	TIter next(dir->GetListOfKeys());
	TH1* obj;

	while ((keyAsObj = (TKey*) next())){
		auto key = (TKey*) keyAsObj;
		//cout << key->GetName() << " " << key->GetClassName() << endl;
		TDirectory* dir2 = (TDirectory*)dir->Get(key->GetName());
		dir2->cd();
		TIter next2(dir2->GetListOfKeys());
		while ((keyAsObj2 = (TKey*)next2())){
			auto key2 = (TKey*) keyAsObj2;
			obj = (TH1*)dir2->Get(key2->GetName()); // copy every th1 histogram to obj
			int bmin = obj->GetXaxis()->FindBin(1.3);
			int bmax = obj->GetXaxis()->FindBin(1.5);
			double obj_K40 = obj->Integral(bmin,bmax);
			//cout<<key2->GetName()<<", obj_Integral() "<<obj_K40<<endl;
			double factor = obj_K40/K40;
			obj -> Scale(factor);
			string Name = (string)key->GetName()+(string)key2->GetName();
			obj -> SetName(Name.c_str());
			writeFile(obj,ofile);
			delete obj;
		}
	}
}
TH1* doAnalysis(TH1* Template, TH1 *h_diff,TFile *ofile){

	double nTemplateSig = Template->Integral(Template->GetXaxis()->FindBin(0.25),Template->GetXaxis()->FindBin(0.8));
	cout<<"nSiginficant interval "<<nTemplateSig<<endl;

	TH1 *obj_new ;
	TKey *keyAsObj_new;
	TDirectory *dir_new = (TDirectory*)ofile->Get("ScaleHist");
	TIter next_new(dir_new->GetListOfKeys());

	while ((keyAsObj_new = (TKey*)next_new() )){
		auto key_new = (TKey*) keyAsObj_new;
		obj_new = (TH1*)dir_new->Get(key_new->GetName());

		double nDailySig = obj_new->Integral(obj_new->GetXaxis()->FindBin(0.25),obj_new->GetXaxis()->FindBin(0.8));
		double diff = nDailySig-nTemplateSig;
		//cout<<"diff "<<diff<<endl;
		h_diff->Fill(diff);
		delete obj_new;
	}
	cout<<"h_diff Standard Dev "<<h_diff-> GetStdDev()<<endl;
	h_diff->Scale(1/h_diff->GetStdDev());
        h_diff->Draw();	
	return h_diff;

}
void Analyzer(){

	TCanvas *canvas = new TCanvas("canvas","",800,600);

	TFile *ofile = new TFile("o.root","recreate");
	ofile->mkdir("ScaleHist");

	TFile *file= new TFile("output.root");
	TDirectory *dir = (TDirectory*)file->Get("HistoCh0");
	dir->cd();
	
	//make a template
	TH1* Template = addHist(dir);
	Template->Draw();
	canvas->SaveAs("template.pdf");

	//Scale histogram to template
	scale(Template,ofile,dir);

	//do analysis
	TH1D *h_diff = new TH1D("h_diff","",100,-30000000,30000000);
	doAnalysis(Template,h_diff,ofile);
	canvas->SaveAs("h_diff.pdf");


}
