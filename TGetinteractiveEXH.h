/*********************************************
Utility Library Function
Copyright(c) 2006-23, S. D. Rajan
All rights reserved

Object-Oriented Numerical Analysis

template class to read user input from
the keyboard

*********************************************/
#pragma once
#include <iostream>             // std::cout, std::cin
#include <cctype>               // limits on values
#include <exception>            // exception handling
#include <sstream>              // string formatting
#include "ArrayContainersEXH.h" // CVector

// define macro for catching input exceptions
#define CATCHEXCEPTION                       \
catch (std::exception& err)                  \
{                                            \
    std::cout << err.what() << "\n";         \
}                                            \
catch (...)                                  \
{                                            \
    std::cout << "Unknown input error ...\n";\
}

template <class T>
class CGetInteractive
{
    public:
        static const int MAXCHARS = 256;	// max. # of characters in input
        static const int MAXINP = 100;		// max. # of inputs
        enum class INPERROR {NONE=0,
                             INVALIDINPUT, INPUTOUTOFBOUNDS, 
                             INVALIDBOUNDS,
                             TOOMANYCHARACTERS, NOTENOUGHINPUT,
                             TOOMANYVALUES};
        #define s_cINPERR static_cast<CGetInteractive<T>::INPERROR>

    public:
        CGetInteractive();
        ~CGetInteractive();

        // gets the data
        void GetInteractive (const std::string& szString, T& V);
        void GetInteractive (const std::string& szString, T& V,
                             T L, T U);
        void GetInteractive (const std::string& szString, T V[],
                             int nSize);
        void GetInteractive (const std::string& szString, T V[],
                             int nSize, T L, T U);
        void GetInteractive (const std::string& szString,
                             CVector<T>& V, int nSize);
        void GetInteractive (const std::string& szString, 
                             CVector<T>& V,  int nSize, T L, T U);
        // string version
        void GetInteractive (const std::string& szString, 
                             std::string& V, int n);

        // raw access and conversion functions
        int GetIntValue (const std::string& szInput, long& V);
        int GetRealValue (const std::string& szInput, double& V);

        // helper functions
        void UpperCase (std::string& s);
        void LowerCase (std::string& s);
        void TrimLeadingZeros (std::string& szTemp);
        void TrimLeft (std::string& szTemp);
        void TrimRight (std::string& szTemp);
        void Trim (std::string& szTemp);

    private:
        std::string m_strDelimiters;   // delimiters for tokenizing input
        int         m_nChars;          // max. length of input string

        int NumChars (int nV, std::string& szT);
        int AtoL (const std::string& szTemp, long& nV, int nSign);
        void AtoDFraction (const std::string& szTemp, double& V);
        void DisplayandRead (const std::string& szPrompt,
                             std::string& szInput);

        template <class T>
        auto GetLB()
        // get lower bound value for different data types
        {
            if constexpr(std::is_same_v<int, T>)
                return -INT_MAX;
            else if constexpr(std::is_same_v<float, T>)
                return -FLT_MAX;
            else if constexpr(std::is_same_v<double, T>)
                return -DBL_MAX;
            else if constexpr(std::is_same_v<long, T>)
                return -LONG_MAX;
        }
        template <class T>
        auto GetUB()
        // get upper bound value for different data types
        {
            if constexpr (std::is_same_v<int, T>)
                return INT_MAX;
            else if constexpr (std::is_same_v<float, T>)
                return FLT_MAX;
            else if constexpr (std::is_same_v<double, T>)
                return DBL_MAX;
            else if constexpr (std::is_same_v<long, T>)
                return LONG_MAX;
        }
        template <class T>
        void ErrorHandler (INPERROR State, T LL, T LU)
        // formats and throws error messages 
        {
            if (State == INPERROR::INVALIDINPUT)
                throw std::exception ("Error: Invalid input.");
            else if (State == INPERROR::INPUTOUTOFBOUNDS)
            {
                std::ostringstream strMessage;
                strMessage << "Error: Value should be between " << LL
                           << " and " << LU << ".";
                throw std::runtime_error (strMessage.str());
            }
            else if (State == INPERROR::INVALIDBOUNDS)
            {
                std::ostringstream strMessage;
                strMessage << "Input bound values between "
                           << LL << " and " << LU << " are incorrect.";
                throw std::runtime_error (strMessage.str());
            }
            else if (State == INPERROR::TOOMANYCHARACTERS)
            {
                std::ostringstream strMessage;
                strMessage << "Error: Number of characters exceeds limit of "
                           << m_nChars << " for this data type";
                throw std::runtime_error (strMessage.str());
            }
            else if (State == INPERROR::NOTENOUGHINPUT)
                throw std::exception ("Error: Not enough input.");
            else if (State == INPERROR::TOOMANYVALUES)
                std::cout << "Too many input. Need " << LU
                          << ", but read " << LL << "\n";
        }
};

