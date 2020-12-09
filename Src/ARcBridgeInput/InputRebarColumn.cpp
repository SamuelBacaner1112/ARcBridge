// InputRebarColumn.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "../../Src/ARcBridgeCalculate/ARcBridgeCalculate.h"
#include "../ARcBridgeDBDraw/ARcBridgeDBDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputRebarColumn dialog


CInputRebarColumn::CInputRebarColumn(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputRebarColumn::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputRebarColumn)
	m_sCtcGirder = _T("");
	m_bViewPM = FALSE;
	m_nViewCross = -1;
	//}}AFX_DATA_INIT
	m_nTab		= 0;
	m_nTabRow	= 0;
	m_pDlgCheck = new CInputCheckGrid;
	m_pDlgEdit  = new CInputCheckEdit;

	m_nTabUpperGirder	= -1;
	m_nTabLowerGirder	= -1;
	m_nTabFooting		= -1;
	m_nTabColumn		= -1;
	m_nTabWall			= -1;
	m_nTabJongGirder	= -1;

}

CInputRebarColumn::~CInputRebarColumn()
{

	delete m_pDlgCheck;
	delete m_pDlgEdit;
}


void CInputRebarColumn::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputRebarColumn)
	DDX_Control(pDX, IDC_CHECK_RAD_CROSSTIE, m_checkRadCrossTie);
	DDX_Control(pDX, IDC_COMBO_INPUT_COLUMN, m_comboInputColumn);
	DDX_Control(pDX, IDC_CHECK_MATCH_WITHWALL, m_checkMatchWidthWall);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_TAB_ROW, m_TabRow);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	DDX_Control(pDX, IDC_BUTTON_EXAMINE, m_btnExamine);
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_comboBridgeNo);
	DDX_Text(pDX, IDC_EDIT_CTC_GIRDER, m_sCtcGirder);
	DDX_Check(pDX, IDC_CHECK_VIEW_PM, m_bViewPM);
	DDX_Radio(pDX, IDC_RADIO_CROSS, m_nViewCross);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputRebarColumn, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputRebarColumn)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnSelchangingTab)
	ON_EN_KILLFOCUS(IDC_EDIT_CTC_GIRDER, OnKillfocusEditCtcGirder)
	ON_BN_CLICKED(IDC_BUTTON_EXAMINE, OnButtonExamine)
	ON_BN_CLICKED(IDC_CHECK_MATCH_WITHWALL, OnCheckMatchWithwall)
	ON_BN_CLICKED(IDC_CHECK_VIEW_PM, OnCheckViewPM)
	ON_CBN_SELCHANGE(IDC_COMBO_INPUT_COLUMN, OnSelchangeComboInputColumn)
	ON_BN_CLICKED(IDC_RADIO_CROSS, OnRadioCross)
	ON_BN_CLICKED(IDC_RADIO_PLANE, OnRadioPlane)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_ROW, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_ROW, OnTabSelchanging)
	ON_BN_CLICKED(IDC_CHECK_RAD_CROSSTIE, OnCheckRadCrosstie)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_NOTIFY(GVN_CELL_CHANGED_ROW,  IDC_GRID, OnCellChangedRow)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputRebarColumn message handlers
void CInputRebarColumn::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	
	return;
}

void CInputRebarColumn::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	// TODO: Add your control notification handler code here
	SetFocus();
	m_nTabRow = m_TabRow.GetCurSel();
	if(SetGridDataStd())
		SetDataInit();
	SetControl();
	DrawInputDomyunView(TRUE);

	return;
}

BOOL CInputRebarColumn::SetGridDataStd()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return FALSE;

	if(m_nTab == m_nTabUpperGirder) return SetGridDataUpperGirder(TRUE);
	else if(m_nTab == m_nTabLowerGirder) return SetGridDataUpperGirder(FALSE);
	else if(m_nTab == m_nTabFooting) return SetGridDataFooting();
	else if(m_nTab == m_nTabColumn) 
	{
		if(m_comboInputColumn.GetCurSel() == 0)
			return SetGridDataColumn();
		else
			return SetGridDataCrossTieAndRound();
	}
	else if(m_nTab == m_nTabWall) return SetGridDataWall(m_nTabRow);
	else if(m_nTab == m_nTabJongGirder) return SetGridDataJongGirder();

	return TRUE;
}

void CInputRebarColumn::SetDataSave()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

 	m_Grid.UpdateData(TRUE);

	// ��Ʈ�� �� ����

	SyncRebar();
	if(pBri->IsBoxType())
		SyncRebar(FALSE);

	m_pIn->m_pDoc->SetModifiedFlag();

	if(m_pDlgCheck->GetSafeHwnd() && m_pDlgCheck->IsWindowVisible())
	{
		m_pDlgCheck->Check();
	}
	if(m_pDlgEdit->GetSafeHwnd() && m_pDlgEdit->IsWindowVisible())
	{
		m_pDlgEdit->Check();
	}

	if(m_nTab == m_nTabUpperGirder || m_nTab == m_nTabLowerGirder || m_nTab == m_nTabJongGirder)
	{
		long nJ	= m_nTabRow;
		BOOL bStt	= TRUE;
		BOOL bLeft	= TRUE;
		BOOL bGagak	= TRUE;
		BOOL bJongGirder	= TRUE;
		GetInfoGirder(bStt, bLeft, bGagak, bJongGirder);
		pBri->SetDataDefaultWidthGirder(nJ, bStt, bLeft, bGagak, bJongGirder);

		// ���Ŵ��� �ƴϰ� �������� �ƴ� ��� �����Ҷ����� �Ŵ��� ������ġ �ǰ���� �� �ش�.
		if(m_nTab == m_nTabUpperGirder || m_nTab == m_nTabLowerGirder)
		{
			if(nJ > 0 && nJ < pBri->m_nQtyJigan)
			{
				pBri->SetDataDefaultRebarGirder(nJ, FALSE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE, m_nTab == m_nTabUpperGirder);
			}
		}
	}

	if(m_nTab == m_nTabColumn)
	{
		CWallApp *pWall		= GetCurInWall();
		if(pWall)
		{
			BOOL bCheck	= m_checkRadCrossTie.GetCheck();
			pWall->m_Column.m_nTypeRebarCrossTie	= bCheck ? 1 : 0;

		}
	}
}

void CInputRebarColumn::SetDataDefault()
{
	for(long bri = 0; bri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(bri);
		if(!pBri) continue;

		BOOL bFirst   = IsFirst(bri);
		pBri->SetDefaultedState(GetIDD(), 0x00000001);

		if(!bFirst && !IsClickDefaultButton(bri)) continue;
//		long dan	= 0;

		if(bFirst)
		{
			// �Ŵ�
			// ó������ �Ŵ����� �⺻�����ְ� �״������ʹ� ö�ٹ�ġ�� ���� ����
			pBri->SetDataDefaultGirderStirrupFirst();

			long nJ = 0; for(nJ = 0; nJ < pBri->GetCountJijum(); nJ++)
			{

				pBri->SetDataDefaultRebarMainInGirder_Girder(nJ);
				pBri->SetDataDefaultRebarGirder(nJ, FALSE, TRUE);
				if(pBri->IsBoxType())
				{
					pBri->SetDataDefaultRebarMainInGirder_Girder(nJ, TRUE, TRUE, TRUE, FALSE, FALSE);
					pBri->SetDataDefaultRebarGirder(nJ, FALSE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE);
				}

				pBri->SetDataDefaultRebarColumnStd(nJ);
			}

			// ���Ŵ�
			for(long s = 0; s < 2; s++)
			{
				for(long left = 0; left < 2; left++)
				{
					BOOL bLeft	= left == iLEFT;
					pBri->SetDataDefaultRebarMainInGirder_Girder(0, TRUE, bLeft, FALSE, TRUE);
					pBri->SetDataDefaultRebarGirder(0, FALSE, FALSE, TRUE, bLeft, FALSE, TRUE);
					for(long stt = 0; stt < 2; stt++)
					{
						BOOL bStt	= stt == iSTT;
						pBri->SetDataDefaultRebarMainInGirder_Girder(0, bStt, bLeft, TRUE, TRUE);
						pBri->SetDataDefaultRebarGirder(0, FALSE, FALSE, bStt, bLeft, TRUE, TRUE);
					}
				}
			}
			
			long nCountFooting	= pBri->GetCountFooting();
			long footing = 0; for(footing = 0; footing < nCountFooting; footing++)
			{
				CFootingApp *pFooting	= pBri->GetFooting(footing);
				if(pFooting)
				{
					for(long dan = 0; dan < MAX_REBAR_DAN; dan++)
					{
						pFooting->SetDataDefaultRebarMain_Side(TRUE, dan);
						pFooting->SetDataDefaultRebarMain_Side(FALSE, dan);
					}
					pFooting->SetDataDefaultRebarShear_Front(FALSE);
				}
			}
		}
		else
		{
			if(IsClickDefaultButton(bri))
			{
				long nJ	= m_nTabRow;
				if(m_nTab == m_nTabUpperGirder || m_nTab == m_nTabLowerGirder)
				{
					for(long s = 0; s < 2; s++)
					{
						pBri->SetDataDefaultRebarMainInGirder_Girder(nJ, TRUE, TRUE, TRUE, FALSE, m_nTab == m_nTabUpperGirder);
						// (ARCBRIDGE-3672) ��Ʈ��ö�ٵ� �ʱ�ȭ �ϵ��� bOnlyOrg = TRUE => FALSE
						pBri->SetDataDefaultRebarGirder(nJ, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, FALSE, m_nTab == m_nTabUpperGirder);
					}
				}

				
				if(m_nTab == m_nTabColumn)
				{
					long nIdxInWall	= nJ - 1;
					CWallApp *pWall	= pBri->GetInWall(nIdxInWall);
					if(pWall)
					{
						if(m_comboInputColumn.GetCurSel() == 0)
						{
							pWall->m_Column.SetDataDefaultRebarMain_Plane(!pWall->IsRoundRectWall());
						}
						else
						{
							pWall->m_Column.SetDataDefaultRebarCrossTie_Plane();
							pBri->SetDataDefaultRebarRoundAndCrossTie_VertSection_InWall(nIdxInWall);
						}
					}
				}

				if(m_nTab == m_nTabFooting)
				{
					CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(nJ);
					if(pFooting)
					{
						pBri->SetDataDefaultRebarMain_Front_Footing(nJ, m_checkMatchWidthWall.GetCheck());
						pFooting->SetDataDefaultRebarShear_Front(FALSE);
					}
				}

				if(m_nTab == m_nTabWall)
				{
					pBri->SetDataDefaultRangeSupportRebarForSameUpperSlab(nJ);
				}

				if(m_nTab == m_nTabJongGirder)
				{
					BOOL bStt	= TRUE;
					BOOL bLeft	= TRUE;
					BOOL bGagak	= TRUE;
					BOOL bJongGirder	= TRUE;
					GetInfoGirder(bStt, bLeft, bGagak, bJongGirder);

					for(long s = 0; s < 2; s++)
					{
						pBri->SetDataDefaultRebarMainInGirder_Girder(nJ, bStt, bLeft, bGagak, bJongGirder);
						pBri->SetDataDefaultRebarGirder(nJ, FALSE, FALSE, bStt, bLeft, bGagak, bJongGirder);
					}
				}
			}
		}
	}

	if(SetGridDataStd())
		SetDataInit();
}

void CInputRebarColumn::InitTabCtrl()
{
	m_TabRow.RemoveAllTabs();
	m_TabRow.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_TabRow.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	// row��
	CString str			= _T("");
	CStringArray sArrTabRow;
	long nTab			= 0;
	long nJ = 0; for(nJ = 0; nJ < pBri->GetCountJijum(); nJ++)
	{
		str.Format("����%d", nJ+1); 
		sArrTabRow.Add(str);
	}
	
	long i = 0; for(i = 0; i < sArrTabRow.GetSize(); i++)
	m_TabRow.InsertItem(i, sArrTabRow[i]);

	if(m_TabRow.GetItemCount() <= m_nTabRow) m_nTabRow = 0;
	m_TabRow.SetCurSel(m_nTabRow);
	
	nTab				= 0;
	m_nTabUpperGirder	= -1;
	m_nTabLowerGirder	= -1;
	m_nTabFooting		= -1;
	m_nTabColumn		= -1;
	m_nTabWall			= -1;
	m_nTabJongGirder	= -1;
	m_sArrTabName.RemoveAll();
	
	m_sArrTabName.Add("��ΰŴ�"); m_nTabUpperGirder = nTab++;
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		m_sArrTabName.Add("�ϺΰŴ�"); m_nTabLowerGirder = nTab++; 
	}

	m_sArrTabName.Add("���"); m_nTabColumn = nTab++;

	if(pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
	{
		m_sArrTabName.Add("����"); m_nTabFooting = nTab++;
	}
	m_sArrTabName.Add("��ü"); m_nTabWall	= nTab++;
	m_sArrTabName.Add("���Ŵ�"); m_nTabJongGirder	= nTab++;
		
	m_Tab.DeleteAllItems();
	for(i = 0; i < m_sArrTabName.GetSize(); i++)
		m_Tab.InsertItem(i, m_sArrTabName[i]);

	if(m_Tab.GetItemCount() <= m_nTab) m_nTab = 0;
	m_Tab.SetCurSel(m_nTab);

	ReInitTabRowCtrl();
}

