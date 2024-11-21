// **************************************************
// Vector Template Class
// Copyright(c), 2000-23, S. D. Rajan
// All rights reserved
//
// Implements vector as a simple container
// Indexing starts at 1
// -------------------------------------------------------------------------------
// Visual Studio Debugger:
// In the Watch 1 window, type nameofvector.m_pCells, n to view the entire vector
// n is the size of the vector + 1. Note that the first location may have garbage
// value since the first location is unused.
// **************************************************
#pragma once

#include <string>
#include <iostream>
#include <iomanip>
#include <exception>
#include <sstream>
#include <algorithm>
#include "arraybasecontainerEXH.h"

// defines the vector template class
template <class T>
class CVector : public CArrayBase
{
   // -------------------------
   // template friend functions
   // -------------------------
   // overloaded * operator
   template <typename TT>
   friend CVector<TT> operator* (TT c,
                                 const CVector<TT>&);

   // overloaded << operator
   template <typename TT>
   friend std::ostream& operator<< (std::ostream& os, const CVector<TT>&);

public:
   CVector ();                                 // default constructor
   explicit CVector (int);                     // constructor
   explicit CVector (const char) = delete;     // constructor disallowing char argument
   explicit CVector (const bool) = delete;     // constructor disallowing bool argument
   CVector (const char*);                      // constructor
   explicit CVector (const char*, int);        // constructor
   CVector (const CVector<T>&);                // copy constructor
   CVector (const std::initializer_list<T>&);  // ctor with initializers
   CVector (CVector<T>&&);                     // move constructor
   ~CVector ();                                // destructor
   void SetSize (int);                         // sets the size of the vector
   // used with the default constructor
   void SetSize (int, T);                      // sets the size of the vector and initial value

   // ------------------
   // helper functions
   // ------------------
   // gets the current size of the vector
   int GetSize () const;
   // gets the vector name
   void GetName (std::string& strName) const;
   // display the current values
   void Display (std::ostream& OF,
                 const std::string& strMessage) const;

   // -------------------------------
   // vector manipulations (mutator)
   // -------------------------------
   void Set (T);           // sets the value of all
                           // elements of a vector
   void SetName (const std::string&); // sets the name of the vector

   // --------------------
   // overloaded operators
   // --------------------
   T& operator() (int);                              // row access
   const T& operator() (int) const;                  // row access
   CVector<T>  operator- ();                         // overloaded unary operator
   bool        operator== (const CVector<T>&);       // overloaded == operator
   bool        operator!= (const CVector<T>&);       // overloaded != operator
   CVector<T>& operator= (const CVector<T>&);        // overloaded = operator
   CVector<T>& operator= (CVector<T>&&);             // overloaded move = operator
   CVector<T>  operator+ (const CVector<T>&) const;  // overloaded + operator
   void        operator+= (const CVector<T>&);       // overloaded += operator
   CVector<T>  operator- (const CVector<T>&) const;  // overloaded - operator
   void        operator-= (const CVector<T>&);       // overloaded -= operator
   CVector<T>  operator* (T c);                      // overloaded * operator
   void        operator*= (T c);                     // overloaded *= operator
   T           operator* (const CVector<T>&);        // overloaded * operator

   // --------------------
   // other operations
   // --------------------
   T           TwoNorm ();   // computes the two norm
   T*          begin ();     // for use with C++ STL
   T*          end ();       // for use with C++ STL

   // less function
   template <class T> struct less
   {
       constexpr bool operator() (const T& lhs, const T& rhs) const
       {
           return lhs < rhs;
       }

       // Type of first parameter
       typedef T first_argument_type;

       // Type of second parameter
       typedef T second_argument_type;

       // The result is returned as bool type
       typedef bool result_type;
   };
   // greater function
   template <class T> struct greater
   {
       constexpr bool operator() (const T& lhs, const T& rhs) const
       {
           return lhs > rhs;
       }

       // Type of first parameter
       typedef T first_argument_type;

       // Type of second parameter
       typedef T second_argument_type;

       // The result is returned as bool type
       typedef bool result_type;
   };

private:
   std::string m_strName;   // vector identifier
   int m_nRows;             // number of rows in the vector
   T   *m_pCells;           // address where the vector of
                            // type T is stored
   void Release ();         // similar to destructor
   void Initialize ();      // initialized values
   inline T GetTolerance () const; // tolerance for bool checks
   void ErrorHandler (ERRORVM ErrorCode) const;
};

