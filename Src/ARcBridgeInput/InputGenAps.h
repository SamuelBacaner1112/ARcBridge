// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTGENAPS_H__755B88E3_0A04_42B6_8D9D_3D368852FE6E__INCLUDED_)
#define AFX_INPUTGENAPS_H__755B88E3_0A04_42B6_8D9D_3D368852FE6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputGenAps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputGenAps dialog
class CInputGenAps : public CInputDialogBase
{
// Construction
public:
	void SyncFckFy();
	CInputGenAps(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputGenAps();

	BOOL IsValid();

// Dialog Data
	//{{AFX_DATA(CInputGenAps)
	enum { IDD = IDD_GEN_APS };
	CButton	m_checkStt;
	CButton	m_checkEnd;
	CButton	m_checkBracket;
	CButton	m_checkDivision;
	CXTPComboBox	m_comboCns;
	CButton	m_btnDefault2;
	CButton	m_checkWing;
	CButton	m_checkOrtho;
	CXTPComboBox	m_comboBridgeNo;
	CGridCtrlMng	m_Grid;
	CXTPTabCtrl	m_Tab;
	CString	m_sFck;
	CString	m_sFy;
	CButton	m_btnExamine;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputGenAps)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	CInputCheckEdit *m_pDlgCheck;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputGenAps)
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCheckOrtho();
	afx_msg void OnCheckWing();
	afx_msg void OnButtonDefaultInput2();
	afx_msg void OnSelchangeComboArea();
	afx_msg void OnSelchangeComboCns();
	afx_msg void OnCheckDivision();
	afx_msg void OnCheckBracket();
	afx_msg void OnCheckEnd();
	afx_msg void OnCheckStt();
	afx_msg void OnChangeEditFck();
	afx_msg void OnChangeEditFy();
	afx_msg void OnKillfocusEditFck();
	afx_msg void OnKillfocusEditFy();
	afx_msg void OnButtonExamine();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
private:
	BOOL IsAps();	// 접속슬래브가 하나라도 있는 경우 TRUE
	BOOL IsStt();
	BOOL IsLeft();
	long GetCountCns();
	CApsApp* GetCns(long nIdx);
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
	long m_nTabEnd[2];
	
	CStringArray m_sArrTabName;

	CTwinVectorArray m_tvArrApsSection;
	BOOL m_bMove;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTGENAPS_H__755B88E3_0A04_42B6_8D9D_3D368852FE6E__INCLUDED_)
