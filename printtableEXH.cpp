/******************************************************************************
Utility Library Function
Copyright(c) 2003-23, S. D. Rajan
All rights reserved

Object-Oriented Numerical Analysis

*******************************************************************************
This file contains CPrintTable class functions which prints data in tabular
form to specified output file.

*******************************************************************************/
#include <float.h>
#include <iomanip>
#include "printtableEXH.h"
#include "GlobalErrorHandler.h"

CPrintTable::CPrintTable (const std::string& szFileName)
// ----------------------------------------------------------------------------
// Function: Constructor
// Input:  none
// Output: Class member variables initialized
// ----------------------------------------------------------------------------
{
    FOut = new std::ofstream;
    try
    {
        FOut->open (szFileName, std::ios::out | std::ofstream::app);
    }
    catch (std::ofstream::failure e)
    {
        ErrorHandler (4);
    }
    m_nCols = 0;
    Initialize ();
    m_bFileOpened = true;
}

CPrintTable::CPrintTable (const int nColumns, const std::string& szFileName)
// ----------------------------------------------------------------------------
// Function: Constructor
// Input:  none
// Output: Class member variables initialized
// ----------------------------------------------------------------------------
{
    if (nColumns <= 0 || nColumns > MAXCOLS)
        ErrorHandler (1);

    FOut = new std::ofstream;
    try
    {
        FOut->open (szFileName, std::ios::out | std::ofstream::app);
    }
    catch (std::ofstream::failure e)
    {
        ErrorHandler (4);
    }
    m_nCols = nColumns;
    Initialize ();
    m_bFileOpened = true;
}

CPrintTable::CPrintTable (std::ofstream* FileOutput)
// ----------------------------------------------------------------------------
// Function: Constructor
// Input:  none
// Output: Class member variables initialized
// ----------------------------------------------------------------------------
{
    FOut = FileOutput;
    m_nCols = 0;
    Initialize ();
    m_bUsingFile = true;
}

CPrintTable::CPrintTable (const int nColumns, std::ofstream* FileOutput)
// ----------------------------------------------------------------------------
// Function: Constructor (overloaded)
// Input:  none
// Output: Class member variables initialized
// ----------------------------------------------------------------------------
{
    if (nColumns <= 0 || nColumns > MAXCOLS)
        ErrorHandler (1);

    // set the values
    FOut = FileOutput;
    m_nCols = nColumns;
    Initialize ();
}

CPrintTable::CPrintTable (const int nColumns)
// ----------------------------------------------------------------------------
// Function: Constructor (overloaded)
// Input:  none
// Output: Class member variables initialized
// ----------------------------------------------------------------------------
{
    if (nColumns <= 0 || nColumns > MAXCOLS)
        ErrorHandler(1);

    // set the values
    m_nCols = nColumns;
    Initialize();
}

void CPrintTable::Initialize ()
// ----------------------------------------------------------------------------
// Function: Initialize member variables
// Input:  none
// Output: Class member variables initialized
// ----------------------------------------------------------------------------
{
    m_nCurCol = 0;
    m_nTotalWidth = 0;
    m_nMargin = 0;
    m_nHit = 0;
    m_nCurLine = 0;
    m_bPrint = true;
    m_szTemp = "Undefined";

    for (int i = 0; i < MAXCOLS; i++)
    {
        m_nW[i] = 0;
        m_DVType[i] = TYPEUNDEFINED;
        m_nVMaxIndex[i] = 0;
        m_nVMinIndex[i] = 0;
        m_dVMaxValue[i] = -DBL_MIN;
        m_dVMinValue[i] = DBL_MAX;
        m_dVSum[i] = 0.0;
        m_dVAbsSum[i] = 0.0;
    }
    m_bFileOpened = false;
    m_bUsingFile = false;
}

CPrintTable::~CPrintTable ()
// ----------------------------------------------------------------------------
// Function: Destructor
// Input:  none
// Output: none
// ----------------------------------------------------------------------------
{
    if (m_bFileOpened)
    {
        FOut->close ();
        delete FOut;
        m_bFileOpened = false;
    }
}

void CPrintTable::SetSize (const int nColumns)
// ----------------------------------------------------------------------------
// Function: Sets the number of columns in table
// Input:  nColumns     number of columns
// Output: m_nCols      updated
// ----------------------------------------------------------------------------
{
    // valid input?
    if (nColumns <= 0 || nColumns > MAXCOLS)
        ErrorHandler (1);

    // set the value
    m_nCols = nColumns;
}

