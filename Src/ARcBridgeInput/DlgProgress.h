// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DLGPROGRESS_H__FA323E32_B5EC_4BE0_A3F7_3050ACD2A17C__INCLUDED_)
#define AFX_DLGPROGRESS_H__FA323E32_B5EC_4BE0_A3F7_3050ACD2A17C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgProgress.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress dialog
#include "resource.h"

class AFX_EXT_CLASS CDlgProgress : public CDialog
{
// Construction
public:
	void SetProgressPos(CString sMessage, long nPos);
	void InitProgressData();
	CDlgProgress(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgProgress)
	enum { IDD = IDD_DIALOG_HGPROGRESS };
	CProgressCtrl	m_ctlProgress;
	CString	m_staticMessage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgProgress)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString m_sMessage;
	long m_nCur;
	long m_nMax;
	long m_nMin;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROGRESS_H__FA323E32_B5EC_4BE0_A3F7_3050ACD2A17C__INCLUDED_)
