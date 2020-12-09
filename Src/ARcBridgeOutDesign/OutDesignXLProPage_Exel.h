// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_OUTDESIGNXLPROPAGE_EXEL_H__BB668B81_45A8_4DCC_8AA1_E5652449A3AD__INCLUDED_)
#define AFX_OUTDESIGNXLPROPAGE_EXEL_H__BB668B81_45A8_4DCC_8AA1_E5652449A3AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutDesignXLProPage_Exel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COutDesignXLProPage_Exel dialog

class COutDesignXLProPage_Exel : public CPropertyPage
{
	DECLARE_DYNCREATE(COutDesignXLProPage_Exel)

// Construction
public:
	COutDesignXLProPage_Exel();
	~COutDesignXLProPage_Exel();

	COutDesignOptionSheet *m_pOpt;
// Dialog Data
	//{{AFX_DATA(COutDesignXLProPage_Exel)
	enum { IDD = IDD_PROPPAGE_EXEL };
	BOOL	m_bUseDefault;
	CString	m_szFont;
	CString	m_szLeft;
	CString	m_szBottom;
	CString	m_szRight;
	CString	m_szTop;
	int		m_nSizeFont;
	CString m_szPageRow;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COutDesignXLProPage_Exel)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COutDesignXLProPage_Exel)
	afx_msg void OnCheckDefaultExcel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTDESIGNXLPROPAGE_EXEL_H__BB668B81_45A8_4DCC_8AA1_E5652449A3AD__INCLUDED_)
