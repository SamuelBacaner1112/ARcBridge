// InputBaseCrossPosition.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "../ARcBridgeDBDraw/ARcBridgeDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputBaseCrossPosition dialog


CInputBaseCrossPosition::CInputBaseCrossPosition(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputBaseCrossPosition::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputBaseCrossPosition)
	m_bViewAllDir = TRUE;
	m_bAutoDefaultSlab = FALSE;

	m_bUnderGroundPaveDetail = FALSE;
	//}}AFX_DATA_INIT
}


void CInputBaseCrossPosition::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputBaseCrossPosition)
	DDX_Control(pDX, IDC_BUTTON_APPLY_MIRROR, m_btnMirror);
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_cbBridgeNo);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Check(pDX, IDC_CHECK_VIEW_COUNTER_DIR, m_bViewAllDir);
	DDX_Check(pDX, IDC_CHECK_AUTO_HDAN, m_bAutoDefaultSlab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputBaseCrossPosition, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputBaseCrossPosition)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED_COL,  IDC_GRID, OnCellChangedCol)
	ON_NOTIFY(GVN_CELL_CHANGED,  IDC_GRID, OnCellChanged)
	ON_BN_CLICKED(IDC_CHECK_VIEW_COUNTER_DIR, OnCheckViewCounterDir)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_MIRROR, OnButtonApplyMirror)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputBaseCrossPosition message handlers

BOOL CInputBaseCrossPosition::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		pMsg->wParam = VK_TAB;
	}
	
	return CInputDialogBase::PreTranslateMessage(pMsg);
}

void CInputBaseCrossPosition::OnPreInitDialog()
{

	SetResize(IDC_GRID,	SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_CHECK_VIEW_COUNTER_DIR, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_AUTO_HDAN, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_APPLY_MIRROR, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetHyperHelp(m_pIn->GetManualPath());

	InitComboCtrl();
	m_Grid.InitGrid();
	SetGridTitle();
	SetDataInit();
}

void CInputBaseCrossPosition::InitComboCtrl()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	int nSize = pStd->GetBridgeSize();

	CString str = _T("");
	int i=0; for(i=0; i<nSize; i++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(i);
		str = pStd->GetStringBridgeOrder(i) + _T(" : ") + pBri->m_strBridgeName
			+ _T(" [") + pBri->GetStringBridgeType() + _T("]");
		m_cbBridgeNo.AddString(str);
	}
	m_cbBridgeNo.SetCurSel(m_pIn->m_nCurrentBridge);
}

void CInputBaseCrossPosition::OnSelchangeComboBridgeNo() 
{
	SetDataSave();
	m_pIn->m_nCurrentBridge = m_cbBridgeNo.GetCurSel();
	SetGridTitle();
	SetDataInit();
	SetPaneTitle();
	DrawInputDomyunView(TRUE);
}

void CInputBaseCrossPosition::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	
	int nRow = pGridView->iRow;
	int nCol = pGridView->iColumn;

	SetDataSave();
	SetDataInit();

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	
	if(m_bAutoDefaultSlab)
		pBri->SetDataDefaultHDanSlab();

	// ���� ��ȣ ����� ����(Ȯ��)�޽��� ���
	CString sTitle	= m_Grid.GetTextMatrix(0, nCol);
	if(sTitle.Find("����", 0) != -1)
	{
		m_pIn->MessageExpAndGagak(TRUE, &m_Grid, nRow, nCol);
	}

	//DrawInputDomyunView();

	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);


}

void CInputBaseCrossPosition::OnCellChangedCol(NMHDR* pNotifyStruct, LRESULT* result)
{
	DrawInputDomyunView();
}

