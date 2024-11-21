/*********************************************
Design3D Program
Copyright(c) 2000-21,  S. D. Rajan
All rights reserved
**********************************************

Created On:         May 23, 2001
Created By:         S. D. Rajan
Last modified by:   April 8, 20131
Last modified on:   S. D. Rajan
Functionality:		Contains data for numerical integration
Input:              
Output:             
List of Changes:    
    5-23-2001  SDR  Documentation added
    5-26-2001  SDR  Split into three different classes


********************************************/
#include "stdafx.h"
#include <cmath>
#include <stdlib.h>
#include "NumericalIntegrationEXH.h"

//*****************************************************************
//************************** GAUSS-LEGENDRE ***********************
//*****************************************************************
CGaussLegendre::CGaussLegendre ()
// ----------------------------------------------------------------------------
// Function: Constructor
// Input:    None
// Output:   Gauss coordinates & weights
// ----------------------------------------------------------------------------
{
    // ========================================== location
    // location: order 1 (O(h^2))
    GC[0][0] =  0.0;
    GC[0][1] =  0.0;
    GC[0][2] =  0.0;

    // location: order 2 (O(h^4))
    GC[1][0] = -0.5773502692;
    GC[1][1] =  0.5773502692;
    GC[1][2] =  0.0;

    // location: order 3 (O(h^6))
    GC[2][0] = -0.7745966692;
    GC[2][1] =  0.0;
    GC[2][2] =  0.774596692;

    // location: order 4 (O(h^8)) (ref. Burnett. page 319)
    GC[3][0] = -(sqrt(15.0+2.0*sqrt(30.0)))/(sqrt(35.0));
    GC[3][1] = -(sqrt(15.0-2.0*sqrt(30.0)))/(sqrt(35.0));
    GC[3][2] = -GC[3][1];
    GC[3][3] = -GC[3][0];

    // location: order 5 (O(h^10)) (ref. Burnett. page 319)
    GC[4][0] = -(sqrt(35.0+2.0*sqrt(70.0)))/(sqrt(63.0));
    GC[4][1] = -(sqrt(35.0-2.0*sqrt(70.0)))/(sqrt(63.0));
    GC[4][2] = 0.0;
    GC[4][3] = -GC[4][1];
    GC[4][4] = -GC[4][0];

    // location: order 6 (ref. Atkinson. page 238)
    GC[5][0] = -0.9324695142;
    GC[5][1] = -0.6612093865;
    GC[5][2] = -0.2386191861;
    GC[5][3] = 0.2386191861;
    GC[5][4] = 0.6612093865;
    GC[5][5] = 0.9324695142;

    // location: order 7 (ref. Atkinson. page 238)
    GC[6][0] = -0.9491079123;
    GC[6][1] = -0.7415311856;
    GC[6][2] = -0.4058451514;
    GC[6][3] = 0.0;
    GC[6][4] = 0.4058451514;
    GC[6][5] = 0.7415311856;
    GC[6][6] = 0.9491079123;

    // location: order 8 (ref. Atkinson. page 238)
    GC[7][0] = -0.9602898565;
    GC[7][1] = -0.7966664774;
    GC[7][2] = -0.5255324099;
    GC[7][3] = -0.1834346425;
    GC[7][4] = 0.1834346425;
    GC[7][5] = 0.5255324099;
    GC[7][6] = 0.7966664774;
    GC[7][7] = 0.9602898565;

    // ========================================== weights
    // weight: order 1
    GW[0][0] = 2.0;
    GW[0][1] = 0.0;
    GW[0][2] = 0.0;

    // weight: order 2
    GW[1][0] = 1.0;
    GW[1][1] = 1.0;
    GW[1][2] = 0.0;

    // weight: order 3
    GW[2][0] = 0.5555555556;
    GW[2][1] = 0.8888888889;
    GW[2][2] = 0.5555555556;

    // weight: order 4 (ref. Burnett. page 319)
    GW[3][0] = 49.0/(6.0*(18.0+sqrt(30.0)));
    GW[3][1] = 49.0/(6.0*(18.0-sqrt(30.0)));
    GW[3][2] = GW[3][1];
    GW[3][3] = GW[3][0];

    // weight: order 5 (ref. Burnett. page 319)
    GW[4][0] = 5103.0/(50.0*(322.0+13.0*sqrt(70.0)));
    GW[4][1] = 5103.0/(50.0*(322.0-13.0*sqrt(70.0)));
    GW[4][2] = 128.0/225.0;
    GW[4][3] = GW[4][1];
    GW[4][4] = GW[4][0];

    // weight: order 6 (ref. Atkinson. page 238)
    GW[5][0] = 0.1713244924;
    GW[5][1] = 0.3607615730;
    GW[5][2] = 0.4679139346;
    GW[5][3] = 0.4679139346;
    GW[5][4] = 0.3607615730;
    GW[5][5] = 0.1713244924;

    // weight: order 7 (ref. Atkinson. page 238)
    GW[6][0] = 0.1294849662;
    GW[6][1] = 0.2797053915;
    GW[6][2] = 0.3818300505;
    GW[6][3] = 0.4179591837;
    GW[6][4] = 0.3818300505;
    GW[6][5] = 0.2797053915;
    GW[6][6] = 0.1294849662;

    // weight: order 8 (ref. Atkinson. page 238)
    GW[7][0] = 0.1012285363;
    GW[7][1] = 0.2223810345;
    GW[7][2] = 0.3137066459;
    GW[7][3] = 0.3626837384;
    GW[7][4] = 0.3626837384;
    GW[7][5] = 0.3137066459;
    GW[7][6] = 0.2223810345;
    GW[7][7] = 0.1012285363;

	// tolerance
    m_EPS = 1.0e-6;
}

