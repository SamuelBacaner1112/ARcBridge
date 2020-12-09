// InputBaseGroundInfo.cpp : implementation file
//

#include "stdafx.h"
#include "ARcBridgeInput.h"
#include "../ARcBridgeDBDraw/ARcBridgeDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputBaseGroundInfo dialog


CInputBaseGroundInfo::CInputBaseGroundInfo(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputBaseGroundInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputBaseGroundInfo)
	m_szEL = _T("");
	m_szStation = _T("");
	//}}AFX_DATA_INIT

	m_dVScaleDraw = 5.00;
}


void CInputBaseGroundInfo::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputBaseGroundInfo)
	DDX_Text(pDX, IDC_EDIT_BORING_NAME, m_strBoringName);
	DDX_Control(pDX, IDC_COMBO_GROUNDINFONUM, m_cbGroundInfoNum);
	DDX_Control(pDX, IDC_COMBO_BORING, m_cbBoringNo);
	DDX_Control(pDX, IDC_TAB, m_ctlTab);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Text(pDX, IDC_EDIT_BORING_EL, m_szEL);
	DDX_Text(pDX, IDC_EDIT_BORING_STATION, m_szStation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputBaseGroundInfo, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputBaseGroundInfo)
	ON_CBN_SELCHANGE(IDC_COMBO_BORING, OnSelchangeComboBoring)
	ON_CBN_SELCHANGE(IDC_COMBO_GROUNDINFONUM, OnSelchangeComboGroundinfonum)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_EN_KILLFOCUS(IDC_EDIT_BORING_NAME, OnKillfocusEditBoringName)
	ON_EN_KILLFOCUS(IDC_EDIT_BORING_EL, OnKillfocusEditBoringEL)
	ON_EN_KILLFOCUS(IDC_EDIT_BORING_STATION, OnKillfocusEditBoringStation)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_PREV, OnButtonPrev)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputBaseGroundInfo message handlers

BOOL CInputBaseGroundInfo::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		UpdateData();
		if(m_Grid.GetFocusCell().row == m_Grid.GetCols()-1)
			pMsg->wParam = VK_RIGHT;
		else
			pMsg->wParam = VK_TAB;
	}
	
	///////////////////////////Grid내 콤보박스 초기화///////////////////////////////////
	if(pMsg->hwnd == m_Grid.GetSafeHwnd() && pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		CConcSymbol sym;

		CCellID next = m_Grid.GetFocusCell();
		if(next.col==1 || next.col==2 || next.col==11)
		{
			CStringArray strArr;
			
			if(next.col==1)
			{
				// AAB-6151
				long nCountType = m_bDesignLsd ? 3 : 2;

				for(long i=0; i<nCountType; i++)
					strArr.Add(sym.m_strSoilType[i]);
			}
			if(next.col==2)
			{
				long i=0; for(i=0; i<SOILCOUNT; i++)
					strArr.Add(sym.m_strSoilName[i]);
			}
			if(next.col==11)		// LSD
			{
				strArr.Add(hggettext("1.5-(7.7x10^-3√z)"));
				strArr.Add(hggettext("N60/15(1.5-7.7x10^-3√z)"));
// 				strCombo = strArr.GetAt(0) + _T("!") + strArr.GetAt(1);
			}

			m_Grid.SetComboString(strArr);
		}
	}

	return CInputDialogBase::PreTranslateMessage(pMsg);
}

void CInputBaseGroundInfo::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	SetDataSave();
	SetDataInit();
	//DrawInputDomyunView(FALSE);
	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);
}

