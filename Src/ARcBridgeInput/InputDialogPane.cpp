// InputDialogPane.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "InputDialogPane.h"
#include "ARcBridgeInputStd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputDialogPane

CInputDialogPane::CInputDialogPane()
{
}

CInputDialogPane::~CInputDialogPane()
{
}


BEGIN_MESSAGE_MAP(CInputDialogPane, CWnd)
	//{{AFX_MSG_MAP(CInputDialogPane)
	ON_WM_CREATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInputDialogPane message handlers

int CInputDialogPane::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	UINT nIDD = m_pInputDialog->GetIDD();
	if(m_pInputDialog->GetSafeHwnd()==0)
		m_pInputDialog->Create(nIDD, this);

	m_wndComment.Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, 0);

	return 0;
}

void CInputDialogPane::OnPaint() 
{
	CPaintDC dcPaint(this); // device context for painting
	CXTPClientRect rc(this);
	CXTPBufferDC dc(dcPaint, rc);

	dc.FillSolidRect(rc, RGB(223, 223, 234));

	m_pInputDialog->ShowWindow(SW_SHOW);

	if(m_pInputDialog->m_pIn->m_bShowComment)
	{
		m_wndComment.ShowWindow(SW_SHOW);
		m_pInputDialog->MoveWindow(0, 0, rc.Width(), rc.Height()-26);
		m_wndComment.MoveWindow(0, rc.Height()-24, rc.Width(), 24);
	}
	else
	{
		m_wndComment.ShowWindow(SW_HIDE);
		m_pInputDialog->MoveWindow(0, 0, rc.Width(), rc.Height());
	}
	
	// Do not call CWnd::OnPaint() for painting messages
}
