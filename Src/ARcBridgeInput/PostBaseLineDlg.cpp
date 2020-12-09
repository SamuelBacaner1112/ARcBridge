// PostBaseLineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "PostBaseLineDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPostBaseLineDlg dialog


CPostBaseLineDlg::CPostBaseLineDlg(CWnd* pParent /*=NULL*/)
	: CUserDialog(CPostBaseLineDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPostBaseLineDlg)
	m_nType = -1;
	//}}AFX_DATA_INIT
}


void CPostBaseLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPostBaseLineDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_nType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPostBaseLineDlg, CDialog)
	//{{AFX_MSG_MAP(CPostBaseLineDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPostBaseLineDlg message handlers

void CPostBaseLineDlg::OnOK() 
{
	UpdateData();
	if(m_nType==0)		m_ReturnValue = "���ݰ� ����";
	else if(m_nType==1)	m_ReturnValue = "���ʻ�� ����";
	else if(m_nType==2)	m_ReturnValue = "������ġ �ڵ�����";

	CUserDialog::OnOK();
}

BOOL CPostBaseLineDlg::OnInitDialog() 
{
	CUserDialog::OnInitDialog();
	
	if(m_ReturnValue== "���ݰ� ����") 				m_nType=0;
	else if(m_ReturnValue== "���ʻ�� ����") 		m_nType=1;
	else if(m_ReturnValue== "������ġ �ڵ�����")	m_nType=2;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


