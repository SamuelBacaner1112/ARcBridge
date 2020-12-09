#include "stdafx.h"
#include "ARcBridge.h"
#include "ARcBridgeStmView.h"
#include "ARcBridgeDoc.h"
#include "MainFrm.h"

#include "ARcBridgeView.h"

#include "../Src/ARcBridgeDrawingOption/ARcBridgeDrawingOption.h"
#include "../Src/ARcBridgeDrawing/ARcBridgeDrawingStd.h"
#include "../Src/ARcBridgeDrawingOption/OptionSheet.h"
#include "../Src/ARcBridgeOutBM/ARcBridgeOutBM.h"
#include "../Src/ARcBridgeOutDesign/ARcBridgeOutDesign.h"
#include "../Src/ARcBridgeInput/InputStmModelBase.h"

IMPLEMENT_DYNCREATE(CARcBridgeStmView, CStmSpecialView)

CARcBridgeStmView::CARcBridgeStmView(void)
{
	m_bEditBoundary = FALSE;
}


CARcBridgeStmView::~CARcBridgeStmView(void)
{
}

BEGIN_MESSAGE_MAP(CARcBridgeStmView, CStmSpecialView)
	ON_COMMAND(ID_DRAWING_OPTION, OnDrawingOption)
	ON_COMMAND(ID_DRAWING_EXPORT, OnDrawingExport)
	ON_COMMAND(ID_STRUCT_OUT, OnStructOut)
	ON_COMMAND(ID_GEN_OUT, OnGenOut)
	ON_COMMAND(ID_COMMON_OUT, OnCommonOut)
	ON_COMMAND(ID_EARTH_OUT, OnEarthOut)
	ON_COMMAND(ID_INPUT_END, OnDrawViewCurrentPage)
END_MESSAGE_MAP()

void CARcBridgeStmView::SetCustomInitialUpdate()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(GetDocument());
	if(pDoc == NULL) return;

	pDoc->m_pARcBridgeInputStd->SetUbRenderer(&m_UbRenderer);
}

void CARcBridgeStmView::SetCustomStatusBarText(const CString &strText)
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	if(pFrm == NULL) return;

	pFrm->m_wndStatusBar.SetPaneText(0, strText);
}

void CARcBridgeStmView::SetCustomSize()
{
	m_UbRenderer.ZoomAll();
}

void CARcBridgeStmView::SetCustomLButtonDown()
{
	if(m_UbRenderer.GetActionTypeCur() == ACTION_STM_ADD_ELEMENT)
	{
		CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(GetDocument());
		if(pDoc == NULL) return;

		CStmActionAddElement *pAction = reinterpret_cast<CStmActionAddElement*>(m_UbRenderer.GetAction(ACTION_STM_ADD_ELEMENT));
		if(pAction->IsActionCmd(eStmActionInsertPre))
		{
			CInputStmModelBase *pDlg = reinterpret_cast<CInputStmModelBase*>(pDoc->m_pARcBridgeInputStd->GetCurrentDialog());
			pDlg->RecalculateStmLoadDistribution();
		}
	}
}

void CARcBridgeStmView::SetCustomUndo(CHgModelEntityObj *pEnt)
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(GetDocument());
	if(pDoc == NULL) return;

	if(pEnt->m_nEntType == CStmEntityObject::ENT_STM_ELEMENT)
	{
		CInputStmModelBase *pDlg = reinterpret_cast<CInputStmModelBase*>(pDoc->m_pARcBridgeInputStd->GetCurrentDialog());
		pDlg->RecalculateStmLoadDistribution();
	}
}

void CARcBridgeStmView::SetCustomRedo(CHgModelEntityObj *pEnt)
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(GetDocument());
	if(pDoc == NULL) return;

	if(pEnt->m_nEntType == CStmEntityObject::ENT_STM_ELEMENT)
	{
		CInputStmModelBase *pDlg = reinterpret_cast<CInputStmModelBase*>(pDoc->m_pARcBridgeInputStd->GetCurrentDialog());
		pDlg->RecalculateStmLoadDistribution();
	}
}

