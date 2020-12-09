// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTREBARMAIN3_H__AA30476C_D26A_4C04_BB04_9DBEB4068AC3__INCLUDED_)
#define AFX_INPUTREBARMAIN3_H__AA30476C_D26A_4C04_BB04_9DBEB4068AC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputRebarMain3.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputRebarMain3 dialog

class CInputRebarMain3 : public CInputDialogBase
{
// Construction
public:
	void SetGridData_AcuteAngle();
	void SetGridData_ObtuseAngle();
	void SetGridData_Stirrup();
	CInputRebarMain3(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputRebarMain3();

// Dialog Data
	//{{AFX_DATA(CInputRebarMain3)
	enum { IDD = IDD_REBAR_MAIN3 };
	CXTPComboBox	m_comboAcute;
	CXTPComboBox	m_comboObtuse;
	CXTPComboBox	m_comboDanbu;
	CButton m_btnExamine;
	CXTPComboBox	m_comboCTC;
	CXTPComboBox	m_comboBridgeNo;
	CGridCtrlMng	m_Grid;
	CXTPTabCtrl	m_Tab;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputRebarMain3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	CInputCheckGrid *m_pDlgCheck;
	CInputCheckEdit *m_pDlgEdit;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputRebarMain3)
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnButtonExamine();
	afx_msg void OnSelchangeComboDanbu();
	afx_msg void OnSelchangeComboObtuse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	void OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result);

private:
	long GetCycle();
	long GetJijum();
	long GetSlab();

	CCellID m_CellID;
	void SyncRebar();
	void SetGridData_Bracket();
	void SetGridData_BFooting();
	void SetGridData_Danbu();
	void SetGridData_Hunch();
	void DrawInputDomyunView(BOOL bZoomAll=TRUE);
	long m_nTab;
	void InitComboCtrl();
	void OnPreInitDialog();
	void InitTabCtrl();
	void SetDataDefault();
	void SetDataSave();
	void SetGridData();
	void SetDataInit();
	void SetControl();

	long m_nTabHunch;
	long m_nTabDanbu;
	long m_nTabBracket;
	long m_nTabBFooting;	// 부상방지저판
	long m_nTabStirrup;		// 단 절점부 스터럽
	long m_nTabObtuseAngle;	// 둔각부 보강
	long m_nTabAcuteAngle;	// 벽체 예각부 보강

	CStringArray m_sArrTabName;
public:
	afx_msg void OnBnClickedButtonCorner();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTREBARMAIN3_H__AA30476C_D26A_4C04_BB04_9DBEB4068AC3__INCLUDED_)