CGaussLegendre::~CGaussLegendre ()
// ----------------------------------------------------------------------------
// Function: Destructor. 
// Input:    None
// Output:   None
// ----------------------------------------------------------------------------
{
}

double CGaussLegendre::GetLocation (int nOrder, int nPoint)
{
// ----------------------------------------------------------------------------
// Function: Return natural coordinate for quadrilateral & solid elements. 
// Input:    Integration order & Gauss point.
// Output:   Natural coordinate of a Gauss point.
// ----------------------------------------------------------------------------
    return GC[nOrder-1][nPoint-1];
}

double CGaussLegendre::GetWeight (int nOrder, int nPoint)
{
// ----------------------------------------------------------------------------
// Function: Return weight for quadrilateral & solid elements. 
// Input:    Integration order & Gauss point.
// Output:   Weight of a Gauss point.
// ----------------------------------------------------------------------------
    return GW[nOrder-1][nPoint-1];
}

//*****************************************************************
//************************** GAUSS-LEGENDRE ***********************
//************************** AREA COORDINATES *********************
//*****************************************************************
CGaussLegendreAC::CGaussLegendreAC ()
// ----------------------------------------------------------------------------
// Function: Constructor
// Input:    None
// Output:   Gauss coordinates & weights
// ----------------------------------------------------------------------------
{
    // location: order 1
    GCA[0][0][0] = 0.33333333333;
    GCA[0][0][1] = 0.33333333333;
    GCA[0][0][2] = 0.33333333333;

    GCA[0][1][0] = 0.0;
    GCA[0][1][1] = 0.0;
    GCA[0][1][2] = 0.0;

    GCA[0][2][0] = 0.0;
    GCA[0][2][1] = 0.0;
    GCA[0][2][2] = 0.0;

    GCA[0][3][0] = 0.0;
    GCA[0][3][1] = 0.0;
    GCA[0][3][2] = 0.0;

    GCA[0][4][0] = 0.0;
    GCA[0][4][1] = 0.0;
    GCA[0][4][2] = 0.0;

    GCA[0][5][0] = 0.0;
    GCA[0][5][1] = 0.0;
    GCA[0][5][2] = 0.0;

    // location: order 2
    GCA[1][0][0] = 0.16666666667;
    GCA[1][0][1] = 0.16666666667;
    GCA[1][0][2] = 0.66666666667;

    GCA[1][1][0] = 0.66666666667;
    GCA[1][1][1] = 0.16666666667;
    GCA[1][1][2] = 0.16666666667;

    GCA[1][2][0] = 0.16666666667;
    GCA[1][2][1] = 0.66666666667;
    GCA[1][2][2] = 0.16666666667;

    GCA[1][3][0] = 0.0;
    GCA[1][3][1] = 0.0;
    GCA[1][3][2] = 0.0;

    GCA[1][4][0] = 0.0;
    GCA[1][4][1] = 0.0;
    GCA[1][4][2] = 0.0;
    
    GCA[1][5][0] = 0.0;
    GCA[1][5][1] = 0.0;
    GCA[1][5][2] = 0.0;

    // location: order 3
    GCA[2][0][0] = 0.33333333333;
    GCA[2][0][1] = 0.33333333333;
    GCA[2][0][2] = 0.33333333333;

    GCA[2][1][0] = 0.6;
    GCA[2][1][1] = 0.2;
    GCA[2][1][2] = 0.2;

    GCA[2][2][0] = 0.2;
    GCA[2][2][1] = 0.6;
    GCA[2][2][2] = 0.2;

    GCA[2][3][0] = 0.2;
    GCA[2][3][1] = 0.2;
    GCA[2][3][2] = 0.6;

    GCA[2][4][0] = 0.0;
    GCA[2][4][1] = 0.0;
    GCA[2][4][2] = 0.0;

    GCA[2][5][0] = 0.0;
    GCA[2][5][1] = 0.0;
    GCA[2][5][2] = 0.0;

    // location: order 4
    GCA[3][0][0] = 0.81684757298;
    GCA[3][0][1] = 0.09157621351;
    GCA[3][0][2] = 0.09157621351;

    GCA[3][1][0] = 0.09157621351;
    GCA[3][1][1] = 0.81684757298;
    GCA[3][1][2] = 0.09157621351;

    GCA[3][2][0] = 0.09157621351;
    GCA[3][2][1] = 0.09157621351;
    GCA[3][2][2] = 0.81684757298;

    GCA[3][3][0] = 0.10810301817;
    GCA[3][3][1] = 0.44594849092;
    GCA[3][3][2] = 0.44594849092;

    GCA[3][4][0] = 0.44594849092;
    GCA[3][4][1] = 0.10810301817;
    GCA[3][4][2] = 0.44594849092;

    GCA[3][5][0] = 0.44594849092;
    GCA[3][5][1] = 0.44594849092;
    GCA[3][5][2] = 0.10810301817;

    // weight: order 1
    GWA[0][0] =  1.0;
    GWA[0][1] =  0.0;
    GWA[0][2] =  0.0;
    GWA[0][3] =  0.0;
    GWA[0][4] =  0.0;
    GWA[0][5] =  0.0;

    // weight: order 2
    GWA[1][0] =  0.333333333;
    GWA[1][1] =  0.333333333;
    GWA[1][2] =  0.333333333;
    GWA[1][3] =  0.0;
    GWA[1][4] =  0.0;
    GWA[1][5] =  0.0;

    // weight: order 3
    GWA[2][0] = -0.5625;
    GWA[2][1] = 0.520833333;
    GWA[2][2] = 0.520833333;
    GWA[2][3] = 0.520833333;
    GWA[2][4] = 0.0;
    GWA[2][5] = 0.0;

    // weight: order 4
    GWA[3][0] =  0.109951744;
    GWA[3][1] =  0.109951744;
    GWA[3][2] =  0.109951744;
    GWA[3][3] =  0.223381590;
    GWA[3][4] =  0.223381590;
    GWA[3][5] =  0.223381590;
}

