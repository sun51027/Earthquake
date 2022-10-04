/*******************************************************************************
*
*  Filename    : BoostUtils_PTreeUtils_Variadic.ipp
*  Description : Implementation of variadic query functions
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef MANAGERUTILS_COMMON_BOOSTUTILS_PTREEUTILS_VARIADIC_IPP
#define MANAGERUTILS_COMMON_BOOSTUTILS_PTREEUTILS_VARIADIC_IPP

#include "ManagerUtils/Common/interface/BoostUtils/PTreeUtils.hpp"
#include "ManagerUtils/Common/interface/Variadic.hpp"

/******************************************************************************/

template < typename ... TS >
extern std::string
mgr::MakeQueryString( const std::string& x, TS ... args )
{
    return mgr::MakeQueryString( mgr::MakeVector < std::string > ( x, args ... ) );
}

/******************************************************************************/

template < typename ... TS >
extern bool
mgr::CheckQuery( const boost::property_tree::ptree& tree, const std::string& x, TS ... args )
{
    return mgr::CheckQuery( tree, mgr::MakeQueryString( x, args ... ) );
}

/******************************************************************************/

template < typename T, typename ... TS >
extern T
mgr::GetSingle( const boost::property_tree::ptree& tree, const std::string& x, TS ... args )
{
    return mgr::GetSingle < T > ( tree, mgr::MakeQueryString( x, args ... ) );
}

/******************************************************************************/
template < typename T, typename ... TS >
extern std::vector < T >
mgr::GetList( const boost::property_tree::ptree& tree, const std::string& x, TS ... args )
{
    return mgr::GetList < T > ( tree, mgr::MakeQueryString( x, args ... ) );
}

/******************************************************************************/

#endif/* end of include guard: MANAGERUTILS_COMMON_BOOSTUTILS_PTREEUTILS_VARIADIC_IPP */
