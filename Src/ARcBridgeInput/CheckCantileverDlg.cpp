// CheckCantileverDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "CheckCantileverDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CCheckCantileverDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCheckCantileverDlg, CDialog)

CCheckCantileverDlg::CCheckCantileverDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckCantileverDlg::IDD, pParent)
{
	m_pBri = NULL;
	m_pCalc = NULL;

	m_nCurrentBridge = -1;

	m_nTabSelLoad = -1;			// �������߼���
	m_nTabLoadCombine = -1;		// �������ռ���
	m_nTabLimit = -1;				// �����Ѱ����
	m_nTabExtremel = -1;			// �شܻ�Ȳ�Ѱ����
	m_nTabUseLimit = -1;			// ����Ѱ����
	m_nTabEta		= -1;
	m_nTabDesign = -1;				// ���輱�û���
	m_nTabLoad = -1;				// �ۿ����߼���
}

CCheckCantileverDlg::CCheckCantileverDlg(CRcBridgeRebar *pBri, long nCurrentBridge, CWnd* pParent /*=NULL*/)
	: CDialog(CCheckCantileverDlg::IDD, pParent)
{
	m_pBri	= pBri;
	m_nCurrentBridge = nCurrentBridge;

	m_nTabSelLoad = -1;			// �������߼���
	m_nTabLoadCombine = -1;		// �������ռ���
	m_nTabLimit = -1;				// �����Ѱ����
	m_nTabExtremel = -1;			// �شܻ�Ȳ�Ѱ����
	m_nTabUseLimit = -1;			// ����Ѱ����
	m_nTabEta		= -1;
	m_nTabDesign = -1;				// ���輱�û���
	m_nTabLoad = -1;				// �ۿ����߼���

	SetCalcStdData();
}

CCheckCantileverDlg::~CCheckCantileverDlg()
{
	if(m_pCalc)
	{
		delete m_pCalc;
		m_pCalc	= NULL;
	}
}

void CCheckCantileverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_RECOMMOND, m_btnRecommand);
	DDX_Control(pDX, IDC_BUTTON_EXAMINE, m_btnExamine);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_TAB, m_Tab);
}


BEGIN_MESSAGE_MAP(CCheckCantileverDlg, CDialog)
	ON_BN_CLICKED(IDC_RECOMMOND, OnRecommond)
	ON_BN_CLICKED(IDC_BUTTON_EXAMINE, OnExamine)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA,IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()


// CCheckCantileverDlg �޽��� ó�����Դϴ�.

BOOL CCheckCantileverDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	// ��ư ��Ÿ�� ����

	InitTabCtrl();

	SetDataDefault(TRUE);
	SetGridData();
	SetDataInit();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCheckCantileverDlg::InitTabCtrl()
{
	m_Tab.RemoveAllTabs();
	m_Tab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_Tab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	long nIdx = 0;
	CStringArray sArr;
	sArr.Add("�������߼���");		m_nTabSelLoad = nIdx++;
	sArr.Add("�������ռ���");		m_nTabLoadCombine = nIdx++;
	sArr.Add("�����Ѱ����");		m_nTabLimit = nIdx++;
	sArr.Add("�شܻ�Ȳ�Ѱ����");	m_nTabExtremel = nIdx++;
	sArr.Add("����Ѱ����");		m_nTabUseLimit = nIdx++;
	sArr.Add("���߼������");		m_nTabEta = nIdx++;
	sArr.Add("���輱�û���");		m_nTabDesign = nIdx++;
	sArr.Add("�ۿ����߼���");		m_nTabLoad = nIdx++;

	for (nIdx = 0; nIdx < sArr.GetCount(); ++nIdx )
	{
		m_Tab.InsertItem(nIdx, sArr.GetAt(nIdx));
		
	}

	m_Tab.SetCurSel(0);
}

void CCheckCantileverDlg::SetGridData()
{
	long nIndex = m_Tab.GetCurSel();

	switch (nIndex)
	{
	case 0:
		SetGridDataSelectLoadLsd();
		break;
	case 1:
		SetGridData_CombineLoad();
		break;
	case 2:
		SetGridData_Limit();
		break;
	case 3:
		SetGridData_Extremel();
		break;
	case 4:
		SetGridData_UseLimit();
		break;
	case 5:
		SetGridData_Eta();
		break;
	case 6:
		SetGridData_Design();
		break;
	case 7:
		SetGridData_WorkingLoad();
		break;
	}

	m_Grid.UpdateData(FALSE);
}

void CCheckCantileverDlg::SetDataSave()
{
	m_Grid.UpdateData(TRUE);
}

void CCheckCantileverDlg::SetDataInit()
{
	m_Grid.UpdateData(FALSE);

	long nRows(0), nCols(0);
	m_Grid.GetCellCount(nRows, nCols);

	long nTab = m_Tab.GetCurSel();

	if(nTab == m_nTabDesign || nTab == m_nTabLoad)
	{
		m_Grid.MergeGridEx(0, 0, 0, nCols-1);
		m_Grid.MergeGridEx(0, nRows-1, 0, 0);
	}
}

void CCheckCantileverDlg::OnRecommond() 
{
	SetDataDefault();
	SetGridData();
	SetDataInit();
}

