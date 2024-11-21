/*********************************************
Utility Library Function
Copyright(c) 2006-23, S. D. Rajan
All rights reserved

Object-Oriented Numerical Analysis
*********************************************/
#include <iostream>
#include <cctype>
#include "parserEXH.h"

CParser::CParser ()
// ---------------------------------------------------------------------------
// Function: default constructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    m_strDelimiter = ", ";
    m_strComment = "$";
    m_bLowerCase = false;
}

CParser::CParser (const std::string& strDelimiter,
                  const std::string& strComment,
                  bool bLowerCase)
// ---------------------------------------------------------------------------
// Function: overloaded constructor
// Input:    delimiter, comment, lowercase conversion option
// Output:   none
// ---------------------------------------------------------------------------
{
    if (strDelimiter.empty())
        throw Error::INVALIDDELIMITER;
    if (strComment.empty())
        throw Error::INVALIDCOMMENT;
    SetTraits (strDelimiter, strComment, bLowerCase);
}

CParser::~CParser ()
// ---------------------------------------------------------------------------
// Function: destructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
}

void CParser::SetTraits (const std::string& strDelimiter,
                         const std::string& strComment,
                         bool bLowerCase)
// ---------------------------------------------------------------------------
// Function: sets the traits of the parser
// Input:    strDelimiter - delimiter
//           strComment - comment character(s)
//           bLowerCase - lowercase conversion option
// Output:   none
// ---------------------------------------------------------------------------
{
    m_strDelimiter = strDelimiter;
    m_strComment = strComment;
    m_bLowerCase = bLowerCase;
}

void CParser::ReadNextLine (std::ifstream& FileInput, int& nLineNum, 
                            std::string& strInputString, bool& bEOF) 
// ---------------------------------------------------------------------------
// Function: reads the next line skipping over the comment lines
//           and converts all alphabets to lower case if requested
// Input:    file istream, line #, string to hold the input line
// Output:   updated values of line # and the string
// ---------------------------------------------------------------------------
{
    ReadNextLine (FileInput, nLineNum, strInputString, MAXCHARSPARSER,
                  m_strComment, m_bLowerCase, bEOF);
}

void CParser::ReadNextLine (std::ifstream& FileInput, int& nLineNum, 
                            std::string& strInputString, const int MAXCHARS,
                            const std::string& strComment, 
                            bool& bEOF, bool bLowerCase)
// ---------------------------------------------------------------------------
// Function: reads the next line skipping over the comment lines
//           and converts all alphabets to lower case if requested
// Input:    file istream, line #, string to hold the input line,
//           max. # of characters expected in each input line,
//           comment character(s) at the beginning of a comment line,
//           lowercase conversion option
// Output:   updated values of line # and the string
//           return value is true if successful
//                           false if an error state is encountered
// Restriction: Cannot read a line over 256 characters
// ---------------------------------------------------------------------------
{
    static char szInp[MAXCHARSPARSER+1];

    // enough capacity to read and store?
    if (MAXCHARS > MAXCHARSPARSER)
        throw Error::INPUTLINETOOLONG;

    // comment character(s)
    int nCLen = static_cast<int>(strComment.length());
    bEOF = false;

    // read the line (skip over comment lines)
    for(;;)
    {
        ++nLineNum;
        FileInput.getline (szInp, MAXCHARS);

        // end-of-file?
        if (FileInput.eof())
            bEOF = true;
        if (FileInput.fail())
            FileInput.clear (FileInput.rdstate() & ~std::ios::failbit);
        // unrecoverable error?
        if (FileInput.bad())
            throw Error::UNABLETOREAD;

        // successful read
        strInputString = szInp;
        if (bEOF && strInputString.size() == 0)
            throw Error::ENDOFFILE;
        if (strInputString.substr(0,nCLen) != strComment)
        {
            // convert to lower case?
            if (bLowerCase)
            {
                for (int i=0; i < static_cast<int>(strInputString.length()); i++)
                    strInputString[i] = (char)std::tolower(strInputString[i]);
            }
            break;
        }
    }
}

