// DialogCrackEnv.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "DialogCrackEnv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogCrackEnv dialog


CDialogCrackEnv::CDialogCrackEnv(CWnd* pParent /*=NULL*/)
	: CUserDialog(CDialogCrackEnv::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogCrackEnv)
	m_nType = -1;
	//}}AFX_DATA_INIT
}


void CDialogCrackEnv::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogCrackEnv)
	DDX_Radio(pDX, IDC_RADIO_CRACK1, m_nType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogCrackEnv, CDialog)
	//{{AFX_MSG_MAP(CDialogCrackEnv)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogCrackEnv message handlers

void CDialogCrackEnv::OnOK() 
{
	UpdateData();
	if(m_nType==0)		m_ReturnValue = "건조 환경";
	else if(m_nType==1)	m_ReturnValue = "습윤 환경";
	else if(m_nType==2)	m_ReturnValue = "부식성 환경";
	else if(m_nType==3)	m_ReturnValue = "고부식성 환경";
	else if(m_nType==4)	m_ReturnValue = "건조한 공기 또는 보호막";
	else if(m_nType==5)	m_ReturnValue = "습기, 습한 공기, 흙";
	else if(m_nType==6)	m_ReturnValue = "제빙 화학제";
	else if(m_nType==7)	m_ReturnValue = "해수 및 해수살수 ; 건습";
	else if(m_nType==8)	m_ReturnValue = "물을 담는 구조물(압력이 없는 피트 포함)";
	
	CUserDialog::OnOK();
}

BOOL CDialogCrackEnv::OnInitDialog() 
{
	CUserDialog::OnInitDialog();
	
	if(m_ReturnValue== "건조 환경") 						m_nType=0;
	else if(m_ReturnValue== "습윤 환경") 					m_nType=1;		
	else if(m_ReturnValue== "부식성 환경") 					m_nType=2;		
	else if(m_ReturnValue== "고부식성 환경") 				m_nType=3;		
	else if(m_ReturnValue== "건조한 공기 또는 보호막")		m_nType=4;		
	else if(m_ReturnValue== "습기, 습한 공기, 흙") 			m_nType=5;		
	else if(m_ReturnValue== "제빙 화학제") 					m_nType=6;		
	else if(m_ReturnValue== "해수 및 해수살수 ; 건습") 		m_nType=7;		
	else if(m_ReturnValue== "물을 담는 구조물(압력이 없는 피트 포함)") 	m_nType=8;		

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}