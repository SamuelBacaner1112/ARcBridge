// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_POSTBASELINEDLG_H__4C6320FD_6E38_4D17_8716_7969B9E8A560__INCLUDED_)
#define AFX_POSTBASELINEDLG_H__4C6320FD_6E38_4D17_8716_7969B9E8A560__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PostBaseLineDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPostBaseLineDlg dialog

class CPostBaseLineDlg : public CUserDialog
{
// Construction
public:
	CPostBaseLineDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPostBaseLineDlg)
	enum { IDD = IDD_DIALOG_POST };
	int		m_nType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPostBaseLineDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPostBaseLineDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POSTBASELINEDLG_H__4C6320FD_6E38_4D17_8716_7969B9E8A560__INCLUDED_)
