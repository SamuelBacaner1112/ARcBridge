// InputGenTogong.cpp : implementation file
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
// CInputGenTogong dialog


CInputGenTogong::CInputGenTogong(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputGenTogong::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputGenTogong)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nTab = 0;
	m_nViewDom = 0;

	m_nTabWallEnd = -1;
	m_nTabWallStt = -1;
	for (HGINT32 ix = 0; ix < MAX_JIJUM_COUNT_RC; ++ix)
	{
		m_nTabInWall[ix] = -1;
	}
}

void CInputGenTogong::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputGenTogong)
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_cbBridgeNo);
	DDX_Control(pDX, IDC_COMBO_VIEWDOMYUN, m_cbViewDom);
	DDX_Control(pDX, IDC_COMBO_DIGGING_POS, m_cbDiggingPos);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputGenTogong, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputGenTogong)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_CBN_SELCHANGE(IDC_COMBO_VIEWDOMYUN, OnSelchangeComboViewDom)
	ON_CBN_SELCHANGE(IDC_COMBO_DIGGING_POS, OnSelchangeComboDiggingPos)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID, OnCellChangedRow)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputGenTogong message handlers

BOOL CInputGenTogong::PreTranslateMessage(MSG* pMsg) 
{
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

void CInputGenTogong::OnPreInitDialog()
{
	SetResize(IDC_GRID,	SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_TAB,	SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_DIGGING_POS, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_COMBO_DIGGING_POS, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_COMBO_VIEWDOMYUN, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetHyperHelp(m_pIn->GetManualPath());

	InitComboCtrl();
	InitTabCtrl();
	m_Grid.InitGrid(m_pIn->m_pARcBridgeDataStd->m_Fy);
	SetGridData();
}

void CInputGenTogong::InitComboCtrl()
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
	m_cbViewDom.ResetContent();
	m_cbViewDom.AddString("단면도");
	m_cbViewDom.AddString("정면도");
	m_cbViewDom.SetCurSel(m_nViewDom);

	SetCurSelRubble(); //기초 터파기 형식
}

void CInputGenTogong::InitTabCtrl()
{
	m_Tab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_Tab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();

	long nTab = 0;
	m_nTabWallStt = -1;
	m_nTabWallEnd = -1;
	long i = 0; for(i = 0; i < MAX_JIJUM_COUNT_RC; i++)
		m_nTabInWall[i] = -1;
	m_sArrTabName.RemoveAll();

	m_Tab.RemoveAllTabs(TRUE);

	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		m_sArrTabName.Add(_T("함형라멘교"));
		m_nTabWallStt = nTab++;
	}
	else
	{
		if(pBri->IsOutsideWall()) 
		{
			m_sArrTabName.Add(_T("시점 벽체")); 
			m_nTabWallStt = nTab++;
		}
		CString str = _T("");

		for(i = 0; i < pBri->GetCountInWall(); i++)
		{
			CWallApp *pWall	= pBri->GetInWall(i);
			if(!pWall) continue;
			if(!pWall->m_bIs) continue;

			if(!(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 && (i==0 ||i==pBri->GetCountInWall()-1)))
			{
				str.Format("중간 지점부 %d", i+1);
				m_sArrTabName.Add(str); m_nTabInWall[i] = nTab++;
			}
		}

		if(pBri->IsOutsideWall()) 
		{
			m_sArrTabName.Add(_T("종점 벽체")); 
			m_nTabWallEnd = nTab++;
		}
	}

	for(i = 0; i< m_sArrTabName.GetSize(); i++)
	{
		m_Tab.InsertItem(i, m_sArrTabName.GetAt(i));
	}
	m_nTab = 0;
}

void CInputGenTogong::OnSelchangeComboBridgeNo() 
{
	SetDataSave();
	m_pIn->m_nCurrentBridge = m_cbBridgeNo.GetCurSel();
	InitTabCtrl();
	SetGridData();
	SetDataInit();
	SetPaneTitle();

	DrawInputDomyunView(TRUE);
	m_cbBridgeNo.SetFocus();
}

