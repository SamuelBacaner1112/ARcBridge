// EarthquakeFactKDS17Dlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "EarthquakeFactKDS17Dlg.h"


// CEarthquakeFactKDS17Dlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CEarthquakeFactKDS17Dlg, CDialog)

CEarthquakeFactKDS17Dlg::CEarthquakeFactKDS17Dlg(CWnd* pParent /*=NULL*/)
	: CUserDialog(CEarthquakeFactKDS17Dlg::IDD, pParent)
{
	m_nOption = 0;
}

CEarthquakeFactKDS17Dlg::~CEarthquakeFactKDS17Dlg()
{
}

void CEarthquakeFactKDS17Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CEarthquakeFactKDS17Dlg, CDialog)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID, OnCellChangedRow)
END_MESSAGE_MAP()

BOOL CEarthquakeFactKDS17Dlg::OnInitDialog()
{
	CUserDialog::OnInitDialog();

	CString strName;
	for(long ix = 0; ix < 6; ++ix)
	{
		strName.Format(_T("S%d"), ix+1);
		if(m_ReturnValue == strName) 
			m_nOption = ix;
	}	

	m_Grid.SetFocusCell(0, 0);
	SetGridData();
	SetDataInit();

	return TRUE;
}

void CEarthquakeFactKDS17Dlg::SetGridData()
{
	m_Grid.SetGridDefault(2, 2, 2, 0);
	m_Grid.SetEditable(FALSE);

	CString str;
	long row = 0;
	long col = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;

	m_Grid.AddCell(row, col++, _T("���ݺз�"));
	m_Grid.AddCell(row, col++, _T("���������� ȣĪ"));
	m_Grid.AddCell(row, col++, _T("�з�����"));
	m_Grid.AddCell(row, col++, _T("�з�����"));
	row++, col = 0;

	m_Grid.AddCell(row, col++, _T("���ݺз�"));
	m_Grid.AddCell(row, col++, _T("���������� ȣĪ"));
	m_Grid.AddCell(row, col++, _T("��ݾ� ����, H (m)"));
	m_Grid.AddCell(row, col++, _T("������������ļӵ�, Vs,soil (m/s)"));
	row++, col = 0;

	m_Grid.AddCell(row++, col, &m_nOption, nFormat, TRUE, nLine, _T("S1"), CELL_TYPE_RADIO);
	m_Grid.AddCell(row++, col, &m_nOption, nFormat, TRUE, nLine, _T("S2"), CELL_TYPE_RADIO);
	m_Grid.AddCell(row++, col, &m_nOption, nFormat, TRUE, nLine, _T("S3"), CELL_TYPE_RADIO);
	m_Grid.AddCell(row++, col, &m_nOption, nFormat, TRUE, nLine, _T("S4"), CELL_TYPE_RADIO);
	m_Grid.AddCell(row++, col, &m_nOption, nFormat, TRUE, nLine, _T("S5"), CELL_TYPE_RADIO);
	m_Grid.AddCell(row++, col, &m_nOption, nFormat, TRUE, CELL_READONLY, _T("S6"), CELL_TYPE_RADIO);
	m_Grid.SetLastCellRangeOfRadio(2, 0, row-1, 0);
	row = 2, col = 1;
	
	m_Grid.AddCell(row, col++, _T("�Ϲ�����")); 
	m_Grid.AddCell(row, col++, _T("1 �̸�"));
	m_Grid.AddCell(row, col++, _T("-"));
	row++, col = 1;
	
	m_Grid.AddCell(row, col++, _T("��� �ܴ��� ����"));
	m_Grid.AddCell(row, col++, _T("1~20 ����"));
	m_Grid.AddCell(row, col++, _T("260 �̻�"));
	row++, col = 1;
	
	m_Grid.AddCell(row, col++, _T("��� ������ ����"));
	m_Grid.AddCell(row, col++, _T("1~20 ����"));
	m_Grid.AddCell(row, col++, _T("260 �̸�"));
	row++, col = 1;
		
	m_Grid.AddCell(row, col++, _T("��� �ܴ��� ����"));
	m_Grid.AddCell(row, col++, _T("20 �ʰ�"));
	m_Grid.AddCell(row, col++, _T("180 �̻�"));
	row++, col = 1;

	
	m_Grid.AddCell(row, col++, _T("��� ������ ����"));
	m_Grid.AddCell(row, col++, _T("20 �ʰ�"));
	m_Grid.AddCell(row, col++, _T("180 �̸�"));
	row++, col = 1;

	str = _T("���� ������ Ư���� �� ���������ؼ��� �ʿ��� ����");
	m_Grid.AddCell(row, col++, str, nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, str, nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, str, nFormat, CELL_READONLY);

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(300);
	m_Grid.SetColumnWidth(0, 60);
	m_Grid.SetColumnWidth(1, 120);
	m_Grid.SetColumnWidth(2, 120);
	m_Grid.SetColumnWidth(3, 230);
	m_Grid.SetRowHeightAll(22);
}

void CEarthquakeFactKDS17Dlg::SetDataSave()
{
	m_Grid.UpdateData(TRUE);
}

void CEarthquakeFactKDS17Dlg::SetDataInit()
{
	m_Grid.MergeGridEx(0, 1, 0, 0);
	m_Grid.MergeGridEx(0, 1, 1, 1);
	m_Grid.MergeGridEx(0, 0, 2, 3);
	m_Grid.MergeGridEx(7, 7, 1, 3);

	m_Grid.MergeGridEx(3, 4, 2, 2);
	m_Grid.MergeGridEx(5, 6, 2, 2);

	m_Grid.UpdateData(FALSE);
}

void CEarthquakeFactKDS17Dlg::OnOK() 
{
	SetDataSave();
	m_ReturnValue.Format(_T("S%d"), m_nOption+1);

	CUserDialog::OnOK();
}

void CEarthquakeFactKDS17Dlg::OnCellChangedRow( NMHDR* pNotifyStruct, LRESULT* result )
{
	long nCurRow = m_Grid.GetCurrentCell()->nRow;
	
	if(nCurRow == 7) return; //S6 ��������.

	SetGridData();
	SetDataInit();
}
