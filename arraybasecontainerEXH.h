/*********************************************
Utility Library Function
Copyright(c) 2006-23, S. D. Rajan
All rights reserved

Object-Oriented Numerical Analysis
*********************************************/
#pragma once
#include <ostream>
#include <string>

class CArrayBase
{
    public:
        CArrayBase ();
        ~CArrayBase ();
        // define vector and matrix errors to handle via exception handling
        enum ERRORVM
        {
            MATRIX_ALLOCATION_ERROR,
            MATRIX_INDEX_OUT_OF_BOUNDS,
            MATRIX_INVALID_SIZE,
            MATRIX_INCOMPATIBLE_MATRICES,
            MATRIX_NOT_ENOUGH_INITIALIZERS,
            MATRIX_DEPENDENT_EQUATIONS,
            MATRIX_NOT_POSITIVEDEFINITE,
            VECTOR_ALLOCATION_ERROR,
            VECTOR_INVALID_SIZE,
            VECTOR_INCOMPATIBLE_VECTORS,
            VECTOR_INDEX_OUT_OF_BOUNDS
        };
        void static ShowStatistics (std::ostream& OF);
        void static Reset ();
        static void ErrorHandler (std::ostream& OF,
                                  ERRORVM); // central function
                                            // to handle all errors
        static void ErrorHandler (ERRORVM); // central function
                                            // to handle all errors

    protected:
        static std::string m_strName;       // vector/matrix name
        static double m_dAllocated;         // memory allocation
        static double m_dDeAllocated;       // memory deallocation
        static long   m_ValueR, m_ValueC;   // row and column indices
        static double m_dASOP;              // addition and subtraction OPs
        static double m_dMOP, m_dDOP;       // multiplication and division OPs 
};