void CParser::GetTokens (std::ifstream& IFile, int& nLineNumber,
                         std::vector<std::string>& strVTokens,
                         int& nTokens, bool& bEOF)
// ---------------------------------------------------------------------------
// Function: reads a line of input from a file and parses the input line
//           into distinct tokens based on the specified delimiter(s)
// Input:    ifstream object, 
// Output:   vector containing the tokens, number of tokens
// ---------------------------------------------------------------------------
{
    GetTokens (IFile, nLineNumber, strVTokens, nTokens, m_strDelimiter,
               m_strComment,bEOF, m_bLowerCase);
}

void CParser::GetTokens (std::ifstream& IFile, int& nLineNumber,
                         std::vector<std::string>& strVTokens,
                         int& nTokens, const std::string& strDelimiters,
                         const std::string& strComment,
                         bool& bEOF, bool bLowerCase)
// ---------------------------------------------------------------------------
// Function: reads a line of input from a file and parses the input line
//           into distinct tokens based on the specified delimiter(s)
// Input:    ifstream object, delimiters
// Output:   vector containing the tokens, number of tokens
// ---------------------------------------------------------------------------
{
    static char szEntireLine[MAXCHARSPARSER+1];

    // comment character(s)
    int nCLen = static_cast<int>(strComment.length());
    bEOF = false;
    nTokens = 0;

    for (;;)
    {
        ++nLineNumber;
        IFile.getline (szEntireLine, MAXCHARSPARSER);
        if (IFile.fail())
            throw Error::UNABLETOREAD;
        if (IFile.eof())
            bEOF = true;

        // convert input line into std::string object
        std::string strLine (szEntireLine);
        if (bEOF && strLine.size() == 0)
            throw Error::ENDOFFILE;

        if (strLine.substr(0,nCLen) != strComment)
        {
            // prepare to store the tokens
            strVTokens.clear();
            std::string::size_type start_loc, end_loc;
            std::string strCurDelimiter("");

            // get location of the first character that is not a delimiter
            start_loc = strLine.find_first_not_of (strDelimiters);

            // loop while the beginning index is not the end of string
            while (start_loc != std::string::npos)
            {
                if (start_loc > 0)
                    strCurDelimiter = strLine.at(start_loc-1);
                if (strCurDelimiter == "\"")
                {
                    // get location of the next delimiter
                    end_loc = strLine.find_first_of ("\"", start_loc);
                    if (end_loc == std::string::npos) 
                        throw Error::UNABLETOREAD; // missing end quote
                }
                else
                {
                    // get location of the next delimiter
                    end_loc = strLine.find_first_of (strDelimiters, start_loc);

                    // if this location is the end of string then adjust the value
                    // as string length
                    if (end_loc == std::string::npos) end_loc = strLine.length();
                }

                // save the string between start_loc and end_loc
                strVTokens.push_back (strLine.substr(start_loc,end_loc-start_loc));

                // get location of the next character that is not a delimiter
                start_loc = strLine.find_first_not_of (strDelimiters, end_loc);
            }

            // trim leading and trailing blank spaces
            nTokens = static_cast<int>(strVTokens.size ());
            if (nTokens == 0)
                continue; // skip blank line
            for (int i=0; i < nTokens; i++)
            {
                Trim(strVTokens[i]);
                // convert to lower case?
                if (bLowerCase)
                    ToLower(strVTokens[i]);
            }
            break;
        }
    }
}

int CParser::MaxCharactersPerLine () const
// ----------------------------------------------------------------------------
// Function: returns maximum # of characters per line that can be read
// Input:    none
// Output:   maximum # of characters per line
// ----------------------------------------------------------------------------
{
    return MAXCHARSPARSER;
}