void CInputBaseGroundInfo::OnPreInitDialog()
{
	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_TAB, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	SetResize(IDC_STATIC_BORING, SZ_TOP_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_BORING_NAME, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_BORING_NAME, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_BORING_EL, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_EL, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_EDIT_BORING_STATION, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_BORING_STATION, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_COMBO_BORING, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_BORING_NUM, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_COMBO_GROUNDINFONUM, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC_GROUNDINFONUM, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetHyperHelp(m_pIn->GetManualPath());

	InitGroundInfoComboCtrl();
	InitTabCtrl();
	InitComboBoring();

	m_dlgEValueDlg.m_nTypeUnit = m_pIn->m_pARcBridgeDataStd->m_nTypeUnit;
	m_dScaleDefault	= 100;

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	m_dlgEValueDlg.SetLSD(pStd->IsLsdDesign());
	m_dlgUValueDlg.SetLSD(pStd->IsLsdDesign());
}

void CInputBaseGroundInfo::InitGroundInfoComboCtrl()
{	
	CString	str;
	
	// ARCBRIDGE-2534
	for(long i=0; i<MAX_JIBAN_COUNT+1; i++)
	{
		str.Format(_T("%d 개"), i + 1);
		m_cbGroundInfoNum.AddString(str);
	}
}

void CInputBaseGroundInfo::DrawInputDomyunView(BOOL bZoomAll)
{
	// 기본입력시는 평면을 매번 구해주도록 셋팅해놓자.
	m_pIn->SettingNeedPlane();

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(m_pIn->GetScaleInputDialog());
	
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);

	CDomyun DomAll(pDom);	
	CDomyun DomCurr(pDom);	
	DrawVertGroundEL(&DomAll);	//종단지반고
	DrawLineVertical(&DomAll);
	*pDom << DomAll;
	
	//보링공 draw(tab별)
	CLineInfo* pLine = pStd->GetLineApp();	//종단지반고
	long nSizeBoring = pStd->m_pArrBoringEx.GetSize();
	long n=0; for(n=0; n<nSizeBoring; n++)
	{	
		CBoringDataEx *pBoringEx = pStd->m_pArrBoringEx.GetAt(n);
		if(pBoringEx->m_pArrBoring.GetSize()>0)
		{
			CConcSymbol SymDom(m_pIn->m_pARoadOptionStd);
			SymDom.DrawBoringSymbol(&DomAll, &pBoringEx->m_pArrBoring, pBoringEx->m_EL, frM(pBoringEx->m_dStation), m_dVScaleDraw, TRUE, 0, TRUE, pBoringEx->m_sNameBoring);
			DomAll.Move(frM(pBoringEx->m_dStation),frM(pBoringEx->m_EL)*m_dVScaleDraw);
			*pDom << DomAll;
		}
	}
	pDom->SetCalcExtRect();
	CDRect rectAll = pDom->GetExtRect();

	// 현재 입력중인 지반정보
	long nTab = m_ctlTab.GetCurSel();
	if(nTab > -1 && pStd->m_pArrBoringEx.GetSize() > nTab)
	{
		CBoringDataEx *pBoringEx = pStd->m_pArrBoringEx.GetAt(nTab);
		if(pBoringEx->m_pArrBoring.GetSize()>0)
		{
			CConcSymbol SymDom(m_pIn->m_pARoadOptionStd);
			SymDom.DrawBoringSymbol(&DomCurr, &pBoringEx->m_pArrBoring, pBoringEx->m_EL, frM(pBoringEx->m_dStation), 1, TRUE, 0, TRUE);
			DomCurr.Move(frM(pBoringEx->m_dStation),frM(pBoringEx->m_EL));
		}
		CDRect rectCurr;
		DomCurr.SetCalcExtRect();
		rectCurr = DomCurr.GetExtRect();
		DomCurr.SetLineColor(-1);
		DomCurr.Point(rectCurr.left-rectCurr.Width()/30,rectCurr.bottom-rectCurr.Height()/30);
		DomCurr.Point(rectCurr.right+rectCurr.Width()/30,rectCurr.top+rectCurr.Height()/30);

		DomCurr.SetCalcExtRect();
		rectCurr = DomCurr.GetExtRect();

		double dResizeRatio = (rectAll.Width()/rectCurr.Width())/5.;
		DomCurr.RedrawByScale(dResizeRatio);
		DomCurr.SetCalcExtRect();
		rectCurr = DomCurr.GetExtRect();
		DomCurr.Move(rectAll.left-rectCurr.right,
					 (rectAll.top+rectAll.bottom)/2-(rectCurr.top+rectCurr.bottom)/2);
	}
	*pDom << DomCurr;

	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

void CInputBaseGroundInfo::SetGridTitle()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
//	HGBOOL bLSD = pStd->IsLsdDesign();

	int nTab = m_ctlTab.GetCurSel();
	if(nTab<0) 
	{
		m_Grid.SetRowCount(1);
		m_Grid.SetColumnCount(1);
		m_Grid.SetFixedRowCount();
		m_Grid.SetFixedColumnCount();
		m_Grid.SetColumnWidth(0,500);
		m_Grid.SetRowHeight(0,50);
		m_Grid.SetTextMatrix(0,0,"입력된 자료가 없습니다.");
		m_Grid.SetRedraw(TRUE,TRUE);
		return;
	}

	CBoringDataEx *pBoring = pStd->m_pArrBoringEx.GetAt(nTab);

	int nRows = pBoring->m_pArrBoring.GetSize() + 1;
	int nCols = m_bDesignLsd==TRUE ? 16 : 8;
	HGINT32 nCol = m_bDesignLsd==TRUE ? 7 : 6;

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowCount(nRows);
	m_Grid.SetColumnCount(nCols);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(55);
	m_Grid.SetColumnWidth(1,100);
	m_Grid.SetColumnWidth(2,170);
	m_Grid.SetColumnWidth(nCol,100);
	m_Grid.SetColumnWidth(nCol+1,100);
	if(m_bDesignLsd)
	{
		// AAB-6151
		// β, m, s 는 지반정보입력에서 삭제됩니다. 
		// 변수는 유지하고 입력에서만 숨김....
		m_Grid.SetColumnWidth(11,0);
		m_Grid.SetColumnWidth(12,80);
		m_Grid.SetColumnWidth(13,80);
		m_Grid.SetColumnWidth(14,0);
		m_Grid.SetColumnWidth(15,0);
	}

	m_Grid.SetTextBkColor(CLR_BACK);

	int i=0; for(i=1; i<nRows; i++)
	{
		m_Grid.SetCellType(i, 1, CT_COMBO);
		m_Grid.SetUserComboDialog(i, 2, &m_dlgBoringDlg);
		m_Grid.SetUserComboDialog(i, nCol, &m_dlgEValueDlg);
		m_Grid.SetUserComboDialog(i, nCol+1, &m_dlgUValueDlg);
		m_Grid.SetCellType(i, nCol, CT_USERDIALOGCOMBO_EDIT);
		m_Grid.SetCellType(i, nCol+1, CT_USERDIALOGCOMBO_EDIT);

		if(m_bDesignLsd)
		{
			m_Grid.SetCellType(i, 11, CT_COMBO);
			m_Grid.SetUserComboDialog(i, 13, &m_dlgBoringAlphaEDlg);
			m_Grid.SetUserComboDialog(i, 14, &m_dlgBoringMsDlg);
			m_Grid.SetUserComboDialog(i, 15, &m_dlgBoringMsDlg);
			m_Grid.SetCellType(i, 13, CT_USERDIALOGCOMBO_EDIT);
			m_Grid.SetCellType(i, 14, CT_USERDIALOGCOMBO_EDIT);
			m_Grid.SetCellType(i, 15, CT_USERDIALOGCOMBO_EDIT);
		}
	}

	nCols = 0;
	m_Grid.SetTextMatrix(0, nCols++, _T("구분"));
	m_Grid.SetTextMatrix(0, nCols++, _T("토질 분류"));
	m_Grid.SetTextMatrix(0, nCols++, _T("보링 명칭"));
	m_Grid.SetTextMatrix(0, nCols++, _T("dH"));
	m_Grid.SetTextMatrix(0, nCols++, _T("N"));
	m_Grid.SetTextMatrix(0, nCols++, _T("c"));	// 지반의 점착력
//	m_Grid.SetTextMatrix(0, 6, _T("Cu"));	// 비배수 전단 강도
	if( m_bDesignLsd == TRUE )
	{
		m_Grid.SetTextMatrix(0, nCols++, "Cu");
	}
	m_Grid.SetTextMatrix(0, nCols++, "Eo");		// 탄성계수
	m_Grid.SetTextMatrix(0, nCols++, "μ");		// 포아슨비
	
	if( m_bDesignLsd == TRUE )
	{
		m_Grid.SetTextMatrix(0, nCols++, _T("σv'"));	
		m_Grid.SetTextMatrix(0, nCols++, _T("N60"));	
		m_Grid.SetTextMatrix(0, nCols++, _T("β"));	
		m_Grid.SetTextMatrix(0, nCols++, _T("qu(N/mm²)"));	
		m_Grid.SetTextMatrix(0, nCols++, _T("αE"));	
		m_Grid.SetTextMatrix(0, nCols++, _T("m"));	
		m_Grid.SetTextMatrix(0, nCols, _T("s"));
	}

	CString str = _T("");
	for(i=1; i<nRows; i++)
	{
		str.Format(_T("%d"), i);
		m_Grid.SetTextMatrix(i, 0, str);
	}

	m_Grid.SetRedraw(TRUE,TRUE);
}

