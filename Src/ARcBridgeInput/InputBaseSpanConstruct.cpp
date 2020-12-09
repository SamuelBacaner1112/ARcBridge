// InputBaseSpanConstruct.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "../ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "DialogSetExpJointSpace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputBaseSpanConstruct dialog


CInputBaseSpanConstruct::CInputBaseSpanConstruct(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputBaseSpanConstruct::IDD, pParent)
{
	m_bViewAllDir = TRUE;
	//{{AFX_DATA_INIT(CInputBaseSpanConstruct)
	m_bExp = FALSE;
	m_nTabExpJoint = -1;
	m_nTabAngle = -1;
	m_nTabExtLine = -1;
	m_nTabGagak = -1;
	m_nTabSpan = -1;
	//}}AFX_DATA_INIT
}


void CInputBaseSpanConstruct::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputBaseSpanConstruct)
	DDX_Control(pDX, IDC_TAB, m_ctlTab);
	DDX_Control(pDX, IDC_BUTTON_SAME_ANGLE, m_btSameAngle);
	DDX_Control(pDX, IDC_BUTTON_SAME_EXP_JOINT, m_btSameExpJoint);
	DDX_Control(pDX, IDC_BUTTON_AUTO_RADIUS_GAGAK, m_btnAutoRadius);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_cbBridgeNo);
	DDX_Check(pDX, IDC_CHECK_VIEW_COUNTER_DIR, m_bViewAllDir);
	DDX_Check(pDX, IDC_CHECK_EXP, m_bExp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputBaseSpanConstruct, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputBaseSpanConstruct)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_BN_CLICKED(IDC_BUTTON_SAME_ANGLE, OnButtonSameAngle)
	ON_BN_CLICKED(IDC_BUTTON_SAME_EXP_JOINT, OnButtonSameExpJoint)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_RADIUS_GAGAK, OnButtonAutoRadiusGagak)
	ON_BN_CLICKED(IDC_BUTTON_EXP_JOINT, OnButtonCheckExpJoint)
	ON_BN_CLICKED(IDC_BUTTON_REF_EXP_JOINT, OnButtonRefExpJoint)
	ON_BN_CLICKED(IDC_BUTTON_CALC_EXPANSION, OnBnClickedButtonCalcExpansion)
	ON_BN_CLICKED(IDC_CHECK_VIEW_COUNTER_DIR, OnCheckViewCounterDir)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnSelchangingTab)
	ON_BN_CLICKED(IDC_CHECK_EXP, OnCheckExp)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_MESSAGE(EXPANSION_RECOMMEND, OnExpansionRecommend)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputBaseSpanConstruct message handlers

BOOL CInputBaseSpanConstruct::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		pMsg->wParam = VK_TAB;
	}
	
	return CInputDialogBase::PreTranslateMessage(pMsg);
}

void CInputBaseSpanConstruct::OnPreInitDialog() 
{
	m_btSameAngle.ShowWindow(SW_SHOW);
//	m_btnAutoRadius.ShowWindow(SW_HIDE);

	SetResize(IDC_GRID						, SZ_TOP_LEFT	, SZ_BOTTOM_RIGHT);
	SetResize(IDC_TAB						, SZ_TOP_LEFT	, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_SAME_ANGLE			, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_SAME_EXP_JOINT			, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_AUTO_RADIUS_GAGAK	, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_EXP_JOINT	, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_REF_EXP_JOINT	, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_CALC_EXPANSION, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_VIEW_COUNTER_DIR	, SZ_TOP_RIGHT	, SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_EXP					, SZ_TOP_RIGHT	, SZ_TOP_RIGHT);

	SetHyperHelp(m_pIn->GetManualPath());

	InitComboCtrl();
	InitTabCtrl();
	m_Grid.InitGrid();
	SetGridData();
	SetControl();

	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	m_dScaleDefault	= MAX(100,toM(pBri->m_dLengthBridge)/30*100); // 30M������ �������� 1:100, �ּ� 100;
}

void CInputBaseSpanConstruct::InitComboCtrl()
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

void CInputBaseSpanConstruct::OnSelchangeComboBridgeNo() 
{
	SetDataSave();
	m_pIn->m_nCurrentBridge = m_cbBridgeNo.GetCurSel();
	InitTabCtrl();
	SetGridData();
	SetDataInit();
	SetPaneTitle();
	SetControl();

	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	m_dScaleDefault	= MAX(100,toM(pBri->m_dLengthBridge)/30*100); // 30M������ �������� 1:100, �ּ� 100;
	m_pIn->SetScaleInputDialog(m_dScaleDefault);

	DrawInputDomyunView(TRUE);
}

void CInputBaseSpanConstruct::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	
	int nRow = pGridView->iRow;
	int nCol = pGridView->iColumn;

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	int nSeparBri = pBri->m_nSeparBri;

	SetDataSave();

	int nTab = m_ctlTab.GetCurSel();
	if(nTab==m_nTabAngle)
	{
		if(nRow == 1)
		{
			double dAng = atof(m_Grid.GetTextMatrix(nRow, nCol));
			if(dAng < -180 || dAng > 180)
			{
				double dModify = atof(m_Grid.GetLastChangeData());
				m_Grid.SetTextMatrix(nRow, nCol, _T("%.3f"), dModify);

				m_Grid.SetRedraw(TRUE, TRUE);
				return;
			}
			else if(dAng < 0)
			{
				m_Grid.SetTextMatrix(nRow, nCol, _T("%.3f"), 180 + dAng);
				SetDataSave();
			}
		}

		for(int nSepar=-1; nSepar<2; nSepar++)
		{
			if(nSeparBri==0 && nSepar!=0) continue;
			if(nSeparBri==1 && nSepar==-1) continue;
			if(nSeparBri==2 && nSepar==1) continue;
			pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge+nSepar);

			if(nRow==1)
			{
				// ������ Elevation ����
				int nCols = m_Grid.GetCols();
				if(pBri->m_bAutoWingEl && (nCol==1 || nCol==nCols-1))
				{
					CWingWall* pWing = pBri->GetWingWall(TRUE, TRUE);
					if(pWing)
						pWing->m_dEL = Round(pBri->GetElWingWallEnd(TRUE, TRUE), 0);
					pWing = pBri->GetWingWall(TRUE, FALSE);
					if(pWing)
						pWing->m_dEL = Round(pBri->GetElWingWallEnd(TRUE, FALSE), 0);
					pWing = pBri->GetWingWall(FALSE, TRUE);
					if(pWing)
						pWing->m_dEL = Round(pBri->GetElWingWallEnd(FALSE, TRUE), 0);
					pWing = pBri->GetWingWall(FALSE, FALSE);
					if(pWing)
						pWing->m_dEL = Round(pBri->GetElWingWallEnd(FALSE, FALSE), 0);
				}

				// ���� ���� ����
				int nCountFooting = pBri->GetCountInWall() + 2;
				int i=0; for(i=0; i<nCountFooting; i++)
					pBri->SetFootingEndShape(i);
			}
			if(nRow==2)	//���� ����, ������ �����Ǿ��� ��쿡�� �������� �ʱ�ȭ ��.
			{
				pBri->SetDataDefaultJijumLength();
				pBri->SetDataDefaultCountShoe();
				SetGridData();
			}
		}		
	}
	if(nTab==m_nTabSpan)
	{
		double dModify = atof(m_Grid.GetLastChangeData());

//		UINT nState = 0;
		int nCols = m_Grid.GetCols();
		int i=0; for(i=1; i<nCols; i++)
		{
			structGridCell *pCell = m_Grid.GetCell(1, i);
			if(pCell && pCell->nLine == CELL_READONLY)
				break;
		}
		int nReadOnly = i;
//		int nReadOnly = nCols-1;		// ������ ���� �������� ReadOnly�̹Ƿ�...
	
		double dLenSum = 0;
		for(i=1; i<nCols; i++)
		{
			if(i==nReadOnly)
				continue;
			dLenSum += atof(m_Grid.GetTextMatrix(1, i));
		}

		for(int nSepar=-1; nSepar<2; nSepar++)
		{
			if(nSeparBri==0 && nSepar!=0) continue;
			if(nSeparBri==1 && nSepar==-1) continue;
			if(nSeparBri==2 && nSepar==1) continue;
			pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge+nSepar);

			double dSpareLen = pBri->m_dLengthBridge - dLenSum;
			CString str = _T("");
			if(dSpareLen<=0)
			{
				AfxMessageBox("�Էµ� ���� �Ÿ��� ���� ������ ���̺��� Ů�ϴ�. ���� �Ÿ��� �ٽ� �Է��Ͽ� �ֽʽÿ�.  ");
				m_Grid.SetTextMatrix(nRow, nCol, _T("%.f"), dModify);

				m_Grid.SetRedraw(TRUE, TRUE);
				return;
			}
			str.Format("%.f", dSpareLen);
			m_Grid.SetItemText(1, nReadOnly, str);
			m_Grid.SetRedraw(TRUE, TRUE);
		}
		SetDataSave();
	}
	if(nTab==m_nTabExpJoint)
	{
		SetGridData();
	}
	SetDataInit();

	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);
}