void CParser::TrimLeadingZeros (std::string& strTemp) const
// ----------------------------------------------------------------------------
// Function: Removes all leading zeros
// Input:    string
// Output:   string without leading zeros
// ----------------------------------------------------------------------------
{
    int nPos = 0;		// current parsing position

    // find first nonzero character
    while (strTemp[nPos] == '0' && nPos < MAXCHARSPARSER)
        nPos++;

    // length of string
    int nLength = static_cast<int>(strTemp.length());

    // zero-filled string?
    if (nPos == (MAXCHARSPARSER-1) || (nLength-nPos) == 0)
    {
        strTemp = "";
        return;
    }

    // extract string without leading zeros
    strTemp = strTemp.substr(nPos, nLength-nPos);
}

void CParser::TrimLeft (std::string& strTemp) const
// ----------------------------------------------------------------------------
// Function: Removes all leading blank characters
// Input:    string
// Output:   string without leading blanks
// ----------------------------------------------------------------------------
{
    int nPos = 0;       // current parsing position

    // find first nonblank character
    while (strTemp[nPos] == ' ' && nPos < MAXCHARSPARSER)
        nPos++;

    // length of string
    auto nLength = strTemp.length();
    if (nLength == 0)
        return;

    // empty string?
    if (nPos == (MAXCHARSPARSER-1) || (nLength-nPos) == 0)
    {
        strTemp = "";
        return;
    }

    // extract string without leading blanks
    strTemp = strTemp.substr(nPos, nLength-nPos);

}

void CParser::TrimRight (std::string& strTemp) const
// ----------------------------------------------------------------------------
// Function: Removes all trailing blank characters
// Input:    string
// Output:   string without trailing blanks
// ----------------------------------------------------------------------------
{
    size_t nLast=0;      // current parsing position

    // find last nonblank character
    auto nLength = strTemp.length();
    if (nLength == 0)
        return;
    for (auto i=nLength-1; i >= 0; i--)
    {
        nLast = i;
        if (strTemp[nLast] != ' ') break;
    }

    // full string?
    if (nLast == (MAXCHARSPARSER-1))
        return;

    // extract string without trailing blanks
    strTemp = strTemp.substr(0, nLast+1);
}

void CParser::Trim (std::string& strTemp) const
// ----------------------------------------------------------------------------
// Function: Removes all leading and trailing blank characters
// Input:    string
// Output:   string without leading and trailing blanks
// ----------------------------------------------------------------------------
{
    TrimLeft (strTemp);
    TrimRight (strTemp);
}

void CParser::ToUpper (std::string& strInputString) const
// ----------------------------------------------------------------------------
// Function: Converts the string to upper case alphabets
// Input:    string
// Output:   upper case string 
// ----------------------------------------------------------------------------
{
    for (int i=0; i < static_cast<int>(strInputString.length()); i++)
        strInputString[i] = (char)toupper(strInputString[i]);
}

void CParser::ToLower (std::string& strInputString) const
// ----------------------------------------------------------------------------
// Function: Converts the string to upper case alphabets
// Input:    string
// Output:   upper case string 
// ----------------------------------------------------------------------------
{
    for (int i=0; i < static_cast<int>(strInputString.length()); i++)
        strInputString[i] = (char)tolower(strInputString[i]);
}

bool CParser::GetIntValue (const std::string& strUserInput,
                           int& nV) const
// ----------------------------------------------------------------------------
// Function: Converts the string to an integer value
// Input:    string
// Output:   integer value
// ----------------------------------------------------------------------------
{
    long lV;

    if (!GetLongValue (strUserInput, lV))
        return false;
    nV = static_cast<int> (lV);
    return true;
}

bool CParser::GetLongValue (const std::string& strUserInput,
                            long& lV) const
