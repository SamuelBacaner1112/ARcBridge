// DialogSTMArcCornerShear.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ARcBridgeInput.h"
#include "afxdialogex.h"

// CDialogSTMArcCornerShear 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogSTMArcCornerShear, CDialog)

CDialogSTMArcCornerShear::CDialogSTMArcCornerShear(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSTMArcCornerShear::IDD, pParent)
{
	m_nUpper = 0;
	m_nStt = 0;
	m_bChange = FALSE;
	
	m_pIn = NULL;
}

CDialogSTMArcCornerShear::~CDialogSTMArcCornerShear()
{
}

void CDialogSTMArcCornerShear::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CDialogSTMArcCornerShear, CDialog)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_BN_CLICKED(IDOK, &CDialogSTMArcCornerShear::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogSTMArcCornerShear::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDialogSTMArcCornerShear 메시지 처리기입니다.


BOOL CDialogSTMArcCornerShear::OnInitDialog()
{
	CDialog::OnInitDialog();

	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	if(!pStd) return FALSE;

	CRcBridgeApp *pBridge = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBridge) return FALSE;

	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return FALSE;

	SetDanRebarLocation();

	for (HGINT32 nDan = 0; nDan < 3; ++nDan)
	{
		m_dDiaCycle1Ori[nDan] = pBri->m_dDia_DanbuRebar_Cycle1[m_nUpper][m_nStt][nDan];
		m_dDiaCycle2Ori[nDan] = pBri->m_dDia_DanbuRebar_Cycle2[m_nUpper][m_nStt][nDan];
	}

	m_nCountStirrupAtDanBuRebarOri = pBri->m_nCountStirrupAtDanBuRebar[m_nUpper][m_nStt];
	m_dDiaStirrupAtDanBuRebarOri = pBri->m_dDiaStirrupAtDanBuRebar[m_nUpper][m_nStt];

	m_Grid.InitGrid(pStd->IsLsdDesign() ? pBridge->GetValueStmFy(m_bUpper ? eLsdStmCornerUpper : eLsdStmCornerLower) : pStd->m_Fy);
	SetGridData();
	SetDataInit();

	return TRUE;
}

void CDialogSTMArcCornerShear::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	structGridCell *pCell = m_Grid.GetCurrentCell();

	if (pCell)
	{
		SetDataSave();
		SetGridData();
		SetDataInit();
	}
}

void CDialogSTMArcCornerShear::SetDataInit()
{
	m_Grid.UpdateData(FALSE);
	long nRows(0), nCols(0);
	m_Grid.GetCellCount(nRows, nCols);

	m_Grid.MergeGridEx(0, nRows - 1, 0, 0);
}

void CDialogSTMArcCornerShear::SetDataSave()
{
	m_Grid.UpdateData(TRUE);
}

void CDialogSTMArcCornerShear::SetGridData()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	HGINT32 nRow		= 0;
	HGINT32 nCol		= 0;
	UINT nFormat	= DT_RIGHT | DT_VCENTER;
	UINT nLine	= CELL_LINE_NO;

	m_Grid.SetGridDefault(2, 2, 1, 1);

	m_Grid.AddCell(nRow, nCol++, hggettext("구분"));
	m_Grid.AddCell(nRow, nCol++, hggettext("1Cylce"));
	m_Grid.AddCell(nRow, nCol, hggettext("2Cylce"));
	nRow++;
	nCol	= 0;

	for (HGINT32 nDan = 0; nDan < 3; ++nDan)
	{
		m_Grid.AddCell(nRow, nCol++, hggettext("단절점부"));
		m_Grid.AddCell(nRow, nCol++, &pBri->m_dDia_DanbuRebar_Cycle1[m_nUpper][m_nStt][nDan] ,nFormat, nLine, _T(""), CELL_TYPE_DIA);
		m_Grid.AddCell(nRow, nCol, &pBri->m_dDia_DanbuRebar_Cycle2[m_nUpper][m_nStt][nDan] ,nFormat, nLine, _T(""), CELL_TYPE_DIA);
		nRow++;
		nCol	= 0;
	}
	CStringArray sArr;
	CString str;
	for (HGINT32 ix = 0; ix < 10; ++ix)
	{
		str.Format("%d", ix);
		sArr.Add(str);
	}
	m_Grid.AddCell(nRow, nCol++, hggettext("스트럽"));
	m_Grid.AddCell(nRow, nCol++, &pBri->m_nCountStirrupAtDanBuRebar[m_nUpper][m_nStt] ,nFormat,FALSE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(nRow, nCol++, &pBri->m_dDiaStirrupAtDanBuRebar[m_nUpper][m_nStt] ,nFormat, nLine, _T(""), CELL_TYPE_DIA);

	long row = 0;
	long col = 0;
	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
}

