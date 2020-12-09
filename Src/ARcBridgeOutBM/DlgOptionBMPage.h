// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DLGOPTIONBMPAGE_H__076A9898_F373_4D54_A8A5_B4C0425AF174__INCLUDED_)
#define AFX_DLGOPTIONBMPAGE_H__076A9898_F373_4D54_A8A5_B4C0425AF174__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOptionBMPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionBMPage dialog

class CDlgOptionBMPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CDlgOptionBMPage)

// Construction
public:
	CDlgOptionBMSheet *m_pOpt;
	CDlgOptionBMPage();
	~CDlgOptionBMPage();

// Dialog Data
	//{{AFX_DATA(CDlgOptionBMPage)
	enum { IDD = IDD_PROPPAGE_OPTION_BM };
	BOOL	m_bConvertForm;
	BOOL	m_bConvertRef;
	BOOL	m_bDrawElEarth;
	BOOL	m_bRefCellBMSum;
	BOOL	m_bSepRebarMonument;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDlgOptionBMPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgOptionBMPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckRefcellSumcell();
	afx_msg void OnCheckSepRebarMonument();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetControl();
	void SetDataSave();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPTIONBMPAGE_H__076A9898_F373_4D54_A8A5_B4C0425AF174__INCLUDED_)