void CInputBaseCrossPosition::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	
	int nCol = pGridView->iColumn;

	CString str = (m_Grid.GetTextMatrix(0, nCol)).Left(2);
	m_pView->GetDomyun()->STMakeCursor(str);

	CString strComment = _T("");
	CString strCol  = m_Grid.GetTextMatrix(0,nCol);
	if(strCol=="���� ����")		strComment = "���������� ���� �߽ɼ����� �����մϴ�.";
	if(strCol=="������ ����")	strComment = "������ �����ܿ� ���� ���� ������ �����մϴ�. (Ȯ�������� �ƴ� ��� ���ؼ����� ����)";
	if(strCol=="������ ����")	strComment = "������ �����ܿ� ���� ���� ������ �����մϴ�. (Ȯ�������� �ƴ� ��� ���ؼ����� ����)";
	if(strCol=="SL")			strComment = "�����߽��� �������� ������ �����ܱ����� �Ÿ��� �Է��մϴ�. (-)����,(+)����, ���� mm";
	if(strCol=="SR")			strComment = "�����߽��� �������� ������ �����ܱ����� �Ÿ��� �Է��մϴ�. (-)����,(+)����, ���� mm";

	m_pIn->SetComment(strComment);
}

void CInputBaseCrossPosition::DrawInputDomyunView(BOOL bZoomAll)
{
	// �⺻�Է½ô� ����� �Ź� ��������.
	m_pIn->SettingNeedPlane();

	for(long bri = 0; bri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		m_pIn->m_pARcBridgeDataStd->GetBridge(bri)->InitSaveCrossSection();
		m_pIn->m_pARcBridgeDataStd->GetBridge(bri)->InitSaveXySlabUpperByLenFrSttStation();
		m_pIn->m_pARcBridgeDataStd->GetBridge(bri)->InitSaveXyMatchLineAndSlabUpper();
	}

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(m_pIn->GetScaleInputDialog());

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	CDomyun Dom(pDom);

	long nJijum = 0;
	double sta = pBri->GetStationOnJijum(nJijum);
	CDPoint vAng = CDPoint(0,1);
	if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pBri->GetJijum(0)->m_dLengthUgan[1];
	if(pBri->m_bIsExp || pBri->IsExtLine())
	{
		sta = pBri->GetStationBridgeMid();
	}

	
	int nSeparBri = pBri->m_nSeparBri;// ������ �и� ����(0:������ ��ü, 1:���༱, 2:���༱)

	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);
	m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DBStd.m_pDrawCrossSection->DrawSimpleCross(&Dom, sta, TRUE, TRUE, TRUE);
	m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	DBStd.m_pDrawCrossSection->DimCrossDistSlab(&Dom, TRUE, m_bViewAllDir, FALSE, sta);

	if(nSeparBri != 0 && m_pIn->m_bViewAllDir == TRUE)
	{
		if(nSeparBri == 1)
			pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge+1);	//���༱ �� ���, ���༱(���༱+1)�� ǥ��, ���� ������ ���༱ ǥ��
		else
			pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge-1);	//���༱ �� ���, ���༱(���༱-1)�� ǥ��, ���� ������ ���༱ ǥ��

		CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);
		m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
		DBStd.m_pDrawCrossSection->DrawSimpleCross(&Dom, sta, FALSE, TRUE, TRUE);
		m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		DBStd.m_pDrawCrossSection->DimCrossSectionFullLeftRight(&Dom, pBri, pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge), 3);
		//DBStd.m_pDrawCrossSection->DimCrossDistSlab(&Dom, FALSE, m_bViewAllDir, TRUE, sta);
	}
	*pDom << Dom;
	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

