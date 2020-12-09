// OutDesignOptionSheet.cpp : implementation file
//

#include "StdAfx.h"
#include "arcbridgeOutDesign.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutDesignOptionSheet

IMPLEMENT_DYNAMIC(COutDesignOptionSheet, CPropertySheet)

COutDesignOptionSheet::COutDesignOptionSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

COutDesignOptionSheet::COutDesignOptionSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_psh.dwFlags &=~PSH_HASHELP;
	m_psh.dwFlags |= PSH_NOAPPLYNOW ;
	m_XLOptionPage.m_psp.dwFlags &=~PSP_HASHELP;
	m_XLEtcOptionPage.m_psp.dwFlags &=~PSP_HASHELP;
	m_XLStmOptionPage.m_psp.dwFlags &=~PSP_HASHELP;

	AddPage(&m_XLOptionPage);
	AddPage(&m_XLStmOptionPage);
	AddPage(&m_XLEtcOptionPage);
	AddPage(&m_XLExelOptionPage);

	m_XLOptionPage.m_pOpt		= this;
	m_XLEtcOptionPage.m_pOpt	= this;
	m_XLExelOptionPage.m_pOpt	= this;
	m_XLStmOptionPage.m_pOpt	= this;
}

COutDesignOptionSheet::~COutDesignOptionSheet()
{
}


BEGIN_MESSAGE_MAP(COutDesignOptionSheet, CPropertySheet)
	//{{AFX_MSG_MAP(COutDesignOptionSheet)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutDesignOptionSheet message handlers

BOOL COutDesignOptionSheet::OnInitDialog() 
{
	HGBOOL bWindows10Ver = IsWindows10Version();
	HGINT32 cy = bWindows10Ver ? 289 : 243;

	CRect rect, rectOld;
	GetClientRect(&rectOld);
	SetWindowPos(&wndTop, -1, -1, 334, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);	
	GetClientRect(&rect);
	int cxDiff = rectOld.Width() - rect.Width();

	long nMargin = 10;
	HWND hWnd = (HWND)::GetDlgItem(m_hWnd, AFX_IDC_TAB_CONTROL);
	::MoveWindow(hWnd, nMargin, nMargin, rect.Width()-nMargin*2, rect.Height()-40, TRUE);

	BOOL bResult = CPropertySheet::OnInitDialog(); 

	int SheetButtons[] = { IDOK, IDCANCEL, ID_APPLY_NOW, IDHELP };
	long i=0; for(i=0; i<4; i++)
	{
		hWnd = ::GetDlgItem(m_hWnd, SheetButtons[i]);
		if (hWnd != NULL)
		{
			::GetWindowRect(hWnd, &rectOld);
			ScreenToClient(&rectOld);
			::SetWindowPos(hWnd, NULL,
				rectOld.left - cxDiff-5, rectOld.top,
				0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}
	// TODO: Add your specialized code here
	return bResult;
}

void COutDesignOptionSheet::OnSize(UINT nType, int cx, int cy) 
{
	CPropertySheet::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
