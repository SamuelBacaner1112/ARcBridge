// InputBaseDesignBasicData.cpp : implementation file
//

#include "stdafx.h"
#include "ARcBridgeInput.h"
#include "../ARcBridgeData/ARcBridgeData.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputBaseDesignBasicData dialog


CInputBaseDesignBasicData::CInputBaseDesignBasicData(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputBaseDesignBasicData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputBaseDesignBasicData)
	//}}AFX_DATA_INIT

	m_dCover	  = 100; 
	m_dRebarSpace = 125;
	m_nTab	= 0;
	m_nTabFactor	= -1;
	m_nTabSettle	= -1;
	m_nTabJoint		= -1;
	m_nTabWeight	= -1;
	m_nTabUWeight	= -1;
	m_nTabRebarFactor	= -1;
	m_nTabRailLoad		= -1;
	m_nTabTankLoad		= -1;
	m_nTabResistFactor	= -1;
	m_nTabFootResistFactor	= -1;
	bTabTankLoad	= TRUE;
	m_bBecomeSmallGrid	= FALSE;
	m_pLsdManager = NULL;
}


void CInputBaseDesignBasicData::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputBaseDesignBasicData)
	DDX_Control(pDX, IDC_BUTTON_BASEDATA_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_BUTTON_BASEDATA_LOAD, m_btnLoad);
	DDX_Control(pDX, IDC_EDIT_LOAD5, m_editRailDistWheelMid);
	DDX_Control(pDX, IDC_EDIT_LOAD4, m_editRailDistWheel);
	DDX_Control(pDX, IDC_EDIT_LOAD3, m_editRailLoadWheel);
	DDX_Control(pDX, IDC_EDIT_LOAD2, m_editDistributionLoadBack);
	DDX_Control(pDX, IDC_EDIT_LOAD1, m_editDistributionLoadFront);
	DDX_Control(pDX, IDC_TEXT12, m_ctlStatic_Tank5);
	DDX_Control(pDX, IDC_TEXT11, m_ctlStatic_Tank4);
	DDX_Control(pDX, IDC_TEXT9, m_ctlStatic_Trailer6);
	DDX_Control(pDX, IDC_TEXT10, m_ctlStatic_Trailer7);
	DDX_Control(pDX, IDC_EDIT_TANK_WIDTH_SIDE, m_ctlEdit_Tank_WidSide);
	DDX_Control(pDX, IDC_EDIT_TANK_WIDTH_INNER, m_ctlEdit_Tank_WidInner);
	DDX_Control(pDX, IDC_TEXT8, m_ctlStatic_Trailer5);
	DDX_Control(pDX, IDC_TEXT7, m_ctlStatic_Trailer4);
	DDX_Control(pDX, IDC_TEXT6, m_ctlStatic_Trailer3);
	DDX_Control(pDX, IDC_TEXT5, m_ctlStatic_Trailer2);
	DDX_Control(pDX, IDC_TEXT4, m_ctlStatic_Trailer1);
	DDX_Control(pDX, IDC_TEXT3, m_ctlStatic_Tank3);
	DDX_Control(pDX, IDC_EDIT_TRAILER_WIDTH_TOT, m_ctlEdit_Trailer_WidTot);
	DDX_Control(pDX, IDC_EDIT_TRAILER_WIDTH_SIDE, m_ctlEdit_Trailer_WidSide);
	DDX_Control(pDX, IDC_EDIT_TRAILER_WIDTH_INNER, m_ctlEdit_Trailer_WidInner);
	DDX_Control(pDX, IDC_EDIT_TRAILER_P3, m_ctlEdit_Trailer_P3);
	DDX_Control(pDX, IDC_EDIT_TRAILER_P2, m_ctlEdit_Trailer_P2);
	DDX_Control(pDX, IDC_EDIT_TRAILER_P1, m_ctlEdit_Trailer_P1);
	DDX_Control(pDX, IDC_EDIT_TRAILER_L4, m_ctlEdit_Trailer_L4);
	DDX_Control(pDX, IDC_EDIT_TRAILER_L3, m_ctlEdit_Trailer_L3);
	DDX_Control(pDX, IDC_EDIT_TRAILER_L2, m_ctlEdit_Trailer_L2);
	DDX_Control(pDX, IDC_EDIT_TRAILER_L1, m_ctlEdit_Trailer_L1);
	DDX_Control(pDX, IDC_EDIT_TANK_WIDTH_TOT, m_ctlEdit_Tank_WidTot);
	DDX_Control(pDX, IDC_EDIT_TANK_WEIGHT, m_ctlEdit_Tank_Weight);
	DDX_Control(pDX, IDC_EDIT_TANK_LENGTH, m_ctlEdit_Tank_Len);
	DDX_Control(pDX, IDC_BITMAP_TRAILER, m_ctlBM_Trailer);
	DDX_Control(pDX, IDC_BITMAP_TANK, m_ctlBM_Tank);
	DDX_Control(pDX, IDC_GROUPBOX_TRAILER, m_ctlGB_Trailer);
	DDX_Control(pDX, IDC_GROUPBOX_TANK, m_ctlGB_Tank);
	DDX_Control(pDX, IDC_TEXT2, m_ctlStatic_Tank2);
	DDX_Control(pDX, IDC_TEXT1, m_ctlStatic_Tank1);
	DDX_Control(pDX, IDC_BUTTON_REMOVE_LOAD, m_btnRemoveLoad);
	DDX_Control(pDX, IDC_BUTTON_ADD_LOAD, m_btnAddLoad);
	DDX_Control(pDX, IDC_COMBO_RAIL_CAR, m_ComboRailCar);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputBaseDesignBasicData, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputBaseDesignBasicData)
	ON_CBN_SELCHANGE(IDC_COMBO_RAIL_CAR, OnSelchangeComboRailCar)
	ON_BN_CLICKED(IDC_BUTTON_ADD_LOAD, OnButtonAddLoad)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_LOAD, OnButtonRemoveLoad)
	ON_BN_CLICKED(IDC_BUTTON_BASEDATA_LOAD, OnButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_BASEDATA_SAVE, OnButtonSave)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputBaseDesignBasicData message handlers

void CInputBaseDesignBasicData::OnPreInitDialog()
{
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;

	InitTabCtrl();

	SetResize(IDC_TAB,			   SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID,			   SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_ADD_LOAD, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_REMOVE_LOAD, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_COMBO_RAIL_CAR, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_RAIL_CAR, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_GROUPBOX_TANK,	SZ_TOP_LEFT,		SZ_TOP_RIGHT);
	SetResize(IDC_GROUPBOX_TRAILER,	SZ_TOP_LEFT,		SZ_TOP_RIGHT);

	SetResize(IDC_BUTTON_BASEDATA_SAVE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_BASEDATA_LOAD, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	
	SetResize(IDC_EDIT_LOAD1,	SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_LOAD2,	SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_LOAD3,	SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_LOAD4,	SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_LOAD5,	SZ_TOP_RIGHT,		SZ_TOP_RIGHT);

	SetResize(IDC_STATIC_LOAD1,	SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_LOAD2,	SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_LOAD3,	SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_LOAD4,	SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_LOAD5,	SZ_TOP_RIGHT,		SZ_TOP_RIGHT);

	SetHyperHelp(m_pIn->GetManualPath());

	m_Grid.InitGrid(m_pIn->m_pARcBridgeDataStd->m_Fy);
	
	if(m_nTab == m_nTabTankLoad)
	{
		ShowControlTankLoad(TRUE);
		ShowControlRailLoad();
		SetEditDataTankLoad();
	}
	else
	{
		ShowControlTankLoad(FALSE);
		ShowControlRailLoad();
		
		SetGridData();
	}

	m_EarhtquakeDlg.SetKDS(pStd->m_nTypeEarthQuakeApply==1);
	
	if(m_pLsdManager == NULL)
	{
		m_pLsdManager = new CLsdManager;
		m_pLsdManager->SetDesignCondition(pStd->IsDesignKDS2019() ? LSD_DESIGN_CONDITION_KDS_2019 : LSD_DESIGN_CONDITION_LSD_2015);
		m_Grid.SetLsdManager(m_pLsdManager);
	}
}

void CInputBaseDesignBasicData::DrawInputDomyunView(BOOL bZoomAll)
{
	// 기본입력시는 평면을 매번 구해주도록 셋팅해놓자.
	m_pIn->SettingNeedPlane();

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);

	CDomyun Dom(pDom);
	if(m_nTab==m_nTabRailLoad)
	{
		DrawRailLoadInput(&Dom);
	}

	*pDom << Dom;

	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

void CInputBaseDesignBasicData::InitTabCtrl()
{
	m_Tab.RemoveAllTabs();
	m_Tab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_Tab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	m_nTabFactor	= -1;
	m_nTabSettle	= -1;
	m_nTabJoint		= -1;
	m_nTabWeight	= -1;
	m_nTabUWeight	= -1;
	m_nTabRebarFactor	= -1;
	m_nTabRailLoad		= -1;
	m_nTabTankLoad		= -1;
	m_nTabResistFactor	= -1;
	m_nTabFootResistFactor	= -1;

	long nTab = 0;
	m_sArrTabName.RemoveAll();
	m_sArrTabName.Add("설계 환경");					m_nTabFactor		= nTab++;
	if(m_bDesignLsd)
	{
		m_sArrTabName.Add("재료저항계수");			m_nTabResistFactor		= nTab++;
		m_sArrTabName.Add("기초부 저항계수");		m_nTabFootResistFactor	= nTab++;
	}
	else
	{
		m_sArrTabName.Add("철근 정착장");			m_nTabSettle		= nTab++;
		m_sArrTabName.Add("철근 이음장");			m_nTabJoint			= nTab++;
	}
	m_sArrTabName.Add("철근 단위 중량 및 단면적");	m_nTabWeight		= nTab++;
	m_sArrTabName.Add("기타 재료의 단위 중량");		m_nTabUWeight		= nTab++;
	if (m_bDesignLsd == FALSE)
	{
		m_sArrTabName.Add("철근 정착, 이음 보정 계수");	m_nTabRebarFactor	= nTab++;
		m_sArrTabName.Add("철도 하중");					m_nTabRailLoad		= nTab++;
	}
	m_sArrTabName.Add("탱크 및 중차량 하중");		m_nTabTankLoad		= nTab++;

	long i = 0; for(i = 0; i < m_sArrTabName.GetSize(); i++)
		m_Tab.InsertItem(i, m_sArrTabName[i]);

	m_nTab = 0;
	m_Tab.SetCurSel(m_nTab);
}

void CInputBaseDesignBasicData::SetDataInit()
{
	m_Grid.UpdateData(FALSE);
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;
	long nRows = 0;
	long nCols = 0;
	
	long nTypeRail = pStd->m_nTypeRailLoad;
	BOOL bRail2017 = pStd->IsRailDesign2017();
	BOOL bShow = bRail2017 ? (nTypeRail == RAIL_LOAD_USER_2017) : (nTypeRail == RAIL_LOAD_USER);
	int iShow = bShow ? SW_SHOW : SW_HIDE;
	GetDlgItem(IDC_BUTTON_ADD_LOAD)->ShowWindow(iShow);
	GetDlgItem(IDC_BUTTON_REMOVE_LOAD)->ShowWindow(iShow);
	
	bShow = (bRail2017==FALSE && m_nTab==m_nTabRailLoad && (nTypeRail==RAIL_LOAD_EL25 || nTypeRail==RAIL_LOAD_EL22 || nTypeRail==RAIL_LOAD_EL18));
	iShow = bShow ? SW_SHOW : SW_HIDE;
	GetDlgItem(IDC_COMBO_RAIL_CAR)->ShowWindow(iShow);
	GetDlgItem(IDC_STATIC_RAIL_CAR)->ShowWindow(iShow);
	m_ComboRailCar.SetCurSel(pStd->m_nRailQtyCar-1);

	// 머지할게 있는지 체크하고 머지함.
	m_Grid.GetCellCount(nRows, nCols);

	if (m_nTab == m_nTabFactor)
	{
		m_Grid.MergeGridEx(0, 0, 0, 1,TRUE, FALSE);
		m_Grid.MergeGridEx(1, nRows, 0, 0);
	}
	else if(m_nTab == m_nTabSettle)
	{
		m_Grid.MergeGrid(0, 1, 0, nCols-1);
		if(pStd->m_nTypeRebarConnect != 0)
		{
			m_Grid.MergeGrid(nRows-2, nRows-2, 1, nCols-1);
			m_Grid.MergeGrid(nRows-1, nRows-1, 1, nCols-1);
		}
	}
	else if(m_nTab == m_nTabJoint)
	{
		m_Grid.MergeGrid(0, 2, 0, nCols-1);
		if(pStd->m_nTypeRebarConnect != 0)
		{
			m_Grid.MergeGrid(nRows-2, nRows-2, 1, nCols-1);
			m_Grid.MergeGrid(nRows-1, nRows-1, 1, nCols-1);
		}
	}
	else if(m_nTab == m_nTabRebarFactor)
	{
		m_Grid.MergeGrid(1, nRows, 0, 1);
	}
//	else if(m_nTab == m_nTabRailLoad && pStd->IsRailDesign())  
//	{
//		m_Grid.MergeGrid(0, nRows-1, 0, 1);
//	}

	AfxGetMainWnd()->SendMessage(WM_USER+12);

	SetDataInitRailLoadUser();
}

void CInputBaseDesignBasicData::SetDataSave()
{
	m_Grid.UpdateData(TRUE);
	if(m_nTab == m_nTabTankLoad) SetDataSaveTankLoad();

	m_pIn->m_pDoc->SetModifiedFlag();
	AfxGetMainWnd()->SendMessage(WM_USER+12);
	// Update StatusBar Unit Combo
	AfxGetMainWnd()->SendMessage(WM_USER+14);

	SetDataSaveRailLoadUser();

	if(m_nTab == m_nTabFactor)
	{
		m_pIn->m_pARcBridgeDataStd->m_pBasicConcInfo->SetLenghtRebarMax(m_pIn->m_pARcBridgeDataStd->m_dLengthRebarMax);
	}
}
/*
#define DEFAULT_BASEDESIGNBASEDATA_FACTOR		0x00000001
#define DEFAULT_BASEDESIGNBASEDATA_SETTLE		0x00000002
#define DEFAULT_BASEDESIGNBASEDATA_JOINT		0x00000004
#define DEFAULT_BASEDESIGNBASEDATA_WEIGHT		0x00000008
#define DEFAULT_BASEDESIGNBASEDATA_UWEIGHT		0x00000010
#define DEFAULT_BASEDESIGNBASEDATA_REBARFACTOR	0x00000020
#define DEFAULT_BASEDESIGNBASEDATA_RAILLOAD		0x00000040 */

void CInputBaseDesignBasicData::SetDataDefault()
{
	BOOL bFirst	= !IsUsedFlag();
	if(!bFirst && !m_bClickDefaultButton) return;

	if(bFirst)
	{
		CARcBridgeDataStd *pStd    = m_pIn->m_pARcBridgeDataStd;
		pStd->m_nTypeDesignMethod	= pStd->IsLockRcBridgeLsdModule() ? DESIGN_METHOD_LSD : DESIGN_METHOD_STRENGTH;
		pStd->m_pBasicConcInfo->SetDesignLsd(pStd->IsLockRcBridgeLsdModule());
	}

	if(bFirst || m_nTab == m_nTabFactor)		SetDataDefaultFactor();
	if(bFirst || m_nTab == m_nTabSettle)		SetDataDefaultSettle();
	if(bFirst || m_nTab == m_nTabJoint)			SetDataDefaultJoint();
	if(bFirst || m_nTab == m_nTabWeight)		SetDataDefaultWeight();
	if(bFirst || m_nTab == m_nTabUWeight)		SetDataDefaultUWeight();
	if(bFirst || m_nTab == m_nTabRebarFactor)	SetDataDefaultRebarFactor();
	if(bFirst || m_nTab == m_nTabRailLoad)		SetDataDefaultRailLoad();
	if(bFirst || m_nTab == m_nTabTankLoad)		SetDataDefaultTankLoad();
	if(bFirst || m_nTab == m_nTabResistFactor)		SetDataDefaultMatResistFactor();
	if(bFirst || m_nTab == m_nTabFootResistFactor)	SetDataDefaultBaseResistFactor();

	if(m_nTab == m_nTabTankLoad)
	{
		ShowControlTankLoad(TRUE);
		SetEditDataTankLoad();
	}
	else
	{
		ShowControlTankLoad(FALSE);
		SetGridData();
		SetDataInit();
	}
	m_pIn->m_pDoc->SetModifiedFlag();
}

void CInputBaseDesignBasicData::SetDataDefaultFactor_Deduct()
{
	CARcBridgeDataStd *pStd    = m_pIn->m_pARcBridgeDataStd;

	if(pStd->m_nTypeKciCode==DESIGN_CODE_KCI_2003)
	{
		pStd->m_Pi_F = 0.85;
		pStd->m_Pi_C = 0.70;
		pStd->m_Pi_V = 0.80;
	}
	else
	{
		pStd->m_Pi_F = 0.85;
		pStd->m_Pi_C = 0.65;
		pStd->m_Pi_V = 0.75;
	}
	if(pStd->IsDoro2010())
	{
		pStd->m_Pi_C = 0.70;
		pStd->m_Pi_V = 0.80;
	}
	pStd->m_Pi_F_Cobel = pStd->m_Pi_V;

	if(pStd->IsDesignKDS2019() || pStd->IsRailDesignKDS2018())
	{
		pStd->m_Pi_F	= 0.85;
		pStd->m_Pi_C	= 0.65;
		pStd->m_Pi_V	= 0.7;
		pStd->m_PI_B	= 0.65;
		pStd->m_Pi_F_Cobel = 0.75;
	}
	else if(pStd->IsRailDesign2017())
	{
		pStd->m_Pi_F	= 0.90;
		pStd->m_Pi_C	= 0.65;
		pStd->m_Pi_V	= 0.85;
		//pStd->m_PI_B	= 0.65;
		pStd->m_Pi_F_Cobel = 0.80;
	}
	
	pStd->m_Es_Steel			= pStd->GetEs_Steel();
}

void CInputBaseDesignBasicData::SetDataDefaultFactor()
{
	CARcBridgeDataStd *pStd    = m_pIn->m_pARcBridgeDataStd;

	double Wc = (pStd->m_pBasicConcInfo->m_UWeightConcrete>0) ? pStd->m_pBasicConcInfo->m_UWeightConcrete/1000 : 2.350;

	pStd->m_nTypeConditionApply		= DESIGN_CONDITION_ROAD;	// 도로교 설계 기준을 우선적용함
	pStd->m_nTypeKciCode			= DESIGN_CODE_KCI_2012;
	pStd->m_nTypeRoadCode			= pStd->IsLsdDesign() ? DESIGN_ROAD_2015 : DESIGN_ROAD_2010;
	pStd->m_nTypeRailCode			= DESIGN_RAIL_2017;
	pStd->m_nDegreeBridge			= 0;
	pStd->m_Fck					= 270;
	pStd->m_Fy					= 4000;
	pStd->m_Ec					= GetValueEcConc(pStd->m_Fck,Wc,TRUE,pStd->m_nTypeKciCode); // 도로교설계기준 38페이지
	pStd->m_Es					= 2000000;
	pStd->m_Es_Steel			= pStd->GetEs_Steel();
	pStd->m_NValue_Steel			= 8.0;
	pStd->m_nTypeUnit				= UNIT_TYPE_SI;
	pStd->m_nLSDRoadType = 0;
	pStd->m_dDesignRoadWidth = 3600;
	pStd->m_nApplyBoredPileASD = 0;
	pStd->m_nApplyPhcPileASD = 0;

	SetDataDefaultFactor_Deduct();
	pStd->m_dDeltaTemperature		= 20;		// 가동받침의 이동량 산정시 온도변화량
	pStd->m_dCoefficientExpansion	= 0.000010;	// 선팽창계수
	pStd->m_dRateDryAndShrinkage	= 0.00015;	// 건조수축율

	pStd->m_dLengthRebarMax			= 8000;
	pStd->m_nDegreeEarthQuake		=	0;	// 내진등급 (1,2,특)
	pStd->m_nRegionEarthQuake		=	0;	// 지진구역 (1,2)
	pStd->m_dCoefficientGround	    = 1.0;
	pStd->m_nTypeRebarConnect		=	1;  // 철근겹이음 방식 - 보정계수에 의한 방법
	pStd->m_nTypeBMApply			=	1;
	pStd->m_pBasicConcInfo->m_nRoundRebarConnect = 1;
	pStd->m_bIncludeSumZero			= TRUE;

	pStd->m_RatePoasong		= 0.1667;		// 포아슨비
	pStd->m_gValue			= 9.810;		// 중력가속도
	pStd->m_nTypeDesingEQ	= 3;
	
	pStd->m_nTypeDesingEQ			= 1;
	pStd->m_nTypeRigidFootingEQ		= 0;
	pStd->m_nTypeJointPosFootingEQ	= 0;	// 내진해석시 기초부 절점 위치 (기초상단,기초중앙)
	pStd->m_nTypePropertyPosFootingEQ	= 0;	// 내진해석시 기초부 절점 위치 (기초상단,기초중앙)
	pStd->m_nTypeUpperWeightEQ		= 0;	// 내진해석시 상부중량 설정
	pStd->m_nTypeComboEQ			= 0;
	pStd->m_dRateDAMP_EQ			= 0.05;
	pStd->SetStringUnitType();
	pStd->m_bApplyAllowInc			= TRUE;	// 말뚝응력 검토시 허용응력 증가계수 적용
}

void CInputBaseDesignBasicData::SetDataDefaultSettle() 
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;

	m_pIn->m_pARcBridgeDataStd->m_pBasicConcInfo->SetDataDefaultRebarSettle(pStd->m_Fck,pStd->m_Fy);

	m_dCover = 100;
	m_dRebarSpace = 125;
}

