/*******************************************************************************
*
*  Filename    : Intersect.hpp
*  Description : Calculation for intersections
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef MANAGERUTILS_MATHS_INTERSECT_HPP
#define MANAGERUTILS_MATHS_INTERSECT_HPP

/*******************************************************************************
*   Intersect - Given four points A(p1x,p1y), B(p2x,p2y), C(p3x,p3y), D(p4x,p4y)
*               Check if segments AB and CD are intersecting. If yes, store
*               results in intx, and inty return true;
*               Return false otherewise;
*******************************************************************************/
bool Intersect(
    const double p1x, const double p1y,
    const double p2x, const double p2y,
    const double p3x, const double p3y,
    const double p4x, const double p4y,
    double& intx, double& inty
    );

#endif/* end of include guard: MANAGERUTILS_MATHS_INTERSECT_HPP */
