// InputGenColumn.cpp : implementation file
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
// CInputGenColumn dialog


CInputGenColumn::CInputGenColumn(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputGenColumn::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputGenColumn)
	m_nViewCross = -1;
	//}}AFX_DATA_INIT
	m_nTab = 0;
	m_bMove = FALSE;
	long i = 0; for(i = 0; i < MAX_JIJUM_COUNT_RC; i++)
		m_nTabInWall[i] = -1;
}


void CInputGenColumn::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputGenColumn)
	DDX_Control(pDX, IDC_TAB, m_Tab);
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_comboBridgeNo);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_GridControl(pDX, IDC_GRID_SECTION, m_GridSection);
	DDX_Radio(pDX, IDC_RADIO_CROSS, m_nViewCross);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputGenColumn, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputGenColumn)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
	ON_BN_CLICKED(IDC_RADIO_CROSS, OnRadioCross)
	ON_BN_CLICKED(IDC_RADIO_PLANE, OnRadioPlane)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID_SECTION, OnCellChangedDataSection)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID_SECTION, OnCellChangedSection)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputGenColumn message handlers
void CInputGenColumn::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	
	return;
}

void CInputGenColumn::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	SetFocus();
	m_nTab = m_Tab.GetCurSel();
	SetGridData();
	SetDataInit();
	DrawInputDomyunView(TRUE);

	return;
}

void CInputGenColumn::OnSelchangeComboBridgeNo() 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	m_pIn->m_nCurrentBridge = m_comboBridgeNo.GetCurSel();
	InitTabCtrl();
	SetControl();
	SetGridData();
	SetDataInit();
	SetPaneTitle();
	DrawInputDomyunView(TRUE);
	m_comboBridgeNo.SetFocus();
}

void CInputGenColumn::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	structGridCell *pCell = m_Grid.GetCurrentCell();
	if(pCell && m_Grid.GetFocus())
	{
		m_pIn->SetComment(pCell->sTitle);
		m_pView->GetDomyun()->STMakeCursor(m_Grid.GetItemText(0,pCell->nCol));
	}
}

void CInputGenColumn::OnCellChangedSection(NMHDR* pNotifyStruct, LRESULT* result)
{
	structGridCell *pCell = m_GridSection.GetCurrentCell();
	if(pCell)
	{
		m_pIn->SetComment(pCell->sTitle);
		m_pView->GetDomyun()->STMakeCursor(m_GridSection.GetItemText(0,pCell->nCol));
	}
}

void CInputGenColumn::OnSelchangeComboInwall() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	SetDataSave();
	InitTabCtrl();
	SetGridData();
	SetDataInit();
	DrawInputDomyunView(TRUE);
}


void CInputGenColumn::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	CCellID cell = m_Grid.GetFocusCell();
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();

	// 기둥개수를 변경할때마다 기둥간격만 권고안 해준다.
	
	CString str = m_Grid.GetTextMatrix(0, cell.col);
	if(str == "기둥수")
	{
		SetDataSave();
		long nIdxInWall = GetIdxCurInWall();
		if(nIdxInWall > -1)
		{
			CWallApp *pWall = pBri->GetInWall(nIdxInWall);
			if(pWall) 
			{
				pWall->SetDataDefaultDistColumn(FALSE);
				SetGridData();
				SetDataInit();
			}
		}
	}


	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);
}

void CInputGenColumn::OnCellChangedDataSection(NMHDR* pNotifyStruct, LRESULT* result)
{
	CCellID cell = m_GridSection.GetFocusCell();

	// 기둥개수를 변경할때마다 기둥간격만 권고안 해준다.
	
	CString str = m_GridSection.GetTextMatrix(0, cell.col);
	if(str == "단면형상")
	{
		SetDataSave();
		CWallApp *pWall = GetCurInWall();
		if(pWall) 
		{
			// 여기서는 특이하게 단면형상이 바뀌더라도 기둥의 폭이 벽체폭을 넘으면 
			// 안되므로 반지름을 기준으로 해서 항상 폭을 맞춰 준다.
			if(pWall->m_Column.m_nType == COL_TYPE_RECTANGLE)
				pWall->m_Column.m_dB[iOUTTER][iUPPER] = pWall->m_dW;
			else if(pWall->m_Column.m_nType == COL_TYPE_TRACK)
				pWall->m_Column.m_dB[iOUTTER][iUPPER] = pWall->m_dW/2-pWall->m_Column.m_dR[iOUTTER][iUPPER];
			SetGridData();
			SetDataInit();

			DrawInputDomyunView(TRUE);
		}
	}


	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);
}

