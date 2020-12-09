// RebarJointDlg.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "RebarJointDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRebarJointDlg dialog


CRebarJointDlg::CRebarJointDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRebarJointDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRebarJointDlg)
	//}}AFX_DATA_INIT
	m_pBri		= NULL;
	m_pStd		= NULL;
	m_nCycle	= -1;
	m_bInputJointSuOf2Cycle	= FALSE;
}
CRebarJointDlg::CRebarJointDlg(CRcBridgeRebar *pBri, CARcBridgeDataStd *pStd, long nCycle, CWnd* pParent /*=NULL*/)
	: CDialog(CRebarJointDlg::IDD, pParent)
{
	m_pBri		= pBri;
	m_pStd		= pStd;
	m_nCycle	= nCycle;
	m_bInputJointSuOf2Cycle	= FALSE;
}

void CRebarJointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRebarJointDlg)
	DDX_Control(pDX, IDC_CHECK_DANBU_BENDING, m_checkDanbuBending);
	DDX_Control(pDX, IDC_CHECK_STTWALL_INNER, m_checkSttWallInner);
	DDX_Control(pDX, IDC_CHECK_INWALL_INNER, m_checkInWallInner);
	DDX_Control(pDX, IDC_CHECK_ENDWALL_INNER, m_checkEndWallInner);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_TAB_UPPERSLAB, m_ctlTabUpperSlab);
	DDX_Control(pDX, IDC_TAB_OUTWALL, m_ctlTabOutWall);
	DDX_GridControl(pDX, IDC_GRID_UPPERSLAB, m_GridUpperSlab);
	DDX_GridControl(pDX, IDC_GRID_OUTWALL, m_GridOutWall);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRebarJointDlg, CDialog)
	//{{AFX_MSG_MAP(CRebarJointDlg)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRebarJointDlg message handlers

BOOL CRebarJointDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString str = _T("");
	str.Format("철근이음 설정(CYCLE%d)", m_nCycle+1);
	this->SetWindowText(str);

	m_ctlTabUpperSlab.RemoveAllTabs();
	m_ctlTabUpperSlab.SendInitialUpdate(TRUE);
	m_ctlTabUpperSlab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	m_ctlTabOutWall.RemoveAllTabs();
	m_ctlTabOutWall.SendInitialUpdate(TRUE);
	m_ctlTabOutWall.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	


	InitTab();
	InitGridManager();
	SetGridData();
	SetControl();
	SetDataInit();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRebarJointDlg::InitTab()
{
	m_ctlTabUpperSlab.InsertItem(0, "상부슬래브");
	m_ctlTabOutWall.InsertItem(0, "외측벽체");
}

void CRebarJointDlg::SetGridData()
{
	if(m_nCycle == 0 || m_nCycle == 2)
	{
		SetGridDataUpperSlab_Cycle1();
		SetGridDataOutWall_Cycle1();
	}
	else if(m_nCycle == 1 || m_nCycle == 3)
	{
		SetGridDataUpperSlab_Cycle2();
		SetGridDataOutWall_Cycle2();
	}
}

// 상부슬래브 1사이클
void CRebarJointDlg::SetGridDataUpperSlab_Cycle1()
{
	// 그리드 초기화
	m_GridUpperSlab.SetGridDefault(2, 2, 1, 1);
	m_GridUpperSlab.SetCellSize(20, -1);

	CRcBridgeRebar *pBri = m_pBri;
	if(!pBri) return;

	long row = 0, col = 0;
	CString str = _T("");

	m_GridUpperSlab.AddCell(row, col, "철근위치");
	m_GridUpperSlab.AddCell(row, col+1, "이음수");

	row	= 1;
	col	= 0;
	// 상면
	long i = 0; for(i = 0; i < pBri->m_nQtyJigan; i++)
	{
		str.Format("상면 : 지간%2d", i+1);
		m_GridUpperSlab.AddCell(row, col, str);
		m_GridUpperSlab.AddCell(row++, col+1, &pBri->m_nCountJointMainRebar_UpperSlab_Cycle1[i][iUPPER][0]);
	}

	// 하면
	col = 0;
	for(i = 0; i < pBri->m_nQtyJigan; i++)
	{
		str.Format("하면 : 지간%2d", i+1);
		m_GridUpperSlab.AddCell(row, col, str);
		m_GridUpperSlab.AddCell(row++, col+1, &pBri->m_nCountJointMainRebar_UpperSlab_Cycle1[i][iLOWER][0]);
	}

	m_GridUpperSlab.GetCellCount(row, col);
	m_GridUpperSlab.SetRows(row);
	m_GridUpperSlab.SetCols(col);
	m_GridUpperSlab.SetColumnWidth(0, 150);
	m_GridUpperSlab.SetColumnWidth(1, 100);
	
}

