// DrawingOptionCommon.cpp : implementation file
//

#include "stdafx.h"
#include "../../ARcBridge/ARcBridgeDoc.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"
#include "../../Src/ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "../ARcBridgeDrawing/ARcBridgeDrawingStd.h"
#include "arcbridgedrawingoption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawingOptionCommon property page

IMPLEMENT_DYNCREATE(CDrawingOptionCommon, COptionBase)

CDrawingOptionCommon::CDrawingOptionCommon() : COptionBase(CDrawingOptionCommon::IDD)
{
	//{{AFX_DATA_INIT(CDrawingOptionCommon)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = NULL;
}

CDrawingOptionCommon::~CDrawingOptionCommon()
{
}

void CDrawingOptionCommon::DoDataExchange(CDataExchange* pDX)
{
	COptionBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawingOptionCommon)
	DDX_Control(pDX, IDC_OPT_TREE, m_Tree);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_OPT_GRID1, m_Grid1);
	DDX_GridControl(pDX, IDC_OPT_GRID2, m_Grid2);
}


BEGIN_MESSAGE_MAP(CDrawingOptionCommon, COptionBase)
	//{{AFX_MSG_MAP(CDrawingOptionCommon)
	ON_BN_CLICKED(IDC_OPT_DEFAULT, OnOptionDefault)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_OPT_GRID1, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawingOptionCommon message handlers

BOOL CDrawingOptionCommon::OnInitDialog() 
{
	COptionBase::OnInitDialog();

	m_nCurBri = (dynamic_cast<CARcBridgeDoc*>(m_pDoc))->m_nCurBriNoOfSettingDomyun;
	InitTree();

	// 그리드 1
	long nRowCount = 6;
	CRect Rect;
	m_Grid1.GetClientRect(&Rect);
	m_Grid1.EnableReturnPass(TRUE);
	m_Grid1.SetFixedRowCount(1);
	m_Grid1.SetFixedColumnCount(1);
	m_Grid1.SetRowCount(nRowCount);
	m_Grid1.SetColumnCount(2);
	m_Grid1.SetRowHeightAll(20);
	m_Grid1.SetColumnWidth(0, Rect.Width()/2-8);
	m_Grid1.SetColumnWidth(1, Rect.Width()/2+8);
	m_Grid1.EnableInsertRow(FALSE);
	m_Grid1.EnableInsertCol(FALSE);
	m_Grid1.SetTextBkColor(CLR_BACK);
	InitGrid1();

	// 그리드 2
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;
	CDrawDogak* pDgk = pStd->GetDogak();

	if(pDgk->m_bHCadDgk)
	{		
		long nRow = pDgk->m_szAttdefPrompt.GetSize() + 1;
		CRect Rect;
		m_Grid2.GetClientRect(&Rect);
		m_Grid2.EnableReturnPass(TRUE);
		m_Grid2.SetFixedRowCount(1);
		m_Grid2.SetFixedColumnCount(1);
		m_Grid2.SetRowCount(nRow);
		m_Grid2.SetColumnCount(2);
		m_Grid2.SetRowHeightAll(20);
		m_Grid2.SetColumnWidthAll(Rect.Width()/2-8);
		m_Grid2.EnableInsertRow(FALSE);
		m_Grid2.EnableInsertCol(FALSE);
		m_Grid2.SetTextBkColor(CLR_BACK);
	}
	InitGrid2();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDrawingOptionCommon::OnSetActive() 
{
	if(((COptionSheet*)GetParent())->m_cbBriNo.GetSafeHwnd()!=NULL)
		((COptionSheet*)GetParent())->m_cbBriNo.ShowWindow(FALSE);

	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd *pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	if(((COptionSheet*)GetParent())->m_bSetOK)
		pOpt->m_nCurPropPage = 0;
	
	return COptionBase::OnSetActive();
}

void CDrawingOptionCommon::OnOptionDefault()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CDrawDogak* pDgk = pStd->GetDogak();

	pOpt->DefaultDrawingOptionCommon();
	pDgk->Initialize(TRUE, pOpt->m_strDgkName, pOpt->m_strDomNumHead);

	m_Tree.DeleteAllItems();
	InitTree();
	InitGrid1();
	InitGrid2();
}