// void CInputBaseGroundInfo::ClearBoringInfoData(int nBoring)
// {
// 	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
// 
// 	CBoringDataEx *pBoring = pStd->m_pArrBoring.GetAt(nBoring);
// 
// 	pBoring->m_sNameBoring	= _T("");
// 	pBoring->m_dStation		= 0.0;
// 	pBoring->m_EL			= 0.0;
// 
// 	AhTPADelete(&pBoring->m_pArrBoring,	(CBoringData*)0);
// }

void CInputBaseGroundInfo::SetDataDefaultBoringInfo(int nBoring)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	CBoringDataEx *pBoringEx = pStd->m_pArrBoringEx.GetAt(nBoring);
	
//	HGBOOL bLSD = pStd->IsLsdDesign();
	CString str = _T("");
	str.Format(_T("BH-%d"), nBoring+1);
	pBoringEx->m_sNameBoring = str;
	pBoringEx->m_dStation = 0.0;
	pBoringEx->m_EL = 0.0;

	long nQtyBoring = 1;
	pStd->MakeSoilLayer(nBoring,nQtyBoring);

	int nSize = pBoringEx->m_pArrBoring.GetSize();
	int i=0; for(i=0; i<nSize; i++)
	{
		CBoringData* pBoring = pBoringEx->m_pArrBoring.GetAt(i);
		pBoring->m_nTypeSoil =  0;
		pBoring->m_nNameSoil =  0;
		pBoring->m_nNValue   = 10;
		pBoring->m_dDepth    = 5.0*(i+1);
		pBoring->m_dEo		= 100;
		pBoring->m_RatePoasong	= 0.2;
		// LSD
		pBoring->m_dSigma_V = 0.0;
		pBoring->m_dN60 = 10.0;
		pBoring->m_nTypeBeta = 0;
		pBoring->m_dqu = 100.0; 
		pBoring->m_dAlphaE = 1.0;
		pBoring->m_dm = 2.05;
		pBoring->m_ds = 0.00293;
	}
}

