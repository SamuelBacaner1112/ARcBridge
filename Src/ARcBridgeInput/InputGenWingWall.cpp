// InputGenWingWall.cpp : implementation file
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
// CInputGenWingWall dialog


CInputGenWingWall::CInputGenWingWall(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputGenWingWall::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputGenWingWall)
	m_bAutoWingEl = FALSE;
	m_nRowPrev = 0;
	//}}AFX_DATA_INIT
}


void CInputGenWingWall::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputGenWingWall)
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_cbBridgeNo);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Check(pDX, IDC_CHECK_AUTOEL, m_bAutoWingEl);
	DDX_Check(pDX, IDC_CHECK_AUTO_HEIGHT_SOIL, m_bAutoHeightSoil);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputGenWingWall, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputGenWingWall)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_BN_CLICKED(IDC_CHECK_AUTOEL, OnCheckAutoEl)
	ON_BN_CLICKED(IDC_CHECK_AUTO_HEIGHT_SOIL, OnCheckAutoHeightSoil)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID, OnCellChangedRow)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_BN_CLICKED(IDC_BUTTON_CALC_SW, OnButtonCalcSw)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputGenWingWall message handlers

BOOL CInputGenWingWall::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		UpdateData();
		if(m_Grid.GetFocusCell().col == m_Grid.GetCols()-1)
			pMsg->wParam = VK_RIGHT;
		else
			pMsg->wParam = VK_TAB;
	}

	if(pMsg->hwnd==m_Grid.GetSafeHwnd() && pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK)
	{
		CCellID next = m_Grid.GetFocusCell();
		if(next.col==1)
		{
			CStringArray strArr;
			strArr.Add(_T("직각 처리"));
			strArr.Add(_T("사각 처리"));
			m_Grid.SetComboString(strArr);
		}
		if(next.col==2)
		{
			CStringArray strArr;
			strArr.Add(_T("변화 없음"));
			strArr.Add(_T("세로 방향"));
			strArr.Add(_T("가로 방향"));
			m_Grid.SetComboString(strArr);
		}
	}

	return CInputDialogBase::PreTranslateMessage(pMsg);
}

void CInputGenWingWall::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	CCellID cell = m_Grid.GetFocusCell();
	CString sTitle	= m_Grid.GetTextMatrix(0, cell.col);

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);

	int nTab = m_Tab.GetCurSel();
	BOOL bStt = nTab==0 ? TRUE : FALSE;
	BOOL bLeft = cell.row == 1 ? TRUE : FALSE;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(sTitle.Find(_T("단부 처리")) != -1)
	{
		if(pWing->m_nDirHunch == 2 &&  (pWing->m_dLT+(pWing->m_dT2 - pWing->m_dT1)) > pWing->m_dL1)
		{
			AfxMessageBox(_T("가로 방향의 단면 변화부는 \"L1\" 구간에만 설치가 가능합니다. 입력값을 다시 설정해 주십시오.  "));
			SetDataInit();
		}
	}
	SetDataSave();

	if(sTitle.Find(_T("H1")) != -1)
	{
		double dMaxH = pWing->GetEndEl() - pBri->GetXySectionWingWallStt(bStt, FALSE).y;
		if((pWing->m_dHL) > dMaxH)
		{
			AfxMessageBox(_T("날개벽 배근은 단부EL를 기준으로 기초하단까지 지원됩니다."));
			pWing->m_dHL = dMaxH;
		}
	}
	if(sTitle.Find(_T("단부 처리")) != -1)
	{
		if(pWing->m_bRAngEnd)
		{
			pWing->m_dL2 = pBri->GetLengthFootingWingWall(bStt, bLeft, FALSE, FALSE);
		}
	}

	if(m_bAutoWingEl && (sTitle==_T("L1") || sTitle==_T("L2")))
	{
		double dEL = pBri->GetElWingWallEnd(bStt, bLeft);
		pWing->m_dEL = Round(dEL, 0);
	}
	if(sTitle==_T("SW수평"))
	{
		if(pWing->m_bSWHor == FALSE) 
			pWing->m_dSW = 1.0;
	}
	
	if(pWing->m_nDirHunch == 0)
	{
		pWing->m_dT2 = pWing->m_dT1;
		pWing->m_dLT = 0.0;
	}

	if(sTitle==_T("단면 변화") || sTitle==_T("L1") || sTitle==_T("L2") || sTitle==_T("SW수평"))
	{
		SetGridData();
	}
	SetDataInit();

	// 철근 재배치
	SetRebarWingWall();
	m_Grid.SetFocusCell(cell.row, cell.col);

	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);
}