template <class T>
CGetInteractive<T>::CGetInteractive()
// ---------------------------------------------------------------------------
// Function: default constructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    m_strDelimiters = ", "; // field delimiters are blank space and comma
}

template <class T>
CGetInteractive<T>::~CGetInteractive()
// ---------------------------------------------------------------------------
// Function: destructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
}

template <class T>
void CGetInteractive<T>::DisplayandRead (const std::string& szPrompt,
                                         std::string& szInput)
// ---------------------------------------------------------------------------
// Function: display prompt and read the input
// Input:    szPrompt    prompt to display on the screen
// Output:   szInput     contains the input read
// ---------------------------------------------------------------------------
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

template <class T>
void CGetInteractive<T>::GetInteractive (const std::string& szPrompt,
                                         CVector<T>& VU, 
                                         int nSize,
                                         T L, T U)
// ---------------------------------------------------------------------------
// Function: gets the vector data typed on the keyboard ensuring that
//           the values are within the specified range
// Input:    szPrompt    prompt to display on the screen
//           VU          array of values
//           nSize       # of items to read
//           L           lower bound
//           U           upper bound
// Output:   VU          contains the values read
// ---------------------------------------------------------------------------
{
    T VUCPP[nSize]; 
    GetInteractive (szPrompt, VUCPP, nSize, L, U);
    for (int i=0; i < nSize; i++)
        VU(i+1) = VUCPP[i];
}

template <class T>
void CGetInteractive<T>::GetInteractive (const std::string& szPrompt,
                                         T VU[], 
                                         int nSize,
                                         T L, T U)
// ---------------------------------------------------------------------------
// Function: gets the vector data typed on the keyboard ensuring that
//           the values are within the specified range
// Input:    szPrompt    prompt to display on the screen
//           VU          array of values
//           nSize       # of items to read
//           L           lower bound
//           U           upper bound
// Output:   VU          contains the values read
// ---------------------------------------------------------------------------
{
    // range correct?
    if (L < GetLB<T>() || U > GetUB<T>() || L > U)
        ErrorHandler (INPERROR::INVALIDBOUNDS, L, U);

    std::string szInput;         // input read
    std::string szCurrentString; // current string being processed
    bool bError = false,         // error flag
         bEOI = false;           // end of input flag
    T Value = 0;                 // value read
    CGetInteractive<T>::INPERROR State = CGetInteractive<T>::INPERROR::NONE;

    do 
    {
        try
        {
            DisplayandRead (szPrompt, szInput);
            Trim (szInput);
    
            bError = false;
            // get location of the first character that is not a delimiter
            auto start_loc = szInput.find_first_not_of (m_strDelimiters);
            int i = 0;
            for (;;)
            {
                auto end_loc = szInput.find_first_of(m_strDelimiters, start_loc);
                szCurrentString = szInput.substr(start_loc, end_loc - start_loc);
                if (typeid(T) == typeid(int) || typeid(T) == typeid(long))
                {
                    long V;
                    if ((State = s_cINPERR(GetIntValue (szCurrentString, V))) !=
                           CGetInteractive<T>::INPERROR::NONE)
                    {
                        bError = true;
                        ErrorHandler (State, GetLB<T>(), GetUB<T>());
                        break;
                    }
                    Value = static_cast<T>(V);
                }
                else if (typeid(T) == typeid(float) || typeid(T) == typeid(double))
                {
                    double V;
                    if ((State = s_cINPERR(GetRealValue (szCurrentString, V))) !=
                           CGetInteractive<T>::INPERROR::NONE)
                    {
                        bError = true;
                        ErrorHandler (State, GetLB<T>(), GetUB<T>());
                        break;
                    }
                    Value = static_cast<T>(V);
                }

                start_loc = szInput.find_first_not_of (m_strDelimiters, end_loc);
                // reached end of input?
                if (start_loc == std::string::npos)
                {
                    // not enough input
                    if (i < nSize-1)
                    {
                        bError = true;
                        ErrorHandler (CGetInteractive<T>::INPERROR::NOTENOUGHINPUT,
                                      L, U);
                        break;
                    }
                    else // reach end of input
                        bEOI = true;
                }
                // value within range?
                if (Value < L || Value > U)
                {
                    bError = true;
                    ErrorHandler (CGetInteractive<T>::INPERROR::INPUTOUTOFBOUNDS,
                                  L, U);
                    break;
                }

                // too many values?
                if (i >= nSize)
                {
                    ErrorHandler (CGetInteractive<T>::INPERROR::TOOMANYVALUES,
                                  i+1, nSize);
                }
                else
                    VU[i] = Value;
                // reach end-of-input?
                if (bEOI)
                    break;

                // no, parse next item even if more than needed
                i++;
            }
        }
        CATCHEXCEPTION
    } while (bError);
}

