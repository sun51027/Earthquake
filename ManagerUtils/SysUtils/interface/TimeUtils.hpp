/*******************************************************************************
*
*  Filename    : TimeUtils.hpp
*  Description : System time display functions
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef MANAGERUTILS_SYSUTILS_TIMEUTILS_HPP
#define MANAGERUTILS_SYSUTILS_TIMEUTILS_HPP

#include <string>

namespace mgr{
    /*******************************************************************************
    *   ** CurrentDateTime
    *   Return current date and time as std::string
    *******************************************************************************/
    extern std::string CurrentDateTime();

    /*******************************************************************************
    *   ** Sleep Thead for set number of MilliSeconds
    *******************************************************************************/
    extern void SleepMillSec( const unsigned );
}/* mgr */

#endif/* end of include guard */
