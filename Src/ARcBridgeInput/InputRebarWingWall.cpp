// InputRebarWingWall.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "../ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "RebarWingWallDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputRebarWingWall dialog


CInputRebarWingWall::CInputRebarWingWall(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputRebarWingWall::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputRebarWingWall)
	for(long iSE = iSTT; iSE <= iEND; iSE++)
	{
		for(long iLR = iLEFT; iLR <= iRIGHT; iLR++)
		{
			for(long iIO = iOUTTER; iIO <= iINNER; iIO++)
			{
				for(long nDan = 0; nDan <= 1; nDan++)
				{
					m_nCountUpper[iSE][iLR][iIO][nDan] = -1;
					m_nCountLower[iSE][iLR][iIO][nDan] = -1;
					m_nCountSide[iSE][iLR][iIO][nDan] = -1;
				}
				
			}
		}
	}
	//}}AFX_DATA_INIT
	m_nSelComboPlan = 0;
	m_pDlgCheck = new CInputCheckGrid;
}

CInputRebarWingWall::~CInputRebarWingWall()
{
	delete m_pDlgCheck;
}

void CInputRebarWingWall::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputRebarWingWall)
	DDX_Control(pDX, IDC_BUTTON_OPTION, m_btnOption);
	DDX_Control(pDX, IDC_COMBO_SELECT, m_cbSelect);
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_cbBridgeNo);
	DDX_Control(pDX, IDC_BUTTON_EXAMINE, m_btnExamine);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	DDX_Control(pDX, IDC_COMBO_PLAN, m_CBPlan);
	DDX_CBIndex(pDX, IDC_COMBO_PLAN, m_nSelComboPlan);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputRebarWingWall, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputRebarWingWall)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_CBN_SELCHANGE(IDC_COMBO_SELECT, OnSelchangeComboSelect)
	ON_CBN_SELCHANGE(IDC_COMBO_PLAN, OnSelchangeComboPlan)
	ON_BN_CLICKED(IDC_BUTTON_OPTION, OnButtonOption)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
	ON_BN_CLICKED(IDC_BUTTON_EXAMINE, OnButtonExamine)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputRebarWingWall message handlers

BOOL CInputRebarWingWall::PreTranslateMessage(MSG* pMsg) 
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

void CInputRebarWingWall::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
// 	int nCombo = m_cbSelect.GetCurSel();
// 	CString strCombo = _T(""), strStt = _T(""), strLeft = _T("");
// 	m_cbSelect.GetLBText(nCombo, strCombo);
// 	strStt = strCombo.Left(4);
//  	strLeft = strCombo.Left(9).Right(4);

	//BOOL bStt = strStt==_T("시점") ? TRUE : FALSE;
	//BOOL bLeft = strLeft==_T("좌측") ? TRUE : FALSE;

	//구간수변경시 
	structGridCell *pCell	= m_Grid.GetCurrentCell();
	if(m_Grid.GetTextMatrix(1, pCell->nCol) == hggettext("구간") || m_Grid.GetTextMatrix(1, pCell->nCol) == hggettext("\n구간\n"))
	{
		m_Grid.UpdateData(TRUE);

		CString strPos = m_Grid.GetTextMatrix(0, pCell->nCol);
		if(strPos.Left(4) == _T("상면"))
		{
			OnSelchangeComboUpperCount();
		}
		if(strPos.Left(4) == _T("하면"))
		{
			OnSelchangeComboLowerCount();
		}
		if(strPos.Left(4) == _T("측면"))
		{
			OnSelchangeComboSideCount();
		}
	}

	SetDataSave();
	SetGridData();
	SetDataInit();
	//pBri->SetDataDefaultRebarJewonWingWall(bStt, bLeft);

	DrawInputDomyunView();
	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);
}

void CInputRebarWingWall::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	int nCol = pGridView->iColumn;

	CString str = m_Grid.GetTextMatrix(0, nCol);
	m_pView->GetDomyun()->STMakeCursor(str);
}

void CInputRebarWingWall::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SetGridData();
	SetDataInit();
	DrawInputDomyunView();
	
	return;
}

void CInputRebarWingWall::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SetDataSave();
	
	return;
}

void CInputRebarWingWall::OnPreInitDialog()
{
	SetResize(IDC_BUTTON_EXAMINE,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_OPTION,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_TAB,	SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID,	SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetHyperHelp(m_pIn->GetManualPath());

	InitComboCtrl();
	InitComboSelect();
	InitTabCtrl();
}

void CInputRebarWingWall::InitComboCtrl()
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

void CInputRebarWingWall::InitComboSelect()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	m_cbSelect.ResetContent();

	CWingWall* pWing = pBri->GetWingWall(TRUE, TRUE);
	if(pWing->m_bExist)
		m_cbSelect.AddString(_T("시점 좌측 날개벽"));
	pWing = pBri->GetWingWall(TRUE, FALSE);
	if(pWing->m_bExist)
		m_cbSelect.AddString(_T("시점 우측 날개벽"));
	pWing = pBri->GetWingWall(FALSE, TRUE);
	if(pWing->m_bExist)
		m_cbSelect.AddString(_T("종점 좌측 날개벽"));
	pWing = pBri->GetWingWall(FALSE, FALSE);
	if(pWing->m_bExist)
		m_cbSelect.AddString(_T("종점 우측 날개벽"));

	m_cbSelect.SetCurSel(0);

	// 평면도 콤보박스
	m_CBPlan.ResetContent();
	m_CBPlan.AddString(hggettext("상부 평면도"));
	m_CBPlan.AddString(hggettext("하부 평면도"));

	//B,C 구간이 없는 경우는 상부 평면도만 표현
	int nCombo = m_cbSelect.GetCurSel();
	if(nCombo == -1) return;

	CString strCombo = _T("");//, strStt = _T("");//, strLeft = _T("");
	m_cbSelect.GetLBText(nCombo, strCombo);
	CString strStt = strCombo.Left(4);
	CString strLeft = strCombo.Left(9).Right(4);

	BOOL bStt = strStt==_T("시점") ? TRUE : FALSE;
	BOOL bLeft = strLeft==_T("좌측") ? TRUE : FALSE;

	pWing = pBri->GetWingWall(bStt, bLeft);
	if(!pWing->IsBlockB() || !pWing->IsBlockC())
		GetDlgItem(IDC_COMBO_PLAN)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_COMBO_PLAN)->EnableWindow(TRUE);

	m_CBPlan.SetCurSel(0);
	m_nSelComboPlan = 0;
}

void CInputRebarWingWall::InitTabCtrl()
{
	m_Tab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_Tab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	m_Tab.RemoveAllTabs();
	m_Tab.InsertItem(0, _T("전면"));
	m_Tab.InsertItem(1, _T("배면"));
}

