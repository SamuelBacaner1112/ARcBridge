// EarthquakeGradeKDS17Dlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "EarthquakeGradeKDS17Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CEarthquakeGradeKDS17Dlg ��ȭ �����Դϴ�.

CEarthquakeGradeKDS17Dlg::CEarthquakeGradeKDS17Dlg(CWnd* pParent /*=NULL*/)
	: CUserDialog(CEarthquakeGradeKDS17Dlg::IDD, pParent)
{
	m_nType = -1;
}

CEarthquakeGradeKDS17Dlg::~CEarthquakeGradeKDS17Dlg()
{
}

void CEarthquakeGradeKDS17Dlg::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);

	DDX_Radio(pDX, IDC_RADIO1, m_nType);
}


BEGIN_MESSAGE_MAP(CEarthquakeGradeKDS17Dlg, CDialog)
END_MESSAGE_MAP()


// CEarthquakeGradeKDS17Dlg �޽��� ó�����Դϴ�.


BOOL CEarthquakeGradeKDS17Dlg::OnInitDialog()
{
	CUserDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	if(m_ReturnValue== "I ���") 
		m_nType=2;
	else if(m_ReturnValue== "II���") 
		m_nType=3;
	else if(m_ReturnValue == "Ư��� 2400")
		m_nType=1;
	else if(m_ReturnValue == "Ư��� 4800")
		m_nType=0;

	UpdateData(FALSE);	

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CEarthquakeGradeKDS17Dlg::OnOK()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	UpdateData();

	if(m_nType==2)
		m_ReturnValue = "I ���";
	else if(m_nType==3)
		m_ReturnValue = "II���";
	else if(m_nType==0)
		m_ReturnValue = "Ư��� 4800";
	else if(m_nType==1)
		m_ReturnValue = "Ư��� 2400";

	CUserDialog::OnOK();
}
