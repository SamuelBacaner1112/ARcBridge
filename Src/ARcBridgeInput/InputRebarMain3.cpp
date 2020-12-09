// InputRebarMain3.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "../ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "InputRebarMain3.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputRebarMain3 dialog


CInputRebarMain3::CInputRebarMain3(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputRebarMain3::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputRebarMain3)
	//}}AFX_DATA_INIT
	m_nTabHunch	= -1;
	m_nTabDanbu	= -1;
	m_nTabBracket	= -1;
	m_nTabBFooting	= -1;
	m_nTabStirrup	= -1;
	m_nTabObtuseAngle	= -1;
	m_nTabAcuteAngle	= -1;	
	m_nTab	= 0;

	m_pDlgCheck = new CInputCheckGrid;
	m_pDlgEdit  = new CInputCheckEdit;
}

CInputRebarMain3::~CInputRebarMain3()
{
	delete m_pDlgCheck;
	delete m_pDlgEdit;
}

void CInputRebarMain3::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputRebarMain3)
	DDX_Control(pDX, IDC_COMBO_OBTUSE, m_comboObtuse);
	DDX_Control(pDX, IDC_COMBO_DANBU, m_comboDanbu);
	DDX_Control(pDX, IDC_BUTTON_EXAMINE, m_btnExamine);
	DDX_Control(pDX, IDC_COMBO_CTC, m_comboCTC);
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_comboBridgeNo);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputRebarMain3, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputRebarMain3)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_BN_CLICKED(IDC_BUTTON_EXAMINE, OnButtonExamine)
	ON_CBN_SELCHANGE(IDC_COMBO_DANBU, OnSelchangeComboDanbu)
	ON_CBN_SELCHANGE(IDC_COMBO_OBTUSE, OnSelchangeComboObtuse)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW,  IDC_GRID, OnCellChangedRow)
	ON_BN_CLICKED(IDC_BUTTON_CORNER, &CInputRebarMain3::OnBnClickedButtonCorner)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputRebarMain3 message handlers
void CInputRebarMain3::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SetDataSave();
	
	return;
}

void CInputRebarMain3::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	SetFocus();
	m_nTab	= m_Tab.GetCurSel();
	SetGridData();
	SetDataInit();
	SetControl();
	if(m_nTab == m_nTabAcuteAngle || m_nTab == m_nTabObtuseAngle)
		m_Grid.SetFocusCell(1, 1);
	DrawInputDomyunView(TRUE);

	return;
}

void CInputRebarMain3::SetGridData()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	if(m_nTab == m_nTabBFooting)
	{
		m_Grid.InitGrid(pBri->GetValueFy(ePartLowerSlab));
	}
	else
	{
		m_Grid.InitGrid(pBri->GetValueFy(ePartUpperSlab));
	}
	if(m_nTab == m_nTabHunch) SetGridData_Hunch();
	else if(m_nTab == m_nTabDanbu) SetGridData_Danbu();
	else if(m_nTab == m_nTabBracket) SetGridData_Bracket();
	else if(m_nTab == m_nTabBFooting) SetGridData_BFooting();
	else if(m_nTab == m_nTabStirrup) SetGridData_Stirrup();
	else if(m_nTab == m_nTabObtuseAngle) SetGridData_ObtuseAngle();
	else if(m_nTab == m_nTabAcuteAngle) SetGridData_AcuteAngle();

	if(m_nTab == m_nTabObtuseAngle || m_nTab == m_nTabAcuteAngle)
		m_nRowFocusItemTitle	= 0;
	else
		m_nRowFocusItemTitle	= 1;
	m_Grid.SetRedraw(TRUE, TRUE);
}

void CInputRebarMain3::SetDataSave()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	m_Grid.UpdateData(TRUE);

	// 
	SyncRebar();

	// �����༱ �и� ����
	m_pIn->m_pARcBridgeDataStd->CopyToCoupleBridge_RebarMain(m_pIn->m_nCurrentBridge);

	m_pIn->m_pDoc->SetModifiedFlag();

	if(m_pDlgCheck->GetSafeHwnd() && m_pDlgCheck->IsWindowVisible())
	{
		pBri->MakeRebarInfoCycleStd(TRUE);
		m_pDlgCheck->Check();
	}
	if(m_pDlgEdit->GetSafeHwnd() && m_pDlgEdit->IsWindowVisible())
	{
		pBri->MakeRebarInfoCycleStd(TRUE);
		m_pDlgEdit->Check();
	}

	if(m_nTab == m_nTabHunch)
	{
		// ������ ��ö�ٰ� ��±� ��ǥ �ʱ�ȭ
		pBri->m_bCaledRebarMain_Plane	= FALSE;
		pBri->m_bCaledRebarSupport_Plane	= FALSE;
	}	
}

void CInputRebarMain3::SetDataDefault()
{
	for(long bri = 0; bri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(bri);
		if(!pBri) return;

		BOOL bFirst   = IsFirst(bri);
		pBri->SetDefaultedState(GetIDD(), 0x00000001);
		if(!bFirst && !IsClickDefaultButton(bri)) continue;

		if(bFirst)
		{
			pBri->SetDataDefaultRebarHunch();
			pBri->SetDataDefaultRebarDanbu();
			pBri->SetDataDefaultRebarBracket();
			pBri->SetDataDefaultRebarObtuseAngle();
		}
		else
		{
			if(m_nTab == m_nTabHunch) pBri->SetDataDefaultRebarHunch();
			else if(m_nTab == m_nTabDanbu) pBri->SetDataDefaultRebarDanbu();
			else if(m_nTab == m_nTabBracket) pBri->SetDataDefaultRebarBracket();
			else if(m_nTab == m_nTabBFooting) pBri->SetDataDefaultRebarBFooting();
			else if(m_nTab == m_nTabObtuseAngle) pBri->SetDataDefaultRebarObtuseAngle();
			else if(m_nTab == m_nTabAcuteAngle) pBri->setDataDefaultRebarAcuteAngle();
			
			// �����༱ �и� ����
			m_pIn->m_pARcBridgeDataStd->CopyToCoupleBridge_RebarMain(m_pIn->m_nCurrentBridge);
		}

		if(m_nTab == m_nTabHunch)
		{
			// ������ ��ö�ٰ� ��±� ��ǥ �ʱ�ȭ
			pBri->m_bCaledRebarMain_Plane	= FALSE;
			pBri->m_bCaledRebarSupport_Plane	= FALSE;
		}
	}

	SetGridData();
	SetDataInit();
}

void CInputRebarMain3::InitTabCtrl()
{
	m_Tab.RemoveAllTabs();
	m_Tab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_Tab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	m_nTabHunch	= -1;
	m_nTabDanbu	= -1;
	m_nTabBracket	= -1;
	m_nTabBFooting	= -1;
	m_nTabStirrup	= -1;
	m_nTabObtuseAngle	= -1;
	m_nTabAcuteAngle	= -1;
	
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	CStringArray sArrTabName;
	long nTab		= 0;
	sArrTabName.Add("��ġ�� ö��"); m_nTabHunch	= nTab++;
	sArrTabName.Add("�� ������ ö��"); m_nTabDanbu	= nTab++;
	if(pBri->IsBracket(TRUE) || pBri->IsBracket(FALSE))
	{
		sArrTabName.Add("�귡Ŷ ö��"); m_nTabBracket	= nTab++;
	}
	if(pBri->m_bExistProtectionRiseFooting)
	{
		sArrTabName.Add("�λ�������� ö��"); m_nTabBFooting = nTab++;
	}
	sArrTabName.Add("�� ������ ���ͷ� ö��"); m_nTabStirrup	= nTab++;
	sArrTabName.Add("�а��� ���� ö��"); m_nTabObtuseAngle = nTab++;
	if(pBri->IsBoxType() == FALSE)
		sArrTabName.Add("��ü ������ ���� ö��"); m_nTabAcuteAngle = nTab++;
	
	long i = 0; for(i = 0; i < sArrTabName.GetSize(); i++)
	m_Tab.InsertItem(i, sArrTabName[i]);

	if(m_Tab.GetItemCount() <= m_nTab) m_nTab = 0;
	m_Tab.SetCurSel(m_nTab);
}

void CInputRebarMain3::OnPreInitDialog()
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
	SetResize(IDC_TAB,					SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID,					SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_COMBO_DANBU,			SZ_TOP_LEFT,		SZ_TOP_LEFT);
	SetResize(IDC_COMBO_OBTUSE,			SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_CORNER,		SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);

	SetHyperHelp(m_pIn->GetManualPath());

	SetGridData();
}

BOOL CInputRebarMain3::PreTranslateMessage(MSG* pMsg) 
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

void CInputRebarMain3::InitComboCtrl()
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

	SetControl();
}

void CInputRebarMain3::OnSelchangeComboBridgeNo() 
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

