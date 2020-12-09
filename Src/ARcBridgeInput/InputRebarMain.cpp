// InputRebarMain.cpp : implementation file
//

#include "stdafx.h"
#include "../../Src/ARcBridgeCalculate/ARcBridgeCalculate.h"
#include "arcbridgeinput.h"
#include "../ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "RebarJoint1CycleDlg.h"
#include "TypeCycle3Dlg.h"
#include "RebarDanDlg.h"
#include "RebarHunchDlg.h"
#include "OptimizeRebar.h"
#include "RebarSupportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputRebarMain dialog


CInputRebarMain::CInputRebarMain(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputRebarMain::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputRebarMain)
	//}}AFX_DATA_INIT
	m_nTabCycle1 = -1;
	m_nTabCycle3 = -1;
	m_nTabLowerSlab = -1;
	m_nTabCycle2 = -1;
	m_nTabFooting = -1;
	m_nTabArchRib = -1;
	m_nTabInWall = -1;
	m_nTabUpperSlab = -1;

	m_nTabOutWall = -1;
	m_nTabCycle4 = -1;
	m_nTab = 0;
	m_nTabCycle = 0;
 
	m_pDlgCheck = new CInputCheckGrid;
	m_pDlgCheckRib = new CInputCheckArchRib;
	m_pProgress	= new CDlgProgress;
}

CInputRebarMain::~CInputRebarMain()
{
	delete m_pDlgCheck;
	delete m_pDlgCheckRib;
	delete m_pProgress;
}

void CInputRebarMain::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputRebarMain)
	DDX_Control(pDX, IDC_CHECK_VIEW_CYCLE2, m_checkViewCycle2);
	DDX_Control(pDX, IDC_CHECK_2DAN, m_check2Dan);
	DDX_Control(pDX, IDC_BUTTON_CHECKRIB, m_btnCheckRib);
	DDX_Control(pDX, IDC_STATIC_RIB, m_staticRib);
	DDX_Control(pDX, IDC_COMBO_DAN, m_comboDan);
	DDX_Control(pDX, IDC_CHECK_REBAR_LENGTH, m_checkRebarLength);
	DDX_Control(pDX, IDC_BUTTON_OPTIMIZE, m_btnOptimize);
	DDX_Control(pDX, IDC_CHECK_JOINT, m_checkJoint);
	DDX_Control(pDX, IDC_BUTTON_SAMESTTEND2, m_btnSameSttEnd);
	DDX_Control(pDX, IDC_BUTTON_SAMEINWALL2, m_btnSameInWall);
	DDX_Control(pDX, IDC_CHECK_BMD, m_checkBMD);
	DDX_Control(pDX, IDC_BUTTON_TYPECYCLE3, m_btnTypeCycle3);
	DDX_Control(pDX, IDC_COMBO_CTC, m_comboCTC);
	DDX_Control(pDX, IDC_BUTTON_REBARJOINT, m_btnRebarJoint);
	DDX_Control(pDX, IDC_BUTTON_REBARDAN, m_btnRebarDan);
	DDX_Control(pDX, IDC_CHECK_ALLMARK, m_checkMark);
	DDX_Control(pDX, IDC_BUTTON_EXAMINE, m_btnExamine);
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_comboBridgeNo);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	DDX_Control(pDX, IDC_TAB_CYCLE, m_TabCycle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputRebarMain, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputRebarMain)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_BN_CLICKED(IDC_BUTTON_REBARJOINT, OnButtonRebarjoint)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_BN_CLICKED(IDC_BUTTON_REBARDAN, OnButtonRebardan)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnSelchangingTab)
	ON_BN_CLICKED(IDC_CHECK_ALLMARK, OnCheckAllmark)
	ON_CBN_SELCHANGE(IDC_COMBO_CTC, OnSelchangeComboCtc)
	ON_BN_CLICKED(IDC_BUTTON_TYPECYCLE3, OnButtonTypecycle3)
	ON_BN_CLICKED(IDC_CHECK_BMD, OnCheckBmd)
	ON_BN_CLICKED(IDC_BUTTON_EXAMINE, OnButtonExamine)
	ON_BN_CLICKED(IDC_BUTTON_SAMEINWALL2, OnButtonSameinwall2)
	ON_BN_CLICKED(IDC_BUTTON_SAMESTTEND2, OnButtonSamesttend2)
	ON_BN_CLICKED(IDC_CHECK_JOINT, OnCheckJoint)
	ON_BN_CLICKED(IDC_BUTTON_OPTIMIZE, OnButtonOptimize)
	ON_BN_CLICKED(IDC_CHECK_REBAR_LENGTH, OnCheckRebarLength)
	ON_CBN_SELCHANGE(IDC_COMBO_DAN, OnSelchangeComboDan)
	ON_BN_CLICKED(IDC_BUTTON_CHECKRIB, OnButtonCheckrib)
	ON_BN_CLICKED(IDC_CHECK_2DAN, OnCheck2dan)
	ON_BN_CLICKED(IDC_CHECK_VIEW_CYCLE2, OnCheckViewCycle2)
	ON_BN_CLICKED(IDC_BUTTON_SUPPORT, OnButtonRebarSupport)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW,  IDC_GRID, OnCellChangedRow)

	ON_NOTIFY(GVN_CELL_INSERTED_COL, IDC_GRID, OnCellInsertedCol)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CYCLE, OnTabSelchangeCycle)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_CYCLE, OnTabSelchangingCycle)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputRebarMain message handlers
void CInputRebarMain::OnTabSelchangingCycle(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	
	return;
}

void CInputRebarMain::SetGridData()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long nCycle	= GetCurCycle();
	long nDan	= m_comboDan.GetCurSel();
	BOOL bDetailInput	= pBri->m_bDetailInput23DanMainRebar;
	CInputRebarMain2 *pDlg	= dynamic_cast<CInputRebarMain2*>(m_pIn->GetDialogByIDD(IDD_REBAR_MAIN2));
	
	if(m_nTab == m_nTabUpperSlab)
	{
		m_Grid.InitGrid(pBri->GetValueFy(ePartUpperSlab));
		if(bDetailInput && nDan > 0)
		{
			pDlg->SetGridData_UpperSlab(nCycle, &m_Grid, nDan);
		}
		else
		{
			SetGridData_UpperSlab(nCycle);
		}
		m_nRowFocusItemTitle	= 1;
	}
	else if(m_nTab == m_nTabLowerSlab)
	{
		m_Grid.InitGrid(pBri->GetValueFy(ePartLowerSlab));
		if(bDetailInput && nDan > 0)
		{
			pDlg->SetGridData_LowerSlab(nCycle, &m_Grid, nDan);
		}
		else
		{
			SetGridData_LowerSlab(nCycle);
		}
		m_nRowFocusItemTitle	= 1;
	}
	else if(m_nTab == m_nTabOutWall)
	{
		m_Grid.InitGrid(pBri->GetValueFy(ePartOutWallStt));
		if(bDetailInput && nDan > 0)
		{
			pDlg->SetGridData_OutWall(nCycle, &m_Grid, nDan);
		}
		else
		{
			if(IsSet2Dan())
				SetGridData_OutWall2Dan(nCycle);
			else
				SetGridData_OutWall(nCycle);
		}
		m_nRowFocusItemTitle	= 1;
	}
	else if(m_nTab == m_nTabInWall)
	{
		m_Grid.InitGrid(pBri->GetValueFy(ePartInWall));

		if(bDetailInput && nDan > 0)
		{
			pDlg->SetGridData_InWall(nCycle, &m_Grid, nDan);
		}
		else
		{
			SetGridData_InWall(nCycle);
		}
		m_nRowFocusItemTitle	= 1;
	}
	else if(m_nTab == m_nTabFooting)
	{
		m_Grid.InitGrid(pBri->GetValueFy(ePartFootStt));
		SetGridDataCycle13_Footing(nCycle);
		m_nRowFocusItemTitle	= 0;
	}
	else if(m_nTab == m_nTabArchRib)
	{
		SetGridData_ArchRib(nCycle);
		m_nRowFocusItemTitle	= 1;
	}

	//m_Grid.SetRedraw(TRUE, TRUE);
}

void CInputRebarMain::SetDataSave()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	m_Grid.UpdateData(TRUE);

	SyncRebar();

	// 컨트롤 값 저장
	pBri->m_nIdxCTC_MainRebar	= m_comboCTC.GetCurSel();

	// 상하행선 분리 적용
	m_pIn->m_pARcBridgeDataStd->CopyToCoupleBridge_RebarMain(m_pIn->m_nCurrentBridge);

	if(m_pDlgCheck->GetSafeHwnd() && m_pDlgCheck->IsWindowVisible())
	{
		pBri->MakeRebarInfoCycleStd(TRUE);
		m_pDlgCheck->Check();
	}

	if(m_nTab == m_nTabUpperSlab)
	{
		// 슬래브 주철근과 배력근 좌표 초기화
		pBri->m_bCaledRebarMain_Plane	= FALSE;
		pBri->m_bCaledRebarSupport_Plane	= FALSE;
	}
}

void CInputRebarMain::SetDataDefault()
{
	for(long bri = 0; bri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(bri);
		if(!pBri) return;

		BOOL bFirst   = IsFirst(bri);
		pBri->SetDefaultedState(GetIDD(), 0x00000001);
		if(!bFirst && !IsClickDefaultButton(bri)) continue;

		BOOL bVertDir = FALSE;
		if(bFirst)
		{
			// 처음들어온다면 Sync는 맞춰주고 기본값을 하자.
			bVertDir	= (pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH && pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)? TRUE : pBri->IsVertDir();

			//pBri->SyncBridge(0, !bVertDir);
			pBri->GetTvVertSection_Std(0, !bVertDir, FALSE);
			pBri->SyncBridgeRebar(TRUE, bVertDir?-1:1, TRUE);
		}

		long nType	= 0;
		if(m_nTab == m_nTabUpperSlab) nType = 0;
		else if(m_nTab == m_nTabOutWall) nType = 1;
		else if(m_nTab == m_nTabInWall) nType = 2;
		else if(m_nTab == m_nTabFooting) nType = 3;
		else if(m_nTab == m_nTabArchRib) nType = 4;
		else if(m_nTab == m_nTabLowerSlab) nType = 5;
		else nType = -1;
		
		// 처음 권고안일경우 전체 권고안
		long nCycle = GetCurCycle();
		if(bFirst) 
		{
			nType	= -1;	
			nCycle	= -1;
		}

		if(bFirst)
		{
			pBri->SetDataDefaultSetRebarDan(-1);
			pBri->SetDataDefaultCountJointMainRebarInWall();
			pBri->SetDataDefaultCountJointMainArchRib();
			pBri->SetDataDefaultRebarInfoCycle_Std(nType, nCycle);
			
		}
		else
		{
			pBri->SetDataDefaultRebarInfoCycle_Std(nType, nCycle, FALSE, pBri->m_bDetailInput23DanMainRebar ? m_comboDan.GetCurSel() : 0);
			// 상하행선 분리 적용
			m_pIn->m_pARcBridgeDataStd->CopyToCoupleBridge_RebarMain(m_pIn->m_nCurrentBridge);
		}

		// 슬래브 주철근과 배력근 좌표 초기화
		if(m_nTab == m_nTabUpperSlab)
		{
			pBri->m_bCaledRebarMain_Plane	= FALSE;
			pBri->m_bCaledRebarSupport_Plane	= FALSE;
		}

		if(bFirst)
		{
			pBri->GetTvVertSection_Std(0, !bVertDir, FALSE, TRUE);
		}
	}

	SetGridData();
	SetDataInit();

	
}

void CInputRebarMain::InitTabCtrl()
{
	m_TabCycle.RemoveAllTabs();
	m_TabCycle.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_TabCycle.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	// 사이클 탭
	CStringArray sArrTabCycleName;
	long nTab		= 0;
	m_nTabCycle1	= -1;
	m_nTabCycle2	= -1;
	m_nTabCycle3	= -1;
	m_nTabCycle4	= -1;
	sArrTabCycleName.Add("① CYCLE"); m_nTabCycle1 = nTab++;
	if(pBri->GetCountCycleMainRebar() == 4)
	{
		sArrTabCycleName.Add("③ CYCLE"); m_nTabCycle3	= nTab++;
	}
	// 2005.01.03부터 이 입력창에서는 1 cycle만 입력 받기로 함.
	// 혹시라도 다른  cycle을 입력 받는 것으로 변경을 원한다면
	// 바로 아래 주석만 풀면 만사 ok
	/*
	sArrTabCycleName.Add("② CYCLE"); m_nTabCycle2 = nTab++;
	if(pBri->GetCountCycleMainRebar() > 2)
	{
		sArrTabCycleName.Add("③ CYCLE"); m_nTabCycle3 = nTab++;
		sArrTabCycleName.Add("④ CYCLE"); m_nTabCycle4 = nTab++;
	}
	*/

	long i = 0; for(i = 0; i < sArrTabCycleName.GetSize(); i++)
	m_TabCycle.InsertItem(i, sArrTabCycleName[i]);

	if(m_TabCycle.GetItemCount() <= m_nTabCycle) m_nTabCycle = 0;
	m_TabCycle.SetCurSel(m_nTabCycle);

	nTab			= 0;
	m_nTabUpperSlab	= -1;
	m_nTabLowerSlab = -1;
	m_nTabOutWall	= -1;
	m_nTabInWall	= -1;
	m_nTabFooting	= -1;
	m_nTabArchRib		= -1;
	m_sArrTabName.RemoveAll();
	
	m_sArrTabName.Add("상부슬래브"); m_nTabUpperSlab = nTab++;
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		m_sArrTabName.Add("하부슬래브"); m_nTabLowerSlab = nTab++;
	}
	if(pBri->IsOutWall(TRUE) || pBri->IsOutWall(FALSE))
	{
		m_sArrTabName.Add("외측벽체"); m_nTabOutWall = nTab++;
	}
	if(pBri->GetCountInWall() > 0 && pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB)
	{
		if(!(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH && pBri->GetJijumConnectRibAndSlab() == 1 && pBri->GetCountInWall() == 1))
		{
			m_sArrTabName.Add("내측벽체"); m_nTabInWall = nTab++;
		}
	}
	if(pBri->GetCountFooting() > 0 && pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB && pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
	{
		m_sArrTabName.Add("기초"); m_nTabFooting = nTab++;
	}
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		m_sArrTabName.Add("아치리브"); m_nTabArchRib	= nTab++;
	}
		
	m_Tab.DeleteAllItems();
	for(i = 0; i < m_sArrTabName.GetSize(); i++)
		m_Tab.InsertItem(i, m_sArrTabName[i]);

	if(m_Tab.GetItemCount() <= m_nTab) m_nTab = 0;
	if(m_pIn->m_nLastTab_RebarMain > 0 && m_pIn->m_nLastTab_RebarMain < m_Tab.GetItemCount())
		m_nTab	= m_pIn->m_nLastTab_RebarMain;
	m_Tab.SetCurSel(m_nTab);
}

