// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DLGOCC3D_H__92A3773D_A34A_45F3_9D3B_F031FB31F16E__INCLUDED_)
#define AFX_DLGOCC3D_H__92A3773D_A34A_45F3_9D3B_F031FB31F16E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOcc3D.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOcc3D dialog
class CDlgOccView;
class CUbiARcBridgeData;
class CARcBridgeDataStd;

class CDlgOcc3D : public CXTResizeDialog
{
// Construction
public:
	void InitComboBridge(long nBri=0);
	void InitControl();
	CDlgOcc3D(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgOcc3D)
	enum { IDD = IDD_DIALOG_OCCVIEW_SELECT };
	CButton	m_ctlRebarOption;
	CButton	m_btnDraw3d;
	CXTFlatComboBox	m_comRcBridge;
	CXTFlatComboBox	m_comRebarCreateKind;
	int		m_nRadioKind3d;
	//}}AFX_DATA

	CDlgOccView			*m_pDlgOccView;
	CUbiARcBridgeData	*m_pUbiARcBridgeData;
	CARcBridgeDataStd	*m_pARcBridgeDataStd;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOcc3D)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOcc3D)
	afx_msg void OnDlgoccButtonDraw3d();
	afx_msg void OnSelchangeDlgoccComboRc();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDlgoccRadioSelectRc();
	afx_msg void OnDlgoccRadioAllRc();
	virtual BOOL OnInitDialog();
	afx_msg void OnCancelMode();
	afx_msg void OnSelchangeComboRebarCreatekind();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOCC3D_H__92A3773D_A34A_45F3_9D3B_F031FB31F16E__INCLUDED_)
