// **************************************************
// Matrix Template Class
// Copyright(c), 2000-23, S. D. Rajan
// All rights reserved
//
// Implements a two-dimensional array
// Row and Column indexing start at 1
// ---------------------------------------------------------------------------------------------------
// Visual Studio Debugger:
// In the Watch 1 window, type nameofmatrix.m_pCells[1], n to view the entire first row of the matrix.
// n is the number of columns in the matrix + 1. Note that the first location may have garbage
// value since the first location is unused.
// Similarly,  type nameofmatrix.m_pCells[2], n to view the entire second row of the matrix.
// **************************************************
#pragma once

#include <string>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdarg.h>
#include "arraybasecontainerEXH.h"
#include "vectorcontainerEXH.h"

// defines the Matrix class
template <class T>
class CMatrix : public CArrayBase
{
   // -------------------------
   // template friend functions
   // -------------------------
   template <typename TT>
   friend CMatrix<TT> operator* (TT c,
                                 const CMatrix<TT>&);  // overloaded * operator
   template <typename TT>
   friend CMatrix<TT> operator* (const CMatrix<TT>&,
                                 const CMatrix<TT>&);  // overloaded * operator
   template <typename TT>
   friend CVector<TT> operator* (const CMatrix<TT>&,
                                 const CVector<TT>&);  // overloaded * operator
   template <typename TT>
   friend CVector<TT> operator* (const CVector<TT>&,
                                 const CMatrix<TT>&);  // overloaded * operator

public:
   CMatrix ();                                  // default constructor
   explicit CMatrix (int, int);                 // constructor
   explicit CMatrix (char, char) = delete;      // constructor disallowing char arguments
   explicit CMatrix (char, int) = delete;       // constructor disallowing char arguments
   explicit CMatrix (int, char) = delete;       // constructor disallowing char arguments
   CMatrix (const char *);                      // constructor
   explicit CMatrix (const char *, int, int);   // constructor
   CMatrix (const CMatrix<T>&);                 // copy constructor
#ifndef __NOMOVECTOR__
   CMatrix (CMatrix<T>&&);                      // move constructor
#endif
   CMatrix (const std::initializer_list<T>&);   // ctor with initializers
   ~CMatrix ();                                 // destructor
   void SetSize (int, int);                     // sets the size of the matrix
   // used with the default constructor
   void SetSize (int, int, T);                  // sets the size of the matrix and initial value
   // and initial value

   // ------------------
   // helper functions
   // ------------------
   int GetRows () const;              // gets the current number of rows
   int GetColumns () const;           // gets the current number of columns
   void GetName (std::string&) const; // gets the matrix name
   void Display (std::ostream& OF,
                 const std::string& strMessage) const;
   void DisplayColumnWise (std::ostream& OF,
                           const std::string& strMessage) const;
   void DisplayRow (int nRow, std::ostream& OF,
                    const std::string& strMessage) const;
   void DisplayColumn (int nColumn, std::ostream& OF,
                       const std::string& strMessage) const;
   void Display (std::ostream& OF,
                 const std::string& strMessage,
                 int nSR, int nSC, int nER, int nEC) const;

   // ------------------------------
   // matrix manipulations (mutator)
   // ------------------------------
   void Set (T);                      // sets the value of all elements
                                      // of a matrix
   void SetName (const std::string&); // sets the matrix name

   // --------------------
   // overloaded operators
   // --------------------
   T& operator() (int, int);                         // row-col access
   const T& operator() (int, int) const;             // row-col access
   CMatrix<T>  operator- ();                         // overloaded unary operator
   bool        operator== (const CMatrix<T>&);       // overloaded == operator
   bool        operator!= (const CMatrix<T>&);       // overloaded != operator
   T& operator= (const CMatrix&);                    // overloaded = operator
#ifndef __NOMOVECTOR__
   CMatrix<T>& operator= (CMatrix<T>&&);             // overloaded move = operator
#endif
   CMatrix<T>  operator+ (const CMatrix<T>&) const;  // overloaded + operator
   CMatrix<T>  operator+ (T c) const;                // overloaded + operator
   void        operator+= (const CMatrix<T>&);       // overloaded += operator
   CMatrix<T>  operator- (const CMatrix<T>&) const;  // overloaded - operator
   CMatrix<T>  operator- (T c) const;                // overloaded - operator
   void        operator-= (const CMatrix<T>&);       // overloaded -= operator
   CMatrix<T>  operator* (T c);                      // overloaded * operator
   void        operator*= (T c);                     // overloaded *= operator
   CVector<T>  operator/ (const CVector<T>& b);      // overloaded / operator

