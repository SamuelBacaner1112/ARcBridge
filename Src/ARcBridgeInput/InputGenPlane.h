// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTGENPLANE_H__B5DCE6AF_3FFB_42F4_B3AC_F80B4CBD2D58__INCLUDED_)
#define AFX_INPUTGENPLANE_H__B5DCE6AF_3FFB_42F4_B3AC_F80B4CBD2D58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputGenPlane.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputGenPlane dialog

class CInputGenPlane : public CInputDialogBase
{
// Construction
public:
	CInputGenPlane(CWnd* pParent = NULL);   // standard constructor
	~CInputGenPlane();
	BOOL IsValid();

// Dialog Data
	//{{AFX_DATA(CInputGenPlane)
	enum { IDD = IDD_GEN_PLANE };
	CXTPComboBox	m_cbBridgeNo;
	CButton	m_btnFootingOptimize;
	CGridCtrlMng	m_Grid;
	CXTPTabCtrl	m_Tab;
	int		m_nCurrentDim;
	BOOL	m_bFootingShape;
	BOOL	m_bCheckfootLenFloat;
	//}}AFX_DATA
	void KillFocusDlg();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputGenPlane)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputGenPlane)
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnButtonFootingOptimize();
	afx_msg void OnRadioCurrentDim();
	afx_msg void OnRadioWholeDim();
	afx_msg void OnCheckFootingShape();
	afx_msg void OnCheckFootingLenFloat();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	DECLARE_MESSAGE_MAP()
private:
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);

	BOOL IsOKMoveOtherDlg();
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void OnPreInitDialog();
	void InitComboCtrl();
	void InitTabCtrl();
	void SetGridData();
	void SetDataDefault();
	void SetDataInit();
	void SetDataSave();
	void SetShowWindow();
	void SetEnableWindow();
	CString dMarginL, dMarginR;
	CString strWL, strWR;
	CString strAFL, strAFR;
	CString strAWL, strAWR;
	CString strCombo;
	CStringArray sArr1, sArr2; //날개벽설치, 기초단부직각처리 콤보내용
	UINT m_nLineReadOnlyBaseL, m_nLineReadOnlyBaseR;
	UINT m_nLineReadOnlyAWL, m_nLineReadOnlyAWR;
	BOOL IsWingWallAngleValid(BOOL bStt, BOOL bLeft, CDPoint vAng);

	CDlgProgress* m_pProgress;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTGENPLANE_H__B5DCE6AF_3FFB_42F4_B3AC_F80B4CBD2D58__INCLUDED_)
