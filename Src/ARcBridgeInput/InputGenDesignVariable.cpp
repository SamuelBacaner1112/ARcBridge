// InputGenDesignVariable.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "../ARcBridgeDBDraw/ARcBridgeDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputGenDesignVariable dialog


CInputGenDesignVariable::CInputGenDesignVariable(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputGenDesignVariable::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputGenDesignVariable)
	//}}AFX_DATA_INIT
	m_pBriForCover	= NULL;
	m_nTabCover = -1;
	m_nTabEnv = -1;
	m_nTabCondition = -1;
	
	m_nTabDesignRating = -1;
	m_nTabExposureRating = -1;
	m_nTabRebarFactor	= -1;
	m_nTabRebarSettle = -1;
	m_nTabRebarJoint = -1;
	m_nTabLsdSupportRebar = -1;
	m_nMergeRowSupportBarSlab = 0;
	m_nMergeRowSupportBarWall = 0;
	m_nMergeRowSupportBarFoot = 0;
	
	m_pDlgCheck = new CInputCheckGrid;
}

CInputGenDesignVariable::~CInputGenDesignVariable()
{
	if(m_pBriForCover)
	{
		delete m_pBriForCover;
		m_pBriForCover	= NULL;
	}
	
	delete m_pDlgCheck;
}

void CInputGenDesignVariable::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputGenDesignVariable)
	DDX_Control(pDX, IDC_BUTTON_BASEDATA_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_BUTTON_BASEDATA_LOAD, m_btnLoad);
	DDX_Control(pDX, IDC_BUTTON_APPLY_SAMEALL, m_btnSameAll);
	DDX_Control(pDX, IDC_BUTTON_APPLY_MIRROR2, m_btnMirror);
	DDX_Control(pDX, IDC_CHECK_QUICK_SEARCH, m_checkQuickSearch);
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_cbBridgeNo);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputGenDesignVariable, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputGenDesignVariable)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_BN_CLICKED(IDC_CHECK_QUICK_SEARCH, OnCheckQuickSearch)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_MIRROR2, OnButtonApplyMirror)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_SAMEALL, OnButtonApplySameAll)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
	ON_BN_CLICKED(IDC_BUTTON_BASEDATA_SAVE, OnButtonBasedataSave)
	ON_BN_CLICKED(IDC_BUTTON_BASEDATA_LOAD, OnButtonBasedataLoad)
	ON_BN_CLICKED(IDC_BUTTON_EXAMINE, OnButtonExamine)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputGenDesignVariable message handlers

void CInputGenDesignVariable::SetGridData()
{
	m_Grid.SetType(0);
	m_Grid.SetCellSize(-1, -1);

	int nTab = m_Tab.GetCurSel();

	if(nTab == m_nTabCondition)			SetGridDataCondition();
	if(nTab == m_nTabEnv)				SetGridDataEnv();
	if(nTab == m_nTabCover)				SetGridDataCover();
	if(m_bDesignLsd)
	{
		if(nTab == m_nTabExposureRating)	SetGridDataExposureRating();
		if(nTab == m_nTabDesignRating)		SetGridDataDesignRating();
		if(nTab == m_nTabRebarFactor)		SetGridDataRebarFactor();
		if(nTab == m_nTabRebarSettle)		SetGridDataRebarSettle();
		if(nTab == m_nTabRebarJoint)		SetGridDataRebarJoint();
		if(nTab == m_nTabLsdSupportRebar)	SeteGridDataLsdSupportRebar();
	}
}

void CInputGenDesignVariable::SetDataSave()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	m_Grid.UpdateData(TRUE);
	

	// 주철근 피복의 2, 3단을 1단에서 100씩 추가로 더해서 적용시킨다.
	// 따로 입력을 받지않기 때문에 여기서 자동으로 지정해줌
	// 단, 날개벽은 입력을 받을 예정이므로 일단 보류
	
	double dDefaultOffset	= 100;
	long dan = 0; for(dan = 1; dan < MAX_REBAR_DAN; dan++)
	{
		pBri->m_dCoverUpper_UpperSlab[dan]		= pBri->m_dCoverUpper_UpperSlab[0] + (dan * dDefaultOffset);	// 상부슬래브 상면
		pBri->m_dCoverLower_UpperSlab[dan]		= pBri->m_dCoverLower_UpperSlab[0] + (dan * dDefaultOffset);	// 상부슬래브 하면
		pBri->m_dCoverSide_UpperSlab[dan]		= pBri->m_dCoverSide_UpperSlab[0] + (dan * dDefaultOffset);		// 상부슬래브 측면;	// 상부슬래브 측면
		pBri->m_dCoverUpper_LowerSlab[dan]		= pBri->m_dCoverUpper_LowerSlab[0] + (dan * dDefaultOffset);	// 하부슬래브 상면
		pBri->m_dCoverLower_LowerSlab[dan]		= pBri->m_dCoverLower_LowerSlab[0] + (dan * dDefaultOffset);	// 하부슬래브 하면
		pBri->m_dCoverWallOut[dan]				= pBri->m_dCoverWallOut[0] + (dan * dDefaultOffset);			// 벽체외면
		pBri->m_dCoverWallIn[dan]				= pBri->m_dCoverWallIn[0] + (dan * dDefaultOffset);				// 벽체 내면
		pBri->m_dCoverWallMid[dan]				= pBri->m_dCoverWallMid[0] + (dan * dDefaultOffset);			// 중간벽체
		pBri->m_dCoverColumn[dan]				= pBri->m_dCoverColumn[0] + (dan * dDefaultOffset);				// 기둥
//		pBri->m_dCoverWingOut[dan]				= pBri->m_dCoverWingOut[0] + (dan * dDefaultOffset);			// 날개벽 외면
//		pBri->m_dCoverWingIn[dan]				= pBri->m_dCoverWingIn[0] + (dan * dDefaultOffset);				// 날개벽 내면
		pBri->m_dCoverUpper_Footing[dan]		= pBri->m_dCoverUpper_Footing[0] + (dan * dDefaultOffset);		// 기초 상면
		pBri->m_dCoverSide_Footing[dan]		= pBri->m_dCoverSide_Footing[0] + (dan * dDefaultOffset);		// 기초 측면
		pBri->m_dCoverLower_Footing_Lean[dan]	= pBri->m_dCoverLower_Footing_Lean[0] + (dan * dDefaultOffset);		// 기초 하면(직접기초시)
		pBri->m_dCoverLower_Footing_Pile[dan]	= pBri->m_dCoverLower_Footing_Pile[0] + (dan * dDefaultOffset);		// 기초 하면(말뚝기초시)
		pBri->m_dCoverRib[iUPPER][dan]		= pBri->m_dCoverRib[iUPPER][0] + (dan * dDefaultOffset);
		pBri->m_dCoverRib[iLOWER][dan]		= pBri->m_dCoverRib[iLOWER][0] + (dan * dDefaultOffset);
	}
	SyncCover(pBri);
	pBri->SyncBridge_DesignData();

	m_pIn->m_pDoc->SetModifiedFlag();
	CInputDialogBase::SetDataSave();

	int nTab = m_Tab.GetCurSel();
	// 슬래브 주철근과 배력근 좌표 초기화
	if(nTab == m_nTabEnv)
	{
		pBri->m_bCaledRebarMain_Plane	= FALSE;
		pBri->m_bCaledRebarSupport_Plane	= FALSE;
	}

	// 종단표현방법 초기화
	pBri->m_bFirstCalledGetTvVertSection_Std	= FALSE;
	pBri->m_bSecondCalledGetTvVertSection_Std	= FALSE;
}

void CInputGenDesignVariable::SetDataInit()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	m_Grid.UpdateData(FALSE);
	m_Grid.SetRedraw(TRUE, TRUE);

	long nRows = 0;
	long nCols = 0;
	m_Grid.GetCellCount(nRows, nCols);

	int nTab = m_Tab.GetCurSel();

	if(nTab == m_nTabEnv || nTab == m_nTabCondition)  
	{
		if(nTab == m_nTabCondition)
		{
			m_Grid.MergeGridEx(0, nRows-1, 0, 1);
			m_Grid.MergeGrid(0, nRows-1, 2, 3, TRUE, FALSE);
		}
		else if(nTab == m_nTabEnv)
		{
			m_Grid.MergeGridEx(1, nRows-1, 0, 0, FALSE, TRUE);
			m_Grid.MergeGrid(0, nRows-1, 1, 3);
			//		m_Grid.MergeGrid(6, 7, 3, 3, FALSE, TRUE);
			if(m_bDesignLsd == FALSE)
			{
				m_Grid.MergeGridEx(m_nMergeRowSupportBarSlab,m_nMergeRowSupportBarSlab+1,4,4);
				m_Grid.MergeGridEx(m_nMergeRowSupportBarWall,m_nMergeRowSupportBarWall+1,4,4);
				m_Grid.MergeGridEx(m_nMergeRowSupportBarFoot,m_nMergeRowSupportBarFoot+1,4,4);
			}
			
			m_Grid.MergeGrid(1, nRows-1, 2, 3, TRUE, FALSE);
			m_Grid.MergeGrid(0, nRows-1, 4, 5, TRUE, FALSE);
		}
		else
		{
			m_Grid.MergeGridEx(0, nRows-1, 0, 1, TRUE, TRUE, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		}
	}

	if (nTab == m_nTabRebarFactor)
	{
		m_Grid.MergeGridEx(0, nRows-1, 0, 0);
	}
	else if(nTab == m_nTabLsdSupportRebar)
	{
		m_Grid.MergeGridEx(0, nRows-1, 0, 1);
		m_Grid.MergeGridEx(0, nRows-1, 1, 2, TRUE, FALSE, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}
	
	m_btnMirror.EnableWindow(pBri->m_nSeparBri!=0);
	GetDlgItem(IDC_BUTTON_EXAMINE)->EnableWindow(nTab == m_nTabExposureRating);
	m_checkQuickSearch.SetCheck(pBri->m_bQuickSearchLine);
}

// 입력사항 추가시 항목을 추가해주어야함.... => OnButtonApplyMirror() 
void CInputGenDesignVariable::SetDataDefault()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	
	int nTab = m_Tab.GetCurSel();

	for(long bri = 0; bri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		CRcBridgeRebar *pBri = pStd->GetBridgeRebar(bri);
		BOOL bFirst	= IsFirst(bri);
		pBri->SetDefaultedState(GetIDD(), 0x00000001);
		if(!bFirst && !IsClickDefaultButton(bri)) continue;

		if(bFirst || nTab == m_nTabCondition)
		{
			pBri->SetDataDefaultCondition();
		}
		
		if(bFirst || nTab == m_nTabEnv)				
		{
			pBri->SetDataDefaultEnv();
			// 트리 갱신
			AfxGetMainWnd()->SendMessage(WM_USER+8, -1);
		}
		if(bFirst || nTab == m_nTabCover)			pBri->SetDataDefaultCover();

		if(m_bDesignLsd)
		{
			if(bFirst || nTab == m_nTabExposureRating)	pBri->SetDataDefaultExposureRatingLsd();
			if(bFirst || nTab == m_nTabDesignRating)	pBri->SetDataDefaultDesignRatingLsd();
			if(bFirst || nTab == m_nTabRebarFactor)	pBri->SetDataDefaultRebarFactorLsd();
			if(bFirst || nTab == m_nTabRebarSettle)	pBri->SetDataDefaultRebarSettleLsd();
			if(bFirst || nTab == m_nTabRebarJoint)	pBri->SetDataDefaultRebarJointLsd();
			if(bFirst || nTab == m_nTabLsdSupportRebar)	pBri->SetDataDefaultLsdSupportRebar();
		}

		pBri->SyncBridge_DesignData();

		if(nTab == m_nTabEnv)
		{
			// 슬래브 주철근과 배력근 좌표 초기화
			pBri->m_bCaledRebarMain_Plane	= FALSE;
			pBri->m_bCaledRebarSupport_Plane	= FALSE;
		}
	}

	SetGridData();
	SetDataInit();
	m_pIn->m_pDoc->SetModifiedFlag();
}

void CInputGenDesignVariable::AddCellByBridgePart(HGINT32 &iRow, HGINT32 &iCol, const CString &strCategory, HGDOUBLE dArr[])
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	UINT nFormatLeft	= DT_LEFT | DT_VCENTER;
	UINT nFormat	= DT_RIGHT | DT_VCENTER;

	UINT CellLine = CELL_LINE_NO;
	CString szUnit_CGS	= _T("kgf/cm²");
	CString szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	
	const HGINT32 iEnd = pBri->GetMaterialDetail() ? ePartCount : ePartUpperSlab + 1;
	for (HGINT32 ix = ePartUpperSlab; ix < iEnd; ++ix)
	{
		CString strPart;

		switch (ix)
		{
		case ePartUpperSlab:
			strPart = "본체/상부슬래브";
			break;
		case ePartInWall:
			strPart = "중간지점부:벽체";
			break;
		case ePartInFoot:
			strPart = "중간지점부:기초";
			break;
		default:
			strPart = pBri->GetStringPart(static_cast<EPartOfBridge>(ix));
		}

		m_Grid.AddCell(iRow, iCol, strCategory, DT_CENTER|DT_VCENTER,CellLine);
		m_Grid.AddCell(iRow, iCol+1, "  " + strPart, DT_LEFT,CellLine);
		m_Grid.AddCell(iRow, iCol+2, &dArr[ix], nFormat);
		if(m_bDesignLsd) m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
		else m_Grid.SetCellDec(CELL_DEC_3);
		m_Grid.AddCell(iRow, iCol+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), dArr[ix]), nFormat);
		m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
		m_Grid.AddCell(iRow++, iCol+4, szUnit_Apply, nFormatLeft, CELL_READONLY);
	}

	if (pBri->GetMaterialDetail() == FALSE)
	{
		m_Grid.AddCell(iRow, iCol, strCategory, DT_CENTER|DT_VCENTER,CellLine);
		m_Grid.AddCell(iRow, iCol+1, "  " + pBri->GetStringPart(ePartApsStt), DT_LEFT,CellLine);
		m_Grid.AddCell(iRow, iCol+2, &dArr[ePartApsStt], nFormat);
		if(m_bDesignLsd) m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
		else m_Grid.SetCellDec(CELL_DEC_3);
		m_Grid.AddCell(iRow, iCol+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), dArr[ePartApsStt]), nFormat);
		m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
		m_Grid.AddCell(iRow++, iCol+4, szUnit_Apply, nFormatLeft, CELL_READONLY);
	}
}

void CInputGenDesignVariable::AddCellFckConc(HGINT32 &iRow, HGINT32 &iCol)
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	UINT CellLine = CELL_LINE_NO;
	UINT nFormatCenter = DT_CENTER|DT_VCENTER;

	CString szUnit_CGS   = "kgf/cm²";
	CString szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(iRow, iCol, m_bDesignLsd ? "fck : 콘크리트 강도" : "  버림 콘크리트 강도", nFormatCenter ,CellLine);
	m_Grid.AddCell(iRow, iCol+1, "  버림 콘크리트 강도", GVHT_LEFT,CellLine);
	m_Grid.AddCell(iRow, iCol+4, szUnit_Apply, GVHT_LEFT,CELL_READONLY);
	m_Grid.AddCell(iRow, iCol+2, &pBri->m_dFckLeanConc, DT_RIGHT,CellLine);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(iRow++, iCol+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dFckLeanConc), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_1);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);

	szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(iRow, iCol, m_bDesignLsd ? "fck : 콘크리트 강도" : "  MASS 콘크리트 강도", nFormatCenter,CellLine);
	m_Grid.AddCell(iRow, iCol+1, "  MASS 콘크리트 강도", GVHT_LEFT,CellLine);
	m_Grid.AddCell(iRow, iCol+4, szUnit_Apply, GVHT_LEFT,CELL_READONLY);
	m_Grid.AddCell(iRow, iCol+2, &pBri->m_dFckMassConc, DT_RIGHT,CellLine);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(iRow++, iCol+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dFckMassConc), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_1);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
}

void CInputGenDesignVariable::AddCellValidDepthBaseGround(HGINT32 &iRow, HGINT32 &iCol)
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	UINT CellLine = CELL_LINE_NO;

	m_Grid.AddCell(iRow, iCol, m_bDesignLsd ? "기초지반" : "  기초의 유효 근입깊이", GVHT_LEFT,CellLine);
	m_Grid.AddCell(iRow, iCol+1, "  기초의 유효 근입깊이", GVHT_LEFT,CellLine);
	m_Grid.AddCell(iRow, iCol+4, "m", GVHT_LEFT,CELL_READONLY);
	m_Grid.AddCell(iRow, iCol+2, &pBri->m_dValidDepthBaseGround, DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_3);
	m_Grid.AddCell(iRow++, iCol+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dValidDepthBaseGround), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_3);
}

void CInputGenDesignVariable::AddCellWallSpringEQ(HGINT32 &iRow, HGINT32 &iCol)
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	UINT CellLine = CELL_LINE_NO;

	CString szUnit_CGS		= UNIT_CGS_KGF_CM2;
	CString szUnit_Apply	= m_bDesignLsd ? pStd->GetStringUnitType(szUnit_CGS) : (pStd->IsSI() ? "kN/m²" : UNIT_CGS_KGF_CM2);
	m_Grid.AddCell(iRow, iCol, m_bDesignLsd ? "내진해석" : "  벽체부 스프링강성계산시 지반변형계수", GVHT_LEFT,CellLine);
	m_Grid.AddCell(iRow, iCol+1, "  벽체부 스프링강성계산시 지반변형계수", GVHT_LEFT,CellLine);
	m_Grid.AddCell(iRow, iCol+4, szUnit_Apply, GVHT_LEFT,CELL_READONLY);
	m_Grid.AddCell(iRow, iCol+2, &pBri->m_dEo_CalcWallSpringEQ, DT_RIGHT,CellLine);m_Grid.SetCellDec(pStd->GetDecimal(szUnit_CGS, CELL_DEC_0));m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(iRow++, iCol+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dEo_CalcWallSpringEQ), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_0);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
}

void CInputGenDesignVariable::AddCellModelingEQ(HGINT32 &iRow, HGINT32 &iCol, HGINT32 iCellLine)
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	UINT CellLine = CELL_LINE_NO;
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		m_Grid.AddCell(iRow, iCol, m_bDesignLsd ? "내진해석" : "  내진해석\n", GVHT_LEFT,CellLine);
		m_Grid.AddCell(iRow, iCol+1, "  해석방법", GVHT_LEFT,CellLine);
		m_Grid.AddCell(iRow, iCol+2, "  등가정적 하중법", DT_RIGHT, CELL_READONLY);
		m_Grid.AddCell(iRow, iCol+3, "  등가정적 하중법", DT_RIGHT, CELL_READONLY);
		m_Grid.AddCell(iRow++, iCol+4, "", DT_RIGHT, CELL_READONLY);

		for(long ix=0; ix<eGoalEarthQuakeCount; ++ix)
		{
			if(pStd->m_nTypeEarthQuakeApply == 0 && ix > eGoalEQ_Function)
			{
				pBri->m_bGoalEarthQuake[ix] = FALSE;
				continue;
			}

			m_Grid.AddCell(iRow, iCol, m_bDesignLsd ? "내진해석" : "  내진해석\n", GVHT_LEFT,CellLine);
			m_Grid.AddCell(iRow, iCol+1, "  성능목표", GVHT_LEFT,CellLine);
			m_Grid.AddCell(iRow, iCol+2, &pBri->m_bGoalEarthQuake[ix], DT_RIGHT, CellLine, pBri->GetStringGoalEarthQuake((EGoalOfEarthQuake)ix), CELL_TYPE_CHECK); 
			m_Grid.AddCell(iRow, iCol+3, pBri->GetStringGoalEarthQuake((EGoalOfEarthQuake)ix), DT_RIGHT);//, TRUE, CellLine, "", CELL_TYPE_COMBO, sArr);
			m_Grid.AddCell(iRow++, iCol+4, "", DT_RIGHT, CELL_READONLY);
		}

		m_Grid.AddCell(iRow, iCol, m_bDesignLsd ? "내진해석" : "  내진해석\n", GVHT_LEFT,CellLine);
		m_Grid.AddCell(iRow, iCol+1, "  설계수평지진계수(kh) 적용률", GVHT_LEFT,CellLine);
		m_Grid.AddCell(iRow, iCol+2, &pBri->m_dCoffKh_Sei, DT_RIGHT,iCellLine, _T(""));
		m_Grid.AddCell(iRow, iCol+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dCoffKh_Sei), DT_RIGHT,iCellLine);
		m_Grid.AddCell(iRow++, iCol+4, _T("%"), GVHT_LEFT,CELL_READONLY);
	}
	else
	{
		CHgStringArray sArr;
		sArr.RemoveAll();
		sArr.Add("시점측 벽체에 편토압 적용");
		sArr.Add("종점측 벽체에 편토압 적용");
		sArr.Add("각각의 벽체에 편토압 적용");

		m_Grid.AddCell(iRow, iCol, m_bDesignLsd ? "내진해석" : "  내진해석시 모델링 방법", GVHT_LEFT,CellLine);
		m_Grid.AddCell(iRow, iCol+1, "  내진해석시 모델링 방법", GVHT_LEFT,CellLine);		
		m_Grid.AddCell(iRow, iCol+2, &pBri->m_nSelectModelingCountEQ, DT_RIGHT, TRUE, iCellLine, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(iRow, iCol+3, m_pIn->GetStringArray(sArr, pBri->m_nSelectModelingCountEQ), DT_RIGHT);//, TRUE, CellLine, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(iRow++, iCol+4, "", DT_RIGHT, CELL_READONLY);
	}
}

