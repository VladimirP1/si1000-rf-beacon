/////////////////////////////////////////////////////////////////////////////
// WinUsbDevice.cpp
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "WinUsbDevice.h"
#include "DeviceInformation.h"
#include <Windows.h>

/////////////////////////////////////////////////////////////////////////////
// Linker Dependencies
/////////////////////////////////////////////////////////////////////////////

#ifdef _M_X64
#pragma comment (lib, "C:\\WinDDK\\7600.16385.1\\lib\\wlh\\amd64\\winusb.lib")
#else
#pragma comment (lib, "C:\\WinDDK\\7600.16385.1\\lib\\wxp\\i386\\winusb.lib")
#endif

/////////////////////////////////////////////////////////////////////////////
// Static Global Variables
/////////////////////////////////////////////////////////////////////////////

// A static device resource manager capable
// of returning device information using a
// specified GUID.
//
// Can also open a device and return its
// handle.
static CDeviceInformation DeviceInfo;

/////////////////////////////////////////////////////////////////////////////
// CWinUsbDevice Class - Constructor/Destructor
/////////////////////////////////////////////////////////////////////////////

CWinUsbDevice::CWinUsbDevice()
{
    m_handle        = INVALID_HANDLE_VALUE;
    m_winUsbHandle = INVALID_HANDLE_VALUE;

    memset(m_devicePath, 0x00, sizeof(m_devicePath));
}

CWinUsbDevice::~CWinUsbDevice()
{
    if (IsOpened())
    {
        Close();
    }
}

/////////////////////////////////////////////////////////////////////////////
// CWinUsbDevice Class - Static Public Methods
/////////////////////////////////////////////////////////////////////////////

// Get the current GUID used to manage
// the list of devices
GUID CWinUsbDevice::GetGuid()
{
    return DeviceInfo.GetGuid();
}

// Set the GUID used to manage the list
// of devices
void CWinUsbDevice::SetGuid(GUID guid)
{
    DeviceInfo.SetGuid(guid);
}

// Return the number of devices that match
// the current GUID
DWORD CWinUsbDevice::GetNumDevices()
{
    return DeviceInfo.GetNumDevices();
}

// Get the device path for the device given its
// index in the device list
BOOL CWinUsbDevice::GetDevicePath(DWORD index, TCHAR* path)
{
    return DeviceInfo.GetDevicePath(index, path);
}

// Get the VID/PID for the device given its
// index in the device list
BOOL CWinUsbDevice::GetVidPid(DWORD index, WORD* vid, WORD* pid)
{
    return DeviceInfo.GetVidPid(index, vid, pid);
}

/////////////////////////////////////////////////////////////////////////////
// CWinUsbDevice Class - Public Device Management Methods
/////////////////////////////////////////////////////////////////////////////

// Connect to a device given its index in the device list
BOOL CWinUsbDevice::OpenByIndex(DWORD index)
{
    BOOL success = FALSE;

    // Clear the cached device path
    memset(m_devicePath, 0x00, sizeof(m_devicePath));

    // Connect to the device and store the device path for later
    m_handle = DeviceInfo.OpenByIndex(index, m_devicePath);

    if (m_handle != INVALID_HANDLE_VALUE)
    {
        if (OnOpen())
        {
            success = TRUE;
        }
        else
        {
            Close();
        }
    }

    return success;
}

// Connect to a device given its path
BOOL CWinUsbDevice::OpenByDevicePath(const TCHAR* path)
{
    BOOL success = FALSE;

    // Clear the cached device path
    memset(m_devicePath, 0x00, sizeof(m_devicePath));

    // Connect to the device
    m_handle = DeviceInfo.OpenByDevicePath(path);

    // Connnected successfully
    if (m_handle != INVALID_HANDLE_VALUE)
    {
        // Cache the device path
        _tcscpy_s(m_devicePath, MAX_PATH, path);

        if (OnOpen())
        {
            success = TRUE;
        }
        else
        {
            Close();
        }
    }

    return success;
}

