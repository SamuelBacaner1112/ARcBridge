// InputGenHyunTa.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "InputGenHyunTa.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputGenHyunTa dialog


CInputGenHyunTa::CInputGenHyunTa(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CInputGenHyunTa::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputGenHyunTa)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nTab	= 0;
	m_nDan = 0;
	m_nGugan = 0;
	m_bViewHoriz = FALSE;
	m_nTabMain = -1;
	m_nTabRound = -1;
	m_nTabCross = -1;

	m_pIn = NULL;
	m_pDlgCheck = new CInputCheckEdit;
}

CInputGenHyunTa::~CInputGenHyunTa()
{
	delete m_pDlgCheck;
}

void CInputGenHyunTa::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputGenHyunTa)
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_comboBridgeNo);
	DDX_Control(pDX, IDC_COMBO_FOOTING, m_comboFooting);
	DDX_Control(pDX, IDC_COMBO_DAN, m_comboDan);
	DDX_Control(pDX, IDC_COMBO_GUGAN, m_comboGugan);
	DDX_Control(pDX, IDC_BUTTON_EXAMINE, m_btnExamine);
	DDX_Control(pDX, IDC_CHECK_REAL, m_checkHoriz);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	DDX_DomControl(pDX, IDC_DOM, m_DomView);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDOK, m_btnOK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputGenHyunTa, CXTResizeDialog)
	//{{AFX_MSG_MAP(CInputGenHyunTa)
	ON_BN_CLICKED(IDC_BUTTON_EXAMINE, OnButtonExamine)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_CBN_SELCHANGE(IDC_COMBO_FOOTING, OnSelchangeComboFooting)
	ON_CBN_SELCHANGE(IDC_COMBO_DAN, OnSelchangeComboDan)
	ON_CBN_SELCHANGE(IDC_COMBO_GUGAN, OnSelchangeComboGugan)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
	ON_BN_CLICKED(IDC_CHECK_REAL, OnCheckHorizontal)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_BN_CLICKED(IDC_BUTTON_RECOMMAND, &CInputGenHyunTa::OnBnClickedButtonRecommand)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputGenHyunTa message handlers

void CInputGenHyunTa::InitComboCtrl()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
 	if(!pBri) return;

	CString str = _T("");
	
	// 교량
	long nSize = pStd->GetBridgeSize();
	long nCountCombo = m_comboBridgeNo.GetCount();
	long i = 0; for(i = 0; i < nCountCombo; i++)
		m_comboBridgeNo.DeleteString(0);
	for(i = 0; i < nSize; i++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(i);
		str = pStd->GetStringBridgeOrder(i) + _T(" : ") + pBri->m_strBridgeName
			+ _T(" [") + pBri->GetStringBridgeType() + _T("]");
		m_comboBridgeNo.AddString(str);
	}
	m_comboBridgeNo.SetCurSel(m_pIn->m_nCurrentBridge);

	// 기초위치
	if(pBri->IsBoxType())
	{
		m_comboFooting.EnableWindow(FALSE);
	}
	else
	{
		long nCount = m_comboFooting.GetCount();
		for(i = 0; i < nCount; i++)
			m_comboFooting.DeleteString(0);

		m_comboFooting.AddString("시점부");
		for(i = 0; i < pBri->GetCountInWall(); i++)
		{
			str.Format("중간 지점부 %d", i+1);
			m_comboFooting.AddString(str);
		}
		m_comboFooting.AddString("종점부");
		m_comboFooting.SetCurSel(0);
		m_comboFooting.EnableWindow(TRUE);
	}
}