void CInputGenDesignVariable::AddCellFootForce(HGINT32 &iRow, HGINT32 &iCol, HGINT32 iCellLine)
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	UINT CellLine = CELL_LINE_NO;
	if (pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
	{
		CHgStringArray sArr;
		sArr.RemoveAll();
		sArr.Add((pBri->GetTypeJointPosFootingEQ()==1)? "기초 도심[M/V]" : "기초상단[M/V]");
		sArr.Add((pBri->GetTypeJointPosFootingEQ()==1)? "기초 하단[(M+Hx기초두께/2)/V]" : "기초 하단[(M+Hx기초두께)/V]");

		m_Grid.AddCell(iRow, iCol, m_bDesignLsd ? "내진해석" : "  기초검토 단면력 산정시 최대편심 고려위치(지진시)", GVHT_LEFT,CellLine);
		m_Grid.AddCell(iRow, iCol+1, "  기초검토 단면력 산정시 최대편심 고려위치(지진시)", GVHT_LEFT,CellLine);
		m_Grid.AddCell(iRow, iCol+2, &pBri->m_nSelectFootForce, DT_RIGHT, TRUE, (pBri->m_nSelectModelingCountEQ==2)? iCellLine:CELL_READONLY, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(iRow, iCol+3, m_pIn->GetStringArray(sArr, pBri->m_nSelectFootForce), DT_RIGHT);//, TRUE, CellLine, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(iRow++, iCol+4, "", DT_RIGHT, CELL_READONLY);
	}
}

void CInputGenDesignVariable::SetGridDataCondition()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	BOOL bKDS17 = (pStd->m_nTypeEarthQuakeApply == 0) ? FALSE : TRUE;

	// 그리드 초기화
	m_Grid.SetGridDefault(2, 5, 1, 2);
	m_Grid.SetColumnWidth(0,m_bDesignLsd ? 180 : 140);
	m_Grid.SetColumnWidth(1,m_bDesignLsd ? 330 : 200);
	m_Grid.SetColumnWidth(2,m_bDesignLsd ? 280 : 200);
	m_Grid.SetColumnWidth(3,m_bDesignLsd ? 60 : 60);
	m_Grid.SetColumnWidth(4, 80);
	
 	m_Grid.SetRowHeightAll(20);

	CStringArray sArr;
	CString str;
	HGINT32 row = 0;
	HGINT32 col = 0;
	m_Grid.AddCell(row, col,	  "구  분");
	m_Grid.AddCell(row, col+1,	  "구  분");
	m_Grid.AddCell(row, col+2, "설정값");
	m_Grid.AddCell(row, col+3, "설정값");
	m_Grid.AddCell(row++, col+4, "단  위");


	// 여기서 부터는 하부측 제원이다. 
	// 따라서, 슬래브교일 경우 여기서 부터는 출력하지 않음
//	UINT CellLine = (pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB) ? CELL_LINE_NO : CELL_READONLY;
	UINT CellLine = CELL_LINE_NO;

	CString szUnit_CGS   = "tonf/m³";
	CString szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);

	if (m_bDesignLsd)
	{
		m_Grid.AddCell(row, col, "재료 물성치 상세 입력", DT_CENTER,CellLine);
		m_Grid.AddCell(row, col+1, "  재료 물성치 상세 입력", GVHT_LEFT,CellLine);
		
		sArr.RemoveAll();
		sArr.Add("적용");
		sArr.Add("미적용");
		m_Grid.AddCell(row, col+2, &pBri->GetMaterialDetail(), DT_RIGHT, CellLine, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row, col+3, m_pIn->GetStringArray(sArr, !pBri->GetMaterialDetail()), DT_RIGHT);
		m_Grid.AddCell(row++, col+4, "", DT_RIGHT, CELL_READONLY);

		AddCellByBridgePart(row, col, "fck : 콘크리트 강도", pBri->m_Fck);
		
		AddCellFckConc(row, col);
				
		AddCellByBridgePart(row, col, "fy : 철근 항복강도", pBri->m_Fy);		

		AddCellByBridgePart(row, col, "fvy : 전단철근 항복강도", pBri->m_Fvy);

		AddCellByBridgePart(row, col, "Ec : 콘크리트 탄성계수", pBri->m_Ec);

		m_Grid.AddCell(row, col, "재료특성 및 계수값", DT_CENTER,CellLine);
		m_Grid.AddCell(row, col+1, "  ε : 건조수축률", GVHT_LEFT,CellLine);
		m_Grid.AddCell(row, col+2, &pBri->GetRateDryAndShrinkage(), DT_RIGHT); 
		m_Grid.SetCellDec(CELL_DEC_6);
		m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->GetRateDryAndShrinkage()), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_6);
		m_Grid.AddCell(row++, col+4, "", DT_RIGHT, CELL_READONLY);

		m_Grid.AddCell(row, col, "재료특성 및 계수값", DT_CENTER,CellLine);
		m_Grid.AddCell(row, col+1, "  α : 선팽창계수", GVHT_LEFT,CellLine);
		m_Grid.AddCell(row, col+2, &pBri->GetCoefficientExpansion(), DT_RIGHT); m_Grid.SetCellDec(CELL_DEC_7);
		m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->GetCoefficientExpansion()), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_7);
		m_Grid.AddCell(row++, col+4, "", DT_RIGHT, CELL_READONLY);

		m_Grid.AddCell(row, col, "접속슬래브", DT_CENTER,CellLine);
		m_Grid.AddCell(row, col+1, "  fy : 다웰바 강종", GVHT_LEFT,CellLine);
		m_Grid.AddCell(row, col+2, &pBri->GetDBar_ForDowelBar(), DT_RIGHT, CellLine, pBri->GetDBar_ForDowelBar() ? "D바" : "H바", CELL_TYPE_CHECK);
		m_Grid.AddCell(row, col+3, pBri->GetDBar_ForDowelBar() ? "D바" : "H바", DT_RIGHT, CellLine);
		m_Grid.AddCell(row++, col+4, "", DT_RIGHT, CELL_READONLY);

		/*
		m_Grid.AddCell(row, col, "기초지반", DT_CENTER,CellLine);
		m_Grid.AddCell(row++, col+1, "  기초지반의 종류", GVHT_LEFT,CellLine);

		m_Grid.AddCell(row, col, "기초지반", DT_CENTER,CellLine);
		m_Grid.AddCell(row++, col+1, "  기초지반의 N치", GVHT_LEFT,CellLine);

		m_Grid.AddCell(row, col, "기초지반", DT_CENTER,CellLine);
		m_Grid.AddCell(row++, col+1, "  기초지반의 점참력", GVHT_LEFT,CellLine);

		m_Grid.AddCell(row, col, "기초지반", DT_CENTER,CellLine);
		m_Grid.AddCell(row++, col+1, "  근입지반의 단위중량", GVHT_LEFT,CellLine);

		m_Grid.AddCell(row, col, "기초지반", DT_CENTER,CellLine);
		m_Grid.AddCell(row++, col+1, "  지반변형계수(Eo)", GVHT_LEFT,CellLine);

		m_Grid.AddCell(row, col, "기초지반", DT_CENTER,CellLine);
		m_Grid.AddCell(row++, col+1, "  지반포아송비(v)", GVHT_LEFT,CellLine);

		AddCellValidDepthBaseGround(row++, col);

		AddCellPile(row, col);
		row += 3;
		*/
	}

	m_Grid.AddCell(row, col, m_bDesignLsd ? "뒷채움 및 성토재" : "  뒷채움의 단위중량", GVHT_LEFT,CellLine);
	m_Grid.AddCell(row, col+1, "  뒷채움의 단위중량", GVHT_LEFT,CellLine);
	m_Grid.AddCell(row, col+4, szUnit_Apply, GVHT_LEFT,CELL_READONLY);
	m_Grid.AddCell(row, col+2, &pBri->m_dUnitWeigthBackFill, DT_RIGHT,CellLine);m_Grid.SetCellDec(pStd->GetDecimal(szUnit_CGS, CELL_DEC_3));m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(row++, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dUnitWeigthBackFill), DT_RIGHT,CellLine);m_Grid.SetCellDec(pStd->GetDecimal(szUnit_CGS, CELL_DEC_3));m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);

	if(pBri->IsBoxType() && pBri->m_bApplyGroundWaterLevel)
	{
		m_Grid.AddCell(row, col, m_bDesignLsd ? "뒷채움 및 성토재" : "  뒷채움의 수중 단위중량", GVHT_LEFT,CellLine);
		m_Grid.AddCell(row, col+1, "  뒷채움의 수중 단위중량", GVHT_LEFT,CellLine);
		m_Grid.AddCell(row, col+4, szUnit_Apply, GVHT_LEFT,CELL_READONLY);
		m_Grid.AddCell(row, col+2, &pBri->m_dUnitWeigthBackFillWater, DT_RIGHT,CellLine);m_Grid.SetCellDec(pStd->GetDecimal(szUnit_CGS, CELL_DEC_3));m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
		m_Grid.AddCell(row++, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dUnitWeigthBackFillWater), DT_RIGHT,CellLine);m_Grid.SetCellDec(pStd->GetDecimal(szUnit_CGS, CELL_DEC_3));m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	}

	m_Grid.AddCell(row, col, m_bDesignLsd ? "뒷채움 및 성토재" : "  뒷채움의 내부마찰각", GVHT_LEFT,CellLine);
	m_Grid.AddCell(row, col+1, "  뒷채움의 내부마찰각", GVHT_LEFT,CellLine);
	m_Grid.AddCell(row, col+4, "도", GVHT_LEFT, CELL_READONLY);
	m_Grid.AddCell(row, col+2, &pBri->m_dAngleFrictionBackFill, DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_2);
	m_Grid.AddCell(row++, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dAngleFrictionBackFill), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_2);
	
	if (m_bDesignLsd == FALSE)
	{
		if(pStd->IsRailDesign())
		{
			szUnit_CGS   = "tonf/m²";
			szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
			m_Grid.AddCell(row, col, "  토압계산시 상재 활하중", GVHT_LEFT,CellLine);
			m_Grid.AddCell(row, col+1, "  토압계산시 상재 활하중", GVHT_LEFT,CellLine);
			m_Grid.AddCell(row, col+4, szUnit_Apply, GVHT_LEFT,CELL_READONLY);
			m_Grid.AddCell(row, col+2, &pBri->m_dLiveLoadAlways, DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_3);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
			m_Grid.AddCell(row++, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dLiveLoadAlways), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_3);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
			//
			m_Grid.AddCell(row, col, "  토압계산시 상재 고정하중", GVHT_LEFT,CellLine);
			m_Grid.AddCell(row, col+1, "  토압계산시 상재 고정하중", GVHT_LEFT,CellLine);
			m_Grid.AddCell(row, col+4, szUnit_Apply, GVHT_LEFT,CELL_READONLY);
			m_Grid.AddCell(row, col+2, &pBri->m_dDeadLoadAlways, DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_3);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
			m_Grid.AddCell(row++, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dDeadLoadAlways), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_3);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
		}
		else
		{
			szUnit_CGS   = "tonf/m²";
			szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
			m_Grid.AddCell(row, col, "  토압계산시 상재하중", GVHT_LEFT,CellLine);
			m_Grid.AddCell(row, col+1, "  토압계산시 상재하중", GVHT_LEFT,CellLine);
			m_Grid.AddCell(row, col+4, szUnit_Apply, GVHT_LEFT,CELL_READONLY);
			m_Grid.AddCell(row, col+2, &pBri->m_dLiveLoadAlways, DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_3);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
			m_Grid.AddCell(row++, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dLiveLoadAlways), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_3);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
		}


		m_Grid.AddCell(row, col, "  상재하중에 의한 수평력 적용", GVHT_LEFT,CellLine);
		m_Grid.AddCell(row, col+1, "  상재하중에 의한 수평력 적용", GVHT_LEFT,CellLine);
		sArr.RemoveAll();
		sArr.Add("토압계산시 상재하중 적용");
		sArr.Add("상재활하중 x 토압계수");
		m_Grid.AddCell(row, col+2, &pBri->m_nSelectLiveLoadHori, DT_RIGHT, TRUE, CellLine, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row, col+3, m_pIn->GetStringArray(sArr, pBri->m_nSelectLiveLoadHori), DT_RIGHT);//, TRUE, CellLine, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+4, "", GVHT_LEFT,CELL_READONLY);


		szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
		m_Grid.AddCell(row, col, "  보도활하중", GVHT_LEFT,CellLine);
		m_Grid.AddCell(row, col+1, "  보도활하중", GVHT_LEFT,CellLine);
		m_Grid.AddCell(row, col+4, szUnit_Apply, GVHT_LEFT,CELL_READONLY);
		m_Grid.AddCell(row, col+2, &pBri->m_dLiveLoadFootway, DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_3);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
		m_Grid.AddCell(row++, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dLiveLoadFootway), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_3);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	}
	
	szUnit_CGS   = "tonf/m³";
	szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(row, col, m_bDesignLsd ? "뒷채움 및 성토재" : "  성토재료의 단위중량(세부포장 미적용시)", GVHT_LEFT,CellLine);
	m_Grid.AddCell(row, col+1, "  성토재료의 단위중량(세부포장 미적용시)", GVHT_LEFT,CellLine);
	m_Grid.AddCell(row, col+4, szUnit_Apply, GVHT_LEFT,CELL_READONLY);
	m_Grid.AddCell(row, col+2, &pBri->m_dUnitWeightHeapSoil, DT_RIGHT,CellLine);m_Grid.SetCellDec(pStd->GetDecimal(szUnit_CGS, CELL_DEC_3));m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(row++, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dUnitWeightHeapSoil), DT_RIGHT,CellLine);m_Grid.SetCellDec(pStd->GetDecimal(szUnit_CGS, CELL_DEC_3));m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	

//	m_Grid.AddCell(row, col, "  근입지반의 단위중량", GVHT_LEFT,CellLine);
//	m_Grid.AddCell(row, col+1, "  근입지반의 단위중량", GVHT_LEFT,CellLine);
//	m_Grid.AddCell(row, col+4, szUnit_Apply, GVHT_LEFT,CellLine);
//	m_Grid.AddCell(row, col+2, &pBri->m_dUnitWeightValidDepth, DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_3);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
//	m_Grid.AddCell(row++, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dUnitWeightValidDepth), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_3);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	
	if (m_bDesignLsd == FALSE)
	{
		AddCellValidDepthBaseGround(row, col);

		//	szUnit_CGS		= UNIT_CGS_KGF_CM2;
		//	szUnit_Apply	= pStd->GetStringUnitType(szUnit_CGS);
		//	m_Grid.AddCell(row, col, "  안정검토시의 토압계수", GVHT_LEFT,CellLine); 
		//	m_Grid.AddCell(row, col+1, "  안정검토시의 토압계수", GVHT_LEFT,CellLine);
		//	sArr.RemoveAll();
		//	sArr.Add("Rankine 토압");
		//	sArr.Add("Coulomb 토압");
		//	m_Grid.AddCell(row, col+2, &pBri->m_nTypeToapSafeCheck, DT_RIGHT, TRUE, CellLine, "", CELL_TYPE_COMBO, sArr);
		//	m_Grid.AddCell(row++, col+3,  m_pIn->GetStringArray(sArr, pBri->m_nTypeToapSafeCheck), DT_RIGHT);

		m_Grid.AddCell(row, col, "  전도에 대한 안전율\n", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  평상시 ", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pBri->m_SafeFactorOverturning[0], DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_2);
		m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_SafeFactorOverturning[0]), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_2);
		m_Grid.AddCell(row++, col+4, "", GVHT_LEFT, CELL_READONLY);

		BOOL bEarthQuakeDesign = pBri->GetTypeDesingEQ() != 0 ? TRUE : FALSE;
		if(bEarthQuakeDesign)
		{
			m_Grid.AddCell(row, col, "  전도에 대한 안전율\n", GVHT_LEFT,CellLine);
			m_Grid.AddCell(row, col+1, "  지진시 ", GVHT_LEFT,CellLine);
			m_Grid.AddCell(row, col+2, &pBri->m_SafeFactorOverturning[1], DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_2);
			m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_SafeFactorOverturning[1]), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_2);
			m_Grid.AddCell(row++, col+4, "", GVHT_LEFT, CELL_READONLY);

		}

		m_Grid.AddCell(row, col, "  활동에 대한 안전율\n", GVHT_LEFT,CellLine);
		m_Grid.AddCell(row, col+1, "  평상시", GVHT_LEFT,CellLine);
		m_Grid.AddCell(row, col+2, &pBri->m_SafeFactorSliding[0], DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_2);
		m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_SafeFactorSliding[0]), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_2);
		m_Grid.AddCell(row++, col+4, "", GVHT_LEFT, CELL_READONLY);

		if(bEarthQuakeDesign)
		{
			m_Grid.AddCell(row, col, "  활동에 대한 안전율\n", GVHT_LEFT,CellLine);
			m_Grid.AddCell(row, col+1, "  지진시", GVHT_LEFT,CellLine);
			m_Grid.AddCell(row, col+2, &pBri->m_SafeFactorSliding[1], DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_2);
			m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_SafeFactorSliding[1]), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_2);
			m_Grid.AddCell(row++, col+4, "", GVHT_LEFT, CELL_READONLY);

			AddCellWallSpringEQ(row, col);

			AddCellModelingEQ(row, col, CellLine);

			AddCellFootForce(row, col, CellLine);
		}

		//	m_Grid.AddCell(row, col, "  내진해석시 말뚝스프링강성 산정", GVHT_LEFT,CellLine);
		//	m_Grid.AddCell(row, col+1, "  내진해석시 말뚝스프링강성 산정", GVHT_LEFT,CellLine);
		//	sArr.RemoveAll();
		//	sArr.Add("말뚝머리 고정");
		//	sArr.Add("말뚝머리 힌지");
		//	m_Grid.AddCell(row, col+2, &pBri->m_nSelectPileCalcSpringEQ, DT_RIGHT, TRUE, CellLine, "", CELL_TYPE_COMBO, sArr);
		//	m_Grid.AddCell(row++, col+3, m_pIn->GetStringArray(sArr, pBri->m_nSelectPileCalcSpringEQ), DT_RIGHT);//, TRUE, CellLine, "", CELL_TYPE_COMBO, sArr);
	}	

	m_Grid.AddCell(row, col, m_bDesignLsd ? "구조해석" : "  2D Frame 해석 툴", GVHT_LEFT,CELL_LINE_NO); 
	m_Grid.AddCell(row, col+1, "  2D Frame 해석 툴", GVHT_LEFT,CELL_LINE_NO);
	sArr.RemoveAll();
	sArr.Add("SAP 2000 7");
	sArr.Add("SAP 2000 8");
	sArr.Add("SAP 2000 9");
	sArr.Add("SAP 2000 10");
	sArr.Add("MIDAS Civil");
	m_Grid.AddCell(row, col+2, &pBri->m_nSelect2DFrameTool, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->m_nSelect2DFrameTool), DT_RIGHT);
	m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);
	
	m_Grid.AddCell(row, col, m_bDesignLsd ? "구조해석" : "  3D Plate 해석 툴", GVHT_LEFT,CELL_LINE_NO); 
	m_Grid.AddCell(row, col+1, "  3D Plate 해석 툴", GVHT_LEFT,CELL_LINE_NO);

	// 마지막에 m_Grid.SetCellBaseOfIndex(1); 추가한 이유
	// SAP 2000은 지원이 안되는데 불구하고, 콤보가 계속 나와 있어서 
	// 콤보에서 SAP2000을 제거 하기 위해 BaseOfIndex를 1로 바꾸로 항목을 제거함
	// 만약에 SAP 2000을 다시 넣으려면 SetCellBaseOfIndex를 다시 0으로 돌려 놓아야 됨.
	sArr.RemoveAll();
	//sArr.Add("SAP 2000");
	sArr.Add("MIDAS Civil");
	sArr.Add("3D 해석 안함");
	m_Grid.AddCell(row, col+2, &pBri->m_nSelect3DShellTool, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr); m_Grid.SetCellBaseOfIndex(1);
	m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->m_nSelect3DShellTool-1), DT_RIGHT);
	m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);

	m_Grid.AddCell(row, col, m_bDesignLsd ? "구조해석" : "  날개벽 해석방법", GVHT_LEFT,CELL_LINE_NO); 
	m_Grid.AddCell(row, col+1, "  날개벽 해석방법", GVHT_LEFT,CELL_LINE_NO);
	sArr.RemoveAll();
	sArr.Add("관용법");
	sArr.Add("관용법 + 3D Plate 해석");
	sArr.Add("3D Plate 해석");
	m_Grid.AddCell(row, col+2, &pBri->m_nSelectWingCalc, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->m_nSelectWingCalc), DT_RIGHT);
	m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);

	m_Grid.AddCell(row, col, m_bDesignLsd ? "구조해석" : "  3D Plate 해석결과 적용", GVHT_LEFT,CELL_LINE_NO); 
	m_Grid.AddCell(row, col+1, "  3D Plate 해석결과 적용", GVHT_LEFT,CELL_LINE_NO);
	sArr.RemoveAll();
	sArr.Add("부재의 각 절점 평균값");
	sArr.Add("부재의 각 절점 최대값");
	sArr.Add("부재의 Center");
	m_Grid.AddCell(row, col+2, &pBri->m_nSelect3DOutputValue, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->m_nSelect3DOutputValue), DT_RIGHT);
	m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);

	if(pBri->m_nSelect2DFrameTool == SOLVE_MIDAS || pBri->m_nSelect3DShellTool == 1)
	{
		m_Grid.AddCell(row, col, m_bDesignLsd ? "구조해석" : "  MIDAS input version", GVHT_LEFT,CELL_LINE_NO); 
		m_Grid.AddCell(row, col+1, "  MIDAS input version", GVHT_LEFT,CELL_LINE_NO);

		sArr.RemoveAll();
		sArr.Add("MIDAS Civil 4.3");
		sArr.Add("MIDAS Civil 8.6.1");
		m_Grid.AddCell(row, col+2, &pBri->m_nSelectMidasVer, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->m_nSelectMidasVer), DT_RIGHT);
		m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);
	}
	
	sArr.RemoveAll();
	sArr.Add(hggettext("최대 단면력 평균값"));
	sArr.Add(hggettext("최대 2개 부재 평균값"));
	sArr.Add(hggettext("최대 3개 부재 평균값"));
	sArr.Add(hggettext("최대 4개 부재 평균값"));
	sArr.Add(hggettext("최대 5개 부재 평균값"));
	// 	sArr.Add(hggettext("부재 Center값"));
	m_Grid.AddCell(row, col, m_bDesignLsd ? "구조해석" : "  3D Plate 해석결과 적용 부재수", GVHT_LEFT,CELL_LINE_NO); 
	m_Grid.AddCell(row, col+1, "  3D Plate 해석결과 적용 부재수", GVHT_LEFT,CELL_LINE_NO);
	m_Grid.AddCell(row, col+2, &pBri->m_nApplyWingWallCalcValueCount, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->m_nApplyWingWallCalcValueCount), DT_RIGHT);
	m_Grid.AddCell(row++, col+4, _T(""), DT_RIGHT, CELL_READONLY);

	if (m_bDesignLsd)
	{
		m_Grid.AddCell(row, col, "내진해석", GVHT_LEFT,CellLine); 
		m_Grid.AddCell(row, col+1, "  내진해석적용", GVHT_LEFT,CellLine);
		sArr.RemoveAll();
		sArr.Add("내진해석 하지않음"); 
		sArr.Add("내진해석 적용"); 
		m_Grid.AddCell(row, col+2, &pBri->GetTypeDesingEQ(), DT_RIGHT, TRUE, CellLine, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->GetTypeDesingEQ()), DT_RIGHT);
		m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);

		HGINT32 iCellLineEQ = pBri->GetTypeDesingEQ() == 0 ? CELL_READONLY : CellLine;

		AddCellModelingEQ(row, col, iCellLineEQ);

		m_Grid.AddCell(row, col, "내진해석", GVHT_LEFT,CELL_LINE_NO); 
		m_Grid.AddCell(row, col+1, "  내진등급", GVHT_LEFT,CELL_LINE_NO); 
		if(bKDS17)
		{
			sArr.RemoveAll(); sArr.Add("I 등급"); sArr.Add("II등급"); sArr.Add("특등급 2400"); sArr.Add("특등급 4800");
			m_Grid.AddCell(row, col+2, &pBri->GetDegreeEarthQuake(), DT_RIGHT, TRUE, iCellLineEQ, "", CELL_TYPE_AUTO, sArr, &m_EarthquakeGradeKDS17Dlg);
		}
		else
		{
			sArr.RemoveAll(); sArr.Add("I 등급"); sArr.Add("II등급");
			m_Grid.AddCell(row, col+2, &pBri->GetDegreeEarthQuake(), DT_RIGHT, TRUE, iCellLineEQ, "", CELL_TYPE_AUTO, sArr, &m_EarthquakeGradeDlg);			
		}
		m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->GetDegreeEarthQuake()), DT_RIGHT);
		m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);

		m_Grid.AddCell(row, col, "내진해석", GVHT_LEFT,CELL_LINE_NO); 
		m_Grid.AddCell(row, col+1, "  지진구역", GVHT_LEFT,CELL_LINE_NO);
		sArr.RemoveAll(); sArr.Add("I 구역"); sArr.Add("II구역");
		m_Grid.AddCell(row, col+2, &pBri->GetRegionEarthQuake(), DT_RIGHT, TRUE, iCellLineEQ, "", CELL_TYPE_AUTO, sArr, &m_EarhtquakeDlg);
		m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->GetRegionEarthQuake()), DT_RIGHT);
		m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);

		// KDS17은 지반종류, 시설물기준은 지반계수를 받고 있었는데 KDS에서도 지반계수를 받도록 수정. -KDS일때는 그냥 입력값으로
		if(bKDS17)
		{
			m_Grid.AddCell(row, col, "내진해석", GVHT_LEFT,CELL_LINE_NO); 
			m_Grid.AddCell(row, col+1, "  지반보정계수", GVHT_LEFT,CELL_LINE_NO);
			m_Grid.AddCell(row, col+2, &pBri->GetCoefficientGround(), DT_RIGHT, iCellLineEQ); m_Grid.SetCellDec(CELL_DEC_2);
			m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->GetCoefficientGround()), DT_RIGHT,iCellLineEQ);m_Grid.SetCellDec(CELL_DEC_2);
			m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);
		}
		else
		{
			m_Grid.AddCell(row, col, "내진해석", GVHT_LEFT,CELL_LINE_NO); 
			m_Grid.AddCell(row, col+1, "  지반계수", GVHT_LEFT,CELL_LINE_NO);
			m_Grid.AddCell(row, col+2, &pBri->GetCoefficientGround(), DT_RIGHT, iCellLineEQ, "", CELL_TYPE_AUTO, sArr, &m_EarthquakeFactDlg); m_Grid.SetCellDec(CELL_DEC_2);
			m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->GetCoefficientGround()), DT_RIGHT,iCellLineEQ);
			m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);
		}

		if(bKDS17)
		{
			sArr.RemoveAll();
			m_Grid.AddCell(row, col, "내진해석", GVHT_LEFT,CELL_LINE_NO); 
			m_Grid.AddCell(row, col+1, "  지반종류", GVHT_LEFT,CELL_LINE_NO);
			for(long ix = 0; ix < 6; ++ix)
			{
				str.Format(_T("S%d"), ix+1);
				sArr.Add(str);
			}
			m_Grid.AddCell(row, col+2, &pBri->GetTypeCoefficientGround(), DT_RIGHT, TRUE, iCellLineEQ, "", CELL_TYPE_AUTO, sArr, &m_EarthquakeFactKDS17Dlg);
			m_Grid.AddCell(row, col+3, sArr.GetAt(pBri->GetTypeCoefficientGround()), DT_RIGHT,iCellLineEQ);			
			m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);
		}

		if(bKDS17)
		{
			m_Grid.AddCell(row, col, "내진해석", GVHT_LEFT,CELL_LINE_NO); 
			m_Grid.AddCell(row, col+1, hggettext("  감쇠비(ξ) : 감쇠보정계수(CD) 산정"), GVHT_LEFT,CELL_LINE_NO);
			m_Grid.AddCell(row, col+2, &pBri->GetRateDAMP_EQ_CD(), DT_RIGHT, iCellLineEQ); m_Grid.SetCellDec(CELL_DEC_3);
			m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->GetRateDAMP_EQ_CD()), DT_RIGHT,iCellLineEQ);
			m_Grid.AddCell(row++, col+4, "%",GVHT_LEFT, CELL_READONLY);
		}

		m_Grid.AddCell(row, col, "내진해석", GVHT_LEFT,CELL_LINE_NO); 
		m_Grid.AddCell(row, col+1, "  중력가속도", GVHT_LEFT,CELL_LINE_NO);
		m_Grid.AddCell(row, col+2, &pBri->GetGravity(), DT_RIGHT, iCellLineEQ); m_Grid.SetCellDec(CELL_DEC_3);
		m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->GetGravity()), DT_RIGHT,iCellLineEQ);
		m_Grid.AddCell(row++, col+4, "m/sec²",GVHT_LEFT, CELL_READONLY);

		m_Grid.AddCell(row, col, "내진해석", GVHT_LEFT,CELL_LINE_NO); 
		m_Grid.AddCell(row, col+1, "  내진해석시 강성 모델링 방법", GVHT_LEFT,CELL_LINE_NO);
		sArr.RemoveAll(); 
		sArr.Add("벽체강성과 기초강성을 조합"); 
		sArr.Add("벽체강성과 기초강성을 개별적용"); 
		sArr.Add("기초강성을 무한강성으로 적용");
		m_Grid.AddCell(row, col+2, &pBri->GetTypeRigidFootingEQ(), DT_RIGHT, TRUE, iCellLineEQ, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->GetTypeRigidFootingEQ()), DT_RIGHT, iCellLineEQ);
		m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);

		AddCellWallSpringEQ(row, col);

		m_Grid.AddCell(row, col, "내진해석", GVHT_LEFT,CELL_LINE_NO); 
		m_Grid.AddCell(row, col+1, "  벽체강성 개별적용시 회전강성 적용", GVHT_LEFT,CELL_LINE_NO);
		sArr.RemoveAll(); sArr.Add("적용"); sArr.Add("적용안함");
		m_Grid.AddCell(row, col+2, &pBri->GetApplyRigidWallEQRotate(), DT_RIGHT, iCellLineEQ, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row, col+3, m_pIn->GetStringArray(sArr, !pBri->GetApplyRigidWallEQRotate()), DT_RIGHT);
		m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);

		m_Grid.AddCell(row, col, "내진해석", GVHT_LEFT,CELL_LINE_NO); 
		m_Grid.AddCell(row, col+1, "  내진해석시 기초 모델링 위치", GVHT_LEFT,CELL_LINE_NO);
		sArr.RemoveAll(); 
		sArr.Add("기초의 상단 위치"); 
		sArr.Add("기초의 도심 위치"); 
		m_Grid.AddCell(row, col+2, &pBri->GetTypeJointPosFootingEQ(), DT_RIGHT, TRUE, iCellLineEQ, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->GetTypeJointPosFootingEQ()), DT_RIGHT);
		m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);

		m_Grid.AddCell(row, col, "내진해석", GVHT_LEFT,CELL_LINE_NO); 
		m_Grid.AddCell(row, col+1, "  내진해석시 기초 모델링 강성", GVHT_LEFT,CELL_LINE_NO);
		sArr.RemoveAll(); 
		sArr.Add("기초의 강성"); 
		sArr.Add("벽체의 강성"); 
		m_Grid.AddCell(row, col+2, &pBri->GetTypePropertyPosFootingEQ(), DT_RIGHT, TRUE, iCellLineEQ, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->GetTypePropertyPosFootingEQ()), DT_RIGHT);
		m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);

		m_Grid.AddCell(row, col, "내진해석", GVHT_LEFT,CELL_LINE_NO); 
		m_Grid.AddCell(row, col+1, "  내진해석시 수평토압 재하 위치", GVHT_LEFT,CELL_LINE_NO);
		sArr.RemoveAll(); 
		sArr.Add("정적하중과 동적하중을 합력 재하 - 도로설계편람(건설교통부,2001)"); 
		sArr.Add("정적하중과 동적하중을 분리 재하 - 내진설계편람(한국도로공사,2000)"); 
		m_Grid.AddCell(row, col+2, &pBri->GetTypeSoilPressureEQ(), DT_RIGHT, TRUE, iCellLineEQ, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->GetTypeSoilPressureEQ()), DT_RIGHT);
		m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);

		m_Grid.AddCell(row, col, "내진해석", GVHT_LEFT,CELL_LINE_NO); 
		m_Grid.AddCell(row, col+1, "  내진해석시 연직중량 산정 범위", GVHT_LEFT,CELL_LINE_NO);
		sArr.RemoveAll(); 
		sArr.Add("상부구조물(슬래브,기타구조물)"); 
		sArr.Add("상부구조물,하부구조1/2"); 
		m_Grid.AddCell(row, col+2, &pBri->GetTypeUpperWeightEQ(), DT_RIGHT, TRUE, iCellLineEQ, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->GetTypeUpperWeightEQ()), DT_RIGHT);
		m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);

		AddCellFootForce(row, col, iCellLineEQ);
	}

	if(m_bDesignLsd == FALSE)
	{
		AddCellFckConc(row, col);
	}
}

