// DrawingOptionHyunTa.cpp : implementation file
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
// CDrawingOptionHyunTa property page

IMPLEMENT_DYNCREATE(CDrawingOptionHyunTa, COptionBase)

CDrawingOptionHyunTa::CDrawingOptionHyunTa() : COptionBase(CDrawingOptionHyunTa::IDD)
{
	//{{AFX_DATA_INIT(CDrawingOptionHyunTa)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = NULL;
	m_nCurBri = 0;
}

CDrawingOptionHyunTa::~CDrawingOptionHyunTa()
{
}

void CDrawingOptionHyunTa::DoDataExchange(CDataExchange* pDX)
{
	COptionBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawingOptionHyunTa)
	DDX_Control(pDX, IDC_OPT_TREE, m_Tree);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_OPT_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CDrawingOptionHyunTa, COptionBase)
	//{{AFX_MSG_MAP(CDrawingOptionHyunTa)
	ON_BN_CLICKED(IDC_OPT_DEFAULT, OnOptionDefault)
	ON_CBN_SELCHANGE(IDC_OPT_BRIDGE_NO, OnSelchangeComboBridgeNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawingOptionHyunTa message handlers

BOOL CDrawingOptionHyunTa::OnInitDialog() 
{
	COptionBase::OnInitDialog();
	
	m_nCurBri = (dynamic_cast<CARcBridgeDoc*>(m_pDoc))->m_nCurBriNoOfSettingDomyun;
	// TODO: Add extra initialization here
	InitTree();
	
	InitGrid();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDrawingOptionHyunTa::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(((COptionSheet*)GetParent())->m_cbBriNo.GetSafeHwnd()!=NULL)
		((COptionSheet*)GetParent())->m_cbBriNo.ShowWindow(TRUE);
	
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd *pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	
	if(((COptionSheet*)GetParent())->m_bSetOK)
		pOpt->m_nCurPropPage = 2;

	return COptionBase::OnSetActive();
}

void CDrawingOptionHyunTa::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;
	
	SetDataSave();
	
	pStd->SetModifiedAll();
	m_pDoc->SetModifiedFlag();
}

void CDrawingOptionHyunTa::OnSelchangeComboBridgeNo() 
{
	SetDataSave();

	m_nCurBri = ((COptionSheet*)GetParent())->m_cbBriNo.GetCurSel();

	m_Tree.DeleteAllItems();
	InitTree();
	InitGrid();
}

void CDrawingOptionHyunTa::OnOptionDefault()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nCurBri);

	pData->DefaultDrawingOptionHyunTa();

	m_Tree.DeleteAllItems();
	InitTree();
	InitGrid();
}

void CDrawingOptionHyunTa::InitGrid()
{
	CARcBridgeDoc* pDoc					= dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd			= pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd* pOpt	= pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nCurBri);
	CARcBridgeDataStd* pDataStd			 = pDoc->m_pARcBridgeDataStd;

	CRcBridgeApp *pBri = pDataStd->GetBridge(m_nCurBri);
	if( pBri == NULL ) return;

	m_Grid.DeleteAllItems();

	if(pBri->IsExsitHyunTa())
	{
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

		m_Grid.SetTextMatrix(0, 0, _T("���� �׸�"));
		m_Grid.SetTextMatrix(1, 0, _T("���鵵 ������"));
		m_Grid.SetTextMatrix(2, 0, _T("�ܸ鵵 ������"));
		m_Grid.SetTextMatrix(3, 0, _T("ö�ٿ������� ö�ǻ� ������"));
		m_Grid.SetTextMatrix(4, 0, _T("SPACER �� ������"));
		
		m_Grid.SetTextMatrix(0, 1, _T("���� ��"));
		m_Grid.SetTextMatrix(1, 1, _T("%.f"), pData->m_dScaleHyunTaFront, (UINT)DT_RIGHT);
		m_Grid.SetTextMatrix(2, 1, _T("%.f"), pData->m_dScaleHyunTaSection, (UINT)DT_RIGHT);
		m_Grid.SetTextMatrix(3, 1, _T("%.f"), pData->m_dScaleHyunTaDetailHold, (UINT)DT_RIGHT);
		m_Grid.SetTextMatrix(4, 1, _T("%.f"), pData->m_dScaleHyunTaDetailSpac, (UINT)DT_RIGHT);
	}
	m_Grid.SetRedraw(TRUE, TRUE);
}