void CInputRebarMain::OnPreInitDialog()
{
	// 시작하면서 pick box의 크기를 정해줌
	if(m_pView) 
	{
		CTwinVector tv;
		tv.SetPickBoxSize(m_pView->GetSelectionRectSize()/m_pView->GetRatioWindow());
	}
	
	m_checkJoint.SetCheck(TRUE);
	m_checkRebarLength.SetCheck(TRUE);
	InitTabCtrl();
	InitComboCtrl();

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	m_btnCheckRib.ShowWindow((pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_BUTTON_SUPPORT)->ShowWindow(m_bDesignLsd ? SW_SHOW : SW_HIDE);

	SetResize(IDC_BUTTON_EXAMINE,		SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_OPTIMIZE,		SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
	SetResize(IDC_TAB,					SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
	SetResize(IDC_TAB_CYCLE,			SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID,					SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_TYPECYCLE3,	SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_REBARDAN,		SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_REBARJOINT,	SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_SUPPORT,		SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_CHECKRIB,		SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_VIEW,			SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_ALLMARK,		SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_BMD,			SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_JOINT,			SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_REBAR_LENGTH,		SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_VIEW_CYCLE2,		SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_SAMEINWALL2,		SZ_BOTTOM_RIGHT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_SAMESTTEND2,		SZ_BOTTOM_RIGHT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_CHECK_2DAN,		SZ_BOTTOM_RIGHT,		SZ_BOTTOM_RIGHT);

	SetHyperHelp(m_pIn->GetManualPath());

	m_Grid.InitGrid(m_pIn->m_pARcBridgeDataStd->m_Fy);
	m_comboDan.SetCurSel(0);
	SetGridData();
}

BOOL CInputRebarMain::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		UpdateData();
		if(m_Grid.GetFocusCell().col == m_Grid.GetCols()-1)
			pMsg->wParam = VK_RIGHT;
		else
			pMsg->wParam = VK_TAB;
	}
	
	structGridCell *cell = m_Grid.GetCurrentCell();
	if(cell)
	{
		
	}

	return CInputDialogBase::PreTranslateMessage(pMsg);
}

void CInputRebarMain::InitComboCtrl()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	CString str = _T("");
	// 교량
	long nSize = pStd->GetBridgeSize();
	long i = 0; for(i = 0; i < nSize; i++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(i);
		str = pStd->GetStringBridgeOrder(i) + _T(" : ") + pBri->m_strBridgeName
			+ _T(" [") + pBri->GetStringBridgeType() + _T("]");
		m_comboBridgeNo.AddString(str);
	}
	m_comboBridgeNo.SetCurSel(m_pIn->m_nCurrentBridge);

	// CTC
	for(long ctc = 0; ctc < MAX_REBAR_CTC; ctc++)
	{
		CHgBaseConcStd baseConcStd;
		str.Format("CTC %.0f", baseConcStd.GetValueRebarCTC(ctc));
		m_comboCTC.AddString(str);
	}

	SetControl();
}

void CInputRebarMain::OnSelchangeComboBridgeNo() 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	m_pIn->m_nCurrentBridge = m_comboBridgeNo.GetCurSel();
	InitTabCtrl();
	SetControl();
	SetGridData();
	SetDataInit();
	SetPaneTitle();
	DrawInputDomyunView(TRUE);
	m_comboBridgeNo.SetFocus();
}

void CInputRebarMain::DrawInputDomyunView(BOOL bZoomAll)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(m_pIn->GetScaleInputDialog());

	CDomyun Dom(pDom);
	BOOL bCheckOverLength	= m_checkRebarLength.GetCheck();
	CARcBridgeDBStandard DBStd(m_pIn->GetRcBridgeRebar(), m_pIn->m_pARoadOptionStd, m_pIn->m_pARcBridgeDataStd);
	DBStd.m_bCheckOverLengthRebar	= bCheckOverLength;

	// 말뚝 비율을 기본값으로
	pBri->m_footingEnd.m_exFooting.m_Pile.InitScaleCutting();

	// 이음장 치수 표기 할지?
	BOOL bViewJointDim	= m_checkJoint.GetCheck();
	
	// 기초와 교량중 현재 그리는 개체에 대한 셋팅을 해준다
	// 기초를 그려야 될때는 해당 기초가 사보강인지 판단하고, 다른걸 그릴때는 슬래브가 사보강인지 판단해서
	// 사보강이면 직거리로 셋팅해준다.
	BOOL bDrawFooting	= m_nTab == m_nTabFooting;
	BOOL bDrawBridge	= !bDrawFooting;
	long nIdxFooting	= GetIdxJijumCurFooting();
	CFootingApp *pFooting	= NULL;
	if(nIdxFooting > -1)
		pFooting	= pBri->GetFooting(nIdxFooting);
	
	// 모든 객체들의 동기화를 해줌.
	BOOL bVertDir	= pFooting && pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH? TRUE : pBri->IsVertDir();
	long nVertDir	= pFooting && pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH? -1 : 0;
	
	//pBri->SyncBridge(0, !bVertDir);
	pBri->GetTvVertSection_Std(0, !bVertDir, FALSE);
	pBri->SyncBridgeRebar(TRUE, bVertDir?-1:1, TRUE);
	
	// 종단면 그림
	DBStd.m_pDrawVertSection->DrawVertSectionAll(&Dom, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, !bVertDir);
	DBStd.m_pDrawVertSection->DrawDiagramBMD_SFD(&Dom, m_checkBMD.GetCheck(), FALSE, FALSE);

	long nCycle = GetCurCycle();
	BOOL bDimUpperSlab	= (m_nTab == m_nTabUpperSlab);
	BOOL bDimOutWall	= (m_nTab == m_nTabOutWall);
	BOOL bDimInWall		= (m_nTab == m_nTabInWall);
	BOOL bDimFooting	= (m_nTab == m_nTabFooting);
	BOOL bDimRib		= (m_nTab == m_nTabArchRib);
	BOOL bDimLowerSlab	= (m_nTab == m_nTabLowerSlab);
	BOOL bAllMark		= m_checkMark.GetCheck();
	
	// 모든 철근 마크 표기일때.
	if(bAllMark)
	{
		bDrawBridge		= TRUE;
		bDrawFooting	= TRUE;
		bDimUpperSlab	= TRUE;
		bDimOutWall		= TRUE;
		bDimInWall		= TRUE;
		bDimFooting		= TRUE;
		bDimRib			= TRUE;
		bDimLowerSlab	= TRUE;
	}

	// 철근 그리기(치수, 이음 포함)
	long nCountWall		= pBri->GetCountInWall();
	long nCountFooting	= pBri->GetCountFooting();
	long nDan			= bDimRib || pBri->m_bDetailInput23DanMainRebar ? m_comboDan.GetCurSel() : 0;
	
	// hidden == 0 : 현재 사이클 그림
	// hidden == 1 : 점선으로 그려야 되는 사이클 그림
	for(long hidden = 0; hidden < 2; hidden++)
	{
		BOOL bHidden	= hidden == 1;
		if(hidden == 1 && !m_checkViewCycle2.GetCheck()) continue;
		if(hidden == 1)
		{
			switch(nCycle)
			{
			case 0: nCycle	= 1; break;
			case 2: nCycle	= 3; break;
			}
			DBStd.m_bDrawRebarInfoCycleByHidden	= TRUE;
		}

		long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			if(bDrawBridge)
			{
				DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_UpperSlab(&Dom, nCycle, dan, TRUE, !bHidden && bDimUpperSlab && dan==nDan, !bHidden && bDimUpperSlab, 0, CDRect(0, 0, 0, 0), TRUE, nVertDir);
				DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_ArchRib(&Dom, nCycle, dan, TRUE, !bHidden && bDimRib && dan==nDan, !bHidden && bDimRib && dan == nDan, 0, CDRect(0, 0, 0, 0), TRUE, nVertDir);
				DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_LowerSlab(&Dom, nCycle, dan, TRUE, !bHidden && bDimLowerSlab && dan==nDan, !bHidden && bDimLowerSlab, 0, CDRect(0, 0, 0, 0), TRUE, nVertDir);

				DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_OutWall(&Dom, nCycle, dan, TRUE, !bHidden && bDimOutWall && dan==nDan, !bHidden && bDimOutWall);
				DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_OutWall_Hunch(&Dom, nCycle, !bHidden && bDimOutWall);
				
				// 외측벽체 입력시 상부슬래브와 외측벽체 연결된 철근 마크해줘야 함
				if(!bDimUpperSlab && bDimOutWall)
				{
					CRebarInfoCycle *pRC	= pBri->GetRebarInfoCycleUpperSlab(nCycle, 0, TRUE, 0);
					if(pRC)
						DBStd.MarkRebarInfoCycle(&Dom, pRC, TRUE, FALSE, TRUE, 2, 0, CDRect(0, 0, 0, 0), 0.5, FALSE, &pBri->m_pArrRebarMain);

					pRC	= pBri->GetRebarInfoCycleUpperSlab(nCycle, 0, TRUE, -1);
					if(pRC)
						DBStd.MarkRebarInfoCycle(&Dom, pRC, TRUE, FALSE, TRUE, 0, 0, CDRect(0, 0, 0, 0), 0.5, FALSE, &pBri->m_pArrRebarMain);
				}
			
				// 벽체들 철근 그리기
				long wall = 0; for(wall = 0; wall < nCountWall; wall++)
				{
					CWallApp *pWall	= pBri->GetInWall(wall);
					if(pWall)
					{
						for(long left = 0; left < 2; left++)
						{
							BOOL bLeft	= left == iLEFT;
							DBStd.m_pDrawWall->DrawRebarMain_Front(&Dom, pWall, nCycle, dan, bLeft, !bHidden && dan == nDan && bDimInWall, !bHidden && bDimInWall);
						}
					}
				}
			}

			// (42071) 입력탭이 기초이고 직각방향일때는 거더 철근 위치가 맞지 않으니 그리지 말자.
			if(!(bDrawFooting && nVertDir == -1))
			{
				// 거더 철근
				long nJ = 0; for(nJ = 0; nJ < pBri->GetCountJijum(); nJ++)
				{
					DBStd.m_pDrawVertSection->DrawRebarAll_SlabGirder(&Dom, nJ, FALSE, FALSE, 0, FALSE, TRUE, TRUE, TRUE, TRUE, FALSE, -1, TRUE, TRUE);
					DBStd.m_pDrawVertSection->DrawRebarAll_SlabGirder(&Dom, nJ, FALSE, FALSE, 0, FALSE, TRUE, TRUE, TRUE, TRUE, FALSE, -1, TRUE, FALSE);
				}
			}

			// 기초들 그리기
			if(bDrawFooting)
			{
				long footing = 0; for(footing = 0; footing < nCountFooting; footing++)
				{
					CFootingApp *pFooting	= pBri->GetFooting(footing);
					if(!pFooting) continue;
					if(nIdxFooting != footing && !bAllMark) continue;

					for(long upper = 0; upper < 2; upper++)
					{
						DBStd.m_pDrawFooting->DrawRebarMain_Front(&Dom, pFooting, nCycle, dan, upper, !bHidden && bDimFooting && dan==0, !bHidden && bDimFooting);	
					}
				}
			}
		}

		if(bDrawBridge)
		{
			DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_UpperSlabHunch(&Dom, nCycle, FALSE);
			DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_LowerSlabHunch(&Dom, nCycle, FALSE);
			DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_Danbu(&Dom, FALSE, FALSE, FALSE, nCycle);
			for(long stt = 0; stt < 2; stt++)
			{
				BOOL bStt	= stt == iSTT;
				DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_Bracket(&Dom, bStt, FALSE, !bHidden && bDimOutWall, TRUE, 0);
				DBStd.m_pDrawVertSection->DrawBFootingRebar(&Dom, bStt, FALSE, FALSE, TRUE, FALSE, FALSE);
			}
		}
	}

	nCycle	= GetCurCycle();
	DBStd.m_bDrawRebarInfoCycleByHidden	= FALSE;

	// 단부 치수 표기
	DBStd.m_pDrawVertSection->DimRebarMainAssm_VertSection_UpperSlabSide(&Dom, 0, nCycle, 0);

	// 지점별 슈 위치 표시(삼각형)
	long nJ = 0; for(nJ = 0; nJ < pBri->GetCountJijum(); nJ++)
		DBStd.m_pDrawVertSection->DrawJijumSign(&Dom, nJ);


	// 이음장 치수 기입
	if(bDrawBridge && bViewJointDim)
	{
		long nDimDanJointDim	= 3;
		if(bDimUpperSlab)
		{
			if(pBri->IsBoxType())
				DBStd.m_pDrawVertSection->DimRebarMainAssm_VertSection_BoxOutSide(&Dom, nDimDanJointDim, nCycle, 0, nDan);
			else
				DBStd.m_pDrawVertSection->DimRebarMainAssm_VertSection_Upper(&Dom, nDimDanJointDim, nCycle, 0, nDan);
			DBStd.m_pDrawVertSection->DimRebarMainAssm_VertSection_Lower(&Dom, nDimDanJointDim, nCycle, 0, nDan);
		}
		else if(bDimLowerSlab)
		{
			DBStd.m_pDrawVertSection->DimRebarMainAssm_VertSection_BoxOutSide(&Dom, nDimDanJointDim, nCycle, 0, nDan);
			DBStd.m_pDrawVertSection->DimRebarMainAssm_VertSection_Lower(&Dom, nDimDanJointDim, nCycle, 0, nDan, FALSE);
		}
		else if(bDimOutWall)
		{
			DBStd.m_pDrawVertSection->DimRebarMainAssm_VertSection_OutWall(&Dom, nDimDanJointDim, nCycle, TRUE, 0, nDan);
			DBStd.m_pDrawVertSection->DimRebarMainAssm_VertSection_OutWall(&Dom, nDimDanJointDim, nCycle, FALSE, 0, nDan);
			if(pBri->IsBoxType())
				DBStd.m_pDrawVertSection->DimRebarMainAssm_VertSection_BoxOutSide(&Dom, nDimDanJointDim, nCycle, 0, nDan);
			else
				DBStd.m_pDrawVertSection->DimRebarMainAssm_VertSection_Upper(&Dom, nDimDanJointDim, nCycle, 0, nDan);
		}
		else if(bDimRib)
		{
			DBStd.m_pDrawVertSection->DimRebarMainAssm_VertSection_ArchRib(&Dom, 1, nCycle, 0);
		}
		else
		{
			// 벽체들 철근 그리기
			long wall = 0; for(wall = 0; wall < nCountWall; wall++)
			{
				CWallApp *pWall	= pBri->GetInWall(wall);
				if(pWall)
				{
					for(long left = 0; left < 2; left++)
					{
						if(bViewJointDim && bDimInWall)
						{
							DBStd.m_pDrawWall->DimRebarMainAssm_Front(&Dom, pWall, nDimDanJointDim, nCycle, left==iLEFT, 0, nDan);
						}
					}
				}
			}
		}
	}

	

	*pDom << Dom;

	pBri->GetTvVertSection_Std(0, !bVertDir, FALSE, TRUE);
	DBStd.m_bCheckOverLengthRebar	= FALSE;
		
	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

