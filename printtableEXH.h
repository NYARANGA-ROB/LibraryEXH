/******************************************************************************
Utility Library Function
Copyright(c) 2003-23, S. D. Rajan
All rights reserved

Object-Oriented Numerical Analysis
*********************************************

Contains CPrintTable class definitions for functions that print data in tabular
form to specified output file.
******************************************************************************/
#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>
#include "ArrayContainersEXH.h"         // vector and matrix containers

class CPrintTable
{
    public:
        CPrintTable (const int);                     // constructor (overloaded)
        CPrintTable (const int, std::ofstream*);     // constructor (overloaded)
        CPrintTable (std::ofstream*);                // constructor
        CPrintTable (const std::string& szFileName); // constructor
        CPrintTable (const int nColumns, 
                     const std::string& szFileName);
        ~CPrintTable ();                             // destructor

        enum TFORMAT { CENTERTEXT = 0, ASIS = 1, RIGHTJUSTIFY = 2 };
        static const int MAXCOLS      = 17;
        static const int MAXWIDTH     = 140;
        static const int MAXCOLWIDTH  = 20;

        // helper functions
        void Initialize ();
        void SetSize (const int nColumns);
        void PrintTableHeading (const char *, 
                                const std::string[], 
                                const int nWidth[], int);
        void PrintTableHeading (const std::string&, 
                                const std::string[], 
                                const int nWidth[], int);
        void PrintNext (CVector<std::string>& szV, int i);
        void PrintNext (const int);
        void PrintNext (const int nV, int nIndex);
        void PrintNext (const float);
        void PrintNext (const float fV, int nIndex);
        void PrintNext (const double);
        void PrintNext (const double, int nIndex);
        void PrintNext (const char*);
        void PrintNext (const std::string&);

        void PrintNextLine (const int, const char*);
        void PrintNextLine (const int, const std::string&);
        void PrintNextLine (const int, const char*, const std::string&);
        void PrintNextLine (const int, const char*, int);
        void PrintNextLine (const int, const char*, long);
        void PrintNextLine (const int, const char*, float);
        void PrintNextLine (const int, const char*, double);
        void PrintNextLine (const int, const char*, const double, 
                            const char *);

        void SkipLinesAndSpaces (const int n) const;
        void SkipLines (const int n) const;
        void GetStatistics (CVector<double>& dMin, CVector<double>& dMax,
                        CVector<double>& dSum, CVector<double>& dAbsSum, 
                        CVector<int>& nMinIndex, CVector<int>& nMaxIndex,
                        int& nHit) const;
        
        void UpdateCurLine ();
        void AllDone ();
        void SetPrint (bool);
        void PCopy (const int nWidth[]);

        enum DataType {TYPEUNDEFINED, TYPEINT, TYPELONG, TYPEFLOAT, TYPEDOUBLE};

    private: 
        // member variables
        std::ofstream* FOut;                // ofstream to which to print           
        int            m_nCols;             // number of columns in table
        std::string    m_szTemp;            // temporary string variable
        int            m_nCurCol;           // Current column number
        int            m_nW[MAXCOLS];       // character-width of each column
        DataType       m_DVType[MAXCOLS];   // vector of data types
        bool           m_bPrint;            // should print current data or not
        bool           m_bFileOpened;       // file opened in this class
        bool           m_bUsingFile;        // using output file or not
        
        // statistics related 
        int         m_nVMaxIndex[MAXCOLS];  // indices of max values per column
        int         m_nVMinIndex[MAXCOLS];  // indices of min values per column
        double      m_dVMaxValue[MAXCOLS];  // max values per column
        double      m_dVMinValue[MAXCOLS];  // min values per column
        double      m_dVSum[MAXCOLS];       // sums per column
        double      m_dVAbsSum[MAXCOLS];    // absolute sums per column
        int         m_nTotalWidth;          // total width of entire table
        int         m_nMargin;              // On left/right, slack between 
                                            // MAXWIDTH and nTotalWidth.
        int         m_nCurLine;             // current line
        int         m_nHit;                 // number of datalines in table
        
        // member functions
        void DoNewLine ();
        void SkipSpaces () const;
        void SkipSpaces (const int) const;
        void ErrorHandler (const int nErrorCode) const;
        void NoDataInTable () const;

        // templates for statistics
        template <class T>
        void UpdateMinMax (const T V)
        {
            if (m_nCurLine == 0)
            {
                m_nVMaxIndex[m_nCurCol] = m_nCurLine + 1;
                m_nVMinIndex[m_nCurCol] = m_nCurLine + 1;
                m_dVMaxValue[m_nCurCol] = V;
                m_dVMinValue[m_nCurCol] = V;
            }
            else
            {
                if (V > T(m_dVMaxValue[m_nCurCol]))
                {
                    m_nVMaxIndex[m_nCurCol] = m_nCurLine + 1;
                    m_dVMaxValue[m_nCurCol] = V;
                }
                if (V < T(m_dVMinValue[m_nCurCol]))
                {
                    m_nVMinIndex[m_nCurCol] = m_nCurLine + 1;
                    m_dVMinValue[m_nCurCol] = V;
                }
            }
            m_dVSum[m_nCurCol] += V;
            m_dVAbsSum[m_nCurCol] += fabs((double)V);
        }
        template <class T>
        void UpdateMinMax (const T V, int nIndex)
        {
            if (m_nVMaxIndex[m_nCurCol] == 0)
            {
                m_nVMaxIndex[m_nCurCol] = nIndex;
                m_nVMinIndex[m_nCurCol] = nIndex;
                m_dVMaxValue[m_nCurCol] = V;
                m_dVMinValue[m_nCurCol] = V;
            }
            else
            {
                if (V > T(m_dVMaxValue[m_nCurCol]))
                {
                    m_nVMaxIndex[m_nCurCol] = nIndex;
                    m_dVMaxValue[m_nCurCol] = V;
                }
                if (V < T(m_dVMinValue[m_nCurCol]))
                {
                    m_nVMinIndex[m_nCurCol] = nIndex;
                    m_dVMinValue[m_nCurCol] = V;
                }
            }
            m_dVSum[m_nCurCol] += V;
            m_dVAbsSum[m_nCurCol] += fabs((double)V);
        }

        // string extension functions
        const char* whitespace(void);
        const std::string& TrimLeft     (std::string& myString);
        const std::string& TrimRight    (std::string& myString);
        const std::string& ConvertUpper (std::string& myString);
        const std::string& ConvertLower (std::string& myString);
        const std::string& strCopy (std::string& myString, const std::string& strSource,
                                    int first, int last);
        const std::string& Format(std::string& myString, const char* pcszFormat, ... );
};