void CInputGenWingWall::OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;

	int nRow = pGridView->iRow;

	if(m_nRowPrev != 0 && nRow != m_nRowPrev) DrawInputDomyunView();
	m_nRowPrev = nRow;
}

void CInputGenWingWall::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;

	int nCol = pGridView->iColumn;

	CString str = m_Grid.GetTextMatrix(0, nCol);
	if(str==_T("SW") || str==_T("SH"))
		str = _T("1:") + str;
	if(str.Left(3)==_T("HuW") || str.Left(3)==_T("HuH"))
		str.Format(_T("HuW%s X HuH%s"), str.Right(1), str.Right(1));
	m_pView->GetDomyun()->STMakeCursor(str);
}

void CInputGenWingWall::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SetFocus();

	SetGridData();
	SetDataInit();

	DrawInputDomyunView();

	return;
}

void CInputGenWingWall::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SetDataSave();

	return;
}

void CInputGenWingWall::OnPreInitDialog()
{

	SetResize(IDC_CHECK_AUTOEL,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_AUTO_HEIGHT_SOIL,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_TAB,	SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID,	SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_CALC_SW,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetHyperHelp(m_pIn->GetManualPath());
	m_nRowPrev = 0;

	InitComboCtrl();
	InitTabCtrl();
	SetEnableWindow();
	m_Grid.InitGrid(m_pIn->m_pARcBridgeDataStd->m_Fy);
	m_Grid.EnableReturnPass(FALSE, FALSE);
	SetGridData();
}

void CInputGenWingWall::InitComboCtrl()
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

void CInputGenWingWall::InitTabCtrl()
{
	m_Tab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_Tab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	m_Tab.RemoveAllTabs();

	m_Tab.InsertItem(0, _T("시점측 날개벽"));
	m_Tab.InsertItem(1, _T("종점측 날개벽"));
}

void CInputGenWingWall::OnSelchangeComboBridgeNo() 
{
	m_pIn->m_nCurrentBridge = m_cbBridgeNo.GetCurSel();

	SetEnableWindow();

	SetGridData();
	SetDataInit();
	SetPaneTitle();

	DrawInputDomyunView();
	m_cbBridgeNo.SetFocus();
}

void CInputGenWingWall::DrawInputDomyunView(BOOL bZoomAll)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(75);

	int nTab = m_Tab.GetCurSel();
	BOOL bStt = nTab==0;

	int nRow = m_Grid.GetFocusCell().row;
	if(nRow==0) nRow = 1;
	BOOL bLeft = (nRow==2)? FALSE : TRUE;

	double dSpare1 = 1000.0;
	double dSpare2 = 2000.0;
	int se = bStt ? -1 : 1;
	int lr = bLeft ? -1 : 1;
	int nLeft = bLeft ? -se : se;	// 시점일 경우 교량 진행 방향의 좌우와 날개벽의 좌우가 반대임.

	CDRect rcFront(0, 0, 0, 0);
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	pBri->GetTvVertSection_Std(nLeft, TRUE, TRUE, FALSE, 2);
	if(pWing->m_bExist)
	{		
		CHgBaseBMStd baseBM;

		// 정면도
		CDomyun DomFront(pDom);

		m_pIn->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_CONC);
		DBStd.m_pDrawWingWall->DrawGenCutBriSect(&DomFront, bStt, bLeft);
		DBStd.m_pDrawWingWall->DrawGenFront(&DomFront, bStt, bLeft);

		m_pIn->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_DIML);
		DBStd.m_pDrawWingWall->DimGenFront(&DomFront, bStt, bLeft);
		DBStd.m_pDrawWingWall->MarkGenFrontEl(&DomFront, bStt, bLeft);
		DBStd.m_pDrawWingWall->MarkGenFrontSection(&DomFront, bStt, bLeft);
		DBStd.m_pDrawWingWall->MarkGenFrontSlope(&DomFront, bStt, bLeft);

		rcFront = DomFront.GetExtRect();

		m_pIn->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_SMALL_TITLE);
		DomFront.SetTextAlignHorz(TA_CENTER);
		DomFront.SetTextHeight(5);
		DomFront.TextOut(rcFront.CenterPoint().x, rcFront.bottom+dSpare1, _T("정 면 도"));

		*pDom << DomFront;

		// 단면도
		CDomyun DomSect(pDom);

		m_pIn->m_pARoadOptionStd->SetEnvType(&DomSect, HCAD_CONC);
		DBStd.m_pDrawWingWall->DrawGenSection(&DomSect, bStt, bLeft);

		m_pIn->m_pARoadOptionStd->SetEnvType(&DomSect, HCAD_DIML);
		DBStd.m_pDrawWingWall->DimGenSection(&DomSect, bStt, bLeft);
		DBStd.m_pDrawWingWall->MarkGenSection(&DomSect, bStt, bLeft);

		CDRect rcSect = DomSect.GetExtRect();

		m_pIn->m_pARoadOptionStd->SetEnvType(&DomSect, HCAD_SMALL_TITLE);
		DomSect.SetTextAlignHorz(TA_CENTER);
		DomSect.SetTextHeight(5);
		DomSect.TextOut(rcSect.CenterPoint().x, rcSect.bottom+dSpare1, _T("단 면 도"));

		if(se*lr>0)
			DomSect.Move((rcFront.Width()+rcSect.Width()+dSpare2)*se, 0);
		else
			DomSect.Move((rcFront.Width()+dSpare2)*se, 0);

		*pDom << DomSect;

		// 평면도
		pBri->GetTvWingWallPlane(bStt, TRUE, TRUE);

		CTwinVector tvEnd, tvOut, tvSlab;
		CString strDir = bLeft ? _T("좌측") : _T("우측");
		pBri->m_tvArrPlaneWingWall.GetTvByInfo(strDir+_T("날개벽끝단"), tvEnd);
		pBri->m_tvArrPlaneWingWall.GetTvByInfo(strDir+_T("날개벽외측"), tvOut);
		pBri->m_tvArrPlaneWingWall.GetTvByInfo(_T("슬래브끝단외측"), tvSlab);
		CDPoint vSlab = tvSlab.GetXyDir().Rotate90();
		CDPoint xyMid(0, 0), xyCut(0, 0);
		xyMid = tvEnd.GetXyMid();
		BOOL bCross = GetXyMatchSegAndLine(tvSlab.m_v1, tvSlab.m_v2, xyMid, vSlab, xyCut);
		double dCutLen = bCross ? ~(xyCut-tvOut.m_v1) : 1500;
		if(dCutLen<1500)
			dCutLen = 1500;

		CDomyun DomPlane(pDom);

		m_pIn->m_pARoadOptionStd->SetEnvType(&DomPlane, HCAD_CONC);
		DBStd.m_pDrawWingWall->DrawGenCutPlane(&DomPlane, bStt, bLeft, dCutLen, TRUE);

		m_pIn->m_pARoadOptionStd->SetEnvType(&DomPlane, HCAD_DIML);
		DBStd.m_pDrawWingWall->DimGenPlane(&DomPlane, bStt, bLeft);
		DBStd.m_pDrawWingWall->MarkGenPlane(&DomPlane, bStt, bLeft);

		CDRect rcPlane = DomPlane.GetExtRect();

		m_pIn->m_pARoadOptionStd->SetEnvType(&DomPlane, HCAD_SMALL_TITLE);
		DomPlane.SetTextAlignHorz(TA_CENTER);
		DomPlane.SetTextHeight(5);
		DomPlane.TextOut(rcPlane.CenterPoint().x, rcPlane.bottom+dSpare1, _T("평 면 도"));

		CDPoint xyMove = pBri->GetXySectionWingWallStt(bStt, TRUE);
		xyMove.y = rcFront.bottom - rcPlane.top + dSpare2;
		DomPlane.Move(xyMove);

		*pDom << DomPlane;
	}

	// 위치도
	if(pBri->m_nTypeBridge==BRIDGE_TYPE_PORTAL)
	{
		CDomyun DomLoc(pDom);

		double dScale = 250.0;
		double dRatio = 50.0/dScale;
		DomLoc.SetScaleDim(dScale);

		// 교량 평면
		m_pIn->m_pARoadOptionStd->SetEnvType(&DomLoc, HCAD_CONC);
		DBStd.m_pDrawPlane->DrawPlaneCenterLineEachSlab(&DomLoc);
		DBStd.m_pDrawPlane->DrawPlaneSlabOnlyOutLine(&DomLoc);

		// 벽체 및 기둥 평면
		int nCountInWall = pBri->GetCountInWall();
		int i=0; for(i=0; i<nCountInWall; i++)
		{
			CWallApp *pWall = pBri->GetInWall(i);
			if(pWall)
			{
				DBStd.m_pDrawWall->DrawPlan(&DomLoc, pWall, FALSE);
				if(pWall->m_bTopIsColumn)
					DBStd.m_pDrawColumn->DrawPlan(&DomLoc, &pWall->m_Column);
			}
		}

		// 날개벽 평면
		pBri->GetTvWingWallPlane(TRUE);
		DBStd.m_pDrawWingWall->DrawGenPlane(&DomLoc, TRUE);
		pBri->GetTvWingWallPlane(FALSE);
		DBStd.m_pDrawWingWall->DrawGenPlane(&DomLoc, FALSE);

		CDPoint xyC = pBri->GetXyCenterWingWallPlane(bStt, bLeft);
		double dRadius = pWing->m_bExist ? (pWing->m_dL1+pWing->m_dL2+dSpare2)/2.0 : dSpare2;
		m_pIn->m_pARoadOptionStd->SetEnvType(&DomLoc, HCAD_DIML);
		DomLoc.Circle(xyC, dRadius);

		// 교량 진행 방향으로 회전
		DBStd.RotateDomByBridgeSttEndXy(&DomLoc);

		DomLoc.SetCalcExtRect();
		CDRect rcLoc = DomLoc.GetExtRect();

		m_pIn->m_pARoadOptionStd->SetEnvType(&DomLoc, HCAD_SMALL_TITLE);
		DomLoc.SetTextAlignHorz(TA_CENTER);
		DomLoc.SetTextHeight(7.5);
		DomLoc.TextOut(rcLoc.CenterPoint().x, rcLoc.bottom+dSpare1, _T("위 치 도"));

		if(bStt)
			DomLoc.Move(-rcLoc.left, -rcLoc.top);
		else
			DomLoc.Move(-rcLoc.right, -rcLoc.top);
		DomLoc.RedrawByScale(dRatio);

		if(bStt)
			DomLoc.Move(rcFront.right+dSpare1, rcFront.CenterPoint().y);
		else
			DomLoc.Move(rcFront.left-dSpare1, rcFront.CenterPoint().y);

		*pDom << DomLoc;
	}

	pBri->GetTvVertSection_Std(nLeft, TRUE, TRUE, TRUE);
	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

