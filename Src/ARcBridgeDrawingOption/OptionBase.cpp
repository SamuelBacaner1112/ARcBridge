// OptionBase.cpp: implementation of the COptionBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARcBridgeDrawingOption.h"
#include "OptionBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionBase property page

IMPLEMENT_DYNCREATE(COptionBase, CPropertyPage)

COptionBase::COptionBase() : CPropertyPage(COptionBase::IDD)
{
	//{{AFX_DATA_INIT(COptionBase)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nCurBri = -1;
}

COptionBase::COptionBase(UINT nIDTemplate, UINT nIDCaption) : CPropertyPage(nIDTemplate, nIDCaption)
{
	m_nCurBri = -1;
}

COptionBase::COptionBase(LPCTSTR lpszTemplateName, UINT nIDCaption) : CPropertyPage(lpszTemplateName, nIDCaption)
{
	m_nCurBri = -1;
}

COptionBase::~COptionBase()
{
}

void COptionBase::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionBase)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


//BEGIN_MESSAGE_MAP(COptionBase, CPropertyPage)
	//{{AFX_MSG_MAP(COptionBase)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
//END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionBase message handlers