// 외측벽체 1사이클
void CRebarJointDlg::SetGridDataOutWall_Cycle1()
{
	// 그리드 초기화
	m_GridOutWall.SetGridDefault(2, 2, 1, 1);
	m_GridOutWall.SetCellSize(20, -1);

	CRcBridgeRebar *pBri = m_pBri;
	if(!pBri) return;
	BOOL bNoOutWall	= !pBri->IsOutWall(TRUE) && !pBri->IsOutWall(FALSE);
	if(bNoOutWall) return;

	long row = 0, col = 0;
	CString str = _T("");

	m_GridOutWall.AddCell(row, col, "철근위치");
	m_GridOutWall.AddCell(row, col+1, "이음수");

	row	= 1;
	col	= 0;

	m_GridOutWall.AddCell(row, col, "시점측 : 외측면");
	m_GridOutWall.AddCell(row++, col+1, &pBri->m_nCountJointMainRebar_OutWall_Cycle1[iSTT][iOUTTER][0]);
	m_GridOutWall.AddCell(row, col, "시점측 : 내측면");
	m_GridOutWall.AddCell(row++, col+1, &pBri->m_nCountJointMainRebar_OutWall_Cycle1[iSTT][iINNER][0]);
	m_GridOutWall.AddCell(row, col, "종점측 : 외측면");
	m_GridOutWall.AddCell(row++, col+1, &pBri->m_nCountJointMainRebar_OutWall_Cycle1[iEND][iOUTTER][0]);
	m_GridOutWall.AddCell(row, col, "종점측 : 내측면");
	m_GridOutWall.AddCell(row++, col+1, &pBri->m_nCountJointMainRebar_OutWall_Cycle1[iEND][iINNER][0]);


	m_GridOutWall.GetCellCount(row, col);
	m_GridOutWall.SetRows(row);
	m_GridOutWall.SetCols(col);
	m_GridOutWall.SetColumnWidth(0, 150);
	m_GridOutWall.SetColumnWidth(1, 100);
}

void CRebarJointDlg::InitGridManager()
{
	m_GridUpperSlab.InitGrid(m_pStd->m_Fy);
	m_GridOutWall.InitGrid(m_pStd->m_Fy);
}

void CRebarJointDlg::SetDataInit()
{
	SetDataInitControl();

	m_GridUpperSlab.UpdateData(FALSE);
	m_GridOutWall.UpdateData(FALSE);
}

void CRebarJointDlg::OnCancel()
{
	CDialog::OnCancel();
}

void CRebarJointDlg::OnOK()
{
	SetDataSave();
	CDialog::OnOK();
}

void CRebarJointDlg::SetDataSave()
{
	SetDataSaveControl();

	m_GridUpperSlab.UpdateData(TRUE);
	m_GridOutWall.UpdateData(TRUE);
}