void CInputBaseGroundInfo::SetDataDefault()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	BOOL bFirst = !IsUsedFlag();
	if(!bFirst && !m_bClickDefaultButton) return;

	long nTab = m_ctlTab.GetCurSel();

	if(bFirst)
		pStd->m_pArrBoringEx.Add(new CBoringDataEx);

	SetDataDefaultBoringInfo(nTab);
}

void CInputBaseGroundInfo::SetDataInit()
{
	SetGridTitle();

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
//	HGBOOL bLSD = pStd->IsLsdDesign();

	int nTab = m_ctlTab.GetCurSel();
	if(nTab<0) return;

	// ARCBRIDGE-2534
	m_cbGroundInfoNum.SetCurSel(pStd->m_pArrBoringEx.GetSize() - 1);

	CBoringDataEx *pBoringEx = pStd->m_pArrBoringEx.GetAt(nTab);

	m_strBoringName  = pBoringEx->m_sNameBoring;
	m_szStation.Format("%.3f",pBoringEx->m_dStation);
	m_szEL.Format("%.3f",pBoringEx->m_EL);
	m_cbBoringNo.SetCurSel(pBoringEx->m_pArrBoring.GetSize()-1);

	UpdateData(FALSE);

	CConcSymbol sym;
	CString str = _T("");
	long nRow = 1;
	CString szKgf_Cm2 = pStd->IsSI() ? "kN/m²" : UNIT_CGS_KGF_CM2;
	if(m_bDesignLsd)
		szKgf_Cm2 = pStd->m_szKgf_CM2;

	BOOL bEnable = TRUE;
	for(long i=0; i<pBoringEx->m_pArrBoring.GetSize(); i++)
	{
		CBoringData* pBoring = pBoringEx->m_pArrBoring.GetAt(i);
		long nCol = 1;

 		double Eo = GetValueUnitChange(pBoring->m_dEo,UNIT_CGS_KGF_CM2,szKgf_Cm2);

		str = pBoring->GetStringSoilType();
		m_Grid.SetTextMatrix(nRow, nCol++, str);
		m_Grid.SetTextMatrix(nRow, nCol++, sym.m_strSoilName[pBoring->m_nNameSoil]);
		m_Grid.SetTextMatrix(nRow, nCol++, _T("%.3f"), pBoring->m_dDepth);
		str.Format(_T("%d"), pBoring->m_nNValue);		m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format(_T("%.2f"), GetValueUnitChange(pBoring->m_c ,UNIT_CGS_TONF_M2,pStd->m_szTonf_M2));		
		
		m_Grid.SetTextMatrix(nRow, nCol++, str);
		if( m_bDesignLsd )
		{
			str.Format(_T("%.2f"), GetValueUnitChange(pBoring->m_Cu,UNIT_CGS_TONF_M2,pStd->m_szTonf_M2));		m_Grid.SetTextMatrix(nRow, nCol++, str);
		}
		str.Format(_T("%.0f"), Eo);						m_Grid.SetTextMatrix(nRow, nCol++, str);
		str.Format(_T("%.2f"), pBoring->m_RatePoasong);	m_Grid.SetTextMatrix(nRow, nCol++, str);	
		
		if( m_bDesignLsd )
		{
			// σv' : 사질토,점성토
			bEnable = (pBoring->m_nTypeSoil != 2) ? TRUE : FALSE;
			HGDOUBLE dVal = GetValueUnitChange(pBoring->m_dSigma_V,UNIT_CGS_KGF_CM2,pStd->m_szKgf_CM2);
			str = GetStrDouble(dVal, 2, m_bDesignLsd); m_Grid.SetTextMatrix(nRow, nCol, str);
			if(bEnable)
			{
				m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(nRow, nCol)& ~ GVIS_READONLY);
				m_Grid.SetItemBkColour(nRow, nCol, RGB_WHITE);
			}
			else
			{
				m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(nRow, nCol)| GVIS_READONLY);
				m_Grid.SetItemBkColour(nRow, nCol, CLR_READONLY);
			}
			nCol++;
			// N60 : 사질토
			bEnable = (pBoring->m_nTypeSoil == 0) ? TRUE : FALSE;
			str = GetStrDouble(pBoring->m_dN60, 0, m_bDesignLsd); m_Grid.SetTextMatrix(nRow, nCol, str);
			if(bEnable)
			{
				m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(nRow, nCol)& ~ GVIS_READONLY);
				m_Grid.SetItemBkColour(nRow, nCol, RGB_WHITE);
			}
			else
			{
				m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(nRow, nCol)| GVIS_READONLY);
				m_Grid.SetItemBkColour(nRow, nCol, CLR_READONLY);
			}
			nCol++;
			// β
			str = pBoring->m_nTypeBeta==0 ? ("1.5-(7.7x10^-3√z)") : ("N60/15(1.5-7.7x10^-3√z)");  m_Grid.SetTextMatrix(nRow, nCol++, str);
			// qu : 암반
			bEnable = (pBoring->m_nTypeSoil == 2) ? TRUE : FALSE;
			dVal = GetValueUnitChange(pBoring->m_dqu,UNIT_CGS_KGF_CM2,pStd->m_szKgf_CM2);
			str = GetStrDouble(dVal, 2, m_bDesignLsd); m_Grid.SetTextMatrix(nRow, nCol, str);
			if(bEnable)
			{
				m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(nRow, nCol)& ~ GVIS_READONLY);
				m_Grid.SetItemBkColour(nRow, nCol, RGB_WHITE);
			}
			else
			{
				m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(nRow, nCol)| GVIS_READONLY);
				m_Grid.SetItemBkColour(nRow, nCol, CLR_READONLY);
			}
			nCol++;

			// αE : 암반
			bEnable = (pBoring->m_nTypeSoil == 2) ? TRUE : FALSE;
			str = GetStrDouble(pBoring->m_dAlphaE, 2, m_bDesignLsd);	m_Grid.SetTextMatrix(nRow, nCol, str);
			if(bEnable)
			{
				m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(nRow, nCol)& ~ GVIS_READONLY);
				m_Grid.SetItemBkColour(nRow, nCol, RGB_WHITE);
			}
			else
			{
				m_Grid.SetItemState(nRow, nCol, m_Grid.GetItemState(nRow, nCol)| GVIS_READONLY);
				m_Grid.SetItemBkColour(nRow, nCol, CLR_READONLY);
			}
			nCol++;
			// m
			str = GetStrDouble(pBoring->m_dm, 4, m_bDesignLsd);	m_Grid.SetTextMatrix(nRow, nCol++, str);
			// s
			str = GetStrDouble(pBoring->m_ds, 7, m_bDesignLsd);	m_Grid.SetTextMatrix(nRow, nCol++, str);
		}

		nRow++;
	}

	m_dlgEValueDlg.m_nTypeUnit = m_pIn->m_pARcBridgeDataStd->m_nTypeUnit;
	m_Grid.SetRedraw(TRUE, TRUE);
}