// =============== definitions ===========================================
template <class T>
CVector<T>::CVector ()
// ---------------------------------------------------------------------------
// Function: default ctor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
   Initialize ();
}

template <class T>
void CVector<T>::Initialize ()
// ---------------------------------------------------------------------------
// Function: initializer
// Input:
// Output:
// ---------------------------------------------------------------------------
{
   m_pCells = nullptr;
   m_nRows = 0;
}

template <class T>
CVector<T>::CVector (int n)
// ---------------------------------------------------------------------------
// Function: overloaded ctor
// Input:    size of the vector
// Output:   none
// ---------------------------------------------------------------------------
{
   Initialize();
   SetSize (n);
}

template <class T>
CVector<T>::CVector (const char* strName)
// ---------------------------------------------------------------------------
// Function: overloaded ctor
// Input:    vector name
// Output:   none
// ---------------------------------------------------------------------------
{
   Initialize();
   m_strName = strName;
}

template <class T>
CVector<T>::CVector (const char* strName, int n)
// ---------------------------------------------------------------------------
// Function: overloaded ctor
// Input:    vector name and size
// Output:   none
// ---------------------------------------------------------------------------
{
   Initialize();
   m_strName = strName;
   SetSize (n);
}

template <class T>
CVector<T>::CVector (const CVector<T>& A)
// ---------------------------------------------------------------------------
// Function: copy ctor
// Input:    vector
// Output:   none
// ---------------------------------------------------------------------------
{
   Initialize();
   m_strName = A.m_strName;
   if (A.m_nRows == 0)
       ErrorHandler (VECTOR_INVALID_SIZE);
   SetSize (A.m_nRows);
   for (int i=1; i <= m_nRows; i++)
      m_pCells[i] = A.m_pCells[i];
}

template <class T>
CVector<T>::CVector (CVector<T>&& A)
// ---------------------------------------------------------------------------
// Function: move ctor
// Input:    vector
// Output:   none
// ---------------------------------------------------------------------------
{
   // copy the data pointer and its size from the source object
   m_pCells = A.m_pCells;
   m_nRows = A.GetSize();
   m_strName = A.m_strName;

   // release the data pointer from the source object so that
   // the destructor does not free the memory
   A.m_pCells = nullptr;
   A.m_nRows = 0;
}

template <class T>
CVector<T>::CVector (const std::initializer_list<T>& List)
// ---------------------------------------------------------------------------
// Function: Constructor with initial values
// Input:    Size of vector, list of values
// Output:   none
// ---------------------------------------------------------------------------
{
   Initialize();
   m_strName = "?";

   int nR = static_cast<int>(List.size());
   if (nR == 0)
   {
      m_ValueR = nR;
      ErrorHandler (VECTOR_INVALID_SIZE);
   }

   SetSize (nR);
   int nIndex = 0;
   for (auto i = List.begin(); i < List.end(); i++)
   {
      (*this)(++nIndex) = *(i);
   }
}

template <class T>
void CVector<T>::SetSize (int nR)
// ---------------------------------------------------------------------------
// Function: dynamically allocates memory
// Input:    vector size
// Output:   none
// ---------------------------------------------------------------------------
{
   // do nothing if the current size is the same as the requested size
   if (nR == m_nRows)
       return;

   // check whether nR is legal
   if (nR <= 0)
   {
      m_ValueR = nR;
      ErrorHandler (VECTOR_INVALID_SIZE);
      m_nRows = 0;
   }
   Release ();
   try
   {
      m_pCells = new T [nR + 1];
   }
   catch (std::bad_alloc)
   {
      m_ValueR = nR;
      ErrorHandler (VECTOR_ALLOCATION_ERROR);
   }
   m_nRows = nR;
   m_dAllocated += static_cast<double>(sizeof(T)*(nR+1));
}

template <class T>
void CVector<T>::SetSize (int nR, T v)
// ---------------------------------------------------------------------------
// Function: dynamically allocates memory
// Input:    vector size
// Output:   none
// ---------------------------------------------------------------------------
{
    SetSize (nR);
    Set(v);
}

template <class T>
CVector<T>::~CVector ()
// ---------------------------------------------------------------------------
// Function: dtor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
   // deallocate storage
   Release ();
}

template <class T>
void CVector<T>::Release ()
// ---------------------------------------------------------------------------
// Function: dynamically deallocates memory
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
   // deallocate storage
   if (m_pCells != nullptr)
   {
      delete [] m_pCells;
      m_dDeAllocated += static_cast<double>(sizeof(T)*(m_nRows+1));
      m_pCells = nullptr;
      m_nRows = 0;
   }
}