BOOL CInputGenHyunTa::OnInitDialog()
{
	// 시작하면서 pick box의 크기를 정해줌
	CXTResizeDialog::OnInitDialog();

	m_DomView.GetDomyun()->SetCWnd((CWnd*)&m_DomView);

// 	if(m_DomView) 
// 	{
// 		CTwinVector tv;
// 		tv.SetPickBoxSize(m_DomView.GetSelectionRectSize()/m_DomView.GetRatioWindow());
// 	}
	
	InitTabCtrl();
	InitComboCtrl();

	SetResize(IDC_BUTTON_EXAMINE,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_TAB,				SZ_TOP_LEFT,	SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID,				SZ_TOP_LEFT,	SZ_BOTTOM_RIGHT);
	SetResize(IDC_CHECK_REAL,		SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	SetResize(IDC_STATIC1,			SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	SetResize(IDC_STATIC2,			SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	SetResize(IDC_STATIC3,			SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	SetResize(IDC_COMBO_GUGAN,		SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	SetResize(IDC_COMBO_DAN,		SZ_TOP_RIGHT,	SZ_TOP_RIGHT);

// 	SetHyperHelp(m_pIn->GetManualPath());



	// 현장 타설이 있는 지점 우선 선택해 둠 ////////////////////////
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	long nJ = 0; for(nJ = 0; nJ <= pBri->m_nQtyJigan; nJ++)
	{
		CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(nJ);
		if(pFooting)
		{
			if(pFooting->m_bIs && pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_PILE)
			{
				if(pFooting->m_exFooting.m_Pile.m_nType == PILE_TYPE_HYUNJANG)
				{
					m_comboFooting.SetCurSel(nJ);
					break;
				}
			}
		}
	}
	////////////////////////////////////////////////////////////////

	SetGridData();
	SetDataInit();
	SetControl();

	DrawInputDomyunView(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control

}

void CInputGenHyunTa::InitTabCtrl()
{
	m_Tab.RemoveAllTabs();
	m_Tab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_Tab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);


	long nTab = 0;
	m_nTabMain = -1;
	m_nTabRound = -1;
	m_sArrTabName.RemoveAll();

	m_sArrTabName.Add("주철근 배치"); m_nTabMain = nTab++;
	m_sArrTabName.Add("띠철근 배치"); m_nTabRound = nTab++;
	m_sArrTabName.Add("크로스 타이 배치"); m_nTabCross = nTab++;

	
	long i = 0; for(i = 0; i < m_sArrTabName.GetSize(); i++)
		m_Tab.InsertItem(i, m_sArrTabName[i]);

	if(m_Tab.GetItemCount() <= m_nTab || m_nTab < 0) m_nTab = 0;
	m_Tab.SetCurSel(m_nTab);
}

void CInputGenHyunTa::SetDataDefault()
{
	for(long bri = 0; bri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		CRcBridgeApp *pBri = m_pIn->m_pARcBridgeDataStd->GetBridge(bri);
		if(!pBri) return;

// 		if(!IsFirst(bri))
// 		{
// 			if(IsClickDefaultButton(bri))
// 			{
// 				CExPileApp *pPile	= GetPile();
// 				if(!pPile) return;
// 				if(pPile->m_nType == PILE_TYPE_HYUNJANG)
// 				{
// 					pPile->m_dT1 = 0;
// 					pPile->m_dT2 = 0;
// 					pPile->SetDatDefaultHyunTaRebar();
// 				}
// 			}
// 		}
// 		else
// 		{
			long nCountFooting	= pBri->GetCountFooting();
			for(long j = 0; j < nCountFooting; j++)
			{
				CFootingApp *pFooting	= pBri->GetFooting(j);
				if(!pFooting) continue;
				if(pFooting->m_exFooting.m_nType != EXFOOTING_TYPE_PILE) continue;
				CExPileApp *pPile	= &pFooting->m_exFooting.m_Pile;
				if(pPile->m_nType == PILE_TYPE_HYUNJANG)
				{
					pPile->m_dT1 = 0;
					pPile->m_dT2 = 0;
					pPile->SetDatDefaultHyunTaRebar();
				}
			}

// 			pBri->SetDefaultedState(GetIDD(), 1);
// 		}
	}

	SetControl();
	SetGridData();
	SetDataInit();
}

void CInputGenHyunTa::SetDataSave()
{
 	m_Grid.UpdateData(TRUE);
}

void CInputGenHyunTa::SetGridData()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	CExPileApp *pPile	= GetPile();
	if(!pPile) return;
	
	// 현장타설 말뚝이 아니면 경고 메시지 후 리턴
	if(pPile->m_nType != PILE_TYPE_HYUNJANG) 
		return;

	m_Grid.InitGrid(m_nTab == m_nTabMain ? pPile->m_pHyunTa->m_dHP_Fy : pPile->m_pHyunTa->m_dHP_Fvy);
	
	if(m_nTab == m_nTabMain) SetGridDataMain();
	else if(m_nTab == m_nTabRound) SetGridDataRound();
	else if(m_nTab == m_nTabCross) SetGridDataCrossTie();
}

void CInputGenHyunTa::SetDataInit()
{
	CExPileApp *pPile	= GetPile();
	if(!pPile)
	{
		m_pIn->GridMessage(&m_Grid, "선택하신 기초는 말뚝기초가 아닙니다.");
		return;
	}
	
	// 현장타설 말뚝이 아니면 경고 메시지 후 리턴
	if(pPile->m_nType != PILE_TYPE_HYUNJANG) 
	{
		m_pIn->GridMessage(&m_Grid, "선택하신 기초의 말뚝은 현장타설 말뚝이 아닙니다.");
		return;
	}


	m_Grid.UpdateData(FALSE);
	if(m_nTab == m_nTabRound)
	{
		long nRows = 0;
		long nCols = 0;
		m_Grid.GetCellCount(nRows, nCols);
		m_Grid.MergeGrid(0, 0, 0, nCols-1);
	}
}


void CInputGenHyunTa::SetControl()
{
	CExPileApp *pPile	= GetPile();
	if(pPile)
	{
		if(pPile->m_pHyunTa->m_nHP_MainRebarDanSu > 0 && pPile->m_pHyunTa->m_nHP_MainRebarDanSu < 4)
			m_comboDan.SetCurSel(pPile->m_pHyunTa->m_nHP_MainRebarDanSu-1);
		if(pPile->m_pHyunTa->m_nHP_MainRebarGuganSu >= 0 && pPile->m_pHyunTa->m_nHP_MainRebarGuganSu < 3)
			m_comboGugan.SetCurSel(pPile->m_pHyunTa->m_nHP_MainRebarGuganSu);
		m_nGugan = m_comboGugan.GetCurSel();
	}

	BOOL bEnable = FALSE;
	if(pPile)
	{
		if(m_nTab == m_nTabMain && pPile->m_nType == PILE_TYPE_HYUNJANG)
			bEnable	= TRUE;
	}
	
	if(bEnable) 
	{
		m_comboDan.EnableWindow(TRUE);
		m_comboGugan.EnableWindow(TRUE);
	}
	else
	{
		m_comboDan.EnableWindow(FALSE);
		m_comboGugan.EnableWindow(FALSE);
	}

	if(m_bViewHoriz)
	{
		m_checkHoriz.SetCheck(TRUE);
	}
}

void CInputGenHyunTa::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_DomView.GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(m_pIn->GetScaleInputDialog());

	CExPileApp *pPile	= GetPile();
	if(!pPile) 
		return;

	if(pPile->m_nType != PILE_TYPE_HYUNJANG || pPile->m_dDia<=2*pPile->m_pHyunTa->m_dHP_MainRebarDist[pPile->m_pHyunTa->m_nHP_MainRebarDanSu-1] || pPile->m_dLength<=0) 
		return;

	CDrawExPile drawExPile(pPile, m_pIn->m_pARoadOptionStd);

	CDomyun domPlane(pDom);
	CDomyun domSection(pDom);


	// 평면
	drawExPile.DrawPlanDetail(&domPlane);
	drawExPile.DrawPlanRebarCrossTieDetail(&domPlane);
	drawExPile.DrawSectionPlanRebarMain_D(&domPlane, 0);		// 세번째 인자인 dDepth 사용안함.. 
	if(m_nTab == m_nTabMain)
	{
		int i=0; for(i=0; i<=pPile->m_pHyunTa->m_nHP_MainRebarDanSu; i++)
		{
			drawExPile.MarkRebarMainPlanDetail_D(&domPlane, i, TRUE);
		}
	}
	if(m_nTab == m_nTabRound) drawExPile.MarkRebarRoundPlanDetail_D(&domPlane, m_nDan, TRUE);
	drawExPile.DimRebarRoundPlanDetail_D(&domPlane, m_nDan, TRUE);
	if(m_nTab == m_nTabCross) drawExPile.MarkRebarCrossTiePlanDetail_D(&domPlane, m_nDan,TRUE);
	drawExPile.DimRebarCrossTiePlan_D(&domPlane, TRUE);
	CDRect rectPlane;
	domPlane.SetCalcExtRect();
	rectPlane = domPlane.GetExtRect();


	*pDom << domPlane;


	// 단면(정면)
	drawExPile.DrawFrontDetail(&domSection, FALSE);
	drawExPile.DrawFrontRebarMainDetail(&domSection, FALSE);
	drawExPile.DrawFrontRebarRoundDetail(&domSection, m_nDan, FALSE);
	if(m_nTab == m_nTabMain) drawExPile.DimFrontMainRebar_D(&domSection, 0, TRUE, FALSE, m_bViewHoriz);
	else if(m_nTab == m_nTabRound || m_nTab == m_nTabCross) drawExPile.DimFrontRoundRebar_D(&domSection, m_nDan, 0,TRUE, FALSE, m_bViewHoriz);
	
	CDRect rectSection;
	domSection.SetCalcExtRect();
	rectSection = domSection.GetExtRect();

	CDPoint xyMove(0, 0);
	xyMove.x = rectPlane.right - rectSection.left + domSection.Always(5);
	xyMove.y = rectPlane.top + rectPlane.Height()/2 - rectSection.top - rectSection.Height()/2;
	if(m_bViewHoriz) xyMove.x = xyMove.x * -1;
	else xyMove.x += domSection.Always(30);

	domSection.Move(xyMove);

	if(m_bViewHoriz)
	{
		CDPoint cp= rectPlane.CenterPoint();
		domSection.Rotate(cp, CDPoint(0, 1));
	}

	*pDom << domSection;

	if(bZoomAll)
	{
		m_DomView.ZoomAll();
	}
	else
	{
		m_DomView.RedrawAll();
	}
}

void CInputGenHyunTa::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	DrawInputDomyunView(FALSE);
	return;
}

void CInputGenHyunTa::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	SetFocus();
	m_nTab = m_Tab.GetCurSel();
	SetGridData();
	SetDataInit();
	SetControl();
	DrawInputDomyunView(FALSE);
	return;
}

void CInputGenHyunTa::OnButtonExamine() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	CExPileApp *pPile	= GetPile();
	if(!pPile)
	{
		AfxMessageBox("선택하신 기초는 말뚝기초가 아닙니다.");
		return;
	}
	
	// 현장타설 말뚝이 아니면 경고 메시지 후 리턴
	if(pPile->m_nType != PILE_TYPE_HYUNJANG) 
	{
		AfxMessageBox("선택하신 기초의 말뚝은 현장타설 말뚝이 아닙니다.");
		return;
	}
	
	BOOL bFemNormal = pBri->m_FemNormal.IsOutputLoaded();
	BOOL bFemEQ     = pBri->IsRcSlab() ? TRUE : (pBri->GetFemManage(COMBO_EARTHQUAKE)->IsOutputLoaded());
	
	if(!bFemNormal || (pBri->GetTypeDesingEQ() == 1 && !bFemEQ))
	{
		AfxMessageBox("모델링 해석을 수행후에 설계검토를 하시기 바랍니다.");
	}
	else 
	{
		long nJijum = m_comboFooting.GetCurSel();
		m_pDlgCheck->SetBriAndContent(pBri,IDD_DIALOG_HYUNTA,nJijum);
		m_pDlgCheck->ShowAndCheck(this);	
	}
}

