// RebarWingWallDlg.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "RebarWingWallDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRebarWingWallDlg dialog


CRebarWingWallDlg::CRebarWingWallDlg(CARcBridgeInputStd* pIn, BOOL bStt, BOOL bLeft, CWnd* pParent /*=NULL*/)
	: CDialog(CRebarWingWallDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRebarWingWall)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pIn = pIn;
	m_bStt = bStt;
	m_bLeft = bLeft;

	m_nTab = -1;
	m_bAddRebarReinforce = FALSE;
	m_bSelMainCTCD = FALSE;
	m_bUpperRebarSlope= FALSE;

	m_szArr.RemoveAll();
	CRebarShearDetail shear;
	long i = 0; for(i = 0; i < TYPE_SHEAR_SU; i++)
		m_szArr.Add(shear.GetStringType(i));
}


void CRebarWingWallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRebarWingWallDlg)
	DDX_Control(pDX, IDC_HYPER_HELP, m_hyperHelp);
	DDX_Control(pDX, IDC_COMBO_REBAR_CURVE, m_cbRebarCurve);
	DDX_Control(pDX, IDC_COMBO_SLOPE_BLOCK_OUT, m_cbSlopeBlockOut);
	DDX_Control(pDX, IDC_COMBO_SLOPE_BLOCK_IN, m_cbSlopeBlockIn);
	DDX_Control(pDX, IDC_COMBO_HUNCH_CTC, m_cbHunchCTC);
	DDX_Control(pDX, IDC_COMBO_STD_VERCTC, m_cbStdVerCTC);
	DDX_Control(pDX, IDC_COMBO_MAINCTC_B, m_cbMainCTCB);
	DDX_Control(pDX, IDC_COMBO_MAINCTC_A, m_cbMainCTCA);
	DDX_Control(pDX, IDC_COMBO_SHEAR_BIND, m_cbShearBind);
	DDX_Control(pDX, IDC_BUTTON_RECOMMAND, m_btnRecommand);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Check(pDX, IDC_CHECK_MAINCTC_D, m_bSelMainCTCD);
	DDX_Check(pDX, IDC_CHECK_UPPER_REBAR_SLOPE, m_bUpperRebarSlope);
	DDX_Check(pDX, IDC_CHECK_ADDREBAR_REINFORCE, m_bAddRebarReinforce);
	DDX_Control(pDX, IDC_COMBO_REBAR_SHEAR_BC, m_cbRebarShearBC);
	DDX_Control(pDX, IDC_TAB, m_ctlTab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRebarWingWallDlg, CDialog)
	//{{AFX_MSG_MAP(CRebarWingWallDlg)
	ON_BN_CLICKED(IDC_BUTTON_RECOMMAND, OnButtonDefault)
	ON_BN_CLICKED(IDC_CHECK_UPPER_REBAR_SLOPE, OnCheckUpperRebarSlope)
	ON_CBN_SELCHANGE(IDC_COMBO_REBAR_SHEAR_BC, OnSelchangeComboRebarShearBc)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRebarWingWallDlg message handlers