void CDrawingOptionCommon::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW *pGridView = (NM_GRIDVIEW *)pNotifyStruct;	
	
	int nRow = pGridView->iRow;
    int nCol = pGridView->iColumn;	

	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	if(nRow==1)
	{
		pOpt->m_strDgkName = m_Grid1.GetTextMatrix(nRow, nCol);
		pStd->GetDogak()->Initialize(TRUE, pOpt->m_strDgkName, pOpt->m_strDomNumHead);
		CDrawDogak* pDgk = pStd->GetDogak();
		if(pDgk->m_bHCadDgk)
		{
			pOpt->m_strDgkAttrPrompt.RemoveAll();
			pOpt->m_strDgkAttrText.RemoveAll();

			long i=0; for(i=0; i<pDgk->m_szAttdefPrompt.GetSize(); i++)
			{
				pOpt->m_strDgkAttrPrompt.Add(pDgk->m_szAttdefPrompt[i]);
				pOpt->m_strDgkAttrText.Add(pDgk->m_szAttdefText[i]);
			}
		}

		InitGrid2();
	}
}

void CDrawingOptionCommon::InitGrid1()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	m_Grid1.SetTextMatrix(0, 0, _T("설정 항목"));
	m_Grid1.SetTextMatrix(1, 0, _T("도각 파일"));
	m_Grid1.SetTextMatrix(2, 0, _T("외부 참조 도각 파일"));
	m_Grid1.SetTextMatrix(3, 0, _T("도면 번호 첫 머리 문자"));
	m_Grid1.SetTextMatrix(4, 0, _T("상세박스 폭"));
	m_Grid1.SetTextMatrix(5, 0, _T("상세박스 높이"));

	CString str = _T("");
	m_Grid1.SetTextMatrix(0, 1, _T("설정 값"));
	m_Grid1.SetTextMatrix(1, 1, pOpt->m_strDgkName, (UINT)DT_LEFT);
	m_Grid1.SetCellType(1, 1, CT_FILEBUTTON);
	m_Grid1.SetInitFileDlgText(_T("도각 파일 (.dgk)|*.dgk|XRef DWG 파일 (.dwg)|*.dwg||"));
	m_Grid1.SetTextMatrix(2, 1, pOpt->m_strXRefPath, (UINT)DT_LEFT);
	m_Grid1.SetCellType(2, 1, CT_FILEBUTTON);
	m_Grid1.SetTextMatrix(3, 1, pOpt->m_strDomNumHead, (UINT)DT_LEFT);
	str.Format(_T("%.1f"), pOpt->m_dWidthRebarTableRect);
	m_Grid1.SetTextMatrix(4, 1, str);
	str.Format(_T("%.1f"), pOpt->m_dHeightRebarTableRect);
	m_Grid1.SetTextMatrix(5, 1, str);

	CDrawDogak *pDgk = pStd->GetDogak();
	if(pDgk->m_bHCadDgk)
	{
		pOpt->m_strDgkAttrPrompt.RemoveAll();
		pOpt->m_strDgkAttrText.RemoveAll();

		long i=0; for(i=0; i<pDgk->m_szAttdefPrompt.GetSize(); i++)
		{
			pOpt->m_strDgkAttrPrompt.Add(pDgk->m_szAttdefPrompt[i]);
			pOpt->m_strDgkAttrText.Add(pDgk->m_szAttdefText[i]);
		}				
	}

	m_Grid1.SetRedraw(TRUE, TRUE);
}

void CDrawingOptionCommon::InitGrid2()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CDrawDogak* pDgk = pStd->GetDogak();

	if(pDgk->m_bHCadDgk)
	{
		m_Grid2.SetTextMatrix(0, 0, _T("설정 항목"));
		m_Grid2.SetTextMatrix(0, 1, _T("설정 값"));
		if(pOpt->m_strDgkAttrPrompt.GetSize()==0)
		{
			long i=0; for(i=0; i<pDgk->m_szAttdefPrompt.GetSize(); i++)
			{
				pOpt->m_strDgkAttrPrompt.Add(pDgk->m_szAttdefPrompt[i]);
				pOpt->m_strDgkAttrText.Add(pDgk->m_szAttdefText[i]);
			}
		}
		long i=0; for(i=0; i<pOpt->m_strDgkAttrPrompt.GetSize(); i++)
		{
			m_Grid2.SetTextMatrix(i+1, 0, pOpt->m_strDgkAttrPrompt.GetAt(i));
			m_Grid2.SetTextMatrix(i+1, 1, pOpt->m_strDgkAttrText.GetAt(i));
		}		
	}
	else
	{
		m_Grid2.DeleteAllItems();
	}

	m_Grid2.SetRedraw(TRUE, TRUE);
}

