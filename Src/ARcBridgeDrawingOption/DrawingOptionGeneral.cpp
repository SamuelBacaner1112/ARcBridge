// DrawingOptionGeneral.cpp : implementation file
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
// CDrawingOptionGeneral property page

IMPLEMENT_DYNCREATE(CDrawingOptionGeneral, COptionBase)

CDrawingOptionGeneral::CDrawingOptionGeneral() : COptionBase(CDrawingOptionGeneral::IDD)
{
	//{{AFX_DATA_INIT(CDrawingOptionGeneral)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = NULL;
	m_nCurBri = 0;
}

CDrawingOptionGeneral::~CDrawingOptionGeneral()
{
}

void CDrawingOptionGeneral::DoDataExchange(CDataExchange* pDX)
{
	COptionBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawingOptionGeneral)
	DDX_Control(pDX, IDC_OPT_TREE, m_Tree);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_OPT_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CDrawingOptionGeneral, COptionBase)
	//{{AFX_MSG_MAP(CDrawingOptionGeneral)
	ON_BN_CLICKED(IDC_OPT_DEFAULT, OnOptionDefault)
	ON_CBN_SELCHANGE(IDC_OPT_BRIDGE_NO, OnSelchangeComboBridgeNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawingOptionGeneral message handlers

BOOL CDrawingOptionGeneral::OnInitDialog() 
{
	COptionBase::OnInitDialog();

	m_nCurBri = (dynamic_cast<CARcBridgeDoc*>(m_pDoc))->m_nCurBriNoOfSettingDomyun;
	InitTree();

	long nRowCount = 10;
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

BOOL CDrawingOptionGeneral::OnSetActive() 
{
	if(((COptionSheet*)GetParent())->m_cbBriNo.GetSafeHwnd()!=NULL)
		((COptionSheet*)GetParent())->m_cbBriNo.ShowWindow(TRUE);

	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd *pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	if(((COptionSheet*)GetParent())->m_bSetOK)
		pOpt->m_nCurPropPage = 2;
	
	return COptionBase::OnSetActive();
}

void CDrawingOptionGeneral::OnSelchangeComboBridgeNo() 
{
	SetDataSave();

	m_nCurBri = ((COptionSheet*)GetParent())->m_cbBriNo.GetCurSel();

	m_Tree.DeleteAllItems();
	InitTree();
	InitGrid();
}

void CDrawingOptionGeneral::OnOptionDefault()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nCurBri);

	pData->DefaultDrawingOptionGeneral();

	m_Tree.DeleteAllItems();
	InitTree();
	InitGrid();
}