BOOL CRebarWingWallDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		UpdateData();
		if(m_Grid.GetFocusCell().col == m_Grid.GetCols()-1)
			pMsg->wParam = VK_RIGHT;
		else
			pMsg->wParam = VK_TAB;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CRebarWingWallDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	COLORREF clrLink = RGB(0x00, 0x00, 0xFF);
	m_hyperHelp.SetColors(clrLink, clrLink, clrLink);
	m_hyperHelp.SetURL(m_pIn->GetManualPath());
	m_hyperHelp.SetTipText(_T("현재 입력창에 대한 도움말을 표시합니다."));
	
	CString str = _T("");
	int i=0; for(i=0; i<MAX_REBAR_CTC; i++)
	{
		CHgBaseConcStd hgBaseConcStd;
		str.Format(_T("CTC %.f"), hgBaseConcStd.GetValueRebarCTC(i));
		m_cbStdVerCTC.AddString(str);
		m_cbMainCTCA.AddString(str);
		m_cbMainCTCB.AddString(str);
	}

	CRcBridgeRebar* pBri = m_pIn->GetRcBridgeRebar();
	CWingWall* pWing = pBri->GetWingWall(m_bStt, m_bLeft);

	double dL2 = pWing->m_dL2;
	if(dL2==0)
	{
		SetDlgItemText(IDC_STATIC_MAINCTC_A, _T("D구간 주철근 배치 간격:"));
		GetDlgItem(IDC_STATIC_MAINCTC_B)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_MAINCTC_B)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_REBAR_CURVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_REBAR_CURVE)->EnableWindow(FALSE);
	}
	else
	{
		SetDlgItemText(IDC_STATIC_MAINCTC_A, _T("A구간 주철근 배치 간격:"));
		GetDlgItem(IDC_STATIC_MAINCTC_B)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_MAINCTC_B)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_REBAR_CURVE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_REBAR_CURVE)->EnableWindow(TRUE);
	}

	long nDan = 0;
	int nSize = pWing->m_xyArrSide[1][nDan].GetSize();
	for(i=0; i<nSize; i++)
	{
		str.Format(_T("%d 개"), i+1);
		m_cbSlopeBlockOut.AddString(str);
	}
	nSize = pWing->m_xyArrSide[0][nDan].GetSize();
	for(i=0; i<nSize; i++)
	{
		str.Format(_T("%d 개"), i+1);
		m_cbSlopeBlockIn.AddString(str);
	}

	BOOL bCheckCTCD = pWing->m_dL1==0 || pWing->m_dL2==0 ? FALSE : TRUE;
	GetDlgItem(IDC_CHECK_MAINCTC_D)->EnableWindow(bCheckCTCD);

	m_Grid.InitGrid(pBri->GetValueFy(m_bStt? ePartWingWallStt : ePartWingWallEnd));
	InitTabCtrl();
	SetGridData();
	SetDataInit();
	
	OnCheckUpperRebarSlope();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRebarWingWallDlg::OnSelchangeTab(NMHDR* pNotifyStruct, LRESULT* result) 
{
	// TODO: Add your control notification handler code here
	m_nTab = m_ctlTab.GetCurSel();
	SetGridData();
	SetDataInit();

	return;
}

void CRebarWingWallDlg::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	SetDataSave();
	SetGridData();
	SetDataInit();
}

void CRebarWingWallDlg::InitTabCtrl()
{
	m_ctlTab.RemoveAllTabs();
	m_ctlTab.SendInitialUpdate(TRUE);
	m_ctlTab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	m_ctlTab.InsertItem(0, hggettext("철근입력"));
	m_ctlTab.InsertItem(1, hggettext("단수입력"));

	m_nTab = m_ctlTab.GetCurFocus();
}

void CRebarWingWallDlg::SetDataDefault()
{
	CRcBridgeRebar* pBri = m_pIn->GetRcBridgeRebar();
	CWingWall* pWing = pBri->GetWingWall(m_bStt, m_bLeft);

	if((m_bStt && m_bLeft) || (!m_bStt && !m_bLeft))
	{
		//pBri->SyncBridge(1);
		pBri->GetTvVertSection_Std(1, TRUE, FALSE);
	}
	else
	{
		//pBri->SyncBridge(-1);
		pBri->GetTvVertSection_Std(-1, TRUE, FALSE);
	}

	pBri->SetDataDefaultRebarWingWallOption(m_bStt, m_bLeft);

	GetDlgItem(IDC_STATIC_SLOPE_BLOCK_OUT)->EnableWindow();
	m_cbSlopeBlockOut.EnableWindow();
	GetDlgItem(IDC_STATIC_SLOPE_BLOCK_IN)->EnableWindow();
	m_cbSlopeBlockIn.EnableWindow();

	pBri->GetTvVertSection_Std(-1, TRUE, FALSE, TRUE);
	SetDataInit();
}


void CRebarWingWallDlg::SetGridData()
{
	CRcBridgeRebar* pBri = m_pIn->GetRcBridgeRebar();
	CWingWall* pWing = pBri->GetWingWall(m_bStt, m_bLeft);

	m_Grid.SetGridDefault(0, 0, 0, 0);

	if(m_nTab == 0) SetGridData_Rebar();
	else if(m_nTab == 1) SetGridData_Dan();

	SetComboInit();
	m_bUpperRebarSlope = pWing->m_bUpperRebarSlope;
	if(pWing->m_dL1 == 0 || pWing->m_dL2 == 0)
		pWing->m_bSelMainCTCD	= FALSE;

	m_bSelMainCTCD = pWing->m_bSelMainCTCD;
	m_bAddRebarReinforce = pWing->m_bAddRebarReinforce;
}

