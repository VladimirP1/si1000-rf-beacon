///////////////////////////////////////////////////////////////////////////////
// SLAB_USB_BulkDlg.h : header file
///////////////////////////////////////////////////////////////////////////////

#pragma once

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

#include "afxwin.h"
#include <dbt.h>
#include "FileTransferDevice.h"

///////////////////////////////////////////////////////////////////////////////
// CSLAB_USB_BulkDlg dialog
///////////////////////////////////////////////////////////////////////////////

class CSLAB_USB_BulkDlg : public CDialogEx
{
// Construction
public:
	CSLAB_USB_BulkDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SLAB_USB_BULK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

    CComboBox m_comboDevList;

// Implementation
protected:
	HICON                   m_hIcon;
    HDEVNOTIFY	            m_hNotifyDevNode;
    CString                 m_exeDir;
    CFileTransferDevice     m_dev;
    BYTE                    m_numPages;
    WORD                    m_pageSize;
    DWORD                   m_storageSize;

    void InitializeDialog();
    void UpdateDeviceList();
    CString GetSelectedDevicePath();

    void RegisterDeviceChange(GUID guid);
	void UnregisterDeviceChange();

    BOOL Open();
    BOOL Configure();
    void Close();
    void OnOpened();
    void OnClosed();

    void UpdateDeviceInfo(BOOL clear = FALSE);
    void UpdateStorageInfo(BOOL clear = FALSE);
    void UpdateUsedSpace(BOOL clear = FALSE);

    void SendFile();
    void ReceiveFile();

    CString GetStringDescriptor(BYTE index);
    void SetProgress(int nID, int value);

public:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnClose();
    afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD_PTR dwData);
    afx_msg void OnDestroy();
    afx_msg void OnCbnDropdownComboDeviceList();
    afx_msg void OnCbnCloseupComboDeviceList();
    afx_msg void OnBnClickedCheckOpen();
    afx_msg void OnBnClickedButtonBrowseTx();
    afx_msg void OnBnClickedButtonSend();
    afx_msg void OnBnClickedButtonBrowseRx();
    afx_msg void OnBnClickedButtonReceive();
    afx_msg void OnBnClickedButtonResetState();
    afx_msg void OnBnClickedButtonRefreshStorageInfo();
    afx_msg void OnBnClickedButtonRefreshUsedSpace();
    afx_msg void OnDropFiles(HDROP hDropInfo);
};
