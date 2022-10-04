/*******************************************************************************
*
*  Filename    : OStreamUtils.hpp
*  Description : Handful of helpful all public classes to enhance ostream usage.
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef MANAGERUTILS_COMMON_STLUTILS_OSTREAMUTILS_HPP
#define MANAGERUTILS_COMMON_STLUTILS_OSTREAMUTILS_HPP

#include <iostream>

namespace mgr {

/*******************************************************************************
*   Clearing a output line ( better used for screen outputs)
*******************************************************************************/
class clearline {
public:
  clearline(){}
  virtual ~clearline(){}
  friend std::ostream& operator<<( std::ostream&, const clearline& );
};

/*******************************************************************************
*   Printing a seperator line by repeating the same character multiple times
*******************************************************************************/
class seperator {
private:
  char token;
  size_t  n;
public:
  seperator(const char x='*',const size_t m= 50 );
  virtual ~seperator();
  friend std::ostream& operator<<( std::ostream&, const seperator& );
};



}/* mgr */

#endif /* end of include guard: MANAGERUTILS_COMMON_STLUTILS_OSTREAMUTILS_HPP */