void CInputBaseCrossPosition::SetGridTitle()
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2,2,1,0);

	CARcBridgeDataStd *pStd		= m_pIn->m_pARcBridgeDataStd;
	CLineInfoApp	  *pLineApp = pStd->GetLineApp();
	CRcBridgeApp	  *pBri		= pStd->GetBridge(m_pIn->m_nCurrentBridge);

	long row = 1;
	long col = -1;
	CString str = _T("");

	CStringArray sArrLine;
	long nLineCount = pLineApp->GetLinesu();
	int i=0; for(i=0; i<nLineCount; i++)
	{
		CLineInfo* pLine = pLineApp->GetLine(i);
		sArrLine.Add(pLine->GetNameLineEach());
	}
	
	UINT nFormat = DT_CENTER|DT_VCENTER;
	m_Grid.AddCell(0, ++col,_T("���� ����"));	m_Grid.AddCell(row, col, &pBri->m_nLineBase,  nFormat, TRUE, CELL_READONLY, "", CELL_TYPE_COMBO, sArrLine);
	m_Grid.AddCell(0, ++col,_T("������ ����"));	m_Grid.AddCell(row, col, &pBri->m_nLineLeft,  nFormat, TRUE, CELL_LINE_NO , "", CELL_TYPE_COMBO, sArrLine);
	m_Grid.AddCell(0, ++col,_T("������ ����"));	m_Grid.AddCell(row, col, &pBri->m_nLineRight, nFormat, TRUE, CELL_LINE_NO , "", CELL_TYPE_COMBO, sArrLine);
	m_Grid.AddCell(0, ++col,_T("SL"));			m_Grid.AddCell(row, col, &pBri->m_dWidthSlabLeft);
	m_Grid.AddCell(0, ++col,_T("SR"));			m_Grid.AddCell(row, col, &pBri->m_dWidthSlabRight);

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(65);
	m_Grid.SetColumnWidth(0,120);
	m_Grid.SetColumnWidth(1,120);
	m_Grid.SetColumnWidth(2,120);
}

void CInputBaseCrossPosition::SetDataDefault()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	for(long bri = 0; bri < pStd->GetBridgeSize(); bri++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(bri);
		BOOL bFirst   = IsFirst(bri);

		/*
		// ���ܻ��� : std�� flag�� 0�ΰ��� �̹� Ⱦ����ġ�Է¿��� �ǰ���� �� ������ ģ��. /////
		if(pStd->m_nFlag < 1)
			bFirst	= FALSE;
		//////////////////////////////////////////////////////////////////////////////////////////
		*/

		pBri->SetDefaultedState(GetIDD(), 0x00000001);

		if(bFirst || IsClickDefaultButton(bri))
		{
			pBri->SetDataDefaultCrossPosition();
			pBri->SetDataDefaultHDanUnderMember();
			pBri->SetDataDefaultHDanSlab();
			pBri->m_bNeedSettingVertSection	= TRUE;
		}
	}
	SetGridTitle();
	SetDataInit();
}

void CInputBaseCrossPosition::SetDataInit()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	m_Grid.UpdateData(FALSE);
	m_Grid.SetRedraw(TRUE, TRUE);

	m_btnMirror.EnableWindow(pBri->m_nSeparBri!=0);
	m_bViewAllDir  = m_pIn->m_bViewAllDir;

	UpdateData(FALSE);
	AfxGetMainWnd()->SendMessage(WM_USER+12);
}

void CInputBaseCrossPosition::SetDataSave()
{
	UpdateData(TRUE);
	m_Grid.UpdateData(TRUE);

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	pBri->m_bNeedSettingVertSection	= TRUE;

	m_pIn->m_bViewAllDir = m_bViewAllDir;
	m_pIn->m_pDoc->SetModifiedFlag();
	AfxGetMainWnd()->SendMessage(WM_USER+12);
}

void CInputBaseCrossPosition::OnCheckViewCounterDir() 
{
	UpdateData(TRUE);
	m_pIn->m_bViewAllDir = m_bViewAllDir;
	DrawInputDomyunView();

}

void CInputBaseCrossPosition::OnButtonApplyMirror() 
{
	// TODO: Add your control notification handler code here
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(pBri==NULL || pBri->m_nSeparBri==0) return;

	CRcBridgeApp* pBriMatch = pStd->GetBridgeMatch(pBri);
	if(pBriMatch==NULL || pBriMatch->m_nSeparBri==0) return;

	CString strMsg = _T("");
	strMsg.Format("%s��(��) �����߽��� �������� ��ġ����˴ϴ�.\n����Ͻðڽ��ϱ� ?",pBriMatch->m_strBridgeName);
	if(AfxMessageBox(strMsg,MB_YESNO)==IDYES)
	{
		pBri->m_dWidthSlabRight = -pBriMatch->m_dWidthSlabLeft;
		pBri->m_dWidthSlabLeft  = -pBriMatch->m_dWidthSlabRight;
		if(m_bAutoDefaultSlab)
			pBri->SetDataDefaultHDanSlab();
		SetDataInit();
		DrawInputDomyunView();
	}
}
