// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DLGADDRATESHEET_H__641C15D1_42C1_454A_BCFC_284335A82C1B__INCLUDED_)
#define AFX_DLGADDRATESHEET_H__641C15D1_42C1_454A_BCFC_284335A82C1B__INCLUDED_

#include "DlgAddRatePageByDia.h"	// Added by ClassView
#include "DlgAddRatePageByMainRebar.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAddRateSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAddRateSheet

class CDlgAddRateSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CDlgAddRateSheet)

// Construction
public:
	CDlgAddRateSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDlgAddRateSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAddRateSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	double m_dRateUpper;
	double m_dRateLower;
	double m_dDiaOrg;
	long m_nIdxPage;
	CSafetyTypedPtrArray <CObArray, CRebar*> *m_pArrRebar;
	CDlgAddRatePageByMainRebar m_dlgAddRatePageByMainRebar;
	CDlgAddRatePageByDia m_dlgAddRatePageByDia;

	virtual ~CDlgAddRateSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDlgAddRateSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGADDRATESHEET_H__641C15D1_42C1_454A_BCFC_284335A82C1B__INCLUDED_)
