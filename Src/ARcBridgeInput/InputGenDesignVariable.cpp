// InputGenDesignVariable.cpp : implementation file
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
// CInputGenDesignVariable dialog


CInputGenDesignVariable::CInputGenDesignVariable(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputGenDesignVariable::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputGenDesignVariable)
	//}}AFX_DATA_INIT
	m_pBriForCover	= NULL;
	m_nTabCover = -1;
	m_nTabEnv = -1;
	m_nTabCondition = -1;
	
	m_nTabDesignRating = -1;
	m_nTabExposureRating = -1;
	m_nTabRebarFactor	= -1;
	m_nTabRebarSettle = -1;
	m_nTabRebarJoint = -1;
	m_nTabLsdSupportRebar = -1;
	m_nMergeRowSupportBarSlab = 0;
	m_nMergeRowSupportBarWall = 0;
	m_nMergeRowSupportBarFoot = 0;
	
	m_pDlgCheck = new CInputCheckGrid;
}

CInputGenDesignVariable::~CInputGenDesignVariable()
{
	if(m_pBriForCover)
	{
		delete m_pBriForCover;
		m_pBriForCover	= NULL;
	}
	
	delete m_pDlgCheck;
}

void CInputGenDesignVariable::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputGenDesignVariable)
	DDX_Control(pDX, IDC_BUTTON_BASEDATA_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_BUTTON_BASEDATA_LOAD, m_btnLoad);
	DDX_Control(pDX, IDC_BUTTON_APPLY_SAMEALL, m_btnSameAll);
	DDX_Control(pDX, IDC_BUTTON_APPLY_MIRROR2, m_btnMirror);
	DDX_Control(pDX, IDC_CHECK_QUICK_SEARCH, m_checkQuickSearch);
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_cbBridgeNo);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputGenDesignVariable, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputGenDesignVariable)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_BN_CLICKED(IDC_CHECK_QUICK_SEARCH, OnCheckQuickSearch)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_MIRROR2, OnButtonApplyMirror)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_SAMEALL, OnButtonApplySameAll)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
	ON_BN_CLICKED(IDC_BUTTON_BASEDATA_SAVE, OnButtonBasedataSave)
	ON_BN_CLICKED(IDC_BUTTON_BASEDATA_LOAD, OnButtonBasedataLoad)
	ON_BN_CLICKED(IDC_BUTTON_EXAMINE, OnButtonExamine)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputGenDesignVariable message handlers

void CInputGenDesignVariable::SetGridData()
{
	m_Grid.SetType(0);
	m_Grid.SetCellSize(-1, -1);

	int nTab = m_Tab.GetCurSel();

	if(nTab == m_nTabCondition)			SetGridDataCondition();
	if(nTab == m_nTabEnv)				SetGridDataEnv();
	if(nTab == m_nTabCover)				SetGridDataCover();
	if(m_bDesignLsd)
	{
		if(nTab == m_nTabExposureRating)	SetGridDataExposureRating();
		if(nTab == m_nTabDesignRating)		SetGridDataDesignRating();
		if(nTab == m_nTabRebarFactor)		SetGridDataRebarFactor();
		if(nTab == m_nTabRebarSettle)		SetGridDataRebarSettle();
		if(nTab == m_nTabRebarJoint)		SetGridDataRebarJoint();
		if(nTab == m_nTabLsdSupportRebar)	SeteGridDataLsdSupportRebar();
	}
}

void CInputGenDesignVariable::SetDataSave()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	m_Grid.UpdateData(TRUE);
	

	// ��ö�� �Ǻ��� 2, 3���� 1�ܿ��� 100�� �߰��� ���ؼ� �����Ų��.
	// ���� �Է��� �����ʱ� ������ ���⼭ �ڵ����� ��������
	// ��, �������� �Է��� ���� �����̹Ƿ� �ϴ� ����
	
	double dDefaultOffset	= 100;
	long dan = 0; for(dan = 1; dan < MAX_REBAR_DAN; dan++)
	{
		pBri->m_dCoverUpper_UpperSlab[dan]		= pBri->m_dCoverUpper_UpperSlab[0] + (dan * dDefaultOffset);	// ��ν����� ���
		pBri->m_dCoverLower_UpperSlab[dan]		= pBri->m_dCoverLower_UpperSlab[0] + (dan * dDefaultOffset);	// ��ν����� �ϸ�
		pBri->m_dCoverSide_UpperSlab[dan]		= pBri->m_dCoverSide_UpperSlab[0] + (dan * dDefaultOffset);		// ��ν����� ����;	// ��ν����� ����
		pBri->m_dCoverUpper_LowerSlab[dan]		= pBri->m_dCoverUpper_LowerSlab[0] + (dan * dDefaultOffset);	// �Ϻν����� ���
		pBri->m_dCoverLower_LowerSlab[dan]		= pBri->m_dCoverLower_LowerSlab[0] + (dan * dDefaultOffset);	// �Ϻν����� �ϸ�
		pBri->m_dCoverWallOut[dan]				= pBri->m_dCoverWallOut[0] + (dan * dDefaultOffset);			// ��ü�ܸ�
		pBri->m_dCoverWallIn[dan]				= pBri->m_dCoverWallIn[0] + (dan * dDefaultOffset);				// ��ü ����
		pBri->m_dCoverWallMid[dan]				= pBri->m_dCoverWallMid[0] + (dan * dDefaultOffset);			// �߰���ü
		pBri->m_dCoverColumn[dan]				= pBri->m_dCoverColumn[0] + (dan * dDefaultOffset);				// ���
//		pBri->m_dCoverWingOut[dan]				= pBri->m_dCoverWingOut[0] + (dan * dDefaultOffset);			// ������ �ܸ�
//		pBri->m_dCoverWingIn[dan]				= pBri->m_dCoverWingIn[0] + (dan * dDefaultOffset);				// ������ ����
		pBri->m_dCoverUpper_Footing[dan]		= pBri->m_dCoverUpper_Footing[0] + (dan * dDefaultOffset);		// ���� ���
		pBri->m_dCoverSide_Footing[dan]		= pBri->m_dCoverSide_Footing[0] + (dan * dDefaultOffset);		// ���� ����
		pBri->m_dCoverLower_Footing_Lean[dan]	= pBri->m_dCoverLower_Footing_Lean[0] + (dan * dDefaultOffset);		// ���� �ϸ�(�������ʽ�)
		pBri->m_dCoverLower_Footing_Pile[dan]	= pBri->m_dCoverLower_Footing_Pile[0] + (dan * dDefaultOffset);		// ���� �ϸ�(���ұ��ʽ�)
		pBri->m_dCoverRib[iUPPER][dan]		= pBri->m_dCoverRib[iUPPER][0] + (dan * dDefaultOffset);
		pBri->m_dCoverRib[iLOWER][dan]		= pBri->m_dCoverRib[iLOWER][0] + (dan * dDefaultOffset);
	}
	SyncCover(pBri);
	pBri->SyncBridge_DesignData();

	m_pIn->m_pDoc->SetModifiedFlag();
	CInputDialogBase::SetDataSave();

	int nTab = m_Tab.GetCurSel();
	// ������ ��ö�ٰ� ��±� ��ǥ �ʱ�ȭ
	if(nTab == m_nTabEnv)
	{
		pBri->m_bCaledRebarMain_Plane	= FALSE;
		pBri->m_bCaledRebarSupport_Plane	= FALSE;
	}

	// ����ǥ����� �ʱ�ȭ
	pBri->m_bFirstCalledGetTvVertSection_Std	= FALSE;
	pBri->m_bSecondCalledGetTvVertSection_Std	= FALSE;
}

void CInputGenDesignVariable::SetDataInit()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	m_Grid.UpdateData(FALSE);
	m_Grid.SetRedraw(TRUE, TRUE);

	long nRows = 0;
	long nCols = 0;
	m_Grid.GetCellCount(nRows, nCols);

	int nTab = m_Tab.GetCurSel();

	if(nTab == m_nTabEnv || nTab == m_nTabCondition)  
	{
		if(nTab == m_nTabCondition)
		{
			m_Grid.MergeGridEx(0, nRows-1, 0, 1);
			m_Grid.MergeGrid(0, nRows-1, 2, 3, TRUE, FALSE);
		}
		else if(nTab == m_nTabEnv)
		{
			m_Grid.MergeGridEx(1, nRows-1, 0, 0, FALSE, TRUE);
			m_Grid.MergeGrid(0, nRows-1, 1, 3);
			//		m_Grid.MergeGrid(6, 7, 3, 3, FALSE, TRUE);
			if(m_bDesignLsd == FALSE)
			{
				m_Grid.MergeGridEx(m_nMergeRowSupportBarSlab,m_nMergeRowSupportBarSlab+1,4,4);
				m_Grid.MergeGridEx(m_nMergeRowSupportBarWall,m_nMergeRowSupportBarWall+1,4,4);
				m_Grid.MergeGridEx(m_nMergeRowSupportBarFoot,m_nMergeRowSupportBarFoot+1,4,4);
			}
			
			m_Grid.MergeGrid(1, nRows-1, 2, 3, TRUE, FALSE);
			m_Grid.MergeGrid(0, nRows-1, 4, 5, TRUE, FALSE);
		}
		else
		{
			m_Grid.MergeGridEx(0, nRows-1, 0, 1, TRUE, TRUE, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		}
	}

	if (nTab == m_nTabRebarFactor)
	{
		m_Grid.MergeGridEx(0, nRows-1, 0, 0);
	}
	else if(nTab == m_nTabLsdSupportRebar)
	{
		m_Grid.MergeGridEx(0, nRows-1, 0, 1);
		m_Grid.MergeGridEx(0, nRows-1, 1, 2, TRUE, FALSE, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}
	
	m_btnMirror.EnableWindow(pBri->m_nSeparBri!=0);
	GetDlgItem(IDC_BUTTON_EXAMINE)->EnableWindow(nTab == m_nTabExposureRating);
	m_checkQuickSearch.SetCheck(pBri->m_bQuickSearchLine);
}

// �Է»��� �߰��� �׸��� �߰����־����.... => OnButtonApplyMirror() 
void CInputGenDesignVariable::SetDataDefault()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	
	int nTab = m_Tab.GetCurSel();

	for(long bri = 0; bri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		CRcBridgeRebar *pBri = pStd->GetBridgeRebar(bri);
		BOOL bFirst	= IsFirst(bri);
		pBri->SetDefaultedState(GetIDD(), 0x00000001);
		if(!bFirst && !IsClickDefaultButton(bri)) continue;

		if(bFirst || nTab == m_nTabCondition)
		{
			pBri->SetDataDefaultCondition();
		}
		
		if(bFirst || nTab == m_nTabEnv)				
		{
			pBri->SetDataDefaultEnv();
			// Ʈ�� ����
			AfxGetMainWnd()->SendMessage(WM_USER+8, -1);
		}
		if(bFirst || nTab == m_nTabCover)			pBri->SetDataDefaultCover();

		if(m_bDesignLsd)
		{
			if(bFirst || nTab == m_nTabExposureRating)	pBri->SetDataDefaultExposureRatingLsd();
			if(bFirst || nTab == m_nTabDesignRating)	pBri->SetDataDefaultDesignRatingLsd();
			if(bFirst || nTab == m_nTabRebarFactor)	pBri->SetDataDefaultRebarFactorLsd();
			if(bFirst || nTab == m_nTabRebarSettle)	pBri->SetDataDefaultRebarSettleLsd();
			if(bFirst || nTab == m_nTabRebarJoint)	pBri->SetDataDefaultRebarJointLsd();
			if(bFirst || nTab == m_nTabLsdSupportRebar)	pBri->SetDataDefaultLsdSupportRebar();
		}

		pBri->SyncBridge_DesignData();

		if(nTab == m_nTabEnv)
		{
			// ������ ��ö�ٰ� ��±� ��ǥ �ʱ�ȭ
			pBri->m_bCaledRebarMain_Plane	= FALSE;
			pBri->m_bCaledRebarSupport_Plane	= FALSE;
		}
	}

	SetGridData();
	SetDataInit();
	m_pIn->m_pDoc->SetModifiedFlag();
}

void CInputGenDesignVariable::AddCellByBridgePart(HGINT32 &iRow, HGINT32 &iCol, const CString &strCategory, HGDOUBLE dArr[])
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	UINT nFormatLeft	= DT_LEFT | DT_VCENTER;
	UINT nFormat	= DT_RIGHT | DT_VCENTER;

	UINT CellLine = CELL_LINE_NO;
	CString szUnit_CGS	= _T("kgf/cm��");
	CString szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	
	const HGINT32 iEnd = pBri->GetMaterialDetail() ? ePartCount : ePartUpperSlab + 1;
	for (HGINT32 ix = ePartUpperSlab; ix < iEnd; ++ix)
	{
		CString strPart;

		switch (ix)
		{
		case ePartUpperSlab:
			strPart = "��ü/��ν�����";
			break;
		case ePartInWall:
			strPart = "�߰�������:��ü";
			break;
		case ePartInFoot:
			strPart = "�߰�������:����";
			break;
		default:
			strPart = pBri->GetStringPart(static_cast<EPartOfBridge>(ix));
		}

		m_Grid.AddCell(iRow, iCol, strCategory, DT_CENTER|DT_VCENTER,CellLine);
		m_Grid.AddCell(iRow, iCol+1, "  " + strPart, DT_LEFT,CellLine);
		m_Grid.AddCell(iRow, iCol+2, &dArr[ix], nFormat);
		if(m_bDesignLsd) m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
		else m_Grid.SetCellDec(CELL_DEC_3);
		m_Grid.AddCell(iRow, iCol+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), dArr[ix]), nFormat);
		m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
		m_Grid.AddCell(iRow++, iCol+4, szUnit_Apply, nFormatLeft, CELL_READONLY);
	}

	if (pBri->GetMaterialDetail() == FALSE)
	{
		m_Grid.AddCell(iRow, iCol, strCategory, DT_CENTER|DT_VCENTER,CellLine);
		m_Grid.AddCell(iRow, iCol+1, "  " + pBri->GetStringPart(ePartApsStt), DT_LEFT,CellLine);
		m_Grid.AddCell(iRow, iCol+2, &dArr[ePartApsStt], nFormat);
		if(m_bDesignLsd) m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
		else m_Grid.SetCellDec(CELL_DEC_3);
		m_Grid.AddCell(iRow, iCol+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), dArr[ePartApsStt]), nFormat);
		m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
		m_Grid.AddCell(iRow++, iCol+4, szUnit_Apply, nFormatLeft, CELL_READONLY);
	}
}

void CInputGenDesignVariable::AddCellFckConc(HGINT32 &iRow, HGINT32 &iCol)
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	UINT CellLine = CELL_LINE_NO;
	UINT nFormatCenter = DT_CENTER|DT_VCENTER;

	CString szUnit_CGS   = "kgf/cm��";
	CString szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(iRow, iCol, m_bDesignLsd ? "fck : ��ũ��Ʈ ����" : "  ���� ��ũ��Ʈ ����", nFormatCenter ,CellLine);
	m_Grid.AddCell(iRow, iCol+1, "  ���� ��ũ��Ʈ ����", GVHT_LEFT,CellLine);
	m_Grid.AddCell(iRow, iCol+4, szUnit_Apply, GVHT_LEFT,CELL_READONLY);
	m_Grid.AddCell(iRow, iCol+2, &pBri->m_dFckLeanConc, DT_RIGHT,CellLine);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(iRow++, iCol+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dFckLeanConc), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_1);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);

	szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(iRow, iCol, m_bDesignLsd ? "fck : ��ũ��Ʈ ����" : "  MASS ��ũ��Ʈ ����", nFormatCenter,CellLine);
	m_Grid.AddCell(iRow, iCol+1, "  MASS ��ũ��Ʈ ����", GVHT_LEFT,CellLine);
	m_Grid.AddCell(iRow, iCol+4, szUnit_Apply, GVHT_LEFT,CELL_READONLY);
	m_Grid.AddCell(iRow, iCol+2, &pBri->m_dFckMassConc, DT_RIGHT,CellLine);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(iRow++, iCol+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dFckMassConc), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_1);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
}

void CInputGenDesignVariable::AddCellValidDepthBaseGround(HGINT32 &iRow, HGINT32 &iCol)
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	UINT CellLine = CELL_LINE_NO;

	m_Grid.AddCell(iRow, iCol, m_bDesignLsd ? "��������" : "  ������ ��ȿ ���Ա���", GVHT_LEFT,CellLine);
	m_Grid.AddCell(iRow, iCol+1, "  ������ ��ȿ ���Ա���", GVHT_LEFT,CellLine);
	m_Grid.AddCell(iRow, iCol+4, "m", GVHT_LEFT,CELL_READONLY);
	m_Grid.AddCell(iRow, iCol+2, &pBri->m_dValidDepthBaseGround, DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_3);
	m_Grid.AddCell(iRow++, iCol+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dValidDepthBaseGround), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_3);
}

void CInputGenDesignVariable::AddCellWallSpringEQ(HGINT32 &iRow, HGINT32 &iCol)
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	UINT CellLine = CELL_LINE_NO;

	CString szUnit_CGS		= UNIT_CGS_KGF_CM2;
	CString szUnit_Apply	= m_bDesignLsd ? pStd->GetStringUnitType(szUnit_CGS) : (pStd->IsSI() ? "kN/m��" : UNIT_CGS_KGF_CM2);
	m_Grid.AddCell(iRow, iCol, m_bDesignLsd ? "�����ؼ�" : "  ��ü�� �������������� ���ݺ������", GVHT_LEFT,CellLine);
	m_Grid.AddCell(iRow, iCol+1, "  ��ü�� �������������� ���ݺ������", GVHT_LEFT,CellLine);
	m_Grid.AddCell(iRow, iCol+4, szUnit_Apply, GVHT_LEFT,CELL_READONLY);
	m_Grid.AddCell(iRow, iCol+2, &pBri->m_dEo_CalcWallSpringEQ, DT_RIGHT,CellLine);m_Grid.SetCellDec(pStd->GetDecimal(szUnit_CGS, CELL_DEC_0));m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(iRow++, iCol+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dEo_CalcWallSpringEQ), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_0);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
}

void CInputGenDesignVariable::AddCellModelingEQ(HGINT32 &iRow, HGINT32 &iCol, HGINT32 iCellLine)
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	UINT CellLine = CELL_LINE_NO;
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		m_Grid.AddCell(iRow, iCol, m_bDesignLsd ? "�����ؼ�" : "  �����ؼ�\n", GVHT_LEFT,CellLine);
		m_Grid.AddCell(iRow, iCol+1, "  �ؼ����", GVHT_LEFT,CellLine);
		m_Grid.AddCell(iRow, iCol+2, "  ����� ���߹�", DT_RIGHT, CELL_READONLY);
		m_Grid.AddCell(iRow, iCol+3, "  ����� ���߹�", DT_RIGHT, CELL_READONLY);
		m_Grid.AddCell(iRow++, iCol+4, "", DT_RIGHT, CELL_READONLY);

		for(long ix=0; ix<eGoalEarthQuakeCount; ++ix)
		{
			if(pStd->m_nTypeEarthQuakeApply == 0 && ix > eGoalEQ_Function)
			{
				pBri->m_bGoalEarthQuake[ix] = FALSE;
				continue;
			}

			m_Grid.AddCell(iRow, iCol, m_bDesignLsd ? "�����ؼ�" : "  �����ؼ�\n", GVHT_LEFT,CellLine);
			m_Grid.AddCell(iRow, iCol+1, "  ���ɸ�ǥ", GVHT_LEFT,CellLine);
			m_Grid.AddCell(iRow, iCol+2, &pBri->m_bGoalEarthQuake[ix], DT_RIGHT, CellLine, pBri->GetStringGoalEarthQuake((EGoalOfEarthQuake)ix), CELL_TYPE_CHECK); 
			m_Grid.AddCell(iRow, iCol+3, pBri->GetStringGoalEarthQuake((EGoalOfEarthQuake)ix), DT_RIGHT);//, TRUE, CellLine, "", CELL_TYPE_COMBO, sArr);
			m_Grid.AddCell(iRow++, iCol+4, "", DT_RIGHT, CELL_READONLY);
		}

		m_Grid.AddCell(iRow, iCol, m_bDesignLsd ? "�����ؼ�" : "  �����ؼ�\n", GVHT_LEFT,CellLine);
		m_Grid.AddCell(iRow, iCol+1, "  ��������������(kh) �����", GVHT_LEFT,CellLine);
		m_Grid.AddCell(iRow, iCol+2, &pBri->m_dCoffKh_Sei, DT_RIGHT,iCellLine, _T(""));
		m_Grid.AddCell(iRow, iCol+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dCoffKh_Sei), DT_RIGHT,iCellLine);
		m_Grid.AddCell(iRow++, iCol+4, _T("%"), GVHT_LEFT,CELL_READONLY);
	}
	else
	{
		CHgStringArray sArr;
		sArr.RemoveAll();
		sArr.Add("������ ��ü�� ����� ����");
		sArr.Add("������ ��ü�� ����� ����");
		sArr.Add("������ ��ü�� ����� ����");

		m_Grid.AddCell(iRow, iCol, m_bDesignLsd ? "�����ؼ�" : "  �����ؼ��� �𵨸� ���", GVHT_LEFT,CellLine);
		m_Grid.AddCell(iRow, iCol+1, "  �����ؼ��� �𵨸� ���", GVHT_LEFT,CellLine);		
		m_Grid.AddCell(iRow, iCol+2, &pBri->m_nSelectModelingCountEQ, DT_RIGHT, TRUE, iCellLine, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(iRow, iCol+3, m_pIn->GetStringArray(sArr, pBri->m_nSelectModelingCountEQ), DT_RIGHT);//, TRUE, CellLine, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(iRow++, iCol+4, "", DT_RIGHT, CELL_READONLY);
	}
}

