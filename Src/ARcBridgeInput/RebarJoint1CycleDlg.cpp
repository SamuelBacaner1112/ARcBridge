// RebarJoint1CycleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "RebarJoint1CycleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRebarJoint1CycleDlg dialog


CRebarJoint1CycleDlg::CRebarJoint1CycleDlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CRebarJoint1CycleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRebarJoint1CycleDlg)
	//}}AFX_DATA_INIT
	m_pBri		= NULL;
	m_pStd		= NULL;
	m_nCycle	= -1;
	m_strTitleRib = _T("");
}
CRebarJoint1CycleDlg::CRebarJoint1CycleDlg(CRcBridgeRebar *pBri, CARcBridgeDataStd *pStd, long nCycle, CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CRebarJoint1CycleDlg::IDD, pParent)
{
	m_pBri		= pBri;
	m_pStd		= pStd;
	m_nCycle	= nCycle;
	m_strTitleRib = _T("");

}

void CRebarJoint1CycleDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRebarJoint1CycleDlg)
	DDX_Control(pDX, IDC_BUTTON_RECOMMAND, m_btnRecommand);
	DDX_Control(pDX, IDC_TAB_CYCLE, m_ctlTabCycle);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_GridControl(pDX, IDC_GRID_UPPERSLAB, m_GridUpperSlab);
	DDX_GridControl(pDX, IDC_GRID_OUTWALL, m_GridOutWall);
	DDX_GridControl(pDX, IDC_GRID_INWALL, m_GridInWall);
	DDX_GridControl(pDX, IDC_GRID_RIB, m_GridRib);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_STATIC_RIB, m_strTitleRib);
}


BEGIN_MESSAGE_MAP(CRebarJoint1CycleDlg, CXTResizeDialog)
	//{{AFX_MSG_MAP(CRebarJoint1CycleDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CYCLE, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_CYCLE, OnTabSelchanging)
	ON_BN_CLICKED(IDC_BUTTON_RECOMMAND, OnButtonRecommand)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID_OUTWALL, OnCellChangedDataOutWall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRebarJoint1CycleDlg message handlers

