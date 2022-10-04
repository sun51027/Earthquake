/*******************************************************************************
*
*  Filename    : PathUtils_Generate.cc
*  Description : Implementation of functions for generating file names
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "ManagerUtils/Common/interface/Random.hpp"
#include <string>
using namespace std;

namespace mgr{
    string
    RandomFileName( const unsigned n, const string& ext, const bool hidden )
    {
        string ans = RandomString( n );

        if( ext != "" ){
            ans += "." + ext;
        }

        if( hidden ){
            ans = "." + ans;
        }

        return ans;
    }
}/* mgr */
