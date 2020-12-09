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
	
	if(	m_ReturnValue== "I 구역") 
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
		m_ReturnValue = "I 구역";
	else
		m_ReturnValue = "II구역";

	CUserDialog::OnOK();
}

void CEarthquakeDlg::SetEQText()
{
	CString str;
	if(m_bKSD)
	{
		GetDlgItem(IDC_STATIC_EQ1)->SetWindowText(_T("울산광역시, 광주광역시, 세종특별자치시"));
		GetDlgItem(IDC_STATIC_EQ2)->SetWindowText(_T("전라북도, 전라남도"));
		GetDlgItem(IDC_STATIC_EQ3)->SetWindowText(_T("강원도 북부, 제주도"));

		str += _T("(2) 강원도 북부(군, 시) : 홍천, 철원, 화천, 횡성, 평창, 양구, 인제, 고성,\n\n");
		str += _T("                                   양양, 춘천시, 속초시\n\n");
		str += _T("(3) 행정구역의 경계를 통과하는 교량의 경우에는 구역계수가 큰 값을 적용한다.\n\n");
		GetDlgItem(IDC_STATIC_EQ4)->SetWindowText(str);
	}
	else
	{
		GetDlgItem(IDC_STATIC_EQ1)->SetWindowText(_T("울산광역시, 광주광역시"));
		GetDlgItem(IDC_STATIC_EQ2)->SetWindowText(_T("전라북도, 전라남도 북동부"));
		GetDlgItem(IDC_STATIC_EQ3)->SetWindowText(_T("강원도 북부, 전라남도 남서부, 제주도"));
		str += _T("(2) 전라남도 북동부(군, 시) : 장성, 담양, 곡성, 구례, 장홍, 보성, 여천,\n\n");
		str += _T("                                         화순, 광양시, 나주시, 여천시, 여수시, 순천시\n\n");
		str += _T("(3) 강원도 북부(군, 시) : 홍천, 철원, 화천, 횡성, 평창, 양구, 인제, 고성,\n\n");
		str += _T("                                   양양, 춘천시, 속초시\n\n");
		str += _T("(4) 전라남도 남서부(군, 시) : 무안, 신안, 완도, 영광, 진도, 해남, 영암,\n\n");
		str += _T("                                         강진, 고흥, 함평, 목포시\n\n");
		str += _T("(5) 행정구역의 경계를 통과하는 교량의 경우에는 구역계수가 큰 값을 적용한다.\n\n");
		GetDlgItem(IDC_STATIC_EQ4)->SetWindowText(str);
	}
}

void CEarthquakeDlg::SetKDS( BOOL bKDS )
{
	m_bKSD = bKDS;
}
