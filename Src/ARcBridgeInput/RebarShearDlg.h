// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#include "afxwin.h"
#if !defined(AFX_REBARSHEARDLG_H__CB7A18F5_7364_4EC1_9786_E0C19B0CCA3C__INCLUDED_)
#define AFX_REBARSHEARDLG_H__CB7A18F5_7364_4EC1_9786_E0C19B0CCA3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RebarShearDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRebarShearDlg dialog

class CRebarShearDlg : public CDialog
{
// Construction
public:
	CRebarShearDlg(CWnd* pParent = NULL);   // standard constructor
	CRebarShearDlg(CRcBridgeRebar *pBri, CARcBridgeDataStd *pStd, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRebarShearDlg)
	enum { IDD = IDD_DIALOG_REBARSHEAR };
	CButton	m_btnOk;
	CButton	m_btnCancel;
	CButton	m_btnRecommand;
	CButton m_CheckUpperHunch;
	CButton m_CheckLowerHunch;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRebarShearDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRebarShearDlg)
	afx_msg void OnRecommond();
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckButtonUpperSlab();
	afx_msg void OnCheckButtonLowerSlab();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SyncData();
	void InitGridManager();
	CGridCtrlMng	m_Grid;
	CRcBridgeRebar* m_pBri;
	CARcBridgeDataStd *m_pStd;
	void SetControl();
	void SetDataSave();
	void SetDataInit();
	void SetGridData();
	HGBOOL IsSingleHook();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REBARSHEARDLG_H__CB7A18F5_7364_4EC1_9786_E0C19B0CCA3C__INCLUDED_)
