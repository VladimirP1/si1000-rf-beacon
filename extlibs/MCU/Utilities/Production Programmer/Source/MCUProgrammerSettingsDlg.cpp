// MCUProgrammerSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MCUProgrammer.h"
#include "MCUProgrammerSettingsDlg.h"
#include "SiUtilExports.h"
#include "PartList.h"
#include "CustomDDX.h"
#include "Version.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMCUProgrammerSettingsDlg dialog
/////////////////////////////////////////////////////////////////////////////

CMCUProgrammerSettingsDlg::CMCUProgrammerSettingsDlg(PPROGRAM_SETTINGS pProgramSettings, CWnd* pParent /*=NULL*/)
	: CDialog(CMCUProgrammerSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMCUProgrammerSettingsDlg)
	m_EraseCodeSpace = FALSE;
	m_FlashPersist = FALSE;
	m_SerializeParts = FALSE;
	m_UnicodeFormat = FALSE;
	m_HexFileNotBanked = _T("");
	m_HexFileBank1 = _T("");
	m_HexFileBank2 = _T("");
	m_HexFileBank3 = _T("");
	m_MaxSerialNumber = 0;
	m_NumberPartsInRange = 0;
	m_SerialNumberCodeLocation = 0x00000;
	m_SerialNumberIncrement = 1;
	m_StartingSerialNumber = 0;
	m_LogToFile = FALSE;
	m_LogFile = _T("MCUProgramLog.txt");
	m_AppendToLog = FALSE;
	m_LockCodeSpace = FALSE;
	m_wLockByteVal = 0x00;
	m_rLockByteVal = 0x00;
	m_wLockByteAddr = -1;
	m_rLockByteAddr = -1;
	m_bBigEndianFormat = TRUE;
	//}}AFX_DATA_INIT

	m_pProgramSettings = pProgramSettings;

}


void CMCUProgrammerSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMCUProgrammerSettingsDlg)
	DDX_Control(pDX, IDC_COMBO_SERIALNUMBERSIZE, m_ComboSerialNumberSize);
	DDX_Control(pDX, IDC_COMBO_PARTNUMBER, m_ComboPartNumber);
	DDX_Control(pDX, IDC_COMBO_DEBUGADAPTER, m_ComboDebugAdapter);
	DDX_Check(pDX, IDC_CHECK_ERASECODESPACE, m_EraseCodeSpace);
	DDX_Check(pDX, IDC_CHECK_FLASHPERSIST, m_FlashPersist);
	DDX_Check(pDX, IDC_CHECK_SERIALIZEPARTS, m_SerializeParts);
	DDX_Check(pDX, IDC_CHECK_UNICODE, m_UnicodeFormat);
	DDX_Text(pDX, IDC_EDIT_HEXFILE_NOT_BANKED, m_HexFileNotBanked);
	DDX_Text(pDX, IDC_EDIT_HEXFILE_BANK1, m_HexFileBank1);
	DDX_Text(pDX, IDC_EDIT_HEXFILE_BANK2, m_HexFileBank2);
	DDX_Text(pDX, IDC_EDIT_HEXFILE_BANK3, m_HexFileBank3);
	DDX_Text(pDX, IDC_EDIT_MAXSERIALNUMBER, m_MaxSerialNumber);
	DDX_Text(pDX, IDC_EDIT_NUMBERPARTSINRANGE, m_NumberPartsInRange);
	DDX_Text(pDX, IDC_EDIT_SERIALNUMBERINCREMENT, m_SerialNumberIncrement);
	DDV_MinMaxUInt(pDX, m_SerialNumberIncrement, 0, 4228250625);
	DDX_Text(pDX, IDC_EDIT_STARTINGSERIALNUMBER, m_StartingSerialNumber);
	DDX_Text(pDX, IDC_EDIT_CURRENTSERIALNUMBER, m_CurrentSerialNumber);
	DDX_Check(pDX, IDC_CHECK_LOGTOFILE, m_LogToFile);
	DDX_Text(pDX, IDC_EDIT_LOGFILE, m_LogFile);
	DDX_Check(pDX, IDC_CHECK_APPENDLOGFILE, m_AppendToLog);
	DDX_Check(pDX, IDC_CHECK_LOCK_CODE_SPACE, m_LockCodeSpace);
	DDX_Check(pDX, IDC_CHECK_ENDIAN, m_bBigEndianFormat);
	//}}AFX_DATA_MAP
	DDX_TextHex(pDX, IDC_EDIT_SERIALNUMBERCODELOCATION, m_SerialNumberCodeLocation, 5);
	DDV_MaxCharsHex(pDX, m_SerialNumberCodeLocation, 5);
	DDV_MinMaxUInt(pDX, m_SerialNumberCodeLocation, 0x00000, 0x1FFFF);
	DDX_TextHex(pDX, IDC_EDIT_WRITE_LOCK_BYTE, m_wLockByteVal, 2);
	DDV_MaxCharsHex(pDX, m_wLockByteVal, 2);
	DDV_MinMaxUInt(pDX, m_wLockByteVal, 0x00, 0xFF);
	DDX_TextHex(pDX, IDC_EDIT_READ_LOCK_BYTE, m_rLockByteVal, 2);
	DDV_MaxCharsHex(pDX, m_rLockByteVal, 2);
	DDV_MinMaxUInt(pDX, m_rLockByteVal, 0x00, 0xFF);
}