// =============== member functions ===========================================
template <class T>
T* CVector<T>::begin ()
// ---------------------------------------------------------------------------
// Function: return the memory address of where the vector starts
// Input:    
// Output:   
// ---------------------------------------------------------------------------
{
    return &m_pCells[1];
}

template <class T>
T* CVector<T>::end ()
// ---------------------------------------------------------------------------
// Function: return the memory address of where the vector ends
// Input:    
// Output:   
// ---------------------------------------------------------------------------
{
    return &m_pCells[m_nRows+1];
}

template <class T>
void CVector<T>::SetName (const std::string& strName)
// ---------------------------------------------------------------------------
// Function: sets the name of the vector
// Input:    vector name
// Output:   none
// ---------------------------------------------------------------------------
{
   m_strName = strName;
}

template <class T>
void CVector<T>::GetName (std::string& strName) const
// ---------------------------------------------------------------------------
// Function: gets the name of the vector
// Input:    string to hold vector name
// Output:   vector name
// ---------------------------------------------------------------------------
{
   strName = m_strName;
}

template <class T>
void CVector<T>::Set (T dV)
// ---------------------------------------------------------------------------
// Function: sets the value of all the elements in the vector to the
//           specified value
// Input:    specified value
// Output:   none
// ---------------------------------------------------------------------------
{
   for (int i=1; i <= m_nRows; i++)
      m_pCells[i] = dV;
}

template <class T>
int CVector<T>::GetSize () const
// ---------------------------------------------------------------------------
// Function: gets the size of the vector
// Input:    none
// Output:   returns the size
// ---------------------------------------------------------------------------
{
   return m_nRows;
}

template <class T>
T CVector<T>::TwoNorm () 
// ---------------------------------------------------------------------------
// Function: computes the two-norm of the vector
// Input:    none
// Output:   the two-norm
// ---------------------------------------------------------------------------
{
   T TN = 0;
   for (int i=1; i <= m_nRows; i++)
   {
       TN += m_pCells[i]*m_pCells[i];
   }

   return (sqrt(TN));
}

template <class T>
void CVector<T>::Display (std::ostream& OF, const std::string& strMessage) const
// ---------------------------------------------------------------------------
// Function: displays the contents of the vector
// Input:    string to display
// Output:   none
// ---------------------------------------------------------------------------
{
   const int NUMPERROW = 6;
   OF << strMessage;
   int nCurRow = 0;
   for (int i = 1; i <= m_nRows; i += NUMPERROW)
   {
      ++nCurRow;
      int nS = (nCurRow - 1)*NUMPERROW + 1;
      int nE = std::min(nS + NUMPERROW - 1, m_nRows);
      for (int j = nS; j <= nE; j++)
      {
         OF << "(" << std::setw(3) << j << ") " << std::setw(15) << m_pCells[j] << "  ";
      }
      OF << std::endl;
   }
}

// ----------------------------
// specialized member functions
// ----------------------------
// ---------------------------------------------------------------
// Function: these functions obtain the tolerance for carrying out
//           logical checks
// Input:    None
// Output:   returns the tolerance value
// ---------------------------------------------------------------
template <>
inline int CVector<int>::GetTolerance() const
{
   return 0;
}

template <>
inline float CVector<float>::GetTolerance() const
{
   return 0.0001f;
}

template <>
inline double CVector<double>::GetTolerance() const
{
   return 1.0e-6;
}

// ==================== Overloaded Operators ========================
#ifdef _DEBUG
template <class T>
T& CVector<T>::operator() (int nR)  // T& is reference
// ---------------------------------------------------------------------------
// Function: overloaded () operator to access vector contents
//           carries out bound checking
// Input:    index or location
// Output:   value at the specified index
// ---------------------------------------------------------------------------
{
   // row-column reference in bounds?
   if (nR <= 0 || nR > m_nRows)
   {
      m_ValueR = nR;
      ErrorHandler (VECTOR_INDEX_OUT_OF_BOUNDS);
      return m_pCells[0];
   }
   else
      return m_pCells[nR];
}
#else
template <class T>
inline T& CVector<T>::operator() (int nR)
// ---------------------------------------------------------------------------
// Function: overloaded () operator to access vector contents
// Input:    index or location
// Output:   value at the specified index
// ---------------------------------------------------------------------------
{
   return m_pCells[nR];
}
#endif

