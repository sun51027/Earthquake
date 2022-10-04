/*******************************************************************************
*
*  Filename    : computing_test.cc
*  Description : Unit testing functions for functions defined in Compute.hpp
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "ManagerUtils/Maths/interface/StatisticsUtil.hpp"

#include <iostream>

using namespace std;
int
main( int argc, char* argv[] )
{
    cout << "Confidence level from sigma " << endl
         << mgr::stat::GetConfidenceLevel( 1 ) << endl
         << mgr::stat::GetConfidenceLevel( 2 ) << endl;
    cout << "Sigma value from confidence level" << endl
         << mgr::stat::GetSigmaInterval( mgr::stat::onesigma_level ) << endl
         << mgr::stat::GetSigmaInterval( mgr::stat::twosigma_level ) << endl;
    cout << "Partial Derivative test" << endl;
    mgr::gsl::gsl_multifunc func;
    func.f = [ ]( const gsl_vector* x, void* ){
                 double ans = 0;

                 for( size_t i = 0; i < x->size; ++i ){
                     ans += gsl_vector_get( x, i ) * gsl_vector_get( x, i );
                 }

                 return ans;
             };
    func.n      = 3;
    func.params = nullptr;
    gsl_vector* x = gsl_vector_alloc( 3 );
    gsl_vector_set( x, 0, 1 );
    gsl_vector_set( x, 1, 2 );
    gsl_vector_set( x, 2, 3 );
    cout << func.f( x, nullptr ) << endl;
    cout << mgr::gsl::partial_deriv( &func, x, 0 ) << endl
         << mgr::gsl::partial_deriv( &func, x, 1 ) << endl
         << mgr::gsl::partial_deriv( &func, x, 2 ) << endl;
    cout << "1D Minos Error testing" << endl;
    gsl_function gaussnll;
    gaussnll.function = [ ]( double x, void* p ){
                            double* param = (double*)p;
                            double mean   = param[ 0 ];
                            double sig    = param[ 1 ];
                            return ( x - mean ) * ( x - mean ) / ( 2. * sig * sig );
                        };
    {
        double test[] = { 1, 2 };
        double mean = 0, errup = 10, errlo = -10;
        gaussnll.params = test;
        mgr::stat::MinosError( &gaussnll, mean, errlo, errup );
        cout << mean << " " << errup << " " << errlo << endl;
        cout << gaussnll.function( mean, test ) << endl;
        cout << gaussnll.function( errup, test ) << endl;
        cout << gaussnll.function( errlo, test ) << endl;
    }
    cout << "2D minos error test" << endl;
    mgr::gsl::gsl_multifunc twogaussnll;
    twogaussnll.f = [ ]( const gsl_vector* x, void* param ){
                        double* p    = (double*)param;
                        double mean1 = p[ 0 ];
                        double sig1  = p[ 1 ];
                        double mean2 = p[ 2 ];
                        double sig2  = p[ 3 ];
                        double x1    = gsl_vector_get( x, 0 );
                        double x2    = gsl_vector_get( x, 1 );
                        return ( x1 - mean1 ) * ( x1 - mean1 ) / ( 2. * sig1 * sig1 )
                               + ( x2 - mean2 ) * ( x2 - mean2 ) / ( 2. * sig2 * sig2 );
                    };
    twogaussnll.n = 2;
    double test[] = { 3, 0.5, 5, 1 };
    twogaussnll.params = test;
    mgr::gsl::gsl_multifunc twosum;
    twosum.f = [ ]( const gsl_vector* x, void* ){
                   double ans = 0;

                   for( size_t i = 0; i < x->size; ++i ){
                       ans += gsl_vector_get( x, i );
                   }

                   return ans;
               };
    twosum.n = 2;
    gsl_vector* init = gsl_vector_alloc( 2 );
    gsl_vector_set_all( init, 1 );
    double central, up, down;
    mgr::stat::MinosError( &twogaussnll, &twosum, init, central, up, down );
    cout << central << "  " << up << " " << down << endl;
    {
        cout << "Pre-defined  NLL test" << endl;
        double a           = 1;
        double b           = -100;
        double c           = 100;
        double gausparam[] = { 5, 2 };
        gsl_function test  = { &mgr::stat::GaussianNLL, &gausparam };
        mgr::stat::MinosError( &test, a, b, c );
        cout << a << " " << b << " " << c << endl;
        double binomialparam[] = { 100, 200 };
        a    = 100. / 200.;
        b    = 0.001;
        c    = 0.999;
        test = { &mgr::stat::BinomialNLL, &binomialparam };
        mgr::stat::MinosError( &test, a, b, c );
        cout << a << " " << b << " " << c << endl;
        double poissonparam = 10;
        a    = 10;
        b    = a / 2;
        c    = a * 2;
        test = { &mgr::stat::PoissonNLL, &poissonparam };
        mgr::stat::MinosError( &test, a, b, c );
        cout << a << " " << b << " " << c << endl;
    }
    return 0;
}
