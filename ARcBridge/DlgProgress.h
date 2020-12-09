// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DLGPROGRESS_H__252F8701_2EE2_4012_9359_9385B22DB915__INCLUDED_)
#define AFX_DLGPROGRESS_H__252F8701_2EE2_4012_9359_9385B22DB915__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgProgress.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress dialog

class AFX_EXT_CLASS CDlgProgress : public CDialog
{
// Construction
public:
	void InitProgressData();
	void SetProgressData(CString sMessage, long nCurPos);
	CString m_sMessage;
	long m_nCur;

	CDlgProgress(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgProgress)
	enum { IDD = IDD_DIALOG_PROGRESS };
	CProgressCtrl	m_ctlProgress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgProgress)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	long m_nMin;
	long m_nMax;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROGRESS_H__252F8701_2EE2_4012_9359_9385B22DB915__INCLUDED_)
