// InputRebarOthers.cpp : implementation file
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
// CInputRebarOthers dialog


CInputRebarOthers::CInputRebarOthers(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputRebarOthers::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputRebarOthers)
	m_nTabCenterGuard = -1;
	m_nTabRetainingWall = -1;
	m_nTabSoundProof = -1;
	m_nTabGuardHalf = -1;
	m_nTabHandRail = -1;
	m_nTabMonument = -1;
	m_nTabNonRebar = -1;
	m_nTabSoundProofType = -1;
	m_nTabHole = -1;
	m_nTabFootwayBlock = -1;
	m_nTabHoleFootway = -1;
	m_nTabRailGuardFence = -1;
	m_nTabGuardFence = -1;
	m_nTabDetectGravel = -1;
	m_bPosition		= FALSE;
	m_nGWGugan		= 0;
	m_nUpperGugan	= 0;
	m_nSideGugan	= 0;
	m_dFck			= 0.0;
	m_dFy			= 0.0;
	m_sFck			= _T("");
	m_sFy			= _T("");
	m_bGWhalf		= FALSE;
	m_dAggregate = 0.0;
	//}}AFX_DATA_INIT
}

void CInputRebarOthers::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputRebarOthers)
	DDX_Control(pDX, IDC_STATIC_UPPER_GUGAN, m_sUpperGugan);
	DDX_Control(pDX, IDC_STATIC_SIDE_GUGAN, m_sSideGugan);
	DDX_Control(pDX, IDC_STATIC_GW_GUGAN, m_sGWGugan);
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_cbBridgeNo);
	DDX_Control(pDX, IDC_COMBO_GW_GUGAN, m_cbGWGugan);
	DDX_Control(pDX, IDC_COMBO_UPPER_GUGAN, m_cbUpperGugan);
	DDX_Control(pDX, IDC_COMBO_SIDE_GUGAN, m_cbSideGugan);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Check(pDX, IDC_CHECK_POSITION, m_bPosition);
	DDX_CBIndex(pDX, IDC_COMBO_GW_GUGAN, m_nGWGugan);
	DDX_CBIndex(pDX, IDC_COMBO_UPPER_GUGAN, m_nUpperGugan);
	DDX_CBIndex(pDX, IDC_COMBO_SIDE_GUGAN, m_nSideGugan);
	DDX_Text(pDX, IDC_EDIT_FCK, m_dFck);
	DDX_Text(pDX, IDC_EDIT_FY, m_dFy);
	DDX_Text(pDX, IDC_STATIC_OTHER_FCK, m_sFck);
	DDX_Text(pDX, IDC_STATIC_OTHER_FY, m_sFy);
	DDX_Check(pDX, IDC_CHECK_GWHALF, m_bGWhalf);
	DDX_Text(pDX, IDC_EDIT_AGG, m_dAggregate);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInputRebarOthers, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputRebarOthers)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_CBN_SELCHANGE(IDC_COMBO_GW_GUGAN, OnSelchangeComboGWGugan)
	ON_CBN_SELCHANGE(IDC_COMBO_UPPER_GUGAN, OnSelchangeComboUpperGugan)
	ON_CBN_SELCHANGE(IDC_COMBO_SIDE_GUGAN, OnSelchangeComboSideGugan)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID, OnCellChangedRow)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_BN_CLICKED(IDC_CHECK_POSITION, OnCheckPosition)
	ON_EN_CHANGE(IDC_EDIT_FY, OnChangeEditFy)
	ON_EN_CHANGE(IDC_EDIT_FCK, OnChangeEditFck)
	ON_BN_CLICKED(IDC_CHECK_GWHALF, OnCheckGwhalf)
	ON_EN_KILLFOCUS(IDC_EDIT_FCK, OnKillfocusEditFck)
	ON_EN_KILLFOCUS(IDC_EDIT_FY, OnKillfocusEditFy)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
	ON_EN_KILLFOCUS(IDC_EDIT_AGG, OnKillfocusEditAgg)
	ON_EN_CHANGE(IDC_EDIT_AGG, OnChangeEditAgg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputRebarOthers message handlers

BOOL CInputRebarOthers::PreTranslateMessage(MSG* pMsg) 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		UpdateData();
		if(m_Grid.GetFocusCell().col == m_Grid.GetCols()-1)
			pMsg->wParam = VK_RIGHT;
		else
			pMsg->wParam = VK_TAB;
	}

	if(pMsg->hwnd==m_Grid.GetSafeHwnd() && (pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK))
	{	
		CCellID next = m_Grid.GetFocusCell();
		CString str = m_Grid.GetTextMatrix(next.row,0);
		long nStr = (long)GetNumberInString(str) - 1;
		long nHDan =  nStr > 0 ? nStr : 0;
		
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan);
				
		if(pGW)
		{
			// 방호벽,중분대,중분대1/2,난간기초 타입중 방음벽기초와 같은 경우
			if(pGW->IsTypeHDanRoadTotalGuardFenceAndCentralWall() || pGW->IsTypeHDanRoadHandRail())
			{
				if(pGW->m_dW1==0 && pGW->m_dW2==0 && pGW->m_dW4==0 && pGW->m_dW5==0)
					pGW->m_bSoundProof = TRUE;
			}
		}
		
		CString strTab = GetNameOfTab();
		if(strTab!=_T("교명주 받침"))
		{
			if(pGW->m_bSoundProof || strTab==_T("방음벽 기초") || strTab==_T("방호벽(방음벽기초타입)") || strTab==_T("연석"))
			{
				if(next.col==1 || next.col==2 || next.col==3)
				{	
					CStringArray strArr;
					strArr.Add(GetStringDia(10));
					strArr.Add(GetStringDia(13));
					strArr.Add(GetStringDia(16));
					strArr.Add(GetStringDia(19));
					strArr.Add(GetStringDia(22));
					strArr.Add(GetStringDia(25));
					strArr.Add(GetStringDia(29));
					strArr.Add(GetStringDia(32));
					strArr.Add(GetStringDia(35));
					m_Grid.SetComboString(strArr);
				}
				if(next.col==4)
				{	
					CString str;
					CStringArray strArr;
					long i=0; for(i=0; i<MAX_REBAR_CTC; i++)
					{
						CHgBaseConcStd hgBaseConcStd;
						str.Format(_T("CTC %.f"), hgBaseConcStd.GetValueRebarCTC(i));
						strArr.Add(str);
					}
					m_Grid.SetComboString(strArr);
				}
				if(next.col==5 || next.col==6)
				{	
					CString str;
					CStringArray strArr;
					long i=0; for(i=0; i<MAX_REBAR_GUGAN; i++)
					{
						if(next.col==5)
							str.Format(_T("%d @ %.0f"),i+1, pGW ? pGW->m_dCTC : 250);	// pGW==NULL ??
						else if(next.col==6)
						{
							str.Format(_T("배력근 %d개당"),i);
							if(i==MAX_REBAR_GUGAN-1)
								str.Format(_T("배력근 %d개당"),i+1);
						}
						strArr.Add(str);
					}
					m_Grid.SetComboString(strArr);
				}
			}
			else if(strTab==_T("흙막이벽"))
			{
				if(next.col==1 || next.col==2 || next.col==3)
				{	
					CStringArray strArr;
					strArr.Add(GetStringDia(10));
					strArr.Add(GetStringDia(13));
					strArr.Add(GetStringDia(16));
					strArr.Add(GetStringDia(19));
					strArr.Add(GetStringDia(22));
					strArr.Add(GetStringDia(25));
					strArr.Add(GetStringDia(29));
					strArr.Add(GetStringDia(32));
					strArr.Add(GetStringDia(35));
					m_Grid.SetComboString(strArr);
				}
				if(next.col==7 || next.col==8)
				{	
					CString str;
					CStringArray strArr;
					long i=0; for(i=0; i<MAX_REBAR_GUGAN; i++)
					{
						if(next.col==7)
							str.Format(_T("%d @ %.0f"),i+1, pGW->m_dCTC);
						else if(next.col==8)
						{
							str.Format(_T("배력근 %d개당"),i);
							if(i==MAX_REBAR_GUGAN-1)
								str.Format(_T("배력근 %d개당"),i+1);
						}
						
						strArr.Add(str);
					}
					m_Grid.SetComboString(strArr);
				}
				if(next.col==6)
				{	
					CString str;
					CStringArray strArr;
					long i=0; for(i=0; i<MAX_REBAR_CTC; i++)
					{
						CHgBaseConcStd hgBaseConcStd;
						str.Format(_T("CTC %.f"), hgBaseConcStd.GetValueRebarCTC(i));
						strArr.Add(str);
					}
					m_Grid.SetComboString(strArr);
				}
			}
			else if(strTab==_T("연석+보도") || strTab==_T("통신구") || strTab==_T("통신구+보도"))
			{
				if(next.col==1 || next.col==2 || next.col==3)
				{	
					CStringArray strArr;
					strArr.Add(GetStringDia(10));
					strArr.Add(GetStringDia(13));
					strArr.Add(GetStringDia(16));
					strArr.Add(GetStringDia(19));
					strArr.Add(GetStringDia(22));
					strArr.Add(GetStringDia(25));
					strArr.Add(GetStringDia(29));
					strArr.Add(GetStringDia(32));
					strArr.Add(GetStringDia(35));
					m_Grid.SetComboString(strArr);
				}
				if(next.col==4)
				{	
					CString str;
					CStringArray strArr;
					long i=0; for(i=0; i<MAX_REBAR_CTC; i++)
					{
						CHgBaseConcStd hgBaseConcStd;
						str.Format(_T("CTC %.f"), hgBaseConcStd.GetValueRebarCTC(i));
						strArr.Add(str);
					}
					m_Grid.SetComboString(strArr);
				}
				if(strTab==_T("연석+보도") && next.col==5)
				{	
					CString str;
					CStringArray strArr;
					long i=0; for(i=0; i<MAX_REBAR_GUGAN; i++)
					{
						str.Format(_T("%d @ %.0f"),i+1, pGW->m_dCTC);
						strArr.Add(str);
					}
					m_Grid.SetComboString(strArr);
				}

			}
			else
			{
				if(next.col==1 || next.col==2 || next.col==3)
				{
					if(m_bGWhalf && strTab==_T("중분대"))
					{
						if(next.col==1)
						{
							CStringArray strArr;
							strArr.Add(GetStringDia(10));
							strArr.Add(GetStringDia(13));
							strArr.Add(GetStringDia(16));
							strArr.Add(GetStringDia(19));
							strArr.Add(GetStringDia(22));
							strArr.Add(GetStringDia(25));
							strArr.Add(GetStringDia(29));
							strArr.Add(GetStringDia(32));
							strArr.Add(GetStringDia(35));
							m_Grid.SetComboString(strArr);
						}
						if(next.col==2)
						{
							CString str;
							CStringArray strArr;
							long i=0; for(i=0; i<MAX_REBAR_CTC; i++)
							{
								CHgBaseConcStd hgBaseConcStd;
								str.Format(_T("CTC %.f"), hgBaseConcStd.GetValueRebarCTC(i));
								strArr.Add(str);
							}
							m_Grid.SetComboString(strArr);
						}
					}
					else
					{
						CStringArray strArr;
						strArr.Add(GetStringDia(10));
						strArr.Add(GetStringDia(13));
						strArr.Add(GetStringDia(16));
						strArr.Add(GetStringDia(19));
						strArr.Add(GetStringDia(22));
						strArr.Add(GetStringDia(25));
						strArr.Add(GetStringDia(29));
						strArr.Add(GetStringDia(32));
						strArr.Add(GetStringDia(35));
						m_Grid.SetComboString(strArr);
					}
				}
				if(next.col==5)
				{	
					CString str;
					CStringArray strArr;
					long i=0; for(i=0; i<MAX_REBAR_CTC; i++)
					{
						CHgBaseConcStd hgBaseConcStd;
						str.Format(_T("CTC %.f"), hgBaseConcStd.GetValueRebarCTC(i));
						strArr.Add(str);
					}
					m_Grid.SetComboString(strArr);
				}
				if(next.col==6 || next.col==7)
				{	
					CString str;
					CStringArray strArr;
					long i=0; for(i=0; i<MAX_REBAR_GUGAN; i++)
					{
						if(next.col==6)
							str.Format(_T("%d @ %.0f"),i+1, pGW->m_dCTC);
						else if(next.col==7)
						{
							str.Format(_T("배력근 %d개당"),i);
							if(i==MAX_REBAR_GUGAN-1)
								str.Format(_T("배력근 %d개당"),i+1);
						}
						
						strArr.Add(str);
					}
					m_Grid.SetComboString(strArr);
				}
				if(m_Grid.GetTextMatrix(0, next.col) == "타입")
				{
					CStringArray strArr;
					strArr.Add("타입1");
					strArr.Add("타입2");
					m_Grid.SetComboString(strArr);
				}
			}
			
		}
		else
		{
			if(next.col==0 || next.col==1)
			{
				CStringArray strArr;
				strArr.Add(GetStringDia(10));
				strArr.Add(GetStringDia(13));
				strArr.Add(GetStringDia(16));
				strArr.Add(GetStringDia(19));
				strArr.Add(GetStringDia(22));
				strArr.Add(GetStringDia(25));
				strArr.Add(GetStringDia(29));
				strArr.Add(GetStringDia(32));
				strArr.Add(GetStringDia(35));
				m_Grid.SetComboString(strArr);
			}
		}
	}
	
	return CInputDialogBase::PreTranslateMessage(pMsg);
}

void CInputRebarOthers::SetControl()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	
	CString szKgf_cm2_CGS   = "kgf/cm²";
	CString szKgf_cm2_Apply = pStd->GetStringUnitType(szKgf_cm2_CGS);
	GetDlgItem(IDC_STATIC_OTHER_FCK)->SetWindowText(szKgf_cm2_Apply);
	GetDlgItem(IDC_STATIC_OTHER_FY)->SetWindowText(szKgf_cm2_Apply);
	UpdateData();

	CString strTab = GetNameOfTab();
	BOOL bNonRebarConc	= strTab == _T("무근 콘크리트");
	GetDlgItem(IDC_STATIC_SETTING)->ShowWindow(TRUE);
	//GetDlgItem(IDC_STATIC_FCK)->ShowWindow(!bNonRebarConc);	
	//GetDlgItem(IDC_EDIT_FCK)->ShowWindow(!bNonRebarConc);
	//GetDlgItem(IDC_STATIC_OTHER_FCK)->ShowWindow(!bNonRebarConc);
	GetDlgItem(IDC_STATIC_OTHER_FY)->ShowWindow(!bNonRebarConc);
	GetDlgItem(IDC_EDIT_FY)->ShowWindow(!bNonRebarConc);
	GetDlgItem(IDC_STATIC_FY)->ShowWindow(!bNonRebarConc);
// 	GetDlgItem(IDC_STATIC_AGG)->ShowWindow(strTab.Find("무근", 0) == -1); //strTab.Find("교명주", 0) == -1 && 
// 	GetDlgItem(IDC_EDIT_AGG)->ShowWindow( strTab.Find("무근", 0) == -1); //strTab.Find("교명주", 0) == -1 &&
}

void CInputRebarOthers::OnButtonApply()
{
	UpdateData();
	SetDataSave();
	SetDataInit();
	SetControl();

	CCellID cID = m_Grid.GetFocusCell();
	CString str = m_Grid.GetTextMatrix(cID.row, 0);
	if(str.Find(_T("중앙부"))>0)
		DrawInputDomyunView(TRUE, TRUE);
	else
		DrawInputDomyunView();
}

void CInputRebarOthers::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	CString str = GetNameOfTab();
	GetDlgItem(IDC_STATIC_SETTING)->SetWindowText(str);
		
	if(0)//str == _T("방음벽 기초"))
		SetDataDefaultGuganCombo();
	if(str == _T("중분대"))
		GetDlgItem(IDC_CHECK_GWHALF)->ShowWindow(TRUE);
	else
		GetDlgItem(IDC_CHECK_GWHALF)->ShowWindow(FALSE);

	SetGridTitle();
	SetDataInit();
	SetControl();
	SetFocusCell();

	CCellID cID = m_Grid.GetFocusCell();
	str = m_Grid.GetTextMatrix(cID.row, 0);

	SetDlgFckFyAg();

	if(str.Find(_T("중앙부"))>0)
		DrawInputDomyunView(TRUE, TRUE);
	else
		DrawInputDomyunView();

	return;
}

