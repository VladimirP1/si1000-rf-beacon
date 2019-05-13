
// SLAB_USB_Interrupt.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSLAB_USB_InterruptApp:
// See SLAB_USB_Interrupt.cpp for the implementation of this class
//

class CSLAB_USB_InterruptApp : public CWinApp
{
public:
	CSLAB_USB_InterruptApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSLAB_USB_InterruptApp theApp;