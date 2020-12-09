// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OPTIMIZEREBAR_H__B953B832_40C0_4D53_BB0E_B2D307B1CEC8__INCLUDED_)
#define AFX_OPTIMIZEREBAR_H__B953B832_40C0_4D53_BB0E_B2D307B1CEC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptimizeRebar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptimizeRebar dialog

class COptimizeRebar : public CDialog
{
// Construction
public:
	COptimizeRebar(CRcBridgeData *pBri, CWnd* pParent = NULL);   // standard constructor

	CRcBridgeData *m_pBri;
	CStringArray m_strArr;

// Dialog Data
	//{{AFX_DATA(COptimizeRebar)
	enum { IDD = IDD_DIALOG_OPTIMIZE };
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CXTPComboBox	m_cbDiaMin;
	CXTPComboBox	m_cbDiaMax;
	BOOL	m_bCheckRebar2Dan;
	BOOL	m_bCheckSpliteCycle;
	CString	m_szSafeRate;
	BOOL	m_bCheckRebarMatch;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptimizeRebar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptimizeRebar)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIMIZEREBAR_H__B953B832_40C0_4D53_BB0E_B2D307B1CEC8__INCLUDED_)
