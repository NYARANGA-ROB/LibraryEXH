/*********************************************
Utility Library Function
Copyright(c) 2006-23, S. D. Rajan
All rights reserved

Object-Oriented Numerical Analysis
*********************************************/
#include "getinteractiveEXH.h"
#include <algorithm>
#include <iostream>
#include <cctype>
#include <exception>

CGetInteractive::CGetInteractive()
{
    m_strDelimiters = ", ";
}

//=======================================================
//=======================================================
//===================== int version =====================
//=======================================================
//=======================================================
void CGetInteractive::GetInteractive (const std::string& szPrompt,
                                      int nUV[], 
                                      int nSize)
{
    std::string szInput;
    std::string szCurrentString;
    bool bError = false;
    INPERROR nState = CGetInteractive::INPERROR::NONE;

    do 
    {
        try
        {
            DisplayandRead (szPrompt, szInput);
            Trim (szInput);
    
            int nV;
            bError = false;
            // get location of the first character that is not a delimiter
            auto start_loc = szInput.find_first_not_of(m_strDelimiters);
            for (int i=0; i < nSize; i++)
            {
                auto end_loc = szInput.find_first_of(m_strDelimiters, start_loc);
                szCurrentString = szInput.substr(start_loc, end_loc - start_loc);
                if ((nState = GetIntValue (szCurrentString, nV)) != CGetInteractive::INPERROR::NONE)
                {
                    bError = true;
                    ErrorHandler (nState, -INT_MAX, INT_MAX);
                    break;
                }
                nUV[i] = nV;
                start_loc = szInput.find_first_not_of (m_strDelimiters, end_loc);
                if (start_loc == std::string::npos && i != nSize - 1)
                {
                    bError = true;
                    ErrorHandler (CGetInteractive::INPERROR::NOTENOUGHINPUT, -INT_MAX, INT_MAX);
                    break;
                }
            }
        }
        catch (std::exception& err)
        {
            std::cout << err.what() << "\n";
        }
        catch (...)
        {
            std::cout << "Unknown input error ...\n";
        }
    } while (bError);
}

void CGetInteractive::GetInteractive (const std::string& szPrompt, int& nV)
{
    GetInteractive (szPrompt, nV, -INT_MAX, INT_MAX);
}

void CGetInteractive::GetInteractive (const std::string& szPrompt, 
                                      int& nV,
                                      int nL, int nU)
{
    // range correct?
    if (nL < -INT_MAX || nU > INT_MAX)
        ErrorHandler (CGetInteractive::INPERROR::INVALIDBOUNDS, 0, 0);
    
    // initialize
    int nUV[1];

    do
    {
        GetInteractive(szPrompt, nUV, 1);
    } while (nUV[0] < nL || nUV[0] > nU);

    // convert to scalar int and return value to user
    nV = nUV[0];
}

CGetInteractive::INPERROR CGetInteractive::GetIntValue (const std::string& szUserInput,
                                                        int& nV)
{
    // initialize
    int nSign = 1;	                  // assume positive input
    int nPos = 0;	                  // current parsing position
    std::string szTemp;               // temporary string
    INPERROR State = INPERROR::NONE;  // error state

    // trim leading and trailing blanks
    std::string szInput = szUserInput;
    Trim (szInput);

    // first character + or -
    if (szInput[nPos] == '-')
    {
        nSign = -1;		// negative integer
        nPos++;
    }
    else if (szInput[nPos] == '+')
    {
        nPos++;			// ignore + sign
    }

    // extract string without + or -
    int nLast = static_cast<int>(szInput.length());
    szTemp = szInput.substr(nPos, nLast-nPos);
    // trim leading zeros
    if (nLast > 1) 
        TrimLeadingZeros (szTemp);

    // rest of the string is legal?
    int nLoc = static_cast<int>(szTemp.find_first_not_of ("0123456789"));
    if (nLoc >= 0)		// invalid input
        State = INPERROR::INVALIDINPUT;
    // invalid input
    else if (szTemp.length() == 0)
        State = INPERROR::INVALIDINPUT;
    else
    {	
        // legal input so far.
        State = static_cast<INPERROR>(AtoI (szTemp, nV, nSign));
        // account for proper sign
        if (State == INPERROR::NONE)
            nV *= nSign;
    }

    return State;
}