void CDrawingOptionCommon::InitGrid()
{
	InitGrid1();
	InitGrid2();
}

void CDrawingOptionCommon::InitTree()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	m_Tree.SetBkColor(CLR_BACK);
	m_Tree.m_pOptions = &m_Options;

	// 기존의 있는 리스트 삭제
	for(POSITION pos=m_Tree.m_pOptions->m_Items.GetHeadPosition(); pos!=NULL;)
		delete m_Tree.m_pOptions->m_Items.GetNext(pos);
	m_Tree.m_pOptions->m_Items.RemoveAll();

	BOOL bTableShade = (pOpt->m_dwDrawCommon & COMMON_TABLESHADE) ? TRUE : FALSE;
	BOOL bTitleBox = (pOpt->m_dwDrawCommon & COMMON_TITLEBOX) ? TRUE : FALSE;
	BOOL bNoHCad = (pOpt->m_dwDrawCommon & COMMON_NOHCAD) ? TRUE : FALSE;
	BOOL bDisplayAngLenStd	= (pOpt->m_dwDrawCommon & COMMON_DISPLAY_ANG_LEN_BASE) ? TRUE : FALSE;

	m_Options.SetOptionItem(_T("도면 출력"), IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption(_T("SHADETABLE"), bTableShade ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("SHADETABLE"),_T("\t테이블 테두리 입체 표현"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("TITLEBOX"), bTitleBox ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("TITLEBOX"),_T("\t중제목 박스 입체 표현"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("NOHCAD"), bNoHCad ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("NOHCAD"),_T("\tHCAD Block 사용 안함"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOption(_T("DISPLAY_ANG_LEN_BASE"), bDisplayAngLenStd ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("DISPLAY_ANG_LEN_BASE"),_T("\t사거리 표기 기준 표시"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	

	m_Options.SetOptionItem("Elevation 표기 방법", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("TYPE_EL", pOpt->m_nTypeEL);
		m_Options.SetOptionItem("TYPE_EL", "\tEL. 15.000",0, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("TYPE_EL", "\tEL.:15.000",1, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("TYPE_EL", "\tEL.=15.000",2, IDB_RADIOON, IDB_RADIOOFF);

	m_Options.SetOptionItem("콘크리트강도 표기 방법", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("콘크리트강도표기방법", pOpt->m_nSelectFckDomText);
		m_Options.SetOptionItem("콘크리트강도표기방법", "\t콘크리트 강도",0, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("콘크리트강도표기방법", "\t굵은 골재 최대치수",1, IDB_RADIOON, IDB_RADIOOFF);

	m_Options.SetOptionItem("대제목 표기 방법", IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOptionItem("\t일체형 교량", IDB_ITEMS, IDB_ITEMS);
			m_Options.SetOption("TYPE_TITLE_BRIDGE", pOpt->m_nTypeTitleBridge);
			m_Options.SetOptionItem("TYPE_TITLE_BRIDGE", "\t\t도면명", 0, IDB_RADIOON, IDB_RADIOOFF);
			m_Options.SetOptionItem("TYPE_TITLE_BRIDGE", "\t\t교량명+도면명", 1, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("\t상하행 분리형 교량", IDB_ITEMS, IDB_ITEMS);
			m_Options.SetOption("TYPE_TITLE_SEP_BRIDGE", pOpt->m_nTypeTitleSeparatorBridge);
			m_Options.SetOptionItem("TYPE_TITLE_SEP_BRIDGE", "\t\t도면명", 0, IDB_RADIOON, IDB_RADIOOFF);
			m_Options.SetOptionItem("TYPE_TITLE_SEP_BRIDGE", "\t\t교량명+도면명", 1, IDB_RADIOON, IDB_RADIOOFF);
			m_Options.SetOptionItem("TYPE_TITLE_SEP_BRIDGE", "\t\t교량명+선형/반대방향명+도면명", 2, IDB_RADIOON, IDB_RADIOOFF);

	m_Options.SetOptionItem(hggettext("철근재료표 출력옵션"), IDB_ITEMS, IDB_ITEMS);
		m_Options.SetOption("REBAR_TABLE", pOpt->m_nTypeRebarTable);
		m_Options.SetOptionItem("REBAR_TABLE", "\t기본타입", 0, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem("REBAR_TABLE", "\t도면간소화 타입", 1, IDB_RADIOON, IDB_RADIOOFF);

		// (AAB-7090) 미가공 철근재료표 추가
		m_Options.SetOption(_T("ADD_TABLE_RAW_REBAR"), pOpt->m_bAddRawRebarTable ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
		m_Options.SetOptionItem(_T("ADD_TABLE_RAW_REBAR"),hggettext("\t미가공 철근재료표 추가"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);
		
	m_Options.BeginTrans();
	m_Tree.Initialize(NULL);
}

void CDrawingOptionCommon::OnOK() 
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	long nTableTypePrev = pOpt->m_nTypeRebarTable;

	pOpt->m_dwDrawCommon = 0x000000;
	if(m_Options.GetOption(_T("SHADETABLE"))) pOpt->m_dwDrawCommon |= COMMON_TABLESHADE;	
	if(m_Options.GetOption(_T("TITLEBOX"))) pOpt->m_dwDrawCommon |= COMMON_TITLEBOX;		
	if(m_Options.GetOption(_T("NOHCAD"))) pOpt->m_dwDrawCommon |= COMMON_NOHCAD;	
	if(m_Options.GetOption(_T("DISPLAY_ANG_LEN_BASE"))) pOpt->m_dwDrawCommon |= COMMON_DISPLAY_ANG_LEN_BASE;

	pOpt->m_nTypeEL	= m_Options.GetOption("TYPE_EL");
	pOpt->m_nSelectFckDomText	= m_Options.GetOption("콘크리트강도표기방법");
	pOpt->m_nTypeTitleSeparatorBridge	= m_Options.GetOption("TYPE_TITLE_SEP_BRIDGE");	// 분리형 교량 대제목 타입(0:도면명, 1:교량명+도면명, 2:교량명+선형/반대방향명+도면명)
	pOpt->m_nTypeTitleBridge			= m_Options.GetOption("TYPE_TITLE_BRIDGE");			// 일체형 교량 대제목 타입(0:도면명, 1:교량명+도면명)
	pOpt->m_nTypeRebarTable = m_Options.GetOption("REBAR_TABLE"); //도면간소화타입
	if((nTableTypePrev == 0 && pOpt->m_nTypeRebarTable == 1) || (nTableTypePrev == 1 && pOpt->m_nTypeRebarTable == 0))
		AfxMessageBox(_T("도면간소화 타입은 철근의 마킹과 철근재료표를 지원하며 설정은 각각 적용하여야 합니다.\n철근재료표 설정은 도면환경설정 - 공통사항의 [철근재료표 출력옵션]에서 설정되며,\nMarking설정은 도면형식 - 치수스타일의 [Rebar Marking]에서 설정됩니다."));
	
	pOpt->m_bAddRawRebarTable	= m_Options.GetOption(_T("ADD_TABLE_RAW_REBAR")) == CCustomOptions::CheckTrue ? TRUE : FALSE;
	
	int nRow = 1;
	pOpt->m_strDgkName = m_Grid1.GetTextMatrix(nRow++, 1);
	pOpt->m_strXRefPath = m_Grid1.GetTextMatrix(nRow++, 1);
	pOpt->m_strDomNumHead = m_Grid1.GetTextMatrix(nRow++, 1);
	pOpt->m_dWidthRebarTableRect = hgatof(m_Grid1.GetTextMatrix(nRow++, 1));
	pOpt->m_dHeightRebarTableRect = hgatof(m_Grid1.GetTextMatrix(nRow++, 1));

	pStd->GetDogak()->Initialize(TRUE, pOpt->m_strDgkName, pOpt->m_strDomNumHead);
	CHgBaseDrawStd::SetUseHCadBlock(!(pOpt->m_dwDrawCommon & COMMON_NOHCAD));
	CARcBridgeDBStandard::SetDisplayAngLenStd(pOpt->m_dwDrawCommon & COMMON_DISPLAY_ANG_LEN_BASE);

	CDrawDogak *pDgk = pStd->GetDogak();
	if(pDgk->m_bHCadDgk)
	{
		for(int i=0; i<pDgk->m_szAttdefPrompt.GetSize(); i++)
		{
			pOpt->m_strDgkAttrText.SetAt(i, m_Grid2.GetTextMatrix(i+1, 1));
			pDgk->m_szAttdefText.SetAt(i, m_Grid2.GetTextMatrix(i+1, 1));
		}
	}

	pStd->SetModifiedAll();
	m_pDoc->SetModifiedFlag();
}