void CInputGenHyunTa::OnSelchangeComboBridgeNo() 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	m_pIn->m_nCurrentBridge = m_comboBridgeNo.GetCurSel();
	InitTabCtrl();
	SetControl();
	SetGridData();
	SetDataInit();
//	SetPaneTitle();
	DrawInputDomyunView(TRUE);
	m_comboBridgeNo.SetFocus();
}

void CInputGenHyunTa::OnSelchangeComboFooting() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	InitTabCtrl();
	SetGridData();
	SetDataInit();
	SetControl();
	DrawInputDomyunView(TRUE);
	
	m_comboFooting.SetFocus();	
}

void CInputGenHyunTa::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	CCellID cell = m_Grid.GetFocusCell();

	SetDataSave();

	if(m_Grid.GetTextMatrix(0, cell.col) == _T("fy"))
	{
		SetGridData();
		SetDataInit();
	}

	DrawInputDomyunView(FALSE);
}

void CInputGenHyunTa::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	structGridCell *pCell = m_Grid.GetCurrentCell();
	if(pCell)
	{
		m_pIn->SetComment(pCell->sTitle);
		m_DomView.GetDomyun()->STMakeCursor(m_Grid.GetItemText(0,pCell->nCol));

		if(m_nTab == m_nTabMain)
		{
			if(pCell->nCol < 6+2*(m_nGugan)) m_nDan = 0;
			else m_nDan = (pCell->nCol - 3-2*(m_nGugan))/3;
			DrawInputDomyunView(FALSE);
		}
		else if(m_nTab == m_nTabRound)
		{
			m_nDan = pCell->nCol/6;
			DrawInputDomyunView(FALSE);
		}
		else m_nDan = 0;
		
		
		m_pIn->SetComment(pCell->sTitle);	
		CString sTitle	= m_Grid.GetItemText(0,pCell->nCol);
		CString subTitle = m_Grid.GetItemText(1, pCell->nCol);
		
		sTitle.TrimLeft("\n");
		sTitle.TrimRight("\n");
		
		CString sMark	= sTitle;
		
		if(sTitle == "B" || sTitle == "C") sMark = _T("B @ C");
		if(sTitle == "E" || sTitle == "F") sMark = _T("E @ F");
		if(sTitle == "내측 띠철근" || sTitle == "외측 띠철근") sMark = subTitle;

		m_DomView.GetDomyun()->STMakeCursor(sMark);
	}
}


