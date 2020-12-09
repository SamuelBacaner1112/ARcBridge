// DrawingOptionAps.cpp : implementation file
//

#include "stdafx.h"
#include "../../ARcBridge/ARcBridgeDoc.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"
#include "../ARcBridgeDrawing/ARcBridgeDrawingStd.h"
#include "arcbridgedrawingoption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawingOptionAps property page

IMPLEMENT_DYNCREATE(CDrawingOptionAps, COptionBase)

CDrawingOptionAps::CDrawingOptionAps() : COptionBase(CDrawingOptionAps::IDD)
{
	//{{AFX_DATA_INIT(CDrawingOptionAps)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = NULL;
	m_nCurBri = 0;
}

CDrawingOptionAps::~CDrawingOptionAps()
{
}

void CDrawingOptionAps::DoDataExchange(CDataExchange* pDX)
{
	COptionBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawingOptionAps)
	DDX_Control(pDX, IDC_OPT_TREE, m_Tree);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_OPT_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CDrawingOptionAps, COptionBase)
	//{{AFX_MSG_MAP(CDrawingOptionAps)
	ON_BN_CLICKED(IDC_OPT_DEFAULT, OnOptionDefault)
	ON_CBN_SELCHANGE(IDC_OPT_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_WM_ACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawingOptionAps message handlers

BOOL CDrawingOptionAps::OnInitDialog() 
{
	COptionBase::OnInitDialog();
	m_nCurBri = (dynamic_cast<CARcBridgeDoc*>(m_pDoc))->m_nCurBriNoOfSettingDomyun;

	InitTree();

	long nRowCount = 4;
	CRect Rect;
	m_Grid.GetClientRect(&Rect);
	m_Grid.EnableReturnPass(TRUE);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowCount(nRowCount);
	m_Grid.SetColumnCount(2);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, Rect.Width()/2);
	m_Grid.SetColumnWidth(1, Rect.Width()/2);
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetTextBkColor(CLR_BACK);
	InitGrid();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDrawingOptionAps::OnSetActive() 
{
	if(((COptionSheet*)GetParent())->m_cbBriNo.GetSafeHwnd()!=NULL)
		((COptionSheet*)GetParent())->m_cbBriNo.ShowWindow(TRUE);

	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd *pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	if(((COptionSheet*)GetParent())->m_bSetOK)
		pOpt->m_nCurPropPage = 4;
	
	return COptionBase::OnSetActive();
}

void CDrawingOptionAps::OnSelchangeComboBridgeNo() 
{
	SetDataSave();

	m_nCurBri = ((COptionSheet*)GetParent())->m_cbBriNo.GetCurSel();

	m_Tree.DeleteAllItems();
	InitTree();
	InitGrid();
}

void CDrawingOptionAps::OnOptionDefault()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nCurBri);

	pData->DefaultDrawingOptionAps();

	m_Tree.DeleteAllItems();
	InitTree();
	InitGrid();
}

void CDrawingOptionAps::InitGrid()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nCurBri);

	long nRow	= 0;
	m_Grid.SetTextMatrix(nRow++, 0, _T("설정 항목"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("기본 스케일"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("상세'A' 스케일"));
	m_Grid.SetTextMatrix(nRow, 0, _T("철근제원간격"));

	nRow	= 0;
	m_Grid.SetTextMatrix(nRow++, 1, _T("설정 값"));
	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_dScaleRebarAps, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_dScaleDetailAps, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_dSpaceBetweenRebarDetailAps, (UINT)DT_RIGHT);

	m_Grid.SetRedraw(TRUE, TRUE);
}

