/////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "CustomDDX.h"
#include <AfxPriv.h>

/////////////////////////////////////////////////////////////////////////////
// Dialog Data Exchange Functions
/////////////////////////////////////////////////////////////////////////////

void AFXAPI DDX_TextHex(CDataExchange* pDX, int nIDC, UINT& value, int nChars)
{
	ASSERT(nChars >= 1);        // allow them something

    HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
    
	// UpdateData(TRUE)
	if (pDX->m_bSaveAndValidate)
    {
		CString valueStr;
		TCHAR*	endPtr;

		// Get the text from the edit box
        int nLen = ::GetWindowTextLength(hWndCtrl);
        ::GetWindowText(hWndCtrl, valueStr.GetBufferSetLength(nLen), nLen + 1);
        valueStr.ReleaseBuffer();

		// Convert to a numeric value (hexadecimal)
		value = ::_tcstoul(valueStr, &endPtr, 16);

		// Make sure that the number is a valid hex value
		if (valueStr.SpanIncluding(_T("0123456789ABCDEFabcdef")).GetLength() != valueStr.GetLength())
		{
			AfxMessageBox(_T("Enter a hexadecimal value."), MB_ICONEXCLAMATION);
			pDX->Fail();
		}
    }
	// UpdateData(FALSE);
    else
    {
		CString formatStr;
		CString valueStr;

		// Generate the format string for the next line
		// i.e. if nChars = 5, then formatStr = "%05X"
		// This will display a fixed number of digits
		// preceeded by 0's
		formatStr.Format(_T("%%0%dX"), nChars);
		valueStr.Format(formatStr, value);

		// Set the edit box text
		AfxSetWindowText(hWndCtrl, valueStr);
    }
}

/////////////////////////////////////////////////////////////////////////////
// Dialog Data Validation Functions
/////////////////////////////////////////////////////////////////////////////

void AFXAPI DDV_MaxCharsHex(CDataExchange* pDX, UINT const& value, int nChars)
{
	ASSERT(nChars >= 1);        // allow them something

	CString valueStr;
	valueStr.Format(_T("%X"), value);

	if (pDX->m_bSaveAndValidate && valueStr.GetLength() > nChars)
	{
		TCHAR szT[32];
		::_stprintf_s(szT, _T("%d"), nChars);
		CString prompt;
		AfxFormatString1(prompt, AFX_IDP_PARSE_STRING_SIZE, szT);
		AfxMessageBox(prompt, MB_ICONEXCLAMATION, AFX_IDP_PARSE_STRING_SIZE);
		prompt.Empty(); // exception prep
		pDX->Fail();
	}
    else if (pDX->m_pDlgWnd->GetDlgItem(pDX->m_idLastControl)->m_hWnd != NULL && pDX->m_bEditLastControl)
	{
		// limit the control max-chars automatically
		::SendMessage(pDX->m_pDlgWnd->GetDlgItem(pDX->m_idLastControl)->m_hWnd, EM_LIMITTEXT, nChars, 0);
	}
}
