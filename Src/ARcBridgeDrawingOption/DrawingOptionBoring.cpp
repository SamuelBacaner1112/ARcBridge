// DrawingOptionBoring.cpp : implementation file
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
// CDrawingOptionBoring property page

IMPLEMENT_DYNCREATE(CDrawingOptionBoring, COptionBase)

CDrawingOptionBoring::CDrawingOptionBoring() : COptionBase(CDrawingOptionBoring::IDD)
{
	//{{AFX_DATA_INIT(CDrawingOptionBoring)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = NULL;
	m_nCurBri = 0;
}

CDrawingOptionBoring::~CDrawingOptionBoring()
{
}

void CDrawingOptionBoring::DoDataExchange(CDataExchange* pDX)
{
	COptionBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawingOptionBoring)
	DDX_Control(pDX, IDC_OPT_TREE, m_Tree);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_OPT_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CDrawingOptionBoring, COptionBase)
	//{{AFX_MSG_MAP(CDrawingOptionBoring)
	ON_BN_CLICKED(IDC_OPT_DEFAULT, OnOptionDefault)
	ON_CBN_SELCHANGE(IDC_OPT_BRIDGE_NO, OnSelchangeComboBridgeNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawingOptionBoring message handlers

BOOL CDrawingOptionBoring::OnInitDialog() 
{
	COptionBase::OnInitDialog();

	m_nCurBri = (dynamic_cast<CARcBridgeDoc*>(m_pDoc))->m_nCurBriNoOfSettingDomyun;
	InitTree();

	long nRowCount = 2;
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

BOOL CDrawingOptionBoring::OnSetActive() 
{
	if(((COptionSheet*)GetParent())->m_cbBriNo.GetSafeHwnd()!=NULL)
		((COptionSheet*)GetParent())->m_cbBriNo.ShowWindow(TRUE);

	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd *pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	if(((COptionSheet*)GetParent())->m_bSetOK)
		pOpt->m_nCurPropPage = 8;
	
	return COptionBase::OnSetActive();
}

void CDrawingOptionBoring::OnSelchangeComboBridgeNo() 
{
	SetDataSave();

	m_nCurBri = ((COptionSheet*)GetParent())->m_cbBriNo.GetCurSel();

	m_Tree.DeleteAllItems();
	InitTree();
	InitGrid();
}

void CDrawingOptionBoring::OnOptionDefault()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nCurBri);

	pData->DefaultDrawingOptionBoring();

	m_Tree.DeleteAllItems();
	InitTree();
	InitGrid();
}

void CDrawingOptionBoring::InitGrid()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nCurBri);

	long nRow	= 0;
	m_Grid.SetTextMatrix(nRow++, 0, _T("설정 항목"));
	m_Grid.SetTextMatrix(nRow, 0, _T("기본 스케일"));
	
	nRow	= 0;
	m_Grid.SetTextMatrix(nRow++, 1, _T("설정 값"));
	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_dScaleBoring, (UINT)DT_RIGHT);
	
	m_Grid.SetRedraw(TRUE, TRUE);
}

void CDrawingOptionBoring::InitTree()
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

	BOOL bVerSect	= (pData->m_dwDrawBoring & BORING_VERSECT) ? TRUE : FALSE;
	BOOL bForce		= (pData->m_dwDrawBoring & BORING_FORCE) ? TRUE : FALSE;
	BOOL bLegend		= (pData->m_dwDrawBoring & BORING_LEGEND) ? TRUE : FALSE;
	BOOL bNote	= (pData->m_dwDrawBoring & BORING_NOTE) ? TRUE : FALSE;

	m_Options.SetOptionItem(_T("도면 출력"), IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption(_T("VERSECT"), bVerSect ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("VERSECT"),_T("\t단면도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("TABLE"), bForce ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("TABLE"),_T("\t테이블"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("LEGEND"), bLegend ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("LEGEND"),_T("\t범례"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption(_T("NOTE"), bNote ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("NOTE"),_T("\tNOTE"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
//	m_Options.SetOptionItem(_T("출력 옵션"), IDB_ITEMS, IDB_ITEMS);

	m_Options.BeginTrans();
	m_Tree.Initialize(NULL);
}

void CDrawingOptionBoring::SetDataSave()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nCurBri);

	pData->m_dwDrawBoring	= 0x000000;
	if(m_Options.GetOption(_T("VERSECT")))	pData->m_dwDrawBoring |= BORING_VERSECT;
	if(m_Options.GetOption(_T("TABLE")))	pData->m_dwDrawBoring |= BORING_FORCE;
	if(m_Options.GetOption(_T("LEGEND")))	pData->m_dwDrawBoring |= BORING_LEGEND;
	if(m_Options.GetOption(_T("NOTE")))	pData->m_dwDrawBoring |= BORING_NOTE;
	
	int nRow = 1;
	pData->m_dScaleBoring		= m_Grid.GetTextMatrixDouble(nRow++, 1);
}

void CDrawingOptionBoring::OnOK() 
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;

	SetDataSave();

	pStd->SetModifiedAll();
	m_pDoc->SetModifiedFlag();
}
