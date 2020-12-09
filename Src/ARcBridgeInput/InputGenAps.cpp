// InputGenAps.cpp : implementation file
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
// CInputGenAps dialog


CInputGenAps::CInputGenAps(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputGenAps::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputGenAps)
	m_sFck = _T("");
	m_sFy = _T("");
	//}}AFX_DATA_INIT
	m_nTab  = 0;
	m_bMove	= FALSE;
	m_nTabEnd[0] = -1;
	m_nTabEnd[1] = -1;
	m_nTabStt[0] = -1;
	m_nTabStt[1] = -1;
	m_pDlgCheck = new CInputCheckEdit;
}

CInputGenAps::~CInputGenAps()
{
	delete m_pDlgCheck;
}

void CInputGenAps::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputGenAps)
	DDX_Control(pDX, IDC_CHECK_STT, m_checkStt);
	DDX_Control(pDX, IDC_CHECK_END, m_checkEnd);
	DDX_Control(pDX, IDC_CHECK_BRACKET, m_checkBracket);
	DDX_Control(pDX, IDC_CHECK_DIVISION, m_checkDivision);
	DDX_Control(pDX, IDC_COMBO_CNS, m_comboCns);
	DDX_Control(pDX, IDC_BUTTON_RECOMMAND2, m_btnDefault2);
	DDX_Control(pDX, IDC_CHECK_WING, m_checkWing);
	DDX_Control(pDX, IDC_CHECK_ORTHO, m_checkOrtho);
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_comboBridgeNo);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	DDX_Text(pDX, IDC_EDIT_FCK, m_sFck);
	DDX_Text(pDX, IDC_EDIT_FY, m_sFy);
	DDX_Control(pDX, IDC_BUTTON_EXAMINE, m_btnExamine);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputGenAps, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputGenAps)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_CHECK_ORTHO, OnCheckOrtho)
	ON_BN_CLICKED(IDC_CHECK_WING, OnCheckWing)
	ON_BN_CLICKED(IDC_BUTTON_RECOMMAND2, OnButtonDefaultInput2)
	ON_CBN_SELCHANGE(IDC_COMBO_CNS, OnSelchangeComboCns)
	ON_BN_CLICKED(IDC_CHECK_DIVISION, OnCheckDivision)
	ON_BN_CLICKED(IDC_CHECK_BRACKET, OnCheckBracket)
	ON_BN_CLICKED(IDC_CHECK_END, OnCheckEnd)
	ON_BN_CLICKED(IDC_CHECK_STT, OnCheckStt)
	ON_EN_CHANGE(IDC_EDIT_FCK, OnChangeEditFck)
	ON_EN_CHANGE(IDC_EDIT_FY, OnChangeEditFy)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
	ON_EN_KILLFOCUS(IDC_EDIT_FCK, OnKillfocusEditFck)
	ON_EN_KILLFOCUS(IDC_EDIT_FY, OnKillfocusEditFy)
	ON_BN_CLICKED(IDC_BUTTON_EXAMINE, OnButtonExamine)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputGenAps message handlers
void CInputGenAps::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	SetDataSave();

	return;
}

void CInputGenAps::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	SetFocus();
	m_nTab = m_Tab.GetCurSel();
	SetGridData();
	SetDataInit();
	SetControl();
	DrawInputDomyunView(TRUE);

	m_tvArrApsSection.RemoveAll();
	return;
}

