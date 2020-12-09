// DialogSTMFootingMainMid.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ARcBridgeInput.h"
#include "afxdialogex.h"
#include "DialogSTMFootingMainMid.h"

// CDialogSTMFootingMainMid 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogSTMFootingMainMid, CDialog)

CDialogSTMFootingMainMid::CDialogSTMFootingMainMid(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSTMFootingMainMid::IDD, pParent)
{
	m_bChange = FALSE;
	m_pIn = NULL;
}

CDialogSTMFootingMainMid::~CDialogSTMFootingMainMid()
{
}

void CDialogSTMFootingMainMid::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CDialogSTMFootingMainMid, CDialog)
	ON_BN_CLICKED(IDOK, &CDialogSTMFootingMainMid::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogSTMFootingMainMid::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDialogSTMFootingMainMid 메시지 처리기입니다.


BOOL CDialogSTMFootingMainMid::OnInitDialog()
{
	CDialog::OnInitDialog();

	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	if(!pStd) return FALSE;

	CRcBridgeApp *pBridge = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBridge) return FALSE;

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return FALSE;

	CWallApp *pWall = pBri->GetInWall(m_nCurIdx);
	CFootingApp *pFooting = &pWall->m_footing;
	if(!pFooting) return FALSE;
	m_FootingOri = *pFooting;

	m_Grid.InitGrid(pStd->IsLsdDesign() ? pBridge->GetValueStmFy(eLsdStmFootMid) : pStd->m_Fy);
	SetGridData();
	SetDataInit();

	return TRUE;
}

void CDialogSTMFootingMainMid::SetDataInit()
{
	m_Grid.UpdateData(FALSE);
	m_Grid.MergeGridEx(0, 1, 0, 0);
	m_Grid.MergeGridEx(0, 0, 0, 4);
}

void CDialogSTMFootingMainMid::SetDataSave()
{
	m_Grid.UpdateData(TRUE);
}

void CDialogSTMFootingMainMid::SetGridData()
{
	m_Grid.SetGridDefault(3, 3, 2, 1, TRUE, FALSE);

	HGINT32 nRow = 0, nCol = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	CWallApp *pWall = pBri->GetInWall(m_nCurIdx);
	CFootingApp *pFooting = &pWall->m_footing;
	if(!pFooting) return;

	m_Grid.AddCell(nRow, nCol++, hggettext("구분"));
	m_Grid.AddCell(nRow, nCol++, hggettext("상면"));
	m_Grid.AddCell(nRow, nCol++, hggettext("상면"));
	m_Grid.AddCell(nRow, nCol++, hggettext("하면"));
	m_Grid.AddCell(nRow, nCol, hggettext("하면"));
	nRow++;
	nCol = 0;

	m_Grid.AddCell(nRow, nCol++, hggettext("구분"));
	m_Grid.AddCell(nRow, nCol++, hggettext("1cycle"));
	m_Grid.AddCell(nRow, nCol++, hggettext("2cycle"));
	m_Grid.AddCell(nRow, nCol++, hggettext("1cycle"));
	m_Grid.AddCell(nRow, nCol, hggettext("2cycle"));
	nRow++;
	nCol = 0;

	CString str = _T("");	
	for(HGINT32 nDan = 0; nDan < MAX_REBAR_DAN; nDan++)
	{
		str.Format(hggettext("%d단"), nDan+1);
		m_Grid.AddCell(nRow, nCol++, str);
		for(HGINT32 nUpper = 0; nUpper < 2; nUpper++)
		{
			for(HGINT32 nCycle = 0; nCycle < 2; nCycle++)
				m_Grid.AddCell(nRow, nCol++, &pFooting->m_setRebarDan[nUpper].m_dDia[nCycle][nDan], nFormat, nLine, _T(""), CELL_TYPE_DIA);
		}
		nRow++;
		nCol = 0;
	}

	long row= 0, col = 0;
	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetRowHeightAll(20);
}


void CDialogSTMFootingMainMid::OnBnClickedOk()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	CWallApp *pWall = pBri->GetInWall(m_nCurIdx);
	CFootingApp *pFooting = &pWall->m_footing;
	if(!pFooting) return;

	SetDataSave();

	for(HGINT32 nDan = 0; nDan < MAX_REBAR_DAN; nDan++)
	{
		for(HGINT32 nUpper = 0; nUpper < 2; nUpper++)
		{
			for(HGINT32 nCycle = 0; nCycle < 2; nCycle++)
			{
				if (pFooting->m_setRebarDan[nUpper].m_dDia[nCycle][nDan] != m_FootingOri.m_setRebarDan[nUpper].m_dDia[nCycle][nDan])
				{
					m_bChange = TRUE;
					break;
				}
			}
		}
	}

	CDialog::OnOK();
}


void CDialogSTMFootingMainMid::OnBnClickedCancel()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	CWallApp *pWall = pBri->GetInWall(m_nCurIdx);
	CFootingApp *pFooting = &pWall->m_footing;
	if(!pFooting) return;

	*pFooting = m_FootingOri;

	CDialog::OnCancel();
}

void CDialogSTMFootingMainMid::OnCellChangedData( NMHDR* pNotifyStruct, LRESULT* result )
{
	structGridCell *pCell = m_Grid.GetCurrentCell();

	if (pCell)
	{
		SetDataSave();
		SetGridData();
		SetDataInit();
	}
}