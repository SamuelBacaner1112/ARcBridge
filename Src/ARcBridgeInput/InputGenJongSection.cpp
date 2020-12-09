// InputGenJongSection.cpp : implementation file
//

#include "stdafx.h"
#include "../../ARcBridge/ARcBridgeDoc.h"
#include "arcbridgeinput.h"
#include "InputGenJongSection.h"
#include "../ARcBridgeDBDraw/ARcBridgeDBDraw.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputGenJongSection dialog


CInputGenJongSection::CInputGenJongSection(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputGenJongSection::IDD, pParent)
	, m_dBoringEL(0)
{
	//{{AFX_DATA_INIT(CInputGenJongSection)
	m_bCheckStation = FALSE;
	m_szHyungHaEL	= _T("");
	m_szHyungHaStation = _T("");
	//}}AFX_DATA_INIT
	m_nTab = 0;
	m_nTabWallStt	= -1;
	m_nTabWallEnd	= -1;
	long i = 0; for(i = 0; i < MAX_JIJUM_COUNT_RC; i++) 
		m_nTabInWall[i]	= -1;
	m_nTabRib		= -1;
	m_nTabOutWall	= -1;
	m_nTabInWallBox	= -1;
	m_nTabLowerSlab	= -1;
	m_nTabPRF		= -1;
	m_bMove			= FALSE;
	m_nLeft = 0;
	m_bCheckViewCross	= FALSE;
	m_bCheckViewEarth	= FALSE;

	m_pProgress	= new CDlgProgress;
}

CInputGenJongSection::~CInputGenJongSection()
{
	AhTPADelete(&m_ptvArrInWall, (CTwinVectorArray*)0);
	AhTPADelete(&m_ptvArrFooting, (CTwinVectorArray*)0);
	AhTPADelete(&m_ptvArrExFooting, (CTwinVectorArray*)0);

	delete m_pProgress;
}

void CInputGenJongSection::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputGenJongSection)
	DDX_Control(pDX, IDC_CHECK_VIEW_CROSS, m_checkViewCross);
	DDX_Control(pDX, IDC_BUTTON_SAMEINWALL, m_btnSameInWall);
	DDX_Control(pDX, IDC_BUTTON_SAMESTTEND, m_btnSameSttEnd);
	DDX_Control(pDX, IDC_CHECK_VIEW_EARTH, m_checkViewEarth);
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_comboBridgeNo);
	DDX_Control(pDX, IDC_BUTTON_EXAMINE, m_btnExamine);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	DDX_Check(pDX, IDC_CHECK_STATION, m_bCheckStation);
	DDX_Text(pDX, IDC_EDIT_HYUNGHA_EL, m_szHyungHaEL);
	DDX_Text(pDX, IDC_EDIT_HYUNGHA_STATION, m_szHyungHaStation);
	DDX_Control(pDX, IDC_COMBO_BORING, m_comboBoring);
	DDX_Text(pDX, IDC_EDIT_EL, m_dBoringEL);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputGenJongSection, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputGenJongSection)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_EXAMINE, OnButtonExamine)
	ON_BN_CLICKED(IDC_CHECK_VIEW_EARTH, OnCheckViewEarth)
	ON_BN_CLICKED(IDC_BUTTON_SAMESTTEND, OnButtonSamesttend)
	ON_BN_CLICKED(IDC_BUTTON_SAMEINWALL, OnButtonSameinwall)
	ON_BN_CLICKED(IDC_CHECK_VIEW_CROSS, OnCheckViewCross)
	ON_BN_CLICKED(IDC_CHECK_STATION, OnCheckStation)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
	ON_EN_KILLFOCUS(IDC_EDIT_HYUNGHA_STATION, OnKillfocusEditHyunghaStation)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW,  IDC_GRID, OnCellChangedRow)
	ON_CBN_SELCHANGE(IDC_COMBO_BORING, OnSelchangeComboBoring)
	ON_EN_KILLFOCUS(IDC_EDIT_EL, OnEnKillfocusEditEl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputGenJongSection message handlers
void CInputGenJongSection::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	
	return;
}

void CInputGenJongSection::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	SetFocus();
	m_nTab = m_Tab.GetCurSel();
	SetGridData();
	SetDataInit();
	SetDataInitBoring(TRUE);
	DrawInputDomyunView(m_checkViewCross.GetCheck() ? TRUE : FALSE);

	return;
}

void CInputGenJongSection::OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result)
{
	if(m_nTab == m_nTabRib)
		DrawInputDomyunView(FALSE);

	if(m_nTab == m_nTabOutWall || m_nTab == m_nTabInWallBox)
		DrawInputDomyunView(FALSE);
}


void CInputGenJongSection::SetGridData()
{
	m_Grid.SetGridDefault(0, 0, 0, 0);
	if(m_nTab == m_nTabWallStt)	SetGridDataSttWall();
	else if(m_nTab == m_nTabWallEnd) SetGridDataEndWall();
	else if(m_nTab == m_nTabRib) SetGridDataRib();
	else if(m_nTab == m_nTabPRF) SetGridDataPRF();
	else if(m_nTab == m_nTabOutWall) SetGridDataOutWallBox();
	else if(m_nTab == m_nTabInWallBox) SetGridDataInWallBox();
	else if(m_nTab == m_nTabLowerSlab) SetGridDataLowerSlab();
	else 
	{
		long i = 0; for(i = 0; i < MAX_JIJUM_COUNT_RC; i++)
			if(m_nTab == m_nTabInWall[i]) SetGridDataInWall(i);
	}
}

void CInputGenJongSection::SetDataSave()
{
	m_Grid.UpdateData(TRUE);

	CRcBridgeApp *pBri = m_pIn->m_pARcBridgeDataStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	
//	pBri->SyncBridgeBoxType(TRUE);

	// 상하행선 분리 적용
	m_pIn->m_pARcBridgeDataStd->CopyToCoupleBridge_VertSection(m_pIn->m_nCurrentBridge);

	m_pIn->SetDialogEnable();
	SetDataInitBoring();

	if(pBri->IsBoxType() && pBri->m_bSlopeLowerSlab == FALSE)
	{
		pBri->m_nTypeJongdanForRebar = 1;
	}

	long nCount = m_pIn->GetRcBridge()->GetCountInWall();
	//		long nIdxHunch = 0;
	for(long ix = 0; ix < nCount; ++ix)
	{
		if(m_nTabInWall[ix] == m_nTab)
		{
			CWallApp *pWall = pBri->GetInWall(ix);
			if(pWall)
			{
 				if(pWall->m_dW > (pBri->m_dWL[ix][iUPPER] + pBri->m_dWR[ix][iUPPER]))
 				{
					AfxMessageBox(_T("수평구간폭(WL, WR)이 벽체폭(W)보다 작습니다.\n수평구간폭(WL, WR)을 벽체폭(W)보다 같거나 크게 입력하여 주시기 바랍니다."), MB_OK | MB_ICONWARNING);
 				}
			}
		}
	}
}

/*
#define DEFAULT_GENJONGSECTION_STT		0x00000001
#define DEFAULT_GENJONGSECTION_END		0x00000002
#define DEFAULT_GENJONGSECTION_INWALL	0x00000004
*/

void CInputGenJongSection::SetDataDefault()
{
	for(long bri = 0; bri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		CRcBridgeApp *pBri = m_pIn->m_pARcBridgeDataStd->GetBridge(bri);
		if(!pBri) continue;

		BOOL bFirst	= IsFirst(bri);
		if(!bFirst)
		{
			if(IsClickDefaultButton(bri))
			{
				if(!IsDefaulted(bri, DEFAULT_GENJONGSECTION_STT) || (m_nTab == m_nTabWallStt) || (m_nTab == m_nTabOutWall && GetIdxCurOutWall() == 0)) 
				{
					pBri->SetDataDefaultShapeBridge(TRUE);
					// Box타입이면 기초는 하부슬래브에서
					if(m_nTab != m_nTabOutWall) 
						pBri->SetDataDefaultShapeFooting(&pBri->m_footingStt, pBri->m_dWS, pBri->m_dStationBridgeStt);
				}
				else if(!IsDefaulted(bri, DEFAULT_GENJONGSECTION_END) || (m_nTab == m_nTabWallEnd) || (m_nTab == m_nTabOutWall && GetIdxCurOutWall() == 1)) 
				{
					pBri->SetDataDefaultShapeBridge(FALSE);
					// Box타입이면 기초는 하부슬래브에서
					if(m_nTab != m_nTabOutWall) 
						pBri->SetDataDefaultShapeFooting(&pBri->m_footingEnd, pBri->m_dWE, pBri->m_dStationBridgeStt+pBri->m_dLengthBridge);
				}
				else if(!IsDefaulted(bri, DEFAULT_GENJONGSECTION_ARCHRIB) || (m_nTab == m_nTabRib))
				{
					pBri->SetDataDefaultShapeRib();
				}
				else if(!IsDefaulted(bri, DEFAULT_GENJONGSECTION_PRF) || (m_nTab == m_nTabPRF))
				{
					pBri->SetDataDefaultPRF();
				}
				else if(!IsDefaulted(bri, DEFAULT_GENJONGSECTION_LOWERSlAB) || (m_nTab == m_nTabLowerSlab))
				{
					pBri->SetDataDefaultBoxLowerSlab();
				}
				else 
				{
					long i = 0; for(i = 0; i < pBri->GetCountInWall(); i++)
					{
						CWallApp *pWall	= pBri->GetInWall(i);
						if(pWall)
						{
							if(!IsDefaulted(bri, DEFAULT_GENJONGSECTION_INWALL) || (m_nTab == m_nTabInWall[i]) || (m_nTab == m_nTabInWallBox && GetIdxCurInWall() == i))
							{
								pBri->SetDataDefaultShapeInWall(i);
								// Box타입이면 기초는 하부슬래브에서
								if(m_nTab == m_nTabInWallBox)
									pBri->SetDataDefaultShapeFooting(&pWall->m_footing, pWall->m_dW, pBri->GetStationOnJijum(i+1, 0), i);
							}
						}
					}
				}


				// 상하행선 분리 적용
				m_pIn->m_pARcBridgeDataStd->CopyToCoupleBridge_VertSection(m_pIn->m_nCurrentBridge);

				pBri->m_bNeedSettingVertSection	= TRUE;
				
				long nIdxMatchBri	= m_pIn->m_pARcBridgeDataStd->GetIdxMatchBridge(m_pIn->m_nCurrentBridge);
				if(nIdxMatchBri > -1)
				{
					CRcBridgeRebar *pBriMatch	= m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(nIdxMatchBri);
					if(pBriMatch)
						pBriMatch->m_bNeedSettingVertSection	= TRUE;
				}

				// 지반정보 ARCBRIDGE-2591
				CFootingApp *pFooting = GetCurrentFooting();
				if(pFooting)
				{
					long nBoringIdx = pBri->GetBoringIndexByStation(pFooting->m_dSta);
					pFooting->SetBoringData(0, nBoringIdx);
				}
			}
		}
		else
		{
			pBri->SetDataDefaultShapeStd(0, FALSE);
			pBri->SetDefaultedState(GetIDD(), DEFAULT_GENJONGSECTION_STT|DEFAULT_GENJONGSECTION_END|DEFAULT_GENJONGSECTION_INWALL|DEFAULT_GENJONGSECTION_ARCHRIB|DEFAULT_GENJONGSECTION_PRF|DEFAULT_GENJONGSECTION_LOWERSlAB);

			pBri->m_bNeedSettingVertSection	= TRUE;
		}
	}

	SetGridData();
	SetDataInit();
}

void CInputGenJongSection::InitTabCtrl()
{
	m_Tab.RemoveAllTabs();
	m_Tab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_Tab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	
	
	long nTab = 0;
	m_nTabWallStt = -1;
	m_nTabWallEnd = -1;
	long i = 0; for(i = 0; i < MAX_JIJUM_COUNT_RC; i++)
		m_nTabInWall[i] = -1;
	m_nTabRib = -1;
	m_nTabOutWall	= -1;
	m_nTabInWallBox	= -1;
	m_nTabLowerSlab	= -1;
	m_nTabPRF = -1;
	m_sArrTabName.RemoveAll();
	
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();

	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		m_sArrTabName.Add("시종점부"); m_nTabOutWall = nTab++;
		if(pBri->m_nQtyJigan > 1)
		{
			m_sArrTabName.Add("중간지점부"); m_nTabInWallBox = nTab++;
		}
		m_sArrTabName.Add("하부슬래브"); m_nTabLowerSlab = nTab++;
		m_sArrTabName.Add("부상방지저판"); m_nTabPRF = nTab++;
	}
	else
	{
		m_sArrTabName.Add("시점부"); m_nTabWallStt = nTab++;
		if(pBri->GetCountInWall() > 0 && pBri->m_nCountInWallHunch > 0)
		{
			CString str = _T("");
			long i = 0; for(i = 0; i < pBri->GetCountInWall(); i++)
			{
				if(pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB) str.Format("중간 지점부 %d", i+1);
				else str.Format("중간 지점부 %d", i+1);
				m_sArrTabName.Add(str); m_nTabInWall[i] = nTab++;
			}
		}
		m_sArrTabName.Add("종점부"); m_nTabWallEnd = nTab++;

		if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		{
			m_sArrTabName.Add("아치리브"); m_nTabRib = nTab++;
		}
	}

	for(i = 0; i < m_sArrTabName.GetSize(); i++)
		m_Tab.InsertItem(i, m_sArrTabName[i]);

	if(m_Tab.GetItemCount() <= m_nTab) m_nTab = 0;
	m_Tab.SetCurSel(m_nTab);
}

void CInputGenJongSection::OnPreInitDialog()
{
	// 시작하면서 pick box의 크기를 정해줌
// 	if(m_pView) 
// 	{
// 		CTwinVector tv;
// 		tv.SetPickBoxSize(m_pView->GetSelectionRectSize()/m_pView->GetRatioWindow());
// 	}

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	HGBOOL bLSD = pStd->IsLsdDesign();

	InitTabCtrl();
	InitComboCtrl();


	SetResize(IDC_BUTTON_EXAMINE,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_SAMESTTEND,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_SAMEINWALL,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_TAB,			   SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID,			   SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_CHECK_VIEW_EARTH,		SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_VIEW_CROSS,		SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_VIEW,		SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	//
	SetResize(IDC_STATIC_HYUNGHA,		SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_STATION,		SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_HYUNGHA_STATION,	SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_HYUNG_EL,		SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_HYUNGHA_EL,		SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	//
	SetResize(IDC_EDIT_EL,		SZ_BOTTOM_RIGHT,	SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_EL_UNIT,		SZ_BOTTOM_RIGHT,	SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_EL,		SZ_BOTTOM_RIGHT,	SZ_BOTTOM_RIGHT);
	SetResize(IDC_COMBO_BORING,		SZ_BOTTOM_RIGHT,	SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_GROUND,		SZ_BOTTOM_RIGHT,	SZ_BOTTOM_RIGHT);

	SetHyperHelp(m_pIn->GetManualPath());

	m_Grid.InitGrid(m_pIn->m_pARcBridgeDataStd->m_Fy);
	GetDlgItem(IDC_EDIT_HYUNGHA_STATION)->EnableWindow(m_bCheckStation);
	SetTextHyungHaElevation();
	
	SetGridData();
}

BOOL CInputGenJongSection::PreTranslateMessage(MSG* pMsg) 
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

void CInputGenJongSection::InitComboCtrl()
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

	// 지반정보
	long nSizeBoring = pStd->m_pArrBoringEx.GetSize();
	for (HGINT32 ix = 0; ix < nSizeBoring; ++ix)
	{
		CBoringDataEx *pBoringEx = pStd->m_pArrBoringEx.GetAt(ix);
		if(pBoringEx->m_pArrBoring.GetSize()>0)
		{
			CString str = pBoringEx->m_sNameBoring;
			m_comboBoring.AddString(str);
		}

	}

// 	CFootingApp *pFooting = GetCurrentFooting();
// 	if( NULL == pFooting ) return;
// 
// 	SetDataInitBoring(TRUE);

//	m_comboBoring.SetCurSel(0);

	SetControl();
	SetDataInitBoring(TRUE);
}

void CInputGenJongSection::OnSelchangeComboBridgeNo() 
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

void CInputGenJongSection::DrawInputDomyunView(BOOL bZoomAll)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	pBri->m_bSettingPlane	= FALSE;
	pBri->m_bNeedSettingPlane	= TRUE;

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);	
	pDom->SetScaleDim(m_pIn->GetScaleInputDialog());

	CDomyun Dom(pDom);

	CARcBridgeDBStandard DBStd(m_pIn->GetRcBridgeRebar(), m_pIn->m_pARoadOptionStd, m_pIn->m_pARcBridgeDataStd);

	BOOL bInput			= FALSE;
	BOOL bCuttingPile	= !m_checkViewEarth.GetCheck();

	// 말뚝 비율을 기본값으로
	pBri->m_footingEnd.m_exFooting.m_Pile.InitScaleCutting();

	// 모든 객체들의 동기화를 해줌.
	// 일반도는 무조건 선형따라 그림 /////////////////////////////
	long nOldTypeJongDanRebar	= pBri->m_nTypeJongdanForRebar;
	pBri->m_nTypeJongdanForRebar	= 2;
	////////////////////////////////////////////////////////////////
	m_nLeft	= m_pIn->GetLeft();
	
#ifdef _DEBUG

	/*
	CHgBaseDrawStd baseDraw;
	CTwinVectorArray tvArrUpper, tvArrLeft, tvArrRight, tvArrMid, tvArrArch, tvArrMid2;
	pBri->GetTvPostJigan(tvArrUpper, tvArrLeft, tvArrRight, tvArrMid, tvArrArch, tvArrMid2, 1);
	baseDraw.DrawTvArray(&Dom, tvArrMid2, TRUE, TRUE);

	CTwinVectorArray tvArrHid;
	CRebarPlacing rb;
	rb.GetTvArrayHatch(tvArrMid, tvArrHid, 500, 45);
	baseDraw.DrawTvArray(&Dom, tvArrHid, TRUE, TRUE);
	*/
