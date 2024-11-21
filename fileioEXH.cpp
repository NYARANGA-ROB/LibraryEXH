/*********************************************
Utility Function
Copyright(c) 2000-23, S. D. Rajan
All rights reserved

Functions to handle simple file operations

Object-Oriented Numerical Analysis
*********************************************/
#include "fileioEXH.h"
#include "getinteractiveEXH.h"

std::string CFileIO::OpenInputFileByName (const std::string& strPrompt,
                                          std::ifstream& IFile,
                                          const std::ios::openmode& oMode)
// ---------------------------------------------------------------------------
// Function: opens a file for reading
// Input:    prompt, ifstream object, open mode
// Output:   none
// ---------------------------------------------------------------------------
{
	bool bDone = false;
    std::string strFileName;

	do
    {
        m_GI.GetInteractive (strPrompt, strFileName, 256);

        // cancel?
        if (strFileName == "cancel")
            return "";

		// open the file
		IFile.open (strFileName.c_str(), oMode); 
		if (!IFile)
        {
            std::cout << "Unable to open file." << std::endl;
			IFile.clear ();
		}
		else
			bDone = true; // file opened successfully
	} while (!bDone);

    return strFileName;
}

std::string CFileIO::OpenOutputFileByName (const std::string& strPrompt,
                                           std::ofstream& OFile,
                                           const std::ios::openmode& oMode)
// ---------------------------------------------------------------------------
// Function: opens a file for writing
// Input:    prompt, ofstream object, open mode
// Output:   none
// ---------------------------------------------------------------------------
{
	bool bDone = false;
    std::string strFileName;

	do
    {
        m_GI.GetInteractive (strPrompt, strFileName, 256);

		// open the file
		OFile.open (strFileName.c_str(), oMode); 
		if (!OFile)
        {
			std::cout << "Unable to open file." << std::endl;
			OFile.clear ();
		}
		else
			bDone = true; // file opened successfully
	} while (!bDone);

    return strFileName;
}

void CFileIO::Rewind (std::ifstream& IOFile)
// ---------------------------------------------------------------------------
// Function: rewinds an input file
// Input:    ifstream object
// Output:   file positioned at the beginning 
// ---------------------------------------------------------------------------
{
    // clear all error bits
    IOFile.clear (std::ios_base::goodbit);

    // now position the file at byte 0 (beginning of the file)
    IOFile.seekg (0L, std::ios::beg);
}

bool CFileIO::OpenInputFile (std::ifstream& IFile, const std::string& strFileName)
// ---------------------------------------------------------------------------
// Function: opens a file for reading
// Input:    ifstream object, filename
// Output:   true if file is opened successfully
// ---------------------------------------------------------------------------
{
    IFile.open (strFileName.c_str(), std::ios::in); 
    if (!IFile)
    {
#ifdef _WINDOWS
        AfxMessageBox ("Unable to open file for reading", MB_OK, 0);
#endif
        // clear bad bit, if any
        IFile.clear();
        return false;
    }

    return true;
}

bool CFileIO::OpenOutputFile (std::ofstream& OFile, 
                              const std::string& strFileName)
// ---------------------------------------------------------------------------
// Function: opens a file for writing
// Input:    ofstream object, filename
// Output:   true if file is opened successfully
// ---------------------------------------------------------------------------
{
    OFile.open (strFileName.c_str(), std::ios::out); 
    if (!OFile)
    {
#ifdef _WINDOWS
        AfxMessageBox ("Unable to open file for writing", MB_OK, 0);
#endif
        // clear bad bit, if any
        OFile.clear();
        return false;
    }

    return true;
}

void CFileIO::DeleteFile (const std::string& strFileName)
// ---------------------------------------------------------------------------
// Function: deletes a file
// Input:    filename
// Output:   
// ---------------------------------------------------------------------------
{
    remove (strFileName.c_str());
}