   // --------------------
   // other operations
   // --------------------
   CMatrix<T>  Tr ();       // computes the transpose 
   T           MaxNorm ();  // computes the max norm

private:
   const int NUMPERROW = 6; // # of matrix elements to display per row/column
   std::string m_strName;   // matrix identifier
   T   **m_pCells;          // address where the matrix of
                            // type T is stored
   int m_nRows;             // number of rows in the matrix
   int m_nColumns;          // number of columns in the matrix
   void Release ();         // similar to destructor
   void Initialize ();      // initialized values
   inline T GetTolerance () const; // tolerance for bool checks

   void GaussElimination (CMatrix<T>& A,
                          CVector<T>& x,
                          CVector<T>& b,
                          T TOL);
   void ErrorHandler (ERRORVM) const;
};

// =============== definitions ===========================================
template <class T>
CMatrix<T>::CMatrix ()
// ---------------------------------------------------------------------------
// Function: default ctor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
   Initialize ();
}

template <class T>
void CMatrix<T>::Initialize ()
// ---------------------------------------------------------------------------
// Function: initializer
// Input:
// Output:
// ---------------------------------------------------------------------------
{
   m_pCells = nullptr;
   m_nRows = 0;
   m_nColumns = 0;
}

template <class T>
CMatrix<T>::CMatrix (int nR, int nC)
// ---------------------------------------------------------------------------
// Function: overloaded ctor
// Input:    # of rows and columns
// Output:   none
// ---------------------------------------------------------------------------
{
   Initialize();
   SetSize (nR, nC);
}

template <class T>
CMatrix<T>::CMatrix (const char* strName)
// ---------------------------------------------------------------------------
// Function: overloaded ctor
// Input:    matrix name
// Output:   none
// ---------------------------------------------------------------------------
{
   Initialize();
   m_strName = strName;
}

template <class T>
CMatrix<T>::CMatrix (const char* strName, int nR, int nC)
// ---------------------------------------------------------------------------
// Function: overloaded ctor
// Input:    matrix name, # of rows and columns
// Output:   none
// ---------------------------------------------------------------------------
{
   Initialize();
   m_strName = strName;
   SetSize (nR, nC);
}

template <class T>
CMatrix<T>::CMatrix (const CMatrix<T>& A)
// ---------------------------------------------------------------------------
// Function: copy ctor
// Input:    matrix
// Output:   none
// ---------------------------------------------------------------------------
{
   if (A.m_nRows <= 0 || A.m_nColumns <= 0)
      ErrorHandler (MATRIX_INVALID_SIZE);
   Initialize();
   m_strName = A.m_strName;
   SetSize (A.m_nRows, A.m_nColumns);
   for (int i=1; i <= m_nRows; i++)
   {
      for (int j=1; j <= m_nColumns; j++)
      {
         m_pCells[i][j] = A.m_pCells[i][j];
      }
   }
}

#ifndef __NOMOVECTOR__
template <class T>
CMatrix<T>::CMatrix (CMatrix<T>&& A)
// ---------------------------------------------------------------------------
// Function: move ctor
// Input:    matrix
// Output:   none
// ---------------------------------------------------------------------------
{
   // copy the data pointer and its size from the source object
   m_pCells = A.m_pCells;
   m_nRows = A.m_nRows;
   m_nColumns = A.m_nColumns;
   m_strName = A.m_strName;

   // release the data pointer from the source object so that
   // the destructor does not free the memory
   A.m_pCells = nullptr;
   A.m_nRows = 0;
   A.m_nColumns = 0;
}
#endif

template <class T>
CMatrix<T>::CMatrix (const std::initializer_list<T>& List)
// ---------------------------------------------------------------------------
// Function: Constructor with initial values
// Input:    Size of matrix, list of values
// Output:   none
// ---------------------------------------------------------------------------
{
   Initialize();
   m_strName = "?";

   int nCount = static_cast<int>(List.size());
   int nIndex = 0;
   auto i = List.begin();
   int nRows = static_cast<int>(*(i++));
   int nCols = static_cast<int>(*(i++));

   SetSize(nRows, nCols);

   for (int nRN = 1; nRN <= nRows; nRN++)
   {
      for (int nCN = 1; nCN <= nCols; nCN++)
      {
         if (i == List.end())
         {
            ErrorHandler (MATRIX_NOT_ENOUGH_INITIALIZERS);
            return;
         }
         else
            (*this)(nRN,nCN) = *(i++);
      }
   }
}