// convert alpha string to int integer
CGetInteractive::INPERROR CGetInteractive::AtoI (const std::string& szTemp, int& nV,
                                                 int nSign)
{
    int nC = 0;
    nV = 0;
    int nLen = static_cast<int>(szTemp.length());

    // capacity of a long number
    int nVMAX = (nSign >= 1 ? INT_MAX : INT_MAX);
    std::string szT;
    int nChars = NumChars(nVMAX, szT);
    if (nLen > nChars)
        return (INPERROR::TOOMANYCHARACTERS);
    else if (nLen == nChars)
    {
        if (szT < szTemp) return (INPERROR::TOOMANYCHARACTERS);
    }
        
    // Example: value of xyz is
    //          z*10**0 + y*10**1 + z*10**2
    for (int i=nLen-1; i >= 0; i--)
    {
        nV += static_cast<int>((szTemp[i] - '0')*pow(10.0, nC));
        nC++;
    }

    return INPERROR::NONE;
}

//=======================================================
//=======================================================
//==================== long version =====================
//=======================================================
//=======================================================
void CGetInteractive::GetInteractive (const std::string& szPrompt,
                                      long lUV[], 
                                      int nSize)
{
    std::string szInput;
    std::string szCurrentString;
    bool bError = false;
    INPERROR State = INPERROR::NONE;

    do 
    {
        try
        {
            DisplayandRead (szPrompt, szInput);
            Trim (szInput);
    
            long lV;
            bError = false;
            // get location of the first character that is not a delimiter
            auto start_loc = szInput.find_first_not_of(m_strDelimiters);
            for (int i=0; i < nSize; i++)
            {
                auto end_loc = szInput.find_first_of(m_strDelimiters, start_loc);
                szCurrentString = szInput.substr(start_loc, end_loc - start_loc);
                if ((State = static_cast<INPERROR>(GetLongValue (szCurrentString, lV)))
                            != INPERROR::NONE)
                {
                    bError = true;
                    ErrorHandler (State, -LONG_MAX, LONG_MAX);
                    break;
                }
                lUV[i] = lV;
                start_loc = szInput.find_first_not_of (m_strDelimiters, end_loc);
                if (start_loc == std::string::npos && i != nSize - 1)
                {
                    bError = true;
                    ErrorHandler (INPERROR::NOTENOUGHINPUT, -LONG_MAX, LONG_MAX);
                    break;
                }
            }
        }
        catch (std::exception& err)
        {
            std::cout << err.what() << "\n";
        }
        catch (...)
        {
            std::cout << "Unknown input error ...\n";
        }
    } while (bError);
}


void CGetInteractive::GetInteractive (const std::string& szPrompt, 
                                      long& lV)
{
    GetInteractive (szPrompt, lV, -LONG_MAX, LONG_MAX);
}

void CGetInteractive::GetInteractive (const std::string& szPrompt, 
                                      long& lV,
                                      long lL, long lU)
{
    // range correct?
    if (lL < -LONG_MAX || lU > LONG_MAX)
        ErrorHandler (INPERROR::INVALIDBOUNDS, 0, 0);
    
    // initialize
    long lUV[1];

    do
    {
        GetInteractive (szPrompt, lUV, 1);
    } while (lUV[0] < lL || lUV[0] > lU);

    // convert to scalar int and return value to user
    lV = lUV[0];
}

CGetInteractive::INPERROR CGetInteractive::GetLongValue (const std::string& szUserInput,
                                                         long& lV)
{
    // initialize
    int nSign = 1;	                  // assume positive input
    int nPos = 0;	                  // current parsing position
    std::string szTemp;	              // temporary string
    INPERROR State = INPERROR::NONE;  // error state

    // trim leading and trailing blanks
    std::string szInput = szUserInput;
    Trim (szInput);

    // first character + or -
    if (szInput[nPos] == '-')
    {
        nSign = -1;		// negative integer
        nPos++;
    }
    else if (szInput[nPos] == '+')
    {
        nPos++;			// ignore + sign
    }

    // extract string without + or -
    int nLast = static_cast<int>(szInput.length());
    szTemp = szInput.substr(nPos, nLast-nPos);
    // trim leading zeros
    if (nLast > 1) 
        TrimLeadingZeros (szTemp);

    // rest of the string is legal?
    int nLoc = static_cast<int>(szTemp.find_first_not_of ("0123456789"));
    if (nLoc >= 0)		// invalid input
        State = INPERROR::INVALIDINPUT;	
    // invalid input
    else if (szTemp.length() == 0)
        State = INPERROR::INVALIDINPUT;
    else
    {	
        // legal input so far.
        State = AtoL (szTemp, lV, nSign);
        // account for proper sign
        lV *= nSign;
    }

    return State;
}