void CInputBaseSpanConstruct::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;

	CARcBridgeDataStd *pStd		= m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp	  *pBri		= pStd->GetBridge(m_pIn->m_nCurrentBridge);
	
	int nCol = pGridView->iColumn;

	CString str = m_Grid.GetTextMatrix(0, nCol);
	int nTab = m_ctlTab.GetCurSel();
	if(nTab == m_nTabGagak) 
		str = m_Grid.GetTextMatrix(1, nCol);
	m_pView->GetDomyun()->STMakeCursor(str);

	CString strComment = _T("");
	if(nTab==m_nTabAngle)
	{
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		{
			strComment	= "��ġ���� �米�� �������� �ʽ��ϴ�.";
		}
		else
		{
			strComment.Format("����(%s)�� �����߽ɼ��� ���� ������ �Է��մϴ�. (���� : ��)",str);
		}
	}
	else if(nTab==m_nTabSpan)
	{
		strComment.Format(pBri->GetStringJigan() + "����ġ��(%s)�� �Է��մϴ�. �����߽� Station �Ÿ� (���� : mm)",str);
	}
	else if(nTab==m_nTabGagak)
	{
		if(str=="sDL")	strComment = "�����߽��� ���������� ����ó�� ���� (������ ����, ���� : mm)";
		if(str=="sHL")	strComment = "������ �����ܿ� ���� �̰ݰŸ� (������ ����, ���� : mm)";
		if(str=="sRL")	strComment = "����ó������ � �ݰ� (������ ����, ���� : mm)";
		if(str=="sDR")	strComment = "�����߽��� ���������� ����ó�� ���� (������ ����, ���� : mm)";
		if(str=="sHR")	strComment = "������ �����ܿ� ���� �̰ݰŸ� (������ ����, ���� : mm)";
		if(str=="sRR")	strComment = "����ó������ � �ݰ� (������ ����, ���� : mm)";
		//
		if(str=="eDL")	strComment = "�����߽��� ���������� ����ó�� ���� (������ ����, ���� : mm)";
		if(str=="eHL")	strComment = "������ �����ܿ� ���� �̰ݰŸ� (������ ����, ���� : mm)";
		if(str=="eRL")	strComment = "����ó������ � �ݰ� (������ ����, ���� : mm)";
		if(str=="eDR")	strComment = "�����߽��� ���������� ����ó�� ���� (������ ����, ���� : mm)";
		if(str=="eHR")	strComment = "������ �����ܿ� ���� �̰ݰŸ� (������ ����, ���� : mm)";
		if(str=="eRR")	strComment = "����ó������ � �ݰ� (������ ����, ���� : mm)";
		//
		if(str=="WS")	strComment = "������ ��ü �β� : ������౳ (���� : mm) , ����ó���� ��� [�ܸ� ���� �Է�]�� ����";
		if(str=="WE")	strComment = "������ ��ü �β� : ������౳ (���� : mm) , ����ó���� ��� [�ܸ� ���� �Է�]�� ����";
	}

	m_pIn->SetComment(strComment);
}


void CInputBaseSpanConstruct::DrawInputDomyunView(BOOL bZoomAll)
{
	// �⺻�Է½ô� ����� �Ź� ��������.
	m_pIn->SettingNeedPlane();

	for(long bri = 0; bri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		m_pIn->m_pARcBridgeDataStd->GetBridge(bri)->InitSaveCrossSection();
		m_pIn->m_pARcBridgeDataStd->GetBridge(bri)->InitSaveXySlabUpperByLenFrSttStation();
		m_pIn->m_pARcBridgeDataStd->GetBridge(bri)->InitSaveXyMatchLineAndSlabUpper();
	}

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	int nSeparBri = pBri->m_nSeparBri;

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(m_pIn->GetScaleInputDialog());

	if(!pBri->IsBriInLine())
		return;

	int nTab = m_ctlTab.GetCurSel();

	CDomyun Dom(pDom);

	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);

	DBStd.m_pDrawPlane->DrawPlaneGen(&Dom);
	if(nTab==m_nTabAngle)
	{
		DBStd.m_pDrawPlane->DimPlaneJijumAngle(&Dom);
		DBStd.m_pDrawPlane->DimPlaneJijumLength(&Dom,FALSE,FALSE,TRUE);
	}
	if(nTab==m_nTabSpan || nTab==m_nTabExpJoint)
	{
		DBStd.m_pDrawPlane->DimPlaneJijumAngle(&Dom,FALSE);
		DBStd.m_pDrawPlane->DimPlaneJijumLength(&Dom);
	}
	if(nTab==m_nTabGagak)
		DBStd.m_pDrawPlane->DimPlaneJijumGagak(&Dom);

	DBStd.m_pDrawPlane->MarkPlaneBridgeSttEnd(&Dom);
	if(nSeparBri != 0 && m_pIn->m_bViewAllDir == TRUE)
	{
		if(nSeparBri == 1)
			pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge+1);	//���༱ �� ���, ���༱(���༱+1)�� ǥ��, ���� ������ ���༱ ǥ��
		else
			pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge-1);	//���༱ �� ���, ���༱(���༱-1)�� ǥ��, ���� ������ ���༱ ǥ��

		CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);

		DBStd.m_pDrawPlane->DrawPlaneGen(&Dom);
		DBStd.m_pDrawPlane->MarkPlaneBridgeSttEnd(&Dom);
	}

	CEnvRegist env;		
	BOOL bRotate = env.LoadEnvInt(CEnvRegist::INPUTDOM_ROTATE);
	if(bRotate)
	{
		// ���� ���� �������� ȸ��
		DBStd.RotateDomByBridgeSttEndXy(&Dom);
	}

	*pDom << Dom;

	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

