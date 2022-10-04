/*******************************************************************************
*
*  Filename    : STLUtils_OStreamUtils.cc
*  Description : Ostream interaction class Implementation
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "ManagerUtils/Common/interface/STLUtils/OStreamUtils.hpp"

#include <ostream>

namespace mgr{
    /*******************************************************************************
    *   Clear line implementation
    *******************************************************************************/

    std::ostream&
    operator<<( std::ostream& os, const clearline& )
    {
        for( size_t i = 0; i < 255; ++i ){
            os << "\b \b" << std::flush;
        }

        return os;
    }

    /*******************************************************************************
    *   seperator implementation
    *******************************************************************************/
    seperator::seperator( const char x, const size_t m ) :
        token( x ),
        n( m ){}

    seperator::~seperator(){}

    std::ostream&
    operator<<( std::ostream& os, const seperator& x )
    {
        for( size_t i = 0; i < x.n; ++i ){
            os << x.token << std::flush;
        }

        return os;
    }
}/* mgr */