void CInputRebarMain3::DrawInputDomyunView(BOOL bZoomAll)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(m_pIn->GetScaleInputDialog());

	CDomyun Dom(pDom);
	CARcBridgeDBStandard DBStd(m_pIn->GetRcBridgeRebar(), m_pIn->m_pARoadOptionStd, m_pIn->m_pARcBridgeDataStd);

	long nJ	= GetJijum();
	long nCycle = GetCycle();
	long nSlab = GetSlab();
	BOOL bDimUpperSlab	= FALSE;
	BOOL bDimOutWall	= FALSE;
	BOOL bDimInWall		= FALSE;
	BOOL bDimFooting	= FALSE;
	BOOL bDimDanbu		= m_nTab == m_nTabDanbu;
	BOOL bDimHunch		= m_nTab == m_nTabHunch;
	BOOL bDimBracket	= m_nTab == m_nTabBracket;
	BOOL bDimStirrup	= m_nTab == m_nTabStirrup;
	CDRect rectVertSection	= CDRect(0, 0, 0, 0);

	// ���� ������ �⺻������
	pBri->m_footingEnd.m_exFooting.m_Pile.InitScaleCutting();

	// ��� ��ü���� ����ȭ�� ����.
	BOOL bVertDir	= pBri->IsVertDir(); //TRUE;
	//pBri->SyncBridge(0, !bVertDir);
	pBri->GetTvVertSection_Std(0, !bVertDir, FALSE);
	pBri->SyncBridgeRebar(TRUE, bVertDir ? -1 : 1, TRUE);
	
	if(m_nTab == m_nTabAcuteAngle)
	{
		CCellID cell   = m_Grid.GetFocusCell();
		CString sTitle	= m_Grid.GetTextMatrix(cell.row, 0);
		BOOL bStt	= sTitle.Find("����", 0) != -1;

		DBStd.m_pDrawVertSection->DrawRebar_OutWall_VertStd(&Dom, bStt, FALSE, FALSE, TRUE, 0, TRUE);

		Dom.SetCalcExtRect();
		rectVertSection	= Dom.GetExtRect();
	}
	else if(m_nTab == m_nTabBFooting)
	{
		DBStd.m_pDrawVertSection->DrawBFootingSection(&Dom, TRUE);
		DBStd.m_pDrawVertSection->DrawBFootingRebar(&Dom, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE);
	}
	else
	{

		// ���ܸ� �׸�
		DBStd.m_pDrawVertSection->DrawVertSectionAll(&Dom, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE);

		Dom.SetCalcExtRect();
		rectVertSection	= Dom.GetExtRect();

		//��ġ ǥ�� ��
		long nPosSlab = -1;
		long nJ = 0;
		if(m_nTab == m_nTabObtuseAngle)
		{
			CCellID cell		= m_Grid.GetFocusCell();
			CString sTitle		= m_Grid.GetTextMatrix(cell.row, pBri->IsBoxType()? 1: 0);
			CString sUpper	= pBri->IsBoxType()? m_Grid.GetTextMatrix(cell.row, 0) : _T("");

			BOOL bStt	= sTitle.Find("����", 0) != -1;
						
			nPosSlab = (sUpper.Find(_T("�Ϻ�"))>-1 && pBri->IsBoxType())? iLOWER : iUPPER;
			nJ = bStt? 0 : pBri->GetCountJijum()-1;
		}
		else
		{
			nPosSlab= nSlab;
		}
		DBStd.m_pDrawVertSection->DrawSignHunchByJijum(&Dom, nJ, TRUE, nPosSlab == iUPPER);

		// ö�� �׸���(ġ��, ���� ����)
		long nCountWall	= pBri->GetCountInWall();

		long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_UpperSlab(&Dom, nCycle, dan, TRUE, FALSE, bDimUpperSlab);
			if(pBri->IsBoxType())
				DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_LowerSlab(&Dom, nCycle, dan, TRUE, FALSE, bDimUpperSlab);
			DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_OutWall(&Dom, nCycle, dan, TRUE, FALSE, bDimOutWall);
			DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_OutWall_Hunch(&Dom, nCycle, TRUE);
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
				DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_ArchRib(&Dom, nCycle, dan, TRUE, FALSE, FALSE, 0);

			// ������ ����
			for(long updn = 0; updn < 2; updn++)
			{
				DBStd.m_pDrawFooting->DrawRebarMain_Front(&Dom, &pBri->m_footingStt, nCycle, dan, updn, FALSE, bDimFooting);
				DBStd.m_pDrawFooting->DrawRebarMain_Front(&Dom, &pBri->m_footingEnd, nCycle, dan, updn, FALSE, bDimFooting);
			}

			// ��ü�� ö�� �׸���.(��ü�� ���� ����)
			long wall = 0; for(wall = 0; wall < nCountWall; wall++)
			{
				CWallApp *pWall	= pBri->GetInWall(wall);
				if(pWall)
				{
					for(long left = 0; left < 2; left++)
					{
						DBStd.m_pDrawWall->DrawRebarMain_Front(&Dom, pWall, nCycle, dan, left, bDimInWall, bDimInWall);
					}
					for(long updn = 0; updn < 2; updn++)
					{
						DBStd.m_pDrawFooting->DrawRebarMain_Front(&Dom, &pWall->m_footing, nCycle, dan, updn, bDimFooting && dan==0, bDimFooting);
					}
				}
			}

			// �Ŵ� ö��
			long nJ = 0; for(nJ = 0; nJ < pBri->GetCountJijum(); nJ++)
			{
				DBStd.m_pDrawVertSection->DrawRebarAll_SlabGirder(&Dom, nJ, FALSE, FALSE, 0, FALSE, TRUE, TRUE, TRUE, TRUE, FALSE, -1, TRUE);
			}
		}
		//��ġ
		DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_UpperSlabHunch(&Dom, nCycle, bDimHunch);
		DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_LowerSlabHunch(&Dom, nCycle, bDimHunch);

		//��������
		DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_Danbu(&Dom, FALSE, bDimDanbu, TRUE, nCycle);

		//�귡Ŷ, �������� ���ͷ�
		for(long stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_Bracket(&Dom, bStt, FALSE, bDimBracket, TRUE, 0, CDRect(0, 0, 0, 0), nCycle);
			DBStd.m_pDrawVertSection->DrawRebarShear_VertSection_UpperSlabDanbu(&Dom, nCycle, bStt, bDimStirrup, FALSE, TRUE);
			DBStd.m_pDrawVertSection->DrawRebarShear_VertSection_LowerSlabDanbu(&Dom, nCycle, bStt, bDimStirrup, FALSE, TRUE);
		}
	}
	
	// �а��� ������ �� �а��� ���� ö�� �׷���
	if(m_nTab == m_nTabObtuseAngle)
	{
		BOOL bBox	= pBri->IsBoxType();

		CCellID cell		= m_Grid.GetFocusCell();
		CString sTitle		= m_Grid.GetTextMatrix(cell.row, bBox? 1: 0);
		CString sUpper	= bBox? m_Grid.GetTextMatrix(cell.row, 0) : _T("");
		
		BOOL bStt	= sTitle.Find("����", 0) != -1;
		BOOL bLeft	= sTitle.Find("����", 0) != -1;

		BOOL bUpperSlab = (bBox && sUpper.Find(_T("�Ϻν�����"))>-1)? FALSE : TRUE;

		DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_SlabObtuse(&Dom, bUpperSlab, bStt, bLeft, TRUE, TRUE, TRUE);
	}

	if(m_nTab == m_nTabAcuteAngle)
	{
		CCellID cell		= m_Grid.GetFocusCell();
		CString sTitle	= m_Grid.GetTextMatrix(cell.row, 0);
		
		BOOL bStt	= sTitle.Find("����", 0) != -1;
		BOOL bLeft	= sTitle.Find("����", 0) != -1;

		CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;
		BOOL bFront	= bStt ? bLeft : !bLeft;

		DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_OutWallAcute(&Dom, bStt, bLeft, TRUE, FALSE, TRUE);
		DBStd.m_pDrawFooting->DrawRebarMain_Front_Obtuse(&Dom, pFooting, bFront, TRUE, FALSE, TRUE);

		DBStd.m_pDrawVertSection->DrawRebarSupport_VertSection_OutWallAcuteAngle(&Dom, bStt, bLeft, TRUE, TRUE, TRUE, 0);
		DBStd.m_pDrawFooting->DrawRebarSupport_FrontObtuse(&Dom, pFooting, bFront, TRUE, TRUE, 0, TRUE);
	}

	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();
	m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_MAIN_TITLE);

	CString sCycle	= _T("");
	sCycle.Format("%d CYCLE", nCycle+1);
	//Dom.TextOut(CDPoint(rect.left+rect.Width()/2, rect.bottom+Dom.GetTextHeight()*2), sCycle);

	// ������ �� ��ġ ǥ��(�ﰢ��)
	for(nJ = 0; nJ < pBri->GetCountJijum(); nJ++)
		DBStd.m_pDrawVertSection->DrawJijumSign(&Dom, nJ);

	Dom.SetCalcExtRect();
	rect	= Dom.GetExtRect();

	*pDom << Dom;

	// �а��� �����Է����϶� �а��� ǥ��
	if(m_nTab == m_nTabObtuseAngle)
	{
		CCellID cell			= m_Grid.GetFocusCell();
		cell.row				= cell.row == -1 ? 1 : cell.row;
		cell.col					= cell.col == -1 ? 1 : cell.col;

		m_Grid.SetFocusCell(cell.row, cell.col);

		CString sTitle			= m_Grid.GetTextMatrix(cell.row, pBri->IsBoxType()? 1: 0);
		CString sUpper		= pBri->IsBoxType()? m_Grid.GetTextMatrix(cell.row, 0) : _T("");
		BOOL bUpperSlab	= (pBri->IsBoxType() && sUpper.Find(_T("�Ϻν�����"))>-1)? FALSE : TRUE;
		
		BOOL bStt	= sTitle.Find("����", 0) != -1;
		BOOL bLeft	= sTitle.Find("����", 0) != -1;
		
		BOOL bDrawPlanObtuse = FALSE;
		if(bUpperSlab)
			bDrawPlanObtuse = pBri->m_bIsObtuseAngle_UpperSlab[bStt ? iSTT : iEND][bLeft ? iLEFT : iRIGHT];
		else
			bDrawPlanObtuse = pBri->m_bIsObtuseAngle_LowerSlab[bStt ? iSTT : iEND][bLeft ? iLEFT : iRIGHT];

		if(bDrawPlanObtuse)
		{
			// ���
			DBStd.m_pDrawPlane->DrawPlaneGenObtuse(&Dom, bStt, bLeft, TRUE, !bUpperSlab);
			DBStd.m_pDrawPlane->DrawRebarPlane_Slab_Obtuse(&Dom, bUpperSlab, bStt, bLeft, TRUE, TRUE, TRUE, TRUE);
			
			Dom.SetCalcExtRect();
			CDRect rectPlane	= Dom.GetExtRect();

			sTitle	= m_Grid.GetTextMatrix(cell.row, 0) + "(���)";
			Dom.TextOut(CDPoint(rectPlane.left+rectPlane.Width()/2, rectPlane.bottom+Dom.GetTextHeight()*2), sTitle);
			
			CDPoint xyMove	= CDPoint(0, 0);
			xyMove.x	= (rect.left+rect.Width()/2) - rectPlane.right - Dom.Always(30);
			xyMove.y	= rect.bottom - rectPlane.top + Dom.Always(30);

			Dom.Move(xyMove);
			*pDom << Dom;

			// �ϸ�
			DBStd.m_pDrawPlane->DrawPlaneGenObtuse(&Dom, bStt, bLeft, FALSE, !bUpperSlab);
			DBStd.m_pDrawPlane->DrawRebarPlane_Slab_Obtuse(&Dom, bUpperSlab, bStt, bLeft, FALSE, TRUE, TRUE, TRUE);
			
			Dom.SetCalcExtRect();
			rectPlane	= Dom.GetExtRect();

			sTitle	= m_Grid.GetTextMatrix(cell.row, 0) + "(�ϸ�)";
			Dom.TextOut(CDPoint(rectPlane.left+rectPlane.Width()/2, rectPlane.bottom+Dom.GetTextHeight()*2), sTitle);
			
			xyMove.x	= (rect.left+rect.Width()/2) - rectPlane.left + Dom.Always(30);
			xyMove.y	= rect.bottom - rectPlane.top + Dom.Always(30);

			Dom.Move(xyMove);
			*pDom << Dom;
		}

	}
	// ���� ��� ǥ��
	else if(m_nTab == m_nTabAcuteAngle)
	{
		CCellID cell	= m_Grid.GetFocusCell();
		CString sTitle	= m_Grid.GetTextMatrix(cell.row, 0);
		
		BOOL bStt	= sTitle.Find("����", 0) != -1;
		BOOL bLeft	= sTitle.Find("����", 0) != -1;
		
		// ��������� ���ܸ� ���� �Ʒ��ʿ�
		*pDom << Dom;

		CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;
		BOOL bFront	= bStt ? bLeft : !bLeft;

		long nDrawUpper	= 0;
		BOOL bDimForTopIsRight	= FALSE;//bStt ? FALSE : TRUE;
		long nRotate	= bStt ? 1 : -1;
		CDRect rectPlane	= DBStd.m_pDrawFooting->DrawRebar_Plane_ObtuseStd(&Dom, pFooting, bFront, TRUE, TRUE, FALSE, nDrawUpper, bDimForTopIsRight, nRotate);
		Dom.TextOut(CDPoint(rectPlane.left+rectPlane.Width()/2, rectPlane.bottom+Dom.GetTextHeight()*2), sTitle+"(�������)");
		CDPoint xyMove	= CDPoint(0, 0);
		if(bStt)
		{
			xyMove.x	= rectVertSection.left - rectPlane.left;
			xyMove.y	= rectVertSection.top - rectPlane.bottom - Dom.Always(60);
		}
		else
		{
			xyMove.x	= rectVertSection.right - rectPlane.right;
			xyMove.y	= rectVertSection.top - rectPlane.bottom - Dom.Always(60);
		}
		Dom.Move(xyMove);
		*pDom << Dom;

		// ��ü�� ������ �¿�����
		CDRect rectFront	= DBStd.m_pDrawCrossSection->DrawRebarCrossSection_AcuteStd(&Dom, bStt, bLeft, TRUE, TRUE, TRUE);
		Dom.TextOut(CDPoint(rectFront.left+rectFront.Width()/2, rectFront.bottom+Dom.GetTextHeight()*2), sTitle+"(��ü����)");
		if(bStt)
		{
			xyMove.x	= rectVertSection.left - rectFront.right - Dom.Always(40);
			xyMove.y	= rectVertSection.top - rectFront.top;
		}
		else
		{
			xyMove.x	= rectVertSection.right - rectFront.left + Dom.Always(40);
			xyMove.y	= rectVertSection.top - rectFront.top;
		}
		Dom.Move(xyMove);
				
		*pDom << Dom;
	}
	
	if(m_nTab == m_nTabAcuteAngle)
	{
		CCellID cell	= m_Grid.GetFocusCell();
		CString sTitle	= m_Grid.GetTextMatrix(cell.row, 0);
		
		BOOL bStt	= sTitle.Find("����", 0) != -1;
		BOOL bLeft	= sTitle.Find("����", 0) != -1;
		
		DBStd.m_pDrawPlane->DrawPlanePosSign(&Dom, bStt, bLeft);
		Dom.SetCalcExtRect();
		CDRect rectPos	= Dom.GetExtRect();

		if(bStt)
		{
			Dom.Move(CDPoint(rectVertSection.right - rectPos.left + Dom.Always(50), rectVertSection.top - rectPos.top));
		}
		else
		{
			Dom.Move(CDPoint(rectVertSection.left - rectPos.right - Dom.Always(50), rectVertSection.top - rectPos.top));
		}

		*pDom << Dom;
	}
	pBri->GetTvVertSection_Std(0, !bVertDir, FALSE, TRUE);
	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

