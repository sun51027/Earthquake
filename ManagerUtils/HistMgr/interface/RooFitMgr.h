/*******************************************************************************
*
*  Filename    : RooFitMgr.h
*  Description : Class for manager RooFit object across multiple class instances
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef MANAGERUTILS_ROOTMGR_ROOFITMGR_H
#define MANAGERUTILS_ROOTMGR_ROOFITMGR_H

#include "ManagerUtils/HistMgr/interface/RootObjMgr.h"

#include "RooAbsData.h"
#include "RooAbsPdf.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"
#include <string>
#include <vector>

namespace mgr{
    class RooFitMgr {
        public:

            RooFitMgr( const std::string& );
            virtual ~RooFitMgr ();

            RooFitMgr( const RooFitMgr& )                  = delete;
            const RooFitMgr& operator=( const RooFitMgr& ) = delete;

            // Static methods only exists for RooRealVar
            static RooRealVar* StaticNewVar(
                const std::string& name,
                const std::string& title,
                const std::string& unit,
                const double       min,
                const double       max
                );
            static RooRealVar*              StaticVar( const std::string& );
            static std::vector<std::string> StaticVarNameList();

            // RooRealVar Access member functions
            RooRealVar*              NewVar( const std::string&, const double, const double );
            RooRealVar*              NewVar( const std::string&, const double, const double, const double );
            RooRealVar*              Var( const std::string& );
            const RooRealVar*        Var( const std::string& ) const;
            std::vector<std::string> VarNameList() const;
            std::vector<RooRealVar*> VarContains( const std::string& );
            void                     SetConstant( const bool set = kTRUE );
            void                     RemoveVar( const std::string& );

            // DataSet access members
            RooAbsData*              DataSet( const std::string& );
            const RooAbsData*        DataSet( const std::string& ) const;
            std::vector<std::string> SetNameList() const;
            void                     AddDataSet( RooAbsData* );
            void                     RemoveDataSet( const std::string& );

            // PDF making functions
            void                     AddPdf( RooAbsPdf* );
            RooAbsPdf*               Pdf( const std::string& );
            const RooAbsPdf*         Pdf( const std::string& ) const;
            std::vector<std::string> PdfNameList() const;
            void                     RemovePdf( const std::string& );

            // Function access members
            void                     AddFunc( RooAbsReal* );
            RooAbsReal*              Func( const std::string& );
            const RooAbsReal*        Func( const std::string& ) const;
            std::vector<std::string> FuncNameList() const;

        private:

            std::string _tag;

            static RootObjMgr<RooRealVar> _staticvarmgr;

            RootObjMgr<RooRealVar> _varmgr;
            RootObjMgr<RooAbsData> _setmgr;
            RootObjMgr<RooAbsPdf> _pdfmgr;
            RootObjMgr<RooAbsReal> _funcmgr;
    };
};


#endif/* end of include guard: MANAGERUTILS_ROOTMGR_ROOFITMGR_H */
