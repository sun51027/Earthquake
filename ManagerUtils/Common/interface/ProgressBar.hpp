/*******************************************************************************
*
*  Filename    : Named.hpp
*  Description : Base Class with std::string as ID and latex title
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef MANAGERUTILS_COMMON_PROGRESSBAR_HPP
#define MANAGERUTILS_COMMON_PROGRESSBAR_HPP

#include <iostream>
#include <chrono>
#include <tuple>

#define PBSTR "========================================================================================================================================="
#define PBWIDTH 60

namespace mgr{
    std::tuple<int, int, int> format_duration( std::chrono::milliseconds& );
    void ProgressBar( const int&, const int& );
}

#endif/* end of include guard: MANAGERUTILS_COMMON_NAMED_HPP */
