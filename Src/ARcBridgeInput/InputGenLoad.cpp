// InputGenLoad.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "arcbridgeinput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CInputGenLoad ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CInputGenLoad, CInputDialogBase)

CInputGenLoad::CInputGenLoad(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputGenLoad::IDD, pParent)
{
	m_bAutoDefaultCombine = TRUE;
	m_bShowUseLoad = FALSE;				// (ARCBRIDGE-2908) �⺻�� False��...
	m_bAutoCombinInit = FALSE;
	m_bEtaCombinEach	= FALSE;

	m_nTabSelectLoad = -1;
	m_nTabCombineUseLoad = -1;
	m_nTabCombineUltLoad = -1;
	m_nTabCombineEccLoad = -1;
	m_nTabCombineEQLoad = -1;

	m_nTabLsdLoadFactor		= -1;
	m_nTabLsdComboLimit		= -1;
	m_nTabLsdComboExtreme	= -1;
	m_nTabLsdComboService	= -1;
	m_nTabLsdComboFatigue	= -1;
	m_nTabLsdModifiedFactor	= -1;
	m_nTabLsdLoad			= -1;
	m_nTabLsdUserLoadNormal	= -1;
	m_nTabLsdUserLoadEQ		= -1;
	m_nTabLsdUserLoadEQ2	= -1;
	m_nTabLsdUserLoadEQ3	= -1;
	m_nTabLsdUserLoadEQ4	= -1;

	m_nIdxUserSel		= 0;
}

CInputGenLoad::~CInputGenLoad()
{
}

void CInputGenLoad::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_cbBridgeNo);
	DDX_Control(pDX, IDC_BUTTON_REMOVE_COMBINE, m_btnRemoveCombine);
	DDX_Control(pDX, IDC_BUTTON_ADD_COMBINE, m_btnAddCombine);
	DDX_Control(pDX, IDC_BUTTON_APPLY_MIRROR2, m_btnMirror);
	DDX_Control(pDX, IDC_BUTTON_RESET_DETAILCOMBOA, m_btnLsdComboReset);
	DDX_Control(pDX, IDC_BUTTON_ADD_USER, m_btnAddUser);
	DDX_Control(pDX, IDC_BUTTON_REMOVE_USER, m_btnRemoveUser);
	DDX_Control(pDX, IDC_TAB_LSDETA, m_ctlTabLsdEta);
	DDX_Check(pDX, IDC_CHECK_AUTO_COMBINE, m_bAutoDefaultCombine);
	DDX_Check(pDX, IDC_CHECK_SHOW_USELOAD, m_bShowUseLoad);
	DDX_Check(pDX, IDC_CHECK_AUTO_COMBIN_INIT, m_bAutoCombinInit);
	DDX_Check(pDX, IDC_CHECK_ETA_COMBIN_EACH, m_bEtaCombinEach);
}


BEGIN_MESSAGE_MAP(CInputGenLoad, CInputDialogBase)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_LSDETA, OnTabSelchangeLsdEta)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
	ON_BN_CLICKED(IDC_BUTTON_RESET_DETAILCOMBOA, OnButtonLsdComboReset)
	ON_BN_CLICKED(IDC_BUTTON_ADD_COMBINE, OnButtonAddCombine)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_COMBINE, OnButtonRemoveCombine)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_SAMEALL, OnButtonApplySameAll)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_MIRROR2, OnButtonApplyMirror)
	ON_BN_CLICKED(IDC_CHECK_SHOW_USELOAD, OnCheckShowUseLoad)
	ON_BN_CLICKED(IDC_CHECK_AUTO_COMBIN_INIT, OnCheckAutoCombinInit)
	ON_BN_CLICKED(IDC_CHECK_ETA_COMBIN_EACH, OnCheckEtaCombinEach)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_USER, OnButtonRemoveUser)
	ON_BN_CLICKED(IDC_BUTTON_ADD_USER, OnButtonAddUser)
	ON_NOTIFY(GVN_CELL_BUTTONCLICK, IDC_GRID, OnCellButtonClick)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID, OnCellChangedRow)
END_MESSAGE_MAP()

// CInputGenLoad �޽��� ó�����Դϴ�.

void CInputGenLoad::OnPreInitDialog()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();

	m_nTabSelectLoad = -1;
	m_nTabCombineUseLoad = -1;
	m_nTabCombineUltLoad = -1;
	m_nTabCombineEccLoad = -1;
	m_nTabCombineEQLoad = -1;

	m_nTabLsdLoadFactor		= -1;
	m_nTabLsdComboLimit		= -1;
	m_nTabLsdComboExtreme	= -1;
	m_nTabLsdComboService	= -1;
	m_nTabLsdComboFatigue	= -1;
	m_nTabLsdModifiedFactor	= -1;
	m_nTabLsdLoad			= -1;
	m_nTabLsdUserLoadNormal	= -1;
	m_nTabLsdUserLoadEQ		= -1;
	m_nTabLsdUserLoadEQ2	= -1;
	m_nTabLsdUserLoadEQ3	= -1;
	m_nTabLsdUserLoadEQ4	= -1;

	long nTab = 0;
	m_sArrTabName.RemoveAll();

	if (m_bDesignLsd)
	{
		m_sArrTabName.Add(_T("�������߼���"));		m_nTabSelectLoad	 = nTab++;
		m_sArrTabName.Add(_T("�������ռ���"));		m_nTabLsdLoadFactor	 = nTab++;
		m_sArrTabName.Add(_T("�����Ѱ����"));		m_nTabLsdComboLimit	 = nTab++;
		m_sArrTabName.Add(_T("�شܻ�Ȳ�Ѱ����"));	m_nTabLsdComboExtreme	 = nTab++;
		m_sArrTabName.Add(_T("����Ѱ����"));		m_nTabLsdComboService	 = nTab++;
		// (ARCBRIDGE-3232) �Ƿ� ���� ���� ���� -> �Ƿ�Ȱ������ �״�� ����.
		// m_sArrTabName.Add(_T("�Ƿ��Ѱ����"));		m_nTabLsdComboFatigue	 = nTab++;
		m_sArrTabName.Add(_T("���߼������"));		m_nTabLsdModifiedFactor	 = nTab++;
		m_sArrTabName.Add(_T("����"));				m_nTabLsdLoad	 = nTab++;
		m_sArrTabName.Add(_T("���������-���"));	m_nTabLsdUserLoadNormal	= nTab++;
		if(pBri && pBri->IsBoxType())
		{
			m_sArrTabName.Add(_T("���������-������(�ر�����)"));	m_nTabLsdUserLoadEQ		= nTab++;
			m_sArrTabName.Add(_T("���������-������(��ɼ���)"));	m_nTabLsdUserLoadEQ2	= nTab++;
			if(pBri->m_pARcBridgeDataStd->m_nTypeEarthQuakeApply > 0)
			{
				m_sArrTabName.Add(_T("���������-������(��ú���)"));	m_nTabLsdUserLoadEQ3		= nTab++;
				m_sArrTabName.Add(_T("���������-������(��⺹��/�θ�ȣ)"));	m_nTabLsdUserLoadEQ4	= nTab++;
			}
		}
		else
		{
			m_sArrTabName.Add(_T("���������-������"));	m_nTabLsdUserLoadEQ		= nTab++;
		}

		m_szArrTypeUserLoad.RemoveAll();
		m_szArrTypeUserLoad.Add(hggettext("��������(V)"));
		m_szArrTypeUserLoad.Add(hggettext("��������(M)"));
		m_szArrTypeUserLoad.Add(hggettext("��������(V)"));
		m_szArrTypeUserLoad.Add(hggettext("��������(Wx)"));
		m_szArrTypeUserLoad.Add(hggettext("��������(Wy)"));
		m_szArrTypeUserLoad.Add(hggettext("��������(Wz)"));
	}
	else
	{
		m_sArrTabName.Add(_T("�������߼���"));		m_nTabSelectLoad	 = nTab++;
		m_sArrTabName.Add(_T("�����������"));		m_nTabCombineUseLoad = nTab++;
		m_sArrTabName.Add(_T("�����������"));		m_nTabCombineUltLoad = nTab++;

		// ��ũ��Ʈ ������� + KCI 2007���� ������
		if(pStd->m_nTypeConditionApply != DESIGN_CONDITION_CONCRETE || pStd->m_nTypeKciCode != 1)
		{
			m_sArrTabName.Add(_T("�����������(�ִ����)"));
			m_nTabCombineEccLoad = nTab++;
		}

		CRcBridgeApp *pBri = m_pIn->GetRcBridge();
		if(pBri->GetTypeDesingEQ() != 0 && (pBri && pBri->IsRcSlab() == FALSE))
		{
			m_sArrTabName.Add(_T("������������"));
			m_nTabCombineEQLoad  = nTab++;
		}

		m_sArrTabName.Add(_T("����"));				m_nTabLsdLoad	 = nTab++;
	}

	InitTabCtrl();
	InitComboCtrl();

	SetResize(IDC_TAB,			   SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID,			   SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_ADD_COMBINE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_REMOVE_COMBINE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_APPLY_SAMEALL, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_APPLY_MIRROR2, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_ADD_USER,	SZ_BOTTOM_LEFT,	SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_REMOVE_USER,	SZ_BOTTOM_LEFT,	SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_SHOW_USELOAD, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_AUTO_COMBINE, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_AUTO_COMBIN_INIT, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_ETA_COMBIN_EACH, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_TAB_LSDETA, SZ_TOP_LEFT,		SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_RESET_DETAILCOMBOA, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	SetHyperHelp(m_pIn->GetManualPath());

	if (pBri && pBri->m_bUnderGroundDesignHeight)
	{
		pBri->m_dUnderGroundDesignHeight = pBri->CalcUnderGroundDesignHeight();
	}
	if(pBri && m_bDesignLsd)
	{
		m_bAutoCombinInit = !(pBri->m_plsdManager->IsAllowUserInput());
		m_bEtaCombinEach = pBri->m_bEtaCombinEach;
	}

	// (ARCBRIDGE-2761) �۾��ϴµ��� �ӽ÷� ���Ƶд�.
	// GetDlgItem(IDC_CHECK_ETA_COMBIN_EACH)->EnableWindow(FALSE);

	SetGridData();
	UpdateData(FALSE);
}

void CInputGenLoad::InitComboCtrl()
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

void CInputGenLoad::InitTabCtrl()
{
	//	m_Tab.RemoveAllTabs();
	m_Tab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_Tab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	long i = 0; for(i = 0; i < m_sArrTabName.GetSize(); i++)
		m_Tab.InsertItem(i, m_sArrTabName[i]);

	int nTab = m_Tab.GetCurSel();
	m_Tab.SetCurSel(nTab);

	//Lsd Eta�� Tab
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar *pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	long ixItem(0);
	m_ctlTabLsdEta.DeleteAllItems();
	for(long ix=0; ix<ePartCount; ++ix)
	{
		if(pBri->IsUsePart((EPartOfBridge)ix, 0) == FALSE)
			continue;

		m_ctlTabLsdEta.InsertItem(++ixItem, pBri->GetStringPartMerge((EPartOfBridge)ix));
	}

	//	m_Tab.SetOwner(this->GetSafeOwner());
}

void CInputGenLoad::SetGridData()
{
	m_Grid.SetType(0);
	m_Grid.SetCellSize(-1, -1);

	int nTab = m_Tab.GetCurSel();

	if(nTab == m_nTabCombineUseLoad)	SetGridDataCombineNormal(COMBO_SERVICE);
	if(nTab == m_nTabCombineUltLoad)	SetGridDataCombineNormal(COMBO_ULTIMATE);
	if(nTab == m_nTabCombineEccLoad)	SetGridDataCombineNormal(COMBO_ECCENTRIC);
	if(nTab == m_nTabCombineEQLoad)		SetGridDataCombineEQLoad();
	if(nTab == m_nTabSelectLoad)		SetGridDataSelectLoad();
	if(nTab == m_nTabLsdLoad)			SetGridData_LsdLoad();

	if(m_bDesignLsd)
	{
		if(nTab == m_nTabLsdLoadFactor)		SetGridData_LsdLoadFactor();
		if(nTab == m_nTabLsdComboLimit)		SetGridData_LsdCombo(eLSD_COMBINATION_LIMIT);
		if(nTab == m_nTabLsdComboExtreme)	SetGridData_LsdCombo(eLSD_COMBINATION_EXTREME);
		if(nTab == m_nTabLsdComboService)	SetGridData_LsdCombo(eLSD_COMBINATION_SERVICE);
		if(nTab == m_nTabLsdComboFatigue)	SetGridData_LsdCombo(eLSD_COMBINATION_FATIGUE);
		if(nTab == m_nTabLsdModifiedFactor)	SetGridData_LsdModifiedFactor();
		if(nTab == m_nTabLsdUserLoadNormal || nTab == m_nTabLsdUserLoadEQ || nTab == m_nTabLsdUserLoadEQ2 || nTab == m_nTabLsdUserLoadEQ3 || nTab == m_nTabLsdUserLoadEQ4)		SetGridData_LsdUserLoad();
	}
}

