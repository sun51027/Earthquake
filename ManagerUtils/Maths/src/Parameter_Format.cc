/*******************************************************************************
*
*  Filename    : Parameter_Formatting.cc
*  Description : String Formatting functions for Parameter class
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*  Using boost::format and std::regex for proper styling
*
*******************************************************************************/
#include "ManagerUtils/Maths/interface/Parameter.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <regex>
#include <string>

using namespace std;

namespace mgr{
    /*******************************************************************************
    *   Double variable string representations
    *******************************************************************************/
    string
    FloatingPoint( double x, const int precision, const bool spaceflag )
    {
        static const int max_precision = 8;
        static const int max_digits    = 27;
        boost::format fltfmt           =
            ( precision < 0 || precision >= max_precision ) ?
            boost::format( boost::str( boost::format( "%%.%dlf" ) % max_precision ) ) :
            boost::format( boost::str( boost::format( "%%.%df" ) % precision ) );
        string retstr = boost::str( fltfmt % x );

        // stripping trailing zero after decimal point
        if( precision < 0 && retstr.find( '.' ) != string::npos ){
            boost::trim_right_if( retstr, boost::is_any_of( "0" ) );
        }

        // stripping trailing decimal point
        boost::trim_right_if( retstr, boost::is_any_of( "." ) );

        // Add latex "\," spacing every 3 digits away from decimal point
        for( int space = max_digits; space > 0 && spaceflag; space = space - 3 ){
            if( retstr.find( '.' ) != string::npos ){
                // If decimal point exists, expand around decimal point
                boost::format bfexp( "(.*\\d)(\\d{%d}\\..*)" );
                boost::format afexp( "(.*\\.\\d{%d})(\\d.*)" );
                const std::regex beforedec( boost::str( bfexp % space ) );
                const std::regex afterdec( boost::str( afexp % space ) );
                retstr = std::regex_replace( retstr, beforedec, "$1\\,$2" );
                retstr = std::regex_replace( retstr, afterdec, "$1\\,$2" );
            }
            else{
                // If decimal point doesnt exist, expand around right most side
                boost::format bfexp( "(.*\\d)(\\d{%d})" );
                const std::regex beforedec( boost::str( bfexp % space ) );
                retstr = std::regex_replace( retstr, beforedec, "$1\\,$2" );
            }
        }

        return retstr;
    }

    /******************************************************************************/

    string
    Scientific( double x, const unsigned precision, const bool space )
    {
        // Getting exponent
        int exp = 0;

        while( x && fabs( x ) > 10 ){
            x /= 10;
            exp++;
        }

        while( x && fabs( x ) < 1 ){
            x *= 10;
            exp--;
        }

        if( exp ){
            return boost::str( boost::format( "%s\\times10^{%d}" )
                % FloatingPoint( x, precision, space )
                % exp
                );
        }
        else{
            return boost::str( boost::format( "%s" ) % FloatingPoint( x, precision, space ) );
        }
    }

    /*******************************************************************************
    *   Floating point formating options
    *******************************************************************************/
    string
    FloatingPoint( const Parameter& x, const int precision, const bool space )
    {
        string ans;
        const string cen = FloatingPoint( x.CentralValue(), precision, space );
        const string up  = FloatingPoint( x.AbsUpperError(), precision, space );
        const string lo  = FloatingPoint( x.AbsLowerError(), precision, space );
        boost::format symmfmt( "%s\\pm%s" );
        boost::format asymfmt( "%s^{+%s}_{-%s}" );

        if( x.AbsUpperError() == x.AbsLowerError() ){
            if( x.AbsUpperError() == 0. ){
                ans = cen;
            }
            else{
                ans = str( symmfmt % cen % up );
            }
        }
        else{
            ans = str( asymfmt % cen % up % lo );
        }

        return ans;
    }

    /******************************************************************************/

    string
    Scientific( const Parameter& x, const unsigned precision, const bool space )
    {
        boost::format symmbase( "%s\\pm%s" );
        boost::format asymbase( "%s^{+%s}_{-%s}" );
        boost::format symmexpfmt( "(%s)\\times10^{%d}" );
        boost::format asymexpfmt( "%s\\times10^{%d}" );
        string base;
        string ans;
        int exponent = 0;
        double cen   = x.CentralValue();
        double up    = x.AbsUpperError();
        double lo    = x.AbsLowerError();

        if( cen != 0 ){
            while( fabs( cen ) > 10 ){
                exponent++;
                cen /= 10;
                up  /= 10;
                lo  /= 10;
            }

            while( fabs( cen ) < 1. ){
                exponent--;
                cen *= 10;
                up  *= 10;
                lo  *= 10;
            }
        }
        else if( up != 0 || lo != 0 ){
            while( fabs( lo ) > 1 && fabs( up ) > 1 ){
                exponent++;
                cen /= 10;
                up  /= 10;
                lo  /= 10;
            }

            while( fabs( lo ) < 0.1 && fabs( up ) < 0.1 ){
                exponent--;
                cen *= 10;
                up  *= 10;
                lo  *= 10;
            }
        }

        const string censtr = FloatingPoint( cen, precision, space );
        const string upstr  = FloatingPoint( up, precision, space );
        const string lostr  = FloatingPoint( lo, precision, space );

        if( up == lo ){
            if( up == 0 ){
                base = censtr;
            }
            else{
                base = str( symmbase % censtr % upstr );
            }
        }
        else{
            base = str( asymbase % censtr % upstr % lostr );
        }

        if( exponent == 0 ){
            ans = base;
        }
        else{
            if( up == lo && up != 0 ){// Special case for symmetric errors
                ans = str( symmexpfmt % base % exponent );
            }
            else{
                ans = str( asymexpfmt % base % exponent );
            }
        }

        return ans;
    }

    /******************************************************************************/

    string
    HiggsDataCard( const Parameter& x, const double scale )
    {
        static const string null = "--";
        boost::format symmfmt( "%lf" );
        boost::format asymfmt( "%lf/%lf" );

        if( x.CentralValue() == 0 &&
            x.AbsUpperError() == 0 &&
            x.AbsLowerError() == 0 )
        {
            return null;
        }
        else if( x.AbsUpperError() == x.AbsLowerError() ){
            return str( symmfmt % ( 1 + scale * x.RelAvgError() ) );
        }
        else{
            return str( asymfmt % ( 1. + scale * x.RelUpperError() ) % ( 1. - scale * x.RelLowerError() ) );
        }

        return null;
    }
}/* mgr */
