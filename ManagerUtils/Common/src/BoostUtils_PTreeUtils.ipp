/*******************************************************************************
*
*  Filename    : BoostUtils_PTreeUtils.ipp
*  Description : Implementation of variadic interfaces
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*   Additional comments
*
*******************************************************************************/
#ifndef MANAGERUTILS_COMMON_BOOSTUTILS_PTREEUTILS_IPP
#define MANAGERUTILS_COMMON_BOOSTUTILS_PTREEUTILS_IPP

#include "ManagerUtils/Common/interface/BoostUtils/PTreeUtils.hpp"
#include <iostream>

/******************************************************************************/

template < typename T >
extern T
mgr::GetSingle( const boost::property_tree::ptree& tree, const std::string& query )
{
    return tree.get < T > ( query );
}

/******************************************************************************/

template < typename T >
extern std::vector < T >
mgr::GetList( const boost::property_tree::ptree& tree, const std::string& query )
{
    std::vector < T > ans;

    // http://stackoverflow.com/questions/23481262/using-boost-property-tree-to-read-int-array
    for( auto& v : tree.get_child( query ) ){
        if( !v.first.empty() ){
            std::cerr << "Warning! Skipping over illegal format at branch: (" << query
                      << ")  with index value: (" << v.first.data() << ")" << std::endl;
            continue;
        }
        ans.push_back( v.second.get_value < T > ( ) );
    }

    return ans;
}

#endif/* end of include guard: MANAGERUTILS_COMMON_BOOSTUTILS_PTREEUTILS_IPP */