void CInputGenAps::SetGridData()
{
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 1, 0);
	m_Grid.SetCellSize(20, -1);

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	CApsApp *pAps = GetAps();
	if(!pAps) return;
	long nCountCns	= GetCountCns();
	
	CDPointArray xyArr;	// 배경색 반전위치
	CStringArray sArr;
	CString str = _T("");
	CString strHelp = _T("");
	long row = 0;
	long col = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;

	CFootingApp *pFooting = &pBri->m_footingEnd;

	m_Grid.AddCell(row, col, "L1"); m_Grid.AddCell(row+1, col++,  &pAps->m_dL1);
	m_Grid.AddCell(row, col, "L2"); m_Grid.AddCell(row+1, col++,  &pAps->m_dL2);
	m_Grid.AddCell(row, col, "L3"); m_Grid.AddCell(row+1, col++,  &pAps->m_dL3);
	for(long cns = 0; cns < nCountCns; cns++)
	{
		CApsApp *pCns	= GetCns(cns);
		if(cns == 0)
		{
			m_Grid.AddCell(row, col, "CD1"); m_Grid.AddCell(row+1, col++,  &pCns->m_dAD1);
			m_Grid.AddCell(row, col, "CD2"); m_Grid.AddCell(row+1, col++,  &pCns->m_dAD2);
		}
		str.Format("CL%d", cns+1);
		m_Grid.AddCell(row, col, str); m_Grid.AddCell(row+1, col++,  &pCns->m_dL1);
	}
	m_Grid.AddCell(row, col, "H1"); m_Grid.AddCell(row+1, col++,  &pAps->m_dH1);
	m_Grid.AddCell(row, col, "H2"); m_Grid.AddCell(row+1, col++,  &pAps->m_dH2);
	m_Grid.AddCell(row, col, "AD1"); m_Grid.AddCell(row+1, col++,  &pAps->m_dAD1, nFormat, nLine, "시점에서 종점으로 봤을때, 접속슬래브의 왼쪽측면과 교량 왼쪽측면과의 이격거리");
	m_Grid.AddCell(row, col, "AD2"); m_Grid.AddCell(row+1, col++,  &pAps->m_dAD2, nFormat, nLine, "시점에서 종점으로 봤을때, 접속슬래브의 오른쪽측면과 교량 오른쪽측면과의 이격거리");
	m_Grid.AddCell(row, col, "S(%)"); m_Grid.AddCell(row+1, col++,  &pAps->m_dSlope);
	m_Grid.AddCell(row, col, "CTC"); m_Grid.AddCell(row+1, col++,  &pAps->m_dCtcDowelBar, nFormat, nLine, "Dowel Bar 설치 간격");
	m_Grid.AddCell(row, col, "D"); m_Grid.AddCell(row+1, col++,  &pAps->m_dDiaDowelBar, nFormat, nLine, "Dowel Bar 지름", CELL_TYPE_DIA);
	m_Grid.AddCell(row, col, "L"); m_Grid.AddCell(row+1, col++,  &pAps->m_dLenDowelBar, nFormat, nLine, "Dowel Bar 길이");
	m_Grid.AddCell(row, col, "sT"); m_Grid.AddCell(row+1, col++,  &pAps->m_dST, nFormat, nLine, "접속슬래브와 교량본체간 이격거리");
	if(IsStt() && pBri->m_bBracketLengthStt)
	{
		if(IsLeft())
		{
			m_Grid.AddCell(row, col, "BL");
			m_Grid.AddCell(row+1, col++,  &pBri->m_dBrkLenStt[0], nFormat, nLine, "시점측 브래킷과 접속 슬래브의 좌측 이격거리");
			if(!pBri->m_bDivisionApsStt)
			{
				m_Grid.AddCell(row, col, "BR");
				m_Grid.AddCell(row+1, col++,  &pBri->m_dBrkLenStt[1], nFormat, nLine, "시점측 브래킷과 접속 슬래브의 우측 이격거리");
			}
		}
		else
		{
			m_Grid.AddCell(row, col, "BR");
			m_Grid.AddCell(row+1, col++,  &pBri->m_dBrkLenStt[1], nFormat, nLine, "시점측 브래킷과 접속 슬래브의 우측 이격거리");
		}
	}
	if(!IsStt() && pBri->m_bBracketLengthEnd)
	{
		if(IsLeft())
		{
			m_Grid.AddCell(row, col, "BL");
			m_Grid.AddCell(row+1, col++,  &pBri->m_dBrkLenEnd[0], nFormat, nLine, "종점측 브래킷과 접속 슬래브의 좌측 이격거리");
			if(!pBri->m_bDivisionApsEnd)
			{
				m_Grid.AddCell(row, col, "BR");
				m_Grid.AddCell(row+1, col++,  &pBri->m_dBrkLenEnd[1], nFormat, nLine, "종점측 브래킷과 접속 슬래브의 우측 이격거리");
			}
		}
		else
		{
			m_Grid.AddCell(row, col, "BR");
			m_Grid.AddCell(row+1, col++,  &pBri->m_dBrkLenEnd[1], nFormat, nLine, "종점측 브래킷과 접속 슬래브의 우측 이격거리");
		}
	}

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(50);

	// 상태바에서 단위계 변경시 fck 단위계 변경시켜야 되므로 호출함.
	SetControl();
}

void CInputGenAps::SetDataSave()
{
	m_Grid.UpdateData(TRUE);
	SyncFckFy();
}

