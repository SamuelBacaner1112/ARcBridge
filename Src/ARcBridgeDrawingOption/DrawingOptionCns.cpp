// DrawingOptionCns.cpp : implementation file
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
// CDrawingOptionCns property page

IMPLEMENT_DYNCREATE(CDrawingOptionCns, COptionBase)

CDrawingOptionCns::CDrawingOptionCns() : COptionBase(CDrawingOptionCns::IDD)
{
	//{{AFX_DATA_INIT(CDrawingOptionCns)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = NULL;
	m_nCurBri = 0;
}

CDrawingOptionCns::~CDrawingOptionCns()
{
}

void CDrawingOptionCns::DoDataExchange(CDataExchange* pDX)
{
	COptionBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawingOptionCns)
	DDX_Control(pDX, IDC_OPT_TREE, m_Tree);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_OPT_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CDrawingOptionCns, COptionBase)
	//{{AFX_MSG_MAP(CDrawingOptionCns)
	ON_BN_CLICKED(IDC_OPT_DEFAULT, OnOptionDefault)
	ON_CBN_SELCHANGE(IDC_OPT_BRIDGE_NO, OnSelchangeComboBridgeNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawingOptionCns message handlers

BOOL CDrawingOptionCns::OnInitDialog() 
{
	COptionBase::OnInitDialog();
	m_nCurBri = (dynamic_cast<CARcBridgeDoc*>(m_pDoc))->m_nCurBriNoOfSettingDomyun;

	InitTree();

	long nRowCount = 3;
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

BOOL CDrawingOptionCns::OnSetActive() 
{
	if(((COptionSheet*)GetParent())->m_cbBriNo.GetSafeHwnd()!=NULL)
		((COptionSheet*)GetParent())->m_cbBriNo.ShowWindow(TRUE);

	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd *pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	if(((COptionSheet*)GetParent())->m_bSetOK)
		pOpt->m_nCurPropPage = 5;
	
	return COptionBase::OnSetActive();
}

void CDrawingOptionCns::OnSelchangeComboBridgeNo() 
{
	SetDataSave();

	m_nCurBri = ((COptionSheet*)GetParent())->m_cbBriNo.GetCurSel();

	m_Tree.DeleteAllItems();
	InitTree();
	InitGrid();
}

void CDrawingOptionCns::OnOptionDefault()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nCurBri);

	pData->DefaultDrawingOptionCns();

	m_Tree.DeleteAllItems();
	InitTree();
	InitGrid();
}

void CDrawingOptionCns::InitGrid()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nCurBri);

	long nRow	= 0;
	m_Grid.SetTextMatrix(nRow++, 0, _T("설정 항목"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("기본 스케일"));
	m_Grid.SetTextMatrix(nRow, 0, _T("철근제원간격"));

	nRow	= 0;
	m_Grid.SetTextMatrix(nRow++, 1, _T("설정 값"));
	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_dScaleRebarCns, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_dSpaceBetweenRebarDetailCns, (UINT)DT_RIGHT);


	m_Grid.SetRedraw(TRUE, TRUE);
}