BEGIN_MESSAGE_MAP(CMCUProgrammerSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CMCUProgrammerSettingsDlg)
	ON_EN_CHANGE(IDC_EDIT_SERIALNUMBERINCREMENT, OnChangeSerial)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_NOT_BANKED, OnButtonBrowseNotBanked)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_BANK1, OnButtonBrowseBank1)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_BANK2, OnButtonBrowseBank2)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_BANK3, OnButtonBrowseBank3)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_LOG, OnButtonBrowseLog)
	ON_BN_CLICKED(IDC_CHECK_LOGTOFILE, OnCheckLogtofile)
	ON_BN_CLICKED(IDC_BUTTON_LOADSETTINGS, OnButtonLoadsettings)
	ON_BN_CLICKED(IDC_BUTTON_SAVESETTINGS, OnButtonSavesettings)
	ON_BN_CLICKED(IDC_CHECK_LOCK_CODE_SPACE, OnCheckLockCodeSpace)
	ON_CBN_SELCHANGE(IDC_COMBO_PARTNUMBER, OnSelchangeComboPartnumber)
	ON_CBN_SELCHANGE(IDC_COMBO_SERIALNUMBERSIZE, OnChangeSerial)
	ON_EN_CHANGE(IDC_EDIT_STARTINGSERIALNUMBER, OnChangeSerial)
	ON_EN_CHANGE(IDC_EDIT_CURRENTSERIALNUMBER, OnChangeSerial)
	ON_BN_CLICKED(IDC_CHECK_UNICODE, OnChangeSerial)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMCUProgrammerSettingsDlg message handlers

BOOL CMCUProgrammerSettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString deviceSerial;
	DWORD i = 0;
	const char *pString;

	// Initialize the list of part numbers
	for (i = 0; i < PARTLISTSIZE; i++)
	{
		m_ComboPartNumber.AddString(PartList[i].partNumber);
	}

	// List all EC and ToolStick debug adapters found
	if (SUCCEEDED(USBDebugDevices(&m_ConnectedDevices)))
	{
		for (i = 0; i < m_ConnectedDevices; i++)
		{
			if (SUCCEEDED(GetUSBDeviceSN(i, &pString)))
			{
				m_ComboDebugAdapter.AddString(pString);
			}
		}
	}

	// Next list COM ports for a serial debug adapter
	for (i = 1; i <= 255; i++)
	{
		deviceSerial.Format("COM%d", i);
		m_ComboDebugAdapter.AddString(deviceSerial);
	}

	// Initialize the selection to the serial if it is in the list,
	// otherwise the first item
	m_ComboDebugAdapter.SelectString(-1, m_pProgramSettings->debugAdapter);

	if (m_ComboDebugAdapter.GetCurSel() < 0)
	{
		m_ComboDebugAdapter.SetCurSel(0);
	}
	
	// Initialize all other settings from the program settings structure
	m_ComboPartNumber.SelectString(-1, m_pProgramSettings->partNumber);

	m_HexFileNotBanked	= m_pProgramSettings->hexFileNotBanked;
	m_HexFileBank1		= m_pProgramSettings->hexFileBank1;
	m_HexFileBank2		= m_pProgramSettings->hexFileBank2;
	m_HexFileBank3		= m_pProgramSettings->hexFileBank3;
	m_SerializeParts	= m_pProgramSettings->serializeParts;
	m_UnicodeFormat		= m_pProgramSettings->unicodeFormat;
	m_bBigEndianFormat	= m_pProgramSettings->bigEndianFormat;
	m_EraseCodeSpace	= m_pProgramSettings->eraseCodeSpace;
	m_FlashPersist		= m_pProgramSettings->flashPersist;
	
	m_ComboSerialNumberSize.SetCurSel(m_pProgramSettings->serialNumberSize - 1);

	m_StartingSerialNumber		= m_pProgramSettings->startingSerialNumber;
	m_SerialNumberIncrement		= m_pProgramSettings->serialNumberIncrement;
	m_SerialNumberCodeLocation	= m_pProgramSettings->serialNumberCodeLocation;
	m_LogToFile					= m_pProgramSettings->logToFile;
	m_AppendToLog				= m_pProgramSettings->appendToLog;
	m_LogFile					= m_pProgramSettings->logFilename;
	m_LockCodeSpace				= m_pProgramSettings->lockCodeMemory;
	m_wLockByteVal				= m_pProgramSettings->writeLockValue;
	m_rLockByteVal				= m_pProgramSettings->readLockValue;
	m_CurrentSerialNumber		= m_pProgramSettings->currentSerialNumber;

	// Only enable the append if we are logging, if not do not allow it
	// as a selection
	if (m_LogToFile)
	{
		GetDlgItem(IDC_CHECK_APPENDLOGFILE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_APPENDLOGFILE)->EnableWindow(FALSE);
	}

	UpdateData(FALSE);

	// Update the serial ranges
	OnChangeSerial();

	// Enable/Disable the lock byte controls
	UpdateLockByteCtrls();

	// Update the hex file controls for banking
	UpdateHexFileCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMCUProgrammerSettingsDlg::OnOK() 
{
	if (UpdateData(TRUE))
	{
		// Check that valid settings are selected
		if (m_ComboPartNumber.GetCurSel() == -1)
		{
			MessageBox("Please select a part number", "Error", MB_OK | MB_ICONEXCLAMATION);
		}
		else if (m_HexFileNotBanked.IsEmpty() &&
				 m_HexFileBank1.IsEmpty() &&
				 m_HexFileBank2.IsEmpty() &&
				 m_HexFileBank3.IsEmpty())
		{
			MessageBox("Please enter a valid file name", "Error", MB_OK | MB_ICONEXCLAMATION);
		}
		else if (m_StartingSerialNumber > m_MaxSerialNumber)
		{
			MessageBox("Please enter starting serial number that is in range", "Error", MB_OK | MB_ICONEXCLAMATION);
		}
		else
		{
			// If there are no errors, then set the program settings structure to contain
			// all the program settings, and close the dialog with an OK
			m_ComboPartNumber.GetWindowText(m_pProgramSettings->partNumber);
			m_pProgramSettings->PartNumberComboIndex = m_ComboPartNumber.GetCurSel();

			m_ComboDebugAdapter.GetWindowText(m_pProgramSettings->debugAdapter);
			m_pProgramSettings->debugAdapterType		= ((DWORD)m_ComboDebugAdapter.GetCurSel() >= m_ConnectedDevices) ? SERIAL_ADAPTER : USB_ADAPTER;
			m_pProgramSettings->hexFileNotBanked			= m_HexFileNotBanked;
			m_pProgramSettings->hexFileBank1				= m_HexFileBank1;
			m_pProgramSettings->hexFileBank2				= m_HexFileBank2;
			m_pProgramSettings->hexFileBank3				= m_HexFileBank3;
			m_pProgramSettings->serializeParts				= m_SerializeParts;
			m_pProgramSettings->unicodeFormat				= m_UnicodeFormat;
			m_pProgramSettings->bigEndianFormat				= m_bBigEndianFormat;

			m_pProgramSettings->eraseCodeSpace				= m_EraseCodeSpace;
			m_pProgramSettings->flashPersist				= m_FlashPersist;
			m_pProgramSettings->startingSerialNumber		= m_StartingSerialNumber;
			m_pProgramSettings->currentSerialNumber			= m_CurrentSerialNumber;
			m_pProgramSettings->maxSerialNumber				= m_MaxSerialNumber;
			m_pProgramSettings->serialNumberIncrement		= m_SerialNumberIncrement;
			m_pProgramSettings->serialNumberSize			= m_ComboSerialNumberSize.GetCurSel() + 1;

			m_pProgramSettings->serialNumberSizeComboIndex = m_ComboSerialNumberSize.GetCurSel();


			m_pProgramSettings->serialNumberCodeLocation	= m_SerialNumberCodeLocation;
			m_pProgramSettings->lockCodeMemory				= m_LockCodeSpace;
			m_pProgramSettings->writeLockAddress			= m_wLockByteAddr;
			m_pProgramSettings->writeLockValue				= m_wLockByteVal;
			m_pProgramSettings->readLockAddress				= m_rLockByteAddr;
			m_pProgramSettings->readLockValue				= m_rLockByteVal;

			// The EC protocol is C2 or JTAG depending on the device, currently the cutoff is the C8051F300
			// for JTAG, and everything above it is C2. This is denoted by setting the #define for
			// the index of the first C2 device listed, and is named FIRST_C2_DEVICE_LISTED
			m_pProgramSettings->ecProtocol = (m_ComboPartNumber.GetCurSel() >= FIRST_C2_DEVICE_LISTED) ? EC_C2 : EC_JTAG;
			
			m_pProgramSettings->logToFile	= m_LogToFile;
			m_pProgramSettings->appendToLog	= m_AppendToLog;
			m_pProgramSettings->logFilename	= m_LogFile;
			
			// If we are logging to file, try to open it now and set it up for logging
			if (m_LogToFile)
			{
				CFile file;
				UINT nOpenFlags = CFile::modeWrite | CFile::modeCreate;

				// If we are appending to the log file, then don't
				// clear the existing file
				if (m_AppendToLog)
				{
					nOpenFlags |= CFile::modeNoTruncate;
				}

				// Create/Test the file path
				if (file.Open(m_LogFile, nOpenFlags))
				{
					file.Close();
				}
				// File test failed
				else
				{
					if (MessageBox("Could not open file for logging, do you want to continue with logging disabled?", "Error", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
					{
						m_pProgramSettings->logToFile = m_LogToFile = FALSE;
					}
					else
					{
						// Return but don't close the dialog
						return;
					}
				}
			}

			// Return and close the dialog with IDOK
			CDialog::OnOK();
		}
	}

	// If we encountered any errors, then return
	// but don't close the dialog
}

UINT CMCUProgrammerSettingsDlg::GetMaxSerialNumber(UINT serialNumberSize, BOOL unicodeFormat)
{
	// Reset the max serial number to 0
	UINT maxSerialNumber = 0;

	if(!unicodeFormat)
	{
		// Increase the max serial number by a factor of 256 for each byte that
		// is allocated for it ie, 1 byte is 256, 2 bytes is 256 * 256, etc.
		for (UINT i = 0; i < serialNumberSize; i++)
		{
			maxSerialNumber = (256 * (maxSerialNumber + 1)) - 1;
		}
	}
	else
	{
		if(serialNumberSize == 4)
		{
			maxSerialNumber = 99999999;
		}
		else if(serialNumberSize == 3)
		{
			maxSerialNumber = 999999;
		}
		else if(serialNumberSize == 2)
		{
			maxSerialNumber = 9999;
		}
		else if(serialNumberSize == 1)
		{
			maxSerialNumber = 99;
		}
	}

	return maxSerialNumber;
}

UINT CMCUProgrammerSettingsDlg::GetNumberPartsInRange(UINT serialNumberIncrement, UINT startingSerialNumber, UINT maxSerialNumber)
{
	UINT numberPartsInRange = 0;

	// Determine the number of serial numbers it range if the increment is
	// not 0. If it is, then there is only 1 serial in range, the starting serial number
	if (serialNumberIncrement > 0)
	{
		// Reset the parts in range
		numberPartsInRange = 0;

		// If the starting serial is 0, and the size is 4 bytes with an increment of 1, then display the
		// max UINT value, otherwise the equation overflows it to 0
		// If the starting serial is less than the max serial number then equate the number of parts in range
		// If the starting serial is equal to the max serial, there is only 1 part in range and is a special
		// case since the equation will produce a 0
		if ((startingSerialNumber == 0) && (maxSerialNumber == 0xFFFFFFFF) && (serialNumberIncrement == 1))
		{
			numberPartsInRange = 0xFFFFFFFF;
		}
		else if (startingSerialNumber < maxSerialNumber)
		{
			numberPartsInRange = ((maxSerialNumber - startingSerialNumber) / serialNumberIncrement) + 1;
		}
		else if (startingSerialNumber == maxSerialNumber)
		{
			numberPartsInRange = 1;
		}
	}
	else
	{
		numberPartsInRange = 1;
	}

	return numberPartsInRange;
}

void CMCUProgrammerSettingsDlg::OnChangeSerial() 
{
	UINT i = 0;

	// This function is called when any serial settings are changed
	// and updates the information in the read only boxes

	UpdateData(TRUE);
	
	// Obtain the maximum serial number and display it
	UINT serialNumberSize = m_ComboSerialNumberSize.GetCurSel() + 1;
	
	// Determine the maximum serial number
	m_MaxSerialNumber = GetMaxSerialNumber(serialNumberSize, m_UnicodeFormat);

	// Determine the number of serial numbers it range
	m_NumberPartsInRange = GetNumberPartsInRange(m_SerialNumberIncrement, m_StartingSerialNumber, m_MaxSerialNumber);

	UpdateData(FALSE);
}

void CMCUProgrammerSettingsDlg::CopySettingsToDlg(PROGRAM_SETTINGS settings)
{
	UpdateData(TRUE);

	m_ComboSerialNumberSize.SetCurSel(settings.serialNumberSizeComboIndex);
	m_ComboPartNumber.SetCurSel(settings.PartNumberComboIndex);

	// Select the debug adapter
	for (int i = 0; i < m_ComboDebugAdapter.GetCount(); i++)
	{
		CString text;
		m_ComboDebugAdapter.GetLBText(i, text);

		if (text == settings.debugAdapter)
		{
			m_ComboDebugAdapter.SetCurSel(i);
			break;
		}
	}

	m_EraseCodeSpace			= settings.eraseCodeSpace;
	m_SerializeParts			= settings.serializeParts;
	m_HexFileNotBanked			= settings.hexFileNotBanked;
	m_SerialNumberCodeLocation	= settings.serialNumberCodeLocation;
	m_SerialNumberIncrement		= settings.serialNumberIncrement;
	m_StartingSerialNumber		= settings.startingSerialNumber;
	m_LogToFile					= settings.logToFile;
	m_LogFile					= settings.logFilename;
	m_AppendToLog				= settings.appendToLog;

	m_HexFileBank1				= settings.hexFileBank1;
	m_HexFileBank2				= settings.hexFileBank2;
	m_HexFileBank3				= settings.hexFileBank3;
	m_FlashPersist				= settings.flashPersist;
	m_LockCodeSpace				= settings.lockCodeMemory;
	m_wLockByteVal				= settings.writeLockValue;
	m_rLockByteVal				= settings.readLockValue;

	m_CurrentSerialNumber		= settings.currentSerialNumber;
	m_UnicodeFormat				= settings.unicodeFormat;
	m_bBigEndianFormat			= settings.bigEndianFormat;

	UpdateData(FALSE);
}

void CMCUProgrammerSettingsDlg::CopySettingsFromDlg(PROGRAM_SETTINGS& settings)
{
	UpdateData(TRUE);

	settings.serialNumberSizeComboIndex		= m_ComboSerialNumberSize.GetCurSel();
	settings.PartNumberComboIndex			= m_ComboPartNumber.GetCurSel();

	m_ComboDebugAdapter.GetWindowText(settings.debugAdapter);

	settings.eraseCodeSpace					= m_EraseCodeSpace;
	settings.serializeParts					= m_SerializeParts;
	settings.hexFileNotBanked				= m_HexFileNotBanked;
	settings.serialNumberCodeLocation		= m_SerialNumberCodeLocation;
	settings.serialNumberIncrement			= m_SerialNumberIncrement;
	settings.startingSerialNumber			= m_StartingSerialNumber;
	settings.logToFile						= m_LogToFile;
	settings.logFilename					= m_LogFile;
	settings.appendToLog					= m_AppendToLog;

	settings.hexFileBank1					= m_HexFileBank1;
	settings.hexFileBank2					= m_HexFileBank2;
	settings.hexFileBank3					= m_HexFileBank3;
	settings.flashPersist					= m_FlashPersist;
	settings.lockCodeMemory					= m_LockCodeSpace;
	settings.writeLockValue					= m_wLockByteVal;
	settings.readLockValue					= m_rLockByteVal;

	settings.currentSerialNumber			= m_CurrentSerialNumber;
	settings.unicodeFormat					= m_UnicodeFormat;
	settings.bigEndianFormat				= m_bBigEndianFormat;
}

void CMCUProgrammerSettingsDlg::BrowseForHexFile(enum WhichFilePath which)
{
	UpdateData(TRUE);

	BOOL bOpen;
	CString filter;
	CString defExt;

	if (which == FP_LOG)
	{
		bOpen	= FALSE;
		filter	= "Text Files (*.txt)|*.txt|All Files (*.*)|*.*||";
		defExt	= "";
	}
	else
	{
		bOpen	= TRUE;
		filter	= "Hex Files (*.hex)|*.hex|All Files (*.*)|*.*||";
		defExt	= "txt";
	}

	CFileDialog fileDlg(bOpen, defExt, NULL, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_NOCHANGEDIR, filter);
	
	// Browse for the Hex file
	if (fileDlg.DoModal() == IDOK)
	{
		switch (which)
		{
		case FP_NOT_BANKED:	m_HexFileNotBanked	= fileDlg.GetPathName();		break;
		case FP_BANK1:		m_HexFileBank1		= fileDlg.GetPathName();		break;
		case FP_BANK2:		m_HexFileBank2		= fileDlg.GetPathName();		break;
		case FP_BANK3:		m_HexFileBank3		= fileDlg.GetPathName();		break;
		case FP_LOG:		m_LogFile			= fileDlg.GetPathName();		break;
		}
	}

	UpdateData(FALSE);
}

void CMCUProgrammerSettingsDlg::OnButtonBrowseNotBanked() 
{
	BrowseForHexFile(FP_NOT_BANKED);
}

void CMCUProgrammerSettingsDlg::OnButtonBrowseBank1() 
{
	BrowseForHexFile(FP_BANK1);
}

void CMCUProgrammerSettingsDlg::OnButtonBrowseBank2() 
{
	BrowseForHexFile(FP_BANK2);
}

void CMCUProgrammerSettingsDlg::OnButtonBrowseBank3() 
{
	BrowseForHexFile(FP_BANK3);
}

void CMCUProgrammerSettingsDlg::OnButtonBrowseLog()
{
	BrowseForHexFile(FP_LOG);
}

void CMCUProgrammerSettingsDlg::OnCheckLogtofile() 
{
	UpdateData(TRUE);

	// Only enable the append if we are logging, if not do not allow it
	// as a selection
	if (m_LogToFile)
	{
		GetDlgItem(IDC_CHECK_APPENDLOGFILE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_APPENDLOGFILE)->EnableWindow(FALSE);
	}	
}

void CMCUProgrammerSettingsDlg::OnCheckLockCodeSpace() 
{
	// Update the lock byte edit boxes and captions
	UpdateLockByteCtrls();
}

// Update the lock byte edit boxes and captions
void CMCUProgrammerSettingsDlg::UpdateLockByteCtrls()
{
	BOOL bEnableLockCtrls = FALSE;

// Enable/disable the lock byte controls based on the check status
// of the lock code space checkbox

	bEnableLockCtrls = IsDlgButtonChecked(IDC_CHECK_LOCK_CODE_SPACE);

	// Enable the lock byte controls if the checkbox is enabled
	// otherwise disable the controls
	GetDlgItem(IDC_STATIC_WRITE_LOCK_BYTE)->EnableWindow(bEnableLockCtrls);
	GetDlgItem(IDC_EDIT_WRITE_LOCK_BYTE)->EnableWindow(bEnableLockCtrls);
	GetDlgItem(IDC_STATIC_READ_LOCK_BYTE)->EnableWindow(bEnableLockCtrls);
	GetDlgItem(IDC_EDIT_READ_LOCK_BYTE)->EnableWindow(bEnableLockCtrls);

// Show one or two lock byte edit boxes depending on the selected part
// number

	CString partNum;
	m_ComboPartNumber.GetWindowText(partNum);

	// Get the read/write lock byte addresses
	if (GetLockByteAddresses(partNum, m_wLockByteAddr, m_rLockByteAddr))
	{
		BOOL bTwoLockBytes = TRUE;

		CString textA;
		CString textB;
		
		CString prefix = "Write Lock";

		// If the addresses are the same, then there only needs to be
		// one edit box for the lock byte value
		if (m_wLockByteAddr == m_rLockByteAddr)
		{
			prefix = "R/W Lock";
			bTwoLockBytes = FALSE;
		}

		// If addresses are the same, then show:
		//
		// "R/W Lock
		//  (0x00000):"
		//
		// Otherwise show:
		//
		// "Write Lock
		//  (0x00000):"
		// "Read Lock
		//  (0x00000):"
		//

		textA.Format("%s\n(0x%05X):", prefix, m_wLockByteAddr);
		textB.Format("Read Lock\n(0x%05X):", m_rLockByteAddr);

		SetDlgItemText(IDC_STATIC_WRITE_LOCK_BYTE, textA);
		SetDlgItemText(IDC_STATIC_READ_LOCK_BYTE, textB);

		GetDlgItem(IDC_STATIC_WRITE_LOCK_BYTE)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_WRITE_LOCK_BYTE)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_READ_LOCK_BYTE)->ShowWindow(bTwoLockBytes);
		GetDlgItem(IDC_EDIT_READ_LOCK_BYTE)->ShowWindow(bTwoLockBytes);

		m_LockCodeSpace = TRUE;
	}
	// If an unknown part or no part has been selected
	else
	{
		// Hide all lock byte controls
		GetDlgItem(IDC_STATIC_WRITE_LOCK_BYTE)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_WRITE_LOCK_BYTE)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_READ_LOCK_BYTE)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_READ_LOCK_BYTE)->ShowWindow(FALSE);

		// Uncheck the lock code space checkbox
		CheckDlgButton(IDC_CHECK_LOCK_CODE_SPACE, FALSE);
		m_LockCodeSpace = FALSE;
	}
}

