// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_GUARDWALLDLG_H__6B72A869_AEEB_412C_8E8B_6C04465B15EA__INCLUDED_)
#define AFX_GUARDWALLDLG_H__6B72A869_AEEB_412C_8E8B_6C04465B15EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GuardWallDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGuardWallDlg dialog

class CARcBridgeInputStd;

class CGuardWallDlg : public CDialog
{
// Construction
public:
	CGuardWallDlg(CARcBridgeInputStd* pIn, BOOL bUnderGroundRahmen, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGuardWallDlg)
	enum { IDD = IDD_DIALOG_GUARDWALL };
	CXTHyperLink	m_hyperHelp;
	CButton		m_btDefault;
	CButton		m_btOK;
	CDomyunView		m_DomView;
	CXTPTabCtrl		m_ctlTab;
	CGridCtrlMng		m_Grid;
	BOOL			m_bRoadNotSlab;
	//}}AFX_DATA

private:
	CARcBridgeInputStd* m_pIn;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGuardWallDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGuardWallDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
private:
	long m_nTab1;	// 연석, 방호벽 및 중분대
	long m_nTab2;	// 자갈막이(철근), 보차도 경계석
	long m_nTab3;	// 교측보도, 보도
	long m_nTab4;	// 연석+보도, 방음벽 기초
	long m_nTab5;	// 통신구, 흙막이벽
	long m_nTab6;	// 통신구+보도, 난간기초
	long m_nTab7;	// 다이크, 다이크
	long m_nTab8;	// 자갈막이(무근)

	BOOL m_bRail;
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);

	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void SetDataDefault();
	void SetGridTitle();
	void SetGridTitleGW();					// 방호벽, 중분대
	void SetGridTitleFW();					// 보도

	void SetDataInit();			
	void SetDataInitGW();					// 방호벽, 중분대
	void SetDataInitFootway();				// 보도

	void SetDataSave();	
	void SetDataSaveGW();					// 방호벽, 중분대
	void SetDataSaveFootway();				// 보도
	void InitTabCtrl();
	void SetFocusCellByType();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GUARDWALLDLG_H__6B72A869_AEEB_412C_8E8B_6C04465B15EA__INCLUDED_)
