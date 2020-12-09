// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTBASECROSSPOSITION_H__506AF5F5_C3A7_43D0_AAAC_72002846407C__INCLUDED_)
#define AFX_INPUTBASECROSSPOSITION_H__506AF5F5_C3A7_43D0_AAAC_72002846407C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputBaseCrossPosition.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputBaseCrossPosition dialog

class CInputBaseCrossPosition : public CInputDialogBase
{
// Construction
public:
	CInputBaseCrossPosition(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputBaseCrossPosition)
	enum { IDD = IDD_BASE6_CROSS_POSITION };
	CButton	m_btnMirror;
	CXTPComboBox	m_cbBridgeNo;
	CButton	m_btDelete;
	CButton	m_btAdd;
	CXTPComboBox	m_ComboLoadType;
	CXTPComboBox	m_ComboLoadSelect;
	CGridCtrlMng	m_Grid;
	BOOL	m_bUnderGroundPaveDetail;
	BOOL	m_bViewAllDir;
	BOOL	m_bAutoDefaultSlab;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputBaseCrossPosition)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputBaseCrossPosition)
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCellChangedCol(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCheckViewCounterDir();
	afx_msg void OnButtonApplyMirror();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnPreInitDialog();
	void InitComboCtrl();

	void SetDataDefault();
	void SetDataInit();
	void SetDataSave();
	void SetGridTitle();
	//

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTBASECROSSPOSITION_H__506AF5F5_C3A7_43D0_AAAC_72002846407C__INCLUDED_)
