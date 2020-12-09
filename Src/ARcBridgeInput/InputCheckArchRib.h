// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTCHECKARCHRIB_H__79E0603C_2F9A_49B8_B1CD_AA8E01CC9CDF__INCLUDED_)
#define AFX_INPUTCHECKARCHRIB_H__79E0603C_2F9A_49B8_B1CD_AA8E01CC9CDF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputCheckArchRib.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputCheckArchRib dialog
class CARcBridgeCalcStd;
class CGirderAndColumn;
class CInputCheckArchRib : public CXTResizeDialog
{
// Construction
public:
	CInputCheckArchRib(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputCheckArchRib();

	CARcBridgeInputStd* m_pIn;
	CRcBridgeRebar *m_pBri;
	
	long m_nInputIDD;
	long m_nTypeCheck;
	
	CARcBridgeCalcStd *m_pCalc;

	CGridCtrlMng m_Grid; 
	CDomyun m_Dom;

	void Check();
	void ShowAndCheck(CWnd *pWndParent);
	void SetBriAndContent(CRcBridgeRebar *pBri, long nInputIDD, long nTypeCheck);
	
// Dialog Data
	//{{AFX_DATA(CInputCheckArchRib)
	enum { IDD = IDD_DIALOG_CHECK_ARCHRIB };
	CXTPTabCtrl	m_ctlTab;
	CButton	m_btnOk;
	CButton	m_btnCancel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputCheckArchRib)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputCheckArchRib)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
//	afx_msg void OnTabSelchange(NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg void OnTabSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
private:
	CDomyunView m_DomView;
	CTypedPtrArray <CObArray, CGirderAndColumn*> m_pList;

	void InitTab();
	void SetGridTitle();
	void SetDataInit();

	void ShowDlg();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTCHECKARCHRIB_H__79E0603C_2F9A_49B8_B1CD_AA8E01CC9CDF__INCLUDED_)