void CInputGenDesignVariable::AddCellFootForce(HGINT32 &iRow, HGINT32 &iCol, HGINT32 iCellLine)
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	UINT CellLine = CELL_LINE_NO;
	if (pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
	{
		CHgStringArray sArr;
		sArr.RemoveAll();
		sArr.Add((pBri->GetTypeJointPosFootingEQ()==1)? "���� ����[M/V]" : "���ʻ��[M/V]");
		sArr.Add((pBri->GetTypeJointPosFootingEQ()==1)? "���� �ϴ�[(M+Hx���ʵβ�/2)/V]" : "���� �ϴ�[(M+Hx���ʵβ�)/V]");

		m_Grid.AddCell(iRow, iCol, m_bDesignLsd ? "�����ؼ�" : "  ���ʰ��� �ܸ�� ������ �ִ���� �����ġ(������)", GVHT_LEFT,CellLine);
		m_Grid.AddCell(iRow, iCol+1, "  ���ʰ��� �ܸ�� ������ �ִ���� �����ġ(������)", GVHT_LEFT,CellLine);
		m_Grid.AddCell(iRow, iCol+2, &pBri->m_nSelectFootForce, DT_RIGHT, TRUE, (pBri->m_nSelectModelingCountEQ==2)? iCellLine:CELL_READONLY, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(iRow, iCol+3, m_pIn->GetStringArray(sArr, pBri->m_nSelectFootForce), DT_RIGHT);//, TRUE, CellLine, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(iRow++, iCol+4, "", DT_RIGHT, CELL_READONLY);
	}
}

void CInputGenDesignVariable::SetGridDataCondition()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	BOOL bKDS17 = (pStd->m_nTypeEarthQuakeApply == 0) ? FALSE : TRUE;

	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 5, 1, 2);
	m_Grid.SetColumnWidth(0,m_bDesignLsd ? 180 : 140);
	m_Grid.SetColumnWidth(1,m_bDesignLsd ? 330 : 200);
	m_Grid.SetColumnWidth(2,m_bDesignLsd ? 280 : 200);
	m_Grid.SetColumnWidth(3,m_bDesignLsd ? 60 : 60);
	m_Grid.SetColumnWidth(4, 80);
	
 	m_Grid.SetRowHeightAll(20);

	CStringArray sArr;
	CString str;
	HGINT32 row = 0;
	HGINT32 col = 0;
	m_Grid.AddCell(row, col,	  "��  ��");
	m_Grid.AddCell(row, col+1,	  "��  ��");
	m_Grid.AddCell(row, col+2, "������");
	m_Grid.AddCell(row, col+3, "������");
	m_Grid.AddCell(row++, col+4, "��  ��");


	// ���⼭ ���ʹ� �Ϻ��� �����̴�. 
	// ����, �����걳�� ��� ���⼭ ���ʹ� ������� ����
//	UINT CellLine = (pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB) ? CELL_LINE_NO : CELL_READONLY;
	UINT CellLine = CELL_LINE_NO;

	CString szUnit_CGS   = "tonf/m��";
	CString szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);

	if (m_bDesignLsd)
	{
		m_Grid.AddCell(row, col, "��� ����ġ �� �Է�", DT_CENTER,CellLine);
		m_Grid.AddCell(row, col+1, "  ��� ����ġ �� �Է�", GVHT_LEFT,CellLine);
		
		sArr.RemoveAll();
		sArr.Add("����");
		sArr.Add("������");
		m_Grid.AddCell(row, col+2, &pBri->GetMaterialDetail(), DT_RIGHT, CellLine, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row, col+3, m_pIn->GetStringArray(sArr, !pBri->GetMaterialDetail()), DT_RIGHT);
		m_Grid.AddCell(row++, col+4, "", DT_RIGHT, CELL_READONLY);

		AddCellByBridgePart(row, col, "fck : ��ũ��Ʈ ����", pBri->m_Fck);
		
		AddCellFckConc(row, col);
				
		AddCellByBridgePart(row, col, "fy : ö�� �׺�����", pBri->m_Fy);		

		AddCellByBridgePart(row, col, "fvy : ����ö�� �׺�����", pBri->m_Fvy);

		AddCellByBridgePart(row, col, "Ec : ��ũ��Ʈ ź�����", pBri->m_Ec);

		m_Grid.AddCell(row, col, "���Ư�� �� �����", DT_CENTER,CellLine);
		m_Grid.AddCell(row, col+1, "  �� : ���������", GVHT_LEFT,CellLine);
		m_Grid.AddCell(row, col+2, &pBri->GetRateDryAndShrinkage(), DT_RIGHT); 
		m_Grid.SetCellDec(CELL_DEC_6);
		m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->GetRateDryAndShrinkage()), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_6);
		m_Grid.AddCell(row++, col+4, "", DT_RIGHT, CELL_READONLY);

		m_Grid.AddCell(row, col, "���Ư�� �� �����", DT_CENTER,CellLine);
		m_Grid.AddCell(row, col+1, "  �� : ����â���", GVHT_LEFT,CellLine);
		m_Grid.AddCell(row, col+2, &pBri->GetCoefficientExpansion(), DT_RIGHT); m_Grid.SetCellDec(CELL_DEC_7);
		m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->GetCoefficientExpansion()), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_7);
		m_Grid.AddCell(row++, col+4, "", DT_RIGHT, CELL_READONLY);

		m_Grid.AddCell(row, col, "���ӽ�����", DT_CENTER,CellLine);
		m_Grid.AddCell(row, col+1, "  fy : ������ ����", GVHT_LEFT,CellLine);
		m_Grid.AddCell(row, col+2, &pBri->GetDBar_ForDowelBar(), DT_RIGHT, CellLine, pBri->GetDBar_ForDowelBar() ? "D��" : "H��", CELL_TYPE_CHECK);
		m_Grid.AddCell(row, col+3, pBri->GetDBar_ForDowelBar() ? "D��" : "H��", DT_RIGHT, CellLine);
		m_Grid.AddCell(row++, col+4, "", DT_RIGHT, CELL_READONLY);

		/*
		m_Grid.AddCell(row, col, "��������", DT_CENTER,CellLine);
		m_Grid.AddCell(row++, col+1, "  ���������� ����", GVHT_LEFT,CellLine);

		m_Grid.AddCell(row, col, "��������", DT_CENTER,CellLine);
		m_Grid.AddCell(row++, col+1, "  ���������� Nġ", GVHT_LEFT,CellLine);

		m_Grid.AddCell(row, col, "��������", DT_CENTER,CellLine);
		m_Grid.AddCell(row++, col+1, "  ���������� ������", GVHT_LEFT,CellLine);

		m_Grid.AddCell(row, col, "��������", DT_CENTER,CellLine);
		m_Grid.AddCell(row++, col+1, "  ���������� �����߷�", GVHT_LEFT,CellLine);

		m_Grid.AddCell(row, col, "��������", DT_CENTER,CellLine);
		m_Grid.AddCell(row++, col+1, "  ���ݺ������(Eo)", GVHT_LEFT,CellLine);

		m_Grid.AddCell(row, col, "��������", DT_CENTER,CellLine);
		m_Grid.AddCell(row++, col+1, "  �������Ƽۺ�(v)", GVHT_LEFT,CellLine);

		AddCellValidDepthBaseGround(row++, col);

		AddCellPile(row, col);
		row += 3;
		*/
	}

	m_Grid.AddCell(row, col, m_bDesignLsd ? "��ä�� �� ������" : "  ��ä���� �����߷�", GVHT_LEFT,CellLine);
	m_Grid.AddCell(row, col+1, "  ��ä���� �����߷�", GVHT_LEFT,CellLine);
	m_Grid.AddCell(row, col+4, szUnit_Apply, GVHT_LEFT,CELL_READONLY);
	m_Grid.AddCell(row, col+2, &pBri->m_dUnitWeigthBackFill, DT_RIGHT,CellLine);m_Grid.SetCellDec(pStd->GetDecimal(szUnit_CGS, CELL_DEC_3));m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(row++, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dUnitWeigthBackFill), DT_RIGHT,CellLine);m_Grid.SetCellDec(pStd->GetDecimal(szUnit_CGS, CELL_DEC_3));m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);

	if(pBri->IsBoxType() && pBri->m_bApplyGroundWaterLevel)
	{
		m_Grid.AddCell(row, col, m_bDesignLsd ? "��ä�� �� ������" : "  ��ä���� ���� �����߷�", GVHT_LEFT,CellLine);
		m_Grid.AddCell(row, col+1, "  ��ä���� ���� �����߷�", GVHT_LEFT,CellLine);
		m_Grid.AddCell(row, col+4, szUnit_Apply, GVHT_LEFT,CELL_READONLY);
		m_Grid.AddCell(row, col+2, &pBri->m_dUnitWeigthBackFillWater, DT_RIGHT,CellLine);m_Grid.SetCellDec(pStd->GetDecimal(szUnit_CGS, CELL_DEC_3));m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
		m_Grid.AddCell(row++, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dUnitWeigthBackFillWater), DT_RIGHT,CellLine);m_Grid.SetCellDec(pStd->GetDecimal(szUnit_CGS, CELL_DEC_3));m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	}

	m_Grid.AddCell(row, col, m_bDesignLsd ? "��ä�� �� ������" : "  ��ä���� ���θ�����", GVHT_LEFT,CellLine);
	m_Grid.AddCell(row, col+1, "  ��ä���� ���θ�����", GVHT_LEFT,CellLine);
	m_Grid.AddCell(row, col+4, "��", GVHT_LEFT, CELL_READONLY);
	m_Grid.AddCell(row, col+2, &pBri->m_dAngleFrictionBackFill, DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_2);
	m_Grid.AddCell(row++, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dAngleFrictionBackFill), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_2);
	
	if (m_bDesignLsd == FALSE)
	{
		if(pStd->IsRailDesign())
		{
			szUnit_CGS   = "tonf/m��";
			szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
			m_Grid.AddCell(row, col, "  ��а��� ���� Ȱ����", GVHT_LEFT,CellLine);
			m_Grid.AddCell(row, col+1, "  ��а��� ���� Ȱ����", GVHT_LEFT,CellLine);
			m_Grid.AddCell(row, col+4, szUnit_Apply, GVHT_LEFT,CELL_READONLY);
			m_Grid.AddCell(row, col+2, &pBri->m_dLiveLoadAlways, DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_3);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
			m_Grid.AddCell(row++, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dLiveLoadAlways), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_3);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
			//
			m_Grid.AddCell(row, col, "  ��а��� ���� ��������", GVHT_LEFT,CellLine);
			m_Grid.AddCell(row, col+1, "  ��а��� ���� ��������", GVHT_LEFT,CellLine);
			m_Grid.AddCell(row, col+4, szUnit_Apply, GVHT_LEFT,CELL_READONLY);
			m_Grid.AddCell(row, col+2, &pBri->m_dDeadLoadAlways, DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_3);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
			m_Grid.AddCell(row++, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dDeadLoadAlways), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_3);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
		}
		else
		{
			szUnit_CGS   = "tonf/m��";
			szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
			m_Grid.AddCell(row, col, "  ��а��� ��������", GVHT_LEFT,CellLine);
			m_Grid.AddCell(row, col+1, "  ��а��� ��������", GVHT_LEFT,CellLine);
			m_Grid.AddCell(row, col+4, szUnit_Apply, GVHT_LEFT,CELL_READONLY);
			m_Grid.AddCell(row, col+2, &pBri->m_dLiveLoadAlways, DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_3);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
			m_Grid.AddCell(row++, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dLiveLoadAlways), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_3);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
		}


		m_Grid.AddCell(row, col, "  �������߿� ���� ����� ����", GVHT_LEFT,CellLine);
		m_Grid.AddCell(row, col+1, "  �������߿� ���� ����� ����", GVHT_LEFT,CellLine);
		sArr.RemoveAll();
		sArr.Add("��а��� �������� ����");
		sArr.Add("����Ȱ���� x ��а��");
		m_Grid.AddCell(row, col+2, &pBri->m_nSelectLiveLoadHori, DT_RIGHT, TRUE, CellLine, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row, col+3, m_pIn->GetStringArray(sArr, pBri->m_nSelectLiveLoadHori), DT_RIGHT);//, TRUE, CellLine, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+4, "", GVHT_LEFT,CELL_READONLY);


		szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
		m_Grid.AddCell(row, col, "  ����Ȱ����", GVHT_LEFT,CellLine);
		m_Grid.AddCell(row, col+1, "  ����Ȱ����", GVHT_LEFT,CellLine);
		m_Grid.AddCell(row, col+4, szUnit_Apply, GVHT_LEFT,CELL_READONLY);
		m_Grid.AddCell(row, col+2, &pBri->m_dLiveLoadFootway, DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_3);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
		m_Grid.AddCell(row++, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dLiveLoadFootway), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_3);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	}
	
	szUnit_CGS   = "tonf/m��";
	szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(row, col, m_bDesignLsd ? "��ä�� �� ������" : "  ��������� �����߷�(�������� �������)", GVHT_LEFT,CellLine);
	m_Grid.AddCell(row, col+1, "  ��������� �����߷�(�������� �������)", GVHT_LEFT,CellLine);
	m_Grid.AddCell(row, col+4, szUnit_Apply, GVHT_LEFT,CELL_READONLY);
	m_Grid.AddCell(row, col+2, &pBri->m_dUnitWeightHeapSoil, DT_RIGHT,CellLine);m_Grid.SetCellDec(pStd->GetDecimal(szUnit_CGS, CELL_DEC_3));m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(row++, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dUnitWeightHeapSoil), DT_RIGHT,CellLine);m_Grid.SetCellDec(pStd->GetDecimal(szUnit_CGS, CELL_DEC_3));m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	

//	m_Grid.AddCell(row, col, "  ���������� �����߷�", GVHT_LEFT,CellLine);
//	m_Grid.AddCell(row, col+1, "  ���������� �����߷�", GVHT_LEFT,CellLine);
//	m_Grid.AddCell(row, col+4, szUnit_Apply, GVHT_LEFT,CellLine);
//	m_Grid.AddCell(row, col+2, &pBri->m_dUnitWeightValidDepth, DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_3);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
//	m_Grid.AddCell(row++, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dUnitWeightValidDepth), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_3);m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	
	if (m_bDesignLsd == FALSE)
	{
		AddCellValidDepthBaseGround(row, col);

		//	szUnit_CGS		= UNIT_CGS_KGF_CM2;
		//	szUnit_Apply	= pStd->GetStringUnitType(szUnit_CGS);
		//	m_Grid.AddCell(row, col, "  ����������� ��а��", GVHT_LEFT,CellLine); 
		//	m_Grid.AddCell(row, col+1, "  ����������� ��а��", GVHT_LEFT,CellLine);
		//	sArr.RemoveAll();
		//	sArr.Add("Rankine ���");
		//	sArr.Add("Coulomb ���");
		//	m_Grid.AddCell(row, col+2, &pBri->m_nTypeToapSafeCheck, DT_RIGHT, TRUE, CellLine, "", CELL_TYPE_COMBO, sArr);
		//	m_Grid.AddCell(row++, col+3,  m_pIn->GetStringArray(sArr, pBri->m_nTypeToapSafeCheck), DT_RIGHT);

		m_Grid.AddCell(row, col, "  ������ ���� ������\n", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  ���� ", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pBri->m_SafeFactorOverturning[0], DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_2);
		m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_SafeFactorOverturning[0]), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_2);
		m_Grid.AddCell(row++, col+4, "", GVHT_LEFT, CELL_READONLY);

		BOOL bEarthQuakeDesign = pBri->GetTypeDesingEQ() != 0 ? TRUE : FALSE;
		if(bEarthQuakeDesign)
		{
			m_Grid.AddCell(row, col, "  ������ ���� ������\n", GVHT_LEFT,CellLine);
			m_Grid.AddCell(row, col+1, "  ������ ", GVHT_LEFT,CellLine);
			m_Grid.AddCell(row, col+2, &pBri->m_SafeFactorOverturning[1], DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_2);
			m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_SafeFactorOverturning[1]), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_2);
			m_Grid.AddCell(row++, col+4, "", GVHT_LEFT, CELL_READONLY);

		}

		m_Grid.AddCell(row, col, "  Ȱ���� ���� ������\n", GVHT_LEFT,CellLine);
		m_Grid.AddCell(row, col+1, "  ����", GVHT_LEFT,CellLine);
		m_Grid.AddCell(row, col+2, &pBri->m_SafeFactorSliding[0], DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_2);
		m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_SafeFactorSliding[0]), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_2);
		m_Grid.AddCell(row++, col+4, "", GVHT_LEFT, CELL_READONLY);

		if(bEarthQuakeDesign)
		{
			m_Grid.AddCell(row, col, "  Ȱ���� ���� ������\n", GVHT_LEFT,CellLine);
			m_Grid.AddCell(row, col+1, "  ������", GVHT_LEFT,CellLine);
			m_Grid.AddCell(row, col+2, &pBri->m_SafeFactorSliding[1], DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_2);
			m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_SafeFactorSliding[1]), DT_RIGHT,CellLine);m_Grid.SetCellDec(CELL_DEC_2);
			m_Grid.AddCell(row++, col+4, "", GVHT_LEFT, CELL_READONLY);

			AddCellWallSpringEQ(row, col);

			AddCellModelingEQ(row, col, CellLine);

			AddCellFootForce(row, col, CellLine);
		}

		//	m_Grid.AddCell(row, col, "  �����ؼ��� ���ҽ��������� ����", GVHT_LEFT,CellLine);
		//	m_Grid.AddCell(row, col+1, "  �����ؼ��� ���ҽ��������� ����", GVHT_LEFT,CellLine);
		//	sArr.RemoveAll();
		//	sArr.Add("���ҸӸ� ����");
		//	sArr.Add("���ҸӸ� ����");
		//	m_Grid.AddCell(row, col+2, &pBri->m_nSelectPileCalcSpringEQ, DT_RIGHT, TRUE, CellLine, "", CELL_TYPE_COMBO, sArr);
		//	m_Grid.AddCell(row++, col+3, m_pIn->GetStringArray(sArr, pBri->m_nSelectPileCalcSpringEQ), DT_RIGHT);//, TRUE, CellLine, "", CELL_TYPE_COMBO, sArr);
	}	

	m_Grid.AddCell(row, col, m_bDesignLsd ? "�����ؼ�" : "  2D Frame �ؼ� ��", GVHT_LEFT,CELL_LINE_NO); 
	m_Grid.AddCell(row, col+1, "  2D Frame �ؼ� ��", GVHT_LEFT,CELL_LINE_NO);
	sArr.RemoveAll();
	sArr.Add("SAP 2000 7");
	sArr.Add("SAP 2000 8");
	sArr.Add("SAP 2000 9");
	sArr.Add("SAP 2000 10");
	sArr.Add("MIDAS Civil");
	m_Grid.AddCell(row, col+2, &pBri->m_nSelect2DFrameTool, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->m_nSelect2DFrameTool), DT_RIGHT);
	m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);
	
	m_Grid.AddCell(row, col, m_bDesignLsd ? "�����ؼ�" : "  3D Plate �ؼ� ��", GVHT_LEFT,CELL_LINE_NO); 
	m_Grid.AddCell(row, col+1, "  3D Plate �ؼ� ��", GVHT_LEFT,CELL_LINE_NO);

	// �������� m_Grid.SetCellBaseOfIndex(1); �߰��� ����
	// SAP 2000�� ������ �ȵǴµ� �ұ��ϰ�, �޺��� ��� ���� �־ 
	// �޺����� SAP2000�� ���� �ϱ� ���� BaseOfIndex�� 1�� �ٲٷ� �׸��� ������
	// ���࿡ SAP 2000�� �ٽ� �������� SetCellBaseOfIndex�� �ٽ� 0���� ���� ���ƾ� ��.
	sArr.RemoveAll();
	//sArr.Add("SAP 2000");
	sArr.Add("MIDAS Civil");
	sArr.Add("3D �ؼ� ����");
	m_Grid.AddCell(row, col+2, &pBri->m_nSelect3DShellTool, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr); m_Grid.SetCellBaseOfIndex(1);
	m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->m_nSelect3DShellTool-1), DT_RIGHT);
	m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);

	m_Grid.AddCell(row, col, m_bDesignLsd ? "�����ؼ�" : "  ������ �ؼ����", GVHT_LEFT,CELL_LINE_NO); 
	m_Grid.AddCell(row, col+1, "  ������ �ؼ����", GVHT_LEFT,CELL_LINE_NO);
	sArr.RemoveAll();
	sArr.Add("�����");
	sArr.Add("����� + 3D Plate �ؼ�");
	sArr.Add("3D Plate �ؼ�");
	m_Grid.AddCell(row, col+2, &pBri->m_nSelectWingCalc, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->m_nSelectWingCalc), DT_RIGHT);
	m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);

	m_Grid.AddCell(row, col, m_bDesignLsd ? "�����ؼ�" : "  3D Plate �ؼ���� ����", GVHT_LEFT,CELL_LINE_NO); 
	m_Grid.AddCell(row, col+1, "  3D Plate �ؼ���� ����", GVHT_LEFT,CELL_LINE_NO);
	sArr.RemoveAll();
	sArr.Add("������ �� ���� ��հ�");
	sArr.Add("������ �� ���� �ִ밪");
	sArr.Add("������ Center");
	m_Grid.AddCell(row, col+2, &pBri->m_nSelect3DOutputValue, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->m_nSelect3DOutputValue), DT_RIGHT);
	m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);

	if(pBri->m_nSelect2DFrameTool == SOLVE_MIDAS || pBri->m_nSelect3DShellTool == 1)
	{
		m_Grid.AddCell(row, col, m_bDesignLsd ? "�����ؼ�" : "  MIDAS input version", GVHT_LEFT,CELL_LINE_NO); 
		m_Grid.AddCell(row, col+1, "  MIDAS input version", GVHT_LEFT,CELL_LINE_NO);

		sArr.RemoveAll();
		sArr.Add("MIDAS Civil 4.3");
		sArr.Add("MIDAS Civil 8.6.1");
		m_Grid.AddCell(row, col+2, &pBri->m_nSelectMidasVer, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->m_nSelectMidasVer), DT_RIGHT);
		m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);
	}
	
	sArr.RemoveAll();
	sArr.Add(hggettext("�ִ� �ܸ�� ��հ�"));
	sArr.Add(hggettext("�ִ� 2�� ���� ��հ�"));
	sArr.Add(hggettext("�ִ� 3�� ���� ��հ�"));
	sArr.Add(hggettext("�ִ� 4�� ���� ��հ�"));
	sArr.Add(hggettext("�ִ� 5�� ���� ��հ�"));
	// 	sArr.Add(hggettext("���� Center��"));
	m_Grid.AddCell(row, col, m_bDesignLsd ? "�����ؼ�" : "  3D Plate �ؼ���� ���� �����", GVHT_LEFT,CELL_LINE_NO); 
	m_Grid.AddCell(row, col+1, "  3D Plate �ؼ���� ���� �����", GVHT_LEFT,CELL_LINE_NO);
	m_Grid.AddCell(row, col+2, &pBri->m_nApplyWingWallCalcValueCount, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->m_nApplyWingWallCalcValueCount), DT_RIGHT);
	m_Grid.AddCell(row++, col+4, _T(""), DT_RIGHT, CELL_READONLY);

	if (m_bDesignLsd)
	{
		m_Grid.AddCell(row, col, "�����ؼ�", GVHT_LEFT,CellLine); 
		m_Grid.AddCell(row, col+1, "  �����ؼ�����", GVHT_LEFT,CellLine);
		sArr.RemoveAll();
		sArr.Add("�����ؼ� ��������"); 
		sArr.Add("�����ؼ� ����"); 
		m_Grid.AddCell(row, col+2, &pBri->GetTypeDesingEQ(), DT_RIGHT, TRUE, CellLine, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->GetTypeDesingEQ()), DT_RIGHT);
		m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);

		HGINT32 iCellLineEQ = pBri->GetTypeDesingEQ() == 0 ? CELL_READONLY : CellLine;

		AddCellModelingEQ(row, col, iCellLineEQ);

		m_Grid.AddCell(row, col, "�����ؼ�", GVHT_LEFT,CELL_LINE_NO); 
		m_Grid.AddCell(row, col+1, "  �������", GVHT_LEFT,CELL_LINE_NO); 
		if(bKDS17)
		{
			sArr.RemoveAll(); sArr.Add("I ���"); sArr.Add("II���"); sArr.Add("Ư��� 2400"); sArr.Add("Ư��� 4800");
			m_Grid.AddCell(row, col+2, &pBri->GetDegreeEarthQuake(), DT_RIGHT, TRUE, iCellLineEQ, "", CELL_TYPE_AUTO, sArr, &m_EarthquakeGradeKDS17Dlg);
		}
		else
		{
			sArr.RemoveAll(); sArr.Add("I ���"); sArr.Add("II���");
			m_Grid.AddCell(row, col+2, &pBri->GetDegreeEarthQuake(), DT_RIGHT, TRUE, iCellLineEQ, "", CELL_TYPE_AUTO, sArr, &m_EarthquakeGradeDlg);			
		}
		m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->GetDegreeEarthQuake()), DT_RIGHT);
		m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);

		m_Grid.AddCell(row, col, "�����ؼ�", GVHT_LEFT,CELL_LINE_NO); 
		m_Grid.AddCell(row, col+1, "  ��������", GVHT_LEFT,CELL_LINE_NO);
		sArr.RemoveAll(); sArr.Add("I ����"); sArr.Add("II����");
		m_Grid.AddCell(row, col+2, &pBri->GetRegionEarthQuake(), DT_RIGHT, TRUE, iCellLineEQ, "", CELL_TYPE_AUTO, sArr, &m_EarhtquakeDlg);
		m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->GetRegionEarthQuake()), DT_RIGHT);
		m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);

		// KDS17�� ��������, �ü��������� ���ݰ���� �ް� �־��µ� KDS������ ���ݰ���� �޵��� ����. -KDS�϶��� �׳� �Է°�����
		if(bKDS17)
		{
			m_Grid.AddCell(row, col, "�����ؼ�", GVHT_LEFT,CELL_LINE_NO); 
			m_Grid.AddCell(row, col+1, "  ���ݺ������", GVHT_LEFT,CELL_LINE_NO);
			m_Grid.AddCell(row, col+2, &pBri->GetCoefficientGround(), DT_RIGHT, iCellLineEQ); m_Grid.SetCellDec(CELL_DEC_2);
			m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->GetCoefficientGround()), DT_RIGHT,iCellLineEQ);m_Grid.SetCellDec(CELL_DEC_2);
			m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);
		}
		else
		{
			m_Grid.AddCell(row, col, "�����ؼ�", GVHT_LEFT,CELL_LINE_NO); 
			m_Grid.AddCell(row, col+1, "  ���ݰ��", GVHT_LEFT,CELL_LINE_NO);
			m_Grid.AddCell(row, col+2, &pBri->GetCoefficientGround(), DT_RIGHT, iCellLineEQ, "", CELL_TYPE_AUTO, sArr, &m_EarthquakeFactDlg); m_Grid.SetCellDec(CELL_DEC_2);
			m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->GetCoefficientGround()), DT_RIGHT,iCellLineEQ);
			m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);
		}

		if(bKDS17)
		{
			sArr.RemoveAll();
			m_Grid.AddCell(row, col, "�����ؼ�", GVHT_LEFT,CELL_LINE_NO); 
			m_Grid.AddCell(row, col+1, "  ��������", GVHT_LEFT,CELL_LINE_NO);
			for(long ix = 0; ix < 6; ++ix)
			{
				str.Format(_T("S%d"), ix+1);
				sArr.Add(str);
			}
			m_Grid.AddCell(row, col+2, &pBri->GetTypeCoefficientGround(), DT_RIGHT, TRUE, iCellLineEQ, "", CELL_TYPE_AUTO, sArr, &m_EarthquakeFactKDS17Dlg);
			m_Grid.AddCell(row, col+3, sArr.GetAt(pBri->GetTypeCoefficientGround()), DT_RIGHT,iCellLineEQ);			
			m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);
		}

		if(bKDS17)
		{
			m_Grid.AddCell(row, col, "�����ؼ�", GVHT_LEFT,CELL_LINE_NO); 
			m_Grid.AddCell(row, col+1, hggettext("  �����(��) : ���躸�����(CD) ����"), GVHT_LEFT,CELL_LINE_NO);
			m_Grid.AddCell(row, col+2, &pBri->GetRateDAMP_EQ_CD(), DT_RIGHT, iCellLineEQ); m_Grid.SetCellDec(CELL_DEC_3);
			m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->GetRateDAMP_EQ_CD()), DT_RIGHT,iCellLineEQ);
			m_Grid.AddCell(row++, col+4, "%",GVHT_LEFT, CELL_READONLY);
		}

		m_Grid.AddCell(row, col, "�����ؼ�", GVHT_LEFT,CELL_LINE_NO); 
		m_Grid.AddCell(row, col+1, "  �߷°��ӵ�", GVHT_LEFT,CELL_LINE_NO);
		m_Grid.AddCell(row, col+2, &pBri->GetGravity(), DT_RIGHT, iCellLineEQ); m_Grid.SetCellDec(CELL_DEC_3);
		m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->GetGravity()), DT_RIGHT,iCellLineEQ);
		m_Grid.AddCell(row++, col+4, "m/sec��",GVHT_LEFT, CELL_READONLY);

		m_Grid.AddCell(row, col, "�����ؼ�", GVHT_LEFT,CELL_LINE_NO); 
		m_Grid.AddCell(row, col+1, "  �����ؼ��� ���� �𵨸� ���", GVHT_LEFT,CELL_LINE_NO);
		sArr.RemoveAll(); 
		sArr.Add("��ü������ ���ʰ����� ����"); 
		sArr.Add("��ü������ ���ʰ����� ��������"); 
		sArr.Add("���ʰ����� ���Ѱ������� ����");
		m_Grid.AddCell(row, col+2, &pBri->GetTypeRigidFootingEQ(), DT_RIGHT, TRUE, iCellLineEQ, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->GetTypeRigidFootingEQ()), DT_RIGHT, iCellLineEQ);
		m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);

		AddCellWallSpringEQ(row, col);

		m_Grid.AddCell(row, col, "�����ؼ�", GVHT_LEFT,CELL_LINE_NO); 
		m_Grid.AddCell(row, col+1, "  ��ü���� ��������� ȸ������ ����", GVHT_LEFT,CELL_LINE_NO);
		sArr.RemoveAll(); sArr.Add("����"); sArr.Add("�������");
		m_Grid.AddCell(row, col+2, &pBri->GetApplyRigidWallEQRotate(), DT_RIGHT, iCellLineEQ, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row, col+3, m_pIn->GetStringArray(sArr, !pBri->GetApplyRigidWallEQRotate()), DT_RIGHT);
		m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);

		m_Grid.AddCell(row, col, "�����ؼ�", GVHT_LEFT,CELL_LINE_NO); 
		m_Grid.AddCell(row, col+1, "  �����ؼ��� ���� �𵨸� ��ġ", GVHT_LEFT,CELL_LINE_NO);
		sArr.RemoveAll(); 
		sArr.Add("������ ��� ��ġ"); 
		sArr.Add("������ ���� ��ġ"); 
		m_Grid.AddCell(row, col+2, &pBri->GetTypeJointPosFootingEQ(), DT_RIGHT, TRUE, iCellLineEQ, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->GetTypeJointPosFootingEQ()), DT_RIGHT);
		m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);

		m_Grid.AddCell(row, col, "�����ؼ�", GVHT_LEFT,CELL_LINE_NO); 
		m_Grid.AddCell(row, col+1, "  �����ؼ��� ���� �𵨸� ����", GVHT_LEFT,CELL_LINE_NO);
		sArr.RemoveAll(); 
		sArr.Add("������ ����"); 
		sArr.Add("��ü�� ����"); 
		m_Grid.AddCell(row, col+2, &pBri->GetTypePropertyPosFootingEQ(), DT_RIGHT, TRUE, iCellLineEQ, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->GetTypePropertyPosFootingEQ()), DT_RIGHT);
		m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);

		m_Grid.AddCell(row, col, "�����ؼ�", GVHT_LEFT,CELL_LINE_NO); 
		m_Grid.AddCell(row, col+1, "  �����ؼ��� ������� ���� ��ġ", GVHT_LEFT,CELL_LINE_NO);
		sArr.RemoveAll(); 
		sArr.Add("�������߰� ���������� �շ� ���� - ���μ������(�Ǽ������,2001)"); 
		sArr.Add("�������߰� ���������� �и� ���� - �����������(�ѱ����ΰ���,2000)"); 
		m_Grid.AddCell(row, col+2, &pBri->GetTypeSoilPressureEQ(), DT_RIGHT, TRUE, iCellLineEQ, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->GetTypeSoilPressureEQ()), DT_RIGHT);
		m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);

		m_Grid.AddCell(row, col, "�����ؼ�", GVHT_LEFT,CELL_LINE_NO); 
		m_Grid.AddCell(row, col+1, "  �����ؼ��� �����߷� ���� ����", GVHT_LEFT,CELL_LINE_NO);
		sArr.RemoveAll(); 
		sArr.Add("��α�����(������,��Ÿ������)"); 
		sArr.Add("��α�����,�Ϻα���1/2"); 
		m_Grid.AddCell(row, col+2, &pBri->GetTypeUpperWeightEQ(), DT_RIGHT, TRUE, iCellLineEQ, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row, col+3,  m_pIn->GetStringArray(sArr, pBri->GetTypeUpperWeightEQ()), DT_RIGHT);
		m_Grid.AddCell(row++, col+4,  "", DT_RIGHT, CELL_READONLY);

		AddCellFootForce(row, col, iCellLineEQ);
	}

	if(m_bDesignLsd == FALSE)
	{
		AddCellFckConc(row, col);
	}
}

