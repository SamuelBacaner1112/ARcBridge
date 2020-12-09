// ShortcutPaneDrawing.cpp : implementation file
//

#include "stdafx.h"
#include "ARcBridge.h"
#include "ShortcutPaneDrawing.h"

#include "MainFrm.h"
#include "ARcBridgeDoc.h"
#include "ARcBridgeView.h"
#include "../Src/ARcBridgeDrawing/ARcBridgeDrawing.h"
#include "../Src/ARcBridgeDrawingOption/ARcBridgeDrawingOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShortcutPaneDrawing

CShortcutPaneDrawing::CShortcutPaneDrawing()
{
}

CShortcutPaneDrawing::~CShortcutPaneDrawing()
{
}


BEGIN_MESSAGE_MAP(CShortcutPaneDrawing, CXTPShortcutBarPane)
	//{{AFX_MSG_MAP(CShortcutPaneDrawing)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_DRAWING, OnSelchangedTreeDrawing)
	ON_WM_NCRBUTTONUP()
	ON_WM_SIZE()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CShortcutPaneDrawing message handlers

BOOL CShortcutPaneDrawing::Create(LPCTSTR lpszCaption, CXTPShortcutBar* pParent)
{
	CXTPShortcutBarPane::Create(lpszCaption, pParent);

	m_wndTreeDrawing.Create(WS_VISIBLE|TVS_HASBUTTONS|TVS_LINESATROOT|TVS_TRACKSELECT|TVS_SHOWSELALWAYS, CXTPEmptyRect(), this, IDC_TREE_DRAWING);

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP_DRAWING);

	m_ilTreeIcons.Create(16, 16, ILC_COLOR32|ILC_MASK, 2, 1);
	m_ilTreeIcons.Add(&bitmap, RGB(192, 192, 192));
	m_wndTreeDrawing.SetImageList(&m_ilTreeIcons, TVSIL_NORMAL);

	COLORREF clrBack = RGB(253, 250, 255);
	m_wndTreeDrawing.SetBkColor(clrBack);

	InitTree();

	return TRUE;
}

void CShortcutPaneDrawing::InitTree()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)pFrm->GetActiveDocument();
	if(pDoc == NULL)
		return;
	CARcBridgeDataStd* pStd = pDoc->m_pARcBridgeDataStd;
	CARcBridgeDrawingStd* pDraw = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd *pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	CString strTitleBri, strTitle, strTitleSub;
	HTREEITEM hItemFirst=NULL, hItemBri=NULL, hItem=NULL;

	pDraw->MakePageInfo();
	pOpt->MakeDrawingOption();

	m_wndTreeDrawing.DeleteAllItems();

	int nCurPage = pDraw->m_nCurPage;

	int nPage = 1;
	int nCountBri = pStd->GetBridgeSize();
	int nCountCate = pDraw->GetCountCategory();
	int i=0; for(i=0; i<nCountBri; i++)
	{
		int nPageCount = 1;
		CRcBridgeApp* pBri = pStd->GetBridge(i);

		// modified by jaeho 2004.04.09
		/*
		strTitleBri = pBri->m_strBridgeName;
		*/
		strTitleBri = pStd->GetStringBridgeOrder(i) + _T(" : ") + pBri->m_strBridgeName
			+ _T(" [") + pBri->GetStringBridgeType() + _T("]");


		hItemBri = m_wndTreeDrawing.InsertItem(strTitleBri, 0, 0);
		m_wndTreeDrawing.SetItemData(hItemBri, -1);
		if(i==0)
			hItemFirst = hItemBri;

		
		for(int j=0; j<nCountCate; j++)
		{
			strTitle = pDraw->GetTitleCategory(j);
			if(strTitle == "사용자 도면") continue;

			hItem = m_wndTreeDrawing.InsertItem(strTitle, 1, 1, hItemBri);
			m_wndTreeDrawing.SetItemData(hItem, -1);

			int nCountCategoryPage = pDraw->GetCountBriCategoryPage(i, strTitle);
			for(int k=1; k<nCountCategoryPage+1; k++)
			{
				CPageInfo* pInfo = pDraw->GetPageInfo(i, nPageCount);
				if(pInfo)
				{
					strTitleSub.Format(_T("%03d : %s"), nPage++, pInfo->m_strTitle);
					pInfo->m_hItem = m_wndTreeDrawing.InsertItem(strTitleSub, 2, 2, hItem);

					long nCurPage	= pDraw->GetPageNum(pInfo);
					m_wndTreeDrawing.SetItemData(pInfo->m_hItem, nCurPage);
					nPageCount++;
				}
			}
		}
		m_wndTreeDrawing.SetItemBold(hItemBri);
		m_wndTreeDrawing.Expand(hItemBri, TVE_EXPAND);
	}
	m_wndTreeDrawing.SetItemHeight(18);
	m_wndTreeDrawing.SelectSetFirstVisible(hItemFirst);

	//AddItem(_T("도면 선택"), &m_wndTreeDrawing, 0);

	CString str = _T("");
	int nSttPage = 0;
	for(i=0; i<nCountCate; i++)
	{
		nSttPage = pDraw->GetCategorySttPage(i);
		str = pDraw->GetTitleCategory(i);

		if(str==_T("종평면도"))
		{
			CDrawingVerticalPlane *pMng	= ((CDrawingVerticalPlane*)pDraw->GetDrawPageMng()->GetPage(i));
			pMng->m_nSttPage = nSttPage;
		}
		else if(str==_T("일반도"))
		{
			CDrawingGeneral *pMng	= ((CDrawingGeneral*)pDraw->GetDrawPageMng()->GetPage(i));
			pMng->m_nSttPage = nSttPage;
		}
		else if(str==_T("구조도"))
		{
			CDrawingRebar *pMng	= ((CDrawingRebar*)pDraw->GetDrawPageMng()->GetPage(i));
			pMng->m_nSttPage = nSttPage;
		}
		else if(str==_T("접속 슬래브"))
		{
			CDrawingAps *pMng	= ((CDrawingAps*)pDraw->GetDrawPageMng()->GetPage(i));
			pMng->m_nSttPage = nSttPage;
		}
		else if(str==_T("완충 슬래브"))
		{
			CDrawingCns *pMng	= ((CDrawingCns*)pDraw->GetDrawPageMng()->GetPage(i));
			pMng->m_nSttPage = nSttPage;
		}
		else if(str==_T("기타 도면"))
		{
			CDrawingOthers *pMng	= ((CDrawingOthers*)pDraw->GetDrawPageMng()->GetPage(i));
			pMng->m_nSttPage = nSttPage;
		}
		else if(str==_T("지질 주상도"))
		{
			CDrawingBoring *pMng	= ((CDrawingBoring*)pDraw->GetDrawPageMng()->GetPage(i));
			pMng->m_nSttPage		= nSttPage;
		}
	}

	pDraw->m_nCurPage = nCurPage;
	pDraw->SetModifiedAll(FALSE);
	pDraw->GetDrawPageMng()->SetPageModifyAll();
}