// ============================== utility functions ======================
void CPrintTable::SkipLines (int n) const
// ----------------------------------------------------------------------------
// Function: this function skips specified number of lines
// Input:    n    number of lines to skip
// Output:   none
// ----------------------------------------------------------------------------
{
    for (int i = 0; i < n; i++)
    {
        if (m_bFileOpened || m_bUsingFile)
            *FOut << "\n";
        else
            std::cout << "\n";
    }
}

void CPrintTable::SkipSpaces () const
// ----------------------------------------------------------------------------
// Function: this function skips preset number of spaces
// Input:  none
// Output: none
// ----------------------------------------------------------------------------
{
    for (int i = 0; i < m_nMargin; i++)
    {
        if (m_bFileOpened || m_bUsingFile)
            *FOut << " ";
        else
            std::cout << " ";
    }
}

void CPrintTable::SkipSpaces (const int n) const
// ----------------------------------------------------------------------------
// Function: this function skips specified number of lines
// Input:  n    number of spaces to skip
// Output: none
// ----------------------------------------------------------------------------
{
    if (n <= 0)
        return;
    for (int i = 0; i < n; i++)
    {
        if (m_bFileOpened || m_bUsingFile)
            *FOut << " ";
        else
            std::cout << " ";
    }
}

void CPrintTable::UpdateCurLine ()
// ----------------------------------------------------------------------------
// Function: Increments the current line number by one
// Input:  none
// Output: none
// ----------------------------------------------------------------------------
{
    if (m_bPrint)
        m_nCurLine++;
}

void CPrintTable::NoDataInTable () const
// ----------------------------------------------------------------------------
// Function: Formats and prints out "No data in table"
// Input:  none
// Output: none
// ----------------------------------------------------------------------------
{
    SkipSpaces ((m_nTotalWidth - 17) / 2);
    if (m_bFileOpened || m_bUsingFile)
        *FOut << "No data in table." << "\n";
    else
        std::cout << "No data in table." << "\n";
}

void CPrintTable::GetStatistics (CVector<double>& dVMin,
                                 CVector<double>& dVMax, CVector<double>& dVSum,
                                 CVector<double>& dVAbsSum, CVector<int>& nVMinIndex,
                                 CVector<int>& nVMaxIndex, int& nHit) const
// ----------------------------------------------------------------------------
// Function:  Retrieves the statistics-related member variable values   
// Input:   dVMin       vector of min values
//          dVMax       vector of max values
//          dVSum       vector of sums
//          dVAbsSum    vector of absolute sums
//          nVMinIndex  vector of index for min value
//          nVMaxIndex  vector of index for max value
//          nHit        number of datalines in table
// Output:  updated input values
// ----------------------------------------------------------------------------
{
    for (int i = 0; i < m_nCols; i++)
    {
        dVMin(i + 1) = m_dVMinValue[i];
        nVMinIndex(i + 1) = m_nVMinIndex[i];
        dVMax(i + 1) = m_dVMaxValue[i];
        nVMaxIndex(i + 1) = m_nVMaxIndex[i];
        dVSum(i + 1) = m_dVSum[i];
        dVAbsSum(i + 1) = m_dVAbsSum[i];
    }
    nHit = m_nHit;
}

void CPrintTable::PrintNext (CVector<std::string>& szV, int i)
// ----------------------------------------------------------------------------
// Function:    Formats and prints the value of specified index of vector
// Input:   szV     vector to print
//          i       index
// Output:  none
// ----------------------------------------------------------------------------
{
    if (m_bPrint)
    {
        if (m_bFileOpened || m_bUsingFile)
            *FOut << std::setw(m_nW[m_nCurCol]) << std::setiosflags(std::ios::left)
                  << szV(i);
        else
            std::cout << std::setw(m_nW[m_nCurCol]) << std::setiosflags(std::ios::left)
                      << szV(i);
    }
    DoNewLine ();
    m_nHit++;
}

void CPrintTable::AllDone ()
// ----------------------------------------------------------------------------
// Function: Checks for data in table
// Input:  none
// Output: none
// ----------------------------------------------------------------------------
{
    if (m_nHit == 0)
        NoDataInTable ();

    if (m_bFileOpened)
    {
        FOut->close ();
        delete FOut;
        m_bFileOpened = false;
    }
}