CGaussLegendreAC::~CGaussLegendreAC ()
// ----------------------------------------------------------------------------
// Function: Destructor. 
// Input:    None
// Output:   None
// ----------------------------------------------------------------------------
{
}

int CGaussLegendreAC::GetNumPoints (int nOrder)
// ----------------------------------------------------------------------------
// Function: Return # of points for the given order
// Input:    Integration order
// Output:   # of points
// ----------------------------------------------------------------------------
{
    if (nOrder == 1)
        return 1;
    else if (nOrder == 2)
        return 3;
    else if (nOrder == 3)
        return 4;
    else if (nOrder == 4)
        return 6;
    else
        return 0;
}

double CGaussLegendreAC::GetLocationAC (int nOrder, int nPoint, int nCoor)
// ----------------------------------------------------------------------------
// Function: Return area coordinate for triangular elements. 
// Input:    Integration order, which Gauss point, & which coordinate.
// Output:   Area coordinate of a Gauss point.
// ----------------------------------------------------------------------------
{
    return GCA[nOrder-1][nPoint-1][nCoor-1];
}

double CGaussLegendreAC::GetWeightAC (int nOrder, int nPoint)
// ----------------------------------------------------------------------------
// Function: Return weight for triangular elements. 
// Input:    Integration order, which Gauss point, & which coordinate.
// Output:   Weight of a Gauss point.
// ----------------------------------------------------------------------------
{
    return GWA[nOrder-1][nPoint-1];
}