void CMCUProgrammerSettingsDlg::UpdateHexFileCtrls()
{
	UINT		numBanks		= 0;
	BOOL		bEnableBank1	= FALSE;
	BOOL		bEnableBank2	= FALSE;
	BOOL		bEnableBank3	= FALSE;
	CString		partNum;

	// Get selected part number from combo box
	m_ComboPartNumber.GetWindowText(partNum);

	// Check to see if the part supports code banking
	GetNumCodeBanks(partNum, numBanks);

	// Determine which banks should be enabled
	if (numBanks >= 1)	bEnableBank1 = TRUE;
	if (numBanks >= 2)	bEnableBank2 = TRUE;
	if (numBanks >= 3)	bEnableBank3 = TRUE;

	// Enable/disable bank controls depending on the number of banks

	GetDlgItem(IDC_STATIC_BANK1)->EnableWindow(bEnableBank1);
	GetDlgItem(IDC_STATIC_BANK2)->EnableWindow(bEnableBank2);
	GetDlgItem(IDC_STATIC_BANK3)->EnableWindow(bEnableBank3);

	GetDlgItem(IDC_EDIT_HEXFILE_BANK1)->EnableWindow(bEnableBank1);
	GetDlgItem(IDC_EDIT_HEXFILE_BANK2)->EnableWindow(bEnableBank2);
	GetDlgItem(IDC_EDIT_HEXFILE_BANK3)->EnableWindow(bEnableBank3);

	GetDlgItem(IDC_BUTTON_BROWSE_BANK1)->EnableWindow(bEnableBank1);
	GetDlgItem(IDC_BUTTON_BROWSE_BANK2)->EnableWindow(bEnableBank2);
	GetDlgItem(IDC_BUTTON_BROWSE_BANK3)->EnableWindow(bEnableBank3);

	// Clear hex file name(s) for disabled banks
	if (!bEnableBank1)
	{
		// Clear the banked hex filename
		SetDlgItemText(IDC_EDIT_HEXFILE_BANK1, "");
		m_HexFileBank1.Empty();
	}
	if (!bEnableBank2)
	{
		// Clear the banked hex filename
		SetDlgItemText(IDC_EDIT_HEXFILE_BANK2, "");
		m_HexFileBank2.Empty();
	}
	if (!bEnableBank3)
	{
		// Clear the banked hex filename
		SetDlgItemText(IDC_EDIT_HEXFILE_BANK3, "");
		m_HexFileBank3.Empty();
	}
}

