// InputBaseCrossConstruct.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "../../Src/ARcBridgeCalculate/ARcBridgeCalculate.h"
#include "../ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "GuardWallDlg.h"
#include "SlabShapeDlg.h"
#include "InputBaseCrossConstruct.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputBaseCrossConstruct dialog


CInputBaseCrossConstruct::CInputBaseCrossConstruct(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputBaseCrossConstruct::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputBaseCrossConstruct)
	m_dTS						 = 0;		// ������ �β�
	m_bLevelFrRoadCenterToEdge	 = FALSE;	// --�����߽ɰ��� ���� ����//Ⱦ�� ������ �����Է� �߰����� 20050610
	m_nQtyRail					 = 2;		// [������ ���� �Է�], ���� ���Ҽ�, ö�������� ���
	m_dDisBaseElevation			 = 0;		// F.L ������ġ(ö���� ������� ���� ��ġ)
	m_dDisBaseElevationRL		 = 0;		// --R.L ������ġ(ö���� ������� ���� ��ġ)//Ⱦ�� ������ �����Է� �߰����� 20050610
	m_dHeightRail				 = 0;		// --ö���� ���ϳ���//Ⱦ�� ������ �����Է� �߰����� 20050610
	m_dHeightDosangUnderTie		 = 0;		// ö���� �������
	m_dSlopeDosang				 = 0;		// ö���� ���� ����
	m_bUnderGroundPaveDetail	 = FALSE;		// ���� ���� ����
	m_bApplyLineSlope			 = TRUE;		// --Ⱦ��� ���� ���� ����//Ⱦ�� ������ �����Է� �߰����� 20050610
	m_dWidthRoadLeft	 = 0;		// ���� ��� �Ÿ�
	m_dWidthRoadRight	 = 0;		// ���� ��� �Ÿ�
	m_bViewAllDir = TRUE;
	m_szUnderElevationEnd = _T("");
	m_szUnderElevationStt = _T("");
	m_bViewHatch = FALSE;
	m_dDistRoadCenterLevelLeft = 0.0;
	m_dDistRoadCenterLevelRight = 0.0;
	//}}AFX_DATA_INIT
	m_nTabSlab		= -1;
	m_nTabUnderGround = -1;
	m_nTabRoad		= -1;
	m_nTabPave		= -1;
	m_nTabNori		= -1;
	m_nTabRailData	= -1;
	m_nTabRail		= -1;
	m_nTabDistribute = -1;
	m_nColSelect	= -1;

	m_bViewHatch	= TRUE;
	m_bShowSectionEnd = FALSE;
	m_bShowPlane = FALSE;
	m_pCalc	= NULL;
}

CInputBaseCrossConstruct::~CInputBaseCrossConstruct()
{
	if(m_pCalc)
	{
		delete m_pCalc;
		m_pCalc	= NULL;
	}
}

void CInputBaseCrossConstruct::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputBaseCrossConstruct)
	DDX_Control(pDX, IDC_TAB, m_ctlTab);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Text(pDX, IDC_EDIT_TS, m_dTS);											// ������ �β�
	DDX_Check(pDX, IDC_CHECK_LEVEL_ROADCENTER, m_bLevelFrRoadCenterToEdge);		// --�����߽ɰ��� ���� ����//Ⱦ�� ������ �����Է� �߰����� 20050610
	DDX_Text(pDX, IDC_HDANIN_DIS_BASE_ELEVATION, m_dDisBaseElevation);		// F.L ������ġ(ö���� ������� ���� ��ġ)
	DDX_Check(pDX, IDC_CHECK_APPLY_LINE_SLOPE, m_bApplyLineSlope);					// --Ⱦ��� ���� ���� ����//Ⱦ�� ������ �����Է� �߰����� 20050610
	DDX_Check(pDX, IDC_CHECK_VIEW_COUNTER_DIR, m_bViewAllDir);
	DDX_Control(pDX, IDC_BUTTON_GUARDWALL, m_btGuardWall);
	DDX_Control(pDX, IDC_BUTTON_SLAB_SHAPE, m_btSlabShape);
	DDX_Control(pDX, IDC_BUTTON_DELETE_COL, m_btDelete);
	DDX_Control(pDX, IDC_BUTTON_INSERT_COL, m_btAdd);
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_cbBridgeNo);
	DDX_Check(pDX, IDC_CHECK_VIEW_HATCH, m_bViewHatch);
	DDX_Text(pDX, IDC_EDIT_ROADCENTER_LEVEL_LEFT, m_dDistRoadCenterLevelLeft);
	DDX_Text(pDX, IDC_EDIT_ROADCENTER_LEVEL_RIGHT, m_dDistRoadCenterLevelRight);
	DDX_Check(pDX, IDC_CHECK_VIEW_END_CROSS, m_bShowSectionEnd);
	DDX_Check(pDX, IDC_CHECK_VIEW_PLANE, m_bShowPlane);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputBaseCrossConstruct, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputBaseCrossConstruct)
	ON_EN_KILLFOCUS(IDC_EDIT_TS, OnKillfocusEditTS)
	ON_EN_SETFOCUS(IDC_EDIT_TS, OnSetfocusEditTS)
	ON_BN_CLICKED(IDC_CHECK_LEVEL_ROADCENTER, OnCheckLevelRoadCenter)		//--
	ON_EN_KILLFOCUS(IDC_HDANIN_DIS_BASE_ELEVATION, OnKillfocusHdaninDisBaseElevation)
	ON_BN_CLICKED(IDC_CHECK_APPLY_LINE_SLOPE, OnCheckApplyLineSlope)
	ON_BN_CLICKED(IDC_BUTTON_GUARDWALL, OnButtonGuardWall)
	ON_BN_CLICKED(IDC_BUTTON_GUARDWALL_UNDER, OnButtonGuardWallUnder)
	ON_BN_CLICKED(IDC_BUTTON_SLAB_SHAPE, OnButtonSlabShape)
	ON_BN_CLICKED(IDC_CHECK_VIEW_COUNTER_DIR, OnCheckViewCounterDir)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_BN_CLICKED(IDC_BUTTON_INSERT_COL, OnButtonInsertCol)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_COL, OnButtonDeleteCol)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_VIEW_HATCH, OnCheckViewHatch)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnSelchangingTab)
	ON_EN_KILLFOCUS(IDC_EDIT_ROADCENTER_LEVEL_LEFT, OnKillfocusEditRoadcenterLevelLeft)
	ON_EN_KILLFOCUS(IDC_EDIT_ROADCENTER_LEVEL_RIGHT, OnKillfocusEditRoadcenterLevelRight)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_BN_CLICKED(IDC_CHECK_VIEW_PLANE, OnBnClickedCheckViewPlane)
	ON_BN_CLICKED(IDC_CHECK_VIEW_END_CROSS, OnBnClickedCheckViewEndCross)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputBaseCrossConstruct message handlers

BOOL CInputBaseCrossConstruct::PreTranslateMessage(MSG* pMsg) 
{
	m_Grid.ViewGridCellForCombo(pMsg);
	if( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		UpdateData();
		if(m_Grid.GetFocusCell().col == m_Grid.GetCols()-1)
			pMsg->wParam = VK_RIGHT;
		else
			pMsg->wParam = VK_TAB;
	}

	if(pMsg->hwnd==m_Grid.GetSafeHwnd() && pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
	{
		long nTab = m_ctlTab.GetCurSel();
		CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
		CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);

		CCellID next = m_Grid.GetFocusCell();
		CStringArray strArr;
		if(nTab == m_nTabSlab || nTab == m_nTabRoad)
		{
			if(next.row==3)
			{
				long nGW = nTab==0 ? 10 : 11;		// ������Ⱦ�ܿ����� ����ũ ����
				long n = 0; for(n = 0; n < nGW; n++)
					strArr.Add(GetStringTypeHDanByNum(n, pStd->IsRailDesign()));
				m_Grid.SetComboString(strArr);
			}
		}
		else if(nTab == m_nTabPave)
		{
			if(next.row==2)
			{
				if(pBri->m_bUnderGroundRahmen)
				{
					strArr.Add("�ƽ���Ʈ����");
					strArr.Add("��ũ��Ʈ����");
				}
				else
				{
					strArr.Add("ASCON ����");
					strArr.Add("LMC ����");
				}
				m_Grid.SetComboString(strArr);
			}
		}
		else if (nTab == m_nTabRailData)
		{
			if(next.row==1)
			{
				for (HGINT32 ix = 1; ix < 10; ++ix)
				{
					CString str;
					str.Format("%d ��", ix);
					
					strArr.Add(str);
				}

				m_Grid.SetComboString(strArr);
			}
		}
		else if(nTab == m_nTabRail)
		{
			if(next.row==2)
			{
				long n = 0; for(n = 0; n < pStd->m_pLineApp->GetLinesu();n++)
				{
					CLineInfo* pLine = pStd->m_pLineApp->GetLine(n);
					strArr.Add(pLine->GetNameLineEach());				
				}
				m_Grid.SetComboString(strArr);
			}
		}

	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CInputBaseCrossConstruct::OnPreInitDialog()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	CRcBridgeRebar *pRebar = dynamic_cast<CRcBridgeRebar*>(pBri);

	if(m_pCalc)
	{
		delete m_pCalc;
		m_pCalc	= NULL;
	}

	m_pCalc = new CARcBridgeCalcStd(pRebar);

	pRebar->SetConcData(TRUE, FALSE, FALSE, FALSE, FALSE);
	m_pCalc->m_pARoadOptionStd = m_pIn->m_pARoadOptionStd;
	m_pCalc->m_pView		   = m_pView;
//	m_pCalc->CalculateAll();

	SetResize(IDC_TAB, SZ_TOP_LEFT,	SZ_BOTTOM_RIGHT);

	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_INSERT_COL, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_DELETE_COL, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_SLAB_SHAPE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_GUARDWALL, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_GUARDWALL_UNDER, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	//Ⱦ�� ���� �׷� �ڽ�
	SetResize(IDC_STATIC_THICK,	  SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_TS,	  SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_TS,		  SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_CHECK_LEVEL_ROADCENTER,		  SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_CHECK_APPLY_LINE_SLOPE	, SZ_TOP_LEFT, SZ_TOP_LEFT);	
	SetResize(IDC_STATIC_RAIL_FL				, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_HDANIN_DIS_BASE_ELEVATION		, SZ_TOP_LEFT, SZ_TOP_LEFT);

	// �����߽ɿ� ���� �����Ÿ�
	SetResize(IDC_STATIC_ROADCENTER_LEVEL_LEFT			, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_ROADCENTER_LEVEL_RIGHT			, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_ROADCENTER_LEVEL_LEFT			, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_ROADCENTER_LEVEL_RIGHT			, SZ_TOP_LEFT, SZ_TOP_LEFT);
	
	SetResize(IDC_CHECK_VIEW_COUNTER_DIR, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_VIEW_HATCH, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_VIEW_END_CROSS, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_VIEW_PLANE, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_VIEW, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetHyperHelp(m_pIn->GetManualPath());

	InitComboCtrl();
	InitTabCtrl();
	m_Grid.InitGrid();
	SetDataInit();
	SetDataSave();

	// �������� �� �ʺ�� Ⱦ�� ������ �Է±��̸� �����ش�. (10377)
	long nJijum = 0;
	double sta = pBri->GetStationOnJijum(nJijum);
	CDPoint vAng = CDPoint(0,1);
	if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pBri->GetJijum(0)->m_dLengthUgan[1];
	if(pBri->m_bIsExp || pBri->IsExtLine())
	{
		sta = pBri->GetStationBridgeMid();
	}

	double dTotal = pBri->GetWidthSlabAct(sta, vAng);
	double dSum = 0;
	long nSize = pBri->GetQtyHDan(FALSE);
	long i = 0; for(i = 0; i < nSize; i++)
	{
		dSum += pBri->GetLengthHDan(i);
	}

	double dLast = pBri->GetLengthHDan(nSize - 1);
	pBri->SetLengthHDan(nSize - 1,  dLast + (dTotal - dSum));

	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		pBri->m_bUnderGroundRahmen = FALSE;
		pBri->m_bCalcOpen	= FALSE;
		m_bViewHatch = FALSE;
	}

	// (ARCBRIDGE-3675) ������ �и��϶���.
	GetDlgItem(IDC_CHECK_LEVEL_ROADCENTER)->EnableWindow(pBri->m_nSeparBri == 0 ? FALSE : TRUE);
}

// ��ü ���� ǥ���� ���� ���� ǥ��
void CInputBaseCrossConstruct::InitComboCtrl()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);

	long nSize = pStd->GetBridgeSize();

	CString str = _T("");
	long i=0; for(i=0; i<nSize; i++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(i);
		str = pStd->GetStringBridgeOrder(i) + _T(" : ") + pBri->m_strBridgeName
			+ _T(" [") + pBri->GetStringBridgeType() + _T("]");
		m_cbBridgeNo.AddString(str);
	}
	m_cbBridgeNo.SetCurSel(m_pIn->m_nCurrentBridge);
}