void CPrintTable::SkipLinesAndSpaces (int n) const
// ----------------------------------------------------------------------------
// Function: Skips specified number of lines and preset number of spaces
// Input:    n    number of lines
// Output:   none
// ----------------------------------------------------------------------------
{
    for (int i = 0; i < n; i++)
    {
        if (m_bFileOpened || m_bUsingFile)
            *FOut << "\n";
        else
            std::cout << "\n";
    }
    SkipSpaces ();
}

void CPrintTable::PrintTableHeading (const char* szHeading,
                                     const std::string szHeaders[],
                                     const int nWidth[], int nType)
// ----------------------------------------------------------------------------
// Function:  Formats and prints table headings
// Input:     szHeading   heading text
//            szHeaders[] vector of column headers
//            nWidth[]    vector of specified column width
//            nType       type of text alignment
// Output:    none
// ----------------------------------------------------------------------------
{
    int i, j, nLines = 1;
    std::string szL1[MAXCOLS] = { "                    ", "                    ",
                                  "                    ", "                    ",
                                  "                    ", "                    ",
                                  "                    ", "                    ",
                                  "                    ", "                    " };
    std::string szL2[MAXCOLS] = { "                    ", "                    ",
                                  "                    ", "                    ",
                                  "                    ", "                    ",
                                  "                    ", "                    ",
                                  "                    ", "                    " };

    for (m_nTotalWidth = 0, j = 0; j < m_nCols; j++)
    {
        if (nWidth[j] > MAXCOLWIDTH)
            ErrorHandler (3);
        m_nTotalWidth += nWidth[j];
    }
    if (m_nTotalWidth > MAXWIDTH)
        ErrorHandler (2);
    m_nMargin = (MAXWIDTH - m_nTotalWidth) / 2;

    // center and print the table heading
    if (nType <= 1)
    {
        SkipLines (2);
        int nLen = static_cast<int>(strlen(szHeading));
        nLen = (m_nTotalWidth - nLen) / 2;
        SkipSpaces (m_nMargin + nLen);
        if (m_bFileOpened || m_bUsingFile)
            *FOut << szHeading << "\n";
        else
            std::cout << szHeading << "\n";
        SkipSpaces ();
    }

    // compute number of lines
    for (i = 0; i < m_nCols; i++)
    {
        j = 0;
        m_szTemp = szHeaders[i];
        szL1[i] = m_szTemp;

        for (j = 0; j < (int)m_szTemp.length(); j++)
        {
            if (m_szTemp[j] == '$')
            {
                szL1[i] = " ";
                strCopy (szL1[i], m_szTemp, 0, j - 1);
                strCopy (szL2[i], m_szTemp, j + 1, static_cast<int>(m_szTemp.length() - 1));
                nLines = 2;
                break;
            }
        }
    }

    // column headings
    if (nType <= 1)
    {
        for (j = 0; j < m_nTotalWidth; j++)
        {
            if (m_bFileOpened || m_bUsingFile)
                *FOut << "-";
            else
                std::cout << "-";
        }
    }
    for (j = 0; j < nLines; j++)
    {
        SkipLines (1); SkipSpaces ();
        for (i = 0; i < m_nCols; i++)
        {
            if (j == 0)
            {
                TrimRight (szL1[i]);
                if (m_bFileOpened || m_bUsingFile)
                    *FOut << std::setw(nWidth[i])
                          << std::setiosflags(std::ios::left) << szL1[i];
                else
                    std::cout << std::setw(nWidth[i])
                              << std::setiosflags(std::ios::left) << szL1[i];
            }
            else
            {
                if (!szL2[i].empty())
                {
                    TrimRight(szL2[i]);
                    if (m_bFileOpened || m_bUsingFile)
                        *FOut << std::setw(nWidth[i])
                              << std::setiosflags(std::ios::left) << szL2[i];
                    else
                        std::cout << std::setw(nWidth[i])
                                  << std::setiosflags(std::ios::left) << szL2[i];
                }
                else
                    SkipSpaces (nWidth[i]);
            }
        }
    }
    
    //if (nType == 0 || nType == 2)
    {
        SkipLines (1); SkipSpaces ();
        for (j = 0; j < m_nTotalWidth; j++)
        {
            if (m_bFileOpened || m_bUsingFile)
                *FOut << "-";
            else
                std::cout << "-";
        }
        SkipLines (1);
        SkipSpaces ();
    }

    // set column widths
    PCopy (nWidth);
}

