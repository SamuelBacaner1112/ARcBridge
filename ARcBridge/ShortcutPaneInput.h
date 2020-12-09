// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_SHORTCUTPANEINPUT_H__134DFD65_9FFE_48BE_9C3A_5B56A0B79C3A__INCLUDED_)
#define AFX_SHORTCUTPANEINPUT_H__134DFD65_9FFE_48BE_9C3A_5B56A0B79C3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShortcutPaneInput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShortcutPaneInput window

class CShortcutPaneInput : public CXTPShortcutBarPane
{
// Construction
public:
	CShortcutPaneInput();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShortcutPaneInput)
	//}}AFX_VIRTUAL

// Implementation
public:
	CXTTreeCtrl_Deprecated m_wndTreeInput;
	CStringArray m_cmdStrArr;
	CDWordArray m_cmdIDArr;

	CImageList m_ilTreeIcons;

	void InitTreeState();
	void SetItemPosition(UINT nMenuID);
	void SetItemPositionUpdate(UINT nMenuID, BOOL bSelect = FALSE);
	BOOL Create(LPCTSTR lpszCaption, CXTPShortcutBar* pParent);
	virtual ~CShortcutPaneInput();

	// Generated message map functions
protected:
	//{{AFX_MSG(CShortcutPaneInput)
	afx_msg void OnSelchangedTreeInput(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	HTREEITEM hItemPrev;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHORTCUTPANEINPUT_H__134DFD65_9FFE_48BE_9C3A_5B56A0B79C3A__INCLUDED_)
