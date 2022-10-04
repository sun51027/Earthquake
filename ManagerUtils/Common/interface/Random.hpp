/*******************************************************************************
*
*  Filename    : Random.hpp
*  Description : helper functions for random variable usage.
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef MANAGERUTILS_COMMON_RANDOM_HPP
#define MANAGERUTILS_COMMON_RANDOM_HPP

#include <string>

namespace mgr{
    /*******************************************************************************
    *   SeedRandomTime()
    *   * Seeding number generator by time of data
    *******************************************************************************/
    extern void SeedRandomTime();

    /*******************************************************************************
    *   RandomString()
    *   * Returning a randomly generated alpha-numerical string
    *******************************************************************************/
    extern std::string RandomString( const unsigned length );
}/* mgr */


#endif/* end of include guard: MANAGERUTILS_COMMON_RANDOM_HPP */