void CPrintTable::PrintTableHeading (const std::string& szHeading,
                                     const std::string szHeaders[],
                                     const int nWidth[], int nType)
// ----------------------------------------------------------------------------
// Function:  Formats and prints table headings
// Input:     szHeading   heading text
//            szHeaders[] vector of column headers
//            nWidth[]    vector of specified column width
//            nType       type of text alignment
// Output:    none
// ----------------------------------------------------------------------------
{
    int i, j, nLines = 1;
    std::string szL1[MAXCOLS] = { "                    ", "                    ",
                                  "                    ", "                    ",
                                  "                    ", "                    ",
                                  "                    ", "                    ",
                                  "                    ", "                    " };
    std::string szL2[MAXCOLS] = { "                    ", "                    ",
                                  "                    ", "                    ",
                                  "                    ", "                    ",
                                  "                    ", "                    ",
                                  "                    ", "                    " };

    for (m_nTotalWidth = 0, j = 0; j < m_nCols; j++)
    {
        if (nWidth[j] > MAXCOLWIDTH)
            ErrorHandler (3);
        m_nTotalWidth += nWidth[j];
    }
    if (m_nTotalWidth > MAXWIDTH)
        ErrorHandler (2);
    m_nMargin = (MAXWIDTH - m_nTotalWidth) / 2;

    // center and print the table heading
    if (nType <= 1)
    {
        SkipLines (2);
        int nLen = static_cast<int>(szHeading.length());
        nLen = (m_nTotalWidth - nLen) / 2;
        SkipSpaces (m_nMargin + nLen);
        if (m_bFileOpened || m_bUsingFile)
            *FOut << szHeading << "\n";
        else
            std::cout << szHeading << "\n";
        SkipSpaces ();
    }

    // compute number of lines
    for (i = 0; i < m_nCols; i++)
    {
        j = 0;
        m_szTemp = szHeaders[i];
        szL1[i] = m_szTemp;

        for (j = 0; j < (int)m_szTemp.length(); j++)
        {
            if (m_szTemp[j] == '$')
            {
                szL1[i] = " ";
                strCopy (szL1[i], m_szTemp, 0, j - 1);
                strCopy (szL2[i], m_szTemp, j + 1, static_cast<int>(m_szTemp.length() - 1));
                nLines = 2;
                break;
            }
        }
    }

    // column headings
    if (nType <= 1)
    {
        for (j = 0; j < m_nTotalWidth; j++)
        {
            if (m_bFileOpened || m_bUsingFile)
                *FOut << "-";
            else
                std::cout << "-";
        }
    }
    for (j = 0; j < nLines; j++)
    {
        SkipLines (1); SkipSpaces ();
        for (i = 0; i < m_nCols; i++)
        {
            if (j == 0)
            {
                TrimRight (szL1[i]);
                if (m_bFileOpened || m_bUsingFile)
                    *FOut << std::setw(nWidth[i])
                          << std::setiosflags(std::ios::left) << szL1[i];
                else
                    std::cout << std::setw(nWidth[i])
                              << std::setiosflags(std::ios::left) << szL1[i];
            }
            else
            {
                if (!szL2[i].empty())
                {
                    TrimRight(szL2[i]);
                    if (m_bFileOpened || m_bUsingFile)
                        *FOut << std::setw(nWidth[i])
                              << std::setiosflags(std::ios::left) << szL2[i];
                    else
                        std::cout << std::setw(nWidth[i])
                                  << std::setiosflags(std::ios::left) << szL2[i];
                }
                else
                    SkipSpaces (nWidth[i]);
            }
        }
    }
    if (nType == 0 || nType == 2)
    {
        SkipLines (1); SkipSpaces ();
        for (j = 0; j < m_nTotalWidth; j++)
        {
            if (m_bFileOpened || m_bUsingFile)
                *FOut << "-";
            else
                std::cout << "-";
        }
        SkipLines (1);
        SkipSpaces ();
    }

    // set column widths
    PCopy (nWidth);
}

