/*******************************************************************************
*
*  Filename    : pointer_test.cc
*  Description : Parameter* to double*  value test
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "ManagerUtils/Maths/interface/Parameter.hpp"
#include "ManagerUtils/Maths/interface/StatisticsUtil.hpp"

#include <iostream>

using namespace std;
int
main( int argc, char* argv[] )
{
    mgr::Parameter x( 2, 1, 0.5 );
    cout << mgr::stat::LinearVarianceNLL( 1, &x ) << endl;
    cout << mgr::stat::LinearVarianceNLL( 1.5, &x ) << endl;
    cout << mgr::stat::LinearVarianceNLL( 2.0, &x ) << endl;
    cout << mgr::stat::LinearVarianceNLL( 2.5, &x ) << endl;
    cout << mgr::stat::LinearVarianceNLL( 3.0, &x ) << endl;
    double y[] = { 2, 1, 0.5 };
    cout << mgr::stat::LinearVarianceNLL( 1, y ) << endl;
    cout << mgr::stat::LinearVarianceNLL( 1.5, y ) << endl;
    cout << mgr::stat::LinearVarianceNLL( 2.0, y ) << endl;
    cout << mgr::stat::LinearVarianceNLL( 2.5, y ) << endl;
    cout << mgr::stat::LinearVarianceNLL( 3.0, y ) << endl;
    return 0;
}
