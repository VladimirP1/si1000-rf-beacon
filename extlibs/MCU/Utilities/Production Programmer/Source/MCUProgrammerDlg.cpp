// MCUProgrammerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MCUProgrammer.h"
#include "MCUProgrammerDlg.h"
#include "MCUProgrammerSettingsDlg.h"
#include "SiUtilExports.h"
#include "LogFunctions.h"
#include "Version.h"
#include "PartList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMCUProgrammerDlg dialog

CMCUProgrammerDlg::CMCUProgrammerDlg(CString filename, CWnd* pParent /*=NULL*/)
	: CDialog(CMCUProgrammerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMCUProgrammerDlg)
	m_EraseCodeSpace = FALSE;
	m_FlashPersist = FALSE;
	m_LockCodeSpace = FALSE;
	m_SerializeParts = FALSE;
	m_UnicodeFormat = FALSE;
	m_CheckSum = _T("");
	m_DebugAdapter = _T("");
	m_HexFileNotBanked = _T("");
	m_HexFileBank1 = _T("");
	m_HexFileBank2 = _T("");
	m_HexFileBank3 = _T("");
	m_PartNumber = _T("");
	m_CurrentSerialNumberString = _T("");
	m_SerialNumberIncrement = _T("");
	m_bBigEndianFormat = TRUE;
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// Initially nothing is programmed, this will get signalled
	// after valid settings have been set by the user
	m_SettingsHaveBeenProgrammed = FALSE;

	// Initialize all settings
	m_ProgramSettings.startingSerialNumber		= 0;
	m_ProgramSettings.currentSerialNumber		= 0;
	m_ProgramSettings.maxSerialNumber			= 0;
	m_ProgramSettings.eraseCodeSpace			= FALSE;
	m_ProgramSettings.flashPersist				= FALSE;
	m_ProgramSettings.serialNumberSize			= 4;
	m_ProgramSettings.serialNumberCodeLocation	= 0x00000;
	m_ProgramSettings.serialNumberIncrement		= 1;
	m_ProgramSettings.serializeParts			= FALSE;
	m_ProgramSettings.debugAdapterType			= 0x00;
	m_ProgramSettings.ecProtocol				= 0x00;
	m_ProgramSettings.logToFile					= FALSE;
	m_ProgramSettings.appendToLog				= TRUE;
	m_ProgramSettings.logFilename				= "MCUProgramLog.txt";
	m_ProgramSettings.lockCodeMemory			= FALSE;
	m_ProgramSettings.writeLockValue			= 0xFF;
	m_ProgramSettings.readLockValue				= 0xFF;
	m_ProgramSettings.unicodeFormat				= FALSE;
	m_ProgramSettings.bigEndianFormat			= TRUE;
	m_fileName									= filename;
}

void CMCUProgrammerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMCUProgrammerDlg)
	DDX_Control(pDX, IDC_RICHEDIT_LOG, m_Log);
	DDX_Check(pDX, IDC_CHECK_ERASECODESPACE, m_EraseCodeSpace);
	DDX_Check(pDX, IDC_CHECK_FLASH_PERSIST, m_FlashPersist);
	DDX_Check(pDX, IDC_CHECK_LOCK_CODE_SPACE, m_LockCodeSpace);
	DDX_Check(pDX, IDC_CHECK_SERIALIZEPARTS, m_SerializeParts);
	DDX_Check(pDX, IDC_CHECK_UNICODE, m_UnicodeFormat);
	DDX_Text(pDX, IDC_EDIT_DEBUGADAPTER, m_DebugAdapter);
	DDX_Text(pDX, IDC_EDIT_HEX_NOT_BANKED, m_HexFileNotBanked);
	DDX_Text(pDX, IDC_EDIT_HEX_BANK1, m_HexFileBank1);
	DDX_Text(pDX, IDC_EDIT_HEX_BANK2, m_HexFileBank2);
	DDX_Text(pDX, IDC_EDIT_HEX_BANK3, m_HexFileBank3);
	DDX_Text(pDX, IDC_EDIT_PARTNUMBER, m_PartNumber);
	DDX_Text(pDX, IDC_EDIT_CURRENTSERIALNUMBER, m_CurrentSerialNumberString);
	DDX_Text(pDX, IDC_EDIT_SERIALNUMBERINCREMENT, m_SerialNumberIncrement);
	DDX_Check(pDX, IDC_CHECK_ENDIAN, m_bBigEndianFormat);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMCUProgrammerDlg, CDialog)
	//{{AFX_MSG_MAP(CMCUProgrammerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_COMMAND(IDM_ABOUTBOX, OnAboutbox)
	ON_COMMAND(ID_PROGRAMMENU_LOADSETTINGS, OnProgrammenuLoadsettings)
	ON_COMMAND(ID_PROGRAMMENU_SAVESETTINGS, OnProgrammenuSavesettings)
	ON_COMMAND(ID_PROGRAMMENU_CONFIGUREPROGRAMMINGINFORMATION, OnProgrammenuConfigureprogramminginformation)
	ON_COMMAND(ID_PROGRAMMENU_EXIT, OnProgrammenuExit)
	ON_BN_CLICKED(IDC_BUTTON_PROGRAMDEVICE, OnButtonProgramdevice)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMCUProgrammerDlg message handlers

