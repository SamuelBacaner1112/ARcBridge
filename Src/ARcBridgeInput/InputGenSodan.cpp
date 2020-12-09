// InputGenSodan.cpp : implementation file
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
// CInputGenSodan dialog


CInputGenSodan::CInputGenSodan(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputGenSodan::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputGenSodan)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nRowPrev = 0;
	m_bStt = FALSE;
	m_bEnd = FALSE;
	m_nTab = -1;
}


void CInputGenSodan::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputGenSodan)
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_cbBridgeNo);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputGenSodan, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputGenSodan)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID, OnCellChangedRow)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputGenSodan message handlers

BOOL CInputGenSodan::PreTranslateMessage(MSG* pMsg) 
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

void CInputGenSodan::OnPreInitDialog()
{
	SetResize(IDC_GRID,	SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_TAB,	SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	SetHyperHelp(m_pIn->GetManualPath());

	m_nRowPrev = 0;
	InitComboCtrl();
	InitTabCtrl();
	m_Grid.InitGrid(m_pIn->m_pARcBridgeDataStd->m_Fy);
	SetGridData();
}

void CInputGenSodan::InitComboCtrl()
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

void CInputGenSodan::InitTabCtrl()
{
	m_Tab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_Tab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	CRcBridgeApp* pBri = m_pIn->GetRcBridge();

	m_Tab.RemoveAllTabs(TRUE);

	if((pBri->IsWingWall(TRUE, TRUE) || pBri->IsWingWall(TRUE, FALSE)) && pBri->IsOutsideWall())
	{
		m_Tab.InsertItem(0, _T("시점측 날개벽"));
		m_bStt = TRUE;
	}
	else m_bStt = FALSE;

	if((pBri->IsWingWall(FALSE, TRUE) || pBri->IsWingWall(FALSE, FALSE)) && pBri->IsOutsideWall())
	{
		m_Tab.InsertItem(1, _T("종점측 날개벽"));
		m_bEnd = TRUE;
	}
	else m_bEnd = FALSE;

	m_Tab.SetCurSel(0);
	m_nTab = m_Tab.GetCurSel();
}

void CInputGenSodan::OnSelchangeComboBridgeNo() 
{
	SetDataSave();
	m_pIn->m_nCurrentBridge = m_cbBridgeNo.GetCurSel();
	InitTabCtrl();
	SetGridData();
	SetDataInit();

	DrawInputDomyunView(TRUE);
	m_cbBridgeNo.SetFocus();	
}

void CInputGenSodan::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	SetDataSave();

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	//DrawInputDomyunView();

	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);
}

void CInputGenSodan::OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	
	int nRow = pGridView->iRow;

	if(m_nRowPrev != 0 && m_nRowPrev != nRow) DrawInputDomyunView();
	m_nRowPrev = nRow;
}

void CInputGenSodan::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SetFocus();
	m_nTab = m_Tab.GetCurSel();
	SetGridData();
	SetDataInit();

	DrawInputDomyunView(TRUE);
	
	return;
}

void CInputGenSodan::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SetDataSave();
	
	return;
}

void CInputGenSodan::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(100);

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);

	int nRow = m_Grid.GetFocusCell().row;
	int nTab = m_Tab.GetCurSel();
	BOOL bStt = (nTab==0 && m_bStt) ? TRUE : FALSE;

	if(nRow==0 || nRow == -1) nRow = 2;
	BOOL bLeft = (nRow==2 && (m_Grid.GetItemText(2, 0)=="좌측")) ? TRUE : FALSE;

	if(!pBri->IsWingWall(bStt, bLeft) || !pBri->IsOutsideWall()) 
	{
		CInputDialogBase::DrawInputDomyunView(bZoomAll);
		return;
	}

	int se = bStt ? -1 : 1;
	int nLeft = bLeft ? -1*se : 1*se;	// 시점일 경우 교량 진행 방향의 좌우와 날개벽의 좌우가 반대임.
	//long nTypeJongdanForRebarOrg = pBri->m_nTypeJongdanForRebar;
	//pBri->m_nTypeJongdanForRebar = 2;

	pBri->GetTvVertSection_Std(nLeft, TRUE, FALSE, FALSE, 2);
	//pBri->SyncBridge(nLeft, TRUE);

	CDomyun Dom(pDom);

	//m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DBStd.m_pDrawWingWall->DrawGenFront(&Dom, bStt, bLeft);
	DBStd.m_pDrawWingWall->DrawGenCutBriSect(&Dom, bStt, bLeft);
	
	DBStd.m_pDrawWingWall->DrawGenSodan(&Dom, bStt, bLeft);

	DBStd.m_pDrawWingWall->DimGenSodan(&Dom, bStt, bLeft);

	*pDom << Dom;
	
	//pBri->m_nTypeJongdanForRebar	= nTypeJongdanForRebarOrg;
	pBri->GetTvVertSection_Std(nLeft, TRUE, FALSE, TRUE);

	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

