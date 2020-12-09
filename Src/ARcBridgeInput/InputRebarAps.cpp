// InputRebarAps.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "../ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "ApsSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputRebarAps dialog


CInputRebarAps::CInputRebarAps(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputRebarAps::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputRebarAps)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nTab	= 0;

	for(HGINT32 ix = 0; ix < 2; ++ix)
	{
		m_nTabStt[ix] = -1;
		m_nTabEnd[ix] = -1;

		for (HGINT32 iy = 0; iy < MAX_CNS; ++iy)
		{
			m_nTabEndCns[ix][iy] = -1;
			m_nTabSttCns[ix][iy] = -1;
		}
	}

	m_pDlgCheck = new CInputCheckEdit;
	m_pDlgCheckGrid = new CInputCheckGrid;
	m_bCheckDowelRebarSep = FALSE;
}

CInputRebarAps::~CInputRebarAps()
{
	delete m_pDlgCheck;
	delete m_pDlgCheckGrid;
}

void CInputRebarAps::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputRebarAps)
	DDX_Control(pDX, IDC_BUTTON_SETTING, m_btnSetting);
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_comboBridgeNo);
	DDX_Control(pDX, IDC_BUTTON_EXAMINE, m_btnExamine);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Check(pDX, IDC_CHECK_DOWELREBAR_SEP, m_bCheckDowelRebarSep);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputRebarAps, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputRebarAps)
	ON_BN_CLICKED(IDC_BUTTON_EXAMINE, OnButtonExamine)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
	ON_BN_CLICKED(IDC_CHECK_SAMEOFFSET, OnCheckSameoffset)
	ON_BN_CLICKED(IDC_BUTTON_SETTING, OnButtonSetting)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_BN_CLICKED(IDC_CHECK_DOWELREBAR_SEP, OnCheckDowelRebarSep)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputRebarAps message handlers
void CInputRebarAps::InitComboCtrl()
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

	SetControl();
}

