// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DRAWINGOPTIONBORING_H__F7926145_2FD8_43B0_98A2_EF12A462057E__INCLUDED_)
#define AFX_DRAWINGOPTIONBORING_H__F7926145_2FD8_43B0_98A2_EF12A462057E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawingOptionBoring.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDrawingOptionBoring dialog
#include "OptionBase.h"
class CDrawingOptionBoring : public COptionBase
{
	DECLARE_DYNCREATE(CDrawingOptionBoring)

// Construction
public:
	CDrawingOptionBoring();
	~CDrawingOptionBoring();

	CDocument* m_pDoc;
	CGridCtrlMng m_Grid;

// Dialog Data
	//{{AFX_DATA(CDrawingOptionBoring)
	enum { IDD = IDD_OPTION_BORING };	
	//}}AFX_DATA
	CCustomOptions m_Options;	

	void InitTree();
	void InitGrid();
	void SetDataSave();

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDrawingOptionBoring)
	public:
	virtual BOOL OnSetActive();
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDrawingOptionBoring)
	virtual BOOL OnInitDialog();
	afx_msg void OnOptionDefault();
	afx_msg void OnSelchangeComboBridgeNo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWINGOPTIONBORING_H__F7926145_2FD8_43B0_98A2_EF12A462057E__INCLUDED_)
