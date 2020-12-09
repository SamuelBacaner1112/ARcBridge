// BridgeTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "BridgeTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBridgeTypeDlg dialog


CBridgeTypeDlg::CBridgeTypeDlg(CWnd* pParent /*=NULL*/)
	: CUserDialog(CBridgeTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBridgeTypeDlg)
	m_nBridgeType = 0;
	//}}AFX_DATA_INIT
	m_pARcBridgeDataStd	= NULL;
}


void CBridgeTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBridgeTypeDlg)
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Radio(pDX, IDC_RADIO_TYPE1, m_nBridgeType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBridgeTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CBridgeTypeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBridgeTypeDlg message handlers

BOOL CBridgeTypeDlg::OnInitDialog() 
{
	CUserDialog::OnInitDialog();

	HGBOOL bLSD = m_pARcBridgeDataStd->IsLsdDesign();
	if(m_pARcBridgeDataStd)
	{
		if(m_pARcBridgeDataStd->IsSupportTypeStd(BRIDGE_TYPE_PORTAL))
		{
			GetDlgItem(IDC_RADIO_TYPE1)->EnableWindow(TRUE);
		}

		if(m_pARcBridgeDataStd->IsSupportTypeStd(BRIDGE_TYPE_PI))
		{
			GetDlgItem(IDC_RADIO_TYPE2)->EnableWindow(TRUE);
		}

		if(m_pARcBridgeDataStd->IsSupportTypeStd(BRIDGE_TYPE_SLAB))
		{
			GetDlgItem(IDC_RADIO_TYPE3)->EnableWindow(TRUE);
		}

		if(m_pARcBridgeDataStd->IsSupportTypeStd(BRIDGE_TYPE_TRAP))
		{
			GetDlgItem(IDC_RADIO_TYPE4)->EnableWindow(TRUE);
		}

		if(m_pARcBridgeDataStd->IsSupportTypeStd(BRIDGE_TYPE_TRAP2))
		{
			GetDlgItem(IDC_RADIO_TYPE5)->EnableWindow(TRUE);
		}

		if(m_pARcBridgeDataStd->IsSupportTypeStd(BRIDGE_TYPE_ARCH) && bLSD==FALSE)
		{
			GetDlgItem(IDC_RADIO_TYPE6)->EnableWindow(TRUE);
		}

		if(m_pARcBridgeDataStd->IsSupportTypeStd(BRIDGE_TYPE_BOX))
		{
			GetDlgItem(IDC_RADIO_TYPE7)->EnableWindow(TRUE);
		}
	}

	if(m_ReturnValue==_T("���� ��౳"))
		m_nBridgeType = 0;
	else if(m_ReturnValue==_T("���� ��౳"))
		m_nBridgeType = 1;
	else if(m_ReturnValue==_T("�����걳"))
		m_nBridgeType = 2;
	else if(m_ReturnValue==_T("��� ���� ��౳"))
		m_nBridgeType = 3;
	else if(m_ReturnValue==_T("��� ���� ��౳2"))
		m_nBridgeType = 4;
	else if(m_ReturnValue==_T("��ġ�� ��౳"))
		m_nBridgeType = 5;
	else if(m_ReturnValue==_T("BOX�� ��౳"))
		m_nBridgeType = 6;

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBridgeTypeDlg::OnOK() 
{
	UpdateData();

	if(m_nBridgeType==0)
		m_ReturnValue = _T("���� ��౳");
	else if(m_nBridgeType==1)
		m_ReturnValue = _T("���� ��౳");
	else if(m_nBridgeType==2)
		m_ReturnValue = _T("�����걳");
	else if(m_nBridgeType==3)
		m_ReturnValue = _T("��� ���� ��౳");
	else if(m_nBridgeType==4)
		m_ReturnValue = _T("��� ���� ��౳2");
	else if(m_nBridgeType==5)
		m_ReturnValue = _T("��ġ�� ��౳");
	else if(m_nBridgeType==6)
		m_ReturnValue = _T("BOX�� ��౳");

	CUserDialog::OnOK();
}