void CInputRebarMain::SetDataInit()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	m_Grid.UpdateData(FALSE);

	long nRows(0), nCols(0);
	m_Grid.GetCellCount(nRows, nCols);
	m_Grid.MergeGrid(0, 0, 0, nCols);

	CHgBaseConcStd baseConc;
	m_comboCTC.SetCurSel(baseConc.GetIdxByRebarCTC(pBri->m_dCTC_MainRebar));
}

void CInputRebarMain::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	structGridCell *pCell = m_Grid.GetCurrentCell();
	if(pCell)
	{
		m_pIn->SetComment(pCell->sTitle);
		CString str	= m_Grid.GetItemText(m_nTab==m_nTabFooting ? 0 : 1,pCell->nCol);
		m_pView->GetDomyun()->STMakeCursor(str);
	}
}

void CInputRebarMain::OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result)
{
	m_CellID	= m_Grid.GetFocusCell();

	if(m_nTab == m_nTabFooting)
		DrawInputDomyunView(FALSE);
}

void CInputRebarMain::SetControl()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	if(m_nTab == m_nTabFooting)
	{
		m_btnSameSttEnd.EnableWindow(TRUE);
		m_btnSameInWall.EnableWindow(TRUE);
	}
	else
	{
		m_btnSameSttEnd.EnableWindow(FALSE);
		m_btnSameInWall.EnableWindow(FALSE);
	}

	if(m_nTab == m_nTabArchRib)
	{
		m_comboDan.ShowWindow(TRUE);
		m_staticRib.ShowWindow(TRUE);
	}
	else
	{
		m_comboDan.ShowWindow(FALSE);
		m_staticRib.ShowWindow(FALSE);
	}

	// 2,3단 상세입력일 경우 
	if(pBri->m_bDetailInput23DanMainRebar)
	{
		m_comboDan.ShowWindow(TRUE);
		m_staticRib.ShowWindow(TRUE);

		m_check2Dan.SetCheck(FALSE);
		m_check2Dan.ShowWindow(FALSE);
	}

	if(pBri->GetCountCycleMainRebar() < 4)
		m_btnTypeCycle3.EnableWindow(FALSE);
	else
		m_btnTypeCycle3.EnableWindow(TRUE);
}

void CInputRebarMain::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);
}

// 사이클 1, 3 입력(상부슬래브 상,하면)
void CInputRebarMain::SetGridData_UpperSlab(long nCycle)
{
	if(!(nCycle == 0 || nCycle == 2)) return;

	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 2, 0);
	m_Grid.SetCellSize(20, 50);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long row		= 0, col = 0;
	CString str		= _T("");
	CString sHelp	= _T("");
	CString sUpdn	= _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;
//	long nNum		= 1;


	// 상부슬래브 상,하면 구간별 이음 간격/철근지름
	for(long upper = 0; upper < 2; upper++)
	{
		BOOL bUpper	= upper == iUPPER;
		long nType	= bUpper ? MARK_UPPERSLAB_UPPER : MARK_UPPERSLAB_LOWER;
		pBri->GetMarkRebarMain(nType, TRUE, nCycle, TRUE, TRUE, _T(""), 0);

		// 상부 슬래브 상면 각 구간별 이음 간격
		sUpdn		= bUpper ? "상면 " : "하면 ";
		long nNum		= 1;
		long n = 0; for(n = 0; n < pBri->m_nQtyJigan; n++)
		{
			long i = 0; for(i = 0; i < pBri->m_nCountJointMainRebar_UpperSlab_Cycle1[n][upper][0]; i++)
			{
				if(upper == iUPPER) str.Format("UD%d", nNum++);
				else str.Format("LD%d", nNum++);

				if(i > 0) sHelp.Format("%d번째 지간의 %d번째 이음과 %d번째 이음과의 간격", n+1, i, i+1);
				else sHelp.Format("%d번째 지간의 시작위치와 %d번째 이음과의 간격", n+1, i+1);
				sHelp = sUpdn + sHelp;

				// 하면이고 신축이음이 있다면 시점 처음과 종점 끝에 신축이음 철근 길이 추가
				if(!bUpper && pBri->IsBlockOut(TRUE) && n == 0 && i == 0)
				{
					m_Grid.AddCell(row, col, sUpdn);
					m_Grid.AddCell(row+1, col, "FXSL");
					m_Grid.AddCell(row+2, col, &pBri->m_fxJointStt.m_dLenHorRebar[nCycle][0], nFormat, nLine, "시점 신축이음 하면 철근 길이"); 
					m_Grid.SetCellDec(CELL_DEC_0);
					m_Grid.SetCellPrintZero(TRUE);
					if(!bUpper) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
					col++;
				}

				m_Grid.AddCell(row, col, sUpdn);
				m_Grid.AddCell(row+1, col, str);
				m_Grid.AddCell(row+2, col, &pBri->m_pArrDistUpperSlab[nCycle][upper][0], n, i, nFormat, nLine, sHelp); 
				m_Grid.SetCellDec(CELL_DEC_0);
				m_Grid.SetCellPrintZero(TRUE);
				if(!bUpper) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
				col++;

				// 하면이고 신축이음이 있다면 시점 처음과 종점 끝에 신축이음 철근 길이 추가
				if(!bUpper && pBri->IsBlockOut(FALSE) && n == pBri->m_nQtyJigan-1 && i == pBri->m_nCountJointMainRebar_UpperSlab_Cycle1[n][upper][0]-1)
				{
					m_Grid.AddCell(row, col, sUpdn);
					m_Grid.AddCell(row+1, col, "FXEL");
					m_Grid.AddCell(row+2, col, &pBri->m_fxJointEnd.m_dLenHorRebar[nCycle][0], nFormat, nLine, "시점 신축이음 하면 철근 길이"); 
					m_Grid.SetCellDec(CELL_DEC_0);
					m_Grid.SetCellPrintZero(TRUE);
					if(!bUpper) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
					col++;
				}
			}
		}

		// 상부슬래브 각 구간별 철근 지름
		// 각 구간별 이음 간격 개수보다 1개 많다.
		// 그리고, 신축이음 개수만큼 추가된다.
		// 3cycle은 지름 입력받지 않음
		if(nCycle == 0)
		{
			for(n = 0; n < pBri->m_nQtyJigan; n++)
			{
				long nAddExpJoint	= pBri->m_bExpJoint[n+1] ? 1 : 0;
				if(n == pBri->m_nQtyJigan-1)
					nAddExpJoint	= 0;
				long i = 0; for(i = 0; i < pBri->m_nCountJointMainRebar_UpperSlab_Cycle1[n][upper][0] + nAddExpJoint; i++)
				{
					str	= pBri->GetMarkRebarMain(nType, TRUE, nCycle, FALSE, TRUE, _T(""), 0);

					sHelp.Format("%d번째 지간의 %d번째 주철근 지름", n+1, i+1);
					sHelp = sUpdn + sHelp;

					m_Grid.AddCell(row, col, sUpdn);
					m_Grid.AddCell(row+1, col, str);
					m_Grid.AddCell(row+2, col, &pBri->m_pArrDiaUpperSlab[nCycle][upper][0], n, i, nFormat, nLine, sHelp, CELL_TYPE_DIA);
					if(!bUpper) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
					col++;
				}

				if(n == pBri->m_nQtyJigan-1)
				{
					str	= pBri->GetMarkRebarMain(nType, TRUE, nCycle, FALSE, TRUE, _T(""), 0);

					sHelp.Format("%d번째 지간의 %d번째 주철근 지름", n+1, i+1);
					sHelp = sUpdn + sHelp;

					m_Grid.AddCell(row, col, sUpdn);
					m_Grid.AddCell(row+1, col, str);
					m_Grid.AddCell(row+2, col, &pBri->m_pArrDiaUpperSlab[nCycle][upper][0], n, i, nFormat, nLine, sHelp, CELL_TYPE_DIA);
					if(!bUpper) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
					col++;
				}
			}
		}
	}
}

// cycle 1, 3 입력(시종점측 벽체 외측만)
void CInputRebarMain::SetGridData_OutWall(long nCycle)
{
	if(!(nCycle == 0 || nCycle == 2)) return;
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 2, 0);
	m_Grid.SetCellSize(20, -1);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long row		= 0, col = 0;
	CString str		= _T("");
	CString sHelp	= _T("");
	CString sStt	= _T("");
	CString sInner	= _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;
	long nCountJoint	= 0;

	// 외측벽체 시,종점 이음 간격/철근지름
	for(long stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		sStt		= bStt ? "시점측 벽체" : "종점측 벽체";
		long nNum		= 1;
		// 전면 배면
		for(long inner = 0; inner < 2; inner++)
		{
			BOOL bInner	= inner == iINNER;
			long nType	= bStt ? (bInner ? MARK_STTWALL_INNER : MARK_STTWALL_OUTTER) : (bInner ? MARK_ENDWALL_INNER : MARK_ENDWALL_OUTTER);
			sInner	= bInner ? "전면 " : "배면 ";

			pBri->GetMarkRebarMain(nType, TRUE, nCycle, TRUE, TRUE, _T(""), 0);

			long nSttCol = col;

			// 외측벽체 시종점 이음 간격
			nCountJoint	= pBri->m_nCountJointMainRebar_OutWall_Cycle1[stt][inner][0];
			long i = 0; for(i = 0; i < nCountJoint; i++)
			{
				if(stt == iUPPER) str.Format("SD%d", nNum++);
				else str.Format("ED%d", nNum++);

				sHelp.Format("%d번째 이음 까지의 간격", i+1);
				sHelp = sStt + sInner + sHelp;

				m_Grid.AddCell(row, col, sStt+"("+sInner+")");
				m_Grid.AddCell(row+1, col, str);
				m_Grid.AddCell(row+2, col, &pBri->m_pArrDistOutWall[nCycle][stt][inner][0], i, nFormat, nLine, sHelp); 
				m_Grid.SetCellDec(CELL_DEC_0);
				m_Grid.SetCellPrintZero(TRUE);
				if(!bStt) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
				col++;
			}

			// 외측벽체 시종점 철근 지름
			// 3cycle은 지름 입력 받지 않음
			if(nCycle == 0)
			{
				long nIndex = 0;
				nCountJoint	= bInner ? nCountJoint + 1 : nCountJoint;
				for(i = 0; i < nCountJoint; i++)
				{
					// Box형의 외측철근 첫번째 이음은 하부슬래브 하면철근이다.
					if(pBri->IsBoxType() && bInner == FALSE && i == 0)
						continue;

					str	= pBri->GetMarkRebarMain(nType, TRUE, nCycle, FALSE, TRUE, _T(""), 0);

					sHelp.Format("%d번째 철근 지름", nIndex+1);
					sHelp = sStt + sHelp;


					m_Grid.AddCell(row, col, sStt+"("+sInner+")");
					m_Grid.AddCell(row+1, col, str);
					m_Grid.AddCell(row+2, col, &pBri->m_pArrDiaOutWall[nCycle][stt][inner][0], i, nFormat, nLine, sHelp, CELL_TYPE_DIA);
					if(!bStt)
					{
						m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
						m_Grid.SetLastCellFy(pBri->GetValueFy(ePartOutWallEnd));
					}
					col++;
					nIndex++;
				}
			}

			m_Grid.SetColumnCount(col);
			if(col - nSttCol < 2)
			{
				m_Grid.SetColumnWidth(col -1, 120);
			}
			else
			{
				long n = 0; for(n = nSttCol; n < col; n++)
				{
					m_Grid.SetColumnWidth(n, 60);
				}
			}
		}
	}
}

void CInputRebarMain::OnButtonRebarjoint() 
{
	// TODO: Add your control notification handler code here
	CRebarJoint1CycleDlg dlg(m_pIn->GetRcBridgeRebar(), m_pIn->m_pARcBridgeDataStd, m_nTabCycle);
	if(dlg.DoModal() == IDOK)
	{
		// 이음 개수 변경시 지름이나 거리에 대해서 array개수를 정확하게 맞춰 줘야 한다. ////////////////////
		// 물론, 이음이 모자랄 경우 gridmaneger가 array를 추가하지만, 남을경우 제거를 해주지 않으므로, 
		// 지름이나 거리 가져올때 -1 옵션에 의해 마지막걸 가지고 오면 잘못된 값을 가져올 수 있다.
		CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
		if(!pBri) return;
		long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			pBri->SetDataDefaultRebarInfoCycle_Std(-1, -1, TRUE, dan, FALSE);
		//////////////////////////////////////////////////////////////////////////////////////////////////
		

		SetGridData();
		SetDataInit();
		DrawInputDomyunView(FALSE);

		// 상하행선 분리 적용
		m_pIn->m_pARcBridgeDataStd->CopyToCoupleBridge_RebarMain(m_pIn->m_nCurrentBridge);
	}
}