BOOL CInputGenWingWall::IsValid()
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

	// 교량의 형식 중에 문형 라멘교나 박스교가 없을 경우 날개벽 제원 입력 메뉴를 표시하지 않음
	if(nSizeBri==nCountBri1)
		return FALSE;
	// 교량의 형식이 모두 슬래브교일 경우 기둥 위치 입력 이후의 메뉴를 표시하지 않음
	if(nSizeBri==nCountBri2)
		return FALSE;

	return TRUE;
}

void CInputGenWingWall::SetDataDefault()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	for(long bri = 0; bri < pStd->GetBridgeSize(); bri++)
	{
		CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(bri);
		if(!pBri) continue;

		BOOL bFirst   = IsFirst(bri);
		pBri->SetDefaultedState(GetIDD(), 0x00000001);
		if(!bFirst && !IsClickDefaultButton(bri)) continue;

		BOOL bStt = ((m_Tab.GetCurSel()==0) || bFirst) ? TRUE : FALSE;
		BOOL bEnd = ((m_Tab.GetCurSel()==1) || bFirst) ? TRUE : FALSE;
		BOOL bLeft  = ((m_Grid.GetFocusCell().row==1) || bFirst) ? TRUE : FALSE;
		BOOL bRight = ((m_Grid.GetFocusCell().row==2) || bFirst) ? TRUE : FALSE;

		if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL && pBri->m_nTypeBridge!=BRIDGE_TYPE_TRAP2)
			continue;

		CWingWall* pWing = NULL;
		if(bStt)
		{
			if(bLeft)
			{
				pWing = pBri->GetWingWall(TRUE, TRUE);
				// (ARCBRIDGE-2690) 날개벽 설치 유무 기본값은 평면제원으로
				//BOOL bExistOld	= pWing->m_bExist;
				//pWing->m_bExist = pBri->m_nSeparBri!=1;
				//if(!bExistOld && pWing->m_bExist)
				//	pWing->m_nAttachPos	= 0;

				pBri->SetDataDefaultWingWall(TRUE, TRUE, bFirst);
				pBri->m_dAddLenghForGuardWall[bStt][bLeft] = 0;			// 방호벽 추가길이
			}
			if(bRight)
			{
				pWing = pBri->GetWingWall(TRUE, FALSE);
				//pWing->m_bExist = pBri->m_nSeparBri!=2;
				pBri->SetDataDefaultWingWall(TRUE, FALSE, bFirst);
				pBri->m_dAddLenghForGuardWall[bStt][bRight] = 0;		// 방호벽 추가길이
			}
		}
		if(bEnd)
		{
			if(bLeft)
			{
				pWing = pBri->GetWingWall(FALSE, TRUE);
				//BOOL bExistOld	= pWing->m_bExist;
				//pWing->m_bExist = pBri->m_nSeparBri!=2;
				//if(!bExistOld && pWing->m_bExist)
				//	pWing->m_nAttachPos	= 0;
				pBri->SetDataDefaultWingWall(FALSE, TRUE, bFirst);
				pBri->m_dAddLenghForGuardWall[bEnd][bLeft] = 0;			// 방호벽 추가길이
			}
			if(bRight)
			{
				pWing = pBri->GetWingWall(FALSE, FALSE);
				//BOOL bExistOld	= pWing->m_bExist;
				//pWing->m_bExist = pBri->m_nSeparBri!=1;
				//if(!bExistOld && pWing->m_bExist)
				//	pWing->m_nAttachPos	= 0;
				pBri->SetDataDefaultWingWall(FALSE, FALSE, bFirst);
				pBri->m_dAddLenghForGuardWall[bEnd][bRight] = 0;		// 방호벽 추가길이
			}
		}

		pBri->m_bAutoWingEl = TRUE;
	}

	m_pIn->m_pDoc->SetModifiedFlag();
}