/*
#define DEFAULT_BASESPANCONSTRUCT_MAKEJIJUM		0x00000001
#define DEFAULT_BASESPANCONSTRUCT_JIJUMANGLE	0x00000002
#define DEFAULT_BASESPANCONSTRUCT_JIJUMLENGTH	0x00000004
#define DEFAULT_BASESPANCONSTRUCT_GAGAK			0x00000008
#define DEFAULT_BASESPANCONSTRUCT_EXPWIDTH		0x00000010
#define DEFAULT_BASESPANCONSTRUCT_DISTSHOE		0x00000020
*/
void CInputBaseSpanConstruct::SetDataDefault()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	long nTab = m_ctlTab.GetCurSel();
	for(int nBri = 0; nBri < pStd->GetBridgeSize(); nBri++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(nBri);

		if(!IsDefaulted(nBri, DEFAULT_BASESPANCONSTRUCT_MAKEJIJUM) || IsClickDefaultButton(nBri))				
			pBri->SetDataInitJijum();// ���� ����

		// ���� ���Ŀ� ���� ������ ���� ���� �� ���� ����
		if(!IsDefaulted(nBri, DEFAULT_BASESPANCONSTRUCT_JIJUMANGLE) || (nTab == m_nTabAngle && IsClickDefaultButton(nBri)))	
			pBri->SetDataDefaultJijumAngle();

		// ���� �Ÿ� ����
		if(!IsDefaulted(nBri, DEFAULT_BASESPANCONSTRUCT_JIJUMLENGTH) || (nTab == m_nTabSpan && IsClickDefaultButton(nBri)))
			pBri->SetDataDefaultJijumLength();

		if(!IsDefaulted(nBri, DEFAULT_BASESPANCONSTRUCT_GAGAK) || (nTab == m_nTabGagak && IsClickDefaultButton(nBri)))	
			pBri->SetDataDefaultGagak();

		if(!IsDefaulted(nBri, DEFAULT_BASESPANCONSTRUCT_EXPWIDTH) || (nTab == m_nTabExtLine && IsClickDefaultButton(nBri)))
			pBri->SetDataDefaultExpWidth();

		if(!IsDefaulted(nBri, DEFAULT_BASESPANCONSTRUCT_EXPJOINT) || (nTab == m_nTabExpJoint && IsClickDefaultButton(nBri)))
			pBri->MakeExpJoint();

		if(!IsDefaulted(nBri, DEFAULT_BASESPANCONSTRUCT_DISTSHOE) || IsClickDefaultButton(nBri))	
			pBri->SetDataDefaultDistShoe();

		pBri->SyncBridge(0, FALSE, FALSE, FALSE);
		pBri->m_bNeedSettingVertSection	= TRUE;
	}

	SetGridData();
}


void CInputBaseSpanConstruct::SetDataSave()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	m_Grid.UpdateData(TRUE);
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(pBri == NULL) return;

	long nTab = m_ctlTab.GetCurSel();
	if(nTab == m_nTabExpJoint)
	{
		BOOL bExistExpJoint = (pBri->IsRcSlab() || pBri->IsPiRahmen()) ? TRUE : FALSE;
		CDesignExpansionJoint	*pDesignExp	= pBri->m_pDesignExpanJoint;
		pDesignExp->SetDataSave(m_Grid);
		if(bExistExpJoint)
		{
			CString szExpRefName = _T("");
			for(long nExp=iSTT; nExp<=iEND; nExp++)
			{
				szExpRefName = m_Grid.GetTextMatrix(nExp+2, 7);
				pBri->SetExpJoint(szExpRefName, nExp == 0, FALSE);
			}
		}
	}
	else
	{
		if(!pBri->m_bIsExp)
		{
			pBri->m_dExpSttW[0]	= 0;
			pBri->m_dExpSttH[0]	= 0;
			pBri->m_dExpSttR[0]	= 0;
			pBri->m_dExpSttW[1]	= 0;
			pBri->m_dExpSttH[1]	= 0;
			pBri->m_dExpSttR[1]	= 0;

		}
		if(pBri)
		{	
			if(nTab == m_nTabSpan)
			{
				pBri->SyncJijumStation();
			}

			pBri->SetDataDefaultDistJijumDirPlacing();
			pBri->m_bNeedSettingVertSection	= TRUE;

			// �����༱ �и� ����
			pStd->CopyToCoupleBridge_SpanConst(m_pIn->m_nCurrentBridge);

			long nBriCouple	= pStd->GetIdxMatchBridge(m_pIn->m_nCurrentBridge);
			if(nBriCouple > -1)
			{
				CRcBridgeRebar *pBriCouple	= pStd->GetBridgeRebar(nBriCouple);
				pBriCouple->SetDataDefaultDistJijumDirPlacing();
				pBriCouple->m_bNeedSettingVertSection	= TRUE;
			}
		}
	}

	m_pIn->m_bViewAllDir = m_bViewAllDir;
	m_pIn->m_pDoc->SetModifiedFlag();
	AfxGetMainWnd()->SendMessage(WM_USER+12);
}


BOOL CInputBaseSpanConstruct::DestroyWindow()
{ 

	return CInputDialogBase::DestroyWindow();
}

void CInputBaseSpanConstruct::OnButtonSameExpJoint() 
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);

	long nExp = max(m_Grid.GetFocusCell().row - 2, 0);
	CExpansionJointData *pExpData = pBri->m_pDesignExpanJoint->GetExpansionJointData(nExp);

	long nVPos1 = nExp == 0 ? 1 : 0;
	long nVPos2 = nExp == 0 ? 0 : 1;

	for(long ix = 0; ix < pBri->m_pDesignExpanJoint->GetSize(); ix++)
	{
		if(ix == nExp) continue;

		CExpansionJointData *pExpJoint = pBri->m_pDesignExpanJoint->GetExpansionJointData(ix);

		pExpJoint->m_strExpanJoint = pExpData->m_strExpanJoint;
		pExpJoint->m_dMaxAllowExp = pExpData->m_dMaxAllowExp;
		pExpJoint->m_dMaxLayingGap = pExpData->m_dMaxLayingGap;
		pExpJoint->m_dMinLayingGap = pExpData->m_dMinLayingGap;

		*pExpJoint->m_pBlockW[nVPos2] = *pExpData->m_pBlockW[nVPos1];
		*pExpJoint->m_pBlockH[nVPos2] = *pExpData->m_pBlockH[nVPos1];
	}

	SetGridData();
	SetDataInit();
	DrawInputDomyunView(TRUE);
}

void CInputBaseSpanConstruct::OnButtonSameAngle() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	CCellID cID = m_Grid.GetFocusCell();
	int nCol = cID.col;
	int nCols = m_Grid.GetCols();
	if(nCol < 1 || nCol >= nCols) return;

	double dAng = m_Grid.GetTextMatrixDouble(1, nCol);
	int i=0; for(i=1; i<nCols; i++)
	{
		m_Grid.SetTextMatrix(1, i, _T("%.3f"), dAng);

		// ������ Elevation ����
		if(pBri->m_bAutoWingEl && (i==1 || i==nCols-1))
		{
			CWingWall* pWing = pBri->GetWingWall(TRUE, TRUE);
			if(pWing)
				pWing->m_dEL = Round(pBri->GetElWingWallEnd(TRUE, TRUE), 0);
			pWing = pBri->GetWingWall(TRUE, FALSE);
			if(pWing)
				pWing->m_dEL = Round(pBri->GetElWingWallEnd(TRUE, FALSE), 0);
			pWing = pBri->GetWingWall(FALSE, TRUE);
			if(pWing)
				pWing->m_dEL = Round(pBri->GetElWingWallEnd(FALSE, TRUE), 0);
			pWing = pBri->GetWingWall(FALSE, FALSE);
			if(pWing)
				pWing->m_dEL = Round(pBri->GetElWingWallEnd(FALSE, FALSE), 0);
		}

		// ���� ���� ����
		int nCountFooting = pBri->GetCountInWall() + 2;
		int j=0; for(j=0; j<nCountFooting; j++)
			pBri->SetFootingEndShape(j);
	}

	SetDataSave();
	SetGridData();
	SetDataInit();

	DrawInputDomyunView();
}

