// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTGENSODAN_H__4355D3A1_D05F_403A_A6EE_481968CA5D0F__INCLUDED_)
#define AFX_INPUTGENSODAN_H__4355D3A1_D05F_403A_A6EE_481968CA5D0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputGenSodan.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputGenSodan dialog

class CInputGenSodan : public CInputDialogBase
{
// Construction
public:
	CInputGenSodan(CWnd* pParent = NULL);   // standard constructor

	BOOL IsValid();

// Dialog Data
	//{{AFX_DATA(CInputGenSodan)
	enum { IDD = IDD_GEN_SODAN };
	CXTPComboBox	m_cbBridgeNo;
	CGridCtrlMng	m_Grid;
	CXTPTabCtrl	m_Tab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputGenSodan)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputGenSodan)
	afx_msg void OnSelchangeComboBridgeNo();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result);
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnPreInitDialog();
	void InitComboCtrl();
	void InitTabCtrl();
	void SetDataDefault();
	void SetDataInit();
	void SetDataSave();

	long m_nRowPrev;
	long m_nTab;
	BOOL m_bStt;
	BOOL m_bEnd;
	void SetGridData();
	void SetGridDataSoDan(long row, long col, CWingWall *pWingWall);

	void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);

public:
	void SetFocusItem(CString szTitle);
	BOOL IsFocusItem(CString &szTitle);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTGENSODAN_H__4355D3A1_D05F_403A_A6EE_481968CA5D0F__INCLUDED_)
