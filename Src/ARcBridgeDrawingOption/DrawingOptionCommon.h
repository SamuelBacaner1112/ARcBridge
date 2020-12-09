// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DRAWINGOPTIONCOMMON_H__46485F84_B686_439D_9D36_0F37766D2D00__INCLUDED_)
#define AFX_DRAWINGOPTIONCOMMON_H__46485F84_B686_439D_9D36_0F37766D2D00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawingOptionCommon.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDrawingOptionCommon dialog
#include "OptionBase.h"
class CDrawingOptionCommon : public COptionBase
{
	DECLARE_DYNCREATE(CDrawingOptionCommon)

// Construction
public:
	CDrawingOptionCommon();
	~CDrawingOptionCommon();

	CDocument* m_pDoc;
	CGridCtrlMng m_Grid1;
	CGridCtrlMng m_Grid2;

// Dialog Data
	//{{AFX_DATA(CDrawingOptionCommon)
	enum { IDD = IDD_OPTION_COMMON };	
	//}}AFX_DATA
	CCustomOptions m_Options;

	void InitTree();
	void InitGrid();
	void InitGrid1();
	void InitGrid2();
	void SetDataSave() {}

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDrawingOptionCommon)
	public:
	virtual BOOL OnSetActive();
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDrawingOptionCommon)
	virtual BOOL OnInitDialog();
	afx_msg void OnOptionDefault();
	//}}AFX_MSG
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWINGOPTIONCOMMON_H__46485F84_B686_439D_9D36_0F37766D2D00__INCLUDED_)