void CInputRebarAps::OnPreInitDialog()
{
	// 시작하면서 pick box의 크기를 정해줌
	if(m_pView) 
	{
		CTwinVector tv;
		tv.SetPickBoxSize(m_pView->GetSelectionRectSize()/m_pView->GetRatioWindow());
	}
	
	InitTabCtrl();
	InitComboCtrl();

	GetDlgItem(IDC_CHECK_DOWELREBAR_SEP)->SetWindowText(hggettext("다웰바 철근 분리 집계"));

	SetResize(IDC_BUTTON_EXAMINE, SZ_BOTTOM_LEFT,	SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_SETTING, SZ_BOTTOM_LEFT,	SZ_BOTTOM_LEFT);
	SetResize(IDC_TAB,			  SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID,			  SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_CHECK_DOWELREBAR_SEP, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	

	SetHyperHelp(m_pIn->GetManualPath());

	m_Grid.InitGrid(m_pIn->m_pARcBridgeDataStd->m_Fy);
	SetGridData();
	SetDataInit();
	SetControl();
}

void CInputRebarAps::InitTabCtrl()
{
	m_Tab.RemoveAllTabs();
	m_Tab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_Tab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	CString str		= _T("");
	CString sLeft	= _T("");
	long nTab(0);
	long left(0), cns(0);
	for(left = 0; left < 2; left++)
	{
		m_nTabStt[left]	= -1;
		m_nTabEnd[left]	= -1;
		for(cns = 0; cns < MAX_CNS; cns++)
		{
			m_nTabSttCns[left][cns]	= -1;
			m_nTabEndCns[left][cns]	= -1;
		}
	}

	m_sArrTabName.RemoveAll();
	if(pBri->IsOutWall(TRUE)) 
	{
		for(left = 0; left < 2; left++)
		{
			sLeft	= left == iLEFT ? "(좌)" : "(우)";
			if(!pBri->m_bDivisionApsStt) sLeft = "";
			if(pBri->m_apsStt[left].m_bIs)
			{
				m_sArrTabName.Add("시점측 접속슬래브"+sLeft);
				m_nTabStt[left] = nTab++;

				// 완충슬래브
				for(cns = 0; cns < MAX_CNS; cns++)
				{
					CApsApp *pCns	= &pBri->m_cnsStt[left][cns];
					if(pCns->m_bIs && pBri->m_nCountCnsStt[left] > cns)
					{
						str.Format("시점측 완충슬래브%d%s", cns+1, sLeft);
						m_sArrTabName.Add(str);
						m_nTabSttCns[left][cns]	= nTab++;
					}
				}
			}
		}
	}
	if(pBri->IsOutWall(FALSE)) 
	{
		for(left = 0; left < 2; left++)
		{
			sLeft	= left == iLEFT ? "(좌)" : "(우)";
			if(!pBri->m_bDivisionApsEnd) sLeft = "";
			if(pBri->m_apsEnd[left].m_bIs)
			{
				m_sArrTabName.Add("종점측 접속슬래브"+sLeft);
				m_nTabEnd[left] = nTab++;

				// 완충슬래브
				for(cns = 0; cns < MAX_CNS; cns++)
				{
					CApsApp *pCns	= &pBri->m_cnsEnd[left][cns];
					if(pCns->m_bIs && pBri->m_nCountCnsEnd[left] > cns)
					{
						str.Format("종점측 완충슬래브%d%s", cns+1, sLeft);
						m_sArrTabName.Add(str);
						m_nTabEndCns[left][cns]	= nTab++;
					}
				}
			}
		}
	}
	
	long i = 0; for(i = 0; i < m_sArrTabName.GetSize(); i++)
		m_Tab.InsertItem(i, m_sArrTabName[i]);

	if(m_Tab.GetItemCount() <= m_nTab) m_nTab = 0;
	m_Tab.SetCurSel(m_nTab);
}

void CInputRebarAps::SetDataDefault()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	for(long bri = 0; bri < pStd->GetBridgeSize(); bri++)
	{
		CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(bri);
		if(!pBri) return;

		BOOL bFirst	= IsFirst(bri);

		if(bFirst)
		{
			// 시점측 접속슬래브 ////////////////////////////////
			SetDataDefaultRebarApsStd(&pBri->m_apsStt[iLEFT]);
			SetDataDefaultRebarApsStd(&pBri->m_apsStt[iRIGHT]);
			
			// 시점측 완충슬래브(좌)
			SetDataDefaultRebarApsStd(&pBri->m_cnsStt[iLEFT][0]);
			SetDataDefaultRebarApsStd(&pBri->m_cnsStt[iLEFT][1]);
			
			// 시점측 완충슬래브(우)
			SetDataDefaultRebarApsStd(&pBri->m_cnsStt[iRIGHT][0]);
			SetDataDefaultRebarApsStd(&pBri->m_cnsStt[iRIGHT][1]);


			// 종점측 접속슬래브 ////////////////////////////////
			SetDataDefaultRebarApsStd(&pBri->m_apsEnd[iLEFT]);
			SetDataDefaultRebarApsStd(&pBri->m_apsEnd[iRIGHT]);
			
			// 시점측 완충슬래브(좌)
			SetDataDefaultRebarApsStd(&pBri->m_cnsEnd[iLEFT][0]);
			SetDataDefaultRebarApsStd(&pBri->m_cnsEnd[iLEFT][1]);
			
			// 시점측 완충슬래브(우)
			SetDataDefaultRebarApsStd(&pBri->m_cnsEnd[iRIGHT][0]);
			SetDataDefaultRebarApsStd(&pBri->m_cnsEnd[iRIGHT][1]);

			pBri->SetDefaultedState(GetIDD(), 0x00000001);
		}
		else
		{
			if(IsClickDefaultButton(bri))
			{
				if(m_nTab == m_nTabStt[iLEFT])			SetDataDefaultRebarApsStd(&pBri->m_apsStt[iLEFT]);
				if(m_nTab == m_nTabStt[iRIGHT])			SetDataDefaultRebarApsStd(&pBri->m_apsStt[iRIGHT]);
				if(m_nTab == m_nTabSttCns[iLEFT][0])	SetDataDefaultRebarApsStd(&pBri->m_cnsStt[iLEFT][0]);
				if(m_nTab == m_nTabSttCns[iLEFT][1])	SetDataDefaultRebarApsStd(&pBri->m_cnsStt[iLEFT][1]);
				if(m_nTab == m_nTabSttCns[iRIGHT][0])	SetDataDefaultRebarApsStd(&pBri->m_cnsStt[iRIGHT][0]);
				if(m_nTab == m_nTabSttCns[iRIGHT][1])	SetDataDefaultRebarApsStd(&pBri->m_cnsStt[iRIGHT][1]);

				if(m_nTab == m_nTabEnd[iLEFT])			SetDataDefaultRebarApsStd(&pBri->m_apsEnd[iLEFT]);
				if(m_nTab == m_nTabEnd[iRIGHT])			SetDataDefaultRebarApsStd(&pBri->m_apsEnd[iRIGHT]);
				if(m_nTab == m_nTabEndCns[iLEFT][0])	SetDataDefaultRebarApsStd(&pBri->m_cnsEnd[iLEFT][0]);
				if(m_nTab == m_nTabEndCns[iLEFT][1])	SetDataDefaultRebarApsStd(&pBri->m_cnsEnd[iLEFT][1]);
				if(m_nTab == m_nTabEndCns[iRIGHT][0])	SetDataDefaultRebarApsStd(&pBri->m_cnsEnd[iRIGHT][0]);
				if(m_nTab == m_nTabEndCns[iRIGHT][1])	SetDataDefaultRebarApsStd(&pBri->m_cnsEnd[iRIGHT][1]);
			}
		}
	}

	SetGridData();
	SetDataInit();
}

