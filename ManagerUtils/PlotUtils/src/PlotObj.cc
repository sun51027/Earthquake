/*******************************************************************************
 *
 *  Filename    : PlotObj.cc
 *  Description : Plotting Object declaration
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
 *******************************************************************************/

#include "../../PlotUtils/interface/Constants.hpp"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TPad.h"
#include "TPaveText.h"

namespace mgr {
/*******************************************************************************
 *   Pad settings functions
 *******************************************************************************/
static const float TOP_BOTTOM_SEP = 0.3;
static const float LEFT_RIGHT_SEP = 0.8;

TCanvas *NewCanvas()
{
  TCanvas *c = new TCanvas("c", "c", DEFAULT_CANVAS_WIDTH, DEFAULT_CANVAS_HEIGHT);
  c->SetTicks(1, 1);
  return c;
}

/******************************************************************************/

TPad *NewTopPad()
{
  TPad *pad = new TPad("toppad", "", 0, TOP_BOTTOM_SEP, 1., 1.0); // xlow ylow xup yup
  pad->SetTicks(1, 1);
  pad->SetBottomMargin(0.05);
  pad->SetLeftMargin(PLOT_X_MIN);
  pad->SetRightMargin(1 - PLOT_X_MAX);
  pad->SetTopMargin((1 - PLOT_Y_MAX) / (1 - TOP_BOTTOM_SEP));
  return pad;
}

/******************************************************************************/

TPad *NewBottomPad()
{
  TPad *pad = new TPad("botpad", "", 0, 0.00, 1, TOP_BOTTOM_SEP);
  pad->SetTicks(1, 1);
  pad->SetTopMargin(0.025);
  pad->SetLeftMargin(PLOT_X_MIN);
  pad->SetRightMargin(1 - PLOT_X_MAX);
  pad->SetBottomMargin((PLOT_Y_MIN) / (TOP_BOTTOM_SEP));
  return pad;
}

/******************************************************************************/

TPad *NewLeftPad()
{
  TPad *pad = new TPad("leftpad", "", 0., 0., LEFT_RIGHT_SEP, 1.);
  pad->SetTicks(1, 1);
  pad->SetRightMargin(0.025);
 // pad->SetTopMargin(PLOT_Y_MIN);
 // pad->SetBottomMargin(1 - PLOT_Y_MAX);
 // pad->SetLeftMargin((PLOT_X_MIN) / (LEFT_RIGHT_SEP));
  return pad;
}

/******************************************************************************/

TPad *NewRightPad()
{
  TPad *pad = new TPad("rightpad", "", LEFT_RIGHT_SEP, 0., 1., 1.);
  pad->SetTicks(1, 1);
//  pad->SetLeftMargin(0.1);
  //pad->SetLeftMargin(0.025);
  //pad->SetTopMargin(PLOT_Y_MIN);
  //pad->SetBottomMargin(0.9);
  //pad->SetBottomMargin(1 - PLOT_Y_MAX);
  //pad->SetRightMargin((PLOT_X_MIN) / (1-LEFT_RIGHT_SEP));
  return pad;
}

/******************************************************************************/
void SetSinglePad(TVirtualPad *pad)
{
  pad->SetTicks(1, 1);
  pad->SetLeftMargin(PLOT_X_MIN);
  // pad->SetLeftMargin( 0.2);
  pad->SetRightMargin(1 - PLOT_X_MAX);
  pad->SetBottomMargin(PLOT_Y_MIN);
  pad->SetTopMargin(1 - PLOT_Y_MAX);
}

/******************************************************************************/

void SetSinglePadWithPalette(TVirtualPad *pad)
{
  pad->SetLeftMargin(PLOT_X_MIN);
  pad->SetBottomMargin(PLOT_Y_MIN);
  pad->SetTopMargin(1 - PLOT_Y_MAX);
}

/*******************************************************************************
 *   New Legend settings
 *******************************************************************************/
TLegend *NewLegend(const float x_min, const float y_min, const float x_max, const float y_max)
{
  TLegend *ans = new TLegend(x_min, y_min, x_max, y_max);
  // Setting up default values
  ans->SetBorderSize(0);
  ans->SetTextFont(FONT_TYPE);
  ans->SetTextSize(TEXT_FONT_SIZE);
  ans->SetFillColorAlpha(0, 0);
  return ans;
}

/******************************************************************************/

TPaveText *NewTextBox(const float x_min, const float y_min, const float x_max, const float y_max)
{
  TPaveText *ans = new TPaveText(x_min, y_min, x_max, y_max, "NDC");
  ans->SetTextFont(FONT_TYPE);
  ans->SetTextSize(TEXT_FONT_SIZE);
  ans->SetFillColor(kWhite);
  return ans;
}

/*******************************************************************************
 *   TH1D division functions
 *******************************************************************************/
TH1D *DivideHist(TH1D *num, TH1D *den, const double cen)
{
  TH1D *ans = (TH1D *)(num->Clone());
  ans->Divide(den);

  for (int i = 0; i < num->GetNcells(); ++i) {
    if (num->GetBinContent(i) == 0 && den->GetBinContent(i) == 0) {
      ans->SetBinContent(i, cen);
    }
  }

  return ans;
}

/*******************************************************************************
 *   TGraph division functions
 *******************************************************************************/
TGraph *DividedGraphSimple(TGraph *num, TGraph *den)
{
  TGraph *ans = new TGraph(num->GetN());

  for (int i = 0; i < num->GetN(); ++i) {
    const double origx = num->GetX()[i];
    const double origy = num->GetY()[i];
    const double deny  = den->Eval(origx);

    if (fabs(deny) > 0.0000001) {
      ans->SetPoint(i, origx, origy / deny);
    } else {
      ans->SetPoint(i, origx, 1);
    }
  }

  ans->SetTitle("");
  return ans;
}

/******************************************************************************/

TGraphAsymmErrors *DividedGraph(TGraphAsymmErrors *num, TGraph *den)
{
  TGraphAsymmErrors *ans = new TGraphAsymmErrors(num->GetN());

  for (int i = 0; i < num->GetN(); ++i) {
    const double origx  = num->GetX()[i];
    const double origy  = num->GetY()[i];
    const double xerrlo = num->GetErrorXlow(i);
    const double xerrhi = num->GetErrorXhigh(i);
    const double yerrlo = num->GetErrorYlow(i);
    const double yerrhi = num->GetErrorYhigh(i);
    const double deny   = den->Eval(origx);

    if (deny > 0.0000001) {
      ans->SetPoint(i, origx, origy / deny);
      ans->SetPointError(i, xerrlo, xerrhi, yerrlo / deny, yerrhi / deny);
    } else {
      ans->SetPoint(i, origx, 1);
      ans->SetPointError(i, xerrlo, xerrhi, 0, 0);
    }
  }

  ans->SetTitle("");
  return ans;
}

/******************************************************************************/

TGraphAsymmErrors *PullGraph(TGraph *numgraph, TGraphAsymmErrors *dengraph)
{
  TGraphAsymmErrors *ans = new TGraphAsymmErrors(dengraph->GetN());

  for (int i = 0; i < dengraph->GetN(); ++i) {
    const double x    = dengraph->GetX()[i];
    const double y    = dengraph->GetY()[i];
    const double numy = numgraph->Eval(x);
    const double pull = y == 0     ? 0
                        : numy > y ? (numy - y) / dengraph->GetErrorYhigh(i)
                                   : (numy - y) / dengraph->GetErrorYlow(i);
    ans->SetPoint(i, x, pull);
    ans->SetPointError(i, dengraph->GetErrorX(i), dengraph->GetErrorX(i), 0, 0 // No y error
    );
  }

  return ans;
}
} // namespace mgr
