/*******************************************************************************
*
*  Filename    : Named.cc
*  Description : Implmentation to convertion functions in Named class
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/

#include "ManagerUtils/Common/interface/Named.hpp"

using namespace std;

namespace mgr{
    /*******************************************************************************
    *   Constructor and Destructor
    *******************************************************************************/
    Named::Named( const string& x ) :
        _name( x ),
        _latexname( "" ),
        _rootname( "" )
    {
    }

    /******************************************************************************/

    Named::~Named()
    {
    }

    /*******************************************************************************
    *   Basic access functions
    *******************************************************************************/
    const std::string&
    Named::Name() const
    {
        return _name;
    }

    /******************************************************************************/

    const std::string&
    Named::LatexName() const
    {
        return _latexname;
    }

    /******************************************************************************/

    const std::string&
    Named::RootName() const
    {
        return _rootname;
    }

    /*******************************************************************************
    *   Setting functions
    *******************************************************************************/

    void
    Named::SetLatexName( const string& x )
    {
        _latexname = x;
    }

    /******************************************************************************/

    void
    Named::SetRootName( const string& x )
    {
        _rootname = x;
    }
}/* mgr */