void CDialogSTMArcCornerShear::SetDanRebarLocation()
{
	if (m_ixStmKey == STM_MODEL_CORNER_STT_UPPER_PLUS || m_ixStmKey == STM_MODEL_CORNER_STT_UPPER_MINUS
		|| m_ixStmKey == STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_1 || m_ixStmKey == STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1
		|| m_ixStmKey == STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_2 || m_ixStmKey == STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_2)
	{
		m_nUpper = 0, m_nStt = 0;
	}
	else if (m_ixStmKey == STM_MODEL_CORNER_END_UPPER_PLUS || m_ixStmKey == STM_MODEL_CORNER_END_UPPER_MINUS
		|| m_ixStmKey == STM_MODEL_CORNER_END_UPPER_PLUS_EXT_1 || m_ixStmKey == STM_MODEL_CORNER_END_UPPER_MINUS_EXT_1
		|| m_ixStmKey == STM_MODEL_CORNER_END_UPPER_PLUS_EXT_2 || m_ixStmKey == STM_MODEL_CORNER_END_UPPER_MINUS_EXT_2)
	{
		m_nUpper = 0, m_nStt = 1;
	}
	else if (m_ixStmKey == STM_MODEL_CORNER_STT_LOWER_PLUS || m_ixStmKey == STM_MODEL_CORNER_STT_LOWER_MINUS
		|| m_ixStmKey == STM_MODEL_CORNER_STT_LOWER_PLUS_EXT_1 || m_ixStmKey == STM_MODEL_CORNER_STT_LOWER_MINUS_EXT_1
		|| m_ixStmKey == STM_MODEL_CORNER_STT_LOWER_PLUS_EXT_2 || m_ixStmKey == STM_MODEL_CORNER_STT_LOWER_MINUS_EXT_2)
	{
		m_nUpper = 1, m_nStt = 0;
	}
	else if (m_ixStmKey == STM_MODEL_CORNER_END_LOWER_PLUS || m_ixStmKey == STM_MODEL_CORNER_END_LOWER_MINUS
		|| m_ixStmKey == STM_MODEL_CORNER_END_LOWER_PLUS_EXT_1 || m_ixStmKey == STM_MODEL_CORNER_END_LOWER_MINUS_EXT_1
		|| m_ixStmKey == STM_MODEL_CORNER_END_LOWER_PLUS_EXT_2 || m_ixStmKey == STM_MODEL_CORNER_END_LOWER_MINUS_EXT_2)
	{
		m_nUpper = 1, m_nStt = 1;
	}
}



void CDialogSTMArcCornerShear::OnBnClickedOk()
{
	SetDataSave();

	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	for (HGINT32 nDan = 0; nDan < 3; ++nDan)
	{
		if (m_dDiaCycle1Ori[nDan] != pBri->m_dDia_DanbuRebar_Cycle1[m_nUpper][m_nStt][nDan]
		|| m_dDiaCycle2Ori[nDan] != pBri->m_dDia_DanbuRebar_Cycle2[m_nUpper][m_nStt][nDan])
		{
			m_bChange = TRUE;
			break;
		}
	}

	if (m_nCountStirrupAtDanBuRebarOri != pBri->m_nCountStirrupAtDanBuRebar[m_nUpper][m_nStt]
	|| m_dDiaStirrupAtDanBuRebarOri != pBri->m_dDiaStirrupAtDanBuRebar[m_nUpper][m_nStt])
	{
		m_bChange = TRUE;
	}

	CDialog::OnOK();
}


void CDialogSTMArcCornerShear::OnBnClickedCancel()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	for (HGINT32 nDan = 0; nDan < 3; ++nDan)
	{
		pBri->m_dDia_DanbuRebar_Cycle1[m_nUpper][m_nStt][nDan] = m_dDiaCycle1Ori[nDan];
		pBri->m_dDia_DanbuRebar_Cycle2[m_nUpper][m_nStt][nDan] = m_dDiaCycle2Ori[nDan];
	}

	pBri->m_nCountStirrupAtDanBuRebar[m_nUpper][m_nStt] = m_nCountStirrupAtDanBuRebarOri;
	pBri->m_dDiaStirrupAtDanBuRebar[m_nUpper][m_nStt] = m_dDiaStirrupAtDanBuRebarOri;

	CDialog::OnCancel();
}