void CInputGenLoad::SetGridDataCombineNormal(long nTypeCombine)
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	long nSizeRow = pBri->m_MatComboUse.GetRow() + 1;
	if(nTypeCombine == COMBO_ULTIMATE)
	{
		nSizeRow = pBri->m_MatComboUlt.GetRow() + 1;
	}
	if(nTypeCombine == COMBO_ECCENTRIC)
	{
		nSizeRow = pBri->m_MatComboEcc.GetRow() + 1;
	}

	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(1,1,1,1);

	long nRow = 0;
	long nCol = 0;
	// TITLE
	BOOL bApplyLoad[BRIDGE_LOAD_BASIC] = {FALSE,};

	long nIdxCol = 0;
	m_Grid.AddCell(nRow,nCol++,"����");
	if(pBri->m_dWLoadNormal & LOAD_CASE_DEAD)		{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"��������");} nIdxCol++;
	if(pBri->m_dWLoadNormal & LOAD_CASE_SOIL)		{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"�������");} nIdxCol++;
	if(pBri->m_dWLoadNormal & LOAD_CASE_LIVE_FULL)	{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"\n����\nȰ����");} nIdxCol++;
	// (35244) ���� Ȱ���� ������.. ���� ���� �����ߴ����� �˼� ����. ���յ� ����
	if(FALSE /*pBri->m_dWLoadNormal & LOAD_CASE_LIVE_ECC*/)	{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"\n����\nȰ����");} nIdxCol++;
	if(pBri->m_dWLoadNormal & LOAD_CASE_TANK)		{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"\n������\n����");} nIdxCol++;
	if(pBri->m_dWLoadNormal & LOAD_CASE_TOAP_BOTH)	{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"�����");} nIdxCol++;
	if(pBri->m_dWLoadNormal & LOAD_CASE_TOAP_ONE)	{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"�����");} nIdxCol++;
	if(pBri->m_dWLoadNormal & LOAD_CASE_TEMP1)		{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"\n�µ�����\n(+)");} nIdxCol++;
	if(pBri->m_dWLoadNormal & LOAD_CASE_TEMP2)		{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"\n�µ�����\n(-)");} nIdxCol++;
	if(pBri->m_dWLoadNormal & LOAD_CASE_SHRINK)		{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"��������");} nIdxCol++;
	if(pBri->m_dWLoadNormal & LOAD_CASE_SUBSID)		{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"����ħ��");} nIdxCol++;
	if(pBri->m_dWLoadNormal & LOAD_CASE_SB)			{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"\n�õ� ��\n��������");} nIdxCol++;
	if(pBri->m_dWLoadNormal & LOAD_CASE_LR)			{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"\n��뷹��\n������");} nIdxCol++;
	if(pBri->m_dWLoadNormal & LOAD_CASE_EQ)			{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"��������");} nIdxCol++;
	if(pBri->m_dWLoadNormal & LOAD_CASE_WATERHORI)		{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"�������");} nIdxCol++;
	if(pBri->m_dWLoadNormal & LOAD_CASE_WATERVERT)	{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"��������");} nIdxCol++;
	BOOL bEarthAnchor = (pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR && pBri->m_bApplyAnchorForce);
	if(bEarthAnchor)
	{
		m_Grid.AddCell(nRow,nCol++,"\n�η¹���\n��Ŀ");
	}
	if(pBri->IsBoxType() && pBri->m_bApplyGroundWaterLevel)
	{
		m_Grid.AddCell(nRow,nCol++,"\n���ϼ�\n���");
	}
	m_Grid.AddCell(nRow,nCol++,"\n3D\nPlate");

	CString str = _T("");
	UINT nFormat = DT_CENTER|DT_VCENTER;
	for(nRow=1; nRow<nSizeRow; nRow++)
	{
		nCol = 0;
		str.Format("SV%02d",nRow);
		if(nTypeCombine == COMBO_ULTIMATE)
		{
			str.Format("UL%02d",nRow);
		}
		if(nTypeCombine == COMBO_ECCENTRIC)
		{
			str.Format("UE%02d",nRow);
		}
		m_Grid.AddCell(nRow,nCol++,str);
		nIdxCol = 1;
		for(nCol=1; nCol<BRIDGE_LOAD_BASIC+1; nCol++)
		{
			if(bApplyLoad[nCol-1]==FALSE) continue;
			CString strComent = _T("");
			if(nCol==2 && pStd->m_nTypeConditionApply == DESIGN_CONDITION_CONCRETE && pStd->m_nTypeKciCode == 1 && pBri->m_bUnderGroundRahmen)
			{
				double dH = toM(pBri->m_dUnderGroundDesignHeight);
				double dFactor = pBri->m_MatComboUse(nRow-1,nCol-1);
				if(nTypeCombine == COMBO_ULTIMATE)
				{
					dFactor = pBri->m_MatComboUlt(nRow-1,nCol-1);
				}
				if(nTypeCombine == COMBO_ECCENTRIC)
				{
					dFactor = pBri->m_MatComboEcc(nRow-1,nCol-1);
				}

				if(dH <= 2.0)
					strComent.Format("%.3f x ��H, ��H = %.3f (h��2m)", dFactor, pBri->m_dFactorComboVLoad);
				else
					strComent.Format("%.3f x ��H, ��H = 1.05-0.025x%.3f = %.3f �� 0.875(h>2m)", dFactor/pBri->m_dFactorComboVLoad, dH, pBri->m_dFactorComboVLoad);
			}

			if(nTypeCombine == COMBO_SERVICE)
			{
				m_Grid.AddCell(nRow,nIdxCol,&pBri->m_MatComboUse(nRow-1,nCol-1),DT_CENTER, CELL_LINE_NO, strComent);
			}
			else if(nTypeCombine == COMBO_ULTIMATE)
			{
				m_Grid.AddCell(nRow,nIdxCol,&pBri->m_MatComboUlt(nRow-1,nCol-1),DT_CENTER, CELL_LINE_NO, strComent);
			}
			else if(nTypeCombine == COMBO_ECCENTRIC)
			{
				m_Grid.AddCell(nRow,nIdxCol,&pBri->m_MatComboEcc(nRow-1,nCol-1),DT_CENTER, CELL_LINE_NO, strComent);
			}

			m_Grid.SetCellDec(CELL_DEC_2);
			m_Grid.SetCellPrintZero(FALSE);
			nIdxCol++;
		}
		if(bEarthAnchor)
		{
			m_Grid.AddCell(nRow,nIdxCol++, &pBri->m_dFactorAnchor[nTypeCombine][nRow-1],DT_CENTER, CELL_LINE_NO);
			m_Grid.SetCellDec(CELL_DEC_2);
			m_Grid.SetCellPrintZero(FALSE);
		}
		if(pBri->IsBoxType() && pBri->m_bApplyGroundWaterLevel)
		{
			m_Grid.AddCell(nRow,nIdxCol++, &pBri->m_bApplyGroundWater[nTypeCombine][nRow-1],nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		}
		m_Grid.AddCell(nRow,nIdxCol++, &pBri->m_bApplyCombo3D[nTypeCombine][nRow-1],nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	}
	m_Grid.GetCellCount(nRow, nCol);
	m_Grid.SetRows(nRow);
	m_Grid.SetCols(nCol);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0,50);
	m_Grid.SetColumnWidthAll(60);
	m_Grid.SetColumnWidth(nCol-1,50);
	if(pBri->IsBoxType() && pBri->m_bApplyGroundWaterLevel)
	{
		m_Grid.SetColumnWidth(nCol-2,50);
	}
}

void CInputGenLoad::SetGridDataCombineEQLoad()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	long nSizeRow = pBri->m_MatComboEQ.GetRow() + 1;

	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(1,1,1,1);

	long nRow = 0;
	long nCol = 0;
	// TITLE
	BOOL bApplyLoad[BRIDGE_LOAD_BASIC] = {FALSE,};

	long nIdxCol = 0;
	m_Grid.AddCell(nRow,nCol++,"����");
	//	m_Grid.AddCell(nRow,nCol++,"\n3D\nPlate");
	if(pBri->m_dWLoadEQ & LOAD_CASE_DEAD)		{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"��������");} nIdxCol++;
	if(pBri->m_dWLoadEQ & LOAD_CASE_SOIL)		{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"�������");} nIdxCol++;
	if(pBri->m_dWLoadEQ & LOAD_CASE_LIVE_FULL)	{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"\n����\nȰ����");} nIdxCol++;
	// (35244) ���� Ȱ���� ������.. ���� ���� �����ߴ����� �˼� ����. ���յ� ����
	if(FALSE/*pBri->m_dWLoadEQ & LOAD_CASE_LIVE_ECC*/)	{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"\n����\nȰ����");} nIdxCol++;
	if(pBri->m_dWLoadEQ & LOAD_CASE_TANK)		{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"\n������\n����");} nIdxCol++;
	if(pBri->m_dWLoadEQ & LOAD_CASE_TOAP_BOTH)	{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"���");} nIdxCol++;
	if(pBri->m_dWLoadEQ & LOAD_CASE_TOAP_ONE)	{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"�����");} nIdxCol++;
	if(pBri->m_dWLoadEQ & LOAD_CASE_TEMP1)		{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"\n�µ�����\n(+)");} nIdxCol++;
	if(pBri->m_dWLoadEQ & LOAD_CASE_TEMP2)		{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"\n�µ�����\n(-)");} nIdxCol++;
	if(pBri->m_dWLoadEQ & LOAD_CASE_SHRINK)		{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"��������");} nIdxCol++;
	if(pBri->m_dWLoadEQ & LOAD_CASE_SUBSID)		{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"����ħ��");} nIdxCol++;
	if(pBri->m_dWLoadEQ & LOAD_CASE_SB)			{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"\n�õ� ��\n��������");} nIdxCol++;
	if(pBri->m_dWLoadEQ & LOAD_CASE_LR)			{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"\n��뷹��\n������");} nIdxCol++;
	if(pBri->m_dWLoadEQ & LOAD_CASE_EQ)			{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"��������");} nIdxCol++;
	if(pBri->m_dWLoadEQ & LOAD_CASE_WATERHORI)		{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"�������");} nIdxCol++;
	if(pBri->m_dWLoadEQ & LOAD_CASE_WATERVERT)	{bApplyLoad[nIdxCol]=TRUE; m_Grid.AddCell(nRow,nCol++,"��������");} nIdxCol++;
	BOOL bEarthAnchor = (pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR && pBri->m_bApplyAnchorForce);
	if(bEarthAnchor)
	{
		m_Grid.AddCell(nRow,nCol++,"\n�η¹���\n��Ŀ");
	}
	if(pBri->IsBoxType() && pBri->m_bApplyGroundWaterLevel)
	{
		m_Grid.AddCell(nRow,nCol++,"\n���ϼ�\n���");
	}

	CString str = _T("");
	UINT nFormat = DT_CENTER|DT_VCENTER;
	for(nRow=1; nRow<nSizeRow; nRow++)
	{
		nCol = 0;
		str.Format("EQ%02d",nRow);
		m_Grid.AddCell(nRow,nCol++,str);
		//		m_Grid.AddCell(nRow,nCol++,&pBri->m_bApplyCombo3D[COMBO_EARTHQUAKE][nRow-1],nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		nIdxCol = 1;
		for(nCol=1; nCol<BRIDGE_LOAD_BASIC+1; nCol++)
		{
			if(bApplyLoad[nCol-1]==FALSE) continue;
			CString strComent = _T("");
			if(nCol==3 && pStd->m_nTypeConditionApply == DESIGN_CONDITION_CONCRETE && pStd->m_nTypeKciCode == 1 && pBri->m_bUnderGroundRahmen)
			{
				double dH = toM(pBri->m_dUnderGroundDesignHeight);
				double dFactor = pBri->m_MatComboEQ(nRow-1,nCol-1);
				if(dH <= 2.0) strComent.Format("%.3f x ��H, ��H = %.3f (h��2m)", dFactor, pBri->m_dFactorComboVLoad);
				else strComent.Format("%.3f x ��H, ��H = 1.05-0.025x%.3f = %.3f �� 0.875(h>2m)", dFactor/pBri->m_dFactorComboVLoad, dH, pBri->m_dFactorComboVLoad);
			}

			m_Grid.AddCell(nRow,nIdxCol,&pBri->m_MatComboEQ(nRow-1,nCol-1),DT_CENTER, CELL_LINE_NO, strComent);
			m_Grid.SetCellDec(CELL_DEC_2);  m_Grid.SetCellPrintZero(FALSE);
			nIdxCol++;
		}
		if(bEarthAnchor)
		{
			m_Grid.AddCell(nRow,nIdxCol++, &pBri->m_dFactorAnchor[COMBO_EARTHQUAKE][nRow-1],DT_CENTER, CELL_LINE_NO);
			m_Grid.SetCellDec(CELL_DEC_2);
			m_Grid.SetCellPrintZero(FALSE);
		}
		if(pBri->IsBoxType() && pBri->m_bApplyGroundWaterLevel)
		{
			m_Grid.AddCell(nRow,nIdxCol++, &pBri->m_bApplyGroundWater[COMBO_EARTHQUAKE][nRow-1],nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
		}
	}
	m_Grid.GetCellCount(nRow, nCol);
	m_Grid.SetRows(nRow);
	m_Grid.SetCols(nCol);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetRowHeight(0,50);
	m_Grid.SetColumnWidthAll(60);
	if(pBri->IsBoxType() && pBri->m_bApplyGroundWaterLevel)
	{
		m_Grid.SetColumnWidth(nCol-1,50);
	}
}

void CInputGenLoad::SetGridDataSelectLoad()
{
	if(m_bDesignLsd)
		SetGridDataSelectLoadLsd();
	else
		SetGridDataSelectLoadUsd();
}