void CInputGenTogong::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	SetDataSave();
	SetGridData();
	SetDataInit();

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	//DrawInputDomyunView(FALSE);

	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);
}

void CInputGenTogong::OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result)
{
	DrawInputDomyunView();
}

void CInputGenTogong::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SetFocus();
	m_nTab = m_Tab.GetCurSel();
	SetGridData();
	SetDataInit();
	InitComboCtrl();

	DrawInputDomyunView(TRUE);
	
	return;
}

void CInputGenTogong::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SetDataSave();
	
	return;
}

void CInputGenTogong::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(100);

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) return;


	CRcBridgeRebar* pBriDraw = NULL;
	if(pBri->IsBoxType())
	{
		pBriDraw = new CRcBridgeRebar;
		// Box형식 일때는 수량용으로 만들어진 교량을 그려준다
		if(pStd->MakeRcBridgeForBM(pBri, pBriDraw, TRUE) == FALSE)
		{
			delete pBriDraw;
			return;
		}
	}
	else
	{
		pBriDraw	= pBri;
	}

	CARcBridgeDBStandard DBStd(pBriDraw, m_pIn->m_pARoadOptionStd, m_pIn->m_pARcBridgeDataStd);

	BOOL bDrawRubble = pBriDraw->IsBoxType() && pBriDraw->m_dThickCobbStone > 0 && pBriDraw->m_nTypeLower == LOWERSLAB_TYPE_COBBSTONE;
	int nIndex = GetJijum();

	if(nIndex < 0 || nIndex > pBri->m_nQtyJigan) 
	{
		// 입력창 view에 여백 주기
		CInputDialogBase::DrawInputDomyunView(bZoomAll);
		if(pBri->IsBoxType())
			delete pBriDraw;
		return;
	}

	CDomyun Dom(pDom);
	if(m_nViewDom == 0)			// 단면도
	{
		if(pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
		{
			pBriDraw->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 1);
			BOOL bTrap = (pBriDraw->m_nTypeBridge == BRIDGE_TYPE_TRAP);
			DBStd.m_pDrawVertSection->DrawPartByJijum(&Dom, nIndex, bTrap);
		}
		else
		{
			//pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 2);
			pBriDraw->GetTvVertSection_Std(0, FALSE, FALSE, FALSE);

			DBStd.m_pDrawVertSection->DrawVertSection(&Dom, TRUE, &pBriDraw->m_tvArrVertSection);
			DBStd.m_pDrawExFooting->DrawFront(&Dom, &pBriDraw->m_footingBox.m_exFooting, FALSE);
			if(bDrawRubble)
				DBStd.m_pDrawVertSection->DrawExFootingBoxType(&Dom, TRUE);
			long nCount = pBriDraw->GetCountInWall();

			for(long ix = 0; ix < nCount; ix++)
			{
				CWallApp *pWall = pBriDraw->GetInWall(ix);
				if(pWall)
					DBStd.m_pDrawWall->DrawFront(&Dom, pWall, TRUE);
			}
		}

		// 토공선 그림
 		DBStd.m_pDrawVertSection->DrawTogongLine(&Dom, pBriDraw, nIndex, TRUE, TRUE);
 		DBStd.m_pDrawVertSection->DimTogongLine(&Dom, pBriDraw, nIndex, TRUE, TRUE, TRUE);

		
		pBriDraw->GetTvVertSection_Std(0, TRUE, FALSE, TRUE);
	}
	else if(m_nViewDom == 1)	// 정면도
	{
		if(pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
			pBriDraw->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 2, FALSE);
		else
			pBriDraw->GetTvVertSection_Std(0, TRUE, FALSE, FALSE);

		DBStd.m_pDrawCrossSection->DrawCrossAllAtJijum(&Dom, nIndex, TRUE, FALSE, TRUE, TRUE, bDrawRubble);

		if(nIndex == 0 || nIndex == pBriDraw->m_nQtyJigan)
		{
			BOOL bInnerAtLeft	= pBriDraw->m_nTypeWallForRebarGen==0 ? FALSE : TRUE;
			BOOL bInnerAtRight	= bInnerAtLeft;
			DBStd.m_pDrawCrossSection->DrawCrossSection_OutWall_Exp(&Dom, nIndex==0, bInnerAtLeft, bInnerAtRight);
		}

		CFootingApp *pFooting = pBriDraw->GetFootingByJijumNumber(nIndex);
		if(pFooting)
		{
			// 토공선 그림
			DBStd.m_pDrawCrossSection->DrawTogongLine(&Dom, nIndex, TRUE, TRUE, TRUE, pBriDraw->m_nDeductNearTupagiBM);
			DBStd.m_pDrawCrossSection->DimTogongLine(&Dom, nIndex, FALSE, TRUE, pBriDraw->m_nDeductNearTupagiBM);
		}

		pBriDraw->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, 2);
	}

	
	*pDom << Dom;

	if(pBri->IsBoxType())
	{
		delete pBriDraw;
	}

	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

