// CG: This file was added by the Splash Screen component.
// Splash.cpp : implementation file
//

#include "stdafx.h"  // e. g. stdafx.h
#include "resource.h"  // e.g. resource.h

#include "Splash.h"  // e.g. splash.h

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//   Splash Screen class

BOOL CSplashWnd::c_bShowSplashWnd;
CSplashWnd* CSplashWnd::c_pSplashWnd;
BOOL CSplashWnd::c_bLSD;
CSplashWnd::CSplashWnd()
{
}

CSplashWnd::~CSplashWnd()
{
	// Clear the static window pointer.
	ASSERT(c_pSplashWnd == this);
	c_pSplashWnd = NULL;
}

BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	//{{AFX_MSG_MAP(CSplashWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSplashWnd::EnableSplashScreen(BOOL bEnable /*= TRUE*/)
{
	c_bShowSplashWnd = bEnable;
}

void CSplashWnd::ShowSplashScreen(CWnd* pParentWnd /*= NULL*/)
{
	if (!c_bShowSplashWnd || c_pSplashWnd != NULL)
		return;

	// Allocate a new splash screen, and create the window.
	c_pSplashWnd = new CSplashWnd;
	if (!c_pSplashWnd->Create(pParentWnd))
		delete c_pSplashWnd;
	else
		c_pSplashWnd->UpdateWindow();
}

BOOL CSplashWnd::PreTranslateAppMessage(MSG* pMsg)
{
	if (c_pSplashWnd == NULL)
		return FALSE;

	// If we get a keyboard or mouse message, hide the splash screen.
	if (pMsg->message == WM_KEYDOWN ||
	    pMsg->message == WM_SYSKEYDOWN ||
	    pMsg->message == WM_LBUTTONDOWN ||
	    pMsg->message == WM_RBUTTONDOWN ||
	    pMsg->message == WM_MBUTTONDOWN ||
	    pMsg->message == WM_NCLBUTTONDOWN ||
	    pMsg->message == WM_NCRBUTTONDOWN ||
	    pMsg->message == WM_NCMBUTTONDOWN)
	{
		c_pSplashWnd->HideSplashScreen();
		return TRUE;	// message handled here
	}

	return FALSE;	// message not handled
}

BOOL CSplashWnd::Create(CWnd* pParentWnd /*= NULL*/)
{
	if (!m_bitmap.LoadBitmap(c_bLSD ? IDB_SPLASH_LSD : IDB_SPLASH))
		return FALSE;

	BITMAP bm;
	m_bitmap.GetBitmap(&bm);

	CreateEx(0,
		AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
		NULL, WS_POPUP | WS_VISIBLE, 0, 0, bm.bmWidth, bm.bmHeight, pParentWnd->GetSafeHwnd(), NULL);

	int iThick = bm.bmHeight-35;
	int iB = bm.bmHeight-25;

	m_ProcessBar.Create(WS_CHILD | WS_VISIBLE | PBS_SMOOTH, CRect(85, iThick, 175, iB), this, 0);
	return TRUE;
}

void CSplashWnd::HideSplashScreen()
{
	// Destroy the window, and update the mainframe.
	DestroyWindow();

	if(AfxGetMainWnd() && IsWindow(AfxGetMainWnd()->m_hWnd))
	{
		AfxGetMainWnd()->UpdateWindow();
	}
}

void CSplashWnd::PostNcDestroy()
{
	// Free the C++ class.
	delete this;
}

int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Center the window.
	CenterWindow();

	// Set a timer to destroy the splash screen.
	SetTimer(1, 3000, NULL);

	return 0;
}

void CSplashWnd::OnPaint()
{
	CPaintDC dc(this);

	CDC dcImage;
	if (!dcImage.CreateCompatibleDC(&dc))
		return;

	BITMAP bm;
	m_bitmap.GetBitmap(&bm);

	// Paint the image.
	CBitmap* pOldBitmap = dcImage.SelectObject(&m_bitmap);
	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY);
	dcImage.SelectObject(pOldBitmap);
	dc.SetBkMode(TRANSPARENT);

	CFont font;
	font.CreatePointFont(85,_T("Arial"));
	dc.SelectObject(&font);
	if(m_ProcessBar.GetSafeHwnd() && m_ProcessBar.IsWindowVisible())
	{
		dc.SetTextColor(RGB(255,255,255));
		dc.TextOut(35,bm.bmHeight-38,_T("Reading..."));
		dc.SetTextColor(RGB(0,0,0));
		dc.TextOut(34,bm.bmHeight-38+1,_T("Reading..."));
	}
	else
	{
		dc.SetTextColor(RGB(255,255,255));
		dc.TextOut(35,bm.bmHeight-38,_T("Program Initializing..."));
		dc.SetTextColor(RGB(0,0,0));
		dc.TextOut(34,bm.bmHeight-38+1,_T("Program Initializing..."));
	}
}

void CSplashWnd::OnTimer(UINT nIDEvent)
{
	// Destroy the splash screen window.
	HideSplashScreen();
}

void CSplashWnd::SetLSD( BOOL bLSD )
{
	c_bLSD = bLSD;
}
