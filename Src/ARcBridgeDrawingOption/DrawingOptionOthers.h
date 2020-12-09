// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DRAWINGOPTIONOTHERS_H__DFA047D4_CEE4_4400_828F_1056B282DF89__INCLUDED_)
#define AFX_DRAWINGOPTIONOTHERS_H__DFA047D4_CEE4_4400_828F_1056B282DF89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawingOptionOthers.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDrawingOptionOthers dialog
#include "OptionBase.h"
class CDrawingOptionOthers : public COptionBase
{
	DECLARE_DYNCREATE(CDrawingOptionOthers)

// Construction
public:
	CDrawingOptionOthers();
	~CDrawingOptionOthers();

	CDocument* m_pDoc;
	CGridCtrlMng m_Grid;

// Dialog Data
	//{{AFX_DATA(CDrawingOptionOthers)
	enum { IDD = IDD_OPTION_OTHERS };	
	//}}AFX_DATA
	CCustomOptions m_Options;	

	void InitTree();
	void InitGrid();
	void SetDataSave();

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDrawingOptionOthers)
	public:
	virtual BOOL OnSetActive();
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDrawingOptionOthers)
	virtual BOOL OnInitDialog();
	afx_msg void OnOptionDefault();
	afx_msg void OnSelchangeComboBridgeNo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWINGOPTIONOTHERS_H__DFA047D4_CEE4_4400_828F_1056B282DF89__INCLUDED_)
