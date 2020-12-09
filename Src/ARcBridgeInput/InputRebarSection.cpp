// InputRebarSection.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "../ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "RebarSupportDlg.h"
#include "RebarShearDlg.h"
#include "CheckCantileverDlg.h"
#include "InputRebarSection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputRebarSection dialog


CInputRebarSection::CInputRebarSection(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputRebarSection::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputRebarSection)
	m_nTabUpperSlab = -1;
	m_nTabArchConnectStirrup = -1;
	m_nTabArchRib = -1;
	for (HGINT32 ix = 0; ix < MAX_JIJUM_COUNT_RC; ++ix)
	{
		m_nTabFooting[ix] = -1;
		m_nTabInWall[ix] = -1;
	}
	m_nTabArchJijumSupport = -1;
	m_nTabOutWallEnd = -1;
	m_nTabBracket = -1;
	m_nTabRebarShear = -1;
	m_nTabLowerSlab = -1;
	m_nTabRebarSupport = -1;
	m_nTabOutWallStt = -1;

	m_dAccu = 0.0;
	m_sPart = _T("");
	//}}AFX_DATA_INIT
	m_nTab				= 0;
	m_nTabRebar			= 0;
	m_bSamePlaceInWall	= FALSE;
	m_bChangeCTC		= FALSE;
	m_bSyncBridge		= FALSE;	// SyncBridge�� �ѹ��� �ϸ� �ȴ�.(�� �ű涧 ���� �� �ʿ� ����)

	m_pDlgCheck = new CInputCheckGrid;
}

CInputRebarSection::~CInputRebarSection()
{
	delete m_pDlgCheck;
}

void CInputRebarSection::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputRebarSection)
	DDX_Control(pDX, IDC_COMBO_CTC_PART, m_comboCTCPart);
	DDX_Control(pDX, IDC_BUTTON_SAMESTTEND2, m_btnSameSttEnd);
	DDX_Control(pDX, IDC_BUTTON_SAMEINWALL2, m_btnSameInWall);
	DDX_Control(pDX, IDC_CHECK_BMD1, m_checkBMD);
	DDX_Control(pDX, IDC_CHECK_SFD, m_checkSFD);
	DDX_Control(pDX, IDC_CHECK_SAMEOFFSET_TANDC, m_checkSameOffset);
	DDX_Control(pDX, IDC_COMBO_UPPER, m_comboUpper);
	DDX_Control(pDX, IDC_COMBO_LOWER, m_comboLower);
	DDX_Control(pDX, IDC_COMBO_CTC, m_comboCTC);
	DDX_Control(pDX, IDC_BUTTON_SUPPORT, m_btnRebarSupport);
	DDX_Control(pDX, IDC_BUTTON_SHEAR, m_btnRebarShear);
	DDX_Control(pDX, IDC_BUTTON_CANTILEVER, m_btnCheckCantil);
	DDX_Control(pDX, IDC_BUTTON_EXAMINE, m_btnExamine);
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_comboBridgeNo);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	DDX_Control(pDX, IDC_TAB_REBAR, m_TabRebar);
	DDX_Text(pDX, IDC_EDIT_ACCU, m_dAccu);
	DDX_Text(pDX, IDC_STATIC_PART, m_sPart);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputRebarSection, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputRebarSection)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_BN_CLICKED(IDC_BUTTON_SUPPORT, OnButtonRebarSupport)
	ON_BN_CLICKED(IDC_BUTTON_SHEAR, OnButtonRebarShear)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnSelchangingTab)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_CBN_SELCHANGE(IDC_COMBO_UPPER, OnSelchangeComboUpper)
	ON_CBN_SELCHANGE(IDC_COMBO_LOWER, OnSelchangeComboLower)
	ON_BN_CLICKED(IDC_CHECK_SAMEOFFSET_TANDC, OnCheckSameoffsetTandc)
	ON_CBN_SELCHANGE(IDC_COMBO_CTC, OnSelchangeComboCtc)
	ON_BN_CLICKED(IDC_CHECK_SFD, OnCheckSfd)
	ON_BN_CLICKED(IDC_CHECK_BMD1, OnCheckBmd1)
	ON_BN_CLICKED(IDC_BUTTON_EXAMINE, OnButtonExamine)
	ON_BN_CLICKED(IDC_BUTTON_SAMEINWALL2, OnButtonSameinwall2)
	ON_BN_CLICKED(IDC_BUTTON_SAMESTTEND2, OnButtonSamesttend2)
	ON_EN_KILLFOCUS(IDC_EDIT_ACCU, OnKillfocusEditAccu)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_REBAR, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_REBAR, OnTabSelchanging)
	ON_CBN_SELCHANGE(IDC_COMBO_CTC_PART, OnSelchangeComboCtcPart)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW,  IDC_GRID, OnCellChangedRow)

	ON_NOTIFY(GVN_CELL_INSERTED_COL, IDC_GRID, OnCellInsertedCol)
	ON_NOTIFY(GVN_CELL_DELETED_COL, IDC_GRID, OnCellDeletedCol)
	ON_BN_CLICKED(IDC_BUTTON_CANTILEVER, OnBnClickedButtonCantilever)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputRebarSection message handlers
void CInputRebarSection::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	
	return;
}

void CInputRebarSection::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	SetFocus();
	m_nTabRebar = m_TabRebar.GetCurSel();
	SetGridData();
	SetDataInit();
	SetControl();
	
	DrawInputDomyunView(FALSE);
	SetCTCPart();
	return;
}

void CInputRebarSection::SetGridData()
{
	BOOL bSupportRebar = (m_nTabRebar == m_nTabRebarSupport) ? TRUE : FALSE;
// 	if(bSupportRebar)
// 		m_nRowFocusItemTitle	= 1;
// 	else
		m_nRowFocusItemTitle	= 1;

	if(m_nTab == m_nTabUpperSlab)
	{
		SetGridData_UpperSlab(bSupportRebar);
	}
	else if(m_nTab == m_nTabLowerSlab)
	{
		SetGridData_LowerSlab(bSupportRebar);
	}
	else if(m_nTab == m_nTabOutWallStt)
	{
		SetGridData_OutWall(TRUE, bSupportRebar);
	}
	else if(m_nTab == m_nTabOutWallEnd)
	{
		SetGridData_OutWall(FALSE, bSupportRebar);
	}
	else if(m_nTab == m_nTabBracket)
	{
		SetGridData_Bracket(bSupportRebar);
	}
	else if(m_nTab == m_nTabArchRib)
	{
		SetGridData_ArchRib(bSupportRebar);
	}
	else if(m_nTab == m_nTabArchJijumSupport)
	{
		SetGridData_ArchJijumSupport();
	}
	else if(m_nTab == m_nTabArchConnectStirrup)
	{
		SetGridData_ArchRibConnectSectionStirrup();
	}
	else 
	{
		for (long nInWall = 0; nInWall < MAX_JIJUM_COUNT_RC ; nInWall++)
		{
			if(m_nTab == m_nTabInWall[nInWall])
				SetGridData_InWall(bSupportRebar);
		}

		long footing = 0; for(footing = 0; footing < MAX_JIJUM_COUNT_RC; footing++)
		{
			if(m_nTab == m_nTabFooting[footing])
				SetGridData_Footing(footing, bSupportRebar);
		}
	}

	m_Grid.EnableInsertCol();
}

void CInputRebarSection::SetDataSave()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	m_Grid.UpdateData(TRUE);
	SyncRebar();

	// �����༱ �и� ����
	m_pIn->m_pARcBridgeDataStd->CopyToCoupleBridge_RebarStandardSection(m_pIn->m_nCurrentBridge);

	m_pIn->m_pDoc->SetModifiedFlag();

	if(m_pDlgCheck->GetSafeHwnd() && m_pDlgCheck->IsWindowVisible())
	{
		pBri->MakeRebarInfoCycleStd(TRUE);
		m_pDlgCheck->Check();
	}

	if(m_nTab == m_nTabUpperSlab || m_nTab == m_nTabLowerSlab)
	{
		// ������ ��ö�ٰ� ��±� ��ǥ �ʱ�ȭ
		pBri->m_bCaledRebarMain_Plane	= FALSE;
		pBri->m_bCaledRebarSupport_Plane	= FALSE;
	}
}

// m_bChangeCTC : CTC�� ����Ǹ� ��ü �ǰ���� �ؾ� �ȴ�.
void CInputRebarSection::SetDataDefault()
{
	for(long bri = 0; bri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(bri);
		if(!pBri) continue;

		BOOL bFirst   = IsFirst(bri);
		pBri->SetDefaultedState(GetIDD(), 0x00000001);
		if(!bFirst && !IsClickDefaultButton(bri)) continue;

		// ARCBRIDGE-3732
		if (pBri->m_nTypeBridge == BRIDGE_TYPE_PI || pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB)
		{
			BOOL bVerDir = pBri->IsVertDir();
			pBri->SyncBridge(0, !bVerDir);
		}
		else
			pBri->SyncBridge();

		long footing	= GetIdxCurFooting();
		long nType		= GetTypeDefault();
			
		// ó�� �ǰ���ϰ�� ��ü �ǰ��
		if(bFirst)
		{
			pBri->SetDataDefaultRebarSupport_VertSection_Std(-1);
			pBri->SetDataDefaultRebarShear_VertSection_Std(-1);
			m_bChangeCTC	= FALSE;

			if(pBri->m_pARcBridgeDataStd->IsLsdDesign())
			{
				// ĵ�ŷ��� ���� �������� �ʱ�ȭ
				pBri->SetDataDefaultSelectLoadLsd(TRUE);
				pBri->SetDataDefaultLoadFactor(TRUE);
				pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_LIMIT, TRUE);
				pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_EXTREME, TRUE);
				pBri->SetDataDefaultComboLSD(eLSD_COMBINATION_SERVICE, TRUE);
			}
		}
		else
		{
			if(m_bChangeCTC) 
			{
				// ���ö�� ���
				pBri->SetDataDefaultRebarSupport_VertSection_Std(-1);

				// ����ö�� ���
				pBri->SetDataDefaultRebarShear_VertSection_Std(-1);

				m_bChangeCTC	= FALSE;
			}
			else
			{
				if(m_nTabRebar == m_nTabRebarSupport)
					pBri->SetDataDefaultRebarSupport_VertSection_Std(nType, footing);
				else if(m_nTabRebar == m_nTabRebarShear)
					pBri->SetDataDefaultRebarShear_VertSection_Std(nType, footing);
			}

			// �����༱ �и� ����
			m_pIn->m_pARcBridgeDataStd->CopyToCoupleBridge_RebarStandardSection(m_pIn->m_nCurrentBridge);
		}
		if(m_pDlgCheck->GetSafeHwnd() && m_pDlgCheck->IsWindowVisible())
		{
			pBri->MakeRebarInfoCycleStd(TRUE);
			m_pDlgCheck->Check();
		}

		if(m_nTab == m_nTabUpperSlab || m_nTab == m_nTabLowerSlab)
		{
			// ������ ��ö�ٰ� ��±� ��ǥ �ʱ�ȭ
			pBri->m_bCaledRebarMain_Plane	= FALSE;
			pBri->m_bCaledRebarSupport_Plane	= FALSE;
		}
	}

	SyncRebar();
	SetGridData();
	SetDataInit();
}