void CInputRebarMain3::SetDataInit()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	m_Grid.UpdateData(FALSE);

	long nCols	= m_Grid.GetCols();
	long nRows	= m_Grid.GetRows();

	// �ܺ�, ����� �� ��� merge
	if(m_nTab == m_nTabDanbu)
	{
		m_Grid.MergeGrid(0, 0, 1, nCols-1);
		m_Grid.SetMergeRow(0, 3, 0);
		m_Grid.SetMergeCol(1, 1, 2);
		m_Grid.SetMergeCol(1, 3, 4);
		if(pBri->IsBoxType())
		{
			m_Grid.SetMergeCol(1, 5, 6);
			m_Grid.SetMergeCol(1, 7, 8);
		}
	}
	else if(m_nTab == m_nTabBracket)
	{
		m_Grid.SetMergeRow(0, 2, 0);
		m_Grid.SetMergeCol(0, 1, 2);
		m_Grid.SetMergeCol(0, 3, 4);
	}
	else if(m_nTab == m_nTabBFooting)
	{
		m_Grid.MergeGrid(0, 0, 0, nCols-1);
	}
	else if(m_nTab == m_nTabStirrup)
	{
		m_Grid.SetMergeCol(0, 0, 5);
		m_Grid.SetMergeCol(1, 0, 2);
		m_Grid.SetMergeCol(1, 3, 5);
		if(pBri->IsBoxType())
		{
			m_Grid.SetMergeCol(0, 6, 11);
			m_Grid.SetMergeCol(1, 6, 8);
			m_Grid.SetMergeCol(1, 9, 11);
		}
	}
	else if(m_nTab == m_nTabObtuseAngle)
	{
		m_Grid.MergeGrid(0, 0, 0, nCols-1);
		if(pBri->IsBoxType())
			m_Grid.MergeGridEx(0, nRows-1, 0, 0, FALSE, TRUE, DT_CENTER | DT_VCENTER | DT_SINGLELINE);			
		m_Grid.MergeGrid(0, 1, 0, 1);
	}
	else if(m_nTab == m_nTabHunch)
	{
		m_Grid.SetMergeRow(0, 1, 0);
		m_Grid.MergeGrid(0, 1, 1, nCols-1);
	}
}


void CInputRebarMain3::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	m_CellID	= m_Grid.GetFocusCell();
	structGridCell *pCell = m_Grid.GetCurrentCell();
	if(pCell)
	{
		m_pIn->SetComment(pCell->sTitle);
		
		m_pView->GetDomyun()->STMakeCursor(m_Grid.GetItemText(m_nRowFocusItemTitle,pCell->nCol));
	}
	DrawInputDomyunView(FALSE);
}

void CInputRebarMain3::SetControl()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	CHgBaseConcStd baseConc;

	m_comboCTC.SetCurSel(baseConc.GetIdxByRebarCTC(pBri->m_dCTC_MainRebar));
	m_comboObtuse.SetCurSel(0);
	m_comboDanbu.SetCurSel(pBri->m_nType_DanbuRebar);
	

	m_comboCTC.EnableWindow(FALSE);
	m_comboDanbu.EnableWindow(m_nTab == m_nTabDanbu);
	m_comboObtuse.ShowWindow(m_nTab == m_nTabObtuseAngle);
	m_comboObtuse.EnableWindow(m_nTab == m_nTabObtuseAngle);
	m_btnExamine.EnableWindow(m_bDesignLsd == FALSE);
}

void CInputRebarMain3::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	CCellID cell		= m_Grid.GetFocusCell();

	SetDataSave();
	if(m_nTab != m_nTabObtuseAngle && m_nTab != m_nTabAcuteAngle)
	{
		SetGridData();
		SetDataInit();
	}
	//DrawInputDomyunView(FALSE);

	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);
}

void CInputRebarMain3::OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result)
{
}