void CInputRebarColumn::OnPreInitDialog()
{
	// �����ϸ鼭 pick box�� ũ�⸦ ������
	if(m_pView) 
	{
		CTwinVector tv;
		tv.SetPickBoxSize(m_pView->GetSelectionRectSize()/m_pView->GetRatioWindow());
	}
	
	m_nViewCross	= 0;
	m_bViewPM = FALSE;
	InitTabCtrl();
	InitComboCtrl();


	SetResize(IDC_BUTTON_EXAMINE,		SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
	SetResize(IDC_TAB,					SZ_BOTTOM_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_TAB_ROW,				SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID,					SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);
	SetResize(IDC_COMBO_INPUT_COLUMN,			SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_CTC_GIRDER,	SZ_TOP_LEFT,		SZ_TOP_LEFT);
	SetResize(IDC_EDIT_CTC_GIRDER,		SZ_TOP_LEFT,		SZ_TOP_LEFT);
	SetResize(IDC_CHECK_MATCH_WITHWALL,	SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_VIEW_PM,		SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_RADIO_CROSS,			SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_RADIO_PLANE,			SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_VIEW,			SZ_TOP_RIGHT,		SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_RAD_CROSSTIE,	SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
	

	SetHyperHelp(m_pIn->GetManualPath());

	m_Grid.InitGrid(m_pIn->m_pARcBridgeDataStd->m_Fy);

	// ó�� ������ �Ŵ��� �ִ� ù��° ������ ������ �д�. ///////////////////
	if(m_nTab == 0 && m_nTabRow == 0)
	{
		long nFindJijum	= -1;
		long nFindPart	= -1;
		CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
		long nJ = 0; for(nJ = 0; nJ <= pBri->m_nQtyJigan; nJ++)
		{
			if(pBri->IsGirderRebar(nJ, TRUE))
			{
				nFindJijum	= nJ;
				nFindPart	= 0;
				break;
			}
		}
		// �Ŵ��� ������ ��� �ִ°� ù���粨 ����
		if(nFindJijum == -1)
		{
			for(nJ = 0; nJ <= pBri->m_nQtyJigan; nJ++)
			{
				if(nJ == 0 || nJ == pBri->m_nQtyJigan) continue;

				CWallApp *pWall	= pBri->GetInWall(nJ-1);
				if(pWall)
				{
					if(pWall->m_bIs && (pWall->m_bTopIsColumn || pWall->IsRoundRectWall()))
					{
						nFindJijum	= nJ;
						nFindPart	= 1;
						break;
					}
				}
			}
		}

		// ��յ� ������ ���� ù��°�� ����
		if(nFindJijum == -1)
		{
			for(nJ = 0; nJ <= pBri->m_nQtyJigan; nJ++)
			{
				CFootingApp *pFooting = pBri->GetFootingByJijumNumber(nJ);
				if(pFooting)
				{
					if(pFooting->m_bIs)
					{
						nFindJijum = nJ;
						nFindPart	= 2;
						break;
					}
				}
			}
		}

		if(nFindJijum > -1)
		{
			m_TabRow.SetCurSel(nFindJijum);
			m_nTabRow	= nFindJijum;
			m_Tab.SetCurSel(nFindPart);
			m_nTab		= nFindPart;
		}
	}
	///////////////////////////////////////////////////////////////////////

	SetGridData();
	SetControl();
}

BOOL CInputRebarColumn::PreTranslateMessage(MSG* pMsg) 
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
	
	return CInputDialogBase::PreTranslateMessage(pMsg);
}

void CInputRebarColumn::OnSelchangeComboBridgeNo() 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	m_pIn->m_nCurrentBridge = m_comboBridgeNo.GetCurSel();
	InitTabCtrl();
	SetControl();
	if(SetGridDataStd())
		SetDataInit();
	SetPaneTitle();
	DrawInputDomyunView(TRUE);
	m_comboBridgeNo.SetFocus();
}

void CInputRebarColumn::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)  
{
	// TODO: Add your control notification handler code here
	SetFocus();
	m_nTab	= m_Tab.GetCurSel();

	ReInitTabRowCtrl();

	if(SetGridDataStd())
		SetDataInit();
	else
		m_btnExamine.EnableWindow(FALSE);

	SetControl();
	m_Grid.SetSelectedRange(1, 1, 1, 1);
	DrawInputDomyunView(TRUE);


	*pResult = 0;
}

void CInputRebarColumn::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CInputRebarColumn::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	structGridCell *pCell = m_Grid.GetCurrentCell();
	if(pCell)
	{
		//DrawInputDomyunView(FALSE);

		m_pIn->SetComment(pCell->sTitle);
		
		CString str	= m_Grid.GetItemText(1, pCell->nCol);
		if(str == "B" || str == "C") str		= "B@C";
		else if(str == "aB" || str == "aC") str	= "aB@aC";
		else if(str == "E" || str == "F") str	= "E@F";

		if(m_nTab == m_nTabWall)
		{
			str	= "������� ���·� ��ٵǴ� ��±� ����";
		}

		m_pView->GetDomyun()->STMakeCursor(str);
	}

	if(GetIdxInputStirrup() != -1)
	{
		DrawInputDomyunView(FALSE);		
	}

}

void CInputRebarColumn::SetControl()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	if(m_nTab == m_nTabColumn) 
	{
		BOOL bColumn = (pBri->GetTypeColumnByJijum(m_nTabRow)==-1) ? FALSE : TRUE;
		m_comboInputColumn.EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CTC_GIRDER)->EnableWindow(FALSE);
		m_checkMatchWidthWall.EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_VIEW_PM)->EnableWindow(bColumn);

		GetDlgItem(IDC_RADIO_CROSS)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PLANE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_RAD_CROSSTIE)->EnableWindow(TRUE);
	}
	else if(m_nTab == m_nTabUpperGirder || m_nTab == m_nTabLowerGirder)
	{
		m_comboInputColumn.EnableWindow(FALSE);

		long nJ	= m_nTabRow;
		if(pBri->IsGirderRebar(nJ, m_nTab == m_nTabUpperGirder))
			GetDlgItem(IDC_EDIT_CTC_GIRDER)->EnableWindow(TRUE);
		else
			GetDlgItem(IDC_EDIT_CTC_GIRDER)->EnableWindow(FALSE);
		m_checkMatchWidthWall.EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_VIEW_PM)->EnableWindow(FALSE);

		GetDlgItem(IDC_RADIO_CROSS)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PLANE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_RAD_CROSSTIE)->EnableWindow(FALSE);
	}
	else if(m_nTab == m_nTabFooting)
	{
		m_comboInputColumn.EnableWindow(FALSE);
		
		GetDlgItem(IDC_EDIT_CTC_GIRDER)->EnableWindow(FALSE);
		m_checkMatchWidthWall.EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_VIEW_PM)->EnableWindow(FALSE);

		GetDlgItem(IDC_RADIO_CROSS)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PLANE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_RAD_CROSSTIE)->EnableWindow(FALSE);
	}
	else if(m_nTab == m_nTabWall)
	{
		BOOL bInWall = (m_nTabRow==0 || m_nTabRow==pBri->m_nQtyJigan)? FALSE : TRUE;
		m_comboInputColumn.EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CTC_GIRDER)->EnableWindow(FALSE);
		m_checkMatchWidthWall.EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_VIEW_PM)->EnableWindow(bInWall);

		GetDlgItem(IDC_RADIO_CROSS)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PLANE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_RAD_CROSSTIE)->EnableWindow(FALSE);
	}
	else if(m_nTab == m_nTabJongGirder)
	{
		m_comboInputColumn.EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_CTC_GIRDER)->EnableWindow(TRUE);
		m_checkMatchWidthWall.EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_VIEW_PM)->EnableWindow(FALSE);

		GetDlgItem(IDC_RADIO_CROSS)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_PLANE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_RAD_CROSSTIE)->EnableWindow(FALSE);
	}
	else
	{
		m_comboInputColumn.EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_CTC_GIRDER)->EnableWindow(FALSE);
		m_checkMatchWidthWall.EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_VIEW_PM)->EnableWindow(FALSE);

		GetDlgItem(IDC_RADIO_CROSS)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PLANE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_RAD_CROSSTIE)->EnableWindow(FALSE);
	}

//	m_btnExamine.EnableWindow(FALSE);

}

void CInputRebarColumn::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	CCellID cell		= m_Grid.GetFocusCell();
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();

	//#0029089
	if(m_nTab == m_nTabFooting) 
	{
		CFootingApp *pFooting = pBri->GetFootingByJijumNumber(m_nTabRow);

		if(pFooting)
		{
			if(cell.row == 3 && cell.col == 8)
			{
				double dCtC = pFooting->m_dCTC_MainRebar * pFooting->m_nCountWrapMainRebar / 2;

				double dValue = m_Grid.GetTextMatrixDouble(cell.row, cell.col);

				if(dValue != dCtC)
				{
					CString strInfo;

					strInfo.Format("����ö�� ���� ����(%0.f)�� ��ġ���� �ʽ��ϴ�.\n �̴�� �����Ͻðڽ��ϴ�.", dCtC);

					if(IDYES != AfxMessageBox(strInfo, MB_YESNO))
					{
						m_Grid.UpdateData(FALSE);
						return;
					}
				}
			}
		}	
	}

	SetDataSave();

	CString sTitle2	= m_Grid.GetTextMatrix(1, cell.col);
	if((m_nTab == m_nTabLowerGirder || m_nTab == m_nTabUpperGirder) && sTitle2 == "����")
	{
		long nJ	= m_nTabRow;
		for(long s = 0; s < 2; s++)
		{
			pBri->SetDataDefaultRebarMainInGirder_Girder(nJ, TRUE, TRUE, TRUE, FALSE, m_nTab == 0);
			pBri->SetDataDefaultRebarGirder(nJ, TRUE, FALSE, TRUE, TRUE, TRUE, FALSE, FALSE, m_nTab == 0);
		}
		if(SetGridDataStd())
			SetDataInit();
	}
	if(m_nTab == m_nTabColumn)
	{
		if(SetGridDataStd())
			SetDataInit();
	}
	if(sTitle2 == "����")
	{
		if(SetGridDataStd())
			SetDataInit();
	}

	if(sTitle2 == "\n��ġ\n")
	{
		// ���Ŵ�
		CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
		if(pBri)
		{
			BOOL bLeft	= TRUE;
			BOOL bStt	= TRUE;
			BOOL bGagak	= TRUE;
			BOOL bJongGirder	= TRUE;
			GetInfoGirder(bStt, bLeft, bGagak, bJongGirder);
			if(bJongGirder)
			{
				pBri->SetDataDefaultRebarMainInGirder_Girder(0, bStt, bLeft, bGagak, bJongGirder);
				pBri->SetDataDefaultRebarGirder(0, TRUE, TRUE, bStt, bLeft, bGagak, bJongGirder);
			}

			if(SetGridDataStd())
				SetDataInit();
		}
	}


	//DrawInputDomyunView(FALSE);
	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);
}