void CInputRebarSection::InitTabCtrl()
{
	m_TabRebar.RemoveAllTabs();
	m_TabRebar.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_TabRebar.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	// ����Ŭ ��
	CStringArray sArrTabCycleName;
	long nTab		= 0;
	m_nTabRebarSupport	= -1;
	m_nTabRebarShear	= -1;
	
	sArrTabCycleName.Add("���ö�� �Է�"); m_nTabRebarSupport = nTab++;
	sArrTabCycleName.Add("����ö�� �Է�"); m_nTabRebarShear = nTab++;

	long i = 0; for(i = 0; i < sArrTabCycleName.GetSize(); i++)
	m_TabRebar.InsertItem(i, sArrTabCycleName[i]);

	if(m_TabRebar.GetItemCount() <= m_nTabRebar) m_nTabRebar = 0;
	m_TabRebar.SetCurSel(m_nTabRebar);


	
	nTab				= 0;
	m_nTabUpperSlab		= -1;
	m_nTabLowerSlab		= -1;
	m_nTabOutWallStt	= -1;
	m_nTabOutWallEnd	= -1;
	for(i = 0; i < MAX_JIJUM_COUNT_RC; i++)
	{
		m_nTabInWall[i]	= -1;
		m_nTabFooting[i]	= -1;
	}
	m_nTabBracket		= -1;
	m_nTabArchRib			= -1;
	m_nTabArchJijumSupport	= -1;
	m_nTabArchConnectStirrup	= -1;

	m_sArrTabName.RemoveAll();
	
	m_sArrTabName.Add("��ν�����"); m_nTabUpperSlab = nTab++;
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		m_sArrTabName.Add("�Ϻν�����"); m_nTabLowerSlab = nTab++;
	}
	if(pBri->IsOutWall(TRUE))
	{
		m_sArrTabName.Add("��������ü"); m_nTabOutWallStt = nTab++;
	}
	if(pBri->IsOutWall(FALSE))
	{
		m_sArrTabName.Add("��������ü"); m_nTabOutWallEnd = nTab++;
	}
	//31534	
	if(pBri->GetCountInWall() > 0 && pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB)
	{
		long nCountInWallTmp = pBri->GetCountInWall();

		CString str = _T("");

		if(!(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH && pBri->GetJijumConnectRibAndSlab() == 1 && pBri->GetCountInWall() == 1))
		{
			for (long ix=0; ix<nCountInWallTmp ; ix++)
			{
				str.Format("������ü%d", ix+1);
				m_sArrTabName.Add(str); m_nTabInWall[ix] = nTab++;
			}
		}
	}

	
	long nCountInWall	= pBri->GetCountInWall();
	long nJ		= 0;
	CString str			= _T("");
	if(pBri->IsOutWall(TRUE) && pBri->IsBoxType() == FALSE)
	{
		m_sArrTabName.Add("����������"); m_nTabFooting[nJ++] = nTab++;
	}
	else 
		m_nTabFooting[nJ++]	= -1;

	if(pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB && pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
	{
		for(i = 0; i < nCountInWall; i++)
		{
			CWallApp *pWall	= pBri->GetInWall(i);
			if(!pWall->m_footing.m_bIs) 
			{
				m_nTabFooting[nJ++]	= -1;
				continue;
			}
			str.Format("�߰���������%d", i+1);
			m_sArrTabName.Add(str); m_nTabFooting[nJ++] = nTab++;
		}
	}
	else
	{
		for(i = 0; i < nCountInWall; i++)
		{
			m_nTabFooting[nJ++]	= -1;
		}
	}

	if(pBri->IsOutWall(FALSE) && pBri->IsBoxType() == FALSE)
	{
		m_sArrTabName.Add("����������"); m_nTabFooting[nJ++] = nTab++;
	}
	else
		m_nTabFooting[nJ++]	= -1;
	
	if(pBri->IsOutWall(TRUE) || pBri->IsOutWall(FALSE))
	{
		m_sArrTabName.Add("�����"); m_nTabBracket	= nTab++;
	}

	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		m_sArrTabName.Add("��ġ����"); m_nTabArchRib = nTab++;
		m_sArrTabName.Add("��������� ����"); m_nTabArchJijumSupport	= nTab++;
		m_sArrTabName.Add("���պ� ���ͷ�"); m_nTabArchConnectStirrup	= nTab++;
	}

	
	m_Tab.DeleteAllItems();
	for(i = 0; i < m_sArrTabName.GetSize(); i++)
		m_Tab.InsertItem(i, m_sArrTabName[i]);

	if(m_Tab.GetItemCount() <= m_nTab) m_nTab = 0;
	m_Tab.SetCurSel(m_nTab);
}

void CInputRebarSection::OnPreInitDialog()
{
	// �����ϸ鼭 pick box�� ũ�⸦ ������
	if(m_pView) 
	{
		CTwinVector tv;
		tv.SetPickBoxSize(m_pView->GetSelectionRectSize()/m_pView->GetRatioWindow());
	}
	
	InitTabCtrl();
	InitComboCtrl();

	SetResize(IDC_BUTTON_EXAMINE,		SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
	SetResize(IDC_TAB,					SZ_BOTTOM_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_TAB_REBAR,			SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID,					SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_SUPPORT,		SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_SHEAR,			SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_CANTILEVER,			SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_VIEW,			SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_BMD1,			SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_SFD,			SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_ACCU,			SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_MM,			SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_ACCU,			SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_SAMEINWALL2,		SZ_BOTTOM_RIGHT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_SAMESTTEND2,		SZ_BOTTOM_RIGHT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_PART,				SZ_BOTTOM_RIGHT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_COMBO_CTC_PART,			SZ_BOTTOM_RIGHT,		SZ_BOTTOM_RIGHT);

	SetHyperHelp(m_pIn->GetManualPath());

	m_Grid.InitGrid(m_pIn->m_pARcBridgeDataStd->m_Fy);
	SetGridData();
	SetCTCPart();
}

BOOL CInputRebarSection::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		UpdateData();
		if(m_Grid.GetFocusCell().col == m_Grid.GetCols()-1)
			pMsg->wParam = VK_RIGHT;
		else
			pMsg->wParam = VK_TAB;
	}
		
	structGridCell *cell = m_Grid.GetCurrentCell();
	if(cell)
	{
		
	}

	return CInputDialogBase::PreTranslateMessage(pMsg);
}

void CInputRebarSection::InitComboCtrl()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	CString str = _T("");
	// ����
	long nSize = pStd->GetBridgeSize();
	long i = 0; for(i = 0; i < nSize; i++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(i);
		str = pStd->GetStringBridgeOrder(i) + _T(" : ") + pBri->m_strBridgeName
			+ _T(" [") + pBri->GetStringBridgeType() + _T("]");
		m_comboBridgeNo.AddString(str);
	}
	m_comboBridgeNo.SetCurSel(m_pIn->m_nCurrentBridge);

	// CTC
	for(long ctc = 0; ctc < MAX_REBAR_CTC; ctc++)
	{
		CHgBaseConcStd baseConcStd;
		str.Format("CTC %.0f", baseConcStd.GetValueRebarCTC(ctc));
		m_comboCTC.AddString(str);
	}
	InitComboCtrlPart();

	// ����
	for(long block = 0; block < MAX_REBAR_BLOCK; block++)
	{
		str.Format("%d����", block+1);
		m_comboUpper.AddString(str);
		m_comboLower.AddString(str);
	}
	SetControl();
}

void CInputRebarSection::OnSelchangeComboBridgeNo() 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	m_pIn->m_nCurrentBridge = m_comboBridgeNo.GetCurSel();
	InitTabCtrl();
	SetControl();
	SetGridData();
	SetDataInit();
	SetPaneTitle();
	DrawInputDomyunView(TRUE);
	m_comboBridgeNo.SetFocus();
}

void CInputRebarSection::DrawInputDomyunView(BOOL bZoomAll)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(m_pIn->GetScaleInputDialog());


	CDomyun Dom(pDom);
	CARcBridgeDBStandard DBStd(m_pIn->GetRcBridgeRebar(), m_pIn->m_pARoadOptionStd, m_pIn->m_pARcBridgeDataStd);

	// ���� ������ �⺻������
	pBri->m_footingEnd.m_exFooting.m_Pile.InitScaleCutting();

	// ���ʿ� ������ ���� �׸��� ��ü�� ���� ������ ���ش�
	// ���ʸ� �׷��� �ɶ��� �ش� ���ʰ� �纸������ �Ǵ��ϰ�, �ٸ��� �׸����� �����갡 �纸������ �Ǵ��ؼ� �纸���̸� ���Ÿ��� �������ش�.
	long nIdxFooting	= GetIdxCurFooting();
	CFootingApp *pFooting	= NULL;
	if(nIdxFooting > -1)
		pFooting	= pBri->GetFooting(nIdxFooting);
	
	BOOL bDrawFooting	= TRUE;//pFooting ? TRUE :FALSE;
	BOOL bDrawBridge	= TRUE;//bDrawFooting;
	
	// (ARCBRIDGE-1659) ���� ��ö���� �������� ��ġ�� ���ָ� ���ʵ� ������������ �׷��ش�.
	// ��� ��ü���� ����ȭ�� ����.
	BOOL bVertDir	= (pFooting && pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH) ? pFooting->IsVertDir() : pBri->IsVertDir();

	if(1)//!m_bSyncBridge)
	{
		//pBri->SyncBridge(0, !bVertDir);
		pBri->GetTvVertSection_Std(0, !bVertDir, FALSE);
		pBri->SyncBridgeRebar(FALSE, bVertDir ? -1 : 1);
		m_bSyncBridge	= TRUE;
	}

	// ���ܸ� �׸�
	DBStd.m_pDrawVertSection->DrawVertSectionAll(&Dom, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE);

	// ���Ʈ�� / ���ܷµ�
	DBStd.m_pDrawVertSection->DrawDiagramBMD_SFD(&Dom, m_checkBMD.GetCheck(), m_checkSFD.GetCheck(), FALSE);

	// ��ö�� 1cycle �׸���
	DBStd.m_pDrawVertSection->DrawRebarMain_VertSectionAll(&Dom, 0, FALSE, FALSE, FALSE);

	// ���ö��/����ö�� �׸���
	long dan(0), upper(0), i(0);
	BOOL bDimUpperSlab	= (m_nTab == m_nTabUpperSlab);
	BOOL bDimOutWallStt	= (m_nTab == m_nTabOutWallStt);
	BOOL bDimOutWallEnd	= (m_nTab == m_nTabOutWallEnd);
	BOOL bDimBracketStt	= bDimOutWallStt;
	BOOL bDimBracketEnd	= bDimOutWallEnd;
	BOOL bDimFooting	= FALSE;
	BOOL bDimBracket	= m_nTab == m_nTabBracket;
	BOOL bDimRib		= m_nTab == m_nTabArchRib;
	BOOL bDimArchJijum	= m_nTab == m_nTabArchJijumSupport;
	BOOL bDimArchConnect	= m_nTab == m_nTabArchConnectStirrup;
	BOOL bDimRebar		= FALSE;
	BOOL bDimSupportRebar	= (m_nTabRebar == m_nTabRebarSupport);
	BOOL bDimShearRebar		= !bDimSupportRebar;
	BOOL bDimLowerSlab	= m_nTab == m_nTabLowerSlab;
	if(m_nTab == m_nTabArchJijumSupport)
	{
		bDrawBridge		= FALSE;
		bDrawFooting	= TRUE;
	}

	long nJ	= 0;
	if(pFooting)
	{
		long footing = 0; for(footing = 0; footing < MAX_JIJUM_COUNT_RC; footing++)
		{
			CFootingApp *pFootingTmp	= pBri->GetFootingByJijumNumber(footing);
			if(pFootingTmp == pFooting)
			{
				bDimFooting	= TRUE;
				nJ	= footing;
				break;
			}
		}
	}

	//31534
