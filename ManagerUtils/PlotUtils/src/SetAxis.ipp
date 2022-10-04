/*******************************************************************************
 *
 *  Filename    : SetAxis.ipp
 *  Description : Implements template functions from SetAxis.hpp
 *  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
 *
 *******************************************************************************/
#ifndef MANAGERUTILS_PLOTUTILS_SETAXIS_IPP
#define MANAGERUTILS_PLOTUTILS_SETAXIS_IPP
#include <iostream>
#include <string>
using namespace std;
template <class T>
void mgr::SetAxis(T *plot)
{
  // Global settings from TGaxis
  TGaxis::SetMaxDigits(4);
  TGaxis::SetExponentOffset(-1000, -1000);
  plot->GetXaxis()->SetLabelFont(FONT_TYPE);
  plot->GetXaxis()->SetTitleFont(FONT_TYPE);
  plot->GetYaxis()->SetLabelFont(FONT_TYPE);
  plot->GetYaxis()->SetTitleFont(FONT_TYPE);
  plot->GetXaxis()->SetLabelSize(AXIS_LABEL_FONT_SIZE);
  plot->GetXaxis()->SetTitleSize(AXIS_TITLE_FONT_SIZE);
  plot->GetYaxis()->SetLabelSize(AXIS_LABEL_FONT_SIZE);
  plot->GetYaxis()->SetTitleSize(AXIS_TITLE_FONT_SIZE);
  plot->GetYaxis()->SetTitleOffset(1.2);

  if (plot->GetMaximum() >= 10000) {
    string YTitle;
    YTitle = plot->GetYaxis()->GetTitle() + std::string(" x 10^{3}");
    plot->GetYaxis()->SetTitle(YTitle.c_str());
  }

  /* if(plot->GetXaxis()->GetXmin()-1000 <=0.001||plot->GetXaxis()->GetXmin()-2000
 <=0.001||plot->GetXaxis()->GetXmin()-3000 <=0.001){
 //TGaxis::
 plot->GetXaxis()->SetNdivisions(-502);
 plot->GetXaxis()->ChangeLabel(1,-1,-1,-1,-1,-1," -10^{-9}");
 plot->GetXaxis()->ChangeLabel(-1,-1,-1,-1,-1,-1,"10^{-9}");
 }*/
}

template <class T>
void mgr::SetTopPlotAxis(T *plot)
{
  SetAxis(plot);
  plot->GetXaxis()->SetLabelSize(0);
  plot->GetXaxis()->SetTitleSize(0);
  plot->GetYaxis()->SetTitleOffset(1.2);
}

template <class T>
void mgr::SetBottomPlotAxis(T *plot)
{
  SetAxis(plot);
  plot->GetYaxis()->SetNdivisions(503);
  plot->GetXaxis()->SetTitleOffset(3.0);
  plot->GetYaxis()->SetTitleOffset(1.2);
}

template <class T>
void mgr::SetRightPlotAxis(T *plot)
{
  SetAxis(plot);
  plot->GetXaxis()->SetLabelSize(0);
  plot->GetXaxis()->SetNdivisions(503);
  plot->GetXaxis()->SetTitleSize(0);
  plot->GetYaxis()->SetLabelSize(0);
  plot->GetYaxis()->SetTitleSize(0);
  plot->GetYaxis()->SetTitleOffset(1.2);
}

template <class T>
void mgr::SetLeftPlotAxis(T *plot)
{
  SetAxis(plot);
  plot->GetYaxis()->SetNdivisions(503);
  plot->GetXaxis()->SetTitleOffset(3.0);
  plot->GetYaxis()->SetTitleOffset(1.2);
}
template <class T>
void mgr::SetNormToUnity(T *plot)
{
  Double_t norm = plot->Integral();
  plot->Scale(1 / norm);
}

#endif /* end of include guard: MANAGERUTILS_PLOTUTILS_SETAXIS_IPP */