// 설계 선택사항
void CInputGenDesignVariable::SetGridDataEnv()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar *pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	BOOL bArch = (pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH);
	const HGBOOL bLsd = m_bDesignLsd;

	// 그리드 초기화
	m_Grid.SetGridDefault(2, 6, 1, 4);

	UINT nFormat = DT_CENTER|DT_VCENTER;

	CStringArray sArr;
	long row = 0;
	long col = 0;
	m_Grid.AddCell(row, col,	  "구  분");
	m_Grid.AddCell(row, col+1,	  "구  분");
	m_Grid.AddCell(row, col+2,	  "구  분");
	m_Grid.AddCell(row, col+3,	  "구  분");
	m_Grid.AddCell(row, col+4, "설정값");
	m_Grid.AddCell(row++, col+5, "설정값");

	if(bLsd == FALSE)
	{
		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  인장철근에 대한 수축온도철근비 검토", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  인장철근에 대한 수축온도철근비 검토", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  인장철근에 대한 수축온도철근비 검토", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckMainSlabAndFootTemper,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bCheckMainSlabAndFootTemper ? "검토" : "검토안함", DT_RIGHT);

		if(pBri->IsBoxType() == FALSE)
		{
			m_Grid.AddCell(row, col, "단면검토 철근");
			m_Grid.AddCell(row, col+1, "  기초 휨검토");
			m_Grid.AddCell(row, col+2, "  온도철근비를 최소철근비로 적용(도로공사)", DT_LEFT);
			m_Grid.AddCell(row, col+3, "  온도철근비를 최소철근비로 적용(도로공사)", DT_LEFT);
			m_Grid.AddCell(row, col+4, &pBri->m_bCheckRebarRateFooting,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
			m_Grid.AddCell(row++, col+5, pBri->m_bCheckRebarRateFooting ? "검토" : "검토안함", DT_RIGHT);
		}

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  수평철근량 검토\n(슬래브)\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  주철근비\n", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  [도.설 4.7.5.7(2)]", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarSlabMain,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bCheckSuppoerBarSlabMain ? "검토" : "검토안함", DT_RIGHT);

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  수평철근량 검토\n(슬래브)\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  주철근비\n", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  주철근의 20%", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarSlabMainOver20,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bCheckSuppoerBarSlabMainOver20 ? "검토" : "검토안함", DT_RIGHT);

		m_nMergeRowSupportBarSlab = row;
		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  수평철근량 검토\n(슬래브)\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  단면적비\n", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  fy가 400MPa 이하인 이형철근", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarSlabArea,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5,&pBri->m_dCheckSuppoerBarSlabArea, DT_RIGHT,CELL_LINE_NO);m_Grid.SetCellDec(CELL_DEC_4);

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  수평철근량 검토\n(슬래브)\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  단면적비\n", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  fy가 400MPa 초과하는 이형철근", DT_LEFT);
		m_Grid.AddCell(row, col+4, "");
		m_Grid.AddCell(row++, col+5, "0.002 x 400/fy", DT_RIGHT, CELL_READONLY);

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  수평철근량 검토\n(슬래브)\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  노출면 300 mm²이상", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  노출면 300 mm²이상", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarSlab300,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bCheckSuppoerBarSlab300 ? "검토" : "검토안함", DT_RIGHT);
		//////////////////////////////////////////////////////////////////////////
		CString strTitle = bArch? "  수평철근량 검토\n(벽체, 아치리브)\n\n" : "  수평철근량 검토\n(벽체)\n\n";
		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, strTitle, DT_LEFT);
		m_Grid.AddCell(row, col+2, "  0.0015hs", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  0.0015hs", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarWall15hs,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5,_T("h:부재두께,s:철근간격"), DT_RIGHT);

		m_nMergeRowSupportBarWall = row;
		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, strTitle, DT_LEFT);
		m_Grid.AddCell(row, col+2, "  단면적비\n", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  fy=400MPa 이상이며 Φ16 이하의 이형철근", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarWallArea,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5,&pBri->m_dCheckSuppoerBarWallAreaForOver400, DT_RIGHT,CELL_LINE_NO);m_Grid.SetCellDec(CELL_DEC_4);

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, strTitle, DT_LEFT);
		m_Grid.AddCell(row, col+2, "  단면적비\n", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  기타 이형철근", DT_LEFT);
		//m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarWallArea,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5,&pBri->m_dCheckSuppoerBarWallArea, DT_RIGHT,CELL_LINE_NO);m_Grid.SetCellDec(CELL_DEC_4);

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, strTitle, DT_LEFT);
		m_Grid.AddCell(row, col+2, "  주철근비", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  주철근의 20%", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarWallMainOver20,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bCheckSuppoerBarWallMainOver20 ? "검토" : "검토안함", DT_RIGHT);

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, strTitle, DT_LEFT);
		m_Grid.AddCell(row, col+2, "  노출면 300 mm²이상", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  노출면 300 mm²이상", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarWall300,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bCheckSuppoerBarWall300 ? "검토" : "검토안함", DT_RIGHT);

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, strTitle, DT_LEFT);
		m_Grid.AddCell(row, col+2, "  노출면 배치 철근량 검토", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  노출면 배치 철근량 검토", DT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_bSelectSupportBarWall,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, "노출면에 1/2이상,2/3이하 배치", DT_RIGHT);
		if(bArch)		// Arch교 일경우 아치리브 배력근 검토 옵션
		{
			m_Grid.AddCell(row, col, "단면검토 철근");
			m_Grid.AddCell(row, col+1, "  수평철근량 검토\n(벽체, 아치리브)\n", DT_LEFT);
			m_Grid.AddCell(row, col+2, "  최소철근직경 13mm이상", DT_LEFT);
			m_Grid.AddCell(row, col+3, "  최소철근직경 13mm이상", DT_LEFT);
			m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarArchRibMinDia1,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
			m_Grid.AddCell(row++, col+5, pBri->m_bCheckSuppoerBarArchRibMinDia1 ? "검토" : "검토안함", DT_RIGHT);

			m_Grid.AddCell(row, col, "단면검토 철근");
			m_Grid.AddCell(row, col+1, "  수평철근량 검토\n(벽체, 아치리브)\n", DT_LEFT);
			m_Grid.AddCell(row, col+2, "  주철근 지름의 1/3이상", DT_LEFT);
			m_Grid.AddCell(row, col+3, "  주철근 지름의 1/3이상", DT_LEFT);
			m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarArchRibMinDia2,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
			m_Grid.AddCell(row++, col+5, pBri->m_bCheckSuppoerBarArchRibMinDia2 ? "검토" : "검토안함", DT_RIGHT);
		}
		//////////////////////////////////////////////////////////////////////////
		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  수평철근량 검토\n(기초)\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  주철근비", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  주철근의 20%", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarFootMainOver20,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bCheckSuppoerBarFootMainOver20 ? "검토" : "검토안함", DT_RIGHT);

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  수평철근량 검토\n(기초)\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  노출면 300 mm²이상", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  노출면 300 mm²이상", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarFoot300,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bCheckSuppoerBarFoot300 ? "검토" : "검토안함", DT_RIGHT);

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  수평철근량 검토\n(기초)\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  0.0015hs", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  0.0015hs", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarFoot15hs,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5,_T("h:부재두께,s:철근간격"), DT_RIGHT);

		m_nMergeRowSupportBarFoot = row;
		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  수평철근량 검토\n(기초)\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  단면적비\n", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  fy가 400MPa 이하인 이형철근", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarFootArea,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5,&pBri->m_dCheckSuppoerBarFootArea, DT_RIGHT,CELL_LINE_NO);m_Grid.SetCellDec(CELL_DEC_4);

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  수평철근량 검토\n(기초)\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  단면적비\n", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  fy가 400MPa 초과하는 이형철근", DT_LEFT);
		m_Grid.AddCell(row, col+4, "");
		m_Grid.AddCell(row++, col+5, "0.002 x 400/fy", DT_RIGHT, CELL_READONLY);

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  수평철근량 검토\n(날개벽)\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  노출면 300 mm²이상", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  노출면 300 mm²이상", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarWing300,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bCheckSuppoerBarWing300 ? "검토" : "검토안함", DT_RIGHT);

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  수평철근량 검토\n(날개벽)\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  0.0015hs", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  0.0015hs", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarWing15hs,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5,_T("h:부재두께,s:철근간격"), DT_RIGHT);

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  수평철근량 검토\n(날개벽)\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  단면적비", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  단면적비", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarWingArea,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5,&pBri->m_dCheckSuppoerBarWingArea, DT_RIGHT,CELL_LINE_NO);m_Grid.SetCellDec(CELL_DEC_4);

		sArr.RemoveAll();
		sArr.Add(hggettext("벽체(기둥)연단"));
		sArr.Add(hggettext("벽체(기둥)연단에서 d위치"));
		sArr.Add(hggettext("벽체(기둥)연단에서 d/2위치"));
		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  전단검토\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  슬래브 전단검토시 위험단면 위치", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  슬래브 전단검토시 위험단면 위치", DT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nApplyCalcShearDistSlab, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nApplyCalcShearDistSlab), DT_RIGHT);

		sArr.RemoveAll();
		sArr.Add(hggettext("슬래브 연단"));
		sArr.Add(hggettext("슬래브 연단에서 d위치"));
		sArr.Add(hggettext("슬래브 연단에서 d/2위치"));
		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  전단검토\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  벽체 전단검토시 위험단면 위치", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  벽체 전단검토시 위험단면 위치", DT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nApplyCalcShearDistWall, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nApplyCalcShearDistWall), DT_RIGHT);

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  전단검토\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  부재 중앙부 전단 검토", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  부재 중앙부 전단 검토", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCalcShearCenter,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bCalcShearCenter ? "검토" : "검토안함", DT_RIGHT);
	}
	else
	{
		sArr.RemoveAll();
		sArr.Add(hggettext("벽체(기둥)연단"));
		sArr.Add(hggettext("벽체(기둥)연단에서 d위치"));
		sArr.Add(hggettext("벽체(기둥)연단에서 d/2위치"));
		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  전단검토\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  슬래브 전단검토시 위험단면 위치", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  슬래브 전단검토시 위험단면 위치", DT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nApplyCalcShearDistSlab, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nApplyCalcShearDistSlab), DT_RIGHT);

		sArr.RemoveAll();
		sArr.Add(hggettext("슬래브 연단"));
		sArr.Add(hggettext("슬래브 연단에서 d위치"));
		sArr.Add(hggettext("슬래브 연단에서 d/2위치"));
		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  전단검토\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  벽체 전단검토시 위험단면 위치", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  벽체 전단검토시 위험단면 위치", DT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nApplyCalcShearDistWall, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nApplyCalcShearDistWall), DT_RIGHT);

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  전단검토\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  부재 중앙부 전단 검토", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  부재 중앙부 전단 검토", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCalcShearCenter,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bCalcShearCenter ? "검토" : "검토안함", DT_RIGHT);
	}

	m_Grid.AddCell(row, col, "단면검토 철근");
	m_Grid.AddCell(row, col+1, "  최소전단 검토\n\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  슬래브", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  슬래브", DT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bApplyMinAvSlab,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bApplyMinAvSlab ? "검토" : "검토안함", DT_RIGHT);

	m_Grid.AddCell(row, col, "단면검토 철근");
	m_Grid.AddCell(row, col+1, "  최소전단 검토\n\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  벽체", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  벽체", DT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bApplyMinAvWall,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bApplyMinAvWall ? "검토" : "검토안함", DT_RIGHT);

	m_Grid.AddCell(row, col, "단면검토 철근");
	m_Grid.AddCell(row, col+1, "  최소전단 검토\n\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  기초", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  기초", DT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bApplyMinAvFoot,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bApplyMinAvFoot ? "검토" : "검토안함", DT_RIGHT);

	m_Grid.AddCell(row, col, "단면검토 철근");
	m_Grid.AddCell(row, col+1, "  최소전단 검토\n\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  날개벽", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  날개벽", DT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bApplyMinAvWing,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bApplyMinAvWing ? "검토" : "검토안함", DT_RIGHT);

	m_Grid.AddCell(row, col, "단면검토 철근");
	m_Grid.AddCell(row, col+1, "  최소전단 검토\n\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  접속슬래브", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  접속슬래브", DT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bApplyMinAvAps,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bApplyMinAvAps ? "검토" : "검토안함", DT_RIGHT);

	m_Grid.AddCell(row, col, "단면검토 철근");
	m_Grid.AddCell(row, col+1, "  최소전단 검토\n\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  현장타설말뚝", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  현장타설말뚝", DT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bApplyMinAvHyunTa,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bApplyMinAvHyunTa ? "검토" : "검토안함", DT_RIGHT);

	//
	m_Grid.AddCell(row, col, "단면검토 철근");
	m_Grid.AddCell(row, col+1, "  날개벽\n");
	m_Grid.AddCell(row, col+2, "  전단검토시 철근간격에 대한 검토 적용", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  전단검토시 철근간격에 대한 검토 적용", DT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bCheckMinCtcShearWing, nFormat, bLsd ? CELL_READONLY : CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bCheckMinCtcShearWing ? "검토" : "검토안함", DT_RIGHT);

	m_Grid.AddCell(row, col, "단면검토 철근");
	m_Grid.AddCell(row, col+1, "  날개벽\n");
	m_Grid.AddCell(row, col+2, "  D구간 철근량 산정시 T1 두께 적용", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  D구간 철근량 산정시 T1 두께 적용", DT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bApplyT1CalcWingWall, nFormat, CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bApplyT1CalcWingWall ? "적용" : "적용안함", DT_RIGHT);

	sArr.RemoveAll();
	sArr.Add(hggettext("벽체연결부"));
	sArr.Add(hggettext("날개벽끝"));
	m_Grid.AddCell(row, col, "단면검토 철근");
	m_Grid.AddCell(row, col+1, "  날개벽\n");
	m_Grid.AddCell(row, col+2, hggettext("  관용법해석시 기준위치"), GVHT_LEFT);
	m_Grid.AddCell(row, col+3, hggettext("  관용법해석시 기준위치"), GVHT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_nApplyWingWallHeightBasePos, DT_RIGHT, TRUE, CELL_LINE_NO, _T(""), CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nApplyWingWallHeightBasePos), DT_RIGHT);
	//
	m_Grid.AddCell(row, col, "단면검토 철근");
	m_Grid.AddCell(row, col+1, "  접속슬래브\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  전단검토시 철근간격에 대한 검토 적용", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  전단검토시 철근간격에 대한 검토 적용", DT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bCheckMinCtcShearAps, nFormat, bLsd? CELL_READONLY : CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bCheckMinCtcShearAps ? "검토" : "검토안함", DT_RIGHT);

	sArr.RemoveAll();
	sArr.Add(hggettext("지점위치"));
	sArr.Add(hggettext("지점에서 d위치"));
	sArr.Add(hggettext("지점에서 d/2위치"));
	m_Grid.AddCell(row, col, "단면검토 철근");
	m_Grid.AddCell(row, col+1, "  접속슬래브\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  전단검토시 하중산정위치", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  전단검토시 하중산정위치", DT_LEFT);
	m_Grid.AddCell(row,  col+4, &pBri->m_nSelectApsShearCheck, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectApsShearCheck), DT_RIGHT);

	if (bLsd == FALSE)
	{
		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  기초 전단검토\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  앞굽 검토시 위험단면 위치", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  앞굽 검토시 위험단면 위치", DT_LEFT);
		sArr.RemoveAll();
		sArr.Add("벽체(기둥)전면");
		sArr.Add("벽체(기둥)전면에서 d위치");
		sArr.Add("벽체(기둥)전면에서 d/2위치");
		m_Grid.AddCell(row,  col+4, &pBri->m_nApplyCalcShearDistFront, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nApplyCalcShearDistFront), DT_RIGHT);

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  기초 전단검토\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  뒷굽 검토시 위험단면 위치", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  뒷굽 검토시 위험단면 위치", DT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nApplyCalcShearDistBack, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nApplyCalcShearDistBack), DT_RIGHT);

		sArr.RemoveAll(); 
		long i=0; for(i=0; i<MAX_CRACK_ENV_TYPE; i++) sArr.Add(pStd->GetStringCrackDesign(i));
		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  부식에대한\n 환경조건\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  슬래브", DT_LEFT); 
		m_Grid.AddCell(row, col+3, "  슬래브", DT_LEFT); 
		m_Grid.AddCell(row,  col+4, &pBri->m_nTypeCrackDesignSlab, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_AUTO, sArr, &m_DlgCrackEnv);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypeCrackDesignSlab), DT_RIGHT);

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  부식에대한\n 환경조건\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  벽체", DT_LEFT); 
		m_Grid.AddCell(row, col+3, "  벽체", DT_LEFT); 
		m_Grid.AddCell(row,  col+4, &pBri->m_nTypeCrackDesignWall, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_AUTO, sArr, &m_DlgCrackEnv);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypeCrackDesignWall), DT_RIGHT);

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  부식에대한\n 환경조건\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  기초", DT_LEFT); 
		m_Grid.AddCell(row, col+3, "  기초", DT_LEFT); 
		m_Grid.AddCell(row,  col+4, &pBri->m_nTypeCrackDesignFoot, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_AUTO, sArr, &m_DlgCrackEnv);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypeCrackDesignFoot), DT_RIGHT);

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  부식에대한\n 환경조건\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  날개벽", DT_LEFT); 
		m_Grid.AddCell(row, col+3, "  날개벽", DT_LEFT); 
		m_Grid.AddCell(row,  col+4, &pBri->m_nTypeCrackDesignWing, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_AUTO, sArr, &m_DlgCrackEnv);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypeCrackDesignWing), DT_RIGHT);

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  부식에대한\n 환경조건\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  접속슬래브", DT_LEFT); 
		m_Grid.AddCell(row, col+3, "  접속슬래브", DT_LEFT); 
		m_Grid.AddCell(row,  col+4, &pBri->m_nTypeCrackDesignAps, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_AUTO, sArr, &m_DlgCrackEnv);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypeCrackDesignAps), DT_RIGHT);
		if(pStd->m_nTypeKciCode != DESIGN_CODE_KCI_2003)
		{
			m_Grid.AddCell(row, col, "단면검토 철근");
			m_Grid.AddCell(row, col+1, "  철근간격검토시 1단철근 피복위치로 fs보정", DT_LEFT);
			m_Grid.AddCell(row, col+2, "  철근간격검토시 1단철근 피복위치로 fs보정", DT_LEFT);
			m_Grid.AddCell(row, col+3, "  철근간격검토시 1단철근 피복위치로 fs보정", DT_LEFT);
			sArr.RemoveAll();
			sArr.Add("적용");
			sArr.Add("적용안함");
			m_Grid.AddCell(row,  col+4, &pBri->m_nSelectApplyFs, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
			m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectApplyFs), DT_RIGHT);
		}

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  기초두께 상한값 적용 (도.설.하 p85)", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  기초두께 상한값 적용 (도.설.하 p85)", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  기초두께 상한값 적용 (도.설.하 p85)", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bApplyFootingMinThick,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bApplyFootingMinThick ? "적용" : "적용안함", DT_RIGHT);
	}
	else
	{
		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  기초 전단검토\n\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  앞굽 검토시 위험단면 위치(직접기초)", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  앞굽 검토시 위험단면 위치(직접기초)", DT_LEFT);
		
		sArr.RemoveAll();
		sArr.Add("벽체(기둥)전면");
		sArr.Add("벽체(기둥)전면에서 d위치");
		sArr.Add("벽체(기둥)전면에서 d/2위치");
		m_Grid.AddCell(row,  col+4, &pBri->m_nApplyCalcShearDistFront, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nApplyCalcShearDistFront), DT_RIGHT);

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  기초 전단검토\n\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  뒷굽 검토시 위험단면 위치(직접기초)", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  뒷굽 검토시 위험단면 위치(직접기초)", DT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nApplyCalcShearDistBack, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nApplyCalcShearDistBack), DT_RIGHT);

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  기초 전단검토\n\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  앞굽 검토시 위험단면 위치(말뚝기초)", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  앞굽 검토시 위험단면 위치(말뚝기초)", DT_LEFT);

		sArr.RemoveAll();
		sArr.Add("벽체(기둥)전면");
		sArr.Add("벽체(기둥)전면에서 가장가까운 말뚝(환산A)");
		sArr.Add("벽체(기둥)전면에서 가장가까운 말뚝(0.1D)");
		m_Grid.AddCell(row,  col+4, &pBri->m_nApplyCalcShearDistFrontPile, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nApplyCalcShearDistFrontPile), DT_RIGHT);

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  기초 전단검토\n\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  뒷굽 검토시 위험단면 위치(말뚝기초)", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  뒷굽 검토시 위험단면 위치(말뚝기초)", DT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nApplyCalcShearDistBackPile, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nApplyCalcShearDistBackPile), DT_RIGHT);

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  전단검토시 Θ : 0 = 자동계산", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  전단검토시 Θ : 0 = 자동계산", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  전단검토시 Θ : 0 = 자동계산", GVHT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_dThetaShearDesign, DT_RIGHT,0);
		m_Grid.AddCell(row++, col+5, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dThetaShearDesign), DT_RIGHT,CELL_LINE_NO);

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  전단력에 의한 종방향 추가인장력 검토", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  전단력에 의한 종방향 추가인장력 검토", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  전단력에 의한 종방향 추가인장력 검토", GVHT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckDeltaTR,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bCheckDeltaTR ? "검토" : "검토안함", DT_RIGHT);

		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  사용성 검토시 유효탄성계수비 : 0 = 탄성계수비", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  사용성 검토시 유효탄성계수비 : 0 = 탄성계수비", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  사용성 검토시 유효탄성계수비 : 0 = 탄성계수비", GVHT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_dElasticModularRatioUseCheck, DT_RIGHT,0);
		m_Grid.AddCell(row++, col+5, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dElasticModularRatioUseCheck), DT_RIGHT,CELL_LINE_NO);

		sArr.RemoveAll();
		sArr.Add("균열폭 검토");
		sArr.Add("간접균열 검토");
		sArr.Add("균열폭, 간접균열 검토");
		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  사용성 검토시 균열검토", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  사용성 검토시 균열검토", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  사용성 검토시 균열검토", GVHT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nSelectUsebilityCheck, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectUsebilityCheck), DT_RIGHT);

		sArr.RemoveAll();
		if(pStd->IsDesignKDS2019())
		{
			sArr.Add(hggettext("KDS 24 14 21 4.2.3.2"));
			pBri->m_nTypeCrackWidthCondition = 0;
		}
		else
		{
			sArr.Add(hggettext("도설2015 5.8.3.4"));
			sArr.Add(hggettext("도설2012 5.6.3.4"));
		}
		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  균열폭계산", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  균열폭계산", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  균열폭계산", GVHT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nTypeCrackWidthCondition, DT_RIGHT, TRUE, pStd->IsDesignKDS2019() ? CELL_READONLY : CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypeCrackWidthCondition), DT_RIGHT);

		sArr.RemoveAll();
		sArr.Add("주철근/2 + 전단철근");
		sArr.Add("주철근/2");
		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  설계피복 산정", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  설계피복 산정", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  설계피복 산정", GVHT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nSelectExposureCoverCheck, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectExposureCoverCheck), DT_RIGHT);

		sArr.RemoveAll();
		sArr.Add("0.135fck/fy ");
		sArr.Add("0.1Nu(fyd x Ag) ");
		m_Grid.AddCell(row, col, "단면검토 철근");
		m_Grid.AddCell(row, col+1, "  기둥 최소 철근비", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  기둥 최소 철근비", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  기둥 최소 철근비", GVHT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nSelectPminColumn, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectPminColumn), DT_RIGHT);
	}
	
	m_Grid.AddCell(row, col, "단면검토 철근");
	m_Grid.AddCell(row, col+1, "  단면검토시 헌치제원 적용", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  단면검토시 헌치제원 적용", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  단면검토시 헌치제원 적용", GVHT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bSelectHunchDesignCheck,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bSelectHunchDesignCheck ? "적용" : "적용안함", DT_RIGHT);

	if (bLsd == FALSE)
	{
		m_Grid.AddCell(row, col, "구조계산");
		m_Grid.AddCell(row, col+1, "  날개벽 계산시\n  하중계수", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  수평토압", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  수평토압", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_dFactorToap, DT_RIGHT,0);m_Grid.SetCellDec(CELL_DEC_2);
		m_Grid.AddCell(row++, col+5, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dFactorToap), DT_RIGHT,0);m_Grid.SetCellDec(CELL_DEC_2);

		m_Grid.AddCell(row, col, "구조계산");
		m_Grid.AddCell(row, col+1, "  날개벽 계산시\n  하중계수", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  상재하중", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  상재하중", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_dFactorLive, DT_RIGHT,0);m_Grid.SetCellDec(CELL_DEC_2);
		m_Grid.AddCell(row++, col+5, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dFactorLive), DT_RIGHT,0);m_Grid.SetCellDec(CELL_DEC_2);
	}

	if(pBri->IsBoxType() == FALSE)
	{
		long nBaseIndex(1);
		// 토압계수 산정시 수평지진계수
		m_Grid.AddCell(row, col, "구조계산");
		m_Grid.AddCell(row, col+1, "  토압계수 산정시\n 수평지진계수", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  시점 벽체", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  시점 벽체", GVHT_LEFT);
		sArr.RemoveAll();
//		sArr.Add("자동");		(ARCBRIDGE-3535) 자동은 선택에서 제거
		if(pStd->m_nTypeEarthQuakeApply == 0)
		{
			sArr.Add("0.5A");
			sArr.Add("1.0A");
			sArr.Add("1.5A");
			sArr.Add("0.5A X S");
			sArr.Add("1.0A X S");
			sArr.Add("1.5A X S");
		}
		else
		{
			sArr.Add("0.5S");
			sArr.Add("1.0S");
			sArr.Add("1.5S");
			sArr.Add("0.5S X 지반보정계수");
			sArr.Add("1.0S X 지반보정계수");
			sArr.Add("1.5S X 지반보정계수");
		}
		m_Grid.AddCell(row,  col+4, &pBri->m_nSelFactorKhStt, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);  m_Grid.SetCellBaseOfIndex(nBaseIndex);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelFactorKhStt - nBaseIndex), DT_RIGHT);

		m_Grid.AddCell(row, col, "구조계산");
		m_Grid.AddCell(row, col+1, "  토압계수 산정시\n 수평지진계수", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  종점 벽체", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  종점 벽체", GVHT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nSelFactorKhEnd, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);	  m_Grid.SetCellBaseOfIndex(nBaseIndex);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelFactorKhEnd - nBaseIndex), DT_RIGHT);
	}

	sArr.RemoveAll();
	sArr.Add("표준 점유폭 3.0 m마다 적용");
	sArr.Add("W = Wc / N 으로 계산");
	m_Grid.AddCell(row, col, "구조계산");
	m_Grid.AddCell(row, col+1, "활하중\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  계산시의 설계 차로폭(W)", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  계산시의 설계 차로폭(W)", GVHT_LEFT);
	m_Grid.AddCell(row,  col+4, &pBri->m_nSelectDesignRoadWidth, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectDesignRoadWidth), DT_RIGHT);

	m_Grid.AddCell(row, col, "구조계산");
	m_Grid.AddCell(row, col+1, "활하중\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  계산시의 지간길이 적용", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  계산시의 지간길이 적용", GVHT_LEFT);
	sArr.RemoveAll();
	sArr.Add("설계지간 산정 방법에 따라 적용");
	sArr.Add("경사길이 적용");
	sArr.Add("직각길이 적용");
	m_Grid.AddCell(row,  col+4, &pBri->m_nSelectLiveLoadSpanLength, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectLiveLoadSpanLength), DT_RIGHT);

	sArr.RemoveAll();
	sArr.Add("좌측 부터 재하");
	sArr.Add("우측 부터 재하");
	m_Grid.AddCell(row, col, "구조계산");
	m_Grid.AddCell(row, col+1, "활하중\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  재하시의 차선 재하기준", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  재하시의 차선 재하기준", GVHT_LEFT);
	m_Grid.AddCell(row,  col+4, &pBri->m_nSelectStartLane, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectStartLane), DT_RIGHT);

	m_Grid.AddCell(row, col, "구조계산");
	m_Grid.AddCell(row, col+1, "활하중\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  계산시 충격계수 사용자입력(i)", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  계산시 충격계수 사용자입력(i)", GVHT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bCheckUseri,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5,&pBri->m_dCheckUseriValue, DT_RIGHT,CELL_LINE_NO);m_Grid.SetCellDec(CELL_DEC_4);

	m_Grid.AddCell(row, col, "구조계산");
	m_Grid.AddCell(row, col+1, "  기초작용력 계산시 기초관성력 적용", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  기초작용력 계산시 기초관성력 적용", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  기초작용력 계산시 기초관성력 적용", DT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bApplyFootingGForce,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bApplyFootingGForce ? "검토" : "검토안함", DT_RIGHT);
	//
	m_Grid.AddCell(row, col, "구조계산");
	m_Grid.AddCell(row, col+1, "  기초 지지력 검토시 MASS 작용력 포함", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  기초 지지력 검토시 MASS 작용력 포함", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  기초 지지력 검토시 MASS 작용력 포함", DT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bApplyMassCalcSafe,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bApplyMassCalcSafe ? "검토" : "검토안함", DT_RIGHT);

	m_Grid.AddCell(row, col, "구조계산");
	m_Grid.AddCell(row, col+1, "  지점부 단면검토시 부재력 위치", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  지점부 단면검토시 부재력 위치", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  지점부 단면검토시 부재력 위치", GVHT_LEFT);
	sArr.RemoveAll();
	
	if(pBri->m_nSelectRigidModeling != 0)
	{
		sArr.Add("강역구간의 절점에서 선택");
		sArr.Add("위험단면의 절점에서 선택");

		m_Grid.AddCell(row,  col+4, &pBri->m_nSelectForceCheckPos, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectForceCheckPos), DT_RIGHT);
	}
	else
	{
		// (ARCBRIDGE-2368) 강역적용 안했을때 위험단면적용 하지않고 최대부재력 적용할수 있도록.
		sArr.Add("최대부재력 위치에서 선택");
		sArr.Add("위험단면의 절점에서 선택");

		m_Grid.AddCell(row,  col+4, &pBri->m_nSelectForceCheckPos, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectForceCheckPos), DT_RIGHT);
	}

	sArr.RemoveAll();
	sArr.Add("CASE별로 집계");
	sArr.Add("부호별로 집계");
	sArr.Add("최대작용력집계(모멘트최대-축력최소)"); // 큰값으로 집계에서 명칭만 변경
	sArr.Add("최대작용력집계(모멘트최대-축력최대)");
	m_Grid.AddCell(row, col, "구조계산");
	m_Grid.AddCell(row, col+1, "  기초 검토시 반력 집계 방법", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  기초 검토시 반력 집계 방법", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  기초 검토시 반력 집계 방법", GVHT_LEFT);
	m_Grid.AddCell(row,  col+4, &pBri->m_nSelectFootingReactionForce, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectFootingReactionForce), DT_RIGHT);

	m_Grid.AddCell(row, col, "구조계산");
	if (bLsd)
	{
		m_Grid.AddCell(row, col+1, "  경사교각(Type2)의 앞굽검토(외측벽체연단)", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  경사교각(Type2)의 앞굽검토(외측벽체연단)", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  경사교각(Type2)의 앞굽검토(외측벽체연단)", GVHT_LEFT);
	}
	else
	{
		m_Grid.AddCell(row, col+1, "  경사교각(Type2), 아치형라멘교의 앞굽검토(외측벽체연단)", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  경사교각(Type2), 아치형라멘교의 앞굽검토(외측벽체연단)", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  경사교각(Type2), 아치형라멘교의 앞굽검토(외측벽체연단)", GVHT_LEFT);
	}
	
	m_Grid.AddCell(row, col+4, &pBri->m_bCheckFrontFootTrap2,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bCheckFrontFootTrap2 ? "적용" : "적용안함", DT_RIGHT);

	BOOL bApplyType = (pBri->m_nTypeBridge==BRIDGE_TYPE_TRAP2 || pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH)? TRUE : FALSE;
	m_Grid.AddCell(row, col, "구조계산");
	if (bLsd)
	{
		m_Grid.AddCell(row, col+1, "  경사교각(TYPE-2) 기초부등침하 가정", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  경사교각(TYPE-2) 기초부등침하 가정", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  경사교각(TYPE-2) 기초부등침하 가정", GVHT_LEFT);
	}
	else
	{
		m_Grid.AddCell(row, col+1, "  경사교각(TYPE-2), 아치형 라멘교 기초부등침하 가정", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  경사교각(TYPE-2), 아치형 라멘교 기초부등침하 가정", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  경사교각(TYPE-2), 아치형 라멘교 기초부등침하 가정", GVHT_LEFT);
	}
	
	sArr.RemoveAll();
	sArr.Add("각각의 벽체별로 부등침하 적용");
	sArr.Add("기초전체를 하나의 부등침하로 적용");
	m_Grid.AddCell(row,  col+4, &pBri->m_nSelSubsideType, DT_RIGHT, TRUE, bApplyType? CELL_LINE_NO : CELL_READONLY, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelSubsideType), DT_RIGHT);

	if (bLsd == FALSE)
	{
		//
		m_Grid.AddCell(row, col, "구조계산");
		m_Grid.AddCell(row, col+1, "  절점부 보강철근 검토시 내측인장 대한 검토 적용", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  절점부 보강철근 검토시 내측인장 대한 검토 적용", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  절점부 보강철근 검토시 내측인장 대한 검토 적용", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckCornerInside,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bCheckCornerInside ? "검토" : "검토안함", DT_RIGHT);

		// 단절점부 설계 선택옵션 추가... 2005-11-24 9:32
		m_Grid.AddCell(row, col, "구조계산");
		m_Grid.AddCell(row, col+1, "  절점부 보강철근 판정시 인장응력 ft 선택", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  절점부 보강철근 판정시 인장응력 ft 선택", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  절점부 보강철근 판정시 인장응력 ft 선택", DT_LEFT);
		sArr.RemoveAll();
		sArr.Add(pStd->IsSI()? "0.08√fck" : "0.25√fck");
		sArr.Add(pStd->IsSI()? "1/3√fck" : "1.06√fck");
		sArr.Add(pStd->IsSI()? "0.13√fck" : "0.42√fck");
		sArr.Add(pStd->IsSI()? "0.22√fck" : "0.7√fck");
		m_Grid.AddCell(row,  col+4, &pBri->m_nSelCalcCornerFt, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelCalcCornerFt), DT_RIGHT);
	}

	sArr.RemoveAll();
	//	sArr.Add("슬래브로 검토 : 최대 모멘트");		// 27258 
	//	sArr.Add("벽체로 검토 : 최대모멘트,최대축력");
	// (ARCBRIDGE-3616) 강도설계법도 옵션명 변경. LSD/강도설계법 구분없이 LSD와 같게 변경