void CPrintTable::PrintNextLine (const int nFormat, const char *sz)
// ----------------------------------------------------------------------------
// Function: formats and prints a line
// Input:    nFormat     text alignment
//           sz          text
// Output:   none
// ----------------------------------------------------------------------------
{
    m_nHit++;
    SkipLines (1);
    switch (nFormat)
    {
        case CENTERTEXT:
            SkipSpaces ((MAXWIDTH - static_cast<int>(strlen(sz))) / 2);
            if (m_bFileOpened || m_bUsingFile)
                *FOut << sz;
            else
                std::cout << sz;
            break;
        case ASIS:
        default:
            if (m_bFileOpened || m_bUsingFile)
                *FOut << sz;
            else
                std::cout << sz;
            break;
    }
}

void CPrintTable::PrintNextLine (const int nFormat, const std::string& szM)
// ----------------------------------------------------------------------------
// Function: formats and prints a line
// Input:   nFormat     text alignment
//          sz          text
// Output:  none
// ----------------------------------------------------------------------------
{
    m_nHit++;
    SkipLines (1);
    switch (nFormat)
    {
        case CENTERTEXT:
            SkipSpaces ((MAXWIDTH - static_cast<int>(szM.length())) / 2);
            if (m_bFileOpened || m_bUsingFile)
                *FOut << szM;
            else
                std::cout << szM;
            break;
        case ASIS:
        default:
            if (m_bFileOpened)
                *FOut << szM;
            else
                std::cout << szM;
            break;
    }
}

void CPrintTable::PrintNextLine (const int nFormat, const char *sz1,
                                 const std::string& str)
// ----------------------------------------------------------------------------
// Function: formats and prints a line
// Input:   nFormat     text alignment
//          sz1         text
//          str         string text
// Output:  none
// ----------------------------------------------------------------------------
{
    m_nHit++;
    SkipLines (1);
    switch (nFormat)
    {
        case CENTERTEXT:
            SkipSpaces ((MAXWIDTH - static_cast<int>(strlen(sz1)) -
                static_cast<int>(str.length())) / 2);
            if (m_bFileOpened || m_bUsingFile)
                *FOut << sz1 << str;
            else
                std::cout << sz1 << str;
            break;
        case ASIS:
        default:
            if (m_bFileOpened || m_bUsingFile)
                *FOut << sz1 << str;
            else
                std::cout << sz1 << str;
            break;
    }
}

void CPrintTable::PrintNextLine (const int nFormat, const char *sz,
                                 const int nV)
// ----------------------------------------------------------------------------
// Function: formats and prints a line
// Input:   nFormat     text alignment
//          sz          text 
//          nV          integer value
// Output:  none
// ----------------------------------------------------------------------------
{
    m_nHit++;
    SkipLines (1);
    switch (nFormat)
    {
        case CENTERTEXT:
            SkipSpaces ((m_nTotalWidth - static_cast<int>(strlen(sz))) / 2);
            if (m_bFileOpened || m_bUsingFile)
                *FOut << sz << nV;
            else
                std::cout << sz << nV;
            break;
        case ASIS:
        default:
            if (m_bFileOpened || m_bUsingFile)
                *FOut << sz << nV;
            else
                std::cout << sz << nV;
            break;
    }
}

void CPrintTable::PrintNextLine (const int nFormat, const char *sz,
                                 const long lV)
// ----------------------------------------------------------------------------
// Function: formats and prints a line
// Input:   nFormat     text alignment
//          sz          text 
//          nV          float value
// Output:  none
// ----------------------------------------------------------------------------
{
    m_nHit++;
    SkipLines (1);
    switch (nFormat)
    {
        case CENTERTEXT:
            SkipSpaces ((m_nTotalWidth - static_cast<int>(strlen(sz))) / 2);
            if (m_bFileOpened || m_bUsingFile)
                *FOut << sz << lV;
            else
                std::cout << sz << lV;
            break;
        case ASIS:
        default:
            if (m_bFileOpened || m_bUsingFile)
                *FOut << sz << lV;
            else
                std::cout << sz << lV;
            break;
    }
}

void CPrintTable::PrintNextLine (const int nFormat, const char *sz,
                                 const float fV)