void CInputGenAps::SetDataDefault()
{
	for(long bri = 0; bri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		CRcBridgeApp *pBri = m_pIn->m_pARcBridgeDataStd->GetBridge(bri);
		if(!pBri) return;

		if(!IsFirst(bri))
		{
			if(IsClickDefaultButton(bri))
			{
				for(long left = 0; left < 2; left++)
				{
					BOOL bLeft	= (left == iLEFT);
					if(m_nTab == m_nTabStt[left]) pBri->SetDataDefaultShapeAps(TRUE, bLeft, TRUE);
					if(m_nTab == m_nTabEnd[left]) pBri->SetDataDefaultShapeAps(FALSE, bLeft, TRUE);
				}
			}
		}
		else
		{
			for(long left = 0; left < 2; left++)
			{
				BOOL bLeft	= (left == iLEFT);
				pBri->SetDataDefaultShapeAps(TRUE, bLeft, TRUE);
				pBri->SetDataDefaultShapeAps(FALSE, bLeft, TRUE);
				pBri->SetDefaultedState(GetIDD(), 0x00000001);
			}
		}
	}

	SetControl();
	SetGridData();
	SetDataInit();
}

void CInputGenAps::InitTabCtrl()
{
	m_Tab.RemoveAllTabs();
	m_Tab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_Tab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	long nTab	= 0;
	long left	= 0;
	CString str	= _T("");
	for(left = 0; left < 2; left++)
	{
		m_nTabStt[left]	= -1;
		m_nTabEnd[left]	= -1;
	}
	m_sArrTabName.RemoveAll();
	if(pBri->IsOutWall(TRUE))
	{
		for(left = 0; left < 2; left++)
		{
			if(pBri->m_apsStt[left].m_bIs) 
			{
				str	= left == iLEFT ? (pBri->m_bDivisionApsStt ? "시점측 접속 슬래브(좌)" : "시점측 접속 슬래브") : "시점측 접속 슬래브(우)";
				m_sArrTabName.Add(str);
				m_nTabStt[left] = nTab++;
			}
		}
	}
	if(pBri->IsOutWall(FALSE)) 
	{
		for(left = 0; left < 2; left++)
		{
			if(pBri->m_apsEnd[left].m_bIs)
			{
				str	= left == iLEFT ? (pBri->m_bDivisionApsEnd ? "종점측 접속 슬래브(좌)" : "종점측 접속 슬래브") : "종점측 접속 슬래브(우)";
				m_sArrTabName.Add(str);
				m_nTabEnd[left] = nTab++;
			}
		}
	}
	
	long i = 0; for(i = 0; i < m_sArrTabName.GetSize(); i++)
		m_Tab.InsertItem(i, m_sArrTabName[i]);

	if(m_Tab.GetItemCount() <= m_nTab) m_nTab = 0;
	m_Tab.SetCurSel(m_nTab);
}

void CInputGenAps::OnPreInitDialog()
{
	// 시작하면서 pick box의 크기를 정해줌
	if(m_pView) 
	{
		CTwinVector tv;
		tv.SetPickBoxSize(m_pView->GetSelectionRectSize()/m_pView->GetRatioWindow());
	}
	
	InitTabCtrl();
	InitComboCtrl();


	SetResize(IDC_BUTTON_EXAMINE,	SZ_BOTTOM_LEFT,	SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_RECOMMAND2,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_TAB,			  SZ_TOP_LEFT,	SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID,			  SZ_TOP_LEFT,	SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_OPTION,  SZ_TOP_RIGHT,	SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_CNS,	  SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_APS,	  SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	SetResize(IDC_COMBO_CNS,      SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_ORTHO,    SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_WING,     SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_DIVISION, SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_BRACKET,  SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_STT,	  SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_END,	  SZ_TOP_RIGHT,	SZ_TOP_RIGHT);

	SetHyperHelp(m_pIn->GetManualPath());

	m_Grid.InitGrid(3000);	// 여기서는 다웰바만 입력 받으므로 무조건 D바가 나와야 된다.
	SetGridData();
}

BOOL CInputGenAps::PreTranslateMessage(MSG* pMsg) 
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
		
	structGridCell *cell = m_Grid.GetCurrentCell();
	if(cell)
	{
		
	}

	return CInputDialogBase::PreTranslateMessage(pMsg);
}

void CInputGenAps::InitComboCtrl()
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

	// 완추슬래브 개수
	for(long cns = 0; cns <= MAX_CNS; cns++)
	{
		if(cns == 0) str = "없음";
		else str.Format("%d개", cns);
		m_comboCns.AddString(str);
	}

	SetControl();
}

void CInputGenAps::OnSelchangeComboBridgeNo() 
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

