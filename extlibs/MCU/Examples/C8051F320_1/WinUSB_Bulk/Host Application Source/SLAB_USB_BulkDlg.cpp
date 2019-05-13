///////////////////////////////////////////////////////////////////////////////
// SLAB_USB_BulkDlg.cpp : implementation file
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// File Transfer File System
///////////////////////////////////////////////////////////////////////////////
//
// The USB Bulk file transfer firmware example supports generic flash page reads
// and writes. This application builds on the firmware protocol to implement a
// very basic file system supporting only a single file.
//
// Offset   Size  Description
// ------   ----  -----------
// 0        1     File Size LSB (D-bytes)
// 1        1     File Size ... (D-bytes)
// 2        1     File Size ... (D-bytes)
// 3        1     File Size MSB (D-bytes)
// 4        1     File Name Length (N-bytes)
// 5        N     File Name (ASCII no null-terminator)
//
// 5+N      D     File Data
//
// 5+N+D    P     Padding (fill page with 0xFF)
//
// The file header section contains the file size in bytes, the file name
// string length, and file name string.
//
// The file data section contains the binary data contents of the file.
//
// The file footer contains any padding that might be necessary to fill the
// last page of the file data section (0 - 511 bytes).
//

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SLAB_USB_Bulk.h"
#include "SLAB_USB_BulkDlg.h"
#include "afxdialogex.h"
#include "FileUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Namespace
///////////////////////////////////////////////////////////////////////////////

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// Global Variables
///////////////////////////////////////////////////////////////////////////////

// The Silabs USB Bulk GUID specified in the WinUSB driver INF
static const GUID USB_BULK_GUID = { 0x7be00ea9, 0xa77d, 0x4b28, { 0xae, 0xdd, 0x89, 0xf9, 0xaa, 0x97, 0x8e, 0xee } };

///////////////////////////////////////////////////////////////////////////////
// Static Global Functions
///////////////////////////////////////////////////////////////////////////////

// Format a decimal number as a string with digit grouping.
static CString Delimit(int value, int digits = 3, TCHAR delimeter = _T(','))
{
    CString text;

    text.Format(_T("%d"), value);

    for (int i = text.GetLength() - digits; i > 0; i -= digits)
    {
        text.Insert(i, delimeter);
    }

    return text;
}

// Convert a USB string descriptor to a CString object
static CString StringDescriptorToCString(BYTE descriptor[MAX_PATH])
{
    CString str;
    int len = 0;

    len = descriptor[0];

    // Check for minimum string descriptor length
    if (len < 2)
        return str;

    // Check for correct descriptor type
    if (descriptor[1] != USB_STRING_DESCRIPTOR_TYPE)
        return str;

    // Convert from string descriptor length in bytes
    // to string character length.
    // Subtract 2 for the bLength and bDescriptorType fields.
    // Divide by 2 because each character is a two byte unicode value.
    len = ((len - 2) / 2);

#ifdef _UNICODE
    str = (WCHAR*)&descriptor[2];
    str = str.Left(len);
#else
    // Convert Unicode to Ascii by dropping the 2nd byte
    // of every 2-byte unicode value
    for (int i = 0; i < len; i++)
    {
        str += descriptor[2 + i * 2];
    }
#endif

    return str;
}

///////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
///////////////////////////////////////////////////////////////////////////////

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CSLAB_USB_BulkDlg dialog
///////////////////////////////////////////////////////////////////////////////

CSLAB_USB_BulkDlg::CSLAB_USB_BulkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSLAB_USB_BulkDlg::IDD, pParent)
    , m_numPages(0)
    , m_pageSize(0)
    , m_storageSize(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSLAB_USB_BulkDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_DEVICE_LIST, m_comboDevList);
}