//*****************************************************************
//************************** GAUSS-LEGENDRE ***********************
//************************** VOLUME COORDINATES ******************
//*****************************************************************
CGaussLegendreVC::CGaussLegendreVC ()
// ----------------------------------------------------------------------------
// Function: Constructor
// Input:    None
// Output:   Gauss coordinates & weights
// ----------------------------------------------------------------------------
{
    // location: order 1 (1 point)
    GCV[0][0][0] = 0.25;
    GCV[0][0][1] = 0.25;
    GCV[0][0][2] = 0.25;
    GCV[0][0][3] = 0.25;

    // location: order 2 (4 points)
    GCV[1][0][0] = 0.58541020;
    GCV[1][0][1] = 0.13819660;
    GCV[1][0][2] = 0.13819660;
    GCV[1][0][3] = 0.13819660;

    GCV[1][1][0] = 0.13819660;
    GCV[1][1][1] = 0.58541020;
    GCV[1][1][2] = 0.13819660;
    GCV[1][1][3] = 0.13819660;

    GCV[1][2][0] = 0.13819660;
    GCV[1][2][1] = 0.13819660;
    GCV[1][2][2] = 0.58541020;
    GCV[1][2][3] = 0.13819660;

    GCV[1][3][0] = 0.13819660;
    GCV[1][3][1] = 0.13819660;
    GCV[1][3][2] = 0.13819660;
    GCV[1][3][3] = 0.58541020;

    // location: order 3 (5 points)
    GCV[2][0][0] = 0.25;
    GCV[2][0][1] = 0.25;
    GCV[2][0][2] = 0.25;
    GCV[2][0][3] = 0.25;

    GCV[2][1][0] = 0.5;
    GCV[2][1][1] = 0.16666666667;
    GCV[2][1][2] = 0.16666666667;
    GCV[2][1][3] = 0.16666666667;

    GCV[2][2][0] = 0.16666666667;
    GCV[2][2][1] = 0.5;
    GCV[2][2][2] = 0.16666666667;
    GCV[2][2][3] = 0.16666666667;

    GCV[2][3][0] = 0.16666666667;
    GCV[2][3][1] = 0.16666666667;
    GCV[2][3][2] = 0.5;
    GCV[2][3][3] = 0.16666666667;

    GCV[2][4][0] = 0.16666666667;
    GCV[2][4][1] = 0.16666666667;
    GCV[2][4][2] = 0.16666666667;
    GCV[2][4][3] = 0.5;

    // location: order 4 (11 points)
    GCV[3][0][0] = 0.25;
    GCV[3][0][1] = 0.25;
    GCV[3][0][2] = 0.25;
    GCV[3][0][3] = 0.25;

    double dL1 = 11.0/14.0, dL2 = 1.0/14.0;
    GCV[3][1][0] = dL1;
    GCV[3][1][1] = dL2;
    GCV[3][1][2] = dL2;
    GCV[3][1][3] = dL2;

    GCV[3][2][0] = dL2;
    GCV[3][2][1] = dL1;
    GCV[3][2][2] = dL2;
    GCV[3][2][3] = dL2;

    GCV[3][3][0] = dL2;
    GCV[3][3][1] = dL2;
    GCV[3][3][2] = dL1;
    GCV[3][3][3] = dL2;

    GCV[3][4][0] = dL2;
    GCV[3][4][1] = dL2;
    GCV[3][4][2] = dL2;
    GCV[3][4][3] = dL1;

    dL1 = (1.0+sqrt(5.0/14.0))/4.0;  dL2 = (1.0-sqrt(5.0/14.0))/4.0;
    GCV[3][5][0] = dL1;
    GCV[3][5][1] = dL1;
    GCV[3][5][2] = dL2;
    GCV[3][5][3] = dL2;

    GCV[3][6][0] = dL1;
    GCV[3][6][1] = dL2;
    GCV[3][6][2] = dL1;
    GCV[3][6][3] = dL2;

    GCV[3][7][0] = dL1;
    GCV[3][7][1] = dL2;
    GCV[3][7][2] = dL2;
    GCV[3][7][3] = dL1;

    GCV[3][8][0] = dL2;
    GCV[3][8][1] = dL1;
    GCV[3][8][2] = dL1;
    GCV[3][8][3] = dL2;

    GCV[3][9][0] = dL2;
    GCV[3][9][1] = dL1;
    GCV[3][9][2] = dL2;
    GCV[3][9][3] = dL1;

    GCV[3][10][0] = dL2;
    GCV[3][10][1] = dL2;
    GCV[3][10][2] = dL1;
    GCV[3][10][3] = dL1;

    // location: order 5 
    // (A.H. Stroud, Approximate calculation of multiple integrals, 1971, p315)
    GCV[4][0][0] = 0.25;
    GCV[4][0][1] = 0.25;
    GCV[4][0][2] = 0.25;
    GCV[4][0][3] = 0.25;

    dL1 = (7.0-sqrt(15.0))/34.0;      // 1.0/3.0; (Keast, 1986)
    dL2 = (13.0+3.0*sqrt(15.0))/34.0; // 0.0;

    GCV[4][1][0] = dL1;
    GCV[4][1][1] = dL1;
    GCV[4][1][2] = dL1;
    GCV[4][1][3] = dL2;

    GCV[4][2][0] = dL1;
    GCV[4][2][1] = dL1;
    GCV[4][2][2] = dL2;
    GCV[4][2][3] = dL1;

    GCV[4][3][0] = dL1;
    GCV[4][3][1] = dL2;
    GCV[4][3][2] = dL1;
    GCV[4][3][3] = dL1;

    GCV[4][4][0] = dL2;
    GCV[4][4][1] = dL1;
    GCV[4][4][2] = dL1;
    GCV[4][4][3] = dL1;

    dL1 = (7.0+sqrt(15.0))/34.0;      // 0.090909090909091; (Keast, 1986)
    dL2 = (13.0-3.0*sqrt(15.0))/34.0; // 0.727272727272727;

    GCV[4][5][0] = dL1;
    GCV[4][5][1] = dL1;
    GCV[4][5][2] = dL1;
    GCV[4][5][3] = dL2;

    GCV[4][6][0] = dL1;
    GCV[4][6][1] = dL1;
    GCV[4][6][2] = dL2;
    GCV[4][6][3] = dL1;

    GCV[4][7][0] = dL1;
    GCV[4][7][1] = dL2;
    GCV[4][7][2] = dL1;
    GCV[4][7][3] = dL1;

    GCV[4][8][0] = dL2;
    GCV[4][8][1] = dL1;
    GCV[4][8][2] = dL1;
    GCV[4][8][3] = dL1;
    
    dL1 = (10.0-2.0*sqrt(15.0))/40.0; //0.066550153573664; (Keast, 1986)
    dL2 = (10.0+2.0*sqrt(15.0))/40.0; //0.433449846426336;

    GCV[4][9][0] = dL1;
    GCV[4][9][1] = dL1;
    GCV[4][9][2] = dL2;
    GCV[4][9][3] = dL2;
    
    GCV[4][10][0] = dL1;
    GCV[4][10][1] = dL2;
    GCV[4][10][2] = dL1;
    GCV[4][10][3] = dL2;
    
    GCV[4][11][0] = dL1;
    GCV[4][11][1] = dL2;
    GCV[4][11][2] = dL2;
    GCV[4][11][3] = dL1;

    GCV[4][12][0] = dL2;
    GCV[4][12][1] = dL1;
    GCV[4][12][2] = dL1;
    GCV[4][12][3] = dL2;

    GCV[4][13][0] = dL2;
    GCV[4][13][1] = dL1;
    GCV[4][13][2] = dL2;
    GCV[4][13][3] = dL1;

    GCV[4][14][0] = dL2;
    GCV[4][14][1] = dL2;
    GCV[4][14][2] = dL1;
    GCV[4][14][3] = dL1;

    // weight: order 1
    double dW;
    GWV[0][0] =  1.0;
    GWV[0][1] =  0.0;
    GWV[0][2] =  0.0;
    GWV[0][3] =  0.0;
    GWV[0][4] =  0.0;

    // weight: order 2
    GWV[1][0] =  0.25;
    GWV[1][1] =  0.25;
    GWV[1][2] =  0.25;
    GWV[1][3] =  0.25;
    GWV[1][4] =  0.0;

    // weight: order 3
    GWV[2][0] = -0.8;
    GWV[2][1] =  0.45;
    GWV[2][2] =  0.45;
    GWV[2][3] =  0.45;
    GWV[2][4] =  0.45;

    // weight: order 4
    GWV[3][0]  = -148.0/1875.0;
    dW = 343.0/7500.0;
    GWV[3][1]  = dW;
    GWV[3][2]  = dW;
    GWV[3][3]  = dW;
    GWV[3][4]  = dW;
    dW = 56.0/375.0;
    GWV[3][5]  = dW;
    GWV[3][6]  = dW;
    GWV[3][7]  = dW;
    GWV[3][8]  = dW;
    GWV[3][9]  = dW;
    GWV[3][10] = dW;

     // weight: order 5 (Shroud 1971, p315)
    GWV[4][0] =  16.0/135.0;
    dW = (2665.0+14.0*sqrt(15.0))/37800.0;
    GWV[4][1] =  dW;
    GWV[4][2] =  dW;
    GWV[4][3] =  dW;
    GWV[4][4] =  dW;
    dW = (2665.0-14.0*sqrt(15.0))/37800.0;
    GWV[4][5] =  dW;
    GWV[4][6] =  dW;
    GWV[4][7] =  dW;
    GWV[4][8] =  dW;
    dW = 20.0/378.0;
    GWV[4][9] = dW;
    GWV[4][10] = dW;
    GWV[4][11] = dW;
    GWV[4][12] = dW;
    GWV[4][13] = dW;
    GWV[4][14] = dW;

    /* (P. Keast, Moderate-degree tetrahedral quadrature formulas, 
    Comput. Methods Appl. Mech. Engrg. 55 (1986), 339--348.)
    GWV[4][0] =  0.030283678097089; 
    GWV[4][1] =  0.006026785714286;
    GWV[4][2] =  0.006026785714286;
    GWV[4][3] =  0.006026785714286;
    GWV[4][4] =  0.006026785714286;
    GWV[4][5] =  0.011645249086029;
    GWV[4][6] =  0.011645249086029;
    GWV[4][7] =  0.011645249086029;
    GWV[4][8] =  0.011645249086029;
    GWV[4][9] =  0.010949141561386;
    GWV[4][10] = 0.010949141561386;
    GWV[4][11] =  0.010949141561386;
    GWV[4][12] =  0.010949141561386;
    GWV[4][13] =  0.010949141561386;
    GWV[4][14] =  0.010949141561386*/
}