BOOL CMCUProgrammerSettingsDlg::GetLockByteAddresses(CString partNum, UINT& wLockAddr, UINT& rLockAddr)
{
	BOOL retVal = FALSE;

	// Search through the part list for a matching part name
	for (int i = 0; i < PARTLISTSIZE; i++)
	{
		if (PartList[i].partNumber == partNum)
		{
			wLockAddr = PartList[i].wLockAddr;
			rLockAddr = PartList[i].rLockAddr;

			retVal = TRUE;
			break;
		}
	}

	return retVal;
}

BOOL CMCUProgrammerSettingsDlg::GetNumCodeBanks(CString partNum, UINT& numBanks)
{
	BOOL retVal = FALSE;

	// Search through the part list for a matching part name
	for (int i = 0; i < PARTLISTSIZE; i++)
	{
		// Found matching part name
		if (PartList[i].partNumber == partNum)
		{
			// Get the number of code banks (excluding common bank)
			numBanks	= PartList[i].numBanks;
			retVal		= TRUE;
			break;
		}
	}

	return retVal;
}

void CMCUProgrammerSettingsDlg::OnButtonLoadsettings() 
{
	CFileDialog fileDlg(TRUE, "pgs", NULL, OFN_NOCHANGEDIR, "Program Settings Files (*.pgs)|*.pgs|All Files(*.*)|*.*||");
	
	// Browse for the Hex file
	if (fileDlg.DoModal() == IDOK)
	{
		PROGRAM_SETTINGS settings;

		// Load settings from file and apply to dialog controls
		LoadSettingsFile(fileDlg.GetPathName(), settings);
		CopySettingsToDlg(settings);

		// Configure the check boxes
		OnCheckLogtofile();

		// Update the serial settings
		OnChangeSerial();

		// Update the lock byte edit boxes and captions
		UpdateLockByteCtrls();

		// Update the hex file controls for banking
		UpdateHexFileCtrls();
	}
}

