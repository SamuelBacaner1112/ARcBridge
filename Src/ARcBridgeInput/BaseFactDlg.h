// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_BASEFACTDLG_H__8E8688E8_54C6_43F1_BA47_D53F1D1E0FCE__INCLUDED_)
#define AFX_BASEFACTDLG_H__8E8688E8_54C6_43F1_BA47_D53F1D1E0FCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseFactDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBaseFactDlg dialog

class CBaseFactDlg : public CUserDialog
{
// Construction
public:
	CBaseFactDlg(CWnd* pParent = NULL);   // standard constructor
	CBaseFactDlg(CFootingApp *pFooting, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBaseFactDlg)
	enum { IDD = IDD_DIALOG_BASEFACT };
	CString	m_szFact;
	CString	m_szTitle;
	double	m_dRD;
	//}}AFX_DATA
	CDomyunView m_DomView;
	CFootingApp *m_pFooting;
	long m_nType;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseFactDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBaseFactDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);	
	afx_msg void OnButtonRedraw();
	virtual void OnOK();
	afx_msg void OnButtonRecommand();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:	
	void OnLButtonDownDom(UINT nFlags, CDPoint ptPos);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASEFACTDLG_H__8E8688E8_54C6_43F1_BA47_D53F1D1E0FCE__INCLUDED_)