// ���� ���û���
void CInputGenDesignVariable::SetGridDataEnv()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar *pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	BOOL bArch = (pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH);
	const HGBOOL bLsd = m_bDesignLsd;

	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 6, 1, 4);

	UINT nFormat = DT_CENTER|DT_VCENTER;

	CStringArray sArr;
	long row = 0;
	long col = 0;
	m_Grid.AddCell(row, col,	  "��  ��");
	m_Grid.AddCell(row, col+1,	  "��  ��");
	m_Grid.AddCell(row, col+2,	  "��  ��");
	m_Grid.AddCell(row, col+3,	  "��  ��");
	m_Grid.AddCell(row, col+4, "������");
	m_Grid.AddCell(row++, col+5, "������");

	if(bLsd == FALSE)
	{
		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ����ö�ٿ� ���� ����µ�ö�ٺ� ����", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  ����ö�ٿ� ���� ����µ�ö�ٺ� ����", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  ����ö�ٿ� ���� ����µ�ö�ٺ� ����", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckMainSlabAndFootTemper,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bCheckMainSlabAndFootTemper ? "����" : "�������", DT_RIGHT);

		if(pBri->IsBoxType() == FALSE)
		{
			m_Grid.AddCell(row, col, "�ܸ���� ö��");
			m_Grid.AddCell(row, col+1, "  ���� �ڰ���");
			m_Grid.AddCell(row, col+2, "  �µ�ö�ٺ� �ּ�ö�ٺ�� ����(���ΰ���)", DT_LEFT);
			m_Grid.AddCell(row, col+3, "  �µ�ö�ٺ� �ּ�ö�ٺ�� ����(���ΰ���)", DT_LEFT);
			m_Grid.AddCell(row, col+4, &pBri->m_bCheckRebarRateFooting,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
			m_Grid.AddCell(row++, col+5, pBri->m_bCheckRebarRateFooting ? "����" : "�������", DT_RIGHT);
		}

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ����ö�ٷ� ����\n(������)\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  ��ö�ٺ�\n", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  [��.�� 4.7.5.7(2)]", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarSlabMain,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bCheckSuppoerBarSlabMain ? "����" : "�������", DT_RIGHT);

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ����ö�ٷ� ����\n(������)\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  ��ö�ٺ�\n", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  ��ö���� 20%", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarSlabMainOver20,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bCheckSuppoerBarSlabMainOver20 ? "����" : "�������", DT_RIGHT);

		m_nMergeRowSupportBarSlab = row;
		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ����ö�ٷ� ����\n(������)\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  �ܸ�����\n", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  fy�� 400MPa ������ ����ö��", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarSlabArea,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5,&pBri->m_dCheckSuppoerBarSlabArea, DT_RIGHT,CELL_LINE_NO);m_Grid.SetCellDec(CELL_DEC_4);

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ����ö�ٷ� ����\n(������)\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  �ܸ�����\n", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  fy�� 400MPa �ʰ��ϴ� ����ö��", DT_LEFT);
		m_Grid.AddCell(row, col+4, "");
		m_Grid.AddCell(row++, col+5, "0.002 x 400/fy", DT_RIGHT, CELL_READONLY);

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ����ö�ٷ� ����\n(������)\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  ����� 300 mm���̻�", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  ����� 300 mm���̻�", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarSlab300,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bCheckSuppoerBarSlab300 ? "����" : "�������", DT_RIGHT);
		//////////////////////////////////////////////////////////////////////////
		CString strTitle = bArch? "  ����ö�ٷ� ����\n(��ü, ��ġ����)\n\n" : "  ����ö�ٷ� ����\n(��ü)\n\n";
		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, strTitle, DT_LEFT);
		m_Grid.AddCell(row, col+2, "  0.0015hs", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  0.0015hs", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarWall15hs,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5,_T("h:����β�,s:ö�ٰ���"), DT_RIGHT);

		m_nMergeRowSupportBarWall = row;
		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, strTitle, DT_LEFT);
		m_Grid.AddCell(row, col+2, "  �ܸ�����\n", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  fy=400MPa �̻��̸� ��16 ������ ����ö��", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarWallArea,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5,&pBri->m_dCheckSuppoerBarWallAreaForOver400, DT_RIGHT,CELL_LINE_NO);m_Grid.SetCellDec(CELL_DEC_4);

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, strTitle, DT_LEFT);
		m_Grid.AddCell(row, col+2, "  �ܸ�����\n", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  ��Ÿ ����ö��", DT_LEFT);
		//m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarWallArea,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5,&pBri->m_dCheckSuppoerBarWallArea, DT_RIGHT,CELL_LINE_NO);m_Grid.SetCellDec(CELL_DEC_4);

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, strTitle, DT_LEFT);
		m_Grid.AddCell(row, col+2, "  ��ö�ٺ�", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  ��ö���� 20%", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarWallMainOver20,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bCheckSuppoerBarWallMainOver20 ? "����" : "�������", DT_RIGHT);

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, strTitle, DT_LEFT);
		m_Grid.AddCell(row, col+2, "  ����� 300 mm���̻�", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  ����� 300 mm���̻�", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarWall300,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bCheckSuppoerBarWall300 ? "����" : "�������", DT_RIGHT);

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, strTitle, DT_LEFT);
		m_Grid.AddCell(row, col+2, "  ����� ��ġ ö�ٷ� ����", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  ����� ��ġ ö�ٷ� ����", DT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_bSelectSupportBarWall,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, "����鿡 1/2�̻�,2/3���� ��ġ", DT_RIGHT);
		if(bArch)		// Arch�� �ϰ�� ��ġ���� ��±� ���� �ɼ�
		{
			m_Grid.AddCell(row, col, "�ܸ���� ö��");
			m_Grid.AddCell(row, col+1, "  ����ö�ٷ� ����\n(��ü, ��ġ����)\n", DT_LEFT);
			m_Grid.AddCell(row, col+2, "  �ּ�ö������ 13mm�̻�", DT_LEFT);
			m_Grid.AddCell(row, col+3, "  �ּ�ö������ 13mm�̻�", DT_LEFT);
			m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarArchRibMinDia1,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
			m_Grid.AddCell(row++, col+5, pBri->m_bCheckSuppoerBarArchRibMinDia1 ? "����" : "�������", DT_RIGHT);

			m_Grid.AddCell(row, col, "�ܸ���� ö��");
			m_Grid.AddCell(row, col+1, "  ����ö�ٷ� ����\n(��ü, ��ġ����)\n", DT_LEFT);
			m_Grid.AddCell(row, col+2, "  ��ö�� ������ 1/3�̻�", DT_LEFT);
			m_Grid.AddCell(row, col+3, "  ��ö�� ������ 1/3�̻�", DT_LEFT);
			m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarArchRibMinDia2,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
			m_Grid.AddCell(row++, col+5, pBri->m_bCheckSuppoerBarArchRibMinDia2 ? "����" : "�������", DT_RIGHT);
		}
		//////////////////////////////////////////////////////////////////////////
		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ����ö�ٷ� ����\n(����)\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  ��ö�ٺ�", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  ��ö���� 20%", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarFootMainOver20,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bCheckSuppoerBarFootMainOver20 ? "����" : "�������", DT_RIGHT);

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ����ö�ٷ� ����\n(����)\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  ����� 300 mm���̻�", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  ����� 300 mm���̻�", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarFoot300,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bCheckSuppoerBarFoot300 ? "����" : "�������", DT_RIGHT);

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ����ö�ٷ� ����\n(����)\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  0.0015hs", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  0.0015hs", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarFoot15hs,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5,_T("h:����β�,s:ö�ٰ���"), DT_RIGHT);

		m_nMergeRowSupportBarFoot = row;
		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ����ö�ٷ� ����\n(����)\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  �ܸ�����\n", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  fy�� 400MPa ������ ����ö��", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarFootArea,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5,&pBri->m_dCheckSuppoerBarFootArea, DT_RIGHT,CELL_LINE_NO);m_Grid.SetCellDec(CELL_DEC_4);

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ����ö�ٷ� ����\n(����)\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  �ܸ�����\n", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  fy�� 400MPa �ʰ��ϴ� ����ö��", DT_LEFT);
		m_Grid.AddCell(row, col+4, "");
		m_Grid.AddCell(row++, col+5, "0.002 x 400/fy", DT_RIGHT, CELL_READONLY);

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ����ö�ٷ� ����\n(������)\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  ����� 300 mm���̻�", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  ����� 300 mm���̻�", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarWing300,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bCheckSuppoerBarWing300 ? "����" : "�������", DT_RIGHT);

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ����ö�ٷ� ����\n(������)\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  0.0015hs", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  0.0015hs", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarWing15hs,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5,_T("h:����β�,s:ö�ٰ���"), DT_RIGHT);

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ����ö�ٷ� ����\n(������)\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  �ܸ�����", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  �ܸ�����", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckSuppoerBarWingArea,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5,&pBri->m_dCheckSuppoerBarWingArea, DT_RIGHT,CELL_LINE_NO);m_Grid.SetCellDec(CELL_DEC_4);

		sArr.RemoveAll();
		sArr.Add(hggettext("��ü(���)����"));
		sArr.Add(hggettext("��ü(���)���ܿ��� d��ġ"));
		sArr.Add(hggettext("��ü(���)���ܿ��� d/2��ġ"));
		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ���ܰ���\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  ������ ���ܰ���� ����ܸ� ��ġ", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  ������ ���ܰ���� ����ܸ� ��ġ", DT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nApplyCalcShearDistSlab, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nApplyCalcShearDistSlab), DT_RIGHT);

		sArr.RemoveAll();
		sArr.Add(hggettext("������ ����"));
		sArr.Add(hggettext("������ ���ܿ��� d��ġ"));
		sArr.Add(hggettext("������ ���ܿ��� d/2��ġ"));
		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ���ܰ���\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  ��ü ���ܰ���� ����ܸ� ��ġ", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  ��ü ���ܰ���� ����ܸ� ��ġ", DT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nApplyCalcShearDistWall, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nApplyCalcShearDistWall), DT_RIGHT);

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ���ܰ���\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  ���� �߾Ӻ� ���� ����", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  ���� �߾Ӻ� ���� ����", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCalcShearCenter,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bCalcShearCenter ? "����" : "�������", DT_RIGHT);
	}
	else
	{
		sArr.RemoveAll();
		sArr.Add(hggettext("��ü(���)����"));
		sArr.Add(hggettext("��ü(���)���ܿ��� d��ġ"));
		sArr.Add(hggettext("��ü(���)���ܿ��� d/2��ġ"));
		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ���ܰ���\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  ������ ���ܰ���� ����ܸ� ��ġ", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  ������ ���ܰ���� ����ܸ� ��ġ", DT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nApplyCalcShearDistSlab, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nApplyCalcShearDistSlab), DT_RIGHT);

		sArr.RemoveAll();
		sArr.Add(hggettext("������ ����"));
		sArr.Add(hggettext("������ ���ܿ��� d��ġ"));
		sArr.Add(hggettext("������ ���ܿ��� d/2��ġ"));
		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ���ܰ���\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  ��ü ���ܰ���� ����ܸ� ��ġ", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  ��ü ���ܰ���� ����ܸ� ��ġ", DT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nApplyCalcShearDistWall, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nApplyCalcShearDistWall), DT_RIGHT);

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ���ܰ���\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  ���� �߾Ӻ� ���� ����", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  ���� �߾Ӻ� ���� ����", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCalcShearCenter,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bCalcShearCenter ? "����" : "�������", DT_RIGHT);
	}

	m_Grid.AddCell(row, col, "�ܸ���� ö��");
	m_Grid.AddCell(row, col+1, "  �ּ����� ����\n\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  ������", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  ������", DT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bApplyMinAvSlab,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bApplyMinAvSlab ? "����" : "�������", DT_RIGHT);

	m_Grid.AddCell(row, col, "�ܸ���� ö��");
	m_Grid.AddCell(row, col+1, "  �ּ����� ����\n\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  ��ü", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  ��ü", DT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bApplyMinAvWall,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bApplyMinAvWall ? "����" : "�������", DT_RIGHT);

	m_Grid.AddCell(row, col, "�ܸ���� ö��");
	m_Grid.AddCell(row, col+1, "  �ּ����� ����\n\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  ����", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  ����", DT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bApplyMinAvFoot,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bApplyMinAvFoot ? "����" : "�������", DT_RIGHT);

	m_Grid.AddCell(row, col, "�ܸ���� ö��");
	m_Grid.AddCell(row, col+1, "  �ּ����� ����\n\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  ������", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  ������", DT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bApplyMinAvWing,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bApplyMinAvWing ? "����" : "�������", DT_RIGHT);

	m_Grid.AddCell(row, col, "�ܸ���� ö��");
	m_Grid.AddCell(row, col+1, "  �ּ����� ����\n\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  ���ӽ�����", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  ���ӽ�����", DT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bApplyMinAvAps,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bApplyMinAvAps ? "����" : "�������", DT_RIGHT);

	m_Grid.AddCell(row, col, "�ܸ���� ö��");
	m_Grid.AddCell(row, col+1, "  �ּ����� ����\n\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  ����Ÿ������", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  ����Ÿ������", DT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bApplyMinAvHyunTa,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bApplyMinAvHyunTa ? "����" : "�������", DT_RIGHT);

	//
	m_Grid.AddCell(row, col, "�ܸ���� ö��");
	m_Grid.AddCell(row, col+1, "  ������\n");
	m_Grid.AddCell(row, col+2, "  ���ܰ���� ö�ٰ��ݿ� ���� ���� ����", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  ���ܰ���� ö�ٰ��ݿ� ���� ���� ����", DT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bCheckMinCtcShearWing, nFormat, bLsd ? CELL_READONLY : CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bCheckMinCtcShearWing ? "����" : "�������", DT_RIGHT);

	m_Grid.AddCell(row, col, "�ܸ���� ö��");
	m_Grid.AddCell(row, col+1, "  ������\n");
	m_Grid.AddCell(row, col+2, "  D���� ö�ٷ� ������ T1 �β� ����", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  D���� ö�ٷ� ������ T1 �β� ����", DT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bApplyT1CalcWingWall, nFormat, CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bApplyT1CalcWingWall ? "����" : "�������", DT_RIGHT);

	sArr.RemoveAll();
	sArr.Add(hggettext("��ü�����"));
	sArr.Add(hggettext("��������"));
	m_Grid.AddCell(row, col, "�ܸ���� ö��");
	m_Grid.AddCell(row, col+1, "  ������\n");
	m_Grid.AddCell(row, col+2, hggettext("  ������ؼ��� ������ġ"), GVHT_LEFT);
	m_Grid.AddCell(row, col+3, hggettext("  ������ؼ��� ������ġ"), GVHT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_nApplyWingWallHeightBasePos, DT_RIGHT, TRUE, CELL_LINE_NO, _T(""), CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nApplyWingWallHeightBasePos), DT_RIGHT);
	//
	m_Grid.AddCell(row, col, "�ܸ���� ö��");
	m_Grid.AddCell(row, col+1, "  ���ӽ�����\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  ���ܰ���� ö�ٰ��ݿ� ���� ���� ����", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  ���ܰ���� ö�ٰ��ݿ� ���� ���� ����", DT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bCheckMinCtcShearAps, nFormat, bLsd? CELL_READONLY : CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bCheckMinCtcShearAps ? "����" : "�������", DT_RIGHT);

	sArr.RemoveAll();
	sArr.Add(hggettext("������ġ"));
	sArr.Add(hggettext("�������� d��ġ"));
	sArr.Add(hggettext("�������� d/2��ġ"));
	m_Grid.AddCell(row, col, "�ܸ���� ö��");
	m_Grid.AddCell(row, col+1, "  ���ӽ�����\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  ���ܰ���� ���߻�����ġ", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  ���ܰ���� ���߻�����ġ", DT_LEFT);
	m_Grid.AddCell(row,  col+4, &pBri->m_nSelectApsShearCheck, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectApsShearCheck), DT_RIGHT);

	if (bLsd == FALSE)
	{
		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ���� ���ܰ���\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  �ձ� ����� ����ܸ� ��ġ", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  �ձ� ����� ����ܸ� ��ġ", DT_LEFT);
		sArr.RemoveAll();
		sArr.Add("��ü(���)����");
		sArr.Add("��ü(���)���鿡�� d��ġ");
		sArr.Add("��ü(���)���鿡�� d/2��ġ");
		m_Grid.AddCell(row,  col+4, &pBri->m_nApplyCalcShearDistFront, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nApplyCalcShearDistFront), DT_RIGHT);

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ���� ���ܰ���\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  �ޱ� ����� ����ܸ� ��ġ", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  �ޱ� ����� ����ܸ� ��ġ", DT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nApplyCalcShearDistBack, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nApplyCalcShearDistBack), DT_RIGHT);

		sArr.RemoveAll(); 
		long i=0; for(i=0; i<MAX_CRACK_ENV_TYPE; i++) sArr.Add(pStd->GetStringCrackDesign(i));
		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  �νĿ�����\n ȯ������\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  ������", DT_LEFT); 
		m_Grid.AddCell(row, col+3, "  ������", DT_LEFT); 
		m_Grid.AddCell(row,  col+4, &pBri->m_nTypeCrackDesignSlab, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_AUTO, sArr, &m_DlgCrackEnv);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypeCrackDesignSlab), DT_RIGHT);

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  �νĿ�����\n ȯ������\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  ��ü", DT_LEFT); 
		m_Grid.AddCell(row, col+3, "  ��ü", DT_LEFT); 
		m_Grid.AddCell(row,  col+4, &pBri->m_nTypeCrackDesignWall, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_AUTO, sArr, &m_DlgCrackEnv);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypeCrackDesignWall), DT_RIGHT);

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  �νĿ�����\n ȯ������\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  ����", DT_LEFT); 
		m_Grid.AddCell(row, col+3, "  ����", DT_LEFT); 
		m_Grid.AddCell(row,  col+4, &pBri->m_nTypeCrackDesignFoot, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_AUTO, sArr, &m_DlgCrackEnv);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypeCrackDesignFoot), DT_RIGHT);

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  �νĿ�����\n ȯ������\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  ������", DT_LEFT); 
		m_Grid.AddCell(row, col+3, "  ������", DT_LEFT); 
		m_Grid.AddCell(row,  col+4, &pBri->m_nTypeCrackDesignWing, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_AUTO, sArr, &m_DlgCrackEnv);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypeCrackDesignWing), DT_RIGHT);

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  �νĿ�����\n ȯ������\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  ���ӽ�����", DT_LEFT); 
		m_Grid.AddCell(row, col+3, "  ���ӽ�����", DT_LEFT); 
		m_Grid.AddCell(row,  col+4, &pBri->m_nTypeCrackDesignAps, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_AUTO, sArr, &m_DlgCrackEnv);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypeCrackDesignAps), DT_RIGHT);
		if(pStd->m_nTypeKciCode != DESIGN_CODE_KCI_2003)
		{
			m_Grid.AddCell(row, col, "�ܸ���� ö��");
			m_Grid.AddCell(row, col+1, "  ö�ٰ��ݰ���� 1��ö�� �Ǻ���ġ�� fs����", DT_LEFT);
			m_Grid.AddCell(row, col+2, "  ö�ٰ��ݰ���� 1��ö�� �Ǻ���ġ�� fs����", DT_LEFT);
			m_Grid.AddCell(row, col+3, "  ö�ٰ��ݰ���� 1��ö�� �Ǻ���ġ�� fs����", DT_LEFT);
			sArr.RemoveAll();
			sArr.Add("����");
			sArr.Add("�������");
			m_Grid.AddCell(row,  col+4, &pBri->m_nSelectApplyFs, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
			m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectApplyFs), DT_RIGHT);
		}

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ���ʵβ� ���Ѱ� ���� (��.��.�� p85)", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  ���ʵβ� ���Ѱ� ���� (��.��.�� p85)", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  ���ʵβ� ���Ѱ� ���� (��.��.�� p85)", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bApplyFootingMinThick,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bApplyFootingMinThick ? "����" : "�������", DT_RIGHT);
	}
	else
	{
		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ���� ���ܰ���\n\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  �ձ� ����� ����ܸ� ��ġ(��������)", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  �ձ� ����� ����ܸ� ��ġ(��������)", DT_LEFT);
		
		sArr.RemoveAll();
		sArr.Add("��ü(���)����");
		sArr.Add("��ü(���)���鿡�� d��ġ");
		sArr.Add("��ü(���)���鿡�� d/2��ġ");
		m_Grid.AddCell(row,  col+4, &pBri->m_nApplyCalcShearDistFront, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nApplyCalcShearDistFront), DT_RIGHT);

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ���� ���ܰ���\n\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  �ޱ� ����� ����ܸ� ��ġ(��������)", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  �ޱ� ����� ����ܸ� ��ġ(��������)", DT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nApplyCalcShearDistBack, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nApplyCalcShearDistBack), DT_RIGHT);

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ���� ���ܰ���\n\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  �ձ� ����� ����ܸ� ��ġ(���ұ���)", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  �ձ� ����� ����ܸ� ��ġ(���ұ���)", DT_LEFT);

		sArr.RemoveAll();
		sArr.Add("��ü(���)����");
		sArr.Add("��ü(���)���鿡�� ���尡��� ����(ȯ��A)");
		sArr.Add("��ü(���)���鿡�� ���尡��� ����(0.1D)");
		m_Grid.AddCell(row,  col+4, &pBri->m_nApplyCalcShearDistFrontPile, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nApplyCalcShearDistFrontPile), DT_RIGHT);

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ���� ���ܰ���\n\n", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  �ޱ� ����� ����ܸ� ��ġ(���ұ���)", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  �ޱ� ����� ����ܸ� ��ġ(���ұ���)", DT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nApplyCalcShearDistBackPile, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nApplyCalcShearDistBackPile), DT_RIGHT);

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ���ܰ���� �� : 0 = �ڵ����", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  ���ܰ���� �� : 0 = �ڵ����", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  ���ܰ���� �� : 0 = �ڵ����", GVHT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_dThetaShearDesign, DT_RIGHT,0);
		m_Grid.AddCell(row++, col+5, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dThetaShearDesign), DT_RIGHT,CELL_LINE_NO);

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ���ܷ¿� ���� ������ �߰������ ����", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  ���ܷ¿� ���� ������ �߰������ ����", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  ���ܷ¿� ���� ������ �߰������ ����", GVHT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckDeltaTR,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bCheckDeltaTR ? "����" : "�������", DT_RIGHT);

		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ��뼺 ����� ��ȿź������� : 0 = ź�������", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  ��뼺 ����� ��ȿź������� : 0 = ź�������", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  ��뼺 ����� ��ȿź������� : 0 = ź�������", GVHT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_dElasticModularRatioUseCheck, DT_RIGHT,0);
		m_Grid.AddCell(row++, col+5, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dElasticModularRatioUseCheck), DT_RIGHT,CELL_LINE_NO);

		sArr.RemoveAll();
		sArr.Add("�տ��� ����");
		sArr.Add("�����տ� ����");
		sArr.Add("�տ���, �����տ� ����");
		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ��뼺 ����� �տ�����", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  ��뼺 ����� �տ�����", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  ��뼺 ����� �տ�����", GVHT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nSelectUsebilityCheck, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectUsebilityCheck), DT_RIGHT);

		sArr.RemoveAll();
		if(pStd->IsDesignKDS2019())
		{
			sArr.Add(hggettext("KDS 24 14 21 4.2.3.2"));
			pBri->m_nTypeCrackWidthCondition = 0;
		}
		else
		{
			sArr.Add(hggettext("����2015 5.8.3.4"));
			sArr.Add(hggettext("����2012 5.6.3.4"));
		}
		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  �տ������", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  �տ������", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  �տ������", GVHT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nTypeCrackWidthCondition, DT_RIGHT, TRUE, pStd->IsDesignKDS2019() ? CELL_READONLY : CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypeCrackWidthCondition), DT_RIGHT);

		sArr.RemoveAll();
		sArr.Add("��ö��/2 + ����ö��");
		sArr.Add("��ö��/2");
		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  �����Ǻ� ����", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  �����Ǻ� ����", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  �����Ǻ� ����", GVHT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nSelectExposureCoverCheck, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectExposureCoverCheck), DT_RIGHT);

		sArr.RemoveAll();
		sArr.Add("0.135fck/fy ");
		sArr.Add("0.1Nu(fyd x Ag) ");
		m_Grid.AddCell(row, col, "�ܸ���� ö��");
		m_Grid.AddCell(row, col+1, "  ��� �ּ� ö�ٺ�", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  ��� �ּ� ö�ٺ�", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  ��� �ּ� ö�ٺ�", GVHT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nSelectPminColumn, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectPminColumn), DT_RIGHT);
	}
	
	m_Grid.AddCell(row, col, "�ܸ���� ö��");
	m_Grid.AddCell(row, col+1, "  �ܸ����� ��ġ���� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  �ܸ����� ��ġ���� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  �ܸ����� ��ġ���� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bSelectHunchDesignCheck,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bSelectHunchDesignCheck ? "����" : "�������", DT_RIGHT);

	if (bLsd == FALSE)
	{
		m_Grid.AddCell(row, col, "�������");
		m_Grid.AddCell(row, col+1, "  ������ ����\n  ���߰��", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  �������", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  �������", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_dFactorToap, DT_RIGHT,0);m_Grid.SetCellDec(CELL_DEC_2);
		m_Grid.AddCell(row++, col+5, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dFactorToap), DT_RIGHT,0);m_Grid.SetCellDec(CELL_DEC_2);

		m_Grid.AddCell(row, col, "�������");
		m_Grid.AddCell(row, col+1, "  ������ ����\n  ���߰��", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  ��������", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  ��������", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_dFactorLive, DT_RIGHT,0);m_Grid.SetCellDec(CELL_DEC_2);
		m_Grid.AddCell(row++, col+5, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pBri->m_dFactorLive), DT_RIGHT,0);m_Grid.SetCellDec(CELL_DEC_2);
	}

	if(pBri->IsBoxType() == FALSE)
	{
		long nBaseIndex(1);
		// ��а�� ������ �����������
		m_Grid.AddCell(row, col, "�������");
		m_Grid.AddCell(row, col+1, "  ��а�� ������\n �����������", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  ���� ��ü", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  ���� ��ü", GVHT_LEFT);
		sArr.RemoveAll();
//		sArr.Add("�ڵ�");		(ARCBRIDGE-3535) �ڵ��� ���ÿ��� ����
		if(pStd->m_nTypeEarthQuakeApply == 0)
		{
			sArr.Add("0.5A");
			sArr.Add("1.0A");
			sArr.Add("1.5A");
			sArr.Add("0.5A X S");
			sArr.Add("1.0A X S");
			sArr.Add("1.5A X S");
		}
		else
		{
			sArr.Add("0.5S");
			sArr.Add("1.0S");
			sArr.Add("1.5S");
			sArr.Add("0.5S X ���ݺ������");
			sArr.Add("1.0S X ���ݺ������");
			sArr.Add("1.5S X ���ݺ������");
		}
		m_Grid.AddCell(row,  col+4, &pBri->m_nSelFactorKhStt, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);  m_Grid.SetCellBaseOfIndex(nBaseIndex);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelFactorKhStt - nBaseIndex), DT_RIGHT);

		m_Grid.AddCell(row, col, "�������");
		m_Grid.AddCell(row, col+1, "  ��а�� ������\n �����������", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  ���� ��ü", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  ���� ��ü", GVHT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nSelFactorKhEnd, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);	  m_Grid.SetCellBaseOfIndex(nBaseIndex);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelFactorKhEnd - nBaseIndex), DT_RIGHT);
	}

	sArr.RemoveAll();
	sArr.Add("ǥ�� ������ 3.0 m���� ����");
	sArr.Add("W = Wc / N ���� ���");
	m_Grid.AddCell(row, col, "�������");
	m_Grid.AddCell(row, col+1, "Ȱ����\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  ������ ���� ������(W)", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  ������ ���� ������(W)", GVHT_LEFT);
	m_Grid.AddCell(row,  col+4, &pBri->m_nSelectDesignRoadWidth, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectDesignRoadWidth), DT_RIGHT);

	m_Grid.AddCell(row, col, "�������");
	m_Grid.AddCell(row, col+1, "Ȱ����\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  ������ �������� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  ������ �������� ����", GVHT_LEFT);
	sArr.RemoveAll();
	sArr.Add("�������� ���� ����� ���� ����");
	sArr.Add("������ ����");
	sArr.Add("�������� ����");
	m_Grid.AddCell(row,  col+4, &pBri->m_nSelectLiveLoadSpanLength, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectLiveLoadSpanLength), DT_RIGHT);

	sArr.RemoveAll();
	sArr.Add("���� ���� ����");
	sArr.Add("���� ���� ����");
	m_Grid.AddCell(row, col, "�������");
	m_Grid.AddCell(row, col+1, "Ȱ����\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  ���Ͻ��� ���� ���ϱ���", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  ���Ͻ��� ���� ���ϱ���", GVHT_LEFT);
	m_Grid.AddCell(row,  col+4, &pBri->m_nSelectStartLane, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectStartLane), DT_RIGHT);

	m_Grid.AddCell(row, col, "�������");
	m_Grid.AddCell(row, col+1, "Ȱ����\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  ���� ��ݰ�� ������Է�(i)", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  ���� ��ݰ�� ������Է�(i)", GVHT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bCheckUseri,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5,&pBri->m_dCheckUseriValue, DT_RIGHT,CELL_LINE_NO);m_Grid.SetCellDec(CELL_DEC_4);

	m_Grid.AddCell(row, col, "�������");
	m_Grid.AddCell(row, col+1, "  �����ۿ�� ���� ���ʰ����� ����", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  �����ۿ�� ���� ���ʰ����� ����", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  �����ۿ�� ���� ���ʰ����� ����", DT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bApplyFootingGForce,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bApplyFootingGForce ? "����" : "�������", DT_RIGHT);
	//
	m_Grid.AddCell(row, col, "�������");
	m_Grid.AddCell(row, col+1, "  ���� ������ ����� MASS �ۿ�� ����", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  ���� ������ ����� MASS �ۿ�� ����", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  ���� ������ ����� MASS �ۿ�� ����", DT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bApplyMassCalcSafe,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bApplyMassCalcSafe ? "����" : "�������", DT_RIGHT);

	m_Grid.AddCell(row, col, "�������");
	m_Grid.AddCell(row, col+1, "  ������ �ܸ����� ����� ��ġ", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  ������ �ܸ����� ����� ��ġ", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  ������ �ܸ����� ����� ��ġ", GVHT_LEFT);
	sArr.RemoveAll();
	
	if(pBri->m_nSelectRigidModeling != 0)
	{
		sArr.Add("���������� �������� ����");
		sArr.Add("����ܸ��� �������� ����");

		m_Grid.AddCell(row,  col+4, &pBri->m_nSelectForceCheckPos, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectForceCheckPos), DT_RIGHT);
	}
	else
	{
		// (ARCBRIDGE-2368) �������� �������� ����ܸ����� �����ʰ� �ִ����� �����Ҽ� �ֵ���.
		sArr.Add("�ִ����� ��ġ���� ����");
		sArr.Add("����ܸ��� �������� ����");

		m_Grid.AddCell(row,  col+4, &pBri->m_nSelectForceCheckPos, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectForceCheckPos), DT_RIGHT);
	}

	sArr.RemoveAll();
	sArr.Add("CASE���� ����");
	sArr.Add("��ȣ���� ����");
	sArr.Add("�ִ��ۿ������(���Ʈ�ִ�-����ּ�)"); // ū������ ���迡�� ��Ī�� ����
	sArr.Add("�ִ��ۿ������(���Ʈ�ִ�-����ִ�)");
	m_Grid.AddCell(row, col, "�������");
	m_Grid.AddCell(row, col+1, "  ���� ����� �ݷ� ���� ���", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  ���� ����� �ݷ� ���� ���", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  ���� ����� �ݷ� ���� ���", GVHT_LEFT);
	m_Grid.AddCell(row,  col+4, &pBri->m_nSelectFootingReactionForce, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectFootingReactionForce), DT_RIGHT);

	m_Grid.AddCell(row, col, "�������");
	if (bLsd)
	{
		m_Grid.AddCell(row, col+1, "  ��米��(Type2)�� �ձ�����(������ü����)", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  ��米��(Type2)�� �ձ�����(������ü����)", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  ��米��(Type2)�� �ձ�����(������ü����)", GVHT_LEFT);
	}
	else
	{
		m_Grid.AddCell(row, col+1, "  ��米��(Type2), ��ġ����౳�� �ձ�����(������ü����)", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  ��米��(Type2), ��ġ����౳�� �ձ�����(������ü����)", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  ��米��(Type2), ��ġ����౳�� �ձ�����(������ü����)", GVHT_LEFT);
	}
	
	m_Grid.AddCell(row, col+4, &pBri->m_bCheckFrontFootTrap2,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bCheckFrontFootTrap2 ? "����" : "�������", DT_RIGHT);

	BOOL bApplyType = (pBri->m_nTypeBridge==BRIDGE_TYPE_TRAP2 || pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH)? TRUE : FALSE;
	m_Grid.AddCell(row, col, "�������");
	if (bLsd)
	{
		m_Grid.AddCell(row, col+1, "  ��米��(TYPE-2) ���ʺε�ħ�� ����", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  ��米��(TYPE-2) ���ʺε�ħ�� ����", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  ��米��(TYPE-2) ���ʺε�ħ�� ����", GVHT_LEFT);
	}
	else
	{
		m_Grid.AddCell(row, col+1, "  ��米��(TYPE-2), ��ġ�� ��౳ ���ʺε�ħ�� ����", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  ��米��(TYPE-2), ��ġ�� ��౳ ���ʺε�ħ�� ����", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  ��米��(TYPE-2), ��ġ�� ��౳ ���ʺε�ħ�� ����", GVHT_LEFT);
	}
	
	sArr.RemoveAll();
	sArr.Add("������ ��ü���� �ε�ħ�� ����");
	sArr.Add("������ü�� �ϳ��� �ε�ħ�Ϸ� ����");
	m_Grid.AddCell(row,  col+4, &pBri->m_nSelSubsideType, DT_RIGHT, TRUE, bApplyType? CELL_LINE_NO : CELL_READONLY, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelSubsideType), DT_RIGHT);

	if (bLsd == FALSE)
	{
		//
		m_Grid.AddCell(row, col, "�������");
		m_Grid.AddCell(row, col+1, "  ������ ����ö�� ����� �������� ���� ���� ����", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  ������ ����ö�� ����� �������� ���� ���� ����", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  ������ ����ö�� ����� �������� ���� ���� ����", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bCheckCornerInside,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bCheckCornerInside ? "����" : "�������", DT_RIGHT);

		// �������� ���� ���ÿɼ� �߰�... 2005-11-24 9:32
		m_Grid.AddCell(row, col, "�������");
		m_Grid.AddCell(row, col+1, "  ������ ����ö�� ������ �������� ft ����", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  ������ ����ö�� ������ �������� ft ����", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  ������ ����ö�� ������ �������� ft ����", DT_LEFT);
		sArr.RemoveAll();
		sArr.Add(pStd->IsSI()? "0.08��fck" : "0.25��fck");
		sArr.Add(pStd->IsSI()? "1/3��fck" : "1.06��fck");
		sArr.Add(pStd->IsSI()? "0.13��fck" : "0.42��fck");
		sArr.Add(pStd->IsSI()? "0.22��fck" : "0.7��fck");
		m_Grid.AddCell(row,  col+4, &pBri->m_nSelCalcCornerFt, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelCalcCornerFt), DT_RIGHT);
	}

	sArr.RemoveAll();
	//	sArr.Add("������� ���� : �ִ� ���Ʈ");		// 27258 
	//	sArr.Add("��ü�� ���� : �ִ���Ʈ,�ִ����");
	// (ARCBRIDGE-3616) ����������� �ɼǸ� ����. LSD/��������� ���о��� LSD�� ���� ����
//	if (bLsd)
	{
		sArr.Add("�ں���� ����");
		sArr.Add("�ھ������� ����");
	}
// 	else
// 	{
// 		sArr.Add("������� ����");
// 		sArr.Add("��ü�� ����");
	// 	}

	m_Grid.AddCell(row, col, "�������");
	m_Grid.AddCell(row, col+1, "  ��.������ ��ü ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  ��.������ ��ü ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  ��.������ ��ü ����", GVHT_LEFT);
	m_Grid.AddCell(row,  col+4, &pBri->m_nSelectWallSectionDesign, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectWallSectionDesign), DT_RIGHT);

	if(pBri->m_nSelectWallSectionDesign == 1)
	{
		m_Grid.AddCell(row, col, "�������");
		m_Grid.AddCell(row, col+1, "  ��,������ ��ü �ھ������ ��뼺����", DT_LEFT);
		m_Grid.AddCell(row, col+2, "  ��,������ ��ü �ھ������ ��뼺����", DT_LEFT);
		m_Grid.AddCell(row, col+3, "  ��,������ ��ü �ھ������ ��뼺����", DT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bOutWallUsebilityCheck,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bOutWallUsebilityCheck ? "����" : "�������", DT_RIGHT);
	}

	m_Grid.AddCell(row, col, "�������");
	m_Grid.AddCell(row, col+1, "  �߰������� ��ü ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  �߰������� ��ü ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  �߰������� ��ü ����", GVHT_LEFT);
	m_Grid.AddCell(row,  col+4, &pBri->m_nSelectInWallSectionDesign, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectInWallSectionDesign), DT_RIGHT);

	if(bLsd)
	{
		sArr.RemoveAll();
		sArr.Add(hggettext("�ٴ���(2-135kN) "));
		sArr.Add(hggettext("�ķ�(192kN)"));
		m_Grid.AddCell(row, col, "�������");
		m_Grid.AddCell(row, col+1, hggettext("  ���ӽ����� Ȱ���߻����� ���� ����"), GVHT_LEFT);
		m_Grid.AddCell(row, col+2, hggettext("  ���ӽ����� Ȱ���߻����� ���� ����"), GVHT_LEFT);
		m_Grid.AddCell(row, col+3, hggettext("  ���ӽ����� Ȱ���߻����� ���� ����"), GVHT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nSelectApsAxleWeight, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectApsAxleWeight), DT_RIGHT);

		sArr.RemoveAll();
		sArr.Add(hggettext("Dc"));
		sArr.Add(hggettext("Dc/Dw"));
		sArr.Add(hggettext("Dc/Dw/Ev"));
		m_Grid.AddCell(row, col, "�������");
		m_Grid.AddCell(row, col+1, hggettext("  ���ӽ����� ��������"), GVHT_LEFT);
		m_Grid.AddCell(row, col+2, hggettext("  ���ӽ����� ��������"), GVHT_LEFT);
		m_Grid.AddCell(row, col+3, hggettext("  ���ӽ����� ��������"), GVHT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nSelectApsDeadLoad, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectApsDeadLoad), DT_RIGHT);
	}

	//////////////////////////////////////////////////////////////////////////
	sArr.RemoveAll();
	sArr.Add("�������� ����");
	sArr.Add("���Ѱ��� ����");
	sArr.Add("EndOffset ����");

	m_Grid.AddCell(row, col, "�𵨸�");
	m_Grid.AddCell(row, col+1, "  �𵨸� �ؼ�\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  �������� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  �������� ����", GVHT_LEFT);
	m_Grid.AddCell(row,  col+4, &pBri->m_nSelectRigidModeling, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectRigidModeling), DT_RIGHT);

	m_Grid.AddCell(row, col, "�𵨸�");
	m_Grid.AddCell(row, col+1, "  �𵨸� �ؼ�\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  ��ü ���� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  ��ü ���� ����", GVHT_LEFT);
	sArr.RemoveAll();
	sArr.Add("���� ���� �߽���ġ ����");
	sArr.Add("Ⱦ�ܸ� �ִ� ����");
	m_Grid.AddCell(row,  col+4, &pBri->m_nSelectModelingHeight, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectModelingHeight), DT_RIGHT);

	if (bLsd == FALSE)
	{
		m_Grid.AddCell(row, col, "�𵨸�");
		m_Grid.AddCell(row, col+1, "  �𵨸� �ؼ�\n\n", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  ������ �µ����� ����", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  ������ �µ����� ����", GVHT_LEFT);
		sArr.RemoveAll();
		sArr.Add("�µ�����(+/-), ���ϸ� �µ��� ����");
		sArr.Add("�µ�����(+/-) ����");
		sArr.Add("���ϸ� �µ��� ����");
		m_Grid.AddCell(row,  col+4, &pBri->m_nSelectTempLoadSlab, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectTempLoadSlab), DT_RIGHT);
	}

	m_Grid.AddCell(row, col, "�𵨸�");
	m_Grid.AddCell(row, col+1, "  �𵨸� �ؼ�\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  �µ����� ��ü�� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  �µ����� ��ü�� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bSelectTempLoadAddWall,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bSelectTempLoadAddWall ? "����" : "�������", DT_RIGHT);

	m_Grid.AddCell(row, col, "�𵨸�");
	m_Grid.AddCell(row, col+1, "  �𵨸� �ؼ�\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  �������� ��ü�� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  �������� ��ü�� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bSelectShrinkLoadAddWall,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bSelectShrinkLoadAddWall ? "����" : "�������", DT_RIGHT);

	if (bLsd == FALSE)
	{
		m_Grid.AddCell(row, col, "�𵨸�");
		m_Grid.AddCell(row, col+1, "  �𵨸� �ؼ�\n\n", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  ENV ���� ���� : �����", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  ENV ���� ���� : �����", GVHT_LEFT);
		m_Grid.AddCell(row, col+4, &pBri->m_bSelectEnvToapOne,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+5, pBri->m_bSelectEnvToapOne ? "����" : "�������", DT_RIGHT);
	}

	m_Grid.AddCell(row, col, "�𵨸�");
	m_Grid.AddCell(row, col+1, "  �𵨸� �ؼ�\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  ENV ���� ���� : ����ħ��", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  ENV ���� ���� : ����ħ��", GVHT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bSelectEnvSubsid,nFormat, (pBri->m_nSelect2DFrameTool == SOLVE_MIDAS || pBri->GetCountJijum() > 5) ? CELL_READONLY : CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bSelectEnvSubsid ? "����" : "�������", DT_RIGHT, (pBri->m_nSelect2DFrameTool == SOLVE_MIDAS || pBri->GetCountJijum() > 5) ? CELL_READONLY : CELL_LINE_NO);

	m_Grid.AddCell(row, col, "�𵨸�");
	m_Grid.AddCell(row, col+1, "  �𵨸� �ؼ�\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  �ܸ�Ư���� ��ġ ���", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  �ܸ�Ư���� ��ġ ���", GVHT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bSelectHunchModeling,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bSelectHunchModeling ? "����" : "�������", DT_RIGHT);
	//////////////////////////////////////////////////////////////////////////

	m_Grid.AddCell(row, col, "�𵨸�");
	m_Grid.AddCell(row, col+1, "  �߰������� �������� ���� ������ ��ġ ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  �߰������� �������� ���� ������ ��ġ ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  �߰������� �������� ���� ������ ��ġ ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bSelectRigidCalcHunch,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bSelectRigidCalcHunch ? "����" : "�������", DT_RIGHT);
	
	m_Grid.AddCell(row, col, "�𵨸�");
	m_Grid.AddCell(row, col+1, "  �������� ������ ���� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  �������� ������ ���� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  �������� ������ ���� ����", GVHT_LEFT);
	sArr.RemoveAll();
	sArr.Add("�ּ� ���� ����");
	sArr.Add("���� ��ü ����");
	sArr.Add("�ִ� ��ü ����");
	m_Grid.AddCell(row,  col+4, &pBri->m_nSelectDesignSpanLength, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectDesignSpanLength), DT_RIGHT);

	m_Grid.AddCell(row, col, "�𵨸�");
	m_Grid.AddCell(row, col+1, "  ��� �𵨸��� ������� ���", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  ��� �𵨸��� ������� ���", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  ��� �𵨸��� ������� ���", GVHT_LEFT);
	sArr.RemoveAll();
	sArr.Add("�ڵ�");
	sArr.Add("������� ���");
	sArr.Add("�������� ���");
	sArr.Add("(���+����)/2 ���");
	m_Grid.AddCell(row,  col+4, &pBri->m_nSelectDesignSpanApply, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectDesignSpanApply), DT_RIGHT);

	m_Grid.AddCell(row, col, "�𵨸�");
	m_Grid.AddCell(row, col+1, "  �������� ������ ��ħ��ġ ���", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  �������� ������ ��ħ��ġ ���", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  �������� ������ ��ħ��ġ ���", GVHT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bSelectSpanLengthApplyShoe,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bSelectSpanLengthApplyShoe ? "����" : "�������", DT_RIGHT);

	if (bLsd == FALSE)
	{
		sArr.RemoveAll();
		sArr.Add("��մܸ�2�� ���Ʈ");
		sArr.Add("������ġ�ܸ�2�� ���Ʈ");
		m_Grid.AddCell(row, col, "�𵨸�");
		m_Grid.AddCell(row, col+1, "  ��ġ���� ����� �ܸ�2�� ���Ʈ�� ����", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, "  ��ġ���� ����� �ܸ�2�� ���Ʈ�� ����", GVHT_LEFT);
		m_Grid.AddCell(row, col+3, "  ��ġ���� ����� �ܸ�2�� ���Ʈ�� ����", GVHT_LEFT);
		m_Grid.AddCell(row,  col+4, &pBri->m_nArchRibSectionMomentPos, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nArchRibSectionMomentPos), DT_RIGHT);
	}

	m_Grid.AddCell(row, col, "����");
	m_Grid.AddCell(row, col+1, "  �������,������ ���� ����", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  �������,������ ���� ����", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  �������,������ ���� ����", DT_LEFT);
	sArr.RemoveAll();
	sArr.Add("��ö��, ��±�");
	sArr.Add("��ö��");
	sArr.Add("�������");
	m_Grid.AddCell(row,  col+4, &pBri->m_nSelectJointUpperRange, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nSelectJointUpperRange), DT_RIGHT);

	// ��ٽ� ���� ǥ�� ���
	BOOL bTrap = pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP || pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 || pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH;
	if(bTrap || (pBri->IsBoxType() && pBri->m_bSlopeLowerSlab == FALSE))
	{
		pBri->m_nTypeJongdanForRebar	= 1;
	}
	m_Grid.AddCell(row, col, "����");
	m_Grid.AddCell(row, col+1, "  ����ǥ�����\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  ����(������)", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  ����(������)", DT_LEFT);
	sArr.RemoveAll();
	sArr.Add("LEVEL");
	sArr.Add("���");
	m_Grid.AddCell(row,  col+4, &pBri->m_nTypeJongdanForRebar, DT_RIGHT, TRUE, (bTrap || (pBri->IsBoxType() && pBri->m_bSlopeLowerSlab == FALSE)) ? CELL_READONLY : CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypeJongdanForRebar), DT_RIGHT);

	m_Grid.AddCell(row, col, "����");
	m_Grid.AddCell(row, col+1, "  ����ǥ�����\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  ������ü ���鵵(�Ϲݵ�, ������)", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  ������ü ���鵵(�Ϲݵ�, ������)", DT_LEFT);
	sArr.RemoveAll();
	sArr.Add("������ ���� ����");
	sArr.Add("��, ���� ����");
	//sArr.Add("���� Sta. ����");
	m_Grid.AddCell(row,  col+4, &pBri->m_nTypeWallForRebarGen, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypeWallForRebarGen), DT_RIGHT);

	m_Grid.AddCell(row, col, "����");
	m_Grid.AddCell(row, col+1, "  ö�ٹ�ġ �ɼ�\n\n\n\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  ��ö�� ��ġ", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  ��ö�� ��ġ", DT_LEFT);
	sArr.RemoveAll();
	sArr.Add("2CYCLE");
	sArr.Add("4CYCLE");
	m_Grid.AddCell(row,  col+4, &pBri->m_nIdxCountCycleMainRebar, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nIdxCountCycleMainRebar), DT_RIGHT);

	m_Grid.AddCell(row, col, "����");
	m_Grid.AddCell(row, col+1, "  ö�ٹ�ġ �ɼ�\n\n\n\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  ��ö�� ����", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  ��ö�� ����", DT_LEFT);
	sArr.RemoveAll();
	sArr.Add("��ν����� ����");
	sArr.Add("��ü ����");
	m_Grid.AddCell(row, col+4, &pBri->m_nTypeStandardCTCRebarMain, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypeStandardCTCRebarMain), DT_RIGHT);

	m_Grid.AddCell(row, col, "����");
	m_Grid.AddCell(row, col+1, "  ö�ٹ�ġ �ɼ�\n\n\n\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  ������ ��±�", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  ������ ��±�", DT_LEFT);
	sArr.RemoveAll();
	sArr.Add("������������ ������ ��ġ");
	sArr.Add("������������ ������ ��ġ");
	sArr.Add("�纸����ġ(��ü)");
	sArr.Add("�纸����ġ(����)");
	m_Grid.AddCell(row, col+4, &pBri->m_nTypePlacingSupportRebar_UpperSlab, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypePlacingSupportRebar_UpperSlab), DT_RIGHT);

	m_Grid.AddCell(row, col, "����");
	m_Grid.AddCell(row, col+1, "  ö�ٹ�ġ �ɼ�\n\n\n\n", DT_LEFT);
	m_Grid.AddCell(row, col+2, "  ������ ��ö��", DT_LEFT);
	m_Grid.AddCell(row, col+3, "  ������ ��ö��", DT_LEFT);
	sArr.RemoveAll();
	sArr.Add("�������� ��ġ");
	sArr.Add("�纸����ġ(��ü)");
	sArr.Add("�纸����ġ(����)");
	m_Grid.AddCell(row,  col+4, &pBri->m_nTypePlacingMainRebar_UpperSlab, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypePlacingMainRebar_UpperSlab), DT_RIGHT);

	m_Grid.AddCell(row, col, "����");
	m_Grid.AddCell(row, col+1, "  ö�ٹ�ġ �ɼ�\n\n\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  ������ ��ö�� �����ö�� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  ������ ��ö�� �����ö�� ����", GVHT_LEFT);
	sArr.RemoveAll();
	sArr.Add("���� ����");
	sArr.Add("�¿��� ���� ����");
	m_Grid.AddCell(row,  col+4, &pBri->m_nTypeSideRebarMain_UpperSlabSabogang, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypeSideRebarMain_UpperSlabSabogang), DT_RIGHT);

	m_Grid.AddCell(row, col, "����");
	m_Grid.AddCell(row, col+1, "  ö�ٹ�ġ �ɼ�\n\n\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  ���� ��ö��", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  ���� ��ö��", GVHT_LEFT);
	sArr.RemoveAll();
	sArr.Add("�������� ��ġ");
	sArr.Add("�纸����ġ(��ü)");
	sArr.Add("�纸����ġ(����)");
	sArr.Add("���������� ���� ��ġ");
	sArr.Add("���ʿ����� ���� ��ġ");
	m_Grid.AddCell(row,  col+4, &pBri->m_nTypePlacingMainRebar_Footing, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypePlacingMainRebar_Footing), DT_RIGHT);

	m_Grid.AddCell(row, col, "����");
	m_Grid.AddCell(row, col+1, "  ö�ٹ�ġ �ɼ�\n\n\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  ��ü ����ö�� ����� ó��", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  ��ü ����ö�� ����� ó��", GVHT_LEFT);
	sArr.RemoveAll();
	sArr.Add("�ڵ�(D22�̻� �)");
	sArr.Add("�");
	sArr.Add("����");
	m_Grid.AddCell(row,  col+4, &pBri->m_nTypeCurveOfWallVertRebar, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypeCurveOfWallVertRebar), DT_RIGHT);

	m_Grid.AddCell(row, col, "����");
	m_Grid.AddCell(row, col+1, "  ö�ٹ�ġ �ɼ�\n\n\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  �ֿ��� ��ö�� 1Cycle�� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  �ֿ��� ��ö�� 1Cycle�� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bFixOutSideMainRebarBy1Cyle, nFormat, CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bFixOutSideMainRebarBy1Cyle ? "����" : "�������", DT_RIGHT);

	m_Grid.AddCell(row, col, "����");
	m_Grid.AddCell(row, col+1, "  ö�ٹ�ġ �ɼ�\n\n\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  ������ ���� ����ö�� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  ������ ���� ����ö�� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bRemoveShearRebarAtSlabYundan, nFormat, CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bRemoveShearRebarAtSlabYundan ? "����" : "�������", DT_RIGHT);

	m_Grid.AddCell(row, col, "����");
	m_Grid.AddCell(row, col+1, "  ö�ٹ�ġ �ɼ�\n\n\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  ��պ� ��ö�� �������", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  ��պ� ��ö�� �������", GVHT_LEFT);
	sArr.RemoveAll();
	sArr.Add("Ŀ�÷�");
	sArr.Add("������");
	m_Grid.AddCell(row,  col+4, &pBri->m_nTypeCouplerColumnMainRebar, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+5, m_pIn->GetStringArray(sArr, pBri->m_nTypeCouplerColumnMainRebar), DT_RIGHT);

	m_Grid.AddCell(row, col, "����");
	m_Grid.AddCell(row, col+1, "  ö�ٹ�ġ �ɼ�\n\n\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  ������ ��� ���� �� ���� ��������(���,����ö��)", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  ������ ��� ���� �� ���� ��������(���,����ö��)", GVHT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bApplySameSupportRebar, nFormat, CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bApplySameSupportRebar ? "����" : "�������", DT_RIGHT);

	m_Grid.AddCell(row, col, "����");
	m_Grid.AddCell(row, col+1, "  ö�ٹ�ġ �ɼ�\n\n\n\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, "  ������ ��� ���� �� ���� ��������(��ö��)", GVHT_LEFT);
	m_Grid.AddCell(row, col+3, "  ������ ��� ���� �� ���� ��������(��ö��)", GVHT_LEFT);
	m_Grid.AddCell(row, col+4, &pBri->m_bApplySameMainRebar, nFormat, CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+5, pBri->m_bApplySameMainRebar ? "����" : "�������", DT_RIGHT);

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);

	m_Grid.SetColumnWidth(0,100);
	m_Grid.SetColumnWidth(1,120);
	m_Grid.SetColumnWidth(2,80);
	m_Grid.SetColumnWidth(3,200);
	m_Grid.SetColumnWidth(4,60);
	m_Grid.SetColumnWidth(5,200);

	m_Grid.SetRowHeightAll(20);
}

void CInputGenDesignVariable::OnPreInitDialog()
{
	CARcBridgeDataStd *pStd	= m_pIn->m_pARcBridgeDataStd;

	m_nTabCover = -1;
	m_nTabCondition = -1;
	m_nTabEnv = -1;
	m_nTabExposureRating	= -1;		// ������
	m_nTabDesignRating		= -1;		// ������
	m_nTabRebarSettle		= -1;		// ö�� ������
	m_nTabRebarJoint		= -1;		// ö�� ������a
	m_nTabLsdSupportRebar = -1;

	long nTab = 0;
	m_sArrTabName.RemoveAll();

	if(m_bDesignLsd)
	{
		m_sArrTabName.Add(_T("��������"));			m_nTabCondition			 = nTab++;
		m_sArrTabName.Add(_T("�Ǻ�����"));			m_nTabCover				 = nTab++;
		m_sArrTabName.Add(_T("������"));			m_nTabExposureRating	 = nTab++;
		m_sArrTabName.Add(_T("������"));			m_nTabDesignRating		 = nTab++;
		m_sArrTabName.Add(_T("���輱�û���"));		m_nTabEnv				 = nTab++;
		m_sArrTabName.Add(_T("ö�� ����, ���� ���� ���"));	m_nTabRebarFactor= nTab++;
		m_sArrTabName.Add(_T("ö�� ������"));		m_nTabRebarSettle		 = nTab++;
		m_sArrTabName.Add(_T("ö�� ������"));		m_nTabRebarJoint		 = nTab++;
		m_sArrTabName.Add(_T("����ö�ٷ� ����"));	m_nTabLsdSupportRebar	 = nTab++;
	}
	else
	{
		m_sArrTabName.Add(_T("��������"));			m_nTabCondition	 = nTab++;
		m_sArrTabName.Add(_T("���輱�û���"));		m_nTabEnv		 = nTab++;
		m_sArrTabName.Add(_T("�Ǻ�����"));			m_nTabCover		 = nTab++;
	}
	
	InitTabCtrl();
	InitComboCtrl();

	SetResize(IDC_TAB,				    SZ_TOP_LEFT,	SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID,			        SZ_TOP_LEFT,	SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_APPLY_MIRROR2, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_APPLY_SAMEALL, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetResize(IDC_BUTTON_BASEDATA_SAVE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_BASEDATA_LOAD, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_EXAMINE,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	
	SetResize(IDC_CHECK_QUICK_SEARCH,	SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	

	SetHyperHelp(m_pIn->GetManualPath());

	m_Grid.InitGrid(m_pIn->m_pARcBridgeDataStd->m_Fy);
	m_EarhtquakeDlg.SetKDS(pStd->m_nTypeEarthQuakeApply==1);

	SetGridData();

//#ifdef _DEBUG
	m_checkQuickSearch.ShowWindow(SW_SHOW);
//#else
//	m_checkQuickSearch.ShowWindow(SW_HIDE);
//#endif
}

void CInputGenDesignVariable::InitTabCtrl()
{
//	m_Tab.RemoveAllTabs();
	m_Tab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_Tab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	long i = 0; for(i = 0; i < m_sArrTabName.GetSize(); i++)
		m_Tab.InsertItem(i, m_sArrTabName[i]);

	int nTab = m_Tab.GetCurSel();
	m_Tab.SetCurSel(nTab);
}


void CInputGenDesignVariable::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	CInputDialogBase::OnTabSelchange(pNMHDR, pResult);
	SetDataSave();
	return;
}

void CInputGenDesignVariable::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	int nTab = m_Tab.GetCurSel();
	
	SetGridData();
	SetDataInit();
	DrawInputDomyunView(TRUE);
	
	if(nTab == m_nTabEnv)
		m_pIn->SetComment("'*' �׸��� ������ ������ؿ� ���� �ǰ��", FALSE);

	return;
}

BOOL CInputGenDesignVariable::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		UpdateData();
		pMsg->wParam = VK_TAB;
	}


	return CInputDialogBase::PreTranslateMessage(pMsg);
}

// �ܺ� ��Ʈ�ѵ� ����
void CInputGenDesignVariable::InitComboCtrl()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	CString str = _T("");
	long nSize = pStd->GetBridgeSize();
	long i = 0; for(i = 0; i < nSize; i++)
	{
		CRcBridgeApp *pBri = pStd->GetBridge(i);
		str = pStd->GetStringBridgeOrder(i) + _T(" : ") + pBri->m_strBridgeName
			+ _T(" [") + pBri->GetStringBridgeType() + _T("]");
		m_cbBridgeNo.AddString(str);
	}
	m_cbBridgeNo.SetCurSel(m_pIn->m_nCurrentBridge);
}

void CInputGenDesignVariable::OnSelchangeComboBridgeNo() 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	m_pIn->m_nCurrentBridge = m_cbBridgeNo.GetCurSel();
	SetGridData();
	SetDataInit();
	SetPaneTitle();
	m_cbBridgeNo.SetFocus();
}

