// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_FINITEPILEDLG_H__FD309B6E_FAFC_4E82_AEF3_1CDD78A092E2__INCLUDED_)
#define AFX_FINITEPILEDLG_H__FD309B6E_FAFC_4E82_AEF3_1CDD78A092E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FinitePileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFinitePileDlg dialog

class CFinitePileDlg : public CDialog
{
// Construction
public:
	CFinitePileDlg(CWnd* pParent = NULL);   // standard constructor
	CFinitePileDlg(CExPileApp* pExPile, CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CFinitePileDlg)
	enum { IDD = IDD_DIALOG_FINITE_PILE };
	CButton	m_btnRecommand;
	CButton	m_btnCancel;
	CButton	m_btnOK;
	CString	m_sFixPhi1[2];
	CString	m_sFixPhi2[2];
	CString	m_sFixPhi3[2];
	CString	m_sFixPhi4[2];
	CString	m_sHingePhi1[2];
	CString	m_sHingePhi2[2];
	CString	m_sHingePhi3[2];
	CString	m_sHingePhi4[2];
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFinitePileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFinitePileDlg)
	afx_msg void OnRecommand();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CExPileApp* m_pExPile;
	void SaveData();
	void InitData();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINITEPILEDLG_H__FD309B6E_FAFC_4E82_AEF3_1CDD78A092E2__INCLUDED_)
