///////////////////////////////////////////////////////////////////////////////
// FileTransferDevice.cpp
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "FileTransferDevice.h"

///////////////////////////////////////////////////////////////////////////////
// CFileTransferDevice Class - Constructor/Destructor
///////////////////////////////////////////////////////////////////////////////

CFileTransferDevice::CFileTransferDevice(void)
{
}

CFileTransferDevice::~CFileTransferDevice(void)
{
}

///////////////////////////////////////////////////////////////////////////////
// CFileTransferDevice Class - Public Methods
///////////////////////////////////////////////////////////////////////////////

// Enable or disable flash writes by sending the proper
// flash key values
//
// Returns TRUE on success
BOOL CFileTransferDevice::SetFlashKey(BOOL enable)
{
    BOOL    success;
    BYTE    command[OUT_PACKET_SIZE];
    BYTE    response[IN_PACKET_SIZE];
    DWORD   written;
    DWORD   read;

    // Build the command packet
    command[0] = CMD_SET_FLASH_KEY; // Command ID

    // Enable flash writes
    if (enable)
    {
        command[1] = 0xa5;  // FLKEY[0]
        command[2] = 0xF1;  // FLKEY[1]
    }
    // Disable flash writes
    else
    {
        command[1] = 0x00;  // FLKEY[0]
        command[2] = 0x00;  // FLKEY[1]
    }

    // Send command packet
    success = this->WritePipe(OUT_PIPE_ID, command, sizeof(command), &written);

    if (success)
    {
        // Receive response packet
        success = this->ReadPipe(IN_PIPE_ID, response, sizeof(response), &read);

        if (!success || read != IN_PACKET_SIZE || response[0] != RSP_SUCCESS)
            success = FALSE;
    }

    return success;
}

// Return flash page information:
// numPages - Number of writeable pages on the device
// pageSize - Size of each flash page in bytes
//
// Returns TRUE on success
BOOL CFileTransferDevice::GetPageInfo(BYTE& numPages, WORD& pageSize)
{
    BOOL    success;
    BYTE    command[OUT_PACKET_SIZE];
    BYTE    response[IN_PACKET_SIZE];
    DWORD   written;
    DWORD   read;

    // Build the command packet
    command[0] = CMD_GET_PAGE_INFO; // Command ID

    // Send command packet
    success = this->WritePipe(OUT_PIPE_ID, command, sizeof(command), &written);

    if (success)
    {
        // Receive response packet
        success = this->ReadPipe(IN_PIPE_ID, response, sizeof(response), &read);

        if (!success ||
            read != sizeof(response) ||
            response[0] != RSP_SUCCESS)
        {
            success = FALSE;
        }
        else
        {
            // Parse response for numPages and pageSize
            numPages = response[1];
            pageSize = MAKEWORD(response[2], response[3]);
        }
    }

    return success;
}

// Read a flash page from the device and return the contents:
// page - specifies the page to read (the first writeable/readable
//        page is 0; page index is relative to the start of the
//        writeable flash region on the device)
// buffer - returns the contents of the specified flash page
// size - specifies the size of the buffer; must be equal to the
//        size of a page as determined by GetPageInfo()
//
// Returns TRUE on success
BOOL CFileTransferDevice::ReadPage(BYTE page, BYTE buffer[], DWORD size)
{
    BOOL    success;
    BYTE    command[OUT_PACKET_SIZE];
    DWORD   responseSize                = size + IN_PACKET_SIZE;
    BYTE*   response                    = new BYTE[responseSize];
    DWORD   written;
    DWORD   read;

    // Build the command packet
    command[0] = CMD_READ_PAGE; // Command ID
    command[1] = page;          // Page number

    // Send command packet
    success = this->WritePipe(OUT_PIPE_ID, command, sizeof(command), &written);

    if (success)
    {
        // Receive read page block packets followed by response packet
        success = this->ReadPipe(IN_PIPE_ID, response, responseSize, &read);

        if (!success ||
            read != responseSize ||
            response[size] != RSP_SUCCESS)
        {
            success = FALSE;
        }
        else
        {
            memcpy(buffer, response, size);
        }
    }

    delete [] response;

    return success;
}

// Convenience method for ReadPage()
// page - specifies the page to read (the first writeable/readable
//        page is 0; page index is relative to the start of the
//        writeable flash region on the device)
// data - returns the contents of the specified flash page
// size - specifies the size of the flash page as determined by GetPageInfo()
BOOL CFileTransferDevice::ReadPage(BYTE page, std::vector<BYTE>& data, DWORD size)
{
    BOOL success;

    if (size > 0)
    {
        // Allocate an array for the data
        BYTE* buffer = new BYTE[size];

        // Read the page
        success = ReadPage(page, buffer, size);

        if (success)
        {
            data.clear();

            // Copy data from array to vector
            for (DWORD i = 0; i < size;i ++)
                data.push_back(buffer[i]);
        }

        delete [] buffer;
    }

    return success;
}