CGaussLegendreVC::~CGaussLegendreVC ()
// ----------------------------------------------------------------------------
// Function: Destructor. 
// Input:    None
// Output:   None
// ----------------------------------------------------------------------------
{
}

double CGaussLegendreVC::GetLocationVC (int nOrder, int nPoint, int nCoor)
// ----------------------------------------------------------------------------
// Function: Return volumn coordinate for tetrahedra elements. 
// Input:    Integration order, which Gauss point, & which coordinate.
// Output:   Volume coordinate of a Gauss point. 
// ----------------------------------------------------------------------------
{
    return GCV[nOrder-1][nPoint-1][nCoor-1];
}

double CGaussLegendreVC::GetWeightVC (int nOrder, int nPoint)
// ----------------------------------------------------------------------------
// Function: Return weight for tetrahedra elements. 
// Input:    Integration order, which Gauss point, & which coordinate.
// Output:   Volumn coordinate of a Gauss point.
// ----------------------------------------------------------------------------
{
    return GWV[nOrder-1][nPoint-1];
}

int CGaussLegendreVC::GetNumPoints (int nOrder)
// ----------------------------------------------------------------------------
// Function: Return # of points for the given order
// Input:    Integration order
// Output:   # of points
// ----------------------------------------------------------------------------
{
    if (nOrder == 1)
        return 1;
    else if (nOrder == 2)
        return 4;
    else if (nOrder == 3)
        return 5;
    else if (nOrder == 4)
        return 11;
    else if (nOrder == 5)
        return 15;
    else
        return 0;
}