void CRebarWingWallDlg::SetGridData_Rebar()
{
	m_Grid.SetGridDefault(2, 2, 2, 1);
	m_Grid.SetCellSize(20, -1);

	CRcBridgeRebar* pBri = m_pIn->GetRcBridgeRebar();
	CWingWall* pWing = pBri->GetWingWall(m_bStt, m_bLeft);
	long row = 0, col = 0;
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;
	long nDan = 0;

	double dL1 = pWing->m_dL1;
	double dL2 = pWing->m_dL2;

	//콤보박스 내용
	CStringArray strArrRebar;
	strArrRebar.RemoveAll();
	CRebarShearDetail shear;
	long i = 0; for(i = 0; i < TYPE_SHEAR_SU; i++)
		strArrRebar.Add(shear.GetStringType(i));
	strBlank = _T("-");
	
	//제목
	m_Grid.AddCell(row, col, _T("구간"));		m_Grid.AddCell(row+1, col++, _T("구간"));
	m_Grid.AddCell(row, col, _T("전면"));		m_Grid.AddCell(row+1, col++, _T("주철근"));
	m_Grid.AddCell(row, col, _T("전면"));		m_Grid.AddCell(row+1, col++, _T("배력철근"));
	m_Grid.AddCell(row, col, _T("배면"));		m_Grid.AddCell(row+1, col++, _T("주철근"));
	m_Grid.AddCell(row, col, _T("배면"));		m_Grid.AddCell(row+1, col++, _T("배력철근"));
	m_Grid.AddCell(row, col, _T("전단철근"));	m_Grid.AddCell(row+1, col++, _T("직경"));
	m_Grid.AddCell(row, col, _T("전단철근"));	m_Grid.AddCell(row+1, col++, _T("간격"));
	m_Grid.AddCell(row, col, _T("전단철근"));	m_Grid.AddCell(row+1, col++, _T("종류"));
	m_Grid.AddCell(row, col, _T("헌치철근"));	m_Grid.AddCell(row+1, col, _T("직경"));
	col = 0, row = 2;
	if(dL2 != 0)
	{
		m_Grid.AddCell(row++, col, _T("A"));	m_Grid.AddCell(row++, col, _T("B"));
		m_Grid.AddCell(row++, col, _T("C"));
	}
	if(dL1 != 0)
		m_Grid.AddCell(row++, col, _T("D"));

	row = 2, col = 1;
	//<내용>
	//-전면/배면
	for(long iFB = iFRONT; iFB <= iBACK; iFB++)
	{
		BOOL bFront = (iFB == iFRONT) ? TRUE : FALSE;
		//주철근
		row = 2;
		if(dL2 != 0)
		{
			m_Grid.AddCell(row++, col, &pWing->m_dDiaMainA[bFront][nDan], nFormat, nLine, _T(""), CELL_TYPE_DIA);
			m_Grid.AddCell(row++, col, &pWing->m_dDiaMainB[bFront][nDan], nFormat, nLine, _T(""), CELL_TYPE_DIA);
			m_Grid.AddCell(row++, col, &pWing->m_dDiaMainC[bFront][nDan], nFormat, nLine, _T(""), CELL_TYPE_DIA);
		}
		if(dL1 != 0)
			m_Grid.AddCell(row++, col, &pWing->m_dDiaMainD[bFront][nDan], nFormat, nLine, _T(""), CELL_TYPE_DIA);
		col++;

		//배력철근
		row = 2;
		pWing->m_dDiaSupportB[bFront][nDan] = pWing->m_dDiaMainC[bFront][nDan];
		pWing->m_dDiaSupportC[bFront][nDan] = pWing->m_dDiaMainB[bFront][nDan];
		if(dL2 != 0)
		{
			m_Grid.AddCell(row++, col, &pWing->m_dDiaSupportA[bFront][nDan], nFormat, nLine, _T(""), CELL_TYPE_DIA);
			m_Grid.AddCell(row++, col, &pWing->m_dDiaSupportB[bFront][nDan], nFormat, CELL_READONLY, _T(""), CELL_TYPE_DIA);
			m_Grid.AddCell(row++, col, &pWing->m_dDiaSupportC[bFront][nDan], nFormat, CELL_READONLY, _T(""), CELL_TYPE_DIA);
		}
		if(dL1 != 0)
			m_Grid.AddCell(row++, col, &pWing->m_dDiaSupportD[bFront][nDan], nFormat, nLine, _T(""), CELL_TYPE_DIA);

		col++;
	}

	//-전단철근
	row = 2;
	if(dL2 != 0)
	{
		//A열
		m_Grid.AddCell(row, col, &pWing->m_RebarShearDetailA.m_dDia, nFormat, nLine, _T(""), CELL_TYPE_DIA);
		m_Grid.SetLastCellFy(pBri->GetValueFy(m_bStt? ePartWingWallStt : ePartWingWallEnd, TRUE));
		m_Grid.AddCell(row, col+1, &pWing->m_dSpaceShearA, nFormat, nLine);
		m_Grid.AddCell(row++, col+2, &pWing->m_RebarShearDetailA.m_nType, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, strArrRebar);
		//B열
		if(pWing->m_nTypeShearBC==1) 
		{
			m_Grid.AddCell(row, col, &strBlank, nFormat, CELL_READONLY, _T(""), CELL_TYPE_DIA);
			m_Grid.SetLastCellFy(pBri->GetValueFy(m_bStt? ePartWingWallStt : ePartWingWallEnd, TRUE));
			m_Grid.AddCell(row, col+1, &strBlank, nFormat, CELL_READONLY);
			m_Grid.AddCell(row++, col+2, &strBlank, nFormat, CELL_READONLY, _T(""), CELL_TYPE_COMBO, strArrRebar);

		}
		else
		{
			m_Grid.AddCell(row, col, &pWing->m_RebarShearDetailB.m_dDia, nFormat, nLine, _T(""), CELL_TYPE_DIA);
			m_Grid.SetLastCellFy(pBri->GetValueFy(m_bStt? ePartWingWallStt : ePartWingWallEnd, TRUE));
			m_Grid.AddCell(row, col+1, &pWing->m_dSpaceShearB, nFormat, nLine);
			m_Grid.AddCell(row++, col+2, &pWing->m_RebarShearDetailB.m_nType, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, strArrRebar);
		}
		//C열
		if(pWing->m_nTypeShearBC==0)
		{
			m_Grid.AddCell(row, col, &strBlank, nFormat, CELL_READONLY, _T(""), CELL_TYPE_DIA);
			m_Grid.SetLastCellFy(pBri->GetValueFy(m_bStt? ePartWingWallStt : ePartWingWallEnd, TRUE));
			m_Grid.AddCell(row, col+1, &strBlank, nFormat, CELL_READONLY);
			m_Grid.AddCell(row++, col+2, &strBlank, nFormat, CELL_READONLY, _T(""), CELL_TYPE_COMBO, strArrRebar);
		}
		else
		{
			m_Grid.AddCell(row, col, &pWing->m_RebarShearDetailC.m_dDia, nFormat, nLine, _T(""), CELL_TYPE_DIA);
			m_Grid.SetLastCellFy(pBri->GetValueFy(m_bStt? ePartWingWallStt : ePartWingWallEnd, TRUE));
			m_Grid.AddCell(row, col+1, &pWing->m_dSpaceShearC, nFormat, nLine);
			m_Grid.AddCell(row++, col+2, &pWing->m_RebarShearDetailC.m_nType, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, strArrRebar);
		}
	}
	//D열
	if(dL1 != 0)
	{
		m_Grid.AddCell(row, col, &pWing->m_RebarShearDetailD.m_dDia, nFormat, nLine, _T(""), CELL_TYPE_DIA);
		m_Grid.SetLastCellFy(pBri->GetValueFy(m_bStt? ePartWingWallStt : ePartWingWallEnd, TRUE));
		m_Grid.AddCell(row, col+1, &pWing->m_dSpaceShearD, nFormat, nLine);
		m_Grid.AddCell(row++, col+2, &pWing->m_RebarShearDetailD.m_nType, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, strArrRebar);
	}

	row = 2, col = 8;
	//-헌치철근
	if(pWing->m_dL2==0)
	{
		if(dL2 != 0)
		{
			m_Grid.AddCell(row++, col, &strBlank, nFormat, nLine, _T(""), CELL_TYPE_DIA);
			m_Grid.AddCell(row++, col, &strBlank, nFormat, nLine, _T(""), CELL_TYPE_DIA);
			m_Grid.AddCell(row++, col, &strBlank, nFormat, nLine, _T(""), CELL_TYPE_DIA);
		}
		if(dL1 != 0)
			m_Grid.AddCell(row++, col, &pWing->m_dDiaHuUpper, nFormat, nLine, _T(""), CELL_TYPE_DIA);
	}
	else
	{
		if(dL2 != 0)
		{
			m_Grid.AddCell(row++, col, &pWing->m_dDiaHuUpper, nFormat, nLine, _T(""), CELL_TYPE_DIA);
			m_Grid.AddCell(row++, col, &pWing->m_dDiaHuLower, nFormat, nLine, _T(""), CELL_TYPE_DIA);
			m_Grid.AddCell(row++, col, &pWing->m_dDiaHuFoot, nFormat, nLine, _T(""), CELL_TYPE_DIA);
		}
		if(dL1 != 0)
			m_Grid.AddCell(row++, col, &strBlank, nFormat, CELL_READONLY, _T(""), CELL_TYPE_DIA);
	}

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(60);
	m_Grid.SetColumnWidth(0, 40);
	m_Grid.SetColumnWidth(6, 50);
	m_Grid.SetColumnWidth(7, 80);

	//m_Grid.ExpandColumnsToFit();
	m_Grid.SetRedraw(TRUE, TRUE);
}

