// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_UVALUEINPUTDLG_H__905F416C_EF74_490A_9883_A9A500D42583__INCLUDED_)
#define AFX_UVALUEINPUTDLG_H__905F416C_EF74_490A_9883_A9A500D42583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UValueInputDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUValueInputDlg dialog

class CUValueInputDlg : public CUserDialog
{
// Construction
public:
	CUValueInputDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUValueInputDlg)
	enum { IDD = IDD_DIALOG_UVALUE };
	CButton	m_btOK;
	CButton	m_btCancel;
	CString	m_UValue;
	//}}AFX_DATA

	void SetLSD(BOOL bLSD) {m_bLSD = bLSD;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUValueInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUValueInputDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bLSD;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UVALUEINPUTDLG_H__905F416C_EF74_490A_9883_A9A500D42583__INCLUDED_)
