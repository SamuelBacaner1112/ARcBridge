// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTGENHYUNTA_H__C09ABD65_DD1E_4E53_AE48_F4BC6D68173C__INCLUDED_)
#define AFX_INPUTGENHYUNTA_H__C09ABD65_DD1E_4E53_AE48_F4BC6D68173C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputGenHyunTa.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputGenHyunTa dialog
class CInputCheckEdit; 

class CInputGenHyunTa : public CXTResizeDialog
{
// Construction
public:
	CInputGenHyunTa(CWnd* pParent = NULL);   // standard constructor
	~CInputGenHyunTa();

	BOOL IsValid();

// Dialog Data
	//{{AFX_DATA(CInputGenHyunTa)
	enum { IDD = IDD_DIALOG_HYUNTA };
	CXTPComboBox	m_comboBridgeNo;
	CXTPComboBox	m_comboFooting;
	CXTPComboBox	m_comboDan;
	CXTPComboBox	m_comboGugan;
	CButton	m_checkHoriz;
	CButton	m_btnExamine;
	
	CXTPTabCtrl	m_Tab;
	CGridCtrlMng	m_Grid;
	CButton	m_btnOK;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputGenHyunTa)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	CInputCheckEdit *m_pDlgCheck;
public:
	void SetDataStd(CARcBridgeInputStd* pInput);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputGenHyunTa)
	afx_msg void OnButtonExamine();
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnSelchangeComboFooting();
	afx_msg void OnSelchangeComboDan();
	afx_msg void OnSelchangeComboGugan();
	afx_msg void OnCheckHorizontal();

	//}}AFX_MSG
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	DECLARE_MESSAGE_MAP()
private:
	long m_nDan;
	long m_nGugan;
	BOOL m_bViewHoriz;

	CARcBridgeInputStd* m_pIn;

	CExPileApp* GetPile();
	long m_nTabRound;
	long m_nTabMain;

	long m_nTabCross;
	void SetGridDataCrossTie();

	void SetGridDataMain();
	void SetGridDataRound();
	void DrawInputDomyunView(BOOL bZoomAll=TRUE);
	long m_nTab;
	void InitComboCtrl();
	
	void InitTabCtrl();
	void SetDataDefault();
	void SetDataSave();
	void SetGridData();
	void SetDataInit();
	void SetControl();

	CStringArray m_sArrTabName;

private:
	CDomyunView m_DomView;
public:
	afx_msg void OnOK();
	afx_msg void OnBnClickedButtonRecommand();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTGENHYUNTA_H__C09ABD65_DD1E_4E53_AE48_F4BC6D68173C__INCLUDED_)
