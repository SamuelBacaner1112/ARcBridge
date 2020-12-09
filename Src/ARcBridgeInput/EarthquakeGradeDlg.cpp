// EarthquakeGradeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "EarthquakeGradeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEarthquakeGradeDlg dialog


CEarthquakeGradeDlg::CEarthquakeGradeDlg(CWnd* pParent /*=NULL*/)
	: CUserDialog(CEarthquakeGradeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEarthquakeGradeDlg)
		// NOTE: the ClassWizard will add member initialization here
	m_nType = -1;
	//}}AFX_DATA_INIT
}


void CEarthquakeGradeDlg::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEarthquakeGradeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Radio(pDX, IDC_RADIO1, m_nType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEarthquakeGradeDlg, CDialog)
	//{{AFX_MSG_MAP(CEarthquakeGradeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEarthquakeGradeDlg message handlers

BOOL CEarthquakeGradeDlg::OnInitDialog() 
{
	CUserDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_ReturnValue== "I 등급") 
		m_nType=0;
	else if(m_ReturnValue== "II등급") 
		m_nType=1;	

	UpdateData(FALSE);	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEarthquakeGradeDlg::OnOK() 
{
	UpdateData();
	if(m_nType==0)
		m_ReturnValue = "I 등급";
	else if(m_nType==1)
		m_ReturnValue = "II등급";
	
	CUserDialog::OnOK();
}
