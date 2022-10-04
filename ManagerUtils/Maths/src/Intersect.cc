/*******************************************************************************
*
*  Filename    : Intersect.cc
*  Description : Implementation for intersection calculations
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/

bool
Intersect(
    const double p1x, const double p1y,
    const double p2x, const double p2y,
    const double p3x, const double p3y,
    const double p4x, const double p4y,
    double& intx, double& inty
    )
{
    const double s1x = p2x - p1x;
    const double s1y = p2y - p1y;
    const double s2x = p4x - p3x;
    const double s2y = p4y - p3y;
    const double s   = ( -s1y * ( p1x - p3x ) + s1x * ( p1y - p3y ) ) / ( -s2x * s1y + s1x * s2y );
    const double t   = ( s2x * ( p1y - p3y ) - s2y * ( p1x - p3x ) ) / ( -s2x * s1y + s1x * s2y );

    if( s >= 0 && s <= 1 && t >= 0 && t <= 1 ){
        intx = p1x + ( t * s1x );
        inty = p1y + ( t * s1y );
        return true;
    }
    else{
        return false;
    }
}