template <class T>
void CMatrix<T>::SetSize (int nR, int nC)
// ---------------------------------------------------------------------------
// Function: dynamically allocates memory
// Input:    matrix size (# of rows and columns)
// Output:   none
// ---------------------------------------------------------------------------
{
   // do nothing if the current size is the same as the requested size
   if (nR == m_nRows && nC == m_nColumns)
      return;

   m_ValueR = nR; m_ValueC = nC;
   // check whether nR and nC are legal
   if (nR <= 0 || nC <= 0)
   {
      ErrorHandler (MATRIX_INVALID_SIZE);
      m_nRows = m_nColumns = 0;
      return;
   }
   Release ();
   size_t size = nR*nC + 1;
   try
   {
      m_pCells = new T*[nR + 1];
   }
   catch (std::bad_alloc)
   {
      ErrorHandler (MATRIX_ALLOCATION_ERROR);
   }
   try
   {
      m_pCells[0] = new T[size];
   }
   catch (std::bad_alloc)
   {
      ErrorHandler (MATRIX_ALLOCATION_ERROR);
   }
   m_pCells[1] = m_pCells[0];
   for (int i=2; i <= nR; i++)
      m_pCells[i] = m_pCells[i-1]+nC;
   m_nRows = nR;
   m_nColumns = nC;
   m_dAllocated += static_cast<double>(sizeof(T*)*(nR+1));
   m_dAllocated += static_cast<double>(sizeof(T)*size);
}

template <class T>
void CMatrix<T>::SetSize (int nR, int nC, T V)
// ---------------------------------------------------------------------------
// Function: dynamically allocates memory
// Input:    matrix size (# of rows and columns)
// Output:   none
// ---------------------------------------------------------------------------
{
   SetSize (nR, nC);
   Set (V);
}

template <class T>
CMatrix<T>::~CMatrix ()
// ---------------------------------------------------------------------------
// Function: dtor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    Release ();
   /*
   // deallocate storage
   if (m_pCells != nullptr)
   {
      delete [] m_pCells[0];
      m_pCells[0] = nullptr;
      m_dDeAllocated += static_cast<double>(sizeof(T*)*(m_nRows+1));
   }
   if (m_pCells != nullptr)
   {
      delete [] m_pCells;
      m_pCells = nullptr;
      size_t nSize = m_nRows*m_nColumns+1;
      m_dDeAllocated += static_cast<double>(sizeof(T)*nSize);
   }
   m_nRows = 0;
   m_nColumns = 0;
   */
}

template <class T>
void CMatrix<T>::Release ()
// ---------------------------------------------------------------------------
// Function: dynamically deallocates memory
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
   // deallocate storage
   if (m_pCells != nullptr)
   {
      delete [] m_pCells[0];
      m_dDeAllocated += static_cast<double>(sizeof(T*)*(m_nRows+1));
   }
   if (m_pCells != nullptr)
   {
      delete [] m_pCells;
      size_t nSize = m_nRows*m_nColumns+1;
      m_dDeAllocated += static_cast<double>(sizeof(T)*nSize);
   }
   m_pCells = nullptr;
   m_nRows = 0;
   m_nColumns = 0;
}

// ----------------------------
// specialized member functions
// ----------------------------
template <>
inline int CMatrix<int>::GetTolerance() const
{
   return 0;
}

template <>
inline float CMatrix<float>::GetTolerance() const
{
   return 0.0001f;
}

template <>
inline double CMatrix<double>::GetTolerance() const
{
   return 1.0e-6;
}

// =============== member functions ===========================================
template <class T>
void CMatrix<T>::SetName (const std::string& strName)
// ---------------------------------------------------------------------------
// Function: sets the name of the matrix
// Input:    matrix name
// Output:   none
// ---------------------------------------------------------------------------
{
   m_strName = strName;
}

template <class T>
void CMatrix<T>::GetName (std::string& strName) const
// ---------------------------------------------------------------------------
// Function: gets the name of the matrix
// Input:    string to hold matrix name
// Output:   matrix name
// ---------------------------------------------------------------------------
{
   strName = m_strName;
}

template <class T>
int CMatrix<T>::GetRows () const
// ---------------------------------------------------------------------------
// Function: gets the # of rows in the matrix
// Input:    none
// Output:   # of rows
// ---------------------------------------------------------------------------
{
   return (m_nRows);
}

template <class T>
int CMatrix<T>::GetColumns () const
// ---------------------------------------------------------------------------
// Function: gets the # of columns in the matrix
// Input:    none
// Output:   # of columns
// ---------------------------------------------------------------------------
{
   return (m_nColumns);
}

template <class T>
void CMatrix<T>::Set (T dV)
// ---------------------------------------------------------------------------
// Function: sets the value of all the elements in the matrix to the
//           specified value
// Input:    specified value
// Output:   none
// ---------------------------------------------------------------------------
{
   for (int i=1; i <= m_nRows; i++)
   {
      for (int j=1; j <= m_nColumns; j++)
      {
         m_pCells[i][j] = dV; // or, (*this)(i,j) = dV;
      }
   }
}