void CInputRebarColumn::SetDataInit()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	// ��ս��� �ƴ� ��� �Է� ���� ���� ����.
	CWallApp *pWall	= GetCurInWall();
	m_btnExamine.EnableWindow(TRUE);

	if(m_nTab == m_nTabUpperGirder || m_nTab == m_nTabLowerGirder)
	{
		long nJ	= m_nTabRow;
		if(!pBri->IsGirderRebar(nJ, m_nTab == m_nTabUpperGirder)) 
		{
			m_pIn->GridMessage(&m_Grid, "�����Ͻ� ������ �Ŵ�ö���� ���� ���� �ʽ��ϴ�.");
			m_btnExamine.EnableWindow(FALSE);
			return;
		}
	}
	else if(m_nTab == m_nTabColumn)
	{
		if(!pWall)
		{
			m_pIn->GridMessage(&m_Grid, "�����Ͻ� ������ ��ü�� ���� ���� �ʽ��ϴ�");
			m_btnExamine.EnableWindow(FALSE);
			return;
		}
		if(!pWall->m_bTopIsColumn && !pWall->IsRoundRectWall() && m_nTab != m_nTabFooting)
		{
			m_pIn->GridMessage(&m_Grid, "�����Ͻ� ��ü�� ��ս��� �ƴմϴ�");
			m_btnExamine.EnableWindow(FALSE);
			return;
		}

		m_checkRadCrossTie.SetCheck(pWall->m_Column.m_nTypeRebarCrossTie == 0 ? FALSE : TRUE);
		
	}
	else if(m_nTab == m_nTabFooting)
	{
		BOOL bIsFooting = FALSE;
		m_btnExamine.EnableWindow(FALSE);
		CFootingApp *pFooting = pBri->GetFootingByJijumNumber(m_nTabRow);
		if(pFooting)
			bIsFooting = pFooting->m_bIs;
		if(!bIsFooting)
		{
			m_pIn->GridMessage(&m_Grid, "�����Ͻ� ������ ���ʰ� ���� ���� �ʽ��ϴ�");
			return;
		}
	}
	else if(m_nTab == m_nTabWall)
	{
		BOOL bIsWall	= FALSE;
		BOOL bInWall(FALSE);
		long nJ	= m_nTabRow;
		if(nJ == 0)
			bIsWall	= pBri->IsOutWall(TRUE) ? TRUE : FALSE;
		else if(nJ == pBri->m_nQtyJigan)
			bIsWall	= pBri->IsOutWall(FALSE) ? TRUE : FALSE;
		else
		{
			long nJijumConnect = pBri->GetJijumConnectRibAndSlab();
			CWallApp *pWall	= pBri->GetInWall(nJ - 1);
			if(pWall && (nJ != nJijumConnect))
				bIsWall	= pWall->m_bIs;

			bInWall = TRUE;
		}

		if(!bIsWall)
		{
			m_pIn->GridMessage(&m_Grid, "�����Ͻ� ������ ��ü�� ���� ���� �ʽ��ϴ�");
			m_btnExamine.EnableWindow(FALSE);
			return;
		}
		else if((bInWall && pBri->m_nSelectInWallSectionDesign == 0) || (bInWall == FALSE && pBri->m_nSelectWallSectionDesign == 0) || (bInWall == FALSE && pBri->m_pARcBridgeDataStd->IsLsdDesign() == FALSE))
		{
			// �ں���� ���� �Ҷ��� ���⼭ ������ �ȵȴ�.
			// (ARCBRIDGE-3620) ����������϶��� ��ö�� �Է¿��� �����Ѵ�. - ������ü�̰� �ھ�������϶�. (ARCBRIDGE-3654) ������ü�϶��� ���⼭ ������並 ����Ѵ�.
			m_btnExamine.EnableWindow(FALSE);
		}
	}


	m_Grid.UpdateData(FALSE);

	long nRows(0), nCols(0);
	m_Grid.GetCellCount(nRows, nCols);
	if(m_nTab == m_nTabWall)
	{
		m_Grid.MergeGrid(0, nRows-1, 0, 1);
	}
	else
	{
		m_Grid.MergeGrid(0, 0, 1, nCols);
		m_Grid.MergeGrid(0, 1, 0, 0);
	}
}


void CInputRebarColumn::DrawInputDomyunView(BOOL bZoomAll)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);

	BOOL bColumn = m_nTab == m_nTabColumn ? ((pBri->GetTypeColumnByJijum(m_nTabRow)==-1) ? FALSE : TRUE) : FALSE;
	BOOL bInWall = m_nTab == m_nTabWall ? ((m_nTabRow==0 || m_nTabRow==pBri->m_nQtyJigan)? FALSE : TRUE) : FALSE;
	if(m_nTabRow == pBri->GetJijumConnectRibAndSlab()) bColumn = bInWall = FALSE;
	
	if(m_bViewPM && ((m_nTab == m_nTabColumn && bColumn) || (m_nTab == m_nTabWall && bInWall)))
	{
		BOOL bFemNormal = pBri->m_FemNormal.IsOutputLoaded();
		BOOL bFemEQ     = pBri->IsRcSlab() ? TRUE : (pBri->GetFemManage(COMBO_EARTHQUAKE)->IsOutputLoaded());
		if(!bFemNormal || (pBri->GetTypeDesingEQ() == 1 && !bFemEQ))
		{
			AfxMessageBox("�𵨸� �ؼ��� �����Ŀ� Ȯ���Ͻñ� �ٶ��ϴ�.");
		}
		else
		{
			DrawPM(pDom);
			CInputDialogBase::DrawInputDomyunView(bZoomAll);
			return;
		}
	}

	pDom->SetScaleDim(m_pIn->GetScaleInputDialog());

	CARcBridgeDBStandard DBStd(m_pIn->GetRcBridgeRebar(), m_pIn->m_pARoadOptionStd, m_pIn->m_pARcBridgeDataStd);

	// ���� ������ �⺻������
	pBri->m_footingEnd.m_exFooting.m_Pile.InitScaleCutting();

	// ��� ��ü���� ����ȭ�� ����.
	BOOL bVertDir	= FALSE;
	if(m_nTab == m_nTabUpperGirder || m_nTab == m_nTabLowerGirder)	// �Ŵ��� �׻� ��Ÿ��� �Է¹���
		bVertDir	= FALSE;
	else if(m_nTab == m_nTabFooting)	// ���ʴ� �׻� ���Ÿ��� �Է¹����� ��ö��/��±�/����ö�� ���� �Է� ������ ��Ÿ���.
		bVertDir	= TRUE;
	pBri->SyncBridge(0, !bVertDir);
	if(m_nTab != m_nTabJongGirder)
	{
		pBri->SyncBridge_CrossSection_SttEndFooting(TRUE);
		//pBri->GetTvVertSection_Std(0, !bVertDir, FALSE);

		pBri->SyncBridgeRebar(TRUE, bVertDir ? -1 : 1);
	}
	
	long nJ			= m_nTabRow;
	long nIdxWall	= nJ-1;
	CWallApp *pWall		= pBri->GetInWall(nIdxWall);
//	BOOL bLeft		= TRUE;
	if(m_nTab == m_nTabColumn)
	{
		if(!pWall) 
		{
			CInputDialogBase::DrawInputDomyunView(bZoomAll);
			return;
		}
	}
	if(m_nTab == m_nTabUpperGirder || m_nTab == m_nTabLowerGirder)
	{
		if(!pBri->IsGirderRebar(nJ,m_nTab == m_nTabUpperGirder)) 
		{
			CInputDialogBase::DrawInputDomyunView(bZoomAll);
			return;
		}
	}
	if(m_nTab == m_nTabWall)
	{
// 		BOOL bOK	= FALSE;
// 		if(nJ == 0)
// 			bOK	= pBri->IsOutWall(TRUE);
// 		else if(nJ == pBri->m_nQtyJigan)
// 			bOK	= pBri->IsOutWall(FALSE);
// 		else
// 		{
// 			if(pWall)
// 			{
// 				bOK	= pWall->m_bIs && !pWall->m_bTopIsColumn;
// 			}
// 		}
	}
	if(m_nTab == m_nTabJongGirder)
	{
// 		bLeft	= m_nTabRow == iLEFT;
	}
	
	long nCurDan		= m_Grid.GetFocusCell().row-2;
	if(nCurDan < 0) nCurDan	= 0;

	if(m_nTab == m_nTabUpperGirder || m_nTab == m_nTabLowerGirder)
	{
		CDomyun Dom(pDom);

		// ���ܸ� �Ŵ��� �� �׸���
		DBStd.m_pDrawVertSection->DrawDetail_Girder(&Dom, nJ, TRUE, TRUE, TRUE, FALSE, m_nTab == m_nTabUpperGirder);

		// ��Ʈ�� ö�� ���� �׸���
		long nIdxInputStirrup	= GetIdxInputStirrup();
		DBStd.m_pDrawVertSection->DrawRebarAll_SlabGirder(&Dom, nJ, TRUE, TRUE, nCurDan, TRUE, FALSE, TRUE, TRUE, TRUE, FALSE, nIdxInputStirrup, FALSE, m_nTab == m_nTabUpperGirder);

		if(pBri->IsBoxType() == FALSE)
		{
			// ������ �� ��ġ ǥ��(�ﰢ��)
			DBStd.m_pDrawVertSection->DrawJijumSign(&Dom, nJ);
		}

		*pDom << Dom;
	}
	// ���
	else if(m_nTab == m_nTabColumn)
	{
		if(pWall->m_bTopIsColumn || pWall->IsRoundRectWall())
		{
			CColumnApp *pColumn	= &pWall->m_Column;

			// ��� ///////////////////////////////////////////////////////////////
			CDomyun domPlane(pDom);
			
			// ��� ��� �׸���
			BOOL bCrossTie = pWall->m_bTopIsColumn == TRUE && m_comboInputColumn.GetCurSel() == 1;
			BOOL bMainRebar = m_comboInputColumn.GetCurSel() == 0;
			BOOL bRotateVertDim = pWall->IsRoundRectWall();

			DBStd.m_pDrawColumn->DrawRebar_PlaneStd(&domPlane, pWall, bMainRebar, bCrossTie, TRUE, TRUE, bCrossTie, 
				bRotateVertDim);
	
			domPlane.SetCalcExtRect();
			CDRect rectPlane = domPlane.GetExtRect();
			CDPoint xyPlane = CDPoint(rectPlane.right, rectPlane.bottom-rectPlane.Height()/2);

			if(pWall->IsRoundRectWall())
			{
				domPlane.Rotate(rectPlane.CenterPoint(), CDPoint(0, -1));
				domPlane.SetCalcExtRect();
				rectPlane = domPlane.GetExtRect();

				CString strMsg = "* ���� ��ü�� �Էµ� ��ö�� ���̿� 2Cycle�� �ڵ����� ��ٵ˴ϴ�";
				CDPoint xyText	= CDPoint(rectPlane.CenterPoint().x, rectPlane.top - domPlane.Always(30));

				m_pIn->m_pARoadOptionStd->SetEnvType(&domPlane, HCAD_SMALL_TITLE);
				domPlane.SetTextAlignHorz(TA_CENTER);
				domPlane.TextOut(xyText, strMsg);
			}

			*pDom << domPlane;

			// ���� �� ///////////////////////////////////////////////////////
			if(!pWall->IsRoundRectWall())
			{
				CDomyun domFront(pDom);

				DBStd.m_pDrawVertSection->DrawDetail_Column_VertSection(&domFront, nIdxWall);
				
				long dan = 0; for(dan = 0; dan < 3; dan++)
				{
					BOOL bDim	= TRUE;
					DBStd.m_pDrawColumn->DrawRebarMain_Front(&domFront, &pWall->m_Column, FALSE, dan, FALSE, TRUE, TRUE);
					DBStd.m_pDrawColumn->DrawRebarRound_Front(&domFront, &pWall->m_Column, nJ, FALSE, dan, bDim, TRUE, TRUE, FALSE);
				}
				DBStd.m_pDrawColumn->DimRebarCrossTie_Front(&domFront, &pWall->m_Column, nJ, pColumn->GetCountDan_RebarMain()+1, FALSE, TRUE, TRUE);

				domFront.SetCalcExtRect();
				CDRect rectFront	= domFront.GetExtRect();
				CDPoint xyFront		= CDPoint(rectFront.left, rectFront.bottom-rectFront.Height()/2);
			
				domFront.Move(CDPoint(xyPlane.x-xyFront.x + 4000, xyPlane.y-xyFront.y));

				*pDom << domFront;
			}
		}
	}
	else if(m_nTab == m_nTabFooting)
	{
		CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(nJ);
		if(pFooting)
		{
			CDomyun Dom(pDom);

			if(pWall)
			{
				if(pWall->m_bTopIsColumn)
				{
					DBStd.m_pDrawWall->DrawLeftSide(&Dom, pWall, FALSE, FALSE, TRUE, 0, TRUE);
				}
			}

			CDRect rectSide	= DBStd.m_pDrawFooting->DrawRebar_SideStd(&Dom, pFooting, TRUE, TRUE, TRUE);
			*pDom << Dom;

			CDomyun domPlane(pDom);
#ifdef _DEBUG
			CDRect rectPlane	= DBStd.m_pDrawFooting->DrawRebar_PlaneStd(&domPlane, pFooting, TRUE, FALSE, FALSE, TRUE, nJ==0 ? 1 : -1, FALSE, TRUE);
			domPlane.Move(rectSide.left-rectPlane.left, rectSide.bottom-rectPlane.top+Dom.Always(130));
#else
			CDRect rectPlane	= DBStd.m_pDrawFooting->DrawRebar_PlaneStd(&domPlane, pFooting, TRUE, FALSE, FALSE, TRUE, nJ==0 ? 1 : -1, FALSE, TRUE);
			domPlane.Move(rectSide.left-rectPlane.left, rectSide.bottom-rectPlane.top+Dom.Always(130));
#endif
			
			
			// Ÿ��Ʋ
			domPlane.SetCalcExtRect();
			rectPlane	= domPlane.GetExtRect();
			m_pIn->m_pARoadOptionStd->SetEnvType(&domPlane, HCAD_SMALL_TITLE);
			domPlane.SetTextAlignHorz(TA_CENTER);
			domPlane.SetTextHeight(5);
			domPlane.TextOut(rectPlane.left+rectPlane.Width()/2, rectPlane.bottom+Dom.Always(20), "���� ��鵵");

			m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SMALL_TITLE);
			Dom.SetTextAlignHorz(TA_CENTER);
			Dom.SetTextHeight(5);
			Dom.TextOut(rectSide.left+rectSide.Width()/2, rectSide.bottom+Dom.Always(20), "���� ���鵵");


			*pDom << Dom;
			*pDom << domPlane;
		}
	}
	else if(m_nTab == m_nTabWall && (nJ==0 || nJ==pBri->m_nQtyJigan || bInWall))
	{
		CDomyun Dom(pDom);

		CRebarPlacing rb;
//		double dSta	= pBri->m_dStationBridgeStt;
		double dSta = pBri->GetStationByTarget((nJ==0) ? BRIDGE_OFF_STT_SLAB: BRIDGE_OFF_END_SLAB, 0, TRUE);
		//double dMid	= 0;
		double dRange	= 0;
		BOOL bWall	= TRUE;
		double dDiffTop = 0;

		if(nJ == 0)
		{
			CWallApp wall;
			pBri->MakeWallByOutWall(wall, TRUE, TRUE);

			DBStd.m_pDrawCrossSection->DrawRebar_OutWall_CrossSectionStd(&Dom, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, FALSE, TRUE);
			// �׸��°� �����ε� Station�� ������.
			// dSta	= wall.m_xyOrg.x;
			dSta = pBri->GetStationByTarget(BRIDGE_OFF_STT_STT, 0, FALSE);
// 			double dMid	= wall.m_dCenterOfWall;
			dRange	= wall.m_dRangeSupportRebarForSameUpperSlab;

			dDiffTop = wall.m_dDiffTop[1];
		}
		else if(nJ == pBri->m_nQtyJigan)
		{
			CWallApp wall;
			pBri->MakeWallByOutWall(wall, FALSE, TRUE);

			DBStd.m_pDrawCrossSection->DrawRebar_OutWall_CrossSectionStd(&Dom, FALSE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, FALSE, TRUE);

			//dSta	= wall.m_xyOrg.x;
			dSta = pBri->GetStationByTarget(BRIDGE_OFF_END_END, 0, FALSE);
//			dMid	= wall.m_dCenterOfWall;
			dRange	= wall.m_dRangeSupportRebarForSameUpperSlab;
//			rb.Mirror(tvArrUpperSlabRebar, CDPoint(dMid, 0), TRUE);

			dDiffTop = wall.m_dDiffTop[0];
		}
		else
		{
			CWallApp *pWall	= pBri->GetInWall(nJ-1);
			if(!pWall || pWall->m_bTopIsColumn)
				bWall	= FALSE;
			if(bWall)
			{

				DBStd.m_pDrawCrossSection->DrawRebar_InWall_SideStd(&Dom, nJ, TRUE, TRUE, TRUE, TRUE, "������", "������", TRUE, FALSE, TRUE, FALSE, TRUE);

				dSta	= pWall->m_xyOrg.x;
				if(pBri->IsBoxType())
				{
					// BoxŸ���϶��� Station�� ����������� �̵�
					CDPoint vAng	= ToDPointFrDegree(pWall->GetAngleWallByHor(TRUE));
					dSta -= pWall->m_dW/vAng.y/2.;
				}
// 				dMid	= pWall->m_dCenterOfWall;
				dRange	= pWall->m_dRangeSupportRebarForSameUpperSlab;
//				rb.Mirror(tvArrUpperSlabRebar, CDPoint(dMid, 0), TRUE);
			}
			dDiffTop = pWall->m_dDiffTop[0];
		}

		CTwinVectorArray tvArrUpperSlabRebar, tvArrTmp;
		pBri->GetTvRebarSupport_CrossSection_UpperSlab(tvArrUpperSlabRebar, tvArrTmp, tvArrTmp, dSta, TRUE, 0, 0, TRUE, FALSE, TRUE, 1);

		// �Է³���Ȯ���� ���� ǥ��
		if(bWall)
		{
			// Ⱦ�ܸ��� Wall�� m_dDiffTop ��ŭ ������ �׸��� ������ �̰͵� ��������Ѵ�.
			tvArrUpperSlabRebar.Move(CDPoint(0, -dDiffTop));

			CHgBaseDrawStd baseDraw;
			m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
			baseDraw.DrawTvArray(&Dom, tvArrUpperSlabRebar);

			// �Է³��� dim ǥ��
			m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
			CDRect rect	= tvArrUpperSlabRebar.GetRect();
			CTwinVector tvU, tvL;
			tvU.m_v1	= CDPoint(rect.left, rect.bottom);
			tvU.m_v2	= CDPoint(rect.right, rect.bottom);
			tvL			= tvU;
			tvL.m_v1.y	-= dRange;
			tvL.m_v2.y	-= dRange;

			Dom.SetDirection("RIGHT");
			Dom.DimMoveTo(tvL.m_v2.x, tvL.m_v2.y, 0);
			Dom.DimLineToExtend(tvU.m_v2.y - tvL.m_v2.y, 0, "������� ���·� ��ٵǴ� ��±� ����", COMMA(dRange));

			m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SYMB);
			m_pIn->m_pARoadOptionStd->SetEnvLType(&Dom, LT_HIDDEN_LINE);
			baseDraw.DrawTv(&Dom, tvU);
			baseDraw.DrawTv(&Dom, tvL);
			
			*pDom << Dom;
		}
	}
	else if(m_nTab == m_nTabJongGirder)
	{
		CDomyun Dom(pDom);
		
		BOOL bGagak	= TRUE;
		BOOL bStt	= TRUE;
		BOOL bLeft	= TRUE;
		BOOL bJongGirder	= FALSE;
		GetInfoGirder(bStt, bLeft, bGagak, bJongGirder);


		if(m_nViewCross == 0)
		{
			// ���ܸ� �Ŵ��� �� �׸���
			DBStd.m_pDrawVertSection->DrawDetail_Girder(&Dom, 0, bStt, bLeft, bGagak, bJongGirder);

			// ��Ʈ�� ö�� ���� �׸���
			long nIdxInputStirrup	= GetIdxInputStirrup();
			DBStd.m_pDrawVertSection->DrawRebarAll_SlabGirder(&Dom, 0, TRUE, TRUE, nCurDan, TRUE, FALSE, bStt, bLeft, bGagak, bJongGirder, nIdxInputStirrup);
		}
		else
		{
			DBStd.m_pDrawPlane->DrawPlaneGen(&Dom, 2000, TRUE, TRUE, TRUE, FALSE, FALSE);
			DBStd.m_pDrawPlane->DrawRebarSupport_Plane_UpperSlabGirderJong(&Dom, bStt, bLeft, bGagak, TRUE, TRUE, TRUE, TRUE);
			DBStd.m_pDrawPlane->DrawRebarStirrup_Plane_UpperSlabGirderJong(&Dom, bStt, bLeft, bGagak, TRUE, TRUE, TRUE);
		}


		
		*pDom << Dom;
	}
		
	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

