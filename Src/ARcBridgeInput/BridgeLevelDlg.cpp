// BridgeLevelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "BridgeLevelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBridgeLevelDlg dialog


CBridgeLevelDlg::CBridgeLevelDlg(CWnd* pParent /*=NULL*/)
	: CUserDialog(CBridgeLevelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBridgeLevelDlg)
	m_nType = -1;
	//}}AFX_DATA_INIT
	m_nTypeUnit = 0;
}


void CBridgeLevelDlg::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBridgeLevelDlg)
	DDX_Radio(pDX, IDC_RADIO1, m_nType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBridgeLevelDlg, CDialog)
	//{{AFX_MSG_MAP(CBridgeLevelDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBridgeLevelDlg message handlers

void CBridgeLevelDlg::OnOK() 
{
	UpdateData();
	if(m_nType==0)
		m_ReturnValue = "1 등교";
	else if(m_nType==1)
		m_ReturnValue = "2 등교";
	else 
		m_ReturnValue = "3 등교";
	
	CUserDialog::OnOK();
}

BOOL CBridgeLevelDlg::OnInitDialog() 
{
	CUserDialog::OnInitDialog();

	BOOL bSI = (m_nTypeUnit==1) ? TRUE : FALSE;
	GetDlgItem(IDC_STATIC_LOAD1)->SetWindowText(bSI ? "하중등급" : "하중");
	GetDlgItem(IDC_STATIC_LOAD2)->SetWindowText(bSI ? "총하중"	 : "총중량");
	GetDlgItem(IDC_STATIC_LOAD3)->SetWindowText(bSI ? "전륜하중" : "전륜하중");
	GetDlgItem(IDC_STATIC_LOAD4)->SetWindowText(bSI ? "후륜하중" : "총중량");
	//
	GetDlgItem(IDC_STATIC_TONF1)->SetWindowText(bSI ? "W(kN)"	 : "W(t)");
	GetDlgItem(IDC_STATIC_TONF2)->SetWindowText(bSI ? "1.8W(kN)" : "1.8W(t)");
	GetDlgItem(IDC_STATIC_TONF3)->SetWindowText(bSI ? "0.1W(kN)" : "0.1W(kg)");
	GetDlgItem(IDC_STATIC_TONF4)->SetWindowText(bSI ? "0.4W(kN)" : "0.4W(kg)");
	//
	GetDlgItem(IDC_STATIC_DB24_2)->SetWindowText(bSI ? "432" : "43.2");
	GetDlgItem(IDC_STATIC_DB24_3)->SetWindowText(bSI ? "24" : "2,400");
	GetDlgItem(IDC_STATIC_DB24_4)->SetWindowText(bSI ? "96" : "9,600");
	//
	GetDlgItem(IDC_STATIC_DB18_2)->SetWindowText(bSI ? "324" : "32.4");
	GetDlgItem(IDC_STATIC_DB18_3)->SetWindowText(bSI ? "18" : "1,800");
	GetDlgItem(IDC_STATIC_DB18_4)->SetWindowText(bSI ? "72" : "7,200");
	//
	GetDlgItem(IDC_STATIC_DB13_2)->SetWindowText(bSI ? "243" : "24.3");
	GetDlgItem(IDC_STATIC_DB13_3)->SetWindowText(bSI ? "13.5" : "1,350");
	GetDlgItem(IDC_STATIC_DB13_4)->SetWindowText(bSI ? "54" : "5,400");
		
	if(m_ReturnValue== "1 등교") 
		m_nType=0;
	else if(m_ReturnValue== "2 등교") 
		m_nType=1;
	else
		m_nType=2;

	UpdateData(FALSE);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
