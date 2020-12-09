// DlgSelectBMPage.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeoutbm.h"
#include "DlgSelectBMPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectBMPage property page

IMPLEMENT_DYNCREATE(CDlgSelectBMPage, CPropertyPage)

CDlgSelectBMPage::CDlgSelectBMPage() : CPropertyPage(CDlgSelectBMPage::IDD)
{
	//{{AFX_DATA_INIT(CDlgSelectBMPage)
	//}}AFX_DATA_INIT

	m_pOpt = NULL;
}

CDlgSelectBMPage::~CDlgSelectBMPage()
{
}

void CDlgSelectBMPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelectBMPage)
	DDX_Control(pDX, IDC_LIST_NORMAL_BM, m_listNormalBM);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelectBMPage, CPropertyPage)
	//{{AFX_MSG_MAP(CDlgSelectBMPage)
	ON_BN_CLICKED(ID_ALLNOSELECT, OnAllnoselect)
	ON_BN_CLICKED(ID_ALLSELECT, OnAllselect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectBMPage message handlers

void CDlgSelectBMPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	SetDataSave();
	CPropertyPage::OnOK();
}

void CDlgSelectBMPage::OnAllnoselect() 
{
	// TODO: Add your control notification handler code here
	
}

void CDlgSelectBMPage::OnAllselect() 
{
	// TODO: Add your control notification handler code here
	
}

void CDlgSelectBMPage::SetDataSave()
{

}
