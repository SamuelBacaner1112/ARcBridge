// DlgOptionBMPage.cpp : implementation file
//

#include "stdafx.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"
#include "arcbridgeoutbm.h"
#include "DlgOptionBMPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionBMPage property page

IMPLEMENT_DYNCREATE(CDlgOptionBMPage, CPropertyPage)

CDlgOptionBMPage::CDlgOptionBMPage() : CPropertyPage(CDlgOptionBMPage::IDD)
{
	//{{AFX_DATA_INIT(CDlgOptionBMPage)
	m_bConvertForm = FALSE;
	m_bConvertRef = FALSE;
	m_bDrawElEarth = FALSE;
	m_bRefCellBMSum = FALSE;
	m_bSepRebarMonument = FALSE;
	//}}AFX_DATA_INIT

	m_pOpt = NULL;
}

CDlgOptionBMPage::~CDlgOptionBMPage()
{
}

void CDlgOptionBMPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOptionBMPage)
	DDX_Check(pDX, IDC_CHECK_CONVERT_FORM, m_bConvertForm);
	DDX_Check(pDX, IDC_CHECK_CONVERT_REF, m_bConvertRef);
	DDX_Check(pDX, IDC_CHECK_DRAW_ELEARTH, m_bDrawElEarth);
	DDX_Check(pDX, IDC_CHECK_REFCELL_SUMCELL, m_bRefCellBMSum);
	DDX_Check(pDX, IDC_CHECK_SEP_REBAR_MONUMENT, m_bSepRebarMonument);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOptionBMPage, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgOptionBMPage)
	ON_BN_CLICKED(IDC_CHECK_REFCELL_SUMCELL, OnCheckRefcellSumcell)
	ON_BN_CLICKED(IDC_CHECK_SEP_REBAR_MONUMENT, OnCheckSepRebarMonument)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionBMPage message handlers

BOOL CDlgOptionBMPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgOptionBMPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	SetDataSave();
	CPropertyPage::OnOK();
}

void CDlgOptionBMPage::SetDataSave()
{
	UpdateData(TRUE);
	if(!m_pOpt)
		return;
	if(!m_pOpt->m_pStd)
		return;
	m_pOpt->m_pStd->m_bSeparateExcludeRebarMonument	= m_bSepRebarMonument;
	m_pOpt->m_pStd->m_bDrawElEarth		= m_bDrawElEarth;		// �𵵿� �����ݼ� ǥ������?
	m_pOpt->m_pStd->m_bConvertToForm	= m_bConvertForm;		// ������½� ���ĺ�ȯ
	//m_pOpt->m_pStd->m_bConvertToRef		= m_bConvertRef;		// ������½� ������������ ���?
	m_pOpt->m_pStd->m_bRefCellBMSum		= m_bRefCellBMSum;

	if(m_pOpt->m_pStd->m_pARcBridgeDataStd)
	{
		CARcBridgeDataStd *pDataStd	= m_pOpt->m_pStd->m_pARcBridgeDataStd;
		pDataStd->m_bSepFootingForSiljuk	= ((CButton*)GetDlgItem(IDC_CHECK_SEP_FOOTING))->GetCheck();
	}
}

void CDlgOptionBMPage::SetControl()
{
	if(!m_pOpt)
		return;
	if(!m_pOpt->m_pStd)
		return;
	m_bSepRebarMonument	= m_pOpt->m_pStd->m_bSeparateExcludeRebarMonument;
	m_bDrawElEarth		= m_pOpt->m_pStd->m_bDrawElEarth;
	m_bConvertForm		= m_pOpt->m_pStd->m_bConvertToForm;
	//m_bConvertRef		= m_pOpt->m_pStd->m_bConvertToRef;	// ����۵����� �����Ƿ� ����(�� ��ȹ�� ����)
	m_bRefCellBMSum		= m_pOpt->m_pStd->m_bRefCellBMSum;

	if(m_pOpt->m_pStd->m_pARcBridgeDataStd)
	{
		CARcBridgeDataStd *pDataStd	= m_pOpt->m_pStd->m_pARcBridgeDataStd;
		long nTypeBM	= pDataStd->m_nTypeBMApply;
		if(nTypeBM == TYPE_BM_NORMAL)
		{
			pDataStd->m_bSepFootingForSiljuk	= FALSE;
			GetDlgItem(IDC_CHECK_SEP_FOOTING)->EnableWindow(FALSE);
		}

		((CButton*)GetDlgItem(IDC_CHECK_SEP_FOOTING))->SetCheck(pDataStd->m_bSepFootingForSiljuk);
	}
	UpdateData(FALSE);
}

void CDlgOptionBMPage::OnCheckRefcellSumcell() 
{
	// TODO: Add your control notification handler code here
	
}

void CDlgOptionBMPage::OnCheckSepRebarMonument() 
{
	// TODO: Add your control notification handler code here
	
}
