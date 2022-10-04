/*******************************************************************************
*
*  Filename    : TimeUtils_Boost.cc
*  Description : Creating python library for TimeUtils.hpp
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "ManagerUtils/SysUtils/interface/TimeUtils.hpp"
#include <boost/python.hpp>

using namespace boost::python;

BOOST_PYTHON_MODULE( pluginTimeUtils ){
    def( "CurrentDateTime", mgr::CurrentDateTime );
    def( "SleepMillSec",    mgr::SleepMillSec );
}
