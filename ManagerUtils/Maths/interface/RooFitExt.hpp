/*******************************************************************************
*
*  Filename    : RooFitExt.hpp
*  Description : Extension to some mathematical computations for RooFit objects
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef MANAGERUTILS_MATHS_ROOFITEXT_HPP
#define MANAGERUTILS_MATHS_ROOFITEXT_HPP

#include <RooAbsData.h>
#include <RooAbsPdf.h>

/*******************************************************************************
*   Kolmongorov-Smirov Test
*   * Using the RooAbsPdf::generate() and RooDataSet.createHistorgram() to
*   Create Root flavour histogram objects to compute the Kolmongorov-Smirov
*   test probability value.
*   * Parameters
*      RooRealVar& x: the variable you wish to compute the KS test (only 1D right now)
*      double gen: Generated number of events will be 10 times greater
*                  than the RooDataSet.sumEntries return value
*      unsigned bin: Number of bins in generated histograms will be 1000 bins
*                    by default.
*******************************************************************************/
extern double KSTest(
    RooAbsData&      dataset,
    RooAbsPdf&       pdf,
    RooRealVar&      var,
    const RooCmdArg& cut = RooCmdArg::none(),
    double           gen = 10,
    unsigned         bin = 1000
    );

#endif/* end of include guard: MANAGERUTILS_MATHS_ROOFITEXT_HPP */
