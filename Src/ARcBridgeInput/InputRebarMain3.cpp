// InputRebarMain3.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "../ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "InputRebarMain3.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputRebarMain3 dialog


CInputRebarMain3::CInputRebarMain3(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputRebarMain3::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputRebarMain3)
	//}}AFX_DATA_INIT
	m_nTabHunch	= -1;
	m_nTabDanbu	= -1;
	m_nTabBracket	= -1;
	m_nTabBFooting	= -1;
	m_nTabStirrup	= -1;
	m_nTabObtuseAngle	= -1;
	m_nTabAcuteAngle	= -1;	
	m_nTab	= 0;

	m_pDlgCheck = new CInputCheckGrid;
	m_pDlgEdit  = new CInputCheckEdit;
}

CInputRebarMain3::~CInputRebarMain3()
{
	delete m_pDlgCheck;
	delete m_pDlgEdit;
}

void CInputRebarMain3::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputRebarMain3)
	DDX_Control(pDX, IDC_COMBO_OBTUSE, m_comboObtuse);
	DDX_Control(pDX, IDC_COMBO_DANBU, m_comboDanbu);
	DDX_Control(pDX, IDC_BUTTON_EXAMINE, m_btnExamine);
	DDX_Control(pDX, IDC_COMBO_CTC, m_comboCTC);
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_comboBridgeNo);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputRebarMain3, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputRebarMain3)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_BN_CLICKED(IDC_BUTTON_EXAMINE, OnButtonExamine)
	ON_CBN_SELCHANGE(IDC_COMBO_DANBU, OnSelchangeComboDanbu)
	ON_CBN_SELCHANGE(IDC_COMBO_OBTUSE, OnSelchangeComboObtuse)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW,  IDC_GRID, OnCellChangedRow)
	ON_BN_CLICKED(IDC_BUTTON_CORNER, &CInputRebarMain3::OnBnClickedButtonCorner)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputRebarMain3 message handlers
void CInputRebarMain3::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SetDataSave();
	
	return;
}

void CInputRebarMain3::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	SetFocus();
	m_nTab	= m_Tab.GetCurSel();
	SetGridData();
	SetDataInit();
	SetControl();
	if(m_nTab == m_nTabAcuteAngle || m_nTab == m_nTabObtuseAngle)
		m_Grid.SetFocusCell(1, 1);
	DrawInputDomyunView(TRUE);

	return;
}

void CInputRebarMain3::SetGridData()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	if(m_nTab == m_nTabBFooting)
	{
		m_Grid.InitGrid(pBri->GetValueFy(ePartLowerSlab));
	}
	else
	{
		m_Grid.InitGrid(pBri->GetValueFy(ePartUpperSlab));
	}
	if(m_nTab == m_nTabHunch) SetGridData_Hunch();
	else if(m_nTab == m_nTabDanbu) SetGridData_Danbu();
	else if(m_nTab == m_nTabBracket) SetGridData_Bracket();
	else if(m_nTab == m_nTabBFooting) SetGridData_BFooting();
	else if(m_nTab == m_nTabStirrup) SetGridData_Stirrup();
	else if(m_nTab == m_nTabObtuseAngle) SetGridData_ObtuseAngle();
	else if(m_nTab == m_nTabAcuteAngle) SetGridData_AcuteAngle();

	if(m_nTab == m_nTabObtuseAngle || m_nTab == m_nTabAcuteAngle)
		m_nRowFocusItemTitle	= 0;
	else
		m_nRowFocusItemTitle	= 1;
	m_Grid.SetRedraw(TRUE, TRUE);
}

void CInputRebarMain3::SetDataSave()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	m_Grid.UpdateData(TRUE);

	// 
	SyncRebar();

	// 상하행선 분리 적용
	m_pIn->m_pARcBridgeDataStd->CopyToCoupleBridge_RebarMain(m_pIn->m_nCurrentBridge);

	m_pIn->m_pDoc->SetModifiedFlag();

	if(m_pDlgCheck->GetSafeHwnd() && m_pDlgCheck->IsWindowVisible())
	{
		pBri->MakeRebarInfoCycleStd(TRUE);
		m_pDlgCheck->Check();
	}
	if(m_pDlgEdit->GetSafeHwnd() && m_pDlgEdit->IsWindowVisible())
	{
		pBri->MakeRebarInfoCycleStd(TRUE);
		m_pDlgEdit->Check();
	}

	if(m_nTab == m_nTabHunch)
	{
		// 슬래브 주철근과 배력근 좌표 초기화
		pBri->m_bCaledRebarMain_Plane	= FALSE;
		pBri->m_bCaledRebarSupport_Plane	= FALSE;
	}	
}

void CInputRebarMain3::SetDataDefault()
{
	for(long bri = 0; bri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(bri);
		if(!pBri) return;

		BOOL bFirst   = IsFirst(bri);
		pBri->SetDefaultedState(GetIDD(), 0x00000001);
		if(!bFirst && !IsClickDefaultButton(bri)) continue;

		if(bFirst)
		{
			pBri->SetDataDefaultRebarHunch();
			pBri->SetDataDefaultRebarDanbu();
			pBri->SetDataDefaultRebarBracket();
			pBri->SetDataDefaultRebarObtuseAngle();
		}
		else
		{
			if(m_nTab == m_nTabHunch) pBri->SetDataDefaultRebarHunch();
			else if(m_nTab == m_nTabDanbu) pBri->SetDataDefaultRebarDanbu();
			else if(m_nTab == m_nTabBracket) pBri->SetDataDefaultRebarBracket();
			else if(m_nTab == m_nTabBFooting) pBri->SetDataDefaultRebarBFooting();
			else if(m_nTab == m_nTabObtuseAngle) pBri->SetDataDefaultRebarObtuseAngle();
			else if(m_nTab == m_nTabAcuteAngle) pBri->setDataDefaultRebarAcuteAngle();
			
			// 상하행선 분리 적용
			m_pIn->m_pARcBridgeDataStd->CopyToCoupleBridge_RebarMain(m_pIn->m_nCurrentBridge);
		}

		if(m_nTab == m_nTabHunch)
		{
			// 슬래브 주철근과 배력근 좌표 초기화
			pBri->m_bCaledRebarMain_Plane	= FALSE;
			pBri->m_bCaledRebarSupport_Plane	= FALSE;
		}
	}

	SetGridData();
	SetDataInit();
}

void CInputRebarMain3::InitTabCtrl()
{
	m_Tab.RemoveAllTabs();
	m_Tab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_Tab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	m_nTabHunch	= -1;
	m_nTabDanbu	= -1;
	m_nTabBracket	= -1;
	m_nTabBFooting	= -1;
	m_nTabStirrup	= -1;
	m_nTabObtuseAngle	= -1;
	m_nTabAcuteAngle	= -1;
	
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	CStringArray sArrTabName;
	long nTab		= 0;
	sArrTabName.Add("헌치부 철근"); m_nTabHunch	= nTab++;
	sArrTabName.Add("단 절점부 철근"); m_nTabDanbu	= nTab++;
	if(pBri->IsBracket(TRUE) || pBri->IsBracket(FALSE))
	{
		sArrTabName.Add("브래킷 철근"); m_nTabBracket	= nTab++;
	}
	if(pBri->m_bExistProtectionRiseFooting)
	{
		sArrTabName.Add("부상방지저판 철근"); m_nTabBFooting = nTab++;
	}
	sArrTabName.Add("단 절점부 스터럽 철근"); m_nTabStirrup	= nTab++;
	sArrTabName.Add("둔각부 보강 철근"); m_nTabObtuseAngle = nTab++;
	if(pBri->IsBoxType() == FALSE)
		sArrTabName.Add("벽체 예각부 보강 철근"); m_nTabAcuteAngle = nTab++;
	
	long i = 0; for(i = 0; i < sArrTabName.GetSize(); i++)
	m_Tab.InsertItem(i, sArrTabName[i]);

	if(m_Tab.GetItemCount() <= m_nTab) m_nTab = 0;
	m_Tab.SetCurSel(m_nTab);
}