void CInputRebarColumn::InitComboCtrl()
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

	m_comboInputColumn.SetCurSel(0);

	SetControl();
}

// ��� �Ŵ� �Է�
BOOL CInputRebarColumn::SetGridDataUpperGirder(BOOL bUpperSlabGirder)
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 2, 1);
	m_Grid.SetCellSize(20, 50);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return FALSE;

	long row		= 0, col = 0;
	CString str		= _T("");
	CString sHelp	= _T("");
	CString sUpdn	= _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;
	CStringArray sArr;


	// ���� �������� ��Ʈ��
	long nJ	= m_nTabRow;
	if(!pBri->IsGirderRebar(nJ, m_nTab == m_nTabUpperGirder)) 
	{
		m_pIn->GridMessage(&m_Grid, "�Ŵ��� ���� ���� �ʽ��ϴ�");
		return FALSE;
	}

	CGirderSectionRebar *pGirderRB	= &pBri->m_SlabGirderRebar[bUpperSlabGirder ? iUPPER : iLOWER][nJ];

	// Ÿ��Ʋ
	m_Grid.AddCell(row, col, "����\n");
	m_Grid.AddCell(row+1, col++, "����\n");
	
	if(nJ == 0 || nJ == pBri->GetCountJijum()-1)
	{
		m_Grid.AddCell(row, col, "��ġ");
		m_Grid.AddCell(row+1, col++, "D");
	}

	//��ν������϶��ϰ� �Ϻ꽽�����϶��ϰ� ���ϸ� ������ �ٲ�.
	if(bUpperSlabGirder)
	{
		m_Grid.AddCell(row, col, "�Ŵ�(���)");
		m_Grid.AddCell(row+1, col++, "dc");
		m_Grid.AddCell(row, col, "�Ŵ�(���)");
		m_Grid.AddCell(row+1, col++, "GU");
		m_Grid.AddCell(row, col, "�Ŵ�(���)");
		m_Grid.AddCell(row+1, col++, "C1");
		m_Grid.AddCell(row, col, "�Ŵ�(���)");
		m_Grid.AddCell(row+1, col++, "B");
		m_Grid.AddCell(row, col, "�Ŵ�(���)");
		m_Grid.AddCell(row+1, col++, "C");
		m_Grid.AddCell(row, col, "�Ŵ�(���)");
		m_Grid.AddCell(row+1, col++, "C2");

		m_Grid.AddCell(row, col, "�Ŵ�(�ϸ�)");
		m_Grid.AddCell(row+1, col++, "dc'");
		m_Grid.AddCell(row, col, "�Ŵ�(�ϸ�)");
		m_Grid.AddCell(row+1, col++, "GL");
		m_Grid.AddCell(row, col, "�Ŵ�(�ϸ�)");
		m_Grid.AddCell(row+1, col++, "aC1");
		m_Grid.AddCell(row, col, "�Ŵ�(�ϸ�)");
		m_Grid.AddCell(row+1, col++, "aB");
		m_Grid.AddCell(row, col, "�Ŵ�(�ϸ�)");
		m_Grid.AddCell(row+1, col++, "aC");
		m_Grid.AddCell(row, col, "�Ŵ�(�ϸ�)");
		m_Grid.AddCell(row+1, col++, "aC2");
	}
	else
	{
		m_Grid.AddCell(row, col, "�Ŵ�(���)");
		m_Grid.AddCell(row+1, col++, "dc'");
		m_Grid.AddCell(row, col, "�Ŵ�(���)");
		m_Grid.AddCell(row+1, col++, "GU");
		m_Grid.AddCell(row, col, "�Ŵ�(���)");
		m_Grid.AddCell(row+1, col++, "aC1");
		m_Grid.AddCell(row, col, "�Ŵ�(���)");
		m_Grid.AddCell(row+1, col++, "aB");
		m_Grid.AddCell(row, col, "�Ŵ�(���)");
		m_Grid.AddCell(row+1, col++, "aC");
		m_Grid.AddCell(row, col, "�Ŵ�(���)");
		m_Grid.AddCell(row+1, col++, "aC2");

		m_Grid.AddCell(row, col, "�Ŵ�(�ϸ�)");
		m_Grid.AddCell(row+1, col++, "dc");
		m_Grid.AddCell(row, col, "�Ŵ�(�ϸ�)");
		m_Grid.AddCell(row+1, col++, "GL");
		m_Grid.AddCell(row, col, "�Ŵ�(�ϸ�)");
		m_Grid.AddCell(row+1, col++, "C1");
		m_Grid.AddCell(row, col, "�Ŵ�(�ϸ�)");
		m_Grid.AddCell(row+1, col++, "B");
		m_Grid.AddCell(row, col, "�Ŵ�(�ϸ�)");
		m_Grid.AddCell(row+1, col++, "C");
		m_Grid.AddCell(row, col, "�Ŵ�(�ϸ�)");
		m_Grid.AddCell(row+1, col++, "C2");
	}
	

	m_Grid.AddCell(row, col, "�Ŵ�(����)");
	m_Grid.AddCell(row+1, col++, "GS");
	m_Grid.AddCell(row, col, "�Ŵ�(����)");
	m_Grid.AddCell(row+1, col++, "F1");
	m_Grid.AddCell(row, col, "�Ŵ�(����)");
	m_Grid.AddCell(row+1, col++, "E");
	m_Grid.AddCell(row, col, "�Ŵ�(����)");
	m_Grid.AddCell(row+1, col++, "F");
	m_Grid.AddCell(row, col, "�Ŵ�(����)");
	m_Grid.AddCell(row+1, col++, "F2");

	m_Grid.AddCell(row, col, "���ͷ�");
	m_Grid.AddCell(row+1, col++, "GT");

	m_Grid.AddCell(row, col, "���ͷ�");
	m_Grid.AddCell(row+1, col++, "����");
	for(long strup = 0; strup < pGirderRB->m_nCountStirrup; strup++)
	{
		if(strup > 0)
		{
			str.Format("A%d", strup+1);
			m_Grid.AddCell(row, col, "���ͷ�");
			m_Grid.AddCell(row+1, col++, str);
		}

		str.Format("B%d", strup+1);
		m_Grid.AddCell(row, col, "���ͷ�");
		m_Grid.AddCell(row+1, col++, str);
	}
	row++;

	BOOL bUpper	= TRUE;

	long dan = 0; for(dan = 0; dan < 1; dan++)
	{
		row	++;
		col	= 0;
		str.Format("%d��", dan+1);
		m_Grid.AddCell(row, col++, str);

		// �Ŵ�ö�� ��ġ(���� ����)
		if(nJ == 0 || nJ == pBri->GetCountJijum()-1)
		{
			if(nJ == 0)
				sHelp	= "������������ ���� ~ �Ŵ�ö�� ���������� �Ÿ�";
			else
				sHelp	= "�Ŵ�ö�� ���� ~ ������ ������ ��������� �Ÿ�";
			m_Grid.AddCell(row, col++, &pGirderRB->m_dDist, nFormat, nLine, sHelp);
		}
		
		// ���Ϻ�
		for(long upper = 0; upper < 2; upper++)
		{
			bUpper	= (upper == iUPPER);
			sUpdn	= bUpper ? "���" : "�Ϻ�";
			
			m_Grid.AddCell(row, col++, bUpper ? &pGirderRB->m_dCoverUpper[dan] : &pGirderRB->m_dCoverLower[dan], nFormat, nLine); if(upper==iLOWER) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
			m_Grid.AddCell(row, col++, &pGirderRB->m_dDiaUL[upper][dan], nFormat, nLine, str+"ö������", CELL_TYPE_DIA);if(upper==iLOWER) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);m_Grid.SetLastCellFy(pBri->GetValueFy(ePartUpperSlab));
			m_Grid.AddCell(row, col++, &pGirderRB->m_dC1[upper][dan], nFormat, nLine, "����"+sUpdn+" ö�� ���۰Ÿ�"); if(upper==iLOWER) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
			m_Grid.AddCell(row, col++, &pGirderRB->m_nB[upper][dan], nFormat, FALSE, nLine, sUpdn+"��� ����");if(upper==iLOWER) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
			m_Grid.AddCell(row, col++, &pGirderRB->m_dC[upper][dan], nFormat, FALSE, sUpdn+"ö�� ����");if(upper==iLOWER) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
			m_Grid.AddCell(row, col++, &pGirderRB->m_dC2[upper][dan], nFormat, nLine, "����"+sUpdn+" ö�� ������ �Ÿ�");if(upper==iLOWER) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);

		}
		if(dan == 0)
		{
			// ����
			m_Grid.AddCell(row, col++, &pGirderRB->m_dDiaSide, nFormat, nLine, "����ö�� ����", CELL_TYPE_DIA);m_Grid.SetLastCellFy(pBri->GetValueFy(ePartUpperSlab, TRUE));
			m_Grid.AddCell(row, col++, &pGirderRB->m_dF1, nFormat, nLine, "����ö�� ���� �Ÿ�");
			m_Grid.AddCell(row, col++, &pGirderRB->m_nE, nFormat, FALSE, nLine, "ö�� ö�� ����");
			m_Grid.AddCell(row, col++, &pGirderRB->m_dF, nFormat, FALSE, "����ö�� ����");
			m_Grid.AddCell(row, col++, &pGirderRB->m_dF2, nFormat, nLine, "����ö�� ������ �Ÿ�");

			// ��Ʈ�� ö�� ����
			m_Grid.AddCell(row, col++, &pGirderRB->m_dDiaStirrup, nFormat, nLine, "STIRRUP ö�� ����", CELL_TYPE_DIA);m_Grid.SetLastCellFy(pBri->GetValueFy(ePartUpperSlab, TRUE));

			sArr.RemoveAll();
			long strup = 0;
			for(strup = 0; strup < MAX_REBAR_STIRRUP; strup++)
			{
				str.Format("%d", strup+1);
				sArr.Add(str);
			}
			m_Grid.AddCell(row, col++, &pGirderRB->m_nCountStirrup, nFormat, FALSE, nLine, "STIRRUP ö�� ����", CELL_TYPE_COMBO, sArr);

			for(strup = 0; strup < pGirderRB->m_nCountStirrup; strup++)
			{
				sHelp.Format("%d", strup+1);
				if(strup > 0)
				{
					m_Grid.AddCell(row, col++, &pGirderRB->m_xyDistStirrup[strup].x, nFormat, nLine, sHelp);
				}
				m_Grid.AddCell(row, col++, &pGirderRB->m_xyDistStirrup[strup].y, nFormat, nLine, sHelp);
			}
		}
		else
		{
			m_Grid.AddCell(row, col++, "", nFormat, CELL_READONLY);
			m_Grid.AddCell(row, col++, "", nFormat, CELL_READONLY);
			m_Grid.AddCell(row, col++, "", nFormat, CELL_READONLY);
			m_Grid.AddCell(row, col++, "", nFormat, CELL_READONLY);
			m_Grid.AddCell(row, col++, "", nFormat, CELL_READONLY);
			m_Grid.AddCell(row, col++, "", nFormat, CELL_READONLY);
		}
	}

	// ��Ʈ�� �� ���
	m_sCtcGirder.Format("%.0f", pGirderRB->m_dCTC);
	UpdateData(FALSE);

	return TRUE;
}