template <class T>
void CMatrix<T>::Display (std::ostream& OF, const std::string& strMessage) const
// ---------------------------------------------------------------------------
// Function: displays the contents of the matrix rowwise
// Input:    string to display
// Output:   none
// ---------------------------------------------------------------------------
{
   OF << strMessage;
   for (int i = 1; i <= m_nRows; i++)
   {
      OF << "Row: " << i << "\n";
      int nCurItm = 0;
      for (int j = 1; j <= m_nColumns; j += NUMPERROW)
      {
         ++nCurItm;
         int nS = (nCurItm - 1)*NUMPERROW + 1;
         int nE = std::min(nS + NUMPERROW - 1, m_nColumns);
         for (int k = nS; k <= nE; k++)
         {
            OF << "(" << std::setw(3) << k << ") " << std::setw(15) << m_pCells[i][k] << "  ";
         }
         OF << std::endl;
      }
   }
}

template <class T>
void CMatrix<T>::DisplayColumnWise (std::ostream& OF, 
                                    const std::string& strMessage) const
// ---------------------------------------------------------------------------
// Function: displays the contents of the matrix columnwise
// Input:    string to display
// Output:   none
// ---------------------------------------------------------------------------
{
   OF << strMessage;
   for (int i = 1; i <= m_nColumns; i++)
   {
      OF << "Column: " << i << "\n";
      int nCurItm = 0;
      for (int j = 1; j <= m_nRows; j += NUMPERROW)
      {
         ++nCurItm;
         int nS = (nCurItm - 1)*NUMPERROW + 1;
         int nE = std::min(nS + NUMPERROW - 1, m_nRows);
         for (int k = nS; k <= nE; k++)
         {
            OF << "(" << std::setw(3) << k << ") " << std::setw(15) << m_pCells[k][i] << "  ";
         }
         OF << std::endl;
      }
   }
}

template <class T>
void CMatrix<T>::DisplayRow (int nRow, std::ostream& OF,
                             const std::string& strMessage) const
{
   if (nRow <= 0 || nRow > m_nRows)
       throw MATRIX_INDEX_OUT_OF_BOUNDS;
   OF << strMessage;
   for (int i = nRow; i <= nRow; i++)
   {
      OF << "Row: " << i << "\n";
      int nCurItm = 0;
      for (int j = 1; j <= m_nColumns; j += NUMPERROW)
      {
         ++nCurItm;
         int nS = (nCurItm - 1)*NUMPERROW + 1;
         int nE = std::min(nS + NUMPERROW - 1, m_nColumns);
         for (int k = nS; k <= nE; k++)
         {
            OF << "(" << std::setw(3) << k << ") " << std::setw(15) << m_pCells[i][k] << "  ";
         }
         OF << std::endl;
      }
   }
}

template <class T>
void CMatrix<T>::DisplayColumn (int nColumn, std::ostream& OF,
                                const std::string& strMessage) const
{
   if (nColumn <= 0 || nColumn > m_nColumns)
       throw MATRIX_INDEX_OUT_OF_BOUNDS;
   OF << strMessage;
   for (int i = nColumn; i <= nColumn; i++)
   {
      OF << "Col: " << i << "\n";
      int nCurItm = 0;
      for (int j = 1; j <= m_nRows; j += NUMPERROW)
      {
         ++nCurItm;
         int nS = (nCurItm - 1)*NUMPERROW + 1;
         int nE = std::min(nS + NUMPERROW - 1, m_nRows);
         for (int k = nS; k <= nE; k++)
         {
            OF << "(" << std::setw(3) << k << ") " << std::setw(15) << m_pCells[k][i] << "  ";
         }
         OF << std::endl;
      }
   }
}

template <class T>
void CMatrix<T>::Display (std::ostream& OF, const std::string& strMessage,
                          int nStartRow, int nStartColumn,
                          int nEndRow,   int nEndColumn) const
// ---------------------------------------------------------------------------
// Function: displays the contents of the matrix
// Input:    string to display
// Output:   none
// ---------------------------------------------------------------------------
{
    const int NUMPERROW = 6;
    OF << strMessage;
    for (int i = nStartRow; i <= nEndRow; i++)
    {
        OF << "Row: " << i << "\n";
        int nCurItm = 0;
        for (int j = nStartColumn; j <= nEndColumn; j += NUMPERROW)
        {
            ++nCurItm;
            int nS = (nCurItm - 1)*NUMPERROW + 1;
            int nE = std::min(nS + NUMPERROW - 1, nEndColumn);
            for (int k = nS; k <= nE; k++)
            {
                OF << "(" << std::setw(3) << k << ") " << std::setw(15) << m_pCells[i][k] << "  ";
            }
            OF << std::endl;
        }
    }
}


