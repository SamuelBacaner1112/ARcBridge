// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_SHORTCUTPANESTART_H__3B13C465_8F95_4B35_9754_9C841E1750CA__INCLUDED_)
#define AFX_SHORTCUTPANESTART_H__3B13C465_8F95_4B35_9754_9C841E1750CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShortcutPaneStart.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShortcutPaneStart window

class CShortcutPaneStart : public CXTPShortcutBarPane
{
// Construction
public:
	CShortcutPaneStart();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShortcutPaneStart)
	//}}AFX_VIRTUAL

// Implementation
public:
	CXTPTaskPanel m_wndTaskPanel;
	CImageList m_ilTaskIcons;

	void InitMRUList();
	BOOL Create(LPCTSTR lpszCaption, CXTPShortcutBar* pParent);
	virtual ~CShortcutPaneStart();

	// Generated message map functions
protected:
	//{{AFX_MSG(CShortcutPaneStart)
	afx_msg LRESULT OnTaskPanelNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHORTCUTPANESTART_H__3B13C465_8F95_4B35_9754_9C841E1750CA__INCLUDED_)