void CInputGenWingWall::SetGridTitle()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	BOOL bBox = (pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)? TRUE : FALSE;
	BOOL bUseH2 = FALSE;
	// 부상방지판이 없는경우에만 H2를 입력할 수 있다.
	if(bBox && !pBri->m_bExistProtectionRiseFooting)
	{
		bUseH2 = TRUE;
	}

	m_Grid.SetGridDefault(2, 2, 1, 1);

	long row		= 0, col = 0;
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	m_Grid.AddCell(row, col++, _T("구분"));
	m_Grid.AddCell(row, col++, _T("단부 처리"));
	m_Grid.AddCell(row, col++, _T("단면 변화"));
	m_Grid.AddCell(row, col++, _T("ELE"));
	m_Grid.AddCell(row, col++, _T("L1"));
	m_Grid.AddCell(row, col++, _T("L2"));
	m_Grid.AddCell(row, col++, _T("H1"));
	m_Grid.AddCell(row, col++, _T("HP"));
	m_Grid.AddCell(row, col++, _T("H2"), nFormat, bUseH2? nLine : CELL_READONLY);
	m_Grid.AddCell(row, col++, _T("SW"));
	m_Grid.AddCell(row, col++, _T("SW수평"));
	m_Grid.AddCell(row, col++, _T("T1"));
	m_Grid.AddCell(row, col++, _T("T2"));
	m_Grid.AddCell(row, col++, _T("TL"));
	m_Grid.AddCell(row, col++, _T("SH"));
	m_Grid.AddCell(row, col++, _T("HuW1"));
	m_Grid.AddCell(row, col++, _T("HuH1"));
	m_Grid.AddCell(row, col++, _T("HuW2"));
	m_Grid.AddCell(row, col++, _T("HuH2"));
	m_Grid.AddCell(row, col++, _T("횡단구간"));
	m_Grid.AddCell(row, col++, _T("방호벽 추가길이(m)"));
	m_Grid.AddCell(row, col++, _T("적용 토피고"));

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(1, 100);
	m_Grid.SetColumnWidth(2, 80);
	m_Grid.SetColumnWidth(9, 80);
	m_Grid.SetColumnWidth(10, 60);
	m_Grid.SetColumnWidth(19, 60);
	m_Grid.SetColumnWidth(20, 140);
	m_Grid.SetColumnWidth(21, 80);
}