// 	BOOL bDimInWall = FALSE;
// 	if(pBri->GetCountInWall() > 0)
// 	{
// 		for (long ix = 0; ix< MAX_JIJUM_COUNT_RC; ix++)
// 		{
// 			if(m_nTab == m_nTabInWall[ix])
// 			{
//				bDimInWall		= (m_nTab == m_nTabInWall[ix]);
// 				break;
// 			}
// 		}
// 	}

	// ���� �Է��Ҷ��� ��ü / ��ν����� ��±�,����ö���� �׸��� �ʴ´�
	// �����米�� ���������� ���Ÿ���Ÿ� ����� ������, 
	// ����ϰ�� ���Ÿ���Ÿ� ����� ��Ʊ� ������.
	if(bDimFooting)
	{
		bDrawBridge	= FALSE;
	}

	if(bDrawBridge)
	{
		long stt(0);
		// ��ν�����
		for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			bDimRebar	= (dan==0) ? TRUE : FALSE;
			for(upper = 0; upper < 2; upper++)
			{
				DBStd.m_pDrawVertSection->DrawRebarSupport_VertSection_UpperSlab(&Dom, (upper==iUPPER), dan, bDimUpperSlab&&bDimRebar&&bDimSupportRebar, bDimUpperSlab&&bDimRebar&&bDimSupportRebar, 0, TRUE, CDRect(0, 0, 0, 0), bVertDir ? -1 : 1, dan==0);
				if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
					DBStd.m_pDrawVertSection->DrawRebarSupport_VertSection_ArchRib(&Dom, (upper==iUPPER), dan, bDimRib&&bDimSupportRebar, bDimRib&&bDimSupportRebar, 0, TRUE, CDRect(0, 0, 0, 0), bVertDir ? -1 : 1);
				if(pBri->IsBoxType())
					DBStd.m_pDrawVertSection->DrawRebarSupport_VertSection_LowerSlab(&Dom, (upper==iUPPER), dan, bDimLowerSlab&&bDimRebar&&bDimSupportRebar, bDimLowerSlab&&bDimRebar&&bDimSupportRebar, 0, TRUE, CDRect(0, 0, 0, 0), bVertDir ? -1 : 1, dan==0);
			}
		}
		DBStd.m_pDrawVertSection->DrawRebarShear_VertSection_UpperSlab(&Dom, bDimUpperSlab&&bDimShearRebar, bDimUpperSlab&&bDimShearRebar, TRUE, CDRect(0, 0, 0, 0), bVertDir?-1:1);
		if(pBri->IsBoxType())
		{
			DBStd.m_pDrawVertSection->DrawRebarShear_VertSection_LowerSlab(&Dom, bDimLowerSlab&&bDimShearRebar, bDimLowerSlab&&bDimShearRebar, TRUE, CDRect(0, 0, 0, 0), bVertDir?-1:1);
		}
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		{
			DBStd.m_pDrawVertSection->DrawRebarShear_VertSection_ArchRib(&Dom, bDimRib&&bDimShearRebar, bDimRib&&bDimShearRebar, TRUE, CDRect(0, 0, 0, 0), bVertDir ? -1 : 1);
			DBStd.m_pDrawVertSection->DrawRebarStirrup_VertSection_ConnectSection(&Dom, bDimArchConnect, bDimArchConnect, TRUE);
		}
		for(stt = 0; stt < 2; stt++)
		{
			DBStd.m_pDrawVertSection->DrawRebarShear_VertSection_UpperSlabDanbu(&Dom, 0, (stt==iSTT), FALSE, FALSE, FALSE);
			DBStd.m_pDrawVertSection->DrawRebarShear_VertSection_LowerSlabDanbu(&Dom, 0, (stt==iSTT), FALSE, FALSE, FALSE);
			DBStd.m_pDrawVertSection->DrawRebarSupport_VertSection_UpperSlabDanBu(&Dom, (stt==iSTT), FALSE, FALSE, FALSE);			
			if(pBri->IsBoxType())
				DBStd.m_pDrawVertSection->DrawRebarSupport_VertSection_LowerSlabDanBu(&Dom, (stt==iSTT), FALSE, FALSE, FALSE);
		}
		

		// ������ü
		for(stt = 0; stt < 2; stt++)
		{
			BOOL bDimOutWall	= (stt==iSTT) ? bDimOutWallStt : bDimOutWallEnd;
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				bDimRebar	= (dan==0) ? TRUE : FALSE;
				for(long inner = 0; inner < 2; inner++)
				{
					DBStd.m_pDrawVertSection->DrawRebarSupport_VertSection_OutWall(&Dom, (stt==iSTT), (inner==iINNER), dan, bDimOutWall&&bDimRebar&&bDimSupportRebar, bDimOutWall&&bDimRebar&&bDimSupportRebar, TRUE, CDRect(0, 0, 0, 0), bVertDir ? -1 : 1);
				}
			}
			DBStd.m_pDrawVertSection->DrawRebarSupport_VertSection_OutWallHunch(&Dom, stt==iSTT, bDimOutWall, TRUE, CDRect(0, 0, 0, 0), bVertDir ? -1 : 1);

			long nDimDan	= pBri->GetDanMainRebarOutWall(0, stt==iSTT, FALSE);
			DBStd.m_pDrawVertSection->DrawRebarShear_VertSection_OutWall(&Dom, (stt==iSTT), nDimDan, bDimOutWall&&bDimShearRebar, bDimOutWall&&bDimShearRebar, FALSE, TRUE, bVertDir ? -1 : 1);
		}

		// ����� & �λ��������
		for(stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			BOOL bDim	= bStt ? bDimBracketStt : bDimBracketEnd;
			DBStd.m_pDrawVertSection->DrawRebarSupport_VertSection_Bracket(&Dom, bStt, bDim&&bDimSupportRebar, bDim&&bDimSupportRebar, TRUE);
			DBStd.m_pDrawVertSection->DrawRebarShear_VertSection_Bracket(&Dom, bStt, bDimBracket&&bDimShearRebar, bDimBracket&&bDimShearRebar, TRUE);
			DBStd.m_pDrawVertSection->DrawBFootingRebar(&Dom, bStt, FALSE, FALSE, TRUE, TRUE, TRUE);
		}

		// ������ü
		//31543
		long nInWallDim = 0;		
		for (nInWallDim = 0; nInWallDim < MAX_JIJUM_COUNT_RC; nInWallDim++)
		{
			if(m_nTab == m_nTabInWall[nInWallDim])
				break;
		}
		BOOL bInWallDim = FALSE;

		long nCountWall = pBri->GetCountInWall();
		for(long wall = 0; wall < nCountWall; wall++)
		{
			CWallApp *pWall	= pBri->GetInWall(wall);			

			if(wall == nInWallDim)
				bInWallDim = TRUE;
			else
				bInWallDim = FALSE;

			if(pWall)
			{
				for(dan = 0; dan < MAX_REBAR_DAN; dan++)
				{
					bDimRebar = (dan==0) ? TRUE : FALSE;
					for(long left = 0; left < 2; left++)
					{
						DBStd.m_pDrawWall->DrawRebarSupport_Front(&Dom, pWall, (left==iLEFT), dan, bDimRebar&&bInWallDim&&bDimSupportRebar, bDimRebar&&bInWallDim&&bDimSupportRebar, TRUE, bVertDir);
					}
				}

				DBStd.m_pDrawWall->DrawRebarShear_Front(&Dom, pWall, 0, bInWallDim&&bDimShearRebar, bInWallDim&&bDimShearRebar, FALSE, TRUE, bVertDir);
			}
		}

		// �Ŵ� ö��
		long nJ = 0; for(nJ = 0; nJ < pBri->GetCountJijum(); nJ++)
		{
			DBStd.m_pDrawVertSection->DrawRebarAll_SlabGirder(&Dom, nJ, FALSE, FALSE, 0, FALSE, TRUE, TRUE, TRUE, TRUE, FALSE, -1, TRUE);
		}

		// ��ġ�� ���ö��
		long nCountHunchRebar	= pBri->m_pArrRCMainUpperSlabHunch[0].GetSize();
		for(i = 0; i < nCountHunchRebar; i++)
		{
			DBStd.m_pDrawVertSection->DrawRebarSupport_VertSection_UpperSlabHunch(&Dom, i, FALSE, FALSE, FALSE);
		}
		nCountHunchRebar	= pBri->m_pArrRCMainLowerSlabHunch[0].GetSize();
		for(i = 0; i < nCountHunchRebar; i++)
		{
			DBStd.m_pDrawVertSection->DrawRebarSupport_VertSection_LowerSlabHunch(&Dom, i, FALSE, FALSE, FALSE);
		}
	}

	if(bDrawFooting)
	{
		// ����
		long nCountJijum	= pBri->GetCountJijum();
		for(i = 0; i < nCountJijum; i++)
		{
			CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(i);
			if(pFooting && pFooting->m_bIs)
			{
				for(upper = 0; upper < 2; upper++)
				{
					for(dan = 0; dan < MAX_REBAR_DAN; dan++)
					{
						bDimRebar = (dan==0) ? TRUE : FALSE;
						if(m_nTab != m_nTabArchJijumSupport)
						{
							bDimRebar	= m_nTab == m_nTabFooting[i] ? bDimRebar : FALSE;
						}
						else
						{
							CCellID cell	= m_Grid.GetFocusCell();
							CString sTitle	= m_Grid.GetTextMatrix(cell.row, 0);
// 							if(sTitle.Find("����", 0) != -1 && i == 0)
// 								bDimRebar	= bDimRebar;
// 							else if(sTitle.Find("����", 0) != -1 && i == nCountJijum-1)
// 								bDimRebar	= bDimRebar;
// 							else
// 								bDimRebar	= FALSE;

							if(((sTitle.Find("����", 0) != -1 && i == 0) && (sTitle.Find("����", 0) != -1 && i == nCountJijum-1)) == FALSE)
								bDimRebar	= FALSE;

						}
						DBStd.m_pDrawFooting->DrawRebarSupport_Front(&Dom, pFooting, upper, dan, bDimFooting&&bDimSupportRebar&&bDimRebar, bDimFooting&&bDimSupportRebar&&bDimRebar, !upper&&bDimSupportRebar, TRUE, bVertDir ? -1 : 1);	
						if(upper==0)
						{
							DBStd.m_pDrawFooting->DrawRebarSupport_FrontSlope(&Dom, pFooting, dan, bDimArchJijum&&bDimRebar, bDimArchJijum&&bDimRebar, TRUE);
							DBStd.m_pDrawFooting->DrawRebarMain_FrontSlope(&Dom, pFooting, dan, bDimArchJijum&&bDimRebar, bDimArchJijum&&bDimRebar, TRUE);
						}
					}
				}
				bDimRebar = m_nTab == m_nTabFooting[i] ? TRUE : FALSE;
				DBStd.m_pDrawFooting->DrawRebarSideOut_Front(&Dom, pFooting, TRUE, TRUE, bDimRebar, bDimRebar, TRUE);
				DBStd.m_pDrawFooting->DrawRebarSideOut_Front(&Dom, pFooting, FALSE, TRUE, FALSE, FALSE, TRUE);

				DBStd.m_pDrawFooting->DrawRebarShear_Front(&Dom, pFooting, TRUE, bDimFooting&&bDimShearRebar&&bDimRebar, bDimFooting&&bDimShearRebar&&bDimRebar, TRUE, 0, TRUE, bVertDir ? -1 : 1);


				// ���� �Է��Ҷ��� ��ٿ� �����ϱ� ���� ������ ġ���� �־� �ش�. /////
				if(m_nTab == m_nTabFooting[i])
				{
					if(nJ == 0 || nJ == pBri->m_nQtyJigan)
					{
						CWallApp wall;
						pBri->MakeWallByOutWall(wall, nJ==0, FALSE);
						DBStd.m_pDrawWall->DimFootingUpperReferForSupAndShearRebar(&Dom, &wall);
					}
					else
					{
						CWallApp *pWall	= pBri->GetInWall(nJ-1);
						if(pWall)
						{
							if(pWall->m_bIs)
							{
								DBStd.m_pDrawWall->DimFootingUpperReferForSupAndShearRebar(&Dom, pWall);
							}
						}
					}
				}
				//////////////////////////////////////////////////////////////////
			}
		}
	}

	// ������ �� ��ġ ǥ��(�ﰢ��)
	for(nJ = 0; nJ < pBri->GetCountJijum(); nJ++)
		DBStd.m_pDrawVertSection->DrawJijumSign(&Dom, nJ);

	pBri->GetTvVertSection_Std(0, !bVertDir, FALSE, TRUE);
	*pDom << Dom;
	
	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

void CInputRebarSection::SetDataInit()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	CHgBaseConcStd baseConc;

	// �׸��� ������ ���� ��� �Ǵ�.
	BOOL bSupportRebar = (m_nTabRebar == m_nTabRebarSupport) ? TRUE : FALSE;
	if(m_nTab == m_nTabUpperSlab)
	{
		// ��ν������ �׻� ����.
	}
	else if(m_nTab == m_nTabLowerSlab)
	{
		// �Ϻ� ������� BoxType�϶��� ����
		if(pBri->IsBoxType() == FALSE)
		{
			m_pIn->GridMessage(&m_Grid, "�Ϻ� ������� �������� �ʽ��ϴ�");
			return;
		}
	}
	else if(m_nTab == m_nTabOutWallStt)
	{
		if(!pBri->IsOutWall(TRUE))
		{
			m_pIn->GridMessage(&m_Grid, "������ ��ü�� �������� �ʽ��ϴ�");
			return;
		}
	}
	else if(m_nTab == m_nTabOutWallEnd)
	{
		if(!pBri->IsOutWall(FALSE))
		{
			m_pIn->GridMessage(&m_Grid, "������ ��ü�� �������� �ʽ��ϴ�");
			return;
		}
	}
	else if(m_nTab == m_nTabBracket)
	{
		if(bSupportRebar)
			return;
	}
	else if(m_nTab == m_nTabArchRib)
	{
		if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH)
		{
			m_pIn->GridMessage(&m_Grid, "��ġ���갡 ���� ���� �ʽ��ϴ�");
			return;
		}
	}
	else if(m_nTab == m_nTabArchJijumSupport)
	{
		if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH)
		{
			m_pIn->GridMessage(&m_Grid, "��ġ���갡 ���� ���� �ʽ��ϴ�.");
		}
	}
	else if(m_nTab == m_nTabArchConnectStirrup)
	{
		if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH)
		{
			m_pIn->GridMessage(&m_Grid, "��ġ���갡 ���� ���� �ʽ��ϴ�.");
		}
	}
	else 
	{
		BOOL bIsInWall = FALSE;
		BOOL bIsFooting = FALSE;
		long ix =0;
		for (ix=0; ix< MAX_JIJUM_COUNT_RC; ix++)
		{
			if(m_nTab == m_nTabInWall[ix])
			{
				bIsInWall = TRUE;
				break;
			}
			if(m_nTab == m_nTabFooting[ix])
			{
				bIsFooting;
				break;
			}
		}

		//���� ö�� or ���
		if(bIsInWall)
		{
			if(m_nTab == m_nTabInWall[ix])
			{
				if(pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB)
				{
					m_pIn->GridMessage(&m_Grid, "�߰����� ��ü�� �������� �ʽ��ϴ�");
					return;
				}
			}
		}
		else //����
		{
			if(m_nTab == m_nTabFooting[ix])
			{
				CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(ix);
				if(pFooting)
				{
					if(pFooting->m_bIs)
						bIsFooting	= TRUE;
				}
			}
			if(!bIsFooting)
			{
				m_pIn->GridMessage(&m_Grid, "�ش� ������ ���ʰ� �������� �ʽ��ϴ�");
				return;
			}
		}
	}

	m_Grid.UpdateData(FALSE);


	long nRows(0), nCols(0);
	m_Grid.GetCellCount(nRows, nCols);
	if(m_nTab == m_nTabArchJijumSupport)
	{
		m_Grid.MergeGrid(0, 0, 0, nCols);
	}
	else
	{
		m_Grid.MergeGrid(0, m_nTabRebar == m_nTabRebarSupport ? 0 : 1, 1, nCols);
		m_Grid.MergeGrid(0, 1, 0, 0);
	}

	// ��Ʈ�Ѱ� ���
	BOOL bIsShear = FALSE;
	for (long ix =0; ix < MAX_JIJUM_COUNT_RC; ix++)
	{
		if(m_nTab == m_nTabInWall[ix])
		{
			CWallApp *pWallTmp = pBri->GetInWall(ix);
			if(pWallTmp && bIsShear == FALSE)
			{
				m_comboCTC.SetCurSel(baseConc.GetIdxByRebarCTC(pWallTmp->m_dCTC_SupportRebar));
				break;
			}
		}
		else
		{
			m_comboCTC.SetCurSel(baseConc.GetIdxByRebarCTC(pBri->m_dCTC_SupportRebar[iUPPER]));
		}
	}
	CPlaceRebarByBlock *pPlace	= NULL;

	pPlace	= GetPlaceRebarByBlock(TRUE);
	if(pPlace) m_comboUpper.SetCurSel(pPlace->m_xyArr.GetSize()-1);
	pPlace	= GetPlaceRebarByBlock(FALSE);
	if(pPlace) m_comboLower.SetCurSel(pPlace->m_xyArr.GetSize()-1);

	m_dAccu	= pBri->m_dAccuracyArchRibLowerSupportDim;
	UpdateData(FALSE);
}


