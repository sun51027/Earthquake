/*******************************************************************************
*
*  Filename    : PackagePathMgr.cc
*  Description : One Line descrption of file contents
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "ManagerUtils/Common/interface/STLUtils.hpp"
#include "ManagerUtils/SysUtils/interface/PackagePathMgr.hpp"
#include "ManagerUtils/SysUtils/interface/PathUtils.hpp"

#include <boost/filesystem.hpp>
#include <cstdlib>
#include <exception>

using namespace std;
namespace fs = boost::filesystem;

namespace mgr{
    /*******************************************************************************
    *   Constructor and Destructor
    *******************************************************************************/
    PackagePathMgr::PackagePathMgr(
        const std::string& package,
        const std::string& sub_package,
        const std::string& settings_dir,
        const std::string& results_dir,
        const std::string& samples_dir
        ) :
        _package_name( package ),
        _sub_package_name( sub_package ),
        _settings_dir( settings_dir ),
        _results_dir( results_dir ),
        _samples_dir( samples_dir )
    {
    }

    PackagePathMgr::~PackagePathMgr()
    {
    }

    /*******************************************************************************
    *   Public functions
    *******************************************************************************/
    string
    PackagePathMgr::PackageDir() const
    {
        return PackagePath( _package_name );
        ;
    }

    /******************************************************************************/

    string
    PackagePathMgr::SubPackageDir() const
    {
        return SubPackagePath( _package_name, _sub_package_name );
    }

    /******************************************************************************/

    string
    PackagePathMgr::SubDir( const string& x ) const
    {
        return SubPackageDir() / x;
    }

    /******************************************************************************/

    string
    PackagePathMgr::SettingsDir() const
    {
        return SubDir( _settings_dir );
    }

    /******************************************************************************/

    string
    PackagePathMgr::ResultsDir() const
    {
        return SubDir( _results_dir );
    }

    /******************************************************************************/

    string
    PackagePathMgr::SamplesDir() const
    {
        return SubDir( _samples_dir );
    }
}/* mgr */
