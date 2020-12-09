// DlgProgress.cpp : implementation file
//

#include "stdafx.h"
#include "DlgProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress dialog


CDlgProgress::CDlgProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProgress)
	m_staticMessage = _T("");
	//}}AFX_DATA_INIT
}


void CDlgProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProgress)
	DDX_Control(pDX, IDC_HGPROGRESS, m_ctlProgress);
	DDX_Text(pDX, IDC_STATIC_HGMESSAGE, m_staticMessage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProgress, CDialog)
	//{{AFX_MSG_MAP(CDlgProgress)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress message handlers

void CDlgProgress::InitProgressData()
{
	m_nMin	= 0;
	m_nMax	= 100;
	m_nCur	= 0;
	m_sMessage	= _T("");

	m_ctlProgress.SetRange((short)m_nMin, (short)m_nMax);

	SetProgressPos("", 0);
}

void CDlgProgress::SetProgressPos(CString sMessage, long nPos)
{
	m_staticMessage	= sMessage;
	m_ctlProgress.SetPos(nPos);

	m_nCur	= nPos;
	m_sMessage	= sMessage;

	UpdateData(FALSE);
	
}

BOOL CDlgProgress::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
