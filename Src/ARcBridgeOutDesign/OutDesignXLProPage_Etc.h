// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OUTDESIGNXLPROPAGE_ETC_H__467C15E4_D81F_439B_A454_21EEE0194B58__INCLUDED_)
#define AFX_OUTDESIGNXLPROPAGE_ETC_H__467C15E4_D81F_439B_A454_21EEE0194B58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutDesignXLProPage_Etc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COutDesignXLProPage_Etc dialog

class COutDesignXLProPage_Etc : public CPropertyPage
{
	DECLARE_DYNCREATE(COutDesignXLProPage_Etc)

// Construction
public:
	void SetDataSave();
	void InitListCtrl();
	COutDesignOptionSheet *m_pOpt;

	COutDesignXLProPage_Etc();
	~COutDesignXLProPage_Etc();

// Dialog Data
	//{{AFX_DATA(COutDesignXLProPage_Etc)
	enum { IDD = IDD_PROPPAGE_ETC };
	CButton	m_btnCheckNone;
	CButton	m_btnCheckAll;
	CListCtrl	m_ctrlEtcList_Etc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COutDesignXLProPage_Etc)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COutDesignXLProPage_Etc)
	afx_msg void OnAllcheck();
	afx_msg void OnAlluncheck();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTDESIGNXLPROPAGE_ETC_H__467C15E4_D81F_439B_A454_21EEE0194B58__INCLUDED_)
