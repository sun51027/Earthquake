/*******************************************************************************
*
*  Filename    : LatexMgr.hpp
*  Description : LatexMgr class implementation
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "ManagerUtils/PlotUtils/interface/Common.hpp"
#include "ManagerUtils/PlotUtils/interface/LatexMgr.hpp"

namespace mgr{
    /*******************************************************************************
    *   Constructor and Destructor
    *******************************************************************************/
    LatexMgr::LatexMgr() :
        _linecount( 0 ),
        _origin_x( 0.5 ),
        _origin_y( 0.5 )
    {
        // Default values
        SetNDC( kTRUE );
        SetTextFont( FONT_TYPE );
        SetTextSize( TEXT_FONT_SIZE );
    }

    LatexMgr::~LatexMgr(){}

    /*******************************************************************************
    *   Writing functions
    *******************************************************************************/
    LatexMgr&
    LatexMgr::WriteLine( const std::string& line )
    {
        DrawLatex(
            _origin_x,
            _origin_y - _linecount * LINE_HEIGHT,
            line.c_str()
            );
        _linecount++;
        return *this;
    }

    /*******************************************************************************
    *   Repositioning functions
    *******************************************************************************/
    LatexMgr&
    LatexMgr::SetOrigin( const double x, const double y, const Short_t a )
    {
        SetTextAlign( a );
        return SetOrigin( x, y );
    }

    /******************************************************************************/

    LatexMgr&
    LatexMgr::SetOrigin( const double x, const double y )
    {
        _origin_x  = x;
        _origin_y  = y;
        _linecount = 0;
        return *this;
    }
}/* mgr */