//	if (bLsd)
	{
		sArr.Add("휨부재로 검토");
		sArr.Add("휨압축부재로 검토");
	}
// 	else
// 	{
// 		sArr.Add("슬래브로 검토");
// 		sArr.Add("벽체로 검토");
	// 	}

	m_Grid.AddCell(row, col, "구조계산");
	m_Grid.AddCell(row, col+1, "  시.종점부 벽체 검토", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  시.종점부 벽체 검토", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  시.종점부 벽체 검토", GVHT_LEFT);
	m_Grid.AddCell(row,  col+4, &pBri->m_nSelectWallSectionDesign, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectWallSectionDesign), DT_RIGHT);

	if(pBri->m_nSelectWallSectionDesign == 1)
	{
		m_Grid.AddCell(row, col, "구조계산");
		m_Grid.AddCell(row, col+1, "  시,종점부 벽체 휨압축부재 사용성검토", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  시,종점부 벽체 휨압축부재 사용성검토", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  시,종점부 벽체 휨압축부재 사용성검토", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bOutWallUsebilityCheck,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bOutWallUsebilityCheck ? "검토" : "검토안함", DT_RIGHT);
	}

	m_Grid.AddCell(row, col, "구조계산");
	m_Grid.AddCell(row, col+1, "  중간지점부 벽체 검토", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  중간지점부 벽체 검토", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  중간지점부 벽체 검토", GVHT_LEFT);
	m_Grid.AddCell(row,  col+4, &pBri->m_nSelectInWallSectionDesign, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectInWallSectionDesign), DT_RIGHT);

	if(bLsd)
	{
		sArr.RemoveAll();
		sArr.Add(hggettext("텐덤축(2-135kN) "));
		sArr.Add(hggettext("후륜(192kN)"));
		m_Grid.AddCell(row, col, "구조계산");
		m_Grid.AddCell(row, col+1, hggettext("  접속슬래브 활하중산정시 축중 적용"), GVHT_LEFT);
		m_Grid.AddCell(row, col+2, hggettext("  접속슬래브 활하중산정시 축중 적용"), GVHT_LEFT);
		m_Grid.AddCell(row, col+3, hggettext("  접속슬래브 활하중산정시 축중 적용"), GVHT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nSelectApsAxleWeight, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectApsAxleWeight), DT_RIGHT);

		sArr.RemoveAll();
		sArr.Add(hggettext("Dc"));
		sArr.Add(hggettext("Dc/Dw"));
		sArr.Add(hggettext("Dc/Dw/Ev"));
		m_Grid.AddCell(row, col, "구조계산");
		m_Grid.AddCell(row, col+1, hggettext("  접속슬래브 고정하중"), GVHT_LEFT);
		m_Grid.AddCell(row, col+2, hggettext("  접속슬래브 고정하중"), GVHT_LEFT);
		m_Grid.AddCell(row, col+3, hggettext("  접속슬래브 고정하중"), GVHT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nSelectApsDeadLoad, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectApsDeadLoad), DT_RIGHT);
	}

	//////////////////////////////////////////////////////////////////////////
	sArr.RemoveAll();
	sArr.Add("강역적용 안함");
	sArr.Add("무한강성 적용");
	sArr.Add("EndOffset 적용");

	m_Grid.AddCell(row, col, "모델링");
	m_Grid.AddCell(row, col+1, "  모델링 해석\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  강역구간 적용", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  강역구간 적용", GVHT_LEFT);
	m_Grid.AddCell(row,  col+4, &pBri->m_nSelectRigidModeling, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectRigidModeling), DT_RIGHT);

	m_Grid.AddCell(row, col, "모델링");
	m_Grid.AddCell(row, col+1, "  모델링 해석\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  벽체 높이 설정", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  벽체 높이 설정", GVHT_LEFT);
	sArr.RemoveAll();
	sArr.Add("도로 선형 중심위치 높이");
	sArr.Add("횡단면 최대 높이");
	m_Grid.AddCell(row,  col+4, &pBri->m_nSelectModelingHeight, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectModelingHeight), DT_RIGHT);

	if (bLsd == FALSE)
	{
		m_Grid.AddCell(row, col, "모델링");
		m_Grid.AddCell(row, col+1, "  모델링 해석\n\n", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  슬래브 온도하중 적용", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  슬래브 온도하중 적용", GVHT_LEFT);
		sArr.RemoveAll();
		sArr.Add("온도하중(+/-), 상하면 온도차 재하");
		sArr.Add("온도하중(+/-) 재하");
		sArr.Add("상하면 온도차 재하");
		m_Grid.AddCell(row,  col+4, &pBri->m_nSelectTempLoadSlab, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectTempLoadSlab), DT_RIGHT);
	}

	m_Grid.AddCell(row, col, "모델링");
	m_Grid.AddCell(row, col+1, "  모델링 해석\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  온도하중 벽체에 재하", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  온도하중 벽체에 재하", GVHT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bSelectTempLoadAddWall,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bSelectTempLoadAddWall ? "적용" : "적용안함", DT_RIGHT);

	m_Grid.AddCell(row, col, "모델링");
	m_Grid.AddCell(row, col+1, "  모델링 해석\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  건조수축 벽체에 재하", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  건조수축 벽체에 재하", GVHT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bSelectShrinkLoadAddWall,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bSelectShrinkLoadAddWall ? "적용" : "적용안함", DT_RIGHT);

	if (bLsd == FALSE)
	{
		m_Grid.AddCell(row, col, "모델링");
		m_Grid.AddCell(row, col+1, "  모델링 해석\n\n", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  ENV 조합 적용 : 편토압", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  ENV 조합 적용 : 편토압", GVHT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bSelectEnvToapOne,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bSelectEnvToapOne ? "적용" : "적용안함", DT_RIGHT);
	}

	m_Grid.AddCell(row, col, "모델링");
	m_Grid.AddCell(row, col+1, "  모델링 해석\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  ENV 조합 적용 : 지점침하", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  ENV 조합 적용 : 지점침하", GVHT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bSelectEnvSubsid,nFormat, (pBri->m_nSelect2DFrameTool == SOLVE_MIDAS || pBri->GetCountJijum() > 5) ? CELL_READONLY : CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bSelectEnvSubsid ? "적용" : "적용안함", DT_RIGHT, (pBri->m_nSelect2DFrameTool == SOLVE_MIDAS || pBri->GetCountJijum() > 5) ? CELL_READONLY : CELL_LINE_NO);

	m_Grid.AddCell(row, col, "모델링");
	m_Grid.AddCell(row, col+1, "  모델링 해석\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  단면특성에 헌치 고려", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  단면특성에 헌치 고려", GVHT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bSelectHunchModeling,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bSelectHunchModeling ? "적용" : "적용안함", DT_RIGHT);
	//////////////////////////////////////////////////////////////////////////

	m_Grid.AddCell(row, col, "모델링");
	m_Grid.AddCell(row, col+1, "  중간지점부 강역구간 계산시 슬래브 헌치 포함", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  중간지점부 강역구간 계산시 슬래브 헌치 포함", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  중간지점부 강역구간 계산시 슬래브 헌치 포함", GVHT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bSelectRigidCalcHunch,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bSelectRigidCalcHunch ? "적용" : "적용안함", DT_RIGHT);
	
	m_Grid.AddCell(row, col, "모델링");
	m_Grid.AddCell(row, col+1, "  설계지간 산정시 적용 길이", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  설계지간 산정시 적용 길이", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  설계지간 산정시 적용 길이", GVHT_LEFT);
	sArr.RemoveAll();
	sArr.Add("최소 지간 길이");
	sArr.Add("교량 전체 길이");
	sArr.Add("최대 전체 길이");
	m_Grid.AddCell(row,  col+4, &pBri->m_nSelectDesignSpanLength, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectDesignSpanLength), DT_RIGHT);

	m_Grid.AddCell(row, col, "모델링");
	m_Grid.AddCell(row, col+1, "  상시 모델링시 경사지간 고려", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  상시 모델링시 경사지간 고려", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  상시 모델링시 경사지간 고려", GVHT_LEFT);
	sArr.RemoveAll();
	sArr.Add("자동");
	sArr.Add("경사지간 고려");
	sArr.Add("직각지간 고려");
	sArr.Add("(경사+직각)/2 고려");
	m_Grid.AddCell(row,  col+4, &pBri->m_nSelectDesignSpanApply, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectDesignSpanApply), DT_RIGHT);

	m_Grid.AddCell(row, col, "모델링");
	m_Grid.AddCell(row, col+1, "  설계지간 산정시 받침위치 고려", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  설계지간 산정시 받침위치 고려", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  설계지간 산정시 받침위치 고려", GVHT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bSelectSpanLengthApplyShoe,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bSelectSpanLengthApplyShoe ? "적용" : "적용안함", DT_RIGHT);

	if (bLsd == FALSE)
	{
		sArr.RemoveAll();
		sArr.Add("평균단면2차 모멘트");
		sArr.Add("검토위치단면2차 모멘트");
		m_Grid.AddCell(row, col, "모델링");
		m_Grid.AddCell(row, col+1, "  아치리브 검토시 단면2차 모멘트의 산정", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  아치리브 검토시 단면2차 모멘트의 산정", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  아치리브 검토시 단면2차 모멘트의 산정", GVHT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nArchRibSectionMomentPos, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nArchRibSectionMomentPos), DT_RIGHT);
	}

	m_Grid.AddCell(row, col, "도면");
	m_Grid.AddCell(row, col+1, "  상부이음,정착장 적용 범위", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  상부이음,정착장 적용 범위", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  상부이음,정착장 적용 범위", DT_LEFT);
	sArr.RemoveAll();
	sArr.Add("주철근, 배력근");
	sArr.Add("주철근");
	sArr.Add("적용안함");
	m_Grid.AddCell(row,  col+4, &pBri->m_nSelectJointUpperRange, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectJointUpperRange), DT_RIGHT);

	// 배근시 종단 표현 방법
	BOOL bTrap = pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP || pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 || pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH;
	if(bTrap || (pBri->IsBoxType() && pBri->m_bSlopeLowerSlab == FALSE))
	{
		pBri->m_nTypeJongdanForRebar	= 1;
	}
	m_Grid.AddCell(row, col, "도면");
	m_Grid.AddCell(row, col+1, "  도면표현방법\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  종단(구조도)", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  종단(구조도)", DT_LEFT);
	sArr.RemoveAll();
	sArr.Add("LEVEL");
	sArr.Add("경사");
	m_Grid.AddCell(row,  col+4, &pBri->m_nTypeJongdanForRebar, DT_RIGHT, TRUE, (bTrap || (pBri->IsBoxType() && pBri->m_bSlopeLowerSlab == FALSE)) ? CELL_READONLY : CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypeJongdanForRebar), DT_RIGHT);

	m_Grid.AddCell(row, col, "도면");
	m_Grid.AddCell(row, col+1, "  도면표현방법\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  외측벽체 정면도(일반도, 구조도)", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  외측벽체 정면도(일반도, 구조도)", DT_LEFT);
	sArr.RemoveAll();
	sArr.Add("슬래브 끝단 기준");
	sArr.Add("시, 종점 기준");
	//sArr.Add("실제 Sta. 기준");
	m_Grid.AddCell(row,  col+4, &pBri->m_nTypeWallForRebarGen, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypeWallForRebarGen), DT_RIGHT);

	m_Grid.AddCell(row, col, "도면");
	m_Grid.AddCell(row, col+1, "  철근배치 옵션\n\n\n\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  주철근 배치", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  주철근 배치", DT_LEFT);
	sArr.RemoveAll();
	sArr.Add("2CYCLE");
	sArr.Add("4CYCLE");
	m_Grid.AddCell(row,  col+4, &pBri->m_nIdxCountCycleMainRebar, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nIdxCountCycleMainRebar), DT_RIGHT);

	m_Grid.AddCell(row, col, "도면");
	m_Grid.AddCell(row, col+1, "  철근배치 옵션\n\n\n\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  주철근 간격", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  주철근 간격", DT_LEFT);
	sArr.RemoveAll();
	sArr.Add("상부슬래브 기준");
	sArr.Add("벽체 기준");
	m_Grid.AddCell(row, col+4, &pBri->m_nTypeStandardCTCRebarMain, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypeStandardCTCRebarMain), DT_RIGHT);

	m_Grid.AddCell(row, col, "도면");
	m_Grid.AddCell(row, col+1, "  철근배치 옵션\n\n\n\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  슬래브 배력근", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  슬래브 배력근", DT_LEFT);
	sArr.RemoveAll();
	sArr.Add("시점각도기준 경사방향 배치");
	sArr.Add("종점각도기준 경사방향 배치");
	sArr.Add("사보강배치(전체)");
	sArr.Add("사보강배치(절반)");
	m_Grid.AddCell(row, col+4, &pBri->m_nTypePlacingSupportRebar_UpperSlab, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypePlacingSupportRebar_UpperSlab), DT_RIGHT);

	m_Grid.AddCell(row, col, "도면");
	m_Grid.AddCell(row, col+1, "  철근배치 옵션\n\n\n\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  슬래브 주철근", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  슬래브 주철근", DT_LEFT);
	sArr.RemoveAll();
	sArr.Add("선형방향 배치");
	sArr.Add("사보강배치(전체)");
	sArr.Add("사보강배치(절반)");
	m_Grid.AddCell(row,  col+4, &pBri->m_nTypePlacingMainRebar_UpperSlab, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypePlacingMainRebar_UpperSlab), DT_RIGHT);

	m_Grid.AddCell(row, col, "도면");
	m_Grid.AddCell(row, col+1, "  철근배치 옵션\n\n\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  슬래브 주철근 사방향철근 형태", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  슬래브 주철근 사방향철근 형태", GVHT_LEFT);
	sArr.RemoveAll();
	sArr.Add("직선 형태");
	sArr.Add("좌우측 선형 형태");
	m_Grid.AddCell(row,  col+4, &pBri->m_nTypeSideRebarMain_UpperSlabSabogang, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypeSideRebarMain_UpperSlabSabogang), DT_RIGHT);

	m_Grid.AddCell(row, col, "도면");
	m_Grid.AddCell(row, col+1, "  철근배치 옵션\n\n\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  기초 주철근", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  기초 주철근", GVHT_LEFT);
	sArr.RemoveAll();
	sArr.Add("선형방향 배치");
	sArr.Add("사보강배치(전체)");
	sArr.Add("사보강배치(절반)");
	sArr.Add("기초좌측면 기준 배치");
	sArr.Add("기초우측면 기준 배치");
	m_Grid.AddCell(row,  col+4, &pBri->m_nTypePlacingMainRebar_Footing, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypePlacingMainRebar_Footing), DT_RIGHT);

	m_Grid.AddCell(row, col, "도면");
	m_Grid.AddCell(row, col+1, "  철근배치 옵션\n\n\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  벽체 수직철근 절곡부 처리", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  벽체 수직철근 절곡부 처리", GVHT_LEFT);
	sArr.RemoveAll();
	sArr.Add("자동(D22이상 곡선)");
	sArr.Add("곡선");
	sArr.Add("직선");
	m_Grid.AddCell(row,  col+4, &pBri->m_nTypeCurveOfWallVertRebar, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypeCurveOfWallVertRebar), DT_RIGHT);

	m_Grid.AddCell(row, col, "도면");
	m_Grid.AddCell(row, col+1, "  철근배치 옵션\n\n\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  최외측 주철근 1Cycle로 고정", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  최외측 주철근 1Cycle로 고정", GVHT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bFixOutSideMainRebarBy1Cyle, nFormat, CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bFixOutSideMainRebarBy1Cyle ? "적용" : "적용안함", DT_RIGHT);

	m_Grid.AddCell(row, col, "도면");
	m_Grid.AddCell(row, col+1, "  철근배치 옵션\n\n\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  슬래브 연단 전단철근 제거", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  슬래브 연단 전단철근 제거", GVHT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bRemoveShearRebarAtSlabYundan, nFormat, CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bRemoveShearRebarAtSlabYundan ? "적용" : "적용안함", DT_RIGHT);

	m_Grid.AddCell(row, col, "도면");
	m_Grid.AddCell(row, col+1, "  철근배치 옵션\n\n\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  기둥부 주철근 이음방법", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  기둥부 주철근 이음방법", GVHT_LEFT);
	sArr.RemoveAll();
	sArr.Add("커플러");
	sArr.Add("겹이음");
	m_Grid.AddCell(row,  col+4, &pBri->m_nTypeCouplerColumnMainRebar, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypeCouplerColumnMainRebar), DT_RIGHT);

	m_Grid.AddCell(row, col, "도면");
	m_Grid.AddCell(row, col+1, "  철근배치 옵션\n\n\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  상하행 배근 직경 및 간격 동일적용(배력,전단철근)", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  상하행 배근 직경 및 간격 동일적용(배력,전단철근)", GVHT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bApplySameSupportRebar, nFormat, CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bApplySameSupportRebar ? "적용" : "적용안함", DT_RIGHT);

	m_Grid.AddCell(row, col, "도면");
	m_Grid.AddCell(row, col+1, "  철근배치 옵션\n\n\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  상하행 배근 직경 및 간격 동일적용(주철근)", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  상하행 배근 직경 및 간격 동일적용(주철근)", GVHT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bApplySameMainRebar, nFormat, CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bApplySameMainRebar ? "적용" : "적용안함", DT_RIGHT);

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);

	m_Grid.SetColumnWidth(0,100);
	m_Grid.SetColumnWidth(1,120);
	m_Grid.SetColumnWidth(2,80);
	m_Grid.SetColumnWidth(3,200);
	m_Grid.SetColumnWidth(4,60);
	m_Grid.SetColumnWidth(5,200);

	m_Grid.SetRowHeightAll(20);
}

void CInputGenDesignVariable::OnPreInitDialog()
{
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;

	m_nTabCover = -1;
	m_nTabCondition = -1;
	m_nTabEnv = -1;
	m_nTabExposureRating	= -1;		// 노출등급
	m_nTabDesignRating		= -1;		// 설계등급
	m_nTabRebarSettle		= -1;		// 철근 정착장
	m_nTabRebarJoint		= -1;		// 철근 이음장a
	m_nTabLsdSupportRebar = -1;

	long nTab = 0;
	m_sArrTabName.RemoveAll();

	if(m_bDesignLsd)
	{
		m_sArrTabName.Add(_T("설계조건"));			m_nTabCondition			 = nTab++;
		m_sArrTabName.Add(_T("피복설정"));			m_nTabCover				 = nTab++;
		m_sArrTabName.Add(_T("노출등급"));			m_nTabExposureRating	 = nTab++;
		m_sArrTabName.Add(_T("설계등급"));			m_nTabDesignRating		 = nTab++;
		m_sArrTabName.Add(_T("설계선택사항"));		m_nTabEnv				 = nTab++;
		m_sArrTabName.Add(_T("철근 정착, 이음 보정 계수"));	m_nTabRebarFactor= nTab++;
		m_sArrTabName.Add(_T("철근 정착장"));		m_nTabRebarSettle		 = nTab++;
		m_sArrTabName.Add(_T("철근 이음장"));		m_nTabRebarJoint		 = nTab++;
		m_sArrTabName.Add(_T("수평철근량 검토"));	m_nTabLsdSupportRebar	 = nTab++;
	}
	else
	{
		m_sArrTabName.Add(_T("설계조건"));			m_nTabCondition	 = nTab++;
		m_sArrTabName.Add(_T("설계선택사항"));		m_nTabEnv		 = nTab++;
		m_sArrTabName.Add(_T("피복설정"));			m_nTabCover		 = nTab++;
	}
	
	InitTabCtrl();
	InitComboCtrl();

	SetResize(IDC_TAB,				    SZ_TOP_LEFT,	SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID,			        SZ_TOP_LEFT,	SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_APPLY_MIRROR2, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_APPLY_SAMEALL, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_BUTTON_BASEDATA_SAVE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_BASEDATA_LOAD, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_EXAMINE,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	
	SetResize(IDC_CHECK_QUICK_SEARCH,	SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	

	SetHyperHelp(m_pIn->GetManualPath());

	m_Grid.InitGrid(m_pIn->m_pARcBridgeDataStd->m_Fy);
	m_EarhtquakeDlg.SetKDS(pStd->m_nTypeEarthQuakeApply==1);

	SetGridData();

//#ifdef _DEBUG
	m_checkQuickSearch.ShowWindow(SW_SHOW);
//#else
//	m_checkQuickSearch.ShowWindow(SW_HIDE);
//#endif
}

void CInputGenDesignVariable::InitTabCtrl()
{
//	m_Tab.RemoveAllTabs();
	m_Tab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_Tab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	long i = 0; for(i = 0; i < m_sArrTabName.GetSize(); i++)
		m_Tab.InsertItem(i, m_sArrTabName[i]);

	int nTab = m_Tab.GetCurSel();
	m_Tab.SetCurSel(nTab);
}


void CInputGenDesignVariable::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	CInputDialogBase::OnTabSelchange(pNMHDR, pResult);
	SetDataSave();
	return;
}

void CInputGenDesignVariable::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	int nTab = m_Tab.GetCurSel();
	
	SetGridData();
	SetDataInit();
	DrawInputDomyunView(TRUE);
	
	if(nTab == m_nTabEnv)
		m_pIn->SetComment("'*' 항목은 선택한 설계기준에 의해 권고안", FALSE);

	return;
}

BOOL CInputGenDesignVariable::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		UpdateData();
		pMsg->wParam = VK_TAB;
	}


	return CInputDialogBase::PreTranslateMessage(pMsg);
}

