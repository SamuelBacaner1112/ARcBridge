// BoringTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ARcBridgeInput.h"
#include "BoringTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBoringTypeDlg dialog
#define	SOILCOUNT		37
CString strSoilName[SOILCOUNT] = 
	{
		"��Ʈ", "����", "�� ��Ż", "�ڰ��� �� ��ũ��Ʈ", "��Ʈ�� ��", "��Ʈ�� �ڰ�", "������ ��",
		"������ �ڰ�", "ǳȭ��", "����", "���", "������", "ǥ��", "�Ÿ���", "����", "ǳȭ��", 
		"��ź��", "�ڰ�", "ȣ�ڵ�", "������", "������", "�ذ��", "�����", "������", "����",
		"��Ʈ ���� ����(CL)", "���� ���� ��Ʈ(ML)", "���� ���� ��(SC)", "��Ʈ ���� ��(SM)", "���� ���� �ڰ�(GC)",
		"��(SP)", "��Ʈ ���� �ڰ�(GM)", "�𷡿� �ڰ� ���� ȣ�ڵ�", "ǳȭ �ܷ���(RS)",
		"ǳȭ��(WR)", "����(SR)", "���(HR)"
	};

CBoringTypeDlg::CBoringTypeDlg(CWnd* pParent /*=NULL*/)
	: CUserDialog(CBoringTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBoringTypeDlg)
	m_nBoringType = 0;
	//}}AFX_DATA_INIT
}


void CBoringTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBoringTypeDlg)
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Radio(pDX, IDC_RADIO1, m_nBoringType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBoringTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CBoringTypeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBoringTypeDlg message handlers
BOOL CBoringTypeDlg::OnInitDialog() 
{
	CUserDialog::OnInitDialog();

		
	long n=0; for(n=0; n<SOILCOUNT; n++)
	{
		if(m_ReturnValue==strSoilName[n]) m_nBoringType = n;
	}
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBoringTypeDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	m_ReturnValue = strSoilName[m_nBoringType];
	CUserDialog::OnOK();
}