/*
#define BRIDGE_LOAD_DEAD		 0 // ��������, �������
#define BRIDGE_LOAD_LIVE_FULL	 1 // Ȱ���� (����)
#define BRIDGE_LOAD_LIVE_ECC	 2 // Ȱ���� (����)
#define BRIDGE_LOAD_TANK		 3 // ��ũ �� Ư�� ����������
#define BRIDGE_LOAD_TOAP_BOTH	 4 // �����
#define BRIDGE_LOAD_TOAP_ONE	 5 // �����
#define BRIDGE_LOAD_TEMP1		 6 // �µ�����(+)
#define BRIDGE_LOAD_TEMP2		 7 // �µ�����(-)
#define BRIDGE_LOAD_SHRINK		 8 // ��������
#define BRIDGE_LOAD_SUBSID		 9 // ����ħ��
#define BRIDGE_LOAD_SB			10 // �õ� �� ��������
#define BRIDGE_LOAD_LR			11 // ��뷹�� ������
#define BRIDGE_LOAD_EQ			12 // ��������
*/
void CInputGenLoad::SetGridDataSelectLoadUsd()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(BRIDGE_LOAD_BASIC+1, 2, 1, 1); // +1 - Ÿ��Ʋ

	BOOL bEarthQuakeDesign = pBri->GetTypeDesingEQ() != 0 ? TRUE : FALSE;

	CStringArray sArrLoad;
	sArrLoad.Add("��������");
	sArrLoad.Add("�������");
	sArrLoad.Add("Ȱ����");
	sArrLoad.Add("���� Ȱ����");
	sArrLoad.Add("��ũ �� Ư�� ����������");
	sArrLoad.Add("�����");
	sArrLoad.Add("�����");
	sArrLoad.Add("�µ�����(+)");
	sArrLoad.Add("�µ�����(-)");
	sArrLoad.Add("��������");
	sArrLoad.Add("����ħ��");
	sArrLoad.Add("�õ� �� ��������");
	sArrLoad.Add("��뷹�� ������");
	sArrLoad.Add("��������");
	sArrLoad.Add("�������");
	sArrLoad.Add("��������");
	sArrLoad.Add("������ ��������");

	long nRow  = 0;
	long nCol  = 0;
	long nLoad = 0;
	m_Grid.AddCell(nRow,nCol,"�� ��");
	m_Grid.AddCell(nRow++,nCol+1,"������");

	UINT nFormat = DT_CENTER|DT_VCENTER;
	for(nLoad=0; nLoad<BRIDGE_LOAD_BASIC; nLoad++)
	{
		if(bEarthQuakeDesign == FALSE && nLoad == BRIDGE_LOAD_EQ)
		{
			continue;
		}
		// (35244) ���� Ȱ���� ������.. ���� ���� �����ߴ����� �˼� ����. ���յ� ����
		if(nLoad == BRIDGE_LOAD_LIVE_ECC)
		{
			continue;
		}
		if(pStd->IsRailDesign() == FALSE && (nLoad == BRIDGE_LOAD_SB || nLoad == BRIDGE_LOAD_LR))
		{
			continue;
		}
		if(nLoad == BRIDGE_LOAD_SUBSID && (pBri->IsBoxType() || pBri->IsRcSlab()))		// (ARCBRIDGE-3642) �����걳 �϶� ����ħ�� ���� ���� ���ϵ��� ����
		{
			pBri->m_bApplyLoad[nLoad] = FALSE;
			continue;
		}
		if(nLoad == BRIDGE_LOAD_WATERHORI || nLoad == BRIDGE_LOAD_WATERVERT)
		{
			if(pBri->IsBoxType() == FALSE || pBri->m_bApplyGroundWaterLevel == FALSE)
				continue;
		}

		m_Grid.AddCell(nRow,nCol,sArrLoad.GetAt(nLoad));

		// (40855) ���ʰ� �ִ� Ÿ�Կ����� ���������� ���� ������..
		BOOL bEQLoad = (nLoad == BRIDGE_LOAD_EQ && !(pBri->IsRcSlab() || pBri->IsBoxType()))? TRUE : FALSE;
		if((pBri->m_bUnderGroundRahmen && nLoad == BRIDGE_LOAD_TANK) || bEQLoad)
			m_Grid.AddCell(nRow,nCol+1,&pBri->m_bApplyLoad[nLoad],nFormat,CELL_READONLY, "", CELL_TYPE_CHECK);
		else
			m_Grid.AddCell(nRow,nCol+1,&pBri->m_bApplyLoad[nLoad],nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);

		nRow++;
	}

	// (ARCBRIDGE-3634) '������ Ȱ����'���� '������ ��������'���� ����, ���߶���϶��� 
	if(bEarthQuakeDesign) // && pBri->IsBoxType() == FALSE)
	{
		m_Grid.AddCell(nRow,nCol, (pBri->m_bUnderGroundRahmen && pBri->m_bCalcOpen == FALSE) ? _T("������ ��������") : _T("������ �̵�����"));
		m_Grid.AddCell(nRow,nCol+1, &pBri->m_bApplyLiveLoadEQ, nFormat, CELL_LINE_NO, "", CELL_TYPE_CHECK);
		nRow++;
	}
	else
	{
		pBri->m_bApplyLiveLoadEQ = FALSE;
	}
	if(pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR)
	{
		m_Grid.AddCell(nRow,nCol,_T("�η¹�����Ŀ"));
		m_Grid.AddCell(nRow,nCol+1, &pBri->m_bApplyAnchorForce, nFormat, CELL_LINE_NO, "", CELL_TYPE_CHECK);
		nRow++;
	}

	m_Grid.SetColumnWidth(0, 200);
	m_Grid.SetColumnWidth(1, 80);
}

void CInputGenLoad::SetGridDataSelectLoadLsd()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	m_Grid.SetGridDefault(4, 4, 2, 1);
	m_Grid.SetLsdManager(pBri->m_plsdManager);
	m_Grid.SetGridInitDesignLoad(m_bShowUseLoad);
	
	m_Grid.DeleteColumn(4);
	m_Grid.DeleteColumn(2);

	m_Grid.SetTextMatrix(0, 2, "��������");
}

void CInputGenLoad::SetDataInit()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	m_Grid.UpdateData(FALSE);
	m_Grid.SetRedraw(TRUE, TRUE);

	long nRows = 0;
	long nCols = 0;
	m_Grid.GetCellCount(nRows, nCols);	

	int nTab = m_Tab.GetCurSel();
	if (nTab == m_nTabLsdLoad)
	{
		if (pBri->m_bUnderGroundRahmen && pBri->m_bCalcOpen == FALSE && pBri->m_nUnderGroundDesignType == 1)
		{
			m_Grid.SetCellType((int)m_xyButtonRailLoad.y, (int)m_xyButtonRailLoad.x, CT_BUTTON);
		}
	}

	if(m_bDesignLsd)
	{
		if( nTab == m_nTabLsdModifiedFactor)
		{
			//������ ���� �ʴ� �κ� ����
			m_Grid.SetFiexedRows(2);
			m_Grid.SetEditMask(0, 1, _T("��D"));
			m_Grid.SetEditMask(0, 2, _T("��R"));
			m_Grid.SetEditMask(0, 3, _T("��I"));
			m_Grid.SetEditMask(0, 4, _T("��i"));
		}
		else if((nTab == m_nTabLsdUserLoadNormal || nTab == m_nTabLsdUserLoadEQ || nTab == m_nTabLsdUserLoadEQ2 || nTab == m_nTabLsdUserLoadEQ3 || nTab == m_nTabLsdUserLoadEQ4) && nCols > 5)
		{
			//		m_Grid.SetMergeRow(0, 1, 1);
			m_Grid.SetMergeCol(nRows-1, 1, 2);
			m_Grid.SetMergeCol(nRows-1, 3, 5);
			m_Grid.SetRedraw();
		}
	}

	BOOL bCombine = (nTab==m_nTabCombineUseLoad || nTab==m_nTabCombineUltLoad ||
		nTab==m_nTabCombineEccLoad || nTab==m_nTabCombineEQLoad ||
		nTab==m_nTabLsdComboLimit || nTab==m_nTabLsdComboExtreme ||
		nTab==m_nTabLsdComboService || nTab == m_nTabLsdComboFatigue) ? TRUE : FALSE;
	BOOL bUserLoad = (nTab == m_nTabLsdUserLoadNormal || nTab == m_nTabLsdUserLoadEQ || nTab == m_nTabLsdUserLoadEQ2 || nTab == m_nTabLsdUserLoadEQ3 || nTab == m_nTabLsdUserLoadEQ4);

	m_btnAddCombine.EnableWindow(bCombine || bUserLoad);
	m_btnRemoveCombine.EnableWindow(bCombine || bUserLoad);
	m_btnMirror.EnableWindow(pBri->m_nSeparBri!=0);
	m_btnLsdComboReset.ShowWindow((nTab==m_nTabLsdLoadFactor) ? SW_SHOW : SW_HIDE);
	m_ctlTabLsdEta.ShowWindow((nTab==m_nTabLsdModifiedFactor) ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_CHECK_AUTO_COMBINE)->EnableWindow(nTab==m_nTabSelectLoad);
	GetDlgItem(IDC_CHECK_SHOW_USELOAD)->EnableWindow(nTab==m_nTabSelectLoad);
	GetDlgItem(IDC_CHECK_SHOW_USELOAD)->ShowWindow(m_bDesignLsd ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_CHECK_AUTO_COMBIN_INIT)->EnableWindow(nTab==m_nTabLsdLoadFactor);

	m_btnAddCombine.SetWindowText(bUserLoad ? hggettext("����������߰�") : hggettext("�������� �߰�"));
	m_btnRemoveCombine.SetWindowText(bUserLoad ? hggettext("��������߻���") : hggettext("�������� ����"));

	m_btnAddUser.ShowWindow(bUserLoad ? SW_SHOW : SW_HIDE);
	m_btnRemoveUser.ShowWindow(bUserLoad ? SW_SHOW : SW_HIDE);

	if(m_bDesignLsd && m_bAutoCombinInit)
	{
		if(bUserLoad == FALSE)
		{
			m_btnLsdComboReset.EnableWindow(FALSE);
			m_btnAddCombine.EnableWindow(FALSE);
			m_btnRemoveCombine.EnableWindow(FALSE);
		}
	}
	else
	{
		m_btnLsdComboReset.EnableWindow(TRUE);
	}
}

void CInputGenLoad::SetDataDefault()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;

	int nTab = m_Tab.GetCurSel();
	m_bAutoCombinInit = TRUE;				// (ARCBRIDGE-3201) �޺� �⺻�� �ڵ����� üũ�� �⺻�� ����

	for(long bri = 0; bri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		CRcBridgeRebar *pBri = pStd->GetBridgeRebar(bri);
		BOOL bFirst	= IsFirst(bri);
		// (ARCBRIDGE-2288) ������ �ԷµǾ��ִ� ������ ���������� �ʱ�ȭ��Ű�� �ȵȴ�.
		if(bFirst && pStd->m_nFlag > 0 && pStd->m_nFlag < 21)
		{
			if(pBri->IsFirstDefault(IDD_GEN_DESIGN_VARIABLE) == FALSE)
			{
				bFirst = FALSE;
			}
		}

		pBri->SetDefaultedState(GetIDD(), 0x00000001);
		if(!bFirst && !IsClickDefaultButton(bri)) continue;

		if(m_bDesignLsd)
		{
			if(bFirst || nTab == m_nTabSelectLoad)		{pBri->SetDataDefaultSelectLoadLsd();}
			if(bFirst || nTab == m_nTabLsdLoadFactor)	pBri->SetDataDefaultLoadFactor();
			if(bFirst || nTab == m_nTabLsdComboLimit)	pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_LIMIT);
			if(bFirst || nTab == m_nTabLsdComboExtreme)	pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_EXTREME);
			if(bFirst || nTab == m_nTabLsdComboService)	pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_SERVICE);
			if(bFirst || nTab == m_nTabLsdComboFatigue)	pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_FATIGUE);
			if(bFirst || nTab == m_nTabLsdModifiedFactor)	pBri->SetDataDefaultModifiedFactor(-1);
			if(bFirst || nTab == m_nTabLsdLoad)			pBri->SetDataDefaultLsdLoad();

			pBri->m_plsdManager->SetAllowUserInput(!m_bAutoCombinInit);
		}
		else
		{
			if(bFirst || nTab == m_nTabSelectLoad)		pBri->SetDataDefaultSelectLoad(); // COMBO ����Ʈ���� �Ǿ�� ��...
			if(bFirst || nTab == m_nTabCombineUseLoad)	pBri->SetDataDefaultCombineUseLoad();
			if(bFirst || nTab == m_nTabCombineUltLoad)	pBri->SetDataDefaultCombineUltLoad();
			if(bFirst || nTab == m_nTabCombineEccLoad)	pBri->SetDataDefaultCombineEccLoad();
			if(bFirst || nTab == m_nTabCombineEQLoad)	pBri->SetDataDefaultCombineEQLoad();
		}

		pBri->SyncBridge_DesignData();
	}

	SetGridData();
	SetDataInit();
	UpdateData(FALSE);
	m_pIn->m_pDoc->SetModifiedFlag();
}

void CInputGenLoad::SetDataSave()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	m_Grid.UpdateData(TRUE);
	UpdateData(TRUE);

	int nTab = m_Tab.GetCurSel();
	if(nTab==m_nTabSelectLoad && m_bDesignLsd==FALSE)
	{
		DWORD dWNormal = pBri->m_dWLoadNormal;
		DWORD dWEQ     = pBri->m_dWLoadEQ;

		pBri->SetCombineNormalDWORD();
		pBri->SetCombineEarthQuakeDWORD();
		if(m_bAutoDefaultCombine && dWNormal!=pBri->m_dWLoadNormal)
		{
			pBri->SetDataDefaultCombineUltLoad();
			pBri->SetDataDefaultCombineUseLoad();
			pBri->SetDataDefaultCombineEccLoad();
		}
		if(m_bAutoDefaultCombine && dWEQ!=pBri->m_dWLoadEQ)
		{
			pBri->SetDataDefaultCombineEQLoad();
		}
	}

	if(m_bDesignLsd && nTab == m_nTabLsdLoadFactor)
	{
		SetGridData();
		SetDataInit();
	}

	if (nTab == m_nTabLsdLoad)
	{
		if (pBri->m_bUnderGroundDesignHeight)
		{
			pBri->m_dUnderGroundDesignHeight = pBri->CalcUnderGroundDesignHeight();
		}

		if (pBri->m_nUnderGroundDesignType == 0 && (pBri->m_nUnderGroundDesignSelect == 0 || pBri->m_nUnderGroundDesignSelect == 1))
		{
			double LV[12] = {5.1,3.9,2.1,1.7,1.5,1.5,1.5,1.5,1.5,1.2,1.1,0.9};
			if(pBri->m_nUnderGroundDesignSelect==1) LV[11] = 1.0;

			long nSelect = 1;
			double HeightUD = toM(pBri->m_dUnderGroundDesignHeight);
			if(HeightUD<1.5)  nSelect = 0;
			else if(HeightUD<2.0)  nSelect = 1;
			else if(HeightUD<2.5)  nSelect = 2;
			else if(HeightUD<3.0)  nSelect = 3;
			else if(HeightUD<4.0)  nSelect = 4;
			else if(HeightUD<5.0)  nSelect = 5;
			else if(HeightUD<6.0)  nSelect = 6;
			else if(HeightUD<7.0)  nSelect = 7;
			else if(HeightUD<8.0)  nSelect = 8;
			else if(HeightUD<9.0)  nSelect = 9;
			else if(HeightUD<10.0) nSelect = 10;
			else if(HeightUD>=10.0) nSelect = 11;

			pBri->m_dUnderGroundDesignLiveAlways = LV[nSelect];
		}
		// ARCBRIDGE-3363
		if(pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
		{
			pBri->m_bApplyGroundWaterLevel = FALSE;
			pBri->m_dGroundWaterLevel = 0;
		}
	}
	if(nTab == m_nTabLsdUserLoadNormal || nTab == m_nTabLsdUserLoadEQ || nTab == m_nTabLsdUserLoadEQ2 || nTab == m_nTabLsdUserLoadEQ3 || nTab == m_nTabLsdUserLoadEQ4)
	{
		CStringArray sArr; //Lsd ���߱���
		pBri->m_plsdManager->GetStringArrayLsdLoadCaseKindForUserLoad(sArr);

		long nIndex = nTab - m_nTabLsdUserLoadNormal;
		long nSizeUserLoad = pBri->m_vpUserLoadSet[nIndex].size();
		for(long nCase = 0; nCase < nSizeUserLoad; nCase++)
		{
			long nIndexLoadCase = pBri->m_vpUserLoadSet[nIndex][nCase]->m_nTypeLoadCase;
			if(nIndexLoadCase < 0 || nIndexLoadCase >= sArr.GetSize()) continue;

			pBri->m_vpUserLoadSet[nIndex][nCase]->m_sTypeLoadCase = sArr.GetAt(nIndexLoadCase);
		}
	}

	pBri->SyncBridge_DesignData();

	m_pIn->m_pDoc->SetModifiedFlag();
	CInputDialogBase::SetDataSave();
}