void CInputRebarAps::SetDataSave()
{
	m_Grid.UpdateData(TRUE);


	m_pIn->m_pARcBridgeDataStd->SyncApsDowelRebarSeperateRebarOption(m_bCheckDowelRebarSep);

	// 하면CTC는 옵션에 따라 상면 CTC를 참고 한다.
	CApsApp *pAps	= GetAps();
	if(!pAps) return;

	// 주철근2,3은 주철근1의 지름을 따라가고, 배력근2는 배력근1의 지름을 따라간다.
	for(long upper = 0; upper < 2; upper++)
	{
		pAps->m_dDiaMain2[upper]	= pAps->m_dDiaMain1[upper];
		pAps->m_dDiaMain3[upper]	= pAps->m_dDiaMain1[upper];
		pAps->m_dDiaSup2[upper]	= pAps->m_dDiaSup1[upper];
	}

	m_pIn->m_pDoc->SetModifiedFlag();

	if(m_pDlgCheck->GetSafeHwnd() && m_pDlgCheck->IsWindowVisible())
	{
		m_pDlgCheck->Check();
	}
}

void CInputRebarAps::SetGridData()
{
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 2, 0);
	m_Grid.SetCellSize(20, -1);

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	CApsApp *pAps = GetAps();
	if(!pAps) return;
	BOOL bHunch	= pAps->m_dH2 > 0;
	
	CDPointArray xyArr;	// 배경색 반전위치
	CStringArray sArr;
	CString str = _T("");
	CString strHelp = _T("");
	long row = 0;
	long col = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;

	m_Grid.InitGrid(pAps->m_dFy);

	col	= 0;
	m_Grid.AddCell(row, col, "주철근");
	m_Grid.AddCell(row+1, col++, "CTC(상)");
	m_Grid.AddCell(row, col, "주철근");
	m_Grid.AddCell(row+1, col++, "CTC(하)");
	m_Grid.AddCell(row, col, "주철근");
	m_Grid.AddCell(row+1, col++, "MRU");
	m_Grid.AddCell(row, col, "주철근");
	m_Grid.AddCell(row+1, col++, "MRL");

	m_Grid.AddCell(row, col, "배력철근");
	m_Grid.AddCell(row+1, col++, "CTC(상)");
	m_Grid.AddCell(row, col, "배력철근");
	m_Grid.AddCell(row+1, col++, "CTC(하)");
	m_Grid.AddCell(row, col, "배력철근");
	m_Grid.AddCell(row+1, col++, "SRU");
	m_Grid.AddCell(row, col, "배력철근");
	m_Grid.AddCell(row+1, col++, "SRL");

	if(bHunch)
	{
		m_Grid.AddCell(row, col, "헌치부");
		m_Grid.AddCell(row+1, col++, "HR");
		m_Grid.AddCell(row, col, "헌치부");
		m_Grid.AddCell(row+1, col++, "HSR");
		m_Grid.AddCell(row, col, "스터럽");
		m_Grid.AddCell(row+1, col++, "TR");
	}

	BOOL bOrthoSlabEnd = pAps->m_bOrthoSlabEnd;
	
	m_Grid.AddCell(row, col, hggettext("사보강 배근"));
	m_Grid.AddCell(row+1, col++, _T("적용"));
	m_Grid.AddCell(row, col, hggettext("사보강 배근"));
	m_Grid.AddCell(row+1, col++, _T("좌측배치"));
	m_Grid.AddCell(row, col, hggettext("사보강 배근"));
	m_Grid.AddCell(row+1, col++, _T("우측배치"));	
	row+=2;

	CHgBaseConcStd baseConc;
	sArr.RemoveAll();
	sArr.Add("100");
	sArr.Add("110");
	sArr.Add("120");
	sArr.Add("125");
	sArr.Add("150");
	sArr.Add("200");
	sArr.Add("250");
	sArr.Add("300");
	sArr.Add("400");
	sArr.Add("450");
	sArr.Add("500");
	sArr.Add("600");
	sArr.Add("800");
	sArr.Add("1200");
		
	col	= 0;
	m_Grid.AddCell(row, col++, &pAps->m_dCtcMain[iUPPER], nFormat, nLine, "상면 주철근 배치 간격", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col++, &pAps->m_dCtcMain[iLOWER], nFormat, nLine, "하면 주철근 배치 간격", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col++, &pAps->m_dDiaMain1[iUPPER], nFormat, nLine, "상면 주철근 지름", CELL_TYPE_DIA);
	m_Grid.AddCell(row, col++, &pAps->m_dDiaMain1[iLOWER], nFormat, nLine, "하면 주철근 지름", CELL_TYPE_DIA);

	m_Grid.AddCell(row, col++, &pAps->m_dCtcSup[iUPPER], nFormat, nLine, "상면 배력철근 배치 간격", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col++, &pAps->m_dCtcSup[iLOWER], nFormat, nLine, "하면 배력철근 배치 간격", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col++, &pAps->m_dDiaSup1[iUPPER], nFormat, nLine, "상면 배력철근 지름", CELL_TYPE_DIA);
	m_Grid.AddCell(row, col++, &pAps->m_dDiaSup1[iLOWER], nFormat, nLine, "하면 배력철근 지름", CELL_TYPE_DIA);

	if(bHunch)
	{
		m_Grid.AddCell(row, col++, &pAps->m_dDiaHunch, nFormat, nLine, "헌치부철근 지름(주철근)", CELL_TYPE_DIA);
		m_Grid.AddCell(row, col++, &pAps->m_dDiaHunchSup, nFormat, nLine, "헌치부철근 지름(배력근)", CELL_TYPE_DIA);
		m_Grid.AddCell(row, col++, &pAps->m_placeStrup.m_dDia, nFormat, nLine, "스터럽 철근 지름", CELL_TYPE_DIA);
	}
	
	BOOL bReadOnly = (bOrthoSlabEnd || !pAps->m_bApplySabogangRebar);
	m_Grid.AddCell(row, col++, &pAps->m_bApplySabogangRebar, nFormat, bOrthoSlabEnd ? CELL_READONLY : nLine, hggettext(""), CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, &pAps->m_nCountSabogangRebarUpper[0], nFormat, FALSE, bReadOnly ? CELL_READONLY : nLine);
	m_Grid.AddCell(row, col++, &pAps->m_nCountSabogangRebarUpper[1], nFormat, FALSE, bReadOnly ? CELL_READONLY : nLine);

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(0, 80);
	m_Grid.SetColumnWidth(1, 80);
	m_Grid.SetColumnWidth(4, 80);
	m_Grid.SetColumnWidth(5, 80);
	m_Grid.SetColumnWidth(9, 80);
	m_Grid.SetColumnWidth(10, 80);
}

