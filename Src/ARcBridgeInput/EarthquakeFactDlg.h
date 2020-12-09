// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_EARTHQUAKEFACTDLG_H__98C8F1F2_512E_46C8_A3E8_B058161FB460__INCLUDED_)
#define AFX_EARTHQUAKEFACTDLG_H__98C8F1F2_512E_46C8_A3E8_B058161FB460__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EarthquakeFactDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEarthquakeFactDlg dialog

class CEarthquakeFactDlg : public CUserDialog
{
// Construction
public:
	CEarthquakeFactDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEarthquakeFactDlg)
	enum { IDD = IDD_DIALOG_EARTHFACT };
	int		m_nType;
	CString	m_dFact1;
	CString	m_dFact2;
	CString	m_dFact3;
	CString	m_dFact4;
	CString	m_dFact5;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEarthquakeFactDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEarthquakeFactDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EARTHQUAKEFACTDLG_H__98C8F1F2_512E_46C8_A3E8_B058161FB460__INCLUDED_)