void CCheckCantileverDlg::OnExamine() 
{

}

BOOL CCheckCantileverDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_Grid.ViewGridCellForCombo(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}

void CCheckCantileverDlg::SetCalcStdData()
{
	m_pCalc = new CARcBridgeCalcStd( m_pBri );
}

void CCheckCantileverDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	CDialog::OnCancel();
}

void CCheckCantileverDlg::OnOK() 
{
	// TODO: Add extra validation here
	SetDataSave();

	if( m_pCalc )
		m_pCalc->m_pCantilever->CalculateAll();

	CDialog::OnOK();
}

void CCheckCantileverDlg::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult)
{
	SetDataSave();
}

void CCheckCantileverDlg::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	SetGridData();
	SetDataInit();
}

void CCheckCantileverDlg::SetGridDataSelectLoadLsd()
{
	CARcBridgeDataStd *pStd = m_pBri->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_nCurrentBridge);
	if(!pBri) return;

	m_Grid.SetGridDefault(4, 4, 2, 1);
	m_Grid.SetLsdManager(pBri->m_plsdMngForCantilever);
	m_Grid.SetGridInitDesignLoad(FALSE);

	m_Grid.DeleteColumn(4);
	m_Grid.DeleteColumn(2);
}

void CCheckCantileverDlg::SetGridData_CombineLoad()
{
	CARcBridgeDataStd *pStd = m_pBri->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_nCurrentBridge);
	if(!pBri) return;

	m_Grid.SetLsdManager(pBri->m_plsdMngForCantilever);
	m_Grid.SetGridInitLoadFactor();
}

void CCheckCantileverDlg::SetGridData_Limit()
{
	SetGridData_LsdCombo(eLSD_COMBINATION_LIMIT);
}

void CCheckCantileverDlg::SetGridData_Extremel()
{
	SetGridData_LsdCombo(eLSD_COMBINATION_EXTREME);
}

void CCheckCantileverDlg::SetGridData_UseLimit()
{
	SetGridData_LsdCombo(eLSD_COMBINATION_SERVICE);
}

