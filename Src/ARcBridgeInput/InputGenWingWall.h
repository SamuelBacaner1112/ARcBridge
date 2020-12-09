// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTGENWINGWALL_H__ACC465B2_BA5E_42CA_BF4D_F80882707EB8__INCLUDED_)
#define AFX_INPUTGENWINGWALL_H__ACC465B2_BA5E_42CA_BF4D_F80882707EB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputGenWingWall.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputGenWingWall dialog

class CInputGenWingWall : public CInputDialogBase
{
// Construction
public:
	CInputGenWingWall(CWnd* pParent = NULL);   // standard constructor

	BOOL IsValid();

// Dialog Data
	//{{AFX_DATA(CInputGenWingWall)
	enum { IDD = IDD_GEN_WINGWALL };
	CXTPComboBox	m_cbBridgeNo;
	CXTPTabCtrl	m_Tab;
	CGridCtrlMng	m_Grid;
	BOOL	m_bAutoWingEl;
	BOOL	m_bAutoHeightSoil;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputGenWingWall)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputGenWingWall)
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnCheckAutoEl();
	afx_msg void OnCheckAutoHeightSoil();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnButtonCalcSw();
	DECLARE_MESSAGE_MAP()
private:
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);

	BOOL IsOKMoveOtherDlg();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnPreInitDialog();
	void InitComboCtrl();
	void InitTabCtrl();
	void SetGridTitle();
	void SetGridData();
	void SetDataDefault();
	void SetDataInit();
	void SetDataSave();
	void SetRebarWingWall();
	void SetEnableWindow();

	long m_nRowPrev;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTGENWINGWALL_H__ACC465B2_BA5E_42CA_BF4D_F80882707EB8__INCLUDED_)
