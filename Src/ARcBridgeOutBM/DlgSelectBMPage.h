// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DLGSELECTBMPAGE_H__9E6853B8_BDE4_4521_B7FF_CDC96C833C6F__INCLUDED_)
#define AFX_DLGSELECTBMPAGE_H__9E6853B8_BDE4_4521_B7FF_CDC96C833C6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelectBMPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectBMPage dialog

class CDlgSelectBMPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CDlgSelectBMPage)

// Construction
public:
	CDlgOptionBMSheet *m_pOpt;
	CDlgSelectBMPage();
	~CDlgSelectBMPage();

// Dialog Data
	//{{AFX_DATA(CDlgSelectBMPage)
	enum { IDD = IDD_PROPPAGE_SELECT_BM };
	CListCtrl	m_listNormalBM;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelectBMPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgSelectBMPage)
	afx_msg void OnAllnoselect();
	afx_msg void OnAllselect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetDataSave();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELECTBMPAGE_H__9E6853B8_BDE4_4521_B7FF_CDC96C833C6F__INCLUDED_)