void CInputRebarSection::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	structGridCell *pCell = m_Grid.GetCurrentCell();
	
	if(m_nTab == m_nTabArchJijumSupport)
	{
		DrawInputDomyunView(FALSE);
	}
	if(pCell)
	{
		m_pIn->SetComment(pCell->sTitle);
		
		CString sCursor	= m_Grid.GetItemText(1, pCell->nCol);
		sCursor.Replace("\n", "");
		m_pView->GetDomyun()->STMakeCursor(sCursor);
	}
	

}

void CInputRebarSection::OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result)
{
	m_CellID	= m_Grid.GetFocusCell();
}

void CInputRebarSection::SetControl()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;


	// ���� �޺� ����
	if(m_nTabRebar == m_nTabRebarSupport)
	{
		if(m_nTab == m_nTabUpperSlab || m_nTab == m_nTabLowerSlab)
		{
			GetDlgItem(IDC_STATIC_UPPER)->SetWindowText("����ġ");
			GetDlgItem(IDC_STATIC_LOWER)->SetWindowText("�ϸ��ġ");
		}
		else if(m_nTab == m_nTabOutWallStt || m_nTab == m_nTabOutWallEnd)
		{
			GetDlgItem(IDC_STATIC_UPPER)->SetWindowText("������ġ");
			GetDlgItem(IDC_STATIC_LOWER)->SetWindowText("������ġ");
		}
		else if(m_nTab == m_nTabBracket)
		{
			GetDlgItem(IDC_STATIC_UPPER)->SetWindowText("������ġ");
			GetDlgItem(IDC_STATIC_LOWER)->SetWindowText("������ġ");
		}
		else
		{
			GetDlgItem(IDC_STATIC_UPPER)->SetWindowText("����ġ");
			GetDlgItem(IDC_STATIC_LOWER)->SetWindowText("�ϸ��ġ");
		}
		
		for (long ix =0; ix < MAX_JIJUM_COUNT_RC; ix++)
		{
			if(m_nTab == m_nTabInWall[ix])
			{
				GetDlgItem(IDC_STATIC_UPPER)->SetWindowText("������ġ");
				GetDlgItem(IDC_STATIC_LOWER)->SetWindowText("������ġ");
			}
		}
	}
	else
	{
		if(m_nTab == m_nTabBracket)
		{
			GetDlgItem(IDC_STATIC_UPPER)->SetWindowText("��������");
			GetDlgItem(IDC_STATIC_LOWER)->SetWindowText("��������");
		}
		else
		{
			GetDlgItem(IDC_STATIC_UPPER)->SetWindowText("���ܹ�ġ");
			GetDlgItem(IDC_STATIC_LOWER)->SetWindowText("���ܹ�ġ");
		}
	}
	
	if(m_nTabRebar	== m_nTabRebarSupport)
	{
		if(m_nTab == m_nTabBracket)
		{
			m_comboUpper.EnableWindow(FALSE);
			m_comboLower.EnableWindow(FALSE);
		}
		else
		{
			m_comboUpper.EnableWindow(TRUE);
			m_comboLower.EnableWindow(TRUE);
		}
	}
	else
	{
		if(m_nTab == m_nTabBracket)
		{
			m_comboUpper.EnableWindow(TRUE);
			m_comboLower.EnableWindow(TRUE);
		}
		else
		{
			m_comboUpper.EnableWindow(TRUE);
			m_comboLower.EnableWindow(FALSE);
		}
	}

	// ������ü ��ٽ� ��� ��ս��̸� �޺��� ��� disable
	//{31543
	for(long ix =0; ix < MAX_JIJUM_COUNT_RC; ix++)
	{
		if(m_nTab == m_nTabInWall[ix] && !IsWallType())
		{
			m_comboUpper.EnableWindow(FALSE);
			m_comboLower.EnableWindow(FALSE);
			break;
		}
	}

	// ��ġ���� �����θ� ��� disable
	if(m_nTab == m_nTabArchJijumSupport)
	{
		m_comboUpper.EnableWindow(FALSE);
		m_comboLower.EnableWindow(FALSE);
	}

	if(m_nTab == m_nTabArchRib)
	{
		GetDlgItem(IDC_STATIC_ACCU)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_MM)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_ACCU)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_STATIC_ACCU)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_MM)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_ACCU)->ShowWindow(FALSE);
	}

	
	BOOL bFooting	= FALSE;
	long footing = 0; for(footing = 0; footing < MAX_JIJUM_COUNT_RC; footing++)
	{
		if(m_nTab == m_nTabFooting[footing])
			bFooting	= TRUE;
	}

	if(bFooting)
	{
		m_btnSameSttEnd.EnableWindow(TRUE);
		m_btnSameInWall.EnableWindow(TRUE);
	}
	else
	{
		m_btnSameSttEnd.EnableWindow(FALSE);
		m_btnSameInWall.EnableWindow(FALSE);
	}

	// ĵƿ���� ����
	m_btnCheckCantil.EnableWindow(pBri->IsCantileverDesign());

	// ǥ�ع�ġ ����
	CHgBaseConcStd baseConc;
	if(pBri->m_nIdxCTC_SupportAndShearRebar > -1)
		m_comboCTC.SetCurSel(baseConc.GetIdxByRebarCTC(pBri->m_dCTC_SupportRebar[iUPPER]));

	if(m_nTab == m_nTabBracket && m_nTabRebar == m_nTabRebarSupport)
		m_comboCTCPart.EnableWindow(FALSE);
	else
		m_comboCTCPart.EnableWindow(TRUE);


	// ������౸�� ���� ���� ���
	m_checkSameOffset.SetCheck(pBri->m_bSameOffsetPlaceTAndCBlock);
}

void CInputRebarSection::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	CCellID cell		= m_Grid.GetFocusCell();

	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);
}

void CInputRebarSection::OnButtonRebarSupport() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	CRebarSupportDlg dlg(pBri, m_pIn->m_pARcBridgeDataStd);
	if(dlg.DoModal() == IDOK)
	{
		SyncRebar();
		DrawInputDomyunView(FALSE);

		// �����༱ �и� ����
		m_pIn->m_pARcBridgeDataStd->CopyToCoupleBridge_RebarStandardSection(m_pIn->m_nCurrentBridge);

		if(m_pDlgCheck->GetSafeHwnd() && m_pDlgCheck->IsWindowVisible())
		{
			pBri->MakeRebarInfoCycleStd(TRUE);
			m_pDlgCheck->Check();
		}

		SetCTCPart();
	}
}

void CInputRebarSection::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_nTab	= m_Tab.GetCurSel();
	SetGridData();
	SetDataInit();
	SetControl();
	
	DrawInputDomyunView(FALSE);
	SetCTCPart();
	*pResult = 0;
}

void CInputRebarSection::OnButtonRebarShear() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	CRebarShearDlg dlg(pBri, m_pIn->m_pARcBridgeDataStd);
	if(dlg.DoModal() == IDOK)
	{
		SyncRebar();
		DrawInputDomyunView(FALSE);

		// �����༱ �и� ����
		m_pIn->m_pARcBridgeDataStd->CopyToCoupleBridge_RebarStandardSection(m_pIn->m_nCurrentBridge);

		if(m_pDlgCheck->GetSafeHwnd() && m_pDlgCheck->IsWindowVisible())
		{
			pBri->MakeRebarInfoCycleStd(TRUE);
			m_pDlgCheck->Check();
		}
	}
}

void CInputRebarSection::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	
	*pResult = 0;
}

void CInputRebarSection::SetGridData_UpperSlab(BOOL bSupportRebar)
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 2, 1);
	m_Grid.SetCellSize(20, 50);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	
	long row		= 0, col = 0;
	CString str		= _T("");
	CString sHelp	= _T("");
	CString sUpdn	= _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	CPlaceRebarByBlock *pPlace	= NULL;
	long block(0);
	// ����
	m_Grid.AddCell(row, col, "����\n");
	m_Grid.AddCell(row+1, col, "����\n");
	m_Grid.AddCell(row+2, col, "����");
	m_Grid.AddCell(row+3, col++, "����");

	// ���ö��
	if(bSupportRebar)
	{
		// ���
		pPlace	= &pBri->m_placeByBlock_SupportRebar_UpperSlab[iUPPER][0];
		pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();
		for(block = 0; block < pPlace->m_xyArr.GetSize(); block++)
		{
			str.Format("U%d", block+1);
			m_Grid.AddCell(row, col, "���");
			m_Grid.AddCell(row+1, col, str);
			m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.AddCell(row+3, col++, &pPlace->m_xyArr, block, FALSE, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
		}


		// �ϸ�
		pPlace	= &pBri->m_placeByBlock_SupportRebar_UpperSlab[iLOWER][0];
		pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();
		for(block = 0; block < pPlace->m_xyArr.GetSize(); block++)
		{
			str.Format("L%d", block+1);
			m_Grid.AddCell(row, col, "�ϸ�");
			m_Grid.AddCell(row+1, col, str); m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
			m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine); m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2); m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.AddCell(row+3, col++, &pPlace->m_xyArr, block, FALSE, nFormat, nLine); m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2); m_Grid.SetCellDec(CELL_DEC_0);
		}
	}
	// ����ö��
	else
	{
		pPlace	= &pBri->m_placeByBlock_ShearRebar_UpperSlab;
		pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();
		for(block = 0; block < pPlace->m_xyArr.GetSize(); block++)
		{
			str.Format("V%d\n", block+1);
			m_Grid.AddCell(row, col, str);
			m_Grid.AddCell(row+1, col, str);
			m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.AddCell(row+3, col++, &pPlace->m_xyArr, block, FALSE, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
		}
	}
}

void CInputRebarSection::SetGridData_OutWall(BOOL bStt, BOOL bSupportRebar)
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 2, 1);
	m_Grid.SetCellSize(20, 50);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	
	long row		= 0, col = 0;
	CString str		= _T("");
	CString sHelp	= _T("");
	CString sUpdn	= _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	CPlaceRebarByBlock *pPlace	= NULL;
	long block(0);
	long stt	= bStt ? iSTT : iEND;
	CString sStt	= bStt ? "������ ��ü" : "������ ��ü";

	// ����
	m_Grid.AddCell(row, col, "����\n");
	m_Grid.AddCell(row+1, col, "����\n");
	m_Grid.AddCell(row+2, col, "����");
	m_Grid.AddCell(row+3, col++, "����");

	// ���ö��
	if(bSupportRebar)
	{
		// ����
		pPlace	= &pBri->m_placeByBlock_SupportRebar_OutWall[stt][iOUTTER][0];
		pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();
		for(block = 0; block < pPlace->m_xyArr.GetSize(); block++)
		{
			str.Format("O%d", block+1);
			m_Grid.AddCell(row, col, "���");
			m_Grid.AddCell(row+1, col, str);
			m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.AddCell(row+3, col++, &pPlace->m_xyArr, block, FALSE, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
		}


		// ����
		pPlace	= &pBri->m_placeByBlock_SupportRebar_OutWall[stt][iINNER][0];
		pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();
		for(block = 0; block < pPlace->m_xyArr.GetSize(); block++)
		{
			str.Format("I%d", block+1);
			m_Grid.AddCell(row, col, "����");
			m_Grid.AddCell(row+1, col, str); m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
			m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine); m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2); m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.AddCell(row+3, col++, &pPlace->m_xyArr, block, FALSE, nFormat, nLine); m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2); m_Grid.SetCellDec(CELL_DEC_0);
		}
	}
	// ����ö��
	else
	{
		pPlace	= &pBri->m_placeByBlock_ShearRebar_OutWall[stt];
		pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();
		for(block = 0; block < pPlace->m_xyArr.GetSize(); block++)
		{
			str.Format("V%d\n", block+1);
			m_Grid.AddCell(row, col, str);
			m_Grid.AddCell(row+1, col, str);
			m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.AddCell(row+3, col++, &pPlace->m_xyArr, block, FALSE, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
		}
	}
}

