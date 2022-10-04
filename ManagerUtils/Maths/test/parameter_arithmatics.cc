/*******************************************************************************
*
*  Filename    : parameter_arithmatics.cc
*  Description : Testing the arithmatics of paramters
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "ManagerUtils/Maths/interface/Parameter.hpp"
#include <iostream>
#include <vector>

#include <boost/format.hpp>
using namespace std;
using namespace mgr;

void
AddTest( const vector<Parameter>& list )
{
    for( size_t i = 0; i < list.size(); ++i ){
        cout << list[ i ].CentralValue();

        if( i != list.size() - 1 ){
            cout << "+";
        }
        else{
            cout << " & ";
        }
    }

    const Parameter sum = SumUncorrelated( list );
    cout << sum.CentralValue() << "&" << sum.AbsUpperError() << "&" << sum.AbsLowerError() << endl;
}

void
ProdTest( const vector<double> chain )
{
    Parameter original  = Poisson::Minos( chain.front() );
    Parameter finalpass = Poisson::Minos( chain.back() );
    vector<Parameter> prodlist;
    prodlist.push_back( original );
    cout << chain.front() << flush;

    for( size_t i = 0; i < chain.size() - 1; ++i ){
        cout << boost::format( "\\times\\frac{%lg}{%lg}" ) % chain[ i + 1 ] % chain[ i ] << flush;
        prodlist.push_back( Efficiency::Minos( chain[ i + 1 ], chain[ i ] ) );
    }

    cout << "&" << flush;
    const Parameter product = ProdUncorrelated( prodlist );
    cout << product.CentralValue() << "&" << product.AbsUpperError() << "&" << product.AbsLowerError() << endl;
}

int
main( int argc, char const* argv[] )
{
    cout << ">>>> Addition testing: Sum to 100" << endl;
    {
        Parameter a = Poisson::Minos( 10 );
        Parameter b = Poisson::Minos( 20 );
        Parameter c = Poisson::Minos( 30 );
        Parameter d = Poisson::Minos( 40 );
        Parameter e = Poisson::Minos( 50 );
        Parameter f = Poisson::Minos( 60 );
        Parameter g = Poisson::Minos( 70 );
        Parameter h = Poisson::Minos( 80 );
        Parameter i = Poisson::Minos( 90 );
        cout << "Direct value: " << FloatingPoint( Poisson::Minos( 100 ), 4 ) << endl;
        AddTest( { e, e } );
        AddTest( { d, f } );
        AddTest( { c, g } );
        AddTest( { b, h } );
        AddTest( { a, i } );
        AddTest( { a, c, c, c } );
        AddTest( { b, b, b, b, b } );
        AddTest( { a, a, a, a, a, a, a, a, a, a } );
        cout << endl;
    }
    cout << ">>>> Addition testing: Sum to 1000" << endl;
    {
        Parameter a = Poisson::Minos( 100 );
        Parameter b = Poisson::Minos( 200 );
        Parameter c = Poisson::Minos( 300 );
        Parameter d = Poisson::Minos( 400 );
        Parameter e = Poisson::Minos( 500 );
        Parameter f = Poisson::Minos( 600 );
        Parameter g = Poisson::Minos( 700 );
        Parameter h = Poisson::Minos( 800 );
        Parameter i = Poisson::Minos( 900 );
        cout << "Direct value: " << FloatingPoint( Poisson::Minos( 1000 ), 4 ) << endl;
        AddTest( { e, e } );
        AddTest( { d, f } );
        AddTest( { c, g } );
        AddTest( { b, h } );
        AddTest( { a, i } );
        AddTest( { a, c, c, c } );
        AddTest( { b, b, b, b, b } );
        AddTest( { a, a, a, a, a, a, a, a, a, a } );
        cout << endl;
    }
    cout << ">>>> Addition testing: Partial addition to 100" << endl;
    {
        Parameter a = Poisson::Minos( 20 );
        cout << "Direct value: " << FloatingPoint( Poisson::Minos( 100 ), 4 ) << endl;
        cout << "(20+20+20+20+20) & " << FloatingPoint( Sum( a, a, a, a, a ), 4 ) << endl;
        cout << "(20+20)+(20+20+20) & " << FloatingPoint( Sum( Sum( a, a ), Sum( a, a, a ) ), 4 ) << endl;
        cout << "(20+20)+(20+20)+20 & " << FloatingPoint( Sum( Sum( a, a ), Sum( a, a ), a ), 4 ) << endl;
        cout << "(((20+20)+20)+20)+20 )& " << FloatingPoint( Sum( Sum( Sum( Sum( a, a ), a ), a ), a ), 4 ) << endl;
        cout << endl;
    }
    cout << ">>>> Product testing: Product to 50" << endl;
    {
        cout << "Direct value : " << FloatingPoint( Poisson::Minos( 50 ), 4 ) << endl;
        ProdTest( { 100, 50 } );
        ProdTest( { 100, 75, 50 } );
        ProdTest( { 100, 75, 60, 50 } );
        ProdTest( { 200, 100, 75, 60, 50 } );
        cout << endl;
    }
    cout << ">>> Product test: Partial product " << endl;
    {
        Parameter a  = Poisson::Minos( 200 );
        Parameter e1 = Efficiency::Minos( 100, 200 );
        Parameter e2 = Efficiency::Minos( 75, 100 );
        Parameter e3 = Efficiency::Minos( 60, 75 );
        Parameter e4 = Efficiency::Minos( 50, 60 );
        cout << "Direct value: " << FloatingPoint( Poisson::Minos( 50 ), 4 ) << endl;
        cout << "(a*b*c*d*e)" << FloatingPoint( Prod( a, e1, e2, e3, e4 ), 4 ) << endl;
        cout << "(a*b)*(c*d*e)" << FloatingPoint( Prod( Prod( a, e1 ), Prod( e2, e3, e4 ) ), 4 ) << endl;
        cout << "(a*b)*(c*d)*e" << FloatingPoint( Prod( Prod( a, e1 ), Prod( e2, e3 ), e4 ), 4 ) << endl;
        cout << "((((a*b)*c)*d)*e)" << FloatingPoint( Prod( Prod( Prod( Prod( a, e1 ), e2 ), e3 ), e4 ), 4 ) << endl;
    }
    cout << "\n>>> Another product test: Scale factor tests" << endl;
    {
        Parameter s( 1, 0, 0 );
        Parameter a( 0.9867, 0.0060, 0.0060 );
        Parameter b( 0.9791, 0.0132, 0.0132 );
        Parameter c( 0.9384, 0.0022, 0.0023 );
        cout << FloatingPoint( a, 3 ) << endl;
        cout << FloatingPoint( b, 3 ) << endl;
        cout << FloatingPoint( c, 3 ) << endl;
        cout << FloatingPoint( Prod( a, b, c ), 3 ) << endl;
        cout << FloatingPoint( s *= a, 3 ) << endl;
        cout << FloatingPoint( s *= b, 3 ) << endl;
        cout << FloatingPoint( s *= c, 3 ) << endl;
    }
    cout << "\n>> Specical case testing " << endl;
    {
        Parameter a( 11394.14521, +3455.26185, +2904.08486 );
        Parameter b( 1.65585, +1.88848, +0.83634 );
        cout << FloatingPoint( a, 5 ) << endl;
        cout << FloatingPoint( b, 5 ) << endl;
        a += b;
        cout << FloatingPoint( a, 5 ) << endl;
    }
    cout << "\n>>> Scale case testing" << endl;
    {
        Parameter a( 0, 0, 0 );
        Parameter b = Poisson::Minos( 10 );
        cout << FloatingPoint( b, 5 ) << endl;
        cout << FloatingPoint( a + b, 5 ) << endl;
        cout << FloatingPoint( b + a, 5 ) << endl;
    }
    cout << "\n>>> Scale product testing" << endl;
    {
        Parameter a( 1, 0.001, 0.2 );
        Parameter b( 100, 0.1, 0.2 );
        Parameter c( 1000, 100, 200 );
        cout << FloatingPoint( Prod( a, b ), 3 ) << endl;
        cout << FloatingPoint( Prod( b, c ), 3 ) << endl;
        // cout << FloatingPoint( Prod(c,b), 3 ) << endl;
        // cout << FloatingPoint( Prod(a,b,c), 3 ) << endl;
    }
    cout << "\n>>> Product test " << endl;
    {
        Parameter results = Prod(
            Parameter( 1, 0.0107266, 0.0107266 ),
            Parameter( 1, 0.0329748, 0.0279871 ),
            Parameter( 1,         0,  0.170623 ),
            Parameter( 1,  0.133084,   0.13232 ),
            Parameter( 1, 0.0243757, 0.0240791 ),
            Parameter( 1, 0.0257803, 0.0254051 ),
            Parameter( 1, 0.0249153, 0.0249153 ),
            Parameter( 1,  0.163553,  0.163553 ),
            Parameter( 1, 0.0547323, 0.0542217 ),
            Parameter( 1,     0.046,     0.046 ),
            Parameter( 1,      0.03,      0.03 )
            );
        cout << FloatingPoint( results, 3 ) << endl;
    }
    cout << "\n>>> Product test" << endl;
    {
        Parameter a( 1, 1.46, 0.787 );
        Parameter b( 1, 0.046, 0.046 );
        Parameter c( 1, 0.03, 0.03 );
        cout << FloatingPoint( Prod( a, b, c ), 5 ) << endl;
        cout << FloatingPoint( Prod( a, b ), 5 ) << endl;
        cout << FloatingPoint( Prod( b, c ), 5 ) << endl;
        cout << FloatingPoint( Prod( a, c ), 5 ) << endl;
    }
    return 0;
}
