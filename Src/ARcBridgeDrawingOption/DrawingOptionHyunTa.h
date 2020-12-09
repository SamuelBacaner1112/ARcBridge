// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DRAWINGOPTIONHYUNTA_H__6BB7D694_122C_4400_8830_BF7D0DF37923__INCLUDED_)
#define AFX_DRAWINGOPTIONHYUNTA_H__6BB7D694_122C_4400_8830_BF7D0DF37923__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawingOptionHyunTa.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDrawingOptionHyunTa dialog
#include "OptionBase.h"
class CDrawingOptionHyunTa : public COptionBase
{
	DECLARE_DYNCREATE(CDrawingOptionHyunTa)

// Construction
public:
	CDrawingOptionHyunTa();
	~CDrawingOptionHyunTa();

	CDocument* m_pDoc;
	CGridCtrlMng m_Grid;
// Dialog Data
	//{{AFX_DATA(CDrawingOptionHyunTa)
	enum { IDD = IDD_OPTION_HYUNTA };	
	//}}AFX_DATA
	CCustomOptions m_Options;	
	
	void InitTree();
	void InitGrid();
	void SetDataSave();

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDrawingOptionHyunTa)
	public:
	virtual BOOL OnSetActive();
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDrawingOptionHyunTa)
	virtual BOOL OnInitDialog();
	afx_msg void OnOptionDefault();
	afx_msg void OnSelchangeComboBridgeNo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWINGOPTIONHYUNTA_H__6BB7D694_122C_4400_8830_BF7D0DF37923__INCLUDED_)