// 
void CInputRebarSection::SetGridData_InWall(BOOL bSupportRebar)
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 2, 1);
	m_Grid.SetCellSize(20, 50);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	
	long block(0), wall(0);

	// ��ü���� �ϳ��� ������� �޼��� ǥ��
	if(!IsWallType())
	{
		m_pIn->GridMessage(&m_Grid, "�߰����� ��ü�� ��� ��ս��Դϴ�");
		return;
	}

	long row		= 0, col = 0;
	CString str		= _T("");
	CString sHelp	= _T("");
	CString sUpdn	= _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	CString sWall	= _T("");
	CPlaceRebarByBlock *pPlace	= NULL;
	
	// ����
	m_Grid.AddCell(row, col, "����\n");
	m_Grid.AddCell(row+1, col, "����\n");
	m_Grid.AddCell(row+2, col, "����");
	m_Grid.AddCell(row+3, col++, "����");

	//31543
	long nWall = 0;
	for (long ix =0; ix < MAX_JIJUM_COUNT_RC ; ix++)
	{
		if(m_nTab == m_nTabInWall[ix])
		{
			nWall = ix;
			break;
		}
	}

	wall = nWall;

	// ���ö��
	if(bSupportRebar)
	{
		CWallApp *pWall	= pBri->GetInWall(wall);
		if(!pWall) return;//continue; //todo 31543
		nLine			= pWall->m_bTopIsColumn ? CELL_READONLY : CELL_LINE_NO;
		sWall.Format("�߰�����%d", wall+1);

		// ����
		pPlace	= &pWall->m_placeByBlock_SupportRebar[iLEFT][0];
		pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();
		for(block = 0; block < pPlace->m_xyArr.GetSize(); block++)
		{
			str.Format("L%d", block+1);
			if(pWall->IsRoundRectWall())
				m_Grid.AddCell(row, col, sWall);
			else
				m_Grid.AddCell(row, col, sWall+"(����)");
			m_Grid.AddCell(row+1, col, str);
			m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.AddCell(row+3, col++, &pPlace->m_xyArr, block, FALSE, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
		}

		// ����� ��ü�� ��ö�� ���� ��±��� �Է� �޴� ���̹Ƿ� ���ʸ� ����
		if(!pWall->m_bRoundRect)
		{
			// ����
			pPlace	= &pWall->m_placeByBlock_SupportRebar[iRIGHT][0];
			pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();
			for(block = 0; block < pPlace->m_xyArr.GetSize(); block++)
			{
				str.Format("R%d", block+1);
				m_Grid.AddCell(row, col, sWall+"(����)");
				m_Grid.AddCell(row+1, col, str); m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
				m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine); m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2); m_Grid.SetCellDec(CELL_DEC_0);
				m_Grid.AddCell(row+3, col++, &pPlace->m_xyArr, block, FALSE, nFormat, nLine); m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2); m_Grid.SetCellDec(CELL_DEC_0);
			}
		}
	}
	// ����ö��
	else
	{
		CWallApp *pWall	= pBri->GetInWall(wall);
		if(!pWall) return; //continue;
		nLine			= pWall->m_bTopIsColumn ? CELL_READONLY : CELL_LINE_NO;
		sWall.Format("�߰�����%d", wall+1);

		pPlace	= &pWall->m_placeByBlock_ShearRebar;
		pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();
		for(block = 0; block < pPlace->m_xyArr.GetSize(); block++)
		{
			str.Format("V%d", block+1);
			m_Grid.AddCell(row, col, sWall);
			m_Grid.AddCell(row+1, col, str);
			m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.AddCell(row+3, col++, &pPlace->m_xyArr, block, FALSE, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
		}
	}
}

void CInputRebarSection::SetGridData_Footing(long nIdx, BOOL bSupportRebar)
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 2, 1);
	m_Grid.SetCellSize(20, 50);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(nIdx);
	if(!pFooting) return;
	
	long row		= 0, col = 0;
	CString str		= _T("");
	CString sHelp	= _T("");
	CString sUpdn	= _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;
	CString sFooting	= "����";
	
	CPlaceRebarByBlock *pPlace	= NULL;
	long block(0);
	// ����
	m_Grid.AddCell(row, col, "����\n");
	m_Grid.AddCell(row+1, col, "����\n");
	m_Grid.AddCell(row+2, col, "����");
	m_Grid.AddCell(row+3, col++, "����");

	// ���ö��
	if(bSupportRebar)
	{
		// ���
		pPlace	= &pFooting->m_placeByBlock_Support_Front[iUPPER][0];
		pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();
		for(block = 0; block < pPlace->m_xyArr.GetSize(); block++)
		{
			str.Format("U%d", block+1);
			m_Grid.AddCell(row, col, "���");
			m_Grid.AddCell(row+1, col, str);
			m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.AddCell(row+3, col++, &pPlace->m_xyArr, block, FALSE, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
		}

		// �ϸ�
		pPlace	= &pFooting->m_placeByBlock_Support_Front[iLOWER][0];
		pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();
		for(block = 0; block < pPlace->m_xyArr.GetSize(); block++)
		{
			str.Format("L%d", block+1);
			m_Grid.AddCell(row, col, "�ϸ�");
			m_Grid.AddCell(row+1, col, str); m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
			m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine); m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2); m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.AddCell(row+3, col++, &pPlace->m_xyArr, block, FALSE, nFormat, nLine); m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2); m_Grid.SetCellDec(CELL_DEC_0);
		}

		// ����ö��
		pPlace	= &pFooting->m_placeByBlock_Side_Out;
		pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();
		for(block = 0; block < pPlace->m_xyArr.GetSize(); block++)
		{
			str.Format("S%d", block+1);
			m_Grid.AddCell(row, col, "����");
			m_Grid.AddCell(row+1, col, str); m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
			m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine); m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR3); m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.AddCell(row+3, col++, &pPlace->m_xyArr, block, FALSE, nFormat, nLine); m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR3); m_Grid.SetCellDec(CELL_DEC_0);
		}
	}
	// ����ö��
	else
	{
		pPlace	= &pFooting->m_placeByBlock_Shear_Front;
		pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();
		for(block = 0; block < pPlace->m_xyArr.GetSize(); block++)
		{
			str.Format("V%d\n", block+1);
			m_Grid.AddCell(row, col, str);
			m_Grid.AddCell(row+1, col, str);
			m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.AddCell(row+3, col++, &pPlace->m_xyArr, block, FALSE, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
		}
	}
}

// ���� �޺� ����
void CInputRebarSection::SetCombo_Block()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long nCountUpper			= 1;
	long nCountLower			= 1;
	CPlaceRebarByBlock *pPlace	= NULL;
	if(1)//m_nTab == m_nTabUpperSlab)
	{
		pPlace	= GetPlaceRebarByBlock(TRUE);
		if(pPlace)
			nCountUpper	= pPlace->m_xyArr.GetSize();

		pPlace		= GetPlaceRebarByBlock(FALSE);
		if(pPlace)
			nCountLower	= pPlace->m_xyArr.GetSize();
	}

	m_comboUpper.SetCurSel(nCountUpper-1);
	m_comboLower.SetCurSel(nCountLower-1);
}

// ���� ������� placerebarbyblock ������ ����
CPlaceRebarByBlock* CInputRebarSection::GetPlaceRebarByBlock(BOOL bUpperOrOutterOrLeft)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return NULL;

	BOOL bSupportRebar = (m_nTabRebar == m_nTabRebarSupport) ? TRUE : FALSE;
	CPlaceRebarByBlock *pPlace	= NULL;
	if(!bSupportRebar && !bUpperOrOutterOrLeft && m_nTab != m_nTabBracket) return pPlace;	// ���ö���� �Ϻα��� �Է��� ���� ����

	if(m_nTab == m_nTabUpperSlab)
	{
		if(bSupportRebar)
		{
			long upper	= bUpperOrOutterOrLeft ? iUPPER : iLOWER;
			pPlace	= &pBri->m_placeByBlock_SupportRebar_UpperSlab[upper][0];
		}
		else
		{
			pPlace	= &pBri->m_placeByBlock_ShearRebar_UpperSlab;
		}
	}
	else if(m_nTab == m_nTabLowerSlab)
	{
		if(bSupportRebar)
		{
			long upper	= bUpperOrOutterOrLeft ? iUPPER : iLOWER;
			pPlace	= &pBri->m_placeByBlock_SupportRebar_LowerSlab[upper][0];
		}
		else
		{
			pPlace	= &pBri->m_placeByBlock_ShearRebar_LowerSlab;
		}
	}
	else if(m_nTab == m_nTabOutWallStt)
	{
		if(bSupportRebar)
		{
			long inner	= bUpperOrOutterOrLeft ? iOUTTER : iINNER;
			pPlace	= &pBri->m_placeByBlock_SupportRebar_OutWall[iSTT][inner][0];
		}
		else
		{
			pPlace	= &pBri->m_placeByBlock_ShearRebar_OutWall[iSTT];
		}
	}
	else if(m_nTab == m_nTabOutWallEnd)
	{
		if(bSupportRebar)
		{
			long inner	= bUpperOrOutterOrLeft ? iOUTTER : iINNER;
			pPlace	= &pBri->m_placeByBlock_SupportRebar_OutWall[iEND][inner][0];
		}
		else
		{
			pPlace	= &pBri->m_placeByBlock_ShearRebar_OutWall[iEND];
		}
	}
	else if(m_nTab == m_nTabBracket)
	{
		pPlace	= bUpperOrOutterOrLeft ? &pBri->m_placeByBlock_ShearRebar_Bracket[iSTT] : &pBri->m_placeByBlock_ShearRebar_Bracket[iEND];
	}
	else if(m_nTab == m_nTabArchRib)
	{
		if(bSupportRebar)
		{
			pPlace	= bUpperOrOutterOrLeft ? &pBri->m_placeByBlock_SupportRebar_ArchRib[iUPPER][0][iLEFT] : &pBri->m_placeByBlock_SupportRebar_ArchRib[iUPPER][0][iRIGHT];
		}
		else
		{
			pPlace	= bUpperOrOutterOrLeft ? &pBri->m_placeByBlock_ShearRebar_ArchRib[iLEFT] : &pBri->m_placeByBlock_ShearRebar_ArchRib[iRIGHT];
		}
	}
	else if(m_nTab == m_nTabArchJijumSupport)
	{
		pPlace	= bUpperOrOutterOrLeft ? &pBri->m_footingStt.m_placeByBlock_Support_FrontSlope : &pBri->m_footingEnd.m_placeByBlock_Support_FrontSlope;
	}
	else 
	{
		//31543
		long ix =0;
		for (ix=0; ix< MAX_JIJUM_COUNT_RC; ix++)
		{
			if(m_nTab == m_nTabInWall[ix])
			{
				long left	= bUpperOrOutterOrLeft ? iLEFT : iRIGHT;

				CWallApp *pWall = pBri->GetInWall(ix);
				if(pWall)
				{
					if(bSupportRebar)
					{
						pPlace	= &pWall->m_placeByBlock_SupportRebar[left][0];
					}
					else
					{
						pPlace	= &pWall->m_placeByBlock_ShearRebar;
					}
				}
			}
			if(m_nTab == m_nTabFooting[ix])
			{
				if(m_nTab == m_nTabFooting[ix])
				{
					long upper	= bUpperOrOutterOrLeft ? iUPPER : iLOWER;
					CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(ix);
					if(pFooting)
					{
						if(bSupportRebar)
						{
							pPlace	= &pFooting->m_placeByBlock_Support_Front[upper][0];
						}
						else
						{
							pPlace	= &pFooting->m_placeByBlock_Shear_Front;
						}
					}
				}
			}
		}		
	}

	return pPlace;
}

void CInputRebarSection::OnSelchangeComboUpper() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	CPlaceRebarByBlock *pPlace	= GetPlaceRebarByBlock(TRUE);
	if(pPlace)
	{
		pPlace->m_nCountBlock	= m_comboUpper.GetCurSel()+1;
		pPlace->FitCount();
		SetGridData();
		SetDataInit();
	}

	DrawInputDomyunView(FALSE);
}

void CInputRebarSection::OnSelchangeComboLower() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	CPlaceRebarByBlock *pPlace	= GetPlaceRebarByBlock(FALSE);
	if(pPlace)
	{
		pPlace->m_nCountBlock	= m_comboLower.GetCurSel()+1;
		pPlace->FitCount();
		SetGridData();
		SetDataInit();
	}	

	DrawInputDomyunView(FALSE);
}

void CInputRebarSection::OnCheckSameoffsetTandc() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	
	if(AfxMessageBox("ö�ٹ�ġ�� �� �����Ͻðڽ��ϱ�?", MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		m_checkSameOffset.SetCheck(!m_checkSameOffset.GetCheck());
		return;
	}


	pBri->m_bSameOffsetPlaceTAndCBlock	= m_checkSameOffset.GetCheck();

	// ��±�, ����ö�� ��ü �ǰ�� ��.
	pBri->SyncBridgeRebar(TRUE, 0, FALSE);
	pBri->SetDataDefaultRebarSupport_VertSection_Std(-1, -1);
	pBri->SetDataDefaultRebarShear_VertSection_Std(-1);

	// ���ʴ� ��ö�ٵ� �ǰ�� ����� ��.
	long nCountFooting	= pBri->GetCountFooting();
	long footing = 0; for(footing = 0; footing < nCountFooting; footing++)
	{
		CFootingApp *pFooting	= pBri->GetFooting(footing);
		if(pFooting)
		{
			long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				pFooting->SetDataDefaultRebarMain_Side(TRUE, dan);
				pFooting->SetDataDefaultRebarMain_Side(FALSE, dan);
			}
			pFooting->SetDataDefaultRebarShear_Front(FALSE);
		}
	}

	SetDataDefault();
	SyncRebar();
	SetGridData();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}


// ���� ������ �ִ� ��ü�� ��ü�� ��ü�� �ϳ��� �ִ���..
BOOL CInputRebarSection::IsWallType()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return FALSE;

	long nCountInWall	= pBri->GetCountInWall();
	BOOL bWall			= FALSE;
	long wall = 0; for(wall = 0; wall < nCountInWall; wall++)
	{
		CWallApp *pWall	= pBri->GetInWall(wall);
		if(pWall)
		{
			if(!pWall->m_bTopIsColumn)
			{
				bWall	= TRUE;
				break;
			}
		}
	}

	

	return bWall;
}