void CInputGenDesignVariable::DrawInputDomyunView(BOOL bZoomAll)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(m_pIn->GetScaleInputDialog());
	CDomyun Dom(pDom);

	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, m_pIn->m_pARcBridgeDataStd);

#ifdef _DEBUG
//////////////////////////////////////////////////////////////////////////
//	������ Mesh �׽�Ʈ
	CTwinVectorArray tvArrWing, tvArrFix, tvArrInit, tvArrSK;
	CDoubleArray dArr;
	CDPoint xyWingStt(0,0),xyWingEnd(0,0),xyWingFootStt(0,0),xyWingFootEnd(0,0);
	double dDistThick = 0;
	for(long ix = 0; ix < 2; ix++)
	{
		pBri->GetTvArrayWingWallInitialMesh(tvArrWing, tvArrFix, tvArrSK, xyWingStt, xyWingEnd, xyWingFootStt, xyWingFootEnd, dDistThick, ix, TRUE, TRUE);
		m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, ix == 0 ? HCAD_DIML : HCAD_CONC);
		DBStd.DrawTvArray(&Dom, tvArrWing);
	}
	m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DBStd.DrawTvArray(&Dom, tvArrFix);
//////////////////////////////////////////////////////////////////////////
#endif

	

	if(m_Tab.GetCurSel() != m_nTabCover)
	{

			CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, m_pIn->m_pARcBridgeDataStd);

