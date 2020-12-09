// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DRAWINGOPTIONAPS_H__5AF243F0_AEDB_4523_B2E0_C06CE79DDE46__INCLUDED_)
#define AFX_DRAWINGOPTIONAPS_H__5AF243F0_AEDB_4523_B2E0_C06CE79DDE46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawingOptionAps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDrawingOptionAps dialog
#include "OptionBase.h"
class CDrawingOptionAps : public COptionBase
{
	DECLARE_DYNCREATE(CDrawingOptionAps)

// Construction
public:
	CDrawingOptionAps();
	~CDrawingOptionAps();

	CDocument* m_pDoc;
	CGridCtrlMng m_Grid;

// Dialog Data
	//{{AFX_DATA(CDrawingOptionAps)
	enum { IDD = IDD_OPTION_APS };	
	//}}AFX_DATA
	CCustomOptions m_Options;	

	void InitTree();
	void InitGrid();
	void SetDataSave();

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDrawingOptionAps)
	public:
	virtual BOOL OnSetActive();
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDrawingOptionAps)
	virtual BOOL OnInitDialog();
	afx_msg void OnOptionDefault();
	afx_msg void OnSelchangeComboBridgeNo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWINGOPTIONAPS_H__5AF243F0_AEDB_4523_B2E0_C06CE79DDE46__INCLUDED_)