template <class T>
void CGetInteractive<T>::GetInteractive (const std::string& szPrompt,
                                         CVector<T>& VU, 
                                         int nSize)
// ---------------------------------------------------------------------------
// Function: gets the vector data typed on the keyboard 
// Input:    szPrompt    prompt to display on the screen
//           VU          array of values
//           nSize       # of items to read
// Output:   VU          contains the values read
// ---------------------------------------------------------------------------
{
    if (nSize > MAXINP)
        ErrorHandler (INPERROR::TOOMANYVALUES, nSize, MAXINP);
    T VUCPP[MAXINP];
    GetInteractive (szPrompt, VUCPP, nSize, GetLB<T>(), GetUB<T>());
    for (int i = 0; i < nSize; i++)
        VU(i+1) = VUCPP[i];
}

template <class T>
void CGetInteractive<T>::GetInteractive (const std::string& szPrompt,
                                         T VU[], 
                                         int nSize)
// ---------------------------------------------------------------------------
// Function: gets the vector data typed on the keyboard 
// Input:    szPrompt    prompt to display on the screen
//           VU          array of values
//           nSize       # of items to read
// Output:   VU          contains the values read
// ---------------------------------------------------------------------------
{
    GetInteractive (szPrompt, VU, nSize, GetLB<T>(), GetUB<T>());
}

template <class T>
void CGetInteractive<T>::GetInteractive (const std::string& szPrompt, T& V)
// ---------------------------------------------------------------------------
// Function: gets a single data typed on the keyboard
// Input:    szPrompt    prompt to display on the screen
//           V           data
// Output:   V           contains the value read
// ---------------------------------------------------------------------------
{
    GetInteractive (szPrompt, V, GetLB<T>(), GetUB<T>());
}

template <class T>
void CGetInteractive<T>::GetInteractive (const std::string& szPrompt, 
                                         T& V,
                                         T L, T U)
// ---------------------------------------------------------------------------
// Function: gets a single data typed on the keyboard that lies in the 
//           specified range [L, U]
// Input:    szPrompt    prompt to display on the screen
//           V           data
//           L           lower bound
//           U           upper bound
// Output:   V           contains the value read
// ---------------------------------------------------------------------------
{
    // initialize
    T VU[1];
    GetInteractive (szPrompt, VU, 1, L, U);

    // convert to scalar number and return value to user
    V = VU[0];
}

template <class T>
int CGetInteractive<T>::GetIntValue (const std::string& szUserInput,
                                     long& V)
// ---------------------------------------------------------------------------
// Function: converts the string szUserInput to a long value V
// Input:    szUserInput    user-inputted string
// Output:   V              contains the value read
// ---------------------------------------------------------------------------
{
    // initialize
    int nSign = 1;	             // assume positive input
    int nPos = 0;	             // current parsing position
    std::string szTemp;          // temporary string
    CGetInteractive<T>::INPERROR // error state
        State = CGetInteractive<T>::INPERROR::NONE;  

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
    int nLast = static_cast<int> (szInput.length());
    szTemp = szInput.substr (nPos, nLast-nPos);
    // trim leading zeros
    if (nLast > 1) 
        TrimLeadingZeros (szTemp);

    // rest of the string is legal?
    int nLoc = static_cast<int>(szTemp.find_first_not_of ("0123456789"));
    if (nLoc >= 0)		// invalid input
        State = CGetInteractive<T>::INPERROR::INVALIDINPUT;		
    // invalid input
    else if (szTemp.length() == 0)
        State = CGetInteractive<T>::INPERROR::INVALIDINPUT;
    else
    {	
        // legal input so far.
        State = s_cINPERR(AtoL (szTemp, V, nSign));
        // account for proper sign
        if (State == CGetInteractive<T>::INPERROR::NONE)
            V *= nSign;
    }

    return static_cast<int>(State);
}

