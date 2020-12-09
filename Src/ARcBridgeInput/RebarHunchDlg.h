// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_REBARHUNCHDLG_H__CC546E5F_0478_487A_A15E_E168944CF0CB__INCLUDED_)
#define AFX_REBARHUNCHDLG_H__CC546E5F_0478_487A_A15E_E168944CF0CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RebarHunchDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRebarHunchDlg dialog

class CRebarHunchDlg : public CDialog
{
// Construction
public:
	CRebarHunchDlg(CWnd* pParent = NULL);   // standard constructor
	CRebarHunchDlg(CRcBridgeRebar *pBri, CARcBridgeDataStd *pStd, long nCycle, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRebarHunchDlg)
	enum { IDD = IDD_DIALOG_REBARHUNCH };
	CButton	m_btnRecommand;
	CButton	m_btnOk;
	CXTPTabCtrl	m_ctlTab;
	CButton	m_btnCancel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRebarHunchDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRebarHunchDlg)
	afx_msg void OnButtonRecommand();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);

private:
	void SetDataDefault();
	void InitTab();
	void SetGridData();
	void SetDataSave();
	void SetDataInit();

	void SetGridDataUpperSlab();
	void SetGridDataLowerSlab();

	CGridCtrlMng	m_Grid;	
	CStringArray m_sArrTabName;
	long m_nTab;
	long m_nTabUpperSlab;
	long m_nTabLowerSlab;

	long m_nCycle;
	CRcBridgeRebar* m_pBri;
	CARcBridgeDataStd *m_pStd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REBARHUNCHDLG_H__CC546E5F_0478_487A_A15E_E168944CF0CB__INCLUDED_)
