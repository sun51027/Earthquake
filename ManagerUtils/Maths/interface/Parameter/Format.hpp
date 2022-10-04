/*******************************************************************************
*
*  Filename    : ParameterFormat.hpp
*  Description : Producing a string representing a Parameter with a specific format
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef MANAGERUTILS_MATHS_PARAMETER_FORMAT_HPP
#define MANAGERUTILS_MATHS_PARAMETER_FORMAT_HPP

#include "ManagerUtils/Maths/interface/Parameter/Parameter.hpp"
#include <string>

namespace mgr {

/***********************************************************************************
* Function to representing a double as a floating point string                    *
* @param  input     the input double                                              *
* @param  precision Required number of digits after the decimal point             *
*                   Note that if precision is greater than 8, it will be rounded  *
*                   down to 8. If int is smaller than 0, it will print out the    *
*                   the the results of precision set to 8 with the exception that *
*                   the trailing zero will be stripped off.                       *
* @param  space     Whether or not to include spacing                             *
* @return           Returns output as a std::string                               *
***********************************************************************************/
extern std::string FloatingPoint(
  double     input,
  const int  precision,
  const bool space = true
  );

extern std::string Scientific(
  double         input,
  const unsigned precision,
  const bool     space = true
  );

/**************************************************************************************
*                                                                                    *
*   Latex style string format                                                        *
*   Both will be in the form of                                                      *
*        {central_value}^{upper_error}_{lower_error}                                 *
*   With the Scientific option moving a common exponent factor outside like          *
*        {central_value}^{upper_error}_{lower_error} \times 10^{exp}                 *
*   if exp != 0                                                                      *
*                                                                                    *
*   If the precision is set to be non-negative                                       *
*   The precision represents the number of digits to display for behind the          *
*   decimal point for the central value. And number of precision of the error will   *
*   be one greater than that of the central value                                    *
*                                                                                    *
*   If the precision is set to negative, (only possible for floating point notation) *
*   It will dump all of the digits (stripping trailing zeros)                        *
*                                                                                    *
**************************************************************************************/
extern std::string FloatingPoint(
  const Parameter& input,
  const int        precision = -1.,
  const bool       space = true
  );
extern std::string Scientific(
  const Parameter& input,
  const unsigned   sig,
  const bool       space = true
  );

/*******************************************************************************
*   Higgs Combine data card format
*   In the format of 1+scale*relup/1-scale*reldown
*******************************************************************************/
extern std::string HiggsDataCard( const Parameter&, const double scale );

} /* mgr */



#endif/* end of include guard: MANAGERUTILS_MATHS_PARAMETERFORMAT_HPP */