// ���� �Է����� ��ü ã��
CWallApp* CInputRebarColumn::GetCurInWall()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return NULL;
	return pBri->GetInWall(m_nTabRow-1);
}

// ���� �Է����� ��ġ ã��
// long CInputRebarColumn::GetIdxCurHunch()
// {
// 	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
// 	if(!pBri) return -1;
// 
// 	// ���� �Է����� ��ü�� ������ ã���� ��.(��ս��϶��� �����Ƿ�)
// 	CWallApp *pWall = GetCurInWall();
// 	if(!pWall) return -1;
// 	if(!pWall->m_bTopIsColumn) return -1;
// 
// 	return pBri->GetIdxHunchInWall(m_nTabRow, TRUE);
// }

BOOL CInputRebarColumn::SetGridDataColumn()
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 2, 1);
	m_Grid.SetCellSize(20, 50);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return FALSE;
	CWallApp *pWall		= GetCurInWall();
	

	if(!pWall) 
	{
		m_pIn->GridMessage(&m_Grid, "��ü�� ���� ���� �ʽ��ϴ�");
		return FALSE;
	}
	if(!pWall->m_bTopIsColumn && !pWall->IsRoundRectWall()) 
	{
		m_pIn->GridMessage(&m_Grid, "��ս� ��ü�� �ƴմϴ�");
		return FALSE;
	}
	if(pWall->m_bTopIsColumn)
		pWall->m_bRoundRect	= FALSE;
	CColumnApp *pColumn	= &pWall->m_Column;
	
	long row		= 0, col = 0;
	CString str		= _T("");
	CString sHelp	= _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;
	UINT nLineReadOnly = CELL_READONLY;
	long nInputRebarDan = 3;

	BOOL bStraightVert = FALSE, bStraightHorz = FALSE;
//	UINT nLineHorz = 0, nLineVert = 0;

	// Ÿ��Ʋ
	m_Grid.AddCell(row, col, "����\n");
	m_Grid.AddCell(row+1, col++, "����\n");
	if(!pWall->m_bRoundRect)
	{
		m_Grid.AddCell(row, col, "��ö��");
		m_Grid.AddCell(row+1, col++, "dc");
		m_Grid.AddCell(row, col, "��ö��");
		m_Grid.AddCell(row+1, col++, "M");
	}

	m_Grid.AddCell(row, col, "����");
	m_Grid.AddCell(row+1, col++, "R");
	m_Grid.AddCell(row, col, "���򱸰�");
	m_Grid.AddCell(row+1, col++, "C1");
	m_Grid.AddCell(row, col, "���򱸰�");
	m_Grid.AddCell(row+1, col++, "B");
	m_Grid.AddCell(row, col, "���򱸰�");
	m_Grid.AddCell(row+1, col++, "C");
	m_Grid.AddCell(row, col, "���򱸰�");
	m_Grid.AddCell(row+1, col++, "C2");
	m_Grid.AddCell(row, col, "��������");
	m_Grid.AddCell(row+1, col++, "F1");
	m_Grid.AddCell(row, col, "��������");
	m_Grid.AddCell(row+1, col++, "E");
	m_Grid.AddCell(row, col, "��������");
	m_Grid.AddCell(row+1, col++, "F");
	m_Grid.AddCell(row, col, "��������");
	m_Grid.AddCell(row+1, col++, "F2");
	row++;

	long dan = 0; 
	for(dan = 0; dan < nInputRebarDan; dan++) 
	{
		// ����, ���򱸰��� ���������� �����ϴ��� �˻��Ѵ�
		bStraightVert = m_pIn->m_pARcBridgeDataStd->IsExistMainRebarStraightVert(pColumn, dan);
		bStraightHorz = m_pIn->m_pARcBridgeDataStd->IsExistMainRebarStraightHorz(pColumn, dan);
		
		UINT nLineHorz = bStraightHorz == TRUE ? nLine : nLineReadOnly;
		UINT nLineVert = bStraightVert == TRUE ? nLine : nLineReadOnly;
		
		row	++;
		col	= 0;
		str.Format("%d��", dan+1);
		m_Grid.AddCell(row, col++, str);

		sHelp	= "�簢��/Ʈ���� ����� ���򱸰��̳�, �ٰ����� �� �鿡 ���� ��ö�� ��ġ�� �����մϴ�.";
		if(!pWall->m_bRoundRect)
		{
			m_Grid.AddCell(row, col++, &pColumn->m_dCover[iOUTTER][dan], nFormat, (dan==0) ? CELL_READONLY : nLine, str+" ��ö�ٱ����� �Ÿ�");
			m_Grid.AddCell(row, col++, &pColumn->m_dDiaMain[iOUTTER][dan], nFormat, nLine, str+" ��ö�� ö������", CELL_TYPE_DIA);
		}
				
		m_Grid.AddCell(row, col++, &pColumn->m_nR[iOUTTER][dan], nFormat, FALSE, nLine, "����� ��ö�� ����");
		m_Grid.AddCell(row, col++, &pColumn->m_dC1[iOUTTER][dan], nFormat, nLineHorz, sHelp);
		m_Grid.AddCell(row, col++, &pColumn->m_nB[iOUTTER][dan], nFormat, FALSE, nLineHorz, sHelp);
		m_Grid.AddCell(row, col++, &pColumn->m_dC[iOUTTER][dan], nFormat, pWall->m_bTopIsColumn == FALSE ? nLineReadOnly : nLineHorz, sHelp);
		m_Grid.AddCell(row, col++, &pColumn->m_dC2[iOUTTER][dan], nFormat, nLineHorz, sHelp);

		//32488 ��� ��ü�� �ϴ��� CTC ������ ���� ��ġ�� ��쿡 CTC ������ ��ĥ �� �ֵ��� �ϱ� ���Ͽ� ��ü�̴��� ReadOnly����
		sHelp	= "�簢�� ����� ���������� ���� ��ö�� ��ġ�� �����մϴ�.";
		m_Grid.AddCell(row, col++, &pColumn->m_dF1[iOUTTER][dan], nFormat, nLineVert, sHelp);
		m_Grid.AddCell(row, col++, &pColumn->m_nE[iOUTTER][dan], nFormat, FALSE, nLineVert, sHelp);
		m_Grid.AddCell(row, col++, &pColumn->m_dF[iOUTTER][dan], nFormat, nLineVert, sHelp); //pBri->m_nTypePlacingMainRebar_UpperSlab;
		//m_Grid.AddCell(row, col++, &pColumn->m_dF[iOUTTER][dan], nFormat, pWall->m_bTopIsColumn == FALSE ? nLineReadOnly : nLineVert, sHelp); //32488 ������ �κ�
		m_Grid.AddCell(row, col++, &pColumn->m_dF2[iOUTTER][dan], nFormat, nLineVert, sHelp);
	}

	return TRUE;
}

// 2,3���� 1�� ö���� ���� ���ٵ簡 �ϴµ��� �Է��� ���� �ʴ� ö�ٿ� ���ؼ� 
// ����ȭ�� ������.
void CInputRebarColumn::SyncRebar(BOOL bUpperSlabGirder)
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	// ��Ʈ�� ������ö��(��)�� ������ö��(��, 1��)�� ��ġ�� ���󰣴�.
	long nJ	= m_nTabRow;
	if(pBri->IsGirderRebar(nJ, m_nTab == m_nTabUpperGirder))
	{
		pBri->SetDataDefaultRebarGirder(nJ, FALSE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, bUpperSlabGirder);
		CGirderSectionRebar *pGirderRB		= &pBri->m_SlabGirderRebar[bUpperSlabGirder ? iUPPER : iLOWER][nJ];
		pGirderRB->m_dDiaSide_I	= pGirderRB->m_dDiaSide;
	}

	long nIdxWall			= nJ-1;
	CWallApp *pWall		= pBri->GetInWall(nIdxWall);
	if(pWall)
	{
		CColumnApp *pColumn	= &pWall->m_Column;
		pColumn->SyncRebar();
	}

	// ���ʴ� ��ö�� ������ 2,3�� 1�ܰ� �����ϰ� ��
	long footing = 0; for(footing = 0; footing < pBri->GetCountFooting(); footing++)
	{
		CFootingApp *pFooting	= pBri->GetFooting(footing);
		if(!pFooting) continue;

		long dan = 0; for(dan = 1; dan < MAX_REBAR_DAN; dan++)
		{
			for(long upper = 0; upper < 2; upper++)
				pFooting->m_placeByBlock_Main_Side[upper][dan]	= pFooting->m_placeByBlock_Main_Side[upper][0];
		}
	}
}

