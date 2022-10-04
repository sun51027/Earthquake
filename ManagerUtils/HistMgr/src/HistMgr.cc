#include "ManagerUtils/HistMgr/interface/HistMgr.h"

#include "TFile.h"
#include "TGaxis.h"

#include <iostream>
#include <regex>

using namespace std;
using namespace mgr;

/*******************************************************************************
*   Constructor/Destructor
*******************************************************************************/
HistMgr::HistMgr( const string& tag ) :
    _tag( tag )
{
}

/******************************************************************************/

HistMgr::~HistMgr()
{
}

/*******************************************************************************
*   Histogram access functions
*******************************************************************************/
TH1D*
HistMgr::Hist( const string& name )
{
    return _histmgr.GetObj( name );
}

/******************************************************************************/

const TH1D*
HistMgr::Hist( const string& name ) const
{
    return _histmgr.GetObj( name );
}

/******************************************************************************/

vector<string>
HistMgr::AvailableHistList() const
{
    return _histmgr.ObjNameList();
}

/*******************************************************************************
*   Histogram addition functions
*******************************************************************************/
void
HistMgr::AddHist(
    const string& label,
    const string& xtitle,
    const string& ytitle,
    const int     bin_size,
    const double  x_lower,
    const double  x_upper
    )
{
    string aliasname = _tag + "_" + label;

    TH1D* hist = new TH1D( aliasname.c_str(), _tag.c_str(), bin_size, x_lower, x_upper );
    hist->SetDirectory( 0 );
    hist->SetStats( false );
    hist->GetXaxis()->SetTitle( xtitle.c_str() );
    hist->GetYaxis()->SetTitle( ytitle.c_str() );
    hist->Sumw2();
    _histmgr.AddObj( hist );
}

void
HistMgr::AddHist( const string& label, TH1* hist )
{
    string aliasname = _tag + "_" + label;
    hist->SetNameTitle( aliasname.c_str(), _tag.c_str() );
    hist->SetDirectory( 0 );
    hist->SetStats( false );
    hist->Sumw2();
}

/*******************************************************************************
*   Styling memeber functions
*******************************************************************************/
void
HistMgr::Scale( const double x )
{
    for( auto hist : _histmgr ){
        for( int i = 0; i < hist->GetSize(); ++i ){
            double bincontent = hist->GetBinContent( i );
            double binerror   = hist->GetBinError( i );
            bincontent *= x;
            binerror   *= x;
            hist->SetBinContent( i, bincontent );
            hist->SetBinError( i, binerror );
        }
    }
}

/******************************************************************************/

void
HistMgr::SetColor( const Color_t x )
{
    SetLineColor( x );
    SetFillColor( x );
}

/******************************************************************************/

void
HistMgr::SetLineColor( const Color_t x )
{
    for( auto hist : _histmgr ){
        hist->SetLineColor( x );
    }
}

/******************************************************************************/

void
HistMgr::SetFillColor( const Color_t x )
{
    for( auto hist : _histmgr ){
        hist->SetFillColor( x );
    }
}

/******************************************************************************/

void
HistMgr::SetFillStyle( const Style_t x )
{
    for( auto hist : _histmgr ){
        hist->SetFillStyle( x );
    }
}

/*******************************************************************************
*   File Loading and saving functions
*******************************************************************************/
void
HistMgr::LoadFromFile( const std::string& filename )
{
    TFile* histfile = TFile::Open( filename.c_str(), "READ" );

    for( auto hist : _histmgr ){
        const string histrootname = hist->GetName();
        TH1D* histinfile          = (TH1D*)( histfile->Get( histrootname.c_str() ) );

        for( int i = 1; i < hist->GetSize(); ++i ){
            hist->SetBinContent( i, histinfile->GetBinContent( i ) );
            hist->SetBinError( i, histinfile->GetBinError( i ) );
        }
    }

    delete histfile;
}

/******************************************************************************/

void
HistMgr::SaveToFile( const std::string& filename )
{
    TFile* histfile = TFile::Open( filename.c_str(), "UPDATE" );

    for( const auto hist : _histmgr ){
        hist->Write();
    }

    delete histfile;
}

/*******************************************************************************
*   Axis title formatting functions
*******************************************************************************/

string
HistMgr::GetXUnit( const TH1D* x )
{
    static const std::regex re( ".*\\ \\[(.*)\\]" );

    const string title = x->GetXaxis()->GetTitle();
    std::smatch matchresults;

    std::regex_match( title, matchresults, re );

    if( matchresults.size() != 2 ){
        return "";
    }
    else{
        return matchresults[ 1 ];
    }
}