//*****************************************************************
//************************** GAUSS-LEGENDRE ***********************
//********* AREA COORDINATES - LAUFFER FORMULA ********************
//*****************************************************************
CGaussLegendreACL::CGaussLegendreACL ()
// ----------------------------------------------------------------------------
// Function: Constructor
// Input:    None
// Output:   Gauss coordinates & weights
// ----------------------------------------------------------------------------
{
    // location: order 1
    GCAL[0][0][0] = 0.0;
    GCAL[0][0][1] = 0.0;
    GCAL[0][0][2] = 1.0;

    GCAL[0][1][0] = 1.0;
    GCAL[0][1][1] = 0.0;
    GCAL[0][1][2] = 0.0;

    GCAL[0][2][0] = 0.0;
    GCAL[0][2][1] = 1.0;
    GCAL[0][2][2] = 0.0;

    // weight: order 1
    GWAL[0][0] =  1.0/3.0;
    GWAL[0][1] =  1.0/3.0;
    GWAL[0][2] =  1.0/3.0;
}

CGaussLegendreACL::~CGaussLegendreACL ()
// ----------------------------------------------------------------------------
// Function: Destructor. 
// Input:    None
// Output:   None
// ----------------------------------------------------------------------------
{
}

double CGaussLegendreACL::GetLocationACL (int nOrder, int nPoint, int nCoor)
// ----------------------------------------------------------------------------
// Function: Return area coordinate for triangular elements. 
// Input:    Integration order, which Gauss point, & which coordinate.
// Output:   Area coordinate of a Gauss point.
// ----------------------------------------------------------------------------
{
    return GCAL[nOrder-1][nPoint-1][nCoor-1];
}

