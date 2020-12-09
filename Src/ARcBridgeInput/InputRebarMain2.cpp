// InputRebarMain2.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "../ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "RebarJoint1CycleDlg.h"
#include "RebarDanDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputRebarMain2 dialog


CInputRebarMain2::CInputRebarMain2(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputRebarMain2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputRebarMain2)
	//}}AFX_DATA_INIT
	m_nTab	= 0;
	m_nTabCycle		= 0;
	m_nTabCycle2	= -1;
	m_nTabCycle4	= -1;

	m_nTabLowerSlab = -1;
	m_nTabUpperSlab = -1;
	m_nTabFooting = -1;
	m_nTabArchRib = -1;
	m_nTabOutWall = -1;
	m_nTabInWall = -1;

	m_pDlgCheck = new CInputCheckGrid;
}

CInputRebarMain2::~CInputRebarMain2()
{
	delete m_pDlgCheck;
}

void CInputRebarMain2::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputRebarMain2)
	DDX_Control(pDX, IDC_CHECK_BENDING, m_checkBending);
	DDX_Control(pDX, IDC_CHECK_VIEW_CYCLE1, m_checkViewCycle1);
	DDX_Control(pDX, IDC_CHECK_2DAN, m_check2Dan);
	DDX_Control(pDX, IDC_STATIC_RIB, m_staticRib);
	DDX_Control(pDX, IDC_COMBO_DAN, m_comboDan);
	DDX_Control(pDX, IDC_CHECK_REBAR_LENGTH, m_checkRebarLength);
	DDX_Control(pDX, IDC_CHECK_JOINT, m_checkJoint);
	DDX_Control(pDX, IDC_BUTTON_REBARDAN, m_btnRebarDan);
	DDX_Control(pDX, IDC_BUTTON_SAMESTTEND2, m_btnSameSttEnd);
	DDX_Control(pDX, IDC_BUTTON_SAMEINWALL2, m_btnSameInWall);
	DDX_Control(pDX, IDC_CHECK_BMD, m_checkBMD);
	DDX_Control(pDX, IDC_BUTTON_REBARJOINT, m_btnRebarJoint);
	DDX_Control(pDX, IDC_BUTTON_EXAMINE, m_btnExamine);
	DDX_Control(pDX, IDC_CHECK_ALLMARK, m_checkMark);
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_comboBridgeNo);
	DDX_Control(pDX, IDC_COMBO_CTC, m_comboCTC);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_TAB_CYCLE, m_TabCycle);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputRebarMain2, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputRebarMain2)
	ON_BN_CLICKED(IDC_CHECK_ALLMARK, OnCheckAllmark)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnSelchangingTab)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_BN_CLICKED(IDC_BUTTON_REBARJOINT, OnButtonRebarjoint)
	ON_BN_CLICKED(IDC_CHECK_BMD, OnCheckBmd)
	ON_BN_CLICKED(IDC_BUTTON_EXAMINE, OnButtonExamine)
	ON_BN_CLICKED(IDC_BUTTON_SAMEINWALL2, OnButtonSameinwall2)
	ON_BN_CLICKED(IDC_BUTTON_SAMESTTEND2, OnButtonSamesttend2)
	ON_BN_CLICKED(IDC_BUTTON_REBARDAN, OnButtonRebardan)
	ON_BN_CLICKED(IDC_CHECK_JOINT, OnCheckJoint)
	ON_BN_CLICKED(IDC_CHECK_REBAR_LENGTH, OnCheckRebarLength)
	ON_CBN_SELCHANGE(IDC_COMBO_DAN, OnSelchangeComboDan)
	ON_BN_CLICKED(IDC_CHECK_2DAN, OnCheck2dan)
	ON_BN_CLICKED(IDC_CHECK_VIEW_CYCLE1, OnCheckViewCycle1)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
	ON_BN_CLICKED(IDC_CHECK_BENDING, OnCheckBending)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW,  IDC_GRID, OnCellChangedRow)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputRebarMain message handlers
void CInputRebarMain2::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	
	return;
}

void CInputRebarMain2::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	SetFocus();
	m_nTabCycle = m_TabCycle.GetCurSel();
	SetGridData();
	SetDataInit();
	DrawInputDomyunView(FALSE);

	return;
}

void CInputRebarMain2::SetGridData()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long nCycle	= GetCurCycle();
	
	BOOL bDetailInput	= pBri->m_bDetailInput23DanMainRebar;
	long nDan			= bDetailInput ? m_comboDan.GetCurSel() : 0;


	if(m_nTab == m_nTabUpperSlab)
	{
		m_Grid.InitGrid(pBri->GetValueFy(ePartUpperSlab));
		SetGridData_UpperSlab(nCycle, &m_Grid, nDan);
		m_nRowFocusItemTitle	= 1;
	}
	else if(m_nTab == m_nTabOutWall)
	{
		m_Grid.InitGrid(pBri->GetValueFy(ePartOutWallStt));
		if(IsSet2Dan() && !bDetailInput)
			SetGridData_OutWall2Dan(nCycle);
		else
			SetGridData_OutWall(nCycle, &m_Grid, nDan);
		m_nRowFocusItemTitle	= 1;
	}
	else if(m_nTab == m_nTabInWall)
	{
		m_Grid.InitGrid(pBri->GetValueFy(ePartInWall));
		SetGridData_InWall(nCycle, &m_Grid, nDan);
		m_nRowFocusItemTitle	= 1;
	}
	else if(m_nTab == m_nTabFooting)
	{
		m_Grid.InitGrid(pBri->GetValueFy(ePartFootStt));
		SetGridDataCycle24_Footing(nCycle);
		m_nRowFocusItemTitle	= 0;
	}
	else if(m_nTab == m_nTabArchRib)
	{
		m_Grid.InitGrid(pBri->GetValueFy(ePartUpperSlab));
		SetGridData_ArchRib(nCycle);
		m_nRowFocusItemTitle	= 1;
	}
	else if(m_nTab == m_nTabLowerSlab)
	{
		m_Grid.InitGrid(pBri->GetValueFy(ePartLowerSlab));
		SetGridData_LowerSlab(nCycle, &m_Grid, nDan);
	}

	m_Grid.SetRedraw(TRUE, TRUE);
}

