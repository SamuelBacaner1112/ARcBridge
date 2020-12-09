// DialogSTMFootingMain.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ARcBridgeInput.h"
#include "afxdialogex.h"


// CDialogSTMFootingMain 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogSTMArcCornerMain, CDialog)

CDialogSTMArcCornerMain::CDialogSTMArcCornerMain(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSTMArcCornerMain::IDD, pParent)
{
	m_bChange = FALSE;
	for (HGINT32 nType = OUTSIDE_REBAR; nType < REBARTYPE_COUNT; ++nType)
	{
		for (HGINT32 nIdex = 0; nIdex < 6; ++nIdex)
		{
			m_arrDiaOrg[nType][nIdex] = 0.0;
			m_arrDia[nType][nIdex] = 0.0;
		}
	}

	m_pIn = NULL;
}

CDialogSTMArcCornerMain::~CDialogSTMArcCornerMain()
{
}

void CDialogSTMArcCornerMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CDialogSTMArcCornerMain, CDialog)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_BN_CLICKED(IDOK, &CDialogSTMArcCornerMain::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogSTMArcCornerMain::OnBnClickedCancel)
END_MESSAGE_MAP()

// CDialogSTMFootingMain 메시지 처리기입니다.

BOOL CDialogSTMArcCornerMain::OnInitDialog()
{
	CDialog::OnInitDialog();

	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	if(!pStd) return FALSE;

	CRcBridgeApp *pBridge = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBridge) return FALSE;

	SetDataRebarDia();

	for (HGINT32 nType = OUTSIDE_REBAR; nType < REBARTYPE_COUNT; ++nType)
	{
		for (HGINT32 nIdex = 0; nIdex < 6; ++nIdex)
		{
			m_arrDiaOrg[nType][nIdex] = m_arrDia[nType][nIdex];
		}
	}
	
	m_Grid.InitGrid(pStd->IsLsdDesign() ? pBridge->GetValueStmFy(m_bUpper ? eLsdStmCornerUpper : eLsdStmCornerLower) : pStd->m_Fy);
	SetGridData();
	SetDataInit();

	return TRUE;
}

void CDialogSTMArcCornerMain::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	structGridCell *pCell = m_Grid.GetCurrentCell();

	if (pCell)
	{
		SetDataSave();
		SetGridData();
		SetDataInit();
	}
}

void CDialogSTMArcCornerMain::SetDataInit()
{
	m_Grid.UpdateData(FALSE);

	long nRows(0), nCols(0);
	m_Grid.GetCellCount(nRows, nCols);

	m_Grid.MergeGridEx(0, 1, 0, nCols - 1);
}

void CDialogSTMArcCornerMain::SetDataSave()
{
	m_Grid.UpdateData(TRUE);

	for (HGINT32 nType = OUTSIDE_REBAR; nType < REBARTYPE_COUNT; ++nType)
	{
		for (HGINT32 nIdex = 0; nIdex < 6; ++nIdex)
		{
			if (m_arrDiaOrg[nType][nIdex] != m_arrDia[nType][nIdex])
			{
				m_bChange = TRUE;
			}		
		}
	}
}

