// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DIALOGCRACKENV_H__6243E30A_5985_431F_8978_6E951E4FE271__INCLUDED_)
#define AFX_DIALOGCRACKENV_H__6243E30A_5985_431F_8978_6E951E4FE271__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogCrackEnv.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogCrackEnv dialog

class CDialogCrackEnv : public CUserDialog
{
// Construction
public:
	CDialogCrackEnv(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogCrackEnv)
	enum { IDD = IDD_DIALOG_CRACK_ENV };
	int		m_nType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogCrackEnv)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogCrackEnv)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGCRACKENV_H__6243E30A_5985_431F_8978_6E951E4FE271__INCLUDED_)
