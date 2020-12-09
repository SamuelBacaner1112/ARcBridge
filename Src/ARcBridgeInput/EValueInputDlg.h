// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_EVALUEINPUTDLG_H__38ED3D8E_2C39_4A99_95AF_FD4146F801C3__INCLUDED_)
#define AFX_EVALUEINPUTDLG_H__38ED3D8E_2C39_4A99_95AF_FD4146F801C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EValueInputDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEValueInputDlg dialog

class CEValueInputDlg : public CUserDialog
{
// Construction
public:
	CEValueInputDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEValueInputDlg)
	enum { IDD = IDD_DIALOG_EVALUE };
	CStatic	m_szUnit;
	CButton	m_btOK;
	CButton	m_btCancel;
	CString	m_szEValue;
	//}}AFX_DATA

	long m_nTypeUnit;
	void SetLSD(BOOL bLSD) {m_bLSD = bLSD;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEValueInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEValueInputDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bLSD;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVALUEINPUTDLG_H__38ED3D8E_2C39_4A99_95AF_FD4146F801C3__INCLUDED_)
