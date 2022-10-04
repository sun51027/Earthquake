import sys
# /*******************************************************************************
# *   Getting histogram graphically maximum point
# *******************************************************************************/
def GetHistYmax( hist ):
    ans = 0
    for i in range( hist.GetNcells() ):
        bincont = abs( hist.GetBinContent( i ) )
        binerr  = abs( hist.GetBinError( i ) )
        ans = max( ans, bincont + binerr )
    return ans

def GetHistlstYmax( histlst ):
    ans = 0
    for hist in histlst:
        ans = max( ans, GetHistYmax( hist ) )
    return ans

# /*******************************************************************************
# *   TGraph Max and min calculations
# *******************************************************************************/
def GetGraphYmax( gr ):
    ans = -sys.maxint

    for i in range( gr.GetN() ):
        bin = gr.GetY()[ i ] + gr.GetErrorYhigh( i )
        ans = max( ans, bin )
    return ans

def GetGraphYmin( gr ):
    ans = sys.maxint

    for i in range( gr.GetN() ):
        bin = gr.GetY()[ i ] - gr.GetErrorYlow( i )
        ans = min( ans, bin )
    return ans

def GetGraphlstYmax( grlst ):
    ans = -sys.maxint
    
    for gr in grlst:
        ans = max( ans, GetGraphYmax( gr ) )
    return ans

def GetGraphlstYmin( grlst ):
    ans = sys.maxint

    for gr in grlst:
        ans = min( ans, GetGraphYmin( graph ) )
    return ans

# /*******************************************************************************
# *   TH1D division functions
# *******************************************************************************/
def DivideHist( num, den, cen=0 ):
    ans = num.Clone()
    ans.Divide( den )

    for i in range( num.GetNcells() ):
        if num.GetBinContent( i ) == 0 and den.GetBinContent( i ) == 0:
            ans.SetBinContent( i, cen )
    return ans;

# /*******************************************************************************
# *   TH1D summation functions
# *******************************************************************************/

def SumHist( histlst ):
    hist = histlst[0].Clone()
    for h in histlst[1:]:
        hist.Add(h)
    return hist
