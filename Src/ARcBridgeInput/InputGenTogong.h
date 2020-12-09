// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTGENTOGONG_H__73B9BAB6_5771_4EB4_9A51_E5DA53A0192D__INCLUDED_)
#define AFX_INPUTGENTOGONG_H__73B9BAB6_5771_4EB4_9A51_E5DA53A0192D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputGenTogong.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputGenTogong dialog

class CInputGenTogong : public CInputDialogBase
{
// Construction
public:
	CInputGenTogong(CWnd* pParent = NULL);   // standard constructor

	BOOL IsValid();

// Dialog Data
	//{{AFX_DATA(CInputGenTogong)
	enum { IDD = IDD_GEN_TOGONG };
	CXTPComboBox	m_cbBridgeNo;
	CXTPComboBox m_cbViewDom;
	CGridCtrlMng	m_Grid;
	CXTPTabCtrl	m_Tab;
	CXTPComboBox	m_cbDiggingPos;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputGenTogong)
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
	afx_msg void OnSelchangeComboViewDom();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnSelchangeComboDiggingPos();
	DECLARE_MESSAGE_MAP()

private:
	long GetJijum();
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnPreInitDialog();
	void InitComboCtrl();
	void InitTabCtrl();
	void SetGridData();
	void SetDataDefault();
	int	m_nTab;
	long m_nTabWallStt;
	long m_nTabWallEnd;
	long m_nTabInWall[MAX_JIJUM_COUNT_RC];

	int m_nViewDom;

	void SetDataSave();
	void SetDataInit();
	
	void SetGridDataAtJijum(long nJijum);
	void SetCurSelRubble();
	void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	CStringArray m_sArrTabName;

public:
	void SetFocusItem(CString szTitle);
	BOOL IsFocusItem(CString &szTitle);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTGENTOGONG_H__73B9BAB6_5771_4EB4_9A51_E5DA53A0192D__INCLUDED_)