void CInputRebarOthers::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	UpdateData();
	SetDataSave();

	return;
}

void CInputRebarOthers::OnPreInitDialog()
{	
	SetResize(IDC_TAB, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID,	SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	
	SetResize(IDC_STATIC_VIEW, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_POSITION, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_SETTING, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_FCK, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_FY, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_AGG, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_FCK, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_FY, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_AGG, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_OTHER_FCK, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_OTHER_FY, SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_STATIC_GW_GUGAN, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_COMBO_GW_GUGAN, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_UPPER_GUGAN, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_COMBO_UPPER_GUGAN, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_SIDE_GUGAN, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_COMBO_SIDE_GUGAN, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_GWHALF, SZ_TOP_LEFT, SZ_TOP_LEFT);
	
	SetHyperHelp(m_pIn->GetManualPath());

	InitComboCtrl();
	InitTabCtrl();
	SetGridTitle();
	if(m_Grid.GetRows()>1)
		m_Grid.SetFocusCell(1,0);	//SetFckAndFy();
}

void CInputRebarOthers::InitComboCtrl()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	long nSize = pStd->GetBridgeSize();
	CString str = _T("");
	long i=0; for(i=0; i<nSize; i++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(i);
		str = pStd->GetStringBridgeOrder(i) + _T(" : ") + pBri->m_strBridgeName
			+ _T(" [") + pBri->GetStringBridgeType() + _T("]");
		m_cbBridgeNo.AddString(str);
	}
	m_cbBridgeNo.SetCurSel(m_pIn->m_nCurrentBridge);
}

void CInputRebarOthers::InitTabCtrl()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);

	GetDlgItem(IDC_CHECK_GWHALF)->ShowWindow(FALSE);

	// Tab Setting
	m_Tab.SendInitialUpdate(TRUE);
	// Clip children of the tab control from paint routines to reduce flicker.
	m_Tab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	m_Tab.DeleteAllItems();
	
	BOOL bGuardFence		= TRUE;
	BOOL bCenterGuard		= TRUE;
	BOOL bCenterGuardHalf	= TRUE;
	BOOL bSoundProof		= TRUE;
	BOOL bRetainingWall		= TRUE;
	BOOL bHandRail			= TRUE;
	BOOL bRailGuardFence	= TRUE;
	BOOL bFootwayBlock		= TRUE;
	BOOL bHole				= TRUE;
	BOOL bHoleFootway		= TRUE;
	BOOL bSoundProofType	= TRUE;
	BOOL bDetectGravel		= TRUE;
	BOOL bNonRear			= TRUE;

	CDPoint vAng = CDPoint(0, 1);
	CJijum* pJL = pBri->GetJijum(0);
	long nTabCnt = 0;
	long nQtyHDan = pBri->GetQtyHDan();

	m_nTabMonument		 = -1;
	m_nTabHoleFootway	 = -1;
	m_nTabHole			 = -1;
	m_nTabRailGuardFence = -1;
	m_nTabFootwayBlock	 = -1;
	m_nTabHandRail		 = -1;
	m_nTabRetainingWall	 = -1;
	m_nTabGuardHalf		 = -1;
	m_nTabSoundProofType = -1;
	m_nTabSoundProof	 = -1;
	m_nTabCenterGuard	 = -1;
	m_nTabGuardFence	 = -1;
	m_nTabDetectGravel   = -1;
	m_nTabNonRebar		 = -1;

	// 방음벽 타입은 탭이 하나만 나와야 함(왜냐하면 콤보로 선택되기 때문임)
	BOOL bFirstSoundProof	= TRUE;
	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;

		// 보차도 경계석, 보도는 방음벽 타입 아님.
		if(pGW->IsTypeHDanRoadFootwayBlock() || pGW->IsTypeHDanRoadFootway())
		{
			pGW->m_bSoundProof	= FALSE;
		}

		if(!pStd->IsRailDesign())
		{
			// 차도이면 무조건 아님
			if(pGW->IsTypeHDanOnlyNone())
				pGW->m_bSoundProof	= FALSE;

			// 방호벽,중분대,중분대1/2,난간기초중 방음벽 타입인지?
			if(pGW->IsTypeHDanRoadTotalGuardFenceAndCentralWall() || pGW->IsTypeHDanRoadHandRail())
			{
				if(pGW->m_dW1==0 && pGW->m_dW2==0 && pGW->m_dW4==0 && pGW->m_dW5==0)
				{
					pGW->m_bSoundProof = TRUE;
					if(bSoundProofType)
					{
						if(bFirstSoundProof)
						{
							m_Tab.InsertItem(nTabCnt, _T("방호벽(방음벽기초타입)"));
							m_nTabSoundProofType = nTabCnt++;
							bFirstSoundProof	= FALSE;
						}
						bSoundProofType = FALSE;
					}
				}
				else
					pGW->m_bSoundProof = FALSE;
			}
			
			if(pGW->IsTypeHDanRoadGuardFence() && !pGW->m_bSoundProof && bGuardFence)	// 같은 이름이 tab이 중복되어 생성되지 않도록
			{
				m_Tab.InsertItem(nTabCnt, _T("방호벽"));
				m_nTabGuardFence = nTabCnt++;
				bGuardFence = FALSE;		
			}
			else if(pGW->IsTypeHDanRoadCentralWallFull() && !pGW->m_bSoundProof  && bCenterGuard)
			{
				m_Tab.InsertItem(nTabCnt, _T("중분대"));
				m_nTabCenterGuard = nTabCnt++;
				bCenterGuard = FALSE;
				m_bGWhalf = pGW->m_bGWhalf;
			}
			else if(pGW->IsTypeHDanRoadCentralWallHalf() && !pGW->m_bSoundProof  && bCenterGuardHalf)
			{
				m_Tab.InsertItem(nTabCnt, _T("중분대1/2"));
				m_nTabGuardHalf	= nTabCnt++;
				pGW->m_bSoundProof ? bSoundProofType = FALSE : 
				bCenterGuardHalf = FALSE;
			}
			else if(pGW->IsTypeHDanRoadSoundProof() && bSoundProof)
			{
				if(bFirstSoundProof)
				{
					m_Tab.InsertItem(nTabCnt, _T("방음벽 기초"));
					m_nTabSoundProof = nTabCnt++;
					bFirstSoundProof	= FALSE;
				}
				bSoundProof = FALSE;
			}
			else if(pGW->IsTypeHDanRoadRetainingWall() && bRetainingWall)
			{
				m_Tab.InsertItem(nTabCnt, _T("흙막이벽"));
				m_nTabRetainingWall	= nTabCnt++;
				bRetainingWall = FALSE;
			}
			else if(pGW->IsTypeHDanRoadHandRail() && !pGW->m_bSoundProof && bHandRail)
			{
				m_Tab.InsertItem(nTabCnt, _T("난간기초"));
				m_nTabHandRail = nTabCnt++;
				bHandRail = FALSE;
			}
			else if(pGW->IsTypeHDanNonRebarConcrete() && bNonRear)
			{
				m_Tab.InsertItem(nTabCnt, _T("무근 콘크리트"));
				m_nTabNonRebar	= nTabCnt++;
				bNonRear	= FALSE;

			}
		}
		else
		{
			if(pGW->IsTypeHDanRailGuardFence() && bRailGuardFence)
			{
				m_Tab.InsertItem(nTabCnt, _T("연석"));
				m_nTabRailGuardFence = nTabCnt++;
				bRailGuardFence = FALSE;
			}
			else if(pGW->IsTypeHDanRailFootwayBlock() && bFootwayBlock)
			{
				m_Tab.InsertItem(nTabCnt, _T("연석+보도"));
				m_nTabFootwayBlock = nTabCnt++;
				bFootwayBlock = FALSE;
			}
			else if(pGW->IsTypeHDanRailHole() && bHole)
			{
				m_Tab.InsertItem(nTabCnt, _T("통신구"));
				m_nTabHole = nTabCnt++;
				bHole = FALSE;
			}
			else if(pGW->IsTypeHDanRailHoleFootway() && bHoleFootway)
			{
				m_Tab.InsertItem(nTabCnt, _T("통신구+보도"));		
				m_nTabHoleFootway = nTabCnt++;
				bHoleFootway = FALSE;
			}
			else if(pGW->IsTypeHDanRailDetectGravel() && bDetectGravel)
			{
				m_Tab.InsertItem(nTabCnt, _T("자갈막이(철근)"));		
				m_nTabDetectGravel = nTabCnt++;
				bDetectGravel = FALSE;
			}
			else if(pGW->IsTypeHDanNonRebarConcrete() && bNonRear)
			{
				m_Tab.InsertItem(nTabCnt, _T("무근 콘크리트"));
				m_nTabNonRebar	= nTabCnt++;
				bNonRear	= FALSE;
			}
		}

		// 새로 추가된 경우 권고안 실행(방호벽, 중분대, 중분대 1/2, 난간기초, 연석, 연석+보도, 통신구, 통신구+보도)
		// 새로 추가된 경우를 정확히 판단하기란 힘드므로 아예 권고안을 하지 않음 modified by jaeho 2006.11.20
		/*
		if( (   pGW->IsTypeHDanRoadTotalGuardFenceAndCentralWall()
			 || pGW->IsTypeHDanRoadHandRail()
			 || pGW->IsTypeHDanRailGuardFence()
			 || pGW->IsTypeHDanRailFootwayBlock()
			 || pGW->IsTypeHDanRailDetectGravel()
			 || pGW->IsTypeHDanRailTotalHole())	&& pGW->m_dSC<1 )
			 
			pBri->m_Util.SetDataDefaultRebarGuardWall(pGW, sta, vAng);

		if(pGW->m_bSoundProof && (pGW->m_nSGugan==0 || pGW->m_nUGugan==0))
			pBri->m_Util.SetDataDefaultRebarGuardWall(pGW, sta, vAng);
			*/
	}

	if(pBri->m_nMonumenu>0)
	{
		m_Tab.InsertItem(nTabCnt, _T("교명주 받침"));
		m_nTabMonument	= nTabCnt++;
	}
}

void CInputRebarOthers::OnSelchangeComboBridgeNo() 
{
	m_pIn->m_nCurrentBridge = m_cbBridgeNo.GetCurSel();
	
	InitTabCtrl();

	CString str = GetNameOfTab();
	GetDlgItem(IDC_STATIC_SETTING)->SetWindowText(str);
		
	if(str ==_T("방음벽 기초") || str==_T("방호벽(방음벽기초타입)"))
	{
		SetDataDefaultGuganCombo();
	}

	SetGridTitle();
	SetDataInit();
	SetControl();
	SetPaneTitle();

	CCellID cID = m_Grid.GetFocusCell();
	str = m_Grid.GetTextMatrix(cID.row, 0);
	if(str.Find(_T("중앙부"))>0)
		DrawInputDomyunView(TRUE, TRUE);
	else
		DrawInputDomyunView();
}

void CInputRebarOthers::DrawInputDomyunView(BOOL bZoomAll, BOOL bCover)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);
		
	long nTab = m_Tab.GetCurSel();
	CDomyun* pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(nTab==2 ? 20 : 15);

	long nHDan = GetHDan();
	CString strTab = GetNameOfTab();
	CDRect rtMain(0,0,0,0);
	if(strTab==_T("방호벽") || strTab==_T("자갈막이(철근)"))
		rtMain = DrawGuardFence(pDom, nHDan);
	else if(strTab==_T("중분대"))
		rtMain = DrawCenterGuard(pDom, nHDan);
	else if(strTab==_T("중분대1/2"))
		rtMain = DrawCenterGuardHalf(pDom, nHDan);
	else if(strTab==_T("방음벽 기초") || strTab==_T("방호벽(방음벽기초타입)"))
		rtMain = DrawSoundProof(pDom, nHDan);
	else if(strTab==_T("흙막이벽"))
		rtMain = DrawRetainingWall(pDom, nHDan);
	else if(strTab==_T("난간기초"))
		rtMain = DrawHandRail(pDom, nHDan);
	else if(strTab==_T("연석"))
		rtMain = DrawRailGuardFence(pDom, nHDan, TRUE);
	else if(strTab==_T("연석+보도"))
		rtMain = DrawFootwayBlock(pDom, nHDan, TRUE, bCover);
	else if(strTab==_T("통신구") || strTab==_T("통신구+보도"))
		rtMain = DrawTotalHole(pDom, nHDan, TRUE, bCover);
	else if(strTab==_T("교명주 받침"))
		DrawMonument(pDom);
	else if(strTab==_T("무근 콘크리트"))
		rtMain = DrawCrossGuardWall(pDom, nHDan);



		

	DrawCrossPosition(pDom, nHDan, rtMain);

	CInputDialogBase::DrawInputDomyunView();
}

CDRect CInputRebarOthers::DrawGuardFence(CDomyun* pDom, long nHDan, BOOL bInput)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);

	CDomyun Dom(pDom);
	DBStd.m_pDrawCrossSection->DrawRebarGuardFence(&Dom, nHDan, FALSE, FALSE, bInput);

	Dom.SetCalcExtRect();
	CDRect rtMain = Dom.GetExtRect();
	
	*pDom << Dom;

	return rtMain;
}

CDRect CInputRebarOthers::DrawCenterGuard(CDomyun* pDom, long nHDan, BOOL bInput)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);

	CDomyun Dom(pDom);
	DBStd.m_pDrawCrossSection->DrawRebarCenterGuard(&Dom, nHDan, FALSE, FALSE, bInput);

	Dom.SetCalcExtRect();
	CDRect rtMain = Dom.GetExtRect();
	
	*pDom << Dom;

	return rtMain;
}

CDRect CInputRebarOthers::DrawCenterGuardHalf(CDomyun* pDom, long nHDan, BOOL bInput)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);

	CDomyun Dom(pDom);
	DBStd.m_pDrawCrossSection->DrawRebarCenterGuardHalf(&Dom, nHDan, FALSE, FALSE, bInput);

	Dom.SetCalcExtRect();
	CDRect rtMain = Dom.GetExtRect();
	
	*pDom << Dom;

	return rtMain;
}

CDRect CInputRebarOthers::DrawSoundProof(CDomyun* pDom, long nHDan, BOOL bInput)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);
	
	CDomyun Dom(pDom);
	DBStd.m_pDrawCrossSection->DrawRebarSoundProof(&Dom, nHDan, FALSE, FALSE, bInput);

	Dom.SetCalcExtRect();
	CDRect rtMain = Dom.GetExtRect();
	
	*pDom << Dom;

	return rtMain;
}

CDRect CInputRebarOthers::DrawRetainingWall(CDomyun* pDom, long nHDan, BOOL bInput)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);
	
	CDomyun Dom(pDom);
	DBStd.m_pDrawCrossSection->DrawRebarRetainingWall(&Dom, nHDan, FALSE, FALSE, bInput);

	Dom.SetCalcExtRect();
	CDRect rtMain = Dom.GetExtRect();
	
	*pDom << Dom;

	return rtMain;
}

CDRect CInputRebarOthers::DrawHandRail(CDomyun* pDom, long nHDan, BOOL bInput)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);

	CDomyun Dom(pDom);
	DBStd.m_pDrawCrossSection->DrawRebarHandRail(&Dom, nHDan, FALSE, FALSE, bInput);

	Dom.SetCalcExtRect();
	CDRect rtMain = Dom.GetExtRect();
	
	*pDom << Dom;

	return rtMain;
}

CDRect CInputRebarOthers::DrawRailGuardFence(CDomyun* pDom, long nHDan, BOOL bInput)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);
	
	CDomyun Dom(pDom);
	DBStd.m_pDrawCrossSection->DrawRebarRailGuardFence(&Dom, nHDan, FALSE, FALSE, TRUE);

	Dom.SetCalcExtRect();
	CDRect rtMain = Dom.GetExtRect();
	
	*pDom << Dom;

	return rtMain;
}

CDRect CInputRebarOthers::DrawFootwayBlock(CDomyun* pDom, long nHDan, BOOL bInput, BOOL bCover)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);
	
	CDomyun Dom(pDom);
	DBStd.m_pDrawCrossSection->DrawRebarFootwayBlock(&Dom, nHDan, FALSE, FALSE, TRUE, bCover);

	Dom.SetCalcExtRect();
	CDRect rtMain = Dom.GetExtRect();
	
	*pDom << Dom;

	return rtMain;
}

