// DrawingOptionOthers.cpp : implementation file
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
// CDrawingOptionOthers property page

IMPLEMENT_DYNCREATE(CDrawingOptionOthers, COptionBase)

CDrawingOptionOthers::CDrawingOptionOthers() : COptionBase(CDrawingOptionOthers::IDD)
{
	//{{AFX_DATA_INIT(CDrawingOptionOthers)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = NULL;
	m_nCurBri = 0;
}

CDrawingOptionOthers::~CDrawingOptionOthers()
{
}

void CDrawingOptionOthers::DoDataExchange(CDataExchange* pDX)
{
	COptionBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawingOptionOthers)
	DDX_Control(pDX, IDC_OPT_TREE, m_Tree);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_OPT_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CDrawingOptionOthers, COptionBase)
	//{{AFX_MSG_MAP(CDrawingOptionOthers)
	ON_BN_CLICKED(IDC_OPT_DEFAULT, OnOptionDefault)
	ON_CBN_SELCHANGE(IDC_OPT_BRIDGE_NO, OnSelchangeComboBridgeNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawingOptionOthers message handlers

BOOL CDrawingOptionOthers::OnInitDialog() 
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

BOOL CDrawingOptionOthers::OnSetActive() 
{
	if(((COptionSheet*)GetParent())->m_cbBriNo.GetSafeHwnd()!=NULL)
		((COptionSheet*)GetParent())->m_cbBriNo.ShowWindow(TRUE);

	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd *pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	if(((COptionSheet*)GetParent())->m_bSetOK)
		pOpt->m_nCurPropPage = 6;
	
	return COptionBase::OnSetActive();
}

void CDrawingOptionOthers::OnSelchangeComboBridgeNo() 
{
	SetDataSave();

	m_nCurBri = ((COptionSheet*)GetParent())->m_cbBriNo.GetCurSel();

	m_Tree.DeleteAllItems();
	InitTree();
	InitGrid();
}

void CDrawingOptionOthers::OnOptionDefault()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nCurBri);

	pData->DefaultDrawingOptionOthers();

	m_Tree.DeleteAllItems();
	InitTree();
	InitGrid();
}

void CDrawingOptionOthers::InitGrid()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nCurBri);


	long nRow	= 0;

	m_Grid.SetTextMatrix(nRow++, 0, _T("설정 항목"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("기본 스케일"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("방호벽 상세 스케일"));
	m_Grid.SetTextMatrix(nRow, 0, _T("교명주 상세 스케일"));

	nRow	= 0;
	m_Grid.SetTextMatrix(nRow++, 1, _T("설정 값"));
	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_dScaleRebarOthers, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_dScaleDetailGuardWall, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_dScaleDetailMonument, (UINT)DT_RIGHT);

	m_Grid.SetRedraw(TRUE, TRUE);
}

void CDrawingOptionOthers::InitTree()
{
	m_Tree.SetBkColor(CLR_BACK);
	m_Tree.m_pOptions = &m_Options;

	// 기존의 있는 리스트 삭제
	for(POSITION pos=m_Tree.m_pOptions->m_Items.GetHeadPosition(); pos!=NULL;)
		delete m_Tree.m_pOptions->m_Items.GetNext(pos);
	m_Tree.m_pOptions->m_Items.RemoveAll();
}

void CDrawingOptionOthers::SetDataSave()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nCurBri);

	long nRow = 1;
	pData->m_dScaleRebarOthers		= m_Grid.GetTextMatrixDouble(nRow++, 1);
	pData->m_dScaleDetailGuardWall	= m_Grid.GetTextMatrixDouble(nRow++, 1);
	pData->m_dScaleDetailMonument	= m_Grid.GetTextMatrixDouble(nRow++, 1);
}

void CDrawingOptionOthers::OnOK() 
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;

	SetDataSave();

	pStd->SetModifiedAll();
	m_pDoc->SetModifiedFlag();
}
