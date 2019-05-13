///////////////////////////////////////////////////////////////////////////////
// SLAB_USB_InterruptDlg.cpp : implementation file
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SLAB_USB_Interrupt.h"
#include "SLAB_USB_InterruptDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Global Variables
///////////////////////////////////////////////////////////////////////////////

// The Silabs USB Interrupt GUID specified in the WinUSB driver INF
static const GUID USB_INTERRUPT_GUID = { 0x220fa291, 0xb048, 0x418c, { 0xac, 0xa1, 0xe1, 0xfa, 0xd9, 0x9, 0x77, 0x44 } };

///////////////////////////////////////////////////////////////////////////////
// Static Global Functions
///////////////////////////////////////////////////////////////////////////////

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
// CSLAB_USB_InterruptDlg dialog
///////////////////////////////////////////////////////////////////////////////

CSLAB_USB_InterruptDlg::CSLAB_USB_InterruptDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSLAB_USB_InterruptDlg::IDD, pParent)
    , m_pot(FILTER_NUM_SAMPLES)
    , m_temp(FILTER_NUM_SAMPLES)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSLAB_USB_InterruptDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_DEVICE_LIST, m_comboDevList);
}

BEGIN_MESSAGE_MAP(CSLAB_USB_InterruptDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, &CSLAB_USB_InterruptDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CSLAB_USB_InterruptDlg::OnBnClickedCancel)
    ON_WM_CLOSE()
    ON_WM_DEVICECHANGE()
    ON_WM_DESTROY()
    ON_CBN_DROPDOWN(IDC_COMBO_DEVICE_LIST, &CSLAB_USB_InterruptDlg::OnCbnDropdownComboDeviceList)
    ON_CBN_CLOSEUP(IDC_COMBO_DEVICE_LIST, &CSLAB_USB_InterruptDlg::OnCbnCloseupComboDeviceList)
    ON_BN_CLICKED(IDC_CHECK_OPEN, &CSLAB_USB_InterruptDlg::OnBnClickedCheckOpen)
    ON_BN_CLICKED(IDC_CHECK_LED1, &CSLAB_USB_InterruptDlg::OnBnClickedCheckOutControl)
    ON_BN_CLICKED(IDC_CHECK_LED2, &CSLAB_USB_InterruptDlg::OnBnClickedCheckOutControl)
    ON_BN_CLICKED(IDC_CHECK_P10, &CSLAB_USB_InterruptDlg::OnBnClickedCheckOutControl)
    ON_BN_CLICKED(IDC_CHECK_P11, &CSLAB_USB_InterruptDlg::OnBnClickedCheckOutControl)
    ON_BN_CLICKED(IDC_CHECK_P12, &CSLAB_USB_InterruptDlg::OnBnClickedCheckOutControl)
    ON_BN_CLICKED(IDC_CHECK_P13, &CSLAB_USB_InterruptDlg::OnBnClickedCheckOutControl)
    ON_WM_TIMER()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CSLAB_USB_InterruptDlg message handlers
///////////////////////////////////////////////////////////////////////////////

BOOL CSLAB_USB_InterruptDlg::OnInitDialog()
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

void CSLAB_USB_InterruptDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSLAB_USB_InterruptDlg::OnPaint()
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
HCURSOR CSLAB_USB_InterruptDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Enter won't close the dialog
void CSLAB_USB_InterruptDlg::OnBnClickedOk()
{
}

// Escape won't close the dialog
void CSLAB_USB_InterruptDlg::OnBnClickedCancel()
{
}

// Close should still close the dialog.
//
// The default CDialog::OnClose() method calls OnCancel()
// which in our case won't do anything.
void CSLAB_USB_InterruptDlg::OnClose()
{
   CDialogEx::OnOK();
}

void CSLAB_USB_InterruptDlg::OnDestroy()
{
    CDialogEx::OnDestroy();

    // Close the WinUSB handle if open
    if (m_winusb.IsOpened())
    {
        Close();
    }

	// Stop receiving WM_DEVICECHANGE messages
	UnregisterDeviceChange();
}

// Update the device list whenever the user drops down or closes
// the device combo box
void CSLAB_USB_InterruptDlg::OnCbnDropdownComboDeviceList()
{
    UpdateDeviceList();
}

// Update the device list whenever the user drops down or closes
// the device combo box
void CSLAB_USB_InterruptDlg::OnCbnCloseupComboDeviceList()
{
    UpdateDeviceList();
}

void CSLAB_USB_InterruptDlg::InitializeDialog()
{
    ((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_POT))->SetRange(0, 255);
    ((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_TEMP))->SetRange(0, 255);

    CWinUsbDevice::SetGuid(USB_INTERRUPT_GUID);

    // Register to receive WM_DEVICECHANGE messages
    // for devices with the specified GUID
	RegisterDeviceChange(USB_INTERRUPT_GUID);

	UpdateDeviceList();
}

void CSLAB_USB_InterruptDlg::UpdateDeviceList()
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
CString CSLAB_USB_InterruptDlg::GetSelectedDevicePath()
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
void CSLAB_USB_InterruptDlg::OnBnClickedCheckOpen()
{
    if (m_winusb.IsOpened())
    {
        Close();
    }
    else
    {
        Open();
    }
}