// ----------------------------------------------------------------------------
// Function: formats and prints a line
// Input:   nFormat     text alignment
//          sz          text 
//          nV          float value
// Output:  none
// ----------------------------------------------------------------------------
{
    m_nHit++;
    SkipLines (1);
    switch (nFormat)
    {
        case CENTERTEXT:
            SkipSpaces ((m_nTotalWidth - static_cast<int>(strlen(sz))) / 2);
            if (m_bFileOpened || m_bUsingFile)
                *FOut << sz << fV;
            else
                std::cout << sz << fV;
            break;
        case ASIS:
        default:
            if (m_bFileOpened || m_bUsingFile)
                *FOut << sz << fV;
            else
                std::cout << sz << fV;
            break;
    }
}

void CPrintTable::PrintNextLine (const int nFormat, const char *sz,
                                 const double dV)
// ----------------------------------------------------------------------------
// Function: formats and prints a line
// Input:   nFormat     text alignment
//          sz          text 
//          nV          double precision value 
// Output:  none
// ----------------------------------------------------------------------------
{
    m_nHit++;
    SkipLines (1);
    switch (nFormat)
    {
        case CENTERTEXT:
            SkipSpaces ((m_nTotalWidth - static_cast<int>(strlen(sz))) / 2);
            if (m_bFileOpened || m_bUsingFile)
                *FOut << sz << dV;
            else
                std::cout << sz << dV;
            break;
        case ASIS:
        default:
            if (m_bFileOpened || m_bUsingFile)
                *FOut << sz << dV;
            else
                std::cout << sz << dV;
            break;
    }
}

void CPrintTable::PrintNextLine (const int nFormat, const char *sz,
                                 const double dV, const char *sz1)
// ----------------------------------------------------------------------------
// Function: formats and prints a line
// Input:   nFormat     text alignment
//          sz          text 
//          nV          double precision value 
//          sz1         text
// Output:  none
// ----------------------------------------------------------------------------
{
    m_nHit++;
    SkipLines (1);
    switch (nFormat)
    {
        case CENTERTEXT:
            SkipSpaces ((m_nTotalWidth - static_cast<int>(strlen(sz))) / 2);
            if (m_bFileOpened || m_bUsingFile)
                *FOut << sz << dV << sz1;
            else
                std::cout << sz << dV << sz1;
            break;
        case ASIS:
        default:
            if (m_bFileOpened || m_bUsingFile)
                *FOut << sz << dV << sz1;
            else
                std::cout << sz << dV << sz1;
            break;
    }
}

void CPrintTable::PrintNext (const int nV)
// ----------------------------------------------------------------------------
// Function: Checks if value is min or max of its column. 
//           Print value to the table and skip to the next column, starting a 
//           new row if necessary.
// Input:   nV      integer value
// Output:  none
// ----------------------------------------------------------------------------
{
    UpdateMinMax (nV);
    if (m_bPrint)
    {
        if (m_bFileOpened || m_bUsingFile)
            *FOut << std::setw(m_nW[m_nCurCol]) << std::setiosflags(std::ios::left)
                  << nV;
        else
            std::cout << std::setw(m_nW[m_nCurCol]) << std::setiosflags(std::ios::left)
                      << nV;
    }
    DoNewLine ();
    m_nHit++;
}

void CPrintTable::PrintNext (const int nV, int nIndex)
// ----------------------------------------------------------------------------
// Function: Checks if value is min or max of its column. 
//           Print value to the table and skip to the next column, starting a 
//           new row if necessary.
// Input:   nV      integer value
//          nIndex  index of value
// Output:  none
// ----------------------------------------------------------------------------
{
    UpdateMinMax (nV, nIndex);
    if (m_bPrint)
    {
        if (m_bFileOpened || m_bUsingFile)
            *FOut << std::setw(m_nW[m_nCurCol]) << std::setiosflags(std::ios::left)
                  << nV;
        else
            std::cout << std::setw(m_nW[m_nCurCol]) << std::setiosflags(std::ios::left)
                      << nV;
    }
    DoNewLine ();
    m_nHit++;
}

void CPrintTable::PrintNext (const float fV)
// ----------------------------------------------------------------------------
// Function: Checks if value is min or max of its column. 
//           Print value to the table and skip to the next column, starting a 
//           new row if necessary.
// Input:   fV      float value
// Output:  none
// ----------------------------------------------------------------------------
{
    UpdateMinMax (fV);
    if (m_bPrint)
    {
        if (m_bFileOpened || m_bUsingFile)
            *FOut << std::setw(m_nW[m_nCurCol]) << std::setiosflags(std::ios::left)
                  << fV;
        else
            std::cout << std::setw(m_nW[m_nCurCol]) << std::setiosflags(std::ios::left)
                      << fV;
    }
    DoNewLine ();
    m_nHit++;
}