#ifdef _DEBUG
template <class T>
const T& CVector<T>::operator() (int nR) const
// ---------------------------------------------------------------------------
// Function: overloaded () operator to access vector contents
// Input:    index or location
// Output:   value at the specified index
// ---------------------------------------------------------------------------
{
   // row-column reference in bounds?
   if (nR <= 0 || nR > m_nRows)
   {
      m_ValueR = nR;
      ErrorHandler (VECTOR_INDEX_OUT_OF_BOUNDS);
      return m_pCells[0];
   }
   else
      return m_pCells[nR];
}
#else
template <class T>
inline const T& CVector<T>::operator() (int nR) const
// ---------------------------------------------------------------------------
// Function: overloaded () operator to access vector contents
// Input:    index or location
// Output:   value at the specified index
// ---------------------------------------------------------------------------
{
   return m_pCells[nR];
}
#endif

// unary -
template <typename T>
CVector<T> CVector<T>::operator- ()   // unary -
// ---------------------------------------------------------------------------
// Function: overloaded - operator
// Input:
// Output:   modified values
// ---------------------------------------------------------------------------
{
   CVector<T> VT (m_nRows);
   for (int i=1; i <= m_nRows; i++)
      VT(i) = -m_pCells[i];
   return VT;
}

template <class T>
CVector<T>& CVector<T>::operator= (const CVector<T>& matarg)
// ---------------------------------------------------------------------------
// Function: overloaded = operator
// Input:    vector to use as rvalue
// Output:   modified values
// ---------------------------------------------------------------------------
{
   // check whether vector is assigned to itself
   if (this != &matarg)
   {
      // compatible vectors?
      if (m_nRows != matarg.m_nRows)
      {
         m_ValueR = matarg.m_nRows;
         ErrorHandler (CArrayBase::VECTOR_INCOMPATIBLE_VECTORS);
         return *this;
      }
      // now copy
      for (int i=1; i <= matarg.m_nRows; i++)
         m_pCells[i] = matarg.m_pCells[i];
   }

   return *this;
}

template <class T>
CVector<T>& CVector<T>::operator= (CVector<T>&& A)
// ---------------------------------------------------------------------------
// Function: overloaded move = operator
// Input:    vector to use as rvalue
// Output:   modified values
// ---------------------------------------------------------------------------
{
   // check whether vector is assigned to itself
   if (this != &A)
   {
      // compatible vectors?
      if (m_nRows != A.m_nRows)
      {
         m_ValueR = A.m_nRows;
         ErrorHandler (VECTOR_INCOMPATIBLE_VECTORS);
         return *this;
      }
      // free the existing resource
      Release ();

      // copy the data pointer and its length from the source object
      m_pCells = A.m_pCells;
      m_nRows = A.GetSize();
      m_strName = A.m_strName;

      // release the data pointer from the source object so that
      // the destructor does not free the memory multiple times
      A.m_pCells = NULL;
      A.m_nRows = 0;
   }

   return *this;
}

template <class T>
bool CVector<T>::operator== (const CVector<T>& VRight)
// ---------------------------------------------------------------------------
// Function: overloaded == operator
// Input:    scale value
// Output:   none
// ---------------------------------------------------------------------------
{
   for (int i=1; i <= m_nRows; i++)
   {
      if (abs(m_pCells[i]-VRight.m_pCells[i]) > GetTolerance())
         return false;
   }
   return true;
}

template <class T>
bool CVector<T>::operator!= (const CVector<T>& VRight)
// ---------------------------------------------------------------------------
// Function: overloaded != operator
// Input:    scale value
// Output:   none
// ---------------------------------------------------------------------------
{
   for (int i=1; i <= m_nRows; i++)
   {
      if (abs(m_pCells[i]-VRight.m_pCells[i]) > GetTolerance())
         return true;
   }
   return false;
}

template <class T>
CVector<T> CVector<T>::operator+ (const CVector<T>& VRight) const
// ---------------------------------------------------------------------------
// Function: overloaded + operator
// Input:    vector to use as rvalue
// Output:   vector with the sum
// ---------------------------------------------------------------------------
{
   CVector<T> VResult;

   // compatible vectors?
   if (m_nRows != VRight.m_nRows)
   {
      m_ValueR = VRight.m_nRows;
      ErrorHandler (VECTOR_INCOMPATIBLE_VECTORS);
      return VResult;
   }

   VResult.SetSize (VRight.m_nRows);
   // now add
   for (int i=1; i <= VRight.m_nRows; i++)
      VResult.m_pCells[i] = m_pCells[i] + VRight.m_pCells[i];

   return VResult;
}