void CInputRebarMain3::OnPreInitDialog()
{
	// 시작하면서 pick box의 크기를 정해줌
	if(m_pView) 
	{
		CTwinVector tv;
		tv.SetPickBoxSize(m_pView->GetSelectionRectSize()/m_pView->GetRatioWindow());
	}
	
	InitTabCtrl();
	InitComboCtrl();


	SetResize(IDC_BUTTON_EXAMINE,		SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
	SetResize(IDC_TAB,					SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID,					SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_COMBO_DANBU,			SZ_TOP_LEFT,		SZ_TOP_LEFT);
	SetResize(IDC_COMBO_OBTUSE,			SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_CORNER,		SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);

	SetHyperHelp(m_pIn->GetManualPath());

	SetGridData();
}

BOOL CInputRebarMain3::PreTranslateMessage(MSG* pMsg) 
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

void CInputRebarMain3::InitComboCtrl()
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

void CInputRebarMain3::OnSelchangeComboBridgeNo() 
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

void CInputRebarMain3::DrawInputDomyunView(BOOL bZoomAll)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(m_pIn->GetScaleInputDialog());

	CDomyun Dom(pDom);
	CARcBridgeDBStandard DBStd(m_pIn->GetRcBridgeRebar(), m_pIn->m_pARoadOptionStd, m_pIn->m_pARcBridgeDataStd);

	long nJ	= GetJijum();
	long nCycle = GetCycle();
	long nSlab = GetSlab();
	BOOL bDimUpperSlab	= FALSE;
	BOOL bDimOutWall	= FALSE;
	BOOL bDimInWall		= FALSE;
	BOOL bDimFooting	= FALSE;
	BOOL bDimDanbu		= m_nTab == m_nTabDanbu;
	BOOL bDimHunch		= m_nTab == m_nTabHunch;
	BOOL bDimBracket	= m_nTab == m_nTabBracket;
	BOOL bDimStirrup	= m_nTab == m_nTabStirrup;
	CDRect rectVertSection	= CDRect(0, 0, 0, 0);

	// 말뚝 비율을 기본값으로
	pBri->m_footingEnd.m_exFooting.m_Pile.InitScaleCutting();

	// 모든 객체들의 동기화를 해줌.
	BOOL bVertDir	= pBri->IsVertDir(); //TRUE;
	//pBri->SyncBridge(0, !bVertDir);
	pBri->GetTvVertSection_Std(0, !bVertDir, FALSE);
	pBri->SyncBridgeRebar(TRUE, bVertDir ? -1 : 1, TRUE);
	
	if(m_nTab == m_nTabAcuteAngle)
	{
		CCellID cell   = m_Grid.GetFocusCell();
		CString sTitle	= m_Grid.GetTextMatrix(cell.row, 0);
		BOOL bStt	= sTitle.Find("시점", 0) != -1;

		DBStd.m_pDrawVertSection->DrawRebar_OutWall_VertStd(&Dom, bStt, FALSE, FALSE, TRUE, 0, TRUE);

		Dom.SetCalcExtRect();
		rectVertSection	= Dom.GetExtRect();
	}
	else if(m_nTab == m_nTabBFooting)
	{
		DBStd.m_pDrawVertSection->DrawBFootingSection(&Dom, TRUE);
		DBStd.m_pDrawVertSection->DrawBFootingRebar(&Dom, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE);
	}
	else
	{

		// 종단면 그림
		DBStd.m_pDrawVertSection->DrawVertSectionAll(&Dom, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE);

		Dom.SetCalcExtRect();
		rectVertSection	= Dom.GetExtRect();

		//위치 표시 원
		long nPosSlab = -1;
		long nJ = 0;
		if(m_nTab == m_nTabObtuseAngle)
		{
			CCellID cell		= m_Grid.GetFocusCell();
			CString sTitle		= m_Grid.GetTextMatrix(cell.row, pBri->IsBoxType()? 1: 0);
			CString sUpper	= pBri->IsBoxType()? m_Grid.GetTextMatrix(cell.row, 0) : _T("");

			BOOL bStt	= sTitle.Find("시점", 0) != -1;
						
			nPosSlab = (sUpper.Find(_T("하부"))>-1 && pBri->IsBoxType())? iLOWER : iUPPER;
			nJ = bStt? 0 : pBri->GetCountJijum()-1;
		}
		else
		{
			nPosSlab= nSlab;
		}
		DBStd.m_pDrawVertSection->DrawSignHunchByJijum(&Dom, nJ, TRUE, nPosSlab == iUPPER);

		// 철근 그리기(치수, 이음 포함)
		long nCountWall	= pBri->GetCountInWall();

		long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_UpperSlab(&Dom, nCycle, dan, TRUE, FALSE, bDimUpperSlab);
			if(pBri->IsBoxType())
				DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_LowerSlab(&Dom, nCycle, dan, TRUE, FALSE, bDimUpperSlab);
			DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_OutWall(&Dom, nCycle, dan, TRUE, FALSE, bDimOutWall);
			DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_OutWall_Hunch(&Dom, nCycle, TRUE);
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
				DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_ArchRib(&Dom, nCycle, dan, TRUE, FALSE, FALSE, 0);

			// 시종점 기초
			for(long updn = 0; updn < 2; updn++)
			{
				DBStd.m_pDrawFooting->DrawRebarMain_Front(&Dom, &pBri->m_footingStt, nCycle, dan, updn, FALSE, bDimFooting);
				DBStd.m_pDrawFooting->DrawRebarMain_Front(&Dom, &pBri->m_footingEnd, nCycle, dan, updn, FALSE, bDimFooting);
			}

			// 벽체들 철근 그리기.(벽체의 기초 포함)
			long wall = 0; for(wall = 0; wall < nCountWall; wall++)
			{
				CWallApp *pWall	= pBri->GetInWall(wall);
				if(pWall)
				{
					for(long left = 0; left < 2; left++)
					{
						DBStd.m_pDrawWall->DrawRebarMain_Front(&Dom, pWall, nCycle, dan, left, bDimInWall, bDimInWall);
					}
					for(long updn = 0; updn < 2; updn++)
					{
						DBStd.m_pDrawFooting->DrawRebarMain_Front(&Dom, &pWall->m_footing, nCycle, dan, updn, bDimFooting && dan==0, bDimFooting);
					}
				}
			}

			// 거더 철근
			long nJ = 0; for(nJ = 0; nJ < pBri->GetCountJijum(); nJ++)
			{
				DBStd.m_pDrawVertSection->DrawRebarAll_SlabGirder(&Dom, nJ, FALSE, FALSE, 0, FALSE, TRUE, TRUE, TRUE, TRUE, FALSE, -1, TRUE);
			}
		}
		//헌치
		DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_UpperSlabHunch(&Dom, nCycle, bDimHunch);
		DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_LowerSlabHunch(&Dom, nCycle, bDimHunch);

		//단절점부
		DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_Danbu(&Dom, FALSE, bDimDanbu, TRUE, nCycle);

		//브래킷, 단절점부 스터럽
		for(long stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_Bracket(&Dom, bStt, FALSE, bDimBracket, TRUE, 0, CDRect(0, 0, 0, 0), nCycle);
			DBStd.m_pDrawVertSection->DrawRebarShear_VertSection_UpperSlabDanbu(&Dom, nCycle, bStt, bDimStirrup, FALSE, TRUE);
			DBStd.m_pDrawVertSection->DrawRebarShear_VertSection_LowerSlabDanbu(&Dom, nCycle, bStt, bDimStirrup, FALSE, TRUE);
		}
	}
	
	// 둔각부 보강일 때 둔각부 보강 철근 그려줌
	if(m_nTab == m_nTabObtuseAngle)
	{
		BOOL bBox	= pBri->IsBoxType();

		CCellID cell		= m_Grid.GetFocusCell();
		CString sTitle		= m_Grid.GetTextMatrix(cell.row, bBox? 1: 0);
		CString sUpper	= bBox? m_Grid.GetTextMatrix(cell.row, 0) : _T("");
		
		BOOL bStt	= sTitle.Find("시점", 0) != -1;
		BOOL bLeft	= sTitle.Find("좌측", 0) != -1;

		BOOL bUpperSlab = (bBox && sUpper.Find(_T("하부슬래브"))>-1)? FALSE : TRUE;

		DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_SlabObtuse(&Dom, bUpperSlab, bStt, bLeft, TRUE, TRUE, TRUE);
	}

	if(m_nTab == m_nTabAcuteAngle)
	{
		CCellID cell		= m_Grid.GetFocusCell();
		CString sTitle	= m_Grid.GetTextMatrix(cell.row, 0);
		
		BOOL bStt	= sTitle.Find("시점", 0) != -1;
		BOOL bLeft	= sTitle.Find("좌측", 0) != -1;

		CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;
		BOOL bFront	= bStt ? bLeft : !bLeft;

		DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_OutWallAcute(&Dom, bStt, bLeft, TRUE, FALSE, TRUE);
		DBStd.m_pDrawFooting->DrawRebarMain_Front_Obtuse(&Dom, pFooting, bFront, TRUE, FALSE, TRUE);

		DBStd.m_pDrawVertSection->DrawRebarSupport_VertSection_OutWallAcuteAngle(&Dom, bStt, bLeft, TRUE, TRUE, TRUE, 0);
		DBStd.m_pDrawFooting->DrawRebarSupport_FrontObtuse(&Dom, pFooting, bFront, TRUE, TRUE, 0, TRUE);
	}

	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();
	m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_MAIN_TITLE);

	CString sCycle	= _T("");
	sCycle.Format("%d CYCLE", nCycle+1);
	//Dom.TextOut(CDPoint(rect.left+rect.Width()/2, rect.bottom+Dom.GetTextHeight()*2), sCycle);

	// 지점별 슈 위치 표시(삼각형)
	for(nJ = 0; nJ < pBri->GetCountJijum(); nJ++)
		DBStd.m_pDrawVertSection->DrawJijumSign(&Dom, nJ);

	Dom.SetCalcExtRect();
	rect	= Dom.GetExtRect();

	*pDom << Dom;

	// 둔각부 보강입력중일땐 둔각부 표현
	if(m_nTab == m_nTabObtuseAngle)
	{
		CCellID cell			= m_Grid.GetFocusCell();
		cell.row				= cell.row == -1 ? 1 : cell.row;
		cell.col					= cell.col == -1 ? 1 : cell.col;

		m_Grid.SetFocusCell(cell.row, cell.col);

		CString sTitle			= m_Grid.GetTextMatrix(cell.row, pBri->IsBoxType()? 1: 0);
		CString sUpper		= pBri->IsBoxType()? m_Grid.GetTextMatrix(cell.row, 0) : _T("");
		BOOL bUpperSlab	= (pBri->IsBoxType() && sUpper.Find(_T("하부슬래브"))>-1)? FALSE : TRUE;
		
		BOOL bStt	= sTitle.Find("시점", 0) != -1;
		BOOL bLeft	= sTitle.Find("좌측", 0) != -1;
		
		BOOL bDrawPlanObtuse = FALSE;
		if(bUpperSlab)
			bDrawPlanObtuse = pBri->m_bIsObtuseAngle_UpperSlab[bStt ? iSTT : iEND][bLeft ? iLEFT : iRIGHT];
		else
			bDrawPlanObtuse = pBri->m_bIsObtuseAngle_LowerSlab[bStt ? iSTT : iEND][bLeft ? iLEFT : iRIGHT];

		if(bDrawPlanObtuse)
		{
			// 상면
			DBStd.m_pDrawPlane->DrawPlaneGenObtuse(&Dom, bStt, bLeft, TRUE, !bUpperSlab);
			DBStd.m_pDrawPlane->DrawRebarPlane_Slab_Obtuse(&Dom, bUpperSlab, bStt, bLeft, TRUE, TRUE, TRUE, TRUE);
			
			Dom.SetCalcExtRect();
			CDRect rectPlane	= Dom.GetExtRect();

			sTitle	= m_Grid.GetTextMatrix(cell.row, 0) + "(상면)";
			Dom.TextOut(CDPoint(rectPlane.left+rectPlane.Width()/2, rectPlane.bottom+Dom.GetTextHeight()*2), sTitle);
			
			CDPoint xyMove	= CDPoint(0, 0);
			xyMove.x	= (rect.left+rect.Width()/2) - rectPlane.right - Dom.Always(30);
			xyMove.y	= rect.bottom - rectPlane.top + Dom.Always(30);

			Dom.Move(xyMove);
			*pDom << Dom;

			// 하면
			DBStd.m_pDrawPlane->DrawPlaneGenObtuse(&Dom, bStt, bLeft, FALSE, !bUpperSlab);
			DBStd.m_pDrawPlane->DrawRebarPlane_Slab_Obtuse(&Dom, bUpperSlab, bStt, bLeft, FALSE, TRUE, TRUE, TRUE);
			
			Dom.SetCalcExtRect();
			rectPlane	= Dom.GetExtRect();

			sTitle	= m_Grid.GetTextMatrix(cell.row, 0) + "(하면)";
			Dom.TextOut(CDPoint(rectPlane.left+rectPlane.Width()/2, rectPlane.bottom+Dom.GetTextHeight()*2), sTitle);
			
			xyMove.x	= (rect.left+rect.Width()/2) - rectPlane.left + Dom.Always(30);
			xyMove.y	= rect.bottom - rectPlane.top + Dom.Always(30);

			Dom.Move(xyMove);
			*pDom << Dom;
		}

	}
	// 기초 평면 표현
	else if(m_nTab == m_nTabAcuteAngle)
	{
		CCellID cell	= m_Grid.GetFocusCell();
		CString sTitle	= m_Grid.GetTextMatrix(cell.row, 0);
		
		BOOL bStt	= sTitle.Find("시점", 0) != -1;
		BOOL bLeft	= sTitle.Find("좌측", 0) != -1;
		
		// 기초평면은 종단면 기초 아래쪽에
		*pDom << Dom;

		CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;
		BOOL bFront	= bStt ? bLeft : !bLeft;

		long nDrawUpper	= 0;
		BOOL bDimForTopIsRight	= FALSE;//bStt ? FALSE : TRUE;
		long nRotate	= bStt ? 1 : -1;
		CDRect rectPlane	= DBStd.m_pDrawFooting->DrawRebar_Plane_ObtuseStd(&Dom, pFooting, bFront, TRUE, TRUE, FALSE, nDrawUpper, bDimForTopIsRight, nRotate);
		Dom.TextOut(CDPoint(rectPlane.left+rectPlane.Width()/2, rectPlane.bottom+Dom.GetTextHeight()*2), sTitle+"(기초평면)");
		CDPoint xyMove	= CDPoint(0, 0);
		if(bStt)
		{
			xyMove.x	= rectVertSection.left - rectPlane.left;
			xyMove.y	= rectVertSection.top - rectPlane.bottom - Dom.Always(60);
		}
		else
		{
			xyMove.x	= rectVertSection.right - rectPlane.right;
			xyMove.y	= rectVertSection.top - rectPlane.bottom - Dom.Always(60);
		}
		Dom.Move(xyMove);
		*pDom << Dom;

		// 벽체는 시종점 좌우측에
		CDRect rectFront	= DBStd.m_pDrawCrossSection->DrawRebarCrossSection_AcuteStd(&Dom, bStt, bLeft, TRUE, TRUE, TRUE);
		Dom.TextOut(CDPoint(rectFront.left+rectFront.Width()/2, rectFront.bottom+Dom.GetTextHeight()*2), sTitle+"(벽체정면)");
		if(bStt)
		{
			xyMove.x	= rectVertSection.left - rectFront.right - Dom.Always(40);
			xyMove.y	= rectVertSection.top - rectFront.top;
		}
		else
		{
			xyMove.x	= rectVertSection.right - rectFront.left + Dom.Always(40);
			xyMove.y	= rectVertSection.top - rectFront.top;
		}
		Dom.Move(xyMove);
				
		*pDom << Dom;
	}
	
	if(m_nTab == m_nTabAcuteAngle)
	{
		CCellID cell	= m_Grid.GetFocusCell();
		CString sTitle	= m_Grid.GetTextMatrix(cell.row, 0);
		
		BOOL bStt	= sTitle.Find("시점", 0) != -1;
		BOOL bLeft	= sTitle.Find("좌측", 0) != -1;
		
		DBStd.m_pDrawPlane->DrawPlanePosSign(&Dom, bStt, bLeft);
		Dom.SetCalcExtRect();
		CDRect rectPos	= Dom.GetExtRect();

		if(bStt)
		{
			Dom.Move(CDPoint(rectVertSection.right - rectPos.left + Dom.Always(50), rectVertSection.top - rectPos.top));
		}
		else
		{
			Dom.Move(CDPoint(rectVertSection.left - rectPos.right - Dom.Always(50), rectVertSection.top - rectPos.top));
		}

		*pDom << Dom;
	}
	pBri->GetTvVertSection_Std(0, !bVertDir, FALSE, TRUE);
	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

