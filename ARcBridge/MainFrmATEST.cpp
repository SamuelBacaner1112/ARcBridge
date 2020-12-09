#include "stdafx.h"
#include "ARcBridge.h"
#include "MainFrm.h"
#include "ARcBridgeDoc.h"
#include "ARcBridgeView.h"
#include "../Src/ARcBridgeOutDesign/OutDesignDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// INSERT_ATESTCODE
DECLARE_ATT_VARIABLES()
DECLARE_ATT_CLS_SPEEDABOVE()


LRESULT CMainFrame::OnAttMainFrm(WPARAM wp, LPARAM lp)
{
	switch(wp)
	{
	DECLARE_ATT_CASE_BASE()

	case 54:// eMsgAppSendsddata:
		{
		BOOL bActive=TRUE;
		::SendMessage(m_hWndATest,WM_ATT_MAINMSG,24,bActive);
		HGLONG32 nCount = attGetCountStru();
		::SendMessage(m_hWndATest,WM_ATT_MAINMSG,25,nCount);
		::SendMessage(m_hWndATest,WM_ATT_MAINMSG,26,0);
		}
		return 0;
	case 55:// eMsgAppSetThoughtATestVariable
		{
			CStaticATest::m_bThoughtATest= (HGLONG32)lp;
		}
		return 0;
		


	DECLARE_ATT_CASE_OPEN_DOCUMENT()
	}


	DECLARE_ATT_STRCMP_BASE()
	DECLARE_ATT_STRCMP_BASE_MULTIBYTE() 
	DECLARE_ATT_STRCMP_KEY_ATT_OPEN()

	return 0;
}


