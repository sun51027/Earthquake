/*******************************************************************************
*
*  Filename    : PlotObj.hpp
*  Description : Function for delcaring plotting objects
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef MANAGERUTILS_PLOTUTILS_PLOTOBJ_HPP
#define MANAGERUTILS_PLOTUTILS_PLOTOBJ_HPP

#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TPad.h"
#include "TPaveText.h"

namespace mgr{
    /*******************************************************************************
    *   * New Pad - default settings for new padding in top and botom pads
    *******************************************************************************/
    extern TCanvas* NewCanvas();
    extern TPad*    NewTopPad();
    extern TPad*    NewBottomPad();
    extern void     SetSinglePad( TVirtualPad* );
    extern void     SetSinglePadWithPalette( TVirtualPad* );

    /*******************************************************************************
    *   * New Legend
    *   Create new Tlegend instance with all the font/size settings done.
    *******************************************************************************/
    extern TLegend* NewLegend(
        const float xmin,
        const float ymin,
        const float xmax = PLOT_X_TEXT_MAX,
        const float ymax = PLOT_Y_TEXT_MAX );

    /*******************************************************************************
    *   * NewTextBox
    *   Creating a new TextBox instance with all the font/sizing settings done.
    *******************************************************************************/
    extern TPaveText* NewTextBox(
        const float xmin,
        const float ymin,
        const float xmax,
        const float ymax
        );

    /*******************************************************************************
    *   * Divide Histogram objects
    *   Creates a new TGraph such that the binning contents of the bin is the
    *   numerical division of the of the numerator histogram and the denominator
    *   histogram. Sets the bins where both are empty to a set value (default to 1)
    *******************************************************************************/
    extern TH1D* DivideHist(
        TH1D*        num,
        TH1D*        den,
        const double cen = 1
        );

    /*******************************************************************************
    *   * Divide TGraphAsymmErrors
    *   Creating a new TGraph that is the division of two existing TGraphs,
    *   the numerator number of points will be used. The points in between
    *   for the demominator will ge evaluted using the Eval function:
    *
    *   >  NewGraph->GetY()[i] == Num->GetY()[i] / Den->Eval( Num->GetX()[i] );
    *
    *   The Y error will be a simple devition of the Numerators value by the
    *   Denominators central value. No error propagation is calcualted:
    *
    *   >  NewGraph->GetErrorYhigh(i) = Num->GetErrorYhigh(i) / Den->Eval( Num->GetX()[i]);
    *
    *   The X Error will be a duplicate
    *
    *   >  NewGraph->GetErrorXhigh(i) = Num->GetErrorXhigh(i)
    *
    *******************************************************************************/
    extern TGraph* DividedGraphSimple(
        TGraph* num,
        TGraph* den
        );

    extern TGraphAsymmErrors* DividedGraph(
        TGraphAsymmErrors* num,
        TGraph*            den
        );

    extern TGraphAsymmErrors* PullGraph(
        TGraph*            numgraph,
        TGraphAsymmErrors* dengraph
        );
};


#endif/* end of include guard: MANAGERUTILS_PLOTUTILS_PLOTOBJ_HPP */