void CInputRebarMain2::SetDataSave()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	m_Grid.UpdateData(TRUE);

	SyncRebar();

	// 상하행선 분리 적용
	m_pIn->m_pARcBridgeDataStd->CopyToCoupleBridge_RebarMain(m_pIn->m_nCurrentBridge);

	m_pIn->m_pDoc->SetModifiedFlag();
	
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

void CInputRebarMain2::SetDataDefault()
{
	for(long bri = 0; bri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(bri);
		if(!pBri) return;

		BOOL bFirst   = IsFirst(bri);
		pBri->SetDefaultedState(GetIDD(), 0x00000001);
		if(!bFirst && !IsClickDefaultButton(bri)) continue;

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
			nCycle	= 1;	// 2사이클에서 전체사이클을 기본값적용하면 1사이클도 기본값으로 변한다.
		}

		if(bFirst)
		{
			pBri->SetDataDefaultCountJoint(nCycle);
			pBri->SetDataDefaultRebarInfoCycle_Std(nType, nCycle);
		}
		else
		{
			pBri->SetDataDefaultRebarInfoCycle_Std(nType, nCycle, FALSE, pBri->m_bDetailInput23DanMainRebar ? m_comboDan.GetCurSel() : 0);
			// 상하행선 분리 적용
			m_pIn->m_pARcBridgeDataStd->CopyToCoupleBridge_RebarMain(m_pIn->m_nCurrentBridge);
		}

		if(m_nTab == m_nTabUpperSlab)
		{
			// 슬래브 주철근과 배력근 좌표 초기화
			pBri->m_bCaledRebarMain_Plane	= FALSE;
			pBri->m_bCaledRebarSupport_Plane	= FALSE;
		}
	}

	SetGridData();
	SetDataInit();
}

void CInputRebarMain2::InitTabCtrl()
{
	m_TabCycle.RemoveAllTabs();
	m_TabCycle.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_TabCycle.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	// 사이클 탭
	CStringArray sArrTabCycleName;
	long nTab		= 0;
	m_nTabCycle2	= -1;
	m_nTabCycle4	= -1;
	sArrTabCycleName.Add("② CYCLE"); m_nTabCycle2	= 0;

	/* 당분간 4cycle 입력은 받지 않기로 함
	// 필요하다면 이 주석만 풀면 4cycle에 대해서도 입력 받을 수 있음. 단.. 2cycle 입력시 자동으로 4cycle도 변하므로 
	// 2cycle과 4cycle간의 동기화 부분의 코드는 약간의 수정이 필요함
	if(pBri->GetCountCycleMainRebar() > 2)
	{
		sArrTabCycleName.Add("④ CYCLE"); m_nTabCycle4	= 1;
	}
	*/

	long i = 0; for(i = 0; i < sArrTabCycleName.GetSize(); i++)
	m_TabCycle.InsertItem(i, sArrTabCycleName[i]);

	if(m_TabCycle.GetItemCount() <= m_nTabCycle) m_nTabCycle = 0;
	m_TabCycle.SetCurSel(m_nTabCycle);


	
	nTab			= 0;
	m_nTabUpperSlab	= -1;
	m_nTabOutWall	= -1;
	m_nTabInWall	= -1;
	m_nTabFooting	= -1;
	m_nTabArchRib	= -1;
	m_nTabLowerSlab	= -1;
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
		m_sArrTabName.Add("아치리브"); m_nTabArchRib = nTab++;
	}
		
	m_Tab.DeleteAllItems();
	for(i = 0; i < m_sArrTabName.GetSize(); i++)
		m_Tab.InsertItem(i, m_sArrTabName[i]);

	if(m_Tab.GetItemCount() <= m_nTab) m_nTab = 0;
	if(m_pIn->m_nLastTab_RebarMain > 0 && m_pIn->m_nLastTab_RebarMain < m_Tab.GetItemCount())
		m_nTab	= m_pIn->m_nLastTab_RebarMain;
	m_Tab.SetCurSel(m_nTab);
}