void CInputRebarMain3::SetDataInit()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	m_Grid.UpdateData(FALSE);

	long nCols	= m_Grid.GetCols();
	long nRows	= m_Grid.GetRows();

	// 단부, 브라켓 일 경우 merge
	if(m_nTab == m_nTabDanbu)
	{
		m_Grid.MergeGrid(0, 0, 1, nCols-1);
		m_Grid.SetMergeRow(0, 3, 0);
		m_Grid.SetMergeCol(1, 1, 2);
		m_Grid.SetMergeCol(1, 3, 4);
		if(pBri->IsBoxType())
		{
			m_Grid.SetMergeCol(1, 5, 6);
			m_Grid.SetMergeCol(1, 7, 8);
		}
	}
	else if(m_nTab == m_nTabBracket)
	{
		m_Grid.SetMergeRow(0, 2, 0);
		m_Grid.SetMergeCol(0, 1, 2);
		m_Grid.SetMergeCol(0, 3, 4);
	}
	else if(m_nTab == m_nTabBFooting)
	{
		m_Grid.MergeGrid(0, 0, 0, nCols-1);
	}
	else if(m_nTab == m_nTabStirrup)
	{
		m_Grid.SetMergeCol(0, 0, 5);
		m_Grid.SetMergeCol(1, 0, 2);
		m_Grid.SetMergeCol(1, 3, 5);
		if(pBri->IsBoxType())
		{
			m_Grid.SetMergeCol(0, 6, 11);
			m_Grid.SetMergeCol(1, 6, 8);
			m_Grid.SetMergeCol(1, 9, 11);
		}
	}
	else if(m_nTab == m_nTabObtuseAngle)
	{
		m_Grid.MergeGrid(0, 0, 0, nCols-1);
		if(pBri->IsBoxType())
			m_Grid.MergeGridEx(0, nRows-1, 0, 0, FALSE, TRUE, DT_CENTER | DT_VCENTER | DT_SINGLELINE);			
		m_Grid.MergeGrid(0, 1, 0, 1);
	}
	else if(m_nTab == m_nTabHunch)
	{
		m_Grid.SetMergeRow(0, 1, 0);
		m_Grid.MergeGrid(0, 1, 1, nCols-1);
	}
}


void CInputRebarMain3::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	m_CellID	= m_Grid.GetFocusCell();
	structGridCell *pCell = m_Grid.GetCurrentCell();
	if(pCell)
	{
		m_pIn->SetComment(pCell->sTitle);
		
		m_pView->GetDomyun()->STMakeCursor(m_Grid.GetItemText(m_nRowFocusItemTitle,pCell->nCol));
	}
	DrawInputDomyunView(FALSE);
}

void CInputRebarMain3::SetControl()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	CHgBaseConcStd baseConc;

	m_comboCTC.SetCurSel(baseConc.GetIdxByRebarCTC(pBri->m_dCTC_MainRebar));
	m_comboObtuse.SetCurSel(0);
	m_comboDanbu.SetCurSel(pBri->m_nType_DanbuRebar);
	

	m_comboCTC.EnableWindow(FALSE);
	m_comboDanbu.EnableWindow(m_nTab == m_nTabDanbu);
	m_comboObtuse.ShowWindow(m_nTab == m_nTabObtuseAngle);
	m_comboObtuse.EnableWindow(m_nTab == m_nTabObtuseAngle);
	m_btnExamine.EnableWindow(m_bDesignLsd == FALSE);
}