void CInputRebarSection::OnSelchangeComboCtc() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	
	// 
	if(AfxMessageBox("ǥ�� ���� ����� ���ö�ٰ� ����ö���� �ǰ���� ��ü������ ����˴ϴ�. ��� �Ͻðڽ��ϱ�?", MB_YESNO | MB_ICONQUESTION)==IDNO)
	{
		m_comboCTC.SetCurSel(pBri->m_nIdxCTC_SupportAndShearRebar);

		return;
	}

	// ��Ʈ�� �� ����
	CHgBaseConcStd baseConc;
	pBri->m_dCTC_SupportRebar[iUPPER]	= baseConc.GetValueRebarCTC(m_comboCTC.GetCurSel());
	pBri->m_dCTC_SupportRebar[iLOWER]	= baseConc.GetValueRebarCTC(m_comboCTC.GetCurSel());
	pBri->m_dCTC_ShearRebar[iUPPER]		= pBri->m_dCTC_SupportRebar[iUPPER];
	pBri->m_dCTC_ShearRebar[iLOWER]		= pBri->m_dCTC_SupportRebar[iLOWER];

	// ������ü
	for(long stt = 0; stt < 2; stt++)
	{
		pBri->m_dCTC_SupportRebarOutWall[stt]	= pBri->m_dCTC_SupportRebar[iUPPER];
		pBri->m_dCTC_ShearRebarOutWall[stt]		= pBri->m_dCTC_ShearRebar[iUPPER];
	}


	// ������ü
	long nCountWall	= pBri->GetCountInWall();
	long wall = 0; for(wall = 0; wall < nCountWall; wall++)
	{
		CWallApp *pWall	= pBri->GetInWall(wall);
		if(pWall)
		{
			pWall->m_dCTC_SupportRebar		= pBri->m_dCTC_SupportRebar[iUPPER];
			pWall->m_dCTC_ShearRebar		= pBri->m_dCTC_ShearRebar[iUPPER];
		}
	}

	// ����
	long nCountFooting = pBri->GetCountFooting();
	long footing = 0; for(footing = 0; footing < nCountFooting; footing++)
	{
		CFootingApp *pFooting	= pBri->GetFooting(footing);
		if(pFooting)
		{
			pFooting->m_dCTC_SupportRebar		= pBri->m_dCTC_SupportRebar[iUPPER];
			pFooting->m_dCTC_ShearRebar		= pBri->m_dCTC_ShearRebar[iUPPER];
		}
	}
	

	// ������ ����Ǹ� �ǰ���� �ٽ� �� �ش�.
	m_bChangeCTC	= TRUE;
	m_bClickDefaultButton	= TRUE;
	SetDataDefault();
	SetDataSave();
	DrawInputDomyunView(FALSE);

	SetCTCPart();
}


// 2,3�ܿ� ���ؼ��� �Է��� ���� �ʴ� ��... ������ �Է¹��� �ʴ� ����Ÿ�� ���ؼ� ����ȭ ���ش�.
void CInputRebarSection::SyncRebar()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long nCountInWall	= pBri->GetCountInWall();
	long nCountFooting	= pBri->GetCountFooting();

	// ����ö���� ���� �Է� �����Ƿ� ������ CPlaceByBlock�� �־��� /////////////////////////////////////
	pBri->m_placeByBlock_ShearRebar_UpperSlab.m_dDia	= pBri->m_RebarShearDetail_UpperSlab.m_dDia;
	pBri->m_placeByBlock_ShearRebar_LowerSlab.m_dDia	= pBri->m_RebarShearDetail_LowerSlab.m_dDia;
	pBri->m_placeByBlock_ShearRebar_OutWall[iSTT].m_dDia	= pBri->m_RebarShearDetail_OutWall[iSTT].m_dDia;
	pBri->m_placeByBlock_ShearRebar_OutWall[iEND].m_dDia	= pBri->m_RebarShearDetail_OutWall[iEND].m_dDia;

	long wall = 0; for(wall = 0; wall < nCountInWall; wall++)
	{
		CWallApp *pWall	= pBri->GetInWall(wall);
		if(pWall)
			pWall->m_placeByBlock_ShearRebar.m_dDia	= pWall->m_rebarShearDetail.m_dDia;
	}

	long footing = 0; for(footing = 0; footing < nCountFooting; footing++)
	{
		CFootingApp *pFooting	= pBri->GetFooting(footing);
		if(pFooting)
		{
			pFooting->m_placeByBlock_Shear_Front.m_dDia = pFooting->m_rebarShearDetail.m_dDia;
			pFooting->m_placeByBlock_Shear_Side.m_dDia	= pFooting->m_placeByBlock_Shear_Front.m_dDia;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////
	
	


	// ������ü�� ù��° �Ϳ� ���ؼ��� �Է��� �ް� �������� �Է��� ���� �ʴ� ��� ������ ��ü�� �Էµ���Ÿ�� ��� �Ѱ� 
	// ��� �ȴ�.
	// ����� ��ü�� ������ ö�� �̹Ƿ� ���� ��±� ������ �Է� ���� ���� ///////////////
	// �׻� ������ �� ���� ���� ����. 
	for(HGINT32 wall = 0; wall < nCountInWall; wall++)
	{
		CWallApp *pWall	= pBri->GetInWall(wall);
		if(pWall)
		{
			if(pWall->m_bRoundRect)
			{
				long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
					pWall->m_placeByBlock_SupportRebar[iRIGHT][dan]	= pWall->m_placeByBlock_SupportRebar[iLEFT][dan];
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////

	if(m_bSamePlaceInWall)
	{
		if(nCountInWall > 1)
		{
			CWallApp *pWallSource	= pBri->GetInWall(0);
			long wall = 0; for(wall = 1; wall < nCountInWall; wall++)
			{
				CWallApp *pWall	= pBri->GetInWall(wall);
				if(pWall)
				{
					pWall->CopyPlaceRebar_SupportRebar(pWallSource);
					pWall->CopyPlaceRebar_ShearRebar(pWallSource);
				}
			}
		}
	}

	// ���ʴ� 1�ܿ� ���ؼ��� �Է��� ������ 2,3�ܿ� ���ؼ� ��� �Է��� ����� �ȴ�.
	for(footing = 0; footing < nCountFooting; footing++)
	{
		CFootingApp *pFooting	= pBri->GetFooting(footing);
		if(pFooting)
		{
			for(long upper = 0; upper < 2; upper++)
			{
				CPlaceRebarByBlock *pPlaceSource	= &pFooting->m_placeByBlock_Support_Front[upper][0];
				long dan = 0; for(dan = 1; dan < MAX_REBAR_DAN; dan++)
				{
					CPlaceRebarByBlock *pPlace		= &pFooting->m_placeByBlock_Support_Front[upper][dan];
					*pPlace	= *pPlaceSource;
				}
			}
		}
	}
}

void CInputRebarSection::OnButtonDanbu() 
{
	// TODO: Add your control notification handler code here
	
}

// ���� �Է����� Ǫ�ø���.(������ȣ ���� �ƴ�) 
long CInputRebarSection::GetIdxCurFooting()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	long nIdx	= -1;
	long footing	= -1;
	long nJ = 0; for(nJ = 0; nJ < MAX_JIJUM_COUNT_RC; nJ++)
	{
		CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(nJ);
		if(pFooting)
		{
			if(pFooting->m_bIs)
				footing++;
		}
		if(m_nTab == m_nTabFooting[nJ])
		{
			nIdx	= footing;
			break;
		}
	}


	return nIdx;
}

void CInputRebarSection::OnCheckSfd() 
{
	// TODO: Add your control notification handler code here
	DrawInputDomyunView(FALSE);
}

void CInputRebarSection::OnCheckBmd1() 
{
	// TODO: Add your control notification handler code here
	DrawInputDomyunView(FALSE);
}

void CInputRebarSection::OnButtonExamine() 
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	BOOL bFemNormal = pBri->m_FemNormal.IsOutputLoaded();
	BOOL bFemEQ     = pBri->IsRcSlab() ? TRUE : (pBri->GetFemManage(COMBO_EARTHQUAKE)->IsOutputLoaded());
	
	if(pBri->GetCountFooting()>0 && pBri->IsFirstDefault(m_pIn->m_dlgRebarColumn.GetIDD()))
	{
		AfxMessageBox("����� ����� ������並 �Ͻñ� �ٶ��ϴ�.");
		return;
	}

	if(!bFemNormal || (pBri->GetTypeDesingEQ() == 1 && !bFemEQ))
	{
		AfxMessageBox("�𵨸� �ؼ��� �����Ŀ� ������並 �Ͻñ� �ٶ��ϴ�.");
		return;
	}

	if(m_pDlgCheck->GetSafeHwnd() && m_pDlgCheck->IsWindowVisible()) m_pDlgCheck->DestroyWindow();
	m_pDlgCheck->SetBriAndContent(pBri,IDD_REBAR_SECTION,0);

	m_pDlgCheck->ShowAndCheck(this);	
	CString str = _T("");
	str.Format("���� �� ����ö�� ����");
	m_pDlgCheck->SetWindowText(str);
}

// �������� ���� ����.
void CInputRebarSection::OnButtonSamesttend2() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	ApplySameShapeSttAndEnd();
	SetGridData();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

void CInputRebarSection::OnButtonSameinwall2() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	long nIdxInWallStd	= GetJijumFooting();
	ApplySameShapeInWall(nIdxInWallStd);
	SetGridData();
	SetDataInit();
	DrawInputDomyunView(FALSE);	
}

// ������ü ���� ���� ����
// nIdxInWallStd : ������ �Ǵ� ���� ��ü
void CInputRebarSection::ApplySameShapeInWall(long nIdxInWallStd)
{
	BOOL bSupportRebar = (m_nTabRebar == m_nTabRebarSupport) ? TRUE : FALSE;
	
	CString str	= _T("");
	if(bSupportRebar)
		str.Format("��� �߰����� ������ ���ö�� ��ġ�� %d������ �����ϰ� �����Ͻðڽ��ϱ�?", nIdxInWallStd+1);
	else
		str.Format("��� �߰����� ������ ����ö�� ��ġ�� %d������ �����ϰ� �����Ͻðڽ��ϱ�?", nIdxInWallStd+1);
	if(AfxMessageBox(str, MB_YESNO) != IDYES) return;

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	
	long nCountInWall	= pBri->GetCountInWall();
	if(nCountInWall <= 1) return;

	CWallApp *pWallOrg	= pBri->GetInWall(nIdxInWallStd);
	if(!pWallOrg) return;

	CRebarPlacing rb;
	long wall = 0; for(wall = 0; wall < nCountInWall; wall++)
	{
		if(wall == nIdxInWallStd) continue;

		CWallApp *pWall	= pBri->GetInWall(wall);
		double dCover	= pWall->m_footing.GetCoverSide(TRUE, 0, FALSE);
		double dW		= pWall->m_footing.GetWidth() - (dCover * 2);
		double dLen		= 0;

		if(bSupportRebar)
		{
			for(long upper = 0; upper < 2; upper++)
			{
				long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
				{
					pWall->m_footing.m_placeByBlock_Support_Front[upper][dan]	= pWallOrg->m_footing.m_placeByBlock_Support_Front[upper][dan];
//					pWall->m_footing.m_placeByBlock_Support_Front[upper][dan].FitCount();
					rb.ReverseRebar(pWall->m_footing.m_placeByBlock_Support_Front[upper][dan].m_xyArr);
					dLen	= GetLengthPlaceByBlock(&pWall->m_footing.m_placeByBlock_Support_Front[upper][dan]);
					if(Compare(dLen, dW, "<"))
					{
						pWall->m_footing.m_placeByBlock_Support_Front[upper][dan].m_xyArr.InsertAt(0, CDPoint(1, dW-dLen));
						pWall->m_footing.m_placeByBlock_Support_Front[upper][dan].m_nCountBlock++;
					}
				}
			}
		}
		else
		{
			pWall->m_footing.m_placeByBlock_Shear_Front	= pWallOrg->m_footing.m_placeByBlock_Shear_Front;
//			pWall->m_footing.m_placeByBlock_Shear_Front.FitCount();
			rb.ReverseRebar(pWall->m_footing.m_placeByBlock_Shear_Front.m_xyArr);
			dLen	= GetLengthPlaceByBlock(&pWall->m_footing.m_placeByBlock_Shear_Front);
			if(Compare(dLen, dW, "<"))
			{
				pWall->m_footing.m_placeByBlock_Shear_Front.m_xyArr.InsertAt(0, CDPoint(1, dW-dLen));
				pWall->m_footing.m_placeByBlock_Shear_Front.m_nCountBlock++;
			}
			pWall->m_footing.m_placeByBlock_Side_Out	= pWallOrg->m_footing.m_placeByBlock_Side_Out;
		}
	}

	SetGridData();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

// ������ ������ �ܸ� ������ ���� ����.
void CInputRebarSection::ApplySameShapeSttAndEnd()
{
	BOOL bSupportRebar = (m_nTabRebar == m_nTabRebarSupport) ? TRUE : FALSE;
	BOOL bStt	= m_nTab == m_nTabFooting[0] ? TRUE : FALSE;
	CString sSrc	= bStt ? "����" : "����";
	CString sTar	= bStt ? "����" : "����";

	CString sMessage	= bSupportRebar ? sTar + "�� ������ ���ö�� ��ġ�� " + sSrc + "���� �����ϰ� �����Ͻðڽ��ϱ�?" : 
											sTar + "�� ������ ����ö�� ��ġ�� " + sSrc + "���� �����ϰ� �����Ͻðڽ��ϱ�?";
	
	if(AfxMessageBox(sMessage, MB_YESNO) != IDYES) return;

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	double dCover	= pBri->m_footingEnd.GetCoverSide(TRUE, 0, FALSE);
	double dW		= pBri->m_footingEnd.GetWidth() - (dCover * 2);
	double dLen		= 0;
	CRebarPlacing rb;
	if(bSupportRebar)
	{
		for(long upper = 0; upper < 2; upper++)
		{
			long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				CPlaceRebarByBlock *pPlaceSrc	= bStt ? &pBri->m_footingStt.m_placeByBlock_Support_Front[upper][dan] : &pBri->m_footingEnd.m_placeByBlock_Support_Front[upper][dan];
				CPlaceRebarByBlock *pPlaceTar	= bStt ? &pBri->m_footingEnd.m_placeByBlock_Support_Front[upper][dan] : &pBri->m_footingStt.m_placeByBlock_Support_Front[upper][dan];

				*pPlaceTar	= *pPlaceSrc;
				rb.ReverseRebar(pPlaceTar->m_xyArr);
				dLen	= GetLengthPlaceByBlock(pPlaceTar);
				if(Compare(dLen, dW, "<"))
				{
					pPlaceTar->m_xyArr.InsertAt(0, CDPoint(1, dW-dLen));
					pPlaceTar->m_nCountBlock++;
				}
			}
		}
	}
	else
	{
		CPlaceRebarByBlock *pPlaceSrc	= bStt ? &pBri->m_footingStt.m_placeByBlock_Shear_Front : &pBri->m_footingEnd.m_placeByBlock_Shear_Front;
		CPlaceRebarByBlock *pPlaceTar	= bStt ? &pBri->m_footingEnd.m_placeByBlock_Shear_Front : &pBri->m_footingStt.m_placeByBlock_Shear_Front;

		*pPlaceTar	= *pPlaceSrc;
		rb.ReverseRebar(pPlaceTar->m_xyArr);
		dLen	= GetLengthPlaceByBlock(pPlaceTar);
		if(Compare(dLen, dW, "<"))
		{
			pPlaceTar->m_xyArr.InsertAt(0, CDPoint(1, dW-dLen));
			pPlaceTar->m_nCountBlock++;
		}

		pPlaceSrc	= bStt ? &pBri->m_footingStt.m_placeByBlock_Side_Out : &pBri->m_footingEnd.m_placeByBlock_Side_Out;
		pPlaceTar	= bStt ? &pBri->m_footingEnd.m_placeByBlock_Side_Out : &pBri->m_footingStt.m_placeByBlock_Side_Out;
		*pPlaceTar	= *pPlaceSrc;
	}

	SetGridData();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

long CInputRebarSection::GetJijumFooting()
{
	long nIdx	= -1;
	long footing = 0; for(footing = 0; footing < MAX_JIJUM_COUNT_RC; footing++)
	{
		if(m_nTab == m_nTabFooting[footing])
		{
			nIdx	= footing;
			break;
		}
	}

	return nIdx;
}

double CInputRebarSection::GetLengthPlaceByBlock(CPlaceRebarByBlock *pPlace)
{
	if(!pPlace) return 0;

	double dLen	= 0;
	long nSize	= pPlace->m_xyArr.GetSize();
	long i = 0; for(i = 0; i < pPlace->m_xyArr.GetSize(); i++)
	{
		CDPoint xyR	= nSize > i ? pPlace->m_xyArr.GetAt(i) : CDPoint(0, 0);
		dLen += (xyR.x * xyR.y);
	}

	return dLen;
}



void CInputRebarSection::SetGridData_Bracket(BOOL bSupportRebar)
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 2, 1);
	m_Grid.SetCellSize(20, 50);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	
	// ��ü���� �ϳ��� ������� �޼��� ǥ��
	if(bSupportRebar)
	{
		m_pIn->GridMessage(&m_Grid, "����� ��±��� �ڵ����� ��ٵ˴ϴ�.");
		return;
	}

	long row		= 0, col = 0;
	CString str		= _T("");
	CString sHelp	= _T("");
	CString sUpdn	= _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	CString sWall	= _T("");
	CPlaceRebarByBlock *pPlace	= NULL;
	
	// ����
	m_Grid.AddCell(row, col, "����\n");
	m_Grid.AddCell(row+1, col, "����\n");
	m_Grid.AddCell(row+2, col, "����");
	m_Grid.AddCell(row+3, col++, "����");

	// ���ö��
	if(bSupportRebar)
	{

	
	}
	// ����ö��
	else
	{
		for(long stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			CString sStt	= bStt ? "����" : "����";
			pPlace	= &pBri->m_placeByBlock_ShearRebar_Bracket[stt];
			pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();
			for(long block = 0; block < pPlace->m_xyArr.GetSize(); block++)
			{
				str.Format("%s%d", bStt ? "VS" : "VE", block+1);
				m_Grid.AddCell(row, col, sStt);
				m_Grid.AddCell(row+1, col, str);
				m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
				m_Grid.AddCell(row+3, col++, &pPlace->m_xyArr, block, FALSE, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
			}
		}
	}
}

void CInputRebarSection::OnCellInsertedCol(NMHDR* pNotifyStruct, LRESULT* result)
{
	CCellID cell	= m_Grid.GetFocusCell();
	m_Grid.InsertColumnMng(cell.col);
	SetGridData();
	SetDataInit();
	SetCombo_Block();
}

void CInputRebarSection::OnCellDeletedCol(NMHDR* pNotifyStruct, LRESULT* result)
{
	CCellID cell	= m_Grid.GetFocusCell();
	m_Grid.InsertColumnMng(cell.col, TRUE);
	SetGridData();
	SetDataInit();
	SetCombo_Block();
}

void CInputRebarSection::SetGridData_ArchRib(BOOL bSupportRebar)
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 2, 1);
	m_Grid.SetCellSize(20, 50);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH) return;
	
	long row		= 0, col = 0;
	CString str		= _T("");
	CString sHelp	= _T("");
	CString sUpdn	= _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	CPlaceRebarByBlock *pPlace	= NULL;
	long block(0);
	// ����
	m_Grid.AddCell(row, col, "����\n");
	m_Grid.AddCell(row+1, col, "����\n");
	m_Grid.AddCell(row+2, col, "����");
	m_Grid.AddCell(row+3, col++, "����");

	// ���ö��
	if(bSupportRebar)
	{
		// �������
		pPlace	= &pBri->m_placeByBlock_SupportRebar_ArchRib[iUPPER][0][iLEFT];
		pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();
		for(block = 0; block < pPlace->m_xyArr.GetSize(); block++)
		{
			str.Format("UL%d", block+1);
			m_Grid.AddCell(row, col, "�輱����");
			m_Grid.AddCell(row+1, col, str);
			m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.AddCell(row+3, col++, &pPlace->m_xyArr, block, FALSE, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
		}

		// ������
		pPlace	= &pBri->m_placeByBlock_SupportRebar_ArchRib[iUPPER][0][iRIGHT];
		pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();
		for(block = 0; block < pPlace->m_xyArr.GetSize(); block++)
		{
			str.Format("UR%d", block+1);
			m_Grid.AddCell(row, col, "�輱����");
			m_Grid.AddCell(row+1, col, str);m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
			m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine);m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2); m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.AddCell(row+3, col++, &pPlace->m_xyArr, block, FALSE, nFormat, nLine);m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2); m_Grid.SetCellDec(CELL_DEC_0);
		}

	}
	// ����ö��
	else
	{
		// ����
		pPlace	= &pBri->m_placeByBlock_ShearRebar_ArchRib[iLEFT];
		pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();
		for(block = 0; block < pPlace->m_xyArr.GetSize(); block++)
		{
			str.Format("VL%d\n", block+1);
			m_Grid.AddCell(row, col, str);
			m_Grid.AddCell(row+1, col, str);
			m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.AddCell(row+3, col++, &pPlace->m_xyArr, block, FALSE, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
		}

		// ����
		pPlace	= &pBri->m_placeByBlock_ShearRebar_ArchRib[iRIGHT];
		pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();
		for(block = 0; block < pPlace->m_xyArr.GetSize(); block++)
		{
			str.Format("VR%d\n", block+1);
			m_Grid.AddCell(row, col, str);
			m_Grid.AddCell(row+1, col, str);
			m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine);m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2); m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.AddCell(row+3, col++, &pPlace->m_xyArr, block, FALSE, nFormat, nLine);m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2); m_Grid.SetCellDec(CELL_DEC_0);
		}
	}
}

