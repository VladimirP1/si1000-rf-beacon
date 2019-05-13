///////////////////////////////////////////////////////////////////////////////
// FileTransferDevice.h
///////////////////////////////////////////////////////////////////////////////

#pragma once

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

#include "WinUsbDevice.h"
#include <vector>

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

// Endpoint Sizes
#define IN_PACKET_SIZE      64
#define OUT_PACKET_SIZE     64

// Pipe IDs
#define IN_PIPE_ID          0x81
#define OUT_PIPE_ID         0x01

// USB Command IDs
#define CMD_SET_FLASH_KEY     0
#define CMD_GET_PAGE_INFO     1
#define CMD_WRITE_PAGE        2
#define CMD_READ_PAGE         3

// USB Response Codes
#define RSP_SUCCESS           0
#define RSP_INVALID           1

// Vendor Request Definitions
#define REQ_RST_STATE      0x01        // Reset state machine
#define VALUE_DEFAULT      0x0000      // Leave flash interface unchanged
#define VALUE_UNLOCK       0x0001      // Unlock flash interface

///////////////////////////////////////////////////////////////////////////////
// CFileTransferDevice Class
///////////////////////////////////////////////////////////////////////////////

// Manage USB bulk commands and control requests for the USB Bulk file
// transfer example device firmware using WinUSB
class CFileTransferDevice : public CWinUsbDevice
{
// Constructor/Destructor
public:
    CFileTransferDevice(void);
    ~CFileTransferDevice(void);

// Public Methods
public:
    BOOL SetFlashKey(BOOL enable);
    BOOL GetPageInfo(BYTE& numPages, WORD& pageSize);
    BOOL ReadPage(BYTE page, BYTE buffer[], DWORD size);
    BOOL ReadPage(BYTE page, std::vector<BYTE>& data, DWORD size);
    BOOL WritePage(BYTE page, BYTE buffer[], DWORD size);
    BOOL WritePage(BYTE page, const std::vector<BYTE>& data);
    BOOL WritePage(BYTE page, std::vector<BYTE>::iterator begin, std::vector<BYTE>::iterator end);
    BOOL ResetState(BOOL unlockFlashInterface);
};
