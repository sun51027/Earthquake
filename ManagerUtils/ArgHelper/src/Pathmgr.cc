// Get directly from Enoch
// should modify later


#include "ManagerUtils/ArgHelper/interface/Common.hpp"
#include "ManagerUtils/ArgHelper/interface/Pathmgr.hpp"

using namespace std;

namespace mgr{
    /*******************************************************************************
    *   Constructor and Destructor
    *******************************************************************************/
    Pathmgr::Pathmgr(
        const std::string& package,
        const std::string& sub_package,
        const std::string& settings_dir,
        const std::string& results_dir,
        const std::string& datas_dir
        ) :
        _package_name( package ),
        _sub_package_name( sub_package ),
        _settings_dir( settings_dir ),
        _results_dir( results_dir ),
        _datas_dir( datas_dir )
    {
    }

    Pathmgr::~Pathmgr()
    {
    }

    string
    Pathmgr::PackageDir() const
    {
        return PackagePath( _package_name );
        ;
    }

    string
    Pathmgr::SubPackageDir() const
    {
        return SubPackagePath( _package_name, _sub_package_name );
    }

    string
    Pathmgr::SubDir( const string& x ) const
    {
        return SubPackageDir() / x;
    }

    string
    Pathmgr::SettingsDir() const
    {
        return SubDir( _settings_dir );
    }

    string
    Pathmgr::ResultsDir() const
    {
        return SubDir( _results_dir );
    }

    string
    Pathmgr::DatasDir() const
    {
        return SubDir( _datas_dir );
    }
}
