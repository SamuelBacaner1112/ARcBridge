// DlgAddRatePageByDia.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "DlgAddRatePageByDia.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAddRatePageByDia property page

IMPLEMENT_DYNCREATE(CDlgAddRatePageByDia, CPropertyPage)

CDlgAddRatePageByDia::CDlgAddRatePageByDia() : CPropertyPage(CDlgAddRatePageByDia::IDD)
{
	//{{AFX_DATA_INIT(CDlgAddRatePageByDia)
	m_dRate1 = 0.0;
	m_dRate2 = 0.0;
	//}}AFX_DATA_INIT
	// 지름콤보용 값들.
	const long nCountDia	= 12;
	double dDia[nCountDia]	= {10, 13, 16, 19, 22, 25, 29, 32, 35, 38, 41, 51};
	long i = 0; for(i = 0; i < nCountDia; i++)
	{
		m_dArrDia.Add(dDia[i]);
	}
	m_nIdxCurPage	= 0;

	m_dDiaOrg = 0.;
	m_pSheet = NULL;
}

CDlgAddRatePageByDia::~CDlgAddRatePageByDia()
{

}

void CDlgAddRatePageByDia::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAddRatePageByDia)
	DDX_Control(pDX, IDC_COMBO_DIA2, m_comboDia2);
	DDX_Control(pDX, IDC_COMBO_DIA1, m_comboDia1);
	DDX_Text(pDX, IDC_EDIT_RATE1, m_dRate1);
	DDX_Text(pDX, IDC_EDIT_RATE2, m_dRate2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAddRatePageByDia, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgAddRatePageByDia)
	ON_CBN_SELCHANGE(IDC_COMBO_DIA1, OnSelchangeComboDia1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAddRatePageByDia message handlers

void CDlgAddRatePageByDia::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pSheet)
	{
		if(m_pSheet->m_pArrRebar && m_pSheet->m_nIdxPage == m_nIdxCurPage)
		{
			UpdateData(TRUE);
			double dDiaLower	= GetDia(TRUE);
			double dDiaUpper	= GetDia(FALSE);
			double dRateAddLower = (100+m_dRate1)/100.0;
			double dRateAddUpper = (100+m_dRate2)/100.0;

			long nSize	= m_pSheet->m_pArrRebar->GetSize();
			long i = 0; for(i = 0; i < nSize; i++)
			{
				CRebar *pRB	= m_pSheet->m_pArrRebar->GetAt(i);
				if(pRB->m_dDia <= dDiaLower)
					pRB->m_dRateAdd	= dRateAddLower;
				else if(pRB->m_dDia >= dDiaUpper)
					pRB->m_dRateAdd	= dRateAddUpper;
			}

			m_pSheet->m_dRateLower	= m_dRate1;
			m_pSheet->m_dRateUpper	= m_dRate2;
			m_pSheet->m_dDiaOrg		= dDiaLower;
			m_pSheet->m_nIdxPage	= m_nIdxCurPage;
		}
	}
	
	CPropertyPage::OnOK();
}

BOOL CDlgAddRatePageByDia::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_comboDia2.EnableWindow(FALSE);
	m_dDiaOrg	= m_pSheet->m_dDiaOrg;
	m_dRate1	= m_pSheet->m_dRateLower;
	m_dRate2	= m_pSheet->m_dRateUpper;	


	// TODO: Add extra initialization here
	long nSize	= m_dArrDia.GetSize();
	CString str	= _T("");
	long i = 0; for(i = 0; i < nSize; i++)
	{
		str.Format("D%0.f", m_dArrDia.GetAt(i));
		if(i < nSize-1)
			m_comboDia1.AddString(str);
		m_comboDia2.AddString(str);
	}

	long nIdxDiaOrg	= GetIdxDia(m_dDiaOrg);
	if(nIdxDiaOrg > nSize - 1)
		nIdxDiaOrg	= nSize - 2;
	if(nIdxDiaOrg < 0)
		nIdxDiaOrg	= 0;

	m_comboDia1.SetCurSel(nIdxDiaOrg);
	m_comboDia2.SetCurSel(nIdxDiaOrg+1);
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// 지름에 해당하는 인덱스 리턴
long CDlgAddRatePageByDia::GetIdxDia(double dDia)
{
	long i = 0; for(i = 0; i < m_dArrDia.GetSize(); i++)
	{
		if(Compare(dDia, m_dArrDia.GetAt(i), "=")) return i;
	}

	return -1;
}

double CDlgAddRatePageByDia::GetDia(BOOL bLower)
{
	long nIdxDia	= bLower ? m_comboDia1.GetCurSel() : m_comboDia2.GetCurSel();
	double dDia		= m_dArrDia.GetAt(nIdxDia);
	
	return dDia;
}

void CDlgAddRatePageByDia::OnSelchangeComboDia1() 
{
	// TODO: Add your control notification handler code here
	m_comboDia2.SetCurSel(m_comboDia1.GetCurSel()+1);
	
}


BOOL CDlgAddRatePageByDia::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pSheet)
		m_pSheet->m_nIdxPage	= m_nIdxCurPage;

	return CPropertyPage::OnSetActive();
}