// ==================== Overloaded Operators ========================
template <class T>
T& CMatrix<T>::operator() (int nR, int nC)
// ---------------------------------------------------------------------------
// Function: overloaded () operator to access matrix contents
//           carries out bound checking
// Input:    row and column indices
// Output:   value at the specified indices
// ---------------------------------------------------------------------------
{
#ifdef _DEBUG
   if (nR <= 0 || nR > m_nRows || nC <= 0 || nC > m_nColumns)
   {
      m_ValueR = nR; m_ValueC = nC;
      ErrorHandler (MATRIX_INDEX_OUT_OF_BOUNDS);
      return m_pCells[0][0];
   }
   else
      return m_pCells[nR][nC];
#else
   return m_pCells[nR][nC];
#endif
}

template <class T>
const T& CMatrix<T>::operator() (int nR, int nC) const
// ---------------------------------------------------------------------------
// Function: overloaded () operator to access matrix contents
//           carries out bound checking
// Input:    row and column indices
// Output:   value at the specified indices
// ---------------------------------------------------------------------------
{
#ifdef _DEBUG
   if (nR <= 0 || nR > m_nRows || nC <= 0 || nC > m_nColumns)
   {
       m_ValueR = nR; m_ValueC = nC;
       ErrorHandler (MATRIX_INDEX_OUT_OF_BOUNDS);
       return m_pCells[0][0];
   }
   else
      return m_pCells[nR][nC];
#else
   return m_pCells[nR][nC];
#endif
}

template <class T>
T& CMatrix<T>::operator= (const CMatrix& matarg)
// ---------------------------------------------------------------------------
// Function: overloaded = operator
// Input:    matrix to use as rvalue
// Output:   modified values
// ---------------------------------------------------------------------------
{
   // check whether matrix is assigned to itself
   if (this != &matarg)
   {
      // compatible matrices?
      if (m_nRows != matarg.m_nRows || m_nColumns != matarg.m_nColumns)
      {
         m_ValueR = matarg.m_nRows; m_ValueC = matarg.m_nColumns;
         ErrorHandler (MATRIX_INCOMPATIBLE_MATRICES);
         return (T&)(*this);
      }
      // now copy
      for (int i=1; i <= matarg.m_nRows; i++)
      {
         for (int j=1; j <= matarg.m_nColumns; j++)
         {
            m_pCells[i][j]= matarg.m_pCells[i][j];
         }
      }
   }

   return (T&)(*this);
}

#ifndef __NOMOVECTOR__ 
template <class T>
CMatrix<T>& CMatrix<T>::operator= (CMatrix&& A)
// ---------------------------------------------------------------------------
// Function: overloaded move = operator
// Input:    matrix to use as rvalue
// Output:   modified values
// ---------------------------------------------------------------------------
{
   // check whether matrix is assigned to itself
   if (this != &A)
   {
      // compatible matrices?
      if (m_nRows != A.m_nRows || m_nColumns != A.m_nColumns)
      {
         m_ValueR = A.m_nRows; m_ValueC = A.m_nColumns;
         ErrorHandler (MATRIX_INCOMPATIBLE_MATRICES);
         return (*this);
      }
      // free the existing resource
      Release ();

      // copy the data pointer and its length from the source object
      m_pCells = A.m_pCells;
      m_nRows = A.GetRows();
      m_nColumns = A.GetColumns();
      m_strName = A.m_strName;

      // release the data pointer from the source object so that
      // the destructor does not free the memory multiple times
      A.m_pCells = nullptr;
      A.m_nRows = A.m_nColumns = 0;
   }

   return *this;
}
#endif

template <class T>
bool CMatrix<T>::operator== (const CMatrix<T>& MRight)
// ---------------------------------------------------------------------------
// Function: overloaded == operator
// Input:    scale value
// Output:   none
// ---------------------------------------------------------------------------
{
   // compatible matrices?
   if (m_nRows != MRight.m_nRows || m_nColumns != MRight.m_nColumns)
   {
      m_ValueR = MRight.m_nRows; m_ValueC = MRight.m_nColumns;
      ErrorHandler (MATRIX_INCOMPATIBLE_MATRICES);
      return false;
   }

   for (int i=1; i <= m_nRows; i++)
   {
      for  (int j=1; j <= m_nColumns; j++)
      {
         if (abs(m_pCells[i][j]-MRight.m_pCells[i][j]) > GetTolerance())
            return false;
      }
   }
   return true;
}

template <class T>
bool CMatrix<T>::operator!= (const CMatrix<T>& MRight)
// ---------------------------------------------------------------------------
// Function: overloaded != operator
// Input:    scale value
// Output:   none
// ---------------------------------------------------------------------------
{
   // compatible matrices?
   if (m_nRows != MRight.m_nRows || m_nColumns != MRight.m_nColumns)
   {
      m_ValueR = MRight.m_nRows; m_ValueC = MRight.m_nColumns;
      ErrorHandler (MATRIX_INCOMPATIBLE_MATRICES);
      return false;
   }

   for (int i=1; i <= m_nRows; i++)
   {
      for  (int j=1; j <= m_nColumns; j++)
      {
         if (abs(m_pCells[i][j]-MRight.m_pCells[i][j]) > GetTolerance())
            return true;
      }
   }
   return false;
}

