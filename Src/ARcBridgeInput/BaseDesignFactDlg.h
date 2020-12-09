// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_BASEDESIGNFACTDLG_H__F2A0A4F9_738F_417A_96AC_0D57C565C16F__INCLUDED_)
#define AFX_BASEDESIGNFACTDLG_H__F2A0A4F9_738F_417A_96AC_0D57C565C16F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseDesignFactDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBaseDesignFactDlg dialog

class CBaseDesignFactDlg : public CUserDialog
{
// Construction
public:
	CBaseDesignFactDlg(CWnd* pParent = NULL);   // standard constructor
	CBaseDesignFactDlg(CFootingApp *pFooting, CWnd* pParent = NULL);   

// Dialog Data
	//{{AFX_DATA(CBaseDesignFactDlg)
	enum { IDD = IDD_DIALOG_BASEDESIGNFACT };
	CString	m_szTitle;
	CString	m_szFact;
	double m_dLB;
	//}}AFX_DATA
	CDomyunView m_DomView;
	CFootingApp *m_pFooting;
	long m_nType;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseDesignFactDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBaseDesignFactDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRedraw();
	afx_msg void OnButtonRecommand();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:	
	void OnLButtonDownDom(UINT nFlags, CDPoint ptPos);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASEDESIGNFACTDLG_H__F2A0A4F9_738F_417A_96AC_0D57C565C16F__INCLUDED_)