void CInputBaseDesignBasicData::SetDataDefaultJoint() 
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;

	m_pIn->m_pARcBridgeDataStd->m_pBasicConcInfo->SetDataDefaultRebarJoint(pStd->m_Fck,pStd->m_Fy);

	m_dCover = 100;
	m_dRebarSpace = 125;
}

void CInputBaseDesignBasicData::SetDataDefaultWeight()
{
	m_pIn->m_pARcBridgeDataStd->m_pBasicConcInfo->SetDataDefaultRebarInfo();
}

void CInputBaseDesignBasicData::SetDataDefaultUWeight()
{
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;
	if(pStd == NULL) return;
	
	CBasicConcInfo *pBasicConc = m_pIn->m_pARcBridgeDataStd->m_pBasicConcInfo;
	BOOL bRail2017 = pStd->IsRailDesign2017();

	pBasicConc->m_nTypeDesignApply	= (pStd->m_nTypeConditionApply<=DESIGN_CONDITION_ROAD) ? 0 : 1;
	pBasicConc->m_bDesignKDS		= pStd->IsDesignKDS2019();
	pBasicConc->SetDataDefaultUnitWeight(pStd->m_nTypeConditionApply);
}

void CInputBaseDesignBasicData::SetDataDefaultRebarFactor()
{
	m_pIn->m_pARcBridgeDataStd->m_pBasicConcInfo->SetDataDefaultRebarFactor();
}

BOOL CInputBaseDesignBasicData::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		if(m_Grid.GetFocusCell().col==m_Grid.GetCols()-1)
			pMsg->wParam = VK_RIGHT;
		else
			pMsg->wParam = VK_TAB;
	}



	return CInputDialogBase::PreTranslateMessage(pMsg);
}

void CInputBaseDesignBasicData::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	
	return;
}

void CInputBaseDesignBasicData::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	SetFocus();
	m_nTab = m_Tab.GetCurSel();
	if(m_nTab == m_nTabTankLoad)
	{
		ShowControlTankLoad(TRUE);
		ShowControlRailLoad();
		SetEditDataTankLoad();
	}
	else
	{
		ShowControlTankLoad(FALSE);
		ShowControlRailLoad();
		SetGridData();
		SetDataInit();
	}

	DrawInputDomyunView(TRUE);

	return;
}

