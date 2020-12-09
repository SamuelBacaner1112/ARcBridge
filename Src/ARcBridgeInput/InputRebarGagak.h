// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTREBARGAGAK_H__EB3F9CEB_6102_49B2_907D_3F55950BABB7__INCLUDED_)
#define AFX_INPUTREBARGAGAK_H__EB3F9CEB_6102_49B2_907D_3F55950BABB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputRebarGagak.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputRebarGagak dialog

class CInputRebarGagak : public CInputDialogBase
{
// Construction
public:
	BOOL IsValid();
	CInputRebarGagak(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputRebarGagak)
	enum { IDD = IDD_REBAR_GAGAK };
	CComboBox	m_comboBridgeNo;
	CComboBox	m_comboCTC;
	CButton	m_btnExamine;
	CGridCtrlMng	m_Grid;
	CXTPTabCtrl	m_Tab;
	int		m_nCross;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputRebarGagak)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputRebarGagak)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnRadioCross();
	afx_msg void OnRadioPlane();
	//}}AFX_MSG
	void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	void OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result);
	DECLARE_MESSAGE_MAP()
private:
	BOOL IsLeft();
	BOOL IsStt();
	void SetDataDefault();
	long m_nTab;
	void SetControl();
	void InitComboCtrl();
	void InitTabCtrl();
	void OnPreInitDialog();
	void DrawInputDomyunView(BOOL bZoomAll=FALSE);
	void SetGridData();
	void SetDataInit();
	void SetDataSave();
	CStringArray m_sArrTabName;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTREBARGAGAK_H__EB3F9CEB_6102_49B2_907D_3F55950BABB7__INCLUDED_)