void CInputGenColumn::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(m_pIn->m_strME.bViewMouseEvent)
	{
		CDPoint xy = m_pIn->m_strME.xy;

		// 벽체 선택 여부만 체크함.
		long nCount = pBri->GetCountInWall();
		long i = 0; for(i = 0; i < nCount; i++)
		{
			CWallApp *pWall = pBri->GetInWall(i);
			if(pWall)
			{
				if(pWall->m_tvArrPlan.IsSelect(xy))
				{
					SetDataSave();
					OnSelchangeComboInwall();
					break;
				}
			}
		}
	}
	
	
	CInputDialogBase::OnLButtonDblClk(nFlags, point);
}

void CInputGenColumn::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_pIn->m_strME.bViewMouseEvent)
	{

	}


	CInputDialogBase::OnLButtonDown(nFlags, point);
}

void CInputGenColumn::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if(m_pIn->m_strME.bViewMouseEvent)
	{

		if(m_bMove)
		{
			m_bMove = FALSE;
			DrawInputDomyunView(FALSE);
		}
	}

	CInputDialogBase::OnLButtonUp(nFlags, point);
}

void CInputGenColumn::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_pIn->m_strME.bViewMouseEvent) 
	{
		CDPoint xyMouse(0, 0);
		m_bMove = FALSE;

		if(m_bMove)
		{			
			SetDataInit();
			m_Grid.SetRedraw();
			m_pView->SetFocus();
			DrawInputDomyunView(FALSE);
		}
	}
	
	CInputDialogBase::OnMouseMove(nFlags, point);
}

void CInputGenColumn::SetGridData()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	CWallApp *pWall = GetCurInWall();
	if(!pWall) return;

	
	m_Grid.SetGridDefault(0, 0, 0, 0);
	m_GridSection.SetGridDefault(0, 0, 0, 0);
	SetGridDataColumnDist();
	SetGridDataColumnSection();
}

void CInputGenColumn::SetDataSave()
{
	m_Grid.UpdateData(TRUE);
	m_GridSection.UpdateData(TRUE);

	// 트랙형은 B값을 자동 지정해 준다.
	CWallApp *pWall	= GetCurInWall();
	if(pWall)
	{
		if(pWall->m_bTopIsColumn)
		{
			if(pWall->m_Column.m_nType == COL_TYPE_TRACK)
			{
				// 가로 값 체크
				double dW	= pWall->GetWidthWall();
				pWall->m_Column.m_dB[iOUTTER][iUPPER]	= dW;

				// 가로 값 체크
				if(pWall->m_Column.m_dB[iOUTTER][iUPPER] < pWall->m_Column.m_dR[iOUTTER][iUPPER] * 2)
				{
					pWall->m_Column.m_dR[iOUTTER][iUPPER] = pWall->m_Column.m_dB[iOUTTER][iUPPER] / 2;
				}

				// 세로 값 체크
				if(pWall->m_Column.m_dH[iOUTTER][iUPPER] < pWall->m_Column.m_dR[iOUTTER][iUPPER] * 2)
				{
					pWall->m_Column.m_dR[iOUTTER][iUPPER] = pWall->m_Column.m_dH[iOUTTER][iUPPER] / 2;
				}

				SetDataInit();
			}
		}
	}
}