void CARcBridgeStmView::SetCustomLock()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(GetDocument());
	if(pDoc == NULL) return;

	CInputStmModelBase *pDlg = reinterpret_cast<CInputStmModelBase*>(pDoc->m_pARcBridgeInputStd->GetCurrentDialog());

	pDlg->SetUnlock();
}

CStmUbManager *CARcBridgeStmView::GetUbManager()
{	
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(GetDocument());
	if(pDoc == NULL) return NULL;

	return pDoc->GetUbManager();
}

void CARcBridgeStmView::SetDomyunView(CARcBridgeView *pView)
{
	m_pDomyunView = pView;
}

void CARcBridgeStmView::SettingViewByShortcutBar()
{
	m_pDomyunView->SettingViewByShortcutBar();
}

void CARcBridgeStmView::OnDrawingOption() 
{
	m_pDomyunView->SendMessage(WM_COMMAND, ID_DRAWING_OPTION);
}

void CARcBridgeStmView::OnDrawingExport() 
{
	m_pDomyunView->SendMessage(WM_COMMAND, ID_DRAWING_EXPORT);
}

void CARcBridgeStmView::OnStructOut() 
{
	m_pDomyunView->SendMessage(WM_COMMAND, ID_STRUCT_OUT);
}

// 일반수량 출력
void CARcBridgeStmView::OnGenOut() 
{
	m_pDomyunView->SendMessage(WM_COMMAND, ID_GEN_OUT);
}

// 공통수량 출력
void CARcBridgeStmView::OnCommonOut() 
{
	m_pDomyunView->SendMessage(WM_COMMAND, ID_COMMON_OUT);
}

// 토공수량 출력
void CARcBridgeStmView::OnEarthOut() 
{
	m_pDomyunView->SendMessage(WM_COMMAND, ID_EARTH_OUT);
}

void CARcBridgeStmView::OnDrawViewCurrentPage()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(GetDocument());
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();

	pFrm->SendMessage(WM_SWITCH_VIEW, VIEWID_DEFAULT, ID_INPUT_END);

	CARcBridgeDataStd* pStd = pDoc->m_pARcBridgeDataStd;
	CARcBridgeDrawingStd *pDraw = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd *pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CDrawPageMng* pDrawMng = pDraw->GetDrawPageMng();


	// INSERT_ATESTCODE
	AfxGetMainWnd()->SendMessage(WM_ATT_MAINMSG,eMsgAppSetCAME_IN_DOMYUNPOS);


	if(pDraw->GetCountTotalPage() == 0)
	{ 
		pFrm->m_paneShortcutDrawing.InitTree();
		if(pDraw->GetCountTotalPage() == 0) return;
	}

	CXTPShortcutBarItem* pSelItem = pFrm->m_wndShortcutBar.GetItem(2);
	pFrm->m_wndShortcutBar.SelectItem(pSelItem);

	pDrawMng->SetPageInit();
	if(pDraw->m_nCurPage < 1)
		pDraw->m_nCurPage = 1;
	if(pDraw->m_nCurPage > pDraw->m_pDrawPageMng->m_nTotalPageSize)
		pDraw->m_nCurPage = pDraw->m_pDrawPageMng->m_nTotalPageSize;

	pDraw->SetModified(pDraw->GetCurPage(), FALSE);	
	COutMsgDlg msgDlg(this, pDraw->GetDrawPageMng());
	msgDlg.Run();

	long nSubPage = pDraw->GetSubPage(pDraw->m_nCurPage) + 1;
	pFrm->m_paneShortcutDrawing.SetItemPositionUpdate(nSubPage);
	pFrm->SendMessage(WM_UPDATE_DRAWING_TITLE, pDraw->m_nCurPage);
	pFrm->SendMessage(WM_UPDATE_TOOLBAR_DRAW);
	Sleep(0);

	pDoc->m_pARcBridgeInputStd->HideDialog();
	pFrm->m_paneManager.ClosePane(IDR_PANE_INPUT);
	pDoc->m_pARcBridgeInputStd->SetSaveMenuIDD(ID_INPUT_END);
}
