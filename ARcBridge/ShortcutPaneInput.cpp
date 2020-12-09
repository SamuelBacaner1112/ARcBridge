// ShortcutPaneInput.cpp : implementation file
//

#include "stdafx.h"
#include "ARcBridge.h"
#include "ShortcutPaneInput.h"

#include "MainFrm.h"
#include "ARcBridgeDoc.h"
#include "ARcBridgeView.h"
#include "../Src/ARcBridgeInput/ARcBridgeInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShortcutPaneInput

CShortcutPaneInput::CShortcutPaneInput()
{
	hItemPrev = NULL;
}

CShortcutPaneInput::~CShortcutPaneInput()
{
}


BEGIN_MESSAGE_MAP(CShortcutPaneInput, CXTPShortcutBarPane)
	//{{AFX_MSG_MAP(CShortcutPaneInput)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_INPUT, OnSelchangedTreeInput)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CShortcutPaneInput message handlers

BOOL CShortcutPaneInput::Create(LPCTSTR lpszCaption, CXTPShortcutBar *pParent)
{
	CXTPShortcutBarPane::Create(lpszCaption, pParent);

	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)pFrm->GetActiveDocument();
	CARcBridgeInputStd* pIn = pDoc->m_pARcBridgeInputStd;

	m_wndTreeInput.Create(WS_VISIBLE|TVS_HASBUTTONS|TVS_LINESATROOT|TVS_TRACKSELECT|TVS_SHOWSELALWAYS, CXTPEmptyRect(), this, IDC_TREE_INPUT);

	CBitmap bmpHead, bitmap1, bitmap2, bitmap3, bitmap4;
	bmpHead.LoadBitmap(IDB_BITMAP_INPUT_HEAD);
	bitmap1.LoadBitmap(IDR_TOOLBAR_BASE);
	bitmap2.LoadBitmap(IDR_TOOLBAR_GENERAL);
	bitmap3.LoadBitmap(IDR_TOOLBAR_REBAR);
	bitmap4.LoadBitmap(IDR_TOOLBAR_STM);

	m_ilTreeIcons.Create(16, 16, ILC_COLOR32|ILC_MASK, 2, 1);
	m_ilTreeIcons.Add(&bmpHead, RGB(192, 192, 192));
	m_ilTreeIcons.Add(&bitmap1, RGB(192, 192, 192));
	m_ilTreeIcons.Add(&bitmap2, RGB(192, 192, 192));
	m_ilTreeIcons.Add(&bitmap3, RGB(192, 192, 192));
	m_ilTreeIcons.Add(&bitmap4, RGB(192, 192, 192));
	m_wndTreeInput.SetImageList(&m_ilTreeIcons, TVSIL_NORMAL);

	COLORREF clrBack = RGB(253, 250, 255);
	m_wndTreeInput.SetBkColor(clrBack);

	CString strMenu, strSubMenu;

	int nImage = 3;
	HTREEITEM hItemFirst, hItem;
	for(UINT i=0; i<MENU_COUNT; i++)
	{
		// i==0 : 일반 설정
		// i==1 : 형상 입력
		// i==2 : 철근 배치
		// i==3 : Stm 해석
		pIn->m_menuInput.GetMenuString(i, strMenu, MF_BYPOSITION);
		strMenu = strMenu.Mid(0,strMenu.Find("("));
		hItem = m_wndTreeInput.InsertItem(strMenu, i, i);
		if(i==0)
			hItemFirst = hItem;
		CMenu* pMenu = pIn->m_menuInput.GetSubMenu(i);
		for(UINT j=0; j < (UINT)pMenu->GetMenuItemCount(); j++)
		{
			pMenu->GetMenuString(j, strSubMenu, MF_BYPOSITION);
			m_wndTreeInput.InsertItem(strSubMenu, nImage, nImage, hItem);
			m_cmdStrArr.Add(strMenu + " - " + strSubMenu);
			m_cmdIDArr.Add(pMenu->GetMenuItemID(j));
			nImage++;
		}
		m_wndTreeInput.SetItemBold(hItem);
		m_wndTreeInput.Expand(hItem, TVE_EXPAND);
	}
	m_wndTreeInput.SetItemHeight(18);
	m_wndTreeInput.SelectSetFirstVisible(hItemFirst);

	//AddItem(_T("입력 선택"), &m_wndTreeInput, 0);
	InitTreeState();

	return TRUE;
}

void CShortcutPaneInput::InitTreeState()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)pFrm->GetActiveDocument();
	if(pDoc == NULL)
		return;
	CARcBridgeInputStd* pIn = pDoc->m_pARcBridgeInputStd;

	HTREEITEM hParentItem = m_wndTreeInput.GetRootItem();
	HTREEITEM hChildItem = m_wndTreeInput.GetChildItem(hParentItem);
	UINT nItemSize = m_cmdIDArr.GetSize();
	BOOL bEnable = FALSE;

	for(UINT i=0; i<nItemSize; i++)
	{
		if(m_wndTreeInput.GetChildItem(hChildItem)==NULL)
		{
/*			if(i==0)
			{
				m_wndTreeInput.EnableItem(hChildItem, TRUE);
			}
			else*/
			{
				bEnable = pIn->IsEnableDialog(m_cmdIDArr.GetAt(i));
				m_wndTreeInput.EnableItem(hChildItem, bEnable);
			}
		}

		hChildItem = m_wndTreeInput.GetNextItem(hChildItem, TVGN_NEXT);
		if(hChildItem==NULL)
		{
			hParentItem = m_wndTreeInput.GetNextItem(hParentItem, TVGN_NEXT);
			if(hParentItem!=NULL)
			{
				hChildItem = m_wndTreeInput.GetChildItem(hParentItem);
			}
		}
	}
}

