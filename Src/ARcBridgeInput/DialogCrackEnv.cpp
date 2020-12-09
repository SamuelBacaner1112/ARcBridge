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
	if(m_nType==0)		m_ReturnValue = "���� ȯ��";
	else if(m_nType==1)	m_ReturnValue = "���� ȯ��";
	else if(m_nType==2)	m_ReturnValue = "�νļ� ȯ��";
	else if(m_nType==3)	m_ReturnValue = "��νļ� ȯ��";
	else if(m_nType==4)	m_ReturnValue = "������ ���� �Ǵ� ��ȣ��";
	else if(m_nType==5)	m_ReturnValue = "����, ���� ����, ��";
	else if(m_nType==6)	m_ReturnValue = "���� ȭ����";
	else if(m_nType==7)	m_ReturnValue = "�ؼ� �� �ؼ���� ; �ǽ�";
	else if(m_nType==8)	m_ReturnValue = "���� ��� ������(�з��� ���� ��Ʈ ����)";
	
	CUserDialog::OnOK();
}

BOOL CDialogCrackEnv::OnInitDialog() 
{
	CUserDialog::OnInitDialog();
	
	if(m_ReturnValue== "���� ȯ��") 						m_nType=0;
	else if(m_ReturnValue== "���� ȯ��") 					m_nType=1;		
	else if(m_ReturnValue== "�νļ� ȯ��") 					m_nType=2;		
	else if(m_ReturnValue== "��νļ� ȯ��") 				m_nType=3;		
	else if(m_ReturnValue== "������ ���� �Ǵ� ��ȣ��")		m_nType=4;		
	else if(m_ReturnValue== "����, ���� ����, ��") 			m_nType=5;		
	else if(m_ReturnValue== "���� ȭ����") 					m_nType=6;		
	else if(m_ReturnValue== "�ؼ� �� �ؼ���� ; �ǽ�") 		m_nType=7;		
	else if(m_ReturnValue== "���� ��� ������(�з��� ���� ��Ʈ ����)") 	m_nType=8;		

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}