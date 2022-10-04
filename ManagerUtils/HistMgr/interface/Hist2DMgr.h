#ifndef MANAGERUTILS_ROOTMGR_HIST2DMGR_H
#define MANAGERUTILS_ROOTMGR_HIST2DMGR_H

#include "ManagerUtils/HistMgr/interface/RootObjMgr.h"

#include "TH2D.h"
#include <string>
#include <vector>

namespace mgr{
    class Hist2DMgr {
        public:

            Hist2DMgr( const std::string& );// Must have unique name
            virtual ~Hist2DMgr ();

            // Explicitly removing copy constructor
            Hist2DMgr( const Hist2DMgr& )            = delete;
            Hist2DMgr& operator=( const Hist2DMgr& ) = delete;

            TH2D*       Hist2D( const std::string& );
            const TH2D* Hist2D( const std::string& ) const;

            std::vector<std::string> AvailableHist2DList() const;

            void AddHist2D(
                const std::string&,
                const std::string&,
                const std::string&,
                const unsigned,
                const double,
                const double,
                const unsigned,
                const double,
                const double
                );

        private:

            std::string _tag;
            RootObjMgr<TH2D> _histmgr;
    };
};

#endif/* end of include guard: MANAGERUTILS_ROOTMGR_HIST2DMGR_HPP */