void CInputGenAps::DrawInputDomyunView(BOOL bZoomAll)
{
	CRcBridgeRebar*pBri = m_pIn->GetRcBridgeRebar();
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(m_pIn->GetScaleInputDialog());

	CDomyun Dom(pDom);
	//pBri->SyncBridge(0, FALSE, TRUE, TRUE);
	pBri->GetTvVertSection_Std(0, TRUE, FALSE);
	pBri->SyncBridgeAps(0, TRUE);
	
	CApsApp *pAps = GetAps();
	if(pAps->m_bIs==FALSE) 
	{
		CInputDialogBase::DrawInputDomyunView(bZoomAll);
		return;
	}
	CARcBridgeDBStandard DBStd(m_pIn->GetRcBridgeRebar(), m_pIn->m_pARoadOptionStd, m_pIn->m_pARcBridgeDataStd);

	// 평면도
	// 평면도는 좌우측 모두 그려야 된다.
	// 치수기입은 현재 입력중인 접속슬래브만..
	BOOL bStt	= IsStt();
	BOOL bLeft	= IsLeft();

	for(long left = 0; left < 2; left++)
	{
		CApsApp *pAps	= bStt ? &pBri->m_apsStt[left] : &pBri->m_apsEnd[left];
		if(pAps->m_bIs)
		{
			DBStd.m_pDrawAps->DrawPlane(&Dom, pAps, DBStd.IsViewVectorInfo());
			DBStd.m_pDrawAps->DrawPlaneDowelBar(&Dom, pAps);
			if((left == iLEFT) == bLeft)
			{
				DBStd.m_pDrawAps->DimPlaneSide(&Dom, pAps, 0, TRUE);
				DBStd.m_pDrawAps->DimPlaneTop(&Dom, pAps, 0, FALSE);
				DBStd.m_pDrawAps->DimTextPlaneDowelBar(&Dom, pAps);
			}
			BOOL bBrkLen = bStt ? pBri->m_bBracketLengthStt : pBri->m_bBracketLengthEnd;
			if(bBrkLen)
			{
				m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
				double dLenL = bStt? pBri->m_dBrkLenStt[iLEFT] : pBri->m_dBrkLenEnd[iLEFT];
				double dLenR = bStt? pBri->m_dBrkLenStt[iRIGHT] : pBri->m_dBrkLenEnd[iRIGHT];
				CTwinVector tv;
				CString szDir = bStt? "RIGHT" : "LEFT";
				tv = pAps->m_tvArrPlan.GetTvByInfo("기준면");
				Dom.SetDirection(szDir);
				if(dLenL)
				{
					long nDir = bStt? -1 : 1;
					Dom.DimMoveTo(tv.m_v1.x, tv.m_v1.y, 0);
					Dom.DimLineTo(dLenL*nDir, 0, "BL");
				}
				if(dLenR)
				{
					long nDir = bStt? 1 : -1;
					Dom.DimMoveTo(tv.m_v2.x, tv.m_v2.y, 0);
					Dom.DimLineTo(dLenR*nDir, 0, "BR");
				}
			}

			// 완충슬래브 평면도
			for(long cns = 0; cns < MAX_CNS; cns++)
			{
				CApsApp *pCns	= bStt ? &pBri->m_cnsStt[left][cns] : &pBri->m_cnsEnd[left][cns];
				if(pCns)
				{
					DBStd.m_pDrawAps->DrawPlane(&Dom, pCns);
					if((left == iLEFT) == bLeft)
					{
						DBStd.m_pDrawAps->DimPlaneTop(&Dom, pCns, 0, TRUE);
						if(cns == 0)
							DBStd.m_pDrawAps->DimPlaneSide(&Dom, pCns, 0, TRUE);
					}
				}
			}
		}
	}

	// 날개벽 평면
	m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	pBri->GetTvWingWallPlane(bStt, TRUE, TRUE);
	DBStd.m_pDrawWingWall->DrawGenPlane(&Dom, bStt);

	// 수평으로 회전
	long nJijum = bStt ? 0 : pBri->m_nQtyJigan;
	double dStaJijum = pBri->GetStationOnJijum(nJijum);
	CDPoint vAngLine = pBri->GetLineBase()->GetAngleAzimuth(dStaJijum);
	CDPoint xyJijum = pBri->GetXyOnJijum(nJijum);
 	Dom.Rotate(xyJijum, vAngLine.GetMirrorVert());


	CDRect rectPlane;
	Dom.SetCalcExtRect();
	rectPlane = Dom.GetExtRect();
	*pDom << Dom;

	// 단면도
	// 단면은 현재 선택된 곳만 보여줌... 일단..
	pAps->GetTvSide();
	DBStd.m_pDrawAps->DrawSection(&Dom, pAps, &m_tvArrApsSection);
	DBStd.m_pDrawAps->DimSectionSide(&Dom, pAps, 0, TRUE);
	DBStd.m_pDrawAps->DimSectionBottom(&Dom, pAps, 0, TRUE);
	DBStd.m_pDrawAps->DimSectionTop(&Dom, pAps, 0);
	DBStd.m_pDrawAps->DimTextSectionSlope(&Dom, pAps);

	// 교량 브라켓 부분을 컷팅해서 그려줌
	CTwinVectorArray tvArrTot;
	CDRect rect;
	
	if(bStt)
	{
		pBri->m_tvArrVertSection.GetTvArrByInfoRange("시점브라켓헌치", "시점브라켓상부측면", tvArrTot);
		rect = tvArrTot.GetRect();
		rect.left -= 50;
		rect.top += 1000;
		rect.bottom -= 1000;
		rect.right += 2000;
	}
	else
	{
		pBri->m_tvArrVertSection.GetTvArrByInfoRange("종점브라켓상부측면", "종점브라켓헌치", tvArrTot);
		rect = tvArrTot.GetRect();
		rect.left -= 2000;
		rect.top += 1000;
		rect.bottom -= 1000;
		rect.right += 50;
	}
	
	tvArrTot = pBri->m_tvArrVertSection;

	DBStd.DrawCuttingTvArray(&Dom, tvArrTot, rect, TRUE);
	CDRect rectSection;
	Dom.SetCalcExtRect();
	rectSection = Dom.GetExtRect();

	CDPoint xyMove;
	xyMove.x = rectPlane.right - rectSection.left + 500;
	xyMove.y = (rectPlane.top+rectPlane.bottom)/2 - (rectSection.top+rectSection.bottom)/2;
	Dom.Move(xyMove);
	*pDom << Dom;

	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE);
	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

