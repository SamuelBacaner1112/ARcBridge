// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTREBARSECTION_H__59A6E659_5B52_4517_AA78_1346256E836D__INCLUDED_)
#define AFX_INPUTREBARSECTION_H__59A6E659_5B52_4517_AA78_1346256E836D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputRebarSection.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputRebarSection dialog
class CDlgInputRebarShear;
class CInputRebarSection : public CInputDialogBase
{
// Construction
public:
	CInputRebarSection(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputRebarSection();

// Dialog Data
	//{{AFX_DATA(CInputRebarSection)
	enum { IDD = IDD_REBAR_SECTION };
	CXTPComboBox	m_comboCTCPart;
	CButton	m_btnSameSttEnd;
	CButton	m_btnSameInWall;
	CButton	m_checkBMD;
	CButton	m_checkSFD;
	CButton	m_checkSameOffset;
	CXTPComboBox	m_comboUpper;
	CXTPComboBox	m_comboLower;
	CXTPComboBox	m_comboCTC;
	CButton	m_btnRebarSupport;
	CButton	m_btnRebarShear;
	CButton	m_btnExamine;
	CButton	m_btnCheckCantil;
	CXTPComboBox	m_comboBridgeNo;
	CGridCtrlMng	m_Grid;
	CTabCtrl	m_Tab;
	CXTPTabCtrl	m_TabRebar;
	double	m_dAccu;
	CString	m_sPart;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputRebarSection)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	CInputCheckGrid *m_pDlgCheck;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputRebarSection)
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnButtonRebarSupport();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonRebarShear();
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboUpper();
	afx_msg void OnSelchangeComboLower();
	afx_msg void OnCheckSameoffsetTandc();
	afx_msg void OnSelchangeComboCtc();
	afx_msg void OnButtonDanbu();
	afx_msg void OnCheckSfd();
	afx_msg void OnCheckBmd1();
	afx_msg void OnButtonExamine();
	afx_msg void OnButtonSameinwall2();
	afx_msg void OnButtonSamesttend2();
	afx_msg void OnCellInsertedCol(NMHDR* pNotifyStruct, LRESULT* result);	
	afx_msg void OnCellDeletedCol(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnKillfocusEditAccu();
	afx_msg void OnSelchangeComboCtcPart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	void OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result);
private:
	void SetGridData_Bracket(BOOL bSupportRebar);
	BOOL m_bSyncBridge;
	double GetLengthPlaceByBlock(CPlaceRebarByBlock *pPlace);
	long GetJijumFooting();
	void ApplySameShapeSttAndEnd();
	void ApplySameShapeInWall(long nIdxInWallStd);
	CCellID m_CellID;
	long GetIdxCurFooting();
	void SyncRebar();
	BOOL m_bChangeCTC;
	BOOL IsWallType();
	BOOL m_bSamePlaceInWall;	// 내측벽체 배근을 통일한다.
	CPlaceRebarByBlock* GetPlaceRebarByBlock(BOOL bUpperOrInnerOrLeft);
	void SetCombo_Block();
	void SetGridData_Footing(long nIdx, BOOL bSupportRebar);
	void SetGridData_InWall(BOOL bSupportRebar);
	void SetGridData_OutWall(BOOL bStt, BOOL bSupportRebar);
	void SetGridData_UpperSlab(BOOL bSupportRebar);
	void SetGridData_LowerSlab(BOOL bSupportRebar);
	void DrawInputDomyunView(BOOL bZoomAll=TRUE);
	long m_nTab;
	long m_nTabRebar;
	void InitComboCtrl();
	void OnPreInitDialog();
	void InitTabCtrl();
	void SetDataDefault();
	void SetDataSave();
	void SetGridData();
	void SetDataInit();
	void SetControl();

	long m_nTabRebarSupport;
	long m_nTabRebarShear;

	long m_nTabUpperSlab;
	long m_nTabLowerSlab;
	long m_nTabOutWallStt;
	long m_nTabOutWallEnd;
	long m_nTabFooting[MAX_JIJUM_COUNT_RC];
	long m_nTabBracket;
	long m_nTabArchRib;
	long m_nTabArchJijumSupport;
	long m_nTabArchConnectStirrup;	
	long m_nTabInWall[MAX_JIJUM_COUNT_RC];//todo 31534
	
	CStringArray m_sArrTabName;

	double GetCtcShearBracket(long nIndex);

public:
	void InitComboCtrlPart();
	long GetTypeDefault();
	double* GetCTC(BOOL bSupport=FALSE);
	void SetCTCPart();
	double GetCTCPart();
	CString GetStringPart();
	void SetStringCTCPart();
	void SetGridData_ArchRibConnectSectionStirrup();
	void SetGridData_ArchJijumSupport();
	void SetGridData_ArchRib(BOOL bSupportRebar);
	afx_msg void OnBnClickedButtonCantilever();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTREBARSECTION_H__59A6E659_5B52_4517_AA78_1346256E836D__INCLUDED_)
