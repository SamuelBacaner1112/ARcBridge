// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_TYPECYCLE3DLG_H__6738905A_BD8A_42F8_B4EC_506A848E8A6E__INCLUDED_)
#define AFX_TYPECYCLE3DLG_H__6738905A_BD8A_42F8_B4EC_506A848E8A6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TypeCycle3Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTypeCycle3Dlg dialog

class CTypeCycle3Dlg : public CDialog
{
// Construction
public:
	CTypeCycle3Dlg(CWnd* pParent = NULL);   // standard constructor
	CTypeCycle3Dlg(CRcBridgeRebar *pBri, CARcBridgeDataStd *pStd, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTypeCycle3Dlg)
	enum { IDD = IDD_DIALOG_TYPECYCLE3 };
	CButton	m_btnCancel;
	CButton	m_btnOK;
	CButton	m_btnRecommond;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTypeCycle3Dlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTypeCycle3Dlg)
	afx_msg void OnRecommond();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
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

#endif // !defined(AFX_TYPECYCLE3DLG_H__6738905A_BD8A_42F8_B4EC_506A848E8A6E__INCLUDED_)
