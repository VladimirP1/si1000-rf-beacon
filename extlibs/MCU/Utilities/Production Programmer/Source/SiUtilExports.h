// These exports are common to SiUtil.dll, SiDebug.dll, and SlideHWLib.dll

//
// *** Please keep the function declarations in alphabetical order.  ***
//

#define SIUTIL_API(type) extern "C" __declspec(dllexport) type __stdcall 


SIUTIL_API(HRESULT)	Connect(int		nComPort = 1, 
						    int		nDisableDialogBoxes = 0, 
							int		nECprotocol = 0, 
							int		nBaudRateIndex = 0);

SIUTIL_API(BOOL)	Connected();

SIUTIL_API(HRESULT)	ConnectUSB(const char *	sSerialNum, 
							   int			nECprotocol = 0, 
							   int			nPowerTarget = 0, 
							   int			nDisableDialogBoxes = 0);

SIUTIL_API(HRESULT)	Disconnect(int	nComPort = 1);

SIUTIL_API(HRESULT)	DisconnectUSB();

SIUTIL_API(HRESULT)	Download(char *	sDownloadFile, 
							 int	nDeviceErase = 0, 
							 int	nDisableDialogBoxes = 0, 
							 int	nDownloadScratchPadSFLE = 0, 
							 int	nBankSelect = -1, 
							 int	nLockFlash = 0,
							 BOOL	bPersistFlash = TRUE);

SIUTIL_API(HRESULT)	DownloadCS(char *	sDownloadFile, 
							 int	nDeviceErase = 0, 
							 int	nDisableDialogBoxes = 0, 
							 int	nDownloadScratchPadSFLE = 0, 
							 int	nBankSelect = -1, 
							 int	nLockFlash = 0,
							 BOOL	bPersistFlash = TRUE,
							 unsigned short *usChecksum = NULL);

SIUTIL_API(HRESULT)	VerifyFlashDownload(char *	sDownloadFile, 
							 int	nDisableDialogBoxes = 0, 
							 int	nBankSelect = -1);

SIUTIL_API(HRESULT)	FLASHErase(int				nComPort = 1, 
							   int				nDisableDialogBoxes = 0, 
							   int				nECprotocol = 0);

SIUTIL_API(HRESULT)	FLASHEraseUSB(const char *		sSerialNum, 
								  int				nDisableDialogBoxes = 0, 
								  int				nECprotocol = 0);

SIUTIL_API(HRESULT)	GetCodeMemory(BYTE *		ptrMem, 
								  DWORD			wStartAddress, 
								  unsigned int	nLength);

SIUTIL_API(HRESULT)	GetDeviceName(const char **	psDeviceName);

SIUTIL_API(char *)	GetDLLVersion();

SIUTIL_API(char*)	GetErrorMsg(HRESULT	errorCode);

SIUTIL_API(HRESULT)	GetRAMMemory(BYTE *			ptrMem, 
								 DWORD			wStartAddress, 
								 unsigned int	nLength);

SIUTIL_API(int)		GetSAFirmwareVersion();

SIUTIL_API(HRESULT)	GetScratchPadMemory(BYTE *			ptrMem, 
										DWORD			wStartAddress, 
										unsigned int	nLength);

SIUTIL_API(HRESULT)	GetUSBDevice(const char **	psSerialNum);  // Note: Not published

SIUTIL_API(HRESULT)	GetUSBDeviceSN(DWORD			dwDeviceNum, 
								   const char **	psSerialNum);

SIUTIL_API(HRESULT)	GetUSBDLLVersion(const char **	pVersionString);

SIUTIL_API(int)		GetUSBFirmwareVersion();

SIUTIL_API(HRESULT)	GetXRAMMemory(BYTE *		ptrMem, 
								  DWORD			wStartAddress, 
								  unsigned int	nLength);

SIUTIL_API(HRESULT)	ISupportBanking(int *	nSupportedBanks);

SIUTIL_API(HRESULT)	SetAdapter(BYTE		bAdapter);

SIUTIL_API(void)	SetAppName(char *	appName);

SIUTIL_API(HRESULT)	SetCodeMemory(BYTE *		ptrMem, 
								  DWORD			wStartAddress, 
								  unsigned int	nLength,
								  int			nDisableDialogs=0);

SIUTIL_API(HRESULT)	SetJTAGDeviceAndConnect(int		nComPort = 1, 
										    int		nDisableDialogBoxes = 0, 
										    BYTE	DevicesBeforeTarget = 0, 
										    BYTE	DevicesAfterTarget = 0,
										    WORD	IRBitsBeforeTarget = 0, 
										    WORD	IRBitsAfterTarget = 0);

SIUTIL_API(HRESULT)	SetJTAGDeviceAndConnectUSB(const char *	SerialNum, 
											  int			nPowerTarget = 0, 
											  int			nDisableDialogBoxes = 0, 
											  BYTE			DevicesBeforeTarget = 0, 
											  BYTE			DevicesAfterTarget = 0,
											  WORD			IRBitsBeforeTarget = 0, 
											  WORD			IRBitsAfterTarget = 0);

SIUTIL_API(HRESULT)	SetRAMMemory(BYTE *			ptrMem, 
								 DWORD			wStartAddress, 
								 unsigned		int nLength);

SIUTIL_API(HRESULT)	SetScratchPadMemory(BYTE *			ptrMem, 
										DWORD			wStartAddress, 
										unsigned int	nLength,
										int				nDisableDialogs=0);

SIUTIL_API(HRESULT)	SetTargetGo();

SIUTIL_API(HRESULT)	SetTargetHalt();

SIUTIL_API(HRESULT)	SetUSBDevice(const char *	sSerialNum);   // Note: Not published

SIUTIL_API(HRESULT)	SetXRAMMemory(BYTE *		ptrMem, 
								  DWORD			wStartAddress, 
								  unsigned int	nLength);

SIUTIL_API(HRESULT)	TargetReset();

SIUTIL_API(HRESULT)	USBDebugDevices(DWORD *	dwDevices);

