/*******************************************************************************
*
*  Filename    : Random.cc
*  Description : Implementation of Random.hpp functions
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "ManagerUtils/Common/interface/Random.hpp"

#include <cstdlib>
#include <string>
#include <sys/time.h>

using namespace std;
namespace mgr{
    /******************************************************************************/

    void
    SeedRandomTime()
    {
        struct timeval time;
        gettimeofday( &time, NULL );
        srand( time.tv_usec );// Initializing to microseconds to avoid name collision
    }

    /******************************************************************************/

    string
    RandomString( const unsigned n )
    {
        static const string alphanum = "0123456789"
                                       "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                       "abcdefghijklmnopqrstuvwxyz";
        string ans = "";
        SeedRandomTime();

        for( unsigned i = 0; i < n; ++i ){
            ans.push_back( alphanum[ rand() % alphanum.length() ] );
        }

        return ans;
    }
}/* mgr */