void CInputGenColumn::SetDataDefault()
{
	for(long bri = 0; bri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		CRcBridgeApp *pBri = m_pIn->m_pARcBridgeDataStd->GetBridge(bri);
		CRcBridgeRebar *pBriRebar = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(bri);
		if(!pBri) return;
		CWallApp *pWall = GetCurInWall();
		if(!pWall) return;

		BOOL bFirst   = IsFirst(bri);
		pBri->SetDefaultedState(GetIDD(), 0x00000001);
		if(!bFirst && !IsClickDefaultButton(bri)) continue;

		if(!bFirst)
		{
			pBri->SetDataDefaultShapeTopInWall(GetIdxCurInWall());
			if(pWall) 	pWall->SetDataDefaultDistColumn(TRUE);
		}
		else
		{
			long i = 0; for(i = 0; i < pBri->GetCountInWall(); i++)
			{
				long nJ	= i + 1;

				pBri->SetDataDefaultShapeTopInWall(i);
				pBri->GetInWall(i)->SetDataDefaultDistColumn(TRUE);

				pWall->m_Column.SetDataDefaultRebarMain_Plane();
				pWall->m_Column.SetDataDefaultRebarCrossTie_Plane();
				pBriRebar->SetDataDefaultRebarRoundAndCrossTie_VertSection_InWall(i);
				pBriRebar->SetDataDefaultRebarMainInGirder_Girder(nJ, TRUE, TRUE, TRUE, FALSE, TRUE);
				pBriRebar->SetDataDefaultRebarGirder(nJ, FALSE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, TRUE);
				pBriRebar->SetDataDefaultRebarMainInGirder_Girder(nJ, TRUE, TRUE, TRUE, FALSE, FALSE);
				pBriRebar->SetDataDefaultRebarGirder(nJ, FALSE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE);
			}
		}
	}

	SetGridData();
	SetDataInit();
}

void CInputGenColumn::InitTabCtrl()
{
	m_Tab.RemoveAllTabs();
	m_Tab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_Tab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	
	
	long i = 0; for(i = 0; i < MAX_JIJUM_COUNT_RC; i++)
		m_nTabInWall[i] = -1;
	m_sArrTabName.RemoveAll();
	
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
		
	if(pBri->GetCountInWall() > 0 && pBri->m_nCountInWallHunch > 0)
	{
		CString str = _T("");
		long nTab = 0;

		long i = 0; for(i = 0; i < pBri->GetCountInWall(); i++)
		{
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB) str.Format("중간 지점부 %d", i+1);
			else str.Format("중간 지점부 %d", i+1);
			m_sArrTabName.Add(str); m_nTabInWall[i] = nTab++;
		}
	}

	for(i = 0; i < m_sArrTabName.GetSize(); i++)
		m_Tab.InsertItem(i, m_sArrTabName[i]);

	if(m_Tab.GetItemCount() <= m_nTab) m_nTab = 0;
	m_Tab.SetCurSel(m_nTab);
}

void CInputGenColumn::OnPreInitDialog()
{
	InitComboCtrl();
	InitTabCtrl();

	SetResize(IDC_TAB,			   SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID,					   SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID_SECTION,			   SZ_TOP_RIGHT,	SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_VIEW, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RADIO_CROSS, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RADIO_PLANE, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetHyperHelp(m_pIn->GetManualPath());

	m_Grid.InitGrid(m_pIn->m_pARcBridgeDataStd->m_Fy);
	m_GridSection.InitGrid(m_pIn->m_pARcBridgeDataStd->m_Fy);
	SetGridData();
}

void CInputGenColumn::SetDataInit()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	CWallApp *pWall = GetCurInWall();

	if(!pWall)
	{
		m_pIn->GridMessage(&m_Grid, "중간지점에 벽체가 존재하지 않습니다.");
		m_pIn->GridMessage(&m_GridSection, "중간지점에 벽체가 존재하지 않습니다.");
		return;
	}

	if(!pWall->m_bIs) 
	{
		m_pIn->GridMessage(&m_Grid, "중간지점에 벽체가 존재하지 않습니다.");
		m_pIn->GridMessage(&m_GridSection, "중간지점에 벽체가 존재하지 않습니다.");
		return;
	}

	if(!pWall->m_bTopIsColumn)
	{
		m_pIn->GridMessage(&m_Grid, "중간지점의 벽체가 기둥식이 아닙니다.");
		m_pIn->GridMessage(&m_GridSection, "중간지점의 벽체가 기둥식이 아닙니다.");
		return;
	}
	
	m_Grid.UpdateData(FALSE);
	m_GridSection.UpdateData(FALSE);
}