BOOL CRebarJoint1CycleDlg::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	CSize sz;
	sz.cx	= 10;
	sz.cy	= 10;
	this->SetMinSize(sz);
	
	// TODO: Add extra initialization here
	CString str = _T("");
	str.Format("철근이음 설정(CYCLE%d)", m_nCycle+1);
	this->SetWindowText(str);

	m_ctlTabCycle.RemoveAllTabs();
	m_ctlTabCycle.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_ctlTabCycle.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	
	SetResize(IDOK,			SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_RECOMMAND,		SZ_TOP_LEFT,		SZ_TOP_LEFT);
	

	InitTab();
	m_ctlTabCycle.SetCurSel(m_nCycle);

	InitGridManager();
	SetGridData();
	SetControl();
	SetDataInit();
	

	// 슬래브의 경우 입력창 사이즈를 축소해서 상부슬래브만 보이게 한다.
	if(m_pBri)
	{
		if(m_pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB)
		{
			CRect rect(0, 0, 0, 0);
			GetWindowRect(rect);
			SetWindowPos(&wndTop, rect.left, rect.top, rect.Width()/4, rect.Height(), 0);
			SendMessage(WM_SIZE);
		}
		else if(m_pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH && m_pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
		{
			CRect rect(0, 0, 0, 0);
			GetWindowRect(rect);
			SetWindowPos(&wndTop, rect.left, rect.top, rect.Width()/4*3, rect.Height(), 0);
			SendMessage(WM_SIZE);
		}
		else
		{
			m_strTitleRib = (m_pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)? _T("리브") : _T("하부슬래브");
		}

		m_pBri->SaveCountJointMainRebar();
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRebarJoint1CycleDlg::InitTab()
{
	m_ctlTabCycle.InsertItem(0, "1 CYCLE");
	m_ctlTabCycle.InsertItem(1, "2 CYCLE");
}

void CRebarJoint1CycleDlg::SetGridData()
{
	CRcBridgeRebar *pBri = m_pBri;
	if(!pBri) return;

	SetGridDataUpperSlab(m_nCycle);
	SetGridDataOutWall(m_nCycle);
	SetGridDataInWall(m_nCycle);
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		SetGridDataRib(m_nCycle);
	else if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
		SetGridDataLowerSlab(m_nCycle);
}

// 상부슬래브 1사이클
void CRebarJoint1CycleDlg::SetGridDataUpperSlab(long nCycle)
{
	BOOL bCycle13	= nCycle == 0 || nCycle == 2;
	// 그리드 초기화
	m_GridUpperSlab.SetGridDefault(2, 3, 1, 3);
	m_GridUpperSlab.SetCellSize(20, -1);

	CRcBridgeRebar *pBri = m_pBri;
	if(!pBri) return;

	long row = 0, col = 0;
	CString str = _T("");
	CString str1 = _T("");
	CString sDan	= _T("");
	BOOL bIsDetail	= IsDetailInput23Dan();

	m_GridUpperSlab.AddCell(row, col, "철근위치");
	m_GridUpperSlab.AddCell(row, col+1, "철근위치");
	m_GridUpperSlab.AddCell(row, col+2, "철근위치");
	m_GridUpperSlab.AddCell(row, col+3, "이음수");

	row	= 1;
	col	= 0;

	long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		if(dan > 0 && !bIsDetail) break;
		sDan.Format("%d단\n", dan+1);

		for(long upper = 0; upper < 2; upper++)
		{
			BOOL bUpper	= upper == iUPPER;

			long i = 0; for(i = 0; i < pBri->m_nQtyJigan; i++)
			{
				if(pBri->m_nQtyJigan == 1) 
				{
					str.Format("지간%2d\n", i+1);
					str1	= bUpper ? "상면" : "하면";
				}
				else 
				{
					str.Format("지간%2d", i+1);
					str1	= bUpper ? "상면\n" : "하면\n";
				}

				m_GridUpperSlab.AddCell(row, col, bIsDetail ? sDan : str1);
				m_GridUpperSlab.AddCell(row, col+1, str1);
				m_GridUpperSlab.AddCell(row, col+2, str);
				m_GridUpperSlab.AddCell(row++, col+3, bCycle13 ? &pBri->m_nCountJointMainRebar_UpperSlab_Cycle1[i][upper][dan] : &pBri->m_nCountJointMainRebar_UpperSlab_Cycle2[i][upper][dan]);
				m_GridUpperSlab.SetCellPrintZero(TRUE);
			}
		}
	}

	m_GridUpperSlab.GetCellCount(row, col);
	m_GridUpperSlab.SetRows(row);
	m_GridUpperSlab.SetCols(col);
	m_GridUpperSlab.SetColumnWidth(0, 30);
	m_GridUpperSlab.SetColumnWidth(1, 70);
	m_GridUpperSlab.SetColumnWidth(2, 60);
	m_GridUpperSlab.SetColumnWidth(3, 50);
}

// 외측벽체 1사이클
void CRebarJoint1CycleDlg::SetGridDataOutWall(long nCycle)
{
	BOOL bCycle13	= nCycle == 0 || nCycle == 2;
	// 그리드 초기화
	m_GridOutWall.SetGridDefault(2, 3, 1, 3);
	m_GridOutWall.SetCellSize(20, -1);

	CRcBridgeRebar *pBri = m_pBri;
	if(!pBri) return;
	if(!pBri->IsOutWall(TRUE) && !pBri->IsOutWall(FALSE)) return;

	long row = 0, col = 0;
	CString str = _T("");
	CString sDan	= _T("");
	BOOL bIsDetail	= IsDetailInput23Dan();

	m_GridOutWall.AddCell(row, col, "철근위치");
	m_GridOutWall.AddCell(row, col+1, "철근위치");
	m_GridOutWall.AddCell(row, col+2, "철근위치");
	m_GridOutWall.AddCell(row, col+3, "이음수");

	row	= 1;
	col	= 0;

	long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		if(dan > 0 && !bIsDetail) break;
		sDan.Format("%d단\n", dan+1);

		for(long stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			if(!pBri->IsOutWall(bStt)) continue;
			for(long inner = 0; inner < 2; inner++)
			{
				BOOL bInner	= inner == iINNER;
					
				

				m_GridOutWall.AddCell(row, col, bIsDetail ? sDan : (bStt ? "시점측\n" : "종점측\n"));
				m_GridOutWall.AddCell(row, col+1, bStt ? "시점측\n" : "종점측\n");
				m_GridOutWall.AddCell(row, col+2, bInner ? "내측" : "외측");
				m_GridOutWall.AddCell(row++, col+3, bCycle13 ? &pBri->m_nCountJointMainRebar_OutWall_Cycle1[stt][inner][dan] : &pBri->m_nCountJointMainRebar_OutWall_Cycle2[stt][inner][dan]);
				m_GridOutWall.SetCellPrintZero(TRUE);
			}
		}
	}

	m_GridOutWall.GetCellCount(row, col);
	m_GridOutWall.SetRows(row);
	m_GridOutWall.SetCols(col);
	m_GridOutWall.SetColumnWidth(0, 30);
	m_GridOutWall.SetColumnWidth(1, 70);
	m_GridOutWall.SetColumnWidth(2, 60);
	m_GridOutWall.SetColumnWidth(3, 50);
}

