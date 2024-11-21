/*********************************************
Utility Library Function
Copyright(c) 2006-23, S. D. Rajan
All rights reserved

Object-Oriented Numerical Analysis
*********************************************/
#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <exception>
#include <cmath>
#include <limits.h>
#include <float.h>

class CGetInteractive
{
    public:
        static const int MAXCHARS = 256;	// max. # of characters in input
        enum class INPERROR {NONE=0, INVALIDINPUT, INPUTOUTOFBOUNDS, INVALIDBOUNDS,
                             TOOMANYCHARACTERS, NOTENOUGHINPUT};

    public:
        CGetInteractive();
        void TrimLeadingZeros (std::string& szTemp);
        void TrimLeft (std::string& szTemp);
        void TrimRight (std::string& szTemp);
        void Trim (std::string& szTemp);

        // int version
        void GetInteractive (const std::string& szString, int& V);
        void GetInteractive (const std::string& szString, int& V,
                             int nL, int nU);
        void GetInteractive (const std::string& szString, int V[],
                             int nSize);
        // long version
        void GetInteractive (const std::string& szString, long& V);
        void GetInteractive (const std::string& szString, long& V,
                             long lL, long lU);
        void GetInteractive (const std::string& szString, long V[],
                             int nSize);
        // float version
        void GetInteractive (const std::string& szString, float& V);
        void GetInteractive (const std::string& szString, float& V,
                             float fL, float fU);
        void GetInteractive (const std::string& szString, float V[],
                             int nSize);
        // double version
        void GetInteractive (const std::string& szString, double& V);
        void GetInteractive (const std::string& szString, double& V,
                             double dL, double dU);
        void GetInteractive (const std::string& szString, double V[],
                             int nSize);
        // string version
        void GetInteractive (const std::string& szString, std::string& V,
                             int n);

        // raw access and conversion functions
        INPERROR GetIntValue (const std::string& szInput, int& nV);
        INPERROR GetLongValue (const std::string& szInput, long& lV);
        INPERROR GetFloatValue (const std::string& szInput, float& fV);
        INPERROR GetDoubleValue (const std::string& szInput, double& dV);
        void UpperCase (std::string& s);
        void LowerCase (std::string& s);
        void FlushInput();

        // pause-like function
        void PAKTC();

    private:
        int NumChars (long lV, std::string& szT);
        INPERROR AtoI (const std::string& szTemp, int& nV, int nSign);
        INPERROR AtoL (const std::string& szTemp, long& nV, int nSign);
        void AtoDFraction (const std::string& szTemp, float& fV);
        void AtoDFraction (const std::string& szTemp, double& dV);
        void DisplayandRead (const std::string& szPrompt,
                             std::string& szInput);
        // display error message
        template <class T>
        void ErrorHandler (INPERROR nState, T LL=0, T LU=0)
        {
            // check the state
            if (nState == INPERROR::INVALIDINPUT)
                throw std::exception ("Error: Invalid input.");
            else if (nState == INPERROR::INPUTOUTOFBOUNDS)
            {
                std::ostringstream strMessage;
                strMessage << "Error: Value should be between " << LL
                           << " and " << LU;
                throw std::runtime_error (strMessage.str());
            }
            else if (nState == INPERROR::INVALIDBOUNDS)
                throw std::exception ("Error: Check specified range values.");
            else if (nState == INPERROR::TOOMANYCHARACTERS)
            {
                std::ostringstream strMessage;
                strMessage << "Error: Number of characters restricted to "
                           << LL;
                throw std::runtime_error (strMessage.str());
            }
            else if (nState == INPERROR::NOTENOUGHINPUT)
                throw std::exception ("Error: Not enough input.");
        }
    
        std::string m_strDelimiters;
};
