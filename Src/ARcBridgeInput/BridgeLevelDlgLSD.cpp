// BridgeLevelDlgLSD.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "resource.h"
#include "BridgeLevelDlgLSD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CBridgeLevelDlgLSD 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBridgeLevelDlgLSD, CUserDialog)

CBridgeLevelDlgLSD::CBridgeLevelDlgLSD(CWnd* pParent /*=NULL*/)
	: CUserDialog(CBridgeLevelDlgLSD::IDD, pParent)
{
	m_nType = -1;
}

CBridgeLevelDlgLSD::~CBridgeLevelDlgLSD()
{
}

void CBridgeLevelDlgLSD::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nType);
}


BEGIN_MESSAGE_MAP(CBridgeLevelDlgLSD, CUserDialog)
END_MESSAGE_MAP()

// CBridgeLevelDlgLSD 메시지 처리기입니다.
void CBridgeLevelDlgLSD::OnOK()
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

BOOL CBridgeLevelDlgLSD::OnInitDialog()
{
	CUserDialog::OnInitDialog();

	if(m_ReturnValue== "1 등교") 
		m_nType=0;
	else if(m_ReturnValue== "2 등교") 
		m_nType=1;
	else
		m_nType=2;

	UpdateData(FALSE);	

	return TRUE;
}