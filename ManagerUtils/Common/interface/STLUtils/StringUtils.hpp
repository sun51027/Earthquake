/*******************************************************************************
*
*  Filename    : StringUtils.hpp
*  Description : A handful of usefule std::string enhancing functions
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef MANAGERUTILS_COMMON_STLUTILS_STRINGUTILS_HPP
#define MANAGERUTILS_COMMON_STLUTILS_STRINGUTILS_HPP

#include <string>

/*******************************************************************************
*   Boost Path flavour string joining
*   'a' / 'b' ~ 'a' + '/' + 'b', automatically removes redundancies
*******************************************************************************/
extern std::string operator/( const std::string&, const std::string& );

/*******************************************************************************
*   Boost PTree flavour string joinining
*   "a" * "b" ~ "a" + '.' + "b"
*******************************************************************************/
extern std::string operator*( const std::string&, const std::string& );


#endif /* end of include guard: MANAGERUTILS_COMMON_STLUTILS_STRINGUTILS_HPP */