BOOL CInputGenAps::IsValid()
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

	// 교량의 형식 중에 문형 라멘교나 박스교가 없을 경우 접속 슬래브 배근 입력 메뉴를 표시하지 않음
	if(nSizeBri==nCountBri1)
		return FALSE;
	// 교량의 형식이 모두 슬래브교일 경우 기둥 위치 입력 이후의 메뉴를 표시하지 않음
	if(nSizeBri==nCountBri2)
		return FALSE;

	return TRUE;
}

void CInputGenAps::SetDataInit()
{
	if(!IsAps())
	{
		m_pIn->GridMessage(&m_Grid, "접속슬래브가 존재 하지 않습니다");
		return;
	}

	m_Grid.UpdateData(FALSE);
}

void CInputGenAps::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	structGridCell *pCell = m_Grid.GetCurrentCell();
	if(pCell)
	{
		m_pIn->SetComment(pCell->sTitle);
		
		CString str = m_Grid.GetItemText(0, pCell->nCol);
		if(str=="CTC")
			str.Format("C.T.C %.0f L=600", m_Grid.GetTextMatrixDouble(pCell->nRow, pCell->nCol));
		if(str=="S(%)")
			str.Format("S=%.2f%%", m_Grid.GetTextMatrixDouble(pCell->nRow, pCell->nCol));
		m_pView->GetDomyun()->STMakeCursor(str);
	}
}

void CInputGenAps::SetControl()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	CARcBridgeDataStd *pStd	= pBri->m_pARcBridgeDataStd;

	m_checkStt.SetCheck(pBri->m_apsStt[iLEFT].m_bIs);
	m_checkEnd.SetCheck(pBri->m_apsEnd[iLEFT].m_bIs);

	// 접속슬래브가 하나도 없는 경우 모든 컨트롤 disable
	m_checkWing.EnableWindow(IsAps());
	m_checkOrtho.EnableWindow(IsAps());
	m_comboCns.EnableWindow(IsAps());
	m_checkDivision.EnableWindow(IsAps());
	m_checkBracket.EnableWindow(IsAps());

	if(pStd->IsLsdDesign())
	{
		GetDlgItem(IDC_EDIT_FCK)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FY)->EnableWindow(FALSE);
	}

	// 접속슬래브 콘크리트 강도
	CApsApp *pAps = GetAps();
	if(pAps)
	{
		CString szKgf_cm2_CGS   = "kgf/cm²";
		CString szKgf_cm2_Apply = pStd->GetStringUnitType(szKgf_cm2_CGS);
		GetDlgItem(IDC_STATIC_UNIT_FCK)->SetWindowText(szKgf_cm2_Apply);
		GetDlgItem(IDC_STATIC_UNIT_FY)->SetWindowText(szKgf_cm2_Apply);
		m_sFck.Format("%.1f", pStd->GetValueUnitType(pAps->m_dFck, szKgf_cm2_CGS, szKgf_cm2_Apply));
		m_sFy.Format("%.1f", pStd->GetValueUnitType(pAps->m_dFy, szKgf_cm2_CGS, szKgf_cm2_Apply));

		m_checkWing.SetCheck(pAps->m_bApplyWingAng);
		m_checkOrtho.SetCheck(pAps->m_bOrthoSlabEnd);
		m_comboCns.SetCurSel(GetCountCns());

		// 횡방향 분리 옵션 ////
		BOOL bStt	= (m_nTab == m_nTabStt[iLEFT]) || (m_nTab == m_nTabStt[iRIGHT]) ? TRUE : FALSE;
		m_checkDivision.SetCheck(bStt ? pBri->m_bDivisionApsStt : pBri->m_bDivisionApsEnd);
		m_checkBracket.SetCheck(bStt ? pBri->m_bBracketLengthStt : pBri->m_bBracketLengthEnd);
		if(m_nTab == m_nTabStt[iLEFT] || m_nTab == m_nTabEnd[iLEFT])
		{
			m_checkDivision.EnableWindow(TRUE);
			m_checkBracket.EnableWindow(TRUE);
		}
		else
		{
			m_checkDivision.EnableWindow(FALSE);
			m_checkBracket.EnableWindow(FALSE);
		}
		// 횡방향 분리 옵션 ////
	}

	UpdateData(FALSE);

}

