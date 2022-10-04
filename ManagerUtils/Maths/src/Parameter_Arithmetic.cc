/*******************************************************************************
*
*  Filename    : ParameterArithmetic.cc
*  Description : One Line descrption of file contents
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "ManagerUtils/Maths/interface/Parameter.hpp"

#include "ManagerUtils/Maths/interface/GSLUtil.hpp"
#include "ManagerUtils/Maths/interface/StatisticsUtil.hpp"

#include <gsl/gsl_vector.h>
#include <iostream>
#include <vector>

using namespace std;

namespace mgr{
    /*******************************************************************************
    *   General Interfacing functions with MakeMinos functions
    *******************************************************************************/
    Parameter
    MakeMinos(
        gsl_function* nll,
        const double  initguess,
        const double  min,
        const double  max,
        const double  confidencelevel
        )
    {
        double central    = initguess;
        double lowerbound = min;
        double upperbound = max;
        mgr::stat::MinosError( nll, central, lowerbound, upperbound, confidencelevel );
        return Parameter( central, upperbound - central, central - lowerbound );
    }

    /******************************************************************************/

    Parameter
    MakeMinos(
        mgr::gsl::gsl_multifunc* nll,
        mgr::gsl::gsl_multifunc* varfunction,
        gsl_vector*              initguess,
        const double             confidencelevel,
        gsl_vector*              upperguess,
        gsl_vector*              lowerguess
        )
    {
        double central;
        double upperbound;
        double lowerbound;
        mgr::stat::MinosError(
            nll,
            varfunction,
            initguess,
            central,
            lowerbound,
            upperbound,
            confidencelevel,
            upperguess,
            lowerguess
            );
        return Parameter( central, upperbound - central, central - lowerbound );
    }

    /*******************************************************************************
    *   Approximate Loglikelihood functions and first derivative.
    *******************************************************************************/
    double
    LinearVarianceNLL( double x, const Parameter& parm )
    {
        static const double maxrelerror = sqrt( 10. );
        static const double minprod     = sqrt( mgr::gsl::epsilon );
        // Getting values
        const double central = parm.CentralValue();
        // Relative error must not be smaller than set value,
        // scaling up effective error otherwise
        const double effupper = std::max( parm.AbsUpperError(), parm.AbsLowerError() / maxrelerror );
        const double efflower = std::max( parm.AbsLowerError(), parm.AbsUpperError() / maxrelerror );
        // product of errors must be a small finite value
        const double sigsq   = std::max( effupper * efflower, minprod );
        const double sigdiff = effupper - efflower;
        const double relasym = std::min( sigdiff / sigsq, 3.2 );
        // Original definition as followed
        const double num = ( x - central ) * ( x - central );
        const double de  = sigsq * ( 1 + ( x - central ) * relasym );
        const double ans = 0.5 * num / de;
        return ans;
    }

    /*******************************************************************************
    *   Hidden method for Making a master uncorrelated member functions
    *******************************************************************************/
    struct uncorrelatedparam
    {
        const vector<Parameter>* listptr;
        double                   ( * nll )( double, const Parameter& );
    };

    static double
    UncorrelatedNLL( const gsl_vector* x, void* param )
    {
        uncorrelatedparam* p = (uncorrelatedparam*)param;
        double ans           = 0;

        for( size_t i = 0; i < p->listptr->size(); ++i ){
            const Parameter& thisp = p->listptr->at( i );
            const double thisx     = gsl_vector_get( x, i );
            ans += p->nll( thisx, thisp );
        }

        return ans;
    }

    /*******************************************************************************
    *   Main function for interfacing with user
    *******************************************************************************/
    Parameter
    SumUncorrelated(
        const vector<Parameter>& paramlist,
        const double confidencelevel,
        double ( * nll )( double, const Parameter& )
        )
    {
        const unsigned dim = paramlist.size();
        mgr::gsl::gsl_multifunc masternll;
        uncorrelatedparam param = { &paramlist, nll };
        masternll.f      = &UncorrelatedNLL;
        masternll.n      = dim;
        masternll.params = &param;
        mgr::gsl::gsl_multifunc varfunction;
        varfunction.f      = &mgr::gsl::sum;
        varfunction.n      = dim;
        varfunction.params = nullptr;// parameter-less
        gsl_vector* initguess  = gsl_vector_alloc( dim );
        gsl_vector* upperguess = gsl_vector_alloc( dim );
        gsl_vector* lowerguess = gsl_vector_alloc( dim );

        for( size_t i = 0; i < paramlist.size(); ++i ){
            const Parameter& p = paramlist.at( i );
            gsl_vector_set( initguess,  i, p.CentralValue() );
            gsl_vector_set( upperguess, i, p.CentralValue() + p.AbsUpperError() / 1.5 );
            gsl_vector_set( lowerguess, i, p.CentralValue() - p.AbsLowerError() / 1.5 );
        }

        // Calling MinosError function for acutal computation
        Parameter ans = MakeMinos(
            &masternll,
            &varfunction,
            initguess,
            confidencelevel,
            upperguess,
            lowerguess
            );
        // Releaing gsl vector calls
        gsl_vector_free( initguess );
        gsl_vector_free( upperguess );
        gsl_vector_free( lowerguess );
        return ans;
    }

    /******************************************************************************/

    Parameter
    ProdUncorrelated(
        const std::vector<Parameter>& paramlist,
        const double confidencelevel,
        double ( * nll )( double, const Parameter& )
        )
    {
        const unsigned dim = paramlist.size();
        // Getting normalized version of list
        double prod = 1.;
        vector<Parameter> normlist;

        for( const auto& p : paramlist ){
            prod *= p.CentralValue();
            normlist.push_back( p.NormParam() );
        }

        mgr::gsl::gsl_multifunc masternll;
        uncorrelatedparam param = { &normlist, nll };
        masternll.f      = &UncorrelatedNLL;
        masternll.n      = dim;
        masternll.params = &param;
        mgr::gsl::gsl_multifunc varfunction;
        varfunction.f      = &mgr::gsl::product;
        varfunction.n      = dim;
        varfunction.params = nullptr;// parameter-less
        gsl_vector* initguess  = gsl_vector_alloc( dim );
        gsl_vector* upperguess = gsl_vector_alloc( dim );
        gsl_vector* lowerguess = gsl_vector_alloc( dim );

        for( size_t i = 0; i < normlist.size(); ++i ){
            const Parameter& p = normlist.at( i );
            gsl_vector_set( initguess,  i, p.CentralValue() );
            gsl_vector_set( upperguess, i, p.CentralValue() + p.AbsUpperError() / 1.2 );
            gsl_vector_set( lowerguess, i, p.CentralValue() - p.AbsLowerError() / 1.2 );
        }

        // Calling MinosError function for acutal computation
        Parameter ans = MakeMinos(
            &masternll,
            &varfunction,
            initguess,
            confidencelevel,
            upperguess,
            lowerguess
            );
        // Releaing gsl vector calls
        gsl_vector_free( initguess );
        gsl_vector_free( upperguess );
        gsl_vector_free( lowerguess );
        return ans * prod;
    }
}/* mgr */