void CInputRebarMain2::OnPreInitDialog()
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


	SetResize(IDC_BUTTON_EXAMINE,		SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
	SetResize(IDC_TAB,					SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
	SetResize(IDC_TAB_CYCLE,			SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID,					SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_REBARJOINT,	SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_REBARDAN,	SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_VIEW,			SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_ALLMARK,		SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_BMD,			SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_JOINT,			SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_VIEW_CYCLE1,			SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_REBAR_LENGTH,		SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_SAMEINWALL2,		SZ_BOTTOM_RIGHT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_SAMESTTEND2,		SZ_BOTTOM_RIGHT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_CHECK_2DAN,		SZ_BOTTOM_RIGHT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_CHECK_BENDING,		SZ_BOTTOM_RIGHT,		SZ_BOTTOM_RIGHT);
	
	SetHyperHelp(m_pIn->GetManualPath());

	m_comboDan.SetCurSel(0);
	SetGridData();
}

BOOL CInputRebarMain2::PreTranslateMessage(MSG* pMsg) 
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

void CInputRebarMain2::InitComboCtrl()
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

void CInputRebarMain2::OnSelchangeComboBridgeNo() 
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

void CInputRebarMain2::DrawInputDomyunView(BOOL bZoomAll)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(m_pIn->GetScaleInputDialog());

	CDomyun Dom(pDom);
	CARcBridgeDBStandard DBStd(m_pIn->GetRcBridgeRebar(), m_pIn->m_pARoadOptionStd, m_pIn->m_pARcBridgeDataStd);
	DBStd.m_bCheckOverLengthRebar	= m_checkRebarLength.GetCheck();

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
	pBri->SyncBridgeRebar(TRUE, bVertDir ? -1 : 1, TRUE);

	
	// 종단면 그림
	DBStd.m_pDrawVertSection->DrawVertSectionAll(&Dom, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE);
	DBStd.m_pDrawVertSection->DrawDiagramBMD_SFD(&Dom, m_checkBMD.GetCheck(), FALSE, FALSE);

	long nCycle = GetCurCycle();
	long nDan = 0;
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
	nDan			= bDimRib || pBri->m_bDetailInput23DanMainRebar ? m_comboDan.GetCurSel() : 0;

	// hidden == 0 : 현재 사이클 그림
	// hidden == 1 : 점선으로 그려야 되는 사이클 그림
	for(long hidden = 0; hidden < 2; hidden++)
	{
		BOOL bHidden	= hidden == 1;
		if(hidden == 1 && !m_checkViewCycle1.GetCheck()) continue;
		if(hidden == 1)
		{
			switch(nCycle)
			{
			case 1: nCycle	= 0; break;
			case 3: nCycle	= 2; break;
			}
			DBStd.m_bDrawRebarInfoCycleByHidden	= TRUE;
		}

		long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			if(bDrawBridge)
			{
				DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_UpperSlab(&Dom, nCycle, dan, TRUE, !bHidden && bDimUpperSlab && dan==nDan, !bHidden && bDimUpperSlab, 0, CDRect(0, 0, 0, 0), TRUE, nVertDir);
				DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_ArchRib(&Dom, nCycle, dan, TRUE, !bHidden && bDimRib && dan==nDan, !bHidden && bDimRib, 0, CDRect(0, 0, 0, 0), TRUE, nVertDir);
				DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_LowerSlab(&Dom, nCycle, dan, TRUE, !bHidden && bDimLowerSlab && dan==nDan, !bHidden && bDimLowerSlab, 0, CDRect(0, 0, 0, 0), TRUE, nVertDir);

				DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_OutWall(&Dom, nCycle, dan, TRUE, !bHidden && bDimOutWall && dan==nDan, !bHidden && bDimOutWall, 0, CDRect(0, 0, 0, 0), TRUE, FALSE, nVertDir);
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
				
				// 벽체들 철근 그리기.(벽체의 기초 포함)
				long wall = 0; for(wall = 0; wall < nCountWall; wall++)
				{
					CWallApp *pWall	= pBri->GetInWall(wall);
					if(pWall)
					{
						for(long left = 0; left < 2; left++)
						{
							DBStd.m_pDrawWall->DrawRebarMain_Front(&Dom, pWall, nCycle, dan, left, !bHidden && bDimInWall && dan==nDan, !bHidden && bDimInWall && dan==nDan);
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
					if(footing != nIdxFooting && !bAllMark) continue;

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
			DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_Danbu(&Dom, FALSE, FALSE, TRUE, nCycle);
			for(long stt = 0; stt < 2; stt++)
			{
				BOOL bStt	= stt == iSTT;
				DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_Bracket(&Dom, bStt, FALSE, !bHidden && bDimOutWall, TRUE, 0, CDRect(0, 0, 0, 0), 1);
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
			if(pBri->IsBoxType())
				DBStd.m_pDrawVertSection->DimRebarMainAssm_VertSection_BoxOutSide(&Dom, nDimDanJointDim, nCycle, 0, nDan);
			else
				DBStd.m_pDrawVertSection->DimRebarMainAssm_VertSection_Upper(&Dom, nDimDanJointDim, nCycle, 0, nDan);
			DBStd.m_pDrawVertSection->DimRebarMainAssm_VertSection_OutWall(&Dom, nDimDanJointDim, nCycle, TRUE, 0, nDan);
			DBStd.m_pDrawVertSection->DimRebarMainAssm_VertSection_OutWall(&Dom, nDimDanJointDim, nCycle, FALSE, 0, nDan);
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

	pBri->GetTvVertSection_Std(0, !bVertDir, FALSE, TRUE);
	*pDom << Dom;
	DBStd.m_bCheckOverLengthRebar	= FALSE;

	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

void CInputRebarMain2::SetDataInit()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	CHgBaseConcStd baseConc;

	m_Grid.UpdateData(FALSE);

	long nRows(0), nCols(0);
	m_Grid.GetCellCount(nRows, nCols);
	m_Grid.MergeGrid(0, 0, 0, nCols);

	m_comboCTC.SetCurSel(baseConc.GetIdxByRebarCTC(pBri->m_dCTC_MainRebar));
	m_checkBending.SetCheck(pBri->m_bBendingDanBuRebar);
}


void CInputRebarMain2::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	structGridCell *pCell = m_Grid.GetCurrentCell();
	if(pCell)
	{
		m_pIn->SetComment(pCell->sTitle);
		CString str	= m_Grid.GetItemText(m_nTab==m_nTabFooting ? 0 : 1,pCell->nCol);
		m_pView->GetDomyun()->STMakeCursor(str);
	}
}
void CInputRebarMain2::OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result)
{
	m_CellID	= m_Grid.GetFocusCell();

	if(m_nTab == m_nTabFooting)
		DrawInputDomyunView(FALSE);
}

void CInputRebarMain2::SetControl()
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

	m_comboCTC.EnableWindow(FALSE);
}

void CInputRebarMain2::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	CCellID cell		= m_Grid.GetFocusCell();

	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);
}

void CInputRebarMain2::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
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


// 사이클 2 입력(상부슬래브 상,하면)
void CInputRebarMain2::SetGridData_UpperSlab(long nCycle, CGridCtrlMng *pGrid, long nDan)
{
	// 그리드 초기화
	if(!pGrid)
		pGrid	= &m_Grid;
	pGrid->SetGridDefault(2, 2, 2, 0);
	pGrid->SetCellSize(20, 50);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long row		= 0, col = 0;
	CString str		= _T("");
	CString sHelp	= _T("");
	CString sUpdn	= _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;
	//nNum		= 1;


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
			long nCountJoint	= nCycle == 0 || nCycle == 2 ? pBri->m_nCountJointMainRebar_UpperSlab_Cycle1[n][upper][nDan] : pBri->m_nCountJointMainRebar_UpperSlab_Cycle2[n][upper][nDan];
			long i = 0; for(i = 0; i < nCountJoint; i++)
			{
				if(upper == iUPPER) str.Format("UD%d", nNum++);
				else str.Format("LD%d", nNum++);

				if(i > 0) sHelp.Format("%d번째 지간의 %d번째 이음과 %d번째 이음과의 간격", n+1, i, i+1);
				else sHelp.Format("%d번째 지간의 시작위치와 %d번째 이음과의 간격", n+1, i+1);
				sHelp = sUpdn + sHelp;

				// 하면이고 신축이음이 있다면 시점 처음과 종점 끝에 신축이음 철근 길이 추가
				if(!bUpper && pBri->IsBlockOut(TRUE) && n == 0 && i == 0)
				{
					pGrid->AddCell(row, col, sUpdn);
					pGrid->AddCell(row+1, col, "FXSL");
					pGrid->AddCell(row+2, col, &pBri->m_fxJointStt.m_dLenHorRebar[nCycle][nDan], nFormat, nLine, "시점 신축이음 하면 철근 길이"); 
					pGrid->SetCellDec(CELL_DEC_0);
					pGrid->SetCellPrintZero(TRUE);
					if(!bUpper) pGrid->SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
					col++;
				}

				pGrid->AddCell(row, col, sUpdn);
				pGrid->AddCell(row+1, col, str);
				pGrid->AddCell(row+2, col, &pBri->m_pArrDistUpperSlab[nCycle][upper][nDan], n, i, nFormat, nLine, sHelp); 
				pGrid->SetCellDec(CELL_DEC_0);
				pGrid->SetCellPrintZero(TRUE);
				if(!bUpper) pGrid->SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
				col++;
				
				// 하면이고 신축이음이 있다면 시점 처음과 종점 끝에 신축이음 철근 길이 추가
				if(!bUpper && pBri->IsBlockOut(FALSE) && n == pBri->m_nQtyJigan-1 && i == pBri->m_nCountJointMainRebar_UpperSlab_Cycle1[n][upper][nDan]-1)
				{
					pGrid->AddCell(row, col, sUpdn);
					pGrid->AddCell(row+1, col, "FXEL");
					pGrid->AddCell(row+2, col, &pBri->m_fxJointEnd.m_dLenHorRebar[nCycle][nDan], nFormat, nLine, "시점 신축이음 하면 철근 길이"); 
					pGrid->SetCellDec(CELL_DEC_0);
					pGrid->SetCellPrintZero(TRUE);
					if(!bUpper) pGrid->SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
					col++;
				}
			}
		}

		// 상부슬래브 각 구간별 철근 지름
		// 각 구간별 이음 간격 개수보다 1개 많다.
		// 그리고, 신축이음 개수만큼 추가된다.
		if(nCycle == 0 || nCycle==1)
		{
			for(n = 0; n < pBri->m_nQtyJigan; n++)
			{
				long nAddExpJoint	= pBri->m_bExpJoint[n+1] ? 1 : 0;
				if(n == pBri->m_nQtyJigan-1)
					nAddExpJoint	= 0;
				long nCountJoint	= nCycle == 0 || nCycle == 2 ? pBri->m_nCountJointMainRebar_UpperSlab_Cycle1[n][upper][nDan] : pBri->m_nCountJointMainRebar_UpperSlab_Cycle2[n][upper][nDan];
				long i = 0; for(i = 0; i < nCountJoint + nAddExpJoint; i++)
				{
					str	= pBri->GetMarkRebarMain(nType, TRUE, nCycle, FALSE, TRUE, _T(""), 0);

					sHelp.Format("%d번째 지간의 %d번째 주철근 지름", n+1, i+1);
					sHelp = sUpdn + sHelp;

					pGrid->AddCell(row, col, sUpdn);
					pGrid->AddCell(row+1, col, str);
					pGrid->AddCell(row+2, col, &pBri->m_pArrDiaUpperSlab[nCycle][upper][nDan], n, i, nFormat, nLine, sHelp, CELL_TYPE_DIA);
					if(!bUpper) pGrid->SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
					pGrid->AddCell(row+3, col, &pBri->m_pArrExistUpperSlab[nCycle][upper][nDan], n, i, nFormat, nLine, "", CELL_TYPE_CHECK);
					col++;
				}

				if(n == pBri->m_nQtyJigan-1)
				{
					str	= pBri->GetMarkRebarMain(nType, TRUE, nCycle, FALSE, TRUE, _T(""), 0);

					sHelp.Format("%d번째 지간의 %d번째 주철근 지름", n+1, i+1);
					sHelp = sUpdn + sHelp;

					pGrid->AddCell(row, col, sUpdn);
					pGrid->AddCell(row+1, col, str);
					pGrid->AddCell(row+2, col, &pBri->m_pArrDiaUpperSlab[nCycle][upper][nDan], n, i, nFormat, nLine, sHelp, CELL_TYPE_DIA);
					if(!bUpper) pGrid->SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
					pGrid->AddCell(row+3, col, &pBri->m_pArrExistUpperSlab[nCycle][upper][nDan], n, i, nFormat, nLine, "", CELL_TYPE_CHECK);
					col++;
				}
			}
		}
	}
}