void CInputGenAps::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	CCellID cell = m_Grid.GetFocusCell();

	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);
}


void CInputGenAps::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	// view에서 온 메시지라면.. 단면도 체크를 함.
	if(m_pIn->m_strME.bViewMouseEvent)
	{
		CDPoint xy = m_pIn->m_strME.xy;

		m_pIn->m_strME.bViewMouseEvent = FALSE;
	}
	
	
	CInputDialogBase::OnLButtonDblClk(nFlags, point);
}

void CInputGenAps::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_pIn->m_strME.bViewMouseEvent)
	{
		// 접속슬래브 체크
		m_tvArrApsSection.OnLButtonDown(nFlags, m_pIn->m_strME.xy);
	}


	CInputDialogBase::OnLButtonDown(nFlags, point);
}

void CInputGenAps::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if(m_pIn->m_strME.bViewMouseEvent)
	{

		// 접속슬래브 단면 체크
		m_bMove = m_tvArrApsSection.IsSelect();
		m_tvArrApsSection.OnLButtonUp(nFlags, m_pIn->m_strME.xy);

		if(m_bMove)
		{
			m_bMove = FALSE;
			DrawInputDomyunView(FALSE);
		}
	}

	CInputDialogBase::OnLButtonUp(nFlags, point);
}

void CInputGenAps::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_pIn->m_strME.bViewMouseEvent) 
	{
		CDPoint xyMouse(0, 0);
		m_bMove = FALSE;

		if(m_tvArrApsSection.OnMouseMove(nFlags, m_pIn->m_strME.xy))
		{
			m_bMove = TRUE;
		}

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



BOOL CInputGenAps::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	return CInputDialogBase::DestroyWindow();
}

// 현재 작업중인 접속슬래브 리턴 
CApsApp* CInputGenAps::GetAps()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(m_nTab < 0) return NULL;
	BOOL bStt	= IsStt();
	long left	= IsLeft() ? iLEFT : iRIGHT;


	if(bStt)
	{
		return &pBri->m_apsStt[left];
	}

	if(!bStt)
	{
		return &pBri->m_apsEnd[left];
	}

	return NULL;
}

void CInputGenAps::OnCheckOrtho() 
{
	CApsApp *pAps = GetAps();
	if(pAps)
	{
		pAps->m_bOrthoSlabEnd = m_checkOrtho.GetCheck();

		if(pAps->m_bOrthoSlabEnd)
		{
			pAps->m_bApplySabogangRebar = FALSE;
			pAps->m_nCountSabogangRebarUpper[iLEFT] = 0;
			pAps->m_nCountSabogangRebarUpper[iRIGHT] = 0;
			pAps->m_nCountSabogangRebarLower[iLEFT] = 0;
			pAps->m_nCountSabogangRebarLower[iRIGHT] = 0;
		}

		DrawInputDomyunView(FALSE);
	}
}

void CInputGenAps::OnCheckWing() 
{
	// TODO: Add your control notification handler code here
	CApsApp *pAps = GetAps();
	if(pAps)
	{
		pAps->m_bApplyWingAng = m_checkWing.GetCheck();
		DrawInputDomyunView(FALSE);
	}
}

// 콘크리트 권고안
void CInputGenAps::OnButtonDefaultInput2() 
{
	// TODO: Add your control notification handler code here
	if(AfxMessageBox("현재 입력 상자의 데이터가 모두 기본값으로 바뀝니다. 계속 하시겠습니까?  ", MB_YESNO|MB_ICONQUESTION)==IDNO)
		return;

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(pBri)
	{
		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft	= (left == iLEFT);
			if(m_nTab == m_nTabStt[left]) pBri->SetDataDefaultShapeAps(TRUE, bLeft, FALSE);
			if(m_nTab == m_nTabEnd[left]) pBri->SetDataDefaultShapeAps(FALSE, bLeft, FALSE);
			SetGridData();
			SetDataInit();
			DrawInputDomyunView(TRUE);
		}
	}
}

