/*******************************************************************************
*
*  Filename    : TimeUtils.cc
*  Description : Implementation of time related system functions
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*
*******************************************************************************/
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <thread>

using namespace std;

namespace mgr{
    /******************************************************************************/

    string
    CurrentDateTime()
    {
        // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
        // for more information about date/time format
        time_t now = time( 0 );
        struct tm tstruct;
        char buf[ 80 ];
        tstruct = *localtime( &now );
        strftime( buf, sizeof( buf ), "%Y-%m-%d|%X", &tstruct );
        return buf;
    }

    /******************************************************************************/

    void
    SleepMillSec( const unsigned x )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( x ) );
    }
}/* mgr */
