/*******************************************************************************
*
*  Filename    : Paramter_CommonDistro_Poisson.cc
*  Description : Implementation for Poisson mesurement functions
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "ManagerUtils/Maths/interface/Parameter/Arithmetic.hpp"
#include "ManagerUtils/Maths/interface/Parameter/CommonDistro.hpp"
#include "ManagerUtils/Maths/interface/StatisticsUtil.hpp"
#include <cmath>

namespace mgr{
    Parameter
    Poisson::Minos( const double obs, const double confidencelevel )
    {
        gsl_function poisson;
        poisson.function = &mgr::stat::PoissonNLL;
        poisson.params   = const_cast<double*>( &obs );
        return MakeMinos( &poisson, obs, mgr::gsl::epsilon, 2 * obs + 1, confidencelevel );
    }
}/* mgr */