void CInputGenSodan::SetDataDefault()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfoApp *pLine = NULL;

	for(long bri = 0; bri < pStd->GetBridgeSize(); bri++)
	{
		CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(bri);
		if(!pBri) continue;

		BOOL bFirst   = IsFirst(bri);
		pBri->SetDefaultedState(GetIDD(), 0x00000001);
		if(!bFirst && !IsClickDefaultButton(bri)) continue;
		
		double dStaStt = pBri->GetStationBridgeStt(FALSE);
		double dStaEnd = dStaStt + pBri->GetLengthBridge(FALSE);
		pLine = pStd->GetLineApp();		
		double dElStt = 0;
		double dElEnd = 0;

		pLine->GetLine(0)->GetElevationErathOnStation(dStaStt, dElStt);
		pLine->GetLine(0)->GetElevationErathOnStation(dStaEnd, dElEnd);

		if(bFirst)
		{
			pBri->m_pWingWallStt[0]->SetDataDefaultSodan(dElStt);
			pBri->m_pWingWallStt[1]->SetDataDefaultSodan(dElStt);

			pBri->m_pWingWallEnd[0]->SetDataDefaultSodan(dElEnd);
			pBri->m_pWingWallEnd[1]->SetDataDefaultSodan(dElEnd);
		}
		else
		{
			if(IsClickDefaultButton(bri))
			{
				CWingWall *pLWingWall, *pRWingWall;

				if(m_nTab==0 && m_bStt)
				{
					pLWingWall = pBri->m_pWingWallStt[0];
					pRWingWall = pBri->m_pWingWallStt[1];
				}
				else if(!m_bStt && !m_bEnd)
				{
					return;
				}
				else
				{
					pLWingWall = pBri->m_pWingWallEnd[0];
					pRWingWall = pBri->m_pWingWallEnd[1];
				}
				if(pLWingWall->m_bExist) pLWingWall->SetDataDefaultSodan(m_bStt ? dElStt:dElEnd);
				if(pRWingWall->m_bExist) pRWingWall->SetDataDefaultSodan(m_bStt ? dElStt:dElEnd);
			}
		}
	}


	SetGridData();
	SetDataInit();
}

void CInputGenSodan::SetDataInit()
{
	m_Grid.UpdateData(FALSE);

	long nRows(0), nCols(0);
	m_Grid.GetCellCount(nRows, nCols);
	m_Grid.MergeGridEx(0, 1, 0, nCols-1);
}

void CInputGenSodan::SetDataSave()
{
	m_Grid.UpdateData(TRUE);
}

void CInputGenSodan::SetGridData()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();

	m_Grid.SetGridDefault(3, 2, 2, 1);

	CWingWall *pLWingWall, *pRWingWall;
	
	if(m_nTab==0 && m_bStt)
	{
		pLWingWall = pBri->m_pWingWallStt[0];
		pRWingWall = pBri->m_pWingWallStt[1];
	}
	else if(!m_bStt && !m_bEnd)
	{
		return;
	}
	else
	{
		pLWingWall = pBri->m_pWingWallEnd[0];
		pRWingWall = pBri->m_pWingWallEnd[1];
	}

	// Title
	long row = 0;
	long col = 0;

	m_Grid.AddCell(row, col, "구분\n"); m_Grid.AddCell(row+1, col++, "구분\n");
	m_Grid.AddCell(row, col, "EL.(m)\n"); m_Grid.AddCell(row+1, col++, "EL.(m)\n");
	m_Grid.AddCell(row, col, "sD\n"); m_Grid.AddCell(row+1, col++, "sD\n");
	m_Grid.AddCell(row, col, "경사부 구배(1:n)"); m_Grid.AddCell(row+1, col++, "1단");
	m_Grid.AddCell(row, col, "경사부 구배(1:n)"); m_Grid.AddCell(row+1, col++, "2단이상");
	m_Grid.AddCell(row, col, "소단부"); m_Grid.AddCell(row+1, col++, "최대높이");
	m_Grid.AddCell(row, col, "소단부"); m_Grid.AddCell(row+1, col++, "소단폭");
	m_Grid.AddCell(row, col, "소단부"); m_Grid.AddCell(row+1, col++, "소단경사(%)");
	m_Grid.AddCell(row, col, "벽체전면"); m_Grid.AddCell(row+1, col++, "소단폭");
	m_Grid.AddCell(row, col, "벽체전면"); m_Grid.AddCell(row+1, col, "옹벽설치");
	col = 0;
	row = 2;

	if(pLWingWall->m_bExist)			// 좌측 날개벽 있음..
	{
		m_Grid.AddCell(row, col, "좌측");
		SetGridDataSoDan(row++, col+1, pLWingWall);
	}

	if(pRWingWall->m_bExist)			// 우측 날개벽 있음..
	{
		m_Grid.AddCell(row, col, "우측");
		SetGridDataSoDan(row, col+1, pRWingWall);
	}
	
	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(60);
	m_Grid.SetColumnWidth(7,85);
}