void CInputRebarMain::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_nTab	= m_Tab.GetCurSel();
	m_pIn->m_nLastTab_RebarMain	= m_nTab;
	SetGridData();
	SetDataInit();
	SetControl();
	DrawInputDomyunView(FALSE);
	*pResult = 0;
}

void CInputRebarMain::OnButtonRebardan() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	CRebarDanDlg dlg(m_pIn->GetRcBridgeRebar(), m_pIn->m_pARcBridgeDataStd, m_nTabCycle);
	if(m_nTab == m_nTabUpperSlab) dlg.m_nTypeTab	= 0;
	else if(m_nTab == m_nTabLowerSlab) dlg.m_nTypeTab	= 1;
	else if(m_nTab == m_nTabOutWall) dlg.m_nTypeTab	= (m_nTabLowerSlab > -1)? 2 : 1;
	else if(m_nTab == m_nTabInWall) dlg.m_nTypeTab	= (m_nTabLowerSlab > -1)? 3 : 2;
	else if(m_nTab == m_nTabFooting) dlg.m_nTypeTab	= 3;
	else dlg.m_nTypeTab	= 0;

	if(dlg.DoModal() == IDOK)
	{
		SetControl();
		SetGridData();
		SetDataInit();
		DrawInputDomyunView(FALSE);

		// 상하행선 분리 적용
		m_pIn->m_pARcBridgeDataStd->CopyToCoupleBridge_RebarMain(m_pIn->m_nCurrentBridge);
	}
}

void CInputRebarMain::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	
	*pResult = 0;
}

void CInputRebarMain::OnCheckAllmark() 
{
	// TODO: Add your control notification handler code here
	DrawInputDomyunView(FALSE);
}

// cycle1과 3
void CInputRebarMain::SetGridDataCycle13_Footing(long nCycle)
{
	if(!(nCycle == 0 || nCycle == 2)) return;

	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 1, 1);
	m_Grid.SetCellSize(20, -1);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	long nCountFooting	= pBri->GetCountJijum();

	long row		= 0, col = 0;
	CString str		= _T("");
	CString sHelp	= _T("");
	CString sUpdn	= _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;
	long nIdxWall	= 0;

	// 타이틀 생성 L1, R1, L2, R2
	CFootingApp cFooting;
	cFooting.GetMarkRebarMain(MARK_UPPER, TRUE, nCycle, TRUE, TRUE, _T(""), 0);
	m_Grid.AddCell(row, col++, "구분");
	m_Grid.AddCell(row, col++, "L1");
	m_Grid.AddCell(row, col++, "L2");	// slope일때만 있음
	m_Grid.AddCell(row, col++, "L2'");	// slope일때만 있음
	m_Grid.AddCell(row, col++, cFooting.GetMarkRebarMain(MARK_UPPER, TRUE, nCycle, FALSE, TRUE, _T(""), 0));
	cFooting.GetMarkRebarMain(MARK_LOWER, TRUE, nCycle, TRUE, TRUE, _T(""), 0);
	m_Grid.AddCell(row, col++, "L3");
	m_Grid.AddCell(row, col++, "L4");	// slope일때만 있음
	m_Grid.AddCell(row, col++, cFooting.GetMarkRebarMain(MARK_LOWER, TRUE, nCycle, FALSE, TRUE, _T(""), 0));

	long footing = 0; for(footing = 0; footing < nCountFooting; footing++)
	{
		double dFy(0);
		CFootingApp *pFooting = NULL;
		if(footing == 0) 
		{
			if(pBri->IsFooting(TRUE)) pFooting = &pBri->m_footingStt;
			str	= "시점측";

			dFy = pBri->GetValueFy(ePartFootStt);
		}
		else if(footing == nCountFooting-1)
		{
			if(pBri->IsFooting(FALSE)) pFooting = &pBri->m_footingEnd;
			str	= "종점측";

			dFy = pBri->GetValueFy(ePartFootEnd);
		}
		else
		{
			CWallApp *pWall	= pBri->GetInWall(nIdxWall++);
			pFooting		= &pWall->m_footing;
			str.Format("중간지점%d", nIdxWall);

			dFy = pBri->GetValueFy(ePartInFoot);
		}

		
		if(!pFooting) continue;
		if(!pFooting->m_bIs) continue;
		long nType	= pFooting->GetTypeByInputValue();

		BOOL bExt	= nType == FOOTING_TYPE_LEFTSLOPE || nType == FOOTING_TYPE_RIGHTSLOPE;
		BOOL bExt2	= nType == FOOTING_TYPE_BASIC_LEFTSLOPE || nType == FOOTING_TYPE_BASIC_RIGHTSLOPE;

		row++;
		col	= 0;

		m_Grid.AddCell(row, col++, str);
		for(long upper = 0; upper < 2; upper++)
		{
			BOOL bUpper	= upper == iUPPER;
			// 세로길이1
			m_Grid.AddCell(row, col++, &pFooting->m_dDistMainRebarVert[nCycle][upper], nFormat, nLine, sHelp); 
			m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.SetCellPrintZero(TRUE);

			// 세로길이2
			m_Grid.AddCell(row, col++, &pFooting->m_dDistMainRebarVertExt[nCycle][upper], nFormat, bExt || bExt2? nLine : CELL_READONLY, sHelp); 
			m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.SetCellPrintZero(TRUE);

			// 가로길이2(경사2타입에만 입력받음)
			if(bUpper)
			{
				m_Grid.AddCell(row, col++, &pFooting->m_dDistMainRebarHorExt[nCycle], nFormat, bExt2 ? nLine : CELL_READONLY, sHelp); 
				m_Grid.SetCellDec(CELL_DEC_0);
				m_Grid.SetCellPrintZero(TRUE);
			}

			// 지름
			m_Grid.AddCell(row, col++, &pFooting->m_setRebarDan[upper].m_dDia[nCycle][0], nFormat, nLine, sHelp, CELL_TYPE_DIA);
			m_Grid.SetLastCellFy(dFy);
		}
	}

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(0, 100);
}

void CInputRebarMain::OnSelchangeComboCtc() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	CHgBaseConcStd baseConc;

	// 컨트롤 값 저장
	pBri->m_dCTC_MainRebar		= baseConc.GetValueRebarCTC(m_comboCTC.GetCurSel());

	// CTC변경시 기초에 대해서는 주철근 간격 권고안 해줌
	long nCountFooting	= pBri->GetCountFooting();
	long footing = 0; for(footing = 0; footing < nCountFooting; footing++)
	{
		CFootingApp *pFooting	= pBri->GetFooting(footing);
		if(pFooting)
		{
			pFooting->m_dCTC_MainRebar		= pBri->m_dCTC_MainRebar;
			for(long upper = 0; upper < 2; upper++)
			{
				long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
				{
					pFooting->SetDataDefaultRebarMain_Side(upper==iUPPER, dan);
					pFooting->SetDataDefaultRebarShear_Front(FALSE);
				}
			}
		}
	}
}

// 철근에 대한 동기화설정
void CInputRebarMain::SyncRebar()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	pBri->SyncCycleMainRebar(0, 2);

	//SetGridData();	// 요거를 하면 포커스가 사라짐.
	SetDataInit();
}

long CInputRebarMain::GetCurCycle()
{
	long nCycle	= 0;
	if(m_nTabCycle == m_nTabCycle1) nCycle	= 0;
	else if(m_nTabCycle == m_nTabCycle3) nCycle	= 2;

	return nCycle;
}

// 3cycle 배근 방법
void CInputRebarMain::OnButtonTypecycle3() 
{
	// TODO: Add your control notification handler code here
	CTypeCycle3Dlg dlg(m_pIn->GetRcBridgeRebar(), m_pIn->m_pARcBridgeDataStd);
	if(dlg.DoModal() == IDOK)
	{
		SetGridData();
		SetDataInit();
		DrawInputDomyunView(FALSE);

		// 상하행선 분리 적용
		m_pIn->m_pARcBridgeDataStd->CopyToCoupleBridge_RebarMain(m_pIn->m_nCurrentBridge);
	}
}

// 현재 입력중인 푸팅리턴.
// 지점을 리턴함.
long CInputRebarMain::GetIdxJijumCurFooting()
{
	long nIdx	= -1;
	if(m_nTab == m_nTabFooting)
	{
		if(m_CellID.row > 0)
			nIdx	= m_CellID.row - 1;
	}

	return nIdx;
}

void CInputRebarMain::SetGridData_InWall(long nCycle)
{
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 2, 0);
	m_Grid.SetCellSize(20, -1);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long row		= 0, col = 0;
	CString str		= _T("");
	CString sHelp	= _T("");
	CString sWall	= _T("");
	CString sLeft	= _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;
	long nNum		= 1;
	long nCountJoint	= 0;

	long nCountInWall	= pBri->GetCountInWall();
	long nType			= MARK_LEFT;

	long wall = 0; for(wall = 0; wall < nCountInWall; wall++)
	{
		CWallApp *pWall	= pBri->GetInWall(wall);
		if(!pWall) continue;
		if(pWall->m_bTopIsColumn) continue;

		BOOL bNotInput = (pBri->IsArchType() && wall+1 == pBri->GetJijumConnectRibAndSlab())? TRUE : FALSE;

		sWall.Format("중간지점%d", wall+1);
		nNum		= 1;
		
		// 좌우측
		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT;
			sLeft	= bLeft ? "좌측" : "우측";
			nType	= bLeft ? MARK_LEFT : MARK_RIGHT;

			pWall->GetMarkRebarMain(nType, TRUE, nCycle, TRUE, TRUE, _T(""), 0);

			long nSttCol = col;

			// 이음 간격
			nCountJoint	= pWall->m_nCountJointMainRebar_Cycle1[left][0];
			long i = 0; for(i = 0; i < nCountJoint; i++)
			{
				str.Format("WD%d", nNum++);
				
				sHelp.Format(" %d번째 이음 까지의 간격", i+1);
				sHelp = sWall + sLeft + sHelp;

				m_Grid.AddCell(row, col, sWall+"("+sLeft+")");
				m_Grid.AddCell(row+1, col, str);
				m_Grid.AddCell(row+2, col, &pWall->m_dArrDistRC[nCycle][left][0], i, nFormat, nLine, sHelp); 
				m_Grid.SetCellDec(CELL_DEC_0);
				m_Grid.SetCellPrintZero(TRUE); 
				if(!bLeft) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
				col++;
			}

			// 외측벽체 시종점 철근 지름
			// 3,4cycle은 지름 입력 받지 않음
			if(nCycle == 0 || nCycle == 1)
			{
// 				nCountJoint	= nCountJoint;
				for(i = 0; i < nCountJoint+1; i++)	// 지름은 하나 더 입력받음
				{
					str	= pWall->GetMarkRebarMain(nType, TRUE, nCycle, FALSE, TRUE, _T(""), 0);

					sHelp.Format(" %d번째 철근 지름", i+1);
					sHelp = sWall + sHelp;


					m_Grid.AddCell(row, col, sWall+"("+sLeft+")");
					m_Grid.AddCell(row+1, col, str);
					if(bNotInput)
						m_Grid.AddCell(row+2, col, _T(""), nFormat, CELL_READONLY);
					else
						m_Grid.AddCell(row+2, col, &pWall->m_dArrDiaRC[nCycle][left][0], i, nFormat, nLine, sHelp, CELL_TYPE_DIA); 
					m_Grid.SetCellDec(CELL_DEC_0);
					m_Grid.SetCellPrintZero(TRUE);
					if(!bLeft) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
					col++;
				}
			}

			m_Grid.SetColumnCount(col);
			if(col - nSttCol < 2)
			{
				m_Grid.SetColumnWidth(col -1, 120);
			}
			else
			{
				long n = 0; for(n = nSttCol; n < col; n++)
				{
					m_Grid.SetColumnWidth(n, 60);
				}
			}
		}
	}
}

void CInputRebarMain::SetGridData_ArchRib(long nCycle)
{
	long nDan	= m_comboDan.GetCurSel();

	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 2, 0);
	m_Grid.SetCellSize(20, 50);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long row		= 0, col = 0;
	CString str		= _T("");
	CString sHelp	= _T("");
	CString sUpper	= _T("");
	CString sRib	= _T("아치리브");
	
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;
//	long nNum		= 1;
// 	long nCountJoint	= 0;
// 	long nType			= MARK_ARCHRIB_UPPER;
	
	// 상하면
	for(long upper = 0; upper < 2; upper++)
	{
		BOOL bUpper	= upper == iUPPER;
		sUpper	= bUpper ? "배선" : "복선";
		long nType	= bUpper ? MARK_ARCHRIB_UPPER : MARK_ARCHRIB_LOWER;

		long nNum		= 1;
		pBri->GetMarkRebarMain(nType, TRUE, nCycle, TRUE, TRUE, _T(""), 0);

		// 이음 간격
		long nCountJoint	= pBri->m_nCountJointMainRebar_ArchRib_Cycle1[upper][nDan];
		long i = 0; for(i = 0; i < nCountJoint; i++)
		{
			if(bUpper)
				str.Format("RT%d", nNum++);
			else
				str.Format("RB%d", nNum++);

						
			sHelp.Format(" %d번째 이음 까지의 간격", i+1);
			sHelp = sRib + sUpper + sHelp;

			m_Grid.AddCell(row, col, sRib+"("+sUpper+")");
			m_Grid.AddCell(row+1, col, str);
			m_Grid.AddCell(row+2, col, &pBri->m_dArrDistRib[nCycle][upper][nDan], i, nFormat, nLine, sHelp); 
			m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.SetCellPrintZero(TRUE); 
			if(!bUpper) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
			col++;
		}

		// 철근 지름
		// 3,4cycle은 지름 입력 받지 않음
		if(nCycle == 0 || nCycle == 1)
		{
// 			nCountJoint	= nCountJoint;
			for(i = 0; i < nCountJoint+1; i++)	// 지름은 하나 더 입력받음
			{
				str	= pBri->GetMarkRebarMain(nType, TRUE, nCycle, FALSE, TRUE, _T(""), 0);

				sHelp.Format(" %d번째 철근 지름", i+1);
				sHelp = sRib + sHelp;


				m_Grid.AddCell(row, col, sRib+"("+sUpper+")");
				m_Grid.AddCell(row+1, col, str);
				m_Grid.AddCell(row+2, col, &pBri->m_dArrDiaRib[nCycle][upper][nDan], i, nFormat, nLine, sHelp, CELL_TYPE_DIA); 
				m_Grid.SetCellDec(CELL_DEC_0);
				m_Grid.SetCellPrintZero(TRUE);
				if(!bUpper) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
				if(nDan > 0)
				{
					m_Grid.AddCell(row+3, col, &pBri->m_dArrExistRib[nCycle][upper][nDan], i, nFormat, nLine, "", CELL_TYPE_CHECK);
				}
				col++;
			}
		}
	}
}