void CInputRebarMain3::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	CCellID cell		= m_Grid.GetFocusCell();

	SetDataSave();
	if(m_nTab != m_nTabObtuseAngle && m_nTab != m_nTabAcuteAngle)
	{
		SetGridData();
		SetDataInit();
	}
	//DrawInputDomyunView(FALSE);

	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);
}

void CInputRebarMain3::OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result)
{
}

void CInputRebarMain3::SetGridData_Hunch()
{
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 2, 1);
	m_Grid.SetCellSize(20, -1);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long row = 0, col = 0;
	CString str = _T(""), sHelp = _T("");
	CStringArray sArr;
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	
	long nCountCycle	= pBri->GetCountCycleMainRebar();	// 선택된 사이클 수
	long nCountDia		= pBri->m_nQtyJigan+1;
	long nUpperColCount = 0;
	long nCountSlab = 1; //보통은 상부슬래브만.
	if(pBri->IsBoxType()) nCountSlab = 2; //Box형은 하부슬래브까지.
	
	m_Grid.AddCell(row, col, _T("구분"));	
	m_Grid.AddCell(row+1, col++, _T("구분"));	
	
	for(long ix = iUPPER ; ix < nCountSlab; ix++)
	{
		CString strSlab = (ix == iUPPER) ? _T("상부슬래브") : _T("하부슬래브");

		long cycle = 0; for(cycle = 0; cycle < nCountCycle; cycle++)
		{
			m_Grid.AddCell(row, col, strSlab);
			str.Format("%dCYCLE", cycle+1);
			m_Grid.AddCell(row+1, col++, str);
		}

		m_Grid.AddCell(row, col, strSlab);
		m_Grid.AddCell(row+1, col++, "철근타입");

		row	= 1;
		long nIdxWall		= 1;
		CDoubleArray *pdArr = NULL;
		long i = 0; for(i = 0; i < nCountDia; i++)
		{
			row++;
			col = nUpperColCount;
			long nJ	= i;

			if(i == 0)
			{
				str = "시점벽체";
			}
			else if(i == nCountDia-1) 
			{
				str = "종점벽체";
			}
			else 
			{
				str.Format("중간지점%d", nIdxWall++);
			}

			if(ix == iUPPER)
			{
				m_Grid.AddCell(row, col++, str);
			}
			
			for(cycle = 0; cycle < nCountCycle; cycle++)
			{
				if(ix == iUPPER)
					pdArr	= &pBri->m_dArrDiaUpperSlabHunch[cycle];
				else
					pdArr	= &pBri->m_dArrDiaLowerSlabHunch[cycle];

				m_Grid.AddCell(row, col++, pdArr, i, nFormat, nLine, sHelp, CELL_TYPE_DIA);
				m_Grid.SetLastCellFy(pBri->GetValueFy(ix == iUPPER? ePartUpperSlab : ePartLowerSlab));
			}

			// 헌치 철근 타입
			// 기둥식이면 무조건 1로 고정, 벽식이면 0 또는 1
			// 헌치에 R이 있다면 무조건 0으로 고정
			CWallApp *pWall	= pBri->GetInWall(nIdxWall-2);
			if(i > 0 && i < nCountDia-1 && pWall)
			{
				if(pWall->m_bTopIsColumn)
				{
					m_Grid.AddCell(row, col++, "V형", nFormat, CELL_READONLY);
				}
				else
				{
					BOOL bRound	= pBri->IsRoundHunch(nJ, TRUE, ix == iUPPER);
					BOOL bXType = !pBri->IsHunch(nJ, ix == iUPPER);

					sArr.RemoveAll();
					sArr.Add("X형");
					sArr.Add("V형");

					if(bRound)
					{
						long nSize	= pBri->m_dArrTypeUpperSlabHunch.GetSize();
						if(nSize > i)
							pBri->m_dArrTypeUpperSlabHunch.SetAt(i, 0);
					}
					//헌치 없는데 직경이있는경우 X형만 지원
					if(bXType)
					{
						if(ix == iUPPER)
						{
							long nSize	= pBri->m_dArrTypeUpperSlabHunch.GetSize();
							if(nSize > i)
								pBri->m_dArrTypeUpperSlabHunch.SetAt(i, 0);
						}
						else
						{
							long nSize	= pBri->m_dArrTypeLowerSlabHunch.GetSize();
							if(nSize > i)
								pBri->m_dArrTypeLowerSlabHunch.SetAt(i, 0);
						}
					}

					if(ix == iUPPER)
						m_Grid.AddCell(row, col++, &pBri->m_dArrTypeUpperSlabHunch, i, nFormat, bRound || bXType? CELL_READONLY : nLine, sHelp, CELL_TYPE_COMBO, sArr);
					else
						m_Grid.AddCell(row, col++, &pBri->m_dArrTypeLowerSlabHunch, i, nFormat, bRound || bXType? CELL_READONLY : nLine, sHelp, CELL_TYPE_COMBO, sArr);
				}
			}
			else
			{
				m_Grid.AddCell(row, col++, "", nFormat, CELL_READONLY);
			}
		}
		nUpperColCount = col;
		row = 0;
	}
	
	
	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(100);
	m_Grid.SetColumnWidth(0, 150);
}

void CInputRebarMain3::SetGridData_Danbu()
{
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 3, 1);
	m_Grid.SetCellSize(20, -1);

	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long row = 0, col = 0;
	CString str = _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	col	= 0;
	
	long nCountSlab = 1;
	if(pBri->IsBoxType()) nCountSlab = 2;

	for(long nSlab = iUPPER; nSlab < nCountSlab; nSlab++)
	{
		double dFy(0);
		if(nSlab == iUPPER)
		{
			m_Grid.AddCell(row, col+1, "상부슬래브");
			m_Grid.AddCell(row, col+2, "상부슬래브");
			m_Grid.AddCell(row, col+3, "상부슬래브");
			m_Grid.AddCell(row++, col+4, "상부슬래브");
		}
		else
		{
			m_Grid.AddCell(row, col, "하부슬래브");
			m_Grid.AddCell(row, col+1, "하부슬래브");
			m_Grid.AddCell(row, col+2, "하부슬래브");
			m_Grid.AddCell(row++, col+3, "하부슬래브");
			dFy = pBri->GetValueFy(ePartLowerSlab);
		}

		if(nSlab == iUPPER)
		{
			m_Grid.AddCell(row, col, "구분");
			m_Grid.AddCell(row+1, col, "구분");
			m_Grid.AddCell(row+2, col, "구분");
			m_Grid.AddCell(row+3, col++, "구분");
		}

		m_Grid.AddCell(row, col, "시점측", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+1, col, "CYCLE 1", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+2, col++, &pBri->m_bIsDanbuRebar_Cycle1[nSlab][iSTT], nFormat, nLine, " ", CELL_TYPE_CHECK);

		m_Grid.AddCell(row, col, "시점측", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+1, col, "CYCLE 2", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+2, col++, &pBri->m_bIsDanbuRebar_Cycle2[nSlab][iSTT], nFormat, nLine, " ", CELL_TYPE_CHECK);


		m_Grid.AddCell(row, col, "종점측", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+1, col, "CYCLE 1", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+2, col++, &pBri->m_bIsDanbuRebar_Cycle1[nSlab][iEND], nFormat, nLine, "  ", CELL_TYPE_CHECK);

		m_Grid.AddCell(row, col, "종점측", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+1, col, "CYCLE 2", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+2, col++, &pBri->m_bIsDanbuRebar_Cycle2[nSlab][iEND], nFormat, nLine, "  ", CELL_TYPE_CHECK);


		row+=3;

		CHgBaseConcStd baseConc;
		BOOL bSttCycle1	= pBri->m_bIsDanbuRebar_Cycle1[nSlab][iSTT];
		BOOL bEndCycle1	= pBri->m_bIsDanbuRebar_Cycle1[nSlab][iEND];
		BOOL bSttCycle2	= pBri->m_bIsDanbuRebar_Cycle2[nSlab][iSTT];
		BOOL bEndCycle2	= pBri->m_bIsDanbuRebar_Cycle2[nSlab][iEND];

		long dan = 0; for(dan = 0; dan < 3; dan++)
		{
			if(nSlab == iUPPER) col	= 0;
			else col = 5;

			str.Format("%d단", dan+1);
			if(nSlab == iUPPER)
				m_Grid.AddCell(row, col++, str);
			m_Grid.AddCell(row, col++, &pBri->m_dDia_DanbuRebar_Cycle1[nSlab][iSTT][dan], nFormat, bSttCycle1 ? nLine : CELL_READONLY, "", CELL_TYPE_DIA);
			if(nSlab == iLOWER)	m_Grid.SetLastCellFy(dFy);
			m_Grid.AddCell(row, col++, &pBri->m_dDia_DanbuRebar_Cycle2[nSlab][iSTT][dan], nFormat, bSttCycle2 ? nLine : CELL_READONLY, "", CELL_TYPE_DIA);
			if(nSlab == iLOWER)	m_Grid.SetLastCellFy(dFy);
			m_Grid.AddCell(row, col++, &pBri->m_dDia_DanbuRebar_Cycle1[nSlab][iEND][dan], nFormat, bEndCycle1 ? nLine : CELL_READONLY, "", CELL_TYPE_DIA);
			if(nSlab == iLOWER)	m_Grid.SetLastCellFy(dFy);
			m_Grid.AddCell(row, col++, &pBri->m_dDia_DanbuRebar_Cycle2[nSlab][iEND][dan], nFormat, bEndCycle2 ? nLine : CELL_READONLY, "", CELL_TYPE_DIA);
			if(nSlab == iLOWER)	m_Grid.SetLastCellFy(dFy);
			row++;
		}
		
		row = 0;
	}
	
	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidth(0, 60);
	m_Grid.SetColumnWidth(1, 100);
	m_Grid.SetColumnWidth(2, 100);
	
	if(pBri->IsBoxType())
	{
		m_Grid.SetColumnWidth(5, 100);
		m_Grid.SetColumnWidth(6, 100);
		m_Grid.SetColumnWidth(7, 100);
		m_Grid.SetColumnWidth(8, 100);
	}
}

