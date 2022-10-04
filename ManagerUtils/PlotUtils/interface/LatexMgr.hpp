/*******************************************************************************
*
*  Filename    : LatexMgr.hpp
*  Description : Wrapper class for better latex drawing support
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef MANAGERUTILS_PLOTUTILS_LATEXMGR_HPP
#define MANAGERUTILS_PLOTUTILS_LATEXMGR_HPP

#include "TLatex.h"
#include <string>

namespace mgr{
    class LatexMgr : public TLatex {
        public:

            LatexMgr();
            virtual ~LatexMgr ();

            LatexMgr& WriteLine( const std::string& );
            LatexMgr& SetOrigin( const double, const double );
            LatexMgr& SetOrigin( const double, const double, const Short_t );

        private:

            unsigned _linecount;
            double _origin_x;
            double _origin_y;
    };
}/* mgr */


#endif/* end of include guard: MANAGERUTILS_PLOTUTILS_LATEXMGR_HPP */