BEGIN_MESSAGE_MAP(CSLAB_USB_BulkDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, &CSLAB_USB_BulkDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CSLAB_USB_BulkDlg::OnBnClickedCancel)
    ON_WM_CLOSE()
    ON_WM_DEVICECHANGE()
    ON_WM_DESTROY()
    ON_CBN_DROPDOWN(IDC_COMBO_DEVICE_LIST, &CSLAB_USB_BulkDlg::OnCbnDropdownComboDeviceList)
    ON_CBN_CLOSEUP(IDC_COMBO_DEVICE_LIST, &CSLAB_USB_BulkDlg::OnCbnCloseupComboDeviceList)
    ON_BN_CLICKED(IDC_CHECK_OPEN, &CSLAB_USB_BulkDlg::OnBnClickedCheckOpen)
    ON_BN_CLICKED(IDC_BUTTON_BROWSE_TX, &CSLAB_USB_BulkDlg::OnBnClickedButtonBrowseTx)
    ON_BN_CLICKED(IDC_BUTTON_SEND, &CSLAB_USB_BulkDlg::OnBnClickedButtonSend)
    ON_BN_CLICKED(IDC_BUTTON_BROWSE_RX, &CSLAB_USB_BulkDlg::OnBnClickedButtonBrowseRx)
    ON_BN_CLICKED(IDC_BUTTON_RECEIVE, &CSLAB_USB_BulkDlg::OnBnClickedButtonReceive)
    ON_BN_CLICKED(IDC_BUTTON_RESET_STATE, &CSLAB_USB_BulkDlg::OnBnClickedButtonResetState)
    ON_BN_CLICKED(IDC_BUTTON_REFRESH_STORAGE_INFO, &CSLAB_USB_BulkDlg::OnBnClickedButtonRefreshStorageInfo)
    ON_BN_CLICKED(IDC_BUTTON_REFRESH_USED_SPACE, &CSLAB_USB_BulkDlg::OnBnClickedButtonRefreshUsedSpace)
    ON_WM_DROPFILES()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CSLAB_USB_BulkDlg message handlers
///////////////////////////////////////////////////////////////////////////////

BOOL CSLAB_USB_BulkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	InitializeDialog();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSLAB_USB_BulkDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSLAB_USB_BulkDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSLAB_USB_BulkDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Enter won't close the dialog
void CSLAB_USB_BulkDlg::OnBnClickedOk()
{
}

// Escape won't close the dialog
void CSLAB_USB_BulkDlg::OnBnClickedCancel()
{
}

// Close should still close the dialog.
//
// The default CDialog::OnClose() method calls OnCancel()
// which in our case won't do anything.
void CSLAB_USB_BulkDlg::OnClose()
{
   CDialogEx::OnOK();
}

void CSLAB_USB_BulkDlg::OnDestroy()
{
    CDialogEx::OnDestroy();

    // Close the WinUSB handle if open
    if (m_dev.IsOpened())
    {
        Close();
    }

	// Stop receiving WM_DEVICECHANGE messages
	UnregisterDeviceChange();
}

// Update the device list whenever the user drops down or closes
// the device combo box
void CSLAB_USB_BulkDlg::OnCbnDropdownComboDeviceList()
{
    UpdateDeviceList();
}

// Update the device list whenever the user drops down or closes
// the device combo box
void CSLAB_USB_BulkDlg::OnCbnCloseupComboDeviceList()
{
    UpdateDeviceList();
}

void CSLAB_USB_BulkDlg::InitializeDialog()
{
    TCHAR path[MAX_PATH];

    // Set progress control range to 0 - 100 %
    ((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_FILE_SIZE))->SetRange(0, 100);

    // Get the exe file path
    if (GetModuleFileName(NULL, path, MAX_PATH) > 0)
    {
        // Get the exe directory path
        m_exeDir = GetDirectory(path);

        // Set the default receive folder to the application exe folder
        SetDlgItemText(IDC_EDIT_RX_FOLDER_PATH, m_exeDir);
    }

    // Specify the USB Bulk WinUSB GUID
    CWinUsbDevice::SetGuid(USB_BULK_GUID);

    // Register to receive WM_DEVICECHANGE messages
    // for devices with the specified GUID
	RegisterDeviceChange(USB_BULK_GUID);

	UpdateDeviceList();
}