void CInputGenTogong::SetGridData()
{
	m_Grid.SetGridDefault(0, 0, 0, 0);

	long nJ	= GetJijum();
	
	SetGridDataAtJijum(nJ);
}

void CInputGenTogong::SetDataDefault()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CLineInfoApp *pLine = pStd->GetLineApp();

	for(long bri = 0; bri < pStd->GetBridgeSize(); bri++)
	{
		CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(bri);
		if(!pBri) continue;

		BOOL bFirst   = IsFirst(bri);
		pBri->SetDefaultedState(GetIDD(), 0x00000001);
		if(!bFirst && !IsClickDefaultButton(bri)) continue;

		CFootingApp *pfooting	= NULL;
		double dEL				= 0;

		if(!bFirst)
		{
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
				pfooting = &pBri->m_footingBox;
			else if(pBri->IsOutsideWall() && m_nTab == 0)
				pfooting = &pBri->m_footingStt;
			else if(pBri->IsOutsideWall() && m_nTab == m_nTabWallEnd)
				pfooting = &pBri->m_footingEnd;
			else
			{
				if(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2)
				{
					
				}
				else
				{
					long nIndex = GetJijum()-1;
					CWallApp *pWall = pBri->GetInWall(nIndex);
					if(!pWall) continue;
					pfooting = &pWall->m_footing;
				}
			}
			if(!pfooting) continue;

			CBoringData data;
			pfooting->m_dDiggingSlopeTosa = data.m_dGubeTosa;	// 터파기구배 (토사)  
			pfooting->m_dDiggingSlopePungwhaam = data.m_dGubePungwhaam; //  (풍화암)
			pfooting->m_dDiggingSlopeYunam = data.m_dGubeYunam;	 //  (연암)
			pfooting->m_dDiggingSlopeGyungam = data.m_dGubeGyungam;	 //  (경암)
			
			pfooting->SetDataDefaultTogong();
			pLine->GetLine(0)->GetElevationErathOnStation(pfooting->m_dSta,dEL);
			pfooting->m_dEL_DiggingFirst = dEL;
			
			pfooting->m_dHeightPavement = pBri->m_dBTHS;
			pfooting->m_dHeightNosang = 1000;

			pfooting->m_nTypeTupagiForLean = 1;
			pfooting->m_dAddHeightTupagi = 0;
		}
		else
		{
			int i=0; for(i=0; i<pBri->GetCountJijum(); i++)
			{
				if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
					pfooting = &pBri->m_footingBox;
				else if(i == 0)
					pfooting = &pBri->m_footingStt;
				else if(i == pBri->GetCountInWall()+1)
					pfooting = &pBri->m_footingEnd;
				else
				{
					CWallApp *pWall = pBri->GetInWall(i-1);
					pfooting = &pWall->m_footing;
				}
				CBoringData data;
				pfooting->m_dDiggingSlopeTosa = data.m_dGubeTosa;	// 터파기구배 (토사)  
				pfooting->m_dDiggingSlopePungwhaam = data.m_dGubePungwhaam; //  (풍화암)
				pfooting->m_dDiggingSlopeYunam = data.m_dGubeYunam;	 //  (연암)
				pfooting->m_dDiggingSlopeGyungam = data.m_dGubeGyungam;	 //  (경암)
				
				pfooting->SetDataDefaultTogong();
				pLine->GetLine(0)->GetElevationErathOnStation(pfooting->m_dSta,dEL);
				pfooting->m_dEL_DiggingFirst = dEL;
				
				pfooting->m_dHeightPavement = pBri->m_dBTHS;
				pfooting->m_dHeightNosang = 1000;

				pfooting->m_nTypeTupagiForLean = 1;
				pfooting->m_dAddHeightTupagi = 0;
			}
		}
		
	}
	SetGridData();
	SetDataInit();
}

