#include "ManagerUtils/ArgHelper/interface/Parsermgr.hpp"
#include <boost/exception/diagnostic_information.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>

namespace opt = boost::program_options;
using namespace std;

namespace mgr{
    Parsermgr::Parsermgr()
    {
        _desc.add_options()
            ( "help,h", "print help options and exit program" )
        ;
    }

    Parsermgr&
    Parsermgr::AddOptions( const opt::options_description& de )
    {
        _desc.add( de );
        return *this;
    }

    int
    Parsermgr::ParseOptions( int argc, char* argv[] )
    {
        try {
            opt::store( opt::parse_command_line( argc, argv, _desc ), _vm );
            opt::notify( _vm );
        }
        catch( boost::exception& e ){
            cerr << "Error parsing command!" << endl;
            cerr << boost::diagnostic_information( e );
            cerr << _desc << endl;
            return FAIL_PARSER;
        }

        if( _vm.count( "help" ) ){
            cout << _desc << endl;
            return HELP_PARSER;
        }

        return PASS_PARSER;
    }

    bool
    Parsermgr::CheckOption( const string& option )
    {
        return _vm.count( option );
    }

    string
    Parsermgr::OptName()
    {
        vector<string> taglist;

        for( const auto& opt : _namelist ){
            string ans = "";

            if( ans == "" ){
                try {
                    ans = GetOption<string>( opt );
                }
                catch( ... ){
                }
            }

            if( ans == "" ){
                try {
                    ans = boost::lexical_cast<string>( GetOption<int>( opt ) );
                }
                catch( ... ){
                }
            }

            if( ans == "" ){
                try {
                    ans = boost::lexical_cast<string>( GetOption<double>( opt ) );
                }
                catch( ... ){
                }
            }

            taglist.push_back( ans );
        }

        for( const auto& opt : _cutlist ){
            string ans = "";

            if( ans == "" ){
                try {
                    ans = GetOption<string>( opt );
                }
                catch( ... ){
                }
            }

            if( ans == "" ){
                try {
                    ans = boost::lexical_cast<string>( GetOption<int>( opt ) );
                }
                catch( ... ){
                }
            }

            if( ans == "" ){
                try {
                    ans = boost::lexical_cast<string>( GetOption<double>( opt ) );
                }
                catch( ... ){
                }
            }

            if( ans == "" ){
                taglist.push_back( opt );
            }
            else{
                taglist.push_back( opt + "_" + ans );
            }
        }

        return CustomName( taglist );
    }

    string
    Parsermgr::CustomName( const vector<string>& taglist )
    {
        string ans = "";

        for( const auto& name : taglist ){
            ans += ( "_" + name );
        }

        return ans;
    }

    void
    Parsermgr::GetOptlist()
    {
        for( const auto& it : _vm ){
            cout << it.first.c_str() << " ";
        }
    }
}