#endif

	// 횡단보기
	// mass입력용이므로 mass에 대해서만 치수기입을 한다.
	pBri->m_bSettingVertSection	= FALSE;
	//m_pIn->StartSpeedTest();
	SyncBridge(m_nLeft, TRUE, TRUE);
	//m_pIn->EndSpeedTest();

	if(m_bCheckViewCross)
	{
		if(m_nTab != m_nTabRib)
		{
			long nJ	= m_nTab;
			DBStd.m_pDrawCrossSection->DrawCrossAllAtJijum(&Dom, nJ, TRUE, TRUE, TRUE);
			if(nJ == 0 || nJ == pBri->m_nQtyJigan)
			{
				BOOL bInnerAtLeft	= pBri->m_nTypeWallForRebarGen==0 ? FALSE : TRUE;
				BOOL bInnerAtRight	= bInnerAtLeft;
				DBStd.m_pDrawCrossSection->DrawCrossSection_OutWall_Exp(&Dom, nJ==0, bInnerAtLeft, bInnerAtRight);
				DBStd.m_pDrawCrossSection->DimFrontBottom(&Dom, nJ, 0, TRUE);	// 하면 치수
			}
		}
		else
		{
			DBStd.m_pDrawCrossSection->DrawCrossSection_ArchRibStd(&Dom, TRUE);
		}
	}
	// 종단 보기
	else
	{
		long i(0);
		long nJ	= m_nTab;
		// 종단면 그림		
		DBStd.m_pDrawVertSection->DrawVertSection(&Dom, TRUE, &m_tvArrBridge);
		DBStd.m_pDrawVertSection->DrawVertSectionCenterLine(&Dom, TRUE, TRUE, TRUE, nJ);
		DBStd.m_pDrawVertSection->DrawVertSectionAbut(&Dom);
		DBStd.m_pDrawVertSection->DrawVertSectionPier(&Dom);
		DBStd.m_pDrawVertSection->DrawVertSectionArchHunchHidden(&Dom);


		if(pBri->IsBoxType())
		{
			DBStd.m_pDrawExFooting->DrawFront(&Dom, &pBri->m_footingBox.m_exFooting, FALSE, bCuttingPile, &m_tvArrExFootingEnd);
			DBStd.m_pDrawVertSection->DrawExFootingBoxType(&Dom, TRUE);
		}
		else
		{
			DBStd.m_pDrawFooting->DrawFront(&Dom, &pBri->m_footingStt, &m_tvArrFootingStt);
			DBStd.m_pDrawFooting->DrawFront(&Dom, &pBri->m_footingEnd, &m_tvArrFootingEnd);
			DBStd.m_pDrawExFooting->DrawFront(&Dom, &pBri->m_footingStt.m_exFooting, FALSE, bCuttingPile, &m_tvArrExFootingStt);
			DBStd.m_pDrawExFooting->DrawFront(&Dom, &pBri->m_footingEnd.m_exFooting, FALSE, bCuttingPile, &m_tvArrExFootingEnd);
		}


		for(long stt = 0; stt < 2; stt++)
		{
			DBStd.m_pDrawVertSection->DrawVertSectionFlexibleJoint(&Dom, stt==iSTT);
		}

		if(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 || pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		{
			if(m_nTab != m_nTabRib)
			{
				DBStd.m_pDrawFooting->DimFrontSide(&Dom, &pBri->m_footingStt, 0, TRUE, m_nTab==m_nTabWallStt, FALSE);
				DBStd.m_pDrawFooting->DimFrontSide(&Dom, &pBri->m_footingEnd, 0, TRUE, m_nTab==m_nTabWallEnd, FALSE);
			}
		}
		
		long nCount = m_pIn->GetRcBridge()->GetCountInWall();
//		long nIdxHunch = 0;
		for(i = 0; i < nCount; i++)
		{
			bInput = (m_nTab == m_nTabInWall[i]);//
			CWallApp *pWall = m_pIn->GetRcBridge()->GetInWall(i);
			
			DBStd.m_pDrawWall->DrawFront(&Dom, pWall, TRUE, m_ptvArrInWall.GetAt(i));
			DBStd.m_pDrawFooting->DrawFront(&Dom, &pWall->m_footing, m_ptvArrFooting.GetAt(i));
			DBStd.m_pDrawExFooting->DrawFront(&Dom, &pWall->m_footing.m_exFooting, FALSE, bCuttingPile, m_ptvArrExFooting.GetAt(i));
			
			if(!m_bMove || m_ptvArrFooting.GetAt(i)->IsSelect() || m_ptvArrExFooting.GetAt(i)->IsSelect() || m_ptvArrInWall.GetAt(i)->IsSelect())
			{
				if(m_nTab != m_nTabRib && i+1 != pBri->GetJijumConnectRibAndSlab())
				{
					DBStd.m_pDrawWall->DimLeftFront(&Dom, pWall);
					DBStd.m_pDrawWall->DimMidFront(&Dom, pWall, bInput);

					DBStd.m_pDrawExFooting->DimFrontLower(&Dom, &pWall->m_footing.m_exFooting, 0, bInput);
					BOOL bLeft	= FALSE;
					if(pWall->m_footing.m_nType == FOOTING_TYPE_RIGHTSLOPE)
						bLeft	= TRUE;
					DBStd.m_pDrawExFooting->DimFrontSide(&Dom, &pWall->m_footing.m_exFooting, bLeft, 0, bInput);

					DBStd.m_pDrawFooting->DimTextFrontEL(&Dom, &pWall->m_footing, FALSE, bInput);
					DBStd.m_pDrawFooting->DimFrontUpper(&Dom, &pWall->m_footing, 0, bInput);
					DBStd.m_pDrawFooting->DimFrontSide(&Dom, &pWall->m_footing, 0, TRUE, bInput);
				}
			}
		}

		// 종단면 치수
		if(!m_bMove || m_tvArrBridge.IsSelect() || m_tvArrFootingStt.IsSelect() || m_tvArrExFootingStt.IsSelect() || 
			m_tvArrFootingEnd.IsSelect() || m_tvArrExFootingEnd.IsSelect())
		{
			if(m_nTab != m_nTabRib)
			{
				BOOL bInputStt = (m_nTab==m_nTabWallStt || (m_nTab==m_nTabOutWall && GetIdxCurOutWall() == 0))? TRUE : FALSE;
				BOOL bInputEnd = (m_nTab==m_nTabWallEnd || (m_nTab==m_nTabOutWall && GetIdxCurOutWall() == 1))? TRUE : FALSE;
				// 교량
 				DBStd.m_pDrawVertSection->DimVertSectionJongSlope(&Dom);
 				DBStd.m_pDrawVertSection->DimTextVerSectionEL(&Dom);
 				DBStd.m_pDrawVertSection->DimVertSectionUpper(&Dom, 1, bInputStt, m_nTab==m_nTabWallStt||m_nTab==m_nTabWallEnd||m_nTab==m_nTabOutWall);
 				DBStd.m_pDrawVertSection->DimVertSectionUpperSlabThick(&Dom, TRUE, 0);
				DBStd.m_pDrawVertSection->DimVerSectionHunch(&Dom, 0, TRUE, bInputStt);
				DBStd.m_pDrawVertSection->DimVerSectionHunch(&Dom, 0, FALSE, bInputEnd);

				DBStd.m_pDrawVertSection->DimVerSectionBracketHunch(&Dom, 0, TRUE, bInputStt);
				DBStd.m_pDrawVertSection->DimVerSectionBracketHunch(&Dom, 0, FALSE, bInputEnd);
				DBStd.m_pDrawVertSection->DimVertSectionPRF(&Dom, m_nTab==m_nTabPRF);

				if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
				{
					DBStd.m_pDrawVertSection->DimVerSectionBoxSideStt(&Dom, 0, m_nTab);
					DBStd.m_pDrawVertSection->DimVerSectionBoxSideEnd(&Dom, 0, bInputEnd);

					DBStd.m_pDrawVertSection->DimVerSectionHunch(&Dom, 0, TRUE, bInputStt, FALSE);
					DBStd.m_pDrawVertSection->DimVerSectionHunch(&Dom, 0, FALSE, bInputEnd, FALSE);
					DBStd.m_pDrawExFooting->DimFrontLowerBox(&Dom, pBri, 0, m_nTab==m_nTabLowerSlab);
					DBStd.m_pDrawVertSection->DimVertSectionLowerSlab(&Dom, 0, TRUE, TRUE);
				}
				
				if(pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
				{
					DBStd.m_pDrawVertSection->DimVerSectionSideStt(&Dom, 0, bInputStt);
					DBStd.m_pDrawVertSection->DimVerSectionSideEnd(&Dom, 0, bInputEnd);
					// 기초
					DBStd.m_pDrawFooting->DimTextFrontEL(&Dom, &pBri->m_footingStt, FALSE, bInputStt);
					DBStd.m_pDrawFooting->DimTextFrontEL(&Dom, &pBri->m_footingEnd, TRUE, bInputEnd);

					DBStd.m_pDrawFooting->DimFrontUpper(&Dom, &pBri->m_footingStt, 0, m_nTab==m_nTabWallStt, FALSE);
					DBStd.m_pDrawFooting->DimFrontUpper(&Dom, &pBri->m_footingEnd, 0, m_nTab==m_nTabWallEnd, TRUE);

					// 확대기초
					DBStd.m_pDrawExFooting->DimFrontLower(&Dom, &pBri->m_footingStt.m_exFooting, 0, m_nTab==m_nTabWallStt);
					//DBStd.m_pDrawExFooting->DimFrontSide(&Dom, &pBri->m_footingStt.m_exFooting, FALSE, 0, m_nTab==m_nTabWallStt);
					DBStd.m_pDrawExFooting->DimFrontLower(&Dom, &pBri->m_footingEnd.m_exFooting, 0, m_nTab==m_nTabWallEnd);
					//DBStd.m_pDrawExFooting->DimFrontSide(&Dom, &pBri->m_footingEnd.m_exFooting, TRUE, 0, m_nTab==m_nTabWallEnd);
				}

				// 내측헌치
				long i = 0; for(i = 0; i < nCount; i++)
				{
					bInput = (m_nTab == m_nTabInWall[i] || (m_nTab == m_nTabInWallBox && GetIdxCurInWall() == i));
					CWallApp *pWall = m_pIn->GetRcBridge()->GetInWall(i);

					long nIdxHunch = pBri->GetIdxHunchInWall(i, TRUE);
					if(nIdxHunch > -1) 
					{
						DBStd.m_pDrawVertSection->DimVerSectionInHunch(&Dom, 0, nIdxHunch, bInput);
						if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
							DBStd.m_pDrawVertSection->DimVerSectionInHunch(&Dom, 0, nIdxHunch, bInput ,FALSE);

						if(pWall->m_nType == WALL_TYPE_V)
							DBStd.m_pDrawVertSection->DimVerSectionInHunch(&Dom, 0, nIdxHunch+1, bInput);
					}
				}
			}

			// 리브 정보 출력
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
				DBStd.m_pDrawVertSection->DimTextRibInfo(&Dom);
		}
		
		// 지반선 그림
		if(m_bCheckViewEarth)
		{
			CLineInfo *pLine	= pBri->GetLineBase();
			double dStationStt = max(pBri->m_dStationBridgeStt - 10000, pLine->m_dStationBp);
			double dStationEnd = min(pBri->m_dStationBridgeStt + pBri->m_dLengthBridge +  10000, pLine->GetStationEpLine());
			CDomyun DomBoring(pDom);
			DBStd.m_pDrawVertSection->DrawElEarthLine(&Dom,dStationStt,dStationEnd,TRUE,FALSE,FALSE, 0.,m_nTab);
		}

		// 아치리브 치수
		if(m_nTab == m_nTabRib)
		{
			CCellID cell	= m_Grid.GetFocusCell();
			long nUpper	= 0;
			switch(cell.row)
			{
			case 1: nUpper	= 1; break;
			case 2: nUpper	= 0; break;
			case 3: nUpper	= -1; break;
			}

			DBStd.m_pDrawVertSection->DimVertSectionArchRiblAndf(&Dom, nUpper, TRUE);
		}
	}
	
	*pDom << Dom;
	pBri->m_nTypeJongdanForRebar	= nOldTypeJongDanRebar;

	m_pIn->DrawMarginPoint(pDom, 40.0);

	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

void CInputGenJongSection::SetDataInit()
{
	m_Grid.UpdateData(FALSE);
	SetDataInitBoring(TRUE);
}


void CInputGenJongSection::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	structGridCell *pCell = m_Grid.GetCurrentCell();
	if(pCell)
	{
		m_pIn->SetComment(pCell->sTitle);
		
		CString sTitle	= m_Grid.GetItemText(0,pCell->nCol);
		CString sMark	= sTitle;

		if(m_nTab == m_nTabWallStt || m_nTab == m_nTabWallEnd || m_nTab == m_nTabOutWall)
		{
			if(sTitle == "HH" || sTitle == "HW") sMark	= "HHxHW";
			if(sTitle == "WHW" || sTitle == "WHH") sMark	= "WHHxWHW";
			if(sTitle == "LHH" || sTitle == "LHW") sMark	= "LHHxLHW";
		}
		else
		{
			long i = 0; for(i = 0; i < MAX_JIJUM_COUNT_RC; i++)
			{
				if(m_nTab == m_nTabInWall[i] || (m_nTab == m_nTabInWallBox))
				{
					if(sTitle == "HLW" || sTitle == "HHL")
					{
						sMark	= "HHLxHLW";
						break;
					}
					else if(sTitle == "HRW" || sTitle == "HHR")
					{
						sMark	= "HHRxHRW";
						break;
					}
					else if(sTitle == "LLW" || sTitle == "LLH")
					{
						sMark	= "LLHxLLW";
						break;
					}
					else if(sTitle == "LRW" || sTitle == "LRH")
					{
						sMark	= "LRHxLRW";
						break;
					}
					break;
				}
			}
		}

		// el 선택시 EL. 까지만 찍어줌
		if(sTitle.Find("EL", 0) != -1)
		{
			sMark	= "EL.";
		}

		m_pView->GetDomyun()->STMakeCursor(sMark);
	}
}

void CInputGenJongSection::SetControl()
{
	// 내측벽체헌치
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	CString str = _T("");


	// 내측벽체
	long nCount = pBri->GetCountInWall();

	// 교량을 바꾸면 real stretch용 변수들을 초기화 해줘야 된다. ////////
	// 내측벽체 좌표 정보변수 생성
	AhTPADelete(&m_ptvArrInWall, (CTwinVectorArray*)0);
	long i = 0; for(i = 0; i < nCount; i++)
	{
		m_ptvArrInWall.Add(new CTwinVectorArray);
	}

	// 기초 좌표 정보변수 생성(기초는 항상 벽체만큼 생성)
	AhTPADelete(&m_ptvArrFooting, (CTwinVectorArray*)0);
	for(i = 0; i < nCount; i++)
	{
		m_ptvArrFooting.Add(new CTwinVectorArray);
	}

	// 확대기초 좌표 정보변수 생성(확대기초는 항상 벽체 만큼 생성)
	AhTPADelete(&m_ptvArrExFooting, (CTwinVectorArray*)0);
	for(i = 0; i < nCount; i++)
	{
		m_ptvArrExFooting.Add(new CTwinVectorArray);
	}

	
	m_tvArrBridge.RemoveAll();
	m_tvArrFootingStt.RemoveAll();
	m_tvArrFootingEnd.RemoveAll();
	m_tvArrExFootingStt.RemoveAll();
	m_tvArrExFootingEnd.RemoveAll();
	////////////////////////////////////////////////////////////////////


	if(nCount <= 1)
		m_btnSameInWall.EnableWindow(FALSE);
	else
		m_btnSameInWall.EnableWindow(TRUE);

	m_checkViewCross.SetCheck(m_bCheckViewCross);
	m_checkViewEarth.SetCheck(m_bCheckViewEarth);
}

// 시점측 벽체
void CInputGenJongSection::SetGridDataSttWall()
{
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 1, 0);
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	CDPointArray xyArr;	// 배경색 반전위치
	CStringArray sArr;
	CString str = _T("");
	CString strHelp = _T("");
	long row = 0;
	long col = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;
	
	CFootingApp *pFooting = &pBri->m_footingStt;
	BOOL bWall	= pBri->IsOutWall(TRUE);
		
	long nColOfFooting	= -1;
	long nColOfDan		= -1;

	if(bWall)
	{
		m_Grid.AddCell(row, col, "EL.(m)"); m_Grid.AddCell(row+1, col++, &pBri->m_dELSB, nFormat, nLine, "시점측 기초 하단 EL.", CELL_TYPE_METER); m_Grid.SetCellDec(CELL_DEC_3);
		m_Grid.AddCell(row, col, "BH"); m_Grid.AddCell(row+1, col++, &pBri->m_dBHS);
		m_Grid.AddCell(row, col, "BTH"); m_Grid.AddCell(row+1, col++, &pBri->m_dBTHS, nFormat);
		//m_Grid.AddCell(row, col, "BW"); m_Grid.AddCell(row+1, col++, &pBri->m_dBWS, nFormat, CELL_READONLY);
		m_Grid.AddCell(row, col, "BHH"); m_Grid.AddCell(row+1, col++, &pBri->m_dBHHS);
		m_Grid.AddCell(row, col, "BHW"); m_Grid.AddCell(row+1, col++, &pBri->m_dBHWS);
	}
	else
	{
		// BLOCK OUT
		if(pBri->m_bExpJoint[0])
		{
			m_Grid.AddCell(row, col, "FXW"); m_Grid.AddCell(row+1, col++, &pBri->m_fxJointStt.m_dWidth, nFormat, nLine, "시점측 BLOCK OUT 폭");
			m_Grid.AddCell(row, col, "FXH"); m_Grid.AddCell(row+1, col++, &pBri->m_fxJointStt.m_dHeight, nFormat, nLine, "시점측 BLOCK OUT 높이");
		}
	}

	m_Grid.AddCell(row, col, "HH"); m_Grid.AddCell(row+1, col++, &pBri->m_dHHS);
	m_Grid.AddCell(row, col, "HW"); m_Grid.AddCell(row+1, col++, &pBri->m_dHWS);
	m_Grid.AddCell(row, col, "HR"); m_Grid.AddCell(row+1, col++, &pBri->m_dHRS);	// 헌치 R
	sArr.RemoveAll();
	sArr.Add("접선");
	sArr.Add("2P");
	m_Grid.AddCell(row, col, "HR(T)"); m_Grid.AddCell(row+1, col++, &pBri->m_nTypeRoundOfHunchStt, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);

	m_Grid.AddCell(row, col, "W"); m_Grid.AddCell(row+1, col++, &pBri->m_dWS, nFormat, nLine, "시점벽체 폭");
	
	if(bWall)
	{
		if(pBri->m_nTypeBridge != BRIDGE_TYPE_BOX) 
		{
			m_Grid.AddCell(row, col, "FB"); m_Grid.AddCell(row+1, col++, &pFooting->m_dFWL);
			m_Grid.AddCell(row, col, "FF"); m_Grid.AddCell(row+1, col++, &pFooting->m_dFWR);
			m_Grid.AddCell(row, col, "FH"); m_Grid.AddCell(row+1, col++, &pFooting->m_dFH);
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 || pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
			{
				m_Grid.AddCell(row, col, "WHW"); m_Grid.AddCell(row+1, col++, &pBri->m_dWSHW, nFormat, nLine, "시점벽체 헌치 폭");
				m_Grid.AddCell(row, col, "WHH"); m_Grid.AddCell(row+1, col++, &pBri->m_dWSHH, nFormat, nLine, "시점벽체 헌치 높이");

				m_Grid.AddCell(row, col, "FSW"); m_Grid.AddCell(row+1, col++, &pFooting->m_dFSRW, nFormat, nLine, "시점기초 경사 폭");
				m_Grid.AddCell(row, col, "FSH"); m_Grid.AddCell(row+1, col++, &pFooting->m_dFSRH, nFormat, nLine, "시점기초 경사 높이");
				m_Grid.AddCell(row, col, "FEW"); m_Grid.AddCell(row+1, col++, &pFooting->m_dFERW, nFormat, nLine, "시점기초 경사 아래 확장 폭");
			}
		}
		else
		{
			m_Grid.AddCell(row, col, "FB"); m_Grid.AddCell(row+1, col++, &pBri->m_dFLS, nFormat, nLine, "시점기초 폭");
			m_Grid.AddCell(row, col, "FH"); m_Grid.AddCell(row+1, col++, &pBri->m_dFHS, nFormat, nLine, "시점기초 높이");

		}
		
		// 확대기초타입
		sArr.RemoveAll();
		sArr.Add("직접기초");
		sArr.Add("MASS기초");
		sArr.Add("말뚝기초");
		
		nColOfFooting = col;
		nColOfDan	= -1;
		m_Grid.AddCell(row, col, "기초형식"); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_nType, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
		if(pFooting->m_exFooting.m_bIs)
		{
			if(pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_MASS)
			{
				nColOfDan = col;
				sArr.RemoveAll();
				long i = 0; for(i = 0; i < MAX_MASS_DAN; i++)
				{
					if(i == 0) str = "단없음";
					else str.Format("%d단", i);
					sArr.Add(str);
				}
				m_Grid.AddCell(row, col, "MASS단수"); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_nCountDan, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);

				sArr.RemoveAll();
				sArr.Add("교축");
				sArr.Add("교축직각");
				m_Grid.AddCell(row, col, "단차방향"); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_bFrontDanMass, nFormat, nLine, "", CELL_TYPE_COMBO, sArr);

				sArr.RemoveAll();
				sArr.Add("좌측");
				sArr.Add("우측");
				m_Grid.AddCell(row, col, "MASS기준면"); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_bLeftOrgOfMass, nFormat, nLine, "", CELL_TYPE_COMBO, sArr);
			}
			m_Grid.AddCell(row, col, "EFL"); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_dEFWL);
			m_Grid.AddCell(row, col, "EFR"); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_dEFWR);
			m_Grid.AddCell(row, col, "EFB"); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_dEFWB);
			m_Grid.AddCell(row, col, "EFF"); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_dEFWF);
			if(pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_MASS)
			{
				long i = 0; for(i = 0; i < pFooting->m_exFooting.m_nCountDan; i++)
				{
					str.Format("%d", i+1);
					m_Grid.AddCell(row, col, "EW"+str); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_dEFW[i]);
					m_Grid.AddCell(row, col, "ES"+str); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_dEFS[i]);
					m_Grid.AddCell(row, col, "EH"+str); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_dEFH[i]);
				}
			}
			m_Grid.AddCell(row, col, "EH"); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_dEFHR);
		}
	}


	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(0, 70);
	if(nColOfFooting > -1)
	{
		m_Grid.SetColumnWidth(nColOfFooting, 100);
	}

	if(nColOfDan > -1) 
	{
		m_Grid.SetColumnWidth(nColOfDan, 80);
		m_Grid.SetColumnWidth(nColOfDan+1, 100);
		m_Grid.SetColumnWidth(nColOfDan+2, 100);
	}

	m_Grid.SetRedraw(TRUE, TRUE);
}