void CInputBaseDesignBasicData::SetGridDataFactor()
{
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 1, 2);

	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;

	long nBaseIndex(0);

	long row = 0;
	long col = 0;
	CStringArray sArr;
	CString str = _T("");
	UINT nFormatLeft  = GVHT_LEFT;
	UINT nFormatRight = DT_RIGHT;
	UINT nLine = CELL_LINE_NO;
	UINT CellLineRoad = (!pStd->IsRailDesign()) ? CELL_LINE_NO : CELL_READONLY;
	UINT CellLineRail = ( pStd->IsRailDesign()) ? CELL_LINE_NO : CELL_READONLY;
	
	BOOL bRail2017 = pStd->IsRailDesign2017();
	BOOL bKDS17 = (pStd->m_nTypeEarthQuakeApply == 0) ? FALSE : TRUE;

	row = 0;
	col = 0;
	m_Grid.AddCell(row, col++,"구  분");
	m_Grid.AddCell(row, col++,"구  분");
	m_Grid.AddCell(row, col++,"설정값");
	m_Grid.AddCell(row++, col,"단  위");

	// 강도설계법 or 한계상태설계법
	if(pStd->IsLockRcBridgeLsdModule())
	{
		sArr.RemoveAll();
		sArr.Add(_T("강도설계법"));
		sArr.Add(_T("한계상태설계법"));
		
		col	= 0;
		m_Grid.AddCell(row, col++, "설계기준 및 등급");
		m_Grid.AddCell(row, col++, hggettext("  설계방법"), nFormatLeft);
		m_Grid.AddCell(row++, col++, &pStd->m_nTypeDesignMethod, nFormatRight, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
	}
		
	sArr.RemoveAll();
	long i=0; for(i=0; i<MAX_CONDITION_TYPE; i++)
		sArr.Add(pStd->GetStringConditionApply(i));
	col = 0;
	m_Grid.AddCell(row, col++, "설계기준 및 등급");
	m_Grid.AddCell(row, col++, "  적용설계기준(하중조합)", nFormatLeft);
	m_Grid.AddCell(row++, col, &pStd->m_nTypeConditionApply, nFormatRight, TRUE, m_bDesignLsd ? CELL_READONLY : nLine, "", CELL_TYPE_COMBO, sArr);

	// (ARCBRIDGE-3536) 선택 없음. 한계상태일때는 'KCI 2012', 강도설계법일때는 도로교2010일때 'KCI 2007'로 고정 도로교2008일때 'KCI 2003' 사용
	UINT nCellLine = CELL_READONLY;
	if(m_bDesignLsd == FALSE)
	{
		if(pStd->IsRailDesign())
		{
			nCellLine = bRail2017 ? CELL_READONLY : CELL_LINE_NO;
		}
		else
		{
			nCellLine = CELL_LINE_NO;
		}
	}
	
	sArr.RemoveAll();
	sArr.Add("KCI 2003");
	sArr.Add("KCI 2007");
	if(m_bDesignLsd || pStd->IsRailDesign2017() || pStd->m_nTypeConditionApply == DESIGN_CONDITION_CONCRETE || pStd->IsDoro2010())
		sArr.Add("KCI 2012");
	col = 0;
	m_Grid.AddCell(row, col++, "설계기준 및 등급");
	m_Grid.AddCell(row, col++, "  콘크리트 구조설계기준", nFormatLeft);
	m_Grid.AddCell(row++, col, &pStd->m_nTypeKciCode, nFormatRight, TRUE, nCellLine, "", CELL_TYPE_COMBO, sArr);

	//
	sArr.RemoveAll();
	if (m_bDesignLsd)
	{
		sArr.Add("도로교설계기준 2015");
		sArr.Add("도로교설계기준 KDS 24 10 11 : 2019");
		nBaseIndex = 2;
	}
	else
	{
		sArr.Add("2008 설계기준");
		sArr.Add("2010 설계기준");
		nBaseIndex = 0;
	}
	
	col = 0;
	m_Grid.AddCell(row, col++, "설계기준 및 등급");
	m_Grid.AddCell(row, col++, "  도로교 설계기준", nFormatLeft);
	m_Grid.AddCell(row++, col, &pStd->m_nTypeRoadCode, nFormatRight, TRUE, pStd->m_nTypeConditionApply == DESIGN_CONDITION_ROAD ? CELL_LINE_NO : CELL_READONLY, "", CELL_TYPE_COMBO, sArr);  m_Grid.SetCellBaseOfIndex(nBaseIndex);

	sArr.RemoveAll(); sArr.Add("1 등교"); sArr.Add("2 등교"); sArr.Add("3 등교");
	
	col = 0;
	m_Grid.AddCell(row, col++, "설계기준 및 등급");
	m_Grid.AddCell(row, col++, "  설계교량의 등급", nFormatLeft);
	if (m_bDesignLsd)
	{
		m_Grid.AddCell(row++, col, &pStd->m_nDegreeBridge, nFormatRight, TRUE, CellLineRoad, "", CELL_TYPE_COMBO, sArr, &m_BridgeLevelDlgLsd);
	}
	else
	{
		m_Grid.AddCell(row++, col, &pStd->m_nDegreeBridge, nFormatRight, TRUE, CellLineRoad, "", CELL_TYPE_COMBO, sArr, &m_BridgeLevelDlg);
	}

	if (m_bDesignLsd == FALSE)
	{
		// 철도교 관련 선택사항
		sArr.RemoveAll();
		sArr.Add("철도설계기준 2004");
		sArr.Add("철도설계기준 2017");
		sArr.Add("철도설계기준 KDS 47 00 00 : 2018");
		
		col = 0;
		m_Grid.AddCell(row, col++, "설계기준 및 등급");
		m_Grid.AddCell(row, col++, "  철도 설계기준", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_nTypeRailCode, nFormatRight, TRUE, CellLineRail, "", CELL_TYPE_COMBO, sArr);

		if(bRail2017 == FALSE)
		{
			sArr.RemoveAll();
			sArr.Add("1 급선");
			sArr.Add("2 급선");
			sArr.Add("3 급선");
			sArr.Add("4 급선");
			col = 0;
			m_Grid.AddCell(row, col++, "설계기준 및 등급");
			m_Grid.AddCell(row, col++, "  설계선로의 등급", nFormatLeft); 
			m_Grid.AddCell(row++, col, &pStd->m_nDegreeRail, nFormatRight, TRUE, CellLineRail, "", CELL_TYPE_COMBO, sArr);
		}		

		sArr.RemoveAll();
		long icMaxRail = bRail2017 ? MAX_RAIL_LOAD_2017 : MAX_RAIL_LOAD;
		for(i = 0; i < icMaxRail; i++)
			sArr.Add(pStd->GetStringRailLoad(i));
		col = 0;
		m_Grid.AddCell(row, col++, "설계기준 및 등급");
		m_Grid.AddCell(row, col++, "  열차하중의 종류", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_nTypeRailLoad, nFormatRight, TRUE, CellLineRail, "", CELL_TYPE_COMBO, sArr);

		sArr.RemoveAll();
		sArr.Add("단선");
		sArr.Add("복선");
		col = 0;
		m_Grid.AddCell(row, col++, "설계기준 및 등급");
		m_Grid.AddCell(row, col++, "  계산선로", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_nTypeRailQty, nFormatRight, TRUE, CellLineRail, "", CELL_TYPE_COMBO, sArr);
	}
	else
	{
		sArr.RemoveAll();
		sArr.Add(_T("일반도로"));
		sArr.Add(_T("고속도로"));
		col = 0;
		m_Grid.AddCell(row, col++, "설계기준 및 등급");
		m_Grid.AddCell(row, col++, "  도로의 종류", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_nLSDRoadType, nFormatRight, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);

		col = 0;
		m_Grid.AddCell(row, col++, "설계기준 및 등급");
		m_Grid.AddCell(row, col++, "  설계차로의 폭(Wp)", nFormatLeft);
		m_Grid.AddCell(row, col++, &pStd->m_dDesignRoadWidth, nFormatRight);
		m_Grid.SetCellDec(CELL_DEC_0);
		m_Grid.AddCell(row++, col, "mm",nFormatLeft);
	}

	col = 0;
	sArr.RemoveAll();
	sArr.Add(_T("시설물별 설계기준"));
	sArr.Add(_T("내진설계(KDS 17 10 00)"));

	UINT nLineEq = (pStd->m_nTypeConditionApply == DESIGN_CONDITION_ROAD && pStd->m_nTypeRoadCode == DESIGN_ROAD_KDS_2019) || pStd->IsRailDesignKDS2018() ? CELL_READONLY : CELL_LINE_NO;
	m_Grid.AddCell(row, col++, "설계기준 및 등급");
	m_Grid.AddCell(row, col++, hggettext(" 내진설계기준"), nFormatLeft);
	m_Grid.AddCell(row++, col, &pStd->m_nTypeEarthQuakeApply, nFormatRight, TRUE, nLineEq, _T(""), CELL_TYPE_COMBO, sArr);

	sArr.RemoveAll(); sArr.Add("CGS 단위계"); sArr.Add("SI 단위계");
	col = 0;
	m_Grid.AddCell(row, col++, "설계기준 및 등급");
	m_Grid.AddCell(row, col++, "  적용단위계", nFormatLeft); 
	m_Grid.AddCell(row++, col, &pStd->m_nTypeUnit, nFormatRight, TRUE, (pStd->m_nTypeConditionApply == DESIGN_CONDITION_ROAD && pStd->m_nTypeRoadCode == DESIGN_ROAD_2010) || m_bDesignLsd ? CELL_READONLY : nLine, "", CELL_TYPE_COMBO, sArr);

	if(m_bDesignLsd)
	{
 		col	= 0;
		m_Grid.AddCell(row, col++, hggettext("설계기준 및 등급"));
		m_Grid.AddCell(row, col++, hggettext(" 하중조합의 출력방법"), nFormatLeft);
		m_Grid.AddCell(row, col++, &pStd->m_bPrintConditionApply, nFormatRight, nLine, _T(""), CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col++, pStd->m_bPrintConditionApply ? hggettext("기본하중조합") : hggettext("세부하중조합"), nFormatRight, CELL_READONLY);
		
		col	= 0;
		sArr.RemoveAll();
		sArr.Add(hggettext("도로교설계기준 2015"));
		sArr.Add(hggettext("강구조설계기준 2014"));
		m_Grid.AddCell(row, col++, hggettext("설계기준 및 등급"));
		m_Grid.AddCell(row, col++, hggettext(" 강관말뚝 검토기준"), nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_nTypeCheckSteelPile, nFormatRight, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
		col	= 0;

		sArr.RemoveAll();
		sArr.Add(_T("적용안함"));
		sArr.Add(_T("적용"));
		col = 0;

		m_Grid.AddCell(row, col++, "설계기준 및 등급");
		m_Grid.AddCell(row, col++, "  강관매입말뚝 검토시 허용응력설계법 적용", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_nApplyBoredPileASD, nFormatRight, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

		col = 0;
		m_Grid.AddCell(row, col++, "설계기준 및 등급");
		m_Grid.AddCell(row, col++, "  PHC말뚝 검토시 허용응력설계법 적용", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_nApplyPhcPileASD, nFormatRight, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

		col = 0;
		sArr.RemoveAll();
		sArr.Add(hggettext("현장타설말뚝의 철근비 적용"));
		sArr.Add(hggettext("지진시 축방향철근비 적용"));
		m_Grid.AddCell(row, col++, "설계기준 및 등급");
		m_Grid.AddCell(row, col++, "  극단상황한계상태Ⅰ의 현장타설말뚝 철근비", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_nSelectPmin_Hyunta_Extreme, nFormatRight, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	}


	CString szKgf_cm2_CGS   = "kgf/cm²";
	CString szKgf_cm2_Apply = pStd->GetStringUnitType(szKgf_cm2_CGS);

	if(m_bDesignLsd == FALSE)
	{
		col = 0;
		m_Grid.AddCell(row, col++, "재료특성 및 계수값");
		m_Grid.AddCell(row, col++, "  fck : 콘크리트 강도", nFormatLeft);
		m_Grid.AddCell(row, col++, &pStd->m_Fck, nFormatRight);
		m_Grid.SetCellDec(CELL_DEC_1);
		m_Grid.SetCellUnit(szKgf_cm2_CGS,szKgf_cm2_Apply);
		m_Grid.AddCell(row++, col, szKgf_cm2_Apply,nFormatLeft);

		col = 0;
		m_Grid.AddCell(row, col++, "재료특성 및 계수값");
		m_Grid.AddCell(row, col++, "  fy  : 철근 항복강도", nFormatLeft);
		m_Grid.AddCell(row, col++, &pStd->m_Fy, nFormatRight);
		m_Grid.SetCellDec(CELL_DEC_1);
		m_Grid.SetCellUnit(szKgf_cm2_CGS,szKgf_cm2_Apply);
		m_Grid.AddCell(row++, col, szKgf_cm2_Apply,nFormatLeft);

		col	= 0;
		m_Grid.AddCell(row, col++, "재료특성 및 계수값");
		m_Grid.AddCell(row, col++, "  fy  : 다웰바 강종", nFormatLeft, nLine);	
		m_Grid.AddCell(row++, col, &pStd->m_bIsDBar_ForDowelBar, nFormatRight, nLine, pStd->m_bIsDBar_ForDowelBar ? "D바" : "H바", CELL_TYPE_CHECK);

		col = 0;
		m_Grid.AddCell(row, col++, "재료특성 및 계수값");
		m_Grid.AddCell(row, col++, "  Ec  : 콘크리트 탄성계수", nFormatLeft);
		m_Grid.AddCell(row, col++, &pStd->m_Ec, nFormatRight);
		m_Grid.SetCellDec(CELL_DEC_1);
		m_Grid.SetCellUnit(szKgf_cm2_CGS,szKgf_cm2_Apply);
		m_Grid.AddCell(row++, col, szKgf_cm2_Apply,nFormatLeft);
	}

	col = 0;
	m_Grid.AddCell(row, col++, "재료특성 및 계수값");
	m_Grid.AddCell(row, col++, "  Es  : 철근의 탄성계수", nFormatLeft);
	m_Grid.AddCell(row, col++, &pStd->m_Es, nFormatRight);
	m_Grid.SetCellDec(CELL_DEC_1);
	m_Grid.SetCellUnit(szKgf_cm2_CGS,szKgf_cm2_Apply);
	m_Grid.AddCell(row++, col, szKgf_cm2_Apply,nFormatLeft); 
	
	col = 0;
	m_Grid.AddCell(row, col++, "재료특성 및 계수값");
	m_Grid.AddCell(row, col++, "  Es  : 강재의 탄성계수", nFormatLeft);
	m_Grid.AddCell(row, col++, &pStd->m_Es_Steel, nFormatRight);
	m_Grid.SetCellDec(CELL_DEC_1);
	m_Grid.SetCellUnit(szKgf_cm2_CGS,szKgf_cm2_Apply);
	m_Grid.AddCell(row++, col, szKgf_cm2_Apply,nFormatLeft); 
	
	if (m_bDesignLsd)
	{
		col = 0;
		m_Grid.AddCell(row, col++, "재료특성 및 계수값");
		m_Grid.AddCell(row, col++, "  v   : 콘크리트의 포아슨비", nFormatLeft);
		m_Grid.AddCell(row++, col++, &pStd->m_RatePoasong, nFormatRight);
	}
	else
	{
		col = 0;
		m_Grid.AddCell(row, col++, "재료특성 및 계수값");
		m_Grid.AddCell(row, col++, "  n   : 강재와 콘크리트의 탄성계수비", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_NValue_Steel, nFormatRight);

		col = 0;
		m_Grid.AddCell(row, col++, "재료특성 및 계수값");
		m_Grid.AddCell(row, col++, "  Φf : 휨부재", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_Pi_F, nFormatRight);
		m_Grid.SetCellDec(CELL_DEC_2); 

		col = 0;
		m_Grid.AddCell(row, col++, "재료특성 및 계수값");
		m_Grid.AddCell(row, col++, "  Φc : 축방향압축부재", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_Pi_C, nFormatRight);
		m_Grid.SetCellDec(CELL_DEC_2); 

		col = 0;
		m_Grid.AddCell(row, col++, "재료특성 및 계수값");
		m_Grid.AddCell(row, col++, "  Φv : 전단과 비틀림", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_Pi_V, nFormatRight);
		m_Grid.SetCellDec(CELL_DEC_2); 

		//	col = 1; m_Grid.AddCell(++row, col++, "  Φb : 콘크리트의 지압", nFormatLeft); m_Grid.AddCell(row, col++, &pStd->m_PI_B, nFormatRight); m_Grid.SetCellDec(CELL_DEC_2); 
		col = 0;
		m_Grid.AddCell(row, col++, "재료특성 및 계수값");
		m_Grid.AddCell(row, col++, "  받침부 설계 감소계수 (Φ)", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_Pi_F_Cobel, nFormatRight);
		m_Grid.SetCellDec(CELL_DEC_2); 

		col = 0;
		m_Grid.AddCell(row, col++, "재료특성 및 계수값");
		m_Grid.AddCell(row, col++, "  ε  : 건조수축률", nFormatLeft); 
		m_Grid.AddCell(row++, col, &pStd->m_dRateDryAndShrinkage, nFormatRight);
		m_Grid.SetCellDec(CELL_DEC_6); 

		col = 0;
		m_Grid.AddCell(row, col++, "재료특성 및 계수값");
		m_Grid.AddCell(row, col++, "  α  : 선팽창계수", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_dCoefficientExpansion, nFormatRight);
		m_Grid.SetCellDec(CELL_DEC_7); 

		//	col = 1; m_Grid.AddCell(++row, col++, "  ΔT  : 가동받침의 이동량 산정시 온도변화량", nFormatLeft); m_Grid.AddCell(row, col++, &pStd->m_dDeltaTemperature, nFormatRight); m_Grid.SetCellDec(CELL_DEC_1); m_Grid.AddCell(row, col++, "도",nFormatLeft); // 온도

		sArr.RemoveAll(); 
		//	sArr.Add("내진해석 하지않음"); 
		//	sArr.Add("단일모드 스펙트럼해석"); 
		//	sArr.Add("다중모드 스펙트럼해석"); // 향후 지원 ?????
		//	col = 1; m_Grid.AddCell(++row, col++, "  내진해석방법", nFormatLeft); m_Grid.AddCell(row, col++, &pStd->m_nTypeDesingEQ, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
		sArr.Add("내진해석 하지않음"); 
		sArr.Add("내진해석 적용"); 
		col = 0;
		m_Grid.AddCell(row, col++, "내진해석");
		m_Grid.AddCell(row, col++, "  내진해석적용", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_nTypeDesingEQ, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);

		if(pStd->m_nTypeDesingEQ != 0)
		{
			m_BridgeLevelDlg.m_nTypeUnit = pStd->m_nTypeUnit;
			col = 0;
			m_Grid.AddCell(row, col++, "내진해석");
			m_Grid.AddCell(row, col++, "  내진등급", nFormatLeft);
			if(bKDS17)
			{
				sArr.RemoveAll(); sArr.Add("I 등급"); sArr.Add("II등급"); sArr.Add("특등급 2400"); sArr.Add("특등급 4800");
				m_Grid.AddCell(row++, col, &pStd->m_nDegreeEarthQuake, nFormatRight, TRUE, nLine, "", CELL_TYPE_AUTO, sArr, &m_EarthquakeGradeKDS17Dlg);
			}
			else
			{
				sArr.RemoveAll(); sArr.Add("I 등급"); sArr.Add("II등급");;
				m_Grid.AddCell(row++, col, &pStd->m_nDegreeEarthQuake, nFormatRight, TRUE, nLine, "", CELL_TYPE_AUTO, sArr, &m_EarthquakeGradeDlg);
			}

			sArr.RemoveAll(); sArr.Add("I 구역"); sArr.Add("II구역");
			col = 0;
			m_Grid.AddCell(row, col++, "내진해석");
			m_Grid.AddCell(row, col++, "  지진구역", nFormatLeft);
			m_Grid.AddCell(row++, col, &pStd->m_nRegionEarthQuake, nFormatRight, TRUE, nLine, "", CELL_TYPE_AUTO, sArr, &m_EarhtquakeDlg);

			col = 0;
			m_Grid.AddCell(row, col++, "내진해석");
			m_Grid.AddCell(row, col++, bKDS17 ? "  지반보정계수" : "  지반계수", nFormatLeft);
			if(bKDS17)
				m_Grid.AddCell(row++, col, &pStd->m_dCoefficientGround, nFormatRight, nLine);
			else
				m_Grid.AddCell(row++, col, &pStd->m_dCoefficientGround, nFormatRight, nLine, "", CELL_TYPE_AUTO, sArr, &m_EarthquakeFactDlg);
			m_Grid.SetCellDec(CELL_DEC_2);
			
			if(bKDS17)
			{
				sArr.RemoveAll();
				col = 0;
				m_Grid.AddCell(row, col++, "내진해석");
				m_Grid.AddCell(row, col++, "  지반종류", nFormatLeft);
				for(long ix = 0; ix < 6; ++ix)
				{
					str.Format(_T("S%d"), ix+1);
					sArr.Add(str);
				}
				m_Grid.AddCell(row++, col, &pStd->m_nTypeCoefficientGround, nFormatRight, TRUE, nLine, "", CELL_TYPE_AUTO, sArr, &m_EarthquakeFactKDS17Dlg);
				m_Grid.SetCellDec(CELL_DEC_2);
			
				col = 0;
				m_Grid.AddCell(row, col++, "내진해석");
				m_Grid.AddCell(row, col++, hggettext("  감쇠비(ξ) : 감쇠보정계수(CD) 산정"), nFormatLeft);
				m_Grid.AddCell(row, col++, &pStd->m_dRateDAMP_EQ_CD, nFormatRight);	m_Grid.SetCellDec(CELL_DEC_3);
				m_Grid.AddCell(row++, col, _T("%"), nFormatLeft);
			}

			col = 0;
			m_Grid.AddCell(row, col++, "내진해석");
			m_Grid.AddCell(row, col++, "  중력 가속도", nFormatLeft);
			m_Grid.AddCell(row, col++, &pStd->m_gValue, nFormatRight);
			m_Grid.SetCellDec(CELL_DEC_3);
			m_Grid.AddCell(row++, col, "m/sec²",nFormatLeft);

			sArr.RemoveAll(); 
			sArr.Add("벽체강성과 기초강성을 조합"); 
			sArr.Add("벽체강성과 기초강성을 개별적용"); 
			sArr.Add("기초강성을 무한강성으로 적용");
			col = 0;
			m_Grid.AddCell(row, col++, "내진해석");
			m_Grid.AddCell(row, col++, "  내진해석시 강성 모델링 방법", nFormatLeft);
			m_Grid.AddCell(row++, col, &pStd->m_nTypeRigidFootingEQ, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);

			sArr.RemoveAll(); sArr.Add("적용"); sArr.Add("적용안함");
			col = 0;
			m_Grid.AddCell(row, col++, "내진해석");
			m_Grid.AddCell(row, col++, "  벽체강성 개별적용시 회전강성 적용", nFormatLeft);
			m_Grid.AddCell(row++, col, &pStd->m_bApplyRigidWallEQRotate, nFormatRight, nLine, "", CELL_TYPE_COMBO, sArr);

			sArr.RemoveAll(); 
			sArr.Add("기초의 상단 위치"); 
			sArr.Add("기초의 도심 위치"); 
			col = 0;
			m_Grid.AddCell(row, col++, "내진해석");
			m_Grid.AddCell(row, col++, "  내진해석시 기초 모델링 위치", nFormatLeft);
			m_Grid.AddCell(row++, col, &pStd->m_nTypeJointPosFootingEQ, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);

			sArr.RemoveAll(); 
			sArr.Add("기초의 강성"); 
			sArr.Add("벽체의 강성"); 
			col = 0;
			m_Grid.AddCell(row, col++, "내진해석");
			m_Grid.AddCell(row, col++, "  내진해석시 기초 모델링 강성", nFormatLeft);
			m_Grid.AddCell(row++, col, &pStd->m_nTypePropertyPosFootingEQ, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);

			sArr.RemoveAll(); 
			sArr.Add("정적하중과 동적하중을 합력 재하 - 도로설계편람(건설교통부,2001)"); 
			sArr.Add("정적하중과 동적하중을 분리 재하 - 내진설계편람(한국도로공사,2000)"); 
			col = 0;
			m_Grid.AddCell(row, col++, "내진해석");
			m_Grid.AddCell(row, col++, "  내진해석시 수평토압 재하 위치", nFormatLeft);
			m_Grid.AddCell(row++, col, &pStd->m_nTypeSoilPressureEQ, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);

			sArr.RemoveAll(); 
			sArr.Add("상부구조물(슬래브,기타구조물)"); 
			sArr.Add("상부구조물,하부구조1/2"); 
			col = 0;
			m_Grid.AddCell(row, col++, "내진해석");
			m_Grid.AddCell(row, col++, "  내진해석시 연직중량 산정 범위", nFormatLeft);
			m_Grid.AddCell(row++, col, &pStd->m_nTypeUpperWeightEQ, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);

			//	sArr.RemoveAll(); // 다중모드에서만 사용....
			//	sArr.Add("CQC"); 
			//	sArr.Add("GMC"); 
			//	sArr.Add("SRSS"); 
			//	sArr.Add("ABS");
			//	col = 1; m_Grid.AddCell(++row, col++, "  내진해석결과에 대한 조합 방법", nFormatLeft); m_Grid.AddCell(row, col++, &pStd->m_nTypeComboEQ, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
			//
			//	col = 1; m_Grid.AddCell(++row, col++, "  내진해석시 감쇠비", nFormatLeft); m_Grid.AddCell(row, col++, &pStd->m_dRateDAMP_EQ, nFormatRight); m_Grid.SetCellDec(CELL_DEC_2); 
		}
	}

	col = 0;
	m_Grid.AddCell(row, col++, "철근");
	m_Grid.AddCell(row, col++, "  철근 겹이음 적용 길이", nFormatLeft);
	m_Grid.AddCell(row, col++, &pStd->m_dLengthRebarMax, nFormatRight);
	m_Grid.AddCell(row++, col, "mm",nFormatLeft);
	
	if (m_bDesignLsd == FALSE)
	{
		sArr.RemoveAll(); sArr.Add("보정계수에 의한방법"); sArr.Add("산식에 의한방법");
		col = 0;
		m_Grid.AddCell(row, col++, "철근");
		m_Grid.AddCell(row, col++, "  겹이음 및 정착장 산출방법", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_nTypeRebarConnect, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
	}

	sArr.RemoveAll(); sArr.Add("반올림 안함"); sArr.Add("반올림"); sArr.Add("올림");
	col = 0;
	m_Grid.AddCell(row, col++, "철근");
	m_Grid.AddCell(row, col++, "  철근 겹이음, 정착장 산정시 반올림", nFormatLeft);
	m_Grid.AddCell(row++, col, &pStd->m_pBasicConcInfo->m_nRoundRebarConnect, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);

	sArr.RemoveAll();
	sArr.Add("반올림 안함");
	sArr.Add("MM단위 반올림");
	sArr.Add("CM단위 3사4입");
	sArr.Add("CM단위 4사5입");
	col = 0;
	m_Grid.AddCell(row, col++, "철근");
	m_Grid.AddCell(row, col++, "  철근 길이 반올림 적용", nFormatLeft);
	m_Grid.AddCell(row++, col, &pStd->m_pBasicConcInfo->m_nRoundRebarLength, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
	
	if (m_bDesignLsd == FALSE)
	{
		sArr.RemoveAll(); sArr.Add("적용"); sArr.Add("적용안함");
		col = 0;
		m_Grid.AddCell(row, col++, "기타 설정");
		m_Grid.AddCell(row, col++, "  말뚝응력 검토시 허용응력 증가계수 적용", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_bApplyAllowInc, nFormatRight, nLine, "", CELL_TYPE_COMBO, sArr);
	}

	sArr.RemoveAll();
	sArr.Add("파일명_교량명_방향");
	sArr.Add("교량명_방향");
	sArr.Add("파일명_방향");
	sArr.Add("파일명_교량명");
	sArr.Add("교량명");
	col = 0;
	m_Grid.AddCell(row, col++, "기타 설정");
	m_Grid.AddCell(row, col++, "  Excel출력시 파일명 설정방법", nFormatLeft);
	m_Grid.AddCell(row++, col, &pStd->m_nTypeFileNameForExcel, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);

	if (m_bDesignLsd == FALSE)
	{
		if(pStd->IsRailDesign2017())
		{
			sArr.RemoveAll(); sArr.Add("적용"); sArr.Add("적용안함");
			col = 0;
			m_Grid.AddCell(row, col++, "기타 설정");
			m_Grid.AddCell(row, col++, "  철도교 안정검토시 활하중 충격 포함", nFormatLeft);
			m_Grid.AddCell(row++, col, &pStd->m_nTypeFootingSafetyApplyCoefficientImpact, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
		}
		else
		{
			pStd->m_nTypeFootingSafetyApplyCoefficientImpact = 0;
		}

		sArr.RemoveAll(); sArr.Add("적용"); sArr.Add("적용안함");
		col = 0;
		m_Grid.AddCell(row, col++, "기타 설정");
		m_Grid.AddCell(row, col++, "  철도교 슬래브 유효폭 산정시 슬래브 두께 적용", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_nTypeCalcSlabEffWidth, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
	}
	
	long nLineTypeProcess	= nLine;
	if(!pStd->IsEnableSlabStandardOutput())
	{
		nLineTypeProcess	= CELL_READONLY;
		pStd->m_nTypeProcess	= TYPE_PROCESS_NORMAL;
	}
	else
	{
		if (m_bDesignLsd == FALSE)
		{
			col	= 0;
			m_Grid.AddCell(row, col++, "기타 설정");
			m_Grid.AddCell(row, col++, "  슬래브 표준도 작업", nFormatLeft); 
			m_Grid.AddCell(row++, col, &pStd->m_nTypeProcess, nFormatRight, FALSE, nLineTypeProcess, pStd->m_nTypeProcess == TYPE_PROCESS_NORMAL ? "적용안함" : "적용", CELL_TYPE_CHECK);
		}
	}


	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0,200);
	m_Grid.SetColumnWidth(1,300);
	m_Grid.SetColumnWidth(2,250);
	m_Grid.SetColumnWidth(3, 80);
}

void CInputBaseDesignBasicData::SetGridDataSettle()
{
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;
	if(!pStd) return;
	CBasicConcInfo *basicConc	= pStd->m_pBasicConcInfo;

	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 2, 1);

	long nRow = 0;
	long nCol = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	UINT nLine = CELL_LINE_NO;	

	CString str = _T("");
	if(pStd->m_nTypeRebarConnect==0)
	{	
		// 타이틀 그리기
		m_Grid.AddCell(nRow, 0,"철근호칭\n");
		m_Grid.AddCell(nRow+1, 0,"철근호칭\n");
		m_Grid.AddCell(nRow, 1,"인장철근");
		m_Grid.AddCell(nRow, 2,"인장철근");
		m_Grid.AddCell(nRow, 3,"인장철근");
		m_Grid.AddCell(nRow, 4,"압축철근");
		m_Grid.AddCell(nRow, 5,"압축철근");
		nRow = 1;
		m_Grid.AddCell(nRow, 1,"기본정착");
		m_Grid.AddCell(nRow, 2,"일반철근");
		m_Grid.AddCell(nRow, 3,"상부철근");	
		m_Grid.AddCell(nRow, 4,"기본정착");
		m_Grid.AddCell(nRow, 5,"나선철근");
		nRow = 2;
		m_Grid.AddCell(nRow++, 0,"D10");
		m_Grid.AddCell(nRow++, 0,"D13");
		m_Grid.AddCell(nRow++, 0,"D16");
		m_Grid.AddCell(nRow++, 0,"D19");
		m_Grid.AddCell(nRow++, 0,"D22");
		m_Grid.AddCell(nRow++, 0,"D25");
		m_Grid.AddCell(nRow++, 0,"D29");
		m_Grid.AddCell(nRow++, 0,"D32");
		m_Grid.AddCell(nRow++, 0,"D35");
		m_Grid.AddCell(nRow++, 0,"D38");
		m_Grid.AddCell(nRow++, 0,"D41");
		m_Grid.AddCell(nRow++, 0,"D51");

		// 데이타
		m_Grid.GetCellCount(nRow, nCol);
		CString str = _T("");
		long i=0; for(i=2; i<nRow; i++)
		{
			long j=0; for(j=1; j<nCol; j++)
			{
				m_Grid.AddCell(i, j, &m_pIn->m_pARcBridgeDataStd->m_pBasicConcInfo->m_RebarSettle[i-2][j-1], nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
			}
		}
	}
	else
	{			
		// 타이틀
		m_Grid.AddCell(nRow, 0,"철근호칭\n");
		m_Grid.AddCell(nRow+1, 0,"철근호칭\n");
		m_Grid.AddCell(nRow, 1,"인장철근");
		m_Grid.AddCell(nRow, 2,"인장철근");
		m_Grid.AddCell(nRow, 3,"인장철근");
		m_Grid.AddCell(nRow, 4,"압축철근");
		m_Grid.AddCell(nRow, 5,"압축철근");
		nRow = 1;
		m_Grid.AddCell(nRow, 1,"기본정착");
		m_Grid.AddCell(nRow, 2,"일반철근");
		m_Grid.AddCell(nRow, 3,"상부철근");	
		m_Grid.AddCell(nRow, 4,"기본정착");
		m_Grid.AddCell(nRow++, 5,"나선철근");
		m_Grid.AddCell(nRow++, 0,"D10");
		m_Grid.AddCell(nRow++, 0,"D13");
		m_Grid.AddCell(nRow++, 0,"D16");
		m_Grid.AddCell(nRow++, 0,"D19");
		m_Grid.AddCell(nRow++, 0,"D22");
		m_Grid.AddCell(nRow++, 0,"D25");
		m_Grid.AddCell(nRow++, 0,"D29");
		m_Grid.AddCell(nRow++, 0,"D32");
		m_Grid.AddCell(nRow++, 0,"D35");
		m_Grid.AddCell(nRow++, 0,"D38");
		m_Grid.AddCell(nRow++, 0,"D41");
		m_Grid.AddCell(nRow++, 0,"D51");
		m_Grid.AddCell(nRow++, 0,"피복두께");	
		m_Grid.AddCell(nRow++, 0,"철근간격");	

		// 데이타
		CString str = _T("");
		double dVal[] = { 10, 13, 16, 19, 22, 25, 29, 32, 35, 38 , 41, 51 };
		m_Grid.GetCellCount(nRow, nCol);

		long i=0; for(i=2; i<nRow-2; i++)
		{
			long j=0; for(j=1; j<nCol; j++)
			{
				basicConc->m_RebarSettle[i-2][j-1]	= pStd->GetValueSettle(dVal[i-2], j-1, m_dCover/10, m_dRebarSpace/10);
				str.Format("%.0f",basicConc->m_RebarSettle[i-2][j-1]);
				m_Grid.AddCell(i, j, str, nFormat, CELL_READONLY);
			}
		}
		
		for(long c = 1; c < nCol; c++)
		{
			if(c == 1)
			{
				m_Grid.AddCell(i, c, &m_dCover);
				m_Grid.AddCell(i+1, c, &m_dRebarSpace);
			}
			else
			{
				str.Format("%.0f",m_dCover);
				m_Grid.AddCell(i, c, str);
				str.Format("%.0f",m_dRebarSpace);
				m_Grid.AddCell(i+1, c, str);
			}
		}
	}

	m_Grid.SetRows(nRow);
	m_Grid.SetCols(nCol);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(70);
}

void CInputBaseDesignBasicData::SetGridDataJoint()
{
	// 그리드 초기화
	m_Grid.SetGridDefault(3, 3, 3, 1);
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;

	long nRow = 0;
	long nCol = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	UINT nLine = CELL_LINE_NO;	
	
	if(pStd->m_nTypeRebarConnect==0)
	{	
		m_Grid.AddCell(nRow, 0,"철근호칭\n");
		m_Grid.AddCell(nRow+1, 0,"철근호칭\n");
		m_Grid.AddCell(nRow+2, 0,"철근호칭\n");
		m_Grid.AddCell(nRow, 1,"인장철근");
		m_Grid.AddCell(nRow, 2,"인장철근");
		m_Grid.AddCell(nRow, 3,"인장철근");
		m_Grid.AddCell(nRow, 4,"인장철근");
		m_Grid.AddCell(nRow, 5,"압축철근");
		m_Grid.AddCell(nRow, 6,"압축철근");
		nRow = 1;
		m_Grid.AddCell(nRow, 1,"일반");
		m_Grid.AddCell(nRow, 2,"일반");
		m_Grid.AddCell(nRow, 3,"상부");
		m_Grid.AddCell(nRow, 4,"상부");
		m_Grid.AddCell(nRow, 5,"일반");	
		m_Grid.AddCell(nRow, 6,"폐합");
		nRow = 2;
		m_Grid.AddCell(nRow, 1,"A급");
		m_Grid.AddCell(nRow, 2,"B급");
		m_Grid.AddCell(nRow, 3,"A급");
		m_Grid.AddCell(nRow, 4,"B급");
		m_Grid.AddCell(nRow, 5,"B급");
		m_Grid.AddCell(nRow, 6,"띠철근");
		m_Grid.AddCell(0, 7,"나선철근\n");
		m_Grid.AddCell(1, 7,"나선철근\n");
		m_Grid.AddCell(2, 7,"나선철근\n");
		//
		nRow = 3;
		m_Grid.AddCell(nRow++, 0,"D10");
		m_Grid.AddCell(nRow++, 0,"D13");
		m_Grid.AddCell(nRow++, 0,"D16");
		m_Grid.AddCell(nRow++, 0,"D19");
		m_Grid.AddCell(nRow++, 0,"D22");
		m_Grid.AddCell(nRow++, 0,"D25");
		m_Grid.AddCell(nRow++, 0,"D29");
		m_Grid.AddCell(nRow++, 0,"D32");
		m_Grid.AddCell(nRow++, 0,"D35");
		m_Grid.AddCell(nRow++, 0,"D38");
		m_Grid.AddCell(nRow++, 0,"D41");
		m_Grid.AddCell(nRow++, 0,"D51");


		m_Grid.GetCellCount(nRow, nCol);
		CString str = _T("");
		long i=0; for(i=3; i<nRow; i++)
		{
			long j=0; for(j=1; j<nCol; j++)
			{
				m_Grid.AddCell(i, j, &m_pIn->m_pARcBridgeDataStd->m_pBasicConcInfo->m_RebarJoint[i-3][j-1], nFormat, nLine);
			}
		}
	}
	else
	{	
		m_Grid.AddCell(nRow, 0,"철근호칭\n");
		m_Grid.AddCell(nRow+1, 0,"철근호칭\n");
		m_Grid.AddCell(nRow+2, 0,"철근호칭\n");
		m_Grid.AddCell(nRow, 1,"인장철근");
		m_Grid.AddCell(nRow, 2,"인장철근");
		m_Grid.AddCell(nRow, 3,"인장철근");
		m_Grid.AddCell(nRow, 4,"인장철근");
		m_Grid.AddCell(nRow, 5,"압축철근");
		m_Grid.AddCell(nRow, 6,"압축철근");
		nRow = 1;
		m_Grid.AddCell(nRow, 1,"일반");
		m_Grid.AddCell(nRow, 2,"일반");
		m_Grid.AddCell(nRow, 3,"상부");
		m_Grid.AddCell(nRow, 4,"상부");
		m_Grid.AddCell(nRow, 5,"일반");	
		m_Grid.AddCell(nRow, 6,"폐합");
		nRow = 2;
		m_Grid.AddCell(nRow, 1,"A급");
		m_Grid.AddCell(nRow, 2,"B급");
		m_Grid.AddCell(nRow, 3,"A급");
		m_Grid.AddCell(nRow, 4,"B급");
		m_Grid.AddCell(nRow, 5,"B급");
		m_Grid.AddCell(nRow, 6,"띠철근");
		m_Grid.AddCell(0, 7,"나선철근\n");
		m_Grid.AddCell(1, 7,"나선철근\n");
		m_Grid.AddCell(2, 7,"나선철근\n");
		//
		nRow = 3;
		m_Grid.AddCell(nRow++, 0,"D10");
		m_Grid.AddCell(nRow++, 0,"D13");
		m_Grid.AddCell(nRow++, 0,"D16");
		m_Grid.AddCell(nRow++, 0,"D19");
		m_Grid.AddCell(nRow++, 0,"D22");
		m_Grid.AddCell(nRow++, 0,"D25");
		m_Grid.AddCell(nRow++, 0,"D29");
		m_Grid.AddCell(nRow++, 0,"D32");
		m_Grid.AddCell(nRow++, 0,"D35");
		m_Grid.AddCell(nRow++, 0,"D38");
		m_Grid.AddCell(nRow++, 0,"D41");
		m_Grid.AddCell(nRow++, 0,"D51");

//		nRow = 15;
		m_Grid.AddCell(nRow++, 0,"피복두께");	
		m_Grid.AddCell(nRow, 0,"철근간격");	

		m_Grid.GetCellCount(nRow, nCol);
		CString str = _T("");
		double dVal[] = { 10, 13, 16, 19, 22, 25, 29, 32, 35, 38 , 41, 51 };
		long i=0; for(i=3; i<nRow-2; i++)
		{
			long j=0; for(j=1; j<nCol; j++)
			{
				str.Format("%g",m_pIn->m_pARcBridgeDataStd->GetValueJoint(dVal[i-3], j-1, m_dCover/10, m_dRebarSpace/10));
				m_Grid.AddCell(i,j,str, nFormat, CELL_READONLY);
			}			
		}

		for(long c = 1; c < nCol; c++)
		{
			if(c == 1)
			{
				m_Grid.AddCell(i, c, &m_dCover);
				m_Grid.AddCell(i+1, c, &m_dRebarSpace);
			}
			else
			{
				str.Format("%.0f",m_dCover);
				m_Grid.AddCell(i, c, str);
				str.Format("%.0f",m_dRebarSpace);
				m_Grid.AddCell(i+1, c, str);
			}
		}
	}

		
	m_Grid.SetRows(nRow);
	m_Grid.SetCols(nCol);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(70);
}

void CInputBaseDesignBasicData::SetGridDataWeight()
{
	// 그리드 초기화
	m_Grid.SetGridDefault();
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;

	long nRow = 0;
	long nCol = 0;
// 	CString szCGS   = "mm²";
// 	CString szApply = pStd->GetStringUnitType(szCGS);
	CString szTitle	= _T("");

	m_Grid.AddCell(nRow, nCol++, "\n구 분");
	m_Grid.AddCell(nRow, nCol++, "\n단위질량\n(kg/m)");
	
	CString szCGS   = "cm²";
	CString szApply = pStd->GetStringUnitType(szCGS);
	szTitle.Format("\n단면적\n(%s)", szApply);
	m_Grid.AddCell(nRow, nCol++, szTitle);

	szTitle.Format("\n지  름\n(%s)", "mm");
	m_Grid.AddCell(nRow, nCol++, szTitle);

	szCGS   = "cm";
	szApply = pStd->GetStringUnitType(szCGS);
	szTitle.Format("\n둘  레\n(%s)", szApply);
	m_Grid.AddCell(nRow, nCol++, szTitle);

	szTitle.Format("\n모서리부 구부림\n반지름(%s)", "mm");
	m_Grid.AddCell(nRow, nCol++, szTitle);

	szTitle.Format("\n모서리부 절곡시\n적용값(%s)", "mm");
	m_Grid.AddCell(nRow, nCol++, szTitle);
	//
	nRow = 1;
	m_Grid.AddCell(nRow++, 0,"D10");
	m_Grid.AddCell(nRow++, 0,"D13");
	m_Grid.AddCell(nRow++, 0,"D16");
	m_Grid.AddCell(nRow++, 0,"D19");
	m_Grid.AddCell(nRow++, 0,"D22");
	m_Grid.AddCell(nRow++, 0,"D25");
	m_Grid.AddCell(nRow++, 0,"D29");
	m_Grid.AddCell(nRow++, 0,"D32");
	m_Grid.AddCell(nRow++, 0,"D35");
	m_Grid.AddCell(nRow++, 0,"D38");
	m_Grid.AddCell(nRow++, 0,"D41");
	m_Grid.AddCell(nRow++, 0,"D51");


	CString str = _T("");
	long i=0; for(i=1; i<nRow; i++)
	{
		long j=0; for(j=1; j<nCol; j++)
		{
			if(j==nCol-1 || j==nCol-2)
				str.Format("%.0f", m_pIn->m_pARcBridgeDataStd->m_pBasicConcInfo->m_RebarWeight[i-1][j-1]);
			else
				str.Format("%.3f", m_pIn->m_pARcBridgeDataStd->m_pBasicConcInfo->m_RebarWeight[i-1][j-1]);
			m_Grid.AddCell(i, j, &m_pIn->m_pARcBridgeDataStd->m_pBasicConcInfo->m_RebarWeight[i-1][j-1]);
			if(j==nCol-1 || j==nCol-2)
				m_Grid.SetCellDec(0);
			else
				m_Grid.SetCellDec(3);

			if(j == 2)	// 단면적 단위계 적용
			{
				szCGS   = "cm²";
				szApply = pStd->GetStringUnitType(szCGS);
				m_Grid.SetCellUnit(szCGS, szApply);
			}
			else if(j == 4)
			{
				szCGS   = "cm"; 
				szApply = pStd->GetStringUnitType(szCGS);
				m_Grid.SetCellUnit(szCGS, szApply);
			}
		}
	}

	m_Grid.GetCellCount(nRow, nCol);
	m_Grid.SetRows(nRow);
	m_Grid.SetCols(nCol);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0,50);
	m_Grid.SetColumnWidthAll(105);
}

void CInputBaseDesignBasicData::SetGridDataUWeight()
{
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 1, 1);
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;

	long nRow = 0;
	long nCol = 0;
	long nRows = 0;
	long nCols = 0;
	
	m_Grid.AddCell(nRow  , 0,"구  분");
	m_Grid.AddCell(nRow  , 1,"설정값");
	m_Grid.AddCell(nRow++, 2,"단  위");

	CString szKgf_m3_CGS   = "kgf/m³";
	CString szKgf_m3_Apply = pStd->GetStringUnitType(szKgf_m3_CGS);
	long nDot = (pStd->m_nTypeUnit==UNIT_TYPE_SI) ? (m_bDesignLsd ? 7 : CELL_DEC_3) : CELL_DEC_0;
	//
	m_Grid.AddCell(nRow, nCol,"  강재, 주강 단강",GVHT_LEFT); m_Grid.AddCell(nRow, nCol+1, &pStd->m_pBasicConcInfo->m_UWeightSteel, DT_RIGHT);		m_Grid.SetCellDec(nDot);  m_Grid.SetCellUnit(szKgf_m3_CGS,szKgf_m3_Apply);	m_Grid.AddCell(nRow++, nCol+2, szKgf_m3_Apply, DT_LEFT);
	m_Grid.AddCell(nRow, nCol,"  주철",GVHT_LEFT);			 m_Grid.AddCell(nRow, nCol+1, &pStd->m_pBasicConcInfo->m_UWeightCastIron, DT_RIGHT);		m_Grid.SetCellDec(nDot);   m_Grid.SetCellUnit(szKgf_m3_CGS,szKgf_m3_Apply);	m_Grid.AddCell(nRow++, nCol+2, szKgf_m3_Apply, DT_LEFT);
	m_Grid.AddCell(nRow, nCol,"  알미늄",GVHT_LEFT);			 m_Grid.AddCell(nRow, nCol+1, &pStd->m_pBasicConcInfo->m_UWeightAluminum, DT_RIGHT);		m_Grid.SetCellDec(nDot);   m_Grid.SetCellUnit(szKgf_m3_CGS,szKgf_m3_Apply);	m_Grid.AddCell(nRow++, nCol+2, szKgf_m3_Apply, DT_LEFT);
	m_Grid.AddCell(nRow, nCol,"  철근 콘크리트",GVHT_LEFT);	 m_Grid.AddCell(nRow, nCol+1, &pStd->m_pBasicConcInfo->m_UWeightSteelConcrete, DT_RIGHT);m_Grid.SetCellDec(nDot);   m_Grid.SetCellUnit(szKgf_m3_CGS,szKgf_m3_Apply);	m_Grid.AddCell(nRow++, nCol+2, szKgf_m3_Apply, DT_LEFT);
	m_Grid.AddCell(nRow, nCol,"  자갈,쇄석",GVHT_LEFT);		 m_Grid.AddCell(nRow, nCol+1, &pStd->m_pBasicConcInfo->m_UWeightGravel, DT_RIGHT);		m_Grid.SetCellDec(nDot);   m_Grid.SetCellUnit(szKgf_m3_CGS,szKgf_m3_Apply);	m_Grid.AddCell(nRow++, nCol+2, szKgf_m3_Apply, DT_LEFT);
	m_Grid.AddCell(nRow, nCol,"  시멘트 모르터",GVHT_LEFT);	 m_Grid.AddCell(nRow, nCol+1, &pStd->m_pBasicConcInfo->m_UWeightCement, DT_RIGHT);		m_Grid.SetCellDec(nDot);   m_Grid.SetCellUnit(szKgf_m3_CGS,szKgf_m3_Apply);	m_Grid.AddCell(nRow++, nCol+2, szKgf_m3_Apply, DT_LEFT);
	m_Grid.AddCell(nRow, nCol,"  목재",GVHT_LEFT);			 m_Grid.AddCell(nRow, nCol+1, &pStd->m_pBasicConcInfo->m_UWeightLumber, DT_RIGHT);		m_Grid.SetCellDec(nDot);   m_Grid.SetCellUnit(szKgf_m3_CGS,szKgf_m3_Apply);	m_Grid.AddCell(nRow++, nCol+2, szKgf_m3_Apply, DT_LEFT);
	m_Grid.AddCell(nRow, nCol,"  역청재(방수용)",GVHT_LEFT);  m_Grid.AddCell(nRow, nCol+1, &pStd->m_pBasicConcInfo->m_UWeightInvChungje, DT_RIGHT);	m_Grid.SetCellDec(nDot);   m_Grid.SetCellUnit(szKgf_m3_CGS,szKgf_m3_Apply);	m_Grid.AddCell(nRow++, nCol+2, szKgf_m3_Apply, DT_LEFT);
	// 횡단구성의 포장에서 입력받고 있기 때문에 여기에서는 입력을 삭제하기로 협의함 2010.05.20 
	//m_Grid.AddCell(nRow, nCol,"  포장",GVHT_LEFT);			 m_Grid.AddCell(nRow, nCol+1, &pStd->m_pBasicConcInfo->m_UWeightAsphalt, DT_RIGHT);		m_Grid.SetCellDec(nDot);   m_Grid.SetCellUnit(szKgf_m3_CGS,szKgf_m3_Apply);	m_Grid.AddCell(nRow++, nCol+2, szKgf_m3_Apply, DT_LEFT);
	m_Grid.AddCell(nRow, nCol, m_bDesignLsd ? "  콘크리트" : "  무근 콘크리트",GVHT_LEFT);	 m_Grid.AddCell(nRow, nCol+1, &pStd->m_pBasicConcInfo->m_UWeightConcrete, DT_RIGHT);		m_Grid.SetCellDec(nDot);   m_Grid.SetCellUnit(szKgf_m3_CGS,szKgf_m3_Apply);	m_Grid.AddCell(nRow++, nCol+2, szKgf_m3_Apply, DT_LEFT);
	m_Grid.AddCell(nRow, nCol,"  지하수",GVHT_LEFT);  m_Grid.AddCell(nRow, nCol+1, &pStd->m_pBasicConcInfo->m_UWeightWater, DT_RIGHT);	m_Grid.SetCellDec(nDot);   m_Grid.SetCellUnit(szKgf_m3_CGS,szKgf_m3_Apply);	m_Grid.AddCell(nRow++, nCol+2, szKgf_m3_Apply, DT_LEFT);

	m_Grid.GetCellCount(nRows, nCols);
	m_Grid.SetRows(nRows);
	m_Grid.SetCols(nCols);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0,200);
	m_Grid.SetColumnWidth(1, 80);
	m_Grid.SetColumnWidth(2, 80);
}

void CInputBaseDesignBasicData::SetGridDataRebarFactorUsd()
{
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 1, 2);
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;

	long nRow = 0;
	long nRows = 0;
	long nCols = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;

	m_Grid.AddCell(nRow, 0,"구분");		
	m_Grid.AddCell(nRow, 1,"종류");		
	m_Grid.AddCell(nRow, 2,"계수");		
	

	nRow = 1;
	m_Grid.AddCell(nRow, 0,"α\n"); 
	m_Grid.AddCell(nRow+1, 0,"α\n"); 
	m_Grid.AddCell(nRow, 1,"상부철근"); m_Grid.AddCell(nRow, 2, &pStd->m_pBasicConcInfo->m_dAlpha_UpRebar); m_Grid.SetCellDec(CELL_DEC_2); 
	m_Grid.AddCell(nRow+1, 1,"일반철근"); m_Grid.AddCell(nRow+1, 2, &pStd->m_pBasicConcInfo->m_dAlpha_GenBar); m_Grid.SetCellDec(CELL_DEC_2); 
	nRow = 3;
	m_Grid.AddCell(nRow, 0,"β\n");
	m_Grid.AddCell(nRow+1, 0,"β\n");
	m_Grid.AddCell(nRow+2, 0,"β\n");
	m_Grid.AddCell(nRow, 1,"피복두께 3db미만, 순간격 6db미만 도막철근");	m_Grid.AddCell(nRow, 2, &pStd->m_pBasicConcInfo->m_dBeta_Tomak1);	m_Grid.SetCellDec(CELL_DEC_2); 
	m_Grid.AddCell(nRow+1, 1,"기타도막철근"); m_Grid.AddCell(nRow+1, 2, &pStd->m_pBasicConcInfo->m_dBeta_Tomak2); m_Grid.SetCellDec(CELL_DEC_2); 
	m_Grid.AddCell(nRow+2, 1,"도막되지 않은철근"); m_Grid.AddCell(nRow+2, 2, &pStd->m_pBasicConcInfo->m_dBeta_GenBar); m_Grid.SetCellDec(CELL_DEC_2); 
	nRow = 6;
	m_Grid.AddCell(nRow, 0,"r\n");
	m_Grid.AddCell(nRow+1, 0,"r\n");
	m_Grid.AddCell(nRow, 1,"D22이상의 철근"); m_Grid.AddCell(nRow, 2, pStd->m_nTypeUnit == UNIT_TYPE_SI ? "1.00" : "1.25", nFormat, CELL_READONLY); 
	m_Grid.AddCell(nRow+1, 1,"D19이하의 철근과 이형철선"); m_Grid.AddCell(nRow+1, 2, pStd->m_nTypeUnit == UNIT_TYPE_SI ? "0.80" : "1.00", nFormat, CELL_READONLY);
	nRow = 8;
	m_Grid.AddCell(nRow, 0,"λ\n");
	m_Grid.AddCell(nRow+1, 0,"λ\n");
	m_Grid.AddCell(nRow, 1,"경량콘크리트"); m_Grid.AddCell(nRow, 2, &pStd->m_pBasicConcInfo->m_dIpSmallConc); m_Grid.SetCellDec(CELL_DEC_2); 
	m_Grid.AddCell(nRow+1, 1,"일반콘크리트"); m_Grid.AddCell(nRow+1, 2, &pStd->m_pBasicConcInfo->m_dIpGenConc); m_Grid.SetCellDec(CELL_DEC_2); 


	m_Grid.GetCellCount(nRows, nCols);
	m_Grid.SetRows(nRows);
	m_Grid.SetCols(nCols);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(70);
	m_Grid.SetColumnWidth(1,300);
}

void CInputBaseDesignBasicData::SetGridData()
{
	m_Grid.SetType(0);
	
	if(m_nTab == m_nTabFactor)	SetGridDataFactor();
	if(m_nTab == m_nTabSettle)	SetGridDataSettle();
	if(m_nTab == m_nTabJoint)	SetGridDataJoint();
	if(m_nTab == m_nTabWeight)	SetGridDataWeight();
	if(m_nTab == m_nTabUWeight)	SetGridDataUWeight();
	if(m_nTab == m_nTabRebarFactor)	SetGridDataRebarFactor();
	if(m_nTab == m_nTabRailLoad)	SetGridDataRailLoad();
	if(m_nTab == m_nTabTankLoad)	SetEditDataTankLoad();
	if(m_nTab == m_nTabResistFactor)	SetGridDataResistFactor();
	if(m_nTab == m_nTabFootResistFactor)	SetGridDataFootResistFactor();
}

void CInputBaseDesignBasicData::OnCellChanged(NMHDR *pNotifyStruct, LRESULT *result)
{
}

void CInputBaseDesignBasicData::OnCellChangedData(NMHDR *pNotifyStruct, LRESULT *result)
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;

	CCellID cell = m_Grid.GetFocusCell();
	CString str = m_Grid.GetTextMatrix(cell.row, 1);

	//////////////////////////////////////////////////////////////////////////
	if( m_nTab == m_nTabWeight || m_nTab == m_nTabUWeight )
	{
		str = m_Grid.GetTextMatrix(cell.row, cell.col);
		// 0, 음수, 문자열 입력 불가.

		HGDOUBLE dVal = atof(str); 
		if( 0 < dVal )
		{
			SetDataSave();
		}
		
		SetGridData();
		SetDataInit();
		return;
	}
	//////////////////////////////////////////////////////////////////////////

	SetDataSave();

	if(m_nTab == m_nTabFactor)
	{
		if(str.Find("fck : 콘크리트 강도")!=-1 || str.Find("무근 콘크리트")!=-1)
		{
			double Wc = (pStd->m_pBasicConcInfo->m_UWeightConcrete>0) ? pStd->m_pBasicConcInfo->m_UWeightConcrete/1000 : 2.350;
			pStd->m_Ec = GetValueEcConc(pStd->m_Fck,Wc,pStd->IsSI(),pStd->m_nTypeKciCode); // 도로교설계기준 38페이지
			SetDataInit();
		}
		if(str.Find(_T("강관말뚝")) != -1 && str.Find(_T("검토기준")) != -1)
		{
			pStd->SetDataDefaultBaseResistFactor_SteelPile();
		}
		if(str.Find("fy  : 다웰바 강종") != -1 || str.Find("내진해석적용") != -1)
		{
			SetGridData();
			SetDataInit();
		}
		if( str.Find("도로교 설계기준") != -1)
		{
			pStd->m_nTypeKciCode = DESIGN_CODE_KCI_2007;
			if(pStd->m_nTypeUnit == UNIT_TYPE_CGS && pStd->m_nTypeRoadCode == DESIGN_ROAD_2010)
			{
				pStd->m_nTypeUnit = UNIT_TYPE_SI;
				pStd->SetStringUnitType();
				pStd->m_pBasicConcInfo->SetUnit(pStd->IsSI());
				SetDataDefaultRebarFactor();
				SetDataDefaultSettle();
				SetDataDefaultJoint();
				SetDataDefaultUWeight();
				AfxGetMainWnd()->SendMessage(WM_USER+14); // 단위계 상태바
			}
			SetGridData();
			SetDataInit();
		}
		if(str.Find("적용단위계")!=-1) 		
		{
			pStd->SetStringUnitType();
			SetGridData();
			SetDataInit();
			pStd->m_pBasicConcInfo->SetUnit(pStd->IsSI());
			SetDataDefaultRebarFactor();
			SetDataDefaultSettle();
			SetDataDefaultJoint();
		}

		if(str.Find("열차하중의 종류")!=-1) 		
		{
			pStd->SetRailLoad();
		}
		if(str.Find("적용설계기준(하중조합)")!=-1)
		{
			pStd->SetRailLoad();
			for(long nBri=0; nBri<pStd->GetBridgeSize(); nBri++)
			{
				CRcBridgeRebar *pBri = pStd->GetBridgeRebar(nBri);
				pBri->SetDataDefaultSelectLoad();
				pBri->SetDataDefaultCombineEccLoad();
				pBri->SetDataDefaultCombineUltLoad();
				pBri->SetDataDefaultCombineUseLoad();
				pBri->SetDataDefaultCombineEQLoad();
				pBri->SetDataDefaultEnv();
				m_BridgeLevelDlg.m_nTypeUnit = pStd->m_nTypeUnit;
			}

			SetDataDefaultJoint();
			SetDataDefaultSettle();

			if(pStd->m_nTypeConditionApply == DESIGN_CONDITION_ROAD && pStd->m_nTypeRoadCode == DESIGN_ROAD_2010)
			{
				pStd->m_nTypeKciCode = DESIGN_CODE_KCI_2007;
			}
			else if(pStd->m_nTypeConditionApply == DESIGN_CONDITION_RAIL)
			{
				pStd->m_nTypeRailCode = DESIGN_RAIL_2017;
				pStd->m_nTypeKciCode = DESIGN_CODE_KCI_2012;
			}

			SetGridData();
			SetDataInit();
		}


		if(str.Find("콘크리트 구조설계기준")!=-1 || str.Find("도로교 설계기준")!=-1 || str.Find("적용설계기준")!=-1)
		{
			if(str.Find("콘크리트 구조설계기준")!=-1)
			{
				if(pStd->m_nTypeConditionApply == DESIGN_CONDITION_ROAD && pStd->m_nTypeRoadCode == DESIGN_ROAD_2010 && pStd->m_nTypeKciCode == DESIGN_CODE_KCI_2003)
				{
					// (ARCBRIDGE-1857) 2010기준 선택시 KCI2003은 선택 안되게
					AfxMessageBox(_T("[KCI2003]\n지원되지않는 설계기준입니다."), MB_OK, MB_ICONINFORMATION);
					pStd->m_nTypeKciCode = DESIGN_CODE_KCI_2007;
				}
			}

			if(pStd->m_nTypeRoadCode == DESIGN_ROAD_KDS_2019)
			{
				pStd->m_nTypeEarthQuakeApply = 1;
			}

			SetDataDefaultUWeight();
			SetDataDefaultFactor_Deduct();

			double Wc = (pStd->m_pBasicConcInfo->m_UWeightConcrete>0) ? pStd->m_pBasicConcInfo->m_UWeightConcrete/1000 : 2.350;
			pStd->m_Ec = GetValueEcConc(pStd->m_Fck,Wc,pStd->IsSI(), pStd->m_nTypeKciCode); // 도로교설계기준 38페이지

			BOOL bKds2019 = m_pIn->m_pARcBridgeDataStd->IsDesignKDS2019();
			m_pLsdManager->SetDesignCondition(bKds2019 ? LSD_DESIGN_CONDITION_KDS_2019 : LSD_DESIGN_CONDITION_LSD_2015);
			for(long nBri=0; nBri<pStd->GetBridgeSize(); nBri++)
			{
				CRcBridgeRebar *pBri = pStd->GetBridgeRebar(nBri);
				pBri->SyncBridge_DesignData();
			}

			SetGridData(); 
			SetDataInit();
		}
		if(str.Find(_T("철도")) != -1 && str.Find(_T("설계기준")) != -1)
		{
			if(pStd->IsRailDesign2017())
			{				
				pStd->m_nTypeRailLoad = 0;
				pStd->m_nTypeKciCode = DESIGN_CODE_KCI_2012;

				if(pStd->IsRailDesignKDS2018())
				{
					pStd->m_nTypeEarthQuakeApply = 1;
				}
			}
			else
			{
				pStd->m_nTypeKciCode = MIN(pStd->m_nTypeKciCode, DESIGN_CODE_KCI_2007);
			}

			SetDataDefaultUWeight();
			SetDataDefaultFactor_Deduct();

			pStd->SetRailLoad();

			SetGridData(); 
			SetDataInit();
		}
		if(str.Find("슬래브 표준도", 0) != -1)
		{
			SetGridData();
			SetDataInit();
		}
		if(str.Find("열차하중의 종류")!=-1) 		
		{
			pStd->SetRailLoad();
		}
		if(str.Find("설계방법")!=-1) 		
		{
			BOOL bContinue = OnChangeMethod();
			m_bDesignLsd = pStd->IsLsdDesign();
			if(bContinue)
			{
				pStd->m_nTypeKciCode = (m_bDesignLsd || pStd->IsRailDesign2017() || pStd->m_nTypeConditionApply == DESIGN_CONDITION_CONCRETE) ? DESIGN_CODE_KCI_2012 : DESIGN_CODE_KCI_2007;
				
				pStd->SetChangeDesignMethod();
				SetDataDefaultUWeight();

				InitTabCtrl();
				SetGridData();
				SetDataInit();

				long nSizeBri = pStd->GetBridgeSize();
				for(long ixBri = 0; ixBri < nSizeBri; ++ixBri)
				{
					CRcBridgeApp *pBri	= pStd->GetBridge(ixBri);
					pBri->m_mStmSolver.clear();
				}
			}
			else
			{
				SetDataInit();
			}
		}
		if (str.Find("설계차로의 폭(Wp)")!=-1)
		{
			pStd->m_dDesignRoadWidth = Round(pStd->m_dDesignRoadWidth, 0);
			if (pStd->m_dDesignRoadWidth <= 0)
			{
				pStd->m_dDesignRoadWidth = 1;
			}
			else if (pStd->m_dDesignRoadWidth > 3600)
			{
				pStd->m_dDesignRoadWidth = 3600;
			}

			SetGridData();
			SetDataInit();
		}
		if(str.Find("내진설계기준")!=-1)
		{
			m_EarhtquakeDlg.SetKDS(pStd->m_nTypeEarthQuakeApply==1);
			if(pStd->m_nTypeEarthQuakeApply == 0)
			{
				long nSizeBri = pStd->GetBridgeSize();
				for(long ixBri = 0; ixBri < nSizeBri; ++ixBri)
				{
					CRcBridgeApp *pBri	= pStd->GetBridge(ixBri);
					if(!pBri) continue;		

					if(pBri->GetDegreeEarthQuake() > 1)
						pBri->SetDegreeEarthQuake(0);
				}
			}
			SetGridData();
			SetDataInit();
		}
	}
	else if(m_nTab == m_nTabSettle || m_nTab == m_nTabJoint)
	{
		SetGridData();
		SetDataInit();
	}
	else if(m_nTab == m_nTabRebarFactor) //철근정착, 이음 보정계수
	{
		if(cell.row > 0 && cell.row < 7 && cell.col== 2)
		{
			long nSizeBri = pStd->GetBridgeSize();
			for(long ixBri = 0; ixBri < nSizeBri; ++ixBri)
			{
				CRcBridgeApp *pBri	= pStd->GetBridge(ixBri);
				if(!pBri) continue;		

				pBri->m_plsdRebar->GetOption().dn1[0] = pStd->m_plsdRebar->GetOption().dn1[0];
				pBri->m_plsdRebar->GetOption().dn1[1] = pStd->m_plsdRebar->GetOption().dn1[1];
				pBri->m_plsdRebar->GetOption().dn2    = pStd->m_plsdRebar->GetOption().dn2;
				pBri->m_plsdRebar->GetOption().dRestraintStress	= pStd->m_plsdRebar->GetOption().dRestraintStress;
				pBri->m_plsdRebar->GetOption().dOverRatio		= pStd->m_plsdRebar->GetOption().dOverRatio;
				pBri->m_plsdRebar->GetOption().dAsRatio		= pStd->m_plsdRebar->GetOption().dAsRatio;
			}
		}
	}
	else if (m_nTab == m_nTabResistFactor)
	{
		SetDataInit();
	}
}


void CInputBaseDesignBasicData::SetDataDefaultRailLoad()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;

	pStd->SetRailLoad();
}

void CInputBaseDesignBasicData::SetGridDataRailLoad()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;

	if(!pStd->IsRailDesign())
	{
		m_Grid.SetGridDefault(1,1,1,1, TRUE, TRUE);
		m_Grid.SetColumnWidthAll(500);
		m_Grid.SetRowHeightAll(100);
		m_Grid.SetTextMatrix(0,0,"철도교 설계기준이 적용되지 않았습니다.");
		return;
	}

	long nCountRailLoadWheel = pStd->GetCountRailLoadWheel();
	long nSizeRow = 4;
	long nSizeCol = 1 + nCountRailLoadWheel;

	// 그리드 초기화
	m_Grid.SetGridDefault(nSizeRow, nSizeCol, 1, 1);
	m_Grid.SetColumnWidthAll(55);
	m_Grid.SetColumnWidth(0,80);

	long nRow = 0;
	long nCol = 0;
	// TITLE
	m_Grid.AddCell(nRow++,nCol,"구분");
	m_Grid.AddCell(nRow++,nCol,"하중종류");
	m_Grid.AddCell(nRow++,nCol,"하중크기");
	m_Grid.AddCell(nRow++,nCol,"거리(m)");

	CStringArray sArr;
	CString str = _T("");
			
	sArr.Add("Pd");
	sArr.Add("P");
	sArr.Add("W");

	BOOL bUser = pStd->IsRailDesign2017() ? (pStd->m_nTypeRailLoad == RAIL_LOAD_USER_2017) : (pStd->m_nTypeRailLoad == RAIL_LOAD_USER);
	UINT CellLine = bUser ? CELL_LINE_NO : CELL_READONLY;
	for(nCol=1; nCol<nSizeCol; nCol++)
	{
		nRow=0;
		str.Format("L%d", nCol);
		m_Grid.AddCell(nRow++,nCol,str);
		m_Grid.AddCell(nRow++,nCol, &pStd->m_dArrTypeLoadTrainWheel, nCol-1, DT_CENTER, CellLine, "", CELL_TYPE_COMBO, sArr); m_Grid.SetCellDec(CELL_DEC_3);
		m_Grid.AddCell(nRow++,nCol, &pStd->m_dArrLoadTrainWheel, nCol-1, DT_CENTER,CellLine); m_Grid.SetCellUnit(UNIT_CGS_TONF,pStd->m_szTonf); m_Grid.SetCellDec(CELL_DEC_3); 
		m_Grid.AddCell(nRow++,nCol, &pStd->m_dArrDistTrainWheel, nCol-1, DT_CENTER,CellLine); m_Grid.SetCellDec(CELL_DEC_3); 
	}
}

