/*******************************************************************************
*
*  Filename    : StatisticsUtil.hpp
*  Description : General purpose Statistics related utility functions
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef MANAGERUTILS_MATHS_STATISTICUTIL_HPP
#define MANAGERUTILS_MATHS_STATISTICUTIL_HPP

#include "ManagerUtils/Maths/interface/GSLUtil.hpp"

namespace mgr{
    namespace stat{
        /*******************************************************************************
        *   Sigma interval - Confidence level conversion.
        *******************************************************************************/
        extern double NormalCDF( const double x );
        extern double GetConfidenceLevel( const double sigmainterval );
        extern double GetSigmaInterval( const double confidencelevel );

        extern const double onesigma_level;
        extern const double twosigma_level;

        /*******************************************************************************
        *   General purpose Minos error computation
        *******************************************************************************/
        extern double DeltaNLLFromSigma( const double sigma );
        extern double DeltaNLLFromConfidence( const double confidence );

        extern int MinosError(
            gsl_function* nllfunction,
            double&       guess,
            double&       min,
            double&       max,
            const double  confidencelevel = onesigma_level
            );


        extern int MinosError(
            mgr::gsl::gsl_multifunc* nllfunction,
            mgr::gsl::gsl_multifunc* varfunction,
            gsl_vector*              initguess,
            double&                  central,
            double&                  min,
            double&                  max,
            const double             confidencelevel = onesigma_level,
            gsl_vector*              upperguess = nullptr,
            gsl_vector*              lowerguess = nullptr
            );

        /*******************************************************************************
        *   Common Distribution NLLs
        *   Defined in standard gsl_function format
        *******************************************************************************/

        /*******************************************************************************
        *   double params[0] - mean,
        *   double params[1] - sigma
        *******************************************************************************/
        extern double GaussianNLL( double, void* params );

        /*******************************************************************************
        *   double param[0] - passed
        *   double param[1] - total
        *******************************************************************************/
        extern double BinomialNLL( double, void* params );

        /*******************************************************************************
        *   double param[0] - observed
        *******************************************************************************/
        extern double PoissonNLL( double, void* params );
    }/* stat */
}/* mgr */

#endif/* end of include guard: MANAGERUTILS_MATHS_STATISTICUTIL_HPP */