void CInputBaseGroundInfo::SetDataSave()
{
	UpdateData();
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
// 	HGBOOL m_bDesignLsd = pStd->IsLsdDesign();
	int nTab = m_ctlTab.GetCurSel();

	if(nTab<0 || nTab>=pStd->m_pArrBoringEx.GetSize()) return;

	CBoringDataEx *pBoringEx = pStd->m_pArrBoringEx.GetAt(nTab);

	pBoringEx->m_sNameBoring = m_strBoringName;
	pBoringEx->m_dStation = atof(m_szStation);
	pBoringEx->m_EL = atof(m_szEL);

	UpdateData(FALSE);

	CConcSymbol sym;
	CString str = _T("");
	long nRow = 1;
	CString szKgf_Cm2 = pStd->IsSI() ? "kN/m²" : UNIT_CGS_KGF_CM2;
	if(m_bDesignLsd)
		szKgf_Cm2 = pStd->m_szKgf_CM2;

	for(long nB=0; nB<pBoringEx->m_pArrBoring.GetSize(); nB++)
	{
		CBoringData* pBoring = pBoringEx->m_pArrBoring.GetAt(nB);
		long nCol = 1;

		str = m_Grid.GetTextMatrix(nRow, nCol++);
		pBoring->SetStringSoilType(str);
		str = m_Grid.GetTextMatrix(nRow, nCol++);
		for(long nSoil=0; nSoil<SOILCOUNT; nSoil++)
		{
			if(str==sym.m_strSoilName[nSoil]) 
			{
				pBoring->m_nNameSoil = nSoil; 
				break;
			}
		}
		if(nRow == 1)
			pBoring->m_dELStart = pBoringEx->m_EL;
		else
			pBoring->m_dELStart = pBoringEx->m_EL - m_Grid.GetTextMatrixDouble(nRow-1, nCol);

		pBoring->m_dDepth = m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pBoring->m_nNValue = m_Grid.GetTextMatrixInt(nRow, nCol++);
		pBoring->m_c  = GetValueUnitChange(m_Grid.GetTextMatrixDouble(nRow, nCol++),pStd->m_szTonf_M2,UNIT_CGS_TONF_M2);
		if( m_bDesignLsd == TRUE )
			pBoring->m_Cu = GetValueUnitChange(m_Grid.GetTextMatrixDouble(nRow, nCol++),pStd->m_szTonf_M2,UNIT_CGS_TONF_M2);
		double Eo = m_Grid.GetTextMatrixDouble(nRow, nCol++);
		pBoring->m_dEo = GetValueUnitChange(Eo,szKgf_Cm2,UNIT_CGS_KGF_CM2);
		pBoring->m_RatePoasong = m_Grid.GetTextMatrixDouble(nRow, nCol++);

		if( m_bDesignLsd == TRUE )
		{
			pBoring->m_dSigma_V = GetValueUnitChange(m_Grid.GetTextMatrixDouble(nRow, nCol++),pStd->m_szKgf_CM2, UNIT_CGS_KGF_CM2);
			pBoring->m_dN60 = m_Grid.GetTextMatrixDouble(nRow, nCol++);
			str = m_Grid.GetTextMatrix(nRow, nCol++);
			if ( str == ("1.5-(7.7x10^-3√z)") )
				pBoring->m_nTypeBeta = 0;
			else
				pBoring->m_nTypeBeta = 1;
			pBoring->m_dqu = GetValueUnitChange(m_Grid.GetTextMatrixDouble(nRow, nCol++),pStd->m_szKgf_CM2, UNIT_CGS_KGF_CM2);
			pBoring->m_dAlphaE = m_Grid.GetTextMatrixDouble(nRow, nCol++);
			pBoring->m_dm = m_Grid.GetTextMatrixDouble(nRow, nCol++);
			pBoring->m_ds = m_Grid.GetTextMatrixDouble(nRow, nCol++);
		}

		nRow++;
	}

	if(m_pIn->GetDialogByIDD(IDD_GEN_FOOTING)->IsUsedFlag())
	{
		for(long nBri = 0; nBri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); nBri++)
		{
			CRcBridgeApp *pBri = m_pIn->m_pARcBridgeDataStd->GetBridge(nBri);
			if(pBri == NULL) continue;

			long nCountJijum = pBri->GetCountJijum();
			for(long nJijum = 0; nJijum < nCountJijum; nJijum++)
			{
				CFootingApp *pFooting = pBri->GetFootingByJijumNumber(nJijum); 
				if(pFooting == NULL) continue;
				if(pBri->m_bAutoGroundInfo[nJijum])
				{
					pBri->SetDataDefaultBaseGround(nJijum, FALSE);
				}
			}
		}
	}
}