void CInputGenColumn::SetControl()
{
	// 내측벽체헌치
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
}

void CInputGenColumn::DrawInputDomyunView(BOOL bZoomAll)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(m_pIn->GetScaleInputDialog());

	CDomyun Dom(pDom);
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, m_pIn->m_pARcBridgeDataStd);

	// 필요한 좌표들 모두 계산.
	long nCountWall = pBri->GetCountInWall();
	long i = 0; for(i = 0; i < nCountWall; i++)
	{
		CWallApp *pWall = pBri->GetInWall(i);
		if(pWall)
		{
			pWall->GetTvPlan();
		}
	}


	if(m_nViewCross == 0)	// 횡단면 보기
	{
		CWallApp *pWall = GetCurInWall();
		if(pWall)
		{
			pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 2);

			// 횡단면 상부 슬래브
			double sta = pWall->m_xyOrg.x;
			CDPoint vAng = pBri->GetAngleJijum(m_nTab+1);
			m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
			DBStd.m_pDrawCrossSection->DrawCrossSection(&Dom, sta, vAng);
			DBStd.m_pDrawCrossSection->DrawCrossSlabHunch(&Dom, sta, vAng);	// 슬래브 헌치
			

			DBStd.m_pDrawWall->DimLeftPlane(&Dom, pWall, TRUE);

			// 횡단 기둥
			DBStd.m_pDrawWall->DrawLeftSide(&Dom, pWall);

			DBStd.m_pDrawFooting->DrawLeftSide(&Dom, &pWall->m_footing, FALSE);

			pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, 2);
		}
	}
	else	// 평면 보기
	{
		// 교량 평면
		DBStd.m_pDrawPlane->DrawPlaneSlabOnlyOutLine(&Dom);
		DBStd.m_pDrawPlane->DrawPlaneSlabSttEnd(&Dom);
		DBStd.m_pDrawPlane->DrawPlaneCenterLineEachSlab(&Dom, 2000);
		DBStd.m_pDrawPlane->MarkPlaneArrowDirection(&Dom, 3000);
		
		// 벽체 평면
		for(i = 0; i < nCountWall; i++)
		{
			CWallApp *pWall = pBri->GetInWall(i);
			if(pWall)
			{
				pWall->GetTvPlan();

				DBStd.m_pDrawWall->DrawPlan(&Dom, pWall);
				DBStd.m_pDrawWall->DimRightPlane(&Dom, pWall, 2, GetIdxCurInWall()==i);
			}
		}

		// 기둥 단면
		CWallApp *pWall = GetCurInWall();
		if(pWall)
		{
			if(pWall->m_bTopIsColumn)
			{
				//DBStd.m_pDrawColumn->DrawPlan(&Dom, &pWall->m_Column);
				DBStd.m_pDrawColumn->DimPlan(&Dom, &pWall->m_Column, 0, TRUE);
			}
		}

		// 회전
		DBStd.RotateDomByBridgeSttEndXy(&Dom);
	}


	

	*pDom << Dom;

	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

BOOL CInputGenColumn::IsValid()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	int nSizeBri = pStd->GetBridgeSize();
	BOOL bColumn	= FALSE;
	for(int nBr=0; nBr<nSizeBri; nBr++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(nBr);
		
		long nCountInWall	= pBri->GetCountInWall();
		long wall = 0; for(wall = 0; wall < nCountInWall; wall++)
		{
			CWallApp *pWall	= pBri->GetInWall(wall);
			if(!pWall) continue;
			if(!pWall->m_bIs) continue;
			if(pWall->m_bTopIsColumn)
			{
				bColumn	= TRUE;
				break;
			}
		}

		if(bColumn)
			break;
	}

	return bColumn;
}

