/*******************************************************************************
*
*  Filename    : GetRange.cc
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/

#include "TGraph.h"
#include "TH1.h"
#include <algorithm>
#include <float.h>
#include <math.h>
#include <string>
#include <vector>

#include <iostream>

namespace mgr{
    /*******************************************************************************
    *   Getting histogram graphically maximum point
    *******************************************************************************/
    double
    GetYmax( const TH1* hist )
    {
        double ans = 0;

        for( int i = 1; i <= hist->GetNcells(); ++i ){
            const double bincont = hist->GetBinContent( i );
            const double binerr  = hist->GetBinError( i );
            ans = std::max( ans, fabs( bincont + binerr ) );
        }

        return ans;
    }

    /******************************************************************************/

    double
    GetYmax( const std::vector<TH1*>& histlist )
    {
        double ans = 0;

        for( const auto& hist : histlist ){
            ans = std::max( ans, GetYmax( hist ) );
        }

        return ans;
    }

    /*******************************************************************************
    *   TGraph Max and min calculations
    *******************************************************************************/
    double
    GetYmax( const TGraph* x )
    {
        double ans = -DBL_MAX;

        for( int i = 0; i < x->GetN(); ++i ){
            const double bin = x->GetY()[ i ] + x->GetErrorYhigh( i );
            ans = std::max( ans, fabs( bin ) );
        }

        return ans;
    }

    /******************************************************************************/

    double
    GetYmin( const TGraph* x )
    {
        double ans = DBL_MAX;

        for( int i = 0; i < x->GetN(); ++i ){
            const double bin = x->GetY()[ i ] - x->GetErrorYlow( i );
            ans = std::min( ans, bin );
        }

        return ans;
    }

    /******************************************************************************/

    double
    GetYmax( const std::vector<TGraph*>& list )
    {
        double ans = -DBL_MAX;

        for( const auto& graph : list ){
            ans = std::max( ans, GetYmax( graph ) );
        }

        return ans;
    }

    /******************************************************************************/

    double
    GetYmin( const std::vector<TGraph*>& list )
    {
        double ans = DBL_MAX;

        for( const auto& graph : list ){
            ans = std::min( ans, GetYmin( graph ) );
        }

        return ans;
    }
}