// ��������� ����
void CInputRebarSection::SetGridData_ArchJijumSupport()
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 1, 1);
	m_Grid.SetCellSize(20, 50);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH) return;
	
	long row		= 0, col = 0;
	CString str		= _T("");
	CString sHelp	= _T("");
	CString sUpdn	= _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	m_Grid.AddCell(row, col, "����");
	m_Grid.AddCell(row+1, col, "����");
	m_Grid.AddCell(row+2, col++, "����");

	m_Grid.AddCell(row, col, "D1");	// 1�� �Ǻ�
	m_Grid.AddCell(row+1, col, &pBri->m_footingStt.m_dCoverSlope[0]);
	m_Grid.AddCell(row+2, col++, &pBri->m_footingEnd.m_dCoverSlope[0]);

	m_Grid.AddCell(row, col, "D2");	// 2�� �Ǻ�
	m_Grid.AddCell(row+1, col, &pBri->m_footingStt.m_dCoverSlope[1]);
	m_Grid.AddCell(row+2, col++, &pBri->m_footingEnd.m_dCoverSlope[1]);

	m_Grid.AddCell(row, col, "D3");	// 3�� �Ǻ�
	m_Grid.AddCell(row+1, col, &pBri->m_footingStt.m_dCoverSlope[2]);
	m_Grid.AddCell(row+2, col++, &pBri->m_footingEnd.m_dCoverSlope[2]);

	m_Grid.AddCell(row, col, "L");	// ��ö�� �ٸ� ����
	m_Grid.AddCell(row+1, col, &pBri->m_footingStt.m_dLenLeg_Support_FrontSlope);
	m_Grid.AddCell(row+2, col++, &pBri->m_footingEnd.m_dLenLeg_Support_FrontSlope);

	m_Grid.AddCell(row, col, "M");	// ��ö�� ����
	m_Grid.AddCell(row+1, col, &pBri->m_footingStt.m_dDia_Main_FrontSlope, nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(row+2, col++, &pBri->m_footingEnd.m_dDia_Main_FrontSlope, nFormat, nLine, "", CELL_TYPE_DIA);

	m_Grid.AddCell(row, col, "S");	// ��±� ����
	m_Grid.AddCell(row+1, col, &pBri->m_footingStt.m_placeByBlock_Support_FrontSlope.m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(row+2, col++, &pBri->m_footingEnd.m_placeByBlock_Support_FrontSlope.m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);

	m_Grid.AddCell(row, col, "C1");	// ��±� ���� ����
	m_Grid.AddCell(row+1, col, &pBri->m_footingStt.m_placeByBlock_Support_FrontSlope.m_xyArr, 0, FALSE); m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(row+2, col++, &pBri->m_footingEnd.m_placeByBlock_Support_FrontSlope.m_xyArr, 0, FALSE); m_Grid.SetCellDec(CELL_DEC_0);

	m_Grid.AddCell(row, col, "B");	// ��±� ����
	m_Grid.AddCell(row+1, col, &pBri->m_footingStt.m_placeByBlock_Support_FrontSlope.m_xyArr, 1, TRUE); m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(row+2, col++, &pBri->m_footingEnd.m_placeByBlock_Support_FrontSlope.m_xyArr, 1, TRUE); m_Grid.SetCellDec(CELL_DEC_0);

	m_Grid.AddCell(row, col, "C");	// ��±� ����
	m_Grid.AddCell(row+1, col, &pBri->m_footingStt.m_placeByBlock_Support_FrontSlope.m_xyArr, 1, FALSE); m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(row+2, col++, &pBri->m_footingEnd.m_placeByBlock_Support_FrontSlope.m_xyArr, 1, FALSE); m_Grid.SetCellDec(CELL_DEC_0);

	m_Grid.AddCell(row, col, "C2");	// ��±� ������ ����
	m_Grid.AddCell(row+1, col, &pBri->m_footingStt.m_placeByBlock_Support_FrontSlope.m_xyArr, 2, FALSE); m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(row+2, col++, &pBri->m_footingEnd.m_placeByBlock_Support_FrontSlope.m_xyArr, 2, FALSE); m_Grid.SetCellDec(CELL_DEC_0);

}

// ��ġ���� ���պ� ���ͷ�ö��
void CInputRebarSection::SetGridData_ArchRibConnectSectionStirrup()
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 1, 1);
	m_Grid.SetCellSize(20, 50);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH) return;
	
	long row		= 0, col = 0;
	CString str		= _T("");
	CString sHelp	= _T("");
	CString sUpdn	= _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	m_Grid.AddCell(row, col, "����");
	m_Grid.AddCell(row+1, col, "����");
	m_Grid.AddCell(row+2, col++, "����");


	m_Grid.AddCell(row, col, "����");	// ����ö�� ���δ� ����
	m_Grid.AddCell(row+1, col, &pBri->m_nCountWrapShearRebar_StirrupRebar_ConnectSection[iLEFT]);
	m_Grid.AddCell(row+2, col++, &pBri->m_nCountWrapShearRebar_StirrupRebar_ConnectSection[iRIGHT]);

	m_Grid.AddCell(row, col, "D");	// ö������
	m_Grid.AddCell(row+1, col, &pBri->m_placeByBlock_StirrupRebar_ConnectSection[iLEFT].m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);
	m_Grid.AddCell(row+2, col++, &pBri->m_placeByBlock_StirrupRebar_ConnectSection[iRIGHT].m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);

	m_Grid.AddCell(row, col, "C1");	// ���� ����
	m_Grid.AddCell(row+1, col, &pBri->m_placeByBlock_StirrupRebar_ConnectSection[iLEFT].m_xyArr, 0, FALSE);
	m_Grid.AddCell(row+2, col++, &pBri->m_placeByBlock_StirrupRebar_ConnectSection[iRIGHT].m_xyArr, 0, FALSE);

	m_Grid.AddCell(row, col, "B");	// ����
	m_Grid.AddCell(row+1, col, &pBri->m_placeByBlock_StirrupRebar_ConnectSection[iLEFT].m_xyArr, 1, TRUE);
	m_Grid.AddCell(row+2, col++, &pBri->m_placeByBlock_StirrupRebar_ConnectSection[iRIGHT].m_xyArr, 1, TRUE);

	m_Grid.AddCell(row, col, "C");	// ����
	m_Grid.AddCell(row+1, col, &pBri->m_placeByBlock_StirrupRebar_ConnectSection[iLEFT].m_xyArr, 1, FALSE);
	m_Grid.AddCell(row+2, col++, &pBri->m_placeByBlock_StirrupRebar_ConnectSection[iRIGHT].m_xyArr, 1, FALSE);

	m_Grid.AddCell(row, col, "C2");	// ������ ����
	m_Grid.AddCell(row+1, col, &pBri->m_placeByBlock_StirrupRebar_ConnectSection[iLEFT].m_xyArr, 2, FALSE);
	m_Grid.AddCell(row+2, col++, &pBri->m_placeByBlock_StirrupRebar_ConnectSection[iRIGHT].m_xyArr, 2, FALSE);
}

