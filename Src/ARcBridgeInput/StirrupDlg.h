// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_STIRRUPDLG_H__5F424841_F0C9_4431_B0D3_3CA1C0024FD6__INCLUDED_)
#define AFX_STIRRUPDLG_H__5F424841_F0C9_4431_B0D3_3CA1C0024FD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StirrupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStirrupDlg dialog

class CStirrupDlg : public CDialog
{
// Construction
public:
	CStirrupDlg(CWnd* pParent = NULL);   // standard constructor
	CStirrupDlg(CRcBridgeRebar *pBri, long nJ, CInputDialogBase* pParent);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStirrupDlg)
	enum { IDD = IDD_DIALOG_STIRRUP };
	CXTPComboBox	m_comboStirrup;
	CButton	m_btnOk;
	CGridCtrlMng	m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStirrupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStirrupDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboStirrup();
	//}}AFX_MSG
	void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	DECLARE_MESSAGE_MAP()

public:
	void SetDataDefault();
	void SetDataInit();
	void SetGridData();
	void SetDataSave();
	long m_nJ;
	CRcBridgeRebar* m_pBri;
	CGirderSectionRebar* m_pGirderRB;
	CInputDialogBase *m_pParent;

private:
	void InitCombo();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STIRRUPDLG_H__5F424841_F0C9_4431_B0D3_3CA1C0024FD6__INCLUDED_)