void CDialogSTMArcCornerMain::SetGridData()
{
	HGINT32 nRow		= 0;
	HGINT32 nCol		= 0;
	UINT nFormat	= DT_RIGHT | DT_VCENTER;
	UINT nLine	= CELL_LINE_NO;

	m_Grid.SetGridDefault(2, 2, 2, 1);

	m_Grid.AddCell(nRow, nCol++, hggettext("구분"));
	m_Grid.AddCell(nRow, nCol++, hggettext("1단"));
	m_Grid.AddCell(nRow, nCol++, hggettext("1단"));
	m_Grid.AddCell(nRow, nCol++, hggettext("2단"));
	m_Grid.AddCell(nRow, nCol++, hggettext("2단"));
	m_Grid.AddCell(nRow, nCol++, hggettext("3단"));
	m_Grid.AddCell(nRow, nCol, hggettext("3단"));
	nRow++;
	nCol	= 0;

	m_Grid.AddCell(nRow, nCol++, hggettext("구분"));
	m_Grid.AddCell(nRow, nCol++, hggettext("1Cycle"));
	m_Grid.AddCell(nRow, nCol++, hggettext("2Cycle"));
	m_Grid.AddCell(nRow, nCol++, hggettext("1Cycle"));
	m_Grid.AddCell(nRow, nCol++, hggettext("2Cycle"));
	m_Grid.AddCell(nRow, nCol++, hggettext("1Cycle"));
	m_Grid.AddCell(nRow, nCol, hggettext("2Cycle"));
	nRow++;
	nCol	= 0;

	m_Grid.AddCell(nRow, nCol++, hggettext("외측주철근"));
	for (HGINT32 ix = 0; ix < 6; ++ix)
	{
		m_Grid.AddCell(nRow, nCol++, &m_arrDia[OUTSIDE_REBAR][ix] ,nFormat, nLine, _T(""), CELL_TYPE_DIA);
	}
	nRow++;
	nCol	= 0;

	m_Grid.AddCell(nRow, nCol++, hggettext("슬래브내측주철근"));
	for (HGINT32 ix = 0; ix < 6; ++ix)
	{
		m_Grid.AddCell(nRow, nCol++, &m_arrDia[SLABINSIDE_REBAR][ix] ,nFormat, nLine, _T(""), CELL_TYPE_DIA);
	}
	nRow++;
	nCol	= 0;

	m_Grid.AddCell(nRow, nCol++, hggettext("벽체내측주철근"));
	for (HGINT32 ix = 0; ix < 6; ++ix)
	{
		m_Grid.AddCell(nRow, nCol++, &m_arrDia[WALLINSIDE_REBAR][ix] ,nFormat, nLine, _T(""), CELL_TYPE_DIA);
	}
	nRow++;
	nCol	= 0;
	m_Grid.AddCell(nRow, nCol++, hggettext("헌치"));
	for (HGINT32 ix = 0; ix < 6; ++ix)
	{
		if (ix < 2)
			m_Grid.AddCell(nRow, nCol++, &m_arrDia[HUNCH][ix] ,nFormat, nLine, _T(""), CELL_TYPE_DIA);
		else
			m_Grid.AddCell(nRow, nCol++, _T(""));
	}

	long row = 0;
	long col = 0;
	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);

	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 120);
}