BOOL CInputRebarColumn::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	return CInputDialogBase::DestroyWindow();
}

BOOL CInputRebarColumn::SetGridDataFooting()
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 2, 1);
	m_Grid.SetCellSize(20, 50);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return FALSE;
	CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(m_nTabRow);
	if(!pFooting) 
	{
		m_pIn->GridMessage(&m_Grid, "���ʰ� ���� ���� �ʽ��ϴ�.");
		return FALSE;
	}
	
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

	// ���
	pPlace	= &pFooting->m_placeByBlock_Main_Side[iUPPER][0];
	for(block = 0; block < pPlace->m_nCountBlock; block++)
	{
		str.Format("U%d", block+1);
		m_Grid.AddCell(row, col, "���");
		m_Grid.AddCell(row+1, col, str);
		m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine);
		m_Grid.AddCell(row+3, col++, &pPlace->m_xyArr, block, FALSE, nFormat, nLine);
	}


	// �ϸ�
	pPlace	= &pFooting->m_placeByBlock_Main_Side[iLOWER][0];
	for(block = 0; block < pPlace->m_nCountBlock; block++)
	{
		str.Format("L%d", block+1);
		m_Grid.AddCell(row, col, "�ϸ�");
		m_Grid.AddCell(row+1, col, str); m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
		m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine); m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
		m_Grid.AddCell(row+3, col++, &pPlace->m_xyArr, block, FALSE, nFormat, nLine); m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
	}

	// ����ö��(����)
	pPlace	= &pFooting->m_placeByBlock_Shear_Side;
	for(block = 0; block < pPlace->m_nCountBlock; block++)
	{
		str.Format("V%d", block+1);
		m_Grid.AddCell(row, col, "����ö��");
		m_Grid.AddCell(row+1, col, str); m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR3);
		m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine); m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR3);
		m_Grid.AddCell(row+3, col++, &pPlace->m_xyArr, block, FALSE, nFormat, nLine); m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR3);
	}

	return TRUE;
}

// TabRow�� tab ���� ���÷� ���� ����
// Tab�� �Ŵ�,����϶��� ������ü ������ŭ, �����϶��� ������ü+������ü ������ ���� �Ǿ�� ��.
void CInputRebarColumn::ReInitTabRowCtrl()
{
	m_TabRow.RemoveAllTabs();
	m_TabRow.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_TabRow.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	// row��
	CString str			= _T("");
	m_sArrTabRow.RemoveAll();

	if(m_nTab == m_nTabJongGirder)
	{
		m_sArrTabRow.Add("����");
		m_sArrTabRow.Add("����");
		if(pBri->m_bIsExp)
		{
			m_sArrTabRow.Add("������(����,����)");
			m_sArrTabRow.Add("������(����,����)");
			m_sArrTabRow.Add("������(����,����)");
			m_sArrTabRow.Add("������(����,����)");
		}
	}
	else
	{
		m_sArrTabRow.Add("������");

		long nCountInWall	= pBri->GetCountInWall();
		long wall = 0; for(wall = 0; wall < nCountInWall; wall++)
		{
			str.Format("�߰�����%d", wall+1); 
			m_sArrTabRow.Add(str);
		}
		m_sArrTabRow.Add("������");
	}
	
	long i = 0; for(i = 0; i < m_sArrTabRow.GetSize(); i++)
	m_TabRow.InsertItem(i, m_sArrTabRow.GetAt(i));

	if(m_TabRow.GetItemCount() <= m_nTabRow) m_nTabRow = 0;
	m_TabRow.SetCurSel(m_nTabRow);
}

void CInputRebarColumn::OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result)
{
}

CARcBridgeInputStd* CInputRebarColumn::GetIn()
{
	return m_pIn;
}

BOOL CInputRebarColumn::IsValid()
{
	return TRUE;
}
void CInputRebarColumn::OnKillfocusEditCtcGirder()
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	if(m_nTab != m_nTabUpperGirder && m_nTab != m_nTabLowerGirder && m_nTab != m_nTabJongGirder) return;
	long nJ	= m_nTabRow;
	BOOL bStt	= TRUE;
	BOOL bLeft	= TRUE;
	BOOL bGagak	= TRUE;
	BOOL bJongGirder	= TRUE;
	GetInfoGirder(bStt, bLeft, bGagak, bJongGirder);
	BOOL bUpper = m_nTab == m_nTabUpperGirder ? TRUE : FALSE;

	if(bJongGirder)
	{
		if(!pBri->IsGirderRebarJong(bStt, bLeft, bGagak)) return;
	}
	else
	{
		if(!pBri->IsGirderRebar(nJ, m_nTab == m_nTabUpperGirder)) return;	
	}

	UpdateData(TRUE);

	CGirderSectionRebar *pGirder	= bJongGirder ? pBri->GetGirderRebarJong(bStt, bLeft, bGagak) : &pBri->m_SlabGirderRebar[bUpper ? iUPPER : iLOWER][nJ];
	pGirder->m_dCTC	= atof(m_sCtcGirder);

	DrawInputDomyunView(FALSE);
}
void CInputRebarColumn::OnButtonExamine() 
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	BOOL bFemNormal = pBri->m_FemNormal.IsOutputLoaded();
	BOOL bFemEQ     = pBri->IsRcSlab() ? TRUE : (pBri->GetFemManage(COMBO_EARTHQUAKE)->IsOutputLoaded());
	
	if(!bFemNormal || (pBri->GetTypeDesingEQ() == 1 && !bFemEQ))
	{
		AfxMessageBox("�𵨸� �ؼ��� �����Ŀ� ������並 �Ͻñ� �ٶ��ϴ�.");
		return;
	}

	if(m_nTab == m_nTabFooting) return;

	if(m_nTab == m_nTabColumn)
	{
		if(m_pDlgCheck->GetSafeHwnd() && m_pDlgCheck->IsWindowVisible()) m_pDlgCheck->DestroyWindow();
		m_pDlgCheck->SetBriAndContent(pBri,IDD_REBAR_COLUMN,m_nTabRow);
		m_pDlgCheck->ShowAndCheck(this);
		CString str = _T("");
		str.Format("��� ���� - %s", m_sArrTabRow.GetAt(m_nTabRow));
		m_pDlgCheck->SetWindowText(str);		
	}
	if(m_nTab == m_nTabUpperGirder || m_nTab == m_nTabLowerGirder)
	{
		if(m_pDlgEdit->GetSafeHwnd() && m_pDlgEdit->IsWindowVisible()) m_pDlgEdit->DestroyWindow();
		m_pDlgEdit->SetBriAndContent(pBri,IDD_REBAR_COLUMN,m_nTabRow);
		m_pDlgEdit->ShowAndCheck(this);	
	}
	if(m_nTab == m_nTabWall)
	{
		BOOL bCheckColumn = FALSE;
		if(m_nTabRow == 0 || m_nTabRow == pBri->m_nQtyJigan)
		{
			bCheckColumn = pBri->m_nSelectWallSectionDesign == 1 ? TRUE : FALSE;
		}
		else
		{
			bCheckColumn = pBri->m_nSelectInWallSectionDesign == 1 ? TRUE : FALSE;
		}

		if(bCheckColumn)
		{
			if(m_pDlgCheck->GetSafeHwnd() && m_pDlgCheck->IsWindowVisible()) m_pDlgCheck->DestroyWindow();
			m_pDlgCheck->SetBriAndContent(pBri,IDD_REBAR_COLUMN,m_nTabRow);
			m_pDlgCheck->ShowAndCheck(this);	
			CString str = _T("");
			str.Format("��ü ���� - %s", m_sArrTabRow.GetAt(m_nTabRow));
			m_pDlgCheck->SetWindowText(str);		
		}
	}
	if(m_nTab == m_nTabJongGirder)
	{
		BOOL bStt	= TRUE;
		BOOL bLeft	= TRUE;
		BOOL bGagak	= TRUE;
		BOOL bJongGirder	= TRUE;
		GetInfoGirder(bStt, bLeft, bGagak, bJongGirder);
		if(bGagak || m_nTabRow<2)
		{
			if(pBri->IsGagakForLine(bStt, bLeft) || m_nTabRow<2)
			{
				if(m_pDlgEdit->GetSafeHwnd() && m_pDlgEdit->IsWindowVisible()) m_pDlgEdit->DestroyWindow();
				// ��ΰŴ��� ���Ŵ��� �����ϱ� ���� MAX_JIJUM_COUNT_RC�� ���ߴ�...
				m_pDlgEdit->SetBriAndContent(pBri,IDD_REBAR_COLUMN,m_nTabRow+MAX_JIJUM_COUNT_RC);
				m_pDlgEdit->ShowAndCheck(this);			
			}
		}
	}
}

void CInputRebarColumn::OnCheckMatchWithwall() 
{
	// TODO: Add your control notification handler code here
}

void CInputRebarColumn::DrawPM(CDomyun *pDomP)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	CARcBridgeCalcStd *pCalcStd = new CARcBridgeCalcStd(pBri);
	pCalcStd->CalculateAll();

	long nJijum = m_nTabRow;
	CGirderAndColumn *pGirCol = pCalcStd->m_pColGir->m_pList.GetAt(nJijum);

	CColumn ColCalc(pGirCol->pConcColumn[0]);
	ColCalc.CalculateAll(TRUE);

	CDomyun Dom(pDomP);
	Dom.SetCWnd(m_pView);

	CDrawPM DrawPM(m_pIn->m_pARoadOptionStd);
	DrawPM.DrawPM(&Dom,&ColCalc);

	*pDomP << Dom;
	delete pCalcStd;
}

void CInputRebarColumn::OnCheckViewPM() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DrawInputDomyunView();
}

void CInputRebarColumn::KillFocusDlg()
{
}

void CInputRebarColumn::OnSelchangeComboInputColumn() 
{
	// TODO: Add your control notification handler code here
	if(SetGridDataStd())
		SetDataInit();
}