void CInputGenLoad::SetGridData_LsdLoadFactor()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	m_Grid.SetLsdManager(pBri->m_plsdManager);
	m_Grid.SetGridInitLoadFactor();
}

void CInputGenLoad::SetGridData_LsdCombo(long nTypeLsdCombo)
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	m_Grid.SetLsdManager(pBri->m_plsdManager);
	m_Grid.SetGridInitLoadCase(static_cast<ELSDLoadGroupKind>(nTypeLsdCombo),  eLSD_LOADCASE_DIR_BRI);
}

void CInputGenLoad::SetGridData_LsdModifiedFactor()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	long nIndex = m_ctlTabLsdEta.GetCurFocus();

	m_Grid.SetLsdManager(pBri->m_plsdManager);
	m_Grid.SetGridLoadModified(nIndex);
}

void CInputGenLoad::SetGridData_LsdLoad()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	BOOL bLsd = pStd->IsLsdDesign();
	BOOL bBoxType = pBri->IsBoxType();

	m_Grid.SetGridDefault(7, 4, 1, 2);

	CStringArray sArr;
	HGINT32 iRow = 0;
	HGINT32 iCol = 0;
	m_Grid.AddCell(iRow, iCol++, hggettext("��  ��"));
	m_Grid.AddCell(iRow, iCol++, hggettext("��  ��"));
	m_Grid.AddCell(iRow, iCol++, hggettext("������"));
	m_Grid.AddCell(iRow, iCol++, hggettext("������"));
	m_Grid.AddCell(iRow, iCol, hggettext("����"));
	iRow++;
	iCol = 0;

	HGINT32 iRowLiveLoad = -1;
	HGINT32 iRowUnderGroundRahmen = -1;
	HGINT32 iRowTemperatureSlopeT = -1;
	
	pBri->m_dCorrectionFactor_LiveLoadAlways = bLsd ? 1.33 : 1.00;
	if(bLsd)
	{
		m_Grid.AddCell(iRow, iCol++, hggettext("����ħ�� ���밪"));
		m_Grid.AddCell(iRow, iCol++, hggettext("����ħ�� ���밪"));
		m_Grid.AddCell(iRow, iCol++, &pBri->m_dDepthApplySubSid, DT_RIGHT);m_Grid.SetCellDec(CELL_DEC_1);
		m_Grid.AddCell(iRow, iCol+1, hggettext("mm"), DT_LEFT, CELL_READONLY);
		iRow++;
		iCol = 0;

		// Ȱ���� - ��ȿ�� �����
		sArr.RemoveAll();
		if(pStd->IsDesignKDS2019())
		{
			sArr.Add("� ��Ʈ�� ��[KDS 24 10 11 4.6.2.4]");
			sArr.Add("�ܼ��� �� ������[KDS 24 10 11 4.6.4]");
		}
		else
		{
			sArr.Add("� ��Ʈ�� ��(4.6.4)");
			sArr.Add("�ܼ��� �� ������(4.6.2.4)");
		}

		iRowLiveLoad = iRow;
		m_Grid.AddCell(iRow, iCol++, hggettext("Ȱ����(LL)"));
		m_Grid.AddCell(iRow, iCol++, hggettext("��ȿ��(E) �����"));
		m_Grid.AddCell(iRow, iCol++, &pBri->m_nLoadEffeciveW, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(iRow, iCol+1, _T(""), DT_LEFT, CELL_READONLY);
		iRow++;
		iCol = 0;

		// Ȱ���� - � ��Ʈ�� ���� �簢 ���
		sArr.RemoveAll();
		sArr.Add("�������");
		sArr.Add("�ּһ簢����");
		sArr.Add("�����簢����");
		sArr.Add("�����簢����");

		m_Grid.AddCell(iRow, iCol++, hggettext("Ȱ����(LL)"));
		m_Grid.AddCell(iRow, iCol++, hggettext("� ��Ʈ�� ���� �簢 ���"));
		m_Grid.AddCell(iRow, iCol++, &pBri->m_nLoadEquiStrip, DT_RIGHT, TRUE, pBri->m_nLoadEffeciveW==0 ? CELL_LINE_NO : CELL_READONLY, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(iRow, iCol+1, _T(""), DT_LEFT, CELL_READONLY);
		iRow++;
		iCol = 0;

		// Ȱ���� - ��ȿ�� ���� ������ ���ϰ��
		m_Grid.AddCell(iRow, iCol++, hggettext("Ȱ����(LL)"));
		m_Grid.AddCell(iRow, iCol++, hggettext("��ȿ�� ���� ������ ���ϰ��"));
		m_Grid.AddCell(iRow, iCol++, &pBri->m_bLoadMultiRoadSurConfficient, DT_CENTER|DT_VCENTER, CELL_LINE_NO, pBri->m_bLoadMultiRoadSurConfficient==TRUE ? "����" : "������", CELL_TYPE_CHECK);
		m_Grid.AddCell(iRow, iCol+1, _T(""), DT_LEFT, CELL_READONLY);
		iRow++;
		iCol = 0;

		sArr.RemoveAll();
		sArr.Add("������޺� ȿ�� x ��");
		sArr.Add("1���(KL-510)");

		m_Grid.AddCell(iRow, iCol++, hggettext("Ȱ����(LL)"));
		m_Grid.AddCell(iRow, iCol++, hggettext("ǥ����������(��)"));
		m_Grid.AddCell(iRow, iCol++, &pBri->m_nLoadStandardLane, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(iRow, iCol+1, _T(""), DT_LEFT, CELL_READONLY);
		iRow++;
		iCol = 0;

		m_Grid.AddCell(iRow, iCol++, hggettext("��������(ES)"));
		m_Grid.AddCell(iRow, iCol++, hggettext("��������(ES)"));
		m_Grid.AddCell(iRow, iCol++, &pBri->m_dLiveLoadAlways, DT_RIGHT, CELL_LINE_NO); m_Grid.SetCellDec(bLsd? CELL_DEC_6 : CELL_DEC_3); m_Grid.SetCellUnit(UNIT_CGS_TONF_M2, pStd->m_szTonf_M2);
		m_Grid.AddCell(iRow, iCol+1, pStd->m_szTonf_M2, DT_LEFT, CELL_READONLY);
		iRow++;
		iCol = 0;

		m_Grid.AddCell(iRow, iCol++, hggettext("����Ȱ���� ������ �������"));
		m_Grid.AddCell(iRow, iCol++, hggettext("����Ȱ���� ������ �������"));
		m_Grid.AddCell(iRow, iCol++, &pBri->m_bCorrectionFactor_LiveLoadAlways, DT_CENTER|DT_VCENTER, CELL_LINE_NO, pBri->m_bCorrectionFactor_LiveLoadAlways==TRUE ? "����" : "������", CELL_TYPE_CHECK);
		m_Grid.AddCell(iRow, iCol+1, _T(""), DT_LEFT, CELL_READONLY);
		iRow++;
		iCol = 0;

		sArr.RemoveAll();
		sArr.Add(hggettext("LS����"));
		sArr.Add(hggettext("ES����"));
		sArr.Add(hggettext("LS����, ES����"));
		// 	sArr.Add(hggettext("���� Center��"));
		m_Grid.AddCell(iRow, iCol++, hggettext("������ ����� ����Ȱ���� ����"));
		m_Grid.AddCell(iRow, iCol++, hggettext("������ ����� ����Ȱ���� ����"));
		m_Grid.AddCell(iRow, iCol++, &pBri->m_nSelectWingLoad_LS_ES, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(iRow, iCol+1, _T(""), DT_RIGHT, CELL_READONLY);
		iRow++;
		iCol = 0;

		sArr.RemoveAll();
		sArr.Add(hggettext("0 mm"));
		sArr.Add(hggettext("300 mm"));
		m_Grid.AddCell(iRow, iCol++, hggettext("������ ����Ȱ���� ������ ��鿡�� ������ �Ÿ�"));
		m_Grid.AddCell(iRow, iCol++, hggettext("������ ����Ȱ���� ������ ��鿡�� ������ �Ÿ�"));
		m_Grid.AddCell(iRow, iCol++, &pBri->m_nLiveLoadDistToBackWall, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(iRow, iCol+1, _T(""), DT_RIGHT, CELL_READONLY);
		iRow++;
		iCol = 0;

		sArr.RemoveAll();
		sArr.Add("�������");
		sArr.Add("RANKINE");
		sArr.Add("Coulomb");
		m_Grid.AddCell(iRow, iCol++, hggettext("LS ���߰��� ��а��"));
		m_Grid.AddCell(iRow, iCol++, hggettext("LS ���߰��� ��а��"));
		m_Grid.AddCell(iRow, iCol++, &pBri->m_nSelectKindCoefficientKo, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(iRow, iCol+1, _T(""), DT_LEFT, CELL_READONLY);
		iRow++;
		iCol = 0;

		sArr.RemoveAll();
		sArr.Add("EV");
		sArr.Add("BP");
		sArr.Add("WA");
		m_Grid.AddCell(iRow, iCol++, hggettext("���μ��� ��������"));
		m_Grid.AddCell(iRow, iCol++, hggettext("���μ��� ��������"));
		m_Grid.AddCell(iRow, iCol++, &pBri->m_nSelectInteriorWaterPress, DT_RIGHT, TRUE, bBoxType? CELL_LINE_NO : CELL_READONLY, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(iRow, iCol+1, _T(""), DT_LEFT, CELL_READONLY);
		iRow++;
		iCol = 0;

		m_Grid.AddCell(iRow, iCol++, hggettext("���μ��� �������"));
		m_Grid.AddCell(iRow, iCol++, hggettext("���μ��� �������"));
		m_Grid.AddCell(iRow, iCol++, &pBri->m_dHeightInteriorWaterPress, DT_RIGHT, bBoxType? CELL_LINE_NO : CELL_READONLY);
		m_Grid.AddCell(iRow, iCol+1, hggettext("mm"), DT_LEFT, CELL_READONLY);
		iRow++;
		iCol = 0;

		m_Grid.AddCell(iRow, iCol++, hggettext("��������(PL)"));
		m_Grid.AddCell(iRow, iCol++, hggettext("��������(PL)"));
		m_Grid.AddCell(iRow, iCol++, &pBri->m_dLiveLoadFootway, DT_RIGHT, CELL_LINE_NO); m_Grid.SetCellDec(bLsd? CELL_DEC_6 : CELL_DEC_3); m_Grid.SetCellUnit(UNIT_CGS_TONF_M2, pStd->m_szTonf_M2);
		m_Grid.AddCell(iRow, iCol+1, pStd->m_szTonf_M2, DT_LEFT, CELL_READONLY);
		iRow++;
		iCol = 0;

		m_Grid.AddCell(iRow, iCol++, hggettext("�µ���ȭ��(��)"));
		m_Grid.AddCell(iRow, iCol++, hggettext("�µ���ȭ��(��)"));
		m_Grid.AddCell(iRow, iCol++, &pBri->m_dTemperatureChangeLoadGap, DT_RIGHT, CELL_LINE_NO);
		m_Grid.AddCell(iRow, iCol+1, hggettext("��"), DT_LEFT, CELL_READONLY);
		iRow++;
		iCol = 0;

		for (HGINT32 ix=0; ix<3; ++ix)
		{
			iRowTemperatureSlopeT = ix==0 ? iRow : iRowTemperatureSlopeT;
			CString str;
			m_Grid.AddCell(iRow, iCol++, hggettext("�µ����(TG)"));
			str.Format(_T("T%d"), ix+1); m_Grid.AddCell(iRow, iCol++, str);
			m_Grid.AddCell(iRow, iCol++, &pBri->m_dTemperatureSlopeT[ix], DT_RIGHT, CELL_LINE_NO);
			m_Grid.AddCell(iRow, iCol+1, hggettext("��"), DT_LEFT, CELL_READONLY);
			iRow++;
			iCol = 0;
		}

//		CString strArr[2] = { hggettext(""), hggettext("��ũ��Ʈ ���� ����") };
		sArr.RemoveAll();
		sArr.Add("�ƽ���Ʈ ���� ����");
		sArr.Add("��ũ��Ʈ ���� ����");
		m_Grid.AddCell(iRow, iCol++, hggettext("�µ����(TG)"));
		m_Grid.AddCell(iRow, iCol++, hggettext("�Ϻ��� �µ��� ������"));
		m_Grid.AddCell(iRow, iCol++, &pBri->m_nApplyPavement, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(iRow, iCol+1, hggettext(""), DT_LEFT, CELL_READONLY);
		iRow++;
		iCol = 0;
	}
	
	iRowUnderGroundRahmen = iRow;
	sArr.RemoveAll();
	sArr.Add("�̵�����");
	sArr.Add("��������");
	m_Grid.AddCell(iRow, iCol++, hggettext("���߶�౳"));
	m_Grid.AddCell(iRow, iCol++, hggettext("Ȱ���� ����"));
	m_Grid.AddCell(iRow, iCol++, &pBri->m_bCalcOpen, DT_RIGHT, pBri->m_bUnderGroundRahmen? CELL_LINE_NO : CELL_READONLY, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(iRow, iCol+1, _T(""), DT_LEFT, CELL_READONLY);
	iRow++;
	iCol = 0;

	const HGBOOL bInput = pBri->m_bUnderGroundRahmen && pBri->m_bCalcOpen == FALSE;
	sArr.RemoveAll();
	sArr.Add("���λ�������");
	sArr.Add("ö����������");
	sArr.Add("������Է�");
	m_Grid.AddCell(iRow, iCol++, hggettext("���߶�౳"));
	m_Grid.AddCell(iRow, iCol++, hggettext("���Ȱ���� ����"));
	m_Grid.AddCell(iRow, iCol++, &pBri->m_nUnderGroundDesignType, DT_RIGHT, TRUE, bInput ? CELL_LINE_NO : CELL_READONLY, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(iRow, iCol+1, _T(""), DT_LEFT, CELL_READONLY);
	iRow++;
	iCol = 0;

	m_Grid.AddCell(iRow, iCol++, hggettext("���߶�౳"));
	m_Grid.AddCell(iRow, iCol++, hggettext("���Ȱ���� ����"));
	m_Grid.AddCell(iRow, iCol++, &pBri->m_dUnderGroundDesignLiveAlways, DT_RIGHT, bInput && pBri->m_nUnderGroundDesignType == 2 ? CELL_LINE_NO : CELL_READONLY); m_Grid.SetCellDec(pStd->GetDecimal(UNIT_CGS_TONF_M2)); m_Grid.SetCellUnit(UNIT_CGS_TONF_M2, pStd->m_szTonf_M2);
	m_Grid.AddCell(iRow, iCol+1, pStd->m_szTonf_M2, DT_LEFT, CELL_READONLY);
	iRow++;
	iCol = 0;

	sArr.RemoveAll();
	sArr.Add("��������ö����");
	sArr.Add("ö�����������(�����)");
	sArr.Add("���μ�����(��)");
	sArr.Add("���μ������(��)");
	if (bLsd == FALSE)
		sArr.Add("�����������(��)");
	
	m_Grid.AddCell(iRow, iCol++, hggettext("���߶�౳"));
	m_Grid.AddCell(iRow, iCol++, hggettext("���Ȱ���� �������"));
	m_Grid.AddCell(iRow, iCol++, &pBri->m_nUnderGroundDesignSelect, DT_RIGHT, TRUE, bInput && pBri->m_nUnderGroundDesignType == 0 ? CELL_LINE_NO : CELL_READONLY, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(iRow, iCol+1, _T(""), DT_LEFT, CELL_READONLY);
	iRow++;
	iCol = 0;

	m_Grid.AddCell(iRow, iCol++, hggettext("���߶�౳"));
	m_Grid.AddCell(iRow, iCol++, hggettext("ö�� ��������"));
	m_xyButtonRailLoad.x = iCol;
	m_xyButtonRailLoad.y = iRow;
	m_Grid.AddCell(iRow, iCol++, hggettext("��������"), DT_CENTER | DT_VCENTER, CELL_READONLY);
	m_Grid.AddCell(iRow, iCol+1, _T(""), DT_LEFT, CELL_READONLY);
	iRow++;
	iCol = 0;

	if(pStd->IsRailDesign2017())
	{
		BOOL bActive = (pBri->m_bCalcOpen == 0 && pBri->m_nUnderGroundDesignType == 1);
		sArr.RemoveAll();
		if(pStd->IsRailDesignKDS2018())
		{
			sArr.Add("KDS 24 12 20 ���� �������� 4.1.3(3) ");
			sArr.Add("KDS 47 10 30 ���� �� ����ü� 4.1 ");
		}
		else
		{
			sArr.Add("���ϱ�����(7.3.2) ");
			sArr.Add("���� �� ����ü�(5.1.3) ");
		}
		m_Grid.AddCell(iRow, iCol++, hggettext("���߶�౳"));
		m_Grid.AddCell(iRow, iCol++, hggettext("ö�� �������� ������ ����"));
		m_Grid.AddCell(iRow, iCol++, &pBri->m_nUnderGroundDistribute, DT_RIGHT, TRUE, bActive || !pStd->IsRailDesignKDS2018() ? CELL_LINE_NO : CELL_READONLY, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(iRow, iCol+1, _T(""), DT_LEFT, CELL_READONLY);
		iRow++;
		iCol = 0;
	}

	HGINT32 iRowUnderGroundDesignHeight = iRow;
	m_Grid.AddCell(iRow, iCol++, hggettext("���߶�౳"));
	m_Grid.AddCell(iRow, iCol++, hggettext("��������"));
	m_Grid.AddCell(iRow, iCol++, &pBri->m_bUnderGroundDesignHeight, DT_CENTER|DT_VCENTER, bInput ? CELL_LINE_NO : CELL_READONLY, pBri->m_bUnderGroundDesignHeight==TRUE ? "�ڵ�����" : "������Է�", CELL_TYPE_CHECK);
	m_Grid.AddCell(iRow, iCol++, &pBri->m_dUnderGroundDesignHeight, DT_RIGHT, bInput && pBri->m_bUnderGroundDesignHeight == FALSE ? CELL_LINE_NO : CELL_READONLY); m_Grid.SetCellDec(pStd->GetDecimal(UNIT_CGS_M)); m_Grid.SetCellUnit("mm", pStd->m_szM);
	m_Grid.AddCell(iRow, iCol, pStd->m_szM, DT_LEFT, CELL_READONLY);
	iRow++;
	iCol = 0;

	if(bLsd == FALSE && bBoxType)
	{
		m_Grid.AddCell(iRow, iCol++, hggettext("���μ��� �������"));
		m_Grid.AddCell(iRow, iCol++, hggettext("���μ��� �������"));
		m_Grid.AddCell(iRow, iCol++, &pBri->m_dHeightInteriorWaterPress, DT_RIGHT, CELL_LINE_NO); m_Grid.SetCellDec(pStd->GetDecimal(UNIT_CGS_M)); m_Grid.SetCellUnit("mm", pStd->m_szM);
		m_Grid.AddCell(iRow, iCol+1, pStd->m_szM, DT_LEFT, CELL_READONLY);
		iRow++;
		iCol = 0;
	}

	HGINT32 iRowWaterLevel = iRow;
	m_Grid.AddCell(iRow, iCol++, hggettext("���ϼ���(EL.)"));
	m_Grid.AddCell(iRow, iCol++, hggettext("���ϼ���(EL.)"));
	m_Grid.AddCell(iRow, iCol++, &pBri->m_bApplyGroundWaterLevel, DT_CENTER|DT_VCENTER, bBoxType ? CELL_LINE_NO : CELL_READONLY, pBri->m_bApplyGroundWaterLevel==TRUE ? "����" : "������", CELL_TYPE_CHECK);
	m_Grid.AddCell(iRow, iCol++, &pBri->m_dGroundWaterLevel, DT_RIGHT, bBoxType && pBri->m_bApplyGroundWaterLevel ? CELL_LINE_NO : CELL_READONLY); m_Grid.SetCellDec(pStd->GetDecimal(UNIT_CGS_M)); m_Grid.SetCellUnit("mm", pStd->m_szM);
	m_Grid.AddCell(iRow, iCol, pStd->m_szM, DT_LEFT, CELL_READONLY);
	iRow++;
	iCol = 0;

	m_Grid.GetCellCount(iRow, iCol);
	m_Grid.SetRows(iRow);
	m_Grid.SetCols(iCol);

	m_Grid.SetColumnWidth(0, 100);
	m_Grid.SetColumnWidth(1, 200);
	m_Grid.SetColumnWidth(2, 150);
	m_Grid.SetColumnWidth(4, 60);
	m_Grid.SetRowHeightAll(20);

	// Merge
	if (iRowLiveLoad >= 0)
	{
		m_Grid.SetMergeRow(iRowLiveLoad, iRowLiveLoad + 3, 0);
	}

	if( iRowTemperatureSlopeT >= 0 )
	{
		m_Grid.SetMergeRow(iRowTemperatureSlopeT, iRowTemperatureSlopeT + 3, 0);
	}

	if (iRowUnderGroundRahmen >= 0)
	{
		m_Grid.SetMergeRow(iRowUnderGroundRahmen, iRowUnderGroundRahmen + 5, 0);
		m_Grid.SetMergeRow(iRowUnderGroundRahmen + 1, iRowUnderGroundRahmen + 2, 1);
	}

	for (HGINT32 ix = 0; ix < iRow; ++ix)
	{
		if (ix != iRowWaterLevel && ix != iRowUnderGroundDesignHeight)
		{
			m_Grid.SetMergeCol(ix, 2, 3);
		}

		if(ix >= iRowLiveLoad && iRowLiveLoad + 3 >= ix)
			continue;

		if(ix >= iRowTemperatureSlopeT && iRowTemperatureSlopeT + 3 >= ix)
			continue;

		if(ix >= iRowUnderGroundRahmen && iRowUnderGroundRahmen + 5 >= ix)
			continue;

		m_Grid.SetMergeCol(ix, 0, 1);
	}
}

void CInputGenLoad::SetGridData_LsdUserLoad()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	int nTab = m_Tab.GetCurSel();
	long nKind = nTab - m_nTabLsdUserLoadNormal;

	CCellID pCell = m_Grid.GetFocusCell();
	if(pCell.col > 0 && pCell.row > 0)
	{
		HGINT32 icUserLoad = 0;
		HGINT32 icLoad = pBri->GetCountUserLoadSetLsd(nKind);
		icUserLoad = icLoad < 1 ? 0 : pBri->m_vpUserLoadSet[nKind][m_nIdxUserSel]->m_pArrUserLoad.GetSize();

		long nRowMax = min(m_Grid.GetRowCount()-2, icUserLoad);
		long nIdx = pCell.row - 1;

		if(nIdx >= 0 && nIdx < nRowMax)
		{
			CBridgeUserLoad *pUserLoad = NULL;

			pUserLoad = pBri->m_vpUserLoadSet[nKind][m_nIdxUserSel]->m_pArrUserLoad.GetAt(nIdx);

			if(pUserLoad == NULL) return;

			SetGridData_User(pUserLoad->m_nType);
		}
		else SetGridData_User();
	}
	else
		SetGridData_User();
}

void CInputGenLoad::OnTabSelchangeLsdEta(NMHDR *pNMHDR, LRESULT *pResult)
{
	SetGridData_LsdModifiedFactor();
	SetDataInit();
}

void CInputGenLoad::OnButtonLsdComboReset()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_LIMIT);
	pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_EXTREME);
	pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_SERVICE);
	pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_FATIGUE);

	SetGridData();
	SetDataInit();
}

void CInputGenLoad::OnButtonAddCombine()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	int nTab = m_Tab.GetCurSel();
	long nPos = m_Grid.GetFocusCell().row;

	if(nTab == m_nTabLsdUserLoadNormal || nTab == m_nTabLsdUserLoadEQ || nTab == m_nTabLsdUserLoadEQ2 || nTab == m_nTabLsdUserLoadEQ3 || nTab == m_nTabLsdUserLoadEQ4)
	{
		long nKind = nTab - m_nTabLsdUserLoadNormal;

		HGINT32 nPos = pBri->GetCountUserLoadSetLsd(nKind);

		pBri->InsertUserLoadSetLsd(nKind, nPos);
		MakeStringUserLoadLsd();

		CString str(_T(""));
		str.Format(hgwgettext("��������� %d"), nPos+1);
		pBri->m_vpUserLoadSet[nKind][nPos]->m_szTitle = str;
	}
	else
	{
		if(nPos<0)
		{
			AfxMessageBox("�߰��� ��ġ�� �����Ͻʽÿ�.", MB_ICONINFORMATION);
			return;
		}

		if(m_bDesignLsd)
		{
			ELSDLoadCaseDir Dir = eLSD_LOADCASE_DIR_BRI;

			long nData = m_Grid.GetItemData(nPos,0);	
			ELSDCombKind nLType = static_cast<ELSDCombKind>(nData &0xFF);
			long nIdx = (nData >> 16);
			pBri->m_plsdManager->InsertLoadCase(nLType, Dir, nIdx);
		}
		else
		{
			if(nTab == m_nTabCombineUseLoad) pBri->m_MatComboUse.Add(TRUE,nPos);
			if(nTab == m_nTabCombineUltLoad) pBri->m_MatComboUlt.Add(TRUE,nPos);
			if(nTab == m_nTabCombineEccLoad) pBri->m_MatComboEcc.Add(TRUE,nPos);
			if(nTab == m_nTabCombineEQLoad)	 pBri->m_MatComboEQ.Add(TRUE,nPos);
		}
	}

	SetGridData();
	SetDataInit();
}

void CInputGenLoad::OnButtonRemoveCombine()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	int nTab = m_Tab.GetCurSel();
	long nPos = m_Grid.GetFocusCell().row;

	if(nTab == m_nTabLsdUserLoadNormal || nTab == m_nTabLsdUserLoadEQ)
	{
		long nKind = nTab - m_nTabLsdUserLoadNormal;

		pBri->EraseUserLoadSetLsd(nKind, m_nIdxUserSel);
		MakeStringUserLoadLsd();
		if(pBri->m_vpUserLoadSet[nKind].size() > 0 && pBri->m_vpUserLoadSet[nKind].size() <= m_nIdxUserSel)
			m_nIdxUserSel--;
	}
	else
	{
		if(nPos<0)
		{
			AfxMessageBox("������ ������ �����Ͻʽÿ�", MB_ICONINFORMATION);
			return;
		}

		if(m_bDesignLsd)
		{
			ELSDLoadCaseDir Dir = eLSD_LOADCASE_DIR_BRI;

			long nData = m_Grid.GetItemData(nPos,0);	
			ELSDCombKind nLType = static_cast<ELSDCombKind>(nData &0xFF);
			long nIdx = (nData >> 16);
			pBri->m_plsdManager->RemoveLoadCase(nLType,  Dir, nIdx);
		}
		else
		{
			if(nTab == m_nTabCombineUseLoad) pBri->m_MatComboUse.Remove(TRUE,nPos);
			if(nTab == m_nTabCombineUltLoad) pBri->m_MatComboUlt.Remove(TRUE,nPos);
			if(nTab == m_nTabCombineEccLoad) pBri->m_MatComboEcc.Remove(TRUE,nPos);
			if(nTab == m_nTabCombineEQLoad)	 pBri->m_MatComboEQ.Remove(TRUE,nPos);
		}
	}

	SetGridData();
	SetDataInit();
}

void CInputGenLoad::OnSelchangeComboBridgeNo()
{
	SetDataSave();
	m_pIn->m_nCurrentBridge = m_cbBridgeNo.GetCurSel();

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);

	if(pBri && m_bDesignLsd)
	{
		m_bAutoCombinInit = !(pBri->m_plsdManager->IsAllowUserInput());
		m_bEtaCombinEach = pBri->m_bEtaCombinEach;
	}

	SetGridData();
	SetDataInit();
	SetPaneTitle();
	m_cbBridgeNo.SetFocus();

	UpdateData(FALSE);
}

void CInputGenLoad::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nTab = m_Tab.GetCurSel();
	if(nTab == m_nTabLsdUserLoadNormal || nTab == m_nTabLsdUserLoadEQ || nTab == m_nTabLsdUserLoadEQ2 || nTab == m_nTabLsdUserLoadEQ3 || nTab == m_nTabLsdUserLoadEQ4)
	{
		MakeStringUserLoadLsd();
		m_nIdxUserSel = 0;
	}

	SetGridData();
	SetDataInit();
	DrawInputDomyunView(TRUE);
}

void CInputGenLoad::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult)
{
	SetDataSave();
}