void CRebarWingWallDlg::SetGridData_Dan()
{
	m_Grid.SetGridDefault(2, 2, 2, 1);
	m_Grid.SetCellSize(20, -1);

	CRcBridgeRebar* pBri = m_pIn->GetRcBridgeRebar();
	CWingWall* pWing = pBri->GetWingWall(m_bStt, m_bLeft);
	long row = 0, col = 0;
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;
	long nDan = 1;

	double dL1 = pWing->m_dL1;
	double dL2 = pWing->m_dL2;

	//콤보박스 내용
	CStringArray strArrDansu;
	CString str;
	for(long ix = 0; ix <3; ix++)
	{
		if(ix == 0) str = _T("없음");
		else str.Format(_T("%d단"), ix);
		strArrDansu.Add(str);
	}
	
	strBlank = _T("-");
	
	//제목
	m_Grid.AddCell(row, col, _T("구간"));		m_Grid.AddCell(row+1, col++, _T("구간"));
	m_Grid.AddCell(row, col, _T("전면"));		m_Grid.AddCell(row+1, col++, _T("단수"));
	m_Grid.AddCell(row, col, _T("전면"));		m_Grid.AddCell(row+1, col++, _T("주철근 2단"));
	m_Grid.AddCell(row, col, _T("전면"));		m_Grid.AddCell(row+1, col++, _T("배력철근 2단"));
	m_Grid.AddCell(row, col, _T("배면"));		m_Grid.AddCell(row+1, col++, _T("단수"));
	m_Grid.AddCell(row, col, _T("배면"));		m_Grid.AddCell(row+1, col++, _T("주철근 2단"));
	m_Grid.AddCell(row, col, _T("배면"));		m_Grid.AddCell(row+1, col, _T("배력철근 2단"));

	col = 0, row = 2;
	if(dL2 != 0)
	{
		m_Grid.AddCell(row++, col, _T("A"));	m_Grid.AddCell(row++, col, _T("B"));
		m_Grid.AddCell(row++, col, _T("C"));
	}
	if(dL1 != 0)
		m_Grid.AddCell(row++, col, _T("D"));

	row = 2, col = 1;
	//<내용>
	//-전면/배면
	for(long iFB = iFRONT; iFB <= iBACK; iFB++)
	{
		BOOL bFront = (iFB == iFRONT) ? TRUE : FALSE;
	
		row = 2;
		//단수
		pWing->m_nCountLayerC[bFront] = pWing->m_nCountLayerB[bFront];
		if(dL2 != 0)
		{
			m_Grid.AddCell(row++, col, &pWing->m_nCountLayerA[bFront], nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, strArrDansu);
			m_Grid.AddCell(row++, col, &pWing->m_nCountLayerB[bFront], nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, strArrDansu);
			m_Grid.AddCell(row++, col, &pWing->m_nCountLayerC[bFront], nFormat, TRUE, CELL_READONLY, _T(""), CELL_TYPE_COMBO, strArrDansu);
		}
		if(dL1 != 0)
		{
			m_Grid.AddCell(row++, col, &pWing->m_nCountLayerD[bFront], nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, strArrDansu);
		}
		col++;
	
		//주철근
		row = 2;
		if(dL2 != 0)
		{
			m_Grid.AddCell(row++, col, &pWing->m_dDiaMainA[bFront][nDan], nFormat, nLine, _T(""), CELL_TYPE_DIA);
			m_Grid.AddCell(row++, col, &pWing->m_dDiaMainB[bFront][nDan], nFormat, nLine, _T(""), CELL_TYPE_DIA);
			m_Grid.AddCell(row++, col, &pWing->m_dDiaMainC[bFront][nDan], nFormat, nLine, _T(""), CELL_TYPE_DIA);
		}
		if(dL1 != 0)
			m_Grid.AddCell(row++, col, &pWing->m_dDiaMainD[bFront][nDan], nFormat, nLine, _T(""), CELL_TYPE_DIA);
		col++;
		
		//배력철근
		//B,C 구간
		pWing->m_dDiaSupportB[bFront][nDan] = pWing->m_dDiaMainC[bFront][nDan];
		pWing->m_dDiaSupportC[bFront][nDan] = pWing->m_dDiaMainB[bFront][nDan];

		row = 2;
		if(dL2 != 0)
		{
			m_Grid.AddCell(row++, col, &pWing->m_dDiaSupportA[bFront][nDan], nFormat, nLine, _T(""), CELL_TYPE_DIA);
			m_Grid.AddCell(row++, col, &pWing->m_dDiaSupportB[bFront][nDan], nFormat, CELL_READONLY, _T(""), CELL_TYPE_DIA);
			m_Grid.AddCell(row++, col, &pWing->m_dDiaSupportC[bFront][nDan], nFormat, CELL_READONLY, _T(""), CELL_TYPE_DIA);
		}
		if(dL1 != 0)
			m_Grid.AddCell(row++, col, &pWing->m_dDiaSupportD[bFront][nDan], nFormat, nLine, _T(""), CELL_TYPE_DIA);
		col++;
	}

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(80);
	m_Grid.SetColumnWidth(0, 40);
	m_Grid.SetRedraw(TRUE, TRUE);
}

