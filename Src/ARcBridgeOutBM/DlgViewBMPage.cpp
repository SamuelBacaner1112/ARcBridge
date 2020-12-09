// DlgViewBMPage.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeoutbm.h"
#include "DlgViewBMPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgViewBMPage property page

IMPLEMENT_DYNCREATE(CDlgViewBMPage, CPropertyPage)

CDlgViewBMPage::CDlgViewBMPage() : CPropertyPage(CDlgViewBMPage::IDD)
{
	//{{AFX_DATA_INIT(CDlgViewBMPage)
	m_bNormalBM = TRUE;
	m_bRebarBM = TRUE;
	//}}AFX_DATA_INIT

	m_pViewBM = NULL;
	m_pOpt= NULL;
}

CDlgViewBMPage::~CDlgViewBMPage()
{
}

void CDlgViewBMPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgViewBMPage)
	DDX_Control(pDX, IDC_LIST_VIEW, m_listView);
	DDX_Check(pDX, IDC_CHECK_NORMAL_BM, m_bNormalBM);
	DDX_Check(pDX, IDC_CHECK_REBAR_BM, m_bRebarBM);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgViewBMPage, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgViewBMPage)
	ON_BN_CLICKED(ID_ALLNOSELECT, OnAllnoselect)
	ON_BN_CLICKED(ID_ALLSELECT, OnAllselect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgViewBMPage message handlers

void CDlgViewBMPage::OnAllnoselect() 
{
	// TODO: Add your control notification handler code here
	m_pViewBM->SetAllView(FALSE);
	InitListCtrl();
	SetControl();
}

void CDlgViewBMPage::OnAllselect() 
{
	// TODO: Add your control notification handler code here
	m_pViewBM->SetAllView(TRUE);
	InitListCtrl();
	SetControl();
}

void CDlgViewBMPage::SetDataSave()
{
	UpdateData(TRUE);

	for(int i = 0; i < m_listView.GetItemCount(); i++)
	{		
		BOOL bIsView = m_listView.GetCheck(i);
		m_pViewBM->SetBMState(i, bIsView);
	}

	m_pOpt->m_pStd->m_bCalNormalBM	= m_bNormalBM;
	m_pOpt->m_pStd->m_bCalRebarBM	= m_bRebarBM;
}

void CDlgViewBMPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	SetDataSave();
	CPropertyPage::OnOK();
}

void CDlgViewBMPage::InitListCtrl()
{
	LV_ITEM lvitem;

	long ExtStyle = 		
			LVS_EX_FULLROWSELECT | 
	//		LVS_EX_UNDERLINEHOT | 
			LVS_EX_INFOTIP |
			LVS_EX_FLATSB  |
	//		LVS_EX_GRIDLINES |
			LVS_EX_CHECKBOXES;
	//		LVS_EX_SUBITEMIMAGES;	
	m_listView.SetExtendedStyle(ExtStyle);

	m_listView.DeleteAllItems();
	m_listView.DeleteColumn(0);

	m_listView.InsertColumn(0,"출력항목",LVCFMT_LEFT,250);
		
	for(int i = 0; i < m_pViewBM->GetBMSize(); i++)
	{
		CString sBigClass	= _T("");
		BOOL bIsView		= FALSE;
		BOOL bIs			= TRUE;
		m_pViewBM->GetBMInfo(i, bIsView, sBigClass, bIs);


		if(1)
		{
			lvitem.mask		= LVIF_TEXT;	
			lvitem.iItem	= i;
			lvitem.iSubItem = 0;
			lvitem.pszText	= sBigClass.GetBuffer(sBigClass.GetLength());
			m_listView.InsertItem(&lvitem);		

			m_listView.SetCheck(i, bIsView);
		}
	}
}

void CDlgViewBMPage::SetControl()
{
	m_bNormalBM	= m_pOpt->m_pStd->m_bCalNormalBM;
	m_bRebarBM	= m_pOpt->m_pStd->m_bCalRebarBM;

	UpdateData(FALSE);
}

BOOL CDlgViewBMPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitListCtrl();
	SetControl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