BOOL CInputRebarColumn::SetGridDataCrossTieAndRound()
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 2, 1);
	m_Grid.SetCellSize(20, 50);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return FALSE;
	CWallApp *pWall		= GetCurInWall();
	if(!pWall) 
	{
		m_pIn->GridMessage(&m_Grid, "��ü�� ���� ���� �ʽ��ϴ�");
		return FALSE;
	}
	if(!pWall->m_bTopIsColumn) 
	{
		m_pIn->GridMessage(&m_Grid, "��ս� ��ü�� �ƴմϴ�");
		return FALSE;
	}
	CColumnApp *pColumn	= &pWall->m_Column;
	

	long row = 0, col = 0;
	CString str = _T(""), sHelp = _T("");
	CStringArray sArr;
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	CString sInner	= _T("");
	CString strTmp	= _T("");
	m_Grid.AddCell(row, col, "����\n");
	m_Grid.AddCell(row+1, col, "����\n");
	m_Grid.AddCell(row+2, col, "����");
	m_Grid.AddCell(row+3, col, "����");
	

	long dan  = 0;
	long nCountDanRebarMain	= pColumn->GetCountDan_RebarMain();
	for(long inner = 0; inner < 2; inner++)
	{
		CPlaceRebarByBlock *pPlace	= &pColumn->m_placeRebarRound[inner][dan];
		if(pPlace == NULL) continue;

		BOOL bOutter	= (inner == iOUTTER);		
		sHelp				= bOutter ? "������ö������(1��ö�� ������ �ش�)" : "������ö������(2,3�� ����ö�ٰ�, 1,2,3�� ����ö�ٿ� �ش�)";
		strTmp			= bOutter ? "H" : "h";
		sInner				= bOutter ? "������ö��" : "������ö��";

		col++;
		m_Grid.AddCell(row, col, sInner);
		m_Grid.AddCell(row+1, col, bOutter ? "R1" : "RI1");
		m_Grid.AddCell(row+2, col, &pPlace->m_dDia, nFormat, nLine, sHelp, CELL_TYPE_DIA); if(!bOutter) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);

		col++;
		m_Grid.AddCell(row, col, sInner);
		m_Grid.AddCell(row+1, col, "����");
		sArr.RemoveAll();
		long i = 0; for(i = 0; i < MAX_REBAR_BLOCK; i++)
		{
			str.Format("%d", i+1);
			sArr.Add(str);
		}
		m_Grid.AddCell(row+2, col, &pPlace->m_nCountBlock, nFormat, FALSE, nLine, "��ö�� ������", CELL_TYPE_COMBO, sArr); if(!bOutter) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);

		for(long block = 0; block < pPlace->m_nCountBlock; block++)
		{
			sHelp.Format("%s %d��° ����", sInner, block+1);
			col++;
			str.Format("%s%d", strTmp, block+1);
			m_Grid.AddCell(row, col, sInner);
			m_Grid.AddCell(row+1, col, str); 
			m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine, sHelp+" ����"); m_Grid.SetCellDec(0); if(!bOutter) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
			m_Grid.AddCell(row+3, col, &pPlace->m_xyArr, block, FALSE, nFormat, nLine, sHelp+" ����"); if(!bOutter) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
		}
	}

	col++;

	// ũ�ν�Ÿ�� ����
	m_Grid.AddCell(row, col, "ũ�ν�Ÿ��");
	m_Grid.AddCell(row+1, col, "CT1");
	m_Grid.AddCell(row+2, col, &pColumn->m_dDiaCrossTie, nFormat, nLine, "ũ�ν�Ÿ�� ����", CELL_TYPE_DIA);
	col++;

	// ������� �ƴҶ��� �Է¹���.
	if(pColumn->m_nTypeRebarCrossTie == 0)
	{
		// �����ġ
		m_Grid.AddCell(row, col, "ũ�ν�Ÿ��");
		m_Grid.AddCell(row+1, col, "CTH1");
		m_Grid.AddCell(row+2, col, "1", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+3, col, &pColumn->m_dCTH1, nFormat, nLine, "ũ�ν�Ÿ�� �����ġ ù��° ���� ����");
		col++;

		m_Grid.AddCell(row, col, "ũ�ν�Ÿ��");
		m_Grid.AddCell(row+1, col, "HN");
		m_Grid.AddCell(row+2, col, &pColumn->m_nCTH, nFormat, FALSE, nLine, "ũ�ν�Ÿ�� �����ġ �ι�° ���� ����");
		m_Grid.AddCell(row+3, col, &pColumn->m_dCTH, nFormat, nLine, "ũ�ν�Ÿ�� �����ġ �ι�° ���� ����");
		col++;

		m_Grid.AddCell(row, col, "ũ�ν�Ÿ��");
		m_Grid.AddCell(row+1, col, "CTH");
		m_Grid.AddCell(row+2, col, "1", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+3, col, &pColumn->m_dCTH2, nFormat, nLine, "ũ�ν�Ÿ�� �����ġ ������ ���� ����");
		col++;

		// ������ġ
		m_Grid.AddCell(row, col, "ũ�ν�Ÿ��");
		m_Grid.AddCell(row+1, col, "CTV1");
		m_Grid.AddCell(row+2, col, "1", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+3, col, &pColumn->m_dCTV1, nFormat, nLine, "ũ�ν�Ÿ�� ������ġ ù��° ���� ����");
		col++;

		m_Grid.AddCell(row, col, "ũ�ν�Ÿ��");
		m_Grid.AddCell(row+1, col, "VN");
		m_Grid.AddCell(row+2, col, &pColumn->m_nCTV, nFormat, FALSE, nLine, "ũ�ν�Ÿ�� ������ġ �ι�° ���� ����");
		m_Grid.AddCell(row+3, col, &pColumn->m_dCTV, nFormat, nLine, "ũ�ν�Ÿ�� ������ġ �ι�° ���� ����");
		col++;

		m_Grid.AddCell(row, col, "ũ�ν�Ÿ��");
		m_Grid.AddCell(row+1, col, "CTV");
		m_Grid.AddCell(row+2, col, "1", nFormat, CELL_READONLY);
		m_Grid.AddCell(row+3, col, &pColumn->m_dCTV2, nFormat, nLine, "ũ�ν�Ÿ�� ������ġ ������ ���� ����");
		col++;
	}


	CPlaceRebarByBlock *pPlace	= &pColumn->m_placeRebarCrossTie;
	m_Grid.AddCell(row, col, "ũ�ν�Ÿ��");
	m_Grid.AddCell(row+1, col, "����");
	sArr.RemoveAll();
	long i = 0; for(i = 0; i < MAX_REBAR_BLOCK; i++)
	{
		str.Format("%d", i+1);
		sArr.Add(str);
	}
	m_Grid.AddCell(row+2, col, &pPlace->m_nCountBlock, nFormat, FALSE, nLine, "��ö�� ������", CELL_TYPE_COMBO, sArr);

	for(long block = 0; block < pPlace->m_nCountBlock; block++)
	{
		sHelp.Format("%s %d��° ����", "ũ�ν�Ÿ��", block+1);
		col++;
		str.Format("%s%d", strTmp, block+1);
		m_Grid.AddCell(row, col, "ũ�ν�Ÿ��");
		m_Grid.AddCell(row+1, col, str); 
		m_Grid.AddCell(row+2, col, &pPlace->m_xyArr, block, TRUE, nFormat, nLine, sHelp+" ����"); m_Grid.SetCellDec(0);
		m_Grid.AddCell(row+3, col, &pPlace->m_xyArr, block, FALSE, nFormat, nLine, sHelp+" ����");
	}

	return TRUE;
}

BOOL CInputRebarColumn::SetGridDataWall(long nJ)
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(1, 2, 0, 2);
	m_Grid.SetCellSize(20, -1);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return FALSE;

	long row		= 0, col = 0;
	CString str		= _T("");
//	CString sHelp	= _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;

	CString sHelp	= "(��ν����� ��� ��±��� ���� ���� EL. - �Էµ� ����)���� ���ʿ� ��ٵǴ� ��±ٿ� ���ؼ� ����";
	if(nJ == 0)
	{
		if(!pBri->IsOutWall(TRUE)) 
		{
			m_pIn->GridMessage(&m_Grid, "������ ��ü�� �������� �ʽ��ϴ�");
			return FALSE;
		}
		m_Grid.AddCell(row, col++, "������� ���·� ��ٵǴ� ��±� ����");
		m_Grid.AddCell(row, col++, "������� ���·� ��ٵǴ� ��±� ����");
		m_Grid.AddCell(row, col++, &pBri->m_dRangeSupportRebarForSameUpperSlab[iSTT], nFormat, nLine, sHelp);
		m_Grid.AddCell(row, col++, "", nFormat, CELL_READONLY);
		m_Grid.AddCell(row, col, "", nFormat, CELL_READONLY);

		row++;
		col	= 0;
		m_Grid.AddCell(row, col++, "������� ���·� \n�߰��Ǵ� ��±�\n");
		m_Grid.AddCell(row, col++, "��ġ");
		m_Grid.AddCell(row, col++, &pBri->m_placeByBlock_SupportRebarAddSameUpperSlab[iSTT].m_xyArr, 0, TRUE, nFormat, nLine, "����");
		m_Grid.AddCell(row, col++, "@", nFormat, CELL_READONLY);
		m_Grid.AddCell(row, col, &pBri->m_placeByBlock_SupportRebarAddSameUpperSlab[iSTT].m_xyArr, 0, FALSE, nFormat, nLine, "����");

		row++;
		col = 0;
		m_Grid.AddCell(row, col++, "������� ���·� \n�߰��Ǵ� ��±�\n");
		m_Grid.AddCell(row, col++, "��������");
		m_Grid.AddCell(row, col++, &pBri->m_dDistAddSupportRebarForSameUpperSlab[iSTT][iLEFT]);
		m_Grid.AddCell(row, col++, "", nFormat, CELL_READONLY);
		m_Grid.AddCell(row, col, "", nFormat, CELL_READONLY);

		row++;
		col = 0;
		m_Grid.AddCell(row, col++, "������� ���·� \n�߰��Ǵ� ��±�\n");
		m_Grid.AddCell(row, col++, "��������");
		m_Grid.AddCell(row, col++, &pBri->m_dDistAddSupportRebarForSameUpperSlab[iSTT][iRIGHT]);
		m_Grid.AddCell(row, col++, "", nFormat, CELL_READONLY);
		m_Grid.AddCell(row, col, "", nFormat, CELL_READONLY);
	}
	else if(nJ == pBri->m_nQtyJigan)
	{
		if(!pBri->IsOutWall(FALSE)) 
		{
			m_pIn->GridMessage(&m_Grid, "������ ��ü�� �������� �ʽ��ϴ�");
			return FALSE;
		}
		m_Grid.AddCell(row, col++, "������� ���·� ��ٵǴ� ��±� ����");
		m_Grid.AddCell(row, col++, "������� ���·� ��ٵǴ� ��±� ����");
		m_Grid.AddCell(row, col++, &pBri->m_dRangeSupportRebarForSameUpperSlab[iEND], nFormat, nLine, sHelp);
		m_Grid.AddCell(row, col++, "", nFormat, CELL_READONLY);
		m_Grid.AddCell(row, col, "", nFormat, CELL_READONLY);

		row++;
		col	= 0;
		m_Grid.AddCell(row, col++, "������� ���·� \n�߰��Ǵ� ��±�\n");
		m_Grid.AddCell(row, col++, "��ġ");
		m_Grid.AddCell(row, col++, &pBri->m_placeByBlock_SupportRebarAddSameUpperSlab[iEND].m_xyArr, 0, TRUE, nFormat, nLine, "����");
		m_Grid.AddCell(row, col++, "@", nFormat, CELL_READONLY);
		m_Grid.AddCell(row, col, &pBri->m_placeByBlock_SupportRebarAddSameUpperSlab[iEND].m_xyArr, 0, FALSE, nFormat, nLine, "����");

		row++;
		col = 0;
		m_Grid.AddCell(row, col++, "������� ���·� \n�߰��Ǵ� ��±�\n");
		m_Grid.AddCell(row, col++, "��������");
		m_Grid.AddCell(row, col++, &pBri->m_dDistAddSupportRebarForSameUpperSlab[iEND][iLEFT]);
		m_Grid.AddCell(row, col++, "", nFormat, CELL_READONLY);
		m_Grid.AddCell(row, col, "", nFormat, CELL_READONLY);

		row++;
		col = 0;
		m_Grid.AddCell(row, col++, "������� ���·� \n�߰��Ǵ� ��±�\n");
		m_Grid.AddCell(row, col++, "��������");
		m_Grid.AddCell(row, col++, &pBri->m_dDistAddSupportRebarForSameUpperSlab[iEND][iRIGHT]);
		m_Grid.AddCell(row, col++, "", nFormat, CELL_READONLY);
		m_Grid.AddCell(row, col, "", nFormat, CELL_READONLY);
	}
	else 
	{
		CWallApp *pWall	= pBri->GetInWall(nJ-1);
		if(!pWall) 
		{
			m_pIn->GridMessage(&m_Grid, "��ü�� �������� �ʽ��ϴ�");
			return FALSE;
		}
		if(!pWall->m_bIs) 
		{
			m_pIn->GridMessage(&m_Grid, "��ü�� �������� �ʽ��ϴ�");
			return FALSE;
		}
		if(pWall->m_bTopIsColumn)
		{
			m_pIn->GridMessage(&m_Grid, "��ü�� �������� �ʽ��ϴ�.");
			return FALSE;
		}

		m_Grid.AddCell(row, col++, "������� ���·� ��ٵǴ� ��±� ����");
		m_Grid.AddCell(row, col++, "������� ���·� ��ٵǴ� ��±� ����");
		m_Grid.AddCell(row, col++, &pWall->m_dRangeSupportRebarForSameUpperSlab, nFormat, nLine, sHelp);
		m_Grid.AddCell(row, col++, "", nFormat, CELL_READONLY);
		m_Grid.AddCell(row, col, "", nFormat, CELL_READONLY);

		row++;
		col	= 0;
		m_Grid.AddCell(row, col++, "������� ���·� \n�߰��Ǵ� ��±�\n");
		m_Grid.AddCell(row, col++, "��ġ");
		m_Grid.AddCell(row, col++, &pWall->m_placeByBlock_SupportRebarAddSameUpperSlab.m_xyArr, 0, TRUE, nFormat, nLine, "����");
		m_Grid.AddCell(row, col++, "@", nFormat, CELL_READONLY);
		m_Grid.AddCell(row, col, &pWall->m_placeByBlock_SupportRebarAddSameUpperSlab.m_xyArr, 0, FALSE, nFormat, nLine, "����");

		row++;
		col = 0;
		m_Grid.AddCell(row, col++, "������� ���·� \n�߰��Ǵ� ��±�\n");
		m_Grid.AddCell(row, col++, "��������");
		m_Grid.AddCell(row, col++, &pWall->m_dDistAddSupportRebarForSameUpperSlab[iLEFT]);
		m_Grid.AddCell(row, col++, "", nFormat, CELL_READONLY);
		m_Grid.AddCell(row, col, "", nFormat, CELL_READONLY);

		row++;
		col = 0;
		m_Grid.AddCell(row, col++, "������� ���·� \n�߰��Ǵ� ��±�\n");
		m_Grid.AddCell(row, col++, "��������");
		m_Grid.AddCell(row, col++, &pWall->m_dDistAddSupportRebarForSameUpperSlab[iRIGHT]);
		m_Grid.AddCell(row, col++, "", nFormat, CELL_READONLY);
		m_Grid.AddCell(row, col, "", nFormat, CELL_READONLY);
	}


	long nRows(0), nCols(0);
	m_Grid.GetCellCount(nRows, nCols);
	m_Grid.SetRows(nRows);
	m_Grid.SetCols(nCols);
	m_Grid.SetColumnWidth(0, 200);
	m_Grid.SetColumnWidth(1, 100);
	m_Grid.SetColumnWidth(2, 50);

	return TRUE;
}

