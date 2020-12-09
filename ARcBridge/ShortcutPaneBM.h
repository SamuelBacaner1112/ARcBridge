// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_SHORTCUTPANEBM_H__3154958F_AB59_4DF5_AEA5_F58163FB3E23__INCLUDED_)
#define AFX_SHORTCUTPANEBM_H__3154958F_AB59_4DF5_AEA5_F58163FB3E23__INCLUDED_

#include "DialogMenuOutBM.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShortcutPaneBM.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShortcutPaneBM window

class CShortcutPaneBM : public CXTPShortcutBarPane
{
// Construction
public:
	CShortcutPaneBM();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShortcutPaneBM)
	//}}AFX_VIRTUAL

// Implementation
public:
	CDialogMenuOutBM m_dlgMenuOutBM;
	void InitTree();
	CXTTreeCtrl m_wndTreeBM;
	BOOL CreateStyle1(LPCTSTR lpszCaption, CXTPShortcutBar* pParent);
	virtual ~CShortcutPaneBM();

	// Generated message map functions
protected:
	//{{AFX_MSG(CShortcutPaneBM)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHORTCUTPANEBM_H__3154958F_AB59_4DF5_AEA5_F58163FB3E23__INCLUDED_)
