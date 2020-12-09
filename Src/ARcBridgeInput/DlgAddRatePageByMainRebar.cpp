// DlgAddRatePageByMainRebar.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "DlgAddRatePageByMainRebar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAddRatePageByMainRebar property page

IMPLEMENT_DYNCREATE(CDlgAddRatePageByMainRebar, CPropertyPage)

CDlgAddRatePageByMainRebar::CDlgAddRatePageByMainRebar() : CPropertyPage(CDlgAddRatePageByMainRebar::IDD)
{
	//{{AFX_DATA_INIT(CDlgAddRatePageByMainRebar)
	m_dRate2 = 0.0;
	m_dRate1 = 0.0;
	//}}AFX_DATA_INIT
	m_nIdxCurPage	= 1;

	m_pSheet = NULL;
}

CDlgAddRatePageByMainRebar::~CDlgAddRatePageByMainRebar()
{
}

void CDlgAddRatePageByMainRebar::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAddRatePageByMainRebar)
	DDX_Text(pDX, IDC_EDIT_RATE2, m_dRate2);
	DDX_Text(pDX, IDC_EDIT_RATE1, m_dRate1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAddRatePageByMainRebar, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgAddRatePageByMainRebar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAddRatePageByMainRebar message handlers

void CDlgAddRatePageByMainRebar::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pSheet)
	{
		if(m_pSheet->m_pArrRebar && m_pSheet->m_nIdxPage == m_nIdxCurPage)
		{
			UpdateData(TRUE);
			double dRateMainRebar = (100+m_dRate1)/100.0;
			double dRateEtcRebar = (100+m_dRate2)/100.0;

			long nSize	= m_pSheet->m_pArrRebar->GetSize();
			long i = 0; for(i = 0; i < nSize; i++)
			{
				CRebar *pRB	= m_pSheet->m_pArrRebar->GetAt(i);
				if(pRB->m_bMainRebar)
					pRB->m_dRateAdd	= dRateMainRebar;
				else
					pRB->m_dRateAdd	= dRateEtcRebar;
			}
			m_pSheet->m_dRateLower	= m_dRate1;
			m_pSheet->m_dRateUpper	= m_dRate2;
			m_pSheet->m_nIdxPage	= m_nIdxCurPage;
		}
	}	
	CPropertyPage::OnOK();
}

BOOL CDlgAddRatePageByMainRebar::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_dRate1	= m_pSheet->m_dRateLower;
	m_dRate2	= m_pSheet->m_dRateUpper;		

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgAddRatePageByMainRebar::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pSheet)
		m_pSheet->m_nIdxPage	= m_nIdxCurPage;
	
	return CPropertyPage::OnSetActive();
}
