/*******************************************************************************
*
*  Filename    : STLUtils_StringUtils.cc
*  Description : Implementation of string enhacing utilities
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "ManagerUtils/Common/interface/STLUtils/StringUtils.hpp"

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;


/******************************************************************************/

std::string
operator/( const std::string& x, const std::string& y )
{
    return ( fs::path( x ) / y ).string();
}

/******************************************************************************/

std::string
operator*( const std::string& x, const std::string& y )
{
    return x + "." + y;
}
