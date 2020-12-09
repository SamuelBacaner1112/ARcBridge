// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTCHECKEDIT_H__E1735401_108C_44AB_9739_695049BEE3F0__INCLUDED_)
#define AFX_INPUTCHECKEDIT_H__E1735401_108C_44AB_9739_695049BEE3F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputCheckEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputCheckEdit dialog
class CARcBridgeCalcStd;
class CInputCheckEdit : public CXTResizeDialog
{
// Construction
public:
	CInputCheckEdit(CWnd* pParent = NULL);   // standard constructor
	CInputCheckEdit(CRcBridgeRebar *pBri, long nInputIDD, long nTypeCheck=0, CWnd* pParent = NULL);
	~CInputCheckEdit();

	CRcBridgeRebar *m_pBri;
	long m_nInputIDD;
	long m_nTypeCheck;

	CString	   m_sFilename;
	CARcBridgeCalcStd *m_pCalc;

	void Check();
	void ShowAndCheck(CWnd *pWndParent);
	BOOL GetVerify();
	void SetBriAndContent(CRcBridgeRebar *pBri, long nInputIDD, long nTypeCheck);

// Dialog Data
	//{{AFX_DATA(CInputCheckEdit)
	enum { IDD = IDD_DIALOG_CHECK_EDIT };
	CXTPTabCtrl	m_ctlTab;
	CEdit		m_RichEdit;
	CString	m_szCheckEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputCheckEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

protected:

	// Generated message map functions
	//{{AFX_MSG(CInputCheckEdit)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
private:
	void InitTab();
	void ShowDlg();
	void SetDataInit();

	void Calculate();
	void ReadOutFile();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTCHECKEDIT_H__E1735401_108C_44AB_9739_695049BEE3F0__INCLUDED_)