CDRect CInputRebarOthers::DrawTotalHole(CDomyun* pDom, long nHDan, BOOL bInput, BOOL bCover)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);
	
	CDomyun Dom(pDom);
	DBStd.m_pDrawCrossSection->DrawRebarTotalHole(&Dom, nHDan, FALSE, FALSE, TRUE, bCover);

	Dom.SetCalcExtRect();
	CDRect rtMain = Dom.GetExtRect();
	
	*pDom << Dom;

	return rtMain;
}

void CInputRebarOthers::DrawMonument(CDomyun* pDom)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);
	
	CDomyun DomFront(pDom);
	CDomyun DomPlane(pDom);

	DBStd.m_pDrawCrossSection->DrawMonumentFront(&DomFront);
	DBStd.m_pDrawCrossSection->DrawRebarMonumentFront(&DomFront);
	DBStd.m_pDrawCrossSection->DimRebarMonumentFront(&DomFront);
	DBStd.m_pDrawCrossSection->MarkRebarMonuFront(&DomFront);

	CDRect rcFront = DomFront.GetExtRect();

	m_pIn->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_SMALL_TITLE);
	DomFront.SetTextAlignHorz(TA_CENTER);
	DomFront.SetTextHeight(5);
	DomFront.TextOut(rcFront.CenterPoint().x, rcFront.bottom+DomFront.Always(20), _T("정 면 도"));

	*pDom << DomFront;

	DBStd.m_pDrawCrossSection->DrawMonumentPlane(&DomPlane);
	DBStd.m_pDrawCrossSection->DrawRebarMonumentPlane(&DomPlane);
	DBStd.m_pDrawCrossSection->DimRebarMonumentPlane(&DomPlane);
	DBStd.m_pDrawCrossSection->MarkRebarMonuPlane(&DomPlane);

	CDRect rcPlane = DomPlane.GetExtRect();

	m_pIn->m_pARoadOptionStd->SetEnvType(&DomPlane, HCAD_SMALL_TITLE);
	DomPlane.SetTextAlignHorz(TA_CENTER);
	DomPlane.SetTextHeight(5);
	DomPlane.TextOut(rcPlane.CenterPoint().x, rcPlane.bottom+DomPlane.Always(20), _T("평 면 도"));

	DomPlane.Move(rcFront.Width()+DomPlane.Always(20), rcFront.bottom-rcPlane.bottom);

	*pDom << DomPlane;
}

void CInputRebarOthers::DrawCrossPosition(CDomyun* pDom, long nHDan, CDRect rtMain)
{
	if(!m_bPosition) return;

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);

	CDPoint vAng = CDPoint(0,1);
	double sta = pBri->GetStationOnJijum(0,0);
	CJijum* pJL = pBri->GetJijum(0);
	if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pJL->m_dLengthUgan[1];

	CDomyun DomLoc(pDom);
	DomLoc.SetScaleDim(100);

	m_pIn->m_pARoadOptionStd->SetEnvType(&DomLoc, HCAD_CONC);
	DBStd.m_pDrawCrossSection->DrawCrossSection(&DomLoc, sta);
	
	// 구간표시
	CDPoint A[30];
	pBri->GetXySlabUpperAct(sta, vAng, A);
	
	CString sGuGan = _T("");
	long nQtyHDan = pBri->GetQtyHDan();
	double dDis=0, dPrev=0;

	long n = 0; for(n = 0; n<nQtyHDan; n++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(n);
		if(!pGW) continue;
		
		sGuGan.Format("%d 구간", n+1);
		dDis = pBri->GetLengthHDanAct(sta, vAng, n);
		if(n==nHDan)
		{
			double dH = pGW->m_dH1+pGW->m_dH2+pGW->m_dH3;
			
			m_pIn->m_pARoadOptionStd->SetEnvType(&DomLoc, HCAD_BARX);
			
			DomLoc.SetTextHeight(5);
			DomLoc.TextOut( (A[0].x+dPrev)+(0.5*dDis), A[n].y+dH+DomLoc.Always(10), sGuGan);
		}
		dPrev += dDis;
	}
	//
	
	DomLoc.SetCalcExtRect();
	CDRect rtCross = DomLoc.GetExtRect();
	double dRate = rtMain.Width()/rtCross.Width() * 2;
	DomLoc.RedrawByScale(dRate);

	DomLoc.SetCalcExtRect();
	rtCross = DomLoc.GetExtRect();
	
	DomLoc.Move(	rtMain.left - rtCross.right - DomLoc.Always(10), 
					(rtMain.bottom + rtMain.top)/2 - (rtCross.bottom + rtCross.top)/2	);

	*pDom << DomLoc;
}

void CInputRebarOthers::SetDataDefault()
{	
	for(long bri=0; bri<m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(bri);
		if(!pBri) return;

		BOOL bFirst = IsFirst(bri);
		pBri->SetDefaultedState(GetIDD(), 0x00000001);
		if(!bFirst && !IsClickDefaultButton(bri)) continue;
	
		CDPoint vAng = CDPoint(0, 1);
		double sta = pBri->GetStationOnJijum(0,0);
		CJijum* pJL = pBri->GetJijum(0);
		if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
			sta += pJL->m_dLengthUgan[1];

		CString str = GetNameOfTab();
		long nQtyHDan = pBri->GetQtyHDan();
		
		if(bFirst)
		{
			long i=0; for(i=0; i<nQtyHDan; i++)
			{
				CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i);
				if(!pGW) continue;
				pBri->m_Util.SetDataDefaultRebarGuardWall(pGW, sta, vAng);
				if((str==_T("방음벽 기초") && pGW->IsTypeHDanRoadSoundProof())
					|| (str==_T("방호벽(방음벽기초타입)") && (pGW->IsTypeHDanRoadTotalGuardFenceAndCentralWall() || pGW->IsTypeHDanRoadHandRail())))
				{
					SetDataDefaultGuganCombo();
					SetGridTitleSoundProof();
				}
			}	
		}
		else
		{	
			long i=0; for(i=0; i<nQtyHDan; i++)
			{
				CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i);
				if(!pGW) continue;
				
				if(		( str == _T("방호벽")		&&	pGW->IsTypeHDanRoadGuardFence()		 )
					||	( str == _T("중분대1/2")	&&	pGW->IsTypeHDanRoadCentralWallHalf() )
					||	( str == _T("중분대")		&&	pGW->IsTypeHDanRoadCentralWallFull() )
					||	( str == _T("난간기초")		&&	pGW->IsTypeHDanRoadHandRail()		 )
					||	( str == _T("방음벽 기초")	&&	pGW->IsTypeHDanRoadSoundProof()		 )	
					||	( str == _T("흙막이벽")	    &&	pGW->IsTypeHDanRoadRetainingWall()	 )
					||	( str == _T("연석")		    &&	pGW->IsTypeHDanRailGuardFence()		 )
					||	( str == _T("연석+보도")    &&	pGW->IsTypeHDanRailFootwayBlock()	 )
					||	( str == _T("통신구")		&&	pGW->IsTypeHDanRailHole()			 )
					||	( str == _T("통신구+보도")  &&	pGW->IsTypeHDanRailHoleFootway()	 )
					||	( str == _T("자갈막이(철근)")		&&	pGW->IsTypeHDanRailDetectGravel()	 )
					||  ( str==_T("방호벽(방음벽기초타입)") && (pGW->IsTypeHDanRoadTotalGuardFenceAndCentralWall() || pGW->IsTypeHDanRoadHandRail()))
					|| (str == _T("무근 콘크리트") && pGW->IsTypeHDanNonRebarConcrete()))
				
					pBri->m_Util.SetDataDefaultRebarGuardWall(pGW, sta, vAng);

				if((str==_T("방음벽 기초") && pGW->IsTypeHDanRoadSoundProof())
					|| (str==_T("방호벽(방음벽기초타입)") && (pGW->IsTypeHDanRoadTotalGuardFenceAndCentralWall() || pGW->IsTypeHDanRoadHandRail())))
				{
					SetDataDefaultGuganCombo();
					SetGridTitleSoundProof();
				}			
			}	
		}

		// 교명주 받침대
		if(bFirst || str == _T("교명주 받침"))
		{

			SetDataDefaultMonument();	
			SetDataInit();
		}
	}

	SetDlgFckFyAg();
	DrawInputDomyunView();
}

void CInputRebarOthers::SetDataDefaultMonument()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	for(long bri = 0; bri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(bri);
		if(!pBri) return;
		
		double dH1 = pBri->m_dMonuH1;
		double dD  = pBri->m_dMonuD;
		double dW  = pBri->m_dMonuW;
		double dL  = pBri->m_dMonuL;

		pBri->m_dMonuFck  = pStd->m_Fck;
		pBri->m_dMonuR_Fy =  pStd->m_Fy;
		pBri->m_dMonuR_DiaHor = 16;
		pBri->m_dMonuR_DiaVer = 16;
		pBri->m_dMonuR_A1 = 50.0;
		pBri->m_dMonuR_C1 = 50.0;
		pBri->m_dMonuR_C2 = 0.0;
		pBri->m_dMonuR_D1 = 50.0;
		pBri->m_dMonuR_D2 = 0.0;
		pBri->m_dMonuR_H  = dH1 - pBri->m_dMonuR_A1;
		pBri->m_dMonuR_L  = 700.0;
		pBri->m_dMonuR_A  = 100.0;
		pBri->m_dMonuR_B  = 100.0;
		pBri->m_dMonuR_C  = 100.0;
		pBri->m_dMonuR_D  = 100.0;
		pBri->m_dMonuAggregate = 19;
		pBri->m_nMonuR_nC = (long)((dW-pBri->m_dMonuR_C1*2)/pBri->m_dMonuR_C);
		pBri->m_nMonuR_nD = (long)((dL-pBri->m_dMonuR_D1*2)/pBri->m_dMonuR_D);
		pBri->m_dMonuR_C3 = dW - pBri->m_dMonuR_C1*2.0 - pBri->m_dMonuR_C2 - pBri->m_nMonuR_nC*pBri->m_dMonuR_C;
		// str에 셋팅하지 않으면 C3이 이상하게 계산된다. ㅡㅡ;; (release에서) : trace도 안됨..
		// 그냥 str 셋팅만 함
		CString str	= _T("");
		str.Format(_T("%f"), pBri->m_dMonuR_C3);
		
		pBri->m_dMonuR_D3 = dL - pBri->m_dMonuR_D1*2 - pBri->m_dMonuR_D2 - pBri->m_nMonuR_nD*pBri->m_dMonuR_D;
		pBri->m_nMonuR_nB = (long)((dD-pBri->m_dMonuR_C1)/pBri->m_dMonuR_B);
		if(pBri->m_nMonuR_nB*pBri->m_dMonuR_B==dD-pBri->m_dMonuR_C1)
			pBri->m_nMonuR_nB--;
		

		CDPoint xyL[3];
		pBri->GetXyRebarMonuFront(xyL, TRUE);
		pBri->m_nMonuR_nA = (long)((xyL[0].y - xyL[1].y)/pBri->m_dMonuR_A);
	}
}