template <class T>
int CGetInteractive<T>::AtoL (const std::string& szTemp, long& lV,
                              int nSign)
// ---------------------------------------------------------------------------
// Function: converts the string szTemp to a long value lV
// Input:    szTemp   string to be converted
//           nSign    sign of the number (not used)
// Output:   lV       
// ---------------------------------------------------------------------------
{
    int nC = 0;
    lV = 0;
    int nLen = static_cast<int>(szTemp.length());

    // capacity of a long number
    int nVMAX = (nSign >= 1 ? INT_MAX : INT_MAX);
    std::string szT;
    m_nChars = NumChars(nVMAX, szT);
    if (nLen > m_nChars)
        return static_cast<int>(CGetInteractive<T>::INPERROR::TOOMANYCHARACTERS);
    else if (nLen == m_nChars)
    {
        if (szT < szTemp) 
            return static_cast<int>(CGetInteractive<T>::INPERROR::TOOMANYCHARACTERS);
    }
        
    // Example: value of xyz is
    //          z*10**0 + y*10**1 + z*10**2
    for (int i=nLen-1; i >= 0; i--)
    {
        lV += static_cast<int>((szTemp[i] - '0')*pow(10.0, nC));
        nC++;
    }

    return 0;
}

template <class T>
int CGetInteractive<T>::GetRealValue (const std::string& szUserInput,
                                      double& V)
// ---------------------------------------------------------------------------
// Function: converts the string szTemp to a double value V
// Input:    szUserInput   string to be converted
// Output:   V             the double value of the string
// ---------------------------------------------------------------------------
{
    // initialize
    double dSignM = 1.0;          // assume positive mantissa
    double dSignE = 1.0;          // assume positive exponent
    int nPos = 0;		          // current parsing position
    std::string szTemp;           // temporary string
    CGetInteractive<T>::INPERROR  // error state
        State = CGetInteractive<T>::INPERROR::NONE;
    // store exponent, mantissa and fractional components
    std::string szExponent= "", szMantissa = "",
                szFraction = "";
    long nVE, nVM; 
    double VF;

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
            return static_cast<int>(CGetInteractive<T>::INPERROR::INVALIDINPUT);
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
        State = CGetInteractive<T>::INPERROR::INVALIDINPUT;
    // invalid input
    else if (szMantissa.length() == 0 && szFraction.length() == 0)
    {
        V = 0.0;
        return static_cast<int>(State);
    }
    // valid input
    else
    {	
        int nCharsE = static_cast<int>(szExponent.length()); 
        int nCharsM = static_cast<int>(szMantissa.length()); 
        // trim leading zeros
        if (nCharsE > 0)
        {
            TrimLeadingZeros (szExponent);
            nCharsE = static_cast<int>(szExponent.length()); 
        }
        if (nCharsM > 0)
        {
            TrimLeadingZeros (szMantissa);
            nCharsM = static_cast<int>(szMantissa.length()); 
        }
        // value of exponent
        if ((State = s_cINPERR(AtoL (szExponent, nVE, static_cast<int>(dSignE))))
                  != CGetInteractive<T>::INPERROR::NONE)
            return (static_cast<int>(State));
        // capacity of a long number
        std::string szT;
        int nChars = NumChars(DBL_MAX_10_EXP, szT);
        if (nCharsE > nChars)
            return static_cast<int>((CGetInteractive<T>::INPERROR::TOOMANYCHARACTERS));
        else if (nCharsE == nChars)
        {
            if (szT < szExponent) 
                return static_cast<int>((CGetInteractive<T>::INPERROR::TOOMANYCHARACTERS));
        }
            
         // value of mantissa
         if ((State = s_cINPERR(AtoL (szMantissa, nVM, static_cast<int>(dSignM))))
                     != CGetInteractive<T>::INPERROR::NONE)
             return (static_cast<int>(State));
         // value of fractional component
         AtoDFraction (szFraction, VF);

         // account for proper sign and final value
         V = dSignM*(float(nVM) + VF)*pow(10.0f,dSignE*nVE);
    }

    return static_cast<int>(State);
}