// Update the list of available devices that matches the GUID specified above
void CSLAB_USB_BulkDlg::UpdateDeviceList()
{
	int			sel			= m_comboDevList.GetCurSel();
	CString		selText;
	DWORD		numDevices	= 0;

    // Remember the original device selection
	if (sel != CB_ERR)
	{
		m_comboDevList.GetLBText(sel, selText);
	}

    // Rebuild the device list
	m_comboDevList.ResetContent();

    numDevices = CWinUsbDevice::GetNumDevices();

	for (DWORD i = 0; i < numDevices; i++)
	{
		TCHAR path[MAX_PATH];

        // Add valid device paths to the device list
        if (CWinUsbDevice::GetDevicePath(i, path))
        {
            m_comboDevList.AddString(path);
        }
	}

	sel = 0;

    // Attempt to reselect the originally selected device
	for (int i = 0; i < m_comboDevList.GetCount(); i++)
	{
		CString item;
		
		m_comboDevList.GetLBText(i, item);

		if (item == selText)
		{
			sel = i;
			break;
		}
	}

	m_comboDevList.SetCurSel(sel);
}

// Return the currently selected device path
CString CSLAB_USB_BulkDlg::GetSelectedDevicePath()
{
	CString		path;
	CString		selText;
	int			sel			= m_comboDevList.GetCurSel();
	int			pos			= 0;

	if (sel != CB_ERR)
	{
		m_comboDevList.GetLBText(sel, selText);

		path = selText;
	}

	return path;
}

// Toggle between open and close
void CSLAB_USB_BulkDlg::OnBnClickedCheckOpen()
{
    if (m_dev.IsOpened())
    {
        Close();
    }
    else
    {
        Open();
    }
}

// Open a device using WinUSB
BOOL CSLAB_USB_BulkDlg::Open()
{
    BOOL success = FALSE;

	CString sPath = GetSelectedDevicePath();

	if (!sPath.IsEmpty())
	{
        if (m_dev.OpenByDevicePath(sPath))
        {
            if (Configure())
            {
                // Reset the device state machine to IDLE and possibly re-enumerate
                // to unlock the flash write/erase interface
                if (m_dev.ResetState(TRUE))
                {
                    success = TRUE;
                }
                else
                {
                    MessageBox(_T("Failed to reset device state."), _T("Error"), MB_ICONERROR);
                }
            }
            else
            {
                MessageBox(_T("Failed to configure pipe policies."), _T("Error"), MB_ICONERROR);
            }

            if (!success)
                m_dev.Close();
        }
        else
        {
            MessageBox(_T("Failed to open device."), _T("Error"), MB_ICONERROR);
        }
	}

    if (success)
        OnOpened();

    return success;
}

// Configure pipe policies for the Bulk IN/OUT pipes
BOOL CSLAB_USB_BulkDlg::Configure()
{
    DWORD timeout;

    timeout = 1000;

    // Set Bulk IN transfer timeout to 1000 ms, default is 0 (forever)
    if (!m_dev.SetPipePolicy(IN_PIPE_ID, PIPE_TRANSFER_TIMEOUT, &timeout, sizeof(timeout)))
        return FALSE;

    // Set Bulk OUT transfer timeout to 1000 ms, default is 0 (forever)
    if (!m_dev.SetPipePolicy(OUT_PIPE_ID, PIPE_TRANSFER_TIMEOUT, &timeout, sizeof(timeout)))
        return FALSE;

    // Flush cached data in the IN pipe
    if (!m_dev.FlushPipe(IN_PIPE_ID))
        return FALSE;

    return TRUE;
}

// Close a device using WinUSB
void CSLAB_USB_BulkDlg::Close()
{
    m_dev.Close();
    OnClosed();
}

// Called after successfully opening a device to manage
// all of the control states
void CSLAB_USB_BulkDlg::OnOpened()
{
    // Toggle Open/Close button caption to "Close"
    SetDlgItemText(IDC_CHECK_OPEN, _T("Close"));

    // Check the Open/Close button to indicate that a device
    // is currently open
    CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_OPEN);
    pBtn->SetCheck(TRUE);

    // Disable the device list combo
    // Enable send and receive buttons
    GetDlgItem(IDC_COMBO_DEVICE_LIST)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(TRUE);
    GetDlgItem(IDC_BUTTON_RECEIVE)->EnableWindow(TRUE);

    // Populate controls with newly opened device information
    UpdateDeviceInfo();
    UpdateStorageInfo();
    UpdateUsedSpace();
}