void CInputRebarMain::OnCheckBmd() 
{
	// TODO: Add your control notification handler code here
	DrawInputDomyunView(FALSE);
}

void CInputRebarMain::OnButtonExamine() 
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	BOOL bFemNormal = pBri->m_FemNormal.IsOutputLoaded();
	BOOL bFemEQ     = pBri->IsRcSlab() ? TRUE : (pBri->GetFemManage(COMBO_EARTHQUAKE)->IsOutputLoaded());

	if(pBri->GetCountFooting()>0 && pBri->IsFirstDefault(m_pIn->m_dlgRebarColumn.GetIDD()))
	{
		AfxMessageBox("보기둥 배근후 설계검토를 하시기 바랍니다.");
		return;
	}

	if(!bFemNormal || (pBri->GetTypeDesingEQ() == 1 && !bFemEQ))
	{
		AfxMessageBox("모델링 해석을 수행후에 설계검토를 하시기 바랍니다.");
		return;
	}

	if(m_pDlgCheck->GetSafeHwnd() && m_pDlgCheck->IsWindowVisible()) m_pDlgCheck->DestroyWindow();
	m_pDlgCheck->SetBriAndContent(pBri,IDD_REBAR_MAIN,0);

	m_pDlgCheck->ShowAndCheck(this);	
	CString str = _T("");
	str.Format("주철근 검토");
	m_pDlgCheck->SetWindowText(str);	
}

// 시종점부 동일 적용.
void CInputRebarMain::OnButtonSamesttend2() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	ApplySameShapeSttAndEnd();
	SetGridData();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CInputRebarMain::OnButtonSameinwall2() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	long nIdxInWallStd	= GetIdxJijumCurFooting();
	ApplySameShapeInWall(nIdxInWallStd);
	SetGridData();
	SetDataInit();
	DrawInputDomyunView(FALSE);	
}


// 내측벽체 형상 동일 적용
// nIdxInWallStd : 기준이 되는 내측 벽체
void CInputRebarMain::ApplySameShapeInWall(long nIdxInWallStd)
{
	CString str	= _T("");
	str.Format("모든 중간지점 기초의 주철근 배근을 %d지점과 동일하게 적용하시겠습니까?", nIdxInWallStd+1);
	if(AfxMessageBox(str, MB_YESNO) != IDYES) return;

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	
	long nCountInWall	= pBri->GetCountInWall();
	if(nCountInWall <= 1) return;

	CWallApp *pWallOrg	= pBri->GetInWall(nIdxInWallStd);
	if(!pWallOrg) return;

	long wall = 0; for(wall = 0; wall < nCountInWall; wall++)
	{
		if(wall == nIdxInWallStd) continue;

		CWallApp *pWall	= pBri->GetInWall(wall);

		for(long upper = 0; upper < 2; upper++)
		{
			long nCycle	= 0;
			pWall->m_footing.m_dDistMainRebarVert[nCycle][upper]	= pWallOrg->m_footing.m_dDistMainRebarVert[nCycle][upper];
			pWall->m_footing.m_dDistMainRebarVertExt[nCycle][upper]	= pWallOrg->m_footing.m_dDistMainRebarVertExt[nCycle][upper];
			pWall->m_footing.m_dDistMainRebarHor[nCycle][upper]		= pWallOrg->m_footing.m_dDistMainRebarHor[nCycle][upper];
			pWall->m_footing.m_setRebarDan[upper].m_dDia[nCycle][0]	= pWallOrg->m_footing.m_setRebarDan[upper].m_dDia[nCycle][0];

			nCycle	= 2;
			pWall->m_footing.m_dDistMainRebarVert[nCycle][upper]	= pWallOrg->m_footing.m_dDistMainRebarVert[nCycle][upper];
			pWall->m_footing.m_dDistMainRebarVertExt[nCycle][upper]	= pWallOrg->m_footing.m_dDistMainRebarVertExt[nCycle][upper];
			pWall->m_footing.m_dDistMainRebarHor[nCycle][upper]		= pWallOrg->m_footing.m_dDistMainRebarHor[nCycle][upper];
			pWall->m_footing.m_setRebarDan[upper].m_dDia[nCycle][0]	= pWallOrg->m_footing.m_setRebarDan[upper].m_dDia[nCycle][0];
		}
	}
		
	SetGridData();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

// 시점과 종점의 단면 형상을 같게 만듬.
void CInputRebarMain::ApplySameShapeSttAndEnd()
{
	if(AfxMessageBox("종점측 기초 주철근 배근을 시점측과 동일하게 적용하시겠습니까?", MB_YESNO) != IDYES) return;

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	for(long upper = 0; upper < 2; upper++)
	{
		long nCycle	= 0;
		pBri->m_footingEnd.m_dDistMainRebarVert[nCycle][upper]	= pBri->m_footingStt.m_dDistMainRebarVert[nCycle][upper];
		pBri->m_footingEnd.m_dDistMainRebarVertExt[nCycle][upper]	= pBri->m_footingStt.m_dDistMainRebarVertExt[nCycle][upper];
		pBri->m_footingEnd.m_dDistMainRebarHor[nCycle][upper]		= pBri->m_footingStt.m_dDistMainRebarHor[nCycle][upper];
		pBri->m_footingEnd.m_setRebarDan[upper].m_dDia[nCycle][0]	= pBri->m_footingStt.m_setRebarDan[upper].m_dDia[nCycle][0];

		nCycle	= 2;
		pBri->m_footingEnd.m_dDistMainRebarVert[nCycle][upper]	= pBri->m_footingStt.m_dDistMainRebarVert[nCycle][upper];
		pBri->m_footingEnd.m_dDistMainRebarVertExt[nCycle][upper]	= pBri->m_footingStt.m_dDistMainRebarVertExt[nCycle][upper];
		pBri->m_footingEnd.m_dDistMainRebarHor[nCycle][upper]		= pBri->m_footingStt.m_dDistMainRebarHor[nCycle][upper];
		pBri->m_footingEnd.m_setRebarDan[upper].m_dDia[nCycle][0]	= pBri->m_footingStt.m_setRebarDan[upper].m_dDia[nCycle][0];
	}

	SetGridData();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CInputRebarMain::OnCheckJoint() 
{
	// TODO: Add your control notification handler code here

	DrawInputDomyunView(FALSE);
}

void CInputRebarMain::OnButtonOptimize() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	BOOL bFemNormal = pBri->m_FemNormal.IsOutputLoaded();
	BOOL bFemEQ     = pBri->IsRcSlab() ? TRUE : (pBri->GetFemManage(COMBO_EARTHQUAKE)->IsOutputLoaded());
	
	if(!bFemNormal || (pBri->GetTypeDesingEQ() == 1 && !bFemEQ))
	{
		AfxMessageBox("모델링 해석을 수행후에 설계검토를 하시기 바랍니다.");
		return;
	}

	COptimizeRebar dlg(pBri);
	if(dlg.DoModal()==IDOK)
	{
		if(m_pProgress)
		{
			delete m_pProgress;
			m_pProgress = NULL;
		}

		m_pProgress	= new CDlgProgress;
		if(!m_pProgress->GetSafeHwnd())
		{
			m_pProgress->Create(IDD_DIALOG_HGPROGRESS, this);
		}
		m_pProgress->ShowWindow(SW_SHOW);
		m_pProgress->CenterWindow();
		m_pProgress->InitProgressData();

		m_pProgress->SetProgressPos("최적설계 준비중............", 5);

		pBri->SetConcData(TRUE,TRUE,TRUE,FALSE,TRUE);
		CARcBridgeCalcStd *pCalc = new CARcBridgeCalcStd(pBri);
		pCalc->CalculateAll();
		pCalc->m_pFooting->CalculateAll();
		delete pCalc;

		m_pProgress->SetProgressPos("상부슬래브 최적화............", 25);
		OptimizeRebarSlab();

		m_pProgress->SetProgressPos("외측벽체 최적화............", 40);
		OptimizeRebarOutWall();

		m_pProgress->SetProgressPos("중간벽체 최적화............", 55);
		OptimizeRebarMiddleWall();

		if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		{
			m_pProgress->SetProgressPos("아치리브 최적화............", 65);
			OptimizeRebarArchRib();
		}

		m_pProgress->SetProgressPos("기초 최적화............", 70);
		OptimizeRebarFooting();

//		CConcBasic *pConc = pBri->GetConcBasicByTitle(&pBri->m_pArrConcWall, "시점벽체 하단(전면)");

		m_pProgress->SetProgressPos("철근입력사항 변경중............", 85);
		SyncOptimizeRebarMain();
		if(pBri->m_bOptimizeRebarMatch) SetRebarSymmetry();

		pBri->MakeRebarInfoCycleStd(TRUE);
		m_pProgress->SetProgressPos("최적설계 완료............", 100);
		SetGridData();
		SetDataInit();
		
		m_pProgress->ShowWindow(SW_HIDE);

		if(m_pProgress)
		{
			delete m_pProgress;
			m_pProgress = NULL;
		}

		pBri->m_bFirstCalledGetTvVertSection_Std	= FALSE;
		pBri->m_bSecondCalledGetTvVertSection_Std	= FALSE;

		OnButtonExamine();
		DrawInputDomyunView(FALSE);
	}
}

void CInputRebarMain::OptimizeRebarSlab()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	double dCtcMain = pBri->GetCtcRebarMain(TRUE);

	for(long nCase=0; nCase<2; ++nCase)
	{
		// 0 : 극한한계, 1 : 극단상황한계
		if(m_bDesignLsd == FALSE && nCase == 1)
			break;

		ETypeEnvComboLsd eType = nCase == 0 ? eEnvLimit : eEnvExtremeI;
		
		long nSize = pBri->m_pArrConcSlab[eType].GetSize();
		long i=0; for(i=0; i<nSize; i++)
		{
			CConcBasic *pConc = pBri->m_pArrConcSlab[eType].GetAt(i);
			OptimizeBeam(pConc, dCtcMain);
		}
	}
}

void CInputRebarMain::OptimizeRebarOutWall()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	double dCtcMain = pBri->GetCtcRebarMain(FALSE);
	double dCtc = 0;

	for(long nCase=0; nCase<2; ++nCase)
	{
		// 0 : 극한한계, 1 : 극단상황한계
		if(m_bDesignLsd == FALSE && nCase == 1)
			break;

		ETypeEnvComboLsd eType = nCase == 0 ? eEnvLimit : eEnvExtremeI;

		long nSize = pBri->m_pArrConcWall[eType].GetSize();
		long i=0; for(i=0; i<nSize; i++)
		{
			CConcBasic *pConc = pBri->m_pArrConcWall[eType].GetAt(i);

			if(pConc->m_nTypeSection==SECTION_SINGLE)
			{
				CDPoint vAngleJijum = pBri->GetAngleJijum((i<nSize/2)? 0:pBri->m_nQtyJigan);
				if(pBri->m_nTypeStandardCTCRebarMain == 0)		// 상부 슬래브 기준
				{
					dCtc = dCtcMain/vAngleJijum.y;
				}
				else
				{
					CDPoint vAng1 = pBri->GetAngleJijum(0);
					double dCtcTemp = dCtcMain*vAng1.y;
					dCtc = dCtcTemp/vAngleJijum.y;
				}
				OptimizeBeam(pConc, dCtc);
			}
			else if(pConc->m_nTypeSection==SECTION_WALL)
			{
				OptimizeWall(pConc);
			}
		}
	}
}

void CInputRebarMain::OptimizeRebarMiddleWall()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	
	for(long nCase=0; nCase<2; ++nCase)
	{
		// 0 : 극한한계, 1 : 극단상황한계
		if(m_bDesignLsd == FALSE && nCase == 1)
			break;

		ETypeEnvComboLsd eType = nCase == 0 ? eEnvLimit : eEnvExtremeI;

		long nSize = pBri->m_pArrConcWall[eType].GetSize();
		long i=0; for(i=0; i<nSize; i++)
		{
			CConcBasic *pConc = pBri->m_pArrConcWall[eType].GetAt(i);
			if(pConc->m_nTypeSection==SECTION_WALL)
			{
				OptimizeWall(pConc);
			}
		}
	}
}

void CInputRebarMain::OptimizeRebarFooting()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	for(long nCase=0; nCase<2; ++nCase)
	{
		// 0 : 극한한계, 1 : 극단상황한계
		if(m_bDesignLsd == FALSE && nCase == 1)
			break;

		ETypeEnvComboLsd eType = nCase == 0 ? eEnvLimit : eEnvExtremeI;

		long nSize = pBri->m_pArrConcFoot[eType].GetSize();
		long i=0; for(i=0; i<nSize; i++)
		{
			CConcBasic *pConc = pBri->m_pArrConcFoot[eType].GetAt(i);
			double dCtcTen = (pConc->m_RbT1C_B[0] > 0)?  Round(pConc->m_Bo / pConc->m_RbT1C_B[0], 0) : pBri->GetCtcRebarMain(FALSE);
			OptimizeBeam(pConc, dCtcTen, FALSE, TRUE);
		}

		//////////////////////////////////////////////////////////////////////////
		long nSizeRev = pBri->m_pArrConcFootRev[eType].GetSize();
		for(i=0; i<nSizeRev; i++)
		{
			CConcBasic *pConc = pBri->m_pArrConcFootRev[eType].GetAt(i);
			double dCtcCom = (pConc->m_RbC1C_B[0] > 0)? Round(pConc->m_Bo / pConc->m_RbC1C_B[0], 0) : pBri->GetCtcRebarMain(FALSE);
			OptimizeBeam(pConc, dCtcCom, TRUE, TRUE);
		}
	}
}