void CInputBaseDesignBasicData::DrawRailLoadInput(CDomyun *pDomP)
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	if(!pStd->IsRailDesign()) return;

	CDomyun Dom(pDomP);
	CDomyun DomText(pDomP);
	Dom.SetScaleDim(0.2);	
	DomText.SetScaleDim(0.2);
	
	double dBRadius = 0.8;
	double dSRadius = 0.6;
	long nCountRailLoad	= pStd->GetCountRailLoadWheel();
	long	nType		= 0,
			nTypeTmp	= 0;
	double	dLoad		= 0,
			dLoadTmp	= 0;
	double	dDist		= 0,
			dDistTmp	= 0;
	double dDistTot		= 0;
	double dExtLen		= 4;	// 하중들어가는 선 길이..
	CString str		= _T("");
	CString strUnit = _T("");

	m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, MAIN_MEMBER);
	m_pIn->m_pARoadOptionStd->SetEnvType(&DomText, HCAD_DIML);
	long i = 0; for(i = 0; i < nCountRailLoad; i++)
	{
		pStd->GetRailLoadWheelInfo(i, nType, dLoad, dDist);
		pStd->GetRailLoadWheelInfo(i-1, nTypeTmp, dLoadTmp, dDistTmp);
		dDistTot += (i == 0) ? 0 : dDistTmp;
			
		double dWidth = i==0 ? -6 : 6;
		
		strUnit = pStd->GetStringUnitType("tonf");
		dLoad = GetValueUnitChange(dLoad,UNIT_CGS_TONF,strUnit);
		dLoadTmp = GetValueUnitChange(dLoadTmp,UNIT_CGS_TONF,strUnit);
		str.Format("%.3f %s", dLoad,strUnit);
		if(nType == 0)	// 동륜하중 (Pd)
		{			
			Dom.Circle(dDistTot, dBRadius, dBRadius);
			Dom.LineTo(dDistTot, 0, dDistTot, dSRadius*2+dExtLen);
		}
		else if(nType == 1)	// 륜하중(P)
		{
			Dom.Circle(dDistTot, dSRadius, dSRadius);
			Dom.LineTo(dDistTot, 0, dDistTot, dSRadius*2+dExtLen);
		}
		else if(nType == 2)	// 분포하중(W)
		{
			Dom.Rectangle(dDistTot, 0, dDistTot+dWidth, dSRadius);
		}		
		if(nType==2)
		{
			strUnit = pStd->GetStringUnitType("tonf/m");
			DomText.SetTextAlignHorz(TA_CENTER);
			str.Format("%.3f %s", dLoad,strUnit);
			DomText.TextOut(dDistTot+dWidth/2, dSRadius+0.1, str);
		}
		else
		{
			DomText.SetTextAlignHorz(TA_LEFT);
			DomText.TextOut(dDistTot-0.5, dSRadius*2+1+0.1, str);
			DomText.Rotate(CDPoint(dDistTot, dSRadius*2+1), CDPoint(0, 1));
		}
		DomText.RedrawByScale(100);		
		*pDomP << DomText;
	}
	Dom.LineTo(0, 0, dDistTot, 0);
	//
	m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	long nDimDan = 0;
	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(0, 0, nDimDan);

	for(i = 0; i < nCountRailLoad-1; i++)
	{
		pStd->GetRailLoadWheelInfo(i, nType, dLoad, dDist);
		str.Format("%.3f", dDist);
		Dom.DimLineTo(dDist, nDimDan, str);
	}

	if(pStd->IsRailDesign2017() && (pStd->m_nTypeRailLoad <= RAIL_LOAD_KRL2012_P))
	{
		m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_PAPR);
		double dRectHeight = dExtLen/2;
		double dStt = -dDistTot*3;
		double dEnd = dDistTot*4;
		double dGap = (dEnd-dStt)/8;

		Dom.Rectangle(dStt, 0, dEnd, dRectHeight);
		for(long ix = 0; ix <= 8; ++ix)
		{
			Dom.GiSiArrowExt(dStt+dGap*ix,0,dRectHeight,0,2,_T(""));
		}

		DomText.SetTextAlignHorz(TA_RIGHT);
		str.Format("%.1f %s", GetValueUnitChange(pStd->m_DistributionLoadFront,UNIT_CGS_TONF_M,pStd->m_szTonf_M),pStd->m_szTonf_M);
		DomText.TextOut(dEnd, dRectHeight+0.1, str);
		DomText.RedrawByScale(100);	
		*pDomP << DomText;
	}

	// 그림 그린후 가운데 제목 씀.
	Dom.SetCalcExtRect();
	CDRect rect = Dom.GetExtRect();
	m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, MIDDLE_TITLE);
	str.Format("%s", pStd->GetStringRailLoad(pStd->m_nTypeRailLoad)); 
	Dom.SetTextAlignHorz(TA_CENTER);
	Dom.TextOut(rect.left+rect.Width()/2, 8, str);
	Dom.RedrawByScale(100);		
	
	*pDomP << Dom;
}