void CInputRebarWingWall::OnSelchangeComboBridgeNo() 
{
	m_pIn->m_nCurrentBridge = m_cbBridgeNo.GetCurSel();

	SetEnableWindow();
	InitTabCtrl();
	InitComboSelect();

	//교량이 바뀔때마다 해줘야 합니다.
	for(long iSE = iSTT; iSE <= iEND; iSE++)
		for(long iLR = iLEFT; iLR <= iRIGHT; iLR++)
			for(long iIO = iOUTTER; iIO <= iINNER; iIO++)
				for(long nDan = 0; nDan <= 1; nDan++)	
					SetDataDefaultBlock(iSE, iLR, iIO, nDan);
	
	
	SetGridData();
	SetDataInit();
	SetPaneTitle();

	DrawInputDomyunView();
}

void CInputRebarWingWall::OnSelchangeComboSelect() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);

	int nCombo = m_cbSelect.GetCurSel();
	if(nCombo == -1) return;
	CString strCombo = _T("");//, strStt = _T(""), strLeft = _T("");
	m_cbSelect.GetLBText(nCombo, strCombo);
	CString strStt = strCombo.Left(4);
	CString strLeft = strCombo.Left(9).Right(4);

	BOOL bStt = strStt==_T("시점") ? TRUE : FALSE;
	BOOL bLeft = strLeft==_T("좌측") ? TRUE : FALSE;

	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);
	if(!pWing->IsBlockB() || !pWing->IsBlockC())
	{
		GetDlgItem(IDC_COMBO_PLAN)->EnableWindow(FALSE);
		m_CBPlan.SetCurSel(0);
	}
	else
		GetDlgItem(IDC_COMBO_PLAN)->EnableWindow(TRUE);

	SetGridData();
	SetDataInit();
	SetDataSave();
	DrawInputDomyunView();
}

void CInputRebarWingWall::OnSelchangeComboPlan() 
{
 	UpdateData(TRUE);
 	DrawInputDomyunView();
}
void CInputRebarWingWall::OnSelchangeComboUpperCount() 
{
	UpdateData();

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);

	int nCombo = m_cbSelect.GetCurSel();
	if(nCombo == -1) return;
	int nTab = m_Tab.GetCurSel();
	CString strCombo = _T("");//, strStt = _T(""), strLeft = _T("");
	m_cbSelect.GetLBText(nCombo, strCombo);
	CString strStt = strCombo.Left(4);
	CString strLeft = strCombo.Left(9).Right(4);

	BOOL bStt = strStt==_T("시점") ? TRUE : FALSE;
	BOOL bLeft = strLeft==_T("좌측") ? TRUE : FALSE;
	int nOut = nTab==0 ? 1 : 0;

	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	for(long nDan = 0; nDan < 2; nDan++)
	{
		int nCount = pWing->m_xyArrUpper[nOut][nDan].GetSize();
		int nBlock = m_nCountUpper[bStt][bLeft][nOut][nDan] + 1;

		if(nCount < 1) continue;
		if(nCount<nBlock)
		{
			long i=0; for(i=0; i<(nBlock-nCount); i++)
				pWing->m_xyArrUpper[nOut][nDan].Add(CDPoint(0,0));
		}
		if(nCount>nBlock)
		{
			long i=0; for(i=0; i<(nCount-nBlock); i++)
			{
				nCount = pWing->m_xyArrUpper[nOut][nDan].GetSize();
				pWing->m_xyArrUpper[nOut][nDan].RemoveAt(nCount-1);
			}
		}
	}

	SetGridData();
	SetDataInit();
	//pBri->SetDataDefaultRebarJewonWingWall(bStt, bLeft);

	DrawInputDomyunView();
}

void CInputRebarWingWall::OnSelchangeComboLowerCount() 
{
	UpdateData();

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);

	int nCombo = m_cbSelect.GetCurSel();
	if(nCombo == -1) return;
	int nTab = m_Tab.GetCurSel();
	CString strCombo = _T("");//, strStt = _T(""), strLeft = _T("");
	m_cbSelect.GetLBText(nCombo, strCombo);
	CString strStt = strCombo.Left(4);
	CString strLeft = strCombo.Left(9).Right(4);

	BOOL bStt = strStt==_T("시점") ? TRUE : FALSE;
	BOOL bLeft = strLeft==_T("좌측") ? TRUE : FALSE;
	int nOut = nTab==0 ? 1 : 0;

	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	for(long nDan = 0; nDan < 2; nDan++)
	{
		int nCount = pWing->m_xyArrLower[nOut][nDan].GetSize();
		int nBlock = m_nCountLower[bStt][bLeft][nOut][nDan] + 1;

		if(nCount < 1) continue;
		if(nCount<nBlock)
		{
			long i=0; for(i=0; i<(nBlock-nCount); i++)
				pWing->m_xyArrLower[nOut][nDan].Add(CDPoint(0,0));
		}
		if(nCount>nBlock)
		{
			long i=0; for(i=0; i<(nCount-nBlock); i++)
			{
				nCount = pWing->m_xyArrLower[nOut][nDan].GetSize();
				pWing->m_xyArrLower[nOut][nDan].RemoveAt(nCount-1);
			}
		}
	}
	

	SetGridData();
	SetDataInit();
	//pBri->SetDataDefaultRebarJewonWingWall(bStt, bLeft);

	DrawInputDomyunView();
}

void CInputRebarWingWall::OnSelchangeComboSideCount() 
{
	UpdateData();

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);

	int nCombo = m_cbSelect.GetCurSel();
	if(nCombo == -1) return;
	int nTab = m_Tab.GetCurSel();
	CString strCombo = _T("");//, strStt = _T(""), strLeft = _T("");
	m_cbSelect.GetLBText(nCombo, strCombo);
	CString strStt = strCombo.Left(4);
	CString strLeft = strCombo.Left(9).Right(4);

	BOOL bStt = strStt==_T("시점") ? TRUE : FALSE;
	BOOL bLeft = strLeft==_T("좌측") ? TRUE : FALSE;
	int nOut = nTab==0 ? 1 : 0;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);
	
	for(long nDan = 0; nDan < 2; nDan++)
	{
		int nCount = pWing->m_xyArrSide[nOut][nDan].GetSize();
		int nBlock = m_nCountSide[bStt][bLeft][nOut][nDan] + 1;

		if(nCount < 1) continue;
		if(nCount<nBlock)
		{
			long i=0; for(i=0; i<(nBlock-nCount); i++)
				pWing->m_xyArrSide[nOut][nDan].Add(CDPoint(0,0));
		}
		if(nCount>nBlock)
		{
			long i=0; for(i=0; i<(nCount-nBlock); i++)
			{
				long nCnt = pWing->m_xyArrSide[nOut][nDan].GetSize();
				pWing->m_xyArrSide[nOut][nDan].RemoveAt(nCnt-1);
			}
		}
	}
}

void CInputRebarWingWall::DrawInputDomyunView(BOOL bZoomAll)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = m_pIn->GetRcBridgeRebar();
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);
	int nCombo = m_cbSelect.GetCurSel();
	int nTab = m_Tab.GetCurSel();
	CString strCombo = _T(""), strStt = _T(""), strLeft = _T("");
	if(nCombo>=0)
		m_cbSelect.GetLBText(nCombo, strCombo);
	strStt = strCombo.Left(4);
	strLeft = strCombo.Left(9).Right(4);

	BOOL bStt = strStt==_T("시점");
	BOOL bLeft = strLeft==_T("좌측");
	int nOut = nTab==0 ? 1 : 0;
