#ifndef COMMON_IPP
#define COMMON_IPP

using namespace std;
using namespace boost::property_tree;

namespace mgr {
    template < typename T >
    T
    GetSingle( const string& tag, const ptree& root )
    {
        return root.get < T > ( tag );
    }

    template < typename T >
    vector < T > GetList( const string &tag, const ptree &root ){
        vector < T > ans;

        for( auto& n : root.get_child( tag ) ){
            ans.push_back( n.second.get_value < T > ( ) );
        }

        return ans;
    }
}
#endif
