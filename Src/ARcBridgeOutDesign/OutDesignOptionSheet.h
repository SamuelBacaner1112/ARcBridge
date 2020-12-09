// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OUTDESIGNOPTIONSHEET_H__A35A1B91_A4D5_42C7_8DFB_FD4D94ADD728__INCLUDED_)
#define AFX_OUTDESIGNOPTIONSHEET_H__A35A1B91_A4D5_42C7_8DFB_FD4D94ADD728__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutDesignOptionSheet.h : header file
//

#include "OutDesignXLProPage_Etc.h"
#include "OutDesignXLPropPage.h"
#include "OutDesignXLPropPage_Stm.h"
#include "OutDesignXLProPage_Exel.h"

/////////////////////////////////////////////////////////////////////////////
// COutDesignOptionSheet

class COutDesignOptionSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(COutDesignOptionSheet)

// Construction
public:
	COutDesignOptionSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	COutDesignOptionSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	COutDesignXLPropPage m_XLOptionPage;
	COutDesignXLProPage_Etc m_XLEtcOptionPage;
	COutDesignXLProPage_Exel m_XLExelOptionPage;
	COutDesignXLPropPage_Stm	m_XLStmOptionPage;

	COptionItem *m_pEtcOption;
	COptionItem *m_pXLOption;
	COptionItem *m_pStmOption;

	CARcBridgeOutDesignStd *m_pStd;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COutDesignOptionSheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COutDesignOptionSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(COutDesignOptionSheet)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTDESIGNOPTIONSHEET_H__A35A1B91_A4D5_42C7_8DFB_FD4D94ADD728__INCLUDED_)