#ifdef _DEBUG
		/* ������ ���� ������ �׽�Ʈ
		pBri->m_nTypeJongdanForRebar	= 2;
		CTwinVectorArray tvArr;
		BOOL bStt	= FALSE;
		BOOL bLeft	= TRUE;

		int se = bStt ? -1 : 1;
		int nLeft = bLeft ? -se : se;	// ������ ��� ���� ���� ������ �¿�� �������� �¿찡 �ݴ���.
		pBri->SyncBridge(nLeft, TRUE);
		pBri->GetTvWingWallFront(bStt, bLeft, 1, tvArr, TRUE, FALSE, TRUE, TRUE);
		CHgBaseDrawStd baseDraw;
		//baseDraw.DrawTvArray(&Dom, tvArr, TRUE, TRUE);

		CTwinVectorArray tvArrResult;
		CDoubleArray dArrAngleSu;
		CRebarPlacing rb;
		rb.SeperatorTwinVectorArea(tvArr, tvArrResult, dArrAngleSu);

		long n	= 0;
		long i = 0; for(i = 0; i < dArrAngleSu.GetSize(); i++)
		{
			CTwinVectorArray tvArrTmp;
			for(long j = 0; j < dArrAngleSu.GetAt(i); j++)
			{
				tvArrTmp.Add(tvArrResult.GetAt(n++));
			}
				
			baseDraw.DrawTvArray(&Dom, tvArrTmp);
		}
		*/
/*
		// ��鵵(���) �׽�Ʈ //////////////
 		pBri->SetSearchIP();
 		if(!pBri->m_bCaledRebarMain_Plane || !pBri->m_bCaledRebarSupport_Plane)
 			pBri->SyncBridgeRebar(FALSE);
 
 		DBStd.m_pDrawPlane->DrawRebar_Plane_UpperSlabStdForTest(&Dom, -1, TRUE, TRUE, TRUE, TRUE, FALSE, 0);
 			
		//DBStd.m_pDrawPlane->DrawRebar_Plane_UpperSlabStd(&Dom, -1, TRUE, FALSE, FALSE, FALSE, FALSE);
*/			
			
		// ��鵵(���) ��±� �׽�Ʈ
		/*
		CTwinVectorArray tvArrPlane;
		m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
		pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1, TRUE);
		CHgBaseDrawStd baseDraw;
		baseDraw.DrawTvArray(&Dom, tvArrPlane);
		DBStd.m_pDrawPlane->DrawRebarSupport_Plane_UpperSlabGirderAndHunch(&Dom, 0, FALSE, 0, FALSE, FALSE, FALSE, 0);
		DBStd.m_pDrawPlane->DrawRebarSupport_Plane_UpperSlabGirderAndHunch(&Dom, 1, FALSE, 0, FALSE, FALSE, FALSE, 0);
		*/
	
		/*
		CTwinVectorArray tvArrPlaneExp;
		pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlaneExp, 0, TRUE, 2, TRUE, TRUE);
		CRebarPlacing rb;
//			rb.GetOffsetTvArray(tvArrPlaneExp, 100, TRUE);
		CHgBaseDrawStd baseDraw;
		baseDraw.DrawTvArray(&Dom, tvArrPlaneExp, m_pIn->IsViewVectorInfo());
			

		pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlaneExp, -1, TRUE);
		m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
		baseDraw.DrawTvArray(&Dom, tvArrPlaneExp, m_pIn->IsViewVectorInfo(), m_pIn->IsViewVectorInfo());
		*/
			
							
		// ��鵵(���) �׽�Ʈ //////////////

		// ��ġ����(���) �׽�Ʈ/////
	//	DBStd.m_pDrawPlane->DrawRebar_Plane_ArchRibStd(&Dom, -1, TRUE, FALSE, FALSE, FALSE);
		//////////////////////////////

		// ��鵵(�Ϲ�) �׽�Ʈ ///////////////
/*
		CHgBaseDrawStd baseDraw;
		CTwinVectorArray tvArrPlane;
		m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
		pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, 0, TRUE, 2);
		baseDraw.DrawTvArray(&Dom, tvArrPlane, DBStd.IsViewVectorInfo(), DBStd.IsViewVectorInfo());

		m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
		pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1, TRUE, 2);
		baseDraw.DrawTvArray(&Dom, tvArrPlane, DBStd.IsViewVectorInfo(), DBStd.IsViewVectorInfo());
		*/
		/*

  		// ������ ���
		if(pBri->IsOutWall(TRUE))
		{
			pBri->GetTvWingWallPlane(TRUE);
			DBStd.m_pDrawWingWall->DrawGenPlane(&Dom, TRUE, TRUE);
			DBStd.m_pDrawWingWall->DimGenAngle(&Dom, TRUE, FALSE);
		}

		if(pBri->IsOutWall(FALSE))
		{
			pBri->GetTvWingWallPlane(FALSE);
			DBStd.m_pDrawWingWall->DrawGenPlane(&Dom, FALSE, TRUE);
			DBStd.m_pDrawWingWall->DimGenAngle(&Dom, FALSE, FALSE);
		}
		*/
		// ��鵵(�Ϲ�) �׽�Ʈ ///////////////

		// Ⱦ�ܸ鵵(���) �׽�Ʈ //////////////
// 			double dSta	= pBri->GetStationSmallestWidthOfUpperSlab();
// 			BOOL bVertDir	= pBri->IsVertDir();
// 			pBri->SyncBridge(0, !bVertDir);
// 			pBri->SyncBridgeRebar(FALSE);
// 			
// 			double dStaLeft = pBri->GetStationOnJijum(0, 0);
// 			double dStaRight = (pBri->GetStationOnJijum(0, 0)+pBri->GetStationOnJijum(1, 0))/2;
// 			dStaLeft	= pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, FALSE);
// 			DBStd.m_pDrawCrossSection->DrawRebar_CrossSectionStd(&Dom, dStaLeft, dStaLeft, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE);

		//DBStd.m_pDrawCrossSection->DrawRebarSupport_CrossSection_UpperSlab(&Dom, dStaLeft, FALSE, 0, FALSE, FALSE, 0, FALSE, TRUE);
		/*
		BOOL bVertDir	= pBri->IsVertDir();
		pBri->SyncBridge(0, !bVertDir);
		pBri->SyncBridgeRebar(FALSE);
		double dSta	= pBri->GetStationOnJijum(0, 0, bVertDir) + pBri->GetLengthJigan(0, 0, bVertDir)/2;
		DBStd.m_pDrawCrossSection->DrawRebar_CrossSectionStd(&Dom, dSta, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE);
		*/


		// Ⱦ�ܸ鵵(���) �׽�Ʈ //////////////

		// ������鵵(���) �׽�Ʈ ///////////
		/*
		BOOL bVertDir	= TRUE;
		pBri->SyncBridge(0, !bVertDir);
		pBri->SyncBridgeRebar(FALSE, -1);
		long nJ	= 0;
		CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(0);
		if(pFooting)
		{
			DBStd.m_pDrawFooting->DrawRebar_PlaneStd(&Dom, pFooting, TRUE, TRUE, TRUE, TRUE, 0);//nJ==0?1:-1);
		}
		*/
		// ������鵵(���) �׽�Ʈ ///////////


		// ��ü //
/*
		BOOL bVertDir	= pBri->IsVertDir();
		pBri->SyncBridge(0, !bVertDir);
		pBri->SyncBridgeRebar(FALSE, 0, TRUE);

		DBStd.m_pDrawCrossSection->DrawRebar_OutWall_CrossSectionStd(&Dom, FALSE, TRUE, FALSE, TRUE, TRUE);
*/
		// ��ü //

		// �߰���ü ///
		/*
		BOOL bVertDir	= pBri->IsVertDir();
		pBri->SyncBridge(0, !bVertDir, FALSE);
		pBri->SyncBridgeRebar(FALSE, bVertDir ? -1 : 1);
		CWallApp *pWall	= pBri->GetInWall(0);
		if(pWall)
		{
			DBStd.m_pDrawCrossSection->DrawRebar_InWall_SideStd(&Dom, 1, FALSE, FALSE, TRUE, TRUE, "��", "��", TRUE);
		}
		*/
		//	
#endif
		
	}
	// �Ǻ��Է½� �Ǻ��Է»𵵿����� ���� ������ �׸�.
	else
	{
		CRcBridgeRebar *pBri	= GetRcBridgeRebarForCover();
		CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, m_pIn->m_pARcBridgeDataStd);
		CCellID cell	= m_Grid.GetFocusCell();
		CString sArea	= m_Grid.GetTextMatrix(cell.row, 0);

		if(sArea.Find("������", 0) != -1)
		{
			BOOL bStt(TRUE), bLeft(TRUE);
			CWingWall* pWing = NULL;//pBri->GetWingWall(bStt, bLeft);
			for(long stt = 0; stt < 2; stt++)
			{
				for(long left = 0; left < 2; left++)
				{
					CWingWall *pWingTmp	= pBri->GetWingWall(stt==iSTT, left==iLEFT);
					if(!pWingTmp) continue;
					if(pWingTmp->m_bExist)
					{
						bStt	= stt == iSTT;
						bLeft	= left == iLEFT;
						pWing	= pWingTmp;
						break;
					}
				}
				if(pWing) break;
			}

			if(!pWing) return;

			long nOut	= 1;
			CDRect rcFront(0, 0, 0, 0);
			int se = bStt ? -1 : 1;
			int nLeft = bLeft ? -1*se : 1*se;	// ������ ��� ���� ���� ������ �¿�� �������� �¿찡 �ݴ���.
			CDPoint vAng = pBri->GetAngleWingWallBetweenWall(bStt, bLeft);
			double dT1 = pWing->m_dT1;
			double dT2 = pWing->m_dT2;
			double dSpareT = nOut==0 ? max(dT1, dT2)/vAng.y*vAng.x : 0.0;
			double dSpare1 = 1000.0;
			double dSpare2 = 2000.0;
			double lr = bLeft ? -1.0 : 1.0;
			//pBri->SyncBridge(nLeft, TRUE);
			pBri->GetTvVertSection_Std(nLeft, TRUE, FALSE);

			pBri->GetTvWingWallPlane(bStt);

			// ���鵵
			CDomyun DomFront(pDom);

			//.. �Ϲݵ�
			m_pIn->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_CONC);
			DBStd.m_pDrawWingWall->DrawGenFront(&DomFront, bStt, bLeft, nOut);
			DBStd.m_pDrawWingWall->DrawGenCutBriSect(&DomFront, bStt, bLeft, nOut);
			//DBStd.m_pDrawWingWall->DrawBlockBoundary(&DomFront, bStt, bLeft, nOut);

			//.. ������
			m_pIn->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_BARC);

			DBStd.m_pDrawWingWall->DrawRebarHunchFootingFront(&DomFront, bStt, bLeft, nOut);
			DBStd.m_pDrawWingWall->DrawRebarHunchWallFront(&DomFront, bStt, bLeft, nOut);

			m_pIn->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_DIML);
			rcFront = DomFront.GetExtRect();

			m_pIn->m_pARoadOptionStd->SetEnvType(&DomFront, HCAD_SMALL_TITLE);
			DomFront.SetTextAlignHorz(TA_CENTER);
			DomFront.SetTextHeight(7.5);
			DomFront.TextOut(rcFront.CenterPoint().x, rcFront.bottom+dSpare1, _T("�� �� ��"));

			DomFront.Move(dSpareT*se, 0);

			*pDom << DomFront;

			// �ܸ鵵
			CDomyun DomSect(pDom);

			CTwinVectorArray tvArrHu, tvArrGen;
			pBri->GetTvArrayRebarWingWallHunchFootingSect(tvArrHu, bStt, bLeft);
			CDPoint xyHu1 = tvArrHu.GetAt(0).m_v1;
			CDPoint xyHu2 = tvArrHu.GetAt(2).m_v2;
			double dHuW = pWing->m_dHuW1;
			double dCoverWingOut = pBri->m_dCoverWingOut[0];
			double dSpare = fabs(xyHu1.x-xyHu2.x) + dCoverWingOut*2 - (dT2+dHuW);

			m_pIn->m_pARoadOptionStd->SetEnvType(&DomSect, HCAD_CONC);
			DBStd.m_pDrawWingWall->DrawGenSection(&DomSect, bStt, bLeft, dSpare);

			m_pIn->m_pARoadOptionStd->SetEnvType(&DomSect, HCAD_BARC);
			DBStd.m_pDrawWingWall->DrawRebarSection(&DomSect, bStt, bLeft);

			if(sArea != "������ ����")
			{
				m_pIn->m_pARoadOptionStd->SetEnvType(&DomSect, HCAD_DIML);
				DBStd.m_pDrawWingWall->DimForCoverInput(&DomSect, sArea, bStt, bLeft);
			}

			CDRect rcSect = DomSect.GetExtRect();

			m_pIn->m_pARoadOptionStd->SetEnvType(&DomSect, HCAD_SMALL_TITLE);
			DomSect.SetTextAlignHorz(TA_CENTER);
			DomSect.SetTextHeight(7.5);
			DomSect.TextOut(rcSect.CenterPoint().x, rcSect.bottom+dSpare1, _T("�� �� ��"));

			if(se*lr>0)
				DomSect.Move((rcFront.Width()+rcSect.Width()-dSpare2)*se, 0);
			else
				DomSect.Move((rcFront.Width()+dSpare2)*se, 0);

			*pDom << DomSect;

			// ��鵵
			pBri->GetTvWingWallPlane(bStt, TRUE, TRUE);

			CDomyun DomPlane(pDom);

			CTwinVector tvEnd, tvOut, tvSlab;
			pBri->m_tvArrPlaneWingWall.GetTvByInfo(_T("����������"), tvEnd);
			pBri->m_tvArrPlaneWingWall.GetTvByInfo(_T("����������"), tvOut);
			pBri->m_tvArrPlaneWingWall.GetTvByInfo(_T("�����곡�ܿ���"), tvSlab);
			CDPoint vSlab = tvSlab.GetXyDir().Rotate90();
			CDPoint xyMid(0, 0), xyCut(0, 0);
			xyMid = tvEnd.GetXyMid();
			GetXyMatchSegAndLine(tvSlab.m_v1, tvSlab.m_v2, xyMid, vSlab, xyCut);
			double dCutLen = 1500;// ? ~(xyCut-tvOut.m_v1) : 1500;
			if(dCutLen<1500)
				dCutLen = 1500;

			pBri->GetTvArrayRebarWingWallHunchWallPlane(tvArrHu, bStt, bLeft, TRUE);
			xyHu1 = tvArrHu.GetAt(0).m_v1;
			xyHu2 = tvArrHu.GetAt(2).m_v2;
			CDPoint vHu1 = tvArrHu.GetAt(0).GetXyDir();
			CDPoint vHu2 = tvArrHu.GetAt(2).GetXyDir();
			GetXyMatchLineAndLine(xyHu1, vHu1, xyHu2, vHu2, xyHu1);
			dSpare = max(~(xyHu1-xyHu2)+dCoverWingOut*2, dCutLen);

			m_pIn->m_pARoadOptionStd->SetEnvType(&DomPlane, HCAD_CONC);
			DBStd.m_pDrawWingWall->DrawGenCutPlane(&DomPlane, bStt, bLeft, dSpare, TRUE);

			m_pIn->m_pARoadOptionStd->SetEnvType(&DomPlane, HCAD_BARC);
			DBStd.m_pDrawWingWall->DrawRebarPlane(&DomPlane, bStt, bLeft);

			if(sArea == "������ ����")
			{
				m_pIn->m_pARoadOptionStd->SetEnvType(&DomPlane, HCAD_DIML);
				DBStd.m_pDrawWingWall->DimForCoverInput(&DomPlane, sArea, bStt, bLeft);
			}

			DomPlane.SetCalcExtRect();
			CDRect rcPlane = DomPlane.GetExtRect();

			m_pIn->m_pARoadOptionStd->SetEnvType(&DomPlane, HCAD_SMALL_TITLE);
			DomPlane.SetTextAlignHorz(TA_CENTER);
			DomPlane.SetTextHeight(7.5);
			DomPlane.TextOut(rcPlane.CenterPoint().x, rcPlane.bottom+dSpare1, _T("�� �� ��"));

			CDPoint xyMove = pBri->GetXySectionWingWallStt(bStt, TRUE);
			xyMove.y = rcFront.bottom - rcPlane.top + dSpare2;
			DomPlane.Move(xyMove);

			pBri->GetTvVertSection_Std(nLeft, TRUE, FALSE, TRUE);
			*pDom << DomPlane;
		}
		else
		{
			BOOL bVertDir	= pBri->IsVertDir();
			//pBri->SyncBridge(0, !bVertDir);
			pBri->GetTvVertSection_Std(0, !bVertDir, FALSE);
			pBri->SyncBridgeRebar(TRUE, bVertDir?-1:1);

			if(sArea == "������ ����")
			{
				CHgBaseDrawStd baseDraw;
				long nJ	= 0;
				double dSta	= pBri->GetStationOnJijum(nJ);
				
				CTwinVectorArray tvArrCross;
				m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
				pBri->GetTvCrossSection(tvArrCross, dSta, CDPoint(0, 1), FALSE, TRUE, FALSE, 0, TRUE);
				baseDraw.DrawTvArray(&Dom, tvArrCross);
				
				DBStd.m_pDrawCrossSection->DrawRebarSupport_CrossSection_UpperSlab(&Dom, dSta, TRUE, 0, FALSE, FALSE, 0);
				DBStd.m_pDrawCrossSection->DrawRebarSupport_CrossSection_UpperSlab(&Dom, dSta, FALSE, 0, FALSE, FALSE, 0);

				m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
				DBStd.m_pDrawCrossSection->DimForCoverInput(&Dom, sArea, nJ);	
			}
			else
			{
				DBStd.m_pDrawVertSection->DrawVertSectionAll(&Dom, FALSE, FALSE, FALSE, FALSE, TRUE);
				DBStd.m_pDrawVertSection->DrawRebarMain_VertSectionAll(&Dom, 0, FALSE, FALSE, FALSE, 0, TRUE, FALSE);

				m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
				DBStd.m_pDrawVertSection->DimForCoverInput(&Dom, sArea);	
			}
			pBri->GetTvVertSection_Std(0, !bVertDir, FALSE, TRUE);
		}
	}

	*pDom << Dom;
	
	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

void CInputGenDesignVariable::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	structGridCell *pCell = m_Grid.GetCurrentCell();
	if(pCell && m_Grid.GetFocus())
	{
		m_pIn->SetComment(pCell->sTitle);
		CString str	= m_Grid.GetItemText(pCell->nRow, 0);
		m_pView->GetDomyun()->STMakeCursor(str);
	}

	if(m_Tab.GetCurSel() == m_nTabCover)
	{
		DrawInputDomyunView(TRUE);
	}
}



// �Ǻ�����
void CInputGenDesignVariable::SetGridDataCover()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 1, 1);

	long row = 0;
	long col = 0;
	m_Grid.AddCell(row, col,	  "ö�� ��ġ");				m_Grid.AddCell(row++, col+1,	  "������");
	m_Grid.AddCell(row, col,	  "������ ���");				m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverUpper_UpperSlab[0]);
	m_Grid.AddCell(row, col,	  "������ �ϸ�");				m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverLower_UpperSlab[0]);
	m_Grid.AddCell(row, col,	  "������ ����");				m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverSide_UpperSlab[0]);
	if(pBri->IsBoxType())
	{
		m_Grid.AddCell(row, col,	  "�Ϻν����� ���");		m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverUpper_LowerSlab[0]);
		m_Grid.AddCell(row, col,	  "�Ϻν����� �ϸ�");		m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverLower_LowerSlab[0]);
	}

	// ��ü�� ��� ������ �������鿡 ���� �Ǻ� ������ ��ü �ܸ� �Ǻ����� �Ѵ�.
	if(pBri->IsOutWall(TRUE) || pBri->IsOutWall(FALSE))
	{
		m_Grid.AddCell(row, col,	  "��ü �ܸ�");			m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverWallOut[0]);
	}
	else
	{
		m_Grid.AddCell(row, col,	  "������ ����(������)");		m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverWallOut[0]);
	}

	if(pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB)
	{
		m_Grid.AddCell(row, col,	  "��ü ����");			m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverWallIn[0]);
		m_Grid.AddCell(row, col,	  "���� ��ü");			m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverWallMid[0]);
		m_Grid.AddCell(row, col,	  "���");				m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverColumn[0]);
		m_Grid.AddCell(row, col,	  "������ �ܸ�");			m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverWingOut[0]);
		m_Grid.AddCell(row, col,	  "������ ����");			m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverWingIn[0]);
		m_Grid.AddCell(row, col,	  "������ ���");			m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverWingUpper);
		m_Grid.AddCell(row, col,	  "������ ����");			m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverWingEnd);
		if(pBri->IsBoxType() == FALSE)
		{
			m_Grid.AddCell(row, col,	  "���� ���");				m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverUpper_Footing[0]);
			m_Grid.AddCell(row, col,	  "���� ����");				m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverSide_Footing[0]);
			m_Grid.AddCell(row, col,	  "���� �ϸ�(��������)");	m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverLower_Footing_Lean[0]);
			m_Grid.AddCell(row, col,	  "���� �ϸ�(���ұ���)");	m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverLower_Footing_Pile[0]);
		}
	}

	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		m_Grid.AddCell(row, col,	  "RIB ���");			m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverRib[iUPPER][0]);
		m_Grid.AddCell(row, col,	  "RIB �ϸ�");			m_Grid.AddCell(row++, col+1,	 &pBri->m_dCoverRib[iLOWER][0]);
	}

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0, 150);
	m_Grid.SetColumnWidth(1, 80);
}

