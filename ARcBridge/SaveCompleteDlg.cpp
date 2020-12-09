// SaveCompleteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridge.h"
#include "SaveCompleteDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSaveCompleteDlg dialog


CSaveCompleteDlg::CSaveCompleteDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveCompleteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSaveCompleteDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSaveCompleteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveCompleteDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaveCompleteDlg, CDialog)
	//{{AFX_MSG_MAP(CSaveCompleteDlg)
	ON_WM_CREATE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaveCompleteDlg message handlers

int CSaveCompleteDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CenterWindow();
	SetTimer(1, 750, NULL);
	
	return 0;
}

void CSaveCompleteDlg::OnTimer(UINT nIDEvent) 
{
	DestroyWindow();
	
	CDialog::OnTimer(nIDEvent);
}