void CInputBaseDesignBasicData::OnSelchangeComboRailCar() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	if(!pStd->IsRailDesign()) return;

	pStd->m_nRailQtyCar = m_ComboRailCar.GetCurSel() + 1;
	pStd->SetRailLoad();

	SetGridData();
	SetDataInit();
	DrawInputDomyunView();	
}

void CInputBaseDesignBasicData::OnButtonAddLoad() 
{
	// TODO: Add your control notification handler code here
	if(m_Tab.GetCurSel()!=m_nTabRailLoad) return;

	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	if(!pStd->IsRailDesign()) return;

	pStd->m_dArrDistTrainWheel.Add(0);
	pStd->m_dArrLoadTrainWheel.Add(0);
	pStd->m_dArrTypeLoadTrainWheel.Add(0);

	SetGridData();
	SetDataInit();
	DrawInputDomyunView();	
}

void CInputBaseDesignBasicData::OnButtonRemoveLoad() 
{
	// TODO: Add your control notification handler code here
	if(m_Tab.GetCurSel()!=m_nTabRailLoad) return;

	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	if(!pStd->IsRailDesign()) return;

	long nIndexLoad = m_Grid.GetFocusCell().col-1;
	if(nIndexLoad<0 || nIndexLoad>=pStd->m_dArrDistTrainWheel.GetSize()) return;

	pStd->m_dArrDistTrainWheel.RemoveAt(nIndexLoad);
	pStd->m_dArrLoadTrainWheel.RemoveAt(nIndexLoad);
	pStd->m_dArrTypeLoadTrainWheel.RemoveAt(nIndexLoad);

	SetGridData();
	SetDataInit();	
	DrawInputDomyunView();	
}