// 하나 입력할때 동시에 다른 거에도 적용되어야 하는 철근들에 대해서 동기화해줌
void CInputRebarMain3::SyncRebar()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	pBri->SyncCycleMainRebar(1, 3);


	for (long nStt =0; nStt < 2; nStt++)
	{
		BOOL bStt = nStt==iSTT;

		CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;
		if(pFooting == NULL) continue;

		for (long nLeft =0; nLeft< 2; nLeft++)
		{
			BOOL bLeft = nLeft==iLEFT;

			pFooting->m_bIsObtuse[nLeft] = pBri->m_bIsAcuteAngle_OutWall[nStt][nLeft];
		}	

	}	

	//SetGridData();	// 요거를 하면 포커스가 사라짐.
	SetDataInit();
}

// 현재 셀이 선택되어 있는 곳의 지점
long CInputRebarMain3::GetJijum()
{
	long nJ	= 0;
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return 0;

	if(m_nTab == m_nTabHunch)
	{
		nJ	= m_CellID.row-2;
	}
	else if(m_nTab == m_nTabDanbu || m_nTab == m_nTabBracket)
	{
		nJ	= m_CellID.col < 3 ? 0 : pBri->m_nQtyJigan;
		if(pBri->IsBoxType())
		{
			if(m_CellID.col == 1 || m_CellID.col == 2
				 || m_CellID.col == 5 || m_CellID.col == 6)
				nJ = 0;
			else
				nJ = pBri->m_nQtyJigan;
		}
	}
	else if(m_nTab == m_nTabStirrup)
	{
		nJ	= m_CellID.col < 3 ? 0 : pBri->m_nQtyJigan;
		if(pBri->IsBoxType())
		{
			if(m_CellID.col == 0 || m_CellID.col == 1 || m_CellID.col == 2
				|| m_CellID.col == 6 || m_CellID.col == 7 || m_CellID.col == 8)
				nJ = 0;
			else
				nJ = pBri->m_nQtyJigan;
		}
	}
	else if(m_nTab == m_nTabObtuseAngle || m_nTab == m_nTabAcuteAngle)
	{
		nJ	= m_CellID.row < 3 ? 0 : pBri->m_nQtyJigan;
	}

	return nJ;
}

// 현재 셀이 선택되어 있는 곳의 사이클
long CInputRebarMain3::GetCycle()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();

	CCellID cell		= m_Grid.GetFocusCell();
	CString strTitle	= m_Grid.GetTextMatrix(1, cell.col);

	long nCycle	= 0;

	// 헌치부인경우 탭에 따라 다름
	// 단부 탭인 경우 무조건 1cycle
	long nCol	= m_CellID.col;
	if(m_nTab == m_nTabHunch)
	{
		if(pBri->IsBoxType()) //BOX면 하부슬래브까지 추가되서..
		{
			if(strTitle == "1CYCLE") nCycle = 0;
			if(strTitle == "2CYCLE") nCycle = 1;
			if(strTitle == "3CYCLE") nCycle = 2;
			if(strTitle == "4CYCLE") nCycle = 3;
		}
		else
		{
			if(nCol < m_Grid.GetCols()-1)
				nCycle	= nCol-1;
		}
	}
	else if(m_nTab == m_nTabStirrup)
	{
		nCycle	= 0;
	}
	else if(m_nTab == m_nTabObtuseAngle || m_nTab == m_nTabAcuteAngle)
	{
		nCycle	= 0;
	}
	else 
	{
		if(nCol == 1 || nCol == 3)
			nCycle	= 0;
		else if(nCol == 2 || nCol == 4)
			nCycle	= 1;

		if(pBri->IsBoxType())
		{
			if(nCol == 5 || nCol == 7)
				nCycle = 0;
			else if(nCol == 6 || nCol == 8)
				nCycle = 1;
		}
	}

	return nCycle;
}

long CInputRebarMain3::GetSlab()
{
	long nCol	= m_CellID.col;
	long bSlab = iUPPER;

	if(m_nTab == m_nTabHunch)
	{
		if(nCol >= 1 && nCol <= 3)
			bSlab = iUPPER;
		else if(nCol >=4 && nCol <= 6)
			bSlab = iLOWER;
	}
	else if(m_nTab == m_nTabDanbu)
	{
		if(nCol >= 1 && nCol <= 4)
			bSlab = iUPPER;
		else if(nCol >= 5 && nCol <= 8)
			bSlab = iLOWER;
	}
	else if(m_nTab == m_nTabStirrup)
	{
		if(nCol >= 0 && nCol <=5)
			bSlab = iUPPER;
		else if(nCol >= 6 && nCol <=11)
			bSlab = iLOWER;
	}
	return bSlab;
}

void CInputRebarMain3::OnButtonExamine() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	BOOL bIsBraket = (pBri->IsBracket(TRUE) || pBri->IsBracket(FALSE));

	if(pBri->IsOutsideWall()==FALSE)
	{
		AfxMessageBox("외측벽체가 없으므로 설계검토 항목이 없습니다.");
		return;
	}

	if(m_nTab==m_nTabDanbu) //단절점부철근
	{
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
		m_pDlgCheck->SetBriAndContent(pBri,IDD_REBAR_MAIN3,0);

		m_pDlgCheck->ShowAndCheck(this);	
		CString str = _T("");
		str.Format("기타 철근 검토");
		m_pDlgCheck->SetWindowText(str);
	}
	else if(m_nTab == m_nTabBracket) //브래킷 철근
	{
		if(m_pDlgEdit->GetSafeHwnd() && m_pDlgEdit->IsWindowVisible()) m_pDlgEdit->DestroyWindow();

		long nTypeCheck = 0;
		m_pDlgEdit->SetBriAndContent(pBri,IDD_REBAR_MAIN3,nTypeCheck);

		m_pDlgEdit->ShowAndCheck(this);	
		CString str = _T("");
		str.Format("브래킷 검토");
		m_pDlgEdit->SetWindowText(str);
	}
	else if(m_nTab == m_nTabStirrup) //단 절점부 스터럽 철근
	{
		BOOL bFemNormal = pBri->m_FemNormal.IsOutputLoaded();
		BOOL bFemEQ     = pBri->IsRcSlab() ? TRUE : (pBri->GetFemManage(COMBO_EARTHQUAKE)->IsOutputLoaded());
		
		if(!bFemNormal || (pBri->GetTypeDesingEQ() == 1 && !bFemEQ))
		{
			AfxMessageBox("모델링 해석을 수행후에 설계검토를 하시기 바랍니다.");
			return;
		}
		
		if(m_pDlgCheck->GetSafeHwnd() && m_pDlgCheck->IsWindowVisible()) m_pDlgCheck->DestroyWindow();
		m_pDlgCheck->SetBriAndContent(pBri,IDD_REBAR_MAIN3,1);
		
		m_pDlgCheck->ShowAndCheck(this);	
		CString str = _T("");
		str.Format("기타 철근 검토");
		m_pDlgCheck->SetWindowText(str);
	}
	else 
	{
		AfxMessageBox("설계검토 항목이 없습니다.");
	}
}

void CInputRebarMain3::SetGridData_Bracket()
{
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 2, 1);
	m_Grid.SetCellSize(20, -1);

	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long row = 0, col = 0;
	CString str = _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	col	= 0;
	m_Grid.AddCell(row, col, "구분");
	m_Grid.AddCell(row+1, col, "구분");
	m_Grid.AddCell(row+2, col++, "구분");
	
	
	m_Grid.AddCell(row, col, "시점", nFormat, CELL_READONLY);
	m_Grid.AddCell(row+1, col, "CYCLE 1", nFormat, CELL_READONLY);
	m_Grid.AddCell(row+2, col++, &pBri->m_bIsBracketRebar[0][iSTT], nFormat, nLine, " ", CELL_TYPE_CHECK);

	m_Grid.AddCell(row, col, "시점", nFormat, CELL_READONLY);
	m_Grid.AddCell(row+1, col, "CYCLE 2", nFormat, CELL_READONLY);
	m_Grid.AddCell(row+2, col++, &pBri->m_bIsBracketRebar[1][iSTT], nFormat, nLine, " ", CELL_TYPE_CHECK);
	
	
	m_Grid.AddCell(row, col, "종점", nFormat, CELL_READONLY);
	m_Grid.AddCell(row+1, col, "CYCLE 1", nFormat, CELL_READONLY);
	m_Grid.AddCell(row+2, col++, &pBri->m_bIsBracketRebar[0][iEND], nFormat, nLine, "  ", CELL_TYPE_CHECK);

	m_Grid.AddCell(row, col, "종점", nFormat, CELL_READONLY);
	m_Grid.AddCell(row+1, col, "CYCLE 2", nFormat, CELL_READONLY);
	m_Grid.AddCell(row+2, col, &pBri->m_bIsBracketRebar[1][iEND], nFormat, nLine, "  ", CELL_TYPE_CHECK);

	
	row+=3;

	CHgBaseConcStd baseConc;
	BOOL bSttCycle1	= pBri->m_bIsBracketRebar[0][iSTT];
	BOOL bEndCycle1	= pBri->m_bIsBracketRebar[0][iEND];
	BOOL bSttCycle2	= pBri->m_bIsBracketRebar[1][iSTT];
	BOOL bEndCycle2	= pBri->m_bIsBracketRebar[1][iEND];

	long dan	= 0;
	col	= 0;
	str.Format("%d단", dan+1);
	m_Grid.AddCell(row, col++, str);
	m_Grid.AddCell(row, col++, &pBri->m_dDiaRebarMain_Bracket_Cycle1[iSTT], nFormat, bSttCycle1 ? nLine : CELL_READONLY, "", CELL_TYPE_DIA);
	m_Grid.AddCell(row, col++, &pBri->m_dDiaRebarMain_Bracket_Cycle2[iSTT], nFormat, bSttCycle2 ? nLine : CELL_READONLY, "", CELL_TYPE_DIA);
	m_Grid.AddCell(row, col++, &pBri->m_dDiaRebarMain_Bracket_Cycle1[iEND], nFormat, bEndCycle1 ? nLine : CELL_READONLY, "", CELL_TYPE_DIA);
	m_Grid.AddCell(row, col++, &pBri->m_dDiaRebarMain_Bracket_Cycle2[iEND], nFormat, bEndCycle2 ? nLine : CELL_READONLY, "", CELL_TYPE_DIA);
	row++;

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidth(0, 60);
	m_Grid.SetColumnWidth(1, 100);
	m_Grid.SetColumnWidth(2, 100);
}