void CInputGenSodan::SetGridDataSoDan(long row, long col, CWingWall *pWingWall)
{
	UINT nFormat = DT_CENTER | DT_VCENTER;

	m_Grid.AddCell(row, col++, &pWingWall->m_dElSodanBottom);m_Grid.SetCellDec(CELL_DEC_3);
	m_Grid.AddCell(row, col++, &pWingWall->m_dDistNoriStart);
	m_Grid.AddCell(row, col++, &pWingWall->m_dSlopeNoriFirst); m_Grid.SetCellDec(CELL_DEC_1);
	m_Grid.AddCell(row, col++, &pWingWall->m_dSlopeNoriNext); m_Grid.SetCellDec(CELL_DEC_1);
	m_Grid.AddCell(row, col++, &pWingWall->m_dHeightMaxSodan);
	m_Grid.AddCell(row, col++, &pWingWall->m_dWidthSodan);
	m_Grid.AddCell(row, col++, &pWingWall->m_dSlopeSodan); m_Grid.SetCellDec(CELL_DEC_1);
	m_Grid.AddCell(row, col++, &pWingWall->m_dWidthSodanWallFront);
	m_Grid.AddCell(row, col++, &pWingWall->m_bRetainWallFront, nFormat, CELL_LINE_NO, "", CELL_TYPE_CHECK);
}

void CInputGenSodan::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	structGridCell *pCell = m_Grid.GetCurrentCell();
	if(pCell)
	{
		m_pIn->SetComment(pCell->sTitle);	
		CString sTitle	= m_Grid.GetItemText(0,pCell->nCol);
		CString subTitle = m_Grid.GetItemText(1, pCell->nCol);

		sTitle.TrimLeft("\n");
		sTitle.TrimRight("\n");

		CString sMark	= sTitle;
		
		
		if(sTitle == "경사부 구배(1:n)" && subTitle == "1단") sMark	= "1단(1:n)";
		if(sTitle == "경사부 구배(1:n)" && subTitle == "2단이상") sMark	= "2단이상(1:n)";

		if(sTitle == "nEL.(m)") sMark = "EL";
		if(sTitle == "sD")
		{
			sMark = sTitle;
		}

		if(sTitle == "소단부" && subTitle == "소단폭") sMark = "소단폭";
		if(sTitle == "소단부" && subTitle == "소단경사(%)") sMark.Format("%s%%", m_Grid.GetItemText(pCell->nRow, pCell->nCol));

		if(sTitle == "벽체전면" && subTitle == "소단폭") sMark = "벽체전면소단폭";

		m_pView->GetDomyun()->STMakeCursor(sMark);
	}
}

BOOL CInputGenSodan::IsValid()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	int nCountBri1 = 0;
	int nCountBri2 = 0;
	int nSizeBri = pStd->GetBridgeSize();
	for(int nBr=0; nBr<nSizeBri; nBr++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(nBr);

		if(!pBri->IsOutsideWall())
			nCountBri1++;
		if(pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB)
			nCountBri2++;
	}

	// 교량의 형식 중에 문형 라멘교나 박스교가 없을 경우 소단 제원 입력 메뉴를 표시하지 않음
	if(nSizeBri==nCountBri1)
		return FALSE;
	// 교량의 형식이 모두 슬래브교일 경우 기둥 위치 입력 이후의 메뉴를 표시하지 않음
	if(nSizeBri==nCountBri2)
		return FALSE;

	return TRUE;
}

BOOL CInputGenSodan::IsFocusItem(CString &szTitle)
{
	if(szTitle.Find("1단", 0) != -1) szTitle	= "1단";
	if(szTitle.Find("2단이상", 0) != -1) szTitle	= "2단이상";
	if(szTitle == "EL")	szTitle	= "\nEL.(m)\n";
	if(szTitle == "sD")	szTitle	= "\nsD\n";
	if(szTitle.Find("%", 0) != -1)	szTitle	= "소단경사(%)";

	if(GetDlgItem(IDC_GRID)) 
	{
		CGridCtrlMng *pGrid = (CGridCtrlMng *)GetDlgItem(IDC_GRID);
		if(pGrid->GetSafeHwnd())
		{
			for(long nCol=0; nCol<pGrid->GetColumnCount(); nCol++)
			{
				long nRowCheck	= 1;

				if(pGrid->GetTextMatrix(nRowCheck,nCol)==szTitle)
				{
					return TRUE;					
				}
			}
		}
	}
	return FALSE;
}

void CInputGenSodan::SetFocusItem(CString szTitle)
{
	long nRowFocus	= m_Grid.GetFocusCell().row;

	if(GetDlgItem(IDC_GRID)) 
	{
		CGridCtrlMng *pGrid = (CGridCtrlMng *)GetDlgItem(IDC_GRID);
		if(pGrid->GetSafeHwnd())
		{
			for(long nCol=0; nCol<pGrid->GetColumnCount(); nCol++)
			{
				long nRowCheck	= 1;

				if(pGrid->GetTextMatrix(nRowCheck, nCol)==szTitle)
				{
					GetFocus();
					//pGrid->ResetSelection();
					pGrid->SetSelectedRange(-1,-1,-1,-1);
					pGrid->SetFocusCell(nRowFocus,nCol);					
					pGrid->SendMessage(WM_KEYDOWN,VK_F2);
					break;
				}
			}
		}
	}
}