// 종점측 벽체
void CInputGenJongSection::SetGridDataEndWall()
{
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 1, 0);

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	long bBox = pBri->m_nTypeBridge == BRIDGE_TYPE_BOX;	// 박스교인 경우 값이 시점측을 따라가는 경우가 좀 있다.

	CDPointArray xyArr;	// 배경색 반전위치
	CStringArray sArr;
	CString str = _T("");
	CString strHelp = _T("");
	long row = 0;
	long col = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;

	CFootingApp *pFooting = &pBri->m_footingEnd;
	BOOL bWall	= pBri->IsOutWall(FALSE);
	long nColOfFooting = -1;
	long nColOfDan = -1;

	if(bWall)
	{
		m_Grid.AddCell(row, col, "EL.(m)"); m_Grid.AddCell(row+1, col++, bBox ? &pBri->m_dELSB : &pBri->m_dELEB, nFormat, nLine, "종점측 기초 하단 EL.", CELL_TYPE_METER); m_Grid.SetCellDec(CELL_DEC_3);
		m_Grid.AddCell(row, col, "BH"); m_Grid.AddCell(row+1, col++, &pBri->m_dBHE);
		m_Grid.AddCell(row, col, "BTH"); m_Grid.AddCell(row+1, col++, &pBri->m_dBTHE, nFormat);
		m_Grid.AddCell(row, col, "BHH"); m_Grid.AddCell(row+1, col++, &pBri->m_dBHHE);
		m_Grid.AddCell(row, col, "BHW"); m_Grid.AddCell(row+1, col++, &pBri->m_dBHWE);
	}
	else
	{
		// BLOCK OUT
		if(pBri->m_bExpJoint[pBri->m_nQtyJigan])
		{
			m_Grid.AddCell(row, col, "FXW"); m_Grid.AddCell(row+1, col++, &pBri->m_fxJointEnd.m_dWidth, nFormat, nLine, "종점측 BLOCK OUT 폭");
			m_Grid.AddCell(row, col, "FXH"); m_Grid.AddCell(row+1, col++, &pBri->m_fxJointEnd.m_dHeight, nFormat, nLine, "종점측 BLOCK OUT 높이");
		}
	}

	
	m_Grid.AddCell(row, col, "HH"); m_Grid.AddCell(row+1, col++, &pBri->m_dHHE, nFormat, nLine);
	m_Grid.AddCell(row, col, "HW"); m_Grid.AddCell(row+1, col++, &pBri->m_dHWE);
	m_Grid.AddCell(row, col, "HR"); m_Grid.AddCell(row+1, col++, &pBri->m_dHRE);	// 헌치 R
	sArr.RemoveAll();
	sArr.Add("접선");
	sArr.Add("2P");
	m_Grid.AddCell(row, col, "HR(T)"); m_Grid.AddCell(row+1, col++, &pBri->m_nTypeRoundOfHunchEnd, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col, "W"); m_Grid.AddCell(row+1, col++, &pBri->m_dWE, nFormat, nLine, "종점벽체 폭");

	if(bWall)
	{
		if(pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
		{
			m_Grid.AddCell(row, col, "FF"); 	m_Grid.AddCell(row+1, col++, &pFooting->m_dFWL);
			m_Grid.AddCell(row, col, "FB"); m_Grid.AddCell(row+1, col++, &pFooting->m_dFWR);
			m_Grid.AddCell(row, col, "FH"); m_Grid.AddCell(row+1, col++, &pFooting->m_dFH);
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 || pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
			{
				m_Grid.AddCell(row, col, "WHW"); m_Grid.AddCell(row+1, col++, &pBri->m_dWEHW, nFormat, nLine, "종점벽체 헌치 폭");
				m_Grid.AddCell(row, col, "WHH"); m_Grid.AddCell(row+1, col++, &pBri->m_dWEHH, nFormat, nLine, "종점벽체 헌치 높이");

				m_Grid.AddCell(row, col, "FSW"); m_Grid.AddCell(row+1, col++, &pFooting->m_dFSLW, nFormat, nLine, "종점기초 경사 폭");
				m_Grid.AddCell(row, col, "FSH"); m_Grid.AddCell(row+1, col++, &pFooting->m_dFSLH, nFormat, nLine, "종점기초 경사 높이");
				m_Grid.AddCell(row, col, "FEW"); m_Grid.AddCell(row+1, col++, &pFooting->m_dFELW, nFormat, nLine, "종점기초 경사 아래 확장 폭");
			}
		}
		else
		{
			m_Grid.AddCell(row, col, "FB"); m_Grid.AddCell(row+1, col++, &pBri->m_dFRE, nFormat, nLine, "종점기초 폭");
			m_Grid.AddCell(row, col, "FH"); m_Grid.AddCell(row+1, col++, &pBri->m_dFHS, nFormat, nLine, "종점기초 높이");
		}

		// 확대기초타입
		// 박스교일때는 종점측에 대해서 기초타입 설정이 불필요함.
		if(pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
		{
			sArr.RemoveAll();
			sArr.Add("직접기초");
			sArr.Add("MASS기초");
			sArr.Add("말뚝기초");
			
			nColOfFooting = col;
			m_Grid.AddCell(row, col, "기초형식"); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_nType, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
			if(pFooting->m_exFooting.m_bIs)
			{
				if(pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_MASS)
				{
					nColOfDan = col;
					sArr.RemoveAll();
					long i = 0; for(i = 0; i < MAX_MASS_DAN; i++)
					{
						if(i == 0) str = "단없음";
						else str.Format("%d단", i);
						sArr.Add(str);
					}
					m_Grid.AddCell(row, col, "MASS단수"); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_nCountDan, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);

					sArr.RemoveAll();
					sArr.Add("교축");
					sArr.Add("교축직각");
					m_Grid.AddCell(row, col, "단차방향"); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_bFrontDanMass, nFormat, nLine, "", CELL_TYPE_COMBO, sArr);


					sArr.RemoveAll();
					sArr.Add("좌측");
					sArr.Add("우측");
					m_Grid.AddCell(row, col, "MASS기준면"); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_bLeftOrgOfMass, nFormat, nLine, "", CELL_TYPE_COMBO, sArr);
				}
				m_Grid.AddCell(row, col, "EFL"); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_dEFWL);
				m_Grid.AddCell(row, col, "EFR"); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_dEFWR);
				m_Grid.AddCell(row, col, "EFB"); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_dEFWB);
				m_Grid.AddCell(row, col, "EFF"); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_dEFWF);
				if(pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_MASS)
				{
					long i = 0; for(i = 0; i < pFooting->m_exFooting.m_nCountDan; i++)
					{
						str.Format("%d", i+1);
						m_Grid.AddCell(row, col, "EW"+str); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_dEFW[i]);
						m_Grid.AddCell(row, col, "ES"+str); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_dEFS[i]);
						m_Grid.AddCell(row, col, "EH"+str); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_dEFH[i]);
					}
				}

				m_Grid.AddCell(row, col, "EH"); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_dEFHR);
			}
		}
	}

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(0, 70);
	if(nColOfFooting > -1) m_Grid.SetColumnWidth(nColOfFooting, 100);
	if(nColOfDan > -1)
	{
		m_Grid.SetColumnWidth(nColOfDan, 80);
		m_Grid.SetColumnWidth(nColOfDan+1, 100);
		m_Grid.SetColumnWidth(nColOfDan+2, 100);
	}

	m_Grid.SetRedraw(TRUE, TRUE);
}

void CInputGenJongSection::SetGridDataInWall(long nIdxWall)
{
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 1, 0);
	m_Grid.SetCellSize(-1, -1);

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	// 벽체가 없어도 지간이 있으면 일단 생성
	CWallApp *pWall = pBri->GetInWall(nIdxWall);
	if(!pWall) return;

	CStringArray sArr;
	sArr.Add("수직형");
	sArr.Add("경사형");
	sArr.Add("V자형");
	sArr.Add("벽체없음");

	long nIdxHunch = pBri->GetIdxHunchInWall(nIdxWall, TRUE);
	long i = 0;
	long row = 0;
	long col = 0;
	long nStt = 0;
	long nEnd = 0;
	CString str = _T("");
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;


	// 벽체
