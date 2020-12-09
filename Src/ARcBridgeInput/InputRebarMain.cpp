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

	// ��Ʈ�� �� ����
	pBri->m_nIdxCTC_MainRebar	= m_comboCTC.GetCurSel();

	// �����༱ �и� ����
	m_pIn->m_pARcBridgeDataStd->CopyToCoupleBridge_RebarMain(m_pIn->m_nCurrentBridge);

	if(m_pDlgCheck->GetSafeHwnd() && m_pDlgCheck->IsWindowVisible())
	{
		pBri->MakeRebarInfoCycleStd(TRUE);
		m_pDlgCheck->Check();
	}

	if(m_nTab == m_nTabUpperSlab)
	{
		// ������ ��ö�ٰ� ��±� ��ǥ �ʱ�ȭ
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
			// ó�����´ٸ� Sync�� �����ְ� �⺻���� ����.
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
		
		// ó�� �ǰ���ϰ�� ��ü �ǰ��
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
			// �����༱ �и� ����
			m_pIn->m_pARcBridgeDataStd->CopyToCoupleBridge_RebarMain(m_pIn->m_nCurrentBridge);
		}

		// ������ ��ö�ٰ� ��±� ��ǥ �ʱ�ȭ
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

	// ����Ŭ ��
	CStringArray sArrTabCycleName;
	long nTab		= 0;
	m_nTabCycle1	= -1;
	m_nTabCycle2	= -1;
	m_nTabCycle3	= -1;
	m_nTabCycle4	= -1;
	sArrTabCycleName.Add("�� CYCLE"); m_nTabCycle1 = nTab++;
	if(pBri->GetCountCycleMainRebar() == 4)
	{
		sArrTabCycleName.Add("�� CYCLE"); m_nTabCycle3	= nTab++;
	}
	// 2005.01.03���� �� �Է�â������ 1 cycle�� �Է� �ޱ�� ��.
	// Ȥ�ö� �ٸ�  cycle�� �Է� �޴� ������ ������ ���Ѵٸ�
	// �ٷ� �Ʒ� �ּ��� Ǯ�� ���� ok
	/*
	sArrTabCycleName.Add("�� CYCLE"); m_nTabCycle2 = nTab++;
	if(pBri->GetCountCycleMainRebar() > 2)
	{
		sArrTabCycleName.Add("�� CYCLE"); m_nTabCycle3 = nTab++;
		sArrTabCycleName.Add("�� CYCLE"); m_nTabCycle4 = nTab++;
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
	
	m_sArrTabName.Add("��ν�����"); m_nTabUpperSlab = nTab++;
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		m_sArrTabName.Add("�Ϻν�����"); m_nTabLowerSlab = nTab++;
	}
	if(pBri->IsOutWall(TRUE) || pBri->IsOutWall(FALSE))
	{
		m_sArrTabName.Add("������ü"); m_nTabOutWall = nTab++;
	}
	if(pBri->GetCountInWall() > 0 && pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB)
	{
		if(!(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH && pBri->GetJijumConnectRibAndSlab() == 1 && pBri->GetCountInWall() == 1))
		{
			m_sArrTabName.Add("������ü"); m_nTabInWall = nTab++;
		}
	}
	if(pBri->GetCountFooting() > 0 && pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB && pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
	{
		m_sArrTabName.Add("����"); m_nTabFooting = nTab++;
	}
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		m_sArrTabName.Add("��ġ����"); m_nTabArchRib	= nTab++;
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
	// �����ϸ鼭 pick box�� ũ�⸦ ������
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
	// ����
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

	// ���� ������ �⺻������
	pBri->m_footingEnd.m_exFooting.m_Pile.InitScaleCutting();

	// ������ ġ�� ǥ�� ����?
	BOOL bViewJointDim	= m_checkJoint.GetCheck();
	
	// ���ʿ� ������ ���� �׸��� ��ü�� ���� ������ ���ش�
	// ���ʸ� �׷��� �ɶ��� �ش� ���ʰ� �纸������ �Ǵ��ϰ�, �ٸ��� �׸����� �����갡 �纸������ �Ǵ��ؼ�
	// �纸���̸� ���Ÿ��� �������ش�.
	BOOL bDrawFooting	= m_nTab == m_nTabFooting;
	BOOL bDrawBridge	= !bDrawFooting;
	long nIdxFooting	= GetIdxJijumCurFooting();
	CFootingApp *pFooting	= NULL;
	if(nIdxFooting > -1)
		pFooting	= pBri->GetFooting(nIdxFooting);
	
	// ��� ��ü���� ����ȭ�� ����.
	BOOL bVertDir	= pFooting && pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH? TRUE : pBri->IsVertDir();
	long nVertDir	= pFooting && pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH? -1 : 0;
	
	//pBri->SyncBridge(0, !bVertDir);
	pBri->GetTvVertSection_Std(0, !bVertDir, FALSE);
	pBri->SyncBridgeRebar(TRUE, bVertDir?-1:1, TRUE);
	
	// ���ܸ� �׸�
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
	
	// ��� ö�� ��ũ ǥ���϶�.
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

	// ö�� �׸���(ġ��, ���� ����)
	long nCountWall		= pBri->GetCountInWall();
	long nCountFooting	= pBri->GetCountFooting();
	long nDan			= bDimRib || pBri->m_bDetailInput23DanMainRebar ? m_comboDan.GetCurSel() : 0;
	
	// hidden == 0 : ���� ����Ŭ �׸�
	// hidden == 1 : �������� �׷��� �Ǵ� ����Ŭ �׸�
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
				
				// ������ü �Է½� ��ν������ ������ü ����� ö�� ��ũ����� ��
				if(!bDimUpperSlab && bDimOutWall)
				{
					CRebarInfoCycle *pRC	= pBri->GetRebarInfoCycleUpperSlab(nCycle, 0, TRUE, 0);
					if(pRC)
						DBStd.MarkRebarInfoCycle(&Dom, pRC, TRUE, FALSE, TRUE, 2, 0, CDRect(0, 0, 0, 0), 0.5, FALSE, &pBri->m_pArrRebarMain);

					pRC	= pBri->GetRebarInfoCycleUpperSlab(nCycle, 0, TRUE, -1);
					if(pRC)
						DBStd.MarkRebarInfoCycle(&Dom, pRC, TRUE, FALSE, TRUE, 0, 0, CDRect(0, 0, 0, 0), 0.5, FALSE, &pBri->m_pArrRebarMain);
				}
			
				// ��ü�� ö�� �׸���
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

			// (42071) �Է����� �����̰� ���������϶��� �Ŵ� ö�� ��ġ�� ���� ������ �׸��� ����.
			if(!(bDrawFooting && nVertDir == -1))
			{
				// �Ŵ� ö��
				long nJ = 0; for(nJ = 0; nJ < pBri->GetCountJijum(); nJ++)
				{
					DBStd.m_pDrawVertSection->DrawRebarAll_SlabGirder(&Dom, nJ, FALSE, FALSE, 0, FALSE, TRUE, TRUE, TRUE, TRUE, FALSE, -1, TRUE, TRUE);
					DBStd.m_pDrawVertSection->DrawRebarAll_SlabGirder(&Dom, nJ, FALSE, FALSE, 0, FALSE, TRUE, TRUE, TRUE, TRUE, FALSE, -1, TRUE, FALSE);
				}
			}

			// ���ʵ� �׸���
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

	// �ܺ� ġ�� ǥ��
	DBStd.m_pDrawVertSection->DimRebarMainAssm_VertSection_UpperSlabSide(&Dom, 0, nCycle, 0);

	// ������ �� ��ġ ǥ��(�ﰢ��)
	long nJ = 0; for(nJ = 0; nJ < pBri->GetCountJijum(); nJ++)
		DBStd.m_pDrawVertSection->DrawJijumSign(&Dom, nJ);


	// ������ ġ�� ����
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
			// ��ü�� ö�� �׸���
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

	// 2,3�� ���Է��� ��� 
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

// ����Ŭ 1, 3 �Է�(��ν����� ��,�ϸ�)
void CInputRebarMain::SetGridData_UpperSlab(long nCycle)
{
	if(!(nCycle == 0 || nCycle == 2)) return;

	// �׸��� �ʱ�ȭ
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


	// ��ν����� ��,�ϸ� ������ ���� ����/ö������
	for(long upper = 0; upper < 2; upper++)
	{
		BOOL bUpper	= upper == iUPPER;
		long nType	= bUpper ? MARK_UPPERSLAB_UPPER : MARK_UPPERSLAB_LOWER;
		pBri->GetMarkRebarMain(nType, TRUE, nCycle, TRUE, TRUE, _T(""), 0);

		// ��� ������ ��� �� ������ ���� ����
		sUpdn		= bUpper ? "��� " : "�ϸ� ";
		long nNum		= 1;
		long n = 0; for(n = 0; n < pBri->m_nQtyJigan; n++)
		{
			long i = 0; for(i = 0; i < pBri->m_nCountJointMainRebar_UpperSlab_Cycle1[n][upper][0]; i++)
			{
				if(upper == iUPPER) str.Format("UD%d", nNum++);
				else str.Format("LD%d", nNum++);

				if(i > 0) sHelp.Format("%d��° ������ %d��° ������ %d��° �������� ����", n+1, i, i+1);
				else sHelp.Format("%d��° ������ ������ġ�� %d��° �������� ����", n+1, i+1);
				sHelp = sUpdn + sHelp;

				// �ϸ��̰� ���������� �ִٸ� ���� ó���� ���� ���� �������� ö�� ���� �߰�
				if(!bUpper && pBri->IsBlockOut(TRUE) && n == 0 && i == 0)
				{
					m_Grid.AddCell(row, col, sUpdn);
					m_Grid.AddCell(row+1, col, "FXSL");
					m_Grid.AddCell(row+2, col, &pBri->m_fxJointStt.m_dLenHorRebar[nCycle][0], nFormat, nLine, "���� �������� �ϸ� ö�� ����"); 
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

				// �ϸ��̰� ���������� �ִٸ� ���� ó���� ���� ���� �������� ö�� ���� �߰�
				if(!bUpper && pBri->IsBlockOut(FALSE) && n == pBri->m_nQtyJigan-1 && i == pBri->m_nCountJointMainRebar_UpperSlab_Cycle1[n][upper][0]-1)
				{
					m_Grid.AddCell(row, col, sUpdn);
					m_Grid.AddCell(row+1, col, "FXEL");
					m_Grid.AddCell(row+2, col, &pBri->m_fxJointEnd.m_dLenHorRebar[nCycle][0], nFormat, nLine, "���� �������� �ϸ� ö�� ����"); 
					m_Grid.SetCellDec(CELL_DEC_0);
					m_Grid.SetCellPrintZero(TRUE);
					if(!bUpper) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
					col++;
				}
			}
		}

		// ��ν����� �� ������ ö�� ����
		// �� ������ ���� ���� �������� 1�� ����.
		// �׸���, �������� ������ŭ �߰��ȴ�.
		// 3cycle�� ���� �Է¹��� ����
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

					sHelp.Format("%d��° ������ %d��° ��ö�� ����", n+1, i+1);
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

					sHelp.Format("%d��° ������ %d��° ��ö�� ����", n+1, i+1);
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

// cycle 1, 3 �Է�(�������� ��ü ������)
void CInputRebarMain::SetGridData_OutWall(long nCycle)
{
	if(!(nCycle == 0 || nCycle == 2)) return;
	// �׸��� �ʱ�ȭ
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

	// ������ü ��,���� ���� ����/ö������
	for(long stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		sStt		= bStt ? "������ ��ü" : "������ ��ü";
		long nNum		= 1;
		// ���� ���
		for(long inner = 0; inner < 2; inner++)
		{
			BOOL bInner	= inner == iINNER;
			long nType	= bStt ? (bInner ? MARK_STTWALL_INNER : MARK_STTWALL_OUTTER) : (bInner ? MARK_ENDWALL_INNER : MARK_ENDWALL_OUTTER);
			sInner	= bInner ? "���� " : "��� ";

			pBri->GetMarkRebarMain(nType, TRUE, nCycle, TRUE, TRUE, _T(""), 0);

			long nSttCol = col;

			// ������ü ������ ���� ����
			nCountJoint	= pBri->m_nCountJointMainRebar_OutWall_Cycle1[stt][inner][0];
			long i = 0; for(i = 0; i < nCountJoint; i++)
			{
				if(stt == iUPPER) str.Format("SD%d", nNum++);
				else str.Format("ED%d", nNum++);

				sHelp.Format("%d��° ���� ������ ����", i+1);
				sHelp = sStt + sInner + sHelp;

				m_Grid.AddCell(row, col, sStt+"("+sInner+")");
				m_Grid.AddCell(row+1, col, str);
				m_Grid.AddCell(row+2, col, &pBri->m_pArrDistOutWall[nCycle][stt][inner][0], i, nFormat, nLine, sHelp); 
				m_Grid.SetCellDec(CELL_DEC_0);
				m_Grid.SetCellPrintZero(TRUE);
				if(!bStt) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
				col++;
			}

			// ������ü ������ ö�� ����
			// 3cycle�� ���� �Է� ���� ����
			if(nCycle == 0)
			{
				long nIndex = 0;
				nCountJoint	= bInner ? nCountJoint + 1 : nCountJoint;
				for(i = 0; i < nCountJoint; i++)
				{
					// Box���� ����ö�� ù��° ������ �Ϻν����� �ϸ�ö���̴�.
					if(pBri->IsBoxType() && bInner == FALSE && i == 0)
						continue;

					str	= pBri->GetMarkRebarMain(nType, TRUE, nCycle, FALSE, TRUE, _T(""), 0);

					sHelp.Format("%d��° ö�� ����", nIndex+1);
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
		// ���� ���� ����� �����̳� �Ÿ��� ���ؼ� array������ ��Ȯ�ϰ� ���� ��� �Ѵ�. ////////////////////
		// ����, ������ ���ڶ� ��� gridmaneger�� array�� �߰�������, ������� ���Ÿ� ������ �����Ƿ�, 
		// �����̳� �Ÿ� �����ö� -1 �ɼǿ� ���� �������� ������ ���� �߸��� ���� ������ �� �ִ�.
		CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
		if(!pBri) return;
		long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			pBri->SetDataDefaultRebarInfoCycle_Std(-1, -1, TRUE, dan, FALSE);
		//////////////////////////////////////////////////////////////////////////////////////////////////
		

		SetGridData();
		SetDataInit();
		DrawInputDomyunView(FALSE);

		// �����༱ �и� ����
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

		// �����༱ �и� ����
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

// cycle1�� 3
void CInputRebarMain::SetGridDataCycle13_Footing(long nCycle)
{
	if(!(nCycle == 0 || nCycle == 2)) return;

	// �׸��� �ʱ�ȭ
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

	// Ÿ��Ʋ ���� L1, R1, L2, R2
	CFootingApp cFooting;
	cFooting.GetMarkRebarMain(MARK_UPPER, TRUE, nCycle, TRUE, TRUE, _T(""), 0);
	m_Grid.AddCell(row, col++, "����");
	m_Grid.AddCell(row, col++, "L1");
	m_Grid.AddCell(row, col++, "L2");	// slope�϶��� ����
	m_Grid.AddCell(row, col++, "L2'");	// slope�϶��� ����
	m_Grid.AddCell(row, col++, cFooting.GetMarkRebarMain(MARK_UPPER, TRUE, nCycle, FALSE, TRUE, _T(""), 0));
	cFooting.GetMarkRebarMain(MARK_LOWER, TRUE, nCycle, TRUE, TRUE, _T(""), 0);
	m_Grid.AddCell(row, col++, "L3");
	m_Grid.AddCell(row, col++, "L4");	// slope�϶��� ����
	m_Grid.AddCell(row, col++, cFooting.GetMarkRebarMain(MARK_LOWER, TRUE, nCycle, FALSE, TRUE, _T(""), 0));

	long footing = 0; for(footing = 0; footing < nCountFooting; footing++)
	{
		double dFy(0);
		CFootingApp *pFooting = NULL;
		if(footing == 0) 
		{
			if(pBri->IsFooting(TRUE)) pFooting = &pBri->m_footingStt;
			str	= "������";

			dFy = pBri->GetValueFy(ePartFootStt);
		}
		else if(footing == nCountFooting-1)
		{
			if(pBri->IsFooting(FALSE)) pFooting = &pBri->m_footingEnd;
			str	= "������";

			dFy = pBri->GetValueFy(ePartFootEnd);
		}
		else
		{
			CWallApp *pWall	= pBri->GetInWall(nIdxWall++);
			pFooting		= &pWall->m_footing;
			str.Format("�߰�����%d", nIdxWall);

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
			// ���α���1
			m_Grid.AddCell(row, col++, &pFooting->m_dDistMainRebarVert[nCycle][upper], nFormat, nLine, sHelp); 
			m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.SetCellPrintZero(TRUE);

			// ���α���2
			m_Grid.AddCell(row, col++, &pFooting->m_dDistMainRebarVertExt[nCycle][upper], nFormat, bExt || bExt2? nLine : CELL_READONLY, sHelp); 
			m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.SetCellPrintZero(TRUE);

			// ���α���2(���2Ÿ�Կ��� �Է¹���)
			if(bUpper)
			{
				m_Grid.AddCell(row, col++, &pFooting->m_dDistMainRebarHorExt[nCycle], nFormat, bExt2 ? nLine : CELL_READONLY, sHelp); 
				m_Grid.SetCellDec(CELL_DEC_0);
				m_Grid.SetCellPrintZero(TRUE);
			}

			// ����
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

	// ��Ʈ�� �� ����
	pBri->m_dCTC_MainRebar		= baseConc.GetValueRebarCTC(m_comboCTC.GetCurSel());

	// CTC����� ���ʿ� ���ؼ��� ��ö�� ���� �ǰ�� ����
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

// ö�ٿ� ���� ����ȭ����
void CInputRebarMain::SyncRebar()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	pBri->SyncCycleMainRebar(0, 2);

	//SetGridData();	// ��Ÿ� �ϸ� ��Ŀ���� �����.
	SetDataInit();
}

long CInputRebarMain::GetCurCycle()
{
	long nCycle	= 0;
	if(m_nTabCycle == m_nTabCycle1) nCycle	= 0;
	else if(m_nTabCycle == m_nTabCycle3) nCycle	= 2;

	return nCycle;
}

// 3cycle ��� ���
void CInputRebarMain::OnButtonTypecycle3() 
{
	// TODO: Add your control notification handler code here
	CTypeCycle3Dlg dlg(m_pIn->GetRcBridgeRebar(), m_pIn->m_pARcBridgeDataStd);
	if(dlg.DoModal() == IDOK)
	{
		SetGridData();
		SetDataInit();
		DrawInputDomyunView(FALSE);

		// �����༱ �и� ����
		m_pIn->m_pARcBridgeDataStd->CopyToCoupleBridge_RebarMain(m_pIn->m_nCurrentBridge);
	}
}

// ���� �Է����� Ǫ�ø���.
// ������ ������.
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
	// �׸��� �ʱ�ȭ
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

		sWall.Format("�߰�����%d", wall+1);
		nNum		= 1;
		
		// �¿���
		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT;
			sLeft	= bLeft ? "����" : "����";
			nType	= bLeft ? MARK_LEFT : MARK_RIGHT;

			pWall->GetMarkRebarMain(nType, TRUE, nCycle, TRUE, TRUE, _T(""), 0);

			long nSttCol = col;

			// ���� ����
			nCountJoint	= pWall->m_nCountJointMainRebar_Cycle1[left][0];
			long i = 0; for(i = 0; i < nCountJoint; i++)
			{
				str.Format("WD%d", nNum++);
				
				sHelp.Format(" %d��° ���� ������ ����", i+1);
				sHelp = sWall + sLeft + sHelp;

				m_Grid.AddCell(row, col, sWall+"("+sLeft+")");
				m_Grid.AddCell(row+1, col, str);
				m_Grid.AddCell(row+2, col, &pWall->m_dArrDistRC[nCycle][left][0], i, nFormat, nLine, sHelp); 
				m_Grid.SetCellDec(CELL_DEC_0);
				m_Grid.SetCellPrintZero(TRUE); 
				if(!bLeft) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
				col++;
			}

			// ������ü ������ ö�� ����
			// 3,4cycle�� ���� �Է� ���� ����
			if(nCycle == 0 || nCycle == 1)
			{
// 				nCountJoint	= nCountJoint;
				for(i = 0; i < nCountJoint+1; i++)	// ������ �ϳ� �� �Է¹���
				{
					str	= pWall->GetMarkRebarMain(nType, TRUE, nCycle, FALSE, TRUE, _T(""), 0);

					sHelp.Format(" %d��° ö�� ����", i+1);
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

	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 2, 0);
	m_Grid.SetCellSize(20, 50);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long row		= 0, col = 0;
	CString str		= _T("");
	CString sHelp	= _T("");
	CString sUpper	= _T("");
	CString sRib	= _T("��ġ����");
	
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;
//	long nNum		= 1;
// 	long nCountJoint	= 0;
// 	long nType			= MARK_ARCHRIB_UPPER;
	
	// ���ϸ�
	for(long upper = 0; upper < 2; upper++)
	{
		BOOL bUpper	= upper == iUPPER;
		sUpper	= bUpper ? "�輱" : "����";
		long nType	= bUpper ? MARK_ARCHRIB_UPPER : MARK_ARCHRIB_LOWER;

		long nNum		= 1;
		pBri->GetMarkRebarMain(nType, TRUE, nCycle, TRUE, TRUE, _T(""), 0);

		// ���� ����
		long nCountJoint	= pBri->m_nCountJointMainRebar_ArchRib_Cycle1[upper][nDan];
		long i = 0; for(i = 0; i < nCountJoint; i++)
		{
			if(bUpper)
				str.Format("RT%d", nNum++);
			else
				str.Format("RB%d", nNum++);

						
			sHelp.Format(" %d��° ���� ������ ����", i+1);
			sHelp = sRib + sUpper + sHelp;

			m_Grid.AddCell(row, col, sRib+"("+sUpper+")");
			m_Grid.AddCell(row+1, col, str);
			m_Grid.AddCell(row+2, col, &pBri->m_dArrDistRib[nCycle][upper][nDan], i, nFormat, nLine, sHelp); 
			m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.SetCellPrintZero(TRUE); 
			if(!bUpper) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
			col++;
		}

		// ö�� ����
		// 3,4cycle�� ���� �Է� ���� ����
		if(nCycle == 0 || nCycle == 1)
		{
// 			nCountJoint	= nCountJoint;
			for(i = 0; i < nCountJoint+1; i++)	// ������ �ϳ� �� �Է¹���
			{
				str	= pBri->GetMarkRebarMain(nType, TRUE, nCycle, FALSE, TRUE, _T(""), 0);

				sHelp.Format(" %d��° ö�� ����", i+1);
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
		AfxMessageBox("����� ����� ������並 �Ͻñ� �ٶ��ϴ�.");
		return;
	}

	if(!bFemNormal || (pBri->GetTypeDesingEQ() == 1 && !bFemEQ))
	{
		AfxMessageBox("�𵨸� �ؼ��� �����Ŀ� ������並 �Ͻñ� �ٶ��ϴ�.");
		return;
	}

	if(m_pDlgCheck->GetSafeHwnd() && m_pDlgCheck->IsWindowVisible()) m_pDlgCheck->DestroyWindow();
	m_pDlgCheck->SetBriAndContent(pBri,IDD_REBAR_MAIN,0);

	m_pDlgCheck->ShowAndCheck(this);	
	CString str = _T("");
	str.Format("��ö�� ����");
	m_pDlgCheck->SetWindowText(str);	
}

// �������� ���� ����.
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


// ������ü ���� ���� ����
// nIdxInWallStd : ������ �Ǵ� ���� ��ü
void CInputRebarMain::ApplySameShapeInWall(long nIdxInWallStd)
{
	CString str	= _T("");
	str.Format("��� �߰����� ������ ��ö�� ����� %d������ �����ϰ� �����Ͻðڽ��ϱ�?", nIdxInWallStd+1);
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

// ������ ������ �ܸ� ������ ���� ����.
void CInputRebarMain::ApplySameShapeSttAndEnd()
{
	if(AfxMessageBox("������ ���� ��ö�� ����� �������� �����ϰ� �����Ͻðڽ��ϱ�?", MB_YESNO) != IDYES) return;

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
		AfxMessageBox("�𵨸� �ؼ��� �����Ŀ� ������並 �Ͻñ� �ٶ��ϴ�.");
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

		m_pProgress->SetProgressPos("�������� �غ���............", 5);

		pBri->SetConcData(TRUE,TRUE,TRUE,FALSE,TRUE);
		CARcBridgeCalcStd *pCalc = new CARcBridgeCalcStd(pBri);
		pCalc->CalculateAll();
		pCalc->m_pFooting->CalculateAll();
		delete pCalc;

		m_pProgress->SetProgressPos("��ν����� ����ȭ............", 25);
		OptimizeRebarSlab();

		m_pProgress->SetProgressPos("������ü ����ȭ............", 40);
		OptimizeRebarOutWall();

		m_pProgress->SetProgressPos("�߰���ü ����ȭ............", 55);
		OptimizeRebarMiddleWall();

		if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		{
			m_pProgress->SetProgressPos("��ġ���� ����ȭ............", 65);
			OptimizeRebarArchRib();
		}

		m_pProgress->SetProgressPos("���� ����ȭ............", 70);
		OptimizeRebarFooting();

//		CConcBasic *pConc = pBri->GetConcBasicByTitle(&pBri->m_pArrConcWall, "������ü �ϴ�(����)");

		m_pProgress->SetProgressPos("ö���Է»��� ������............", 85);
		SyncOptimizeRebarMain();
		if(pBri->m_bOptimizeRebarMatch) SetRebarSymmetry();

		pBri->MakeRebarInfoCycleStd(TRUE);
		m_pProgress->SetProgressPos("�������� �Ϸ�............", 100);
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
		// 0 : �����Ѱ�, 1 : �شܻ�Ȳ�Ѱ�
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
		// 0 : �����Ѱ�, 1 : �شܻ�Ȳ�Ѱ�
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
				if(pBri->m_nTypeStandardCTCRebarMain == 0)		// ��� ������ ����
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
		// 0 : �����Ѱ�, 1 : �شܻ�Ȳ�Ѱ�
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
		// 0 : �����Ѱ�, 1 : �شܻ�Ȳ�Ѱ�
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

// ����ȭ ����
// 1. ���Ʈ�� ���Ͽ� �������� Ȯ���ϵ��� ������ ����
//    -> �������� �̴޵ǰų� ö�ٺ� N.G�� ��� ����
//    -> �ּ����濡�� �����ؼ� �ִ��������... (2�� ������ô� �ִ����濡�� ����)
//    -> 2�� �� 2Cycle�и� �ɼ� ����
//    -> 2Cycle�и��� 1Cycleö���� ������ 2Cycle���� ũ�� ����
// 2. ��Ī ����� ��ġ�Ǵ� �ܸ鿡�� ū �������� ���� => ����.....
void CInputRebarMain::OptimizeBeam(CConcBasic *pConc, double dCtcMain, BOOL bRev, BOOL bFooting)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	double DiaArr[9] = {10,13,16,19,22,25,29,32,35};
	long nIdx1Cycle  = GetIdxDia(pBri->m_dOptimizeDiaMinimum);
	long nIdx2Cycle  = GetIdxDia(pBri->m_dOptimizeDiaMinimum);
	long nIdxMaxDia  = GetIdxDia(pBri->m_dOptimizeDiaMaximum);

	double dCtcTen = dCtcMain;
	//////////////////////////////////////////////////////////////////////////
	// STEP 1-1 : 1�� ��ġ�� üũ
	// ö�������� �ʱ�ȭ
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
	BOOL bConsider2Cycle = IsConsider2Cycle(pConc, bRev); // ��ü�߾ӹ��,��ü�ϴ�����.....
	while(1)
	{
		if(bConsider2Cycle==FALSE) Empty2Cycle(pConc);
		if(CheckBeamForOptimize(pConc)) break;
		
		if(pBri->m_bOptimizeRebarSplit)
		{
			// 1CYCLE�� ���� �����ϰ� �����ϸ� break
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
	// STEP 1-2 : 2�� ��ġ�� üũ
	// ö�������� �ʱ�ȭ
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
				// 1CYCLE�� ���� �����ϰ� �����ϸ� break
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
 

// BEAM�� ����ȭ�� �����ϸ� 1,2CYCLE������ ����.
void CInputRebarMain::OptimizeWall(CConcBasic *pConc)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	double stdSF = pBri->m_dOptimizeSafeRate/100;
	double DiaArr[9] = {10,13,16,19,22,25,29,32,35};
	long nIdx1Cycle  = GetIdxDia(pBri->m_dOptimizeDiaMinimum);
	long nIdxMaxDia  = GetIdxDia(pBri->m_dOptimizeDiaMaximum);

	//////////////////////////////////////////////////////////////////////////
	// STEP 1-1 : 1�� ��ġ�� üũ
	// ö�������� �ʱ�ȭ
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
	// STEP 1-2 : 2�� ��ġ�� üũ
	// ö�������� �ʱ�ȭ
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
		// 0 : �����Ѱ�, 1 : �شܻ�Ȳ�Ѱ�
		if(m_bDesignLsd == FALSE && nCase == 1)
			break;

		ETypeEnvComboLsd eType = nCase == 0 ? eEnvLimit : eEnvExtremeI;

		// ��� ������ ���
		pConc	= pBri->GetConcBasicByTitle(&pBri->m_pArrConcSlab[eType], "�����ܺ�");
		if(pConc)
		{
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				pBri->SetDiaMainRebarUpperSlab(0, dan, TRUE, 0, 0, pConc->m_RbT1C_DIA[dan], nCase == 1);
				pBri->SetDiaMainRebarUpperSlab(1, dan, TRUE, 0, 0, pConc->m_RbT2C_DIA[dan], nCase == 1);
			}
		}

		pConc	= pBri->GetConcBasicByTitle(&pBri->m_pArrConcSlab[eType], "�����ܺ�");
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
			sTitle.Format("�߰�����%d", jijum);
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
			pConc	= pBri->GetConcBasicByTitle(&pBri->m_pArrConcSlab[eType], "��ġ���� ���պ�");
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

		// ��� ������ �ϸ�
		for(jijum = 0; jijum < pBri->m_nQtyJigan; jijum++)
		{
			sTitle.Format("�߾Ӻ�(����%d)", jijum+1);
			pConc	= pBri->GetConcBasicByTitle(&pBri->m_pArrConcSlab[eType], sTitle);
			if(pConc)
			{
				for(dan = 0; dan < MAX_REBAR_DAN; dan++)
				{
					// ������ 1���� ö�� 2���� ��������
					// ������ 2�� �̻��̸� �� �����ο� �ִ� ö�� ���� ���� ����
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


		// ���� ��ü
		long stt(0), inner(0);
		BOOL bStt(TRUE), bInner(TRUE);
		CString sStt(""), sInner("");
		for(stt = 0; stt < 2; stt++)
		{
			bStt	= stt == iSTT;
			sStt	= bStt ? "������ü" : "������ü";
			if(!pBri->IsOutWall(bStt)) continue;

			for(inner = 0; inner < 2; inner++)
			{
				bInner	= inner == iINNER;
				sInner	= bInner ? "����" : "���";


				// ��ü �ϴ�
				sTitle.Format("%s �ϴ�(%s)", sStt, sInner);
				pConc	= pBri->GetConcBasicByTitle(&pBri->m_pArrConcWall[eType], sTitle);
				if(pConc)
				{
					for(dan = 0; dan < MAX_REBAR_DAN; dan++)
					{
						pBri->SetDiaMainRebarOutWall(0, dan, bStt, bInner, 0, pConc->m_RbT1C_DIA[dan], nCase == 1);
						pBri->SetDiaMainRebarOutWall(1, dan, bStt, bInner, 0, pConc->m_RbT2C_DIA[dan], nCase == 1);
					}
				}

				if(sInner == "���")
				{
					// ��ü ��ܺ�
					sTitle.Format("%s ��ܺ�", sStt);
					pConc	= pBri->GetConcBasicByTitle(&pBri->m_pArrConcWall[eType], sTitle);
					if(pConc)
					{
						for(dan = 0; dan < MAX_REBAR_DAN; dan++)
						{
							// �̰Ŵ� ��ν����� ����, ���� �ܺο� ����
							// �� �� ū�Ÿ� �����
							if(pConc->m_RbT1C_DIA[dan] > pBri->GetDiaMainRebarUpperSlab(0, dan, TRUE, bStt ? 0 : -1, bStt ? 0 : -1))
								pBri->SetDiaMainRebarUpperSlab(0, dan ,TRUE, bStt ? 0 : -1, bStt ? 0 : -1, pConc->m_RbT1C_DIA[dan], nCase == 1);

							if(pConc->m_RbT2C_DIA[dan] > pBri->GetDiaMainRebarUpperSlab(1, dan, TRUE, bStt ? 0 : -1, bStt ? 0 : -1))
								pBri->SetDiaMainRebarUpperSlab(1, dan ,TRUE, bStt ? 0 : -1, bStt ? 0 : -1, pConc->m_RbT2C_DIA[dan], nCase == 1);
						}
					}
				}

				// ��ü �߾Ӻ�
				sTitle.Format("%s �߾�(%s)", sStt, sInner); 
				pConc	= pBri->GetConcBasicByTitle(&pBri->m_pArrConcWall[eType], sTitle);
				if(pConc)
				{
					for(dan = 0; dan < MAX_REBAR_DAN; dan++)
					{
						// 1cycle
						long nStt	= 1;
						long nEnd	= pBri->GetCountJointMainRebarOutWall(0, dan, bStt, bInner);
						if(sInner == "���")
							nEnd --;

						long i = 0; for(i = nStt; i <= nEnd; i++)
						{
							pBri->SetDiaMainRebarOutWall(0, dan, bStt, bInner, i, pConc->m_RbT1C_DIA[dan], nCase == 1);
						}

						// 2cycle
						if(sInner == "����")
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


		// ������ü
		long nCountInWall	= pBri->GetCountInWall();
		long wall(0), left(0);
		for(wall = 0; wall < nCountInWall; wall++)
		{
			CWallApp *pWall	= pBri->GetInWall(wall);
			if(!pWall) continue;
			if(!pWall->m_bIs) continue;

			sTitle.Format("����%d", wall+2);
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

		// ����
		long footing(0), upper(0);
		BOOL bUpper(TRUE);
		CString sFront	= _T("");
		CString sTitleRev	= _T("");
		CString sFrontRev	= bUpper ? "�ձ�" : "�ޱ�";
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
				sFront	= bUpper ? "�ޱ�" : "�ձ�";
				sFrontRev	= bUpper ? "�ձ�" : "�ޱ�";
				if(footing == 0)
				{
					sTitle.Format("������(%s)", sFront);
					sTitleRev.Format("������(%s)", sFrontRev);
				}
				else if(footing == pBri->m_nQtyJigan)
				{
					sTitle.Format("������(%s)", sFront);
					sTitleRev.Format("������(%s)", sFrontRev);
				}
				else
				{
					sTitle.Format("�߰�����%d", footing);
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


				// rev�ʿ� ������ �����Ǿ����� �Ǵ��ؼ�
				// �ִٸ� 1cycle�� ���ؼ� ū�ɷ� ��Ī�����ش�.
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
				sFront	= bUpper ? "�ޱ�" : "�ձ�";
				sFrontRev	= bUpper ? "�ձ�" : "�ޱ�";
				if(footing == 0)
				{
					sTitle.Format("������(%s)", sFront);
					sTitleRev.Format("������(%s)", sFrontRev);
				}
				else if(footing == pBri->m_nQtyJigan)
				{
					sTitle.Format("������(%s)", sFront);
					sTitleRev.Format("������(%s)", sFrontRev);
				}
				else
				{
					sTitle.Format("�߰�����%d", footing);
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

				// rev�ʿ� ������ �����Ǿ����� �Ǵ��ؼ�
				// �ִٸ� 1cycle�� ���ؼ� ū�ɷ� ��Ī�����ش�.
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

	// ��ġ����
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


	// 2, 4 ����Ŭ ��ũ
	if(pBri->GetCountCycleMainRebar() == 4)
	{
		pBri->SyncCycleMainRebar(0, 2);
		pBri->SyncCycleMainRebar(1, 3);
	}
}

// �ɼ� ���ý� �¿� ö�� ��Ī���� ������ü �¿�, ������ �찢��..
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
			// ������ 
			// ��� ������ ��� �¿����ܺ�
			dDiaLeft = pBri->GetDiaMainRebarUpperSlab(nCycle, dan, TRUE, 0, 0);
			dDiaRight = pBri->GetDiaMainRebarUpperSlab(nCycle, dan, TRUE, -1, -1);

			dDiaMax = max(dDiaLeft, dDiaRight);

			pBri->SetDiaMainRebarUpperSlab(nCycle, dan, TRUE, 0, 0, dDiaMax);
			pBri->SetDiaMainRebarUpperSlab(nCycle, dan, TRUE, -1, 1, dDiaMax);

			// ������ü ���� ���
			for(long nInner=0; nInner<2; nInner++)
			{
				BOOL bInner = (nInner==iINNER)? TRUE : FALSE;
				// ������ü �ϴ�
				dDiaLeft = pBri->GetDiaMainRebarOutWall(nCycle, dan, TRUE, bInner, 0);
				dDiaRight = pBri->GetDiaMainRebarOutWall(nCycle, dan, FALSE, bInner, 0);
				dDiaMax = max(dDiaLeft, dDiaRight);

				pBri->SetDiaMainRebarOutWall(nCycle, dan, TRUE, bInner, 0, dDiaMax);
				pBri->SetDiaMainRebarOutWall(nCycle, dan, FALSE, bInner, 0, dDiaMax);

				// ��ü ö�� �������� �ϳ��� ö������ �����...
				dDiaMax = 0;
				// ���� ū Dia�� ã�´�.
				// ���� ��ü
				long nStt	= 1;
				long nEnd	= pBri->GetCountJointMainRebarOutWall(0, dan, TRUE, bInner);
				if(!bInner)
					nEnd --;
				
				long i = 0; for(i = nStt; i <= nEnd; i++)
				{
					dDiaLeft = pBri->GetDiaMainRebarOutWall(nCycle, dan, TRUE, bInner, i);
					dDiaMax = max(dDiaMax, dDiaLeft);
				}
				// ������ü
				nEnd	= pBri->GetCountJointMainRebarOutWall(0, dan, FALSE, bInner);
				if(!bInner)
					nEnd --;
				for(i = nStt; i <= nEnd; i++)
				{
					dDiaRight = pBri->GetDiaMainRebarOutWall(nCycle, dan, FALSE, bInner, i);
					dDiaMax = max(dDiaMax, dDiaRight);
				}

				// ���� ū Dia�� ��� �����.
				nEnd	= pBri->GetCountJointMainRebarOutWall(0, dan, TRUE, bInner);
				if(!bInner)
					nEnd --;
				
				for(i = nStt; i <= nEnd; i++)
				{
					pBri->SetDiaMainRebarOutWall(nCycle, dan, TRUE, bInner, i, dDiaMax);
				}
				// ������ü
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
	if(pConc->m_sTitle.Find("��ü")!=-1)
	{
		if(pConc->m_sTitle.Find("����")!=-1)
		{
			if(pConc->m_sTitle.Find("�߾�(���)")!=-1)
			{
				return pBri->IsJointMainRebar_OutWall_Cycle2(TRUE, FALSE);
				/*
				CSetRebarJoint *pSetRebarJoint = &pBri->m_pArrSetRebarJointOutWall_Outter[0];
				return pSetRebarJoint->m_bJoint;
				*/
			}
			if(pConc->m_sTitle.Find("�ϴ�(����)")!=-1)
			{
				return pBri->IsJointMainRebar_OutWall_Cycle2(TRUE, TRUE);
				/*
				CSetRebarJoint *pSetRebarJoint = &pBri->m_pArrSetRebarJointOutWall_Inner[0][1];
				return pSetRebarJoint->m_bJoint;
				*/
			}
		}

		if(pConc->m_sTitle.Find("����")!=-1)
		{
			CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
			if(pConc->m_sTitle.Find("�߾�(���)")!=-1)
			{
				return pBri->IsJointMainRebar_OutWall_Cycle2(FALSE, FALSE);
				/*
				CSetRebarJoint *pSetRebarJoint = &pBri->m_pArrSetRebarJointOutWall_Outter[1];
				return pSetRebarJoint->m_bJoint;
				*/
			}
			if(pConc->m_sTitle.Find("�ϴ�(����)")!=-1)
			{
				return pBri->IsJointMainRebar_OutWall_Cycle2(FALSE, TRUE);
				/*
				CSetRebarJoint *pSetRebarJoint = &pBri->m_pArrSetRebarJointOutWall_Inner[1][1];
				return pSetRebarJoint->m_bJoint;
				*/
			}
		}
	}
	// ����(������)
	else if(pConc->m_sTitle.Find("�ձ�", 0) != -1 || pConc->m_sTitle.Find("�ޱ�", 0) != -1)
	{
		if(!bRev) return TRUE;
		BOOL bStt	= pConc->m_sTitle.Find("����", 0) != -1;
		BOOL bFront	= pConc->m_sTitle.Find("�ձ�", 0) != -1;
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
		AfxMessageBox("�𵨸� �ؼ��� �����Ŀ� ������並 �Ͻñ� �ٶ��ϴ�.");
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
	// �׸��� �ʱ�ȭ
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

	// ������ü ��,���� ���� ����/ö������
	for(long stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		sStt		= bStt ? "������ ��ü" : "������ ��ü";
		
		// ���� ���
		for(long inner = 0; inner < 2; inner++)
		{
			BOOL bInner	= inner == iINNER;
			long nType	= bStt ? (bInner ? MARK_STTWALL_INNER : MARK_STTWALL_OUTTER) : (bInner ? MARK_ENDWALL_INNER : MARK_ENDWALL_OUTTER);
			sInner	= bInner ? "���� " : "��� ";

			pBri->GetMarkRebarMain(nType, TRUE, nCycle, TRUE, TRUE, _T(""), 0);

			// ������ü ������ ���� ����
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

	// �׸��� �ʱ�ȭ
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


	// �Ϻν����� ��,�ϸ� ������ ���� ����/ö������
	for(long upper = 0; upper < 2; upper++)
	{
		BOOL bUpper	= upper == iUPPER;
		long nType	= bUpper ? MARK_LOWERSLAB_UPPER : MARK_LOWERSLAB_LOWER;
		pBri->GetMarkRebarMain(nType, TRUE, nCycle, TRUE, TRUE, _T(""), 0);

		// �Ϻ� ������ ��� �� ������ ���� ����
		sUpdn		= bUpper ? "��� " : "�ϸ� ";
		long nNum		= 1;
		long n = 0; for(n = 0; n < pBri->m_nQtyJigan; n++)
		{
			long i = 0; for(i = 0; i < pBri->m_nCountJointMainRebar_LowerSlab_Cycle1[n][upper][0]; i++)
			{
				if(upper == iUPPER) str.Format("UD%d", nNum++);
				else str.Format("LD%d", nNum++);

				if(i > 0) sHelp.Format("%d��° ������ %d��° ������ %d��° �������� ����", n+1, i, i+1);
				else sHelp.Format("%d��° ������ ������ġ�� %d��° �������� ����", n+1, i+1);
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

		// �Ϻν����� �� ������ ö�� ����
		// �� ������ ���� ���� �������� 1�� ����.
		// �׸���, �������� ������ŭ �߰��ȴ�.
		// 3cycle�� ���� �Է¹��� ����
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

					sHelp.Format("%d��° ������ %d��° ��ö�� ����", n+1, i+1);
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

					sHelp.Format("%d��° ������ %d��° ��ö�� ����", n+1, i+1);
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

		// �����༱ �и� ����
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
