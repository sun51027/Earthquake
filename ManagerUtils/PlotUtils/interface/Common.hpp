/*******************************************************************************
*
*  Filename    : Common.hpp
*  Description : Typical helper setups for plotting objects
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*  Making it easier to comply with standards in
*   https://ghm.web.cern.ch/ghm/plots/
*
*******************************************************************************/
#ifndef MANAGERUTILS_PLOTUTILS_COMMON_HPP
#define MANAGERUTILS_PLOTUTILS_COMMON_HPP

/*******************************************************************************
*   Including commong ROOT plotting objects
*******************************************************************************/
#include "TCanvas.h"
#include "TColor.h"
#include "TFile.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "THStack.h"
#include "TLegend.h"
#include "TLine.h"
#include "TMultiGraph.h"
#include "TPad.h"
#include "TPaveText.h"

/*******************************************************************************
*   Including custom defined functions
*******************************************************************************/
#include "Constants.hpp"
#include "DrawCMS.hpp"
#include "GetRange.hpp"
#include "LatexMgr.hpp"
#include "PlotObj.hpp"
#include "SaveUtils.hpp"
#include "SetAxis.hpp"
#include "../src/SetAxis.ipp"
#include "../../SysUtils/interface/PathUtils.hpp"

#endif/* end of include guard: MANAGERUTILS_PLOTUTILS_COMMON_HPP */