//	long nColOfKindStt = -1;
	long nColOfKindEnd = -1;

	long nColOfKindStt = col;
	// 슬래브일때는 벽체종류를 입력받을 필요 없음
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB)
	{
		if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH)
		{
			m_Grid.AddCell(row, col, "벽체종류"); m_Grid.AddCell(row+1, col++, &pWall->m_nType, nFormat, TRUE, CELL_READONLY, "", CELL_TYPE_COMBO, sArr);
		}

		//if(pBri->GetJijumConnectRibAndSlab() != nIdxWall+1)
		if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH)
		{
			sArr.RemoveAll(); sArr.Add("기둥식"); sArr.Add("벽체식");
			m_Grid.AddCell(row, col, "단면형상"); m_Grid.AddCell(row+1, col++, &pWall->m_bTopIsColumn, nFormat, pWall->m_nType != WALL_TYPE_VER ? CELL_READONLY : nLine, "", CELL_TYPE_COMBO, sArr);

			// 벽체식인 경우 끝단 라운드 타입 적용 입력받음
			if(pWall->IsEnableRoundRectWall())
			{
				m_Grid.AddCell(row, col, "끝단라운드"); m_Grid.AddCell(row+1, col++, &pWall->m_bRoundRect, nFormat, nLine, "", CELL_TYPE_CHECK);
			}
		}
		nColOfKindEnd	= col;
	}

	// 아치교면 벽체를 아크형태로 바꿀지 선택한다.
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH && pBri->GetJijumConnectRibAndSlab() == nIdxWall+1)
	{
		m_Grid.AddCell(row, col, "접합"); m_Grid.AddCell(row+1, col++, &pBri->m_bConnectRibAndUpperSlab, nFormat, nLine, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row, col, "접합부라운드"); m_Grid.AddCell(row+1, col++, &pWall->m_bSideIsArc, nFormat, nLine, "", CELL_TYPE_CHECK);
		nColOfKindEnd	= col;
	}
	
	
	if(pWall->m_bIs)
	{
		BOOL bEditEL = pBri->m_nTypeBridge != BRIDGE_TYPE_BOX;
		if(pWall->m_footing.m_bIs)
		{
			m_Grid.AddCell(row, col, "EL.(m)"); m_Grid.AddCell(row+1, col++, &pWall->m_dELB, nFormat, bEditEL ? nLine : CELL_READONLY, "", CELL_TYPE_METER); m_Grid.SetCellDec(CELL_DEC_3);		
		}
		m_Grid.AddCell(row, col, "W"); m_Grid.AddCell(row+1, col++, &pWall->m_dW, nFormat, nLine, "벽체 폭");
	}

	if(pWall->m_nType == WALL_TYPE_SLOPE)
	{
		double dAngle	= pBri->GetAngleJijum(nIdxWall+1).GetAngleDegree();
		m_dAngleByHorOfInWall[nIdxWall]	= pWall->m_dAngle + pWall->GetAngleUpperSlab();
		m_dAngleByUpperSlabOfInWall_Angle[nIdxWall]	= pWall->GetAngleByAngle(pWall->m_dAngle, dAngle);
		m_dAngleByHorOfInWall_Angle[nIdxWall]	= m_dAngleByUpperSlabOfInWall_Angle[nIdxWall] + pWall->GetAngleUpperSlab();
		

		m_Grid.AddCell(row, col, "A"); 
		m_Grid.AddCell(row+1, col, &pWall->m_dAngle, nFormat, nLine, "상부슬래브 접선각");
		m_Grid.AddCell(row+2, col++, &m_dAngleByUpperSlabOfInWall_Angle[nIdxWall], nFormat, nLine, "상부슬래브 접선각(사방향각도)");	// 사방향 각도

		m_Grid.AddCell(row, col, "A'"); 
		m_Grid.AddCell(row+1, col, &m_dAngleByHorOfInWall[nIdxWall], nFormat, nLine, "수평에 대한 벽체 각");
		m_Grid.AddCell(row+2, col++, &m_dAngleByHorOfInWall_Angle[nIdxWall], nFormat, nLine, "수평에 대한 벽체 각(사방향각도)");

	}
	else if(pWall->m_nType == WALL_TYPE_V)
	{
		m_Grid.AddCell(row, col, "H"); m_Grid.AddCell(row+1, col++, &pWall->m_dH);
		m_Grid.AddCell(row, col, "A"); m_Grid.AddCell(row+1, col++, &pWall->m_dAngleV);
		m_Grid.AddCell(row, col, "WL"); m_Grid.AddCell(row+1, col++, &pWall->m_dWL);
		m_Grid.AddCell(row, col, "WR"); m_Grid.AddCell(row+1, col++, &pWall->m_dWR);
	}

	// 헌치
	// 신축 이음인 경우 헌치부 높이가 0이 될 수 있도록 입력받아야 된다.
	if(pWall->m_nType == WALL_TYPE_V)
		str.Format("%d", i+1);
	else
		str = "";

	m_Grid.AddCell(row, col, "HHL"+str); m_Grid.AddCell(row+1, col++, &pBri->m_dHHL[nIdxHunch][iUPPER]);
	m_Grid.AddCell(row, col, "HLW"+str); m_Grid.AddCell(row+1, col++, &pBri->m_dHLW[nIdxHunch][iUPPER]);
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH)
	{
		if(pBri->m_nTypeBridge != BRIDGE_TYPE_TRAP && pBri->m_nTypeBridge != BRIDGE_TYPE_TRAP2)
		{
			m_Grid.AddCell(row, col, "WL"+str); m_Grid.AddCell(row+1, col++, &pBri->m_dWL[nIdxHunch][iUPPER]);
		}
		m_Grid.AddCell(row, col, "HLR"+str); m_Grid.AddCell(row+1, col++, &pBri->m_dHLR[nIdxHunch][iUPPER]);	// 헌치 R
		sArr.RemoveAll();
		sArr.Add("접선");
		sArr.Add("2P");
		m_Grid.AddCell(row, col, "HLR(T)"); m_Grid.AddCell(row+1, col++, &pBri->m_nTypeRoundOfHunchInWall[nIdxHunch][iLEFT], nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
	}
	

	m_Grid.AddCell(row, col, "HHR"+str); m_Grid.AddCell(row+1, col++, &pBri->m_dHHR[nIdxHunch][iUPPER], nFormat, CELL_READONLY, "HHL값과 동일하게 적용됨");
	m_Grid.AddCell(row, col, "HRW"+str); m_Grid.AddCell(row+1, col++, &pBri->m_dHRW[nIdxHunch][iUPPER]);
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH)
	{
		if(pBri->m_nTypeBridge != BRIDGE_TYPE_TRAP && pBri->m_nTypeBridge != BRIDGE_TYPE_TRAP2)
		{
			m_Grid.AddCell(row, col, "WR"+str); m_Grid.AddCell(row+1, col++, &pBri->m_dWR[nIdxHunch][iUPPER]);
		}
		m_Grid.AddCell(row, col, "HRR"+str); m_Grid.AddCell(row+1, col++, &pBri->m_dHRR[nIdxHunch][iUPPER]);	// 헌치 R
		sArr.RemoveAll();
		sArr.Add("접선");
		sArr.Add("2P");
		m_Grid.AddCell(row, col, "HRR(T)"); m_Grid.AddCell(row+1, col++, &pBri->m_nTypeRoundOfHunchInWall[nIdxHunch][iRIGHT], nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
	}
	
	
	if(pWall->IsVertWall())
	{
		m_Grid.AddCell(row, col, "HB"+str); m_Grid.AddCell(row+1, col++, &pBri->m_dHBH[nIdxHunch][iUPPER]);
	}
	
	
	if(pWall->m_nType == WALL_TYPE_V)
	{
		str.Format("%d", i+2);
		nStt = col;
		m_Grid.AddCell(row, col, "HHL"+str); m_Grid.AddCell(row+1, col++, &pBri->m_dHHL[nIdxHunch+1][iUPPER]);
		m_Grid.AddCell(row, col, "HLW"+str); m_Grid.AddCell(row+1, col++, &pBri->m_dHLW[nIdxHunch+1][iUPPER]);
		m_Grid.AddCell(row, col, "WL"+str); m_Grid.AddCell(row+1, col++, &pBri->m_dWL[nIdxHunch+1][iUPPER]);
		m_Grid.AddCell(row, col, "HLR"+str); m_Grid.AddCell(row+1, col++, &pBri->m_dHLR[nIdxHunch+1][iUPPER]);	// 헌치 R
		
		m_Grid.AddCell(row, col, "HHR"+str); m_Grid.AddCell(row+1, col++, &pBri->m_dHHR[nIdxHunch+1][iUPPER], nFormat, CELL_READONLY, "HHL값과 동일하게 적용됨");
		m_Grid.AddCell(row, col, "HRW"+str); m_Grid.AddCell(row+1, col++, &pBri->m_dHRW[nIdxHunch+1][iUPPER]);
		m_Grid.AddCell(row, col, "WR"+str); m_Grid.AddCell(row+1, col++, &pBri->m_dWR[nIdxHunch+1][iUPPER]);
		m_Grid.AddCell(row, col, "HRR"+str); m_Grid.AddCell(row+1, col++, &pBri->m_dHRR[nIdxHunch+1][iUPPER]);	// 헌치 R
		
		m_Grid.AddCell(row, col, "HB"+str); m_Grid.AddCell(row+1, col++, &pBri->m_dHBH[nIdxHunch+1][iUPPER]);
		nEnd = col;
	}

	// 아치교는 벽체 받침에 대해서 입력 받음
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		m_Grid.AddCell(row, col, "BW"); m_Grid.AddCell(row+1, col++, &pBri->m_dRibBlockW[nIdxWall], nFormat, nLine, "벽체 받침 폭");
		m_Grid.AddCell(row, col, "BH"); m_Grid.AddCell(row+1, col++, &pBri->m_dRibBlockH[nIdxWall], nFormat, nLine, "벽체 받침 높이");
	}


	// 슬래브나 박스형일때는 기초나 기초타입에 대해서 입력불필요
	long nColOfFooting = -1;
	long nColOfDan = -1;
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB && pBri->m_nTypeBridge != BRIDGE_TYPE_BOX && pWall->m_footing.m_bIs)
	{
		// 기초에 대해서 입력
		if(pWall->m_nType == WALL_TYPE_SLOPE)
		{
			m_Grid.AddCell(row, col, "FH"); m_Grid.AddCell(row+1, col++, &pWall->m_footing.m_dFH);
			m_Grid.AddCell(row, col, "FW"); m_Grid.AddCell(row+1, col++, &pWall->m_footing.m_dFW);

			if(pWall->IsLeftSlope())
			{
				m_Grid.AddCell(row, col, "FSW"); m_Grid.AddCell(row+1, col++, &pWall->m_footing.m_dFSLW);
				m_Grid.AddCell(row, col, "FSH"); m_Grid.AddCell(row+1, col++, &pWall->m_footing.m_dFSRH);
				m_Grid.AddCell(row, col, "FSU"); m_Grid.AddCell(row+1, col++, &pWall->m_footing.m_dFWL);
			}
			else
			{
				m_Grid.AddCell(row, col, "FSW"); m_Grid.AddCell(row+1, col++, &pWall->m_footing.m_dFSRW);
				m_Grid.AddCell(row, col, "FSH"); m_Grid.AddCell(row+1, col++, &pWall->m_footing.m_dFSLH);
				m_Grid.AddCell(row, col, "FSU"); m_Grid.AddCell(row+1, col++, &pWall->m_footing.m_dFWR);
			}
		}
		else
		{
			m_Grid.AddCell(row, col, "FH"); m_Grid.AddCell(row+1, col++, &pWall->m_footing.m_dFH, nFormat, nLine, "기초 높이");
			m_Grid.AddCell(row, col, "FL"); m_Grid.AddCell(row+1, col++, &pWall->m_footing.m_dFWL, nFormat, nLine, "기초 폭");
		}

		
		// 확대 기초에 대해서 입력
		CExFootingApp *pExFooting = &pWall->m_footing.m_exFooting;

		// 확대기초타입
		sArr.RemoveAll();
		sArr.Add("직접기초");
		sArr.Add("MASS기초");
		sArr.Add("말뚝기초");

		
		nColOfFooting = col;
		m_Grid.AddCell(row, col, "기초형식"); m_Grid.AddCell(row+1, col++, &pExFooting->m_nType, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
		if(pExFooting->m_bIs)
		{
			if(pExFooting->m_nType == EXFOOTING_TYPE_MASS)
			{
				nColOfDan = col;
				sArr.RemoveAll();
				long i = 0; for(i = 0; i < MAX_MASS_DAN; i++)
				{
					if(i == 0) str = "단없음";
					else str.Format("%d단", i);
					sArr.Add(str);
				}
				m_Grid.AddCell(row, col, "MASS단수"); m_Grid.AddCell(row+1, col++, &pExFooting->m_nCountDan, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);

				sArr.RemoveAll();
				sArr.Add("교축");
				sArr.Add("교축직각");
				m_Grid.AddCell(row, col, "단차방향"); m_Grid.AddCell(row+1, col++, &pExFooting->m_bFrontDanMass, nFormat, nLine, "", CELL_TYPE_COMBO, sArr);

				sArr.RemoveAll();
				sArr.Add("좌측");
				sArr.Add("우측");
				m_Grid.AddCell(row, col, "MASS기준면"); m_Grid.AddCell(row+1, col++, &pExFooting->m_bLeftOrgOfMass, nFormat, nLine, "", CELL_TYPE_COMBO, sArr);
			}
			m_Grid.AddCell(row, col, "EFL"); m_Grid.AddCell(row+1, col++, &pExFooting->m_dEFWL);
			m_Grid.AddCell(row, col, "EFR"); m_Grid.AddCell(row+1, col++, &pExFooting->m_dEFWR);
			m_Grid.AddCell(row, col, "EFB"); m_Grid.AddCell(row+1, col++, &pExFooting->m_dEFWB);
			m_Grid.AddCell(row, col, "EFF"); m_Grid.AddCell(row+1, col++, &pExFooting->m_dEFWF);
			if(pExFooting->m_nType == EXFOOTING_TYPE_MASS)
			{
				long i = 0; for(i = 0; i < pExFooting->m_nCountDan; i++)
				{
					str.Format("%d", i+1);
					m_Grid.AddCell(row, col, "EW"+str); m_Grid.AddCell(row+1, col++, &pExFooting->m_dEFW[i]);
					m_Grid.AddCell(row, col, "ES"+str); m_Grid.AddCell(row+1, col++, &pExFooting->m_dEFS[i]);
					m_Grid.AddCell(row, col, "EH"+str); m_Grid.AddCell(row+1, col++, &pExFooting->m_dEFH[i]);
				}
			}

			m_Grid.AddCell(row, col, "EH"); m_Grid.AddCell(row+1, col++, &pExFooting->m_dEFHR);
		}
	}


	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(2, 70);
	if(nColOfKindEnd > -1)	
	{
		for(long c = nColOfKindStt; c < nColOfKindEnd; c++)
		{
			m_Grid.SetColumnWidth(c, 100);
		}
	}
	if(nColOfFooting > -1)	m_Grid.SetColumnWidth(nColOfFooting, 100);
	if(nColOfDan > -1)		
	{
		m_Grid.SetColumnWidth(nColOfDan, 80);
		m_Grid.SetColumnWidth(nColOfDan+1, 100);
		m_Grid.SetColumnWidth(nColOfDan+2, 100);
	}

	// 셀배경 반전.
	COLORREF clr = GRID_TEXT_BK_COLOR_REV;
	for(i = nStt; i < nEnd; i++)
	{
		m_Grid.SetItemBkColour(1, i, clr);
	}

	m_Grid.SetRedraw(TRUE, TRUE);
}

CFootingApp *CInputGenJongSection::GetCurrentFooting()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	CFootingApp *pFooting	= NULL;
	if(pBri->IsBoxType())
	{
		pFooting	= &pBri->m_footingBox;
	}
	else
	{
		if(m_nTab == m_nTabWallStt) 
		{
			pFooting	= &pBri->m_footingStt;
		}
		else if(m_nTab == m_nTabWallEnd) 
		{
			pFooting	= &pBri->m_footingEnd;
		}
		else 
		{
			long nIdxInWall = GetIdxCurInWall();
			if(nIdxInWall > -1)
			{
				CWallApp *pWall = pBri->GetInWall(nIdxInWall);
				if(pWall) 
					pFooting	= &pWall->m_footing;
			}
		}
	}

	return pFooting;
}

void CInputGenJongSection::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	CCellID cell = m_Grid.GetFocusCell();

	// 벽체종류나 경사면일때 벽체 각이 바뀌는 경우 다시 그려야 됨.
	CString str = m_Grid.GetTextMatrix(0, cell.col);
	
	// 메스에서 다른 타입으로 변경시 두께 변경해줘야 되므로 이전 타입을 보관하고 있음 /////////
	long nJ	= 0;
	CFootingApp *pFooting	= NULL;
	if(pBri->IsBoxType())
	{
		pFooting	= &pBri->m_footingBox;
	}
	else
	{
		if(m_nTab == m_nTabWallStt) 
		{
			pFooting	= &pBri->m_footingStt;
			nJ	= 0;

			if(str == "W")
			{
				// 벽체 너비 변경시 평면 제원 다시 구함.
				m_pIn->SettingNeedPlane();
			}
		}
		else if(m_nTab == m_nTabWallEnd) 
		{
			pFooting	= &pBri->m_footingEnd;
			nJ	= pBri->m_nQtyJigan;

			if(str == "W")
			{
				// 벽체 너비 변경시 평면 제원 다시 구함.
				m_pIn->SettingNeedPlane();
			}
		}
		else 
		{
			long nIdxInWall = GetIdxCurInWall();
			if(nIdxInWall > -1)
			{
				CWallApp *pWall = pBri->GetInWall(nIdxInWall);
				if(pWall) 
					pFooting	= &pWall->m_footing;
				nJ	= nIdxInWall + 1;
			}
		}
	}


	long nOldTypeExFooting	= -1;
	if(pFooting)
	{
		if(pBri->IsBoxType() == FALSE)
			nOldTypeExFooting	= pFooting->m_exFooting.m_nType;
		else
			nOldTypeExFooting	= pBri->m_nTypeLower;
	}
	// 메스에서 다른 타입으로 변경시 두께 변경해줘야 되므로 이전 타입을 보관하고 있음 /////////


	SetDataSave();


	// 수평에 대한 각도를 입력 받은 경우 원래 벽체의 각도(상부슬래브 접선각)을 계산해서 넣어준다. /////
	if(str == "A'" && cell.row == 1)
	{
		long nIdxInWall = GetIdxCurInWall();
		CWallApp *pWall	= pBri->GetInWall(nIdxInWall);
		if(pWall)
		{
			pWall->m_dAngle	= m_dAngleByHorOfInWall[nIdxInWall] - pWall->GetAngleUpperSlab();
			SetGridData();
			SetDataInit();
		}
	}
	else if((str == "A" || str == "A'" ) && cell.row == 2)
	{
		
		long nIdxInWall	= GetIdxCurInWall();
		CWallApp *pWall	= pBri->GetInWall(nIdxInWall);
		if(pWall)
		{
			if(str == "A'")
			{
				m_dAngleByUpperSlabOfInWall_Angle[nIdxInWall]	= m_dAngleByHorOfInWall_Angle[nIdxInWall] - pWall->GetAngleUpperSlab();
			}
			double dAngle	= pBri->GetAngleJijum(nJ).GetAngleDegree();
			pWall->m_dAngle	= pWall->GetAngleByVert(m_dAngleByUpperSlabOfInWall_Angle[nIdxInWall], dAngle);
			SetGridData();
			SetDataInit();
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////
	if(str == "벽체종류" || str == "A" || str == "기초형식" || str == "MASS단수")
	{
		SetGridData();
		
		if(str == "벽체종류")
		{
			long nIdxInWall = GetIdxCurInWall();
			CWallApp *pWall	= pBri->GetInWall(nIdxInWall);
			if(pWall)
			{
				pBri->KeepHunchCount();
				pBri->SetOrgInWall(nIdxInWall);
				pBri->SetDataDefaultShapeFooting(&pWall->m_footing, pWall->m_dW, pBri->GetXyOnJijum(nIdxInWall+1).x, nIdxInWall);
			}
		}
		// mass 단수가 바뀌면 mass에 대해서만 권고안을 한다.
		if(str == "MASS단수" || str == "기초형식")
		{
			if(pFooting)
			{
				if(str == "MASS단수" || str == "단차방향")
				{
					pFooting->SetDataDefaultShapeExFooting();
				}
				// 직접 <-> MASS 선택시 권고안 해줌
				else  if(str == "기초형식")
				{
					CExFootingApp *pExFooting	= &pFooting->m_exFooting;
					if(pExFooting->m_nType == EXFOOTING_TYPE_MASS || (pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_MASS))
					{
						pExFooting->m_dEFWL = 200;
						pExFooting->m_dEFWR = 200;
						pExFooting->m_dEFHR = 2000;
						pExFooting->m_nCountDan = 0;
						if(pBri->IsBoxType() == FALSE)
						{
							pExFooting->m_nCountDan = 2;
							pFooting->SetDataDefaultShapeExFooting();
						}
					}
					else if(nOldTypeExFooting == EXFOOTING_TYPE_MASS && (pExFooting->m_nType == EXFOOTING_TYPE_LEAN || pExFooting->m_nType == EXFOOTING_TYPE_PILE))
					{
						pExFooting->m_dEFHR = 100;
					}
					else if(pBri->IsBoxType() && nOldTypeExFooting == BOXFOOTING_TYPE_MASS && pBri->m_nTypeLower != BOXFOOTING_TYPE_MASS)
					{
						pExFooting->m_dEFHR = 100;
					}
					
					if(pExFooting->m_nType == EXFOOTING_TYPE_PILE || (pBri->IsBoxType() && pBri->IsPileBase(0)))
					{
						double dAngle	= pBri->GetAngleJijum(nJ).GetAngleDegree();
						pFooting->SyncFooting(FALSE, dAngle);
						if(Compare(pExFooting->m_Pile.m_dDia, 0.0, "=") || Compare(pExFooting->m_Pile.m_dLength, 0.0, "="))
						{
							pExFooting->m_Pile.SetDataDefaultNormal();
						}
						pBri->SetDataDefaultPilePos(pFooting);
					}
					if(pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR)
					{
						pBri->SetDataDefaultEarthAnchor(pFooting);
					}
				}

				SetGridData();
			}
		}
		SetDataInit();
	}
	
	if(str == "단면형상")
	{
		SetGridData();
		
		long nIdxInWall = GetIdxCurInWall();
		CWallApp *pWall = pBri->GetInWall(nIdxInWall);
		if(pWall) 
		{
			if(pWall->m_bTopIsColumn)
			{
				//31720
				CTwinVectorArray tvArrVertSection;
				tvArrVertSection.AddFromTvArray(pBri->m_tvArrVertSection);

				pWall->SyncWall(tvArrVertSection);
				pBri->SetDataDefaultShapeTopInWall(nIdxInWall); // (ARCBRIDGE-3607) 벽체식 = > 기둥식 변경시 
				pWall->SetDataDefaultDistColumn(TRUE);
				
				pBri->SetDataDefaultRebarColumnStd(nIdxInWall);
			}
		}

		SetDataInit();
	}

	if(pBri->IsBoxType() && str.Find(_T("EL.")) != -1 && pBri->m_bSlopeLowerSlab == FALSE)
	{
		pBri->m_dELEB = pBri->m_dELSB;
	}

	if(str.Find(_T("HHL")) != -1 || str.Find(_T("LLH")) != -1)
	{
		long nIdxWall = pBri->IsBoxType() ? cell.row - 1 : m_nTab - 1;
		pBri->m_dHHR[nIdxWall][iUPPER] = pBri->m_dHHL[nIdxWall][iUPPER];
		pBri->m_dHHR[nIdxWall][iLOWER] = pBri->m_dHHL[nIdxWall][iLOWER];
	}

	if(str == "접합")
	{
		long nIdxInWall = GetIdxCurInWall();
		CWallApp *pWall = pBri->GetInWall(nIdxInWall);
		if(pWall) 
		{
			pWall->m_bIs	= TRUE;
			pWall->m_dW	= pBri->m_bConnectRibAndUpperSlab ? 10000 : 0;
			SetDataInit();
		}
	}
	
	pBri->m_bNeedSettingVertSection	= TRUE;

	SetGridData();
	SetDataInit();

	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);
}