void CInputGenTogong::OnSelchangeComboViewDom() 
{
	m_nViewDom = m_cbViewDom.GetCurSel();

	DrawInputDomyunView();	
}

void CInputGenTogong::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	structGridCell *pCell = m_Grid.GetCurrentCell();
	if(pCell)
	{
		m_pIn->SetComment(pCell->sTitle);	
		CString sTitle	= m_Grid.GetItemText(0,pCell->nCol);

		sTitle.TrimLeft("\n");
		sTitle.TrimRight("\n");
		sTitle.TrimRight("\n(m)");
		CString sMark	= sTitle;

		if(m_nTab == m_nTabWallStt || m_nTab == m_nTabWallEnd)
		{
			
			if(sTitle == "n") sMark	= "1:n(V:H)";
		}

		if(sTitle == "터파기 구배")
		{
			sMark = m_Grid.GetItemText(1, pCell->nCol);
		}
		if(sTitle.Find("지하수위", 0) != -1)
			sMark	= "EL2";

		m_pView->GetDomyun()->STMakeCursor(sMark);
	}
}

void CInputGenTogong::SetDataInit()
{
	m_Grid.UpdateData(FALSE);

	long nRows(0), nCols(0);
	m_Grid.GetCellCount(nRows, nCols);
	m_Grid.MergeGrid(0, 1, 0, nCols-1);
}

void CInputGenTogong::SetDataSave()
{
	m_Grid.UpdateData(TRUE);
}

