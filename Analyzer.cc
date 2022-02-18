TH1* AddHist(TH1 *obj, TDirectory *dir, TDirectory *dir_inner){
	int count = 0;
	
	TH1D *Template = new TH1D ("Template","",1024,-0.5,4.5);
	//TKey *key;
	//TIter next(file->GetListOfKeys())
	for (auto&& keyAsObj : *dir->GetListOfKeys()){
		for (auto&& keyAsObj2 : *dir_inner->GetListOfKeys()){
		count++;
		 auto key = (TKey*) keyAsObj2;
		// cout << key->GetName() << " " << key->GetClassName() << endl;
		 obj = (TH1*)dir_inner->Get(key->GetName()); // copy every th1 histogram to obj
	
		 Template->Add( obj );
	         delete obj;
	  }
	}
	Template->SetXTitle("Energy (MeV)");
	Template->SetYTitle("Entries");
	//Template->SetStats(0);
	
	cout<<count<<endl;	
	return Template;
			

}
void Analyzer(){

	TCanvas *canvas = new TCanvas("canvas","",800,600);

	TFile *file= new TFile("output.root");
	TDirectory *dir = (TDirectory*)file->Get("HistoCh0");
	dir->cd();
	TDirectory *dir_inner;
	dir_inner->cd();

//	TFile *oFile = new TFile("o.root","recreate");
	TH1 *obj ;

	TH1* Template = AddHist(obj,dir,dir_inner);
	Template->Draw();
	int bmin = Template->GetXaxis()->FindBin(1.3);
	int bmax = Template->GetXaxis()->FindBin(1.5);
	cout<<"bmin "<<bmin<<", bmax "<<bmax<<endl;
	double K40 = Template->Integral(bmin,bmax);
	cout<<"Integral K40 "<<K40<<endl;
	
	int count = 0;
	//scale
	
	for (auto&& keyAsObj : *dir->GetListOfKeys()){
	  for (auto&& keyAsObj2 : *dir_inner->GetListOfKeys()){
		auto key = (TKey*) keyAsObj2;
		obj = (TH1*)dir_inner->Get(key->GetName()); // copy every th1 histogram to obj
		 
		count++;
		int bmin = Template->GetXaxis()->FindBin(1.3);
		int bmax = Template->GetXaxis()->FindBin(1.5);
		//double obj_K40 = obj->Integral(bmin,bmax);
		double obj_K40 = obj->Integral();
		cout<<"count "<<count<<", obj_Integral() "<<obj_K40<<endl;
//		double factor = K40/obj_K40;
//		obj->Scale(factor);
	        delete obj;
	  }
	}
//	oFile->cd("Template");
//	oFile->cd("../");
//	oFile->cd("Scale/
//	Template->Write();
//	output->Close();
	cout<<count<<endl;	
		

}
