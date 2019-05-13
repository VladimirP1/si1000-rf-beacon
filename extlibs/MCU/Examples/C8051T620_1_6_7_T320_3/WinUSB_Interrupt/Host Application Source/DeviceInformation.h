/////////////////////////////////////////////////////////////////////////////
// DeviceInformation.h
/////////////////////////////////////////////////////////////////////////////

#pragma once

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "CriticalSectionLock.h"
#include <SetupAPI.h>

/////////////////////////////////////////////////////////////////////////////
// CDeviceInformation Class
/////////////////////////////////////////////////////////////////////////////

// Manages device list enumeration based on a unique GUID.
// Creates/frees a device handle using CreateFile()
class CDeviceInformation
{
// Constructor/Destructor
public:
    CDeviceInformation();
    ~CDeviceInformation();

// Public Static Methods
public:
    static BOOL GetVidPidFromDevicePath(const TCHAR* devicePath, WORD* vid, WORD* pid);

// Public Methods
public:
    GUID GetGuid();
    void SetGuid(GUID guid);
    DWORD GetNumDevices();
    HANDLE OpenByIndex(DWORD index, TCHAR* devicePath);
    HANDLE OpenByDevicePath(const TCHAR* devicePath);
    BOOL GetDevicePath(DWORD index, TCHAR* devicePath);
    BOOL GetVidPid(DWORD index, WORD* vid, WORD* pid);

// Protected Methods
protected:
    void UpdateDeviceList();

// Protected Members
protected:
    CCriticalSectionLock    m_lock;
    GUID                    m_guid;
    HDEVINFO                m_hDevInfo;
};
