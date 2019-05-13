// MCUProgrammer.h : main header file for the MCUProgrammer application
//

#if !defined(AFX_MCUPROGRAMMER_H__423B8AF6_C6AC_4521_B806_2860A31ED7B4__INCLUDED_)
#define AFX_MCUPROGRAMMER_H__423B8AF6_C6AC_4521_B806_2860A31ED7B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#define EC_JTAG		0x00
#define EC_C2		0x01

#define USB_ADAPTER		0x01
#define SERIAL_ADAPTER	0x02

// Programming Settings Structure
typedef struct PROGRAM_SETTINGS
{
	CString partNumber;
	int PartNumberComboIndex;	// Note: Added to save value when not in settings dialog
	CString debugAdapter;
	BYTE debugAdapterType;
	BYTE ecProtocol;
	CString hexFileNotBanked;
	CString hexFileBank1;
	CString hexFileBank2;
	CString hexFileBank3;
	BOOL eraseCodeSpace;
	BOOL serializeParts;
	UINT startingSerialNumber;
	UINT maxSerialNumber;
	BYTE serialNumberSize;
	int serialNumberSizeComboIndex; // Note: Added to save value when not in settings dialog
	UINT serialNumberIncrement;
	DWORD serialNumberCodeLocation;
	
	BOOL lockCodeMemory;
	DWORD readLockAddress;
	DWORD writeLockAddress;
	BYTE readLockValue;
	BYTE writeLockValue;

	BOOL flashPersist;

	BOOL logToFile;
	CString logFilename;
	BOOL appendToLog;
	UINT currentSerialNumber;
	BOOL unicodeFormat;
	BOOL bigEndianFormat;
	
} PROGRAM_SETTINGS, *PPROGRAM_SETTINGS;

/////////////////////////////////////////////////////////////////////////////
// CMCUProgrammerApp:
// See MCUProgrammer.cpp for the implementation of this class
//

class CMCUProgrammerApp : public CWinApp
{
public:
	CMCUProgrammerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMCUProgrammerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMCUProgrammerApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MCUPPROGRAMMER_H__423B8AF6_C6AC_4521_B806_2860A31ED7B4__INCLUDED_)