void CInputBaseCrossConstruct::InitTabCtrl()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	long nSelectOld = m_ctlTab.GetCurSel();

	m_ctlTab.RemoveAllTabs();
	m_ctlTab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_ctlTab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	long nTab = 0;
	m_nTabSlab = nTab++;
	m_ctlTab.InsertItem(m_nTabSlab,"������Ⱦ��");
	if(pBri->m_bUnderGroundRahmen)
	{
		m_nTabUnderGround = nTab++;
		m_ctlTab.InsertItem(m_nTabUnderGround,"���߶�౳");
		m_nTabRoad = nTab++;
		if(pStd->IsRailDesign())	m_ctlTab.InsertItem(m_nTabRoad,"ö��Ⱦ��");
		else						m_ctlTab.InsertItem(m_nTabRoad,"����Ⱦ��");
		m_nTabNori = nTab++;
		m_ctlTab.InsertItem(m_nTabNori,"������");
		m_nTabPave = nTab++;
		m_ctlTab.InsertItem(m_nTabPave,"��������");
	}
	else
	{
		m_nTabUnderGround = -1;
		m_nTabRoad = -1;
		m_nTabNori = -1;
		m_nTabPave = nTab++;
		m_ctlTab.InsertItem(m_nTabPave,"��������");
	}
	if(pStd->IsRailDesign())
	{
		m_nTabRailData = nTab++;
		m_ctlTab.InsertItem(m_nTabRailData,"ö������");

		m_nTabRail = nTab++;
		m_ctlTab.InsertItem(m_nTabRail,"������ġ");

		if(!pBri->m_bUnderGroundRahmen)
		{
			m_nTabDistribute = nTab++;
			m_ctlTab.InsertItem(m_nTabDistribute,"������");
		}
	}

	if(nSelectOld>=0 && nSelectOld<nTab)
	{
		m_ctlTab.SetCurSel(nSelectOld);
	}
}
void CInputBaseCrossConstruct::SetGridTitle()
{
	long nTab = m_ctlTab.GetCurSel();

	if(nTab == m_nTabSlab)			SetGridTitleSlab();
	else if(nTab == m_nTabUnderGround) SetGridTitleUnderGround();
	else if(nTab == m_nTabRoad)		SetGridTitleRoad();
	else if(nTab == m_nTabNori)		SetGridTitleNori();
	else if(nTab == m_nTabPave)		SetGridTitlePave();
	else if(nTab == m_nTabRailData) SetGridTitleRailData();
	else if(nTab == m_nTabRail)		SetGridTitleRail();
	else if(nTab == m_nTabDistribute)	SetGridTitleDistribute();
}
//������ Ⱦ��
void CInputBaseCrossConstruct::SetGridTitleSlab()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	BOOL bLsd = pStd->IsLsdDesign();
	long nRows = bLsd? 6 : 5;
	long nCols = pBri->GetQtyHDan(FALSE) + 1;
	
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowCount(0);
	m_Grid.SetColumnCount(0);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(120);
	m_Grid.SetColumnWidth(0, 80);

	m_Grid.SetTextBkColor(CLR_BACK);

	long nRow = 0;
	long nCol = 1;
	CString str = _T("");
	for(nCol=1; nCol<nCols; nCol++)
	{
		str.Format("L%d", nCol);
		m_Grid.SetTextMatrix(0, nCol, str);
		if(!pBri->m_bUnderGroundRahmen && pBri->m_bApplyLineSlope)
		{
			m_Grid.SetCellType(2, nCol, CT_CHECKBOX);
			m_Grid.SetTextMatrix(2, nCol, "");
		}
		else
			m_Grid.SetCellType(2, nCol, CT_EDIT);
		m_Grid.SetCellType(3, nCol, CT_COMBO);
		m_Grid.SetCellType(4, nCol, CT_CHECKBOX);
		if(bLsd)
			m_Grid.SetCellType(5, nCol, CT_CHECKBOX);
	}

	nRow = 0;
	m_Grid.SetTextMatrix(nRow++, 0, _T("����"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("����(mm)"));
	if(!pBri->m_bUnderGroundRahmen && pBri->m_bApplyLineSlope)
		m_Grid.SetTextMatrix(nRow++, 0, _T("���� ����"));
	else
		m_Grid.SetTextMatrix(nRow++, 0, _T("Ⱦ���(%)"));

	m_Grid.SetTextMatrix(nRow++, 0, _T("����"));
	if(pStd->IsLsdDesign())
	{
		m_Grid.SetTextMatrix(nRow++, 0, _T("��������"));
	}
	m_Grid.SetTextMatrix(nRow++, 0, _T("��������"));
}

void CInputBaseCrossConstruct::SetGridTitleUnderGround()
{
	long nRows = 5;
	long nCols = 3;

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowCount(0);
	m_Grid.SetColumnCount(0);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount(2);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(100);
	m_Grid.SetColumnWidth(1, 150);

	m_Grid.SetTextBkColor(CLR_BACK);

	long nRow = 0;
	m_Grid.SetTextMatrix(nRow, 0, _T("����"));
	m_Grid.SetTextMatrix(nRow, 1, _T("����"));
	m_Grid.SetMergeCol(nRow, 0,1);
	m_Grid.SetTextMatrix(nRow++, 2, _T("�Է°�"));

	m_Grid.SetTextMatrix(nRow, 0, _T("���䵵��Ⱦ��"));
	m_Grid.SetTextMatrix(nRow++, 1, _T("���� ��� �Ÿ� DL(mm)"));

	m_Grid.SetTextMatrix(nRow, 0, _T("���䵵��Ⱦ��"));
	m_Grid.SetTextMatrix(nRow++, 1, _T("���� ��� �Ÿ� DR(mm)"));
	m_Grid.SetMergeRow(1, 2, 0);

	m_Grid.SetTextMatrix(nRow, 0, _T("����������"));
	m_Grid.SetTextMatrix(nRow++, 1, _T("���� Elevation ES(m)"));

	m_Grid.SetTextMatrix(nRow, 0, _T("����������"));
	m_Grid.SetTextMatrix(nRow++, 1, _T("���� Elevation EE(m)"));
	m_Grid.SetMergeRow(3, 4, 0);
}

// ���߶���� ����Ⱦ��, ö��Ⱦ��
void CInputBaseCrossConstruct::SetGridTitleRoad()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	long nRows = 5;
	long nCols = pBri->GetQtyHDan(TRUE) + 1;

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowCount(0);
	m_Grid.SetColumnCount(0);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(100);
	m_Grid.SetColumnWidth(0, 80);

	m_Grid.SetTextBkColor(CLR_BACK);

	long nRow = 0;
	long nCol = 1;
	CString str = _T("");
	for(nCol=1; nCol<nCols; nCol++)
	{
		str.Format("L%d", nCol);
		m_Grid.SetTextMatrix(0, nCol, str);
		if(pBri->m_bApplyLineSlope)
			m_Grid.SetCellType(2, nCol, CT_CHECKBOX);
		else
			m_Grid.SetCellType(2, nCol, CT_EDIT);
		m_Grid.SetCellType(3, nCol, CT_COMBO);
		m_Grid.SetCellType(4, nCol, CT_CHECKBOX);
	}

	nRow = 0;
	m_Grid.SetTextMatrix(nRow++, 0, _T("����"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("����(mm)"));
	if(pBri->m_bApplyLineSlope)
		m_Grid.SetTextMatrix(nRow++, 0, _T("���� ����"));
	else
		m_Grid.SetTextMatrix(nRow++, 0, _T("Ⱦ���(%)"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("����"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("��������"));
}
//������
void CInputBaseCrossConstruct::SetGridTitleNori()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	long nRows = 3;
	long nCols = 5;

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowCount(0);
	m_Grid.SetColumnCount(0);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount(2);
	m_Grid.SetFixedColumnCount(0);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(100);

//	long nRow = 0;
//	long nCol = 1;
	CString str = _T("");
/*	for(nCol=1; nCol<nCols; nCol++)
	{
		for(nRow=1; nRow<nRows; nRow++)
		{
			m_Grid.SetCellType(nRow, nCol, CT_EDIT);
			m_Grid.SetTextMatrix(nRow, nCol, str);
		}
	}
*/	m_Grid.SetTextBkColor(CLR_BACK);

	m_Grid.SetTextMatrix(0, 0, _T("���α���(1:n)"));
	m_Grid.SetTextMatrix(0, 2, _T("�Ҵܺ�"));
	m_Grid.SetTextMatrix(1, 0, _T("1��"));
	m_Grid.SetTextMatrix(1, 1, _T("2���̻�"));
	m_Grid.SetTextMatrix(1, 2, _T("�ִ����(mm)"));
	m_Grid.SetTextMatrix(1, 3, _T("�Ҵ���(mm)"));
	m_Grid.SetTextMatrix(1, 4, _T("�Ҵܰ��(%)"));
	m_Grid.SetMergeCol(0,0,1);
	m_Grid.SetMergeCol(0,2,4);
}
//���� ����
void CInputBaseCrossConstruct::SetGridTitlePave()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	long nRows = (pBri->m_bUnderGroundRahmen && pBri->m_bUnderGroundPaveDetail)? 5 : 4;
	long nCols = 0;
	if(pStd->IsRailDesign())
		nCols = 2;
	else
	{
		nCols = 3;
		if(pBri->m_bUnderGroundRahmen)
			nCols++;
	}

	if (pBri->m_bUnderGroundRahmen) // ��������
	{
		nCols++;
	}

	if(pBri->m_bUnderGroundRahmen && pBri->m_bUnderGroundPaveDetail)
		nCols += 7;
	else
	{
		if(pBri->m_bPrevApplyCalcOpen)
			nCols++;
	}

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowCount(0);
	m_Grid.SetColumnCount(0);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount(2);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(80);
	m_Grid.SetColumnWidth(0,110);
	m_Grid.SetColumnWidth(1,110);

	m_Grid.SetTextBkColor(CLR_BACK);

	long nCol = 0;
	CString str = _T("");

	str.Format("�����߷�(%s)",pBri->m_pARcBridgeDataStd->m_szKgf_M3);
	m_Grid.SetTextMatrix(0, nCol, _T("��  ��"));
	m_Grid.SetMergeRow(0,1,nCol);
	m_Grid.SetTextMatrix(2, nCol, _T("����(mm)"));
	m_Grid.SetTextMatrix(3, nCol++, str);

	if(pStd->IsRailDesign())
	{
		m_Grid.SetTextMatrix(0, nCol, _T("�����Ż"));
		m_Grid.SetMergeRow(0,1,nCol++);
	}
	else
	{
		m_Grid.SetTextMatrix(0, nCol, _T("��������"));
		m_Grid.SetMergeCol(0,nCol,nCol+1);
		m_Grid.SetTextMatrix(1, nCol, _T("����"));
		m_Grid.SetCellType(2, nCol++, CT_COMBO);
		m_Grid.SetTextMatrix(1, nCol++, _T("�β�"));

		if(pBri->m_bUnderGroundRahmen)
		{
			m_Grid.SetTextMatrix(0, nCol, _T("�����Ż"));
			m_Grid.SetMergeRow(0,1,nCol++);
		}
	}

	if (pBri->m_bUnderGroundRahmen)
	{
		m_Grid.SetTextMatrix(0, nCol, _T("��������"));
		m_Grid.SetMergeRow(0,1,nCol);
		m_Grid.SetCellType(2, nCol++, CT_CHECKBOX);
	}

	if(pBri->m_bUnderGroundRahmen && pBri->m_bUnderGroundPaveDetail)
	{
		m_Grid.SetTextMatrix(4, 0, "������(m)");

		long nSttCol = nCol;
		if(pStd->IsRailDesign())
			str.Format("������");
		else
			str.Format("��������");
		m_Grid.SetTextMatrix(0, nCol, str);
		m_Grid.SetTextMatrix(1, nCol++, _T("ǥ��"));
		m_Grid.SetTextMatrix(1, nCol++, _T("����"));
		m_Grid.SetTextMatrix(1, nCol++, _T("�߰���"));
		m_Grid.SetTextMatrix(1, nCol++, _T("��������"));
		m_Grid.SetTextMatrix(1, nCol++, _T("���������"));
		m_Grid.SetTextMatrix(1, nCol++, _T("EPS��"));
		m_Grid.SetTextMatrix(1, nCol, _T("���"));
		m_Grid.SetMergeCol(0,nSttCol,nCol);
	}
	else
	{
		// 2006.10.16 - ������ ���߶�༱���ϵ��� �ϰ� �����ϱ��...
		if(pBri->m_bPrevApplyCalcOpen)
		{
			m_Grid.SetTextMatrix(0, nCol, _T("�� ��"));
			m_Grid.SetMergeRow(0,1,nCol);
		}
	}
}

void CInputBaseCrossConstruct::SetGridTitleRailData()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	long nRows = 6;
	long nCols = 2;

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowCount(0);
	m_Grid.SetColumnCount(0);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(80);
	m_Grid.SetColumnWidth(0, 130);

	m_Grid.SetTextBkColor(CLR_BACK);

	m_Grid.SetCellType(1,1,CT_COMBO);

	long nRow = 0;
	nRow = 0;
	m_Grid.SetTextMatrix(nRow, 0, _T("����"));
	m_Grid.SetTextMatrix(nRow++, 1, _T("�Է°�"));

	m_Grid.SetTextMatrix(nRow, 0, _T("ö�� ���� ����"));
	m_Grid.SetTextMatrix(nRow++, 1, _T(""));

	m_Grid.SetTextMatrix(nRow, 0, _T("�˵� �����߷�(kN/m)"));
	m_Grid.SetTextMatrix(nRow++, 1, _T(""));

	m_Grid.SetTextMatrix(nRow, 0, _T("R.L ���� ���� (mm)"));
	m_Grid.SetTextMatrix(nRow++, 1, _T(""));

	m_Grid.SetTextMatrix(nRow, 0, _T("���� ���� HR (mm)"));
	m_Grid.SetTextMatrix(nRow++, 1, _T(""));

	m_Grid.SetTextMatrix(nRow, 0, _T("���� ���� HD (mm)"));
	m_Grid.SetTextMatrix(nRow++, 1, _T(""));

	m_Grid.SetTextMatrix(nRow, 0, _T("���� �ڰ� ��� HS 1 :"));
	m_Grid.SetTextMatrix(nRow++, 1, _T(""));
}

