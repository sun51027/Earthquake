/*******************************************************************************
*
*  Filename    : Parameter.cc
*  Description : Implmentation of arithmetics for Parameter class
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "ManagerUtils/Maths/interface/Parameter.hpp"

#include <iostream>

using namespace std;

namespace mgr{
    /*******************************************************************************
    *   Constructor And Destructor
    *******************************************************************************/
    Parameter::Parameter()
    {
        _centralValue = 0;
        _error_up     = 0;
        _error_down   = 0;
    }

    /******************************************************************************/

    Parameter::Parameter(
        const double c,
        const double error_up,
        const double error_down ) :
        _centralValue( c ),
        _error_up( error_up ),
        _error_down( error_down )
    {
        if( _error_up < 0 ){
            cerr << "Warning! Upper error is small than zero! Assuming flipped sign: "
                 << c << " " << error_up << " " << error_down
                 << endl;
            _error_up = -_error_up;
        }

        if( _error_down < 0 ){
            cerr << "Warning! Lower error is small than zero! Assuming flipped sign: "
                 << c << " " << error_up << " " << error_down
                 << endl;
            _error_down = -_error_down;
        }
    }

    /******************************************************************************/

    Parameter::Parameter( const Parameter& x )
    {
        *this = x;
    }

    /******************************************************************************/

    Parameter::Parameter( const RooRealVar& x ) :
        _centralValue( x.getVal() ),
        _error_up( x.getErrorHi() ),
        _error_down( x.getErrorLo() )
    {
    }

    /******************************************************************************/

    Parameter::~Parameter(){}


    /*******************************************************************************
    *   Assignment operator
    *******************************************************************************/
    Parameter&
    Parameter::operator=( const Parameter& x )
    {
        _centralValue = x._centralValue;
        _error_up     = x._error_up;
        _error_down   = x._error_down;
        return *this;
    }

    /*******************************************************************************
    *   Arithmetics operators, call functions defined in ParameterArithmetic.hpp
    *******************************************************************************/
    Parameter
    Parameter::NormParam() const
    {
        return Parameter( 1,
            RelUpperError(),
            RelLowerError()
            );
    }

    /*******************************************************************************
    *   Parameter - Parameter arithmetics
    *   Call functions defined in ParameterArithmetic.hpp
    *******************************************************************************/
    Parameter&
    Parameter::operator+=( const Parameter& x )
    {
        *this = Sum( *this, x );
        return *this;
    }

    /******************************************************************************/

    Parameter&
    Parameter::operator*=( const Parameter& x )
    {
        *this = Prod( *this, x );
        return *this;
    }

    /******************************************************************************/

    Parameter
    Parameter::operator+( const Parameter& x ) const
    {
        return Sum( *this, x );
    }

    /******************************************************************************/

    Parameter
    Parameter::operator*( const Parameter& x ) const
    {
        return Prod( *this, x );
    }

    /*******************************************************************************
    *   Parameter - double arithmetics
    *   Required for Parameter Arithmetics
    *******************************************************************************/
    Parameter&
    Parameter::operator*=( const double x )
    {
        _centralValue *= x;
        _error_up     *= fabs( x );
        _error_down   *= fabs( x );
        return *this;
    }

    /******************************************************************************/

    Parameter&
    Parameter::operator/=( const double x )
    {
        _centralValue /= x;
        _error_up     /= fabs( x );
        _error_down   /= fabs( x );
        return *this;
    }

    /******************************************************************************/

    Parameter
    Parameter::operator*( const double x ) const
    {
        Parameter ans( *this );
        ans *= x;
        return ans;
    }

    /******************************************************************************/

    Parameter
    Parameter::operator/( const double x ) const
    {
        Parameter ans( *this );
        ans /= x;
        return ans;
    }

    /******************************************************************************/

    Parameter
    operator*( const double y, const Parameter& x )
    {
        return x * y;
    }

    /******************************************************************************/

    Parameter
    operator/( const double y, const Parameter& x )
    {
        const double centralValue = y / x._centralValue;
        const double err_up       = centralValue * x.RelUpperError();
        const double err_dw       = centralValue * x.RelLowerError();
        return Parameter( centralValue, err_up, err_dw );
    }
}/* mgr */
