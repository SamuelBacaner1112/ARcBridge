#include "stdafx.h"

#include "CommandReDir.h"
#include "CommandResultDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CCommandReDir::CCommandReDir(CCommandResultDlg *pDlg) : m_pDlg(pDlg)
{

}

CCommandReDir::~CCommandReDir()
{

}

void CCommandReDir::OnChildStarted(HGLPCTSTR lpszCmdLine)
{		
	CString strText = _T(">>Start<<");

	CopyData(m_pDlg->m_hWnd, SET_WINDOW_TEXT, NULL, strText.GetBuffer(0), HGGETCB(strText.GetLength()));	
}

// Send stdout or stderr text to the display window.

void CCommandReDir::OnChildWrite(UINT OutputID, HGLPCTSTR lpszOutput)
{	
	CString szOutput = lpszOutput;
// 	if(m_pDlg->m_nTypeRun == 0)
// 	{
// 		long nIdxStr = szOutput.Find(_T("OUTPUT.pcom"));
// 		if(nIdxStr != -1)
// 		{
// 			CString szCombo = szOutput.Mid(nIdxStr + 11, 2);
// 			if(szCombo == _T("01"))
// 			{
// 				CString szPrint = _T("  R E A D I N G   I N P U T   D A T A");
// 				CopyData(m_pDlg->m_hWnd, STDIO_WRITE_TEXT, NULL, szPrint, HGGETCB(::hgstrlen(szPrint)));
// 				CopyData(m_pDlg->m_hWnd, STDIO_WRITE_TEXT, NULL, _T(""), HGGETCB(::hgstrlen(_T(""))));
// 
// 				szPrint = _T("  B E G I N   A N A L Y S I S");
// 				CopyData(m_pDlg->m_hWnd, STDIO_WRITE_TEXT, NULL, szPrint, HGGETCB(::hgstrlen(szPrint)));
// 				CopyData(m_pDlg->m_hWnd, STDIO_WRITE_TEXT, NULL, _T(""), HGGETCB(::hgstrlen(_T(""))));
// 			}
// 
// 			szOutput.Format(_T("  >>>  Stress Flow Analysis Done  :  Combination %s"), szCombo);
// 			CopyData(m_pDlg->m_hWnd, STDIO_WRITE_TEXT, NULL, szOutput, HGGETCB(::hgstrlen(lpszOutput)));
// 		}
// 	}
// 	else
	{
		CopyData(m_pDlg->m_hWnd, STDIO_WRITE_TEXT, NULL, szOutput, HGGETCB(::hgstrlen(lpszOutput))); 
	}
}

// Send stdout text to the display window.

void CCommandReDir::OnChildStdOutWrite(HGLPCTSTR lpszBuffer)
{
	OnChildWrite(STDIO_WRITE_TEXT, lpszBuffer);
}

// Send stderr text to the display window.

void CCommandReDir::OnChildStdErrWrite(HGLPCTSTR lpszBuffer)
{
	OnChildWrite(STDERR_WRITE_TEXT, lpszBuffer);
}

// Child process is terminated correctly.

void CCommandReDir::OnChildTerminate()
{
	// Terminate the child process.
	// Send STDIO_CLOSE Command with empty string to call
	// TerminateChildProcess().	
	m_pDlg->AddCommand(STDIO_CLOSE, _T(""));

	// Terminate the application or change the window text.
	//CFrameWnd *pFrame = (CFrameWnd*)AfxGetMainWnd();
	//if (pFrame != NULL)
	//{
	//if (m_pDlg->m_bExitClose)
	//	pFrame->PostMessage(WM_COMMAND, ID_APP_EXIT, 0);
	//else if(theApp.m_nStep < STEP3)
	//{
	//	++theApp.m_nStep;
	//	while(IsChildRunning());
	//	AfxGetMainWnd()->SendMessage(WM_NEXT_COMMAND, (WPARAM)theApp.m_nStep, (LPARAM)0);
	//}
	//else if(theApp.m_nStep == STEP6)
	//{
	//	++theApp.m_nStep;
	//	AfxGetMainWnd()->SendMessage(WM_NEXT_COMMAND, (WPARAM)theApp.m_nStep, (LPARAM)0);
	//}
	//else
	//	if (pFrame != NULL)
	CString sMessage = _T(">>End<<");
	CopyData(m_pDlg->m_hWnd, SET_WINDOW_TEXT, NULL, sMessage.GetBuffer(0), HGGETCB(sMessage.GetLength()));
	//}
}