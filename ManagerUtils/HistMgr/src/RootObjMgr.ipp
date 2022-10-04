#include <iostream>

namespace mgr {
    /*******************************************************************************
    *   Constructor and destructor
    *******************************************************************************/
    template < typename T >
    RootObjMgr < T > ::RootObjMgr()
    {
    }

    /******************************************************************************/

    template < typename T >
    RootObjMgr < T > ::~RootObjMgr()
    {
    }

    /*******************************************************************************
    *   Addition and removal functions
    *******************************************************************************/
    template < typename T >
    void
    RootObjMgr < T > ::AddObj( T * obj )
    {
                          // name                // sample_name  // sample
        const std::string store = MakeStoreName( obj->GetName(), obj->GetTitle() );

        if( store != obj->GetName() ){
            obj->SetTitle( "" );
        }

        _objmap.erase( store );// deleting existing instance if already exist
        _objmap[ store ] = std::unique_ptr < T > ( obj );
    }

    /******************************************************************************/

    template < typename T >
    void
    RootObjMgr < T > ::RemoveObj( const std::string& name )
    {
        if( _objmap.count( name ) ){
            _objmap.erase( name );
        }
        else{
        }
    }

    /*******************************************************************************
    *   Object access functions
    *******************************************************************************/
    template < typename T >
    T *
    RootObjMgr < T > ::GetObj( const std::string& name )
    {
        if( _objmap.count( name ) ){
            return _objmap.at( name ).get();
        }
        else{
            return NULL;
        }
    }

    /******************************************************************************/

    template < typename T >
    const T *
    RootObjMgr < T > ::GetObj( const std::string& name ) const
    {
        if( _objmap.count( name ) ){
            return _objmap.at( name ).get();
        }
        else{
            return NULL;
        }
    }

    /*******************************************************************************
    *   Object listing functions
    *******************************************************************************/
    template < typename T >
    std::vector < std::string >
    RootObjMgr < T > ::ObjNameList() const
    {
        std::vector < std::string > ans;

        for( const auto& mypair : _objmap ){
            ans.push_back( mypair.first );
        }

        return ans;
    }

    /******************************************************************************/

    template < typename T >
    std::vector < T * >
    RootObjMgr < T > ::GetObjContains( const std::string& substring )
    {
        std::vector < T * > ans;

        for( auto& mypair : _objmap ){
            if( mypair.first.find( substring ) != std::string::npos ){
                ans.push_back( mypair.second.get() );
            }
        }

        return ans;
    }

    /*******************************************************************************
    *   Naming convention functions
    *******************************************************************************/
    template < typename T >
    std::string
    RootObjMgr < T > ::MakeStoreName( const std::string& objname, const std::string& tag ) const
    {
        std::regex pattern( tag + "_" );

        return std::regex_replace( objname, pattern, "" );
    }
}/* mgr */