//��������
void CInputBaseCrossConstruct::SetGridTitleRail()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	long nRows = 3;
	long nCols = pBri->GetQtyRailNode()+1;

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowCount(0);
	m_Grid.SetColumnCount(0);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(80);
	m_Grid.SetColumnWidth(0, 120);

	m_Grid.SetTextBkColor(CLR_BACK);

	long nRow = 0;
//	long nCol = 1;
	CString str = _T("");
/*	for(nCol=1; nCol<nCols; nCol++)
	{
		for(nRow=1; nRow<nRows; nRow++)
		{
			m_Grid.SetCellType(nRow, nCol, CT_EDIT);
			m_Grid.SetTextMatrix(nRow, nCol, str);
		}
	}
*/	long n=0; for(n=1; n<nCols; n++)
	{
		str.Format("C%d",n);
		m_Grid.SetTextMatrix(0,n,str);		
		m_Grid.SetCellType(2,n,CT_COMBO);
	}

	nRow = 0;
	m_Grid.SetTextMatrix(nRow++, 0, _T("����"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("�����߽ɰ���(mm)"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("���ؼ���"));
}

//������
void CInputBaseCrossConstruct::SetGridTitleDistribute()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	long nRows = 2;
	long nCols = 5;

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowCount(0);
	m_Grid.SetColumnCount(0);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(80);
	
	m_Grid.SetTextBkColor(CLR_BACK);

	long nRow = 0;
	long nCol = 0;
	m_Grid.SetTextMatrix(nRow, nCol++, _T("b"));
	m_Grid.SetTextMatrix(nRow, nCol++, _T("H1"));
	m_Grid.SetTextMatrix(nRow, nCol++, _T("d"));
	m_Grid.SetTextMatrix(nRow, nCol++, _T("g"));
	m_Grid.SetTextMatrix(nRow, nCol++, _T("h"));
}

void CInputBaseCrossConstruct::SetDataInit()
{
	m_Grid.SetGridDefault();
	SetGridTitle();

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	BOOL bRail = pStd->IsRailDesign();
	long nTab = m_ctlTab.GetCurSel();

	m_dTS						 = pBri->m_dTS;		// ������ �β�

	if(pBri->m_nSeparBri == 0)
		m_bLevelFrRoadCenterToEdge = FALSE;
	else if(pBri->m_bUnderGroundRahmen && nTab == m_nTabSlab)
		m_bLevelFrRoadCenterToEdge	= pBri->m_bLevelFrRoadCenterToEdgeOfUnderGroundRahmenSlab;
	else
		m_bLevelFrRoadCenterToEdge	 = pBri->m_bLevelFrRoadCenterToEdge;		// --�����߽ɰ��� ���� ����//Ⱦ�� ������ �����Է� �߰����� 20050610


	// ö���� ������ �����Է�
	m_nQtyRail					 = pBri->GetQtyRailNode()-1;		// [������ ���� �Է�], ���� ���Ҽ�, ö�������� ���
	m_dUnitWeightRail			= GetValueUnitChange(pBri->m_dUnitWeightRail, UNIT_CGS_TONF_M, pStd->m_szTonf_M);
	m_dDisBaseElevation			 = pBri->m_dDisBaseElevation;		// F.L ������ġ(ö���� ������� ���� ��ġ)
	m_dDisBaseElevationRL		 = pBri->m_dDisBaseElevationRL;		// --R.L ������ġ(ö���� ������� ���� ��ġ)//Ⱦ�� ������ �����Է� �߰����� 20050610
	m_dHeightRail				 = pBri->m_dHeightRail;		// --ö���� ���ϳ���//Ⱦ�� ������ �����Է� �߰����� 20050610
	m_dHeightDosangUnderTie		 = pBri->m_dHeightDosangUnderTie;		// ö���� �������
	m_dSlopeDosang				 = pBri->m_dSlopeDosang;		// ö���� ���� ����

	// ���߶�౳ ������ �����Է�
	m_bUnderGroundPaveDetail	 = pBri->m_bUnderGroundPaveDetail;		// ���� ����
	m_bApplyLineSlope			 = pBri->m_bApplyLineSlope;		// --Ⱦ��� ���� ���� ����//Ⱦ�� ������ �����Է� �߰����� 20050610

	m_dWidthRoadLeft	 = pBri->m_dWidthRoadLeft;		// ���� ��� �Ÿ�
	m_dWidthRoadRight	 = pBri->m_dWidthRoadRight;		// ���� ��� �Ÿ�
	m_szUnderElevationStt.Format("%.3f",pBri->m_dUnderGroundCenterELStt);// ��������EL (���ܽ����� EL)
	m_szUnderElevationEnd.Format("%.3f",pBri->m_dUnderGroundCenterELEnd);// ��������EL (���������� EL)

	m_bViewAllDir				 = m_pIn->m_bViewAllDir;	//�����༱ ��� ǥ��

	m_dDistRoadCenterLevelLeft	= pBri->m_dDistRoadCenterLevelLeft;
	m_dDistRoadCenterLevelRight	= pBri->m_dDistRoadCenterLevelRight;

//	GetDlgItem(IDC_CHECK_APPLY_LINE_SLOPE				)->EnableWindow   (!bRail);
	GetDlgItem(IDC_CHECK_APPLY_LINE_SLOPE)->SetWindowText("�������� ���� ���� ����");
	if(pBri->m_bUnderGroundRahmen)
	{
		if(bRail)
			GetDlgItem(IDC_CHECK_APPLY_LINE_SLOPE)->SetWindowText("ö��Ⱦ�� ���� ���� ����");
		else
			GetDlgItem(IDC_CHECK_APPLY_LINE_SLOPE)->SetWindowText("����Ⱦ�� ���� ���� ����");
	}
	
	GetDlgItem(IDC_CHECK_VIEW_HATCH)->EnableWindow((pBri->m_nTypeBridge==BRIDGE_TYPE_BOX)? FALSE : TRUE);
	GetDlgItem(IDC_BUTTON_GUARDWALL_UNDER		)->EnableWindow   (pBri->m_bUnderGroundRahmen);
	if(bRail)
		GetDlgItem(IDC_BUTTON_GUARDWALL_UNDER		)->SetWindowText("ö�� �μ� ������");
	else
		GetDlgItem(IDC_BUTTON_GUARDWALL_UNDER		)->SetWindowText("���� �μ� ������");

	GetDlgItem(IDC_EDIT_ROADCENTER_LEVEL_LEFT)->EnableWindow(m_bLevelFrRoadCenterToEdge);
	GetDlgItem(IDC_EDIT_ROADCENTER_LEVEL_RIGHT)->EnableWindow(m_bLevelFrRoadCenterToEdge);

	if(nTab == m_nTabSlab)			SetDataInitSlab();
	else if(nTab == m_nTabUnderGround) SetDataInitUnderGround();
	else if(nTab == m_nTabRoad)		SetDataInitRoad();
	else if(nTab == m_nTabNori)		SetDataInitNori();
	else if(nTab == m_nTabPave)		SetDataInitPave();
	else if(nTab == m_nTabRailData) SetDataInitRailData();
	else if(nTab == m_nTabRail)		SetDataInitRail();
	else if(nTab == m_nTabDistribute)	SetDataInitDistribute();
	
	m_Grid.SetRedraw(TRUE,TRUE);
	UpdateData(FALSE);
}
void CInputBaseCrossConstruct::SetDataInitSlab()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	CString str = _T("");
	double sta = pBri->GetStationOnJijum(0);
	if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pBri->GetJijum(0)->m_dLengthUgan[1];
	if(pBri->m_bIsExp || pBri->IsExtLine())
	{
		sta = pBri->GetStationBridgeMid();
	}

	CDPoint vAng = CDPoint(0, 1);
	long nCols = pBri->GetQtyHDan(FALSE)+1;
	BOOL bRoadNotSlab = FALSE;
	BOOL bLsd = pStd->IsLsdDesign();
	
	for(long nCol=1; nCol<nCols; nCol++)
	{
		long nRow = 1;
		long nHDan = nCol - 1;
		CGuardWallRC *pGW = pBri->GetGuardWallByHDan(nHDan);
		if(pGW==NULL) continue;

		m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(1, nCol) & ~GVIS_READONLY);
		m_Grid.SetItemBkColour(nRow, nCol, CLR_BACK);
		m_Grid.SetItemFgColour(nRow, nCol);
		
		str.Format("%.f", pBri->GetLengthHDanAct(sta, vAng, nHDan,bRoadNotSlab));
		m_Grid.SetTextMatrix(nRow++, nCol, str);
		m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(1, nCol) & ~GVIS_READONLY);
		m_Grid.SetItemBkColour(nRow, nCol, CLR_BACK);
		m_Grid.SetItemFgColour(nRow, nCol);

		if(!pBri->m_bUnderGroundRahmen && pBri->m_bApplyLineSlope)
		{
			m_Grid.SetCheck(nRow++, nCol, pBri->IsLevelHDanApplyLineSlope(nHDan,bRoadNotSlab));
		}
		else
		{
			str.Format("%.3f", pBri->GetSlopeHDanByStation(sta, nHDan,bRoadNotSlab)*100);
			m_Grid.SetTextMatrix(nRow++, nCol, str);
		}
		m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(1, nCol) & ~GVIS_READONLY);
		m_Grid.SetItemBkColour(nRow, nCol, CLR_BACK);
		m_Grid.SetItemFgColour(nRow, nCol);

		str = pBri->GetStringTypeHDan(nHDan, FALSE, bRoadNotSlab);
		m_Grid.SetTextMatrix(nRow++, nCol, str);
		
		if(bLsd)
		{
			if(pGW->IsPaveOnHDan() == FALSE)
			{
				m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(1, nCol) & ~GVIS_READONLY);
				m_Grid.SetItemBkColour(nRow, nCol, CLR_BACK);
				m_Grid.SetItemFgColour(nRow, nCol);
				m_Grid.SetTextMatrix(nRow, nCol, pGW->m_bDWload? _T("DW") : _T("DC"));
				m_Grid.SetCheck(nRow++, nCol, pGW->m_bDWload);	
			}
			else
			{
				m_Grid.SetItemState(nRow, nCol, GVIS_READONLY);
				m_Grid.SetItemBkColour(nRow, nCol, CLR_READONLY);
				m_Grid.SetItemFgColour(nRow, nCol, CLR_READONLY);
				m_Grid.SetCellType(nRow, nCol, CT_EDIT);
				m_Grid.SetTextMatrix(nRow++, nCol, _T(""));
			}
		}

		if(pGW->IsTypeHDanOnlyNone())
		{
			m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(1, nCol) & ~GVIS_READONLY);
			m_Grid.SetItemBkColour(nRow, nCol, CLR_BACK);
		}
		else
		{
			m_Grid.SetItemState(nRow, nCol, GVIS_READONLY);
			m_Grid.SetItemBkColour(nRow, nCol, CLR_READONLY);
		}
		m_Grid.SetItemFgColour(nRow, nCol);
		m_Grid.SetCheck(nRow++, nCol, pBri->IsVariableLenHDan(nHDan, bRoadNotSlab));
	}
}

void CInputBaseCrossConstruct::SetDataInitUnderGround()
{
	long nRow = 1;
	long nCol = 2;

	CString str;
	str.Format("%g", m_dWidthRoadLeft);
	m_Grid.SetTextMatrix(nRow++, nCol, str);

	str.Format("%g", m_dWidthRoadRight);
	m_Grid.SetTextMatrix(nRow++, nCol, str);

	str.Format("%s", m_szUnderElevationStt);
	m_Grid.SetTextMatrix(nRow++, nCol, str);

	str.Format("%s", m_szUnderElevationEnd);
	m_Grid.SetTextMatrix(nRow++, nCol, str);	
}