// Write a flash page to the device:
// page - specifies the page to write (the first writeable/readable
//        page is 0; page index is relative to the start of the
//        writeable flash region on the device)
// buffer - specifies the data to write to the specified flash page
// size - specifies the size of the buffer; must be equal to the
//        size of a page as determined by GetPageInfo()
//
// Returns TRUE on success
BOOL CFileTransferDevice::WritePage(BYTE page, BYTE buffer[], DWORD size)
{
    BOOL    success;
    DWORD   commandSize                 = OUT_PACKET_SIZE + size;
    BYTE*   command                     = new BYTE[commandSize];
    BYTE    response[IN_PACKET_SIZE];
    DWORD   written;
    DWORD   read;

    // Build the command packet
    command[0] = CMD_WRITE_PAGE;    // Command ID
    command[1] = page;              // Page number

    // Page Data[size]
    memcpy(&command[OUT_PACKET_SIZE], buffer, size);

    // Send command packet
    success = this->WritePipe(OUT_PIPE_ID, command, commandSize, &written);

    if (success)
    {
        // Receive response packet
        success = this->ReadPipe(IN_PIPE_ID, response, sizeof(response), &read);

        if (!success ||
            read != sizeof(response) ||
            response[0] != RSP_SUCCESS)
        {
            success = FALSE;
        }
    }

    delete [] command;

    return success;
}

// Convenience method for WritePage()
// page - specifies the page to write (the first writeable/readable
//        page is 0; page index is relative to the start of the
//        writeable flash region on the device)
// data - A vector containing the page data to write. Size must be
//        exactly the size of a page as determined by GetPageInfo().
//
// Returns TRUE on sucess
BOOL CFileTransferDevice::WritePage(BYTE page, const std::vector<BYTE>& data)
{
    BOOL success = FALSE;

    if (data.size() > 0)
    {
        // Allocate an array for the data
        BYTE* buffer = new BYTE[data.size()];

        // Copy from the vector to the array
        for (size_t i = 0; i < data.size(); i++)
            buffer[i] = data[i];

        // Write the page
        success = WritePage(page, buffer, data.size());

        delete [] buffer;
    }

    return success;
}

// Convenience method for WritePage()
// page - specifies the page to write (the first writeable/readable
//        page is 0; page index is relative to the start of the
//        writeable flash region on the device)
// begin - An iterator to the beginning of a BYTE vector containing
//         a page to write.
// end - An iterator to the end of a BYTE vector containing a page to write.
//
// The amount of data specified must be exactly the size of a page as
// determined by GetPageInfo().
//
// Returns TRUE on success
BOOL CFileTransferDevice::WritePage(BYTE page, std::vector<BYTE>::iterator begin, std::vector<BYTE>::iterator end)
{
    BOOL success = FALSE;

    ptrdiff_t size = distance(begin, end);

    if (size > 0)
    {
        // Allocate an array for the data
        BYTE* buffer = new BYTE[size];
        int i = 0;

        // Copy page data from vector to array
        for (std::vector<BYTE>::iterator it = begin; it != end; it++, i++)
            buffer[i] = *it;

        // Write the page
        success = WritePage(page, buffer, size);

        delete [] buffer;
    }

    return success;
}

// Reset the device state machine to the IDLE state:
// unlockFlashInterface - If TRUE and the flash write/erase interface is locked
//                        due to an illegal write (ie using incorrect flash key),
//                        then unlock the flash interface by resetting the device
//
// NOTE: The device may re-enumerate to unlock the flash interface. If this occurs,
// then the user must disconnect and re-establish a new handle to the device.
//
// Returns TRUE on success
BOOL CFileTransferDevice::ResetState(BOOL unlockFlashInterface)
{
    BOOL successFlush;
    BOOL successReset;
    WINUSB_SETUP_PACKET setup;

    // bmRequestType: OUT, vendor specific, device request
    setup.RequestType = 0x40;

    // bRequest: Reset State
    setup.Request = REQ_RST_STATE;
    
    // wValue: Unlock flash interface
    if (unlockFlashInterface)
        setup.Value = VALUE_UNLOCK;
    else
        setup.Value = VALUE_DEFAULT;

    // wIndex, wLength unused
    setup.Index = 0x0000;
    setup.Length = 0x0000;

    // Clear unread data in the IN WinUSB buffer
    successFlush = this->FlushPipe(IN_PIPE_ID);

    // Send reset state control request
    successReset = this->ControlTransfer(setup, NULL, NULL);

    return (successFlush && successReset);
}
