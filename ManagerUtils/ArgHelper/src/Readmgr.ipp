#ifndef READMGR_IPP
#define READMGR_IPP

#include "ManagerUtils/ArgHelper/interface/Common.hpp"

using namespace std;
using namespace boost::property_tree;

namespace mgr {
    template < typename T >
    T
    Readmgr::GetSingleData( const string& tag )
    {
        return GetSingle < T > ( tag, root );
    }

    template < typename T >
    T
    Readmgr::GetSubSingleData( const string& sub, const string& tag )
    {
        return GetSingle< T > ( tag, GetSubTree( sub ) );
    }

    template < typename T >
    vector < T > 
    Readmgr::GetListData( const string &tag )
    {
        return GetList < T > ( tag, root );
    }

    template < typename T >
    vector< T >
    Readmgr::GetSubListData( const string& sub, const string& tag )
    {
        return GetList< T > ( tag, GetSubTree( sub ) ) ;
    }
}
#endif
