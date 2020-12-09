// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OPTIONSHEET_H__19850B59_EC74_48E3_8416_3D5EFA55FD68__INCLUDED_)
#define AFX_OPTIONSHEET_H__19850B59_EC74_48E3_8416_3D5EFA55FD68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionSheet.h : header file
//

#include "ARcBridgeDrawingOption.h"

/////////////////////////////////////////////////////////////////////////////
// COptionSheet

class AFX_EXT_CLASS COptionSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(COptionSheet)

// Construction
public:
	COptionSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	COptionSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0, BOOL bSlabStandardOutput=FALSE);

// Attributes
public:
	CDocument *m_pDoc;

	CDrawingOptionCommon m_pageOptionCommon;
	CDrawingOptionVerticalPlane m_pageOptionVerticalPlane;
	CDrawingOptionGeneral m_pageOptionGeneral;
	CDrawingOptionRebar m_pageOptionRebar;
	CDrawingOptionAps m_pageOptionAps;
	CDrawingOptionCns m_pageOptionCns;
	CDrawingOptionOthers m_pageOptionOthers;
	CDrawingOptionShoeArrange m_pageOptionShoeArrange;
	CDrawingOptionBoring m_pageOptionBoring;
	CDrawingOptionHyunTa m_pageOptionHyunTa;
	CDrawingOptionSlabStandardOutput m_pageOptionSlabStandardOutput;

	CXTPComboBox m_cbBriNo;
	CButton m_btnLoad;
	CButton m_btnSave;
	CButton m_btnDefault;
	BOOL m_bSetOK;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionSheet)
	public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL
	afx_msg void OnOptionLoad();
	afx_msg void OnOptionSave();

// Implementation
public:
	virtual ~COptionSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(COptionSheet)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchangeComboBridgeNo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void InitComboCtrl();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSHEET_H__19850B59_EC74_48E3_8416_3D5EFA55FD68__INCLUDED_)