void CDialogSTMArcCornerMain::SetDataRebarDia()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	HGINT32 nIdx = 0;
	for (HGINT32 nDan = 0; nDan < 3; ++nDan)
	{
		for(int nCycle = 0; nCycle < 2; nCycle++)		// Cycle수
		{
			if (m_ixStmKey == STM_MODEL_CORNER_STT_UPPER_PLUS || m_ixStmKey == STM_MODEL_CORNER_STT_UPPER_MINUS
				|| m_ixStmKey == STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_1 || m_ixStmKey == STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1
				|| m_ixStmKey == STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_2 || m_ixStmKey == STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_2)
			{
				m_arrDia[OUTSIDE_REBAR][nIdx] =  pBri->GetDiaMainRebarUpperSlab(nCycle, nDan, TRUE, 0, 0);	
				// 내측인장일때 2Cycle철근은 포함이 안될수 있다. 걸러준다.
				if(pBri->GetInfoMainRebar_UpperSlab(FALSE, 0, nCycle, nDan, -1).x > 0)
				{
					m_arrDia[SLABINSIDE_REBAR][nIdx] = pBri->GetDiaMainRebarUpperSlab(nCycle, nDan, FALSE, 0, 0);
				}
				m_arrDia[WALLINSIDE_REBAR][nIdx] = pBri->IsExistMainRebarOutWall(nCycle, nDan, TRUE, TRUE, -1) ? pBri->GetDiaMainRebarOutWall(nCycle, nDan, TRUE, TRUE, -1) : 0;
				if (nDan == 0)
				{
					m_arrDia[HUNCH][nIdx] = pBri->GetDiaMainRebarUpperSlabHunch(nCycle, 0);
				}
			}
			else if (m_ixStmKey == STM_MODEL_CORNER_END_UPPER_PLUS || m_ixStmKey == STM_MODEL_CORNER_END_UPPER_MINUS
				|| m_ixStmKey == STM_MODEL_CORNER_END_UPPER_PLUS_EXT_1 || m_ixStmKey == STM_MODEL_CORNER_END_UPPER_MINUS_EXT_1
				|| m_ixStmKey == STM_MODEL_CORNER_END_UPPER_PLUS_EXT_2 || m_ixStmKey == STM_MODEL_CORNER_END_UPPER_MINUS_EXT_2)
			{
				m_arrDia[OUTSIDE_REBAR][nIdx] =  pBri->GetDiaMainRebarUpperSlab(nCycle, nDan, TRUE, pBri->m_nQtyJigan-1, -1);	
				// 내측인장일때 2Cycle철근은 포함이 안될수 있다. 걸러준다.
				if(pBri->GetInfoMainRebar_UpperSlab(FALSE, pBri->m_nQtyJigan-1, nCycle, nDan, 1).x > 0)
				{
					m_arrDia[SLABINSIDE_REBAR][nIdx] = pBri->GetDiaMainRebarUpperSlab(nCycle, nDan, FALSE, pBri->m_nQtyJigan-1, -1);
				}
				m_arrDia[WALLINSIDE_REBAR][nIdx] = pBri->IsExistMainRebarOutWall(nCycle, nDan, FALSE, TRUE, -1) ? pBri->GetDiaMainRebarOutWall(nCycle, nDan, FALSE, TRUE, -1) : 0;
				if (nDan == 0)
				{
					m_arrDia[HUNCH][nIdx] = pBri->GetDiaMainRebarUpperSlabHunch(nCycle, pBri->m_nQtyJigan);
				}
			}
			else if (m_ixStmKey == STM_MODEL_CORNER_STT_LOWER_PLUS || m_ixStmKey == STM_MODEL_CORNER_STT_LOWER_MINUS
				|| m_ixStmKey == STM_MODEL_CORNER_STT_LOWER_PLUS_EXT_1 || m_ixStmKey == STM_MODEL_CORNER_STT_LOWER_MINUS_EXT_1
				|| m_ixStmKey == STM_MODEL_CORNER_STT_LOWER_PLUS_EXT_2 || m_ixStmKey == STM_MODEL_CORNER_STT_LOWER_MINUS_EXT_2)
			{
				m_arrDia[OUTSIDE_REBAR][nIdx] =  pBri->GetDiaMainRebarLowerSlab(nCycle, nDan, FALSE, 0, 0);	
				// 내측인장일때 2Cycle철근은 포함이 안될수 있다. 걸러준다.
				if(pBri->GetInfoMainRebar_LowerSlab(FALSE, 0, nCycle, nDan, -1).x > 0)
				{
					m_arrDia[SLABINSIDE_REBAR][nIdx] = pBri->GetDiaMainRebarLowerSlab(nCycle, nDan, TRUE, 0, 0);
				}
				m_arrDia[WALLINSIDE_REBAR][nIdx] = pBri->IsExistMainRebarOutWall(nCycle, nDan, TRUE, TRUE, 0) ? pBri->GetDiaMainRebarOutWall(nCycle, nDan, TRUE, TRUE, 0) : 0;
				if (nDan == 0)
				{
					m_arrDia[HUNCH][nIdx] = pBri->GetDiaMainRebarLowerSlabHunch(nCycle, 0);
				}
			}
			else if (m_ixStmKey == STM_MODEL_CORNER_END_LOWER_PLUS || m_ixStmKey == STM_MODEL_CORNER_END_LOWER_MINUS
				|| m_ixStmKey == STM_MODEL_CORNER_END_LOWER_PLUS_EXT_1 || m_ixStmKey == STM_MODEL_CORNER_END_LOWER_MINUS_EXT_1
				|| m_ixStmKey == STM_MODEL_CORNER_END_LOWER_PLUS_EXT_2 || m_ixStmKey == STM_MODEL_CORNER_END_LOWER_MINUS_EXT_2)
			{
				m_arrDia[OUTSIDE_REBAR][nIdx] =  pBri->GetDiaMainRebarLowerSlab(nCycle, nDan, FALSE, pBri->m_nQtyJigan-1, -1);	
				if(pBri->GetInfoMainRebar_LowerSlab(FALSE, pBri->m_nQtyJigan-1, nCycle, nDan, 1).x > 0)
				{
					m_arrDia[SLABINSIDE_REBAR][nIdx] = pBri->GetDiaMainRebarLowerSlab(nCycle, nDan, TRUE, pBri->m_nQtyJigan-1, -1);
				}
				m_arrDia[WALLINSIDE_REBAR][nIdx] = pBri->IsExistMainRebarOutWall(nCycle, nDan, FALSE, TRUE, 0) ? pBri->GetDiaMainRebarOutWall(nCycle, nDan, FALSE, TRUE, 0) : 0;
				if (nDan == 0)
				{
					m_arrDia[HUNCH][nIdx] = pBri->GetDiaMainRebarUpperSlabHunch(nCycle, pBri->m_nQtyJigan);
				}
			}
			nIdx++;
		}
	}
}