void CInputBaseSpanConstruct::SetGridData()
{
	long nTab = m_ctlTab.GetCurSel();

	if(nTab == m_nTabAngle)	SetGridDataJijumAngle();
	if(nTab == m_nTabSpan)		SetGridDataSpan();
	if(nTab == m_nTabGagak)	SetGridDataGagak();
	if(nTab == m_nTabExtLine)	SetGridDataExtLine();
	if(nTab == m_nTabExpJoint)	SetGridDataExpJoint();
}

void CInputBaseSpanConstruct::SetGridDataSpan()
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2,2,1,1);

	CARcBridgeDataStd *pStd		= m_pIn->m_pARcBridgeDataStd;
	CLineInfoApp	  *pLineApp = pStd->GetLineApp();
	CRcBridgeApp	  *pBri		= pStd->GetBridge(m_pIn->m_nCurrentBridge);

	UINT nFormat = DT_CENTER|DT_VCENTER;
	m_Grid.AddCell(0, 0,_T("����"));
	m_Grid.AddCell(1, 0,_T("�Ÿ�(mm)"));

	CString str = _T("");

	long nCol = 1;
	long i=0; for(i=0; i<pBri->m_nQtyJigan; i++)
	{
		CJijum *pJL = pBri->GetJijum(i);
		CJijum *pJR = pBri->GetJijum(i+1);
		if(pBri->m_bExpJoint[i] || (!pBri->IsOutWall(TRUE) && i==0))
		{
			str.Format("U%d1", i+1);
			m_Grid.AddCell(0,nCol, str);
			m_Grid.AddCell(1,nCol++, &pJL->m_dLengthUgan[1]); m_Grid.SetCellDec(CELL_DEC_0);
			str.Format("S%d1", i+1);
			m_Grid.AddCell(0,nCol, str);
			m_Grid.AddCell(1,nCol++, &pJL->m_dLengthSlabToShoe[1]); m_Grid.SetCellDec(CELL_DEC_0);
		}
		UINT nLine = (i==pBri->m_nQtyJigan-1) ? CELL_READONLY : CELL_LINE_NO;
		str.Format("L%d", i+1);
		m_Grid.AddCell(0,nCol, str);
		m_Grid.AddCell(1,nCol++, &pBri->m_dLengthJigan[i],nFormat,nLine); m_Grid.SetCellDec(CELL_DEC_0);
		if(pBri->m_bExpJoint[i+1] || (!pBri->IsOutWall(FALSE) && i == pBri->m_nQtyJigan-1))
		{
			str.Format("S%d2", i+1);
			m_Grid.AddCell(0,nCol, str);
			m_Grid.AddCell(1,nCol++, &pJR->m_dLengthSlabToShoe[0]); m_Grid.SetCellDec(CELL_DEC_0);
			str.Format("U%d2", i+1);
			m_Grid.AddCell(0,nCol, str);
			m_Grid.AddCell(1,nCol++, &pJR->m_dLengthUgan[0]); m_Grid.SetCellDec(CELL_DEC_0);
		}
	}
	long nSizeRow = 0;
	long nSizeCol = 0;
	m_Grid.GetCellCount(nSizeRow, nSizeCol);
	m_Grid.SetRows(nSizeRow);
	m_Grid.SetCols(nSizeCol);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(80);
	m_Grid.SetColumnWidth(0,100);
}


void CInputBaseSpanConstruct::SetGridDataJijumAngle()
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(3,2,1,1);

	CARcBridgeDataStd *pStd		= m_pIn->m_pARcBridgeDataStd;
	CLineInfoApp	  *pLineApp = pStd->GetLineApp();
	CRcBridgeApp	  *pBri		= pStd->GetBridge(m_pIn->m_nCurrentBridge);

	BOOL bExistExpJoint = (pBri->IsRcSlab() || pBri->IsPiRahmen()) ? TRUE : FALSE;

	CStringArray sArr;
	UINT nFormat = DT_CENTER|DT_VCENTER;
	UINT nLine	= pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH ? CELL_READONLY : CELL_LINE_NO;
	m_Grid.AddCell(0, 0, _T("����"));
	m_Grid.AddCell(1, 0, _T("����(��)"));
	if(bExistExpJoint)
	{
		m_Grid.AddCell(2, 0, _T("���� ����"));
		sArr.Add(_T("����"));
		sArr.Add(_T("����"));
	}

	CString str = _T(""), strCol = _T("");

	long nCol = 1;
	long i=0; for(i=0; i<pBri->m_nQtyJigan+1; i++)
	{
		CJijum *pJ = pBri->GetJijum(i);
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
			pJ->m_vAngle	= CDPoint(0, 1);
		pJ->m_strJijumShoe[0] = _T("");
		pJ->m_strJijumShoe[1] = _T("");
		char cAP  = (i==0 || i==pBri->m_nQtyJigan) ? 'A' : 'P';
		long nIdx = i;
		if(i==0) nIdx = 1;
		if(i==pBri->m_nQtyJigan) nIdx = 2;
		BOOL bExpJoint = pBri->m_bExpJoint[i];
		BOOL bShoeStt  = (bExpJoint && i!=0) ? TRUE : FALSE;
		BOOL bShoeEnd  = (bExpJoint && i!=pBri->m_nQtyJigan) ? TRUE : FALSE;
		if(bShoeStt)
		{
			str.Format("%c%d(����)", cAP, nIdx);
			if(cAP=='A')
			{
				str.Format("%c%d(��ħ��)", cAP, nIdx);
				if(pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB)
					strCol.Format("%c%d", cAP, nIdx);
				else
					strCol = "����";
			}
			else
				strCol = str;
			m_Grid.AddCell(0, nCol, str);
			pJ->m_strJijumShoe[0] = strCol;
			str.Format("%.3f", pBri->GetAngleJijumShoe(i,TRUE).GetAngleDegree());
			m_Grid.AddCell(1, nCol, str, nFormat, CELL_READONLY);
			if(bExistExpJoint && (i == 0 || i == pBri->m_nQtyJigan))
			{
				m_Grid.AddCell(2, nCol, _T("-"), nFormat, CELL_READONLY);
			}
			nCol++;
		}
		str.Format("%c%d", cAP, nIdx);
		m_Grid.AddCell(0, nCol, str);
		// 2008.03.12
		// ���� ���� �ڸ��� 7�ڸ� �� �޶�� ����� �־�����,
		// ���� ������ 3�ڸ��� ������ �ǹǷ� 3�ڸ��� �Ƚ���.
		// 7�ڸ� �����ϴ� ��κ��� ����ڿ��� �ذ� �� ������ �Ǵ�. (�ӽ�ȫ, ����ȣ)
		m_Grid.AddCell(1, nCol, &pJ->m_vAngle, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_3);
		if(bExistExpJoint)
		{
			if((i == 0 || i == pBri->m_nQtyJigan))
			{
				m_Grid.AddCell(2, nCol, &pBri->m_nExpJointType[i == 0 ? iSTT : iEND], nFormat, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
			}
			else
			{
				m_Grid.AddCell(2, nCol, _T("-"), nFormat, CELL_READONLY);
			}
		}
		if(!pBri->m_bExpJoint[i])
		{
			pJ->m_strJijumShoe[0] = str;
		}
		
		nCol++;
		if(bShoeEnd)
		{
			str.Format("%c%d(�Ŀ�)", cAP, nIdx);
			if(cAP=='A')
			{
				str.Format("%c%d(��ħ��)", cAP, nIdx);
				if(pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB)
					strCol.Format("%c%d", cAP, nIdx);
				else
					strCol = "����";
			}
			else
				strCol = str;
			m_Grid.AddCell(0, nCol, str);
			pJ->m_strJijumShoe[1] = strCol;
			str.Format("%.3f", pBri->GetAngleJijumShoe(i,FALSE).GetAngleDegree());
			m_Grid.AddCell(1, nCol, str, nFormat, CELL_READONLY);
			if(bExistExpJoint && (i == 0 || i == pBri->m_nQtyJigan))
			{
				m_Grid.AddCell(2, nCol, _T("-"), nFormat, CELL_READONLY);
			}
			nCol++;
		}
	}
	long nSizeRow = 0;
	long nSizeCol = 0;
	m_Grid.GetCellCount(nSizeRow, nSizeCol);
	m_Grid.SetRows(nSizeRow);
	m_Grid.SetCols(nSizeCol);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(80);
	m_Grid.SetColumnWidth(0,100);

	m_Grid.SetRedraw(TRUE, TRUE);
}

