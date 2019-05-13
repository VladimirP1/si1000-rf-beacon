/////////////////////////////////////////////////////////////////////////////
// WinUsbDevice.h
/////////////////////////////////////////////////////////////////////////////

#pragma once

/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include <winusb.h>

/////////////////////////////////////////////////////////////////////////////
// CWinUsbDevice Class
/////////////////////////////////////////////////////////////////////////////

// Manage communications with a USB device. Supports interrupt, bulk, and
// control endpoints.
class CWinUsbDevice
{
// Constructor/Destructor
public:
    CWinUsbDevice();
    ~CWinUsbDevice();

// Static Public Methods
public:
    static GUID GetGuid();
    static void SetGuid(GUID guid);
    static DWORD GetNumDevices();
    static BOOL GetDevicePath(DWORD index, TCHAR* path);
    static BOOL GetVidPid(DWORD index, WORD* vid, WORD* pid);

// Public Methods
public:
    BOOL OpenByIndex(DWORD index);
    BOOL OpenByDevicePath(const TCHAR* path);
    BOOL Close();
    BOOL IsOpened();

    BOOL GetOpenedDevicePath(TCHAR* path);
    BOOL GetOpenedVidPid(WORD* vid, WORD* pid);

    BOOL GetDeviceDescriptor(USB_DEVICE_DESCRIPTOR* descriptor);
    BOOL GetStringDescriptor(BYTE descriptor[MAX_PATH], BYTE index);
    BOOL GetInterfaceDescriptor(USB_INTERFACE_DESCRIPTOR* descriptor);

    BOOL ControlTransfer(WINUSB_SETUP_PACKET setup, BYTE buffer[], DWORD* bytesTransferred);
    BOOL WritePipe(BYTE pipeID, BYTE buffer[], DWORD size, DWORD* bytesWritten);
    BOOL ReadPipe(BYTE pipeID, BYTE buffer[], DWORD size, DWORD* bytesRead);
    BOOL GetPipePolicy(BYTE pipeID, DWORD policyType, LPVOID value, DWORD size, DWORD* bytesRead);
    BOOL SetPipePolicy(BYTE pipeID, DWORD policyType, LPVOID value, DWORD size);
    BOOL FlushPipe(BYTE pipeID);

// Protected Methods
protected:
    BOOL OpenWinUsbHandle();
    BOOL CloseWinUsbHandle();

    virtual BOOL OnOpen();
    virtual void OnClose();

// Protected Members
protected:
    HANDLE                      m_handle;
    WINUSB_INTERFACE_HANDLE     m_winUsbHandle;
    TCHAR                       m_devicePath[MAX_PATH];
};
