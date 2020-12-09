// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTDIALOGPANE_H__BE8CE620_53B7_43C6_AC30_98AF4F4EBDA1__INCLUDED_)
#define AFX_INPUTDIALOGPANE_H__BE8CE620_53B7_43C6_AC30_98AF4F4EBDA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "InputDialogBase.h"
#include "InputComment.h"

// InputDialogPane.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputDialogPane window

class AFX_EXT_CLASS CInputDialogPane : public CWnd
{
// Construction
public:
	CInputDialogPane();

// Attributes
public:
	CInputDialogBase* m_pInputDialog;
	CInputComment m_wndComment;

// Operations
public:
	void SetInputDialog(CInputDialogBase* pDlg) { m_pInputDialog = pDlg; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputDialogPane)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CInputDialogPane();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInputDialogPane)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTDIALOGPANE_H__BE8CE620_53B7_43C6_AC30_98AF4F4EBDA1__INCLUDED_)