double CGaussLegendreACL::GetWeightACL (int nOrder, int nPoint)
// ----------------------------------------------------------------------------
// Function: Return weight for triangular elements. 
// Input:    Integration order, which Gauss point, & which coordinate.
// Output:   Weight of a Gauss point.
// ----------------------------------------------------------------------------
{
    return GWAL[nOrder-1][nPoint-1];
}

//****************************************************************
//************************** SIMPSON'S RULE **********************
//****************************************************************
CSimpsonRule::CSimpsonRule ()
{
    double w1 = 1.0/3.0, w2 = 2.0/3.0, w4 = 4.0/3.0;

    // ========================================== location
    // location: order 1 
    SRC[0][0] = -0.5;
    SRC[0][1] =  0.0;
    SRC[0][2] =  0.5;

    // location: order 2 
    SRC[1][0] = -0.5;
    SRC[1][1] = -0.25;
    SRC[1][2] =  0.0;
    SRC[1][3] =  0.25;
    SRC[1][4] =  0.5;

    // location: order 3
    SRC[2][0] = -0.5;
    SRC[2][1] = -0.3333333333333333;
    SRC[2][2] = -0.1666666666666667;
    SRC[2][3] =  0.0;
    SRC[2][4] =  0.1666666666666667;
    SRC[2][5] =  0.3333333333333333;
    SRC[2][6] =  0.5;

    // ========================================== weight
    // weight: order 1 
    SRW[0][0] = w1;
    SRW[0][1] = w4;
    SRW[0][2] = w1;

    // weight: order 2 
    SRW[1][0] = w1;
    SRW[1][1] = w4;
    SRW[1][2] = w2;
    SRW[1][3] = w4;
    SRW[1][4] = w1;

    // weight: order 3 
    SRW[2][0] = w1;
    SRW[2][1] = w4;
    SRW[2][2] = w2;
    SRW[2][3] = w4;
    SRW[2][4] = w2;
    SRW[2][5] = w4;
    SRW[2][6] = w1;
}

