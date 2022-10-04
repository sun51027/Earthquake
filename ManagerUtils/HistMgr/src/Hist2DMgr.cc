#include "ManagerUtils/HistMgr/interface/Hist2DMgr.h"

using namespace std;
using namespace mgr;

/*******************************************************************************
*   Constructor and destructor
*******************************************************************************/
Hist2DMgr::Hist2DMgr( const string& tag ) :
    _tag( tag )
{
}

Hist2DMgr::~Hist2DMgr()
{
}

/*******************************************************************************
*  Histogram access functions
*******************************************************************************/
TH2D*
Hist2DMgr::Hist2D( const string& name )
{
    return _histmgr.GetObj( name );
}

/******************************************************************************/

const TH2D*
Hist2DMgr::Hist2D( const string& name ) const
{
    return _histmgr.GetObj( name );
}

/******************************************************************************/

vector<string>
Hist2DMgr::AvailableHist2DList() const
{
    return _histmgr.ObjNameList();
}

/*******************************************************************************
*   TH2D declaration functions
*******************************************************************************/
void
Hist2DMgr::AddHist2D(
    const std::string& label,
    const std::string& xtitle,
    const std::string& ytitle,
    const unsigned     xbin,
    const double       xmin,
    const double       xmax,
    const unsigned     ybin,
    const double       ymin,
    const double       ymax
    )
{
    string aliasname = _tag + "_" + label;

    TH2D* hist = new TH2D(
        aliasname.c_str(), _tag.c_str(),
        xbin, xmin, xmax,
        ybin, ymin, ymax
        );

    hist->SetDirectory( 0 );
    hist->SetStats( false );
    hist->GetXaxis()->SetTitle( xtitle.c_str() );
    hist->GetYaxis()->SetTitle( ytitle.c_str() );
    hist->Sumw2();
    _histmgr.AddObj( hist );
}