void CInputBaseGroundInfo::OnSelchangeComboBoring() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	int nTab = m_ctlTab.GetCurSel();

	CBoringDataEx *pBoringEx = pStd->m_pArrBoringEx.GetAt(nTab);

	int nPrevCountLayer = pBoringEx->m_pArrBoring.GetSize();

	int nCountLayer = m_cbBoringNo.GetCurSel() + 1;

	if(nPrevCountLayer<nCountLayer)
	{
		int i=0; for(i=nPrevCountLayer; i<nCountLayer; i++)
		{
			pBoringEx->m_pArrBoring.Add(new CBoringData);
			CBoringData* pBoring = pBoringEx->m_pArrBoring.GetAt(i);
			pBoring->m_nTypeSoil =  0;
			pBoring->m_nNameSoil =  0;
			pBoring->m_nNValue   = 10;
			pBoring->m_dDepth	 = 5.0*(i+1);
			pBoring->m_dEo		= 100;
			pBoring->m_RatePoasong	= 0.2;
			// LSD
			pBoring->m_dSigma_V = 0.0;
			pBoring->m_dN60 = 10.0;
			pBoring->m_nTypeBeta = 0;
			pBoring->m_dqu = 100.0; 
			pBoring->m_dAlphaE = 1.0;
			pBoring->m_dm = 2.05;
			pBoring->m_ds = 0.00293;
		}
	}
	else
	{
		AhTPAMake(nCountLayer,&pBoringEx->m_pArrBoring,(CBoringData*)0);
	}

	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CInputBaseGroundInfo::InitComboBoring()
{
	CString str = _T("");

	long i=0; for(i=0; i<MAX_BORING; i++)
	{
		str.Format(_T("%d 개"), i+1);
		m_cbBoringNo.AddString(str);
	}
}

void CInputBaseGroundInfo::DrawVertGroundEL(CDomyun *pDom)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	CLineInfo* pLine = pStd->GetLineApp();	//종단지반고

	CDomyun Dom(pDom);

	Dom.SetLineColor(1);
	Dom.SetTextColor(2);
	Dom.SetTextHeight(pLine->GetTextHeightIp());
   	
	pLine->DrawJongEarthLine(&Dom, m_dVScaleDraw);						// 종단 곡선모양
	
	DrawCursor(&Dom, m_Grid.GetFocusCell().row);

	*pDom << Dom;
}

void CInputBaseGroundInfo::DrawCursor(CDomyun *pDom, int nRow)
{
// 	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
// 	CLineInfoApp* pLineApp = pStd->GetLineApp();

// 	double dSta = frM(m_Grid.GetTextMatrixDouble(nRow,1));
// 	double dEle = frM(m_Grid.GetTextMatrixDouble(nRow,3));

// 	double dStaSttJongDan = pLineApp->m_VIPData[0][0];
// 	double dStaEndJongDan = pLineApp->m_VIPData[pLineApp->m_VIPsu - 1][0];
// 	double dElevBase = pLineApp->GetMinSectionEL(dStaSttJongDan, dStaEndJongDan);

	CDomyun Dom(pDom);

	*pDom << Dom;
}

