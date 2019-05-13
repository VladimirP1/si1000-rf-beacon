
#include "stdafx.h"
#include "LogFunctions.h"

void LogMessage(CRichEditCtrl* richEditCtrl, BOOL status, const char* message, BOOL logToFile, CString logFilename)
{
	CString logMessage;
	CString timeString;
	CTime time;
	CHARFORMAT cf;

	richEditCtrl->GetDefaultCharFormat(cf);
	
	cf.dwMask = CFM_COLOR;
	cf.dwEffects = 0;

	if (status)
	{
		cf.crTextColor = RGB(0, 0, 0);
	}
	else
	{
		cf.crTextColor = RGB(255, 0, 0);
	}

	// Obtain the current time and create a string for the log
	time = CTime::GetCurrentTime();
	timeString = time.Format("%H:%M:%S");

	// Create the log string with the time, error if needed and the message
	logMessage.Format("%s - %s%s\r\n", timeString, (status ? "" : "ERROR: "), message);
	
	// Set Focus to auto scroll the Richedit window and update it
	richEditCtrl->SetFocus();
	richEditCtrl->SetSel(0xFFFF, 0xFFFF);

	richEditCtrl->HideSelection(FALSE, TRUE);

	richEditCtrl->SetSelectionCharFormat(cf);
	richEditCtrl->ReplaceSel(logMessage);

	richEditCtrl->HideSelection(TRUE, TRUE);

	// If we are logging to file, put the message in the file as well
	if (logToFile)
	{
		CFile file;

		// Create the file if it does not exist, but don't overwrite exisiting data
		if (file.Open(logFilename, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate))
		{
			// Seek to end to append text
			file.SeekToEnd();
			file.Write(logMessage, logMessage.GetLength());
			file.Close();
		}
	}
}