void CInputGenAps::OnSelchangeComboArea() 
{
	// TODO: Add your control notification handler code here

	DrawInputDomyunView(FALSE);	
}

// 현재 작업중의 완추슬래브
CApsApp* CInputGenAps::GetCns(long nIdx)
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(m_nTab < 0) return NULL;
	if(nIdx < 0 || nIdx > 1) return NULL;
	BOOL bStt	= IsStt();
	long left	= IsLeft() ? iLEFT : iRIGHT;

	if(bStt)
	{
		return &pBri->m_cnsStt[left][nIdx];
	}

	if(!bStt)
	{
		return &pBri->m_cnsEnd[left][nIdx];
	}

	return NULL;
}

// 완충슬래브 개수
long CInputGenAps::GetCountCns()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(m_nTab < 0) return 0;
	BOOL bStt	= IsStt();
	long left	= IsLeft() ? iLEFT : iRIGHT;

	if(bStt)
	{
		return pBri->m_nCountCnsStt[left];
	}

	if(!bStt)
	{
		return pBri->m_nCountCnsEnd[left];
	}

	return 0;
}

void CInputGenAps::OnSelchangeComboCns() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	if(m_nTab < 0) return;
	BOOL bStt	= IsStt();
	BOOL left	= IsLeft() ? iLEFT : iRIGHT;

	if(bStt)
	{
		pBri->m_nCountCnsStt[left]	= m_comboCns.GetCurSel();	
	}
	else
	{
		pBri->m_nCountCnsEnd[left]	= m_comboCns.GetCurSel();
	}

	SetGridData();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

// 좌측접속슬래브에서만 횡방향 분리를 설정할 수 있다.
void CInputGenAps::OnCheckDivision() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	if(m_nTab < 0) return;
	long stt	= m_nTab == m_nTabStt[iLEFT] ? iSTT : -1;
	if(stt < 0)
		stt		= m_nTab == m_nTabEnd[iLEFT] ? iEND : -1;

	if(stt > -1)
	{
		if(stt == iSTT)
			pBri->m_bDivisionApsStt	= m_checkDivision.GetCheck();
		else 
			pBri->m_bDivisionApsEnd	= m_checkDivision.GetCheck();

		//pBri->SyncBridge(0, FALSE, TRUE, TRUE);
		pBri->SyncBridgeAps(0, TRUE);

		InitTabCtrl();
		SetGridData();
		SetDataInit();

		DrawInputDomyunView(FALSE);
	}
}

BOOL CInputGenAps::IsStt()
{
	return (m_nTab == m_nTabStt[iLEFT]) || (m_nTab == m_nTabStt[iRIGHT]) ? TRUE : FALSE;
}

BOOL CInputGenAps::IsLeft()
{
	return (m_nTab == m_nTabStt[iLEFT]) || (m_nTab == m_nTabEnd[iLEFT]) ? TRUE : FALSE;
}

BOOL CInputGenAps::IsAps()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return FALSE;
	
	if(!pBri->IsAps(TRUE, TRUE) && !pBri->IsAps(TRUE, FALSE) && !pBri->IsAps(FALSE, TRUE) && !pBri->IsAps(FALSE, FALSE))
		return FALSE;

	return TRUE;
}

void CInputGenAps::OnCheckBracket() 
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	if(m_nTab < 0) return;
	long stt	= m_nTab == m_nTabStt[iLEFT] ? iSTT : -1;
	if(stt < 0)
		stt		= m_nTab == m_nTabEnd[iLEFT] ? iEND : -1;

	if(stt > -1)
	{
		if(stt == iSTT)
			pBri->m_bBracketLengthStt	= m_checkBracket.GetCheck();
		else 
			pBri->m_bBracketLengthEnd	= m_checkBracket.GetCheck();

		//pBri->SyncBridge(0, FALSE, TRUE, TRUE);
		pBri->SyncBridgeAps(0, TRUE);

		InitTabCtrl();
		SetGridData();
		SetDataInit();

		DrawInputDomyunView(FALSE);
	}
}

void CInputGenAps::OnCheckEnd() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	if(!pBri->IsBracket(FALSE))
	{
		AfxMessageBox("브라켓이 없는 경우 접속슬래브를 설치할 수 없습니다");
		m_checkEnd.SetCheck(FALSE);
	}

	pBri->m_apsEnd[iLEFT].m_bIs	= m_checkEnd.GetCheck();
	if(!pBri->m_apsEnd[iLEFT].m_bIs)
		pBri->m_apsEnd[iRIGHT].m_bIs	= FALSE;

	pBri->ModifyBridgeType(TRUE);
	InitTabCtrl();
	SetGridData();
	SetDataInit();
	SetControl();

	DrawInputDomyunView(FALSE);
}