void CInputRebarOthers::SetGridTitle()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	
	SetGuganCombo(FALSE);

	m_Grid.SetGridDefault(1,1,1);
	m_Grid.InitGrid(pBri->GetValueFy(ePartUpperSlab));
	CString strTab = GetNameOfTab();

	if(strTab==_T("방호벽") || strTab==_T("중분대") || strTab==_T("중분대1/2") || strTab==_T("난간기초") || strTab==_T("자갈막이(철근)"))	
	{
		if(m_bGWhalf && strTab==_T("중분대"))
		{
			GetDlgItem(IDC_CHECK_GWHALF)->ShowWindow(TRUE);
			SetGridTitleCentralWall();	// 중분대 일체형
		}
		else
			SetGridTitleGuardFenceAndCentralWall(strTab);
	}
	else if(strTab==_T("방음벽 기초") || strTab == _T("방호벽(방음벽기초타입)"))
		SetGridTitleSoundProof();
	else if(strTab==_T("흙막이벽"))
		SetGridTitleRetainingWall();
	else if(strTab==_T("연석"))
		SetGridTitleRailGuardFence();
	else if(strTab==_T("연석+보도"))
		SetGridTitleFootwayBlock();
	else if(strTab==_T("통신구"))
		SetGridTitleHole();
	else if(strTab==_T("통신구+보도"))
		SetGridTitleHoleFootway();
	else if(strTab==_T("교명주 받침"))
		SetGridTitleMonument();
	else if(strTab==_T("무근 콘크리트"))
		SetGridTitleNonRebarConcrete();
}
void CInputRebarOthers::SetGridTitleCentralWall()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	
	if(pBri->GetQtyHDanTotalGuardFence()==0) return;

	long nRows = 1;
	long nCols = 5;

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowResize(FALSE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(50);

	long nCol=0;
	m_Grid.SetTextMatrix(0,	nCol++, _T("구간"));
	m_Grid.SetTextMatrix(0, nCol++, _T("VR"));
	m_Grid.SetColumnWidth(nCol, 80);
	m_Grid.SetTextMatrix(0, nCol++, _T("주철근 간격"));
	m_Grid.SetTextMatrix(0, nCol++, _T("F1"));
	m_Grid.SetTextMatrix(0, nCol++, _T("F2"));

	long nQtyHDan = pBri->GetQtyHDan();
	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;

		if(pGW->IsTypeHDanRoadCentralWallFull())
		{
			m_Grid.SetRowCount(nRows+1);
			m_Grid.SetTextMatrix(nRows, 0, _T("%.0f 구간"), (double)i+1);

			long nCol = 1;
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);		
			nRows++;
		}
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputRebarOthers::SetGridTitleGuardFenceAndCentralWall(CString strTab)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	
	if(pBri->GetQtyHDanTotalGuardFence()==0) return;

	long nRows = 1;
	long nCols = 16;
	if(strTab == _T("자갈막이(철근)"))	// 자갈막이는 F2, F3 없다.
		nCols = 14;

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowResize(FALSE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(nCols-1, 70);

	long nCol=0;
	m_Grid.SetTextMatrix(0,	nCol++, _T("구간"));
	m_Grid.SetTextMatrix(0,	nCol++, _T("HR"));
	m_Grid.SetTextMatrix(0, nCol++, _T("VR"));
	m_Grid.SetTextMatrix(0, nCol++, _T("SR"));
	m_Grid.SetTextMatrix(0, nCol++, _T("EA"));
	m_Grid.SetColumnWidth(nCol, 80);
	m_Grid.SetTextMatrix(0, nCol++, _T("주철근 간격"));
	m_Grid.SetColumnWidth(nCol, 110);
	m_Grid.SetTextMatrix(0, nCol++, _T("전단철근 묶음수"));
	m_Grid.SetColumnWidth(nCol, 100);
	m_Grid.SetTextMatrix(0, nCol++, _T("전단철근 구간"));
	m_Grid.SetTextMatrix(0, nCol++, _T("DC1"));
	m_Grid.SetTextMatrix(0, nCol++, _T("DC2"));
	m_Grid.SetTextMatrix(0, nCol++, _T("DC3"));
	m_Grid.SetTextMatrix(0, nCol++, _T("L1"));
	m_Grid.SetTextMatrix(0, nCol++, _T("F1"));
	if(strTab != _T("자갈막이(철근)"))	// 자갈막이는 F2, F3 없다.
	{
		m_Grid.SetTextMatrix(0, nCol++, _T("F2"));
		m_Grid.SetTextMatrix(0, nCol++, _T("F3"));
	}

	// 배근타입
	m_Grid.SetTextMatrix(0, nCol++, "타입");

	BOOL bType = FALSE;
	long nQtyHDan = pBri->GetQtyHDan();
	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;

		if(strTab==_T("방호벽"))				bType = pGW->IsTypeHDanRoadGuardFence();
		else if(strTab==_T("중분대"))			bType = pGW->IsTypeHDanRoadCentralWallFull();
		else if(strTab==_T("중분대1/2"))		bType = pGW->IsTypeHDanRoadCentralWallHalf();
		else if(strTab==_T("난간기초"))			bType = pGW->IsTypeHDanRoadHandRail();
		else if(strTab==_T("자갈막이(철근)"))	bType = pGW->IsTypeHDanRailDetectGravel();

		if(bType && !pGW->m_bSoundProof)
		{
			m_Grid.SetRowCount(nRows+1);
			m_Grid.SetTextMatrix(nRows, 0, _T("%.0f 구간"), (double)i+1);

			long nCol = 1;
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			nCol++;
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);		

			nCol+=5;
			if(strTab != _T("자갈막이(철근)"))	// 자갈막이는 F2, F3 없다.
			{
				nCol+=2;
			}
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);		

			nRows++;
		}
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputRebarOthers::SetGridTitleSoundProof()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	
	if(pBri->GetQtyHDanTotalGuardFence()==0) return;
	
	SetGuganCombo(TRUE);
	m_Grid.DeleteAllItems();
	
	long nRows = 3;
	long nCols = 12 + m_nUpperGugan + 1 + m_nSideGugan + 1;

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowResize(FALSE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(50);

	long nCol = 1;
	m_Grid.SetTextMatrix(0, 0, _T("구분"));
	m_Grid.SetTextMatrix(1, 0, _T("개수"));
	m_Grid.SetTextMatrix(2, 0, _T("간격"));
	m_Grid.SetTextMatrix(0, nCol++, _T("HR"));
	m_Grid.SetTextMatrix(0, nCol++, _T("VR"));
	m_Grid.SetTextMatrix(0, nCol++, _T("SR"));
	m_Grid.SetColumnWidth(nCol, 80);
	m_Grid.SetTextMatrix(0, nCol++, _T("주철근 간격"));
	m_Grid.SetColumnWidth(nCol, 110);
	m_Grid.SetTextMatrix(0, nCol++, _T("전단철근 묶음수"));
	m_Grid.SetColumnWidth(nCol, 100);
	m_Grid.SetTextMatrix(0, nCol++, _T("전단철근 구간"));	

	long i=0; for(i=0; i<m_nUpperGugan+1; i++)
		m_Grid.SetTextMatrix(0, nCol++, _T("U%.0f"), (double)i+1);
	for(i=0; i<m_nSideGugan+1; i++)
		m_Grid.SetTextMatrix(0, nCol++, _T("S%.0f"), (double)i+1);
	
	m_Grid.SetTextMatrix(0, nCol++, _T("F1"));
	m_Grid.SetTextMatrix(0, nCol++, _T("L1"));
	m_Grid.SetTextMatrix(0, nCol++, _T("DC1"));
	m_Grid.SetTextMatrix(0, nCol++, _T("DC2"));
	m_Grid.SetTextMatrix(0, nCol++, _T("DC3"));

	m_Grid.SetCellType(1, 1, CT_COMBO);
	m_Grid.SetCellType(1, 2, CT_COMBO);
	m_Grid.SetCellType(1, 3, CT_COMBO);
	m_Grid.SetCellType(1, 4, CT_COMBO);
	m_Grid.SetCellType(1, 5, CT_COMBO);
	m_Grid.SetCellType(1, 6, CT_COMBO);

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputRebarOthers::SetGridTitleRetainingWall()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	
	if(pBri->GetQtyHDanTotalGuardFence()==0) return;

	long nRows = 1;
	long nCols = 13;

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowResize(FALSE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(50);

	long nCol=0;
	m_Grid.SetTextMatrix(0,	nCol++, _T("구간"));
	m_Grid.SetTextMatrix(0,	nCol++, _T("HR"));
	m_Grid.SetTextMatrix(0, nCol++, _T("VR"));
	m_Grid.SetTextMatrix(0, nCol++, _T("SR"));
	m_Grid.SetTextMatrix(0, nCol++, _T("VEA"));
	m_Grid.SetTextMatrix(0, nCol++, _T("HEA"));
	m_Grid.SetColumnWidth(nCol, 80);
	m_Grid.SetTextMatrix(0, nCol++, _T("주철근 간격"));
	m_Grid.SetColumnWidth(nCol, 110);
	m_Grid.SetTextMatrix(0, nCol++, _T("전단철근 묶음수"));
	m_Grid.SetColumnWidth(nCol, 100);
	m_Grid.SetTextMatrix(0, nCol++, _T("전단철근 구간"));
	m_Grid.SetTextMatrix(0, nCol++, _T("DC1"));
	m_Grid.SetTextMatrix(0, nCol++, _T("DC2"));
	m_Grid.SetTextMatrix(0, nCol++, _T("DC3"));
	m_Grid.SetTextMatrix(0, nCol++, _T("L1"));

	long i=0; for(i=0; i<pBri->GetQtyHDan(); i++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadRetainingWall())
		{
			m_Grid.SetRowCount(nRows+1);
			m_Grid.SetTextMatrix(nRows, 0, _T("%.0f 구간"), (double)i+1);

			long nCol = 1;
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			nCol++;			nCol++;
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			nRows++;
		}
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputRebarOthers::SetGridTitleRailGuardFence()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	
	if(pBri->GetQtyHDanTotalGuardFence()==0) return;
	
	long nRows = 1;
	long nCols = 14;

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowResize(FALSE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(50);

	long nCol=0;
	m_Grid.SetTextMatrix(0,	nCol++, _T("구간"));
	m_Grid.SetTextMatrix(0,	nCol++, _T("HR"));
	m_Grid.SetTextMatrix(0, nCol++, _T("VR"));
	m_Grid.SetTextMatrix(0, nCol++, _T("SR"));
	m_Grid.SetColumnWidth(nCol, 80);
	m_Grid.SetTextMatrix(0, nCol++, _T("주철근 간격"));
	m_Grid.SetColumnWidth(nCol, 110);
	m_Grid.SetTextMatrix(0, nCol++, _T("전단철근 묶음수"));
	m_Grid.SetColumnWidth(nCol, 100);
	m_Grid.SetTextMatrix(0, nCol++, _T("전단철근 구간"));
	m_Grid.SetTextMatrix(0, nCol++, _T("EA"));
	m_Grid.SetTextMatrix(0, nCol++, _T("S"));
	m_Grid.SetTextMatrix(0, nCol++, _T("DC1"));
	m_Grid.SetTextMatrix(0, nCol++, _T("DC2"));
	m_Grid.SetTextMatrix(0, nCol++, _T("DC3"));
	m_Grid.SetTextMatrix(0, nCol++, _T("L1"));
	m_Grid.SetTextMatrix(0, nCol++, _T("F1"));

	long i=0; for(i=0; i<pBri->GetQtyHDan(); i++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailGuardFence())
		{
			m_Grid.SetRowCount(nRows+1);
			m_Grid.SetTextMatrix(nRows, 0, _T("%.0f 구간"), (double)i+1);

			long nCol = 1;
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);		
			nRows++;
		}
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputRebarOthers::SetGridTitleFootwayBlock()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	
	if(pBri->GetQtyHDanTotalGuardFence()==0) return;

	long nRows = 1;
	long nCols = 14;

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowResize(FALSE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(50);

	long nCol=0;
	m_Grid.SetColumnWidth(nCol, 100);
	m_Grid.SetTextMatrix(0,	nCol++, _T("구 간"));
	m_Grid.SetTextMatrix(0,	nCol++, _T("HR"));
	m_Grid.SetTextMatrix(0, nCol++, _T("VR"));
	m_Grid.SetTextMatrix(0, nCol++, _T("SR"));
	m_Grid.SetColumnWidth(nCol, 80);
	m_Grid.SetTextMatrix(0, nCol++, _T("주철근 간격"));
	m_Grid.SetColumnWidth(nCol, 110);
	m_Grid.SetTextMatrix(0, nCol++, _T("전단철근 묶음수"));
	m_Grid.SetTextMatrix(0, nCol++, _T("E1"));
	m_Grid.SetTextMatrix(0, nCol++, _T("E2"));
	m_Grid.SetTextMatrix(0, nCol++, _T("E3"));
	m_Grid.SetTextMatrix(0, nCol++, _T("E4"));
	m_Grid.SetTextMatrix(0, nCol++, _T("DC1"));
	m_Grid.SetTextMatrix(0, nCol++, _T("DC2"));
	m_Grid.SetTextMatrix(0, nCol++, _T("DC3"));
	m_Grid.SetTextMatrix(0, nCol++, _T("L1"));

	long i=0; for(i=0; i<pBri->GetQtyHDan(); i++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailFootwayBlock())
		{
			m_Grid.SetRowCount(nRows+1);
			m_Grid.SetTextMatrix(nRows, 0, _T("%.0f 구간(중앙부)"), (double)i+1);
			m_Grid.SetRowCount(nRows+2);
			m_Grid.SetTextMatrix(nRows+1, 0, _T("%.0f 구간(단  부)"), (double)i+1);

			long nCol = 1;
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol, CT_COMBO);		
			nRows++;		nCol = 1;
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);		
			nRows++;
		}
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputRebarOthers::SetGridTitleHole()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	
	if(pBri->GetQtyHDanTotalGuardFence()==0) return;

	long nRows = 1;
	long nCols = 12;

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowResize(FALSE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(50);

	long nCol=0;
	m_Grid.SetColumnWidth(nCol, 100);
	m_Grid.SetTextMatrix(0,	nCol++, _T("구 간"));
	m_Grid.SetTextMatrix(0,	nCol++, _T("HR"));
	m_Grid.SetTextMatrix(0, nCol++, _T("VR"));
	m_Grid.SetTextMatrix(0, nCol++, _T("SR"));
	m_Grid.SetColumnWidth(nCol, 80);
	m_Grid.SetTextMatrix(0, nCol++, _T("주철근 간격"));
	m_Grid.SetTextMatrix(0, nCol++, _T("E1"));
	m_Grid.SetTextMatrix(0, nCol++, _T("E2"));
	m_Grid.SetTextMatrix(0, nCol++, _T("E3"));
	//m_Grid.SetTextMatrix(0, nCol++, _T("E4"));
	m_Grid.SetTextMatrix(0, nCol++, _T("DC1"));
	m_Grid.SetTextMatrix(0, nCol++, _T("DC2"));
	m_Grid.SetTextMatrix(0, nCol++, _T("DC3"));
	m_Grid.SetTextMatrix(0, nCol++, _T("L1"));

	long i=0; for(i=0; i<pBri->GetQtyHDan(); i++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailHole())
		{
			m_Grid.SetRowCount(nRows+1);
			m_Grid.SetTextMatrix(nRows, 0, _T("%.0f 구간(중앙부)"), (double)i+1);
			m_Grid.SetRowCount(nRows+2);
			m_Grid.SetTextMatrix(nRows+1, 0, _T("%.0f 구간(단  부)"), (double)i+1);

			long nCol = 1;
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol, CT_COMBO);
			nRows++;		nCol = 1;
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);		
			nRows++;
		}
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}


void CInputRebarOthers::SetGridTitleHoleFootway()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	
	if(pBri->GetQtyHDanTotalGuardFence()==0) return;

	long nRows = 1;
	long nCols = 12;

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowResize(FALSE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(50);

	long nCol=0;
	m_Grid.SetColumnWidth(nCol, 100);
	m_Grid.SetTextMatrix(0,	nCol++, _T("구 간"));
	m_Grid.SetTextMatrix(0,	nCol++, _T("HR"));
	m_Grid.SetTextMatrix(0, nCol++, _T("VR"));
	m_Grid.SetTextMatrix(0, nCol++, _T("SR"));
	m_Grid.SetColumnWidth(nCol, 80);
	m_Grid.SetTextMatrix(0, nCol++, _T("주철근 간격"));
	m_Grid.SetTextMatrix(0, nCol++, _T("E1"));
	m_Grid.SetTextMatrix(0, nCol++, _T("E2"));
	m_Grid.SetTextMatrix(0, nCol++, _T("E3"));
	//m_Grid.SetTextMatrix(0, nCol++, _T("E4"));
	m_Grid.SetTextMatrix(0, nCol++, _T("DC1"));
	m_Grid.SetTextMatrix(0, nCol++, _T("DC2"));
	m_Grid.SetTextMatrix(0, nCol++, _T("DC3"));
	m_Grid.SetTextMatrix(0, nCol++, _T("L1"));

	long i=0; for(i=0; i<pBri->GetQtyHDan(); i++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailHoleFootway())
		{
			m_Grid.SetRowCount(nRows+1);
			m_Grid.SetTextMatrix(nRows, 0, _T("%.0f 구간(중앙부)"), (double)i+1);
			m_Grid.SetRowCount(nRows+2);
			m_Grid.SetTextMatrix(nRows+1, 0, _T("%.0f 구간(단  부)"), (double)i+1);

			long nCol = 1;
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol, CT_COMBO);		
			nRows++;		nCol = 1;
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);
			m_Grid.SetCellType(nRows, nCol++, CT_COMBO);		
			nRows++;
		}
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputRebarOthers::SetGridTitleMonument()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	long nRows = 2;
	long nCols = 19;

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowResize(FALSE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount(0);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(44);
	m_Grid.SetColumnWidth(0, 48);
	m_Grid.SetColumnWidth(1, 48);

	long nCol=0;
	m_Grid.SetTextMatrix(0,	nCol++, _T("HR"));
	m_Grid.SetTextMatrix(0,	nCol++, _T("VR"));
	m_Grid.SetTextMatrix(0,	nCol++, _T("H"));
	m_Grid.SetTextMatrix(0,	nCol++, _T("L"));
	m_Grid.SetTextMatrix(0,	nCol++, _T("nA"));
	m_Grid.SetTextMatrix(0,	nCol++, _T("A"));
	m_Grid.SetTextMatrix(0,	nCol++, _T("A1"));
	m_Grid.SetTextMatrix(0,	nCol++, _T("nB"));
	m_Grid.SetTextMatrix(0,	nCol++, _T("B"));
	m_Grid.SetTextMatrix(0,	nCol++, _T("nC"));
	m_Grid.SetTextMatrix(0,	nCol++, _T("C"));
	m_Grid.SetTextMatrix(0,	nCol++, _T("C1"));
	m_Grid.SetTextMatrix(0,	nCol++, _T("C2"));
	m_Grid.SetTextMatrix(0,	nCol++, _T("C3"));
	m_Grid.SetTextMatrix(0,	nCol++, _T("nD"));
	m_Grid.SetTextMatrix(0,	nCol++, _T("D"));
	m_Grid.SetTextMatrix(0,	nCol++, _T("D1"));
	m_Grid.SetTextMatrix(0,	nCol++, _T("D2"));
	m_Grid.SetTextMatrix(0,	nCol++, _T("D3"));

	m_Grid.SetCellType(1, 0, CT_COMBO);
	m_Grid.SetCellType(1, 1, CT_COMBO);

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputRebarOthers::SetDataInit()
{
	CString strTab = GetNameOfTab();
	if(strTab==_T("방호벽") || strTab==_T("중분대") || strTab==_T("중분대1/2") || strTab==_T("난간기초") || strTab==_T("자갈막이(철근)"))
	{
		if(m_bGWhalf && strTab==_T("중분대"))
			SetDataInitCentralWall();
		else
			SetDataInitGuardFenceAndCentralWall(strTab);
	}
	else if(strTab==_T("방음벽 기초") || strTab == _T("방호벽(방음벽기초타입)"))
		SetDataInitSoundProof(strTab);
	else if(strTab==_T("흙막이벽"))
		SetDataInitRetainingWall();
	else if(strTab==_T("연석"))
		SetDataInitRailGuardFence();
	else if(strTab==_T("연석+보도"))
		SetDataInitFootwayBlock();
	else if(strTab==_T("통신구"))
		SetDataInitHole();
	else if(strTab==_T("통신구+보도"))
		SetDataInitHoleFootway();
	else if(strTab==_T("교명주 받침"))
		SetDataInitMonument();
	else if(strTab==_T("무근 콘크리트"))
		SetDataInitNonRebarConcrete();

	// 도움말
	CCellID cID = m_Grid.GetFocusCell();
	CString strCol = m_Grid.GetTextMatrix(0,cID.col);
	CString strComment = GetComment(strCol);
	m_pIn->SetComment(strComment);
	
	// 상태바에서 단위계 변경시 fck 단위계 변경시켜야 되므로 호출함.
	SetControl();
}
	
void CInputRebarOthers::SetDataInitCentralWall()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	if(pBri->GetQtyHDanTotalGuardFence()==0) return;

	long nRows = 0;
	CGuardWallRC* pGW = NULL;
	long nQtyHDan = pBri->GetQtyHDan();
	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;
		
		if(pGW->IsTypeHDanRoadCentralWallFull())
		{
			CString strCGS = _T("kgf/cm²");
			CString strSI  = pStd->GetStringUnitType(strCGS);
			m_dFck = pStd->m_nTypeUnit==0 ? pGW->m_dFck : pStd->GetValueUnitType(pGW->m_dFck, strCGS, strSI);
			m_dFy = pStd->m_nTypeUnit==0 ? pGW->m_dFy : pStd->GetValueUnitType(pGW->m_dFy, strCGS, strSI);
			m_dAggregate	= pGW->m_dAggregate;

			long nCol=1;
			CString str(_T(""));
			str.Format(_T("%.0f @ %.0f"), pGW->m_dEC, pGW->m_dCTC);
	
			m_Grid.SetTextMatrix(++nRows, nCol++, GetStringDia(pGW->m_dVR));
			m_Grid.SetTextMatrix(nRows, nCol++, _T("CTC %.0f"), pGW->m_dCTC);			// 주철근 간격
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF1);					// 철근 치수 1
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF2);					// 철근 치수 2

		} 
	}
	m_Grid.SetRedraw(TRUE,TRUE);

	CCellID cID = m_Grid.GetFocusCell();
	CString strComment = _T("");
	CString strCol = m_Grid.GetTextMatrix(0,cID.col);
	CString strGugan = m_Grid.GetTextMatrix(cID.row, cID.col); 
