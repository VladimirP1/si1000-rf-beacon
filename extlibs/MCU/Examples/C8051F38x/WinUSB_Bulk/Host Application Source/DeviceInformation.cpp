/////////////////////////////////////////////////////////////////////////////
// DeviceInformation.cpp
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "DeviceInformation.h"
#include <Windows.h>

/////////////////////////////////////////////////////////////////////////////
// Linker Dependencies
/////////////////////////////////////////////////////////////////////////////

#ifdef _M_X64
#pragma comment (lib, "C:\\WINDDK\\7600.16385.1\\lib\\wlh\\amd64\\setupapi.lib")
#else
#pragma comment (lib, "C:\\WinDDK\\7600.16385.1\\lib\\wxp\\i386\\setupapi.lib")
#endif

/////////////////////////////////////////////////////////////////////////////
// Static Global Constants
/////////////////////////////////////////////////////////////////////////////

static const GUID DEFAULT_GUID = { 0x220fa291, 0xb048, 0x418c, { 0xac, 0xa1, 0xe1, 0xfa, 0xd9, 0x9, 0x77, 0x44 } };

/////////////////////////////////////////////////////////////////////////////
// CDeviceInformation Class - Constructor/Destructor
/////////////////////////////////////////////////////////////////////////////

CDeviceInformation::CDeviceInformation()
{
    m_guid      = DEFAULT_GUID;
    m_hDevInfo  = NULL;
}

CDeviceInformation::~CDeviceInformation()
{
    m_lock.Lock();

    // Free the device info list structure
    if (m_hDevInfo != NULL)
    {
        SetupDiDestroyDeviceInfoList(m_hDevInfo);
    }

    m_lock.Unlock();
}

/////////////////////////////////////////////////////////////////////////////
// CDeviceInformation Class - Public Static Methods
/////////////////////////////////////////////////////////////////////////////

// Return the VID/PID from the specified device path
BOOL CDeviceInformation::GetVidPidFromDevicePath(const TCHAR* devicePath, WORD* vid, WORD* pid)
{
    TCHAR sVid[5];
    TCHAR sPid[5];

    memset(sVid, 0x00, sizeof(sVid));
    memset(sPid, 0x00, sizeof(sPid));

    memcpy_s(sVid, sizeof(sVid), &devicePath[12], sizeof(TCHAR) * 4);
    memcpy_s(sPid, sizeof(sPid), &devicePath[21], sizeof(TCHAR) * 4);

    *vid = (WORD)_tcstoul(sVid, NULL, 16);
    *pid = (WORD)_tcstoul(sPid, NULL, 16);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CDeviceInformation Class - Public Methods
/////////////////////////////////////////////////////////////////////////////

// Return the GUID used for device managment
GUID CDeviceInformation::GetGuid()
{
    GUID guid;

    m_lock.Lock();
    guid = m_guid;
    m_lock.Unlock();

    return guid;
}

// Set the GUID used for device management.
// Must call GetNumDevices() after the GUID is changed
void CDeviceInformation::SetGuid(GUID guid)
{
    m_lock.Lock();
    m_guid = guid;
    m_lock.Unlock();
}

// Builds a cached list of devices using the current
// GUID and returns the number of devices
DWORD CDeviceInformation::GetNumDevices()
{
    DWORD numDevices = 0;
    TCHAR devicePath[MAX_PATH];

    m_lock.Lock();

    // Update and cache device indexing structure: m_hDevInfo
    UpdateDeviceList();

    // Count the number of devices with matching GUID
    while (GetDevicePath(numDevices, devicePath))
    {
        numDevices++;
    }

    m_lock.Unlock();

    return numDevices;
}

// Connect to a device by its index in the device list and
// return the device's path
HANDLE CDeviceInformation::OpenByIndex(DWORD index, TCHAR* devicePath)
{
    HANDLE  handle = INVALID_HANDLE_VALUE;
    TCHAR   path[MAX_PATH];

    memset(path, 0x00, sizeof(path));

    m_lock.Lock();

    // Get the device path using the index in the device list
    if (GetDevicePath(index, path))
    {
        handle = OpenByDevicePath(path);

        // If device was opened successfully, then return the device path
        if (handle != INVALID_HANDLE_VALUE)
        {
            _tcscpy_s(devicePath, MAX_PATH, path);
        }
    }

    m_lock.Unlock();

    return handle;
}

// Connect to a device by its device path
HANDLE CDeviceInformation::OpenByDevicePath(const TCHAR* devicePath)
{
    HANDLE handle;

    m_lock.Lock();

	// Open the device
	handle = CreateFile(devicePath,
        GENERIC_READ | GENERIC_WRITE,
        NULL,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
        NULL);

    m_lock.Unlock();

	return handle;
}

// Return the device path for a device given its index in the device list
BOOL CDeviceInformation::GetDevicePath(DWORD index, TCHAR* devicePath)
{
    BOOL status = FALSE;

    m_lock.Lock();

    // Make sure that the device list is valid
    if (m_hDevInfo != NULL &&
        m_hDevInfo != INVALID_HANDLE_VALUE)
    {
        SP_DEVICE_INTERFACE_DATA deviceInterfaceData;
        deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

        // Query the device using the index to get the interface data
        BOOL result = SetupDiEnumDeviceInterfaces(m_hDevInfo, NULL, &m_guid, index, &deviceInterfaceData);

        // If a successful query was made, use it to get the detailed data of the device
        if (result)
        {
            BOOL                                detailResult;
            DWORD                               length;
            DWORD                               required;
            PSP_DEVICE_INTERFACE_DETAIL_DATA    pDeviceInterfaceDetailData;

            // Obtain the length of the detailed data structure, then allocate space and retrieve it
			SetupDiGetDeviceInterfaceDetail(m_hDevInfo, &deviceInterfaceData, NULL, 0, &length, NULL);
			pDeviceInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)new BYTE[length];

            pDeviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
			detailResult = SetupDiGetDeviceInterfaceDetail(m_hDevInfo, &deviceInterfaceData, pDeviceInterfaceDetailData, length, &required, NULL);
					
			// If getting the device detail provides a valid result, then copy it's device
			// path into the device path argument pointer and set status to TRUE
			if (detailResult)
			{
                _tcscpy_s(devicePath, MAX_PATH, pDeviceInterfaceDetailData->DevicePath);
				status = TRUE;
			}

			// Deallocate space for the detailed data structure
			delete [] pDeviceInterfaceDetailData;
        }
    }

    m_lock.Unlock();

    return status;
}

// Return the device VID/PID for a device given its index in the device list
BOOL CDeviceInformation::GetVidPid(DWORD index, WORD* vid, WORD* pid)
{
    BOOL    success = FALSE;
    TCHAR   devicePath[MAX_PATH];

    // Get the device path by index
    if (GetDevicePath(index, devicePath))
    {
        // Get the VID/PID from the device path
        success = GetVidPidFromDevicePath(devicePath, vid, pid);
    }

    return success;
}

/////////////////////////////////////////////////////////////////////////////
// CDeviceInformation Class - Protected Methods
/////////////////////////////////////////////////////////////////////////////

// Rebuild the device list structure using the current GUID
void CDeviceInformation::UpdateDeviceList()
{
    m_lock.Lock();

    // Free old device info
    if (m_hDevInfo != NULL)
    {
        SetupDiDestroyDeviceInfoList(m_hDevInfo);
    }

    // Use the GUID to get a handle to a list of all connected devices
    m_hDevInfo = SetupDiGetClassDevs(&m_guid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

    m_lock.Unlock();
}