// Close the current device if opened
BOOL CWinUsbDevice::Close()
{
    BOOL success = TRUE;
    
    // Close the control handle
    if (!CloseWinUsbHandle())
    {
        success = FALSE;
    }

    // Close the device handle
    if (!CloseHandle(m_handle))
    {
        success = FALSE;
    }

    OnClose();

    return success;
}

// Return TRUE if a device is opened
BOOL CWinUsbDevice::IsOpened()
{
    return m_handle != INVALID_HANDLE_VALUE;
}

// Return the cached device path if a device is opened
BOOL CWinUsbDevice::GetOpenedDevicePath(TCHAR* path)
{
    BOOL success = FALSE;

    if (IsOpened())
    {
        _tcscpy_s(path, MAX_PATH, m_devicePath);
        success = TRUE;
    }

    return success;
}

// Return the cached VID/PID if a device is opened
BOOL CWinUsbDevice::GetOpenedVidPid(WORD* vid, WORD* pid)
{
    BOOL success = FALSE;

    if (IsOpened())
    {
        // Get the VID/PID from the device path
        success = CDeviceInformation::GetVidPidFromDevicePath(m_devicePath, vid, pid);
    }

    return success;
}

// Return the standard USB device descriptor
BOOL CWinUsbDevice::GetDeviceDescriptor(USB_DEVICE_DESCRIPTOR* descriptor)
{
    BOOL    success     = FALSE;
    DWORD   read        = 0;
    DWORD   err;

    memset(descriptor, 0x00, sizeof(USB_DEVICE_DESCRIPTOR));

    if (IsOpened())
    {
        success = WinUsb_GetDescriptor(
            m_winUsbHandle,
            USB_DEVICE_DESCRIPTOR_TYPE,
            0,
            0,
            (PUCHAR)descriptor,
            sizeof(USB_DEVICE_DESCRIPTOR),
            &read);

        if (!success)
        {
            err = GetLastError();
        }
    }

    return success;
}

// Return a string descriptor by index
BOOL CWinUsbDevice::GetStringDescriptor(BYTE descriptor[MAX_PATH], BYTE index)
{
    BOOL    success     = FALSE;
    DWORD   read        = 0;
    DWORD   err;

    if (IsOpened())
    {
        success = WinUsb_GetDescriptor(
            m_winUsbHandle,
            USB_STRING_DESCRIPTOR_TYPE,
            index,
            0,
            (PUCHAR)descriptor,
            MAX_PATH,
            &read);

        if (!success)
        {
            err = GetLastError();
        }
    }

    return success;
}

// Return the USB interface descriptor
BOOL CWinUsbDevice::GetInterfaceDescriptor(USB_INTERFACE_DESCRIPTOR* descriptor)
{
    BOOL    success = FALSE;
    DWORD   err;

    memset(descriptor, 0x00, sizeof(USB_INTERFACE_DESCRIPTOR));

    if (IsOpened())
    {
        success = WinUsb_QueryInterfaceSettings(
            m_winUsbHandle,
            0,
            descriptor);

        if (!success)
        {
            err = GetLastError();
        }
    }

    return success;
}

// Transfer over the control endpoint
BOOL CWinUsbDevice::ControlTransfer(WINUSB_SETUP_PACKET setup, BYTE buffer[], DWORD* bytesTransferred)
{
    BOOL    success = FALSE;
    DWORD   err;

    if (IsOpened())
    {
        success = WinUsb_ControlTransfer(
            m_winUsbHandle,
            setup,
            buffer,
            setup.Length,
            bytesTransferred,
            NULL);

        if (!success)
        {
            err = GetLastError();
        }
    }

    return success;
}

