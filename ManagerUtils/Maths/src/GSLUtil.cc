/*******************************************************************************
*
*  Filename    : GSLUtil.cc
*  Description : Implementation of GSL enhancing functions
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "ManagerUtils/Maths/interface/GSLUtil.hpp"

#include <gsl/gsl_deriv.h>
#include <iostream>

namespace mgr{
    namespace gsl{
        /*******************************************************************************
        *   GSL settings constants
        *******************************************************************************/
        extern const double epsilon         = 1e-6;
        extern const unsigned max_iteration = 1e+3;

        /*******************************************************************************
        *   GSL iterator solver
        *******************************************************************************/
        void
        IterateSolver( gsl_multiroot_fsolver* solver )
        {
            int status;
            size_t iteration = 0;

            do {
                iteration++;
                status = gsl_multiroot_fsolver_iterate( solver );
                // if( status ){ break; }
                status = gsl_multiroot_test_residual( solver->f, epsilon );
            } while( status == GSL_CONTINUE && iteration < max_iteration * solver->f->size );
        }

        /******************************************************************************/

        void
        IterateSolver( gsl_root_fsolver* solver )
        {
            int status;
            size_t iter = 0;
            double lo;
            double hi;

            do {
                iter++;
                status = gsl_root_fsolver_iterate( solver );
                lo     = gsl_root_fsolver_x_lower( solver );
                hi     = gsl_root_fsolver_x_upper( solver );
                status = gsl_root_test_interval( lo, hi, 0, epsilon );

                if( status == GSL_SUCCESS ){
                    break;
                }
            } while( status == GSL_CONTINUE && iter < max_iteration );
        }

        /******************************************************************************/

        void
        IterateSolver( gsl_min_fminimizer* solver )
        {
            int status;
            size_t iter = 0;
            double lo;
            double hi;

            do {
                iter++;
                status = gsl_min_fminimizer_iterate( solver );
                lo     = gsl_min_fminimizer_x_lower( solver );
                hi     = gsl_min_fminimizer_x_upper( solver );
                status = gsl_min_test_interval( lo, hi, epsilon, 0 );
            } while( status == GSL_CONTINUE && iter < max_iteration );
        }

        /******************************************************************************/

        void
        IterateSolver( gsl_multimin_fminimizer* solver )
        {
            int status;
            unsigned iter = 0;
            double size;// size of stepping vector

            do {
                iter++;
                status = gsl_multimin_fminimizer_iterate( solver );

                if( status ){
                    break;
                }

                size   = gsl_multimin_fminimizer_size( solver );
                status = gsl_multimin_test_size( size, epsilon );
            } while( status == GSL_CONTINUE && iter < max_iteration * solver->x->size );
        }

        /*******************************************************************************
        *   1D function solving at y = dedicated value
        *******************************************************************************/
        double
        Solve1D(
            gsl_function* func,
            double        yval,
            double        xmin,
            double        xmax
            )
        {
            struct argparam
            {
                gsl_function* original_func;
                double        yval;
            };
            argparam newparams = { func, yval };
            gsl_function augfunc;
            augfunc.function = [ ]( double x, void* param ){
                                   argparam* p = (argparam*)param;
                                   return p->original_func->function( x, p->original_func->params ) - p->yval;
                               };
            augfunc.params = &newparams;
            gsl_root_fsolver* solver = gsl_root_fsolver_alloc( gsl_root_fsolver_brent );
            gsl_root_fsolver_set( solver, &augfunc, xmin, xmax );
            IterateSolver( solver );
            double ans = gsl_root_fsolver_root( solver );
            gsl_root_fsolver_free( solver );
            return ans;
        }

        /*******************************************************************************
        *   Handy parameter-less gsl_multifunc
        *******************************************************************************/
        double
        sum( const gsl_vector* x, void* )
        {
            double ans = 0;

            for( size_t i = 0; i < x->size; ++i ){
                ans += gsl_vector_get( x, i );
            }

            return ans;
        }

        /******************************************************************************/

        double
        product( const gsl_vector* x, void* )
        {
            double ans = 1;

            for( size_t i = 0; i < x->size; ++i ){
                ans *= gsl_vector_get( x, i );
            }

            return ans;
        }

        /*******************************************************************************
        *   Multidimension helper functions
        *******************************************************************************/
        double
        projected_function( double x, void* p )
        {
            projected_param* param = (projected_param*)p;
            gsl_vector* newvec     = gsl_vector_alloc( param->original_x->size );
            gsl_vector_memcpy( newvec, param->original_x );
            gsl_vector_set( newvec, param->varidx, x );
            double ans = param->original_func->f( newvec, param->original_func->params );
            gsl_vector_free( newvec );
            return ans;
        }

        /*******************************************************************************
        *   Partial derivative function
        *******************************************************************************/
        int
        partial_deriv(
            gsl_multifunc* function,
            gsl_vector*    x,
            size_t         varidx,
            double         h,
            double&        result,
            double&        error
            )
        {
            gsl_function f;
            projected_param p = { function, x, varidx };
            f.function = &projected_function;
            f.params   = &p;
            double projx = gsl_vector_get( x, varidx );
            int retans   = gsl_deriv_central( &f, projx, h, &result, &error );
            return retans;
        }

        /******************************************************************************/
        // Simpler interfacting functions
        double
        partial_deriv(
            gsl_multifunc* function,
            gsl_vector*    x,
            size_t         varidx,
            double         h
            )
        {
            double result, error;
            partial_deriv( function, x, varidx, h, result, error );
            return result;
        }

        /******************************************************************************/

        double
        partial_deriv_error(
            gsl_multifunc* function,
            gsl_vector*    x,
            size_t         varidx,
            double         h
            )
        {
            double result, error;
            partial_deriv( function, x, varidx, h, result, error );
            return error;
        }
    };/* namespace gsl  */
};/* namespace mgr */
