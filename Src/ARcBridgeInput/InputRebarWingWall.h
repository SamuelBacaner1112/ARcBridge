// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTREBARWINGWALL_H__FBB694E1_AD62_4DF6_9EF4_8A4AAC6CD853__INCLUDED_)
#define AFX_INPUTREBARWINGWALL_H__FBB694E1_AD62_4DF6_9EF4_8A4AAC6CD853__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputRebarWingWall.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputRebarWingWall dialog

class CInputRebarWingWall : public CInputDialogBase
{
// Construction
public:
	CInputRebarWingWall(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputRebarWingWall();

	BOOL IsValid();

// Dialog Data
	//{{AFX_DATA(CInputRebarWingWall)
	enum { IDD = IDD_REBAR_WINGWALL };
	CXTPComboBox	m_cbSelect;
	CXTPComboBox	m_cbBridgeNo;
	CXTPComboBox	m_CBPlan;
	CButton	m_btnExamine;
	CButton	m_btnOption;
	CXTPTabCtrl	m_Tab;
	CGridCtrlMng	m_Grid;
	int	m_nSelComboPlan;
	long m_nCountUpper[2][2][2][2]; //시종점/좌우측/전면배면/1단2단
	long m_nCountLower[2][2][2][2];
	long m_nCountSide[2][2][2][2];
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputRebarWingWall)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	CInputCheckGrid *m_pDlgCheck;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputRebarWingWall)
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnSelchangeComboSelect();
	afx_msg void OnButtonOption();
	afx_msg void OnButtonExamine();
	afx_msg void OnSelchangeComboPlan();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	DECLARE_MESSAGE_MAP()
private:
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnPreInitDialog();
	void InitComboCtrl();
	void InitComboSelect();
	void InitTabCtrl();
	void SetDataDefault();
	void SetDataDefaultBlock(BOOL bStt, BOOL bLeft, BOOL bOut, long nDan);
	void SetDataInit();
	void SetDataSave();
	void SetEnableWindow();
	void SetGridData();

	void OnSelchangeComboUpperCount();
	void OnSelchangeComboLowerCount();
	void OnSelchangeComboSideCount();
public:
	void KillFocusDlg();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTREBARWINGWALL_H__FBB694E1_AD62_4DF6_9EF4_8A4AAC6CD853__INCLUDED_)
