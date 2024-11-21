/*********************************************
Utility Library Function
Copyright(c) 2006-23, S. D. Rajan
All rights reserved

Object-Oriented Numerical Analysis
*********************************************/
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <cfenv>
#include "GlobalErrorHandler.h"

int CGlobalErrorHandler::m_nLineNumber = 0;

CGlobalErrorHandler::CGlobalErrorHandler()
// ---------------------------------------------------------------------------
// Function: ctor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    std::feclearexcept(FE_OVERFLOW);
    std::feclearexcept(FE_UNDERFLOW);
}

CGlobalErrorHandler::~CGlobalErrorHandler()
// ---------------------------------------------------------------------------
// Function: dtor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{

}

bool CGlobalErrorHandler::OverFlowDetected ()
// ---------------------------------------------------------------------------
// Function: checks to see if floating overflow has occurred or not
// Input:    none
// Output:   true if the overflow has occurred
//           false otherwise
// ---------------------------------------------------------------------------
{
    return (bool)std::fetestexcept(FE_OVERFLOW);
}

bool CGlobalErrorHandler::UnderFlowDetected ()
// ---------------------------------------------------------------------------
// Function: checks to see if floating underflow has occurred or not
// Input:    none
// Output:   true if the underflow has occurred
//           false otherwise
// ---------------------------------------------------------------------------
{
    return (bool)std::fetestexcept(FE_UNDERFLOW);
}

void CGlobalErrorHandler::ErrorHandler (ERRORCODE& ErCode)
// ---------------------------------------------------------------------------
// Function: Displays the error message on the error stream
// Input:    Error #
// Output:   none
// ---------------------------------------------------------------------------
{
    ErrorHandler (std::cerr, ErCode);
}

void CGlobalErrorHandler::ErrorHandler (std::ostream& OF, ERRORCODE& ErCode)
// ---------------------------------------------------------------------------
// Function: Displays the error message on the error stream
// Input:    Error #
// Output:   none
// ---------------------------------------------------------------------------
{
    OF << '\n';
    std::ostringstream strMessage;

    if (ErCode == CANNOTOPENIFILE)
        strMessage << "Cannot open specified input file.";
    else if (ErCode == CANNOTOPENOFILE)
        strMessage << "Cannot open specified output file.";
    else if (ErCode == INVALIDCMDLINEARGUMENTS)
        strMessage << "Command line needs input_file output_file.";
    else if (ErCode == FLOATEXCEPTION)
        strMessage << "Floating point exception. Check input data.";
    else if (ErCode == MAXCHARSEXCEEDED)
        strMessage << "Input line is too long.";
    else if (ErCode == ENDOFFILE)
        strMessage << "End of file while reading input.";
    else if (ErCode == FILEBADERROR)
        strMessage << "Unrecoverable file read error.";
    else if (ErCode == MISSINGENDQUOTE)
        strMessage << "Missing end quote in input.";
    else if (ErCode == INVALIDLONGVALUE)
        strMessage << "Invalid int or long value.";
    else if (ErCode == INVALIDDOUBLEVALUE)
        strMessage << "Invalid float or double value.";
    else // 
        strMessage << "Unknown error.";

    std::string strM = strMessage.str();
    OF << strM << "\n";
}

void CGlobalErrorHandler::ErrorHandler (CArrayBase::ERRORVM& err)
// ---------------------------------------------------------------------------
// Function: Displays the CVector/CMatrix error message on the error stream
// Input:    Error #
// Output:   none
// ---------------------------------------------------------------------------
{
    CArrayBase::ErrorHandler (std::cerr, err);
}

void CGlobalErrorHandler::ErrorHandler (std::ostream& OF, 
                                        CArrayBase::ERRORVM& err)
// ---------------------------------------------------------------------------
// Function: Displays the CVector/CMatrix error message on the error stream
// Input:    Error #
// Output:   none
// ---------------------------------------------------------------------------
{
    CArrayBase::ErrorHandler (OF, err);
}

void CGlobalErrorHandler::ErrorHandler (std::exception& err)
// ---------------------------------------------------------------------------
// Function: Displays the standard error message on the error stream
// Input:    Error #
// Output:   none
// ---------------------------------------------------------------------------
{
    std::cerr << err.what() << "\n";
}

void CGlobalErrorHandler::ErrorHandler (std::ostream& OF, std::exception& err)
// ---------------------------------------------------------------------------
// Function: Displays the standard error message on the error stream
// Input:    Error #
// Output:   none
// ---------------------------------------------------------------------------
{
    OF << "Caught " << typeid(err).name()
       << " : " << err.what() << "\n";
}

std::string CGlobalErrorHandler::CustomErrorMessage (const char* str1,
                                                     const char* str2)
// ---------------------------------------------------------------------------
// Function: 
// Input:    
// Output:   
// ---------------------------------------------------------------------------
{
    std::string strM = str1;
    strM += str2;
    return strM;
}

std::string CGlobalErrorHandler::CustomErrorMessage (const char* str1)
// ---------------------------------------------------------------------------
// Function: 
// Input:    
// Output:   
// ---------------------------------------------------------------------------
{
    std::string strM = what();
    strM += str1;
    return strM;
}