void CInputBaseSpanConstruct::SetGridDataGagak()
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(3,2,2,1);

	CARcBridgeDataStd *pStd		= m_pIn->m_pARcBridgeDataStd;
	CLineInfoApp	  *pLineApp = pStd->GetLineApp();
	CRcBridgeApp	  *pBri		= pStd->GetBridge(m_pIn->m_nCurrentBridge);

	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;

	m_Grid.AddCell(0, 0,_T("����"));
	m_Grid.AddCell(2, 0,_T("�Ÿ�(mm)"));

	long nCol = 1;
	m_Grid.AddCell(0,nCol,"������");
	m_Grid.AddCell(1,nCol+0,"�ܺ�����");
	m_Grid.AddCell(1,nCol+1,"����R");
	m_Grid.AddCell(1,nCol+2,"sDL");
	m_Grid.AddCell(1,nCol+3,"sHL");
	m_Grid.AddCell(1,nCol+4,"sRL");
	m_Grid.AddCell(1,nCol+5,"�ܺ�����");
	m_Grid.AddCell(1,nCol+6,"����R");
	m_Grid.AddCell(1,nCol+7, "sDR");
	m_Grid.AddCell(1,nCol+8,"sHR");
	m_Grid.AddCell(1,nCol+9,"sRR");
	m_Grid.AddCell(2,nCol++, &pBri->m_bExpCutStt[0], nFormat, nLine, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(2,nCol++, &pBri->m_bExpSttRoundIsTan[0], nFormat, nLine, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(2,nCol++, &pBri->m_dExpSttW[0]); m_Grid.SetCellDec(CELL_DEC_0); m_Grid.SetLastCellMinLimit(0);
	m_Grid.AddCell(2,nCol++, &pBri->m_dExpSttH[0]); m_Grid.SetCellDec(CELL_DEC_0); m_Grid.SetLastCellMinLimit(0);
	m_Grid.AddCell(2,nCol++, &pBri->m_dExpSttR[0]); m_Grid.SetCellDec(CELL_DEC_0); m_Grid.SetLastCellMinLimit(0);
	m_Grid.AddCell(2,nCol++, &pBri->m_bExpCutStt[1], nFormat, nLine, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(2,nCol++, &pBri->m_bExpSttRoundIsTan[1], nFormat, nLine, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(2,nCol++, &pBri->m_dExpSttW[1]); m_Grid.SetCellDec(CELL_DEC_0); m_Grid.SetLastCellMinLimit(0);
	m_Grid.AddCell(2,nCol++, &pBri->m_dExpSttH[1]); m_Grid.SetCellDec(CELL_DEC_0); m_Grid.SetLastCellMinLimit(0);
	m_Grid.AddCell(2,nCol++, &pBri->m_dExpSttR[1]); m_Grid.SetCellDec(CELL_DEC_0); m_Grid.SetLastCellMinLimit(0);
	//
	m_Grid.AddCell(0,nCol,"������");
	m_Grid.AddCell(1,nCol+0,"�ܺ�����");
	m_Grid.AddCell(1,nCol+1,"����R");
	m_Grid.AddCell(1,nCol+2,"eDL");
	m_Grid.AddCell(1,nCol+3,"eHL");
	m_Grid.AddCell(1,nCol+4,"eRL");
	m_Grid.AddCell(1,nCol+5,"�ܺ�����");
	m_Grid.AddCell(1,nCol+6,"����R");
	m_Grid.AddCell(1,nCol+7,"eDR");
	m_Grid.AddCell(1,nCol+8,"eHR");
	m_Grid.AddCell(1,nCol+9,"eRR");
	m_Grid.AddCell(2,nCol++, &pBri->m_bExpCutEnd[0], nFormat, nLine, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(2,nCol++, &pBri->m_bExpEndRoundIsTan[0], nFormat, nLine, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(2,nCol++, &pBri->m_dExpEndW[0]); m_Grid.SetCellDec(CELL_DEC_0); m_Grid.SetLastCellMinLimit(0);
	m_Grid.AddCell(2,nCol++, &pBri->m_dExpEndH[0]); m_Grid.SetCellDec(CELL_DEC_0); m_Grid.SetLastCellMinLimit(0);
	m_Grid.AddCell(2,nCol++, &pBri->m_dExpEndR[0]); m_Grid.SetCellDec(CELL_DEC_0); m_Grid.SetLastCellMinLimit(0);
	m_Grid.AddCell(2,nCol++, &pBri->m_bExpCutEnd[1], nFormat, nLine, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(2,nCol++, &pBri->m_bExpEndRoundIsTan[1], nFormat, nLine, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(2,nCol++, &pBri->m_dExpEndW[1]); m_Grid.SetCellDec(CELL_DEC_0); m_Grid.SetLastCellMinLimit(0);
	m_Grid.AddCell(2,nCol++, &pBri->m_dExpEndH[1]); m_Grid.SetCellDec(CELL_DEC_0); m_Grid.SetLastCellMinLimit(0);
	m_Grid.AddCell(2,nCol++, &pBri->m_dExpEndR[1]); m_Grid.SetCellDec(CELL_DEC_0); m_Grid.SetLastCellMinLimit(0);

	// ���� �Է¹޴� ��� ��ü�� ������ ��ü�� ���ؼ��� �Է��� �޾ƾ� �ȴ�.
	if(pBri->IsOutWall(TRUE))
	{
		m_Grid.AddCell(0, nCol, "��ü�β�");
		m_Grid.AddCell(1, nCol, "WS");
		m_Grid.AddCell(2, nCol++, &pBri->m_dWS);
	}
	if(pBri->IsOutWall(FALSE))
	{
		m_Grid.AddCell(0, nCol, "��ü�β�");
		m_Grid.AddCell(1, nCol, "WE");
		m_Grid.AddCell(2, nCol++, &pBri->m_dWE);
	}

	long nSizeRow = 0;
	long nSizeCol = 0;
	m_Grid.GetCellCount(nSizeRow, nSizeCol);
	m_Grid.SetRows(nSizeRow);
	m_Grid.SetCols(nSizeCol);
	m_Grid.MergeGrid(0,1,0,0);
	m_Grid.MergeGrid(0,0,1,10);
	m_Grid.MergeGrid(0,0,11,20);
	if(pBri->IsOutWall(TRUE) && pBri->IsOutWall(FALSE))
	{
		m_Grid.MergeGrid(0,0,21,22);
	}
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(60);
	m_Grid.SetColumnWidth(0,100);

	m_Grid.SetRedraw(TRUE, TRUE);
}

void CInputBaseSpanConstruct::SetGridDataExtLine()
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(3,2,2,1);

	CARcBridgeDataStd *pStd		= m_pIn->m_pARcBridgeDataStd;
	CLineInfoApp	  *pLineApp = pStd->GetLineApp();
	CRcBridgeApp	  *pBri		= pStd->GetBridge(m_pIn->m_nCurrentBridge);

	m_Grid.AddCell(0, 0,_T("����"));
	m_Grid.AddCell(2, 0,_T("�Ÿ�(mm)"));

	long nCol = 1;
	m_Grid.AddCell(0,nCol,"������");
	m_Grid.AddCell(1,nCol,"sL");
	m_Grid.AddCell(1,nCol+1,"sR");
	m_Grid.AddCell(2,nCol++, &pBri->m_dDisExtLineSttLeft); m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(2,nCol++, &pBri->m_dDisExtLineSttRight); m_Grid.SetCellDec(CELL_DEC_0);
	//
	m_Grid.AddCell(0,nCol,"������");
	m_Grid.AddCell(1,nCol,"eL");
	m_Grid.AddCell(1,nCol+1,"eR");
	m_Grid.AddCell(2,nCol++, &pBri->m_dDisExtLineEndLeft); m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(2,nCol++, &pBri->m_dDisExtLineEndRight); m_Grid.SetCellDec(CELL_DEC_0);

	long nSizeRow = 0;
	long nSizeCol = 0;
	m_Grid.GetCellCount(nSizeRow, nSizeCol);
	m_Grid.SetRows(nSizeRow);
	m_Grid.SetCols(nSizeCol);
	m_Grid.MergeGrid(0,1,0,0);
	m_Grid.MergeGrid(0,0,1,2);
	m_Grid.MergeGrid(0,0,3,4);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(80);
	m_Grid.SetColumnWidth(0,100);

	m_Grid.SetRedraw(TRUE, TRUE);
}

void CInputBaseSpanConstruct::SetGridDataExpJoint()
{
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp	  *pBri		= pStd->GetBridge(m_pIn->m_nCurrentBridge);

	m_Grid.SetGridDefault(3,2,2,1);

	CDesignExpansionJoint	*pDesignExp	= pBri->m_pDesignExpanJoint;
	pDesignExp->SetDataInit(m_Grid);

	m_Grid.SetRedraw(TRUE, TRUE);
}


void CInputBaseSpanConstruct::InitTabCtrl()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	long nSelectOld = m_ctlTab.GetCurSel();

	m_ctlTab.RemoveAllTabs();
	m_ctlTab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_ctlTab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);


	m_nTabAngle = m_nTabSpan = m_nTabGagak = m_nTabExtLine = m_nTabExpJoint = -1;

	long nTab = 0;
	
	m_nTabAngle = nTab;
	m_ctlTab.InsertItem(nTab++,"���� ����");

	m_nTabSpan = nTab;
	m_ctlTab.InsertItem(nTab++, pBri->GetStringJigan() + " ����");

	if(pBri->m_bIsExp)
	{
		m_nTabGagak = nTab;
		m_ctlTab.InsertItem(nTab++,"���� ó��");
	}

// Ȯ������ �Է�â ������(2007.09.25) ������� ����. �ϴ� �����Ұ��� ��.
// 	pBri->IsExtLine())
//	{
//		m_nTabExtLine = nTab;
// 		m_ctlTab.InsertItem(nTab++,"Ȯ�� ����");
//	}

	if(pBri->IsOutWall(TRUE) == FALSE || pBri->IsOutWall(FALSE) == FALSE)
	{
		m_nTabExpJoint = nTab;
		m_ctlTab.InsertItem(nTab++,"�������� ����");
	}


	if(nSelectOld>=0 && nSelectOld<nTab)
	{
		m_ctlTab.SetCurSel(nSelectOld);
	}
}

void CInputBaseSpanConstruct::SetDataInit()
{
	m_Grid.UpdateData(FALSE);
	m_Grid.SetRedraw(TRUE, TRUE);

	// �Է�ȭ�� �׸���� �����ε�.....
	if(m_ctlTab.GetCurSel() == m_nTabSpan)
	{
		CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
		CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
		if(!pBri) return;

		CString str = _T("");
//		double dLen = 0.0;
		pBri->m_arrStrJijumLen.RemoveAll();
		pBri->m_arrJijumLen.RemoveAll();
		long i=0; for(i=1; i<m_Grid.GetCols(); i++)
		{
			str = m_Grid.GetTextMatrix(0, i);
			double dLen = m_Grid.GetTextMatrixDouble(1, i);
			pBri->m_arrStrJijumLen.Add(str);
			pBri->m_arrJijumLen.Add(dLen);
		}

		// �����༱ �и� ����
		m_pIn->m_pARcBridgeDataStd->CopyToCoupleBridge_SpanConst(m_pIn->m_nCurrentBridge);
	}
	m_bViewAllDir  = m_pIn->m_bViewAllDir;

	UpdateData(FALSE);
	AfxGetMainWnd()->SendMessage(WM_USER+12);
}

void CInputBaseSpanConstruct::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetDataSave();
	
	return;
}

void CInputBaseSpanConstruct::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nTab = m_ctlTab.GetCurSel();

	m_btSameAngle.EnableWindow(nTab==m_nTabAngle);
	m_btnAutoRadius.EnableWindow(m_bExp && nTab==m_nTabGagak);
//	m_btnAutoRadius.ShowWindow(m_bExp);

	SetControl();
	SetFocus();
	SetGridData();
	SetDataInit();
	DrawInputDomyunView(TRUE);

	return;
}

void CInputBaseSpanConstruct::OnButtonCheckExpJoint() 
{
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp	  *pBri		= pStd->GetBridge(m_pIn->m_nCurrentBridge);

	CDesignExpansionJoint	*pDesignExp	= pBri->m_pDesignExpanJoint;

	SetDataSave();

	CExpansionJointDlg ExpJointDlg(this);
	ExpJointDlg.SetExpansionJoint(pDesignExp);
	if(ExpJointDlg.DoModal()==IDOK)
	{
		SetDataInit();
	}
}

void CInputBaseSpanConstruct::OnButtonRefExpJoint() 
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);


	long nExp = max(m_Grid.GetFocusCell().row - 2, 0);
	CExpansionJointData	*pExpData	= pBri->m_pDesignExpanJoint->GetExpansionJointData(nExp);


	CDialogExpansionJointRef Dlg(pStd->m_pExpJointRefArr, pExpData);

	if(Dlg.DoModal() == IDOK)
	{
	}
	SetGridData();
	SetDataInit();
	DrawInputDomyunView(TRUE);
}

