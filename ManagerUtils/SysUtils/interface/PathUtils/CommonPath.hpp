/*******************************************************************************
*
*  Filename    : CommonPath.hpp
*  Description : Functions for accessing common paths
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef MANAGERUTILS_SYSUTILS_PATHUTILS_COMMONPATH_HPP
#define MANAGERUTILS_SYSUTILS_PATHUTILS_COMMONPATH_HPP

#include <string>

/*******************************************************************************
*   Operator / for strings
*   Boost filesystem flavour path appendage
*******************************************************************************/

namespace mgr {
/*******************************************************************************
*   * GetEnv
*   Returning a system environment variable, C++ string as input and output
*******************************************************************************/
extern std::string GetEnv( const std::string& );

/*******************************************************************************
*   * ConvertToAbsPath
*   Chaing a relative path to an absolute one
*******************************************************************************/
extern std::string ConvertToAbsPath( const std::string& );

/*******************************************************************************
*   * Basename
*   * Stripping path to basename
*******************************************************************************/
extern std::string Basename( const std::string& );

/*******************************************************************************
*   ** CMSSWSrc
*   Returns the absolute path of the CMSSSW_X_Y_Z/src/ directory of this program
*******************************************************************************/
extern std::string CMSSWSrc();

/*******************************************************************************
*   ** GetPackagePath
*   Return tha absolute path to a package in CMSSW_X_Y_Z/src/Package
*******************************************************************************/
extern std::string PackagePath( const std::string& x );

/*******************************************************************************
*   ** GetSubPackagePath
*   Returns the absolute path ta package in CMSSW_X_Y_Z/src/Package/SubPackage
*******************************************************************************/
extern std::string SubPackagePath( const std::string& x, const std::string& y );

/*******************************************************************************
*   ** CheckPath
*   Check if parent directory of file exists, create if doesn't
*******************************************************************************/
extern bool CheckPath( const std::string& filename );


}/* mgr */

#endif/* end of include guard:  */