// 중간지점 1사이클
void CRebarJoint1CycleDlg::SetGridDataInWall(long nCycle)
{
	BOOL bCycle13	= nCycle == 0 || nCycle == 2;
	// 그리드 초기화
	m_GridInWall.SetGridDefault(2, 3, 1, 3);
	m_GridInWall.SetCellSize(20, -1);

	CRcBridgeRebar *pBri = m_pBri;
	if(!pBri) return;

	long row = 0, col = 0;
	CString str = _T("");
	CString sDan	= _T("");
	long nCountInWall	= pBri->GetCountInWall();
	BOOL bIsDetail	= IsDetailInput23Dan();

	m_GridInWall.AddCell(row, col, "철근위치");
	m_GridInWall.AddCell(row, col+1, "철근위치");
	m_GridInWall.AddCell(row, col+2, "철근위치");
	m_GridInWall.AddCell(row, col+3, "이음수");

	row	= 1;
	col	= 0;

	long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		if(dan > 0 && !bIsDetail) break;
		sDan.Format("%d단\n", dan+1);

		long wall = 0; for(wall = 0; wall < nCountInWall; wall++)
		{
			CWallApp *pWall	= pBri->GetInWall(wall);
			if(!pWall) continue;

			str.Format("중간지점%d\n", wall+1);

			for(long left = 0; left < 2; left++)
			{
				BOOL bLeft	= left == iLEFT;
				m_GridInWall.AddCell(row, col, bIsDetail ? sDan : str);
				m_GridInWall.AddCell(row, col+1, str);
				m_GridInWall.AddCell(row, col+2, bLeft ? "좌측" : "우측");
				m_GridInWall.AddCell(row++, col+3, bCycle13 ? &pWall->m_nCountJointMainRebar_Cycle1[left][dan] : &pWall->m_nCountJointMainRebar_Cycle2[left][dan]);
				m_GridInWall.SetCellPrintZero(TRUE);
			}
		}
	}

	m_GridInWall.GetCellCount(row, col);
	m_GridInWall.SetRows(row);
	m_GridInWall.SetCols(col);
	m_GridInWall.SetColumnWidth(0, 30);
	m_GridInWall.SetColumnWidth(1, 70);
	m_GridInWall.SetColumnWidth(2, 60);
	m_GridInWall.SetColumnWidth(3, 50);
}

