// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DRAWINGOPTIONSHOEARRANGE_H__EA6DD95A_08CF_4781_B2B3_2F393A4AC363__INCLUDED_)
#define AFX_DRAWINGOPTIONSHOEARRANGE_H__EA6DD95A_08CF_4781_B2B3_2F393A4AC363__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawingOptionShoeArrange.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDrawingOptionShoeArrange dialog
#include "OptionBase.h"
class CDrawingOptionShoeArrange : public COptionBase
{
	DECLARE_DYNCREATE(CDrawingOptionShoeArrange)

// Construction
public:
	CDrawingOptionShoeArrange();
	~CDrawingOptionShoeArrange();

	CDocument* m_pDoc;
	CGridCtrlMng m_Grid;

// Dialog Data
	//{{AFX_DATA(CDrawingOptionShoeArrange)
	enum { IDD = IDD_OPTION_SHOE_ARRANGE };	
	//}}AFX_DATA
	CCustomOptions m_Options;	

	void InitTree();
	void InitGrid();
	void SetDataSave();

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDrawingOptionShoeArrange)
	public:
	virtual BOOL OnSetActive();
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDrawingOptionShoeArrange)
	virtual BOOL OnInitDialog();
	afx_msg void OnOptionDefault();
	afx_msg void OnSelchangeComboBridgeNo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWINGOPTIONSHOEARRANGE_H__EA6DD95A_08CF_4781_B2B3_2F393A4AC363__INCLUDED_)
