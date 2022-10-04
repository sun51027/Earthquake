from ManagerUtils.PlotUtils.Constants import *
import ROOT
# /*******************************************************************************
# *   Pad settings functions
# *******************************************************************************/
def NewCanvas( obj="c" ):
    ROOT.gROOT.SetBatch(ROOT.kTRUE)
    c = ROOT.TCanvas( obj, obj, DEFAULT_CANVAS_WIDTH, DEFAULT_CANVAS_HEIGHT )
    return c

def NewTopPad():
    pad = ROOT.TPad( "toppad", "", 0., TOP_BOTTOM_SEP, 1., 1. )
    pad.SetTicks( 1, 1 )
    pad.SetBottomMargin( 0.05 )
    pad.SetLeftMargin( PLOT_X_MIN )
    pad.SetRightMargin( 1 - PLOT_X_MAX )
    pad.SetTopMargin( ( 1 - PLOT_Y_MAX ) / ( 1 - TOP_BOTTOM_SEP ) )
    return pad

def NewBottomPad():
    pad = ROOT.TPad( "botpad", "", 0., 0.0, 1., TOP_BOTTOM_SEP )
    pad.SetTicks( 1, 1 )
    pad.SetTopMargin( 0.025 )
    pad.SetLeftMargin( PLOT_X_MIN )
    pad.SetRightMargin( 1 - PLOT_X_MAX )
    pad.SetBottomMargin( ( PLOT_Y_MIN ) / ( TOP_BOTTOM_SEP ) )
    return pad

def SetSinglePad( c ):
    c.SetTicks( 1, 1)
    c.SetLeftMargin( PLOT_X_MIN )
    c.SetRightMargin( 1 - PLOT_X_MAX )
    c.SetBottomMargin( PLOT_Y_MIN )
    c.SetTopMargin( 1 - PLOT_Y_MAX )

def SetSinglePadWithPalette( c ): 
    c.SetTicks( 1, 1)
    c.SetLeftMargin( PLOT_X_MIN )
    c.SetBottomMargin( PLOT_Y_MIN )
    c.SetTopMargin( 1 - PLOT_Y_MAX )

# /*******************************************************************************
# *   New Legend settings
# *******************************************************************************/
def NewLegend( xmin, ymin, xmax, ymax ):
    leg = ROOT.TLegend( xmin, ymin, xmax, ymax )
    leg.SetBorderSize( 0 )
    leg.SetTextFont( FONT_TYPE )
    leg.SetTextSize( TEXT_FONT_SIZE )
    leg.SetFillColorAlpha( 0, 0 )
    leg.SetLineColor( ROOT.kWhite )
    return leg

def NewTextBox( xmin, ymin, xmax, ymax ):
    text = ROOT.TPaveText( xmin, ymin, xmax, ymax, "NDC" )
    text.SetTextFont( FONT_TYPE )
    text.SetTextSize( TEXT_FONT_SIZE )
    text.SetFillColor( ROOT.kWhite )
    return text

def DrawEntryLeft( entry ):
    tl = ROOT.TLatex()
    tl.SetNDC( True )
    tl.SetTextFont( FONT_TYPE )
    tl.SetTextSize( TEXT_FONT_SIZE )
    tl.SetTextAlign( BOTTOM_LEFT )
    tl.DrawLatex( PLOT_X_MIN, PLOT_Y_MAX + TEXT_MARGIN / 2, entry )

def DrawEntryRight( entry ):
    tl = ROOT.TLatex()
    tl.SetNDC( True )
    tl.SetTextFont( FONT_TYPE )
    tl.SetTextSize( TEXT_FONT_SIZE )
    tl.SetTextAlign( BOTTOM_RIGHT )
    tl.DrawLatex( PLOT_X_MAX, PLOT_Y_MAX + ( TEXT_MARGIN / 2 ), entry )

# /*******************************************************************************
# *    Plot CMS label
# *******************************************************************************/
def DrawCMSLabel( tag ):
    tl = ROOT.TLatex()
    tl.SetNDC( True )
    tl.SetTextFont( FONT_TYPE )
    tl.SetTextSize( AXIS_TITLE_FONT_SIZE )
    tl.SetTextAlign( TOP_LEFT )
    tl.DrawLatex( PLOT_X_MIN + TEXT_MARGIN, PLOT_Y_MAX - TEXT_MARGIN, "#bf{CMS}" )

    if tag == PRELIMINARY:
        tl.DrawLatex( PLOT_X_MIN + TEXT_MARGIN, PLOT_Y_MAX - TEXT_MARGIN - LINE_HEIGHT, "#it{Preliminary}" )
    
    elif tag == SIMULATION:
        tl.DrawLatex( PLOT_X_MIN + TEXT_MARGIN, PLOT_Y_MAX - TEXT_MARGIN - LINE_HEIGHT, "#it{Simulation}" )

def DrawCMSLabelOuter( tag ):
    tl = ROOT.TLatex()
    tl.SetNDC( True )
    tl.SetTextFont( FONT_TYPE )
    tl.SetTextSize( AXIS_TITLE_FONT_SIZE )
    tl.SetTextAlign( BOTTOM_LEFT )
    text = "#bf{CMS} "

    if tag == PRELIMINARY:
        text += "#it{Preliminary}"
    elif tag == SIMULATION:
        text += "#it{Simulation}"

    tl.DrawLatex( PLOT_X_MIN, PLOT_Y_MAX + TEXT_MARGIN / 2, text )

def DrawLuminosity( lumi ):
    tl = ROOT.TLatex()
    tl.SetNDC( True )
    tl.SetTextFont( FONT_TYPE )
    tl.SetTextSize( AXIS_TITLE_FONT_SIZE )
    buffer = "{0:.1f}fb^{{-1}} ({1} TeV)".format( lumi / 1000., 13 )
    tl.SetTextAlign( BOTTOM_RIGHT )
    tl.DrawLatex( PLOT_X_MAX, PLOT_Y_MAX + ( TEXT_MARGIN / 2 ), buffer )