void CInputGenWingWall::SetGridData()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL && pBri->m_nTypeBridge!=BRIDGE_TYPE_TRAP2 && pBri->m_nTypeBridge!=BRIDGE_TYPE_ARCH && pBri->m_nTypeBridge!=BRIDGE_TYPE_BOX)
	{
		m_pIn->GridMessage(&m_Grid, "날개벽을 설치 할 수 없습니다.");
		m_Grid.SetRedraw(TRUE,TRUE);
		return;
	}

	m_bAutoWingEl = pBri->m_bAutoWingEl;
	UpdateData(FALSE);

	SetGridTitle();

	CWingWall* pWing = NULL;

	CString str = _T("");
	CStringArray strArr;
	int nTab = m_Tab.GetCurSel();
	int nCols = m_Grid.GetCols();
	BOOL bStt = nTab==0 ? TRUE : FALSE;
	BOOL bLeft = TRUE;

	BOOL bBox = (pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)? TRUE : FALSE;
	BOOL bUseH2 = FALSE;
	// 부상방지판이 없으면 L2를 입력 할수 없고 H2를 무조건 입력 할수 있다.
	if(bBox && !pBri->m_bExistProtectionRiseFooting)
	{
		bUseH2 = TRUE;
	}

	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;
	long row		= 1, col = 0;
	for(long ix=0; ix<2; ++ix)
	{
		bLeft = ix==0 ? TRUE : FALSE;
		pWing = pBri->GetWingWall(bStt, bLeft);

		BOOL bUseL2 = pWing->m_nAttachPos == 1 ? FALSE : TRUE;
		if(bUseH2 || !bUseL2)
		{
			pWing->m_dL2 = 0.0; 
		}

		nLine = (pWing->m_bExist)? CELL_LINE_NO : CELL_READONLY;

		str = bLeft? _T("좌측") : _T("우측");
		m_Grid.AddCell(row, col++, str);
		m_Grid.AddCell(row, col++, &pWing->m_bRAngEnd, nFormat, nLine, pWing->m_bRAngEnd? _T("직각 처리") : _T("사각 처리"), CELL_TYPE_CHECK);
		strArr.Add(_T("변화 없음"));
		strArr.Add(_T("세로 방향"));
		strArr.Add(_T("가로 방향"));
		m_Grid.AddCell(row, col++, &pWing->m_nDirHunch, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, strArr);
	
		m_Grid.AddCell(row, col++, &pWing->m_dEL, nFormat, m_bAutoWingEl? CELL_READONLY : nLine, _T(""), CELL_TYPE_METER); m_Grid.SetCellDec(CELL_DEC_3);
		m_Grid.AddCell(row, col++, &pWing->m_dL1, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_1);
		m_Grid.AddCell(row, col++, &pWing->m_dL2, nFormat, (bUseH2 || !bUseL2)? CELL_READONLY : nLine); m_Grid.SetCellDec(CELL_DEC_0);
		m_Grid.AddCell(row, col++, &pWing->m_dHL, nFormat, (pWing->m_dL1 == 0)? CELL_READONLY : nLine); m_Grid.SetCellDec(CELL_DEC_0);
		m_Grid.AddCell(row, col++, &pWing->m_dH3, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
		if(pWing->m_bSWHor)  m_Grid.AddCell(row, col++, _T("-"), nFormat, CELL_READONLY);
		else m_Grid.AddCell(row, col++, &pWing->m_dH2, nFormat, bUseH2? nLine : CELL_READONLY); m_Grid.SetCellDec(CELL_DEC_0);

		if(Compare(pWing->m_dL1, 0.0, "="))
		{
			pWing->m_dSW	= 1;
			pWing->m_dHL	= 0.0;
		}
		if(pWing->m_bSWHor) m_Grid.AddCell(row, col++, _T("-"), nFormat, CELL_READONLY);
		else m_Grid.AddCell(row, col++, &pWing->m_dSW, nFormat, (pWing->m_dL1 == 0)? CELL_READONLY : nLine); m_Grid.SetCellDec(CELL_DEC_5);
		m_Grid.AddCell(row, col++, &pWing->m_bSWHor, nFormat, (pWing->m_dL1 == 0)? CELL_READONLY : nLine, _T(""), CELL_TYPE_CHECK);

		if(pWing->m_nDirHunch==0)
		{
			pWing->m_dT2 = pWing->m_dT1;
			pWing->m_dLT = 0.0;
		}
		m_Grid.AddCell(row, col++, &pWing->m_dT1, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
		m_Grid.AddCell(row, col++, &pWing->m_dT2, nFormat, (pWing->m_nDirHunch==0)? CELL_READONLY : nLine); m_Grid.SetCellDec(CELL_DEC_0);
		m_Grid.AddCell(row, col++, &pWing->m_dLT, nFormat, (pWing->m_nDirHunch==0)? CELL_READONLY : nLine); m_Grid.SetCellDec(CELL_DEC_0);
		m_Grid.AddCell(row, col++, &pWing->m_dSH, nFormat, (pWing->m_nDirHunch==0)? CELL_READONLY : nLine); m_Grid.SetCellDec(CELL_DEC_1);

		if(Compare(pWing->m_dL2, 0.0, "="))
		{
			pWing->m_dHuW1	= 0.0;
			pWing->m_dHuH1	= 0.0;
		}
		m_Grid.AddCell(row, col++, &pWing->m_dHuW1, nFormat, (pWing->m_dL2 == 0)? CELL_READONLY : nLine); m_Grid.SetCellDec(CELL_DEC_0);
		m_Grid.AddCell(row, col++, &pWing->m_dHuH1, nFormat, (pWing->m_dL2 == 0)? CELL_READONLY : nLine); m_Grid.SetCellDec(CELL_DEC_0);
		m_Grid.AddCell(row, col++, &pWing->m_dHuW2, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
		m_Grid.AddCell(row, col++, &pWing->m_dHuH2, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);

		// 횡단구간
		long nQtyHDan = pBri->GetQtyHDan();
		if(bStt)
			str.Format("L%d", bLeft ? nQtyHDan : 1);
		else
			str.Format("L%d", bLeft ? 1 : nQtyHDan);
		m_Grid.AddCell(row, col++, str, nFormat, CELL_READONLY);
		m_Grid.AddCell(row, col++, &pBri->m_dAddLenghForGuardWall[bStt][bLeft], nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_3);

		if(pBri->m_bAutoWingHeightSoil)
		{
			pWing->m_dHeightSoil = pBri->CalcUnderGroundDesignHeightForWingWall(bStt, bLeft);
		}
		m_Grid.AddCell(row, col++, &pWing->m_dHeightSoil, nFormat, pBri->m_bAutoWingHeightSoil? CELL_READONLY : nLine); m_Grid.SetCellDec(CELL_DEC_0);

		++row;
		col = 0;
	}
}

void CInputGenWingWall::SetDataInit()
{
	m_Grid.UpdateData(FALSE);
}

void CInputGenWingWall::SetDataSave()
{
	m_Grid.UpdateData(TRUE);

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);

	int nRow = m_Grid.GetFocusCell().row;
	int nTab = m_Tab.GetCurSel();
	BOOL bStt = nTab==0 ? TRUE : FALSE;
	BOOL bLeft = nRow==2 ? FALSE : TRUE;

	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);
	if(pWing)
	{
		// L1이 없을땐 sw는 무조건 1이 되어야 한다.
		if(Compare(pWing->m_dL1, 0.0, "="))
			pWing->m_dSW	= 1;

		if(pWing->m_bSWHor)
		{
			pWing->m_dSW = 999999.0;
			pWing->m_dH2 = 0;
		}
		if(pWing->m_dL1 == 0) pWing->m_bSelMainCTCD = FALSE;


		// 경사가 0이면 위쪽으로 끝없이 올라가면서 프로그램이 멈추게 된다.
		if(Compare(pWing->m_dSW, 0.0, _T("="), 0.00001))
		{
			pWing->m_dSW = 1.0;
			m_Grid.UpdateData(FALSE);
		}
	}
	
	m_pIn->m_pDoc->SetModifiedFlag();
	AfxGetMainWnd()->SendMessage(WM_USER+8, -1);
	AfxGetMainWnd()->SendMessage(WM_USER+13);
}