void CRebarWingWallDlg::SetDataInit()
{
	UpdateData(FALSE);
	m_Grid.UpdateData(FALSE);

	m_Grid.SetMergeRow(0, 1, 0);
	if(m_nTab == 0)
	{
		m_Grid.SetMergeCol(0, 1, 2);
		m_Grid.SetMergeCol(0, 3, 4);
		m_Grid.SetMergeCol(0, 5, 7);
	}
	else if(m_nTab == 1)
	{
		m_Grid.SetMergeCol(0, 1, 3);
		m_Grid.SetMergeCol(0, 4, 6);
	}
	m_Grid.SetRedraw(TRUE, TRUE);
}

void CRebarWingWallDlg::SetDataSave()
{
	UpdateData();
	m_Grid.UpdateData(TRUE);
	CRcBridgeRebar* pBri = m_pIn->GetRcBridgeRebar();
	CWingWall* pWing = pBri->GetWingWall(m_bStt, m_bLeft);
	
	CHgBaseConcStd hgBaseConcStd;
	pWing->m_dStdVerCTC = hgBaseConcStd.GetValueRebarCTC(m_cbStdVerCTC.GetCurSel());
	pWing->m_dMainCTCA = hgBaseConcStd.GetValueRebarCTC(m_cbMainCTCA.GetCurSel());
	pWing->m_dMainCTCB = hgBaseConcStd.GetValueRebarCTC(m_cbMainCTCB.GetCurSel());
	pWing->m_nHunchCTC = m_cbHunchCTC.GetCurSel()==0 ? 1 : 0;
	pWing->m_nCountShearBind = m_cbShearBind.GetCurSel() + 2;
	pWing->m_nRebarCurve = m_cbRebarCurve.GetCurSel();
	pWing->m_bUpperRebarSlope = m_bUpperRebarSlope;
	pWing->m_nCountSlopeBlock[1] = m_cbSlopeBlockOut.GetCurSel() + 1;
	pWing->m_nCountSlopeBlock[0] = m_cbSlopeBlockIn.GetCurSel() + 1;
	pWing->m_bSelMainCTCD = m_bSelMainCTCD;
	pWing->m_nTypeShearBC = m_cbRebarShearBC.GetCurSel();
	pWing->m_bAddRebarReinforce = m_bAddRebarReinforce;
}

