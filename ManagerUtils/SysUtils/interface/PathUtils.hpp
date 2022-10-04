/*******************************************************************************
*
*  Filename    : PathUtils.hpp
*  Description : C++ Wrapper functions for path interaction
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*  Powered Mainly powered by boost/filesystem and POSIX
*
*******************************************************************************/
#ifndef MANAGERUTILS_SYSUTILS_PATHUTILS
#define MANAGERUTILS_SYSUTILS_PATHUTILS

/*******************************************************************************
*   Automatically including string enhancing libraries
*******************************************************************************/
#include "../../Common/interface/STLUtils/StringUtils.hpp"

/*******************************************************************************
*   Commonly used Paths
*******************************************************************************/
#include "../../SysUtils/interface/PathUtils/CommonPath.hpp"

/*******************************************************************************
*   Globbing related functions
*******************************************************************************/
#include "../../SysUtils/interface/PathUtils/Glob.hpp"

/*******************************************************************************
*   String generation functions
*******************************************************************************/
#include "../../SysUtils/interface/PathUtils/Generate.hpp"


#endif/* end of include guard: MANAGERUTILS_SYSUTILS_PATHUTILS */
