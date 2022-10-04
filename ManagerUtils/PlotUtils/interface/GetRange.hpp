/*******************************************************************************
*
*  Filename    : GetRange.hpp
*  Description : Getting Range of Plotting objects
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef MANAGERUTILS_PLOTUTILS_GETRANGE_HPP
#define MANAGERUTILS_PLOTUTILS_GETRANGE_HPP

#include "../../Common/interface/Variadic.hpp"
#include "TGraph.h"
#include "TH1.h"
#include <vector>

namespace mgr{
    /*******************************************************************************
    *   * GetYMax - getting y axis maximum (including error) for a histogram
    *******************************************************************************/
    extern double GetYmax( const TH1* );
    extern double GetYmax( const std::vector<TH1*>& );

    /*******************************************************************************
    *   ** GetYmax( const TGraph* )
    *   ** GetYmin( const TGraph* ) Getting y max and y min of single graph
    *******************************************************************************/
    extern double GetYmax( const TGraph* );
    extern double GetYmin( const TGraph* );

    /*******************************************************************************
    *   ** GetYmax( const vector<TGraph*>& )
    *   ** GetYmin( const veoctr<TGraph*>& ) Getting ymax and ymin of multiple graphs
    *******************************************************************************/
    extern double GetYmax( const std::vector<TGraph*>& );
    extern double GetYmin( const std::vector<TGraph*>& );

    /*******************************************************************************
    *   Variadic interface
    *******************************************************************************/
    template<typename ... TS>
    double
    GetYmin( TGraph* x, TS ... arg )
    {
        return GetYmin( MakeVector<TGraph*>( x, arg ... ) );
    }

    /******************************************************************************/

    template<typename ... TS>
    double
    GetYmax( TGraph* x, TS ... arg )
    {
        return GetYmax( MakeVector<TGraph*>( x, arg ... ) );
    }

    /******************************************************************************/

    template<typename ... TS>
    double
    GetYmax( TH1* x, TS ... arg )
    {
        return GetYmax( MakeVector<TH1*>( x, arg ... ) );
    }
};

#endif/* end of include guard: MANAGERUTILS_PLOTUTILS_GETRANGE_HPP */