//	int nDan = 0;
	int se = bStt ? -1 : 1;
	int nLeft = bLeft ? -1*se : 1*se;	// 시점일 경우 교량 진행 방향의 좌우와 날개벽의 좌우가 반대임.

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(m_pIn->GetScaleInputDialog());

	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	BOOL bSide		= (pWing->m_nAttachPos == 0 )? FALSE : TRUE;
	CDPoint vAng = pBri->GetAngleWingWallBetweenWall(bStt, bLeft);
	double dT2 = pWing->m_dT2;
	if(bSide)
	{
//		double dAngle	= vAng.GetAngleDegree();
		double dAngJijum = pBri->GetAngleJijum(bStt? 0 : pBri->m_nQtyJigan).GetAngleDegree();
		if((bStt && bLeft) || (!bStt && !bLeft))
		{
			dAngJijum = 180 - dAngJijum;
		}
	}

	double dSpare1 = 1000.0;
	double dSpare2 = 2000.0;
	double lr = bLeft ? -1.0 : 1.0;

	CDRect rcFront(0, 0, 0, 0);
	if(pWing->m_bExist)
	{
		int nDan = 0;

		pBri->GetTvVertSection_Std(nLeft, TRUE, FALSE, FALSE, 2);
		pBri->GetTvWingWallPlane(bStt);

		// 정면도
		CDomyun DomFront(pDom);

		//.. 일반도
		m_pIn->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_CONC);
 		DBStd.m_pDrawWingWall->DrawGenCutBriSect(&DomFront, bStt, bLeft, nOut, TRUE);
 		DBStd.m_pDrawWingWall->DrawGenFront(&DomFront, bStt, bLeft, nOut);
 		DBStd.m_pDrawWingWall->DrawBlockBoundary(&DomFront, bStt, bLeft, nOut);

		//.. 구조도
		m_pIn->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_BARC);
  		DBStd.m_pDrawWingWall->DrawRebarBlockAMainFront(&DomFront, bStt, bLeft, nOut, nDan);
  		DBStd.m_pDrawWingWall->DrawRebarBlockBMainFront(&DomFront, bStt, bLeft, nOut, nDan);
   		DBStd.m_pDrawWingWall->DrawRebarBlockCMainFront(&DomFront, bStt, bLeft, nOut, nDan);
  		DBStd.m_pDrawWingWall->DrawRebarBlockDMainFront(&DomFront, bStt, bLeft, nOut, nDan);

		DBStd.m_pDrawWingWall->DrawRebarBlockASupportFront(&DomFront, bStt, bLeft, nOut, nDan);
		DBStd.m_pDrawWingWall->DrawRebarBlockDSupportFront(&DomFront, bStt, bLeft, nOut, nDan);

		DBStd.m_pDrawWingWall->DrawRebarBlockAShearFront(&DomFront, bStt, bLeft, nOut);
		DBStd.m_pDrawWingWall->DrawRebarBlockBCShearFront(&DomFront, bStt, bLeft, nOut);
		DBStd.m_pDrawWingWall->DrawRebarBlockDShearFront(&DomFront, bStt, bLeft, nOut);

		DBStd.m_pDrawWingWall->DrawRebarHunchFootingFront(&DomFront, bStt, bLeft, nOut);
		DBStd.m_pDrawWingWall->DrawRebarHunchWallFront(&DomFront, bStt, bLeft, nOut);

		m_pIn->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_DIML);
		DBStd.m_pDrawWingWall->MarkGenFrontSection(&DomFront, bStt, bLeft, FALSE);
		DBStd.m_pDrawWingWall->DimRebarFrontUpper(&DomFront, bStt, bLeft, nOut);
		DBStd.m_pDrawWingWall->DimRebarFrontLower(&DomFront, bStt, bLeft, nOut);
		DBStd.m_pDrawWingWall->DimRebarFrontSide(&DomFront, bStt, bLeft, nOut);

		DBStd.m_pDrawWingWall->MarkRebarBlockAFront(&DomFront, bStt, bLeft, nOut);
		DBStd.m_pDrawWingWall->MarkRebarBlockBCFront(&DomFront, bStt, bLeft, nOut);
		DBStd.m_pDrawWingWall->MarkRebarBlockDFront(&DomFront, bStt, bLeft, nOut);
		if(nOut==0)
		{
			DBStd.m_pDrawWingWall->MarkRebarHunchWallFront(&DomFront, bStt, bLeft);
			DBStd.m_pDrawWingWall->MarkRebarHunchFootingFront(&DomFront, bStt, bLeft);
		}

		rcFront = DomFront.GetExtRect();

		m_pIn->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_SMALL_TITLE);
		DomFront.SetTextAlignHorz(TA_CENTER);
		DomFront.SetTextHeight(7.5);
		DomFront.TextOut(rcFront.CenterPoint().x, rcFront.bottom+dSpare1, _T("정 면 도"));

		//DomFront.Move(dSpareT*se, 0);

		*pDom << DomFront;

		// 단면도
		CDomyun DomSect(pDom);

		CTwinVectorArray tvArrHu, tvArrGen;
		pBri->GetTvArrayRebarWingWallHunchFootingSect(tvArrHu, bStt, bLeft);
		CDPoint xyHu1 = tvArrHu.GetAt(0).m_v1;
		CDPoint xyHu2 = tvArrHu.GetAt(2).m_v2;
		double dHuW = pWing->m_dHuW1;
		double dHuH = pWing->m_dHuH1;
		double dCoverWingOut = pBri->m_dCoverWingOut[0];
		double dSpare = (!bSide)? fabs(xyHu1.x-xyHu2.x) + dCoverWingOut*2 - (dT2+dHuW)
			: fabs(xyHu1.x-xyHu2.x) + dCoverWingOut*2 + (dT2+dHuW)+1000;
		if(dHuW==0 || dHuH==0) dSpare = 1500;
		if(pBri->IsBoxType() && !pBri->m_bExistProtectionRiseFooting) dSpare = 0;

		m_pIn->m_pARoadOptionStd->SetEnvType(&DomSect, HCAD_CONC);
		DBStd.m_pDrawWingWall->DrawGenSection(&DomSect, bStt, bLeft, dSpare, TRUE);

		m_pIn->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_BARC);
		DBStd.m_pDrawWingWall->DrawRebarSection(&DomSect, bStt, bLeft);

		DBStd.m_pDrawWingWall->DimRebarSectIn(&DomSect, bStt, bLeft);
		DBStd.m_pDrawWingWall->DimRebarSectOut(&DomSect, bStt, bLeft);
		DBStd.m_pDrawWingWall->DimRebarSectTop(&DomSect, bStt, bLeft);

 		DBStd.m_pDrawWingWall->MarkRebarSection(&DomSect, bStt, bLeft, TRUE);
 		DBStd.m_pDrawWingWall->MarkRebarSection(&DomSect, bStt, bLeft, FALSE);

		CDRect rcSect = DomSect.GetExtRect();

		m_pIn->m_pARoadOptionStd->SetEnvType(&DomSect, HCAD_SMALL_TITLE);
		DomSect.SetTextAlignHorz(TA_CENTER);
		DomSect.SetTextHeight(7.5);
		DomSect.TextOut(rcSect.CenterPoint().x, rcSect.bottom+dSpare1, _T("단 면 도"));

		if(se*lr>0)
			DomSect.Move((rcFront.Width()+rcSect.Width()-dSpare2)*se, 0);
		else
			DomSect.Move((rcFront.Width()+dSpare2)*se, 0);

		*pDom << DomSect;

		// 평면도
		BOOL bUpper = (m_nSelComboPlan == 0) ? TRUE : FALSE;
		CString strPlane = (m_nSelComboPlan == 0) ? _T("상 부 ") : _T("하 부 ");
		pBri->GetTvWingWallPlane(bStt, TRUE, TRUE);

		CDomyun DomPlane(pDom);

		CTwinVector tvEnd, tvOut, tvSlab;
		pBri->m_tvArrPlaneWingWall.GetTvByInfo(strLeft+_T("날개벽끝단"), tvEnd);
		pBri->m_tvArrPlaneWingWall.GetTvByInfo(strLeft+_T("날개벽외측"), tvOut);
		pBri->m_tvArrPlaneWingWall.GetTvByInfo(_T("슬래브끝단외측"), tvSlab);
		CDPoint vSlab = tvSlab.GetXyDir().Rotate90();
		CDPoint xyMid(0, 0), xyCut(0, 0);
		xyMid = tvEnd.GetXyMid();
		BOOL bCross = GetXyMatchSegAndLine(tvSlab.m_v1, tvSlab.m_v2, xyMid, vSlab, xyCut);
		double dCutLen = bCross ? ~(xyCut-tvOut.m_v1) : 1500;
		if(dCutLen<1500)
			dCutLen = 1500;

		pBri->GetTvArrayRebarWingWallHunchWallPlane(tvArrHu, bStt, bLeft, bUpper);
		xyHu1 = tvArrHu.GetAt(0).m_v1;
		xyHu2 = tvArrHu.GetAt(2).m_v2;
		CDPoint vHu1 = tvArrHu.GetAt(0).GetXyDir();
		CDPoint vHu2 = tvArrHu.GetAt(2).GetXyDir();
		if(!bSide) GetXyMatchLineAndLine(xyHu1, vHu1, xyHu2, vHu2, xyHu1);
		dSpare = max(~(xyHu1-xyHu2)+dCoverWingOut*2, dCutLen);
		if(pWing->m_bAddRebarReinforce && !bSide)
		{
			CDPoint xyRf[3];
			pBri->GetXyRebarWingWallReinforce(xyRf, bStt, bLeft);
			dSpare = max(~(xyRf[0]-xyRf[1])+200, dSpare);
		}

		m_pIn->m_pARoadOptionStd->SetEnvType(&DomPlane, HCAD_CONC);
		DBStd.m_pDrawWingWall->DrawGenCutPlane(&DomPlane, bStt, bLeft, dSpare, bUpper);

		m_pIn->m_pARoadOptionStd->SetEnvType(&DomPlane, HCAD_BARC);
		DBStd.m_pDrawWingWall->DrawRebarPlane(&DomPlane, bStt, bLeft, bUpper);

		m_pIn->m_pARoadOptionStd->SetEnvType(&DomPlane, HCAD_DIML);
		DBStd.m_pDrawWingWall->DimRebarPlaneLower(&DomPlane, bStt, bLeft, bUpper);
		DBStd.m_pDrawWingWall->DimRebarPlaneEnd(&DomPlane, bStt, bLeft, bUpper);

		if(bUpper)
		{
			DBStd.m_pDrawWingWall->MarkRebarPlaneUpper(&DomPlane, bStt, bLeft, TRUE);
			DBStd.m_pDrawWingWall->MarkRebarPlaneUpper(&DomPlane, bStt, bLeft, FALSE);
		}
		else
		{
			DBStd.m_pDrawWingWall->MarkRebarPlaneLower(&DomPlane, bStt, bLeft, TRUE);
			DBStd.m_pDrawWingWall->MarkRebarPlaneLower(&DomPlane, bStt, bLeft, FALSE);
		}

		CDRect rcPlane = DomPlane.GetExtRect();

		m_pIn->m_pARoadOptionStd->SetEnvType(&DomPlane, HCAD_SMALL_TITLE);
		DomPlane.SetTextAlignHorz(TA_CENTER);
		DomPlane.SetTextHeight(7.5);
		DomPlane.TextOut(rcPlane.CenterPoint().x, rcPlane.bottom+dSpare1, strPlane + _T("평 면 도"));

		CDPoint xyMove(0, 0);
		if(bUpper)
		{
			xyMove = pBri->GetXySectionWingWallStt(bStt, TRUE);
			xyMove.y = rcFront.bottom - rcPlane.top + dSpare2;
		}
		else
		{
			xyMove = pBri->GetXySectionWingWallStt(bStt, FALSE);
			xyMove.y = rcFront.top - rcPlane.bottom - dSpare2;
		}

		DomPlane.Move(xyMove);
		*pDom << DomPlane;
		
		pBri->GetTvVertSection_Std(nLeft, TRUE, FALSE, TRUE);
	}

	// 위치도
	if(pBri->m_nTypeBridge==BRIDGE_TYPE_PORTAL)
	{
		CDomyun DomLoc(pDom);

		double dScale = 250.0;
		double dRatio = 50.0/dScale;
		DomLoc.SetScaleDim(dScale);

		//long nTypeJongdanForRebarOrg = pBri->m_nTypeJongdanForRebar;
		//pBri->m_nTypeJongdanForRebar = 2;

		//pBri->SyncBridge();
		pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 2);

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
		DomLoc.TextOut(rcLoc.CenterPoint().x, rcLoc.bottom+dSpare1/dRatio, _T("위 치 도"));

		if(bStt)
			DomLoc.Move(-rcLoc.left, -rcLoc.top);
		else
			DomLoc.Move(-(rcLoc.left+rcLoc.Width()), -rcLoc.top);
		DomLoc.RedrawByScale(dRatio);

		if(bStt)
			DomLoc.Move(rcFront.right+dSpare1, rcFront.top+dSpare2);
		else
			DomLoc.Move(rcFront.left-dSpare1, rcFront.top+dSpare2);

		*pDom << DomLoc;

		pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE);
	}
	
	//pBri->m_nTypeJongdanForRebar = nTypeJongdanForRebarOrg;
	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

