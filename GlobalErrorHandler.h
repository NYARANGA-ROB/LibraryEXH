/*********************************************
Utility Library Function
Copyright(c) 2006-21, S. D. Rajan
All rights reserved

Object-Oriented Numerical Analysis
*********************************************/
/*
All exceptions thrown by components of the C++ Standard library throw exceptions derived
from this exception class. These are:

exception	       description
---------          -----------
bad_alloc	       thrown by new on allocation failure
bad_cast	       thrown by dynamic_cast when it fails in a dynamic cast
bad_exception	   thrown by certain dynamic exception specifiers
bad_typeid	       thrown by typeid
bad_function_call  thrown by empty function objects
bad_weak_ptr	   thrown by shared_ptr when passed a bad weak_ptr

Also deriving from exception, header <exception> defines two generic exception types that
can be inherited by custom exceptions to report errors:

exception	    description
---------       -----------
logic_error	    error related to the internal logic of the program
                domain_error       Reports violations of a precondition.
                invalid_argument   Indicates an invalid argument to the function from which it is thrown.
                length_error       Indicates an attempt to produce an object whose length
                                   is greater than or equal to npos (the largest
                                   representable value of context s size type, usually std::size_t).
                out_of_range       Reports an out-of-range argument.
                bad_cast           Thrown for executing an invalid dynamic_cast
                                   expression in runtime type identification
                bad_typeid         Reports a null pointer p in an
                                   expression typeid(*p).

runtime_error	error detected during runtime
                range_error      reports violation of a postcondition
                overflow_error   reports an arithmetic overflow
                bad_alloc        reports a failure to allocate storage
*/
#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <exception>
#include "arraybasecontainerEXH.h"

class CGlobalErrorHandler : public std::exception
{
    public:
        CGlobalErrorHandler ();
        ~CGlobalErrorHandler ();

        enum ERRORCODE
        {
            CANNOTOPENIFILE, CANNOTOPENOFILE,
            INVALIDCMDLINEARGUMENTS, FLOATEXCEPTION,
            MAXCHARSEXCEEDED, ENDOFFILE, FILEBADERROR,
            MISSINGENDQUOTE, INVALIDLONGVALUE,
            INVALIDDOUBLEVALUE, TABLECREATIONERROR
        };

        void static ErrorHandler (std::ostream& OF, ERRORCODE& err);
        void static ErrorHandler (ERRORCODE& err);
        void static ErrorHandler (std::ostream& OF, CArrayBase::ERRORVM& err);
        void static ErrorHandler (CArrayBase::ERRORVM& err);
        void static ErrorHandler (std::ostream& OF, std::exception& err);
        void static ErrorHandler (std::exception& err);
        bool OverFlowDetected ();
        bool UnderFlowDetected ();
        template <class T>
        bool static FloatException (T x)
        // ---------------------------------------------------------------------------
        // Function: checks to see if the floating point number has an abnormal value
        // Input:    double
        // Output:   true if the value is abnormal
        //           false otherwise
        // ---------------------------------------------------------------------------
        {
            switch (std::fpclassify(x))
            {
            case FP_INFINITE:  // Infinite, e.g. 1.0/0.0, 2*DBL_MAX
                return true;
            case FP_NAN:       // Not-a-Number, e.g. 0.0/0.0, sqrt(-1.0)
                return true;
            case FP_SUBNORMAL: // Subnormal, e.g. DBL_MIN/2
                return true;
            case FP_ZERO:      // Zero, e.g. -0.0
                return true;
            case FP_NORMAL:    // normal value
                return false;
            }

            return false;
        }

    protected:
        virtual const char* what() const noexcept
        {
            return "Exception thrown.\n";
        };
        static int m_nLineNumber;
        std::string CustomErrorMessage (const char* str);
        std::string CustomErrorMessage (const char* str, const char* str2);
        template <class T>
        std::string CustomErrorMessage (const char* str1, const T& str2)
        {
            std::ostringstream strM;
            strM << str1 << str2;
            return strM.str();
        }
};