void CInputGenColumn::InitComboCtrl()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	CString str = _T("");
	// 교량
	long nSize = pStd->GetBridgeSize();
	long i = 0; for(i = 0; i < nSize; i++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(i);
		str = pStd->GetStringBridgeOrder(i) + _T(" : ") + pBri->m_strBridgeName
			+ _T(" [") + pBri->GetStringBridgeType() + _T("]");
		m_comboBridgeNo.AddString(str);
	}
	m_comboBridgeNo.SetCurSel(m_pIn->m_nCurrentBridge);

	SetControl();
}

BOOL CInputGenColumn::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		UpdateData();
		if(m_Grid.GetFocusCell().col == m_Grid.GetCols()-1)
			pMsg->wParam = VK_RIGHT;
		else
			pMsg->wParam = VK_TAB;
	}

	return CInputDialogBase::PreTranslateMessage(pMsg);
}

void CInputGenColumn::SetGridDataColumnDist()
{
	m_Grid.SetGridDefault(0, 0, 0, 0);

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	CWallApp *pWall = GetCurInWall();
	if(!pWall) return;
	
	CStringArray sArr;
	CString str = _T("");
	CString strHelp = _T("");
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;

	if(pWall->m_bTopIsColumn)
	{
		long row = 0;
		long col = 0;

		m_Grid.SetGridDefault(2, 2, 1, 0);
		long i = 0; for(i = 0; i < MAX_COLUMN; i++)
		{
			str.Format("%d개", i+1);
			sArr.Add(str);
		}
		m_Grid.AddCell(row, col, "기둥수"); m_Grid.AddCell(row+1, col++, &pWall->m_nCountColumn, nFormat, TRUE, nLine, "기둥개수", CELL_TYPE_COMBO, sArr); m_Grid.SetCellBaseOfIndex(1);

		for(i = 0; i < pWall->m_nCountColumn; i++)
		{
			str.Format("%d", i+1);
			m_Grid.AddCell(row, col, "CL"+str); m_Grid.AddCell(row+1, col++, &pWall->m_dDistColumn[i], nFormat, nLine, str+"번째 기둥의 간격", CELL_TYPE_AUTO);
		}


		m_Grid.GetCellCount(row, col);
		m_Grid.SetRows(row);
		m_Grid.SetCols(col);
		m_Grid.SetColumnWidthAll(50);
		m_Grid.SetColumnWidth(0, 100);
	}
}

