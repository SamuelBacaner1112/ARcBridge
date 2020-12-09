// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_MIDASINPUTPROGRESSDLG_H__F66E3A95_9B71_4C3E_A236_4DDE4D68F8AB__INCLUDED_)
#define AFX_MIDASINPUTPROGRESSDLG_H__F66E3A95_9B71_4C3E_A236_4DDE4D68F8AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MidasInputProgressDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMidasInputProgressDlg dialog

class CMidasInputProgressDlg : public CDialog
{
// Construction
public:
	CMidasInputProgressDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMidasInputProgressDlg)
	enum { IDD = IDD_DIALOG_MIDASREAD };
	CProgressCtrl	m_ctlProgress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMidasInputProgressDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMidasInputProgressDlg)
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIDASINPUTPROGRESSDLG_H__F66E3A95_9B71_4C3E_A236_4DDE4D68F8AB__INCLUDED_)
