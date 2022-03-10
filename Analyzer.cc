#include <iostream>

#include "TCanvas.h"
#include "TDirectory.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TKey.h"

using namespace std;
TH1 *addHist(TDirectory *dir)
{
   int count = 0;

   TH1D *Template = new TH1D("Template", "", 1024, -0.5, 4.5);

   TKey *keyAsObj, *keyAsObj2;
   TIter next(dir->GetListOfKeys());
   TH1  *obj;

   while ((keyAsObj = (TKey *)next())) {
      auto        key  = (TKey *)keyAsObj;
      TDirectory *dir2 = (TDirectory *)dir->Get(key->GetName());
      dir2->cd();
      TIter next2(dir2->GetListOfKeys());
      while ((keyAsObj2 = (TKey *)next2())) {
         count++;
         auto key2 = (TKey *)keyAsObj2;
         obj       = (TH1 *)dir2->Get(key2->GetName()); // copy every th1 histogram to
                                                        // obj
         double obj_K40 = obj->Integral();
         Template->Add(obj);

         delete obj;
      }
   }

   Template->SetXTitle("Energy (MeV)");
   Template->SetYTitle("Entries");
   // Template->SetStats(0);
   cout << count << endl;
   return Template;
}
void writeFile(TH1 *obj, TFile *ofile)
{
   ofile->cd("AdjustedHist");
   obj->Write();
}
TH1 *setZeroBinContent(TH1 *hist)
{
   for (int i = 0; i < hist->GetNbinsX(); i++) {
      if (hist->GetBinContent(i) == 0 && hist->GetBinContent(i + 1) == 0 && hist->GetBinContent(i + 2) == 0) {
         continue;
      } else if (hist->GetBinContent(i) == 0) {
         hist->SetBinContent(i, (hist->GetBinContent(i - 1) + hist->GetBinContent(i + 1)) / 2);
      }
   }

   return hist;
}
void FittingScale(TH1 *h_diff)
{
   TCanvas *canvas = new TCanvas("canvas", "", 800, 600);
   h_diff->Fit("gaus");
   h_diff->Draw();
   canvas->SaveAs("h_fitting_gaus.pdf");

   TH1D *h_scale = new TH1D("h_scale", "", 100, -10000, 10000);
   for (int i = 0; i < 100; i++) {
      h_scale->SetBinContent(i + 1, h_diff->GetBinContent(i + 1) / (h_diff->GetFunction("gaus")->GetParameter(2)));
   }
   h_scale->Draw();
   h_scale->SetStats(0);
   canvas->SaveAs("h_scale.pdf");

   double sigma = h_diff->GetFunction("gaus")->GetParameter(2);
}
const double minK40   = 1.3;
const double maxK40   = 1.5;
const double minRadon = 0.25;
const double maxRadon = 0.8;

void doAnalysis(TH1 *Template, TH1 *h_diff, TDirectory *dir, TFile *ofile)
{
   TCanvas *c   = new TCanvas("canvas", "", 800, 600);
   double   K40 = Template->Integral(Template->GetXaxis()->FindBin(minK40), Template->GetXaxis()->FindBin(maxK40));

   TKey    *keyAsObj, *keyAsObj2;
   TIter    next(dir->GetListOfKeys());
   TH1     *obj;
   Double_t x[4000], y[4000];
   Int_t    n = 2500;
   int      i = 0;
   TString  hist_name[2500];

   while ((keyAsObj = (TKey *)next())) {
      auto        key  = (TKey *)keyAsObj;
      TDirectory *dir2 = (TDirectory *)dir->Get(key->GetName());
      dir2->cd();
      TIter next2(dir2->GetListOfKeys());

      while ((keyAsObj2 = (TKey *)next2())) {
         auto key2 = (TKey *)keyAsObj2;
         obj       = (TH1 *)dir2->Get(key2->GetName()); // copy every th1 histogram to
                                                        // obj

         obj            = setZeroBinContent(obj);
         double obj_K40 = obj->Integral(obj->GetXaxis()->FindBin(minK40), obj->GetXaxis()->FindBin(maxK40));

         if (obj_K40 > 0) {
            double factor         = obj_K40 / K40;
            TH1D  *ScaledTemplate = (TH1D *)(Template->Clone("ScaledTemplate"));
            ScaledTemplate->Scale(obj_K40 / K40); // template scale to same
                                                  // as the hourly plot

            double nTemplateSig = ScaledTemplate->Integral(ScaledTemplate->GetXaxis()->FindBin(minRadon),
                                                           ScaledTemplate->GetXaxis()->FindBin(maxRadon));
            double nDailySig    = obj->Integral(obj->GetXaxis()->FindBin(minRadon), obj->GetXaxis()->FindBin(maxRadon));
            double diff         = nDailySig - nTemplateSig;

            hist_name[i].Form("%s", key->GetName());
            hist_name[i].Remove(0, 4);
            hist_name[i].Insert(2, "/");
            //		cout<<i<<"
            //"<<hist_name[i]<<endl;

            x[i] = (double)(i + 1);
            y[i] = diff;
            i++;
            h_diff->Fill(diff);

            delete obj;
            delete ScaledTemplate;
         }
      }
   }

   TGraph *gr = new TGraph(n, x, y);

   for (int i = 0; i < 25; i++) {
      //		cout<<"x["<<i*100<<"] "<<x[i*100]<<" day
      //"<<x[i*100]<<", GetbinLabel
      //"<<gr->GetXaxis()->FindBin(x[i*100])<<endl; x[0]   = 1   day 1
      // FindBin(1) = x[100] = 101 day 101 FindBin(101)
      gr->GetXaxis()->SetBinLabel(i * 100, hist_name[i * 100]);
      //	cout<<"i*100 "<<i*100<<" "<<hist_name[i*100]<<endl;
   }

   for (int i = 0; i < n; i++) {
      cout << i << " " << gr->GetPointX(i) << "\t" << gr->GetPointY(i) << endl;
      ;
   }
   cout << gr->GetXaxis()->GetXmax() << endl;
   cout << gr->GetXaxis()->GetXmin() << endl;
   gr->GetXaxis()->SetLimits(0, 2500);
   gr->Draw("AP");
   c->SetGrid();
   c->SaveAs("DiffvsTime.pdf");

   //	FittingScale(h_diff);
}
// void Analyzer(){
int main()
{
   TCanvas *canvas = new TCanvas("canvas", "", 800, 600);

   TFile *ofile = new TFile("o.root", "recreate");
   ofile->mkdir("AdjustedHist");

   TFile      *file = new TFile("output.root");
   TDirectory *dir  = (TDirectory *)file->Get("HistoCh0");
   dir->cd();

   // make a template
   TH1 *Template = addHist(dir);
   //	Template->Draw();
   ///	canvas->SaveAs("template.pdf");

   // check zero entry bin
   Template = setZeroBinContent(Template);
   //	Template->Draw();
   //	canvas->SaveAs("template_adjust.pdf");

   // do analysis
   TH1D *h_diff = new TH1D("h_diff", "", 100, -10000, 10000);
   doAnalysis(Template, h_diff, dir, ofile);

   // fitting

   return 0;
}
