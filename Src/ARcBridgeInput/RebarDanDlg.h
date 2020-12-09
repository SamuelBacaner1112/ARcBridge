// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_REBARDANDLG_H__97C7552C_BE9B_4C9F_94E9_7F3BB31106F3__INCLUDED_)
#define AFX_REBARDANDLG_H__97C7552C_BE9B_4C9F_94E9_7F3BB31106F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RebarDanDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRebarDanDlg dialog

class CRebarDanDlg : public CDialog
{
// Construction
public:
	void SetGridDataRib();
	long m_nTypeTab;	// 처음 뜰때 선택되어 져야 할 탭 종류(0 : 상부슬래브, 1 : 외측벽체, 2 : 내측벽체, 3 : 기초)
	CRebarDanDlg(CWnd* pParent = NULL);   // standard constructor
	CRebarDanDlg(CRcBridgeRebar *pBri, CARcBridgeDataStd *pStd, long nCycle, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRebarDanDlg)
	enum { IDD = IDD_DIALOG_REBARDAN };
	CButton	m_checkDetailInput23Dan;
	CButton	m_btnRecommand;
	CButton	m_btnOk;
	CXTPTabCtrl	m_ctlTab;
	CButton	m_btnCancel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRebarDanDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRebarDanDlg)
	afx_msg void OnButtonRecommand();
	afx_msg void OnCheckDetailinput23dan();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);

private:
	void SetGridDataFooting();
	void InitTab();
	void SetGridData();
	void SetDataSave();
	void SetDataInit();

	void SetGridDataUpperSlab();
	void SetGridDataLowerSlab();
	void SetGridDataOutWall();
	void SetGridDataInWall();

	void SetExistOutWall2Dan();

	CGridCtrlMng	m_Grid;	
	CStringArray m_sArrTabName;
	long m_nTab;
	long m_nTabUpperSlab;
	long m_nTabLowerSlab;
	long m_nTabOutWall;
	long m_nTabInWall;
	long m_nTabFooting;
	long m_nTabRib;

	long m_nCycle;
	CRcBridgeRebar* m_pBri;
	CARcBridgeDataStd *m_pStd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REBARDANDLG_H__97C7552C_BE9B_4C9F_94E9_7F3BB31106F3__INCLUDED_)