void CInputBaseGroundInfo::InitTabCtrl()
{
	m_ctlTab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_ctlTab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	int nCountBoring = pStd->m_pArrBoringEx.GetSize();
	BOOL bFirst = !IsUsedFlag();
	m_ctlTab.DeleteAllItems();

	if(bFirst)
		m_ctlTab.InsertItem(0, _T("BH-1"));
	else
	{
		int i=0; for(i=0; i<nCountBoring; i++)
		{
			CBoringDataEx *pBoringEx = pStd->m_pArrBoringEx.GetAt(i);
			m_ctlTab.InsertItem(i, pBoringEx->m_sNameBoring);
		}
	}
	m_ctlTab.SetCurSel(0);
}


void CInputBaseGroundInfo::OnSelchangeComboGroundinfonum() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	// ARCBRIDGE-2534
	long nSizeBoring  = pStd->m_pArrBoringEx.GetSize();
	long nCountBoring = m_cbGroundInfoNum.GetCurSel() + 1;

	AhTPAMake(nCountBoring,&pStd->m_pArrBoringEx,(CBoringDataEx*)0);

	for(long i=nSizeBoring; i<nCountBoring; i++)
	{
		SetDataDefaultBoringInfo(i);
	}

	BOOL bExistBoring = (nCountBoring>0) ? TRUE : FALSE;
	GetDlgItem(IDC_EDIT_BORING_NAME)->EnableWindow(bExistBoring);
	GetDlgItem(IDC_EDIT_BORING_EL)->EnableWindow(bExistBoring);
	GetDlgItem(IDC_EDIT_BORING_STATION)->EnableWindow(bExistBoring);
	GetDlgItem(IDC_COMBO_BORING)->EnableWindow(bExistBoring);

	InitTabCtrl();
	m_ctlTab.SetCurFocus(0);
	SetDataInit();
	DrawInputDomyunView();
}

void CInputBaseGroundInfo::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SetDataInit();

	DrawInputDomyunView();

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	long nTab = m_ctlTab.GetCurSel();
	CBoringDataEx *pBoringEx = pStd->m_pArrBoringEx.GetAt(nTab);

	CString str = pBoringEx->m_sNameBoring;
	m_pView->GetDomyun()->STMakeCursor(str);

	return;
}

void CInputBaseGroundInfo::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SetDataSave();

	return;
}

void CInputBaseGroundInfo::OnKillfocusEditBoringName() 
{
	UpdateData();

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	int nTab = m_ctlTab.GetCurSel();

	CBoringDataEx *pBoringEx = pStd->m_pArrBoringEx.GetAt(nTab);

	pBoringEx->m_sNameBoring = m_strBoringName;

	// 이름 변경시 탭 이름도 변경해준다. ///////
	InitTabCtrl();
	m_ctlTab.SetCurSel(nTab);
	///////////////////////////////////////////

	DrawInputDomyunView(FALSE);
}

void CInputBaseGroundInfo::OnKillfocusEditBoringEL() 
{
	UpdateData();

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	int nTab = m_ctlTab.GetCurSel();
	CBoringDataEx *pBoringEx = pStd->m_pArrBoringEx.GetAt(nTab);

	pBoringEx->m_EL = atof(m_szEL);

	DrawInputDomyunView(FALSE);
}

void CInputBaseGroundInfo::OnKillfocusEditBoringStation() 
{
	UpdateData();

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	int nTab = m_ctlTab.GetCurSel();
	CBoringDataEx *pBoringEx = pStd->m_pArrBoringEx.GetAt(nTab);

	pBoringEx->m_dStation = atof(m_szStation);

	DrawInputDomyunView(FALSE);
}

void CInputBaseGroundInfo::DrawLineVertical(CDomyun *pDomP)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(0);
	CLineInfo* pLine = pStd->GetLineApp();

	int nVIPCount = pLine->m_VIPsu;
	double dVBPSta = pLine->m_VIPData[0][0];
	double dVEPSta = pLine->m_VIPData[nVIPCount-1][0];
	double dTextH = pLine->GetTextHeightIp()/200.;

	CDomyun Dom(pDomP);
	CDomyun DomAll(pDomP);

	// 종단 선형 표시
	Dom.SetLineColor(1);
	Dom.SetTextHeight(dTextH);

   	pLine->DrawJongDan(&Dom,m_dVScaleDraw);

	DomAll << Dom;

	// 교량 시종점 표시
	int nBridgeCount = pStd->GetBridgeSize();
	for(int nBr=0; nBr<nBridgeCount; nBr++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(nBr);

		if(pBri->m_nSeparBri==2) continue;

		double dSttBri = pBri->m_dStationBridgeStt;
		double dEndBri = dSttBri + pBri->m_dLengthBridge;
		CString strBri = pBri->m_strBridgeName;

		Dom.SetLineColor(3);
		Dom.SetTextColor(2);
		Dom.SetTextHeight(dTextH);
		pLine->DrawJongBridge(&Dom, dVBPSta, dVEPSta, dSttBri, dEndBri,m_dVScaleDraw);

		CDRect rect;
		Dom.SetCalcExtRect();
		rect = Dom.GetExtRect();

		Dom.SetTextHeight(dTextH*1.5);
		Dom.SetTextAngle(0);
		Dom.SetTextAlignHorz(TA_CENTER);
		Dom.TextOut((dSttBri+dEndBri)*0.5, rect.bottom, strBri);

		DomAll << Dom;
	}

	*pDomP << DomAll;
}