void CInputRebarMain2::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	
	*pResult = 0;
}

// cycle 2 입력(시종점측 벽체 외측만)
void CInputRebarMain2::SetGridData_OutWall(long nCycle, CGridCtrlMng *pGrid, long nDan)
{
	// 그리드 초기화
	if(!pGrid)
		pGrid	= &m_Grid;
	pGrid->SetGridDefault(2, 2, 2, 0);
	pGrid->SetCellSize(20, -1);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long row		= 0, col = 0;
	CString str		= _T("");
	CString sHelp	= _T("");
	CString sStt	= _T("");
	CString sInner	= _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;
//	long nNum		= 1;
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
			nCountJoint	= nCycle == 0 || nCycle == 2 ? pBri->m_nCountJointMainRebar_OutWall_Cycle1[stt][inner][nDan] : pBri->m_nCountJointMainRebar_OutWall_Cycle2[stt][inner][nDan];
			long i = 0; for(i = 0; i < nCountJoint; i++)
			{
				if(stt == iUPPER) str.Format("SD%d", nNum++);
				else str.Format("ED%d", nNum++);

				sHelp.Format("%d번째 이음 까지의 간격", i+1);
				sHelp = sStt + sInner + sHelp;

				pGrid->AddCell(row, col, sStt+"("+sInner+")");
				pGrid->AddCell(row+1, col, str);
				pGrid->AddCell(row+2, col, &pBri->m_pArrDistOutWall[nCycle][stt][inner][nDan], i, nFormat, nLine, sHelp); 
				pGrid->SetCellDec(CELL_DEC_0);
				pGrid->SetCellPrintZero(TRUE);
				if(!bStt) pGrid->SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
				col++;
			}

			// 외측벽체 시종점 철근 지름
			// 3cycle은 지름 입력 받지 않음
			if(nCycle == 0 || nCycle == 1)
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

					pGrid->AddCell(row, col, sStt+"("+sInner+")");
					pGrid->AddCell(row+1, col, str);
					pGrid->AddCell(row+2, col, &pBri->m_pArrDiaOutWall[nCycle][stt][inner][nDan], i, nFormat, nLine, sHelp, CELL_TYPE_DIA);
					if(!bStt)
					{
						pGrid->SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
						pGrid->SetLastCellFy(pBri->GetValueFy(ePartOutWallEnd));
					}
					pGrid->AddCell(row+3, col, &pBri->m_pArrExistOutWall[nCycle][stt][inner][nDan], i, nFormat, nLine, "", CELL_TYPE_CHECK);
					
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

void CInputRebarMain2::OnCheckAllmark() 
{
	// TODO: Add your control notification handler code here
	DrawInputDomyunView(FALSE);
}


// 내측철근 지름 설정(1단만) - 모든 cycle
void CInputRebarMain2::SetGridData_InWall(long nCycle, CGridCtrlMng *pGrid, long nDan)
{
		// 그리드 초기화
	if(!pGrid)
		pGrid	= &m_Grid;
	pGrid->SetGridDefault(2, 2, 2, 0);
	pGrid->SetCellSize(20, -1);

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
			nCountJoint	= nCycle == 0 || nCycle == 2 ? pWall->m_nCountJointMainRebar_Cycle1[left][nDan] : pWall->m_nCountJointMainRebar_Cycle2[left][nDan];
			long i = 0; for(i = 0; i < nCountJoint; i++)
			{
				str.Format("WD%d", nNum++);

				sHelp.Format(" %d번째 이음 까지의 간격", i+1);
				sHelp = sWall + sLeft + sHelp;

				pGrid->AddCell(row, col, sWall+"("+sLeft+")");
				pGrid->AddCell(row+1, col, str);
				pGrid->AddCell(row+2, col, &pWall->m_dArrDistRC[nCycle][left][nDan], i, nFormat, nLine, sHelp); 
				pGrid->SetCellDec(CELL_DEC_0);
				pGrid->SetCellPrintZero(TRUE);
				if(!bLeft) pGrid->SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
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


					pGrid->AddCell(row, col, sWall+"("+sLeft+")");
					pGrid->AddCell(row+1, col, str);
					if(bNotInput)
						pGrid->AddCell(row+2, col, _T(""), nFormat, CELL_READONLY);
					else
						pGrid->AddCell(row+2, col, &pWall->m_dArrDiaRC[nCycle][left][nDan], i, nFormat, nLine, sHelp, CELL_TYPE_DIA);
					if(!bLeft) pGrid->SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
					if(bNotInput)
						pGrid->AddCell(row+3, col, _T(""), nFormat, CELL_READONLY);
					else
						pGrid->AddCell(row+3, col, &pWall->m_dArrExistRC[nCycle][left][nDan], i, nFormat, nLine, "", CELL_TYPE_CHECK);
					
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


void CInputRebarMain2::SetGridDataCycle24_Footing(long nCycle)
{
	if(!(nCycle == 1 || nCycle == 3)) return;

	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 1, 1);
	m_Grid.SetCellSize(20, -1);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	long nCountWall	= pBri->GetCountInWall();

	long nCountFooting	= nCountWall + 2;

	long row		= 0, col = 0;
	CString str		= _T("");
	CString sHelp	= _T("");
	CString sUpper	= _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;
	long nIdxWall	= 0;

	// 타이틀 생성 
	CFootingApp cFooting;
	cFooting.GetMarkRebarMain(MARK_UPPER, TRUE, nCycle, TRUE, TRUE, _T(""), 0);
	m_Grid.AddCell(row, col++, "구분");
	m_Grid.AddCell(row, col++, "L1");
	m_Grid.AddCell(row, col++, "L2");
	m_Grid.AddCell(row, col++, "L2'");
	m_Grid.AddCell(row, col++, cFooting.GetMarkRebarMain(MARK_UPPER, TRUE, nCycle, FALSE, TRUE, _T(""), 0));
	m_Grid.AddCell(row, col++, "U타입");

	cFooting.GetMarkRebarMain(MARK_LOWER, TRUE, nCycle, TRUE, TRUE, _T(""), 0);
	m_Grid.AddCell(row, col++, "L3");
	m_Grid.AddCell(row, col++, "L4");
	m_Grid.AddCell(row, col++, cFooting.GetMarkRebarMain(MARK_LOWER, TRUE, nCycle, FALSE, TRUE, _T(""), 0));
	m_Grid.AddCell(row, col++, "U타입");

	long footing = 0; for(footing = 0; footing < nCountFooting; footing++)
	{
		CFootingApp *pFooting = NULL;
		EPartOfBridge ePart = ePartFootStt;
		if(footing == 0) 
		{
			if(pBri->IsFooting(TRUE)) pFooting = &pBri->m_footingStt;
			str	= "시점측";
		}
		else if(footing == nCountFooting-1)
		{
			if(pBri->IsFooting(FALSE)) pFooting = &pBri->m_footingEnd;
			str	= "종점측";
			ePart = ePartFootEnd;
		}
		else
		{
			CWallApp *pWall	= pBri->GetInWall(nIdxWall++);
			pFooting		= &pWall->m_footing;
			str.Format("중간지점%d", nIdxWall);
			ePart = ePartInFoot;
		}
		
		if(!pFooting) continue;
		if(!pFooting->m_bIs) continue;
		long nType	= pFooting->GetTypeByInputValue();

		BOOL bExt	= nType == FOOTING_TYPE_LEFTSLOPE || nType == FOOTING_TYPE_RIGHTSLOPE;
		BOOL bExt2	= nType == FOOTING_TYPE_BASIC_LEFTSLOPE || nType == FOOTING_TYPE_BASIC_RIGHTSLOPE;

		row++;
		col	= 0;
		BOOL bReadOnly	= FALSE;
		BOOL bBoth		= nType == FOOTING_TYPE_BASIC_BOTH;
		BOOL bUpper		= TRUE;

		m_Grid.AddCell(row, col++, str);
		for(long upper = 0; upper < 2; upper++)
		{
			bUpper	= upper == iUPPER;
			// 기초 양쪽 길이가 같은 형태인경우 2사이클에서 상면 철근이 없고, 가로길이를 입력받지 않는다.
			bReadOnly = FALSE;

			// 세로길이
			m_Grid.AddCell(row, col++, &pFooting->m_dDistMainRebarVert[nCycle][upper], nFormat, bReadOnly ? CELL_READONLY : nLine, sHelp); 
			m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.SetCellPrintZero(TRUE);

			if(nType == FOOTING_TYPE_BASIC_LEFTSLOPE || nType == FOOTING_TYPE_BASIC_RIGHTSLOPE)
			{
				m_Grid.AddCell(row, col++, &pFooting->m_dDistMainRebarVertExt[nCycle][upper], nFormat, bReadOnly ? CELL_READONLY : nLine, sHelp); 
				m_Grid.SetCellDec(CELL_DEC_0);
				m_Grid.SetCellPrintZero(TRUE);
			}
			else
			{
				// 가로길이
				if(bBoth && upper == iLOWER) 
				{
					m_Grid.AddCell(row, col++, &pFooting->m_dDistMainRebarHor[nCycle][upper], nFormat, CELL_READONLY, sHelp); 
					m_Grid.SetCellDec(CELL_DEC_0);
					m_Grid.SetCellPrintZero(TRUE);
				}
				else
				{
					m_Grid.AddCell(row, col++, &pFooting->m_dDistMainRebarHor[nCycle][upper], nFormat, bExt || bExt2 ? nLine : (bBoth ? CELL_READONLY : nLine), sHelp); 
					m_Grid.SetCellDec(CELL_DEC_0);
					m_Grid.SetCellPrintZero(TRUE);
				}
			}

			// 가로길이2(경사2타입에만 입력받음)
			if(upper == iUPPER)
			{
				m_Grid.AddCell(row, col++, &pFooting->m_dDistMainRebarHorExt[nCycle], nFormat, bExt2 ? nLine : CELL_READONLY, sHelp); 
				m_Grid.SetCellDec(CELL_DEC_0);
				m_Grid.SetCellPrintZero(TRUE);
			}
			
			// 지름
			m_Grid.AddCell(row, col++, &pFooting->m_setRebarDan[upper].m_dDia[nCycle][0], nFormat, bReadOnly ? CELL_READONLY : nLine, sHelp, CELL_TYPE_DIA);
			m_Grid.SetLastCellFy(pBri->GetValueFy(ePart));

			// 'ㄷ'타입
			if(bReadOnly || (bBoth && !bUpper))
				m_Grid.AddCell(row, col++, "", nFormat, CELL_READONLY);
			else
				m_Grid.AddCell(row, col++, &pFooting->m_b2CycleMainRebarIsSameTypeWidth1Cycle[upper], nFormat, nLine, "", CELL_TYPE_CHECK);
		}
	}

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(0, 100);
}


long CInputRebarMain2::GetCurCycle()
{
	long nCycle	= 1;
	if(m_nTabCycle == m_nTabCycle2) nCycle	= 1;
	if(m_nTabCycle == m_nTabCycle4) nCycle	= 3;

	return nCycle;
}

void CInputRebarMain2::OnButtonRebarjoint() 
{
	// TODO: Add your control notification handler code here
	long nCycle	= GetCurCycle();
	CRebarJoint1CycleDlg dlg(m_pIn->GetRcBridgeRebar(), m_pIn->m_pARcBridgeDataStd, nCycle);
	if(dlg.DoModal() == IDOK)
	{
		// 이음 개수 변경시 지름이나 거리에 대해서 array개수를 정확하게 맞춰 줘야 한다. ////////////////////
		// 물론, 이음이 모자랄 경우 gridmaneger가 array를 추가하지만, 남을경우 제거를 해주지 않으므로, 
		// 지름이나 거리 가져올때 -1 옵션에 의해 마지막걸 가지고 오면 잘못된 값을 가져올 수 있다.
		CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
		if(!pBri) return;
		long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			pBri->SetDataDefaultRebarInfoCycle_Std(-1, -1, TRUE, dan);
		//////////////////////////////////////////////////////////////////////////////////////////////////
		

		SetGridData();
		SetDataInit();
		DrawInputDomyunView(FALSE);

		// 상하행선 분리 적용
		m_pIn->m_pARcBridgeDataStd->CopyToCoupleBridge_RebarMain(m_pIn->m_nCurrentBridge);
	}
}

void CInputRebarMain2::SyncRebar()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	pBri->SyncCycleMainRebar(1, 3);

	//SetGridData();	// 요거를 하면 포커스가 사라짐.
	SetDataInit();

}
// 현재 입력중인 푸팅리턴.
long CInputRebarMain2::GetIdxJijumCurFooting()
{
	long nIdx	= -1;
	if(m_nTab == m_nTabFooting)
	{
		if(m_CellID.row > 0)
			nIdx	= m_CellID.row - 1;
	}

	return nIdx;
}

void CInputRebarMain2::OnCheckBmd() 
{
	// TODO: Add your control notification handler code here
	DrawInputDomyunView(FALSE);	
}

void CInputRebarMain2::OnButtonExamine() 
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
	m_pDlgCheck->SetBriAndContent(pBri,IDD_REBAR_MAIN2,0);

	m_pDlgCheck->ShowAndCheck(this);	
	CString str = _T("");
	str.Format("주철근 검토");
	m_pDlgCheck->SetWindowText(str);
}

