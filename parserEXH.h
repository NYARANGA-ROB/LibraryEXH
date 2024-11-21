/*********************************************
Utility Library Function
Copyright(c) 2006-23, S. D. Rajan
All rights reserved

Object-Oriented Numerical Analysis
*********************************************/
#pragma once
#include <fstream>
#include <string>
#include <vector>

class CParser
{
    public:
        enum class Error {INVALIDDELIMITER, INVALIDCOMMENT,
                          INPUTLINETOOLONG, UNABLETOREAD,
                          ENDOFFILE};
        static const int MAXCHARSPARSER = 256;
        CParser ();
        CParser (const std::string& strDelimiter,
                 const std::string& strComment, 
                 bool bLowerCase);
        ~CParser ();

        void SetTraits (const std::string& strDelimiter,
                        const std::string& strComment, 
                        bool bLowerCase);
        void GetTokens (std::ifstream& IFile, int& nLineNumber,
                        std::vector<std::string>& strVTokens,
                        int& nTokens, bool& bEOF);
        void GetTokens (std::ifstream& IFile, int& nLineNumber,
                        std::vector<std::string>& strVTokens,
                        int& nTokens, const std::string& strDelimiters,
                        const std::string& strComment,
                        bool& bEOF, bool bLowerCase = true);
        void ReadNextLine (std::ifstream& FileInput, int& nL,
                           std::string& strInputString,
                           bool& bEOF);
        void ReadNextLine (std::ifstream& FileInput, int& nL,
                           std::string& strInputString,
                           const int MAXCHARS, 
                           const std::string& strComment,
                           bool& bEOF,
                           bool bLowerCase = true);
        int MaxCharactersPerLine () const;

        bool GetIntValue (const std::string& strInput, int& nV) const;
        bool GetLongValue (const std::string& strInput, long& lV) const;
        bool GetFloatValue (const std::string& strInput, float& fV) const;
        bool GetDoubleValue (const std::string& strInput, double& dV) const;

        void Trim (std::string& strTemp) const;
        void TrimLeft (std::string& strTemp) const;
        void TrimRight (std::string& strTemp) const;
        void TrimLeadingZeros (std::string& strTemp) const;
        void ToLower (std::string& strInputString) const;
        void ToUpper (std::string& strInputString) const;
        static void ErrorMessage (CParser::Error err);

    private:
        std::string m_strDelimiter;
        std::string m_strComment;
        bool        m_bLowerCase;
        bool AtoL (const std::string& strTemp, long& lV,
                   int nSign) const;
        double AtoDFraction (const std::string& strTemp) const;
        int NumChars (long lV, std::string& strT) const;
};
