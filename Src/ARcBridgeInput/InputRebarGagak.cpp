// InputRebarGagak.cpp : implementation file
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
// CInputRebarGagak dialog


CInputRebarGagak::CInputRebarGagak(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputRebarGagak::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputRebarGagak)
	m_nTab	= 0;
	m_nCross = -1;
	//}}AFX_DATA_INIT
}


void CInputRebarGagak::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputRebarGagak)
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_comboBridgeNo);
	DDX_Control(pDX, IDC_COMBO_CTC, m_comboCTC);
	DDX_Control(pDX, IDC_BUTTON_EXAMINE, m_btnExamine);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	DDX_Radio(pDX, IDC_RADIO_CROSS, m_nCross);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputRebarGagak, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputRebarGagak)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_BN_CLICKED(IDC_RADIO_CROSS, OnRadioCross)
	ON_BN_CLICKED(IDC_RADIO_PLANE, OnRadioPlane)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW,  IDC_GRID, OnCellChangedRow)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputRebarGagak message handlers

BOOL CInputRebarGagak::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CInputDialogBase::PreTranslateMessage(pMsg);
}

void CInputRebarGagak::SetDataSave()
{
	m_Grid.UpdateData(TRUE);

	m_pIn->m_pDoc->SetModifiedFlag();
}

void CInputRebarGagak::SetDataInit()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	m_Grid.UpdateData(FALSE);

	CHgBaseConcStd baseConc;
	m_comboCTC.SetCurSel(baseConc.GetIdxByRebarCTC(pBri->m_dCTC_MainRebar));
}

void CInputRebarGagak::SetGridData()
{
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 1, 1);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;
	CStringArray sArr, sArrType;
	CString str	= _T("");

	sArr.RemoveAll();
	sArr.Add("주철근 기준 배치");
	sArr.Add("배력철근 기준 배치");

	sArrType.RemoveAll();
	sArrType.Add("직선 철근");
	sArrType.Add("곡선 철근");

	long row(0), col(0);
	m_Grid.AddCell(row, col, "위치");
	m_Grid.AddCell(row, col+1, "가각부 주철근 배치 기준");
	m_Grid.AddCell(row, col+2, "철근 연장 거리");
	m_Grid.AddCell(row, col+3, "철근타입");
	m_Grid.AddCell(row, col+4, "선형방향배치");
	m_Grid.AddCell(row, col+5, "보강철근개수");
	row++;

	for(long stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT;
			BOOL bReadOnly	= pBri->m_nTypeRebarMainRebarOfExp[stt][left] == 1;

			str.Format("%s %s(%s%s)", stt==iSTT ? "시점" : "종점", left==iLEFT ? "좌측" : "우측", bStt ? "S" : "E", bLeft ? "L" : "R");

			m_Grid.AddCell(row, col, str);
			m_Grid.AddCell(row, col+1, &pBri->m_nTypeOrgMainRebarOfExp[stt][left], nFormat, TRUE, bReadOnly ? CELL_READONLY : nLine, "", CELL_TYPE_COMBO, sArr);
			m_Grid.AddCell(row, col+2, &pBri->m_dLenMainRebarOfExp[stt][left]);
			m_Grid.AddCell(row, col+3, &pBri->m_nTypeRebarMainRebarOfExp[stt][left], nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArrType);
			m_Grid.AddCell(row, col+4, &pBri->m_bLineDirRebarOfExp[stt][left], nFormat, nLine, "", CELL_TYPE_CHECK);
			m_Grid.AddCell(row, col+5, &pBri->m_nCountSupportRebarOfExpAtLineDirRebar[stt][left], nFormat, FALSE, pBri->m_bLineDirRebarOfExp[stt][left] ? nLine : CELL_READONLY, "");
			row++;
		}
	}

	long nRows(0), nCols(0);
	m_Grid.GetCellCount(nRows, nCols);
	m_Grid.SetRows(nRows);
	m_Grid.SetCols(nCols);

	m_Grid.SetColumnWidth(0, 100);
	m_Grid.SetColumnWidth(1, 300);
	m_Grid.SetColumnWidth(2, 150);
	m_Grid.SetColumnWidth(3, 100);
}

