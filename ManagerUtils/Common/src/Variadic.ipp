/*******************************************************************************
*
*  Filename    : Variadic.ipp
*  Description : implementation of Variadic template functions
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef MANAGERUTILS_COMMON_VARIADIC_IPP
#define MANAGERUTILS_COMMON_VARIADIC_IPP
#include "../../Common/interface/Variadic.hpp"

namespace mgr {
    /*******************************************************************************
    *   Single Argument response
    *******************************************************************************/
    template < typename OBJTYPE, typename ARGTYPE >
    std::vector < OBJTYPE >
    MakeVector( ARGTYPE first )
    {
        OBJTYPE x( first );
        return std::vector < OBJTYPE > ( 1, x );
    }

    /*******************************************************************************
    *   Multi Argument return value
    *******************************************************************************/
    template < typename OBJTYPE, typename ARGTYPE, typename ... VARTYPE >
    std::vector < OBJTYPE >
    MakeVector( ARGTYPE first, VARTYPE ... arg )
    {
        std::vector < OBJTYPE > ans = MakeVector < OBJTYPE > ( arg ... );
        ans.insert( ans.begin(), OBJTYPE( first ) );
        return ans;
    }
}/* mgr */



#endif/* end of include guard: MANAGERUTILS_COMMON_VARIADIC_IPP */