//	long nGugan = (long)GetNumberInString(strGugan);

//	if(strCol=="전단철근 구간")	
//		strComment.Format(_T("전단 철근이 상단에서부터 수평철근 %d 구간당 하나씩 위치합니다."), nGugan);

	m_pIn->SetComment(strComment);
}

void CInputRebarOthers::SetDataInitGuardFenceAndCentralWall(CString strTab)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	if(pBri->GetQtyHDanTotalGuardFence()==0) return;

	long nRows = 0;
	BOOL bType = FALSE;
	CGuardWallRC* pGW = NULL;
	long nQtyHDan = pBri->GetQtyHDan();

	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;
	
		if(strTab==_T("방호벽"))				bType = pGW->IsTypeHDanRoadGuardFence();
		else if(strTab==_T("중분대"))			bType = pGW->IsTypeHDanRoadCentralWallFull();
		else if(strTab==_T("중분대1/2"))		bType = pGW->IsTypeHDanRoadCentralWallHalf();
		else if(strTab==_T("난간기초"))			bType = pGW->IsTypeHDanRoadHandRail();
		else if(strTab==_T("자갈막이(철근)"))	bType = pGW->IsTypeHDanRailDetectGravel();
		
		if(bType && !pGW->m_bSoundProof)
		{
			CString strCGS = _T("kgf/cm²");
			CString strSI  = pStd->GetStringUnitType(strCGS);
			m_dFck = pStd->m_nTypeUnit==0 ? pGW->m_dFck : pStd->GetValueUnitType(pGW->m_dFck, strCGS, strSI);
			m_dFy = pStd->m_nTypeUnit==0 ? pGW->m_dFy : pStd->GetValueUnitType(pGW->m_dFy, strCGS, strSI);
			m_dAggregate	= pGW->m_dAggregate;

			long nCol=1;
			CString str(_T(""));
			str.Format(_T("%.0f @ %.0f"), pGW->m_dEC, pGW->m_dCTC);
	
			m_Grid.SetTextMatrix(++nRows, nCol++, GetStringDia(pGW->m_dHR));
			m_Grid.SetTextMatrix(nRows, nCol++, GetStringDia(pGW->m_dVR));
			m_Grid.SetTextMatrix(nRows, nCol++, GetStringDia(pGW->m_dSR));
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dE);					// 배력철근 구간수
			m_Grid.SetTextMatrix(nRows, nCol++, _T("CTC %.0f"), pGW->m_dCTC);			// 주철근 간격
			m_Grid.SetTextMatrix(nRows, nCol++, str);									// 전단철근 묶음수
			m_Grid.SetTextMatrix(nRows, nCol++, _T("배력근 %.0f개당"), pGW->m_dSC);		// 전단철근 간격
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc1);				// 피복 1
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc2);				// 피복 2
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc3);				// 피복 3
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dL1);					// 철근 치수 4
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF1);					// 철근 치수 1
			
			if(!pGW->IsTypeHDanRailDetectGravel())	// 자갈막이는 F2, F3 없다.
			{
				m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF2);				// 철근 치수 2
				m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF3);				// 철근 치수 3
			}

			CString sTypeRebar	= pGW->m_nTypeRebar == 0 ? "타입1" : "타입2";
			if(!pGW->IsTypeHDanRailGuardFence() && !pGW->IsTypeHDanRoadGuardFence())
			{
				m_Grid.SetItemState(nRows, nCol, m_Grid.GetItemState(nRows, nCol) | GVIS_READONLY);
				m_Grid.SetItemBkColour(nRows, nCol, CLR_READONLY);
				m_Grid.SetItemFgColour(nRows, nCol, RGB(128, 128, 128));
			}			
			m_Grid.SetTextMatrix(nRows, nCol++, sTypeRebar);
		} 
	}
	m_Grid.SetRedraw(TRUE,TRUE);

	CCellID cID = m_Grid.GetFocusCell();
	CString strComment = _T("");
	CString strCol = m_Grid.GetTextMatrix(0,cID.col);
	CString strGugan = m_Grid.GetTextMatrix(cID.row, cID.col); 
	long nGugan = (long)GetNumberInString(strGugan);

	if(strCol=="전단철근 구간")	
		strComment.Format(_T("전단 철근이 상단에서부터 수평철근 %d 구간당 하나씩 위치합니다."), nGugan);

	m_pIn->SetComment(strComment);
}

void CInputRebarOthers::SetDataInitSoundProof(CString strTab)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	if(pBri->GetQtyHDanTotalGuardFence()==0) return;

	long nGugan = GetGugan();
	if(nGugan<0)	// 다른 다이얼로그 갔다오는 경우
	{
		SetDataDefaultGuganCombo();
		SetGridTitleSoundProof();
		nGugan = GetGugan();
	}
	CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nGugan);
	if(!pGW) return;

	CString strCGS = _T("kgf/cm²");
	CString strSI  = pStd->GetStringUnitType(strCGS);
	m_dFck = pStd->m_nTypeUnit==0 ? pGW->m_dFck : pStd->GetValueUnitType(pGW->m_dFck, strCGS, strSI);
	m_dFy = pStd->m_nTypeUnit==0 ? pGW->m_dFy : pStd->GetValueUnitType(pGW->m_dFy, strCGS, strSI);
	m_dAggregate	= pGW->m_dAggregate;

	long nCol  = 1;
	CString str(_T(""));
	str.Format(_T("%.0f @ %.0f"), pGW->m_dEC, pGW->m_dCTC);
	
	m_Grid.SetTextMatrix(1, nCol++, GetStringDia(pGW->m_dHR));
	m_Grid.SetTextMatrix(1, nCol++, GetStringDia(pGW->m_dVR));
	m_Grid.SetTextMatrix(1, nCol++, GetStringDia(pGW->m_dSR));
	m_Grid.SetTextMatrix(1, nCol++, _T("CTC %.0f"), pGW->m_dCTC);
	m_Grid.SetTextMatrix(1, nCol++, str);									// 전단철근 묶음수
	m_Grid.SetTextMatrix(1, nCol++, _T("배력근 %.0f개당"), pGW->m_dSC);		// 전단철근 간격

	long j=0; for(j=0; j<m_nUpperGugan+1; j++)
	{
		m_Grid.SetTextMatrix(1, nCol, _T("%.f"), (double)pGW->m_nU[j]);
		m_Grid.SetTextMatrix(2, nCol++, _T("%.f"), pGW->m_dU[j]);
	}
	for(j=0; j<m_nSideGugan+1; j++)
	{
		m_Grid.SetTextMatrix(1, nCol, _T("%.f"), (double)pGW->m_nS[j]);
		m_Grid.SetTextMatrix(2, nCol++, _T("%.f"), pGW->m_dS[j]);
	}

	m_Grid.SetTextMatrix(1, nCol++, _T("%.f"), pGW->m_dF1);
	m_Grid.SetTextMatrix(1, nCol++, _T("%.f"), pGW->m_dL1);
	m_Grid.SetTextMatrix(1, nCol++, _T("%.f"), pGW->m_ddc1);
	m_Grid.SetTextMatrix(1, nCol++, _T("%.f"), pGW->m_ddc2);
	m_Grid.SetTextMatrix(1, nCol++, _T("%.f"), pGW->m_ddc3);


	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputRebarOthers::SetDataInitRetainingWall()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	if(pBri->GetQtyHDanTotalGuardFence()==0) return;

	long nRows = 0;
	CGuardWallRC* pGW = NULL;
	long nQtyHDan = pBri->GetQtyHDan();

	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		pGW = pBri->GetGuardWallByHDan(i);	
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadRetainingWall())
		{
			CString strCGS = _T("kgf/cm²");
			CString strSI  = pStd->GetStringUnitType(strCGS);
			m_dFck = pStd->m_nTypeUnit==0 ? pGW->m_dFck : pStd->GetValueUnitType(pGW->m_dFck, strCGS, strSI);
			m_dFy = pStd->m_nTypeUnit==0 ? pGW->m_dFy : pStd->GetValueUnitType(pGW->m_dFy, strCGS, strSI);
			m_dAggregate	= pGW->m_dAggregate;

			long nCol=1;
			CString str(_T(""));
			str.Format(_T("%.0f @ %.0f"), pGW->m_dEC, pGW->m_dCTC);

			m_Grid.SetTextMatrix(++nRows, nCol++, GetStringDia(pGW->m_dHR));
			m_Grid.SetTextMatrix(nRows, nCol++, GetStringDia(pGW->m_dVR));
			m_Grid.SetTextMatrix(nRows, nCol++, GetStringDia(pGW->m_dSR));
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dE);					// 배력철근 구간수
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dEH);					// 배력철근 구간수
			m_Grid.SetTextMatrix(nRows, nCol++, _T("CTC %.0f"), pGW->m_dCTC);			// 주철근 간격
			m_Grid.SetTextMatrix(nRows, nCol++, str);									// 전단철근 묶음수
			m_Grid.SetTextMatrix(nRows, nCol++, _T("배력근 %.0f개당"), pGW->m_dSC);		// 전단철근 간격
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc1);				// 피복 1
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc2);				// 피복 2
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc3);				// 피복 3
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dL1);					// 철근 치수 4

		} 
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputRebarOthers::SetDataInitRailGuardFence()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	if(pBri->GetQtyHDanTotalGuardFence()==0) return;

	long nRows = 0;
	CGuardWallRC* pGW = NULL;
	long nQtyHDan = pBri->GetQtyHDan();

	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		pGW = pBri->GetGuardWallByHDan(i);	
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailGuardFence())
		{
			CString strCGS = _T("kgf/cm²");
			CString strSI  = pStd->GetStringUnitType(strCGS);
			m_dFck = pStd->m_nTypeUnit==0 ? pGW->m_dFck : pStd->GetValueUnitType(pGW->m_dFck, strCGS, strSI);
			m_dFy = pStd->m_nTypeUnit==0 ? pGW->m_dFy : pStd->GetValueUnitType(pGW->m_dFy, strCGS, strSI);
			m_dAggregate	= pGW->m_dAggregate;

			long nCol=1;
			CString str(_T(""));
			str.Format(_T("%.0f @ %.0f"), pGW->m_dEC, pGW->m_dCTC);
			
			m_Grid.SetTextMatrix(++nRows, nCol++, GetStringDia(pGW->m_dHR));
			m_Grid.SetTextMatrix(nRows, nCol++, GetStringDia(pGW->m_dVR));
			m_Grid.SetTextMatrix(nRows, nCol++, GetStringDia(pGW->m_dSR));
			m_Grid.SetTextMatrix(nRows, nCol++, _T("CTC %.0f"), pGW->m_dCTC);			// 주철근 간격
			m_Grid.SetTextMatrix(nRows, nCol++, str);									// 전단철근 묶음수
			m_Grid.SetTextMatrix(nRows, nCol++, _T("배력근 %.0f개당"), pGW->m_dSC);		// 전단철근 간격
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dE);					// 배력철근 구간수
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF2);					// 배력철근 간격
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc1);				// 피복 1
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc2);				// 피복 2
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc3);				// 피복 3
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dL1);					// 철근 치수 4
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF1);					// 철근 치수 1

		} 
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputRebarOthers::SetDataInitFootwayBlock()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	if(pBri->GetQtyHDanTotalGuardFence()==0) return;

	long nRows = 0;
	CGuardWallRC* pGW = NULL;
	long nQtyHDan = pBri->GetQtyHDan();

	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		pGW = pBri->GetGuardWallByHDan(i);	
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailFootwayBlock())
		{
			CString strCGS = _T("kgf/cm²");
			CString strSI  = pStd->GetStringUnitType(strCGS);
			m_dFck = pStd->m_nTypeUnit==0 ? pGW->m_dFck : pStd->GetValueUnitType(pGW->m_dFck, strCGS, strSI);
			m_dFy = pStd->m_nTypeUnit==0 ? pGW->m_dFy : pStd->GetValueUnitType(pGW->m_dFy, strCGS, strSI);
			m_dAggregate	= pGW->m_dAggregate;

			long nCol = 1;
			CString str(_T(""));
			str.Format(_T("%.0f @ %.0f"), pGW->m_dEC, pGW->m_dCTC);
			
			nRows++;
			m_Grid.SetTextMatrix(nRows, nCol++, GetStringDia(pGW->m_dHR));
			m_Grid.SetTextMatrix(nRows, nCol++, GetStringDia(pGW->m_dVR));
			m_Grid.SetTextMatrix(nRows, nCol++, GetStringDia(pGW->m_dSR));
			m_Grid.SetTextMatrix(nRows, nCol++, _T("CTC %.0f"), pGW->m_dCTC);			// 주철근 간격
			m_Grid.SetTextMatrix(nRows, nCol++, str);									// 전단철근 묶음수
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF1);					// 배력철근 구간수 상
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF2);					// 배력철근 구간수 하
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF3);					// 배력철근 구간수 수평
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF);					// 배력철근 구간수 우
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc1);				// 피복 1
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc2);				// 피복 2
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc3);				// 피복 3
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dL1);					// 철근 치수 4
			long j=0; for(j=1; j<14; j++)
			{
				m_Grid.SetItemState(nRows, j, m_Grid.GetItemState(nRows, j) | GVIS_READONLY);
				m_Grid.SetItemBkColour(nRows, j, CLR_READONLY);
				m_Grid.SetItemFgColour(nRows, j, RGB(128, 128, 128));
			}
			nRows++;	nCol = 1;
			m_Grid.SetTextMatrix(nRows, nCol++, GetStringDia(pGW->m_dHR));
			m_Grid.SetTextMatrix(nRows, nCol++, GetStringDia(pGW->m_dVR));
			m_Grid.SetTextMatrix(nRows, nCol++, GetStringDia(pGW->m_dSR));
			m_Grid.SetTextMatrix(nRows, nCol++, _T("CTC %.0f"), pGW->m_dCTC);			// 주철근 간격
			m_Grid.SetTextMatrix(nRows, nCol++, str);									// 전단철근 묶음수
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF1);					// 배력철근 구간수 상
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF2);					// 배력철근 구간수 하
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF3);					// 배력철근 구간수 수평
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF);					// 배력철근 구간수 우
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc1);				// 피복 1
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc2);				// 피복 2
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc3);				// 피복 3
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dL1);					// 철근 치수 4

		} 
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputRebarOthers::SetDataInitHole()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	if(pBri->GetQtyHDanTotalGuardFence()==0) return;

	long nRows = 0;
	CGuardWallRC* pGW = NULL;
	long nQtyHDan = pBri->GetQtyHDan();

	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailHole())
		{
			CString strCGS = _T("kgf/cm²");
			CString strSI  = pStd->GetStringUnitType(strCGS);
			m_dFck = pStd->m_nTypeUnit==0 ? pGW->m_dFck : pStd->GetValueUnitType(pGW->m_dFck, strCGS, strSI);
			m_dFy = pStd->m_nTypeUnit==0 ? pGW->m_dFy : pStd->GetValueUnitType(pGW->m_dFy, strCGS, strSI);
			m_dAggregate	= pGW->m_dAggregate;

			long nCol = 1;
			
			nRows++;
			m_Grid.SetTextMatrix(nRows, nCol++, GetStringDia(pGW->m_dHR));
			m_Grid.SetTextMatrix(nRows, nCol++, GetStringDia(pGW->m_dVR));
			m_Grid.SetTextMatrix(nRows, nCol++, GetStringDia(pGW->m_dSR));
			m_Grid.SetTextMatrix(nRows, nCol++, _T("CTC %.0f"), pGW->m_dCTC);			// 주철근 간격
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF1);					// 배력철근 구간수 상
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF2);					// 배력철근 구간수 하
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF3);					// 배력철근 구간수 수평
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc1);				// 피복 1
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc2);				// 피복 2
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc3);				// 피복 3
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dL1);					// 철근 치수 4
			long j=0; for(j=1; j<12; j++)
			{
				m_Grid.SetItemState(nRows, j, m_Grid.GetItemState(nRows, j) | GVIS_READONLY);
				m_Grid.SetItemBkColour(nRows, j, CLR_READONLY);
				m_Grid.SetItemFgColour(nRows, j, RGB(128, 128, 128));
			}
			nRows++;	nCol = 1;
			m_Grid.SetTextMatrix(nRows, nCol++, GetStringDia(pGW->m_dHR));
			m_Grid.SetTextMatrix(nRows, nCol++, GetStringDia(pGW->m_dVR));
			m_Grid.SetTextMatrix(nRows, nCol++, GetStringDia(pGW->m_dSR));
			m_Grid.SetTextMatrix(nRows, nCol++, _T("CTC %.0f"), pGW->m_dCTC);			// 주철근 간격
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF1);					// 배력철근 구간수 상
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF2);					// 배력철근 구간수 하
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF3);					// 배력철근 구간수 수평
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc1);				// 피복 1
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc2);				// 피복 2
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc3);				// 피복 3
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dL1);					// 철근 치수 4

		} 
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputRebarOthers::SetDataInitHoleFootway()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	if(pBri->GetQtyHDanTotalGuardFence()==0) return;

	long nRows = 0;
	CGuardWallRC* pGW = NULL;
	long nQtyHDan = pBri->GetQtyHDan();

	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailHoleFootway())
		{
			CString strCGS = _T("kgf/cm²");
			CString strSI  = pStd->GetStringUnitType(strCGS);
			m_dFck = pStd->m_nTypeUnit==0 ? pGW->m_dFck : pStd->GetValueUnitType(pGW->m_dFck, strCGS, strSI);
			m_dFy = pStd->m_nTypeUnit==0 ? pGW->m_dFy : pStd->GetValueUnitType(pGW->m_dFy, strCGS, strSI);
			m_dAggregate	= pGW->m_dAggregate;

			long nCol = 1;
			
			nRows++;
			m_Grid.SetTextMatrix(nRows, nCol++, GetStringDia(pGW->m_dHR));
			m_Grid.SetTextMatrix(nRows, nCol++, GetStringDia(pGW->m_dVR));
			m_Grid.SetTextMatrix(nRows, nCol++, GetStringDia(pGW->m_dSR));
			m_Grid.SetTextMatrix(nRows, nCol++, _T("CTC %.0f"), pGW->m_dCTC);			// 주철근 간격
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF1);					// 배력철근 구간수 상
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF2);					// 배력철근 구간수 하
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF3);					// 배력철근 구간수 수평
			//m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF);				// 배력철근 구간수 우
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc1);				// 피복 1
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc2);				// 피복 2
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc3);				// 피복 3
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dL1);					// 철근 치수 4
			long j=0; for(j=1; j<12; j++)
			{
				m_Grid.SetItemState(nRows, j, m_Grid.GetItemState(nRows, j) | GVIS_READONLY);
				m_Grid.SetItemBkColour(nRows, j, CLR_READONLY);
				m_Grid.SetItemFgColour(nRows, j, RGB(128, 128, 128));
			}
			nRows++;	nCol = 1;
			m_Grid.SetTextMatrix(nRows, nCol++, GetStringDia(pGW->m_dHR));
			m_Grid.SetTextMatrix(nRows, nCol++, GetStringDia(pGW->m_dVR));
			m_Grid.SetTextMatrix(nRows, nCol++, GetStringDia(pGW->m_dSR));
			m_Grid.SetTextMatrix(nRows, nCol++, _T("CTC %.0f"), pGW->m_dCTC);			// 주철근 간격
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF1);					// 배력철근 구간수 상
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF2);					// 배력철근 구간수 하
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dF3);					// 배력철근 구간수 수평
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc1);				// 피복 1
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc2);				// 피복 2
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_ddc3);				// 피복 3
			m_Grid.SetTextMatrix(nRows, nCol++, _T("%.f"), pGW->m_dL1);					// 철근 치수 4
		} 
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputRebarOthers::SetDataInitMonument()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	long nCol = 0;
	CString strCGS = _T("kgf/cm²");
	CString strSI  = pStd->GetStringUnitType(strCGS);
	m_dFck = pStd->m_nTypeUnit==0 ? pBri->m_dMonuFck : pStd->GetValueUnitType(pBri->m_dMonuFck, strCGS, strSI);
	m_dFy = pStd->m_nTypeUnit==0 ? pBri->m_dMonuR_Fy : pStd->GetValueUnitType(pBri->m_dMonuR_Fy, strCGS, strSI);
	
 	m_Grid.SetTextMatrix(1, nCol++, GetStringDia(pBri->m_dMonuR_DiaHor));
	m_Grid.SetTextMatrix(1, nCol++, GetStringDia(pBri->m_dMonuR_DiaVer));
	m_Grid.SetTextMatrix(1, nCol++, _T("%.f"), pBri->m_dMonuR_H);
	m_Grid.SetTextMatrix(1, nCol++, _T("%.f"), pBri->m_dMonuR_L);
	m_Grid.SetTextMatrix(1, nCol++, COMMA(pBri->m_nMonuR_nA));
	m_Grid.SetTextMatrix(1, nCol++, _T("%.f"), pBri->m_dMonuR_A);
	m_Grid.SetTextMatrix(1, nCol++, _T("%.f"), pBri->m_dMonuR_A1);
	m_Grid.SetTextMatrix(1, nCol++, COMMA(pBri->m_nMonuR_nB));
	m_Grid.SetTextMatrix(1, nCol++, _T("%.f"), pBri->m_dMonuR_B);
	m_Grid.SetTextMatrix(1, nCol++, COMMA(pBri->m_nMonuR_nC));
	m_Grid.SetTextMatrix(1, nCol++, _T("%.f"), pBri->m_dMonuR_C);
	m_Grid.SetTextMatrix(1, nCol++, _T("%.f"), pBri->m_dMonuR_C1);
	m_Grid.SetTextMatrix(1, nCol++, _T("%.f"), pBri->m_dMonuR_C2);
	m_Grid.SetTextMatrix(1, nCol++, _T("%.f"), pBri->m_dMonuR_C3);
	m_Grid.SetTextMatrix(1, nCol++, COMMA(pBri->m_nMonuR_nD));
	m_Grid.SetTextMatrix(1, nCol++, _T("%.f"), pBri->m_dMonuR_D);
	m_Grid.SetTextMatrix(1, nCol++, _T("%.f"), pBri->m_dMonuR_D1);
	m_Grid.SetTextMatrix(1, nCol++, _T("%.f"), pBri->m_dMonuR_D2);
	m_Grid.SetTextMatrix(1, nCol++, _T("%.f"), pBri->m_dMonuR_D3);

	m_Grid.SetRedraw(TRUE,TRUE);

	m_dAggregate = pBri->m_dMonuAggregate;

	UpdateData(FALSE);
}