void CRebarJoint1CycleDlg::InitGridManager()
{
	m_GridUpperSlab.InitGrid(m_pStd->m_Fy);
	m_GridOutWall.InitGrid(m_pStd->m_Fy);
	m_GridInWall.InitGrid(m_pStd->m_Fy);
	m_GridRib.InitGrid(m_pStd->m_Fy);
}

void CRebarJoint1CycleDlg::SetDataInit()
{
	SetDataInitControl();

	m_GridUpperSlab.UpdateData(FALSE);
	m_GridOutWall.UpdateData(FALSE);
	m_GridInWall.UpdateData(FALSE);
	m_GridRib.UpdateData(FALSE);

	// 머지를 멋지게 해준다
	long nRows	= 0;
	long nCols	= 0;
	m_GridUpperSlab.GetCellCount(nRows, nCols);
	m_GridUpperSlab.MergeGrid(0, nRows, 0, 2);

	m_GridOutWall.GetCellCount(nRows, nCols);
	m_GridOutWall.MergeGrid(0, nRows, 0, 2);

	m_GridInWall.GetCellCount(nRows, nCols);
	m_GridInWall.MergeGrid(0, nRows, 0, 2);
		
	m_GridRib.GetCellCount(nRows, nCols);
	m_GridRib.MergeGrid(0, nRows, 0, 1);
}

void CRebarJoint1CycleDlg::SetDataSave()
{
	SetDataSaveControl();

	m_GridUpperSlab.UpdateData(TRUE);
	m_GridOutWall.UpdateData(TRUE);
	m_GridInWall.UpdateData(TRUE);
	m_GridRib.UpdateData(TRUE);
}


void CRebarJoint1CycleDlg::SetControl()
{
}

// 컨트롤 정보 출력
void CRebarJoint1CycleDlg::SetDataInitControl()
{
	CRcBridgeRebar *pBri = m_pBri;
	if(!pBri) return;

	CString sCycle	= _T("");
	sCycle.Format("%d CYCLE", m_nCycle+1);
	SetWindowText(sCycle);
}

void CRebarJoint1CycleDlg::SetDataSaveControl()
{
	CRcBridgeRebar *pBri = m_pBri;
	if(!pBri) return;
}

BOOL CRebarJoint1CycleDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CXTResizeDialog::PreTranslateMessage(pMsg);
}

void CRebarJoint1CycleDlg::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	
	return;
}

void CRebarJoint1CycleDlg::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	m_nCycle	= m_ctlTabCycle.GetCurSel();
	SetGridData();
	SetDataInit();
	SetControl();
	return;
}

void CRebarJoint1CycleDlg::OnOK() 
{
	// TODO: Add extra validation here
	SetDataSave();
	CXTResizeDialog::OnOK();
}

void CRebarJoint1CycleDlg::OnButtonRecommand() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri = m_pBri;
	if(!pBri) return;

	if(AfxMessageBox("현재 입력 상자의 데이터가 모두 기본값으로 바뀝니다. 계속 하시겠습니까?  ", MB_YESNO | MB_ICONQUESTION)==IDNO)
	return;

	pBri->SetDataDefaultCountJoint(m_nCycle);

	SetGridData();
	SetDataInit();
}

void CRebarJoint1CycleDlg::SetGridDataRib(long nCycle)
{
	BOOL bCycle13	= nCycle == 0 || nCycle == 2;
	// 그리드 초기화
	m_GridRib.SetGridDefault(2, 2, 1, 2);
	m_GridRib.SetCellSize(20, -1);

	CRcBridgeRebar *pBri = m_pBri;
	if(!pBri) return;

	long row = 0, col = 0;
	CString str = _T("");
	
	m_GridRib.AddCell(row, col, "철근위치");
	m_GridRib.AddCell(row, col+1, "철근위치");
	m_GridRib.AddCell(row, col+2, "이음수");

	row	= 1;
	col	= 0;

	for(long upper = 0; upper < 2; upper++)
	{
		BOOL bUpper	= upper == iUPPER;
		
		long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			str.Format("%d단\n", dan+1);
			m_GridRib.AddCell(row, col, bUpper ? "상면\n" : "하면\n");
			m_GridRib.AddCell(row, col+1, str);
			m_GridRib.AddCell(row++, col+2, bCycle13 ? &pBri->m_nCountJointMainRebar_ArchRib_Cycle1[upper][dan] : &pBri->m_nCountJointMainRebar_ArchRib_Cycle2[upper][dan]);
			m_GridRib.SetCellPrintZero(TRUE);
		}
	}

	m_GridRib.GetCellCount(row, col);
	m_GridRib.SetRows(row);
	m_GridRib.SetCols(col);
	m_GridRib.SetColumnWidth(0, 100);
	m_GridRib.SetColumnWidth(1, 60);
	m_GridRib.SetColumnWidth(2, 50);
}