// CYCLE2 : 외측벽체 철근연결, 이음수 설정
void CRebarJointDlg::SetGridDataOutWall_Cycle2()
{
	// 그리드 초기화
	m_GridOutWall.SetGridDefault(2, 2, 1, 1);
	m_GridOutWall.SetCellSize(20, -1);

	CRcBridgeRebar *pBri = m_pBri;
	if(!pBri) return;
	BOOL bNoOutWall	= !pBri->IsOutWall(TRUE) && !pBri->IsOutWall(FALSE);
	if(bNoOutWall) 
	{
		m_GridOutWall.SetGridDefault(0, 0, 0, 0);
		return;
	}

	long row = 0, col = 0;
	CString str		= _T("");
	CString sHelp	= _T("");
	CString sStt	= _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	m_GridOutWall.AddCell(row, col, "철근위치");
	m_GridOutWall.AddCell(row, col+1, "연결");
	if(m_bInputJointSuOf2Cycle)
		m_GridOutWall.AddCell(row, col+2, "이음수");

	row	= 1;
	col	= 0;

	// 시점측 벽체	
	for(long stt = 0; stt < 2; stt++)
	{
		if(stt == iSTT) sStt = _T("시점측");
		else sStt = _T("종점측");

		// 외측면
		CSetRebarJoint *pSetRebarJoint = &pBri->m_pArrSetRebarJointOutWall_Outter[stt];
		str.Format("%s : 외측면", sStt);
		m_GridOutWall.AddCell(row, col, str);
		m_GridOutWall.AddCell(row, col+1, &pSetRebarJoint->m_bJoint, nFormat, nLine, sHelp, CELL_TYPE_CHECK);
		if(m_bInputJointSuOf2Cycle)
			m_GridOutWall.AddCell(row, col+2, &pSetRebarJoint->m_nCountJoint, nFormat, FALSE, nLine, sHelp);
		row++;

		// 내측면
		for(long upper = 0; upper < 2; upper++)
		{
			pSetRebarJoint = &pBri->m_pArrSetRebarJointOutWall_Inner[stt][upper];
			
			if(upper == 0) str.Format("%s : 내측상단~상부슬래브", sStt);
			else 
			{
				if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
					str.Format("%s : 내측하단~하부슬래브", sStt);
				else
					str.Format("%s : 내측하단~기초", sStt);

			}
			m_GridOutWall.AddCell(row, col, str);
			m_GridOutWall.AddCell(row, col+1, &pSetRebarJoint->m_bJoint, nFormat, nLine, sHelp, CELL_TYPE_CHECK);
			if(m_bInputJointSuOf2Cycle)
				m_GridOutWall.AddCell(row, col+2, &pSetRebarJoint->m_nCountJoint, nFormat, FALSE, nLine, sHelp);
			row++;
		}
	}

	m_GridOutWall.GetCellCount(row, col);
	m_GridOutWall.SetRows(row);
	m_GridOutWall.SetCols(col);
	m_GridOutWall.SetColumnWidth(0, 170);
	m_GridOutWall.SetColumnWidth(1, 50);
	if(m_bInputJointSuOf2Cycle)
		m_GridOutWall.SetColumnWidth(2, 55);
}

