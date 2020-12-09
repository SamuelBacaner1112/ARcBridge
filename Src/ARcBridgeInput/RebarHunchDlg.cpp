// RebarHunchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "RebarHunchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRebarHunchDlg dialog


CRebarHunchDlg::CRebarHunchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRebarHunchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRebarHunchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pBri		= NULL;
	m_pStd		= NULL;
	m_nCycle	= -1;
	m_nTab		= 0;

	m_nTabUpperSlab = -1;
	m_nTabLowerSlab = -1;
}
CRebarHunchDlg::CRebarHunchDlg(CRcBridgeRebar *pBri, CARcBridgeDataStd *pStd, long nCycle, CWnd* pParent /*=NULL*/)
	: CDialog(CRebarHunchDlg::IDD, pParent)
{
	m_pBri		= pBri;
	m_pStd		= pStd;
	m_nCycle	= nCycle;
	m_nTab		= 0;

	m_nTabUpperSlab = -1;
	m_nTabLowerSlab = -1;
}

void CRebarHunchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRebarHunchDlg)
	DDX_Control(pDX, IDC_BUTTON_RECOMMAND, m_btnRecommand);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDC_TAB, m_ctlTab);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRebarHunchDlg, CDialog)
	//{{AFX_MSG_MAP(CRebarHunchDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
	ON_BN_CLICKED(IDC_BUTTON_RECOMMAND, OnButtonRecommand)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRebarHunchDlg message handlers

BOOL CRebarHunchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString str = _T("");
	str.Format("헌치부 철근 설정");
	this->SetWindowText(str);

	m_ctlTab.RemoveAllTabs();
	m_ctlTab.SendInitialUpdate(TRUE);
	m_ctlTab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);



	m_Grid.InitGrid(m_pStd->m_Fy);
	InitTab();
	SetGridData();
	SetDataInit();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRebarHunchDlg::OnOK() 
{
	// TODO: Add extra validation here
	SetDataSave();
	
	CDialog::OnOK();
}

void CRebarHunchDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CRebarHunchDlg::SetGridData()
{
	if(m_nTab == m_nTabUpperSlab) SetGridDataUpperSlab();
	else if(m_nTab == m_nTabLowerSlab) SetGridDataLowerSlab();
}

void CRebarHunchDlg::SetDataInit()
{
	m_Grid.UpdateData(FALSE);
}

void CRebarHunchDlg::SetDataSave()
{
	m_Grid.UpdateData(TRUE);
}

void CRebarHunchDlg::InitTab()
{
	m_ctlTab.RemoveAllTabs();
	m_ctlTab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_ctlTab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	
	
	long nTab		= 0;
	m_nTabUpperSlab	= -1;
	m_nTabLowerSlab = -1;
	m_sArrTabName.RemoveAll();

	if(!m_pBri) return;
	
	m_sArrTabName.Add("상부슬래브"); m_nTabUpperSlab = nTab++;
	if(m_pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		m_sArrTabName.Add("하부슬래브"); m_nTabLowerSlab = nTab++;
	}
		
	long i = 0; for(i = 0; i < m_sArrTabName.GetSize(); i++)
		m_ctlTab.InsertItem(i, m_sArrTabName[i]);

	if(m_ctlTab.GetItemCount() <= m_nTab) m_nTab = 0;
	m_ctlTab.SetCurSel(m_nTab);
}

void CRebarHunchDlg::SetGridDataUpperSlab()
{
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 1, 1);
	m_Grid.SetCellSize(20, -1);

	CRcBridgeRebar *pBri = m_pBri;
	if(!pBri) return;

	long row = 0, col = 0;
	CString str = _T(""), sHelp = _T("");
	CStringArray sArr;
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	
	long nCountCycle	= pBri->GetCountCycleMainRebar();	// 선택된 사이클 수
	long nCountDia		= pBri->m_nQtyJigan+1;
	
	m_Grid.AddCell(row, col++, "상부슬래브");
	long cycle = 0; for(cycle = 0; cycle < nCountCycle; cycle++)
	{
		str.Format("%dCYCLE", cycle+1);
		m_Grid.AddCell(row, col++, str);
	}

	row	= 0;
	long nIdxWall		= 1;
	CDoubleArray *pdArr = NULL;
	BOOL bOK			= FALSE;
	long i = 0; for(i = 0; i < nCountDia; i++)
	{
		row++;
		col = 0;
		bOK	= TRUE;
		if(i == 0)
		{
			str = "시점벽체";
			if(!pBri->IsOutWall(TRUE)) bOK = FALSE;
		}
		else if(i == nCountDia-1) 
		{
			str = "종점벽체";
			if(!pBri->IsOutWall(FALSE)) bOK = FALSE;
		}
		else 
		{
			str.Format("중간지점%d", nIdxWall++);
		}

		m_Grid.AddCell(row, col++, str);
		for(cycle = 0; cycle < nCountCycle; cycle++)
		{
			pdArr	= &pBri->m_dArrDiaUpperSlabHunch[cycle];
			m_Grid.AddCell(row, col++, pdArr, i, nFormat, bOK ? nLine : CELL_READONLY, sHelp, CELL_TYPE_DIA);
		}
		
	}


	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(60);
	m_Grid.SetColumnWidth(0, 150);
}

void CRebarHunchDlg::SetGridDataLowerSlab()
{
}
// 외측벽체 철근 단수 설정



void CRebarHunchDlg::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	
	return;
}

void CRebarHunchDlg::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	SetFocus();
	m_nTab = m_ctlTab.GetCurSel();
	SetGridData();
	SetDataInit();

	return;
}

BOOL CRebarHunchDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_Grid.ViewGridCellForCombo(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}

void CRebarHunchDlg::OnButtonRecommand() 
{
	// TODO: Add your control notification handler code here
	SetDataDefault();
}

void CRebarHunchDlg::SetDataDefault()
{
	CRcBridgeRebar *pBri = m_pBri;
	if(!pBri) return;

	long nCountDia		= pBri->m_nQtyJigan+1;
	
	long cycle = 0; for(cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
	{
		CDoubleArray *pdArr = &pBri->m_dArrDiaUpperSlabHunch[cycle];
		pdArr->RemoveAll();
		long i = 0; for(i = 0; i < nCountDia; i++)
		{
			pdArr->Add(cycle == 0 ? 19 : 0);
		}
	}
	SetDataInit();
}