// 시,종 점측 벽체
void CInputGenTogong::SetGridDataAtJijum(long nJijum)
{
	// 그리드 초기화
	m_Grid.SetGridDefault(3, 2, 2, 0);
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	if(nJijum < 0 || nJijum > pBri->m_nQtyJigan) return;

	long row = 0;
	long col = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	CFootingApp *pFooting;
	

	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX) pFooting = &pBri->m_footingBox;
	else if(nJijum == 0) pFooting = &pBri->m_footingStt;
	else if(nJijum == pBri->GetCountJijum()-1) pFooting = &pBri->m_footingEnd;
	else
	{
		pFooting = &(pBri->GetInWall(nJijum-1)->m_footing);
	}
	
	m_Grid.AddCell(row, col, "EL1\n(m)"); m_Grid.AddCell(row+1, col, "EL1\n(m)");
	m_Grid.AddCell(row+2, col++, &pFooting->m_dEL_DiggingFirst, nFormat, 0, "", CELL_TYPE_METER); m_Grid.SetCellDec(CELL_DEC_3);
	m_Grid.AddCell(row, col, "지하수위\n(m)"); m_Grid.AddCell(row+1, col, "지하수위\n(m)"); 
	m_Grid.AddCell(row+2, col++, &pFooting->m_dEL_DiggingWater, nFormat,  0, "", CELL_TYPE_METER); m_Grid.SetCellDec(CELL_DEC_3);
	m_Grid.AddCell(row, col, "Pf\n"); m_Grid.AddCell(row+1, col, "Pf\n"); m_Grid.AddCell(row+2, col++, &pFooting->m_dDistOffsetDigging);
	if(nJijum == 0 || nJijum == pBri->GetCountJijum()-1)
	{
		m_Grid.AddCell(row, col, "Db\n"); m_Grid.AddCell(row+1, col, "Db\n"); m_Grid.AddCell(row+2, col++, &pFooting->m_dDistOffsetBackFill);
		m_Grid.AddCell(row, col, "n\n"); m_Grid.AddCell(row+1, col, "n\n"); m_Grid.AddCell(row+2, col++, &pFooting->m_dSlopeBackFill); m_Grid.SetCellDec(CELL_DEC_1);
	}
	m_Grid.AddCell(row, col, "터파기 구배"); m_Grid.AddCell(row+1, col, "토사");
	m_Grid.AddCell(row+2, col++, &pFooting->m_dDiggingSlopeTosa, nFormat); m_Grid.SetCellDec(CELL_DEC_1);
	m_Grid.AddCell(row, col, "터파기 구배"); m_Grid.AddCell(row+1, col, "풍화암"); 
	m_Grid.AddCell(row+2, col++, &pFooting->m_dDiggingSlopePungwhaam, nFormat); m_Grid.SetCellDec(CELL_DEC_1);
	m_Grid.AddCell(row, col, "터파기 구배"); m_Grid.AddCell(row+1, col, "연암");
	m_Grid.AddCell(row+2, col++, &pFooting->m_dDiggingSlopeYunam, nFormat); m_Grid.SetCellDec(CELL_DEC_1);
	m_Grid.AddCell(row, col, "터파기 구배"); m_Grid.AddCell(row+1, col, "경암"); 
	m_Grid.AddCell(row+2, col++, &pFooting->m_dDiggingSlopeGyungam, nFormat); m_Grid.SetCellDec(CELL_DEC_1);

	m_Grid.AddCell(row, col, "굴착 깊이"); m_Grid.AddCell(row+1, col, "최초굴착");
	m_Grid.AddCell(row+2, col++, &pFooting->m_dDepthDiggingFirst, nFormat);
	m_Grid.AddCell(row, col, "굴착 깊이"); m_Grid.AddCell(row+1, col, "단위굴착");
	m_Grid.AddCell(row+2, col++, &pFooting->m_dDepthDiggingNext, nFormat);

	if(nJijum == 0 || nJijum == pBri->GetCountJijum()-1)
	{
		m_Grid.AddCell(row, col, "H1\n"); m_Grid.AddCell(row+1, col, "H1\n"); m_Grid.AddCell(row+2, col++, &pFooting->m_dHeightPavement);
		m_Grid.AddCell(row, col, "H2\n"); m_Grid.AddCell(row+1, col, "H2\n"); m_Grid.AddCell(row+2, col++, &pFooting->m_dHeightNosang);
	}

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(60);
}

BOOL CInputGenTogong::IsValid()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	int nCountBri = 0;
	int nSizeBri = pStd->GetBridgeSize();
	for(int nBr=0; nBr<nSizeBri; nBr++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(nBr);

		if(pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB)
			nCountBri++;
	}

	// 교량의 형식이 모두 슬래브교일 경우 기둥 위치 입력 이후의 메뉴를 표시하지 않음
	if(nSizeBri==nCountBri)
		return FALSE;

	return TRUE;
}

long CInputGenTogong::GetJijum()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return -1;

	long nJ	= -1;

	if(m_nTab == m_nTabWallStt)
		nJ	= m_nTabWallStt;
	else if(m_nTab == m_nTabWallEnd)
		nJ	= pBri->m_nQtyJigan;
	else
	{
		long i = 0; for(i = 0; i < pBri->GetCountInWall(); i++)
		{
			if(m_nTab == m_nTabInWall[i])
			{
				nJ	= i+1;
				break;
			}
		}
	}

	return nJ;
}


