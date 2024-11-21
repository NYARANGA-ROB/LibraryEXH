/*********************************************
Utility Library Function
Copyright(c) 2000-23, S. D. Rajan
All rights reserved

Functions to handle simple file operations

Object-Oriented Numerical Analysis
*********************************************/
#pragma once

#include <iostream>
#include <fstream>	
#include <string>
#include <vector>
#include <stdio.h>
#include "getinteractiveEXH.h"

class CFileIO
{
    public:
        std::string OpenInputFileByName (const std::string& strPrompt, 
                                         std::ifstream& IFile, 
                                         const std::ios::openmode&);
        std::string OpenOutputFileByName (const std::string& strPrompt,
                                          std::ofstream& OFile,
                                          const std::ios::openmode&);
        void Rewind (std::ifstream& IOFile);
        bool OpenInputFile (std::ifstream& IFile,
                            const std::string& strFileName);
        bool OpenOutputFile (std::ofstream& OFile,
                             const std::string& strFileName);
        void DeleteFile (const std::string& strFileName);
    
    private:
        CGetInteractive m_GI;
};

