/*******************************************************************************
*
*  Filename    : Parameter.hh
*  Description : Storage class for handling parameters
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*  Includes support for systematic and statistical uncertainties
*
*******************************************************************************/
#ifndef MANAGERUTILS_MATHS_PARAMETER_PARAMETER_HPP
#define MANAGERUTILS_MATHS_PARAMETER_PARAMETER_HPP

#include <RooRealVar.h>

namespace mgr {

class Parameter
{
public:

  // Constructor, Desctructor and conversion :: See Parameter.cc
  Parameter();
  Parameter(
    const double central,
    const double error_up,
    const double error_down );
  Parameter( const Parameter& );
  Parameter( const RooRealVar& );

  virtual
  ~Parameter ();

  Parameter& operator=( const Parameter& );

  inline operator double() const { return CentralValue(); }

  // Basic access functions
  inline double CentralValue()  const { return _centralValue; }
  inline double AbsUpperError() const { return _error_up; }
  inline double AbsLowerError() const { return _error_down; }
  inline double AbsAvgError()   const { return (AbsUpperError()+AbsLowerError() ) / 2; }
  inline double RelUpperError() const { return AbsUpperError()/CentralValue(); }
  inline double RelLowerError() const { return AbsLowerError()/CentralValue(); }
  inline double RelAvgError()   const { return ( RelUpperError()+RelLowerError() ) /2.; }
  inline double UpperValue()    const { return CentralValue() + AbsUpperError();}
  inline double LowerValue()    const { return CentralValue() - AbsLowerError(); }

  // Normalized version
  Parameter NormParam() const;

  // Error arithmatics :: See src/Parameter_Arithmatics.cc
  Parameter&       operator+=( const Parameter& );
  Parameter&       operator*=( const Parameter& );
  Parameter&       operator*=( const double );
  Parameter&       operator/=( const double );
  Parameter        operator+( const Parameter& ) const;
  Parameter        operator*( const Parameter& ) const;
  Parameter        operator*( const double )     const;
  Parameter        operator/( const double )     const;
  friend Parameter operator*( const double, const Parameter& );
  friend Parameter operator/( const double, const Parameter& );

private:
  double _centralValue;
  double _error_up;
  double _error_down;
};

} /* mgr */


#endif/* end of include guard: __PARAMETER_HH__ */