void CInputRebarGagak::DrawInputDomyunView(BOOL bZoomAll)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(m_pIn->GetScaleInputDialog());

	CARcBridgeDBStandard DBStd(m_pIn->GetRcBridgeRebar(), m_pIn->m_pARoadOptionStd, m_pIn->m_pARcBridgeDataStd);

	BOOL bStt	= IsStt();
	BOOL bLeft	= IsLeft();
	BOOL bCross	= m_nCross == 0;


	CDomyun Dom(pDom);
	CHgBaseDrawStd baseDraw;

	if(bCross)
	{
		BOOL bInner	= pBri->m_nTypeWallForRebarGen == 1;

		DBStd.m_pDrawCrossSection->DrawCrossAllAtJijum(&Dom, bStt ? 0 : pBri->m_nQtyJigan, TRUE, FALSE, TRUE, FALSE);
		if(bStt)
		{
			double dMid	= pBri->GetMidCrossSection(FALSE);
			Dom.Mirror(dMid, TRUE);
		}

		CTwinVectorArray tvArrRebarMainSide;
		DBStd.m_pDrawCrossSection->DrawCrossSection_OutWall_Exp(&Dom, bStt, bInner, bInner, !bStt);
		DBStd.m_pDrawCrossSection->DrawRebarMain_CrossSection_OutWallExp(&Dom, bStt, 0, bInner, bInner, 0, TRUE, TRUE, !bStt, tvArrRebarMainSide);
		//DBStd.m_pDrawCrossSection->DrawRebarMain_CrossSection_OutWallExp(&Dom, bStt, FALSE, bInner, 0, TRUE, TRUE, !bStt);
	}
	else
	{
		DBStd.m_pDrawPlane->DrawPlaneGen(&Dom, 5000, TRUE, FALSE, TRUE, FALSE, FALSE);
		CTwinVectorArray tvArrRebarOut;
		pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrRebarOut, 0, TRUE);
		m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
		baseDraw.DrawTvArray(&Dom, tvArrRebarOut);
		
		DBStd.m_pDrawPlane->DrawRebarMain_Plane_Slab_Exp(&Dom, TRUE, TRUE, 0, FALSE, FALSE, -1, TRUE);
		DBStd.m_pDrawPlane->DrawRebarMain_Plane_Slab_Exp(&Dom, TRUE, TRUE, 0, FALSE, FALSE, 1, TRUE);

		// 철근 배치 기준선
		DBStd.m_pDrawPlane->DrawMainRebarOrgLineOfExp(&Dom, bStt, bLeft);
#ifdef _DEBUG
		//DBStd.RotateDomByBridgeSttEndXy(&Dom);
#else
		DBStd.RotateDomByBridgeSttEndXy(&Dom);
#endif
	}

	*pDom << Dom;	

	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

