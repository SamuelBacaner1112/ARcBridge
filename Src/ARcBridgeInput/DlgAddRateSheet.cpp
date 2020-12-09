// DlgAddRateSheet.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "DlgAddRateSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAddRateSheet

IMPLEMENT_DYNAMIC(CDlgAddRateSheet, CPropertySheet)

CDlgAddRateSheet::CDlgAddRateSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CDlgAddRateSheet::CDlgAddRateSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_psh.dwFlags &=~PSH_HASHELP;
	m_psh.dwFlags |= PSH_NOAPPLYNOW ;

	m_dlgAddRatePageByDia.m_psp.dwFlags &=~PSP_HASHELP;
	m_dlgAddRatePageByMainRebar.m_psp.dwFlags &=~PSP_HASHELP;

	m_dlgAddRatePageByDia.m_pSheet			= this;
	m_dlgAddRatePageByMainRebar.m_pSheet	= this;

	long nIdx	= 0;
	AddPage(&m_dlgAddRatePageByDia);
	m_dlgAddRatePageByDia.m_nIdxCurPage	= nIdx++;
	AddPage(&m_dlgAddRatePageByMainRebar);
	m_dlgAddRatePageByMainRebar.m_nIdxCurPage	= nIdx++;

	m_pArrRebar	= NULL;
	m_nIdxPage	= -1;
}

CDlgAddRateSheet::~CDlgAddRateSheet()
{
}


BEGIN_MESSAGE_MAP(CDlgAddRateSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CDlgAddRateSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAddRateSheet message handlers