// Called after closing a device to manage
// all of the control states
void CSLAB_USB_BulkDlg::OnClosed()
{
    // Toggle Open/Close button caption to "Open"
    SetDlgItemText(IDC_CHECK_OPEN, _T("Open"));

    // Uncheck the Open/Close button to indicate that a device
    // is not currently opened
    CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_OPEN);
    pBtn->SetCheck(FALSE);

    // Re-enable the device list combo
    // Disable send and receive buttons
    GetDlgItem(IDC_COMBO_DEVICE_LIST)->EnableWindow(TRUE);
    GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_RECEIVE)->EnableWindow(FALSE);

    // Clear device information
    UpdateDeviceInfo(TRUE);
    UpdateStorageInfo(TRUE);
    UpdateUsedSpace(TRUE);
}

// Update opened device information
void CSLAB_USB_BulkDlg::UpdateDeviceInfo(BOOL clear)
{
    USB_DEVICE_DESCRIPTOR desc;

    if (clear)
    {
        SetDlgItemText(IDC_EDIT_VID, _T(""));
        SetDlgItemText(IDC_EDIT_PID, _T(""));
        SetDlgItemText(IDC_EDIT_MFR, _T(""));
        SetDlgItemText(IDC_EDIT_PRODUCT, _T(""));
    }
    // Read the USB device descriptor
    else if (m_dev.GetDeviceDescriptor(&desc))
    {
        CString str;
            
        str.Format(_T("0x%04x"), desc.idVendor);
        SetDlgItemText(IDC_EDIT_VID, str);
            
        str.Format(_T("0x%04x"), desc.idProduct);
        SetDlgItemText(IDC_EDIT_PID, str);
            
        SetDlgItemText(IDC_EDIT_MFR, GetStringDescriptor(desc.iManufacturer));
        SetDlgItemText(IDC_EDIT_PRODUCT, GetStringDescriptor(desc.iProduct));
    }
    else
    {
        SetDlgItemText(IDC_EDIT_VID, _T("Failed to get device descriptor!"));
        SetDlgItemText(IDC_EDIT_PID, _T("Failed to get device descriptor!"));
        SetDlgItemText(IDC_EDIT_MFR, _T("Failed to get device descriptor!"));
        SetDlgItemText(IDC_EDIT_PRODUCT, _T("Failed to get device descriptor!"));
    }
}

// Update opened device storage information
void CSLAB_USB_BulkDlg::UpdateStorageInfo(BOOL clear)
{
    m_numPages = 0;
    m_pageSize = 0;
    m_storageSize = 0;

    if (clear)
    {
        SetDlgItemText(IDC_EDIT_NUM_FLASH_PAGES, _T(""));
        SetDlgItemText(IDC_EDIT_FLASH_PAGE_SIZE, _T(""));
        SetDlgItemText(IDC_EDIT_STORAGE_SIZE, _T(""));
    }
    else if (m_dev.GetPageInfo(m_numPages, m_pageSize))
    {
        CString text;

        m_storageSize = m_numPages * m_pageSize;

        text.Format(_T("%d"), m_numPages);
        SetDlgItemText(IDC_EDIT_NUM_FLASH_PAGES, text);

        text.Format(_T("%d"), m_pageSize);
        SetDlgItemText(IDC_EDIT_FLASH_PAGE_SIZE, text);

        text.Format(_T("%s"), Delimit(m_storageSize));
        SetDlgItemText(IDC_EDIT_STORAGE_SIZE, text);
    }
    else
    {
        SetDlgItemText(IDC_EDIT_NUM_FLASH_PAGES, _T("--"));
        SetDlgItemText(IDC_EDIT_FLASH_PAGE_SIZE, _T("--"));
        SetDlgItemText(IDC_EDIT_STORAGE_SIZE, _T("--"));
    }
}

