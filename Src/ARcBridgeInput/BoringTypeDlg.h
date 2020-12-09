// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_BORINGTYPEDLG_H__3929C5F6_AD4C_4E80_9653_C9843FC3532D__INCLUDED_)
#define AFX_BORINGTYPEDLG_H__3929C5F6_AD4C_4E80_9653_C9843FC3532D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BoringTypeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBoringTypeDlg dialog

class CBoringTypeDlg : public CUserDialog
{
// Construction
public:
	CBoringTypeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBoringTypeDlg)
	enum { IDD = IDD_DIALOG_BORINGMARK };
	CButton	m_btOK;
	CButton	m_btCancel;
	int		m_nBoringType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBoringTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBoringTypeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BORINGTYPEDLG_H__3929C5F6_AD4C_4E80_9653_C9843FC3532D__INCLUDED_)
