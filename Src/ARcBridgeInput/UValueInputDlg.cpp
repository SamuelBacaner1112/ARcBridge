// UValueInputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ARcBridgeInput.h"
#include "UValueInputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUValueInputDlg dialog


CUValueInputDlg::CUValueInputDlg(CWnd* pParent /*=NULL*/)
	: CUserDialog(CUValueInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUValueInputDlg)
	m_UValue = _T("");
	m_bLSD = FALSE;
	//}}AFX_DATA_INIT
}


void CUValueInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUValueInputDlg)
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Text(pDX, IDC_EDIT_U, m_UValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUValueInputDlg, CDialog)
	//{{AFX_MSG_MAP(CUValueInputDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUValueInputDlg message handlers

BOOL CUValueInputDlg::OnInitDialog() 
{
	CUserDialog::OnInitDialog();

	if(m_bLSD)
	{
		SetWindowPos(&wndTop, 0, 0, 940, 500, SWP_NOMOVE | SWP_DRAWFRAME);
		GetDlgItem(IDOK)->SetWindowPos(&wndTop, 750, 440, 80, 20, SWP_DRAWFRAME);
		GetDlgItem(IDCANCEL)->SetWindowPos(&wndTop, 840, 440, 80, 20, SWP_DRAWFRAME);

		GetDlgItem(IDC_STATIC_POISSON_INPUT)->SetWindowPos(&wndTop, 690, 407, 80, 16, SWP_DRAWFRAME);
		GetDlgItem(IDC_EDIT_U)->SetWindowPos(&wndTop, 785, 405, 100, 20, SWP_DRAWFRAME);
		GetDlgItem(IDC_STATIC_POASONG_GROUP)->SetWindowPos(&wndTop, 0, 395, 930, 38, SWP_DRAWFRAME);


		GetDlgItem(IDC_STATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LSD)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_STATIC_LSD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC)->ShowWindow(SW_SHOW);
	}

	m_UValue = m_ReturnValue;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUValueInputDlg::OnOK() 
{
	UpdateData();
	m_ReturnValue.Format("%g",atof(m_UValue));
	CUserDialog::OnOK();
}