void CInputBaseSpanConstruct::OnButtonAutoRadiusGagak() 
{
	CARcBridgeDataStd *pStd		= m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp	  *pBri		= pStd->GetBridge(m_pIn->m_nCurrentBridge);
	CLineInfo *pLineL = pBri->GetLineLeftRight(TRUE);
	CLineInfo *pLineR = pBri->GetLineLeftRight(FALSE);

	CDPoint SttA[4],EndA[4];
	pBri->GetXySss(BRIDGE_OFF_STT_SLAB,SttA);
	pBri->GetXySss(BRIDGE_OFF_END_SLAB,EndA);

	// ������ ��������
	double staStt = pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB,-1,FALSE) + pBri->m_dExpSttW[0];
	double staEnd = pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB,-1,FALSE) - pBri->m_dExpEndW[0];
	CDPoint xySegStt1 = pLineL->GetXyLineDis(staStt,pBri->m_dWidthSlabLeft);
	CDPoint xySegStt2 = SttA[0];
	CDPoint xySegEnd1 = pLineL->GetXyLineDis(staEnd,pBri->m_dWidthSlabLeft);
	CDPoint xySegEnd2 = EndA[0];
	CDPoint xyCenStt(0,0),xySegStt(0,0),xyCenEnd(0,0),xySegEnd(0,0);

	if(pBri->m_dExpSttW[0] > 0 && pBri->m_dExpSttH[0] > 0)	//���� ����
	{
		pBri->m_dExpSttR[0] = pLineL->GetRadiusForGaGak(pBri->m_dWidthSlabLeft,xySegStt1,xySegStt2,TRUE ,TRUE, staStt,xyCenStt,xySegStt);
	}
	if(pBri->m_dExpEndW[0] > 0 && pBri->m_dExpEndH[0] > 0)	//���� ����
	{
		pBri->m_dExpEndR[0] = pLineL->GetRadiusForGaGak(pBri->m_dWidthSlabLeft,xySegEnd1,xySegEnd2,FALSE,TRUE, staEnd,xyCenEnd,xySegEnd);
	}

	staStt = pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB,1,FALSE) + pBri->m_dExpSttW[1];
	staEnd = pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB,1,FALSE) - pBri->m_dExpEndW[1];
	xySegStt1 = pLineR->GetXyLineDis(staStt,pBri->m_dWidthSlabRight);
	xySegStt2 = SttA[3];
	xySegEnd1 = pLineR->GetXyLineDis(staEnd,pBri->m_dWidthSlabRight);
	xySegEnd2 = EndA[3];
	if(pBri->m_dExpSttW[1] > 0 && pBri->m_dExpSttH[1] > 0)	//���� ����
	{
		pBri->m_dExpSttR[1] = pLineR->GetRadiusForGaGak(pBri->m_dWidthSlabRight,xySegStt1,xySegStt2,TRUE ,FALSE, staStt,xyCenStt,xySegStt);
	}
	if(pBri->m_dExpEndW[1] > 0 && pBri->m_dExpEndH[1] > 0)	//���� ����
	{
		pBri->m_dExpEndR[1] = pLineL->GetRadiusForGaGak(pBri->m_dWidthSlabRight,xySegEnd1,xySegEnd2,FALSE,FALSE, staEnd,xyCenEnd,xySegEnd);
	}
	
	SetGridData();
	SetDataInit();
	DrawInputDomyunView(TRUE);

}
void CInputBaseSpanConstruct::OnCheckViewCounterDir() 
{
	UpdateData(TRUE);
	m_pIn->m_bViewAllDir = m_bViewAllDir;
	DrawInputDomyunView();

}