BOOL CInputGenHyunTa::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if(pMsg->message == WM_MOUSEWHEEL)
	{
		m_DomView.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
	}

	return CXTResizeDialog::PreTranslateMessage(pMsg);
}

void CInputGenHyunTa::SetGridDataRound()
{
	CExPileApp *pPile	= GetPile();
	if(!pPile) return;

	CStringArray sArr;
	CString str = _T("");
	CString strHelp = _T("");
	long row = 0;
	long col = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;

	long iFix = (pPile->m_pHyunTa->m_nHP_MainRebarDanSu > 1)? 2:1;
	m_Grid.SetGridDefault(2, 2, iFix, 0);
	m_Grid.SetCellSize(20, -1);

	m_Grid.AddCell(row, col, "fy");
	m_Grid.AddCell(row+1, col++, &pPile->m_pHyunTa->m_dHP_Fvy, nFormat, nLine);
	m_Grid.SetCellUnit(UNIT_CGS_KGF_CM2, m_pIn->m_pARcBridgeDataStd->m_szKgf_CM2);
	
	row = 0;
	long i = 0; for(i = 0; i < iFix; i++)
	{
		if(i == 0) str = "외측 띠철근";
		else str = "내측 띠철근";

		row = 0;
		if(iFix == 2) m_Grid.AddCell(row++, col, str);
		m_Grid.AddCell(row++, col, "Dia");
		m_Grid.AddCell(row, col++, &pPile->m_pHyunTa->m_dHP_RoundRebarDia[i], nFormat, nLine, "띠철근 직경", CELL_TYPE_DIA);
		row = 0;

		if(iFix == 2) m_Grid.AddCell(row++, col, str);
		m_Grid.AddCell(row++, col, "L1");
		m_Grid.AddCell(row, col++, &pPile->m_pHyunTa->m_dHP_RoundRebarL1[i], nFormat, nLine);
		row = 0;

		if(iFix == 2) m_Grid.AddCell(row++, col, str);
		m_Grid.AddCell(row++, col, "L2");
		m_Grid.AddCell(row, col++, &pPile->m_pHyunTa->m_dHP_RoundRebarL2[i], nFormat, nLine);
		row = 0;

		if(iFix == 2) m_Grid.AddCell(row++, col, str);
		m_Grid.AddCell(row++, col, "CTC1");
		m_Grid.AddCell(row, col++, &pPile->m_pHyunTa->m_dHP_RoundRebarCTC1[i], nFormat, nLine);
		row = 0;

		if(iFix == 2) m_Grid.AddCell(row++, col, str);
		m_Grid.AddCell(row++, col, "CTC2");
		m_Grid.AddCell(row, col++, &pPile->m_pHyunTa->m_dHP_RoundRebarCTC2[i], nFormat, nLine);
		row = 0;

		if(iFix == 2) m_Grid.AddCell(row++, col, str);
		m_Grid.AddCell(row++, col, "CTC3");
		m_Grid.AddCell(row, col++, &pPile->m_pHyunTa->m_dHP_RoundRebarCTC3[i], nFormat, nLine);
		row = 0;
	}
	
	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(55);
}