void CInputRebarOthers::SetDataSave()
{
	CString strTab = GetNameOfTab();
	
	if(strTab==_T("방호벽") || strTab==_T("중분대") || strTab==_T("중분대1/2") || strTab==_T("난간기초") || strTab==_T("자갈막이(철근)"))
	{
		if(m_bGWhalf && strTab==_T("중분대"))
			SetDataSaveCentralWall();
		else
			SetDataSaveGuardFenceAndCentralWall(strTab);
	}
	else if(strTab==_T("방음벽 기초") || strTab == _T("방호벽(방음벽기초타입)"))
		SetDataSaveSoundProof(strTab);
	else if(strTab==_T("흙막이벽"))
		SetDataSaveRetainingWall();
	else if(strTab==_T("연석"))
		SetDataSaveRailGuardFence();
	else if(strTab==_T("연석+보도"))
		SetDataSaveFootwayBlock();
	else if(strTab==_T("통신구"))
		SetDataSaveHole();
	else if(strTab==_T("통신구+보도"))
		SetDataSaveHoleFootway();
	else if(strTab==_T("교명주 받침"))
		SetDataSaveMonument();
	else if(strTab==_T("무근 콘크리트"))
		SetDataSaveNonRebarConcrete();

	m_pIn->m_pDoc->SetModifiedFlag();
}

void CInputRebarOthers::SetDataSaveCentralWall()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	if(pBri->GetQtyHDanTotalGuardFence()==0) return;

	long nRows = 0;
	long nQtyHDan = pBri->GetQtyHDan();
	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;
		
		if(pGW->IsTypeHDanRoadCentralWallFull())
		{
			long nCol=1;
			CString str(_T(""));

			pGW->m_dVR	= pStd->GetDia(m_Grid.GetTextMatrix(++nRows, nCol++));
			str			= m_Grid.GetTextMatrix(nRows, nCol++);
			pGW->m_dCTC = GetNumberInString(str);
			pGW->m_dF1  = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dF2  = m_Grid.GetTextMatrixLong(nRows, nCol++);
		}
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputRebarOthers::SetDataSaveGuardFenceAndCentralWall(CString strTab)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	if(pBri->GetQtyHDanTotalGuardFence()==0) return;

	long nRows = 0;
	CGuardWallRC* pGW = NULL;
	BOOL bType = FALSE;
	long nQtyHDan = pBri->GetQtyHDan();

	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;

		if(strTab==_T("방호벽"))				bType = pGW->IsTypeHDanRoadGuardFence();
		else if(strTab==_T("중분대"))			bType = pGW->IsTypeHDanRoadCentralWallFull();
		else if(strTab==_T("중분대1/2"))		bType = pGW->IsTypeHDanRoadCentralWallHalf();
		else if(strTab==_T("난간기초"))			bType = pGW->IsTypeHDanRoadHandRail();
		else if(strTab==_T("자갈막이(철근)"))	bType = pGW->IsTypeHDanRailDetectGravel();
		
		if(bType && !pGW->m_bSoundProof)
		{
			long nCol=1;
			CString str(_T(""));

			pGW->m_dHR	= pStd->GetDia(m_Grid.GetTextMatrix(++nRows, nCol++));
			pGW->m_dVR	= pStd->GetDia(m_Grid.GetTextMatrix(nRows, nCol++));
			pGW->m_dSR	= pStd->GetDia(m_Grid.GetTextMatrix(nRows, nCol++));
			pGW->m_dE	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			str = m_Grid.GetTextMatrix(nRows, nCol++);
			pGW->m_dCTC = GetNumberInString(str);
			str = m_Grid.GetTextMatrix(nRows, nCol++);
			pGW->m_dEC = atof(str.Left(1));
			str = m_Grid.GetTextMatrix(nRows, nCol++);
			pGW->m_dSC  = GetNumberInString(str);
			pGW->m_ddc1 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_ddc2 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_ddc3 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dL1	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dF1  = m_Grid.GetTextMatrixLong(nRows, nCol++);
			
			if(!pGW->IsTypeHDanRailDetectGravel())	// 자갈막이는 F2, F3 없다.
			{
				pGW->m_dF2  = m_Grid.GetTextMatrixLong(nRows, nCol++);
				pGW->m_dF3  = m_Grid.GetTextMatrixLong(nRows, nCol++);
			}

			str	= m_Grid.GetTextMatrix(nRows, nCol++);
			pGW->m_nTypeRebar	= str == "타입1" ? 0 : 1;
			if(!pGW->IsTypeHDanRailGuardFence() && !pGW->IsTypeHDanRoadGuardFence())
				pGW->m_nTypeRebar	= 0;
		}
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputRebarOthers::SetDataSaveSoundProof(CString strTab)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	if(pBri->GetQtyHDanTotalGuardFence()==0) return;

	long nGugan = GetGugan();
	CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nGugan);
	if(!pGW) return;

	long nCol = 1;
	CString str(_T(""));
	
	pGW->m_dHR	= pStd->GetDia(m_Grid.GetTextMatrix(1, nCol++));
	pGW->m_dVR	= pStd->GetDia(m_Grid.GetTextMatrix(1, nCol++));
	pGW->m_dSR	= pStd->GetDia(m_Grid.GetTextMatrix(1, nCol++));
	str = m_Grid.GetTextMatrix(1, nCol++);
	pGW->m_dCTC = GetNumberInString(str);
	str = m_Grid.GetTextMatrix(1, nCol++);
	pGW->m_dEC = atof(str.Left(1));	
	str = m_Grid.GetTextMatrix(1, nCol++);
	pGW->m_dSC  = GetNumberInString(str);

	long j=0; for(j=0; j<m_nUpperGugan+1; j++)
	{
		pGW->m_nU[j] = m_Grid.GetTextMatrixLong(1, nCol);
		pGW->m_dU[j] = m_Grid.GetTextMatrixLong(2, nCol++);
	}
	for(j=0; j<m_nSideGugan+1; j++)
	{
		pGW->m_nS[j] = m_Grid.GetTextMatrixLong(1, nCol);
		pGW->m_dS[j] = m_Grid.GetTextMatrixLong(2, nCol++);
	}
	
	pGW->m_dF1  = m_Grid.GetTextMatrixLong(1, nCol++);
	pGW->m_dL1	= m_Grid.GetTextMatrixLong(1, nCol++);
	pGW->m_ddc1 = m_Grid.GetTextMatrixLong(1, nCol++);
	pGW->m_ddc2 = m_Grid.GetTextMatrixLong(1, nCol++);
	pGW->m_ddc3 = m_Grid.GetTextMatrixLong(1, nCol++);

	pGW->m_nUGugan = m_nUpperGugan + 1;
	pGW->m_nSGugan = m_nSideGugan + 1;

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputRebarOthers::SetDataSaveRetainingWall()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	if(pBri->GetQtyHDanTotalGuardFence()==0) return;

	long nRows = 0;
	long nQtyHDan = pBri->GetQtyHDan();

	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadRetainingWall())
		{
			long nCol=1;
			CString str(_T(""));

			pGW->m_dHR	= pStd->GetDia(m_Grid.GetTextMatrix(++nRows, nCol++));
			pGW->m_dVR	= pStd->GetDia(m_Grid.GetTextMatrix(nRows, nCol++));
			pGW->m_dSR	= pStd->GetDia(m_Grid.GetTextMatrix(nRows, nCol++));
			pGW->m_dE	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dEH	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			str = m_Grid.GetTextMatrix(nRows, nCol++);
			pGW->m_dCTC = GetNumberInString(str);
			str = m_Grid.GetTextMatrix(nRows, nCol++);
			pGW->m_dEC = atof(str.Left(1));
			str = m_Grid.GetTextMatrix(nRows, nCol++);
			pGW->m_dSC  = GetNumberInString(str);
			pGW->m_ddc1 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_ddc2 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_ddc3 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dL1	= m_Grid.GetTextMatrixLong(nRows, nCol++);
		}
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputRebarOthers::SetDataSaveRailGuardFence()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	if(pBri->GetQtyHDanTotalGuardFence()==0) return;

	long nRows = 0;
	long nQtyHDan = pBri->GetQtyHDan();

	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailGuardFence())
		{
			long nCol=1;
			CString str(_T(""));

			pGW->m_dHR	= pStd->GetDia(m_Grid.GetTextMatrix(++nRows, nCol++));
			pGW->m_dVR	= pStd->GetDia(m_Grid.GetTextMatrix(nRows, nCol++));
			pGW->m_dSR	= pStd->GetDia(m_Grid.GetTextMatrix(nRows, nCol++));
			str = m_Grid.GetTextMatrix(nRows, nCol++);
			pGW->m_dCTC = GetNumberInString(str);
			str = m_Grid.GetTextMatrix(nRows, nCol++);
			pGW->m_dEC = atof(str.Left(1));
			str = m_Grid.GetTextMatrix(nRows, nCol++);
			pGW->m_dSC  = GetNumberInString(str);
			pGW->m_dE	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dF2	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_ddc1 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_ddc2 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_ddc3 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dL1	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dF1  = m_Grid.GetTextMatrixLong(nRows, nCol++);
		}
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputRebarOthers::SetDataSaveFootwayBlock()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	if(pBri->GetQtyHDanTotalGuardFence()==0) return;

	long nRows = 0;
	long nQtyHDan = pBri->GetQtyHDan();

	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i);	
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailFootwayBlock())
		{
			long nCol = 1;
			CString str(_T(""));

			nRows++;
			pGW->m_dHR	= pStd->GetDia(m_Grid.GetTextMatrix(nRows, nCol++));
			pGW->m_dVR	= pStd->GetDia(m_Grid.GetTextMatrix(nRows, nCol++));
			pGW->m_dSR	= pStd->GetDia(m_Grid.GetTextMatrix(nRows, nCol++));
			str = m_Grid.GetTextMatrix(nRows, nCol++);
			pGW->m_dCTC = GetNumberInString(str);
			str = m_Grid.GetTextMatrix(nRows, nCol++);
			pGW->m_dEC = atof(str.Left(1));
			pGW->m_dF1	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dF2	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dF3	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dF	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_ddc1 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_ddc2 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_ddc3 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dL1	= m_Grid.GetTextMatrixLong(nRows, nCol);
			
			nRows++;	nCol = 1;
			pGW->m_dHR	= pStd->GetDia(m_Grid.GetTextMatrix(nRows, nCol++));
			pGW->m_dVR	= pStd->GetDia(m_Grid.GetTextMatrix(nRows, nCol++));
			pGW->m_dSR	= pStd->GetDia(m_Grid.GetTextMatrix(nRows, nCol++));
			str = m_Grid.GetTextMatrix(nRows, nCol++);
			pGW->m_dCTC = GetNumberInString(str);
			str = m_Grid.GetTextMatrix(nRows, nCol++);
			pGW->m_dEC = atof(str.Left(1));
			pGW->m_dF1	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dF2	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dF3	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dF	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_ddc1 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_ddc2 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_ddc3 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dL1	= m_Grid.GetTextMatrixLong(nRows, nCol++);
		}
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputRebarOthers::SetDataSaveHole()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	if(pBri->GetQtyHDanTotalGuardFence()==0) return;

	long nRows = 0;
	long nQtyHDan = pBri->GetQtyHDan();

	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailHole())
		{
			long nCol = 1;
			CString str(_T(""));

			nRows++;
			pGW->m_dHR	= pStd->GetDia(m_Grid.GetTextMatrix(nRows, nCol++));
			pGW->m_dVR	= pStd->GetDia(m_Grid.GetTextMatrix(nRows, nCol++));
			pGW->m_dSR	= pStd->GetDia(m_Grid.GetTextMatrix(nRows, nCol++));
			str = m_Grid.GetTextMatrix(nRows, nCol++);
			pGW->m_dCTC = GetNumberInString(str);
			pGW->m_dF1	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dF2	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dF3	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			//pGW->m_dF	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_ddc1 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_ddc2 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_ddc3 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dL1	= m_Grid.GetTextMatrixLong(nRows, nCol);
			
			nRows++;	nCol = 1;
			pGW->m_dHR	= pStd->GetDia(m_Grid.GetTextMatrix(nRows, nCol++));
			pGW->m_dVR	= pStd->GetDia(m_Grid.GetTextMatrix(nRows, nCol++));
			pGW->m_dSR	= pStd->GetDia(m_Grid.GetTextMatrix(nRows, nCol++));
			str = m_Grid.GetTextMatrix(nRows, nCol++);
			pGW->m_dCTC = GetNumberInString(str);
			pGW->m_dF1	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dF2	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dF3	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			//pGW->m_dF	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_ddc1 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_ddc2 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_ddc3 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dL1	= m_Grid.GetTextMatrixLong(nRows, nCol++);
		}
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}