void CInputRebarAps::SetDataInit()
{
	if(!IsAps())
	{
		m_pIn->GridMessage(&m_Grid, "접속슬래브가 존재 하지 않습니다");
		return;
	}

	m_Grid.UpdateData(FALSE);

	long nRows(0), nCols(0);
	m_Grid.GetCellCount(nRows, nCols);
	m_Grid.MergeGrid(0, 0, 0, nCols);
}


void CInputRebarAps::SetControl()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(pBri == NULL) return;

	m_bCheckDowelRebarSep = pBri->m_bApsDowelSeparateRebar;
	UpdateData(FALSE);	
}

void CInputRebarAps::DrawInputDomyunView(BOOL bZoomAll)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(m_pIn->GetScaleInputDialog());

	CDomyun domPlane(pDom);
	CDomyun domSection(pDom);
	CDomyun domFront(pDom);

	CApsApp *pAps = GetAps();
	if(!pAps) 
	{
		CInputDialogBase::DrawInputDomyunView(bZoomAll);
		return;
	}
	CARcBridgeDBStandard DBStd(m_pIn->GetRcBridgeRebar(), m_pIn->m_pARoadOptionStd, m_pIn->m_pARcBridgeDataStd);

	// return 시키기 전에 이부분이 나오면 안된다.
	pBri->GetTvVertSection_Std(0, FALSE, FALSE);
	pBri->SyncBridgeAps(0, FALSE);

	// 평면도
	DBStd.m_pDrawAps->DrawPlane(&domPlane, pAps);
	CDRect rectPlane;
	domPlane.SetCalcExtRect();
	rectPlane = domPlane.GetExtRect();

	DBStd.m_pDrawAps->DrawPlanCenterLine(&domPlane, pAps, TRUE);
	DBStd.m_pDrawAps->DrawRebarMainOutPlan(&domPlane, pAps);
	DBStd.m_pDrawAps->DrawRebarMainPlan(&domPlane, pAps, AREA_APS_ALL, TRUE, TRUE);
	DBStd.m_pDrawAps->DrawRebarSupPlan(&domPlane, pAps, AREA_APS_ALL, TRUE, TRUE);
	DBStd.m_pDrawAps->DrawRebarShearPlan(&domPlane, pAps);
	DBStd.m_pDrawAps->DrawRebarMainHunchPlan(&domPlane, pAps, TRUE);
	DBStd.m_pDrawAps->DrawRebarSupHunchPlan(&domPlane, pAps, TRUE);
	DBStd.m_pDrawAps->MarkRebarOutPlan(&domPlane, pAps);

	DBStd.m_pDrawAps->DimTextCuttingCircleInfo(&domPlane, pAps);
	DBStd.m_pDrawAps->DrawSectionMark(&domPlane, pAps);
	*pDom << domPlane;

	// 측면도
	pAps->GetTvSide(TRUE);
	DBStd.m_pDrawAps->DrawSection(&domSection, pAps);
	CDRect rectSection;
	domSection.SetCalcExtRect();
	rectSection = domSection.GetExtRect();

	DBStd.m_pDrawAps->DrawRebarSupSide(&domSection, pAps, TRUE, TRUE, TRUE, TRUE);
	DBStd.m_pDrawAps->DrawRebarSupSide(&domSection, pAps, FALSE, TRUE, TRUE, TRUE);
	DBStd.m_pDrawAps->DrawRebarMainOutSide(&domSection, pAps, TRUE);
	DBStd.m_pDrawAps->DrawRebarHunchSide(&domSection, pAps, TRUE, TRUE, TRUE);
	DBStd.m_pDrawAps->DrawRebarHunchSupSide(&domSection, pAps, TRUE);
	DBStd.m_pDrawAps->DrawRebarStrupSide(&domSection, pAps, TRUE);
	DBStd.m_pDrawAps->DrawRebarShearSide(&domSection, pAps, TRUE);
	
	CDPoint xyMove;
	xyMove.x = rectPlane.left - rectSection.left;
	xyMove.y = rectPlane.top - rectSection.bottom - 8000;
	domSection.Move(xyMove);

	*pDom << domSection;

	// 정면도
	DBStd.m_pDrawAps->DrawFront(&domFront, pAps);
	CDRect rectFront;
	domFront.SetCalcExtRect();
	rectFront = domFront.GetExtRect();

	DBStd.m_pDrawAps->DrawRebarMainFront(&domFront, pAps, TRUE, TRUE, TRUE, TRUE);
	DBStd.m_pDrawAps->DrawRebarMainFront(&domFront, pAps, FALSE, TRUE, TRUE, TRUE);
	DBStd.m_pDrawAps->DrawRebarSupOutFront(&domFront, pAps, TRUE);
	DBStd.m_pDrawAps->DrawRebarShearFront(&domFront, pAps, TRUE);
	
	xyMove.x = rectPlane.left - rectFront.right - 8000;
	xyMove.y = rectPlane.top - rectFront.top;
	domFront.Move(xyMove);
	*pDom << domFront;

	/*
	// 테이블
	CDomyun domTable(pDom);
	pAps->SetDataDefaultRebarJewon();
	DBStd.m_pDrawAps->DrawRebarJewonTable(&domTable, pAps);
	CDRect rectTable;
	domTable.SetCalcExtRect();
	rectTable	= domTable.GetExtRect();

	xyMove.x	= rectPlane.right - rectTable.left + 5000;
	xyMove.y	= rectPlane.top - rectPlane.top;
	domTable.Move(xyMove);
	*pDom << domTable;
	*/
	pBri->GetTvVertSection_Std(0, FALSE, FALSE, TRUE);
	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

