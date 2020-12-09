// DrawingOptionRebar.cpp : implementation file
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
// CDrawingOptionRebar property page

IMPLEMENT_DYNCREATE(CDrawingOptionRebar, COptionBase)

CDrawingOptionRebar::CDrawingOptionRebar() : COptionBase(CDrawingOptionRebar::IDD)
{
	//{{AFX_DATA_INIT(CDrawingOptionRebar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = NULL;
	m_nCurBri = 0;
}

CDrawingOptionRebar::~CDrawingOptionRebar()
{
}

void CDrawingOptionRebar::DoDataExchange(CDataExchange* pDX)
{
	COptionBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawingOptionRebar)
	DDX_Control(pDX, IDC_OPT_TREE, m_Tree);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_OPT_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CDrawingOptionRebar, COptionBase)
	//{{AFX_MSG_MAP(CDrawingOptionRebar)
	ON_BN_CLICKED(IDC_OPT_DEFAULT, OnOptionDefault)
	ON_CBN_SELCHANGE(IDC_OPT_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_OPT_GRID, OnCellChangedData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawingOptionRebar message handlers

BOOL CDrawingOptionRebar::OnInitDialog() 
{
	COptionBase::OnInitDialog();

	m_nCurBri = (dynamic_cast<CARcBridgeDoc*>(m_pDoc))->m_nCurBriNoOfSettingDomyun;
	InitTree();

	CARcBridgeDoc* pDoc					= dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CRcBridgeRebar* pBri = pDoc->m_pARcBridgeDataStd->GetBridgeRebar(m_nCurBri);
	if( pBri == NULL ) return TRUE;

	long nRowCount = 14;
	if(pBri->IsBoxType()) nRowCount = 15;

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

BOOL CDrawingOptionRebar::OnSetActive() 
{
	if(((COptionSheet*)GetParent())->m_cbBriNo.GetSafeHwnd()!=NULL)
		((COptionSheet*)GetParent())->m_cbBriNo.ShowWindow(TRUE);

	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd *pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	if(((COptionSheet*)GetParent())->m_bSetOK)
		pOpt->m_nCurPropPage = 3;
	
	return COptionBase::OnSetActive();
}

void CDrawingOptionRebar::OnSelchangeComboBridgeNo() 
{
	SetDataSave();

	m_nCurBri = ((COptionSheet*)GetParent())->m_cbBriNo.GetCurSel();

	m_Tree.DeleteAllItems();
	InitTree();
	InitGrid();
}

void CDrawingOptionRebar::OnOptionDefault()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nCurBri);

	pData->DefaultDrawingOptionRebar();

	m_Tree.DeleteAllItems();
	InitTree();
	InitGrid();
}

