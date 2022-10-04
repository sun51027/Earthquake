/*******************************************************************************
*
*  Filename    : Exponent.cc
*  Description : Implmentation of exponent functions
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/

int
Power( int base, unsigned exp )
{
    int result = 1;

    while( exp ){
        if( exp & 1 ){
            result *= base;
        }

        exp >>= 1;
        base *= base;
    }

    return result;
}

/******************************************************************************/