void CInputBaseCrossConstruct::SetDataInitRoad()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	CString str = _T("");
	double sta = pBri->GetStationOnJijum(0);
	CDPoint vAng = CDPoint(0,1);
	if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pBri->GetJijum(0)->m_dLengthUgan[1];
	if(pBri->m_bIsExp || pBri->IsExtLine())
	{
		sta = pBri->GetStationBridgeMid();
	}

	long nCols = pBri->GetQtyHDan(TRUE)+1;
	BOOL bRoadNotSlab = TRUE;
	for(long nCol=1; nCol<nCols; nCol++)
	{
		long nRow = 1;
		m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(1, nCol) & ~GVIS_READONLY);
		m_Grid.SetItemBkColour(nRow, nCol, CLR_BACK);
		m_Grid.SetItemFgColour(nRow, nCol);

		str.Format("%.f", pBri->GetLengthHDanAct(sta, vAng, nCol-1,bRoadNotSlab));
		m_Grid.SetTextMatrix(nRow++, nCol, str);
		m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(1, nCol) & ~GVIS_READONLY);
		m_Grid.SetItemBkColour(nRow, nCol, CLR_BACK);
		m_Grid.SetItemFgColour(nRow, nCol);

		if(pBri->m_bApplyLineSlope)
		{
			m_Grid.SetCheck(nRow++, nCol, pBri->IsLevelHDanApplyLineSlope(nCol-1,bRoadNotSlab));
		}
		else
		{
			str.Format("%.3f", pBri->GetSlopeHDanByStation(sta, nCol-1,bRoadNotSlab)*100);
			m_Grid.SetTextMatrix(nRow++, nCol, str);
		}
		m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(1, nCol) & ~GVIS_READONLY);
		m_Grid.SetItemBkColour(nRow, nCol, CLR_BACK);
		m_Grid.SetItemFgColour(nRow, nCol);

		str = pBri->GetStringTypeHDan(nCol-1, FALSE,bRoadNotSlab);		
		m_Grid.SetTextMatrix(nRow++, nCol, str);
		m_Grid.SetCheck(nRow++, nCol, pBri->IsVariableLenHDan(nCol-1, bRoadNotSlab));
	}

}
void CInputBaseCrossConstruct::SetDataInitNori()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	CString str = _T("");
	long nRow = 2;
	long nCol = 0;
	str.Format("%.3f", pBri->m_dUnderGroundNoriSlopeFirst);		m_Grid.SetTextMatrix(nRow, nCol++, str);
	str.Format("%.3f", pBri->m_dUnderGroundNoriSlopeNext);		m_Grid.SetTextMatrix(nRow, nCol++, str);
	str.Format("%.0f", pBri->m_dUnderGroundNoriMaxHeight);		m_Grid.SetTextMatrix(nRow, nCol++, str);
	str.Format("%.0f", pBri->m_dUnderGroundNoriWidth);			m_Grid.SetTextMatrix(nRow, nCol++, str);
	str.Format("%.3f", pBri->m_dUnderGroundNoriSolpeSodan);		m_Grid.SetTextMatrix(nRow, nCol++, str);
}
void CInputBaseCrossConstruct::SetDataInitPave()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	CString str = _T("");

	long nRow = 2;
	long nCol = 1;
	if(pStd->IsRailDesign())
	{
		str.Format("%.0f", pBri->m_dUnderGroundHeightMortar);	m_Grid.SetTextMatrix(nRow, nCol, str);
		
		str = pStd->GetStrDoubleUnitChange(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightCement, UNIT_CGS_KGF_M3,pBri->m_pARcBridgeDataStd->m_szKgf_M3, TRUE, 1);
		m_Grid.SetTextMatrix(nRow + 1, nCol++, str);
	}
	else
	{
		if(pBri->m_bUnderGroundRahmen)
		{
			if(pBri->m_nTypePavement == 0)
				str = "�ƽ���Ʈ����";
			else
				str = "��ũ��Ʈ����";
			m_Grid.SetTextMatrix(nRow, nCol, str);

			m_Grid.SetItemBkColour(nRow + 1, nCol, CLR_READONLY);
			m_Grid.SetItemState(nRow + 1, nCol++, m_Grid.GetItemState(nRow, nCol) | GVIS_READONLY);

			if(pBri->m_bUnderGroundPaveDetail) m_Grid.SetItemBkColour(nRow, nCol, CLR_READONLY);
			str.Format("%.0f", pBri->m_dUnderGroundHeightPavement);			m_Grid.SetTextMatrix(nRow, nCol, str);

			str = pStd->GetStrDoubleUnitChange(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightAsphalt, UNIT_CGS_KGF_M3,pBri->m_pARcBridgeDataStd->m_szKgf_M3, TRUE, 1);
			m_Grid.SetTextMatrix(nRow + 1, nCol++, str);

			str.Format("%.0f", pBri->m_dUnderGroundHeightMortar);	m_Grid.SetTextMatrix(nRow, nCol, str);
			
			str = pStd->GetStrDoubleUnitChange(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightCement, UNIT_CGS_KGF_M3,pBri->m_pARcBridgeDataStd->m_szKgf_M3, TRUE, 1);
			m_Grid.SetTextMatrix(nRow + 1, nCol++, str);
		}
		else
		{
			if(pBri->m_nRoadMaterial == 0)
				str = "ASCON ����";
			else
				str = "LMC ����";
			m_Grid.SetTextMatrix(nRow, nCol, str);

			m_Grid.SetItemBkColour(nRow + 1, nCol, CLR_READONLY);
			m_Grid.SetItemState(nRow + 1, nCol++, m_Grid.GetItemState(nRow, nCol) | GVIS_READONLY);

			str.Format("%.0f", pBri->m_dTP);	m_Grid.SetTextMatrix(nRow, nCol, str);

			str = pStd->GetStrDoubleUnitChange(pBri->m_nRoadMaterial == 0 ? pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightAsphalt : pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightCement, UNIT_CGS_KGF_M3,pBri->m_pARcBridgeDataStd->m_szKgf_M3, TRUE, 1);
			m_Grid.SetTextMatrix(nRow + 1, nCol++, str);
		}
	}

	if (pBri->m_bUnderGroundRahmen)
	{
		m_Grid.SetTextMatrix(nRow, nCol, m_bUnderGroundPaveDetail? _T("����") : _T("�������"));
		m_Grid.SetCheck(nRow, nCol++, m_bUnderGroundPaveDetail); // ��������
	}
	
/*
	if(pBri->m_bUnderGroundRahmen)
	{
		if(pBri->m_nTypePavement == 0)
			str = "�ƽ���Ʈ����";
		else
			str = "��ũ��Ʈ����";
		m_Grid.SetTextMatrix(nRow, nCol++, str);
		if(pBri->m_bUnderGroundPaveDetail) m_Grid.SetItemBkColour(nRow, nCol, CLR_READONLY);
		str.Format("%.0f", pBri->m_dUnderGroundHeightPavement);			m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%.0f", pBri->m_dUnderGroundHeightMortar);			m_Grid.SetTextMatrix(nRow, nCol++, str);
		nRow = 3;
		nCol = 2;
		str.Format("%.1f", pStd->GetValueUnitType(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightAsphalt, UNIT_CGS_KGF_M3,pBri->m_pARcBridgeDataStd->m_szKgf_M3));	m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%.1f", pStd->GetValueUnitType(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightCement, UNIT_CGS_KGF_M3,pBri->m_pARcBridgeDataStd->m_szKgf_M3));	m_Grid.SetTextMatrix(nRow, nCol++, str);

		pBri->SetDataInitPaveDetailWidth();
	}
	else
	{
		if(pStd->IsRailDesign())
		{
			str.Format("%.0f", pBri->m_dUnderGroundHeightMortar);	m_Grid.SetTextMatrix(2, nCol, str);
			str.Format("%.1f", pStd->GetValueUnitType(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightCement, UNIT_CGS_KGF_M3,pBri->m_pARcBridgeDataStd->m_szKgf_M3));	m_Grid.SetTextMatrix(3, nCol++, str);
		}
		else
		{
			if(pBri->m_nRoadMaterial == 0)
				str = "ASCON ����";
			else
				str = "LMC ����";
			m_Grid.SetTextMatrix(nRow, nCol++, str);
			str.Format("%.0f", pBri->m_dTP);	m_Grid.SetTextMatrix(nRow, nCol++, str);
			str.Format("%.0f", pBri->GetUnderGroundPaveHeightTosa());	m_Grid.SetTextMatrix(nRow, nCol++, str);
			nRow = 3;
			nCol = 2;
			str.Format("%.1f", pStd->GetValueUnitType(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightAsphalt, UNIT_CGS_KGF_M3,pBri->m_pARcBridgeDataStd->m_szKgf_M3));	m_Grid.SetTextMatrix(nRow, nCol++, str);
		}

		if(pBri->m_bPrevApplyCalcOpen) str.Format("%.1f", pStd->GetValueUnitType(pBri->m_dUnderGroundPaveWeightTosa, UNIT_CGS_KGF_M3,pBri->m_pARcBridgeDataStd->m_szKgf_M3));	m_Grid.SetTextMatrix(nRow, nCol++, str);
	}

*/

	if(pBri->m_bUnderGroundRahmen && pBri->m_bUnderGroundPaveDetail)
	{
		long nSttCol = nCol;
		nRow = 2;
		nCol = nSttCol;
		str.Format("%.0f", pBri->m_dUnderGroundPaveHeightPyo);			m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%.0f", pBri->m_dUnderGroundPaveHeightGee);			m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%.0f", pBri->m_dUnderGroundPaveHeightMiddle);		m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%.0f", pBri->m_dUnderGroundPaveHeightBozo);			m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%.0f", pBri->m_dUnderGroundPaveHeightDong);			m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%.0f", pBri->m_dUnderGroundPaveHeightEps);			m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%.0f", pBri->GetUnderGroundPaveHeightTosa());		m_Grid.SetTextMatrixEx(nRow, nCol, str, TRUE); m_Grid.SetItemBkColour(nRow, nCol, CLR_READONLY);

		nRow = 3;
		nCol = nSttCol;
		str.Format("%s", pStd->GetStrDoubleUnitChange(pBri->m_dUnderGroundPaveWeightPyo, UNIT_CGS_KGF_M3,pBri->m_pARcBridgeDataStd->m_szKgf_M3, TRUE, 1));	m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%s", pStd->GetStrDoubleUnitChange(pBri->m_dUnderGroundPaveWeightGee, UNIT_CGS_KGF_M3,pBri->m_pARcBridgeDataStd->m_szKgf_M3, TRUE, 1));	m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%s", pStd->GetStrDoubleUnitChange(pBri->m_dUnderGroundPaveWeightMiddle, UNIT_CGS_KGF_M3,pBri->m_pARcBridgeDataStd->m_szKgf_M3, TRUE, 1));	m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%s", pStd->GetStrDoubleUnitChange(pBri->m_dUnderGroundPaveWeightBozo, UNIT_CGS_KGF_M3,pBri->m_pARcBridgeDataStd->m_szKgf_M3, TRUE, 1));	m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%s", pStd->GetStrDoubleUnitChange(pBri->m_dUnderGroundPaveWeightDong, UNIT_CGS_KGF_M3,pBri->m_pARcBridgeDataStd->m_szKgf_M3, TRUE, 1));	m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%s", pStd->GetStrDoubleUnitChange(pBri->m_dUnderGroundPaveWeightEps, UNIT_CGS_KGF_M3,pBri->m_pARcBridgeDataStd->m_szKgf_M3, TRUE, 1));	m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%s", pStd->GetStrDoubleUnitChange(pBri->m_dUnderGroundPaveWeightTosa, UNIT_CGS_KGF_M3,pBri->m_pARcBridgeDataStd->m_szKgf_M3, TRUE, 1));	m_Grid.SetTextMatrix(nRow, nCol, str);

		nRow = 4;
		nCol = nSttCol;
		str.Format("%.3f", pBri->m_dUnderGroundPaveWidthPyo);			m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%.3f", pBri->m_dUnderGroundPaveWidthGee);			m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%.3f", pBri->m_dUnderGroundPaveWidthMiddle);		m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%.3f", pBri->m_dUnderGroundPaveWidthBozo);			m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%.3f", pBri->m_dUnderGroundPaveWidthDong);			m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%.3f", pBri->m_dUnderGroundPaveWidthEps);			m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format("%.3f", pBri->m_dUnderGroundPaveWidthTosa);			m_Grid.SetTextMatrix(nRow, nCol, str);
	}
	else
	{
		if(pBri->m_bPrevApplyCalcOpen)
		{
			nRow = 3;
			str.Format("%.1f", pStd->GetValueUnitType(pBri->m_dUnderGroundPaveWeightTosa, UNIT_CGS_KGF_M3,pBri->m_pARcBridgeDataStd->m_szKgf_M3));	m_Grid.SetTextMatrix(nRow, nCol, str);
		}
	}
}


void CInputBaseCrossConstruct::SetDataInitRailData()
{
	long nRow = 1;

	CString str;
	str.Format("%d ��", m_nQtyRail + 1);
	m_Grid.SetTextMatrix(nRow++, 1, str);

	str.Format("%g", m_dUnitWeightRail);
	m_Grid.SetTextMatrix(nRow++, 1, str);

	str.Format("%g", m_dDisBaseElevationRL);
	m_Grid.SetTextMatrix(nRow++, 1, str);
	
	str.Format("%.10g", m_dHeightRail);
	m_Grid.SetTextMatrix(nRow++, 1, str);

	str.Format("%g", m_dHeightDosangUnderTie);
	m_Grid.SetTextMatrix(nRow++, 1, str);
	
	str.Format("%g", m_dSlopeDosang);
	m_Grid.SetTextMatrix(nRow++, 1, str);
}

void CInputBaseCrossConstruct::SetDataInitRail()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	CString str = _T("");
	for(long nRail = 0;nRail < pBri->GetQtyRailNode(); nRail++)
	{
		str.Format("%.0f", pBri->GetLengthRailByLine(nRail));
		m_Grid.SetTextMatrix(1, nRail+1, str);
		long nLine = pBri->GetRailLineNumber(nRail);
		CString strName = pStd->m_pLineApp->GetLine(nLine)->GetNameLineEach();
		m_Grid.SetTextMatrix(2, nRail+1, strName);
	}
}
void CInputBaseCrossConstruct::SetDataInitDistribute()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	CString str = _T("");
	long nRow = 1;
	long nCol = 0;
	str.Format("%.0f", pBri->m_dRail_LengthTie);			m_Grid.SetTextMatrix(nRow, nCol++, str);// ħ����
	str.Format("%.0f", pBri->m_dRail_HeightTie);			m_Grid.SetTextMatrix(nRow, nCol++, str);// ħ�����
	double dD = pBri->m_dDisBaseElevationRL - pBri->m_dHeightRail - pBri->m_dRail_HeightTie - pBri->m_dUnderGroundHeightMortar;
	if(pBri->m_dDisBaseElevationRL == 0) dD = pBri->GetHeightDosangInput() - pBri->m_dHeightRail;
	str.Format("%.0f", dD);			m_Grid.SetTextMatrix(nRow, nCol++, str);	// ħ��β��� ������ �ڰ��������
	str.Format("%.0f", pBri->m_dUnderGroundHeightMortar);	m_Grid.SetTextMatrix(nRow, nCol++, str);	// �����Ż
	str.Format("%.0f", pBri->m_dTS);				m_Grid.SetTextMatrix(nRow, nCol++, str);// ������β�

	long i = 0; for(i = 2; i < 5; i++)
	{
		m_Grid.SetItemState(1, i, m_Grid.GetItemState(1, i) | GVIS_READONLY);
		m_Grid.SetItemBkColour(1, i, CLR_READONLY);
		m_Grid.SetItemFgColour(1, i, RGB(128, 128, 128));
	}
}

