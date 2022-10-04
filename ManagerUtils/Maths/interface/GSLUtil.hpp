/*******************************************************************************
*
*  Filename    : GSLUtil.hpp
*  Description : Helper Utility functions using GSL
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef MANAGERUTILS_MATHS_GSLSETTINGS_HPP
#define MANAGERUTILS_MATHS_GSLSETTINGS_HPP

#include <gsl/gsl_min.h>
#include <gsl/gsl_multimin.h>
#include <gsl/gsl_multiroots.h>
#include <gsl/gsl_roots.h>
#include <gsl/gsl_vector.h>

namespace mgr{
    namespace gsl{
        /*******************************************************************************
        *   Common GSL settings constants
        *******************************************************************************/
        extern const double epsilon;
        extern const unsigned max_iteration;

        /*******************************************************************************
        *   GSL iterator solver
        *******************************************************************************/
        extern void IterateSolver( gsl_multiroot_fsolver* solver );
        extern void IterateSolver( gsl_root_fsolver* solver );
        extern void IterateSolver( gsl_min_fminimizer* solver );
        extern void IterateSolver( gsl_multimin_fminimizer* solver );

        /*******************************************************************************
        *   Solving a one dimension gsl_function func at func(x) = a
        *******************************************************************************/
        extern double Solve1D(
            gsl_function* function,
            double        yval,
            double        xmin,
            double        ymax
            );

        /*******************************************************************************
        *   Multi variable functions
        *   Using same wrapper as gsl minimizer function:
        *    - double (* f) (const gsl_vector * x, void * params)
        *    - size_t n
        *    - void* params
        *******************************************************************************/
        typedef gsl_multimin_function gsl_multifunc;

        /*******************************************************************************
        *   Handy parameter-less gsl_multi_function function pointers
        *******************************************************************************/
        double sum( const gsl_vector* x, void* params );
        double product( const gsl_vector* x, void* params );

        /*******************************************************************************
        *   Multidimension projection function
        *******************************************************************************/
        struct projected_param
        {
            gsl_multifunc* original_func;
            gsl_vector*    original_x;
            size_t         varidx;
        };

        extern double projected_function( double, void* );

        /*******************************************************************************
        *   Partial derivative - Wrapper for gsl_deriv function
        *******************************************************************************/
        extern int partial_deriv(
            gsl_multifunc* function,
            gsl_vector*    x,
            size_t         varidx,
            double         h,
            double&        result,
            double&        error
            );

        extern double partial_deriv(
            gsl_multifunc* function,
            gsl_vector*    x,
            size_t         varidx,
            double         h = epsilon
            );

        extern double partial_deriv_error(
            gsl_multifunc* function,
            gsl_vector*    x,
            size_t         varidx,
            double         h = epsilon
            );
    };/* gsl */
}/* mgr */

#endif/* end of include guard: MANAGERUTILS_MATHS_GSLSETTINGS_HPP */