void CInputRebarMain3::SetGridData_BFooting()
{
	m_Grid.SetGridDefault();
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	if(!pBri->m_bExistProtectionRiseFooting)
	{
		m_Grid.SetGridDefault(1,1,1,1,TRUE,FALSE);
		m_Grid.SetColumnWidth(0,300);
		m_Grid.SetRowHeight(0,50);
		m_Grid.AddCell(0,0,_T("부상방지저판이 설치되어 있지 않습니다."));
		return;
	}

	long nRow = 0;
	long nCol = 0;
	UINT nFormat = DT_CENTER|DT_VCENTER;
	UINT nLine = CELL_LINE_NO;

	CStringArray strArr;
	strArr.Add(_T("주철근CTC"));
	strArr.Add(_T("주철근CTC/2"));

	m_Grid.SetGridDefault(3,1,2,0,TRUE,FALSE);

	nRow = 0;
	m_Grid.AddCell(nRow++, nCol, _T("DM"));
	m_Grid.AddCell(nRow++, nCol, _T("직경"));
	m_Grid.AddCell(nRow, nCol, &pBri->m_dPRF_Dia_Main, nFormat, nLine, _T(""), CELL_TYPE_DIA);
	++nCol;

	nRow = 0;
	m_Grid.AddCell(nRow++, nCol, _T("DM"));
	m_Grid.AddCell(nRow++, nCol, _T("CTC"));
	m_Grid.AddCell(nRow, nCol, &pBri->m_bPRF_SameCTC_Main, nFormat, nLine, _T(""), CELL_TYPE_COMBO, strArr);
	++nCol;

	nRow = 0;
	m_Grid.AddCell(nRow++, nCol, _T("BO2"));
	m_Grid.AddCell(nRow++, nCol, _T("직경"));
	m_Grid.AddCell(nRow, nCol, &pBri->m_dPRF_Dia_Support, nFormat, nLine, _T(""), CELL_TYPE_DIA);
	++nCol;

	nRow = 0;
	m_Grid.AddCell(nRow++, nCol, _T("BO2"));
	m_Grid.AddCell(nRow++, nCol, _T("수평 CTC"));
	m_Grid.AddCell(nRow, nCol, &pBri->m_dPRF_CTC_Support, nFormat, nLine, _T(""));
	++nCol;

	nRow = 0;
	m_Grid.AddCell(nRow++, nCol, _T("BO2"));
	m_Grid.AddCell(nRow++, nCol, _T("수직 CTC"));
	m_Grid.AddCell(nRow, nCol, &pBri->m_dPRF_CTC_SupportVert, nFormat, nLine, _T(""));
	++nCol;

	nRow = 0;
	m_Grid.AddCell(nRow++, nCol, _T("BO3"));
	m_Grid.AddCell(nRow++, nCol, _T("직경"));
	m_Grid.AddCell(nRow, nCol, &pBri->m_dPRF_Dia_Shear, nFormat, nLine, _T(""), CELL_TYPE_DIA);
	++nCol;

	nRow = 0;
	m_Grid.AddCell(nRow++, nCol, _T("BO3"));
	m_Grid.AddCell(nRow++, nCol, _T("CTC"));
	m_Grid.AddCell(nRow, nCol, &pBri->m_dPRF_CTC_Shear, nFormat, nLine, _T(""));
	++nCol;

	CStringArray sArrType,sArrRebar;
	sArrType.Add(_T("캡형"));
	sArrType.Add(_T("겹침형"));
	sArrType.Add(_T("ㄷ형"));
	sArrType.Add(_T("꺽인 ㄷ형"));
	sArrType.Add(_T("캡형(정착장)"));
	sArrType.Add(_T("간격재"));

	CString str = _T("");
	sArrRebar.Add("없음");
	long nIdx = 0;
	for(nIdx = 1; nIdx < CUL_MAX_REBAR; nIdx ++)
	{
		str.Format("%d", nIdx);
		sArrRebar.Add(str);
	}	

	nRow = 0;
	m_Grid.AddCell(nRow++, nCol, _T("BO3"));
	m_Grid.AddCell(nRow++, nCol, _T("타입"));
	m_Grid.AddCell(nRow, nCol, &pBri->m_nPRF_Type_Shear, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArrType);
	++nCol;

	nRow = 0;
	m_Grid.AddCell(nRow++, nCol, _T("BO3"));
	m_Grid.AddCell(nRow++, nCol, _T("주철근 구간수"));
	m_Grid.AddCell(nRow, nCol, &pBri->m_nPRF_Count_Shear, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArrRebar);
	++nCol;

	m_Grid.GetCellCount(nRow, nCol);
	m_Grid.SetRows(nRow);
	m_Grid.SetCols(nCol);
	m_Grid.SetColumnWidthAll(75);
	m_Grid.SetColumnWidth(1,110);
	m_Grid.SetColumnWidth(nCol-1,100);
}

void CInputRebarMain3::OnSelchangeComboDanbu() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	pBri->m_nType_DanbuRebar = m_comboDanbu.GetCurSel();
	SyncRebar();
}

void CInputRebarMain3::SetGridData_Stirrup()
{
	// 그리드 초기화
	m_Grid.SetGridDefault(3, 2, 3, 0);
	m_Grid.SetCellSize(20, 80);

	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long row = 0, col = 0;
	CStringArray sArr, sArr2;
	CString str = _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	col	= 0;
	
	long i = 0; for(i = 0; i < 10; i++)
	{
		str.Format("%d", i);
		sArr.Add(str);
		str.Format("%d", i+1);
		sArr2.Add(str);
	}
	
	long nCountSlab = 1;
	if(pBri->IsBoxType()) nCountSlab = 2;

	for(long iUD = 0; iUD < nCountSlab; iUD++)
	{
		CString strSlab = (iUD == iUPPER) ? _T("상부슬래브") : _T("하부슬래브");

		m_Grid.AddCell(row, col, strSlab, nFormat, CELL_READONLY);
		m_Grid.AddCell(row+1, col, "시점", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+2, col, "직경", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+3, col++, &pBri->m_dDiaStirrupAtDanBuRebar[iUD][iSTT], nFormat, nLine, " ", CELL_TYPE_DIA);
		m_Grid.SetLastCellFy(pBri->GetValueFy(iUD == iUPPER? ePartUpperSlab : ePartLowerSlab));

		m_Grid.AddCell(row, col, strSlab, nFormat, CELL_READONLY);
		m_Grid.AddCell(row+1, col, "시점", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+2, col, "개수", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+3, col++, &pBri->m_nCountStirrupAtDanBuRebar[iUD][iSTT], nFormat, FALSE, nLine, " ", CELL_TYPE_COMBO, sArr);
	
		m_Grid.AddCell(row, col, strSlab, nFormat, CELL_READONLY);
		m_Grid.AddCell(row+1, col, "시점", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+2, col, "주철근구간수", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+3, col++, &pBri->m_nCountStirrupCtcMainRebar[iUD][iSTT], nFormat, FALSE, nLine, " ", CELL_TYPE_COMBO, sArr2);
		
		m_Grid.AddCell(row, col, strSlab, nFormat, CELL_READONLY);
		m_Grid.AddCell(row+1, col, "종점", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+2, col, "직경", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+3, col++, &pBri->m_dDiaStirrupAtDanBuRebar[iUD][iEND], nFormat, nLine, " ", CELL_TYPE_DIA);
			m_Grid.SetLastCellFy(pBri->GetValueFy(iUD == iUPPER? ePartUpperSlab : ePartLowerSlab));
		
		m_Grid.AddCell(row, col, strSlab, nFormat, CELL_READONLY);
		m_Grid.AddCell(row+1, col, "종점", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+2, col, "개수", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+3, col++, &pBri->m_nCountStirrupAtDanBuRebar[iUD][iEND], nFormat, FALSE, nLine, " ", CELL_TYPE_COMBO, sArr);
		
		m_Grid.AddCell(row, col, strSlab, nFormat, CELL_READONLY);
		m_Grid.AddCell(row+1, col, "종점", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+2, col, "주철근구간수", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+3, col++, &pBri->m_nCountStirrupCtcMainRebar[iUD][iEND], nFormat, FALSE, nLine, " ", CELL_TYPE_COMBO, sArr2);
	}
}