void CInputGenJongSection::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
/*	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	
	// view에서 온 메시지라면.. 단면도 체크를 함.
	if(m_pIn->m_strME.bViewMouseEvent)
	{
		CDPoint xy = m_pIn->m_strME.xy;
		long nIdx = 0;

		// 브리지 체크
		if(m_tvArrBridge.GetTvSelect(xy, nIdx))
		{
			CTwinVector tv	= m_tvArrBridge.GetAt(nIdx);

			// 내측헌치 아래면 선택시 내측헌치 너비 권고안 해줌
			long nFind	= tv.m_sInfo.Find("번째내측", 0);
			if(nFind != -1)
			{
				long nIdxHunch	= atoi((char*)(LPCSTR)tv.m_sInfo.Left(nFind))-1;
				long nIdxInWall	= -1;
				BOOL bLeft		= FALSE;
				pBri->GetInfoHunch(nIdxHunch, nIdxInWall, bLeft);
				pBri->SetDatadefaultShapeInWallHunch(nIdxInWall);
				SetGridData();
				SetDataInit();
			}
		}
		else if(m_tvArrBridge.IsSelect(xy))
		{
			CDPoint xyStt = m_pIn->GetRcBridge()->GetXySectionSttEnd(TRUE);
			CDPoint xyEnd = m_pIn->GetRcBridge()->GetXySectionSttEnd(FALSE);

			SetDataSave();
			if(xyStt.x >= xy.x && xyStt.y >= xy.y && m_nTabWallStt > -1)
				m_Tab.SetCurSel(m_nTabWallStt);
			else if(xyEnd.x <= xy.x && xyEnd.x >= xy.y && m_nTabWallEnd > -1)
				m_Tab.SetCurSel(m_nTabWallEnd);
			
			
			OnTabSelchange(0, 0);
		}
		// 시점측 기초 체크
		else if(m_tvArrFootingStt.GetTvSelect(xy, nIdx))
		{
			CTwinVector tv	= m_tvArrFootingStt.GetAt(nIdx);
		}
		else if(m_tvArrFootingStt.IsSelect(xy))
		{
			SetDataSave();
			m_Tab.SetCurSel(m_nTabWallStt);
			OnTabSelchange(0, 0);
		}
		// 시점측 확대기초 체크
		else if(m_tvArrExFootingStt.GetTvSelect(xy, nIdx))
		{
			CTwinVector tv	= m_tvArrExFootingStt.GetAt(nIdx);
		}
		else if(m_tvArrExFootingStt.IsSelect(xy))
		{
			SetDataSave();
			m_Tab.SetCurSel(m_nTabWallStt);
			OnTabSelchange(0, 0);
		}
		// 종점측 기초 체크
		else if(m_tvArrFootingEnd.GetTvSelect(xy, nIdx))
		{
			CTwinVector tv	= m_tvArrFootingEnd.GetAt(nIdx);
		}
		else if(m_tvArrFootingEnd.IsSelect(xy))
		{
			SetDataSave();
			m_Tab.SetCurSel(m_nTabWallEnd);
			OnTabSelchange(0, 0);
		}
		// 종점측 확대기초 체크
		else if(m_tvArrExFootingEnd.GetTvSelect(xy, nIdx))
		{
			CTwinVector tv	= m_tvArrExFootingEnd.GetAt(nIdx);
		}
		else if(m_tvArrExFootingEnd.IsSelect(xy))
		{
			SetDataSave();
			m_Tab.SetCurSel(m_nTabWallEnd);
			OnTabSelchange(0, 0);
		}
		else 
		{
			BOOL bResult = FALSE;
			// 벽체 체크
			long i = 0; for(i = 0; i < m_ptvArrInWall.GetSize(); i++)
			{
				if(m_ptvArrInWall.GetAt(i)->GetTvSelect(xy, nIdx))
				{
					CTwinVector tv	= m_ptvArrInWall.GetAt(i)->GetAt(nIdx);
					bResult = TRUE;
					CString str = tv.m_sInfo;
					
					if(str == "벽체하면좌측" || str == "벽체하면우측")
					{
						m_pIn->GetRcBridge()->GetInWall(i)->m_bApplyAngleEnd = !m_pIn->GetRcBridge()->GetInWall(i)->m_bApplyAngleEnd;
						DrawInputDomyunView(FALSE);
					}
					else
					{
						
					}

					break;
				}
				else if(m_ptvArrInWall.GetAt(i)->IsSelect(xy))
				{
					bResult = TRUE;

					SetDataSave();
					m_Tab.SetCurSel(m_nTabInWall[i]);
					OnTabSelchange(0, 0);
					break;
				}
			}

			// 기초 체크
			if(!bResult)
			{
				for(i = 0; i < m_ptvArrFooting.GetSize(); i++)
				{
					if(m_ptvArrFooting.GetAt(i)->GetTvSelect(xy, nIdx))
					{
						CTwinVector tv	= m_ptvArrFooting.GetAt(i)->GetAt(nIdx);
						bResult = TRUE;
						break;
					}
					else if(m_ptvArrFooting.GetAt(i)->IsSelect(xy))
					{
						SetDataSave();
						m_Tab.SetCurSel(m_nTabInWall[i]);
						OnTabSelchange(0, 0);
						bResult = TRUE;
						break;
					}
				}
			}

			// 확대기초 체크
			if(!bResult)
			{
				for(i = 0; i < m_ptvArrExFooting.GetSize(); i++)
				{
					if(m_ptvArrExFooting.GetAt(i)->GetTvSelect(xy, nIdx))
					{
						CTwinVector tv	= m_ptvArrExFooting.GetAt(i)->GetAt(nIdx);
						bResult = TRUE;
						break;
					}
					else if(m_ptvArrExFooting.GetAt(i)->IsSelect(xy))
					{
						SetDataSave();
						m_Tab.SetCurSel(m_nTabInWall[i]);
						OnTabSelchange(0, 0);
						bResult = TRUE;
						break;
					}
				}
			}
		}

		
		m_pIn->m_strME.bViewMouseEvent = FALSE;
	}
	
*/	
	CInputDialogBase::OnLButtonDblClk(nFlags, point);
}

long CInputGenJongSection::OnHitTest()
{
	// 브리지 체크	
/*	long nIdx=0;
	CTwinVector tvSelect;
	if(m_tvArrBridge.GetTvSelect(m_pIn->m_strME.xy, nIdx, TRUE)) 
	{
		tvSelect	= m_tvArrBridge.GetAt(nIdx);
		if(tvSelect.val.valHor && tvSelect.val.valVer) return 2;
		else if(tvSelect.val.valHor && !tvSelect.val.valVer) return 1;
		else if(!tvSelect.val.valHor && tvSelect.val.valVer) return 0;
		return -1;
	}
	// 시점측 기초	
	if(m_tvArrFootingStt.GetTvSelect(m_pIn->m_strME.xy, nIdx, TRUE)) 
	{
		tvSelect	= m_tvArrFootingStt.GetAt(nIdx);
		if(tvSelect.val.valHor && tvSelect.val.valVer) return 2;
		else if(tvSelect.val.valHor && !tvSelect.val.valVer) return 1;
		else if(!tvSelect.val.valHor && tvSelect.val.valVer) return 0;
		return -1;
	}

	// 시점측 확대기초	
	if(m_tvArrExFootingStt.GetTvSelect(m_pIn->m_strME.xy, nIdx, TRUE))
	{
		tvSelect	= m_tvArrExFootingStt.GetAt(nIdx);
		if(tvSelect.val.valHor && tvSelect.val.valVer) return 2;
		else if(tvSelect.val.valHor && !tvSelect.val.valVer) return 1;
		else if(!tvSelect.val.valHor && tvSelect.val.valVer) return 0;
		return -1;
	}
	// 종점측 기초	
	if(m_tvArrFootingEnd.GetTvSelect(m_pIn->m_strME.xy, nIdx, TRUE))
	{
		tvSelect	= m_tvArrFootingEnd.GetAt(nIdx);
		if(tvSelect.val.valHor && tvSelect.val.valVer) return 2;
		else if(tvSelect.val.valHor && !tvSelect.val.valVer) return 1;
		else if(!tvSelect.val.valHor && tvSelect.val.valVer) return 0;
		return -1;
	}
	// 종점측 확대기초	
	if(m_tvArrExFootingEnd.GetTvSelect(m_pIn->m_strME.xy, nIdx, TRUE))
	{
		tvSelect	= m_tvArrExFootingEnd.GetAt(nIdx);
		if(tvSelect.val.valHor && tvSelect.val.valVer) return 2;
		else if(tvSelect.val.valHor && !tvSelect.val.valVer) return 1;
		else if(!tvSelect.val.valHor && tvSelect.val.valVer) return 0;
		return -1;
	}

	// 내측벽체
	long i = 0; for(i = 0; i < m_ptvArrInWall.GetSize(); i++)
		if(m_ptvArrInWall.GetAt(i)->GetTvSelect(m_pIn->m_strME.xy, nIdx, TRUE))
		{
			tvSelect	= m_ptvArrInWall.GetAt(i)->GetAt(nIdx);
			if(tvSelect.val.valHor && tvSelect.val.valVer) return 2;
			else if(tvSelect.val.valHor && !tvSelect.val.valVer) return 1;
			else if(!tvSelect.val.valHor && tvSelect.val.valVer) return 0;
			return -1;
		}

	// 기초
	for(i = 0; i < m_ptvArrFooting.GetSize(); i++)
		if(m_ptvArrFooting.GetAt(i)->GetTvSelect(m_pIn->m_strME.xy, nIdx, TRUE))
		{
			tvSelect	= m_ptvArrFooting.GetAt(i)->GetAt(nIdx);
			if(tvSelect.val.valHor && tvSelect.val.valVer) return 2;
			else if(tvSelect.val.valHor && !tvSelect.val.valVer) return 1;
			else if(!tvSelect.val.valHor && tvSelect.val.valVer) return 0;
			return -1;
		}
	
	// 확대기초
	for(i = 0; i < m_ptvArrExFooting.GetSize(); i++)
		if(m_ptvArrExFooting.GetAt(i)->GetTvSelect(m_pIn->m_strME.xy, nIdx, TRUE))
		{
			tvSelect	= m_ptvArrExFooting.GetAt(i)->GetAt(nIdx);
			if(tvSelect.val.valHor && tvSelect.val.valVer) return 2;
			else if(tvSelect.val.valHor && !tvSelect.val.valVer) return 1;
			else if(!tvSelect.val.valHor && tvSelect.val.valVer) return 0;
			return -1;
		}
*/		
	return -1;
}

void CInputGenJongSection::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
/*	if(m_pIn->m_strME.bViewMouseEvent)
	{
		//m_pView->SetGroupFlash(FALSE);

		// 브리지 체크
		m_tvArrBridge.OnLButtonDown(nFlags, m_pIn->m_strME.xy);
		// 시점측 기초
		m_tvArrFootingStt.OnLButtonDown(nFlags, m_pIn->m_strME.xy);
		// 시점측 확대기초
		m_tvArrExFootingStt.OnLButtonDown(nFlags, m_pIn->m_strME.xy);
		// 종점측 기초
		m_tvArrFootingEnd.OnLButtonDown(nFlags, m_pIn->m_strME.xy);
		// 종점측 확대기초
		m_tvArrExFootingEnd.OnLButtonDown(nFlags, m_pIn->m_strME.xy);

		// 내측벽체
		long i = 0; for(i = 0; i < m_ptvArrInWall.GetSize(); i++)
		{
			m_ptvArrInWall.GetAt(i)->OnLButtonDown(nFlags, m_pIn->m_strME.xy);
		}

		// 기초
		for(i = 0; i < m_ptvArrFooting.GetSize(); i++)
		{
			m_ptvArrFooting.GetAt(i)->OnLButtonDown(nFlags, m_pIn->m_strME.xy);
		}

		// 확대기초
		for(i = 0; i < m_ptvArrExFooting.GetSize(); i++)
		{
			m_ptvArrExFooting.GetAt(i)->OnLButtonDown(nFlags, m_pIn->m_strME.xy);
		}
	}

	*/
	CInputDialogBase::OnLButtonDown(nFlags, point);
}

void CInputGenJongSection::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
/*
	if(m_pIn->m_strME.bViewMouseEvent)
	{
		//m_pView->SetGroupFlash(TRUE);
		// 브리지 체크
		m_bMove = m_tvArrBridge.IsSelect();
		m_tvArrBridge.OnLButtonUp(nFlags, m_pIn->m_strME.xy);
		
		// 시점측기초
		m_bMove = !m_bMove ? m_tvArrFootingStt.IsSelect() : m_bMove;
		m_tvArrFootingStt.OnLButtonUp(nFlags, m_pIn->m_strME.xy);
		// 시점측확대기초
		m_bMove = !m_bMove ? m_tvArrExFootingStt.IsSelect() : m_bMove;
		m_tvArrExFootingStt.OnLButtonUp(nFlags, m_pIn->m_strME.xy);

		// 종점측기초
		m_bMove = !m_bMove ? m_tvArrFootingEnd.IsSelect() : m_bMove;
		m_tvArrFootingEnd.OnLButtonUp(nFlags, m_pIn->m_strME.xy);
		// 종점측확대기초
		m_bMove = !m_bMove ? m_tvArrExFootingEnd.IsSelect() : m_bMove;
		m_tvArrExFootingEnd.OnLButtonUp(nFlags, m_pIn->m_strME.xy);
		
		// 내측벽체
		long i = 0; for(i = 0; i < m_ptvArrInWall.GetSize(); i++)
		{
			m_bMove = !m_bMove ? m_ptvArrInWall.GetAt(i)->IsSelect() : m_bMove;
			m_ptvArrInWall.GetAt(i)->OnLButtonUp(nFlags, m_pIn->m_strME.xy);
		}
		// 기초
		for(i = 0; i < m_ptvArrFooting.GetSize(); i++)
		{
			m_bMove = !m_bMove ? m_ptvArrFooting.GetAt(i)->IsSelect() : m_bMove;
			m_ptvArrFooting.GetAt(i)->OnLButtonUp(nFlags, m_pIn->m_strME.xy);
		}
		// 확대기초
		for(i = 0; i < m_ptvArrExFooting.GetSize(); i++)
		{
			m_bMove = !m_bMove ? m_ptvArrExFooting.GetAt(i)->IsSelect() : m_bMove;
			m_ptvArrExFooting.GetAt(i)->OnLButtonUp(nFlags, m_pIn->m_strME.xy);
		}

		if(m_bMove)
		{
			m_bMove = FALSE;
			DrawInputDomyunView(FALSE);
		}
	}
	*/
	CInputDialogBase::OnLButtonUp(nFlags, point);
}

void CInputGenJongSection::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	/*
	if(nFlags & MK_CONTROL) 	
		CTwinVectorArray::SetXySnap(10, 10);			
	else
		CTwinVectorArray::SetXySnap(100, 100);		
		*/