void CInputGenDesignVariable::OnCellChangedData(NMHDR *pNotifyStruct, LRESULT *result)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	int nTab = m_Tab.GetCurSel();

	CCellID cell = m_Grid.GetFocusCell();
	CString str  = m_Grid.GetTextMatrix(cell.row, 0);
	CString str2 = m_Grid.GetTextMatrix(cell.row, 1);
	CString str5 = m_Grid.GetTextMatrix(cell.row, 4);

	BOOL bReDrawGrid(FALSE);

	if(str2.Find("�������� ����")!=-1)
	{
		if(pBri->IsBoxType())
		{
			m_Grid.UpdateData(TRUE);
			long nSelectRigid = pBri->m_nSelectRigidModeling;
			if(nSelectRigid == 1)
			{
				pBri->m_nSelectRigidModeling = 2;
				AfxMessageBox(_T("Box�� ��౳������ ���Ѱ����� ������ �� �����ϴ�.\nEndOffet�� �����մϴ�."));
				SetGridData();
				SetDataInit();
				return;
			}

			if(pBri->m_nSelectRigidModeling == 0)
			{
				// ������������� �����ϸ� ��ü����� ��ġ�� �Է¸��ϵ��� �ٲ��� �Ǵϱ� SetDataInit�� �ٽ��ѹ� ���ش�.
				SetGridData();
				SetDataInit();
			}
		}
	}

	SetDataSave();
	if(str2.Find("��ö�� ��ġ")!=-1)
	{
		CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
		if(!pBri) return;

		if(pBri->GetCountCycleMainRebar() > 3)
		{
			pBri->SyncCycleMainRebar(0, 2);
			pBri->SyncCycleMainRebar(1, 3);
		}

		// Ʈ�� ����
		AfxGetMainWnd()->SendMessage(WM_USER+8, -1);
	}

	if(str2.Find("���ݺ������", 0) != -1)
	{
		SetGridData();
		SetDataInit();
	}

	if(str2=="  3D Plate �ؼ� ��")
	{
		m_pIn->SetDialogEnable();
		bReDrawGrid = TRUE;
	}

	if(str2=="  2D Frame �ؼ� ��")
	{
		// �ؼ��� ������ �ٲ���ٸ� ������ �ʱ�ȭ
		pBri->m_FemNormal.RemoveAllData();
		long i=0; for(i=0; i<3; i++)
		{
			pBri->m_FemEQ[i].RemoveAllData();
		}
		if(pBri->m_nSelect2DFrameTool == SOLVE_MIDAS)
		{
			pBri->m_bSelectEnvSubsid = TRUE;

			// (40867) ���õ� �ؼ����� Midas�̰� ���ݸ𵨸��� NLLINK��� Spring���� �ٲ��ش�.
			pBri->m_n2DGeneralSpringType = 1;
			pBri->m_n2DEQSpringType = 1;
		}
		bReDrawGrid = TRUE;
	}

	if(str2=="  �����ؼ��� �𵨸� ���")
	{
		SetGridData();
		SetDataInit();
	}

	// [v]�� ���� ���ý�
	if(str5=="����" || str5=="�������" || str5=="����" || str5=="�������" || bReDrawGrid)
	{
		SetGridData();
 		SetDataInit();
	}
	
	if (m_bDesignLsd && nTab == m_nTabCondition)
	{
		if (str == "��� ����ġ �� �Է�")
		{
			SyncMaterial();
		}

		if((pBri->GetMaterialDetail() && cell.row > 1 && cell.row <= ePartCount+1) || (pBri->GetMaterialDetail() == FALSE && (cell.row == 2 || cell.row == 3)))
		{
			double uWC = ( m_pIn->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightConcrete>0) ?  m_pIn->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightConcrete/1000 : 2.350;

			long nPos = cell.row - 2;
			if(pBri->GetMaterialDetail() == FALSE && cell.row == 3)
				nPos = ePartApsStt;
			pBri->m_Ec[nPos] = GetValueEcConc(pBri->m_Fck[nPos], uWC, m_pIn->m_pARcBridgeDataStd->IsSI(), m_pIn->m_pARcBridgeDataStd->m_nTypeKciCode); // ���α�������� 38������;
		}

		
		if (pBri->GetMaterialDetail() == FALSE)
		{
			if (str2 == "  ��ü/��ν�����" || str2 == "  " + pBri->GetStringPart(ePartApsStt))
			{
				SyncMaterial();
			}
		}

		// ���� ��ġ �Է»��׿� ���Ͽ� �Ҽ��� 4° �ڸ����� �ڵ� ���Ǿ�� �ϹǷ� �ٽ� �����Ѵ�.
		SetGridData();
		SetDataInit();
	}

	if (nTab == m_nTabEnv)
	{
		HGINT32 iScrollPosHor = m_Grid.GetScrollPos(SB_HORZ);
		HGINT32 iScrollPosVert = m_Grid.GetScrollPos(SB_VERT);
		m_Grid.LockWindowUpdate();
		SetGridData();
		SetDataInit();
		m_Grid.UnlockWindowUpdate();
		m_Grid.SetScrollPos(SB_HORZ, iScrollPosHor);
		m_Grid.SetScrollPos(SB_VERT, iScrollPosVert);
	}

	if(m_bDesignLsd && (nTab >= m_nTabExposureRating && nTab <= m_nTabRebarJoint))
	{
		m_Grid.UpdateData(FALSE);
		SetGridData();
		SetDataInit();
	}
}

// �Ǻ� �Է¿� ��������
CRcBridgeRebar* CInputGenDesignVariable::GetRcBridgeRebarForCover()
{
	/*
	if(m_pBriForCover) 
	{
		delete m_pBriForCover;
		m_pBriForCover	= NULL;
	}
	if(!m_pBriForCover) MakeRcBridgeForCover();
	*/

	return m_pIn->GetRcBridgeRebar();
}

BOOL CInputGenDesignVariable::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CInputDialogBase::DestroyWindow();
}

// Ŀ�� �� �������� ����
void CInputGenDesignVariable::SyncCover(CRcBridgeRebar *pBri)
{
	if(!pBri) return;

	long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		// ���� ��ü
		long nCountInwall	 = pBri->GetCountInWall();
		long wall = 0; for(wall = 0; wall < nCountInwall; wall++)
		{
			CWallApp *pWall	= pBri->GetInWall(wall);
			if(pWall)
			{
				pWall->m_dCoverLeft[dan]	= pBri->m_dCoverWallIn[dan];
				pWall->m_dCoverRight[dan]	= pBri->m_dCoverWallIn[dan];
				pWall->m_dCoverFrontBack[dan]	= pBri->m_dCoverWallIn[dan];
			}
		}

		// ����
		long nCountFooting	= pBri->GetCountFooting();
		long footing = 0; for(footing = 0; footing < nCountFooting; footing++)
		{
			CFootingApp *pFooting	= pBri->GetFooting(footing);
			if(pFooting)
			{
				pFooting->m_dCoverUpper[dan]	= pBri->m_dCoverUpper_Footing[dan];
				pFooting->m_dCoverSide[dan]	= pBri->m_dCoverSide_Footing[dan];
				if(pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_PILE)
					pFooting->m_dCoverLower[dan]	= pBri->m_dCoverLower_Footing_Pile[dan];
				else
					pFooting->m_dCoverLower[dan]	= pBri->m_dCoverLower_Footing_Lean[dan];
			}
		}
	}
}

// ���� �˻� ��������?
void CInputGenDesignVariable::OnCheckQuickSearch() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	pBri->m_bNeedSettingVertSection	= TRUE;
	

	pBri->m_bQuickSearchLine	= m_checkQuickSearch.GetCheck();
	pBri->SetSearchIP();
}

void CInputGenDesignVariable::OnButtonApplyMirror() 
{
	// TODO: Add your control notification handler code here
	// �Է»��� �߰��� �׸��� �߰����־����....
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(pBri==NULL || pBri->m_nSeparBri==0) return;

	CRcBridgeRebar* pBriMatch = pStd->GetBridgeMatchRebar(pBri);
	if(pBriMatch==NULL || pBriMatch->m_nSeparBri==0) return;

	CString strMsg = _T("");
	strMsg.Format("��� ���� �Է»����� %s��(��) �����ϰ� �����˴ϴ�.\n����Ͻðڽ��ϱ� ?",pBriMatch->m_strBridgeName);
	if(AfxMessageBox(strMsg,MB_YESNO)==IDYES)
	{
		MakeSameCondition(pBri,pBriMatch);
		MakeSameEnv(pBri,pBriMatch);
		MakeSameCover(pBri,pBriMatch);
		if(m_bDesignLsd)
		{
			MakeSameLsdSupportRebar(pBri,pBriMatch);//����ö��
		}
		//////////////////////////////////////////////////////////////////////////
		SetDataInit();
	}	
}

void CInputGenDesignVariable::MakeSameCover(CRcBridgeApp* pBri1, CRcBridgeApp* pBri2)
{
	if(pBri1==NULL || pBri2==NULL) return;
	// �Ǻ����� -------------------------------------------------------
	pBri1->m_dCoverUpper_UpperSlab[0]	= pBri2->m_dCoverUpper_UpperSlab[0];
	pBri1->m_dCoverLower_UpperSlab[0]	= pBri2->m_dCoverLower_UpperSlab[0];
	pBri1->m_dCoverSide_UpperSlab[0]	= pBri2->m_dCoverSide_UpperSlab[0];
	pBri1->m_dCoverUpper_LowerSlab[0]	= pBri2->m_dCoverUpper_LowerSlab[0];
	pBri1->m_dCoverLower_LowerSlab[0]	= pBri2->m_dCoverLower_LowerSlab[0];
	pBri1->m_dCoverWallOut[0]			= pBri2->m_dCoverWallOut[0];
	pBri1->m_dCoverWallIn[0]			= pBri2->m_dCoverWallIn[0];
	pBri1->m_dCoverWallMid[0]			= pBri2->m_dCoverWallMid[0];
	pBri1->m_dCoverColumn[0]			= pBri2->m_dCoverColumn[0];
	pBri1->m_dCoverWingOut[0]			= pBri2->m_dCoverWingOut[0];
	pBri1->m_dCoverWingIn[0]			= pBri2->m_dCoverWingIn[0];
	pBri1->m_dCoverWingUpper			= pBri2->m_dCoverWingUpper;
	pBri1->m_dCoverWingEnd				= pBri2->m_dCoverWingEnd;
	pBri1->m_dCoverUpper_Footing[0]		= pBri2->m_dCoverUpper_Footing[0];
	pBri1->m_dCoverSide_Footing[0]		= pBri2->m_dCoverSide_Footing[0];
	pBri1->m_dCoverLower_Footing_Lean[0]	= pBri2->m_dCoverLower_Footing_Lean[0];
	pBri1->m_dCoverLower_Footing_Pile[0]	= pBri2->m_dCoverLower_Footing_Pile[0];
	pBri1->m_dCoverRib[iUPPER][0]		= pBri2->m_dCoverRib[iUPPER][0];
	pBri1->m_dCoverRib[iLOWER][0]		= pBri2->m_dCoverRib[iLOWER][0];
}

void CInputGenDesignVariable::MakeSameEnv(CRcBridgeRebar* pBri1, CRcBridgeRebar* pBri2)
{
	if(pBri1==NULL || pBri2==NULL) return;

	// ���輱�û��� -------------------------------------------------------
	
	//�ܸ����ö��
	if(m_bDesignLsd)
	{
		pBri1->m_nApplyCalcShearDistFrontPile	= pBri2->m_nApplyCalcShearDistFrontPile;
		pBri1->m_nApplyCalcShearDistBackPile	= pBri2->m_nApplyCalcShearDistBackPile;
		pBri1->m_dThetaShearDesign				= pBri2->m_dThetaShearDesign;
		pBri1->m_bCheckDeltaTR					= pBri2->m_bCheckDeltaTR;
		pBri1->m_dElasticModularRatioUseCheck	= pBri2->m_dElasticModularRatioUseCheck;
		pBri1->m_nSelectUsebilityCheck			= pBri2->m_nSelectUsebilityCheck;
		pBri1->m_nTypeCrackWidthCondition		= pBri2->m_nTypeCrackWidthCondition;
		pBri1->m_nSelectExposureCoverCheck		= pBri2->m_nSelectExposureCoverCheck;
		pBri1->m_nSelectPminColumn				= pBri2->m_nSelectPminColumn;
	}
	else
	{
		pBri1->m_bCheckMainSlabAndFootTemper	= pBri2->m_bCheckMainSlabAndFootTemper;
		pBri1->m_bCheckRebarRateFooting			= pBri2->m_bCheckRebarRateFooting;
		pBri1->m_nSelectApplyFs					= pBri2->m_nSelectApplyFs;
		pBri1->m_bCheckSuppoerBarSlabMain		= pBri2->m_bCheckSuppoerBarSlabMain;
		pBri1->m_bCheckSuppoerBarSlabMainOver20	= pBri2->m_bCheckSuppoerBarSlabMainOver20;
		pBri1->m_bCheckSuppoerBarSlabArea		= pBri2->m_bCheckSuppoerBarSlabArea;
		pBri1->m_dCheckSuppoerBarSlabArea		= pBri2->m_dCheckSuppoerBarSlabArea;
		pBri1->m_bCheckSuppoerBarSlab300		= pBri2->m_bCheckSuppoerBarSlab300;
		pBri1->m_bCheckSuppoerBarWall15hs		= pBri2->m_bCheckSuppoerBarWall15hs;
		pBri1->m_dCheckSuppoerBarWallAreaForOver400	= pBri2->m_dCheckSuppoerBarWallAreaForOver400;
		pBri1->m_bCheckSuppoerBarWallArea		= pBri2->m_bCheckSuppoerBarWallArea;
		pBri1->m_dCheckSuppoerBarWallArea		= pBri2->m_dCheckSuppoerBarWallArea;
		pBri1->m_bCheckSuppoerBarWallMainOver20	= pBri2->m_bCheckSuppoerBarWallMainOver20;
		pBri1->m_bCheckSuppoerBarWall300		= pBri2->m_bCheckSuppoerBarWall300;
		pBri1->m_bSelectSupportBarWall			= pBri2->m_bSelectSupportBarWall;
		pBri1->m_bCheckSuppoerBarArchRibMinDia1	= pBri2->m_bCheckSuppoerBarArchRibMinDia1;
		pBri1->m_bCheckSuppoerBarArchRibMinDia2	= pBri2->m_bCheckSuppoerBarArchRibMinDia2;
		pBri1->m_bCheckSuppoerBarFootMainOver20	= pBri2->m_bCheckSuppoerBarFootMainOver20;
		pBri1->m_bCheckSuppoerBarFoot300		= pBri2->m_bCheckSuppoerBarFoot300;
		pBri1->m_bCheckSuppoerBarFoot15hs		= pBri2->m_bCheckSuppoerBarFoot15hs;
		pBri1->m_bCheckSuppoerBarFootArea		= pBri2->m_bCheckSuppoerBarFootArea;
		pBri1->m_dCheckSuppoerBarFootArea		= pBri2->m_dCheckSuppoerBarFootArea;
		pBri1->m_bCheckSuppoerBarWing300		= pBri2->m_bCheckSuppoerBarWing300;
		pBri1->m_bCheckSuppoerBarWing15hs		= pBri2->m_bCheckSuppoerBarWing15hs;
		pBri1->m_bCheckSuppoerBarWingArea		= pBri2->m_bCheckSuppoerBarWingArea;
		pBri1->m_dCheckSuppoerBarWingArea		= pBri2->m_dCheckSuppoerBarWingArea;

		pBri1->m_nTypeCrackDesignSlab			= pBri2->m_nTypeCrackDesignSlab;
		pBri1->m_nTypeCrackDesignWall			= pBri2->m_nTypeCrackDesignWall;
		pBri1->m_nTypeCrackDesignFoot			= pBri2->m_nTypeCrackDesignFoot;
		pBri1->m_nTypeCrackDesignWing			= pBri2->m_nTypeCrackDesignWing;
		pBri1->m_nTypeCrackDesignAps			= pBri2->m_nTypeCrackDesignAps;
		pBri1->m_bApplyFootingMinThick			= pBri2->m_bApplyFootingMinThick;
	}
	pBri1->m_nApplyCalcShearDistSlab		= pBri2->m_nApplyCalcShearDistSlab;
	pBri1->m_nApplyCalcShearDistWall		= pBri2->m_nApplyCalcShearDistWall;
	pBri1->m_bCalcShearCenter				= pBri2->m_bCalcShearCenter;
	pBri1->m_bApplyMinAvSlab				= pBri2->m_bApplyMinAvSlab;
	pBri1->m_bApplyMinAvWall				= pBri2->m_bApplyMinAvWall;
	pBri1->m_bApplyMinAvFoot				= pBri2->m_bApplyMinAvFoot;
	pBri1->m_bApplyMinAvWing				= pBri2->m_bApplyMinAvWing;
	pBri1->m_bApplyMinAvAps					= pBri2->m_bApplyMinAvAps;
	pBri1->m_bCheckMinCtcShearWing			= pBri2->m_bCheckMinCtcShearWing;
	pBri1->m_bApplyT1CalcWingWall			= pBri2->m_bApplyT1CalcWingWall;
	pBri1->m_bCheckMinCtcShearAps			= pBri2->m_bCheckMinCtcShearAps;
	pBri1->m_nSelectApsShearCheck			= pBri2->m_nSelectApsShearCheck;
	pBri1->m_nApplyCalcShearDistFront		= pBri2->m_nApplyCalcShearDistFront;
	pBri1->m_nApplyCalcShearDistBack		= pBri2->m_nApplyCalcShearDistBack;	
	pBri1->m_bSelectHunchDesignCheck		= pBri2->m_bSelectHunchDesignCheck;

	//�������
	if(m_bDesignLsd)
	{
		pBri1->m_nSelectApsAxleWeight			= pBri2->m_nSelectApsAxleWeight;
		pBri1->m_nSelectApsDeadLoad				= pBri2->m_nSelectApsDeadLoad;
	}
	else
	{
		pBri1->m_dFactorToap				= pBri2->m_dFactorToap;
		pBri1->m_dFactorLive				= pBri2->m_dFactorLive;
		pBri1->m_bCheckCornerInside			= pBri2->m_bCheckCornerInside;
		pBri1->m_nSelCalcCornerFt			= pBri2->m_nSelCalcCornerFt;
		pBri1->m_nSelectTempLoadSlab		= pBri2->m_nSelectTempLoadSlab;
		pBri1->m_bSelectEnvToapOne			= pBri2->m_bSelectEnvToapOne;
		pBri1->m_nArchRibSectionMomentPos	= pBri2->m_nArchRibSectionMomentPos;
		pBri1->m_nSelectJointUpperRange		= pBri2->m_nSelectJointUpperRange;
		pBri1->m_nTypeJongdanForRebar		= pBri2->m_nTypeJongdanForRebar;
		pBri1->m_nTypeWallForRebarGen		= pBri2->m_nTypeWallForRebarGen;
		pBri1->m_nIdxCountCycleMainRebar	= pBri2->m_nIdxCountCycleMainRebar;
	}
	pBri1->m_nSelFactorKhStt				= pBri2->m_nSelFactorKhStt;
	pBri1->m_nSelFactorKhEnd				= pBri2->m_nSelFactorKhEnd;
	pBri1->m_nSelectDesignRoadWidth			= pBri2->m_nSelectDesignRoadWidth;
	pBri1->m_nSelectLiveLoadSpanLength		= pBri2->m_nSelectLiveLoadSpanLength;
	pBri1->m_nSelectStartLane				= pBri2->m_nSelectStartLane;
	pBri1->m_bCheckUseri					= pBri2->m_bCheckUseri;
	pBri1->m_dCheckUseriValue				= pBri2->m_dCheckUseriValue;
	pBri1->m_bApplyFootingGForce			= pBri2->m_bApplyFootingGForce;
	pBri1->m_bApplyMassCalcSafe				= pBri2->m_bApplyMassCalcSafe;
	pBri1->m_nSelectForceCheckPos			= pBri2->m_nSelectForceCheckPos;
	pBri1->m_nSelectFootingReactionForce	= pBri2->m_nSelectFootingReactionForce;
	pBri1->m_bCheckFrontFootTrap2			= pBri2->m_bCheckFrontFootTrap2;
	pBri1->m_nSelSubsideType				= pBri2->m_nSelSubsideType;
	pBri1->m_nSelectWallSectionDesign		= pBri2->m_nSelectWallSectionDesign;
	pBri1->m_nSelectInWallSectionDesign		= pBri2->m_nSelectInWallSectionDesign;
	pBri1->m_nSelectRigidModeling			= pBri2->m_nSelectRigidModeling;
	pBri1->m_nSelectModelingHeight			= pBri2->m_nSelectModelingHeight;
	pBri1->m_bSelectTempLoadAddWall			= pBri2->m_bSelectTempLoadAddWall;
	pBri1->m_bSelectShrinkLoadAddWall		= pBri2->m_bSelectShrinkLoadAddWall;
	pBri1->m_bSelectEnvSubsid				= pBri2->m_bSelectEnvSubsid;
	pBri1->m_bSelectHunchModeling			= pBri2->m_bSelectHunchModeling;
	pBri1->m_bSelectRigidCalcHunch			= pBri2->m_bSelectRigidCalcHunch;
	pBri1->m_nSelectDesignSpanLength		= pBri2->m_nSelectDesignSpanLength;
	pBri1->m_nSelectDesignSpanApply			= pBri2->m_nSelectDesignSpanApply;
	pBri1->m_bSelectSpanLengthApplyShoe		= pBri2->m_bSelectSpanLengthApplyShoe;
	pBri1->m_nTypeStandardCTCRebarMain		= pBri2->m_nTypeStandardCTCRebarMain;
	pBri1->m_nTypePlacingSupportRebar_UpperSlab		= pBri2->m_nTypePlacingSupportRebar_UpperSlab;
	pBri1->m_nTypePlacingMainRebar_UpperSlab		= pBri2->m_nTypePlacingMainRebar_UpperSlab;
	pBri1->m_nTypeSideRebarMain_UpperSlabSabogang	= pBri2->m_nTypeSideRebarMain_UpperSlabSabogang;
	pBri1->m_nTypePlacingMainRebar_Footing	= pBri2->m_nTypePlacingMainRebar_Footing;
	pBri1->m_nTypeCurveOfWallVertRebar		= pBri2->m_nTypeCurveOfWallVertRebar;
	pBri1->m_bFixOutSideMainRebarBy1Cyle	= pBri2->m_bFixOutSideMainRebarBy1Cyle;
	pBri1->m_bRemoveShearRebarAtSlabYundan	= pBri2->m_bRemoveShearRebarAtSlabYundan;
	pBri1->m_nTypeCouplerColumnMainRebar	= pBri2->m_nTypeCouplerColumnMainRebar;
	pBri1->m_bApplySameSupportRebar			= pBri2->m_bApplySameSupportRebar;
	pBri1->m_bApplySameMainRebar			= pBri2->m_bApplySameMainRebar;

}

