/*******************************************************************************
*
*  Filename    : RooFitUtils.hh
*  Description : Implementation of functions defined in RooFitUtils.hh
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/

#include "ManagerUtils/PlotUtils/interface/Common.hpp"
#include "ManagerUtils/PlotUtils/interface/RooFitUtils.hpp"
#include "TAxis.h"
#include <iostream>
#include <map>

namespace mgr{
    void
    SetFrame( RooPlot* frame )
    {
        // Must set
        SetSinglePad( gPad );// Using global pad settings
        // Common Axis settings
        SetAxis( frame );// see PlotObj.cc
        frame->SetTitle( "" );
    }

    /******************************************************************************/

    TGraphAsymmErrors*
    MakeCurveError(
        TGraph* errorplot,
        TGraph* centralplot
        )
    {
        std::map<double, std::pair<double, double> > fiterr;

        for( int i = 0; i < errorplot->GetN(); ++i ){
            const double x = errorplot->GetX()[ i ];
            const double y = errorplot->GetY()[ i ];

            if( !fiterr.count( x ) ){
                fiterr[ x ] = std::make_pair( 1e10, -1e10 );
            }

            fiterr.at( x ).first  = std::min( fiterr.at( x ).first, y );
            fiterr.at( x ).second = std::max( fiterr.at( x ).second, y );
        }

        TGraphAsymmErrors* ans = new TGraphAsymmErrors( fiterr.size() );
        size_t i               = 0;

        for( const auto& fiterrval : fiterr ){
            const double x   = fiterrval.first;
            const double y   = centralplot->Eval( x );
            const double ylo = fiterrval.second.first;
            const double yhi = fiterrval.second.second;
            ans->SetPoint( i, x, y );
            ans->SetPointError( i, 0, 0, y - ylo, yhi - y );
            ++i;
        }

        return ans;
    }

    TGraph*
    PlotFitErrorOn(
        RooPlot*         frame,
        RooAbsPdf*       pdf,
        RooFitResult*    fitres,
        const RooCmdArg& arg1,
        const RooCmdArg& arg2,
        const RooCmdArg& arg3
        )
    {
        TGraph* fit = PlotOn(
            frame, pdf,
            RooFit::Invisible(),
            arg1, arg2, arg3
            );
        TGraph* fiterr = PlotOn(
            frame, pdf,
            RooFit::VisualizeError( *fitres, 1, kFALSE ),
            RooFit::Invisible(),
            arg1, arg2, arg3
            );
        TGraphAsymmErrors* ans = MakeCurveError( fiterr, fit );
        frame->addObject( ans, "L3" );
        return ans;
    }
};
