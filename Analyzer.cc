TH1* AddHist( TDirectory *dir){
	int count = 0;
	
	TH1D *Template = new TH1D ("Template","",1024,-0.5,4.5);

	TKey *keyAsObj, *keyAsObj2;
	TIter next(dir->GetListOfKeys());
	TH1* obj;

	while ((keyAsObj = (TKey*) next())){
		auto key = (TKey*) keyAsObj;
//		cout << key->GetName() << " " << key->GetClassName() << endl;
		TDirectory* dir2 = (TDirectory*)dir->Get(key->GetName());
		dir2->cd();
		TIter next2(dir2->GetListOfKeys());
		while ((keyAsObj2 = (TKey*)next2())){
			count++;	
			auto key2 = (TKey*) keyAsObj2;
			obj = (TH1*)dir2->Get(key2->GetName()); // copy every th1 histogram to obj
			double obj_K40 = obj->Integral();
//			cout<<"count "<<count<<", obj_Integral() "<<obj_K40<<endl;
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
void Analyzer(){

//	TCanvas *canvas = new TCanvas("canvas","",800,600);

	TFile *oFile = new TFile("o.root","recreate");
	TFile *file= new TFile("output.root");
	TDirectory *dir = (TDirectory*)file->Get("HistoCh0");
	dir->cd();
	
	//make a template
	TH1* Template = AddHist(dir);
	Template->Draw();

	//Scale histogram to template
	int bmin = Template->GetXaxis()->FindBin(1.3);
	int bmax = Template->GetXaxis()->FindBin(1.5);
	cout<<"bmin "<<bmin<<", bmax "<<bmax<<endl;
	double K40 = Template->Integral(bmin,bmax);
	cout<<"Integral K40 "<<K40<<endl;
	
		
	TKey *keyAsObj, *keyAsObj2;
	TIter next(dir->GetListOfKeys());
	TH1* obj;

	while ((keyAsObj = (TKey*) next())){
		auto key = (TKey*) keyAsObj;
		cout << key->GetName() << " " << key->GetClassName() << endl;
		TDirectory* dir2 = (TDirectory*)dir->Get(key->GetName());
		dir2->cd();
		TIter next2(dir2->GetListOfKeys());
		while ((keyAsObj2 = (TKey*)next2())){
			auto key2 = (TKey*) keyAsObj2;
			obj = (TH1*)dir2->Get(key2->GetName()); // copy every th1 histogram to obj
			int bmin = obj->GetXaxis()->FindBin(1.3);
			int bmax = obj->GetXaxis()->FindBin(1.5);
			double obj_K40 = obj->Integral(bmin,bmax);
			cout<<key2->GetName()<<", obj_Integral() "<<obj_K40<<endl;
			delete obj;
		}
	}

}
