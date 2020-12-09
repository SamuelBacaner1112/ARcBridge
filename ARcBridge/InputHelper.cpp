// InputHelper.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridge.h"
#include "InputHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputHelper

CInputHelper::CInputHelper()
{
	m_clrHelpBack = RGB(238, 238, 244);
	m_strDesc = _T("");

	LOGFONT lf;
	VERIFY(::SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(lf), &lf, 0));
	lf.lfWeight = FW_NORMAL;
	lf.lfItalic = FALSE;
	VERIFY(m_fntNormal.CreateFontIndirect(&lf));

	lf.lfWeight = FW_BOLD;
	VERIFY(m_fntBold.CreateFontIndirect(&lf));
}

CInputHelper::~CInputHelper()
{
}


BEGIN_MESSAGE_MAP(CInputHelper, CWnd)
	//{{AFX_MSG_MAP(CInputHelper)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInputHelper message handlers

void CInputHelper::OnPaint() 
{
	CPaintDC dcPaint(this); // device context for painting
	CXTPClientRect rc(this);
	CXTPBufferDC dc(dcPaint, rc);

	dc.FillSolidRect(rc, m_clrHelpBack);
//	dc.MoveTo(rc.left, rc.top);
//	dc.LineTo(rc.right, rc.top);

	CRect rcHelp(rc);

	CXTPFontDC font(&dc, &m_fntBold);

	dc.SetTextColor(GetSysColor(COLOR_BTNTEXT));
	dc.SetBkColor(m_clrHelpBack);
	CRect rcCaption(rcHelp);
	rcCaption.DeflateRect(6, 9);
	dc.DrawText(_T("도움말"), rcCaption, DT_SINGLELINE);
	
	CString strDesc = m_strDesc;
	if (!strDesc.IsEmpty())
	{
		CXTPFontDC font(&dc, &m_fntNormal);
		CXTPEmptyRect rcCaption;
		dc.DrawText(_T("도움말"), rcCaption, DT_SINGLELINE|DT_CALCRECT);
		CRect rcDesc(rcHelp);
		rcDesc.DeflateRect(6, 9 + rcCaption.Height() + 6, 6, 9);
		dc.DrawText(strDesc, rcDesc, DT_WORDBREAK|DT_EXTERNALLEADING);
	}
	// Do not call CWnd::OnPaint() for painting messages
}

void CInputHelper::SetDescString(CString str)
{
	m_strDesc = str;
}