// 콘보 컨트롤들 생성
void CInputGenDesignVariable::InitComboCtrl()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	CString str = _T("");
	long nSize = pStd->GetBridgeSize();
	long i = 0; for(i = 0; i < nSize; i++)
	{
		CRcBridgeApp *pBri = pStd->GetBridge(i);
		str = pStd->GetStringBridgeOrder(i) + _T(" : ") + pBri->m_strBridgeName
			+ _T(" [") + pBri->GetStringBridgeType() + _T("]");
		m_cbBridgeNo.AddString(str);
	}
	m_cbBridgeNo.SetCurSel(m_pIn->m_nCurrentBridge);
}

void CInputGenDesignVariable::OnSelchangeComboBridgeNo() 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	m_pIn->m_nCurrentBridge = m_cbBridgeNo.GetCurSel();
	SetGridData();
	SetDataInit();
	SetPaneTitle();
	m_cbBridgeNo.SetFocus();
}

void CInputGenDesignVariable::DrawInputDomyunView(BOOL bZoomAll)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(m_pIn->GetScaleInputDialog());
	CDomyun Dom(pDom);

	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, m_pIn->m_pARcBridgeDataStd);

#ifdef _DEBUG
//////////////////////////////////////////////////////////////////////////
//	날개벽 Mesh 테스트
	CTwinVectorArray tvArrWing, tvArrFix, tvArrInit, tvArrSK;
	CDoubleArray dArr;
	CDPoint xyWingStt(0,0),xyWingEnd(0,0),xyWingFootStt(0,0),xyWingFootEnd(0,0);
	double dDistThick = 0;
	for(long ix = 0; ix < 2; ix++)
	{
		pBri->GetTvArrayWingWallInitialMesh(tvArrWing, tvArrFix, tvArrSK, xyWingStt, xyWingEnd, xyWingFootStt, xyWingFootEnd, dDistThick, ix, TRUE, TRUE);
		m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, ix == 0 ? HCAD_DIML : HCAD_CONC);
		DBStd.DrawTvArray(&Dom, tvArrWing);
	}
	m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DBStd.DrawTvArray(&Dom, tvArrFix);