void CInputGenLoad::DrawInputDomyunView(BOOL bZoomAll/*=FALSE*/)
{
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);

	int nTab = m_Tab.GetCurSel();
	if (nTab == m_nTabLsdLoad)
	{
		if (pBri->m_bUnderGroundRahmen && pBri->m_bCalcOpen == FALSE)
		{
			if (pBri->m_nUnderGroundDesignType == 0)
			{
				CGridDomyunEx Dom1(pDom);
				m_pIn->m_pARoadOptionStd->SetEnvType(&Dom1,HCAD_TBL1);

				CString str;
				if (pBri->m_nUnderGroundDesignSelect== 0 || pBri->m_nUnderGroundDesignSelect==1)// ��������ö����,ö�����������(�����)
				{
					Dom1.SetColumnCount(13);
					Dom1.SetRowCount(2);
					Dom1.SetRowHeightAll(Dom1.Always(6));

					for(HGINT32 ix=0; ix<13; ++ix)
					{
						if(ix==0) Dom1.SetColumnWidth(ix, Dom1.Always(40));
						else Dom1.SetColumnWidth(ix, Dom1.Always(20));
					}

					str.Format("������(%s)", pStd->m_szTonf);
					Dom1.SetTextMatrix(0, 0, "����(m)");
					Dom1.SetTextMatrix(1, 0, str);

					char  *TH[12] = {"1.0","1.5","2.0","2.5","3.0","4.0","5.0","6.0","7.0","8.0","9.0","10.0"};
					double LV[12] = {5.1,3.9,2.1,1.7,1.5,1.5,1.5,1.5,1.5,1.2,1.1,0.9};
					if(pBri->m_nUnderGroundDesignSelect==1)
						LV[11] = 1.0;
					double dDivide = 1.0;
					if(pBri->m_pARcBridgeDataStd->m_nDegreeBridge==1) dDivide = 18.0/24.0;
					if(pBri->m_pARcBridgeDataStd->m_nDegreeBridge==2) dDivide = 13.5/24.0;

					for(HGINT32 ix=0; ix<12; ++ix)
					{
						str.Format("%.1f", pStd->GetValueUnitType(LV[ix]*dDivide, UNIT_CGS_TONF,pBri->m_pARcBridgeDataStd->m_szTonf));

						Dom1.SetTextMatrix(0, ix+1, TH[ix]);
						Dom1.SetTextMatrix(1, ix+1, str);
					}
				}
				else if (pBri->m_nUnderGroundDesignSelect == 2)// ���μ�����(��)
				{
					Dom1.SetColumnCount(3);
					Dom1.SetRowCount(2);
					Dom1.SetRowHeightAll(Dom1.Always(6));
					Dom1.SetColumnWidthAll(Dom1.Always(60));

					Dom1.SetTextMatrix(0, 0, "���ǰ� 4m �̸��� ���");
					Dom1.SetTextMatrix(0, 1, "���ǰ� 4m �̸��� ��� (B>W1)");
					Dom1.SetTextMatrix(0, 2, "���ǰ� 4m �̻��� ���");

					Dom1.SetTextMatrix(1, 0, "Pvl = P(1+i) / (0.2+2D)");
					Dom1.SetTextMatrix(1, 1, "Pvl = 2P(1+i)(Bo-D-0.1) / Bo��");

					str.Format("Pvl = %.3f %s", pStd->GetValueUnitType(1.0, UNIT_CGS_TONF,pBri->m_pARcBridgeDataStd->m_szTonf),pBri->m_pARcBridgeDataStd->m_szTonf);
					Dom1.SetTextMatrix(1, 2, str);
				}
				else if (pBri->m_nUnderGroundDesignSelect == 3)// ���μ������(��)
				{
					Dom1.SetColumnCount(3);
					Dom1.SetRowCount(2);
					Dom1.SetRowHeightAll(Dom1.Always(6));
					Dom1.SetColumnWidthAll(Dom1.Always(60));

					Dom1.SetTextMatrix(0, 0, "���ǰ� 1.0 �� H �� 4.0m�� ���");
					Dom1.SetTextMatrix(0, 1, "���ǰ� 1.0 �� H �� 4.0m�� ��� (B>W1)");
					Dom1.SetTextMatrix(0, 2, "���ǰ� 4m �̻��� ���");

					Dom1.SetTextMatrix(1, 0, "Pvl = Pr(1+i) / 1.5(0.2+2H)");
					Dom1.SetTextMatrix(1, 1, "Pvl = 2P(1+i)(Bo-H-0.1) / Bo��");

					str.Format("Pvl = %.3f %s", pStd->GetValueUnitType(1.0, UNIT_CGS_TONF,pBri->m_pARcBridgeDataStd->m_szTonf),pBri->m_pARcBridgeDataStd->m_szTonf);
					Dom1.SetTextMatrix(1, 2, str);
				}
				else if (pBri->m_nUnderGroundDesignSelect == 4)// �����������(��)
				{
					Dom1.SetColumnCount(2);
					Dom1.SetRowCount(5);
					Dom1.SetRowHeightAll(Dom1.Always(10));
					Dom1.SetColumnWidth(0, Dom1.Always(60));
					Dom1.SetColumnWidth(1, Dom1.Always(150));

					Dom1.SetTextMatrix(0, 0, "���ǰ� 0.6 �� H �� 2.0m (Bo>W)");
					Dom1.SetTextMatrix(1, 0, "���ǰ� 0.6 �� H �� 2.0m (Bo<W)");
					Dom1.SetTextMatrix(2, 0, "���ǰ� 2.0 �� H �� 4.0m (Bo>W)");
					Dom1.SetTextMatrix(3, 0, "���ǰ� 2.0 �� H �� 4.0m (Bo<W)");
					Dom1.SetTextMatrix(4, 0, "���ǰ� 4m �̻��� ���");

					Dom1.SetTextMatrix(0, 1, "Pvl = 8P1(1+i)(0.75Bo-0.25D-4.225)/Bo��");
					Dom1.SetTextMatrix(1, 1, "Pvl = 8Pvl1{0.125B��-2Bo+8.4+(Bo-4.2)D}/Bo��");
					Dom1.SetTextMatrix(2, 1, "Pvl = 8Pvl1{4.15Bo-17.645-(0.5Bo+0.1)D-0.5D��}/Bo��+ 8Pvl2{8.4-2Bo+(Bo-4.2)D}/Bo��");
					Dom1.SetTextMatrix(3, 1, "Pvl = 8Pvl1{0.125Bo��-2Bo-8.4-(Bo-4.2)D}/Bo��+ 8Pvl2{8.4-2Bo+(Bo-4.2)D}/Bo��");

					str.Format("Pvl = %.3f %s", pStd->GetValueUnitType(1.0, UNIT_CGS_TONF,pBri->m_pARcBridgeDataStd->m_szTonf),pBri->m_pARcBridgeDataStd->m_szTonf);
					Dom1.SetTextMatrix(4, 1, str);
				}

				Dom1.Draw(FALSE);
				m_pIn->m_pARoadOptionStd->SetEnvType(&Dom1,HCAD_TBL2);
				Dom1.DrawOutLine(); 
				Dom1.DrawOutLineShade();

				*pDom << Dom1;
			}
		}
	}
	else if(nTab == m_nTabLsdUserLoadNormal || nTab == m_nTabLsdUserLoadEQ || nTab == m_nTabLsdUserLoadEQ2 || nTab == m_nTabLsdUserLoadEQ3 || nTab == m_nTabLsdUserLoadEQ4)
	{
		CDomyun Dom(pDom);
		Dom.SetScaleDim(100 * 1000);
		DrawModel(&Dom, (nTab == m_nTabLsdUserLoadEQ || nTab == m_nTabLsdUserLoadEQ2 || nTab == m_nTabLsdUserLoadEQ3 || nTab == m_nTabLsdUserLoadEQ4));
		*pDom << Dom;
	}

	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

