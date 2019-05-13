#if !defined(AFX_MCUPROGRAMMERSETTINGSDLG_H__8F83240C_626E_492F_9CF6_B1FCC4672BCC__INCLUDED_)
#define AFX_MCUPROGRAMMERSETTINGSDLG_H__8F83240C_626E_492F_9CF6_B1FCC4672BCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MCUProgrammerSettingsDlg.h : header file
//

#define FIRST_C2_DEVICE_LISTED	53

enum WhichFilePath {
	FP_NOT_BANKED,
	FP_BANK1,
	FP_BANK2,
	FP_BANK3,
	FP_LOG
};

/////////////////////////////////////////////////////////////////////////////
// CMCUProgrammerSettingsDlg dialog

class CMCUProgrammerSettingsDlg : public CDialog
{
// Construction
public:
	CMCUProgrammerSettingsDlg(PPROGRAM_SETTINGS pProgramSettings, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMCUProgrammerSettingsDlg)
	enum { IDD = IDD_PROGRAMMINGSETTINGS_DIALOG };
	CComboBox	m_ComboSerialNumberSize;
	CComboBox	m_ComboPartNumber;
	CComboBox	m_ComboDebugAdapter;
	BOOL	m_EraseCodeSpace;
	BOOL	m_FlashPersist;
	BOOL	m_SerializeParts;
	BOOL	m_UnicodeFormat;
	CString m_HexFileNotBanked;
	CString	m_HexFileBank1;
	CString m_HexFileBank2;
	CString m_HexFileBank3;
	UINT	m_MaxSerialNumber;
	UINT	m_NumberPartsInRange;
	UINT	m_SerialNumberCodeLocation;
	UINT	m_SerialNumberIncrement;
	UINT	m_StartingSerialNumber;
	UINT	m_CurrentSerialNumber;
	BOOL	m_LogToFile;
	CString	m_LogFile;
	BOOL	m_AppendToLog;
	BOOL	m_LockCodeSpace;
	UINT	m_wLockByteVal;
	UINT	m_rLockByteVal;
	BOOL	m_bBigEndianFormat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMCUProgrammerSettingsDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	static void SaveSettingsFile(CString filename, PROGRAM_SETTINGS settings);
	static void LoadSettingsFile(CString filename, PROGRAM_SETTINGS& settings);

	static UINT GetMaxSerialNumber(UINT serialNumberSize, BOOL unicodeFormat);
	static UINT GetNumberPartsInRange(UINT serialNumberIncrement, UINT startingSerialNumber, UINT maxSerialNumber);

protected:

	PPROGRAM_SETTINGS m_pProgramSettings;

	DWORD m_ConnectedDevices;

	UINT m_wLockByteAddr;
	UINT m_rLockByteAddr;

	BOOL GetLockByteAddresses(CString partNum, UINT& wLockAddr, UINT& rLockAddr);
	BOOL GetNumCodeBanks(CString partNum, UINT& numBanks);
	void UpdateLockByteCtrls();
	void UpdateHexFileCtrls();

	void CopySettingsToDlg(PROGRAM_SETTINGS settings);
	void CopySettingsFromDlg(PROGRAM_SETTINGS& settings);

	void BrowseForHexFile(enum WhichFilePath which);

	// Generated message map functions
	//{{AFX_MSG(CMCUProgrammerSettingsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeSerial();
	afx_msg void OnButtonBrowseNotBanked();
	afx_msg void OnButtonBrowseBank1();
	afx_msg void OnButtonBrowseBank2();
	afx_msg void OnButtonBrowseBank3();
	afx_msg void OnButtonBrowseLog();
	afx_msg void OnCheckLogtofile();
	afx_msg void OnButtonLoadsettings();
	afx_msg void OnButtonSavesettings();
	afx_msg void OnCheckLockCodeSpace();
	afx_msg void OnSelchangeComboPartnumber();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MCUPROGRAMMERSETTINGSDLG_H__8F83240C_626E_492F_9CF6_B1FCC4672BCC__INCLUDED_)