void CMCUProgrammerSettingsDlg::OnButtonSavesettings() 
{
	CFileDialog fileDlg(FALSE, "pgs", "ProgramSettings.pgs", OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_NOCHANGEDIR, "Program Settings Files (*.pgs)|*.pgs|All Files(*.*)|*.*||");
	
	// Browse for the Hex file
	if (fileDlg.DoModal() == IDOK)
	{
		PROGRAM_SETTINGS settings;

		// Copy settings from dialog and save to file
		CopySettingsFromDlg(settings);
		SaveSettingsFile(fileDlg.GetPathName(), settings);
	}
}

void CMCUProgrammerSettingsDlg::OnSelchangeComboPartnumber() 
{
	// Update the lock byte edit boxes and captions
	UpdateLockByteCtrls();

	// Update the hex file controls for banking
	UpdateHexFileCtrls();
}

void CMCUProgrammerSettingsDlg::SaveSettingsFile(CString filename, PROGRAM_SETTINGS settings)
{
	CFile iniFile;

	if (iniFile.Open(filename, CFile::modeCreate | CFile::modeWrite))
	{
		CArchive ar(&iniFile, CArchive::store);
		try
		{
			ar << settings.serialNumberSizeComboIndex;	
			ar << settings.PartNumberComboIndex;	
			ar << settings.eraseCodeSpace;					// m_EraseCodeSpace;
			ar << settings.serializeParts;					// m_SerializeParts;
			ar << settings.hexFileNotBanked;				// m_HexFileNotBanked;
			ar << (UINT)settings.serialNumberCodeLocation;	// m_SerialNumberCodeLocation;
			ar << settings.serialNumberIncrement;			// m_SerialNumberIncrement;
			ar << settings.startingSerialNumber;			// m_StartingSerialNumber;
			ar << settings.logToFile;						// m_LogToFile;
			ar << settings.logFilename;						// m_LogFile;
			ar << settings.appendToLog;						// m_AppendToLog;

			// New to version 1.2
			ar << (WORD)VERSION_MAJOR;
			ar << (WORD)VERSION_MINOR;
			ar << settings.hexFileBank1;					// m_HexFileBank1;
			ar << settings.hexFileBank2;					// m_HexFileBank2;
			ar << settings.hexFileBank3;					// m_HexFileBank3;
			ar << settings.flashPersist;					// m_FlashPersist;
			ar << settings.lockCodeMemory;					// m_LockCodeSpace;
			ar << (UINT)settings.writeLockValue;			// m_wLockByteVal;
			ar << (UINT)settings.readLockValue;				// m_rLockByteVal;

			// New to version 2.2
			ar << settings.currentSerialNumber;				// m_CurrentSerialNumber;
			ar << settings.unicodeFormat;					// m_UnicodeFormat;
			ar << settings.bigEndianFormat;					// m_bBigEndianFormat

			// New to version 2.40
			ar << settings.debugAdapter;
			ar << settings.debugAdapterType;
		}
		catch(...)
		{
			
		}

		ar.Close();
		iniFile.Close();
	}
}

