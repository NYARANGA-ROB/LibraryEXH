/*********************************************
Utility Library Function
Copyright(c) 2006-23, S. D. Rajan
All rights reserved

Object-Oriented Numerical Analysis
*********************************************/
#include <iostream>
#include <sstream>
#include "arraybasecontainerEXH.h"

std::string CArrayBase::m_strName;
double CArrayBase::m_dAllocated = 0.0;
double CArrayBase::m_dDeAllocated = 0.0;
long   CArrayBase::m_ValueR = 0;
long   CArrayBase::m_ValueC = 0;
double CArrayBase::m_dASOP = 0.0;
double CArrayBase::m_dMOP = 0.0;
double CArrayBase::m_dDOP = 0.0;

CArrayBase::CArrayBase ()
// ----------------------------------------------------------------------------
// Function: constructor
// Input:    None 
// Output:   None
// ----------------------------------------------------------------------------
{
    m_ValueR = m_ValueC = 0;
}

CArrayBase::~CArrayBase ()
// ----------------------------------------------------------------------------
// Function: destructor
// Input:    None 
// Output:   None
// ----------------------------------------------------------------------------
{
}

void CArrayBase::ShowStatistics (std::ostream& OF)
// ----------------------------------------------------------------------------
// Function: show statistics
// Input:    None 
// Output:   None
// ----------------------------------------------------------------------------
{
    OF << "\n"
       << "  Allocated : " << m_dAllocated << " bytes\n"
       << "DeAllocated : " << m_dDeAllocated << " bytes\n";
}

void CArrayBase::Reset ()
// ----------------------------------------------------------------------------
// Function: resets the stats
// Input:    None 
// Output:   None
// ----------------------------------------------------------------------------
{
    m_dAllocated = m_dDeAllocated = 0.0;
}

void CArrayBase::ErrorHandler (ERRORVM ErrorCode)
// ---------------------------------------------------------------------------
// Function: channels error message via std:err
// Input:    error code 
// Output:   none
// ---------------------------------------------------------------------------
{
    ErrorHandler (std::cerr, ErrorCode);
}

void CArrayBase::ErrorHandler (std::ostream& OF, ERRORVM ErrorCode)
// ---------------------------------------------------------------------------
// Function: channels error message via std:err
// Input:    error code 
// Output:   none
// ---------------------------------------------------------------------------
{
    std::ostringstream strMessage;
    if (!m_strName.empty())
        strMessage << "Array name: " << m_strName << ". ";
    switch (ErrorCode)
    {
        case MATRIX_ALLOCATION_ERROR:
            strMessage << "CMatrix:: Memory allocation failure: " << m_ValueR << ".";
            break;
        case MATRIX_INDEX_OUT_OF_BOUNDS:
            strMessage << "CMatrix::Row-Column (" << m_ValueR << ", "
                      << m_ValueC << ") is out of bounds.";
            break;
        case MATRIX_INVALID_SIZE:
            strMessage << "CMatrix::Constructor. Invalid number of rows "
                         "or columns: " << m_ValueR << " and " << m_ValueC << ".\n";
            break;
        case MATRIX_INCOMPATIBLE_MATRICES:
            strMessage << "CMatrix::Incompatible matrices. ";
            break;
        case MATRIX_NOT_ENOUGH_INITIALIZERS:
            strMessage << "CMatrix::Constructor. Not enough initializers.";
            break;
        case MATRIX_DEPENDENT_EQUATIONS:
            strMessage << "CMatrix::Equations are linearly dependent.";
            break;
        case MATRIX_NOT_POSITIVEDEFINITE:
            strMessage << "CMatrix::Coefficient matrix is not positive definite.";
            break;
        case VECTOR_ALLOCATION_ERROR:
            strMessage << "Vector:: Memory allocation failure. ";
            break;
        case VECTOR_INVALID_SIZE:
            strMessage << "CVector::Constructor. Invalid number of rows "
                      << m_ValueR << ".\n";
            break;
        case VECTOR_INCOMPATIBLE_VECTORS:
            strMessage << "CVector::Incompatible vectors.\n";
            break;
        case VECTOR_INDEX_OUT_OF_BOUNDS:
            strMessage << "CVector::Vector index " << m_ValueR << " is out of bounds.\n";
            break;
    }
    OF << strMessage.str();
}
