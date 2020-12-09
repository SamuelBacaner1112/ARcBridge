// FinitePileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "FinitePileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFinitePileDlg dialog


CFinitePileDlg::CFinitePileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFinitePileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFinitePileDlg)
		// NOTE: the ClassWizard will add member initialization here
	for(long i=0; i<2; i++)
	{
		m_sFixPhi1[i] = _T("");
		m_sFixPhi2[i] = _T("");
		m_sFixPhi3[i] = _T("");
		m_sFixPhi4[i] = _T("");
		m_sHingePhi1[i] = _T("");
		m_sHingePhi2[i] = _T("");
		m_sHingePhi3[i] = _T("");
		m_sHingePhi4[i] = _T("");
	}
	//}}AFX_DATA_INIT

	m_pExPile = NULL;
}
CFinitePileDlg::CFinitePileDlg(CExPileApp* pExPile, CWnd* pParent /*=NULL*/)
	: CDialog(CFinitePileDlg::IDD, pParent)
{
	m_pExPile = pExPile;
}

void CFinitePileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFinitePileDlg)
	DDX_Control(pDX, IDC_RECOMMAND, m_btnRecommand);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Text(pDX, IDC_EDIT_FIX_NOR_PHI1, m_sFixPhi1[0]);
	DDX_Text(pDX, IDC_EDIT_FIX_NOR_PHI2, m_sFixPhi2[0]);
	DDX_Text(pDX, IDC_EDIT_FIX_NOR_PHI3, m_sFixPhi3[0]);
	DDX_Text(pDX, IDC_EDIT_FIX_NOR_PHI4, m_sFixPhi4[0]);
	DDX_Text(pDX, IDC_EDIT_HINGE_NOR_PHI1, m_sHingePhi1[0]);
	DDX_Text(pDX, IDC_EDIT_HINGE_NOR_PHI2, m_sHingePhi2[0]);
	DDX_Text(pDX, IDC_EDIT_HINGE_NOR_PHI3, m_sHingePhi3[0]);
	DDX_Text(pDX, IDC_EDIT_HINGE_NOR_PHI4, m_sHingePhi4[0]);
	DDX_Text(pDX, IDC_EDIT_FIX_PHI1, m_sFixPhi1[1]);
	DDX_Text(pDX, IDC_EDIT_FIX_PHI2, m_sFixPhi2[1]);
	DDX_Text(pDX, IDC_EDIT_FIX_PHI3, m_sFixPhi3[1]);
	DDX_Text(pDX, IDC_EDIT_FIX_PHI4, m_sFixPhi4[1]);
	DDX_Text(pDX, IDC_EDIT_HINGE_PHI1, m_sHingePhi1[1]);
	DDX_Text(pDX, IDC_EDIT_HINGE_PHI2, m_sHingePhi2[1]);
	DDX_Text(pDX, IDC_EDIT_HINGE_PHI3, m_sHingePhi3[1]);
	DDX_Text(pDX, IDC_EDIT_HINGE_PHI4, m_sHingePhi4[1]);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFinitePileDlg, CDialog)
	//{{AFX_MSG_MAP(CFinitePileDlg)
	ON_BN_CLICKED(IDC_RECOMMAND, OnRecommand)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFinitePileDlg message handlers
void CFinitePileDlg::OnRecommand() 
{
	// TODO: Add your control notification handler code here
	for(long i=0; i<2; i++)
	{
		m_sFixPhi1[i].Format("%.3f", 1.0);
		m_sFixPhi2[i].Format("%.3f", 1.0);
		m_sFixPhi3[i].Format("%.3f", 1.0);
		m_sFixPhi4[i].Format("%.3f", 1.0);

		m_sHingePhi1[i].Format("%.3f", 1.0);
		m_sHingePhi2[i].Format("%.3f", 1.0);
		m_sHingePhi3[i].Format("%.3f", 1.0);
		m_sHingePhi4[i].Format("%.3f", 1.0);
	}

	UpdateData(FALSE);
}

void CFinitePileDlg::InitData()
{
	for(long i=0; i<2; i++)
	{
		m_sFixPhi1[i].Format("%.3f", m_pExPile->m_dphi1_Fix[i]);
		m_sFixPhi2[i].Format("%.3f", m_pExPile->m_dphi2_Fix[i]);
		m_sFixPhi3[i].Format("%.3f", m_pExPile->m_dphi3_Fix[i]);
		m_sFixPhi4[i].Format("%.3f", m_pExPile->m_dphi4_Fix[i]);

		m_sHingePhi1[i].Format("%.3f", m_pExPile->m_dphi1_Hinge[i]);
		m_sHingePhi2[i].Format("%.3f", m_pExPile->m_dphi2_Hinge[i]);
		m_sHingePhi3[i].Format("%.3f", m_pExPile->m_dphi3_Hinge[i]);
		m_sHingePhi4[i].Format("%.3f", m_pExPile->m_dphi4_Hinge[i]);
	}

	UpdateData(FALSE);
}

void CFinitePileDlg::SaveData()
{
	UpdateData();
	
	for(long i=0; i<2; i++)
	{
		m_pExPile->m_dphi1_Fix[i] = atof((LPCSTR)m_sFixPhi1[i]);
		m_pExPile->m_dphi2_Fix[i] = atof((LPCSTR)m_sFixPhi2[i]);
		m_pExPile->m_dphi3_Fix[i] = atof((LPCSTR)m_sFixPhi3[i]);
		m_pExPile->m_dphi4_Fix[i] = atof((LPCSTR)m_sFixPhi4[i]);

		m_pExPile->m_dphi1_Hinge[i] = atof((LPCSTR)m_sHingePhi1[i]);
		m_pExPile->m_dphi2_Hinge[i] = atof((LPCSTR)m_sHingePhi2[i]);
		m_pExPile->m_dphi3_Hinge[i] = atof((LPCSTR)m_sHingePhi3[i]);
		m_pExPile->m_dphi4_Hinge[i] = atof((LPCSTR)m_sHingePhi4[i]);
	}
}

BOOL CFinitePileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	InitData();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFinitePileDlg::OnOK() 
{
	// TODO: Add extra validation here
	SaveData();

	CDialog::OnOK();
}