BOOL CMCUProgrammerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetCurrentApplicationDirectory();

	SetIcon(m_hIcon, TRUE);			// Set big icon

	if (!m_fileName.IsEmpty())
	{
		CMCUProgrammerSettingsDlg::LoadSettingsFile(m_fileName, m_ProgramSettings);
		m_SettingsHaveBeenProgrammed = TRUE;
	}

	// Update the settings in the display window with our current settings
	// initialized in the constructor
	UpdateWindowProgramSettings();

	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMCUProgrammerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMCUProgrammerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

HCURSOR CMCUProgrammerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMCUProgrammerDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// Resize windows on resize of the application
	if (GetDlgItem(IDC_RICHEDIT_LOG))
	{
		CRect mainWindowRect, controlWindowRect;
		
		GetDlgItem(IDC_RICHEDIT_LOG)->GetWindowRect(&controlWindowRect);			
		GetWindowRect(&mainWindowRect);
		controlWindowRect.right = mainWindowRect.right - LOG_RIGHT_OFFSET;
		controlWindowRect.bottom = mainWindowRect.bottom - LOG_BOTTOM_OFFSET;
		ScreenToClient(&controlWindowRect);
		GetDlgItem(IDC_RICHEDIT_LOG)->MoveWindow(&controlWindowRect);

		GetDlgItem(IDC_STATIC_LOG)->GetWindowRect(&controlWindowRect);
		controlWindowRect.right = mainWindowRect.right - STATIC_RIGHT_OFFSET;
		controlWindowRect.bottom = mainWindowRect.bottom - STATIC_BOTTOM_OFFSET;
		ScreenToClient(&controlWindowRect);
		GetDlgItem(IDC_STATIC_LOG)->MoveWindow(&controlWindowRect);

		GetDlgItem(IDC_BUTTON_PROGRAMDEVICE)->GetWindowRect(&controlWindowRect);
		controlWindowRect.right = mainWindowRect.right - STATIC_RIGHT_OFFSET;
		ScreenToClient(&controlWindowRect);
		GetDlgItem(IDC_BUTTON_PROGRAMDEVICE)->MoveWindow(&controlWindowRect);
	}

	Invalidate();
	UpdateWindow();	
}