void CCheckCantileverDlg::SetGridData_Design()
{
	CRcBridgeData *pBri	= m_pBri->m_pARcBridgeDataStd->GetBridge(m_nCurrentBridge);
	if( pBri == NULL ) return;

	CCheckCantileverData *pCheckData = pBri->m_pCheckCantilever;
	if( pCheckData == NULL ) return;

	HGBOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	CString strUnitCurTonfM = bLsd ? _T("N/mm") : _T("kN/m");
	CString strUnitCurTonfM2 = bLsd ? _T("N/mm��") : _T("kN/m��");
	CString strUnitCurM = bLsd ? _T("mm") : _T("m");

	CStringArray sArr;
	long row = 0;
	long col = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;

	m_Grid.SetGridDefault(2, 2, 1, 1);

	m_Grid.AddCell(row, col++, hgwgettext("����"));
	m_Grid.AddCell(row, col++, hgwgettext("����"));
	m_Grid.AddCell(row, col++, hgwgettext("����"));
	m_Grid.AddCell(row, col++, hgwgettext("���"));
	row++, col = 0;

	m_Grid.AddCell(row, col++, hgwgettext("������ũ��Ʈ �����߷� ����"));
	m_Grid.AddCell(row, col++, &pCheckData->m_bMugun, nFormat, nLine, hgwgettext("��ȣ��"), CELL_TYPE_CHECK); 
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	row++, col = 0;

	m_Grid.AddCell(row, col++, hgwgettext("������ũ��Ʈ �����߷� ����"));
	m_Grid.AddCell(row, col++, &pCheckData->m_bCentMugun, nFormat, nLine, hgwgettext("�ߺд�"), CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	row++, col = 0;

	m_Grid.AddCell(row, col++, hgwgettext("Ư������ ���� (�ٴ���,�����ؼ�)"));
	m_Grid.AddCell(row, col++, &pCheckData->m_bFloorTank, nFormat, nLine, hgwgettext("��ũ"), CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	row++, col = 0;

	m_Grid.AddCell(row, col++, hgwgettext("Ư������ ���� (�ٴ���,�����ؼ�)"));
	m_Grid.AddCell(row, col++, &pCheckData->m_bFloorTrailer, nFormat, nLine, hgwgettext("Ʈ���Ϸ�"), CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	row++, col = 0;

	m_Grid.AddCell(row, col++, hgwgettext("����ӵ� (V)"));
	m_Grid.AddCell(row, col++, &pCheckData->m_dCo_V, nFormat, nLine);
	m_Grid.AddCell(row, col++, _T("km/h"), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	row++, col = 0;

	m_Grid.AddCell(row, col++, hgwgettext("ǳ���߰���"));
	m_Grid.AddCell(row, col++, &pCheckData->m_dWindStrength, nFormat, nLine); m_Grid.SetCellUnit(_T("tonf/m��"), strUnitCurTonfM2);
	m_Grid.AddCell(row, col++, strUnitCurTonfM2, nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, _T("��.��.�� 2.1.11"), nFormat, CELL_READONLY);
	row++, col = 0;

	m_Grid.AddCell(row, col++, hgwgettext("�ؼ��� ǳ���� ���"));
	m_Grid.AddCell(row, col++, &pCheckData->m_bFloowWind, nFormat, nLine, hgwgettext("�ٴ���"), CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	row++, col = 0;

	m_Grid.AddCell(row, col++, hgwgettext("�ؼ��� ǳ���� ���"));
	m_Grid.AddCell(row, col++, &pCheckData->m_bWind, nFormat, nLine, hgwgettext("�ְŴ�"), CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	row++, col = 0;

	sArr.RemoveAll();
	if(bLsd )
		sArr.Add(hgwgettext("�Ѱ���¼����"));
	else
		sArr.Add(hgwgettext("���������"));
	sArr.Add(hgwgettext("�����������"));

	m_Grid.AddCell(row, col++, hgwgettext("�߾Ӻ� ���� ���"));
	m_Grid.AddCell(row, col++, &pCheckData->m_nSlabCentDesignMethod, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	row++, col = 0;

	sArr.RemoveAll();
	sArr.Add(hgwgettext("�հ躰�ִ밪"));
	sArr.Add(hgwgettext("���ߺ��ִ밪"));
	m_Grid.AddCell(row, col++, hgwgettext("�ִ�ݷ� �������"));
	m_Grid.AddCell(row, col++, &pCheckData->m_nCent_HeightType, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	row++, col = 0;

	m_Grid.AddCell(row, col++, hgwgettext("�������� �ݰ� (R)"));
	m_Grid.AddCell(row, col++, &pCheckData->m_dCenter_R[iLEFT], nFormat, nLine); //m_Grid.SetCellUnit(_T("m"), strUnitCurM);
	m_Grid.AddCell(row, col++, _T("m"), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, _T("���� (ĵƿ���� ����)"), nFormat, CELL_READONLY);
	row++, col = 0;

	m_Grid.AddCell(row, col++, hgwgettext("�������� �ݰ� (R)"));
	m_Grid.AddCell(row, col++, &pCheckData->m_dCenter_R[2], nFormat, nLine); //m_Grid.SetCellUnit(_T("m"), strUnitCurM);
	m_Grid.AddCell(row, col++, _T("m"), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, _T("�߾� (�ְŴ� ����)"), nFormat, CELL_READONLY);
	row++, col = 0;

	m_Grid.AddCell(row, col++, hgwgettext("�������� �ݰ� (R)"));
	m_Grid.AddCell(row, col++, &pCheckData->m_dCenter_R[iRIGHT], nFormat, nLine); //m_Grid.SetCellUnit(_T("m"), strUnitCurM);
	m_Grid.AddCell(row, col++, _T("m"), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, _T("���� (ĵƿ���� ����)"), nFormat, CELL_READONLY);
	row++, col = 0;

	m_Grid.AddCell(row, col++, hgwgettext("ö�� �Ǻ�"));
	m_Grid.AddCell(row, col++, &pCheckData->m_dCoverUp, nFormat, nLine);
	m_Grid.AddCell(row, col++, _T("mm"), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, _T("���"), nFormat, CELL_READONLY);
	row++, col = 0;

	m_Grid.AddCell(row, col++, hgwgettext("ö�� �Ǻ�"));
	m_Grid.AddCell(row, col++, &pCheckData->m_dCoverDn, nFormat, nLine);
	m_Grid.AddCell(row, col++, _T("mm"), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, _T("�ϸ�"), nFormat, CELL_READONLY);
	row++, col = 0;

	m_Grid.AddCell(row, col++, hgwgettext("ö�� �Ǻ�"));
	m_Grid.AddCell(row, col++, &pCheckData->m_dCoverSd, nFormat, nLine);
	m_Grid.AddCell(row, col++, _T("mm"), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, _T("����"), nFormat, CELL_READONLY);
	row++, col = 0;

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(80);
	m_Grid.SetColumnWidth(0, 250);
	m_Grid.SetColumnWidth(1, 120);
	m_Grid.SetColumnWidth(3, 120);
}

void CCheckCantileverDlg::SetGridData_WorkingLoad()
{
	CRcBridgeData *pBri	= m_pBri->m_pARcBridgeDataStd->GetBridge(m_nCurrentBridge);
	if( pBri == NULL ) return;

	CCheckCantileverData *pCheckData = pBri->m_pCheckCantilever;
	if( pCheckData == NULL ) return;

	CWorkingLoad *pWorkingLoadL = pCheckData->GetWorkingLoadData(TRUE);
	if( pWorkingLoadL == NULL ) return;

	CWorkingLoad *pWorkingLoadR = pCheckData->GetWorkingLoadData(FALSE);
	if( pWorkingLoadR == NULL ) return;

	HGBOOL bLsd = m_pBri->m_pARcBridgeDataStd->IsLsdDesign();

	CStringArray sArr;
	long row = 0;
	long col = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;
	CString strUnitCurTonfM = bLsd ? _T("N/mm") : _T("kN/m");
	CString strUnitCurTonfM2 = bLsd ? _T("N/mm��") : _T("kN/m��");
	CString strUnitCurM = bLsd ? _T("mm") : _T("m");

	m_Grid.SetGridDefault(2, 2, 1, 2);

	m_Grid.AddCell(row, col++, hgwgettext("����"));
	m_Grid.AddCell(row, col++, hgwgettext("����"));
	m_Grid.AddCell(row, col++, hgwgettext("����"));
	m_Grid.AddCell(row, col++, hgwgettext("����"));
	m_Grid.AddCell(row, col++, hgwgettext("����"));
	m_Grid.AddCell(row, col++, hgwgettext("���"));
	row++, col = 0;

	m_Grid.AddCell(row, col++, hgwgettext("������"));
	m_Grid.AddCell(row, col++, hgwgettext("��ġ"));
	m_Grid.AddCell(row, col++, &pWorkingLoadL->GetUserLoad(BANGUM).m_bExist, nFormat, nLine, hgwgettext("����"), CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, &pWorkingLoadR->GetUserLoad(BANGUM).m_bExist, nFormat, nLine, hgwgettext("����"), CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	row++, col = 0;
	
	m_Grid.AddCell(row, col++, hgwgettext("������"));
	m_Grid.AddCell(row, col++, hgwgettext("����"));
	m_Grid.AddCell(row, col++, &pWorkingLoadL->GetUserLoad(BANGUM).m_dWeight, nFormat, pWorkingLoadL->GetUserLoad(BANGUM).m_bExist ? nLine : CELL_READONLY); m_Grid.SetCellUnit(_T("tonf/m"), strUnitCurTonfM);
	m_Grid.AddCell(row, col++, &pWorkingLoadR->GetUserLoad(BANGUM).m_dWeight, nFormat, pWorkingLoadR->GetUserLoad(BANGUM).m_bExist ? nLine : CELL_READONLY); m_Grid.SetCellUnit(_T("tonf/m"), strUnitCurTonfM);
	m_Grid.AddCell(row, col++, strUnitCurTonfM, nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, hgwgettext("�������̴� �߷�"), nFormat, CELL_READONLY);
	row++, col = 0;

	m_Grid.AddCell(row, col++, hgwgettext("������"));
	m_Grid.AddCell(row, col++, hgwgettext("����"));
	m_Grid.AddCell(row, col++, &pWorkingLoadL->GetUserLoad(BANGUM).m_dHeight, nFormat, pWorkingLoadL->GetUserLoad(BANGUM).m_bExist ? nLine : CELL_READONLY);  m_Grid.SetCellUnit(_T("m"), strUnitCurM);
	m_Grid.AddCell(row, col++, &pWorkingLoadR->GetUserLoad(BANGUM).m_dHeight, nFormat, pWorkingLoadR->GetUserLoad(BANGUM).m_bExist ? nLine : CELL_READONLY);  m_Grid.SetCellUnit(_T("m"), strUnitCurM);
	m_Grid.AddCell(row, col++, strUnitCurM, nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, hgwgettext("��ȣ�� ���� ����"), nFormat, CELL_READONLY);
	row++, col = 0;

	m_Grid.AddCell(row, col++, hgwgettext("������"));
	m_Grid.AddCell(row, col++, hgwgettext("���ϰŸ�"));
	m_Grid.AddCell(row, col++, &pWorkingLoadL->GetUserLoad(BANGUM).m_dDist, nFormat, pWorkingLoadL->GetUserLoad(BANGUM).m_bExist ? nLine : CELL_READONLY);  m_Grid.SetCellUnit(_T("m"), strUnitCurM);
	m_Grid.AddCell(row, col++, &pWorkingLoadR->GetUserLoad(BANGUM).m_dDist, nFormat, pWorkingLoadR->GetUserLoad(BANGUM).m_bExist ? nLine : CELL_READONLY);  m_Grid.SetCellUnit(_T("m"), strUnitCurM);
	m_Grid.AddCell(row, col++, strUnitCurM, nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, hgwgettext("������κ��� �Ÿ�"), nFormat, CELL_READONLY);
	row++, col = 0;

	if ( TRUE == bLsd )
	{
		CString strLeftLoad = ( TRUE == pWorkingLoadL->GetUserLoad(BANGUM).m_bDw ) ? "DW" : "DC";
		CString strRightLoad = ( TRUE == pWorkingLoadR->GetUserLoad(BANGUM).m_bDw ) ? "DW" : "DC";

		m_Grid.AddCell(row, col++, hgwgettext("������"));
		m_Grid.AddCell(row, col++, hgwgettext("��������"));
		m_Grid.AddCell(row, col++, &pWorkingLoadL->GetUserLoad(BANGUM).m_bDw, nFormat, nLine, strLeftLoad, CELL_TYPE_CHECK);
		m_Grid.AddCell(row, col++, &pWorkingLoadR->GetUserLoad(BANGUM).m_bDw, nFormat, nLine, strRightLoad, CELL_TYPE_CHECK);
		m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
		m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
		row++, col = 0;
	}

	m_Grid.AddCell(row, col++, hgwgettext("����"));
	m_Grid.AddCell(row, col++, hgwgettext("��ġ"));
	m_Grid.AddCell(row, col++, &pWorkingLoadL->GetUserLoad(NANGAN).m_bExist, nFormat, nLine, hgwgettext("����"), CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, &pWorkingLoadR->GetUserLoad(NANGAN).m_bExist, nFormat, nLine, hgwgettext("����"), CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	row++, col = 0;

	m_Grid.AddCell(row, col++, hgwgettext("����"));
	m_Grid.AddCell(row, col++, hgwgettext("����"));
	m_Grid.AddCell(row, col++, &pWorkingLoadL->GetUserLoad(NANGAN).m_dWeight, nFormat, pWorkingLoadL->GetUserLoad(NANGAN).m_bExist ? nLine : CELL_READONLY); m_Grid.SetCellUnit(_T("tonf/m"), strUnitCurTonfM);
	m_Grid.AddCell(row, col++, &pWorkingLoadR->GetUserLoad(NANGAN).m_dWeight, nFormat, pWorkingLoadR->GetUserLoad(NANGAN).m_bExist ? nLine : CELL_READONLY); m_Grid.SetCellUnit(_T("tonf/m"), strUnitCurTonfM);
	m_Grid.AddCell(row, col++, strUnitCurTonfM, nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, hgwgettext("�������̴� �߷�"), nFormat, CELL_READONLY);
	row++, col = 0;

	m_Grid.AddCell(row, col++, hgwgettext("����"));
	m_Grid.AddCell(row, col++, hgwgettext("����"));
	m_Grid.AddCell(row, col++, &pWorkingLoadL->GetUserLoad(NANGAN).m_dHeight, nFormat, pWorkingLoadL->GetUserLoad(NANGAN).m_bExist ? nLine : CELL_READONLY); m_Grid.SetCellUnit(_T("m"), strUnitCurM);
	m_Grid.AddCell(row, col++, &pWorkingLoadR->GetUserLoad(NANGAN).m_dHeight, nFormat, pWorkingLoadR->GetUserLoad(NANGAN).m_bExist ? nLine : CELL_READONLY); m_Grid.SetCellUnit(_T("m"), strUnitCurM);
	m_Grid.AddCell(row, col++, strUnitCurM, nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, hgwgettext("���� ��ü ����"), nFormat, CELL_READONLY);
	row++, col = 0;

	m_Grid.AddCell(row, col++, hgwgettext("����"));
	m_Grid.AddCell(row, col++, hgwgettext("���ϰŸ�"));
	m_Grid.AddCell(row, col++, &pWorkingLoadL->GetUserLoad(NANGAN).m_dDist, nFormat, pWorkingLoadL->GetUserLoad(NANGAN).m_bExist ? nLine : CELL_READONLY); m_Grid.SetCellUnit(_T("m"), strUnitCurM);
	m_Grid.AddCell(row, col++, &pWorkingLoadR->GetUserLoad(NANGAN).m_dDist, nFormat, pWorkingLoadR->GetUserLoad(NANGAN).m_bExist ? nLine : CELL_READONLY); m_Grid.SetCellUnit(_T("m"), strUnitCurM);
	m_Grid.AddCell(row, col++, strUnitCurM, nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, hgwgettext("������κ��� �Ÿ�"), nFormat, CELL_READONLY);
	row++, col = 0;

	if ( TRUE == bLsd )
	{
		CString strLeftLoad = ( TRUE == pWorkingLoadL->GetUserLoad(NANGAN).m_bDw ) ? "DW" : "DC";
		CString strRightLoad = ( TRUE == pWorkingLoadR->GetUserLoad(NANGAN).m_bDw ) ? "DW" : "DC";

		m_Grid.AddCell(row, col++, hgwgettext("����"));
		m_Grid.AddCell(row, col++, hgwgettext("��������"));
		m_Grid.AddCell(row, col++, &pWorkingLoadL->GetUserLoad(NANGAN).m_bDw, nFormat, nLine, strLeftLoad, CELL_TYPE_CHECK);
		m_Grid.AddCell(row, col++, &pWorkingLoadR->GetUserLoad(NANGAN).m_bDw, nFormat, nLine, strRightLoad, CELL_TYPE_CHECK);
		m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
		m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
		row++, col = 0;
	}

	m_Grid.AddCell(row, col++, hgwgettext("����"));
	m_Grid.AddCell(row, col++, hgwgettext("��ġ"));
	m_Grid.AddCell(row, col++, &pWorkingLoadL->GetBodoWeight().m_bExist, nFormat, nLine, hgwgettext("����"), CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, &pWorkingLoadR->GetBodoWeight().m_bExist, nFormat, nLine, hgwgettext("����"), CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	row++, col = 0;

	m_Grid.AddCell(row, col++, hgwgettext("����"));
	m_Grid.AddCell(row, col++, hgwgettext("TL1(����)"));
	m_Grid.AddCell(row, col++, &pWorkingLoadL->GetBodoWeight().m_dWeightStt, nFormat, pWorkingLoadL->GetBodoWeight().m_bExist ? nLine : CELL_READONLY); m_Grid.SetCellUnit(_T("tonf/m��"), strUnitCurTonfM2);
	m_Grid.AddCell(row, col++, &pWorkingLoadR->GetBodoWeight().m_dWeightStt, nFormat, pWorkingLoadR->GetBodoWeight().m_bExist ? nLine : CELL_READONLY); m_Grid.SetCellUnit(_T("tonf/m��"), strUnitCurTonfM2);
	m_Grid.AddCell(row, col++, strUnitCurTonfM2, nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, hgwgettext("�����߷� x ����"), nFormat, CELL_READONLY);
	row++, col = 0;

	m_Grid.AddCell(row, col++, hgwgettext("����"));
	m_Grid.AddCell(row, col++, hgwgettext("TL2(����)"));
	m_Grid.AddCell(row, col++, &pWorkingLoadL->GetBodoWeight().m_dWeightEnd, nFormat, pWorkingLoadL->GetBodoWeight().m_bExist ? nLine : CELL_READONLY); m_Grid.SetCellUnit(_T("tonf/m��"), strUnitCurTonfM2);
	m_Grid.AddCell(row, col++, &pWorkingLoadR->GetBodoWeight().m_dWeightEnd, nFormat, pWorkingLoadR->GetBodoWeight().m_bExist ? nLine : CELL_READONLY); m_Grid.SetCellUnit(_T("tonf/m��"), strUnitCurTonfM2);
	m_Grid.AddCell(row, col++, strUnitCurTonfM2, nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, hgwgettext("�����߷� x ����"), nFormat, CELL_READONLY);
	row++, col = 0;

	m_Grid.AddCell(row, col++, hgwgettext("����"));
	m_Grid.AddCell(row, col++, hgwgettext("H1"));
	m_Grid.AddCell(row, col++, &pWorkingLoadL->GetBodoWeight().m_dHeight, nFormat, pWorkingLoadL->GetBodoWeight().m_bExist ? nLine : CELL_READONLY); m_Grid.SetCellUnit(_T("m"), strUnitCurM);
	m_Grid.AddCell(row, col++, &pWorkingLoadR->GetBodoWeight().m_dHeight, nFormat, pWorkingLoadR->GetBodoWeight().m_bExist ? nLine : CELL_READONLY); m_Grid.SetCellUnit(_T("m"), strUnitCurM);
	m_Grid.AddCell(row, col++, strUnitCurM, nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, hgwgettext("ǳ���� ���� ����"), nFormat, CELL_READONLY);
	row++, col = 0;

	m_Grid.AddCell(row, col++, hgwgettext("��������"));
	m_Grid.AddCell(row, col++, hgwgettext("��ġ"));
	m_Grid.AddCell(row, col++, &pWorkingLoadL->GetWalkLoad(), nFormat, nLine, hgwgettext("����"), CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, &pWorkingLoadR->GetWalkLoad(), nFormat, nLine, hgwgettext("����"), CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	row++, col = 0;

	sArr.RemoveAll();
	sArr.Add(hgwgettext("�Ϲݵ���"));
	sArr.Add(hgwgettext("���ɵ���"));
	sArr.Add(hgwgettext("������Է�"));
	m_Grid.AddCell(row, col++, hgwgettext("��������"));
	m_Grid.AddCell(row, col++, hgwgettext("��������"));
	m_Grid.AddCell(row, col++, &pWorkingLoadL->GetHoriType(), nFormat, TRUE, pWorkingLoadL->GetWalkLoad() ? nLine : CELL_READONLY, _T(""), CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col++, &pWorkingLoadR->GetHoriType(), nFormat, TRUE, pWorkingLoadR->GetWalkLoad() ? nLine : CELL_READONLY, _T(""), CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, bLsd ? _T("��.��(2015) 9.7.3") : hgwgettext("��.��.�� 2.4.3.3"), nFormat, CELL_READONLY);
	row++, col = 0;

	m_Grid.AddCell(row, col++, hgwgettext("��������"));
	m_Grid.AddCell(row, col++, hgwgettext("�����"));
	m_Grid.AddCell(row, col++, &pWorkingLoadL->GetHoriF(), (pWorkingLoadL->GetWalkLoad() && pWorkingLoadL->GetHoriType()==2) ? nLine : CELL_READONLY); m_Grid.SetCellUnit(UNIT_CGS_KGF_M, _T("kN/m"));
	m_Grid.AddCell(row, col++, &pWorkingLoadR->GetHoriF(), nFormat, (pWorkingLoadL->GetWalkLoad() && pWorkingLoadR->GetHoriType()==2) ? nLine : CELL_READONLY); m_Grid.SetCellUnit(UNIT_CGS_KGF_M, _T("kN/m"));
	m_Grid.AddCell(row, col++, strUnitCurTonfM, nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, hgwgettext(""), nFormat, CELL_READONLY);
	row++, col = 0;

	m_Grid.AddCell(row, col++, hgwgettext("��������"));
	m_Grid.AddCell(row, col++, hgwgettext("������"));
	m_Grid.AddCell(row, col++, &pWorkingLoadL->GetVertF(), nFormat, nLine); m_Grid.SetCellUnit(UNIT_CGS_KGF_M, _T("kN/m"));
	m_Grid.AddCell(row, col++, &pWorkingLoadR->GetVertF(), nFormat, nLine); m_Grid.SetCellUnit(UNIT_CGS_KGF_M, _T("kN/m"));
	m_Grid.AddCell(row, col++, strUnitCurTonfM, nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, hgwgettext(""), nFormat, CELL_READONLY);
	row++, col = 0;

	sArr.RemoveAll();
	sArr.Add(hgwgettext("��������"));
	sArr.Add(hgwgettext("�̵�����"));
	m_Grid.AddCell(row, col++, hgwgettext("��������"));
	m_Grid.AddCell(row, col++, hgwgettext("��������"));
	m_Grid.AddCell(row, col++, &pWorkingLoadL->GetWorkLoadType(), nFormat, TRUE, CELL_READONLY, _T(""), CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col++, &pWorkingLoadR->GetWorkLoadType(), nFormat, TRUE, CELL_READONLY, _T(""), CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	m_Grid.AddCell(row, col++, _T(""), nFormat, CELL_READONLY);
	row++, col = 0;

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(90);
	m_Grid.SetColumnWidth(5, 120);
}

void CCheckCantileverDlg::SetGridData_LsdCombo(long nTypeLsdCombo)
{
	CARcBridgeDataStd *pStd = m_pBri->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_nCurrentBridge);
	if(!pBri) return;

	m_Grid.SetLsdManager(pBri->m_plsdMngForCantilever);
	m_Grid.SetGridInitLoadCase(static_cast<ELSDLoadGroupKind>(nTypeLsdCombo),  eLSD_LOADCASE_DIR_PER);
}

void CCheckCantileverDlg::SetDataDefault(HGBOOL bAll)
{
	long nIndex = m_Tab.GetCurSel();

	CARcBridgeDataStd *pStd = m_pBri->m_pARcBridgeDataStd;
	if( pStd == NULL ) 
		return ;

	HGBOOL bLsd = pStd->IsLsdDesign();

	if(bAll)
	{
		long nCount = m_pBri->m_plsdMngForCantilever->GetCountLoadCaseByComboKind(eLSD_COMBINE_LIMIT_I, eLSD_LOADCASE_DIR_PER);
		if(nCount > 0)
			bAll = FALSE;
	}
	
	if( bLsd )
	{
		if( nIndex == 0 || bAll ) m_pBri->SetDataDefaultSelectLoadLsd(TRUE);
		if( nIndex == 1 || bAll ) m_pBri->SetDataDefaultLoadFactor(TRUE);
		if( nIndex == 2 || bAll ) m_pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_LIMIT, TRUE);
		if( nIndex == 3 || bAll ) m_pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_EXTREME, TRUE);
		if( nIndex == 4 || bAll ) m_pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_SERVICE, TRUE);
		if( nIndex == 5 || bAll ) m_pBri->SetDataDefaultModifiedFactor(0, TRUE);
	}
}

void CCheckCantileverDlg::OnCellChangedData( NMHDR *pNMHDR, LRESULT* )
{
	CRcBridgeData *pBri	= m_pBri->m_pARcBridgeDataStd->GetBridge(m_nCurrentBridge);
	if( pBri == NULL ) return;

//	CARcBridgeDataStd *pStd = m_pBri->m_pARcBridgeDataStd;

	CCheckCantileverData *pCheckData = m_pBri->m_pCheckCantilever;
	if( pCheckData == NULL ) return;

	HGBOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	CWorkingLoad *pWorkingLoadL = pCheckData->GetWorkingLoadData(TRUE);
	if( pWorkingLoadL == NULL ) return;

	CWorkingLoad *pWorkingLoadR = pCheckData->GetWorkingLoadData(FALSE);
	if( pWorkingLoadR == NULL ) return;

	NM_GRIDVIEW *GridView = (NM_GRIDVIEW *)pNMHDR;
	long nRow = GridView->iRow;
	long nCol = GridView->iColumn;
	long nTab = m_Tab.GetCurSel();

	CCellID cell = m_Grid.GetFocusCell();
	CString str  = m_Grid.GetTextMatrix(cell.row, 0);

	if(nTab == m_nTabLoad)
	{
		HGINT32 ixSelectRow(0);

		if(nCol==2 || nCol==3) //CheckBox
		{
			BOOL bLeft = (nCol==2) ? TRUE : FALSE;
			CWorkingLoad *pWorking = bLeft ? pWorkingLoadL : pWorkingLoadR;

			BOOL   bFind	= TRUE;
			CString szText	= _T("");

			// ��������
			ixSelectRow = bLsd == TRUE ? 11 : 9;
			if(nRow == ixSelectRow)
			{
				BOOL bBodo = m_Grid.IsCheck(ixSelectRow, nCol);
				if(bBodo)
				{
					if( m_pCalc == NULL )
						return;

					bFind			= FALSE;
					double dLen		= 0;
					double dWidth = m_pCalc->GetLengthModelSpanMax();
					long nQtyHDan = pBri->GetQtyHDan();

					for(long n=0; n<nQtyHDan; n++)
					{	
						CGuardWallRC *pGW = pBri->GetGuardWallByHDan(n);
						if( pGW == NULL ) continue;

						if( pGW->m_nTypeGuardWall == 4 && (bLeft ? (dLen < dWidth/2) : (dLen >= dWidth/2)) )
						{
							bFind = TRUE;
							break;
						}
						dLen += pGW->GetWidth();
					}

					if(!bFind)
					{
						pWorking->GetBodoWeight().m_bExist = FALSE;
						szText.Format("Ⱦ�� ������ �������� %s�� �����κ��� �������� �ʾҽ��ϴ�.", bLeft?"��":"��");
						SetDataInit();
					}
				}
				else
				{
					pWorking->GetBodoWeight().m_bExist		= FALSE;
					pWorking->GetWalkLoad()				= FALSE;
					pWorking->GetBodoWeight().m_dWeightStt	= 0;
					pWorking->GetBodoWeight().m_dWeightEnd	= 0;
					pWorking->GetBodoWeight().m_dHeight		= 0;
					pWorking->GetHoriF()					= 0;
					pWorking->GetVertF()					= 0;

					SetDataInit();
//					DrawInputDomyunView(FALSE);
				}
			}

			//������, ����
			ixSelectRow = bLsd == TRUE ? 6 : 5;
			if((nRow == 1 || nRow == ixSelectRow))
			{	
				BOOL bBangUm = m_Grid.IsCheck(1, nCol);
				BOOL bNanGan = m_Grid.IsCheck(ixSelectRow, nCol);
				if(bBangUm && bNanGan)
				{
					bFind = FALSE;
					szText.Format("�������� ������ ���ÿ� ��ġ�� �� �����ϴ�.");
					SetDataInit();
				}
				else
				{
					SetDataSave();
					if(!pWorking->GetUserLoad(BANGUM).m_bExist)
					{
						pWorking->GetUserLoad(BANGUM).m_dWeight	= 0;
						pWorking->GetUserLoad(BANGUM).m_dHeight	= 0;
						pWorking->GetUserLoad(BANGUM).m_dDist		= 0;
					}
					if(!pWorking->GetUserLoad(NANGAN).m_bExist)
					{
						pWorking->GetUserLoad(NANGAN).m_dWeight	= 0;
						pWorking->GetUserLoad(NANGAN).m_dHeight	= 0;
						pWorking->GetUserLoad(NANGAN).m_dDist		= 0;
					}

					SetDataInit();
//					DrawInputDomyunView(FALSE);
				}
			}

			//��������
			ixSelectRow = bLsd == TRUE ? 15 : 13;
			if(nRow==ixSelectRow)
			{
				SetDataSave();
				BOOL bWalkLoad = m_Grid.IsCheck(ixSelectRow, nCol);

				if(!bWalkLoad)
				{
					pWorking->GetHoriF()	= 0;
					pWorking->GetVertF()	= 0;
				}
				else
				{
					if(pWorking->GetHoriType() == 0)
						pWorking->GetHoriF() = 250;
					else						
						pWorking->GetHoriF() = 375;

					pWorking->GetVertF() = 100;
				}
				SetDataInit();
//				DrawInputDomyunView(FALSE);
			}

			ixSelectRow = bLsd == TRUE ? 16 : 14;
			if(nRow == ixSelectRow) //��������
			{
				SetDataSave();
				CString szCombo = m_Grid.GetTextMatrix(ixSelectRow, nCol);

				if(szCombo == "�Ϲݵ���")
				{
					if(pWorking->GetWalkLoad())
					{
						pWorking->GetHoriF() = 250;
						pWorking->GetVertF() = 100;
					}
				}
				else if(szCombo == "���ɵ���")
				{
					if(pWorking->GetWalkLoad())
					{
						pWorking->GetHoriF() = 375;
						pWorking->GetVertF() = 100;
					}
				}

				SetDataInit();
			}

			if(!bFind)
			{
				AfxMessageBox(szText, MB_ICONEXCLAMATION);
				return;
			}
		}
	}
	else if( m_nTabDesign == nTab )
	{
		if( str == "ǳ���߰���" )
		{
			CString strValue = m_Grid.GetTextMatrix(cell.row, cell.col);
			double dVal = atof(strValue);

			if( dVal < 3 || 10 < dVal )
			{
				SetGridData();
				SetDataInit();
				return;
			}
		}
		else if( str == "�������� �ݰ� (R)" )
		{
			CString strValue = m_Grid.GetTextMatrix(cell.row, cell.col);
			double dVal = atof(strValue);

			if( 10000 < dVal )
			{
				SetGridData();
				SetDataInit();
				return;
			}
		}
	}

	SetDataSave();
	SetGridData();
	SetDataInit();

// 	else if(nTab == m_nTabSelLoad)
// 	{
// 		if(str.Find(_T("EH")) != -1)
// 		{
// 			CRcBridgeApp *pBriApp = pStd->GetBridge(m_nCurrentBridge);
// 			if( pBriApp == NULL ) return;
// 
// 			if(pBriApp)
// 			{
// 				SetDataSave();
// 				pBriApp->m_plsdMngForCantilever->SetLoadDirOption();
// 				m_Grid.UpdateData(FALSE);
// 			}
// 		}
// 	}
}

void CCheckCantileverDlg::SetGridData_Eta()
{
	CARcBridgeDataStd *pStd = m_pBri->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_nCurrentBridge);
	if(!pBri) return;

	m_Grid.SetLsdManager(pBri->m_plsdMngForCantilever);
	m_Grid.SetGridLoadModified(0);
}
