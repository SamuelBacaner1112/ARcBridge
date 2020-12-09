// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_REBARJOINTDLG_H__919F3526_0F66_48DA_9AD5_8F747CEFE787__INCLUDED_)
#define AFX_REBARJOINTDLG_H__919F3526_0F66_48DA_9AD5_8F747CEFE787__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RebarJointDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRebarJointDlg dialog

class CRebarJointDlg : public CDialog
{
// Construction
public:
	BOOL m_bInputJointSuOf2Cycle;
	CRebarJointDlg(CWnd* pParent = NULL);   // standard constructor
	CRebarJointDlg(CRcBridgeRebar *pBri, CARcBridgeDataStd *pStd, long nCycle, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRebarJointDlg)
	enum { IDD = IDD_DIALOG_REBARJOINT };
	CButton	m_checkDanbuBending;
	CButton	m_checkSttWallInner;
	CButton	m_checkInWallInner;
	CButton	m_checkEndWallInner;
	CButton	m_btnOk;
	CButton	m_btnCancel;
	CXTPTabCtrl	m_ctlTabUpperSlab;
	CXTPTabCtrl	m_ctlTabOutWall;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRebarJointDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRebarJointDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOK();
	afx_msg void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void SetDataSaveControl();
	void SetDataInitControl();
	void SetControl();
	
	void SetDataSave();
	void SetDataInit();
	void InitGridManager();
	CGridCtrlMng	m_GridUpperSlab;	
	CGridCtrlMng	m_GridOutWall;	


	void SetGridDataOutWall_Cycle2();
	void SetGridDataUpperSlab_Cycle2();

	void SetGridDataOutWall_Cycle1();
	void SetGridDataUpperSlab_Cycle1();
	void SetGridData();
	void InitTab();

	long m_nCycle;
	CRcBridgeRebar* m_pBri;
	CARcBridgeDataStd *m_pStd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REBARJOINTDLG_H__919F3526_0F66_48DA_9AD5_8F747CEFE787__INCLUDED_)
