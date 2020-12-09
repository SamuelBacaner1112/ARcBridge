// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawingOptionSlabStandardOutput.h: interface for the CDrawingOptionSlabStandardOutput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWINGOPTIONSLABSTANDARDOUTPUT_H__B9A6ED8E_F12B_4BF0_8C20_69C0A4461DA9__INCLUDED_)
#define AFX_DRAWINGOPTIONSLABSTANDARDOUTPUT_H__B9A6ED8E_F12B_4BF0_8C20_69C0A4461DA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OptionBase.h"

class CDrawingOptionSlabStandardOutput : public COptionBase  
{
	DECLARE_DYNCREATE(CDrawingOptionSlabStandardOutput)

// Construction
public:
	CDrawingOptionSlabStandardOutput();
	~CDrawingOptionSlabStandardOutput();

	CDocument* m_pDoc;
	CGridCtrlMng m_Grid;

// Dialog Data
	//{{AFX_DATA(CDrawingOptionSlabStandardOutput)
	enum { IDD = IDD_OPTION_SLAB_STANDARD_OUTPUT };	
	//}}AFX_DATA
	CCustomOptions m_Options;	

	void InitTree();
	void InitGrid();
	void SetDataSave();

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDrawingOptionSlabStandardOutput)
	public:
	virtual BOOL OnSetActive();
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDrawingOptionSlabStandardOutput)
	virtual BOOL OnInitDialog();
	afx_msg void OnOptionDefault();
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_DRAWINGOPTIONSLABSTANDARDOUTPUT_H__B9A6ED8E_F12B_4BF0_8C20_69C0A4461DA9__INCLUDED_)