BOOL CInputRebarWingWall::IsValid()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	int nCountBri1 = 0;
	int nCountBri2 = 0;
	int nCountWing = 0;
	int nSizeBri = pStd->GetBridgeSize();
	for(int nBr=0; nBr<nSizeBri; nBr++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(nBr);

		if(!pBri->IsOutsideWall())
			nCountBri1++;
		if(pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB)
			nCountBri2++;

		for(long stt=0; stt<2; stt++)
		{
			for(long left=0; left<2; left++)
			{
				CWingWall* pWing = pBri->GetWingWall(stt==0, left==0);
				if(pWing)
				{
					if(pWing->m_bExist) nCountWing++;
				}
			}
		}
	}

	// 교량의 형식 중에 문형 라멘교나 박스교가 없을 경우 날개벽 제원 입력 메뉴를 표시하지 않음
	if(nSizeBri==nCountBri1)
		return FALSE;
	// 교량의 형식이 모두 슬래브교일 경우 기둥 위치 입력 이후의 메뉴를 표시하지 않음
	if(nSizeBri==nCountBri2)
		return FALSE;

	// 날개벽이 하나도 없어도 
	if(nCountWing == 0)
		return FALSE;

	return TRUE;
}

void CInputRebarWingWall::SetDataDefault()
{
	for(long bri = 0; bri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(bri);
		if(!pBri) return;
		if(pBri->IsOutsideWall()==FALSE) continue;

		BOOL bFirst   = IsFirst(bri);
		pBri->SetDefaultedState(GetIDD(), 0x00000001);
		if(!bFirst && !IsClickDefaultButton(bri)) continue;

		int nCombo = m_cbSelect.GetCurSel();
		if(nCombo == -1) return;

		int nTab = m_Tab.GetCurSel();
		CString strCombo = _T(""), strStt = _T(""), strLeft = _T("");
		m_cbSelect.GetLBText(nCombo, strCombo);
		if(strCombo.GetLength() <= 9) continue;
		strStt = strCombo.Left(4);
		strLeft = strCombo.Left(9).Right(4);

		BOOL bStt = strStt==_T("시점") || bFirst ? TRUE : FALSE;
		BOOL bEnd = strStt==_T("종점") || bFirst ? TRUE : FALSE;
		BOOL bLeft = strLeft==_T("좌측") || bFirst ? TRUE : FALSE;
		BOOL bRight = strLeft==_T("우측") || bFirst ? TRUE : FALSE;
		BOOL bOut = nTab==0 || bFirst ? TRUE : FALSE;
		BOOL bIn = nTab==1 || bFirst ? TRUE : FALSE;

		for(long nDan = 0; nDan < 2; nDan++)
		{
			if(bStt)
			{
				if(bLeft)
				{
					//pBri->SyncBridge(1);
					pBri->GetTvVertSection_Std(1, TRUE, FALSE);
					pBri->GetTvWingWallPlane(TRUE);
					if(bFirst)
						pBri->SetDataDefaultRebarWingWallOption(TRUE, TRUE);
					if(bOut)
					{
						pBri->SetDataDefaultRebarWingWallUpper(TRUE, TRUE, 1, nDan);
						pBri->SetDataDefaultRebarWingWallLower(TRUE, TRUE, 1, nDan);
						pBri->SetDataDefaultRebarWingWallSide(TRUE, TRUE, 1, nDan);	
					}
					if(bIn)
					{
						pBri->SetDataDefaultRebarWingWallUpper(TRUE, TRUE, 0, nDan);
						pBri->SetDataDefaultRebarWingWallLower(TRUE, TRUE, 0, nDan);
						pBri->SetDataDefaultRebarWingWallSide(TRUE, TRUE, 0, nDan);						
					}
					
					pBri->SetDataDefaultRebarJewonWingWall(TRUE, TRUE);					
					pBri->GetTvVertSection_Std(1, TRUE, FALSE, TRUE);
				}
				if(bRight)
				{
					//pBri->SyncBridge(-1);
					pBri->GetTvVertSection_Std(-1, TRUE, FALSE);
					pBri->GetTvWingWallPlane(TRUE);
					if(bFirst)
						pBri->SetDataDefaultRebarWingWallOption(TRUE, FALSE);
					if(bOut)
					{
						pBri->SetDataDefaultRebarWingWallUpper(TRUE, FALSE, 1, nDan);
						pBri->SetDataDefaultRebarWingWallLower(TRUE, FALSE, 1, nDan);
						pBri->SetDataDefaultRebarWingWallSide(TRUE, FALSE, 1, nDan);
					}
					if(bIn)
					{
						pBri->SetDataDefaultRebarWingWallUpper(TRUE, FALSE, 0, nDan);
						pBri->SetDataDefaultRebarWingWallLower(TRUE, FALSE, 0, nDan);
						pBri->SetDataDefaultRebarWingWallSide(TRUE, FALSE, 0, nDan);
					}
					pBri->SetDataDefaultRebarJewonWingWall(TRUE, FALSE);

					pBri->GetTvVertSection_Std(-1, TRUE, FALSE, TRUE);
				}
			}
			if(bEnd)
			{
				if(bLeft)
				{
					//pBri->SyncBridge(-1);
					pBri->GetTvVertSection_Std(-1, TRUE, FALSE);
					pBri->GetTvWingWallPlane(FALSE);
					if(bFirst)
						pBri->SetDataDefaultRebarWingWallOption(FALSE, TRUE);
					if(bOut)
					{
						pBri->SetDataDefaultRebarWingWallUpper(FALSE, TRUE, 1, nDan);
						pBri->SetDataDefaultRebarWingWallLower(FALSE, TRUE, 1, nDan);
						pBri->SetDataDefaultRebarWingWallSide(FALSE, TRUE, 1, nDan);
					}
					if(bIn)
					{
						pBri->SetDataDefaultRebarWingWallUpper(FALSE, TRUE, 0, nDan);
						pBri->SetDataDefaultRebarWingWallLower(FALSE, TRUE, 0, nDan);
						pBri->SetDataDefaultRebarWingWallSide(FALSE, TRUE, 0, nDan);
					}
					pBri->SetDataDefaultRebarJewonWingWall(FALSE, TRUE);

					pBri->GetTvVertSection_Std(-1, TRUE, FALSE, TRUE);
				}
				if(bRight)
				{
					//pBri->SyncBridge(1);
					pBri->GetTvVertSection_Std(1, TRUE, FALSE);
					pBri->GetTvWingWallPlane(FALSE);
					if(bFirst)
						pBri->SetDataDefaultRebarWingWallOption(FALSE, FALSE);
					if(bOut)
					{
						pBri->SetDataDefaultRebarWingWallUpper(FALSE, FALSE, 1, nDan);
						pBri->SetDataDefaultRebarWingWallLower(FALSE, FALSE, 1, nDan);
						pBri->SetDataDefaultRebarWingWallSide(FALSE, FALSE, 1, nDan);
					}
					if(bIn)
					{
						pBri->SetDataDefaultRebarWingWallUpper(FALSE, FALSE, 0, nDan);
						pBri->SetDataDefaultRebarWingWallLower(FALSE, FALSE, 0, nDan);
						pBri->SetDataDefaultRebarWingWallSide(FALSE, FALSE, 0, nDan);
					}
					pBri->SetDataDefaultRebarJewonWingWall(FALSE, FALSE);

					pBri->GetTvVertSection_Std(1, TRUE, FALSE, TRUE);
				}
			}
		}
	}


	//구간선택 콤보박스 설정.
	for(long iSE = iSTT; iSE <= iEND; iSE++)
	{
		for(long iLR = iLEFT; iLR <= iRIGHT; iLR++)
		{
			for(long iIO = iOUTTER; iIO <= iINNER; iIO++)
			{
				for(long nDan = 0; nDan <= 1; nDan++)
				{
					SetDataDefaultBlock(iSE, iLR, iIO, nDan);
				}
			}
			
		}
	}

	SetGridData();
	SetDataInit();	
	SetDataSave();
}