void CInputRebarMain3::SetGridData_Hunch()
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 2, 1);
	m_Grid.SetCellSize(20, -1);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long row = 0, col = 0;
	CString str = _T(""), sHelp = _T("");
	CStringArray sArr;
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	
	long nCountCycle	= pBri->GetCountCycleMainRebar();	// ���õ� ����Ŭ ��
	long nCountDia		= pBri->m_nQtyJigan+1;
	long nUpperColCount = 0;
	long nCountSlab = 1; //������ ��ν����길.
	if(pBri->IsBoxType()) nCountSlab = 2; //Box���� �Ϻν��������.
	
	m_Grid.AddCell(row, col, _T("����"));	
	m_Grid.AddCell(row+1, col++, _T("����"));	
	
	for(long ix = iUPPER ; ix < nCountSlab; ix++)
	{
		CString strSlab = (ix == iUPPER) ? _T("��ν�����") : _T("�Ϻν�����");

		long cycle = 0; for(cycle = 0; cycle < nCountCycle; cycle++)
		{
			m_Grid.AddCell(row, col, strSlab);
			str.Format("%dCYCLE", cycle+1);
			m_Grid.AddCell(row+1, col++, str);
		}

		m_Grid.AddCell(row, col, strSlab);
		m_Grid.AddCell(row+1, col++, "ö��Ÿ��");

		row	= 1;
		long nIdxWall		= 1;
		CDoubleArray *pdArr = NULL;
		long i = 0; for(i = 0; i < nCountDia; i++)
		{
			row++;
			col = nUpperColCount;
			long nJ	= i;

			if(i == 0)
			{
				str = "������ü";
			}
			else if(i == nCountDia-1) 
			{
				str = "������ü";
			}
			else 
			{
				str.Format("�߰�����%d", nIdxWall++);
			}

			if(ix == iUPPER)
			{
				m_Grid.AddCell(row, col++, str);
			}
			
			for(cycle = 0; cycle < nCountCycle; cycle++)
			{
				if(ix == iUPPER)
					pdArr	= &pBri->m_dArrDiaUpperSlabHunch[cycle];
				else
					pdArr	= &pBri->m_dArrDiaLowerSlabHunch[cycle];

				m_Grid.AddCell(row, col++, pdArr, i, nFormat, nLine, sHelp, CELL_TYPE_DIA);
				m_Grid.SetLastCellFy(pBri->GetValueFy(ix == iUPPER? ePartUpperSlab : ePartLowerSlab));
			}

			// ��ġ ö�� Ÿ��
			// ��ս��̸� ������ 1�� ����, �����̸� 0 �Ǵ� 1
			// ��ġ�� R�� �ִٸ� ������ 0���� ����
			CWallApp *pWall	= pBri->GetInWall(nIdxWall-2);
			if(i > 0 && i < nCountDia-1 && pWall)
			{
				if(pWall->m_bTopIsColumn)
				{
					m_Grid.AddCell(row, col++, "V��", nFormat, CELL_READONLY);
				}
				else
				{
					BOOL bRound	= pBri->IsRoundHunch(nJ, TRUE, ix == iUPPER);
					BOOL bXType = !pBri->IsHunch(nJ, ix == iUPPER);

					sArr.RemoveAll();
					sArr.Add("X��");
					sArr.Add("V��");

					if(bRound)
					{
						long nSize	= pBri->m_dArrTypeUpperSlabHunch.GetSize();
						if(nSize > i)
							pBri->m_dArrTypeUpperSlabHunch.SetAt(i, 0);
					}
					//��ġ ���µ� �������ִ°�� X���� ����
					if(bXType)
					{
						if(ix == iUPPER)
						{
							long nSize	= pBri->m_dArrTypeUpperSlabHunch.GetSize();
							if(nSize > i)
								pBri->m_dArrTypeUpperSlabHunch.SetAt(i, 0);
						}
						else
						{
							long nSize	= pBri->m_dArrTypeLowerSlabHunch.GetSize();
							if(nSize > i)
								pBri->m_dArrTypeLowerSlabHunch.SetAt(i, 0);
						}
					}

					if(ix == iUPPER)
						m_Grid.AddCell(row, col++, &pBri->m_dArrTypeUpperSlabHunch, i, nFormat, bRound || bXType? CELL_READONLY : nLine, sHelp, CELL_TYPE_COMBO, sArr);
					else
						m_Grid.AddCell(row, col++, &pBri->m_dArrTypeLowerSlabHunch, i, nFormat, bRound || bXType? CELL_READONLY : nLine, sHelp, CELL_TYPE_COMBO, sArr);
				}
			}
			else
			{
				m_Grid.AddCell(row, col++, "", nFormat, CELL_READONLY);
			}
		}
		nUpperColCount = col;
		row = 0;
	}
	
	
	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(100);
	m_Grid.SetColumnWidth(0, 150);
}

void CInputRebarMain3::SetGridData_Danbu()
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 3, 1);
	m_Grid.SetCellSize(20, -1);

	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long row = 0, col = 0;
	CString str = _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	col	= 0;
	
	long nCountSlab = 1;
	if(pBri->IsBoxType()) nCountSlab = 2;

	for(long nSlab = iUPPER; nSlab < nCountSlab; nSlab++)
	{
		double dFy(0);
		if(nSlab == iUPPER)
		{
			m_Grid.AddCell(row, col+1, "��ν�����");
			m_Grid.AddCell(row, col+2, "��ν�����");
			m_Grid.AddCell(row, col+3, "��ν�����");
			m_Grid.AddCell(row++, col+4, "��ν�����");
		}
		else
		{
			m_Grid.AddCell(row, col, "�Ϻν�����");
			m_Grid.AddCell(row, col+1, "�Ϻν�����");
			m_Grid.AddCell(row, col+2, "�Ϻν�����");
			m_Grid.AddCell(row++, col+3, "�Ϻν�����");
			dFy = pBri->GetValueFy(ePartLowerSlab);
		}

		if(nSlab == iUPPER)
		{
			m_Grid.AddCell(row, col, "����");
			m_Grid.AddCell(row+1, col, "����");
			m_Grid.AddCell(row+2, col, "����");
			m_Grid.AddCell(row+3, col++, "����");
		}

		m_Grid.AddCell(row, col, "������", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+1, col, "CYCLE 1", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+2, col++, &pBri->m_bIsDanbuRebar_Cycle1[nSlab][iSTT], nFormat, nLine, " ", CELL_TYPE_CHECK);

		m_Grid.AddCell(row, col, "������", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+1, col, "CYCLE 2", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+2, col++, &pBri->m_bIsDanbuRebar_Cycle2[nSlab][iSTT], nFormat, nLine, " ", CELL_TYPE_CHECK);


		m_Grid.AddCell(row, col, "������", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+1, col, "CYCLE 1", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+2, col++, &pBri->m_bIsDanbuRebar_Cycle1[nSlab][iEND], nFormat, nLine, "  ", CELL_TYPE_CHECK);

		m_Grid.AddCell(row, col, "������", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+1, col, "CYCLE 2", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+2, col++, &pBri->m_bIsDanbuRebar_Cycle2[nSlab][iEND], nFormat, nLine, "  ", CELL_TYPE_CHECK);


		row+=3;

		CHgBaseConcStd baseConc;
		BOOL bSttCycle1	= pBri->m_bIsDanbuRebar_Cycle1[nSlab][iSTT];
		BOOL bEndCycle1	= pBri->m_bIsDanbuRebar_Cycle1[nSlab][iEND];
		BOOL bSttCycle2	= pBri->m_bIsDanbuRebar_Cycle2[nSlab][iSTT];
		BOOL bEndCycle2	= pBri->m_bIsDanbuRebar_Cycle2[nSlab][iEND];

		long dan = 0; for(dan = 0; dan < 3; dan++)
		{
			if(nSlab == iUPPER) col	= 0;
			else col = 5;

			str.Format("%d��", dan+1);
			if(nSlab == iUPPER)
				m_Grid.AddCell(row, col++, str);
			m_Grid.AddCell(row, col++, &pBri->m_dDia_DanbuRebar_Cycle1[nSlab][iSTT][dan], nFormat, bSttCycle1 ? nLine : CELL_READONLY, "", CELL_TYPE_DIA);
			if(nSlab == iLOWER)	m_Grid.SetLastCellFy(dFy);
			m_Grid.AddCell(row, col++, &pBri->m_dDia_DanbuRebar_Cycle2[nSlab][iSTT][dan], nFormat, bSttCycle2 ? nLine : CELL_READONLY, "", CELL_TYPE_DIA);
			if(nSlab == iLOWER)	m_Grid.SetLastCellFy(dFy);
			m_Grid.AddCell(row, col++, &pBri->m_dDia_DanbuRebar_Cycle1[nSlab][iEND][dan], nFormat, bEndCycle1 ? nLine : CELL_READONLY, "", CELL_TYPE_DIA);
			if(nSlab == iLOWER)	m_Grid.SetLastCellFy(dFy);
			m_Grid.AddCell(row, col++, &pBri->m_dDia_DanbuRebar_Cycle2[nSlab][iEND][dan], nFormat, bEndCycle2 ? nLine : CELL_READONLY, "", CELL_TYPE_DIA);
			if(nSlab == iLOWER)	m_Grid.SetLastCellFy(dFy);
			row++;
		}
		
		row = 0;
	}
	
	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidth(0, 60);
	m_Grid.SetColumnWidth(1, 100);
	m_Grid.SetColumnWidth(2, 100);
	
	if(pBri->IsBoxType())
	{
		m_Grid.SetColumnWidth(5, 100);
		m_Grid.SetColumnWidth(6, 100);
		m_Grid.SetColumnWidth(7, 100);
		m_Grid.SetColumnWidth(8, 100);
	}
}

