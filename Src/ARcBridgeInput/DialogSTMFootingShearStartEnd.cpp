// DialogSTMFootingShearStartEnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ARcBridgeInput.h"
#include "afxdialogex.h"

// CDialogSTMFootingShearStartEnd 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogSTMFootingShearStartEnd, CDialog)

const int dArrCtc[] = { 100, 125, 150, 200, 250, 300, 400, 450, 500, 600, 800, 900, 1200 };

CDialogSTMFootingShearStartEnd::CDialogSTMFootingShearStartEnd(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSTMFootingShearStartEnd::IDD, pParent)
{
	m_bChange = FALSE;
	m_nCtcShear = 0;

	m_pIn = NULL;
}

CDialogSTMFootingShearStartEnd::~CDialogSTMFootingShearStartEnd()
{
}

void CDialogSTMFootingShearStartEnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CDialogSTMFootingShearStartEnd, CDialog)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_BN_CLICKED(IDOK, &CDialogSTMFootingShearStartEnd::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogSTMFootingShearStartEnd::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDialogSTMFootingShearStartEnd 메시지 처리기입니다.

BOOL CDialogSTMFootingShearStartEnd::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetFootLoation();

	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	if(!pStd) return FALSE;

	CRcBridgeApp *pBridge = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBridge) return FALSE;

	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return FALSE;

	CRebarShearDetail *pDetail	= NULL;
	CFootingApp* pFoot = NULL;
	if (m_nStt == 0)
		pFoot = &pBri->m_footingStt;
	else
		pFoot = &pBri->m_footingEnd;

	pDetail = &pFoot->m_rebarShearDetail;

	m_dDiaRebarOri = pDetail->m_dDia;
	m_nTypeOri = pDetail->m_nType;
	m_dCTCOri = pFoot->m_dCTC_ShearRebar;
	m_nCountWrapOri = pFoot->m_nCountWrapMainRebar;

	//초기값
	for(HGINT32 ix = 0; ix < 13; ix++)
	{
		if(Compare( m_nStt == 0 ? pBri->m_footingStt.m_dCTC_ShearRebar : pBri->m_footingEnd.m_dCTC_ShearRebar, (double)dArrCtc[ix], _T("=")))
		{
			m_nCtcShear = ix;
			break;
		} 
	}

	m_Grid.InitGrid(pStd->IsLsdDesign() ? pBridge->GetValueStmFy(m_nStt == 0 ? eLsdStmFootStt : eLsdStmFootEnd) : pStd->m_Fy);
	SetGridData();
	SetDataInit();

	return TRUE;
}

void CDialogSTMFootingShearStartEnd::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	structGridCell *pCell = m_Grid.GetCurrentCell();

	if (pCell)
	{
		SetDataSave();
		SetGridData();
		SetDataInit();
	}
}

void CDialogSTMFootingShearStartEnd::SetDataInit()
{
	m_Grid.UpdateData(FALSE);
}

void CDialogSTMFootingShearStartEnd::SetDataSave()
{
	m_Grid.UpdateData(TRUE);

	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	CFootingApp* pFoot = NULL;
	if (m_nStt == 0)
		pFoot = &pBri->m_footingStt;
	else
		pFoot = &pBri->m_footingEnd;

	pFoot->m_dCTC_ShearRebar = dArrCtc[m_nCtcShear];
}

void CDialogSTMFootingShearStartEnd::SetGridData()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	HGINT32 nRow		= 0;
	HGINT32 nCol		= 0;
	UINT nFormat	= DT_RIGHT | DT_VCENTER;
	UINT nLine	= CELL_LINE_NO;

	CRebarShearDetail *pDetail	= NULL;
	CFootingApp* pFoot = NULL;
	if (m_nStt == 0)
		pFoot = &pBri->m_footingStt;
	else
		pFoot = &pBri->m_footingEnd;

	pDetail = &pFoot->m_rebarShearDetail;

	m_Grid.SetGridDefault(2, 2, 0, 1);

	m_Grid.AddCell(nRow, nCol++, hggettext("전단철근"));
	m_Grid.AddCell(nRow, nCol, &pDetail->m_dDia ,nFormat, nLine, _T(""), CELL_TYPE_DIA);
	nRow++;
	nCol	= 0;

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
	m_Grid.AddCell(nRow, nCol, &pDetail->m_nType, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
	nRow++;
	nCol	= 0;

	sArr.RemoveAll();
	for(HGINT32 ix = 0; ix < 13; ix++)
	{
		str.Format(_T("CTC %d"), dArrCtc[ix]);
		sArr.Add(str);
	}
	m_Grid.AddCell(nRow, nCol++, hggettext("전단철근간격(S)"));
	m_Grid.AddCell(nRow, nCol, &m_nCtcShear, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
	nRow++;
	nCol	= 0;

	m_Grid.AddCell(nRow, nCol++, hggettext("주철근 묶음"));
	m_Grid.AddCell(nRow, nCol, &pFoot->m_nCountWrapMainRebar, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArrWrap); m_Grid.SetCellBaseOfIndex(1);

	long row = 0;
	long col = 0;
	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidth(0, 120);
	m_Grid.SetColumnWidth(1, 120);
}

void CDialogSTMFootingShearStartEnd::SetFootLoation()
{
	if (m_ixStmKey == STM_MODEL_FOOT_STT_MAXP || m_ixStmKey == STM_MODEL_FOOT_STT_MAXM 
		|| m_ixStmKey == STM_MODEL_FOOT_STT_MINM || m_ixStmKey == STM_MODEL_FOOT_STT_EARTH)
	{
		m_nStt = 0;
	}
	else if (m_ixStmKey == STM_MODEL_FOOT_END_MAXP || m_ixStmKey == STM_MODEL_FOOT_END_MAXM 
		|| m_ixStmKey == STM_MODEL_FOOT_END_MINM || m_ixStmKey == STM_MODEL_FOOT_END_EARTH)
	{
		m_nStt = 1;
	}
}

void CDialogSTMFootingShearStartEnd::OnBnClickedOk()
{
	SetDataSave();

	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	CFootingApp* pFoot = NULL;
	if (m_nStt == 0)
		pFoot = &pBri->m_footingStt;
	else
		pFoot = &pBri->m_footingEnd;

	CRebarShearDetail *pDetail	= &pFoot->m_rebarShearDetail;

	if (pDetail->m_dDia != m_dDiaRebarOri 
		|| pDetail->m_nType != m_nTypeOri
		|| (pFoot->m_dCTC_ShearRebar != m_dCTCOri)
		|| pFoot->m_nCountWrapMainRebar != m_nCountWrapOri)
	{
		m_bChange = TRUE;
	}

	CDialog::OnOK();
}


void CDialogSTMFootingShearStartEnd::OnBnClickedCancel()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	CFootingApp* pFoot = NULL;
	if (m_nStt == 0)
		pFoot = &pBri->m_footingStt;
	else
		pFoot = &pBri->m_footingEnd;

	CRebarShearDetail *pDetail	= &pFoot->m_rebarShearDetail;

	pDetail->m_dDia = m_dDiaRebarOri;
	pDetail->m_nType = m_nTypeOri;
	pFoot->m_dCTC_ShearRebar = m_dCTCOri;
	pFoot->m_nCountWrapMainRebar = m_nCountWrapOri;

	CDialog::OnCancel();
}
