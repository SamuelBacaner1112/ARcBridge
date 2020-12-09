// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_DLGOPTIONBMSHEET_H__B66C996F_71CA_4331_BA66_5DD62E96D5B4__INCLUDED_)
#define AFX_DLGOPTIONBMSHEET_H__B66C996F_71CA_4331_BA66_5DD62E96D5B4__INCLUDED_

#include "DlgViewBMPage.h"	// Added by ClassView
#include "DlgSelectBMPage.h"	// Added by ClassView
#include "DlgOptionBMPage.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOptionBMSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionBMSheet

class CDlgOptionBMSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CDlgOptionBMSheet)

// Construction
public:
	CDlgOptionBMSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDlgOptionBMSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOptionBMSheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	CARcBridgeOutBMStd* m_pStd;
	CDlgOptionBMPage m_dlgOptionBMPage;
	CDlgSelectBMPage m_dlgSelectBMPage;
	CDlgViewBMPage m_dlgViewBMPage;
	virtual ~CDlgOptionBMSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDlgOptionBMSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPTIONBMSHEET_H__B66C996F_71CA_4331_BA66_5DD62E96D5B4__INCLUDED_)
