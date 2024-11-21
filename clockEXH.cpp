/*********************************************
Utility Function
Copyright(c) 2006-23, S. D. Rajan
All rights reserved

Object-Oriented Numerical Analysis
*********************************************/
#include "clockEXH.h"

CClock::CClock ()
// ---------------------------------------------------------------------------
// Function: ctor. marks the current time
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    time (&m_Time); 
}

CClock::CClock (const CClock& Time)
// ---------------------------------------------------------------------------
// Function: copy constructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    m_Time = Time.m_Time;
}

CClock::~CClock()
// ---------------------------------------------------------------------------
// Function: destructor
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
}

double CClock::DiffTime (const CClock& Time) const
// ---------------------------------------------------------------------------
// Function: computes difference in time (in seconds) between
//           two time instances
// Input:    one of the time instances
// Output:   none
// ---------------------------------------------------------------------------
{
    return (difftime (Time.m_Time, m_Time));
}

double CClock::DiffTime () const
// ---------------------------------------------------------------------------
// Function: computes difference in time (in seconds) between
//           current time and marked time
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    time_t CurTime;
    time (&CurTime);
    return (difftime (CurTime, m_Time));
}

    
void CClock::MarkTime ()
// ---------------------------------------------------------------------------
// Function: marks the current time
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    time (&m_Time); 
}

long CClock::GetMarkedTime () const
// ---------------------------------------------------------------------------
// Function: gets the marked time
// Input:    none
// Output:   none
// ---------------------------------------------------------------------------
{
    return static_cast<long>(m_Time); 
}

void CClock::GetDateTime (std::string& strDateTime) const
// ---------------------------------------------------------------------------
// Function: returns as a string the current date and time
// Input:    string to hold current date and time
// Output:   string containing the current date and time
// ---------------------------------------------------------------------------
{
	char buffer[32];
	time_t rawtime;
	struct tm timeinfo;

	time (&rawtime);
	localtime_s (&timeinfo, &rawtime);
	asctime_s (buffer, 32, &timeinfo);
	strDateTime = buffer;
}

