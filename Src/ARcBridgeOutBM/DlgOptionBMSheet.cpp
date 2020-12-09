// DlgOptionBMSheet.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeoutbm.h"
#include "DlgOptionBMSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionBMSheet

IMPLEMENT_DYNAMIC(CDlgOptionBMSheet, CPropertySheet)

CDlgOptionBMSheet::CDlgOptionBMSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CDlgOptionBMSheet::CDlgOptionBMSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_psh.dwFlags &=~PSH_HASHELP;
	m_psh.dwFlags |= PSH_NOAPPLYNOW ;
	m_dlgViewBMPage.m_psp.dwFlags &=~PSP_HASHELP;
	//m_dlgSelectBMPage.m_psp.dwFlags &=~PSP_HASHELP;
	m_dlgOptionBMPage.m_psp.dwFlags &=~PSP_HASHELP;

	AddPage(&m_dlgViewBMPage);
	//AddPage(&m_dlgSelectBMPage);
	AddPage(&m_dlgOptionBMPage);

	m_dlgViewBMPage.m_pOpt		= this;
	m_dlgSelectBMPage.m_pOpt	= this;
	m_dlgOptionBMPage.m_pOpt	= this;
}

CDlgOptionBMSheet::~CDlgOptionBMSheet()
{
}


BEGIN_MESSAGE_MAP(CDlgOptionBMSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CDlgOptionBMSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionBMSheet message handlers

BOOL CDlgOptionBMSheet::OnInitDialog() 
{
	// TODO: Add your specialized code here
	CRect rect, rectOld;
	GetClientRect(&rectOld);
	SetWindowPos(&wndTop, -1, -1, 300, 235, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);	
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