void CInputBaseDesignBasicData::ShowControlTankLoad(BOOL bTank)
{
	m_Grid.ShowWindow(bTank? SW_HIDE : SW_SHOW);
	m_ctlEdit_Trailer_WidTot.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlEdit_Trailer_WidSide.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlEdit_Trailer_WidInner.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlEdit_Trailer_P3.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlEdit_Trailer_P2.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlEdit_Trailer_P1.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlEdit_Trailer_L4.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlEdit_Trailer_L3.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlEdit_Trailer_L2.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlEdit_Trailer_L1.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlEdit_Tank_WidTot.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlEdit_Tank_WidSide.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlEdit_Tank_WidInner.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlEdit_Tank_Weight.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlEdit_Tank_Len.ShowWindow(bTank? SW_SHOW : SW_HIDE);

	m_ctlStatic_Tank5.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlStatic_Tank4.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlStatic_Tank3.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlStatic_Tank2.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlStatic_Tank1.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlStatic_Trailer7.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlStatic_Trailer6.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlStatic_Trailer5.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlStatic_Trailer4.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlStatic_Trailer3.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlStatic_Trailer2.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlStatic_Trailer1.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	
	m_ctlBM_Trailer.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlBM_Tank.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlGB_Trailer.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	m_ctlGB_Tank.ShowWindow(bTank? SW_SHOW : SW_HIDE);
	
	bTabTankLoad = bTank;
}