void CShortcutPaneInput::OnSelchangedTreeInput(NMHDR* pNMHDR, LRESULT* pResult)
{	
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)pFrm->GetActiveDocument();
	CARcBridgeInputStd* pIn = pDoc->m_pARcBridgeInputStd;
	if(pFrm->m_bPrintPreview) return;


	HTREEITEM hItem = m_wndTreeInput.GetSelectedItem();
	HTREEITEM hParentItem = m_wndTreeInput.GetParentItem(hItem);
	BOOL bEnable = FALSE;

	if(m_wndTreeInput.GetChildItem(hItem)==NULL)
	{
		CString strItem = m_wndTreeInput.GetItemText(hItem);
		CString strParentItem = m_wndTreeInput.GetItemText(hParentItem);

		int nTreeSize = m_cmdStrArr.GetSize();
		int i=0; for(i=0; i<nTreeSize; i++)
		{
			CString s	= m_cmdStrArr.GetAt(i);
			DWORD id	= m_cmdIDArr.GetAt(i);
			if(m_cmdStrArr.GetAt(i)==strParentItem + " - " + strItem)
			{
				if(i==0)
					m_wndTreeInput.EnableItem(hItem, TRUE);
				else
				{
					bEnable = pIn->IsEnableDialog(m_cmdIDArr.GetAt(i));
					m_wndTreeInput.EnableItem(hItem, bEnable);
					if(bEnable == FALSE) break;
				}

				if(pIn->GetCurrentDialog())
				{
					if(!pIn->m_bPushMoveButton)	// 버튼눌러오 올때는 이미 옮겨진 입력창이므로 체크안함
					{
						CInputDialogBase *pDlgFind	= pIn->GetDialogByMenuID(m_cmdIDArr.GetAt(i));
						BOOL bCheckMoveOtherDlg	= pDlgFind ? pDlgFind->IsCheckMoveOtherDlg() : TRUE;
						if(!pIn->GetCurrentDialog()->IsOKMoveOtherDlg() && bCheckMoveOtherDlg) return;

						pIn->GetCurrentDialog()->KillFocusDlg();
					}
				}

				pFrm->SendMessage(WM_USER+2, m_cmdIDArr.GetAt(i));
				break;
			}
		}
	}


	if(hItemPrev)
	{
		m_wndTreeInput.SetItemBold(hItemPrev, FALSE);
	}
	if(hItem)
	{
		m_wndTreeInput.SetItemBold(hItem, TRUE);
		hItemPrev = hItem;
	}

	*pResult = 0;
}

void CShortcutPaneInput::SetItemPosition(UINT nMenuID)
{
	HTREEITEM hParentItem = m_wndTreeInput.GetRootItem();
	HTREEITEM hItem = m_wndTreeInput.GetChildItem(hParentItem);

	UINT nItemSize = m_cmdIDArr.GetSize();
	for(UINT i=0; i<nItemSize; i++)
	{
		if(m_wndTreeInput.GetChildItem(hItem)==NULL)
		{
			if(m_cmdIDArr.GetAt(i)==nMenuID)
			{
				m_wndTreeInput.EnableItem(hItem, TRUE);
				m_wndTreeInput.SelectItem(hItem);
			}
		}

		hItem = m_wndTreeInput.GetNextItem(hItem, TVGN_NEXT);
		if(hItem==NULL)
		{
			hParentItem = m_wndTreeInput.GetNextItem(hParentItem, TVGN_NEXT);
			if(hParentItem!=NULL)
			{
				hItem = m_wndTreeInput.GetChildItem(hParentItem);
			}
		}
	}
}

void CShortcutPaneInput::SetItemPositionUpdate(UINT nMenuID, BOOL bSelect)
{
	HTREEITEM hItem = m_wndTreeInput.GetRootItem();
	HTREEITEM hParentItem = hItem;
	hItem = m_wndTreeInput.GetChildItem(hItem);

	CRect rect;
	long nCount = 0;;
	while(hItem)
	{
		if(m_wndTreeInput.GetChildItem(hItem)==NULL)
		{
			if(m_cmdIDArr.GetAt(nCount)==nMenuID-1)
			{
				if(bSelect) 
					m_wndTreeInput.Select(hItem, TVGN_CARET);
				else
				{
					m_wndTreeInput.GetItemRect(hItem, &rect, TRUE);
					m_wndTreeInput.InvalidateRect(rect);
				}
				break;
			}
			++nCount;
		}
		hItem = m_wndTreeInput.GetNextItem(hItem, TVGN_NEXT);
		if(hItem==NULL)
		{
			hParentItem = m_wndTreeInput.GetNextItem(hParentItem, TVGN_NEXT);
			if(hParentItem)
				hItem = m_wndTreeInput.GetChildItem(hParentItem);
		}
	}
}

void CShortcutPaneInput::OnSize(UINT nType, int cx, int cy) 
{
	CXTPShortcutBarPane::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CXTPClientRect rc(this);
	if(m_wndTreeInput.GetSafeHwnd() != 0)
	{
		m_wndTreeInput.MoveWindow(rc);
	}	
}