void CInputGenHyunTa::SetGridDataMain()
{
	CExPileApp *pPile	= GetPile();
	if(!pPile) return;

	CStringArray sArr;
	CString str = _T("");
	CString strHelp = _T("");
	long row = 0;
	long col = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;

	m_Grid.SetGridDefault(2, 2, 1, 0);
	m_Grid.SetCellSize(20, -1);

	CString szUnit_CGS   = "kgf/cm²";
	CString szUnit_Apply = m_pIn->m_pARcBridgeDataStd->GetStringUnitType(szUnit_CGS);

	m_Grid.AddCell(row, col, "fck");
	m_Grid.AddCell(row+1, col++, &pPile->m_pHyunTa->m_dHP_Fck, nFormat, nLine);
	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);

	m_Grid.AddCell(row, col, "fy");
	m_Grid.AddCell(row+1, col++, &pPile->m_pHyunTa->m_dHP_Fy, nFormat, nLine);
	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);

	long i = 0; for(i = 0; i < 3; i++)
	{
		if(i>=pPile->m_pHyunTa->m_nHP_MainRebarDanSu)
		{
			pPile->m_pHyunTa->m_dHP_MainRebarDist[i] = 0;
			pPile->m_pHyunTa->m_dHP_MainRebarDia[i]  = 0;
			pPile->m_pHyunTa->m_dHP_MainRebarEa[i]  = 0;
			continue;
		}
		str.Format("dc%d", i+1);
		m_Grid.AddCell(row, col, str);
		m_Grid.AddCell(row+1, col++, &pPile->m_pHyunTa->m_dHP_MainRebarDist[i], nFormat, nLine);
		
		str.Format("Dia%d", i+1);
		m_Grid.AddCell(row, col, str);
		m_Grid.AddCell(row+1, col++, &pPile->m_pHyunTa->m_dHP_MainRebarDia[i], nFormat, nLine, "", CELL_TYPE_DIA);
		
		str.Format("EA%d", i+1);
		m_Grid.AddCell(row, col, str);
		m_Grid.AddCell(row+1, col++, &pPile->m_pHyunTa->m_dHP_MainRebarEa[i], nFormat, nLine);

		if(i == 0)
		{
			m_Grid.AddCell(row, col, "uL");
			m_Grid.AddCell(row+1, col++, &pPile->m_pHyunTa->m_dHP_MainRebaruL, nFormat, nLine);

			for(long j = 1; j<=pPile->m_pHyunTa->m_nHP_MainRebarGuganSu; j++)
			{
				str.Format("L%d", j);
				m_Grid.AddCell(row, col, str);
				m_Grid.AddCell(row+1, col++, &pPile->m_pHyunTa->m_dHP_MainRebarLen[j-1], nFormat, nLine);

				str.Format("Dia1%d", j);
				m_Grid.AddCell(row, col, str);
				m_Grid.AddCell(row+1, col++, &pPile->m_pHyunTa->m_dHP_MainRebarDiaVar[j-1], nFormat, nLine, "", CELL_TYPE_DIA);
			}
		}
	}
	
	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(55);
}

