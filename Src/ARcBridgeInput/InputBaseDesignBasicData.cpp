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
	// �⺻�Է½ô� ����� �Ź� �����ֵ��� �����س���.
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
	m_sArrTabName.Add("���� ȯ��");					m_nTabFactor		= nTab++;
	if(m_bDesignLsd)
	{
		m_sArrTabName.Add("������װ��");			m_nTabResistFactor		= nTab++;
		m_sArrTabName.Add("���ʺ� ���װ��");		m_nTabFootResistFactor	= nTab++;
	}
	else
	{
		m_sArrTabName.Add("ö�� ������");			m_nTabSettle		= nTab++;
		m_sArrTabName.Add("ö�� ������");			m_nTabJoint			= nTab++;
	}
	m_sArrTabName.Add("ö�� ���� �߷� �� �ܸ���");	m_nTabWeight		= nTab++;
	m_sArrTabName.Add("��Ÿ ����� ���� �߷�");		m_nTabUWeight		= nTab++;
	if (m_bDesignLsd == FALSE)
	{
		m_sArrTabName.Add("ö�� ����, ���� ���� ���");	m_nTabRebarFactor	= nTab++;
		m_sArrTabName.Add("ö�� ����");					m_nTabRailLoad		= nTab++;
	}
	m_sArrTabName.Add("��ũ �� ������ ����");		m_nTabTankLoad		= nTab++;

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

	// �����Ұ� �ִ��� üũ�ϰ� ������.
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

	pStd->m_nTypeConditionApply		= DESIGN_CONDITION_ROAD;	// ���α� ���� ������ �켱������
	pStd->m_nTypeKciCode			= DESIGN_CODE_KCI_2012;
	pStd->m_nTypeRoadCode			= pStd->IsLsdDesign() ? DESIGN_ROAD_2015 : DESIGN_ROAD_2010;
	pStd->m_nTypeRailCode			= DESIGN_RAIL_2017;
	pStd->m_nDegreeBridge			= 0;
	pStd->m_Fck					= 270;
	pStd->m_Fy					= 4000;
	pStd->m_Ec					= GetValueEcConc(pStd->m_Fck,Wc,TRUE,pStd->m_nTypeKciCode); // ���α�������� 38������
	pStd->m_Es					= 2000000;
	pStd->m_Es_Steel			= pStd->GetEs_Steel();
	pStd->m_NValue_Steel			= 8.0;
	pStd->m_nTypeUnit				= UNIT_TYPE_SI;
	pStd->m_nLSDRoadType = 0;
	pStd->m_dDesignRoadWidth = 3600;
	pStd->m_nApplyBoredPileASD = 0;
	pStd->m_nApplyPhcPileASD = 0;

	SetDataDefaultFactor_Deduct();
	pStd->m_dDeltaTemperature		= 20;		// ������ħ�� �̵��� ������ �µ���ȭ��
	pStd->m_dCoefficientExpansion	= 0.000010;	// ����â���
	pStd->m_dRateDryAndShrinkage	= 0.00015;	// ����������

	pStd->m_dLengthRebarMax			= 8000;
	pStd->m_nDegreeEarthQuake		=	0;	// ������� (1,2,Ư)
	pStd->m_nRegionEarthQuake		=	0;	// �������� (1,2)
	pStd->m_dCoefficientGround	    = 1.0;
	pStd->m_nTypeRebarConnect		=	1;  // ö�ٰ����� ��� - ��������� ���� ���
	pStd->m_nTypeBMApply			=	1;
	pStd->m_pBasicConcInfo->m_nRoundRebarConnect = 1;
	pStd->m_bIncludeSumZero			= TRUE;

	pStd->m_RatePoasong		= 0.1667;		// ���ƽ���
	pStd->m_gValue			= 9.810;		// �߷°��ӵ�
	pStd->m_nTypeDesingEQ	= 3;
	
	pStd->m_nTypeDesingEQ			= 1;
	pStd->m_nTypeRigidFootingEQ		= 0;
	pStd->m_nTypeJointPosFootingEQ	= 0;	// �����ؼ��� ���ʺ� ���� ��ġ (���ʻ��,�����߾�)
	pStd->m_nTypePropertyPosFootingEQ	= 0;	// �����ؼ��� ���ʺ� ���� ��ġ (���ʻ��,�����߾�)
	pStd->m_nTypeUpperWeightEQ		= 0;	// �����ؼ��� ����߷� ����
	pStd->m_nTypeComboEQ			= 0;
	pStd->m_dRateDAMP_EQ			= 0.05;
	pStd->SetStringUnitType();
	pStd->m_bApplyAllowInc			= TRUE;	// �������� ����� ������� ������� ����
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
	// �׸��� �ʱ�ȭ
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
	m_Grid.AddCell(row, col++,"��  ��");
	m_Grid.AddCell(row, col++,"��  ��");
	m_Grid.AddCell(row, col++,"������");
	m_Grid.AddCell(row++, col,"��  ��");

	// ��������� or �Ѱ���¼����
	if(pStd->IsLockRcBridgeLsdModule())
	{
		sArr.RemoveAll();
		sArr.Add(_T("���������"));
		sArr.Add(_T("�Ѱ���¼����"));
		
		col	= 0;
		m_Grid.AddCell(row, col++, "������� �� ���");
		m_Grid.AddCell(row, col++, hggettext("  ������"), nFormatLeft);
		m_Grid.AddCell(row++, col++, &pStd->m_nTypeDesignMethod, nFormatRight, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
	}
		
	sArr.RemoveAll();
	long i=0; for(i=0; i<MAX_CONDITION_TYPE; i++)
		sArr.Add(pStd->GetStringConditionApply(i));
	col = 0;
	m_Grid.AddCell(row, col++, "������� �� ���");
	m_Grid.AddCell(row, col++, "  ���뼳�����(��������)", nFormatLeft);
	m_Grid.AddCell(row++, col, &pStd->m_nTypeConditionApply, nFormatRight, TRUE, m_bDesignLsd ? CELL_READONLY : nLine, "", CELL_TYPE_COMBO, sArr);

	// (ARCBRIDGE-3536) ���� ����. �Ѱ�����϶��� 'KCI 2012', ����������϶��� ���α�2010�϶� 'KCI 2007'�� ���� ���α�2008�϶� 'KCI 2003' ���
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
	m_Grid.AddCell(row, col++, "������� �� ���");
	m_Grid.AddCell(row, col++, "  ��ũ��Ʈ �����������", nFormatLeft);
	m_Grid.AddCell(row++, col, &pStd->m_nTypeKciCode, nFormatRight, TRUE, nCellLine, "", CELL_TYPE_COMBO, sArr);

	//
	sArr.RemoveAll();
	if (m_bDesignLsd)
	{
		sArr.Add("���α�������� 2015");
		sArr.Add("���α�������� KDS 24 10 11 : 2019");
		nBaseIndex = 2;
	}
	else
	{
		sArr.Add("2008 �������");
		sArr.Add("2010 �������");
		nBaseIndex = 0;
	}
	
	col = 0;
	m_Grid.AddCell(row, col++, "������� �� ���");
	m_Grid.AddCell(row, col++, "  ���α� �������", nFormatLeft);
	m_Grid.AddCell(row++, col, &pStd->m_nTypeRoadCode, nFormatRight, TRUE, pStd->m_nTypeConditionApply == DESIGN_CONDITION_ROAD ? CELL_LINE_NO : CELL_READONLY, "", CELL_TYPE_COMBO, sArr);  m_Grid.SetCellBaseOfIndex(nBaseIndex);

	sArr.RemoveAll(); sArr.Add("1 �"); sArr.Add("2 �"); sArr.Add("3 �");
	
	col = 0;
	m_Grid.AddCell(row, col++, "������� �� ���");
	m_Grid.AddCell(row, col++, "  ���豳���� ���", nFormatLeft);
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
		// ö���� ���� ���û���
		sArr.RemoveAll();
		sArr.Add("ö��������� 2004");
		sArr.Add("ö��������� 2017");
		sArr.Add("ö��������� KDS 47 00 00 : 2018");
		
		col = 0;
		m_Grid.AddCell(row, col++, "������� �� ���");
		m_Grid.AddCell(row, col++, "  ö�� �������", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_nTypeRailCode, nFormatRight, TRUE, CellLineRail, "", CELL_TYPE_COMBO, sArr);

		if(bRail2017 == FALSE)
		{
			sArr.RemoveAll();
			sArr.Add("1 �޼�");
			sArr.Add("2 �޼�");
			sArr.Add("3 �޼�");
			sArr.Add("4 �޼�");
			col = 0;
			m_Grid.AddCell(row, col++, "������� �� ���");
			m_Grid.AddCell(row, col++, "  ���輱���� ���", nFormatLeft); 
			m_Grid.AddCell(row++, col, &pStd->m_nDegreeRail, nFormatRight, TRUE, CellLineRail, "", CELL_TYPE_COMBO, sArr);
		}		

		sArr.RemoveAll();
		long icMaxRail = bRail2017 ? MAX_RAIL_LOAD_2017 : MAX_RAIL_LOAD;
		for(i = 0; i < icMaxRail; i++)
			sArr.Add(pStd->GetStringRailLoad(i));
		col = 0;
		m_Grid.AddCell(row, col++, "������� �� ���");
		m_Grid.AddCell(row, col++, "  ���������� ����", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_nTypeRailLoad, nFormatRight, TRUE, CellLineRail, "", CELL_TYPE_COMBO, sArr);

		sArr.RemoveAll();
		sArr.Add("�ܼ�");
		sArr.Add("����");
		col = 0;
		m_Grid.AddCell(row, col++, "������� �� ���");
		m_Grid.AddCell(row, col++, "  ��꼱��", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_nTypeRailQty, nFormatRight, TRUE, CellLineRail, "", CELL_TYPE_COMBO, sArr);
	}
	else
	{
		sArr.RemoveAll();
		sArr.Add(_T("�Ϲݵ���"));
		sArr.Add(_T("��ӵ���"));
		col = 0;
		m_Grid.AddCell(row, col++, "������� �� ���");
		m_Grid.AddCell(row, col++, "  ������ ����", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_nLSDRoadType, nFormatRight, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);

		col = 0;
		m_Grid.AddCell(row, col++, "������� �� ���");
		m_Grid.AddCell(row, col++, "  ���������� ��(Wp)", nFormatLeft);
		m_Grid.AddCell(row, col++, &pStd->m_dDesignRoadWidth, nFormatRight);
		m_Grid.SetCellDec(CELL_DEC_0);
		m_Grid.AddCell(row++, col, "mm",nFormatLeft);
	}

	col = 0;
	sArr.RemoveAll();
	sArr.Add(_T("�ü����� �������"));
	sArr.Add(_T("��������(KDS 17 10 00)"));

	UINT nLineEq = (pStd->m_nTypeConditionApply == DESIGN_CONDITION_ROAD && pStd->m_nTypeRoadCode == DESIGN_ROAD_KDS_2019) || pStd->IsRailDesignKDS2018() ? CELL_READONLY : CELL_LINE_NO;
	m_Grid.AddCell(row, col++, "������� �� ���");
	m_Grid.AddCell(row, col++, hggettext(" �����������"), nFormatLeft);
	m_Grid.AddCell(row++, col, &pStd->m_nTypeEarthQuakeApply, nFormatRight, TRUE, nLineEq, _T(""), CELL_TYPE_COMBO, sArr);

	sArr.RemoveAll(); sArr.Add("CGS ������"); sArr.Add("SI ������");
	col = 0;
	m_Grid.AddCell(row, col++, "������� �� ���");
	m_Grid.AddCell(row, col++, "  ���������", nFormatLeft); 
	m_Grid.AddCell(row++, col, &pStd->m_nTypeUnit, nFormatRight, TRUE, (pStd->m_nTypeConditionApply == DESIGN_CONDITION_ROAD && pStd->m_nTypeRoadCode == DESIGN_ROAD_2010) || m_bDesignLsd ? CELL_READONLY : nLine, "", CELL_TYPE_COMBO, sArr);

	if(m_bDesignLsd)
	{
 		col	= 0;
		m_Grid.AddCell(row, col++, hggettext("������� �� ���"));
		m_Grid.AddCell(row, col++, hggettext(" ���������� ��¹��"), nFormatLeft);
		m_Grid.AddCell(row, col++, &pStd->m_bPrintConditionApply, nFormatRight, nLine, _T(""), CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col++, pStd->m_bPrintConditionApply ? hggettext("�⺻��������") : hggettext("������������"), nFormatRight, CELL_READONLY);
		
		col	= 0;
		sArr.RemoveAll();
		sArr.Add(hggettext("���α�������� 2015"));
		sArr.Add(hggettext("������������� 2014"));
		m_Grid.AddCell(row, col++, hggettext("������� �� ���"));
		m_Grid.AddCell(row, col++, hggettext(" �������� �������"), nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_nTypeCheckSteelPile, nFormatRight, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
		col	= 0;

		sArr.RemoveAll();
		sArr.Add(_T("�������"));
		sArr.Add(_T("����"));
		col = 0;

		m_Grid.AddCell(row, col++, "������� �� ���");
		m_Grid.AddCell(row, col++, "  �������Ը��� ����� ������¼���� ����", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_nApplyBoredPileASD, nFormatRight, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

		col = 0;
		m_Grid.AddCell(row, col++, "������� �� ���");
		m_Grid.AddCell(row, col++, "  PHC���� ����� ������¼���� ����", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_nApplyPhcPileASD, nFormatRight, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

		col = 0;
		sArr.RemoveAll();
		sArr.Add(hggettext("����Ÿ�������� ö�ٺ� ����"));
		sArr.Add(hggettext("������ �����ö�ٺ� ����"));
		m_Grid.AddCell(row, col++, "������� �� ���");
		m_Grid.AddCell(row, col++, "  �شܻ�Ȳ�Ѱ���¥��� ����Ÿ������ ö�ٺ�", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_nSelectPmin_Hyunta_Extreme, nFormatRight, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	}


	CString szKgf_cm2_CGS   = "kgf/cm��";
	CString szKgf_cm2_Apply = pStd->GetStringUnitType(szKgf_cm2_CGS);

	if(m_bDesignLsd == FALSE)
	{
		col = 0;
		m_Grid.AddCell(row, col++, "���Ư�� �� �����");
		m_Grid.AddCell(row, col++, "  fck : ��ũ��Ʈ ����", nFormatLeft);
		m_Grid.AddCell(row, col++, &pStd->m_Fck, nFormatRight);
		m_Grid.SetCellDec(CELL_DEC_1);
		m_Grid.SetCellUnit(szKgf_cm2_CGS,szKgf_cm2_Apply);
		m_Grid.AddCell(row++, col, szKgf_cm2_Apply,nFormatLeft);

		col = 0;
		m_Grid.AddCell(row, col++, "���Ư�� �� �����");
		m_Grid.AddCell(row, col++, "  fy  : ö�� �׺�����", nFormatLeft);
		m_Grid.AddCell(row, col++, &pStd->m_Fy, nFormatRight);
		m_Grid.SetCellDec(CELL_DEC_1);
		m_Grid.SetCellUnit(szKgf_cm2_CGS,szKgf_cm2_Apply);
		m_Grid.AddCell(row++, col, szKgf_cm2_Apply,nFormatLeft);

		col	= 0;
		m_Grid.AddCell(row, col++, "���Ư�� �� �����");
		m_Grid.AddCell(row, col++, "  fy  : ������ ����", nFormatLeft, nLine);	
		m_Grid.AddCell(row++, col, &pStd->m_bIsDBar_ForDowelBar, nFormatRight, nLine, pStd->m_bIsDBar_ForDowelBar ? "D��" : "H��", CELL_TYPE_CHECK);

		col = 0;
		m_Grid.AddCell(row, col++, "���Ư�� �� �����");
		m_Grid.AddCell(row, col++, "  Ec  : ��ũ��Ʈ ź�����", nFormatLeft);
		m_Grid.AddCell(row, col++, &pStd->m_Ec, nFormatRight);
		m_Grid.SetCellDec(CELL_DEC_1);
		m_Grid.SetCellUnit(szKgf_cm2_CGS,szKgf_cm2_Apply);
		m_Grid.AddCell(row++, col, szKgf_cm2_Apply,nFormatLeft);
	}

	col = 0;
	m_Grid.AddCell(row, col++, "���Ư�� �� �����");
	m_Grid.AddCell(row, col++, "  Es  : ö���� ź�����", nFormatLeft);
	m_Grid.AddCell(row, col++, &pStd->m_Es, nFormatRight);
	m_Grid.SetCellDec(CELL_DEC_1);
	m_Grid.SetCellUnit(szKgf_cm2_CGS,szKgf_cm2_Apply);
	m_Grid.AddCell(row++, col, szKgf_cm2_Apply,nFormatLeft); 
	
	col = 0;
	m_Grid.AddCell(row, col++, "���Ư�� �� �����");
	m_Grid.AddCell(row, col++, "  Es  : ������ ź�����", nFormatLeft);
	m_Grid.AddCell(row, col++, &pStd->m_Es_Steel, nFormatRight);
	m_Grid.SetCellDec(CELL_DEC_1);
	m_Grid.SetCellUnit(szKgf_cm2_CGS,szKgf_cm2_Apply);
	m_Grid.AddCell(row++, col, szKgf_cm2_Apply,nFormatLeft); 
	
	if (m_bDesignLsd)
	{
		col = 0;
		m_Grid.AddCell(row, col++, "���Ư�� �� �����");
		m_Grid.AddCell(row, col++, "  v   : ��ũ��Ʈ�� ���ƽ���", nFormatLeft);
		m_Grid.AddCell(row++, col++, &pStd->m_RatePoasong, nFormatRight);
	}
	else
	{
		col = 0;
		m_Grid.AddCell(row, col++, "���Ư�� �� �����");
		m_Grid.AddCell(row, col++, "  n   : ����� ��ũ��Ʈ�� ź�������", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_NValue_Steel, nFormatRight);

		col = 0;
		m_Grid.AddCell(row, col++, "���Ư�� �� �����");
		m_Grid.AddCell(row, col++, "  ��f : �ں���", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_Pi_F, nFormatRight);
		m_Grid.SetCellDec(CELL_DEC_2); 

		col = 0;
		m_Grid.AddCell(row, col++, "���Ư�� �� �����");
		m_Grid.AddCell(row, col++, "  ��c : �����������", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_Pi_C, nFormatRight);
		m_Grid.SetCellDec(CELL_DEC_2); 

		col = 0;
		m_Grid.AddCell(row, col++, "���Ư�� �� �����");
		m_Grid.AddCell(row, col++, "  ��v : ���ܰ� ��Ʋ��", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_Pi_V, nFormatRight);
		m_Grid.SetCellDec(CELL_DEC_2); 

		//	col = 1; m_Grid.AddCell(++row, col++, "  ��b : ��ũ��Ʈ�� ����", nFormatLeft); m_Grid.AddCell(row, col++, &pStd->m_PI_B, nFormatRight); m_Grid.SetCellDec(CELL_DEC_2); 
		col = 0;
		m_Grid.AddCell(row, col++, "���Ư�� �� �����");
		m_Grid.AddCell(row, col++, "  ��ħ�� ���� ���Ұ�� (��)", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_Pi_F_Cobel, nFormatRight);
		m_Grid.SetCellDec(CELL_DEC_2); 

		col = 0;
		m_Grid.AddCell(row, col++, "���Ư�� �� �����");
		m_Grid.AddCell(row, col++, "  ��  : ���������", nFormatLeft); 
		m_Grid.AddCell(row++, col, &pStd->m_dRateDryAndShrinkage, nFormatRight);
		m_Grid.SetCellDec(CELL_DEC_6); 

		col = 0;
		m_Grid.AddCell(row, col++, "���Ư�� �� �����");
		m_Grid.AddCell(row, col++, "  ��  : ����â���", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_dCoefficientExpansion, nFormatRight);
		m_Grid.SetCellDec(CELL_DEC_7); 

		//	col = 1; m_Grid.AddCell(++row, col++, "  ��T  : ������ħ�� �̵��� ������ �µ���ȭ��", nFormatLeft); m_Grid.AddCell(row, col++, &pStd->m_dDeltaTemperature, nFormatRight); m_Grid.SetCellDec(CELL_DEC_1); m_Grid.AddCell(row, col++, "��",nFormatLeft); // �µ�

		sArr.RemoveAll(); 
		//	sArr.Add("�����ؼ� ��������"); 
		//	sArr.Add("���ϸ�� ����Ʈ���ؼ�"); 
		//	sArr.Add("���߸�� ����Ʈ���ؼ�"); // ���� ���� ?????
		//	col = 1; m_Grid.AddCell(++row, col++, "  �����ؼ����", nFormatLeft); m_Grid.AddCell(row, col++, &pStd->m_nTypeDesingEQ, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
		sArr.Add("�����ؼ� ��������"); 
		sArr.Add("�����ؼ� ����"); 
		col = 0;
		m_Grid.AddCell(row, col++, "�����ؼ�");
		m_Grid.AddCell(row, col++, "  �����ؼ�����", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_nTypeDesingEQ, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);

		if(pStd->m_nTypeDesingEQ != 0)
		{
			m_BridgeLevelDlg.m_nTypeUnit = pStd->m_nTypeUnit;
			col = 0;
			m_Grid.AddCell(row, col++, "�����ؼ�");
			m_Grid.AddCell(row, col++, "  �������", nFormatLeft);
			if(bKDS17)
			{
				sArr.RemoveAll(); sArr.Add("I ���"); sArr.Add("II���"); sArr.Add("Ư��� 2400"); sArr.Add("Ư��� 4800");
				m_Grid.AddCell(row++, col, &pStd->m_nDegreeEarthQuake, nFormatRight, TRUE, nLine, "", CELL_TYPE_AUTO, sArr, &m_EarthquakeGradeKDS17Dlg);
			}
			else
			{
				sArr.RemoveAll(); sArr.Add("I ���"); sArr.Add("II���");;
				m_Grid.AddCell(row++, col, &pStd->m_nDegreeEarthQuake, nFormatRight, TRUE, nLine, "", CELL_TYPE_AUTO, sArr, &m_EarthquakeGradeDlg);
			}

			sArr.RemoveAll(); sArr.Add("I ����"); sArr.Add("II����");
			col = 0;
			m_Grid.AddCell(row, col++, "�����ؼ�");
			m_Grid.AddCell(row, col++, "  ��������", nFormatLeft);
			m_Grid.AddCell(row++, col, &pStd->m_nRegionEarthQuake, nFormatRight, TRUE, nLine, "", CELL_TYPE_AUTO, sArr, &m_EarhtquakeDlg);

			col = 0;
			m_Grid.AddCell(row, col++, "�����ؼ�");
			m_Grid.AddCell(row, col++, bKDS17 ? "  ���ݺ������" : "  ���ݰ��", nFormatLeft);
			if(bKDS17)
				m_Grid.AddCell(row++, col, &pStd->m_dCoefficientGround, nFormatRight, nLine);
			else
				m_Grid.AddCell(row++, col, &pStd->m_dCoefficientGround, nFormatRight, nLine, "", CELL_TYPE_AUTO, sArr, &m_EarthquakeFactDlg);
			m_Grid.SetCellDec(CELL_DEC_2);
			
			if(bKDS17)
			{
				sArr.RemoveAll();
				col = 0;
				m_Grid.AddCell(row, col++, "�����ؼ�");
				m_Grid.AddCell(row, col++, "  ��������", nFormatLeft);
				for(long ix = 0; ix < 6; ++ix)
				{
					str.Format(_T("S%d"), ix+1);
					sArr.Add(str);
				}
				m_Grid.AddCell(row++, col, &pStd->m_nTypeCoefficientGround, nFormatRight, TRUE, nLine, "", CELL_TYPE_AUTO, sArr, &m_EarthquakeFactKDS17Dlg);
				m_Grid.SetCellDec(CELL_DEC_2);
			
				col = 0;
				m_Grid.AddCell(row, col++, "�����ؼ�");
				m_Grid.AddCell(row, col++, hggettext("  �����(��) : ���躸�����(CD) ����"), nFormatLeft);
				m_Grid.AddCell(row, col++, &pStd->m_dRateDAMP_EQ_CD, nFormatRight);	m_Grid.SetCellDec(CELL_DEC_3);
				m_Grid.AddCell(row++, col, _T("%"), nFormatLeft);
			}

			col = 0;
			m_Grid.AddCell(row, col++, "�����ؼ�");
			m_Grid.AddCell(row, col++, "  �߷� ���ӵ�", nFormatLeft);
			m_Grid.AddCell(row, col++, &pStd->m_gValue, nFormatRight);
			m_Grid.SetCellDec(CELL_DEC_3);
			m_Grid.AddCell(row++, col, "m/sec��",nFormatLeft);

			sArr.RemoveAll(); 
			sArr.Add("��ü������ ���ʰ����� ����"); 
			sArr.Add("��ü������ ���ʰ����� ��������"); 
			sArr.Add("���ʰ����� ���Ѱ������� ����");
			col = 0;
			m_Grid.AddCell(row, col++, "�����ؼ�");
			m_Grid.AddCell(row, col++, "  �����ؼ��� ���� �𵨸� ���", nFormatLeft);
			m_Grid.AddCell(row++, col, &pStd->m_nTypeRigidFootingEQ, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);

			sArr.RemoveAll(); sArr.Add("����"); sArr.Add("�������");
			col = 0;
			m_Grid.AddCell(row, col++, "�����ؼ�");
			m_Grid.AddCell(row, col++, "  ��ü���� ��������� ȸ������ ����", nFormatLeft);
			m_Grid.AddCell(row++, col, &pStd->m_bApplyRigidWallEQRotate, nFormatRight, nLine, "", CELL_TYPE_COMBO, sArr);

			sArr.RemoveAll(); 
			sArr.Add("������ ��� ��ġ"); 
			sArr.Add("������ ���� ��ġ"); 
			col = 0;
			m_Grid.AddCell(row, col++, "�����ؼ�");
			m_Grid.AddCell(row, col++, "  �����ؼ��� ���� �𵨸� ��ġ", nFormatLeft);
			m_Grid.AddCell(row++, col, &pStd->m_nTypeJointPosFootingEQ, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);

			sArr.RemoveAll(); 
			sArr.Add("������ ����"); 
			sArr.Add("��ü�� ����"); 
			col = 0;
			m_Grid.AddCell(row, col++, "�����ؼ�");
			m_Grid.AddCell(row, col++, "  �����ؼ��� ���� �𵨸� ����", nFormatLeft);
			m_Grid.AddCell(row++, col, &pStd->m_nTypePropertyPosFootingEQ, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);

			sArr.RemoveAll(); 
			sArr.Add("�������߰� ���������� �շ� ���� - ���μ������(�Ǽ������,2001)"); 
			sArr.Add("�������߰� ���������� �и� ���� - �����������(�ѱ����ΰ���,2000)"); 
			col = 0;
			m_Grid.AddCell(row, col++, "�����ؼ�");
			m_Grid.AddCell(row, col++, "  �����ؼ��� ������� ���� ��ġ", nFormatLeft);
			m_Grid.AddCell(row++, col, &pStd->m_nTypeSoilPressureEQ, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);

			sArr.RemoveAll(); 
			sArr.Add("��α�����(������,��Ÿ������)"); 
			sArr.Add("��α�����,�Ϻα���1/2"); 
			col = 0;
			m_Grid.AddCell(row, col++, "�����ؼ�");
			m_Grid.AddCell(row, col++, "  �����ؼ��� �����߷� ���� ����", nFormatLeft);
			m_Grid.AddCell(row++, col, &pStd->m_nTypeUpperWeightEQ, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);

			//	sArr.RemoveAll(); // ���߸�忡���� ���....
			//	sArr.Add("CQC"); 
			//	sArr.Add("GMC"); 
			//	sArr.Add("SRSS"); 
			//	sArr.Add("ABS");
			//	col = 1; m_Grid.AddCell(++row, col++, "  �����ؼ������ ���� ���� ���", nFormatLeft); m_Grid.AddCell(row, col++, &pStd->m_nTypeComboEQ, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
			//
			//	col = 1; m_Grid.AddCell(++row, col++, "  �����ؼ��� �����", nFormatLeft); m_Grid.AddCell(row, col++, &pStd->m_dRateDAMP_EQ, nFormatRight); m_Grid.SetCellDec(CELL_DEC_2); 
		}
	}

	col = 0;
	m_Grid.AddCell(row, col++, "ö��");
	m_Grid.AddCell(row, col++, "  ö�� ������ ���� ����", nFormatLeft);
	m_Grid.AddCell(row, col++, &pStd->m_dLengthRebarMax, nFormatRight);
	m_Grid.AddCell(row++, col, "mm",nFormatLeft);
	
	if (m_bDesignLsd == FALSE)
	{
		sArr.RemoveAll(); sArr.Add("��������� ���ѹ��"); sArr.Add("��Ŀ� ���ѹ��");
		col = 0;
		m_Grid.AddCell(row, col++, "ö��");
		m_Grid.AddCell(row, col++, "  ������ �� ������ ������", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_nTypeRebarConnect, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
	}

	sArr.RemoveAll(); sArr.Add("�ݿø� ����"); sArr.Add("�ݿø�"); sArr.Add("�ø�");
	col = 0;
	m_Grid.AddCell(row, col++, "ö��");
	m_Grid.AddCell(row, col++, "  ö�� ������, ������ ������ �ݿø�", nFormatLeft);
	m_Grid.AddCell(row++, col, &pStd->m_pBasicConcInfo->m_nRoundRebarConnect, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);

	sArr.RemoveAll();
	sArr.Add("�ݿø� ����");
	sArr.Add("MM���� �ݿø�");
	sArr.Add("CM���� 3��4��");
	sArr.Add("CM���� 4��5��");
	col = 0;
	m_Grid.AddCell(row, col++, "ö��");
	m_Grid.AddCell(row, col++, "  ö�� ���� �ݿø� ����", nFormatLeft);
	m_Grid.AddCell(row++, col, &pStd->m_pBasicConcInfo->m_nRoundRebarLength, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
	
	if (m_bDesignLsd == FALSE)
	{
		sArr.RemoveAll(); sArr.Add("����"); sArr.Add("�������");
		col = 0;
		m_Grid.AddCell(row, col++, "��Ÿ ����");
		m_Grid.AddCell(row, col++, "  �������� ����� ������� ������� ����", nFormatLeft);
		m_Grid.AddCell(row++, col, &pStd->m_bApplyAllowInc, nFormatRight, nLine, "", CELL_TYPE_COMBO, sArr);
	}

	sArr.RemoveAll();
	sArr.Add("���ϸ�_������_����");
	sArr.Add("������_����");
	sArr.Add("���ϸ�_����");
	sArr.Add("���ϸ�_������");
	sArr.Add("������");
	col = 0;
	m_Grid.AddCell(row, col++, "��Ÿ ����");
	m_Grid.AddCell(row, col++, "  Excel��½� ���ϸ� �������", nFormatLeft);
	m_Grid.AddCell(row++, col, &pStd->m_nTypeFileNameForExcel, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);

	if (m_bDesignLsd == FALSE)
	{
		if(pStd->IsRailDesign2017())
		{
			sArr.RemoveAll(); sArr.Add("����"); sArr.Add("�������");
			col = 0;
			m_Grid.AddCell(row, col++, "��Ÿ ����");
			m_Grid.AddCell(row, col++, "  ö���� ��������� Ȱ���� ��� ����", nFormatLeft);
			m_Grid.AddCell(row++, col, &pStd->m_nTypeFootingSafetyApplyCoefficientImpact, nFormatRight, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
		}
		else
		{
			pStd->m_nTypeFootingSafetyApplyCoefficientImpact = 0;
		}

		sArr.RemoveAll(); sArr.Add("����"); sArr.Add("�������");
		col = 0;
		m_Grid.AddCell(row, col++, "��Ÿ ����");
		m_Grid.AddCell(row, col++, "  ö���� ������ ��ȿ�� ������ ������ �β� ����", nFormatLeft);
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
			m_Grid.AddCell(row, col++, "��Ÿ ����");
			m_Grid.AddCell(row, col++, "  ������ ǥ�ص� �۾�", nFormatLeft); 
			m_Grid.AddCell(row++, col, &pStd->m_nTypeProcess, nFormatRight, FALSE, nLineTypeProcess, pStd->m_nTypeProcess == TYPE_PROCESS_NORMAL ? "�������" : "����", CELL_TYPE_CHECK);
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

	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 2, 1);

	long nRow = 0;
	long nCol = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	UINT nLine = CELL_LINE_NO;	

	CString str = _T("");
	if(pStd->m_nTypeRebarConnect==0)
	{	
		// Ÿ��Ʋ �׸���
		m_Grid.AddCell(nRow, 0,"ö��ȣĪ\n");
		m_Grid.AddCell(nRow+1, 0,"ö��ȣĪ\n");
		m_Grid.AddCell(nRow, 1,"����ö��");
		m_Grid.AddCell(nRow, 2,"����ö��");
		m_Grid.AddCell(nRow, 3,"����ö��");
		m_Grid.AddCell(nRow, 4,"����ö��");
		m_Grid.AddCell(nRow, 5,"����ö��");
		nRow = 1;
		m_Grid.AddCell(nRow, 1,"�⺻����");
		m_Grid.AddCell(nRow, 2,"�Ϲ�ö��");
		m_Grid.AddCell(nRow, 3,"���ö��");	
		m_Grid.AddCell(nRow, 4,"�⺻����");
		m_Grid.AddCell(nRow, 5,"����ö��");
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

		// ����Ÿ
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
		// Ÿ��Ʋ
		m_Grid.AddCell(nRow, 0,"ö��ȣĪ\n");
		m_Grid.AddCell(nRow+1, 0,"ö��ȣĪ\n");
		m_Grid.AddCell(nRow, 1,"����ö��");
		m_Grid.AddCell(nRow, 2,"����ö��");
		m_Grid.AddCell(nRow, 3,"����ö��");
		m_Grid.AddCell(nRow, 4,"����ö��");
		m_Grid.AddCell(nRow, 5,"����ö��");
		nRow = 1;
		m_Grid.AddCell(nRow, 1,"�⺻����");
		m_Grid.AddCell(nRow, 2,"�Ϲ�ö��");
		m_Grid.AddCell(nRow, 3,"���ö��");	
		m_Grid.AddCell(nRow, 4,"�⺻����");
		m_Grid.AddCell(nRow++, 5,"����ö��");
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
		m_Grid.AddCell(nRow++, 0,"�Ǻ��β�");	
		m_Grid.AddCell(nRow++, 0,"ö�ٰ���");	

		// ����Ÿ
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
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(3, 3, 3, 1);
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;

	long nRow = 0;
	long nCol = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	UINT nLine = CELL_LINE_NO;	
	
	if(pStd->m_nTypeRebarConnect==0)
	{	
		m_Grid.AddCell(nRow, 0,"ö��ȣĪ\n");
		m_Grid.AddCell(nRow+1, 0,"ö��ȣĪ\n");
		m_Grid.AddCell(nRow+2, 0,"ö��ȣĪ\n");
		m_Grid.AddCell(nRow, 1,"����ö��");
		m_Grid.AddCell(nRow, 2,"����ö��");
		m_Grid.AddCell(nRow, 3,"����ö��");
		m_Grid.AddCell(nRow, 4,"����ö��");
		m_Grid.AddCell(nRow, 5,"����ö��");
		m_Grid.AddCell(nRow, 6,"����ö��");
		nRow = 1;
		m_Grid.AddCell(nRow, 1,"�Ϲ�");
		m_Grid.AddCell(nRow, 2,"�Ϲ�");
		m_Grid.AddCell(nRow, 3,"���");
		m_Grid.AddCell(nRow, 4,"���");
		m_Grid.AddCell(nRow, 5,"�Ϲ�");	
		m_Grid.AddCell(nRow, 6,"����");
		nRow = 2;
		m_Grid.AddCell(nRow, 1,"A��");
		m_Grid.AddCell(nRow, 2,"B��");
		m_Grid.AddCell(nRow, 3,"A��");
		m_Grid.AddCell(nRow, 4,"B��");
		m_Grid.AddCell(nRow, 5,"B��");
		m_Grid.AddCell(nRow, 6,"��ö��");
		m_Grid.AddCell(0, 7,"����ö��\n");
		m_Grid.AddCell(1, 7,"����ö��\n");
		m_Grid.AddCell(2, 7,"����ö��\n");
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
		m_Grid.AddCell(nRow, 0,"ö��ȣĪ\n");
		m_Grid.AddCell(nRow+1, 0,"ö��ȣĪ\n");
		m_Grid.AddCell(nRow+2, 0,"ö��ȣĪ\n");
		m_Grid.AddCell(nRow, 1,"����ö��");
		m_Grid.AddCell(nRow, 2,"����ö��");
		m_Grid.AddCell(nRow, 3,"����ö��");
		m_Grid.AddCell(nRow, 4,"����ö��");
		m_Grid.AddCell(nRow, 5,"����ö��");
		m_Grid.AddCell(nRow, 6,"����ö��");
		nRow = 1;
		m_Grid.AddCell(nRow, 1,"�Ϲ�");
		m_Grid.AddCell(nRow, 2,"�Ϲ�");
		m_Grid.AddCell(nRow, 3,"���");
		m_Grid.AddCell(nRow, 4,"���");
		m_Grid.AddCell(nRow, 5,"�Ϲ�");	
		m_Grid.AddCell(nRow, 6,"����");
		nRow = 2;
		m_Grid.AddCell(nRow, 1,"A��");
		m_Grid.AddCell(nRow, 2,"B��");
		m_Grid.AddCell(nRow, 3,"A��");
		m_Grid.AddCell(nRow, 4,"B��");
		m_Grid.AddCell(nRow, 5,"B��");
		m_Grid.AddCell(nRow, 6,"��ö��");
		m_Grid.AddCell(0, 7,"����ö��\n");
		m_Grid.AddCell(1, 7,"����ö��\n");
		m_Grid.AddCell(2, 7,"����ö��\n");
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
		m_Grid.AddCell(nRow++, 0,"�Ǻ��β�");	
		m_Grid.AddCell(nRow, 0,"ö�ٰ���");	

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
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault();
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;

	long nRow = 0;
	long nCol = 0;
// 	CString szCGS   = "mm��";
// 	CString szApply = pStd->GetStringUnitType(szCGS);
	CString szTitle	= _T("");

	m_Grid.AddCell(nRow, nCol++, "\n�� ��");
	m_Grid.AddCell(nRow, nCol++, "\n��������\n(kg/m)");
	
	CString szCGS   = "cm��";
	CString szApply = pStd->GetStringUnitType(szCGS);
	szTitle.Format("\n�ܸ���\n(%s)", szApply);
	m_Grid.AddCell(nRow, nCol++, szTitle);

	szTitle.Format("\n��  ��\n(%s)", "mm");
	m_Grid.AddCell(nRow, nCol++, szTitle);

	szCGS   = "cm";
	szApply = pStd->GetStringUnitType(szCGS);
	szTitle.Format("\n��  ��\n(%s)", szApply);
	m_Grid.AddCell(nRow, nCol++, szTitle);

	szTitle.Format("\n�𼭸��� ���θ�\n������(%s)", "mm");
	m_Grid.AddCell(nRow, nCol++, szTitle);

	szTitle.Format("\n�𼭸��� �����\n���밪(%s)", "mm");
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

			if(j == 2)	// �ܸ��� ������ ����
			{
				szCGS   = "cm��";
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
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 1, 1);
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;

	long nRow = 0;
	long nCol = 0;
	long nRows = 0;
	long nCols = 0;
	
	m_Grid.AddCell(nRow  , 0,"��  ��");
	m_Grid.AddCell(nRow  , 1,"������");
	m_Grid.AddCell(nRow++, 2,"��  ��");

	CString szKgf_m3_CGS   = "kgf/m��";
	CString szKgf_m3_Apply = pStd->GetStringUnitType(szKgf_m3_CGS);
	long nDot = (pStd->m_nTypeUnit==UNIT_TYPE_SI) ? (m_bDesignLsd ? 7 : CELL_DEC_3) : CELL_DEC_0;
	//
	m_Grid.AddCell(nRow, nCol,"  ����, �ְ� �ܰ�",GVHT_LEFT); m_Grid.AddCell(nRow, nCol+1, &pStd->m_pBasicConcInfo->m_UWeightSteel, DT_RIGHT);		m_Grid.SetCellDec(nDot);  m_Grid.SetCellUnit(szKgf_m3_CGS,szKgf_m3_Apply);	m_Grid.AddCell(nRow++, nCol+2, szKgf_m3_Apply, DT_LEFT);
	m_Grid.AddCell(nRow, nCol,"  ��ö",GVHT_LEFT);			 m_Grid.AddCell(nRow, nCol+1, &pStd->m_pBasicConcInfo->m_UWeightCastIron, DT_RIGHT);		m_Grid.SetCellDec(nDot);   m_Grid.SetCellUnit(szKgf_m3_CGS,szKgf_m3_Apply);	m_Grid.AddCell(nRow++, nCol+2, szKgf_m3_Apply, DT_LEFT);
	m_Grid.AddCell(nRow, nCol,"  �˹̴�",GVHT_LEFT);			 m_Grid.AddCell(nRow, nCol+1, &pStd->m_pBasicConcInfo->m_UWeightAluminum, DT_RIGHT);		m_Grid.SetCellDec(nDot);   m_Grid.SetCellUnit(szKgf_m3_CGS,szKgf_m3_Apply);	m_Grid.AddCell(nRow++, nCol+2, szKgf_m3_Apply, DT_LEFT);
	m_Grid.AddCell(nRow, nCol,"  ö�� ��ũ��Ʈ",GVHT_LEFT);	 m_Grid.AddCell(nRow, nCol+1, &pStd->m_pBasicConcInfo->m_UWeightSteelConcrete, DT_RIGHT);m_Grid.SetCellDec(nDot);   m_Grid.SetCellUnit(szKgf_m3_CGS,szKgf_m3_Apply);	m_Grid.AddCell(nRow++, nCol+2, szKgf_m3_Apply, DT_LEFT);
	m_Grid.AddCell(nRow, nCol,"  �ڰ�,�⼮",GVHT_LEFT);		 m_Grid.AddCell(nRow, nCol+1, &pStd->m_pBasicConcInfo->m_UWeightGravel, DT_RIGHT);		m_Grid.SetCellDec(nDot);   m_Grid.SetCellUnit(szKgf_m3_CGS,szKgf_m3_Apply);	m_Grid.AddCell(nRow++, nCol+2, szKgf_m3_Apply, DT_LEFT);
	m_Grid.AddCell(nRow, nCol,"  �ø�Ʈ ����",GVHT_LEFT);	 m_Grid.AddCell(nRow, nCol+1, &pStd->m_pBasicConcInfo->m_UWeightCement, DT_RIGHT);		m_Grid.SetCellDec(nDot);   m_Grid.SetCellUnit(szKgf_m3_CGS,szKgf_m3_Apply);	m_Grid.AddCell(nRow++, nCol+2, szKgf_m3_Apply, DT_LEFT);
	m_Grid.AddCell(nRow, nCol,"  ����",GVHT_LEFT);			 m_Grid.AddCell(nRow, nCol+1, &pStd->m_pBasicConcInfo->m_UWeightLumber, DT_RIGHT);		m_Grid.SetCellDec(nDot);   m_Grid.SetCellUnit(szKgf_m3_CGS,szKgf_m3_Apply);	m_Grid.AddCell(nRow++, nCol+2, szKgf_m3_Apply, DT_LEFT);
	m_Grid.AddCell(nRow, nCol,"  ��û��(�����)",GVHT_LEFT);  m_Grid.AddCell(nRow, nCol+1, &pStd->m_pBasicConcInfo->m_UWeightInvChungje, DT_RIGHT);	m_Grid.SetCellDec(nDot);   m_Grid.SetCellUnit(szKgf_m3_CGS,szKgf_m3_Apply);	m_Grid.AddCell(nRow++, nCol+2, szKgf_m3_Apply, DT_LEFT);
	// Ⱦ�ܱ����� ���忡�� �Է¹ް� �ֱ� ������ ���⿡���� �Է��� �����ϱ�� ������ 2010.05.20 
	//m_Grid.AddCell(nRow, nCol,"  ����",GVHT_LEFT);			 m_Grid.AddCell(nRow, nCol+1, &pStd->m_pBasicConcInfo->m_UWeightAsphalt, DT_RIGHT);		m_Grid.SetCellDec(nDot);   m_Grid.SetCellUnit(szKgf_m3_CGS,szKgf_m3_Apply);	m_Grid.AddCell(nRow++, nCol+2, szKgf_m3_Apply, DT_LEFT);
	m_Grid.AddCell(nRow, nCol, m_bDesignLsd ? "  ��ũ��Ʈ" : "  ���� ��ũ��Ʈ",GVHT_LEFT);	 m_Grid.AddCell(nRow, nCol+1, &pStd->m_pBasicConcInfo->m_UWeightConcrete, DT_RIGHT);		m_Grid.SetCellDec(nDot);   m_Grid.SetCellUnit(szKgf_m3_CGS,szKgf_m3_Apply);	m_Grid.AddCell(nRow++, nCol+2, szKgf_m3_Apply, DT_LEFT);
	m_Grid.AddCell(nRow, nCol,"  ���ϼ�",GVHT_LEFT);  m_Grid.AddCell(nRow, nCol+1, &pStd->m_pBasicConcInfo->m_UWeightWater, DT_RIGHT);	m_Grid.SetCellDec(nDot);   m_Grid.SetCellUnit(szKgf_m3_CGS,szKgf_m3_Apply);	m_Grid.AddCell(nRow++, nCol+2, szKgf_m3_Apply, DT_LEFT);

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
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 1, 2);
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;

	long nRow = 0;
	long nRows = 0;
	long nCols = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;

	m_Grid.AddCell(nRow, 0,"����");		
	m_Grid.AddCell(nRow, 1,"����");		
	m_Grid.AddCell(nRow, 2,"���");		
	

	nRow = 1;
	m_Grid.AddCell(nRow, 0,"��\n"); 
	m_Grid.AddCell(nRow+1, 0,"��\n"); 
	m_Grid.AddCell(nRow, 1,"���ö��"); m_Grid.AddCell(nRow, 2, &pStd->m_pBasicConcInfo->m_dAlpha_UpRebar); m_Grid.SetCellDec(CELL_DEC_2); 
	m_Grid.AddCell(nRow+1, 1,"�Ϲ�ö��"); m_Grid.AddCell(nRow+1, 2, &pStd->m_pBasicConcInfo->m_dAlpha_GenBar); m_Grid.SetCellDec(CELL_DEC_2); 
	nRow = 3;
	m_Grid.AddCell(nRow, 0,"��\n");
	m_Grid.AddCell(nRow+1, 0,"��\n");
	m_Grid.AddCell(nRow+2, 0,"��\n");
	m_Grid.AddCell(nRow, 1,"�Ǻ��β� 3db�̸�, ������ 6db�̸� ����ö��");	m_Grid.AddCell(nRow, 2, &pStd->m_pBasicConcInfo->m_dBeta_Tomak1);	m_Grid.SetCellDec(CELL_DEC_2); 
	m_Grid.AddCell(nRow+1, 1,"��Ÿ����ö��"); m_Grid.AddCell(nRow+1, 2, &pStd->m_pBasicConcInfo->m_dBeta_Tomak2); m_Grid.SetCellDec(CELL_DEC_2); 
	m_Grid.AddCell(nRow+2, 1,"�������� ����ö��"); m_Grid.AddCell(nRow+2, 2, &pStd->m_pBasicConcInfo->m_dBeta_GenBar); m_Grid.SetCellDec(CELL_DEC_2); 
	nRow = 6;
	m_Grid.AddCell(nRow, 0,"r\n");
	m_Grid.AddCell(nRow+1, 0,"r\n");
	m_Grid.AddCell(nRow, 1,"D22�̻��� ö��"); m_Grid.AddCell(nRow, 2, pStd->m_nTypeUnit == UNIT_TYPE_SI ? "1.00" : "1.25", nFormat, CELL_READONLY); 
	m_Grid.AddCell(nRow+1, 1,"D19������ ö�ٰ� ����ö��"); m_Grid.AddCell(nRow+1, 2, pStd->m_nTypeUnit == UNIT_TYPE_SI ? "0.80" : "1.00", nFormat, CELL_READONLY);
	nRow = 8;
	m_Grid.AddCell(nRow, 0,"��\n");
	m_Grid.AddCell(nRow+1, 0,"��\n");
	m_Grid.AddCell(nRow, 1,"�淮��ũ��Ʈ"); m_Grid.AddCell(nRow, 2, &pStd->m_pBasicConcInfo->m_dIpSmallConc); m_Grid.SetCellDec(CELL_DEC_2); 
	m_Grid.AddCell(nRow+1, 1,"�Ϲ���ũ��Ʈ"); m_Grid.AddCell(nRow+1, 2, &pStd->m_pBasicConcInfo->m_dIpGenConc); m_Grid.SetCellDec(CELL_DEC_2); 


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
		// 0, ����, ���ڿ� �Է� �Ұ�.

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
		if(str.Find("fck : ��ũ��Ʈ ����")!=-1 || str.Find("���� ��ũ��Ʈ")!=-1)
		{
			double Wc = (pStd->m_pBasicConcInfo->m_UWeightConcrete>0) ? pStd->m_pBasicConcInfo->m_UWeightConcrete/1000 : 2.350;
			pStd->m_Ec = GetValueEcConc(pStd->m_Fck,Wc,pStd->IsSI(),pStd->m_nTypeKciCode); // ���α�������� 38������
			SetDataInit();
		}
		if(str.Find(_T("��������")) != -1 && str.Find(_T("�������")) != -1)
		{
			pStd->SetDataDefaultBaseResistFactor_SteelPile();
		}
		if(str.Find("fy  : ������ ����") != -1 || str.Find("�����ؼ�����") != -1)
		{
			SetGridData();
			SetDataInit();
		}
		if( str.Find("���α� �������") != -1)
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
				AfxGetMainWnd()->SendMessage(WM_USER+14); // ������ ���¹�
			}
			SetGridData();
			SetDataInit();
		}
		if(str.Find("���������")!=-1) 		
		{
			pStd->SetStringUnitType();
			SetGridData();
			SetDataInit();
			pStd->m_pBasicConcInfo->SetUnit(pStd->IsSI());
			SetDataDefaultRebarFactor();
			SetDataDefaultSettle();
			SetDataDefaultJoint();
		}

		if(str.Find("���������� ����")!=-1) 		
		{
			pStd->SetRailLoad();
		}
		if(str.Find("���뼳�����(��������)")!=-1)
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


		if(str.Find("��ũ��Ʈ �����������")!=-1 || str.Find("���α� �������")!=-1 || str.Find("���뼳�����")!=-1)
		{
			if(str.Find("��ũ��Ʈ �����������")!=-1)
			{
				if(pStd->m_nTypeConditionApply == DESIGN_CONDITION_ROAD && pStd->m_nTypeRoadCode == DESIGN_ROAD_2010 && pStd->m_nTypeKciCode == DESIGN_CODE_KCI_2003)
				{
					// (ARCBRIDGE-1857) 2010���� ���ý� KCI2003�� ���� �ȵǰ�
					AfxMessageBox(_T("[KCI2003]\n���������ʴ� ��������Դϴ�."), MB_OK, MB_ICONINFORMATION);
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
			pStd->m_Ec = GetValueEcConc(pStd->m_Fck,Wc,pStd->IsSI(), pStd->m_nTypeKciCode); // ���α�������� 38������

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
		if(str.Find(_T("ö��")) != -1 && str.Find(_T("�������")) != -1)
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
		if(str.Find("������ ǥ�ص�", 0) != -1)
		{
			SetGridData();
			SetDataInit();
		}
		if(str.Find("���������� ����")!=-1) 		
		{
			pStd->SetRailLoad();
		}
		if(str.Find("������")!=-1) 		
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
		if (str.Find("���������� ��(Wp)")!=-1)
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
		if(str.Find("�����������")!=-1)
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
	else if(m_nTab == m_nTabRebarFactor) //ö������, ���� �������
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
		m_Grid.SetTextMatrix(0,0,"ö���� ��������� ������� �ʾҽ��ϴ�.");
		return;
	}

	long nCountRailLoadWheel = pStd->GetCountRailLoadWheel();
	long nSizeRow = 4;
	long nSizeCol = 1 + nCountRailLoadWheel;

	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(nSizeRow, nSizeCol, 1, 1);
	m_Grid.SetColumnWidthAll(55);
	m_Grid.SetColumnWidth(0,80);

	long nRow = 0;
	long nCol = 0;
	// TITLE
	m_Grid.AddCell(nRow++,nCol,"����");
	m_Grid.AddCell(nRow++,nCol,"��������");
	m_Grid.AddCell(nRow++,nCol,"����ũ��");
	m_Grid.AddCell(nRow++,nCol,"�Ÿ�(m)");

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
	double dExtLen		= 4;	// ���ߵ��� �� ����..
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
		if(nType == 0)	// �������� (Pd)
		{			
			Dom.Circle(dDistTot, dBRadius, dBRadius);
			Dom.LineTo(dDistTot, 0, dDistTot, dSRadius*2+dExtLen);
		}
		else if(nType == 1)	// ������(P)
		{
			Dom.Circle(dDistTot, dSRadius, dSRadius);
			Dom.LineTo(dDistTot, 0, dDistTot, dSRadius*2+dExtLen);
		}
		else if(nType == 2)	// ��������(W)
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

	// �׸� �׸��� ��� ���� ��.
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

	// ��ũ �� ������ ����
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
	m_ctlStatic_Trailer3.SetWindowText(bSI? "KN(������������)" : "tonf(������������)");
	m_ctlStatic_Trailer4.SetWindowText(bSI? "KN(������������)" : "tonf(������������)");
	m_ctlStatic_Trailer5.SetWindowText(bSI? "KN(������������)" : "tonf(������������)");
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

	// �׸����� ũ�⸦ �� �ٿ��� �ȴ�.
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
		pStd->m_DistributionLoadFront	= GetValueUnitChange(atof(str), pStd->m_szTonf, UNIT_CGS_TONF);		// �������� ��������

		m_editDistributionLoadFront.GetWindowText(str);
		pStd->m_DistributionLoadBack	= GetValueUnitChange(atof(str), pStd->m_szTonf, UNIT_CGS_TONF);		// �������� ��������

		m_editRailLoadWheel.GetWindowText(str);
		pStd->m_RailLoadWheel			= GetValueUnitChange(atof(str), pStd->m_szTonf, UNIT_CGS_TONF);				// ��������

		m_editRailDistWheel.GetWindowText(str);
		pStd->m_RailDistWheel			= atof(str);				// ���

		m_editRailDistWheelMid.GetWindowText(str);
		pStd->m_RailDistWheelMid		= atof(str);			// ��� (�߰��κ�)
	}
	else
	{
		m_editRailDistWheel.GetWindowText(str);
		pStd->m_RailDistWheel			= atof(str);				// ���
	}
}

void CInputBaseDesignBasicData::SetDataInitRailLoadUser()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	if(!pStd->IsRailDesign()) return;

	CString str	= _T("");
	str.Format("%.3f", GetValueUnitChange(pStd->m_DistributionLoadFront, UNIT_CGS_TONF, pStd->m_szTonf));
	m_editDistributionLoadFront.SetWindowText(str);								// �������� ��������
	
	str.Format("%.3f", GetValueUnitChange(pStd->m_DistributionLoadBack, UNIT_CGS_TONF, pStd->m_szTonf));
	m_editDistributionLoadBack.SetWindowText(str);								// �������� ��������

	str.Format("%.3f", GetValueUnitChange(pStd->m_RailLoadWheel, UNIT_CGS_TONF, pStd->m_szTonf));
	m_editRailLoadWheel.SetWindowText(str);										// ��������

	str.Format("%.3f", pStd->m_RailDistWheel);
	m_editRailDistWheel.SetWindowText(str);										// ���

	str.Format("%.3f", pStd->m_RailDistWheelMid);
	m_editRailDistWheelMid.SetWindowText(str);									// ��� (�߰��κ�)

	UpdateData(FALSE);
}

// ���� �ڷ� ���� 
void CInputBaseDesignBasicData::OnButtonLoad() 
{
	// TODO: Add your control notification handler code here
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;

	CString sTitle	= _T("�����ڷ� �ҷ�����");
	CFileDialog dlg(TRUE, "*.bas",NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
					  "ARcBridge �����ڷ� ���� (*.bas)|*.bas||",GetParent());
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

// ���� �ڷ� ����
void CInputBaseDesignBasicData::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;

	CString sTitle	= _T("�����ڷ� ����");
	CFileDialog dlg(FALSE, "*.bas",NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
								  "ARcBridge �����ڷ� ���� (*.bas)|*.bas||",GetParent());
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
	
	// OnSzie�� InitDialog �ϱ����� �Ŀ� �ι� ���´�. InitDialog���� ������ ��Ʈ�ѿ��� ������ �߻��Ѵ�.
	// ��¥�� OnSzie�� InitDialog�Ŀ� ���ö��� m_nTab�� 0�� �Ǳ� ������ �ʿ� ���� �κ��̴�.
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
	����Ǿ� �ִ� ���Ͽ��� �Ѱ���¸� ������ �� ���� ���ϵ��� ã�� �Լ�
*@details
	Arch�� ������ �������� ����.
	��������, ���ո���
	��������� �ϰ�� = TRUE
*@return BOOL
*@author ������
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
				strUsdType = _T("Arc�� ����");
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
					strUsdType = _T("��������, SPC ����");
					break;
				}
			}

			if(strUsdType.IsEmpty() == FALSE)
				break;
		}
	}

	
	if( pStd->m_nTypeConditionApply == DESIGN_CONDITION_RAIL )		// ö���� �������
	{
		bLSDTypeOK = FALSE;
		strUsdType = "ö���� �������";
	}

	if (pStd->m_nTypeUnit == 0)
	{
		bLSDTypeOK = FALSE;
		strUsdType += "\nCGS ������";
	}

	if(bLSDTypeOK == FALSE)
	{
		CString strMessage;
		strMessage.Format(_T("�������� �ʴ� ����/��������� ����Ǿ����ϴ�.\n������� ������ ������ �����ؾ� �մϴ�.\n\n%s"), strUsdType);
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
	if(AfxMessageBox(_T("����� ����� ���ؿ��� ���õ� ������ ��������� ���� ������ �Է°� �� �ʱ�ȭ�Ǵ� ������ ������, �����ڷἳ��, ����ȯ��, ���ʺμ��� �� �Է�Ȯ���� �����ϼž� �մϴ�. \n�Է��ڷ��� ��������� ������� �� ����� ������ �Է°��� �������� �ʽ��ϴ�. \n��� �����Ͻðڽ��ϱ�?"), MB_YESNO|MB_ICONQUESTION)==IDNO)
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
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 1, 2);
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;

	long nRow = 0;
	long nRows = 0;
	long nCols = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;

	m_Grid.AddCell(nRow, 0,hggettext("����"));		
	m_Grid.AddCell(nRow, 1,hggettext("����"));		
	m_Grid.AddCell(nRow, 2,hggettext("���"));		

	nRow++;
	m_Grid.AddCell(nRow, 0,_T("n1\n")); 
	m_Grid.AddCell(nRow, 1,hggettext("�������� �������(�����ǰ��)")); 
	m_Grid.AddCell(nRow, 2, &pStd->m_plsdRebar->GetOption().dn1[0]);	m_Grid.SetCellDec(CELL_DEC_2);  m_Grid.SetLastCellMinLimit(0.1);

	nRow++;
	m_Grid.AddCell(nRow, 0,_T("n1\n")); 
	m_Grid.AddCell(nRow, 1,hggettext("�������� �������(���ö��, 30cm �̻��� ���̿� ��ġ�� ö��)")); 
	m_Grid.AddCell(nRow, 2, &pStd->m_plsdRebar->GetOption().dn1[1]);	m_Grid.SetCellDec(CELL_DEC_2);  m_Grid.SetLastCellMinLimit(0.1);

	nRow++;
	m_Grid.AddCell(nRow, 0,_T("n2\n")); 
	m_Grid.AddCell(nRow, 1,hggettext("ö������ ���� �������(32mm ����)")); 
	m_Grid.AddCell(nRow, 2, &pStd->m_plsdRebar->GetOption().dn2);	m_Grid.SetCellDec(CELL_DEC_2);  m_Grid.SetLastCellMinLimit(0.1);

	nRow++;
	m_Grid.AddCell(nRow, 0,_T("n2\n")); 
	m_Grid.AddCell(nRow, 1,hggettext("ö������ ���� �������(32mm �ʰ�, (132-db/100))")); 
	m_Grid.AddCell(nRow, 2,hggettext("-"),nFormat,CELL_READONLY); m_Grid.SetCellDec(CELL_DEC_2); 

	nRow++;
	m_Grid.AddCell(nRow, 0,_T("fr")); 
	m_Grid.AddCell(nRow, 1,hggettext("��������Mpa")); 
	m_Grid.AddCell(nRow, 2, &pStd->m_plsdRebar->GetOption().dRestraintStress);	m_Grid.SetCellDec(CELL_DEC_2);

	nRow++;
	m_Grid.AddCell(nRow, 0,_T("Ro")); 
	m_Grid.AddCell(nRow, 1,hggettext("��ħ��������(%)")); 
	m_Grid.AddCell(nRow, 2, &pStd->m_plsdRebar->GetOption().dOverRatio);	m_Grid.SetCellDec(CELL_DEC_1);

	nRow++;
	m_Grid.AddCell(nRow, 0,_T("Ras")); 
	m_Grid.AddCell(nRow, 1,hggettext("�ʿ�/���ö�ٷ���(As.req/As,.prov)")); 
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