void CInputBaseCrossConstruct::SetDataSave()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	long nTab = m_ctlTab.GetCurSel();

	pBri->m_dTS						 = m_dTS;		// ������ �β�
	pBri->m_dDisBaseElevation		 = m_dDisBaseElevation;		// F.L ������ġ(ö���� ������� ���� ��ġ)
	pBri->m_dTSOld					 = m_dTS;
	if(pBri->m_bUnderGroundRahmen && nTab == m_nTabSlab)
		pBri->m_bLevelFrRoadCenterToEdgeOfUnderGroundRahmenSlab	= m_bLevelFrRoadCenterToEdge;
	else
		pBri->m_bLevelFrRoadCenterToEdge = m_bLevelFrRoadCenterToEdge;		// --�����߽ɰ��� ���� ����//Ⱦ�� ������ �����Է� �߰����� 20050610

	// ���߶�౳ ������ �����Է�
	pBri->m_bApplyLineSlope			 = m_bApplyLineSlope;		// --Ⱦ��� ���� ���� ����//Ⱦ�� ������ �����Է� �߰����� 20050610

	MakeLinefoBridge(m_pIn->m_nCurrentBridge);

	m_pIn->m_bViewAllDir			 = m_bViewAllDir;	//�����༱ ��� ǥ��

	pBri->m_dDistRoadCenterLevelLeft	= m_dDistRoadCenterLevelLeft;
	pBri->m_dDistRoadCenterLevelRight	= m_dDistRoadCenterLevelRight;

	if(nTab == m_nTabSlab)			SetDataSaveSlab();
	else if(nTab == m_nTabUnderGround) SetDataSaveUnderGround();
	else if(nTab == m_nTabRoad)		SetDataSaveRoad();
	else if(nTab == m_nTabNori)		SetDataSaveNori();
	else if(nTab == m_nTabPave)		SetDataSavePave();
	else if(nTab == m_nTabRailData) SetDataSaveRailData();
	else if(nTab == m_nTabRail)		SetDataSaveRail();
	else if(nTab == m_nTabDistribute)	SetDataSaveDistribute();

	// ���߶�౳�� �ƴ� ��쿡 �߸� ������ ���� �ʱ�ȭ...
	if(pBri->m_bUnderGroundRahmen==FALSE)
	{
		pBri->m_dUnderGroundDesignHeight = 0;
	}

	pBri->m_bNeedSettingVertSection	= TRUE;
	UpdateData();
	m_pIn->m_pDoc->SetModifiedFlag();

	if(pStd->IsRailDesign() && pBri->m_nQtyRail <= 0)
	{
		AfxMessageBox("ö�������� ���� �Է��� �����մϴ�.");
	}
}

void CInputBaseCrossConstruct::SetDataSaveSlab()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	BOOL bLsd = pStd->IsLsdDesign();
	long nCols = m_Grid.GetCols();
	pBri->SetQtyHDan(nCols-1);
	pBri->MakeGuardWall(nCols-1);
	BOOL bDwLoad = FALSE;
	HGBOOL bRoadNotSlab = FALSE;

	CString str = _T("");
	for(long nCol=1; nCol<nCols; nCol++)
	{
		long nRow = 1;
		long nHDan = nCol-1;
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan);
		if(!pGW) continue;

		pBri->SetLengthHDan(nHDan, m_Grid.GetTextMatrixDouble(nRow++, nCol));
		if(!pBri->m_bUnderGroundRahmen && pBri->m_bApplyLineSlope)
			pBri->SetLevelHDan(nHDan, m_Grid.IsCheck(nRow++, nCol));
		else
			pBri->SetSlopeHDan(nHDan, m_Grid.GetTextMatrixDouble(nRow++, nCol)/100);
		pBri->SetTypeHDanByString(nHDan, FALSE, m_Grid.GetItemText(nRow++, nCol));
		if(bLsd)
		{
			bDwLoad = m_Grid.IsCheck(nRow++, nCol);
		}

		if(pGW->IsTypeHDanOnlyNone())
			pBri->SetVariableLenHDan(nHDan, bRoadNotSlab, m_Grid.IsCheck(nRow++, nCol));
		else
			pBri->SetVariableLenHDan(nHDan, bRoadNotSlab, FALSE);

		pGW->m_nTypeGuardWall = pBri->GetTypeHDan(nHDan);
		pGW->m_nNumberHDan = nHDan;
		pGW->m_bRailRoad = pStd->IsRailDesign();
		pGW->m_bLeftGuardFence = pBri->IsLeftHDan(nHDan);
		pGW->m_dWidhtHDan = pBri->GetLengthHDan(nHDan);
		pGW->m_bDWload = bDwLoad;
	}
}

void CInputBaseCrossConstruct::SetDataSaveUnderGround()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	long nRow = 1;
	long nCol = 2;
	m_dWidthRoadLeft = m_Grid.GetTextMatrixDouble(nRow++, nCol);
	m_dWidthRoadRight = m_Grid.GetTextMatrixDouble(nRow++, nCol);
	m_szUnderElevationStt.Format("%.3f",m_Grid.GetTextMatrixDouble(nRow++, nCol));// ��������EL (���ܽ����� EL)
	m_szUnderElevationEnd.Format("%.3f",m_Grid.GetTextMatrixDouble(nRow++, nCol));// ��������EL (���������� EL)

	pBri->m_dWidthRoadLeft	 = m_dWidthRoadLeft;		// ���� ��� �Ÿ�
	pBri->m_dWidthRoadRight	 = m_dWidthRoadRight;		// ���� ��� �Ÿ�
	pBri->m_dUnderGroundCenterELStt	 = atof(m_szUnderElevationStt);	// ��������EL (���ܽ����� EL)
	pBri->m_dUnderGroundCenterELEnd	 = atof(m_szUnderElevationEnd);	// ��������EL (���������� EL)
}

void CInputBaseCrossConstruct::SetDataSaveRoad()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	long nCols = m_Grid.GetCols();
	pBri->SetQtyHDanRoad(nCols-1);
	pBri->MakeGuardWall(nCols-1, TRUE);
	
	HGBOOL bRoadNotSlab = TRUE;
	CString str = _T("");
	for(long nCol=1; nCol<nCols; nCol++)
	{
		long nRow = 1;
		pBri->SetLengthHDanRoad(nCol-1, m_Grid.GetTextMatrixDouble(nRow++, nCol));
		if(pBri->m_bApplyLineSlope)
			pBri->SetLevelHDanRoad(nCol-1, m_Grid.IsCheck(nRow++, nCol));
		else
			pBri->SetSlopeHDanRoad(nCol-1, m_Grid.GetTextMatrixDouble(nRow++, nCol)/100);
		pBri->SetTypeHDanByString(nCol-1, FALSE, m_Grid.GetItemText(nRow++, nCol),TRUE);
		pBri->SetVariableLenHDan(nCol-1, bRoadNotSlab, m_Grid.IsCheck(nRow++, nCol));
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nCol-1, TRUE);
		if(!pGW)	continue;
		pGW->m_nTypeGuardWall = pBri->GetTypeHDan(nCol-1,TRUE);
		pGW->m_nNumberHDan = nCol-1;
		pGW->m_bRailRoad = pStd->IsRailDesign();
		pGW->m_bLeftGuardFence = pBri->IsLeftHDan(nCol-1,TRUE);
		pGW->m_dWidhtHDan = pBri->GetLengthHDan(nCol-1,TRUE);
	}
}

void CInputBaseCrossConstruct::SetDataSaveNori()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	long nRow = 2;
	long nCol = 0;
	pBri->m_dUnderGroundNoriSlopeFirst	= m_Grid.GetTextMatrixDouble(nRow, nCol++);
	pBri->m_dUnderGroundNoriSlopeNext	= m_Grid.GetTextMatrixDouble(nRow, nCol++);
	pBri->m_dUnderGroundNoriMaxHeight	= m_Grid.GetTextMatrixDouble(nRow, nCol++);
	pBri->m_dUnderGroundNoriWidth		= m_Grid.GetTextMatrixDouble(nRow, nCol++);
	pBri->m_dUnderGroundNoriSolpeSodan	= m_Grid.GetTextMatrixDouble(nRow, nCol++);
}

void CInputBaseCrossConstruct::SetDataSavePave()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	CString str = _T("");
	long nRow = 2;
	long nCol = 1;

	if(pStd->IsRailDesign())
	{
		pBri->m_dUnderGroundHeightMortar	= m_Grid.GetTextMatrixDouble(nRow, nCol);
		pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightCement  = pStd->GetValueUnitType(m_Grid.GetTextMatrixDouble(nRow + 1, nCol++),pBri->m_pARcBridgeDataStd->m_szKgf_M3, UNIT_CGS_KGF_M3);
	}
	else
	{
		if(pBri->m_bUnderGroundRahmen)
		{
			if(m_Grid.GetItemText(nRow, nCol++) == "�ƽ���Ʈ����")
				pBri->m_nTypePavement = 0;
			else
				pBri->m_nTypePavement = 1;

			pBri->m_dUnderGroundHeightPavement	= m_Grid.GetTextMatrixDouble(nRow, nCol);
			pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightAsphalt = pStd->GetValueUnitType(m_Grid.GetTextMatrixDouble(nRow + 1, nCol++),pBri->m_pARcBridgeDataStd->m_szKgf_M3, UNIT_CGS_KGF_M3);

			pBri->m_dUnderGroundHeightMortar	= m_Grid.GetTextMatrixDouble(nRow, nCol);
			pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightCement  = pStd->GetValueUnitType(m_Grid.GetTextMatrixDouble(nRow + 1, nCol++),pBri->m_pARcBridgeDataStd->m_szKgf_M3, UNIT_CGS_KGF_M3);
		}
		else
		{
			long nSelectOld = pBri->m_nRoadMaterial;
			if(m_Grid.GetItemText(nRow, nCol++) == "ASCON ����")
				pBri->m_nRoadMaterial = 0;
			else
				pBri->m_nRoadMaterial = 1;

			pBri->m_dTP	= m_Grid.GetTextMatrixDouble(nRow, nCol);
			if(nSelectOld == 0)
				pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightAsphalt = pStd->GetValueUnitType(m_Grid.GetTextMatrixDouble(nRow + 1, nCol++),pBri->m_pARcBridgeDataStd->m_szKgf_M3, UNIT_CGS_KGF_M3);
			else
				pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightCement = pStd->GetValueUnitType(m_Grid.GetTextMatrixDouble(nRow + 1, nCol++),pBri->m_pARcBridgeDataStd->m_szKgf_M3, UNIT_CGS_KGF_M3);
		}
	}

	if (pBri->m_bUnderGroundRahmen)
	{
		m_bUnderGroundPaveDetail = m_Grid.IsCheck(nRow, nCol++); // ��������

		if(pBri->m_nSeparBri!=0)
		{

			long nIdxMatch = pStd->GetIdxMatchBridge(m_pIn->m_nCurrentBridge);
			CRcBridgeApp* pBriMatch = pStd->GetBridge(nIdxMatch);

			pBriMatch->m_bUnderGroundPaveDetail = pBri->m_bUnderGroundPaveDetail;
		}
	}
	
