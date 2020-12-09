// RoundDlg.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "RoundDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRoundDlg dialog


CRoundDlg::CRoundDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRoundDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRoundDlg)
	//}}AFX_DATA_INIT
	m_pColumn	= NULL;
	m_pStd		= NULL;

	m_pParent = NULL;
}

CRoundDlg::CRoundDlg(CColumnApp *pColumn, CARcBridgeDataStd *pStd, CInputDialogBase* pParent)
	: CDialog(CRoundDlg::IDD, pParent)
{
	m_pColumn	= pColumn;
	m_pStd		= pStd;
	m_pParent	= pParent;
}

void CRoundDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRoundDlg)
	DDX_Control(pDX, IDC_COMBO_ROUND_OUT, m_comboRoundOut);
	DDX_Control(pDX, IDC_COMBO_ROUND_IN, m_comboRoundIn);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDC_BUTTON_RECOMMAND, m_btnRecommand);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRoundDlg, CDialog)
	//{{AFX_MSG_MAP(CRoundDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_ROUND_IN, OnSelchangeComboRoundIn)
	ON_CBN_SELCHANGE(IDC_COMBO_ROUND_OUT, OnSelchangeComboRoundOut)
	ON_BN_CLICKED(IDC_BUTTON_RECOMMAND, OnButtonRecommand)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRoundDlg message handlers

void CRoundDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CRoundDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_Grid.UpdateData(TRUE);

	
	CDialog::OnOK();
}

void CRoundDlg::OnSelchangeComboRound() 
{
	// TODO: Add your control notification handler code here
	
}



void CRoundDlg::SetGridData()
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 2, 1);
	m_Grid.SetCellSize(20, 50);

	if(!m_pColumn) return;
	
	long row = 0, col = 0;
	CString str = _T(""), sHelp = _T("");
	CStringArray sArr;
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	CString sInner	= _T("");
	CString strTmp	= _T("");
	m_Grid.AddCell(row, col, "����\n");
	m_Grid.AddCell(row+1, col, "����\n");
	m_Grid.AddCell(row+2, col, "����");
	m_Grid.AddCell(row+3, col, "����");
	
	long nCountDanRebarMain	= m_pColumn->GetCountDan_RebarMain();
	for(long inner = 0; inner < 2; inner++)
	{
		BOOL bOutter	= (inner == iOUTTER);
		long dan		= bOutter ? 0 : 1;
		if(dan >= nCountDanRebarMain) continue;

		CPlaceRebarByBlock *pPlace	= &m_pColumn->m_placeRebarRound[iOUTTER][dan];
		sHelp			= bOutter ? "������ö������(1��ö�� ������ �ش�)" : "������ö������(2,3�� ����ö�ٰ�, 1,2,3�� ����ö�ٿ� �ش�)";
		strTmp			= bOutter ? "H" : "h";
		sInner			= bOutter ? "������ö��" : "������ö��";

		col++;
		m_Grid.AddCell(row, col, sInner);
		m_Grid.AddCell(row+1, col, bOutter ? "R1" : "RI1");
		m_Grid.AddCell(row+2, col, &pPlace->m_dDia, nFormat, nLine, sHelp, CELL_TYPE_DIA); if(!bOutter) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);

		for(long block = 0; block < pPlace->m_nCountBlock; block++)
		{
			sHelp.Format("%s %d��° ����", sInner, block+1);
			col++;
			str.Format("%s%d", strTmp, block+1);
			m_Grid.AddCell(row, col, sInner);
			m_Grid.AddCell(row+1, col, str); 
			m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine, sHelp+" ����"); m_Grid.SetCellDec(0); if(!bOutter) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
			m_Grid.AddCell(row+3, col, &pPlace->m_xyArr, block, FALSE, nFormat, nLine, sHelp+" ����"); if(!bOutter) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
		}
	}

	col++;

	// ũ�ν�Ÿ�� ����
	m_Grid.AddCell(row, col, "ũ�ν�Ÿ��");
	m_Grid.AddCell(row+1, col, "CT1");
	m_Grid.AddCell(row+2, col, &m_pColumn->m_dDiaCrossTie, nFormat, nLine, "ũ�ν�Ÿ�� ����", CELL_TYPE_DIA);
	col++;

	// �����ġ
	m_Grid.AddCell(row, col, "ũ�ν�Ÿ��");
	m_Grid.AddCell(row+1, col, "CTH1");
	m_Grid.AddCell(row+2, col, "1", nFormat, CELL_READONLY);
	m_Grid.AddCell(row+3, col, &m_pColumn->m_dCTH1, nFormat, nLine, "ũ�ν�Ÿ�� �����ġ ù��° ���� ����");
	col++;

	m_Grid.AddCell(row, col, "ũ�ν�Ÿ��");
	m_Grid.AddCell(row+1, col, "HN");
	m_Grid.AddCell(row+2, col, &m_pColumn->m_nCTH, nFormat, FALSE, nLine, "ũ�ν�Ÿ�� �����ġ �ι�° ���� ����");
	m_Grid.AddCell(row+3, col, &m_pColumn->m_dCTH, nFormat, nLine, "ũ�ν�Ÿ�� �����ġ �ι�° ���� ����");
	col++;

	m_Grid.AddCell(row, col, "ũ�ν�Ÿ��");
	m_Grid.AddCell(row+1, col, "CTH");
	m_Grid.AddCell(row+2, col, "1", nFormat, CELL_READONLY);
	m_Grid.AddCell(row+3, col, &m_pColumn->m_dCTH2, nFormat, nLine, "ũ�ν�Ÿ�� �����ġ ������ ���� ����");
	col++;

	// ������ġ
	m_Grid.AddCell(row, col, "ũ�ν�Ÿ��");
	m_Grid.AddCell(row+1, col, "CTV1");
	m_Grid.AddCell(row+2, col, "1", nFormat, CELL_READONLY);
	m_Grid.AddCell(row+3, col, &m_pColumn->m_dCTV1, nFormat, nLine, "ũ�ν�Ÿ�� ������ġ ù��° ���� ����");
	col++;

	m_Grid.AddCell(row, col, "ũ�ν�Ÿ��");
	m_Grid.AddCell(row+1, col, "VN");
	m_Grid.AddCell(row+2, col, &m_pColumn->m_nCTV, nFormat, FALSE, nLine, "ũ�ν�Ÿ�� ������ġ �ι�° ���� ����");
	m_Grid.AddCell(row+3, col, &m_pColumn->m_dCTV, nFormat, nLine, "ũ�ν�Ÿ�� ������ġ �ι�° ���� ����");
	col++;

	m_Grid.AddCell(row, col, "ũ�ν�Ÿ��");
	m_Grid.AddCell(row+1, col, "CTV");
	m_Grid.AddCell(row+2, col, "1", nFormat, CELL_READONLY);
	m_Grid.AddCell(row+3, col, &m_pColumn->m_dCTV2, nFormat, nLine, "ũ�ν�Ÿ�� ������ġ ������ ���� ����");
	col++;


}

