#include "ManagerUtils/PlotUtils/interface/Common.hpp"
#include "ManagerUtils/PlotUtils/interface/RooFitUtils.hpp"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooRealVar.h"
#include <iostream>
#include <map>

using namespace std;

int
main( int argc, char const* argv[] )
{
    RooRealVar x( "x", "x", 0, -5, 5 );
    RooRealVar m( "m", "m", 0, -5, 5 );
    RooRealVar s( "s", "s", 1, 0, 3 );
    RooGaussian g( "g", "g", x, m, s );
    RooDataSet* dat   = g.generate( x, 100 );
    RooFitResult* res = g.fitTo( *dat, RooFit::Save() );
    RooPlot* frame    = x.frame();
    mgr::PlotOn( frame, dat );
    TGraph* fit    = mgr::PlotOn( frame, &g, RooFit::Invisible() );
    TGraph* fitres = mgr::PlotOn( frame, &g, RooFit::VisualizeError( *res, 1, kFALSE ), RooFit::Invisible() );
    TGraph* fitex  = mgr::MakeCurveError( fitres, fit );
    frame->addObject( fitex, "L3" );
    TCanvas* c = mgr::NewCanvas();
    frame->Draw();
    fitex->SetLineColor( kBlue );
    fitex->SetFillStyle( 3004 );
    fitex->SetFillColor( kCyan );
    fitex->SetLineWidth( 3 );
    mgr::SaveToPDF( c, "tmp/test.pdf" );
    return 0;
}
