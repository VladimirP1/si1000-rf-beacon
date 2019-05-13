///////////////////////////////////////////////////////////////////////////////
// SLAB_USB_InterruptDlg.h : header file
///////////////////////////////////////////////////////////////////////////////

#pragma once

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

#include "afxwin.h"
#include <dbt.h>
#include "WinUsbDevice.h"
#include "MovingAverageFilter.h"

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

#define IN_PACKET_SIZE      8
#define OUT_PACKET_SIZE     8

#define IN_PIPE_ID          0x81
#define OUT_PIPE_ID         0x01

#define IN_TIMER_ID         1
#define IN_TIMER_ELAPSE     15

#define FILTER_NUM_SAMPLES  10

///////////////////////////////////////////////////////////////////////////////
// CSLAB_USB_InterruptDlg dialog
///////////////////////////////////////////////////////////////////////////////

class CSLAB_USB_InterruptDlg : public CDialogEx
{
// Construction
public:
	CSLAB_USB_InterruptDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SLAB_USB_INTERRUPT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

    CComboBox m_comboDevList;

// Implementation
protected:
	HICON                   m_hIcon;
    HDEVNOTIFY	            m_hNotifyDevNode;
    CWinUsbDevice           m_winusb;
    CMovingAverageFilter    m_pot;
    CMovingAverageFilter    m_temp;

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

    void StartInTimer();
    void StopInTimer();

    void UpdateDeviceInfo(BOOL clear = FALSE);
    CString GetStringDescriptor(BYTE index);
    void SetProgress(int nID, int value);

    void SendOutPacket();
    void HandleInPacket();  

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
    afx_msg void OnBnClickedCheckOutControl();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