// convert alpha string to int integer
CGetInteractive::INPERROR CGetInteractive::AtoL (const std::string& szTemp, long& lV,
                                                  int nSign)
{
    int nC = 0;
    lV = 0;
    int nLen = static_cast<int>(szTemp.length());

    // capacity of a long number
    int nVMAX = (nSign >= 1 ? LONG_MAX : LONG_MAX);
    std::string szT;
    int nChars = NumChars(nVMAX, szT);
    if (nLen > nChars)
        return (INPERROR::TOOMANYCHARACTERS);
    else if (nLen == nChars)
    {
        if (szT < szTemp) return (INPERROR::TOOMANYCHARACTERS);
    }
        
    // Example: value of xyz is
    //          z*10**0 + y*10**1 + z*10**2
    for (int i=nLen-1; i >= 0; i--)
    {
        lV += static_cast<long>((szTemp[i] - '0')*pow(10.0, nC));
        nC++;
    }

    return INPERROR::NONE;
}


//=======================================================
//=======================================================
//=================== float version =====================
//=======================================================
//=======================================================
void CGetInteractive::GetInteractive (const std::string& szPrompt,
                                      float fUV[],
                                      int nSize)
{
    std::string szInput;
    std::string szCurrentString;
    bool bError = false;
    INPERROR State = INPERROR::NONE;

    do 
    {
        try
        {
            DisplayandRead (szPrompt, szInput);
            Trim (szInput);
    
            float fV;
            bError = false;
            // get location of the first character that is not a delimiter
            auto start_loc = szInput.find_first_not_of(m_strDelimiters);
            for (int i=0; i < nSize; i++)
            {
                auto end_loc = szInput.find_first_of (m_strDelimiters, start_loc);
                szCurrentString = szInput.substr(start_loc, end_loc - start_loc);
                if ((State = static_cast<INPERROR>(GetFloatValue (szCurrentString, fV)))
                        != INPERROR::NONE)
                {
                    bError = true;
                    ErrorHandler (State, -FLT_MAX, FLT_MAX);
                    break;
                }
                fUV[i] = fV;
                start_loc = szInput.find_first_not_of (m_strDelimiters, end_loc);
                if (start_loc == std::string::npos && i != nSize-1)
                {
                    bError = true;
                    ErrorHandler (INPERROR::NOTENOUGHINPUT, -FLT_MAX, FLT_MAX);
                    break;
                }
            }
        }
        catch (std::exception& err)
        {
            std::cout << err.what() << "\n";
        }
        catch (...)
        {
            std::cout << "Unknown input error ...\n";
        }
    } while (bError);
}

void CGetInteractive::GetInteractive (const std::string& szPrompt,
                                      float& fV)
{
    GetInteractive (szPrompt, fV, -FLT_MAX, FLT_MAX);
}

void CGetInteractive::GetInteractive (const std::string& szPrompt,
                                      float& fV, float fL, float fU)
{
    // range correct?
    if (fL < -FLT_MAX || fU > FLT_MAX)
        ErrorHandler (INPERROR::INVALIDBOUNDS, 0, 0);

    // initialize
    float fUV[1];
    do
    {
        GetInteractive(szPrompt, fUV, 1);
    } while (fUV[0] < fL || fUV[0] > fU);

    // convert to scalar float and return value to user
    fV = fUV[0];
}