/*	if(m_pIn->m_strME.bViewMouseEvent) 
	{
		CDPoint xyMouse(0, 0);
		m_bMove = FALSE;
		if(m_tvArrBridge.OnMouseMove(nFlags, m_pIn->m_strME.xy))
		{
//			xyMouse = m_tvArrBridge.GetTvSelected().GetXyMid();
//			CPoint xyScreen = m_pView->ExtToClient(xyMouse);
//			m_pView->ClientToScreen(&xyScreen);
//			SetCursorPos(xyScreen.x,xyScreen.y);
			m_bMove  = TRUE;
		}
		// 시점측 기초
		else if(m_tvArrFootingStt.OnMouseMove(nFlags, m_pIn->m_strME.xy))
		{
			m_bMove  = TRUE;
		}
		// 시점측 확대기초
		else if(m_tvArrExFootingStt.OnMouseMove(nFlags, m_pIn->m_strME.xy))
		{
			m_bMove  = TRUE;
		}		
		// 종점측 기초
		else if(m_tvArrFootingEnd.OnMouseMove(nFlags, m_pIn->m_strME.xy))
		{
			m_bMove  = TRUE;
		}
		// 종점측 확대기초
		else if(m_tvArrExFootingEnd.OnMouseMove(nFlags, m_pIn->m_strME.xy))
		{
			m_bMove  = TRUE;
		}
		else 
		{
			long i = 0; for(i = 0; i < m_ptvArrInWall.GetSize(); i++)
			{
				if(m_ptvArrInWall.GetAt(i)->OnMouseMove(nFlags, m_pIn->m_strME.xy))
				{
					m_bMove  = TRUE;
					break;
				}
			}

			if(!m_bMove )
			{
				long i = 0; for(i = 0; i < m_ptvArrFooting.GetSize(); i++)
				{
					if(m_ptvArrFooting.GetAt(i)->OnMouseMove(nFlags, m_pIn->m_strME.xy))
					{
						m_bMove  = TRUE;
						break;
					}
				}
			}

			if(!m_bMove )
			{
				long i = 0; for(i = 0; i < m_ptvArrExFooting.GetSize(); i++)
				{
					if(m_ptvArrExFooting.GetAt(i)->OnMouseMove(nFlags, m_pIn->m_strME.xy))
					{
						m_bMove  = TRUE;
						break;
					}
				}
			}
		}

		if(m_bMove)
		{	
			m_pIn->GetRcBridgeRebar()->m_bNeedSettingVertSection	= TRUE;

			SetDataInit();
			m_Grid.SetRedraw();
			m_pView->SetFocus();
			DrawInputDomyunView(FALSE);
		}
	}
	*/
	CInputDialogBase::OnMouseMove(nFlags, point);
}



BOOL CInputGenJongSection::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	//m_pView->SetGroupFlash(FALSE);

	return CInputDialogBase::DestroyWindow();
}

void CInputGenJongSection::OnButtonExamine() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	
	BOOL bFemNormal = pBri->m_FemNormal.IsOutputLoaded();
	BOOL bFemEQ     = pBri->IsRcSlab() ? TRUE : (pBri->GetFemManage(COMBO_EARTHQUAKE)->IsOutputLoaded());

	BOOL bIsFoot = (pBri->IsBoxType() || pBri->GetFootingByJijumNumber(m_nTab) == NULL)? FALSE : TRUE;
	
	if(!bFemNormal || (pBri->GetTypeDesingEQ() == 1 && !bFemEQ))
	{
		AfxMessageBox("모델링 해석을 수행후에 설계검토를 하시기 바랍니다.");
	}
	else if(!bIsFoot)
	{
		AfxMessageBox("선택하신 지점에 검토할 기초가 없습니다.");
	}
	else
	{
		CInputCheckGrid Dlg(pBri, IDD_GEN_JONG_SECTION, m_nTab);
		Dlg.DoModal();
	}
}

// 현재 중간벽체의 인덱스 
// 없으면 -1
long CInputGenJongSection::GetIdxCurInWall()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(pBri)
	{
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
		{
			if(m_nTab == m_nTabInWallBox)
			{
				structGridCell* pCell = m_Grid.GetCurrentCell();
				if(pCell && pCell->nRow > 0)
				{
					return pCell->nRow - 1;
				}
				return 0;
			}
		}
		else
		{
			long i = 0; for(i = 0; i < pBri->GetCountInWall(); i++)
			{
				if(m_nTab == m_nTabInWall[i]) return i;
			}
		}
	}
	else
		return -1;

	return -1;
}

long CInputGenJongSection::GetIdxCurOutWall()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(pBri)
	{
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
		{
			if(m_nTab == m_nTabOutWall)
			{
				structGridCell* pCell = m_Grid.GetCurrentCell();
				if(pCell && pCell->nRow > 0)
				{
					return pCell->nRow - 1;
				}
				return 0;
			}
		}
		else
		{
			if(m_nTab == m_nTabWallStt) return 0;
			if(m_nTab == m_nTabWallEnd) return 1;
		}
	}
	else
		return -1;

	return -1;
}


void CInputGenJongSection::OnCheckViewEarth() 
{
	// TODO: Add your control notification handler code here
	m_bCheckViewEarth	= m_checkViewEarth.GetCheck();
	DrawInputDomyunView(TRUE);
}

// 시종점부 동일 적용.
void CInputGenJongSection::OnButtonSamesttend() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	ApplySameShapeSttAndEnd();
	SetGridData();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CInputGenJongSection::OnButtonSameinwall() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	long nIdxInWallStd	= GetIdxCurInWall();
	ApplySameShapeInWall(nIdxInWallStd);
	SetGridData();
	SetDataInit();
	DrawInputDomyunView(FALSE);	
}


// 내측벽체 형상 동일 적용
// nIdxInWallStd : 기준이 되는 내측 벽체
void CInputGenJongSection::ApplySameShapeInWall(long nIdxInWallStd)
{
	CString str	= _T("");
	str.Format("모든 중간지점의 제원을 %d지점의 제원과 동일하게 적용하시겠습니까?", nIdxInWallStd+1);
	if(AfxMessageBox(str, MB_YESNO) != IDYES) return;

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	
	long nCountInWall	= pBri->GetCountInWall();
	if(nCountInWall <= 1) return;

	CWallApp *pWallOrg	= pBri->GetInWall(nIdxInWallStd);
	if(!pWallOrg) return;
	long nIdxHunchOrg	= pBri->GetIdxHunchInWall(nIdxInWallStd, TRUE);

	long wall = 0; for(wall = 0; wall < nCountInWall; wall++)
	{
		if(wall == nIdxInWallStd) continue;

		CWallApp *pWall	= pBri->GetInWall(wall);

		// 벽체 길이가 같아 지도록 수정 /////////////////////
		CLineInfo *pLine	= pBri->GetLineBase();
		double dStaStt	= pBri->GetStationOnJijum(nIdxInWallStd+1);
		double dStaEnd	= pBri->GetStationOnJijum(wall+1);
		double dELStt	= pLine->GetElevationPlanEl(dStaStt);
		double dELEnd	= pLine->GetElevationPlanEl(dStaEnd);
		pWall->m_dELB	= pWallOrg->m_dELB - (dELStt - dELEnd);
		//////////////////////////////////////////////////////

		
		pWall->m_bIs				= pWallOrg->m_bIs;
		pWall->m_nType				= pWallOrg->m_nType;
		pWall->m_bTopIsColumn		= pWallOrg->m_bTopIsColumn;
		pWall->m_dW					= pWallOrg->m_dW;
		if(wall < nCountInWall-1)
			pWall->m_dAngle			= 90 - pWall->GetAngleUpperSlab();
		else
			pWall->m_dAngle			= 180 - pWallOrg->m_dAngle;
		pWall->m_dH					= pWallOrg->m_dH;
		pWall->m_dAngleV			= pWallOrg->m_dAngleV;
		pWall->m_dWL				= pWallOrg->m_dWL;
		pWall->m_dWR				= pWallOrg->m_dWR;

		// 헌치
		long nIdxHunch = pBri->GetIdxHunchInWall(wall, TRUE);
		for(long upper=0; upper<2; upper++)
		{
			pBri->m_dHLW[nIdxHunch][upper]			= pBri->m_dHLW[nIdxHunchOrg][upper];
			pBri->m_dWL[nIdxHunch][upper]			= pBri->m_dWL[nIdxHunchOrg][upper];
			pBri->m_dHRW[nIdxHunch][upper]			= pBri->m_dHRW[nIdxHunchOrg][upper];
			pBri->m_dWR[nIdxHunch][upper]			= pBri->m_dWR[nIdxHunchOrg][upper];
			pBri->m_dHHL[nIdxHunch][upper]			= pBri->m_dHHL[nIdxHunchOrg][upper];
			pBri->m_dHHR[nIdxHunch][upper]			= pBri->m_dHHR[nIdxHunchOrg][upper];
			pBri->m_dHBH[nIdxHunch][upper]			= pBri->m_dHBH[nIdxHunchOrg][upper];

			pBri->m_dHLW[nIdxHunch+1][upper]		= pBri->m_dHLW[nIdxHunchOrg+1][upper];
			pBri->m_dWL[nIdxHunch+1][upper]			= pBri->m_dWL[nIdxHunchOrg+1][upper];
			pBri->m_dHRW[nIdxHunch+1][upper]		= pBri->m_dHRW[nIdxHunchOrg+1][upper];
			pBri->m_dWR[nIdxHunch+1][upper]			= pBri->m_dWR[nIdxHunchOrg+1][upper];
			pBri->m_dHHL[nIdxHunch+1][upper]		= pBri->m_dHHL[nIdxHunchOrg+1][upper];
			pBri->m_dHHR[nIdxHunch+1][upper]		= pBri->m_dHHR[nIdxHunchOrg+1][upper];
			pBri->m_dHBH[nIdxHunch+1][upper]		= pBri->m_dHBH[nIdxHunchOrg+1][upper];
		}


		// 기초
		pWall->m_footing.m_nType	= pWallOrg->m_footing.m_nType;
		if(pWallOrg->m_footing.m_nType == FOOTING_TYPE_LEFTSLOPE) pWall->m_footing.m_nType	= FOOTING_TYPE_RIGHTSLOPE;
		else if(pWallOrg->m_footing.m_nType == FOOTING_TYPE_RIGHTSLOPE) pWall->m_footing.m_nType	= FOOTING_TYPE_LEFTSLOPE;
		pWall->m_footing.m_dFH		= pWallOrg->m_footing.m_dFH;
		pWall->m_footing.m_dFW		= pWallOrg->m_footing.m_dFW;
		pWall->m_footing.m_dFSLW	= pWallOrg->m_footing.m_dFSLW;
		pWall->m_footing.m_dFSRH	= pWallOrg->m_footing.m_dFSRH;
		pWall->m_footing.m_dFSRW	= pWallOrg->m_footing.m_dFSRW;
		pWall->m_footing.m_dFSLH	= pWallOrg->m_footing.m_dFSLH;
		pWall->m_footing.m_dFH		= pWallOrg->m_footing.m_dFH;
		pWall->m_footing.m_dFWL		= pWallOrg->m_footing.m_dFWL;

			
		// 확대 기초
		CExFootingApp *pExFooting	 = &pWall->m_footing.m_exFooting;
		CExFootingApp *pExFootingOrg = &pWallOrg->m_footing.m_exFooting;

		pExFooting->m_bIs			= pExFootingOrg->m_bIs;
		pExFooting->m_nType			= pExFootingOrg->m_nType;
		pExFooting->m_nCountDan		= pExFootingOrg->m_nCountDan;
		pExFooting->m_bLeftOrgOfMass= pExFootingOrg->m_bLeftOrgOfMass;
		pExFooting->m_dEFWL			= pExFootingOrg->m_dEFWL;
		pExFooting->m_dEFWR			= pExFootingOrg->m_dEFWR;
		long i = 0; for(i = 0; i < pExFooting->m_nCountDan; i++)
		{
			pExFooting->m_dEFW[i]	= pExFootingOrg->m_dEFW[i];
			pExFooting->m_dEFS[i]	= pExFootingOrg->m_dEFS[i];
			pExFooting->m_dEFH[i]	= pExFootingOrg->m_dEFH[i];
		}
		pExFooting->m_dEFHR			= pExFootingOrg->m_dEFHR;
	}
}

// 시점과 종점의 단면 형상을 같게 만듬.
void CInputGenJongSection::ApplySameShapeSttAndEnd()
{
	if(AfxMessageBox("종점측 제원을 시점측과 동일하게 적용하시겠습니까?", MB_YESNO) != IDYES) return;

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	pBri->m_dFHE				= pBri->m_dFHS;
	pBri->m_dFLE				= pBri->m_dFLS;
	pBri->m_dFRE				= pBri->m_dFRS;
	pBri->m_dWE					= pBri->m_dWS;
	pBri->m_dBHWE				= pBri->m_dBHWS;
	pBri->m_dBHHE				= pBri->m_dBHHS;
	pBri->m_dBHE				= pBri->m_dBHS;
	pBri->m_dBTHE				= pBri->m_dBTHS;
	pBri->m_dBWE				= pBri->m_dBWS;
	pBri->m_dHWE				= pBri->m_dHWS;
	pBri->m_dHHE				= pBri->m_dHHS;

	pBri->m_dWEHH				= pBri->m_dWSHH;
	pBri->m_dWEHW				= pBri->m_dWSHW;

	// 벽체 길이가 같아 지도록 수정 /////////////////////
	CLineInfo *pLine	= pBri->GetLineBase();
	double dStaStt	= pBri->GetStationByTarget(BRIDGE_OFF_STT_STT, 0, TRUE);
	double dStaEnd	= pBri->GetStationByTarget(BRIDGE_OFF_END_END, 0, TRUE);
	double dELStt	= pLine->GetElevationPlanEl(dStaStt);
	double dELEnd	= pLine->GetElevationPlanEl(dStaEnd);
	pBri->m_dELEB	= pBri->m_dELSB - (dELStt - dELEnd);
	if(pBri->IsBoxType() && pBri->m_bSlopeLowerSlab == FALSE)
	{
		pBri->m_dELEB	 = pBri->m_dELSB;
	}
	//////////////////////////////////////////////////////

	pBri->m_footingEnd				= pBri->m_footingStt;
	pBri->m_footingEnd.m_dFSLW		= pBri->m_footingStt.m_dFSRW;
	pBri->m_footingEnd.m_dFSRW		= pBri->m_footingStt.m_dFSLW;
	pBri->m_footingEnd.m_dFWL		= pBri->m_footingStt.m_dFWR;
	pBri->m_footingEnd.m_dFWR		= pBri->m_footingStt.m_dFWL;

	pBri->m_footingEnd.m_exFooting.m_bLeftOrgOfMass	= !pBri->m_footingStt.m_exFooting.m_bLeftOrgOfMass;
}

// 해당 지점의 횡단면 보기
// 벽체가 있는 경우에만 보여짐(mass 입력때문에 사용)
void CInputGenJongSection::OnCheckViewCross() 
{
	// TODO: Add your control notification handler code here
	m_bCheckViewCross	= m_checkViewCross.GetCheck();
	DrawInputDomyunView(TRUE);
}

void CInputGenJongSection::OnCheckStation() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_HYUNGHA_STATION)->EnableWindow(m_bCheckStation);
	SetTextHyungHaElevation();
	UpdateData(FALSE);
}

void CInputGenJongSection::SetTextHyungHaElevation()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	CTwinVectorArray tvArr;

	CDPoint vAng(0,1);
	double dMinElevation = 0;
	if(m_bCheckStation)
	{
		AfxMessageBox("Station 값을 입력하신 경우 SKEW각도를 무시한 90도 적용 횡단면 값으로 검토됩니다.\n사교인 경우 자동계산된 값과 차이가 발생할 수 있습니다.");
		double dStationCheck = frM(atof(m_szHyungHaStation));
		double dElevation    = 0;
		if(dStationCheck<pBri->m_dStationBridgeStt)
		{
			dStationCheck = pBri->m_dStationBridgeStt;
		}
		if(dStationCheck>pBri->m_dStationBridgeStt+pBri->m_dLengthBridge) 
		{
			dStationCheck = pBri->m_dStationBridgeStt;
		}
//		double dThickSlab = pBri->GetHeightUpperSlab_OldOrNotUse(dStationCheck);
		double dThickSlab = pBri->GetHeightUpperSlab(pBri->GetJijumNumberByStation(dStationCheck), 0, CDPoint(0,1));
		if(Compare(dThickSlab,pBri->m_dTS,"="))
		{
			pBri->GetTvCrossSection(tvArr,dStationCheck,vAng,FALSE,TRUE,FALSE,0,FALSE,FALSE,FALSE,FALSE);
		}
		else
		{
			pBri->GetTvCrossHunch(tvArr,dStationCheck,vAng,FALSE,0,FALSE,FALSE);
		}
		long nSizeTv = tvArr.GetSize();
		long i=0; for(i=0; i<nSizeTv; i++)
		{
			CTwinVector tV = tvArr.GetAt(i);
			dMinElevation = min(tV.m_v1.y,tV.m_v2.y);
			if(i==0)
			{
				dElevation = dMinElevation;
			}
			else if(dElevation>dMinElevation)
			{
				dElevation = dMinElevation;
			}
		}
		m_szHyungHaStation.Format("%.3f",toM(dStationCheck));
		m_szHyungHaEL.Format("%.3f",toM(dElevation));
	}
	else
	{
		long nQtyJijum = pBri->GetCountJijum();
		double dStationCheck = 0;
		double dElevationCheck   = 0;
		for(long nJijum=0; nJijum<nQtyJijum; nJijum++)
		{
			double dStation   = pBri->GetStationOnJijum(nJijum);
			double dElevation = 0;
//			double dThickSlab = pBri->GetHeightUpperSlab_OldOrNotUse(dStation); 
			double dThickSlab = pBri->GetHeightUpperSlab(nJijum, 0, CDPoint(0,1));
			vAng = pBri->GetAngleJijum(nJijum);
			if(Compare(dThickSlab,pBri->m_dTS,"="))
			{
				pBri->GetTvCrossSection(tvArr,dStation,vAng,FALSE,TRUE,FALSE,0,FALSE,FALSE,FALSE,FALSE);
			}
			else
			{
				pBri->GetTvCrossHunch(tvArr,dStation,vAng,FALSE,0,FALSE,FALSE);
			}
			long nSizeTv = tvArr.GetSize();
			long i=0; for(i=0; i<nSizeTv; i++)
			{
				CTwinVector tV = tvArr.GetAt(i);
				dMinElevation = min(tV.m_v1.y,tV.m_v2.y);
				if(nJijum==0 && i==0)
				{
					dElevation = dMinElevation;
					dStationCheck = dStation;
					dElevationCheck = dElevation;
				}
				else if(dElevationCheck>dMinElevation)
				{
					dStationCheck = dStation;
					dElevationCheck = dMinElevation;
				}
			}
		}
		m_szHyungHaStation.Format("%.3f",toM(dStationCheck));
		m_szHyungHaEL.Format("%.3f",toM(dElevationCheck));
	}
	UpdateData(FALSE);
}

