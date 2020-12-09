// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_REBARJOINT1CYCLEDLG_H__169C8D82_9E3F_4535_8D60_E1F727395222__INCLUDED_)
#define AFX_REBARJOINT1CYCLEDLG_H__169C8D82_9E3F_4535_8D60_E1F727395222__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RebarJoint1CycleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRebarJoint1CycleDlg dialog

class CRebarJoint1CycleDlg : public CXTResizeDialog
{
// Construction
public:
	BOOL IsDetailInput23Dan();
	void SetGridDataRib(long nCycle);
	CRebarJoint1CycleDlg(CWnd* pParent = NULL);   // standard constructor
	CRebarJoint1CycleDlg(CRcBridgeRebar *pBri, CARcBridgeDataStd *pStd, long nCycle, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRebarJoint1CycleDlg)
	enum { IDD = IDD_DIALOG_REBARJOINT_1CYCLE };
	CButton	m_btnRecommand;
	CXTPTabCtrl	m_ctlTabCycle;
	CButton	m_btnOk;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRebarJoint1CycleDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRebarJoint1CycleDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonRecommand();
	//}}AFX_MSG
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
	void OnCellChangedDataOutWall(NMHDR* pNotifyStruct, LRESULT* result);
private:
	void SetDataSaveControl();
	void SetDataInitControl();
	void SetControl();
	
	void SetDataSave();
	void SetDataInit();
	void InitGridManager();
	CGridCtrlMng	m_GridUpperSlab;	
	CGridCtrlMng	m_GridOutWall;	
	CGridCtrlMng	m_GridInWall;	
	CGridCtrlMng	m_GridRib;	


	void SetGridDataInWall(long nCycle);
	void SetGridDataOutWall(long nCycle);
	void SetGridDataUpperSlab(long nCycle);
	void SetGridDataLowerSlab(long nCycle);
	void SetGridData();
	void InitTab();

	long m_nCycle;
	CRcBridgeRebar* m_pBri;
	CARcBridgeDataStd *m_pStd;
public:
	CString m_strTitleRib;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REBARJOINT1CYCLEDLG_H__169C8D82_9E3F_4535_8D60_E1F727395222__INCLUDED_)
