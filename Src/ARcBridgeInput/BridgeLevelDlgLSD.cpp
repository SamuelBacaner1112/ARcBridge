// BridgeLevelDlgLSD.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "resource.h"
#include "BridgeLevelDlgLSD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CBridgeLevelDlgLSD ��ȭ �����Դϴ�.

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

// CBridgeLevelDlgLSD �޽��� ó�����Դϴ�.
void CBridgeLevelDlgLSD::OnOK()
{
	UpdateData();
	if(m_nType==0)
		m_ReturnValue = "1 �";
	else if(m_nType==1)
		m_ReturnValue = "2 �";
	else 
		m_ReturnValue = "3 �";

	CUserDialog::OnOK();
}

BOOL CBridgeLevelDlgLSD::OnInitDialog()
{
	CUserDialog::OnInitDialog();

	if(m_ReturnValue== "1 �") 
		m_nType=0;
	else if(m_ReturnValue== "2 �") 
		m_nType=1;
	else
		m_nType=2;

	UpdateData(FALSE);	

	return TRUE;
}