#include "ManagerUtils/ArgHelper/interface/Common.hpp"
#include "ManagerUtils/ArgHelper/interface/Readmgr.hpp"

using namespace std;
namespace pt = boost::property_tree;

namespace mgr{
    Readmgr::Readmgr( const string& file )
    {
        std::size_t found = file.find( "json" );
        if( found != std::string::npos ){
            ReadFile( file, root );
        }
        else{
            printf( "Using without reading JSON\n" );
        }
    }

    pt::ptree
    Readmgr::GetSubTree( const string& t )
    {
        return root.get_child( t );
    }

    void
    Readmgr::ChangeJSON( const string& file )
    {
        ReadFile( file, root );
    }
}
