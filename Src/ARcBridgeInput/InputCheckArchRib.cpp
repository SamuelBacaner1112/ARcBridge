// InputCheckArchRib.cpp : implementation file
//

#include "stdafx.h"
#include "../../Src/ARcBridgeCalculate/ARcBridgeCalculate.h"
#include "../ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "ARcBridgeInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputCheckArchRib dialog


CInputCheckArchRib::CInputCheckArchRib(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CInputCheckArchRib::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputCheckArchRib)
	//}}AFX_DATA_INIT
	m_pBri	= NULL;
	m_pCalc = NULL;

	m_nTypeCheck = -1;
	m_pIn = NULL;
	m_nInputIDD = -1;
}

CInputCheckArchRib::~CInputCheckArchRib()
{
//	if(m_pCalc) delete m_pCalc;
}

void CInputCheckArchRib::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputCheckArchRib)
	DDX_Control(pDX, IDC_TAB_RIB, m_ctlTab);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID_RIB, m_Grid);
	DDX_DomControl(pDX, IDC_DOM_RIB, m_DomView);
}


BEGIN_MESSAGE_MAP(CInputCheckArchRib, CXTResizeDialog)
	//{{AFX_MSG_MAP(CInputCheckArchRib)
//	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_RIB, OnTabSelchange)
//	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_RIB, OnTabSelchanging)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputCheckArchRib message handlers

BOOL CInputCheckArchRib::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetResize(IDC_DOM_RIB,	 SZ_TOP_CENTER, SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID_RIB,	 SZ_TOP_LEFT, SZ_BOTTOM_CENTER);
	SetResize(IDC_TAB_RIB,	 SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK,		 SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,	 SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	
	
	m_DomView.SetOwnerMouseMode(FALSE);
	m_DomView.m_bEnablePopupMenu = TRUE;
	m_DomView.SetCrossLength(0);
	//////////////////////////////////////////////////////////////////////////
	
	InitTab();
	SetGridTitle();
	SetDataInit();
	
	DrawInputDomyunView();
	ShowDlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputCheckArchRib::InitTab()
{
	m_ctlTab.SendInitialUpdate(TRUE);
	m_ctlTab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	
	long nTab = 0;

	CString str = _T("");
	long nSize = m_pList.GetSize();
	long i=0; for(i=0; i<nSize; i++)
	{
		CGirderAndColumn *pGirCol = m_pList.GetAt(i);
		CConcBasic *pConc = pGirCol->pConcColumn[0];
		str = pConc->m_sTitle;
		m_ctlTab.InsertItem(nTab++, str);
	}
}

void CInputCheckArchRib::SetDataInit()
{
// 	long nRow = 0;
// 	long nCol = 0;
	long nTab = m_ctlTab.GetCurSel();
	CString szOk = _T("");
	CString str = _T("");

	CGirderAndColumn *pGirCol = m_pList.GetAt(nTab);
	long n=0; for(n=0; n<RCBRIDGE_COL_CHECK; n++)
	{
		long nRow = 1;
		long nCol = n+2;
		CConcBasic *pConc = pGirCol->pConcColumn[n];
		CColumn ColCalc(pGirCol->pConcColumn[n]);
		ColCalc.CalculateAll(FALSE);

		BOOL bWall = pConc->IsWall();

		long i=0; for(i=0; i<10; i++)
		{
			if(i==0) str.Format(bWall ? "%.4f" : "%.3f",ColCalc.GetValuePmin());
			if(i==1) str.Format("%.3f",ColCalc.GetRebarUsedRate());
			if(i==2) str.Format("%.3f",MaxRebar_Column);
			if(i==3) str.Format("%s",ColCalc.GetCheckRebarRate()==1 ? "O.K" : "N.G");
			if(i==4) str.Format("%.3f",GetValueUnitChange(ColCalc.GetPiPn(),UNIT_CGS_TONF,pConc->m_szTonf));
			if(i==5) str.Format("%.3f",GetValueUnitChange(pConc->m_Axial,UNIT_CGS_TONF,pConc->m_szTonf));
			if(i==6) str.Format("%.3f",GetValueUnitChange(ColCalc.GetPiMn(),UNIT_CGS_TONF,pConc->m_szTonf));
			if(i==7) str.Format("%.3f",GetValueUnitChange(pConc->m_Moment,UNIT_CGS_TONFM,pConc->m_szTonfM));
			if(i==8) str.Format("%.3f",ColCalc.GetSafeFactorAxial());
			if(i==9) str.Format("%s",ColCalc.IsOkAxial() ? "O.K" : "N.G");

			m_Grid.SetTextMatrix(nRow+i,nCol,str);
			if(str=="N.G") 
				m_Grid.SetItemFgColour(nRow+i, nCol, RGB(255, 0, 0));
			else 
				m_Grid.SetItemFgColour(nRow+i, nCol, RGB(0, 0, 0));
		}
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputCheckArchRib::ShowDlg()
{
	CSize WSize = CSize(920,400);
	SetWindowPos(&wndTop, 0, 0, WSize.cx, WSize.cy, SWP_NOMOVE | SWP_DRAWFRAME);
	
	CenterWindow();
}

void CInputCheckArchRib::SetBriAndContent(CRcBridgeRebar *pBri, long nInputIDD, long nTypeCheck)
{
	m_pBri		 = pBri;
	m_nInputIDD  = nInputIDD;
	m_nTypeCheck = nTypeCheck;
	m_pCalc		 = new CARcBridgeCalcStd(pBri);

	m_pList.RemoveAll();
	pBri->TrimRibByFootingUpper();
	m_pCalc->CalculateAll();
	long nSize = m_pCalc->m_pArchRib->m_pList.GetSize();
	long i=0; for(i=0; i<nSize; i++)
	{
		CGirderAndColumn *pGirCol = m_pCalc->m_pArchRib->m_pList.GetAt(i);
		m_pList.Add(pGirCol);
	}
}

void CInputCheckArchRib::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CXTResizeDialog::OnCancel();
}

void CInputCheckArchRib::OnOK() 
{
	// TODO: Add extra validation here
	
	CXTResizeDialog::OnOK();
}

void CInputCheckArchRib::ShowAndCheck(CWnd *pWndParent)
{
	if(!GetSafeHwnd())	
		Create(IDD, pWndParent);
	
	if(m_ctlTab.GetItemCount()>0 && !IsWindowVisible())	m_ctlTab.SetCurSel(0);
	
	int nTab = m_ctlTab.GetCurSel();	
	
	ShowDlg();
	
	m_ctlTab.SetCurSel(nTab);
	
	ShowWindow(SW_SHOW);
	SetGridTitle();
	SetDataInit();

	DrawInputDomyunView();
//	m_DomView.STMakeCursor(str);
}

void CInputCheckArchRib::Check()
{
	if(!GetSafeHwnd()) return;
	SetGridTitle();
	SetDataInit();

	DrawInputDomyunView();
//	m_DomView.STMakeCursor(str);
}
/*
void CInputCheckArchRib::OnTabSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	long nTab = m_ctlTab.GetCurSel();

	DrawInputDomyunView();
//	m_DomView.STMakeCursor(str);
	
	*pResult = 0;
}

void CInputCheckArchRib::OnTabSelchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	long nTab = m_ctlTab.GetCurSel();
	*pResult = 0;
}*/

void CInputCheckArchRib::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SetDataInit();
	DrawInputDomyunView();
	return;
}

void CInputCheckArchRib::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SetDataInit();
	DrawInputDomyunView();
	return;
}

