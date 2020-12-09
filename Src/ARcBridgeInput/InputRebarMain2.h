// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTREBARMAIN2_H__89B85719_965B_4180_B4B6_77248AF12C8C__INCLUDED_)
#define AFX_INPUTREBARMAIN2_H__89B85719_965B_4180_B4B6_77248AF12C8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputRebarMain2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputRebarMain2 dialog

class CInputRebarMain2 : public CInputDialogBase
{
// Construction
public:
	CInputRebarMain2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputRebarMain2();

// Dialog Data
	//{{AFX_DATA(CInputRebarMain2)
	enum { IDD = IDD_REBAR_MAIN2 };
	CButton	m_checkBending;
	CButton	m_checkViewCycle1;
	CButton	m_check2Dan;
	CStatic	m_staticRib;
	CXTPComboBox	m_comboDan;
	CButton	m_checkRebarLength;
	CButton	m_checkJoint;
	CButton	m_btnRebarDan;
	CButton	m_btnSameSttEnd;
	CButton	m_btnSameInWall;
	CButton	m_checkBMD;
	CButton	m_btnRebarJoint;
	CButton	m_btnExamine;
	CButton	m_checkMark;
	CXTPComboBox	m_comboBridgeNo;
	CXTPComboBox	m_comboCTC;
	CGridCtrlMng	m_Grid;
	CXTPTabCtrl	m_TabCycle;
	CTabCtrl	m_Tab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputRebarMain2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	CInputCheckGrid *m_pDlgCheck;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputRebarMain2)
	afx_msg void OnCheckAllmark();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnButtonRebarjoint();
	afx_msg void OnCheckBmd();
	afx_msg void OnButtonExamine();
	afx_msg void OnButtonSameinwall2();
	afx_msg void OnButtonSamesttend2();
	afx_msg void OnButtonRebardan();
	afx_msg void OnCheckJoint();
	afx_msg void OnCheckRebarLength();
	afx_msg void OnSelchangeComboDan();
	afx_msg void OnCheck2dan();
	afx_msg void OnCheckViewCycle1();
	afx_msg void OnCheckBending();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	void OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result);

public:
	void ApplySameShapeSttAndEnd();
	void ApplySameShapeInWall(long nIdxInWallStd);
	CCellID m_CellID;
	long GetIdxJijumCurFooting();
	void SyncRebar();
	long GetCurCycle();
	void SetGridDataCycle24_Footing(long nCycle);
	void SetGridData_InWall(long nCycle, CGridCtrlMng *pGrid=NULL, long nDan=0);
	void SetGridData_OutWall(long nCycle, CGridCtrlMng *pGrid=NULL, long nDan=0);
	void SetGridData_UpperSlab(long nCycle, CGridCtrlMng *pGrid=NULL, long nDan=0);
	void SetGridData_LowerSlab(long nCycle, CGridCtrlMng *pGrid=NULL, long nDan=0);
	void SetGridData_ArchRib(long nCycle);

	void SetGridData_OutWall2Dan(long nCycle);

	void DrawInputDomyunView(BOOL bZoomAll=TRUE);
	long m_nTab;
	long m_nTabCycle;
	void InitComboCtrl();
	void OnPreInitDialog();
	void InitTabCtrl();
	void SetDataDefault();
	void SetDataSave();
	void SetGridData();
	void SetDataInit();
	void SetControl();

	long m_nTabCycle2;
	long m_nTabCycle4;

	long m_nTabUpperSlab;
	long m_nTabLowerSlab;
	long m_nTabOutWall;
	long m_nTabInWall;
	long m_nTabFooting;
	long m_nTabArchRib;
	
	CStringArray m_sArrTabName;

public:
	BOOL IsSet2Dan();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTREBARMAIN2_H__89B85719_965B_4180_B4B6_77248AF12C8C__INCLUDED_)