// 구간 선택 콤보박스 초기화
void CInputRebarWingWall::SetDataDefaultBlock(BOOL bStt, BOOL bLeft, BOOL bOut, long nDan)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	int nCountUpper, nCountLower, nCountSide;

	nCountUpper = pWing->m_xyArrUpper[bOut][nDan].GetSize();
	nCountLower = pWing->m_xyArrLower[bOut][nDan].GetSize();
	nCountSide = pWing->m_xyArrSide[bOut][nDan].GetSize();

	m_nCountUpper[bStt][bLeft][bOut][nDan] = nCountUpper - 1;
	m_nCountLower[bStt][bLeft][bOut][nDan] = nCountLower - 1;
	m_nCountSide[bStt][bLeft][bOut][nDan] = nCountSide - 2;

}
void CInputRebarWingWall::SetDataInit()
{
//	if(m_cbSelect.GetCurSel() < 0)
//		return;

	m_Grid.UpdateData(FALSE);
	
	long nRows(0), nCols(0);
	m_Grid.GetCellCount(nRows, nCols);
	//Merge
	//2단
	if(nRows > 5)
	{
		m_Grid.MergeGrid(0, 1, 0, nCols-1);
 		m_Grid.SetMergeRow(2, 3, 0);
 		m_Grid.SetMergeRow(4, 5, 0);
	}
	else if(nRows > 1)
	{
		m_Grid.MergeGrid(0, 1, 1, nCols-1);
 		m_Grid.SetMergeRow(0, 1, 0);
	}
	m_Grid.SetRedraw(TRUE, TRUE);

}

