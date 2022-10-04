/*******************************************************************************
*
*  Filename    : xrdls.cc
*  Description : xrootd file system browsing helper tool
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "ManagerUtils/SysUtils/interface/PathUtils.hpp"

#include <iostream>
using namespace std;
int
main( int argc, char* argv[] )
{
    for( int i = 1; i < argc; i++ ){
        // cout << "Globbing: " << argv[i] << endl;
        for( const auto& file : mgr::Glob( argv[ i ] ) ){
            if( mgr::IsRemotePath( file ) ){
                cout << mgr::GetRemotePath( file ) << endl;
            }
            else{
                cout << file << endl;
            }
        }
    }

    return 0;
}