// �ϳ� �Է��Ҷ� ���ÿ� �ٸ� �ſ��� ����Ǿ�� �ϴ� ö�ٵ鿡 ���ؼ� ����ȭ����
void CInputRebarMain3::SyncRebar()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	pBri->SyncCycleMainRebar(1, 3);


	for (long nStt =0; nStt < 2; nStt++)
	{
		BOOL bStt = nStt==iSTT;

		CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;
		if(pFooting == NULL) continue;

		for (long nLeft =0; nLeft< 2; nLeft++)
		{
			BOOL bLeft = nLeft==iLEFT;

			pFooting->m_bIsObtuse[nLeft] = pBri->m_bIsAcuteAngle_OutWall[nStt][nLeft];
		}	

	}	

	//SetGridData();	// ��Ÿ� �ϸ� ��Ŀ���� �����.
	SetDataInit();
}

// ���� ���� ���õǾ� �ִ� ���� ����
long CInputRebarMain3::GetJijum()
{
	long nJ	= 0;
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return 0;

	if(m_nTab == m_nTabHunch)
	{
		nJ	= m_CellID.row-2;
	}
	else if(m_nTab == m_nTabDanbu || m_nTab == m_nTabBracket)
	{
		nJ	= m_CellID.col < 3 ? 0 : pBri->m_nQtyJigan;
		if(pBri->IsBoxType())
		{
			if(m_CellID.col == 1 || m_CellID.col == 2
				 || m_CellID.col == 5 || m_CellID.col == 6)
				nJ = 0;
			else
				nJ = pBri->m_nQtyJigan;
		}
	}
	else if(m_nTab == m_nTabStirrup)
	{
		nJ	= m_CellID.col < 3 ? 0 : pBri->m_nQtyJigan;
		if(pBri->IsBoxType())
		{
			if(m_CellID.col == 0 || m_CellID.col == 1 || m_CellID.col == 2
				|| m_CellID.col == 6 || m_CellID.col == 7 || m_CellID.col == 8)
				nJ = 0;
			else
				nJ = pBri->m_nQtyJigan;
		}
	}
	else if(m_nTab == m_nTabObtuseAngle || m_nTab == m_nTabAcuteAngle)
	{
		nJ	= m_CellID.row < 3 ? 0 : pBri->m_nQtyJigan;
	}

	return nJ;
}

// ���� ���� ���õǾ� �ִ� ���� ����Ŭ
long CInputRebarMain3::GetCycle()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();

	CCellID cell		= m_Grid.GetFocusCell();
	CString strTitle	= m_Grid.GetTextMatrix(1, cell.col);

	long nCycle	= 0;

	// ��ġ���ΰ�� �ǿ� ���� �ٸ�
	// �ܺ� ���� ��� ������ 1cycle
	long nCol	= m_CellID.col;
	if(m_nTab == m_nTabHunch)
	{
		if(pBri->IsBoxType()) //BOX�� �Ϻν�������� �߰��Ǽ�..
		{
			if(strTitle == "1CYCLE") nCycle = 0;
			if(strTitle == "2CYCLE") nCycle = 1;
			if(strTitle == "3CYCLE") nCycle = 2;
			if(strTitle == "4CYCLE") nCycle = 3;
		}
		else
		{
			if(nCol < m_Grid.GetCols()-1)
				nCycle	= nCol-1;
		}
	}
	else if(m_nTab == m_nTabStirrup)
	{
		nCycle	= 0;
	}
	else if(m_nTab == m_nTabObtuseAngle || m_nTab == m_nTabAcuteAngle)
	{
		nCycle	= 0;
	}
	else 
	{
		if(nCol == 1 || nCol == 3)
			nCycle	= 0;
		else if(nCol == 2 || nCol == 4)
			nCycle	= 1;

		if(pBri->IsBoxType())
		{
			if(nCol == 5 || nCol == 7)
				nCycle = 0;
			else if(nCol == 6 || nCol == 8)
				nCycle = 1;
		}
	}

	return nCycle;
}

long CInputRebarMain3::GetSlab()
{
	long nCol	= m_CellID.col;
	long bSlab = iUPPER;

	if(m_nTab == m_nTabHunch)
	{
		if(nCol >= 1 && nCol <= 3)
			bSlab = iUPPER;
		else if(nCol >=4 && nCol <= 6)
			bSlab = iLOWER;
	}
	else if(m_nTab == m_nTabDanbu)
	{
		if(nCol >= 1 && nCol <= 4)
			bSlab = iUPPER;
		else if(nCol >= 5 && nCol <= 8)
			bSlab = iLOWER;
	}
	else if(m_nTab == m_nTabStirrup)
	{
		if(nCol >= 0 && nCol <=5)
			bSlab = iUPPER;
		else if(nCol >= 6 && nCol <=11)
			bSlab = iLOWER;
	}
	return bSlab;
}

void CInputRebarMain3::OnButtonExamine() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	BOOL bIsBraket = (pBri->IsBracket(TRUE) || pBri->IsBracket(FALSE));

	if(pBri->IsOutsideWall()==FALSE)
	{
		AfxMessageBox("������ü�� �����Ƿ� ������� �׸��� �����ϴ�.");
		return;
	}

	if(m_nTab==m_nTabDanbu) //��������ö��
	{
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
		m_pDlgCheck->SetBriAndContent(pBri,IDD_REBAR_MAIN3,0);

		m_pDlgCheck->ShowAndCheck(this);	
		CString str = _T("");
		str.Format("��Ÿ ö�� ����");
		m_pDlgCheck->SetWindowText(str);
	}
	else if(m_nTab == m_nTabBracket) //�귡Ŷ ö��
	{
		if(m_pDlgEdit->GetSafeHwnd() && m_pDlgEdit->IsWindowVisible()) m_pDlgEdit->DestroyWindow();

		long nTypeCheck = 0;
		m_pDlgEdit->SetBriAndContent(pBri,IDD_REBAR_MAIN3,nTypeCheck);

		m_pDlgEdit->ShowAndCheck(this);	
		CString str = _T("");
		str.Format("�귡Ŷ ����");
		m_pDlgEdit->SetWindowText(str);
	}
	else if(m_nTab == m_nTabStirrup) //�� ������ ���ͷ� ö��
	{
		BOOL bFemNormal = pBri->m_FemNormal.IsOutputLoaded();
		BOOL bFemEQ     = pBri->IsRcSlab() ? TRUE : (pBri->GetFemManage(COMBO_EARTHQUAKE)->IsOutputLoaded());
		
		if(!bFemNormal || (pBri->GetTypeDesingEQ() == 1 && !bFemEQ))
		{
			AfxMessageBox("�𵨸� �ؼ��� �����Ŀ� ������並 �Ͻñ� �ٶ��ϴ�.");
			return;
		}
		
		if(m_pDlgCheck->GetSafeHwnd() && m_pDlgCheck->IsWindowVisible()) m_pDlgCheck->DestroyWindow();
		m_pDlgCheck->SetBriAndContent(pBri,IDD_REBAR_MAIN3,1);
		
		m_pDlgCheck->ShowAndCheck(this);	
		CString str = _T("");
		str.Format("��Ÿ ö�� ����");
		m_pDlgCheck->SetWindowText(str);
	}
	else 
	{
		AfxMessageBox("������� �׸��� �����ϴ�.");
	}
}

void CInputRebarMain3::SetGridData_Bracket()
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 2, 1);
	m_Grid.SetCellSize(20, -1);

	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long row = 0, col = 0;
	CString str = _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	col	= 0;
	m_Grid.AddCell(row, col, "����");
	m_Grid.AddCell(row+1, col, "����");
	m_Grid.AddCell(row+2, col++, "����");
	
	
	m_Grid.AddCell(row, col, "����", nFormat, CELL_READONLY);
	m_Grid.AddCell(row+1, col, "CYCLE 1", nFormat, CELL_READONLY);
	m_Grid.AddCell(row+2, col++, &pBri->m_bIsBracketRebar[0][iSTT], nFormat, nLine, " ", CELL_TYPE_CHECK);

	m_Grid.AddCell(row, col, "����", nFormat, CELL_READONLY);
	m_Grid.AddCell(row+1, col, "CYCLE 2", nFormat, CELL_READONLY);
	m_Grid.AddCell(row+2, col++, &pBri->m_bIsBracketRebar[1][iSTT], nFormat, nLine, " ", CELL_TYPE_CHECK);
	
	
	m_Grid.AddCell(row, col, "����", nFormat, CELL_READONLY);
	m_Grid.AddCell(row+1, col, "CYCLE 1", nFormat, CELL_READONLY);
	m_Grid.AddCell(row+2, col++, &pBri->m_bIsBracketRebar[0][iEND], nFormat, nLine, "  ", CELL_TYPE_CHECK);

	m_Grid.AddCell(row, col, "����", nFormat, CELL_READONLY);
	m_Grid.AddCell(row+1, col, "CYCLE 2", nFormat, CELL_READONLY);
	m_Grid.AddCell(row+2, col, &pBri->m_bIsBracketRebar[1][iEND], nFormat, nLine, "  ", CELL_TYPE_CHECK);

	
	row+=3;

	CHgBaseConcStd baseConc;
	BOOL bSttCycle1	= pBri->m_bIsBracketRebar[0][iSTT];
	BOOL bEndCycle1	= pBri->m_bIsBracketRebar[0][iEND];
	BOOL bSttCycle2	= pBri->m_bIsBracketRebar[1][iSTT];
	BOOL bEndCycle2	= pBri->m_bIsBracketRebar[1][iEND];

	long dan	= 0;
	col	= 0;
	str.Format("%d��", dan+1);
	m_Grid.AddCell(row, col++, str);
	m_Grid.AddCell(row, col++, &pBri->m_dDiaRebarMain_Bracket_Cycle1[iSTT], nFormat, bSttCycle1 ? nLine : CELL_READONLY, "", CELL_TYPE_DIA);
	m_Grid.AddCell(row, col++, &pBri->m_dDiaRebarMain_Bracket_Cycle2[iSTT], nFormat, bSttCycle2 ? nLine : CELL_READONLY, "", CELL_TYPE_DIA);
	m_Grid.AddCell(row, col++, &pBri->m_dDiaRebarMain_Bracket_Cycle1[iEND], nFormat, bEndCycle1 ? nLine : CELL_READONLY, "", CELL_TYPE_DIA);
	m_Grid.AddCell(row, col++, &pBri->m_dDiaRebarMain_Bracket_Cycle2[iEND], nFormat, bEndCycle2 ? nLine : CELL_READONLY, "", CELL_TYPE_DIA);
	row++;

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidth(0, 60);
	m_Grid.SetColumnWidth(1, 100);
	m_Grid.SetColumnWidth(2, 100);
}