// 시종점부 동일 적용.
void CInputRebarMain2::OnButtonSamesttend2() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	ApplySameShapeSttAndEnd();
	SetGridData();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CInputRebarMain2::OnButtonSameinwall2() 
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
void CInputRebarMain2::ApplySameShapeInWall(long nIdxInWallStd)
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
			long nCycle	= 1;
			pWall->m_footing.m_dDistMainRebarVert[nCycle][upper]	= pWallOrg->m_footing.m_dDistMainRebarVert[nCycle][upper];
			pWall->m_footing.m_dDistMainRebarVertExt[nCycle][upper]	= pWallOrg->m_footing.m_dDistMainRebarVertExt[nCycle][upper];
			pWall->m_footing.m_dDistMainRebarHor[nCycle][upper]		= pWallOrg->m_footing.m_dDistMainRebarHor[nCycle][upper];
			pWall->m_footing.m_setRebarDan[upper].m_dDia[nCycle][0]	= pWallOrg->m_footing.m_setRebarDan[upper].m_dDia[nCycle][0];

			nCycle	= 3;
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
void CInputRebarMain2::ApplySameShapeSttAndEnd()
{
	if(AfxMessageBox("종점측 기초 주철근 배근을 시점측과 동일하게 적용하시겠습니까?", MB_YESNO) != IDYES) return;

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	for(long upper = 0; upper < 2; upper++)
	{
		long nCycle	= 1;
		pBri->m_footingEnd.m_dDistMainRebarVert[nCycle][upper]	= pBri->m_footingStt.m_dDistMainRebarVert[nCycle][upper];
		pBri->m_footingEnd.m_dDistMainRebarVertExt[nCycle][upper]	= pBri->m_footingStt.m_dDistMainRebarVertExt[nCycle][upper];
		pBri->m_footingEnd.m_dDistMainRebarHor[nCycle][upper]		= pBri->m_footingStt.m_dDistMainRebarHor[nCycle][upper];
		pBri->m_footingEnd.m_setRebarDan[upper].m_dDia[nCycle][0]	= pBri->m_footingStt.m_setRebarDan[upper].m_dDia[nCycle][0];

		nCycle	= 3;
		pBri->m_footingEnd.m_dDistMainRebarVert[nCycle][upper]	= pBri->m_footingStt.m_dDistMainRebarVert[nCycle][upper];
		pBri->m_footingEnd.m_dDistMainRebarVertExt[nCycle][upper]	= pBri->m_footingStt.m_dDistMainRebarVertExt[nCycle][upper];
		pBri->m_footingEnd.m_dDistMainRebarHor[nCycle][upper]		= pBri->m_footingStt.m_dDistMainRebarHor[nCycle][upper];
		pBri->m_footingEnd.m_setRebarDan[upper].m_dDia[nCycle][0]	= pBri->m_footingStt.m_setRebarDan[upper].m_dDia[nCycle][0];
	}

	SetGridData();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}



