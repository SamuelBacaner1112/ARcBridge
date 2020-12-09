// ShortcutPaneBM.cpp : implementation file
//

#include "stdafx.h"
#include "ARcBridge.h"
#include "ShortcutPaneBM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShortcutPaneBM

CShortcutPaneBM::CShortcutPaneBM()
{
}

CShortcutPaneBM::~CShortcutPaneBM()
{
	delete m_dlgMenuOutBM;
}


BEGIN_MESSAGE_MAP(CShortcutPaneBM, CXTPShortcutBarPane)
	//{{AFX_MSG_MAP(CShortcutPaneBM)
	ON_WM_SIZE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CShortcutPaneBM message handlers
BOOL CShortcutPaneBM::CreateStyle1(LPCTSTR lpszCaption, CXTPShortcutBar* pParent)
{
	CXTPShortcutBarPane::Create(lpszCaption, pParent);
	
	UINT nIDD = IDD_DIALOG_MENU_OUTBM;
	if(m_dlgMenuOutBM.GetSafeHwnd() == 0)
	{
		m_dlgMenuOutBM.Create(nIDD, this);
	}

	return TRUE;
}

void CShortcutPaneBM::InitTree()
{
	m_wndTreeBM.DeleteAllItems();
	
	HTREEITEM hItemParent;
	hItemParent	= m_wndTreeBM.InsertItem("¼ö·®", 0, 0);
}

void CShortcutPaneBM::OnSize(UINT nType, int cx, int cy) 
{
	CXTPShortcutBarPane::OnSize(nType, cx, cy);
	
	// TODO: Add your message 
	CXTPClientRect rc(this);
	if(m_dlgMenuOutBM.GetSafeHwnd() != 0)
	{
		m_dlgMenuOutBM.MoveWindow(rc);
	}
}

void CShortcutPaneBM::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	m_dlgMenuOutBM.ShowWindow(SW_SHOW);	
}