void CInputRebarAps::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	
	return;
}

void CInputRebarAps::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	SetFocus();
	m_nTab = m_Tab.GetCurSel();
	SetGridData();
	SetDataInit();
	SetControl();
	DrawInputDomyunView(TRUE);
	return;
}

void CInputRebarAps::OnButtonExamine() 
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	if(m_pDlgCheck->GetSafeHwnd() && m_pDlgCheck->IsWindowVisible()) m_pDlgCheck->DestroyWindow();
	if(m_pDlgCheckGrid->GetSafeHwnd() && m_pDlgCheckGrid->IsWindowVisible()) 
		m_pDlgCheckGrid->DestroyWindow();

	long nTypeCheck	= -1;
	if(m_nTab == m_nTabStt[iLEFT]) nTypeCheck	= 0;
	if(m_nTab == m_nTabEnd[iLEFT]) nTypeCheck	= 1;
	if(nTypeCheck == -1) return;

	CString str = _T("");
	str.Format("접속슬래브 검토");

	if (m_bDesignLsd)
	{
		m_pDlgCheckGrid->SetBriAndContent(pBri,IDD_REBAR_APS, nTypeCheck);
		
		m_pDlgCheckGrid->ShowAndCheck(this);
		
		m_pDlgCheckGrid->SetWindowText(str);
	}
	else
	{
		m_pDlgCheck->SetBriAndContent(pBri,IDD_REBAR_APS, nTypeCheck);

		m_pDlgCheck->ShowAndCheck(this);	

		m_pDlgCheck->SetWindowText(str);	
	}
}

