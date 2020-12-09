// InputGenMonument.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "../ARcBridgeDBDraw/ARcBridgeDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputGenMonument dialog


CInputGenMonument::CInputGenMonument(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputGenMonument::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputGenMonument)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CInputGenMonument::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputGenMonument)
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_cbBridgeNo);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputGenMonument, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputGenMonument)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputGenMonument message handlers

BOOL CInputGenMonument::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		UpdateData();
		if(m_Grid.GetFocusCell().col == m_Grid.GetCols()-1)
			pMsg->wParam = VK_RIGHT;
		else
			pMsg->wParam = VK_TAB;
	}
	
	return CInputDialogBase::PreTranslateMessage(pMsg);
}

void CInputGenMonument::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);

	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;

	int nCol = pGridView->iColumn;
	if(nCol == 0)
	{
		m_Grid.UpdateData(TRUE);
		SetDataInit();
		DrawInputDomyunView(TRUE);
	}
}

void CInputGenMonument::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	
	int nCol = pGridView->iColumn;

	CString str = m_Grid.GetTextMatrix(0, nCol);
	m_pView->GetDomyun()->STMakeCursor(str);
}

void CInputGenMonument::OnPreInitDialog()
{
	SetResize(IDC_GRID,	SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	SetHyperHelp(m_pIn->GetManualPath());

	InitComboCtrl();
}

void CInputGenMonument::InitComboCtrl()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	int nSize = pStd->GetBridgeSize();

	CString str = _T("");
	int i=0; for(i=0; i<nSize; i++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(i);
		str = pStd->GetStringBridgeOrder(i) + _T(" : ") + pBri->m_strBridgeName
			+ _T(" [") + pBri->GetStringBridgeType() + _T("]");
		m_cbBridgeNo.AddString(str);
	}
	m_cbBridgeNo.SetCurSel(m_pIn->m_nCurrentBridge);
}

void CInputGenMonument::OnSelchangeComboBridgeNo() 
{
	m_pIn->m_nCurrentBridge = m_cbBridgeNo.GetCurSel();

	SetDataInit();
	SetPaneTitle();

	DrawInputDomyunView();
}

void CInputGenMonument::DrawInputDomyunView(BOOL bZoomAll)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(20);

	if(pBri->m_nMonumenu==0) 
	{
		CInputDialogBase::DrawInputDomyunView(bZoomAll);
		return;
	}

	double dSpace1 = 200.0;
	double dSpace2 = 400.0;

	CDomyun DomFront(pDom);

	DBStd.m_pDrawCrossSection->DrawMonumentFront(&DomFront);
	DBStd.m_pDrawCrossSection->DimMonumentFront(&DomFront);

	CDRect rcFront = DomFront.GetExtRect();

	m_pIn->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_SMALL_TITLE);
	DomFront.SetTextAlignHorz(TA_CENTER);
	DomFront.SetTextHeight(5);
	DomFront.TextOut(rcFront.CenterPoint().x, rcFront.bottom+dSpace1, _T("정 면 도"));

	DomFront.SetCalcExtRect();
	CDRect rect= DomFront.GetExtRect();
	DomFront.SetLineColor(-1);
	DomFront.Point(rect.left-rect.Width()/10,rect.bottom+rect.Height()/10);

	*pDom << DomFront;

	CDomyun DomPlane(pDom);

	DBStd.m_pDrawCrossSection->DrawMonumentPlane(&DomPlane);
	DBStd.m_pDrawCrossSection->DimMonumentPlane(&DomPlane);

	CDRect rcPlane = DomPlane.GetExtRect();

	m_pIn->m_pARoadOptionStd->SetEnvType(&DomPlane, HCAD_SMALL_TITLE);
	DomPlane.SetTextAlignHorz(TA_CENTER);
	DomPlane.SetTextHeight(5);
	DomPlane.TextOut(rcPlane.CenterPoint().x, rcPlane.bottom+dSpace1, _T("평 면 도"));

	DomPlane.Move(rcFront.Width()+dSpace2, rcFront.bottom-rcPlane.bottom);

	DomPlane.SetCalcExtRect();
	rect= DomPlane.GetExtRect();
	DomPlane.SetLineColor(-1);
	DomPlane.Point(rect.right+rect.Width()/10,rect.top-rect.Height()/10);

	*pDom << DomPlane;

	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