// 현재 작업중인 말뚝을 리턴(말뚝이 없을 경우 NULL)
CExPileApp* CInputGenHyunTa::GetPile()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return NULL;

	long nIdxJijum	= m_comboFooting.GetCurSel();
	CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(nIdxJijum);
	if(!pFooting) return NULL;

	CExPileApp *pPile	= &pFooting->m_exFooting.m_Pile;
	if(!pPile->m_bIs || pFooting->m_exFooting.m_nType != EXFOOTING_TYPE_PILE) return NULL;

	return pPile;
}

void CInputGenHyunTa::OnCheckHorizontal ()
{
	// TODO: Add your control notification handler code here
	m_bViewHoriz = m_checkHoriz.GetCheck();
	DrawInputDomyunView();
}

void CInputGenHyunTa::OnSelchangeComboDan() 
{
	// TODO: Add your control notification handler code here
	CExPileApp *pPile	= GetPile();
	if(pPile)
	{
		pPile->m_pHyunTa->m_nHP_MainRebarDanSu = m_comboDan.GetCurSel()+1;
		SetGridData();
		SetDataInit();
		DrawInputDomyunView(FALSE);
	}
}

BOOL CInputGenHyunTa::IsValid()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	int nSizeBri = pStd->GetBridgeSize();
	BOOL bHyunta	= FALSE;
	for(int nBr=0; nBr<nSizeBri; nBr++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(nBr);

		long nCountFooting	= pBri->GetCountFooting();
		long footing = 0; for(footing = 0; footing < nCountFooting; footing++)
		{
			CFootingApp *pFooting	= pBri->GetFooting(footing);
			if(!pFooting) continue;
			if(!pFooting->m_bIs) continue;
			if(pFooting->m_exFooting.m_nType != EXFOOTING_TYPE_PILE) continue;
			CExPileApp *pPile	= &pFooting->m_exFooting.m_Pile;
			if(pPile->m_nType == PILE_TYPE_HYUNJANG)
			{
				bHyunta	= TRUE;
				break;
			}
		}

		if(bHyunta) break;
	}

	return bHyunta;
}
void CInputGenHyunTa::OnSelchangeComboGugan() 
{
	// TODO: Add your control notification handler code here
	CExPileApp *pPile	= GetPile();
	m_nGugan = m_comboGugan.GetCurSel();
	if(pPile)
	{
		// 변화 구간수 저장...
		pPile->m_pHyunTa->m_nHP_MainRebarGuganSu = m_comboGugan.GetCurSel();
		SetGridData();
		SetDataInit();
		DrawInputDomyunView(FALSE);
	}
}

