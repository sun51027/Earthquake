#ifndef PARSERMGR_IPP
#define PARSERMGR_IPP



namespace opt = boost::program_options;
using namespace std;

namespace mgr {
    template < typename T >
    T
    Parsermgr::GetOption( const string& op )
    {
        if( CheckOption( op ) ){
            return _vm[ op ].as < T > ( );
                
        }
        else{ //return a null object
            return T();
        }
    }

    // http://stackoverflow.com/questions/1657883/variable-number-of-arguments-in-c
    template < typename T >
    void
    Parsermgr::SetFileName( initializer_list < T > list )
    {
        for( const auto& name : list ){
            if( CheckOption( name ) ){
                _namelist.push_back( name );
            }
        }
    }

    template < typename T >
    void
    Parsermgr::AddCutName( initializer_list < T > list )
    {
        for( const auto& name : list ){
            if( CheckOption( name ) ){
                _cutlist.push_back( name );
            }
        }
    }
}
#endif
