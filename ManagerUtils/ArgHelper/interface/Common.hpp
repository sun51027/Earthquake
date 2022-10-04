#ifndef COMMON_HPP
#define COMMON_HPP

// http://stackoverflow.com/questions/1433204/how-do-i-use-extern-to-share-variables-between-source-files-in-c

#include "ManagerUtils/Common/interface/STLUtils/StringUtils.hpp"
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <string>
#include <vector>

/*******************************************************************************
*   Operator overloading, defined in src/Common.cc
*******************************************************************************/


/*******************************************************************************
*   Global variable, defined in src/Common.cc
*******************************************************************************/

namespace mgr{
    /*******************************************************************************
    *   Common variable, defined in src/Common.cc
    *******************************************************************************/

    /*******************************************************************************
    *   Common function, defined in src/Common.cc
    *******************************************************************************/
    extern void        ReadFile( const std::string&, boost::property_tree::ptree& );
    extern std::string SubPackagePath( const std::string&, const std::string& );
    extern std::string PackagePath( const std::string& );

    /*******************************************************************************
    *   Common template function, defined in src/Common.ipp
    *******************************************************************************/
    template<typename T>
    extern T GetSingle( const std::string&, const boost::property_tree::ptree& );

    template<typename T>
    extern std::vector<T> GetList( const std::string&, const boost::property_tree::ptree& );
}

#include "ManagerUtils/ArgHelper/src/Common.ipp"

#endif