// Update opened device used space information
void CSLAB_USB_BulkDlg::UpdateUsedSpace(BOOL clear)
{
    if (clear)
    {
        SetDlgItemText(IDC_EDIT_FILE_SIZE, _T(""));
        SetDlgItemText(IDC_EDIT_FILE_NAME, _T(""));
        SetProgress(IDC_PROGRESS_FILE_SIZE, 0);
    }
    else
    {
        DWORD           fileSize;
        BYTE            fileNameLen;
        CString         filename;
        BOOL            success     = FALSE;
        vector<BYTE>    page;

        // Read page 0, which contains the file header:
        // page[0] - FileSizeLsb
        // page[1] - FileSize
        // page[2] - FileSize
        // page[3] - FileSizeMsb
        // page[4] - FileNameLen
        // page[5..5+FileNameLen] - FileName
        // page[5+FileNameLen..5+FileNameLen+FileSize] - FileContents
        if (m_dev.ReadPage(0, page, m_pageSize))
        {
            // Size of the file contents in bytes
            fileSize = ((DWORD)page[0]) | ((DWORD)page[1] << 8) | ((DWORD)page[2] << 16) | ((DWORD)page[3] << 24);

            // Length of the filename in ASCII characters
            fileNameLen = page[4];
                
            // Read the filename
            for (int i = 0; i < fileNameLen; i++)
                filename += page[5 + i];

            DWORD64 totalSize = (DWORD64)5 + fileSize + fileNameLen;

            if (totalSize <= m_storageSize)
            {
                success = TRUE;
            }
        }

        if (success)
        {
            CString text;

            text.Format(_T("%s"), Delimit(fileSize));
            SetDlgItemText(IDC_EDIT_FILE_SIZE, text);

            SetDlgItemText(IDC_EDIT_FILE_NAME, filename);

            int percent = fileSize * 100 / (m_numPages * m_pageSize);
            percent = min(percent, 100);
            SetProgress(IDC_PROGRESS_FILE_SIZE, percent);
        }
        else
        {
            SetDlgItemText(IDC_EDIT_FILE_SIZE, _T("--"));
            SetDlgItemText(IDC_EDIT_FILE_NAME, _T("--"));
            SetProgress(IDC_PROGRESS_FILE_SIZE, 0);
        }
    }
}

// Create a file header and append it to the front of the file data vector
//
// Binary Layout:
// FileSizeLsb - 1 byte
// FileSize    - 1 byte
// FileSize    - 1 byte
// FileSizeMsb - 1 byte
// FileNameLen - 1 byte
// FileName    - <FileNameLen> bytes
// FileContents - <FileSize> bytes
void AppendFileHeader(vector<BYTE>& data, CString filename)
{
    vector<BYTE> header;

    // Construct the file header that will be stored on the device
    // at page 0 just before the actual file data
    header.push_back((BYTE)data.size());
    header.push_back((BYTE)(data.size() >> 8));
    header.push_back((BYTE)(data.size() >> 16));
    header.push_back((BYTE)(data.size() >> 24));
    header.push_back((BYTE)filename.GetLength());

    for (int i = 0; i < filename.GetLength(); i++)
        header.push_back((BYTE)filename[i]);

    // Append the file header to the front of the file contents
    data.insert(data.begin(), header.begin(), header.end());
}

// Remove the file header from data received from the device
//
// Binary Layout:
// FileSizeLsb - 1 byte (Remove)
// FileSize    - 1 byte (Remove)
// FileSize    - 1 byte (Remove)
// FileSizeMsb - 1 byte (Remove)
// FileNameLen - 1 byte (Remove)
// FileName - <FileNameLen> bytes (Remove)
// FileContents - <FileSize> bytes (Keep)
// PagePadding - Pad to page boundary (Remove)
BOOL RemoveFileHeaderPadding(vector<BYTE>& data, WORD pageSize)
{
    BOOL success = FALSE;

    if (data.size() > 5)
    {
        DWORD totalSize = 0;
        DWORD fileSize = ((DWORD)data[0]) | ((DWORD)data[1] << 8) | ((DWORD)data[2] << 16) | ((DWORD)data[3] << 24);
        BYTE fileNameLen = data[4];
        DWORD paddingSize = 0;

        // Total size including file header and contents
        totalSize = 5 + fileNameLen + fileSize;
        
        // Add padding to total size
        if ((totalSize % pageSize) != 0)
        {
            paddingSize = pageSize - (totalSize % pageSize);
            totalSize += paddingSize;
        }

        // Verify vector size
        if (data.size() == totalSize)
        {
            DWORD headerSize = 5 + fileNameLen;

            // Remove file header
            data.erase(data.begin(), data.begin() + headerSize);

            // Remove padding footer
            data.erase(data.end() - paddingSize, data.end());

            success = TRUE;
        }
    }

    return success;
}

// Pad the buffer to fit on a page boundary. Pad with 0xFF
void PadToPageBoundary(vector<BYTE>& data, WORD pageSize)
{
    DWORD padSize = pageSize - (data.size() % pageSize);

    if (padSize < pageSize)
    {
        data.insert(data.end(), padSize, 0xFF);
    }
}