/*	
	if(pBri->m_bUnderGroundRahmen)
	{
		if(m_Grid.GetItemText(nRow, nCol++) == "�ƽ���Ʈ����")
			pBri->m_nTypePavement = 0;
		else
			pBri->m_nTypePavement = 1;
		pBri->m_dUnderGroundHeightPavement	= m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pBri->m_dUnderGroundHeightMortar	= m_Grid.GetTextMatrixDouble(nRow, nCol++);
		nRow = 3;
		nCol = 2;
		pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightAsphalt = pStd->GetValueUnitType(m_Grid.GetTextMatrixDouble(nRow, nCol++),pBri->m_pARcBridgeDataStd->m_szKgf_M3, UNIT_CGS_KGF_M3);
		pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightCement  = pStd->GetValueUnitType(m_Grid.GetTextMatrixDouble(nRow, nCol++),pBri->m_pARcBridgeDataStd->m_szKgf_M3, UNIT_CGS_KGF_M3);
	}
	else
	{
		if(m_Grid.GetItemText(nRow, nCol++) == "ASCON ����")
			pBri->m_nRoadMaterial = 0;
		else
			pBri->m_nRoadMaterial = 1;
		pBri->m_dTP	= m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pBri->m_dUnderGroundPaveHeightTosa_XXX = m_Grid.GetTextMatrixDouble(nRow, nCol++);
		nRow = 3;
		nCol = 2;
		pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightAsphalt = pStd->GetValueUnitType(m_Grid.GetTextMatrixDouble(nRow, nCol++),pBri->m_pARcBridgeDataStd->m_szKgf_M3, UNIT_CGS_KGF_M3);
		if(pBri->m_bPrevApplyCalcOpen) pBri->m_dUnderGroundPaveWeightTosa = pStd->GetValueUnitType(m_Grid.GetTextMatrixDouble(nRow, nCol++),pBri->m_pARcBridgeDataStd->m_szKgf_M3, UNIT_CGS_KGF_M3);

		pBri->m_dUnderGroundHeightMortar = 0;
	}
*/
	if(pBri->m_bUnderGroundRahmen && pBri->m_bUnderGroundPaveDetail)
	{
		long nSttCol = nCol;
		nRow = 2;
		nCol = nSttCol;
		pBri->m_dUnderGroundPaveHeightPyo	= m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pBri->m_dUnderGroundPaveHeightGee	= m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pBri->m_dUnderGroundPaveHeightMiddle= m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pBri->m_dUnderGroundPaveHeightBozo	= m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pBri->m_dUnderGroundPaveHeightDong	= m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pBri->m_dUnderGroundPaveHeightEps	= m_Grid.GetTextMatrixDouble(nRow, nCol);
		//pBri->m_dUnderGroundPaveHeightTosa	= m_Grid.GetTextMatrixDouble(nRow, nCol);
		double dHeigtAll = pBri->CalcUnderGroundDesignHeight();
		// (ARCBRIDGE-2939) �����̴� �������ǿ��� ��������(ǥ��, ����, �߰���, ��������, EPS��)�� �� ������ �����մϴ�.
		// pBri->m_dUnderGroundPaveHeightTosa_XXX = dHeigtAll - (pBri->m_dUnderGroundPaveHeightPyo + pBri->m_dUnderGroundPaveHeightGee + pBri->m_dUnderGroundPaveHeightMiddle + pBri->m_dUnderGroundPaveHeightBozo + pBri->m_dUnderGroundPaveHeightDong + pBri->m_dUnderGroundPaveHeightEps);

		nRow = 3;
		nCol = nSttCol;
		pBri->m_dUnderGroundPaveWeightPyo	= pStd->GetValueUnitType(m_Grid.GetTextMatrixDouble(nRow, nCol++),pBri->m_pARcBridgeDataStd->m_szKgf_M3, UNIT_CGS_KGF_M3);
		pBri->m_dUnderGroundPaveWeightGee	= pStd->GetValueUnitType(m_Grid.GetTextMatrixDouble(nRow, nCol++),pBri->m_pARcBridgeDataStd->m_szKgf_M3, UNIT_CGS_KGF_M3);
		pBri->m_dUnderGroundPaveWeightMiddle= pStd->GetValueUnitType(m_Grid.GetTextMatrixDouble(nRow, nCol++),pBri->m_pARcBridgeDataStd->m_szKgf_M3, UNIT_CGS_KGF_M3);
		pBri->m_dUnderGroundPaveWeightBozo	= pStd->GetValueUnitType(m_Grid.GetTextMatrixDouble(nRow, nCol++),pBri->m_pARcBridgeDataStd->m_szKgf_M3, UNIT_CGS_KGF_M3);
		pBri->m_dUnderGroundPaveWeightDong	= pStd->GetValueUnitType(m_Grid.GetTextMatrixDouble(nRow, nCol++),pBri->m_pARcBridgeDataStd->m_szKgf_M3, UNIT_CGS_KGF_M3);
		pBri->m_dUnderGroundPaveWeightEps	= pStd->GetValueUnitType(m_Grid.GetTextMatrixDouble(nRow, nCol++),pBri->m_pARcBridgeDataStd->m_szKgf_M3, UNIT_CGS_KGF_M3);
		pBri->m_dUnderGroundPaveWeightTosa	= pStd->GetValueUnitType(m_Grid.GetTextMatrixDouble(nRow, nCol),pBri->m_pARcBridgeDataStd->m_szKgf_M3, UNIT_CGS_KGF_M3);

		nRow = 4;
		nCol = nSttCol;
		pBri->m_dUnderGroundPaveWidthPyo	= m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pBri->m_dUnderGroundPaveWidthGee	= m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pBri->m_dUnderGroundPaveWidthMiddle= m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pBri->m_dUnderGroundPaveWidthBozo	= m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pBri->m_dUnderGroundPaveWidthDong	= m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pBri->m_dUnderGroundPaveWidthEps	= m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pBri->m_dUnderGroundPaveWidthTosa	= m_Grid.GetTextMatrixDouble(nRow, nCol);
	}
	else
	{
		if(pBri->m_bPrevApplyCalcOpen)
		{
			nRow = 3;
			pBri->m_dUnderGroundPaveWeightTosa = pStd->GetValueUnitType(m_Grid.GetTextMatrixDouble(nRow, nCol),pBri->m_pARcBridgeDataStd->m_szKgf_M3, UNIT_CGS_KGF_M3);
		}
	}

	pBri->m_bUnderGroundPaveDetail	 = m_bUnderGroundPaveDetail;		// ���� ���� ����
}

void CInputBaseCrossConstruct::SetDataSaveRailData()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	long nRow = 1;
	long nCol = 1;

	m_nQtyRail = atoi(m_Grid.GetItemText(nRow++, 1)) - 1;

	m_dUnitWeightRail = m_Grid.GetTextMatrixDouble(nRow++, nCol);
	m_dDisBaseElevationRL = m_Grid.GetTextMatrixDouble(nRow++, nCol);
	m_dHeightRail = m_Grid.GetTextMatrixDouble(nRow++, nCol);
	m_dHeightDosangUnderTie = m_Grid.GetTextMatrixDouble(nRow++, nCol);
	m_dSlopeDosang = m_Grid.GetTextMatrixDouble(nRow++, nCol);

	// ö���� ������ �����Է�
	pBri->SetQtyRailNode(m_nQtyRail+1);		// [������ ���� �Է�], ���� ���Ҽ�, ö�������� ���
	pBri->m_dUnitWeightRail			= GetValueUnitChange(m_dUnitWeightRail, pStd->m_szTonf_M, UNIT_CGS_TONF_M);
	pBri->m_dDisBaseElevationRL		 = m_dDisBaseElevationRL;		// --R.L ������ġ(ö���� ������� ���� ��ġ)//Ⱦ�� ������ �����Է� �߰����� 20050610
	pBri->m_dHeightRail				 = m_dHeightRail;		// --ö���� ���ϳ���//Ⱦ�� ������ �����Է� �߰����� 20050610
	pBri->m_dHeightDosangUnderTie	 = m_dHeightDosangUnderTie;		// ö���� �������
	pBri->m_dSlopeDosang			 = m_dSlopeDosang;		// ö���� ���� ����
}

void CInputBaseCrossConstruct::SetDataSaveRail()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	CString str = _T("");
	double dMinDist = 100000;
	double dPreDist = 0;
	for(long nRail = 0;nRail < pBri->GetQtyRailNode(); nRail++)
	{
		double dCenDist = atof(m_Grid.GetItemText(1,nRail+1));
		if(nRail>0)
		{
			dMinDist = min(dMinDist, fabs(dCenDist-dPreDist));
		}
		
		pBri->SetLengthRailByLine(nRail,dCenDist);
		str = m_Grid.GetItemText(2,nRail+1);
		for(long nLine=0; nLine<pStd->m_pLineApp->GetLinesu();nLine++)
		{
			if(pStd->m_pLineApp->GetLine(nLine)->GetNameLineEach() == str)
			{
				pBri->SetRailLineNumber(nRail, nLine);
			}
		}
		dPreDist = dCenDist;
	}
	if(pBri->GetQtyRailNode() == 1)
	{
		// (ARCBRIDGE-3294) ������ �ϳ� �϶��� �ϳ��� �Ÿ��� �״�� ����
		dMinDist = fabs(dPreDist);
	}
	pBri->m_dRail_DistBetweenRail = dMinDist;
}

void CInputBaseCrossConstruct::SetDataSaveDistribute()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	long nRow = 1;
	long nCol = 0;
	pBri->m_dRail_LengthTie				= m_Grid.GetTextMatrixDouble(nRow, nCol++);// ħ����
	pBri->m_dRail_HeightTie				= m_Grid.GetTextMatrixDouble(nRow, nCol++);// ħ�����
	nCol++;			// ħ��β��� ������ �ڰ��������
	pBri->m_dUnderGroundHeightMortar	= m_Grid.GetTextMatrixDouble(nRow, nCol++);// �����Ż
	pBri->m_dTS		= m_Grid.GetTextMatrixDouble(nRow, nCol++);// ������β�
}

void CInputBaseCrossConstruct::OnSelchangeComboBridgeNo() 
{
	m_pIn->m_nCurrentBridge = m_cbBridgeNo.GetCurSel();
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);

	if(m_pCalc)
	{
		delete m_pCalc;
		m_pCalc	= NULL;
	}

	m_pCalc = new CARcBridgeCalcStd(pBri);

	pBri->SetConcData(TRUE, FALSE, FALSE, FALSE, FALSE);
	m_pCalc->m_pARoadOptionStd = m_pIn->m_pARoadOptionStd;
	m_pCalc->m_pView		   = m_pView;

	InitTabCtrl();
	SetDataInit();
	SetPaneTitle();

	DrawInputDomyunView();
}

void CInputBaseCrossConstruct::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;

	CCellID cell	= m_Grid.GetFocusCell();
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	long nTab = m_ctlTab.GetCurSel();

	CRcBridgeRebar *pBriRebar = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	CDPoint vAng = CDPoint(0, 1);
	double sta = pBriRebar->GetStationOnJijum(0,0);
	CJijum* pJL = pBriRebar->GetJijum(0);
	if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pJL->m_dLengthUgan[1];

	long nRow = pGridView->iRow;
	long nCol = pGridView->iColumn;
	CString str = _T("");
	if(nRow==1 && (nTab == m_nTabSlab || nTab == m_nTabRoad))
	{
		long bRoadNotSlab = (nTab == m_nTabRoad);
		if(nCol == pBri->GetQtyHDan(bRoadNotSlab))	//�������� �ƴϸ�(�������̸� �ڵ����� ����)
		{
			double lenHDanGrid = m_Grid.GetTextMatrixDouble(nRow, nCol);
			double lenHDanSaved = pBri->GetLengthHDan(nCol-1, bRoadNotSlab);
			double lenHDanNext = pBri->GetLengthHDan(nCol-2, bRoadNotSlab)+(lenHDanSaved-lenHDanGrid);
			str.Format("%.f", lenHDanNext);	m_Grid.SetTextMatrix(nRow, nCol-1, str);
		}
		else
		{
			double lenHDanGrid = m_Grid.GetTextMatrixDouble(nRow, nCol);
			double lenHDanSaved = pBri->GetLengthHDan(nCol-1, bRoadNotSlab);
			double lenHDanNext = pBri->GetLengthHDan(nCol, bRoadNotSlab)+(lenHDanSaved-lenHDanGrid);
			str.Format("%.f", lenHDanNext);	m_Grid.SetTextMatrix(nRow, nCol+1, str);
		}
	}

	SetDataSave();
	if(nTab == m_nTabSlab)
	{
		if(nRow==3)	//���߶���̰� ������ ��ȣ������ bRoadNotSlab=TRUE �����(���θ� ��ȣ������ ������� ����)
		{
			CGuardWallRC* pGW1 = pBri->GetGuardWallByHDan(nCol-1, FALSE);
			if(pGW1)
				pGW1->SetDataDefaultGuardWall(pBri->GetThickPave(FALSE), FALSE);
			CGuardWallRC* pGW2 = pBriRebar->GetGuardWallByHDan(nCol-1, FALSE);
			if(pGW2)
				pBriRebar->m_Util.SetDataDefaultRebarGuardWall(pGW2, sta, vAng);	// ��� �⺻��
		}
	}
	else if(nTab == m_nTabRoad)
	{
		if(nRow==3)
		{
			CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nCol-1, TRUE);
			if(pGW)
				pGW->SetDataDefaultGuardWall(pBri->GetThickPave(TRUE), TRUE);
		}
	}
	
	SetDataInit();
	m_Grid.ResetSelection();
	m_Grid.SetFocusCell(cell.row, cell.col);
	//DrawInputDomyunView();
	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);
}

void CInputBaseCrossConstruct::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;

	long nCol = pGridView->iColumn;
	CString str = m_Grid.GetTextMatrix(0, nCol);
	m_pView->GetDomyun()->STMakeCursor(str);
}

void CInputBaseCrossConstruct::DrawInputDomyunViewDimSlabRoad(CDomyun* pDom, BOOL bRoadNotSlab, HGDOUBLE dSta, HGDOUBLE dSta1, HGBOOL bInput, HGBOOL bExp)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBriCur = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBriCur)
		return;

	CRcBridgeRebar* pBriMatch = NULL;
	if(pBriCur->m_nSeparBri != 0)
	{
		if(pBriCur->m_nSeparBri == 1)
			pBriMatch = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge+1);	//���༱ �� ���, ���༱(���༱+1)�� ǥ��, ���� ������ ���༱ ǥ��
		else
			pBriMatch = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge-1);	//���༱ �� ���, ���༱(���༱-1)�� ǥ��, ���� ������ ���༱ ǥ��
	}

	CARcBridgeDBStandard DBStd(pBriCur, m_pIn->m_pARoadOptionStd, pStd);
	CDomyun Dom(pDom);
	
	CDPoint vAng90 = CDPoint(0,1);

	BOOL bNotRoadSlab	= pBriCur->m_bUnderGroundRahmen;
	double eleMax = pBriCur->GetElMaxSlabUpper(dSta,vAng90,bNotRoadSlab) + 1650;
	double eleMin = pBriCur->GetElMinSlabUpper(dSta,vAng90,FALSE) - (pBriCur->m_dTS + pBriCur->m_dHHS+300);

	DBStd.m_pDrawCrossSection->DimCrossSection(&Dom, dSta, vAng90, bInput, bRoadNotSlab, bExp);
	DBStd.m_pDrawCrossSection->DimCrossSectionStationAndElevation(&Dom, dSta, TRUE, TRUE, TRUE, TRUE);
	DBStd.m_pDrawCrossSection->MarkGenCrossSlope(&Dom, dSta, vAng90);// ����
	DBStd.MarkCenterOfRoad(&Dom, CDPoint(0,eleMax), eleMax-eleMin);// ���� �߽� ��ũ
	
	if(pBriMatch && m_pIn->m_bViewAllDir == TRUE)// ������ �и� ����(0:������ ��ü, 1:���༱, 2:���༱)
	{
		CARcBridgeDBStandard DBStd(pBriMatch, m_pIn->m_pARoadOptionStd, pStd);
		DBStd.m_pDrawCrossSection->DimCrossSectionFullLeftRight(&Dom, pBriMatch, pBriCur);

		DBStd.m_pDrawCrossSection->DimCrossSection(&Dom, dSta1, vAng90, FALSE,bRoadNotSlab, bExp);
		DBStd.m_pDrawCrossSection->MarkGenCrossSlope(&Dom, dSta1, vAng90);
	}
	if(pBriCur->GetQtyRailNode()>0 && pStd->IsRailDesign())
	{
		DBStd.m_pDrawCrossSection->DimCrossSectionRailCenter(&Dom, dSta, vAng90,bInput);
	}

	*pDom << Dom;
}