void CInputRebarWingWall::SetGridData()
{
	m_Grid.SetGridDefault(2, 2, 2, 1);
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	int nCombo = m_cbSelect.GetCurSel();
	if(nCombo < 0)
	{
		m_pIn->GridMessage(&m_Grid, _T("선택된 날개벽이 없습니다."));
		return;
	}

	int nTab = m_Tab.GetCurSel();
	CString strCombo = _T(""), strStt = _T(""), strLeft = _T("");
	if(nCombo>=0)
		m_cbSelect.GetLBText(nCombo, strCombo);
	strStt = strCombo.Left(4);
	strLeft = strCombo.Left(9).Right(4);

	BOOL bStt = strStt==_T("시점") ? TRUE : FALSE;
	BOOL bLeft = strLeft==_T("좌측") ? TRUE : FALSE;
	int nOut = nTab==0 ? 1 : 0;

	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);
	if(!pWing->m_bExist)
	{
		m_Grid.DeleteAllItems();
		m_Grid.SetRedraw(TRUE,TRUE);
		return;
	}

	CStringArray strArr;
	CString str = _T("");
	for(long ix = 1; ix < 7; ix ++)
	{
		str.Format(_T("%d구간"), ix);
		strArr.Add(str);
	}

	//최대 단수찾기
	long nMaxAB = max(pWing->m_nCountLayerA[nOut], pWing->m_nCountLayerB[nOut]);
	long nMaxCD = max(pWing->m_nCountLayerC[nOut], pWing->m_nCountLayerD[nOut]);
	long nMaxDan = max(nMaxAB, nMaxCD);

	//최대 구간찾기(2단인경우는 구간이 큰쪽으로 열생성)
	int nCountMaxUpper = (nMaxDan > 1) ? max(m_nCountUpper[bStt][bLeft][nOut][0], m_nCountUpper[bStt][bLeft][nOut][1])+1 : m_nCountUpper[bStt][bLeft][nOut][0]+1;
	int nCountMaxLower = (nMaxDan > 1) ? max(m_nCountLower[bStt][bLeft][nOut][0], m_nCountLower[bStt][bLeft][nOut][1])+1 : m_nCountLower[bStt][bLeft][nOut][0]+1;
	int nCountMaxSide = (nMaxDan > 1) ? max(m_nCountSide[bStt][bLeft][nOut][0], m_nCountSide[bStt][bLeft][nOut][1])+1 : m_nCountSide[bStt][bLeft][nOut][0]+1;

	long nRow = 0, nCol = 0, ix = 0;
	//TiTle
	if(nMaxDan > 1)
	{
		m_Grid.SetGridDefault(2, 2, 2, 2);

		m_Grid.AddCell(0, nCol, _T("구분"));
		m_Grid.AddCell(1, nCol, _T("구분"));
		m_Grid.AddCell(2, nCol, _T("1단"));
		m_Grid.AddCell(3, nCol, _T("1단"));
		m_Grid.AddCell(4, nCol, _T("2단"));
		m_Grid.AddCell(5, nCol++, _T("2단"));

		m_Grid.AddCell(4, nCol, _T("개수"));
		m_Grid.AddCell(5, nCol, _T("간격"));
	}
	m_Grid.AddCell(0, nCol, _T("구분"));
	m_Grid.AddCell(1, nCol, _T("구분"));
	m_Grid.AddCell(2, nCol, _T("개수"));
	m_Grid.AddCell(3, nCol++, _T("간격"));
	
	//상면
	if(nCountMaxUpper > 0)
	{
		m_Grid.AddCell(0, nCol, _T("상면배치 간격"));
		m_Grid.AddCell(1, nCol, _T("구간"));
		m_Grid.AddCell(2, nCol, &m_nCountUpper[bStt][bLeft][nOut][0], nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, strArr);
		m_Grid.AddCell(3, nCol, _T("-"), nFormat, CELL_READONLY);
		if(nMaxDan > 1)
		{
			m_Grid.AddCell(4, nCol, &m_nCountUpper[bStt][bLeft][nOut][1], nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, strArr);
			m_Grid.AddCell(5, nCol, _T("-"), nFormat, CELL_READONLY);
		}
		nCol++;

		for(ix=0; ix<nCountMaxUpper; ix++)
		{
			m_Grid.AddCell(0, nCol, _T("상면배치 간격"));
			str.Format(_T("U%d"), ix+1);
			m_Grid.AddCell(1, nCol, str);

			m_Grid.AddCell(2, nCol, &pWing->m_xyArrUpper[nOut][0], ix, TRUE, nFormat, (m_nCountUpper[bStt][bLeft][nOut][0] < ix) ? CELL_READONLY : nLine);	m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.AddCell(3, nCol, &pWing->m_xyArrUpper[nOut][0], ix, FALSE, nFormat, (m_nCountUpper[bStt][bLeft][nOut][0] < ix) ? CELL_READONLY : nLine);	m_Grid.SetCellDec(ix ==0? CELL_DEC_1 : CELL_DEC_0);
			if(nMaxDan > 1)
			{
				//상면 2단활성화조건 : A, D 가 2단 
				BOOL bEnable = (pWing->m_nCountLayerA[nOut] == 2 || pWing->m_nCountLayerD[nOut] == 2);
				m_Grid.AddCell(4, nCol, &pWing->m_xyArrUpper[nOut][1], ix, TRUE, nFormat, (m_nCountUpper[bStt][bLeft][nOut][1] < ix || !bEnable) ? CELL_READONLY : nLine);	m_Grid.SetCellDec(CELL_DEC_0);
				m_Grid.AddCell(5, nCol, &pWing->m_xyArrUpper[nOut][1], ix, FALSE, nFormat, (m_nCountUpper[bStt][bLeft][nOut][1] < ix || !bEnable) ? CELL_READONLY : nLine);	m_Grid.SetCellDec(CELL_DEC_0);
				if(!bEnable) pWing->m_xyArrUpper[nOut][1].SetAt(ix, CDPoint(0, 0));
			}
			nCol++;
		}
	}

	//하면
	if(nCountMaxLower > 0)
	{
		m_Grid.AddCell(0, nCol, _T("하면배치 간격"));
		m_Grid.AddCell(1, nCol, _T("구간"));
		m_Grid.AddCell(2, nCol, &m_nCountLower[bStt][bLeft][nOut][0], nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, strArr);
		m_Grid.AddCell(3, nCol, _T("-"), nFormat, CELL_READONLY);
		if(nMaxDan > 1)
		{
			m_Grid.AddCell(4, nCol, &m_nCountLower[bStt][bLeft][nOut][1], nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, strArr);
			m_Grid.AddCell(5, nCol, _T("-"), nFormat, CELL_READONLY);
		}
		nCol++;

		for(ix=0; ix<nCountMaxLower; ix++)
		{
			m_Grid.AddCell(0, nCol, _T("하면배치 간격"));
			str.Format(_T("L%d"), ix+1);
			m_Grid.AddCell(1, nCol, str);
			m_Grid.AddCell(2, nCol, &pWing->m_xyArrLower[nOut][0], ix, TRUE, nFormat, (m_nCountLower[bStt][bLeft][nOut][0] < ix) ? CELL_READONLY : nLine);		m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.AddCell(3, nCol, &pWing->m_xyArrLower[nOut][0], ix, FALSE, nFormat, (m_nCountLower[bStt][bLeft][nOut][0] < ix) ? CELL_READONLY : nLine);	m_Grid.SetCellDec(CELL_DEC_0);
			if(nMaxDan > 1)
			{
				//하면 2단활성화조건 : B,C가 2단
				BOOL bEnable = (pWing->m_nCountLayerB[nOut] == 2 || pWing->m_nCountLayerC[nOut] == 2);
				m_Grid.AddCell(4, nCol, &pWing->m_xyArrLower[nOut][1], ix, TRUE, nFormat, (m_nCountLower[bStt][bLeft][nOut][1] < ix || !bEnable) ? CELL_READONLY : nLine);	m_Grid.SetCellDec(CELL_DEC_0);
				m_Grid.AddCell(5, nCol, &pWing->m_xyArrLower[nOut][1], ix, FALSE, nFormat, (m_nCountLower[bStt][bLeft][nOut][1] < ix || !bEnable) ? CELL_READONLY : nLine);m_Grid.SetCellDec(CELL_DEC_0);
				if(!bEnable) pWing->m_xyArrLower[nOut][1].SetAt(ix, CDPoint(0, 0));
			}
			nCol++;
		}
	}

	//측면
	if(nCountMaxSide > 0)
	{
		m_Grid.AddCell(0, nCol, _T("측면배치 간격"));
		m_Grid.AddCell(1, nCol, _T("구간"));
		m_Grid.AddCell(2, nCol, &m_nCountSide[bStt][bLeft][nOut][0], nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, strArr);
		m_Grid.AddCell(3, nCol, _T("-"), nFormat, CELL_READONLY);
		if(nMaxDan > 1)
		{
 			m_Grid.AddCell(4, nCol, &m_nCountSide[bStt][bLeft][nOut][1], nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, strArr);
			m_Grid.AddCell(5, nCol, _T("-"), nFormat, CELL_READONLY);
		}
		nCol++;

		for(ix=0; ix<nCountMaxSide; ix++)
		{
			m_Grid.AddCell(0, nCol, _T("측면배치 간격"));
			str.Format(_T("S%d"), ix+1);
			m_Grid.AddCell(1, nCol, str);
			m_Grid.AddCell(2, nCol, &pWing->m_xyArrSide[nOut][0], ix, TRUE, nFormat, (m_nCountSide[bStt][bLeft][nOut][0] < ix) ? CELL_READONLY : nLine);	m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.AddCell(3, nCol, &pWing->m_xyArrSide[nOut][0], ix, FALSE, nFormat, (m_nCountSide[bStt][bLeft][nOut][0] < ix) ? CELL_READONLY : nLine);	m_Grid.SetCellDec(CELL_DEC_0);
			if(nMaxDan > 1)
			{
				m_Grid.AddCell(4, nCol, &pWing->m_xyArrSide[nOut][1], ix, TRUE, nFormat, (m_nCountSide[bStt][bLeft][nOut][1] < ix) ? CELL_READONLY : nLine);	m_Grid.SetCellDec(CELL_DEC_0);
				m_Grid.AddCell(5, nCol, &pWing->m_xyArrSide[nOut][1], ix, FALSE, nFormat, (m_nCountSide[bStt][bLeft][nOut][1] < ix) ? CELL_READONLY : nLine);	m_Grid.SetCellDec(CELL_DEC_0);
			}
			nCol++;
		}
	}

	m_Grid.GetCellCount(nRow, nCol);
	m_Grid.SetRows(nRow);
	m_Grid.SetCols(nCol);
	m_Grid.SetColumnWidth(0, 50);
	m_Grid.SetRedraw(TRUE,TRUE);
}
void CInputRebarWingWall::SetDataSave()
{
	UpdateData(TRUE);
	m_Grid.UpdateData(TRUE);

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	int nCombo = m_cbSelect.GetCurSel();
	if(nCombo < 0) return;

	int nTab = m_Tab.GetCurSel();
	CString strCombo = _T("");//, strStt = _T(""), strLeft = _T("");
	m_cbSelect.GetLBText(nCombo, strCombo);
	CString strStt = strCombo.Left(4);
	CString strLeft = strCombo.Left(9).Right(4);

	BOOL bStt = strStt==_T("시점") ? TRUE : FALSE;
	BOOL bLeft = strLeft==_T("좌측") ? TRUE : FALSE;
	int nOut = nTab==0 ? 1 : 0;

	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);
	for(long nDan = 0; nDan < 2; nDan++)
	{
		if(m_nCountSide[bStt][bLeft][nOut][nDan] < 0) continue;
		if(m_nCountSide[bStt][bLeft][nOut][nDan]+1 < pWing->m_xyArrSide[nOut][nDan].GetSize())
		{
			long nCount = pWing->m_xyArrSide[nOut][nDan].GetSize();
			if(nCount < 1) continue;

			long i=0; for(i=0; i<(nCount-m_nCountSide[bStt][bLeft][nOut][nDan]-1); i++)
			{
				long nCnt = pWing->m_xyArrSide[nOut][nDan].GetSize();
				pWing->m_xyArrSide[nOut][nDan].RemoveAt(nCnt-1);
			}
		}

		pBri->SetRebarWingWallSideExt(bStt, bLeft, nOut, nDan);
	}

	m_pIn->m_pDoc->SetModifiedFlag();

	if(m_pDlgCheck->GetSafeHwnd() && m_pDlgCheck->IsWindowVisible())
	{
		pBri->MakeRebarInfoCycleStd(TRUE);
		m_pDlgCheck->Check();
	}
}

