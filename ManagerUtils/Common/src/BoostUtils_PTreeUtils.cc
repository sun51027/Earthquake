/*******************************************************************************
*
*  Filename    : BoostUtils_PTreeUtils.cc
*  Description : Implementation of non template functions
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*
*******************************************************************************/
#include "ManagerUtils/Common/interface/BoostUtils/PTreeUtils.hpp"
#include "ManagerUtils/Maths/interface/Parameter/Parameter.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>

using namespace std;
using boost::property_tree::ptree;


/******************************************************************************/
ptree
mgr::FromJsonFile( const std::string& filename )
{
    ptree ans;
    boost::property_tree::read_json( filename, ans );
    return ans;
}

/******************************************************************************/

void
mgr::PrintPTree( const boost::property_tree::ptree& tree, unsigned level )
{
    for( const auto& it : tree ){
        for( unsigned i = 0; i < level; ++i ){
            cout << "\t" << flush;
        }

        cout << it.first << ": " << it.second.get_value<std::string>() << endl;
        PrintPTree( it.second, level + 1 );
    }
}

/*******************************************************************************
*
*******************************************************************************/
std::string
mgr::MakeQueryString( const std::vector<std::string>& list )
{
    return boost::join( list, "." );
}

/******************************************************************************/

bool
mgr::CheckQuery( const boost::property_tree::ptree& tree, const std::string& query )
{
    return tree.find( query ) != tree.not_found();
}

/*******************************************************************************
*   Template specialization
*******************************************************************************/
namespace mgr{
    template<>
    Parameter
    GetSingle<Parameter>( const boost::property_tree::ptree& tree, const std::string& query )
    {
        std::vector<double> input = GetList<double>( tree, query );
        input.resize( 3, 0 );

        if( input[ 0 ] == 0 ){
            input[ 0 ] = 1;
        }

        if( input[ 1 ] != 0 && input[ 2 ] == 0 ){
            input[ 2 ] = input[ 1 ];
        }

        return Parameter( input[ 0 ], input[ 1 ], input[ 2 ] );
    }
}/* mgr */
