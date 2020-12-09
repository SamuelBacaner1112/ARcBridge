// DlgProgress.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridge.h"
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
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_sMessage	= _T("");
	m_nCur		= 0;

	m_nMin		= 0;
	m_nMax		= 100;
}


void CDlgProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProgress)
	DDX_Control(pDX, IDC_PROGRESS, m_ctlProgress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProgress, CDialog)
	//{{AFX_MSG_MAP(CDlgProgress)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress message handlers


// 프로그레스 데이타 설정
void CDlgProgress::SetProgressData(CString sMessage, long nCurPos)
{
	if(nCurPos < m_nMin || nCurPos > m_nMax) return;
}

void CDlgProgress::InitProgressData()
{
	m_nMin	= 0;
	m_nMax	= 0;
	m_nCur	= 0;
	m_sMessage	= _T("");
	SetProgressData("", 0);
}