//////////////////////////////////////////////////////////////////////////
#endif

	

	if(m_Tab.GetCurSel() != m_nTabCover)
	{

			CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, m_pIn->m_pARcBridgeDataStd);

#ifdef _DEBUG
		/* 날개벽 영역 나누기 테스트
		pBri->m_nTypeJongdanForRebar	= 2;
		CTwinVectorArray tvArr;
		BOOL bStt	= FALSE;
		BOOL bLeft	= TRUE;

		int se = bStt ? -1 : 1;
		int nLeft = bLeft ? -se : se;	// 시점일 경우 교량 진행 방향의 좌우와 날개벽의 좌우가 반대임.
		pBri->SyncBridge(nLeft, TRUE);
		pBri->GetTvWingWallFront(bStt, bLeft, 1, tvArr, TRUE, FALSE, TRUE, TRUE);
		CHgBaseDrawStd baseDraw;
		//baseDraw.DrawTvArray(&Dom, tvArr, TRUE, TRUE);

		CTwinVectorArray tvArrResult;
		CDoubleArray dArrAngleSu;
		CRebarPlacing rb;
		rb.SeperatorTwinVectorArea(tvArr, tvArrResult, dArrAngleSu);

		long n	= 0;
		long i = 0; for(i = 0; i < dArrAngleSu.GetSize(); i++)
		{
			CTwinVectorArray tvArrTmp;
			for(long j = 0; j < dArrAngleSu.GetAt(i); j++)
			{
				tvArrTmp.Add(tvArrResult.GetAt(n++));
			}
				
			baseDraw.DrawTvArray(&Dom, tvArrTmp);
		}
		*/
/*
		// 평면도(배근) 테스트 //////////////
 		pBri->SetSearchIP();
 		if(!pBri->m_bCaledRebarMain_Plane || !pBri->m_bCaledRebarSupport_Plane)
 			pBri->SyncBridgeRebar(FALSE);
 
 		DBStd.m_pDrawPlane->DrawRebar_Plane_UpperSlabStdForTest(&Dom, -1, TRUE, TRUE, TRUE, TRUE, FALSE, 0);
 			
		//DBStd.m_pDrawPlane->DrawRebar_Plane_UpperSlabStd(&Dom, -1, TRUE, FALSE, FALSE, FALSE, FALSE);
*/			
			
		// 평면도(배근) 배력근 테스트
		/*
		CTwinVectorArray tvArrPlane;
		m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
		pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1, TRUE);
		CHgBaseDrawStd baseDraw;
		baseDraw.DrawTvArray(&Dom, tvArrPlane);
		DBStd.m_pDrawPlane->DrawRebarSupport_Plane_UpperSlabGirderAndHunch(&Dom, 0, FALSE, 0, FALSE, FALSE, FALSE, 0);
		DBStd.m_pDrawPlane->DrawRebarSupport_Plane_UpperSlabGirderAndHunch(&Dom, 1, FALSE, 0, FALSE, FALSE, FALSE, 0);
		*/
	
		/*
		CTwinVectorArray tvArrPlaneExp;
		pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlaneExp, 0, TRUE, 2, TRUE, TRUE);
		CRebarPlacing rb;
//			rb.GetOffsetTvArray(tvArrPlaneExp, 100, TRUE);
		CHgBaseDrawStd baseDraw;
		baseDraw.DrawTvArray(&Dom, tvArrPlaneExp, m_pIn->IsViewVectorInfo());
			

		pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlaneExp, -1, TRUE);
		m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
		baseDraw.DrawTvArray(&Dom, tvArrPlaneExp, m_pIn->IsViewVectorInfo(), m_pIn->IsViewVectorInfo());
		*/
			
							
		// 평면도(배근) 테스트 //////////////

		// 아치리브(배근) 테스트/////
	//	DBStd.m_pDrawPlane->DrawRebar_Plane_ArchRibStd(&Dom, -1, TRUE, FALSE, FALSE, FALSE);
		//////////////////////////////

		// 평면도(일반) 테스트 ///////////////
/*
		CHgBaseDrawStd baseDraw;
		CTwinVectorArray tvArrPlane;
		m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
		pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, 0, TRUE, 2);
		baseDraw.DrawTvArray(&Dom, tvArrPlane, DBStd.IsViewVectorInfo(), DBStd.IsViewVectorInfo());

		m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
		pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1, TRUE, 2);
		baseDraw.DrawTvArray(&Dom, tvArrPlane, DBStd.IsViewVectorInfo(), DBStd.IsViewVectorInfo());
		*/
		/*

  		// 날개벽 평면
		if(pBri->IsOutWall(TRUE))
		{
			pBri->GetTvWingWallPlane(TRUE);
			DBStd.m_pDrawWingWall->DrawGenPlane(&Dom, TRUE, TRUE);
			DBStd.m_pDrawWingWall->DimGenAngle(&Dom, TRUE, FALSE);
		}

		if(pBri->IsOutWall(FALSE))
		{
			pBri->GetTvWingWallPlane(FALSE);
			DBStd.m_pDrawWingWall->DrawGenPlane(&Dom, FALSE, TRUE);
			DBStd.m_pDrawWingWall->DimGenAngle(&Dom, FALSE, FALSE);
		}
		*/
		// 평면도(일반) 테스트 ///////////////

		// 횡단면도(배근) 테스트 //////////////
// 			double dSta	= pBri->GetStationSmallestWidthOfUpperSlab();
// 			BOOL bVertDir	= pBri->IsVertDir();
// 			pBri->SyncBridge(0, !bVertDir);
// 			pBri->SyncBridgeRebar(FALSE);
// 			
// 			double dStaLeft = pBri->GetStationOnJijum(0, 0);
// 			double dStaRight = (pBri->GetStationOnJijum(0, 0)+pBri->GetStationOnJijum(1, 0))/2;
// 			dStaLeft	= pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, FALSE);
// 			DBStd.m_pDrawCrossSection->DrawRebar_CrossSectionStd(&Dom, dStaLeft, dStaLeft, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE);

		//DBStd.m_pDrawCrossSection->DrawRebarSupport_CrossSection_UpperSlab(&Dom, dStaLeft, FALSE, 0, FALSE, FALSE, 0, FALSE, TRUE);
		/*
		BOOL bVertDir	= pBri->IsVertDir();
		pBri->SyncBridge(0, !bVertDir);
		pBri->SyncBridgeRebar(FALSE);
		double dSta	= pBri->GetStationOnJijum(0, 0, bVertDir) + pBri->GetLengthJigan(0, 0, bVertDir)/2;
		DBStd.m_pDrawCrossSection->DrawRebar_CrossSectionStd(&Dom, dSta, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE);
		*/


		// 횡단면도(배근) 테스트 //////////////

		// 기초평면도(배근) 테스트 ///////////
		/*
		BOOL bVertDir	= TRUE;
		pBri->SyncBridge(0, !bVertDir);
		pBri->SyncBridgeRebar(FALSE, -1);
		long nJ	= 0;
		CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(0);
		if(pFooting)
		{
			DBStd.m_pDrawFooting->DrawRebar_PlaneStd(&Dom, pFooting, TRUE, TRUE, TRUE, TRUE, 0);//nJ==0?1:-1);
		}
		*/
		// 기초평면도(배근) 테스트 ///////////


		// 벽체 //
/*
		BOOL bVertDir	= pBri->IsVertDir();
		pBri->SyncBridge(0, !bVertDir);
		pBri->SyncBridgeRebar(FALSE, 0, TRUE);

		DBStd.m_pDrawCrossSection->DrawRebar_OutWall_CrossSectionStd(&Dom, FALSE, TRUE, FALSE, TRUE, TRUE);
*/
		// 벽체 //

		// 중간벽체 ///
		/*
		BOOL bVertDir	= pBri->IsVertDir();
		pBri->SyncBridge(0, !bVertDir, FALSE);
		pBri->SyncBridgeRebar(FALSE, bVertDir ? -1 : 1);
		CWallApp *pWall	= pBri->GetInWall(0);
		if(pWall)
		{
			DBStd.m_pDrawCrossSection->DrawRebar_InWall_SideStd(&Dom, 1, FALSE, FALSE, TRUE, TRUE, "좌", "우", TRUE);
		}
		*/
		//	
#endif
		
	}
	// 피복입력시 피복입력삽도용으로 만든 교량을 그림.
	else
	{
		CRcBridgeRebar *pBri	= GetRcBridgeRebarForCover();
		CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, m_pIn->m_pARcBridgeDataStd);
		CCellID cell	= m_Grid.GetFocusCell();
		CString sArea	= m_Grid.GetTextMatrix(cell.row, 0);

		if(sArea.Find("날개벽", 0) != -1)
		{
			BOOL bStt(TRUE), bLeft(TRUE);
			CWingWall* pWing = NULL;//pBri->GetWingWall(bStt, bLeft);
			for(long stt = 0; stt < 2; stt++)
			{
				for(long left = 0; left < 2; left++)
				{
					CWingWall *pWingTmp	= pBri->GetWingWall(stt==iSTT, left==iLEFT);
					if(!pWingTmp) continue;
					if(pWingTmp->m_bExist)
					{
						bStt	= stt == iSTT;
						bLeft	= left == iLEFT;
						pWing	= pWingTmp;
						break;
					}
				}
				if(pWing) break;
			}

			if(!pWing) return;

			long nOut	= 1;
			CDRect rcFront(0, 0, 0, 0);
			int se = bStt ? -1 : 1;
			int nLeft = bLeft ? -1*se : 1*se;	// 시점일 경우 교량 진행 방향의 좌우와 날개벽의 좌우가 반대임.
			CDPoint vAng = pBri->GetAngleWingWallBetweenWall(bStt, bLeft);
			double dT1 = pWing->m_dT1;
			double dT2 = pWing->m_dT2;
			double dSpareT = nOut==0 ? max(dT1, dT2)/vAng.y*vAng.x : 0.0;
			double dSpare1 = 1000.0;
			double dSpare2 = 2000.0;
			double lr = bLeft ? -1.0 : 1.0;
			//pBri->SyncBridge(nLeft, TRUE);
			pBri->GetTvVertSection_Std(nLeft, TRUE, FALSE);

			pBri->GetTvWingWallPlane(bStt);

			// 정면도
			CDomyun DomFront(pDom);

			//.. 일반도
			m_pIn->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_CONC);
			DBStd.m_pDrawWingWall->DrawGenFront(&DomFront, bStt, bLeft, nOut);
			DBStd.m_pDrawWingWall->DrawGenCutBriSect(&DomFront, bStt, bLeft, nOut);
			//DBStd.m_pDrawWingWall->DrawBlockBoundary(&DomFront, bStt, bLeft, nOut);

			//.. 구조도
			m_pIn->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_BARC);

			DBStd.m_pDrawWingWall->DrawRebarHunchFootingFront(&DomFront, bStt, bLeft, nOut);
			DBStd.m_pDrawWingWall->DrawRebarHunchWallFront(&DomFront, bStt, bLeft, nOut);

			m_pIn->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_DIML);
			rcFront = DomFront.GetExtRect();

			m_pIn->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_SMALL_TITLE);
			DomFront.SetTextAlignHorz(TA_CENTER);
			DomFront.SetTextHeight(7.5);
			DomFront.TextOut(rcFront.CenterPoint().x, rcFront.bottom+dSpare1, _T("정 면 도"));

			DomFront.Move(dSpareT*se, 0);

			*pDom << DomFront;

			// 단면도
			CDomyun DomSect(pDom);

			CTwinVectorArray tvArrHu, tvArrGen;
			pBri->GetTvArrayRebarWingWallHunchFootingSect(tvArrHu, bStt, bLeft);
			CDPoint xyHu1 = tvArrHu.GetAt(0).m_v1;
			CDPoint xyHu2 = tvArrHu.GetAt(2).m_v2;
			double dHuW = pWing->m_dHuW1;
			double dCoverWingOut = pBri->m_dCoverWingOut[0];
			double dSpare = fabs(xyHu1.x-xyHu2.x) + dCoverWingOut*2 - (dT2+dHuW);

			m_pIn->m_pARoadOptionStd->SetEnvType(&DomSect, HCAD_CONC);
			DBStd.m_pDrawWingWall->DrawGenSection(&DomSect, bStt, bLeft, dSpare);

			m_pIn->m_pARoadOptionStd->SetEnvType(&DomSect, HCAD_BARC);
			DBStd.m_pDrawWingWall->DrawRebarSection(&DomSect, bStt, bLeft);

			if(sArea != "날개벽 끝단")
			{
				m_pIn->m_pARoadOptionStd->SetEnvType(&DomSect, HCAD_DIML);
				DBStd.m_pDrawWingWall->DimForCoverInput(&DomSect, sArea, bStt, bLeft);
			}

			CDRect rcSect = DomSect.GetExtRect();

			m_pIn->m_pARoadOptionStd->SetEnvType(&DomSect, HCAD_SMALL_TITLE);
			DomSect.SetTextAlignHorz(TA_CENTER);
			DomSect.SetTextHeight(7.5);
			DomSect.TextOut(rcSect.CenterPoint().x, rcSect.bottom+dSpare1, _T("단 면 도"));

			if(se*lr>0)
				DomSect.Move((rcFront.Width()+rcSect.Width()-dSpare2)*se, 0);
			else
				DomSect.Move((rcFront.Width()+dSpare2)*se, 0);

			*pDom << DomSect;

			// 평면도
			pBri->GetTvWingWallPlane(bStt, TRUE, TRUE);

			CDomyun DomPlane(pDom);

			CTwinVector tvEnd, tvOut, tvSlab;
			pBri->m_tvArrPlaneWingWall.GetTvByInfo(_T("날개벽끝단"), tvEnd);
			pBri->m_tvArrPlaneWingWall.GetTvByInfo(_T("날개벽외측"), tvOut);
			pBri->m_tvArrPlaneWingWall.GetTvByInfo(_T("슬래브끝단외측"), tvSlab);
			CDPoint vSlab = tvSlab.GetXyDir().Rotate90();
			CDPoint xyMid(0, 0), xyCut(0, 0);
			xyMid = tvEnd.GetXyMid();
			GetXyMatchSegAndLine(tvSlab.m_v1, tvSlab.m_v2, xyMid, vSlab, xyCut);
			double dCutLen = 1500;// ? ~(xyCut-tvOut.m_v1) : 1500;
			if(dCutLen<1500)
				dCutLen = 1500;

			pBri->GetTvArrayRebarWingWallHunchWallPlane(tvArrHu, bStt, bLeft, TRUE);
			xyHu1 = tvArrHu.GetAt(0).m_v1;
			xyHu2 = tvArrHu.GetAt(2).m_v2;
			CDPoint vHu1 = tvArrHu.GetAt(0).GetXyDir();
			CDPoint vHu2 = tvArrHu.GetAt(2).GetXyDir();
			GetXyMatchLineAndLine(xyHu1, vHu1, xyHu2, vHu2, xyHu1);
			dSpare = max(~(xyHu1-xyHu2)+dCoverWingOut*2, dCutLen);

			m_pIn->m_pARoadOptionStd->SetEnvType(&DomPlane, HCAD_CONC);
			DBStd.m_pDrawWingWall->DrawGenCutPlane(&DomPlane, bStt, bLeft, dSpare, TRUE);

			m_pIn->m_pARoadOptionStd->SetEnvType(&DomPlane, HCAD_BARC);
			DBStd.m_pDrawWingWall->DrawRebarPlane(&DomPlane, bStt, bLeft);

			if(sArea == "날개벽 끝단")
			{
				m_pIn->m_pARoadOptionStd->SetEnvType(&DomPlane, HCAD_DIML);
				DBStd.m_pDrawWingWall->DimForCoverInput(&DomPlane, sArea, bStt, bLeft);
			}

			DomPlane.SetCalcExtRect();
			CDRect rcPlane = DomPlane.GetExtRect();

			m_pIn->m_pARoadOptionStd->SetEnvType(&DomPlane, HCAD_SMALL_TITLE);
			DomPlane.SetTextAlignHorz(TA_CENTER);
			DomPlane.SetTextHeight(7.5);
			DomPlane.TextOut(rcPlane.CenterPoint().x, rcPlane.bottom+dSpare1, _T("평 면 도"));

			CDPoint xyMove = pBri->GetXySectionWingWallStt(bStt, TRUE);
			xyMove.y = rcFront.bottom - rcPlane.top + dSpare2;
			DomPlane.Move(xyMove);

			pBri->GetTvVertSection_Std(nLeft, TRUE, FALSE, TRUE);
			*pDom << DomPlane;
		}
		else
		{
			BOOL bVertDir	= pBri->IsVertDir();
			//pBri->SyncBridge(0, !bVertDir);
			pBri->GetTvVertSection_Std(0, !bVertDir, FALSE);
			pBri->SyncBridgeRebar(TRUE, bVertDir?-1:1);

			if(sArea == "슬래브 측면")
			{
				CHgBaseDrawStd baseDraw;
				long nJ	= 0;
				double dSta	= pBri->GetStationOnJijum(nJ);
				
				CTwinVectorArray tvArrCross;
				m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
				pBri->GetTvCrossSection(tvArrCross, dSta, CDPoint(0, 1), FALSE, TRUE, FALSE, 0, TRUE);
				baseDraw.DrawTvArray(&Dom, tvArrCross);
				
				DBStd.m_pDrawCrossSection->DrawRebarSupport_CrossSection_UpperSlab(&Dom, dSta, TRUE, 0, FALSE, FALSE, 0);
				DBStd.m_pDrawCrossSection->DrawRebarSupport_CrossSection_UpperSlab(&Dom, dSta, FALSE, 0, FALSE, FALSE, 0);

				m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
				DBStd.m_pDrawCrossSection->DimForCoverInput(&Dom, sArea, nJ);	
			}
			else
			{
				DBStd.m_pDrawVertSection->DrawVertSectionAll(&Dom, FALSE, FALSE, FALSE, FALSE, TRUE);
				DBStd.m_pDrawVertSection->DrawRebarMain_VertSectionAll(&Dom, 0, FALSE, FALSE, FALSE, 0, TRUE, FALSE);

				m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
				DBStd.m_pDrawVertSection->DimForCoverInput(&Dom, sArea);	
			}
			pBri->GetTvVertSection_Std(0, !bVertDir, FALSE, TRUE);
		}
	}

	*pDom << Dom;
	
	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

void CInputGenDesignVariable::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	structGridCell *pCell = m_Grid.GetCurrentCell();
	if(pCell && m_Grid.GetFocus())
	{
		m_pIn->SetComment(pCell->sTitle);
		CString str	= m_Grid.GetItemText(pCell->nRow, 0);
		m_pView->GetDomyun()->STMakeCursor(str);
	}

	if(m_Tab.GetCurSel() == m_nTabCover)
	{
		DrawInputDomyunView(TRUE);
	}
}



// 피복설정
void CInputGenDesignVariable::SetGridDataCover()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 1, 1);

	long row = 0;
	long col = 0;
	m_Grid.AddCell(row, col,	  "철근 위치");				m_Grid.AddCell(row++, col+1,	  "설정값");
	m_Grid.AddCell(row, col,	  "슬래브 상면");				m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverUpper_UpperSlab[0]);
	m_Grid.AddCell(row, col,	  "슬래브 하면");				m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverLower_UpperSlab[0]);
	m_Grid.AddCell(row, col,	  "슬래브 측면");				m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverSide_UpperSlab[0]);
	if(pBri->IsBoxType())
	{
		m_Grid.AddCell(row, col,	  "하부슬래브 상면");		m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverUpper_LowerSlab[0]);
		m_Grid.AddCell(row, col,	  "하부슬래브 하면");		m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverLower_LowerSlab[0]);
	}

	// 벽체가 없어도 슬래브 시종점면에 대한 피복 적용은 벽체 외면 피복으로 한다.
	if(pBri->IsOutWall(TRUE) || pBri->IsOutWall(FALSE))
	{
		m_Grid.AddCell(row, col,	  "벽체 외면");			m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverWallOut[0]);
	}
	else
	{
		m_Grid.AddCell(row, col,	  "슬래브 측면(시종점)");		m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverWallOut[0]);
	}

	if(pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB)
	{
		m_Grid.AddCell(row, col,	  "벽체 내면");			m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverWallIn[0]);
		m_Grid.AddCell(row, col,	  "내측 벽체");			m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverWallMid[0]);
		m_Grid.AddCell(row, col,	  "기둥");				m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverColumn[0]);
		m_Grid.AddCell(row, col,	  "날개벽 외면");			m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverWingOut[0]);
		m_Grid.AddCell(row, col,	  "날개벽 내면");			m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverWingIn[0]);
		m_Grid.AddCell(row, col,	  "날개벽 상면");			m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverWingUpper);
		m_Grid.AddCell(row, col,	  "날개벽 끝단");			m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverWingEnd);
		if(pBri->IsBoxType() == FALSE)
		{
			m_Grid.AddCell(row, col,	  "기초 상면");				m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverUpper_Footing[0]);
			m_Grid.AddCell(row, col,	  "기초 측면");				m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverSide_Footing[0]);
			m_Grid.AddCell(row, col,	  "기초 하면(직접기초)");	m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverLower_Footing_Lean[0]);
			m_Grid.AddCell(row, col,	  "기초 하면(말뚝기초)");	m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverLower_Footing_Pile[0]);
		}
	}

	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		m_Grid.AddCell(row, col,	  "RIB 상면");			m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverRib[iUPPER][0]);
		m_Grid.AddCell(row, col,	  "RIB 하면");			m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverRib[iLOWER][0]);
	}

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 150);
	m_Grid.SetColumnWidth(1, 80);
}