void CInputGenWingWall::OnCheckAutoEl() 
{
	UpdateData();

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	pBri->m_bAutoWingEl = m_bAutoWingEl;

	pBri->SetDataDefaultWingWallEl();

	SetGridData();
	SetDataInit();

	// 철근 재배치
	SetRebarWingWall();

	DrawInputDomyunView(FALSE);
}

void CInputGenWingWall::SetEnableWindow()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	if(!pBri->IsOutsideWall())
	{
		m_Tab.DeleteAllItems();
		m_Grid.DeleteAllItems();
		GetDlgItem(IDC_CHECK_AUTOEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_AUTO_HEIGHT_SOIL)->EnableWindow(FALSE);
	}
	else
	{
		InitTabCtrl();
		GetDlgItem(IDC_CHECK_AUTOEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_AUTO_HEIGHT_SOIL)->EnableWindow(TRUE);
	}

	m_bAutoHeightSoil = pBri->m_bAutoWingHeightSoil;

	// (ARCBRIDGE-3072) BOX타입이 아닐때는 L2길이 재계산 버튼으로 사용한다.
	// GetDlgItem(IDC_BUTTON_CALC_SW)->ShowWindow(pBri->IsBoxType());
	GetDlgItem(IDC_BUTTON_CALC_SW)->SetWindowText(pBri->IsBoxType()? _T("SW 재계산") : _T("L2 재계산"));
}

