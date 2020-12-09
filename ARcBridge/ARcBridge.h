// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridge.h : main header file for the ARCBRIDGE application
//

#if !defined(AFX_ARCBRIDGE_H__9A16D457_794A_4AED_A44E_C44BF02B7A74__INCLUDED_)
#define AFX_ARCBRIDGE_H__9A16D457_794A_4AED_A44E_C44BF02B7A74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CARcBridgeApp:
// See ARcBridge.cpp for the implementation of this class
//

// VERSION, BUILDNUMBER 정보는 include폴더에 versionInfo.h에 들어있다. 
// StdAfx.h 에서 include하고 있다. 
#ifndef VERSION
#define VERSION _T("1.0.1")
#endif

#ifndef BUILDNUMBER
#define BUILDNUMBER _T("100")
#endif

extern const TCHAR szARcBridgeClass[];

class CARcBridgeApp : public CWinApp
{
public:
	CString GetAUpdateProgramInstallPath();
	CString GetARcBridgeName();
	BOOL m_bSuccessOpen;
	CARcBridgeApp(LPCTSTR lpszAppName=NULL);
	~CARcBridgeApp();

	LPCTSTR m_pszAppName;

	CString m_strFileOpenPathName;

	void SetRegFileExt(CString strValue[4], long nIcon=0, BOOL bOpen=TRUE);
	BOOL IsRegFileExt() const;
	void SetWorkDirectory(CString strDir);
	void SetFixedDirectory();
	CString GetWorkDirectory();
	CString GetFixedDirectory();
	CString GetMRUName(UINT nIndex, BOOL bFullPath);
	UINT GetMRUListSize();
	void DeleteWMF();	
	//CLockDataInfo m_LockInfo;
	CString GetStringVersion();
	CString GetStringLockInfo();
	CString GetStringEtcInfo();
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CARcBridgeApp)
	public:
	virtual BOOL InitInstance();	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CARcBridgeApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	afx_msg void OnFileNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
//	virtual void OnFinalRelease();
};

inline CString CARcBridgeApp::GetFixedDirectory()
{
	CString strSection = _T("Afx Directory");
	CString strFixedItem = _T("Fixed Directory");

	return GetProfileString(strSection, strFixedItem);
}

inline CString CARcBridgeApp::GetWorkDirectory()
{
	CString strSection = _T("Afx Directory");
	CString strWorkItem	= _T("Work Directory");

	return GetProfileString(strSection, strWorkItem);
}

inline void CARcBridgeApp::SetWorkDirectory(CString strDir)
{
	CWinApp* pApp = AfxGetApp();
	CString strSection = _T("Afx Directory");
	CString strWorkItem = _T("Work Directory");

	pApp->WriteProfileString(strSection, strWorkItem, strDir);	
//	m_bFirFileOpen = TRUE;
}


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ARCBRIDGE_H__9A16D457_794A_4AED_A44E_C44BF02B7A74__INCLUDED_)
