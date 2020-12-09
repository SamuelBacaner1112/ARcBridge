// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTREBARAPS_H__EE819B26_0642_42DF_B5B7_86E7CAF4966D__INCLUDED_)
#define AFX_INPUTREBARAPS_H__EE819B26_0642_42DF_B5B7_86E7CAF4966D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputRebarAps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputRebarAps dialog

class CInputRebarAps : public CInputDialogBase
{
// Construction
public:
	CInputRebarAps(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputRebarAps();

	BOOL IsValid();
	BOOL		m_bCheckDowelRebarSep;
// Dialog Data
	//{{AFX_DATA(CInputRebarAps)
	enum { IDD = IDD_REBAR_APS };
	CButton	m_btnSetting;
	CXTPComboBox	m_comboBridgeNo;
	CButton	m_btnExamine;
	CXTPTabCtrl	m_Tab;
	CGridCtrlMng	m_Grid;
	//}}AFX_DATA
	
	CInputCheckEdit *m_pDlgCheck;
	CInputCheckGrid *m_pDlgCheckGrid;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputRebarAps)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputRebarAps)
	afx_msg void OnButtonExamine();
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnCheckSameoffset();
	afx_msg void OnButtonSetting();
	//}}AFX_MSG
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCheckDowelRebarSep();
	DECLARE_MESSAGE_MAP()
private:
	BOOL IsAps();
	void SetDataDefaultRebarApsStd(CApsApp *pAps);
	CApsApp* GetAps();
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
	long m_nTabStt[2];
	long m_nTabSttCns[2][MAX_CNS];
	long m_nTabEnd[2];
	long m_nTabEndCns[2][MAX_CNS];
	
	CStringArray m_sArrTabName;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTREBARAPS_H__EE819B26_0642_42DF_B5B7_86E7CAF4966D__INCLUDED_)