// 최적화 순서
// 1. 모멘트에 대하여 안전율을 확보하도록 직경을 조정
//    -> 안전율이 미달되거나 철근비 N.G인 경우 조정
//    -> 최소직경에서 시작해서 최대직경까지... (2단 미적용시는 최대직경에서 종료)
//    -> 2단 및 2Cycle분리 옵션 적용
//    -> 2Cycle분리시 1Cycle철근의 직경이 2Cycle보다 크게 설정
// 2. 대칭 적용시 매치되는 단면에서 큰 직경으로 설정 => 추후.....
void CInputRebarMain::OptimizeBeam(CConcBasic *pConc, double dCtcMain, BOOL bRev, BOOL bFooting)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	double DiaArr[9] = {10,13,16,19,22,25,29,32,35};
	long nIdx1Cycle  = GetIdxDia(pBri->m_dOptimizeDiaMinimum);
	long nIdx2Cycle  = GetIdxDia(pBri->m_dOptimizeDiaMinimum);
	long nIdxMaxDia  = GetIdxDia(pBri->m_dOptimizeDiaMaximum);

	double dCtcTen = dCtcMain;
	//////////////////////////////////////////////////////////////////////////
	// STEP 1-1 : 1단 배치로 체크
	// 철근직경을 초기화
	long j=0; for(j=0; j<REBAR_MAX_LAYER; j++)
	{
		pConc->m_RbT1C_DIA[j] = (j==0) ? pBri->m_dOptimizeDiaMinimum : 0;
		pConc->m_RbT2C_DIA[j] = (j==0) ? pBri->m_dOptimizeDiaMinimum : 0;
		pConc->m_RbT1C_B[j]   = (j==0) ? pConc->m_Bo/dCtcTen : 0;
		pConc->m_RbT2C_B[j]   = (j==0) ? pConc->m_Bo/dCtcTen : 0;
	}
	double SF  = 0;
	long nRate = 0;
	double W	= 0;
	double Wa	= 0;
	BOOL bConsider2Cycle = IsConsider2Cycle(pConc, bRev); // 벽체중앙배면,벽체하단전면.....
	while(1)
	{
		if(bConsider2Cycle==FALSE) Empty2Cycle(pConc);
		if(CheckBeamForOptimize(pConc)) break;
		
		if(pBri->m_bOptimizeRebarSplit)
		{
			// 1CYCLE을 먼저 조정하고 만족하면 break
			pConc->m_RbT1C_DIA[0] = DiaArr[++nIdx1Cycle];
			if(bConsider2Cycle==FALSE) Empty2Cycle(pConc);
			if(CheckBeamForOptimize(pConc)) break;

			pConc->m_RbT2C_DIA[0] = DiaArr[++nIdx2Cycle];
			if(bConsider2Cycle==FALSE) Empty2Cycle(pConc);
			if(CheckBeamForOptimize(pConc)) break;
		}
		else
		{
			nIdx1Cycle++;
			pConc->m_RbT1C_DIA[0] = DiaArr[nIdx1Cycle];
			pConc->m_RbT2C_DIA[0] = DiaArr[nIdx1Cycle];
			if(bConsider2Cycle==FALSE) Empty2Cycle(pConc);
			if(CheckBeamForOptimize(pConc)) break;
		}

		if(nIdx1Cycle==nIdxMaxDia) break;
		if(nIdx2Cycle==nIdxMaxDia) break;
	}

	if(CheckBeamForOptimize(pConc)) return;

	//////////////////////////////////////////////////////////////////////////
	// STEP 1-2 : 2단 배치로 체크
	// 철근직경을 초기화
	if(pBri->m_bOptimizeRebar2Dan)
	{
		for(j=0; j<REBAR_MAX_LAYER; j++)
		{
			pConc->m_RbT1C_DIA[j] = pConc->m_RbT2C_DIA[j] = 0;
			pConc->m_RbT1C_B[j]   = pConc->m_RbT2C_B[j]   = 0;
		}
		nIdx1Cycle = GetIdxDia(pBri->m_dOptimizeDiaMinimum);
		nIdx2Cycle = GetIdxDia(pBri->m_dOptimizeDiaMinimum);

		while(1)
		{
			// STEP 1-2-1
			pConc->m_RbT1C_DIA[0] = DiaArr[nIdx1Cycle];
			pConc->m_RbT2C_DIA[0] = DiaArr[nIdx2Cycle];
			pConc->m_RbT1C_DIA[1] = DiaArr[nIdx1Cycle];
			pConc->m_RbT2C_DIA[1] = 0;
			pConc->m_RbT1C_B[0]   = pConc->m_Bo/dCtcTen;
			pConc->m_RbT2C_B[0]   = pConc->m_Bo/dCtcTen;
			pConc->m_RbT1C_B[1]   = pConc->m_Bo/dCtcTen;
			pConc->m_RbT2C_B[1]   = 0;

			if(bConsider2Cycle==FALSE) Empty2Cycle(pConc);
			if(CheckBeamForOptimize(pConc)) break;

			// STEP 1-2-2
			if(nIdx1Cycle<nIdxMaxDia && pBri->m_bOptimizeRebarSplit)
			{
				// 1CYCLE을 먼저 조정하고 만족하면 break
				pConc->m_RbT1C_DIA[0] = DiaArr[nIdx1Cycle+1];
				if(bConsider2Cycle==FALSE) Empty2Cycle(pConc);
				if(CheckBeamForOptimize(pConc)) break;
			}
			// STEP 1-2-3
			if(nIdx1Cycle<nIdxMaxDia && pBri->m_bOptimizeRebarSplit)
			{
				pConc->m_RbT1C_DIA[0] = DiaArr[nIdx1Cycle+1];
				pConc->m_RbT2C_DIA[0] = DiaArr[nIdx1Cycle+1];
				if(bConsider2Cycle==FALSE) Empty2Cycle(pConc);
				if(CheckBeamForOptimize(pConc)) break;
			}
			// STEP 1-2-4
			pConc->m_RbT1C_DIA[0] = DiaArr[nIdx1Cycle];
			pConc->m_RbT2C_DIA[0] = DiaArr[nIdx2Cycle];
			pConc->m_RbT1C_DIA[1] = DiaArr[nIdx1Cycle];
			pConc->m_RbT2C_DIA[1] = DiaArr[nIdx1Cycle];
			pConc->m_RbT2C_B[1]   = pConc->m_Bo/dCtcTen;
			if(bConsider2Cycle==FALSE) Empty2Cycle(pConc);
			if(CheckBeamForOptimize(pConc)) break;

			if(nIdx1Cycle==nIdxMaxDia) break;
			if(nIdx2Cycle==nIdxMaxDia) break;

			nIdx1Cycle++;
			nIdx2Cycle++;
		}
	}
}
 

// BEAM의 최적화와 동일하며 1,2CYCLE구분은 없음.
void CInputRebarMain::OptimizeWall(CConcBasic *pConc)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	double stdSF = pBri->m_dOptimizeSafeRate/100;
	double DiaArr[9] = {10,13,16,19,22,25,29,32,35};
	long nIdx1Cycle  = GetIdxDia(pBri->m_dOptimizeDiaMinimum);
	long nIdxMaxDia  = GetIdxDia(pBri->m_dOptimizeDiaMaximum);

	//////////////////////////////////////////////////////////////////////////
	// STEP 1-1 : 1단 배치로 체크
	// 철근직경을 초기화
	long j=0; for(j=0; j<REBAR_MAX_LAYER; j++)
	{
		pConc->m_RbT1C_DIA[j] = (j==0) ? pBri->m_dOptimizeDiaMinimum : 0;
		pConc->m_RbT2C_DIA[j] = (j==0) ? pBri->m_dOptimizeDiaMinimum : 0;
		pConc->m_RbT1C_B[j]   = (j==0) ? pConc->m_Bo/pBri->m_dCTC_MainRebar : 0;
		pConc->m_RbT2C_B[j]   = (j==0) ? pConc->m_Bo/pBri->m_dCTC_MainRebar : 0;
	}
	double SF  = 0;
	long nRate = 0;
	while(1)
	{
		CColumn Wall(pConc);
		Wall.CalculateAll(FALSE);
		SF    = Wall.GetSafeFactorAxial();
		nRate = Wall.GetCheckRebarRate();
		if(stdSF<=SF && nRate==1) break;
		
		nIdx1Cycle++;
		pConc->m_RbT1C_DIA[0] = DiaArr[nIdx1Cycle];
		pConc->m_RbT2C_DIA[0] = DiaArr[nIdx1Cycle];
		CColumn Wall2(pConc);
		Wall2.CalculateAll(FALSE);
		SF    = Wall2.GetSafeFactorAxial();
		nRate = Wall2.GetCheckRebarRate();
		if(stdSF<=SF && nRate==1) break;

		if(nIdx1Cycle==nIdxMaxDia) break;
	}
	CColumn Wall(pConc);
	Wall.CalculateAll(FALSE);
	SF    = Wall.GetSafeFactorAxial();
	nRate = Wall.GetCheckRebarRate();
	if(stdSF<=SF && nRate==1) return;

	//////////////////////////////////////////////////////////////////////////
	// STEP 1-2 : 2단 배치로 체크
	// 철근직경을 초기화
	for(j=0; j<REBAR_MAX_LAYER; j++)
	{
		pConc->m_RbT1C_DIA[j] = pConc->m_RbT2C_DIA[j] = 0;
		pConc->m_RbT1C_B[j]   = pConc->m_RbT2C_B[j]   = 0;
	}
	nIdx1Cycle = GetIdxDia(pBri->m_dOptimizeDiaMinimum);
	if(pBri->m_bOptimizeRebar2Dan)
	{
		while(1)
		{
			// STEP 1-2-1
			pConc->m_RbT1C_DIA[0] = DiaArr[nIdx1Cycle];
			pConc->m_RbT2C_DIA[0] = DiaArr[nIdx1Cycle];
			pConc->m_RbT1C_DIA[1] = DiaArr[nIdx1Cycle];
			pConc->m_RbT2C_DIA[1] = DiaArr[nIdx1Cycle];
			pConc->m_RbT1C_B[0]   = pConc->m_Bo/pBri->m_dCTC_MainRebar;
			pConc->m_RbT2C_B[0]   = pConc->m_Bo/pBri->m_dCTC_MainRebar;
			pConc->m_RbT1C_B[1]   = pConc->m_Bo/pBri->m_dCTC_MainRebar;
			pConc->m_RbT2C_B[1]   = pConc->m_Bo/pBri->m_dCTC_MainRebar;

			CColumn Wall(pConc);
			Wall.CalculateAll(FALSE);
			SF    = Wall.GetSafeFactorAxial();
			nRate = Wall.GetCheckRebarRate();
			if(stdSF<=SF && nRate==1) break;

			if(nIdx1Cycle<nIdxMaxDia)
			{
				pConc->m_RbT1C_DIA[0] = DiaArr[nIdx1Cycle+1];
				pConc->m_RbT2C_DIA[0] = DiaArr[nIdx1Cycle+1];
				CColumn Wall(pConc);
				Wall.CalculateAll(FALSE);
				SF    = Wall.GetSafeFactorAxial();
				nRate = Wall.GetCheckRebarRate();
				if(stdSF<=SF && nRate==1) break;
			}

			if(nIdx1Cycle==nIdxMaxDia) break;
			nIdx1Cycle++;
		}
	}
}

long CInputRebarMain::GetIdxDia(double Dia)
{
	long nIdxDia = -1;

	if(Dia==10) nIdxDia = 0;
	if(Dia==13) nIdxDia = 1;
	if(Dia==16) nIdxDia = 2;
	if(Dia==19) nIdxDia = 3;
	if(Dia==22) nIdxDia = 4;
	if(Dia==25) nIdxDia = 5;
	if(Dia==29) nIdxDia = 6;
	if(Dia==32) nIdxDia = 7;
	if(Dia==35) nIdxDia = 8;

	return nIdxDia;
}

