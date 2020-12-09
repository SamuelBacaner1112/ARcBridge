// ShortcutPaneStart.cpp : implementation file
//

#include "stdafx.h"
#include "ARcBridge.h"
#include "MainFrm.h"
#include "ShortcutPaneStart.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShortcutPaneStart

CShortcutPaneStart::CShortcutPaneStart()
{
}

CShortcutPaneStart::~CShortcutPaneStart()
{
}


BEGIN_MESSAGE_MAP(CShortcutPaneStart, CXTPShortcutBarPane)
	//{{AFX_MSG_MAP(CShortcutPaneStart)
	ON_MESSAGE(XTPWM_TASKPANEL_NOTIFY, OnTaskPanelNotify)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CShortcutPaneStart message handlers

BOOL CShortcutPaneStart::Create(LPCTSTR lpszCaption, CXTPShortcutBar *pParent)
{
	CARcBridgeApp* pApp = (CARcBridgeApp*)AfxGetApp();

	CXTPShortcutBarPane::Create(lpszCaption, pParent);
	
	m_wndTaskPanel.Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, CXTPEmptyRect(), this, 0);

	CBitmap bitmap1, bitmap2;
	bitmap1.LoadBitmap(IDR_MAINFRAME);
	bitmap2.LoadBitmap(IDB_BITMAP_SUPPORT);

	m_ilTaskIcons.Create(16, 16, ILC_COLOR32|ILC_MASK, 2, 1);
	m_ilTaskIcons.Add(&bitmap1, RGB(192, 192, 192));
	m_ilTaskIcons.Add(&bitmap2, RGB(192, 192, 192));
	m_wndTaskPanel.SetImageList(&m_ilTaskIcons);

	// 설계 문서 열기
	CXTPTaskPanelGroup* pGroupFileOpen = m_wndTaskPanel.AddGroup(ID_TASKGROUP_FILEOPEN);
	InitMRUList();

	// 새로 만들기
	CXTPTaskPanelGroup* pGroupFileNew = m_wndTaskPanel.AddGroup(ID_TASKGROUP_FILENEW);
	CXTPTaskPanelGroupItem* pItemFileNew = NULL;
	pItemFileNew = pGroupFileNew->AddLinkItem(ID_TASKITEM_FILENEW, 0);

	// 고객 지원
	CXTPTaskPanelGroup* pGroupSupport = m_wndTaskPanel.AddGroup(ID_TASKGROUP_SUPPORT);
	CXTPTaskPanelGroupItem* pItemSupport = NULL;
	pItemSupport = pGroupSupport->AddLinkItem(ID_TASKITEM_HOME, 8);
	pItemSupport->SetSize(CSize(200, 20));
	pItemSupport = pGroupSupport->AddLinkItem(ID_TASKITEM_QNA, 9);
	pItemSupport->SetSize(CSize(200, 20));
	pItemSupport = pGroupSupport->AddLinkItem(ID_TASKITEM_UPDATE, 10);

	// 버전 정보
/*	CXTPTaskPanelGroup* pGroupVersionInfo = m_wndTaskPanel.AddGroup(ID_TASKGROUP_VERSION_INFO);
	CXTPTaskPanelGroupItem* pItemVersionInfo = NULL;
	pItemVersionInfo = pGroupVersionInfo->AddTextItem(pApp->GetARcBridgeName() + _T(" Build Date"));
	pItemVersionInfo->SetSize(CSize(200, 20));
	CString szBuildDate = AfxGetApp()->GetProfileString("Settings", "BuildVersion","");
	if(szBuildDate.IsEmpty())
	{
		pItemVersionInfo = pGroupVersionInfo->AddTextItem(_T("업데이트 날짜 : 정보없음"));
	}
	else
	{
		CString szYear = szBuildDate.Left(4);
		CString szMoon = szBuildDate.Mid(4,2);
		CString szDay = szBuildDate.Mid(6,2);
		CString szDate;
		szDate.Format("%d년 %d월 %d일", atoi(szYear), atoi(szMoon), atoi(szDay));
		pItemVersionInfo = pGroupVersionInfo->AddTextItem(_T("업데이트 날짜 : ") + szDate);
	}
*/
	//AddItem(_T(""), &m_wndTaskPanel, 0);

	m_wndTaskPanel.SetMargins(8, 8, 0, 8, 15);
	m_wndTaskPanel.SetTheme(xtpTaskPanelThemeVisualStudio2012Light);

	m_pShortcutBar->GetPaintManager()->m_nPaneItemHeight = 0;

	return TRUE;
}

LRESULT CShortcutPaneStart::OnTaskPanelNotify(WPARAM wParam, LPARAM lParam)
{
	if(!LDK::aCheckLicenseHGKey()) 
		return 0;

	CARcBridgeApp* pApp = (CARcBridgeApp*)AfxGetApp();
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();

	if(pFrm->m_bPrintPreview) return 0;

	UINT nSize = pApp->GetMRUListSize();
	switch(wParam)
	{
	case XTP_TPN_CLICK:
		{
			CXTPTaskPanelGroupItem* pItem = (CXTPTaskPanelGroupItem*)lParam;
			UINT nID = pItem->GetID();
			if(nID<nSize)
			{
				pApp->OpenDocumentFile(pItem->GetTooltip());
				pApp->DeleteWMF();
			}
			if(nID==ID_TASKITEM_FILEETC)
				pApp->OnFileOpen();
			if(nID==ID_TASKITEM_FILENEW)
				pFrm->SendMessage(WM_COMMAND, ID_FILE_NEW);
			if(nID==ID_TASKITEM_HOME)
				pFrm->SendMessage(WM_COMMAND, ID_APP_HOME);
			if(nID==ID_TASKITEM_QNA)
				pFrm->SendMessage(WM_COMMAND, ID_APP_QNA);
			if(nID==ID_TASKITEM_UPDATE)
				pFrm->SendMessage(WM_COMMAND, ID_APP_UPDATE);
		}
		break;
	}

	return 0;
}

void CShortcutPaneStart::InitMRUList()
{
	CARcBridgeApp* pApp = (CARcBridgeApp*)AfxGetApp();

	CXTPTaskPanelGroup* pGroupFileOpen = m_wndTaskPanel.FindGroup(ID_TASKGROUP_FILEOPEN);
	CXTPTaskPanelGroupItems* pItemsFileOpen = (CXTPTaskPanelGroupItems*)pGroupFileOpen->GetItems();
	pItemsFileOpen->Clear();

	CString strName = _T("");
	CString strPath = _T("");
	UINT nSize = pApp->GetMRUListSize();
	CXTPTaskPanelGroupItem* pItemFileOpen = NULL;
	for(UINT i=0; i<nSize; i++)
	{
		strName = pApp->GetMRUName(i, FALSE);
		strPath = pApp->GetMRUName(i, TRUE);
		pItemFileOpen = pGroupFileOpen->AddTextItem(strName);
//		pItemFileOpen->SetID(i);
		pItemFileOpen->SetType(xtpTaskItemTypeLink);
		pItemFileOpen->SetTooltip(strPath);
		pItemFileOpen->SetSize(CSize(200, 20));
	}
	pItemFileOpen = pGroupFileOpen->AddLinkItem(ID_TASKITEM_FILEETC, 1);
}


void CShortcutPaneStart::OnSize(UINT nType, int cx, int cy) 
{
	CXTPShortcutBarPane::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CXTPClientRect rc(this);
	if(m_wndTaskPanel.GetSafeHwnd() != 0)
	{
		m_wndTaskPanel.MoveWindow(rc);
	}	
}