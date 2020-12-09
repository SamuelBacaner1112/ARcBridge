// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DIALOGMENUOUTBM_H__FD1AA76A_502C_4E03_81EF_44F9A2F1C9D8__INCLUDED_)
#define AFX_DIALOGMENUOUTBM_H__FD1AA76A_502C_4E03_81EF_44F9A2F1C9D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogMenuOutBM.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogMenuOutBM dialog
#include "resource.h"

class CDialogMenuOutBM : public CXTResizeDialog
{
// Construction
public:
	CDialogMenuOutBM(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogMenuOutBM)
	enum { IDD = IDD_DIALOG_MENU_OUTBM };
	CXTPTabCtrl	m_ctlTab;
	CTreeCtrl	m_ctlTreeBM;
	CButton	m_btnBM;
	CButton	m_btnSelectRev;
	CButton	m_btnSelectNone;
	CButton	m_btnSelectBMSum;
	CButton	m_btnSelectBM;
	CButton	m_btnSelectAll;
	//}}AFX_DATA

	COLORREF m_colorBK;
	long m_nTabBMNormal;
	BOOL m_bInitingTree;
	BOOL m_bPushCtrlKey;

	void InitTree();
	void SetSelectBMTab(long nTab);
	void RunOnButtonBmByStruct();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogMenuOutBM)
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnButtonSelectAll();
	afx_msg void OnButtonSelectBm();
	afx_msg void OnButtonSelectBmsum();
	afx_msg void OnButtonSelectNone();
	afx_msg void OnButtonSelectRev();
	afx_msg void OnClickTreeBm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTreeBm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeTabBm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonBm();
	afx_msg void OnDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogMenuOutBM)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetCheckTreeItem( UINT nHitTestInfo );
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGMENUOUTBM_H__FD1AA76A_502C_4E03_81EF_44F9A2F1C9D8__INCLUDED_)
