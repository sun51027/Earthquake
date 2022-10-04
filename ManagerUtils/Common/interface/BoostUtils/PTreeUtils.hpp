/*******************************************************************************
*
*  Filename    : PTreeUtils.hpp
*  Description : Helper functions for PTree query and traversing
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef MANAGERUTILS_COMMON_BOOSTUTILS_PTREEUTILS_HPP
#define MANAGERUTILS_COMMON_BOOSTUTILS_PTREEUTILS_HPP

#include "ManagerUtils/Common/interface/Variadic.hpp"
#include "ManagerUtils/Maths/interface/Parameter/Parameter.hpp"
#include <boost/property_tree/ptree.hpp>
#include <string>
#include <vector>

namespace mgr {

/*******************************************************************************
*   Making ptree from files
*******************************************************************************/
extern boost::property_tree::ptree FromJsonFile( const std::string& );

/*******************************************************************************
*   Dumping the contents of a property tree
*******************************************************************************/
extern void PrintPTree( const boost::property_tree::ptree&, unsigned level = 0 );

/*******************************************************************************
*   General interfacing functions ( for template, see BoostUtils_PTreeUtils.ipp)
*
*   * MakeQueryString : Contract vector of strings {a,b,c} to "a.b.c"
*
*   * CheckQuery : Check if the tree has the the value at "<query>"
*
*   * GetSingle  : Extract the value at "<query>"
*
*   * GetList    : Getting the list at "<query>"
*
*******************************************************************************/
extern std::string MakeQueryString( const std::vector<std::string>& );

extern bool CheckQuery( const boost::property_tree::ptree&, const std::string& );

template<typename T>
extern T GetSingle( const boost::property_tree::ptree&, const std::string& );

template<> // Template specialization
Parameter GetSingle<Parameter>( const boost::property_tree::ptree&, const std::string& );

template<typename T>
extern std::vector<T> GetList( const boost::property_tree::ptree&, const std::string& );


/*******************************************************************************
*   Variadic query interface ( see BoostUtils_PTreeUtils_Variadic.ipp)
*******************************************************************************/
template<typename ... TS>
extern std::string MakeQueryString( const std::string& x, TS ... args );

template<typename ... TS>
extern bool CheckQuery( const boost::property_tree::ptree&, const std::string&, TS ... args );

template<typename T, typename ... TS>
extern T GetSingle( const boost::property_tree::ptree&, const std::string&, TS ... args );

template<typename T, typename ... TS>
extern std::vector<T> GetList( const boost::property_tree::ptree&, const std::string&, TS ... args );

}/* mgr*/

#include "ManagerUtils/Common/src/BoostUtils_PTreeUtils.ipp"
#include "ManagerUtils/Common/src/BoostUtils_PTreeUtils_Variadic.ipp"

#endif/* end of include guard: MANAGERUTILS_COMMON_BOOSTUTILS_PTREEUTILS_HPP */
