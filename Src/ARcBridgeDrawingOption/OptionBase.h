// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// OptionBase.h: interface for the COptionBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIONBASE_H__A4241C02_80FB_4801_9F05_7EFAB93DA05A__INCLUDED_)
#define AFX_OPTIONBASE_H__A4241C02_80FB_4801_9F05_7EFAB93DA05A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionBase.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionBase dialog

class COptionBase : public CPropertyPage
{
	DECLARE_DYNCREATE(COptionBase)

// Construction
public:
	COptionBase();
	COptionBase(UINT nIDTemplate, UINT nIDCaption = 0);
	COptionBase(LPCTSTR lpszTemplateName, UINT nIDCaption = 0);
	~COptionBase();

// Dialog Data
	//{{AFX_DATA(COptionBase)	
	enum { IDD = 0 };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA
	CCustomOptionsTreeCtrl	m_Tree;
	long m_nCurBri;
	virtual void InitTree() {}
	virtual void InitGrid() {}
	virtual void SetDataSave() {}

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COptionBase)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COptionBase)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	//DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_OPTIONBASE_H__A4241C02_80FB_4801_9F05_7EFAB93DA05A__INCLUDED_)
