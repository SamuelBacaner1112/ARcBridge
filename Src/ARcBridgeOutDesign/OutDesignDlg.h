// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OUTDESIGNDLG_H__45DBC38F_4A48_4CD1_978A_CB03F6853B22__INCLUDED_)
#define AFX_OUTDESIGNDLG_H__45DBC38F_4A48_4CD1_978A_CB03F6853B22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutDesignDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COutDesignDlg dialog
//#include "CheckComboBox.h"
#include "resource.h"
class CRcBridgeApp;
class COutDesignDlg : public CDialog, public CThreadWork
{
// Construction
public:
	BOOL CheckStandardFont(CXLControl *pXL, BOOL bInit, CString sFont, double dSize, BOOL bView);
	void InitOption();
	COutDesignDlg(CWnd* pParent = NULL);   // standard constructor

	CARcBridgeOutDesignStd *m_pStd;

	CProgressCtrl	m_ctlProgress;
	BOOL MakeExcel(long nBri);
	UINT DoWork();
	CXLControl *m_pXL;

	long m_nPos;
	BOOL m_bOK;
	BOOL m_bEarthQuake;
	CString m_szLastFileName;
// Dialog Data
	//{{AFX_DATA(COutDesignDlg)
	enum { IDD = IDD_DIALOG_DESIGN_OUT };
	CButton	m_btnOK;
	CButton	m_btnOption;
	CButton	m_btnViewWmf;
	CButton	m_btnPrint;
	CButton	m_btnOpen;
	CTabCtrl	m_ctlTab;
	CComboBox	m_ComboList;
	CStatic	m_sProgText;
	CCheckComboBox	m_ComboSelect;
	int		m_nPrintType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutDesignDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COutDesignDlg)
	afx_msg void OnSelchangeTabType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPrint();
	afx_msg void OnRadioAll();
	afx_msg void OnRadioOne();
	afx_msg void OnRadioSelect();
	afx_msg void OnButtonOpen();
	afx_msg void OnButtonViewwmf();
	afx_msg void OnButtonOption();
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LRESULT OnXlOutComplete(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

private:
	BOOL LoadOutput(CRcBridgeApp *pBri);
	void InitComboARcBridge();
	void InitOptionItemCorner();
};


// INSERT_ATESTCODE
class AFX_EXT_CLASS CStaticATest {public: static HGLONG32 m_bThoughtATest;};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTDESIGNDLG_H__45DBC38F_4A48_4CD1_978A_CB03F6853B22__INCLUDED_)
