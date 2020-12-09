// TypeCycle3Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "TypeCycle3Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTypeCycle3Dlg dialog


CTypeCycle3Dlg::CTypeCycle3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTypeCycle3Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTypeCycle3Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pBri = NULL;
	m_pStd = NULL;
}

CTypeCycle3Dlg::CTypeCycle3Dlg(CRcBridgeRebar *pBri, CARcBridgeDataStd *pStd, CWnd* pParent /*=NULL*/)
	: CDialog(CTypeCycle3Dlg::IDD, pParent)
{
	m_pStd	= pStd;
	m_pBri	= pBri;
}

void CTypeCycle3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTypeCycle3Dlg)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_RECOMMOND, m_btnRecommond);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTypeCycle3Dlg, CDialog)
	//{{AFX_MSG_MAP(CTypeCycle3Dlg)
	ON_BN_CLICKED(IDC_RECOMMOND, OnRecommond)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTypeCycle3Dlg message handlers
void CTypeCycle3Dlg::SetGridData()
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 0, 1);
	m_Grid.SetCellSize(20, -1);

	if(!m_pBri) return;

	long row = 0, col = 0;
	CString str = _T("");
	UINT nFormat	= DT_RIGHT | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	CStringArray sArr;
	sArr.Add("�����Է�");
	sArr.Add("1Cycle�� ���� ��ġ");
	sArr.Add("�������� �������� 1/2 �̵�");
	sArr.Add("�������� �������� 1/2 �̵�");
	sArr.Add("�������� �������̸�ŭ �̵�");
	sArr.Add("�������� �������̸�ŭ �̵�");


	m_Grid.AddCell(row, col, "  ����ö��(��ν�������, ������ü ����ö��)", DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(row++, col+1, &m_pBri->m_nTypeDefaultCycle3_OutRebar, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);

	m_Grid.AddCell(row, col, "  ��ν����� �ϸ�ö��", DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(row++, col+1, &m_pBri->m_nTypeDefaultCycle3_UpperSlabLower, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);

	m_Grid.AddCell(row, col, "  ����ö��(������ü ����ö��)", DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(row++, col+1, &m_pBri->m_nTypeDefaultCycle3_OutWallInner, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);

	if(m_pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		m_Grid.AddCell(row, col, "  ��ġ����(���ϸ�)", DT_LEFT | DT_VCENTER);
		m_Grid.AddCell(row++, col+1, &m_pBri->m_nTypeDefaultCycle3_ArchRib, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
	}

	
	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidth(0, 280);
	m_Grid.SetColumnWidth(1, 200);
}

void CTypeCycle3Dlg::SetDataInit()
{
	if(!m_pBri) return;

	m_Grid.UpdateData(FALSE);
}

void CTypeCycle3Dlg::SetDataSave()
{
	if(!m_pBri) return;

	m_Grid.UpdateData(TRUE);

}

void CTypeCycle3Dlg::SetControl()
{

}

BOOL CTypeCycle3Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// ��ư ��Ÿ�� ����

	SetControl();
	InitGridManager();
	SetGridData();
	SetDataInit();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTypeCycle3Dlg::OnOK() 
{
	// TODO: Add extra validation here
	SetDataSave();
	
	CDialog::OnOK();
}

void CTypeCycle3Dlg::OnRecommond() 
{
	// TODO: Add your control notification handler code here
	if(!m_pBri) return;

	m_pBri->m_nTypeDefaultCycle3_OutRebar		= 2;
	m_pBri->m_nTypeDefaultCycle3_UpperSlabLower	= 2;
	m_pBri->m_nTypeDefaultCycle3_OutWallInner	= 2;
	SetGridData();
	SetDataInit();
}

void CTypeCycle3Dlg::InitGridManager()
{
	m_Grid.InitGrid(m_pStd->m_Fy);
}

BOOL CTypeCycle3Dlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_Grid.ViewGridCellForCombo(pMsg);
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CTypeCycle3Dlg::OnCancel() 
{
	// TODO: Add extra cleanup here
		
	CDialog::OnCancel();
}
