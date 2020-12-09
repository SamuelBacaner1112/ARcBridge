// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DRAWINGOPTIONREBAR_H__10733881_DD8D_4A62_A377_5647EEE536D2__INCLUDED_)
#define AFX_DRAWINGOPTIONREBAR_H__10733881_DD8D_4A62_A377_5647EEE536D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawingOptionRebar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDrawingOptionRebar dialog
#include "OptionBase.h"
class CDrawingOptionRebar : public COptionBase
{
	DECLARE_DYNCREATE(CDrawingOptionRebar)

// Construction
public:
	CDrawingOptionRebar();
	~CDrawingOptionRebar();

	CDocument* m_pDoc;
	CGridCtrlMng m_Grid;

// Dialog Data
	//{{AFX_DATA(CDrawingOptionRebar)
	enum { IDD = IDD_OPTION_REBAR };	
	//}}AFX_DATA
	CCustomOptions m_Options;	

	void InitTree();
	void InitGrid();
	void SetDataSave();

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDrawingOptionRebar)
	public:
	virtual BOOL OnSetActive();
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDrawingOptionRebar)
	virtual BOOL OnInitDialog();
	afx_msg void OnOptionDefault();
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWINGOPTIONREBAR_H__10733881_DD8D_4A62_A377_5647EEE536D2__INCLUDED_)