void CInputBaseCrossConstruct::DrawInputDomyunViewDimNori(CDomyun* pDom, HGDOUBLE dSta, HGDOUBLE dSta1, HGBOOL bInput, HGBOOL bExp)
{
	DrawInputDomyunViewDimSlabRoad(pDom,TRUE, dSta, dSta1, bInput, bExp);
}

void CInputBaseCrossConstruct::DrawInputDomyunViewDimPave(CDomyun* pDom, HGDOUBLE dSta, HGDOUBLE dSta1)
{
}

// void CInputBaseCrossConstruct::DrawInputDomyunViewDimRail(CDomyun* pDom, HGDOUBLE dSta, HGDOUBLE dSta1)
// {
// }

void CInputBaseCrossConstruct::DrawInputDomyunViewPlane(CDomyun* pDom)
{
	if (m_bShowPlane == FALSE)
		return;

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);

	pDom->SetCalcExtRect();
	CDRect rtDomOrg = pDom->GetExtRect();

	CDomyun Dom(pDom);

	DBStd.m_pDrawPlane->DrawPlaneGen(&Dom);
	DBStd.m_pDrawPlane->DrawPlaneLane(&Dom, pBri->m_bUnderGroundRahmen);
	DBStd.m_pDrawPlane->DrawPlaneNori(&Dom);

	if(pBri->m_nSeparBri != 0 && m_pIn->m_bViewAllDir == TRUE)
	{
		if(pBri->m_nSeparBri == 1)
			pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge+1);	//���༱ �� ���, ���༱(���༱+1)�� ǥ��, ���� ������ ���༱ ǥ��
		else
			pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge-1);	//���༱ �� ���, ���༱(���༱-1)�� ǥ��, ���� ������ ���༱ ǥ��

		CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);

		DBStd.m_pDrawPlane->DrawPlaneGen(&Dom);
		DBStd.m_pDrawPlane->DrawPlaneLane(&Dom, pBri->m_bUnderGroundRahmen);
		DBStd.m_pDrawPlane->DrawPlaneNori(&Dom);

		DBStd.m_pDrawPlane->MarkPlaneBridgeSttEnd(&Dom);
	}

	CEnvRegist env;		
	BOOL bRotate = env.LoadEnvInt(CEnvRegist::INPUTDOM_ROTATE);
	if(bRotate)
	{
		// ���� ���� �������� ȸ��
		DBStd.RotateDomByBridgeSttEndXy(&Dom);
	}

	Dom.SetCalcExtRect();
	CDRect rtDom = Dom.GetExtRect();

	Dom.Move(rtDomOrg.right - rtDom.left + Dom.Always(50), rtDomOrg.bottom - rtDom.bottom);

	*pDom << Dom;
}

void CInputBaseCrossConstruct::DrawInputDomyunViewCrossSection(CDomyun* pDom, HGDOUBLE dSta, HGDOUBLE dSta1, HGBOOL bInput)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) 
		return;

	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);
	CDPoint vAng = CDPoint(0,1);

	const HGBOOL bExp = pBri->IsExp(dSta);	// ������ ���߿� ó���Ѵ�. ���� ������ �־
	
	CDomyun Dom(pDom);

	m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	DBStd.m_pDrawCrossSection->DrawCrossSection(&Dom,dSta,vAng,FALSE,TRUE,FALSE,TRUE,TRUE,FALSE,TRUE, bExp, TRUE, 0, -1, m_bViewHatch);
	DBStd.m_pDrawCrossSection->DrawCrossSection(&Dom,dSta,vAng,FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE, bExp);
	DBStd.m_pDrawCrossSection->DrawCrossSlabHunch(&Dom, dSta,vAng, TRUE, 0, FALSE, bExp);	// ������ ��ġ	

	if(pBri->m_nSeparBri != 0 && m_pIn->m_bViewAllDir == TRUE)// ������ �и� ����(0:������ ��ü, 1:���༱, 2:���༱)
	{
		if(pBri->m_nSeparBri == 1)
			pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge+1);	//���༱ �� ���, ���༱(���༱+1)�� ǥ��, ���� ������ ���༱ ǥ��
		else
			pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge-1);	//���༱ �� ���, ���༱(���༱-1)�� ǥ��, ���� ������ ���༱ ǥ��

		CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);
		
		DBStd.m_pDrawCrossSection->DrawCrossSection(&Dom,dSta1,vAng,FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,FALSE, bExp);
		DBStd.m_pDrawCrossSection->DrawCrossSection(&Dom,dSta1,vAng,FALSE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE, bExp);
		DBStd.m_pDrawCrossSection->DrawCrossSlabHunch(&Dom, dSta1, vAng, TRUE, 0, FALSE, bExp);	// ������ ��ġ
	}

	m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	long nTab = m_ctlTab.GetCurSel();
	if(nTab == m_nTabSlab)			DrawInputDomyunViewDimSlabRoad(&Dom,FALSE, dSta, dSta1, bInput, bExp);
	else if(nTab == m_nTabRoad)		DrawInputDomyunViewDimSlabRoad(&Dom,TRUE, dSta, dSta1, bInput, bExp);
	else if(nTab == m_nTabNori)		DrawInputDomyunViewDimNori(&Dom, dSta, dSta1, bInput, bExp);
	else if(nTab == m_nTabPave)		DrawInputDomyunViewDimPave(&Dom, dSta, dSta1);
	else if(nTab == m_nTabRail)		DrawInputDomyunViewDimSlabRoad(&Dom,pBri->m_bUnderGroundRahmen, dSta, dSta1, bInput, bExp);
	else if(nTab == m_nTabUnderGround) DrawInputDomyunViewDimSlabRoad(&Dom,TRUE, dSta, dSta1, bInput, bExp);
	else if(nTab == m_nTabDistribute)
	{
		m_pCalc->m_pLoad->CalculateAll();
		m_pCalc->m_pLoad->DrawDeadLoadRail(&Dom, FALSE, dSta);
	}

	*pDom << Dom;
}

void CInputBaseCrossConstruct::DrawInputDomyunViewCrossSectionStt(CDomyun* pDom)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) 
		return;

	const HGBOOL bInput = (pBri->m_bIsExp || pBri->IsExtLine()) == FALSE;
	if (bInput == FALSE && m_bShowSectionEnd == FALSE)
		return;

	HGDOUBLE dSta(0), dSta1(0);
	GetStation(0, dSta, dSta1);

	CDomyun Dom(pDom);

	DrawInputDomyunViewCrossSection(&Dom, dSta, dSta1, bInput);

	*pDom << Dom;
}

void CInputBaseCrossConstruct::DrawInputDomyunViewCrossSectionMid(CDomyun* pDom)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) 
		return;

	const HGBOOL bInput = pBri->m_bIsExp || pBri->IsExtLine();
	if (bInput == FALSE && m_bShowSectionEnd == FALSE)
		return;

	HGDOUBLE dSta(0), dSta1(0);
	GetStation(1, dSta, dSta1);

	pDom->SetCalcExtRect();
	CDRect rtDomOrg = pDom->GetExtRect();

	CDomyun Dom(pDom);
	DrawInputDomyunViewCrossSection(&Dom, dSta, dSta1, bInput);

	Dom.SetCalcExtRect();
	CDRect rtDom = Dom.GetExtRect();

	Dom.Move(rtDomOrg.CenterPoint().x - rtDom.CenterPoint().x, rtDomOrg.top - rtDom.bottom - Dom.Always(20));

	*pDom << Dom;
}

void CInputBaseCrossConstruct::DrawInputDomyunViewCrossSectionEnd(CDomyun* pDom)
{
	if (m_bShowSectionEnd == FALSE)
		return;

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	HGDOUBLE dSta(0), dSta1(0);
	GetStation(2, dSta, dSta1);

	pDom->SetCalcExtRect();
	CDRect rtDomOrg = pDom->GetExtRect();

	CDomyun Dom(pDom);

	DrawInputDomyunViewCrossSection(&Dom, dSta, dSta1, FALSE);

	Dom.SetCalcExtRect();
	CDRect rtDom = Dom.GetExtRect();

	Dom.Move(rtDomOrg.CenterPoint().x - rtDom.CenterPoint().x, rtDomOrg.top - rtDom.bottom - Dom.Always(20));

	*pDom << Dom;
}