void CInputRebarOthers::SetDataSaveHoleFootway()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	if(pBri->GetQtyHDanTotalGuardFence()==0) return;

	long nRows = 0;
	long nQtyHDan = pBri->GetQtyHDan();

	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailHoleFootway())
		{
			long nCol = 1;
			CString str(_T(""));

			nRows++;
			pGW->m_dHR	= pStd->GetDia(m_Grid.GetTextMatrix(nRows, nCol++));
			pGW->m_dVR	= pStd->GetDia(m_Grid.GetTextMatrix(nRows, nCol++));
			pGW->m_dSR	= pStd->GetDia(m_Grid.GetTextMatrix(nRows, nCol++));
			str = m_Grid.GetTextMatrix(nRows, nCol++);
			pGW->m_dCTC = GetNumberInString(str);
			pGW->m_dF1	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dF2	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dF3	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dF	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_ddc1 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_ddc2 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_ddc3 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dL1	= m_Grid.GetTextMatrixLong(nRows, nCol);
			
			nRows++;	nCol = 1;
			pGW->m_dHR	= pStd->GetDia(m_Grid.GetTextMatrix(nRows, nCol++));
			pGW->m_dVR	= pStd->GetDia(m_Grid.GetTextMatrix(nRows, nCol++));
			pGW->m_dSR	= pStd->GetDia(m_Grid.GetTextMatrix(nRows, nCol++));
			str = m_Grid.GetTextMatrix(nRows, nCol++);
			pGW->m_dCTC = GetNumberInString(str);
			pGW->m_dF1	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dF2	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dF3	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dF	= m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_ddc1 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_ddc2 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_ddc3 = m_Grid.GetTextMatrixLong(nRows, nCol++);
			pGW->m_dL1	= m_Grid.GetTextMatrixLong(nRows, nCol++);
		}
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputRebarOthers::SetDataSaveMonument()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	long nCol=0;
	pBri->m_dMonuR_DiaHor = pStd->GetDia(m_Grid.GetTextMatrix(1, nCol++));
	pBri->m_dMonuR_DiaVer = pStd->GetDia(m_Grid.GetTextMatrix(1, nCol++));
	pBri->m_dMonuR_H = m_Grid.GetTextMatrixDouble(1, nCol++);
	pBri->m_dMonuR_L = m_Grid.GetTextMatrixDouble(1, nCol++);
	pBri->m_nMonuR_nA = m_Grid.GetTextMatrixInt(1, nCol++);
	pBri->m_dMonuR_A  = m_Grid.GetTextMatrixDouble(1, nCol++);
	pBri->m_dMonuR_A1 = m_Grid.GetTextMatrixDouble(1, nCol++);
	pBri->m_nMonuR_nB = m_Grid.GetTextMatrixInt(1, nCol++);
	pBri->m_dMonuR_B  = m_Grid.GetTextMatrixDouble(1, nCol++);
	pBri->m_nMonuR_nC = m_Grid.GetTextMatrixInt(1, nCol++);
	pBri->m_dMonuR_C  = m_Grid.GetTextMatrixDouble(1, nCol++);
	pBri->m_dMonuR_C1 = m_Grid.GetTextMatrixDouble(1, nCol++);
	pBri->m_dMonuR_C2 = m_Grid.GetTextMatrixDouble(1, nCol++);
	pBri->m_dMonuR_C3 = m_Grid.GetTextMatrixDouble(1, nCol++);
	pBri->m_nMonuR_nD = m_Grid.GetTextMatrixInt(1, nCol++);
	pBri->m_dMonuR_D  = m_Grid.GetTextMatrixDouble(1, nCol++);
	pBri->m_dMonuR_D1 = m_Grid.GetTextMatrixDouble(1, nCol++);
	pBri->m_dMonuR_D2 = m_Grid.GetTextMatrixDouble(1, nCol++);
	pBri->m_dMonuR_D3 = m_Grid.GetTextMatrixDouble(1, nCol++);

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputRebarOthers::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	CCellID cell = m_Grid.GetFocusCell();
	if(cell.row == -1 || cell.col == -1)
		return;

	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	long nRow = pGridView->iRow;

	SetDataSave();
	SetDataInit();

	CString str = m_Grid.GetTextMatrix(nRow, 0);
	if(str.Find(_T("중앙부"))>0)
		DrawInputDomyunView(TRUE, TRUE);
	else
		DrawInputDomyunView();

	SetDataInit();
	SetControl();
}

void CInputRebarOthers::OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	long nRow = pGridView->iRow;
	
	SetFckAndFy();

	CString str = m_Grid.GetTextMatrix(nRow, 0);
	if(str.Find(_T("중앙부"))>0)
		DrawInputDomyunView(TRUE, TRUE);
	else
		DrawInputDomyunView();

	UpdateData(FALSE);
}

void CInputRebarOthers::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	
	long nRow = pGridView->iRow;
	long nCol = pGridView->iColumn;
	
	CString str = m_Grid.GetTextMatrix(0, nCol);

	CString strTab = GetNameOfTab();
	if(strTab !=_T("교명주 받침"))
	{
		CString sEA(_T(""));
		CString strDan(_T(""));
		strDan = m_Grid.GetTextMatrix(nRow, 0);
		long nHDan = (long)GetNumberInString(strDan)-1;
		if(nHDan<0) nHDan=0;	// 방음벽일때
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan);
		if(!pGW) return;

		double ddc1 = pGW->m_ddc1;
		double ddc3 = pGW->m_ddc3;
		double dE   = pGW->m_dE;
		double dE1  = pGW->m_dF1;
		double dE2  = pGW->m_dF2;
		double dE3  = pGW->m_dF3;
		double dE4  = pGW->m_dF;
		double dEH  = pGW->m_dEH;
		double dFH  = pGW->m_dFH;
		double dH1  = pGW->m_dH1;
		double dH3  = pGW->m_dH3;
		double dW1  = pGW->m_dW1;
		double dW5  = pGW->m_dW5;

		if(str==_T("EA") || str==_T("VEA") || str==_T("HEA") || 
			str==_T("E1") || str==_T("E2") || str==_T("E3") || str==_T("E4"))
		{
			if(nHDan>-1)
			{
				if(str==_T("EA"))
				{
					if(!pGW->IsTypeHDanRailGuardFence())
						sEA.Format("EA@%.0f=%.0f", dE4, dE*dE4);
					else
						sEA.Format("EA@%.0f=%.0f", dE2, dE*dE2);
				}
				else if(str==_T("VEA"))
					sEA.Format("VEA@%.0f=%.0f", dE4, dE*dE4);
				else if(str==_T("HEA"))
					sEA.Format("HEA@%.0f=%.0f", dFH, dEH*dFH);
				else if(str==_T("E1"))
					sEA.Format(_T("E1@%.0f=%.0f"), (dH3+ddc3-ddc1)/dE1, dH3+ddc3-ddc1);
				else if(str==_T("E2"))
					sEA.Format(_T("E2@%.0f=%.0f"), (dH1-ddc3)/dE2, dH1-ddc3);
				else if(str==_T("E3"))
					pGW->IsLeftGuardFence() ? sEA.Format(_T("E3@%.0f=%.0f"), dW5/dE3, dW5) 
											: sEA.Format(_T("E3@%.0f=%.0f"), dW1/dE3, dW1);
				else if(str==_T("E4"))
					sEA.Format(_T("E4@%.0f=%.0f"), (dH1-ddc3)/dE4, dH1-ddc3);
			}
			str = sEA;
		}
	}
	
	// 도움말
	CString strCol = m_Grid.GetTextMatrix(0,nCol);
	CString strComment = GetComment(strCol);
	m_pIn->SetComment(strComment);
	m_pView->GetDomyun()->STMakeCursor(str);
}

CString CInputRebarOthers::GetComment(CString str)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	
	CCellID cID = m_Grid.GetFocusCell();
	CString strGugan = m_Grid.GetTextMatrix(cID.row, cID.col); 
	long nGugan = (long)GetNumberInString(strGugan);
	CString strCGS  = _T("kgf/cm²");
	CString strUnit = pStd->m_nTypeUnit==0 ? strCGS : pStd->GetStringUnitType(strCGS);
	CString strTab = GetNameOfTab();
	CString strComment = _T("");

	if(str=="HR")				strComment = _T("수평 철근 직경");
	if(str=="VR")				strComment = _T("수직 철근 직경");
	if(str=="SR")				strComment = _T("전단 철근 직경");
	if(str=="EA")				strComment = _T("수평 철근 구간");
	if(str=="S")				strComment = _T("수평 철근 구간 간격");
	if(str=="주철근 간격")		strComment = _T("주철근 간격 (CTC)");
	if(str=="전단철근 묶음수")	strComment = _T("전단 철근이 묶는 주철근의 개수와 간격(CTC)");
	if(str=="전단철근 구간")	strComment.Format(_T("전단 철근이 수평철근 %d 구간당 하나씩 위치합니다."), nGugan);
	if(str=="DC1")				strComment = _T("상단 피복");
	if(str=="DC2")				strComment = _T("좌측 피복");
	if(str=="DC3")				strComment = _T("우측 피복");
	if(str=="E1")				
		strTab.Find(_T("통신구")) < 0 ? strComment = _T("측면 상단쪽 수평 철근 구간") : strComment = _T("측면 수평 철근 구간");
	if(str=="E2")
		strTab.Find(_T("통신구")) < 0 ? strComment = _T("측면 하단쪽 수평 철근 구간") : strComment = _T("HOLE 상부쪽 철근 구간");
	if(str=="E3")				strComment = _T("HOLE 상부쪽 수평 철근 구간");
	if(str=="E4")				strComment = _T("측면 수평 철근 구간");
	if(str=="VEA")				strComment = _T("측면 수평 철근 구간");
	if(str=="HEA")				strComment = _T("하단 수평 철근 구간");
	if(   str=="nA"
	   || str=="nB"
	   || str=="nC"
	   || str=="nD" )			strComment = _T("구간수");
	if(   str=="A"
	   || str=="B"
	   || str=="C"
	   || str=="D"  )			strComment = _T("구간 간격");
	if(   str=="C1"
	   || str=="D1" )			strComment = _T("피복");

	return strComment;
}

void CInputRebarOthers::OnSelchangeComboGWGugan()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	
	m_nGWGugan = m_cbGWGugan.GetCurSel();
	long nGugan = GetGugan();
	CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nGugan);
	if(pGW)
	{
		m_cbUpperGugan.SetCurSel(pGW->m_nUGugan-1);
		m_cbSideGugan.SetCurSel(pGW->m_nSGugan-1);
	}

	UpdateData();
	SetGridTitleSoundProof();
	SetDataInit();
	SetControl();
	UpdateData(FALSE);
	SetDataSave();
	
	CCellID cID = m_Grid.GetFocusCell();
	CString str = m_Grid.GetTextMatrix(cID.row, 0);
	if(str.Find(_T("중앙부"))>0)
		DrawInputDomyunView(TRUE, TRUE);
	else
		DrawInputDomyunView();
}

void CInputRebarOthers::OnSelchangeComboUpperGugan()
{
	UpdateData();
	SetGridTitleSoundProof();
	SetDataInit();
	SetControl();
	UpdateData(FALSE);
	SetDataSave();

	CCellID cID = m_Grid.GetFocusCell();
	CString str = m_Grid.GetTextMatrix(cID.row, 0);
	if(str.Find(_T("중앙부"))>0)
		DrawInputDomyunView(TRUE, TRUE);
	else
		DrawInputDomyunView();
}

void CInputRebarOthers::OnSelchangeComboSideGugan()
{
	UpdateData();
	SetGridTitleSoundProof();
	SetDataInit();
	SetControl();
	UpdateData(FALSE);
	SetDataSave();

	CCellID cID = m_Grid.GetFocusCell();
	CString str = m_Grid.GetTextMatrix(cID.row, 0);
	if(str.Find(_T("중앙부"))>0)
		DrawInputDomyunView(TRUE, TRUE);
	else
		DrawInputDomyunView();
}

void CInputRebarOthers::SetFocusCell()
{
	CString strTab = GetNameOfTab();

	long nRows = m_Grid.GetRows();
	long i=0; for(i=1; i<nRows; i++)
	{
		long nHeightRow = m_Grid.GetRowHeight(i);
		if(nHeightRow>0)
		{
			strTab = GetNameOfTab();
			if(strTab!=_T("교명주 받침") )
				m_Grid.SetFocusCell(i, 1);
			else
				m_Grid.SetFocusCell(i, 0);
			break;
		}
	}

	//Fck Fy Agg 설정



}

