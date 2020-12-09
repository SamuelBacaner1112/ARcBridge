// InputComment.cpp : implementation file
//

#include "stdafx.h"
#include "InputComment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CLR_BACK	RGB(255, 255, 225)

/////////////////////////////////////////////////////////////////////////////
// CInputComment

CInputComment::CInputComment()
{
	m_strDesc = _T("");
	m_bWarning	= FALSE;	// °æ°í ¸Þ½ÃÁö ¶ç¿ï¶§ »ç¿ë(»¡°²°í ±½°í ÀÌÅÅ¸¯À¸·Î Ç¥Çö)

	LOGFONT lf;
	VERIFY(::SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(lf), &lf, 0));
	lf.lfWeight = FW_NORMAL;
	lf.lfItalic = FALSE;
	VERIFY(m_fntNormal.CreateFontIndirect(&lf));

	lf.lfWeight = FW_BOLD;
	lf.lfItalic = FALSE;
	VERIFY(m_fntBold.CreateFontIndirect(&lf));
}

CInputComment::~CInputComment()
{
}


BEGIN_MESSAGE_MAP(CInputComment, CWnd)
	//{{AFX_MSG_MAP(CInputComment)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInputComment message handlers

void CInputComment::OnPaint() 
{
	CPaintDC dcPaint(this); // device context for painting
	CXTPClientRect rc(this);
	CXTPBufferDC dc(dcPaint, rc);

	dc.FillSolidRect(rc, CLR_BACK);
	dc.Draw3dRect(rc, GetXtremeColor(XPCOLOR_FRAME), GetXtremeColor(XPCOLOR_FRAME));	

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP_COMMENT);
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	cdc.SelectObject(&bitmap);
	dc.BitBlt(6, 4, 16, 16, &cdc, 0, 0, SRCCOPY);

	dc.SetTextColor(m_bWarning ? RGB(255, 0, 0) : GetSysColor(COLOR_BTNTEXT));
	dc.SetBkColor(CLR_BACK);
	CString strDesc = m_strDesc;
	if(m_bWarning)
		strDesc.Format("Warning! : %s", m_strDesc);
	if (!strDesc.IsEmpty())
	{
		CXTPFontDC font(&dc, m_bWarning ? &m_fntBold : &m_fntNormal);
		CRect rcDesc(rc);
		rcDesc.DeflateRect(26, 4);
		dc.DrawText(strDesc, rcDesc, DT_WORDBREAK|DT_EXTERNALLEADING);
	}
	// Do not call CWnd::OnPaint() for painting messages
}

void CInputComment::SetDescString(CString str, BOOL bWarring)
{
	m_strDesc = str;
	m_bWarning	= bWarring;
}