CGetInteractive::INPERROR CGetInteractive::GetFloatValue (const std::string& szUserInput,
                                                          float& fV)
{
    // initialize
    float dSignM = 1.0f;             // assume positive mantissa
    float dSignE = 1.0f;             // assume positive exponent
    int nPos = 0;		             // current parsing position
    std::string szTemp;              // temporary string
    INPERROR State = INPERROR::NONE; // error state
    // store exponent, mantissa and fractional components
    std::string szExponent= "", szMantissa = "",
                szFraction = "";
    int nVE, nVM; 
    float fVF;

    // trim leading and trailing blanks
    std::string szInput = szUserInput;
    Trim (szInput);

    // first character + or -
    if (szInput[nPos] == '-')
    {
        dSignM = -1.0;		// negative 
        nPos++;
    }
    else if (szInput[nPos] == '+')
    {
        nPos++;			// ignore + sign
    }

    // extract string without + or - 
    szTemp = szInput.substr(nPos, szInput.length()-nPos);
    int nLast = static_cast<int>(szTemp.length());
    // trim leading zeros (unless the input is 0)
    if (nLast > 1) 
        TrimLeadingZeros (szTemp);

    // is there an exponent?
    int nLocExp = static_cast<int>(szTemp.find_first_of ("eE"));
    if (nLocExp >= 0)
    {   // yes
        szExponent = szTemp.substr(nLocExp+1, nLast-nLocExp);
        szMantissa = szTemp.substr(0, nLocExp);
        nLast = static_cast<int>(szExponent.length());
        if (nLast == 0) // nothing follows e or E
        {
            return INPERROR::INVALIDINPUT;
        }
        // first character + or - in the exponent
        if (szExponent[0] == '-')
        {
            dSignE = -1.0;		// negative 
            szExponent = szExponent.substr(1, nLast-1);
        }
        else if (szExponent[0] == '+')
        {
            szExponent = szExponent.substr(1, nLast-1);
        }
    }
    else // no
        szMantissa = szTemp;

    // is there a decimal?
    int nLocDec = static_cast<int>(szMantissa.find ("."));
    if (nLocDec >= 0)
    {
        nLast = static_cast<int>(szMantissa.length());
        szFraction = szMantissa.substr(nLocDec+1, nLast-1);
        szMantissa = szMantissa.substr(0, nLocDec);
    }

    // valid string?
    int nLocE = static_cast<int>(szExponent.find_first_not_of ("0123456789"));
    int nLocM = static_cast<int>(szMantissa.find_first_not_of ("0123456789"));
    int nLocF = static_cast<int>(szFraction.find_first_not_of ("0123456789"));
    // invalid input
    if (nLocE >= 0 || nLocM >= 0 || nLocF >= 0)
        State = INPERROR::INVALIDINPUT;		
    // invalid input
    else if (szMantissa.length() == 0 && szFraction.length() == 0)
    {
        fV = 0.0;
        return State;
    }
    // valid input
    else
    {	
        int nCharsE = static_cast<int>(szExponent.length()); 
        int nCharsM = static_cast<int>(szMantissa.length()); 
        // trim leading zeros
        if (nCharsE > 0) {
            TrimLeadingZeros (szExponent);
            nCharsE = static_cast<int>(szExponent.length()); 
        }
        if (nCharsM > 0) {
            TrimLeadingZeros (szMantissa);
            nCharsM = static_cast<int>(szMantissa.length()); 
        }
        // value of exponent
        if ((State = AtoI (szExponent, nVE, static_cast<int>(dSignE)))
                != INPERROR::NONE)
            return (State);
        // capacity of a long number
        std::string szT;
        int nChars = NumChars(DBL_MAX_10_EXP, szT);
        if (nCharsE > nChars)
            return (INPERROR::TOOMANYCHARACTERS);
        else if (nCharsE == nChars) {
            if (szT < szExponent) return (INPERROR::TOOMANYCHARACTERS);
        }
            
         // value of mantissa
         if ((State = AtoI (szMantissa, nVM, static_cast<int>(dSignM))) 
                 != INPERROR::NONE)
             return (State);
         // value of fractional component
         AtoDFraction (szFraction, fVF);

         // account for proper sign and final value
         fV = dSignM*(float(nVM) + fVF)*pow(10.0f,dSignE*nVE);
    }

    return State;
}

