/*******************************************************************************
*
*  Filename    : PathUtils.cc
*  Description : Implementations of functions
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "ManagerUtils/Common/interface/STLUtils.hpp"

#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
#include <unistd.h>
using namespace std;
namespace fs = boost::filesystem;


/*******************************************************************************
*   Operator overloading
*******************************************************************************/

namespace mgr{
    /******************************************************************************/

    string
    GetEnv( const std::string& x )
    {
        return getenv( x.c_str() );
    }

    /******************************************************************************/

    string
    ConvertToAbsPath( const std::string& path )
    {
        return fs::absolute( path ).string();
    }

    /******************************************************************************/

    string
    Basename( const std::string& path )
    {
        return fs::path( path ).filename().string();
    }

    /******************************************************************************/

    string
    CMSSWSrc()
    {
        return GetEnv( "CMSSW_BASE" ) / "src";
    }

    /******************************************************************************/

    string
    PackagePath( const std::string& x )
    {
        return CMSSWSrc() / x;
    }

    /******************************************************************************/

    string
    SubPackagePath( const std::string& x, const std::string& y )
    {
        return PackagePath( x ) / y;
    }

    /******************************************************************************/

    bool
    CheckPath( const std::string& filename )
    {
        fs::path parent = fs::path( filename ).parent_path();

        if( fs::is_directory( parent ) ){
            return true;// Early exit if already exists
        }

        if( fs::create_directory( parent ) ){
            cout << "New directory " << parent << " created!" << endl;
            return true;
        }
        else{
            cout << "Failed making new directory " << parent << "!" << endl;
            return false;
        }
    }
}/* mgr */