void CInputGenDesignVariable::OnCellChangedData(NMHDR *pNotifyStruct, LRESULT *result)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	int nTab = m_Tab.GetCurSel();

	CCellID cell = m_Grid.GetFocusCell();
	CString str  = m_Grid.GetTextMatrix(cell.row, 0);
	CString str2 = m_Grid.GetTextMatrix(cell.row, 1);
	CString str5 = m_Grid.GetTextMatrix(cell.row, 4);

	BOOL bReDrawGrid(FALSE);

	if(str2.Find("강역구간 적용")!=-1)
	{
		if(pBri->IsBoxType())
		{
			m_Grid.UpdateData(TRUE);
			long nSelectRigid = pBri->m_nSelectRigidModeling;
			if(nSelectRigid == 1)
			{
				pBri->m_nSelectRigidModeling = 2;
				AfxMessageBox(_T("Box형 라멘교에서는 무한강성을 적용할 수 없습니다.\nEndOffet을 적용합니다."));
				SetGridData();
				SetDataInit();
				return;
			}

			if(pBri->m_nSelectRigidModeling == 0)
			{
				// 강역적용안함을 선택하면 벽체부재력 위치를 입력못하도록 바뀌어야 되니까 SetDataInit을 다시한번 해준다.
				SetGridData();
				SetDataInit();
			}
		}
	}

	SetDataSave();
	if(str2.Find("주철근 배치")!=-1)
	{
		CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
		if(!pBri) return;

		if(pBri->GetCountCycleMainRebar() > 3)
		{
			pBri->SyncCycleMainRebar(0, 2);
			pBri->SyncCycleMainRebar(1, 3);
		}

		// 트리 갱신
		AfxGetMainWnd()->SendMessage(WM_USER+8, -1);
	}

	if(str2.Find("지반변형계수", 0) != -1)
	{
		SetGridData();
		SetDataInit();
	}

	if(str2=="  3D Plate 해석 툴")
	{
		m_pIn->SetDialogEnable();
		bReDrawGrid = TRUE;
	}

	if(str2=="  2D Frame 해석 툴")
	{
		// 해석툴 선택이 바뀌었다면 데이터 초기화
		pBri->m_FemNormal.RemoveAllData();
		long i=0; for(i=0; i<3; i++)
		{
			pBri->m_FemEQ[i].RemoveAllData();
		}
		if(pBri->m_nSelect2DFrameTool == SOLVE_MIDAS)
		{
			pBri->m_bSelectEnvSubsid = TRUE;

			// (40867) 선택된 해석툴이 Midas이고 지반모델링이 NLLINK라면 Spring으로 바꿔준다.
			pBri->m_n2DGeneralSpringType = 1;
			pBri->m_n2DEQSpringType = 1;
		}
		bReDrawGrid = TRUE;
	}

	if(str2=="  내진해석시 모델링 방법")
	{
		SetGridData();
		SetDataInit();
	}

	// [v]에 대한 선택시
	if(str5=="검토" || str5=="검토안함" || str5=="적용" || str5=="적용안함" || bReDrawGrid)
	{
		SetGridData();
 		SetDataInit();
	}
	
	if (m_bDesignLsd && nTab == m_nTabCondition)
	{
		if (str == "재료 물성치 상세 입력")
		{
			SyncMaterial();
		}

		if((pBri->GetMaterialDetail() && cell.row > 1 && cell.row <= ePartCount+1) || (pBri->GetMaterialDetail() == FALSE && (cell.row == 2 || cell.row == 3)))
		{
			double uWC = ( m_pIn->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightConcrete>0) ?  m_pIn->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightConcrete/1000 : 2.350;

			long nPos = cell.row - 2;
			if(pBri->GetMaterialDetail() == FALSE && cell.row == 3)
				nPos = ePartApsStt;
			pBri->m_Ec[nPos] = GetValueEcConc(pBri->m_Fck[nPos], uWC, m_pIn->m_pARcBridgeDataStd->IsSI(), m_pIn->m_pARcBridgeDataStd->m_nTypeKciCode); // 도로교설계기준 38페이지;
		}

		
		if (pBri->GetMaterialDetail() == FALSE)
		{
			if (str2 == "  본체/상부슬래브" || str2 == "  " + pBri->GetStringPart(ePartApsStt))
			{
				SyncMaterial();
			}
		}

		// 폭과 수치 입력사항에 대하여 소수점 4째 자리까지 자동 계산되어야 하므로 다시 갱신한다.
		SetGridData();
		SetDataInit();
	}

	if (nTab == m_nTabEnv)
	{
		HGINT32 iScrollPosHor = m_Grid.GetScrollPos(SB_HORZ);
		HGINT32 iScrollPosVert = m_Grid.GetScrollPos(SB_VERT);
		m_Grid.LockWindowUpdate();
		SetGridData();
		SetDataInit();
		m_Grid.UnlockWindowUpdate();
		m_Grid.SetScrollPos(SB_HORZ, iScrollPosHor);
		m_Grid.SetScrollPos(SB_VERT, iScrollPosVert);
	}

	if(m_bDesignLsd && (nTab >= m_nTabExposureRating && nTab <= m_nTabRebarJoint))
	{
		m_Grid.UpdateData(FALSE);
		SetGridData();
		SetDataInit();
	}
}

// 피복 입력용 교량리턴
CRcBridgeRebar* CInputGenDesignVariable::GetRcBridgeRebarForCover()
{
	/*
	if(m_pBriForCover) 
	{
		delete m_pBriForCover;
		m_pBriForCover	= NULL;
	}
	if(!m_pBriForCover) MakeRcBridgeForCover();
	*/

	return m_pIn->GetRcBridgeRebar();
}

BOOL CInputGenDesignVariable::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CInputDialogBase::DestroyWindow();
}

// 커버 각 부위별로 적용
void CInputGenDesignVariable::SyncCover(CRcBridgeRebar *pBri)
{
	if(!pBri) return;

	long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		// 내측 벽체
		long nCountInwall	 = pBri->GetCountInWall();
		long wall = 0; for(wall = 0; wall < nCountInwall; wall++)
		{
			CWallApp *pWall	= pBri->GetInWall(wall);
			if(pWall)
			{
				pWall->m_dCoverLeft[dan]	= pBri->m_dCoverWallIn[dan];
				pWall->m_dCoverRight[dan]	= pBri->m_dCoverWallIn[dan];
				pWall->m_dCoverFrontBack[dan]	= pBri->m_dCoverWallIn[dan];
			}
		}

		// 기초
		long nCountFooting	= pBri->GetCountFooting();
		long footing = 0; for(footing = 0; footing < nCountFooting; footing++)
		{
			CFootingApp *pFooting	= pBri->GetFooting(footing);
			if(pFooting)
			{
				pFooting->m_dCoverUpper[dan]	= pBri->m_dCoverUpper_Footing[dan];
				pFooting->m_dCoverSide[dan]	= pBri->m_dCoverSide_Footing[dan];
				if(pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_PILE)
					pFooting->m_dCoverLower[dan]	= pBri->m_dCoverLower_Footing_Pile[dan];
				else
					pFooting->m_dCoverLower[dan]	= pBri->m_dCoverLower_Footing_Lean[dan];
			}
		}
	}
}

// 빠른 검색 적용할지?
void CInputGenDesignVariable::OnCheckQuickSearch() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	pBri->m_bNeedSettingVertSection	= TRUE;
	

	pBri->m_bQuickSearchLine	= m_checkQuickSearch.GetCheck();
	pBri->SetSearchIP();
}

void CInputGenDesignVariable::OnButtonApplyMirror() 
{
	// TODO: Add your control notification handler code here
	// 입력사항 추가시 항목을 추가해주어야함....
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(pBri==NULL || pBri->m_nSeparBri==0) return;

	CRcBridgeRebar* pBriMatch = pStd->GetBridgeMatchRebar(pBri);
	if(pBriMatch==NULL || pBriMatch->m_nSeparBri==0) return;

	CString strMsg = _T("");
	strMsg.Format("모든 탭의 입력사항이 %s과(와) 동일하게 설정됩니다.\n계속하시겠습니까 ?",pBriMatch->m_strBridgeName);
	if(AfxMessageBox(strMsg,MB_YESNO)==IDYES)
	{
		MakeSameCondition(pBri,pBriMatch);
		MakeSameEnv(pBri,pBriMatch);
		MakeSameCover(pBri,pBriMatch);
		if(m_bDesignLsd)
		{
			MakeSameLsdSupportRebar(pBri,pBriMatch);//수평철근
		}
		//////////////////////////////////////////////////////////////////////////
		SetDataInit();
	}	
}

void CInputGenDesignVariable::MakeSameCover(CRcBridgeApp* pBri1, CRcBridgeApp* pBri2)
{
	if(pBri1==NULL || pBri2==NULL) return;
	// 피복설정 -------------------------------------------------------
	pBri1->m_dCoverUpper_UpperSlab[0]	= pBri2->m_dCoverUpper_UpperSlab[0];
	pBri1->m_dCoverLower_UpperSlab[0]	= pBri2->m_dCoverLower_UpperSlab[0];
	pBri1->m_dCoverSide_UpperSlab[0]	= pBri2->m_dCoverSide_UpperSlab[0];
	pBri1->m_dCoverUpper_LowerSlab[0]	= pBri2->m_dCoverUpper_LowerSlab[0];
	pBri1->m_dCoverLower_LowerSlab[0]	= pBri2->m_dCoverLower_LowerSlab[0];
	pBri1->m_dCoverWallOut[0]			= pBri2->m_dCoverWallOut[0];
	pBri1->m_dCoverWallIn[0]			= pBri2->m_dCoverWallIn[0];
	pBri1->m_dCoverWallMid[0]			= pBri2->m_dCoverWallMid[0];
	pBri1->m_dCoverColumn[0]			= pBri2->m_dCoverColumn[0];
	pBri1->m_dCoverWingOut[0]			= pBri2->m_dCoverWingOut[0];
	pBri1->m_dCoverWingIn[0]			= pBri2->m_dCoverWingIn[0];
	pBri1->m_dCoverWingUpper			= pBri2->m_dCoverWingUpper;
	pBri1->m_dCoverWingEnd				= pBri2->m_dCoverWingEnd;
	pBri1->m_dCoverUpper_Footing[0]		= pBri2->m_dCoverUpper_Footing[0];
	pBri1->m_dCoverSide_Footing[0]		= pBri2->m_dCoverSide_Footing[0];
	pBri1->m_dCoverLower_Footing_Lean[0]	= pBri2->m_dCoverLower_Footing_Lean[0];
	pBri1->m_dCoverLower_Footing_Pile[0]	= pBri2->m_dCoverLower_Footing_Pile[0];
	pBri1->m_dCoverRib[iUPPER][0]		= pBri2->m_dCoverRib[iUPPER][0];
	pBri1->m_dCoverRib[iLOWER][0]		= pBri2->m_dCoverRib[iLOWER][0];
}

void CInputGenDesignVariable::MakeSameEnv(CRcBridgeRebar* pBri1, CRcBridgeRebar* pBri2)
{
	if(pBri1==NULL || pBri2==NULL) return;

	// 설계선택사항 -------------------------------------------------------
	
	//단면검토철근
	if(m_bDesignLsd)
	{
		pBri1->m_nApplyCalcShearDistFrontPile	= pBri2->m_nApplyCalcShearDistFrontPile;
		pBri1->m_nApplyCalcShearDistBackPile	= pBri2->m_nApplyCalcShearDistBackPile;
		pBri1->m_dThetaShearDesign				= pBri2->m_dThetaShearDesign;
		pBri1->m_bCheckDeltaTR					= pBri2->m_bCheckDeltaTR;
		pBri1->m_dElasticModularRatioUseCheck	= pBri2->m_dElasticModularRatioUseCheck;
		pBri1->m_nSelectUsebilityCheck			= pBri2->m_nSelectUsebilityCheck;
		pBri1->m_nTypeCrackWidthCondition		= pBri2->m_nTypeCrackWidthCondition;
		pBri1->m_nSelectExposureCoverCheck		= pBri2->m_nSelectExposureCoverCheck;
		pBri1->m_nSelectPminColumn				= pBri2->m_nSelectPminColumn;
	}
	else
	{
		pBri1->m_bCheckMainSlabAndFootTemper	= pBri2->m_bCheckMainSlabAndFootTemper;
		pBri1->m_bCheckRebarRateFooting			= pBri2->m_bCheckRebarRateFooting;
		pBri1->m_nSelectApplyFs					= pBri2->m_nSelectApplyFs;
		pBri1->m_bCheckSuppoerBarSlabMain		= pBri2->m_bCheckSuppoerBarSlabMain;
		pBri1->m_bCheckSuppoerBarSlabMainOver20	= pBri2->m_bCheckSuppoerBarSlabMainOver20;
		pBri1->m_bCheckSuppoerBarSlabArea		= pBri2->m_bCheckSuppoerBarSlabArea;
		pBri1->m_dCheckSuppoerBarSlabArea		= pBri2->m_dCheckSuppoerBarSlabArea;
		pBri1->m_bCheckSuppoerBarSlab300		= pBri2->m_bCheckSuppoerBarSlab300;
		pBri1->m_bCheckSuppoerBarWall15hs		= pBri2->m_bCheckSuppoerBarWall15hs;
		pBri1->m_dCheckSuppoerBarWallAreaForOver400	= pBri2->m_dCheckSuppoerBarWallAreaForOver400;
		pBri1->m_bCheckSuppoerBarWallArea		= pBri2->m_bCheckSuppoerBarWallArea;
		pBri1->m_dCheckSuppoerBarWallArea		= pBri2->m_dCheckSuppoerBarWallArea;
		pBri1->m_bCheckSuppoerBarWallMainOver20	= pBri2->m_bCheckSuppoerBarWallMainOver20;
		pBri1->m_bCheckSuppoerBarWall300		= pBri2->m_bCheckSuppoerBarWall300;
		pBri1->m_bSelectSupportBarWall			= pBri2->m_bSelectSupportBarWall;
		pBri1->m_bCheckSuppoerBarArchRibMinDia1	= pBri2->m_bCheckSuppoerBarArchRibMinDia1;
		pBri1->m_bCheckSuppoerBarArchRibMinDia2	= pBri2->m_bCheckSuppoerBarArchRibMinDia2;
		pBri1->m_bCheckSuppoerBarFootMainOver20	= pBri2->m_bCheckSuppoerBarFootMainOver20;
		pBri1->m_bCheckSuppoerBarFoot300		= pBri2->m_bCheckSuppoerBarFoot300;
		pBri1->m_bCheckSuppoerBarFoot15hs		= pBri2->m_bCheckSuppoerBarFoot15hs;
		pBri1->m_bCheckSuppoerBarFootArea		= pBri2->m_bCheckSuppoerBarFootArea;
		pBri1->m_dCheckSuppoerBarFootArea		= pBri2->m_dCheckSuppoerBarFootArea;
		pBri1->m_bCheckSuppoerBarWing300		= pBri2->m_bCheckSuppoerBarWing300;
		pBri1->m_bCheckSuppoerBarWing15hs		= pBri2->m_bCheckSuppoerBarWing15hs;
		pBri1->m_bCheckSuppoerBarWingArea		= pBri2->m_bCheckSuppoerBarWingArea;
		pBri1->m_dCheckSuppoerBarWingArea		= pBri2->m_dCheckSuppoerBarWingArea;

		pBri1->m_nTypeCrackDesignSlab			= pBri2->m_nTypeCrackDesignSlab;
		pBri1->m_nTypeCrackDesignWall			= pBri2->m_nTypeCrackDesignWall;
		pBri1->m_nTypeCrackDesignFoot			= pBri2->m_nTypeCrackDesignFoot;
		pBri1->m_nTypeCrackDesignWing			= pBri2->m_nTypeCrackDesignWing;
		pBri1->m_nTypeCrackDesignAps			= pBri2->m_nTypeCrackDesignAps;
		pBri1->m_bApplyFootingMinThick			= pBri2->m_bApplyFootingMinThick;
	}
	pBri1->m_nApplyCalcShearDistSlab		= pBri2->m_nApplyCalcShearDistSlab;
	pBri1->m_nApplyCalcShearDistWall		= pBri2->m_nApplyCalcShearDistWall;
	pBri1->m_bCalcShearCenter				= pBri2->m_bCalcShearCenter;
	pBri1->m_bApplyMinAvSlab				= pBri2->m_bApplyMinAvSlab;
	pBri1->m_bApplyMinAvWall				= pBri2->m_bApplyMinAvWall;
	pBri1->m_bApplyMinAvFoot				= pBri2->m_bApplyMinAvFoot;
	pBri1->m_bApplyMinAvWing				= pBri2->m_bApplyMinAvWing;
	pBri1->m_bApplyMinAvAps					= pBri2->m_bApplyMinAvAps;
	pBri1->m_bCheckMinCtcShearWing			= pBri2->m_bCheckMinCtcShearWing;
	pBri1->m_bApplyT1CalcWingWall			= pBri2->m_bApplyT1CalcWingWall;
	pBri1->m_bCheckMinCtcShearAps			= pBri2->m_bCheckMinCtcShearAps;
	pBri1->m_nSelectApsShearCheck			= pBri2->m_nSelectApsShearCheck;
	pBri1->m_nApplyCalcShearDistFront		= pBri2->m_nApplyCalcShearDistFront;
	pBri1->m_nApplyCalcShearDistBack		= pBri2->m_nApplyCalcShearDistBack;	
	pBri1->m_bSelectHunchDesignCheck		= pBri2->m_bSelectHunchDesignCheck;

	//구조계산
	if(m_bDesignLsd)
	{
		pBri1->m_nSelectApsAxleWeight			= pBri2->m_nSelectApsAxleWeight;
		pBri1->m_nSelectApsDeadLoad				= pBri2->m_nSelectApsDeadLoad;
	}
	else
	{
		pBri1->m_dFactorToap				= pBri2->m_dFactorToap;
		pBri1->m_dFactorLive				= pBri2->m_dFactorLive;
		pBri1->m_bCheckCornerInside			= pBri2->m_bCheckCornerInside;
		pBri1->m_nSelCalcCornerFt			= pBri2->m_nSelCalcCornerFt;
		pBri1->m_nSelectTempLoadSlab		= pBri2->m_nSelectTempLoadSlab;
		pBri1->m_bSelectEnvToapOne			= pBri2->m_bSelectEnvToapOne;
		pBri1->m_nArchRibSectionMomentPos	= pBri2->m_nArchRibSectionMomentPos;
		pBri1->m_nSelectJointUpperRange		= pBri2->m_nSelectJointUpperRange;
		pBri1->m_nTypeJongdanForRebar		= pBri2->m_nTypeJongdanForRebar;
		pBri1->m_nTypeWallForRebarGen		= pBri2->m_nTypeWallForRebarGen;
		pBri1->m_nIdxCountCycleMainRebar	= pBri2->m_nIdxCountCycleMainRebar;
	}
	pBri1->m_nSelFactorKhStt				= pBri2->m_nSelFactorKhStt;
	pBri1->m_nSelFactorKhEnd				= pBri2->m_nSelFactorKhEnd;
	pBri1->m_nSelectDesignRoadWidth			= pBri2->m_nSelectDesignRoadWidth;
	pBri1->m_nSelectLiveLoadSpanLength		= pBri2->m_nSelectLiveLoadSpanLength;
	pBri1->m_nSelectStartLane				= pBri2->m_nSelectStartLane;
	pBri1->m_bCheckUseri					= pBri2->m_bCheckUseri;
	pBri1->m_dCheckUseriValue				= pBri2->m_dCheckUseriValue;
	pBri1->m_bApplyFootingGForce			= pBri2->m_bApplyFootingGForce;
	pBri1->m_bApplyMassCalcSafe				= pBri2->m_bApplyMassCalcSafe;
	pBri1->m_nSelectForceCheckPos			= pBri2->m_nSelectForceCheckPos;
	pBri1->m_nSelectFootingReactionForce	= pBri2->m_nSelectFootingReactionForce;
	pBri1->m_bCheckFrontFootTrap2			= pBri2->m_bCheckFrontFootTrap2;
	pBri1->m_nSelSubsideType				= pBri2->m_nSelSubsideType;
	pBri1->m_nSelectWallSectionDesign		= pBri2->m_nSelectWallSectionDesign;
	pBri1->m_nSelectInWallSectionDesign		= pBri2->m_nSelectInWallSectionDesign;
	pBri1->m_nSelectRigidModeling			= pBri2->m_nSelectRigidModeling;
	pBri1->m_nSelectModelingHeight			= pBri2->m_nSelectModelingHeight;
	pBri1->m_bSelectTempLoadAddWall			= pBri2->m_bSelectTempLoadAddWall;
	pBri1->m_bSelectShrinkLoadAddWall		= pBri2->m_bSelectShrinkLoadAddWall;
	pBri1->m_bSelectEnvSubsid				= pBri2->m_bSelectEnvSubsid;
	pBri1->m_bSelectHunchModeling			= pBri2->m_bSelectHunchModeling;
	pBri1->m_bSelectRigidCalcHunch			= pBri2->m_bSelectRigidCalcHunch;
	pBri1->m_nSelectDesignSpanLength		= pBri2->m_nSelectDesignSpanLength;
	pBri1->m_nSelectDesignSpanApply			= pBri2->m_nSelectDesignSpanApply;
	pBri1->m_bSelectSpanLengthApplyShoe		= pBri2->m_bSelectSpanLengthApplyShoe;
	pBri1->m_nTypeStandardCTCRebarMain		= pBri2->m_nTypeStandardCTCRebarMain;
	pBri1->m_nTypePlacingSupportRebar_UpperSlab		= pBri2->m_nTypePlacingSupportRebar_UpperSlab;
	pBri1->m_nTypePlacingMainRebar_UpperSlab		= pBri2->m_nTypePlacingMainRebar_UpperSlab;
	pBri1->m_nTypeSideRebarMain_UpperSlabSabogang	= pBri2->m_nTypeSideRebarMain_UpperSlabSabogang;
	pBri1->m_nTypePlacingMainRebar_Footing	= pBri2->m_nTypePlacingMainRebar_Footing;
	pBri1->m_nTypeCurveOfWallVertRebar		= pBri2->m_nTypeCurveOfWallVertRebar;
	pBri1->m_bFixOutSideMainRebarBy1Cyle	= pBri2->m_bFixOutSideMainRebarBy1Cyle;
	pBri1->m_bRemoveShearRebarAtSlabYundan	= pBri2->m_bRemoveShearRebarAtSlabYundan;
	pBri1->m_nTypeCouplerColumnMainRebar	= pBri2->m_nTypeCouplerColumnMainRebar;
	pBri1->m_bApplySameSupportRebar			= pBri2->m_bApplySameSupportRebar;
	pBri1->m_bApplySameMainRebar			= pBri2->m_bApplySameMainRebar;

}