// Send data using a bulk or interrupt OUT endpoint
BOOL CWinUsbDevice::WritePipe(BYTE pipeID, BYTE buffer[], DWORD size, DWORD* bytesWritten)
{
    BOOL success = FALSE;
    DWORD err;

    if (IsOpened())
    {
        success = WinUsb_WritePipe(
            m_winUsbHandle,
            pipeID,
            buffer,
            size,
            bytesWritten,
            NULL);
        
        if (!success)
        {
            err = GetLastError();
        }
    }

    return success;
}

// Receive data using a bulk or interrupt IN endpoint
BOOL CWinUsbDevice::ReadPipe(BYTE pipeID, BYTE buffer[], DWORD size, DWORD* bytesRead)
{
    BOOL success = FALSE;
    DWORD err;

    if (IsOpened())
    {
        success = WinUsb_ReadPipe(
            m_winUsbHandle,
            pipeID,
            buffer,
            size,
            bytesRead,
            NULL);
        
        if (!success)
        {
            err = GetLastError();
        }
    }

    return success;
}

// Get USB pipe policy (ie timeouts)
//
// See "WinUSB Functions for Pipe Policy Modification" on MSDN
// for specific policy type details
BOOL CWinUsbDevice::GetPipePolicy(BYTE pipeID, DWORD policyType, LPVOID value, DWORD size, DWORD* bytesRead)
{
    BOOL success = FALSE;
    DWORD err;

    if (IsOpened())
    {
        DWORD valueLength = size;

        success = WinUsb_GetPipePolicy(
            m_winUsbHandle,
            pipeID,
            policyType,
            &valueLength,
            value);
        
        if (!success)
        {
            err = GetLastError();
        }
        else
        {
            if (bytesRead != NULL)
                *bytesRead = valueLength;
        }
    }

    return success;
}

// Set USB pipe policy (ie timeouts)
//
// See "WinUSB Functions for Pipe Policy Modification" on MSDN
// for specific policy type details
BOOL CWinUsbDevice::SetPipePolicy(BYTE pipeID, DWORD policyType, LPVOID value, DWORD size)
{
    BOOL success = FALSE;
    DWORD err;

    if (IsOpened())
    {
        success = WinUsb_SetPipePolicy(
            m_winUsbHandle,
            pipeID,
            policyType,
            size,
            value);
        
        if (!success)
        {
            err = GetLastError();
        }
    }

    return success;
}

// Flush specified pipe
BOOL CWinUsbDevice::FlushPipe(BYTE pipeID)
{
    BOOL success = FALSE;
    DWORD err;

    if (IsOpened())
    {
        success = WinUsb_FlushPipe(
            m_winUsbHandle,
            pipeID);
        
        if (!success)
        {
            err = GetLastError();
        }
    }

    return success;
}

/////////////////////////////////////////////////////////////////////////////
// CWinUsbDevice Class - Protected Methods
/////////////////////////////////////////////////////////////////////////////

// Retrieve a WinUSB handle
BOOL CWinUsbDevice::OpenWinUsbHandle()
{
    BOOL success = FALSE;

    if (m_handle != INVALID_HANDLE_VALUE)
    {
        success = WinUsb_Initialize(m_handle, &m_winUsbHandle);
    }

    return success;
}

// Free a WinUSB handle
BOOL CWinUsbDevice::CloseWinUsbHandle()
{
    BOOL success = FALSE;

    if (m_winUsbHandle != INVALID_HANDLE_VALUE)
    {
        success = WinUsb_Free(m_winUsbHandle);

        m_winUsbHandle = INVALID_HANDLE_VALUE;
    }

    return success;
}

// OnOpen() is called after Open...()
BOOL CWinUsbDevice::OnOpen()
{
    BOOL success = FALSE;

    if (OpenWinUsbHandle())
    {
        success = TRUE;
    }

    return success;
}

// OnClose() is called after Close()
void CWinUsbDevice::OnClose()
{
    m_handle       = INVALID_HANDLE_VALUE;
    m_winUsbHandle = INVALID_HANDLE_VALUE;

    memset(m_devicePath, 0x00, sizeof(m_devicePath));
}