void CInputGenJongSection::OnKillfocusEditHyunghaStation() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	SetTextHyungHaElevation();
	UpdateData(FALSE);
}

void CInputGenJongSection::KillFocusDlg()
{
	// 종단제원에서 벗어날때면 좌우중심,직방향사방향에 대한 종단 좌표를 모두 구해 놓는다.
	// 다른 곳에서 종단 제원을 여러가지 방법으로 많이 이용하기 때문에 속도 개선을 위해서 구해놓는것임
	if(!m_pProgress->GetSafeHwnd())
	{
		m_pProgress->Create(IDD_DIALOG_HGPROGRESS, this);
	}
	m_pProgress->ShowWindow(SW_SHOW);
	m_pProgress->CenterWindow();
	m_pProgress->InitProgressData();

	CString str	= _T("");
	long nCountTotal		= m_pIn->m_pARcBridgeDataStd->GetBridgeSize() * 2;
	double dOffsetProgress	= 100. / nCountTotal;
	long nIdxProgress		= 0;
	for(long bri = 0; bri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		CRcBridgeRebar *pBri	= m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(bri);
		if(!pBri) continue;

		BOOL bSync	= pBri->m_bNeedSettingVertSection;

		
		if(bSync)
		{
			str.Format("[%s] 교량 전체 동기화 중......", pBri->m_strBridgeName);
			m_pProgress->SetProgressPos(str, (long)(nIdxProgress * dOffsetProgress));
			pBri->SyncBridge(0, TRUE, TRUE, TRUE);
		}
		nIdxProgress++;

		if(bSync)
		{
			str.Format("[%s] 교량 종단제원 동기화 중......", pBri->m_strBridgeName);
			m_pProgress->SetProgressPos(str, (long)(nIdxProgress * dOffsetProgress));
			pBri->SettingVertSection();
		}
		else
		{
			pBri->m_bSettingVertSection	= TRUE;
			pBri->m_bNeedSettingVertSection	= FALSE;
		}
		nIdxProgress++;
	}
		
	

	m_pProgress->SetProgressPos("동기화 완료", 100);
	m_pProgress->DestroyWindow();
	
	AhTPADelete(&m_ptvArrInWall, (CTwinVectorArray*)0);
	AhTPADelete(&m_ptvArrFooting, (CTwinVectorArray*)0);
	AhTPADelete(&m_ptvArrExFooting, (CTwinVectorArray*)0);
	
}

// 종단제원이 변경 될때 마다 해 줘야 되는 동기화만 골라서 여기서 동기화를 함.
void CInputGenJongSection::SyncBridge(long nLeft, BOOL bApplyAngle, BOOL bApplyAngleAps)
{
	CRcBridgeApp *pBri	= m_pIn->GetRcBridge();
	CRebarPlacing rb;
	pBri->m_bFirstCalledGetTvVertSection_Std	= FALSE;
	pBri->m_bSecondCalledGetTvVertSection_Std	= FALSE;

	pBri->GetTvVertSection(nLeft, bApplyAngle);
	// 외측벽체 좌표가 구해지면
	// 외측벽체 좌표의 하면으로 m_footingStt와 m_footingEnd의 m_tvPathLeft를 설정해줘야 된다.
	CWallApp wall;
	CTwinVectorArray tvArrSide, tvArrHidden;
	CTwinVector tv;

	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		pBri->SyncBridgeBoxType(bApplyAngle);
	}
	else
	{
		// 시점 기초
		// (ARCBRIDGE-3442) 벽체 위치가 곡선 선형인경우 종단에 표현되는 벽체 두께와 기초에서의 벽체 두께가 같지 않아 벽체와 기초의 연결부가 틀어지고
		// 치수 위치도 조금씩 어긋난다. 지점 각도를 조금 조정해서 벽체 두께를 맞춰 준다.
		double dAngStt = ToDegree(asin(min(1.0, pBri->m_dWS / pBri->GetThickOutWallByLine(TRUE))));
		double dAngEnd = ToDegree(asin(min(1.0, pBri->m_dWE / pBri->GetThickOutWallByLine(FALSE))));

		pBri->SyncBridge_CrossSection_SttEndFooting(bApplyAngle);
		pBri->m_footingStt.SyncFooting(bApplyAngle, dAngStt);
		pBri->m_footingEnd.SyncFooting(bApplyAngle, dAngEnd);

		pBri->TrimRibByFootingUpper();
		pBri->GetTvRibBlock();
	}
	
	// 접속슬래브 동기화(접속슬래브는 있을지 없을지 모를 수가 있다...) 그래서 옵션처리
	pBri->SyncBridgeAps(nLeft, bApplyAngleAps);

	// 지점 path 기초와 벽체에 설정
	long i = 0; for(i = 0; i < pBri->GetCountInWall(); i++)
	{
		CWallApp *pWall = pBri->GetInWall(i);
		if(pWall)
		{
			//SyncWall(pWall, i+1, bApplyAngle);
			pBri->SyncWall(i+1, pWall, bApplyAngle);

		}
	}
	pBri->TrimRibByConnectUpperSlab();

	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		pBri->m_tvArrVertSection.AddFromTvArray(pBri->m_tvArrVertSectionRib[iUPPER]);
		pBri->m_tvArrVertSection.AddFromTvArray(pBri->m_tvArrVertSectionRib[iLOWER]);
	}

	// 겹치는 선들에 대해서 체크함
	pBri->CheckOverLapLineBetweenSubObject();
}

void CInputGenJongSection::SyncWall(CWallApp *pWall, long nJijum, BOOL bApplyAngle)
{
	CRcBridgeApp *pBri	= m_pIn->GetRcBridge();

	// 경사교2타입인 경우 중간벽체 첫번째와 마지막번째는 EL이 자동조정된다. ////
	if((pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH || pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2) && pWall)
	{
		if(nJijum == 1 || nJijum == pBri->m_nQtyJigan-1)
		{
			CFootingApp *pFooting	= nJijum == 1 ? &pBri->m_footingStt : &pBri->m_footingEnd;
			CTwinVector tvSlope	= pFooting->m_tvArrFront.GetTvByInfo(nJijum==1 ? "기초우측경사" : "기초좌측경사");
			CDPoint xyMatch(0, 0);
			if(GetXyMatchLineAndLine(tvSlope.m_v1, tvSlope.GetXyDir(), pWall->m_xyOrg, ToDPointFrDegree(pWall->GetAngleWallByHor(bApplyAngle)), xyMatch))
			{
				pWall->m_dELB	= xyMatch.y;
			}
			pWall->m_footing = *pFooting;
			pWall->m_footing.m_bIs	= FALSE;
			pWall->m_footing.m_bVirtualFooting	= pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 ? TRUE : FALSE;
			pWall->m_footing.m_exFooting.m_bIs	= FALSE;
		}
		else
		{
			pWall->m_footing.m_bIs	= pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 ? TRUE : FALSE;
			pWall->m_footing.m_bVirtualFooting	= FALSE;
			pWall->m_footing.m_exFooting.m_bIs	= TRUE;
		}
	}
	//////////////////////////////////////////////////////////////////////////////

	CTwinVector tv = pBri->GetTvJijum(nJijum, 0);
	GetSwap(tv.m_v1, tv.m_v2);

	pWall->m_nTypeRebarFront	= pWall->IsVertWall() && pWall->m_tvArrLowerLimit_FrontRebar.GetSize() == 0 ? 1 : 0;
	pWall->m_dDiffTop[0]	= nJijum == 0 ? 0 : pBri->GetDiffELSttAndJijumForVertSection(nJijum, FALSE);
	if(pBri->IsBoxType())
	{
		pWall->m_dELBDiff	= 0;
		// Box 타입은 좌측 우측도 따로 구해서 저장하자.
		pWall->m_dDiffTop[0]	= pBri->GetDiffELSttAndJijumForVertSection(nJijum, (nJijum==0)? FALSE : TRUE, FALSE, TRUE);
		pWall->m_dDiffTop[1]	= pBri->GetDiffELSttAndJijumForVertSection(nJijum, (nJijum==0)? TRUE : FALSE, FALSE, TRUE);
	}
	else
	{
		pWall->m_dELBDiff	= pWall->m_dDiffTop[0];
		pWall->m_dDiffTop[1]	= pWall->m_dDiffTop[0];
	}

	// 내측벽체는 헌치까지만 구한다.
	CDPoint vAng = bApplyAngle ? pBri->GetAngleJijum(nJijum) : CDPoint(0, 1); 
// 	double dLenL	= 0;
// 	double dLenR	= 0;
	if(nJijum > 0 && nJijum < pBri->m_nQtyJigan && !pWall->m_bTopIsColumn)
	{
		double dLenL	= (pBri->m_dDH[0] + pBri->m_dCantilB[0])/vAng.y;
		double dLenR	= (pBri->m_dDH[1] + pBri->m_dCantilB[1])/vAng.y;
		tv.m_v2	= tv.m_v2 + tv.GetXyDir(TRUE) * dLenL;
		tv.m_v1	= tv.m_v1 + tv.GetXyDir() * dLenR;
	}

	// 벽체 양 끝부분의 각도와 path를 적용 ///////////////
	CDPoint vStt = pBri->GetAngleAzimuthTanOnJijum(nJijum, -1);
	CDPoint vEnd = pBri->GetAngleAzimuthTanOnJijum(nJijum, 1);

	pWall->m_tvPathPlan	= tv;
	pWall->m_tvPathPlanLower = tv;
	pWall->m_vAngLeft	= vStt;
	pWall->m_vAngRight	= vEnd;
	/////////////////////////////////////////////////////

	pBri->SyncWall_ColumnPlanJewon(pWall);

	pWall->m_Column.m_dAngPlan = pWall->GetXyDirTop().GetAngleDegree()+90;
	
	// 기둥객체에 EL 설정
	pWall->m_Column.m_dSta	= pWall->m_xyOrg.x;
	pWall->m_Column.m_xyCenInFront.x	= pWall->m_Column.m_dSta;
	pWall->m_Column.m_dEL	= pWall->m_xyOrg.y;
	pWall->m_Column.m_dELB	= pWall->m_dELB;
	pWall->m_Column.m_dELBDiff	= pWall->m_dELBDiff;

	// 기둥은 헌치와 슬래브 높이 고려해서 준다.
	CRebarPlacing rb;
	if(nJijum > 0 && nJijum < pBri->m_nQtyJigan)
	{
		long nIdxHunch	= pBri->GetIdxHunchInWall(nJijum-1, TRUE);
		pWall->m_Column.m_tvArrLeftUpper	= pWall->m_tvArrLeftUpper;
		rb.MoveRebar(pWall->m_Column.m_tvArrLeftUpper, CDPoint(0, -pBri->m_dTS-max(pBri->m_dHHL[nIdxHunch][iUPPER], pBri->m_dHHR[nIdxHunch][iUPPER])-pBri->m_dHBH[nIdxHunch][iUPPER]));
	}
	else
	{
		double dH	= nJijum == 0 ? pBri->m_dHHS : pBri->m_dHHE;
		pWall->m_Column.m_tvArrLeftUpper	= pWall->m_tvArrLeftUpper;
		rb.MoveRebar(pWall->m_Column.m_tvArrLeftUpper, CDPoint(0, -pBri->m_dTS-dH));
	}

	pWall->m_tvArrLowerLimit_Front.RemoveAll();
	pWall->m_tvArrLowerLimit_FrontRebar.RemoveAll();
	
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		pWall->m_tvArrLowerLimit_Front		= pBri->m_tvArrVertSectionRib[iUPPER];
		pWall->m_tvArrLowerLimit_FrontRebar	= pBri->m_tvArrVertSectionRib[iLOWER];
	}

	//31720
	CTwinVectorArray tvArrVertSection;
	tvArrVertSection.AddFromTvArray(pBri->m_tvArrVertSection);

	pWall->SyncWall(tvArrVertSection, bApplyAngle, pBri->GetAngleJijum(nJijum).GetAngleDegree());

	if(!pWall->IsVertWall())
	{
		CDPoint vAng	= ToDPointFrDegree(pWall->GetXyDirTop().GetAngleDegree() + 90 + (90 - pWall->m_dAnglePath));
		CTwinVector tvWallLower	= pWall->m_tvArrFront.GetTvByInfo("벽체하면좌측");
		CDPoint xyJijum	= pBri->GetXySlabUpperByOffsetFrStation(pBri->GetStationOnJijum(nJijum), 0, 0);
		double dDist	= xyJijum.x - tvWallLower.m_v1.x;
		pWall->m_tvPathPlanLower	= tv;
		pWall->m_tvPathPlanLower.m_v1 += vAng * dDist;
		pWall->m_tvPathPlanLower.m_v2 += vAng * dDist;
		pWall->GetTvPlan();
	}

	// 내측헌치 R에 대해서 구해줌(접선일 경우에만 구해주면 됨)
	pBri->ApplyRoundOfInWallHunch(nJijum-1, TRUE);
	pBri->ApplyRoundOfInWallHunch(nJijum-1, FALSE);

	// 중간벽체의 경우에만 아래값 넣어줌
	if(nJijum > 0 && nJijum < pBri->m_nQtyJigan)
	{
		pWall->m_dAngleUpperWall_Front	= pBri->GetVAngHunchLower(nJijum-1, 0, !bApplyAngle).GetAngleDegree();
		pBri->SyncWall_HunchLength(nJijum, pWall, bApplyAngle);
		// 종단이 변경되었을때...
		pBri->SyncWall_JongdanLimitFront(pWall, bApplyAngle);
		// 종단면 일반도 그릴때 필요 //
	}
	else
	{
		pWall->m_dThickUppeSlab	= 0;
		pWall->m_dThickHunch	= 0;
	}

	if(pWall->m_tvArrLowerLimit_Front.GetSize() > 0)
	{
		CDPointArray xyArrMatch;
		CRebarPlacing rb;
		double dSta	= pBri->GetStationOnJijum(nJijum);
		pWall->m_dELB	= pBri->m_pbRib[iUPPER].GetXyByDist(pBri->m_pbRib[iUPPER].GetDistByXPos(dSta)).y;
		pWall->m_Column.m_dELB	= pWall->m_dELB;
	}
}
	
// 리브 정의
void CInputGenJongSection::SetGridDataRib()
{
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 1, 1);
	m_Grid.SetCellSize(20, -1);

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH) return;

	long row = 0;
	long col = 0;
	CString str = _T("");
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;

	// 제목
	m_Grid.AddCell(row, col++, "구분");
	m_Grid.AddCell(row, col++, "Sta.");
	m_Grid.AddCell(row, col++, "EL.");
	//m_Grid.AddCell(row, col++, "D");
	m_Grid.AddCell(row, col++, "l");
	m_Grid.AddCell(row, col++, "f");
	m_Grid.AddCell(row, col++, "m");
	m_Grid.AddCell(row, col++, "O");
	m_Grid.AddCell(row, col++, "라이즈비");
	m_Grid.AddCell(row, col, "두께비");
	

	// 상부리브
	row++;
	col=0;
	m_Grid.AddCell(row, col++, "리브배선");
	m_Grid.AddCell(row, col++, &pBri->m_pbRib[iUPPER].m_xyCrown.x, nFormat, nLine, "리브배선 크라운 Sta.");
	m_Grid.AddCell(row, col++, &pBri->m_pbRib[iUPPER].m_xyCrown.y, nFormat, nLine, "리브배선 크라운 EL.(일반적으로 크라운의 두께는 Span의 1/4~1/60 범위내가 많다.)", CELL_TYPE_METER);
	m_Grid.AddCell(row, col++, &pBri->m_pbRib[iUPPER].m_dDistTotal, nFormat, nLine, "리브배선 스팬(라이즈비(f/l)은 1/3 ~ 1/6정도가 경제적임)");
	m_Grid.AddCell(row, col++, &pBri->m_pbRib[iUPPER].m_dHeightTotal, nFormat, nLine, "리브배선 라이즈(라이즈비(f/l)은 1/3 ~ 1/6정도가 경제적임) 두께비가 1.5이하인 경우는 구조계산시 두께가 동일한 것으로 봄.");
	m_Grid.AddCell(row, col++, &pBri->m_pbRib[iUPPER].m_dFactorM, nFormat, nLine, "리브배선 Ws/Wc.");
	m_Grid.AddCell(row, col++, &pBri->m_pbRib[iUPPER].m_dLenArchLine, nFormat, nLine, "리브 표현시 단위선 길이");
	pBri->m_pbRib[iUPPER].SetCalcRate();
	m_Grid.AddCell(row, col++, &pBri->m_pbRib[iUPPER].m_dRatefl, nFormat, CELL_READONLY, "라이즈비(f/l)은 1/3 ~ 1/6정도가 경제적임");
	m_Grid.AddCell(row, col, "", nFormat, CELL_READONLY);
	

	// 중심선 리브
	row++;
	col=0;
	m_Grid.AddCell(row, col++, "리브축선");
	m_Grid.AddCell(row, col++, &pBri->m_pbRibCen.m_xyCrown.x, nFormat, nLine, "리브축선 크라운 Sta.");
	m_Grid.AddCell(row, col++, &pBri->m_pbRibCen.m_xyCrown.y, nFormat, nLine, "리브축선 크라운 EL.(일반적으로 크라운의 두께는 Span의 1/4~1/60 범위내가 많다.)", CELL_TYPE_METER);
	m_Grid.AddCell(row, col++, &pBri->m_pbRibCen.m_dDistTotal, nFormat, nLine, "리브축선 스팬(라이즈비(f/l)은 1/3 ~ 1/6정도가 경제적임)");
	m_Grid.AddCell(row, col++, &pBri->m_pbRibCen.m_dHeightTotal, nFormat, nLine, "리브축선 라이즈(라이즈비(f/l)은 1/3 ~ 1/6정도가 경제적임) 두께비가 1.5이하인 경우는 구조계산시 두께가 동일한 것으로 봄.");
	m_Grid.AddCell(row, col++, &pBri->m_pbRibCen.m_dFactorM, nFormat, nLine, "리브축선 Ws/Wc.");
	m_Grid.AddCell(row, col++, &pBri->m_pbRibCen.m_dLenArchLine, nFormat, nLine, "리브 표현시 단위선 길이");
	pBri->m_pbRibCen.SetCalcRate();
	m_Grid.AddCell(row, col++, &pBri->m_pbRibCen.m_dRatefl, nFormat, CELL_READONLY, "라이즈비(f/l)은 1/3 ~ 1/6정도가 경제적임");
	double dThickRate	= pBri->GetThickArchRibForCalc(CHECK_POS_LEFT, TRUE)/pBri->GetThickArchRibForCalc(CHECK_POS_CENTER, TRUE);
	m_Grid.AddCell(row, col, m_Grid.GetStrDouble(m_Grid.GetCellLast(), dThickRate), nFormat, CELL_READONLY);


	// 하부리브
	row++;
	col=0;
	m_Grid.AddCell(row, col++, "리브복선");
	m_Grid.AddCell(row, col++, &pBri->m_pbRib[iLOWER].m_xyCrown.x, nFormat, nLine, "리브복선 크라운 Sta.");
	m_Grid.AddCell(row, col++, &pBri->m_pbRib[iLOWER].m_xyCrown.y, nFormat, nLine, "리브복선 크라운 EL.(일반적으로 크라운의 두께는 Span의 1/4~1/60 범위내가 많다.)", CELL_TYPE_METER);
	m_Grid.AddCell(row, col++, &pBri->m_pbRib[iLOWER].m_dDistTotal, nFormat, nLine, "리브복선 스팬(라이즈비(f/l)은 1/3 ~ 1/6정도가 경제적임)");
	m_Grid.AddCell(row, col++, &pBri->m_pbRib[iLOWER].m_dHeightTotal, nFormat, nLine, "리브복선 라이즈(라이즈비(f/l)은 1/3 ~ 1/6정도가 경제적임) 두께비가 1.5이하인 경우는 구조계산시 두께가 동일한 것으로 봄.");
	m_Grid.AddCell(row, col++, &pBri->m_pbRib[iLOWER].m_dFactorM, nFormat, nLine, "리브복선 Ws/Wc.");
	m_Grid.AddCell(row, col++, &pBri->m_pbRib[iLOWER].m_dLenArchLine, nFormat, nLine, "리브 표현시 단위선 길이");
	pBri->m_pbRib[iLOWER].SetCalcRate();
	m_Grid.AddCell(row, col++, &pBri->m_pbRib[iLOWER].m_dRatefl, nFormat, CELL_READONLY, "라이즈비(f/l)은 1/3 ~ 1/6정도가 경제적임");
	m_Grid.AddCell(row, col, "", nFormat, CELL_READONLY);

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(0, 100);
	m_Grid.SetColumnWidth(col-2, 100);
	m_Grid.SetColumnWidth(col-1, 100);

}

