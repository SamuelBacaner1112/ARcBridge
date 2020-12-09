// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MidasInputProgressDlg.h"

// CInputGenWingShellAnalysis 대화 상자입니다.

class CInputGenWingShellAnalysis :	public CInputGenAnalysisBaseShell,
									public CThreadWork
{

public:
	BOOL IsValid();
	CInputGenWingShellAnalysis(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CInputGenWingShellAnalysis();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_GEN_WING_SHELL_ANLYSIS };
	CButton	m_btnViewSetting;
	CXTPComboBox	m_cbSelectWing;
	CComboBox	m_ComboMeshSize;
	//}}AFX_DATA
	CString m_szInputPath;
	long	m_nTypeDesign;

	CMidasInputProgressDlg m_MidasProgressDlg;
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputGenShellAnalysis)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputGenShellAnalysis)
	afx_msg void OnSelchangeComboSelectWing();
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnButtonRunSap2000();
	afx_msg void OnButtonLoadSap();
	afx_msg void OnButtonMakeSap();	
	afx_msg void OnSelchangeComboLoadcase();
	afx_msg void OnSelchangeComboLoadtype();
	afx_msg void OnButtonViewCalc();
	afx_msg void OnSelchangeTabDesignType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTabDesignType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnMidasLoadComplete(WPARAM, LPARAM);
	afx_msg void OnSelchangeComboMeshSize();
	//}}AFX_MSG
	afx_msg LRESULT OnCalcOk(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

private:
	void SetEnableControl();
	BOOL m_bStt;
	BOOL m_bLeft;
	void SetDataInit();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnPreInitDialog();
	void InitComboCtrl();
	void SetLoadComboCombo();
	void SetLoadCaseCombo();
	void InitWingComboCtrl();
	
	UINT DoWork();
};
