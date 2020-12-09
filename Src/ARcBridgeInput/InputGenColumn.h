// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTGENCOLUMN_H__F23A375D_B24E_4130_A5E6_BB1FA6734DED__INCLUDED_)
#define AFX_INPUTGENCOLUMN_H__F23A375D_B24E_4130_A5E6_BB1FA6734DED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputGenColumn.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputGenColumn dialog
class CInputGenColumn : public CInputDialogBase
{
// Construction
public:
	CWallApp* GetCurInWall();
	CInputGenColumn(CWnd* pParent = NULL);   // standard constructor

	BOOL IsValid();

// Dialog Data
	//{{AFX_DATA(CInputGenColumn)
	enum { IDD = IDD_GEN_COLUMN };
	CXTPTabCtrl	m_Tab;
	CXTPComboBox	m_comboBridgeNo;
	CGridCtrlMng	m_Grid;
	CGridCtrlMng	m_GridSection;
	int		m_nViewCross;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputGenColumn)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputGenColumn)
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnSelchangeComboInwall();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRadioCross();
	afx_msg void OnRadioPlane();
	//}}AFX_MSG
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	void OnCellChangedSection(NMHDR* pNotifyStruct, LRESULT* result);
	void OnCellChangedDataSection(NMHDR* pNotifyStruct, LRESULT* result);
	DECLARE_MESSAGE_MAP()
private:
	long GetIdxCurInWall();
	void SetGridDataColumnSection();
	void SetGridDataColumnDist();
	void DrawInputDomyunView(BOOL bZoomAll=TRUE);
	long m_nTab;
	long m_nTabInWall[MAX_JIJUM_COUNT_RC];
	void InitComboCtrl();
	void OnPreInitDialog();
	void InitTabCtrl();
	void SetDataDefault();
	void SetDataSave();
	void SetGridData();
	void SetDataInit();
	void SetControl();

	CStringArray m_sArrTabName;

	BOOL m_bMove;

public:
	void SetFocusItem(CString szTitle);
	BOOL IsFocusItem(CString &szTitle);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTGENCOLUMN_H__F23A375D_B24E_4130_A5E6_BB1FA6734DED__INCLUDED_)