void CInputRebarWingWall::OnButtonOption() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);

	int nCombo = m_cbSelect.GetCurSel();
	CString strCombo = _T("");//, strStt = _T(""), strLeft = _T("");
	m_cbSelect.GetLBText(nCombo, strCombo);
	CString strStt = strCombo.Left(4);
	CString strLeft = strCombo.Left(9).Right(4);

	BOOL bStt = strStt==_T("시점") ? TRUE : FALSE;
	BOOL bLeft = strLeft==_T("좌측") ? TRUE : FALSE;
	

	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	double dStdVerCTC	= pWing->m_dStdVerCTC;
	double dMainCTCA	= pWing->m_dMainCTCA;
	double dMainCTCB	= pWing->m_dMainCTCB;
	BOOL bUpperRebarSlope = pWing->m_bUpperRebarSlope;
	BOOL bSelMainCTCD	= pWing->m_bSelMainCTCD;

	CRebarWingWallDlg dlg(m_pIn, bStt, bLeft, GetParent());
	if(dlg.DoModal()==IDOK)
	{
		int nDan = 0;
		if(dStdVerCTC!=pWing->m_dStdVerCTC)
		{	
			if(AfxMessageBox(_T("수직 철근 표준 배근 간격이 변경되었습니다. "
				"\n\n날개벽 상면 및 하면 배근 간격이 재조정됩니다. "
				"계속 하시겠습니까?  "), MB_YESNO | MB_ICONQUESTION)==IDYES)
			{
				int i=0; for(i=0; i<2; i++)
				{
					pBri->SetDataDefaultRebarWingWallUpper(bStt, bLeft, i, nDan);
					pBri->SetDataDefaultRebarWingWallLower(bStt, bLeft, i, nDan);
				}
			}
			else
				pWing->m_dStdVerCTC = dStdVerCTC;
		}
		if(dMainCTCA!=pWing->m_dMainCTCA)
		{
			if(AfxMessageBox(_T("A구간 주철근 배치 간격이 변경되었습니다. "
				"\n\n날개벽 측면 배근 간격이 재조정됩니다. "
				"계속 하시겠습니까?  "), MB_YESNO | MB_ICONQUESTION)==IDYES)
			{
				int i=0; for(i=0; i<2; i++)
					pBri->SetDataDefaultRebarWingWallSide(bStt, bLeft, i, nDan);
			}
			else
				pWing->m_dMainCTCA = dMainCTCA;
		}
		if(dMainCTCB!=pWing->m_dMainCTCB)
		{
			if(AfxMessageBox(_T("B구간 주철근 배치 간격이 변경되었습니다. "
				"\n\n날개벽 측면 배근 간격이 재조정됩니다. "
				"계속 하시겠습니까?  "), MB_YESNO | MB_ICONQUESTION)==IDYES)
			{
				int i=0; for(i=0; i<2; i++)
					pBri->SetDataDefaultRebarWingWallSide(bStt, bLeft, i, nDan);
			}
			else
				pWing->m_dMainCTCB = dMainCTCB;
		}
		if(bUpperRebarSlope!=pWing->m_bUpperRebarSlope)
		{
			if(AfxMessageBox(_T("상면 주철근 배치 옵션이 변경되었습니다. "
				"\n\n날개벽 측면 배근 간격이 재조정됩니다. "
				"계속 하시겠습니까?  "), MB_YESNO | MB_ICONQUESTION)==IDYES)
			{
				int i=0; for(i=0; i<2; i++)
					pBri->SetDataDefaultRebarWingWallSide(bStt, bLeft, i, nDan);
			}
			else
				pWing->m_bUpperRebarSlope = bUpperRebarSlope;
		}
		if(bSelMainCTCD != pWing->m_bSelMainCTCD)
		{
			pBri->SetDataDefaultRebarWingWallUpper(bStt, bLeft, 0, nDan);
		}

		SetGridData();
		SetDataInit();
		SetDataSave();

		pBri->SetDataDefaultRebarJewonWingWall(bStt, bLeft);

		DrawInputDomyunView();
	}
}