// 2cycle이음입력은 작없이 완료되면 풀 예정
void CRebarJointDlg::SetGridDataUpperSlab_Cycle2()
{
	// 그리드 초기화
	m_GridUpperSlab.SetGridDefault(2, 2, 1, 1);
	m_GridUpperSlab.SetCellSize(20, -1);

	CRcBridgeRebar *pBri = m_pBri;
	if(!pBri) return;

	long row = 0, col = 0;
	CString str		= _T("");
	CString sHelp	= _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	m_GridUpperSlab.AddCell(row, col, "철근위치");
	m_GridUpperSlab.AddCell(row, col+1, "연결");
	if(m_bInputJointSuOf2Cycle)
		m_GridUpperSlab.AddCell(row, col+2, "이음수");

	// 필요한 만큼 이음설정 변수 생성
	long nIdxSetRebarJoint				= 0;
	long nCountNeedSetRebarJointupper	= pBri->m_nQtyJigan;
	long nCountNeedSetRebarJointLower	= (pBri->m_nQtyJigan+1);
	AhTPAMake(nCountNeedSetRebarJointupper, &pBri->m_pArrSetRebarJointUpperSlab[iUPPER], (CSetRebarJoint*)0);
	AhTPAMake(nCountNeedSetRebarJointLower, &pBri->m_pArrSetRebarJointUpperSlab[iLOWER], (CSetRebarJoint*)0);

	row	= 1;
	col	= 0;
	// 상면
	nIdxSetRebarJoint = 0;
	long i = 0; for(i = 0; i < pBri->m_nQtyJigan; i++)
	{
		CSetRebarJoint *pSetRebarJoint = pBri->m_pArrSetRebarJointUpperSlab[iUPPER].GetAt(nIdxSetRebarJoint++);
		str.Format("상면 : 지간%2d", i+1);
		m_GridUpperSlab.AddCell(row, col, str);
		m_GridUpperSlab.AddCell(row, col+1, &pSetRebarJoint->m_bJoint, nFormat, nLine, sHelp, CELL_TYPE_CHECK);
		if(m_bInputJointSuOf2Cycle)
			m_GridUpperSlab.AddCell(row, col+2, &pSetRebarJoint->m_nCountJoint, nFormat, FALSE, nLine, sHelp);
		row++;
	}

	// 하면
	col = 0;
	nIdxSetRebarJoint = 0;
	for(i = 0; i < pBri->m_nQtyJigan+1; i++)
	{
		CSetRebarJoint *pSetRebarJoint = pBri->m_pArrSetRebarJointUpperSlab[iLOWER].GetAt(nIdxSetRebarJoint++);
		if(i == 0) str.Format("하면 : 시점측벽체~지간1");
		else if(i == pBri->m_nQtyJigan) str.Format("하면 : 지간%d~종점측벽체", pBri->m_nQtyJigan);
		else str.Format("하면 : 지간%d~지간%d", i+1, i+2);
		m_GridUpperSlab.AddCell(row, col, str);
		m_GridUpperSlab.AddCell(row, col+1, &pSetRebarJoint->m_bJoint, nFormat, nLine, sHelp, CELL_TYPE_CHECK);
		if(m_bInputJointSuOf2Cycle)
			m_GridUpperSlab.AddCell(row, col+2, &pSetRebarJoint->m_nCountJoint, nFormat, FALSE, nLine, sHelp); 
		row++;
	}

	m_GridUpperSlab.GetCellCount(row, col);
	m_GridUpperSlab.SetRows(row);
	m_GridUpperSlab.SetCols(col);
	m_GridUpperSlab.SetColumnWidth(0, 170);
	m_GridUpperSlab.SetColumnWidth(1, 50);
	if(m_bInputJointSuOf2Cycle)
		m_GridUpperSlab.SetColumnWidth(2, 55);

}

void CRebarJointDlg::SetControl()
{
	if(m_nCycle == 0 || m_nCycle == 2)
	{
		m_checkSttWallInner.ShowWindow(FALSE);
		m_checkEndWallInner.ShowWindow(FALSE);
		m_checkInWallInner.ShowWindow(FALSE);
		m_checkDanbuBending.ShowWindow(FALSE);
	}
	else
	{
		m_checkSttWallInner.ShowWindow(TRUE);
		m_checkEndWallInner.ShowWindow(TRUE);
		m_checkInWallInner.ShowWindow(TRUE);
		m_checkDanbuBending.ShowWindow(TRUE);
	}
}

// 컨트롤 정보 출력
void CRebarJointDlg::SetDataInitControl()
{
	CRcBridgeRebar *pBri = m_pBri;
	if(!pBri) return;

	m_checkSttWallInner.SetCheck(pBri->m_bPlacingOutWallInnerRebar[iSTT]);
	m_checkEndWallInner.SetCheck(pBri->m_bPlacingOutWallInnerRebar[iEND]);
	m_checkInWallInner.SetCheck(pBri->m_bPlacingInWallRebarOfCycle24);
	m_checkDanbuBending.SetCheck(pBri->m_bBendingDanBuRebar);
}

void CRebarJointDlg::SetDataSaveControl()
{
	CRcBridgeRebar *pBri = m_pBri;
	if(!pBri) return;

	pBri->m_bPlacingOutWallInnerRebar[iSTT]	= m_checkSttWallInner.GetCheck();
	pBri->m_bPlacingOutWallInnerRebar[iEND]	= m_checkEndWallInner.GetCheck();
	pBri->m_bPlacingInWallRebarOfCycle24	= m_checkInWallInner.GetCheck();
	pBri->m_bBendingDanBuRebar				= m_checkDanbuBending.GetCheck();
}
