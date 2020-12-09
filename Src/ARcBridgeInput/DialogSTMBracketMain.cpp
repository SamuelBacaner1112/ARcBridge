// DialogSTMBracketMain.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ARcBridgeInput.h"
#include "afxdialogex.h"



// CDialogSTMBracketMain 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogSTMBracketMain, CDialog)

CDialogSTMBracketMain::CDialogSTMBracketMain(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSTMBracketMain::IDD, pParent)
{
	m_bChange = FALSE;

	m_pIn = NULL;
	m_dDiaRebarCycleOri2 = 0.;
	m_dDiaRebarCycleOri1 = 0.;
}

CDialogSTMBracketMain::~CDialogSTMBracketMain()
{
}

void CDialogSTMBracketMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CDialogSTMBracketMain, CDialog)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_BN_CLICKED(IDOK, &CDialogSTMBracketMain::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogSTMBracketMain::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDialogSTMBracketMain 메시지 처리기입니다.


BOOL CDialogSTMBracketMain::OnInitDialog()
{
	CDialog::OnInitDialog();

	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	if(!pStd) return FALSE;

	CRcBridgeApp *pBridge = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBridge) return FALSE;

	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return FALSE;

	if (m_ixStmKey == STM_MODEL_BRACKET_STT)
	{
		m_dDiaRebarCycleOri1 = pBri->m_dDiaRebarMain_Bracket_Cycle1[iSTT];
		m_dDiaRebarCycleOri2 = pBri->m_dDiaRebarMain_Bracket_Cycle2[iSTT];
	}
	else if (m_ixStmKey == STM_MODEL_BRACKET_END)
	{
		m_dDiaRebarCycleOri1 = pBri->m_dDiaRebarMain_Bracket_Cycle1[iEND];
		m_dDiaRebarCycleOri2 = pBri->m_dDiaRebarMain_Bracket_Cycle2[iEND];
	}
	
	HGBOOL bStt = m_ixStmKey == STM_MODEL_BRACKET_STT;
	m_Grid.InitGrid(pStd->IsLsdDesign() ? pBridge->GetValueStmFy(bStt ? eLsdStmBracketStt : eLsdStmBracketEnd) : pStd->m_Fy);
	SetGridData();
	SetDataInit();

	return TRUE;
}

void CDialogSTMBracketMain::SetDataInit()
{
	m_Grid.UpdateData(FALSE);

	m_Grid.MergeGridEx(0, 1, 0, 0);
	m_Grid.MergeGridEx(0, 0, 1, 2);
}

void CDialogSTMBracketMain::SetDataSave()
{
	m_Grid.UpdateData(TRUE);
}

void CDialogSTMBracketMain::SetGridData()
{
	m_Grid.SetGridDefault(3, 3, 2, 1, TRUE, FALSE);

	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long nRow = 0;
	long nCol = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;

	m_Grid.AddCell(nRow, nCol++, hggettext("구분"));
	m_Grid.AddCell(nRow, nCol++, hggettext("브라켓"));
	m_Grid.AddCell(nRow, nCol, hggettext("브라켓"));
	nRow++;
	nCol = 0;

	m_Grid.AddCell(nRow, nCol++, hggettext("구분"));
	m_Grid.AddCell(nRow, nCol++, hggettext("1cycle"));
	m_Grid.AddCell(nRow, nCol, hggettext("2cycle"));
	nRow++;
	nCol = 0;

	m_Grid.AddCell(nRow, nCol++, hggettext("1단"));
	if (m_ixStmKey == STM_MODEL_BRACKET_STT)
	{
		m_Grid.AddCell(nRow, nCol++, &pBri->m_dDiaRebarMain_Bracket_Cycle1[iSTT], nFormat, nLine, _T(""), CELL_TYPE_DIA);
		m_Grid.AddCell(nRow, nCol++, &pBri->m_dDiaRebarMain_Bracket_Cycle2[iSTT], nFormat, nLine, _T(""), CELL_TYPE_DIA);
	}
	else if (m_ixStmKey == STM_MODEL_BRACKET_END)
	{
		m_Grid.AddCell(nRow, nCol++, &pBri->m_dDiaRebarMain_Bracket_Cycle1[iEND], nFormat, nLine, _T(""), CELL_TYPE_DIA);
		m_Grid.AddCell(nRow, nCol++, &pBri->m_dDiaRebarMain_Bracket_Cycle2[iEND], nFormat, nLine, _T(""), CELL_TYPE_DIA);
	}

	long row = 0;
	long col = 0;
	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
}

void CDialogSTMBracketMain::OnBnClickedOk()
{
	SetDataSave();

	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	if (m_ixStmKey == STM_MODEL_BRACKET_STT)
	{
		if ( m_dDiaRebarCycleOri1 != pBri->m_dDiaRebarMain_Bracket_Cycle1[iSTT] || m_dDiaRebarCycleOri2 != pBri->m_dDiaRebarMain_Bracket_Cycle2[iSTT])
			m_bChange = TRUE;
	}
	else if (m_ixStmKey == STM_MODEL_BRACKET_END)
	{
		if ( m_dDiaRebarCycleOri1 != pBri->m_dDiaRebarMain_Bracket_Cycle1[iEND] || m_dDiaRebarCycleOri2 != pBri->m_dDiaRebarMain_Bracket_Cycle2[iEND])
			m_bChange = TRUE;
	}

	CDialog::OnOK();
}


void CDialogSTMBracketMain::OnBnClickedCancel()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	if (m_ixStmKey == STM_MODEL_BRACKET_STT)
	{
		pBri->m_dDiaRebarMain_Bracket_Cycle1[iSTT] = m_dDiaRebarCycleOri1;
		pBri->m_dDiaRebarMain_Bracket_Cycle2[iSTT] = m_dDiaRebarCycleOri2;
	}
	else if (m_ixStmKey == STM_MODEL_BRACKET_END)
	{
		pBri->m_dDiaRebarMain_Bracket_Cycle1[iEND] = m_dDiaRebarCycleOri1;
		pBri->m_dDiaRebarMain_Bracket_Cycle2[iEND] = m_dDiaRebarCycleOri2;
	}

	CDialog::OnCancel();
}

void CDialogSTMBracketMain::OnCellChangedData( NMHDR* pNotifyStruct, LRESULT* result )
{
	structGridCell *pCell = m_Grid.GetCurrentCell();

	if (pCell)
	{
		SetDataSave();
		SetGridData();
		SetDataInit();
	}
}
