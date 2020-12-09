// WaitForSubProgram.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridge.h"
#include "WaitForSubProgram.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaitForSubProgram dialog


CWaitForSubProgram::CWaitForSubProgram(CWnd* pParent /*=NULL*/)
	: CDialog(CWaitForSubProgram::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWaitForSubProgram)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWaitForSubProgram::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaitForSubProgram)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWaitForSubProgram, CDialog)
	//{{AFX_MSG_MAP(CWaitForSubProgram)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaitForSubProgram message handlers