// ----------------------------------------------------------------------------
// Function: Converts the string to a long value
// Input:    string
// Output:   long value
// ----------------------------------------------------------------------------
{
    // initialize
    int nSign = 1;	     // assume positive input
    int nPos = 0;	     // current parsing position
    std::string strTemp; // temporary string

    // trim leading and trailing blanks
    std::string strInput = strUserInput;
    Trim (strInput);

    // first character + or -
    if (strInput[nPos] == '-')
    {
        nSign = -1;		// negative integer
        nPos++;
    }
    else if (strInput[nPos] == '+')
    {
        nPos++;			// ignore + sign
    }

    // extract string without + or -
    int nLast = static_cast<int>(strInput.length());
    strTemp = strInput.substr(nPos, nLast-nPos);
    // trim leading zeros
    if (nLast > 1) TrimLeadingZeros (strTemp);

    // rest of the string is legal?
    int nLoc = static_cast<int>(strTemp.find_first_not_of ("0123456789"));
    if (nLoc >= 0)		// invalid input
        return false;		// set status for invalid input
    // invalid input
    else if (strTemp.length() == 0)
        return false;
    else
    {	
        // legal input so far.
        AtoL (strTemp, lV, nSign);
        // account for proper sign
        lV *= nSign;
    }

    return true;
}

bool CParser::GetFloatValue (const std::string& strUserInput,
                             float& fV) const
// ----------------------------------------------------------------------------
// Function: Converts the string to a float value
// Input:    string
// Output:   float value
// ----------------------------------------------------------------------------
{
    double dV;

    if (!GetDoubleValue (strUserInput, dV))
        return false;
    fV = static_cast<float> (dV);

    return true;
}

bool CParser::GetDoubleValue (const std::string& strUserInput,
                              double& dV) const
// ----------------------------------------------------------------------------
// Function: Converts the string to a double value
// Input:    string
// Output:   double value
// ----------------------------------------------------------------------------
{
    // initialize
    double dSignM = 1.0;	// assume positive mantissa
    double dSignE = 1.0;	// assume positive exponent
    int nPos = 0;		    // current parsing position
    std::string strTemp;    // temporary string

    // store exponent, mantissa and fractional components
    std::string strExponent= "", strMantissa = "",
                strFraction = "";
    long lVE, lVM; 
    double dVF;

    // trim leading and trailing blanks
    std::string strInput = strUserInput;
    Trim (strInput);

    // first character + or -
    if (strInput[nPos] == '-')
    {
        dSignM = -1.0;		// negative 
        nPos++;
    }
    else if (strInput[nPos] == '+')
    {
        nPos++;			// ignore + sign
    }

    // extract string without + or - 
    strTemp = strInput.substr(nPos, strInput.length()-nPos);
    int nLast = static_cast<int>(strTemp.length());
    // trim leading zeros (unless the input is 0)
    if (nLast > 1) TrimLeadingZeros (strTemp);

    // is there an exponent?
    int nLocExp = static_cast<int>(strTemp.find_first_of ("eE"));
    if (nLocExp >= 0)
    { // yes
        strExponent = strTemp.substr(nLocExp+1, nLast-nLocExp);
        strMantissa = strTemp.substr(0, nLocExp);
        nLast = static_cast<int>(strExponent.length());
        if (nLast == 0) // nothing follows e or E
        {
            return false;
        }
        // first character + or - in the exponent
        if (strExponent[0] == '-')
        {
            dSignE = -1.0;		// negative 
            strExponent = strExponent.substr(1, nLast-1);
        }
        else if (strExponent[0] == '+')
        {
            strExponent = strExponent.substr(1, nLast-1);
        }
    }
    else // no
        strMantissa = strTemp;

    // is there a decimal?
    int nLocDec = static_cast<int>(strMantissa.find ("."));
    if (nLocDec >= 0) {
        nLast = static_cast<int>(strMantissa.length());
        strFraction = strMantissa.substr(nLocDec+1, nLast-1);
        strMantissa = strMantissa.substr(0, nLocDec);
    }

    // valid string?
    int nLocE = static_cast<int>(strExponent.find_first_not_of ("0123456789"));
    int nLocM = static_cast<int>(strMantissa.find_first_not_of ("0123456789"));
    int nLocF = static_cast<int>(strFraction.find_first_not_of ("0123456789"));
    // invalid input
    if (nLocE >= 0 || nLocM >= 0 || nLocF >= 0)
        return false;		
    // invalid input
    else if (strMantissa.length() == 0 && strFraction.length() == 0)
    {
        dV = 0.0;
        return true;
    }
    // valid input
    else
    {	
        int nCharsE = static_cast<int>(strExponent.length()); 
        int nCharsM = static_cast<int>(strMantissa.length()); 
        //int nCharsF = static_cast<int>(strFraction.length()); 
        // trim leading zeros
        if (nCharsE > 0) {
            TrimLeadingZeros (strExponent);
            nCharsE = static_cast<int>(strExponent.length()); 
        }
        if (nCharsM > 0) {
            TrimLeadingZeros (strMantissa);
            nCharsM = static_cast<int>(strMantissa.length()); 
        }
        // value of exponent
        if (!AtoL (strExponent, lVE, static_cast<int>(dSignE)))
            return false;
        // capacity of a long number
        std::string strT;
        int nChars = NumChars(DBL_MAX_10_EXP, strT);
        if (nCharsE > nChars)
            return false;
        else if (nCharsE == nChars)
        {
            if (strT < strExponent) return false;
        }
            
         // value of mantissa
         if (!AtoL (strMantissa, lVM, static_cast<int>(dSignM)))
             return false;
         // value of fractional component
         dVF = AtoDFraction (strFraction);

         // account for proper sign and final value
         dV = dSignM*(double(lVM) + dVF)*pow(10.0,dSignE*lVE);
    }

    return true;
}