template <class T>
CMatrix<T> CMatrix<T>::operator+ (const CMatrix& MRight) const
// ---------------------------------------------------------------------------
// Function: overloaded + operator
// Input:    matrix to use as rvalue
// Output:   matrix with the sum
// ---------------------------------------------------------------------------
{
   // compatible matrices?
   if (m_nRows != MRight.m_nRows || m_nColumns != MRight.m_nColumns)
   {
      m_ValueR = MRight.m_nRows; m_ValueC = MRight.m_nColumns;
      ErrorHandler (MATRIX_INCOMPATIBLE_MATRICES);
      return MRight;
   }

   // now add
   CMatrix<T> MResult(MRight.m_nRows, MRight.m_nColumns);
   for (int i=1; i <= MRight.m_nRows; i++)
   {
      for (int j=1; j <= MRight.m_nColumns; j++)
         MResult.m_pCells[i][j] = m_pCells[i][j] + MRight.m_pCells[i][j];
   }

   return MResult;
}

template <class T>
CMatrix<T> CMatrix<T>::operator+ (T right) const
// ---------------------------------------------------------------------------
// Function: overloaded + operator
// Input:    constant to use as rvalue
// Output:   matrix with the sum
// ---------------------------------------------------------------------------
{
   CMatrix<T> MResult(m_nRows, m_nColumns);
   for (int i=1; i <= m_nRows; i++)
   {
      for (int j=1; j <= m_nColumns; j++)
         MResult.m_pCells[i][j] = m_pCells[i][j] + right;
   }

   return MResult;
}

template <class T>
void CMatrix<T>::operator+= (const CMatrix& MRight)
// ---------------------------------------------------------------------------
// Function: overloaded += operator
// Input:    matrix to use as rvalue
// Output:   none
// ---------------------------------------------------------------------------
{
   // compatible matrices?
   if (m_nRows != MRight.m_nRows ||
       m_nColumns != MRight.m_nColumns)
   {
      m_ValueR = MRight.m_nRows; m_ValueC = MRight.m_nColumns;
      ErrorHandler (MATRIX_INCOMPATIBLE_MATRICES);
      return;
   }
   // now add
   for (int i=1; i <= MRight.m_nRows; i++)
   {
      for (int j=1; j <= MRight.m_nColumns; j++)
         m_pCells[i][j] += MRight.m_pCells[i][j];
   }
}

template <class T>
CMatrix<T> CMatrix<T>::operator- (const CMatrix& MRight) const
// ---------------------------------------------------------------------------
// Function: overloaded - operator
// Input:    matrix to use as rvalue
// Output:   matrix with the sum
// ---------------------------------------------------------------------------
{
   // compatible vectors?
   if (m_nRows != MRight.m_nRows || m_nColumns != MRight.m_nColumns)
   {
      m_ValueR = MRight.m_nRows; m_ValueC = MRight.m_nColumns;
      ErrorHandler (MATRIX_INCOMPATIBLE_MATRICES);
      return MRight;
   }

   // now subtract
   CMatrix<T> MResult(MRight.m_nRows, MRight.m_nColumns);
   for (int i=1; i <= MRight.m_nRows; i++)
   {
      for (int j=1; j <= MRight.m_nColumns; j++)
         MResult.m_pCells[i][j] = m_pCells[i][j] - MRight.m_pCells[i][j];
   }

   return MResult;
}

template <class T>
CMatrix<T> CMatrix<T>::operator- (T right) const
// ---------------------------------------------------------------------------
// Function: overloaded - operator
// Input:    constant to use as rvalue
// Output:   matrix with the sum
// ---------------------------------------------------------------------------
{
   CMatrix<T> MResult(m_nRows, m_nColumns);
   for (int i=1; i <= m_nRows; i++)
   {
      for (int j = 1; j <= m_nColumns; j++)
      {
         MResult.m_pCells[i][j] = m_pCells[i][j] - right;
      }
   }

   return MResult;
}

template <typename T>
CMatrix<T> CMatrix<T>::operator- ()   // unary -
// ---------------------------------------------------------------------------
// Function: overloaded - operator
// Input:
// Output:   modified values
// ---------------------------------------------------------------------------
{
   CMatrix<T> MT (m_nRows,m_nColumns);
   for (int i=1; i <= m_nRows; i++)
   {
      for (int j=1; j <= m_nColumns; j++)
      {
         MT.m_pCells[i][j] = -m_pCells[i][j];
      }
   }
   return MT;
}

