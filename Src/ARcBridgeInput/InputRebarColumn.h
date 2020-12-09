// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTREBARCOLUMN_H__882443CE_C334_43F4_B39D_05ACBC780684__INCLUDED_)
#define AFX_INPUTREBARCOLUMN_H__882443CE_C334_43F4_B39D_05ACBC780684__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputRebarColumn.h : header file
//
#include "RoundDlg.h"
#include "StirrupDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CInputRebarColumn dialog
class CInputCheckGrid;

class CInputRebarColumn : public CInputDialogBase
{
// Construction
public:
	long GetIdxInputStirrup();
	long GetInfoGirder(BOOL &bStt, BOOL &bLeft, BOOL &bGagak, BOOL &bJongGirder);
	BOOL SetGridDataWall(long nJ);
	BOOL SetGridDataCrossTieAndRound();
	void KillFocusDlg();
	CARcBridgeInputStd* GetIn();
	CInputRebarColumn(CWnd* pParent = NULL);   // standard constructor
	~CInputRebarColumn();

	BOOL IsValid();

// Dialog Data
	//{{AFX_DATA(CInputRebarColumn)
	enum { IDD = IDD_REBAR_COLUMN };
	CButton	m_checkRadCrossTie;
	CXTPComboBox	m_comboInputColumn;
	CButton	m_checkMatchWidthWall;
	CGridCtrlMng	m_Grid;
	CXTPTabCtrl	m_TabRow;
	CTabCtrl	m_Tab;
	CButton	m_btnExamine;
	CXTPComboBox	m_comboBridgeNo;
	CString	m_sCtcGirder;
	BOOL	m_bViewPM;
	int		m_nViewCross;
	CStringArray m_sArrTabRow;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputRebarColumn)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	CInputCheckGrid *m_pDlgCheck;
	CInputCheckEdit *m_pDlgEdit;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputRebarColumn)
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusEditCtcGirder();
	afx_msg void OnButtonExamine();
	afx_msg void OnCheckMatchWithwall();
	afx_msg void OnCheckViewPM();
	afx_msg void OnSelchangeComboInputColumn();
	afx_msg void OnRadioCross();
	afx_msg void OnRadioPlane();
	afx_msg void OnCheckRadCrosstie();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	void OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result);
private:
	BOOL SetGridDataJongGirder();
	void DrawPM(CDomyun *pDomP);
	void ReInitTabRowCtrl();
	CWallApp* GetCurInWall();
	void SyncRebar(BOOL bUpperSlabGirder = TRUE);
	BOOL SetGridDataFooting();
	BOOL SetGridDataColumn();
//	long GetIdxCurHunch();
	BOOL SetGridDataUpperGirder(BOOL bUpperSlabGirder = TRUE);
	void DrawInputDomyunView(BOOL bZoomAll=TRUE);
	long m_nTab;
	long m_nTabRow;
	void InitComboCtrl();
	void OnPreInitDialog();
	void InitTabCtrl();
	void SetDataDefault();
	void SetDataSave();
	BOOL SetGridDataStd();
	void SetDataInit();
	void SetControl();
	
	CStringArray m_sArrTabName;

	long m_nTabUpperGirder;
	long m_nTabLowerGirder;
	long m_nTabFooting;
	long m_nTabColumn;
	long m_nTabWall;
	long m_nTabJongGirder;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTREBARCOLUMN_H__882443CE_C334_43F4_B39D_05ACBC780684__INCLUDED_)
