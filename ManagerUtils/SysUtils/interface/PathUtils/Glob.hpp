/*******************************************************************************
*
*  Filename    : Glob.hpp
*  Description : Globbing related functions
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*  Main implementation in src/PathUtils_Glob.cc
*
*******************************************************************************/
#ifndef MANAGERUTILS_SYSUTILS_PATHUTILS_GLOB_HPP
#define MANAGERUTILS_SYSUTILS_PATHUTILS_GLOB_HPP

#include <vector>
#include <string>

namespace mgr {
/*******************************************************************************
*   * Glob
*   Master function that most users will directly use. It will attempt to expand
*   the given glob query to a vector<string> of all the paths that match the
*   discription.
*
*   If the format is in the given in the form of:
*   root://<someurl>//some/path
*   If will attempt to glob remote files. More details below
*******************************************************************************/
extern std::vector<std::string> Glob( const std::string& );

/*******************************************************************************
*   * GlobLocal
*   Returns the globbing results from the local file system. Directly powered
*   by the POSIX library
*******************************************************************************/
extern std::vector<std::string> GlobLocal( const std::string& );

/*******************************************************************************
*   * GlobRemote
*   Returns the globbing results from a remote file system. The expected input
*   would be something like:
*
*   root:://<someurl>//some/path/with/glob
*
*   This function will effectively return the reults of the command
*
*   xrdfs <someurl> ls /some/path/with/glob
*
*   As if xrdfs has globbing (which it doesn't)
*
*******************************************************************************/
extern std::vector<std::string> GlobRemote( const std::string& );

/*******************************************************************************
*   * GlobToRegex
*   Changing a Glob notation such as "/test/file/a*.root" to a regular expression
*   string "\/test\/file\/a.*\.root" for custom string matching
*******************************************************************************/
extern std::string GlobToRegex( const std::string& );

/*******************************************************************************
*   * IsRemotePath
*   Boolean check for string format
*******************************************************************************/
extern bool IsRemotePath( const std::string& );

/*******************************************************************************
*   * GetServerURL
*   For a input of root://<someurl>//<somestring>, returns <someurl>
*******************************************************************************/
extern std::string GetServerURL( const std::string& );

/*******************************************************************************
*   GetRemotePath
*   For a input rot root://someurl//somestring, returns /somestrings
*******************************************************************************/
extern std::string GetRemotePath( const std::string& );

} /* mgr */



#endif /* end of include guard: MANAGERUTILS_SYSUTILS_PATHUTILS_GLOB_HPP */
