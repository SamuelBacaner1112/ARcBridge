// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_INPUTCOMMENT_H__5EDC1BAE_08C8_4E36_AB3E_9ACE2DE6C798__INCLUDED_)
#define AFX_INPUTCOMMENT_H__5EDC1BAE_08C8_4E36_AB3E_9ACE2DE6C798__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputComment.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CInputComment window

class CInputComment : public CWnd
{
// Construction
public:
	CInputComment();

// Attributes
public:
	void SetDescString(CString str, BOOL bWarring=FALSE);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputComment)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_bWarning;
	virtual ~CInputComment();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInputComment)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CFont m_fntNormal;
	CFont m_fntBold;
	CString m_strDesc;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTCOMMENT_H__5EDC1BAE_08C8_4E36_AB3E_9ACE2DE6C798__INCLUDED_)
