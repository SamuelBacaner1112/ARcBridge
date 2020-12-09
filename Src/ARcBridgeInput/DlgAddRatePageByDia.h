// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DLGADDRATEPAGEBYDIA_H__3EE61FC3_8287_4C0F_9570_DC33CA5DE963__INCLUDED_)
#define AFX_DLGADDRATEPAGEBYDIA_H__3EE61FC3_8287_4C0F_9570_DC33CA5DE963__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAddRatePageByDia.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAddRatePageByDia dialog
class CDlgAddRateSheet;
class CDlgAddRatePageByDia : public CPropertyPage
{
	DECLARE_DYNCREATE(CDlgAddRatePageByDia)

// Construction
public:
	long m_nIdxCurPage;
	double GetDia(BOOL bLower);
	double m_dDiaOrg;
	long GetIdxDia(double dDia);
	CDoubleArray m_dArrDia;
	CDlgAddRateSheet* m_pSheet;
	CDlgAddRatePageByDia();
	~CDlgAddRatePageByDia();

// Dialog Data
	//{{AFX_DATA(CDlgAddRatePageByDia)
	enum { IDD = IDD_PROPPAGE_ADDRATE_DIA };
	CComboBox	m_comboDia2;
	CComboBox	m_comboDia1;
	double	m_dRate1;
	double	m_dRate2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDlgAddRatePageByDia)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgAddRatePageByDia)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboDia1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGADDRATEPAGEBYDIA_H__3EE61FC3_8287_4C0F_9570_DC33CA5DE963__INCLUDED_)
