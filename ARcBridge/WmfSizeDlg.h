// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_WMFSIZEDLG_H__4E0439D7_426C_46CD_8858_A31BB403EC84__INCLUDED_)
#define AFX_WMFSIZEDLG_H__4E0439D7_426C_46CD_8858_A31BB403EC84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WmfSizeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWmfSizeDlg dialog

class CWmfSizeDlg : public CDialog
{
// Construction
public:
	CWmfSizeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWmfSizeDlg)
	enum { IDD = IDD_DIALOG_WMFSIZE };
	BOOL	m_bAutoSize;
	double	m_dHeight;
	double	m_dWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWmfSizeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWmfSizeDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WMFSIZEDLG_H__4E0439D7_426C_46CD_8858_A31BB403EC84__INCLUDED_)