void CInputBaseCrossConstruct::DrawInputDomyunView(BOOL bZoomAll)
{
	// �⺻�Է½ô� ����� �Ź� ��������.
	m_pIn->SettingNeedPlane();

	for(long bri = 0; bri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		m_pIn->m_pARcBridgeDataStd->GetBridge(bri)->InitSaveCrossSection();
		m_pIn->m_pARcBridgeDataStd->GetBridge(bri)->InitSaveXySlabUpperByLenFrSttStation();
		m_pIn->m_pARcBridgeDataStd->GetBridge(bri)->InitSaveXyMatchLineAndSlabUpper();
	}

	//	SetDataSave();	//���� ���� ��ȣ�� ���� ����� �ξ���. 2005�� 7�� 15�Ͽ� Sample ���� �����ϰ� ���� ���� ���� ��
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(m_pIn->GetScaleInputDialog());

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	
	CDomyun Dom(pDom);
	
	DrawInputDomyunViewCrossSectionStt(&Dom);
	DrawInputDomyunViewCrossSectionMid(&Dom);
	DrawInputDomyunViewCrossSectionEnd(&Dom);

	DrawInputDomyunViewPlane(&Dom);

	*pDom << Dom;

	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

void CInputBaseCrossConstruct::OnButtonInsertCol() 
{
	long nTab = m_ctlTab.GetCurSel();
	if(nTab == m_nTabNori || nTab == m_nTabPave || nTab == m_nTabRail || nTab == m_nTabDistribute)		
		return;

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	CCellID cID = m_Grid.GetFocusCell();
	if(cID.col<0)
		cID.col = 1;
	long nCol = cID.col+1;

	if(nCol == m_Grid.GetCols())
	{
		m_Grid.InsertColumn(_T(""), DT_CENTER|DT_VCENTER|DT_SINGLELINE, nCol-1);
		m_Grid.SetTextMatrix(1, nCol-1, "%.f", m_Grid.GetTextMatrixDouble(1, nCol));
		m_Grid.SetCheck(2, nCol-1, m_Grid.IsCheck(2, nCol));
		m_Grid.SetTextMatrix(3, nCol-1, m_Grid.GetTextMatrix(3, nCol));
	}
	else
	{
		m_Grid.InsertColumn(_T(""), DT_CENTER|DT_VCENTER|DT_SINGLELINE, nCol);
	}

	double dPrevLen = m_Grid.GetTextMatrixDouble(1, nCol-1);
	m_Grid.SetTextMatrix(1, nCol-1, "%.f", dPrevLen/2);
	m_Grid.SetTextMatrix(1, nCol, "%.f", dPrevLen/2);
	m_Grid.SetTextMatrix(3, nCol, GetStringTypeHDanByNum(0, pStd->IsRailDesign()));

	// ��ȣ�� �� ������ ��輮
	long nQtyHDan = 0;
	CGuardWallRC* pGW = NULL;
	if(nTab == m_nTabSlab)
	{
		nQtyHDan = pBri->GetQtyHDan(FALSE)+1;
		pBri->SetQtyHDan(nQtyHDan);
		pBri->MakeGuardWall(nQtyHDan);
		pGW = pBri->GetGuardWallByHDan(nQtyHDan-1);
	}
	else if(nTab == m_nTabRoad)
	{
		nQtyHDan = pBri->GetQtyHDan(TRUE)+1;
		pBri->SetQtyHDanRoad(nQtyHDan);
		pBri->MakeGuardWall(nQtyHDan,TRUE);
		pGW = pBri->GetGuardWallByHDan(nQtyHDan-1, TRUE);
	}
	if(pGW)
	{
		pGW->m_nTypeGuardWall = HDAN_TYPE_ROAD_NONE;
		pGW->m_nNumberHDan = nCol-1;
		pGW->m_bRailRoad = pStd->IsRailDesign();
		pGW->m_bLeftGuardFence = pBri->IsLeftHDan(nCol-1);
		pGW->m_dWidhtHDan = pBri->GetLengthHDan(nCol-1);
		if(nTab == m_nTabSlab)
		{
			pBri->m_pArrGuardWall.InsertAt(nCol-1, pGW);
			pBri->m_pArrGuardWall.RemoveAt(nQtyHDan);
		}
		else if(nTab == m_nTabRoad)
		{
			pBri->m_pArrGuardWallRoad.InsertAt(nCol-1, pGW);
			pBri->m_pArrGuardWallRoad.RemoveAt(nQtyHDan);
		}
	}

	SetDataSave();
	SetDataInit();

	m_Grid.SetFocusCell(cID.row, cID.col);

	DrawInputDomyunView();
}

void CInputBaseCrossConstruct::OnButtonDeleteCol() 
{
	if(m_Grid.GetColumnCount()<=2)
	{
		AfxMessageBox(_T("��� �ϳ� �̻��� Ⱦ�� ������ �����Ͽ��� �մϴ�.  "));
		m_Grid.SetFocus();
		return;
	}

	long nTab = m_ctlTab.GetCurSel();
	if(nTab == m_nTabNori || nTab == m_nTabPave || nTab == m_nTabRail || nTab == m_nTabDistribute)
		return;

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	CCellID cID = m_Grid.GetFocusCell();
	long nRow = 1;
	long nCol = cID.col;
	if(nCol < 0) 
		return;
	
	if(nCol > m_Grid.GetColumnCount()-1)
	{
		return;
	}
	else if(nCol == m_Grid.GetColumnCount()-1)
	{
		if(nCol == 1) return;

		double dPrev = m_Grid.GetTextMatrixDouble(1, nCol-1);
		double dCurr = m_Grid.GetTextMatrixDouble(1, nCol);
		m_Grid.SetTextMatrix(nRow, nCol-1, "%.f", dPrev+dCurr);
	}
	else
	{
		double dCurr = m_Grid.GetTextMatrixDouble(1, nCol);
		double dNext = m_Grid.GetTextMatrixDouble(1, nCol+1);
		m_Grid.SetTextMatrix(nRow, nCol+1, "%.f", dCurr+dNext);
	}
	
	m_Grid.DeleteColumn(nCol);

	if(nTab == m_nTabSlab)
	{
		pBri->m_pArrGuardWall.RemoveAt(nCol-1);
	}
	else if(nTab == m_nTabRoad)
	{
		pBri->m_pArrGuardWallRoad.RemoveAt(nCol-1);
	}

	SetDataSave();
	SetDataInit();
	m_Grid.SetFocusCell(cID.row, nCol);
	DrawInputDomyunView();
}

// 
void CInputBaseCrossConstruct::SetDataDefault()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	if(IsClickDefaultButton(m_pIn->m_nCurrentBridge))	//�� �ǿ��� �ǰ�� ��ư�� Ŭ���Ͽ��� ���
	{
		CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
		long nTab = m_ctlTab.GetCurSel();

		if(nTab == m_nTabSlab)			pBri->SetDataDefaultHDanSlab();		// ������ Ⱦ��
		else if(nTab == m_nTabRoad)		pBri->SetDataDefaultHDanRoad();		// ����Ⱦ��, ö��Ⱦ��
		else if(nTab == m_nTabNori)		pBri->SetDataDefaultHDanNori();		// ������
		else if(nTab == m_nTabPave)		pBri->SetDataDefaultHDanPave();		// ��������
		else if(nTab == m_nTabRail)		pBri->SetDataDefaultHDanRail();		// ������ġ
		else if(nTab == m_nTabDistribute)	pBri->SetDataDefaultHDanDistribute();	// ������
		pBri->m_bNeedSettingVertSection	= TRUE;
	}
	else
	{
		for(long nBri = 0; nBri < pStd->GetBridgeSize(); nBri++)
		{
			CRcBridgeApp* pBri = pStd->GetBridge(nBri);
			BOOL bFirst   = IsFirst(nBri);
			pBri->SetDefaultedState(GetIDD(), 0x00000001);

			if(bFirst || IsClickDefaultButton(nBri)) 
			{
				pBri->SetDataDefaultHDanSlab();		// ������ Ⱦ��
				pBri->SetDataDefaultHDanRoad();		// ����Ⱦ��, ö��Ⱦ��
				pBri->SetDataDefaultHDanNori();		// ������
				pBri->SetDataDefaultHDanPave();		// ��������
				pBri->SetDataDefaultHDanLive();		// ��������
				pBri->SetDataDefaultHDanRail();		// ������ġ
				pBri->SetDataDefaultHDanDistribute();	// ������
				pBri->SetDataDefaultHDanRailLive();	// �Է��� ���߿��� ������ ���⼭ ���� �⺻���� ������.
				pBri->m_bNeedSettingVertSection	= TRUE;
			}
		}
	}

	m_pIn->m_pDoc->SetModifiedFlag();
}

void CInputBaseCrossConstruct::OnSelchangingTab(NMHDR* pNotifyStruct, LRESULT* result) 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	return;
}

void CInputBaseCrossConstruct::OnSelchangeTab(NMHDR* pNotifyStruct, LRESULT* result) 
{
	// TODO: Add your control notification handler code here
	SetDataInit();
	DrawInputDomyunView(TRUE);

	return;
}


// ���߶�౳�� ��쿡 ���������� �����ϰ�
// ���߶�౳�� �ƴ� ���� CARcBridgeDataStd�� ������ ������������ �����Ѵ�...
void CInputBaseCrossConstruct::MakeLinefoBridge(long nBridge)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	pStd->MakeLinefoBridge(nBridge);
}

void CInputBaseCrossConstruct::OnKillfocusEditTS() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	UpdateData();
	pBri->m_dTS = m_dTS;
	pBri->m_dTSOld = m_dTS;

	DrawInputDomyunView(FALSE);
}

void CInputBaseCrossConstruct::OnSetfocusEditTP() 
{
	m_pIn->SetComment(_T("������ �β��� �Է��մϴ�."));

	m_pView->GetDomyun()->STMakeCursor(_T("TP"));
}

void CInputBaseCrossConstruct::OnSetfocusEditTS() 
{
	m_pIn->SetComment(_T("�������� �β��� �Է��մϴ�."));

	m_pView->GetDomyun()->STMakeCursor(_T("TS"));
}

void CInputBaseCrossConstruct::OnCheckLevelRoadCenter() 
{
	UpdateData();	

	SetDataSave();
	SetDataInit();	
	DrawInputDomyunView();
}

void CInputBaseCrossConstruct::OnKillfocusHdaninDisBaseElevation() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	UpdateData();
	pBri->m_dDisBaseElevation = m_dDisBaseElevation;
	DrawInputDomyunView(FALSE);
}

void CInputBaseCrossConstruct::OnCheckApplyLineSlope() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	UpdateData();
	pBri->m_bApplyLineSlope = m_bApplyLineSlope;
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CInputBaseCrossConstruct::OnButtonGuardWall() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	CGuardWallDlg dlg(m_pIn, FALSE, GetParent());
	if(dlg.DoModal()==IDOK)
		pBri->m_bGuardWallDlgUsed = TRUE;

	DrawInputDomyunView(FALSE);
}

void CInputBaseCrossConstruct::OnButtonGuardWallUnder() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	CGuardWallDlg dlg(m_pIn, TRUE, GetParent());
	if(dlg.DoModal()==IDOK)
		pBri->m_bGuardWallUnderDlgUsed = TRUE;

	DrawInputDomyunView(FALSE);
}

void CInputBaseCrossConstruct::OnButtonSlabShape() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	CSlabShapeDlg dlg(m_pIn, GetParent());
	if(dlg.DoModal()==IDOK)
		pBri->m_bSSDlgUsed = TRUE;

	DrawInputDomyunView(FALSE);
}

void CInputBaseCrossConstruct::OnCheckViewCounterDir() 
{
	UpdateData(TRUE);
	m_pIn->m_bViewAllDir = m_bViewAllDir;
	DrawInputDomyunView();

}

void CInputBaseCrossConstruct::OnSize(UINT nType, int cx, int cy) 
{
	CInputDialogBase::OnSize(nType, cx, cy);
}

// �����̳� Ȯ���ε� ���������� �ϳ��� ���ٸ� ��� �޽���.
BOOL CInputBaseCrossConstruct::CheckVariableSection()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	long nCountBri	= pStd->GetBridgeSize();

	CStringArray sArrBridge;
	long i = 0; for(i = 0; i < nCountBri; i++)
	{
		CRcBridgeRebar *pBri	= pStd->GetBridgeRebar(i);
		if(pBri)
		{
			if(pBri->m_bIsExp || pBri->IsExtLine())
			{
				// (ARCBRIDGE-2530) ���������� �ʿ��Ѱ� ������ Ⱦ���̴�. ���߶���̶� ������ Ⱦ���� �˻��ؾ��Ѵ�.
				// if(pBri->GetCountVariableLenHDan(pBri->m_bUnderGroundRahmen) == 0)
				if(pBri->GetCountVariableLenHDan(FALSE) == 0)
				{
					sArrBridge.Add(pBri->m_strBridgeName);
				}
			}
		}
	}

	if(sArrBridge.GetSize() > 0)
	{
		CString str	= _T("");
		long i = 0; for(i = 0; i < sArrBridge.GetSize(); i++)
		{
			str += "["+sArrBridge.GetAt(i)+"]";
		}

		str = str + " ������ ����/Ȯ���̹Ƿ� ���������� �ݵ�� 1���̻� �����Ǿ�� �մϴ�.";
		m_pIn->SetComment(str, TRUE);
	}
	else
		m_pIn->SetComment("");

	return sArrBridge.GetSize() == 0;
}

BOOL CInputBaseCrossConstruct::IsOKMoveOtherDlg()
{
	if(!CheckVariableSection()) return FALSE;
	if(!CheckHDanZero()) return FALSE;

	return TRUE;
}

// ������ 0�� �Է� �Ǿ� ������ ���� �޽���
BOOL CInputBaseCrossConstruct::CheckHDanZero()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	long nCountBri	= pStd->GetBridgeSize();

	CStringArray sArrBridge;
	long i = 0; for(i = 0; i < nCountBri; i++)
	{
		CRcBridgeRebar *pBri	= pStd->GetBridgeRebar(i);
		if(pBri)
		{
			long nQtyHDan	= pBri->GetQtyHDan(FALSE);
			BOOL bZero	= FALSE;
			long i = 0; for(i = 0; i < nQtyHDan; i++)
			{
				if(pBri->GetLengthHDan(i, FALSE) <= 0)
				{
					bZero	= TRUE;
					break;
				}
			}

			if(bZero)
			{
				sArrBridge.Add(pBri->m_strBridgeName);
			}
		}
	}

	if(sArrBridge.GetSize() > 0)
	{
		CString str	= _T("");
		long i = 0; for(i = 0; i < sArrBridge.GetSize(); i++)
		{
			str += "["+sArrBridge.GetAt(i)+"]";
		}

		str = str + " ������ �������̰� 0������ ������ �����մϴ�. �������̴� �׻� 0���� ū ���̾�� �մϴ�.";
		m_pIn->SetComment(str, TRUE);
	}
	else
		m_pIn->SetComment("");

	return sArrBridge.GetSize() == 0;
}

void CInputBaseCrossConstruct::OnCheckViewHatch() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DrawInputDomyunView(FALSE);
}

void CInputBaseCrossConstruct::OnKillfocusEditRoadcenterLevelLeft() 
{
	// TODO: Add your control notification handler code here
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	UpdateData();
	pBri->m_dDistRoadCenterLevelLeft	= m_dDistRoadCenterLevelLeft;
	SetDataInit();
	DrawInputDomyunView(FALSE);	
}

void CInputBaseCrossConstruct::OnKillfocusEditRoadcenterLevelRight() 
{
	// TODO: Add your control notification handler code here
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	UpdateData();
	pBri->m_dDistRoadCenterLevelRight	= m_dDistRoadCenterLevelRight;
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CInputBaseCrossConstruct::OnBnClickedCheckViewPlane()
{
	UpdateData();

	DrawInputDomyunView(TRUE);
}

void CInputBaseCrossConstruct::OnBnClickedCheckViewEndCross()
{
	UpdateData();

	DrawInputDomyunView(TRUE);
}

void CInputBaseCrossConstruct::GetStation(HGINT32 nPos, HGDOUBLE &dSta, HGDOUBLE &dSta1)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) 
		return;

	if (nPos == 0 || nPos == 2)
	{
		const HGINT32 iJijum = nPos == 0 ? 0 : pBri->GetCountJijum() - 1;
		dSta = pBri->GetStationOnJijum(iJijum);
		if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
			dSta += pBri->GetJijum(0)->m_dLengthUgan[1];
	}
	else if(nPos == 1)
	{
		dSta = pBri->GetStationBridgeMid();
	}

	dSta1 = dSta;
	if(pBri->m_nSeparBri != 0 && m_pIn->m_bViewAllDir == TRUE)// ������ �и� ����(0:������ ��ü, 1:���༱, 2:���༱)
	{
		if(pBri->m_nSeparBri == 1)
			pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge+1);	//���༱ �� ���, ���༱(���༱+1)�� ǥ��, ���� ������ ���༱ ǥ��
		else
			pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge-1);	//���༱ �� ���, ���༱(���༱-1)�� ǥ��, ���� ������ ���༱ ǥ��

		CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);
		if (nPos == 0 || nPos == 2)
		{
			const HGINT32 iJijum = nPos == 0 ? 0 : pBri->GetCountJijum() - 1;
			dSta1 = pBri->GetStationOnJijum(iJijum);
			if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
				dSta1 += pBri->GetJijum(0)->m_dLengthUgan[1];
		}
		else if(nPos == 1)
		{
			dSta1 = pBri->GetStationBridgeMid();
		}
	}
}
