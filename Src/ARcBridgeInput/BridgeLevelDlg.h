// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_BRIDGELEVELDLG_H__E979910D_F080_4660_9623_57C761FC341B__INCLUDED_)
#define AFX_BRIDGELEVELDLG_H__E979910D_F080_4660_9623_57C761FC341B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BridgeLevelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBridgeLevelDlg dialog

class CBridgeLevelDlg : public CUserDialog
{
// Construction
public:
	CBridgeLevelDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBridgeLevelDlg)
	enum { IDD = IDD_DIALOG_BRIDGELEVEL };
	int		m_nType;
	//}}AFX_DATA
	int		m_nTypeUnit;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBridgeLevelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBridgeLevelDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BRIDGELEVELDLG_H__E979910D_F080_4660_9623_57C761FC341B__INCLUDED_)