BOOL CRebarJoint1CycleDlg::IsDetailInput23Dan()
{
	if(!m_pBri) return FALSE;

	return m_pBri->m_bDetailInput23DanMainRebar;
}

void CRebarJoint1CycleDlg::SetGridDataLowerSlab( long nCycle )
{
	BOOL bCycle13	= nCycle == 0 || nCycle == 2;
	// 그리드 초기화
	m_GridRib.SetGridDefault(2, 3, 1, 3);
	m_GridRib.SetCellSize(20, -1);

	CRcBridgeRebar *pBri = m_pBri;
	if(!pBri) return;

	long row = 0, col = 0;
	CString str = _T("");
	CString str1 = _T("");
	CString sDan	= _T("");
	BOOL bIsDetail	= IsDetailInput23Dan();

	m_GridRib.AddCell(row, col, "철근위치");
	m_GridRib.AddCell(row, col+1, "철근위치");
	m_GridRib.AddCell(row, col+2, "철근위치");
	m_GridRib.AddCell(row, col+3, "이음수");

	row	= 1;
	col	= 0;

	long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		if(dan > 0 && !bIsDetail) break;
		sDan.Format("%d단\n", dan+1);

		for(long upper = 0; upper < 2; upper++)
		{
			BOOL bUpper	= upper == iUPPER;

			long i = 0; for(i = 0; i < pBri->m_nQtyJigan; i++)
			{
				if(pBri->m_nQtyJigan == 1) 
				{
					str.Format("지간%2d\n", i+1);
					str1	= bUpper ? "상면" : "하면";
				}
				else 
				{
					str.Format("지간%2d", i+1);
					str1	= bUpper ? "상면\n" : "하면\n";
				}

				m_GridRib.AddCell(row, col, bIsDetail ? sDan : str1);
				m_GridRib.AddCell(row, col+1, str1);
				m_GridRib.AddCell(row, col+2, str);
				m_GridRib.AddCell(row++, col+3, bCycle13 ? &pBri->m_nCountJointMainRebar_LowerSlab_Cycle1[i][upper][dan] : &pBri->m_nCountJointMainRebar_LowerSlab_Cycle2[i][upper][dan]);
				m_GridRib.SetCellPrintZero(TRUE);
			}
		}
	}

	m_GridRib.GetCellCount(row, col);
	m_GridRib.SetRows(row);
	m_GridRib.SetCols(col);
	m_GridRib.SetColumnWidth(0, 30);
	m_GridRib.SetColumnWidth(1, 70);
	m_GridRib.SetColumnWidth(2, 60);
	m_GridRib.SetColumnWidth(3, 50);
}

void CRebarJoint1CycleDlg::OnCellChangedDataOutWall( NMHDR* pNotifyStruct, LRESULT* result )
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;

	long nRow = pGridView->iRow;
	long nCol = pGridView->iColumn;

	CString sTitle = m_GridOutWall.GetTextMatrix(nRow, 2);
	long nCntJoint = m_GridOutWall.GetTextMatrixInt(nRow, nCol);
	if(sTitle == hggettext("외측") && nCntJoint <= 0)
	{
		SetDataInit();
		m_GridOutWall.SetFocusCell(nRow, nCol);
		m_GridOutWall.SetFocus();
	}
	SetDataSave();
}
