/*******************************************************************************
*
*  Filename    : parameter_format.cc
*  Description : Unit testing for parameter formatting functions
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "ManagerUtils/Maths/interface/Parameter.hpp"

#include <iostream>
#include <regex>

using namespace std;
using namespace mgr;

int
main( int argc, char const* argv[] )
{
    cout << ">>> Regex testing" << endl;
    const std::string test = "123456789123456.123456789";
    const std::regex reg( "(.*\\d)(\\d{3})" );
    std::smatch mth;
    std::regex_match( test, mth, reg );

    for( const auto& sub : mth ){
        cout << sub << " ";
    }

    cout << endl;
    cout << ">>> General floating point testing" << endl;

    for( int i = -1; i < 11; ++i ){
        cout << FloatingPoint( 12345679123456, i ) << endl;
    }

    for( int i = -1; i < 11; ++i ){
        cout << FloatingPoint( 12345679.123456, i ) << endl;
    }

    cout << ">>> Small Parameter testing" << endl;
    Parameter x( 123, 23, 0.01 );

    for( int i = -1; i < 11; ++i ){
        cout << FloatingPoint( x, i ) << " | " << Scientific( x, i ) << endl;
    }

    return 0;
}
