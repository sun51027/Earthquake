/*******************************************************************************
*
*  Filename    : Efficiency.cc
*  Description : Implementation for efficiency wrappers
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "ManagerUtils/Maths/interface/Parameter/Arithmetic.hpp"
#include "ManagerUtils/Maths/interface/Parameter/CommonDistro.hpp"
#include "ManagerUtils/Maths/interface/StatisticsUtil.hpp"
#include "TEfficiency.h"

namespace mgr{
    /*******************************************************************************
    *   Minos error from calling StatisticsUtil defined functions
    *******************************************************************************/
    Parameter
    Efficiency::Minos( const double passed, const double total, const double confidencelevel )
    {
        double params[] = { passed, total };
        gsl_function binomial;
        binomial.function = &mgr::stat::BinomialNLL;
        binomial.params   = params;
        return MakeMinos(
            &binomial,
            passed / total,
            (mgr::gsl::epsilon)*( mgr::gsl::epsilon ),// Must be very small...
            1 - (mgr::gsl::epsilon)*( mgr::gsl::epsilon ),
            confidencelevel
            );
    }

    /*******************************************************************************
    *   General Wrapper functions for TEfficiency Bayesian method
    *******************************************************************************/
    const bool Efficiency::shortest_interval = true;
    const bool Efficiency::central_interval  = false;

    Parameter
    Efficiency::Bayesian(
        double passed,
        double total,
        double confidencelevel,
        bool   confidencemethod,
        double alpha,
        double beta
        )
    {
        const double central = ( passed + alpha - 1. ) / ( total + alpha + beta - 2. );
        const double err_up  = TEfficiency::Bayesian(
            total, passed,
            confidencelevel,
            alpha, beta,
            true,// For upper boundary
            confidencemethod
            ) - central;
        const double err_down = central - TEfficiency::Bayesian(
            total, passed,
            confidencelevel,
            alpha, beta,
            false,// For lower boundary
            confidencemethod
            );
        return Parameter( central, err_up, err_down );
    }
}/* mgr */