// Open a device using WinUSB
BOOL CSLAB_USB_InterruptDlg::Open()
{
    BOOL success = FALSE;

	CString sPath = GetSelectedDevicePath();

	if (!sPath.IsEmpty())
	{
        success = m_winusb.OpenByDevicePath(sPath);

        if (success)
        {
            if (Configure())
            {
                OnOpened();
            }
            else
            {
                MessageBox(_T("Failed to configure pipe policies"), _T("Error"), MB_ICONERROR);
            }
        }
        else
        {
            MessageBox(_T("Failed to open device"), _T("Error"), MB_ICONERROR);
        }
	}

    return success;
}

// Configure pipe policies for the Interrupt IN/OUT pipes
BOOL CSLAB_USB_InterruptDlg::Configure()
{
    DWORD timeout;

    timeout = 1000;

    // Set Bulk IN transfer timeout to 1000 ms, default is 0 (forever)
    if (!m_winusb.SetPipePolicy(IN_PIPE_ID, PIPE_TRANSFER_TIMEOUT, &timeout, sizeof(timeout)))
        return FALSE;

    // Set Bulk OUT transfer timeout to 1000 ms, default is 0 (forever)
    if (!m_winusb.SetPipePolicy(OUT_PIPE_ID, PIPE_TRANSFER_TIMEOUT, &timeout, sizeof(timeout)))
        return FALSE;

    // Flush cached data in the IN pipe
    if (!m_winusb.FlushPipe(IN_PIPE_ID))
        return FALSE;

    return TRUE;
}

// Close a device using WinUSB
void CSLAB_USB_InterruptDlg::Close()
{
    m_winusb.Close();
    OnClosed();
}

// Called after successfully opening a device to manage
// all of the control states
void CSLAB_USB_InterruptDlg::OnOpened()
{
    // Toggle Open/Close button caption to "Close"
    SetDlgItemText(IDC_CHECK_OPEN, _T("Close"));

    // Check the Open/Close button to indicate that a device
    // is currently open
    CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_OPEN);
    pBtn->SetCheck(TRUE);

    // Disable the device list combo
    GetDlgItem(IDC_COMBO_DEVICE_LIST)->EnableWindow(FALSE);

    // Populate controls with newly opened device information
    UpdateDeviceInfo();

    // Update the device OUT state to match the GUI
    SendOutPacket();

    // Start timer to periodically read IN packets
    StartInTimer();
}

// Called after closing a device to manage
// all of the control states
void CSLAB_USB_InterruptDlg::OnClosed()
{
    // Stop timer to periodically read IN packets
    StopInTimer();

    // Toggle Open/Close button caption to "Open"
    SetDlgItemText(IDC_CHECK_OPEN, _T("Open"));

    // Uncheck the Open/Close button to indicate that a device
    // is not currently opened
    CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_OPEN);
    pBtn->SetCheck(FALSE);

    // Re-enable the device list combo
    GetDlgItem(IDC_COMBO_DEVICE_LIST)->EnableWindow(TRUE);

    // Clear device information
    UpdateDeviceInfo(TRUE);
}

// Start timer to periodically read IN packets
void CSLAB_USB_InterruptDlg::StartInTimer()
{
    SetTimer(IN_TIMER_ID, IN_TIMER_ELAPSE, NULL);
}

// Stop timer to periodically read IN packets
void CSLAB_USB_InterruptDlg::StopInTimer()
{
    KillTimer(IN_TIMER_ID);
}

