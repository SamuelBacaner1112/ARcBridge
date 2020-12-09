// DrawingOptionSlabStandardOutput.cpp: implementation of the CDrawingOptionSlabStandardOutput class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../ARcBridge/ARcBridgeDoc.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"
#include "../ARcBridgeDrawing/ARcBridgeDrawingStd.h"
#include "arcbridgedrawingoption.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CDrawingOptionSlabStandardOutput, COptionBase)

CDrawingOptionSlabStandardOutput::CDrawingOptionSlabStandardOutput() : COptionBase(CDrawingOptionSlabStandardOutput::IDD)
{
	//{{AFX_DATA_INIT(CDrawingOptionSlabStandardOutput)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = NULL;
	m_nCurBri = 0;
}

CDrawingOptionSlabStandardOutput::~CDrawingOptionSlabStandardOutput()
{
}

void CDrawingOptionSlabStandardOutput::DoDataExchange(CDataExchange* pDX)
{
	COptionBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawingOptionSlabStandardOutput)
	DDX_Control(pDX, IDC_OPT_TREE, m_Tree);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_OPT_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CDrawingOptionSlabStandardOutput, COptionBase)
	//{{AFX_MSG_MAP(CDrawingOptionSlabStandardOutput)
	ON_BN_CLICKED(IDC_OPT_DEFAULT, OnOptionDefault)
	ON_CBN_SELCHANGE(IDC_OPT_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_OPT_GRID, OnCellChangedData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawingOptionSlabStandardOutput message handlers

BOOL CDrawingOptionSlabStandardOutput::OnInitDialog() 
{
	COptionBase::OnInitDialog();

	m_nCurBri = (dynamic_cast<CARcBridgeDoc*>(m_pDoc))->m_nCurBriNoOfSettingDomyun;

	InitTree();

	long nRowCount = 6;
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

BOOL CDrawingOptionSlabStandardOutput::OnSetActive() 
{
	if(((COptionSheet*)GetParent())->m_cbBriNo.GetSafeHwnd()!=NULL)
		((COptionSheet*)GetParent())->m_cbBriNo.ShowWindow(TRUE);

	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd *pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	if(((COptionSheet*)GetParent())->m_bSetOK)
		pOpt->m_nCurPropPage = 1;
	
	return COptionBase::OnSetActive();
}

void CDrawingOptionSlabStandardOutput::OnSelchangeComboBridgeNo() 
{
	SetDataSave();

	m_nCurBri = ((COptionSheet*)GetParent())->m_cbBriNo.GetCurSel();


	m_Tree.DeleteAllItems();
	InitTree();
	InitGrid();
}

void CDrawingOptionSlabStandardOutput::OnOptionDefault()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nCurBri);

	pData->DefaultDrawingOptionVerticalPlane();

	m_Tree.DeleteAllItems();
	InitTree();
	InitGrid();
}