// 철근 재배치
void CInputGenWingWall::SetRebarWingWall()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);

	int nRow = m_Grid.GetFocusCell().row;
	int nTab = m_Tab.GetCurSel();
	BOOL bStt = nTab==0 ? TRUE : FALSE;
	BOOL bLeft = nRow==2 ? FALSE : TRUE;

	//	if((bStt && bLeft) || (!bStt && !bLeft))
	//		pBri->SyncBridge(1);
	//	else
	//		pBri->SyncBridge(-1);
	//	pBri->GetTvWingWallPlane(bStt);

	// 날개벽 제원이 바뀌더라도 철근 배근만 권고안하고 철근 다이아 및 옵션은 초기화하지 않는다.
	//	pBri->SetDataDefaultRebarWingWallOption(bStt, bLeft);

	long nDan = 0;
	pBri->SetDataDefaultRebarWingWallUpper(bStt, bLeft, 1, nDan);
	pBri->SetDataDefaultRebarWingWallUpper(bStt, bLeft, 0, nDan);
	pBri->SetDataDefaultRebarWingWallLower(bStt, bLeft, 1, nDan);
	pBri->SetDataDefaultRebarWingWallLower(bStt, bLeft, 0, nDan);
	pBri->SetDataDefaultRebarWingWallSide(bStt, bLeft, 1, nDan);
	pBri->SetDataDefaultRebarWingWallSide(bStt, bLeft, 0, nDan);

	pBri->SetDataDefaultRebarJewonWingWall(bStt, bLeft);
}