//=======================================================
//=======================================================
//===================== double version ==================
//=======================================================
//=======================================================
void CGetInteractive::GetInteractive (const std::string& szPrompt,
                                      double dUV[],
                                      int nSize)
{
    std::string szInput;
    std::string szCurrentString;
    bool bError = false;
    INPERROR State = INPERROR::NONE;

    do 
    {
        try
        {
            DisplayandRead (szPrompt, szInput);
            Trim (szInput);
    
            double dV;
            bError = false;
            // get location of the first character that is not a delimiter
            auto start_loc = szInput.find_first_not_of(m_strDelimiters);
            for (int i=0; i < nSize; i++)
            {
                auto end_loc = szInput.find_first_of (m_strDelimiters, start_loc);
                szCurrentString = szInput.substr(start_loc, end_loc - start_loc);
                if ((State = static_cast<INPERROR>(GetDoubleValue (szCurrentString, dV)))
                        != INPERROR::NONE)
                {
                    bError = true;
                    ErrorHandler (State, -DBL_MAX, DBL_MAX);
                    break;
                }
                dUV[i] = dV;
                start_loc = szInput.find_first_not_of (m_strDelimiters, end_loc);
                if (start_loc == std::string::npos && i != nSize-1)
                {
                    bError = true;
                    ErrorHandler (INPERROR::NOTENOUGHINPUT, -DBL_MAX, DBL_MAX);
                    break;
                }
            }
        }
        catch (std::exception& err)
        {
            std::cout << err.what() << "\n";
        }
        catch (...)
        {
            std::cout << "Unknown input error ...\n";
        }
    } while (bError);
}

void CGetInteractive::GetInteractive (const std::string& szPrompt, 
                                      double& dV)
{
    GetInteractive (szPrompt, dV, -DBL_MAX, DBL_MAX);
}

void CGetInteractive::GetInteractive (const std::string& szPrompt,
                                      double& dV, double dL, double dU)
{
    // range correct?
    if (dL < -DBL_MAX || dU > DBL_MAX)
        ErrorHandler (INPERROR::INVALIDBOUNDS, 0, 0);

    // initialize
    double dUV[1];
    do
    {
        GetInteractive(szPrompt, dUV, 1);
    } while (dUV[0] < dL || dUV[0] > dU);

    // convert to scalar float and return value to user
    dV = dUV[0];
}