void CShortcutPaneDrawing::OnSelchangedTreeDrawing(NMHDR* pNMHDR, LRESULT* pResult)
{	
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CMainFrame *pFrm = (CMainFrame*)AfxGetMainWnd();
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)pFrm->GetActiveDocument();
	CARcBridgeDrawingStd* pDraw = pDoc->m_pARcBridgeDrawingStd;
	if(pFrm->m_bPrintPreview) return;

	HTREEITEM hItem = m_wndTreeDrawing.GetSelectedItem();
	if(m_wndTreeDrawing.GetChildItem(hItem)==NULL)
	{
		CPageInfo* pInfo = pDraw->GetPageInfo(hItem);
		if(pInfo)
		{
			CDrawPageMng* pMng = pDraw->GetPageManage();
			pMng->SetPageInit();
			CString str = pDraw->GetTitleCategory(pInfo->m_nCategory);
			long nPage1 = pDraw->GetPageNum(str, hItem);
			long nPage2 = pMng->GetPageByIdx(pInfo->m_nCategory);
			pDraw->m_nCurPage = nPage1 + nPage2 - 1;
			pFrm->SendMessage(WM_COMMAND, ID_DRAW_CHANGE_PAGE);
		}
	}

	*pResult = 0;
}

void CShortcutPaneDrawing::SetItemPositionUpdate(long nCurPage)
{
	HTREEITEM hParentItem = m_wndTreeDrawing.GetRootItem();
	HTREEITEM hItem = m_wndTreeDrawing.GetChildItem(hParentItem);
	HTREEITEM hPrevItem = NULL;

	CString strPage = _T("");
	long nPage = 0;
	while(hItem)
	{
		if(m_wndTreeDrawing.GetChildItem(hItem)==NULL)
		{
			strPage = m_wndTreeDrawing.GetItemText(hItem);
			nPage = atoi(strPage.Left(3));
			if(nCurPage==nPage)
			{
				m_wndTreeDrawing.Expand(m_wndTreeDrawing.GetParentItem(hItem), TVE_EXPAND);	
				m_wndTreeDrawing.Select(hItem, TVGN_CARET);
				break;
			}
			hPrevItem = hItem;
			hItem = m_wndTreeDrawing.GetNextItem(hItem, TVGN_NEXT);
		}
		else
		{
			hPrevItem = hItem;
			hItem = m_wndTreeDrawing.GetChildItem(hItem);
		}

		if(hItem==NULL)
		{			
			hItem = m_wndTreeDrawing.GetParentItem(hPrevItem);
			hItem = m_wndTreeDrawing.GetNextItem(hItem, TVGN_NEXT);
			if(hItem==NULL)
			{
				hItem = m_wndTreeDrawing.GetNextItem(hParentItem, TVGN_NEXT);
				hParentItem = hItem;
				if(hItem==NULL)
					break;
			}
		}
	}
}

void CShortcutPaneDrawing::OnNcRButtonUp(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CXTPShortcutBarPane::OnNcRButtonUp(nHitTest, point);
}

void CShortcutPaneDrawing::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CXTPShortcutBarPane::OnRButtonUp(nFlags, point);
}

void CShortcutPaneDrawing::OnSize(UINT nType, int cx, int cy) 
{
	CXTPShortcutBarPane::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CXTPClientRect rc(this);
	if(m_wndTreeDrawing.GetSafeHwnd() != 0)
	{
		m_wndTreeDrawing.MoveWindow(rc);
	}	
}