void CInputGenLoad::OnButtonApplySameAll()
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

			if(m_bDesignLsd)
			{
				if(pBri->m_plsdManager && pBri2->m_plsdManager)
					pBri2->m_plsdManager->CopyTotalLsdLoadSelect(pBri->m_plsdManager);
			}
			else
			{
				MakeSameSelectLoad(pBri2,pBri);
				MakeSameCombineUseLoad(pBri2,pBri);
				MakeSameCombineUltLoad(pBri2,pBri);
				MakeSameCombineEccLoad(pBri2,pBri);
				MakeSameCombineEQLoad(pBri2,pBri);
			}
			MakeSameLsdLoad(pBri2,pBri);
		}
	}
}

void CInputGenLoad::OnButtonApplyMirror()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(pBri==NULL || pBri->m_nSeparBri==0) return;

	CRcBridgeRebar* pBriMatch = pStd->GetBridgeMatchRebar(pBri);
	if(pBriMatch==NULL || pBriMatch->m_nSeparBri==0) return;

	CString strMsg = _T("");
	strMsg.Format("��� ���� �Է»����� %s��(��) �����ϰ� �����˴ϴ�.\n����Ͻðڽ��ϱ� ?",pBriMatch->m_strBridgeName);
	if(AfxMessageBox(strMsg,MB_YESNO)==IDYES)
	{
		if(m_bDesignLsd)
		{
			if(pBri->m_plsdManager && pBriMatch->m_plsdManager)
				pBriMatch->m_plsdManager->CopyTotalLsdLoadSelect(pBri->m_plsdManager);
		}
		else
		{
			MakeSameSelectLoad(pBri,pBriMatch);
			MakeSameCombineUseLoad(pBri,pBriMatch);
			MakeSameCombineUltLoad(pBri,pBriMatch);
			MakeSameCombineEccLoad(pBri,pBriMatch);
			MakeSameCombineEQLoad(pBri,pBriMatch);
		}

		MakeSameLsdLoad(pBri,pBriMatch);

		//////////////////////////////////////////////////////////////////////////
		SetDataInit();
	}
}

void CInputGenLoad::MakeSameCombineUltLoad(CRcBridgeApp* pBri1, CRcBridgeApp* pBri2)
{
	if(pBri1==NULL || pBri2==NULL) return;
	pBri1->m_MatComboUlt = pBri2->m_MatComboUlt;
	long i=0; for(i=0; i<MAX_RCBRIDGE_COMBO; i++)
	{
		pBri1->m_bApplyCombo3D[COMBO_ULTIMATE][i] = pBri2->m_bApplyCombo3D[COMBO_ULTIMATE][i];
	}
}

void CInputGenLoad::MakeSameCombineUseLoad(CRcBridgeApp* pBri1, CRcBridgeApp* pBri2)
{
	if(pBri1==NULL || pBri2==NULL) return;
	pBri1->m_MatComboUse = pBri2->m_MatComboUse;
	long i=0; for(i=0; i<MAX_RCBRIDGE_COMBO; i++)
	{
		pBri1->m_bApplyCombo3D[COMBO_SERVICE][i] = pBri2->m_bApplyCombo3D[COMBO_SERVICE][i];
	}
}

void CInputGenLoad::MakeSameCombineEccLoad(CRcBridgeApp* pBri1, CRcBridgeApp* pBri2)
{
	if(pBri1==NULL || pBri2==NULL) return;
	pBri1->m_MatComboEcc = pBri2->m_MatComboEcc;
	long i=0; for(i=0; i<MAX_RCBRIDGE_COMBO; i++)
	{
		pBri1->m_bApplyCombo3D[COMBO_ECCENTRIC][i] = pBri2->m_bApplyCombo3D[COMBO_ECCENTRIC][i];
	}
}