CGetInteractive::INPERROR CGetInteractive::GetDoubleValue (const std::string& szUserInput, 
                                                           double& dV)
{
    // initialize
    double dSignM = 1.0;	         // assume positive mantissa
    double dSignE = 1.0;          	 // assume positive exponent
    int nPos = 0;		             // current parsing position
    std::string szTemp;              // temporary string
    INPERROR State = INPERROR::NONE; // error state
    // store exponent, mantissa and fractional components
    std::string szExponent= "", szMantissa = "",
                szFraction = "";
    int nVE, nVM; 
    double dVF;

    // trim leading and trailing blanks
    std::string szInput = szUserInput;
    Trim (szInput);

    // first character + or -
    if (szInput[nPos] == '-') {
        dSignM = -1.0;		// negative 
        nPos++;
    }
    else if (szInput[nPos] == '+') {
        nPos++;			// ignore + sign
    }

    // extract string without + or - 
    szTemp = szInput.substr(nPos, szInput.length()-nPos);
    int nLast = static_cast<int>(szTemp.length());
    // trim leading zeros (unless the input is 0)
    if (nLast > 1) TrimLeadingZeros (szTemp);

    // is there an exponent?
    int nLocExp = static_cast<int>(szTemp.find_first_of ("eE"));
    if (nLocExp >= 0) { // yes
        szExponent = szTemp.substr(nLocExp+1, nLast-nLocExp);
        szMantissa = szTemp.substr(0, nLocExp);
        nLast = static_cast<int>(szExponent.length());
        if (nLast == 0) // nothing follows e or E
        {
            return INPERROR::INVALIDINPUT;
        }
        // first character + or - in the exponent
        if (szExponent[0] == '-') {
            dSignE = -1.0;		// negative 
            szExponent = szExponent.substr(1, nLast-1);
        }
        else if (szExponent[0] == '+') {
            szExponent = szExponent.substr(1, nLast-1);
        }
    }
    else // no
        szMantissa = szTemp;

    // is there a decimal?
    int nLocDec = static_cast<int>(szMantissa.find ("."));
    if (nLocDec >= 0) {
        nLast = static_cast<int>(szMantissa.length());
        szFraction = szMantissa.substr(nLocDec+1, nLast-1);
        szMantissa = szMantissa.substr(0, nLocDec);
    }

    // valid string?
    int nLocE = static_cast<int>(szExponent.find_first_not_of ("0123456789"));
    int nLocM = static_cast<int>(szMantissa.find_first_not_of ("0123456789"));
    int nLocF = static_cast<int>(szFraction.find_first_not_of ("0123456789"));
    // invalid input
    if (nLocE >= 0 || nLocM >= 0 || nLocF >= 0)
        State = INPERROR::INVALIDINPUT;		
    // invalid input
    else if (szMantissa.length() == 0 && szFraction.length() == 0)
    {
        dV = 0.0;
        return State;
    }
    // valid input
    else
    {	
        int nCharsE = static_cast<int>(szExponent.length()); 
        int nCharsM = static_cast<int>(szMantissa.length()); 
        // trim leading zeros
        if (nCharsE > 0) {
            TrimLeadingZeros (szExponent);
            nCharsE = static_cast<int>(szExponent.length()); 
        }
        if (nCharsM > 0) {
            TrimLeadingZeros (szMantissa);
            nCharsM = static_cast<int>(szMantissa.length()); 
        }
        // value of exponent
        if ((State = AtoI (szExponent, nVE, static_cast<int>(dSignE)))
                 != INPERROR::NONE)
            return (State);
        // capacity of a long number
        std::string szT;
        int nChars = NumChars(DBL_MAX_10_EXP, szT);
        if (nCharsE > nChars)
            return (INPERROR::TOOMANYCHARACTERS);
        else if (nCharsE == nChars) {
            if (szT < szExponent) return (INPERROR::TOOMANYCHARACTERS);
        }
            
         // value of mantissa
         if ((State = AtoI (szMantissa, nVM, static_cast<int>(dSignM)))
                  != INPERROR::NONE)
             return (State);
         // value of fractional component
         AtoDFraction (szFraction, dVF);

         // account for proper sign and final value
         dV = dSignM*(double(nVM) + dVF)*pow(10.0,dSignE*nVE);
    }

    return State;
}

//=======================================================
//=======================================================
//==================== string version ===================
//=======================================================
//=======================================================
void CGetInteractive::GetInteractive (const std::string& szPrompt, 
                                      std::string& Value,
                                      int nMaxChars)
{
    int nState;
    std::string szTemp;
    char szRawInput[MAXCHARS + 1];	// actual user input stored here
    
    do {
        std::cout << szPrompt;

        // flush any characters in buffer
        std::cin.sync();
        
        // grab all the input until end-of-line
        std::cin.getline (szRawInput, MAXCHARS, '\n');
        nState = std::cin.fail(); // state is non-zero if cin fails
        if (nState)
        {
            std::cin.clear ();
            std::cin >> szTemp;	// since cin failed store the invalid
                               // input in szTemp
        }
        else
        {
            Value = szRawInput;
            if (static_cast<int>(Value.length()) > nMaxChars) 
                nState = 4;
        }

        // check the state
        ErrorHandler (static_cast<INPERROR>(nState), nMaxChars, 0);

    } while (nState);	// iterate until the input is valid.
                        // state is zero for a valid input
}

void CGetInteractive::FlushInput ()
{
    std::string szTemp;

    std::cin.clear (0);
    std::cin >> szTemp;	// since cin failed store the invalid
}

// pause-like function
void CGetInteractive::PAKTC ()
{
    std::cout << "Press any key to continue.";
    char ch;
    std::cin.get (ch);

    // flush the input buffer just in case
    while (!std::cin.eof() && ch != '\n')
        std::cin.get (ch);
}

