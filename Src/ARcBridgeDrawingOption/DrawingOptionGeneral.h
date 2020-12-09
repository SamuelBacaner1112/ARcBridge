// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DRAWINGOPTIONGENERAL_H__393372FB_7A70_4040_8499_12D2A1B361AF__INCLUDED_)
#define AFX_DRAWINGOPTIONGENERAL_H__393372FB_7A70_4040_8499_12D2A1B361AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawingOptionGeneral.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDrawingOptionGeneral dialog
#include "OptionBase.h"
class CDrawingOptionGeneral : public COptionBase
{
	DECLARE_DYNCREATE(CDrawingOptionGeneral)

// Construction
public:
	CDrawingOptionGeneral();
	~CDrawingOptionGeneral();

	CDocument* m_pDoc;
	CGridCtrlMng m_Grid;

// Dialog Data
	//{{AFX_DATA(CDrawingOptionGeneral)
	enum { IDD = IDD_OPTION_GENERAL };	
	//}}AFX_DATA
	CCustomOptions m_Options;	

	void InitTree();
	void InitGrid();
	void SetDataSave();

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDrawingOptionGeneral)
	public:
	virtual BOOL OnSetActive();
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDrawingOptionGeneral)
	virtual BOOL OnInitDialog();
	afx_msg void OnOptionDefault();
	afx_msg void OnSelchangeComboBridgeNo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWINGOPTIONGENERAL_H__393372FB_7A70_4040_8499_12D2A1B361AF__INCLUDED_)
