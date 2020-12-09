// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTBASEBRIDGEFORMFACTOR_H__68E07136_F8EE_48C0_A2C5_B340C454E94A__INCLUDED_)
#define AFX_INPUTBASEBRIDGEFORMFACTOR_H__68E07136_F8EE_48C0_A2C5_B340C454E94A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputBaseBridgeFormFactor.h : header file
//

#include "BridgeTypeDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CInputGenBridgeForm dialog

class CInputBaseBridgeFormFactor : public CInputDialogBase
{
// Construction
public:
	BOOL IsIncludeArch();
	BOOL IsCheckMoveOtherDlg();
	long m_nFlag;
	CInputBaseBridgeFormFactor(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputBaseBridgeFormFactor();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputBaseBridgeFormFactor)
	enum { IDD = IDD_BASE4_BRIDGE_FORMFACTOR };
	CButton	m_btSaveIndivisual;
	CButton	m_btInsertIndivisual;
	CButton	m_btNewAll;
	CButton	m_btCopy;
	CButton	m_btMoveDown;
	CButton	m_btMoveUp;
	CButton	m_btDelete;
	CButton	m_btAdd;
	CGridCtrlMng	m_Grid;
	int		m_nPlane;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputBaseBridgeFormFactor)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBridgeTypeDlg m_dlgBridgeType;

	// Generated message map functions
	//{{AFX_MSG(CInputBaseBridgeFormFactor)
	afx_msg void OnButtonAddRow();
	afx_msg void OnButtonDeleteRow();
	afx_msg void OnButtonMoveUp();
	afx_msg void OnButtonMoveDown();
	afx_msg void OnButtonCopy();
	afx_msg void OnButtonNewAll();
	afx_msg void OnButtonSaveIndivisual();
	afx_msg void OnRadioPlane();
	afx_msg void OnRadioVertical();
	afx_msg void OnButtonInsetIndivisual();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	DECLARE_MESSAGE_MAP()
private:
	CDlgProgress *m_pProgress;
	BOOL IsOKMoveOtherDlg();
	void DrawLineVertical(CDomyun *pDomP);
	void DrawInputDomyunView(BOOL bZoomAll = TRUE);
	void DrawLinePlane(CDomyun* pDom);
	void OnPreInitDialog();
	void SetGridTitle();
	void SetDataDefault();
	void SetDataInit();
	void SetDataSave();
	HGBOOL IsEnableUnderGroundRahmen(CRcBridgeApp* pBri) const;
	void SetMatchBridge(CRcBridgeApp* pBriCur, HGINT32 iBridge);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTBASEBRIDGEFORMFACTOR_H__68E07136_F8EE_48C0_A2C5_B340C454E94A__INCLUDED_)