void CInputRebarAps::OnSelchangeComboBridgeNo() 
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

void CInputRebarAps::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	SetDataSave();

	CCellID cell = m_Grid.GetFocusCell();
	CString sTitle	= m_Grid.GetTextMatrix(1, cell.col);

	if(m_nTab == m_nTabStt[iLEFT] || m_nTab == m_nTabStt[iRIGHT] || m_nTab == m_nTabEnd[iLEFT] || m_nTab == m_nTabEnd[iRIGHT])
	{
		CApsApp *pAps = GetAps();
		if(pAps == NULL) return;

		long nMultiple = (long)(pAps->m_dCtcMain[iUPPER] / pAps->m_dCtcMain[iLOWER]);
		if(sTitle == _T("좌측배치") || sTitle == _T("우측배치") || sTitle.Find(_T("CTC")) != -1)
		{
			pAps->m_nCountSabogangRebarLower[iLEFT] = pAps->m_nCountSabogangRebarUpper[iLEFT]*nMultiple;
			pAps->m_nCountSabogangRebarLower[iRIGHT] =  pAps->m_nCountSabogangRebarUpper[iRIGHT]*nMultiple;
		}
	}

	SetGridData();
	SetDataInit();
	DrawInputDomyunView();
}

void CInputRebarAps::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	structGridCell *pCell = m_Grid.GetCurrentCell();
	if(pCell)
	{
		m_pIn->SetComment(pCell->sTitle);
		
		m_pView->GetDomyun()->STMakeCursor(m_Grid.GetItemText(1,pCell->nCol));
	}
}