void CDrawingOptionAps::InitTree()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nCurBri);
	pOpt->SyncApsDowelRebarSeperateRebarOption(m_nCurBri);


	m_Tree.SetBkColor(CLR_BACK);
	m_Tree.m_pOptions = &m_Options;

	// 기존의 있는 리스트 삭제
	for(POSITION pos=m_Tree.m_pOptions->m_Items.GetHeadPosition(); pos!=NULL;)
		delete m_Tree.m_pOptions->m_Items.GetNext(pos);
	m_Tree.m_pOptions->m_Items.RemoveAll();
	/*
	#define REBAR_APS_SIDE		0x00000001	// 단면도
	#define REBAR_APS_FRONT		0x00000002	// 정면도
	#define REBAR_APS_PLANE		0x00000004	// 평면도
	#define REBAR_APS_DETAIL_RB	0x00000008	// 철근상세
	#define REBAR_APS_DETAIL_A	0x00000010	// 상세A
	#define REBAR_APS_TABLE_M	0x00000020	// 재료표
	#define REBAR_APS_TABLE_RB	0x00000040	// 철근 재료표
	#define REBAR_APS_NOTE		0x00000080	// 기본노트
	*/
	BOOL bSide		= (pData->m_dwDrawRebarAps & REBAR_APS_SIDE) ? TRUE : FALSE;
	BOOL bFront		= (pData->m_dwDrawRebarAps & REBAR_APS_FRONT) ? TRUE : FALSE;
	BOOL bPlane		= (pData->m_dwDrawRebarAps & REBAR_APS_PLANE) ? TRUE : FALSE;
	BOOL bDetailRB	= (pData->m_dwDrawRebarAps & REBAR_APS_DETAIL_RB) ? TRUE : FALSE;
	BOOL bDetailA	= (pData->m_dwDrawRebarAps & REBAR_APS_DETAIL_A) ? TRUE : FALSE;
	BOOL bTableM	= (pData->m_dwDrawRebarAps & REBAR_APS_TABLE_M) ? TRUE : FALSE;
	BOOL bTableRB	= (pData->m_dwDrawRebarAps & REBAR_APS_TABLE_RB) ? TRUE : FALSE;
	BOOL bNote		= (pData->m_dwDrawRebarAps & REBAR_APS_NOTE) ? TRUE : FALSE;

	m_Options.SetOptionItem(_T("도면 출력"), IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption(_T("REBAR_APS_SIDE"), bSide ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("REBAR_APS_SIDE"),_T("\t단면도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption(_T("REBAR_APS_FRONT"), bFront ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("REBAR_APS_FRONT"),_T("\t정면도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption(_T("REBAR_APS_PLANE"), bPlane ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("REBAR_APS_PLANE"),_T("\t평면도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption(_T("REBAR_APS_DETAIL_RB"), bDetailRB ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("REBAR_APS_DETAIL_RB"),_T("\t철근상세"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption(_T("REBAR_APS_DETAIL_A"), bDetailA ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("REBAR_APS_DETAIL_A"),_T("\t상세A"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption(_T("REBAR_APS_TABLE_M"), bTableM ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("REBAR_APS_TABLE_M"),_T("\t재료표"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption(_T("REBAR_APS_TABLE_RB"), bTableRB ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("REBAR_APS_TABLE_RB"),_T("\t철근재료표"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption(_T("REBAR_APS_NOTE"), bNote ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("REBAR_APS_NOTE"),_T("\t기본노트"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.SetOptionItem("철근재료표", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOptionItem("\t철근재료표 표현방법", IDB_ITEMS, IDB_ITEMS);
			m_Options.SetOption(_T("철근재료표 강종별로 분리표현"), pData->m_bRebarTableSepByFyAps ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
				m_Options.SetOptionItem(_T("철근재료표 강종별로 분리표현"),_T("\t\t철근재료표 강종별로 분리표현"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0);

	m_Options.BeginTrans();
	m_Tree.Initialize(NULL);
}

void CDrawingOptionAps::SetDataSave()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nCurBri);

	pData->m_dwDrawRebarAps		= 0x000000;
	if(m_Options.GetOption(_T("REBAR_APS_SIDE")))		pData->m_dwDrawRebarAps |= REBAR_APS_SIDE;
	if(m_Options.GetOption(_T("REBAR_APS_FRONT")))		pData->m_dwDrawRebarAps |= REBAR_APS_FRONT;
	if(m_Options.GetOption(_T("REBAR_APS_PLANE")))		pData->m_dwDrawRebarAps |= REBAR_APS_PLANE;
	if(m_Options.GetOption(_T("REBAR_APS_DETAIL_RB")))	pData->m_dwDrawRebarAps |= REBAR_APS_DETAIL_RB;
	if(m_Options.GetOption(_T("REBAR_APS_DETAIL_A")))	pData->m_dwDrawRebarAps |= REBAR_APS_DETAIL_A;
	if(m_Options.GetOption(_T("REBAR_APS_TABLE_M")))	pData->m_dwDrawRebarAps |= REBAR_APS_TABLE_M;
	if(m_Options.GetOption(_T("REBAR_APS_TABLE_RB")))	pData->m_dwDrawRebarAps |= REBAR_APS_TABLE_RB;
	if(m_Options.GetOption(_T("REBAR_APS_NOTE")))		pData->m_dwDrawRebarAps |= REBAR_APS_NOTE;

	BOOL bFyAps = m_Options.GetOption("철근재료표 강종별로 분리표현") == 0? FALSE : TRUE;
	pData->m_bRebarTableSepByFyAps	= bFyAps; //m_Options.GetOption("철근재료표 강종별로 분리표현");

	pDoc->m_pARcBridgeDataStd->SyncApsDowelRebarSeperateRebarOption(pData->m_bRebarTableSepByFyAps, m_nCurBri);

	int nRow = 1;
	pData->m_dScaleRebarAps					= m_Grid.GetTextMatrixDouble(nRow++, 1);
	pData->m_dScaleDetailAps				= m_Grid.GetTextMatrixDouble(nRow++, 1);
	pData->m_dSpaceBetweenRebarDetailAps	= m_Grid.GetTextMatrixDouble(nRow++, 1);
	

}

void CDrawingOptionAps::OnOK() 
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;

	SetDataSave();

	pStd->SetModifiedAll();
	m_pDoc->SetModifiedFlag();
}

