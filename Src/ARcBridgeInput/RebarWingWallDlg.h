// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_REBARWINGWALLDLG_H__5ADF58AD_908A_4FD6_9CE9_7ED88DD09116__INCLUDED_)
#define AFX_REBARWINGWALLDLG_H__5ADF58AD_908A_4FD6_9CE9_7ED88DD09116__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RebarWingWallDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRebarWingWallDlg dialog

class CARcBridgeInputStd;

class CRebarWingWallDlg : public CDialog
{
// Construction
public:
	CRebarWingWallDlg(CARcBridgeInputStd* pIn, BOOL bStt, BOOL bLeft, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRebarWingWallDlg)
	enum { IDD = IDD_DIALOG_REBAR_WINGWALL };
	CXTHyperLink	m_hyperHelp;
	CEdit	m_edtBInD;
	CXTPComboBox	m_cbRebarCurve;
	CXTPComboBox	m_cbSlopeBlockOut;
	CXTPComboBox	m_cbSlopeBlockIn;
	CXTPComboBox	m_cbHunchCTC;
	CXTPComboBox	m_cbStdVerCTC;
	CXTPComboBox	m_cbMainCTCB;
	CXTPComboBox	m_cbMainCTCA;
	CXTPComboBox	m_cbShearBind;
	CXTPComboBox	m_cbRebarShearBC;
	CButton	m_btnRecommand;
	CButton	m_btnOK;
	CGridCtrlMng m_Grid;
	CXTPTabCtrl m_ctlTab;
	BOOL	m_bSelMainCTCD;
	BOOL	m_bUpperRebarSlope;
	BOOL	m_bAddRebarReinforce;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRebarWingWallDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private :
	CARcBridgeInputStd* m_pIn;
	BOOL m_bStt;
	BOOL m_bLeft;
	CString strBlank;
	long m_nTab;

	CStringArray m_szArr;
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRebarWingWallDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonDefault();
	afx_msg void OnCheckUpperRebarSlope();
	afx_msg void OnSelchangeComboRebarShearBc();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnSelchangeTab(NMHDR* pNotifyStruct, LRESULT* result);
	DECLARE_MESSAGE_MAP()

private:
	void InitTabCtrl();
	void SetComboInit();
	void SetDataDefault();
	void SetDataInit();
	void SetDataSave();
	void SetGridData();
	void SetGridData_Rebar();
	void SetGridData_Dan();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REBARWINGWALLDLG_H__5ADF58AD_908A_4FD6_9CE9_7ED88DD09116__INCLUDED_)