BOOL CInputGenTogong::IsFocusItem(CString &szTitle)
{

	if(szTitle == "Pf")	szTitle	= "\nPf\n";
	if(szTitle == "Db")	szTitle	= "\nDb\n";
	if(szTitle == "1:n(V:H)")	szTitle	= "\nn\n";
	if(szTitle == "H1")	szTitle	= "\nH1\n";
	if(szTitle == "H2")	szTitle	= "\nH2\n";

	if(GetDlgItem(IDC_GRID)) 
	{
		CGridCtrlMng *pGrid = (CGridCtrlMng *)GetDlgItem(IDC_GRID);
		if(pGrid->GetSafeHwnd())
		{
			long nRowCheck	= 1;

			for(long nCol=0; nCol<pGrid->GetColumnCount(); nCol++)
			{
				if(pGrid->GetTextMatrix(nRowCheck,nCol)==szTitle)
				{
					return TRUE;					
				}
			}
		}
	}
	return FALSE;
}

void CInputGenTogong::SetFocusItem(CString szTitle)
{
	long nRowFocus	= m_Grid.GetFocusCell().row;

	if(GetDlgItem(IDC_GRID)) 
	{
		CGridCtrlMng *pGrid = (CGridCtrlMng *)GetDlgItem(IDC_GRID);
		if(pGrid->GetSafeHwnd())
		{
			long nRowCheck	= 1;

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

void CInputGenTogong::OnSelchangeComboDiggingPos() 
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return ;

	UpdateData();

	CFootingApp *pFooting = NULL;
	if(pBri->IsBoxType())
		pFooting =&pBri->m_footingBox;
	else if(pBri->IsOutsideWall() && m_nTab == 0)
		pFooting =&pBri->m_footingStt;
	else if(pBri->IsOutsideWall() && m_nTab == m_nTabWallEnd)
		pFooting =&pBri->m_footingEnd;
	else if(!(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 || m_nTab == 0 || m_nTab == m_nTabWallEnd))
	{
		long nIndex = GetJijum()-1;
		CWallApp *pWall = pBri->GetInWall(nIndex);
		if(!pWall) return;
		pFooting = &pWall->m_footing;
	}

	if(pFooting)
	{
		pFooting->m_nTypeTupagiForLean = m_cbDiggingPos.GetCurSel();

		if(pBri->IsBoxType())
		{
			if(pFooting->m_nTypeTupagiForLean == 2)
				pFooting->m_dAddHeightTupagi = pBri->m_dThickCobbStone;
			else
				pFooting->m_dAddHeightTupagi = 0;
		}
	}
	
	DrawInputDomyunView(FALSE);
}

void CInputGenTogong::SetCurSelRubble()
{
	CRcBridgeApp *pBriCurr = m_pIn->GetRcBridge();
	if(pBriCurr == NULL)	return;

	m_cbDiggingPos.ResetContent();
	long nTupagiForLen = 1;

	CFootingApp *pFooting = NULL;
	if(pBriCurr->IsBoxType())
		pFooting =&pBriCurr->m_footingBox;
	else if(pBriCurr->IsOutsideWall() && m_nTab == 0)
		pFooting =&pBriCurr->m_footingStt;
	else if(pBriCurr->IsOutsideWall() && m_nTab == m_nTabWallEnd)
		pFooting =&pBriCurr->m_footingEnd;
	else if(!(pBriCurr->m_nTypeBridge == BRIDGE_TYPE_TRAP2 || m_nTab == 0 || m_nTab == m_nTabWallEnd))
	{
		long nIndex = GetJijum()-1;
		CWallApp *pWall = pBriCurr->GetInWall(nIndex);
		if(!pWall) return;
		pFooting = &pWall->m_footing;
	}

	if(pFooting)
	{
		nTupagiForLen = pFooting->m_nTypeTupagiForLean;
				
		m_cbDiggingPos.AddString(_T("기초 하면"));
		m_cbDiggingPos.AddString(_T("확대기초 하면"));
		if(pBriCurr->IsBoxType() && pBriCurr->m_nTypeLower == BOXFOOTING_TYPE_STONE)
			m_cbDiggingPos.AddString(_T("잡석기초 하면"));

		m_cbDiggingPos.SetCurSel(nTupagiForLen);
	}
}