/*******************************************************************************
*
*  Filename    : PathUtils_Boost.cc
*  Description : Exposing PathUtils functions to python for uniform interface
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "ManagerUtils/SysUtils/interface/PathUtils.hpp"
#include <boost/python.hpp>

using namespace boost::python;

BOOST_PYTHON_MODULE( pluginPathUtils ){
    def( "GetEnv",           mgr::GetEnv );
    def( "ConvertToAbsPath", mgr::ConvertToAbsPath );
    def( "CMSSWSrc",         mgr::CMSSWSrc );
}
