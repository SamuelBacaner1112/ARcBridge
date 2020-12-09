// DialogRailLoadAlways.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ARcBridgeInput.h"


// CDialogRailLoadAlways ��ȭ �����Դϴ�.

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

// CDialogRailLoadAlways �޽��� ó�����Դϴ�.
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

	m_Grid.AddCell(nRow, nCol++, "H1 \n(ħ�����)");			// ħ�����
	m_Grid.AddCell(nRow, nCol++, "BT \n(ħ�����)");	// ħ�����
	m_Grid.AddCell(nRow, nCol++, "LT \n(ħ����)");	// ħ����
	m_Grid.AddCell(nRow, nCol++, "D1 \n(�ణ�Ÿ�)");	// �ణ�Ÿ�
	m_Grid.AddCell(nRow++, nCol, "D2 \n(�˵��߽ɰ� �Ÿ�)");	// �˵� �߽ɰ� �Ÿ�(����)

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
	if(AfxMessageBox("���� �Է� ������ �����Ͱ� ��� �⺻������ �ٲ�ϴ�. ��� �Ͻðڽ��ϱ�?  ", MB_YESNO | MB_ICONQUESTION)==IDNO)
		return;

	SetDataDefault();
	SetDataInit();
}
