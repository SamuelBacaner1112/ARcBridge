// DialogDom.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "DialogDom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogDom dialog


CDialogDom::CDialogDom(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogDom::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogDom)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogDom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogDom)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_DomControl(pDX, IDC_DOM, m_DomView);
}


BEGIN_MESSAGE_MAP(CDialogDom, CDialog)
	//{{AFX_MSG_MAP(CDialogDom)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogDom message handlers

BOOL CDialogDom::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_MOUSEWHEEL)
	{
		m_DomView.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDialogDom::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//DrawInputDomyunView(TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogDom::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_DomView.GetDomyun();
	pDom->SetScaleDim(100);
	pDom->ClearEtt(TRUE, FALSE);

	*pDom << m_Dom;
}
