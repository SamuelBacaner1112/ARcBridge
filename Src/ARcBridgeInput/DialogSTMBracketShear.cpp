// DialogSTMBracketShear.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ARcBridgeInput.h"
#include "afxdialogex.h"


// CDialogSTMBracketShear 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogSTMBracketShear, CDialog)

CDialogSTMBracketShear::CDialogSTMBracketShear(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSTMBracketShear::IDD, pParent)
{
	m_bChange = FALSE;
	m_pIn = NULL;
}

CDialogSTMBracketShear::~CDialogSTMBracketShear()
{
}

void CDialogSTMBracketShear::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CDialogSTMBracketShear, CDialog)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_BN_CLICKED(IDOK, &CDialogSTMBracketShear::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogSTMBracketShear::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDialogSTMBracketShear 메시지 처리기입니다.

BOOL CDialogSTMBracketShear::OnInitDialog()
{
	CDialog::OnInitDialog();

	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	if(!pStd) return FALSE;

	CRcBridgeApp *pBridge = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBridge) return FALSE;

	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return FALSE;

	SetBracketLoation();

	CRebarShearDetail *pDetail	= &pBri->m_RebarShearDetail_Bracket[m_nStt];

	m_dDiaRebarOri = pDetail->m_dDia;
	m_nTypeOri = pDetail->m_nType;
	m_nCountWrapOri = pBri->m_nCountWrapMainRebarForBracket[m_nStt];

	SetBracketLoation();

	HGBOOL bStt = m_ixStmKey == STM_MODEL_BRACKET_STT;
	m_Grid.InitGrid(pStd->IsLsdDesign() ? pBridge->GetValueStmFy(bStt ? eLsdStmBracketStt : eLsdStmBracketEnd) : pStd->m_Fy);
	SetGridData();
	SetDataInit();

	return TRUE;
}

void CDialogSTMBracketShear::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	structGridCell *pCell = m_Grid.GetCurrentCell();

	if (pCell)
	{
		SetDataSave();
		SetGridData();
		SetDataInit();
	}
}

void CDialogSTMBracketShear::SetDataInit()
{
	m_Grid.UpdateData(FALSE);
}

void CDialogSTMBracketShear::SetDataSave()
{
	m_Grid.UpdateData(TRUE);
}

void CDialogSTMBracketShear::SetGridData()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	HGINT32 nRow		= 0;
	HGINT32 nCol		= 0;
	UINT nFormat	= DT_RIGHT | DT_VCENTER;
	UINT nLine	= CELL_LINE_NO;

	m_Grid.SetGridDefault(2, 2, 0, 1);

	m_Grid.AddCell(nRow, nCol++, hggettext("전단철근"));
	m_Grid.AddCell(nRow, nCol, &pBri->m_RebarShearDetail_Bracket[m_nStt].m_dDia,nFormat, nLine, _T(""), CELL_TYPE_DIA);
	nRow++;
	nCol	= 0;

	CRebarShearDetail *pDetail	= &pBri->m_RebarShearDetail_OutWall[m_nStt];

	CStringArray sArr;
	for(HGINT32 ix = 0; ix < TYPE_SHEAR_SU; ix++)
		sArr.Add(pDetail->GetStringType(ix));

	CStringArray sArrWrap;
	CString str;
	for(HGINT32 nWrap = 0; nWrap < MAX_REBAR_WRAP; nWrap++)
	{
		str.Format(_T("%d개씩"), nWrap+1);
		sArrWrap.Add(str);
	}

	m_Grid.AddCell(nRow, nCol++, hggettext("전단철근타입"));
	m_Grid.AddCell(nRow, nCol, &pDetail->m_nType, nFormat, TRUE, CELL_READONLY, "", CELL_TYPE_COMBO, sArr);
	nRow++;
	nCol	= 0;

	m_Grid.AddCell(nRow, nCol++, hggettext("주철근 묶음"));
	m_Grid.AddCell(nRow, nCol, &pBri->m_nCountWrapMainRebarForBracket[m_nStt], nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArrWrap); m_Grid.SetCellBaseOfIndex(1);

	long row = 0;
	long col = 0;
	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidth(0, 120);
	m_Grid.SetColumnWidth(1, 120);
}

void CDialogSTMBracketShear::SetBracketLoation()
{
	if (m_ixStmKey == STM_MODEL_BRACKET_STT)
	{
		m_nStt = 0;
	}
	else if (m_ixStmKey == STM_MODEL_BRACKET_END)
	{
		m_nStt = 1;
	}
}


void CDialogSTMBracketShear::OnBnClickedOk()
{
	SetDataSave();

	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	CRebarShearDetail *pDetail	= &pBri->m_RebarShearDetail_OutWall[m_nStt];

	if (pBri->m_RebarShearDetail_Bracket[m_nStt].m_dDia != m_dDiaRebarOri
		|| pDetail->m_nType != m_nTypeOri
		|| pBri->m_nCountWrapMainRebarForBracket[m_nStt] != m_nCountWrapOri)
	{
		m_bChange = TRUE;
	}

	CDialog::OnOK();
}


void CDialogSTMBracketShear::OnBnClickedCancel()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	CRebarShearDetail *pDetail	= &pBri->m_RebarShearDetail_Bracket[m_nStt];

	pDetail->m_dDia = m_dDiaRebarOri;
	pDetail->m_nType = m_nTypeOri;
	pBri->m_nCountWrapMainRebarForBracket[m_nStt] = m_nCountWrapOri;

	CDialog::OnCancel();
}