// Transfer a file from the host to the device
void CSLAB_USB_BulkDlg::SendFile()
{
    CWaitCursor     wait;

    CString         msg;
    CString         path;
    CString         filename;
    vector<BYTE>    data;

    // Get file path of file to send
    GetDlgItemText(IDC_EDIT_TX_FILE_PATH, path);

    ////////////////
    // Validation //
    ////////////////

    // Verify filename length
    if (path.GetLength() <= 0)
    {
        MessageBox(_T("Please enter a filename."), _T("Error"), MB_ICONERROR);
        return;
    }

    // Verify file exists
    if (!FileExists(path))
    {
        MessageBox(_T("Please enter a valid filename."), _T("Error"), MB_ICONERROR);
        return;
    }

    // Verify and read file contents
    if (!ReadFile(path, data))
    {
        MessageBox(_T("Failed to read file contents."), _T("Error"), MB_ICONERROR);
        return;
    }

    // Get filename + ext
    filename = GetFnameExt(path);

    // Create a file header and append it to the front of the file data
    AppendFileHeader(data, filename);

    // Pad the data on the last page to fill the entire page
    // Fill with 0xFF
    PadToPageBoundary(data, m_pageSize);

    // Validate required storage size
    if (data.size() > m_storageSize)
    {
        msg.Format(_T("File Size: %s bytes\nMax Size: %s bytes\n\nPlease select a smaller file."), Delimit(data.size()), Delimit(m_storageSize));
        MessageBox(msg, _T("Insufficient Storage"), MB_ICONERROR);
        return;
    }

    ///////////////
    // Send File //
    ///////////////

    // Enable flash writes
    m_dev.SetFlashKey(TRUE);

    // Number of pages to write including file header, file contents, and padding
    BYTE numWritePages = data.size() / m_pageSize;

    // Send each page
    for (int i = 0; i < numWritePages; i++)
    {
        size_t                  pageStart   = i * m_pageSize;
        vector<BYTE>::iterator  pageBegin   = data.begin() + pageStart;
        vector<BYTE>::iterator  pageEnd     = pageBegin + m_pageSize;

        // Write the page to the device
        if (!m_dev.WritePage(i, pageBegin, pageEnd))
        {
            msg.Format(_T("Failed writing page: %d."), i);
            MessageBox(msg, _T("Error"), MB_ICONERROR);
            break;
        }
    }

    // Disable flash writes
    m_dev.SetFlashKey(FALSE);

    ////////////////
    // Update GUI //
    ////////////////

    // Update usage information
    UpdateUsedSpace();
}

