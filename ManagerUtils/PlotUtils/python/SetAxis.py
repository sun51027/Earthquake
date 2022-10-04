from ManagerUtils.PlotUtils.Constants import *
from ROOT import TGaxis
def SetAxis( plot ):
    
    # Global settings from TGaxis
    TGaxis.SetMaxDigits( 4 )
    TGaxis.SetExponentOffset( -1000, -1000 )

    # X-axis settings
    x = plot.GetXaxis()
    x.SetLabelFont( FONT_TYPE )
    x.SetTitleFont( FONT_TYPE )
    x.SetLabelSize( AXIS_LABEL_FONT_SIZE )
    x.SetTitleSize( AXIS_TITLE_FONT_SIZE )
 
    # Y-axis settings
    y = plot.GetYaxis()
    y.SetLabelFont( FONT_TYPE )
    y.SetTitleFont( FONT_TYPE )
    y.SetLabelSize( AXIS_LABEL_FONT_SIZE )
    y.SetTitleSize( AXIS_TITLE_FONT_SIZE )
    y.SetTitleOffset( 1.2 )
    
    if plot.GetMaximum() >= 10000:
        y.SetTitle( y.GetTitle() + " x 10^{3}" )


def SetTopPlotAxis( plot ):
    SetAxis( plot )
    plot.GetXaxis().SetLabelSize( 0 )
    plot.GetXaxis().SetTitleSize( 0 )
    plot.GetYaxis().SetTitleOffset( 1.2 )
    
def SetBottomPlotAxis( plot ):
    SetAxis( plot )
    plot.GetYaxis().SetNdivisions( 503 )
    plot.GetXaxis().SetTitleOffset( 3.0 )
    plot.GetYaxis().SetTitleOffset( 1.2 )

def SetNormToUnity( plot ):
    norm = plot.Integral()
    plot.Scale( 1 / norm )
