// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DRAWINGOPTIONVERTICALPLANE_H__7E715298_5D76_4451_89BE_346835AC51A1__INCLUDED_)
#define AFX_DRAWINGOPTIONVERTICALPLANE_H__7E715298_5D76_4451_89BE_346835AC51A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawingOptionVerticalPlane.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDrawingOptionVerticalPlane dialog
#include "OptionBase.h"
class CDrawingOptionVerticalPlane : public COptionBase
{
	DECLARE_DYNCREATE(CDrawingOptionVerticalPlane)

// Construction
public:
	CDrawingOptionVerticalPlane();
	~CDrawingOptionVerticalPlane();

	CDocument* m_pDoc;
	CGridCtrlMng m_Grid;

// Dialog Data
	//{{AFX_DATA(CDrawingOptionVerticalPlane)
	enum { IDD = IDD_OPTION_VERTICAL_PLANE };	
	//}}AFX_DATA
	CCustomOptions m_Options;	

	void InitTree();
	void InitGrid();
	void SetDataSave();

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDrawingOptionVerticalPlane)
	public:
	virtual BOOL OnSetActive();
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDrawingOptionVerticalPlane)
	virtual BOOL OnInitDialog();
	afx_msg void OnOptionDefault();
	afx_msg void OnSelchangeComboBridgeNo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWINGOPTIONVERTICALPLANE_H__7E715298_5D76_4451_89BE_346835AC51A1__INCLUDED_)