void CInputRebarMain2::OnButtonRebardan() 
{
	// TODO: Add your control notification handler code here
	long nCycle	= GetCurCycle();
	CRebarDanDlg dlg(m_pIn->GetRcBridgeRebar(), m_pIn->m_pARcBridgeDataStd, nCycle/*m_nTabCycle*/);
	if(m_nTab == m_nTabUpperSlab) dlg.m_nTypeTab	= 0;
	else if(m_nTab == m_nTabOutWall) dlg.m_nTypeTab	= 1;
	else if(m_nTab == m_nTabInWall) dlg.m_nTypeTab	= 2;
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

void CInputRebarMain2::OnCheckJoint() 
{
	// TODO: Add your control notification handler code here

	DrawInputDomyunView(FALSE);
}

void CInputRebarMain2::OnCheckRebarLength() 
{
	// TODO: Add your control notification handler code here
	DrawInputDomyunView(FALSE);
	
}

void CInputRebarMain2::SetGridData_ArchRib(long nCycle)
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
// 	long nNum		= 1;
// 	long nCountJoint	= 0;
//	long nType			= MARK_ARCHRIB_UPPER;

	// 상하면
	for(long upper = 0; upper < 2; upper++)
	{
		BOOL bUpper	= upper == iUPPER;
		sUpper	= bUpper ? "배선" : "복선";
		long nType	= bUpper ? MARK_ARCHRIB_UPPER : MARK_ARCHRIB_LOWER;
		long nNum		= 1;

		pBri->GetMarkRebarMain(nType, TRUE, nCycle, TRUE, TRUE, _T(""), 0);

		// 이음 간격
		long nCountJoint	= pBri->m_nCountJointMainRebar_ArchRib_Cycle2[upper][nDan];
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
				m_Grid.AddCell(row+3, col, &pBri->m_dArrExistRib[nCycle][upper][nDan], i, nFormat, nLine, "", CELL_TYPE_CHECK);
				col++;
			}
		}
	}
}

