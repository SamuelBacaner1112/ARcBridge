// EarthquakeGradeKDS17Dlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "EarthquakeGradeKDS17Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CEarthquakeGradeKDS17Dlg 대화 상자입니다.

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


// CEarthquakeGradeKDS17Dlg 메시지 처리기입니다.


BOOL CEarthquakeGradeKDS17Dlg::OnInitDialog()
{
	CUserDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	if(m_ReturnValue== "I 등급") 
		m_nType=2;
	else if(m_ReturnValue== "II등급") 
		m_nType=3;
	else if(m_ReturnValue == "특등급 2400")
		m_nType=1;
	else if(m_ReturnValue == "특등급 4800")
		m_nType=0;

	UpdateData(FALSE);	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CEarthquakeGradeKDS17Dlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	UpdateData();

	if(m_nType==2)
		m_ReturnValue = "I 등급";
	else if(m_nType==3)
		m_ReturnValue = "II등급";
	else if(m_nType==0)
		m_ReturnValue = "특등급 4800";
	else if(m_nType==1)
		m_ReturnValue = "특등급 2400";

	CUserDialog::OnOK();
}
