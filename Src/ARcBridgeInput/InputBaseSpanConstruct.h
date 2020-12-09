// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTBASESPANCONSTRUCT_H__577166DC_DBD2_421C_B74B_41B4F8E4557D__INCLUDED_)
#define AFX_INPUTBASESPANCONSTRUCT_H__577166DC_DBD2_421C_B74B_41B4F8E4557D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputBaseSpanConstruct.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputBaseSpanConstruct dialog
#define DEFAULT_BASESPANCONSTRUCT_MAKEJIJUM		0x00000001
#define DEFAULT_BASESPANCONSTRUCT_JIJUMANGLE		0x00000002
#define DEFAULT_BASESPANCONSTRUCT_JIJUMLENGTH	0x00000004
#define DEFAULT_BASESPANCONSTRUCT_GAGAK			0x00000008
#define DEFAULT_BASESPANCONSTRUCT_EXPWIDTH		0x00000010
#define DEFAULT_BASESPANCONSTRUCT_DISTSHOE		0x00000020
#define DEFAULT_BASESPANCONSTRUCT_EXPJOINT		0x00000040

class CInputBaseSpanConstruct : public CInputDialogBase
{
// Construction
public:
	BOOL CheckAngle();
	void KillFocusDlg();
	CInputBaseSpanConstruct(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputBaseSpanConstruct)
	enum { IDD = IDD_BASE7_SPAN_CONSTRUCT };
	CXTPTabCtrl	m_ctlTab;
	CButton	m_btSameAngle;
	CButton	m_btSameExpJoint;
	CButton	m_btnAutoRadius;
	CGridCtrlMng	m_Grid;
	CXTPComboBox	m_cbBridgeNo;
	BOOL	m_bViewAllDir;
	BOOL	m_bExp;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputBaseSpanConstruct)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputBaseSpanConstruct)
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnButtonSameAngle();
	afx_msg void OnButtonSameExpJoint();
	afx_msg void OnButtonAutoRadiusGagak();
	afx_msg void OnCheckViewCounterDir();
	afx_msg void OnCheckExp();
	afx_msg void OnButtonCheckExpJoint();
	afx_msg void OnButtonRefExpJoint();
	afx_msg void OnBnClickedButtonCalcExpansion();

	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnExpansionRecommend(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()

private:
	BOOL IsOKMoveOtherDlg();
	void SetControl();

	void SetGridData();
	void SetGridDataSpan();
	void SetGridDataJijumAngle();
	void SetGridDataGagak();
	void SetGridDataExtLine();
	void SetGridDataExpJoint();

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnPreInitDialog();

	void InitComboCtrl();
	void InitTabCtrl();
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();

	BOOL IsFocusItem(CString &szTitle);
	void SetFocusItem(CString szTitle);
	void OnCursorChange(long nRow, long nCol);

	BOOL CheckGagakLen();
	double GetLenMaxGagak(BOOL bStt, BOOL bLeft);
	CRect m_rcPane;
	CRect m_rcGridJijum;
	CRect m_rcGridJigan;

	long m_nTabAngle;
	long m_nTabSpan;
	long m_nTabGagak;
	long m_nTabExtLine;
	long m_nTabExpJoint;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTBASESPANCONSTRUCT_H__577166DC_DBD2_421C_B74B_41B4F8E4557D__INCLUDED_)