BOOL CInputCheckArchRib::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_MOUSEWHEEL)
	{
		m_DomView.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
	}
	
	return CXTResizeDialog::PreTranslateMessage(pMsg);
}

void CInputCheckArchRib::DrawInputDomyunView(BOOL bZoomAll)
{	
	if(!m_pBri) return;
	CARcBridgeDataStd* pStd = m_pBri->m_pARcBridgeDataStd;

	CARcBridgeDBStandard DBStd(m_pBri, m_pIn->m_pARoadOptionStd, pStd);
	
	CDomyun *pDom = m_DomView.GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(20);

/////////////////////////////////////////
	long nTab = m_ctlTab.GetCurSel();

	CGirderAndColumn *pGirCol = m_pList.GetAt(nTab);

	CColumn ColCalc(pGirCol->pConcColumn[0]);
	ColCalc.CalculateAll(TRUE);

	CDrawPM DrawPM(m_pIn->m_pARoadOptionStd);
	DrawPM.DrawPM(pDom,&ColCalc);
/////////////////////////////////////////	

	pDom->SetCalcExtRect();
	CDRect bound = pDom->GetExtRect();
	pDom->SetLineColor(-1);
	pDom->Point(bound.left, bound.bottom+100.0);

	if(bZoomAll)
		m_DomView.ZoomAll();
	else
		m_DomView.RedrawAll();
}

void CInputCheckArchRib::SetGridTitle()
{
	m_Grid.DeleteAllItems();

	long nSizeRow = 1 + 4 + 6;
	long nSizeCol = RCBRIDGE_COL_CHECK + 2;			

	m_Grid.SetColumnCount(nSizeCol);
	m_Grid.SetRowCount(nSizeRow);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(2);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(100);
	m_Grid.SetColumnWidth(0,30);
	m_Grid.SetColumnWidth(1,70);

	m_Grid.SetMergeCol(0, 0, 1);
	m_Grid.SetTextMatrix(0,0,"구 분");
	m_Grid.SetTextMatrix(0,2,"축력최대");
	m_Grid.SetTextMatrix(0,3,"부모멘트최대");
	m_Grid.SetTextMatrix(0,4,"정모멘트최대");
	m_Grid.SetTextMatrix(0,5,"지진시");

	CStringArray szArr;
	szArr.Add("축력최대");
	szArr.Add("부모멘트최대");
	szArr.Add("정모멘트최대");
	szArr.Add("지진시");

	long nRow = 1;
	m_Grid.SetMergeRow(nRow,nRow+3,0);
	m_Grid.SetTextMatrix(nRow,0,"철근비", DT_WORDBREAK, FALSE);
	m_Grid.SetTextMatrix(nRow++,1,"Pmin");
	m_Grid.SetTextMatrix(nRow++,1,"Puse");
	m_Grid.SetTextMatrix(nRow++,1,"Pmax");
	m_Grid.SetTextMatrix(nRow++,1,"비고");

	m_Grid.SetMergeRow(nRow,nRow+5,0);
	m_Grid.SetTextMatrix(nRow,0,"축방향력ㆍ휨", DT_WORDBREAK, FALSE);
	m_Grid.SetTextMatrix(nRow++,1,"φPn");
	m_Grid.SetTextMatrix(nRow++,1,"Pu");
	m_Grid.SetTextMatrix(nRow++,1,"φMn");
	m_Grid.SetTextMatrix(nRow++,1,"Mn");
	m_Grid.SetTextMatrix(nRow++,1,"안전도");
	m_Grid.SetTextMatrix(nRow++,1,"비고");

	m_Grid.SetRedraw(TRUE,TRUE);
}