void CMCUProgrammerDlg::OnAboutbox() 
{
	// Display the about box
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CMCUProgrammerDlg::OnProgrammenuConfigureprogramminginformation() 
{
	CMCUProgrammerSettingsDlg tspSettingsDlg(&m_ProgramSettings);
	
	// Launch the settings dialog, and on OK fill out the static settings
	// shown on this dialog
	if (tspSettingsDlg.DoModal() == IDOK)
	{
		// If settings have already been programmed determine what to do with
		// serial number settings
		// Check that the serial number is still in range, if so ask if a reset is needed,
		// if it is not in range, it must be set to an "in range" serial
		if (m_SettingsHaveBeenProgrammed && m_ProgramSettings.serializeParts && ((m_CurrentSerialNumber != m_ProgramSettings.startingSerialNumber) && (m_CurrentSerialNumber < m_ProgramSettings.maxSerialNumber)))
		{
			if (MessageBox("Do you want to reset the current serial number to the starting serial number?", "Serialization Settings", MB_YESNO | MB_ICONQUESTION) == IDNO)
			{
				// If the user doesn't want to reset, save a temp starting number that is equal
				// to the current serial for the window update, then restore the old starting serial
				DWORD tempStartingSerialNumber = m_ProgramSettings.startingSerialNumber;
				m_ProgramSettings.startingSerialNumber = m_CurrentSerialNumber;
				
				UpdateWindowProgramSettings();

				m_ProgramSettings.startingSerialNumber = tempStartingSerialNumber;
			}
			else
			{
				// Otherwise just update
				UpdateWindowProgramSettings();
			}
		}
		else
		{
			// If settings have not been programmed, or the serial doesn't
			// need a prompt for updating then just perform the update
			UpdateWindowProgramSettings();
		}

		// Settings have been programmed now if OK was pressed
		m_SettingsHaveBeenProgrammed = TRUE;

		CString startingMessage;
		CTime time;

		time = CTime::GetCurrentTime();

		CString hexMessage;

		if (!m_ProgramSettings.hexFileNotBanked.IsEmpty())
		{
			hexMessage += "  Hex File (Not Banked): ";
			hexMessage += m_ProgramSettings.hexFileNotBanked;
			hexMessage += "\r\n";
		}
		if (!m_ProgramSettings.hexFileBank1.IsEmpty())
		{
			hexMessage += "  Hex File (Common+Bank1): ";
			hexMessage += m_ProgramSettings.hexFileBank1;
			hexMessage += "\r\n";
		}
		if (!m_ProgramSettings.hexFileBank2.IsEmpty())
		{
			hexMessage += "  Hex File (Common+Bank2): ";
			hexMessage += m_ProgramSettings.hexFileBank2;
			hexMessage += "\r\n";
		}
		if (!m_ProgramSettings.hexFileBank3.IsEmpty())
		{
			hexMessage += "  Hex File (Common+Bank3): ";
			hexMessage += m_ProgramSettings.hexFileBank3;
			hexMessage += "\r\n";
		}

		// Display the settings
		startingMessage.Format("Assigned New Programming Session Settings:\r\n\r\n  Debug Adapter: %s\r\n  Part Number: %s\r\n  Erase Code Space: %s\r\n%s  Flash Persistence: %s\r\n  Serialize Parts: %s\r\n  Serial Number Code Location: 0x%05X\r\n  Unicode Format: %s\r\n  Serial Number Size (Bytes): %u\r\n  Starting Serial: %s\r\n  Serial Increment: %s\r\n  Lock Code Space: %s\r\n", m_ProgramSettings.debugAdapter, m_ProgramSettings.partNumber, (m_ProgramSettings.eraseCodeSpace ? "Yes" : "No"), hexMessage, (m_ProgramSettings.flashPersist ? "Yes" : "No"), (m_ProgramSettings.serializeParts ? "Yes" : "No"), m_ProgramSettings.serialNumberCodeLocation, (m_ProgramSettings.unicodeFormat ? "Yes" : "No"), ( m_ProgramSettings.unicodeFormat ? (m_ProgramSettings.serialNumberSize*2) : (m_ProgramSettings.serialNumberSize) ), m_CurrentSerialNumberString, m_SerialNumberIncrement, (m_ProgramSettings.lockCodeMemory ? "Yes" : "No"));
		
		// If lock code space is enabled, then display the lock byte value(s)
		if (m_ProgramSettings.lockCodeMemory)
		{
			CString lockMessage;
			
			if (m_ProgramSettings.writeLockAddress == m_ProgramSettings.readLockAddress)
			{
				lockMessage.Format("  R/W Lock (0x%05X): 0x%02X\r\n", m_ProgramSettings.writeLockAddress, m_ProgramSettings.writeLockValue);
			}
			else
			{
				lockMessage.Format("  Write Lock (0x%05X): 0x%02X\r\n  Read Lock (0x%05X): 0x%02X\r\n", m_ProgramSettings.writeLockAddress, m_ProgramSettings.writeLockValue, m_ProgramSettings.readLockAddress, m_ProgramSettings.readLockValue);
			}

			startingMessage += lockMessage;
		}

		LogMessage(&m_Log, TRUE, startingMessage, m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);

		// Display the session start time and date
		startingMessage.Format("%s - Starting New Programming Session", time.Format("%B %d %Y"));
		LogMessage(&m_Log, TRUE, startingMessage, m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);

		SetCurrentApplicationDirectory();
	}
}

void CMCUProgrammerDlg::UpdateWindowProgramSettings()
{
	// This takes all displayed settings from the program settings
	// structure and displays them in the window
	m_PartNumber		= m_ProgramSettings.partNumber;
	m_DebugAdapter		= m_ProgramSettings.debugAdapter;
	m_HexFileNotBanked	= m_ProgramSettings.hexFileNotBanked;
	m_HexFileBank1		= m_ProgramSettings.hexFileBank1;
	m_HexFileBank2		= m_ProgramSettings.hexFileBank2;
	m_HexFileBank3		= m_ProgramSettings.hexFileBank3;
	m_SerializeParts	= m_ProgramSettings.serializeParts;
	m_UnicodeFormat		= m_ProgramSettings.unicodeFormat;
	m_bBigEndianFormat  = m_ProgramSettings.bigEndianFormat;
	m_EraseCodeSpace	= m_ProgramSettings.eraseCodeSpace;
	m_FlashPersist		= m_ProgramSettings.flashPersist;
	m_LockCodeSpace		= m_ProgramSettings.lockCodeMemory;

	// Determine if serial numbers are used, if not display an N/A in the window
	if (m_ProgramSettings.serializeParts)
	{
//		m_CurrentSerialNumber = m_ProgramSettings.startingSerialNumber;
//		m_CurrentSerialNumberString.Format("%u", m_ProgramSettings.startingSerialNumber);

		m_CurrentSerialNumber = m_ProgramSettings.currentSerialNumber;
		m_CurrentSerialNumberString.Format("%u", m_ProgramSettings.currentSerialNumber);

		m_SerialNumberIncrement.Format("%u", m_ProgramSettings.serialNumberIncrement);
	}
	else
	{
		m_CurrentSerialNumber = m_ProgramSettings.startingSerialNumber;
		m_CurrentSerialNumberString = "N/A";
		m_SerialNumberIncrement = "N/A";
	}

	UpdateData(FALSE);
}

void CMCUProgrammerDlg::OnProgrammenuExit() 
{
	OnOK();
}

void CMCUProgrammerDlg::OnButtonProgramdevice()
{
	BOOL error = FALSE;
	int comPort = -1;
	CString partNumber;
	CString errorMessage;

	BeginWaitCursor();

	if (ValidateSettings(m_ProgramSettings))
	{
		// If settings have not been programmed, then prompt the user
		if (!m_SettingsHaveBeenProgrammed)
		{
			errorMessage.Format("Settings have not been initialized");
			LogMessage(&m_Log, FALSE, errorMessage, m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);
			error = TRUE;
		}

        if ((m_ProgramSettings.debugAdapterType != SERIAL_ADAPTER) &&
            (m_ProgramSettings.debugAdapterType != USB_ADAPTER))
        {
            errorMessage.Format("Please select a valid Debug Adapter in the ConfigurationSettings");
			LogMessage(&m_Log, FALSE, errorMessage, m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);
			error = TRUE;
        }
		
		// If we are flash erasing the part, then
		// do it before connecting (so that we can erase
		// flash locked JTAG parts)
		if (m_ProgramSettings.eraseCodeSpace)
		{
			if (!error)
			{
				if (m_ProgramSettings.debugAdapterType == SERIAL_ADAPTER)
				{
					// If using a serial adapter, remove the "COM" from the string, and get the
					// port number value
					comPort = atoi(m_DebugAdapter.Right(m_DebugAdapter.GetLength() - 3));

					if (FAILED(FLASHErase(comPort, DISABLE_DIALOGS, m_ProgramSettings.ecProtocol)))
					{
						errorMessage.Format("Cannot Erase Code Space on Debug Adapter: %s", m_DebugAdapter);
						LogMessage(&m_Log, FALSE, errorMessage, m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);
						error = TRUE;
					}
				}
				else if (m_ProgramSettings.debugAdapterType == USB_ADAPTER)
				{
					if (FAILED(FLASHEraseUSB(m_DebugAdapter, DISABLE_DIALOGS, m_ProgramSettings.ecProtocol)))
					{
						errorMessage.Format("Cannot Erase Code Space on Debug Adapter: %s", m_DebugAdapter);
						LogMessage(&m_Log, FALSE, errorMessage, m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);
						error = TRUE;
					}
				}
			}
		}

		// Connect to the EC, log error if it cannot connect
		if (!error)
		{
			if (m_ProgramSettings.debugAdapterType == SERIAL_ADAPTER)
			{
				// If using a serial adapter, remove the "COM" from the string, and get the
				// port number value
				comPort = atoi(m_DebugAdapter.Right(m_DebugAdapter.GetLength() - 3));

				if (FAILED(Connect(comPort, DISABLE_DIALOGS, m_ProgramSettings.ecProtocol, 0)))
				{
					errorMessage.Format("Cannot Connect to Debug Adapter: %s", m_DebugAdapter);
					LogMessage(&m_Log, FALSE, errorMessage, m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);
					error = TRUE;
				}
			}
			else if (m_ProgramSettings.debugAdapterType == USB_ADAPTER)
			{
				if (FAILED(ConnectUSB(m_DebugAdapter, m_ProgramSettings.ecProtocol, 1, DISABLE_DIALOGS)))
				{
					errorMessage.Format("Cannot Connect to Debug Adapter: %s", m_DebugAdapter);
					LogMessage(&m_Log, FALSE, errorMessage, m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);
					error = TRUE;
				}
			}
		}

		// Check that partnum matches, log error if not
		if (!error && Connected())
		{
			// Pointer that will point to the part number string
			const char* pPartNumber;

			// Get the part number
			if (SUCCEEDED(GetDeviceName(&pPartNumber)))
			{
				// Check the part number to see if it matches, if not give an error
				if (m_ProgramSettings.partNumber.Compare(pPartNumber) != 0)
				{
					errorMessage.Format("Connected Device (%s) does not match Specified Device (%s)", partNumber, m_ProgramSettings.partNumber);
					LogMessage(&m_Log, FALSE, errorMessage, m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);
					error = TRUE;
				}
			}
			else
			{
				errorMessage.Format("Cannot obtain Connected Device Part Number");
				LogMessage(&m_Log, FALSE, errorMessage, m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);
				error = TRUE;
			}
		}

		// Download first hex image (not banked)
		// log error if it fails
		if (!m_ProgramSettings.hexFileNotBanked.IsEmpty())
		{
			if (!error && Connected())
			{
				HRESULT result;
				result = Download(const_cast<char*>((const char*)m_ProgramSettings.hexFileNotBanked), 0, DISABLE_DIALOGS, 0, -1, 0, m_ProgramSettings.flashPersist);
				if (FAILED(result))
				{
					errorMessage.Format("Error Downloading Hex File (%s)", m_ProgramSettings.hexFileNotBanked);
					LogMessage(&m_Log, FALSE, errorMessage, m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);
					error = TRUE;
					// Get additional error detail
					CString errMsgDetail;
					errMsgDetail = GetErrorMsg(result);
					LogMessage(&m_Log, FALSE, errMsgDetail, m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);
				}
			}
		}

		// Download second hex image (bank1)
		// log error if it fails
		if (!m_ProgramSettings.hexFileBank1.IsEmpty())
		{
			if (!error && Connected())
			{
				HRESULT result;
				result = Download(const_cast<char*>((const char*)m_ProgramSettings.hexFileBank1), 0, DISABLE_DIALOGS, 0, 1, 0, m_ProgramSettings.flashPersist);
				if (FAILED(result))
				{
					errorMessage.Format("Error Downloading Hex File (%s)", m_ProgramSettings.hexFileBank1);
					LogMessage(&m_Log, FALSE, errorMessage, m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);
					error = TRUE;
					// Get additional error detail
					CString errMsgDetail;
					errMsgDetail = GetErrorMsg(result);
					LogMessage(&m_Log, FALSE, errMsgDetail, m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);

				}
			}
		}

		// Download third hex image (bank2)
		// log error if it fails
		if (!m_ProgramSettings.hexFileBank2.IsEmpty())
		{
			if (!error && Connected())
			{
				HRESULT result;
				result = Download(const_cast<char*>((const char*)m_ProgramSettings.hexFileBank2), 0, DISABLE_DIALOGS, 0, 2, 0, m_ProgramSettings.flashPersist);
				if (FAILED(result))
				{
					errorMessage.Format("Error Downloading Hex File (%s)", m_ProgramSettings.hexFileBank2);
					LogMessage(&m_Log, FALSE, errorMessage, m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);
					error = TRUE;
					// Get additional error detail
					CString errMsgDetail;
					errMsgDetail = GetErrorMsg(result);
					LogMessage(&m_Log, FALSE, errMsgDetail, m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);
				}
			}
		}

		// Download fourth hex image (bank3)
		// log error if it fails
		if (!m_ProgramSettings.hexFileBank3.IsEmpty())
		{
			if (!error && Connected())
			{
				HRESULT result;
				result = Download(const_cast<char*>((const char*)m_ProgramSettings.hexFileBank3), 0, DISABLE_DIALOGS, 0, 3, 0, m_ProgramSettings.flashPersist);
				if (FAILED(result))
				{
					errorMessage.Format("Error Downloading Hex File (%s)", m_ProgramSettings.hexFileBank3);
					LogMessage(&m_Log, FALSE, errorMessage, m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);
					error = TRUE;
					// Get additional error detail
					CString errMsgDetail;
					errMsgDetail = GetErrorMsg(result);
					LogMessage(&m_Log, FALSE, errMsgDetail, m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);
				}
			}
		}

		// Wait for device to finish any last minute routines after the download
		Sleep(500);

		// If we are serializing, write the serial number to the part, then check it
		// and log an error if it doesn't match or fails
		if (m_ProgramSettings.serializeParts)
		{
			if (m_UnicodeFormat)
			{

				int nNumChars = 0;
				int nShift = 0, nSNIndex = 0; 
				char writeUnicodeSN[16] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30};
	
				BYTE writeSerialNumber[16] = {0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00};
				BYTE writeSerialNumberTemp[16] = {0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00};

				_itoa_s(m_CurrentSerialNumber, writeUnicodeSN, 10);

				for(nNumChars = 0; nNumChars < 16; nNumChars++)
				{
					if(writeUnicodeSN[nNumChars] == 0x00)
					{
						break;
					}
				}


				nShift = ((m_ProgramSettings.serialNumberSize*2)-nNumChars);
				nShift*=2;

				if(m_bBigEndianFormat)
				{

					int i = 0, nMaxIndex = 0;
					if(m_ProgramSettings.serialNumberSize == 4)  
					{
						nMaxIndex = 16;
					}
					else if(m_ProgramSettings.serialNumberSize == 3)
					{
						nMaxIndex = 12;
					}
					else if(m_ProgramSettings.serialNumberSize == 2)
					{
						nMaxIndex = 8;
					}
					else
					{
						nMaxIndex = 4;
					}
					
					writeSerialNumberTemp[0]	= writeUnicodeSN[0];
					writeSerialNumberTemp[2]	= writeUnicodeSN[1];
					writeSerialNumberTemp[4]	= writeUnicodeSN[2];
					writeSerialNumberTemp[6]	= writeUnicodeSN[3];
					writeSerialNumberTemp[8]	= writeUnicodeSN[4];
					writeSerialNumberTemp[10]	= writeUnicodeSN[5];
					writeSerialNumberTemp[12]	= writeUnicodeSN[6];
					writeSerialNumberTemp[14]	= writeUnicodeSN[7];

					for(i = nShift; i < 16; i+=2)
					{
						writeSerialNumber[i] = writeSerialNumberTemp[nSNIndex];
						nSNIndex+=2;
					}

				}
				else
				{
					int nSNIndex = nShift, nStartIndex = 16-(nNumChars*2);
					int nEndIndex = nNumChars*2;

					writeSerialNumberTemp[14]	= writeUnicodeSN[0];
					writeSerialNumberTemp[12]	= writeUnicodeSN[1];
					writeSerialNumberTemp[10]	= writeUnicodeSN[2];
					writeSerialNumberTemp[8]	= writeUnicodeSN[3];
					writeSerialNumberTemp[6]	= writeUnicodeSN[4];
					writeSerialNumberTemp[4]	= writeUnicodeSN[5];
					writeSerialNumberTemp[2]	= writeUnicodeSN[6];
					writeSerialNumberTemp[0]	= writeUnicodeSN[7];

					for(int i = 0; i < nEndIndex; i+=2)
					{
						
						writeSerialNumber[i] = writeSerialNumberTemp[nStartIndex];
						nSNIndex += 2;
						nStartIndex+=2;
						
					}
					
				}
				
				// Write the value	
				if (!error && Connected())
				{
					if (FAILED(SetCodeMemory(&(writeSerialNumber[0]), m_ProgramSettings.serialNumberCodeLocation, (m_ProgramSettings.serialNumberSize*4), DISABLE_DIALOGS)))
					{
						errorMessage.Format("Error Setting Serial Number (%u)", m_CurrentSerialNumber);
						LogMessage(&m_Log, FALSE, errorMessage, m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);
						error = TRUE;
					}
				}

				// Read the value back and verify it
				BYTE readSerialNumber[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
				if (!error && Connected())
				{
					if (SUCCEEDED(GetCodeMemory(&(readSerialNumber[0]), m_ProgramSettings.serialNumberCodeLocation, (m_ProgramSettings.serialNumberSize*4))))
					{
						char readUnicodeSN[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
						if(m_bBigEndianFormat)
						{
							readUnicodeSN[0] = readSerialNumber[0];
							readUnicodeSN[1] = readSerialNumber[2];
							readUnicodeSN[2] = readSerialNumber[4];
							readUnicodeSN[3] = readSerialNumber[6];
							readUnicodeSN[4] = readSerialNumber[8];
							readUnicodeSN[5] = readSerialNumber[10];
							readUnicodeSN[6] = readSerialNumber[12];
							readUnicodeSN[7] = readSerialNumber[14];
						}
						else
						{
							readUnicodeSN[0] = readSerialNumber[14];
							readUnicodeSN[1] = readSerialNumber[12];
							readUnicodeSN[2] = readSerialNumber[10];
							readUnicodeSN[3] = readSerialNumber[8];
							readUnicodeSN[4] = readSerialNumber[6];
							readUnicodeSN[5] = readSerialNumber[4];
							readUnicodeSN[6] = readSerialNumber[2];
							readUnicodeSN[7] = readSerialNumber[0];
						}

						for (int nCount = 0; nCount < m_ProgramSettings.serialNumberSize*4; nCount++)
						{
							if (readSerialNumber[nCount] != writeSerialNumber[nCount])
							{
								errorMessage.Format("Serial Number in Device (%s) does not match Current Serial Number (%s)", readUnicodeSN, writeUnicodeSN);
								LogMessage(&m_Log, FALSE, errorMessage, m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);
								error = TRUE;
								break;
							}
						}
					}
					else
					{
						LogMessage(&m_Log, FALSE, "Cannot obtain Serial Number from Device", m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);
						error = TRUE;
					}
				}

			}
			else
			{
				BYTE serialNumber[4] = {0x00, 0x00, 0x00, 0x00};
				UINT nIndex=0;

				if (!error && Connected())
				{
					// Since a DWORD is reverse on PC memory, flip all the bytes before writing to flash
					if(m_bBigEndianFormat)
					{
						serialNumber[0] = (m_CurrentSerialNumber & 0xFF000000) >> 24;
						serialNumber[1] = (m_CurrentSerialNumber & 0x00FF0000) >> 16;
						serialNumber[2] = (m_CurrentSerialNumber & 0x0000FF00) >> 8;
						serialNumber[3] = m_CurrentSerialNumber & 0x000000FF;
						nIndex = 4-m_ProgramSettings.serialNumberSize;
					}
					else
					{
						serialNumber[3] = (m_CurrentSerialNumber & 0xFF000000) >> 24;
						serialNumber[2] = (m_CurrentSerialNumber & 0x00FF0000) >> 16;
						serialNumber[1] = (m_CurrentSerialNumber & 0x0000FF00) >> 8;
						serialNumber[0] = m_CurrentSerialNumber & 0x000000FF;
						nIndex=0;
					}

					if (FAILED(SetCodeMemory(&(serialNumber[nIndex]), m_ProgramSettings.serialNumberCodeLocation, m_ProgramSettings.serialNumberSize, DISABLE_DIALOGS)))
					{
						errorMessage.Format("Error Setting Serial Number (%u)", m_CurrentSerialNumber);
						LogMessage(&m_Log, FALSE, errorMessage, m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);
						error = TRUE;
					}
				}

				if (!error && Connected())
				{
					*(DWORD*)serialNumber = 0x00000000;

					if (SUCCEEDED(GetCodeMemory(&(serialNumber[nIndex]), m_ProgramSettings.serialNumberCodeLocation, m_ProgramSettings.serialNumberSize)))
					{
						// Compare in reverse to the current serial number since bytes will be read in reverse order
						// of what we will see on the PC memory
						if(m_bBigEndianFormat)
						{
							if ((DWORD)((serialNumber[0] << 24) + (serialNumber[1] << 16) + (serialNumber[2] << 8) + serialNumber[3]) != m_CurrentSerialNumber)
							{
								errorMessage.Format("Serial Number in Device (%u) does not match Current Serial Number (%u)", serialNumber, m_CurrentSerialNumber);
								LogMessage(&m_Log, FALSE, errorMessage, m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);
								error = TRUE;
							}
						}
						else
						{
							if ((DWORD)((serialNumber[3] << 24) + (serialNumber[2] << 16) + (serialNumber[1] << 8) + serialNumber[0]) != m_CurrentSerialNumber)
							{
								errorMessage.Format("Serial Number in Device (%u) does not match Current Serial Number (%u)", serialNumber, m_CurrentSerialNumber);
								LogMessage(&m_Log, FALSE, errorMessage, m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);
								error = TRUE;
							}
						}
					}
					else
					{
						LogMessage(&m_Log, FALSE, "Cannot obtain Serial Number from Device", m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);
						error = TRUE;
					}
				}
			}
		}

		// If we are locking code space
		if (m_ProgramSettings.lockCodeMemory)
		{
			if (!error && Connected())
			{
				BYTE bytesToWrite;
				BYTE lockByte[2];

				lockByte[0] = m_ProgramSettings.writeLockValue;
				lockByte[1] = m_ProgramSettings.readLockValue;

				// If the lock byte addresses are the same, then we only need to write
				// one byte
				if (m_ProgramSettings.writeLockAddress == m_ProgramSettings.readLockAddress)
				{
					bytesToWrite = 1;
				}
				else
				{
					bytesToWrite = 2;
				}
				
				// Write one or two bytes to the lock byte address
				if (FAILED(SetCodeMemory(lockByte, m_ProgramSettings.writeLockAddress, bytesToWrite, DISABLE_DIALOGS)))
				{
					LogMessage(&m_Log, FALSE, "Error setting the lock byte", m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);
					error = TRUE;
				}
			}
		}

        if (!error)
		{
//			SetTargetGo();
		}

		// Disconnect from EC if connected
		if (Connected() && (m_ProgramSettings.debugAdapterType == SERIAL_ADAPTER))
		{
			Disconnect(comPort);
		}
		else if (Connected() && (m_ProgramSettings.debugAdapterType == USB_ADAPTER))
		{
			DisconnectUSB();
		}

		// Log Success and increment Serial Number
		if (!error)
		{
			CString serialString;
			serialString.Format("with Serial %u", m_CurrentSerialNumber);
			errorMessage.Format("Device %sProgrammed%s and Verified %s", (m_ProgramSettings.eraseCodeSpace ? "Erased, " : ""), (m_ProgramSettings.lockCodeMemory ? ", Locked" : ""), (m_ProgramSettings.serializeParts ? serialString : ""));
			LogMessage(&m_Log, TRUE, errorMessage, m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);

			// Increment serial number, log error if overflow
			if (m_ProgramSettings.serializeParts)
			{
				if (m_CurrentSerialNumber != 0xFFFFFFFF)
				{
					m_CurrentSerialNumber += m_ProgramSettings.serialNumberIncrement;
				}
				else
				{
					errorMessage.Format("WARNING: Max Serial Number reached, overflowing Current Serial Number to %u", m_ProgramSettings.startingSerialNumber);
					LogMessage(&m_Log, TRUE, errorMessage, m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);
					m_CurrentSerialNumber = m_ProgramSettings.startingSerialNumber;
				}

				if (m_CurrentSerialNumber > m_ProgramSettings.maxSerialNumber)
				{
					errorMessage.Format("WARNING: Max Serial Number reached, overflowing Current Serial Number to %u", m_ProgramSettings.startingSerialNumber);
					LogMessage(&m_Log, TRUE, errorMessage, m_ProgramSettings.logToFile, m_ProgramSettings.logFilename);
					m_CurrentSerialNumber = m_ProgramSettings.startingSerialNumber;
				}

				m_ProgramSettings.currentSerialNumber = m_CurrentSerialNumber;
				m_CurrentSerialNumberString.Format("%u", m_CurrentSerialNumber);

			}
		}

		UpdateData(FALSE);
	}

	EndWaitCursor();
}

void CMCUProgrammerDlg::OnDestroy() 
{
	if (m_ProgramSettings.serializeParts)
	{
		if (AfxMessageBox( "Save current serialization settings?", MB_YESNO|MB_ICONQUESTION) == IDYES)
		{
			PromptSaveFile();
		}
	}

	CDialog::OnDestroy();
}

void CMCUProgrammerDlg::SetCurrentApplicationDirectory()
{
	char szAppPath[MAX_PATH] = "";
	CString strAppDirectory;

	GetCurrentDirectory(MAX_PATH, szAppPath);
	GetModuleFileName(0, szAppPath, sizeof(szAppPath) - 1);

	// Extract directory
	strAppDirectory = szAppPath;
	strAppDirectory = strAppDirectory.Left(strAppDirectory.ReverseFind('\\') + 1);

	// Set working directory to the current application directory for the log file
	SetCurrentDirectory(strAppDirectory);
}

void CMCUProgrammerDlg::PromptLoadFile()
{
	// Browse for the .pgs file
	CFileDialog fileDlg(TRUE, "pgs", NULL, OFN_NOCHANGEDIR, "Program Settings Files (*.pgs)|*.pgs|All Files(*.*)|*.*||");

	if (fileDlg.DoModal() == IDOK)
	{
		CMCUProgrammerSettingsDlg::LoadSettingsFile(fileDlg.GetPathName(), m_ProgramSettings);
		m_SettingsHaveBeenProgrammed = TRUE;

		UpdateWindowProgramSettings();
	}
}

void CMCUProgrammerDlg::PromptSaveFile()
{
	// Browse for the .pgs file
	CFileDialog fileDlg(FALSE, "pgs", "ProgramSettings.pgs", OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_NOCHANGEDIR, "Program Settings Files (*.pgs)|*.pgs|All Files(*.*)|*.*||");
	if (fileDlg.DoModal() == IDOK)
	{
		CMCUProgrammerSettingsDlg::SaveSettingsFile(fileDlg.GetPathName(), m_ProgramSettings);
	}
}

BOOL CMCUProgrammerDlg::ValidateSettings(PROGRAM_SETTINGS settings)
{
	BOOL valid = FALSE;

	if (!settings.debugAdapter.IsEmpty())
	{
		// Perform validation similar to CMCUProgrammerSettingsDlg::OnOK()
		if (settings.serialNumberIncrement <= 4228250625 &&
			settings.serialNumberCodeLocation <= 0x1FFFF &&
			settings.PartNumberComboIndex != -1 &&
			(!settings.hexFileNotBanked.IsEmpty() || !settings.hexFileBank1.IsEmpty() || !settings.hexFileBank2.IsEmpty() || !settings.hexFileBank3.IsEmpty()) &&
			settings.startingSerialNumber <= settings.maxSerialNumber &&
			!settings.partNumber.IsEmpty())
		{
			valid = TRUE;
		}
		else
		{
			MessageBox(_T("The program settings are invalid.\n\nPlease open the \"Configure Programming Settings\" dialog to correct program settings."), NULL, MB_ICONWARNING);
		}
	}
	else
	{
		MessageBox(_T("The selected debug adapter is invalid.\n\nPlease open the \"Configure Programming Settings\" dialog to select a valid debug adapter."), NULL, MB_ICONWARNING);
	}

	return valid;
}

void CMCUProgrammerDlg::OnProgrammenuLoadsettings() 
{
	PromptLoadFile();
}

void CMCUProgrammerDlg::OnProgrammenuSavesettings() 
{
	PromptSaveFile();
}
