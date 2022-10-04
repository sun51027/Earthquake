/*******************************************************************************
*
*  Filename    : ProcessUtils.hpp
*  Description : Utility functions for process detection and handling
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef MANAGERUTILS_SYSUTILS_PROCESSUTILS
#define MANAGERUTILS_SYSUTILS_PROCESSUTILS

#include <string>

namespace mgr{
    /*******************************************************************************
    *   * GetCMDOutput( x )
    *   Storing the stdout/stderr of a shell command <x> to return string. Notice
    *   that none of the output will be displayed on screen, so use is resposible to
    *   ensure the input command <x> will finnish without user intervention.
    *******************************************************************************/
    extern std::string GetCMDSTDOutput( const std::string& x );
    extern std::string GetCMDERROutput( const std::string& x );
    extern std::string GetCMDOutput( const std::string& );

    /*******************************************************************************
    *   * NumOfThreads
    *   Returng the number of threads available to the machine
    *******************************************************************************/
    extern unsigned NumOfThreads();

    /*******************************************************************************
    *   * HasProcess( x, exclude )
    *   Returns the number of processes containing the string <x>, wile excluding
    *   processes containing string <exclude>. If b is left black, no exclusion
    *   will be processed
    *******************************************************************************/
    extern int HasProcess( const std::string& x, const std::string& exclude = "" );

    /*******************************************************************************
    *   ** Wait process( x, exclude )
    *   Sleep until the results of HassPorcess(x,exclude) returns 0.
    *******************************************************************************/
    extern void WaitProcess(
        const std::string& process_tag,
        const std::string& exclude = "",
        const unsigned     waitmills = 100
        );
}/* mgr */

#endif/* end of include guard: MANAGERUTILS_SYSUTILS_PROCESSUTILS */