void CInputGenDesignVariable::MakeSameCondition(CRcBridgeApp* pBri1, CRcBridgeApp* pBri2)
{
	if(pBri1==NULL || pBri2==NULL) return;

	if(m_bDesignLsd)
	{
		pBri1->SetMaterialDetail(pBri2->GetMaterialDetail());
		const HGINT32 iEnd = pBri2->GetMaterialDetail() ? ePartCount : ePartUpperSlab + 1;
		for (HGINT32 ix = ePartUpperSlab; ix < iEnd; ++ix)
		{
			pBri1->m_Fck[ix] = pBri2->m_Fck[ix];
			pBri1->m_Fy[ix] = pBri2->m_Fy[ix];
			pBri1->m_Fvy[ix] = pBri2->m_Fvy[ix];
			pBri1->m_Ec[ix] = pBri2->m_Ec[ix];
		}

		pBri1->SetRateDryAndShrinkage(pBri2->GetRateDryAndShrinkage());
		pBri1->SetCoefficientExpansion(pBri2->GetCoefficientExpansion());
		pBri1->SetDBar_ForDowelBar(pBri2->GetDBar_ForDowelBar());
		
	}

	// 설계조건 -------------------------------------------------------
	pBri1->m_dUnitWeigthBackFill		= pBri2->m_dUnitWeigthBackFill;
	pBri1->m_dAngleFrictionBackFill		= pBri2->m_dAngleFrictionBackFill;
	pBri1->m_dLiveLoadAlways			= pBri2->m_dLiveLoadAlways;
	pBri1->m_dDeadLoadAlways			= pBri2->m_dDeadLoadAlways;
	pBri1->m_dLiveLoadFootway			= pBri2->m_dLiveLoadFootway;
	pBri1->m_dUnitWeightHeapSoil		= pBri2->m_dUnitWeightHeapSoil;	
	pBri1->m_dUnitWeightValidDepth		= pBri2->m_dUnitWeightValidDepth;
	pBri1->m_dValidDepthBaseGround		= pBri2->m_dValidDepthBaseGround;
	pBri1->m_dEo_CalcWallSpringEQ		= pBri2->m_dEo_CalcWallSpringEQ;
	pBri1->m_nTypeToapSafeCheck			= pBri2->m_nTypeToapSafeCheck;	
	pBri1->m_SafeFactorOverturning[0]	= pBri2->m_SafeFactorOverturning[0];
	pBri1->m_SafeFactorOverturning[1]	= pBri2->m_SafeFactorOverturning[1];
	pBri1->m_SafeFactorSliding[0]		= pBri2->m_SafeFactorSliding[0];
	pBri1->m_SafeFactorSliding[1]		= pBri2->m_SafeFactorSliding[1];
	pBri1->m_nSelectModelingCountEQ		= pBri2->m_nSelectModelingCountEQ;	
	pBri1->m_nSelectPileCalcSpringEQ	= pBri2->m_nSelectPileCalcSpringEQ;	
	pBri1->m_nSelect2DFrameTool			= pBri2->m_nSelect2DFrameTool;	
	pBri1->m_nSelect3DShellTool			= pBri2->m_nSelect3DShellTool;	
	pBri1->m_nSelect3DOutputValue		= pBri2->m_nSelect3DOutputValue;	
	pBri1->m_nApplyWingWallCalcValueCount		= pBri2->m_nApplyWingWallCalcValueCount;	

	if(m_bDesignLsd)
	{
		pBri1->SetTypeDesingEQ(pBri2->GetTypeDesingEQ());
		pBri1->m_dCoffKh_Sei		= pBri2->m_dCoffKh_Sei;	
		pBri1->SetDegreeEarthQuake(pBri2->GetDegreeEarthQuake());
		pBri1->SetRegionEarthQuake(pBri2->GetRegionEarthQuake());
		pBri1->SetCoefficientGround(pBri2->GetCoefficientGround());
		pBri1->SetGravity(pBri2->GetGravity());
		pBri1->SetTypeRigidFootingEQ(pBri2->GetTypeRigidFootingEQ());
		pBri1->m_dEo_CalcWallSpringEQ = pBri2->m_dEo_CalcWallSpringEQ;
		pBri1->SetApplyRigidWallEQRotate(pBri2->GetApplyRigidWallEQRotate());
		pBri1->SetTypeJointPosFootingEQ(pBri2->GetTypeJointPosFootingEQ());
		pBri1->SetTypePropertyPosFootingEQ(pBri2->GetTypePropertyPosFootingEQ());
		pBri1->SetTypeSoilPressureEQ(pBri2->GetTypeSoilPressureEQ());
		pBri1->SetTypeUpperWeightEQ(pBri2->GetTypeUpperWeightEQ());
		if(pBri1->m_nTypeBridge != BRIDGE_TYPE_BOX && pBri2->m_nTypeBridge != BRIDGE_TYPE_BOX)
		{
			pBri1->m_nSelectFootForce = pBri2->m_nSelectFootForce;	
		}
	}

	pBri1->m_dFckMassConc				= pBri2->m_dFckMassConc;
	pBri1->m_dFckLeanConc				= pBri2->m_dFckLeanConc;
}

void CInputGenDesignVariable::MakeSameLsdSupportRebar(CRcBridgeApp* pBri1, CRcBridgeApp* pBri2)
{
	//노출등급
	CLsdExposureRatingCtl *pExposureTar = &pBri1->m_plsdEnvRating->GetLsdExposureRatingCtl();
	CLsdExposureRatingCtl *pExposureSrc = &pBri2->m_plsdEnvRating->GetLsdExposureRatingCtl();

	pExposureSrc->CopyLsdExposureRating(pExposureTar);
	pExposureSrc->CopyLsdExposureMaterial(pExposureTar);

	//설계등급
	CLsdDesignRatingCtl *pDesignRatingTar = &pBri1->m_plsdEnvRating->GetLsdDesignRatingCtl();
	CLsdDesignRatingCtl *pDesignRatingSrc = &pBri2->m_plsdEnvRating->GetLsdDesignRatingCtl();

	pDesignRatingSrc->CopyLsdDesignRating(pDesignRatingTar);

	//정착장
	pBri1->m_plsdRebar->GetOption() = pBri2->m_plsdRebar->GetOption();

	//수평철근
	pBri1->m_bCheckSupportSlabDryingShrinkage = pBri2->m_bCheckSupportSlabDryingShrinkage;
	pBri1->m_bCheckSupportSlabByReqRebar = pBri2->m_bCheckSupportSlabByReqRebar;
	pBri1->m_bCheckSupportSlabMainRate = pBri2->m_bCheckSupportSlabMainRate;
	pBri1->m_bCheckSupportSlabMainRateSpan = pBri2->m_bCheckSupportSlabMainRateSpan;
	pBri1->m_bCheckSupportWallMainRate = pBri2->m_bCheckSupportWallMainRate;
	pBri1->m_bCheckSupportFootDryingShrinkage = pBri2->m_bCheckSupportFootDryingShrinkage;
	pBri1->m_bSkipAsMinCheckByHoriRebar = pBri2->m_bSkipAsMinCheckByHoriRebar;

}

void CInputGenDesignVariable::OnButtonApplySameAll() 
{
	// TODO: Add your control notification handler code here
	// 입력사항 추가시 항목을 추가해주어야함....
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(pBri==NULL) return;

	CString strMsg = _T("");
	strMsg.Format("모든 교량의 전체 입력사항이 %s과(와) 동일하게 설정됩니다.\n계속하시겠습니까 ?",pBri->m_strBridgeName);
	if(AfxMessageBox(strMsg,MB_YESNO)==IDYES)
	{
		long nQtyBri = pStd->GetBridgeSize();

		long i=0; for(i=0; i<nQtyBri; i++)
		{
			if(i==m_pIn->m_nCurrentBridge) continue;

			CRcBridgeRebar* pBri2 = pStd->GetBridgeRebar(i);

			MakeSameCondition(pBri2,pBri);
			MakeSameEnv(pBri2,pBri);
			MakeSameCover(pBri2,pBri);
			if(m_bDesignLsd)
			{
				MakeSameLsdSupportRebar(pBri2,pBri);
			}
		}
	}	
}

void CInputGenDesignVariable::KillFocusDlg()
{
	if(m_pBriForCover) delete m_pBriForCover;
}

void CInputGenDesignVariable::OnButtonBasedataSave() 
{
	// TODO: Add your control notification handler code here
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);

	CString sTitle	= _T("설계환경 저장");
	CFileDialog dlg(FALSE, "*.bar",NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
								  "ARcBridge 설계환경 파일 (*.bar)|*.bar||",GetParent());
	dlg.m_ofn.lpstrTitle = sTitle;
	if (dlg.DoModal() == IDOK)
	{
		SetDataSave();
		CFile f;
		if(f.Open(dlg.GetPathName(),CFile::modeCreate|CFile::modeWrite)==FALSE) return;
		CArchive ar(&f,CArchive::store);

		pBri->SerializeEnv(ar);
		ar.Close();
		f.Close();		
	}			
}

void CInputGenDesignVariable::OnButtonBasedataLoad() 
{
	// TODO: Add your control notification handler code here
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);

//	주철근배치 TEST CODE
//	pBri->SettingTvArrRebarMainPlane();
//	return;

	CString sTitle	= _T("설계환경 불러오기");
	CFileDialog dlg(TRUE, "*.bar",NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
					  "ARcBridge 설계환경 파일 (*.bar)|*.bar||",GetParent());
	dlg.m_ofn.lpstrTitle = sTitle;	
	if (dlg.DoModal() == IDOK)
	{		
		CFile f;
		if(f.Open(dlg.GetPathName(),CFile::modeRead)==FALSE) return;
		CArchive ar(&f,CArchive::load);
		pBri->SerializeEnv(ar);
		ar.Close();
		f.Close();

		SetGridData();
		SetDataInit();
	}	
}

void CInputGenDesignVariable::OnButtonExamine() 
{
	int nTab = m_Tab.GetCurSel();

	if(nTab != m_nTabExposureRating)
		return;

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	m_pDlgCheck->SetBriAndContent(pBri,IDD_GEN_DESIGN_VARIABLE,0);
	m_pDlgCheck->ShowAndCheck(this);	
	CString str = _T("");
	str.Format("노출등급");
	m_pDlgCheck->SetWindowText(str);
}

void CInputGenDesignVariable::SetGridDataExposureRating()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	m_Grid.SetLsdEnviromentRating(pBri->m_plsdEnvRating);	
	m_Grid.SetLsdManager(pBri->m_plsdManager);
	m_Grid.SetGridInitExposureRating();
}

void CInputGenDesignVariable::SetGridDataDesignRating()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	m_Grid.SetLsdEnviromentRating(pBri->m_plsdEnvRating);
	m_Grid.SetLsdManager(pBri->m_plsdManager);
	m_Grid.SetGridInitDesignRating();
}

void CInputGenDesignVariable::SetGridDataRebarFactor()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	
	m_Grid.SetGridDefault(1, 3, 1, 2);
	
	HGINT32 row = 0;
	HGINT32 col = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;

	m_Grid.AddCell(row, col,hggettext("구분"));		
	m_Grid.AddCell(row, col+1,hggettext("종류 [도.설(2015) 5.9.4.3]"));		
	m_Grid.AddCell(row, col+2,hggettext("계수"));

	row++;
	m_Grid.AddCell(row, 0,_T("n1")); 
	m_Grid.AddCell(row, 1,hggettext("부착상태 보정계수(보통의경우)"), DT_LEFT); 
	m_Grid.AddCell(row, 2, &pBri->m_plsdRebar->GetOption().dn1[0]);	m_Grid.SetCellDec(CELL_DEC_2);  m_Grid.SetLastCellMinLimit(0.1);

	row++;
	m_Grid.AddCell(row, 0,_T("n1")); 
	m_Grid.AddCell(row, 1,hggettext("부착상태 보정계수(상부철근, 30cm 이상의 높이에 배치된 철근)"), DT_LEFT); 
	m_Grid.AddCell(row, 2, &pBri->m_plsdRebar->GetOption().dn1[1]);	m_Grid.SetCellDec(CELL_DEC_2);  m_Grid.SetLastCellMinLimit(0.1);

	row++;
	m_Grid.AddCell(row, 0,_T("n2")); 
	m_Grid.AddCell(row, 1,hggettext("철근지름 영향 보정계수(32mm 이하)"), DT_LEFT); 
	m_Grid.AddCell(row, 2, &pBri->m_plsdRebar->GetOption().dn2);	m_Grid.SetCellDec(CELL_DEC_2);  m_Grid.SetLastCellMinLimit(0.1);

	row++;
	m_Grid.AddCell(row, 0,_T("n2")); 
	m_Grid.AddCell(row, 1,hggettext("철근지름 영향 보정계수(32mm 초과, (132-db/100))"), DT_LEFT); 
	m_Grid.AddCell(row, 2,hggettext("-"),nFormat,CELL_READONLY); m_Grid.SetCellDec(CELL_DEC_2); 

	row++;
	m_Grid.AddCell(row, 0,_T("K")); 
	m_Grid.AddCell(row, 1,hggettext("횡철근에 의한 구속(외측철근)"), DT_LEFT); 
	m_Grid.AddCell(row, 2, &pStd->m_plsdRebar->GetOption().dK_RebarPos[iOUTTER],nFormat,CELL_READONLY);  m_Grid.SetCellDec(CELL_DEC_2);

	row++;
	m_Grid.AddCell(row, 0,_T("K")); 
	m_Grid.AddCell(row, 1,hggettext("횡철근에 의한 구속(내측철근)"), DT_LEFT); 
	m_Grid.AddCell(row, 2, &pStd->m_plsdRebar->GetOption().dK_RebarPos[iINNER]);  m_Grid.SetCellDec(CELL_DEC_2);

	row++;
	m_Grid.AddCell(row, 0,_T("fr")); 
	m_Grid.AddCell(row, 1,hggettext("구속응력(Mpa)"), DT_LEFT); 
	m_Grid.AddCell(row, 2, &pBri->m_plsdRebar->GetOption().dRestraintStress);	m_Grid.SetCellDec(CELL_DEC_2);

	row++;
	m_Grid.AddCell(row, 0,_T("Ro")); 
	m_Grid.AddCell(row, 1,hggettext("겹침이음비율(%)"), DT_LEFT); 
	m_Grid.AddCell(row, 2, &pBri->m_plsdRebar->GetOption().dOverRatio);	m_Grid.SetCellDec(CELL_DEC_1);

	row++;
	m_Grid.AddCell(row, 0,_T("Ras")); 
	m_Grid.AddCell(row, 1,hggettext("필요/사용철근량비(As.req/As,.prov)"), DT_LEFT); 
	m_Grid.AddCell(row, 2, &pBri->m_plsdRebar->GetOption().dAsRatio);	m_Grid.SetCellDec(CELL_DEC_3);

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetRowHeightAll(25);
	m_Grid.SetColumnWidth(0, 100);
	m_Grid.SetColumnWidth(1, 400);
	m_Grid.SetColumnWidth(2, 100);
}

void CInputGenDesignVariable::SetGridDataRebarSettle()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	m_Grid.SetRowHeightAll(25);
	m_Grid.SetGridRebarSimplifySettle(pBri->m_plsdRebar);
}

void CInputGenDesignVariable::SetGridDataRebarJoint()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	m_Grid.SetGridRebarSimplifyConnect(pBri->m_plsdRebar);
}

void CInputGenDesignVariable::SeteGridDataLsdSupportRebar()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	CString str =_T(""), strRoad = _T("");

	// 그리드 초기화
	m_Grid.SetGridDefault(2, 4, 0, 3);

	UINT nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	long nRow = 0, nCol = 0;
	long nRows = 0;
	long nCols = 0;	

	// 접속슬래브
	if(pStd->IsDesignKDS2019())
		strRoad = _T("[KDS 24 14 21 4.6.15]");
	else
		strRoad = _T("(도설 5.12.15)");
	m_Grid.AddCell(nRow, nCol++, hggettext("슬래브"), DT_CENTER | DT_VCENTER);
	str.Format(hggettext("건조수축 및 온도철근 %s"), strRoad);
	m_Grid.AddCell(nRow, nCol++, str, DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, str, DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, &pBri->m_bCheckSupportSlabDryingShrinkage, nFormat, CELL_LINE_NO, _T(""), CELL_TYPE_CHECK); 
	m_Grid.AddCell(nRow, nCol++, pBri->m_bCheckSupportSlabDryingShrinkage ? hggettext("검토") : hggettext("검토안함"), DT_CENTER, CELL_LINE_NO);
	nRow++, nCol = 0;

	m_Grid.AddCell(nRow, nCol++, hggettext("슬래브"), DT_CENTER | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, hggettext("주철근비"), DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, hggettext("필요철근량/사용철근량"), DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, &pBri->m_bCheckSupportSlabByReqRebar, nFormat, CELL_LINE_NO, _T(""), CELL_TYPE_CHECK); 
	m_Grid.AddCell(nRow, nCol++, pBri->m_bCheckSupportSlabByReqRebar ? hggettext("필요철근") : hggettext("사용철근"), DT_CENTER, CELL_LINE_NO);
	nRow++, nCol = 0;

	if(pStd->IsDesignKDS2019())
		strRoad = _T("[KDS 24 14 21 4.6.3.1]");
	else
		strRoad = _T("(도설 5.12.3.1)");
	m_Grid.AddCell(nRow, nCol++, hggettext("슬래브"), DT_CENTER | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, hggettext("주철근비"), DT_LEFT | DT_VCENTER);
	str.Format(hggettext("주철근의 20%% %s"), strRoad);
	m_Grid.AddCell(nRow, nCol++, str, DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, &pBri->m_bCheckSupportSlabMainRate, nFormat, CELL_LINE_NO, _T(""), CELL_TYPE_CHECK); 
	m_Grid.AddCell(nRow, nCol++, pBri->m_bCheckSupportSlabMainRate ? hggettext("검토") : hggettext("검토안함"), DT_CENTER, CELL_LINE_NO);
	nRow++, nCol = 0;

	if(pStd->IsDesignKDS2019())
		strRoad = _T("[KDS 24 14 21 4.6.5.3]");
	else
		strRoad = _T("(도설 5.12.5.3)");
	m_Grid.AddCell(nRow, nCol++, hggettext("슬래브"), DT_CENTER | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, hggettext("주철근비"), DT_LEFT | DT_VCENTER);
	str.Format(hggettext("차량방향에 따른 주철근비 %s"), strRoad);
	m_Grid.AddCell(nRow, nCol++, str, DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, &pBri->m_bCheckSupportSlabMainRateSpan, nFormat, CELL_LINE_NO, _T(""), CELL_TYPE_CHECK); 
	m_Grid.AddCell(nRow, nCol++, pBri->m_bCheckSupportSlabMainRateSpan ? hggettext("검토") : hggettext("검토안함"), DT_CENTER, CELL_LINE_NO);
	nRow++, nCol = 0;
	
	// 벽체
	if(pStd->IsDesignKDS2019())
		strRoad = _T("[KDS 24 14 21 4.6.15]");
	else
		strRoad = _T("(도설 5.12.15)");
	m_Grid.AddCell(nRow, nCol++, hggettext("벽체"), DT_CENTER | DT_VCENTER);
	str.Format(hggettext("건조수축 및 온도철근 %s"), strRoad);
	m_Grid.AddCell(nRow, nCol++, str, DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, str, DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, &pBri->m_bCheckSupportWallDryingShrinkage, nFormat, CELL_LINE_NO, _T(""), CELL_TYPE_CHECK); 
	m_Grid.AddCell(nRow, nCol++, pBri->m_bCheckSupportWallDryingShrinkage ? hggettext("검토") : hggettext("검토안함"), DT_CENTER, CELL_LINE_NO);
	nRow++, nCol = 0;

	if(pStd->IsDesignKDS2019())
		strRoad = _T("[KDS 24 14 21 4.6.8.3]");
	else
		strRoad = _T("(도설 5.12.8.3)");
	m_Grid.AddCell(nRow, nCol++, hggettext("벽체"), DT_CENTER | DT_VCENTER);
	str.Format(hggettext("수직 철근량의 25%% %s"), strRoad);
	m_Grid.AddCell(nRow, nCol++, str, DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, str, DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, &pBri->m_bCheckSupportWallMainRate, nFormat, CELL_LINE_NO, _T(""), CELL_TYPE_CHECK); 
	m_Grid.AddCell(nRow, nCol++, pBri->m_bCheckSupportWallMainRate ? hggettext("검토") : hgwgettext("검토안함"), DT_CENTER, CELL_LINE_NO);
	nRow++, nCol = 0;

	// 기초
	if(pStd->IsDesignKDS2019())
		strRoad = _T("[KDS 24 14 21 4.6.15]");
	else
		strRoad = _T("(도설 5.12.15)");
	m_Grid.AddCell(nRow, nCol++, hggettext("기초"), DT_CENTER | DT_VCENTER);
	str.Format(hggettext("건조수축 및 온도철근 %s"), strRoad);
	m_Grid.AddCell(nRow, nCol++, str, DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, str, DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, &pBri->m_bCheckSupportFootDryingShrinkage, nFormat, CELL_LINE_NO, _T(""), CELL_TYPE_CHECK); 
	m_Grid.AddCell(nRow, nCol++, pBri->m_bCheckSupportFootDryingShrinkage ? hggettext("검토") : hggettext("검토안함"), DT_CENTER, CELL_LINE_NO);
	nRow++, nCol = 0;

	if(pStd->IsDesignKDS2019())
		strRoad = _T("[KDS 24 14 21 4.6.2.1]");
	else
		strRoad = _T("(도설 5.12.2.1(1)(다))");
	m_Grid.AddCell(nRow, nCol++, hggettext("기초"), DT_CENTER | DT_VCENTER);
	str.Format(hggettext("건조수축 및 온도철근으로 최소인장철근 만족 %s"), strRoad);
	m_Grid.AddCell(nRow, nCol++, str, DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, str, DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, &pBri->m_bSkipAsMinCheckByHoriRebar, nFormat, CELL_LINE_NO, _T(""), CELL_TYPE_CHECK); 
	m_Grid.AddCell(nRow, nCol++, pBri->m_bSkipAsMinCheckByHoriRebar ? hggettext("적용") : hggettext("적용안함"), DT_CENTER, CELL_LINE_NO);
	nRow++, nCol = 0;

	m_Grid.GetCellCount(nRows, nCols);
	m_Grid.SetRows(nRows);
	m_Grid.SetCols(nCols);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0,100);
	m_Grid.SetColumnWidth(1,100);
	m_Grid.SetColumnWidth(2,350);
	m_Grid.SetColumnWidth(3,30);
	m_Grid.SetColumnWidth(4,60);	
}

void CInputGenDesignVariable::SyncMaterial()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	
	for (HGINT32 ix = ePartUpperSlab + 1; ix < ePartCount; ++ix)
	{
		if (ix == ePartApsStt || ix == ePartApsEnd)
			continue;

		pBri->m_Fck[ix]	 = pBri->m_Fck[ePartUpperSlab];
		pBri->m_Fy[ix]	 = pBri->m_Fy[ePartUpperSlab];
		pBri->m_Fvy[ix]	 = pBri->m_Fvy[ePartUpperSlab];
		pBri->m_Ec[ix]	 = pBri->m_Ec[ePartUpperSlab];
		
	}

	pBri->m_Fck[ePartApsEnd] = pBri->m_Fck[ePartApsStt];
	pBri->m_Fy[ePartApsEnd] = pBri->m_Fy[ePartApsStt];
	pBri->m_Fvy[ePartApsEnd] = pBri->m_Fvy[ePartApsStt];
	pBri->m_Ec[ePartApsEnd] = pBri->m_Ec[ePartApsStt];
}