void CInputGenHyunTa::SetGridDataCrossTie()
{
	CExPileApp *pPile	= GetPile();
	if(!pPile) return;

	CStringArray sArr;
	CString str = _T("");
	CString strHelp = _T("");
	long row = 0;
	long col = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;

	m_Grid.SetGridDefault(2, 2, 1, 0);
	m_Grid.SetCellSize(20, -1);

	m_Grid.AddCell(row, col, "Dia");
	m_Grid.AddCell(row+1, col++, &pPile->m_pHyunTa->m_dHP_CrossTieDia, nFormat, nLine, "", CELL_TYPE_DIA);

	m_Grid.AddCell(row, col, "C1");
	m_Grid.AddCell(row+1, col++, &pPile->m_pHyunTa->m_dHP_CrossTieC1, nFormat, nLine);

	m_Grid.AddCell(row, col, "B");
	m_Grid.AddCell(row+1, col++, &pPile->m_pHyunTa->m_dHP_CrossTieB, nFormat, nLine);

	m_Grid.AddCell(row, col, "C");
	m_Grid.AddCell(row+1, col++, &pPile->m_pHyunTa->m_dHP_CrossTieC, nFormat, nLine);

	m_Grid.AddCell(row, col, "F1");
	m_Grid.AddCell(row+1, col++, &pPile->m_pHyunTa->m_dHP_CrossTieF1, nFormat, nLine);

	m_Grid.AddCell(row, col, "E");
	m_Grid.AddCell(row+1, col++, &pPile->m_pHyunTa->m_dHP_CrossTieE, nFormat, nLine);

	m_Grid.AddCell(row, col, "F");
	m_Grid.AddCell(row+1, col++, &pPile->m_pHyunTa->m_dHP_CrossTieF, nFormat, nLine);

	m_Grid.AddCell(row, col, "CTC1");
	m_Grid.AddCell(row+1, col++, &pPile->m_pHyunTa->m_dHP_CrossTieCTC1, nFormat, nLine);

	m_Grid.AddCell(row, col, "CTC2");
	m_Grid.AddCell(row+1, col++, &pPile->m_pHyunTa->m_dHP_CrossTieCTC2, nFormat, nLine);

	m_Grid.AddCell(row, col, "CTC3");
	m_Grid.AddCell(row+1, col++, &pPile->m_pHyunTa->m_dHP_CrossTieCTC3, nFormat, nLine);

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(55);
}

void CInputGenHyunTa::SetDataStd(CARcBridgeInputStd* pInput)
{
	m_pIn = pInput;
}

void CInputGenHyunTa::OnOK()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetDataSave();	
	CXTResizeDialog::OnOK();
}

void CInputGenHyunTa::OnBnClickedButtonRecommand()
{
	SetDataDefault();
	DrawInputDomyunView(TRUE);
}