void CDrawingOptionRebar::InitGrid()
{
	CARcBridgeDoc* pDoc					= dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd			= pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd* pOpt	= pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nCurBri);
	CRcBridgeRebar* pBri = pDoc->m_pARcBridgeDataStd->GetBridgeRebar(m_nCurBri);

	long nRow	= 0;
	m_Grid.SetTextMatrix(nRow++, 0, _T("���� �׸�"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("�⺻ ������"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("������ ������ ������"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("BMD ������"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("SFD ������"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("AFD ������"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("��ġ���� ���Ǹ���� �� ������"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("��� �ܸ� �� ������"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("�Ŵ� �ܸ� �� ������"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("������ �а��� �� ������"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("��ü ������ �� ������"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("���� �а��� �� ������"));
	if(pBri->IsBoxType())
		m_Grid.SetTextMatrix(nRow++, 0, _T("�λ�������� ������"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("ö����������"));
	m_Grid.SetTextMatrix(nRow, 0, _T("���̺�� ����Ÿ��"));

	nRow	= 0;
	m_Grid.SetTextMatrix(nRow++, 1, _T("���� ��"));
	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_dScaleRebarBase, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_dScaleRebarWingWall, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_dScaleBMD, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_dScaleSFD, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_dScaleAFD, (UINT)DT_RIGHT);

	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_dScaleRebarDetailArchRibSpringging, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_dScaleRebarDetailColumnSection, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_dScaleRebarDetailGirderSection, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_dScaleRebarDetailSlabAcute, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_dScaleRebarDetailWallObtuse, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_dScaleRebarDetailFootingAcute, (UINT)DT_RIGHT);
	if(pBri->IsBoxType())
		m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_dScaleRebarDetailPRF, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_dSpaceBetweenRebarDetail, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_nCountRowEachTable, (UINT)DT_RIGHT);
	
	m_Grid.SetRedraw(TRUE, TRUE);
}

void CDrawingOptionRebar::InitTree()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nCurBri);
	CRcBridgeRebar* pBri = pDoc->m_pARcBridgeDataStd->GetBridgeRebar(m_nCurBri);
	if( pBri == NULL ) return;

	CWingWall* pWingBL = pBri->GetWingWall(TRUE, TRUE);
	CWingWall* pWingBR = pBri->GetWingWall(TRUE, FALSE);
	CWingWall* pWingEL = pBri->GetWingWall(FALSE, TRUE);
	CWingWall* pWingER = pBri->GetWingWall(FALSE, FALSE);
	
	m_Tree.SetBkColor(CLR_BACK);
	m_Tree.m_pOptions = &m_Options;

	// ������ �ִ� ����Ʈ ����
	for(POSITION pos=m_Tree.m_pOptions->m_Items.GetHeadPosition(); pos!=NULL;)
		delete m_Tree.m_pOptions->m_Items.GetNext(pos);
	m_Tree.m_pOptions->m_Items.RemoveAll();

	BOOL bStdSect = (pData->m_dwDrawRebar & REBAR_STD_SECT) ? TRUE : FALSE;
	BOOL bNumberingMainRebarInStdSect	= pData->m_bMainRebarNumberingInStandardSection ? TRUE : FALSE;
	BOOL bMainAssm = (pData->m_dwDrawRebar & REBAR_MAIN_ASSM) ? TRUE : FALSE;
	BOOL bLenInfoMainAssm	= pData->m_bLengthInfoInMainRebarNumbering;
	BOOL bBMD = (pData->m_dwDrawRebar & REBAR_BMD) ? TRUE : FALSE;
	BOOL bSFD = (pData->m_dwDrawRebar & REBAR_SFD) ? TRUE : FALSE;
	BOOL bAFD	= (pData->m_dwDrawRebar & REBAR_AFD) ? TRUE : FALSE;
	BOOL bCoupler	= (pData->m_dwDrawRebar & REBAR_DETAIL_COUPLER) ? TRUE : FALSE;
	BOOL bUpperSlab = (pData->m_dwDrawRebar & REBAR_UPPER_SLAB) ? TRUE : FALSE;
	BOOL bLowerSlab = (pData->m_dwDrawRebar & REBAR_LOWER_SLAB) ? TRUE : FALSE;
	BOOL bCrossSection	= (pData->m_dwDrawRebar & REBAR_CROSSSECTION) ? TRUE : FALSE;
	BOOL bExcludeDoupleRebar	= pData->m_bExcludeDoupleMainRebarCrossSection;
	BOOL bBeginWall = (pData->m_dwDrawRebar & REBAR_BEGIN_WALL) ? TRUE : FALSE;
	BOOL bEndWall = (pData->m_dwDrawRebar & REBAR_END_WALL) ? TRUE : FALSE;
	BOOL bInWall = (pData->m_dwDrawRebar & REBAR_INWALL_FRONT) ? TRUE : FALSE;
	BOOL bInWallSect = (pData->m_dwDrawRebar & REBAR_INWALL_SECT) ? TRUE : FALSE;
	BOOL bFootingPlane = (pData->m_dwDrawRebar & REBAR_FOOTING_PLANE) ? TRUE : FALSE;
	BOOL bColumnPlane = (pData->m_dwDrawRebar & REBAR_COLUMN_PLANE) ? TRUE : FALSE;
	BOOL bGirderSect = (pData->m_dwDrawRebar & REBAR_GIRDER_SECT) ? TRUE : FALSE;
	BOOL bDetail = (pData->m_dwDrawRebar & REBAR_DETAIL) ? TRUE : FALSE;
	BOOL bWingBL = pWingBL->m_bExist ? ((pData->m_dwDrawRebar & REBAR_WING_BL) ? TRUE : FALSE) : FALSE;
	BOOL bWingBR = pWingBR->m_bExist ? ((pData->m_dwDrawRebar & REBAR_WING_BR) ? TRUE : FALSE) : FALSE;
	BOOL bWingEL = pWingEL->m_bExist ? ((pData->m_dwDrawRebar & REBAR_WING_EL) ? TRUE : FALSE) : FALSE;
	BOOL bWingER = pWingER->m_bExist ? ((pData->m_dwDrawRebar & REBAR_WING_ER) ? TRUE : FALSE) : FALSE;
	BOOL bDetailChairBlock	= (pData->m_dwDrawRebar & REBAR_DETAIL_CHAIR_BLOCK) ? TRUE : FALSE;
	BOOL bDetailPRF = pBri->m_bExistProtectionRiseFooting ? (pData->m_dwDrawRebar & REBAR_DETAIL_PRF) ? TRUE : FALSE : FALSE;

	// ����Ʈ �߰�.
	m_Options.SetOptionItem(_T("���� ���"), IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption(_T("STD_SECT"), bStdSect ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("STD_SECT"),_T("\tǥ�� �ܸ鵵"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption(_T("NUMMB_STD_SECT"), bNumberingMainRebarInStdSect ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("NUMMB_STD_SECT"),_T("\t\t��ö�� �ѹ���"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("MAIN_ASSM"), bMainAssm ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("MAIN_ASSM"),_T("\t��ö�� ������"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption("TYPE_MAIN_ASSM", pData->m_nTypeMainRebarAssm);
			if(pBri->GetCountCycleMainRebar()==2)
			{
				m_Options.SetOptionItem("TYPE_MAIN_ASSM", "\t\t��, �� Cycle",0, IDB_RADIOON, IDB_RADIOOFF);
				m_Options.SetOptionItem("TYPE_MAIN_ASSM", "\t\t��+�� Cycle",1, IDB_RADIOON, IDB_RADIOOFF);
			}
			else
			{
				m_Options.SetOptionItem("TYPE_MAIN_ASSM", "\t\t��, ��, ��+�� Cycle",0, IDB_RADIOON, IDB_RADIOOFF);
				m_Options.SetOptionItem("TYPE_MAIN_ASSM", "\t\t��+��, ��+�� Cycle",1, IDB_RADIOON, IDB_RADIOOFF);
			}
			m_Options.SetOption(_T("LENINFO_MAIN_ASSM"), bLenInfoMainAssm ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("LENINFO_MAIN_ASSM"),_T("\t\tö�ٹ�ȣ�� �������� ǥ��"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("BMD"), bBMD ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("BMD"),_T("\tBMD"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("SFD"), bSFD ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("SFD"),_T("\tSFD"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("AFD"), bAFD ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("AFD"),_T("\tAFD"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("COUPLER"), bCoupler ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("COUPLER"),_T("\tĿ�÷��󼼵�"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("UPPER_SLAB"), bUpperSlab ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("UPPER_SLAB"),_T("\t��� ������ ������"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption("TYPE_DIM_PLANE_SUPPORT", pData->m_nBasePosPlaneSupportRebarDim);
			m_Options.SetOptionItem("TYPE_DIM_PLANE_SUPPORT", "\t\t������ sta. ġ��ǥ��",0, IDB_RADIOON, IDB_RADIOOFF);
			m_Options.SetOptionItem("TYPE_DIM_PLANE_SUPPORT", "\t\t�����߽� ġ��ǥ��",1, IDB_RADIOON, IDB_RADIOOFF);

		if(pBri->IsBoxType())
		{
			m_Options.SetOption(_T("LOWER_SLAB"), bLowerSlab ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("LOWER_SLAB"),_T("\t�Ϻ� ������ ������"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		}
		m_Options.SetOption(_T("CROSSSECTION"), bCrossSection ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("CROSSSECTION"),_T("\tȾ�ܸ� ������"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption(_T("EXCLUDE_DOUPLEREBAR_CROSSSECTION"), bExcludeDoupleRebar ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("EXCLUDE_DOUPLEREBAR_CROSSSECTION"),_T("\t\t������ ö�ٰ� ��ġ�� ����� ö�� ����"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("BEGIN_WALL"), bBeginWall ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("BEGIN_WALL"),_T("\t���� ��ü ������"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("END_WALL"), bEndWall ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("END_WALL"),_T("\t���� ��ü ������"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("INWALL_FRONT"), bInWall ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("INWALL_FRONT"),_T("\t�߰� ��ü ������"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("INWALL_SECT"), bInWallSect ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("INWALL_SECT"),_T("\t�߰� ��ü �ܸ� ������"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("FOOTING_PLANE"), bFootingPlane ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("FOOTING_PLANE"),_T("\t���� ��� ������"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		m_Options.SetOption(_T("COLUMN_PLANE"), bColumnPlane ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("COLUMN_PLANE"),_T("\t��� ��� �󼼵�"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("GIRDER_SECT"), bGirderSect ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("GIRDER_SECT"),_T("\t�Ŵ� �ܸ� �󼼵�"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("DETAIL"), bDetail ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("DETAIL"),_T("\t��� �󼼵�"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("DETAIL_CHAIR_BLOCK"), bDetailChairBlock ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("DETAIL_CHAIR_BLOCK"),_T("\tCHAIR BLOCK �󼼵�"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		if(pBri->IsBoxType())
		{
			m_Options.SetOption(_T("DETAIL_PRF"), bDetailPRF ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("DETAIL_PRF"),_T("\t�λ�������� �󼼵�"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		}
		
		m_Options.SetOption(_T("WING_BL"), bWingBL ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("WING_BL"),_T("\t���� ���� ������"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingBL->m_bExist);	
			m_Options.SetOption(_T("WING_BL_FRONT"), pData->m_bDrawRebarWingWall[0][0] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("WING_BL_FRONT"),_T("\t\t���鵵"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingBL->m_bExist);
			m_Options.SetOption(_T("WING_BL_VERT"), pData->m_bDrawRebarWingWall[0][1] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("WING_BL_VERT") ,_T("\t\t�ܸ鵵(����)"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingBL->m_bExist);
			m_Options.SetOption(_T("WING_BL_HORI"), pData->m_bDrawRebarWingWall[0][2] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("WING_BL_HORI") ,_T("\t\t�ܸ鵵(����)"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingBL->m_bExist);
			m_Options.SetOption(_T("WING_BL_ADD"), pData->m_bDrawRebarWingWall[0][3] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			if(pWingBL->m_bAddRebarReinforce)
			{
				m_Options.SetOptionItem(_T("WING_BL_ADD") ,_T("\t\t�亮����ö�ٻ�"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingBL->m_bExist);
			}

		m_Options.SetOption(_T("WING_BR"), bWingBR ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("WING_BR"),_T("\t���� ���� ������"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingBR->m_bExist);	
			m_Options.SetOption(_T("WING_BR_FRONT"), pData->m_bDrawRebarWingWall[1][0] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("WING_BR_FRONT"),_T("\t\t���鵵"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingBR->m_bExist);
			m_Options.SetOption(_T("WING_BR_VERT"), pData->m_bDrawRebarWingWall[1][1] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("WING_BR_VERT") ,_T("\t\t�ܸ鵵(����)"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingBR->m_bExist);
			m_Options.SetOption(_T("WING_BR_HORI"), pData->m_bDrawRebarWingWall[1][2] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("WING_BR_HORI") ,_T("\t\t�ܸ鵵(����)"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingBR->m_bExist);
			m_Options.SetOption(_T("WING_BR_ADD"), pData->m_bDrawRebarWingWall[1][3] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			if(pWingBR->m_bAddRebarReinforce)
			{
				m_Options.SetOptionItem(_T("WING_BR_ADD") ,_T("\t\t�亮����ö�ٻ�"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingBR->m_bExist);
			}

		m_Options.SetOption(_T("WING_EL"), bWingEL ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("WING_EL"),_T("\t���� ���� ������"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingEL->m_bExist);		
			m_Options.SetOption(_T("WING_EL_FRONT"), pData->m_bDrawRebarWingWall[2][0] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("WING_EL_FRONT"),_T("\t\t���鵵"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingEL->m_bExist);
			m_Options.SetOption(_T("WING_EL_VERT"), pData->m_bDrawRebarWingWall[2][1] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("WING_EL_VERT") ,_T("\t\t�ܸ鵵(����)"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingEL->m_bExist);
			m_Options.SetOption(_T("WING_EL_HORI"), pData->m_bDrawRebarWingWall[2][2] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("WING_EL_HORI") ,_T("\t\t�ܸ鵵(����)"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingEL->m_bExist);
			m_Options.SetOption(_T("WING_EL_ADD"), pData->m_bDrawRebarWingWall[2][3] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			if(pWingEL->m_bAddRebarReinforce)
			{
				m_Options.SetOptionItem(_T("WING_EL_ADD") ,_T("\t\t�亮����ö�ٻ�"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingEL->m_bExist);
			}
		
		m_Options.SetOption(_T("WING_ER"), bWingER ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("WING_ER"),_T("\t���� ���� ������"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingER->m_bExist);	
			m_Options.SetOption(_T("WING_ER_FRONT"), pData->m_bDrawRebarWingWall[3][0] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("WING_ER_FRONT"),_T("\t\t���鵵"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingER->m_bExist);
			m_Options.SetOption(_T("WING_ER_VERT"), pData->m_bDrawRebarWingWall[3][1] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("WING_ER_VERT") ,_T("\t\t�ܸ鵵(����)"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingER->m_bExist);
			m_Options.SetOption(_T("WING_ER_HORI"), pData->m_bDrawRebarWingWall[3][2] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("WING_ER_HORI") ,_T("\t\t�ܸ鵵(����)"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingER->m_bExist);
			m_Options.SetOption(_T("WING_ER_ADD"), pData->m_bDrawRebarWingWall[3][3] ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			if(pWingER->m_bAddRebarReinforce)
			{
				m_Options.SetOptionItem(_T("WING_ER_ADD") ,_T("\t\t�亮����ö�ٻ�"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0, pWingER->m_bExist);
			}
		
	m_Options.SetOptionItem("ö�����ǥ", IDB_ITEMS, IDB_ITEMS);
	m_Options.SetOption(_T("SEPARATE_BM_MAIN"), pData->m_bSeparateOutputRebarBMMain ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("SEPARATE_BM_MAIN"),_T("\t��ü ö�� ���ǥ �и����"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0);		
		m_Options.SetOption(_T("SEPARATE_BM_WING"), pData->m_bSeparateRebarBMWing ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("SEPARATE_BM_WING"),_T("\t������ ö�ټ��� �и����"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0);		
		m_Options.SetOption(_T("SEPARATE_BM_COLUMN"), pData->m_bSeparateRebarBMColumn ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("SEPARATE_BM_COLUMN"),_T("\t����� ö�ټ��� �и����"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0);
		m_Options.SetOption(_T("SEPARATE_BM_OTHER"), pData->m_bSeparateRebarBMOther ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("SEPARATE_BM_OTHER"),_T("\t��Ÿ ö�ټ��� �и����"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0);
		m_Options.SetOptionItem("\tö�����ǥ ǥ�����", IDB_ITEMS, IDB_ITEMS);
			m_Options.SetOption("TYPE_REBARTABLE", pData->m_nTypeOutRebarTable);
			m_Options.SetOptionItem("TYPE_REBARTABLE", "\t\t��ü�� ǥ��",0, IDB_RADIOON, IDB_RADIOOFF);
			m_Options.SetOptionItem("TYPE_REBARTABLE", "\t\t��ö��, ��Ÿö�� �����ؼ� ǥ��",1, IDB_RADIOON, IDB_RADIOOFF);				
		m_Options.SetOption(_T("ö�����ǥ �������� �и�ǥ��"), pData->m_bRebarTableSepByFy ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("ö�����ǥ �������� �и�ǥ��"),_T("\t\tö�����ǥ �������� �и�ǥ��"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF, 0);


		m_Options.SetOption(_T("SEP_FRONTBACK_WALLREBAR"), pData->m_bSepFrontAndBackWallRebar? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("SEP_FRONTBACK_WALLREBAR"),_T("\t��ü �������� ����, ������ �и��ؼ� ǥ��"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("SEP_UPPERLOWER_FOOTINGREBAR"), pData->m_bSepUpperAndLowerFootingRebar? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("SEP_UPPERLOWER_FOOTINGREBAR"),_T("\t���� �������� ���, �ϸ��� �и��ؼ� ǥ��"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("SEP_UPPERLOWER_SLABREBAR"), pData->m_bSepUpperAndLowerSlabRebar? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("SEP_UPPERLOWER_SLABREBAR"),_T("\t��ν����� �������� ���, �ϸ��� �и��ؼ� ǥ��"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("SEP_UPPERSLAB2DAN_ALL"), pData->m_bSepUpperSlab2DanAll? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("SEP_UPPERSLAB2DAN_ALL"),_T("\t��ν����� ������ 2��ö�� ��� ǥ��"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("UPPER_SLAB_HUNCH"), pData->m_bDrawHunchRebarOnly ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("UPPER_SLAB_HUNCH"),_T("\t������ ��ġ �󼼵� �и��ؼ� ǥ��"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

		m_Options.SetOptionItem("\t��ν����� ������ ��ö�� ���ü� ����", IDB_ITEMS, IDB_ITEMS);
			m_Options.SetOption("DIRMARK_UPPERSLAB_MAINREBAR", pData->m_nDirMarkUpperSlabMainRebar);
			m_Options.SetOptionItem("DIRMARK_UPPERSLAB_MAINREBAR", "\t\t�������� ����",0, IDB_RADIOON, IDB_RADIOOFF);
			m_Options.SetOptionItem("DIRMARK_UPPERSLAB_MAINREBAR", "\t\t�������� ����",1, IDB_RADIOON, IDB_RADIOOFF);				
			m_Options.SetOptionItem("DIRMARK_UPPERSLAB_MAINREBAR", "\t\t�ڵ�",2, IDB_RADIOON, IDB_RADIOOFF);				

	m_Options.BeginTrans();
	m_Tree.Initialize(NULL);
}

void CDrawingOptionRebar::SetDataSave()
{
	CARcBridgeDoc* pDoc					= dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd			= pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd* pOpt	= pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nCurBri);
	CRcBridgeRebar* pBri = pDoc->m_pARcBridgeDataStd->GetBridgeRebar(m_nCurBri);

	pData->m_dwDrawRebar = 0x000000;
	pData->m_bMainRebarNumberingInStandardSection = FALSE;
	pData->m_nTypeMainRebarAssm	= m_Options.GetOption("TYPE_MAIN_ASSM");
	pData->m_nBasePosPlaneSupportRebarDim   = m_Options.GetOption("TYPE_DIM_PLANE_SUPPORT");
	pData->m_nTypeOutRebarTable	= m_Options.GetOption("TYPE_REBARTABLE");
	pData->m_bRebarTableSepByFy	= m_Options.GetOption("ö�����ǥ �������� �и�ǥ��");
	pData->m_bSepFrontAndBackWallRebar	= m_Options.GetOption("SEP_FRONTBACK_WALLREBAR");
	pData->m_bSepUpperAndLowerFootingRebar	= m_Options.GetOption("SEP_UPPERLOWER_FOOTINGREBAR");
	pData->m_bSepUpperAndLowerSlabRebar	= m_Options.GetOption("SEP_UPPERLOWER_SLABREBAR");
	pData->m_bSepUpperSlab2DanAll	= m_Options.GetOption("SEP_UPPERSLAB2DAN_ALL");
	pData->m_nDirMarkUpperSlabMainRebar	= m_Options.GetOption("DIRMARK_UPPERSLAB_MAINREBAR");
	pData->m_bDrawHunchRebarOnly	= m_Options.GetOption("UPPER_SLAB_HUNCH");

	if(m_Options.GetOption(_T("STD_SECT")))	pData->m_dwDrawRebar |= REBAR_STD_SECT;
	if(m_Options.GetOption(_T("NUMMB_STD_SECT")))	pData->m_bMainRebarNumberingInStandardSection	= TRUE;
	if(m_Options.GetOption(_T("MAIN_ASSM")))	pData->m_dwDrawRebar |= REBAR_MAIN_ASSM;
	pData->m_bLengthInfoInMainRebarNumbering	= m_Options.GetOption("LENINFO_MAIN_ASSM");
	if(m_Options.GetOption(_T("BMD")))	pData->m_dwDrawRebar |= REBAR_BMD;
	if(m_Options.GetOption(_T("SFD")))	pData->m_dwDrawRebar |= REBAR_SFD;
	if(m_Options.GetOption(_T("AFD")))	pData->m_dwDrawRebar |= REBAR_AFD;
	if(m_Options.GetOption(_T("COUPLER")))	pData->m_dwDrawRebar |= REBAR_DETAIL_COUPLER;
	if(m_Options.GetOption(_T("UPPER_SLAB")))	pData->m_dwDrawRebar |= REBAR_UPPER_SLAB;
	if(pBri->IsBoxType())
	{
		
		if(m_Options.GetOption(_T("LOWER_SLAB")))	pData->m_dwDrawRebar |= REBAR_LOWER_SLAB;
	}
	if(m_Options.GetOption(_T("CROSSSECTION")))	pData->m_dwDrawRebar |= REBAR_CROSSSECTION;
	pData->m_bExcludeDoupleMainRebarCrossSection	= m_Options.GetOption(_T("EXCLUDE_DOUPLEREBAR_CROSSSECTION"));
	if(m_Options.GetOption(_T("BEGIN_WALL")))	pData->m_dwDrawRebar |= REBAR_BEGIN_WALL;
	if(m_Options.GetOption(_T("END_WALL")))	pData->m_dwDrawRebar |= REBAR_END_WALL;
	if(m_Options.GetOption(_T("INWALL_FRONT")))	pData->m_dwDrawRebar |= REBAR_INWALL_FRONT;
	if(m_Options.GetOption(_T("INWALL_SECT")))	pData->m_dwDrawRebar |= REBAR_INWALL_SECT;
	if(m_Options.GetOption(_T("FOOTING_PLANE")))	pData->m_dwDrawRebar |= REBAR_FOOTING_PLANE;
	if(m_Options.GetOption(_T("COLUMN_PLANE")))	pData->m_dwDrawRebar |= REBAR_COLUMN_PLANE;
	if(m_Options.GetOption(_T("GIRDER_SECT")))	pData->m_dwDrawRebar |= REBAR_GIRDER_SECT;
	if(m_Options.GetOption(_T("DETAIL")))	pData->m_dwDrawRebar |= REBAR_DETAIL;
	if(m_Options.GetOption(_T("DETAIL_CHAIR_BLOCK")))	pData->m_dwDrawRebar |= REBAR_DETAIL_CHAIR_BLOCK;
	if(pBri->IsBoxType())
	{
		if(m_Options.GetOption(_T("DETAIL_PRF")))	pData->m_dwDrawRebar |= REBAR_DETAIL_PRF;
	}

	if(m_Options.GetOption(_T("WING_BL")))	pData->m_dwDrawRebar |= REBAR_WING_BL;
	pData->m_bDrawRebarWingWall[0][0] = m_Options.GetOption(_T("WING_BL_FRONT"));
	pData->m_bDrawRebarWingWall[0][1] = m_Options.GetOption(_T("WING_BL_VERT"));
	pData->m_bDrawRebarWingWall[0][2] = m_Options.GetOption(_T("WING_BL_HORI"));
	pData->m_bDrawRebarWingWall[0][3] = m_Options.GetOption(_T("WING_BL_ADD"));
	if(m_Options.GetOption(_T("WING_ER")))	pData->m_dwDrawRebar |= REBAR_WING_ER;
	pData->m_bDrawRebarWingWall[3][0] = m_Options.GetOption(_T("WING_ER_FRONT"));
	pData->m_bDrawRebarWingWall[3][1] = m_Options.GetOption(_T("WING_ER_VERT"));
	pData->m_bDrawRebarWingWall[3][2] = m_Options.GetOption(_T("WING_ER_HORI"));
	pData->m_bDrawRebarWingWall[3][3] = m_Options.GetOption(_T("WING_ER_ADD"));
	if(m_Options.GetOption(_T("WING_BR")))	pData->m_dwDrawRebar |= REBAR_WING_BR;
	pData->m_bDrawRebarWingWall[1][0] = m_Options.GetOption(_T("WING_BR_FRONT"));
	pData->m_bDrawRebarWingWall[1][1] = m_Options.GetOption(_T("WING_BR_VERT"));
	pData->m_bDrawRebarWingWall[1][2] = m_Options.GetOption(_T("WING_BR_HORI"));
	pData->m_bDrawRebarWingWall[1][3] = m_Options.GetOption(_T("WING_BR_ADD"));
	if(m_Options.GetOption(_T("WING_EL")))	pData->m_dwDrawRebar |= REBAR_WING_EL;
	pData->m_bDrawRebarWingWall[2][0] = m_Options.GetOption(_T("WING_EL_FRONT"));
	pData->m_bDrawRebarWingWall[2][1] = m_Options.GetOption(_T("WING_EL_VERT"));
	pData->m_bDrawRebarWingWall[2][2] = m_Options.GetOption(_T("WING_EL_HORI"));
	pData->m_bDrawRebarWingWall[2][3] = m_Options.GetOption(_T("WING_EL_ADD"));
	
	pData->m_bSeparateOutputRebarBMMain	= m_Options.GetOption(_T("SEPARATE_BM_MAIN"));
	pData->m_bSeparateRebarBMWing		= m_Options.GetOption(_T("SEPARATE_BM_WING"));
	pData->m_bSeparateRebarBMColumn		= m_Options.GetOption(_T("SEPARATE_BM_COLUMN"));
	pData->m_bSeparateRebarBMOther		= m_Options.GetOption(_T("SEPARATE_BM_OTHER"));
	

	int nRow = 1;
	pData->m_dScaleRebarBase			= m_Grid.GetTextMatrixDouble(nRow++, 1);
	pData->m_dScaleRebarWingWall		= m_Grid.GetTextMatrixDouble(nRow++, 1);
	pData->m_dScaleBMD					= m_Grid.GetTextMatrixDouble(nRow++, 1);
	pData->m_dScaleSFD					= m_Grid.GetTextMatrixDouble(nRow++, 1);
	pData->m_dScaleAFD				= m_Grid.GetTextMatrixDouble(nRow++, 1);

	pData->m_dScaleRebarDetailArchRibSpringging			= m_Grid.GetTextMatrixDouble(nRow++, 1);
	pData->m_dScaleRebarDetailColumnSection			= m_Grid.GetTextMatrixDouble(nRow++, 1);
	pData->m_dScaleRebarDetailGirderSection			= m_Grid.GetTextMatrixDouble(nRow++, 1);
	pData->m_dScaleRebarDetailSlabAcute				= m_Grid.GetTextMatrixDouble(nRow++, 1);
	pData->m_dScaleRebarDetailWallObtuse			= m_Grid.GetTextMatrixDouble(nRow++, 1);
	pData->m_dScaleRebarDetailFootingAcute			= m_Grid.GetTextMatrixDouble(nRow++, 1);
	if(pBri->IsBoxType())
		pData->m_dScaleRebarDetailPRF				= m_Grid.GetTextMatrixDouble(nRow++, 1);

	pData->m_dSpaceBetweenRebarDetail	= m_Grid.GetTextMatrixDouble(nRow++, 1);
	HGINT32 icRowEachTable  = m_Grid.GetTextMatrixLong(nRow++, 1);
	if (icRowEachTable > 0)
	{
		pData->m_nCountRowEachTable			= icRowEachTable;
	}
}

void CDrawingOptionRebar::OnOK() 
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;

	SetDataSave();

	pStd->SetModifiedAll();
	m_pDoc->SetModifiedFlag();

	AfxGetMainWnd()->SendMessage(WM_USER+8, -1);
	AfxGetMainWnd()->SendMessage(WM_USER+13);
}

void CDrawingOptionRebar::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;

	int nRow = pGridView->iRow;

	CString str = m_Grid.GetTextMatrix(nRow, 0);
	if (str == "���̺�� ����Ÿ��")
	{
		HGINT32 icRowEachTable		= m_Grid.GetTextMatrixLong(nRow++, 1);
		if (icRowEachTable <= 0)
		{
			InitGrid();
		}
	}
}