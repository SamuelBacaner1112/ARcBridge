// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// DrawingOptionCns.h: interface for the CDrawingOptionCns class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWINGOPTIONCNS_H__9A7D0068_E302_4E81_9A18_55E5C14880B4__INCLUDED_)
#define AFX_DRAWINGOPTIONCNS_H__9A7D0068_E302_4E81_9A18_55E5C14880B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "OptionBase.h"
class CDrawingOptionCns : public COptionBase  
{
	DECLARE_DYNCREATE(CDrawingOptionCns)

// Construction
public:
	CDrawingOptionCns();
	~CDrawingOptionCns();

	CDocument* m_pDoc;
	CGridCtrlMng m_Grid;

// Dialog Data
	//{{AFX_DATA(CDrawingOptionCns)
	enum { IDD = IDD_OPTION_CNS };	
	//}}AFX_DATA
	CCustomOptions m_Options;	

	void InitTree();
	void InitGrid();
	void SetDataSave();

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDrawingOptionCns)
	public:
	virtual BOOL OnSetActive();
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDrawingOptionCns)
	virtual BOOL OnInitDialog();
	afx_msg void OnOptionDefault();
	afx_msg void OnSelchangeComboBridgeNo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_DRAWINGOPTIONCNS_H__9A7D0068_E302_4E81_9A18_55E5C14880B4__INCLUDED_)
