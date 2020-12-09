// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_EARTHQUAKEGRADEDLG_H__4D186BDB_2028_4C85_8F63_442880E9D0AE__INCLUDED_)
#define AFX_EARTHQUAKEGRADEDLG_H__4D186BDB_2028_4C85_8F63_442880E9D0AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EarthquakeGradeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEarthquakeGradeDlg dialog

class CEarthquakeGradeDlg : public CUserDialog
{
// Construction
public:
	CEarthquakeGradeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEarthquakeGradeDlg)
	enum { IDD = IDD_DIALOG_EARTHQUACKGRADE };
		// NOTE: the ClassWizard will add data members here
	int		m_nType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEarthquakeGradeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEarthquakeGradeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EARTHQUAKEGRADEDLG_H__4D186BDB_2028_4C85_8F63_442880E9D0AE__INCLUDED_)