void CInputBaseSpanConstruct::OnCursorChange(long nRow, long nCol)
{
	CInputDialogBase::OnCursorChange(nRow, nCol);

	CString str = m_Grid.GetTextMatrix(0, nCol);
	int nTab = m_ctlTab.GetCurSel();

	if(nTab == m_nTabGagak) 
		str = m_Grid.GetTextMatrix(1, nCol);

	m_pView->GetDomyun()->STMakeCursor(str);
}

BOOL CInputBaseSpanConstruct::IsFocusItem(CString &szTitle)
{
	long nCurRow = m_ctlTab.GetCurSel() == m_nTabGagak ? 1 : 0;

	if(GetDlgItem(IDC_GRID)) 
	{
		CGridCtrlMng *pGrid = (CGridCtrlMng *)GetDlgItem(IDC_GRID);
		if(pGrid->GetSafeHwnd())
		{
			for(long nCol=0; nCol<pGrid->GetColumnCount(); nCol++)
			{
				if(pGrid->GetTextMatrix(nCurRow,nCol)==szTitle)
				{
					return TRUE;					
				}
			}
		}
	}

	return FALSE;
}

void CInputBaseSpanConstruct::SetFocusItem(CString szTitle)
{
	long nCurRow = m_ctlTab.GetCurSel() == m_nTabGagak ? 1 : 0;
	if(GetDlgItem(IDC_GRID)) 
	{
		CGridCtrlMng *pGrid = (CGridCtrlMng *)GetDlgItem(IDC_GRID);
		if(pGrid->GetSafeHwnd())
		{
			for(long nCol=0; nCol<pGrid->GetColumnCount(); nCol++)
			{
				if(pGrid->GetTextMatrix(nCurRow,nCol)==szTitle)
				{
					GetFocus();
					//pGrid->ResetSelection();
					pGrid->SetSelectedRange(-1,-1,-1,-1);
					pGrid->SetFocusCell(nCurRow+1,nCol);					
					pGrid->SendMessage(WM_KEYDOWN,VK_F2);
					break;
				}
			}
		}
	}
}

void CInputBaseSpanConstruct::OnCheckExp() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CRcBridgeRebar	*pBri		= m_pIn->GetRcBridgeRebar();
	pBri->m_bIsExp	= m_bExp;
	if(!pBri->m_bIsExp)
	{
		long i = 0; for(i = 0; i < 2; i++)
		{
			pBri->m_dExpEndH[i]	= 0;
			pBri->m_dExpEndR[i]	= 0;
			pBri->m_dExpEndW[i]	= 0;
			pBri->m_dExpSttH[i]	= 0;
			pBri->m_dExpSttR[i]	= 0;
			pBri->m_dExpSttW[i]	= 0;
		}
	}

	m_btnAutoRadius.EnableWindow(m_bExp);

	// ���� ���ý� �޽��� ���
	m_pIn->MessageExpAndGagak();

	InitTabCtrl();
	SetGridData();
	SetDataInit();

	DrawInputDomyunView(FALSE);
}

