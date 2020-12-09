// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_SAVECOMPLETEDLG_H__27857F3C_4F62_460A_8CC3_08B5648BE87C__INCLUDED_)
#define AFX_SAVECOMPLETEDLG_H__27857F3C_4F62_460A_8CC3_08B5648BE87C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SaveCompleteDlg.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CSaveCompleteDlg dialog

class CSaveCompleteDlg : public CDialog
{
// Construction
public:
	CSaveCompleteDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSaveCompleteDlg)
	enum { IDD = IDD_DIALOG_SAVE_COMPLETE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSaveCompleteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSaveCompleteDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAVECOMPLETEDLG_H__27857F3C_4F62_460A_8CC3_08B5648BE87C__INCLUDED_)
