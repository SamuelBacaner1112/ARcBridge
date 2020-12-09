// EValueInputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ARcBridgeInput.h"
#include "EValueInputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEValueInputDlg dialog


CEValueInputDlg::CEValueInputDlg(CWnd* pParent /*=NULL*/)
	: CUserDialog(CEValueInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEValueInputDlg)
	m_szEValue = _T("");
	//}}AFX_DATA_INIT
	m_nTypeUnit = 0;
	m_bLSD = FALSE;
}


void CEValueInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEValueInputDlg)
	DDX_Control(pDX, IDC_STATIC_UNIT, m_szUnit);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Text(pDX, IDC_EDIT_EO, m_szEValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEValueInputDlg, CDialog)
	//{{AFX_MSG_MAP(CEValueInputDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEValueInputDlg message handlers

BOOL CEValueInputDlg::OnInitDialog() 
{
	CUserDialog::OnInitDialog();

	if(m_bLSD)
	{
		SetWindowPos(&wndTop, 0, 0, 930, 500, SWP_NOMOVE | SWP_DRAWFRAME);
		GetDlgItem(IDOK)->SetWindowPos(&wndTop, 750, 440, 80, 20, SWP_DRAWFRAME);
		GetDlgItem(IDCANCEL)->SetWindowPos(&wndTop, 840, 440, 80, 20, SWP_DRAWFRAME);

		GetDlgItem(IDC_STATIC_ELASTIC_INPUT)->SetWindowPos(&wndTop, 600, 407, 120, 16, SWP_DRAWFRAME);
		GetDlgItem(IDC_EDIT_EO)->SetWindowPos(&wndTop, 725, 405, 100, 20, SWP_DRAWFRAME);
		GetDlgItem(IDC_STATIC_UNIT)->SetWindowPos(&wndTop, 830, 407, 60, 20, SWP_DRAWFRAME);
		GetDlgItem(IDC_STATIC)->SetWindowPos(&wndTop, 0, 390, 920, 45, SWP_DRAWFRAME);

		m_szUnit.SetWindowText(_T("(N/mm©÷)"));
		GetDlgItem(IDC_BITMAP_EO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BITMAP_EO_SI)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BITMAP_EO_LSD)->ShowWindow(SW_SHOW);
	}
	else
	{
		if(m_nTypeUnit==0)
		{
			m_szUnit.SetWindowText("(kgf/cm©÷)");
			GetDlgItem(IDC_BITMAP_EO_LSD)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BITMAP_EO_SI)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BITMAP_EO)->ShowWindow(SW_SHOW);
		}
		else
		{
			m_szUnit.SetWindowText("(kN/m©÷)");
			GetDlgItem(IDC_BITMAP_EO_LSD)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BITMAP_EO)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BITMAP_EO_SI)->ShowWindow(SW_SHOW);
		}
	}

	m_szEValue = m_ReturnValue;
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEValueInputDlg::OnOK() 
{	
	UpdateData();
	m_ReturnValue.Format("%g",atof(m_szEValue));
	CUserDialog::OnOK();
}