void CInputBaseSpanConstruct::SetControl()
{
	CRcBridgeRebar	*pBri		= m_pIn->GetRcBridgeRebar();
	m_bExp	= pBri->m_bIsExp;

	long nTab = m_ctlTab.GetCurSel();
	if(pBri->IsOutWall(TRUE) == FALSE || pBri->IsOutWall(FALSE) == FALSE)
	{
		GetDlgItem(IDC_BUTTON_EXP_JOINT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_REF_EXP_JOINT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_CALC_EXPANSION)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_EXP_JOINT)->EnableWindow(nTab == m_nTabExpJoint);
		GetDlgItem(IDC_BUTTON_REF_EXP_JOINT)->EnableWindow(nTab == m_nTabExpJoint);
		GetDlgItem(IDC_BUTTON_CALC_EXPANSION)->EnableWindow(nTab == m_nTabSpan);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_EXP_JOINT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_REF_EXP_JOINT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_CALC_EXPANSION)->ShowWindow(SW_HIDE);
	}

	GetDlgItem(IDC_BUTTON_SAME_ANGLE)->ShowWindow(nTab != m_nTabExpJoint);
	GetDlgItem(IDC_BUTTON_SAME_EXP_JOINT)->ShowWindow(nTab == m_nTabExpJoint);

	UpdateData(FALSE);
}

void CInputBaseSpanConstruct::KillFocusDlg()
{
	long i=0; for(i=0; i<m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); i++)
		m_pIn->m_pARcBridgeDataStd->GetBridge(i)->SyncBridge(0, FALSE, FALSE, FALSE);
}

BOOL CInputBaseSpanConstruct::IsOKMoveOtherDlg()
{
	if(!m_pIn->CheckStation()) return FALSE;
	if(!CheckAngle()) return FALSE;
	if(!CheckGagakLen()) return FALSE;

	return TRUE;
}

// ���� ������ 0~180�� �ƴϸ� ����
BOOL CInputBaseSpanConstruct::CheckAngle()
{
	BOOL bOK	= TRUE;
	for(long bri = 0; bri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		CRcBridgeApp *pBri	= m_pIn->m_pARcBridgeDataStd->GetBridge(bri);
		for(long j = 0; j <= pBri->m_nQtyJigan; j++)
		{
			double dAng	= pBri->GetAngleJijum(j).GetAngleDegree();
			if(dAng < 0 || dAng > 180) 
			{
				bOK	= FALSE;
				break;
			}
		}
		if(!bOK) break;
	}

	// ���� �޽���
	if(!bOK)
	{
		m_pIn->SetComment("������ ���� ������ 0�� ~ 180�� ���� ���� �Է��ؾ� �˴ϴ�.", TRUE);
	}

	return bOK;
}

// ������ ���̰� �������� ����� �ʵ��� üũ
BOOL CInputBaseSpanConstruct::CheckGagakLen()
{
	CRcBridgeRebar	*pBri		= m_pIn->GetRcBridgeRebar();
	BOOL bCheckOk = TRUE;

	if(!pBri->m_bIsExp) return TRUE;
	else
	{
		bCheckOk = TRUE;
		double dLenSttLeft	= pBri->m_dExpSttW[0];
		double dLenSttRight	= pBri->m_dExpSttW[1];
		double dLenEndLeft	= pBri->m_dExpEndW[0];
		double dLenEndRight	= pBri->m_dExpEndW[1];
		
		BOOL bStt, bLeft;
		double dLenMax = 0;
		if(dLenSttLeft > 0)
		{
			dLenMax = GetLenMaxGagak(TRUE, TRUE);
			if(Round(dLenSttLeft, 3) > Round(dLenMax, 3))
			{
				bCheckOk = FALSE;
				bStt = TRUE;
				bLeft = TRUE;
			}
		}
		if(dLenSttRight > 0 && bCheckOk)
		{
			dLenMax = GetLenMaxGagak(TRUE, FALSE);
			if(Round(dLenSttRight, 3) > Round(dLenMax, 3))
			{
				bCheckOk = FALSE;
				bStt = TRUE;
				bLeft = FALSE;
			}
		}
		if(dLenEndLeft > 0 && bCheckOk)
		{
			dLenMax = GetLenMaxGagak(FALSE, TRUE);
			if(Round(dLenEndLeft, 3) > Round(dLenMax, 3))
			{
				bCheckOk = FALSE;
				bStt = FALSE;
				bLeft = TRUE;
			}
		}
		if(dLenEndRight > 0 && bCheckOk)
		{
			dLenMax = GetLenMaxGagak(FALSE, FALSE);
			if(Round(dLenEndRight, 3) > Round(dLenMax, 3))
			{
				bCheckOk = FALSE;
				bStt = FALSE;
				bLeft = FALSE;
			}
		}

		// ���� �޽���
		if(!bCheckOk)
		{
			CString str = _T("");
			str.Format("%s%s ������ ���̴� %.3f���� �۾ƾ��մϴ�.", bStt? "����":"����", bLeft? "����":"����", dLenMax);
			m_pIn->SetComment(str, TRUE);
		}
	}

	return bCheckOk;
}

double CInputBaseSpanConstruct::GetLenMaxGagak(BOOL bStt, BOOL bLeft)
{
	CRcBridgeRebar	*pBri		= m_pIn->GetRcBridgeRebar();
	double dLenMax = 0;

	CLineInfo *pLineLR = pBri->GetLineLeftRight(bLeft);
	CLineInfo *pLineBase = pBri->GetLineBase();

	double dStaBase1 = (bStt)? pBri->GetStationAtSlabEnd(TRUE,0) : pBri->m_dStationBridgeStt;
	double dStaBase2 = (bStt)? (pBri->m_dStationBridgeStt + pBri->m_dLengthBridge) : pBri->GetStationAtSlabEnd(FALSE,0);

	double dWidthSlab = (bLeft)? pBri->m_dWidthSlabLeft : pBri->m_dWidthSlabRight;
	CDPoint vAngStt = pBri->GetAngleAzimuthJijum(0);
	CDPoint vAngEnd = pBri->GetAngleAzimuthJijum(pBri->m_nQtyJigan);

	CDPoint xy1, xy2;
	xy1 = pLineBase->GetXyLineDis(dStaBase1, 0);
	xy2 = pLineBase->GetXyLineDis(dStaBase2, 0);

	double dStaLR1, dStaLR2;
	pLineLR->GetStationMatchByXyAndAngle(xy1, vAngStt, dStaLR1, dWidthSlab);
	pLineLR->GetStationMatchByXyAndAngle(xy2, vAngEnd, dStaLR2, dWidthSlab);

	dLenMax = pLineLR->GetLengthByDis(dStaLR1, dStaLR2, dWidthSlab);
	//
	return dLenMax;
}

LRESULT CInputBaseSpanConstruct::OnExpansionRecommend(WPARAM wp, LPARAM lp)
{
	CRcBridgeRebar	*pBri		= m_pIn->GetRcBridgeRebar();

//	CExpansionJointData *pExpData	= NULL;
	CString szExpRefName = hggettext("");

	long nExpStt = (wp==-1) ? 0 : (long)wp;
	long nExpEnd = (wp==-1) ? 1 : (long)wp;

	for(long nExp=nExpStt; nExp<nExpEnd; ++nExp)
	{
		CExpansionJointData *pExpData	= pBri->m_pDesignExpanJoint->GetExpansionJointData(wp);
		if(pExpData == NULL) continue;

		szExpRefName = m_Grid.GetTextMatrix(nExp+2, 7);
		pBri->SetExpJoint(szExpRefName, nExp == 0, TRUE);
	}

	return 0;
}

void CInputBaseSpanConstruct::OnBnClickedButtonCalcExpansion()
{
	CRcBridgeApp	*pBri		= m_pIn->GetRcBridge();
	if (!pBri)
		return;

	CDialogSetExpJointSpace dlg(pBri);
	if(dlg.DoModal() == IDOK)
	{
		SetGridData();
		SetDataInit();

		DrawInputDomyunView(FALSE);
	}
}
