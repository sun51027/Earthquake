/*******************************************************************************
*
*  Filename    : ParameterArithmetic.hpp
*  Description : Arithmetic opertion with good error propagation estimates
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*  Main reference from here:
*
*******************************************************************************/
#ifndef MANAGERUTILS_MATHS_PARAMETER_ARITHMETIC_HPP
#define MANAGERUTILS_MATHS_PARAMETER_ARITHMETIC_HPP

#include "ManagerUtils/Common/interface/Variadic.hpp"
#include "ManagerUtils/Maths/interface/Parameter/Parameter.hpp"
#include "ManagerUtils/Maths/interface/StatisticsUtil.hpp"
#include <vector>

namespace mgr {

/*******************************************************************************
*   Common interfacing with MinosError functions
*******************************************************************************/
extern Parameter MakeMinos(
  gsl_function* nll,
  const double  initguess,
  const double  min,
  const double  max,
  const double  confidencelevel = mgr::stat::onesigma_level
  );

extern Parameter MakeMinos(
  mgr::gsl::gsl_multifunc* nll,
  mgr::gsl::gsl_multifunc* varfunction,
  gsl_vector*              initguess,
  const double             confidencelevel = mgr::stat::onesigma_level,
  gsl_vector*              upperguess = nullptr,
  gsl_vector*              lowerguess = nullptr
  );

/*******************************************************************************
*   Defining Default approximation NLL function for a given parameter
*******************************************************************************/
double LinearVarianceNLL( double, const Parameter& );

/*******************************************************************************
*   Main calculation functions, do mot call from outside
*   - Implemented in ParamterArithmethic.cc
*******************************************************************************/
Parameter SumUncorrelated(
  const std::vector<Parameter>& paramlist,
  const double confidencelevel = mgr::stat::onesigma_level,
  double ( * nll )( double, const Parameter& ) = & LinearVarianceNLL
  );

Parameter ProdUncorrelated(
  const std::vector<Parameter>& paramlist,
  const double confidencelevel = mgr::stat::onesigma_level,
  double ( * nll )( double, const Parameter& ) = & LinearVarianceNLL
  );

/*******************************************************************************
*   Template variadic functions for better interfacing
*   ex. Sum( a, b, c , d );
*******************************************************************************/
template<typename ... Ts>
Parameter
Sum( const Parameter& x,  Ts ... args )
{
  return SumUncorrelated( MakeVector<Parameter>( x, args ... ) );
}

template<typename ... Ts>
Parameter
Prod( const Parameter& x, Ts ... args )
{
  return ProdUncorrelated( MakeVector<Parameter>( x, args ... ) );
}



}/* mgr */


#endif/* end of include guard: MANAGERUTILS_MATHS_PARAMETERARITHMETIC_HPP */
