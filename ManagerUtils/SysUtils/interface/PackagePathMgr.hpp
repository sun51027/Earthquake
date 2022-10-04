/*******************************************************************************
*
*  Filename    : PackagePathMgr.hpp
*  Description : Base class for handling file names in subpackages
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef MANAGERUTILS_SYSUTILS_FILENAMEMGR_HPP
#define MANAGERUTILS_SYSUTILS_FILENAMEMGR_HPP

#include <map>
#include <string>

namespace mgr{
    class PackagePathMgr {
        public:

            PackagePathMgr(
                const std::string& package,
                const std::string& sub_package,
                const std::string& settings_dir = "settings",
                const std::string& results_dir  = "results",
                const std::string& samples_dir  = "samples"
                );
            virtual ~PackagePathMgr ();

            // Instance functions for getting file names
            std::string PackageDir()    const;
            std::string SubPackageDir() const;

            // Default settings for sample
            std::string SettingsDir() const;
            std::string ResultsDir()  const;
            std::string SamplesDir()  const;
            std::string SubDir( const std::string& ) const;

        private:

            const std::string _package_name;
            const std::string _sub_package_name;
            const std::string _settings_dir;
            const std::string _results_dir;
            const std::string _samples_dir;
    };
}

#endif/* end of include guard: MANAGERUTILS_SYSUTILS_FILENAMEMGR_HPP */