void CDrawingOptionSlabStandardOutput::InitGrid()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nCurBri);

	long nRow	= 0;
	m_Grid.SetTextMatrix(nRow++, 0, _T("설정 항목"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("표준설계요령(1) 기본스케일"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("표준설계요령(2) 기본스케일"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("표준설계요령(3) 기본스케일"));
	m_Grid.SetTextMatrix(nRow++, 0, _T("철근제원간격"));
	m_Grid.SetTextMatrix(nRow, 0, _T("테이블당 데이타수"));
	
	nRow	= 1;
	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_dScaleRcStd1Base, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_dScaleRcStd2Base, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_dScaleRcStd3Base, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_dSpaceBetweenRebarDetail, (UINT)DT_RIGHT);
	m_Grid.SetTextMatrix(nRow++, 1, _T("%.f"), pData->m_nCountRowEachTable, (UINT)DT_RIGHT);

	m_Grid.SetRedraw(TRUE, TRUE);
}

void CDrawingOptionSlabStandardOutput::InitTree()
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

	BOOL bRcStd1_DesignCondition	= (pData->m_dwDrawRcStd & RCSTD1_DESIGN_CONDITION) ? TRUE : FALSE;
	BOOL bRcStd1_Standard_Outline	= (pData->m_dwDrawRcStd & RCSTD1_STANDARD_OUTLINE) ? TRUE : FALSE;
	BOOL bRcStd1_Cross				= (pData->m_dwDrawRcStd & RCSTD1_CROSS) ? TRUE : FALSE;
	BOOL bRcStd1_Plan				= (pData->m_dwDrawRcStd & RCSTD1_PLAN) ? TRUE : FALSE;
	BOOL bRcStd1_Note				= (pData->m_dwDrawRcStd & RCSTD1_NOTE) ? TRUE : FALSE;
	BOOL bRcStd2_Standard			= (pData->m_dwDrawRcStd & RCSTD2_STANDARD) ? TRUE : FALSE;
	BOOL bRcStd2_Plan				= (pData->m_dwDrawRcStd & RCSTD2_PLAN) ? TRUE : FALSE;
	BOOL bRcStd2_BMD				= (pData->m_dwDrawRcStd & RCSTD2_BMD) ? TRUE : FALSE;
	BOOL bRcStd2_SFD				= (pData->m_dwDrawRcStd & RCSTD2_SFD) ? TRUE : FALSE;
	BOOL bRcStd2_MainAssm1			= (pData->m_dwDrawRcStd & RCSTD2_MAINASSM_1) ? TRUE : FALSE;
	BOOL bRcStd2_MainAssm2			= (pData->m_dwDrawRcStd & RCSTD2_MAINASSM_2) ? TRUE : FALSE;
	BOOL bRcStd2_ObtusDetail		= (pData->m_dwDrawRcStd & RCSTD2_OBTUS_DETAIL) ? TRUE : FALSE;
	BOOL bRcStd3_GirderDetail		= (pData->m_dwDrawRcStd & RCSTD3_GIRDER_DETAIL) ? TRUE : FALSE;
	BOOL bRcStd3_GuardWallDetail	= (pData->m_dwDrawRcStd & RCSTD3_GUARDWALL_DETAIL) ? TRUE : FALSE;
	BOOL bRcStd3_Note				= (pData->m_dwDrawRcStd & RCSTD3_NOTE) ? TRUE : FALSE;
	BOOL bRcStd3_Spacer				= (pData->m_dwDrawRcStd & RCSTD3_SPACER) ? TRUE : FALSE;
	BOOL bRcStd3_DesignMethod		= (pData->m_dwDrawRcStd & RCSTD3_DESIGN_METHOD) ? TRUE : FALSE;

	m_Options.SetOptionItem(_T("도면 출력"), IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOptionItem(_T("\tRC슬래브교 표준 설계요령(1)"), IDB_ITEMS, IDB_ITEMS);
			m_Options.SetOption(_T("RCSTD1_DESIGN_CONDITION"), bRcStd1_DesignCondition ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("RCSTD1_DESIGN_CONDITION"),_T("\t\t설계조건"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption(_T("RCSTD1_STANDARD_OUTLINE"), bRcStd1_Standard_Outline ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("RCSTD1_STANDARD_OUTLINE"),_T("\t\t표준단면도 외곽"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption(_T("RCSTD1_CROSS"), bRcStd1_Cross ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("RCSTD1_CROSS"),_T("\t\t횡단면도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption(_T("RCSTD1_PLAN"), bRcStd1_Plan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("RCSTD1_PLAN"),_T("\t\t평면도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption(_T("RCSTD1_NOTE"), bRcStd1_Note ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("RCSTD1_NOTE"),_T("\t\t노트"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			
		m_Options.SetOptionItem(_T("\tRC슬래브교 표준 설계요령(2)"), IDB_ITEMS, IDB_ITEMS);
			m_Options.SetOption(_T("RCSTD2_STANDARD"), bRcStd2_Standard ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("RCSTD2_STANDARD"),_T("\t\t표준단면도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption(_T("RCSTD2_PLAN"), bRcStd2_Plan ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("RCSTD2_PLAN"),_T("\t\t평면도"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption(_T("RCSTD2_BMD"), bRcStd2_BMD ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("RCSTD2_BMD"),_T("\t\tBMD"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption(_T("RCSTD2_SFD"), bRcStd2_SFD ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("RCSTD2_SFD"),_T("\t\tSFD"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption(_T("RCSTD2_MAINASSM_1"), bRcStd2_MainAssm1 ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("RCSTD2_MAINASSM_1"),_T("\t\t주철근 조립도①"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption(_T("RCSTD2_MAINASSM_2"), bRcStd2_MainAssm2 ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("RCSTD2_MAINASSM_2"),_T("\t\t주철근 조립도②"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption(_T("RCSTD2_OBTUS_DETAIL"), bRcStd2_ObtusDetail ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("RCSTD2_OBTUS_DETAIL"),_T("\t\t둔각부 보강상세"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			
		m_Options.SetOptionItem(_T("\tRC슬래브교 표준 설계요령(3)"), IDB_ITEMS, IDB_ITEMS);
			m_Options.SetOption(_T("RCSTD3_GIRDER_DETAIL"), bRcStd3_GirderDetail ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("RCSTD3_GIRDER_DETAIL"),_T("\t\t거더상세"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption(_T("RCSTD3_GUARDWALL_DETAIL"), bRcStd3_GuardWallDetail ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("RCSTD3_GUARDWALL_DETAIL"),_T("\t\t방호벽상세"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption(_T("RCSTD3_NOTE"), bRcStd3_Note ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("RCSTD3_NOTE"),_T("\t\t노트"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption(_T("RCSTD3_SPACER"), bRcStd3_Spacer ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("RCSTD3_SPACER"),_T("\t\t간격 유지재"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption(_T("RCSTD3_DESIGN_METHOD"), bRcStd3_DesignMethod ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("RCSTD3_DESIGN_METHOD"),_T("\t\t설계요령"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

	m_Options.BeginTrans();
	m_Tree.Initialize(NULL);
}

void CDrawingOptionSlabStandardOutput::SetDataSave()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nCurBri);

	pData->m_dwDrawRcStd = 0x000000;
	if(m_Options.GetOption(_T("RCSTD1_DESIGN_CONDITION")))	pData->m_dwDrawRcStd |= RCSTD1_DESIGN_CONDITION;
	if(m_Options.GetOption(_T("RCSTD1_STANDARD_OUTLINE")))	pData->m_dwDrawRcStd |= RCSTD1_STANDARD_OUTLINE;
	if(m_Options.GetOption(_T("RCSTD1_CROSS")))				pData->m_dwDrawRcStd |= RCSTD1_CROSS;
	if(m_Options.GetOption(_T("RCSTD1_PLAN")))				pData->m_dwDrawRcStd |= RCSTD1_PLAN;
	if(m_Options.GetOption(_T("RCSTD1_NOTE")))				pData->m_dwDrawRcStd |= RCSTD1_NOTE;
	if(m_Options.GetOption(_T("RCSTD2_STANDARD")))			pData->m_dwDrawRcStd |= RCSTD2_STANDARD;
	if(m_Options.GetOption(_T("RCSTD2_PLAN")))				pData->m_dwDrawRcStd |= RCSTD2_PLAN;
	if(m_Options.GetOption(_T("RCSTD2_BMD")))				pData->m_dwDrawRcStd |= RCSTD2_BMD;
	if(m_Options.GetOption(_T("RCSTD2_SFD")))				pData->m_dwDrawRcStd |= RCSTD2_SFD;
	if(m_Options.GetOption(_T("RCSTD2_MAINASSM_1")))		pData->m_dwDrawRcStd |= RCSTD2_MAINASSM_1;
	if(m_Options.GetOption(_T("RCSTD2_MAINASSM_2")))		pData->m_dwDrawRcStd |= RCSTD2_MAINASSM_2;
	if(m_Options.GetOption(_T("RCSTD2_OBTUS_DETAIL")))		pData->m_dwDrawRcStd |= RCSTD2_OBTUS_DETAIL;
	if(m_Options.GetOption(_T("RCSTD3_GIRDER_DETAIL")))		pData->m_dwDrawRcStd |= RCSTD3_GIRDER_DETAIL;
	if(m_Options.GetOption(_T("RCSTD3_GUARDWALL_DETAIL")))	pData->m_dwDrawRcStd |= RCSTD3_GUARDWALL_DETAIL;
	if(m_Options.GetOption(_T("RCSTD3_NOTE")))				pData->m_dwDrawRcStd |= RCSTD3_NOTE;
	if(m_Options.GetOption(_T("RCSTD3_SPACER")))			pData->m_dwDrawRcStd |= RCSTD3_SPACER;
	if(m_Options.GetOption(_T("RCSTD3_DESIGN_METHOD")))		pData->m_dwDrawRcStd |= RCSTD3_DESIGN_METHOD;



	int nRow = 1;
	pData->m_dScaleRcStd1Base	= m_Grid.GetTextMatrixDouble(nRow++, 1);	// 도면1 기본 스케일
	pData->m_dScaleRcStd2Base	= m_Grid.GetTextMatrixDouble(nRow++, 1);	// 도면2 기본 스케일
	pData->m_dScaleRcStd3Base	= m_Grid.GetTextMatrixDouble(nRow++, 1);	// 도면3 기본 스케일
	pData->m_dSpaceBetweenRebarDetail	= m_Grid.GetTextMatrixDouble(nRow++, 1);
	
}

void CDrawingOptionSlabStandardOutput::OnOK() 
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;

	SetDataSave();

	pStd->SetModifiedAll();
	m_pDoc->SetModifiedFlag();
}

void CDrawingOptionSlabStandardOutput::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;

	int nRow = pGridView->iRow;

	CString str = m_Grid.GetTextMatrix(nRow, 0);
	if (str == "테이블당 데이타수")
	{
		HGINT32 icRowEachTable		= m_Grid.GetTextMatrixLong(nRow++, 1);
		if (icRowEachTable <= 0)
		{
			InitGrid();
		}
	}
}