void CInputBaseDesignBasicData::SetDataDefaultTankLoad()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;

	// 탱크 및 중차량 하중
	pStd->m_dTank_i				= 0.15;				
	pStd->m_dTank_WeightTank		= 54.42;
	pStd->m_dTank_TankLength		= 4.500;		
	pStd->m_dTank_TankWidth_Side	= 0.700;
	pStd->m_dTank_TankWidth_Inner	= 1.900;
	
	pStd->m_dTank_TrailerL1		= 3.6;		
	pStd->m_dTank_TrailerL2		= 1.5;		
	pStd->m_dTank_TrailerL3		= 4.5;		
	pStd->m_dTank_TrailerL4		= 1.2;
	pStd->m_dTank_TrailerWidth_Side	= 0.975;
	pStd->m_dTank_TrailerWidth_Inner	= 1.250;
	pStd->m_dTank_Ptrailer1			= 4.0815;
	pStd->m_dTank_Ptrailer2			= 8.163;
	pStd->m_dTank_Ptrailer3			= 5.8955;

	
}

void CInputBaseDesignBasicData::SetEditDataTankLoad()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;

	CString str =_T("");
	str.Format("%.3f", pStd->m_dTank_TrailerWidth_Side + pStd->m_dTank_TrailerWidth_Inner);
	m_ctlEdit_Trailer_WidTot.SetWindowText(str);
	str.Format("%.3f", pStd->m_dTank_TrailerWidth_Side);
	m_ctlEdit_Trailer_WidSide.SetWindowText(str);
	str.Format("%.3f", pStd->m_dTank_TrailerWidth_Inner);
	m_ctlEdit_Trailer_WidInner.SetWindowText(str);
	str.Format("%.3f", GetValueUnitChange(pStd->m_dTank_Ptrailer3, UNIT_CGS_TONF, pStd->m_szTonf));
	m_ctlEdit_Trailer_P3.SetWindowText(str);
	str.Format("%.3f", GetValueUnitChange(pStd->m_dTank_Ptrailer2, UNIT_CGS_TONF, pStd->m_szTonf));
	m_ctlEdit_Trailer_P2.SetWindowText(str);
	str.Format("%.3f", GetValueUnitChange(pStd->m_dTank_Ptrailer1, UNIT_CGS_TONF, pStd->m_szTonf));
	m_ctlEdit_Trailer_P1.SetWindowText(str);

	str.Format("%.3f", pStd->m_dTank_TrailerL4);
	m_ctlEdit_Trailer_L4.SetWindowText(str);
	str.Format("%.3f", pStd->m_dTank_TrailerL3);
	m_ctlEdit_Trailer_L3.SetWindowText(str);
	str.Format("%.3f", pStd->m_dTank_TrailerL2);
	m_ctlEdit_Trailer_L2.SetWindowText(str);
	str.Format("%.3f", pStd->m_dTank_TrailerL1);
	m_ctlEdit_Trailer_L1.SetWindowText(str);

	str.Format("%.3f", pStd->m_dTank_TankWidth_Side + pStd->m_dTank_TankWidth_Inner);
	m_ctlEdit_Tank_WidTot.SetWindowText(str);
	str.Format("%.3f", pStd->m_dTank_TankWidth_Side);
	m_ctlEdit_Tank_WidSide.SetWindowText(str);
	str.Format("%.3f", pStd->m_dTank_TankWidth_Inner);
	m_ctlEdit_Tank_WidInner.SetWindowText(str);

	str.Format("%.3f", GetValueUnitChange(pStd->m_dTank_WeightTank, UNIT_CGS_TONF, pStd->m_szTonf));
	m_ctlEdit_Tank_Weight.SetWindowText(str);
	str.Format("%.3f", pStd->m_dTank_TankLength);
	m_ctlEdit_Tank_Len.SetWindowText(str);

	BOOL bSI = pStd->IsSI();
	m_ctlStatic_Tank3.SetWindowText(bSI? "KN" : "tonf");
	m_ctlStatic_Trailer3.SetWindowText(bSI? "KN(편측차륜하중)" : "tonf(편측차륜하중)");
	m_ctlStatic_Trailer4.SetWindowText(bSI? "KN(편측차륜하중)" : "tonf(편측차륜하중)");
	m_ctlStatic_Trailer5.SetWindowText(bSI? "KN(편측차륜하중)" : "tonf(편측차륜하중)");
}

void CInputBaseDesignBasicData::SetDataSaveTankLoad()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CString str = _T("");

	UpdateData(FALSE);
	m_ctlEdit_Tank_Weight.GetWindowText(str);
	pStd->m_dTank_WeightTank		= GetValueUnitChange(atof(str), pStd->m_szTonf, UNIT_CGS_TONF);
	m_ctlEdit_Tank_Len.GetWindowText(str);
	pStd->m_dTank_TankLength		= atof(str);		
	m_ctlEdit_Tank_WidSide.GetWindowText(str);
	pStd->m_dTank_TankWidth_Side	= atof(str);
	m_ctlEdit_Tank_WidInner.GetWindowText(str);
	pStd->m_dTank_TankWidth_Inner	= atof(str);
	
	m_ctlEdit_Trailer_L1.GetWindowText(str);
	pStd->m_dTank_TrailerL1		= atof(str);
	m_ctlEdit_Trailer_L2.GetWindowText(str);
	pStd->m_dTank_TrailerL2		= atof(str);
	m_ctlEdit_Trailer_L3.GetWindowText(str);
	pStd->m_dTank_TrailerL3		= atof(str);
	m_ctlEdit_Trailer_L4.GetWindowText(str);
	pStd->m_dTank_TrailerL4		= atof(str);
	m_ctlEdit_Trailer_WidSide.GetWindowText(str);
	pStd->m_dTank_TrailerWidth_Side		= atof(str);
	m_ctlEdit_Trailer_WidInner.GetWindowText(str);
	pStd->m_dTank_TrailerWidth_Inner	= atof(str);

	m_ctlEdit_Trailer_P1.GetWindowText(str);
	pStd->m_dTank_Ptrailer1			= GetValueUnitChange(atof(str), pStd->m_szTonf, UNIT_CGS_TONF);
	m_ctlEdit_Trailer_P2.GetWindowText(str);
	pStd->m_dTank_Ptrailer2			= GetValueUnitChange(atof(str), pStd->m_szTonf, UNIT_CGS_TONF);
	m_ctlEdit_Trailer_P3.GetWindowText(str);
	pStd->m_dTank_Ptrailer3			= GetValueUnitChange(atof(str), pStd->m_szTonf, UNIT_CGS_TONF);

	SetEditDataTankLoad();
}

