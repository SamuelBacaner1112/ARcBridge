// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTBASESHOEPOSITION_H__947F80BD_803F_4F8E_B1B0_008481F0C9CD__INCLUDED_)
#define AFX_INPUTBASESHOEPOSITION_H__947F80BD_803F_4F8E_B1B0_008481F0C9CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputBaseShoePosition.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputBaseShoePosition dialog

class CInputBaseShoePosition : public CInputDialogBase
{
// Construction
public:
	long GetNumberVerPosByTabIndex(long nTab);
	CInputBaseShoePosition(CWnd* pParent = NULL);   // standard constructor

	BOOL IsValid();

// Dialog Data
	//{{AFX_DATA(CInputBaseShoePosition)
	enum { IDD = IDD_BASE8_SHOE_POSITION };
	int				m_nCrossSect;
	int				m_nTangent;
	CGridCtrlMng		m_Grid;
	CButton		m_btFixShoeSelect;
	CButton		m_btSameApply;
	CXTPComboBox	m_cbBridgeNo;
	CXTPComboBox	m_cbShoeEa;
	BOOL	m_bViewAllDir;
	BOOL	m_bUsePadShoe;
	//}}AFX_DATA
	CXTPTabCtrl	m_ctlTab;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputBaseShoePosition)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputBaseShoePosition)
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnRadioPlane();
	afx_msg void OnRadioTangent();
	afx_msg void OnRadioFixedShoe();
	afx_msg void OnRadioRightAng();
	afx_msg void OnRadioCrossSect();
	afx_msg void OnRadioCurrent();
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnButtonSelectShoeFix();
	afx_msg void OnButtonSameApply();
	afx_msg void OnCheckViewCounterDir();
	afx_msg void OnCheckPadShoe();
	afx_msg void OnSelchangeComboShoeEa();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	long GetNumberJijumByTabIndex(long nTab);
	void SetEnableWindow();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnPreInitDialog();
	void InitComboCtrl();
	void SetGridTitle();
	void SetDataDefault();
	void SetDataInit();
	void SetDataSave();
	void InitTabCtrl();
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTBASESHOEPOSITION_H__947F80BD_803F_4F8E_B1B0_008481F0C9CD__INCLUDED_)