void CInputGenDesignVariable::MakeSameCondition(CRcBridgeApp* pBri1, CRcBridgeApp* pBri2)
{
	if(pBri1==NULL || pBri2==NULL) return;

	if(m_bDesignLsd)
	{
		pBri1->SetMaterialDetail(pBri2->GetMaterialDetail());
		const HGINT32 iEnd = pBri2->GetMaterialDetail() ? ePartCount : ePartUpperSlab + 1;
		for (HGINT32 ix = ePartUpperSlab; ix < iEnd; ++ix)
		{
			pBri1->m_Fck[ix] = pBri2->m_Fck[ix];
			pBri1->m_Fy[ix] = pBri2->m_Fy[ix];
			pBri1->m_Fvy[ix] = pBri2->m_Fvy[ix];
			pBri1->m_Ec[ix] = pBri2->m_Ec[ix];
		}

		pBri1->SetRateDryAndShrinkage(pBri2->GetRateDryAndShrinkage());
		pBri1->SetCoefficientExpansion(pBri2->GetCoefficientExpansion());
		pBri1->SetDBar_ForDowelBar(pBri2->GetDBar_ForDowelBar());
		
	}

	// �������� -------------------------------------------------------
	pBri1->m_dUnitWeigthBackFill		= pBri2->m_dUnitWeigthBackFill;
	pBri1->m_dAngleFrictionBackFill		= pBri2->m_dAngleFrictionBackFill;
	pBri1->m_dLiveLoadAlways			= pBri2->m_dLiveLoadAlways;
	pBri1->m_dDeadLoadAlways			= pBri2->m_dDeadLoadAlways;
	pBri1->m_dLiveLoadFootway			= pBri2->m_dLiveLoadFootway;
	pBri1->m_dUnitWeightHeapSoil		= pBri2->m_dUnitWeightHeapSoil;	
	pBri1->m_dUnitWeightValidDepth		= pBri2->m_dUnitWeightValidDepth;
	pBri1->m_dValidDepthBaseGround		= pBri2->m_dValidDepthBaseGround;
	pBri1->m_dEo_CalcWallSpringEQ		= pBri2->m_dEo_CalcWallSpringEQ;
	pBri1->m_nTypeToapSafeCheck			= pBri2->m_nTypeToapSafeCheck;	
	pBri1->m_SafeFactorOverturning[0]	= pBri2->m_SafeFactorOverturning[0];
	pBri1->m_SafeFactorOverturning[1]	= pBri2->m_SafeFactorOverturning[1];
	pBri1->m_SafeFactorSliding[0]		= pBri2->m_SafeFactorSliding[0];
	pBri1->m_SafeFactorSliding[1]		= pBri2->m_SafeFactorSliding[1];
	pBri1->m_nSelectModelingCountEQ		= pBri2->m_nSelectModelingCountEQ;	
	pBri1->m_nSelectPileCalcSpringEQ	= pBri2->m_nSelectPileCalcSpringEQ;	
	pBri1->m_nSelect2DFrameTool			= pBri2->m_nSelect2DFrameTool;	
	pBri1->m_nSelect3DShellTool			= pBri2->m_nSelect3DShellTool;	
	pBri1->m_nSelect3DOutputValue		= pBri2->m_nSelect3DOutputValue;	
	pBri1->m_nApplyWingWallCalcValueCount		= pBri2->m_nApplyWingWallCalcValueCount;	

	if(m_bDesignLsd)
	{
		pBri1->SetTypeDesingEQ(pBri2->GetTypeDesingEQ());
		pBri1->m_dCoffKh_Sei		= pBri2->m_dCoffKh_Sei;	
		pBri1->SetDegreeEarthQuake(pBri2->GetDegreeEarthQuake());
		pBri1->SetRegionEarthQuake(pBri2->GetRegionEarthQuake());
		pBri1->SetCoefficientGround(pBri2->GetCoefficientGround());
		pBri1->SetGravity(pBri2->GetGravity());
		pBri1->SetTypeRigidFootingEQ(pBri2->GetTypeRigidFootingEQ());
		pBri1->m_dEo_CalcWallSpringEQ = pBri2->m_dEo_CalcWallSpringEQ;
		pBri1->SetApplyRigidWallEQRotate(pBri2->GetApplyRigidWallEQRotate());
		pBri1->SetTypeJointPosFootingEQ(pBri2->GetTypeJointPosFootingEQ());
		pBri1->SetTypePropertyPosFootingEQ(pBri2->GetTypePropertyPosFootingEQ());
		pBri1->SetTypeSoilPressureEQ(pBri2->GetTypeSoilPressureEQ());
		pBri1->SetTypeUpperWeightEQ(pBri2->GetTypeUpperWeightEQ());
		if(pBri1->m_nTypeBridge != BRIDGE_TYPE_BOX && pBri2->m_nTypeBridge != BRIDGE_TYPE_BOX)
		{
			pBri1->m_nSelectFootForce = pBri2->m_nSelectFootForce;	
		}
	}

	pBri1->m_dFckMassConc				= pBri2->m_dFckMassConc;
	pBri1->m_dFckLeanConc				= pBri2->m_dFckLeanConc;
}

void CInputGenDesignVariable::MakeSameLsdSupportRebar(CRcBridgeApp* pBri1, CRcBridgeApp* pBri2)
{
	//������
	CLsdExposureRatingCtl *pExposureTar = &pBri1->m_plsdEnvRating->GetLsdExposureRatingCtl();
	CLsdExposureRatingCtl *pExposureSrc = &pBri2->m_plsdEnvRating->GetLsdExposureRatingCtl();

	pExposureSrc->CopyLsdExposureRating(pExposureTar);
	pExposureSrc->CopyLsdExposureMaterial(pExposureTar);

	//������
	CLsdDesignRatingCtl *pDesignRatingTar = &pBri1->m_plsdEnvRating->GetLsdDesignRatingCtl();
	CLsdDesignRatingCtl *pDesignRatingSrc = &pBri2->m_plsdEnvRating->GetLsdDesignRatingCtl();

	pDesignRatingSrc->CopyLsdDesignRating(pDesignRatingTar);

	//������
	pBri1->m_plsdRebar->GetOption() = pBri2->m_plsdRebar->GetOption();

	//����ö��
	pBri1->m_bCheckSupportSlabDryingShrinkage = pBri2->m_bCheckSupportSlabDryingShrinkage;
	pBri1->m_bCheckSupportSlabByReqRebar = pBri2->m_bCheckSupportSlabByReqRebar;
	pBri1->m_bCheckSupportSlabMainRate = pBri2->m_bCheckSupportSlabMainRate;
	pBri1->m_bCheckSupportSlabMainRateSpan = pBri2->m_bCheckSupportSlabMainRateSpan;
	pBri1->m_bCheckSupportWallMainRate = pBri2->m_bCheckSupportWallMainRate;
	pBri1->m_bCheckSupportFootDryingShrinkage = pBri2->m_bCheckSupportFootDryingShrinkage;
	pBri1->m_bSkipAsMinCheckByHoriRebar = pBri2->m_bSkipAsMinCheckByHoriRebar;

}

void CInputGenDesignVariable::OnButtonApplySameAll() 
{
	// TODO: Add your control notification handler code here
	// �Է»��� �߰��� �׸��� �߰����־����....
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(pBri==NULL) return;

	CString strMsg = _T("");
	strMsg.Format("��� ������ ��ü �Է»����� %s��(��) �����ϰ� �����˴ϴ�.\n����Ͻðڽ��ϱ� ?",pBri->m_strBridgeName);
	if(AfxMessageBox(strMsg,MB_YESNO)==IDYES)
	{
		long nQtyBri = pStd->GetBridgeSize();

		long i=0; for(i=0; i<nQtyBri; i++)
		{
			if(i==m_pIn->m_nCurrentBridge) continue;

			CRcBridgeRebar* pBri2 = pStd->GetBridgeRebar(i);

			MakeSameCondition(pBri2,pBri);
			MakeSameEnv(pBri2,pBri);
			MakeSameCover(pBri2,pBri);
			if(m_bDesignLsd)
			{
				MakeSameLsdSupportRebar(pBri2,pBri);
			}
		}
	}	
}

void CInputGenDesignVariable::KillFocusDlg()
{
	if(m_pBriForCover) delete m_pBriForCover;
}

void CInputGenDesignVariable::OnButtonBasedataSave() 
{
	// TODO: Add your control notification handler code here
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);

	CString sTitle	= _T("����ȯ�� ����");
	CFileDialog dlg(FALSE, "*.bar",NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
								  "ARcBridge ����ȯ�� ���� (*.bar)|*.bar||",GetParent());
	dlg.m_ofn.lpstrTitle = sTitle;
	if (dlg.DoModal() == IDOK)
	{
		SetDataSave();
		CFile f;
		if(f.Open(dlg.GetPathName(),CFile::modeCreate|CFile::modeWrite)==FALSE) return;
		CArchive ar(&f,CArchive::store);

		pBri->SerializeEnv(ar);
		ar.Close();
		f.Close();		
	}			
}

void CInputGenDesignVariable::OnButtonBasedataLoad() 
{
	// TODO: Add your control notification handler code here
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);

//	��ö�ٹ�ġ TEST CODE
//	pBri->SettingTvArrRebarMainPlane();
//	return;

	CString sTitle	= _T("����ȯ�� �ҷ�����");
	CFileDialog dlg(TRUE, "*.bar",NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
					  "ARcBridge ����ȯ�� ���� (*.bar)|*.bar||",GetParent());
	dlg.m_ofn.lpstrTitle = sTitle;	
	if (dlg.DoModal() == IDOK)
	{		
		CFile f;
		if(f.Open(dlg.GetPathName(),CFile::modeRead)==FALSE) return;
		CArchive ar(&f,CArchive::load);
		pBri->SerializeEnv(ar);
		ar.Close();
		f.Close();

		SetGridData();
		SetDataInit();
	}	
}

void CInputGenDesignVariable::OnButtonExamine() 
{
	int nTab = m_Tab.GetCurSel();

	if(nTab != m_nTabExposureRating)
		return;

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	m_pDlgCheck->SetBriAndContent(pBri,IDD_GEN_DESIGN_VARIABLE,0);
	m_pDlgCheck->ShowAndCheck(this);	
	CString str = _T("");
	str.Format("������");
	m_pDlgCheck->SetWindowText(str);
}

void CInputGenDesignVariable::SetGridDataExposureRating()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	m_Grid.SetLsdEnviromentRating(pBri->m_plsdEnvRating);	
	m_Grid.SetLsdManager(pBri->m_plsdManager);
	m_Grid.SetGridInitExposureRating();
}

void CInputGenDesignVariable::SetGridDataDesignRating()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	m_Grid.SetLsdEnviromentRating(pBri->m_plsdEnvRating);
	m_Grid.SetLsdManager(pBri->m_plsdManager);
	m_Grid.SetGridInitDesignRating();
}

void CInputGenDesignVariable::SetGridDataRebarFactor()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	
	m_Grid.SetGridDefault(1, 3, 1, 2);
	
	HGINT32 row = 0;
	HGINT32 col = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;

	m_Grid.AddCell(row, col,hggettext("����"));		
	m_Grid.AddCell(row, col+1,hggettext("���� [��.��(2015) 5.9.4.3]"));		
	m_Grid.AddCell(row, col+2,hggettext("���"));

	row++;
	m_Grid.AddCell(row, 0,_T("n1")); 
	m_Grid.AddCell(row, 1,hggettext("�������� �������(�����ǰ��)"), DT_LEFT); 
	m_Grid.AddCell(row, 2, &pBri->m_plsdRebar->GetOption().dn1[0]);	m_Grid.SetCellDec(CELL_DEC_2);  m_Grid.SetLastCellMinLimit(0.1);

	row++;
	m_Grid.AddCell(row, 0,_T("n1")); 
	m_Grid.AddCell(row, 1,hggettext("�������� �������(���ö��, 30cm �̻��� ���̿� ��ġ�� ö��)"), DT_LEFT); 
	m_Grid.AddCell(row, 2, &pBri->m_plsdRebar->GetOption().dn1[1]);	m_Grid.SetCellDec(CELL_DEC_2);  m_Grid.SetLastCellMinLimit(0.1);

	row++;
	m_Grid.AddCell(row, 0,_T("n2")); 
	m_Grid.AddCell(row, 1,hggettext("ö������ ���� �������(32mm ����)"), DT_LEFT); 
	m_Grid.AddCell(row, 2, &pBri->m_plsdRebar->GetOption().dn2);	m_Grid.SetCellDec(CELL_DEC_2);  m_Grid.SetLastCellMinLimit(0.1);

	row++;
	m_Grid.AddCell(row, 0,_T("n2")); 
	m_Grid.AddCell(row, 1,hggettext("ö������ ���� �������(32mm �ʰ�, (132-db/100))"), DT_LEFT); 
	m_Grid.AddCell(row, 2,hggettext("-"),nFormat,CELL_READONLY); m_Grid.SetCellDec(CELL_DEC_2); 

	row++;
	m_Grid.AddCell(row, 0,_T("K")); 
	m_Grid.AddCell(row, 1,hggettext("Ⱦö�ٿ� ���� ����(����ö��)"), DT_LEFT); 
	m_Grid.AddCell(row, 2, &pStd->m_plsdRebar->GetOption().dK_RebarPos[iOUTTER],nFormat,CELL_READONLY);  m_Grid.SetCellDec(CELL_DEC_2);

	row++;
	m_Grid.AddCell(row, 0,_T("K")); 
	m_Grid.AddCell(row, 1,hggettext("Ⱦö�ٿ� ���� ����(����ö��)"), DT_LEFT); 
	m_Grid.AddCell(row, 2, &pStd->m_plsdRebar->GetOption().dK_RebarPos[iINNER]);  m_Grid.SetCellDec(CELL_DEC_2);

	row++;
	m_Grid.AddCell(row, 0,_T("fr")); 
	m_Grid.AddCell(row, 1,hggettext("��������(Mpa)"), DT_LEFT); 
	m_Grid.AddCell(row, 2, &pBri->m_plsdRebar->GetOption().dRestraintStress);	m_Grid.SetCellDec(CELL_DEC_2);

	row++;
	m_Grid.AddCell(row, 0,_T("Ro")); 
	m_Grid.AddCell(row, 1,hggettext("��ħ��������(%)"), DT_LEFT); 
	m_Grid.AddCell(row, 2, &pBri->m_plsdRebar->GetOption().dOverRatio);	m_Grid.SetCellDec(CELL_DEC_1);

	row++;
	m_Grid.AddCell(row, 0,_T("Ras")); 
	m_Grid.AddCell(row, 1,hggettext("�ʿ�/���ö�ٷ���(As.req/As,.prov)"), DT_LEFT); 
	m_Grid.AddCell(row, 2, &pBri->m_plsdRebar->GetOption().dAsRatio);	m_Grid.SetCellDec(CELL_DEC_3);

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetRowHeightAll(25);
	m_Grid.SetColumnWidth(0, 100);
	m_Grid.SetColumnWidth(1, 400);
	m_Grid.SetColumnWidth(2, 100);
}

void CInputGenDesignVariable::SetGridDataRebarSettle()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	m_Grid.SetRowHeightAll(25);
	m_Grid.SetGridRebarSimplifySettle(pBri->m_plsdRebar);
}

void CInputGenDesignVariable::SetGridDataRebarJoint()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	m_Grid.SetGridRebarSimplifyConnect(pBri->m_plsdRebar);
}

void CInputGenDesignVariable::SeteGridDataLsdSupportRebar()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	CString str =_T(""), strRoad = _T("");

	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 4, 0, 3);

	UINT nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	long nRow = 0, nCol = 0;
	long nRows = 0;
	long nCols = 0;	

	// ���ӽ�����
	if(pStd->IsDesignKDS2019())
		strRoad = _T("[KDS 24 14 21 4.6.15]");
	else
		strRoad = _T("(���� 5.12.15)");
	m_Grid.AddCell(nRow, nCol++, hggettext("������"), DT_CENTER | DT_VCENTER);
	str.Format(hggettext("�������� �� �µ�ö�� %s"), strRoad);
	m_Grid.AddCell(nRow, nCol++, str, DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, str, DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, &pBri->m_bCheckSupportSlabDryingShrinkage, nFormat, CELL_LINE_NO, _T(""), CELL_TYPE_CHECK); 
	m_Grid.AddCell(nRow, nCol++, pBri->m_bCheckSupportSlabDryingShrinkage ? hggettext("����") : hggettext("�������"), DT_CENTER, CELL_LINE_NO);
	nRow++, nCol = 0;

	m_Grid.AddCell(nRow, nCol++, hggettext("������"), DT_CENTER | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, hggettext("��ö�ٺ�"), DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, hggettext("�ʿ�ö�ٷ�/���ö�ٷ�"), DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, &pBri->m_bCheckSupportSlabByReqRebar, nFormat, CELL_LINE_NO, _T(""), CELL_TYPE_CHECK); 
	m_Grid.AddCell(nRow, nCol++, pBri->m_bCheckSupportSlabByReqRebar ? hggettext("�ʿ�ö��") : hggettext("���ö��"), DT_CENTER, CELL_LINE_NO);
	nRow++, nCol = 0;

	if(pStd->IsDesignKDS2019())
		strRoad = _T("[KDS 24 14 21 4.6.3.1]");
	else
		strRoad = _T("(���� 5.12.3.1)");
	m_Grid.AddCell(nRow, nCol++, hggettext("������"), DT_CENTER | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, hggettext("��ö�ٺ�"), DT_LEFT | DT_VCENTER);
	str.Format(hggettext("��ö���� 20%% %s"), strRoad);
	m_Grid.AddCell(nRow, nCol++, str, DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, &pBri->m_bCheckSupportSlabMainRate, nFormat, CELL_LINE_NO, _T(""), CELL_TYPE_CHECK); 
	m_Grid.AddCell(nRow, nCol++, pBri->m_bCheckSupportSlabMainRate ? hggettext("����") : hggettext("�������"), DT_CENTER, CELL_LINE_NO);
	nRow++, nCol = 0;

	if(pStd->IsDesignKDS2019())
		strRoad = _T("[KDS 24 14 21 4.6.5.3]");
	else
		strRoad = _T("(���� 5.12.5.3)");
	m_Grid.AddCell(nRow, nCol++, hggettext("������"), DT_CENTER | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, hggettext("��ö�ٺ�"), DT_LEFT | DT_VCENTER);
	str.Format(hggettext("�������⿡ ���� ��ö�ٺ� %s"), strRoad);
	m_Grid.AddCell(nRow, nCol++, str, DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, &pBri->m_bCheckSupportSlabMainRateSpan, nFormat, CELL_LINE_NO, _T(""), CELL_TYPE_CHECK); 
	m_Grid.AddCell(nRow, nCol++, pBri->m_bCheckSupportSlabMainRateSpan ? hggettext("����") : hggettext("�������"), DT_CENTER, CELL_LINE_NO);
	nRow++, nCol = 0;
	
	// ��ü
	if(pStd->IsDesignKDS2019())
		strRoad = _T("[KDS 24 14 21 4.6.15]");
	else
		strRoad = _T("(���� 5.12.15)");
	m_Grid.AddCell(nRow, nCol++, hggettext("��ü"), DT_CENTER | DT_VCENTER);
	str.Format(hggettext("�������� �� �µ�ö�� %s"), strRoad);
	m_Grid.AddCell(nRow, nCol++, str, DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, str, DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, &pBri->m_bCheckSupportWallDryingShrinkage, nFormat, CELL_LINE_NO, _T(""), CELL_TYPE_CHECK); 
	m_Grid.AddCell(nRow, nCol++, pBri->m_bCheckSupportWallDryingShrinkage ? hggettext("����") : hggettext("�������"), DT_CENTER, CELL_LINE_NO);
	nRow++, nCol = 0;

	if(pStd->IsDesignKDS2019())
		strRoad = _T("[KDS 24 14 21 4.6.8.3]");
	else
		strRoad = _T("(���� 5.12.8.3)");
	m_Grid.AddCell(nRow, nCol++, hggettext("��ü"), DT_CENTER | DT_VCENTER);
	str.Format(hggettext("���� ö�ٷ��� 25%% %s"), strRoad);
	m_Grid.AddCell(nRow, nCol++, str, DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, str, DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, &pBri->m_bCheckSupportWallMainRate, nFormat, CELL_LINE_NO, _T(""), CELL_TYPE_CHECK); 
	m_Grid.AddCell(nRow, nCol++, pBri->m_bCheckSupportWallMainRate ? hggettext("����") : hgwgettext("�������"), DT_CENTER, CELL_LINE_NO);
	nRow++, nCol = 0;

	// ����
	if(pStd->IsDesignKDS2019())
		strRoad = _T("[KDS 24 14 21 4.6.15]");
	else
		strRoad = _T("(���� 5.12.15)");
	m_Grid.AddCell(nRow, nCol++, hggettext("����"), DT_CENTER | DT_VCENTER);
	str.Format(hggettext("�������� �� �µ�ö�� %s"), strRoad);
	m_Grid.AddCell(nRow, nCol++, str, DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, str, DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, &pBri->m_bCheckSupportFootDryingShrinkage, nFormat, CELL_LINE_NO, _T(""), CELL_TYPE_CHECK); 
	m_Grid.AddCell(nRow, nCol++, pBri->m_bCheckSupportFootDryingShrinkage ? hggettext("����") : hggettext("�������"), DT_CENTER, CELL_LINE_NO);
	nRow++, nCol = 0;

	if(pStd->IsDesignKDS2019())
		strRoad = _T("[KDS 24 14 21 4.6.2.1]");
	else
		strRoad = _T("(���� 5.12.2.1(1)(��))");
	m_Grid.AddCell(nRow, nCol++, hggettext("����"), DT_CENTER | DT_VCENTER);
	str.Format(hggettext("�������� �� �µ�ö������ �ּ�����ö�� ���� %s"), strRoad);
	m_Grid.AddCell(nRow, nCol++, str, DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, str, DT_LEFT | DT_VCENTER);
	m_Grid.AddCell(nRow, nCol++, &pBri->m_bSkipAsMinCheckByHoriRebar, nFormat, CELL_LINE_NO, _T(""), CELL_TYPE_CHECK); 
	m_Grid.AddCell(nRow, nCol++, pBri->m_bSkipAsMinCheckByHoriRebar ? hggettext("����") : hggettext("�������"), DT_CENTER, CELL_LINE_NO);
	nRow++, nCol = 0;

	m_Grid.GetCellCount(nRows, nCols);
	m_Grid.SetRows(nRows);
	m_Grid.SetCols(nCols);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidth(0,100);
	m_Grid.SetColumnWidth(1,100);
	m_Grid.SetColumnWidth(2,350);
	m_Grid.SetColumnWidth(3,30);
	m_Grid.SetColumnWidth(4,60);	
}

void CInputGenDesignVariable::SyncMaterial()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	
	for (HGINT32 ix = ePartUpperSlab + 1; ix < ePartCount; ++ix)
	{
		if (ix == ePartApsStt || ix == ePartApsEnd)
			continue;

		pBri->m_Fck[ix]	 = pBri->m_Fck[ePartUpperSlab];
		pBri->m_Fy[ix]	 = pBri->m_Fy[ePartUpperSlab];
		pBri->m_Fvy[ix]	 = pBri->m_Fvy[ePartUpperSlab];
		pBri->m_Ec[ix]	 = pBri->m_Ec[ePartUpperSlab];
		
	}

	pBri->m_Fck[ePartApsEnd] = pBri->m_Fck[ePartApsStt];
	pBri->m_Fy[ePartApsEnd] = pBri->m_Fy[ePartApsStt];
	pBri->m_Fvy[ePartApsEnd] = pBri->m_Fvy[ePartApsStt];
	pBri->m_Ec[ePartApsEnd] = pBri->m_Ec[ePartApsStt];
}