void CInputBaseDesignBasicData::ShowControlRailLoad()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	if(!pStd->IsRailDesign())
	{
		GetDlgItem(IDC_EDIT_LOAD1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_LOAD2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_LOAD3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_LOAD4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_LOAD5)->ShowWindow(SW_HIDE);
		
		GetDlgItem(IDC_STATIC_LOAD1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LOAD2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LOAD3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LOAD4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_LOAD5)->ShowWindow(SW_HIDE);
		return;
	}

	int iShow = m_nTab == m_nTabRailLoad ? SW_SHOW : SW_HIDE;
	
	GetDlgItem(IDC_EDIT_LOAD1)->ShowWindow(iShow);
	GetDlgItem(IDC_EDIT_LOAD2)->ShowWindow(iShow);
	GetDlgItem(IDC_EDIT_LOAD3)->ShowWindow(iShow);
	GetDlgItem(IDC_EDIT_LOAD4)->ShowWindow(iShow);
	GetDlgItem(IDC_EDIT_LOAD5)->ShowWindow(iShow);

	GetDlgItem(IDC_STATIC_LOAD1)->ShowWindow(iShow);
	GetDlgItem(IDC_STATIC_LOAD2)->ShowWindow(iShow);
	GetDlgItem(IDC_STATIC_LOAD3)->ShowWindow(iShow);
	GetDlgItem(IDC_STATIC_LOAD4)->ShowWindow(iShow);
	GetDlgItem(IDC_STATIC_LOAD5)->ShowWindow(iShow);

	int iEnable	= pStd->IsRailDesign2017() ? (pStd->m_nTypeRailLoad == RAIL_LOAD_USER_2017) : (pStd->m_nTypeRailLoad == RAIL_LOAD_USER);
	GetDlgItem(IDC_EDIT_LOAD1)->EnableWindow(iEnable);
	GetDlgItem(IDC_EDIT_LOAD2)->EnableWindow(iEnable);
	GetDlgItem(IDC_EDIT_LOAD3)->EnableWindow(iEnable);
	GetDlgItem(IDC_EDIT_LOAD4)->EnableWindow(pStd->IsRailDesign());	// 10531
	GetDlgItem(IDC_EDIT_LOAD5)->EnableWindow(iEnable);

	GetDlgItem(IDC_STATIC_LOAD1)->EnableWindow(iEnable);
	GetDlgItem(IDC_STATIC_LOAD2)->EnableWindow(iEnable);
	GetDlgItem(IDC_STATIC_LOAD3)->EnableWindow(iEnable);
	GetDlgItem(IDC_STATIC_LOAD4)->EnableWindow(pStd->IsRailDesign());	// 10531
	GetDlgItem(IDC_STATIC_LOAD5)->EnableWindow(iEnable);

	// 그리드의 크기를 좀 줄여야 된다.
	long nWidth	= 150;
	if(m_nTab == m_nTabRailLoad)
	{
		CRect rect;
		m_Grid.GetWindowRect(rect);
		rect.right -= nWidth;
		m_Grid.SetWindowPos(&wndTop, rect.left, rect.bottom, rect.Width(), rect.Height(), SWP_NOMOVE);
		m_bBecomeSmallGrid	= TRUE;
	}
	else if(m_bBecomeSmallGrid)
	{
		CRect rect;
		m_Grid.GetWindowRect(rect);
		rect.right += nWidth;
		m_Grid.SetWindowPos(&wndTop, rect.left, rect.bottom, rect.Width(), rect.Height(), SWP_NOMOVE);
		m_bBecomeSmallGrid	= FALSE;
	}
}

void CInputBaseDesignBasicData::SetDataSaveRailLoadUser()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	if(!pStd->IsRailDesign()) return;

	BOOL bUser = pStd->IsRailDesign2017() ? (pStd->m_nTypeRailLoad == RAIL_LOAD_USER_2017) : (pStd->m_nTypeRailLoad == RAIL_LOAD_USER);
	CString str	= _T("");

	if(bUser)
	{
		m_editDistributionLoadFront.GetWindowText(str);
		pStd->m_DistributionLoadFront	= GetValueUnitChange(atof(str), pStd->m_szTonf, UNIT_CGS_TONF);		// 열차앞쪽 분포하중

		m_editDistributionLoadFront.GetWindowText(str);
		pStd->m_DistributionLoadBack	= GetValueUnitChange(atof(str), pStd->m_szTonf, UNIT_CGS_TONF);		// 열차뒤쪽 분포하중

		m_editRailLoadWheel.GetWindowText(str);
		pStd->m_RailLoadWheel			= GetValueUnitChange(atof(str), pStd->m_szTonf, UNIT_CGS_TONF);				// 동륜하중

		m_editRailDistWheel.GetWindowText(str);
		pStd->m_RailDistWheel			= atof(str);				// 축거

		m_editRailDistWheelMid.GetWindowText(str);
		pStd->m_RailDistWheelMid		= atof(str);			// 축거 (중간부분)
	}
	else
	{
		m_editRailDistWheel.GetWindowText(str);
		pStd->m_RailDistWheel			= atof(str);				// 축거
	}
}

void CInputBaseDesignBasicData::SetDataInitRailLoadUser()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	if(!pStd->IsRailDesign()) return;

	CString str	= _T("");
	str.Format("%.3f", GetValueUnitChange(pStd->m_DistributionLoadFront, UNIT_CGS_TONF, pStd->m_szTonf));
	m_editDistributionLoadFront.SetWindowText(str);								// 열차앞쪽 분포하중
	
	str.Format("%.3f", GetValueUnitChange(pStd->m_DistributionLoadBack, UNIT_CGS_TONF, pStd->m_szTonf));
	m_editDistributionLoadBack.SetWindowText(str);								// 열차뒤쪽 분포하중

	str.Format("%.3f", GetValueUnitChange(pStd->m_RailLoadWheel, UNIT_CGS_TONF, pStd->m_szTonf));
	m_editRailLoadWheel.SetWindowText(str);										// 동륜하중

	str.Format("%.3f", pStd->m_RailDistWheel);
	m_editRailDistWheel.SetWindowText(str);										// 축거

	str.Format("%.3f", pStd->m_RailDistWheelMid);
	m_editRailDistWheelMid.SetWindowText(str);									// 축거 (중간부분)

	UpdateData(FALSE);
}

// 기초 자료 열기 
void CInputBaseDesignBasicData::OnButtonLoad() 
{
	// TODO: Add your control notification handler code here
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;

	CString sTitle	= _T("기초자료 불러오기");
	CFileDialog dlg(TRUE, "*.bas",NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
					  "ARcBridge 기초자료 파일 (*.bas)|*.bas||",GetParent());
	dlg.m_ofn.lpstrTitle = sTitle;	
	if (dlg.DoModal() == IDOK)
	{		
		CFile f;
		if(f.Open(dlg.GetPathName(),CFile::modeRead)==FALSE) return;
		CArchive ar(&f,CArchive::load);
		pStd->SerializeBase(ar);
		ar.Close();
		f.Close();

		SetGridData();
		SetDataInit();
	}						
}

// 기초 자료 저장
void CInputBaseDesignBasicData::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;

	CString sTitle	= _T("기초자료 저장");
	CFileDialog dlg(FALSE, "*.bas",NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
								  "ARcBridge 기초자료 파일 (*.bas)|*.bas||",GetParent());
	dlg.m_ofn.lpstrTitle = sTitle;
	if (dlg.DoModal() == IDOK)
	{
		SetDataSave();
		CFile f;
		if(f.Open(dlg.GetPathName(),CFile::modeCreate|CFile::modeWrite)==FALSE) return;
		CArchive ar(&f,CArchive::store);

		pStd->SerializeBase(ar);
		ar.Close();
		f.Close();		
	}		
}

void CInputBaseDesignBasicData::OnSize(UINT nType, int cx, int cy) 
{
	CInputDialogBase::OnSize(nType, cx, cy);
	
	// OnSzie에 InitDialog 하기전과 후에 두번 들어온다. InitDialog전에 들어오면 컨트롤에서 에러가 발생한다.
	// 어짜피 OnSzie에 InitDialog후에 들어올때는 m_nTab은 0이 되기 때문에 필요 없는 부분이다.
	// TODO: Add your message handler code here
//	if(m_nTab == m_nTabRailLoad)
//	{
//		ShowControlRailLoad();
//		SetGridData();
//		SetDataInit();
//	}
}

void CInputBaseDesignBasicData::SetDataDefaultMatResistFactor()
{
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;
	if(!pStd) return;

	pStd->SetDataDefaultMatResistFactor();
}

void CInputBaseDesignBasicData::SetDataDefaultBaseResistFactor()
{
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;
	if(!pStd) return;

	pStd->SetDataDefaultBaseResistFactor();
}

void CInputBaseDesignBasicData::SetGridDataResistFactor()
{
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;
	if(!pStd) return;

	m_Grid.SetLsdEnviromentCoef(pStd->m_plsdEnv);
	m_Grid.SetGridInitResistCoefMaterial();

	return;
}

void CInputBaseDesignBasicData::SetGridDataFootResistFactor()
{
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;
	if(!pStd) return;

	CLsdResistCoefBaseCtl *pResistCoefCtl = &(pStd->m_plsdEnv->GetLsdResistCoefBaseCtl());
	if(!pResistCoefCtl) return;
	pResistCoefCtl->SetUsed(eRESISTCOEF_ShearFracture, FALSE);

	m_Grid.SetLsdEnviromentCoef(pStd->m_plsdEnv);	
	m_Grid.SetGridInitResistCoefBase();

	return;
}


/**
*@brief
	저장되어 있는 파일에서 한계상태를 적용할 수 없는 파일들을 찾는 함수
*@details
	Arch형 교량은 지원하지 않음.
	내진말뚝, 복합말뚝
	강도설계법 일경우 = TRUE
*@return BOOL
*@author 전기주
*/

BOOL CInputBaseDesignBasicData::IsCheckNotLSDType()
{
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;
	if(!pStd) return FALSE;

	BOOL bLSDTypeOK = TRUE; 
	if(pStd->IsLsdDesign() == FALSE)
		return bLSDTypeOK;

	CString strUsdType(_T(""));
	long nSize = pStd->GetBridgeSize();
	for(long ix=0; ix<nSize; ++ix)
	{
		CRcBridgeApp *pBri = pStd->GetBridge(ix);
		if(pBri)
		{
			if(pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH)
			{
				strUsdType = _T("Arc형 교량");
				bLSDTypeOK = FALSE;
				break;
			}

			long nSizeFoot = pBri->GetCountFooting();
			for(long nFoot=0; nFoot<nSizeFoot; ++nFoot)
			{
				CFootingApp *pFoot = pBri->GetFooting(nFoot);
				if(pFoot == NULL) continue;
				if(pFoot->m_exFooting.m_nType != EXFOOTING_TYPE_PILE) continue;
				if(pFoot->m_exFooting.m_Pile.m_nType ==  PILE_TYPE_EHP || pFoot->m_exFooting.m_Pile.m_nType ==  PILE_TYPE_SPC)
				{
					bLSDTypeOK = FALSE;
					strUsdType = _T("내진말뚝, SPC 말뚝");
					break;
				}
			}

			if(strUsdType.IsEmpty() == FALSE)
				break;
		}
	}

	
	if( pStd->m_nTypeConditionApply == DESIGN_CONDITION_RAIL )		// 철도교 설계기준
	{
		bLSDTypeOK = FALSE;
		strUsdType = "철도교 설계기준";
	}

	if (pStd->m_nTypeUnit == 0)
	{
		bLSDTypeOK = FALSE;
		strUsdType += "\nCGS 단위계";
	}

	if(bLSDTypeOK == FALSE)
	{
		CString strMessage;
		strMessage.Format(_T("지원하지 않는 교량/설계기준이 적용되었습니다.\n설계법을 변경전 형식을 변경해야 합니다.\n\n%s"), strUsdType);
		AfxMessageBox(strMessage);
	}

	return bLSDTypeOK;
}

BOOL CInputBaseDesignBasicData::OnChangeMethod()
{
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;
	if(IsCheckNotLSDType() == FALSE)
	{
		pStd->m_nTypeDesignMethod = (pStd->m_nTypeDesignMethod == DESIGN_METHOD_STRENGTH)? DESIGN_METHOD_LSD : DESIGN_METHOD_STRENGTH; 
		return FALSE;
	}
	if(AfxMessageBox(_T("설계법 변경시 기준에서 제시된 내용의 변경등으로 인해 기존의 입력값 중 초기화되는 사항이 있으며, 기초자료설정, 설계환경, 기초부설정 등 입력확인후 진행하셔야 합니다. \n입력자료의 변경사항은 설계법의 재 변경시 기존의 입력값이 복구되지 않습니다. \n계속 진행하시겠습니까?"), MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		BOOL bLSD = pStd->IsLsdDesign();

		pStd->m_nTypeDesignMethod = bLSD ? DESIGN_METHOD_STRENGTH : DESIGN_METHOD_LSD; 

		pStd->SetChangeDesignMethod();

		return FALSE;
	}

	return TRUE;
}

void CInputBaseDesignBasicData::SetGridDataRebarFactorLsd()
{
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 1, 2);
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;

	long nRow = 0;
	long nRows = 0;
	long nCols = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;

	m_Grid.AddCell(nRow, 0,hggettext("구분"));		
	m_Grid.AddCell(nRow, 1,hggettext("종류"));		
	m_Grid.AddCell(nRow, 2,hggettext("계수"));		

	nRow++;
	m_Grid.AddCell(nRow, 0,_T("n1\n")); 
	m_Grid.AddCell(nRow, 1,hggettext("부착상태 보정계수(보통의경우)")); 
	m_Grid.AddCell(nRow, 2, &pStd->m_plsdRebar->GetOption().dn1[0]);	m_Grid.SetCellDec(CELL_DEC_2);  m_Grid.SetLastCellMinLimit(0.1);

	nRow++;
	m_Grid.AddCell(nRow, 0,_T("n1\n")); 
	m_Grid.AddCell(nRow, 1,hggettext("부착상태 보정계수(상부철근, 30cm 이상의 높이에 배치된 철근)")); 
	m_Grid.AddCell(nRow, 2, &pStd->m_plsdRebar->GetOption().dn1[1]);	m_Grid.SetCellDec(CELL_DEC_2);  m_Grid.SetLastCellMinLimit(0.1);

	nRow++;
	m_Grid.AddCell(nRow, 0,_T("n2\n")); 
	m_Grid.AddCell(nRow, 1,hggettext("철근지름 영향 보정계수(32mm 이하)")); 
	m_Grid.AddCell(nRow, 2, &pStd->m_plsdRebar->GetOption().dn2);	m_Grid.SetCellDec(CELL_DEC_2);  m_Grid.SetLastCellMinLimit(0.1);

	nRow++;
	m_Grid.AddCell(nRow, 0,_T("n2\n")); 
	m_Grid.AddCell(nRow, 1,hggettext("철근지름 영향 보정계수(32mm 초과, (132-db/100))")); 
	m_Grid.AddCell(nRow, 2,hggettext("-"),nFormat,CELL_READONLY); m_Grid.SetCellDec(CELL_DEC_2); 

	nRow++;
	m_Grid.AddCell(nRow, 0,_T("fr")); 
	m_Grid.AddCell(nRow, 1,hggettext("구속응력Mpa")); 
	m_Grid.AddCell(nRow, 2, &pStd->m_plsdRebar->GetOption().dRestraintStress);	m_Grid.SetCellDec(CELL_DEC_2);

	nRow++;
	m_Grid.AddCell(nRow, 0,_T("Ro")); 
	m_Grid.AddCell(nRow, 1,hggettext("겹침이음비율(%)")); 
	m_Grid.AddCell(nRow, 2, &pStd->m_plsdRebar->GetOption().dOverRatio);	m_Grid.SetCellDec(CELL_DEC_1);

	nRow++;
	m_Grid.AddCell(nRow, 0,_T("Ras")); 
	m_Grid.AddCell(nRow, 1,hggettext("필요/사용철근량비(As.req/As,.prov)")); 
	m_Grid.AddCell(nRow, 2, &pStd->m_plsdRebar->GetOption().dAsRatio);	m_Grid.SetCellDec(CELL_DEC_3);

	m_Grid.GetCellCount(nRows, nCols);
	m_Grid.SetRows(nRows);
	m_Grid.SetCols(nCols);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(70);
	m_Grid.SetColumnWidth(1,400);
}

void CInputBaseDesignBasicData::SetGridDataRebarFactor()
{
	if(m_bDesignLsd)
		SetGridDataRebarFactorLsd();
	else
		SetGridDataRebarFactorUsd();

	return;
}