void CMCUProgrammerSettingsDlg::LoadSettingsFile(CString filename, PROGRAM_SETTINGS& settings)
{
	CFile iniFile;

	if (iniFile.Open(filename, CFile::modeRead))
	{
		CArchive ar(&iniFile, CArchive::load);

		try
		{
			UINT uint;

			// Serialize the configuration in
			WORD version_major;
			WORD version_minor;

			ar >> settings.serialNumberSizeComboIndex;
			ar >> settings.PartNumberComboIndex;
			ar >> settings.eraseCodeSpace;
			ar >> settings.serializeParts;
			ar >> settings.hexFileNotBanked;
			ar >> uint;
			settings.serialNumberCodeLocation = uint;
			ar >> settings.serialNumberIncrement;
			ar >> settings.startingSerialNumber;
			ar >> settings.logToFile;
			ar >> settings.logFilename;
			ar >> settings.appendToLog;

			// Previous version of the save file did not
			// serialize these variables
			//
			// So only serialize if the file isn't
			// empty
			if (!ar.IsBufferEmpty())
			{
				ar >> version_major;
				ar >> version_minor;

				ar >> settings.hexFileBank1;
				ar >> settings.hexFileBank2;
				ar >> settings.hexFileBank3;
				ar >> settings.flashPersist;
				ar >> settings.lockCodeMemory;
				ar >> uint;
				settings.writeLockValue = uint;
				ar >> uint;
				settings.readLockValue = uint;
			}
			// Otherwise use default values
			else
			{
				settings.hexFileBank1.Empty();
				settings.hexFileBank2.Empty();
				settings.hexFileBank3.Empty();
				settings.flashPersist	= FALSE;
				settings.lockCodeMemory	= FALSE;
				settings.writeLockValue	= 0xFF;
				settings.readLockValue	= 0xFF;

			}

			// Even newer version, so only continue if more data available
			if (!ar.IsBufferEmpty())
			{
				ar >> settings.currentSerialNumber;
				ar >> settings.unicodeFormat;
				ar >> settings.bigEndianFormat;
			}
			// Otherwise use default values
			else
			{
				settings.currentSerialNumber	= 0;
				settings.unicodeFormat			= FALSE;
				settings.bigEndianFormat		= TRUE;
			}

			// 2.40
			if (!ar.IsBufferEmpty())
			{
				ar >> settings.debugAdapter;
				ar >> settings.debugAdapterType;
			}
			else
			{
			}

			// Initialize the settings that weren't loaded from the ini file
			settings.ecProtocol			= (settings.PartNumberComboIndex >= FIRST_C2_DEVICE_LISTED) ? EC_C2 : EC_JTAG;
			settings.serialNumberSize	= settings.serialNumberSizeComboIndex + 1;
			settings.maxSerialNumber	= CMCUProgrammerSettingsDlg::GetMaxSerialNumber(settings.serialNumberSize, settings.unicodeFormat);

			// Lookup the part number string and lock addresses based on the part number
			// combo box selection
			if (settings.PartNumberComboIndex >= 0 && settings.PartNumberComboIndex < PARTLISTSIZE)
			{
				settings.partNumber			= PartList[settings.PartNumberComboIndex].partNumber;
				settings.readLockAddress	= PartList[settings.PartNumberComboIndex].rLockAddr;
				settings.writeLockAddress	= PartList[settings.PartNumberComboIndex].wLockAddr;
			}
		}
		catch(...)
		{
			
		}

		ar.Close();
		iniFile.Close();
	}
}