void CDrawingOptionGeneral::InitGrid()
{
	CARcBridgeDoc* pDoc					= dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd			= pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd* pOpt	= pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nCurBri);

	m_Grid.SetTextMatrix(0, 0, _T("설정 항목"));
	m_Grid.SetTextMatrix(1, 0, _T("기본 스케일"));
	m_Grid.SetTextMatrix(2, 0, _T("상세 \"A\" 스케일"));
	m_Grid.SetTextMatrix(3, 0, _T("NOTCH 상세 스케일"));
	m_Grid.SetTextMatrix(4, 0, _T("시공이음 상세 스케일"));
	m_Grid.SetTextMatrix(5, 0, _T("시공 순서도 스케일"));
	m_Grid.SetTextMatrix(6, 0, _T("날개벽 스케일"));
	m_Grid.SetTextMatrix(7, 0, _T("상세 \"B\" 스케일"));
	m_Grid.SetTextMatrix(8, 0, _T("균열유발용 줄눈 스케일"));
	m_Grid.SetTextMatrix(9, 0, _T("신축이음 상세 스케일"));

	m_Grid.SetTextMatrix(0, 1, _T("설정 값"));
	m_Grid.SetTextMatrix(1, 1, _T("%.f"), pData->m_dScaleGenBase, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(2, 1, _T("%.f"), pData->m_dScaleGenDetailA, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(3, 1, _T("%.f"), pData->m_dScaleGenNotch, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(4, 1, _T("%.f"), pData->m_dScaleGenConcJoint, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(5, 1, _T("%.f"), pData->m_dScaleGenOrder, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(6, 1, _T("%.f"), pData->m_dScaleGenWingWall, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(7, 1, _T("%.f"), pData->m_dScaleGenDetailB, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(8, 1, _T("%.f"), pData->m_dScaleGenCrack, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(9, 1, _T("%.f"), pData->m_dScaleFXJoint, (UINT)DT_RIGHT);

	m_Grid.SetRedraw(TRUE, TRUE);
}

void CDrawingOptionGeneral::InitTree()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nCurBri);
	CRcBridgeApp* pBri = pDoc->m_pARcBridgeDataStd->GetBridge(m_nCurBri);

	if( pBri == NULL ) return;

	CWingWall* pWingBL = pBri->GetWingWall(TRUE, TRUE);
	CWingWall* pWingBR = pBri->GetWingWall(TRUE, FALSE);
	CWingWall* pWingEL = pBri->GetWingWall(FALSE, TRUE);
	CWingWall* pWingER = pBri->GetWingWall(FALSE, FALSE);
	
	

	m_Tree.SetBkColor(CLR_BACK);
	m_Tree.m_pOptions = &m_Options;

	// 기존의 있는 리스트 삭제
	for(POSITION pos=m_Tree.m_pOptions->m_Items.GetHeadPosition(); pos!=NULL;)
		delete m_Tree.m_pOptions->m_Items.GetNext(pos);
	m_Tree.m_pOptions->m_Items.RemoveAll();

	BOOL bPlane	= (pData->m_dwDrawGen & GEN_PLANE) ? TRUE : FALSE;
	BOOL bCross	= (pData->m_dwDrawGen & GEN_CROSS) ? TRUE : FALSE;
	BOOL bLongi	= (pData->m_dwDrawGen & GEN_LONGI) ? TRUE : FALSE;
	BOOL bDetailA = (pData->m_dwDrawGen & GEN_DETAIL_A) ? TRUE : FALSE;
	BOOL bNotch = (pData->m_dwDrawGen & GEN_NOTCH) ? TRUE : FALSE;
	BOOL bOrder	= (pData->m_dwDrawGen & GEN_ORDER) ? TRUE : FALSE;
	BOOL bFront = (pData->m_dwDrawGen & GEN_FRONT) ? TRUE : FALSE;
	BOOL bFootPlane = (pData->m_dwDrawGen & GEN_FOOT_PLANE) ? TRUE : FALSE;
	BOOL bWallPlane = (pData->m_dwDrawGen & GEN_WALL_PLANE) ? TRUE : FALSE;
	BOOL bDetailFxJoint	= (pData->m_dwDrawGen & GEN_DETAIL_FXJOINT) ? TRUE : FALSE;
	BOOL bDetailCrack	= (pData->m_dwDrawGen & GEN_DETAIL_CRACK) ? TRUE : FALSE;
	BOOL bWingBL = pWingBL->m_bExist ? ((pData->m_dwDrawGen & GEN_WING_BL) ? TRUE : FALSE) : FALSE;
	BOOL bWingBR = pWingBR->m_bExist ? ((pData->m_dwDrawGen & GEN_WING_BR) ? TRUE : FALSE) : FALSE;
	BOOL bWingEL = pWingEL->m_bExist ? ((pData->m_dwDrawGen & GEN_WING_EL) ? TRUE : FALSE) : FALSE;
	BOOL bWingER = pWingER->m_bExist ? ((pData->m_dwDrawGen & GEN_WING_ER) ? TRUE : FALSE) : FALSE;
	BOOL bDetailC = (pData->m_dwDrawGen & GEN_DETAIL_B) ? TRUE : FALSE;
	BOOL bNote = (pData->m_dwDrawGen & GEN_NOTE) ? TRUE : FALSE;
	BOOL bDetailConcJoint = (pData->m_dwDrawGen & GEN_CONCJOINT) ? TRUE : FALSE;
	

	m_Options.SetOptionItem(_T("도면 출력"), IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption(_T("PLANE"), bPlane ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("PLANE"),_T("\t평면도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("CROSS"), bCross ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("CROSS"),_T("\t횡단면도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("LONGI"), bLongi ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("LONGI"),_T("\t종단면도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("DETAIL_A"), bDetailA ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("DETAIL_A"),_T("\t상세 \"A\""), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("NOTCH"), bNotch ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("NOTCH"),_T("\tNOTCH 상세"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("CONCJOINT"), bDetailConcJoint ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("CONCJOINT"),_T("\t시공이음 상세"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("ORDER"), bOrder ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("ORDER"),_T("\t시공 순서도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("FRONT"), bFront ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("FRONT"),_T("\t정면도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption(_T("DETAIL_B"), bDetailC ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("DETAIL_B"),_T("\t상세 \"B\""), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption(_T("NOTE"), bNote ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("NOTE"),_T("\tNOTE"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("FOOT_PLANE"), bFootPlane ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("FOOT_PLANE"),_T("\t기초 평면도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("WALL_PLANE"), bWallPlane ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("WALL_PLANE"),_T("\t벽체 평면도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("DETAIL_FXJOINT"), bDetailFxJoint ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("DETAIL_FXJOINT"),_T("\t신축이음 상세도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("DETAIL_CRACK"), bDetailCrack ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("DETAIL_CRACK"),_T("\t균열유발용 줄눈 상세도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		

		m_Options.SetOption(_T("WING_BL"), bWingBL ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("WING_BL"),_T("\t시점 좌측 날개벽"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingBL->m_bExist);
			m_Options.SetOption(_T("WING_BL_FRONT"), pData->m_bDrawGenWingWall[0][0] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("WING_BL_FRONT"),_T("\t\t정면도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingBL->m_bExist);
			m_Options.SetOption(_T("WING_BL_VERT"), pData->m_bDrawGenWingWall[0][1] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("WING_BL_VERT") ,_T("\t\t단면도(수직)"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingBL->m_bExist);
			m_Options.SetOption(_T("WING_BL_HORI"), pData->m_bDrawGenWingWall[0][2] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("WING_BL_HORI") ,_T("\t\t단면도(수평)"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingBL->m_bExist);

		m_Options.SetOption(_T("WING_BR"), bWingBR ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("WING_BR"),_T("\t시점 우측 날개벽"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingBR->m_bExist);	
			m_Options.SetOption(_T("WING_BR_FRONT"), pData->m_bDrawGenWingWall[1][0] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("WING_BR_FRONT"),_T("\t\t정면도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingBR->m_bExist);
			m_Options.SetOption(_T("WING_BR_VERT"), pData->m_bDrawGenWingWall[1][1] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("WING_BR_VERT") ,_T("\t\t단면도(수직)"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingBR->m_bExist);
			m_Options.SetOption(_T("WING_BR_HORI"), pData->m_bDrawGenWingWall[1][2] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("WING_BR_HORI") ,_T("\t\t단면도(수평)"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingBR->m_bExist);

		m_Options.SetOption(_T("WING_EL"), bWingEL ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("WING_EL"),_T("\t종점 좌측 날개벽"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingEL->m_bExist);
			m_Options.SetOption(_T("WING_EL_FRONT"), pData->m_bDrawGenWingWall[2][0] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("WING_EL_FRONT"),_T("\t\t정면도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingEL->m_bExist);
			m_Options.SetOption(_T("WING_EL_VERT"), pData->m_bDrawGenWingWall[2][1] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("WING_EL_VERT") ,_T("\t\t단면도(수직)"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingEL->m_bExist);
			m_Options.SetOption(_T("WING_EL_HORI"), pData->m_bDrawGenWingWall[2][2] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("WING_EL_HORI") ,_T("\t\t단면도(수평)"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingEL->m_bExist);

		m_Options.SetOption(_T("WING_ER"), bWingER ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("WING_ER"),_T("\t종점 우측 날개벽"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingER->m_bExist);	
			m_Options.SetOption(_T("WING_ER_FRONT"), pData->m_bDrawGenWingWall[3][0] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("WING_ER_FRONT"),_T("\t\t정면도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingER->m_bExist);
			m_Options.SetOption(_T("WING_ER_VERT"), pData->m_bDrawGenWingWall[3][1] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("WING_ER_VERT") ,_T("\t\t단면도(수직)"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingER->m_bExist);
			m_Options.SetOption(_T("WING_ER_HORI"), pData->m_bDrawGenWingWall[3][2] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("WING_ER_HORI") ,_T("\t\t단면도(수평)"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingER->m_bExist);

	m_Options.SetOptionItem(_T("출력 옵션"), IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption(_T("PLANE_XY_COORD"), pData->m_bDrawGenCoordinateXY ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("PLANE_XY_COORD"),_T("\t평면도에 좌표 표현"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("TABLE_PILE_PLANE"), pData->m_bDimTableForPilePlane ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("TABLE_PILE_PLANE"),_T("\t기초평면도 말둑 배치 간격 테이블처리"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

	m_Options.BeginTrans();
	m_Tree.Initialize(NULL);
}

void CDrawingOptionGeneral::SetDataSave()
{
	CARcBridgeDoc* pDoc					= dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd			= pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd* pOpt	= pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nCurBri);

	pData->m_dwDrawGen	= 0x000000;
	if(m_Options.GetOption(_T("PLANE")))		pData->m_dwDrawGen |= GEN_PLANE;
	if(m_Options.GetOption(_T("CROSS")))		pData->m_dwDrawGen |= GEN_CROSS;
	if(m_Options.GetOption(_T("LONGI")))		pData->m_dwDrawGen |= GEN_LONGI;
	if(m_Options.GetOption(_T("DETAIL_A")))		pData->m_dwDrawGen |= GEN_DETAIL_A;
	if(m_Options.GetOption(_T("NOTCH")))		pData->m_dwDrawGen |= GEN_NOTCH;
	if(m_Options.GetOption(_T("CONCJOINT")))	pData->m_dwDrawGen |= GEN_CONCJOINT;
	if(m_Options.GetOption(_T("ORDER")))		pData->m_dwDrawGen |= GEN_ORDER;
	if(m_Options.GetOption(_T("FRONT")))		pData->m_dwDrawGen |= GEN_FRONT;
	if(m_Options.GetOption(_T("FOOT_PLANE")))	pData->m_dwDrawGen |= GEN_FOOT_PLANE;
	if(m_Options.GetOption(_T("WALL_PLANE")))	pData->m_dwDrawGen |= GEN_WALL_PLANE;
	if(m_Options.GetOption(_T("DETAIL_FXJOINT"))) pData->m_dwDrawGen |= GEN_DETAIL_FXJOINT;
	if(m_Options.GetOption(_T("DETAIL_CRACK"))) pData->m_dwDrawGen |= GEN_DETAIL_CRACK;
	if(m_Options.GetOption(_T("WING_BL")))		pData->m_dwDrawGen |= GEN_WING_BL;
	pData->m_bDrawGenWingWall[0][0] = m_Options.GetOption(_T("WING_BL_FRONT"));
	pData->m_bDrawGenWingWall[0][1] = m_Options.GetOption(_T("WING_BL_VERT"));
	pData->m_bDrawGenWingWall[0][2] = m_Options.GetOption(_T("WING_BL_HORI"));
	if(m_Options.GetOption(_T("WING_BR")))		pData->m_dwDrawGen |= GEN_WING_BR;
	pData->m_bDrawGenWingWall[1][0] = m_Options.GetOption(_T("WING_BR_FRONT"));
	pData->m_bDrawGenWingWall[1][1] = m_Options.GetOption(_T("WING_BR_VERT"));
	pData->m_bDrawGenWingWall[1][2] = m_Options.GetOption(_T("WING_BR_HORI"));
	if(m_Options.GetOption(_T("WING_EL")))		pData->m_dwDrawGen |= GEN_WING_EL;
	pData->m_bDrawGenWingWall[2][0] = m_Options.GetOption(_T("WING_EL_FRONT"));
	pData->m_bDrawGenWingWall[2][1] = m_Options.GetOption(_T("WING_EL_VERT"));
	pData->m_bDrawGenWingWall[2][2] = m_Options.GetOption(_T("WING_EL_HORI"));
	if(m_Options.GetOption(_T("WING_ER")))		pData->m_dwDrawGen |= GEN_WING_ER;
	pData->m_bDrawGenWingWall[3][0] = m_Options.GetOption(_T("WING_ER_FRONT"));
	pData->m_bDrawGenWingWall[3][1] = m_Options.GetOption(_T("WING_ER_VERT"));
	pData->m_bDrawGenWingWall[3][2] = m_Options.GetOption(_T("WING_ER_HORI"));
	if(m_Options.GetOption(_T("DETAIL_B")))		pData->m_dwDrawGen |= GEN_DETAIL_B;
	if(m_Options.GetOption(_T("NOTE")))	pData->m_dwDrawGen |= GEN_NOTE;

	pData->m_bDrawGenCoordinateXY = m_Options.GetOption(_T("PLANE_XY_COORD"));
	pData->m_bDimTableForPilePlane	= m_Options.GetOption(_T("TABLE_PILE_PLANE"));
	

		
	int nRow = 1;
	pData->m_dScaleGenBase	= m_Grid.GetTextMatrixDouble(nRow++, 1);
	pData->m_dScaleGenDetailA = m_Grid.GetTextMatrixDouble(nRow++, 1);
	pData->m_dScaleGenNotch	= m_Grid.GetTextMatrixDouble(nRow++, 1);
	pData->m_dScaleGenConcJoint	= m_Grid.GetTextMatrixDouble(nRow++, 1);
	pData->m_dScaleGenOrder = m_Grid.GetTextMatrixDouble(nRow++, 1);
	pData->m_dScaleGenWingWall = m_Grid.GetTextMatrixDouble(nRow++, 1);
	pData->m_dScaleGenDetailB = m_Grid.GetTextMatrixDouble(nRow++, 1);
	pData->m_dScaleGenCrack = m_Grid.GetTextMatrixDouble(nRow++, 1);
	pData->m_dScaleFXJoint = m_Grid.GetTextMatrixDouble(nRow++, 1);
}

void CDrawingOptionGeneral::OnOK() 
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;

	SetDataSave();

	pStd->SetModifiedAll();
	m_pDoc->SetModifiedFlag();
}