// Update opened device information
void CSLAB_USB_InterruptDlg::UpdateDeviceInfo(BOOL clear)
{
    if (clear)
    {
        SetDlgItemText(IDC_EDIT_VID, _T(""));
        SetDlgItemText(IDC_EDIT_PID, _T(""));
        SetDlgItemText(IDC_EDIT_MFR, _T(""));
        SetDlgItemText(IDC_EDIT_PRODUCT, _T(""));
    }
    else
    {
        USB_DEVICE_DESCRIPTOR desc;

        // Read the USB device descriptor
        if (m_winusb.GetDeviceDescriptor(&desc))
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
}

// Return a string for the specified USB string index
CString CSLAB_USB_InterruptDlg::GetStringDescriptor(BYTE index)
{
    CString str;

    if (m_winusb.IsOpened())
    {
        BYTE strDesc[MAX_PATH];

        if (index == 0)
        {
            // No string descriptor
            str = _T("");
        }
        else if (m_winusb.GetStringDescriptor(strDesc, index))
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
void CSLAB_USB_InterruptDlg::SetProgress(int nID, int value)
{
    CProgressCtrl* pProg = (CProgressCtrl*)GetDlgItem(nID);

    pProg->SetPos(value);
    pProg->SetPos(value - 1);
    pProg->SetPos(value);
}

// Send an OUT packet every time a checkbox in the Interrupt Out Transfers
// group is checked
void CSLAB_USB_InterruptDlg::OnBnClickedCheckOutControl()
{
    SendOutPacket();
}

void CSLAB_USB_InterruptDlg::OnTimer(UINT_PTR nIDEvent)
{
    // Read IN packets and update the GUI about every 15 ms
    if (nIDEvent == IN_TIMER_ID)
    {
        HandleInPacket();
    }

    CDialogEx::OnTimer(nIDEvent);
}

void CSLAB_USB_InterruptDlg::SendOutPacket()
{
    BYTE packet[OUT_PACKET_SIZE];

    memset(packet, 0x00, OUT_PACKET_SIZE);

    // LEDs
    if (IsDlgButtonChecked(IDC_CHECK_LED1))
        packet[0] = TRUE;
    if (IsDlgButtonChecked(IDC_CHECK_LED2))
        packet[1] = TRUE;

    // P0.4 - P0.7 Write Latch
    if (IsDlgButtonChecked(IDC_CHECK_P04))
        packet[2] |= 0x10;
    if (IsDlgButtonChecked(IDC_CHECK_P05))
        packet[2] |= 0x20;
    if (IsDlgButtonChecked(IDC_CHECK_P06))
        packet[2] |= 0x40;
    if (IsDlgButtonChecked(IDC_CHECK_P07))
        packet[2] |= 0x80;

    DWORD bytesWritten = 0;

    if (!m_winusb.WritePipe(OUT_PIPE_ID, packet, OUT_PACKET_SIZE, &bytesWritten))
    {
        MessageBox(_T("Failed to send OUT packet"), _T("Error"), MB_ICONERROR);
    }
}

void CSLAB_USB_InterruptDlg::HandleInPacket()
{
    BYTE packet[IN_PACKET_SIZE];
    DWORD bytesRead = 0;

    // Request a single IN packet.  No other IN tokens are sent between calls to
    // HandleInPacket().
    BOOL success = m_winusb.ReadPipe(IN_PIPE_ID, packet, sizeof(packet), &bytesRead);

    if (success)
    {
        if (bytesRead == IN_PACKET_SIZE)
        {
            CString text;

            // Switches
            CheckDlgButton(IDC_CHECK_SW1, packet[0] != 0);
            CheckDlgButton(IDC_CHECK_SW2, packet[1] != 0);

            // P0.0 - P0.3 Read Latch
            CheckDlgButton(IDC_CHECK_P00, (packet[2] & 0x01) != 0);
            CheckDlgButton(IDC_CHECK_P01, (packet[2] & 0x02) != 0);
            CheckDlgButton(IDC_CHECK_P02, (packet[2] & 0x04) != 0);
            CheckDlgButton(IDC_CHECK_P03, (packet[2] & 0x08) != 0);

            // Potentiometer

            // Filter the potentiometer values using
            // a 20 sample moving average filter
            // (round up when necessary)
            int pot = (int)(m_pot.Add(packet[3]) + 0.5);

            SetProgress(IDC_PROGRESS_POT, pot);
            text.Format(_T("%d"), pot);
            SetDlgItemText(IDC_STATIC_POT, text);

            // Temperature

            // Filter the temperature values using
            // a 20 sample moving average filter
            // (round up when necessary)
            int temp = (int)(m_temp.Add(packet[4]) + 0.5);

            SetProgress(IDC_PROGRESS_TEMP, temp);
            text.Format(_T("%d"), temp);
            SetDlgItemText(IDC_STATIC_TEMP, text);
        }
        else
        {
            Close();
            MessageBox(_T("Incorrect IN packet size"), _T("Error"), MB_ICONERROR);
        }
    }
    else
    {
        Close();
        MessageBox(_T("Failed to read IN packet"), _T("Error"), MB_ICONERROR);
    }
}

/////////////////////////////////////////////////////////////////////////////
// CSLAB_USB_InterruptDlg - Surprise Removal Protected Methods
/////////////////////////////////////////////////////////////////////////////

// Register for device change notification for USB devices.
// OnDeviceChange() will handle device arrival and removal.
void CSLAB_USB_InterruptDlg::RegisterDeviceChange(GUID guid)
{
	DEV_BROADCAST_DEVICEINTERFACE devIF = {0};

	devIF.dbcc_size			= sizeof(devIF);    
	devIF.dbcc_devicetype	= DBT_DEVTYP_DEVICEINTERFACE;
    devIF.dbcc_classguid    = guid;
	
	m_hNotifyDevNode = RegisterDeviceNotification(GetSafeHwnd(), &devIF, DEVICE_NOTIFY_WINDOW_HANDLE);
}

// Unregister for device change notification for USB devices
void CSLAB_USB_InterruptDlg::UnregisterDeviceChange()
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
BOOL CSLAB_USB_InterruptDlg::OnDeviceChange(UINT nEventType, DWORD_PTR dwData)
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
                opened = m_winusb.IsOpened();

				if (nEventType == DBT_DEVICEREMOVECOMPLETE)
				{
					TCHAR devPath[MAX_PATH];

					// If a device is currently open
					if (opened)
					{
						// Get our device's path
                        if (m_winusb.GetOpenedDevicePath(devPath))
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
