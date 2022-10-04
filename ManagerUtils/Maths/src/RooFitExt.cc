/*******************************************************************************
*
*  Filename    : RooFitExt.hpp
*  Description : Implementation for RooFitExt.hpp defined functions
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "ManagerUtils/Maths/interface/RooFitExt.hpp"
#include <RooAbsData.h>
#include <RooAbsPdf.h>
#include <RooDataSet.h>
#include <RooRealVar.h>
#include <TH1.h>

/*******************************************************************************
*   Kolmongorov-Smirov Test
*******************************************************************************/
double
KSTest(
    RooAbsData&      dataset,
    RooAbsPdf&       pdf,
    RooRealVar&      var,
    const RooCmdArg& cut,
    double           gen,
    unsigned         bin
    )
{
    RooDataSet* pdfset    = pdf.generate( RooArgSet( var ), dataset.sumEntries() * gen );
    RooAbsData* pdfsubset = pdfset->reduce( RooFit::SelectVars( RooArgSet( var ) ), cut );
    TH1* pdfhist          = pdfsubset->createHistogram( "pdfhist", var, RooFit::Binning( bin ) );
    RooAbsData* setsubset = dataset.reduce( RooFit::SelectVars( RooArgSet( var ) ), cut );
    TH1* dathist          = setsubset->createHistogram( "dathist", var, RooFit::Binning( bin ) );
    double ans            = pdfhist->KolmogorovTest( dathist );
    delete pdfhist;
    delete dathist;
    delete pdfset;
    delete pdfsubset;
    delete setsubset;
    return ans;
}