// convert alpha string to long integer
bool CParser::AtoL (const std::string& strTemp, long& lV,
                    int nSign) const
{
    int nC = 0;
    lV = 0;
    int nLen = static_cast<int>(strTemp.length());

    // capacity of a long number
    //long lVMAX = (nSign >= 1 ? LONG_MAX : labs(LONG_MIN));
    long lVMAX = (nSign >= 1 ? LONG_MAX : LONG_MAX);
    std::string strT;
    int nChars = NumChars(lVMAX, strT);
    if (nLen > nChars)
        return false;
    else if (nLen == nChars)
    {
        if (strT < strTemp) return false;
    }
        
    // Example: value of xyz is
    //          z*10**0 + y*10**1 + z*10**2
    for (int i=nLen-1; i >= 0; i--)
    {
        lV += static_cast<long>((strTemp[i] - '0')*pow(10.0, nC));
        nC++;
    }

    return true;
}

// convert alpha string to double value < 1
double CParser::AtoDFraction (const std::string& strTemp) const
{
    double dVF = 0.0;
    int nC = 1;
    // Example: value of .xyz is
    //          x/10**1 + y/10**2 + z/10**3
    for (int i=0; i < static_cast<int>(strTemp.length()); i++)
    {
        dVF += static_cast<double>(strTemp[i] - '0')*pow(10.0, -nC);
        nC++;
    }

    return dVF;
}

int CParser::NumChars (long lV, std::string& strT) const
{
    int nC = 0;
    long lVT = lV;

    while (lVT > 0)
    {
        strT = strT + ' ';
        lVT /= 10;
        nC++;
    }

    int nX = nC-1;
    while (lV > 0)
    {
        int n = lV - (lV/10)*10;
        lV /= 10;
        strT[nX] = n + '0';
        nX--;
    }

    return nC;
}

void CParser::ErrorMessage (Error err)
// ---------------------------------------------------------------------------
// Function: displays error message
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    std::cerr << "\n";
    if (err == Error::INVALIDDELIMITER)
    {
        std::cerr << "Invalid delimiter has been specified.\n";
    }
    else if (err == Error::INVALIDCOMMENT)
    {
        std::cerr << "Invalid comment has been specified.\n";
    }
    else if (err == Error::INPUTLINETOOLONG)
    {
        std::cerr << "Input line exceeds 256 characters.\n";
    }
    else if (err == Error::UNABLETOREAD)
    {
        std::cerr << "Unable to read.\n";
    }
}
