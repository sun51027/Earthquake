/*******************************************************************************
*
*  Filename    : SaveUtils.hpp
*  Description : Saving utils to combat ROOT's sub-par PDF outputs
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef MANAGERUTILS_PLOTUTILS_SAVEUTILS
#define MANAGERUTILS_PLOTUTILS_SAVEUTILS

#include "TLorentzVector.h"
#include "TCanvas.h"
#include "TH1.h"

namespace mgr{
    /*******************************************************************************
    *   SaveToPDF - saving TCanvas to PDF file, invokes PDF correction ghostscript
    *******************************************************************************/
    void SaveToPDF( TCanvas*, const std::string& );

    /*******************************************************************************
    *   SaveToRoot - saving TCanvas to ROOT File
    *******************************************************************************/
    void SaveToROOT( TCanvas*, const std::string&, const std::string& );

    void SaveToROOT( TH1*, const std::string&, const std::string&);
}


#endif/* end of include guard: MANAGERUTILS_PLOTUTILS_SAVEUTILS */
