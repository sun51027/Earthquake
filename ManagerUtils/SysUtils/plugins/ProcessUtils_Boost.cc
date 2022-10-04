/*******************************************************************************
*
*  Filename    : ProcessUtils_Boost.cc
*  Description : Exposing Process utility functions to python
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "ManagerUtils/SysUtils/interface/ProcessUtils.hpp"
#include <boost/python.hpp>

using namespace std;

/*******************************************************************************
*   Inline function to help with function overloading in python
*******************************************************************************/
inline unsigned
HasProcess_1( const std::string& x )
{
    return mgr::HasProcess( x, "" );
}

/******************************************************************************/

inline void
WaitProcess_2( const string& x, const string& exclude )
{
    return mgr::WaitProcess( x, exclude, 100 );
}

/******************************************************************************/

inline void
WaitProcess_1( const string& x )
{
    return WaitProcess_2( x, "" );
}

/*******************************************************************************
*   boost::python function calls
*******************************************************************************/

using namespace boost::python;

BOOST_PYTHON_MODULE( pluginProcessUtils ){
    def( "NumOfThreads", mgr::NumOfThreads );
    def( "HasProcess",   mgr::HasProcess );
    def( "HasProcess",   HasProcess_1 );
    def( "WaitProcess",  mgr::WaitProcess );
    def( "WaitProcess",  WaitProcess_1 );
    def( "WaitProcess",  WaitProcess_2 );
}
