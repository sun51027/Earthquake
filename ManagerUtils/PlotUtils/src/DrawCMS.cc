/*******************************************************************************
*
*  Filename    : DrawCMS.cc
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "../../PlotUtils/interface/Constants.hpp"
#include "TLatex.h"
#include <string>

namespace mgr{
    void
    DrawCMSLabel( int tag )
    {
        TLatex tl;
        tl.SetNDC( kTRUE );
        tl.SetTextFont( FONT_TYPE );
        tl.SetTextSize( AXIS_TITLE_FONT_SIZE );
        tl.SetTextAlign( TOP_LEFT );
        tl.DrawLatex( PLOT_X_MIN + TEXT_MARGIN, PLOT_Y_MAX - TEXT_MARGIN, "#bf{CMS}" );

        if( tag == PRELIMINARY ){
            tl.DrawLatex( PLOT_X_MIN + TEXT_MARGIN, PLOT_Y_MAX - TEXT_MARGIN - LINE_HEIGHT, "#it{Preliminary}" );
        }
        else if( tag == SIMULATION ){
            tl.DrawLatex( PLOT_X_MIN + TEXT_MARGIN, PLOT_Y_MAX - TEXT_MARGIN - LINE_HEIGHT, "#it{Simulation}" );
        }
        else if( tag == WORK ){
            tl.DrawLatex( PLOT_X_MIN + TEXT_MARGIN, PLOT_Y_MAX - TEXT_MARGIN - LINE_HEIGHT, "#it{Work in progress}" );
        }
        else if( tag == GEN){
            tl.DrawLatex( PLOT_X_MIN + TEXT_MARGIN, PLOT_Y_MAX - TEXT_MARGIN - LINE_HEIGHT, "#it{Generator}" );
        }
    }

    /******************************************************************************/

    void
    DrawCMSLabelOuter( int tag )
    {
        TLatex tl;
        tl.SetNDC( kTRUE );
        tl.SetTextFont( FONT_TYPE );
        tl.SetTextSize( AXIS_TITLE_FONT_SIZE );
        tl.SetTextAlign( BOTTOM_LEFT );
        std::string text = "#bf{CMS} ";

        if( tag == PRELIMINARY ){
            text += "#it{Preliminary}";
        }
        else if( tag == SIMULATION ){
            text += "#it{Simulation}";
        }
        else if( tag == WORK ){
            text += "#it{Work in progress}";
        }
        else if( tag == GEN ){
            text += "#it{Generator}";
        }

        tl.DrawLatex( PLOT_X_MIN, PLOT_Y_MAX + TEXT_MARGIN / 2, text.c_str() );
    }

    /******************************************************************************/

    void
    DrawLuminosity( double luminosity )
    {
        TLatex tl;
        tl.SetNDC( kTRUE );
        tl.SetTextFont( FONT_TYPE );
        tl.SetTextSize( AXIS_TITLE_FONT_SIZE );
        char buffer[ 1024 ];
        sprintf( buffer, "%.1lffb^{-1} (%d TeV)", luminosity / 1000., 13 );
        tl.SetTextAlign( BOTTOM_RIGHT );
        tl.DrawLatex( PLOT_X_MAX, PLOT_Y_MAX + ( TEXT_MARGIN / 2 ), buffer );
    }

    void DrawEntryLeft( char *entry )
    {
	TLatex tl;
   	tl.SetNDC( kTRUE );
    	tl.SetTextFont( FONT_TYPE );
    	tl.SetTextSize( TEXT_FONT_SIZE );
    	tl.SetTextAlign( BOTTOM_LEFT );
   	tl.DrawLatex( PLOT_X_MIN, PLOT_Y_MAX + TEXT_MARGIN / 2, entry );
    }
    void DrawEntryRight( const char *entry )
    {
	TLatex tl;
    	tl.SetNDC( kTRUE );
    	tl.SetTextFont( FONT_TYPE );
    	tl.SetTextSize( TEXT_FONT_SIZE );
    	tl.SetTextAlign( BOTTOM_RIGHT );
    	tl.DrawLatex( PLOT_X_MAX, PLOT_Y_MAX + ( TEXT_MARGIN / 2 ), entry );
    }

}
