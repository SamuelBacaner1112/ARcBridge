// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OUTDESIGNXLPROPPAGE_H__73A08F14_FEB8_4D9B_AA81_78A206C47C62__INCLUDED_)
#define AFX_OUTDESIGNXLPROPPAGE_H__73A08F14_FEB8_4D9B_AA81_78A206C47C62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutDesignXLPropPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COutDesignXLPropPage dialog

class COutDesignXLPropPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COutDesignXLPropPage)

// Construction
public:
	void SetDataSave();
	void InitListCtrl();
	COutDesignOptionSheet *m_pOpt;
	
	COutDesignXLPropPage();
	~COutDesignXLPropPage();

// Dialog Data
	//{{AFX_DATA(COutDesignXLPropPage)
	enum { IDD = IDD_PROPPAGE_XL };
	CButton	m_btnCheckNone;
	CButton	m_btnCheckAll;
	CListCtrl	m_ctrlOutList;
	//}}AFX_DATA

	HGBOOL m_bLsd;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COutDesignXLPropPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COutDesignXLPropPage)
	afx_msg void OnAllcheck();
	afx_msg void OnAlluncheck();
	afx_msg void OnItemchangedListOutlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCustomdrawListOutlist(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	HGBOOL IsEnableItem(const CString &str) const;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTDESIGNXLPROPPAGE_H__73A08F14_FEB8_4D9B_AA81_78A206C47C62__INCLUDED_)
