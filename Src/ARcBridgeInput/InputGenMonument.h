// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTGENMONUMENT_H__69D526F5_CA51_429B_BFF6_22F401C269FB__INCLUDED_)
#define AFX_INPUTGENMONUMENT_H__69D526F5_CA51_429B_BFF6_22F401C269FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputGenMonument.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputGenMonument dialog

class CInputGenMonument : public CInputDialogBase
{
// Construction
public:
	CInputGenMonument(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputGenMonument)
	enum { IDD = IDD_GEN_MONUMENT };
	CXTPComboBox	m_cbBridgeNo;
	CGridCtrlMng	m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputGenMonument)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputGenMonument)
	afx_msg void OnSelchangeComboBridgeNo();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	DECLARE_MESSAGE_MAP()

private:
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnPreInitDialog();
	void InitComboCtrl();
	void SetDataDefault();
	void SetGridTitle();
	void SetDataInit();
	void SetDataSave();
	BOOL IsValid();
	
	void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTGENMONUMENT_H__69D526F5_CA51_429B_BFF6_22F401C269FB__INCLUDED_)