// 둔각부 보강 철근
void CInputRebarMain3::SetGridData_ObtuseAngle()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	BOOL bBoxType = pBri->IsBoxType();

	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 2, bBoxType? 2: 1);
	m_Grid.SetCellSize(20, -1);

	long row = 0, col = 0;
	CStringArray sArr;
	CString str = _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	
	BOOL bUpper	= m_comboObtuse.GetCurSel() == 0;
	long upper	= bUpper ? iUPPER : iLOWER;
	CString sUpper	= bUpper ? "상면" : "하면";
	
	if(bBoxType)	m_Grid.AddCell(row, col++, "구분\n");
	m_Grid.AddCell(row, col++, "구분\n");
	m_Grid.AddCell(row, col++, "설치\n");
	m_Grid.AddCell(row, col++, "철근직경");	// 주철근 직경
	m_Grid.AddCell(row, col++, "철근직경");	// 배력근 직경

	m_Grid.AddCell(row, col++, "피복");	// 둔각부 상면 피복
	m_Grid.AddCell(row, col++, "피복");	// 둔각부 하면 피복
	m_Grid.AddCell(row, col++, "피복");	// 둔각부 시종점 피복
	m_Grid.AddCell(row, col++, "피복");	// 둔각부 좌우측 피복

	m_Grid.AddCell(row, col++, "주철근길이");	// 상면 주철근 길이(선형방향)
	m_Grid.AddCell(row, col++, "주철근길이");	// 상면 주철근 길이(벽체방향)
	m_Grid.AddCell(row, col++, "주철근길이");	// 하면 주철근 길이
	m_Grid.AddCell(row, col++, sUpper+" 배력근배치");	// 배력철근 배치 시작 간격
	m_Grid.AddCell(row, col++, sUpper+" 배력근배치");	// 배력철근 배치 개수
	m_Grid.AddCell(row, col++, sUpper+" 배력근배치");	// 배력철근 배치 간격
	m_Grid.AddCell(row, col++, sUpper+" 배력근배치");	// 배력철근 배치 마지막 간격
	m_Grid.AddCell(row, col++, sUpper+" 주철근배치");	// 주철근 배치 시작 간격
	m_Grid.AddCell(row, col++, sUpper+" 주철근배치");	// 주철근 배치 개수
	m_Grid.AddCell(row, col++, sUpper+" 주철근배치");	// 주철근 배치 간격
	m_Grid.AddCell(row, col++, sUpper+" 주철근배치");	// 주철근 배치 마지막 간격

	if(bUpper)
	{
		m_Grid.AddCell(row, col++, "벽체 배력근배치");	// 배력철근 배치 시작 간격
		m_Grid.AddCell(row, col++, "벽체 배력근배치");	// 배력철근 배치 개수
		m_Grid.AddCell(row, col++, "벽체 배력근배치");	// 배력철근 배치 간격
		m_Grid.AddCell(row, col++, "벽체 배력근배치");	// 배력철근 배치 마지막 간격
	}

	row++;
	col	= 0;
	if(bBoxType)	m_Grid.AddCell(row, col++, "구분\n");
	m_Grid.AddCell(row, col++, "구분\n");
	m_Grid.AddCell(row, col++, "설치\n");
	m_Grid.AddCell(row, col++, bUpper ? "UD" : "LD");	// 주철근 직경
	m_Grid.AddCell(row, col++, bUpper ? "SDU" : "SDL");	// 배력근 직경

	m_Grid.AddCell(row, col++, "D1");	// 둔각부 상면 피복
	m_Grid.AddCell(row, col++, "D2");	// 둔각부 하면 피복
	m_Grid.AddCell(row, col++, "D3");	// 둔각부 시종점 피복
	m_Grid.AddCell(row, col++, "D4");	// 둔각부 좌우측 피복

	m_Grid.AddCell(row, col++, "UL1");	// 상면 주철근 길이(선형방향)
	m_Grid.AddCell(row, col++, "UL2");	// 상면 주철근 길이(벽체방향)
	m_Grid.AddCell(row, col++, "LL");	// 하면 주철근 길이
	m_Grid.AddCell(row, col++, "C1");	// 배력철근 배치 시작 간격
	m_Grid.AddCell(row, col++, "B");	// 배력철근 배치 개수
	m_Grid.AddCell(row, col++, "C");	// 배력철근 배치 간격
	m_Grid.AddCell(row, col++, "C2");	// 배력철근 배치 마지막 간격
	m_Grid.AddCell(row, col++, "F1");	// 주철근 배치 시작 간격
	m_Grid.AddCell(row, col++, "E");	// 주철근 배치 개수
	m_Grid.AddCell(row, col++, "F");	// 주철근 배치 간격
	m_Grid.AddCell(row, col++, "F2");	// 주철근 배치 마지막 간격

	if(bUpper)
	{
		m_Grid.AddCell(row, col++, "C1'");	// 배력철근 배치 시작 간격
		m_Grid.AddCell(row, col++, "B'");	// 배력철근 배치 개수
		m_Grid.AddCell(row, col++, "C'");	// 배력철근 배치 간격
		m_Grid.AddCell(row, col++, "C2'");	// 배력철근 배치 마지막 간격
	}

	

	for(long stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		CString sStt	= bStt ? "시점" : "종점";
		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT;
			CString sLeft	= bLeft ? "좌측" : "우측";

			CPlaceRebarByBlock *pPlaceL	= &pBri->m_placeByBlock_ObtuseAngle_Support_UpperSlab[stt][left][upper];
			CPlaceRebarByBlock *pPlaceC	= &pBri->m_placeByBlock_ObtuseAngle_Main_UpperSlab[stt][left][upper];

			col	= 0;
			row++;
			if(bBoxType)
				m_Grid.AddCell(row, col++, _T("상부슬래브"));
			m_Grid.AddCell(row, col++, sStt+sLeft);
			m_Grid.AddCell(row, col++, &pBri->m_bIsObtuseAngle_UpperSlab[stt][left], nFormat, nLine, "", CELL_TYPE_CHECK);
			m_Grid.AddCell(row, col++, &pPlaceC->m_dDia, nFormat, nLine, sUpper+" 주철근 철근 직경", CELL_TYPE_DIA);
			m_Grid.AddCell(row, col++, &pPlaceL->m_dDia, nFormat, nLine, sUpper + " 배력철근 직경", CELL_TYPE_DIA);

			m_Grid.AddCell(row, col++, &pBri->m_dCoverObtuseAngle_UpperSlab[stt][left][iUPPER], nFormat, nLine, "둔각부 상면 피복");
			m_Grid.AddCell(row, col++, &pBri->m_dCoverObtuseAngle_UpperSlab[stt][left][iLOWER], nFormat, nLine, "둔각부 하면 피복");
			m_Grid.AddCell(row, col++, &pBri->m_dCoverObtuseAngle_UpperSlab_Plane[stt][left][0], nFormat, nLine, "둔각부 시종점 피복");
			m_Grid.AddCell(row, col++, &pBri->m_dCoverObtuseAngle_UpperSlab_Plane[stt][left][1], nFormat, nLine, "둔각부 좌우측 피복");

			m_Grid.AddCell(row, col++, &pBri->m_dLenObtuseAngle_UpperSlab[stt][left][iUPPER], nFormat, nLine, "상면 주철근 길이");
			m_Grid.AddCell(row, col++, &pBri->m_dLenObtuseAngle_OutWall[stt][left], nFormat, nLine, "상면 주철근 벽체 방향 길이");
			m_Grid.AddCell(row, col++, &pBri->m_dLenObtuseAngle_UpperSlab[stt][left][iLOWER], nFormat, nLine, "하면 주철근 길이");
			m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 0, FALSE, nFormat, nLine, sUpper + "배력 철근 배치 처음간격");
			m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 1, TRUE, nFormat, nLine, sUpper + "배력 철근 배치 개수");
			m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 1, FALSE, nFormat, nLine, sUpper + "배력 철근 배치 간격");
			m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 2, FALSE, nFormat, nLine, sUpper + "배력 철근 배치 마지막간격");
			m_Grid.AddCell(row, col++, &pPlaceC->m_xyArr, 0, FALSE, nFormat, nLine, sUpper + "주철근 배치 처음간격");
			m_Grid.AddCell(row, col++, &pPlaceC->m_xyArr, 1, TRUE, nFormat, nLine, sUpper + "주철근 배치 개수");
			m_Grid.AddCell(row, col++, &pPlaceC->m_xyArr, 1, FALSE, nFormat, nLine, sUpper + "주철근 배치 간격");
			m_Grid.AddCell(row, col++, &pPlaceC->m_xyArr, 2, FALSE, nFormat, nLine, sUpper + "주철근 배치 마지막간격");

			// 상면 입력중일땐 벽체 배치 입력도 받음
			if(bUpper)
			{
				pPlaceL	= &pBri->m_placeByBlock_ObtuseAngle_Support_OutWall[stt][left];
				m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 0, FALSE, nFormat, nLine, "외측벽체 배력 철근 배치 처음간격");
				m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 1, TRUE, nFormat, nLine, "외측벽체 배력 철근 배치 개수");
				m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 1, FALSE, nFormat, nLine, "외측벽체 배력 철근 배치 간격");
				m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 2, FALSE, nFormat, nLine, "외측벽체 배력 철근 배치 마지막간격");
			}
		}
	}

	if(bBoxType)
	{
		for(long stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			CString sStt	= bStt ? _T("시점") : _T("종점");
			for(long left = 0; left < 2; left++)
			{
				BOOL bLeft	= left == iLEFT;
				CString sLeft	= bLeft ? _T("좌측") : _T("우측");

				CPlaceRebarByBlock *pPlaceL	= &pBri->m_placeByBlock_ObtuseAngle_Support_LowerSlab[stt][left][upper];
				CPlaceRebarByBlock *pPlaceC	= &pBri->m_placeByBlock_ObtuseAngle_Main_LowerSlab[stt][left][upper];

				col	= 0;
				row++;
				m_Grid.AddCell(row, col++, _T("하부슬래브"));
				m_Grid.AddCell(row, col++, sStt+sLeft);
				m_Grid.AddCell(row, col++, &pBri->m_bIsObtuseAngle_LowerSlab[stt][left], nFormat, nLine, "", CELL_TYPE_CHECK);
				m_Grid.AddCell(row, col++, &pPlaceC->m_dDia, nFormat, nLine, sUpper+" 주철근 철근 직경", CELL_TYPE_DIA);
				m_Grid.AddCell(row, col++, &pPlaceL->m_dDia, nFormat, nLine, sUpper + " 배력철근 직경", CELL_TYPE_DIA);

				m_Grid.AddCell(row, col++, &pBri->m_dCoverObtuseAngle_LowerSlab[stt][left][iUPPER], nFormat, nLine, "둔각부 상면 피복");
				m_Grid.AddCell(row, col++, &pBri->m_dCoverObtuseAngle_LowerSlab[stt][left][iLOWER], nFormat, nLine, "둔각부 하면 피복");
				m_Grid.AddCell(row, col++, &pBri->m_dCoverObtuseAngle_LowerSlab_Plane[stt][left][0], nFormat, nLine, "둔각부 시종점 피복");
				m_Grid.AddCell(row, col++, &pBri->m_dCoverObtuseAngle_LowerSlab_Plane[stt][left][1], nFormat, nLine, "둔각부 좌우측 피복");

				m_Grid.AddCell(row, col++, &pBri->m_dLenObtuseAngle_LowerSlab[stt][left][iUPPER], nFormat, nLine, "상면 주철근 길이");
				m_Grid.AddCell(row, col++, &pBri->m_dLenObtuseAngle_LowerOutWall[stt][left], nFormat, nLine, "상면 주철근 벽체 방향 길이");
				m_Grid.AddCell(row, col++, &pBri->m_dLenObtuseAngle_LowerSlab[stt][left][iLOWER], nFormat, nLine, "하면 주철근 길이");
				m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 0, FALSE, nFormat, nLine, sUpper + "배력 철근 배치 처음간격");
				m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 1, TRUE, nFormat, nLine, sUpper + "배력 철근 배치 개수");
				m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 1, FALSE, nFormat, nLine, sUpper + "배력 철근 배치 간격");
				m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 2, FALSE, nFormat, nLine, sUpper + "배력 철근 배치 마지막간격");
				m_Grid.AddCell(row, col++, &pPlaceC->m_xyArr, 0, FALSE, nFormat, nLine, sUpper + "주철근 배치 처음간격");
				m_Grid.AddCell(row, col++, &pPlaceC->m_xyArr, 1, TRUE, nFormat, nLine, sUpper + "주철근 배치 개수");
				m_Grid.AddCell(row, col++, &pPlaceC->m_xyArr, 1, FALSE, nFormat, nLine, sUpper + "주철근 배치 간격");
				m_Grid.AddCell(row, col++, &pPlaceC->m_xyArr, 2, FALSE, nFormat, nLine, sUpper + "주철근 배치 마지막간격");

				// 상면 입력중일땐 벽체 배치 입력도 받음
				if(bUpper)
				{
					pPlaceL	= &pBri->m_placeByBlock_ObtuseAngle_Support_LowerOutWall[stt][left];
					m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 0, FALSE, nFormat, nLine, "외측벽체 배력 철근 배치 처음간격");
					m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 1, TRUE, nFormat, nLine, "외측벽체 배력 철근 배치 개수");
					m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 1, FALSE, nFormat, nLine, "외측벽체 배력 철근 배치 간격");
					m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 2, FALSE, nFormat, nLine, "외측벽체 배력 철근 배치 마지막간격");
				}
			}
		}
	}

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(0, 100);
	if(bBoxType)
		m_Grid.SetColumnWidth(1, 100);
}