void CInputRebarMain2::OnSelchangeComboDan() 
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


void CInputRebarMain2::OnCheck2dan() 
{
	// TODO: Add your control notification handler code here
	SetGridData();
	SetDataInit();
}

BOOL CInputRebarMain2::IsSet2Dan()
{
	return m_check2Dan.GetCheck();
}

void CInputRebarMain2::SetGridData_OutWall2Dan(long nCycle)
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

void CInputRebarMain2::OnCheckViewCycle1() 
{
	// TODO: Add your control notification handler code here
	DrawInputDomyunView(FALSE);	
}

void CInputRebarMain2::OnCheckBending() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	pBri->m_bBendingDanBuRebar	= m_checkBending.GetCheck();

	DrawInputDomyunView(FALSE);
}

void CInputRebarMain2::SetGridData_LowerSlab( long nCycle, CGridCtrlMng *pGrid/*=NULL*/, long nDan/*=0*/ )
{
	// 그리드 초기화
	if(!pGrid)
		pGrid	= &m_Grid;
	pGrid->SetGridDefault(2, 2, 2, 0);
	pGrid->SetCellSize(20, 50);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long row		= 0, col = 0;
	CString str		= _T("");
	CString sHelp	= _T("");
	CString sUpdn	= _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;
// 	long nNum		= 1;


	// 상부슬래브 상,하면 구간별 이음 간격/철근지름
	for(long upper = 0; upper < 2; upper++)
	{
		BOOL bUpper	= upper == iUPPER;
		long nType	= bUpper ? MARK_LOWERSLAB_UPPER : MARK_LOWERSLAB_LOWER;
		pBri->GetMarkRebarMain(nType, TRUE, nCycle, TRUE, TRUE, _T(""), 0);

		// 상부 슬래브 상면 각 구간별 이음 간격
		sUpdn		= bUpper ? "상면 " : "하면 ";
		long nNum		= 1;
		long n = 0; for(n = 0; n < pBri->m_nQtyJigan; n++)
		{
			long nCountJoint	= nCycle == 0 || nCycle == 2 ? pBri->m_nCountJointMainRebar_LowerSlab_Cycle1[n][upper][nDan] : pBri->m_nCountJointMainRebar_LowerSlab_Cycle2[n][upper][nDan];
			long i = 0; for(i = 0; i < nCountJoint; i++)
			{
				if(upper == iUPPER) str.Format("UD%d", nNum++);
				else str.Format("LD%d", nNum++);

				if(i > 0) sHelp.Format("%d번째 지간의 %d번째 이음과 %d번째 이음과의 간격", n+1, i, i+1);
				else sHelp.Format("%d번째 지간의 시작위치와 %d번째 이음과의 간격", n+1, i+1);
				sHelp = sUpdn + sHelp;

				pGrid->AddCell(row, col, sUpdn);
				pGrid->AddCell(row+1, col, str);
				pGrid->AddCell(row+2, col, &pBri->m_pArrDistLowerSlab[nCycle][upper][nDan], n, i, nFormat, nLine, sHelp); 
				pGrid->SetCellDec(CELL_DEC_0);
				pGrid->SetCellPrintZero(TRUE);
				if(!bUpper) pGrid->SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
				col++;
			}
		}

		// 하부슬래브 각 구간별 철근 지름
		// 각 구간별 이음 간격 개수보다 1개 많다.
		// 그리고, 신축이음 개수만큼 추가된다.
		if(nCycle == 0 || nCycle==1)
		{
			for(n = 0; n < pBri->m_nQtyJigan; n++)
			{
				long nAddExpJoint	= pBri->m_bExpJoint[n+1] ? 1 : 0;
				if(n == pBri->m_nQtyJigan-1)
					nAddExpJoint	= 0;
				long nCountJoint	= nCycle == 0 || nCycle == 2 ? pBri->m_nCountJointMainRebar_LowerSlab_Cycle1[n][upper][nDan] : pBri->m_nCountJointMainRebar_LowerSlab_Cycle2[n][upper][nDan];
				long i = 0; for(i = 0; i < nCountJoint + nAddExpJoint; i++)
				{
					str	= pBri->GetMarkRebarMain(nType, TRUE, nCycle, FALSE, TRUE, _T(""), 0);

					sHelp.Format("%d번째 지간의 %d번째 주철근 지름", n+1, i+1);
					sHelp = sUpdn + sHelp;

					pGrid->AddCell(row, col, sUpdn);
					pGrid->AddCell(row+1, col, str);
					pGrid->AddCell(row+2, col, &pBri->m_pArrDiaLowerSlab[nCycle][upper][nDan], n, i, nFormat, nLine, sHelp, CELL_TYPE_DIA);
					if(!bUpper) pGrid->SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
					pGrid->AddCell(row+3, col, &pBri->m_pArrExistLowerSlab[nCycle][upper][nDan], n, i, nFormat, nLine, "", CELL_TYPE_CHECK);
					col++;
				}

				if(n == pBri->m_nQtyJigan-1)
				{
					str	= pBri->GetMarkRebarMain(nType, TRUE, nCycle, FALSE, TRUE, _T(""), 0);

					sHelp.Format("%d번째 지간의 %d번째 주철근 지름", n+1, i+1);
					sHelp = sUpdn + sHelp;

					pGrid->AddCell(row, col, sUpdn);
					pGrid->AddCell(row+1, col, str);
					pGrid->AddCell(row+2, col, &pBri->m_pArrDiaLowerSlab[nCycle][upper][nDan], n, i, nFormat, nLine, sHelp, CELL_TYPE_DIA);
					if(!bUpper) pGrid->SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
					pGrid->AddCell(row+3, col, &pBri->m_pArrExistLowerSlab[nCycle][upper][nDan], n, i, nFormat, nLine, "", CELL_TYPE_CHECK);
					col++;
				}
			}
		}
	}
}
