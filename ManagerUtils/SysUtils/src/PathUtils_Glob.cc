/*******************************************************************************
*
*  Filename    : PathUtils_Glob.cc
*  Description : Implementation of globbing functions
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "ManagerUtils/Common/interface/STLUtils.hpp"
#include "ManagerUtils/SysUtils/interface/PathUtils/CommonPath.hpp"
#include "ManagerUtils/SysUtils/interface/PathUtils/Glob.hpp"
#include "ManagerUtils/SysUtils/interface/ProcessUtils.hpp"

#include <algorithm>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <glob.h>

using namespace std;

namespace mgr{
    /*******************************************************************************
    *   Master functions
    *******************************************************************************/
    vector<string>
    Glob( const string& path )
    {
        if( IsRemotePath( path ) ){
            return GlobRemote( path );
        }
        else{
            return GlobLocal( path );
        }
    }

    /*******************************************************************************
    *   Local Globbing powered by POSIX <glob.h>
    *******************************************************************************/
    vector<string>
    GlobLocal( const string& path )
    {
        glob_t glob_result;
        glob( path.c_str(), GLOB_TILDE, NULL, &glob_result );
        vector<string> ret( glob_result.gl_pathv, glob_result.gl_pathv + glob_result.gl_pathc );
        globfree( &glob_result );
        return ret;
    }

    /******************************************************************************/

    vector<string>
    GlobRemote( const string& fullpath )
    {
        vector<string> ans;
        const string remoteurl  = GetServerURL( fullpath );
        const string remotepath = GetRemotePath( fullpath );
        // cout << remoteurl << " " << remotepath << endl;
        // Breaking up original directory query by directory
        vector<string> tokens;
        boost::split( tokens, remotepath, boost::is_any_of( "/" ) );
        // Removing any blank entry in path sections
        tokens.erase( std::remove( tokens.begin(), tokens.end(), "" ), tokens.end() );
        // Adding root '/' to the first entry
        tokens.front() = '/' + tokens.front();

        // Truncated intermediate directory that don't require globbing.
        for( unsigned index = 1; index < tokens.size(); ++index ){
            const string prev = tokens[ index - 1 ];
            const string pres = tokens[ index ];

            if( GlobToRegex( prev ) == prev && GlobToRegex( pres ) == pres ){
                tokens[ index - 1 ] = prev / pres;
                tokens.erase( tokens.begin() + index );
                --index;
            }
        }

        // Preparing vector for BFS searching
        vector<std::pair<std::string, unsigned> > querylist;
        querylist.emplace_back( tokens.front(), 1 );// pushing first element int

        for( unsigned i = 0; i < querylist.size(); ++i ){
            const string querydir     = querylist[ i ].first;
            const unsigned querydepth = querylist[ i ].second;

            if( querydepth >= tokens.size() ){
                continue;
            }

            const string cmd = str( boost::format( "xrdfs %1% ls '%2%'" ) % remoteurl % querydir );
            // cout << cmd << endl;
            // Cannot be const
            string rawoutput = GetCMDOutput( cmd );
            vector<string> outlist;
            boost::split( outlist, rawoutput, boost::is_any_of( "\n\t " ) );
            outlist.erase( std::remove( outlist.begin(), outlist.end(), "" ), outlist.end() );
            const string globpattern = querydir + '/' + tokens.at( querydepth );
            std::regex globregex( GlobToRegex( globpattern ) );

            for( const auto& out : outlist ){
                if( std::regex_match( out, globregex ) ){
                    querylist.emplace_back( out, querydepth + 1 );
                }
            }
        }

        std::regex finalglob( GlobToRegex( remotepath ) );

        for( const auto& querypair : querylist ){
            if( std::regex_match( querypair.first, finalglob ) ){
                const std::string fullfilepath = str( boost::format( "root://%s/%s" ) % remoteurl % querypair.first );
                ans.push_back( fullfilepath );
            }
        }

        return ans;
    }

    /*******************************************************************************
    *   Glob - regex translation function
    *******************************************************************************/
    std::string
    GlobToRegex( const std::string& query )
    {
        static const std::regex starmatch( "\\*" );
        static const std::string starrep( ".*" );
        static const std::regex qmmatch( "\\?" );
        static const std::string qmrep( "." );
        std::string ans = query;
        ans = std::regex_replace( ans, starmatch, starrep );
        ans = std::regex_replace( ans, qmmatch, qmrep );
        return ans;
    }

    /*******************************************************************************
    *   IsRemotePath
    *******************************************************************************/
    bool
    IsRemotePath( const std::string& path )
    {
        static const std::string remoteprefix = "root://";

        if( std::mismatch( remoteprefix.begin(), remoteprefix.end(), path.begin() ).first == remoteprefix.end() ){
            return true;
        }
        else{
            return false;
        }
    }

    /*******************************************************************************
    *   Remote Helper functions
    *******************************************************************************/
    std::string
    GetServerURL( const std::string& remotepath )
    {
        static const std::regex urlregex( "root:\\/\\/([a-z.0-9]+)\\/\\/.*" );
        std::smatch urlmatch;

        if( std::regex_match( remotepath, urlmatch, urlregex ) ){
            if( urlmatch.size() == 2 ){
                return urlmatch[ 1 ].str();
            }
            else{
                throw std::invalid_argument( "Input contains more than one url!" );
                return "";
            }
        }
        else{
            throw std::invalid_argument( "Input does not contain valid server url!" );
            return "";
        }
    }

    /******************************************************************************/

    std::string
    GetRemotePath( const std::string& remotepath )
    {
        static const std::regex pathregex( "root:\\/\\/[a-z.0-9]+\\/(\\/.+)" );
        std::smatch pathmatch;

        if( std::regex_match( remotepath, pathmatch, pathregex ) ){
            if( pathmatch.size() == 2 ){
                return pathmatch[ 1 ].str();
            }
            else{
                throw std::invalid_argument( "Input doesn't give a remote path!" );
                return "";
            }
        }
        else{
            throw std::invalid_argument( "Input doesn't give a remote path!" );
            return "";
        }
    }
}/* mgr */
