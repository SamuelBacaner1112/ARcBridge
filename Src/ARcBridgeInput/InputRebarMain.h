// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTREBARMAIN_H__DF44B1F9_0BAD_4787_B471_35A5D2FFA73E__INCLUDED_)
#define AFX_INPUTREBARMAIN_H__DF44B1F9_0BAD_4787_B471_35A5D2FFA73E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputRebarMain.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputRebarMain dialog
class CInputCheckGrid;
class CDlgProgress;
class CInputCheckArchRib;

class CInputRebarMain : public CInputDialogBase
{
// Construction
public:
	CInputRebarMain(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputRebarMain();

// Dialog Data
	//{{AFX_DATA(CInputRebarMain)
	enum { IDD = IDD_REBAR_MAIN };
	CButton	m_checkViewCycle2;
	CButton	m_check2Dan;
	CButton	m_btnCheckRib;
	CStatic	m_staticRib;
	CXTPComboBox	m_comboDan;
	CButton	m_checkRebarLength;
	CButton	m_btnOptimize;
	CButton	m_checkJoint;
	CButton	m_btnSameSttEnd;
	CButton	m_btnSameInWall;
	CButton	m_checkBMD;
	CButton	m_btnTypeCycle3;
	CXTPComboBox	m_comboCTC;
	CButton	m_btnRebarJoint;
	CButton	m_btnRebarDan;
	CButton		m_checkMark; 
	CButton	m_btnExamine;
	CXTPComboBox	m_comboBridgeNo;
	CGridCtrlMng	m_Grid;
	CTabCtrl	m_Tab;
	CXTPTabCtrl	m_TabCycle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputRebarMain)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	CInputCheckGrid *m_pDlgCheck;
	CInputCheckArchRib *m_pDlgCheckRib;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputRebarMain)
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnButtonRebarjoint();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonRebardan();
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckAllmark();
	afx_msg void OnSelchangeComboCtc();
	afx_msg void OnButtonTypecycle3();
	afx_msg void OnCheckBmd();
	afx_msg void OnButtonExamine();
	afx_msg void OnButtonSameinwall2();
	afx_msg void OnButtonSamesttend2();
	afx_msg void OnCheckJoint();
	afx_msg void OnButtonOptimize();
	afx_msg void OnCheckRebarLength();
	afx_msg void OnCellInsertedCol(NMHDR* pNotifyStruct, LRESULT* result);	
	afx_msg void OnSelchangeComboDan();
	afx_msg void OnButtonCheckrib();
	afx_msg void OnCheck2dan();
	afx_msg void OnCheckViewCycle2();
	afx_msg void OnButtonRebarSupport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTabSelchangingCycle(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchangeCycle(NMHDR* pNMHDR, LRESULT* pResult);
	void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	void OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result);

private:
	void Empty2Cycle(CConcBasic *pConc);
	BOOL IsConsider2Cycle(CConcBasic *pConc, BOOL bRev=FALSE);
	long GetIdxDia(double Dia);
	BOOL CheckBeamForOptimize(CConcBasic *pConc);
	void OptimizeBeam(CConcBasic *pConc, double dCtcMain,BOOL bRev=FALSE, BOOL bFooting=FALSE);
	void OptimizeWall(CConcBasic *pConc);
	void OptimizeRebarMiddleWall();
	void OptimizeRebarOutWall();
	void OptimizeRebarArchRib();
	void OptimizeRebarSlab();
	void ApplySameShapeSttAndEnd();
	void ApplySameShapeInWall(long nIdxInWallStd);
	CCellID m_CellID;
	long GetIdxJijumCurFooting();
	long GetCurCycle();
	void SyncRebar();
	void SetGridData_ArchRib(long nCycle);
	void SetGridData_InWall(long nCycle);
	void SetGridDataCycle13_Footing(long nCycle);
	void SetGridData_OutWall(long nCycle);
	void SetGridData_UpperSlab(long nCycle);
	void SetGridData_LowerSlab(long nCycle);
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

	long m_nTabCycle1;
	long m_nTabCycle2;
	long m_nTabCycle3;
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
	void SetGridData_OutWall2Dan(long nCycle);
	CDlgProgress* m_pProgress;
	void SyncOptimizeRebarMain();
	void OptimizeRebarFooting();
	void SetRebarSymmetry();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTREBARMAIN_H__DF44B1F9_0BAD_4787_B471_35A5D2FFA73E__INCLUDED_)