void CInputGenLoad::MakeSameCombineEQLoad(CRcBridgeApp* pBri1, CRcBridgeApp* pBri2)
{
	if(pBri1==NULL || pBri2==NULL) return;
	pBri1->m_MatComboEQ  = pBri2->m_MatComboEQ;
	long i=0; for(i=0; i<MAX_RCBRIDGE_COMBO; i++)
	{
		pBri1->m_bApplyCombo3D[COMBO_EARTHQUAKE][i]  = pBri2->m_bApplyCombo3D[COMBO_EARTHQUAKE][i];
		pBri1->m_bApplyCombo3D[COMBO_EARTHQUAKE2][i] = pBri2->m_bApplyCombo3D[COMBO_EARTHQUAKE2][i];
	}
}

void CInputGenLoad::MakeSameSelectLoad(CRcBridgeApp* pBri1, CRcBridgeApp* pBri2)
{
	if(pBri1==NULL || pBri2==NULL) return;
	// ���߼��� -------------------------------------------------------
	for(long ix = 0; ix < BRIDGE_LOAD_BASIC; ix++)
	{
		pBri1->m_bApplyLoad[ix] = pBri2->m_bApplyLoad[ix];
	}
	pBri1->m_bApplyLiveLoadEQ = pBri2->m_bApplyLiveLoadEQ;
	pBri1->m_dWLoadNormal = pBri2->m_dWLoadNormal;
	pBri1->m_dWLoadEQ = pBri2->m_dWLoadEQ;
}

void CInputGenLoad::MakeSameLsdLoad( CRcBridgeApp* pBri1, CRcBridgeApp* pBri2 )
{
	if(pBri1==NULL || pBri2==NULL) return;
	if(m_bDesignLsd)
	{
		pBri1->m_dDepthApplySubSid = pBri2->m_dDepthApplySubSid;
		pBri1->m_nLoadEffeciveW = pBri2->m_nLoadEffeciveW;
		pBri1->m_nLoadEquiStrip = pBri2->m_nLoadEquiStrip;
		pBri1->m_bLoadMultiRoadSurConfficient = pBri2->m_bLoadMultiRoadSurConfficient;
		pBri1->m_nLoadStandardLane = pBri2->m_nLoadStandardLane;
		pBri1->m_dLiveLoadAlways = pBri2->m_dLiveLoadAlways;
		pBri1->m_bCorrectionFactor_LiveLoadAlways = pBri2->m_bCorrectionFactor_LiveLoadAlways;
		pBri1->m_nSelectWingLoad_LS_ES = pBri2->m_nSelectWingLoad_LS_ES;
		pBri1->m_nLiveLoadDistToBackWall = pBri2->m_nLiveLoadDistToBackWall;
		pBri1->m_nSelectKindCoefficientKo = pBri2->m_nSelectKindCoefficientKo;
		pBri1->m_nSelectInteriorWaterPress = pBri2->m_nSelectInteriorWaterPress;
		pBri1->m_dLiveLoadFootway = pBri2->m_dLiveLoadFootway;
		pBri1->m_dTemperatureChangeLoadGap = pBri2->m_dTemperatureChangeLoadGap;
		for(long ix = 0; ix < 3; ++ix)
		{
			pBri1->m_dTemperatureSlopeT[ix] = pBri2->m_dTemperatureSlopeT[ix];
		}
		pBri1->m_nApplyPavement = pBri2->m_nApplyPavement;
	}

	pBri1->m_dHeightInteriorWaterPress = pBri2->m_dHeightInteriorWaterPress;

	pBri1->m_bCalcOpen = pBri2->m_bCalcOpen;
	pBri1->m_nApplyPavement = pBri2->m_nApplyPavement;
	pBri1->m_nUnderGroundDesignType = pBri2->m_nUnderGroundDesignType;
	pBri1->m_dUnderGroundDesignLiveAlways = pBri2->m_dUnderGroundDesignLiveAlways;
	pBri1->m_nUnderGroundDistribute = pBri2->m_nUnderGroundDistribute;
	pBri1->m_nUnderGroundDesignSelect = pBri2->m_nUnderGroundDesignSelect;
	pBri1->m_bUnderGroundDesignHeight = pBri2->m_bUnderGroundDesignHeight;
	pBri1->m_dUnderGroundDesignHeight = pBri2->m_dUnderGroundDesignHeight;
}

void CInputGenLoad::OnCellChangedData( NMHDR* pNotifyStruct, LRESULT* result )
{
	int nTab = m_Tab.GetCurSel();

	CCellID cell = m_Grid.GetFocusCell();
	CString str  = m_Grid.GetTextMatrix(cell.row, 0);
	CString strSub = m_Grid.GetTextMatrix(cell.row, 1);

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);

	if(nTab == m_nTabLsdLoad)
	{
		if( str == _T("�µ����(TG)") )
		{
			if( strSub == _T("T1") || strSub == _T("T2") || strSub == _T("T3") )
			{
				CString strText = m_Grid.GetTextMatrix(cell.row, cell.col);
				HGDOUBLE dVal = atof(strText);

				if( dVal < 0 )
				{
					SetGridData();
					SetDataInit();
					return;
				}
			}
		}
		else if( str == _T("����ħ�� ���밪") || str == _T("�µ���ȭ��(��)"))
		{
			CString strText = m_Grid.GetTextMatrix(cell.row, cell.col);
			HGDOUBLE dVal = atof(strText);

			if( dVal< 0 )
				m_Grid.SetTextMatrix(cell.row, cell.col, _T("0"), DT_RIGHT|DT_VCENTER, FALSE);
			else if( 100 < dVal )
				m_Grid.SetTextMatrix(cell.row, cell.col, _T("100"), DT_RIGHT|DT_VCENTER, FALSE);
		}
		else if (str == _T("���ϼ���(EL.)"))
		{
			if(AfxMessageBox(_T("�������� �� ���������� �ʱ�ȭ �˴ϴ�.\n��� �Ͻðڽ��ϱ� ? "), MB_YESNO) == IDNO)
			{
				SetDataInit();
				
				return;
			}
			SetDataSave();
			SetGridData();
			SetDataInit();
		}
		else if ( str == "Ȱ����(LL)")
		{
			if(strSub == _T("��ȿ��(E) �����"))
			{
				SetDataSave();
				SetGridData();
				SetDataInit();
			}

		}
		else if(strSub == _T("Ȱ���� ����") || strSub == _T("���Ȱ���� ����"))
		{
			SetDataSave();
			SetGridData();
			SetDataInit();
		}

		if (pStd->IsLsdDesign() == FALSE)
		{
			CString str1  = m_Grid.GetTextMatrix(cell.row, 1);
			if (str1 == _T("��������"))
			{
				HGDOUBLE dNew = pBri->m_dUnderGroundDesignHeight;
				if (cell.col == 2)
				{
					dNew = pBri->CalcUnderGroundDesignHeight();
				}

				HGDOUBLE dOld = m_Grid.GetTextMatrixDouble(cell.row, 3);
				if(pStd->m_nTypeKciCode == 1 && pBri->m_bUnderGroundRahmen && GetValueUnitChange(dNew, "mm", pStd->m_szM) != dOld)
				{
					// kci2007 ����ǰ� ���߶���϶� ���ǰ� �ٲ�� ���������� ���� ���� �Ǵ��Ѵ�.
					if(AfxMessageBox("���������� �������߿� ���� ��������� �����մϴ�.", MB_YESNO) == IDYES)
					{
						pBri->SetDataDefaultCombineUltLoad();
						pBri->SetDataDefaultCombineUseLoad();
						pBri->SetDataDefaultCombineEccLoad();
						pBri->SetDataDefaultCombineEQLoad();
					}
					else
					{
						SetDataInit();
						return;
					}
				}
			}
		}
	}

	if(nTab == m_nTabSelectLoad)
	{
		if(str.Find(_T("EH")) != -1)
		{
			CRcBridgeApp *pBri = m_pIn->GetRcBridge();
			if(pBri)
			{
				SetDataSave();
				pBri->m_plsdManager->SetLoadDirOption();
				m_Grid.UpdateData(FALSE);
			}
		}
	}

	SetDataSave();
	
	if(pStd->IsLsdDesign())
	{
		if(nTab == m_nTabLsdLoad)
		{
			// ����ħ�� ���밪, ��ȿ�� ���� ������ ���ϰ��
			if((cell.row==2 || cell.row==4 || cell.row==7) && cell.col==2)		
			{
				SetGridData();
				m_Grid.UpdateData(FALSE);
			}

			if (str == _T("���߶�౳") || str == _T("���ϼ���(EL.)"))
			{
				if (str == _T("���ϼ���(EL.)"))
				{
					if(pBri->m_bApplyGroundWaterLevel == TRUE && cell.col == 2)
					{
						double dStaStt = pBri->GetStationBridgeStt(FALSE);
						double dStaEnd = dStaStt + pBri->GetLengthBridge(FALSE);
						double dElStt = pBri->GetLineBase()->GetElevationPlanEl(dStaStt);
						double dElEnd = pBri->GetLineBase()->GetElevationPlanEl(dStaEnd);
						pBri->m_dGroundWaterLevel = min(dElStt, dElEnd) - 1000;
					}

					pBri->SetDataDefaultSelectLoad();
					pBri->SetDataDefaultCombineUseLoad();
					pBri->SetDataDefaultCombineUltLoad();
					pBri->SetDataDefaultCombineEccLoad();
					pBri->SetDataDefaultCombineEQLoad();
				}

				SetGridData();
				SetDataInit();
			}

			DrawInputDomyunView(TRUE);
		}

		if(nTab == m_nTabLsdUserLoadNormal || nTab == m_nTabLsdUserLoadEQ || nTab == m_nTabLsdUserLoadEQ2 || nTab == m_nTabLsdUserLoadEQ3 || nTab == m_nTabLsdUserLoadEQ4)
		{
			MakeStringUserLoadLsd();
			SetGridData();
			SetDataInit();
		}

		if (nTab == m_nTabLsdModifiedFactor)
		{
			SetGridData();
			SetDataInit();
		}

		if((nTab == m_nTabSelectLoad || nTab == m_nTabLsdLoadFactor) && m_bAutoCombinInit)
		{
			pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_LIMIT);
			pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_EXTREME);
			pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_SERVICE);
			pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_FATIGUE);

			SetGridData();
			SetDataInit();
		}
	}
}

void CInputGenLoad::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	CInputDialogBase::OnCellChanged(pNotifyStruct, result);

	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	if(pStd->IsLsdDesign() == FALSE) return;

	long nRow = m_Grid.GetFocusCell().row;
	long nCol = m_Grid.GetFocusCell().col;

	int nTab = m_Tab.GetCurSel();
	if(nTab == m_nTabLsdLoadFactor)	
	{
		if(nCol == 5 && (nRow == 5 || nRow == 8))
		{
			m_pIn->SetComment(_T("[��.��(2015) 3.13.2.1] Ȱ���� ���Ͻô� ǳ���� ���ݸ� ������ �� �ִ�. �� ������ ���� WS�� factor�� �����Ͽ� ����Ͻñ� �ٶ��ϴ�."),  TRUE);
		}
		else
		{
			m_pIn->SetComment(_T("�������տ� ���ؼ� ������ ������ �����Ͻñ� �ٶ��ϴ�."),  TRUE);
		}
	}
}

void CInputGenLoad::OnCellButtonClick(NMHDR* pNotifyStruct, LRESULT* result)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(pBri==NULL)
		return;

	NM_GRIDVIEW *pGridView = (NM_GRIDVIEW *)pNotifyStruct;	
	int nRow = pGridView->iRow;
	int nCol = pGridView->iColumn;

	int nTab = m_Tab.GetCurSel();
	if (nTab == m_nTabLsdLoad)
	{
		if (pBri->m_bUnderGroundRahmen && pBri->m_bCalcOpen == FALSE && pBri->m_nUnderGroundDesignType == 1)
		{
			if (m_xyButtonRailLoad.x == nCol && m_xyButtonRailLoad.y == nRow)
			{
				CDialogRailLoadAlways dlg(pBri);
				dlg.DoModal();
			}
		}
	}
}

void CInputGenLoad::OnCheckShowUseLoad()
{
	UpdateData();

	SetGridData();
	SetDataInit();
}