template <class T>
void CMatrix<T>::operator-= (const CMatrix& MRight)
// ---------------------------------------------------------------------------
// Function: overloaded -= operator
// Input:    matrix to use as rvalue
// Output:   none
// ---------------------------------------------------------------------------
{
   // compatible matrices?
   if (m_nRows != MRight.m_nRows ||
         m_nColumns != MRight.m_nColumns)
   {
      m_ValueR = MRight.m_nRows; m_ValueC = MRight.m_nColumns;
      ErrorHandler (MATRIX_INCOMPATIBLE_MATRICES);
      return;
   }
   // now subtract
   for (int i=1; i <= MRight.m_nRows; i++)
   {
      for (int j = 1; j <= MRight.m_nColumns; j++)
      {
         m_pCells[i][j] -= MRight.m_pCells[i][j];
      }
   }
}

template <class T>
void CMatrix<T>::operator*= (T c)
// ---------------------------------------------------------------------------
// Function: overloaded *= operator
// Input:    scale value
// Output:   none
// ---------------------------------------------------------------------------
{
   // scale
   for (int i=1; i <= m_nRows; i++)
   {
      for (int j = 1; j <= m_nColumns; j++)
      {
         m_pCells[i][j] *= c;
      }
   }
}

template <class T>
CMatrix<T> CMatrix<T>::operator* (T c)
// ---------------------------------------------------------------------------
// Function: overloaded * operator
// Input:    scale value
// Output:   none
// ---------------------------------------------------------------------------
{
   CMatrix<T> MResult(m_nRows,m_nColumns);

   // scale
   for (int i=1; i <= m_nRows; i++)
   {
      for (int j=1; j <= m_nColumns; j++)
      {
         MResult.m_pCells[i][j] = m_pCells[i][j]*c;
      }
   }

   return MResult;
}

template <typename T>
CVector<T> CMatrix<T>::operator/ (const CVector<T>& b)
// ---------------------------------------------------------------------------
// Function: overloaded / operator to solve A x = b
// Input:    vector b
// Output:   solution in returned vector x
// ---------------------------------------------------------------------------
{
   CVector<T> V(m_nRows);
   if (b.GetSize() != m_nRows ||
       m_nRows != m_nColumns)
   {
       throw MATRIX_INCOMPATIBLE_MATRICES;
   }
   else
   {
      CMatrix<T> MTemp = (*this);
      CVector<T> bTemp = b;
      T TOL = GetTolerance();
      GaussElimination (MTemp, V, bTemp, TOL);
   }

   return V;
}

template <typename T>
CMatrix<T> operator* (T c,
                      const CMatrix<T>& RightM)
// ---------------------------------------------------------------------------
// Function: overloaded * operator
// Input:    scale value
// Output:   none
// ---------------------------------------------------------------------------
{
   CMatrix<T> MResult(RightM.m_nRows, RightM.m_nColumns);

   // scale
   for (int i=1; i <= RightM.m_nRows; i++)
   {
      for (int j=1; j <= RightM.m_nColumns; j++)
      {
         MResult.m_pCells[i][j] = c*RightM.m_pCells[i][j];
      }
   }

   return MResult;
}

template <typename T>
CMatrix<T> operator* (const CMatrix<T>& A,
                      const CMatrix<T>& B)
// ==================================================================
// Function: multiplies two matrices and stores the result
//           in the third matrix C = A * B
//    Input: matrices A and B
//   Output: matrix C
// ==================================================================
{
   // check for incompatible sizes
   int nRowsA = A.GetRows();
   int nColsA = A.GetColumns();
   int nRowsB = B.GetRows();
   int nColsB = B.GetColumns();
   T zero = static_cast<T>(0);
   CMatrix<T> C(nRowsA, nColsB);
   C.Set(zero);
   if (nColsA != nRowsB)
   {
      CArrayBase::m_ValueR = nColsA; CArrayBase::m_ValueC = nRowsB;
      throw CArrayBase::MATRIX_INCOMPATIBLE_MATRICES;
      return C;
   }

   for (int i=1; i <= nRowsA; i++)
   {
      for (int j=1; j <= nColsB; j++)
      {
         T sum = zero;
         for (int k=1; k <= nColsA; k++)
         {
            sum += A(i,k)*B(k,j);
         }
         C(i,j) = sum;
      }
   }
   return C;
}

template <typename TT>
CVector<TT> operator* (const CMatrix<TT>& A,
                       const CVector<TT>& b)
// ==================================================================
// Function: multiplication of a matrix and a vector
//           c = A * b
//    Input: matrices A and B
//   Output: vector c
// ==================================================================
{
   // check for incompatible sizes
   int nRowsA = A.GetRows();
   int nColsA = A.GetColumns();
   int nRowsB = b.GetSize();
   TT zero = static_cast<TT>(0);
   CVector<TT> c(nRowsA);
   c.Set(zero);
   if (nColsA != nRowsB)
   {
      CArrayBase::m_ValueR = nColsA; CArrayBase::m_ValueC = nRowsB;
      throw (CArrayBase::MATRIX_INCOMPATIBLE_MATRICES);
      return c;
   }

   for (int i = 1; i <= nRowsA; i++)
   {
      TT sum = zero;
      for (int j = 1; j <= nColsA; j++)
      {
         sum += A(i, j)*b(j);
      }
      c(i) = sum;
   }

   return c;
}

