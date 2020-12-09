// EarthquakeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "EarthquakeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEarthquakeDlg dialog


CEarthquakeDlg::CEarthquakeDlg(CWnd* pParent /*=NULL*/)
	: CUserDialog(CEarthquakeDlg::IDD, pParent)
{
	m_nType = -1;
	m_bKSD = FALSE;
}


void CEarthquakeDlg::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nType);
}


BEGIN_MESSAGE_MAP(CEarthquakeDlg, CDialog)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEarthquakeDlg message handlers

BOOL CEarthquakeDlg::OnInitDialog() 
{
	CUserDialog::OnInitDialog();
	
	if(	m_ReturnValue== "I ����") 
		m_nType=0;
	else
		m_nType=1;

	SetEQText();

	SetWindowPos(&wndTop, 0, 0, 500, m_bKSD ? 440 : 530, SWP_NOMOVE);
	GetDlgItem(IDC_STATIC_EQ4)->SetWindowPos(&wndTop, -1, -1, 430, m_bKSD ? 90 : 200, SWP_NOMOVE);
	GetDlgItem(IDOK)->SetWindowPos(&wndTop, 300, m_bKSD ? 370 : 470, -1, -1, SWP_NOSIZE);
	GetDlgItem(IDCANCEL)->SetWindowPos(&wndTop, 400, m_bKSD ? 370 : 470, -1, -1, SWP_NOSIZE);

	UpdateData(FALSE);			
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEarthquakeDlg::OnOK() 
{	
	UpdateData();
	if(m_nType==0)
		m_ReturnValue = "I ����";
	else
		m_ReturnValue = "II����";

	CUserDialog::OnOK();
}

void CEarthquakeDlg::SetEQText()
{
	CString str;
	if(m_bKSD)
	{
		GetDlgItem(IDC_STATIC_EQ1)->SetWindowText(_T("��걤����, ���ֱ�����, ����Ư����ġ��"));
		GetDlgItem(IDC_STATIC_EQ2)->SetWindowText(_T("����ϵ�, ���󳲵�"));
		GetDlgItem(IDC_STATIC_EQ3)->SetWindowText(_T("������ �Ϻ�, ���ֵ�"));

		str += _T("(2) ������ �Ϻ�(��, ��) : ȫõ, ö��, ȭõ, Ⱦ��, ��â, �籸, ����, ��,\n\n");
		str += _T("                                   ���, ��õ��, ���ʽ�\n\n");
		str += _T("(3) ���������� ��踦 ����ϴ� ������ ��쿡�� ��������� ū ���� �����Ѵ�.\n\n");
		GetDlgItem(IDC_STATIC_EQ4)->SetWindowText(str);
	}
	else
	{
		GetDlgItem(IDC_STATIC_EQ1)->SetWindowText(_T("��걤����, ���ֱ�����"));
		GetDlgItem(IDC_STATIC_EQ2)->SetWindowText(_T("����ϵ�, ���󳲵� �ϵ���"));
		GetDlgItem(IDC_STATIC_EQ3)->SetWindowText(_T("������ �Ϻ�, ���󳲵� ������, ���ֵ�"));
		str += _T("(2) ���󳲵� �ϵ���(��, ��) : �强, ���, �, ����, ��ȫ, ����, ��õ,\n\n");
		str += _T("                                         ȭ��, �����, ���ֽ�, ��õ��, ������, ��õ��\n\n");
		str += _T("(3) ������ �Ϻ�(��, ��) : ȫõ, ö��, ȭõ, Ⱦ��, ��â, �籸, ����, ��,\n\n");
		str += _T("                                   ���, ��õ��, ���ʽ�\n\n");
		str += _T("(4) ���󳲵� ������(��, ��) : ����, �ž�, �ϵ�, ����, ����, �س�, ����,\n\n");
		str += _T("                                         ����, ����, ����, ������\n\n");
		str += _T("(5) ���������� ��踦 ����ϴ� ������ ��쿡�� ��������� ū ���� �����Ѵ�.\n\n");
		GetDlgItem(IDC_STATIC_EQ4)->SetWindowText(str);
	}
}

void CEarthquakeDlg::SetKDS( BOOL bKDS )
{
	m_bKSD = bKDS;
}
