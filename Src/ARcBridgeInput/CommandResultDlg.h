// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#pragma once


// #define STDIO_OPEN  1
// #define STDIO_CLOSE 2
// #define STDIO_WRITE 3

//#define WM_STDIO_COMMAND WM_USER + 53
#define WM_CALC_STRESS_FLOW_OK		 WM_USER + 54
#define WM_CALC_STAD2D_OK		 WM_USER + 64
//#define WM_CALC_END		 WM_USER + 55

#include "Afxmt.h" // For CCriticalSection
#include "resource.h"
#include "..\..\..\Engine\BaseEng\Src\AFem\ColorListBox.h"


//class CCommandReDir;
class AFX_EXT_CLASS CCommandResultDlg : public CDialog
{
	// Construction
public:
	CCommandResultDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCommandResultDlg();

	CCommandReDir *m_pRedirect;
	CWnd *m_pMainWnd;
	CCriticalSection m_criticalSection;
	CString m_strCmdLine;
	CString m_szBtnText;
	CStringList m_StdioCommand;
	CButton* m_pButton;	
	CStringList m_StrCmdList;

	BOOL m_bShowCompleteMsg;
	BOOL m_bAutoTerminal;	// 자동으로 종료(닫기버튼 나오지 않음)
	long m_nTypeRun;	// 0 - 응력흐름도, 1- Stad

	CString m_strTitle;

public:
	void SetBtnText(CString szText) { m_szBtnText = szText; };
	void AddCommand(WPARAM wCommand, HGLPCTSTR lpszCommand);
	void Create(CWnd *pParent, CWnd *pMainWnd=NULL, long nTypeRun = 0) { m_pMainWnd = pMainWnd;  CDialog::Create(IDD, pParent); m_nTypeRun = nTypeRun; }
	void SetTitle(const CString &strTitle);
	// Dialog Data
	//{{AFX_DATA(CCommandResultDlg)
	enum { IDD = IDD_DIALOG_COMMAND_RESULT };
	CColorListBox	m_ctlList;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommandResultDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCommandResultDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnButtonGoError();
	afx_msg void OnButtonGoWarning();
	//}}AFX_MSG
	afx_msg LRESULT OnStdioCommand(WPARAM wCommand, LPARAM lParam);
	afx_msg LRESULT OnCopyData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};