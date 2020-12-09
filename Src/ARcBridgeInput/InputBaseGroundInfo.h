// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTBASEGROUNDINFO_H__3ACF3243_7664_4004_82A3_E4F9BA2CA5D5__INCLUDED_)
#define AFX_INPUTBASEGROUNDINFO_H__3ACF3243_7664_4004_82A3_E4F9BA2CA5D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputBaseGroundInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputBaseGroundInfo dialog

//Áö¹Ý
#define MAX_BORING	100
#define MAX_JIBAN_COUNT 20

#include "BoringTypeDlg.h"
#include "EValueInputDlg.h"
#include "UValueInputDlg.h"
#include "MsValueInputDlg.h"
#include "AlphaEValueInputDlg.h"

class CInputBaseGroundInfo : public CInputDialogBase
{
// Construction
public:
	CInputBaseGroundInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputBaseGroundInfo)
	enum { IDD = IDD_BASE5_GROUNDINFO };
	CString			m_strBoringName;
	CXTPComboBox	m_cbGroundInfoNum;
	CXTPComboBox	m_cbBoringNo;
	CXTPTabCtrl		m_ctlTab;
	CGridCtrlMng		m_Grid;
	CString	m_szEL;
	CString	m_szStation;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputBaseGroundInfo)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputBaseGroundInfo)
	afx_msg void OnSelchangeComboBoring();
	afx_msg void OnSelchangeComboGroundinfonum();
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnKillfocusEditBoringName();
	afx_msg void OnKillfocusEditBoringEL();
	afx_msg void OnKillfocusEditBoringStation();
	afx_msg void OnButtonNext();
	afx_msg void OnButtonPrev();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL CheckEoAndPoisonRate();
	double m_dVScaleDraw;
	CBoringTypeDlg m_dlgBoringDlg;
	CEValueInputDlg m_dlgEValueDlg;
	CUValueInputDlg m_dlgUValueDlg;
	CAlphaEValueInputDlg m_dlgBoringAlphaEDlg;
	CMsValueInputDlg m_dlgBoringMsDlg;
	
	void SetDataDefault();
	void SetDataDefaultBoringInfo(int nBoring);
	void OnPreInitDialog();
//	void ClearBoringInfoData(int nBoring);
	void InitComboBoring();	
	void InitGroundInfoComboCtrl();
	void InitTabCtrl();
	void SetGridTitle();
	void SetDataInit();
	void SetDataSave();

	void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	void DrawCursor(CDomyun* pDom, int nRow);
	void DrawVertGroundEL(CDomyun* pDom);
	void DrawLineVertical(CDomyun *pDomP);
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);

	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTBASEGROUNDINFO_H__3ACF3243_7664_4004_82A3_E4F9BA2CA5D5__INCLUDED_)