//=======================================================
//=======================================================
//=================== helper functions ==================
//=======================================================
//=======================================================
// trim leading zeros
void CGetInteractive::TrimLeadingZeros (std::string& szTemp)
{
    int nPos = 0;		// current parsing position

    // find first nonzero character
    while (szTemp[nPos] == '0' && nPos < MAXCHARS)
        nPos++;

    // length of string
    int nLength = static_cast<int>(szTemp.length());

    // zero-filled string?
    if (nPos == (MAXCHARS-1) || (nLength-nPos) == 0)
    {
        szTemp = "";
        return;
    }

    // extract string without leading zeros
    szTemp = szTemp.substr(nPos, nLength-nPos);
}

// trim leading blanks
void CGetInteractive::TrimLeft (std::string& szTemp)
{
    int nPos = 0;		// current parsing position

    // find first nonblank character
    while (szTemp[nPos] == ' ' && nPos < MAXCHARS)
        nPos++;

    // length of string
    int nLength = static_cast<int>(szTemp.length());

    // empty string?
    if (nPos == (MAXCHARS-1) || (nLength-nPos) == 0)
    {
        szTemp = "";
        return;
    }

    // extract string without leading blanks
    szTemp = szTemp.substr(nPos, nLength-nPos);

}

// trim trailing blanks
void CGetInteractive::TrimRight (std::string& szTemp)
{
    int nLast = 0;		// current parsing position

    // find last nonblank character
    int nLength = static_cast<int>(szTemp.length());
    for (int i=nLength-1; i >= 0; i--)
    {
        nLast = i;
        if (szTemp[nLast] != ' ') break;
    }

    // full string?
    if (nLast == (MAXCHARS-1)) {
        return;
    }

    // extract string without trailing blanks
    szTemp = szTemp.substr(0, nLast+1);
}

// trim leading and trailing blanks
void CGetInteractive::Trim (std::string& szTemp)
{
    TrimLeft (szTemp);
    TrimRight (szTemp);
}

void CGetInteractive::UpperCase (std::string& s)
{
    for (auto& c : s) c = static_cast<char>(toupper(c));
}

void CGetInteractive::LowerCase (std::string& s)
{
    for (auto& c : s) c = static_cast<char>(tolower(c));
}

int CGetInteractive::NumChars (long lV, std::string& szT)
{
    int nC = 0;
    long lVT = lV;

    while (lVT > 0)
    {
        szT = szT + ' ';
        lVT /= 10;
        nC++;
    }

    int nX = nC-1;
    while (lV > 0)
    {
        int n = lV - (lV/10)*10;
        lV /= 10;
        szT[nX] = static_cast<char>(n) + '0';
        nX--;
    }

    return nC;
}

// convert alpha string to float value < 1
void CGetInteractive::AtoDFraction (const std::string& szTemp,
                                    float& fVF)
{
    fVF = 0.0f;
    int nC = 1;
    // Example: value of .xyz is
    //          x/10**1 + y/10**2 + z/10**3
    for (int i=0; i < static_cast<int>(szTemp.length()); i++)
    {
        fVF += static_cast<float>((szTemp[i] - '0')*pow(10.0f, -nC));
        nC++;
    }
}

// convert alpha string to float value < 1
void CGetInteractive::AtoDFraction (const std::string& szTemp,
                                    double& dVF)
{
    dVF = 0.0;
    int nC = 1;
    // Example: value of .xyz is
    //          x/10**1 + y/10**2 + z/10**3
    for (int i=0; i < static_cast<int>(szTemp.length()); i++)
    {
        dVF += (szTemp[i] - '0')*pow(10.0f, -nC);
        nC++;
    }
}

// display prompt and read the input
void CGetInteractive::DisplayandRead (const std::string& szPrompt, 
                                      std::string& szInput)
{
    char szRawInput[MAXCHARS + 1];	// actual user input stored here
    int nState;	                    // input state
    std::string szTemp;	            // temporary string

    do {
        // display the prompt
        std::cout << szPrompt;

        // flush any characters in buffer
        std::cin.sync();

        // grab all the input until end-of-line
        std::cin.getline(szRawInput, MAXCHARS, '\n');
        szInput = szRawInput;
        nState = std::cin.fail(); // state is non-zero if cin fails
        if (nState)
        {
            std::cin.clear ();
            std::cin >> szTemp;	// since cin failed store the invalid
                                // input in szTemp
        }
    } while (nState);
}