void CPrintTable::PrintNext (const float fV, int nIndex)
// ----------------------------------------------------------------------------
// Function: Checks if value is min or max of its column. 
//           Print value to the table and skip to the next column, starting a 
//           new row if necessary.
// Input:   fV      float value
//          nIndex  index of value
// Output:  none
// ----------------------------------------------------------------------------
{
    UpdateMinMax (fV, nIndex);
    if (m_bPrint)
    {
        if (m_bFileOpened || m_bUsingFile)
            *FOut << std::setw(m_nW[m_nCurCol]) << std::setiosflags(std::ios::left)
                  << fV;
        else
            std::cout << std::setw(m_nW[m_nCurCol]) << std::setiosflags(std::ios::left)
                      << fV;
    }
    DoNewLine ();
    m_nHit++;
}

void CPrintTable::PrintNext (const double dV)
// ----------------------------------------------------------------------------
// Function: Checks if value is min or max of its column. 
//           Print value to the table and skip to the next column, starting a 
//           new row if necessary.
// Input:   dV      double precision value
// Output:  none
// ----------------------------------------------------------------------------
{
    UpdateMinMax (dV);
    if (m_bPrint)
    {
        if (m_bFileOpened || m_bUsingFile)
            *FOut << std::setw(m_nW[m_nCurCol]) << std::setiosflags(std::ios::left)
                  << dV;
        else
            std::cout << std::setw(m_nW[m_nCurCol]) << std::setiosflags(std::ios::left)
                      << dV;
    }
    DoNewLine ();
    m_nHit++;
}

void CPrintTable::PrintNext (const double dV, int nIndex)
// ----------------------------------------------------------------------------
// Function: Checks if value is min or max of its column. 
//           Print value to the table and skip to the next column, starting a 
//           new row if necessary.
// Input:   dV      double precision value
//          nIndex  index of value
// Output:  none
// ----------------------------------------------------------------------------
{
    UpdateMinMax (dV, nIndex);
    if (m_bPrint)
    {
        if (m_bFileOpened || m_bUsingFile)
            *FOut << std::setw(m_nW[m_nCurCol]) << std::setiosflags(std::ios::left)
                  << dV;
        else
            std::cout << std::setw(m_nW[m_nCurCol]) << std::setiosflags(std::ios::left)
                      << dV;
    }
    DoNewLine ();
    m_nHit++;
}

void CPrintTable::PrintNext (const char* szV)
// ----------------------------------------------------------------------------
// Function: Print value to the table and skip to the next column, starting a 
//           new row if necessary.
// Input:   szV     text 
// Output:  none
// ----------------------------------------------------------------------------
{
    if (m_bPrint)
    {
        if (m_bFileOpened || m_bUsingFile)
            *FOut << std::setw(m_nW[m_nCurCol]) << std::setiosflags(std::ios::left)
                  << szV;
        else
            std::cout << std::setw(m_nW[m_nCurCol]) << std::setiosflags(std::ios::left)
                      << szV;
    }
    DoNewLine ();
    m_nHit++;
}

void CPrintTable::PrintNext (const std::string& szV)
// ----------------------------------------------------------------------------
// Function: Formats and prints next item in line
// Input:   szV     string text 
// Output:  none
// ----------------------------------------------------------------------------
{
    if (m_bPrint)
    {
        if (m_bFileOpened || m_bUsingFile)
            *FOut << std::setw(m_nW[m_nCurCol]) << std::setiosflags(std::ios::left)
                  << szV;
        else
            std::cout << std::setw(m_nW[m_nCurCol]) << std::setiosflags(std::ios::left)
                      << szV;
    }
    DoNewLine ();
    m_nHit++;
}

void CPrintTable::PCopy(const int nWidth[])
// ----------------------------------------------------------------------------
// Function: Copy in a new set of column widths
// Input:  nWidth   column widths
// Output: none
// ----------------------------------------------------------------------------
{
    for (int i = 0; i < m_nCols; i++)
        m_nW[i] = nWidth[i];
}