void CDialogSTMArcCornerMain::OnBnClickedOk()
{
	SetDataSave();

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	HGINT32 nIdx = 0;
	if (m_bChange)
	{
		for (HGINT32 nDan = 0; nDan < 3; ++nDan)
		{
			for(int nCycle = 0; nCycle < 2; nCycle++)		// Cycle수
			{
				if (m_ixStmKey == STM_MODEL_CORNER_STT_UPPER_PLUS || m_ixStmKey == STM_MODEL_CORNER_STT_UPPER_MINUS
					|| m_ixStmKey == STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_1 || m_ixStmKey == STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1
					|| m_ixStmKey == STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_2 || m_ixStmKey == STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_2)
				{
					pBri->SetDiaMainRebarUpperSlab(nCycle, nDan, TRUE, 0, 0, m_arrDia[OUTSIDE_REBAR][nIdx]);	
					if(pBri->GetInfoMainRebar_UpperSlab(FALSE, 0, nCycle, nDan, -1).x > 0)
					{
						pBri->SetDiaMainRebarUpperSlab(nCycle, nDan, FALSE, 0, 0, m_arrDia[SLABINSIDE_REBAR][nIdx]);
					}
					pBri->SetDiaMainRebarOutWall(nCycle, nDan, TRUE, TRUE, -1, m_arrDia[WALLINSIDE_REBAR][nIdx]);
					if (nDan == 0)
					{
						pBri->SetDiaMainRebarUpperSlabHunch(nCycle, 0, m_arrDia[HUNCH][nIdx]);
					}
				}
				else if (m_ixStmKey == STM_MODEL_CORNER_END_UPPER_PLUS || m_ixStmKey == STM_MODEL_CORNER_END_UPPER_MINUS
					|| m_ixStmKey == STM_MODEL_CORNER_END_UPPER_PLUS_EXT_1 || m_ixStmKey == STM_MODEL_CORNER_END_UPPER_MINUS_EXT_1
					|| m_ixStmKey == STM_MODEL_CORNER_END_UPPER_PLUS_EXT_2 || m_ixStmKey == STM_MODEL_CORNER_END_UPPER_MINUS_EXT_2)
				{
					pBri->SetDiaMainRebarUpperSlab(nCycle, nDan, TRUE, pBri->m_nQtyJigan-1, -1, m_arrDia[OUTSIDE_REBAR][nIdx]);	
					if(pBri->GetInfoMainRebar_UpperSlab(FALSE, pBri->m_nQtyJigan-1, nCycle, nDan, 1).x > 0)
					{
						pBri->SetDiaMainRebarUpperSlab(nCycle, nDan, FALSE, pBri->m_nQtyJigan-1, -1, m_arrDia[SLABINSIDE_REBAR][nIdx]);
					}
					pBri->SetDiaMainRebarOutWall(nCycle, nDan, FALSE, TRUE, -1, m_arrDia[WALLINSIDE_REBAR][nIdx]);
					if (nDan == 0)
					{
						pBri->SetDiaMainRebarUpperSlabHunch(nCycle, pBri->m_nQtyJigan, m_arrDia[HUNCH][nIdx]);
					}
				}
				else if (m_ixStmKey == STM_MODEL_CORNER_STT_LOWER_PLUS || m_ixStmKey == STM_MODEL_CORNER_STT_LOWER_MINUS
					|| m_ixStmKey == STM_MODEL_CORNER_STT_LOWER_PLUS_EXT_1 || m_ixStmKey == STM_MODEL_CORNER_STT_LOWER_MINUS_EXT_1
					|| m_ixStmKey == STM_MODEL_CORNER_STT_LOWER_PLUS_EXT_2 || m_ixStmKey == STM_MODEL_CORNER_STT_LOWER_MINUS_EXT_2)
				{
					pBri->SetDiaMainRebarLowerSlab(nCycle, nDan, FALSE, 0, 0, m_arrDia[OUTSIDE_REBAR][nIdx]);	
					if(pBri->GetInfoMainRebar_LowerSlab(FALSE, 0, nCycle, nDan, -1).x > 0)
					{
						pBri->SetDiaMainRebarLowerSlab(nCycle, nDan, TRUE, 0, 0, m_arrDia[SLABINSIDE_REBAR][nIdx]);
					}
					pBri->SetDiaMainRebarOutWall(nCycle, nDan, TRUE, TRUE, 0, m_arrDia[WALLINSIDE_REBAR][nIdx]);
					if (nDan == 0)
					{
						pBri->SetDiaMainRebarLowerSlabHunch(nCycle, 0, m_arrDia[HUNCH][nIdx]);
					}
				}
				else if (m_ixStmKey == STM_MODEL_CORNER_END_LOWER_PLUS || m_ixStmKey == STM_MODEL_CORNER_END_LOWER_MINUS
					|| m_ixStmKey == STM_MODEL_CORNER_END_LOWER_PLUS_EXT_1 || m_ixStmKey == STM_MODEL_CORNER_END_LOWER_MINUS_EXT_1
					|| m_ixStmKey == STM_MODEL_CORNER_END_LOWER_PLUS_EXT_2 || m_ixStmKey == STM_MODEL_CORNER_END_LOWER_MINUS_EXT_2)
				{
					pBri->SetDiaMainRebarLowerSlab(nCycle, nDan, FALSE, pBri->m_nQtyJigan-1, -1, m_arrDia[OUTSIDE_REBAR][nIdx]);
					if(pBri->GetInfoMainRebar_LowerSlab(FALSE, pBri->m_nQtyJigan-1, nCycle, nDan, 1).x > 0)
					{
						pBri->SetDiaMainRebarLowerSlab(nCycle, nDan, TRUE, pBri->m_nQtyJigan-1, -1, m_arrDia[SLABINSIDE_REBAR][nIdx]);
					}
					pBri->SetDiaMainRebarOutWall(nCycle, nDan, FALSE, TRUE, 0, m_arrDia[WALLINSIDE_REBAR][nIdx]);
					if (nDan == 0)
					{
						pBri->SetDiaMainRebarUpperSlabHunch(nCycle, pBri->m_nQtyJigan, m_arrDia[HUNCH][nIdx]);
					}
				}
				nIdx++;
			}
		}

	}
	
	CDialog::OnOK();
}


void CDialogSTMArcCornerMain::OnBnClickedCancel()
{
	CDialog::OnCancel();
}
