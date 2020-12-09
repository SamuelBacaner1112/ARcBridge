// WmfSizeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ARcBridge.h"
#include "WmfSizeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWmfSizeDlg dialog


CWmfSizeDlg::CWmfSizeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWmfSizeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWmfSizeDlg)
	m_bAutoSize = FALSE;
	m_dHeight = 500.0;
	m_dWidth = 500.0;
	//}}AFX_DATA_INIT
}


void CWmfSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWmfSizeDlg)
	DDX_Check(pDX, IDC_CHECK_AUTOSIZE, m_bAutoSize);
	DDX_Text(pDX, IDC_EDIT_WMFHEIGHT, m_dHeight);
	DDX_Text(pDX, IDC_EDIT_WMFWIDTH, m_dWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWmfSizeDlg, CDialog)
	//{{AFX_MSG_MAP(CWmfSizeDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWmfSizeDlg message handlers
