// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DLGADDRATEPAGEBYMAINREBAR_H__D7F62709_AEF7_422F_BF69_8D1CC7FECC19__INCLUDED_)
#define AFX_DLGADDRATEPAGEBYMAINREBAR_H__D7F62709_AEF7_422F_BF69_8D1CC7FECC19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAddRatePageByMainRebar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAddRatePageByMainRebar dialog

class CDlgAddRatePageByMainRebar : public CPropertyPage
{
	DECLARE_DYNCREATE(CDlgAddRatePageByMainRebar)

// Construction
public:
	long m_nIdxCurPage;
	CDlgAddRateSheet* m_pSheet;
	CDlgAddRatePageByMainRebar();
	~CDlgAddRatePageByMainRebar();

// Dialog Data
	//{{AFX_DATA(CDlgAddRatePageByMainRebar)
	enum { IDD = IDD_PROPPAGE_ADDRATE_MAINREBAR };
	double	m_dRate2;
	double	m_dRate1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDlgAddRatePageByMainRebar)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgAddRatePageByMainRebar)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGADDRATEPAGEBYMAINREBAR_H__D7F62709_AEF7_422F_BF69_8D1CC7FECC19__INCLUDED_)
