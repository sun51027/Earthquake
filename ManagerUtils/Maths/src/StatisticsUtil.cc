/*******************************************************************************
*
*  Filename    : StatisticsUtil.cc
*  Description : Implementation of statistics utility functions
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "ManagerUtils/Maths/interface/GSLUtil.hpp"

#include <gsl/gsl_min.h>
#include <gsl/gsl_roots.h>
#include <gsl/gsl_sf_erf.h>

#include <iostream>

namespace mgr{
    namespace stat{
        /*******************************************************************************
        *  Sigma interval - Confidence Level conversion functions
        *  The definition of the GSL error function:
        *    https://www.gnu.org/software/gsl/manual/html_node/Error-Function.html#Error-Function
        *    erf(x) = (2/\sqrt(\pi)) \int_0^x dt \exp(-t^2)
        *  Conversion to CDF of Normal distribution:
        *    https://en.wikipedia.org/wiki/Normal_distribution
        *    CDF(x) = 1/2 * ( 1 + erf(x/sqrt(2)))
        *
        *******************************************************************************/
        double
        NormalCDF( const double x )
        {
            return 0.5 * ( 1 + gsl_sf_erf( x / sqrt( 2. ) ) );
        }

        /******************************************************************************/

        double
        GetConfidenceLevel( const double sigmainterval )
        {
            return NormalCDF( sigmainterval ) - NormalCDF( -sigmainterval );
        }

        /******************************************************************************/

        double
        GetSigmaInterval( const double confidencelevel )
        {
            static const double maxinterval = 10;// Trying to get a larger than 10 sigma is impractical
            gsl_root_fsolver* solver        = gsl_root_fsolver_alloc( gsl_root_fsolver_brent );
            gsl_function func;
            func.function = [ ]( double x, void* y ){
                                return GetConfidenceLevel( x ) - *( (const double*)y );
                            };
            func.params = const_cast<double*>( &confidencelevel );
            gsl_root_fsolver_set( solver, &func, 0, maxinterval );
            gsl::IterateSolver( solver );
            const double ans = gsl_root_fsolver_root( solver );
            gsl_root_fsolver_free( solver );
            return ans;
        }

        extern const double onesigma_level = GetConfidenceLevel( 1 );

        extern const double twosigma_level = GetConfidenceLevel( 2 );


        /*******************************************************************************
        *   General MinosError computing functions
        *******************************************************************************/
        double
        DeltaNLLFromSigma( const double sigma )
        {
            return sigma * sigma / 2;
        }

        double
        DeltaNLLFromConfidence( const double confidence )
        {
            return DeltaNLLFromSigma( GetSigmaInterval( confidence ) );
        }

        /******************************************************************************/

        int
        MinosError(
            gsl_function* nllfunction,
            double&       guess,
            double&       min,
            double&       max,
            const double  confidencelevel
            )
        {
            // Getting central value via minimization
            gsl_min_fminimizer* solver = gsl_min_fminimizer_alloc( gsl_min_fminimizer_brent );
            gsl_min_fminimizer_set( solver, nllfunction, guess, min, max );
            mgr::gsl::IterateSolver( solver );
            // saving central value to initial guess variable
            guess = gsl_min_fminimizer_x_minimum( solver );
            gsl_min_fminimizer_free( solver );
            // Getting error via solving nll(x) = nll(x_min)+ sigmainterval^2 / 2
            double deltanll  = DeltaNLLFromConfidence( confidencelevel );
            double nllmin    = nllfunction->function( guess, nllfunction->params );
            double nlltarget = nllmin + deltanll;
            max = mgr::gsl::Solve1D( nllfunction, nlltarget, guess, max );
            min = mgr::gsl::Solve1D( nllfunction, nlltarget, min, guess );
            return 0;
        }

        /******************************************************************************/

        int
        MinosError(
            mgr::gsl::gsl_multifunc* nllfunction,
            mgr::gsl::gsl_multifunc* varfunction,
            gsl_vector*              initguess,
            double&                  central,
            double&                  min,
            double&                  max,
            const double             confidencelevel,
            gsl_vector*              upperguess,
            gsl_vector*              lowerguess
            )
        {
            // Minimizing to get central value
            static const double defaultstep = 0.005;
            const size_t dim                = nllfunction->n;
            gsl_multimin_fminimizer* solver = gsl_multimin_fminimizer_alloc( gsl_multimin_fminimizer_nmsimplex2, dim );
            gsl_vector* step                = gsl_vector_alloc( dim );
            gsl_vector* bestinput           = gsl_vector_alloc( dim );

            // All steps required setting to 1
            for( size_t i = 0; i < dim; ++i ){
                double stepsize = defaultstep;// Default step size
                const double x  = gsl_vector_get( initguess, i );

                if( upperguess ){
                    const double y = gsl_vector_get( upperguess, i );
                    stepsize = std::min( stepsize, fabs( y - x ) * defaultstep );
                }

                if( lowerguess ){
                    const double y = gsl_vector_get( upperguess, i );
                    stepsize = std::min( stepsize, fabs( y - x ) * defaultstep );
                }

                gsl_vector_set( step, i, stepsize );
            }

            gsl_multimin_fminimizer_set( solver, nllfunction, initguess, step );
            mgr::gsl::IterateSolver( solver );
            gsl_vector_memcpy( bestinput, solver->x );
            central = varfunction->f( bestinput, varfunction->params );
            double nllmin = nllfunction->f( bestinput, nllfunction->params );
            gsl_vector_free( step );
            gsl_multimin_fminimizer_free( solver );
            // Calculating the error by multidimensional root finding
            gsl_multiroot_fsolver* errsolver = gsl_multiroot_fsolver_alloc( gsl_multiroot_fsolver_hybrids, dim + 1 );
            struct minosparam
            {
                mgr::gsl::gsl_multifunc* nllfunc;
                mgr::gsl::gsl_multifunc* varfunc;
                double                   yval;
            };
            gsl_multiroot_function minosfunc;
            minosfunc.f = [ ]( const gsl_vector* input, void* p, gsl_vector* output ){
                              gsl_vector* reduced = gsl_vector_alloc( input->size - 1 );
                              double lambda       = gsl_vector_get( input, input->size - 1 );
                              minosparam* param   = (minosparam*)p;

                              for( size_t i = 0; i < reduced->size; ++i ){
                                  gsl_vector_set( reduced, i, gsl_vector_get( input, i ) );
                              }

                              for( size_t i = 0; i < reduced->size; ++i ){
                                  double nlldiv = mgr::gsl::partial_deriv( param->nllfunc, reduced, i );
                                  double vardiv = mgr::gsl::partial_deriv( param->varfunc, reduced, i );
                                  gsl_vector_set( output, i, nlldiv - lambda * vardiv );
                              }

                              double nllval = param->nllfunc->f( reduced, param->nllfunc->params );
                              gsl_vector_set( output, input->size - 1, nllval - param->yval );
                              return (int)GSL_SUCCESS;
                          };
            minosfunc.n = dim + 1;
            minosparam tmp = { nllfunction, varfunction, nllmin + DeltaNLLFromConfidence( confidencelevel ) };
            minosfunc.params = &tmp;
            // Starting from minimal point
            gsl_vector* errinitguess = gsl_vector_alloc( dim + 1 );
            gsl_vector* finalinput   = gsl_vector_alloc( dim );

            for( size_t i = 0; i < dim; ++i ){
                if( upperguess ){// If upper guess is provided.
                    gsl_vector_set( errinitguess, i, gsl_vector_get( upperguess, i ) );
                }
                else{// Otherwise stargin for Minimum point with small offset
                    gsl_vector_set( errinitguess, i, gsl_vector_get( bestinput, i ) + defaultstep );
                }
            }

            gsl_vector_set( errinitguess, dim, 0.01 );
            gsl_multiroot_fsolver_set( errsolver, &minosfunc, errinitguess );
            mgr::gsl::IterateSolver( errsolver );

            for( size_t i = 0; i < finalinput->size; ++i ){
                gsl_vector_set( finalinput, i, gsl_vector_get( errsolver->x, i ) );
            }

            double boundary1 = varfunction->f( finalinput, varfunction->params );

            // Getting to other side:
            for( size_t i = 0; i < dim; ++i ){
                if( lowerguess ){
                    gsl_vector_set( errinitguess, i, gsl_vector_get( lowerguess, i ) );
                }
                else{
                    double diff = gsl_vector_get( finalinput, i ) - gsl_vector_get( bestinput, i );
                    gsl_vector_set( errinitguess, i, gsl_vector_get( bestinput, i ) - diff );
                }
            }

            gsl_vector_set( errinitguess, dim, -0.1 );
            gsl_multiroot_fsolver_set( errsolver, &minosfunc, errinitguess );
            mgr::gsl::IterateSolver( errsolver );

            for( size_t i = 0; i < finalinput->size; ++i ){
                gsl_vector_set( finalinput, i, gsl_vector_get( errsolver->x, i ) );
            }

            double boundary2 = varfunction->f( finalinput, varfunction->params );
            // Storing outputs
            min = std::min( boundary1, boundary2 );
            max = std::max( boundary1, boundary2 );
            gsl_vector_free( errinitguess );
            gsl_vector_free( finalinput );
            gsl_multiroot_fsolver_free( errsolver );
            return 0;
        }

        /*******************************************************************************
        *   Common NLL functions
        *******************************************************************************/
        double
        GaussianNLL( double x, void* params )
        {
            double mean  = ( (double*)params )[ 0 ];
            double sigma = ( (double*)params )[ 1 ];
            return ( x - mean ) * ( x - mean ) / ( 2 * sigma * sigma );
        }

        /******************************************************************************/

        double
        BinomialNLL( double x, void* params )
        {
            double passed = ( (double*)params )[ 0 ];
            double total  = ( (double*)params )[ 1 ];
            return -passed* log( x ) - ( total - passed ) * log( 1 - x );
        }

        /******************************************************************************/

        double
        PoissonNLL( double x, void* params )
        {
            double obs = ( (double*)params )[ 0 ];
            return x - obs * log( x );
        }

        /******************************************************************************/
        double
        LinearVarianceNLL( double x, void* params )
        {
            double central = ( (double*)params )[ 0 ];
            double errorup = ( (double*)params )[ 1 ];
            double errorlo = ( (double*)params )[ 2 ];
            return ( x - central ) * ( x - central ) / ( 2 * ( errorup * errorlo + ( errorup - errorlo ) * ( x - central ) ) );
        }
    }/* stat */
}/* mgr */