void CInputRebarSection::OnKillfocusEditAccu() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH) return;
	if(m_nTab != m_nTabArchRib) return;

	UpdateData(TRUE);
	pBri->m_dAccuracyArchRibLowerSupportDim	= m_dAccu;

	DrawInputDomyunView(FALSE);
}

void CInputRebarSection::SetStringCTCPart()
{
	m_sPart	= GetStringPart();
	UpdateData(FALSE);
}


CString CInputRebarSection::GetStringPart()
{
	CString sRebar	= m_nTabRebar == m_nTabRebarShear ? "����ö��" : "���ö��";
	CString sPart	= m_nTab < m_sArrTabName.GetSize() ? m_sArrTabName.GetAt(m_nTab) : "";

	return sPart + " " + sRebar + "��ġ����";
}

void CInputRebarSection::SetCTCPart()
{
	InitComboCtrlPart();
	SetStringCTCPart();
	if(m_nTabRebar == m_nTabRebarSupport)
	{
		double dCtc	= GetCTC() ? *GetCTC() : 0;
		CHgBaseConcStd baseConc;
		long nIdx	= baseConc.GetIdxByRebarCTC(dCtc);
		if(nIdx !=-1)
			m_comboCTCPart.SetCurSel(nIdx);
	}
	else
	{
		double dCTCSup		= GetCTC(TRUE) ? *GetCTC(TRUE) : 0;
		double dCTCShear	= GetCTC() ? *GetCTC() : 0;
		long nIdx	= (long)((dCTCShear / dCTCSup) - 1);
		if(m_nTab == m_nTabBracket)
		{
			for(long ix=0; ix<4; ++ix)
			{
				if(GetCtcShearBracket(ix) == dCTCShear)
				{
					nIdx = ix;
					break;
				}
			}
		}
		if(nIdx < 0) nIdx = 0;
		if(nIdx >= m_comboCTCPart.GetCount())
			nIdx = m_comboCTCPart.GetCount()-1;
		
		m_comboCTCPart.SetCurSel(nIdx);
	}
}

double* CInputRebarSection::GetCTC(BOOL bSupport)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return NULL;

	
	double *pDCTC	= NULL;
	BOOL bSupportRebar	= m_nTabRebar == m_nTabRebarSupport || bSupport;

	if(m_nTab == m_nTabUpperSlab)
	{
		pDCTC	= bSupportRebar	? &pBri->m_dCTC_SupportRebar[iUPPER] : &pBri->m_dCTC_ShearRebar[iUPPER];
	}
	else if (m_nTab == m_nTabLowerSlab)
	{
		pDCTC	= bSupportRebar	? &pBri->m_dCTC_SupportRebar[iLOWER] : &pBri->m_dCTC_ShearRebar[iLOWER];
	}
	else if(m_nTab == m_nTabOutWallStt)
	{
		pDCTC	= bSupportRebar ? &pBri->m_dCTC_SupportRebarOutWall[iSTT] : &pBri->m_dCTC_ShearRebarOutWall[iSTT];
	}
	else if(m_nTab == m_nTabOutWallEnd)
	{
		pDCTC	= bSupportRebar ? &pBri->m_dCTC_SupportRebarOutWall[iEND] : &pBri->m_dCTC_ShearRebarOutWall[iEND];
	}
	else if(m_nTab == m_nTabBracket)
	{
		if(bSupportRebar == FALSE)
			pDCTC	= &pBri->m_dCTC_ShearRebarBracket;
	}
	else
	{
		for(long ix =0; ix < MAX_JIJUM_COUNT_RC; ix++)
		{
			if(m_nTab == m_nTabInWall[ix])
			{
				CWallApp *pWall	= pBri->GetInWall(0);
				if(pWall)
				{
					pDCTC	= bSupportRebar ? &pWall->m_dCTC_SupportRebar : &pWall->m_dCTC_ShearRebar;
				}
			}

			if(m_nTab == m_nTabFooting[ix])
			{
				CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(ix);
				if(pFooting)
				{
					pDCTC	= bSupportRebar ? &pFooting->m_dCTC_SupportRebar : &pFooting->m_dCTC_ShearRebar;
				}
			}
		}
	}


	return pDCTC;
}

void CInputRebarSection::OnSelchangeComboCtcPart() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	BOOL bSupportRebar	= m_nTabRebar == m_nTabRebarSupport;
	double *pDCTC	= GetCTC();
	if(pDCTC)
	{
		if(m_nTabRebar == m_nTabRebarSupport)
		{
			CHgBaseConcStd baseConc;
			*pDCTC	= baseConc.GetValueRebarCTC(m_comboCTCPart.GetCurSel());
		}
		else
		{
			if(m_nTab == m_nTabBracket)
			{
				double dCTC	= GetCtcShearBracket(m_comboCTCPart.GetCurSel());
				*pDCTC	= dCTC;
			}
			else
			{
				double dCTC	= (*GetCTC(TRUE)) * (m_comboCTCPart.GetCurSel()+1);
				*pDCTC	= dCTC;
			}
		}

		// ������ü�� ���ÿ� ��� ����� // 31543
		for(long ix =0; ix< MAX_JIJUM_COUNT_RC ; ix++)
		{
			if(m_nTab == m_nTabInWall[ix])
			{
				long nCountInWall	= pBri->GetCountInWall();
				CWallApp *pWallOrg	= pBri->GetInWall(0);
				if(pWallOrg)
				{
					long wall = 0; for(wall = 1; wall < nCountInWall; wall++)
					{
						CWallApp *pWall	= pBri->GetInWall(wall);
						if(!pWall) continue;

						pWall->m_dCTC_ShearRebar	= pWallOrg->m_dCTC_ShearRebar;
						pWall->m_dCTC_SupportRebar	= pWallOrg->m_dCTC_SupportRebar;
					}
				}
			}
		}
	}

	long footing	= GetIdxCurFooting();
	long nType		= GetTypeDefault();
	if(bSupportRebar)
		pBri->SetDataDefaultRebarSupport_VertSection_Std(nType, footing);
	else
		pBri->SetDataDefaultRebarShear_VertSection_Std(nType, footing);

	SyncRebar();
	SetGridData();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}

long CInputRebarSection::GetTypeDefault()
{
	long nType	= -1;
	if(m_nTab == m_nTabUpperSlab) nType = 0;
	else if(m_nTab == m_nTabOutWallStt) nType = 1;
	else if(m_nTab == m_nTabOutWallEnd) nType = 2;
	else if(m_nTab == m_nTabBracket) nType = 5;
	else if(m_nTab == m_nTabArchRib) nType = 6;
	else if(m_nTab == m_nTabArchJijumSupport) nType	= 7;
	else if(m_nTab == m_nTabArchConnectStirrup) nType = 8;
	else if(m_nTab == m_nTabLowerSlab) nType = 9;
	else 
	{
		long footing = 0; for(footing = 0; footing < MAX_JIJUM_COUNT_RC; footing++)
		{
			if(m_nTab == m_nTabInWall[footing])
			{
				nType = 3; 
				break;
			}

			if(m_nTab == m_nTabFooting[footing]) 
			{
				nType = 4;
				break;
			}
		}

		if(nType != 4 && nType != 3) nType = -1; 
	}

	return nType;
}

void CInputRebarSection::InitComboCtrlPart()
{
	long nCount	= m_comboCTCPart.GetCount();
	long i = 0; for(i = 0; i < nCount; i++)
		m_comboCTCPart.DeleteString(0);

	if(m_nTabRebar == m_nTabRebarSupport)
	{
		// CTC
		CString str	= _T("");
		for(long ctc = 0; ctc < MAX_REBAR_CTC; ctc++)
		{
			CHgBaseConcStd baseConcStd;
			str.Format("CTC %.0f", baseConcStd.GetValueRebarCTC(ctc));
			m_comboCTCPart.AddString(str);
		}
	}
	else
	{
		if(GetCTC())
		{
			CString str	= _T("");
			if(m_nTab == m_nTabBracket)
			{
				for(long ix=0; ix<4; ++ix)
				{
					str.Format("CTC %.0f", GetCtcShearBracket(ix));
					m_comboCTCPart.AddString(str);
				}
			}
			else
			{
				double dCTCSup	= *GetCTC(TRUE);

				for(long dup = 0; dup < 4; dup++)
				{
					str.Format("CTC %.0f", dCTCSup*(dup+1));
					m_comboCTCPart.AddString(str);
				}				
			}
		}
	}
}

void CInputRebarSection::SetGridData_LowerSlab( BOOL bSupportRebar )
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 2, 1);
	m_Grid.SetCellSize(20, 50);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long row		= 0, col = 0;
	CString str		= _T("");
	CString sHelp	= _T("");
	CString sUpdn	= _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	CPlaceRebarByBlock *pPlace	= NULL;
	long block(0);
	// ����
	m_Grid.AddCell(row, col, "����\n");
	m_Grid.AddCell(row+1, col, "����\n");
	m_Grid.AddCell(row+2, col, "����");
	m_Grid.AddCell(row+3, col++, "����");

	// ���ö��
	if(bSupportRebar)
	{
		// ���
		pPlace	= &pBri->m_placeByBlock_SupportRebar_LowerSlab[iUPPER][0];
		pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();
		for(block = 0; block < pPlace->m_xyArr.GetSize(); block++)
		{
			str.Format("U%d", block+1);
			m_Grid.AddCell(row, col, "���");
			m_Grid.AddCell(row+1, col, str);
			m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.AddCell(row+3, col++, &pPlace->m_xyArr, block, FALSE, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
		}


		// �ϸ�
		pPlace	= &pBri->m_placeByBlock_SupportRebar_LowerSlab[iLOWER][0];
		pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();
		for(block = 0; block < pPlace->m_xyArr.GetSize(); block++)
		{
			str.Format("L%d", block+1);
			m_Grid.AddCell(row, col, "�ϸ�");
			m_Grid.AddCell(row+1, col, str); m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
			m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine); m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2); m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.AddCell(row+3, col++, &pPlace->m_xyArr, block, FALSE, nFormat, nLine); m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2); m_Grid.SetCellDec(CELL_DEC_0);
		}
	}
	// ����ö��
	else
	{
		pPlace	= &pBri->m_placeByBlock_ShearRebar_LowerSlab;
		pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();
		for(block = 0; block < pPlace->m_xyArr.GetSize(); block++)
		{
			str.Format("V%d\n", block+1);
			m_Grid.AddCell(row, col, str);
			m_Grid.AddCell(row+1, col, str);
			m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
			m_Grid.AddCell(row+3, col++, &pPlace->m_xyArr, block, FALSE, nFormat, nLine); m_Grid.SetCellDec(CELL_DEC_0);
		}
	}
}


void CInputRebarSection::OnBnClickedButtonCantilever()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	CCheckCantileverDlg dlg(pBri, m_pIn->m_nCurrentBridge);
	if(dlg.DoModal() == IDOK)
	{

	}
}

double CInputRebarSection::GetCtcShearBracket( long nIndex )
{
	double dCtc(0);

	switch(nIndex)
	{
	case 0:
		dCtc = 150;	break;
	case 1:
		dCtc = 200;	break;
	case 2:
		dCtc = 250;	break;
	case 3:
		dCtc = 300;	break;
	case 4:
		dCtc = 400;	break;
	default:
		break;
	}

	return dCtc;
}