void CInputGenJongSection::SetGridDataPRF()
{
	//그리드 초기화
	m_Grid.SetGridDefault(2, 2, 1, 0);
	m_Grid.SetCellSize(20, -1);

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	CDPointArray xyArr;	// 배경색 반전위치
	CStringArray sArr;
	CString str = _T("");
	CString strHelp = _T("");
	long row = 0;
	long col = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;

	//Title
	m_Grid.AddCell(row, col++, "설치");
	m_Grid.AddCell(row, col++, "brW");
	m_Grid.AddCell(row, col++, "brH1");
	m_Grid.AddCell(row, col, "brH2");

	row++;
	col = 0;
	//내용
	m_Grid.AddCell(row, col++, &pBri->m_bExistProtectionRiseFooting, nFormat, nLine, "", CELL_TYPE_CHECK);
	nLine = pBri->m_bExistProtectionRiseFooting ? CELL_LINE_NO : CELL_READONLY;
	m_Grid.AddCell(row, col++, &pBri->m_dPRF_W, nFormat, nLine, "");
	m_Grid.AddCell(row, col++, &pBri->m_dPRF_H1, nFormat, nLine, "");
	m_Grid.AddCell(row, col++, &pBri->m_dPRF_H2, nFormat, nLine, "");

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(0, 70);

}

void CInputGenJongSection::SetGridDataLowerSlab()
{
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 1, 0);
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_BOX) return;

	CDPointArray xyArr;	// 배경색 반전위치
	CStringArray sArr;
	CString str = _T("");
	CString strHelp = _T("");
	long row = 0;
	long col = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;

	CFootingApp *pFooting = &pBri->m_footingBox;

	// 기초 형식 (직접기초,MASS기초,말뚝기초,잡석기초,부력방지 Anchor)
	sArr.RemoveAll();
	sArr.Add("직접기초");
	sArr.Add("MASS기초");
	sArr.Add("말뚝기초");
	sArr.Add("잡석기초");
	sArr.Add("부력방지 Anchor");

	m_Grid.AddCell(row, col, _T("기초형식")); 
	m_Grid.AddCell(row+1, col++, &pBri->m_nTypeLower, nFormat, TRUE, nLine, "기초형식", CELL_TYPE_COMBO, sArr);

	m_Grid.AddCell(row, col, "종단적용"); m_Grid.AddCell(row+1, col++, &pBri->m_bSlopeLowerSlab, nFormat, nLine, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col, "T"); m_Grid.AddCell(row+1, col++, &pBri->m_dTS_Lower, nFormat, nLine, _T("하부슬래브 두께"));

	if(pBri->m_nTypeLower == LOWERSLAB_TYPE_COBBSTONE)
	{
		m_Grid.AddCell(row, col, "Tc"); m_Grid.AddCell(row+1, col++, &pBri->m_dThickCobbStone, nFormat, nLine, _T("잡석기초 두께"));
	}

	// 단차는 적용하지 않는다.....
	if(pBri->m_nTypeLower == LOWERSLAB_TYPE_MASS)
	{
		sArr.RemoveAll();
		long ix = 0; for(ix = 0; ix < MAX_MASS_DAN; ix++)
		{
			if(ix == 0) str = "단없음";
			else str.Format("%d단", ix);
			sArr.Add(str);
		}
		m_Grid.AddCell(row, col, "MASS단수"); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_nCountDan, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);

		sArr.RemoveAll();
		sArr.Add("교축");
		sArr.Add("교축직각");
		m_Grid.AddCell(row, col, "단차방향"); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_bFrontDanMass, nFormat, nLine, "", CELL_TYPE_COMBO, sArr);

		sArr.RemoveAll();
		sArr.Add("좌측");
		sArr.Add("우측");
		m_Grid.AddCell(row, col, "MASS기준면"); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_bLeftOrgOfMass, nFormat, nLine, "", CELL_TYPE_COMBO, sArr);
	}
	m_Grid.AddCell(row, col, "EFL"); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_dEFWL);
	m_Grid.AddCell(row, col, "EFR"); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_dEFWR);
	m_Grid.AddCell(row, col, "EFB"); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_dEFWB);
	m_Grid.AddCell(row, col, "EFF"); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_dEFWF);
	if(pBri->m_nTypeLower == LOWERSLAB_TYPE_MASS)
	{
		long i = 0; for(i = 0; i < pFooting->m_exFooting.m_nCountDan; i++)
		{
			str.Format("%d", i+1);
			m_Grid.AddCell(row, col, "EW"+str); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_dEFW[i]);
			m_Grid.AddCell(row, col, "ES"+str); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_dEFS[i]);
			m_Grid.AddCell(row, col, "EH"+str); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_dEFH[i]);
		}
	}
	
	m_Grid.AddCell(row, col, "EH"); m_Grid.AddCell(row+1, col++, &pFooting->m_exFooting.m_dEFHR);
	
	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(0, 120);
	m_Grid.SetColumnWidth(1, 70);
	
	if(pBri->m_nTypeLower == LOWERSLAB_TYPE_MASS)
	{
		m_Grid.SetColumnWidth(3, 80);
		m_Grid.SetColumnWidth(4, 80);
		m_Grid.SetColumnWidth(5, 80);
	}

	m_Grid.SetRedraw(TRUE, TRUE);
}

void CInputGenJongSection::SetGridDataInWallBox()
{
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 1, 1);
	m_Grid.SetCellSize(-1, -1);

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	
	long i = 0;
	long row = 0;
	long col = 0;
	long nStt = 0;
	long nEnd = 0;
	CString str = _T("");
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;


	// 벽체
//	long nColOfKindStt = -1;
// 	long nColOfKindEnd = -1;

	CStringArray sArr;
	sArr.RemoveAll(); sArr.Add("기둥식"); sArr.Add("벽체식");
	m_Grid.AddCell(row, col++, "구분");

	long nColOfKindStt = col;
	m_Grid.AddCell(row, col++, "단면형상");

	// 벽체식인 경우 끝단 라운드 타입 적용 입력받음
	long nColOfKindEnd	= col;
	m_Grid.AddCell(row, col++, "끝단라운드");

	m_Grid.AddCell(row, col++, "EL.(m)");
	m_Grid.AddCell(row, col++, "W");

	// 상부 헌치
	m_Grid.AddCell(row, col++, "HHL");
	m_Grid.AddCell(row, col++, "HLW");
	m_Grid.AddCell(row, col++, "WL");
	m_Grid.AddCell(row, col++, "HHR");
	m_Grid.AddCell(row, col++, "HRW");
	m_Grid.AddCell(row, col++, "WR");
	m_Grid.AddCell(row, col++, "HB");
	// 하부 헌치
	m_Grid.AddCell(row, col++, "LLH");
	m_Grid.AddCell(row, col++, "LLW");
	m_Grid.AddCell(row, col++, "LWL");
	m_Grid.AddCell(row, col++, "LRH");
	m_Grid.AddCell(row, col++, "LRW");
	m_Grid.AddCell(row, col++, "LWR");
	m_Grid.AddCell(row, col, "LHB");
	col = 0;
	row++;
	
	for(long ix=0; ix<pBri->GetCountInWall(); ix++)
	{
		// 벽체가 없어도 지간이 있으면 일단 생성
		CWallApp *pWall = pBri->GetInWall(ix);
		if(!pWall) return;

		long nIdxHunch = pBri->GetIdxHunchInWall(ix, TRUE);

		str.Format(_T("P%d"), ix+1);

		m_Grid.AddCell(row, col++, str);
		m_Grid.AddCell(row, col++, &pWall->m_bTopIsColumn, nFormat, nLine, "", CELL_TYPE_COMBO, sArr);

		// 벽체식인 경우 끝단 라운드 타입 적용 입력받음
		if(pWall->IsEnableRoundRectWall())
		{
			m_Grid.AddCell(row, col++, &pWall->m_bRoundRect, nFormat, nLine, "", CELL_TYPE_CHECK);
		}
		else
		{
			m_Grid.AddCell(row, col++, _T(""),	nFormat, CELL_READONLY);
		}
		nColOfKindEnd	= col;

		if(pWall->m_bIs)
		{
			m_Grid.AddCell(row, col++, &pWall->m_dELB, nFormat, CELL_READONLY, "", CELL_TYPE_METER); m_Grid.SetCellDec(CELL_DEC_3);		
			m_Grid.AddCell(row, col++, &pWall->m_dW, nFormat, nLine, "벽체 폭");
		}

		// 상부 헌치
		m_Grid.AddCell(row, col++, &pBri->m_dHHL[nIdxHunch][iUPPER]);
		m_Grid.AddCell(row, col++, &pBri->m_dHLW[nIdxHunch][iUPPER]);
		m_Grid.AddCell(row, col++, &pBri->m_dWL[nIdxHunch][iUPPER]);
		m_Grid.AddCell(row, col++, &pBri->m_dHHR[nIdxHunch][iUPPER], nFormat, CELL_READONLY, "HHL값과 동일하게 적용됨");
		m_Grid.AddCell(row, col++, &pBri->m_dHRW[nIdxHunch][iUPPER]);
		m_Grid.AddCell(row, col++, &pBri->m_dWR[nIdxHunch][iUPPER]);
		m_Grid.AddCell(row, col++, &pBri->m_dHBH[nIdxHunch][iUPPER]);
		// 하부 헌치
		m_Grid.AddCell(row, col++, &pBri->m_dHHL[nIdxHunch][iLOWER]);
		m_Grid.AddCell(row, col++, &pBri->m_dHLW[nIdxHunch][iLOWER]);
		m_Grid.AddCell(row, col++, &pBri->m_dWL[nIdxHunch][iLOWER]);
		m_Grid.AddCell(row, col++, &pBri->m_dHHR[nIdxHunch][iLOWER], nFormat, CELL_READONLY, "LLH값과 동일하게 적용됨");
		m_Grid.AddCell(row, col++, &pBri->m_dHRW[nIdxHunch][iLOWER]);
		m_Grid.AddCell(row, col++, &pBri->m_dWR[nIdxHunch][iLOWER]);
		m_Grid.AddCell(row, col, &pBri->m_dHBH[nIdxHunch][iLOWER]);
		col = 0;
		row++;
	}
	
	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(1, 70);
	if(nColOfKindEnd > -1)	
	{
		for(long c = nColOfKindStt; c < nColOfKindEnd; c++)
		{
			m_Grid.SetColumnWidth(c, 100);
		}
	}
	
	// 셀배경 반전.
	COLORREF clr = GRID_TEXT_BK_COLOR_REV;
	for(i = nStt; i < nEnd; i++)
	{
		m_Grid.SetItemBkColour(1, i, clr);
	}

	m_Grid.SetRedraw(TRUE, TRUE);
}

void CInputGenJongSection::SetGridDataOutWallBox()
{
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 1, 1);
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	CDPointArray xyArr;	// 배경색 반전위치
	CStringArray sArr;
	CString str = _T("");
	CString strHelp = _T("");
	long row = 0;
	long col = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;

	m_Grid.AddCell(row, col++, "구분");
	m_Grid.AddCell(row, col++, "EL.(m)");
	m_Grid.AddCell(row, col++, "BH");
	m_Grid.AddCell(row, col++, "BTH");
	m_Grid.AddCell(row, col++, "BHH");
	m_Grid.AddCell(row, col++, "BHW");
	m_Grid.AddCell(row, col++, "HH");
	m_Grid.AddCell(row, col++, "HW");
	m_Grid.AddCell(row, col++, "LHH");
	m_Grid.AddCell(row, col++, "LHW");
	m_Grid.AddCell(row, col, "W");
	col = 0;
	row++;

	for(long nStt=0; nStt<2; nStt++)
	{
		BOOL bStt = (nStt == 0)? TRUE : FALSE;
		CJijum* pJijum = pBri->GetJijum(bStt? 0 : pBri->m_nQtyJigan);

		m_Grid.AddCell(row, col++, pJijum->m_strJijumShoe[0]);
		m_Grid.AddCell(row, col++, bStt? &pBri->m_dELSB : &pBri->m_dELEB, nFormat, bStt? nLine : CELL_READONLY, "하단 EL.", CELL_TYPE_METER); m_Grid.SetCellDec(CELL_DEC_3);
		m_Grid.AddCell(row, col++, bStt? &pBri->m_dBHS : &pBri->m_dBHE);
		m_Grid.AddCell(row, col++, bStt? &pBri->m_dBTHS : &pBri->m_dBTHE, nFormat);
		m_Grid.AddCell(row, col++, bStt? &pBri->m_dBHHS : &pBri->m_dBHHE);
		m_Grid.AddCell(row, col++, bStt? &pBri->m_dBHWS : &pBri->m_dBHWE);
		m_Grid.AddCell(row, col++, bStt? &pBri->m_dHHS : &pBri->m_dHHE);
		m_Grid.AddCell(row, col++, bStt? &pBri->m_dHWS : &pBri->m_dHWE);
		m_Grid.AddCell(row, col++, bStt? &pBri->m_dWSHH : &pBri->m_dWEHH);
		m_Grid.AddCell(row, col++, bStt? &pBri->m_dWSHW : &pBri->m_dWEHW);
		m_Grid.AddCell(row, col, bStt? &pBri->m_dWS : &pBri->m_dWE, nFormat, nLine, "시점벽체 폭");

		col = 0;
		row++;
	}

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(1, 70);

	m_Grid.SetRedraw(TRUE, TRUE);
}

void CInputGenJongSection::OnSelchangeComboBoring() 
{
	CRcBridgeApp *pBri = m_pIn->m_pARcBridgeDataStd->GetBridge(m_pIn->m_nCurrentBridge);
 	if(!pBri) return;

 	SetDataInitBoring();
	DrawInputDomyunView(m_checkViewCross.GetCheck() ? TRUE : FALSE);
	UpdateData(FALSE);
}

void CInputGenJongSection::SetDataInitBoring(HGBOOL bUpdateUI)
{
// 	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	UpdateData(TRUE);
	HGINT32 ixBoring = m_comboBoring.GetCurSel();

	CFootingApp *pFooting = GetCurrentFooting();
	if( NULL == pFooting ) return;

	if( bUpdateUI == TRUE )
	{
		ixBoring = pFooting->GetBoringIndex();
		m_dBoringEL = pFooting->GetBoringEL();
	}
	else
	{
		if( ixBoring == -1 )
			ixBoring = 0;
		pFooting->SetBoringData(m_dBoringEL, ixBoring);
	}
	
	m_comboBoring.SetCurSel(ixBoring);
	UpdateData(FALSE);
}

void CInputGenJongSection::OnEnKillfocusEditEl()
{
	CFootingApp *pFooting = GetCurrentFooting();
	if( NULL == pFooting ) return;
	UpdateData(TRUE);
	pFooting->SetBoringData(m_dBoringEL, m_comboBoring.GetCurSel());
}
