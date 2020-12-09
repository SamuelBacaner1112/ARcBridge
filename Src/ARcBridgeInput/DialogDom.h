// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DIALOGDOM_H__245B0DF1_E072_4D46_911B_8D0A4C5FF113__INCLUDED_)
#define AFX_DIALOGDOM_H__245B0DF1_E072_4D46_911B_8D0A4C5FF113__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogDom.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogDom dialog

class CDialogDom : public CDialog
{
// Construction
public:
	CDomyun m_Dom;
	void DrawInputDomyunView(BOOL bZoomAll);
	CDialogDom(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogDom)
	enum { IDD = IDD_DIALOG_DOM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogDom)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogDom)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CDomyunView m_DomView;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGDOM_H__245B0DF1_E072_4D46_911B_8D0A4C5FF113__INCLUDED_)
