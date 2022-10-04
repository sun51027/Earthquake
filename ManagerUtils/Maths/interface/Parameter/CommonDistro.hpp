/*******************************************************************************
*
*  Filename    : Efficiency.hpp
*  Description : Wraper function for our parameter to TEfficiency class
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*  Wrapper functions for storing selection efficiencies to the Parameter
*  objects. Mainly powered by the TEfficiency static functions
*
*******************************************************************************/
#ifndef MANAGERUTILS_MATHS_PARAMETER_COMMONDISTRO_HPP
#define MANAGERUTILS_MATHS_PARAMETER_COMMONDISTRO_HPP

#include "ManagerUtils/Maths/interface/Parameter/Parameter.hpp"
#include "ManagerUtils/Maths/interface/StatisticsUtil.hpp"

namespace mgr {

/*******************************************************************************
*   Efficiency - all efficiency related structures obtained here
*******************************************************************************/
namespace Efficiency
{

extern Parameter Minos(
  const double passed,
  const double total,
  const double confidencelevel = mgr::stat::onesigma_level
  );

/********************************************************************
* General wrapper function for the TEfficiency::Bayesian functions *
********************************************************************/
extern const bool shortest_interval;
extern const bool central_interval;
extern Parameter Bayesian(
  double passed,
  double total,
  double confidencelevel,
  bool   confidencemethod,
  double alpha,
  double beta
  );

};// Efficiency


/*******************************************************************************
*   Poisson - all Poison related method for creating Parameters
*******************************************************************************/
namespace Poisson
{
extern Parameter Minos(
  const double obs ,
  const double confidencelevel = mgr::stat::onesigma_level
);

// TODO: Add more methods

};// Poisson

}/* mgr */




#endif/* end of include guard: MANAGERUTILS_MATHS_EFFICIENCY_HPP */