// Transfer a file from the device to the host
void CSLAB_USB_BulkDlg::ReceiveFile()
{
    CWaitCursor     wait;

    CString         msg;
    CString         folderPath;
    CString         filename;
    CString         filePath;
    vector<BYTE>    page;
    vector<BYTE>    data;
    DWORD           fileSize;
    BYTE            fileNameLen;
    DWORD64         totalFileSize;

    // Get folder path where the received file will be saved
    GetDlgItemText(IDC_EDIT_RX_FOLDER_PATH, folderPath);

    ////////////////
    // Validation //
    ////////////////

    // Verify folder path length
    if (folderPath.GetLength() <= 0)
    {
        MessageBox(_T("Please enter a folder path."), _T("Error"), MB_ICONERROR);
        return;
    }

    // Verify folder path exists
    if (!DirectoryExists(folderPath))
    {
        MessageBox(_T("Please enter a valid folder path."), _T("Error"), MB_ICONERROR);
        return;
    }

    // Get file information
    if (!m_dev.ReadPage(0, page, m_pageSize))
    {
        MessageBox(_T("Failed to get file header."), _T("Error"), MB_ICONERROR);
        return;
    }

    // Parse the file header
    fileSize        = ((DWORD)page[0]) | ((DWORD)page[1] << 8) | ((DWORD)page[2] << 16) | ((DWORD)page[3] << 24);
    fileNameLen     = page[4];
    totalFileSize   = (DWORD64)5 + fileSize + fileNameLen;
    
    // Read the filename
    for (int i = 0; i < fileNameLen; i++)
        filename += page[5 + i];

    // Verify file header against actual storage size
    if (totalFileSize > m_storageSize)
    {
        MessageBox(_T("Invalid file header."), _T("Error"), MB_ICONERROR);
        return;
    }

    // Build the full file path using the user-selected folder path
    // plus the fname + ext stored on the device
    filePath = MakePath(NULL, folderPath, filename, NULL);

    /////////////////////
    // GUI Interaction //
    /////////////////////

    if (FileExists(filePath))
    {
        if (MessageBox(filename + _T(" already exists. Do you wish to overwrite?"), _T("Overwrite File?"), MB_YESNO | MB_ICONQUESTION) != IDYES)
        {
            return;
        }
    }

    //////////////////
    // Receive File //
    //////////////////

    // Calculate the number of pages to read (always round up)
    BYTE numReadPages = (BYTE)((totalFileSize + (m_pageSize - 1)) / m_pageSize);

    // Read each page and build up a vector of bytes
    for (int i = 0; i < numReadPages; i++)
    {
        if (!m_dev.ReadPage(i, page, m_pageSize))
        {
            msg.Format(_T("Failed reading page: %d."), i);
            MessageBox(msg, _T("Error"), MB_ICONERROR);
            return;
        }

        data.insert(data.end(), page.begin(), page.end());
    }

    // Remove the file header from the data received from the device
    if (!RemoveFileHeaderPadding(data, m_pageSize))
    {
        MessageBox(_T("Failed to process file header."), _T("Error"), MB_ICONERROR);
        return;
    }

    // Write the file to disk
    if (!WriteFile(filePath, data))
    {
        MessageBox(_T("Failed to write file."), _T("Error"), MB_ICONERROR);
        return;
    }
}

// Return a string for the specified USB string index
CString CSLAB_USB_BulkDlg::GetStringDescriptor(BYTE index)
{
    CString str;

    if (m_dev.IsOpened())
    {
        BYTE strDesc[MAX_PATH];

        if (index == 0)
        {
            // No string descriptor
            str = _T("");
        }
        else if (m_dev.GetStringDescriptor(strDesc, index))
        {
            // Convert string descriptor to CString
            str = StringDescriptorToCString(strDesc);
        }
        else
        {
            // Failed to retrieve string descriptor
            str = _T("Failed!");
        }
    }

    return str;
}

// Jump through some hoops to get the progress controls to update quickly:
// - Progress controls move very quickly when changing to a lower value
// - But, progress controls move very slowly when changing to a higher value
// - Speed up increasing progress value by going to the desired value, going
//   down a single position, and then finally back to the desired value
void CSLAB_USB_BulkDlg::SetProgress(int nID, int value)
{
    CProgressCtrl* pProg = (CProgressCtrl*)GetDlgItem(nID);

    pProg->SetPos(value);
    pProg->SetPos(value - 1);
    pProg->SetPos(value);
}

void CSLAB_USB_BulkDlg::OnBnClickedButtonBrowseTx()
{
    CString filter = _T("All Files (*.*)|*.*||");
    CFileDialog dlg(TRUE, 0, 0, OFN_HIDEREADONLY, filter);

    if (dlg.DoModal() == IDOK)
    {
        SetDlgItemText(IDC_EDIT_TX_FILE_PATH, dlg.GetPathName());
    }
}

void CSLAB_USB_BulkDlg::OnBnClickedButtonSend()
{
    SendFile();
}

void CSLAB_USB_BulkDlg::OnBnClickedButtonBrowseRx()
{
    BROWSEINFO bi = {0};
    LPITEMIDLIST pidl = SHBrowseForFolder (&bi);

    if (pidl != 0)
    {
        // Get the name of the folder
        TCHAR path[MAX_PATH];

        if (SHGetPathFromIDList(pidl, path))
        {
            SetDlgItemText(IDC_EDIT_RX_FOLDER_PATH, path);
        }

        // Free memory used
        IMalloc * imalloc = 0;
        if (SUCCEEDED(SHGetMalloc(&imalloc)))
        {
            imalloc->Free(pidl);
            imalloc->Release();
        }
    }
}

void CSLAB_USB_BulkDlg::OnBnClickedButtonReceive()
{
    ReceiveFile();
}

void CSLAB_USB_BulkDlg::OnBnClickedButtonRefreshStorageInfo()
{
    UpdateStorageInfo();
}