void CInputBaseGroundInfo::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	long nTab = m_ctlTab.GetCurSel();
	CBoringDataEx *pBoringEx = pStd->m_pArrBoringEx.GetAt(nTab);

	CString str = pBoringEx->m_sNameBoring;
	m_pView->GetDomyun()->STMakeCursor(str);

	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	
	int nCol = pGridView->iColumn;

	CString strComment = _T("");
	CString strCol  = m_Grid.GetTextMatrix(0,nCol);
	if(strCol==hggettext("토질 분류"))
	{
		strComment = hggettext("토질 분류을 입력합니다. 점성토/사질토");
		if(m_bDesignLsd)
		{
			strComment += hggettext("/암반");
		}
	}
	if(strCol=="보링 명칭")		strComment = "해당 토층의 보링 명칭을 선택합니다.";
	if(strCol=="dH")			strComment = "해당 토층의 심도(누가깊이)를 입력합니다. (단위 : m)";
	if(strCol=="N")				strComment = "해당 토층의 평균 N치를 입력합니다.";
	if(strCol=="c")				strComment.Format("지반 점착력 (단위 : %s)",pStd->m_szTonf_M2);
	if(strCol=="Cu")			strComment.Format("비배수 전단강도 (단위 : %s)",pStd->m_szTonf_M2);
	if(strCol=="Eo")			strComment.Format("지반의 탄성계수 (단위 : %s)", pStd->IsSI() ? ( m_bDesignLsd ? "N/mm²" : "kN/m²") : UNIT_CGS_KGF_CM2);
	if(strCol=="μ")			strComment.Format("지반의 포아슨비");
	if(strCol=="σv'")			strComment.Format("대상층 중간에서의 연직유효응력(Mpa)");
	if(strCol=="N60")			strComment.Format("설계구역 지층의 평균 N값(해머 효율에 대해서 보정한 값)");
	if(strCol=="β")			strComment.Format("하중전이계수");
	if(strCol=="qu(N/mm^2)")	strComment.Format("암의 일축압축강도");
	if(strCol=="αE")			strComment.Format("암반 절리를 고려한 감소계수");
	if(strCol=="m")				strComment.Format("파쇄 암반 정수");
	if(strCol=="s")				strComment.Format("파쇄 암반 정수");

	m_pIn->SetComment(strComment);
}

void CInputBaseGroundInfo::OnButtonNext() 
{
	// TODO: Add your control notification handler code here
	BOOL bInputValid = CheckEoAndPoisonRate();

	if(bInputValid==FALSE)
	{
		if(AfxMessageBox("지반정보에 Eo,μ값이 입력되지 않는 항목이 있습니다.\n계속하시겠습니까 ?",MB_YESNO)==IDNO)
			return;
	}

	CInputDialogBase::OnButtonNext();	
}

void CInputBaseGroundInfo::OnButtonPrev() 
{
	// TODO: Add your control notification handler code here
	BOOL bInputValid = CheckEoAndPoisonRate();

	if(bInputValid==FALSE)
	{
		if(AfxMessageBox("지반정보에 Eo,μ값이 입력되지 않는 항목이 있습니다.\n계속하시겠습니까 ?",MB_YESNO)==IDNO)
			return;
	}

	CInputDialogBase::OnButtonPrev();	
}

BOOL CInputBaseGroundInfo::CheckEoAndPoisonRate()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	if(pStd->m_nTypeRigidFootingEQ==2) return TRUE;

	BOOL bInputValid = TRUE;

	long nSizeData = pStd->m_pArrBoringEx.GetSize();

	for(long nB=0; nB<nSizeData; nB++)
	{
		CBoringDataEx *pBorEx = pStd->m_pArrBoringEx.GetAt(nB);
		long nQtyBoring = pBorEx->m_pArrBoring.GetSize();
		long i=0; for(i=0; i<nQtyBoring; i++)
		{
			CBoringData *pBor = pBorEx->m_pArrBoring.GetAt(i);
			if(Compare(pBor->m_dEo,0.0,"=") ||Compare(pBor->m_RatePoasong,0.0,"="))
			{
				bInputValid = FALSE;
				break;
			}
		}
		if(bInputValid==FALSE) break;
	}

	return bInputValid;
}