BOOL CInputGenMonument::IsValid()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	//int nCountBri1 = 0;
	//int nCountBri2 = 0;
	int nCountMonument = 0;
	int nSizeBri = pStd->GetBridgeSize();
	for(int nBr=0; nBr<nSizeBri; nBr++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(nBr);

		/*			
		if(!pBri->IsOutsideWall())
			nCountBri1++;
		if(pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB)
			nCountBri2++;
		if(pBri->IsOutWall(TRUE) && pBri->IsOutWall(FALSE) && pBri->m_nMonumenu>0)
			nCountMonument++;
		*/
		// 날개벽이 없어도 표시
		if(pBri->m_nMonumenu>0)
			nCountMonument++;
	}

	// 교량의 형식 중에 문형 라멘교나 박스교가 없을 경우 날개벽 제원 입력 메뉴를 표시하지 않음
	//if(nSizeBri==nCountBri1)
	//	return FALSE;
	// 교량의 형식이 모두 슬래브교일 경우 기둥 위치 입력 이후의 메뉴를 표시하지 않음
	//if(nSizeBri==nCountBri2)
	//	return FALSE;
	 
	// (ARCBRIDGE-3182) 교명주 받침 설치 갯수 입력을 여기로 이동
	// 교명주가 없을때 표시하지 않음
//	if(nCountMonument == 0)
//		return FALSE;

	return TRUE;
}

void CInputGenMonument::SetDataDefault()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	for(long bri = 0; bri < pStd->GetBridgeSize(); bri++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(bri);
		
		// 날개벽이 없어도 표시
		/*
		if(pBri->m_nTypeBridge != BRIDGE_TYPE_PORTAL)
		{
			if(pBri->m_nTypeBridge != BRIDGE_TYPE_TRAP2)
				continue;
		}
		*/

		if(!IsDefaulted(bri, 0x00000001) || IsClickDefaultButton(bri))
		{
			pBri->m_dMonuH1 = 300.0;
			pBri->m_dMonuH2 = 500.0;
			pBri->m_dMonuH3 = 550.0;
			pBri->m_dMonuD  = 550.0;
			pBri->m_dMonuW  = 950.0;
			pBri->m_dMonuL  = 1400.0;
		}
	}

	m_pIn->m_pDoc->SetModifiedFlag();
}

void CInputGenMonument::SetGridTitle()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	
	int nRows = 1;
	int nCols = 7;

	m_Grid.SetGridDefault(2, 7, 1, 0);

	m_Grid.AddCell(0, 0, _T("교명주 받침 설치"));
	m_Grid.AddCell(0, 1, _T("H1"));
	m_Grid.AddCell(0, 2, _T("H2"));
	m_Grid.AddCell(0, 3, _T("H3"));
	m_Grid.AddCell(0, 4, _T("D"));
	m_Grid.AddCell(0, 5, _T("W"));
	m_Grid.AddCell(0, 6, _T("L"));
}

void CInputGenMonument::SetDataInit()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	// 날개벽이 없어도 표시
	/*
	if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
	{
		if(pBri->m_nTypeBridge != BRIDGE_TYPE_TRAP2)
		{
			m_Grid.SetRedraw(TRUE,TRUE);
			return;
		}
	}
	*/

	SetGridTitle();
//	if(pBri->m_nMonumenu==0) return;

	UINT nLine = pBri->m_nMonumenu == 0 ? CELL_READONLY : CELL_LINE_NO;

	int nCol = 0;
	CStringArray sArr;
	sArr.RemoveAll();
	sArr.Add("적용안함");
	sArr.Add("1개");
	sArr.Add("2개");
	sArr.Add("3개");
	sArr.Add("4개");

	m_Grid.AddCell(1, nCol++, &pBri->m_nMonumenu, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(1, nCol++, &pBri->m_dMonuH1, DT_CENTER, nLine);
	m_Grid.AddCell(1, nCol++, &pBri->m_dMonuH2, DT_CENTER, nLine);
	m_Grid.AddCell(1, nCol++, &pBri->m_dMonuH3, DT_CENTER, nLine);
	m_Grid.AddCell(1, nCol++, &pBri->m_dMonuD, DT_CENTER, nLine);
	m_Grid.AddCell(1, nCol++, &pBri->m_dMonuW, DT_CENTER, nLine);
	m_Grid.AddCell(1, nCol++, &pBri->m_dMonuL, DT_CENTER, nLine);

	m_Grid.UpdateData(FALSE);
	
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(80);
	m_Grid.SetColumnWidth(0, 100);
	m_Grid.SetTextBkColor(CLR_BACK);
}

void CInputGenMonument::SetDataSave()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	m_Grid.UpdateData(TRUE);
	
	m_pIn->m_pDoc->SetModifiedFlag();
}