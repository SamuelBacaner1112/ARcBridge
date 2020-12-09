// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_BRIDGETYPEDLG_H__BF89951E_CC94_4ACE_A619_F87F097F05E0__INCLUDED_)
#define AFX_BRIDGETYPEDLG_H__BF89951E_CC94_4ACE_A619_F87F097F05E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BridgeTypeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBridgeTypeDlg dialog
class CBridgeTypeDlg : public CUserDialog
{
// Construction
public:
	CARcBridgeDataStd *m_pARcBridgeDataStd;	
	CBridgeTypeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBridgeTypeDlg)
//#ifdef _NONG_GI_BAN_
//	enum { IDD = IDD_DIALOG_BRIDGE_TYPE_NONG };
//#else
	enum { IDD = IDD_DIALOG_BRIDGE_TYPE };
//#endif
	CButton	m_btOK;
	CButton	m_btCancel;
	int		m_nBridgeType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBridgeTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBridgeTypeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BRIDGETYPEDLG_H__BF89951E_CC94_4ACE_A619_F87F097F05E0__INCLUDED_)