// long CInputRebarOthers::GetItemNo(CString strItem)
// {
// 	if(strItem==_T("방호벽"))
// 		return HDAN_TYPE_ROAD_GUARDFENCE;
// 	else if(strItem==_T("중분대"))
// 		return HDAN_TYPE_ROAD_CENTERGUARD;
// 	else if(strItem==_T("중분대1/2"))
// 		return HDAN_TYPE_ROAD_CENTERGUARDHALF;
// 	else if(strItem==_T("방음벽 기초"))
// 		return HDAN_TYPE_ROAD_SOUNDPROOF;
// 	else if(strItem==_T("흙막이벽"))
// 		return HDAN_TYPE_ROAD_RETAININGWALL;
// 	else if(strItem==_T("난간기초"))
// 		return HDAN_TYPE_ROAD_HANDRAIL;
// 	else if(strItem==_T("연석+보도"))
// 		return HDAN_TYPE_RAIL_FOOTWAYBLOCK;
// 	else if(strItem==_T("통신구"))
// 		return HDAN_TYPE_RAIL_HOLE;
// 	else if(strItem==_T("통신구+보도"))
// 		return HDAN_TYPE_RAIL_HOLEFOOTWAY;
// 	else if(strItem==_T("자갈막이(철근)"))
// 		return HDAN_TYPE_RAIL_DETECTGRAVEL;
// 	else
// 		return 9;
// }

// 탭 이름 가져오기
CString CInputRebarOthers::GetNameOfTab()
{
	TCITEM tcItem;
	char name[256];
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = name;
	tcItem.cchTextMax = sizeof(name);
	m_Tab.GetItem(m_Tab.GetCurSel(), &tcItem);
	CString str = tcItem.pszText;
	
	return str;
}

// n번째 탭의 이름 가져오기
CString CInputRebarOthers::GetNameOfTab(long nTab)
{
	TCITEM tcItem;
	char name[256];
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = name;
	tcItem.cchTextMax = sizeof(name);
	m_Tab.GetItem(nTab, &tcItem);
	CString str = tcItem.pszText;
	return str;
}
void CInputRebarOthers::OnCheckPosition() 
{
	UpdateData();

	CCellID cID = m_Grid.GetFocusCell();
	CString str = m_Grid.GetTextMatrix(cID.row, 0);
	if(str.Find(_T("중앙부"))>0)
		DrawInputDomyunView(TRUE, TRUE);
	else
		DrawInputDomyunView();
}

// 문자열에서 숫자만 분리
double CInputRebarOthers::GetNumberInString(CString str) 
{
	CString strNumber(_T(""));
	long i = 0; for(i = 0; i<str.GetLength(); i++)
	{
		char ch = str.GetAt(i);
		if(ch >= '0' && ch <= '9') 
			strNumber += ch;
	}
	double dNumber = atof(strNumber);

	return dNumber;
}

void CInputRebarOthers::SetGuganCombo(BOOL bShow)
{		
	m_cbGWGugan.ShowWindow(bShow);
	m_cbUpperGugan.ShowWindow(bShow);
	m_cbSideGugan.ShowWindow(bShow);
	m_sGWGugan.ShowWindow(bShow);
	m_sUpperGugan.ShowWindow(bShow);
	m_sSideGugan.ShowWindow(bShow);
}

long CInputRebarOthers::GetGugan()
{
	if(m_cbGWGugan.GetCount() <= m_nGWGugan)
	{
		m_nGWGugan	= m_cbGWGugan.GetCount()-1;
	}

	CString strCombo(_T(""));
	long nCombo = m_nGWGugan;	//m_cbGWGugan.GetCurSel();
	
		
	m_cbGWGugan.GetLBText(nCombo, strCombo);
	long nGugan = (long)GetNumberInString(strCombo) - 1;

	return nGugan;
}

void CInputRebarOthers::SetDataDefaultGuganCombo()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	
	m_cbGWGugan.ResetContent();
	long nQtyHDan = pBri->GetQtyHDan();

	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadSoundProof() || pGW->m_bSoundProof)			
		{
			CString str(_T(""));
			str.Format(_T("%d 구간"), i+1);
			m_cbGWGugan.AddString(str);
		}	
	}

	m_cbGWGugan.SetCurSel(0);
	long nGugan = GetGugan();
	CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nGugan);
	if(pGW)
	{
		m_cbUpperGugan.SetCurSel(pGW->m_nUGugan-1);
		m_cbSideGugan.SetCurSel(pGW->m_nSGugan-1);
	}
	UpdateData();
}

void CInputRebarOthers::OnChangeEditFy() 
{

}

void CInputRebarOthers::OnChangeEditFck() 
{

}

CString CInputRebarOthers::GetStringDia(double dDia)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CString strCGS = _T("kgf/cm²");
	CString strSI  = pStd->GetStringUnitType(strCGS);

	double dFy = pStd->GetValueUnitType(m_dFy, strSI, strCGS);
	CString sFy = GetCharRebarMark(dFy);
	if(dDia==0) return _T("");
	
	return sFy+Comma(dDia);
}

long CInputRebarOthers::GetHDan()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return -1;

	long nRow = m_Grid.GetFocusCell().row;
	if(nRow<0)		
	{
		nRow = 1;
		m_Grid.SetFocusCell(nRow, 1);
	}

	CString str = m_Grid.GetTextMatrix(nRow,0);
	CString strTab = GetNameOfTab();
	long nHDan = (long)GetNumberInString(str) - 1;

	if(nHDan < 0 || strTab==_T("교명주 받침"))
		nHDan = 0;		// 구간이 그리드에 없거나 교명주인 경우
	if(strTab==_T("방음벽 기초") || strTab==_T("방호벽(방음벽기초타입)"))
		nHDan = GetGugan();
	else if(strTab == _T("무근 콘크리트"))
	{
		for(long ix=0; ix<pBri->GetQtyHDan(); ix++)
		{
			CGuardWallRC* pGW = pBri->GetGuardWallByHDan(ix);
			if(!pGW) continue;
			if(pGW->IsTypeHDanNonRebarConcrete())
			{
				nHDan = ix;
				break;
			}
		}
	}

	return nHDan;
}

void CInputRebarOthers::SetFckAndFy()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);

	long nHDan = GetHDan();
	CString sHDan(_T(""));
	sHDan.Format(_T(" %d구간"), nHDan+1);
	CString strTab = GetNameOfTab();

	CString strCGS = _T("kgf/cm²");
	CString strSI  = pStd->GetStringUnitType(strCGS);
	if(strTab==_T("교명주 받침"))
	{
		GetDlgItem(IDC_STATIC_SETTING)->SetWindowText(strTab);
		m_dFck = pStd->m_nTypeUnit==0 ? pBri->m_dMonuFck : pStd->GetValueUnitType(pBri->m_dMonuFck, strCGS, strSI);
		m_dFy  = pStd->m_nTypeUnit==0 ? pBri->m_dMonuR_Fy : pStd->GetValueUnitType(	pBri->m_dMonuR_Fy, strCGS, strSI);
	}
	else
	{
		if(strTab==_T("방호벽(방음벽기초타입)"))	strTab=_T("방호벽");
		GetDlgItem(IDC_STATIC_SETTING)->SetWindowText(strTab+sHDan);
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan);
		if(pGW)
		{
			m_dFck = pStd->m_nTypeUnit==0 ? pGW->m_dFck : pStd->GetValueUnitType(pGW->m_dFck, strCGS, strSI);
			m_dFy  = pStd->m_nTypeUnit==0 ? pGW->m_dFy : pStd->GetValueUnitType(pGW->m_dFy, strCGS, strSI);
			m_dAggregate	= pGW->m_dAggregate;
		}
	}

	UpdateData(FALSE);
}

void CInputRebarOthers::OnCheckGwhalf() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);

	long nHDan = GetHDan();
	CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan);

	UpdateData(TRUE);
	pGW->m_bGWhalf = m_bGWhalf;

	CDPoint vAng = CDPoint(0, 1);
	double sta = pBri->GetStationOnJijum(0,0);
	CJijum* pJL = pBri->GetJijum(0);
	if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pJL->m_dLengthUgan[1];
	
	CRcBridgeRebar *pBriRebar = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	pBriRebar->m_Util.SetDataDefaultRebarGuardWall(pGW, sta, vAng);
	
	SetGridTitle();
	SetDataInit();
	SetControl();
	SetDataSave();
	DrawInputDomyunView();
}

void CInputRebarOthers::OnKillfocusEditFck() 
{
	// TODO: Add your control notification handler code here
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	UpdateData();

	long nHDan = GetHDan();
	CString strTab = GetNameOfTab();
	CString strCGS = _T("kgf/cm²");
	CString strSI  = pStd->GetStringUnitType(strCGS);
	if(strTab==_T("교명주 받침"))
		pBri->m_dMonuFck = pStd->m_nTypeUnit==0 ? m_dFck : pStd->GetValueUnitType(m_dFck, strSI, strCGS);
	else
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan);
		if(pGW)
		{
			double dFck = pStd->m_nTypeUnit==0 ? m_dFck : pStd->GetValueUnitType(m_dFck, strSI, strCGS);

			if(pGW->IsTypeHDanNonRebarConcrete())
			{
				for (long nIdx =0; nIdx < pBri->GetQtyHDan(); nIdx++)
				{
					CGuardWallRC* pGWTmp = pBri->GetGuardWallByHDan(nIdx);
					if(!pGWTmp) continue;
					if(!pGWTmp->IsTypeHDanNonRebarConcrete()) continue;

					pGWTmp->m_dFck = dFck;
				}
			}
			else
			{
				pGW->m_dFck = dFck;
			}
		}
	}

	SetDataInit();	
}

void CInputRebarOthers::OnKillfocusEditFy() 
{
	// TODO: Add your control notification handler code here
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	
	UpdateData();

	long nHDan = GetHDan();
	CString strTab = GetNameOfTab();
	CString strCGS = _T("kgf/cm²");
	CString strSI  = pStd->GetStringUnitType(strCGS);
	if(strTab==_T("교명주 받침"))
		pBri->m_dMonuR_Fy = pStd->m_nTypeUnit==0 ? m_dFy : pStd->GetValueUnitType(m_dFy, strSI, strCGS);
	else
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan);
		if(pGW) 
			pGW->m_dFy = pStd->m_nTypeUnit==0 ? m_dFy : pStd->GetValueUnitType(m_dFy, strSI, strCGS);
	}

	SetDataInit();	
}

// 무근 콘크리트
void CInputRebarOthers::SetGridTitleNonRebarConcrete()
{

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	
	//최소한의 기본값(비우기)
	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowResize(FALSE);
	m_Grid.SetRowCount(0);
	m_Grid.SetColumnCount(0);
	return;

	/*
	if(pBri->GetQtyHDanNonRebarConcrete() == 0) 
	return;

	long nRows = pBri->GetQtyHDanNonRebarConcrete() + 1;
	long nCols = 4;

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowResize(FALSE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(40);
	m_Grid.SetColumnWidth(0, 60);
	m_Grid.SetColumnWidth(1, 50);
	m_Grid.SetColumnWidth(2, 50);
	m_Grid.SetColumnWidth(3, 100);


	CString str	= _T("");
	long row(0), col(0);
	m_Grid.SetTextMatrix(row, 0, "구분");
	m_Grid.SetTextMatrix(row, 1, "Fck");
	m_Grid.SetTextMatrix(row, 2, "Fy");
	m_Grid.SetTextMatrix(row, 3, "최대골재치수");
	row++;

	long nQtyHDan = pBri->GetQtyHDan();
	long i=0; for(i=0; i<nQtyHDan; i++)
	{
	CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i);
	if(!pGW) continue;
	if(pGW->IsTypeHDanNonRebarConcrete())
	{
	CString str(_T(""));
	col =0; 

	str.Format(_T("%d 구간"), i+1);
	m_Grid.SetCellType(row, col, CT_EDIT);
	m_Grid.SetTextMatrix(row, col++, str);

	str.Format(_T("%.0f"), pGW->m_dFck);
	m_Grid.SetCellType(row, col, CT_EDIT);
	m_Grid.SetTextMatrix(row, col++, str);

	str.Format(_T("%.0f"), pGW->m_dFy);
	m_Grid.SetCellType(row, col, CT_EDIT);
	m_Grid.SetTextMatrix(row, col++, str);

	str.Format(_T("%.0f"), pGW->m_dAggregate);
	m_Grid.SetCellType(row, col, CT_EDIT);
	m_Grid.SetTextMatrix(row++, col, str);

	}	
	}
	*/

}

void CInputRebarOthers::SetDataInitNonRebarConcrete()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	
	if(pBri->GetQtyHDanNonRebarConcrete() == 0) return;
		
	CString strCGS = _T("kgf/cm²");
	CString strSI  = pStd->GetStringUnitType(strCGS);

	long nQtyHDan = pBri->GetQtyHDan();
	for(long ix=0; ix<nQtyHDan; ix++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(ix);
		if(!pGW) continue;
		if(pGW->IsTypeHDanNonRebarConcrete())
		{
			m_dFck = pStd->m_nTypeUnit==0 ? pGW->m_dFck : pStd->GetValueUnitType(pGW->m_dFck, strCGS, strSI);
			m_dFy = pStd->m_nTypeUnit==0 ? pGW->m_dFy : pStd->GetValueUnitType(pGW->m_dFy, strCGS, strSI);
			m_dAggregate = pGW->m_dAggregate;

			break;		
		}	
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputRebarOthers::SetDataSaveNonRebarConcrete()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	if(pBri->GetQtyHDanNonRebarConcrete() == 0) return;

	CString str	= _T("");
	long row(0);
	row = 1;

	long nQtyHDan = pBri->GetQtyHDan();
	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;
		if(pGW->IsTypeHDanNonRebarConcrete())
		{
			CString strCGS	= _T("kgf/cm²");
			CString strSI		= pStd->GetStringUnitType(strCGS);
			double dFck		= m_dFck; 
			double dFy		= m_dFy; 
			double dAgg	= m_dAggregate; 

			pGW->m_dFck = pStd->m_nTypeUnit==0 ? dFck : pStd->GetValueUnitType(dFck, strSI, strCGS);
			pGW->m_dFy = pStd->m_nTypeUnit==0 ? dFy : pStd->GetValueUnitType(dFy, strSI, strCGS);
			pGW->m_dAggregate	= dAgg;
		}	
	}
}

CDRect CInputRebarOthers::DrawCrossGuardWall(CDomyun *pDom, long nHDan)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);
	
	CDomyun Dom(pDom);
	DBStd.m_pDrawCrossSection->DrawCrossGuardWall(&Dom, nHDan);

	Dom.SetCalcExtRect();
	CDRect rtMain = Dom.GetExtRect();
	
	*pDom << Dom;

	return rtMain;
}

void CInputRebarOthers::OnKillfocusEditAgg() 
{
	// TODO: Add your control notification handler code here
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	
	UpdateData();

	long nHDan = GetHDan();
	CString strTab = GetNameOfTab();
	if(strTab==_T("교명주 받침"))
	{
		pBri->m_dMonuAggregate = m_dAggregate;
	}
	else
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan);
		if(pGW) 
			pGW->m_dAggregate	= m_dAggregate;
	}

	SetDataInit();	
}

void CInputRebarOthers::OnChangeEditAgg() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CInputDialogBase::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CInputRebarOthers::SetDlgFckFyAg()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	if(!pStd) return;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	CString strTab = GetNameOfTab();
	long nHdan = GetHDan();

	CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHdan);
	if(!pGW) return;

	CString strCGS = _T("kgf/cm²");
	CString strSI  = pStd->GetStringUnitType(strCGS);
	
	if(strTab==_T("교명주 받침"))
	{
		m_dFck = pStd->m_nTypeUnit==0 ? pBri->m_dMonuFck : pStd->GetValueUnitType( pBri->m_dMonuFck , strCGS, strSI);
		m_dFy = pStd->m_nTypeUnit==0 ? pBri->m_dMonuR_Fy : pStd->GetValueUnitType(pBri->m_dMonuR_Fy, strCGS, strSI);		
		m_dAggregate = pBri->m_dMonuAggregate;
	}
	else
	{
		m_dFck = pStd->m_nTypeUnit==0 ? pGW->m_dFck : pStd->GetValueUnitType(pGW->m_dFck, strCGS, strSI);
		m_dFy = pStd->m_nTypeUnit==0 ? pGW->m_dFy : pStd->GetValueUnitType(pGW->m_dFy, strCGS, strSI);
		m_dAggregate = pGW->m_dAggregate;
	}	
	

	CString sFck = _T("0");
	CString sFy =  _T("0");
	CString sAg = _T("0");
	sFck.Format(_T("%0.f"), m_dFck);
	sFy.Format(_T("%0.f"), m_dFy);
	sAg.Format(_T("%0.f"), m_dAggregate);

	GetDlgItem(IDC_EDIT_FCK)->SetWindowText(sFck);
	GetDlgItem(IDC_EDIT_FY)->SetWindowText(sFy);
	GetDlgItem(IDC_EDIT_AGG)->SetWindowText(sAg);
}