void CInputRebarMain3::SetGridData_BFooting()
{
	m_Grid.SetGridDefault();
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	if(!pBri->m_bExistProtectionRiseFooting)
	{
		m_Grid.SetGridDefault(1,1,1,1,TRUE,FALSE);
		m_Grid.SetColumnWidth(0,300);
		m_Grid.SetRowHeight(0,50);
		m_Grid.AddCell(0,0,_T("�λ���������� ��ġ�Ǿ� ���� �ʽ��ϴ�."));
		return;
	}

	long nRow = 0;
	long nCol = 0;
	UINT nFormat = DT_CENTER|DT_VCENTER;
	UINT nLine = CELL_LINE_NO;

	CStringArray strArr;
	strArr.Add(_T("��ö��CTC"));
	strArr.Add(_T("��ö��CTC/2"));

	m_Grid.SetGridDefault(3,1,2,0,TRUE,FALSE);

	nRow = 0;
	m_Grid.AddCell(nRow++, nCol, _T("DM"));
	m_Grid.AddCell(nRow++, nCol, _T("����"));
	m_Grid.AddCell(nRow, nCol, &pBri->m_dPRF_Dia_Main, nFormat, nLine, _T(""), CELL_TYPE_DIA);
	++nCol;

	nRow = 0;
	m_Grid.AddCell(nRow++, nCol, _T("DM"));
	m_Grid.AddCell(nRow++, nCol, _T("CTC"));
	m_Grid.AddCell(nRow, nCol, &pBri->m_bPRF_SameCTC_Main, nFormat, nLine, _T(""), CELL_TYPE_COMBO, strArr);
	++nCol;

	nRow = 0;
	m_Grid.AddCell(nRow++, nCol, _T("BO2"));
	m_Grid.AddCell(nRow++, nCol, _T("����"));
	m_Grid.AddCell(nRow, nCol, &pBri->m_dPRF_Dia_Support, nFormat, nLine, _T(""), CELL_TYPE_DIA);
	++nCol;

	nRow = 0;
	m_Grid.AddCell(nRow++, nCol, _T("BO2"));
	m_Grid.AddCell(nRow++, nCol, _T("���� CTC"));
	m_Grid.AddCell(nRow, nCol, &pBri->m_dPRF_CTC_Support, nFormat, nLine, _T(""));
	++nCol;

	nRow = 0;
	m_Grid.AddCell(nRow++, nCol, _T("BO2"));
	m_Grid.AddCell(nRow++, nCol, _T("���� CTC"));
	m_Grid.AddCell(nRow, nCol, &pBri->m_dPRF_CTC_SupportVert, nFormat, nLine, _T(""));
	++nCol;

	nRow = 0;
	m_Grid.AddCell(nRow++, nCol, _T("BO3"));
	m_Grid.AddCell(nRow++, nCol, _T("����"));
	m_Grid.AddCell(nRow, nCol, &pBri->m_dPRF_Dia_Shear, nFormat, nLine, _T(""), CELL_TYPE_DIA);
	++nCol;

	nRow = 0;
	m_Grid.AddCell(nRow++, nCol, _T("BO3"));
	m_Grid.AddCell(nRow++, nCol, _T("CTC"));
	m_Grid.AddCell(nRow, nCol, &pBri->m_dPRF_CTC_Shear, nFormat, nLine, _T(""));
	++nCol;

	CStringArray sArrType,sArrRebar;
	sArrType.Add(_T("ĸ��"));
	sArrType.Add(_T("��ħ��"));
	sArrType.Add(_T("����"));
	sArrType.Add(_T("���� ����"));
	sArrType.Add(_T("ĸ��(������)"));
	sArrType.Add(_T("������"));

	CString str = _T("");
	sArrRebar.Add("����");
	long nIdx = 0;
	for(nIdx = 1; nIdx < CUL_MAX_REBAR; nIdx ++)
	{
		str.Format("%d", nIdx);
		sArrRebar.Add(str);
	}	

	nRow = 0;
	m_Grid.AddCell(nRow++, nCol, _T("BO3"));
	m_Grid.AddCell(nRow++, nCol, _T("Ÿ��"));
	m_Grid.AddCell(nRow, nCol, &pBri->m_nPRF_Type_Shear, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArrType);
	++nCol;

	nRow = 0;
	m_Grid.AddCell(nRow++, nCol, _T("BO3"));
	m_Grid.AddCell(nRow++, nCol, _T("��ö�� ������"));
	m_Grid.AddCell(nRow, nCol, &pBri->m_nPRF_Count_Shear, nFormat, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArrRebar);
	++nCol;

	m_Grid.GetCellCount(nRow, nCol);
	m_Grid.SetRows(nRow);
	m_Grid.SetCols(nCol);
	m_Grid.SetColumnWidthAll(75);
	m_Grid.SetColumnWidth(1,110);
	m_Grid.SetColumnWidth(nCol-1,100);
}

void CInputRebarMain3::OnSelchangeComboDanbu() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	pBri->m_nType_DanbuRebar = m_comboDanbu.GetCurSel();
	SyncRebar();
}

void CInputRebarMain3::SetGridData_Stirrup()
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(3, 2, 3, 0);
	m_Grid.SetCellSize(20, 80);

	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long row = 0, col = 0;
	CStringArray sArr, sArr2;
	CString str = _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	col	= 0;
	
	long i = 0; for(i = 0; i < 10; i++)
	{
		str.Format("%d", i);
		sArr.Add(str);
		str.Format("%d", i+1);
		sArr2.Add(str);
	}
	
	long nCountSlab = 1;
	if(pBri->IsBoxType()) nCountSlab = 2;

	for(long iUD = 0; iUD < nCountSlab; iUD++)
	{
		CString strSlab = (iUD == iUPPER) ? _T("��ν�����") : _T("�Ϻν�����");

		m_Grid.AddCell(row, col, strSlab, nFormat, CELL_READONLY);
		m_Grid.AddCell(row+1, col, "����", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+2, col, "����", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+3, col++, &pBri->m_dDiaStirrupAtDanBuRebar[iUD][iSTT], nFormat, nLine, " ", CELL_TYPE_DIA);
		m_Grid.SetLastCellFy(pBri->GetValueFy(iUD == iUPPER? ePartUpperSlab : ePartLowerSlab));

		m_Grid.AddCell(row, col, strSlab, nFormat, CELL_READONLY);
		m_Grid.AddCell(row+1, col, "����", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+2, col, "����", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+3, col++, &pBri->m_nCountStirrupAtDanBuRebar[iUD][iSTT], nFormat, FALSE, nLine, " ", CELL_TYPE_COMBO, sArr);
	
		m_Grid.AddCell(row, col, strSlab, nFormat, CELL_READONLY);
		m_Grid.AddCell(row+1, col, "����", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+2, col, "��ö�ٱ�����", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+3, col++, &pBri->m_nCountStirrupCtcMainRebar[iUD][iSTT], nFormat, FALSE, nLine, " ", CELL_TYPE_COMBO, sArr2);
		
		m_Grid.AddCell(row, col, strSlab, nFormat, CELL_READONLY);
		m_Grid.AddCell(row+1, col, "����", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+2, col, "����", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+3, col++, &pBri->m_dDiaStirrupAtDanBuRebar[iUD][iEND], nFormat, nLine, " ", CELL_TYPE_DIA);
			m_Grid.SetLastCellFy(pBri->GetValueFy(iUD == iUPPER? ePartUpperSlab : ePartLowerSlab));
		
		m_Grid.AddCell(row, col, strSlab, nFormat, CELL_READONLY);
		m_Grid.AddCell(row+1, col, "����", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+2, col, "����", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+3, col++, &pBri->m_nCountStirrupAtDanBuRebar[iUD][iEND], nFormat, FALSE, nLine, " ", CELL_TYPE_COMBO, sArr);
		
		m_Grid.AddCell(row, col, strSlab, nFormat, CELL_READONLY);
		m_Grid.AddCell(row+1, col, "����", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+2, col, "��ö�ٱ�����", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+3, col++, &pBri->m_nCountStirrupCtcMainRebar[iUD][iEND], nFormat, FALSE, nLine, " ", CELL_TYPE_COMBO, sArr2);
	}
}