void CPrintTable::DoNewLine ()
// ----------------------------------------------------------------------------
// Function: Move to the next column, skipping to the next line if necessary.
// Input:  none
// Output: none
// ----------------------------------------------------------------------------
{
    if (!m_bPrint)
    {
        m_nCurCol++;
        if (m_nCurCol == m_nCols)
        {
            m_nCurLine++;
            m_nCurCol = 0;
        }
        return;
    }
    m_nCurCol++;
    if (m_nCurCol == m_nCols)
    {
        if (m_bFileOpened || m_bUsingFile)
            *FOut << "\n";
        else
            std::cout << "\n";
        m_nCurLine++;
        m_nCurCol = 0;
        SkipSpaces();
    }
}

void CPrintTable::SetPrint (bool bPrint)
// ----------------------------------------------------------------------------
// Function: Set print option
// Input:  bPrint       true or false
// Output: m_bPrint     updated
// ----------------------------------------------------------------------------
{
    m_bPrint = bPrint;
}

const char* CPrintTable::whitespace (void)
// ----------------------------------------------------------------------------
// Function: Outputs white spaces
// Input:    None
// Output:   Returns tab and white spaces
// ----------------------------------------------------------------------------
{
    return " \t\r\n";
}

const std::string& CPrintTable::TrimLeft (std::string& myString)
// ----------------------------------------------------------------------------
// Function: trims all leading blanks
// Input:    string
// Output:   modified string
// ----------------------------------------------------------------------------
{
    myString.erase (0, myString.find_first_not_of(whitespace()));
    return myString;
}

const std::string& CPrintTable::TrimRight (std::string& myString)
// ----------------------------------------------------------------------------
// Function: trims all trailing blanks
// Input:    string
// Output:   modified string
// ----------------------------------------------------------------------------
{
    myString.erase (myString.find_last_not_of(whitespace()) + 1);
    return myString;
}

const std::string& CPrintTable::ConvertUpper (std::string& myString)
// ----------------------------------------------------------------------------
// Function: converts all characters to upper case
// Input:    string
// Output:   modified string
// ----------------------------------------------------------------------------
{
    for (std::string::size_type idx = 0; idx < myString.length(); ++idx)
        myString[idx] = static_cast<char>(toupper(myString[idx]));
    return myString;
}

const std::string& CPrintTable::ConvertLower (std::string& myString)
// ----------------------------------------------------------------------------
// Function: converts all characters to lower case
// Input:    string
// Output:   modified string
// ----------------------------------------------------------------------------
{
    for (std::string::size_type idx = 0; idx < myString.length(); ++idx)
        myString[idx] = static_cast<char>(tolower(myString[idx]));
    return myString;
}

const std::string& CPrintTable::strCopy (std::string& myString,
                                         const std::string& strSource,
                                         int first, int last)
// ----------------------------------------------------------------------------
// Function: Copies the specified substring from source to destination
// Input:    myString (destination), strSource (source), first and last chars
//           (locations) to be copied
// Output:   modified string
// ----------------------------------------------------------------------------
{
    myString = strSource.substr (first, last + 1 - first);
    return myString;
}

const std::string& CPrintTable::Format (std::string& myString,
                                        const char* pcszFormat, ...)
// ----------------------------------------------------------------------------
// Function: Formats the string as per user defined format
// Input:    myString, C-style format in pcszFormat
// Output:   modified string (myString)
// ----------------------------------------------------------------------------
{
    const size_t MAXCHARS = 256;
    char buffer[MAXCHARS + 1];
    buffer[MAXCHARS] = '\0';
    va_list argList;
    va_start (argList, pcszFormat);
//#ifdef WIN32
//    _vsnprintf (buffer, MAXCHARS, pcszFormat, argList);
//#else
    vsnprintf (buffer, MAXCHARS, pcszFormat, argList);
//#endif
    va_end(argList);
    myString = buffer;
    return myString;
}

// ==================== Error Handler ========================
void CPrintTable::ErrorHandler (const int nErrorCode) const
// ----------------------------------------------------------------------------
// Function: Prints errors and aborts program
// Input:    nErrorCode   type of error
// Output:   none
// ----------------------------------------------------------------------------
{
    throw CGlobalErrorHandler::TABLECREATIONERROR;
}