void CInputGenLoad::SetGridData_User( long nLoadType/*=0*/ )
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	int nTab = m_Tab.GetCurSel();
	long nKind = (nTab - m_nTabLsdUserLoadNormal);

	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(1,7,0,0);
	if(m_szArrUserLoad.GetSize() == 0)
	{
		m_pIn->GridMessage(&m_Grid, hggettext("���õ� ����� ������ �����ϴ�."));
		return;
	}
	else if(m_szArrUserLoad.GetSize() < m_nIdxUserSel) m_nIdxUserSel = 0;
	
	long nRow = 0;
	long nCol = 0;
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	CStringArray sArr; //Lsd ���߱���

	pBri->m_plsdManager->GetStringArrayLsdLoadCaseKindForUserLoad(sArr);

	long nSizeUserLoad = pBri->m_vpUserLoadSet[nKind].size();
	for(long nCase = 0; nCase < nSizeUserLoad; nCase++)
	{
		CString sLoadCase = pBri->m_vpUserLoadSet[nKind][nCase]->m_sTypeLoadCase; // ���߼����� �������� �� �����Ƿ� �ٽ� ����
		if(sLoadCase == _T(""))
			pBri->m_vpUserLoadSet[nKind][nCase]->m_nTypeLoadCase = 0;
		else
			pBri->m_vpUserLoadSet[nKind][nCase]->m_nTypeLoadCase = pBri->m_plsdManager->GetIndexLsdLoadCaseKindForUserLoad(sLoadCase);
	}

	m_Grid.AddCell(nRow, nCol++, &m_nIdxUserSel, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, m_szArrUserLoad);
	m_Grid.AddCell(nRow, nCol++, hggettext("��������"), nFormat, CELL_READONLY);
	SetGridTitle_UserLoad(nLoadType);
	nRow++;

	CString szNo = m_szArrUserLoad.GetAt(m_nIdxUserSel).Right(2);

	long nSize = pBri->m_vpUserLoadSet[nKind][m_nIdxUserSel]->m_pArrUserLoad.GetSize();
	for(long ix=0; ix<nSize; ++ix)
	{
		nCol = 0;
		CBridgeUserLoad *pUserLoad = pBri->m_vpUserLoadSet[nKind][m_nIdxUserSel]->m_pArrUserLoad.GetAt(ix);;

		if(pUserLoad == NULL) continue;

		szNo.Format(_T("%d"), ix+1);
		m_Grid.AddCell(nRow, nCol++, szNo, nFormat, CELL_READONLY);
		m_Grid.AddCell(nRow, nCol++, &pUserLoad->m_nType, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, m_szArrTypeUserLoad);
		m_Grid.AddCell(nRow, nCol++, &pUserLoad->m_nIdxNo, nFormat);
		if(pUserLoad->m_nType == USER_LOAD_JOINT_V)
		{
			m_Grid.AddCell(nRow, nCol++, &pUserLoad->m_Ux, nFormat); m_Grid.SetCellDec(CELL_DEC_3);
			m_Grid.SetCellUnit(UNIT_CGS_TONF, pStd->m_szTonf);
			m_Grid.AddCell(nRow, nCol++, &pUserLoad->m_Uy, nFormat); m_Grid.SetCellDec(CELL_DEC_3);
			m_Grid.SetCellUnit(UNIT_CGS_TONF, pStd->m_szTonf);
			m_Grid.AddCell(nRow, nCol++, &pUserLoad->m_Uz, nFormat); m_Grid.SetCellDec(CELL_DEC_3);
			m_Grid.SetCellUnit(UNIT_CGS_TONF, pStd->m_szTonf);
			m_Grid.AddCell(nRow, nCol++, _T(""), nFormat, CELL_READONLY);
		}
		else if(pUserLoad->m_nType == USER_LOAD_JOINT_M)
		{
			m_Grid.AddCell(nRow, nCol++, &pUserLoad->m_Mx, nFormat); m_Grid.SetCellDec(CELL_DEC_3);
			m_Grid.SetCellUnit(UNIT_CGS_TONFM, pStd->m_szTonfM);
			m_Grid.AddCell(nRow, nCol++, &pUserLoad->m_My, nFormat); m_Grid.SetCellDec(CELL_DEC_3);
			m_Grid.SetCellUnit(UNIT_CGS_TONFM, pStd->m_szTonfM);
			m_Grid.AddCell(nRow, nCol++, &pUserLoad->m_Mz, nFormat); m_Grid.SetCellDec(CELL_DEC_3);
			m_Grid.SetCellUnit(UNIT_CGS_TONFM, pStd->m_szTonfM);
			m_Grid.AddCell(nRow, nCol++, _T(""), nFormat, CELL_READONLY);
		}
		else if(pUserLoad->m_nType == USER_LOAD_ELEMENT_V)
		{
			m_Grid.AddCell(nRow, nCol++, &pUserLoad->m_dDist[0], nFormat); m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.SetCellUnit(UNIT_CGS_M, pStd->m_szM);
			m_Grid.AddCell(nRow, nCol++, &pUserLoad->m_Ux, nFormat); m_Grid.SetCellDec(CELL_DEC_3);
			m_Grid.SetCellUnit(UNIT_CGS_TONF, pStd->m_szTonf);
			m_Grid.AddCell(nRow, nCol++, &pUserLoad->m_Uy, nFormat); m_Grid.SetCellDec(CELL_DEC_3);
			m_Grid.SetCellUnit(UNIT_CGS_TONF, pStd->m_szTonf);
			m_Grid.AddCell(nRow, nCol++, &pUserLoad->m_Uz, nFormat); m_Grid.SetCellDec(CELL_DEC_3);
			m_Grid.SetCellUnit(UNIT_CGS_TONF, pStd->m_szTonf);
		}
		else if(pUserLoad->m_nType == USER_LOAD_ELEMENT_WX  || pUserLoad->m_nType == USER_LOAD_ELEMENT_WY || pUserLoad->m_nType == USER_LOAD_ELEMENT_WZ)
		{
			m_Grid.AddCell(nRow, nCol++, &pUserLoad->m_dDist[0], nFormat); m_Grid.SetCellDec(CELL_DEC_2);
			m_Grid.AddCell(nRow, nCol++, &pUserLoad->m_dDist[1], nFormat); m_Grid.SetCellDec(CELL_DEC_2);
			m_Grid.AddCell(nRow, nCol++, &pUserLoad->m_dLoadW2[0], nFormat); m_Grid.SetCellDec(CELL_DEC_6);
			m_Grid.SetCellUnit(UNIT_CGS_TONF_M, pStd->m_szTonf_M);
			m_Grid.AddCell(nRow, nCol++, &pUserLoad->m_dLoadW2[1], nFormat); m_Grid.SetCellDec(CELL_DEC_6);
			m_Grid.SetCellUnit(UNIT_CGS_TONF_M, pStd->m_szTonf_M);
		}
		nRow++;
	}
	nCol = 0;

	m_Grid.AddCell(nRow, nCol++, hggettext("���� ��Ī"), nFormat, CELL_READONLY);
	m_Grid.AddCell(nRow, nCol++, &pBri->m_vpUserLoadSet[nKind][m_nIdxUserSel]->m_szTitle, nFormat, nLine);
	m_Grid.AddCell(nRow, nCol++, pBri->m_vpUserLoadSet[nKind][m_nIdxUserSel]->m_szTitle, nFormat, nLine);

	m_Grid.AddCell(nRow, nCol++, hggettext("���߱���"), nFormat, CELL_READONLY);
	m_Grid.AddCell(nRow, nCol++, hggettext("���߱���"), nFormat, CELL_READONLY);
	m_Grid.AddCell(nRow, nCol++, hggettext("���߱���"), nFormat, CELL_READONLY);
	m_Grid.AddCell(nRow, nCol++, &pBri->m_vpUserLoadSet[nKind][m_nIdxUserSel]->m_nTypeLoadCase, DT_LEFT | DT_VCENTER, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
	
	m_Grid.GetCellCount(nRow, nCol);
	m_Grid.SetRows(nRow);
	m_Grid.SetCols(nCol);
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(80);
	m_Grid.SetColumnWidth(0,120);
	m_Grid.SetColumnWidth(1,120);
}

void CInputGenLoad::OnButtonRemoveUser()
{
	// TODO: Add your control notification handler code here
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	int nTab = m_Tab.GetCurSel();
	long nKind = nTab - m_nTabLsdUserLoadNormal;
	
	long nSize = pBri->m_vpUserLoadSet[nKind][m_nIdxUserSel]->m_pArrUserLoad.GetSize();
	if(nSize > 0)
	{
		pBri->m_vpUserLoadSet[nKind][m_nIdxUserSel]->m_pArrUserLoad.RemoveAt(nSize-1);
		
		SetGridData();
		SetDataInit();
	}
}

void CInputGenLoad::OnButtonAddUser()
{
	// TODO: Add your control notification handler code here
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	int nTab = m_Tab.GetCurSel();
	long nKind = nTab - m_nTabLsdUserLoadNormal;

	CBridgeUserLoad *pUserLoad = new CBridgeUserLoad;
	pUserLoad->m_nIdxNo = 0;

	if(pBri->GetCountUserLoadSetLsd(nKind) < 1)
	{
		AfxMessageBox(hggettext("���õ� ������ �����ϴ�."));
		return;
	}
	pBri->m_vpUserLoadSet[nKind][m_nIdxUserSel]->m_pArrUserLoad.Add(pUserLoad);

	SetGridData();
	SetDataInit();
}

void CInputGenLoad::MakeStringUserLoadLsd()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	int nTab = m_Tab.GetCurSel();
	long nKind = nTab - m_nTabLsdUserLoadNormal;

	m_szArrUserLoad.RemoveAll();

	CString str;
	HGINT32 icLoad = pBri->GetCountUserLoadSetLsd(nKind);
	for(HGINT32 ix = 0; ix < icLoad; ++ix)
	{
		if(pBri->m_vpUserLoadSet[nKind][ix]->m_szTitle == _T(""))
			str.Format(hggettext("��������� %d"), ix+1);
		else
			str = pBri->m_vpUserLoadSet[nKind][ix]->m_szTitle;
		m_szArrUserLoad.Add(str);
	}
}

void CInputGenLoad::DrawModel( CDomyun* pDomP, BOOL bEarthQuake )
{
	CDomyun Dom(pDomP);

	m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar *pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	CFEMManage *pFem = bEarthQuake? &pBri->m_FemEQ[2-1] : &pBri->m_FemNormal;
	if(pFem->IsOutputLoaded()==FALSE)
	{
		CARcBridgeCalcStd CalcStd(pBri);
		CalcStd.m_pFrame->MakeSapData(bEarthQuake? 2 : 0,TRUE);
	}

	CDPoint xy1(0,0),xy2(0,0),xyText(0,0),vY(0,1);
	double dOffsetText = Dom.GetTextHeight()*2;
	long nSizeEle = pFem->GetElementSize();
	long i=0; for(i=0; i<nSizeEle; i++)
	{
		CElement *pEle  = pFem->GetElement((unsigned short)i);
		CJoint   *pJnt1 = pFem->GetJoint(pEle->m_nJStt);
		CJoint   *pJnt2 = pFem->GetJoint(pEle->m_nJEnd);

		xy1 = frM(CDPoint(pJnt1->m_vPoint.x,pJnt1->m_vPoint.z));
		xy2 = frM(CDPoint(pJnt2->m_vPoint.x,pJnt2->m_vPoint.z));

		Dom.LineTo(xy1,xy2);
		xyText = (xy1+xy2)/2 - dOffsetText*(xy1-xy2).Unit().Rotate90();
		Dom.TextCircle(xyText.x,xyText.y,COMMA(pEle->m_nIndex+1),3);
	}

	long nSizeJnt = pFem->GetJointSize();
	for(i=0; i<nSizeJnt; i++)
	{
		CJoint  *pJnt = pFem->GetJoint((unsigned short)i);

		xy1 = frM(CDPoint(pJnt->m_vPoint.x,pJnt->m_vPoint.z));

		if(pJnt->IsFix())
			xyText = xy1 - dOffsetText*vY;
		else
			xyText = xy1 + dOffsetText/3*vY;
		Dom.TextOut(xyText,COMMA(pJnt->m_nIdx+1));
	}

	*pDomP << Dom;
}

void CInputGenLoad::OnCheckAutoCombinInit()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar *pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	UpdateData(TRUE);

	pBri->m_plsdManager->SetAllowUserInput(!m_bAutoCombinInit);
	SetDataInit();
}

void CInputGenLoad::OnCheckEtaCombinEach()
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar *pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	UpdateData(TRUE);

	pBri->m_bEtaCombinEach = m_bEtaCombinEach;
	SetDataInit();
}

void CInputGenLoad::OnCellChangedRow( NMHDR* pNotifyStruct, LRESULT* result )
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar *pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;

	int nTab = m_Tab.GetCurSel();
	if(nTab == m_nTabLsdUserLoadNormal || nTab == m_nTabLsdUserLoadEQ || nTab == m_nTabLsdUserLoadEQ2 || nTab == m_nTabLsdUserLoadEQ3 || nTab == m_nTabLsdUserLoadEQ4)
	{
		// ���õǴ� ���߿����� Ÿ��Ʋ�� �ٽ� ���ش�.
		long nKind = nTab - m_nTabLsdUserLoadNormal;

		HGINT32 icUserLoad = pBri->m_vpUserLoadSet[nKind][m_nIdxUserSel]->m_pArrUserLoad.GetSize();
		HGINT32 ixSel = pGridView->iRow - 1;
		if(ixSel > -1 && ixSel < icUserLoad)
		{
			CBridgeUserLoad *pUserLoad = pBri->m_vpUserLoadSet[nKind][m_nIdxUserSel]->m_pArrUserLoad.GetAt(ixSel);
			if(pUserLoad)
			{
				SetGridTitle_UserLoad(pUserLoad->m_nType);
			}
		}
	}
}

void CInputGenLoad::SetGridTitle_UserLoad( long nLoadType/*=0*/ )
{
	UINT nFormat = DT_CENTER|DT_VCENTER;
	HGINT32 nRow = 0;
	HGINT32 nCol = 2;
	if(nLoadType==USER_LOAD_JOINT_V || nLoadType==USER_LOAD_JOINT_M)
	{
		m_Grid.AddCell(nRow, nCol++, hggettext("���� NO."), nFormat, CELL_READONLY);
		m_Grid.AddCell(nRow, nCol++, _T("X"), nFormat, CELL_READONLY);
		m_Grid.AddCell(nRow, nCol++, _T("Y"), nFormat, CELL_READONLY);
		m_Grid.AddCell(nRow, nCol++, _T("Z"), nFormat, CELL_READONLY);
		m_Grid.AddCell(nRow, nCol++, _T("-"), nFormat, CELL_READONLY);
	}
	else if(nLoadType==USER_LOAD_ELEMENT_V)
	{
		m_Grid.AddCell(nRow, nCol++, hggettext("���� NO."), nFormat, CELL_READONLY);
		m_Grid.AddCell(nRow, nCol++, _T("d1"), nFormat, CELL_READONLY);
		m_Grid.AddCell(nRow, nCol++, _T("Vx"), nFormat, CELL_READONLY);
		m_Grid.AddCell(nRow, nCol++, _T("Vy"), nFormat, CELL_READONLY);
		m_Grid.AddCell(nRow, nCol++, _T("Vz"), nFormat, CELL_READONLY);
	}
	else if(nLoadType==USER_LOAD_ELEMENT_WX || nLoadType==USER_LOAD_ELEMENT_WY || nLoadType==USER_LOAD_ELEMENT_WZ)
	{
		m_Grid.AddCell(nRow, nCol++, hggettext("���� NO."), nFormat, CELL_READONLY);
		m_Grid.AddCell(nRow, nCol++, _T("d1"), nFormat, CELL_READONLY); 
		m_Grid.AddCell(nRow, nCol++, _T("d2"), nFormat, CELL_READONLY); 
		m_Grid.AddCell(nRow, nCol++, _T("w1"), nFormat, CELL_READONLY); 
		m_Grid.AddCell(nRow, nCol++, _T("w2"), nFormat, CELL_READONLY); 
	}
	m_Grid.UpdateData(FALSE);
}
