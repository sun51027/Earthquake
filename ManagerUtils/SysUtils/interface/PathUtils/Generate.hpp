/*******************************************************************************
*
*  Filename    : Generate.hpp
*  Description : Generating File names and paths
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*  Main implementation in src/PathUtils_Generate.cc
*******************************************************************************/
#ifndef MANAGERUTILS_SYSUTILS_PATHUTILS_GENERATE_HPP
#define MANAGERUTILS_SYSUTILS_PATHUTILS_GENERATE_HPP

#include <string>

namespace mgr {
/*******************************************************************************
*   RandomFileName
*   * Generate random file name with alpha-numerical length of n
*   * Could add extension to end of filename
*   * Could prefix with '.' to make hiden
*******************************************************************************/
extern std::string RandomFileName(
   const unsigned n ,
   const std::string& ext ="" ,
   const bool hidden=true
);

} /* mgr */


#endif /* end of include guard: MANAGERUTILS_SYSUTILS_PATHUTILS_GENERATE_HPP */