void CInputRebarMain::SyncOptimizeRebarMain()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	CConcBasic *pConc		= NULL;

	pBri->SetDataDefaultSetRebarDan(-1);

	long dan(0), jijum(0);
	CString sTitle	= _T("");
	for(long nCase=0; nCase<2; ++nCase)
	{
		// 0 : 극한한계, 1 : 극단상황한계
		if(m_bDesignLsd == FALSE && nCase == 1)
			break;

		ETypeEnvComboLsd eType = nCase == 0 ? eEnvLimit : eEnvExtremeI;

		// 상부 슬래브 상면
		pConc	= pBri->GetConcBasicByTitle(&pBri->m_pArrConcSlab[eType], "좌측단부");
		if(pConc)
		{
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				pBri->SetDiaMainRebarUpperSlab(0, dan, TRUE, 0, 0, pConc->m_RbT1C_DIA[dan], nCase == 1);
				pBri->SetDiaMainRebarUpperSlab(1, dan, TRUE, 0, 0, pConc->m_RbT2C_DIA[dan], nCase == 1);
			}
		}

		pConc	= pBri->GetConcBasicByTitle(&pBri->m_pArrConcSlab[eType], "우측단부");
		if(pConc)
		{
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				pBri->SetDiaMainRebarUpperSlab(0, dan, TRUE, -1, -1, pConc->m_RbT1C_DIA[dan], nCase == 1);
				pBri->SetDiaMainRebarUpperSlab(1, dan, TRUE, -1, -1, pConc->m_RbT2C_DIA[dan], nCase == 1);
			}
		}

		for(jijum = 1; jijum < pBri->m_nQtyJigan; jijum++)
		{
			sTitle.Format("중간지점%d", jijum);
			pConc	= pBri->GetConcBasicByTitle(&pBri->m_pArrConcSlab[eType], sTitle);
			if(pConc)
			{
				for(dan = 0; dan < MAX_REBAR_DAN; dan++)
				{
					pBri->SetDiaMainRebarUpperSlab(0, dan, TRUE, jijum, 0, pConc->m_RbT1C_DIA[dan], nCase == 1);
					pBri->SetDiaMainRebarUpperSlab(1, dan, TRUE, jijum, 0, pConc->m_RbT2C_DIA[dan], nCase == 1);
				}
			}	
		}

		if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		{
			pConc	= pBri->GetConcBasicByTitle(&pBri->m_pArrConcSlab[eType], "아치리브 접합부");
			long nJijum = pBri->GetJijumConnectRibAndSlab();
			if(pConc)
			{
				for(dan = 0; dan < MAX_REBAR_DAN; dan++)
				{
					pBri->SetDiaMainRebarUpperSlab(0, dan, TRUE, nJijum, 0, pConc->m_RbT1C_DIA[dan], nCase == 1);
					pBri->SetDiaMainRebarUpperSlab(1, dan, TRUE, nJijum, 0, pConc->m_RbT2C_DIA[dan], nCase == 1);
				}
			}
		}

		// 상부 슬래브 하면
		for(jijum = 0; jijum < pBri->m_nQtyJigan; jijum++)
		{
			sTitle.Format("중앙부(지간%d)", jijum+1);
			pConc	= pBri->GetConcBasicByTitle(&pBri->m_pArrConcSlab[eType], sTitle);
			if(pConc)
			{
				for(dan = 0; dan < MAX_REBAR_DAN; dan++)
				{
					// 이음이 1개면 철근 2개다 설정해줌
					// 이음이 2개 이상이면 각 지점부에 있는 철근 빼고 전부 설정
					long nStt	= 1;
					long nEnd	= pBri->GetCountJointMainRebarUpperSlab(0, dan, FALSE, jijum)-1;
					if(nEnd < 1)
					{
						nStt	= 0;
						nEnd	= pBri->GetCountJointMainRebarUpperSlab(0, dan, FALSE, jijum);
					}

					long i = 0; for(i = nStt; i <= nEnd; i++)
						pBri->SetDiaMainRebarUpperSlab(0, dan, FALSE, jijum, i, pConc->m_RbT1C_DIA[dan], nCase == 1);

					nStt	= 0;
					nEnd	= pBri->GetCountJointMainRebarUpperSlab(1, dan, FALSE, jijum)-1;
					if(nEnd < 1)
					{
						nStt	= 0;
						nEnd	= pBri->GetCountJointMainRebarUpperSlab(1, dan, FALSE, jijum);
					}
					for(i = nStt; i <= nEnd; i++)
						pBri->SetDiaMainRebarUpperSlab(1, dan, FALSE, jijum, i, pConc->m_RbT2C_DIA[dan], nCase == 1);
				}
			}
		}


		// 외측 벽체
		long stt(0), inner(0);
		BOOL bStt(TRUE), bInner(TRUE);
		CString sStt(""), sInner("");
		for(stt = 0; stt < 2; stt++)
		{
			bStt	= stt == iSTT;
			sStt	= bStt ? "시점벽체" : "종점벽체";
			if(!pBri->IsOutWall(bStt)) continue;

			for(inner = 0; inner < 2; inner++)
			{
				bInner	= inner == iINNER;
				sInner	= bInner ? "전면" : "배면";


				// 벽체 하단
				sTitle.Format("%s 하단(%s)", sStt, sInner);
				pConc	= pBri->GetConcBasicByTitle(&pBri->m_pArrConcWall[eType], sTitle);
				if(pConc)
				{
					for(dan = 0; dan < MAX_REBAR_DAN; dan++)
					{
						pBri->SetDiaMainRebarOutWall(0, dan, bStt, bInner, 0, pConc->m_RbT1C_DIA[dan], nCase == 1);
						pBri->SetDiaMainRebarOutWall(1, dan, bStt, bInner, 0, pConc->m_RbT2C_DIA[dan], nCase == 1);
					}
				}

				if(sInner == "배면")
				{
					// 벽체 상단부
					sTitle.Format("%s 상단부", sStt);
					pConc	= pBri->GetConcBasicByTitle(&pBri->m_pArrConcWall[eType], sTitle);
					if(pConc)
					{
						for(dan = 0; dan < MAX_REBAR_DAN; dan++)
						{
							// 이거는 상부슬래브 시점, 종점 단부와 같다
							// 둘 중 큰거를 사용함
							if(pConc->m_RbT1C_DIA[dan] > pBri->GetDiaMainRebarUpperSlab(0, dan, TRUE, bStt ? 0 : -1, bStt ? 0 : -1))
								pBri->SetDiaMainRebarUpperSlab(0, dan ,TRUE, bStt ? 0 : -1, bStt ? 0 : -1, pConc->m_RbT1C_DIA[dan], nCase == 1);

							if(pConc->m_RbT2C_DIA[dan] > pBri->GetDiaMainRebarUpperSlab(1, dan, TRUE, bStt ? 0 : -1, bStt ? 0 : -1))
								pBri->SetDiaMainRebarUpperSlab(1, dan ,TRUE, bStt ? 0 : -1, bStt ? 0 : -1, pConc->m_RbT2C_DIA[dan], nCase == 1);
						}
					}
				}

				// 벽체 중앙부
				sTitle.Format("%s 중앙(%s)", sStt, sInner); 
				pConc	= pBri->GetConcBasicByTitle(&pBri->m_pArrConcWall[eType], sTitle);
				if(pConc)
				{
					for(dan = 0; dan < MAX_REBAR_DAN; dan++)
					{
						// 1cycle
						long nStt	= 1;
						long nEnd	= pBri->GetCountJointMainRebarOutWall(0, dan, bStt, bInner);
						if(sInner == "배면")
							nEnd --;

						long i = 0; for(i = nStt; i <= nEnd; i++)
						{
							pBri->SetDiaMainRebarOutWall(0, dan, bStt, bInner, i, pConc->m_RbT1C_DIA[dan], nCase == 1);
						}

						// 2cycle
						if(sInner == "전면")
						{
							nStt	= 1;
							nEnd	= pBri->GetCountJointMainRebarOutWall(1, dan, bStt, bInner)-1;

							long i = 0; for(i = nStt; i <= nEnd; i++)
							{
								pBri->SetDiaMainRebarOutWall(1, dan, bStt, bInner, i, pConc->m_RbT2C_DIA[dan], nCase == 1);
							}
						}
					}
				}
			}
		}


		// 내측벽체
		long nCountInWall	= pBri->GetCountInWall();
		long wall(0), left(0);
		for(wall = 0; wall < nCountInWall; wall++)
		{
			CWallApp *pWall	= pBri->GetInWall(wall);
			if(!pWall) continue;
			if(!pWall->m_bIs) continue;

			sTitle.Format("지점%d", wall+2);
			for(left = 0; left < 2; left++)
			{
				BOOL bLeft	= left == iLEFT;

				pConc	= pBri->GetConcBasicByTitle(&pBri->m_pArrConcWall[eType], sTitle);
				if(pConc)
				{
					for(dan = 0; dan < MAX_REBAR_DAN; dan++)
					{
						long nCountJoint	= pWall->m_nCountJointMainRebar_Cycle1[left][0]+1;

						long i = 0; for(i = 0; i < nCountJoint; i++)
						{
							pWall->SetDiaMainRebar(0, dan, bLeft, i, pConc->m_RbT1C_DIA[dan], nCase == 1);
						}

						pWall->SetDiaMainRebar(1, dan, bLeft, i, pConc->m_RbT2C_DIA[dan], nCase == 1);
					}
				}
			}
		}

		// 기초
		long footing(0), upper(0);
		BOOL bUpper(TRUE);
		CString sFront	= _T("");
		CString sTitleRev	= _T("");
		CString sFrontRev	= bUpper ? "앞굽" : "뒷굽";
		CConcBasic *pConcRev	= NULL;
		for(footing = 0; footing <= pBri->m_nQtyJigan; footing++)
		{
			CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(footing);
			if(!pFooting) continue;
			if(!pFooting->m_bIs) continue;

			for(upper = 0; upper < 2; upper++)
			{
				bUpper	= upper == iUPPER;

				// 1cycle
				sFront	= bUpper ? "뒷굽" : "앞굽";
				sFrontRev	= bUpper ? "앞굽" : "뒷굽";
				if(footing == 0)
				{
					sTitle.Format("시점측(%s)", sFront);
					sTitleRev.Format("시점측(%s)", sFrontRev);
				}
				else if(footing == pBri->m_nQtyJigan)
				{
					sTitle.Format("종점측(%s)", sFront);
					sTitleRev.Format("종점측(%s)", sFrontRev);
				}
				else
				{
					sTitle.Format("중간지점%d", footing);
					sTitleRev	= sTitle;
				}
				pConc	= pBri->GetConcBasicByTitle(&pBri->m_pArrConcFoot[eType], sTitle);
				if(pConc)
				{
					for(dan = 0; dan < MAX_REBAR_DAN; dan++)
					{
						double  dDia = nCase == 0 ? 0 : pFooting->GetDiaMainRebar(0, bUpper, dan);
						pFooting->SetDiaMainRebar(0, bUpper, dan, MAX(dDia, pConc->m_RbT1C_DIA[dan]));
					}
				}


				// rev쪽에 직경이 설정되었는지 판단해서
				// 있다면 1cycle과 비교해서 큰걸로 매칭시켜준다.
				pConcRev	= pBri->GetConcBasicByTitle(&pBri->m_pArrConcFootRev[eType], sTitleRev);
				if(pConcRev)
				{
					for(dan = 0; dan < MAX_REBAR_DAN; dan++)
					{
						double  dDia = nCase == 0 ? 0 : pFooting->GetDiaMainRebar(0, bUpper, dan);
						pFooting->SetJointMainRebarCycle1(bUpper, pConc->m_RbT1C_DIA[dan], MAX(dDia, pConcRev->m_RbT1C_DIA[dan]), dan);
					}
				}


				// 2cycle
				sFront	= bUpper ? "뒷굽" : "앞굽";
				sFrontRev	= bUpper ? "앞굽" : "뒷굽";
				if(footing == 0)
				{
					sTitle.Format("시점측(%s)", sFront);
					sTitleRev.Format("시점측(%s)", sFrontRev);
				}
				else if(footing == pBri->m_nQtyJigan)
				{
					sTitle.Format("종점측(%s)", sFront);
					sTitleRev.Format("종점측(%s)", sFrontRev);
				}
				else
				{
					sTitle.Format("중간지점%d", footing);
					sTitleRev	= sTitle;
				}
				pConc	= pBri->GetConcBasicByTitle(&pBri->m_pArrConcFoot[eType], sTitle);
				if(pConc)
				{
					for(dan = 0; dan < MAX_REBAR_DAN; dan++)
					{
						double  dDia = nCase == 0 ? 0 : pFooting->GetDiaMainRebar(1, bUpper, dan);
						pFooting->SetDiaMainRebar(1, bUpper, dan, MAX(dDia, pConc->m_RbT2C_DIA[dan]));
					}
				}

				// rev쪽에 직경이 설정되었는지 판단해서
				// 있다면 1cycle과 비교해서 큰걸로 매칭시켜준다.
				pConcRev	= pBri->GetConcBasicByTitle(&pBri->m_pArrConcFootRev[eType], sTitle);
				if(pConcRev)
				{
					for(dan = 0; dan < MAX_REBAR_DAN; dan++)
					{
						double  dDia = nCase == 0 ? 0 : pFooting->GetDiaMainRebar(1, bUpper, dan);
						pFooting->SetJointMainRebarCycle2(bUpper, pConc->m_RbT2C_DIA[dan], MAX(dDia, pConcRev->m_RbT2C_DIA[dan]), dan);
					}
				}
			}
		}
	}

	// 아치리브
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		long nSizeArchRib = pBri->m_pArrConcArchRib.GetSize();
		long i=0; for(i=0; i<nSizeArchRib; i++)
		{
			pConc = pBri->m_pArrConcArchRib.GetAt(i);
			if(pConc)
			{
				for(dan = 0; dan < MAX_REBAR_DAN; dan++)
				{
					pBri->SetDiaMainRebarArchRib(0, dan, i, pConc->m_RbT1C_DIA[dan]);
					pBri->SetDiaMainRebarArchRib(1, dan, i, pConc->m_RbT2C_DIA[dan]);
				}
			}
		}
	}


	// 2, 4 사이클 싱크
	if(pBri->GetCountCycleMainRebar() == 4)
	{
		pBri->SyncCycleMainRebar(0, 2);
		pBri->SyncCycleMainRebar(1, 3);
	}
}

// 옵션 선택시 좌우 철근 대칭적용 외측벽체 좌우, 슬래브 우각부..
void CInputRebarMain::SetRebarSymmetry()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	double dDiaLeft = 0;
	double dDiaRight = 0;
	double dDiaMax = 0;

	long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		for(long nCycle=0; nCycle<2; nCycle++)
		{
			// 슬래브 
			// 상부 슬래브 상면 좌우측단부
			dDiaLeft = pBri->GetDiaMainRebarUpperSlab(nCycle, dan, TRUE, 0, 0);
			dDiaRight = pBri->GetDiaMainRebarUpperSlab(nCycle, dan, TRUE, -1, -1);

			dDiaMax = max(dDiaLeft, dDiaRight);

			pBri->SetDiaMainRebarUpperSlab(nCycle, dan, TRUE, 0, 0, dDiaMax);
			pBri->SetDiaMainRebarUpperSlab(nCycle, dan, TRUE, -1, 1, dDiaMax);

			// 외측벽체 전면 배면
			for(long nInner=0; nInner<2; nInner++)
			{
				BOOL bInner = (nInner==iINNER)? TRUE : FALSE;
				// 외측벽체 하단
				dDiaLeft = pBri->GetDiaMainRebarOutWall(nCycle, dan, TRUE, bInner, 0);
				dDiaRight = pBri->GetDiaMainRebarOutWall(nCycle, dan, FALSE, bInner, 0);
				dDiaMax = max(dDiaLeft, dDiaRight);

				pBri->SetDiaMainRebarOutWall(nCycle, dan, TRUE, bInner, 0, dDiaMax);
				pBri->SetDiaMainRebarOutWall(nCycle, dan, FALSE, bInner, 0, dDiaMax);

				// 벽체 철근 나머지는 하나의 철근으로 맞춘다...
				dDiaMax = 0;
				// 가장 큰 Dia를 찾는다.
				// 시점 벽체
				long nStt	= 1;
				long nEnd	= pBri->GetCountJointMainRebarOutWall(0, dan, TRUE, bInner);
				if(!bInner)
					nEnd --;
				
				long i = 0; for(i = nStt; i <= nEnd; i++)
				{
					dDiaLeft = pBri->GetDiaMainRebarOutWall(nCycle, dan, TRUE, bInner, i);
					dDiaMax = max(dDiaMax, dDiaLeft);
				}
				// 종점벽체
				nEnd	= pBri->GetCountJointMainRebarOutWall(0, dan, FALSE, bInner);
				if(!bInner)
					nEnd --;
				for(i = nStt; i <= nEnd; i++)
				{
					dDiaRight = pBri->GetDiaMainRebarOutWall(nCycle, dan, FALSE, bInner, i);
					dDiaMax = max(dDiaMax, dDiaRight);
				}

				// 가장 큰 Dia로 모두 맞춘다.
				nEnd	= pBri->GetCountJointMainRebarOutWall(0, dan, TRUE, bInner);
				if(!bInner)
					nEnd --;
				
				for(i = nStt; i <= nEnd; i++)
				{
					pBri->SetDiaMainRebarOutWall(nCycle, dan, TRUE, bInner, i, dDiaMax);
				}
				// 종점벽체
				nEnd	= pBri->GetCountJointMainRebarOutWall(0, dan, FALSE, bInner);
				if(!bInner)
					nEnd --;
				for(i = nStt; i <= nEnd; i++)
				{
					pBri->SetDiaMainRebarOutWall(nCycle, dan, FALSE, bInner, i, dDiaMax);
				}
			}
		}
	}
}