CSimpsonRule::~CSimpsonRule ()
{
}
 
double CSimpsonRule::GetWeightSR (int nOrder, int nPoint)
// ----------------------------------------------------------------------------
// Function: Return weight for shell elements' through thickness integration. 
// Input:    Integration order & # point.
// Output:   Weight.
// ----------------------------------------------------------------------------
{
    return SRW[nOrder-1][nPoint-1];
}

double CSimpsonRule::GetLocationSR (int nOrder, int nPoint)
// ----------------------------------------------------------------------------
// Function: Return coordinate for shell elements' through thickness integration. 
// Input:    Integration order & # point.
// Output:   Coordinate of a point.
// ----------------------------------------------------------------------------
{
    return SRC[nOrder-1][nPoint-1];
}

//*****************************************************************
//************************** GAUSS-LAGUERRE ***********************
//************************** NATURAL COORDINATES ******************
//*****************************************************************
CGaussLaguerre::CGaussLaguerre ()
// ----------------------------------------------------------------------------
// Function: Constructor
// Input:    None
// Output:   Gauss coordinates & weights
// ----------------------------------------------------------------------------
{
    // ========================================== location
    // location: order 1
    GC[0][0] = 1.0;

    // location: order 2
    GC[1][0] = 0.5857864376;
    GC[1][1] = 3.414213562;

    // location: order 3
    GC[2][0] = 0.415775;
    GC[2][1] = 2.29428;
    GC[2][2] = 6.28995;

    // location: order 4
    GC[3][0] = 0.322548;
    GC[3][1] = 1.74576;
    GC[3][2] = 4.53662;
    GC[3][3] = 9.39507;

    // location: order 5
    GC[4][0] = 0.26356;
    GC[4][1] = 1.4134;
    GC[4][2] = 3.59643;
    GC[4][3] = 7.08581;
    GC[4][4] = 12.6408;

    // ========================================== weights
    // weight: order 1
    GW[0][0] = 1.0;

    // weight: order 2
    GW[1][0] = 0.8535533905;
    GW[1][1] = 0.1464466094;

    // weight: order 3
    GW[2][0] = 0.711093;
    GW[2][1] = 0.278518;
    GW[2][2] = 0.0103893;

    // weight: order 4
    GW[3][0] = 0.603154;
    GW[3][1] = 0.357419;
    GW[3][2] = 0.0388879;
    GW[3][3] = 0.000539295;

    // weight: order 5
    GW[4][0] = 0.521756;
    GW[4][1] = 0.398667;
    GW[4][2] = 0.0759424;
    GW[4][3] = 0.00361176;
    GW[4][4] = 0.00002337;
}

CGaussLaguerre::~CGaussLaguerre ()
// ----------------------------------------------------------------------------
// Function: Destructor. 
// Input:    None
// Output:   None
// ----------------------------------------------------------------------------
{
}

double CGaussLaguerre::GetLocation (int nOrder, int nPoint)
{
// ----------------------------------------------------------------------------
// Function: Return natural coordinate for quadrilateral & solid elements. 
// Input:    Integration order & Gauss point.
// Output:   Natural coordinate of a Gauss point.
// ----------------------------------------------------------------------------
    return GC[nOrder-1][nPoint-1];
}

double CGaussLaguerre::GetWeight (int nOrder, int nPoint)
{
// ----------------------------------------------------------------------------
// Function: Return weight for quadrilateral & solid elements. 
// Input:    Integration order & Gauss point.
// Output:   Weight of a Gauss point.
// ----------------------------------------------------------------------------
    return GW[nOrder-1][nPoint-1];
}