void CInputRebarGagak::OnPreInitDialog()
{
	// 시작하면서 pick box의 크기를 정해줌
	if(m_pView) 
	{
		CTwinVector tv;
		tv.SetPickBoxSize(m_pView->GetSelectionRectSize()/m_pView->GetRatioWindow());
	}
	
	InitTabCtrl();
	InitComboCtrl();


	SetResize(IDC_BUTTON_EXAMINE,		SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
	SetResize(IDC_TAB,					SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID,					SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_RADIO_CROSS,			SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_RADIO_PLANE,			SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_VIEW,			SZ_TOP_RIGHT,		SZ_TOP_RIGHT);

	SetHyperHelp(m_pIn->GetManualPath());

	m_Grid.InitGrid(m_pIn->m_pARcBridgeDataStd->m_Fy);
	SetGridData();
	SetDataInit();
}

BOOL CInputRebarGagak::OnInitDialog() 
{
	CInputDialogBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputRebarGagak::InitTabCtrl()
{
	m_Tab.RemoveAllTabs();
	m_Tab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_Tab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	// 사이클 탭
	CStringArray sArrTabName;
	long nTab		= 0;
	sArrTabName.Add("가각부 주철근"); m_nTab	= nTab++;
	long i = 0; for(i = 0; i < sArrTabName.GetSize(); i++)
	m_Tab.InsertItem(i, sArrTabName[i]);
	m_Tab.SetCurSel(m_nTab);
}

void CInputRebarGagak::InitComboCtrl()
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

	// CTC
	for(long ctc = 0; ctc < MAX_REBAR_CTC; ctc++)
	{
		CHgBaseConcStd baseConcStd;
		str.Format("CTC %.0f", baseConcStd.GetValueRebarCTC(ctc));
		m_comboCTC.AddString(str);
	}

	SetControl();
}

void CInputRebarGagak::OnSelchangeComboBridgeNo() 
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

void CInputRebarGagak::SetControl()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	m_nCross	= 1;
	m_comboCTC.EnableWindow(FALSE);
	UpdateData(FALSE);
}

void CInputRebarGagak::SetDataDefault()
{
	for(long bri = 0; bri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(bri);
		if(!pBri) continue;

		BOOL bFirst   = IsFirst(bri);
		pBri->SetDefaultedState(GetIDD(), 0x00000001);
		if(!bFirst && !IsClickDefaultButton(bri)) continue;

		for(long stt = 0; stt < 2; stt++)
		{
			for(long left = 0; left < 2; left++)
			{
				pBri->m_nTypeOrgMainRebarOfExp[stt][left]	= 1;
				pBri->m_dLenMainRebarOfExp[stt][left]	= pBri->GetExtMainRebarLimitGagak(stt==iSTT, left ==iLEFT); // 1500;
				pBri->m_bLineDirRebarOfExp[stt][left]	= FALSE;
				pBri->m_nCountSupportRebarOfExpAtLineDirRebar[stt][left]	= 1;
			}
		}
	}

	DrawInputDomyunView(FALSE);
}

BOOL CInputRebarGagak::IsStt()
{
	long nRow	= m_Grid.GetFocusCell().row;
	if(nRow == 1 || nRow == 2) return TRUE;

	return FALSE;
}

BOOL CInputRebarGagak::IsLeft()
{
	long nRow	= m_Grid.GetFocusCell().row;

	if(nRow == 1 || nRow == 3) return TRUE;

	return FALSE;
}

void CInputRebarGagak::OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result)
{
	DrawInputDomyunView(FALSE);
	
	CString str	= _T("");
	str.Format("%s%s", IsStt() ? "S" : "E", IsLeft() ? "L" : "R");
	m_pView->GetDomyun()->STMakeCursor(str);
}

void CInputRebarGagak::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);
	SetGridData();
	SetDataInit();
}

void CInputRebarGagak::OnRadioCross() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DrawInputDomyunView(FALSE);
}

void CInputRebarGagak::OnRadioPlane() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DrawInputDomyunView(FALSE);	
}

BOOL CInputRebarGagak::IsValid()
{
	BOOL bIsExp	= FALSE;
	if(!m_pIn) return FALSE;
	if(!m_pIn->m_pARcBridgeDataStd) return FALSE;

	long nCountBri	= m_pIn->m_pARcBridgeDataStd->GetBridgeSize();
	for(long bri = 0; bri < nCountBri; bri++)
	{
		CRcBridgeRebar *pBri	= m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(bri);
		if(pBri)
		{
			if(pBri->m_bIsExp) 
			{
				bIsExp	= TRUE;
				break;
			}
		}
	}

	return bIsExp;
}