// ���Ŵ� �¿���
BOOL CInputRebarColumn::SetGridDataJongGirder()
{
	// �׸��� �ʱ�ȭ
	m_Grid.SetGridDefault(2, 2, 2, 0);
	m_Grid.SetCellSize(20, 50);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return FALSE;

	long row		= 0, col = 0;
	CString str		= _T("");
	CString sHelp	= _T("");
	CString sUpdn	= _T("");
	UINT nFormat	= DT_CENTER | DT_VCENTER;
	UINT nLine		= CELL_LINE_NO;
	CStringArray sArr;


	// ���� �������� ��Ʈ��
	BOOL bStt	= TRUE;
	BOOL bLeft	= TRUE;
	BOOL bGagak	= TRUE;
	BOOL bJongGirder	= TRUE;
	GetInfoGirder(bStt, bLeft, bGagak, bJongGirder);
	if(bGagak)
	{
		if(!pBri->IsGagakForLine(bStt, bLeft))
		{
			m_pIn->GridMessage(&m_Grid, "������ ���� ���� �ʽ��ϴ�");
			return FALSE;
		}
	}
	long left	= bLeft ? iLEFT : iRIGHT;
	CGirderSectionRebar *pGirderRB	= pBri->GetGirderRebarJong(bStt, bLeft, bGagak);

	// Ÿ��Ʋ
	m_Grid.AddCell(row, col, "��ġ\n");
	m_Grid.AddCell(row+1, col++, "��ġ\n");
	
	m_Grid.AddCell(row, col, "��ġ");
	m_Grid.AddCell(row+1, col++, "L1");
	m_Grid.AddCell(row, col, "��ġ");
	m_Grid.AddCell(row+1, col++, "L2");
	m_Grid.AddCell(row, col, "��ġ");
	m_Grid.AddCell(row+1, col++, "D");
	m_Grid.AddCell(row, col, "�Ŵ�(���)");
	m_Grid.AddCell(row+1, col++, "dc");
	m_Grid.AddCell(row, col, "�Ŵ�(���)");
	m_Grid.AddCell(row+1, col++, "GU");
	m_Grid.AddCell(row, col, "�Ŵ�(���)");
	m_Grid.AddCell(row+1, col++, "C1");
	m_Grid.AddCell(row, col, "�Ŵ�(���)");
	m_Grid.AddCell(row+1, col++, "B");
	m_Grid.AddCell(row, col, "�Ŵ�(���)");
	m_Grid.AddCell(row+1, col++, "C");
	m_Grid.AddCell(row, col, "�Ŵ�(���)");
	m_Grid.AddCell(row+1, col++, "C2");

	m_Grid.AddCell(row, col, "�Ŵ�(�ϸ�)");
	m_Grid.AddCell(row+1, col++, "dc'");
	m_Grid.AddCell(row, col, "�Ŵ�(�ϸ�)");
	m_Grid.AddCell(row+1, col++, "GL");
	m_Grid.AddCell(row, col, "�Ŵ�(�ϸ�)");
	m_Grid.AddCell(row+1, col++, "aC1");
	m_Grid.AddCell(row, col, "�Ŵ�(�ϸ�)");
	m_Grid.AddCell(row+1, col++, "aB");
	m_Grid.AddCell(row, col, "�Ŵ�(�ϸ�)");
	m_Grid.AddCell(row+1, col++, "aC");
	m_Grid.AddCell(row, col, "�Ŵ�(�ϸ�)");
	m_Grid.AddCell(row+1, col++, "aC2");

	m_Grid.AddCell(row, col, "�Ŵ�(����)");
	m_Grid.AddCell(row+1, col++, "GS");
	m_Grid.AddCell(row, col, "�Ŵ�(����)");
	m_Grid.AddCell(row+1, col++, "F1");
	m_Grid.AddCell(row, col, "�Ŵ�(����)");
	m_Grid.AddCell(row+1, col++, "E");
	m_Grid.AddCell(row, col, "�Ŵ�(����)");
	m_Grid.AddCell(row+1, col++, "F");
	m_Grid.AddCell(row, col, "�Ŵ�(����)");
	m_Grid.AddCell(row+1, col++, "F2");

	m_Grid.AddCell(row, col, "���ͷ�");
	m_Grid.AddCell(row+1, col++, "GT");

	m_Grid.AddCell(row, col, "���ͷ�");
	m_Grid.AddCell(row+1, col++, "����");
	for(long strup = 0; strup < pGirderRB->m_nCountStirrup; strup++)
	{
		if(strup > 0)
		{
			str.Format("A%d", strup+1);
			m_Grid.AddCell(row, col, "���ͷ�");
			m_Grid.AddCell(row+1, col++, str);
		}

		str.Format("B%d", strup+1);
		m_Grid.AddCell(row, col, "���ͷ�");
		m_Grid.AddCell(row+1, col++, str);
	}
	row++;

	BOOL bUpper	= TRUE;

	long dan = 0; for(dan = 0; dan < 1; dan++)
	{
		row	++;
		col	= 0;
		m_Grid.AddCell(row, col++, &pGirderRB->m_bIs, nFormat, nLine, "", CELL_TYPE_CHECK);

		// �Ŵ�ö�� ��ġ ���� ��ġ�� �� ��ġ
		m_Grid.AddCell(row, col++, &pBri->m_dDistGirderRebarJong[left][iSTT] , nFormat, nLine, sHelp);
		m_Grid.AddCell(row, col++, &pBri->m_dDistGirderRebarJong[left][iEND], nFormat, nLine, sHelp);
		
		// �Ŵ�ö�� ��ġ(���� ����)
		m_Grid.AddCell(row, col++, &pGirderRB->m_dDist, nFormat, nLine, sHelp);
		
		// ���Ϻ�
		for(long upper = 0; upper < 2; upper++)
		{
			bUpper	= (upper == iUPPER);
			sUpdn	= bUpper ? "���" : "�Ϻ�";
			m_Grid.AddCell(row, col++, bUpper ? &pGirderRB->m_dCoverUpper[dan] : &pGirderRB->m_dCoverLower[dan], nFormat, nLine); if(upper==iLOWER) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
			m_Grid.AddCell(row, col++, &pGirderRB->m_dDiaUL[upper][dan], nFormat, nLine, str+"ö������", CELL_TYPE_DIA);if(upper==iLOWER) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2); m_Grid.SetLastCellFy(pBri->GetValueFy(ePartUpperSlab));
			m_Grid.AddCell(row, col++, &pGirderRB->m_dC1[upper][dan], nFormat, nLine, "����"+sUpdn+" ö�� ���۰Ÿ�"); if(upper==iLOWER) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
			m_Grid.AddCell(row, col++, &pGirderRB->m_nB[upper][dan], nFormat, FALSE, nLine, sUpdn+"��� ����");if(upper==iLOWER) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
			m_Grid.AddCell(row, col++, &pGirderRB->m_dC[upper][dan], nFormat, FALSE, sUpdn+"ö�� ����");if(upper==iLOWER) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);
			m_Grid.AddCell(row, col++, &pGirderRB->m_dC2[upper][dan], nFormat, nLine, "����"+sUpdn+" ö�� ������ �Ÿ�");if(upper==iLOWER) m_Grid.SetLastCellBKColor(GRID_TEXT_BK_COLOR2);

		}
		if(dan == 0)
		{
			// ����
			m_Grid.AddCell(row, col++, &pGirderRB->m_dDiaSide, nFormat, nLine, "����ö�� ����", CELL_TYPE_DIA); m_Grid.SetLastCellFy(pBri->GetValueFy(ePartUpperSlab, TRUE));
			m_Grid.AddCell(row, col++, &pGirderRB->m_dF1, nFormat, nLine, "����ö�� ���� �Ÿ�");
			m_Grid.AddCell(row, col++, &pGirderRB->m_nE, nFormat, FALSE, nLine, "ö�� ö�� ����");
			m_Grid.AddCell(row, col++, &pGirderRB->m_dF, nFormat, FALSE, "����ö�� ����");
			m_Grid.AddCell(row, col++, &pGirderRB->m_dF2, nFormat, nLine, "����ö�� ������ �Ÿ�");

			// ��Ʈ�� ö�� ����
			m_Grid.AddCell(row, col++, &pGirderRB->m_dDiaStirrup, nFormat, nLine, "STIRRUP ö�� ����", CELL_TYPE_DIA); m_Grid.SetLastCellFy(pBri->GetValueFy(ePartUpperSlab, TRUE));

			sArr.RemoveAll();
			long strup = 0;
			for(strup = 0; strup < MAX_REBAR_STIRRUP; strup++)
			{
				str.Format("%d", strup+1);
				sArr.Add(str);
			}
			m_Grid.AddCell(row, col++, &pGirderRB->m_nCountStirrup, nFormat, FALSE, nLine, "STIRRUP ö�� ����", CELL_TYPE_COMBO, sArr);

			for(strup = 0; strup < pGirderRB->m_nCountStirrup; strup++)
			{
				sHelp.Format("%d", strup+1);
				if(strup > 0)
				{
					m_Grid.AddCell(row, col++, &pGirderRB->m_xyDistStirrup[strup].x, nFormat, nLine, sHelp);
				}
				m_Grid.AddCell(row, col++, &pGirderRB->m_xyDistStirrup[strup].y, nFormat, nLine, sHelp);
			}
		}
		else
		{
			m_Grid.AddCell(row, col++, "", nFormat, CELL_READONLY);
			m_Grid.AddCell(row, col++, "", nFormat, CELL_READONLY);
			m_Grid.AddCell(row, col++, "", nFormat, CELL_READONLY);
			m_Grid.AddCell(row, col++, "", nFormat, CELL_READONLY);
			m_Grid.AddCell(row, col++, "", nFormat, CELL_READONLY);
			m_Grid.AddCell(row, col++, "", nFormat, CELL_READONLY);
		}
	}

	// ��Ʈ�� �� ���
	m_sCtcGirder.Format("%.0f", pGirderRB->m_dCTC);
	UpdateData(FALSE);

	return TRUE;
}

long CInputRebarColumn::GetInfoGirder(BOOL &bStt, BOOL &bLeft, BOOL &bGagak, BOOL &bJongGirder)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return 0;

	if(m_nTab != m_nTabJongGirder) 
	{
		bJongGirder	= FALSE;
		return m_nTabRow;
	}
	else
		bJongGirder	= TRUE;

	bGagak	= m_nTabRow > 1;
	if(bGagak)
	{
		switch(m_nTabRow)
		{
			case 2: bStt = TRUE; bLeft = TRUE; break;
			case 3: bStt = TRUE; bLeft = FALSE; break;
			case 4: bStt = FALSE; bLeft = TRUE; break;
			case 5: bStt = FALSE; bLeft = FALSE; break;
			default: bGagak	= FALSE;
		}
	}

	if(!bGagak)
	{
		bStt	= TRUE;
		bLeft	= m_nTabRow == iLEFT;
	}

	return 0;
}

void CInputRebarColumn::OnRadioCross() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DrawInputDomyunView(TRUE);
}

void CInputRebarColumn::OnRadioPlane() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DrawInputDomyunView(TRUE);
}

// ���ͷ� ö�� �Է����̸� ���ͷ��� �ε����� ã�Ƽ� �����Ѵ�.
long CInputRebarColumn::GetIdxInputStirrup()
{
	long nIdx	= -1;
	if(m_nTab == m_nTabJongGirder || m_nTab == m_nTabUpperGirder)
	{
		structGridCell *pCell	= m_Grid.GetCurrentCell();
		if(pCell)
		{
			CString str	= pCell->sTitle;
			pCell	= m_Grid.GetCell(0, pCell->nCol);
			if(pCell)
			{
				if(pCell->sTitle.Find("���ͷ�", 0) != -1)
				{
					nIdx	= atol((char*)(LPCSTR)str) - 1;
				}
			}
		}
	}

	return nIdx;
}

void CInputRebarColumn::OnCheckRadCrosstie() 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	SetGridDataStd();
	SetDataInit();
	DrawInputDomyunView(FALSE);
}