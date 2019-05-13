/////////////////////////////////////////////////////////////////////////////
// CriticalSectionLock.h
/////////////////////////////////////////////////////////////////////////////

#ifndef CRITICAL_SECTION_LOCK_H
#define CRITICAL_SECTION_LOCK_H

class CCriticalSectionLock
{
// Constructor/Destructor
public:
	inline CCriticalSectionLock() throw()
	{
		InitializeCriticalSection(&m_cs);
	}
	inline ~CCriticalSectionLock() throw()
	{
		DeleteCriticalSection(&m_cs);
	}

// Public Methods
public:
	inline void Lock() throw()
	{
		EnterCriticalSection(&m_cs);
	}
	inline void Unlock() throw()
	{
		LeaveCriticalSection(&m_cs);
	}

// Protected Members
protected:
	CRITICAL_SECTION m_cs;
};

#endif // CRITICAL_SECTION_LOCK_H
