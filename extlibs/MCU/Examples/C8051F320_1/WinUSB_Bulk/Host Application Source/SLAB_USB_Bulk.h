
// SLAB_USB_Bulk.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSLAB_USB_BulkApp:
// See SLAB_USB_Bulk.cpp for the implementation of this class
//

class CSLAB_USB_BulkApp : public CWinApp
{
public:
	CSLAB_USB_BulkApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSLAB_USB_BulkApp theApp;