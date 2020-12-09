// DrawingOptionVerticalPlane.cpp : implementation file
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
// CDrawingOptionVerticalPlane property page

IMPLEMENT_DYNCREATE(CDrawingOptionVerticalPlane, COptionBase)

CDrawingOptionVerticalPlane::CDrawingOptionVerticalPlane() : COptionBase(CDrawingOptionVerticalPlane::IDD)
{
	//{{AFX_DATA_INIT(CDrawingOptionVerticalPlane)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = NULL;
	m_nCurBri = 0;
}

CDrawingOptionVerticalPlane::~CDrawingOptionVerticalPlane()
{
}

void CDrawingOptionVerticalPlane::DoDataExchange(CDataExchange* pDX)
{
	COptionBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawingOptionVerticalPlane)
	DDX_Control(pDX, IDC_OPT_TREE, m_Tree);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_OPT_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CDrawingOptionVerticalPlane, COptionBase)
	//{{AFX_MSG_MAP(CDrawingOptionVerticalPlane)
	ON_BN_CLICKED(IDC_OPT_DEFAULT, OnOptionDefault)
	ON_CBN_SELCHANGE(IDC_OPT_BRIDGE_NO, OnSelchangeComboBridgeNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawingOptionVerticalPlane message handlers

BOOL CDrawingOptionVerticalPlane::OnInitDialog() 
{
	COptionBase::OnInitDialog();

	m_nCurBri = (dynamic_cast<CARcBridgeDoc*>(m_pDoc))->m_nCurBriNoOfSettingDomyun;

	InitTree();

	long nRowCount = 5;
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

BOOL CDrawingOptionVerticalPlane::OnSetActive() 
{
	if(((COptionSheet*)GetParent())->m_cbBriNo.GetSafeHwnd()!=NULL)
		((COptionSheet*)GetParent())->m_cbBriNo.ShowWindow(TRUE);

	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd *pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	if(((COptionSheet*)GetParent())->m_bSetOK)
		pOpt->m_nCurPropPage = 1;
	
	return COptionBase::OnSetActive();
}

void CDrawingOptionVerticalPlane::OnSelchangeComboBridgeNo() 
{
	SetDataSave();

	m_nCurBri = ((COptionSheet*)GetParent())->m_cbBriNo.GetCurSel();


	m_Tree.DeleteAllItems();
	InitTree();
	InitGrid();
}

void CDrawingOptionVerticalPlane::OnOptionDefault()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nCurBri);

	pData->DefaultDrawingOptionVerticalPlane();

	m_Tree.DeleteAllItems();
	InitTree();
	InitGrid();
}

void CDrawingOptionVerticalPlane::InitGrid()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nCurBri);

	m_Grid.SetTextMatrix(0, 0, _T("설정 항목"));
	m_Grid.SetTextMatrix(1, 0, _T("현황 파일"));
	m_Grid.SetTextMatrix(2, 0, _T("기본 스케일"));
	m_Grid.SetTextMatrix(3, 0, _T("횡단면도 스케일"));
	m_Grid.SetTextMatrix(4, 0, _T("위치도 스케일"));

	m_Grid.SetTextMatrix(0, 1, _T("설정 값"));
	m_Grid.SetTextMatrix(1, 1, pData->m_szDxfPlanName, (UINT)DT_LEFT);
	m_Grid.SetCellType(1, 1, CT_FILEBUTTON);
	m_Grid.SetInitFileDlgText(_T("현황 파일 (.dxf)|*.dxf||"));
	m_Grid.SetTextMatrix(2, 1, _T("%.f"), pData->m_dScaleVPBase, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(3, 1, _T("%.f"), pData->m_dScaleVPCross, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(4, 1, _T("%.f"), pData->m_dScaleVPLocation, (UINT)DT_RIGHT);

	m_Grid.SetRedraw(TRUE, TRUE);
}

void CDrawingOptionVerticalPlane::InitTree()
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

	BOOL bPlane = (pData->m_dwDrawVP & VP_PLANE) ? TRUE : FALSE;
	BOOL bCross = (pData->m_dwDrawVP & VP_CROSS) ? TRUE : FALSE;
	BOOL bVert = (pData->m_dwDrawVP & VP_LONGI) ? TRUE : FALSE;
	BOOL bLoc = (pData->m_dwDrawVP & VP_LOCATION) ? TRUE : FALSE;
	BOOL bCant = (pData->m_dwDrawVP & VP_CANT) ? TRUE : FALSE;
	BOOL bLSlope = (pData->m_dwDrawVP & VP_LONGI_SLOPE) ? TRUE : FALSE;
	BOOL bLegend = (pData->m_dwDrawVP & VP_LEGEND) ? TRUE : FALSE;
	BOOL bNote = (pData->m_dwDrawVP & VP_NOTE) ? TRUE : FALSE;

	m_Options.SetOptionItem(_T("도면 출력"), IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption(_T("PLANE"), bPlane ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("PLANE"),_T("\t평면도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("CROSS"), bCross ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("CROSS"),_T("\t횡단면도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("LONGI"), bVert ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("LONGI"),_T("\t종단면도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("LOCATION"), bLoc ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("LOCATION"),_T("\t위치도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("CANT"), bCant ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("CANT"),_T("\t편경사도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("LONGI_SLOPE"), bLSlope ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("LONGI_SLOPE"),_T("\t종경사도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("LEGEND"), bLegend ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("LEGEND"),_T("\t범례"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("NOTE"), bNote ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("NOTE"),_T("\t기본 노트"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

	m_Options.SetOptionItem(_T("출력 옵션"), IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption(_T("PLANE_XY_COORD"), pData->m_bDrawVPCoordinateXY ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("PLANE_XY_COORD"),_T("\t평면도에 좌표 표현"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

	m_Options.BeginTrans();
	m_Tree.Initialize(NULL);
}

void CDrawingOptionVerticalPlane::SetDataSave()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nCurBri);

	pData->m_dwDrawVP = 0x000000;
	if(m_Options.GetOption(_T("PLANE"))) pData->m_dwDrawVP |= VP_PLANE;
	if(m_Options.GetOption(_T("CROSS"))) pData->m_dwDrawVP |= VP_CROSS;
	if(m_Options.GetOption(_T("LONGI"))) pData->m_dwDrawVP |= VP_LONGI;
	if(m_Options.GetOption(_T("LOCATION"))) pData->m_dwDrawVP |= VP_LOCATION;	
	if(m_Options.GetOption(_T("CANT"))) pData->m_dwDrawVP |= VP_CANT;
	if(m_Options.GetOption(_T("LONGI_SLOPE"))) pData->m_dwDrawVP |= VP_LONGI_SLOPE;
	if(m_Options.GetOption(_T("LEGEND"))) pData->m_dwDrawVP |= VP_LEGEND;
	if(m_Options.GetOption(_T("NOTE"))) pData->m_dwDrawVP |= VP_NOTE;

	pData->m_bDrawVPCoordinateXY = m_Options.GetOption(_T("PLANE_XY_COORD"));

	int nRow = 1;
	pData->m_szDxfPlanName    = m_Grid.GetTextMatrix(nRow++, 1);
	pData->m_dScaleVPBase     = m_Grid.GetTextMatrixDouble(nRow++, 1);
	pData->m_dScaleVPCross	  = m_Grid.GetTextMatrixDouble(nRow++, 1);
	pData->m_dScaleVPLocation = m_Grid.GetTextMatrixDouble(nRow++, 1);
}

void CDrawingOptionVerticalPlane::OnOK() 
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;

	SetDataSave();

	pStd->SetModifiedAll();
	m_pDoc->SetModifiedFlag();
}