template <class T>
void CVector<T>::operator+= (const CVector<T>& VRight)
// ---------------------------------------------------------------------------
// Function: overloaded += operator
// Input:    vector to use as rvalue
// Output:   none
// ---------------------------------------------------------------------------
{
   // compatible vectors?
   if (m_nRows != VRight.m_nRows)
   {
      m_ValueR = VRight.m_nRows;
      ErrorHandler (VECTOR_INCOMPATIBLE_VECTORS);
      return;
   }
   // now add
   for (int i=1; i <= VRight.m_nRows; i++)
      m_pCells[i] += VRight.m_pCells[i];
}

template <class T>
CVector<T> CVector<T>::operator- (const CVector<T>& VRight) const
// ---------------------------------------------------------------------------
// Function: overloaded - operator
// Input:    vector to use as rvalue
// Output:   vector with the sum
// ---------------------------------------------------------------------------
{
   // compatible vectors?
   if (m_nRows != VRight.m_nRows)
   {
      m_ValueR = VRight.m_nRows;
      ErrorHandler (VECTOR_INCOMPATIBLE_VECTORS);
      return VRight;
   }

   CVector<T> VResult(VRight.m_nRows);
   // now subtract
   for (int i=1; i <= VRight.m_nRows; i++)
      VResult.m_pCells[i] = m_pCells[i] - VRight.m_pCells[i];

   return VResult;
}

template <class T>
void CVector<T>::operator-= (const CVector<T>& VRight)
// ---------------------------------------------------------------------------
// Function: overloaded -= operator
// Input:    vector to use as rvalue
// Output:   none
// ---------------------------------------------------------------------------
{
   // compatible vectors?
   if (m_nRows != VRight.m_nRows)
   {
      m_ValueR = VRight.m_nRows;
      ErrorHandler (VECTOR_INCOMPATIBLE_VECTORS);
      return;
   }
   // now add
   for (int i=1; i <= VRight.m_nRows; i++)
      m_pCells[i] -= VRight.m_pCells[i];
}

template <class T>
void CVector<T>::operator*= (T c)
// ---------------------------------------------------------------------------
// Function: overloaded *= operator
// Input:    scale value
// Output:   none
// ---------------------------------------------------------------------------
{
   // scale
   for (int i=1; i <= m_nRows; i++)
      m_pCells[i] *= c;
}

template <class T>
CVector<T> CVector<T>::operator* (T c)
// ---------------------------------------------------------------------------
// Function: overloaded * operator
// Input:    scale value
// Output:   none
// ---------------------------------------------------------------------------
{
   CVector<T> VResult(m_nRows);

   // scale
   for (int i=1; i <= m_nRows; i++)
      VResult.m_pCells[i] = m_pCells[i]*c;

   return VResult;
}

template <class T>
T CVector<T>::operator* (const CVector<T>& VRight)
// ---------------------------------------------------------------------------
// Function: overloaded * operator (dot product)
// Input:    scale value
// Output:   none
// ---------------------------------------------------------------------------
{
   // compatible vectors?
   if (m_nRows != VRight.m_nRows)
   {
      m_ValueR = VRight.m_nRows;
      ErrorHandler (VECTOR_INCOMPATIBLE_VECTORS);
      return 0;
   }
   // dot product
   T sum = 0;
   for (int i=1; i <= m_nRows; i++)
      sum += m_pCells[i]*VRight.m_pCells[i];

   return sum;
}

template <typename T>
CVector<T> operator* (T c,
                      const CVector<T>& RightV)
// ---------------------------------------------------------------------------
// Function: overloaded * operator
// Input:    scale value
// Output:   none
// ---------------------------------------------------------------------------
{
   CVector<T> VResult(RightV.m_nRows);

   // scale
   for (int i=1; i <= RightV.m_nRows; i++)
      VResult.m_pCells[i] = c*RightV.m_pCells[i];

   return VResult;
}

// ==================== Error Handler ========================
template <class T>
void CVector<T>::ErrorHandler (ERRORVM ErrorCode) const
// ---------------------------------------------------------------------------
// Function: channels error message to base class
//           place a breakpoint here to detect the source of error
// Input:    error code
// Output:   none
// ---------------------------------------------------------------------------
{
   throw ErrorCode;
}