void CSLAB_USB_BulkDlg::OnBnClickedButtonRefreshUsedSpace()
{
    UpdateUsedSpace();
}

// Reset the device state machine and unlock the flash interface
// if necessary by resetting the device
void CSLAB_USB_BulkDlg::OnBnClickedButtonResetState()
{
    if (!m_dev.ResetState(TRUE))
    {
        MessageBox(_T("Failed to reset device state."), _T("Error"), MB_ICONERROR);
    }
}

// Handle drag drop of files and directories on the dialog
void CSLAB_USB_BulkDlg::OnDropFiles(HDROP hDropInfo)
{
    UINT numFiles;
    TCHAR filename[MAX_PATH];

    // Get the number of files being dropped
    numFiles = DragQueryFile(hDropInfo, -1, NULL, 0);

    // Only support a single item
    if (numFiles == 1)
    {
        // Get the file path
        if (DragQueryFile(hDropInfo, 0, filename, MAX_PATH) > 0)
        {
            // Path points to a directory
            if (DirectoryExists(filename))
            {
                // Update receive folder path
                SetDlgItemText(IDC_EDIT_RX_FOLDER_PATH, filename);

                // Receive the file
                if (m_dev.IsOpened())
                    ReceiveFile();
            }
            // Path points to a file
            else if (FileExists(filename))
            {
                // Update the send file path
                SetDlgItemText(IDC_EDIT_TX_FILE_PATH, filename);

                // Send the file
                if (m_dev.IsOpened())
                    SendFile();
            }
        }
    }

    CDialogEx::OnDropFiles(hDropInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CSLAB_USB_BulkDlg - Surprise Removal Protected Methods
/////////////////////////////////////////////////////////////////////////////

// Register for device change notification for USB devices.
// OnDeviceChange() will handle device arrival and removal.
void CSLAB_USB_BulkDlg::RegisterDeviceChange(GUID guid)
{
	DEV_BROADCAST_DEVICEINTERFACE devIF = {0};

	devIF.dbcc_size			= sizeof(devIF);    
	devIF.dbcc_devicetype	= DBT_DEVTYP_DEVICEINTERFACE;
    devIF.dbcc_classguid    = guid;
	
	m_hNotifyDevNode = RegisterDeviceNotification(GetSafeHwnd(), &devIF, DEVICE_NOTIFY_WINDOW_HANDLE);
}

// Unregister for device change notification for USB devices
void CSLAB_USB_BulkDlg::UnregisterDeviceChange()
{
	if (m_hNotifyDevNode)
	{
		UnregisterDeviceNotification(m_hNotifyDevNode);
		m_hNotifyDevNode = NULL;
	}
}

// Handle device change messages (ie a device is added or removed)
// - If a device is connected, then add the device to the device list
// - If the device we were connected to was removed, then disconnect from the device
BOOL CSLAB_USB_BulkDlg::OnDeviceChange(UINT nEventType, DWORD_PTR dwData)
{
	// Device has been removed
	if (nEventType == DBT_DEVICEREMOVECOMPLETE ||
		nEventType == DBT_DEVICEARRIVAL)
	{
		if (dwData)
		{
			PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)dwData;

			if (pHdr->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
			{
				PDEV_BROADCAST_DEVICEINTERFACE pDevInf = (PDEV_BROADCAST_DEVICEINTERFACE)pHdr;
				BOOL opened			= FALSE;
				BOOL disconnected	= FALSE;

                // Device path
				CString deviceStr = pDevInf->dbcc_name;

				// Check to see if the device is opened
                opened = m_dev.IsOpened();

				if (nEventType == DBT_DEVICEREMOVECOMPLETE)
				{
					TCHAR devPath[MAX_PATH];

					// If a device is currently open
					if (opened)
					{
						// Get our device's path
                        if (m_dev.GetOpenedDevicePath(devPath))
						{
							// Check to see if our device was removed by comparing device paths
							if (deviceStr.CompareNoCase(CString(devPath)) == 0)
							{
								// Disconnect from it
                                Close();

								disconnected = TRUE;
							}
						}
					}
				}

				// Only update the device list if we aren't connected to a device
				if (disconnected || !opened)
				{
					UpdateDeviceList();
				}
			}
		}
	}

	return TRUE;
}
