// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_REBARSUPPORTDLG_H__5112A674_FF0A_4262_8298_A8CE59AEDAB6__INCLUDED_)
#define AFX_REBARSUPPORTDLG_H__5112A674_FF0A_4262_8298_A8CE59AEDAB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RebarSupportDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRebarSupportDlg dialog

class CRebarSupportDlg : public CDialog
{
// Construction
public:
	CRebarSupportDlg(CWnd* pParent = NULL);   // standard constructor
	CRebarSupportDlg(CRcBridgeRebar *pBri, CARcBridgeDataStd *pStd, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRebarSupportDlg)
	enum { IDD = IDD_DIALOG_REBARSUPPORT };
	CButton	m_btnRecommond;
	CButton	m_btnOk;
	CButton	m_btnCancel;
	CButton	m_checkHunch;
	CButton	m_checkDan;
	CButton	m_check23;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRebarSupportDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRebarSupportDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnRecommond();
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
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REBARSUPPORTDLG_H__5112A674_FF0A_4262_8298_A8CE59AEDAB6__INCLUDED_)