template <typename TT>
CVector<TT> operator* (const CVector<TT>& a,
                       const CMatrix<TT>& B)
// ==================================================================
// Function: multiplies a vector and a matrix
//           c = a * B
//    Input: vector a and matrix B
//   Output: vector C
// ==================================================================
{
   // check for incompatible sizes
   int nRowsA = a.GetSize();
   int nRowsB = B.GetRows();
   int nColsB = B.GetColumns();
   TT zero = static_cast<TT>(0);
   if (nRowsA != nRowsB)
   {
      CArrayBase::ErrorHandler (CArrayBase::MATRIX_INCOMPATIBLE_MATRICES);
   }

   CVector<TT> c(nColsB);
   c.Set(zero);
   for (int i = 1; i <= nColsB; i++)
   {
      TT sum = zero;
      for (int j = 1; j <= nRowsA; j++)
      {
         sum += a(j)*B(j, i);
      }
      c(i) = sum;
   }
   return c;
}

template <class T>
CMatrix<T> CMatrix<T>::Tr ()
// ==================================================================
// Function: computes the transpose of a matrix
//    Input: none
//   Output: the transpose
// ==================================================================
{
    int nRows = GetRows();
    int nCols = GetColumns();
    CMatrix<T> AT(nCols, nRows);

    for (int i=1; i <= nRows; i++)
    {
        for (int j=1; j <= nCols; j++)
        {
            AT(j,i) = (*this)(i,j);
        }
    }

    return AT;
}

template <class T>
T CMatrix<T>::MaxNorm()
// ==================================================================
// Function: finds and returns the value of the largest magnitude
//    Input: none
//   Output: its largest magnitude
// ==================================================================
{
    T maxM = abs((*this)(1,1));
    for (int i=1; i <= m_nRows; i++)
    {
        for (int j=1; j <= m_nColumns; j++)
        {
            maxM = (maxM > abs((*this)(i,j))? abs((*this)(i,j)) : maxM);
        }
    }

    return maxM;
}

template <class T>
void CMatrix<T>::GaussElimination (CMatrix<T>& A,
                                   CVector<T>& x,
                                   CVector<T>& b,
                                   T TOL)
// ==================================================================
// Function: solves A x = b using Gaussian Elimination Technique
//           (this version only for one rhs vector)
//    Input: Matrices A and b
//   Output: Matrix x
//           A false return value indicates a singular A matrix.
// ==================================================================
{
   // number of equations to solve
   int n = A.GetRows();

   // check problem data
   if (A.GetColumns() != n || x.GetSize() != n || b.GetSize() != n)
   {
       m_ValueR = A.GetRows(); m_ValueC = A.GetColumns(); 
       ErrorHandler (MATRIX_INCOMPATIBLE_MATRICES);
   }

   int i, j, k;   // loop indices
   T c;           // multiplier (Step 4)

   // x initially contains b
   x = b;

   // forward elimination
   for (k=1; k <= n-1; k++)              // Step 1
   {
      if (fabs(A(k,k)) <= TOL)           // Step 2
      {
         m_ValueR = k;
         ErrorHandler (MATRIX_DEPENDENT_EQUATIONS);
      }
      for (i=k+1; i <= n; i++)           // Step 3
      {
         c = A(i,k)/A(k,k);              // Step 4
         for (j=k+1; j <= n; j++)        // Step 5
            A(i,j) -= c * A(k,j);        // Step 6
         x(i) -= c * x(k);               // Step 8
      }                                  // Step 9
      int nC = n-k;
   }                                     // Step 10

   // back substitution
   if (fabs(A(n,n)) <= TOL)
   {
       m_ValueR = n;
       ErrorHandler (MATRIX_DEPENDENT_EQUATIONS);
   }
   x(n) /= A(n,n);                       // Step 11

   for (i=n-1; i >= 1; i--)              // Step 12
   {
      T sum = T(0);
      for (j=i+1; j <= n; j++)
         sum += A(i,j) * x(j);           // Step 13
      x(i) = (x(i) - sum)/A(i,i);        // Step 14
   }                                     // Step 15
}

// ==================== Error Handler ========================
template <class T>
void CMatrix<T>::ErrorHandler (ERRORVM ErrorCode) const
// ---------------------------------------------------------------------------
// Function: channels error message to base class
//           place a breakpoint here to detect the source of error
// Input:    error code
// Output:   none
// ---------------------------------------------------------------------------
{
   throw ErrorCode;
}