void CInputGenAps::OnCheckStt() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	if(!pBri->IsBracket(TRUE))
	{
		AfxMessageBox("브라켓이 없는 경우 접속슬래브를 설치할 수 없습니다");
		m_checkStt.SetCheck(FALSE);
	}

	pBri->m_apsStt[iLEFT].m_bIs	= m_checkStt.GetCheck();
	if(!pBri->m_apsStt[iLEFT].m_bIs)
	{
		pBri->m_apsStt[iRIGHT].m_bIs	= FALSE;	
	}

	pBri->ModifyBridgeType(TRUE);
	InitTabCtrl();
	SetGridData();
	SetDataInit();
	SetControl();

	DrawInputDomyunView(FALSE);
}

void CInputGenAps::OnChangeEditFck() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CInputDialogBase::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

}

void CInputGenAps::OnChangeEditFy() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CInputDialogBase::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

}

void CInputGenAps::OnKillfocusEditFck() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	CARcBridgeDataStd *pStd	= pBri->m_pARcBridgeDataStd;
	CApsApp *pAps	= GetAps();
	if(pAps)
	{

		CString szKgf_cm2_CGS   = "kgf/cm²";
		CString szKgf_cm2_Apply = pStd->GetStringUnitType(szKgf_cm2_CGS);
		double dFck	= pStd->GetValueUnitType(atof(m_sFck), szKgf_cm2_Apply, szKgf_cm2_CGS);
		pAps->m_dFck	= dFck;

		SyncFckFy();
	}	
}

void CInputGenAps::OnKillfocusEditFy() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	CARcBridgeDataStd *pStd	= pBri->m_pARcBridgeDataStd;
	CApsApp *pAps	= GetAps();
	if(pAps)
	{

		CString szKgf_cm2_CGS   = "kgf/cm²";
		CString szKgf_cm2_Apply = pStd->GetStringUnitType(szKgf_cm2_CGS);
		double dFy	= pStd->GetValueUnitType(atof(m_sFy), szKgf_cm2_Apply, szKgf_cm2_CGS);
		pAps->m_dFy	= dFy;

		SyncFckFy();
	}		
}

void CInputGenAps::OnButtonExamine() 
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	if(m_pDlgCheck->GetSafeHwnd() && m_pDlgCheck->IsWindowVisible()) m_pDlgCheck->DestroyWindow();
	m_pDlgCheck->SetBriAndContent(pBri,IDD_GEN_APS,m_nTab);
	m_pDlgCheck->ShowAndCheck(this);	

	CString str = _T("");
	str.Format("접속슬래브 검토");
	m_pDlgCheck->SetWindowText(str);	
}

void CInputGenAps::SyncFckFy()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	CApsApp *pAps	= GetAps();
	if(pAps)
	{
		double dFck	= pAps->m_dFck;
		double dFy	= pAps->m_dFy;

		if(m_nTab == m_nTabStt[iLEFT])
		{
			pBri->m_cnsStt[iLEFT][0].m_dFck		= dFck;
			pBri->m_cnsStt[iLEFT][1].m_dFck		= dFck;

			pBri->m_cnsStt[iLEFT][0].m_dFy		= dFy;
			pBri->m_cnsStt[iLEFT][1].m_dFy		= dFy;
		}
		else if(m_nTab == m_nTabStt[iRIGHT])
		{
			pBri->m_cnsStt[iRIGHT][0].m_dFck	= dFck;
			pBri->m_cnsStt[iRIGHT][1].m_dFck	= dFck;

			pBri->m_cnsStt[iRIGHT][0].m_dFy	= dFy;
			pBri->m_cnsStt[iRIGHT][1].m_dFy	= dFy;
		}
		else if(m_nTab == m_nTabEnd[iLEFT])
		{
			pBri->m_cnsEnd[iLEFT][0].m_dFck		= dFck;
			pBri->m_cnsEnd[iLEFT][1].m_dFck		= dFck;

			pBri->m_cnsEnd[iLEFT][0].m_dFy		= dFy;
			pBri->m_cnsEnd[iLEFT][1].m_dFy		= dFy;
		}
		else if(m_nTab == m_nTabEnd[iRIGHT])
		{
			pBri->m_cnsEnd[iRIGHT][0].m_dFck	= dFck;
			pBri->m_cnsEnd[iRIGHT][1].m_dFck	= dFck;

			pBri->m_cnsEnd[iRIGHT][0].m_dFy	= dFy;
			pBri->m_cnsEnd[iRIGHT][1].m_dFy	= dFy;
		}
	}	
}