// 현재 작업중인 접속슬래브 리턴 
CApsApp* CInputRebarAps::GetAps()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(m_nTab < 0) return NULL;

	for(long left = 0; left < 2; left++)
	{
		if(m_nTab == m_nTabStt[left]) return &pBri->m_apsStt[left];
		if(m_nTab == m_nTabEnd[left]) return &pBri->m_apsEnd[left];

		for(long cns = 0; cns < MAX_CNS; cns++)
		{
			if(m_nTabSttCns[left][cns] == m_nTab) return &pBri->m_cnsStt[left][cns];
			if(m_nTabEndCns[left][cns] == m_nTab) return &pBri->m_cnsEnd[left][cns];
		}
	}
	
	return NULL;
}

BOOL CInputRebarAps::PreTranslateMessage(MSG* pMsg) 
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
	
	return CInputDialogBase::PreTranslateMessage(pMsg);
}

void CInputRebarAps::OnCheckSameoffset() 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CInputRebarAps::OnButtonSetting() 
{
	// TODO: Add your control notification handler code here
	CApsSettingDlg dlg(GetAps());
	if(dlg.DoModal() == IDOK)
	{
		DrawInputDomyunView(FALSE);
	}
}

// 접속슬래브 권고안 통합.
void CInputRebarAps::SetDataDefaultRebarApsStd(CApsApp *pAps)
{
	if(!pAps) return;
	pAps->SetDataDefaultCover();
	pAps->SetDataDefaultShearWarp();
	pAps->SetDataDefaultRebar();
}

BOOL CInputRebarAps::IsAps()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return FALSE;
	
	if(!pBri->IsAps(TRUE, TRUE) && !pBri->IsAps(TRUE, FALSE) && !pBri->IsAps(FALSE, TRUE) && !pBri->IsAps(FALSE, FALSE))
		return FALSE;

	return TRUE;
}

BOOL CInputRebarAps::IsValid()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	int nCountBri1 = 0;
	int nCountBri2 = 0;
	int nSizeBri = pStd->GetBridgeSize();
	for(int nBr=0; nBr<nSizeBri; nBr++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(nBr);

		if(!pBri->IsOutsideWall())
			nCountBri1++;
		if(pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB)
			nCountBri2++;
	}

	// 교량의 형식 중에 문형 라멘교나 박스교가 없을 경우 접속 슬래브 배근 입력 메뉴를 표시하지 않음
	if(nSizeBri==nCountBri1)
		return FALSE;
	// 교량의 형식이 모두 슬래브교일 경우 기둥 위치 입력 이후의 메뉴를 표시하지 않음
	if(nSizeBri==nCountBri2)
		return FALSE;

	return TRUE;
}

void CInputRebarAps::OnCheckDowelRebarSep()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(pBri == NULL) return;
	
	UpdateData(TRUE);

	m_pIn->m_pARcBridgeDataStd->SyncApsDowelRebarSeperateRebarOption(m_bCheckDowelRebarSep, m_pIn->m_nCurrentBridge);	
}