void CInputRebarWingWall::SetEnableWindow()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	// 날개벽이 없는 타입인 경우
	if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL && pBri->m_nTypeBridge!=BRIDGE_TYPE_TRAP2 && pBri->m_nTypeBridge!=BRIDGE_TYPE_ARCH && pBri->m_nTypeBridge!=BRIDGE_TYPE_BOX)
	{
		m_Tab.DeleteAllItems();
		m_Grid.DeleteAllItems();
		
		GetDlgItem(IDC_COMBO_SELECT)->EnableWindow(FALSE);
	}
	else
	{
		InitTabCtrl();
		GetDlgItem(IDC_COMBO_SELECT)->EnableWindow(TRUE);
	}
}
void CInputRebarWingWall::OnButtonExamine() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	int nCombo = m_cbSelect.GetCurSel();
	CString strCombo = _T(""), strStt = _T(""), strLeft = _T("");
	if(nCombo>=0)
		m_cbSelect.GetLBText(nCombo, strCombo);
	strStt = strCombo.Left(4);
	strLeft = strCombo.Left(9).Right(4);
	
	BOOL bStt  = strStt==_T("시점");
	BOOL bLeft = strLeft==_T("좌측");

	long nTypeCheck = 0;
	if(bStt && bLeft) nTypeCheck = 0;
	else if(bStt && !bLeft) nTypeCheck = 1;
	else if(!bStt && bLeft) nTypeCheck = 2;
	else if(!bStt && !bLeft) nTypeCheck = 3;
	
	if(m_pDlgCheck->GetSafeHwnd() && m_pDlgCheck->IsWindowVisible()) m_pDlgCheck->DestroyWindow();
	m_pDlgCheck->SetBriAndContent(pBri,IDD_REBAR_WINGWALL,nTypeCheck);

	m_pDlgCheck->ShowAndCheck(this);	
	CString str = _T("");
	str.Format("%s %s 날개벽 검토",strStt,strLeft);
	m_pDlgCheck->SetWindowText(str);
}

//Data바뀔때마다 제원 만들지 말고 나갈때 한번 생성
void CInputRebarWingWall::KillFocusDlg()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	for(BOOL bSE = 0; bSE < 2; bSE++)
	{
		for(BOOL bLR = 0; bLR < 2; bLR++)
		{
			pBri->SetDataDefaultRebarJewonWingWall(bSE, bLR);
		}
	}
}