BOOL CInputRebarMain::IsConsider2Cycle(CConcBasic *pConc, BOOL bRev)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(pConc->m_sTitle.Find("벽체")!=-1)
	{
		if(pConc->m_sTitle.Find("시점")!=-1)
		{
			if(pConc->m_sTitle.Find("중앙(배면)")!=-1)
			{
				return pBri->IsJointMainRebar_OutWall_Cycle2(TRUE, FALSE);
				/*
				CSetRebarJoint *pSetRebarJoint = &pBri->m_pArrSetRebarJointOutWall_Outter[0];
				return pSetRebarJoint->m_bJoint;
				*/
			}
			if(pConc->m_sTitle.Find("하단(전면)")!=-1)
			{
				return pBri->IsJointMainRebar_OutWall_Cycle2(TRUE, TRUE);
				/*
				CSetRebarJoint *pSetRebarJoint = &pBri->m_pArrSetRebarJointOutWall_Inner[0][1];
				return pSetRebarJoint->m_bJoint;
				*/
			}
		}

		if(pConc->m_sTitle.Find("종점")!=-1)
		{
			CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
			if(pConc->m_sTitle.Find("중앙(배면)")!=-1)
			{
				return pBri->IsJointMainRebar_OutWall_Cycle2(FALSE, FALSE);
				/*
				CSetRebarJoint *pSetRebarJoint = &pBri->m_pArrSetRebarJointOutWall_Outter[1];
				return pSetRebarJoint->m_bJoint;
				*/
			}
			if(pConc->m_sTitle.Find("하단(전면)")!=-1)
			{
				return pBri->IsJointMainRebar_OutWall_Cycle2(FALSE, TRUE);
				/*
				CSetRebarJoint *pSetRebarJoint = &pBri->m_pArrSetRebarJointOutWall_Inner[1][1];
				return pSetRebarJoint->m_bJoint;
				*/
			}
		}
	}
	// 기초(시종점)
	else if(pConc->m_sTitle.Find("앞굽", 0) != -1 || pConc->m_sTitle.Find("뒷굽", 0) != -1)
	{
		if(!bRev) return TRUE;
		BOOL bStt	= pConc->m_sTitle.Find("시점", 0) != -1;
		BOOL bFront	= pConc->m_sTitle.Find("앞굽", 0) != -1;
		BOOL bLeft	= (bStt && !bFront) || (!bStt && bFront);
		long nJ		= bStt ? 0 : pBri->m_nQtyJigan;
		BOOL bUpper	= bFront ? TRUE : FALSE;
		
		CDPoint xyInfo	= pBri->GetInfoMainRebar_Footing(nJ, bUpper, 1, 0, bLeft);

		return xyInfo.x > 0;
	}

	return TRUE;
}

void CInputRebarMain::Empty2Cycle(CConcBasic *pConc)
{
	long j=0; for(j=0; j<REBAR_MAX_LAYER; j++)
	{
		pConc->m_RbT2C_DIA[j] = 0;
		pConc->m_RbT2C_B[j]   = 0;
	}
}

void CInputRebarMain::OnCheckRebarLength() 
{
	// TODO: Add your control notification handler code here
	DrawInputDomyunView(FALSE);	
}

void CInputRebarMain::OnCellInsertedCol(NMHDR* pNotifyStruct, LRESULT* result)
{
	CCellID cell	= m_Grid.GetFocusCell();
	m_Grid.InsertColumnMng(cell.col);
	SetDataInit();
}

void CInputRebarMain::OnSelchangeComboDan() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	BOOL bDetailInput	= pBri->m_bDetailInput23DanMainRebar;

	if(m_nTab == m_nTabArchRib || bDetailInput)
	{
		SetGridData();
		SetDataInit();
		DrawInputDomyunView(FALSE);
	}
	
}

void CInputRebarMain::OnButtonCheckrib() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	
	BOOL bFemNormal = pBri->m_FemNormal.IsOutputLoaded();
	BOOL bFemEQ     = pBri->IsRcSlab() ? TRUE : (pBri->GetFemManage(COMBO_EARTHQUAKE)->IsOutputLoaded());
	
	if(!bFemNormal || (pBri->GetTypeDesingEQ() == 1 && !bFemEQ))
	{
		AfxMessageBox("모델링 해석을 수행후에 설계검토를 하시기 바랍니다.");
		return;
	}
	
	m_pDlgCheckRib->m_pIn = m_pIn;
	if(m_pDlgCheckRib->GetSafeHwnd() && m_pDlgCheckRib->IsWindowVisible()) m_pDlgCheckRib->DestroyWindow();
	m_pDlgCheckRib->SetBriAndContent(pBri,IDD_REBAR_MAIN,0);

	m_pDlgCheckRib->ShowAndCheck(this);	
}

void CInputRebarMain::OptimizeRebarArchRib()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	
	long nSize = pBri->m_pArrConcArchRib.GetSize();
	double dCtcMain = pBri->GetCtcRebarMain(FALSE);
	long i=0; for(i=0; i<nSize; i++)
	{
		CConcBasic *pConc = pBri->m_pArrConcArchRib.GetAt(i);
		OptimizeBeam(pConc, dCtcMain);
	}
}

void CInputRebarMain::SetGridData_OutWall2Dan(long nCycle)
{
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 2, 0);
	m_Grid.SetCellSize(20, 50);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long row		= 0, col = 0;
	CString str		= _T("");
	CString sHelp	= _T("");
	CString sStt	= _T("");
	CString sInner	= _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;
	long nCountJoint	= 0;

	// 외측벽체 시,종점 이음 간격/철근지름
	for(long stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		sStt		= bStt ? "시점측 벽체" : "종점측 벽체";
		
		// 전면 배면
		for(long inner = 0; inner < 2; inner++)
		{
			BOOL bInner	= inner == iINNER;
			long nType	= bStt ? (bInner ? MARK_STTWALL_INNER : MARK_STTWALL_OUTTER) : (bInner ? MARK_ENDWALL_INNER : MARK_ENDWALL_OUTTER);
			sInner	= bInner ? "전면 " : "배면 ";

			pBri->GetMarkRebarMain(nType, TRUE, nCycle, TRUE, TRUE, _T(""), 0);

			// 외측벽체 시종점 이음 간격
			nCountJoint	= pBri->m_nCountJointMainRebar_OutWall_Cycle2[stt][inner][0];
			nCountJoint	= bInner ? nCountJoint + 1 : nCountJoint;
			long i = 0; for(i = 0; i < nCountJoint; i++)
			{
				str	= pBri->GetMarkRebarMain(nType, TRUE, nCycle, FALSE, TRUE, _T(""), 0);
				m_Grid.AddCell(row, col, sStt);
				m_Grid.AddCell(row+1, col, str);
				m_Grid.AddCell(row+2, col, &pBri->m_pArrExistOutWall[nCycle][stt][inner][1], i, nFormat, nLine, "", CELL_TYPE_CHECK);
				col++;
			}
		}
	}
}

void CInputRebarMain::OnCheck2dan() 
{
	// TODO: Add your control notification handler code here
	SetGridData();
	SetDataInit();
}

BOOL CInputRebarMain::IsSet2Dan()
{
	return m_check2Dan.GetCheck();
}


void CInputRebarMain::OnCheckViewCycle2() 
{
	// TODO: Add your control notification handler code here
	DrawInputDomyunView(FALSE);
}

void CInputRebarMain::SetGridData_LowerSlab( long nCycle )
{
	if(!(nCycle == 0 || nCycle == 2)) return;

	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 2, 0);
	m_Grid.SetCellSize(20, 50);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long row		= 0, col = 0;
	CString str		= _T("");
	CString sHelp	= _T("");
	CString sUpdn	= _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;
// 	long nNum		= 1;


	// 하부슬래브 상,하면 구간별 이음 간격/철근지름
	for(long upper = 0; upper < 2; upper++)
	{
		BOOL bUpper	= upper == iUPPER;
		long nType	= bUpper ? MARK_LOWERSLAB_UPPER : MARK_LOWERSLAB_LOWER;
		pBri->GetMarkRebarMain(nType, TRUE, nCycle, TRUE, TRUE, _T(""), 0);

		// 하부 슬래브 상면 각 구간별 이음 간격
		sUpdn		= bUpper ? "상면 " : "하면 ";
		long nNum		= 1;
		long n = 0; for(n = 0; n < pBri->m_nQtyJigan; n++)
		{
			long i = 0; for(i = 0; i < pBri->m_nCountJointMainRebar_LowerSlab_Cycle1[n][upper][0]; i++)
			{
				if(upper == iUPPER) str.Format("UD%d", nNum++);
				else str.Format("LD%d", nNum++);

				if(i > 0) sHelp.Format("%d번째 지간의 %d번째 이음과 %d번째 이음과의 간격", n+1, i, i+1);
				else sHelp.Format("%d번째 지간의 시작위치와 %d번째 이음과의 간격", n+1, i+1);
				sHelp = sUpdn + sHelp;

				m_Grid.AddCell(row, col, sUpdn);
				m_Grid.AddCell(row+1, col, str);
				m_Grid.AddCell(row+2, col, &pBri->m_pArrDistLowerSlab[nCycle][upper][0], n, i, nFormat, nLine, sHelp); 
				m_Grid.SetCellDec(CELL_DEC_0);
				m_Grid.SetCellPrintZero(TRUE);
				if(!bUpper) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
				col++;
			}
		}

		// 하부슬래브 각 구간별 철근 지름
		// 각 구간별 이음 간격 개수보다 1개 많다.
		// 그리고, 신축이음 개수만큼 추가된다.
		// 3cycle은 지름 입력받지 않음
		if(nCycle == 0)
		{
			for(n = 0; n < pBri->m_nQtyJigan; n++)
			{
				long nAddExpJoint	= pBri->m_bExpJoint[n+1] ? 1 : 0;
				if(n == pBri->m_nQtyJigan-1)
					nAddExpJoint	= 0;
				long i = 0; for(i = 0; i < pBri->m_nCountJointMainRebar_LowerSlab_Cycle1[n][upper][0] + nAddExpJoint; i++)
				{
					str	= pBri->GetMarkRebarMain(nType, TRUE, nCycle, FALSE, TRUE, _T(""), 0);

					sHelp.Format("%d번째 지간의 %d번째 주철근 지름", n+1, i+1);
					sHelp = sUpdn + sHelp;

					m_Grid.AddCell(row, col, sUpdn);
					m_Grid.AddCell(row+1, col, str);
					m_Grid.AddCell(row+2, col, &pBri->m_pArrDiaLowerSlab[nCycle][upper][0], n, i, nFormat, nLine, sHelp, CELL_TYPE_DIA);
					if(!bUpper) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
					col++;
				}

				if(n == pBri->m_nQtyJigan-1)
				{
					str	= pBri->GetMarkRebarMain(nType, TRUE, nCycle, FALSE, TRUE, _T(""), 0);

					sHelp.Format("%d번째 지간의 %d번째 주철근 지름", n+1, i+1);
					sHelp = sUpdn + sHelp;

					m_Grid.AddCell(row, col, sUpdn);
					m_Grid.AddCell(row+1, col, str);
					m_Grid.AddCell(row+2, col, &pBri->m_pArrDiaLowerSlab[nCycle][upper][0], n, i, nFormat, nLine, sHelp, CELL_TYPE_DIA);
					if(!bUpper) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
					col++;
				}
			}
		}
	}
}

void CInputRebarMain::OnButtonRebarSupport()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	CRebarSupportDlg dlg(pBri, m_pIn->m_pARcBridgeDataStd);
	if(dlg.DoModal() == IDOK)
	{
		pBri->SetDataDefaultRebarSupport_VertSection_Std(-1, -1);

		//SyncRebar();
		DrawInputDomyunView(FALSE);

		// 상하행선 분리 적용
		m_pIn->m_pARcBridgeDataStd->CopyToCoupleBridge_RebarStandardSection(m_pIn->m_nCurrentBridge);

		if(m_pDlgCheck->GetSafeHwnd() && m_pDlgCheck->IsWindowVisible())
		{
			pBri->MakeRebarInfoCycleStd(TRUE);
			m_pDlgCheck->Check();
		}
	}
}

void CInputRebarMain::OnTabSelchangeCycle( NMHDR* pNMHDR, LRESULT* pResult )
{
	SetFocus();
	m_nTabCycle = m_TabCycle.GetCurSel();
	SetGridData();
	SetDataInit();
	DrawInputDomyunView(FALSE);

	return;
}

BOOL CInputRebarMain::CheckBeamForOptimize( CConcBasic *pConc )
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return FALSE;

	double	stdSF = pBri->m_dOptimizeSafeRate / 100;

	BOOL bOK = FALSE;
	if(m_bDesignLsd)
	{
		CLsdBeam Beam(pConc, LSD_ULS_DESIGN);
		BOOL bRate = (Beam.IsOkRebarRate(TRUE) && Beam.IsOkRebarRate(FALSE)) ? TRUE : FALSE;
		double SF = Beam.GetMn() / Beam.GetMu();
		BOOL bCrack = Beam.IsOkCrackWidth();
		bOK = stdSF <= SF && bRate && bCrack;
	}
	else
	{
		CRectBeam Beam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		double SF = Beam.GetSafeFactorMoment();
		long nRate = Beam.GetCheckRebarRate();
		double W = Beam.GetW();
		double Wa = Beam.GetWa();
		bOK = stdSF <= SF && (nRate == 0 || nRate == 1) && W <= Wa;
	}

	return bOK;
}