void CInputGenColumn::SetGridDataColumnSection()
{
	m_GridSection.SetGridDefault(0, 0, 0, 0);

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	CWallApp *pWall = GetCurInWall();
	if(!pWall) return;
	
	CStringArray sArr;
	sArr.Add("원형"); 
	sArr.Add("사각형"); 
	sArr.Add("트랙형"); 
//	sArr.Add("다각형");		(ARCBRIDGE-3276) 다각형 지원하지 않음

	CString str = _T("");
	CString strHelp = _T("");
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;
	CDoubleArray dArrColWidth100;

	if(pWall->m_bTopIsColumn)
	{
		long row = 0;
		long col = 0;

		m_GridSection.SetGridDefault(2, 2, 1, 0);

		dArrColWidth100.Add(col);
		m_GridSection.AddCell(row, col, "단면형상"); m_GridSection.AddCell(row+1, col++, &pWall->m_Column.m_nType, nFormat, TRUE, nLine, "단면형상", CELL_TYPE_COMBO, sArr);		

		if(pWall->m_Column.m_nType == COL_TYPE_CIRCLE)
		{
			m_GridSection.AddCell(row, col, "D"); m_GridSection.AddCell(row+1, col++, &pWall->m_Column.m_dB[iOUTTER][iUPPER], nFormat, CELL_READONLY, "", CELL_TYPE_AUTO);
		}
		else if(pWall->m_Column.m_nType == COL_TYPE_RECTANGLE)
		{
			m_GridSection.AddCell(row, col, "B"); m_GridSection.AddCell(row+1, col++, &pWall->m_Column.m_dB[iOUTTER][iUPPER], nFormat, CELL_READONLY, "", CELL_TYPE_AUTO);
			m_GridSection.AddCell(row, col, "H"); m_GridSection.AddCell(row+1, col++, &pWall->m_Column.m_dH[iOUTTER][iUPPER], nFormat, nLine, "", CELL_TYPE_AUTO);
		}
		else if(pWall->m_Column.m_nType == COL_TYPE_TRACK)
		{
			m_GridSection.AddCell(row, col, "R"); m_GridSection.AddCell(row+1, col++, &pWall->m_Column.m_dR[iOUTTER][iUPPER], nFormat, nLine, "", CELL_TYPE_AUTO);
			//m_GridSection.AddCell(row, col, "B"); m_GridSection.AddCell(row+1, col++, &pWall->m_Column.m_dB, nFormat, CELL_READONLY, "", CELL_TYPE_AUTO);
			m_GridSection.AddCell(row, col, "H"); m_GridSection.AddCell(row+1, col++, &pWall->m_Column.m_dH[iOUTTER][iUPPER], nFormat, nLine, "", CELL_TYPE_AUTO);
		}
		else if(pWall->m_Column.m_nType == COL_TYPE_POLY)
		{
			m_GridSection.AddCell(row, col, "각수"); m_GridSection.AddCell(row+1, col++, &pWall->m_Column.m_nAngSu, nFormat, FALSE, nLine, "", CELL_TYPE_AUTO);
			dArrColWidth100.Add(col);
			m_GridSection.AddCell(row, col, "회전각도(˚)"); m_GridSection.AddCell(row+1, col++, &pWall->m_Column.m_dAngPlanPoly, nFormat, nLine, "", CELL_TYPE_AUTO);
		}

		m_GridSection.GetCellCount(row, col);
		m_GridSection.SetRows(row);
		m_GridSection.SetCols(col);
		m_GridSection.SetColumnWidthAll(50);
		long i = 0; for(i = 0; i < dArrColWidth100.GetSize(); i++)
			m_GridSection.SetColumnWidth((int)dArrColWidth100.GetAt(i) , 100);
		
	}
}

long CInputGenColumn::GetIdxCurInWall()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(pBri)
	{
		long i = 0; for(i = 0; i < pBri->GetCountInWall(); i++)
		{
			if(m_nTabInWall[i] == m_nTab) return i;
		}
	}

	return -1;
}

CWallApp* CInputGenColumn::GetCurInWall()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(pBri)
	{
		long nIdxInWall = GetIdxCurInWall();
		if(nIdxInWall > -1)
		{
			CWallApp *pWall = pBri->GetInWall(nIdxInWall);
			return pWall;
		}
	}
	return NULL;
}

void CInputGenColumn::OnRadioCross() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DrawInputDomyunView(TRUE);
}

void CInputGenColumn::OnRadioPlane() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DrawInputDomyunView(TRUE);
}


BOOL CInputGenColumn::IsFocusItem(CString &szTitle)
{
	long nRowCheck	= 0;

	for(long grid = 0; grid < 2; grid++)
	{
		long nGrid	= grid == 0 ? IDC_GRID : IDC_GRID_SECTION;
		if(GetDlgItem(nGrid)) 
		{
			CGridCtrlMng *pGrid = (CGridCtrlMng *)GetDlgItem(nGrid);
			if(pGrid->GetSafeHwnd())
			{
				for(long nCol=0; nCol<pGrid->GetColumnCount(); nCol++)
				{
					if(pGrid->GetTextMatrix(nRowCheck,nCol)==szTitle)
					{
						return TRUE;					
					}
				}
			}
		}
	}

	return FALSE;
}

void CInputGenColumn::SetFocusItem(CString szTitle)
{
	long nRowCheck	= 0;
	long nRowFocus	= 1;

	for(long grid = 0; grid < 2; grid++)
	{
		long nGrid	= grid == 0 ? IDC_GRID : IDC_GRID_SECTION;

		if(GetDlgItem(nGrid)) 
		{
			CGridCtrlMng *pGrid = (CGridCtrlMng *)GetDlgItem(nGrid);
			if(pGrid->GetSafeHwnd())
			{
				for(long nCol=0; nCol<pGrid->GetColumnCount(); nCol++)
				{
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
}