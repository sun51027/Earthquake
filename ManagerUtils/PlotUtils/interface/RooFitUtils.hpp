/*******************************************************************************
*
*  Filename    : RooFitUtils.hpp
*  Description : Utility functions for better RooFit interaction
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef MANAGERUTILS_PLOTUTILS_ROOFITUTILS_HPP
#define MANAGERUTILS_PLOTUTILS_ROOFITUTILS_HPP

#include "RooAbsData.h"
#include "RooAbsPdf.h"
#include "RooPlot.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"

namespace mgr{
    /*******************************************************************************
    *   ** PlotOn
    *   Wrapper class for a RooFit Object plotOn function. Returns the generated
    *   TGraph instance to allow for advance styling.
    *******************************************************************************/
    template<class T>
    TGraph*
    PlotOn( RooPlot*         frame,
            T*               rooobj,
            const RooCmdArg& arg1 = RooCmdArg::none(),
            const RooCmdArg& arg2 = RooCmdArg::none(),
            const RooCmdArg& arg3 = RooCmdArg::none(),
            const RooCmdArg& arg4 = RooCmdArg::none(),
            const RooCmdArg& arg5 = RooCmdArg::none() )
    {
        rooobj->plotOn( frame, arg1, arg2, arg3, arg4, arg5 );
        return (TGraph*)frame->getObject( frame->numItems() - 1 );
    }

    /*******************************************************************************
    *   ** SetFrame
    *   Setting RooPlot to common setting with the common settings in Common.hpp
    *   with additional RooPlot specializations.
    *******************************************************************************/
    extern void SetFrame( RooPlot* );// Setting common frame options

    /*******************************************************************************
    *   Creating TGraphAsymmErrors represting function with error regions for
    *   objects from RooPlot generated
    *******************************************************************************/
    extern TGraphAsymmErrors* MakeCurveError(
        TGraph* errorplot,
        TGraph* centralplot
        );

    extern TGraph* PlotFitErrorOn(
        RooPlot*         frame,
        RooAbsPdf*       pdf,
        RooFitResult*    fitres,
        const RooCmdArg& arg1 = RooCmdArg::none(),
        const RooCmdArg& arg2 = RooCmdArg::none(),
        const RooCmdArg& arg3 = RooCmdArg::none()
        );
};/* plt */


#endif/* end of include guard: MANAGERUTILS_PLOTUTILS_ROOFITUTILS_HPP */