BOOL CInputGenWingWall::IsOKMoveOtherDlg()
{
	if(!m_pIn->CheckStation()) return FALSE;

	return TRUE;
}

void CInputGenWingWall::OnButtonCalcSw()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);

	int nRow = m_Grid.GetFocusCell().row;
	int nTab = m_Tab.GetCurSel();
	BOOL bStt = nTab==0 ? TRUE : FALSE;
	BOOL bLeft = nRow==2 ? FALSE : TRUE;

	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);
	if(pWing == NULL) return;
	
	// (ARCBRIDGE-3072) BOX타입 일때는 SW 재계산, 직접기초 일때는 L2길이 재계산
	if(pBri->IsBoxType())
	{
		pBri->GetTvVertSection_Std(bStt == bLeft ? 1 : -1, TRUE, TRUE, FALSE, 2);

		//BOX형에서 H2높이제한. 날개벽 꼬리하단 위로 올라가서는 안됨.
		double dL1 = m_Grid.GetTextMatrixDouble(nRow, 4);
		double dL2 = m_Grid.GetTextMatrixDouble(nRow, 5);
		double dH2 = m_Grid.GetTextMatrixDouble(nRow, 7);

		double dHL = pWing->m_dHL;
		double dElEnd = pWing->m_dEL - pWing->m_dH3;
		double dHeight = pBri->GetHeightWingWall(bStt);

		CDPoint xyStt	= pBri->GetXySectionWingWallStt(bStt, TRUE);
		CDPoint vAng	= pBri->GetAngleWingWallBetweenWall(bStt, bLeft);// 수량산출용이라면 구지 날개벽 각도를 벌린상태를 그릴 필요가 없다.
		double dW = (bStt? pBri->m_dWS : pBri->m_dWE)/vAng.y;

		double dHeightEnd = dHeight - (xyStt.y-dElEnd);
		double dHL2 = dHeightEnd - dHL - dH2;
		if(Compare(dL2, 0.0, _T("="), 0.1))
		{
			dL1 -= dW;
		}
		double dSW = RoundUp(dL1 / dHL2, 5);

		pWing->m_dSW = dSW;
	}
	else
	{
		CDPoint vAngBet = pBri->GetAngleWingWallBetweenWall(bStt, bLeft);
		double dDist1 = (pWing->m_nAttachPos==0)? MAX(0, (vAngBet.GetAngleDegree() > 90)? fabs(pWing->m_dT2/vAngBet.y*vAngBet.x) : 0) : 0;

		if(pWing->m_nAttachPos==0)
		{
			pWing->m_dL2 = pBri->GetLengthFootingWingWall(bStt, bLeft, FALSE, FALSE)-dDist1;
		}
	}

	SetDataInit();
	DrawInputDomyunView();
	return;
}

void CInputGenWingWall::OnCheckAutoHeightSoil()
{
	UpdateData();

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	pBri->m_bAutoWingHeightSoil = m_bAutoHeightSoil;

	if(m_bAutoHeightSoil)
		pBri->SetDataDefaultWingWallHeightSoil();

	SetGridData();
	SetDataInit();

	DrawInputDomyunView(FALSE);
}