void CRoundDlg::SetDataInit()
{
	if(!m_pColumn) return;

	m_Grid.UpdateData(FALSE);

	long nRows(0), nCols(0);
	m_Grid.GetCellCount(nRows, nCols);
	m_Grid.MergeGrid(0, 0, 0, nCols);
	m_Grid.MergeGrid(0, 1, 0, 0);

	// �޺� ǥ��
	long nIdx	= m_pColumn->m_placeRebarRound[iOUTTER][1].m_nCountBlock-1;
	if(nIdx > 0)
		m_comboRoundIn.SetCurSel(nIdx);

	nIdx		= m_pColumn->m_placeRebarRound[iOUTTER][0].m_nCountBlock-1;
	if(nIdx > 0)
		m_comboRoundOut.SetCurSel(nIdx);

}

void CRoundDlg::SetDataSave()
{
	if(!m_pColumn) return;

	m_Grid.UpdateData(TRUE);
	
	// �޺� ����
	long nCount	= m_comboRoundIn.GetCurSel()+1;
	m_pColumn->m_placeRebarRound[iOUTTER][1].m_nCountBlock	= nCount;
	nCount		= m_comboRoundOut.GetCurSel()+1;
	m_pColumn->m_placeRebarRound[iOUTTER][0].m_nCountBlock	= nCount;

	m_pColumn->SyncRebar();
}

BOOL CRoundDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_Grid.ViewGridCellForCombo(pMsg);
	
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CRoundDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_Grid.InitGrid(m_pStd->m_Fy);

	SetControl();
	InitCombo();
	SetGridData();
	SetDataInit();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRoundDlg::OnSelchangeComboRoundIn() 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	SetGridData();
	SetDataInit();
}

void CRoundDlg::OnSelchangeComboRoundOut() 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	SetGridData();
	SetDataInit();
}

// �޹� ����
void CRoundDlg::InitCombo()
{
	CString str	= _T("");
	for(long block = 0; block < MAX_REBAR_BLOCK; block++)
	{
		str.Format("%d����", block+1);
		m_comboRoundIn.AddString(str);
		m_comboRoundOut.AddString(str);
	}
}

// �޼� ����
void CRoundDlg::SetControl()
{	
	if(!m_pColumn) return;
	long nCountDan	= m_pColumn->GetCountDan_RebarMain();
	if(nCountDan > 1)
	{
		m_comboRoundIn.EnableWindow(TRUE);
		m_comboRoundIn.EnableWindow(TRUE);
	}
	else
	{
		m_comboRoundIn.EnableWindow(TRUE);
		m_comboRoundIn.EnableWindow(FALSE);
	}
}


void CRoundDlg::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	structGridCell *pCell = m_Grid.GetCurrentCell();
	if(pCell)
	{
		CARcBridgeInputStd *pIn	= (dynamic_cast<CInputRebarColumn*>(m_pParent))->GetIn();
		pIn->SetComment(pCell->sTitle);
		
		m_pParent->m_pView->GetDomyun()->STMakeCursor(m_Grid.GetItemText(1,pCell->nCol));
	}
}

void CRoundDlg::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	SetDataSave();
	if(m_pParent)
		m_pParent->DrawInputDomyunView(FALSE);
}

void CRoundDlg::OnButtonRecommand() 
{
	// TODO: Add your control notification handler code here
	if(m_pParent)
	{
		m_pParent->SetDataDefault();
		SetGridData();
		SetDataInit();
		m_pParent->DrawInputDomyunView(FALSE);
	}
}
