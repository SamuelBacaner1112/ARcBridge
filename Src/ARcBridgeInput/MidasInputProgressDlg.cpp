// MidasInputProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "MidasInputProgressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMidasInputProgressDlg dialog


CMidasInputProgressDlg::CMidasInputProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMidasInputProgressDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMidasInputProgressDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMidasInputProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMidasInputProgressDlg)
	DDX_Control(pDX, IDC_PROGRESS_MIDAS, m_ctlProgress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMidasInputProgressDlg, CDialog)
	//{{AFX_MSG_MAP(CMidasInputProgressDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMidasInputProgressDlg message handlers

void CMidasInputProgressDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	//CDialog::OnCancel();
}
