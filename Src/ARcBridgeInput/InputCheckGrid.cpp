// InputCheckGrid.cpp : implementation file
//

#include "stdafx.h"
#include "../../Src/ARcBridgeCalculate/ARcBridgeCalculate.h"
#include "ARcBridgeInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputCheckGrid dialog

#define LSD_CHECK_AXIAL_COEF		0.66
#define LSD_CHECK_AXIALMOMENT_COEF	1.00


CInputCheckGrid::CInputCheckGrid(CRcBridgeRebar *pBri, long nInputIDD, long nTypeCheck, CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CInputCheckGrid::IDD, pParent)
{
	m_pBri		 = pBri;
	m_nInputIDD  = nInputIDD;
	m_nTypeCheck = nTypeCheck;
	m_pCalc = new CARcBridgeCalcStd(pBri);
	m_bOK = TRUE;

	CalculateBridge();
	m_bUseUserOutput = m_pBri->m_bUseShellUserOutput;
}

CInputCheckGrid::CInputCheckGrid(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CInputCheckGrid::IDD, pParent)
{
	/*
	m_nInputIDD = 0;
	m_nTypeCheck = 0;
	m_pStmModel = NULL;
	m_bUseUserOutput = FALSE;
	*/
	
	m_pBri	= NULL;
	m_pCalc = NULL;
	m_bOK = TRUE;

	m_bUseUserOutput = FALSE;
	m_pStmModel = NULL;
	m_nTypeCheck = -1;
	m_nInputIDD = -1;
} 

CInputCheckGrid::~CInputCheckGrid()
{
	if(m_pCalc) delete m_pCalc;
}

void CInputCheckGrid::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputCheckGrid)
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_TAB1, m_ctlTab);
	DDX_Check(pDX, IDC_CHECK_USER_PLATE_OUTPUT, m_bUseUserOutput);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CInputCheckGrid, CXTResizeDialog)
	//{{AFX_MSG_MAP(CInputCheckGrid)
	ON_BN_CLICKED(IDC_CHECK_USER_PLATE_OUTPUT, OnCheckUserPlateOutput)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, OnTabSelchanging)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputCheckGrid message handlers
BOOL CInputCheckGrid::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetResize(IDC_GRID,	 SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_TAB1,	 SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK,		 SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,	 SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_CHECK_USER_PLATE_OUTPUT,	 SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);


	//////////////////////////////////////////////////////////////////////////
	if(m_nInputIDD==IDD_GEN_FRAME_ANLYSIS || m_nInputIDD==IDD_GEN_SHELL_ANLYSIS)
	{
		m_pBri->SetConcDataSlab(eEnvLimit);
		m_pBri->SetConcDataOutWall(eEnvLimit);
		m_pBri->SetConcDataInWall(eEnvLimit);
		m_pBri->SetConcDataPRF(eEnvLimit);

		if(m_pBri->m_pARcBridgeDataStd->IsLsdDesign())
		{
			MakeMatrixDesign_Lsd();
		}
		else
		{
			MakeMatrixDesign();
		}
	}

	InitTab();
	SetDataInit();

	ShowDlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputCheckGrid::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SetDataInit();
	return;
}

void CInputCheckGrid::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SetDataSave();
	return;
}

void CInputCheckGrid::InitTab()
{
	HGBOOL bLsd = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();

	m_ctlTab.SendInitialUpdate(TRUE);
	m_ctlTab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	long nTab = 0;
	switch(m_nInputIDD)
	{
		case IDD_GEN_FRAME_ANLYSIS :
		case IDD_GEN_SHELL_ANLYSIS :
		{
			SetWindowText("해석결과 집계");
			m_ctlTab.InsertItem(nTab++,"설계단면력");
			if(bLsd)
			{
				m_ctlTab.InsertItem(nTab++,"2D Frame");
			}
			else
			{
				m_ctlTab.InsertItem(nTab++,"2D Frame : 평상시");
				m_ctlTab.InsertItem(nTab++,"2D Frame : 지진시");
			}
			if(m_pBri->m_nSelect3DShellTool != 2)
				m_ctlTab.InsertItem(nTab++,"3D Plate");
			break;
		}
		case IDD_GEN_JONG_SECTION:
		{
			SetWindowText("기초 안정 검토");
			m_ctlTab.InsertItem(nTab++,"전도에 대한 안정");
			m_ctlTab.InsertItem(nTab++,"활동에 대한 안정");
			m_ctlTab.InsertItem(nTab++,"지지력에 대한 안정");
			m_ctlTab.InsertItem(nTab++, "기초 강체검토");
			if (bLsd)
				m_ctlTab.InsertItem(nTab++, "침하에 대한 안정");
			break;
		}
		case IDD_GEN_FOOTING:
		{
			m_nTab_Pile_Disp = -1;
			SetWindowText("말뚝 검토");
			m_ctlTab.InsertItem(nTab++,"허용지지력");

			CFootingApp *pFooting = m_pBri->GetFootingByJijumNumber(m_nTypeCheck);
			if(pFooting->m_exFooting.m_Pile.m_nType != PILE_TYPE_HYUNJANG) // 현장타설 말뚝이 아닐경우만..
			{
				if (bLsd && m_pBri->IsBoredPileASD(m_nTypeCheck) == FALSE && m_pBri->IsPhcPileASD(m_nTypeCheck) == FALSE)
				{
					if(pFooting->m_exFooting.m_Pile.m_nType == PILE_TYPE_STEEL || pFooting->m_exFooting.m_Pile.m_nType == PILE_TYPE_STEEL_DIGGING)
					{
						m_ctlTab.InsertItem(nTab++,"축력");
						m_ctlTab.InsertItem(nTab++,"축압축과 휨");
						m_ctlTab.InsertItem(nTab++,"전단응력");
					}
					// (ARCBRIDGE-3347) 수평변위 검토는 무조건 다 나오도록
					m_nTab_Pile_Disp = nTab;
					m_ctlTab.InsertItem(nTab++,"수평변위");
				}
				else
				{
					m_ctlTab.InsertItem(nTab++,"휨응력");
					m_ctlTab.InsertItem(nTab++,"전단응력");
					m_nTab_Pile_Disp = nTab;
					m_ctlTab.InsertItem(nTab++,"수평변위");
				}
			}
			else
			{
				m_nTab_Pile_Disp = nTab;
				m_ctlTab.InsertItem(nTab++, hggettext("수평변위"));
				m_ctlTab.InsertItem(nTab++,hggettext("말뚝본체"));
			}
			break;
		}
		case IDD_REBAR_MAIN:
		case IDD_REBAR_MAIN2:
		{
			SetWindowText("주철근 검토");
			m_ctlTab.InsertItem(nTab++,"단면력 검토");
			if (bLsd)
				m_ctlTab.InsertItem(nTab++,"사용성 검토");
			else
				m_ctlTab.InsertItem(nTab++,"균열 검토");
			break;
		}
		case IDD_REBAR_MAIN3:
			SetWindowText("우각부 검토");
			break;
		case IDD_REBAR_SECTION:
		{
			SetWindowText("단면 검토");
			m_ctlTab.InsertItem(nTab++,"전단 검토");
			if (bLsd)
				m_ctlTab.InsertItem(nTab++,"수평철근 검토");
			else
				m_ctlTab.InsertItem(nTab++,"배력철근 검토");
			break;
		}
		case IDD_REBAR_WINGWALL:
		{
			SetWindowText("날개벽 검토");

			m_ctlTab.InsertItem(nTab++,"단면력 검토");
			if (bLsd)
			{
				m_ctlTab.InsertItem(nTab++,"사용성 검토");
				m_ctlTab.InsertItem(nTab++,"수평철근 검토");
				m_ctlTab.InsertItem(nTab++,"전단검토");
			}
			else
			{
				m_ctlTab.InsertItem(nTab++,"균열 검토");
				m_ctlTab.InsertItem(nTab++,"전단 검토");
				m_ctlTab.InsertItem(nTab++,"배력철근 검토");
			}
			
			break;
		}
		case IDD_STM_ARC_CORNER:
		case IDD_STM_BRACKET:
		case IDD_STM_FOORING_MID:
		case IDD_STM_FOORING_STARTEND:
		{
			SetWindowText("STM 검토");

			m_ctlTab.InsertItem(0, hggettext("철근타이 강도검토"));
			m_ctlTab.InsertItem(1, hggettext("압축스트럿 강도검토"));
			m_ctlTab.InsertItem(2, hggettext("절점영역 강도검토"));
			if(bLsd == FALSE && m_nInputIDD != IDD_STM_ARC_CORNER)
			{
				// (ARCBRIDGE-2979) 우각부는 최소철근비 검토 안하도록.
				m_ctlTab.InsertItem(3, hggettext("최소철근비 검토"));
			}
			break;
		}
		case IDD_REBAR_APS:
		{
			m_ctlTab.InsertItem(nTab++,"단면력 검토");
			m_ctlTab.InsertItem(nTab++,"사용성 검토");
			m_ctlTab.InsertItem(nTab++,"수평철근 검토");
			m_ctlTab.InsertItem(nTab++,"전단검토");
			break;
		}
		default : break;
	}
}

void CInputCheckGrid::SetGridTitle()
{
	HGBOOL bLSD = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();

	m_Grid.DeleteAllItems();
	if(m_nInputIDD==IDD_GEN_FRAME_ANLYSIS || m_nInputIDD==IDD_GEN_SHELL_ANLYSIS) SetGridTitle_Analysis();
	if(m_nInputIDD==IDD_GEN_WING_SHELL_ANLYSIS)  SetGridTitle_Analysis_WingWall();
	if(m_nInputIDD==IDD_GEN_JONG_SECTION)	SetGridTitle_FootingSafty();
	if(m_nInputIDD==IDD_GEN_FOOTING)
	{
		SetGridTitle_FootingPile();
	}
	if(m_nInputIDD==IDD_REBAR_MAIN)		SetGridTitle_MainRebar();
	if(m_nInputIDD==IDD_REBAR_MAIN2)	SetGridTitle_MainRebar();
	if(m_nInputIDD==IDD_REBAR_MAIN3)	SetGridTitle_Corner();
	if(m_nInputIDD==IDD_REBAR_SECTION)	SetGridTitle_Section();
	if(m_nInputIDD==IDD_REBAR_WINGWALL) SetGridTitle_WingWall();
	if(m_nInputIDD==IDD_REBAR_COLUMN)   SetGridTitle_Column();
	if(m_nInputIDD == IDD_STM_ARC_CORNER || m_nInputIDD == IDD_STM_BRACKET || m_nInputIDD == IDD_STM_FOORING_MID || m_nInputIDD == IDD_STM_FOORING_STARTEND)   
		SetWindowText("STM 설계 검토");
	if (m_nInputIDD==IDD_GEN_DESIGN_VARIABLE) SetGridTitle_DesignVariable_ExposureRating();
	if (m_nInputIDD==IDD_REBAR_APS) SetGridTitle_RebarAps();
}

void CInputCheckGrid::SetDataInit()
{
//	계산하는 부분은 다이얼로그가 생성될때 한번만..
//	if(m_pBri->m_pArrConcWall.GetSize() == 0 || m_pBri->m_pArrConcWall.GetSize() == 0)
//		m_pBri->SetConcData(TRUE,TRUE,TRUE,FALSE);
//	m_pCalc->CalculateAll();

	HGBOOL bLSD = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();

	GetDlgItem(IDC_CHECK_USER_PLATE_OUTPUT)->ShowWindow(SW_HIDE);

	SetGridTitle();

	if(m_nInputIDD==IDD_GEN_FRAME_ANLYSIS || m_nInputIDD==IDD_GEN_SHELL_ANLYSIS)  SetDataInit_Analysis();
	if(m_nInputIDD==IDD_GEN_WING_SHELL_ANLYSIS)  SetDataInit_Analysis_WingWall();
	if(m_nInputIDD==IDD_GEN_JONG_SECTION)	SetDataInit_FootingSafty();
	if(m_nInputIDD==IDD_GEN_FOOTING)		SetDataInit_FootingPile();
	if(m_nInputIDD==IDD_REBAR_MAIN)			SetDataInit_MainRebar();
	if(m_nInputIDD==IDD_REBAR_MAIN2)		SetDataInit_MainRebar();
	if(m_nInputIDD==IDD_REBAR_MAIN3)		SetDataInit_Corner();
	if(m_nInputIDD==IDD_REBAR_SECTION)		SetDataInit_Section();
	if(m_nInputIDD==IDD_REBAR_WINGWALL)		SetDataInit_WingWall();
	if(m_nInputIDD==IDD_REBAR_COLUMN)		SetDataInit_Column();
	if(m_nInputIDD == IDD_STM_ARC_CORNER || m_nInputIDD == IDD_STM_BRACKET || m_nInputIDD == IDD_STM_FOORING_MID || m_nInputIDD == IDD_STM_FOORING_STARTEND)
	{
		SetDataInit_STM();
	}
	if (m_nInputIDD==IDD_GEN_DESIGN_VARIABLE)	SetDataInit_DesignVariable_ExposureRating();
	if (m_nInputIDD==IDD_REBAR_APS)			SetDataInit_RebarAps();

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputCheckGrid::SetGridTitle_Analysis_WingWall()
{
	long nTypeCalc = m_pBri->m_nSelectWingCalc;// 관용, 혼합, 3D

	BOOL bLsd = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();
	BOOL bManual = nTypeCalc != 2 ? TRUE : FALSE;
	BOOL bCalc3D = nTypeCalc != 0 ? TRUE : FALSE;

	long nSizeRow = nTypeCalc == 1 ? 1 + 9 : 1 + 6;
	if(bLsd)
	{
		if(bManual) nSizeRow += 1;
		if(bCalc3D) nSizeRow += 1;
		nSizeRow += 1;
	}

	long nSizeCol = 7; // 타이틀2, D,A,B,C,A'(추가단면)

	m_Grid.SetColumnCount(nSizeCol);
	m_Grid.SetRowCount(nSizeRow);

	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(2);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(70);
	m_Grid.SetColumnWidth(1, 120);

	m_Grid.SetTextMatrix(0,0,"구 분");
	m_Grid.SetMergeCol(0,0,1);

	long nRow = 0;
	long nCol = 2;
	m_Grid.SetTextMatrix(nRow, nCol++, "구간 A");
	m_Grid.SetTextMatrix(nRow, nCol++, "구간 B");
	m_Grid.SetTextMatrix(nRow, nCol++, "구간 C");
	m_Grid.SetTextMatrix(nRow, nCol++, "구간 D");
	m_Grid.SetTextMatrix(nRow, nCol, "구간 A'");

	nRow = 1;
	nCol = 0;
	if(bManual)
	{
		m_Grid.SetTextMatrix(nRow, nCol, "관용법");
		m_Grid.SetMergeRow(nRow, nRow + (bLsd? 3 : 2), 0);
		m_Grid.SetTextMatrix(nRow++, nCol+1, "계수 모멘트");
		m_Grid.SetTextMatrix(nRow++, nCol+1, "계수 전단력");
		if(bLsd)
		{
			m_Grid.SetTextMatrix(nRow++, nCol+1, "사용 모멘트/검토");
			m_Grid.SetTextMatrix(nRow++, nCol+1, "사용 모멘트/판정");
		}
		else
		{
			m_Grid.SetTextMatrix(nRow++, nCol+1, "사용 모멘트");
		}
	}
	if(bCalc3D)
	{
		m_Grid.SetTextMatrix(nRow, nCol, "3D 해석");
		m_Grid.SetMergeRow(nRow, nRow + (bLsd? 3 : 2), 0);
		m_Grid.SetTextMatrix(nRow++, nCol+1, "계수 모멘트");
		m_Grid.SetTextMatrix(nRow++, nCol+1, "계수 전단력");
		if(bLsd)
		{
			m_Grid.SetTextMatrix(nRow++, nCol+1, "사용 모멘트/검토");
			m_Grid.SetTextMatrix(nRow++, nCol+1, "사용 모멘트/판정");
		}
		else
		{
			m_Grid.SetTextMatrix(nRow++, nCol+1, "사용 모멘트");
		}
	}
	m_Grid.SetTextMatrix(nRow, nCol, "적용결과");
	m_Grid.SetMergeRow(nRow, nRow + (bLsd? 3 : 2), 0);
	m_Grid.SetTextMatrix(nRow++, nCol+1, "계수 모멘트");
	m_Grid.SetTextMatrix(nRow++, nCol+1, "계수 전단력");
	if(bLsd)
	{
		m_Grid.SetTextMatrix(nRow++, nCol+1, "사용 모멘트/검토");
		m_Grid.SetTextMatrix(nRow++, nCol+1, "사용 모멘트/판정");
	}
	else
	{
		m_Grid.SetTextMatrix(nRow++, nCol+1, "사용 모멘트");
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputCheckGrid::SetGridTitle_Analysis()
{
	BOOL bLsd = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();
	long nSizeRow = 2 + m_pBri->m_nQtyJigan*2 + 1;// 타이틀,슬래브
	nSizeRow += (m_pBri->m_nQtyJigan-1) * (m_pBri->m_nSelectInWallSectionDesign == 0 ? 5 : 1); //중간벽체
	if(m_pBri->IsRcSlab())		nSizeRow -= (m_pBri->m_nQtyJigan-1) * (m_pBri->m_nSelectInWallSectionDesign == 0 ? 5 : 1);		 // ,중간벽체
	if(m_pBri->IsBoxType()) nSizeRow += (m_pBri->m_nQtyJigan*2 + 1);
	if(m_pBri->IsOutsideWall()) nSizeRow += 5*2;							 // ,시종점벽체
	long nSizeCol = 6;
	long nJijumConnect = m_pBri->GetJijumConnectRibAndSlab();
	long nCntRowArchRib = m_pBri->m_pArrConcArchRib.GetSize();
	long nCntRowPRF = m_pBri->m_bExistProtectionRiseFooting ? 2 : 0;
	if(nJijumConnect > 0)
	{
		nSizeRow -= 1;
		nSizeRow += nCntRowArchRib;
	}
	nSizeRow += nCntRowPRF;

	BOOL bShell(FALSE);
	long nTab = m_ctlTab.GetCurSel();
	if(bLsd)
	{
		nSizeCol = 1 + (3*m_pBri->GetCountApplyEnvType());
	}
	else
	{
		bShell = (nTab == 3);
	}

	m_Grid.SetColumnCount(nSizeCol);
	m_Grid.SetRowCount(nSizeRow);
	m_Grid.SetFixedRowCount(2);
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(80);
	m_Grid.SetColumnWidth(0,180);

	m_Grid.SetTextMatrix(0,0,"구 분");
	m_Grid.SetMergeRow(0,1,0);

	if(bLsd)
	{
		long nCol = 1;
		for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
		{
			ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
			if(m_pBri->IsValidComboEnvType(eEnvType) == FALSE)
				continue;
			if(bShell && (eEnvType == eEnvExtremeI || eEnvType == eEnvExtremeII))
				continue;

			
			CString sTitle = m_pBri->GetStringComboEnvType_Lsd(eEnvType);

			m_Grid.SetTextMatrix(0, nCol, sTitle);
			m_Grid.SetMergeCol(0, nCol, nCol+2);
			m_Grid.SetTextMatrix(1,nCol++,"Mu("+m_pBri->m_pARcBridgeDataStd->m_szTonfM+")");
			m_Grid.SetTextMatrix(1,nCol++,"Vu("+m_pBri->m_pARcBridgeDataStd->m_szTonf+")");
			m_Grid.SetTextMatrix(1,nCol++,"Pu("+m_pBri->m_pARcBridgeDataStd->m_szTonf+")");
		}
	}
	else
	{
		m_Grid.SetTextMatrix(0,1,"계수하중");
		m_Grid.SetMergeCol(0,1,3);
		m_Grid.SetTextMatrix(1,1,"Mu("+m_pBri->m_pARcBridgeDataStd->m_szTonfM+")");
		m_Grid.SetTextMatrix(1,2,"Vu("+m_pBri->m_pARcBridgeDataStd->m_szTonf+")");
		m_Grid.SetTextMatrix(1,3,"Pu("+m_pBri->m_pARcBridgeDataStd->m_szTonf+")");
		m_Grid.SetTextMatrix(0,4,"사용하중");
		m_Grid.SetMergeCol(0,4,5);
		m_Grid.SetTextMatrix(1,4,"Ms("+m_pBri->m_pARcBridgeDataStd->m_szTonfM+")");
		m_Grid.SetTextMatrix(1,5,"Vs("+m_pBri->m_pARcBridgeDataStd->m_szTonf+")");
	}

	CString str = _T("");
	long nRow = 2;
	long nCol = 0;
	// 슬래브
	long i=0; 
	for(long iUD = iUPPER; iUD <= iLOWER; iUD++)
	{
		BOOL bUpperSlab = iUD == iUPPER ? TRUE : FALSE;
		if(iUD == iLOWER && m_pBri->IsBoxType() == FALSE) break;
		CString szPos = _T("슬래브");
		if(m_pBri->IsBoxType())
		{
			szPos = bUpperSlab ? _T("상부슬래브") :  _T("하부슬래브");
		}
		for(i=0; i<m_pBri->m_nQtyJigan; i++)
		{
			if(i==0)
				str.Format("  %s : 좌측단부",szPos);
			else
				str.Format("  %s : 지점부 %d",szPos,i+1);
			m_Grid.SetTextMatrix(nRow++,nCol,str,GVHT_LEFT,FALSE);
			str.Format("  %s : 중앙(지간%d)",szPos,i+1);
			m_Grid.SetTextMatrix(nRow++,nCol,str,GVHT_LEFT,FALSE);

			if(i!=m_pBri->m_nQtyJigan-1) continue;
			str.Format("  %s : 우측단부",szPos);
			m_Grid.SetTextMatrix(nRow++,nCol,str,GVHT_LEFT,FALSE);
		}
	}

	// 시종점벽체
	if(m_pBri->IsOutsideWall())
	{
		str.Format("  시점벽체 : 상단부");
		m_Grid.SetTextMatrix(nRow++,nCol,str,GVHT_LEFT,FALSE);
		str.Format("  시점벽체 : 중앙부(배면)");
		m_Grid.SetTextMatrix(nRow++,nCol,str,GVHT_LEFT,FALSE);
		str.Format("  시점벽체 : 중앙부(전면)");
		m_Grid.SetTextMatrix(nRow++,nCol,str,GVHT_LEFT,FALSE);
		str.Format("  시점벽체 : 하단부(배면)");
		m_Grid.SetTextMatrix(nRow++,nCol,str,GVHT_LEFT,FALSE);
		str.Format("  시점벽체 : 하단부(전면)");
		m_Grid.SetTextMatrix(nRow++,nCol,str,GVHT_LEFT,FALSE);
		str.Format("  종점벽체 : 상단부");
		m_Grid.SetTextMatrix(nRow++,nCol,str,GVHT_LEFT,FALSE);
		str.Format("  종점벽체 : 중앙부(배면)");
		m_Grid.SetTextMatrix(nRow++,nCol,str,GVHT_LEFT,FALSE);
		str.Format("  종점벽체 : 중앙부(전면)");
		m_Grid.SetTextMatrix(nRow++,nCol,str,GVHT_LEFT,FALSE);
		str.Format("  종점벽체 : 하단부(배면)");
		m_Grid.SetTextMatrix(nRow++,nCol,str,GVHT_LEFT,FALSE);
		str.Format("  종점벽체 : 하단부(전면)");
		m_Grid.SetTextMatrix(nRow++,nCol,str,GVHT_LEFT,FALSE);
	}

	// 아치리브
	if(nJijumConnect > 0)
	{
		long n=0; for(n=0; n<nCntRowArchRib; n++)
		{
			CConcBasic *pConc = m_pBri->m_pArrConcArchRib.GetAt(n);
			m_Grid.SetTextMatrix(nRow++,nCol,pConc->m_sTitle,GVHT_LEFT,FALSE);
		}
	}

	// 중간벽체
	if(m_pBri->IsRcSlab()==FALSE)
	{
		if(m_pBri->m_nSelectInWallSectionDesign == 0)
		{
			// 슬래브로 검토
			for(i=1; i<m_pBri->m_nQtyJigan; i++)
			{
				if(i == nJijumConnect) continue;
				str.Format("  중간벽체 %d : 상단부", i);
				m_Grid.SetTextMatrix(nRow++,nCol,str,GVHT_LEFT,FALSE);
				str.Format("  중간벽체 %d : 중앙부(배면)", i);
				m_Grid.SetTextMatrix(nRow++,nCol,str,GVHT_LEFT,FALSE);
				str.Format("  중간벽체 %d : 중앙부(전면)", i);
				m_Grid.SetTextMatrix(nRow++,nCol,str,GVHT_LEFT,FALSE);
				str.Format("  중간벽체 %d : 하단부(배면)", i);
				m_Grid.SetTextMatrix(nRow++,nCol,str,GVHT_LEFT,FALSE);
				str.Format("  중간벽체 %d : 하단부(전면)", i);
				m_Grid.SetTextMatrix(nRow++,nCol,str,GVHT_LEFT,FALSE);
			}
		}
		else
		{
			// 벽체로 검토
			for(i=1; i<m_pBri->m_nQtyJigan; i++)
			{
				if(i == nJijumConnect) continue;
				str.Format("  중간벽체 : 지점 %d",i);
				m_Grid.SetTextMatrix(nRow++,nCol,str,GVHT_LEFT,FALSE);
			}
		}
	}

	if(m_pBri->m_bExistProtectionRiseFooting)
	{
		str = _T("  부상방지턱 : 좌측");
		m_Grid.SetTextMatrix(nRow++,nCol,str,GVHT_LEFT,FALSE);

		str = _T("  부상방지턱 : 우측");
		m_Grid.SetTextMatrix(nRow++,nCol,str,GVHT_LEFT,FALSE);
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

// #define AREA_A		0
// #define AREA_B		1
// #define AREA_C		2
// #define AREA_D		3
// #define AREA_A_		4
void CInputCheckGrid::SetDataInit_Analysis_WingWall()
{
	long nTypeCalc = m_pBri->m_nSelectWingCalc;// 관용, 혼합, 3D

	BOOL bManual = nTypeCalc != 2 ? TRUE : FALSE;
	BOOL bCalc3D = nTypeCalc != 0 ? TRUE : FALSE;

	BOOL bStt  = (m_nTypeCheck == 0 || m_nTypeCheck == 1) ? TRUE : FALSE;
	BOOL bLeft = (m_nTypeCheck == 0 || m_nTypeCheck == 2) ? TRUE : FALSE;

	BOOL bLsd = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();
	CWingWall *pWing = m_pBri->GetWingWall(bStt, bLeft);
	CARcBridgeCalcWing *pCalcWing = m_pCalc->m_pWing[m_nTypeCheck];
	pCalcWing->CalculateAll(bStt, bLeft);

	long nRow = 1;
	long nCol = 2;

	if(bManual)
	{
		nCol = 2;
		UINT nState = m_Grid.GetItemState(nRow,nCol) | GVIS_READONLY;
		for(long ix = 0; ix < 5; ix++)
		{
			double dMomentManual = 0, dShearManual = 0, dMomentUseManual = 0, dMomentUseJudegManual = 0;
			dMomentManual = pCalcWing->m_Mu[ix];
			dShearManual = pCalcWing->m_Vu[ix];
			dMomentUseManual = pCalcWing->m_Mu_Use[ix];
			dMomentUseJudegManual = pCalcWing->m_Mu_Use_Judge[ix];

			m_Grid.SetItemState(nRow,nCol, nState); 
			m_Grid.SetItemBkColour(nRow, nCol, CLR_READONLY);
			m_Grid.SetTextMatrixFormat(nRow+0, nCol, bLsd? "%.0f" : "%.3f", dMomentManual);

			m_Grid.SetItemState(nRow+1,nCol, nState); 
			m_Grid.SetItemBkColour(nRow+1, nCol, CLR_READONLY);
			m_Grid.SetTextMatrixFormat(nRow+1, nCol, bLsd? "%.0f" : "%.3f", dShearManual);

			m_Grid.SetItemState(nRow+2,nCol, nState); 
			m_Grid.SetItemBkColour(nRow+2, nCol, CLR_READONLY);
			m_Grid.SetTextMatrixFormat(nRow+2, nCol, bLsd? "%.0f" : "%.3f", dMomentUseManual);

			if(bLsd)
			{
				m_Grid.SetItemState(nRow+3,nCol, nState); 
				m_Grid.SetItemBkColour(nRow+3, nCol, CLR_READONLY);
				m_Grid.SetTextMatrixFormat(nRow+3, nCol, bLsd? "%.0f" : "%.3f", dMomentUseJudegManual);
			}

			nCol++;
		}
		nRow += bLsd? 4 : 3;;
	}

	
	if(bCalc3D)
	{
		nCol = 2;
		UINT nState = (!m_bUseUserOutput) ? (m_Grid.GetItemState(nRow,nCol) | GVIS_READONLY) : (m_Grid.GetItemState(nRow,nCol) & ~GVIS_READONLY);
		for(long ix = 0; ix < 5; ix++)
		{
			double dMoment3D = 0, dShear3D = 0, dMomentUse3D = 0, dMomentUse3D_Judeg = 0;
			dMoment3D = pWing->m_dMomentUltPlate[ix];
			dShear3D = pWing->m_dShearUltPlate[ix];
			dMomentUse3D = pWing->m_dMomentUsePlate[ix];
			dMomentUse3D_Judeg = pWing->m_dMomentUseJudge_Plate[ix];
			m_Grid.SetItemState(nRow,nCol, nState); 
			m_Grid.SetItemBkColour(nRow, nCol, (nState & GVIS_READONLY) ? CLR_READONLY:CLR_DEFAULT);
			m_Grid.SetTextMatrixFormat(nRow+0, nCol, bLsd? "%.0f" : "%.3f", dMoment3D);

			m_Grid.SetItemState(nRow+1,nCol, nState); 
			m_Grid.SetItemBkColour(nRow+1, nCol, (nState & GVIS_READONLY) ? CLR_READONLY:CLR_DEFAULT);
			m_Grid.SetTextMatrixFormat(nRow+1, nCol, bLsd? "%.0f" : "%.3f", dShear3D);

			m_Grid.SetItemState(nRow+2,nCol, nState); 
			m_Grid.SetItemBkColour(nRow+2, nCol, (nState & GVIS_READONLY) ? CLR_READONLY:CLR_DEFAULT);
			m_Grid.SetTextMatrixFormat(nRow+2, nCol, bLsd? "%.0f" : "%.3f", dMomentUse3D);

			if(bLsd)
			{
				m_Grid.SetItemState(nRow+3,nCol, nState); 
				m_Grid.SetItemBkColour(nRow+3, nCol, (nState & GVIS_READONLY) ? CLR_READONLY:CLR_DEFAULT);
				m_Grid.SetTextMatrixFormat(nRow+3, nCol, bLsd? "%.0f" : "%.3f", dMomentUse3D_Judeg);
			}

			nCol++;
		}
		nRow += bLsd? 4 : 3;;
	}

	double dMoment = 0;
	double dShear = 0;
	double dMomentUse = 0;
	double dMomentUseJudeg = 0;
	nCol = 2;
	UINT nState = m_Grid.GetItemState(nRow,nCol) | GVIS_READONLY;
	for(long ix = 0; ix < 5; ix++)
	{
		if(bManual && bCalc3D)
		{
			dMoment = MAX(pCalcWing->m_Mu[ix], pWing->m_dMomentUltPlate[ix]);
			dShear = MAX(pCalcWing->m_Vu[ix], pWing->m_dShearUltPlate[ix]);
			dMomentUse = MAX(pCalcWing->m_Mu_Use[ix], pWing->m_dMomentUsePlate[ix]);
			dMomentUseJudeg = MAX(pCalcWing->m_Mu_Use_Judge[ix], pWing->m_dMomentUseJudge_Plate[ix]);
		}
		else if(bManual)
		{
			dMoment = pCalcWing->m_Mu[ix];
			dShear = pCalcWing->m_Vu[ix];
			dMomentUse = pCalcWing->m_Mu_Use[ix];
			dMomentUseJudeg = pCalcWing->m_Mu_Use_Judge[ix];
		}
		else
		{
			dMoment = pWing->m_dMomentUltPlate[ix];
			dShear = pWing->m_dShearUltPlate[ix];
			dMomentUse = pWing->m_dMomentUsePlate[ix];
			dMomentUseJudeg = pWing->m_dMomentUseJudge_Plate[ix];
		}

		m_Grid.SetItemState(nRow,nCol, nState); 
		m_Grid.SetItemBkColour(nRow, nCol, CLR_READONLY);
		m_Grid.SetTextMatrixFormat(nRow+0, nCol, bLsd? "%.0f" : "%.3f", dMoment);

		m_Grid.SetItemState(nRow+1,nCol, nState); 
		m_Grid.SetItemBkColour(nRow+1, nCol, CLR_READONLY);
		m_Grid.SetTextMatrixFormat(nRow+1, nCol, bLsd? "%.0f" : "%.3f", dShear);

		m_Grid.SetItemState(nRow+2,nCol, nState); 
		m_Grid.SetItemBkColour(nRow+2, nCol, CLR_READONLY);
		m_Grid.SetTextMatrixFormat(nRow+2, nCol, bLsd? "%.0f" : "%.3f", dMomentUse);

		if(bLsd)
		{
			m_Grid.SetItemState(nRow+3,nCol, nState); 
			m_Grid.SetItemBkColour(nRow+3, nCol, CLR_READONLY);
			m_Grid.SetTextMatrixFormat(nRow+3, nCol, bLsd? "%.0f" : "%.3f", dMomentUseJudeg);
		}

		nCol++;
	}

	m_bUseUserOutput = pWing->m_bUseUserOutput3D;
	GetDlgItem(IDC_CHECK_USER_PLATE_OUTPUT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_CHECK_USER_PLATE_OUTPUT)->EnableWindow(TRUE);
}

void CInputCheckGrid::SetDataInit_Analysis()
{
	BOOL bLsd = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();
	long nTab = m_ctlTab.GetCurSel();

	if(bLsd)
	{
		if(nTab==0) SetDataInit_Analysis_Design_Lsd();
		if(nTab==1) SetDataInit_Analysis_Frame_Lsd();
		if(nTab==2) SetDataInit_Analysis_Shell_Lsd();
	}
	else
	{
		if(nTab==0) SetDataInit_Analysis_Design();
		if(nTab==1) SetDataInit_Analysis_Frame();
		if(nTab==2) SetDataInit_Analysis_Frame();
		if(nTab==3) SetDataInit_Analysis_Shell();
	}

	long nTab3D = bLsd ? 2 : 3;
	m_bUseUserOutput = m_pBri->m_bUseShellUserOutput;
	GetDlgItem(IDC_CHECK_USER_PLATE_OUTPUT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_CHECK_USER_PLATE_OUTPUT)->EnableWindow(nTab==nTab3D);
}

void CInputCheckGrid::SetDataInit_Analysis_Frame()
{
	long nRow = 2;
	long nCol = 1;

	double dMoment = 0;
	double dShear  = 0;
	double dAxial  = 0;

	long nTab = m_ctlTab.GetCurSel();

	CMatrix Mat = (nTab==1) ? m_MatFrame : m_MatEQ;
	// 슬래브
	long i=0; 
	for(long iUD = iUPPER; iUD <= iLOWER; iUD++)
	{
		if(iUD == iLOWER && m_pBri->IsBoxType() == FALSE) break;
		for(i=0; i<m_pBri->m_nQtyJigan; i++)
		{
			long nPosEnd = (i==m_pBri->m_nQtyJigan-1) ? 3 : 2;
			for(long nPos = 0; nPos<nPosEnd; nPos++)
			{
				nCol = 1;
				dMoment = Mat(nRow-2,0);
				dShear  = Mat(nRow-2,1);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
				m_Grid.SetTextMatrix(nRow,nCol++,_T("-"));
				//
				if(nTab==2)
				{
					m_Grid.SetTextMatrix(nRow,nCol++,"");
					m_Grid.SetTextMatrix(nRow,nCol++,"");
				}
				else
				{
					dMoment = Mat(nRow-2,3);
					dShear  = Mat(nRow-2,4);
					m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
					m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
				}
				nRow++;	
			}
		}
	}
	// 시종점벽체
	if(m_pBri->IsOutsideWall())
	{
		for(i=0; i<2; i++)
		{
			long nCheckEnd = 5;
			for(long nCheck = 0; nCheck<nCheckEnd; nCheck++)
			{
				nCol = 1;
				dMoment = Mat(nRow-2,0);
				dShear  = Mat(nRow-2,1);
				dAxial  = Mat(nRow-2,2);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dAxial);
				if(nTab==2)
				{
					m_Grid.SetTextMatrix(nRow,nCol++,"");
					m_Grid.SetTextMatrix(nRow,nCol++,"");
				}
				else
				{
					dMoment = Mat(nRow-2,3);
					dShear  = Mat(nRow-2,4);
					m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
					m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
				}
				nRow++;	
			}
		}
	}

	// 아치리브
	long nJijumConnect = m_pBri->GetJijumConnectRibAndSlab();
	if(nJijumConnect > 0)
	{
		long nSize = m_pBri->m_pArrConcArchRib.GetSize();
		long n=0; for(n=0; n<nSize; n++)
		{
			nCol = 1;
			dMoment = Mat(nRow-2,0);
			dShear  = Mat(nRow-2,1);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
			m_Grid.SetTextMatrix(nRow,nCol++,_T("-"));
			//
			if(nTab==2)
			{
				m_Grid.SetTextMatrix(nRow,nCol++,"");
				m_Grid.SetTextMatrix(nRow,nCol++,"");
			}
			else
			{
				dMoment = Mat(nRow-2,3);
				dShear  = Mat(nRow-2,4);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
			}
			nRow++;	
		}
	}

	// 중간벽체
	if(m_pBri->IsRcSlab()) return;
	for(long nJijum=1; nJijum<m_pBri->m_nQtyJigan; nJijum++)
	{
		if(nJijum == nJijumConnect) continue;

		if(m_pBri->m_nSelectInWallSectionDesign == 0)
		{
			long nCheckEnd = 5;
			for(long nCheck = 0; nCheck<nCheckEnd; nCheck++)
			{
				nCol = 1;
				dMoment = Mat(nRow-2,0);
				dShear  = Mat(nRow-2,1);
				dAxial  = Mat(nRow-2,2);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dAxial);
				if(nTab==2)
				{
					m_Grid.SetTextMatrix(nRow,nCol++,"");
					m_Grid.SetTextMatrix(nRow,nCol++,"");
				}
				else
				{
					dMoment = Mat(nRow-2,3);
					dShear  = Mat(nRow-2,4);
					m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
					m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
				}
				nRow++;	
			}
		}
		else
		{
			nCol = 1;
			dMoment = Mat(nRow-2,0);
			dShear  = Mat(nRow-2,1);
			dAxial  = Mat(nRow-2,2);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dAxial);
			m_Grid.SetTextMatrix(nRow,nCol++,(nTab==2) ? _T("") : _T("-"));
			m_Grid.SetTextMatrix(nRow,nCol++,(nTab==2) ? _T("") : _T("-"));
			nRow++;	
		}
	}

	//부상방지턱
	if(m_pBri->m_bExistProtectionRiseFooting)
	{
		for(long iSE=iSTT; iSE<=iEND; ++iSE)
		{			
			nCol = 1;
			dMoment = Mat(nRow-2,0);
			dShear  = Mat(nRow-2,1);
			dAxial  = Mat(nRow-2,2);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dAxial);
			if(nTab==2)
			{
				m_Grid.SetTextMatrix(nRow,nCol++,"");
				m_Grid.SetTextMatrix(nRow,nCol++,"");
			}
			else
			{
				dMoment = Mat(nRow-2,3);
				dShear  = Mat(nRow-2,4);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
			}
			nRow++;			
		}
	}
}

void CInputCheckGrid::SetDataInit_Analysis_Frame_Lsd()
{
	long nRow = 2;
	long nCol = 1;

	double dMoment = 0;
	double dShear  = 0;
	double dAxial  = 0;

	CMatrix Mat = m_MatFrame;
	// 슬래브
	long i=0; 
	for(long iUD = iUPPER; iUD <= iLOWER; iUD++)
	{
		if(iUD == iLOWER && m_pBri->IsBoxType() == FALSE) break;
		for(i=0; i<m_pBri->m_nQtyJigan; i++)
		{
			long nPosEnd = (i==m_pBri->m_nQtyJigan-1) ? 3 : 2;
			for(long nPos = 0; nPos<nPosEnd; nPos++)
			{
				nCol = 1;
				for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
				{
					ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
					if(m_pBri->IsValidComboEnvType(eEnvType) == FALSE)
						continue;

					dMoment = Mat(nRow-2, nCol-1);
					dShear  = Mat(nRow-2, nCol);
					m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dMoment);
					m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dShear);
					m_Grid.SetTextMatrix(nRow,nCol++,_T("-"));
				}
				nRow++;	
			}
		}
	}
	// 시종점벽체
	if(m_pBri->IsOutsideWall())
	{
		for(i=0; i<2; i++)
		{
			long nCheckEnd = 5;
			for(long nCheck = 0; nCheck<nCheckEnd; nCheck++)
			{
				nCol = 1;
				for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
				{
					ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
					if(m_pBri->IsValidComboEnvType(eEnvType) == FALSE)
						continue;

					dMoment = Mat(nRow-2, nCol-1);
					dShear  = Mat(nRow-2, nCol);
					dAxial  = Mat(nRow-2, nCol+1);
					m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dMoment);
					m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dShear);
					m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dAxial);
				}
				nRow++;	
			}
		}
	}

	// 중간벽체
	if(m_pBri->IsRcSlab()) return;
	for(long nJijum=1; nJijum<m_pBri->m_nQtyJigan; nJijum++)
	{
		long nCheckEnd = (m_pBri->m_nSelectInWallSectionDesign == 0)? 5 : 1;
		for(long nCheck = 0; nCheck<nCheckEnd; nCheck++)
		{
			nCol = 1;
			for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
			{
				ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
				if(m_pBri->IsValidComboEnvType(eEnvType) == FALSE)
					continue;

				dMoment = Mat(nRow-2, nCol-1);
				dShear  = Mat(nRow-2, nCol);
				dAxial  = Mat(nRow-2, nCol+1);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dMoment);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dShear);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dAxial);
			}
			nRow++;	
		}
	}

	// 부상방지저판
	if(m_pBri->m_bExistProtectionRiseFooting)
	{
		for(long iSE=iSTT; iSE<=iEND; ++iSE)
		{
			nCol = 1;
			for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
			{
				ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
				if(m_pBri->IsValidComboEnvType(eEnvType) == FALSE)
					continue;

				dMoment = Mat(nRow-2, nCol-1);
				dShear  = Mat(nRow-2, nCol);
				dAxial  = Mat(nRow-2, nCol+1);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dMoment);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dShear);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dAxial);
			}
			nRow++;				
		}
	}
}

void CInputCheckGrid::SetDataInit_Analysis_Design()
{
	long nRow = 2;
	long nCol = 1;

	double dMoment = 0;
	double dShear  = 0;
	double dAxial  = 0;

	// 슬래브
	long i=0; 
	for(long iUD = iUPPER; iUD <= iLOWER; iUD++)
	{
		if(iUD == iLOWER && m_pBri->IsBoxType() == FALSE) break;
		for(i=0; i<m_pBri->m_nQtyJigan; i++)
		{
			long nPosEnd = (i==m_pBri->m_nQtyJigan-1) ? 3 : 2;
			for(long nPos = 0; nPos<nPosEnd; nPos++)
			{
				nCol = 1;
				dMoment = GetDesignValue(nRow-2,0);
				dShear  = GetDesignValue(nRow-2,1);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
				m_Grid.SetTextMatrix(nRow,nCol++,_T("-"));
				//
				dMoment = GetDesignValue(nRow-2,3);
				dShear  = GetDesignValue(nRow-2,4);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
				//
				nRow++;	
			}
		}
	}
	// 시종점벽체
	if(m_pBri->IsOutsideWall())
	{
		for(i=0; i<2; i++)
		{
			long nCheckEnd = 5;
			for(long nCheck = 0; nCheck<nCheckEnd; nCheck++)
			{
				nCol = 1;
				dMoment = GetDesignValue(nRow-2,0);
				dShear  = GetDesignValue(nRow-2,1);
				dAxial  = GetDesignValue(nRow-2,2);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dAxial);
				//
				dMoment = GetDesignValue(nRow-2,3);
				dShear  = GetDesignValue(nRow-2,4);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
				//
				nRow++;	
			}
		}
	}
	if(m_pBri->IsRcSlab()) return;

	// 아치리브
	long nJijumConnect = m_pBri->GetJijumConnectRibAndSlab();
	if(nJijumConnect > 0)
	{
		long nSize = m_pBri->m_pArrConcArchRib.GetSize();
		long n=0; for(n=0; n<nSize; n++)
		{
			nCol = 1;
			dMoment = GetDesignValue(nRow-2,0);
			dShear  = GetDesignValue(nRow-2,1);
			dAxial  = GetDesignValue(nRow-2,2);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
			m_Grid.SetTextMatrix(nRow,nCol++,_T("-"));
			//
			dMoment = GetDesignValue(nRow-2,3);
			dShear  = GetDesignValue(nRow-2,4);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
			//
			nRow++;	
		}
	}

	// 중간벽체
	for(long nJijum=1; nJijum<m_pBri->m_nQtyJigan; nJijum++)
	{
		nCol = 1;
		if(nJijum == nJijumConnect) continue;
		if(m_pBri->m_nSelectInWallSectionDesign == 0)
		{
			long nCheckEnd = 5;
			for(long nCheck = 0; nCheck<nCheckEnd; nCheck++)
			{
				nCol = 1;
				dMoment = GetDesignValue(nRow-2,0);
				dShear  = GetDesignValue(nRow-2,1);
				dAxial  = GetDesignValue(nRow-2,2);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dAxial);
				//
				dMoment = GetDesignValue(nRow-2,3);
				dShear  = GetDesignValue(nRow-2,4);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
				//
				nRow++;	
			}
		}
		else
		{
			dMoment = GetDesignValue(nRow-2,0);
			dShear  = GetDesignValue(nRow-2,1);
			dAxial  = GetDesignValue(nRow-2,2);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dAxial);
			m_Grid.SetTextMatrix(nRow,nCol++,_T("-"));
			m_Grid.SetTextMatrix(nRow,nCol++,_T("-"));
			nRow++;	
		}
	}

	if(m_pBri->m_bExistProtectionRiseFooting)
	{
		for(long iSE=iSTT; iSE<=iEND; ++iSE)
		{			
			nCol = 1;
			dMoment = GetDesignValue(nRow-2,0);
			dShear  = GetDesignValue(nRow-2,1);
			dAxial  = GetDesignValue(nRow-2,2);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dAxial);
			//
			dMoment = GetDesignValue(nRow-2,3);
			dShear  = GetDesignValue(nRow-2,4);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
			//
			nRow++;				
		}
	}
}

void CInputCheckGrid::SetDataInit_Analysis_Design_Lsd()
{
	long nRow = 2;
	long nCol = 1;

	double dMoment = 0;
	double dShear  = 0;
	double dAxial  = 0;

	// 슬래브
	long i=0; 
	for(long iUD = iUPPER; iUD <= iLOWER; iUD++)
	{
		if(iUD == iLOWER && m_pBri->IsBoxType() == FALSE) break;
		for(i=0; i<m_pBri->m_nQtyJigan; i++)
		{
			long nPosEnd = (i==m_pBri->m_nQtyJigan-1) ? 3 : 2;
			for(long nPos = 0; nPos<nPosEnd; nPos++)
			{
				nCol = 1;
				for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
				{
					ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
					if(m_pBri->IsValidComboEnvType(eEnvType) == FALSE)
						continue;

					dMoment = GetDesignValue(nRow-2, nCol-1);
					dShear  = GetDesignValue(nRow-2, nCol);
					m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dMoment);
					m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dShear);
					m_Grid.SetTextMatrix(nRow,nCol++,_T("-"));
				}
				
				nRow++;	
			}
		}
	}
	// 시종점벽체
	if(m_pBri->IsOutsideWall())
	{
		for(i=0; i<2; i++)
		{
			long nCheckEnd = 5;
			for(long nCheck = 0; nCheck<nCheckEnd; nCheck++)
			{
				nCol = 1;
				for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
				{
					ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
					if(m_pBri->IsValidComboEnvType(eEnvType) == FALSE)
						continue;

					dMoment = GetDesignValue(nRow-2, nCol-1);
					dShear  = GetDesignValue(nRow-2, nCol);
					dAxial  = GetDesignValue(nRow-2, nCol+1);
					m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dMoment);
					m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dShear);
					m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dAxial);
				}
				nRow++;	
			}
		}
	}
	if(m_pBri->IsRcSlab()) return;

	// 중간벽체
	for(long nJijum=1; nJijum<m_pBri->m_nQtyJigan; nJijum++)
	{
	
		long nCheckEnd = (m_pBri->m_nSelectInWallSectionDesign == 0)? 5 : 1;
		for(long nCheck = 0; nCheck<nCheckEnd; nCheck++)
		{
			nCol = 1;
			for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
			{
				ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
				if(m_pBri->IsValidComboEnvType(eEnvType) == FALSE)
					continue;

				dMoment = GetDesignValue(nRow-2, nCol-1);
				dShear  = GetDesignValue(nRow-2, nCol);
				dAxial  = GetDesignValue(nRow-2, nCol+1);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dMoment);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dShear);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dAxial);
			}
			nRow++;	
		}
	}

	// 부상방지저판
	if(m_pBri->m_bExistProtectionRiseFooting)
	{
		for(long iSE = iSTT; iSE <= iEND; ++iSE)
		{
			nCol = 1;
			for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
			{
				ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
				if(m_pBri->IsValidComboEnvType(eEnvType) == FALSE)
					continue;

				dMoment = GetDesignValue(nRow-2, nCol-1);
				dShear  = GetDesignValue(nRow-2, nCol);
				dAxial  = GetDesignValue(nRow-2, nCol+1);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dMoment);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dShear);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dAxial);
			}
			nRow++;
		}
	}
}

void CInputCheckGrid::SetDataInit_Analysis_Shell()
{
	long nRow = 2;
	long nCol = 1;

	double dMoment = 0;
	double dShear  = 0;
	//double dAxial  = 0;

	// 슬래브
	long i=0; 
	for(long iUD = iUPPER; iUD <= iLOWER; iUD++)
	{
		if(iUD == iLOWER && m_pBri->IsBoxType() == FALSE) break;
		for(i=0; i<m_pBri->m_nQtyJigan; i++)
		{
			long nPosEnd = (i==m_pBri->m_nQtyJigan-1) ? 3 : 2;
			for(long nPos = 0; nPos<nPosEnd; nPos++)
			{
				nCol = 1;
				dMoment = m_MatShell(nRow-2,0);
				dShear  = m_MatShell(nRow-2,1);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);

				UINT nState = (m_bUseUserOutput)? (m_Grid.GetItemState(nRow,nCol) | GVIS_READONLY):(m_Grid.GetItemState(nRow,nCol) & ~GVIS_READONLY);
				m_Grid.SetItemState(nRow,nCol, nState); m_Grid.SetItemBkColour(nRow, nCol, (nState & GVIS_READONLY)? CLR_READONLY:CLR_DEFAULT);
				m_Grid.SetTextMatrix(nRow,nCol++,_T("-"));
				//
				dMoment = m_MatShell(nRow-2,3);
				dShear  = m_MatShell(nRow-2,4);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
				//
				nRow++;	
			}
		}
	}
	// 시종점벽체
	if(m_pBri->IsOutsideWall())
	{
		for(i=0; i<2; i++)
		{
			long nCheckEnd = 5;
			for(long nCheck = 0; nCheck<nCheckEnd; nCheck++)
			{
				nCol = 1;
				dMoment = m_MatShell(nRow-2,0);
				dShear  = m_MatShell(nRow-2,1);
				// dAxial  = m_MatShell(nRow-2,2);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
				// m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dAxial);
				UINT nState = (m_bUseUserOutput)? (m_Grid.GetItemState(nRow,nCol) | GVIS_READONLY):(m_Grid.GetItemState(nRow,nCol) & ~GVIS_READONLY);
				m_Grid.SetItemState(nRow,nCol, nState); m_Grid.SetItemBkColour(nRow, nCol, (nState & GVIS_READONLY)? CLR_READONLY:CLR_DEFAULT);
				m_Grid.SetTextMatrix(nRow,nCol++,_T("-"));
				//
				dMoment = m_MatShell(nRow-2,3);
				dShear  = m_MatShell(nRow-2,4);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
				//
				nRow++;	
			}
		}
	}

	// 아치리브
	long nJijumConnect = m_pBri->GetJijumConnectRibAndSlab();
	if(nJijumConnect > 0)
	{
		long nSize = m_pBri->m_pArrConcArchRib.GetSize();
		long n=0; for(n=0; n<nSize; n++)
		{
			nCol = 1;
			dMoment = m_MatShell(nRow-2,0);
			dShear  = m_MatShell(nRow-2,1);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
			UINT nState = (m_bUseUserOutput)? (m_Grid.GetItemState(nRow,nCol) | GVIS_READONLY):(m_Grid.GetItemState(nRow,nCol) & ~GVIS_READONLY);
			m_Grid.SetItemState(nRow,nCol, nState); m_Grid.SetItemBkColour(nRow, nCol, (nState & GVIS_READONLY)? CLR_READONLY:CLR_DEFAULT);
			m_Grid.SetTextMatrix(nRow,nCol++,_T("-"));
			//
			dMoment = m_MatShell(nRow-2,3);
			dShear  = m_MatShell(nRow-2,4);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
			//
			nRow++;	
		}
	}

	// 중간벽체
	if(m_pBri->IsRcSlab()) return;
	for(long nJijum=1; nJijum<m_pBri->m_nQtyJigan; nJijum++)
	{
		if(nJijum == nJijumConnect) continue;
		nCol = 1;

		if(m_pBri->m_nSelectInWallSectionDesign == 0)
		{
			long nCheckEnd = 5;
			for(long nCheck = 0; nCheck<nCheckEnd; nCheck++)
			{
				nCol = 1;
				dMoment = m_MatShell(nRow-2,0);
				dShear  = m_MatShell(nRow-2,1);
				// dAxial  = m_MatShell(nRow-2,2);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
				// m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dAxial);
				UINT nState = (m_bUseUserOutput)? (m_Grid.GetItemState(nRow,nCol) | GVIS_READONLY):(m_Grid.GetItemState(nRow,nCol) & ~GVIS_READONLY);
				m_Grid.SetItemState(nRow,nCol, nState); m_Grid.SetItemBkColour(nRow, nCol, (nState & GVIS_READONLY)? CLR_READONLY:CLR_DEFAULT);
				m_Grid.SetTextMatrix(nRow,nCol++,_T("-"));
				//
				dMoment = m_MatShell(nRow-2,3);
				dShear  = m_MatShell(nRow-2,4);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
				//
				nRow++;	
			}
		}
		else
		{
			dMoment = m_MatShell(nRow-2,0);
			dShear  = m_MatShell(nRow-2,1);
			// dAxial  = m_MatShell(nRow-2,2);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
			// m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dAxial);
			UINT nState = (m_bUseUserOutput)? (m_Grid.GetItemState(nRow,nCol) | GVIS_READONLY):(m_Grid.GetItemState(nRow,nCol) & ~GVIS_READONLY);
			m_Grid.SetItemState(nRow,nCol, nState); m_Grid.SetItemBkColour(nRow, nCol, (nState & GVIS_READONLY)? CLR_READONLY:CLR_DEFAULT);
			m_Grid.SetTextMatrix(nRow,nCol++,_T("-"));

			m_Grid.SetTextMatrix(nRow,nCol++,_T(""));
			m_Grid.SetTextMatrix(nRow,nCol++,_T(""));
			nRow++;	
		}
	}

	//부상방지턱
	if(m_pBri->m_bExistProtectionRiseFooting)
	{
		for(long iSE=iSTT; iSE<=iEND; ++iSE)
		{			
			nCol = 1;
			dMoment = m_MatShell(nRow-2,0);
			dShear  = m_MatShell(nRow-2,1);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
			UINT nState = (m_bUseUserOutput)? (m_Grid.GetItemState(nRow,nCol) | GVIS_READONLY):(m_Grid.GetItemState(nRow,nCol) & ~GVIS_READONLY);
			m_Grid.SetItemState(nRow,nCol, nState); m_Grid.SetItemBkColour(nRow, nCol, (nState & GVIS_READONLY)? CLR_READONLY:CLR_DEFAULT);
			m_Grid.SetTextMatrix(nRow,nCol++,_T("-"));
			
			dMoment = m_MatShell(nRow-2,3);
			dShear  = m_MatShell(nRow-2,4);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dMoment);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f",dShear);
			
			nRow++;				
		}
	}
}

void CInputCheckGrid::SetDataInit_Analysis_Shell_Lsd()
{
	long nRow = 2;
	long nCol = 1;

	double dMoment = 0;
	double dShear  = 0;
	//double dAxial  = 0;

	// 슬래브
	long i=0; 
	for(long iUD = iUPPER; iUD <= iLOWER; iUD++)
	{
		if(iUD == iLOWER && m_pBri->IsBoxType() == FALSE) break;
		for(i=0; i<m_pBri->m_nQtyJigan; i++)
		{
			long nPosEnd = (i==m_pBri->m_nQtyJigan-1) ? 3 : 2;
			for(long nPos = 0; nPos<nPosEnd; nPos++)
			{
				nCol = 1;
				for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
				{
					ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
					if(m_pBri->IsValidComboEnvType(eEnvType) == FALSE)
						continue;

					dMoment = m_MatShell(nRow-2, nCol-1);
					dShear  = m_MatShell(nRow-2, nCol);
					m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dMoment);
					m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dShear);

					UINT nState = (m_bUseUserOutput)? (m_Grid.GetItemState(nRow,nCol) | GVIS_READONLY):(m_Grid.GetItemState(nRow,nCol) & ~GVIS_READONLY);
					m_Grid.SetItemState(nRow,nCol, nState); m_Grid.SetItemBkColour(nRow, nCol, (nState & GVIS_READONLY)? CLR_READONLY:CLR_DEFAULT);
					m_Grid.SetTextMatrix(nRow,nCol++,_T("-"));
				}
				nRow++;	
			}
		}
	}
	// 시종점벽체
	if(m_pBri->IsOutsideWall())
	{
		for(i=0; i<2; i++)
		{
			long nCheckEnd = 5;
			for(long nCheck = 0; nCheck<nCheckEnd; nCheck++)
			{
				nCol = 1;
				for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
				{
					ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
					if(m_pBri->IsValidComboEnvType(eEnvType) == FALSE)
						continue;

					dMoment = m_MatShell(nRow-2, nCol-1);
					dShear  = m_MatShell(nRow-2, nCol);
					m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dMoment);
					m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dShear);
					UINT nState = (m_bUseUserOutput)? (m_Grid.GetItemState(nRow,nCol) | GVIS_READONLY):(m_Grid.GetItemState(nRow,nCol) & ~GVIS_READONLY);
					m_Grid.SetItemState(nRow,nCol, nState); m_Grid.SetItemBkColour(nRow, nCol, (nState & GVIS_READONLY)? CLR_READONLY:CLR_DEFAULT);
					m_Grid.SetTextMatrix(nRow,nCol++,_T("-"));
				}
				nRow++;	
			}
		}
	}

	// 중간벽체
	if(m_pBri->IsRcSlab()) return;
	for(long nJijum=1; nJijum<m_pBri->m_nQtyJigan; nJijum++)
	{
		long nCheckEnd = (m_pBri->m_nSelectInWallSectionDesign == 0)? 5 : 1;
		for(long nCheck = 0; nCheck<nCheckEnd; nCheck++)
		{
			nCol = 1;
			for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
			{
				ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
				if(m_pBri->IsValidComboEnvType(eEnvType) == FALSE)
					continue;

				dMoment = m_MatShell(nRow-2, nCol-1);
				dShear  = m_MatShell(nRow-2, nCol);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dMoment);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dShear);
				UINT nState = (m_bUseUserOutput)? (m_Grid.GetItemState(nRow,nCol) | GVIS_READONLY):(m_Grid.GetItemState(nRow,nCol) & ~GVIS_READONLY);
				m_Grid.SetItemState(nRow,nCol, nState); m_Grid.SetItemBkColour(nRow, nCol, (nState & GVIS_READONLY)? CLR_READONLY:CLR_DEFAULT);
				m_Grid.SetTextMatrix(nRow,nCol++,_T("-"));
			}
			nRow++;	
		}
	}

	// 부상방지저판
	if(m_pBri->m_bExistProtectionRiseFooting)
	{
		for(long iSE=iSTT; iSE<=iEND; ++iSE)
		{
			nCol = 1;
			for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
			{
				ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
				if(m_pBri->IsValidComboEnvType(eEnvType) == FALSE)
					continue;

				dMoment = m_MatShell(nRow-2, nCol-1);
				dShear  = m_MatShell(nRow-2, nCol);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dMoment);
				m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.0f",dShear);
				UINT nState = (m_bUseUserOutput)? (m_Grid.GetItemState(nRow,nCol) | GVIS_READONLY):(m_Grid.GetItemState(nRow,nCol) & ~GVIS_READONLY);
				m_Grid.SetItemState(nRow,nCol, nState); m_Grid.SetItemBkColour(nRow, nCol, (nState & GVIS_READONLY)? CLR_READONLY:CLR_DEFAULT);
				m_Grid.SetTextMatrix(nRow,nCol++,_T("-"));
			}
			nRow++;				
		}
	}
}

void CInputCheckGrid::SetGridTitle_FootingSafty()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pData->IsLsdDesign();
	if (bLsd)
	{
		SetGridTitle_FootingSafty_Lsd();
		return;
	}

	long nTab = m_ctlTab.GetCurSel();
	long nSizeRow = 5; // 타이틀 1, 하중조합 4
	long nSizeCol = 0;
	CString str = _T("");
	
	switch(nTab)
	{
	case 0:	nSizeCol = 8; break;
	case 1:	nSizeCol = 7; break;
	case 2:	nSizeCol = 8; break;
	case 3: nSizeCol = 5; nSizeRow = 2; break;
	}
	
	if(nTab < 3)
	{
		m_Grid.SetColumnCount(nSizeCol);
		m_Grid.SetRowCount(nSizeRow);
		m_Grid.SetFixedRowCount(1);
		m_Grid.SetFixedColumnCount();
		m_Grid.SetRowHeightAll(20);
		m_Grid.SetColumnWidthAll(70);
		m_Grid.SetColumnWidth(0,150);
		
		m_Grid.SetTextMatrix(0,0,"구 분");
	}
	else
	{
		m_Grid.SetColumnCount(nSizeCol);
		m_Grid.SetRowCount(nSizeRow);
		m_Grid.SetFixedRowCount(1);
		m_Grid.SetFixedColumnCount(0);
		m_Grid.SetRowHeightAll(20);
		m_Grid.SetColumnWidthAll(70);
	}

	if(nTab == 0)
	{
		str.Format("Mr(%s)", pData->m_szTonfM);
		m_Grid.SetTextMatrix(0, 1, str);
		str.Format("Mo(%s)", pData->m_szTonfM);
		m_Grid.SetTextMatrix(0, 2, str);
		str.Format("V(%s)", pData->m_szTonf);
		m_Grid.SetTextMatrix(0, 3, str);
		m_Grid.SetTextMatrix(0, 4, "S.F");
		m_Grid.SetTextMatrix(0, 5, "e(m)");
		m_Grid.SetTextMatrix(0, 6, "허용폭(m)");
		m_Grid.SetTextMatrix(0, 7, "비 고");
	}
	else if(nTab == 1)
	{
		str.Format("V(%s)", pData->m_szTonf);
		m_Grid.SetTextMatrix(0, 1, str);
		str.Format("Hu(%s)", pData->m_szTonf);
		m_Grid.SetTextMatrix(0, 2, str);
		str.Format("H(%s)", pData->m_szTonf);
		m_Grid.SetTextMatrix(0, 3, str);
		m_Grid.SetTextMatrix(0, 4, "S.F");
		m_Grid.SetTextMatrix(0, 5, "허용안전율");
		m_Grid.SetTextMatrix(0, 6, "비 고");
	}
	else if(nTab == 2)
	{
		str.Format("V(%s)", pData->m_szTonf);
		m_Grid.SetTextMatrix(0, 1, str);
		str.Format("M(%s)", pData->m_szTonfM);
		m_Grid.SetTextMatrix(0, 2, str);
		m_Grid.SetTextMatrix(0, 3, "e(m)");
		str.Format("Qmax(%s)", pData->m_szTonf);
		m_Grid.SetTextMatrix(0, 4, str);
		str.Format("Qmin(%s)", pData->m_szTonf);
		m_Grid.SetTextMatrix(0, 5, str);
		str.Format("Qa(%s)", pData->m_szTonf);
		m_Grid.SetTextMatrix(0, 6, str);
		m_Grid.SetTextMatrix(0, 7, "비 고");
	}
	else if(nTab == 3)
	{
		m_Grid.SetTextMatrix(0, 0, "β");
		m_Grid.SetTextMatrix(0, 1, "λ(m)");
		m_Grid.SetTextMatrix(0, 2, "βxλ");
		m_Grid.SetTextMatrix(0, 3, "허용값");
		m_Grid.SetTextMatrix(0, 4, "비 고");
	}
	
	if(nTab < 3)
	{
		m_Grid.SetTextMatrix(1, 0, "축력 최대");
		m_Grid.SetTextMatrix(2, 0, "부모멘트 최대");
		m_Grid.SetTextMatrix(3, 0, "정모멘트 최대");
		m_Grid.SetTextMatrix(4, 0, "지진시");
	}
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputCheckGrid::SetDataInit_FootingSafty()
{
	long nTab = m_ctlTab.GetCurSel();

	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pData->IsLsdDesign();
	if (bLsd)
	{
		if(nTab == 0) SetDataInit_FootingSafty_Overtruning_Lsd();
		else if(nTab == 1) SetDataInit_FootingSafty_Sliding_Lsd();
		else if(nTab == 2) SetDataInit_FootingSafty_Support_Lsd();
		else if(nTab == 3) SetDataInit_FootingSafty_RigidBody();
		else if(nTab == 4) SetDataInit_FootingSafty_Subsidence_Lsd();
	}
	else
	{
		if(nTab == 0) SetDataInit_FootingSafty_Overtruning();
		else if(nTab == 1) SetDataInit_FootingSafty_Sliding();
		else if(nTab == 2) SetDataInit_FootingSafty_Support();
		else if(nTab == 3) SetDataInit_FootingSafty_RigidBody();
	}
}

void CInputCheckGrid::SetDataInit_FootingSafty_Overtruning()
{
	
	CString szOK = _T("");

	if(!m_pBri->m_FemNormal.IsOutputLoaded() || (m_pBri->GetTypeDesingEQ() == 1 && !m_pBri->GetFemManage(COMBO_EARTHQUAKE)->IsOutputLoaded())) return;

	for(long nCombo=0; nCombo<4; nCombo++)
	{
		CLoadFooting *pFootingCalc = m_pCalc->m_pFooting->m_pListLoadUse[m_nTypeCheck].GetAt(nCombo);

		long nRow = 0;
		long nCol = 0;
		nCol = 1;
		nRow = nCombo+1;

		// 지진시 해석안함
		if(m_pBri->GetTypeDesingEQ() == 0 && nCombo == 3)
		{
			return;
		}

		szOK = (pFootingCalc->m_bOkOverturning)? "O.K":"N.G";
		double dBa = toM((nCombo==3)? pFootingCalc->m_pFooting->GetWidth()/3:pFootingCalc->m_pFooting->GetWidth()/6);
		m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f", pFootingCalc->m_dMrTotal);
		m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f", pFootingCalc->m_dMoTotal);
		m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f", pFootingCalc->m_dVerTotal);

		// (ARCBRIDGE-3034) Mo가 - 값일수 있으므로 SF는 절대값으로 표기
		m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.2f", fabs(pFootingCalc->m_dMrTotal/pFootingCalc->m_dMoTotal));
		// if(pFootingCalc->m_dMoTotal>0) m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.2f", pFootingCalc->m_dMrTotal/pFootingCalc->m_dMoTotal);
		// else m_Grid.SetTextMatrix(nRow,nCol++,"∞");
		m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f", pFootingCalc->m_dE);
		m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f", dBa);
		m_Grid.SetTextMatrix(nRow,nCol, szOK);
		if(!pFootingCalc->m_bOkOverturning && m_pBri->IsPileBase(m_nTypeCheck)) m_Grid.SetItemFgColour(nRow, nCol, RGB(125, 0, 125));
		else if(!pFootingCalc->m_bOkOverturning) m_Grid.SetItemFgColour(nRow, nCol, RGB(255, 0, 0));
	}
}

void CInputCheckGrid::SetDataInit_FootingSafty_Sliding()
{
	
	CString szOK = _T("");
	
	if(!m_pBri->m_FemNormal.IsOutputLoaded() || (m_pBri->GetTypeDesingEQ() == 1 && !m_pBri->GetFemManage(COMBO_EARTHQUAKE)->IsOutputLoaded())) return;
	
	for(long nCombo=0; nCombo<4; nCombo++)
	{
		long nRow = 0;
		long nCol = 0;

		CLoadFooting *pFootingCalc = m_pCalc->m_pFooting->m_pListLoadUse[m_nTypeCheck].GetAt(nCombo);
		nCol = 1;
		nRow = nCombo+1;

		// 지진시 해석안함
		if(m_pBri->GetTypeDesingEQ() == 0 && nCombo == 3)
		{
			return;
		}
		
		szOK = (pFootingCalc->m_bOkSliding)? "O.K":"N.G";
		double SF_Apply = (nCombo == 3)? m_pBri->m_SafeFactorSliding[1]:m_pBri->m_SafeFactorSliding[0];
		m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f", pFootingCalc->m_dVerTotal);
		m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f", pFootingCalc->m_dHu);
		m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f", pFootingCalc->m_dHorTotal);
		m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.2f", pFootingCalc->m_dHu/fabs(pFootingCalc->m_dHorTotal));
		m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.2f", SF_Apply);
		m_Grid.SetTextMatrix(nRow,nCol, szOK);
		if(!pFootingCalc->m_bOkSliding && m_pBri->IsPileBase(m_nTypeCheck)) m_Grid.SetItemFgColour(nRow, nCol, RGB(125, 0, 125));
		else if(!pFootingCalc->m_bOkSliding) m_Grid.SetItemFgColour(nRow, nCol, RGB(255, 0, 0));
	}
}

void CInputCheckGrid::SetDataInit_FootingSafty_Support()
{
	CString szOK = _T("");
	
	if(!m_pBri->m_FemNormal.IsOutputLoaded() || (m_pBri->GetTypeDesingEQ() == 1 && !m_pBri->GetFemManage(COMBO_EARTHQUAKE)->IsOutputLoaded())) return;
	
	for(long nCombo=0; nCombo<4; nCombo++)
	{
		long nRow = 0;
		long nCol = 0;

		CLoadFooting *pFootingCalc = m_pCalc->m_pFooting->m_pListLoadUse[m_nTypeCheck].GetAt(nCombo);
		nCol = 1;
		nRow = nCombo+1;

		// 지진시 해석안함
		if(m_pBri->GetTypeDesingEQ() == 0 && nCombo == 3)
		{
			return;
		}
		
		szOK = (pFootingCalc->m_bOkSupportForce)? "O.K":"N.G";
		double dQmax = 0;
		if(m_pBri->m_bApplyMassCalcSafe && pFootingCalc->m_pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_MASS)
			dQmax = pFootingCalc->m_dQmax + pFootingCalc->m_dQmaxMass;
		else dQmax = pFootingCalc->m_dQmax;

		m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f", pFootingCalc->m_dVerTotal);
		m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f", pFootingCalc->m_dMrTotal-pFootingCalc->m_dMoTotal);
		m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f", pFootingCalc->m_dE);
		m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.2f", dQmax);
		m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f", pFootingCalc->m_dQmin);
		m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f", pFootingCalc->m_dQa);
		m_Grid.SetTextMatrix(nRow,nCol, szOK);
		if(!pFootingCalc->m_bOkSupportForce && m_pBri->IsPileBase(m_nTypeCheck)) m_Grid.SetItemFgColour(nRow, nCol, RGB(125, 0, 125));
		else if(!pFootingCalc->m_bOkSupportForce) m_Grid.SetItemFgColour(nRow, nCol, RGB(255, 0, 0));
	}
}

void CInputCheckGrid::SetDataInit_FootingSafty_RigidBody()
{
	long nRow = 0;
	long nCol = 0;

	if(!m_pBri->m_FemNormal.IsOutputLoaded() || (m_pBri->GetTypeDesingEQ() == 1 && !m_pBri->GetFemManage(COMBO_EARTHQUAKE)->IsOutputLoaded())) return;
	nCol = 0;

	long nJijum = m_nTypeCheck;
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	CFootingApp *pFooting = m_pBri->GetFootingByJijumNumber(nJijum);
	CBoringData *pBoring  = m_pBri->GetBoringDataJijumBase(nJijum);

	EPartOfBridge ePart = nJijum == 0 ? ePartFootStt : (nJijum == m_pBri->m_nQtyJigan ? ePartFootEnd : ePartInFoot);
	
	BOOL bPileBase = (pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_PILE) ? TRUE : FALSE;
	CDesignPile *pCalcPile = bPileBase ? m_pCalc->GetDesignPile(nJijum) : NULL;

	double alpha = 1.0; // 도로교설계기준,하부구조,5.5.7 (평상시)
	double Eo    = pFooting->m_dBoringEo;
	double L	 = toM(pFooting->GetLength());
	double B	 = toM(pFooting->GetWidth());
	double H     = toM(pFooting->GetHeight());
	double Ec	 = m_pBri->GetValueEc(ePart) * 10;//  pData->m_Ec*10;	// (kgf/cm²->tomf/m2)
	double Bv    = Round(sqrt(L*B),3);
	double kvo	 = 1/30.0 * alpha * Eo;
//	double kv    = kvo * 10 * pow(Bv/30.0,-3./4.); // (kgf/cm²->tomf/m2)
	double kv    = kvo * pow(toCm(frM(Bv))/30.0,-3./4.) * 1000;
	double PileSu= bPileBase ? pFooting->m_exFooting.m_Pile.GetPileSu() : 0;
	double k	 = kv;
	
	if(bPileBase)
	{
		double Kv_pile  = 0;
		Kv_pile = pCalcPile->m_Kv;
		k = Kv_pile*PileSu/(L*B);
	}
	double beta  = pow((3*k)/(Ec*H*H*H),1./4.);
	double FW    = (nJijum==0) ? toM(pFooting->m_dFWL) : toM(pFooting->m_dFWR); // 뒷굽의 길이....
	double l	 = MIN(0,L/2); // (40862) 벽식이므로 0을 적용
	double b	 = MIN(FW,B/2);
	double ramda = MAX(l,b);
	if(nJijum>0 && nJijum<m_pBri->m_nQtyJigan)
	{
		CWallApp *pWall = m_pBri->GetInWall(nJijum-1);
		if(pWall->m_bTopIsColumn)
		{
			l = MIN(toM(pWall->GetDistFromFootingToColumn(TRUE)),L/2); // 수정....要
			if(pWall->m_nCountColumn==1)
			{
				b	  = MIN(FW,B/2);
				ramda = MAX(l,b);
			}
			else
			{
				double a	 = 0;
				double e	 = 0;
				double ramda_= 0;
				a	  = 1.30;
				e	  = FW;
				b	  = toM(pWall->m_dDistColumn[1]/2 - pWall->m_Column.m_dB[iOUTTER][iUPPER]); // 기둥간 순거리의 1/2
				ramda_= MAX(l,b);
				ramda = a*(ramda_*ramda_+e*e) / (ramda_+e);
			}
		}
	}
	BOOL bOK_RigidBody = (ramda*beta<=1.0) ? TRUE : FALSE;
	CString strOK = bOK_RigidBody? _T("O.K") : _T("N.G");
	
	m_Grid.SetTextMatrixFormat(1,nCol++,"%.3f", beta);
	m_Grid.SetTextMatrixFormat(1,nCol++,"%.3f", ramda);
	m_Grid.SetTextMatrixFormat(1,nCol++,"%.3f", ramda*beta);
	m_Grid.SetTextMatrixFormat(1,nCol++,"%.3f", 1.0);
	m_Grid.SetTextMatrix(1,nCol, strOK);
	if(!bOK_RigidBody && m_pBri->IsPileBase(m_nTypeCheck)) m_Grid.SetItemFgColour(nRow+1, nCol, RGB(125, 0, 125));
	else if(!bOK_RigidBody) m_Grid.SetItemFgColour(nRow+1, nCol, RGB(255, 0, 0));

}

void CInputCheckGrid::SetGridTitle_FootingSafty_Lsd()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	const long nJijum = m_nTypeCheck;
	CLsdFootingDesign *pLsdFooting = m_pCalc->m_pFooting->m_pListLsdFootingSafe[nJijum];

	long nTab = m_ctlTab.GetCurSel();
	long nSizeRow = pLsdFooting->GetSizeLoad(eLSD_COMBINATION_LIMIT) + pLsdFooting->GetSizeLoad(eLSD_COMBINATION_EXTREME) + 1;
	if(nTab == 2)
		nSizeRow += pLsdFooting->GetSizeLoad(eLSD_COMBINATION_SERVICE);

	long nSizeCol = 0;
	CString str = _T("");

	switch(nTab)
	{
	case 0:	nSizeCol = 7; break;
	case 1:	nSizeCol = 8; break;
	case 2:	nSizeCol = 8; break;
	case 3:	nSizeCol = 5; nSizeRow = 2;	break;
	case 4: nSizeCol = 7; nSizeRow = pLsdFooting->GetSizeLoad(eLSD_COMBINATION_SERVICE) + 1; break;
	}
	
	m_Grid.SetColumnCount(nSizeCol);
	m_Grid.SetRowCount(nSizeRow);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(90);
	m_Grid.SetColumnWidth(0,65);
	m_Grid.SetColumnWidth(nSizeCol - 1,50);

	m_Grid.SetTextMatrix(0,0,"구 분");

	if(nTab == 0)
	{
		str.Format("Mr(%s)", pData->m_szTonfM);
		m_Grid.SetTextMatrix(0, 1, str);
		str.Format("Mo(%s)", pData->m_szTonfM);
		m_Grid.SetTextMatrix(0, 2, str);
		str.Format("V(%s)", pData->m_szTonf);
		m_Grid.SetTextMatrix(0, 3, str);
		str.Format("e(%s)", pData->m_szM);
		m_Grid.SetTextMatrix(0, 4, str);
		str.Format("허용폭(%s)", pData->m_szM);
		m_Grid.SetTextMatrix(0, 5, str);
		m_Grid.SetTextMatrix(0, 6, "비 고");
	}
	else if(nTab == 1)
	{
		str.Format("V(%s)", pData->m_szTonf);
		m_Grid.SetTextMatrix(0, 1, str);
		str.Format("Qτ(%s)", pData->m_szTonf);
		m_Grid.SetTextMatrix(0, 2, str);
		m_Grid.SetTextMatrix(0, 3, "Φτ");
		m_Grid.SetTextMatrix(0, 4, "QR");
		str.Format("H(%s)", pData->m_szTonf);
		m_Grid.SetTextMatrix(0, 5, str);
		m_Grid.SetTextMatrix(0, 6, "S.F");
		m_Grid.SetTextMatrix(0, 7, "비 고");
	}
	else if(nTab == 2)
	{
		str.Format("V(%s)", pData->m_szTonf);
		m_Grid.SetTextMatrix(0, 1, str);
		str.Format("M(%s)", pData->m_szTonfM);
		m_Grid.SetTextMatrix(0, 2, str);
		str.Format("e(%s)", pData->m_szM);
		m_Grid.SetTextMatrix(0, 3, str);
		str.Format("Qmax(%s)", pData->m_szTonf_M2);
		m_Grid.SetTextMatrix(0, 4, str);
		str.Format("Qmin(%s)", pData->m_szTonf_M2);
		m_Grid.SetTextMatrix(0, 5, str);
		str.Format("Qa(%s)", pData->m_szTonf_M2);
		m_Grid.SetTextMatrix(0, 6, str);
		m_Grid.SetTextMatrix(0, 7, "비 고");
	}
	else if(nTab == 3)
	{
		m_Grid.SetTextMatrix(0, 0, "β");
		m_Grid.SetTextMatrix(0, 1, "λ(m)");
		m_Grid.SetTextMatrix(0, 2, "βxλ");
		m_Grid.SetTextMatrix(0, 3, "허용값");
		m_Grid.SetTextMatrix(0, 4, "비 고");
	}
	else if(nTab == 4)
	{
		str.Format("V(%s)", pData->m_szTonf);
		m_Grid.SetTextMatrix(0, 1, str);
		str.Format("M(%s)", pData->m_szTonfM);
		m_Grid.SetTextMatrix(0, 2, str);
		str.Format("Qmax(%s)", pData->m_szTonf);
		m_Grid.SetTextMatrix(0, 3, str);
		str.Format("Se(%s)", pData->m_szM);
		m_Grid.SetTextMatrix(0, 4, str);
		str.Format("Sa(%s)", pData->m_szM);
		m_Grid.SetTextMatrix(0, 5, str);
		m_Grid.SetTextMatrix(0, 6, "비 고");
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputCheckGrid::SetDataInit_FootingSafty_Overtruning_Lsd()
{
	const long nJijum = m_nTypeCheck;
	CLsdFootingDesign *pLsdFooting = m_pCalc->m_pFooting->m_pListLsdFootingSafe[nJijum];
	const HGBOOL bLsd = TRUE;

	CString str;
	const HGINT32 icCombo = pLsdFooting->GetSizeLoad();
	for (HGINT32 ixCombo = 0; ixCombo < icCombo; ++ixCombo)
	{
		sLsdFootingLoad *pLsdLoad = pLsdFooting->GetLoad(ixCombo);
		if(pLsdLoad == NULL || pLsdLoad->eStatus == eLSD_COMBINATION_SERVICE)
			continue;

		const HGINT32 iRow = ixCombo + 1;
		m_Grid.SetTextMatrix(iRow, 0, pLsdLoad->sTitle);

		str.Format(_T("%s"), GetStrDouble(pLsdLoad->Mr, 0, bLsd));
		m_Grid.SetTextMatrix(iRow, 1, str);

		str.Format(_T("%s"), GetStrDouble(pLsdLoad->Mo, 0, bLsd));
		m_Grid.SetTextMatrix(iRow, 2, str);
		
		str.Format(_T("%s"), GetStrDouble(pLsdLoad->V, 0, bLsd));
		m_Grid.SetTextMatrix(iRow, 3, str);

		str.Format(_T("%s"), GetStrDouble(pLsdLoad->eLoad, 0, bLsd));
		m_Grid.SetTextMatrix(iRow, 4, str);

		str.Format(_T("%s"), GetStrDouble(pLsdLoad->sSafeResult.eAllow, 0, bLsd));
		m_Grid.SetTextMatrix(iRow, 5, str);

		str = pLsdLoad->IsOkOverturning() ? _T("O.K") : _T("N.G");
		m_Grid.SetTextMatrix(iRow, 6, str);
	}
}

void CInputCheckGrid::SetDataInit_FootingSafty_Sliding_Lsd()
{
	const long nJijum = m_nTypeCheck;
	CLsdFootingDesign *pLsdFooting = m_pCalc->m_pFooting->m_pListLsdFootingSafe[nJijum];
	const HGBOOL bLsd = TRUE;

	if(pLsdFooting->IsPileBase())
	{
		m_Grid.SetGridDefault(1, 1, 1, 1);
		m_Grid.SetColumnCount(1);
		m_Grid.SetRowCount(1);
		m_Grid.SetColumnWidth(0, 250);
		m_Grid.SetTextMatrix(0, 0, _T("말뚝이므로 검토 제외"));
		return;
	}
	if(pLsdFooting->IsRock())
	{
		HGBOOL bSkipCheck = TRUE;
		for(HGINT32 ix = 0; ix < eLSD_COMBINATION_COUNT - 1; ix++) // 피로는 제외
		{
			if(pLsdFooting->sCoffSF.slidingResitCoff[LSD_GROUND_TYPE_ROCK][ix] > 0)
			{
				bSkipCheck = FALSE;
				break;
			}
		}

		if(bSkipCheck)
		{
			m_Grid.SetGridDefault(1, 1, 1, 1);
			m_Grid.SetColumnCount(1);
			m_Grid.SetRowCount(1);
			m_Grid.SetColumnWidth(0, 250);
			m_Grid.SetTextMatrix(0, 0, _T("암반에 대한 식 없으므로 검토 제외"));
			return;
		}
	}
	
	CString str;
	const HGINT32 icCombo = pLsdFooting->GetSizeLoad();
	for (HGINT32 ixCombo = 0; ixCombo < icCombo; ++ixCombo)
	{
		sLsdFootingLoad *pLsdLoad = pLsdFooting->GetLoad(ixCombo);
		if(pLsdLoad == NULL || pLsdLoad->eStatus == eLSD_COMBINATION_SERVICE)
			continue;

		const HGINT32 iRow = ixCombo + 1;
		m_Grid.SetTextMatrix(iRow, 0, pLsdLoad->sTitle);

		str.Format(_T("%s"), GetStrDouble(pLsdLoad->V, 0, bLsd));
		m_Grid.SetTextMatrix(iRow, 1, str);

		str.Format(_T("%s"), GetStrDouble(pLsdLoad->sSafeResult.Q_Tau, 0, bLsd));
		m_Grid.SetTextMatrix(iRow, 2, str);

		str.Format(_T("%s"), GetStrDouble(pLsdLoad->sSafeResult.pi_Tau, 2, bLsd));
		m_Grid.SetTextMatrix(iRow, 3, str);

		str.Format(_T("%s"), GetStrDouble(pLsdLoad->sSafeResult.QR, 0, bLsd));
		m_Grid.SetTextMatrix(iRow, 4, str);

		str.Format(_T("%s"), GetStrDouble(pLsdLoad->H, 0, bLsd));
		m_Grid.SetTextMatrix(iRow, 5, str);

		if(Compare(pLsdLoad->H, 0., _T("=")))
			str = _T("∞");
		else
		{
			str.Format(_T("%s"), GetStrDouble(fabs(pLsdLoad->sSafeResult.QR / pLsdLoad->H), 3,  bLsd));
		}
		m_Grid.SetTextMatrix(iRow, 6, str);

		str = pLsdLoad->IsOkSliding() ? _T("O.K") : _T("N.G");
		m_Grid.SetTextMatrix(iRow, 7, str);
	}
}

void CInputCheckGrid::SetDataInit_FootingSafty_Support_Lsd()
{
	const long nJijum = m_nTypeCheck;
	CLsdFootingDesign *pLsdFooting = m_pCalc->m_pFooting->m_pListLsdFootingSafe[nJijum];
	const HGBOOL bLsd = TRUE;

	HGDOUBLE dQmaxMass = (pLsdFooting->m_pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_MASS) ? pLsdFooting->sMassConc.Mass_Qmax : 0;

	CString str;
	const HGINT32 icCombo = pLsdFooting->GetSizeLoad();
	for (HGINT32 ixCombo = 0; ixCombo < icCombo; ++ixCombo)
	{
		sLsdFootingLoad *pLsdLoad = pLsdFooting->GetLoad(ixCombo);
		if(pLsdLoad == NULL)
			continue;

		const HGBOOL bRock = pLsdFooting->IsRock();

		const HGINT32 iRow = ixCombo + 1;
		m_Grid.SetTextMatrix(iRow, 0, pLsdLoad->sTitle);

		str.Format(_T("%s"), GetStrDouble(pLsdLoad->V, 0, bLsd));
		m_Grid.SetTextMatrix(iRow, 1, str);

		str.Format(_T("%s"), GetStrDouble(pLsdLoad->Mr - pLsdLoad->Mo, 0, bLsd));
		m_Grid.SetTextMatrix(iRow, 2, str);

		str.Format(_T("%s"), GetStrDouble(pLsdLoad->eLoad, 0, bLsd));
		m_Grid.SetTextMatrix(iRow, 3, str);

		str.Format(_T("%s"), GetStrDouble(pLsdLoad->sSafeResult.Qmax + dQmaxMass, 3, bLsd));
		m_Grid.SetTextMatrix(iRow, 4, str);

		str.Format(_T("%s"), GetStrDouble(pLsdLoad->sSafeResult.Qmin + dQmaxMass, 3, bLsd));
		m_Grid.SetTextMatrix(iRow, 5, str);

		str.Format(_T("%s"), GetStrDouble(pLsdLoad->sSafeResult.GetQa(bRock), 3, bLsd));
		m_Grid.SetTextMatrix(iRow, 6, str);

		str =  pLsdLoad->IsOkSupport(bRock) ? _T("O.K") : _T("N.G");
		m_Grid.SetTextMatrix(iRow, 7, str);
	}
}

void CInputCheckGrid::SetDataInit_FootingSafty_Subsidence_Lsd()
{
	const long nJijum = m_nTypeCheck;
	CLsdFootingDesign *pLsdFooting = m_pCalc->m_pFooting->m_pListLsdFootingSafe[nJijum];
	const HGBOOL bLsd = TRUE;

	CString str;
	const HGINT32 icCombo = pLsdFooting->GetSizeLoad();
	HGINT32 iRow = 0;
	for (HGINT32 ixCombo = 0; ixCombo < icCombo; ++ixCombo)
	{
		sLsdFootingLoad *pLsdLoad = pLsdFooting->GetLoad(ixCombo);
		if(pLsdLoad == NULL || pLsdLoad->eStatus != eLSD_COMBINATION_SERVICE)
			continue;

		++iRow;
		m_Grid.SetTextMatrix(iRow, 0, pLsdLoad->sTitle);

		str.Format(_T("%s"), GetStrDouble(pLsdLoad->V, 0, bLsd));
		m_Grid.SetTextMatrix(iRow, 1, str);

		str.Format(_T("%s"), GetStrDouble(pLsdLoad->Mr - pLsdLoad->Mo, 0, bLsd));
		m_Grid.SetTextMatrix(iRow, 2, str);

		str.Format(_T("%s"), GetStrDouble(MAX(pLsdLoad->sSafeResult.Qmax, pLsdLoad->sSafeResult.Qmin), 3, bLsd));
		m_Grid.SetTextMatrix(iRow, 3, str);

		str.Format(_T("%s"), GetStrDouble(pLsdLoad->sSafeResult.Se, 3, bLsd));
		m_Grid.SetTextMatrix(iRow, 4, str);

		str.Format(_T("%s"), GetStrDouble(pLsdLoad->sSafeResult.Sa, 3, bLsd));
		m_Grid.SetTextMatrix(iRow, 5, str);

		str =  pLsdLoad->IsOkSubsidence() ? _T("O.K") : _T("N.G");
		m_Grid.SetTextMatrix(iRow, 6, str);
	}
}

void CInputCheckGrid::SetGridTitle_FootingPile()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pData->IsLsdDesign();
	if (bLsd && m_pBri->IsBoredPileASD(m_nTypeCheck) == FALSE && m_pBri->IsPhcPileASD(m_nTypeCheck) == FALSE)
	{
		SetGridTitle_FootingPile_Lsd();
		return;
	}

	long nTab = m_ctlTab.GetCurSel();

	long nIndex = 0;
	long n=0; 
	for(n=0; n<m_nTypeCheck; n++)
	{
		if(m_pBri->IsPileBase(n)) nIndex++;
	}
	CDesignPile *pCalcPile = m_pCalc->m_pArrPileCalc.GetAt(nIndex);

	long nSizeUse = pCalcPile->GetSizePileLoad(PILE_CASE_USE, TRUE);
	long nSizeExt = bLsd ? pCalcPile->GetSizePileLoad(PILE_CASE_EXTREME) : 0;
	long nSizeRow = 1 + (nSizeUse + nSizeExt) * 2; // 타이틀 1, 하중조합 4 x 2
	long nSizeCol = 0;
	CString str = _T("");

	BOOL bPHC = pCalcPile->m_pPile->IsPHC();

	long nSizePile = 0, nSizeCase = 0;
	if(m_pBri->IsBoxType())
	{
		long nJijum = 0;
		CFootingApp *pFooting = m_pBri->GetFooting(nJijum);
		if(pFooting == NULL) return;
		CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
		if(pPile == NULL) return;

		nSizePile = pPile->m_nCountOrg;
		// nSizeCase = m_pBri->IsApplyEarthQuakeDesign() ? (m_pBri->m_nSelectModelingCountEQ==2 ? 3 : 2) : 1; // 상시,붕괴,기능
		nSizeCase = m_pBri->IsApplyEarthQuakeDesign() ? 2 : 1;		// 말뚝 검토를 붕괴/기능 으로 각각 검토하는 경우가 없는데.. 타이틀만 따로 찍고 있다.
		nSizeRow = 1 + nSizePile * nSizeCase;
	}
	
	switch(nTab)
	{
		case 0:	nSizeCol = 7; break;
		case 1:	nSizeCol = bPHC ? 9 : 7; break;
		case 2:	nSizeCol = 6; break;
		case 3:	nSizeCol = 5; break;
	}
	if(bLsd)
	{
		nSizeCol++;
	}
	
	m_Grid.SetColumnCount(nSizeCol);
	m_Grid.SetRowCount(nSizeRow);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(bLsd ? 3 : 2);
	m_Grid.SetRowHeightAll(20);
	if(nTab == 3) 
		m_Grid.SetColumnWidthAll(90);
	else 
		m_Grid.SetColumnWidthAll(75);
	if(m_pBri->IsBoxType())
	{
		m_Grid.SetColumnWidth(0,100);
		if(bLsd)
		{
			m_Grid.SetColumnWidth(2,40);
		}
		else
		{
			m_Grid.SetColumnWidth(1,30);
		}
	}
	else
	{
		m_Grid.SetColumnWidth(0,100);
		if(bLsd)
		{
			m_Grid.SetColumnWidth(1,100);
		}
	}
	m_Grid.SetMergeCol(0, 0, bLsd ? 2 : 1);
	
	m_Grid.SetTextMatrix(0,0,"구 분");
	
	if(nTab == 0)
	{
		long col = bLsd ? 3 : 2;
		str.Format("Vmax(%s)", pData->m_szTonf);
		m_Grid.SetTextMatrix(0, col++, str);
		str.Format("Vmin(%s)", pData->m_szTonf);
		m_Grid.SetTextMatrix(0, col++, str);
		str.Format("Ra(%s)", pData->m_szTonf);
		m_Grid.SetTextMatrix(0, col++, str);
		str.Format("Pa(%s)", pData->m_szTonf);
		m_Grid.SetTextMatrix(0, col++, str);
		m_Grid.SetTextMatrix(0, col++, "비 고");
	}
	else if(nTab == 1)
	{
		long col = bLsd ? 3 : 2;;
		str.Format("V(%s)", pData->m_szTonf);
		m_Grid.SetTextMatrix(0, col++, str);
		str.Format("M(%s)", pData->m_szTonfM);
		m_Grid.SetTextMatrix(0, col++, str);
		if(bPHC)
		{
			str.Format("fc");
			m_Grid.SetTextMatrix(0, col++, str);
			str.Format("fca");
			m_Grid.SetTextMatrix(0, col++, str);
			str.Format("ft");
			m_Grid.SetTextMatrix(0, col++, str);
			str.Format("fta");
			m_Grid.SetTextMatrix(0, col++, str);
		}
		else
		{
			str.Format("fmax");
			m_Grid.SetTextMatrix(0, col++, str);
			str.Format("fsa");
			m_Grid.SetTextMatrix(0, col++, str);
		}
		m_Grid.SetTextMatrix(0, col++, "비 고");
	}
	else if(nTab == 2)
	{
		long col = bLsd ? 3 : 2;
		str.Format("H(%s)", pData->m_szTonf);
		m_Grid.SetTextMatrix(0, col++, str);
		str.Format("τ");
		m_Grid.SetTextMatrix(0, col++, str);
		str.Format("τsa");
		m_Grid.SetTextMatrix(0, col++, str);
		m_Grid.SetTextMatrix(0, col++, "비 고");
	}
	else if(nTab == 3)
	{
		long col = bLsd ? 3 : 2;
		m_Grid.SetTextMatrix(0, col++, "수평변위(mm)");
		m_Grid.SetTextMatrix(0, col++, "허용변위(mm)");
		m_Grid.SetTextMatrix(0, col++, "비 고");
	}

	if(m_pBri->IsBoxType() == FALSE)
	{
		long nRow = 1;
		long nCol = bLsd ? 1 : 0;
		for(long ix = 0; ix < nSizeUse; ix++)
		{
			CPileLoad *pLoad = pCalcPile->GetPileLoad(PILE_CASE_USE, ix);
			if(pLoad == NULL) continue;
			m_Grid.SetTextMatrix(nRow, nCol, pLoad->m_sName);
			m_Grid.SetMergeRow(nRow, nRow+1, nCol);
			m_Grid.SetTextMatrix(nRow, nCol+1, "FIX");
			m_Grid.SetTextMatrix(nRow+1, nCol+1, "HINGE");
			nRow += 2;
		}
		for(long ix = 0; ix < nSizeExt; ix++)
		{
			CPileLoad *pLoad = pCalcPile->GetPileLoad(PILE_CASE_EXTREME, ix);
			if(pLoad == NULL) continue;
			m_Grid.SetTextMatrix(nRow, nCol, pLoad->m_sName);
			m_Grid.SetMergeRow(nRow, nRow+1, nCol);
			m_Grid.SetTextMatrix(nRow, nCol+1, "FIX");
			m_Grid.SetTextMatrix(nRow+1, nCol+1, "HINGE");
			nRow += 2;
		}
		if(bLsd)
		{
			m_Grid.SetTextMatrix(1, 0, "사용한계상태");
			m_Grid.SetMergeRow(1, nSizeUse*2, 0);
			m_Grid.SetTextMatrix(nSizeUse*2+1, 0, "극단상황한계");
			m_Grid.SetMergeRow(nSizeUse*2+1, nSizeUse*2 + nSizeExt*2, 0);
		}
	}
	else
	{
		long nRow = 1;
		CString str = bLsd? _T("사용한계") : _T("상시");
		for(long ix = 0; ix < nSizeCase; ix++)
		{
			if(ix > 0)
			{
				str = bLsd? _T("극단상황한계") : _T("지진시");
				if(nSizeCase == 3)
				{
					str = ix == 1 ? _T("붕괴방지") : _T("기능수행");
				}
			}
			m_Grid.SetTextMatrix(nRow, 0, str);
			m_Grid.SetMergeRow(nRow, nRow + nSizePile - 1, 0);
			for(long kk = 0; kk < nSizePile; kk++)
			{
				if(bLsd)
				{
					//CPileLoad *pLoad = pCalcPile->GetPileLoad((ix==0)? PILE_CASE_USE : PILE_CASE_EXTREME, kk);
					//m_Grid.SetTextMatrix(nRow, 1, pLoad->m_sName);
					str.Format(_T("COMBO %d"), kk+1);
					m_Grid.SetTextMatrix(nRow, 1, str);
					m_Grid.SetTextMatrix(nRow, 2, _T("FIX"));
				}
				else
				{
					m_Grid.SetTextMatrix(nRow, 1, COMMA(kk+1));
				}
				nRow++;
			}
		}
	}
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputCheckGrid::SetDataInit_FootingPile()
{
	long nTab = m_ctlTab.GetCurSel();
	HGBOOL bLSD = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();
	HGBOOL bPileASD = (m_pBri->IsBoredPileASD(m_nTypeCheck) || m_pBri->IsPhcPileASD(m_nTypeCheck))? TRUE : FALSE;
	CFootingApp *pFooting = m_pBri->GetFootingByJijumNumber(m_nTypeCheck);
	if(pFooting == NULL) return;

	if(pFooting->m_exFooting.m_Pile.m_nType != PILE_TYPE_HYUNJANG)
	{
		if(nTab == m_nTab_Pile_Disp)
		{
			SetDataInit_FootingPile_Disp();
		}
		else if(nTab == 0)
		{
			if( bLSD && bPileASD == FALSE)
				SetDataInit_FootingPile_Support_Lsd();
			else
				SetDataInit_FootingPile_Support();
		}
		else if(nTab == 1)
		{
			if( bLSD && bPileASD == FALSE )
				SetDataInit_FootingPile_AxialForce_Lsd();
			else
				SetDataInit_FootingPile_Bending();
		}
		else if(nTab == 2)
		{
			if( bLSD && bPileASD == FALSE )
				SetDataInit_FootingPile_Fa_Lsd();
			else
				SetDataInit_FootingPile_Shear();
		}
		else if(nTab == 3)
		{
			if( bLSD && bPileASD == FALSE )
				SetDataInit_FootingPile_ShaerStress_Lsd();
		}
	}
	else
	{
		if(nTab == 0)
		{
			if( bLSD && bPileASD == FALSE)
				SetDataInit_FootingPile_Support_Lsd();
			else
				SetDataInit_FootingPile_Support();
		}
		else if(nTab == 1)
		{
			SetDataInit_FootingPile_Disp();
		}
		else if(nTab == 2)
		{
			SetDataInit_FootingPile_Hyunta();
		}
	}
	
}

void CInputCheckGrid::SetDataInit_FootingPile_Support()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;

	long nRow = 0;
	long nCol = 0;
	CString szOK = _T("");
	
	if(!m_pBri->m_FemNormal.IsOutputLoaded() || (m_pBri->GetTypeDesingEQ() == 1 && !m_pBri->GetFemManage(COMBO_EARTHQUAKE)->IsOutputLoaded())) return;
	
	long nIndex = 0;
	long n=0; for(n=0; n<m_nTypeCheck; n++)
	{
		if(m_pBri->IsBoxType()) break;
		if(m_pBri->IsPileBase(n)) nIndex++;
	}
	CDesignPile *pCalcPile = m_pCalc->m_pArrPileCalc.GetAt(nIndex);

	HGBOOL bBoxType = m_pBri->IsBoxType();
	HGBOOL bLSD = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();
	long nDec03 = bLSD ? 0 :3;	

	long nSizeUse = pCalcPile->GetSizePileLoad(PILE_CASE_USE);
	long nSizeExt = bLSD ? pCalcPile->GetSizePileLoad(PILE_CASE_EXTREME) : 0;
	long nCntRow = 1 + (nSizeUse + nSizeExt) * 2;

	long nSizePile = 0;
	if(bBoxType)
	{
		long nJijum = 0;
		CFootingApp *pFooting = m_pBri->GetFooting(nJijum);
		if(pFooting == NULL) return;
		CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
		if(pPile == NULL) return;

		nSizePile = pPile->m_nCountOrg;
		long nSizeCase = m_pBri->IsApplyEarthQuakeDesign() ? (m_pBri->m_nSelectModelingCountEQ==2 ? 3 : 2) : 1; // 상시,붕괴,기능
		nCntRow = 1 + nSizePile * nSizeCase;
	}

	long nIdx = 0;
	long nCountFixHinge = bBoxType ? 1 : 2;
	for(long nTypePileCase = PILE_CASE_USE; nTypePileCase < PILE_CASE_COUNT; nTypePileCase++)
	{
		// 강도설계법일때는 사용하중에 상시, 지진시가 다 들어있다.
		if(bLSD == FALSE && nTypePileCase > PILE_CASE_USE) break;
		if(nTypePileCase == PILE_CASE_ULTIMATE) continue;

		long nSizeCombo = pCalcPile->GetSizePileLoad(nTypePileCase, TRUE);
		for(long nCombo = 0; nCombo < nSizeCombo; nCombo++)
		{
			CPileLoad *pLoad = pCalcPile->GetPileLoad(nTypePileCase, nCombo, TRUE);
			if(pLoad == NULL) continue;

			nRow = (nIdx * nCountFixHinge) + 1;
			long nIdxPile = (nRow - 1) % pCalcPile->m_pPile->m_nCountOrg;

			long nTypeCheck = bLSD ? nTypePileCase : (pLoad->IsEarthQuake() ? PILE_CASE_EARTH : PILE_CASE_NORMAL);
			if(bLSD == FALSE && m_pBri->IsBoxType() && nRow > nSizePile)
			{
				nTypeCheck = PILE_CASE_EARTH;
			}
			for(long j = 0; j < nCountFixHinge; j++)
			{
				nCol = bLSD ? 3 : 2;

				BOOL bFix = j == 0 ? TRUE : FALSE;
				HGDOUBLE dVmax(0), dVmin(0);
				if(bBoxType)
				{
					double dDistPile = toM(m_pCalc->m_pFooting->GetDistMaxPile(nIdxPile));
					//CPileLoad *pLoad = pCalcPile->GetPileLoad(nTypePileCase, nCombo);
					dVmax = GetValueUnitChange(pLoad->m_dForceVert * dDistPile, UNIT_CGS_TONF, pData->m_szTonf);
					dVmin = 0;
				}
				else
				{
					dVmax = GetValueUnitChange(pCalcPile->GetValueMaxMinPileHead(nCombo, PILE_FORCE_AXIAL, -1, bFix, TRUE, nTypePileCase, TRUE), UNIT_CGS_TONF, pData->m_szTonf);
					dVmin = GetValueUnitChange(pCalcPile->GetValueMaxMinPileHead(nCombo, PILE_FORCE_AXIAL, -1, bFix, FALSE, nTypePileCase, TRUE), UNIT_CGS_TONF, pData->m_szTonf);
				}
				double Vmax = GetValueUnitChange(pCalcPile->GetValueMaxMinPileHead(nCombo,PILE_FORCE_AXIAL,-1, bFix,TRUE,nTypePileCase,TRUE), UNIT_CGS_TONF, pData->m_szTonf);
				double Vmin = GetValueUnitChange(pCalcPile->GetValueMaxMinPileHead(nCombo,PILE_FORCE_AXIAL,-1, bFix,FALSE,nTypePileCase,TRUE), UNIT_CGS_TONF, pData->m_szTonf);
				double Ra	= bLSD ? pCalcPile->m_Ra[nTypePileCase] : GetValueUnitChange(pCalcPile->m_Ra[nTypeCheck], UNIT_CGS_TONF, pData->m_szTonf);
				double Pa	= bLSD ? pCalcPile->m_Pa[nTypePileCase] : GetValueUnitChange(pCalcPile->m_Pa[nTypeCheck], UNIT_CGS_TONF, pData->m_szTonf);
				if(m_pBri->IsBoxType())
				{
					double dDistPile = m_pCalc->m_pFooting->GetDistMaxPile(nIdxPile);
					if(!bLSD)
						dDistPile = toM(dDistPile);

					//CPileLoad *pLoad = pCalcPile->m_pListUse.GetAt(nIdxPile);
					Vmax = GetValueUnitChange(pLoad->m_dForceVert, UNIT_CGS_TONF, pData->m_szTonf) * dDistPile;
					Vmin = 0;
				}

				BOOL bOK = (Vmax < Ra && (fabs(Vmin) < Pa || Vmin >= 0)) ? TRUE : FALSE;
				szOK = (bOK)? "O.K":"N.G";

				m_Grid.SetTextMatrix(nRow+j, nCol++, GetStrDouble(Vmax, nDec03, bLSD));
				m_Grid.SetTextMatrix(nRow+j, nCol++, GetStrDouble(Vmin, nDec03, bLSD));
				m_Grid.SetTextMatrix(nRow+j, nCol++, GetStrDouble(Ra, nDec03, bLSD));
				m_Grid.SetTextMatrix(nRow+j, nCol++, GetStrDouble(Pa, nDec03, bLSD));
				m_Grid.SetTextMatrix(nRow+j, nCol, szOK);
				if(!bOK) m_Grid.SetItemFgColour(nRow+j, nCol, RGB(255, 0, 0));
				else m_Grid.SetItemFgColour(nRow+j, nCol, RGB(0, 0, 0));
			}
			nIdx++;
		}
	}
}

void CInputCheckGrid::SetDataInit_FootingPile_Bending()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	
	long nRow = 0;
	long nCol = 0;
	CString szOK = _T("");
	
	if(!m_pBri->m_FemNormal.IsOutputLoaded() || (m_pBri->GetTypeDesingEQ() == 1 && !m_pBri->GetFemManage(COMBO_EARTHQUAKE)->IsOutputLoaded())) return;
	
	long nIndex = 0;
	long n=0; for(n=0; n<m_nTypeCheck; n++)
	{
		if(m_pBri->IsBoxType()) break;
		if(m_pBri->IsPileBase(n)) nIndex++;
	}
	CDesignPile *pCalcPile = m_pCalc->m_pArrPileCalc.GetAt(nIndex);

	HGBOOL bLSD = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();
	long nDec03 = bLSD ? 0 :3;	
	long nDec33 = 3;

	long nSizeUse = pCalcPile->GetSizePileLoad(PILE_CASE_USE);
	long nSizeExt = bLSD ? pCalcPile->GetSizePileLoad(PILE_CASE_EXTREME) : 0;
	long nCntRow = 1 + (nSizeUse + nSizeExt) * 2;

	long nSizePile = 0;
	if(m_pBri->IsBoxType())
	{
		long nJijum = 0;
		CFootingApp *pFooting = m_pBri->GetFooting(nJijum);
		if(pFooting == NULL) return;
		CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
		if(pPile == NULL) return;

		nSizePile = pPile->m_nCountOrg;
		long nSizeCase = m_pBri->IsApplyEarthQuakeDesign() ? (m_pBri->m_nSelectModelingCountEQ==2 ? 3 : 2) : 1; // 상시,붕괴,기능
		nCntRow = 1 + nSizePile * nSizeCase;
	}

	long nIdx = 0;
	long nCountFixHinge = m_pBri->IsBoxType() ? 1 : 2;
	for(long nTypePileCase = PILE_CASE_USE; nTypePileCase < PILE_CASE_COUNT; nTypePileCase++)
	{
		if(bLSD == FALSE && nTypePileCase > PILE_CASE_USE) break;
		if(nTypePileCase == PILE_CASE_ULTIMATE) continue;

		long nSizeCombo = pCalcPile->GetSizePileLoad(nTypePileCase, TRUE);
		for(long nCombo = 0; nCombo < nSizeCombo; nCombo++)
		{
			CPileLoad *pLoad = pCalcPile->GetPileLoad(nTypePileCase, nCombo, TRUE);
			if(pLoad == NULL) continue;

			BOOL bEarthquake = bLSD? (nTypePileCase == PILE_CASE_EXTREME) : pLoad->IsEarthQuake();
			long nTypeCheck = bLSD ? nTypePileCase : (bEarthquake ? PILE_CASE_EARTH : PILE_CASE_NORMAL);
			if(bLSD == FALSE && m_pBri->IsBoxType() && nRow > nSizePile)
			{
				nTypeCheck = PILE_CASE_EARTH;
			}
			nRow = (nIdx * nCountFixHinge) + 1;
			for(long j = 0; j < nCountFixHinge; j++)
			{
				nCol = bLSD ? 3 : 2;

				BOOL bFix = j == 0 ? TRUE : FALSE;

				long nIdxPile(-1);
				double yIp  = toM(pCalcPile->m_pPile->m_dDia-pCalcPile->m_pPile->m_dT2*2)/2.0/toM4(frCm4(pCalcPile->m_I33));
				double Ap   = toM2(frCm2(pCalcPile->m_Ap));	
				double Mo = 0, Vo = 0, Vmax = 0, Vmin = 0, DepthLm = 0;
				if(m_pBri->IsBoxType())
				{
					long nIdxPile = (nRow - 1) % pCalcPile->m_pPile->m_nCountOrg;
					double dDistPile = m_pCalc->m_pFooting->GetDistMaxPile(nIdxPile);
					if(!bLSD)
						dDistPile = toM(dDistPile);

					Mo = pLoad->m_dMoment * dDistPile;
					Vmax = Vmin = Vo = pLoad->m_dForceVert * dDistPile;
				}
				else
				{
					Mo   = fabs(pCalcPile->GetValueMaxMinPileHead(nCombo,PILE_FORCE_MOMENT,nIdxPile, bFix,TRUE,nTypePileCase,TRUE,TRUE));
					Vmax   = fabs(pCalcPile->GetValueMaxMinPileHead(nCombo,PILE_FORCE_AXIAL,nIdxPile, bFix,TRUE,nTypePileCase,TRUE));
					Vmin   = fabs(pCalcPile->GetValueMaxMinPileHead(nCombo,PILE_FORCE_AXIAL,nIdxPile, bFix,FALSE,nTypePileCase,TRUE));
					Vo = Vmax;
					DepthLm  = pCalcPile->GetPileDepthMaxMoment(nCombo, nIdxPile, bFix, nTypePileCase);
			
					double MomentLm = fabs(pCalcPile->GetPileMomentByDepth(nCombo, nIdxPile, DepthLm, bFix, nTypePileCase));
			
					Mo = max(MomentLm,Mo);
				}

				if(pCalcPile->m_pPile->IsPHC())
				{
					Vo = pCalcPile->m_pPile->m_bApplyMinAxialAtStressCheckPHC ? Vmin : Vmax;
					double Ae  = pCalcPile->m_pPile->m_dAe_PHC;
					double Ze  = pCalcPile->m_pPile->m_dZe_PHC;
					double fce = 0;
					if(m_pBri->IsBoxType())
					{
						fce = GetValueUnitChange(pCalcPile->m_pPile->m_dFce2_PHC, UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);
					}
					else
					{
						double dDepthMmax = (fabs(pCalcPile->GetPileMomentByDepth(nCombo, nIdxPile, 0, bFix, nTypePileCase)) > fabs(pCalcPile->GetPileMomentByDepth(nCombo, nIdxPile, DepthLm, bFix, nTypePileCase)))? 0 : DepthLm;
						fce = (pCalcPile->m_pPile->IsPHC() && dDepthMmax < pCalcPile->m_pPile->m_dLenHead_PHC) ? pCalcPile->m_pPile->m_dFce2_PHC : pCalcPile->m_pPile->m_dFce1_PHC;
					}

					double fc   = fce + frTon(Mo)*100/Ze + frTon(Vmax)/Ae;
					double ft   = fce - frTon(Mo)*100/Ze + frTon(Vo)/Ae;
					double fca  = pCalcPile->m_dArrFca[nTypePileCase].GetAt(nCombo); //  pCalcPile->m_PHC_Fca[bEarthquake ? 1 : 0];
					double fta  = pCalcPile->m_dArrFsa[nTypePileCase].GetAt(nCombo); // pCalcPile->m_PHC_Fta[bEarthquake ? 1 : 0];

					BOOL bOK = (ft >= fta && fc <= fca)? TRUE:FALSE;
					szOK = (bOK)? "O.K":"N.G";
			
					m_Grid.SetTextMatrix(nRow+j, nCol++, GetStrDouble(GetValueUnitChange(Vmax, UNIT_CGS_TONF, pData->m_szTonf), nDec03, bLSD));
					m_Grid.SetTextMatrix(nRow+j, nCol++, GetStrDouble(GetValueUnitChange(Mo, UNIT_CGS_TONFM, pData->m_szTonfM), nDec03, bLSD));
					m_Grid.SetTextMatrix(nRow+j, nCol++, GetStrDouble(GetValueUnitChange(fc, UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2), nDec33, bLSD));
					m_Grid.SetTextMatrix(nRow+j, nCol++, GetStrDouble(GetValueUnitChange(fca, UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2), nDec33, bLSD));
					m_Grid.SetTextMatrix(nRow+j, nCol++, GetStrDouble(GetValueUnitChange(ft, UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2), nDec33, bLSD));
					m_Grid.SetTextMatrix(nRow+j, nCol++, GetStrDouble(GetValueUnitChange(fta, UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2), nDec33, bLSD));
					m_Grid.SetTextMatrix(nRow+j, nCol, szOK);
					if(!bOK) m_Grid.SetItemFgColour(nRow+j, nCol, RGB(255, 0, 0));
					else m_Grid.SetItemFgColour(nRow+j, nCol, RGB(0, 0, 0));
				}
				else
				{
					double fmax = (Vo/Ap + Mo*yIp)/10;
					double fsa  = m_pBri->IsBoxType() ? GetValueUnitChange(pCalcPile->GetFsa(bEarthquake),pData->m_szKgf_CM2,UNIT_CGS_KGF_CM2) : pCalcPile->m_dArrFsa[nTypePileCase].GetAt(nCombo);

					BOOL bOK = (fmax < fsa)? TRUE:FALSE;
					szOK = (bOK)? "O.K":"N.G";
			
					m_Grid.SetTextMatrix(nRow+j, nCol++, GetStrDouble(GetValueUnitChange(Vo, UNIT_CGS_TONF, pData->m_szTonf), nDec03, bLSD));
					m_Grid.SetTextMatrix(nRow+j, nCol++, GetStrDouble(GetValueUnitChange(Mo, UNIT_CGS_TONFM, pData->m_szTonfM), nDec03, bLSD));
					m_Grid.SetTextMatrix(nRow+j, nCol++, GetStrDouble(GetValueUnitChange(fmax, UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2), nDec33, bLSD));
					m_Grid.SetTextMatrix(nRow+j, nCol++, GetStrDouble(GetValueUnitChange(fsa, UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2), nDec33, bLSD));
					m_Grid.SetTextMatrix(nRow+j, nCol, szOK);
					if(!bOK) m_Grid.SetItemFgColour(nRow+j, nCol, RGB(255, 0, 0));
					else m_Grid.SetItemFgColour(nRow+j, nCol, RGB(0, 0, 0));
				}
			}
			nIdx++;
		}
	}
}

void CInputCheckGrid::SetDataInit_FootingPile_Shear()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	
	long nRow = 0;
	long nCol = 0;
	CString szOK = _T("");
	
	if(!m_pBri->m_FemNormal.IsOutputLoaded() || (m_pBri->GetTypeDesingEQ() == 1 && !m_pBri->GetFemManage(COMBO_EARTHQUAKE)->IsOutputLoaded())) return;
	
	long nIndex = 0;
	long n=0; for(n=0; n<m_nTypeCheck; n++)
	{
		if(m_pBri->IsBoxType()) break;
		if(m_pBri->IsPileBase(n)) nIndex++;
	}
	CDesignPile *pCalcPile = m_pCalc->m_pArrPileCalc.GetAt(nIndex);

	HGBOOL bLSD = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();
	long nDec03 = bLSD ? 0 :3;	
	long nDec33 = 3;	

	long nSizeUse = pCalcPile->GetSizePileLoad(PILE_CASE_USE, TRUE);
	long nSizeExt = bLSD ? pCalcPile->GetSizePileLoad(PILE_CASE_EXTREME) : 0;
	long nCntRow = 1 + (nSizeUse + nSizeExt) * 2;

	long nSizePile = 0;
	if(m_pBri->IsBoxType())
	{
		long nJijum = 0;
		CFootingApp *pFooting = m_pBri->GetFooting(nJijum);
		if(pFooting == NULL) return;
		CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
		if(pPile == NULL) return;

		nSizePile = pPile->m_nCountOrg;
		long nSizeCase = m_pBri->IsApplyEarthQuakeDesign() ? (m_pBri->m_nSelectModelingCountEQ==2 ? 3 : 2) : 1; // 상시,붕괴,기능
		nCntRow = 1 + nSizePile * nSizeCase;
	}

	long nIdx = 0;
	long nCountFixHinge = m_pBri->IsBoxType() ? 1 : 2;
	for(long nTypePileCase = PILE_CASE_USE; nTypePileCase < PILE_CASE_COUNT; nTypePileCase++)
	{
		if(bLSD == FALSE && nTypePileCase > PILE_CASE_USE) break;
		if(nTypePileCase == PILE_CASE_ULTIMATE) continue;

		long nSizeCombo = pCalcPile->GetSizePileLoad(nTypePileCase, TRUE);
		for(long nCombo = 0; nCombo < nSizeCombo; nCombo++)
		{
			CPileLoad *pLoad = pCalcPile->GetPileLoad(nTypePileCase, nCombo, TRUE);
			if(pLoad == NULL) continue;

			BOOL bEarthquake = bLSD? (nTypePileCase == PILE_CASE_EXTREME) : pLoad->IsEarthQuake();
			long nTypeCheck = bLSD ? nTypePileCase : PILE_CASE_NORMAL;		// 강도설계법이고 BOX교가 아닌경우에는 상시조합List에 지진시 조합까지 들어있다.
			if(bLSD == FALSE && m_pBri->IsBoxType() && nRow > nSizePile)
			{
				nTypeCheck = PILE_CASE_EARTH;
			}
			nRow = (nIdx * nCountFixHinge) + 1;
			for(long j = 0; j < nCountFixHinge; j++)
			{
				nCol = bLSD ? 3 : 2;

				BOOL bFix = j == 0 ? TRUE : FALSE;

				double D		= toCm(pCalcPile->m_pPile->m_dDia);
				double T1	= toCm(pCalcPile->m_pPile->m_dT1);
				double T2	= toCm(pCalcPile->m_pPile->m_dT2);
				double D1	= D-T2*2;
				double d		= D-T1*2;
				double alpha	= 4*(D1*D1+D1*d+d*d) / (3*(D1*D1+d*d));	
				double Ap   = toM2(frCm2(pCalcPile->m_Ap));	

				double Ho = 0, Tmax = 0, Tsa = 0;

				if(m_pBri->IsBoxType())
				{
					long nIdxPile = (nRow - 1) % pCalcPile->m_pPile->m_nCountOrg;
					double dDistPile = m_pCalc->m_pFooting->GetDistMaxPile(nIdxPile);
					if(!bLSD)
						dDistPile = toM(dDistPile);

					Ho = pLoad->m_dForceHori * dDistPile;
					Tmax = GetValueUnitChange((alpha * Ho / Ap)*0.1, UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);	// tonf/m2 -> kgf/cm2
					Tsa	= pCalcPile->GetTsa(bEarthquake);
				}
				else
				{
					Ho   = pCalcPile->GetValueMaxMinPileHead(nCombo,PILE_FORCE_HORI,-1,bFix,TRUE, nTypePileCase,TRUE);
					Tmax = GetValueUnitChange((alpha * Ho / Ap)*0.1, UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);	// tonf/m2 -> kgf/cm2
					Tsa  = GetValueUnitChange(pCalcPile->m_dArrTsa[nTypeCheck].GetAt(nCombo), UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);
				}

				Ho = GetValueUnitChange(Ho, UNIT_CGS_TONF, pData->m_szTonf);
				BOOL bOK = (fabs(Tmax) < fabs(Tsa))? TRUE:FALSE;
				szOK = (bOK)? "O.K":"N.G";

				m_Grid.SetTextMatrix(nRow+j ,nCol++, GetStrDouble(Ho, nDec03, bLSD));
				m_Grid.SetTextMatrix(nRow+j ,nCol++, GetStrDouble(Tmax, nDec33, bLSD));
				m_Grid.SetTextMatrix(nRow+j ,nCol++, GetStrDouble(Tsa, nDec33, bLSD));
				m_Grid.SetTextMatrix(nRow+j ,nCol, szOK);
				if(!bOK) m_Grid.SetItemFgColour(nRow+j, nCol, RGB(255, 0, 0));
				else m_Grid.SetItemFgColour(nRow+j, nCol, RGB(0, 0, 0));
			}
			nIdx++;
		}
	}
}

void CInputCheckGrid::SetDataInit_FootingPile_Disp()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	
	long nRow = 0;
	long nCol = 0;
	CString szOK = _T("");
	
	if(!m_pBri->m_FemNormal.IsOutputLoaded() || (m_pBri->GetTypeDesingEQ() == 1 && !m_pBri->GetFemManage(COMBO_EARTHQUAKE)->IsOutputLoaded())) return;
	
	long nIndex = 0;
	long n=0; for(n=0; n<m_nTypeCheck; n++)
	{
		if(m_pBri->IsBoxType()) break;
		if(m_pBri->IsPileBase(n)) nIndex++;
	}
	CDesignPile *pCalcPile = m_pCalc->m_pArrPileCalc.GetAt(nIndex);

	HGBOOL bLSD = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();
	long nDec03 = bLSD ? 0 :3;	
	long nDec33 = 3;	

	long nSizeUse = pCalcPile->GetSizePileLoad(PILE_CASE_USE, TRUE);	//사용한계사이즈
	long nCntRow = 1 + (nSizeUse) * 2;

	long nSizePile = 0;
	if(!bLSD && m_pBri->IsBoxType())
	{
		long nJijum = 0;
		CFootingApp *pFooting = m_pBri->GetFooting(nJijum);
		if(pFooting == NULL) return;
		CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
		if(pPile == NULL) return;

		nSizePile = pPile->m_nCountOrg;
		long nSizeCase = m_pBri->IsApplyEarthQuakeDesign() ? (m_pBri->m_nSelectModelingCountEQ==2 ? 3 : 2) : 1; // 상시,붕괴,기능
		nCntRow = 1 + nSizePile * nSizeCase;
	}

	long nIdx = 0;
	long nCountFixHinge = m_pBri->IsBoxType() ? 1 : 2;
	for(long nTypePileCase = PILE_CASE_USE; nTypePileCase < PILE_CASE_COUNT; nTypePileCase++)
	{
		if(bLSD == FALSE && nTypePileCase == PILE_CASE_EXTREME) continue; // ARCBRIDGE-3343, 한계상태인 경우 극단상황 출력
		if(nTypePileCase == PILE_CASE_ULTIMATE) continue;

		long nSizeCombo = pCalcPile->GetSizePileLoad(nTypePileCase, TRUE);

		for(long nCombo = 0; nCombo < nSizeCombo; nCombo++)
		{
			long nIdxOrg = nCombo;
			CPileLoad *pLoad = pCalcPile->GetPileLoad(nTypePileCase, nCombo, TRUE, nIdxOrg);
			if(pLoad == NULL) continue;

			BOOL bEarthquake = bLSD? (nTypePileCase == PILE_CASE_EXTREME) : pLoad->IsEarthQuake();
			long nTypeCheck = bLSD ? nTypePileCase : (bEarthquake ? PILE_CASE_EARTH : PILE_CASE_NORMAL);
			if(bLSD == FALSE && m_pBri->IsBoxType() && nRow > nSizePile)
			{
				nTypeCheck = PILE_CASE_EARTH;
			}
			nRow = (nIdx * nCountFixHinge) + 1;

			for(long j = 0; j < nCountFixHinge; j++)
			{
				nCol = bLSD ? 0 : 2;

				BOOL bFix = j == 0 ? TRUE : FALSE;

				double DispA  = bEarthquake ? frM(pCalcPile->m_Disa[1]) : frM(pCalcPile->m_Disa[0]);
				double Disp   = 0;
				if(m_pBri->IsBoxType())
				{
					long nIdxArr(0), nIdxPile(0);
					if(bLSD)
					{
						nIdxArr = nTypePileCase;
						nIdxPile = nIdxOrg;
					}
					else
					{
						nIdxArr = bEarthquake ? (nRow <= nSizePile * 2) ? 2 : 3 : 0;
						nIdxPile = (nRow - 1) % nSizePile;
					}
					Disp = frM(m_pCalc->m_pFooting->m_dArrDispX[nIdxArr].GetAt(nIdxPile));
					if(bLSD)
					{
						// 한계상태일때는 변위 결과값이 mm 이기 때문에 단위 다시 돌려 준다.
						Disp = toM(Disp);
					}
				}
				else
				{
					Disp = bFix ? frM(pCalcPile->m_EqFix_deltaX[nTypePileCase].GetAt(nCombo))
						: frM(pCalcPile->m_EqHin_deltaX[nTypePileCase].GetAt(nCombo));
				}

				BOOL bOK = (fabs(Disp) < DispA)? TRUE:FALSE;
				szOK = (bOK)? "O.K":"N.G";

				if(bLSD)
				{
					CString strCombo;
					strCombo.Format("COMBO %d", nCombo + 1);
					CString strFix = bFix ? "FIX" : "HINGE";

					m_Grid.SetTextMatrix(nRow+j,nCol++, GetStrPileCase(nTypePileCase));
					m_Grid.SetTextMatrix(nRow+j,nCol++, strCombo);
					m_Grid.SetTextMatrix(nRow+j,nCol++, strFix);
				}

				m_Grid.SetTextMatrixFormat(nRow+j,nCol++,"%.3f", Disp);
				m_Grid.SetTextMatrixFormat(nRow+j,nCol++,"%.3f", DispA);
				m_Grid.SetTextMatrix(nRow+j,nCol, szOK);
				if(!bOK) m_Grid.SetItemFgColour(nRow+j, nCol, RGB(255, 0, 0));
				else m_Grid.SetItemFgColour(nRow+j, nCol, RGB(0, 0, 0));
			}
			nIdx++;
		}
	}

	if(bLSD)
	{
		long long nMergeRowStt = 1;
		for(long nTypePileCase = PILE_CASE_USE; nTypePileCase < PILE_CASE_COUNT; nTypePileCase++)
		{
			if(bLSD == FALSE && nTypePileCase == PILE_CASE_EXTREME) continue; // ARCBRIDGE-3343, 한계상태인 경우 극단상황 출력
			if(nTypePileCase == PILE_CASE_ULTIMATE) continue;

			long nSizeCombo = pCalcPile->GetSizePileLoad(nTypePileCase, TRUE);

			m_Grid.SetMergeRow(nMergeRowStt, nMergeRowStt + nSizeCombo*nCountFixHinge - 1, 0);
		
			if(m_pBri->IsBoxType() == FALSE)
			{
				for(long ix = 0; ix < nSizeCombo; ++ix)
				{
					m_Grid.SetMergeRow(nMergeRowStt, nMergeRowStt+1, 1);
					nMergeRowStt += nCountFixHinge;
				}
			}
		}
	}
}

void CInputCheckGrid::SetGridTitle_FootingPile_Lsd()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	
	CDesignPile *pCalcPile = GetCurDesignPile();
	CFootingApp *pFooting = m_pBri->GetFootingByJijumNumber(m_nTypeCheck);

	long nCountFixHinge = m_pBri->IsBoxType() ? 1 : 2;
	BOOL bHyunTa = (pFooting->m_exFooting.m_Pile.m_nType == PILE_TYPE_HYUNJANG);
	long nTab = m_ctlTab.GetCurSel();
	long nSizeRow = 0;
	long nSizeCol = 8;
	CString str = _T("");
	if (nTab == 0)
	{
		nSizeRow = pCalcPile->GetSizePileLoad(PILE_CASE_USE, TRUE) + pCalcPile->GetSizePileLoad(PILE_CASE_ULTIMATE) + pCalcPile->GetSizePileLoad(PILE_CASE_EXTREME);
		nSizeRow = nSizeRow * nCountFixHinge + 1;
	}
	else if(nTab == m_nTab_Pile_Disp)
	{
		// 변위검토
		nSizeRow = 0;
		for(long nTypePileCase = PILE_CASE_USE; nTypePileCase < PILE_CASE_COUNT; nTypePileCase++)
		{
			// if(bLSD == FALSE && nTypePileCase == PILE_CASE_EXTREME) continue; // ARCBRIDGE-3343, 한계상태인 경우 극단상황 출력
			if(nTypePileCase == PILE_CASE_ULTIMATE) continue;
			nSizeRow += pCalcPile->GetSizePileLoad(PILE_CASE_USE, TRUE);
		}
		nSizeRow = nSizeRow * nCountFixHinge + 1;
		nSizeCol = 6;
	}
	else if(bHyunTa && nTab == 2)
	{
		// 본체검토
		nSizeRow = pCalcPile->m_sMatHyunta.GetRow();
		nSizeRow = nSizeRow + 1;
		nSizeCol++;
	}
	else
	{
		nSizeRow = pCalcPile->GetSizePileLoad(PILE_CASE_ULTIMATE) + pCalcPile->GetSizePileLoad(PILE_CASE_EXTREME);
		nSizeRow = nSizeRow * nCountFixHinge + 1;
	}

	m_Grid.SetColumnCount(nSizeCol);
	m_Grid.SetRowCount(nSizeRow);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount((bHyunTa && nTab == 2)? 2 : 3);
	m_Grid.SetRowHeightAll(20);
	if(nTab == 3 || (bHyunTa && nTab == 1)) 
		m_Grid.SetColumnWidthAll(90);
	else 
		m_Grid.SetColumnWidthAll(75);
	m_Grid.SetColumnWidth(0, 80);
// 	m_Grid.SetColumnWidth(1, 100);
	
	m_Grid.SetMergeCol(0, 0, (bHyunTa && nTab == 2)? 1 : 2);

	m_Grid.SetTextMatrix(0,0,"구 분");
	m_Grid.SetTextMatrix(0,1,"구 분");
	m_Grid.SetTextMatrix(0,2,"구 분");
	

	if(nTab == 0)
	{
		str.Format("Vmax(%s)", pData->m_szTonf);
		m_Grid.SetTextMatrix(0, 3, str);
		str.Format("Vmin(%s)", pData->m_szTonf);
		m_Grid.SetTextMatrix(0, 4, str);
		str.Format("Qr(%s)", pData->m_szTonf);
		m_Grid.SetTextMatrix(0, 5, str);
		str.Format("Qu(%s)", pData->m_szTonf);
		m_Grid.SetTextMatrix(0, 6, str);
		m_Grid.SetTextMatrix(0, 7, "비 고");
	}
	else if(nTab == m_nTab_Pile_Disp)
	{
		long col = 3;
		m_Grid.SetTextMatrix(0, col++, "수평변위(mm)");
		m_Grid.SetTextMatrix(0, col++, "허용변위(mm)");
		m_Grid.SetTextMatrix(0, col++, "비 고");
	}
	else if(bHyunTa && nTab == 2)
	{
		long col = 2;
		m_Grid.SetTextMatrix(0, col++, "As,min");
		m_Grid.SetTextMatrix(0, col++, "As,use");
		m_Grid.SetTextMatrix(0, col++, "As,max");
		m_Grid.SetTextMatrix(0, col++, "Pu(N)");
		m_Grid.SetTextMatrix(0, col++, "ΦPn(N)");
		m_Grid.SetTextMatrix(0, col++, "안전도");
		m_Grid.SetTextMatrix(0, col++, "비 고");
	}
	else if(nTab == 1)
	{
		long col = 3;
		m_Grid.SetTextMatrix(0, col++, "Pu");
		m_Grid.SetTextMatrix(0, col++, "Pn");
		m_Grid.SetTextMatrix(0, col++, "Φc");
		m_Grid.SetTextMatrix(0, col++, "Pr");
		m_Grid.SetTextMatrix(0, col++, "비 고");
	}
	else if(nTab == 2)
	{
		long col = 3;
		m_Grid.SetTextMatrix(0, col++, "Pu");
		m_Grid.SetTextMatrix(0, col++, "Pu/Pr");
		m_Grid.SetTextMatrix(0, col++, "Mu");
		m_Grid.SetTextMatrix(0, col++, "결과");
		m_Grid.SetTextMatrix(0, col++, "비 고");
	}
	else if(nTab == 3)
	{
		long col = 3;
		str.Format("H (%s)", pData->m_szTonf);
		m_Grid.SetTextMatrix(0, col++, str);
		m_Grid.SetTextMatrix(0, col++, "Fcr");
		m_Grid.SetTextMatrix(0, col++, "Vn");
		m_Grid.SetTextMatrix(0, col++, "Vr");
		m_Grid.SetTextMatrix(0, col++, "비 고");
	}
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputCheckGrid::SetDataInit_FootingPile_Support_Lsd()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	CDesignPile *pCalcPile = GetCurDesignPile();
	const HGBOOL bLsd = TRUE;
	const HGBOOL bBoxType = m_pBri->IsBoxType();

	// (ARCBRIDGE-2580) BOX타입 부재력은 단위폭당(mm) 부재력 이기 때문에 말뚝간격(C.T.C)를 적용해야 한다
	CString str;
	HGINT32 iRow = 1;
	long nCountFixHinge = bBoxType ? 1 : 2;

	for (HGINT32 ixCase = PILE_CASE_USE; ixCase < PILE_CASE_COUNT; ++ixCase)
	{
		const CString strCase = GetStrPileCase(ixCase);
		const HGINT32 icCombo = pCalcPile->GetSizePileLoad(ixCase, TRUE);
		m_Grid.SetMergeRow(iRow, iRow + icCombo * nCountFixHinge - 1, 0);

		for (HGINT32 ixCombo = 0; ixCombo < icCombo; ++ixCombo)
		{
			CPileLoad *pLoad = pCalcPile->GetPileLoad(ixCase, ixCombo, TRUE);
			if(pLoad == NULL) continue;
// 			CString strCombo = pLoad->m_sName;
 			CString strCombo;
 			strCombo.Format("COMBO %d", ixCombo + 1);
			if(nCountFixHinge == 2)
				m_Grid.SetMergeRow(iRow, iRow + 1, 1);

			for (HGINT32 ix = 0; ix < nCountFixHinge; ++ix)
			{
				const HGBOOL bFix = ix == 0;
				CString strFix = bFix ? "FIX" : "HINGE";
				
				m_Grid.SetTextMatrix(iRow, 0, strCase);
				m_Grid.SetTextMatrix(iRow, 1, strCombo);
				m_Grid.SetTextMatrix(iRow, 2, strFix);
				
				HGDOUBLE dVmax(0), dVmin(0);
				if(bBoxType)
				{
					long nIdxOrg = ixCombo % pCalcPile->m_pPile->m_nCountOrg;
					double dDistPile = m_pCalc->m_pFooting->GetDistMaxPile(nIdxOrg);
					dVmax = GetValueUnitChange(pLoad->m_dForceVert * dDistPile, UNIT_CGS_TONF, pData->m_szTonf);
					dVmin = 0;
				}
				else
				{
					dVmax = GetValueUnitChange(pCalcPile->GetValueMaxMinPileHead(ixCombo, PILE_FORCE_AXIAL, -1, bFix, TRUE, ixCase, TRUE), UNIT_CGS_TONF, pData->m_szTonf);
					dVmin = GetValueUnitChange(pCalcPile->GetValueMaxMinPileHead(ixCombo, PILE_FORCE_AXIAL, -1, bFix, FALSE, ixCase, TRUE), UNIT_CGS_TONF, pData->m_szTonf);
				}

				str.Format(_T("%s"), GetStrDouble(dVmax, 0, bLsd));
				m_Grid.SetTextMatrix(iRow, 3, str);

				str.Format(_T("%s"), GetStrDouble(dVmin, 0, bLsd));
				m_Grid.SetTextMatrix(iRow, 4, str);

				const HGDOUBLE dRa = pCalcPile->m_Ra[ixCase];
				str.Format(_T("%s"), GetStrDouble(dRa, 0, bLsd));
				m_Grid.SetTextMatrix(iRow, 5, str);

				const HGDOUBLE dPa = pCalcPile->m_Pa[ixCase];
				str.Format(_T("%s"), GetStrDouble(dPa, 0, bLsd));
				m_Grid.SetTextMatrix(iRow, 6, str);

				str = _T("O.K");
				if(dVmax>dRa) str = _T("N.G");
				if(dVmin<0 && fabs(dVmin)>dPa) str = _T("N.G");
				m_Grid.SetTextMatrix(iRow, 7, str);

				++iRow;
			}
		}
	}
}

void CInputCheckGrid::SetDataInit_FootingPile_AxialForce_Lsd()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	CDesignPile *pCalcPile = GetCurDesignPile();
	const HGBOOL bLsd = TRUE;
	const HGBOOL bBoxType = m_pBri->IsBoxType();

	if(pCalcPile->m_pPile->IsPHC()) return;

	CString str;
	HGINT32 iRow = 1;
	long nCountFixHinge = bBoxType ? 1 : 2;

	for (HGINT32 ixCase = PILE_CASE_ULTIMATE; ixCase < PILE_CASE_COUNT; ++ixCase)
	{
		const CString strCase = GetStrPileCase(ixCase);
		const HGINT32 icCombo = pCalcPile->GetSizePileLoad(ixCase, TRUE);
		m_Grid.SetMergeRow(iRow, iRow + icCombo * nCountFixHinge - 1, 0);

		HGINT32 iRowTable = 1;
		for (HGINT32 ixCombo = 0; ixCombo < icCombo; ++ixCombo)
		{
			CString strCombo;
			strCombo.Format("COMBO %d", ixCombo + 1);
			if(nCountFixHinge == 2)
				m_Grid.SetMergeRow(iRow, iRow + 1, 1);

			for (HGINT32 ix = 0; ix < nCountFixHinge; ++ix)
			{
				const HGBOOL bFix = ix == 0;
				CString strFix = bFix ? "FIX" : "HINGE";

				m_Grid.SetTextMatrix(iRow, 0, strCase);
				m_Grid.SetTextMatrix(iRow, 1, strCombo);
				m_Grid.SetTextMatrix(iRow, 2, strFix);

				if(bBoxType)
				{
					double dDistPile = m_pCalc->m_pFooting->GetDistMaxPile(ixCombo);
					CPileLoad *pLoad = pCalcPile->GetPileLoad(ixCase, ixCombo, TRUE);
					double dP = GetValueUnitChange(pLoad->m_dForceVert * dDistPile, UNIT_CGS_TONF, pData->m_szTonf);
					double dPn = pCalcPile->GetLsdAxialPn();
					double piC = pCalcPile->sLsdCoef.piC[ixCase];
					double dPr = pCalcPile->GetLsdAxialPr(ixCase);

					str = GetStrDouble(dP, 0, bLsd);
					m_Grid.SetTextMatrix(iRow, 3, str);
					str = GetStrDouble(dPn, 0, bLsd);
					m_Grid.SetTextMatrix(iRow, 4, str);
					str = GetStrDouble(piC, 3, bLsd);
					m_Grid.SetTextMatrix(iRow, 5, str);
					str = GetStrDouble(dPr, 0, bLsd);
					m_Grid.SetTextMatrix(iRow, 6, str);
					str = Compare(dPr, dP, NOTRANS(">="))? _T("O.K") : _T("N.G");
					m_Grid.SetTextMatrix(iRow, 7, str);
				}
				else
				{
					str = pCalcPile->m_sCheckAxial[ixCase](iRowTable, 2);
					m_Grid.SetTextMatrix(iRow, 3, str);

					str = pCalcPile->m_sCheckAxial[ixCase](iRowTable, 3);
					m_Grid.SetTextMatrix(iRow, 4, str);

					str = pCalcPile->m_sCheckAxial[ixCase](iRowTable, 4);
					m_Grid.SetTextMatrix(iRow, 5, str);

					str = pCalcPile->m_sCheckAxial[ixCase](iRowTable, 5);
					m_Grid.SetTextMatrix(iRow, 6, str);

					str = pCalcPile->m_sCheckAxial[ixCase](iRowTable, 6);
					m_Grid.SetTextMatrix(iRow, 7, str);
				}

				++iRow;
				++iRowTable;
			}
		}
	}
}

void CInputCheckGrid::SetDataInit_FootingPile_Fa_Lsd()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	CDesignPile *pCalcPile = GetCurDesignPile();
	const HGBOOL bLsd = TRUE;
	const HGBOOL bBoxType = m_pBri->IsBoxType();

	CString str;
	HGINT32 iRow = 1;
	long nCountFixHinge = bBoxType ? 1 : 2;

	for (HGINT32 ixCase = PILE_CASE_ULTIMATE; ixCase < PILE_CASE_COUNT; ++ixCase)
	{
		const CString strCase = GetStrPileCase(ixCase);
		const HGINT32 icCombo = pCalcPile->GetSizePileLoad(ixCase);
		m_Grid.SetMergeRow(iRow, iRow + icCombo * nCountFixHinge - 1, 0);

		HGINT32 iRowTable = 1;
		for (HGINT32 ixCombo = 0; ixCombo < icCombo; ++ixCombo)
		{
			CString strCombo;
			strCombo.Format("COMBO %d", ixCombo + 1);
			if(nCountFixHinge == 2)
				m_Grid.SetMergeRow(iRow, iRow + 1, 1);

			for (HGINT32 ix = 0; ix < nCountFixHinge; ++ix)
			{
				const HGBOOL bFix = ix == 0;
				CString strFix = bFix ? "FIX" : "HINGE";

				m_Grid.SetTextMatrix(iRow, 0, strCase);
				m_Grid.SetTextMatrix(iRow, 1, strCombo);
				m_Grid.SetTextMatrix(iRow, 2, strFix);

				if(bBoxType)
				{
					double dDistPile = m_pCalc->m_pFooting->GetDistMaxPile(ixCombo);
					CPileLoad *pLoad = pCalcPile->GetPileLoad(ixCase, ixCombo, TRUE);
					double dPu = GetValueUnitChange(pLoad->m_dForceVert * dDistPile, UNIT_CGS_TONF, pData->m_szTonf);
					double dMu = GetValueUnitChange(pLoad->m_dMoment * dDistPile, UNIT_CGS_TONFM, pData->m_szTonfM);
					double dPn = pCalcPile->GetLsdAxialPn();
					double piC = pCalcPile->sLsdCoef.piC[ixCase];
					double dPr = pCalcPile->GetLsdAxialPr(ixCase);
					double dMr = pCalcPile->GetLsdAxialMomentMr(ixCase);
					double dResult = 0;
					if(dPu / dPr < 0.2)
					{
						dResult = dPu / (2 * dPr) + dMu / dMr;
					}
					else
					{
						dResult = dPu / dPr + (8./9.) * dMu / dMr;
					}

					str = GetStrDouble(dPu, 0, bLsd);
					m_Grid.SetTextMatrix(iRow, 3, str);
					str = GetStrDouble(dPu/dPr, 3, bLsd);
					m_Grid.SetTextMatrix(iRow, 4, str);
					str = GetStrDouble(dMu, 0, bLsd);
					m_Grid.SetTextMatrix(iRow, 5, str);
					str = GetStrDouble(dResult, 3, bLsd);
					m_Grid.SetTextMatrix(iRow, 6, str);
					str = Compare(dResult, LSD_CHECK_AXIALMOMENT_COEF, NOTRANS("<="))? _T("O.K") : _T("N.G");
					m_Grid.SetTextMatrix(iRow, 7, str);
				}
				else
				{
					str = pCalcPile->m_sCheckAxialMoment[ixCase](iRowTable, 2);
					m_Grid.SetTextMatrix(iRow, 3, str);

					str = pCalcPile->m_sCheckAxialMoment[ixCase](iRowTable, 3);
					m_Grid.SetTextMatrix(iRow, 4, str);

					str = pCalcPile->m_sCheckAxialMoment[ixCase](iRowTable, 4);
					m_Grid.SetTextMatrix(iRow, 5, str);

					str = pCalcPile->m_sCheckAxialMoment[ixCase](iRowTable, 5);
					m_Grid.SetTextMatrix(iRow, 6, str);

					str = pCalcPile->m_sCheckAxialMoment[ixCase](iRowTable, 6);
					m_Grid.SetTextMatrix(iRow, 7, str);
				}

				++iRow;
				++iRowTable;
			}
		}
	}
}

void CInputCheckGrid::SetDataInit_FootingPile_ShaerStress_Lsd()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	CDesignPile *pCalcPile = GetCurDesignPile();
	const HGBOOL bLsd = TRUE;
	const HGBOOL bBoxType = m_pBri->IsBoxType();

	CString str;
	HGINT32 iRow = 1;
	long nCountFixHinge = bBoxType ? 1 : 2;

	for (HGINT32 ixCase = PILE_CASE_ULTIMATE; ixCase < PILE_CASE_COUNT; ++ixCase)
	{
		const CString strCase = GetStrPileCase(ixCase);
		const HGINT32 icCombo = pCalcPile->GetSizePileLoad(ixCase);
		m_Grid.SetMergeRow(iRow, iRow + icCombo * nCountFixHinge - 1, 0);

		HGINT32 iRowTable = 1;
		for (HGINT32 ixCombo = 0; ixCombo < icCombo; ++ixCombo)
		{
			CString strCombo;
			strCombo.Format("COMBO %d", ixCombo + 1);
			if(nCountFixHinge == 2)
				m_Grid.SetMergeRow(iRow, iRow + 1, 1);

			for (HGINT32 ix = 0; ix < nCountFixHinge; ++ix)
			{
				const HGBOOL bFix = ix == 0;
				CString strFix = bFix ? "FIX" : "HINGE";

				m_Grid.SetTextMatrix(iRow, 0, strCase);
				m_Grid.SetTextMatrix(iRow, 1, strCombo);
				m_Grid.SetTextMatrix(iRow, 2, strFix);

				if(bBoxType)
				{
					double dDistPile = m_pCalc->m_pFooting->GetDistMaxPile(ixCombo);
					CPileLoad *pLoad = pCalcPile->GetPileLoad(ixCase, ixCombo, TRUE);
					double dH = GetValueUnitChange(pLoad->m_dForceHori * dDistPile, UNIT_CGS_TONF, pData->m_szTonf);
					double dFcr = pCalcPile->GetLsdShearFcr(ixCase, ixCombo, bFix);
					double dVn = pCalcPile->GetLsdShearVn(ixCase, ixCombo, bFix);
					double dVr = pCalcPile->GetLsdShearVr(ixCase, ixCombo, bFix);

					str = GetStrDouble(dH, 0, bLsd);
					m_Grid.SetTextMatrix(iRow, 3, str);
					str = GetStrDouble(dFcr, 1, bLsd);
					m_Grid.SetTextMatrix(iRow, 4, str);
					str = GetStrDouble(dVn, 0, bLsd);
					m_Grid.SetTextMatrix(iRow, 5, str);
					str = GetStrDouble(dVr, 0, bLsd);
					m_Grid.SetTextMatrix(iRow, 6, str);
					str = Compare(dVr, dH, NOTRANS(">="))? _T("O.K") : _T("N.G");
					m_Grid.SetTextMatrix(iRow, 7, str);
				}
				else
				{
					str = pCalcPile->m_sCheckShear[ixCase](iRowTable, 2);
					m_Grid.SetTextMatrix(iRow, 3, str);

					str = pCalcPile->m_sCheckShear[ixCase](iRowTable, 3);
					m_Grid.SetTextMatrix(iRow, 4, str);

					str = pCalcPile->m_sCheckShear[ixCase](iRowTable, 4);
					m_Grid.SetTextMatrix(iRow, 5, str);

					str = pCalcPile->m_sCheckShear[ixCase](iRowTable, 5);
					m_Grid.SetTextMatrix(iRow, 6, str);

					str = pCalcPile->m_sCheckShear[ixCase](iRowTable, 6);
					m_Grid.SetTextMatrix(iRow, 7, str);
				}

				++iRow;
				++iRowTable;
			}
		}
	}
}

void CInputCheckGrid::SetGridTitle_MainRebar()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pData->IsLsdDesign();
	if (bLsd)
	{
		SetGridTitle_MainRebar_Lsd();
		return;
	}

	long nTab = m_ctlTab.GetCurSel();
	long nSizeSlab = m_pBri->m_pArrConcSlab[eEnvLimit].GetSize();
	long nSizeWall = m_pBri->m_pArrConcWall[eEnvLimit].GetSize();
	long nSizeInWall = m_pBri->m_pArrConcInWall[eEnvLimit].GetSize();
	long nSizeArchRib = m_pBri->m_pArrConcArchRib.GetSize();
	long nSizePRF = m_pBri->m_pArrConcPRF[eEnvLimit].GetSize();

	// 기초의 중간지점부 검토는 주검토위치 하나만 해주기 위해서 시종점부 갯수만
	long nCntFootRev =  m_pBri->m_pArrConcFootRev[eEnvLimit].GetSize();
	long nSizeFoot = m_pBri->m_pArrConcFoot[eEnvLimit].GetSize() + nCntFootRev + m_pBri->m_pArrConcFootCheckAdd[eEnvLimit].GetSize();
	long nSizeFootReal = m_pBri->m_pArrConcFoot[eEnvLimit].GetSize() + m_pBri->m_pArrConcFootRev[eEnvLimit].GetSize() + m_pBri->m_pArrConcFootCheckAdd[eEnvLimit].GetSize();
	long nSizeRow = (nTab == 0) ? (2 + nSizeSlab + nSizeWall + nSizeInWall + nSizeArchRib + nSizeFoot + nSizePRF):(1 + nSizeSlab + nSizeWall + nSizeInWall + nSizeArchRib + nSizeFoot + nSizePRF);
	long nSizeRowReal = (nTab == 0) ? (2 + nSizeSlab + nSizeWall + nSizeInWall + nSizeArchRib + nSizeFootReal + nSizePRF):(1 + nSizeSlab + nSizeWall + nSizeInWall + nSizeArchRib + nSizeFootReal + nSizePRF);
	long nSizeCol = 0;
	CString str = _T("");
	
	long nRow = 0;

	switch(nTab)
	{
		case 0:	nSizeCol = 12; break;
		case 1:	nSizeCol = 5; break;
	}
	
	m_Grid.SetColumnCount(nSizeCol);
	m_Grid.SetRowCount(nSizeRow);
	m_Grid.SetFixedRowCount((nTab==0)? 2:1);
	m_Grid.SetFixedColumnCount(2);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(70);
	m_Grid.SetColumnWidth(0, m_pBri->IsBoxType() ? 100 : 50);
	m_Grid.SetColumnWidth(1, m_pBri->IsBoxType() ? 180 : 140);
	
	if(nTab == 0)
	{
		m_Grid.SetTextMatrix(0,0,"구 분");
		m_Grid.SetMergeCol(0,0,1);
		m_Grid.SetMergeCol(1,0,1);
		m_Grid.SetMergeRow(0,1,0);
		m_Grid.SetMergeRow(0,1,1);

		m_Grid.SetTextMatrix(0, 2, "철근비");
		m_Grid.SetMergeCol(0, 2, 6);
		m_Grid.SetTextMatrix(0, 7, "모멘트");
		m_Grid.SetMergeCol(0, 7, 10);
		m_Grid.SetTextMatrix(1, 2, "Req*4/3");
		m_Grid.SetTextMatrix(1, 3, "Pmin");
		m_Grid.SetTextMatrix(1, 4, "Puse");
		m_Grid.SetTextMatrix(1, 5, "Pmax");
		m_Grid.SetTextMatrix(1, 6, "비 고");
		m_Grid.SetTextMatrix(1, 7, "φMn");
		m_Grid.SetTextMatrix(1, 8, "Mu");
		m_Grid.SetTextMatrix(1, 9, "안전도");
		m_Grid.SetTextMatrix(1, 10, "비 고");
		m_Grid.SetTextMatrix(0, 11, "균열검토");
		m_Grid.SetMergeRow(0,1,11);

		nRow = 2;
	}
	else if(nTab == 1)
	{
		m_Grid.SetTextMatrix(0,0,"구 분");
		m_Grid.SetMergeCol(0,0,1);

		if(pData->m_nTypeKciCode==DESIGN_CODE_KCI_2003)
		{
			m_Grid.SetTextMatrix(0, 2, "W");
			m_Grid.SetTextMatrix(0, 3, "Wa");
		}
		else
		{
			m_Grid.SetTextMatrix(0, 2, "S");
			m_Grid.SetTextMatrix(0, 3, "Sa");
		}
		m_Grid.SetTextMatrix(0, 4, "비 고");

		nRow = 1;
	}

	m_Grid.SetTextMatrix(nRow, 0, "슬래브");
	m_Grid.SetMergeRow(nRow, nRow-1+nSizeSlab, 0);
	m_Grid.SetTextMatrix(nRow+nSizeSlab, 0, "벽  체");
	m_Grid.SetMergeRow(nRow+nSizeSlab, nRow-1+nSizeSlab+nSizeWall, 0);
	if(nSizeInWall)
	{
		m_Grid.SetTextMatrix(nRow+nSizeSlab+nSizeWall, 0, "내측벽체");
		m_Grid.SetMergeRow(nRow+nSizeSlab+nSizeWall, nRow-1+nSizeSlab+nSizeWall+nSizeInWall, 0);
	}
	if(m_pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		m_Grid.SetTextMatrix(nRow+nSizeSlab+nSizeWall+nSizeInWall, 0, "아치리브");
		m_Grid.SetMergeRow(nRow+nSizeSlab+nSizeWall+nSizeInWall, nRow-1+nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib, 0);
	}

	if(nSizeFoot > 0)
	{
		m_Grid.SetTextMatrix(nRow+nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib, 0, "기  초");
	}

	if(nSizePRF > 0)
	{
		long nSttRowPRF = nRow+nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib+nSizeFoot;
		m_Grid.SetTextMatrix(nSttRowPRF, 0, "부상방지저판");
		m_Grid.SetMergeRow(nSttRowPRF, nSttRowPRF+nSizePRF, 0);
	}

	BOOL bFootCheckAdd = (m_pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH || m_pBri->m_nTypeBridge==BRIDGE_TYPE_TRAP2)? TRUE : FALSE;
	CString strSub = _T("");
	BOOL bRev = FALSE;
	BOOL bUpper = FALSE;
	long nRowPos = 0;
	long nIdxFoot = 0;
	long nIdxAdd = 0;
	long n=0; for(n=0; n<nSizeRowReal-nRow; n++)
	{
		CConcBasic *pConc = NULL;
		if(n >= nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib+nSizePRF)
		{
			long nInd = 0;
			if(bFootCheckAdd && m_pBri->m_pArrConcFootCheckAdd[eEnvLimit].GetSize()>0)
			{
				nInd = (n-(nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib));
				
				long nMaxSize = m_pBri->m_bCheckFrontFootTrap2 ? 16 : 12;
				if(nInd < nMaxSize)
				{
					long nIdxSub = 0;
					nIdxSub = nInd/2;

					if(!m_pBri->m_bCheckFrontFootTrap2 && nIdxSub%3 == 2)
					{
						pConc = m_pBri->m_pArrConcFootCheckAdd[eEnvLimit].GetAt(nIdxAdd++);
					}
					else if(m_pBri->m_bCheckFrontFootTrap2 && nIdxSub%4 == 2)
					{
						pConc = m_pBri->m_pArrConcFootCheckAdd[eEnvLimit].GetAt(nIdxAdd++);
						if(bRev)
							nIdxAdd += 2;
					}
					else if(m_pBri->m_bCheckFrontFootTrap2 && nIdxSub%4 == 3)
					{
						pConc = m_pBri->m_pArrConcFootCheckAdd[eEnvLimit].GetAt(nIdxAdd++);
						if(bRev)
							nIdxAdd = 2 ;	//
					}
					else if(!bRev) pConc = m_pBri->m_pArrConcFoot[eEnvLimit].GetAt(nIdxFoot);
					else pConc = m_pBri->m_pArrConcFootRev[eEnvLimit].GetAt(nIdxFoot++);
					str = pConc->m_sTitle;

					bUpper = (pConc->m_sTitle.Right(6) == _T("(뒷굽)"))? !bRev : bRev;
					strSub = (!bUpper)? _T("-하면인장") : _T("-상면인장");
					
					if(pConc->m_sTitle.Left(4) != "지점") str = pConc->m_sTitle + strSub;
					bRev = !bRev;
				}
				else
				{
					if(!bRev) pConc = m_pBri->m_pArrConcFoot[eEnvLimit].GetAt(nIdxFoot);
					else pConc = m_pBri->m_pArrConcFootRev[eEnvLimit].GetAt(nIdxFoot++);
					str = pConc->m_sTitle;

					bUpper = (pConc->m_sTitle.Right(6) == _T("(뒷굽)"))? !bRev	 : bRev;
					strSub = (!bUpper)? _T("-하면인장") : _T("-상면인장");
					
					if(pConc->m_sTitle.Left(4) != "지점") str = pConc->m_sTitle + strSub;
					bRev = !bRev;
				}
			}
			else
			{
				nInd = (n-(nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib))/2;
				
				if(!bRev) pConc = m_pBri->m_pArrConcFoot[eEnvLimit].GetAt(nInd);
				else pConc = m_pBri->m_pArrConcFootRev[eEnvLimit].GetAt(nInd);
				str = pConc->m_sTitle;
				
				bUpper = (pConc->m_sTitle.Right(6) == _T("(뒷굽)"))? !bRev : bRev;
				strSub = (!bUpper)? _T("-하면인장") : _T("-상면인장");
				
				if(pConc->m_sTitle.Left(4) != "지점") str = pConc->m_sTitle + strSub;
				bRev = !bRev;
			}
// 중간에서 빼주면 전체 줄이 안맞는다 값을 0으로 만들어서 안보이게 해주는걸로			
//			if(!bRev && pConc->m_sTitle.Left(8) == "중간지점") continue;
		}
		else if(n >= nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib)
		{
			pConc = m_pBri->m_pArrConcPRF[eEnvLimit].GetAt(n-(nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib));
			str = pConc->m_sTitle;
		}
		else if(n >= nSizeSlab+nSizeWall+nSizeInWall)
		{
			pConc = m_pBri->m_pArrConcArchRib.GetAt(n-(nSizeSlab+nSizeWall+nSizeInWall));
			str = pConc->m_sTitle;
		}
		else if(n >= nSizeSlab + nSizeWall)
		{
			pConc = m_pBri->m_pArrConcInWall[eEnvLimit].GetAt(n-(nSizeSlab+nSizeWall));
			str = pConc->m_sTitle;
		}
		else if(n >= nSizeSlab)
		{
			pConc = m_pBri->GetConcDataWall(n-(nSizeSlab), TRUE);
			str = pConc->m_sTitle;
		}
		else
		{
			pConc = m_pBri->m_pArrConcSlab[eEnvLimit].GetAt(n);
			str = pConc->m_sTitle;
		}

		m_Grid.SetTextMatrix(nRowPos+nRow, 1, str, DT_LEFT, FALSE);
		nRowPos++;
	}

	if(nSizeFoot > 0)
	{
		m_Grid.SetMergeRow(nRow+nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib+nSizePRF, nRowPos+nRow, 0);		// 기초 Merge
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputCheckGrid::SetDataInit_MainRebar()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pData->IsLsdDesign();
	
	long nTab = m_ctlTab.GetCurSel();

	if (bLsd)
	{
		if(nTab == 0) SetDataInit_MainRebar_Stress_Lsd();
		else if(nTab == 1) SetDataInit_MainRebar_Use_Lsd();
	}
	else
	{
		if(nTab == 0) SetDataInit_MainRebar_Stress();
		else if(nTab == 1) SetDataInit_MainRebar_Crack();
	}
}

void CInputCheckGrid::SetDataInit_MainRebar_Stress()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;

	
	long nSizeSlab = m_pBri->m_pArrConcSlab[eEnvLimit].GetSize();
	long nSizeWall = m_pBri->m_pArrConcWall[eEnvLimit].GetSize();
	long nSizeInWall = m_pBri->m_pArrConcInWall[eEnvLimit].GetSize();
	long nSizeArchRib = m_pBri->m_pArrConcArchRib.GetSize();
	long nSizePRF = m_pBri->m_pArrConcPRF[eEnvLimit].GetSize();
	// 기초의 중간지점부 검토는 주검토위치 하나만 해주기 위해서 시종점부 갯수만
	long nSizeFoot = m_pBri->m_pArrConcFoot[eEnvLimit].GetSize() + m_pBri->m_pArrConcFootRev[eEnvLimit].GetSize() + m_pBri->m_pArrConcFootCheckAdd[eEnvLimit].GetSize();
	long nSizeRow = nSizeSlab + nSizeWall + nSizeInWall + nSizeFoot + nSizeArchRib + nSizePRF;

	CString szOK = _T("");
	COLORREF colorbk = RGB(255, 255, 255);

	BOOL bFootCheckAdd = (m_pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH || m_pBri->m_nTypeBridge==BRIDGE_TYPE_TRAP2)? TRUE : FALSE;
	BOOL bRev = FALSE;
	long nRowPos = 0;
	long nIdxFoot = 0;
	long nIdxAdd = 0;
	long n=0; for(n=0; n<nSizeRow; n++)
	{
		long nCol = 0;
		nCol = 2;
		
		CConcBasic *pConc;
		BOOL bFootingConc = FALSE;
		if(n >= nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib+nSizePRF)
		{
			long nInd = 0;
			bFootingConc = TRUE;
			if(bFootCheckAdd && m_pBri->m_pArrConcFootCheckAdd[eEnvLimit].GetSize()>0)
			{
				nInd = (n-(nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib+nSizePRF));
				
				long nMaxSize = m_pBri->m_bCheckFrontFootTrap2 ? 16 : 12;
				if(nInd < nMaxSize)
				{
					long nIdxSub = 0;
					nIdxSub = nInd/2;
					
					if(!m_pBri->m_bCheckFrontFootTrap2 && nIdxSub%3 == 2)
					{
						pConc = m_pBri->m_pArrConcFootCheckAdd[eEnvLimit].GetAt(nIdxAdd++);
					}
					else if(m_pBri->m_bCheckFrontFootTrap2 && nIdxSub%4 == 2)
					{
						pConc = m_pBri->m_pArrConcFootCheckAdd[eEnvLimit].GetAt(nIdxAdd++);
						if(bRev)
							nIdxAdd += 2;
					}
					else if(m_pBri->m_bCheckFrontFootTrap2 && nIdxSub%4 == 3)
					{
						pConc = m_pBri->m_pArrConcFootCheckAdd[eEnvLimit].GetAt(nIdxAdd++);
						if(bRev)
							nIdxAdd = 2 ;	//
					}
					else if(!bRev) pConc = m_pBri->m_pArrConcFoot[eEnvLimit].GetAt(nIdxFoot);
					else pConc = m_pBri->m_pArrConcFootRev[eEnvLimit].GetAt(nIdxFoot++);

					bRev = !bRev;
				}
				else
				{
					if(!bRev) pConc = m_pBri->m_pArrConcFoot[eEnvLimit].GetAt(nIdxFoot);
					else pConc = m_pBri->m_pArrConcFootRev[eEnvLimit].GetAt(nIdxFoot++);

					bRev = !bRev;
				}
			}
			else
			{
				nInd = (n-(nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib+nSizePRF))/2;
				if(!bRev) pConc = m_pBri->m_pArrConcFoot[eEnvLimit].GetAt(nInd);
				else pConc = m_pBri->m_pArrConcFootRev[eEnvLimit].GetAt(nInd);
				
				bRev = !bRev;				
			}

//			if(!bRev && pConc->m_sTitle.Left(8) == "중간지점") continue;
		}
		else if(n >= nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib)
		{
			pConc = m_pBri->m_pArrConcPRF[eEnvLimit].GetAt(n-(nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib));
		}
		else if(n >= nSizeSlab+nSizeWall+nSizeInWall)
		{
			pConc = m_pBri->m_pArrConcArchRib.GetAt(n-(nSizeSlab+nSizeWall+nSizeInWall));
		}
		else if(n >= nSizeSlab+nSizeWall)
		{
			pConc = m_pBri->m_pArrConcInWall[eEnvLimit].GetAt(n-(nSizeSlab+nSizeWall));
		}
		else if(n >= nSizeSlab)
		{
			pConc = m_pBri->GetConcDataWall(n-(nSizeSlab), TRUE);
		}
		else pConc = m_pBri->m_pArrConcSlab[eEnvLimit].GetAt(n);
		
		if(pConc == NULL) continue;
		
		CRectBeam pBeam(pConc, m_pBri->m_bCheckMainSlabAndFootTemper, bFootingConc ? m_pBri->m_bCheckRebarRateFooting : FALSE);
// 배경색 결정
		CString szPos = pConc->m_sTitle.Left(4);
		if(szPos == "시점") colorbk = RGB(235, 235, 255);
		else if(szPos == "종점") colorbk = RGB(255, 255, 200);
		else colorbk = RGB(235, 255, 200);
		if(m_pBri->IsBoxType() && szPos == "상부")
		{
			colorbk = RGB(235, 235, 255);
		}
/////////////////////////////////////////////////////////////////
		BOOL bRebarRate = (pBeam.GetCheckRebarRate() == 0 || pBeam.GetCheckRebarRate() == 1);
		double Pmin = pBeam.GetMinRebarRate();
		double Pmax = pBeam.GetMaxRebarRate();
		double Puse = pBeam.GetUseRebarRate();
		double AsReq = pBeam.GetRebarAreaRequired();
		double dDist = (pBeam.m_pData->m_Ho + pBeam.m_pData->m_Hunch/3.0) - pBeam.m_pData->GetRebarDistTension();
//		double Preq4_3 = (AsReq / ((pConc->m_Ho-pConc->m_RbT1C_dc[0]) * pConc->m_Bo)) *4/3;
		double Preq4_3 = (AsReq / (dDist * pConc->m_Bo)) *4/3;

		double piMn = pBeam.GetPiMn();
		double Mu   = pBeam.GetMu();
		double SF   = pBeam.GetSafeFactorMoment();
		BOOL bOkMoment = pBeam.IsOkMoment();

		double Wa = pBeam.GetWa();
		double W  = pBeam.GetW();
		BOOL bOkCrack = (Wa >= W)? TRUE:FALSE;

		if(pConc->m_nTypeSection==SECTION_WALL)
		{
			CColumn Wall(pConc);
			Wall.CalculateAll(FALSE);

			Puse = Wall.GetRebarUsedRate();
			Pmin = Wall.GetValuePmin();
			Pmax = Wall.GetValuePmax();
			piMn = GetValueUnitChange(Wall.GetPiPn(),UNIT_CGS_TONF,pData->m_szTonf);
			Mu   = GetValueUnitChange(pConc->m_Axial,UNIT_CGS_TONF,pData->m_szTonf);
			SF   = Wall.GetSafeFactorAxial();
			bOkMoment = Wall.IsOkAxial() ? TRUE : FALSE;
			bRebarRate = (Wall.GetCheckRebarRate()==1) ? TRUE : FALSE;
			
			if(m_pBri->m_bOutWallUsebilityCheck == FALSE)
				bOkCrack = TRUE;	// 벽체를 벽체로 검토하는 옵션일 경우에는 사용성검토가 되지 않기 때문에 균열검토를 출력하지 않음(10908)
		}

		if(Compare(Mu, 0.0, _T("="), 0.01) == FALSE)
		{
			szOK = (bRebarRate)? "O.K":"N.G";
			m_Grid.SetTextMatrixFormat(nRowPos+2,nCol++,pConc->m_nTypeSection==SECTION_WALL ? "-" : "%.5f", Preq4_3);
			m_Grid.SetTextMatrixFormat(nRowPos+2,nCol++,"%.5f", Pmin);
			m_Grid.SetTextMatrixFormat(nRowPos+2,nCol++,"%.5f", Puse);
			m_Grid.SetTextMatrixFormat(nRowPos+2,nCol++,"%.5f", Pmax);
			m_Grid.SetTextMatrix(nRowPos+2, nCol, szOK);
			if(!bRebarRate) m_Grid.SetItemFgColour(nRowPos+2, nCol++, RGB(255, 0, 0));
			else m_Grid.SetItemFgColour(nRowPos+2, nCol++, RGB(0, 0, 0));

			szOK = (bOkMoment)? "O.K":"N.G";
			m_Grid.SetTextMatrixFormat(nRowPos+2,nCol++,"%.3f", piMn);
			m_Grid.SetTextMatrixFormat(nRowPos+2,nCol++,"%.3f", Mu);
			m_Grid.SetTextMatrixFormat(nRowPos+2,nCol++,"%.3f", SF);
			m_Grid.SetTextMatrix(nRowPos+2,nCol, szOK);
			if(!bOkMoment) m_Grid.SetItemFgColour(nRowPos+2, nCol++, RGB(255, 0, 0));
			else m_Grid.SetItemFgColour(nRowPos+2, nCol++, RGB(0, 0, 0));

//			if(pConc->m_nTypeSection==SECTION_SINGLE)
//			{
				szOK = (bOkCrack)? "O.K":"N.G";
				m_Grid.SetTextMatrix(nRowPos+2,nCol, (pConc->m_nTypeSection==SECTION_WALL && m_pBri->m_bOutWallUsebilityCheck==FALSE) ? "-" : szOK);
				if(!bOkCrack) m_Grid.SetItemFgColour(nRowPos+2, nCol++, RGB(255, 0, 0));
				else m_Grid.SetItemFgColour(nRowPos+2, nCol++, RGB(0, 0, 0));
//			}
//			else
//			{
//				m_Grid.SetTextMatrix(nRowPos+2,nCol, "-");
//			}

			int i=0; for(i=2; i< 12; i++)
			{
				m_Grid.SetItemBkColour(nRowPos+2, i, colorbk);
			}
		}
		else 
		{
			int i=0; for(i=2; i< 12; i++)
			{
				m_Grid.SetTextMatrix(nRowPos+2, i, "-");
				m_Grid.SetItemBkColour(nRowPos+2, i, colorbk);
			}
			m_Grid.SetRowHeight(nRowPos+2,0);
		}
		nRowPos++;
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputCheckGrid::SetDataInit_MainRebar_Crack()
{
	long nSizeSlab = m_pBri->m_pArrConcSlab[eEnvLimit].GetSize();
	long nSizeWall = m_pBri->m_pArrConcWall[eEnvLimit].GetSize();
	long nSizeInWall = m_pBri->m_pArrConcInWall[eEnvLimit].GetSize();
	long nSizeArchRib = m_pBri->m_pArrConcArchRib.GetSize();
	long nSizePRF = m_pBri->m_pArrConcPRF[eEnvLimit].GetSize();
	long nSizeFoot = m_pBri->m_pArrConcFoot[eEnvLimit].GetSize() + m_pBri->m_pArrConcFootRev[eEnvLimit].GetSize() + m_pBri->m_pArrConcFootCheckAdd[eEnvLimit].GetSize();
	long nSizeRow  = nSizeSlab + nSizeWall + nSizeInWall + nSizeFoot + nSizeArchRib + nSizePRF;
	CString szOK = _T("");
	COLORREF colorbk = RGB(255, 255, 255);
	
	BOOL bFootCheckAdd = (m_pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH || m_pBri->m_nTypeBridge==BRIDGE_TYPE_TRAP2)? TRUE : FALSE;
	BOOL bRev = FALSE;
	long nRowPos = 0;
	long nIdxFoot = 0;
	long nIdxAdd = 0;
	long n=0; for(n=0; n<nSizeRow; n++)
	{
		long nCol = 0;
		nCol = 2;
		CConcBasic *pConc;
		if(n >= nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib+nSizePRF)
		{
			long nInd = 0;
			if(bFootCheckAdd && m_pBri->m_pArrConcFootCheckAdd[eEnvLimit].GetSize()>0)
			{
				nInd = (n-(nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib+nSizePRF));
				long nMaxSize = m_pBri->m_bCheckFrontFootTrap2 ? 16 : 12;
				if(nInd < nMaxSize)
				{
					long nIdxSub = 0;
					nIdxSub = nInd/2;
					
					if(!m_pBri->m_bCheckFrontFootTrap2 && nIdxSub%3 == 2)
					{
						pConc = m_pBri->m_pArrConcFootCheckAdd[eEnvLimit].GetAt(nIdxAdd++);
					}
					else if(m_pBri->m_bCheckFrontFootTrap2 && nIdxSub%4 == 2)
					{
						pConc = m_pBri->m_pArrConcFootCheckAdd[eEnvLimit].GetAt(nIdxAdd++);
						if(bRev)
							nIdxAdd += 2;
					}
					else if(m_pBri->m_bCheckFrontFootTrap2 && nIdxSub%4 == 3)
					{
						pConc = m_pBri->m_pArrConcFootCheckAdd[eEnvLimit].GetAt(nIdxAdd++);
						if(bRev)
							nIdxAdd = 2 ;	//
					}
					else if(!bRev) pConc = m_pBri->m_pArrConcFoot[eEnvLimit].GetAt(nIdxFoot);
					else pConc = m_pBri->m_pArrConcFootRev[eEnvLimit].GetAt(nIdxFoot++);
					
					bRev = !bRev;
				}
				else
				{
					if(!bRev) pConc = m_pBri->m_pArrConcFoot[eEnvLimit].GetAt(nIdxFoot);
					else pConc = m_pBri->m_pArrConcFootRev[eEnvLimit].GetAt(nIdxFoot++);
					
					bRev = !bRev;
				}
			}
			else
			{
				nInd = (n-(nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib+nSizePRF))/2;
				if(!bRev) pConc = m_pBri->m_pArrConcFoot[eEnvLimit].GetAt(nInd);
				else pConc = m_pBri->m_pArrConcFootRev[eEnvLimit].GetAt(nInd);
				
				bRev = !bRev;				
			}
//			if(!bRev && pConc->m_sTitle.Left(8) == "중간지점") continue;
		}
		else if(n >= nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib)
		{
			pConc = m_pBri->m_pArrConcPRF[eEnvLimit].GetAt(n-(nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib));
		}
		else if(n >= nSizeSlab+nSizeWall+nSizeInWall)
		{
			pConc = m_pBri->m_pArrConcArchRib.GetAt(n-(nSizeSlab+nSizeWall+nSizeInWall));
		}
		else if(n >= nSizeSlab+nSizeWall)
		{
			pConc = m_pBri->m_pArrConcInWall[eEnvLimit].GetAt(n-(nSizeSlab+nSizeWall));
		}
		else if(n >= nSizeSlab)
		{
			pConc = m_pBri->GetConcDataWall(n-(nSizeSlab), TRUE);
		}
		else 
			pConc = m_pBri->m_pArrConcSlab[eEnvLimit].GetAt(n);
		
		CRectBeam pBeam(pConc, m_pBri->m_bCheckMainSlabAndFootTemper);

		// 배경색 결정
		CString szPos = pConc->m_sTitle.Left(4);
		if(szPos == "시점") colorbk = RGB(235, 235, 255);
		else if(szPos == "종점") colorbk = RGB(255, 255, 200);
		else colorbk = RGB(235, 255, 200);
		if(m_pBri->IsBoxType() && szPos == "상부")
		{
			colorbk = RGB(235, 235, 255);
		}
		/////////////////////////////////////////////////////////////////

		double Wa = pBeam.GetWa();
		double W  = pBeam.GetW();
//		if(Wa==0 && W==0) continue;
		BOOL bOkCrack = (Wa >= W)? TRUE:FALSE;
		szOK = (bOkCrack)? "O.K":"N.G";

		// 벽체를 벽체로 검토하는 옵션일 경우에는 사용성검토가 되지 않기 때문에 균열검토를 출력하지 않음(10908)
		if(pConc->m_nTypeSection==SECTION_WALL && m_pBri->m_bOutWallUsebilityCheck == FALSE)
			W = 0;

		if(W > 0 && Compare(pConc->m_MomentUse, 0.0, _T("="), 0.01) == FALSE)
		{
			m_Grid.SetTextMatrixFormat(nRowPos+1,nCol++,"%.3f", W);
			m_Grid.SetTextMatrixFormat(nRowPos+1,nCol++,"%.3f", Wa);
			m_Grid.SetTextMatrix(nRowPos+1,nCol, szOK);
			if(!bOkCrack) m_Grid.SetItemFgColour(nRowPos+1, nCol++, RGB(255, 0, 0));
			else m_Grid.SetItemFgColour(nRowPos+1, nCol++, RGB(0, 0, 0));
		}
		else
		{
			m_Grid.SetTextMatrix(nRowPos+1,nCol++,"-");
			m_Grid.SetTextMatrix(nRowPos+1,nCol++,"-");
			m_Grid.SetTextMatrix(nRowPos+1,nCol, "-");
			m_Grid.SetRowHeight(nRowPos+1,0);
		}

		int i=0; for(i=2; i< 5; i++)
		{
			m_Grid.SetItemBkColour(nRowPos+1, i, colorbk);
		}
		nRowPos++;
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputCheckGrid::SetGridTitle_MainRebar_Lsd()
{
	long nTab = m_ctlTab.GetCurSel();
	
	if(nTab == 0)
	{
		long nSizeSlab = m_pBri->GetCountConcSlab();
		long nSizeWall = m_pBri->GetCountConcOutWall();
		long nSizeInWall = m_pBri->GetCountConcInWall();
		long nSizePRF = m_pBri->GetCountConcPRF();

		long nSizeFoot = m_pBri->GetCountConcFooting(TRUE, TRUE);
		long nSizeRow = 2 + nSizeSlab + nSizeWall + nSizeInWall + nSizePRF + nSizeFoot;
		long nSizeCol = m_pBri->m_bCheckDeltaTR ? 12 : 11;

		m_Grid.SetColumnCount(nSizeCol);
		m_Grid.SetRowCount(nSizeRow);
		m_Grid.SetFixedRowCount(2);
		m_Grid.SetFixedColumnCount(3);
		m_Grid.SetRowHeightAll(20);
		m_Grid.SetColumnWidthAll(70);
		m_Grid.SetColumnWidth(0, m_pBri->IsBoxType() ? 100 : 70);
		m_Grid.SetColumnWidth(1, 140);

		m_Grid.SetTextMatrix(0,0,"구 분");
		m_Grid.SetMergeCol(0,0,2);
		m_Grid.SetMergeCol(1,0,2);
		m_Grid.SetMergeRow(0,1,0);
		m_Grid.SetMergeRow(0,1,1);
		m_Grid.SetMergeRow(0,1,2);

		m_Grid.SetTextMatrix(0, 3, "철근량");
		m_Grid.SetMergeCol(0, 3, 6);
		m_Grid.SetTextMatrix(0, 7, "모멘트");
		m_Grid.SetMergeCol(0, 7, m_pBri->m_bCheckDeltaTR ? 11 : 10);
		m_Grid.SetTextMatrix(1, 3, "As,min");
		m_Grid.SetTextMatrix(1, 4, "As,use");
		m_Grid.SetTextMatrix(1, 5, "As,max");
		m_Grid.SetTextMatrix(1, 6, "비 고");
		m_Grid.SetTextMatrix(1, 7, "Mr");
		m_Grid.SetTextMatrix(1, 8, "Mu");
		m_Grid.SetTextMatrix(1, 9, "안전도");
		m_Grid.SetTextMatrix(1, 10, "비 고");
		if(m_pBri->m_bCheckDeltaTR)
		{
			m_Grid.SetTextMatrix(1, 11, "추가인장력");
		}
	}
	else if(nTab == 1)
	{
		long nCntInWall = (m_pBri->m_nSelectInWallSectionDesign == 0)? m_pBri->GetCountInWall() : 0;
		long nSizeSlab = m_pBri->m_pArrConcSlab[eEnvService].GetSize();
		long nSizeWall = (m_pBri->IsOutsideWall() && (m_pBri->m_nSelectWallSectionDesign==0))? MIN(10, m_pBri->m_pArrConcWall[eEnvService].GetSize()):0;
		if(m_pBri->IsOutsideWall() && m_pBri->m_nSelectWallSectionDesign==1 && m_pBri->m_bOutWallUsebilityCheck)
			nSizeWall = 2;
		long nSizeInWall = MIN(nCntInWall*6, m_pBri->m_pArrConcInWall[eEnvService].GetSize());
		long nSizePRF = m_pBri->m_pArrConcPRF[eEnvService].GetSize();
		long nSizeFoot = m_pBri->m_pArrConcFoot[eEnvService].GetSize() + m_pBri->m_pArrConcFootRev[eEnvService].GetSize() + m_pBri->m_pArrConcFootCheckAdd[eEnvService].GetSize();
		long nSizeRow = 2 + nSizeSlab + nSizeWall + nSizeInWall + nSizePRF + nSizeFoot;
		long nSizeCol = 12;

		m_Grid.SetColumnCount(nSizeCol);
		m_Grid.SetRowCount(nSizeRow);
		m_Grid.SetFixedRowCount(2);
		m_Grid.SetFixedColumnCount(2);
		m_Grid.SetRowHeightAll(20);
		m_Grid.SetColumnWidthAll(70);
		m_Grid.SetColumnWidth(0, m_pBri->IsBoxType() ? 100 : 70);
		m_Grid.SetColumnWidth(1, 140);

		m_Grid.SetTextMatrix(0,0,"구 분");
		m_Grid.SetMergeCol(0,0,1);
		m_Grid.SetMergeCol(1,0,1);
		m_Grid.SetMergeRow(0,1,0);
		m_Grid.SetMergeRow(0,1,1);

		m_Grid.SetTextMatrix(0, 2, "균열");
		m_Grid.SetMergeRow(0,1,2);
		m_Grid.SetTextMatrix(0, 3, "최소철근량");
		m_Grid.SetMergeCol(0, 3, 5);
		m_Grid.SetTextMatrix(0, 6, "간접균열검토");
		m_Grid.SetMergeCol(0, 6, 8);
		m_Grid.SetTextMatrix(0, 9, "균열폭 검토");
		m_Grid.SetMergeCol(0, 9, 11);
		m_Grid.SetTextMatrix(1, 3, "As,min");
		m_Grid.SetTextMatrix(1, 4, "As,use");
		m_Grid.SetTextMatrix(1, 5, "비고");
		m_Grid.SetTextMatrix(1, 6, "fs,max");
		m_Grid.SetTextMatrix(1, 7, "fs");
		m_Grid.SetTextMatrix(1, 8, "비고");
		m_Grid.SetTextMatrix(1, 9, "W k");
		m_Grid.SetTextMatrix(1, 10, "Wa");
		m_Grid.SetTextMatrix(1, 11, "비고");
	}
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputCheckGrid::SetDataInit_MainRebar_Stress_Lsd()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pData->IsLsdDesign();
	
	long nSizeSlab = m_pBri->GetCountConcSlab();
	long nSizeWall = m_pBri->GetCountConcOutWall();
	long nSizeInWall = m_pBri->GetCountConcInWall();
	long nSizePRF = m_pBri->GetCountConcPRF();

	long nSizeRow = nSizeSlab + nSizeWall + nSizeInWall + nSizePRF;

	const HGINT32 icValidEnvType = m_pBri->GetCountApplyEnvType(eEnvService);

	long nRowPos = 0;
	long n=0; for(n=0; n<nSizeRow; n++)
	{
		CConcBasic *pConc = NULL;
		CString strPos;
		CString strEnv;
		if (n < nSizeSlab)
		{
			strPos = "슬래브";
			if(m_pBri->IsBoxType())
			{
				strPos = (n < nSizeSlab/2 ) ? "상부슬래브" : "하부슬래브";
			}

			pConc = m_pBri->GetConcDataSlab_Lsd(n, &strEnv);

			if(m_pBri->IsBoxType())
			{
				if (n == 0)
				{
					m_Grid.SetMergeRow(2, 2 + nSizeSlab / 2 - 1, 0);
				}
				else if (n == 2 + nSizeSlab / 2)
				{
					m_Grid.SetMergeRow(2 + nSizeSlab / 2, 2 + nSizeSlab - 1, 0);
				}
			}
			else
			{
				if (n == 0)
				{
					m_Grid.SetMergeRow(2, 2 + nSizeSlab - 1, 0);
				}
			}
		}
		else if (n < nSizeSlab + nSizeWall)
		{
			pConc = m_pBri->GetConcDataWall_Lsd(n-(nSizeSlab), &strEnv);
			if (n-nSizeSlab<nSizeWall / 2)
			{
				strPos = "시점벽체";
			}
			else
			{
				strPos = "종점벽체";
			}

			if (n == nSizeSlab)
			{
				m_Grid.SetMergeRow(2 + nSizeSlab, 2 + nSizeSlab + nSizeWall / 2 - 1, 0);
			}
			else if (n == nSizeSlab + nSizeWall / 2)
			{
				m_Grid.SetMergeRow(2 + nSizeSlab + nSizeWall / 2, 2 + nSizeSlab + nSizeWall - 1, 0);
			}
		}
		else if (n < nSizeSlab + nSizeWall + nSizeInWall)
		{
			pConc = m_pBri->GetConcDataInWall_Lsd(n-(nSizeSlab+nSizeWall), &strEnv);
			long nIdx = pConc->m_sTitle.Find(" ");
			if(nIdx > 0)
				strPos = pConc->m_sTitle.Left(nIdx);
			else
				strPos = pConc->m_sTitle.Left(9);

			if ((n - (nSizeSlab + nSizeWall + nSizeInWall)) % (6 * icValidEnvType) == 0)
			{
				m_Grid.SetMergeRow(2 + nRowPos, 2 + nRowPos + 6 * icValidEnvType - 1, 0);
			}
		}
		else if(n < nSizeSlab + nSizeWall + nSizeInWall + nSizePRF)
		{
			pConc = m_pBri->GetConcDataPRF_Lsd(n-(nSizeSlab+nSizeWall+nSizeInWall), &strEnv);
			long nIdx = pConc->m_sTitle.Find(" ");
			if(nIdx > 0)
				strPos = pConc->m_sTitle.Left(nIdx);
			else
				strPos = pConc->m_sTitle.Left(9);

			if (n == nSizeSlab + nSizeWall + nSizeInWall)
			{
				m_Grid.SetMergeRow(2 + nRowPos, 2 + nRowPos + nSizePRF - 1, 0);
			}
		}

		if (n % icValidEnvType == 0)
		{
			m_Grid.SetMergeRow(2 + nRowPos, 2 + nRowPos + icValidEnvType - 1, 1);
		}

		if(pConc == NULL) 
			continue;

		CString strPos1;
		HGINT32 ix = pConc->m_sTitle.Find(" ");
		if(ix > 0)
		{
			strPos1 = pConc->m_sTitle.Mid(ix);
		}
		else
		{
			strPos1 = pConc->m_sTitle;
		}
		
		SetTextMatrix_MainRebar_Stress(pConc, nRowPos, strPos, strPos1, strEnv);
		
		nRowPos++;
	}
	
	const HGBOOL bBox = m_pBri->IsBoxType();
	const HGBOOL bSlab = m_pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB;
	const HGBOOL bTrap2 = m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2;
	const HGBOOL bIsOutWall = m_pBri->IsOutsideWall();
	const HGINT32 icJijum = bSlab || bBox ? 0 : bTrap2 ? m_pBri->GetCountJijum() - 2 : m_pBri->GetCountJijum();

	for (HGINT32 ix = 0; ix < icJijum; ++ix)
	{
		if (bIsOutWall == FALSE && (ix == 0 || ix == 1))
			continue;

		const HGINT32 ixJijum = ix == 0 ? 0 : ix == 1 ? m_pBri->GetCountJijum() - 1 : ix - 1;

		CConcBasic *pConc = NULL;
		const HGINT32 icPos = GetCountFootPos(ixJijum);
		m_Grid.SetMergeRow(2 + nRowPos, 2 + nRowPos + icPos * icValidEnvType - 1, 0);

		for (HGINT32 ixPos = 0; ixPos < icPos; ++ixPos)
		{
			CString strPos, strPos1;
			HGBOOL bFront(TRUE), bUpper(TRUE);
			HGINT32 ixCheckAdd = -1;
			GetFootInfo_Lsd(ixJijum, ixPos, strPos, strPos1, bFront, bUpper, ixCheckAdd);
			m_Grid.SetMergeRow(2 + nRowPos, 2 + nRowPos + icValidEnvType - 1, 1);

			for (HGINT32 ixEnvType = eEnvLimit; ixEnvType < eEnvService; ++ixEnvType)
			{
				const ETypeEnvComboLsd eEnvType = static_cast<ETypeEnvComboLsd>(ixEnvType);
				if (m_pBri->IsValidComboEnvType(eEnvType) == FALSE)
					continue;
				
				CString strEnv = ixEnvType == eEnvLimit ? "극한" : ixEnvType == eEnvExtremeI ? "극단1" : "극단2";
				pConc = m_pBri->GetConcDataFooting(ixJijum, bFront, ix < 2 ? (bUpper == bFront) : bUpper, ixCheckAdd, eEnvType);
				SetTextMatrix_MainRebar_Stress(pConc, nRowPos, strPos, strPos1, strEnv);

				nRowPos++;
			}
		}
	}
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputCheckGrid::SetTextMatrix_MainRebar_Stress(CConcBasic *pConc, long nRowPos, const CString &strPos, const CString &strPos1, const CString &strEnv)
{
	long nCol = 0;

	m_Grid.SetTextMatrix(nRowPos + 2, nCol++, strPos);
	m_Grid.SetTextMatrix(nRowPos + 2, nCol++, strPos1);
	m_Grid.SetTextMatrix(nRowPos + 2, nCol++, strEnv);

	if (pConc == NULL)
		return;
	
	CLsdBeam pBeam(pConc, strEnv == "극한" ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN);
	SetDataInit_Stress_Lsd(pBeam, nRowPos + 2, nCol);
}

void CInputCheckGrid::SetDataInit_MainRebar_Use_Lsd()
{
	CRcBridgeRebar *pBri = m_pBri;

	long nCntInWall = (pBri->m_nSelectInWallSectionDesign == 0)? pBri->GetCountInWall() : 0;
	long nSizeSlab = pBri->m_pArrConcSlab[eEnvService].GetSize();
	long nSizeWall = (pBri->IsOutsideWall() && (pBri->m_nSelectWallSectionDesign==0))? MIN(10, pBri->m_pArrConcWall[eEnvService].GetSize()):0;
	// (ARCBRIDGE-3618) LSD에서 휨압축부재로 검토시 사용성검토는 Baem검토 데이터를 쓰지않고 기둥검토 데이터를 써달라는 요청으로 따로 출력한다.
	if(pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign==1 && pBri->m_bOutWallUsebilityCheck)
		nSizeWall = 2;
	
	long nSizeInWall = MIN(nCntInWall*6, pBri->m_pArrConcInWall[eEnvService].GetSize());
	long nSizePRF = pBri->m_pArrConcPRF[eEnvService].GetSize();

	HGINT32 ixRow = 1;
	for(HGINT32 ix=0; ix<nSizeSlab+nSizeWall+nSizeInWall+nSizePRF; ++ix)
	{
		++ixRow;

		CString strPos;
		HGINT32 ixCol = 0;
		CConcBasic *pConc = NULL;
		if(ix < nSizeSlab)
		{
			pConc = pBri->m_pArrConcSlab[eEnvService].GetAt(ix);

			strPos = "슬래브";
			if(m_pBri->IsBoxType())
			{
				strPos = (ix < nSizeSlab/2) ? "상부슬래브" : "하부슬래브";
			}

			if(m_pBri->IsBoxType())
			{
				if (ix == 0)
				{
					m_Grid.SetMergeRow(2, 2 + nSizeSlab / 2 - 1, 0);
				}
				else if (ix == 2 + nSizeSlab / 2)
				{
					m_Grid.SetMergeRow(2 + nSizeSlab / 2, 2 + nSizeSlab - 1, 0);
				}
			}
			else
			{
				if (ix == 0)
				{
					m_Grid.SetMergeRow(2, 2 + nSizeSlab - 1, 0);
				}
			}
		}
		else if(ix<(nSizeSlab + nSizeWall))
		{
			BOOL bSttWall(FALSE);
			if (ix-nSizeSlab<nSizeWall / 2)
			{
				strPos = "시점벽체";
				bSttWall = TRUE;
			}
			else
			{
				strPos = "종점벽체";
				bSttWall = FALSE;
			}

			if(pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign==1 && pBri->m_bOutWallUsebilityCheck)
			{
				pConc = m_pCalc->m_pColGir->GetConcWallForOutWallUsebilityCheck(bSttWall);
			}
			else
				pConc = pBri->GetConcDataWall(ix-nSizeSlab, TRUE, eEnvService);

			if (ix == nSizeSlab)
			{
				m_Grid.SetMergeRow(2 + nSizeSlab, 2 + nSizeSlab + nSizeWall / 2 - 1, 0);
			}
			else if (ix == nSizeSlab + nSizeWall / 2)
			{
				m_Grid.SetMergeRow(2 + nSizeSlab + nSizeWall / 2, 2 + nSizeSlab + nSizeWall - 1, 0);
			}
		}
		else if(ix<(nSizeSlab + nSizeWall + nSizeInWall))
		{
			pConc = pBri->m_pArrConcInWall[eEnvService].GetAt(ix-(nSizeSlab + nSizeWall));
			long nIdx = pConc->m_sTitle.Find(" ");
			if(nIdx > 0)
				strPos = pConc->m_sTitle.Left(nIdx);
			else
				strPos = pConc->m_sTitle.Left(9);

			if ((ix - (nSizeSlab + nSizeWall + nSizeInWall)) % 6 == 0)
			{
				m_Grid.SetMergeRow(ixRow, ixRow + 5, 0);
			}
		}
		else if(ix<(nSizeSlab + nSizeWall + nSizeInWall + nSizePRF))
		{
			pConc = pBri->m_pArrConcPRF[eEnvService].GetAt(ix-(nSizeSlab + nSizeWall+nSizeInWall));
			long nIdx = pConc->m_sTitle.Find(" ");
			if(nIdx > 0)
				strPos = pConc->m_sTitle.Left(nIdx);
			else
				strPos = pConc->m_sTitle.Left(9);

			if(ix == nSizeSlab + nSizeWall + nSizeInWall)
			{
				m_Grid.SetMergeRow(ixRow, ixRow + nSizePRF, 0);
			}
		}

		CString strPos1;
		HGINT32 n = pConc->m_sTitle.Find(" ");
		if(n > 0)
		{
			strPos1 = pConc->m_sTitle.Mid(n);
		}
		else
		{
			strPos1 = pConc->m_sTitle;
		}
		
		SetTextMatrix_MainRebar_Use(pConc, ixRow, strPos, strPos1);
	}
	
	const HGBOOL bBox = m_pBri->IsBoxType();
	const HGBOOL bSlab = m_pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB;
	const HGBOOL bTrap2 = m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2;
	const HGBOOL bIsOutWall = m_pBri->IsOutsideWall();
	const HGINT32 icJijum = bSlab || bBox ? 0 : bTrap2 ? m_pBri->GetCountJijum() - 2 : m_pBri->GetCountJijum();
	for (HGINT32 ix = 0; ix < icJijum; ++ix)
	{
		if (bIsOutWall == FALSE && (ix == 0 || ix == 1))
			continue;

		const HGINT32 ixJijum = ix == 0 ? 0 : ix == 1 ? m_pBri->GetCountJijum() - 1 : ix - 1;
		const HGINT32 icPos = GetCountFootPos(ixJijum);
		m_Grid.SetMergeRow(ixRow + 1, ixRow + icPos, 0);

		for (HGINT32 ixPos = 0; ixPos < icPos; ++ixPos)
		{
			++ixRow;
			
			CConcBasic *pConc = NULL;
			CString strPos, strPos1;
			HGBOOL bFront(TRUE), bUpper(TRUE);
			HGINT32 ixCheckAdd = -1;
			if (GetFootInfo_Lsd(ixJijum, ixPos, strPos, strPos1, bFront, bUpper, ixCheckAdd))
			{
				pConc = m_pBri->GetConcDataFooting(ixJijum, bFront, bUpper == bFront, ixCheckAdd, eEnvService);
			}
			
			SetTextMatrix_MainRebar_Use(pConc, ixRow, strPos, strPos1);
		}
	}
}

void CInputCheckGrid::SetTextMatrix_MainRebar_Use(CConcBasic * pConc, HGINT32 ixRow, const CString &strPos, const CString &strPos1)
{
	const HGBOOL bLsd = TRUE;

	HGINT32 ixCol = 0;
	m_Grid.SetTextMatrix(ixRow, ixCol++, strPos);
	m_Grid.SetTextMatrix(ixRow, ixCol++, strPos1);

	if (pConc == NULL)
		return;

	CLsdBeam pBeam(pConc, LSD_ULS_DESIGN);
	SetDataInit_Use_Lsd(pBeam, ixRow, ixCol);
}

void CInputCheckGrid::SetGridTitle_Corner()
{
	long nSizeRow = m_pBri->IsBoxType() ? 5 : 3; // 타이틀 1, 시점, 종점
	long nSizeCol = 6;
	CString str = _T("");
	
	BOOL bOut = (m_nTypeCheck == 0) ? TRUE : FALSE;
	m_Grid.SetColumnCount(nSizeCol);
	m_Grid.SetRowCount(nSizeRow);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(80);
	
	m_Grid.SetTextMatrix(0,0,"구 분");
	
	m_Grid.SetTextMatrix(0, 1, bOut? "fta" : "Th1");
	m_Grid.SetTextMatrix(0, 2, bOut? "ftmax" : "Th2");
	m_Grid.SetTextMatrix(0, 3, "필요철근량");
	m_Grid.SetTextMatrix(0, 4, "사용철근량");
	m_Grid.SetTextMatrix(0, 5, "비 고");
	
	if(m_pBri->IsBoxType() == FALSE)
	{
		m_Grid.SetTextMatrix(1, 0, "시 점");
		m_Grid.SetTextMatrix(2, 0, "종 점");
	}
	else
	{
		m_Grid.SetTextMatrix(1, 0, "상부 시점");
		m_Grid.SetTextMatrix(2, 0, "상부 종점");
		m_Grid.SetTextMatrix(3, 0, "하부 시점");
		m_Grid.SetTextMatrix(4, 0, "하부 종점");
	}
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputCheckGrid::SetDataInit_Corner()
{
	long nCol = 0;
	CString szOK = _T("");
	BOOL bOut = (m_nTypeCheck==0) ? TRUE : FALSE;

	double Fta, Ftmax, AsReq, AsUse;
	BOOL bAdd, bOK;

	long nRow = 1;
	for(long nUD=iUPPER; nUD<=iLOWER; nUD++)
	{
		if(m_pBri->IsBoxType() == FALSE && nUD==iLOWER) 
			break;

		CARcBridgeCalcCornerAndSupport *pCorner = m_pCalc->m_pCorner[nUD];
		for(long n=0; n<2; n++)
		{
			if(bOut)
			{
				Fta = pCorner->m_dFtaUnitApply[n];
				Ftmax = pCorner->m_dFtmax[n];
				AsReq = pCorner->m_dAsReq[n];
				AsUse = pCorner->m_dAsUsed[n];
				bAdd = (Ftmax > Fta)? TRUE:FALSE;
				bOK = (AsReq < AsUse)? TRUE:FALSE;
			}
			else
			{
				// 단부 스트럽철근 사용 철근량
				double dDiaStrup = m_pBri->m_dDiaStirrupAtDanBuRebar[nUD][n];		// 단부 스터럽 철근 직경 (시종점)
				double dCtcStrup = m_pBri->m_dCTC_MainRebar/2 * m_pBri->m_nCountStirrupCtcMainRebar[nUD][n];
				long nCntStrup = m_pBri->m_nCountStirrupAtDanBuRebar[nUD][n];		// 단부 스터럽 철근 개수 (시종점)
				double dAsUse = GetRebarArea_Fix(dDiaStrup, nCntStrup * (frM(UNIT_METER)/dCtcStrup));

				Fta = pCorner->m_dTh1[n];
				Ftmax = pCorner->m_dTh2[n];
				AsReq = pCorner->m_dAs_[n];
				AsUse = (m_pBri->m_pARcBridgeDataStd->IsSI())? frCm2(dAsUse) : dAsUse;
				bAdd = (AsReq > 0)? TRUE:FALSE;
				bOK = (AsReq < AsUse)? TRUE:FALSE;
			}

			if(bAdd)
			{
				szOK = (bOK)? "O.K":"N.G";
			}
			else
			{
				szOK = "보강 불필요";
			}

			nCol = 1;
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f", Fta);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f", Ftmax);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f", AsReq);
			m_Grid.SetTextMatrixFormat(nRow,nCol++,"%.3f", AsUse);
			m_Grid.SetTextMatrix(nRow,nCol, szOK);
			if(!bOK && bAdd) 
			{
				m_Grid.SetItemFgColour(nRow, nCol++, RGB(255, 0, 0));
			}
			else 
			{
				m_Grid.SetItemFgColour(nRow, nCol++, RGB(0, 0, 0));
			}
			nRow++;
		}
	}
}

void CInputCheckGrid::SetGridTitle_Section()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pData->IsLsdDesign();
	if (bLsd)
	{
		SetGridTitle_Section_Lsd();
		return;
	}
	
	long nSizeInWall = 0;
	long nSizeList = m_pCalc->m_pColGir->m_pList.GetSize();
	long i = 0; for(i = 0; i<nSizeList; i++)
	{
		CGirderAndColumn *pGirCol = m_pCalc->m_pColGir->m_pList.GetAt(i);
		if(pGirCol->m_nJijum == 0 || pGirCol->m_nJijum == nSizeList - 1) continue; //#ARCBRIDGE-3568
		if(!pGirCol->m_bWall) continue;
		nSizeInWall++;
	}

	long nTab = m_ctlTab.GetCurSel();
	long nSizeSlab = m_pBri->m_pArrConcSlab[eEnvLimit].GetSize();
//	long nSizeWall = m_pBri->m_pArrConcWall.GetSize()-4;
	long nSizeWall = MIN(6, m_pBri->m_pArrConcWall[eEnvLimit].GetSize());
	if(m_pBri->m_nSelectInWallSectionDesign == 0)
	{
		nSizeInWall = MIN(m_pBri->GetCountInWall() * 3, m_pBri->m_pArrConcInWall[eEnvLimit].GetSize());
	}
	long nSizeArchRib = m_pBri->m_pArrConcArchRib.GetSize();
	long nSizeFoot = m_pBri->m_pArrConcFoot[eEnvLimit].GetSize() + (nTab == 0 ? m_pBri->m_pArrConcFootRev[eEnvLimit].GetSize() : 0 ); // (ARCBRIDGE-3660) 강도설계법일때는 수평철근 검토 반대방향 출력 안함.
	long nSizeAps = (m_pBri->IsAps(TRUE, TRUE)? 1:0) + (m_pBri->IsAps(TRUE, FALSE)? 1:0) + (m_pBri->IsAps(FALSE, TRUE)? 1:0) + (m_pBri->IsAps(FALSE, FALSE)? 1:0);
	long nSizeRow = 2 + nSizeSlab + nSizeWall + nSizeInWall + nSizeFoot + nSizeArchRib + nSizeAps;
	long nSizeCol = 0;
	CString str = _T("");
	
	switch(nTab)
	{
		case 0:	nSizeCol = 15; break;
		case 1:	nSizeCol = 11; break;
	}
	
	m_Grid.SetColumnCount(nSizeCol);
	m_Grid.SetRowCount(nSizeRow);
	m_Grid.SetFixedRowCount(2);
	m_Grid.SetFixedColumnCount(2);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(70);
	m_Grid.SetColumnWidth(1,140);
	
	m_Grid.SetTextMatrix(0,0,"구 분");
	m_Grid.SetMergeCol(0,0,1);
	m_Grid.SetMergeCol(1,0,1);
	m_Grid.SetMergeRow(0,1,0);
	m_Grid.SetMergeRow(0,1,1);

	m_Grid.SetTextMatrix(2,0,"슬래브");
	m_Grid.SetMergeRow(2,1+nSizeSlab,0);
	m_Grid.SetTextMatrix(2+nSizeSlab,0,"벽  체");
	m_Grid.SetMergeRow(2+nSizeSlab,1+nSizeSlab+nSizeWall,0);
	if(nSizeInWall > 0)
	{
		m_Grid.SetTextMatrix(2+nSizeSlab+nSizeWall,0,"중간벽체");
		m_Grid.SetMergeRow(2+nSizeSlab+nSizeWall,1+nSizeSlab+nSizeWall+nSizeInWall,0);
	}
	if(m_pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		m_Grid.SetTextMatrix(2+nSizeSlab+nSizeWall+nSizeInWall,0,"아치리브");
		m_Grid.SetMergeRow(2+nSizeSlab+nSizeWall+nSizeInWall,1+nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib,0);
	}
	if(nSizeFoot > 0)
	{
		m_Grid.SetTextMatrix(2+nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib,0,"기  초");
		m_Grid.SetMergeRow(2+nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib,1+nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib+nSizeFoot,0);
	}
	if(m_pBri->IsAnyAps())
	{
		m_Grid.SetTextMatrix(2+nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib+nSizeFoot,0,"접속슬래브");
		m_Grid.SetMergeRow(2+nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib+nSizeFoot,1+nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib+nSizeFoot+nSizeAps,0);
	}
	
	if(nTab == 0)
	{
		m_Grid.SetTextMatrix(0, 2, "설계전단력");
		m_Grid.SetMergeCol(0, 2, 5);
		m_Grid.SetTextMatrix(0, 6, "최대전단강도");
		m_Grid.SetMergeCol(0, 6, 8);
		m_Grid.SetTextMatrix(0, 9, "전단철근 간격");
		m_Grid.SetMergeCol(0, 9, 11);
		m_Grid.SetTextMatrix(0, 12, "최소전단철근");
		m_Grid.SetMergeCol(0, 12, 14);

		m_Grid.SetTextMatrix(1, 2, "φVn");
		m_Grid.SetTextMatrix(1, 3, "φVc");
		m_Grid.SetTextMatrix(1, 4, "Vu");
		m_Grid.SetTextMatrix(1, 5, "비 고");
		m_Grid.SetTextMatrix(1, 6, "Vs_max");
		m_Grid.SetTextMatrix(1, 7, "Vs");
		m_Grid.SetTextMatrix(1, 8, "비 고");
		m_Grid.SetTextMatrix(1, 9, "S_max");
		m_Grid.SetTextMatrix(1, 10, "S");
		m_Grid.SetTextMatrix(1, 11, "비 고");
		m_Grid.SetTextMatrix(1, 12, "Req Av");
		m_Grid.SetTextMatrix(1, 13, "Use Av");
		m_Grid.SetTextMatrix(1, 14, "비 고");
	}
	else if(nTab == 1)
	{
		m_Grid.SetTextMatrix(0, 2, "주철근비/0.0015hb");
		m_Grid.SetMergeCol(0, 2, 3);
		m_Grid.SetTextMatrix(0, 4, "주철근비/주철근의 20%");
		m_Grid.SetMergeCol(0, 4, 5);
		m_Grid.SetTextMatrix(0, 6, "단면적비");
		m_Grid.SetMergeCol(0, 6, 7);
		m_Grid.SetTextMatrix(0, 8, "300㎟이상");
		m_Grid.SetMergeCol(0, 8, 9);
		m_Grid.SetTextMatrix(0, 10, "비고");
		m_Grid.SetMergeRow(0, 1, 10);
		
		m_Grid.SetTextMatrix(1, 2, "필요철근량");
		m_Grid.SetTextMatrix(1, 3, "사용철근량");
		m_Grid.SetTextMatrix(1, 4, "필요철근량");
		m_Grid.SetTextMatrix(1, 5, "사용철근량");
		m_Grid.SetTextMatrix(1, 6, "필요철근량");
		m_Grid.SetTextMatrix(1, 7, "사용철근량");	
		m_Grid.SetTextMatrix(1, 8, "필요철근량");
		m_Grid.SetTextMatrix(1, 9, "사용철근량");	
	}
	
	long n=0; for(n=0; n<nSizeRow-2; n++)
	{
		CConcBasic *pConc;
		if(n >= nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib+nSizeFoot)
		{
			long nIdxSub = n-(nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib+nSizeFoot);
			
			if(nSizeAps == 1) str = _T("접속슬래브");
			else if(nIdxSub == 0) str = (m_pCalc->m_pApsStt[1])? _T("시점 좌측 접속슬래브") : _T("시점 접속슬래브");
			else if(nIdxSub == 1) str = (m_pCalc->m_pApsStt[1])? _T("시점 우측 접속슬래브") : ((m_pCalc->m_pApsEnd[1])? _T("종점 좌측 접속슬래브") : _T("종점 접속슬래브"));
			else if(nIdxSub == 2) str = (m_pCalc->m_pApsStt[1])? ((m_pCalc->m_pApsEnd[1])? _T("종점 좌측 접속슬래브") : _T("종점 접속슬래브")) : _T("종점 우측 접속슬래브");
			else if(nIdxSub == 3) str = _T("종점 우측 접속슬래브");
		}
		else if(n >= nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib)
		{
			CString strSub(_T(""));
			long nIdxSub = n-(nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib);
			// (ARCBRIDGE-3660) 전단철근은 상하면을 따로 검토하고 수평철근은 하나만 검토한다.
			if(nTab == 0)
			{
				if(nIdxSub%2==0)
				{
					pConc = m_pBri->m_pArrConcFoot[eEnvLimit].GetAt((long)(nIdxSub/2));
					if(pConc->m_sTitle.Right(6) == "(앞굽)") strSub = _T("-하면인장");
					else strSub = _T("-상면인장");
				}
				else
				{
					pConc = m_pBri->m_pArrConcFootRev[eEnvLimit].GetAt((long)(nIdxSub/2));
					if(pConc->m_sTitle.Right(6) == "(앞굽)") strSub = _T("-상면인장");
					else strSub = _T("-하면인장");
				}

			}
			else
			{
				pConc = m_pBri->m_pArrConcFoot[eEnvLimit].GetAt(nIdxSub);
			}
			str = pConc->m_sTitle + strSub;
		}
		else if(n >= nSizeSlab+nSizeWall+nSizeInWall)
		{
			pConc = m_pBri->m_pArrConcArchRib.GetAt(n-(nSizeSlab+nSizeWall+nSizeInWall));
			str = pConc->m_sTitle;
		}
		else if(n >= nSizeSlab+nSizeWall)
		{
			if(m_pBri->m_nSelectInWallSectionDesign == 0)
			{
				pConc = m_pBri->m_pArrConcInWall[eEnvLimit].GetAt(n-(nSizeSlab+nSizeWall));
			}
			else
			{
				long nIndex = 0;
				long i = 0; for(i = 0; i<nSizeList; i++)
				{
					CGirderAndColumn *pGirCol = m_pCalc->m_pColGir->m_pList.GetAt(i);
					if(pGirCol->m_nJijum == 0 || pGirCol->m_nJijum == nSizeList - 1) continue; //#ARCBRIDGE-3568
					if(!pGirCol->m_bWall) continue;
					if(nIndex == n-(nSizeSlab+nSizeWall))
					{
						pConc = pGirCol->pConcColumn[0];
						break;
					}
					nIndex++;
				}
			}
			str = pConc->m_sTitle;
		}
		else if(n >= nSizeSlab)
		{
			pConc = m_pBri->GetConcDataWall(n-(nSizeSlab), FALSE);
			CString strTemp = _T("");
			strTemp = pConc->m_sTitle;
			if(strTemp.Right(1) == ")") str = strTemp.Left(13)+"부";
			else str = strTemp;
		}
		else
		{
			pConc = m_pBri->m_pArrConcSlab[eEnvLimit].GetAt(n);
			str = pConc->m_sTitle;
		}
		
		m_Grid.SetTextMatrix(n+2, 1, str);
	}
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputCheckGrid::SetDataInit_Section()
{
	long nTab = m_ctlTab.GetCurSel();
	
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pData->IsLsdDesign();

	if (bLsd)
	{
		if(nTab == 0) SetDataInit_Section_Shear_Lsd();
		else if(nTab == 1) SetDataInit_Section_Support_Lsd();
	}
	else
	{
		if(nTab == 0) SetDataInit_Section_Shear();
		else if(nTab == 1) SetDataInit_Section_Support();
	}
}

void CInputCheckGrid::SetDataInit_Section_Shear()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	
	long nSizeInWall = 0;
	long nSizeList = m_pCalc->m_pColGir->m_pList.GetSize();
	long i = 0; for(i = 0; i<nSizeList; i++)
	{
		CGirderAndColumn *pGirCol = m_pCalc->m_pColGir->m_pList.GetAt(i);
		if(pGirCol->m_nJijum == 0 || pGirCol->m_nJijum == nSizeList - 1) continue; //#ARCBRIDGE-3568
		if(!pGirCol->m_bWall) continue;
		nSizeInWall++;
	}

	
	long nSizeSlab = m_pBri->m_pArrConcSlab[eEnvLimit].GetSize();
	long nSizeWall = MIN(6, m_pBri->m_pArrConcWall[eEnvLimit].GetSize());
	if(m_pBri->m_nSelectInWallSectionDesign == 0)
	{
		nSizeInWall = MIN(m_pBri->GetCountInWall() * 3, m_pBri->m_pArrConcInWall[eEnvLimit].GetSize());
	}
	long nSizeArchRib = m_pBri->m_pArrConcArchRib.GetSize();
	long nSizeFoot = m_pBri->m_pArrConcFoot[eEnvLimit].GetSize()+m_pBri->m_pArrConcFootRev[eEnvLimit].GetSize();
	long nSizeAps = (m_pBri->IsAps(TRUE, TRUE)? 1:0) + (m_pBri->IsAps(TRUE, FALSE)? 1:0) + (m_pBri->IsAps(FALSE, TRUE)? 1:0) + (m_pBri->IsAps(FALSE, FALSE)? 1:0);
	long nSizeRow = nSizeSlab + nSizeWall + nSizeInWall + nSizeFoot + nSizeArchRib + nSizeAps;
	CString szOK = _T("");
	
	BOOL bCalcShear = FALSE;
	long n=0; for(n=0; n<nSizeRow; n++)
	{
		long nCol = 0;
		nCol = 2;
		BOOL bWall = FALSE;
		BOOL bCheckMinAv = FALSE;
		BOOL bIsAps = FALSE;
		CConcBasic *pConc = NULL;
		if(n >= nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib + nSizeFoot)
		{
			long nIdxSub = n-(nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib+nSizeFoot);
			if(nIdxSub == 0) pConc = m_pCalc->m_pApsStt[0]->m_pConc;
			else if(nIdxSub == 1) pConc = (m_pCalc->m_pApsStt[1])? m_pCalc->m_pApsStt[1]->m_pConc : m_pCalc->m_pApsEnd[0]->m_pConc;
			else if(nIdxSub == 2) pConc = (m_pCalc->m_pApsStt[1])? m_pCalc->m_pApsEnd[0]->m_pConc : m_pCalc->m_pApsEnd[1]->m_pConc;
			else if(nIdxSub == 3) pConc = m_pCalc->m_pApsEnd[1]->m_pConc;

			bCheckMinAv = m_pBri->m_bApplyMinAvAps;
			bIsAps = TRUE;
			bCalcShear = (pConc->m_SvDia==0 && pConc->m_SvCtc==0 && pConc->m_SvEa==0)? FALSE : TRUE;
		}
		else if(n >= nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib)
		{
			long nIdxSub = n-(nSizeSlab+nSizeWall+nSizeInWall+nSizeArchRib);
			bCheckMinAv = m_pBri->m_bApplyMinAvFoot;

			if(nIdxSub%2==0)
			{
				pConc = m_pBri->m_pArrConcFoot[eEnvLimit].GetAt((long)(nIdxSub/2));
			}
			else pConc = m_pBri->m_pArrConcFootRev[eEnvLimit].GetAt((long)(nIdxSub/2));
			bCalcShear = (pConc->m_SvDia==0 && pConc->m_SvCtc==0 && pConc->m_SvEa==0)? FALSE : TRUE;
		}
		else if(n >= nSizeSlab+nSizeWall+nSizeInWall)
		{
			bCheckMinAv = m_pBri->m_bApplyMinAvWall;
			pConc = m_pBri->m_pArrConcArchRib.GetAt(n-(nSizeSlab+nSizeWall+nSizeInWall));
			bCalcShear = (m_pBri->m_RebarShearDetail_UpperSlab.m_nType==TYPE_SHEAR_SPACER)? FALSE : TRUE;

		}
		else if(n >= nSizeSlab+nSizeWall)
		{
			bCheckMinAv = m_pBri->m_bApplyMinAvWall;

			if(m_pBri->m_nSelectInWallSectionDesign == 0)
			{
				pConc = m_pBri->GetConcDataInWall(n-(nSizeSlab+nSizeWall), FALSE);
			}
			else
			{
				long nIndex = 0;
				long i = 0; for(i = 0; i<nSizeList; i++)
				{
					CGirderAndColumn *pGirCol = m_pCalc->m_pColGir->m_pList.GetAt(i);
					if(pGirCol->m_nJijum == 0 || pGirCol->m_nJijum == nSizeList - 1) continue; //#ARCBRIDGE-3568
					if(!pGirCol->m_bWall) continue;
					if(nIndex == n-(nSizeSlab+nSizeWall))
					{
						pConc = pGirCol->pConcColumn[0];
						break;
					}
					nIndex++;
				}
			}

			bCalcShear = (pConc->m_SvDia==0 && pConc->m_SvCtc==0 && pConc->m_SvEa==0)? FALSE : TRUE;
		}
		else if(n >= nSizeSlab)
		{
			bCheckMinAv = m_pBri->m_bApplyMinAvWall;
			pConc = m_pBri->GetConcDataWall(n-(nSizeSlab), FALSE);
			bCalcShear = (pConc->m_SvDia==0 && pConc->m_SvCtc==0 && pConc->m_SvEa==0)? FALSE : TRUE;
			bWall = TRUE;
		}
		else
		{
			bCheckMinAv = m_pBri->m_bApplyMinAvSlab;
			pConc = m_pBri->m_pArrConcSlab[eEnvLimit].GetAt(n);
			bCalcShear = (m_pBri->m_RebarShearDetail_UpperSlab.m_nType==TYPE_SHEAR_SPACER)? FALSE : TRUE;
			if(m_pBri->IsBoxType() && n >= nSizeSlab/2)
			{
				bCalcShear = (m_pBri->m_RebarShearDetail_LowerSlab.m_nType==TYPE_SHEAR_SPACER)? FALSE : TRUE;
			}
		}
		
		double PiVn = 0;
		double PiVc = 0;		// Vc는 있는데 PiVc는 없네...
		double Vu = 0;
		BOOL bOkShear = FALSE;
		double Vsmax = 0;
		double Vs   = 0;
		BOOL bOkMaxShear = FALSE;
		long nCheckShearForce =  0;
		double ReqAv = 0;
		double UseAv = 0;
		double Smax = 0;
		
		if(bWall && m_pBri->m_nSelectWallSectionDesign==1)
		{
			CColumn pWall(pConc);
			pWall.CalculateAll(FALSE);

			PiVn = pWall.GetPiVn();
			PiVc = pConc->m_Pi_V * pWall.GetVc();		// Vc는 있는데 PiVc는 없네...
			Vu = pWall.GetVu();
			bOkShear = (PiVn > Vu)? TRUE:FALSE;
			szOK = (bOkShear)? "O.K":"N.G";

			Vsmax = pWall.GetVsMax();
			Vs   = pWall.GetVs();
			bOkMaxShear = (Vsmax > Vs)? TRUE:FALSE;

			Smax = pWall.GetsMax();
			nCheckShearForce =  pWall.GetCheckShearForce();
			ReqAv = pWall.GetMinAv();
			UseAv = pWall.GetAvUse();
		}
		else
		{
			CRectBeam pBeam(pConc, m_pBri->m_bCheckMainSlabAndFootTemper);
			
			PiVn = pBeam.GetPiVn();
			PiVc = pConc->m_Pi_V * pBeam.GetVc();		// Vc는 있는데 PiVc는 없네...
			Vu = pBeam.GetVu();
			bOkShear = (PiVn > Vu)? TRUE:FALSE;
			szOK = (bOkShear)? "O.K":"N.G";
			
			Vsmax = pBeam.GetVsMax();
			Vs   = pBeam.GetVs();
			bOkMaxShear = (Vsmax > Vs)? TRUE:FALSE;

			Smax = pBeam.GetsMax();
			nCheckShearForce =  pBeam.GetCheckShearForce();
			ReqAv = pBeam.GetMinAv();
			UseAv = pBeam.GetAvUse();
		}
		
		// 설계전단력은 항시 출력이 되어야 함..
		m_Grid.SetTextMatrixFormat(n+2,nCol++,"%.3f", PiVn);
		m_Grid.SetTextMatrixFormat(n+2,nCol++,"%.3f", PiVc);
		m_Grid.SetTextMatrixFormat(n+2,nCol++,"%.3f", Vu);
		m_Grid.SetTextMatrix(n+2, nCol, szOK);
		if(!bOkShear) 
			m_Grid.SetItemFgColour(n+2, nCol++, RGB(255, 0, 0));
		else 
			m_Grid.SetItemFgColour(n+2, nCol++, RGB(0, 0, 0));
		
		szOK = (bOkMaxShear)? "O.K":"N.G";
		
		// CColumn의 nCheckShearForce리턴값이 CRectBeam의 리턴값과 다르다,,
		BOOL bCheckShearForce = (bWall && m_pBri->m_nSelectWallSectionDesign==1) ? nCheckShearForce != 7 : nCheckShearForce != 6;
		if(bCheckShearForce && bCalcShear && fabs(Vu) > 0)
		{
			m_Grid.SetTextMatrixFormat(n+2,nCol++,"%.3f", Vsmax);
			m_Grid.SetTextMatrixFormat(n+2,nCol++,"%.3f", Vs);
			m_Grid.SetTextMatrix(n+2,nCol, szOK);
			if(!bOkMaxShear) m_Grid.SetItemFgColour(n+2, nCol++, RGB(255, 0, 0));
			else m_Grid.SetItemFgColour(n+2, nCol++, RGB(0, 0, 0));
			
			double S    = pData->IsSI() ? pConc->m_SvCtc : toCm(pConc->m_SvCtc);
			BOOL bOkShearCtc = (Smax >= S)? TRUE:FALSE;
			szOK = (bOkShearCtc)? "O.K":"N.G";
			
			if(!bIsAps || m_pBri->m_bCheckMinCtcShearAps)
			{
				m_Grid.SetTextMatrixFormat(n+2,nCol++,"%.3f", Smax);
				m_Grid.SetTextMatrixFormat(n+2,nCol++,"%.3f", S);
				m_Grid.SetTextMatrix(n+2,nCol, szOK);
				if(!bOkShearCtc) m_Grid.SetItemFgColour(n+2, nCol++, RGB(255, 0, 0));
				else m_Grid.SetItemFgColour(n+2, nCol++, RGB(0, 0, 0));
			}
			
			if(bCheckMinAv)
			{
				BOOL bOkMinShear = (ReqAv < UseAv)? TRUE:FALSE;
				szOK = (bOkMinShear)? "O.K":"N.G";
				
				m_Grid.SetTextMatrixFormat(n+2,nCol++,"%.3f", ReqAv);
				m_Grid.SetTextMatrixFormat(n+2,nCol++,"%.3f", UseAv);
				m_Grid.SetTextMatrix(n+2,nCol, szOK);
				if(!bOkMinShear) m_Grid.SetItemFgColour(n+2, nCol++, RGB(255, 0, 0));
				else m_Grid.SetItemFgColour(n+2, nCol++, RGB(0, 0, 0));
			}
		}
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputCheckGrid::SetDataInit_Section_Support()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;

	long nSizeInWall = 0;
	long nSizeList = m_pCalc->m_pColGir->m_pList.GetSize();
	long i = 0; for(i = 0; i<nSizeList; i++)
	{
		CGirderAndColumn *pGirCol = m_pCalc->m_pColGir->m_pList.GetAt(i);
		if(pGirCol->m_nJijum == 0 || pGirCol->m_nJijum == nSizeList - 1) continue; //#ARCBRIDGE-3568
		if(!pGirCol->m_bWall) continue;
		nSizeInWall++;
	}
	
	long nSizeSlab = (m_pBri->IsBoxType())? m_pBri->m_pArrConcSlab[eEnvLimit].GetSize()/2 : m_pBri->m_pArrConcSlab[eEnvLimit].GetSize();
	long nSizeLowerSlab = (m_pBri->IsBoxType())? nSizeSlab : 0;
	long nSizeWall = MIN(6, m_pBri->m_pArrConcWall[eEnvLimit].GetSize());
	if(m_pBri->m_nSelectInWallSectionDesign == 0)
	{
		nSizeInWall = MIN(6, m_pBri->m_pArrConcInWall[eEnvLimit].GetSize());
	}
	long nSizeArchRib = m_pBri->m_pArrConcArchRib.GetSize();
	long nSizeFoot = (m_pBri->IsBoxType())? 0 : m_pBri->m_pArrConcFoot[eEnvLimit].GetSize(); // (ARCBRIDGE-3660) 강도설계법일때는 반대방향 검토안함 + m_pBri->m_pArrConcFootRev[eEnvLimit].GetSize();
	long nSizeAps = (m_pBri->IsAps(TRUE, TRUE)? 1:0) + (m_pBri->IsAps(TRUE, FALSE)? 1:0) + (m_pBri->IsAps(FALSE, TRUE)? 1:0) + (m_pBri->IsAps(FALSE, FALSE)? 1:0);
	long nSizeRow = nSizeSlab + nSizeLowerSlab + nSizeWall + nSizeInWall + nSizeFoot + nSizeArchRib + nSizeAps;
	CString szOK = _T("");
	
	long n=0; for(n=0; n<nSizeRow; n++)
	{
		long nCol = 0;
		nCol = 2;
		long nType = 0;					// 0: Slab, 1: Wall, 2: Foot
		BOOL bCheckMain = FALSE;
		BOOL bCheckMainOver20 = FALSE;
		BOOL bCheckSectA = FALSE;
		BOOL bCheckOver300 = FALSE;
		CConcBasic *pConc;

		if(n >= nSizeSlab+nSizeLowerSlab+nSizeWall+nSizeInWall+nSizeArchRib+nSizeFoot)
		{
			long nIdxSub = n-(nSizeSlab + nSizeLowerSlab +nSizeWall+nSizeInWall+nSizeArchRib+nSizeFoot);
			if(nIdxSub == 0) pConc = m_pCalc->m_pApsStt[0]->m_pConc;
			else if(nIdxSub == 1) pConc = (m_pCalc->m_pApsStt[1])? m_pCalc->m_pApsStt[1]->m_pConc : m_pCalc->m_pApsEnd[0]->m_pConc;
			else if(nIdxSub == 2) pConc = (m_pCalc->m_pApsStt[1])? m_pCalc->m_pApsEnd[0]->m_pConc : m_pCalc->m_pApsEnd[1]->m_pConc;
			else if(nIdxSub == 3) pConc = m_pCalc->m_pApsEnd[1]->m_pConc;

			nType = 3;
			bCheckMain = m_pBri->m_bCheckSuppoerBarSlabMain;
			bCheckMainOver20 = FALSE;
			bCheckSectA = m_pBri->m_bCheckSuppoerBarSlabArea;
			bCheckOver300 = m_pBri->m_bCheckSuppoerBarSlab300;
		}
		else if(n >= nSizeSlab+nSizeLowerSlab+nSizeWall+nSizeInWall+nSizeArchRib)
		{
			nType = 2;
			bCheckMain = m_pBri->m_bCheckSuppoerBarFoot15hs;
			bCheckMainOver20 = m_pBri->m_bCheckSuppoerBarFootMainOver20;
			bCheckSectA = m_pBri->m_bCheckSuppoerBarFootArea;
			bCheckOver300 = m_pBri->m_bCheckSuppoerBarFoot300;

			long nIdxSub = n-(nSizeSlab + nSizeLowerSlab +nSizeWall+nSizeInWall+nSizeArchRib);
			pConc = m_pBri->m_pArrConcFoot[eEnvLimit].GetAt(nIdxSub);
// 			if(nIdxSub%2==0)
// 			{
// 				pConc = m_pBri->m_pArrConcFoot[eEnvLimit].GetAt((long)(nIdxSub/2));
// 			}
// 			else pConc = m_pBri->m_pArrConcFootRev[eEnvLimit].GetAt((long)(nIdxSub/2));
		}
		else if(n >= nSizeSlab+nSizeLowerSlab+nSizeWall+nSizeInWall)
		{
			nType = 1;
			bCheckMain = m_pBri->m_bCheckSuppoerBarWall15hs;
			bCheckMainOver20 = m_pBri->m_bCheckSuppoerBarWallMainOver20;
			bCheckSectA = m_pBri->m_bCheckSuppoerBarWallArea;
			bCheckOver300 = m_pBri->m_bCheckSuppoerBarWall300;
			pConc = m_pBri->m_pArrConcArchRib.GetAt(n-(nSizeSlab + nSizeLowerSlab +nSizeWall+nSizeInWall));
		}
		else if(n >= nSizeSlab+nSizeLowerSlab+nSizeWall)
		{
			nType = 1;
			bCheckMain = m_pBri->m_bCheckSuppoerBarWall15hs;
			bCheckMainOver20 = m_pBri->m_bCheckSuppoerBarWallMainOver20;
			bCheckSectA = m_pBri->m_bCheckSuppoerBarWallArea;
			bCheckOver300 = m_pBri->m_bCheckSuppoerBarWall300;

			if(m_pBri->m_nSelectInWallSectionDesign == 0)
			{
				pConc = m_pBri->GetConcDataInWall(n-(nSizeSlab + nSizeLowerSlab +nSizeWall), FALSE, eEnvLimit);
			}
			else
			{
				long nIndex = 0;
				long i = 0; for(i = 0; i<nSizeList; i++)
				{
					CGirderAndColumn *pGirCol = m_pCalc->m_pColGir->m_pList.GetAt(i);
					if(pGirCol->m_nJijum == 0 || pGirCol->m_nJijum == nSizeList - 1) continue; //#ARCBRIDGE-3568
					if(!pGirCol->m_bWall) continue;
					if(nIndex == n-(nSizeSlab + nSizeLowerSlab +nSizeWall))
					{
						pConc = pGirCol->pConcColumn[0];
						break;
					}
					nIndex++;
				}
			}
		}
		else if(n >= nSizeSlab + nSizeLowerSlab)
		{
			nType = 1;
			bCheckMain = m_pBri->m_bCheckSuppoerBarWall15hs;
			bCheckMainOver20 = m_pBri->m_bCheckSuppoerBarWallMainOver20;
			bCheckSectA = m_pBri->m_bCheckSuppoerBarWallArea;
			bCheckOver300 = m_pBri->m_bCheckSuppoerBarWall300;
			pConc = m_pBri->GetConcDataWall(n-(nSizeSlab + nSizeLowerSlab), FALSE, eEnvLimit, TRUE);
		}
		else if(n >= nSizeSlab)
		{
			nType = 2;
			bCheckMain = m_pBri->m_bCheckSuppoerBarFoot15hs;
			bCheckMainOver20 = m_pBri->m_bCheckSuppoerBarFootMainOver20;
			bCheckSectA = m_pBri->m_bCheckSuppoerBarFootArea;
			bCheckOver300 = m_pBri->m_bCheckSuppoerBarFoot300;
			pConc = m_pBri->m_pArrConcSlab[eEnvLimit].GetAt(n);
		}
		else
		{
			nType = 0;
			bCheckMain = m_pBri->m_bCheckSuppoerBarSlabMain;
			bCheckMainOver20 = m_pBri->m_bCheckSuppoerBarSlabMainOver20;
			bCheckSectA = m_pBri->m_bCheckSuppoerBarSlabArea;
			bCheckOver300 = m_pBri->m_bCheckSuppoerBarSlab300;
			pConc = m_pBri->m_pArrConcSlab[eEnvLimit].GetAt(n);
		}
		
		CRectBeam pBeam(pConc, m_pBri->m_bCheckMainSlabAndFootTemper);
		
		double AsReq = 0;
		double AsUse = 0;
		double dLenSpan = 0;
		CString szOK = _T("");
		
		if(nType == 0)
		{
			long nCheck = static_cast<long>(n - nSizeSlab / 2.);
			long nSizeSlabCheck = static_cast<long>(m_pBri->IsBoxType() ? nSizeSlab / 2. : nSizeSlab);
			if(nCheck < 2) dLenSpan = m_pBri->GetLengthModelSpan(0);
			else if(nCheck > nSizeSlabCheck-3) dLenSpan = m_pBri->GetLengthModelSpan(m_pBri->GetCountJijum()-2);
			else if(nCheck%2 != 0) dLenSpan = m_pBri->GetLengthModelSpan((nCheck-1)/2);
			else if(nCheck%2 == 0) dLenSpan = min(m_pBri->GetLengthModelSpan(nCheck/2 - 1), m_pBri->GetLengthModelSpan(nCheck/2));
		}
		else if(nType == 3)
		{
			BOOL bStt = (n==nSizeSlab+nSizeWall+nSizeArchRib+nSizeFoot); 
			dLenSpan = bStt? (m_pBri->m_apsStt[0].m_dL1 + m_pBri->m_apsStt[0].m_dL2 + m_pBri->m_apsStt[0].m_dL3 )
				: (m_pBri->m_apsEnd[0].m_dL1 + m_pBri->m_apsEnd[0].m_dL2 + m_pBri->m_apsEnd[0].m_dL3);
		}

		long i=0; for(i=0; i<4; i++)
		{
			if(i == 0 && !bCheckMain) { nCol += 2; continue;}
			if(i == 1 && !bCheckMainOver20) { nCol += 2; continue;}
			if(i == 2 && !bCheckSectA) { nCol += 2; continue;}
			if(i == 3 && !bCheckOver300) { nCol += 2; continue;}
			
			if(nType == 0)
			{
				AsReq = m_pCalc->m_pCorner[iUPPER]->GetDoubleCheckConcSlab(pConc, TRUE, i, dLenSpan);
				AsUse = m_pCalc->m_pCorner[iUPPER]->GetDoubleCheckConcSlab(pConc, FALSE, i, dLenSpan);
			}
			else if(nType == 1)
			{
				AsReq = m_pCalc->m_pCorner[iUPPER]->GetDoubleCheckConcWall(pConc, TRUE, i,FALSE);
				AsUse = m_pCalc->m_pCorner[iUPPER]->GetDoubleCheckConcWall(pConc, FALSE, i,FALSE);
			}
			else if(nType == 2)
			{
				AsReq = m_pCalc->m_pCorner[iUPPER]->GetDoubleCheckConcFooting(pConc, TRUE, i);
				AsUse = m_pCalc->m_pCorner[iUPPER]->GetDoubleCheckConcFooting(pConc, FALSE, i);
			}
			else if(nType == 3)
			{
				AsReq = m_pCalc->m_pCorner[iUPPER]->GetDoubleCheckConcAps(pConc, TRUE, i, dLenSpan);
				AsUse = m_pCalc->m_pCorner[iUPPER]->GetDoubleCheckConcAps(pConc, FALSE, i, dLenSpan);
			}

			if(pData->IsSI())
			{
				m_Grid.SetTextMatrixFormat(n+2,nCol++,"%.1f", AsReq);
				m_Grid.SetTextMatrixFormat(n+2,nCol++,"%.1f", AsUse);
			}
			else
			{
				m_Grid.SetTextMatrixFormat(n+2,nCol++,"%.3f", AsReq);
				m_Grid.SetTextMatrixFormat(n+2,nCol++,"%.3f", AsUse);
			}
			
		}
		
		if(nType == 0) szOK = m_pCalc->m_pCorner[iUPPER]->GetStringCheckConcSlab(pConc, TRUE, 4, dLenSpan, FALSE);
		else if(nType == 1) szOK = m_pCalc->m_pCorner[iUPPER]->GetStringCheckConcWall(pConc, TRUE, 4, FALSE);
		else if(nType == 2) szOK = m_pCalc->m_pCorner[iUPPER]->GetStringCheckConcFooting(pConc, TRUE, 4, FALSE);
		else if(nType == 3) szOK = m_pCalc->m_pCorner[iUPPER]->GetStringCheckConcAps(pConc, TRUE, 4, dLenSpan, FALSE);
		
		m_Grid.SetTextMatrix(n+2, nCol, szOK);
		if(szOK != "O.K") m_Grid.SetItemFgColour(n+2, nCol++, RGB(255, 0, 0));
		else m_Grid.SetItemFgColour(n+2, nCol++, RGB(0, 0, 0));
	}	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputCheckGrid::SetGridTitle_Section_Lsd()
{
	const HGINT32 icValidEnvType = m_pBri->GetCountValidComboEnvType();

	long nCntInWall = (m_pBri->m_nSelectInWallSectionDesign == 0)? m_pBri->GetCountInWall() : 0;
	long nSizeSlab = m_pBri->GetCountConcSlab();
	long nSizeWall = m_pBri->m_nSelectWallSectionDesign==1 ? 0 : ((m_pBri->IsOutsideWall())? 6 * icValidEnvType:0);
	long nSizeInWall = nCntInWall * 3 * icValidEnvType;

	long nTab = m_ctlTab.GetCurSel();
	if(nTab == 0)
	{
		long nSizeFoot = m_pBri->GetCountConcFooting(TRUE, TRUE);
		long nSizeRow = 2 + nSizeSlab + nSizeWall + nSizeInWall + nSizeFoot;
		long nSizeCol = 14;

		m_Grid.SetColumnCount(nSizeCol);
		m_Grid.SetRowCount(nSizeRow);
		m_Grid.SetFixedRowCount(2);
		m_Grid.SetFixedColumnCount(3);
		m_Grid.SetRowHeightAll(20);
		m_Grid.SetColumnWidthAll(70);
		m_Grid.SetColumnWidth(1,100);

		m_Grid.SetTextMatrix(0,0,"구 분");
		m_Grid.SetMergeCol(0,0,2);
		m_Grid.SetMergeCol(1,0,2);
		m_Grid.SetMergeRow(0,1,0);
		m_Grid.SetMergeRow(0,1,1);
		m_Grid.SetMergeRow(0,1,2);

		m_Grid.SetTextMatrix(0, 3, "철근비");
		m_Grid.SetMergeCol(0, 3, 7);
		m_Grid.SetTextMatrix(0, 8, "최소전단철근");
		m_Grid.SetMergeCol(0, 8, 10);
		m_Grid.SetTextMatrix(0, 11, "전단철근 간격");
		m_Grid.SetMergeCol(0, 11, 13);
		
		m_Grid.SetTextMatrix(1, 3, "Vcd");
		m_Grid.SetTextMatrix(1, 4, "Vsd");
		m_Grid.SetTextMatrix(1, 5, "Θ");
		m_Grid.SetTextMatrix(1, 6, "Vu");
		m_Grid.SetTextMatrix(1, 7, "비고");
		m_Grid.SetTextMatrix(1, 8, "pv,min");
		m_Grid.SetTextMatrix(1, 9, "pv,use");
		m_Grid.SetTextMatrix(1, 10, "비고");
		m_Grid.SetTextMatrix(1, 11, "S_max");
		m_Grid.SetTextMatrix(1, 12, "S");
		m_Grid.SetTextMatrix(1, 13, "비고");
	}
	else if(nTab == 1)
	{
		// (ARCBRIDGE-3658) 수평철근검토는 출력 갯수가 다르다.
		nSizeSlab = m_pBri->m_pArrConcSlab[eEnvLimit].GetSize();
		nSizeWall = (m_pBri->IsOutsideWall() && m_pBri->m_nSelectWallSectionDesign==0)? MIN(10, m_pBri->GetCountConcOutWall()):0;
		nSizeInWall = m_pBri->m_nSelectInWallSectionDesign == 0 ? (m_pBri->GetCountConcInWall()/2) : 0;
		
		long nSizeList = m_pCalc->m_pColGir->m_pList.GetSize();
		long i=0; for(i=0; i<nSizeList; i++)
		{
			CGirderAndColumn *pGirCol = m_pCalc->m_pColGir->m_pList.GetAt(i);
			if(!pGirCol->m_bWall) continue;

			CConcBasic *pConc = pGirCol->pConcColumn[0];
			if(pConc == NULL) continue;

			if(pGirCol->m_nJijum == 0 || pGirCol->m_nJijum == m_pBri->m_nQtyJigan)
			{
				if(m_pBri->m_nSelectWallSectionDesign==1)
					nSizeInWall++;			// 설계검토에서는 구성이 달라서 헤깔린다. 그냥 nSizeInWall에 밀어넣자.
			}
			else if(pGirCol->m_bWall) nSizeInWall++;
		}

		BOOL bMainRatio = (m_pBri->m_bCheckSupportSlabMainRate || m_pBri->m_bCheckSupportWallMainRate);
		BOOL bMainRatio_Tra = m_pBri->m_bCheckSupportSlabMainRateSpan;

		long nSizeFoot = 0;
		const HGBOOL bBox = m_pBri->IsBoxType();
		const HGBOOL bSlab = m_pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB;
		const HGBOOL bTrap2 = m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2;
		const HGINT32 icJijum = bSlab || bBox ? 0 : bTrap2 ? m_pBri->GetCountJijum() - 2 : m_pBri->GetCountJijum();
		for (HGINT32 ix = 0; ix < icJijum; ++ix)
		{
			if (m_pBri->IsOutsideWall() == FALSE && (ix == 0 || ix == 1))	continue;

			const HGINT32 ixJijum = ix == 0 ? 0 : ix == 1 ? m_pBri->GetCountJijum() - 1 : ix - 1;
			for (HGINT32 ixPos = 0; ixPos < 2; ++ixPos)
			{
				HGBOOL bFront = ixPos == 0 ? TRUE : FALSE;

				CConcBasic *pConc = m_pBri->GetConcDataFooting(ixJijum, bFront, FALSE);
				if(pConc == NULL)	continue;

				nSizeFoot++;
			}
		}

		long nSizeRow = 2 + nSizeSlab + nSizeWall + nSizeInWall + nSizeFoot;
		long nSizeCol = 8;

		if(bMainRatio) nSizeCol += 3;
		if(bMainRatio_Tra) nSizeCol += 3;
		
		m_Grid.SetColumnCount(nSizeCol);
		m_Grid.SetRowCount(nSizeRow);
		m_Grid.SetFixedRowCount(2);
		m_Grid.SetFixedColumnCount(2);
		m_Grid.SetRowHeightAll(20);
		m_Grid.SetColumnWidthAll(70);
		m_Grid.SetColumnWidth(1,140);

		m_Grid.SetTextMatrix(0,0,"구 분");
		m_Grid.SetMergeCol(0,0,1);
		m_Grid.SetMergeCol(1,0,1);
		m_Grid.SetMergeRow(0,1,0);
		m_Grid.SetMergeRow(0,1,1);

		m_Grid.SetTextMatrix(0, 2, "수평방향");
		m_Grid.SetMergeCol(0, 2, 4);
		m_Grid.SetTextMatrix(0, 5, "수직방향");
		m_Grid.SetMergeCol(0, 5, 7);
		long nCol = 8;
		if(bMainRatio)
		{
			m_Grid.SetTextMatrix(0, nCol, "주철근비");
			m_Grid.SetMergeCol(0, nCol, nCol+2);
			nCol += 3;
		}
		if(bMainRatio_Tra)
		{
			m_Grid.SetTextMatrix(0, nCol, "주철근비(전통적설계법)");
			m_Grid.SetMergeCol(0, nCol, nCol+2);
		}

		nCol = 2;
		m_Grid.SetTextMatrix(1, nCol++, "필요철근량");
		m_Grid.SetTextMatrix(1, nCol++, "사용철근량");
		m_Grid.SetTextMatrix(1, nCol++, "비고");
		m_Grid.SetTextMatrix(1, nCol++, "필요철근량");
		m_Grid.SetTextMatrix(1, nCol++, "사용철근량");
		m_Grid.SetTextMatrix(1, nCol++, "비고");	
		if(bMainRatio)
		{
			m_Grid.SetTextMatrix(1, nCol++, "필요철근량");
			m_Grid.SetTextMatrix(1, nCol++, "사용철근량");
			m_Grid.SetTextMatrix(1, nCol++, "비고");	
		}
		if(bMainRatio_Tra)
		{
			m_Grid.SetTextMatrix(1, nCol++, "필요철근량");
			m_Grid.SetTextMatrix(1, nCol++, "사용철근량");
			m_Grid.SetTextMatrix(1, nCol++, "비고");
		}
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputCheckGrid::SetDataInit_Section_Shear_Lsd()
{
	CRcBridgeRebar *pBri = m_pBri;
	HGINT32 ixRow = 1;
	const HGBOOL bLsd = TRUE;

	const HGINT32 icValidEnvType = m_pBri->GetCountValidComboEnvType();
	long nCntInWall = (m_pBri->m_nSelectInWallSectionDesign == 0)? m_pBri->GetCountInWall() : 0;
	long nSizeSlab = m_pBri->GetCountConcSlab();
	long nSizeWall = m_pBri->m_nSelectWallSectionDesign==1 ? 0 : ((m_pBri->IsOutsideWall())? 6 * icValidEnvType:0);
	long nSizeInWall = nCntInWall * 3 * icValidEnvType;
	
	const HGBOOL bIsOutWall = m_pBri->IsOutsideWall();

	long nCntRow = nSizeSlab + nSizeWall + nSizeInWall;
	for(HGINT32 ix=0; ix<nCntRow; ++ix)
	{
		BOOL bSlab = (ix<nSizeSlab)? TRUE:FALSE;
		BOOL bWall = FALSE;

		CString strPos;
		CString strPos1;
		CString strEnv;
		CConcBasic *pData = NULL;
		if(bSlab)
		{
			pData = pBri->GetConcDataSlab_Lsd(ix, &strEnv);
			strPos = "슬래브";
			if(pBri->IsBoxType())
			{
				strPos = (ix < nSizeSlab/2 ) ? "상부슬래브" : "하부슬래브";
			}

			if(m_pBri->IsBoxType())
			{
				if (ix == 0)
				{
					m_Grid.SetMergeRow(2, 2 + nSizeSlab / 2 - 1, 0);
				}
				else if (ix == 2 + nSizeSlab / 2)
				{
					m_Grid.SetMergeRow(2 + nSizeSlab / 2, 2 + nSizeSlab - 1, 0);
				}
			}
			else
			{
				if (ix == 0)
				{
					m_Grid.SetMergeRow(2, 2 + nSizeSlab - 1, 0);
				}
			}
		}
		else if(ix<(nSizeSlab + nSizeWall))
		{
			pData = pBri->GetConcDataWall_Lsd((ix-nSizeSlab), &strEnv, FALSE);
			bWall = TRUE;

			if(ix-nSizeSlab<3*icValidEnvType && bIsOutWall)
				strPos = "시점벽체";
			else if(ix-nSizeSlab<6*icValidEnvType && bIsOutWall)
				strPos = "종점벽체";

			if (ix == nSizeSlab)
			{
				m_Grid.SetMergeRow(2 + nSizeSlab, 2 + nSizeSlab + nSizeWall / 2 - 1, 0);
			}
			else if (ix == nSizeSlab + nSizeWall / 2)
			{
				m_Grid.SetMergeRow(2 + nSizeSlab + nSizeWall / 2, 2 + nSizeSlab + nSizeWall - 1, 0);
			}
		}
		else if(ix<(nSizeSlab + nSizeWall + nSizeInWall))
		{
			long nIndex = ix-(nSizeSlab + nSizeWall);
			pData = pBri->GetConcDataInWall_Lsd(nIndex, &strEnv);
			bWall = FALSE;

			long nCol = (ix-(nSizeSlab+nSizeWall))/(3*icValidEnvType);
			strPos.Format("중간벽체%d", nCol+1);

			if ((ix - (nSizeSlab + nSizeWall + nSizeInWall)) % (3 * icValidEnvType) == 0)
			{
				m_Grid.SetMergeRow(ixRow+1, ixRow + 3 * icValidEnvType, 0);
			}
		}
		
		if(bSlab) 
		{
			strPos1 = pData->m_sTitle;
			if(pBri->IsBoxType())
			{
				strPos1 = pData->m_sTitle.Right(pData->m_sTitle.GetLength() - 11);
			}
		}
		else
		{
			CString strTemp = _T("");
			if(bWall) 
				strTemp = pData->m_sTitle.Mid(9);
			else 
				strTemp = pData->m_sTitle.Mid(10);
			if(strTemp.Right(1) == ")") 
				strPos1 = strTemp.Left(4) + "부";
			else 
				strPos1 = strTemp;
		}

		if (ix % icValidEnvType == 0)
		{
			m_Grid.SetMergeRow(ixRow + 1, ixRow + icValidEnvType, 1);
		}
		
		SetTextMatrix_Section_Shear(pData, ++ixRow, strPos, strPos1, strEnv);
	}

	const HGBOOL bBox = m_pBri->IsBoxType();
	const HGBOOL bSlab = m_pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB;
	const HGBOOL bTrap2 = m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2;
	const HGINT32 icJijum = bSlab || bBox ? 0 : bTrap2 ? m_pBri->GetCountJijum() - 2 : m_pBri->GetCountJijum();
	for (HGINT32 ix = 0; ix < icJijum; ++ix)
	{
		if (bIsOutWall == FALSE && (ix == 0 || ix == 1))
			continue;

		const HGINT32 ixJijum = ix == 0 ? 0 : ix == 1 ? m_pBri->GetCountJijum() - 1 : ix - 1;

		CConcBasic *pConc = NULL;
		const HGINT32 icPos = GetCountFootPos(ixJijum);
		m_Grid.SetMergeRow(ixRow+1, ixRow + icPos * icValidEnvType, 0);

		for (HGINT32 ixPos = 0; ixPos < icPos; ++ixPos)
		{
			CString strPos, strPos1;
			HGBOOL bFront(TRUE), bUpper(TRUE);
			HGINT32 ixCheckAdd = -1;
			GetFootInfo_Lsd(ixJijum, ixPos, strPos, strPos1, bFront, bUpper, ixCheckAdd);

			m_Grid.SetMergeRow(ixRow+1, ixRow + icValidEnvType, 1);

			for (HGINT32 ixEnvType = eEnvLimit; ixEnvType < eEnvService; ++ixEnvType)
			{
				const ETypeEnvComboLsd eEnvType = static_cast<ETypeEnvComboLsd>(ixEnvType);
				if (m_pBri->IsValidComboEnvType(eEnvType) == FALSE)
					continue;

				CString strEnv = ixEnvType == eEnvLimit ? "극한" : ixEnvType == eEnvExtremeI ? "극단1" : "극단2";
				pConc = m_pBri->GetConcDataFooting(ixJijum, bFront, bUpper == bFront, ixCheckAdd, eEnvType);
				SetTextMatrix_Section_Shear(pConc, ++ixRow, strPos, strPos1, strEnv);
			}
		}
	}
}

void CInputCheckGrid::SetTextMatrix_Section_Shear(CConcBasic *pConc, HGINT32 ixRow, const CString &strPos, const CString &strPos1, const CString &strEnv)
{
	const HGBOOL bLsd = TRUE;

	HGINT32 ixCol = 0;
	m_Grid.SetTextMatrix(ixRow, ixCol++, strPos);
	m_Grid.SetTextMatrix(ixRow, ixCol++, strPos1);
	m_Grid.SetTextMatrix(ixRow, ixCol++, strEnv);

	if (pConc == NULL)
		return;

	CLsdBeam pBeam(pConc, strEnv == "극한" ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN);
	SetDataInit_Shear_Lsd(pBeam, ixRow, ixCol);
}

void CInputCheckGrid::SetDataInit_Section_Support_Lsd()
{
	CRcBridgeRebar *pBri = m_pBri;
	const HGBOOL bLsd = TRUE;
	
	const HGINT32 icValidEnvType = m_pBri->GetCountValidComboEnvType();

	// (ARCBRIDGE-3658) 수평철근 검토는 하나의 조합만 출력한다.
	// long nCntInWall = (m_pBri->m_nSelectInWallSectionDesign == 0)? m_pBri->GetCountInWall() : 0;
	// long nSizeSlab = m_pBri->GetCountConcSlab();
	// long nSizeWall = m_pBri->m_nSelectWallSectionDesign==1 ? 0 : ((m_pBri->IsOutsideWall())? 6 * icValidEnvType:0);
	// long nSizeInWall = nCntInWall * 3 * icValidEnvType;

	// (ARCBRIDGE-3660) LSD일때는 전면배면 단면을 모두 검토하고, 강도설계법일때는 위치별로 하나만. 
	long nSizeSlab = pBri->m_pArrConcSlab[eEnvLimit].GetSize();
	long nSizeWall = (pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign==0)? MIN(10, pBri->m_pArrConcWall[eEnvLimit].GetSize()):0;
	long nSizeInWall = pBri->m_nSelectInWallSectionDesign == 0 ? (pBri->GetCountConcInWall()/2) : 0;
	long nSizeInWall2 = 0;

	long nSizeList = m_pCalc->m_pColGir->m_pList.GetSize();
	long i=0; for(i=0; i<nSizeList; i++)
	{
		CGirderAndColumn *pGirCol = m_pCalc->m_pColGir->m_pList.GetAt(i);
		if(!pGirCol->m_bWall) continue;

		CConcBasic *pConc = pGirCol->pConcColumn[0];
		if(pConc == NULL) continue;

		if(pGirCol->m_nJijum == 0 || pGirCol->m_nJijum == pBri->m_nQtyJigan)
		{
			if(pBri->m_nSelectWallSectionDesign==1)
				nSizeInWall2++;			// 설계검토에서는 구성이 달라서 헤깔린다. 그냥 nSizeInWall에 밀어넣자.
		}
		else if(pGirCol->m_bWall) nSizeInWall2++;
	}

	CString strPos, strEnv, strPos1;

	long nSizeRow = nSizeSlab + nSizeWall + nSizeInWall2 + nSizeInWall;
	HGINT32 ixRow = 1;
	for (HGINT32 ix = 0; ix < nSizeRow; ++ix)
	{
		BOOL bSlab = (ix<nSizeSlab)? TRUE:FALSE;
		BOOL bWall = FALSE;

		CConcBasic *pData = NULL;
		if(bSlab)
		{
			pData = pBri->m_pArrConcSlab[eEnvLimit].GetAt(ix);
			strPos = "슬래브";
			if(pBri->IsBoxType())
			{
				strPos = (ix < nSizeSlab/2 ) ? "상부슬래브" : "하부슬래브";
			}

			if(m_pBri->IsBoxType())
			{
				if (ix == 0)
				{
					m_Grid.SetMergeRow(2, 2 + nSizeSlab / 2 - 1, 0);
				}
				else if (ix == 2 + nSizeSlab / 2)
				{
					m_Grid.SetMergeRow(2 + nSizeSlab / 2, 2 + nSizeSlab - 1, 0);
				}
			}
			else
			{
				if (ix == 0)
				{
					m_Grid.SetMergeRow(2, 2 + nSizeSlab - 1, 0);
				}
			}
		}
		else if(ix<(nSizeSlab + nSizeWall))
		{
			BOOL bSttWall = TRUE;
			if (ix-nSizeSlab<nSizeWall / 2)
			{
				strPos = "시점벽체";
				bSttWall = TRUE;
			}
			else
			{
				strPos = "종점벽체";
				bSttWall = FALSE;
			}

			if(pBri->m_nSelectWallSectionDesign==0)
			{
				pData = pBri->GetConcDataWall(ix-nSizeSlab, TRUE, eEnvLimit, TRUE);
			}
			else
			{
				continue;
			}
			bWall = TRUE;

			if (ix == nSizeSlab)
			{
				m_Grid.SetMergeRow(2 + nSizeSlab, 2 + nSizeSlab + nSizeWall / 2 - 1, 0);
			}
			else if (ix == nSizeSlab + nSizeWall / 2)
			{
				m_Grid.SetMergeRow(2 + nSizeSlab + nSizeWall / 2, 2 + nSizeSlab + nSizeWall - 1, 0);
			}
		}
		else if(ix<(nSizeSlab + nSizeWall + nSizeInWall2))
		{
			long nIndex = ix-(nSizeSlab + nSizeWall);

			CGirderAndColumn *pGirCol = NULL;
			long idxCount(0);
			long i=0; for(i=0; i<nSizeList; i++)
			{
				pGirCol = m_pCalc->m_pColGir->m_pList.GetAt(i);
				if(!pGirCol->m_bWall) continue;

				if(pGirCol->m_nJijum == 0 || pGirCol->m_nJijum == pBri->m_nQtyJigan)
				{
					if(pBri->m_nSelectWallSectionDesign == 0)
						continue;
				}
				else
				{
					if(pBri->m_nSelectInWallSectionDesign == 0)
						continue;
				}

				if(idxCount == nIndex)
					break;
				idxCount++;
			}
			if(pGirCol == NULL || !pGirCol->m_bWall) continue;

			pData = pGirCol->pConcColumnLsd[eEnvLimit][1];
			
			if(pGirCol->m_nJijum == 0)
				strPos = "시점벽체";
			else if(pGirCol->m_nJijum == pBri->m_nQtyJigan)
				strPos = "종점벽체";
			else
				strPos = "내측벽체";

			
			if ((ix - (nSizeSlab + nSizeWall)) == 0)
			{
				m_Grid.SetMergeRow(ixRow+1, ixRow + nSizeInWall2, 0);
			}
		}
		else if(ix<(nSizeSlab + nSizeWall + nSizeInWall2 + nSizeInWall))
		{
			long nIndex = ix-(nSizeSlab + nSizeWall + nSizeInWall2);
			if(pBri->m_nSelectInWallSectionDesign==0)
			{
				pData = pBri->m_pArrConcInWall[eEnvLimit].GetAt(nIndex);
				bWall = FALSE;
			}
		
			long nIdx = pData->m_sTitle.Find(" ");
			if(nIdx > 0)
				strPos = pData->m_sTitle.Left(nIdx);
			else
				strPos = pData->m_sTitle.Left(9);

			if ((ix - (nSizeSlab + nSizeWall + nSizeInWall2 + nSizeInWall)) % 3 == 0)
			{
				m_Grid.SetMergeRow(ixRow+1, ixRow + 3, 0);
			}
		}

		if(bSlab) 
		{
			strPos1 = pData->m_sTitle;
			if(pBri->IsBoxType())
			{
				strPos1 = pData->m_sTitle.Right(pData->m_sTitle.GetLength() - 11);
			}
		}
		else
		{
			CString strTemp = _T("");
			if(bWall) 
				strTemp = pData->m_sTitle.Mid(9);
			else 
				strTemp = pData->m_sTitle.Mid(10);

			// (ARCBRIDGE-3658) LSD 에서는 수평철근 검토를 전면 배면 모두 출력...
// 			if(strTemp.Right(1) == ")") 
// 				strPos1 = strTemp.Left(4) + "부";
// 			else 
				strPos1 = strTemp;
		}

		HGINT32 ixCol = 0;
		m_Grid.SetTextMatrix(++ixRow, ixCol++, strPos);
		m_Grid.SetTextMatrix(ixRow, ixCol++, strPos1);

		if (pData == NULL)
			continue;

		CLsdBeam pBeam(pData, LSD_ULS_DESIGN);
		SetDataInit_Support_Lsd(pBeam, ixRow, ixCol);
	}

	const HGBOOL bBox = m_pBri->IsBoxType();
	const HGBOOL bSlab = m_pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB;
	const HGBOOL bTrap2 = m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2;
	const HGINT32 icJijum = bSlab || bBox ? 0 : bTrap2 ? m_pBri->GetCountJijum() - 2 : m_pBri->GetCountJijum();
	for (HGINT32 ix = 0; ix < icJijum; ++ix)
	{
		if (pBri->IsOutsideWall() == FALSE && (ix == 0 || ix == 1))
			continue;

		const HGINT32 ixJijum = ix == 0 ? 0 : ix == 1 ? m_pBri->GetCountJijum() - 1 : ix - 1;

		CConcBasic *pConc = NULL;
		const HGINT32 icPos = 2;

		m_Grid.SetMergeRow(ixRow+1, ixRow + 2, 0);

		for (HGINT32 ixPos = 0; ixPos < icPos; ++ixPos)
		{
			HGBOOL bFront = ixPos == 0 ? TRUE : FALSE;
			
			pConc = pBri->GetConcDataFooting(ixJijum, bFront, FALSE);

			if(pConc == NULL)
				continue;

			if(ix < 2)
			{
				strPos = pConc->m_sTitle.Left(pConc->m_sTitle.GetLength()-6);
				strPos1 = pConc->m_sTitle.Right(6);
			}
			else
			{
				strPos = pConc->m_sTitle;
				strPos1 = pConc->m_sTitle;
			}
			strPos1.Replace("(", "");
			strPos1.Replace(")", "");

			HGINT32 ixCol = 0;
			m_Grid.SetTextMatrix(++ixRow, ixCol++, strPos);
			m_Grid.SetTextMatrix(ixRow, ixCol++, strPos1);

			CLsdBeam pBeam(pConc, LSD_ULS_DESIGN);
			SetDataInit_Support_Lsd(pBeam, ixRow, ixCol);
		}
	}
}

void CInputCheckGrid::ShowDlg()
{
	CSize WSize[] = { 
			CSize(650,420), // IDD_GEN_FRAME_ANLYSIS
			CSize(650,420), // IDD_GEN_SHELL_ANLYSIS
			CSize(700,200), // IDD_GEN_JONG_SECTION
			CSize(650,280), // IDD_GEN_FOOTING
			CSize(960,400), // IDD_REBAR_MAIN
			CSize(960,400), // IDD_REBAR_MAIN2
			CSize(600,200), // IDD_REBAR_MAIN3
			CSize(960,500), // IDD_REBAR_SECTION
			CSize(800,250), // IDD_REBAR_WINGWALL
			CSize(750,250), // IDD_REBAR_COLUMN
			CSize(600,150), // IDD_GEN_WING_SHELL_ANLYSIS
			CSize(900,480),  // IDD_STM_ARC_CORNER, IDD_STM_BRACKET, IDD_STM_FOORING_MID,IDD_STM_FOORING_STARTEND
			CSize(1100,650), // IDD_GEN_DESIGN_VARIABLE
			CSize(1000, 400) // IDD_REBAR_APS
	};
	if(m_pBri->IsBoxType())
	{
		WSize[0].cy = 620;
		WSize[1].cy = 620;
		WSize[4].cx = 1020;
	}

	if(m_pBri->m_pARcBridgeDataStd->IsLsdDesign())
	{
		WSize[0].cx = 960;
		WSize[3].cx = 750;
		WSize[3].cy = 700;
		WSize[8].cx = 950;

	}

	switch(m_nInputIDD)
	{
		case IDD_GEN_FRAME_ANLYSIS:		
			SetWindowPos(&wndTop, 0, 0, WSize[0].cx, WSize[0].cy, SWP_NOMOVE | SWP_DRAWFRAME);		break;
		case IDD_GEN_SHELL_ANLYSIS:		
			SetWindowPos(&wndTop, -1, -1, WSize[1].cx, WSize[1].cy, SWP_NOMOVE | SWP_DRAWFRAME);	break;			
		case IDD_GEN_JONG_SECTION:		
			SetWindowPos(&wndTop, -1, -1, WSize[2].cx, WSize[2].cy, SWP_NOMOVE | SWP_DRAWFRAME);	break;			
		case IDD_GEN_FOOTING:	
			SetWindowPos(&wndTop, -1, -1, WSize[3].cx, WSize[3].cy, SWP_NOMOVE | SWP_DRAWFRAME);	break;			
		case IDD_REBAR_MAIN:	
			SetWindowPos(&wndTop, -1, -1, WSize[4].cx, WSize[4].cy, SWP_NOMOVE | SWP_DRAWFRAME);	break;			
		case IDD_REBAR_MAIN2:	
			SetWindowPos(&wndTop, -1, -1, WSize[5].cx, WSize[5].cy, SWP_NOMOVE | SWP_DRAWFRAME);	break;			
		case IDD_REBAR_MAIN3:
			SetWindowPos(&wndTop, -1, -1, WSize[6].cx, WSize[6].cy, SWP_NOMOVE | SWP_DRAWFRAME);	break;			
		case IDD_REBAR_SECTION:		
			SetWindowPos(&wndTop, -1, -1, WSize[7].cx, WSize[7].cy, SWP_NOMOVE | SWP_DRAWFRAME);	break;
		case IDD_REBAR_WINGWALL:
			SetWindowPos(&wndTop, -1, -1, WSize[8].cx, WSize[8].cy, SWP_NOMOVE | SWP_DRAWFRAME);	break;
		case IDD_REBAR_COLUMN:
			SetWindowPos(&wndTop, -1, -1, WSize[9].cx, WSize[9].cy, SWP_NOMOVE | SWP_DRAWFRAME);	break;
		case IDD_GEN_WING_SHELL_ANLYSIS:
			SetWindowPos(&wndTop, -1, -1, WSize[10].cx, WSize[10].cy, SWP_NOMOVE | SWP_DRAWFRAME);	break;	
		case IDD_STM_ARC_CORNER:
		case IDD_STM_BRACKET:
		case IDD_STM_FOORING_MID:
		case IDD_STM_FOORING_STARTEND:
			SetWindowPos(&wndTop, 0, 0, WSize[11].cx, WSize[11].cy, SWP_NOMOVE | SWP_DRAWFRAME);	break;	
		case IDD_GEN_DESIGN_VARIABLE:
			SetWindowPos(&wndTop, 0, 0, WSize[12].cx, WSize[12].cy, SWP_NOMOVE | SWP_DRAWFRAME);	break;	
		case IDD_REBAR_APS:
			SetWindowPos(&wndTop, 0, 0, WSize[13].cx, WSize[13].cy, SWP_NOMOVE | SWP_DRAWFRAME);	break;	
	}
	CenterWindow();
}

void CInputCheckGrid::MakeMatrixDesign(BOOL bFrame, BOOL bEQ, BOOL bShell)
{
	long nJijumConnect = m_pBri->GetJijumConnectRibAndSlab();
	long nSizeRow = m_pBri->m_nQtyJigan*2 + 1; // 타이틀,슬래브
	
	nSizeRow += (m_pBri->m_nQtyJigan-1) * (m_pBri->m_nSelectInWallSectionDesign == 0 ? 5 : 1); //중간벽체 - 앞축부재로 검토인지 휨부재로 검토인지에따라 갯수가 다르다.
	if(m_pBri->IsBoxType())
	{
		nSizeRow += m_pBri->m_nQtyJigan*2 + 1;
	}
	if(m_pBri->IsRcSlab())		nSizeRow -= (m_pBri->m_nQtyJigan-1) * (m_pBri->m_nSelectInWallSectionDesign == 0 ? 5 : 1);	 // ,중간벽체
	if(m_pBri->IsOutsideWall()) nSizeRow += 5*2;						 // ,시종점벽체(상단,중앙외측.내측,하단외측.내측)
	long nSizeCol = 5; // 계수하중(M,V,P),사용하중(M,V)
	if(nJijumConnect > 0)
	{
		long nSize = m_pBri->m_pArrConcArchRib.GetSize();
		nSizeRow -= 1;		// 아치리브와 접합부분의 벽체는 없다.
		nSizeRow += nSize;
	}
	if(m_pBri->m_bExistProtectionRiseFooting)
		nSizeRow += 2;

	if(bEQ) m_MatEQ.Resize(nSizeRow,nSizeCol);
	if(bShell) m_MatShell.Resize(nSizeRow,nSizeCol);
	if(bFrame) m_MatFrame.Resize(nSizeRow,nSizeCol);

	long nRow = 0;
	long nCol = 0;
	double dMoment = 0;
	double dShear  = 0;
	double dAxial  = 0;

	BOOL bGirderCalc = FALSE;
	BOOL bOutSideWall= TRUE; // TRUE-배면,FALSE-전면

	m_pBri->GetTvVertSection_Std(0, FALSE, FALSE, FALSE, (m_pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH) ? 1:0);
	//////////////////////////////////////////////////////////////////////////
	long i=0;
	long nJijum=0;
	if(bFrame)
	{
		// 평상시 2D FRAME
		for(long iUD = iUPPER; iUD <= iLOWER; iUD++)
		{
			BOOL bUpperSlab = iUD == iUPPER ? TRUE : FALSE;
			if(iUD == iLOWER && m_pBri->IsBoxType() == FALSE) break;
			for(i=0; i<m_pBri->m_nQtyJigan; i++)
			{
				long nPosEnd = (i==m_pBri->m_nQtyJigan-1) ? 3 : 2;
				for(long nPos = 0; nPos<nPosEnd; nPos++)
				{
					double dDistShear = m_pBri->GetDistCheckShearSlab(i, nPos, bUpperSlab, NULL);
					nCol = 0;
					dMoment = Round(m_pBri->GetOutValueSlab(i,nPos,COMBO_ULTIMATE,ELE_MOMENT2,TRUE,FALSE,bUpperSlab),3);
					dShear  = Round(m_pBri->GetOutValueSlab(i,nPos,COMBO_ULTIMATE,ELE_SHEAR_2,TRUE,FALSE,bUpperSlab, eEnvLimit, dDistShear),3);
					dAxial  = 0;
					if(nPos==0 && i>0)
					{
						double dMoment2 = Round(m_pBri->GetOutValueSlab(i-1,2,COMBO_ULTIMATE,ELE_MOMENT2,TRUE,FALSE,bUpperSlab),3);
						double dShear2  = Round(m_pBri->GetOutValueSlab(i-1,2,COMBO_ULTIMATE,ELE_SHEAR_2,TRUE,FALSE,bUpperSlab, eEnvLimit, dDistShear),3);
						dMoment = min(dMoment,dMoment2);
						dShear  = max(dShear ,dShear2);
					}
					m_MatFrame(nRow,nCol++) = dMoment;
					m_MatFrame(nRow,nCol++) = dShear;
					m_MatFrame(nRow,nCol++) = dAxial;
					//
					dMoment = Round(m_pBri->GetOutValueSlab(i,nPos,COMBO_SERVICE,ELE_MOMENT2,TRUE,FALSE,bUpperSlab),3);
					dShear  = Round(m_pBri->GetOutValueSlab(i,nPos,COMBO_SERVICE,ELE_SHEAR_2,TRUE,FALSE,bUpperSlab, eEnvLimit, dDistShear),3);
					if(nPos==0 && i>0)
					{
						double dMoment2 = Round(m_pBri->GetOutValueSlab(i-1,2,COMBO_SERVICE,ELE_MOMENT2,TRUE,FALSE,bUpperSlab),3);
						double dShear2  = Round(m_pBri->GetOutValueSlab(i-1,2,COMBO_SERVICE,ELE_SHEAR_2,TRUE,FALSE,bUpperSlab, eEnvLimit, dDistShear),3);
						dMoment = min(dMoment,dMoment2);
						dShear  = max(dShear ,dShear2);
					}
					m_MatFrame(nRow,nCol++) = dMoment;
					m_MatFrame(nRow,nCol++) = dShear;
					nRow++;	
				}
			}
		}

		// 시종점벽체
		if(m_pBri->IsOutsideWall())
		{
			for(i=0; i<2; i++)
			{
				nJijum  = (i==0) ? 0 : m_pBri->m_nQtyJigan;
				long nPosEnd = 3;
				for(long nPos = 0; nPos<nPosEnd; nPos++)
				{
					double dDistShear = m_pBri->GetDistCheckShearWall(nJijum, nPos, NULL);

					for(long nSide=0; nSide<2; nSide++)
					{
						bOutSideWall = (nSide==0) ? TRUE : FALSE; // TRUE - 배면
						if(nPos==0 && bOutSideWall==FALSE) continue;
						nCol = 0;
						dMoment = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_ULTIMATE,ELE_MOMENT2,bGirderCalc,bOutSideWall),3);
						dShear  = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_ULTIMATE,ELE_SHEAR_2,bGirderCalc,bOutSideWall, eEnvLimit, dDistShear),3);
						dAxial  = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_ULTIMATE,ELE_AXFORCE,bGirderCalc,bOutSideWall),3);
						m_MatFrame(nRow,nCol++) = dMoment;
						m_MatFrame(nRow,nCol++) = dShear;
						m_MatFrame(nRow,nCol++) = dAxial;
						//
						dMoment = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_SERVICE,ELE_MOMENT2,bGirderCalc,bOutSideWall),3);
						dShear  = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_SERVICE,ELE_SHEAR_2,bGirderCalc,bOutSideWall, eEnvLimit, dDistShear),3);
						m_MatFrame(nRow,nCol++) = dMoment;
						m_MatFrame(nRow,nCol++) = dShear;
						nRow++;	
					}
				}
			}
		}

		// 아치리브
		if(nJijumConnect > 0)
		{
			long nSize = m_pBri->m_pArrConcArchRib.GetSize();
			long n=0; for(n=0; n<nSize; n++)
			{
				long nSpan = 0;
				long nPosCheck = 0;
				BOOL bCheckCompare = FALSE;
				BOOL bCrown = FALSE;
				m_pCalc->m_pArchRib->GetInxSpanAndCheckPosByIndex(n, nSpan, nPosCheck, bCheckCompare, bCrown);

				nCol = 0;
				dMoment = Round(m_pCalc->m_pArchRib->GetOutValueArchRibBySlab(nSpan,nPosCheck,COMBO_ULTIMATE,ELE_MOMENT2, bCrown),3);
				dShear  = Round(m_pCalc->m_pArchRib->GetOutValueArchRibBySlab(nSpan,nPosCheck,COMBO_ULTIMATE,ELE_SHEAR_2, bCrown),3);
				dAxial  = 0;
				if(bCheckCompare)
				{
					double dMoment2 = Round(m_pCalc->m_pArchRib->GetOutValueArchRibBySlab(nSpan+1,CHECK_POS_LEFT,COMBO_ULTIMATE,ELE_MOMENT2, bCrown),3);
					double dShear2  = Round(m_pCalc->m_pArchRib->GetOutValueArchRibBySlab(nSpan+1,CHECK_POS_LEFT,COMBO_ULTIMATE,ELE_SHEAR_2, bCrown),3);
					dMoment = min(dMoment,dMoment2);
					dShear  = max(dShear ,dShear2);
				}
				m_MatFrame(nRow,nCol++) = dMoment;
				m_MatFrame(nRow,nCol++) = dShear;
				m_MatFrame(nRow,nCol++) = dAxial;
				//
				dMoment = Round(m_pCalc->m_pArchRib->GetOutValueArchRibBySlab(nSpan,nPosCheck,COMBO_SERVICE,ELE_MOMENT2, bCrown),3);
				dShear  = Round(m_pCalc->m_pArchRib->GetOutValueArchRibBySlab(nSpan,nPosCheck,COMBO_SERVICE,ELE_SHEAR_2, bCrown),3);
				if(bCheckCompare)
				{
					double dMoment2 = Round(m_pCalc->m_pArchRib->GetOutValueArchRibBySlab(nSpan+1,CHECK_POS_LEFT,COMBO_SERVICE,ELE_MOMENT2, bCrown),3);
					double dShear2  = Round(m_pCalc->m_pArchRib->GetOutValueArchRibBySlab(nSpan+1,CHECK_POS_LEFT,COMBO_SERVICE,ELE_SHEAR_2, bCrown),3);
					dMoment = min(dMoment,dMoment2);
					dShear  = max(dShear ,dShear2);
				}
				m_MatFrame(nRow,nCol++) = dMoment;
				m_MatFrame(nRow,nCol++) = dShear;
				nRow++;	
			}
		}

		// 중간벽체
		for(nJijum=1; nJijum<m_pBri->m_nQtyJigan; nJijum++)
		{
			if(m_pBri->IsRcSlab()) break;
			if(nJijum == nJijumConnect) continue;

			if(m_pBri->m_nSelectInWallSectionDesign == 0)
			{
				long nPosEnd = 3;
				for(long nPos = 0; nPos<nPosEnd; nPos++)
				{
					double dDistShear = m_pBri->GetDistCheckShearWall(nJijum, nPos, NULL);

					for(long nSide=0; nSide<2; nSide++)
					{
						bOutSideWall = (nSide==0) ? TRUE : FALSE; // TRUE - 배면
						if(nPos==0 && bOutSideWall==FALSE) continue;
						nCol = 0;
						dMoment = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_ULTIMATE,ELE_MOMENT2,bGirderCalc,bOutSideWall),3);
						dShear  = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_ULTIMATE,ELE_SHEAR_2,bGirderCalc,bOutSideWall, eEnvLimit, dDistShear),3);
						dAxial  = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_ULTIMATE,ELE_AXFORCE,bGirderCalc,bOutSideWall),3);
						m_MatFrame(nRow,nCol++) = dMoment;
						m_MatFrame(nRow,nCol++) = dShear;
						m_MatFrame(nRow,nCol++) = dAxial;
						//
						dMoment = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_SERVICE,ELE_MOMENT2,bGirderCalc,bOutSideWall),3);
						dShear  = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_SERVICE,ELE_SHEAR_2,bGirderCalc,bOutSideWall, eEnvLimit, dDistShear),3);
						m_MatFrame(nRow,nCol++) = dMoment;
						m_MatFrame(nRow,nCol++) = dShear;
						nRow++;	
					}
				}
			}
			else
			{
				nCol = 0;
				long nPos = 0;
				dMoment = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_ULTIMATE,ELE_MOMENT2,bGirderCalc,bOutSideWall),3);
				dShear  = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_ULTIMATE,ELE_SHEAR_2,bGirderCalc,bOutSideWall),3);
				dAxial  = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_ULTIMATE,ELE_AXFORCE,bGirderCalc,bOutSideWall),3);
				m_MatFrame(nRow,nCol++) = dMoment;
				m_MatFrame(nRow,nCol++) = dShear;
				m_MatFrame(nRow,nCol++) = dAxial;
				dMoment = 0;
				dShear  = 0;
				dAxial  = 0;
				m_MatFrame(nRow,nCol++) = dMoment;
				m_MatFrame(nRow,nCol++) = dShear;
				nRow++;	
			}
		}

		if(m_pBri->m_bExistProtectionRiseFooting)
		{
			for(long iSE=iSTT; iSE<=iEND; ++iSE)
			{
				BOOL bStt = iSE == iSTT;
					
				nCol = 0;
				dMoment = Round(m_pBri->GetOutValuePRF(bStt,COMBO_ULTIMATE,ELE_MOMENT2),3);
				dShear  = Round(m_pBri->GetOutValuePRF(bStt,COMBO_ULTIMATE,ELE_SHEAR_2),3);
				dAxial  = Round(m_pBri->GetOutValuePRF(bStt,COMBO_ULTIMATE,ELE_AXFORCE),3);
				m_MatFrame(nRow,nCol++) = dMoment;
				m_MatFrame(nRow,nCol++) = dShear;
				m_MatFrame(nRow,nCol++) = dAxial;
				//
				dMoment = Round(m_pBri->GetOutValuePRF(bStt,COMBO_SERVICE,ELE_MOMENT2),3);
				dShear  = Round(m_pBri->GetOutValuePRF(bStt,COMBO_SERVICE,ELE_SHEAR_2),3);
				m_MatFrame(nRow,nCol++) = dMoment;
				m_MatFrame(nRow,nCol++) = dShear;
				nRow++;		
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 지진시 2D FRAME
	if(bEQ)
	{
		nRow = 0;
		for(long iUD = iUPPER; iUD <= iLOWER; iUD++)
		{
			BOOL bUpperSlab = iUD == iUPPER ? TRUE : FALSE;
			if(iUD == iLOWER && m_pBri->IsBoxType() == FALSE) break;
			for(i=0; i<m_pBri->m_nQtyJigan; i++)
			{
				if(m_pBri->IsRcSlab()) break;

				long nPosEnd = (i==m_pBri->m_nQtyJigan-1) ? 3 : 2;
				for(long nPos = 0; nPos<nPosEnd; nPos++)
				{
					double dDistShear = m_pBri->GetDistCheckShearSlab(i, nPos, bUpperSlab, NULL);
					nCol = 0;
					dMoment = Round(m_pBri->GetOutValueSlab(i,nPos,COMBO_EARTHQUAKE,ELE_MOMENT2,TRUE,FALSE,bUpperSlab),3);
					dShear  = Round(m_pBri->GetOutValueSlab(i,nPos,COMBO_EARTHQUAKE,ELE_SHEAR_2,TRUE,FALSE,bUpperSlab, eEnvLimit, dDistShear),3);
					dAxial  = 0;
					if(nPos==0 && i>0)
					{
						double dMoment2 = Round(m_pBri->GetOutValueSlab(i-1,2,COMBO_EARTHQUAKE,ELE_MOMENT2,TRUE,FALSE,bUpperSlab),3);
						double dShear2  = Round(m_pBri->GetOutValueSlab(i-1,2,COMBO_EARTHQUAKE,ELE_SHEAR_2,TRUE,FALSE,bUpperSlab, eEnvLimit, dDistShear),3);
						dMoment = min(dMoment,dMoment2);
						dShear  = max(dShear ,dShear2);
					}
					if(m_pBri->m_nSelectModelingCountEQ==2)
					{
						double dMoment3 = Round(m_pBri->GetOutValueSlab(i,nPos,COMBO_EARTHQUAKE2,ELE_MOMENT2,TRUE,FALSE,bUpperSlab),3);
						double dShear3  = Round(m_pBri->GetOutValueSlab(i,nPos,COMBO_EARTHQUAKE2,ELE_SHEAR_2,TRUE,FALSE,bUpperSlab, eEnvLimit, dDistShear),3);
						if(nPos==0 && i>0)
						{
							double dMoment4 = Round(m_pBri->GetOutValueSlab(i-1,2,COMBO_EARTHQUAKE2,ELE_MOMENT2,TRUE,FALSE,bUpperSlab),3);
							double dShear4  = Round(m_pBri->GetOutValueSlab(i-1,2,COMBO_EARTHQUAKE2,ELE_SHEAR_2,TRUE,FALSE,bUpperSlab, eEnvLimit, dDistShear),3);
							dMoment3 = min(dMoment3,dMoment4);
							dShear3  = max(dShear3 ,dShear4);
							dMoment  = min(dMoment,dMoment3);
							dShear   = max(dShear ,dShear3);
						}
						else if(nPos==1)
						{
							dMoment = max(dMoment,dMoment3);
							dShear  = max(dShear ,dShear3);
						}
						else
						{
							dMoment = min(dMoment,dMoment3);
							dShear  = max(dShear ,dShear3);
						}
					}
					m_MatEQ(nRow,nCol++) = dMoment;
					m_MatEQ(nRow,nCol++) = dShear;
					m_MatEQ(nRow,nCol++) = dAxial;
					//
					m_MatEQ(nRow,nCol++) = 0;
					m_MatEQ(nRow,nCol++) = 0;
					nRow++;	
				}
			}
		}
		// 시종점벽체
		if(m_pBri->IsOutsideWall())
		{
			for(i=0; i<2; i++)
			{
				nJijum  = (i==0) ? 0 : m_pBri->m_nQtyJigan;
				long nPosEnd = 3;
				for(long nPos = 0; nPos<nPosEnd; nPos++)
				{
					double dDistShear = m_pBri->GetDistCheckShearWall(nJijum, nPos, NULL);

					for(long nSide=0; nSide<2; nSide++)
					{
						bOutSideWall = (nSide==0) ? TRUE : FALSE; // TRUE - 배면
						if(nPos==0 && bOutSideWall==FALSE) continue;
						nCol = 0;
						dMoment = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_EARTHQUAKE,ELE_MOMENT2,bGirderCalc,bOutSideWall),3);
						dShear  = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_EARTHQUAKE,ELE_SHEAR_2,bGirderCalc,bOutSideWall, eEnvLimit, dDistShear),3);
						dAxial  = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_EARTHQUAKE,ELE_AXFORCE,bGirderCalc,bOutSideWall),3);
						if(m_pBri->m_nSelectModelingCountEQ==2)
						{
							double dMoment2 = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_EARTHQUAKE2,ELE_MOMENT2,bGirderCalc,bOutSideWall),3);
							double dShear2  = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_EARTHQUAKE2,ELE_SHEAR_2,bGirderCalc,bOutSideWall, eEnvLimit, dDistShear),3);
							double dAxial2  = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_EARTHQUAKE2,ELE_AXFORCE,bGirderCalc,bOutSideWall),3);
							dMoment = bOutSideWall ? min(dMoment,dMoment2) : max(dMoment,dMoment2);
							dShear  = max(dShear,dShear2);
							dAxial  = max(dAxial,dAxial2);
						}
						m_MatEQ(nRow,nCol++) = dMoment;
						m_MatEQ(nRow,nCol++) = dShear;
						m_MatEQ(nRow,nCol++) = dAxial;
						//
						m_MatEQ(nRow,nCol++) = 0;
						m_MatEQ(nRow,nCol++) = 0;
						nRow++;	
					}
				}
			}
		}

		// 아치리브
		if(nJijumConnect > 0)
		{
			long nSize = m_pBri->m_pArrConcArchRib.GetSize();
			long n=0; for(n=0; n<nSize; n++)
			{
				long nSpan = 0;
				long nPosCheck = 0;
				BOOL bCheckCompare = FALSE;
				BOOL bCrown = FALSE;
				m_pCalc->m_pArchRib->GetInxSpanAndCheckPosByIndex(n, nSpan, nPosCheck, bCheckCompare, bCrown);

				nCol = 0;
				dMoment = Round(m_pCalc->m_pArchRib->GetOutValueArchRibBySlab(nSpan,nPosCheck,COMBO_EARTHQUAKE,ELE_MOMENT2, bCrown),3);
				dShear  = Round(m_pCalc->m_pArchRib->GetOutValueArchRibBySlab(nSpan,nPosCheck,COMBO_EARTHQUAKE,ELE_SHEAR_2, bCrown),3);
				dAxial  = 0;
				if(bCheckCompare)
				{
					double dMoment2 = Round(m_pCalc->m_pArchRib->GetOutValueArchRibBySlab(nSpan+1,CHECK_POS_LEFT,COMBO_EARTHQUAKE,ELE_MOMENT2, bCrown),3);
					double dShear2  = Round(m_pCalc->m_pArchRib->GetOutValueArchRibBySlab(nSpan+1,CHECK_POS_LEFT,COMBO_EARTHQUAKE,ELE_SHEAR_2, bCrown),3);
					dMoment = min(dMoment,dMoment2);
					dShear  = max(dShear ,dShear2);
				}
				if(m_pBri->m_nSelectModelingCountEQ==2)
				{
					double dMoment3 = Round(m_pCalc->m_pArchRib->GetOutValueArchRibBySlab(nSpan,nPosCheck,COMBO_EARTHQUAKE2,ELE_MOMENT2, bCrown),3);
					double dShear3  = Round(m_pCalc->m_pArchRib->GetOutValueArchRibBySlab(nSpan,nPosCheck,COMBO_EARTHQUAKE2,ELE_SHEAR_2, bCrown),3);
					if(bCheckCompare)
					{
						double dMoment4 = Round(m_pCalc->m_pArchRib->GetOutValueArchRibBySlab(nSpan+1,CHECK_POS_LEFT,COMBO_EARTHQUAKE2,ELE_MOMENT2, bCrown),3);
						double dShear4  = Round(m_pCalc->m_pArchRib->GetOutValueArchRibBySlab(nSpan+1,CHECK_POS_LEFT,COMBO_EARTHQUAKE2,ELE_SHEAR_2, bCrown),3);
						dMoment3 = min(dMoment3,dMoment4);
						dShear3  = max(dShear3 ,dShear4);
						dMoment  = min(dMoment,dMoment3);
						dShear   = max(dShear ,dShear3);
					}
					else if(nPosCheck==1)
					{
						dMoment = max(dMoment,dMoment3);
						dShear  = max(dShear ,dShear3);
					}
					else
					{
						dMoment = min(dMoment,dMoment3);
						dShear  = max(dShear ,dShear3);
					}
				}
				m_MatEQ(nRow,nCol++) = dMoment;
				m_MatEQ(nRow,nCol++) = dShear;
				m_MatEQ(nRow,nCol++) = dAxial;
				//
				m_MatEQ(nRow,nCol++) = 0;
				m_MatEQ(nRow,nCol++) = 0;
				nRow++;
			}
		}

		// 중간벽체
		for(nJijum=1; nJijum<m_pBri->m_nQtyJigan; nJijum++)
		{
			if(m_pBri->IsRcSlab()) break;
			if(nJijum == nJijumConnect) continue;

			if(m_pBri->m_nSelectInWallSectionDesign == 0)
			{
				long nPosEnd = 3;
				for(long nPos = 0; nPos<nPosEnd; nPos++)
				{
					double dDistShear = m_pBri->GetDistCheckShearWall(nJijum, nPos, NULL);

					for(long nSide=0; nSide<2; nSide++)
					{
						bOutSideWall = (nSide==0) ? TRUE : FALSE; // TRUE - 배면
						if(nPos==0 && bOutSideWall==FALSE) continue;
						nCol = 0;
						dMoment = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_EARTHQUAKE,ELE_MOMENT2,bGirderCalc,bOutSideWall),3);
						dShear  = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_EARTHQUAKE,ELE_SHEAR_2,bGirderCalc,bOutSideWall, eEnvLimit, dDistShear),3);
						dAxial  = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_EARTHQUAKE,ELE_AXFORCE,bGirderCalc,bOutSideWall),3);
						if(m_pBri->m_nSelectModelingCountEQ==2)
						{
							double dMoment2 = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_EARTHQUAKE2,ELE_MOMENT2,bGirderCalc,bOutSideWall),3);
							double dShear2  = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_EARTHQUAKE2,ELE_SHEAR_2,bGirderCalc,bOutSideWall, eEnvLimit, dDistShear),3);
							double dAxial2  = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_EARTHQUAKE2,ELE_AXFORCE,bGirderCalc,bOutSideWall),3);
							dMoment = bOutSideWall ? min(dMoment,dMoment2) : max(dMoment,dMoment2);
							dShear  = max(dShear,dShear2);
							dAxial  = max(dAxial,dAxial2);
						}
						m_MatEQ(nRow,nCol++) = dMoment;
						m_MatEQ(nRow,nCol++) = dShear;
						m_MatEQ(nRow,nCol++) = dAxial;
						//
						m_MatEQ(nRow,nCol++) = 0;
						m_MatEQ(nRow,nCol++) = 0;
						nRow++;	
					}
				}
			}
			else
			{
				nCol = 0;
				long nPos = 0;
				dMoment = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_EARTHQUAKE,ELE_MOMENT2,bGirderCalc,bOutSideWall),3);
				dShear  = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_EARTHQUAKE,ELE_SHEAR_2,bGirderCalc,bOutSideWall),3);
				dAxial  = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_EARTHQUAKE,ELE_AXFORCE,bGirderCalc,bOutSideWall),3);
				if(m_pBri->m_nSelectModelingCountEQ==2)
				{
					double dMoment2 = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_EARTHQUAKE2,ELE_MOMENT2,bGirderCalc,bOutSideWall),3);
					double dShear2  = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_EARTHQUAKE2,ELE_SHEAR_2,bGirderCalc,bOutSideWall),3);
					double dAxial2  = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_EARTHQUAKE2,ELE_AXFORCE,bGirderCalc,bOutSideWall),3);
					if(fabs(dMoment2/dAxial2)>fabs(dMoment/dAxial)) // 편심이 큰 값으로 적용...
					{
						dMoment = dMoment2;
						dShear  = dShear2;
						dAxial  = dAxial2;
					}
				}
				m_MatEQ(nRow,nCol++) = dMoment;
				m_MatEQ(nRow,nCol++) = dShear;
				m_MatEQ(nRow,nCol++) = dAxial;
				//
				m_MatEQ(nRow,nCol++) = 0;
				m_MatEQ(nRow,nCol++) = 0;
				nRow++;	
			}
		}
		if(m_pBri->m_bExistProtectionRiseFooting)
		{
			for(long iSE=iSTT; iSE<=iEND; ++iSE)
			{	
				BOOL bStt = (iSE == iSTT);
				
				nCol = 0;
				dMoment = Round(m_pBri->GetOutValuePRF(bStt,COMBO_EARTHQUAKE,ELE_MOMENT2),3);
				dShear  = Round(m_pBri->GetOutValuePRF(bStt,COMBO_EARTHQUAKE,ELE_SHEAR_2),3);
				dAxial  = Round(m_pBri->GetOutValuePRF(bStt,COMBO_EARTHQUAKE,ELE_AXFORCE),3);
				if(m_pBri->m_nSelectModelingCountEQ==2)
				{
					double dMoment2 = Round(m_pBri->GetOutValuePRF(bStt,COMBO_EARTHQUAKE2,ELE_MOMENT2),3);
					double dShear2  = Round(m_pBri->GetOutValuePRF(bStt,COMBO_EARTHQUAKE2,ELE_SHEAR_2),3);
					double dAxial2  = Round(m_pBri->GetOutValuePRF(bStt,COMBO_EARTHQUAKE2,ELE_AXFORCE),3);
					dMoment = max(dMoment,dMoment2);
					dShear  = max(dShear,dShear2);
					dAxial  = max(dAxial,dAxial2);
				}
				m_MatEQ(nRow,nCol++) = dMoment;
				m_MatEQ(nRow,nCol++) = dShear;
				m_MatEQ(nRow,nCol++) = dAxial;
				//
				m_MatEQ(nRow,nCol++) = 0;
				m_MatEQ(nRow,nCol++) = 0;
				nRow++;	
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 평상시 3D SHELL
	if(bShell)
	{
		nRow = 0;
		for(long iUD = iUPPER; iUD <= iLOWER; iUD++)
		{
			BOOL bUpperSlab = iUD == iUPPER ? TRUE : FALSE;
			if(iUD == iLOWER && m_pBri->IsBoxType() == FALSE) break;
			for(i=0; i<m_pBri->m_nQtyJigan; i++)
			{
				long nPosEnd = (i==m_pBri->m_nQtyJigan-1) ? 3 : 2;
				for(long nPos = 0; nPos<nPosEnd; nPos++)
				{
					nCol = 0;
					dMoment = Round(m_pBri->GetOutValueSlabShell(i,nPos,COMBO_ULTIMATE,ELE_MOMENT2,bUpperSlab),3);
					dShear  = Round(m_pBri->GetOutValueSlabShell(i,nPos,COMBO_ULTIMATE,ELE_SHEAR_2,bUpperSlab),3);
					dAxial  = 0;
					if(nPos==0 && i>0)
					{
						double dMoment2 = Round(m_pBri->GetOutValueSlabShell(i-1,2,COMBO_ULTIMATE,ELE_MOMENT2,bUpperSlab),3);
						double dShear2  = Round(m_pBri->GetOutValueSlabShell(i-1,2,COMBO_ULTIMATE,ELE_SHEAR_2,bUpperSlab),3);
						dMoment = min(dMoment,dMoment2);
						dShear  = max(dShear ,dShear2);
					}
					m_MatShell(nRow,nCol++) = dMoment;
					m_MatShell(nRow,nCol++) = dShear;
					m_MatShell(nRow,nCol++) = dAxial;
					//
					dMoment = Round(m_pBri->GetOutValueSlabShell(i,nPos,COMBO_SERVICE,ELE_MOMENT2,bUpperSlab),3);
					dShear  = Round(m_pBri->GetOutValueSlabShell(i,nPos,COMBO_SERVICE,ELE_SHEAR_2,bUpperSlab),3);
					if(nPos==0 && i>0)
					{
						double dMoment2 = Round(m_pBri->GetOutValueSlabShell(i-1,2,COMBO_SERVICE,ELE_MOMENT2,bUpperSlab),3);
						double dShear2  = Round(m_pBri->GetOutValueSlabShell(i-1,2,COMBO_SERVICE,ELE_SHEAR_2,bUpperSlab),3);
						dMoment = min(dMoment,dMoment2);
						dShear  = max(dShear ,dShear2);
					}
					m_MatShell(nRow,nCol++) = dMoment;
					m_MatShell(nRow,nCol++) = dShear;
					nRow++;	
				}
			}
		}
		// 시종점벽체
		if(m_pBri->IsOutsideWall())
		{
			for(i=0; i<2; i++)
			{
				long nJijum  = (i==0) ? 0 : m_pBri->m_nQtyJigan;
				long nPosEnd = 3;
				for(long nPos = 0; nPos<nPosEnd; nPos++)
				{
					for(long nSide=0; nSide<2; nSide++)
					{
						bOutSideWall = (nSide==0) ? TRUE : FALSE; // TRUE - 배면
						if(nPos==0 && bOutSideWall==FALSE) continue;
						nCol = 0;
						dMoment = Round(m_pBri->GetOutValueWallShell(nJijum,nPos,COMBO_ULTIMATE,ELE_MOMENT2,bGirderCalc,bOutSideWall),3);
						dShear  = Round(m_pBri->GetOutValueWallShell(nJijum,nPos,COMBO_ULTIMATE,ELE_SHEAR_2,bGirderCalc,bOutSideWall),3);
						dAxial  = Round(m_pBri->GetOutValueWallShell(nJijum,nPos,COMBO_ULTIMATE,ELE_AXFORCE,bGirderCalc,bOutSideWall),3);
						m_MatShell(nRow,nCol++) = dMoment;
						m_MatShell(nRow,nCol++) = dShear;
						m_MatShell(nRow,nCol++) = dAxial;
						//
						dMoment = Round(m_pBri->GetOutValueWallShell(nJijum,nPos,COMBO_SERVICE,ELE_MOMENT2,bGirderCalc,bOutSideWall),3);
						dShear  = Round(m_pBri->GetOutValueWallShell(nJijum,nPos,COMBO_SERVICE,ELE_SHEAR_2,bGirderCalc,bOutSideWall),3);
						m_MatShell(nRow,nCol++) = dMoment;
						m_MatShell(nRow,nCol++) = dShear;
						nRow++;	
					}
				}
			}
		}

		// 아치리브
		if(nJijumConnect > 0)
		{
			long nSize = m_pBri->m_pArrConcArchRib.GetSize();
			long n=0; for(n=0; n<nSize; n++)
			{
				long nSpan = 0;
				long nPosCheck = 0;
				BOOL bCheckCompare = FALSE;
				BOOL bCrown = FALSE;
				m_pCalc->m_pArchRib->GetInxSpanAndCheckPosByIndex(n, nSpan, nPosCheck, bCheckCompare, bCrown);

				nCol = 0;
				dMoment = Round(m_pBri->GetOutValueArchRibShell(nSpan,nPosCheck,COMBO_ULTIMATE,ELE_MOMENT2),3);
				dShear  = Round(m_pBri->GetOutValueArchRibShell(nSpan,nPosCheck,COMBO_ULTIMATE,ELE_SHEAR_2),3);
				dAxial  = 0;
				if(bCheckCompare)
				{
					double dMoment2 = Round(m_pBri->GetOutValueArchRibShell(nSpan+1,CHECK_POS_LEFT,COMBO_ULTIMATE,ELE_MOMENT2),3);
					double dShear2  = Round(m_pBri->GetOutValueArchRibShell(nSpan+1,CHECK_POS_LEFT,COMBO_ULTIMATE,ELE_SHEAR_2),3);
					dMoment = min(dMoment,dMoment2);
					dShear  = max(dShear ,dShear2);
				}
				m_MatShell(nRow,nCol++) = dMoment;
				m_MatShell(nRow,nCol++) = dShear;
				m_MatShell(nRow,nCol++) = dAxial;
				//
				dMoment = Round(m_pBri->GetOutValueArchRibShell(nSpan,nPosCheck,COMBO_SERVICE,ELE_MOMENT2),3);
				dShear  = Round(m_pBri->GetOutValueArchRibShell(nSpan,nPosCheck,COMBO_SERVICE,ELE_SHEAR_2),3);
				if(bCheckCompare)
				{
					double dMoment2 = Round(m_pBri->GetOutValueArchRibShell(nSpan+1,CHECK_POS_LEFT,COMBO_SERVICE,ELE_MOMENT2),3);
					double dShear2  = Round(m_pBri->GetOutValueArchRibShell(nSpan+1,CHECK_POS_LEFT,COMBO_SERVICE,ELE_SHEAR_2),3);
					dMoment = min(dMoment,dMoment2);
					dShear  = max(dShear ,dShear2);
				}
				m_MatShell(nRow,nCol++) = dMoment;
				m_MatShell(nRow,nCol++) = dShear;
				nRow++;	
			}
		}

		// 중간벽체
		for(nJijum=1; nJijum<m_pBri->m_nQtyJigan; nJijum++)
		{
			if(m_pBri->IsRcSlab()) break;
			if(nJijum == nJijumConnect) continue;

			if(m_pBri->m_nSelectInWallSectionDesign == 0)
			{
				long nPosEnd = 3;
				for(long nPos = 0; nPos<nPosEnd; nPos++)
				{
					for(long nSide=0; nSide<2; nSide++)
					{
						bOutSideWall = (nSide==0) ? TRUE : FALSE; // TRUE - 배면
						if(nPos==0 && bOutSideWall==FALSE) continue;
						nCol = 0;
						dMoment = Round(m_pBri->GetOutValueWallShell(nJijum,nPos,COMBO_ULTIMATE,ELE_MOMENT2,bGirderCalc,bOutSideWall),3);
						dShear  = Round(m_pBri->GetOutValueWallShell(nJijum,nPos,COMBO_ULTIMATE,ELE_SHEAR_2,bGirderCalc,bOutSideWall),3);
						dAxial  = Round(m_pBri->GetOutValueWallShell(nJijum,nPos,COMBO_ULTIMATE,ELE_AXFORCE,bGirderCalc,bOutSideWall),3);
						m_MatShell(nRow,nCol++) = dMoment;
						m_MatShell(nRow,nCol++) = dShear;
						m_MatShell(nRow,nCol++) = dAxial;
						//
						dMoment = Round(m_pBri->GetOutValueWallShell(nJijum,nPos,COMBO_SERVICE,ELE_MOMENT2,bGirderCalc,bOutSideWall),3);
						dShear  = Round(m_pBri->GetOutValueWallShell(nJijum,nPos,COMBO_SERVICE,ELE_SHEAR_2,bGirderCalc,bOutSideWall),3);
						m_MatShell(nRow,nCol++) = dMoment;
						m_MatShell(nRow,nCol++) = dShear;
						nRow++;	
					}
				}
			}
			else
			{
				nCol = 0;
				long nPos = CHECK_POS_BOTTOM;
				dMoment = Round(m_pBri->GetOutValueWallShell(nJijum,nPos,COMBO_ULTIMATE,ELE_MOMENT2,bGirderCalc,bOutSideWall),3);
				dShear  = Round(m_pBri->GetOutValueWallShell(nJijum,nPos,COMBO_ULTIMATE,ELE_SHEAR_2,bGirderCalc,bOutSideWall),3);
				dAxial  = Round(m_pBri->GetOutValueWallShell(nJijum,nPos,COMBO_ULTIMATE,ELE_AXFORCE,bGirderCalc,bOutSideWall),3);
				m_MatShell(nRow,nCol++) = dMoment;
				m_MatShell(nRow,nCol++) = dShear;
				m_MatShell(nRow,nCol++) = dAxial;
				dMoment = 0;
				dShear  = 0;
				dAxial  = 0;
				m_MatShell(nRow,nCol++) = dMoment;
				m_MatShell(nRow,nCol++) = dShear;
				nRow++;	
			}
		}

		//부상방지저판
		if(m_pBri->m_bExistProtectionRiseFooting)
		{
			for(long iSE = iSTT; iSE <= iEND; iSE++)
			{
				BOOL bStt = iSE == iSTT;

				nCol = 0;
				dMoment = Round(m_pBri->GetOutValuePRFShell(bStt,COMBO_ULTIMATE,ELE_MOMENT2),3);
				dShear  = Round(m_pBri->GetOutValuePRFShell(bStt,COMBO_ULTIMATE,ELE_SHEAR_2),3);
				dAxial  = 0;

				m_MatShell(nRow,nCol++) = dMoment;
				m_MatShell(nRow,nCol++) = dShear;
				m_MatShell(nRow,nCol++) = dAxial;
				//
				dMoment = Round(m_pBri->GetOutValuePRFShell(bStt,COMBO_SERVICE,ELE_MOMENT2),3);
				dShear  = Round(m_pBri->GetOutValuePRFShell(bStt,COMBO_SERVICE,ELE_SHEAR_2),3);			
				m_MatShell(nRow,nCol++) = dMoment;
				m_MatShell(nRow,nCol++) = dShear;
				nRow++;				
			}
		}		
	}

	m_pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, (m_pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH) ? 1:0);
	//m_pBri->GetTvVertSection_Std(0, FALSE, FALSE, TRUE);
}

void CInputCheckGrid::MakeMatrixDesign_Lsd(BOOL bFrame, BOOL bShell)
{
	long nJijumConnect = m_pBri->GetJijumConnectRibAndSlab();
	long nSizeRow = m_pBri->m_nQtyJigan*2 + 1 + (m_pBri->m_nQtyJigan-1); // 타이틀,슬래브,중간벽체
	if(m_pBri->IsBoxType())
	{
		nSizeRow += m_pBri->m_nQtyJigan*2 + 1;
	}
	if(m_pBri->IsRcSlab())		nSizeRow -= (m_pBri->m_nQtyJigan-1);	 // ,중간벽체
	if(m_pBri->IsOutsideWall()) nSizeRow += 5*2;						 // ,시종점벽체(상단,중앙외측.내측,하단외측.내측)
	long nSizeCol = 3 * m_pBri->GetCountApplyEnvType(); // 한계상태 조합별(M,V,P)
	if(nJijumConnect > 0)
	{
		long nSize = m_pBri->m_pArrConcArchRib.GetSize();
		nSizeRow -= 1;		// 아치리브와 접합부분의 벽체는 없다.
		nSizeRow += nSize;
	}
	if(m_pBri->m_bExistProtectionRiseFooting) nSizeRow += 2;

	if(bShell) m_MatShell.Resize(nSizeRow,nSizeCol);
	if(bFrame) m_MatFrame.Resize(nSizeRow,nSizeCol);

	long nRow = 0;
	long nCol = 0;
	double dMoment = 0;
	double dShear  = 0;
	double dAxial  = 0;

	BOOL bGirderCalc = FALSE;
	BOOL bOutSideWall= TRUE; // TRUE-배면,FALSE-전면

	m_pBri->GetTvVertSection_Std(0, FALSE, FALSE, FALSE, (m_pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH) ? 1:0);
	//////////////////////////////////////////////////////////////////////////
	long i=0;
	long nJijum=0;
	if(bFrame)
	{
		// 평상시 2D FRAME
		for(long iUD = iUPPER; iUD <= iLOWER; iUD++)
		{
			BOOL bUpperSlab = iUD == iUPPER ? TRUE : FALSE;
			if(iUD == iLOWER && m_pBri->IsBoxType() == FALSE) break;
			for(i=0; i<m_pBri->m_nQtyJigan; i++)
			{
				long nPosEnd = (i==m_pBri->m_nQtyJigan-1) ? 3 : 2;
				for(long nPos = 0; nPos<nPosEnd; nPos++)
				{
					nCol = 0;
					double dDistShear = m_pBri->GetDistCheckShearSlab(i, nPos, bUpperSlab, NULL);

					for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
					{
						ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
						if(m_pBri->IsValidComboEnvType(eEnvType) == FALSE)
							continue;

						BOOL bEarthQuake = (eEnvType == eEnvExtremeI || eEnvType == eEnvExtremeII);
						if(bEarthQuake)
						{
							dMoment = Round(m_pBri->GetOutValueSlabEQ(i,nPos,ELE_MOMENT2,bUpperSlab, eEnvType),3);
							dShear  = Round(m_pBri->GetOutValueSlabEQ(i,nPos,ELE_SHEAR_2,bUpperSlab, eEnvType, dDistShear),3);
						}
						else
						{
							dMoment = Round(m_pBri->GetOutValueSlab(i,nPos,COMBO_ULTIMATE,ELE_MOMENT2,TRUE,FALSE,bUpperSlab, eEnvType),3);
							dShear  = Round(m_pBri->GetOutValueSlab(i,nPos,COMBO_ULTIMATE,ELE_SHEAR_2,TRUE,FALSE,bUpperSlab, eEnvType, dDistShear),3);
						}
						
						dAxial  = 0;
						if(nPos==0 && i>0)
						{
							double dMoment2(0), dShear2(0);
							if(bEarthQuake)
							{
								dMoment2 = Round(m_pBri->GetOutValueSlabEQ(i-1,2,ELE_MOMENT2,bUpperSlab, eEnvType),3);
								dShear2  = Round(m_pBri->GetOutValueSlabEQ(i-1,2,ELE_SHEAR_2,bUpperSlab, eEnvType, dDistShear),3);
							}
							else
							{
								dMoment2 = Round(m_pBri->GetOutValueSlab(i-1,2,COMBO_ULTIMATE,ELE_MOMENT2,TRUE,FALSE,bUpperSlab, eEnvType),3);
								dShear2  = Round(m_pBri->GetOutValueSlab(i-1,2,COMBO_ULTIMATE,ELE_SHEAR_2,TRUE,FALSE,bUpperSlab, eEnvType, dDistShear),3);
							}
							dMoment = min(dMoment,dMoment2);
							dShear  = max(dShear ,dShear2);
						}
						m_MatFrame(nRow,nCol++) = dMoment;
						m_MatFrame(nRow,nCol++) = dShear;
						m_MatFrame(nRow,nCol++) = dAxial;
					}
					nRow++;	
				}
			}
		}

		// 시종점벽체
		if(m_pBri->IsOutsideWall())
		{
			for(i=0; i<2; i++)
			{
				nJijum  = (i==0) ? 0 : m_pBri->m_nQtyJigan;
				long nPosEnd = 3;
				for(long nPos = 0; nPos<nPosEnd; nPos++)
				{
					double dDistShear = m_pBri->GetDistCheckShearWall(nJijum, nPos, NULL);

					for(long nSide=0; nSide<2; nSide++)
					{
						bOutSideWall = (nSide==0) ? TRUE : FALSE; // TRUE - 배면
						if(nPos==0 && bOutSideWall==FALSE) continue;
						nCol = 0;

						for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
						{
							ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
							if(m_pBri->IsValidComboEnvType(eEnvType) == FALSE)
								continue;

							BOOL bEarthQuake = (eEnvType == eEnvExtremeI || eEnvType == eEnvExtremeII);
							if(bEarthQuake)
							{
								dMoment = Round(m_pBri->GetOutValueWallEQ(nJijum,nPos,ELE_MOMENT2,bGirderCalc,bOutSideWall, eEnvType),3);
								dShear  = Round(m_pBri->GetOutValueWallEQ(nJijum,nPos,ELE_SHEAR_2,bGirderCalc,bOutSideWall, eEnvType, dDistShear),3);
								dAxial  = Round(m_pBri->GetOutValueWallEQ(nJijum,nPos,ELE_AXFORCE,bGirderCalc,bOutSideWall, eEnvType),3);
							}
							else
							{
								dMoment = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_ULTIMATE,ELE_MOMENT2,bGirderCalc,bOutSideWall, eEnvType),3);
								dShear  = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_ULTIMATE,ELE_SHEAR_2,bGirderCalc,bOutSideWall, eEnvType, dDistShear),3);
								dAxial  = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_ULTIMATE,ELE_AXFORCE,bGirderCalc,bOutSideWall, eEnvType),3);
							}
							
							m_MatFrame(nRow,nCol++) = dMoment;
							m_MatFrame(nRow,nCol++) = dShear;
							m_MatFrame(nRow,nCol++) = dAxial;
						}
						nRow++;	
					}
				}
			}
		}

		// 중간벽체
		for(nJijum=1; nJijum<m_pBri->m_nQtyJigan; nJijum++)
		{
			if(m_pBri->IsRcSlab()) break;
			if(nJijum == nJijumConnect) continue;
		
			long nPosEnd = 3;
			for(long nPos = 0; nPos<nPosEnd; nPos++)
			{
				double dDistShear = m_pBri->GetDistCheckShearWall(nJijum, nPos, NULL);

				for(long nSide=0; nSide<2; nSide++)
				{
					bOutSideWall = (nSide==0) ? TRUE : FALSE; // TRUE - 배면
					if(nPos==0 && bOutSideWall==FALSE) continue;
					// 휨부재로 검토면 모두 저장하고 압축부재로 검토면 하단 하나만 저장한다.
					if(m_pBri->m_nSelectInWallSectionDesign != 0 && (nPos != CHECK_POS_BOTTOM || bOutSideWall)) continue;
					nCol = 0;

					for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
					{
						ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
						if(m_pBri->IsValidComboEnvType(eEnvType) == FALSE)
							continue;

						BOOL bEarthQuake = (eEnvType == eEnvExtremeI || eEnvType == eEnvExtremeII);
						if(bEarthQuake)
						{
							dMoment = Round(m_pBri->GetOutValueWallEQ(nJijum,nPos,ELE_MOMENT2,bGirderCalc,bOutSideWall,eEnvType),3);
							dShear  = Round(m_pBri->GetOutValueWallEQ(nJijum,nPos,ELE_SHEAR_2,bGirderCalc,bOutSideWall,eEnvType, dDistShear),3);
							dAxial  = Round(m_pBri->GetOutValueWallEQ(nJijum,nPos,ELE_AXFORCE,bGirderCalc,bOutSideWall,eEnvType),3);
						}
						else
						{
							dMoment = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_ULTIMATE,ELE_MOMENT2,bGirderCalc,bOutSideWall,eEnvType),3);
							dShear  = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_ULTIMATE,ELE_SHEAR_2,bGirderCalc,bOutSideWall,eEnvType, dDistShear),3);
							dAxial  = Round(m_pBri->GetOutValueWall(nJijum,nPos,COMBO_ULTIMATE,ELE_AXFORCE,bGirderCalc,bOutSideWall,eEnvType),3);
						}
						m_MatFrame(nRow,nCol++) = dMoment;
						m_MatFrame(nRow,nCol++) = dShear;
						m_MatFrame(nRow,nCol++) = dAxial;
					}

					nRow++;	
				}
			}
		}

		if(m_pBri->m_bExistProtectionRiseFooting)
		{
			for(long iSE=iSTT; iSE<=iEND; ++iSE)
			{
				BOOL bStt = (iSE == iSTT);
				
				nCol = 0;
				for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
				{
					ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
					if(m_pBri->IsValidComboEnvType(eEnvType) == FALSE)
						continue;

					BOOL bEarthQuake = (eEnvType == eEnvExtremeI || eEnvType == eEnvExtremeII);
					if(bEarthQuake)
					{
						dMoment = Round(m_pBri->GetOutValuePRFEQ(bStt,ELE_MOMENT2, eEnvType),3);
						dShear  = Round(m_pBri->GetOutValuePRFEQ(bStt,ELE_SHEAR_2, eEnvType),3);
						dAxial  = Round(m_pBri->GetOutValuePRFEQ(bStt,ELE_AXFORCE, eEnvType),3);
					}
					else
					{
						dMoment = Round(m_pBri->GetOutValuePRF(bStt,COMBO_ULTIMATE,ELE_MOMENT2, eEnvType),3);
						dShear  = Round(m_pBri->GetOutValuePRF(bStt,COMBO_ULTIMATE,ELE_SHEAR_2, eEnvType),3);
						dAxial  = Round(m_pBri->GetOutValuePRF(bStt,COMBO_ULTIMATE,ELE_AXFORCE, eEnvType),3);
					}

					m_MatFrame(nRow,nCol++) = dMoment;
					m_MatFrame(nRow,nCol++) = dShear;
					m_MatFrame(nRow,nCol++) = dAxial;
				}
				nRow++;	
			}
		}		
	}

	//////////////////////////////////////////////////////////////////////////
	// 평상시 3D SHELL
	if(bShell)
	{
		nRow = 0;
		for(long iUD = iUPPER; iUD <= iLOWER; iUD++)
		{
			BOOL bUpperSlab = iUD == iUPPER ? TRUE : FALSE;
			if(iUD == iLOWER && m_pBri->IsBoxType() == FALSE) break;
			for(i=0; i<m_pBri->m_nQtyJigan; i++)
			{
				long nPosEnd = (i==m_pBri->m_nQtyJigan-1) ? 3 : 2;
				for(long nPos = 0; nPos<nPosEnd; nPos++)
				{
					nCol = 0;

					for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
					{
						ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
						if(m_pBri->IsValidComboEnvType(eEnvType) == FALSE)
							continue;
						if(eEnvType == eEnvExtremeI || eEnvType == eEnvExtremeII)
						{
							nCol += 3;
							continue;
						}

						dMoment = Round(m_pBri->GetOutValueSlabShell(i,nPos,COMBO_ULTIMATE,ELE_MOMENT2,bUpperSlab, eEnvType),3);
						dShear  = Round(m_pBri->GetOutValueSlabShell(i,nPos,COMBO_ULTIMATE,ELE_SHEAR_2,bUpperSlab, eEnvType),3);
						dAxial  = 0;
						if(nPos==0 && i>0)
						{
							double dMoment2 = Round(m_pBri->GetOutValueSlabShell(i-1,2,COMBO_ULTIMATE,ELE_MOMENT2,bUpperSlab, eEnvType),3);
							double dShear2  = Round(m_pBri->GetOutValueSlabShell(i-1,2,COMBO_ULTIMATE,ELE_SHEAR_2,bUpperSlab, eEnvType),3);
							dMoment = min(dMoment,dMoment2);
							dShear  = max(dShear ,dShear2);
						}
						m_MatShell(nRow,nCol++) = dMoment;
						m_MatShell(nRow,nCol++) = dShear;
						m_MatShell(nRow,nCol++) = dAxial;
					}

					nRow++;	
				}
			}
		}
		// 시종점벽체
		if(m_pBri->IsOutsideWall())
		{
			for(i=0; i<2; i++)
			{
				long nJijum  = (i==0) ? 0 : m_pBri->m_nQtyJigan;
				long nPosEnd = 3;
				for(long nPos = 0; nPos<nPosEnd; nPos++)
				{
					for(long nSide=0; nSide<2; nSide++)
					{
						bOutSideWall = (nSide==0) ? TRUE : FALSE; // TRUE - 배면
						if(nPos==0 && bOutSideWall==FALSE) continue;
						nCol = 0;

						for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
						{
							ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
							if(m_pBri->IsValidComboEnvType(eEnvType) == FALSE)
								continue;
							if(eEnvType == eEnvExtremeI || eEnvType == eEnvExtremeII)
							{
								nCol += 3;
								continue;
							}

							dMoment = Round(m_pBri->GetOutValueWallShell(nJijum,nPos,COMBO_ULTIMATE,ELE_MOMENT2,bGirderCalc,bOutSideWall,eEnvType),3);
							dShear  = Round(m_pBri->GetOutValueWallShell(nJijum,nPos,COMBO_ULTIMATE,ELE_SHEAR_2,bGirderCalc,bOutSideWall,eEnvType),3);
							dAxial  = Round(m_pBri->GetOutValueWallShell(nJijum,nPos,COMBO_ULTIMATE,ELE_AXFORCE,bGirderCalc,bOutSideWall,eEnvType),3);
							m_MatShell(nRow,nCol++) = dMoment;
							m_MatShell(nRow,nCol++) = dShear;
							m_MatShell(nRow,nCol++) = dAxial;
						}
						nRow++;	
					}
				}
			}
		}

		// 중간벽체
		for(nJijum=1; nJijum<m_pBri->m_nQtyJigan; nJijum++)
		{
			if(m_pBri->IsRcSlab()) break;
			if(nJijum == nJijumConnect) continue;

			long nPosEnd = 3;
			for(long nPos = 0; nPos<nPosEnd; nPos++)
			{
				for(long nSide=0; nSide<2; nSide++)
				{
					bOutSideWall = (nSide==0) ? TRUE : FALSE; // TRUE - 배면
					if(nPos==0 && bOutSideWall==FALSE) continue;
					// 휨부재로 검토면 모두 저장하고 압축부재로 검토면 하단 하나만 저장한다.
					if(m_pBri->m_nSelectInWallSectionDesign != 0 && (nPos != CHECK_POS_BOTTOM || bOutSideWall)) continue;

					nCol = 0;
					long nPos = CHECK_POS_BOTTOM;
					for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
					{
						ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
						if(m_pBri->IsValidComboEnvType(eEnvType) == FALSE)
							continue;
						if(eEnvType == eEnvExtremeI || eEnvType == eEnvExtremeII)
						{
							nCol += 3;
							continue;
						}

						dMoment = Round(m_pBri->GetOutValueWallShell(nJijum,nPos,COMBO_ULTIMATE,ELE_MOMENT2,bGirderCalc,bOutSideWall,eEnvType),3);
						dShear  = Round(m_pBri->GetOutValueWallShell(nJijum,nPos,COMBO_ULTIMATE,ELE_SHEAR_2,bGirderCalc,bOutSideWall,eEnvType),3);
						dAxial  = Round(m_pBri->GetOutValueWallShell(nJijum,nPos,COMBO_ULTIMATE,ELE_AXFORCE,bGirderCalc,bOutSideWall,eEnvType),3);
						m_MatShell(nRow,nCol++) = dMoment;
						m_MatShell(nRow,nCol++) = dShear;
						m_MatShell(nRow,nCol++) = dAxial;
					}

					nRow++;	
				}
			}
		}

		if(m_pBri->m_bExistProtectionRiseFooting)
		{
			for(long iSE=iSTT; iSE<=iEND; ++iSE)
			{
				BOOL bStt = (iSE == iSTT);
				nCol = 0;

				for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
				{
					ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
					if(m_pBri->IsValidComboEnvType(eEnvType) == FALSE)
						continue;
					if(eEnvType == eEnvExtremeI || eEnvType == eEnvExtremeII)
					{
						nCol += 3;
						continue;
					}

					dMoment = Round(m_pBri->GetOutValuePRFShell(bStt,COMBO_ULTIMATE,ELE_MOMENT2,eEnvType),3);
					dShear  = Round(m_pBri->GetOutValuePRFShell(bStt,COMBO_ULTIMATE,ELE_SHEAR_2,eEnvType),3);
					dAxial  = Round(m_pBri->GetOutValuePRFShell(bStt,COMBO_ULTIMATE,ELE_AXFORCE,eEnvType),3);
					m_MatShell(nRow,nCol++) = dMoment;
					m_MatShell(nRow,nCol++) = dShear;
					m_MatShell(nRow,nCol++) = dAxial;
				}
				nRow++;	
			}
		}
	}

	m_pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, (m_pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH) ? 1:0);
}

double CInputCheckGrid::GetDesignValue(long nRow, long nCol)
{
	BOOL bLsd = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();
	double Value1 = fabs(m_MatShell(nRow,nCol));
	double Value2 = fabs(m_MatFrame(nRow,nCol));
	double Value3 = bLsd? 0 : fabs(m_MatEQ(nRow,nCol));

	double MaxValue = max(Value1,max(Value2,Value3));

	if(Compare(fabs(Value1),MaxValue,"=")) return m_MatShell(nRow,nCol);
	if(Compare(fabs(Value2),MaxValue,"=")) return m_MatFrame(nRow,nCol);
	if(Compare(fabs(Value3),MaxValue,"=")) return m_MatEQ(nRow,nCol);

	return 0;
}

void CInputCheckGrid::SetGridTitle_WingWall()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pData->IsLsdDesign();
	if (bLsd)
	{
		SetGridTitle_WingWall_Lsd();
		return;
	}

	long nTab = m_ctlTab.GetCurSel();

	CARcBridgeCalcWing *pCalcWing = m_pCalc->m_pWing[m_nTypeCheck];
	long nSizeRow = (nTab==1)? 1:2;
	if(pCalcWing->m_bIsA) nSizeRow += 3;
	if(pCalcWing->m_bIsD) nSizeRow += 1;
	if(pCalcWing->m_bIsA_) nSizeRow += 1;
	if(pCalcWing->m_bIsA && !pCalcWing->m_bIsA_Calc) nSizeRow -= 1;

	long nSizeCol = 0;
	CString str = _T("");
	long nRow = 0;
	
	switch(nTab)
	{
	case 0:	nSizeCol = 10; break;
	case 1:	nSizeCol = 4; break;
	case 2:	nSizeCol = 14; break;
	case 3:	nSizeCol = 8; break;
	}
	
	m_Grid.SetColumnCount(nSizeCol);
	m_Grid.SetRowCount(nSizeRow);
	if(nTab == 1) m_Grid.SetFixedRowCount(1);
	else m_Grid.SetFixedRowCount(2);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(70);
	m_Grid.SetColumnWidth(0,120);
	
	m_Grid.SetTextMatrix(0,0,"구 분");
	if(nTab != 1) m_Grid.SetMergeRow(0,1,0);

	if(nTab == 0)
	{
		m_Grid.SetTextMatrix(0, 1, "철근비");
		m_Grid.SetMergeCol(0, 1, 5);
		m_Grid.SetTextMatrix(0, 6, "모멘트");
		m_Grid.SetMergeCol(0, 6, 9);
		m_Grid.SetTextMatrix(1, 1, "Req*4/3");
		m_Grid.SetTextMatrix(1, 2, "Pmin");
		m_Grid.SetTextMatrix(1, 3, "Puse");
		m_Grid.SetTextMatrix(1, 4, "Pmax");
		m_Grid.SetTextMatrix(1, 5, "비 고");
		m_Grid.SetTextMatrix(1, 6, "φMn");
		m_Grid.SetTextMatrix(1, 7, "Mu");
		m_Grid.SetTextMatrix(1, 8, "안전도");
		m_Grid.SetTextMatrix(1, 9, "비 고");
		
		nRow = 2;
	}
	else if(nTab == 1)
	{		
		if(pData->m_nTypeKciCode==DESIGN_CODE_KCI_2003)
		{
			m_Grid.SetTextMatrix(0, 1, "W");
			m_Grid.SetTextMatrix(0, 2, "Wa");
		}
		else
		{
			m_Grid.SetTextMatrix(0, 1, "S");
			m_Grid.SetTextMatrix(0, 2, "Sa");
		}

		m_Grid.SetTextMatrix(0, 3, "비 고");
		
		nRow = 1;
	}
	else if(nTab == 2)
	{
		m_Grid.SetTextMatrix(0, 1, "설계전단력");
		m_Grid.SetMergeCol(0, 1, 4);
		m_Grid.SetTextMatrix(0, 5, "최대전단강도");
		m_Grid.SetMergeCol(0, 5, 7);
		m_Grid.SetTextMatrix(0, 8, "전단철근 간격");
		m_Grid.SetMergeCol(0, 8, 10);
		m_Grid.SetTextMatrix(0, 11, "최소전단철근");
		m_Grid.SetMergeCol(0, 11, 13);
		
		m_Grid.SetTextMatrix(1, 1, "φVn");
		m_Grid.SetTextMatrix(1, 2, "φVc");
		m_Grid.SetTextMatrix(1, 3, "Vu");
		m_Grid.SetTextMatrix(1, 4, "비 고");
		m_Grid.SetTextMatrix(1, 5, "Vs_max");
		m_Grid.SetTextMatrix(1, 6, "Vs");
		m_Grid.SetTextMatrix(1, 7, "비 고");
		m_Grid.SetTextMatrix(1, 8, "S_max");
		m_Grid.SetTextMatrix(1, 9, "S");
		m_Grid.SetTextMatrix(1, 10, "비 고");
		m_Grid.SetTextMatrix(1, 11, "Req Av");
		m_Grid.SetTextMatrix(1, 12, "Use Av");
		m_Grid.SetTextMatrix(1, 13, "비 고");
	}
	else if(nTab == 3)
	{
		m_Grid.SetTextMatrix(0, 1, "주철근비/0.0015hb");
		m_Grid.SetMergeCol(0, 1, 2);
		m_Grid.SetTextMatrix(0, 3, "단면적비");
		m_Grid.SetMergeCol(0, 3, 4);
		m_Grid.SetTextMatrix(0, 5, "300㎟이상");
		m_Grid.SetMergeCol(0, 5, 6);
		m_Grid.SetTextMatrix(0, 7, "비고");
		m_Grid.SetMergeRow(0, 1, 7);
		
		m_Grid.SetTextMatrix(1, 1, "필요철근량");
		m_Grid.SetTextMatrix(1, 2, "사용철근량");
		m_Grid.SetTextMatrix(1, 3, "필요철근량");
		m_Grid.SetTextMatrix(1, 4, "사용철근량");
		m_Grid.SetTextMatrix(1, 5, "필요철근량");
		m_Grid.SetTextMatrix(1, 6, "사용철근량");	
	}
	
	nRow = 0;
	long n=0; for(n=0; n<5; n++)
	{
		if(!pCalcWing->m_bIsA && (n==0||n==1||n==2)) continue;
		else if(!pCalcWing->m_bIsA_Calc && n==0) continue;
		else if(!pCalcWing->m_bIsD && n==3) continue;
		else if(!pCalcWing->m_bIsA_ && n==4) continue;

		CConcBasic *pConc;
		if(m_nTypeCheck == 0) pConc = m_pBri->m_pArrConcWingSttLeft.GetAt(n);
		else if(m_nTypeCheck == 1) pConc = m_pBri->m_pArrConcWingSttRight.GetAt(n);
		else if(m_nTypeCheck == 2) pConc = m_pBri->m_pArrConcWingEndLeft.GetAt(n);
		else if(m_nTypeCheck == 3) pConc = m_pBri->m_pArrConcWingEndRight.GetAt(n);
		m_Grid.SetTextMatrix((nTab==1)? nRow+1:nRow+2, 0, pConc->m_sTitle);
		nRow ++;
	}
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputCheckGrid::SetDataInit_WingWall()
{
	long nTab = m_ctlTab.GetCurSel();

	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pData->IsLsdDesign();

	if (bLsd)
	{
		if(nTab == 0) SetDataInit_WingWall_Stress_Lsd();
		else if(nTab == 1) SetDataInit_WingWall_Use_Lsd();
		else if(nTab == 2) SetDataInit_WingWall_Support_Lsd();
		else if(nTab == 3) SetDataInit_WingWall_Shear_Lsd();
	}
	else
	{
		if(nTab == 0) SetDataInit_WingWall_Stress();
		else if(nTab == 1) SetDataInit_WingWall_Crack();
		else if(nTab == 2) SetDataInit_WingWall_Shear();
		else if(nTab == 3) SetDataInit_WingWall_Support();
	}
}

void CInputCheckGrid::SetDataInit_WingWall_Stress()
{
	CARcBridgeCalcWing *pCalcWing = m_pCalc->m_pWing[m_nTypeCheck];
	
	long nRow = 0;
	long nCol = 0;
	
	long n=0; for(n=0; n<5; n++)
	{
		if(!pCalcWing->m_bIsA && (n==0||n==1||n==2)) continue;
		else if(!pCalcWing->m_bIsA_Calc && n==0) continue;
		else if(!pCalcWing->m_bIsD && n==3) continue;
		else if(!pCalcWing->m_bIsA_ && n==4) continue;
		
		nCol = 1;
		CConcBasic *pConc;
		if(m_nTypeCheck == 0) pConc = m_pBri->m_pArrConcWingSttLeft.GetAt(n);
		else if(m_nTypeCheck == 1) pConc = m_pBri->m_pArrConcWingSttRight.GetAt(n);
		else if(m_nTypeCheck == 2) pConc = m_pBri->m_pArrConcWingEndLeft.GetAt(n);
		else if(m_nTypeCheck == 3) pConc = m_pBri->m_pArrConcWingEndRight.GetAt(n);
		
		CRectBeam pBeam(pConc, m_pBri->m_bCheckMainSlabAndFootTemper);
		
		double Pmin = pBeam.GetMinRebarRate();
		double Pmax = pBeam.GetMaxRebarRate();
		double Puse = pBeam.GetUseRebarRate();
		double AsReq = pBeam.GetRebarAreaRequired();
		double Preq4_3 = (AsReq / ((pConc->m_Ho-pConc->m_RbT1C_dc[0]) * pConc->m_Bo)) *4/3;

		BOOL bOkRate = ((Pmin < Puse || Preq4_3 < Puse) && Puse < Pmax)? TRUE:FALSE;
		CString szOK = (bOkRate)? "O.K":"N.G";
		
		m_Grid.SetTextMatrixFormat(nRow+2,nCol++,"%.5f", Preq4_3);
		m_Grid.SetTextMatrixFormat(nRow+2,nCol++,"%.5f", Pmin);
		m_Grid.SetTextMatrixFormat(nRow+2,nCol++,"%.5f", Puse);
		m_Grid.SetTextMatrixFormat(nRow+2,nCol++,"%.5f", Pmax);
		m_Grid.SetTextMatrix(nRow+2, nCol, szOK);
		if(!bOkRate) m_Grid.SetItemFgColour(nRow+2, nCol++, RGB(255, 0, 0));
		else m_Grid.SetItemFgColour(nRow+2, nCol++, RGB(0, 0, 0));
		
		
		double piMn = pBeam.GetPiMn();
		double Mu   = pBeam.GetMu();
		double SF   = pBeam.GetSafeFactorMoment();
		BOOL bOkMoment = pBeam.IsOkMoment();
		szOK = (bOkMoment)? "O.K":"N.G";
		
		m_Grid.SetTextMatrixFormat(nRow+2,nCol++,"%.3f", piMn);
		m_Grid.SetTextMatrixFormat(nRow+2,nCol++,"%.3f", Mu);
		m_Grid.SetTextMatrixFormat(nRow+2,nCol++,"%.3f", SF);
		m_Grid.SetTextMatrix(nRow+2,nCol, szOK);
		if(!bOkMoment) m_Grid.SetItemFgColour(nRow+2, nCol++, RGB(255, 0, 0));
		else m_Grid.SetItemFgColour(nRow+2, nCol++, RGB(0, 0, 0));
		nRow++;
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputCheckGrid::SetDataInit_WingWall_Crack()
{
	CARcBridgeCalcWing *pCalcWing = m_pCalc->m_pWing[m_nTypeCheck];
	
	long nRow = 0;
	long nCol = 0;
	
	long n=0; for(n=0; n<5; n++)
	{
		if(!pCalcWing->m_bIsA && (n==0||n==1||n==2)) continue;
		else if(!pCalcWing->m_bIsA_Calc && n==0) continue;
		else if(!pCalcWing->m_bIsD && n==3) continue;
		else if(!pCalcWing->m_bIsA_ && n==4) continue;
		
		nCol = 1;
		CConcBasic *pConc;
		if(m_nTypeCheck == 0) pConc = m_pBri->m_pArrConcWingSttLeft.GetAt(n);
		else if(m_nTypeCheck == 1) pConc = m_pBri->m_pArrConcWingSttRight.GetAt(n);
		else if(m_nTypeCheck == 2) pConc = m_pBri->m_pArrConcWingEndLeft.GetAt(n);
		else if(m_nTypeCheck == 3) pConc = m_pBri->m_pArrConcWingEndRight.GetAt(n);
		
		CRectBeam pBeam(pConc, m_pBri->m_bCheckMainSlabAndFootTemper);

		double Wa = pBeam.GetWa();
		double W  = pBeam.GetW();
		BOOL bOkCrack = (Wa > W)? TRUE:FALSE;
		CString szOK = (bOkCrack)? "O.K":"N.G";
		if(W > 0)
		{
			m_Grid.SetTextMatrixFormat(nRow+1,nCol++,"%.3f", W);
			m_Grid.SetTextMatrixFormat(nRow+1,nCol++,"%.3f", Wa);
			m_Grid.SetTextMatrix(nRow+1,nCol, szOK);
			if(!bOkCrack) m_Grid.SetItemFgColour(nRow+1, nCol++, RGB(255, 0, 0));
			else m_Grid.SetItemFgColour(nRow+1, nCol++, RGB(0, 0, 0));
		}
		else
		{
			m_Grid.SetTextMatrix(nRow+1,nCol++,"-");
			m_Grid.SetTextMatrix(nRow+1,nCol++,"-");
			m_Grid.SetTextMatrix(nRow+1,nCol, "-");
		}
		
		nRow++;
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputCheckGrid::SetDataInit_WingWall_Shear()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	CARcBridgeCalcWing *pCalcWing = m_pCalc->m_pWing[m_nTypeCheck];

	long nRow = 0;
	long nCol = 0;
	
	BOOL bCalcShear = FALSE;
	long n=0; for(n=0; n<5; n++)
	{
		if(!pCalcWing->m_bIsA && (n==0||n==1||n==2)) continue;
		else if(!pCalcWing->m_bIsA_Calc && n==0) continue;
		else if(!pCalcWing->m_bIsD && n==3) continue;
		else if(!pCalcWing->m_bIsA_ && n==4) continue;

		// (ARCBRIDGE-3162) 간격재 일때도 전단검토 출력
// 		if(n==0) bCalcShear = (pCalcWing->m_pWing->m_RebarShearDetailA.m_nType==TYPE_SHEAR_SPACER)? FALSE : TRUE;
// 		else if(n==1) bCalcShear = ((pCalcWing->m_pWing->m_nTypeShearBC==1)? pCalcWing->m_pWing->m_RebarShearDetailC.m_nType==TYPE_SHEAR_SPACER : pCalcWing->m_pWing->m_RebarShearDetailB.m_nType==TYPE_SHEAR_SPACER)? FALSE : TRUE;
// 		else if(n==2) bCalcShear = ((pCalcWing->m_pWing->m_nTypeShearBC==0)? pCalcWing->m_pWing->m_RebarShearDetailB.m_nType==TYPE_SHEAR_SPACER : pCalcWing->m_pWing->m_RebarShearDetailC.m_nType==TYPE_SHEAR_SPACER)? FALSE : TRUE;
// 		else if(n==3) bCalcShear = (pCalcWing->m_pWing->m_RebarShearDetailD.m_nType==TYPE_SHEAR_SPACER)? FALSE : TRUE;
// 		else if(n==4) bCalcShear = (pCalcWing->m_pWing->m_RebarShearDetailA.m_nType==TYPE_SHEAR_SPACER)? FALSE : TRUE;
// 		else bCalcShear = FALSE;
		bCalcShear = TRUE;
		
		long nIdx = n;
		if(n==4 && !pCalcWing->m_bIsD) nIdx = n -1;
		else if(n==3 && !pCalcWing->m_bIsA) nIdx = 0;
		else if(!pCalcWing->m_bIsA_Calc && pCalcWing->m_bIsA) nIdx -= 1;
		nCol = 1;
		BOOL bCheckMinAv = m_pBri->m_bApplyMinAvWing;
		CConcBasic *pConc;
		if(m_nTypeCheck == 0) pConc = m_pBri->m_pArrConcWingSttLeft.GetAt(n);
		else if(m_nTypeCheck == 1) pConc = m_pBri->m_pArrConcWingSttRight.GetAt(n);
		else if(m_nTypeCheck == 2) pConc = m_pBri->m_pArrConcWingEndLeft.GetAt(n);
		else if(m_nTypeCheck == 3) pConc = m_pBri->m_pArrConcWingEndRight.GetAt(n);

		CRectBeam pBeam(pConc, m_pBri->m_bCheckMainSlabAndFootTemper);

		double PiVn = pBeam.GetPiVn();
		double PiVc = pConc->m_Pi_V * pBeam.GetVc();		// Vc는 있는데 PiVc는 없네...
		double Vu = pBeam.GetVu();
		BOOL bOkShear = (PiVn > Vu)? TRUE:FALSE;
		CString szOK = (bOkShear)? "O.K":"N.G";
		
		if(bCalcShear)
		{
			m_Grid.SetTextMatrixFormat(nIdx+2,nCol++,"%.3f", PiVn);
			m_Grid.SetTextMatrixFormat(nIdx+2,nCol++,"%.3f", PiVc);
			m_Grid.SetTextMatrixFormat(nIdx+2,nCol++,"%.3f", Vu);
			m_Grid.SetTextMatrix(nIdx+2, nCol, szOK);
			if(!bOkShear) m_Grid.SetItemFgColour(nIdx+2, nCol++, RGB(255, 0, 0));
			else m_Grid.SetItemFgColour(nIdx+2, nCol++, RGB(0, 0, 0));
		}
		
		double Vsmax = pBeam.GetVsMax();
		double Vs   = pBeam.GetVs();
		BOOL bOkMaxShear = (Vsmax > Vs)? TRUE:FALSE;
		szOK = (bOkMaxShear)? "O.K":"N.G";

		long nCheckShearForce =  pBeam.GetCheckShearForce();
		
		if(nCheckShearForce != 6 && bCalcShear)
		{
			m_Grid.SetTextMatrixFormat(nIdx+2,nCol++,"%.3f", Vsmax);
			m_Grid.SetTextMatrixFormat(nIdx+2,nCol++,"%.3f", Vs);
			m_Grid.SetTextMatrix(nIdx+2,nCol, szOK);
			if(!bOkMaxShear) m_Grid.SetItemFgColour(nIdx+2, nCol++, RGB(255, 0, 0));
			else m_Grid.SetItemFgColour(nIdx+2, nCol++, RGB(0, 0, 0));
			
			double Smax = pBeam.GetsMax();
			double S    = pData->IsSI() ? pConc->m_SvCtc : toCm(pConc->m_SvCtc);
			BOOL bOkShearCtc = (Smax >= S)? TRUE:FALSE;
			szOK = (bOkShearCtc)? "O.K":"N.G";
			
			if(pConc->m_bCheckMinCtcShear)
			{
				m_Grid.SetTextMatrixFormat(nIdx+2,nCol++,"%.3f", Smax);
				m_Grid.SetTextMatrixFormat(nIdx+2,nCol++,"%.3f", S);
				m_Grid.SetTextMatrix(nIdx+2,nCol, szOK);
			}
			else
			{
				m_Grid.SetTextMatrix(nIdx+2,nCol++,"-");
				m_Grid.SetTextMatrix(nIdx+2,nCol++,"-");
				m_Grid.SetTextMatrix(nIdx+2,nCol, "-");
			}
			if(!bOkShearCtc && pConc->m_bMinAvCheck) 
				m_Grid.SetItemFgColour(nIdx+2, nCol++, RGB(255, 0, 0));
			else 
				m_Grid.SetItemFgColour(nIdx+2, nCol++, RGB(0, 0, 0));
			
			if(bCheckMinAv)
			{
				double ReqAv = 0;
				if(nCheckShearForce == 1 || nCheckShearForce == 2)
					ReqAv = pBeam.GetMinAv();
				else
					ReqAv = pBeam.GetAvReq();
				double UseAv = pBeam.GetAvUse();
				BOOL bOkMinShear = (ReqAv < UseAv)? TRUE:FALSE;
				szOK = (bOkMinShear)? "O.K":"N.G";
				
				m_Grid.SetTextMatrixFormat(nIdx+2,nCol++,"%.3f", ReqAv);
				m_Grid.SetTextMatrixFormat(nIdx+2,nCol++,"%.3f", UseAv);
				m_Grid.SetTextMatrix(nIdx+2,nCol, szOK);
				if(!bOkMinShear) m_Grid.SetItemFgColour(nIdx+2, nCol++, RGB(255, 0, 0));
				else m_Grid.SetItemFgColour(nIdx+2, nCol++, RGB(0, 0, 0));
			}
		}
		nRow++;
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputCheckGrid::SetDataInit_WingWall_Support()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	CARcBridgeCalcWing *pCalcWing = m_pCalc->m_pWing[m_nTypeCheck];
	
	long nRow = 0;
	long nCol = 0;
	
	long n=0; for(n=0; n<5; n++)
	{
		if(!pCalcWing->m_bIsA && (n==0||n==1||n==2)) continue;
		else if(!pCalcWing->m_bIsA_Calc && n==0) continue;
		else if(!pCalcWing->m_bIsD && n==3) continue;
		else if(!pCalcWing->m_bIsA_ && n==4) continue;
		
		long nIdx = n;
		if(n==4 && !pCalcWing->m_bIsD) nIdx = n -1;
		else if(n==3 && !pCalcWing->m_bIsA) nIdx = 0;
		else if(!pCalcWing->m_bIsA_Calc && pCalcWing->m_bIsA) nIdx -= 1;

		nCol = 1;
		CConcBasic *pConc;
		if(m_nTypeCheck == 0) pConc = m_pBri->m_pArrConcWingSttLeft.GetAt(n);
		else if(m_nTypeCheck == 1) pConc = m_pBri->m_pArrConcWingSttRight.GetAt(n);
		else if(m_nTypeCheck == 2) pConc = m_pBri->m_pArrConcWingEndLeft.GetAt(n);
		else if(m_nTypeCheck == 3) pConc = m_pBri->m_pArrConcWingEndRight.GetAt(n);

		double AsReq = 0;
		double AsUse = 0;

		BOOL bCheckMain = m_pBri->m_bCheckSuppoerBarWing15hs;
		BOOL bCheckMainOver20 = FALSE;
		BOOL bCheckSectA = m_pBri->m_bCheckSuppoerBarWingArea;
		BOOL bCheckOver300 = m_pBri->m_bCheckSuppoerBarWing300;

		long i=0; for(i=0; i<4; i++)
		{
			if(i == 0 && !bCheckMain) { nCol += 2; continue;}
			if(i == 1 && !bCheckMainOver20) { continue;}
			if(i == 2 && !bCheckSectA) { nCol += 2; continue;}
			if(i == 3 && !bCheckOver300) { nCol += 2; continue;}

			AsReq = m_pCalc->m_pCorner[iUPPER]->GetDoubleCheckConcWall(pConc, TRUE, i, TRUE);
			AsUse = m_pCalc->m_pCorner[iUPPER]->GetDoubleCheckConcWall(pConc, FALSE, i, TRUE);
			
			if(pData->IsSI())
			{
				m_Grid.SetTextMatrixFormat(nIdx+2,nCol++,"%.1f", AsReq);
				m_Grid.SetTextMatrixFormat(nIdx+2,nCol++,"%.1f", AsUse);
			}
			else
			{
				m_Grid.SetTextMatrixFormat(nIdx+2,nCol++,"%.3f", AsReq);
				m_Grid.SetTextMatrixFormat(nIdx+2,nCol++,"%.3f", AsUse);
			}	
		}
		
		CString szOK = m_pCalc->m_pCorner[iUPPER]->GetStringCheckConcWingWall(pConc, TRUE, 3, FALSE);
		
		m_Grid.SetTextMatrix(nIdx+2, nCol, szOK);
		if(szOK != "O.K") m_Grid.SetItemFgColour(nIdx+2, nCol++, RGB(255, 0, 0));
		else m_Grid.SetItemFgColour(nIdx+2, nCol++, RGB(0, 0, 0));

		nRow++;
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputCheckGrid::SetGridTitle_WingWall_Lsd()
{
	long nTab = m_ctlTab.GetCurSel();
	
	CARcBridgeCalcWing *pCalcWing = m_pCalc->m_pWing[m_nTypeCheck];
	long nSizeRow = 2;
	if(pCalcWing->m_bIsA) nSizeRow += 3;
	if(pCalcWing->m_bIsD) nSizeRow += 1;
	if(pCalcWing->m_bIsA_) nSizeRow += 1;
	if(pCalcWing->m_bIsA && !pCalcWing->m_bIsA_Calc) nSizeRow -= 1;

	long nSizeCol = 0;
	switch(nTab)
	{
	case 0:	nSizeCol = 11; break;
	case 1:	nSizeCol = 12; break;
	case 2:	nSizeCol = 7; break;
	case 3:	nSizeCol = 13; break;
	}

	m_Grid.SetColumnCount(nSizeCol);
	m_Grid.SetRowCount(nSizeRow);
	m_Grid.SetFixedRowCount(2);
	if(nTab == 2) m_Grid.SetFixedColumnCount(1);
	else m_Grid.SetFixedColumnCount(2);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(70);
	m_Grid.SetColumnWidth(0,120);
	if(nTab == 0)
	{
		m_Grid.SetColumnWidth(6,100);
		m_Grid.SetColumnWidth(7,100);
	}

	m_Grid.SetTextMatrix(0,0,"구 분");
	if (nTab != 2)
	{
		m_Grid.SetMergeCol(0,0,1);
		m_Grid.SetMergeCol(1,0,1);
		m_Grid.SetMergeRow(0,1,1);
	}
	
	m_Grid.SetMergeRow(0,1,0);

	if(nTab == 0)
	{
		m_Grid.SetTextMatrix(0, 2, "철근비");
		m_Grid.SetMergeCol(0, 2, 5);
		m_Grid.SetTextMatrix(0, 6, "모멘트");
		m_Grid.SetMergeCol(0, 6, 10);
		
		m_Grid.SetTextMatrix(1, 2, "As,min");
		m_Grid.SetTextMatrix(1, 3, "As,use");
		m_Grid.SetTextMatrix(1, 4, "As,max");
		m_Grid.SetTextMatrix(1, 5, "비고");
		m_Grid.SetTextMatrix(1, 6, "Mr");
		m_Grid.SetTextMatrix(1, 7, "Mu");
		m_Grid.SetTextMatrix(1, 8, "안전도");
		m_Grid.SetTextMatrix(1, 9, "비고");
		m_Grid.SetTextMatrix(1, 10, "추가인장력");
	}
	else if(nTab == 1)
	{		
		m_Grid.SetTextMatrix(0, 2, "균열");
		m_Grid.SetMergeRow(0, 1, 2);
		m_Grid.SetTextMatrix(0, 3, "최소철근량");
		m_Grid.SetMergeCol(0, 3, 5);
		m_Grid.SetTextMatrix(0, 6, "간접균열검토");
		m_Grid.SetMergeCol(0, 6, 8);
		m_Grid.SetTextMatrix(0, 9, "균열폭 검토");
		m_Grid.SetMergeCol(0, 9, 11);

		m_Grid.SetTextMatrix(1, 3, "As,min");
		m_Grid.SetTextMatrix(1, 4, "As,use");
		m_Grid.SetTextMatrix(1, 5, "비고");
		m_Grid.SetTextMatrix(1, 6, "fs,max");
		m_Grid.SetTextMatrix(1, 7, "fs");
		m_Grid.SetTextMatrix(1, 8, "비고");
		m_Grid.SetTextMatrix(1, 9, "W k");
		m_Grid.SetTextMatrix(1, 10, "Wa");
		m_Grid.SetTextMatrix(1, 11, "비고");
	}
	else if(nTab == 2)
	{
		m_Grid.SetTextMatrix(0, 1, "수평방향");
		m_Grid.SetMergeCol(0, 1, 3);
		m_Grid.SetTextMatrix(0, 4, "수직방향");
		m_Grid.SetMergeCol(0, 4, 6);
		
		m_Grid.SetTextMatrix(1, 1, "필요철근량");
		m_Grid.SetTextMatrix(1, 2, "사용철근량");
		m_Grid.SetTextMatrix(1, 3, "비고");
		m_Grid.SetTextMatrix(1, 4, "필요철근량");
		m_Grid.SetTextMatrix(1, 5, "사용철근량");
		m_Grid.SetTextMatrix(1, 6, "비고");
	}
	else if(nTab == 3)
	{
		m_Grid.SetTextMatrix(0, 2, "철근비");
		m_Grid.SetMergeCol(0, 2, 6);
		m_Grid.SetTextMatrix(0, 7, "최소전단철근");
		m_Grid.SetMergeCol(0, 7, 9);
		m_Grid.SetTextMatrix(0, 10, "전단철근간격");
		m_Grid.SetMergeCol(0, 10, 12);

		m_Grid.SetTextMatrix(1, 2, "Vcd");
		m_Grid.SetTextMatrix(1, 3, "Vsd");
		m_Grid.SetTextMatrix(1, 4, "Θ");
		m_Grid.SetTextMatrix(1, 5, "Vu");
		m_Grid.SetTextMatrix(1, 6, "비고");
		m_Grid.SetTextMatrix(1, 7, "pv,min");
		m_Grid.SetTextMatrix(1, 8, "pv,use");
		m_Grid.SetTextMatrix(1, 9, "비고");
		m_Grid.SetTextMatrix(1, 10, "S_max");
		m_Grid.SetTextMatrix(1, 11, "S");
		m_Grid.SetTextMatrix(1, 12, "비고");
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputCheckGrid::SetDataInit_WingWall_Stress_Lsd()
{
	CARcBridgeCalcWing *pCalcWing = m_pCalc->m_pWing[m_nTypeCheck];
	const HGBOOL bLsd = TRUE;

	long nRow = 1;

	long nSizeRow = 2;
	if(pCalcWing->m_bIsA) nSizeRow += 3;
	if(pCalcWing->m_bIsD) nSizeRow += 1;
	if(pCalcWing->m_bIsA_) nSizeRow += 1;
	if(pCalcWing->m_bIsA && !pCalcWing->m_bIsA_Calc) nSizeRow -= 1;

	m_Grid.SetMergeRow(2, nSizeRow-1, 0);

	long n=0; for(n=0; n<5; n++)
	{
		if(!pCalcWing->m_bIsA && (n==0||n==1||n==2))
			continue;
		else if(!pCalcWing->m_bIsA_Calc && n==0)
			continue;
		else if(!pCalcWing->m_bIsD && n==3)
			continue;
		else if(!pCalcWing->m_bIsA_ && n==4)
			continue;

		++nRow;
		long nCol = 0;
		
		CString strEnv = m_pBri->GetStringComboEnvType_Lsd(eEnvLimit);
		m_Grid.SetTextMatrix(nRow, nCol++, strEnv);

		CConcBasic *pConc = NULL;
		if(m_nTypeCheck == 0) pConc = m_pBri->m_pArrConcWingSttLeft.GetAt(n);
		else if(m_nTypeCheck == 1) pConc = m_pBri->m_pArrConcWingSttRight.GetAt(n);
		else if(m_nTypeCheck == 2) pConc = m_pBri->m_pArrConcWingEndLeft.GetAt(n);
		else if(m_nTypeCheck == 3) pConc = m_pBri->m_pArrConcWingEndRight.GetAt(n);

		m_Grid.SetTextMatrix(nRow, nCol++, pConc->m_sTitle);
		
		CLsdBeam pBeam(pConc, LSD_ULS_DESIGN);
		SetDataInit_Stress_Lsd(pBeam, nRow, nCol);
	}
}

void CInputCheckGrid::SetDataInit_WingWall_Use_Lsd()
{
	CARcBridgeCalcWing *pCalcWing = m_pCalc->m_pWing[m_nTypeCheck];
	const HGBOOL bLsd = TRUE;

	long ixRow = 1;

	long nSizeRow = 2;
	if(pCalcWing->m_bIsA) nSizeRow += 3;
	if(pCalcWing->m_bIsD) nSizeRow += 1;
	if(pCalcWing->m_bIsA_) nSizeRow += 1;
	if(pCalcWing->m_bIsA && !pCalcWing->m_bIsA_Calc) nSizeRow -= 1;

	m_Grid.SetMergeRow(2, nSizeRow-1, 0);

	long n=0; for(n=0; n<5; n++)
	{
		if(!pCalcWing->m_bIsA && (n==0||n==1||n==2))
			continue;
		else if(!pCalcWing->m_bIsA_Calc && n==0)
			continue;
		else if(!pCalcWing->m_bIsD && n==3)
			continue;
		else if(!pCalcWing->m_bIsA_ && n==4)
			continue;

		++ixRow;
		long ixCol = 0;

		CString strEnv = m_pBri->GetStringComboEnvType_Lsd(eEnvService);
		m_Grid.SetTextMatrix(ixRow, ixCol++, strEnv);

		CConcBasic *pConc = NULL;
		if(m_nTypeCheck == 0) pConc = m_pBri->m_pArrConcWingSttLeft.GetAt(n);
		else if(m_nTypeCheck == 1) pConc = m_pBri->m_pArrConcWingSttRight.GetAt(n);
		else if(m_nTypeCheck == 2) pConc = m_pBri->m_pArrConcWingEndLeft.GetAt(n);
		else if(m_nTypeCheck == 3) pConc = m_pBri->m_pArrConcWingEndRight.GetAt(n);

		m_Grid.SetTextMatrix(ixRow, ixCol++, pConc->m_sTitle);

		HGBOOL bNoneCheck(FALSE);
		HGBOOL bStt = m_nTypeCheck==0 || m_nTypeCheck==1;
		HGBOOL bUseWingCrack = m_pBri->IsUseSelectLoadCaseWingOrApsMatchDesignRating(pConc->m_strDesignDegree , bStt? ePartWingWallStt : ePartWingWallEnd);
		if(m_pBri->IsCheckPosEtaUseLoad(bStt? ePartWingWallStt : ePartWingWallEnd) == FALSE || bUseWingCrack == FALSE)
		{
			bNoneCheck = TRUE;
		}

		if(bNoneCheck)
		{
			for(long ix=0; ix<10; ++ix)
				m_Grid.SetTextMatrix(ixRow, ixCol++, _T("-"));
		}
		else
		{
			CLsdBeam pBeam(pConc, LSD_ULS_DESIGN);
			SetDataInit_Use_Lsd(pBeam, ixRow, ixCol);
		}
	}
}

void CInputCheckGrid::SetDataInit_WingWall_Shear_Lsd()
{
	CARcBridgeCalcWing *pCalcWing = m_pCalc->m_pWing[m_nTypeCheck];
	const HGBOOL bLsd = TRUE;

	HGINT32 ixRow = 1;
	HGINT32 ixCol = 0;

	long nSizeRow = 2;
	if(pCalcWing->m_bIsA) nSizeRow += 3;
	if(pCalcWing->m_bIsD) nSizeRow += 1;
	if(pCalcWing->m_bIsA_) nSizeRow += 1;
	if(pCalcWing->m_bIsA && !pCalcWing->m_bIsA_Calc) nSizeRow -= 1;

	m_Grid.SetMergeRow(2, nSizeRow-1, 0);

	long n=0; for(n=0; n<5; n++)
	{
		if(!pCalcWing->m_bIsA && (n==0||n==1||n==2))
			continue;
		else if(!pCalcWing->m_bIsA_Calc && n==0)
			continue;
		else if(!pCalcWing->m_bIsD && n==3)
			continue;
		else if(!pCalcWing->m_bIsA_ && n==4)
			continue;

		++ixRow;
		ixCol = 0;

		CString strEnv = m_pBri->GetStringComboEnvType_Lsd(eEnvLimit);
		m_Grid.SetTextMatrix(ixRow, ixCol++, strEnv);

		CConcBasic *pConc = NULL;
		if(m_nTypeCheck == 0) pConc = m_pBri->m_pArrConcWingSttLeft.GetAt(n);
		else if(m_nTypeCheck == 1) pConc = m_pBri->m_pArrConcWingSttRight.GetAt(n);
		else if(m_nTypeCheck == 2) pConc = m_pBri->m_pArrConcWingEndLeft.GetAt(n);
		else if(m_nTypeCheck == 3) pConc = m_pBri->m_pArrConcWingEndRight.GetAt(n);

		m_Grid.SetTextMatrix(ixRow, ixCol++, pConc->m_sTitle);

		CLsdBeam pBeam(pConc, LSD_ULS_DESIGN);
		SetDataInit_Shear_Lsd(pBeam, ixRow, ixCol);
	}
}

void CInputCheckGrid::SetDataInit_WingWall_Support_Lsd()
{
	CARcBridgeCalcWing *pCalcWing = m_pCalc->m_pWing[m_nTypeCheck];
	const HGBOOL bLsd = TRUE;

	HGINT32 ixRow = 1;
	HGINT32 ixCol = 0;

	long n=0; for(n=0; n<5; n++)
	{
		if(!pCalcWing->m_bIsA && (n==0||n==1||n==2))
			continue;
		else if(!pCalcWing->m_bIsA_Calc && n==0)
			continue;
		else if(!pCalcWing->m_bIsD && n==3)
			continue;
		else if(!pCalcWing->m_bIsA_ && n==4)
			continue;

		++ixRow;
		ixCol = 0;

		CConcBasic *pConc = NULL;
		if(m_nTypeCheck == 0) pConc = m_pBri->m_pArrConcWingSttLeft.GetAt(n);
		else if(m_nTypeCheck == 1) pConc = m_pBri->m_pArrConcWingSttRight.GetAt(n);
		else if(m_nTypeCheck == 2) pConc = m_pBri->m_pArrConcWingEndLeft.GetAt(n);
		else if(m_nTypeCheck == 3) pConc = m_pBri->m_pArrConcWingEndRight.GetAt(n);

		m_Grid.SetTextMatrix(ixRow, ixCol++, pConc->m_sTitle);

		CLsdBeam pBeam(pConc, LSD_ULS_DESIGN);
		SetDataInit_Support_Lsd(pBeam, ixRow, ixCol);
	}
}

void CInputCheckGrid::SetBriAndContent(CRcBridgeRebar *pBri, long nInputIDD, long nTypeCheck)
{
	m_pBri		 = pBri;
	m_nInputIDD  = nInputIDD;
	m_nTypeCheck = nTypeCheck;
	m_pCalc		 = new CARcBridgeCalcStd(pBri);

	CalculateBridge();
}

void CInputCheckGrid::ShowAndCheck(CWnd *pWndParent)
{
	BOOL bLsd = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();

	m_bOK	= TRUE;

	if(!GetSafeHwnd())	
		Create(IDD, pWndParent);
	
	if(m_ctlTab.GetItemCount()>0 && !IsWindowVisible())	m_ctlTab.SetCurSel(0);
	
	int nTab = m_ctlTab.GetCurSel();	

	ShowDlg();

	m_ctlTab.SetCurSel(nTab);

	ShowWindow(SW_SHOW);

	BOOL bStm = (m_nInputIDD == IDD_STM_ARC_CORNER || m_nInputIDD == IDD_STM_BRACKET || m_nInputIDD == IDD_STM_FOORING_MID || m_nInputIDD == IDD_STM_FOORING_STARTEND);
	if(bStm) //최대유효폭 산정불가 부재 체크를 위해 절점영역 강도검토 호출
	{
		if(bLsd)
			SetDataInit_STM_NodalZone_Lsd();
		else
			SetDataInit_STM_NodalZone();
	}

	SetDataInit();
}

void CInputCheckGrid::Check()
{
	if(!GetSafeHwnd()) return;
	SetDataInit();
}

void CInputCheckGrid::SetGridTitle_Column()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pData->IsLsdDesign();
	if (bLsd)
	{
		SetGridTitle_Column_Lsd();
		return;
	}

	long nSizeRow = 2 + 4; // RCBRIDGE_COL_CHECK - CGirderAndColumn;
	if(m_pBri->GetTypeDesingEQ() == 0)
		nSizeRow -= 1;
	long nSizeCol = 10;
	CString str = _T("");
	
	m_Grid.SetColumnCount(nSizeCol);
	m_Grid.SetRowCount(nSizeRow);
	m_Grid.SetFixedRowCount(2);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(70);
	
	m_Grid.SetTextMatrix(0,0,"구 분");
	
	m_Grid.SetTextMatrix(1, 1, "Pmin");
	m_Grid.SetTextMatrix(1, 2, "Puse");
	m_Grid.SetTextMatrix(1, 3, "Pmax");
	m_Grid.SetTextMatrix(1, 4, "비 고");
	//
	m_Grid.SetTextMatrix(1, 5, "ΦPn");
	m_Grid.SetTextMatrix(1, 6, "Pu");
	m_Grid.SetTextMatrix(1, 7, "Mu");
	m_Grid.SetTextMatrix(1, 8, "안전도");
	m_Grid.SetTextMatrix(1, 9, "비 고");
	
	m_Grid.SetTextMatrix(0, 1, "철근비");
	m_Grid.SetTextMatrix(0, 5, "축방향력,휨");

	m_Grid.SetMergeCol(0, 1, 4);
	m_Grid.SetMergeCol(0, 5, 9);
	m_Grid.SetMergeRow(0, 1, 0);
	
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputCheckGrid::SetDataInit_Column()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pData->IsLsdDesign();
	if (bLsd)
	{
		SetDataInit_Column_Lsd();
		return;
	}

	CGirderAndColumn *pCalCol = NULL;
	long nSizeList = m_pCalc->m_pColGir->m_pList.GetSize();
	long i=0; for(i=0; i<nSizeList; i++)
	{
		CGirderAndColumn *pCalColCur = m_pCalc->m_pColGir->m_pList.GetAt(i);
		if(pCalColCur->m_nJijum == m_nTypeCheck)
		{
			pCalCol = pCalColCur;
			break;
		}
	}
	if(pCalCol == NULL) return;

	CString str = _T(""), strCombo = _T("");

	long nRow = 2;
	for(i=0; i<4; i++) // RCBRIDGE_COL_CHECK - CGirderAndColumn;
	{
		if(pCalCol->m_nTypeDesign[i] < 0)
			continue;
	
		CConcBasic *pConc = pCalCol->pConcColumn[i];
		CColumn ColCalc(pConc, FALSE, m_pBri->m_nSelectPminColumn);
		ColCalc.CalculateAll(FALSE);
		str = _T("");
		long j=0; for(j=0; j<10; j++)
		{
			if(j==0)
			{
				if(pCalCol->m_nTypeDesign[i]==COMBO_ULTIMATE)	 str.Format("UL%02d",pCalCol->m_nCombo[i]+1);
				if(pCalCol->m_nTypeDesign[i]==COMBO_ECCENTRIC)	 str.Format("UE%02d",pCalCol->m_nCombo[i]+1);
				if(pCalCol->m_nTypeDesign[i]==COMBO_EARTHQUAKE)	 str.Format("EQ%02d",pCalCol->m_nCombo[i]+1);
				if(pCalCol->m_nTypeDesign[i]==COMBO_EARTHQUAKE2) str.Format("EQ%02d",pCalCol->m_nCombo[i]+1);
			}
			BOOL bWall = pConc->IsWall();
			if(j==1) str.Format(bWall ? "%.4f" : "%.3f",ColCalc.GetValuePmin());
			if(j==2) str.Format("%.3f",ColCalc.GetRebarUsedRate());
			if(j==3) str.Format("%.3f",MaxRebar_Column);
			if(j==4) str.Format("%s",ColCalc.GetCheckRebarRate()==1 ? "O.K" : "N.G");
			if(j==5) str.Format("%.3f",GetValueUnitChange(ColCalc.GetPiPn(),UNIT_CGS_TONF,pConc->m_szTonf));
			if(j==6) str.Format("%.3f",GetValueUnitChange(pConc->m_Axial,UNIT_CGS_TONF,pConc->m_szTonf));
			if(j==7) str.Format("%.3f",GetValueUnitChange(pConc->m_Moment,UNIT_CGS_TONFM,pConc->m_szTonfM));
			if(j==8) str.Format("%.3f",ColCalc.GetSafeFactorAxial());
			if(j==9) str.Format("%s",ColCalc.IsOkAxial() ? "O.K" : "N.G");
			
			m_Grid.SetTextMatrix(nRow+i,j,str);
			if(str=="N.G") 
				m_Grid.SetItemFgColour(nRow+i, j, RGB(255, 0, 0));
			else 
				m_Grid.SetItemFgColour(nRow+i, j, RGB(0, 0, 0));
		}
	}
}

void CInputCheckGrid::SetGridTitle_Column_Lsd()
{
	CGirderAndColumn *pCalCol = NULL;
	long nSizeList = m_pCalc->m_pColGir->m_pList.GetSize();
	long i=0; for(i=0; i<nSizeList; i++)
	{
		CGirderAndColumn *pCalColCur = m_pCalc->m_pColGir->m_pList.GetAt(i);
		if(pCalColCur->m_nJijum == m_nTypeCheck)
		{
			pCalCol = pCalColCur;
			break;
		}
	}

	if(pCalCol == NULL)
		return;

	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	const HGINT32 icValidEnvType = m_pBri->GetCountValidComboEnvType();
	
	long nSizeRow = icValidEnvType * eCountWallCheck + 1; 
	long nSizeCol = 10;
	CString str = _T("");

	m_Grid.SetColumnCount(nSizeCol);
	m_Grid.SetRowCount(nSizeRow);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(2);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(70);
	m_Grid.SetColumnWidth(1, 100);

	m_Grid.SetTextMatrix(0,0,"구 분");
	m_Grid.SetMergeCol(0, 0, 1);
	m_Grid.SetTextMatrix(0, 2, "As,min");
	m_Grid.SetTextMatrix(0, 3, "As,use");
	m_Grid.SetTextMatrix(0, 4, "As,Pmax");
	str.Format("Pu(%s)", pData->m_szTonf);
	m_Grid.SetTextMatrix(0, 5, str);
	str.Format("Pn(%s)", pData->m_szTonf);
	m_Grid.SetTextMatrix(0, 6, str);
	str.Format("Mu(%s)", pData->m_szTonfM);
	m_Grid.SetTextMatrix(0, 7, str);
	m_Grid.SetTextMatrix(0, 8, "안전도");
	m_Grid.SetTextMatrix(0, 9, "비고");

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputCheckGrid::SetDataInit_Column_Lsd()
{
	CGirderAndColumn *pCalCol = NULL;
	long nSizeList = m_pCalc->m_pColGir->m_pList.GetSize();
	long i=0; for(i=0; i<nSizeList; i++)
	{
		CGirderAndColumn *pCalColCur = m_pCalc->m_pColGir->m_pList.GetAt(i);
		if(pCalColCur->m_nJijum == m_nTypeCheck)
		{
			pCalCol = pCalColCur;
			break;
		}
	}

	if(pCalCol == NULL)
		return;
	
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = TRUE;
	const HGINT32 iJijum = m_nTypeCheck;

	HGINT32 iRow = 0;
	for(long ixEnvType=0; ixEnvType<eEnvService; ++ixEnvType)
	{
		ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ixEnvType;
		if(m_pBri->IsValidComboEnvType(eEnvType) == FALSE)
			continue;
		
		CString strEnv = ixEnvType == eEnvLimit ? "극한" : ixEnvType == eEnvExtremeI ? "극단1" : "극단2";

		m_Grid.SetMergeRow(iRow + 1, iRow + eCountWallCheck, 0);
		

		for(long iCheck=0; iCheck<eCountWallCheck; ++iCheck)
		{
			EWallCheckLsd eWallCheck = static_cast<EWallCheckLsd>(iCheck);
			CString strCheck = m_pBri->GetStringWallCheckCase(eWallCheck);

			CConcBasic *pConc = pCalCol->pConcColumnLsd[ixEnvType][iCheck];

			CColumn ColCalc(pConc, FALSE, m_pBri->m_nSelectPminColumn);
			ColCalc.CalculateAll(FALSE);
			
			HGINT32 iCol = 0;
			m_Grid.SetTextMatrix(++iRow, iCol++, strEnv);
			m_Grid.SetTextMatrix(iRow, iCol++, strCheck);
			CString str = GetStrDouble(ColCalc.GetValuePmin(), 5, bLsd);
			m_Grid.SetTextMatrix(iRow, iCol++, str);
			str = GetStrDouble(ColCalc.GetRebarUsedRate(), 5, bLsd);
			m_Grid.SetTextMatrix(iRow, iCol++, str);
			str = GetStrDouble(ColCalc.GetValuePmax(), 5, bLsd);
			m_Grid.SetTextMatrix(iRow, iCol++, str);
			str = pData->GetStrDoubleUnitChange(ColCalc.m_pData->m_Axial, UNIT_CGS_TONF, pData->m_szTonf);
			m_Grid.SetTextMatrix(iRow, iCol++, str);
			str = pData->GetStrDoubleUnitChange(ColCalc.GetPiPn(), UNIT_CGS_TONF, pData->m_szTonf);
			m_Grid.SetTextMatrix(iRow, iCol++, str);
			str = pData->GetStrDoubleUnitChange(ColCalc.m_pData->m_Moment, UNIT_CGS_TONFM, pData->m_szTonfM);
			m_Grid.SetTextMatrix(iRow, iCol++, str);
			str = GetStrDouble(ColCalc.GetPiPn()/ColCalc.m_pData->m_Axial, 3, bLsd);
			m_Grid.SetTextMatrix(iRow, iCol++, str);
			str = (ColCalc.IsOkAxial() && ColCalc.IsOK_RebarRate()) ? _T("O.K") : _T("N.G");
			m_Grid.SetTextMatrix(iRow, iCol++, str);
		}
	}
}

void CInputCheckGrid::OnOK() 
{
	// TODO: Add extra validation here
	SetDataSave();
	
	CXTResizeDialog::OnOK();
}

void CInputCheckGrid::SetDataSave()
{
	UpdateData(TRUE);

	BOOL bLsd = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();
	long nTab = m_ctlTab.GetCurSel();
	if((m_nInputIDD==IDD_GEN_FRAME_ANLYSIS || m_nInputIDD==IDD_GEN_SHELL_ANLYSIS) && nTab==(bLsd? 2 : 3))
	{
		m_pBri->m_bUseShellUserOutput = m_bUseUserOutput;
		if(m_pBri->m_bUseShellUserOutput)
		{
			SetDataSave_3DOutput();
		}
	}
	if(m_nInputIDD==IDD_GEN_WING_SHELL_ANLYSIS)
	{
		BOOL bStt  = (m_nTypeCheck == 0 || m_nTypeCheck == 1) ? TRUE : FALSE;
		BOOL bLeft = (m_nTypeCheck == 0 || m_nTypeCheck == 2) ? TRUE : FALSE;

		CWingWall *pWing = m_pBri->GetWingWall(bStt, bLeft);
		pWing->m_bUseUserOutput3D = m_bUseUserOutput;
		if(pWing->m_bUseUserOutput3D)
		{
			SetDataSave_3DOutputWingWall();
		}
	}
}

void CInputCheckGrid::SetDataSave_3DOutputWingWall()
{
	BOOL bStt  = (m_nTypeCheck == 0 || m_nTypeCheck == 1) ? TRUE : FALSE;
	BOOL bLeft = (m_nTypeCheck == 0 || m_nTypeCheck == 2) ? TRUE : FALSE;

	CWingWall *pWing = m_pBri->GetWingWall(bStt, bLeft);

	BOOL bLsd = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();
	long nTypeCalc = m_pBri->m_nSelectWingCalc;// 관용, 혼합, 3D
	BOOL bManual = nTypeCalc != 2 ? TRUE : FALSE;
	long nRow = bManual ? 4 : 1;
	long nCol = 2;
	for(long ix = 0; ix < 5; ix++)
	{
		pWing->m_dMomentUltPlate[ix] = m_Grid.GetTextMatrixDouble(nRow, ix + nCol); // A,B,C,D,A'
		pWing->m_dShearUltPlate[ix] = m_Grid.GetTextMatrixDouble(nRow+1, ix + nCol); // A,B,C,D,A'
		pWing->m_dMomentUsePlate[ix] = m_Grid.GetTextMatrixDouble(nRow+2, ix + nCol); // A,B,C,D,A'
		if(bLsd)
		{
			pWing->m_dMomentUseJudge_Plate[ix] = m_Grid.GetTextMatrixDouble(nRow+3, ix + nCol); // A,B,C,D,A'
		}
	}
}


void CInputCheckGrid::SetDataSave_3DOutput()
{
	if(m_pBri->m_bUseShellUserOutput==FALSE) return;
	// 슬래브 부재력 저장
	BOOL bLsd = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();
	long nQtyRow = m_pBri->m_nQtyJigan*2 + 1;
	long nQtyCol = bLsd? m_Grid.GetCols() :  5; // M,V,P,M,V (계수하중,사용하중)
	long nRow = 2;
	long nCol = 1;
	long nSizeSalb = m_pBri->IsBoxType() ? m_pBri->m_nQtyJigan*3*2 : m_pBri->m_nQtyJigan*3;
	m_pBri->m_MatShellSlab.Resize(nSizeSalb, nQtyCol);
	long nIdxRow = 0;
	double dValue = 0;
	long i=0; 
	for(long iUD = iUPPER; iUD <= iLOWER; iUD++)
	{
		if(iUD == iLOWER && m_pBri->IsBoxType() == FALSE) break;
		for(i=0; i<nQtyRow; i++)
		{
			long j=0; for(j=0; j<nQtyCol; j++)
			{
				dValue= (j==2)? 0 : m_Grid.GetTextMatrixDouble(nRow,nCol+j);
				m_pBri->m_MatShellSlab(nIdxRow,j) = dValue;
			}
			nIdxRow++;
			nRow++;
		}
	}
	// 시점벽체 부재력 저장
	if(m_pBri->IsOutWall(TRUE))
	{
		nRow = m_pBri->IsBoxType() ? 2 + (m_pBri->m_nQtyJigan*2 + 1) * 2 : 2 + m_pBri->m_nQtyJigan*2 + 1;
		nQtyRow = 5;
		m_pBri->m_MatShellWallStt.Resize(nQtyRow,nQtyCol);
		for(i=0; i<nQtyRow; i++)
		{
			long j=0; for(j=0; j<nQtyCol; j++)
			{
				dValue= (j==2)? 0 : m_Grid.GetTextMatrixDouble(nRow,nCol+j);
				m_pBri->m_MatShellWallStt(i,j) = dValue;
			}
			nRow++;
		}
	}
	// 종점벽체 부재력 저장
	if(m_pBri->IsOutWall(FALSE))
	{
		nQtyRow = 5;
		m_pBri->m_MatShellWallEnd.Resize(nQtyRow,nQtyCol);
		for(i=0; i<nQtyRow; i++)
		{
			long j=0; for(j=0; j<nQtyCol; j++)
			{
				dValue= (j==2)? 0 : m_Grid.GetTextMatrixDouble(nRow,nCol+j);
				m_pBri->m_MatShellWallEnd(i,j) = dValue;
			}
			nRow++;
		}
	}
	// 내측벽체 부재력 저장
	long nQtyJijum = m_pBri->GetCountJijum();
	for(i=1; i<nQtyJijum-1; i++)
	{
		nQtyRow = 1;
		m_pBri->m_MatShellWallMid[i-1].Resize(nQtyRow,nQtyCol);
		long j=0; for(j=0; j<nQtyCol; j++)
		{
			dValue= (j==2)? 0 : m_Grid.GetTextMatrixDouble(nRow,nCol+j);
			m_pBri->m_MatShellWallMid[i-1](0,j) = dValue;
		}
		nRow++;
	}

	if(m_pBri->m_bExistProtectionRiseFooting)
	{
		nQtyRow = 2;
		m_pBri->m_MatShellPRF.Resize(nQtyRow,nQtyCol);
		
		for(long i = 0; i < 2; ++i)
		{
			for(long j = 0; j < nQtyCol; ++j)
			{
				m_pBri->m_MatShellPRF(i, j) = m_Grid.GetTextMatrixDouble(nRow,nCol+j);
			}
			nRow++;
		}
	}
	// 현재 Mat를 갱신해야 한다.
	if(bLsd)
		MakeMatrixDesign_Lsd(FALSE, TRUE);
	else
		MakeMatrixDesign(FALSE, FALSE, TRUE);
}

BOOL CInputCheckGrid::PreTranslateMessage(MSG* pMsg) 
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
	
	return CXTResizeDialog::PreTranslateMessage(pMsg);
}

void CInputCheckGrid::CalculateBridge()
{
	if(!m_pCalc || !m_pBri) return;
	
//	if(m_pBri->m_pArrConcWall.GetSize() == 0 || m_pBri->m_pArrConcWall.GetSize() == 0)
//		m_pBri->SetConcData(TRUE,TRUE,TRUE,FALSE);
//	m_pCalc->CalculateAll();
	m_pCalc->CalculateCofficient();

	if(m_pBri)
	{
//		BOOL bChange = FALSE;
//		BOOL bArch = m_pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH;
//		long nTypeOld = 0;
// 너무 중복호출이 되서 복구가 되지 않는다. 집계할때나 계산할때 다 해주고 있으니 여기선 하지 말자.
// 		if(m_pBri->m_nTypeJongdanForRebar != (bArch? 1 : 0))
// 		{
// 			nTypeOld	= m_pBri->m_nTypeJongdanForRebar;
// 			m_pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, bArch? 1 : 0);
// 			m_pBri->SyncBridge();
// 			bChange = TRUE;
// 		}

		if(m_pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
			m_pCalc->m_pArchRib->CalculateAll();

		if(m_nInputIDD==IDD_REBAR_MAIN)		
		{
			m_pBri->SetConcData(TRUE,TRUE,TRUE,FALSE,TRUE);
//			m_pCalc->m_pFooting->CalculateAll();
			m_pCalc->CalculateAll();
		}
		if(m_nInputIDD==IDD_REBAR_MAIN2)	
		{
			m_pBri->SetConcData(TRUE,TRUE,TRUE,FALSE,TRUE);
			m_pCalc->m_pFooting->CalculateAll();
		}
		if(m_nInputIDD==IDD_REBAR_MAIN3)	
		{
			m_pBri->SetConcData(TRUE,TRUE,FALSE,FALSE,TRUE);
			m_pCalc->m_pCorner[iUPPER]->CalculateAll(TRUE);
			if(m_pBri->IsBoxType())
			{
				m_pCalc->m_pCorner[iLOWER]->CalculateAll(FALSE);
			}
		}
		if(m_nInputIDD==IDD_REBAR_SECTION)	
		{
			m_pBri->SetConcData(TRUE,TRUE,TRUE,FALSE,TRUE);

			m_pCalc->m_pFooting->CalculateAll();
			m_pCalc->m_pApsStt[0]->CalculateAps();
			m_pCalc->m_pApsEnd[0]->CalculateAps();
			if(m_pCalc->m_pApsStt[1]) m_pCalc->m_pApsStt[1]->CalculateAps();
			if(m_pCalc->m_pApsEnd[1]) m_pCalc->m_pApsEnd[1]->CalculateAps();
			m_pCalc->m_pApsStt[0]->SetConcData();
			m_pCalc->m_pApsEnd[0]->SetConcData();
			if(m_pCalc->m_pApsStt[1]) m_pCalc->m_pApsStt[1]->SetConcData();
			if(m_pCalc->m_pApsEnd[1]) m_pCalc->m_pApsEnd[1]->SetConcData();

			// 값을 집계하기 전에 종단Type을 모델링과 맞춰준다..
			BOOL bArch = (m_pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH || m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2);
			long nTypeJongOld = m_pBri->m_nTypeJongdanForRebar;
			m_pBri->m_nTypeJongdanForRebar = bArch? 1 : 0;
			m_pBri->SyncBridge(0, FALSE, FALSE, FALSE);
			m_pBri->SyncBridgeRebar(TRUE, -1, TRUE);
			m_pCalc->m_pColGir->CalculateAll();
			m_pBri->m_nTypeJongdanForRebar = nTypeJongOld;
			m_pBri->m_nTypeJongdanForRebarOld = nTypeJongOld;
			m_pBri->SyncBridge(0, FALSE, FALSE, FALSE);
		}
		if(m_nInputIDD==IDD_GEN_JONG_SECTION || m_nInputIDD==IDD_GEN_FOOTING)
		{
			m_pBri->SetConcData(FALSE,FALSE,TRUE,FALSE,FALSE);
			m_pCalc->CalculateAll();
//			m_pCalc->m_pFooting->CalculateAll();
		}
		if(m_nInputIDD==IDD_REBAR_WINGWALL)	
		{
			m_pBri->SetConcData(FALSE,FALSE,FALSE,TRUE,FALSE);
			BOOL bStt  = TRUE;
			BOOL bLeft = TRUE;
			if(m_nTypeCheck==2 || m_nTypeCheck==3) bStt = FALSE;
			if(m_nTypeCheck==1 || m_nTypeCheck==3) bLeft = FALSE;
			m_pCalc->m_pWing[m_nTypeCheck]->CalculateAll(bStt,bLeft);
		}
		if(m_nInputIDD==IDD_REBAR_COLUMN)
		{
			m_pBri->SetConcData(FALSE,TRUE,FALSE,FALSE,FALSE);
			m_pCalc->m_pColGir->CalculateAll();
		}
		if (m_nInputIDD==IDD_REBAR_APS)
		{
			const HGBOOL bStt = m_nTypeCheck == 0;
			CARcBridgeCalcAps *pAps = bStt ? m_pCalc->m_pApsStt[0] : m_pCalc->m_pApsEnd[0];
			pAps->CalculateAll();
		}

// 		if(bChange)
// 		{
// //			m_pBri->m_nTypeJongdanForRebar = nTypeOld;
// //			m_pBri->m_nTypeJongdanForRebarOld	= nTypeOld;
// 			m_pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, bArch? 1 : 0);
// 			m_pBri->SyncBridge();
// 		}
	}
}

void CInputCheckGrid::OnCheckUserPlateOutput() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_pBri->m_bUseShellUserOutput = m_bUseUserOutput;

	SetDataInit();
}

void CInputCheckGrid::SetDataInit_STM()
{
	if(!m_pStmModel)	return;

	m_Grid.DeleteAllItems();
	m_Grid.SetGridDefault(2, 2, 1, 1, TRUE, FALSE);
	m_Grid.SetCellSize(GRID_ROW_HEIGHT_20, -1);
	long nTab = m_ctlTab.GetCurSel();
	
	BOOL bLsd = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();

	if(nTab == 0) 
	{
		if( bLsd )
			SetDataInit_STM_RebarTie_Lsd();	// 철근타이 강도검토
		else
			SetDataInit_STM_RebarTie();	// 철근타이 강도검토
	}
	else if(nTab == 1)
	{
		if( bLsd )
			SetDataInit_STM_CompressionStrut_Lsd();	 // 압축스트럿 강도검토
		else
			SetDataInit_STM_CompressionStrut();	 // 압축스트럿 강도검토
	}
	else if(nTab == 2)
	{
		if( bLsd )
			SetDataInit_STM_NodalZone_Lsd();// 절점영역 강도검토
		else
			SetDataInit_STM_NodalZone();// 절점영역 강도검토
	}
	else if(nTab == 3)
	{
		SetDataInit_STM_MinRebar();// 최소철근비 검토
	}
	
	m_Grid.UpdateData(FALSE);

	long nRows = 0;
	long nCols = 0;
	CString str = _T("");

	nCols = m_Grid.GetColumnCount();
	nRows = m_Grid.GetRowCount();
	for(long nRow=0; nRow<=nRows; nRow++)
	{
		str = m_Grid.GetTextMatrix(nRow,nCols-1);
		if(str == _T("N.G"))
		{
			m_Grid.SetItemFgColour(nRow,nCols-1,RGB(255,0,0));	
		}
	}
}

void CInputCheckGrid::SetDataInit_STM_RebarTie()
{
	long row	= 0;
	long col	= 0;
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_READONLY;

	CString str	= _T("");
	CStringArray sArrTitle;

	CStmDesignStd StmDesign(m_pStmModel, NULL, NULL, TRUE);
	StmDesign.SetTypeRoadCode(m_pBri->m_pARcBridgeDataStd->IsDesignKDS2019() ? eRoadCode2019 : eRoadCode2015);
	StmDesign.CalculateArrangeRebar();
	
	long nSizeColumn = 0;
	sArrTitle.Add(hggettext("부재"));
	sArrTitle.Add(_T("Astreq"));
	sArrTitle.Add(_T("Astuse"));
	sArrTitle.Add(_T(" ηreq"));
	sArrTitle.Add(_T("최대유효폭"));
	sArrTitle.Add(_T("Sreq"));
	sArrTitle.Add(_T("Suse"));
	sArrTitle.Add(_T("비고"));

	nSizeColumn = sArrTitle.GetSize();
	for(long col=0; col<nSizeColumn; col++)
	{
		m_Grid.AddCell(row, col, sArrTitle.GetAt(col));
	}

	m_bOK = TRUE;
	row = 1;
	std::map<HGINT32,CStmDesignRebar*> mpRebar = StmDesign.GetDesignRebars();
	std::map<HGINT32,CStmDesignRebar*>::iterator posRebar;
	for(posRebar = mpRebar.begin(); posRebar != mpRebar.end(); ++posRebar)
	{
		CStmDesignRebar *pDesignRebar = StmDesign.GetDesignRebar(posRebar->first);
		for(HGINT32 ixTypeRebar = STM_TIE_REBAR_MAIN; ixTypeRebar <= STM_TIE_REBAR_SUPPORT; ixTypeRebar++)
		{
			std::map<HGINT32,CStmTieRebar*>::iterator posRebar;
			std::map<HGINT32,CStmTieRebar*> mpMain = pDesignRebar->GetTieRebars(ixTypeRebar);
			for(posRebar = mpMain.begin(); posRebar != mpMain.end(); ++posRebar)
			{
				CStmTieRebar *pRebar = pDesignRebar->GetTieRebar(posRebar->first, ixTypeRebar);
				if(!pRebar) continue;
				if(pRebar->m_ixType != ixTypeRebar) continue;

				CStmObjElement *pElement = m_pStmModel->GetElement(pRebar->m_idElement);
				if(!pElement) continue;
				SStmDefineTieType *pTieType = m_pStmModel->GetTieType(pRebar->m_idElement);
				if(!pTieType) continue;

				for(col=0; col<nSizeColumn; col++)
				{
					if(pRebar->m_ixType != STM_TIE_REBAR_SHEAR)
					{
						if(col==0)		str = pElement->GetName();
						else if(col==1)	str.Format(_T("%7.1f"), pRebar->m_dAsReq); 
						else if(col==2)	str.Format(_T("%7.1f"), pRebar->m_dAsUse);
						else if(col==3)	str = _T("-");
						else if(col==4)	str = _T("-");
						else if(col==5)	str = _T("-");
						else if(col==6)	str = _T("-");
						else if(col==7)	str = pRebar->m_bOk ? _T("O.K") : _T("N.G");
						m_Grid.AddCell(row, col ,str, nFormat, nLine);
					}
					else
					{
						if(col==0)		str = pElement->GetName();
						else if(col==1)	str = _T("-");
						else if(col==2)	str = _T("-");
						else if(col==3)	str.Format(NOTRANS("%7.1f"), pRebar->m_dNReq); 
						else if(col==4)	str.Format(NOTRANS("%6.1f"), pRebar->m_dMaxEWidth);
						else if(col==5)	str.Format(NOTRANS("%5.1f"), pRebar->m_dSReq);
						else if(col==6)	str.Format(NOTRANS("%5.1f"), pRebar->m_dCtcShear);
						else if(col==7)	str = pRebar->m_bOk ? NOTRANS("O.K") : NOTRANS("N.G");
						m_Grid.AddCell(row, col ,str, nFormat, nLine);
					}
				}
				row++;
				if(pRebar->m_bOk == FALSE)
				{
					m_bOK = FALSE;
				}
			}
		}
	}
}

void CInputCheckGrid::SetDataInit_STM_CompressionStrut()
{
	long row	= 0;
	long col	= 0;
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_READONLY;

	CString str	= _T("");

	CStmDataConfig *pStmData = m_pStmModel->GetStmDataConfig();
	CStmDesignStd StmDesign(m_pStmModel, NULL, NULL, TRUE);
	StmDesign.SetTypeRoadCode(m_pBri->m_pARcBridgeDataStd->IsDesignKDS2019() ? eRoadCode2019 : eRoadCode2015);
	StmDesign.CalculateNodalZone();
	StmDesign.CalculateDesignStrutStrength();

	CString sTitle[8] = { _T("부재"), _T("βs"), _T("θ"), _T("Fu(kN)"), _T("Φ"), _T("Wreq(mm)"), _T("Wprov(mm)"), _T("비고") };
	long nSizeColumn = 8;
	for(long col=0; col<nSizeColumn; col++)
	{
		m_Grid.AddCell(row, col, sTitle[col]);
	}

	m_bOK = TRUE;
	row = 1;
	std::map<HGINT32,CStmDesignStrutStrength*> mpStrut = StmDesign.GetDesignStrutStrengths();
	std::map<HGINT32,CStmDesignStrutStrength*>::iterator posStrut;
	for(posStrut = mpStrut.begin(); posStrut != mpStrut.end(); ++posStrut)
	{
		CStmDesignStrutStrength *pDesignStrut = StmDesign.GetDesignStrutStrength(posStrut->first);

		std::map<HGINT32,CStmStrutStrength*>::iterator posStrut;
		std::map<HGINT32,CStmStrutStrength*> mpStrutStrength = pDesignStrut->GetStmStrutStrengths();
		for(posStrut = mpStrutStrength.begin(); posStrut != mpStrutStrength.end(); ++posStrut)
		{
			CStmObjElement *pElement = m_pStmModel->GetElement(posStrut->first);
			CString str = _T("");
			for(col = 0; col < nSizeColumn; col++)
			{
				if(col == 0)	str = pElement->GetName();
				else if(col == 1)	str.Format(_T("%7.2f"), posStrut->second->m_dBetaS);// βs
				else if(col == 2)	str.Format(_T("%5.1f"), posStrut->second->m_dTheta);// θ
				else if(col == 3)	str.Format(_T("%7.1f"), posStrut->second->m_dFu);// Fu
				else if(col == 4)	str.Format(_T("%5.2f"), pStmData->m_dPiStrut);// Φ
				else if(col == 5)	str.Format(_T("%5.1f"), posStrut->second->m_dW_req);// Wreq
				else if(col == 6)	str.Format(_T("%5.1f"), posStrut->second->m_dW_prov);// Wprov
				else if(col == 7)	str.Format(_T("%s"), posStrut->second->m_bOK ? _T("O.K") :  _T("N.G"));// 비고

				m_Grid.AddCell(row, col ,str, nFormat, nLine);
			}
			row++;
			if(posStrut->second->m_bOK == FALSE)
			{
				m_bOK = FALSE;
			}
		}
	}
}

void CInputCheckGrid::SetDataInit_STM_NodalZone()
{
	long row	= 0;
	long col	= 0;
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_READONLY;

	CString str	= _T("");

	CStmDesignStd StmDesign(m_pStmModel, NULL, NULL, TRUE);
	StmDesign.SetTypeRoadCode(m_pBri->m_pARcBridgeDataStd->IsDesignKDS2019() ? eRoadCode2019 : eRoadCode2015);
	StmDesign.CalculateNodalZone();

	CString sTitle[8] = { _T("절점"), _T("βn"), _T("종류"), _T("단면력"), _T("Fu(kN)"), _T("Wreq(mm)"), _T("Wprov(mm)"), _T("비고") };
	long nSizeColumn = 8;
	for(long col=0; col<nSizeColumn; col++)
	{
		m_Grid.AddCell(row, col, sTitle[col]);
	}

	m_bOK = TRUE;
	row = 1;

	HGBOOL bCheckMessage(FALSE);
	CLongArray lArrRow;
	std::map<HGINT32,std::vector<CStmDesignNodalZoneBase*>> mvpNodalZone = StmDesign.GetDesignNodalZones();
	std::map<HGINT32,std::vector<CStmDesignNodalZoneBase*>>::iterator vposNodalZone;
	for(vposNodalZone = mvpNodalZone.begin(); vposNodalZone != mvpNodalZone.end(); ++vposNodalZone)
	{
		std::vector<CStmDesignNodalZoneBase*> vpNodalZone = vposNodalZone->second;
		std::vector<CStmDesignNodalZoneBase*>::iterator posNodal;
		for(posNodal = vpNodalZone.begin(); posNodal != vpNodalZone.end(); ++posNodal)
		{
			EStmTypeNodalZone eType = (*posNodal)->GetTypeNodalZone();
			if(eType == eStmNodalZoneInvalid) continue;

			HGBOOL bCheckWidth = (*posNodal)->CheckEffectiveWidth();
			if(!bCheckWidth)
			{
				bCheckMessage = TRUE;
			}

			std::vector<SStmNodalZoneStrength*> vpStrength = (*posNodal)->GetNodalZoneStrength();
			HGINT32 icStrength = vpStrength.size();
			for(long ix = 0; ix < icStrength; ix++)
			{
				SStmNodalZoneStrength *pStrength = vpStrength[ix];
				if(pStrength == NULL) continue;

				CString str = _T("");
				for(col = 0; col < nSizeColumn; col++)
				{
					if(col == 0)	str.Format(_T("%s"), (*posNodal)->GetNameNode());
					else if(col == 1)	str.Format(_T("%.1f"), (*posNodal)->GetBeta());
					else if(col == 2)	str = (*posNodal)->GetNameNodeType();
					else if(col == 3)	str = pStrength->name_element;
					else if(col == 4)	str.Format(_T("%7.1f"), pStrength->fu);
					else if(col == 5)	str.Format(_T("%7.1f"), frM(pStrength->wr));
					else if(col == 6)	str.Format(_T("%7.1f"), frM(pStrength->wp));
					else if(col == 7)	str = bCheckWidth ? (vpStrength[ix]->ok ? _T("O.K") :  _T("N.G")) : _T("CHECK");

					m_Grid.AddCell(row, col ,str, nFormat, nLine);
				}
				row++;
				if(vpStrength[ix]->ok == FALSE)
				{
					m_bOK = FALSE;
				}
			}
			lArrRow.Add(icStrength);
		}
	}

	row = 1;
	long nSizeNodalZone = lArrRow.GetSize();
	for(long ix = 0; ix < nSizeNodalZone; ix++)
	{
		long nRowSize = 0;
		nRowSize = lArrRow.GetAt(ix);
		if(nRowSize > 1)
		{
			for(col = 0; col <= 2; col++)
			{
				m_Grid.MergeGrid(row, row + nRowSize - 1, col, col);
			}
		}
		row += nRowSize;
	}

	if(bCheckMessage)
	{
		AfxMessageBox(_T("최대유효폭 산정이 불가한 부재가 있습니다. 확인후 최대유효폭 Assign 또는 모델링을 수정하시기 바랍니다."));
	}
}

void CInputCheckGrid::SetDataInit_STM_MinRebar()
{
	long row	= 0;
	long col	= 0;
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_READONLY;

	CString str	= _T("");

	CStmDesignStd StmDesign(m_pStmModel, NULL, NULL, TRUE);
	StmDesign.SetTypeRoadCode(m_pBri->m_pARcBridgeDataStd->IsDesignKDS2019() ? eRoadCode2019 : eRoadCode2015);
	StmDesign.CalculateArrangeRebar();

	CString sTitle[4] = { _T("부재"), _T("적용철근비"), _T("최소철근비"), _T("비고") };
	long nSizeColumn = 4;
	for(long col=0; col<nSizeColumn; col++)
	{
		m_Grid.AddCell(row, col, sTitle[col]);
	}

	m_bOK = TRUE;
	row = 1;

	std::map<HGINT32,CStmDesignRebar*> mpRebar = StmDesign.GetDesignRebars();
	std::map<HGINT32,CStmDesignRebar*>::iterator posRebar;
	for(posRebar = mpRebar.begin(); posRebar != mpRebar.end(); ++posRebar)
	{
		CStmDesignRebar *pDesignRebar = StmDesign.GetDesignRebar(posRebar->first);

		std::map<HGINT32,CStmMinRebar*>::iterator posRebar;
		std::map<HGINT32,CStmMinRebar*> mpMinRebar = pDesignRebar->GetMinRebars();
		for(posRebar = mpMinRebar.begin(); posRebar != mpMinRebar.end(); ++posRebar)
		{
			HGINT32 idElem = posRebar->second->GetIdElement();
			CStmObjElement *pElem = m_pStmModel->GetElement(idElem);
			if(pElem == NULL) continue;

			CString str = _T("");
			for(col = 0; col < nSizeColumn; col++)
			{
				if(col == 0)	str.Format(_T("%s"), pElem->GetName());
				else if(col == 1)	str.Format(_T("%.4f"), posRebar->second->GetMinRebarCoeffUse());
				else if(col == 2)	str.Format(_T("%.4f"), posRebar->second->GetMinRebarCoeffRequired());
				else if(col == 3)	str = posRebar->second->IsOk() ? _T("O.K") :  _T("N.G");

				m_Grid.AddCell(row, col ,str, nFormat, nLine);
			}
			row++;
			if(posRebar->second->IsOk() == FALSE)
			{
				m_bOK = FALSE;
			}
		}
	}
}

void CInputCheckGrid::SetDataInit_STM_RebarTie_Lsd()
{
	long row	= 0;
	long col	= 0;
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_READONLY;

	CString str	= _T("");
	CStringArray sArrTitle;

	CStmDesignStd StmDesign(m_pStmModel, NULL, NULL, TRUE);
	StmDesign.SetTypeRoadCode(m_pBri->m_pARcBridgeDataStd->IsDesignKDS2019() ? eRoadCode2019 : eRoadCode2015);
	StmDesign.CalculateArrangeRebar();

	long nDec = 0;
	long nSizeColumn = 0;
	sArrTitle.Add(hggettext("부재"));
	sArrTitle.Add(_T("Ft(N)"));
	sArrTitle.Add(_T("As.req(mm²)"));
	sArrTitle.Add(_T("As.use(mm²)"));
	sArrTitle.Add(_T("As.use/As.req"));
	sArrTitle.Add(_T("ηreq(EA)"));
	sArrTitle.Add(_T("Sreq(mm)"));
	sArrTitle.Add(_T("Suse(mm)"));
	sArrTitle.Add(_T("비고"));

	nSizeColumn = sArrTitle.GetSize();
	for(long col=0; col<nSizeColumn; col++)
	{
		m_Grid.AddCell(row, col, sArrTitle.GetAt(col));
	}

	m_bOK = TRUE;
	row = 1;
	std::map<HGINT32,CStmDesignRebar*> mpRebar = StmDesign.GetDesignRebars();
	std::map<HGINT32,CStmDesignRebar*>::iterator posRebar;
	for(posRebar = mpRebar.begin(); posRebar != mpRebar.end(); ++posRebar)
	{
		CStmDesignRebar *pDesignRebar = StmDesign.GetDesignRebar(posRebar->first);
		for(HGINT32 ixTypeRebar = STM_TIE_REBAR_MAIN; ixTypeRebar <= STM_TIE_REBAR_SUPPORT; ixTypeRebar++)
		{
			std::map<HGINT32,CStmTieRebar*>::iterator posRebar;
			std::map<HGINT32,CStmTieRebar*> mpMain = pDesignRebar->GetTieRebars(ixTypeRebar);
			for(posRebar = mpMain.begin(); posRebar != mpMain.end(); ++posRebar)
			{
				CStmTieRebar *pRebar = pDesignRebar->GetTieRebar(posRebar->first, ixTypeRebar);
				if(!pRebar) continue;
				if(pRebar->m_ixType != ixTypeRebar) continue;

				CStmObjElement *pElement = m_pStmModel->GetElement(pRebar->m_idElement);
				if(!pElement) continue;
				SStmDefineTieType *pTieType = m_pStmModel->GetTieType(pRebar->m_idElement);
				if(!pTieType) continue;

				for(col=0; col<nSizeColumn; col++)
				{
					str = _T("-");
					if(pRebar->m_ixType == STM_TIE_REBAR_MAIN || pRebar->m_ixType == STM_TIE_REBAR_SUPPORT) // 주철근
					{
						if(col==0)		str = pElement->GetName();
						else if(col==1)	str = GetStrDouble(pRebar->m_dFu, nDec, TRUE);
						else if(col==2)	str = GetStrDouble(pRebar->m_dAsReq, nDec, TRUE);
						else if(col==3)	str = GetStrDouble(pRebar->m_dAsUse, nDec, TRUE);
						else if(col==4)	str.Format(_T("%.3f"), pRebar->m_dAsUse / pRebar->m_dAsReq);
					}
					else //전단철근
					{
						if(col==0)		str = pElement->GetName();
						else if(col==1)	str = GetStrDouble(pRebar->m_dFu, nDec, TRUE);
						else if(col==5)	str = GetStrDouble(pRebar->m_dNReq, 1, TRUE);
						else if(col==6)	str = GetStrDouble(pRebar->m_dSReq, nDec, TRUE);
						else if(col==7)	str = CommaC(pRebar->m_dCtcShear);
					}

					if(col==8)	str = pRebar->m_bOk ? _T("O.K") : _T("N.G");

					m_Grid.AddCell(row, col ,str, nFormat, nLine);
				}
				row++;
				if(pRebar->m_bOk == FALSE)
				{
					m_bOK = FALSE;
				}
			}
		}
	}

}

void CInputCheckGrid::SetDataInit_STM_CompressionStrut_Lsd()
{
	long row	= 0;
	long col	= 0;
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_READONLY;

	CString str	= _T("");

	CStmDesignStd StmDesign(m_pStmModel, NULL, NULL, TRUE); 
	StmDesign.SetTypeRoadCode(m_pBri->m_pARcBridgeDataStd->IsDesignKDS2019() ? eRoadCode2019 : eRoadCode2015);
	StmDesign.CalculateNodalZone();
	StmDesign.CalculateDesignStrutStrength();

	CString sTitle[7] = { _T("부재"), _T("Fu(N)"), _T("fcd.max(N/mm²)"), _T("b(mm)"), _T("Wreq(mm)"), _T("Wprov(mm)"), _T("비고") };
	long nSizeColumn = 7;
	for(long col=0; col<nSizeColumn; col++)
	{
		m_Grid.AddCell(row, col, sTitle[col]);
	}

	m_bOK = TRUE;
	row = 1;
	std::map<HGINT32,CStmDesignStrutStrength*> mpStrut = StmDesign.GetDesignStrutStrengths();
	std::map<HGINT32,CStmDesignStrutStrength*>::iterator posStrut;
	for(posStrut = mpStrut.begin(); posStrut != mpStrut.end(); ++posStrut)
	{
		CStmDesignStrutStrength *pDesignStrut = StmDesign.GetDesignStrutStrength(posStrut->first);

		std::map<HGINT32,CStmStrutStrength*>::iterator posStrut;
		std::map<HGINT32,CStmStrutStrength*> mpStrutStrength = pDesignStrut->GetStmStrutStrengths();
		for(posStrut = mpStrutStrength.begin(); posStrut != mpStrutStrength.end(); ++posStrut)
		{
			CStmStrutStrength *pStrut = posStrut->second;
			if(pStrut == NULL) 	continue;

			CStmObjElement *pElement = m_pStmModel->GetElement(posStrut->first);
			if(pElement == NULL) continue;

			CString str = _T("");
			for(col = 0; col < nSizeColumn; col++)
			{
				if(col == 0)	str = pElement->GetName();
				else if(col == 1)	str = GetStrDouble(pStrut->m_dFu, 0, TRUE);
				else if(col == 2)	str = GetStrDouble(pStrut->m_fcd_max, 0, TRUE);
				else if(col == 3)	str = GetStrDouble(m_pStmModel->GetThickModel(), 0, TRUE);
				else if(col == 4)	str = GetStrDouble(pStrut->m_dW_req, 0, TRUE);
				else if(col == 5)	str = GetStrDouble(pStrut->m_dW_prov, 0, TRUE);
				else if(col == 6)	str = pStrut->m_bOK ? _T("O.K") : _T("N.G");

				m_Grid.AddCell(row, col ,str, nFormat, nLine);
			}
			row++;
			if(posStrut->second->m_bOK == FALSE)
			{
				m_bOK = FALSE;
			}
		}
	}
}

void CInputCheckGrid::SetDataInit_STM_NodalZone_Lsd()
{
	long row	= 0;
	long col	= 0;
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_READONLY;

	CString str	= _T("");

	CStmDesignStd StmDesign(m_pStmModel, NULL, NULL, TRUE);
	StmDesign.SetTypeRoadCode(m_pBri->m_pARcBridgeDataStd->IsDesignKDS2019() ? eRoadCode2019 : eRoadCode2015);
	StmDesign.CalculateNodalZone();

	long nSizeColumn = 9;
	CLongArray nArrCntMergeRow;

	m_bOK = TRUE;

	HGBOOL bCheckMessage(FALSE);
	CLongArray lArrRow;
	std::map<HGINT32,std::vector<CStmDesignNodalZoneBase*>> mvpNodalZone = StmDesign.GetDesignNodalZones();
	std::map<HGINT32,std::vector<CStmDesignNodalZoneBase*>>::iterator vposNodalZone;
	for(vposNodalZone = mvpNodalZone.begin(); vposNodalZone != mvpNodalZone.end(); ++vposNodalZone)
	{
		//Merge 계산
		std::vector<CStmDesignNodalZoneBase*> vpNodalZone = vposNodalZone->second;
		std::vector<CStmDesignNodalZoneBase*>::iterator posNodal;
		for(posNodal = vpNodalZone.begin(); posNodal != vpNodalZone.end(); ++posNodal)
		{
			EStmTypeNodalZone eType = (*posNodal)->GetTypeNodalZone();
			if(eType == eStmNodalZoneInvalid) continue;

			HGBOOL bCheckWidth = (*posNodal)->CheckEffectiveWidth();
			if(!bCheckWidth)
			{
				bCheckMessage = TRUE;
			}

			std::vector<SStmNodalZoneStrength*> vpStrength = (*posNodal)->GetNodalZoneStrength();
			if(vpStrength.size() == 0)  continue;

			HGINT32 icStrut = 0;
			for(HGINT32 ix = 0; ix < vpStrength.size(); ix++)
			{
				SStmNodalZoneStrength *pStrength = vpStrength[ix];
				if(pStrength == NULL) continue;
				if(pStrength->name_element.Find(NOTRANS("C-")) == -1) continue;
				icStrut++;

				if(vpStrength[ix]->ok == FALSE)
				{
					m_bOK = FALSE;
				}

			}
			nArrCntMergeRow.Add(icStrut);
		}

		//Data
		CMatrixStr matElement = StmDesign.GetMatrixStringNodalZoneStrength_Lsd(vpNodalZone, FALSE);
		HGINT32 icRow = matElement.GetRows();

		for(HGINT32 row = 0; row < icRow; ++row)
		{
			for(col = 0; col < nSizeColumn; col++)
			{
				CString str = matElement(row, col);

				m_Grid.AddCell(row, col, str, nFormat, nLine);
			}
		}
	}

	//Merge
	row = 1;
	long nSizeNodalZone = nArrCntMergeRow.GetSize();
	for(long ix = 0; ix < nSizeNodalZone; ix++)
	{
		long nRowSize = nArrCntMergeRow.GetAt(ix);
		if(nRowSize > 1)
		{
			for(col = 0; col <= 1; col++)
			{
				m_Grid.MergeGrid(row, row + nRowSize - 1, col, col);
			}
		}
		row += nRowSize;
	}

	if(bCheckMessage)
	{
		AfxMessageBox(_T("최대유효폭 산정이 불가한 부재가 있습니다. 확인후 최대유효폭 Assign 또는 모델링을 수정하시기 바랍니다."));
	}
}

void CInputCheckGrid::SetGridTitle_DesignVariable_ExposureRating()
{
	CLsdExposureRatingCtl *pExposure = &m_pBri->m_plsdEnvRating->GetLsdExposureRatingCtl();
	if(pExposure == NULL) 
		return;

	const long nSize = pExposure->GetMaterialSize();

	m_Grid.SetRowCount(nSize + 2);
	m_Grid.SetColumnCount(12);
	m_Grid.SetFixedRowCount(2);
	m_Grid.SetFixedColumnCount(1);

	m_Grid.SetMergeCol(0, 1, 3);
	m_Grid.SetMergeCol(0, 4, 5);
	m_Grid.SetMergeCol(0, 7, 8);
	m_Grid.SetMergeCol(0, 9, 11);
	m_Grid.SetMergeRow(0,1,0);
	m_Grid.SetMergeRow(0,1,6);

	m_Grid.SetTextMatrix(0, 0, hggettext("구 분"));
	m_Grid.SetTextMatrix(0, 1, hggettext("부 식"));
	m_Grid.SetTextMatrix(0, 4, hggettext("콘크리트 손상"));
	m_Grid.SetTextMatrix(0, 6, hggettext("결정\n노출등급"));
	m_Grid.SetTextMatrix(0, 7, hggettext("최소 콘크리트 강도"));
	m_Grid.SetTextMatrix(0, 9, hggettext("최소피복두께"));

	m_Grid.SetTextMatrix(1, 0, hggettext("구 분"));
	m_Grid.SetTextMatrix(1, 1, hggettext("탄산화"));
	m_Grid.SetTextMatrix(1, 2, hggettext("염화물"));
	m_Grid.SetTextMatrix(1, 3, hggettext("해수염화물"));
	m_Grid.SetTextMatrix(1, 4, hggettext("동결/융해/침식"));
	m_Grid.SetTextMatrix(1, 5, hggettext("화학적침투"));
	m_Grid.SetTextMatrix(1, 6, hggettext("결정\n노출등급"));
	m_Grid.SetTextMatrix(1, 7, hggettext("결정강도"));
	m_Grid.SetTextMatrix(1, 8, hggettext("적용강도"));
	m_Grid.SetTextMatrix(1, 9, hggettext("결정피복"));
	m_Grid.SetTextMatrix(1, 10, hggettext("설계피복"));
	m_Grid.SetTextMatrix(1, 11, hggettext("적용피복"));

	m_Grid.SetColumnWidthAll(85);
	m_Grid.SetColumnWidth(0, 120);
}

void CInputCheckGrid::SetDataInit_DesignVariable_ExposureRating()
{
	CLsdExposureRatingCtl *pExposure = &m_pBri->m_plsdEnvRating->GetLsdExposureRatingCtl();
	if(pExposure == NULL) 
		return;

	long nSize = pExposure->GetMaterialSize();

	CString	str	=	_T("");
	SExposureOutputParams stOutputParams;
	m_pBri->SetExposureParameter(stOutputParams);

	if(nSize != stOutputParams.m_vecConcApplyStrength.size())
	{
		m_pBri->SetDataDefaultExposureRatingLsd();
		nSize = pExposure->GetMaterialSize();		
	}

	CLsdEnviromentOutCalc EnvClac(m_pBri->m_plsdEnvRating);
	EnvClac.SetExposureOutputParams(stOutputParams);
	
	long	row	=	2;
	for(long ix = 0; ix < nSize; ix++)
	{
		long col	= 0;

		// 부식~콘크리트 손상~ 결정 노출등급
		CString strRatingConc = NOTRANS(""), strRatingThick = NOTRANS("");

		pExposure->GetMaterialDecideRating(ix, strRatingConc, strRatingThick);

		CString strDecideRatingOut = 
			(strRatingConc == strRatingThick) ? strRatingConc : strRatingConc + NOTRANS(", ") + strRatingThick;

		vector <CString> vecDecideRatings;
		for(long nCategory = 0 ; nCategory < eExposureRatingCATEGORY2_MAXCOUNT ; ++nCategory)
		{
			CString strDecideRating = pExposure->GetMaterialDecideRatingByCategory(ix, nCategory);
			vecDecideRatings.push_back(strDecideRating);
		}

		//콘크리트 결정강도~ 적용강도
		long nRatingIndex = pExposure->GetIndexByExposureRating(strRatingConc);
		double dConcStrength = pExposure->GetConcStrength(nRatingIndex);
		double dConcApplyStrength =  stOutputParams.m_vecConcApplyStrength.size() <= ix? 0 : stOutputParams.m_vecConcApplyStrength[ix];

		CString strConcreteStrength; strConcreteStrength.Format(NOTRANS("%.0f"),dConcStrength);
		CString strApplyStrength; strApplyStrength.Format(NOTRANS("%.0f"), dConcApplyStrength);
		if(nRatingIndex < 0) 
			strConcreteStrength = _T("-");
		//
		double dThickSum = EnvClac.GetThickSum(ix);
		double dThickDesign = stOutputParams.m_vecDesignCoverThick.size() <= ix? 0 : stOutputParams.m_vecDesignCoverThick[ix];
		double dThickCover   = EnvClac.GetThickCover(ix);

		CString strThickSum, strThickCover, strThickDesign;		
		strThickSum.Format(NOTRANS("%.0f"), dThickSum);
		strThickCover.Format(NOTRANS("%.1f"), dThickCover);
		strThickDesign.Format(NOTRANS("%.0f"), dThickDesign);

		//0 구분
		str = pExposure->GetMaterialTitle(ix);
		m_Grid.SetTextMatrix(row + ix, col++, str);


		//1탄산화
		str = vecDecideRatings[eExposureRatingCATEGORY2_Carbonation];
		m_Grid.SetTextMatrix(row + ix, col++, str);

		//2염화물
		str = vecDecideRatings[eExposureRatingCATEGORY2_Chloride];
		m_Grid.SetTextMatrix(row + ix, col++, str);

		//3해수염화물
		str = vecDecideRatings[eExposureRatingCATEGORY2_Seawater];
		m_Grid.SetTextMatrix(row + ix, col++, str);

		//4동결/융해/침식
		str = vecDecideRatings[eExposureRatingCATEGORY2_Freezing];
		m_Grid.SetTextMatrix(row + ix, col++, str);

		//5화학적침투
		str = vecDecideRatings[eExposureRatingCATEGORY2_Chemical];
		m_Grid.SetTextMatrix(row + ix, col++, str);

		//6결정노출등급
		str = strDecideRatingOut;
		m_Grid.SetTextMatrix(row + ix, col++, str);

		//7 결정강도
		str = strConcreteStrength;
		m_Grid.SetTextMatrix(row + ix, col++, str);

		//8 적용강도
		str = strApplyStrength;
		m_Grid.SetTextMatrix(row + ix, col++, str);

		//9 결정피복
		str = strThickSum;
		m_Grid.SetTextMatrix(row + ix, col++, str);

		//10 적용피복
		str = strThickCover;
		m_Grid.SetTextMatrix(row + ix, col++, str);

		//11 설계피복(구: 적용피복)
		str = strThickDesign;
		m_Grid.SetTextMatrix(row + ix, col++, str);
	}
}

CString CInputCheckGrid::GetStrPileCase(HGINT32 ixCase) const
{
	if (ixCase == PILE_CASE_USE)
	{
		return "사용한계";
	}
	else if (ixCase == PILE_CASE_ULTIMATE)
	{
		return "극한한계";
	}
	else if (ixCase == PILE_CASE_EXTREME)
	{
		return "극단상황한계";
	}

	return "";
}

CDesignPile * CInputCheckGrid::GetCurDesignPile() const
{
	long nIndex = 0;
	long n=0; 
	for(n=0; n<m_nTypeCheck; n++)
	{
		if(m_pBri->IsPileBase(n)) nIndex++;
	}

	CDesignPile *pCalcPile = m_pCalc->m_pArrPileCalc.GetAt(nIndex);

	return pCalcPile;
}

void CInputCheckGrid::SetGridTitle_RebarAps()
{
	BOOL bMainRatio = m_pBri->m_bCheckSupportSlabMainRate;
	BOOL bMainRatio_Tra = m_pBri->m_bCheckSupportSlabMainRateSpan;

	BOOL bLsd = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();
	long nTab = m_ctlTab.GetCurSel();

	long nCol = 0;
	long nSizeRow = 3;
	long nSizeCol = 0;

	switch(nTab)
	{
	case 0:	nSizeCol = 11; break;
	case 1:	nSizeCol = 12; break;
	case 2:	nSizeCol = 7;
		if(bMainRatio)
			nSizeCol += 3;
		if(bMainRatio_Tra)
			nSizeCol += 3;
		break;
	case 3:	nSizeCol = 13; break;
	}

	m_Grid.SetColumnCount(nSizeCol);
	m_Grid.SetRowCount(nSizeRow);
	m_Grid.SetFixedRowCount(2);
	if(nTab == 2) m_Grid.SetFixedColumnCount(1);
	else m_Grid.SetFixedColumnCount(2);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(70);
	
	m_Grid.SetColumnWidth(0, nTab == 2 ? 100 : 120);
	if(nTab == 0)
	{
		m_Grid.SetColumnWidth(6, 90);
		m_Grid.SetColumnWidth(7, 90);
	}

	m_Grid.SetTextMatrix(0,0,"구 분");
	if (nTab != 2)
	{
		m_Grid.SetMergeCol(0,0,1);
		m_Grid.SetMergeCol(1,0,1);
		m_Grid.SetMergeRow(0,1,1);
	}

	m_Grid.SetMergeRow(0,1,0);

	if(nTab == 0)
	{
		m_Grid.SetTextMatrix(0, 2, "철근비");
		m_Grid.SetMergeCol(0, 2, 5);
		m_Grid.SetTextMatrix(0, 6, "모멘트");
		m_Grid.SetMergeCol(0, 6, 10);

		m_Grid.SetTextMatrix(1, 2, bLsd ? _T("As,min") : _T("Pmin"));
		m_Grid.SetTextMatrix(1, 3, bLsd ? _T("As,use") : _T("Puse"));
		m_Grid.SetTextMatrix(1, 4, bLsd ? _T("As,max") : _T("Pmax"));
		m_Grid.SetTextMatrix(1, 5, "비고");
		m_Grid.SetTextMatrix(1, 6, "Mr");
		m_Grid.SetTextMatrix(1, 7, "Mu");
		m_Grid.SetTextMatrix(1, 8, "안전도");
		m_Grid.SetTextMatrix(1, 9, "비고");
		m_Grid.SetTextMatrix(1, 10, "추가인장력");
	}
	else if(nTab == 1)
	{		
		m_Grid.SetTextMatrix(0, 2, "균열");
		m_Grid.SetMergeRow(0, 1, 2);
		m_Grid.SetTextMatrix(0, 3, "최소철근량");
		m_Grid.SetMergeCol(0, 3, 5);
		m_Grid.SetTextMatrix(0, 6, "간접균열검토");
		m_Grid.SetMergeCol(0, 6, 8);
		m_Grid.SetTextMatrix(0, 9, "균열폭 검토");
		m_Grid.SetMergeCol(0, 9, 11);

		m_Grid.SetTextMatrix(1, 3, "As,min");
		m_Grid.SetTextMatrix(1, 4, "As,use");
		m_Grid.SetTextMatrix(1, 5, "비고");
		m_Grid.SetTextMatrix(1, 6, "fs,max");
		m_Grid.SetTextMatrix(1, 7, "fs");
		m_Grid.SetTextMatrix(1, 8, "비고");
		m_Grid.SetTextMatrix(1, 9, "W k");
		m_Grid.SetTextMatrix(1, 10, "Wa");
		m_Grid.SetTextMatrix(1, 11, "비고");
	}
	else if(nTab == 2)
	{
		m_Grid.SetTextMatrix(0, 1, "수평방향");
		m_Grid.SetMergeCol(0, 1, 3);
		m_Grid.SetTextMatrix(0, 4, "수직방향");
		m_Grid.SetMergeCol(0, 4, 6);

		nCol = 7;
		if(bMainRatio)
		{
			m_Grid.SetTextMatrix(0, nCol, "수직철근비");
			m_Grid.SetMergeCol(0, nCol, nCol+2);
			nCol += 3;
		}
		if(bMainRatio_Tra)
		{
			m_Grid.SetTextMatrix(0, nCol, "수직철근비(전통적설계법)");
			m_Grid.SetMergeCol(0, nCol, nCol+2);
		}

		nCol = 1;
		m_Grid.SetTextMatrix(1, nCol++, "필요철근량");
		m_Grid.SetTextMatrix(1, nCol++, "사용철근량");
		m_Grid.SetTextMatrix(1, nCol++, "비고");
		m_Grid.SetTextMatrix(1, nCol++, "필요철근량");
		m_Grid.SetTextMatrix(1, nCol++, "사용철근량");
		m_Grid.SetTextMatrix(1, nCol++, "비고");
		
		if(bMainRatio)
		{
			m_Grid.SetTextMatrix(1, nCol++, "필요철근량");
			m_Grid.SetTextMatrix(1, nCol++, "사용철근량");
			m_Grid.SetTextMatrix(1, nCol++, "비고");
		}
		if(bMainRatio_Tra)
		{
			m_Grid.SetTextMatrix(1, nCol++, "필요철근량");
			m_Grid.SetTextMatrix(1, nCol++, "사용철근량");
			m_Grid.SetTextMatrix(1, nCol++, "비고");
		}
	}
	else if(nTab == 3)
	{
		m_Grid.SetTextMatrix(0, 2, "철근비");
		m_Grid.SetMergeCol(0, 2, 6);
		m_Grid.SetTextMatrix(0, 7, "최소전단철근");
		m_Grid.SetMergeCol(0, 7, 9);
		m_Grid.SetTextMatrix(0, 10, "전단철근간격");
		m_Grid.SetMergeCol(0, 10, 12);

		nCol = 1;
		m_Grid.SetTextMatrix(1, ++nCol, "Vcd");
		m_Grid.SetTextMatrix(1, ++nCol, "Vsd");
		m_Grid.SetTextMatrix(1, ++nCol, "Θ");
		m_Grid.SetTextMatrix(1, ++nCol, "Vu");
		m_Grid.SetTextMatrix(1, ++nCol, "비고");
		m_Grid.SetTextMatrix(1, ++nCol, "pv,min");
		m_Grid.SetTextMatrix(1, ++nCol, "pv,use");
		m_Grid.SetTextMatrix(1, ++nCol, "비고");
		m_Grid.SetTextMatrix(1, ++nCol, "S_max");
		m_Grid.SetTextMatrix(1, ++nCol, "S");
		m_Grid.SetTextMatrix(1, ++nCol, "비고");
	}
}

void CInputCheckGrid::SetDataInit_RebarAps()
{
	long nTab = m_ctlTab.GetCurSel();
	if (nTab == 0)
	{
		SetDataInit_RebarAps_Stress_Lsd();
	}
	else if (nTab == 1)
	{
		SetDataInit_RebarAps_Use_Lsd();
	}
	else if (nTab == 2)
	{
		SetDataInit_RebarAps_Support_Lsd();
	}
	else if (nTab == 3)
	{
		SetDataInit_RebarAps_Shear_Lsd();
	}
}

void CInputCheckGrid::SetDataInit_RebarAps_Stress_Lsd()
{
	const HGBOOL bStt = m_nTypeCheck == 0;
	CARcBridgeCalcAps *pAps = bStt ? m_pCalc->m_pApsStt[0] : m_pCalc->m_pApsEnd[0];

	HGINT32 iRow = 1;
	HGINT32 iCol = 0;
	CString str = m_pBri->GetStringComboEnvType_Lsd(eEnvLimit);
	m_Grid.SetTextMatrix(++iRow, iCol++, str);

	m_Grid.SetTextMatrix(iRow, iCol++, "접속슬래브");
	
	CLsdBeam pBeam(pAps->m_pConc, LSD_ULS_DESIGN);
	SetDataInit_Stress_Lsd(pBeam, iRow, iCol);
}

void CInputCheckGrid::SetDataInit_RebarAps_Use_Lsd()
{
	const HGBOOL bStt = m_nTypeCheck == 0;
	const HGBOOL bLsd = TRUE;

	CARcBridgeCalcAps *pAps = bStt ? m_pCalc->m_pApsStt[0] : m_pCalc->m_pApsEnd[0];

	HGINT32 iRow = 1;
	HGINT32 iCol = 0;
	CString str = m_pBri->GetStringComboEnvType_Lsd(eEnvService);
	m_Grid.SetTextMatrix(++iRow, iCol++, str);

	m_Grid.SetTextMatrix(iRow, iCol++, "접속슬래브");

	CLsdBeam pBeam(pAps->m_pConc, LSD_ULS_DESIGN);
	SetDataInit_Use_Lsd(pBeam, iRow, iCol);
}

void CInputCheckGrid::SetDataInit_RebarAps_Support_Lsd()
{
	const HGBOOL bStt = m_nTypeCheck == 0;

	CARcBridgeCalcAps *pAps = bStt ? m_pCalc->m_pApsStt[0] : m_pCalc->m_pApsEnd[0];

	HGINT32 iRow = 1;
	HGINT32 iCol = 0;
	m_Grid.SetTextMatrix(++iRow, iCol++, "접속슬래브");

	CLsdBeam pBeam(pAps->m_pConc, LSD_ULS_DESIGN);
	SetDataInit_Support_Lsd(pBeam, iRow, iCol);
}

void CInputCheckGrid::SetDataInit_RebarAps_Shear_Lsd()
{
	const HGBOOL bStt = m_nTypeCheck == 0;

	CARcBridgeCalcAps *pAps = bStt ? m_pCalc->m_pApsStt[0] : m_pCalc->m_pApsEnd[0];

	HGINT32 iRow = 1;
	HGINT32 iCol = 0;
	CString str = m_pBri->GetStringComboEnvType_Lsd(eEnvLimit);
	m_Grid.SetTextMatrix(++iRow, iCol++, str);

	m_Grid.SetTextMatrix(iRow, iCol++, "접속슬래브");

	CLsdBeam pBeam(pAps->m_pConc, LSD_ULS_DESIGN);
	SetDataInit_Shear_Lsd(pBeam, iRow, iCol);
}

void CInputCheckGrid::SetDataInit_Stress_Lsd(CLsdBeam &pBeam, HGINT32 iRow, HGINT32 iCol)
{
	const HGBOOL bLsd = TRUE;

	double Asmin = pBeam.GetAsMin();
	double Asuse = pBeam.GetAsUse();
	double Asmax = pBeam.GetAsMax();
	BOOL bRate = pBeam.IsOkRebarRate(FALSE) && pBeam.IsOkRebarRate(TRUE);
	double Mn = pBeam.GetMn();
	double Mu = pBeam.GetMu();
	double dSafeRate = Mn / Mu;

	if(Compare(Mu, 0.0, _T("="), 0.01) == FALSE)
	{
		CString str;
		if(pBeam.m_pData->m_bSkipAsMinCheckByHoriRebar)
			str = _T("-");
		else
			str.Format("%s", GetStrDouble(Asmin, 0, bLsd));
		m_Grid.SetTextMatrix(iRow,iCol++, str);

		str.Format("%s", GetStrDouble(Asuse, 0, bLsd));
		m_Grid.SetTextMatrix(iRow,iCol++, str);

		str.Format("%s", GetStrDouble(Asmax, 0, bLsd));
		m_Grid.SetTextMatrix(iRow,iCol++, str);

		m_Grid.SetTextMatrix(iRow, iCol, (bRate) ? _T("O.K") : _T("N.G"));

		if(!bRate) m_Grid.SetItemFgColour(iRow, iCol++, RGB(255, 0, 0));
		else m_Grid.SetItemFgColour(iRow, iCol++, RGB(0, 0, 0));

		if (pBeam.IsSingleBeam())
		{
			str.Format("%s", GetStrDouble(Mn, 0, bLsd));
			m_Grid.SetTextMatrix(iRow,iCol++, str);

			str.Format("%s", GetStrDouble(Mu, 0, bLsd));
			m_Grid.SetTextMatrix(iRow,iCol++, str);

			str.Format("%s", GetStrDouble(dSafeRate, 3, bLsd));
			m_Grid.SetTextMatrix(iRow,iCol++, str);

			m_Grid.SetTextMatrix(iRow,iCol, pBeam.IsOk_Moment() ? _T("O.K") : _T("N.G"));
			if(!pBeam.IsOk_Moment()) m_Grid.SetItemFgColour(iRow, iCol++, RGB(255, 0, 0));
			else m_Grid.SetItemFgColour(iRow, iCol++, RGB(0, 0, 0));

			if(m_pBri->m_bCheckDeltaTR)
			{
				m_Grid.SetTextMatrix(iRow,iCol, (pBeam.IsNeedShearRebar() && pBeam.IsNeed_AddRebarTR()) ?  _T("N.G") : _T("O.K"));
				if((pBeam.IsNeedShearRebar() && pBeam.IsNeed_AddRebarTR())) m_Grid.SetItemFgColour(iRow, iCol++, RGB(255, 0, 0));
				else m_Grid.SetItemFgColour(iRow, iCol++, RGB(0, 0, 0));
			}
		}
		else
		{
			m_Grid.SetTextMatrix(iRow, iCol++, "-");
			m_Grid.SetTextMatrix(iRow, iCol++, "-");
			m_Grid.SetTextMatrix(iRow, iCol++, "-");
			m_Grid.SetTextMatrix(iRow, iCol++, "-");
			m_Grid.SetTextMatrix(iRow, iCol++, "-");
		}
	}
	else 
	{
		for(HGINT32 ix = 0; ix < 9; ++ix)
		{
			m_Grid.SetTextMatrix(iRow, iCol++, "-");
		}
	}
}

void CInputCheckGrid::SetDataInit_Use_Lsd(CLsdBeam &pBeam, HGINT32 iRow, HGINT32 iCol)
{
	const HGBOOL bLsd = TRUE;
	const HGBOOL bCrack = pBeam.IsCrackSection();

	m_Grid.SetTextMatrix(iRow, iCol++, bCrack ? "균열" : "비균열");
	if (bCrack && Compare(pBeam.m_pData->m_MomentUse, 0.0, _T("="), 0.01) == FALSE)
	{
		CString str;
		str = GetStrDouble(pBeam.GetAsMinCrack(), 3, bLsd);
		m_Grid.SetTextMatrix(iRow, iCol++, str);

		str = GetStrDouble(pBeam.GetAsUse(), 3, bLsd);
		m_Grid.SetTextMatrix(iRow, iCol++, str);

		str = pBeam.IsOkCrackAsMin() ? _T("O.K") : _T("N.G");
		m_Grid.SetTextMatrix(iRow, iCol++, str);

		if(pBeam.m_pData->m_nSelectUsebilityCheck != 0)
		{
			m_Grid.SetTextMatrix(iRow, iCol++, GetStrDouble(pBeam.GetMaxCrackMpa(), 3, bLsd));
			m_Grid.SetTextMatrix(iRow, iCol++, GetStrDouble(pBeam.GetFsCrack(), 3, bLsd));
			str = pBeam.IsOkCrackCtc() ? _T("O.K") : _T("N.G");
			if(str==_T("N.G"))
				m_Grid.SetItemFgColour(iRow, iCol, RGB(255,0,0));
			m_Grid.SetTextMatrix(iRow, iCol++, str);
		}
		else
		{
			m_Grid.SetTextMatrix(iRow, iCol++, _T("-"));
			m_Grid.SetTextMatrix(iRow, iCol++, _T("-"));
			m_Grid.SetTextMatrix(iRow, iCol++, _T("-"));
		}

		//균열폭검토
		if(pBeam.m_pData->m_nSelectUsebilityCheck != 1)
		{
			m_Grid.SetTextMatrix(iRow, iCol++, GetStrDouble(pBeam.GetWk(), 3, bLsd));
			m_Grid.SetTextMatrix(iRow, iCol++, GetStrDouble(pBeam.GetWa(), 3, bLsd));
			str = pBeam.IsOkCrackWidth() ? _T("O.K") : _T("N.G");
			if(str==_T("N.G"))
				m_Grid.SetItemFgColour(iRow, iCol, RGB(255,0,0));
			m_Grid.SetTextMatrix(iRow, iCol++, str);
		}
		else
		{
			m_Grid.SetTextMatrix(iRow, iCol++, _T("-"));
			m_Grid.SetTextMatrix(iRow, iCol++, _T("-"));
			m_Grid.SetTextMatrix(iRow, iCol++, _T("-"));
		}
	}
	else
	{
		m_Grid.SetTextMatrix(iRow, iCol++, "-");
		m_Grid.SetTextMatrix(iRow, iCol++, "-");
		m_Grid.SetTextMatrix(iRow, iCol++, "-");

		m_Grid.SetTextMatrix(iRow, iCol++, _T("-"));
		m_Grid.SetTextMatrix(iRow, iCol++, _T("-"));
		m_Grid.SetTextMatrix(iRow, iCol++, _T("-"));

		m_Grid.SetTextMatrix(iRow, iCol++, _T("-"));
		m_Grid.SetTextMatrix(iRow, iCol++, _T("-"));
		m_Grid.SetTextMatrix(iRow, iCol++, _T("-"));
	}
}

void CInputCheckGrid::SetDataInit_Shear_Lsd(CLsdBeam &pBeam, HGINT32 iRow, HGINT32 iCol)
{
	const HGBOOL bLsd = TRUE;
	const HGBOOL bNeedShear = pBeam.IsNeedShearRebar();

	CString str(_T(""));
	m_Grid.SetTextMatrix(iRow, iCol++, GetStrDouble(pBeam.GetVcd(), 0, bLsd));
	m_Grid.SetTextMatrix(iRow, iCol++, bNeedShear? GetStrDouble(pBeam.GetVd(), 0, bLsd) : _T("-"));
	
	if(bNeedShear) //Theta
		str.Format(_T("%.1f˚"), ToDegree(pBeam.Gettheta()));
	else
		str = _T("-");
	if(bNeedShear && pBeam.IsOk_thetaVu() == FALSE)
		m_Grid.SetItemFgColour(iRow, iCol, RGB(255,0,0));
	m_Grid.SetTextMatrix(iRow, iCol++, str);

	str = GetStrDouble(pBeam.GetVu(), 0, bLsd);
	m_Grid.SetTextMatrix(iRow, iCol++, str);

	str	= (bNeedShear == FALSE || pBeam.IsOk_Vsd())? _T("O.K") : _T("N.G");
	m_Grid.SetTextMatrix(iRow, iCol++, str);

	if(pBeam.m_pData->m_bMinAvCheck || bNeedShear)
	{
		str = GetStrDouble(pBeam.GetpvMin(), 5, bLsd);
		m_Grid.SetTextMatrix(iRow, iCol++, str);

		str = GetStrDouble(pBeam.GetpvUse(), 5, bLsd);
		m_Grid.SetTextMatrix(iRow, iCol++, str);

		BOOL bMinShear =  pBeam.IsOk_pvMin();
		str	=	(bMinShear)? _T("O.K") : _T("N.G");
		m_Grid.SetTextMatrix(iRow, iCol++, str);
	}
	else
	{
		for(long ix=0; ix<3; ++ix)
			m_Grid.SetTextMatrix(iRow, iCol++, _T("-"));
	}

	// (AAB-5691) 한계상태에서는 옵션과 상관없이 출력 - but 전단철근도 불필요하고 최소전단도 검토하지 않는 경우는 출력 X
	// if(pBeam.m_pData->m_bCheckMinCtcShear)
	if(pBeam.m_pData->m_bMinAvCheck || bNeedShear)
	{
		str = GetStrDouble(pBeam.GetSMax(), 0, bLsd);
		m_Grid.SetTextMatrix(iRow, iCol++, str);

		str = GetStrDouble(pBeam.GetSvCtc(), 0, bLsd);
		m_Grid.SetTextMatrix(iRow, iCol++, str);

		str	=	(pBeam.IsOk_SvCtc())? _T("O.K") : _T("N.G");
		m_Grid.SetTextMatrix(iRow, iCol++, str);
	}
	else
	{
		for(long ix=0; ix<3; ++ix)
			m_Grid.SetTextMatrix(iRow, iCol++, _T("-"));
	}
}

void CInputCheckGrid::SetDataInit_Support_Lsd(CLsdBeam &pBeam, HGINT32 iRow, HGINT32 iCol)
{
	const HGBOOL bLsd = TRUE;

	BOOL bMainRatio = (m_nInputIDD == IDD_REBAR_SECTION && (m_pBri->m_bCheckSupportSlabMainRate || m_pBri->m_bCheckSupportWallMainRate))
					||(m_nInputIDD == IDD_REBAR_APS && m_pBri->m_bCheckSupportSlabMainRate); //주철근비
	BOOL bMainRatio_Tra = (m_nInputIDD == IDD_REBAR_SECTION && m_pBri->m_bCheckSupportSlabMainRateSpan)
					|| (m_nInputIDD == IDD_REBAR_APS && m_pBri->m_bCheckSupportSlabMainRateSpan); //주철근비(전통적설계법)
	
	long nCheckSupprotBarRate = pBeam.m_pData->m_nCheckSupportBarRate;
	BOOL bMainRatioType = FALSE;
	BOOL bMainRatioType_Tra = FALSE;
	BOOL bDryingShrinkage = FALSE;
	if(nCheckSupprotBarRate == SUPPORTBAR_CHECK_SLAB)
	{
		bMainRatioType = m_pBri->m_bCheckSupportSlabMainRate;
		bMainRatioType_Tra = bMainRatio_Tra;
		bDryingShrinkage = m_pBri->m_bCheckSupportSlabDryingShrinkage;
	}
	else if(nCheckSupprotBarRate == SUPPORTBAR_CHECK_WALL)
	{
		bMainRatioType = m_pBri->m_bCheckSupportWallMainRate;
		bMainRatioType_Tra = FALSE;
		bDryingShrinkage = m_pBri->m_bCheckSupportWallDryingShrinkage;
	}
	else if(nCheckSupprotBarRate == SUPPORTBAR_CHECK_FOOTING)
	{
		bDryingShrinkage = m_pBri->m_bCheckSupportFootDryingShrinkage;
	}

	CString szReq, szUse, szOk;
	CString str;
	if(bDryingShrinkage)
	{
		str = GetStrDouble(pBeam.GetAsReqDryingShrinkage(TRUE), 1, bLsd);
		m_Grid.SetTextMatrix(iRow, iCol++, str);

		str = GetStrDouble(pBeam.GetAsUseDryingShrinkage(TRUE), 1, bLsd);
		m_Grid.SetTextMatrix(iRow, iCol++, str);

		str = pBeam.IsOkDryingShrinkage(TRUE, FALSE) ? _T("O.K") : _T("N.G");
		m_Grid.SetTextMatrix(iRow, iCol++, str);


		str = GetStrDouble(pBeam.GetAsReqDryingShrinkage(FALSE), 1, bLsd);
		m_Grid.SetTextMatrix(iRow, iCol++, str);

		str = GetStrDouble(pBeam.GetAsUseDryingShrinkage(FALSE), 1, bLsd);
		m_Grid.SetTextMatrix(iRow, iCol++, str);

		str = pBeam.IsOkDryingShrinkage(FALSE, FALSE) ? _T("O.K") : _T("N.G");
		m_Grid.SetTextMatrix(iRow, iCol++, str);
	}
	else
	{
		for(long ix=0; ix<6; ++ix)
		{
			m_Grid.SetTextMatrix(iRow, iCol++, _T("-"));
		}
	}

	if(bMainRatio)
	{
		szReq = bMainRatioType ? GetStrDouble(pBeam.GetAsReqHoriRebarByMainRebarRate(-1), 1, TRUE) : _T("-");
		szUse = bMainRatioType ? GetStrDouble(pBeam.GetAsUseHoriRebarByMainRebarRate(), 1, TRUE) : _T("-");
		szOk = bMainRatioType ? (pBeam.IsOkHoriRebarByMainRebarRate(-1) ? _T("O.K") : _T("N.G")) : _T("-");

		m_Grid.SetTextMatrix(iRow, iCol++, szReq);
		m_Grid.SetTextMatrix(iRow, iCol++, szUse);
		m_Grid.SetTextMatrix(iRow, iCol++, szOk);
	}
	if(bMainRatio_Tra)
	{
		szReq = bMainRatioType_Tra ? GetStrDouble(pBeam.GetAsReqHoriRebarByCarDirAndSpan(), 1, TRUE) : _T("-");
		szUse = bMainRatioType_Tra ? GetStrDouble(pBeam.GetAsUseHoriRebarByCarDirAndSpan(), 1, TRUE) : _T("-");
		szOk = bMainRatioType_Tra ? (pBeam.IsOkHoriRebarByCarDirAndSpan() ? _T("O.K") : _T("N.G")) : _T("-");

		m_Grid.SetTextMatrix(iRow, iCol++, szReq);
		m_Grid.SetTextMatrix(iRow, iCol++, szUse);
		m_Grid.SetTextMatrix(iRow, iCol++, szOk);
	}
}

HGBOOL CInputCheckGrid::GetFootInfo_Lsd(HGINT32 ixJijum, HGINT32 ixPos, CString &strPos, CString &strPos1, HGBOOL &bFront, HGBOOL &bUpper, HGINT32 &ixCheckAdd)
{
	HGBOOL bInFoot = FALSE;
	if (ixJijum == 0)
		strPos = "시점측기초";
	else if (ixJijum == m_pBri->GetCountJijum()-1)
		strPos = "종점측기초";
	else
	{
		strPos.Format("중간지점%d", ixJijum);
		bInFoot = TRUE;
	}
	
	ixCheckAdd = -1;
	if (ixPos == 0)
	{
		if (bInFoot)
		{
			bFront = FALSE;
			bUpper = FALSE;
			strPos1 = "하면";
		}
		else
		{
			bFront = TRUE;
			bUpper = FALSE;
			strPos1 = "앞굽(하면)";
		}

	}
	else if (ixPos == 1)
	{
		if (bInFoot)
		{
			bFront = FALSE;
			bUpper = TRUE;
			strPos1 = "상면";
		}
		else
		{
			bFront = TRUE;
			bUpper = TRUE;
			strPos1 = "앞굽(상면)";
		}
	}
	else if (ixPos == 2)
	{
		bFront = FALSE;
		bUpper = TRUE;
		strPos1 = "뒷굽(상면)";
	}
	else if (ixPos == 3)
	{
		bFront = FALSE;
		bUpper = FALSE;
		strPos1 = "뒷굽(하면)";
	}
	else if (ixPos == 4)
	{
		bFront = TRUE;
		bUpper = FALSE;
		strPos1 = "앞굽2(하면)";
		ixCheckAdd = 0;
	}
	else if (ixPos == 5)
	{
		bFront = TRUE;
		bUpper = TRUE;
		strPos1 = "앞굽2(상면)";
		ixCheckAdd = 0;
	}
	else if (ixPos == 6)
	{
		bFront = TRUE;
		bUpper = FALSE;
		strPos1 = "앞굽3(하면)";
		ixCheckAdd = 1;
	}
	else if (ixPos == 7)
	{
		bFront = TRUE;
		bUpper = TRUE;
		strPos1 = "앞굽3(상면)";
		ixCheckAdd = 1;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

HGINT32 CInputCheckGrid::GetCountFootPos(HGINT32 ixJijum)
{
	const HGBOOL bInFoot = (ixJijum == 0 || ixJijum == m_pBri->GetCountJijum() - 1) ? FALSE : TRUE;

	CFootingApp *pFooting = m_pBri->GetFootingByJijumNumber(ixJijum);
	const HGBOOL bTrap   = (m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP && (pFooting->m_nType == FOOTING_TYPE_RIGHTSLOPE || pFooting->m_nType == FOOTING_TYPE_LEFTSLOPE));
	CLoadFooting *pData = m_pCalc->m_pFooting->GetLoadFootingLsd(ixJijum, eEnvLimit, 0);

	long nTypeFoot = pFooting->GetTypeByInputValue();
	HGINT32 icAddCheck = 0;
	if (pData->m_bTwoUpperLoad && nTypeFoot!=0)
	{
		if (m_pBri->m_bCheckFrontFootTrap2)
		{
			icAddCheck += 4;
		}
		else
		{
			icAddCheck = 2;
		}
	}

	const HGINT32 icPos = (bInFoot ? 2 : 4 + icAddCheck);
	
	return icPos;
}

void CInputCheckGrid::SetDataInit_FootingPile_Hyunta()
{
	CARcBridgeDataStd *pData = m_pBri->m_pARcBridgeDataStd;

	long nRow = 0;
	long nCol = 0;
	CString szOK = _T(""), str(_T(""));

	long nIndex = 0;
	long n=0; for(n=0; n<m_nTypeCheck; n++)
	{
		if(m_pBri->IsBoxType()) break;
		if(m_pBri->IsPileBase(n)) nIndex++;
	}
	CDesignPile *pCalcPile = m_pCalc->m_pArrPileCalc.GetAt(nIndex);

	HGBOOL bLSD = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();

	nRow = 1;
	long nSize = pCalcPile->m_sMatHyunta.GetRow();
	for(long ix=0; ix<nSize; ++ix)
	{
		BOOL bExtreme = (ix > 2) ? TRUE : FALSE;
		nCol = 0;
		double dMoment = GetValueUnitChange(::hgatof(pCalcPile->m_sMatHyunta(ix,DESIGNPILE_HYUNTA_M)),pData->m_szTonfM,UNIT_CGS_TONFM);
		double dAxial  = GetValueUnitChange(::hgatof(pCalcPile->m_sMatHyunta(ix,DESIGNPILE_HYUNTA_P)),pData->m_szTonf,UNIT_CGS_TONF);
		double dDepth = pCalcPile->m_pPile->m_dH0 + hgatof(pCalcPile->m_sMatHyunta(ix, 6));
		CConcBasic *pConc = new CConcBasic;
		pCalcPile->SetConcDataHyunta(pConc, dDepth, bExtreme);

		// Box형 이라면 말뚝 반력이 아니라 PileLoad의 축력을 써야지
		if(m_pBri->IsBoxType())
		{
			long nIdxPile = bExtreme? ix-3 : ix;
			double dDistPile = m_pCalc->m_pFooting->GetDistMaxPile(nIdxPile);
			
			CPileLoad *pPileLoad = pCalcPile->GetPileLoad(bExtreme? PILE_CASE_EXTREME : PILE_CASE_ULTIMATE, nIdxPile, TRUE);
			dMoment = pPileLoad->m_dMoment * dDistPile;
			dAxial  = pPileLoad->m_dForceVert * dDistPile;
		}

		pConc->m_sTitle = pCalcPile->m_sMatHyunta(ix,5);
		pConc->m_Axial	= fabs(dAxial);
		pConc->m_Mux	= fabs(dMoment);

		CColumn Column(pConc, FALSE, pCalcPile->m_nSeclectPmin);	// 현장타설말뚝
		Column.SetValuePmax(pData->GetPmaxColumn());
		Column.CalculateAll(FALSE);

		if(bLSD)
		{
			CString sCase = ix < 3? _T("극한한계") : _T("극단상황한계");
			m_Grid.SetTextMatrix(nRow, nCol++, sCase);
		}

		m_Grid.SetTextMatrix(nRow, nCol++, pConc->m_sTitle);
		m_Grid.SetTextMatrixFormat(nRow, nCol++, _T("%.5f"), Column.GetValuePmin());
		m_Grid.SetTextMatrixFormat(nRow, nCol++, _T("%.5f"), Column.GetRebarUsedRate());
		m_Grid.SetTextMatrixFormat(nRow, nCol++, _T("%.5f"), Column.GetValuePmax());
		str = pData->GetStrDoubleUnitChange(Column.m_pData->m_Axial, UNIT_CGS_TONF, pData->m_szTonf);
		m_Grid.SetTextMatrix(nRow, nCol++, str);
		str = pData->GetStrDoubleUnitChange(Column.GetPiPn(), UNIT_CGS_TONF, pData->m_szTonf);
		m_Grid.SetTextMatrix(nRow, nCol++, str);
		m_Grid.SetTextMatrixFormat(nRow, nCol++, _T("%.3f"), Column.GetPiPn()/Column.m_pData->m_Axial);
		m_bOK	= (Column.IsOkAxial() && Column.IsOK_RebarRate());
		m_Grid.SetTextMatrix(nRow, nCol, m_bOK ? _T("O.K") : _T("N.G"));
		if(!m_bOK)
			m_Grid.SetItemFgColour(nRow, nCol, RGB(255,0,0));

		delete pConc;

		nRow++;
	}

	if(bLSD)
	{
		m_Grid.SetMergeRow(1,3,0);
		m_Grid.SetMergeRow(4,6,0);
	}
}
