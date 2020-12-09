// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DIALOGEHPLINK_H__9C8F1729_4845_473A_8042_62B39A12EFBA__INCLUDED_)
#define AFX_DIALOGEHPLINK_H__9C8F1729_4845_473A_8042_62B39A12EFBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogEHPLink.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogEHPLink dialog

class CDialogEHPLink : public CDialog
{
// Construction
public:
	CDialogEHPLink(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogEHPLink)
	enum { IDD = IDD_EHP_POPUP };
	CButton	m_btnLink;
	CButton	m_btnOK;
	CXTHyperLink	m_hyperURL;
	CBitmap m_bmpLink;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogEHPLink)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogEHPLink)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonLink();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGEHPLINK_H__9C8F1729_4845_473A_8042_62B39A12EFBA__INCLUDED_)