template <class T>
void CGetInteractive<T>::TrimLeadingZeros (std::string& szTemp)
// ---------------------------------------------------------------------------
// Function: trims leading zeros from a string representing a number
// Input:    szTemp  string to be trimmed
// Output:   the trimmed string
// ---------------------------------------------------------------------------
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

template <class T>
void CGetInteractive<T>::TrimLeft (std::string& szTemp)
// ---------------------------------------------------------------------------
// Function: trims the leading blanks from a string
// Input:    szTemp  string to be trimmed
// Output:   the trimmed string
// ---------------------------------------------------------------------------
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

template <class T>
void CGetInteractive<T>::TrimRight (std::string& szTemp)
// ---------------------------------------------------------------------------
// Function: trims the trailing blanks from a string
// Input:    szTemp  string to be trimmed
// Output:   the trimmed string
// ---------------------------------------------------------------------------
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

template <class T>
void CGetInteractive<T>::Trim (std::string& szTemp)
// ---------------------------------------------------------------------------
// Function: trim leading and trailing blanks from a string
// Input:    szTemp   string to be trimmed
// Output:   the trimmed string
// ---------------------------------------------------------------------------
{
    TrimLeft (szTemp);
    TrimRight (szTemp);
}

template <class T>
void CGetInteractive<T>::UpperCase (std::string& s)
// ---------------------------------------------------------------------------
// Function: converts a string to upper case
// Input:    s string to be converted
// Output:   the string is converted to upper case
// ---------------------------------------------------------------------------
{
    for (auto& c : s) c = static_cast<char>(toupper(c));
}

template <class T>
void CGetInteractive<T>::LowerCase (std::string& s)
// ---------------------------------------------------------------------------
// Function: converts a string to lower case
// Input:    s string to be converted
// Output:   the string is converted to lower case
// ---------------------------------------------------------------------------
{
    for (auto& c : s) c = static_cast<char>(tolower(c));
}

template <class T>
int CGetInteractive<T>::NumChars (int nV, std::string& szT)
// ---------------------------------------------------------------------------
// Function: counts the number of characters in an integer number
//           and converts the number to a string
// Input:    nV  integer number
// Output:   szT string representation of the number
//           returns the number of characters in the number
// ---------------------------------------------------------------------------
{
    int nC = 0;
    int nVT = nV;

    while (nVT > 0)
    {
        szT = szT + ' ';
        nVT /= 10;
        nC++;
    }

    int nX = nC-1;
    while (nV > 0)
    {
        int n = nV - (nV/10)*10;
        nV /= 10;
        szT[nX] = static_cast<char>(n) + '0';
        nX--;
    }

    return nC;
}

template <class T>
void CGetInteractive<T>::AtoDFraction (const std::string& szTemp,
                                       double& V)
// ---------------------------------------------------------------------------
// Function: converts string to double value < 1
// Input:    szTemp  string
// Output:   V       double value for the input string
// ---------------------------------------------------------------------------
{
    V = 0.0;
    int nC = 1;
    // Example: value of .xyz is
    //          x/10**1 + y/10**2 + z/10**3
    for (int i=0; i < static_cast<int>(szTemp.length()); i++)
    {
        V += ((szTemp[i] - '0')*pow(10.0, -nC));
        nC++;
    }
}

//=======================================================
//=======================================================
//==================== string version ===================
//=======================================================
//=======================================================
template <class T>
void CGetInteractive<T>::GetInteractive (const std::string& szPrompt, 
                                         std::string& szInput,
                                         int nMaxChars)
{
    bool bError = false;
    do
    {
        bError = false;
        DisplayandRead (szPrompt, szInput);
        if (static_cast<int>(szInput.length()) > nMaxChars)
        {
            bError = true;
            ErrorHandler (INPERROR::TOOMANYCHARACTERS,
                          nMaxChars, 0);
        }

    } while (bError);	// iterate until the input is valid.
}
