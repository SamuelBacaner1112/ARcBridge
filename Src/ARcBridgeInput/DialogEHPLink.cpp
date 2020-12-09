// DialogEHPLink.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "DialogEHPLink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogEHPLink dialog


CDialogEHPLink::CDialogEHPLink(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogEHPLink::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogEHPLink)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogEHPLink::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogEHPLink)
	DDX_Control(pDX, IDC_BUTTON_LINK, m_btnLink);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_STATIC_TEXT_LINK, m_hyperURL);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogEHPLink, CDialog)
	//{{AFX_MSG_MAP(CDialogEHPLink)
	ON_BN_CLICKED(IDC_BUTTON_LINK, OnButtonLink)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogEHPLink message handlers

BOOL CDialogEHPLink::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	COLORREF clrLink = RGB(0x00, 0x00, 0xFF);
	//m_hyperURL.SetInitialize();
	m_hyperURL.SetColors(clrLink, clrLink, clrLink);
	m_hyperURL.EnableToolTips(FALSE);
	m_hyperURL.SetURL(_T("http://www.stnco.co.kr"));

	//m_bmpLink.DeleteObject();
	m_bmpLink.LoadBitmap(IDB_BITMAP_EHP);
	m_btnLink.SetBitmap(m_bmpLink);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CDialogEHPLink::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_bmpLink.DeleteObject();
	return CDialog::DestroyWindow();
}

void CDialogEHPLink::OnButtonLink() 
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL, NULL, "explorer.exe", "http://www.stnco.co.kr", NULL, SW_SHOW);
}
