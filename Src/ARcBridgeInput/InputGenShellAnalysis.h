// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTGENSHELLANALYSIS_H__EDD499A3_D8DE_47F9_B87E_ED28DFC636D1__INCLUDED_)
#define AFX_INPUTGENSHELLANALYSIS_H__EDD499A3_D8DE_47F9_B87E_ED28DFC636D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputGenShellAnalysis.h : header file
//

#include "MidasInputProgressDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CInputGenShellAnalysis dialog

class CInputGenShellAnalysis : public CInputGenAnalysisBaseShell,
                               public CThreadWork
{
// Construction
public:
	BOOL IsValid();
	CInputGenShellAnalysis(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputGenShellAnalysis)
	enum { IDD = IDD_GEN_SHELL_ANLYSIS };
	CButton	m_btnViewSetting;
	BOOL	m_bCheckAllBridge;
	BOOL	m_bKorCode;
	//}}AFX_DATA
	
	CString m_szInputPath;

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
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnButtonRunSap2000();
	afx_msg void OnButtonLoadSap();
	afx_msg void OnButtonMakeSap();
	afx_msg void OnButtonViewCalc();
	afx_msg void OnSelchangeTabDesignType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTabDesignType(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnMidasLoadComplete(WPARAM, LPARAM);
	//}}AFX_MSG
	afx_msg LRESULT OnCalcOk(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
		
private:
	void SetEnableControl();
	int m_nTypeDesign;
	void SetDataInit();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnPreInitDialog();
	void InitComboCtrl();
	void SetLoadComboCombo();
	void SetLoadCaseCombo();	
	void InitComboCtrlToolName();

	UINT DoWork();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTGENSHELLANALYSIS_H__EDD499A3_D8DE_47F9_B87E_ED28DFC636D1__INCLUDED_)