void CRebarWingWallDlg::OnOK() 
{
	CRcBridgeRebar* pBri = m_pIn->GetRcBridgeRebar();
	CWingWall* pWing = pBri->GetWingWall(m_bStt, m_bLeft);

	SetDataSave();

	if(pWing->m_RebarShearDetailA.m_nType==TYPE_SHEAR_SINGLE_HOOK
		|| pWing->m_RebarShearDetailB.m_nType==TYPE_SHEAR_SINGLE_HOOK
		|| pWing->m_RebarShearDetailC.m_nType==TYPE_SHEAR_SINGLE_HOOK
		|| pWing->m_RebarShearDetailD.m_nType==TYPE_SHEAR_SINGLE_HOOK)
	{
		AfxMessageBox(hggettext("한가닥스트럽 선택시 하면피복이 150mm 이상 적용되어야 합니다."), MB_ICONINFORMATION);
	}

	CDialog::OnOK();
}

void CRebarWingWallDlg::OnButtonDefault() 
{
	if(AfxMessageBox("현재 입력 상자의 데이터가 모두 기본값으로 바뀝니다. 계속 하시겠습니까?  ", MB_YESNO|MB_ICONQUESTION)==IDNO)
		return;

	SetDataDefault();
	SetDataInit();
}

void CRebarWingWallDlg::SetComboInit()
{
	CRcBridgeRebar* pBri = m_pIn->GetRcBridgeRebar();
	CWingWall* pWing = pBri->GetWingWall(m_bStt, m_bLeft);

	CHgBaseConcStd baseConc;
	m_cbStdVerCTC.SetCurSel(baseConc.GetIdxByRebarCTC(pWing->m_dStdVerCTC));
	m_cbMainCTCA.SetCurSel(baseConc.GetIdxByRebarCTC(pWing->m_dMainCTCA));
	m_cbMainCTCB.SetCurSel(baseConc.GetIdxByRebarCTC(pWing->m_dMainCTCB));

	m_cbHunchCTC.SetCurSel(pWing->m_nHunchCTC==0 ? 1 : 0);
	m_cbShearBind.SetCurSel(pWing->m_nCountShearBind-2);
	m_cbRebarCurve.SetCurSel(pWing->m_nRebarCurve);
	m_cbSlopeBlockOut.SetCurSel(pWing->m_nCountSlopeBlock[1]-1);
	m_cbSlopeBlockIn.SetCurSel(pWing->m_nCountSlopeBlock[0]-1);
	m_cbRebarShearBC.SetCurSel(pWing->m_nTypeShearBC);
}

void CRebarWingWallDlg::OnCheckUpperRebarSlope() 
{
	UpdateData();

	GetDlgItem(IDC_STATIC_SLOPE_BLOCK_OUT)->EnableWindow(m_bUpperRebarSlope);
	m_cbSlopeBlockOut.EnableWindow(m_bUpperRebarSlope);
	GetDlgItem(IDC_STATIC_SLOPE_BLOCK_IN)->EnableWindow(m_bUpperRebarSlope);
	m_cbSlopeBlockIn.EnableWindow(m_bUpperRebarSlope);
}

void CRebarWingWallDlg::OnSelchangeComboRebarShearBc() 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	SetGridData();
	SetDataInit();
}
