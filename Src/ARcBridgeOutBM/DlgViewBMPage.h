// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DLGVIEWBMPAGE_H__5CBDF91F_5860_410F_9EB6_DA5FECC5DDE9__INCLUDED_)
#define AFX_DLGVIEWBMPAGE_H__5CBDF91F_5860_410F_9EB6_DA5FECC5DDE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgViewBMPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgViewBMPage dialog

class CDlgOptionBMSheet;
class CDlgViewBMPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CDlgViewBMPage)

// Construction
public:
	CViewBM *m_pViewBM;
	CDlgOptionBMSheet *m_pOpt;
	CDlgViewBMPage();
	~CDlgViewBMPage();

// Dialog Data
	//{{AFX_DATA(CDlgViewBMPage)
	enum { IDD = IDD_PROPPAGE_VIEW_BM };
	CListCtrl	m_listView;
	BOOL	m_bNormalBM;
	BOOL	m_bRebarBM;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDlgViewBMPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgViewBMPage)
	afx_msg void OnAllnoselect();
	afx_msg void OnAllselect();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetControl();
	void InitListCtrl();
	void SetDataSave();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVIEWBMPAGE_H__5CBDF91F_5860_410F_9EB6_DA5FECC5DDE9__INCLUDED_)
