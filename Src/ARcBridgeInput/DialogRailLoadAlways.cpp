// DialogRailLoadAlways.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ARcBridgeInput.h"


// CDialogRailLoadAlways 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogRailLoadAlways, CUserDialog)

CDialogRailLoadAlways::CDialogRailLoadAlways(CRcBridgeApp *pBri, CWnd* pParent /*=NULL*/)
	: CUserDialog(CDialogRailLoadAlways::IDD, pParent)
{
	m_pBri = pBri;
}

CDialogRailLoadAlways::~CDialogRailLoadAlways()
{
}

void CDialogRailLoadAlways::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CDialogRailLoadAlways, CUserDialog)
	ON_BN_CLICKED(IDC_BUTTON_RECOMMAND, OnButtonDefault)
END_MESSAGE_MAP()

// CDialogRailLoadAlways 메시지 처리기입니다.
BOOL CDialogRailLoadAlways::OnInitDialog()
{
	CUserDialog::OnInitDialog();

	SetGridData();
	SetDataInit();

	return TRUE;
}

void CDialogRailLoadAlways::SetGridData()
{
	m_Grid.SetGridDefault(2, 5, 1, 0);
	
	long nRow = 0, nCol = 0;

	m_Grid.AddCell(nRow, nCol++, "H1 \n(침목높이)");			// 침목높이
	m_Grid.AddCell(nRow, nCol++, "BT \n(침목길이)");	// 침목길이
	m_Grid.AddCell(nRow, nCol++, "LT \n(침목폭)");	// 침목폭
	m_Grid.AddCell(nRow, nCol++, "D1 \n(축간거리)");	// 축간거리
	m_Grid.AddCell(nRow++, nCol, "D2 \n(궤도중심간 거리)");	// 궤도 중심간 거리(복선)

	nCol = 0;
	m_Grid.AddCell(nRow, nCol++, &m_pBri->m_dRail_HeightTie);
	m_Grid.AddCell(nRow, nCol++, &m_pBri->m_dRail_LengthTie);
	m_Grid.AddCell(nRow, nCol++, &m_pBri->m_dRail_WidthTie);
	m_Grid.AddCell(nRow, nCol++, &m_pBri->m_dRail_DistBetweenWheel);
	m_Grid.AddCell(nRow, nCol++, &m_pBri->m_dRail_DistBetweenRail);

	m_Grid.GetCellCount(nRow, nCol);
	m_Grid.SetRows(nRow);
	m_Grid.SetCols(nCol);
	m_Grid.SetColumnWidthAll(80);
	m_Grid.SetRowHeight(0, 35);
	m_Grid.SetColumnWidth(4, 120);

}

void CDialogRailLoadAlways::SetDataSave()
{
	m_Grid.UpdateData(TRUE);
}

void CDialogRailLoadAlways::SetDataInit()
{
	m_Grid.UpdateData(FALSE);
}

void CDialogRailLoadAlways::OnOK()
{
	SetDataSave();

	CUserDialog::OnOK();
}

void CDialogRailLoadAlways::SetDataDefault()
{
	m_pBri->SetDataDefaultHDanRailLive();	
}

void CDialogRailLoadAlways::OnButtonDefault()
{
	if(AfxMessageBox("현재 입력 상자의 데이터가 모두 기본값으로 바뀝니다. 계속 하시겠습니까?  ", MB_YESNO | MB_ICONQUESTION)==IDNO)
		return;

	SetDataDefault();
	SetDataInit();
}