void CDrawingOptionCns::InitTree()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nCurBri);

	m_Tree.SetBkColor(CLR_BACK);
	m_Tree.m_pOptions = &m_Options;

	// 기존의 있는 리스트 삭제
	for(POSITION pos=m_Tree.m_pOptions->m_Items.GetHeadPosition(); pos!=NULL;)
		delete m_Tree.m_pOptions->m_Items.GetNext(pos);
	m_Tree.m_pOptions->m_Items.RemoveAll();

	// 리스트 추가.
	BOOL bSide		= (pData->m_dwDrawRebarCns & REBAR_CNS_SIDE) ? TRUE : FALSE;
	BOOL bFront		= (pData->m_dwDrawRebarCns & REBAR_CNS_FRONT) ? TRUE : FALSE;
	BOOL bPlane		= (pData->m_dwDrawRebarCns & REBAR_CNS_PLANE) ? TRUE : FALSE;
	BOOL bDetailRB	= (pData->m_dwDrawRebarCns & REBAR_CNS_DETAIL_RB) ? TRUE : FALSE;
	BOOL bDetailA	= (pData->m_dwDrawRebarCns & REBAR_CNS_DETAIL_A) ? TRUE : FALSE;
	BOOL bTableM	= (pData->m_dwDrawRebarCns & REBAR_CNS_TABLE_M) ? TRUE : FALSE;
	BOOL bTableRB	= (pData->m_dwDrawRebarCns & REBAR_CNS_TABLE_RB) ? TRUE : FALSE;
	BOOL bNote		= (pData->m_dwDrawRebarCns & REBAR_CNS_NOTE) ? TRUE : FALSE;

	m_Options.SetOptionItem(_T("도면 출력"), IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption(_T("REBAR_CNS_SIDE"), bSide ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("REBAR_CNS_SIDE"),_T("\t단면도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption(_T("REBAR_CNS_FRONT"), bFront ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("REBAR_CNS_FRONT"),_T("\t정면도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption(_T("REBAR_CNS_PLANE"), bPlane ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("REBAR_CNS_PLANE"),_T("\t평면도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption(_T("REBAR_CNS_DETAIL_RB"), bDetailRB ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("REBAR_CNS_DETAIL_RB"),_T("\t철근상세"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption(_T("REBAR_CNS_DETAIL_A"), bDetailA ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("REBAR_CNS_DETAIL_A"),_T("\t상세A"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption(_T("REBAR_CNS_TABLE_M"), bTableM ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("REBAR_CNS_TABLE_M"),_T("\t재료표"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption(_T("REBAR_CNS_TABLE_RB"), bTableRB ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("REBAR_CNS_TABLE_RB"),_T("\t철근재료표"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption(_T("REBAR_CNS_NOTE"), bNote ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("REBAR_CNS_NOTE"),_T("\t기본노트"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.SetOptionItem("완충슬래브 구조도 표현방법", IDB_ITEMS, IDB_ITEMS);
//		m_Options.SetOption("TYPE_CNSREBAR", pData->m_nTypeCnsRebar);
//		m_Options.SetOptionItem("TYPE_CNSREBAR", "\t완충슬래브1, 2동시 표현", 0, IDB_RADIOON, IDB_RADIOOFF);
//		m_Options.SetOptionItem("TYPE_CNSREBAR", "\t완충슬래브1, 2분리 표현", 1, IDB_RADIOON, IDB_RADIOOFF);

		m_Options.SetOption(_T("TYPE_CNSREBAR"), (pData->m_nTypeCnsRebar==1)? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("TYPE_CNSREBAR"),_T("\t완충슬래브 1,2분리 표현"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);

	m_Options.BeginTrans();
	m_Tree.Initialize(NULL);
}

void CDrawingOptionCns::SetDataSave()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nCurBri);

	pData->m_nTypeCnsRebar		= (m_Options.GetOption(_T("TYPE_CNSREBAR")))? 1 : 0 ;

	pData->m_dwDrawRebarCns		= 0x000000;
	if(m_Options.GetOption(_T("REBAR_CNS_SIDE")))		pData->m_dwDrawRebarCns |= REBAR_CNS_SIDE;
	if(m_Options.GetOption(_T("REBAR_CNS_FRONT")))		pData->m_dwDrawRebarCns |= REBAR_CNS_FRONT;
	if(m_Options.GetOption(_T("REBAR_CNS_PLANE")))		pData->m_dwDrawRebarCns |= REBAR_CNS_PLANE;
	if(m_Options.GetOption(_T("REBAR_CNS_DETAIL_RB")))	pData->m_dwDrawRebarCns |= REBAR_CNS_DETAIL_RB;
	if(m_Options.GetOption(_T("REBAR_CNS_DETAIL_A")))	pData->m_dwDrawRebarCns |= REBAR_CNS_DETAIL_A;
	if(m_Options.GetOption(_T("REBAR_CNS_TABLE_M")))	pData->m_dwDrawRebarCns |= REBAR_CNS_TABLE_M;
	if(m_Options.GetOption(_T("REBAR_CNS_TABLE_RB")))	pData->m_dwDrawRebarCns |= REBAR_CNS_TABLE_RB;
	if(m_Options.GetOption(_T("REBAR_CNS_NOTE")))		pData->m_dwDrawRebarCns |= REBAR_CNS_NOTE;

	
	int nRow = 1;
	pData->m_dScaleRebarCns					= m_Grid.GetTextMatrixDouble(nRow++, 1);
	pData->m_dSpaceBetweenRebarDetailCns	= m_Grid.GetTextMatrixDouble(nRow++, 1);
}

void CDrawingOptionCns::OnOK() 
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;

	SetDataSave();

	pStd->SetModifiedAll();
	m_pDoc->SetModifiedFlag();
}