// �а��� ���� ö��
void CInputRebarMain3::SetGridData_ObtuseAngle()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	BOOL bBoxType = pBri->IsBoxType();

	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 2, bBoxType? 2: 1);
	m_Grid.SetCellSize(20, -1);

	long row = 0, col = 0;
	CStringArray sArr;
	CString str = _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	
	BOOL bUpper	= m_comboObtuse.GetCurSel() == 0;
	long upper	= bUpper ? iUPPER : iLOWER;
	CString sUpper	= bUpper ? "���" : "�ϸ�";
	
	if(bBoxType)	m_Grid.AddCell(row, col++, "����\n");
	m_Grid.AddCell(row, col++, "����\n");
	m_Grid.AddCell(row, col++, "��ġ\n");
	m_Grid.AddCell(row, col++, "ö������");	// ��ö�� ����
	m_Grid.AddCell(row, col++, "ö������");	// ��±� ����

	m_Grid.AddCell(row, col++, "�Ǻ�");	// �а��� ��� �Ǻ�
	m_Grid.AddCell(row, col++, "�Ǻ�");	// �а��� �ϸ� �Ǻ�
	m_Grid.AddCell(row, col++, "�Ǻ�");	// �а��� ������ �Ǻ�
	m_Grid.AddCell(row, col++, "�Ǻ�");	// �а��� �¿��� �Ǻ�

	m_Grid.AddCell(row, col++, "��ö�ٱ���");	// ��� ��ö�� ����(��������)
	m_Grid.AddCell(row, col++, "��ö�ٱ���");	// ��� ��ö�� ����(��ü����)
	m_Grid.AddCell(row, col++, "��ö�ٱ���");	// �ϸ� ��ö�� ����
	m_Grid.AddCell(row, col++, sUpper+" ��±ٹ�ġ");	// ���ö�� ��ġ ���� ����
	m_Grid.AddCell(row, col++, sUpper+" ��±ٹ�ġ");	// ���ö�� ��ġ ����
	m_Grid.AddCell(row, col++, sUpper+" ��±ٹ�ġ");	// ���ö�� ��ġ ����
	m_Grid.AddCell(row, col++, sUpper+" ��±ٹ�ġ");	// ���ö�� ��ġ ������ ����
	m_Grid.AddCell(row, col++, sUpper+" ��ö�ٹ�ġ");	// ��ö�� ��ġ ���� ����
	m_Grid.AddCell(row, col++, sUpper+" ��ö�ٹ�ġ");	// ��ö�� ��ġ ����
	m_Grid.AddCell(row, col++, sUpper+" ��ö�ٹ�ġ");	// ��ö�� ��ġ ����
	m_Grid.AddCell(row, col++, sUpper+" ��ö�ٹ�ġ");	// ��ö�� ��ġ ������ ����

	if(bUpper)
	{
		m_Grid.AddCell(row, col++, "��ü ��±ٹ�ġ");	// ���ö�� ��ġ ���� ����
		m_Grid.AddCell(row, col++, "��ü ��±ٹ�ġ");	// ���ö�� ��ġ ����
		m_Grid.AddCell(row, col++, "��ü ��±ٹ�ġ");	// ���ö�� ��ġ ����
		m_Grid.AddCell(row, col++, "��ü ��±ٹ�ġ");	// ���ö�� ��ġ ������ ����
	}

	row++;
	col	= 0;
	if(bBoxType)	m_Grid.AddCell(row, col++, "����\n");
	m_Grid.AddCell(row, col++, "����\n");
	m_Grid.AddCell(row, col++, "��ġ\n");
	m_Grid.AddCell(row, col++, bUpper ? "UD" : "LD");	// ��ö�� ����
	m_Grid.AddCell(row, col++, bUpper ? "SDU" : "SDL");	// ��±� ����

	m_Grid.AddCell(row, col++, "D1");	// �а��� ��� �Ǻ�
	m_Grid.AddCell(row, col++, "D2");	// �а��� �ϸ� �Ǻ�
	m_Grid.AddCell(row, col++, "D3");	// �а��� ������ �Ǻ�
	m_Grid.AddCell(row, col++, "D4");	// �а��� �¿��� �Ǻ�

	m_Grid.AddCell(row, col++, "UL1");	// ��� ��ö�� ����(��������)
	m_Grid.AddCell(row, col++, "UL2");	// ��� ��ö�� ����(��ü����)
	m_Grid.AddCell(row, col++, "LL");	// �ϸ� ��ö�� ����
	m_Grid.AddCell(row, col++, "C1");	// ���ö�� ��ġ ���� ����
	m_Grid.AddCell(row, col++, "B");	// ���ö�� ��ġ ����
	m_Grid.AddCell(row, col++, "C");	// ���ö�� ��ġ ����
	m_Grid.AddCell(row, col++, "C2");	// ���ö�� ��ġ ������ ����
	m_Grid.AddCell(row, col++, "F1");	// ��ö�� ��ġ ���� ����
	m_Grid.AddCell(row, col++, "E");	// ��ö�� ��ġ ����
	m_Grid.AddCell(row, col++, "F");	// ��ö�� ��ġ ����
	m_Grid.AddCell(row, col++, "F2");	// ��ö�� ��ġ ������ ����

	if(bUpper)
	{
		m_Grid.AddCell(row, col++, "C1'");	// ���ö�� ��ġ ���� ����
		m_Grid.AddCell(row, col++, "B'");	// ���ö�� ��ġ ����
		m_Grid.AddCell(row, col++, "C'");	// ���ö�� ��ġ ����
		m_Grid.AddCell(row, col++, "C2'");	// ���ö�� ��ġ ������ ����
	}

	

	for(long stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		CString sStt	= bStt ? "����" : "����";
		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT;
			CString sLeft	= bLeft ? "����" : "����";

			CPlaceRebarByBlock *pPlaceL	= &pBri->m_placeByBlock_ObtuseAngle_Support_UpperSlab[stt][left][upper];
			CPlaceRebarByBlock *pPlaceC	= &pBri->m_placeByBlock_ObtuseAngle_Main_UpperSlab[stt][left][upper];

			col	= 0;
			row++;
			if(bBoxType)
				m_Grid.AddCell(row, col++, _T("��ν�����"));
			m_Grid.AddCell(row, col++, sStt+sLeft);
			m_Grid.AddCell(row, col++, &pBri->m_bIsObtuseAngle_UpperSlab[stt][left], nFormat, nLine, "", CELL_TYPE_CHECK);
			m_Grid.AddCell(row, col++, &pPlaceC->m_dDia, nFormat, nLine, sUpper+" ��ö�� ö�� ����", CELL_TYPE_DIA);
			m_Grid.AddCell(row, col++, &pPlaceL->m_dDia, nFormat, nLine, sUpper + " ���ö�� ����", CELL_TYPE_DIA);

			m_Grid.AddCell(row, col++, &pBri->m_dCoverObtuseAngle_UpperSlab[stt][left][iUPPER], nFormat, nLine, "�а��� ��� �Ǻ�");
			m_Grid.AddCell(row, col++, &pBri->m_dCoverObtuseAngle_UpperSlab[stt][left][iLOWER], nFormat, nLine, "�а��� �ϸ� �Ǻ�");
			m_Grid.AddCell(row, col++, &pBri->m_dCoverObtuseAngle_UpperSlab_Plane[stt][left][0], nFormat, nLine, "�а��� ������ �Ǻ�");
			m_Grid.AddCell(row, col++, &pBri->m_dCoverObtuseAngle_UpperSlab_Plane[stt][left][1], nFormat, nLine, "�а��� �¿��� �Ǻ�");

			m_Grid.AddCell(row, col++, &pBri->m_dLenObtuseAngle_UpperSlab[stt][left][iUPPER], nFormat, nLine, "��� ��ö�� ����");
			m_Grid.AddCell(row, col++, &pBri->m_dLenObtuseAngle_OutWall[stt][left], nFormat, nLine, "��� ��ö�� ��ü ���� ����");
			m_Grid.AddCell(row, col++, &pBri->m_dLenObtuseAngle_UpperSlab[stt][left][iLOWER], nFormat, nLine, "�ϸ� ��ö�� ����");
			m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 0, FALSE, nFormat, nLine, sUpper + "��� ö�� ��ġ ó������");
			m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 1, TRUE, nFormat, nLine, sUpper + "��� ö�� ��ġ ����");
			m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 1, FALSE, nFormat, nLine, sUpper + "��� ö�� ��ġ ����");
			m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 2, FALSE, nFormat, nLine, sUpper + "��� ö�� ��ġ ����������");
			m_Grid.AddCell(row, col++, &pPlaceC->m_xyArr, 0, FALSE, nFormat, nLine, sUpper + "��ö�� ��ġ ó������");
			m_Grid.AddCell(row, col++, &pPlaceC->m_xyArr, 1, TRUE, nFormat, nLine, sUpper + "��ö�� ��ġ ����");
			m_Grid.AddCell(row, col++, &pPlaceC->m_xyArr, 1, FALSE, nFormat, nLine, sUpper + "��ö�� ��ġ ����");
			m_Grid.AddCell(row, col++, &pPlaceC->m_xyArr, 2, FALSE, nFormat, nLine, sUpper + "��ö�� ��ġ ����������");

			// ��� �Է����϶� ��ü ��ġ �Էµ� ����
			if(bUpper)
			{
				pPlaceL	= &pBri->m_placeByBlock_ObtuseAngle_Support_OutWall[stt][left];
				m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 0, FALSE, nFormat, nLine, "������ü ��� ö�� ��ġ ó������");
				m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 1, TRUE, nFormat, nLine, "������ü ��� ö�� ��ġ ����");
				m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 1, FALSE, nFormat, nLine, "������ü ��� ö�� ��ġ ����");
				m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 2, FALSE, nFormat, nLine, "������ü ��� ö�� ��ġ ����������");
			}
		}
	}

	if(bBoxType)
	{
		for(long stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			CString sStt	= bStt ? _T("����") : _T("����");
			for(long left = 0; left < 2; left++)
			{
				BOOL bLeft	= left == iLEFT;
				CString sLeft	= bLeft ? _T("����") : _T("����");

				CPlaceRebarByBlock *pPlaceL	= &pBri->m_placeByBlock_ObtuseAngle_Support_LowerSlab[stt][left][upper];
				CPlaceRebarByBlock *pPlaceC	= &pBri->m_placeByBlock_ObtuseAngle_Main_LowerSlab[stt][left][upper];

				col	= 0;
				row++;
				m_Grid.AddCell(row, col++, _T("�Ϻν�����"));
				m_Grid.AddCell(row, col++, sStt+sLeft);
				m_Grid.AddCell(row, col++, &pBri->m_bIsObtuseAngle_LowerSlab[stt][left], nFormat, nLine, "", CELL_TYPE_CHECK);
				m_Grid.AddCell(row, col++, &pPlaceC->m_dDia, nFormat, nLine, sUpper+" ��ö�� ö�� ����", CELL_TYPE_DIA);
				m_Grid.AddCell(row, col++, &pPlaceL->m_dDia, nFormat, nLine, sUpper + " ���ö�� ����", CELL_TYPE_DIA);

				m_Grid.AddCell(row, col++, &pBri->m_dCoverObtuseAngle_LowerSlab[stt][left][iUPPER], nFormat, nLine, "�а��� ��� �Ǻ�");
				m_Grid.AddCell(row, col++, &pBri->m_dCoverObtuseAngle_LowerSlab[stt][left][iLOWER], nFormat, nLine, "�а��� �ϸ� �Ǻ�");
				m_Grid.AddCell(row, col++, &pBri->m_dCoverObtuseAngle_LowerSlab_Plane[stt][left][0], nFormat, nLine, "�а��� ������ �Ǻ�");
				m_Grid.AddCell(row, col++, &pBri->m_dCoverObtuseAngle_LowerSlab_Plane[stt][left][1], nFormat, nLine, "�а��� �¿��� �Ǻ�");

				m_Grid.AddCell(row, col++, &pBri->m_dLenObtuseAngle_LowerSlab[stt][left][iUPPER], nFormat, nLine, "��� ��ö�� ����");
				m_Grid.AddCell(row, col++, &pBri->m_dLenObtuseAngle_LowerOutWall[stt][left], nFormat, nLine, "��� ��ö�� ��ü ���� ����");
				m_Grid.AddCell(row, col++, &pBri->m_dLenObtuseAngle_LowerSlab[stt][left][iLOWER], nFormat, nLine, "�ϸ� ��ö�� ����");
				m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 0, FALSE, nFormat, nLine, sUpper + "��� ö�� ��ġ ó������");
				m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 1, TRUE, nFormat, nLine, sUpper + "��� ö�� ��ġ ����");
				m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 1, FALSE, nFormat, nLine, sUpper + "��� ö�� ��ġ ����");
				m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 2, FALSE, nFormat, nLine, sUpper + "��� ö�� ��ġ ����������");
				m_Grid.AddCell(row, col++, &pPlaceC->m_xyArr, 0, FALSE, nFormat, nLine, sUpper + "��ö�� ��ġ ó������");
				m_Grid.AddCell(row, col++, &pPlaceC->m_xyArr, 1, TRUE, nFormat, nLine, sUpper + "��ö�� ��ġ ����");
				m_Grid.AddCell(row, col++, &pPlaceC->m_xyArr, 1, FALSE, nFormat, nLine, sUpper + "��ö�� ��ġ ����");
				m_Grid.AddCell(row, col++, &pPlaceC->m_xyArr, 2, FALSE, nFormat, nLine, sUpper + "��ö�� ��ġ ����������");

				// ��� �Է����϶� ��ü ��ġ �Էµ� ����
				if(bUpper)
				{
					pPlaceL	= &pBri->m_placeByBlock_ObtuseAngle_Support_LowerOutWall[stt][left];
					m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 0, FALSE, nFormat, nLine, "������ü ��� ö�� ��ġ ó������");
					m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 1, TRUE, nFormat, nLine, "������ü ��� ö�� ��ġ ����");
					m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 1, FALSE, nFormat, nLine, "������ü ��� ö�� ��ġ ����");
					m_Grid.AddCell(row, col++, &pPlaceL->m_xyArr, 2, FALSE, nFormat, nLine, "������ü ��� ö�� ��ġ ����������");
				}
			}
		}
	}

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(0, 100);
	if(bBoxType)
		m_Grid.SetColumnWidth(1, 100);
}