// 둔각부 보강 철근
void CInputRebarMain3::SetGridData_AcuteAngle()
{
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 1, 1);
	m_Grid.SetCellSize(20, -1);

	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long row = 0, col = 0;
	CStringArray sArr;
	CString str = _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	
	BOOL bUpper	= m_comboObtuse.GetCurSel() == 0;
	CString sUpper	= bUpper ? "상면" : "하면";

	m_Grid.AddCell(row, col++, "구분");
	m_Grid.AddCell(row, col++, "설치");
	m_Grid.AddCell(row, col++, "OD");	// 벽체 주철근 직경
	m_Grid.AddCell(row, col++, "FD");	// 기초 주철근 직경
	m_Grid.AddCell(row, col++, "ODS");	// 벽체 배력철근 직경
	m_Grid.AddCell(row, col++, "FDS");	// 기초 배력철근 직경

	m_Grid.AddCell(row, col++, "D1");	// 벽체 측면 피복
	m_Grid.AddCell(row, col++, "D2");	// 기초 하면 피복
	m_Grid.AddCell(row, col++, "D3");	// 기초 상면 피복
	

	m_Grid.AddCell(row, col++, "OL");	// 벽체 측면 철근 길이
	m_Grid.AddCell(row, col++, "FL");	// 기초 상면 철근 길이

	m_Grid.AddCell(row, col++, "WMR");	// 벽체 주철근 허용 범위
	m_Grid.AddCell(row, col++, "FMR");	// 기초 주철근 허용 범위
	

	for(long stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		CString sStt	= bStt ? "시점" : "종점";
		CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;

		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT;
			CString sLeft	= bLeft ? "좌측" : "우측";
			BOOL bFront	= bStt ? bLeft : !bLeft;
			long nFront	= bFront ? iFRONT : iBACK;

			CPlaceRebarByBlock *pPlaceO		= &pBri->m_placeByBlock_AcuteAngle_MainRebar_OutWall[stt][left];
			CPlaceRebarByBlock *pPlaceF		= &pFooting->m_placeByBlock_Obtuse_MainRebar[nFront];
			CPlaceRebarByBlock *pPlaceOS	= &pBri->m_placeByBlock_AcuteAngle_SupportRebar_OutWall[stt][left];
			CPlaceRebarByBlock *pPlaceFS	= &pFooting->m_placeByBlock_Obtuse_SupportRebar[nFront];

			col	= 0;
			row++;
			m_Grid.AddCell(row, col++, sStt+sLeft);
			m_Grid.AddCell(row, col++, &pBri->m_bIsAcuteAngle_OutWall[stt][left], nFormat, nLine, "", CELL_TYPE_CHECK);
			m_Grid.AddCell(row, col++, &pPlaceO->m_dDia, nFormat, nLine, "벽체 예각부 주철근 직경", CELL_TYPE_DIA);
			m_Grid.AddCell(row, col++, &pPlaceF->m_dDia, nFormat, nLine, "기초 둔각부 주철근 직경", CELL_TYPE_DIA);
			m_Grid.AddCell(row, col++, &pPlaceOS->m_dDia, nFormat, nLine, "벽체 예각부 배력철근 직경", CELL_TYPE_DIA);
			m_Grid.AddCell(row, col++, &pPlaceFS->m_dDia, nFormat, nLine, "기초 둔각부 배력철근 직경", CELL_TYPE_DIA);

			m_Grid.AddCell(row, col++, &pBri->m_dCoverAcuteAngle_OutWall[stt][left][iUPPER], nFormat, nLine, "벽체 예각부 철근 측면 피복");
			m_Grid.AddCell(row, col++, &pBri->m_dCoverAcuteAngle_OutWall[stt][left][iLOWER], nFormat, nLine, "벽체 예각부 철근 하면 피복");
			m_Grid.AddCell(row, col++, &pFooting->m_dCoverObtuseUpper[nFront], nFormat, nLine, "기초 둔각부 철근 상면 피복");
			
			m_Grid.AddCell(row, col++, &pBri->m_dLenAcuteangle_OutWall[stt][left], nFormat, nLine, "벽체 예각부 철근 길이");
			m_Grid.AddCell(row, col++, &pFooting->m_dLenObtuseUpper[nFront], nFormat, nLine, "기초 둔각부 철근 길이");

			m_Grid.AddCell(row, col++, &pBri->m_dRange_AcuteAngle_OutWall[stt][left], nFormat, nLine, "벽체 주철근 포함 범위");
			m_Grid.AddCell(row, col++, &pFooting->m_dRange_Obtuse_MainRebar[nFront], nFormat, nLine, "기초 주철근 포함 범위");
		}
	}

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(0, 100);
}

void CInputRebarMain3::OnSelchangeComboObtuse() 
{
	// TODO: Add your control notification handler code here
	CCellID cell		= m_Grid.GetFocusCell();
	
	SetGridData();
	SetDataInit();
	DrawInputDomyunView(FALSE);

	m_Grid.SetFocusCell(cell.row, cell.col);
}



void CInputRebarMain3::OnBnClickedButtonCorner()
{
	CDlgCorner dlg(m_pIn);
	
	if(dlg.DoModal()==IDOK)
		DrawInputDomyunView();
}