void CDrawingOptionHyunTa::InitTree()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nCurBri);
	CRcBridgeApp* pBri = pDoc->m_pARcBridgeDataStd->GetBridge(m_nCurBri);
	if( pBri == NULL ) return;

	CWingWall* pWingBL = pBri->GetWingWall(TRUE, TRUE);
	CWingWall* pWingER = pBri->GetWingWall(FALSE, FALSE);
	CWingWall* pWingEL = pBri->GetWingWall(FALSE, TRUE);
	CWingWall* pWingBR = pBri->GetWingWall(TRUE, FALSE);

	m_Tree.SetBkColor(CLR_BACK);
	m_Tree.m_pOptions = &m_Options;

	// ������ �ִ� ����Ʈ ����
	for(POSITION pos=m_Tree.m_pOptions->m_Items.GetHeadPosition(); pos!=NULL;)
		delete m_Tree.m_pOptions->m_Items.GetNext(pos);
	m_Tree.m_pOptions->m_Items.RemoveAll();

	if(pBri->IsExsitHyunTa())
	{
		BOOL bFront	= (pData->m_dwDrawHyunTa & HYUNTA_FRONT) ? TRUE : FALSE;
		BOOL bSectA	= (pData->m_dwDrawHyunTa & HYUNTA_SECT_A) ? TRUE : FALSE;
		BOOL bSectB	= (pData->m_dwDrawHyunTa & HYUNTA_SECT_B) ? TRUE : FALSE;
		BOOL bSectC = (pData->m_dwDrawHyunTa & HYUNTA_SECT_C) ? TRUE : FALSE;
		BOOL bDetailhold	= (pData->m_dwDrawHyunTa & HYUNTA_DETAIL_HOLD) ? TRUE : FALSE;
		BOOL bDetailSpacer	= (pData->m_dwDrawHyunTa & HYUNTA_DETAIL_SPAC) ? TRUE : FALSE;
		BOOL bNote	= (pData->m_dwDrawHyunTa & HYUNTA_NOTE) ? TRUE : FALSE;

		m_Options.SetOptionItem(_T("���� ���"), IDB_ITEMS, IDB_ITEMS);
			m_Options.SetOption(_T("FRONT"), bFront ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("FRONT"),_T("\t���鵵"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption(_T("SECTION_A"), bSectA ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("SECTION_A"),_T("\t�ܸ鵵 A"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption(_T("SECTION_B"), bSectB ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("SECTION_B"),_T("\t�ܸ鵵 B"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption(_T("SECTION_C"), bSectC ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("SECTION_C"),_T("\t�ܸ鵵 C"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption(_T("DETAIL_HOLD"), bDetailhold ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("DETAIL_HOLD"),_T("\tö�ٿ������� ö�ǻ�"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption(_T("DETAIL_SPACER"), bDetailSpacer ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
			m_Options.SetOptionItem(_T("DETAIL_SPACER"),_T("\tSPACER ��"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
			m_Options.SetOption(_T("NOTE"), bNote ? CCustomOptions::CheckTrue : CCustomOptions::CheckFalse);
 			m_Options.SetOptionItem(_T("NOTE"),_T("\tNOTE"), CCustomOptions::CheckTrue, IDB_CHECKON, IDB_CHECKOFF);	
		m_Options.SetOptionItem(_T("���鵵 ǥ�� ���"), IDB_ITEMS, IDB_ITEMS);
			m_Options.SetOption("TYPE_HYUNTA_CUT", pData->m_nTypeHyunTaCut);
			m_Options.SetOptionItem("TYPE_HYUNTA_CUT", "\t���� ���̷� ǥ��", 0, IDB_RADIOON, IDB_RADIOOFF);
			m_Options.SetOptionItem("TYPE_HYUNTA_CUT", "\t���� ������", 1, IDB_RADIOON, IDB_RADIOOFF);
			m_Options.SetOptionItem("TYPE_HYUNTA_CUT", "\t�� ��������", 2, IDB_RADIOON, IDB_RADIOOFF);
		m_Options.SetOptionItem(_T("���� ���� ǥ�� ���"), IDB_ITEMS, IDB_ITEMS);
			m_Options.SetOption("TYPE_HYUNTA_VOL", pData->m_nTypeHyunTaVol);
			m_Options.SetOptionItem("TYPE_HYUNTA_VOL", "\t���� 1���� ����", 0, IDB_RADIOON, IDB_RADIOOFF);
			m_Options.SetOptionItem("TYPE_HYUNTA_VOL", "\t���� ��ü����", 1, IDB_RADIOON, IDB_RADIOOFF);

	}
	m_Options.BeginTrans();
	m_Tree.Initialize(NULL);
}

void CDrawingOptionHyunTa::SetDataSave()
{
	CARcBridgeDoc* pDoc					= dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd			= pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd* pOpt	= pDoc->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nCurBri);

	CRcBridgeApp *pBri = pDoc->m_pARcBridgeDataStd->GetBridge(m_nCurBri);
	pData->m_dwDrawHyunTa	= 0x000000;

	if(pBri->IsExsitHyunTa())
	{
		if(m_Options.GetOption(_T("FRONT")))	pData->m_dwDrawHyunTa |= HYUNTA_FRONT;
		if(m_Options.GetOption(_T("SECTION_A")))	pData->m_dwDrawHyunTa |= HYUNTA_SECT_A;
		if(m_Options.GetOption(_T("SECTION_B")))	pData->m_dwDrawHyunTa |= HYUNTA_SECT_B;
		if(m_Options.GetOption(_T("SECTION_C")))	pData->m_dwDrawHyunTa |= HYUNTA_SECT_C;
		if(m_Options.GetOption(_T("DETAIL_HOLD")))	pData->m_dwDrawHyunTa |= HYUNTA_DETAIL_HOLD;
		if(m_Options.GetOption(_T("DETAIL_SPACER")))	pData->m_dwDrawHyunTa |= HYUNTA_DETAIL_SPAC;
		
		int nRow = 1;
		pData->m_dScaleHyunTaFront	= m_Grid.GetTextMatrixDouble(nRow++, 1);
		pData->m_dScaleHyunTaSection	= m_Grid.GetTextMatrixDouble(nRow++, 1);
		pData->m_dScaleHyunTaDetailHold	= m_Grid.GetTextMatrixDouble(nRow++, 1);
		pData->m_dScaleHyunTaDetailSpac	= m_Grid.GetTextMatrixDouble(nRow++, 1);
		
		pData->m_nTypeHyunTaCut		= m_Options.GetOption("TYPE_HYUNTA_CUT");
		pData->m_nTypeHyunTaVol		= m_Options.GetOption("TYPE_HYUNTA_VOL");
	}
}