// �а��� ���� ö��
void CInputRebarMain3::SetGridData_AcuteAngle()
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 1, 1);
	m_Grid.SetCellSize(20, -1);

	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	long row = 0, col = 0;
	CStringArray sArr;
	CString str = _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	
	BOOL bUpper	= m_comboObtuse.GetCurSel() == 0;
	CString sUpper	= bUpper ? "���" : "�ϸ�";

	m_Grid.AddCell(row, col++, "����");
	m_Grid.AddCell(row, col++, "��ġ");
	m_Grid.AddCell(row, col++, "OD");	// ��ü ��ö�� ����
	m_Grid.AddCell(row, col++, "FD");	// ���� ��ö�� ����
	m_Grid.AddCell(row, col++, "ODS");	// ��ü ���ö�� ����
	m_Grid.AddCell(row, col++, "FDS");	// ���� ���ö�� ����

	m_Grid.AddCell(row, col++, "D1");	// ��ü ���� �Ǻ�
	m_Grid.AddCell(row, col++, "D2");	// ���� �ϸ� �Ǻ�
	m_Grid.AddCell(row, col++, "D3");	// ���� ��� �Ǻ�
	

	m_Grid.AddCell(row, col++, "OL");	// ��ü ���� ö�� ����
	m_Grid.AddCell(row, col++, "FL");	// ���� ��� ö�� ����

	m_Grid.AddCell(row, col++, "WMR");	// ��ü ��ö�� ��� ����
	m_Grid.AddCell(row, col++, "FMR");	// ���� ��ö�� ��� ����
	

	for(long stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		CString sStt	= bStt ? "����" : "����";
		CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;

		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT;
			CString sLeft	= bLeft ? "����" : "����";
			BOOL bFront	= bStt ? bLeft : !bLeft;
			long nFront	= bFront ? iFRONT : iBACK;

			CPlaceRebarByBlock *pPlaceO		= &pBri->m_placeByBlock_AcuteAngle_MainRebar_OutWall[stt][left];
			CPlaceRebarByBlock *pPlaceF		= &pFooting->m_placeByBlock_Obtuse_MainRebar[nFront];
			CPlaceRebarByBlock *pPlaceOS	= &pBri->m_placeByBlock_AcuteAngle_SupportRebar_OutWall[stt][left];
			CPlaceRebarByBlock *pPlaceFS	= &pFooting->m_placeByBlock_Obtuse_SupportRebar[nFront];

			col	= 0;
			row++;
			m_Grid.AddCell(row, col++, sStt+sLeft);
			m_Grid.AddCell(row, col++, &pBri->m_bIsAcuteAngle_OutWall[stt][left], nFormat, nLine, "", CELL_TYPE_CHECK);
			m_Grid.AddCell(row, col++, &pPlaceO->m_dDia, nFormat, nLine, "��ü ������ ��ö�� ����", CELL_TYPE_DIA);
			m_Grid.AddCell(row, col++, &pPlaceF->m_dDia, nFormat, nLine, "���� �а��� ��ö�� ����", CELL_TYPE_DIA);
			m_Grid.AddCell(row, col++, &pPlaceOS->m_dDia, nFormat, nLine, "��ü ������ ���ö�� ����", CELL_TYPE_DIA);
			m_Grid.AddCell(row, col++, &pPlaceFS->m_dDia, nFormat, nLine, "���� �а��� ���ö�� ����", CELL_TYPE_DIA);

			m_Grid.AddCell(row, col++, &pBri->m_dCoverAcuteAngle_OutWall[stt][left][iUPPER], nFormat, nLine, "��ü ������ ö�� ���� �Ǻ�");
			m_Grid.AddCell(row, col++, &pBri->m_dCoverAcuteAngle_OutWall[stt][left][iLOWER], nFormat, nLine, "��ü ������ ö�� �ϸ� �Ǻ�");
			m_Grid.AddCell(row, col++, &pFooting->m_dCoverObtuseUpper[nFront], nFormat, nLine, "���� �а��� ö�� ��� �Ǻ�");
			
			m_Grid.AddCell(row, col++, &pBri->m_dLenAcuteangle_OutWall[stt][left], nFormat, nLine, "��ü ������ ö�� ����");
			m_Grid.AddCell(row, col++, &pFooting->m_dLenObtuseUpper[nFront], nFormat, nLine, "���� �а��� ö�� ����");

			m_Grid.AddCell(row, col++, &pBri->m_dRange_AcuteAngle_OutWall[stt][left], nFormat, nLine, "��ü ��ö�� ���� ����");
			m_Grid.AddCell(row, col++, &pFooting->m_dRange_Obtuse_MainRebar[nFront], nFormat, nLine, "���� ��ö�� ���� ����");
		}
	}

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(50);
	m_Grid.SetColumnWidth(0, 100);
}

void CInputRebarMain3::OnSelchangeComboObtuse() 
{
	// TODO: Add your control notification handler code here
	CCellID cell		= m_Grid.GetFocusCell();
	
	SetGridData();
	SetDataInit();
	DrawInputDomyunView(FALSE);

	m_Grid.SetFocusCell(cell.row, cell.col);
}



void CInputRebarMain3::OnBnClickedButtonCorner()
{
	CDlgCorner dlg(m_pIn);
	
	if(dlg.DoModal()==IDOK)
		DrawInputDomyunView();
}
