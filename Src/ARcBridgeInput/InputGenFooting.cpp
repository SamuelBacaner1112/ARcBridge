// InputGenFooting.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "../ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "FinitePileDlg.h"
#include "InputGenFooting.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputGenFooting dialog


CInputGenFooting::CInputGenFooting(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputGenFooting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputGenFooting)
	m_bApplyPileConstruct = FALSE;
	//}}AFX_DATA_INIT
	m_bMove			= FALSE;
	m_nTabPos = 0;
	m_nTabPosVert	= -1;
	m_dArrTabPosCross.RemoveAll();

	m_nTab = 0;
	m_nTabPileJewon = -1;
	m_nTabPileProperty = -1;
	m_nTabPHC = -1;
	m_nTabSelect = -1;
	m_nTabFact	= -1;
	m_nTabShapeFact	= -1;
	m_nTabFootingGround = -1;
	m_nTabModeling = -1;

	m_dArrID.Add(IDC_BUTTON_FINITE);
	m_dArrID.Add(IDC_BUTTON_JEWON);
	m_dArrID.Add(IDC_GRID_POS);
	m_dArrID.Add(IDC_TAB_POS);
	m_dArrID.Add(IDC_STATIC_VERT_SU);
	m_dArrID.Add(IDC_COMBO_VERT_SU);
	m_bOutPileControl	= FALSE;
	m_bCheckDist = FALSE;
	m_bSPTDefault = FALSE;
}
CInputGenFooting::~CInputGenFooting()
{
}

void CInputGenFooting::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputGenFooting)
	DDX_Control(pDX, IDC_CHECK_SAMEDIST, m_checkSameDist);
	DDX_Control(pDX, IDC_BUTTON_SAME, m_btnSame);
	DDX_Control(pDX, IDC_BUTTON_FINITE, m_btnFinite);
	DDX_Control(pDX, IDC_BUTTON_JEWON, m_btnJewon);
	DDX_Control(pDX, IDC_BUTTON_FCE, m_btnFce);
	DDX_Control(pDX, IDC_COMBO_KIND_PILE, m_comboKindPile);
	DDX_Control(pDX, IDC_COMBO_KIND_PILE2, m_comboKindPile2);
	DDX_Control(pDX, IDC_COMBO_VERT_SU, m_comboVertSu);
	DDX_Control(pDX, IDC_COMBO_FOOTING, m_comboFooting);
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_comboBridgeNo);
	DDX_Control(pDX, IDC_BUTTON_EXAMINE, m_btnExamine);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_GridControl(pDX, IDC_GRID_POS, m_GridPos);
	DDX_Control(pDX, IDC_TAB_POS, m_TabPos);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	DDX_Check(pDX, IDC_CHECK_PILE_KIND, m_bApplyPileConstruct);
	DDX_Check(pDX, IDC_CHECK_DIST, m_bCheckDist);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_DIST, m_checkDist);
	DDX_Control(pDX, IDC_BUTTON_HYUNTA, m_btnHyunta);
}


BEGIN_MESSAGE_MAP(CInputGenFooting, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputGenFooting)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_CBN_SELCHANGE(IDC_COMBO_FOOTING, OnSelchangeComboFooting)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_CBN_SELCHANGE(IDC_COMBO_VERT_SU, OnSelchangeComboVertSu)
	ON_BN_CLICKED(IDC_BUTTON_EXAMINE, OnButtonExamine)
	ON_CBN_SELCHANGE(IDC_COMBO_KIND_PILE, OnSelchangeComboKindPile)
	ON_CBN_SELCHANGE(IDC_COMBO_KIND_PILE2, OnSelchangeComboKindPile2)
	ON_BN_CLICKED(IDC_BUTTON_FINITE, OnButtonFinite)
	ON_BN_CLICKED(IDC_BUTTON_JEWON, OnButtonJewon)
	ON_BN_CLICKED(IDC_BUTTON_SAME, OnButtonSame)
	ON_BN_CLICKED(IDC_CHECK_SAMEDIST, OnCheckSamedist)
	ON_BN_CLICKED(IDC_CHECK_PILE_KIND, OnCheckPileKind)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_POS, OnTabSelchange)
	ON_BN_CLICKED(IDC_CHECK_DIST, OnCheckDist)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID_POS, OnCellChangedDataPos)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID_POS, OnCellChangedPos)
	ON_BN_CLICKED(IDC_BUTTON_FCE, &CInputGenFooting::OnButtonFce)
	ON_NOTIFY(GVN_CELL_BUTTONCLICK, IDC_GRID, OnCellButtonClick)
	ON_BN_CLICKED(IDC_BUTTON_HYUNTA, &CInputGenFooting::OnBnClickedButtonHyunta)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputGenFooting message handlers


void CInputGenFooting::OnSelchangeComboBridgeNo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_pIn->m_nCurrentBridge = m_comboBridgeNo.GetCurSel();
	SetSizeTabAndGrid();
	InitComboCtrl();
	InitTabCtrl();
	InitTabPosCtrl();
	SetGridData();
	SetDataInit();
	SetPaneTitle();
	SetControl();
	DrawInputDomyunView(TRUE);
	m_comboBridgeNo.SetFocus();
}

void CInputGenFooting::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	structGridCell *pCell = m_Grid.GetCurrentCell();
	
	if(pCell && m_Grid.GetFocus())
	{
		BOOL bUse = FALSE;
		if(m_nTab == m_nTabFootingGround)
		{
			CFootingApp *pFooting = GetFooting();
			if(pFooting)
			{
				if(pFooting->m_Qa_Calculate[0] > 0 && pFooting->m_Qa_Calculate[1] >0)
				{
					m_pIn->SetComment("����ڰ� �Է��� �������� ����մϴ�. ��÷�ڷ�� ������ ÷���ؾ��մϴ�", TRUE);
					bUse = TRUE;
				}
				else bUse = FALSE;

			}
		}
		if(!bUse)
		{
			m_pIn->SetComment(pCell->sTitle);
			m_pView->GetDomyun()->STMakeCursor(m_Grid.GetItemText(0,pCell->nCol));
		}
	}
}

void CInputGenFooting::OnCellChangedPos(NMHDR* pNotifyStruct, LRESULT* result)
{
	structGridCell *pCell = m_GridPos.GetCurrentCell();
	if(pCell && m_GridPos.GetFocus())
	{
		m_pIn->SetComment(pCell->sTitle);
		CString str = m_GridPos.GetItemText(pCell->nRow, 0);
		if(m_nTabPos == m_nTabPosVert)
		{
			str.Replace(" ", "");
			m_pView->GetDomyun()->STMakeCursor(str);
		}
		else
		{
			long nFind = str.Find(" ", 0);
			if(nFind != -1)
			{
				str = str.Right(str.GetLength()-(nFind+1));
				str.Replace(" ", "");
			}
			m_pView->GetDomyun()->STMakeCursor(str);
		}
		
	}
}
void CInputGenFooting::OnSelchangeComboFooting() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DrawInputDomyunView(TRUE);

	SetSizeTabAndGrid();
	InitTabCtrl();
	InitTabPosCtrl();
	//MoveControl();
	SetGridData();
	SetDataInit();
	SetControl();

	
	
	m_comboFooting.SetFocus();	
}


void CInputGenFooting::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	CCellID cell = m_Grid.GetFocusCell();

	CString sHead	= m_Grid.GetTextMatrix(cell.row, 0);
	CString sTitle	= m_Grid.GetTextMatrix(cell.row, 1);

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	CFootingApp *pFooting = GetFooting();
	if(!pFooting) return;

	long nIdxFooting = m_comboFooting.GetCurSel();

	if(m_nTab == m_nTabFootingGround)
	{
		SetDataSave();

		structGridCell *pCell	= m_Grid.GetCurrentCell();
		if(pCell)
		{
			if(pCell->nType == CELL_TYPE_COMBO)
			{
				SetGridData();
				SetDataInit();
			}
		}

		if(sTitle.Find(_T("������ ����")) != -1)
		{
			SetGridData();
			SetDataInit();
		}
		else
		{
			if(sTitle.Find(_T("�ڵ����")) != -1 || sTitle.Find(_T("���������� ���θ�����")) != -1)
			{
				SetGridData();
				SetDataInit();
			}
		}

		if( (sHead.Find(_T("���ݺ������")) != -1 && sTitle.Find(_T("���ݺ������(Eo) ����")) != -1) || (sHead.Find(_T("��������")) != -1 && sTitle.Find(_T("���ݺ������(Eo)")) != -1) )
		{
			if(pBri->m_nSelect_Eo_VertType[nIdxFooting]==2)
			{
				pFooting->m_dEo_Input_Vert = pFooting->m_dBoringEo;
				SetGridData();
				SetDataInit();
			}
		}

		if(sTitle == _T("�������� �ڵ�����"))
		{
			HGINT32 nSelFooting = m_comboFooting.GetCurSel();
			if( nSelFooting < 0 ) return;

			if ( pBri->m_bAutoGroundInfo[nSelFooting] == TRUE )
				pBri->SetDataDefaultBaseGround(nSelFooting);

			SetGridData();
			SetDataInit();
		}
		else if(sTitle == _T("���������� ����"))
		{
			SetGridData();
			SetDataInit();
		}
	}
	else if(m_nTab == m_nTabSelect || m_nTab == m_nTabModeling)
	{
		SetDataSave();
		SetGridData();
		SetDataInit();
	}
	else if(m_nTab == m_nTabPileJewon)
	{
		if(sTitle == _T("�������� �ڵ�����"))
		{
			SetDataSave();

			CExPileApp *pPile = GetPile();
			if(!pPile) return;

			if(pBri->m_bPileNvalueAuto) 
				SetNValueByBoring(GetFooting());
			m_Grid.SetGridDefault(0, 0, 0, 0);
			SetGridDataPileJewon();
			SetDataInit();
		}
// 	// ���� ���û��׿��� "Ⱦ���� ���Ұ��� ��������"�� �����ϸ� �ٽ� �׷��� ��.
// 	if(m_nTab == m_nTabSelect)
// 	{
// 		if(sTitle == "Ⱦ���� ���Ұ��� ��������")
// 		{
// 			CExPileApp *pPile	= GetPile();
// 			if(pPile)
// 			{
// 				SetDataSave();
// 				pPile->ApplySameDist();
// 				DrawInputDomyunView(FALSE);
// 			}
// 		}
// 	}
	// �Ϲ��������� ���ұ��̸� �����ϸ� ��� ���ҿ� ����Ǿ�� �Ѵ�
		if(sTitle == "������ ����")
		{
			CExPileApp *pPile = GetPile();
			if(pPile != NULL)
			{
				long nColLength = 2;
				pPile->m_dLength	= m_Grid.GetTextMatrixDouble(cell.row, nColLength);
				long i = 0; for(i = 0; i < pPile->m_pArrPile.GetSize(); i++)
				{
					CPileAttr *pPileAttr = pPile->m_pArrPile.GetAt(i);
					pPileAttr->len = pPile->m_dLength;
				}
				SetDataInit();
			}
		}
		else if(sTitle == "��ä�� ��ũ��Ʈ ����")
		{
			CExPileApp *pPile = GetPile();
			if(pPile)
			{
				SetDataSave();
				SetEpInnerFill(pPile);
				SetDataInit();
			}
		}
		else if(sTitle == "������ ����")
		{
			SetDataSave();
			CExPileApp *pPile = GetPile();
			if(!pPile) return;

			if(pPile->m_nType == PILE_TYPE_EHP)
			{
				if(pPile->m_nKind_EHP == EHP_KIND_550)
					pPile->m_nKind_EHP_Thick = EHP_THICK_9;
				else if(pPile->m_nKind_EHP == EHP_KIND_700)
					pPile->m_nKind_EHP_Thick = EHP_THICK_9;
				else if(pPile->m_nKind_EHP == EHP_KIND_900)
					pPile->m_nKind_EHP_Thick = EHP_THICK_12;
				else if(pPile->m_nKind_EHP == EHP_KIND_1200)
					pPile->m_nKind_EHP_Thick = EHP_THICK_16;
				else if(pPile->m_nKind_EHP == EHP_KIND_1450)
					pPile->m_nKind_EHP_Thick = EHP_THICK_16;
			}
			SetGridData();
			SetDataInit();
		}
		else if( sTitle == "�Ѱ輱�������� (qt)" || sTitle == "���Ҽ����� ��������")
		{
			SetDataSave();
			SetGridData();
			SetDataInit();
		}
	}
	else if(m_nTab == m_nTabPileProperty)
	{
		if(sTitle.Find(_T("���ݺ������(Eo) ����")) != -1 || sTitle.Find(_T("�ڵ�")) != -1 && sTitle.Find(_T("���")) != -1)
		{
			SetDataSave();
			CExPileApp *pPile = GetPile();
			if(pBri->m_bAutoCalcPileProperty[nIdxFooting] && pPile)
			{
				pPile->m_dKv_Input_Pile = 0;

				for(long i=0; i<3; i++)
				{
					pPile->m_dKh_Input_Pile[i] = 0;
					pPile->m_dBeta_Input_Pile[i] = 0;
				}
			}
			SetGridData();
			SetDataInit();
		}
	}


	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);
}
void CInputGenFooting::OnCellChangedDataPos(NMHDR* pNotifyStruct, LRESULT* result)
{
	SetDataSave();
	CCellID cell = m_GridPos.GetFocusCell();
	CString sTitle	= m_GridPos.GetTextMatrix(0, cell.col);

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	BOOL bEarthAnchor = (pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR) ? TRUE : FALSE;
	if(bEarthAnchor && pBri->m_nCountAnchor == 0) return;

	if(bEarthAnchor == FALSE)
	{
		// �����⸻�ҹ�ġ���� ��簢�� �ٲ�ٸ�.. �ش��ϴ� ���� ��� ��簢�� �ٲ�� �ȴ�.
		if(m_nTabPos == m_nTabPosVert)
		{
			if(sTitle == "��簢")
			{
				CExPileApp *pPile = GetPile();
				if(pPile)
				{
					long nOrg = cell.row-1;
					CPileAttr *pPileAttrStt = pPile->GetPileAttr(nOrg, 0);	// ���翭�� ù��° ����.
					if(pPileAttrStt)
					{
						long i = 0; for(i = 0; i < pPile->m_pArrPile.GetSize(); i++)
						{
							CPileAttr *pPileAttr = pPile->m_pArrPile.GetAt(i);
							if(pPileAttr)
							{
								if(pPileAttr->nIdxOrg == nOrg)
									pPileAttr->ang = pPileAttrStt->ang;
							}
						}
					}
				}
			}
		}
		else if(m_dArrTabPosCross.GetSize() > 0)
		{
			if(m_nTabPos == m_dArrTabPosCross.GetAt(0))
			{
				// ù���� ���� ������ �ٲ�ٸ�.. Ⱦ���� ���ϰ��������� ���ش�.
				if(sTitle == "����")
				{
					CExPileApp *pPile	= GetPile();
					if(pPile)
					{
						pPile->ApplySameDist();
						DrawInputDomyunView(FALSE);
					}
				}
			}
		}
	}
	else
	{
		if(pBri->m_bAnchorSameValue)
		{
			// ��Ŀ Ⱦ�ܹ�ġ�� �����ϰ� ����
			if(m_nTabPos == m_nTabPosVert)
			{
				double dDia = pBri->m_dArrDiaAnchor.GetAt(0);
				double dEa = pBri->m_dArrEaAnchor.GetAt(0);
				double dLen = pBri->m_dArrLengthAnchor.GetAt(0);
				for(long ix = 1; ix < pBri->m_nCountAnchor; ix++)
				{
					pBri->m_dArrDiaAnchor.SetAt(ix, dDia);
					pBri->m_dArrEaAnchor.SetAt(ix, dEa);
					pBri->m_dArrLengthAnchor.SetAt(ix, dLen);
				}
			}
			else
			{
				long nCountSide = (long)(pBri->m_dArrEaAnchor.GetAt(0));
				for(long ix = 1; ix < pBri->m_nCountAnchor; ix++)
				{
					for(long kk = 0; kk < nCountSide; kk++)
					{
						pBri->m_dArrDistHDanAnchor[ix].SetAt(kk, pBri->m_dArrDistHDanAnchor[0].GetAt(kk));
					}
				}
			}
		}
	}

	SetGridData();
	SetDataInit();
	SetControl();
	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);
}

void CInputGenFooting::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CInputDialogBase::OnLButtonDblClk(nFlags, point);
}

void CInputGenFooting::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_pIn->m_strME.bViewMouseEvent)
	{

	}


	CInputDialogBase::OnLButtonDown(nFlags, point);
}

void CInputGenFooting::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if(m_pIn->m_strME.bViewMouseEvent)
	{

		if(m_bMove)
		{
			m_bMove = FALSE;
			DrawInputDomyunView(FALSE);
		}
	}

	CInputDialogBase::OnLButtonUp(nFlags, point);
}

void CInputGenFooting::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CInputDialogBase::OnMouseMove(nFlags, point);
}

void CInputGenFooting::SetGridData()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	CFootingApp *pFooting = GetFooting();
	if(!pFooting)
	{
		m_Grid.SetGridDefault(0, 0, 0, 0);
		m_GridPos.SetGridDefault(0, 0, 0, 0);
		return;
	}
	BOOL bPileBase = (pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_PILE || (pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_PILE)) ? TRUE : FALSE;
	if(bPileBase)
	{
		m_Grid.SetGridDefault(0, 0, 0, 0);
		if(m_nTab == m_nTabPileJewon) 
			SetGridDataPileJewon();
		else if(m_nTab == m_nTabPileProperty) 
			SetGridDataPileProperty();
		else if(m_nTab == m_nTabPHC) 
			SetGridDataPHC();
		else if(m_nTab == m_nTabFootingGround) 
			SetGridDataFootingGround();
		else if(m_nTab == m_nTabModeling) 
			SetGridDataModeling();
		else if(m_nTab == m_nTabSelect) 
			SetGridDataSelect();	
	}
	else
	{
		m_Grid.SetGridDefault(0, 0, 0, 0);
		m_GridPos.SetGridDefault(0, 0, 0, 0);
		if(m_nTab == m_nTabFootingGround) 
			SetGridDataFootingGround();
		else if(m_nTab == m_nTabModeling) 
			SetGridDataModeling();
		else if(m_nTab == m_nTabSelect) 
			SetGridDataSelect();
		else if(m_nTab == m_nTabFact) 
			SetGridDataBaseFactor();
		else if(m_nTab == m_nTabShapeFact) 
			SetGridDataBaseShapeFactor();
	}

	if(bPileBase || (pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR))
	{
		m_GridPos.SetGridDefault(0, 0, 0, 0);
		if(m_nTabPos == m_nTabPosVert) 
		{
			SetGridDataVert();
		}
		else if(m_nTabPos > -1)
		{
			CExPileApp *pPile = GetPile();
			if(pPile || (pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR))
			{
				long nCountSide = pPile ? pPile->m_nCountOrg : pBri->m_nCountAnchor;
				for(long i = 0; i < nCountSide; i++)
				{
					if(m_nTabPos == (long)m_dArrTabPosCross[i]) 
					{
						SetGridDataCross(i);
						break;
					}
				}
			}
		}

	}
}

void CInputGenFooting::SetDataSave()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	m_Grid.UpdateData(TRUE);
	m_GridPos.UpdateData(TRUE);

	// �޺����� ����Ÿ�� ����
	CExPileApp *pPile = GetPile();
	if(pPile)
	{
		BOOL bHyunTa = (pPile->m_nType == PILE_TYPE_HYUNJANG);
		BOOL bPHC	= pPile->m_nType == PILE_TYPE_PHC || pPile->m_nType == PILE_TYPE_PHC_DIGGING;

		// ���� Ÿ�Կ� ���� �Է�â�� �������� ���� �ʱ�ȭ
		if(bHyunTa)
		{
			pPile->m_dT1 = 0;
			pPile->m_dT2 = 0;
		}
		if(bPHC)
			pPile->m_dT2 = 0;
		
		pPile->KeepOrgInfo();
		pPile->KeepPile();
		pPile->SetPileArray();
		pPile->SyncEHP();
	}

	CFootingApp *pFooting = GetFooting();
	if(pFooting)
	{
		if(pBri->m_bPileNvalueAuto) 
			SetNValueByBoring(pFooting);

		if(pBri->IsBoxType() && pPile)
		{
			pBri->SetXyPilePos(pFooting);
		}

		if(pFooting->m_bAutoCalcSupportForce)
		{
			for(long ix = 0; ix < 3; ++ix)
			{
				pFooting->m_Qa_Calculate[ix] = 0;
				pFooting->m_Pa_Calculate[ix] = 0;
			}
		}

		if(m_bDesignLsd && pFooting->m_nTypeBaseGround<2)
		{
			pFooting->m_nTypeEquation_Qa = 0;
		}

		HGINT32 nSelFooting = m_comboFooting.GetCurSel();
		if(pBri->m_bAutoGroundInfo[nSelFooting])
		{
			pBri->SetDataDefaultBaseGround(nSelFooting);
		}
	}

	m_pIn->SetDialogEnable();
}

void CInputGenFooting::SetDataDefault()
{
	for(long bri = 0; bri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		CRcBridgeApp *pBri = m_pIn->m_pARcBridgeDataStd->GetBridge(bri);
		if(!pBri) return;

		BOOL bFirst   = IsFirst(bri);
		pBri->SetDefaultedState(GetIDD(), 0x00000001);
		if(!bFirst && !IsClickDefaultButton(bri)) continue;
		if(bFirst)
			m_bSPTDefault = TRUE;

		if(!bFirst)
		{
			if(IsClickDefaultButton(bri))
			{
				CFootingApp *pFooting = GetFooting();
				long nJ	= m_comboFooting.GetCurSel();
				CBoringData *pBoring = pBri->GetBoringDataJijumBase(nJ);

				if(pFooting)
				{
					if( m_nTab == m_nTabFootingGround )
						pBri->SetDataDefaultBaseGround(nJ, TRUE);

					BOOL bPileBase = (pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_PILE || (pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_PILE)) ? TRUE : FALSE;
					if(bPileBase)
					{
						CExPileApp *pPile = GetPile();
						if(pPile)
						{
							if(m_nTabPileJewon== m_nTab)
							{
								pPile->SetDataDefaultNormal();
								SetNValueByBoring(pFooting);
							}
							else if(m_nTabPHC== m_nTab) pPile->SetDataDefaultPHC();
							else if(m_nTabSelect== m_nTab)
							{
								pBri->SetDataDefaultFootingModeling();
								if(m_bDesignLsd)
									pPile->SetDataDefaultApply(m_bDesignLsd);
							}
							else if(m_nTabFootingGround == m_nTab) pFooting->SetDataDefaultFootingGround();
							pBri->SetDataDefaultPilePos(pFooting);
							CExPileApp *pPile = GetPile();
							long i = 0; 
							for(i = 0; i < pPile->m_pArrPile.GetSize(); i++)
							{
								CPileAttr *pPileAttr = pPile->m_pArrPile.GetAt(i);
								pPileAttr->len = pPile->m_dLength;
							}

							if(pBri->IsBoxType() == TRUE)
								pPile->m_bApplyNonDisplacementPile = FALSE;
						}
					}
					else
					{
						if(m_nTabFact == m_nTab) 
							pFooting->SetDataDefaultBaseFactor();
						else if(m_nTabSelect== m_nTab) 
							pBri->SetDataDefaultFootingModeling();
						else if(m_nTabShapeFact == m_nTab) 
							pFooting->SetDataDefaultBaseShapeFactor();
						else if(m_nTabFootingGround == m_nTab) 
						{
							pFooting->SetDataDefaultFootingGround();
							pFooting->m_nTypeEquation_Qa = 0;
							pFooting->SetDataDefaultBaseFactor();
						}
						if(pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR)
						{
							pBri->SetDataDefaultEarthAnchor(pFooting);
						}
					}
				}
			}
		}
		else
		{
//			long nCountFooting	= pBri->GetCountFooting();
			long nCountJijum = pBri->GetCountJijum();
			for(long i = 0; i < nCountJijum; i++)
			{
				CFootingApp *pFooting = pBri->GetFootingByJijumNumber(i); 
				CBoringData *pBoring = pBri->GetBoringDataJijumBase(i);
				if(pFooting == NULL) continue;

				if(i>0 && pBri->IsBoxType()) break;
				
				pBri->SetDataDefaultBaseGround(i, TRUE);

				BOOL bPileBase = pBri->IsPileBase(i);
				if(bPileBase)
				{
					pFooting->SetDataDefaultFootingGround();
					CExPileApp *pPile = GetPile(i);
					if(pPile)
					{
						pPile->SetDataDefaultNormal();
						pPile->SetDataDefaultPHC();
						pBri->SetDataDefaultFootingModeling();
						pBri->SetDataDefaultPilePos(pFooting);
						for(long i = 0; i < pPile->m_pArrPile.GetSize(); i++)
						{
							CPileAttr *pPileAttr = pPile->m_pArrPile.GetAt(i);
							pPileAttr->len = pPile->m_dLength;
						}
						if(pBri->IsBoxType() == TRUE)
							pPile->m_bApplyNonDisplacementPile = FALSE;
						pPile->SetDataDefaultApply(m_bDesignLsd);
					}

					SetNValueByBoring(pFooting);
					pBri->m_bAutoCalcPileProperty[i] = TRUE;
				}
				else
				{
					pFooting->SetDataDefaultFootingGround();
					pFooting->m_nTypeEquation_Qa = 0;
					pBri->SetDataDefaultFootingModeling();
					if(pBri->IsBoxType() == FALSE)
					{
						pFooting->SetDataDefaultBaseFactor();
						pFooting->SetDataDefaultBaseShapeFactor();
					}
					if(pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR)
					{
						pBri->SetDataDefaultEarthAnchor(pFooting);
					}
					if(m_bDesignLsd && pBoring && pBoring->GetBoringType() == TOSA)
						pFooting->m_nTypeEquation_Qa = 0;
				}
			}
		}

		// ���� ���̴� ���ݿ� ���� �ٽ� �ǰ�� ��. //////////
		long nJ	= m_comboFooting.GetCurSel();
		pBri->SetDataDefaultPileLength(nJ);
		if(pBri->m_bPileNvalueAuto) 
		{
			SetNValueByBoring(pBri->GetFootingByJijumNumber(nJ));
		}
		//pBri->SetDataDefaultPileN(nJ);
		////////////////////////////////////////////////////
	}

	InitTabCtrl();
	InitTabPosCtrl();
	SetGridData();
	SetDataInit();
	SetControl();
}

void CInputGenFooting::InitTabPosCtrl()
{
	m_TabPos.RemoveAllTabs();
	m_TabPos.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_TabPos.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	BOOL bEarthAnchor = (pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR) ? TRUE : FALSE;

	CExPileApp *pPile = GetPile();
	if(!pPile && !bEarthAnchor) return;
	if(pPile && pPile->m_bIs == FALSE) return;
	
	long nTab		= 0;
	CString str		= _T("");
	m_nTabPosVert	= -1;
	m_dArrTabPosCross.RemoveAll();
	m_sArrTabPosName.RemoveAll();

	m_sArrTabPosName.Add("������ ��ġ"); m_nTabPosVert = nTab++;

	long nCount = pPile ? pPile->m_nCountOrg : pBri->m_nCountAnchor;
	for(long i = 0; i < nCount; i++)
	{
		str.Format("D%d", i+1); 
		m_sArrTabPosName.Add(str);
		m_dArrTabPosCross.Add(nTab++);
	}

	for(long i = 0; i < m_sArrTabPosName.GetSize(); i++)
	{
		m_TabPos.InsertItem(i, m_sArrTabPosName[i]);
	}

	if(m_TabPos.GetItemCount() <= m_nTabPos) m_nTabPos = 0;
	m_TabPos.SetCurSel(m_nTabPos);
}

void CInputGenFooting::OnPreInitDialog()
{
	InitComboCtrl();
	SetControl();

	InitTabPosCtrl();
	InitTabCtrl();

	SetSizeTabAndGrid();
	

	// ��������
	SetResize(IDC_BUTTON_EXAMINE,	SZ_BOTTOM_LEFT,	SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_SAME,		SZ_BOTTOM_LEFT,	SZ_BOTTOM_LEFT);
	SetResize(IDC_TAB,				SZ_TOP_LEFT,	SZ_BOTTOM_CENTER);
	SetResize(IDC_GRID,				SZ_TOP_LEFT,	SZ_BOTTOM_CENTER);

	SetResize(IDC_BUTTON_FINITE,	SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_JEWON,		SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_FCE,		SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_HYUNTA,	SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	
	SetResize(IDC_CHECK_SAMEDIST,	SZ_BOTTOM_LEFT,	SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_DIST, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	
	// ��ġ����
	SetResize(IDC_TAB_POS,			SZ_TOP_CENTER,	SZ_BOTTOM_RIGHT);
	SetResize(IDC_GRID_POS,			SZ_TOP_CENTER,	SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_VERT_SU,	SZ_TOP_RIGHT,	SZ_TOP_RIGHT);
	SetResize(IDC_COMBO_VERT_SU,	SZ_TOP_RIGHT,	SZ_TOP_RIGHT);

	SetResize(IDC_STATIC_OPTION,		SZ_TOP_RIGHT,	SZ_BOTTOM_RIGHT);


	SetHyperHelp(m_pIn->GetManualPath());

	m_Grid.InitGrid(m_pIn->m_pARcBridgeDataStd->m_Fy);
	m_GridPos.InitGrid(m_pIn->m_pARcBridgeDataStd->m_Fy);

	SetGridData();
}

void CInputGenFooting::SetDataInit()	
{
	CFootingApp *pFooting = GetFooting();
	if(!pFooting)
	{
		m_pIn->GridMessage(&m_Grid, "���ʰ� ���� ���� �ʽ��ϴ�");
		m_pIn->GridMessage(&m_GridPos, "���ʰ� ���� ���� �ʽ��ϴ�");
		return;
	}
//	if(pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_MASS) ; MASS���� = ��������
//	{
//		m_pIn->GridMessage(&m_Grid, "MASS���ʴ� �Է��� ������ �����ϴ�.");
//		m_pIn->GridMessage(&m_GridPos, "MASS���ʴ� �Է��� ������ �����ϴ�.");
//		return;
//	}

	if(m_bDesignLsd && m_nTab == m_nTabFootingGround)
	{
		m_Grid.SetTextMatrix((int)m_xyButton.y, (int)m_xyButton.x, "����");
		m_Grid.SetCellType((int)m_xyButton.y, (int)m_xyButton.x, CT_BUTTON);
		m_Grid.SetTextMatrix((int)m_xyButtonFactor.y, (int)m_xyButtonFactor.x, "���غ���");
		m_Grid.SetCellType((int)m_xyButtonFactor.y, (int)m_xyButtonFactor.x, CT_BUTTON);
	}

	m_Grid.UpdateData(FALSE);
	m_GridPos.UpdateData(FALSE);

	long nRows = 0;
	long nCols = 0;
	m_Grid.GetCellCount(nRows, nCols);
	if(m_nTab == m_nTabFootingGround || m_nTab == m_nTabSelect || m_nTab == m_nTabPileJewon || m_nTab == m_nTabPileProperty || m_nTab == m_nTabModeling)
	{
		m_Grid.MergeGrid(0, 0, 0, 1);
		if(m_nTab == m_nTabModeling)
		{
			m_Grid.MergeGridEx(1, nRows-1, 0, 1, TRUE, TRUE, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		}
		else
		{
			m_Grid.MergeGridEx(1, nRows-1, 0, 0, FALSE, TRUE);
			m_Grid.MergeGridEx(1, nRows-1, 1, 1, FALSE, TRUE, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			m_Grid.MergeGrid(0,nRows, 2, 3, TRUE,FALSE);
		}
	}

	m_GridPos.SetRedraw(TRUE, TRUE);
	m_Grid.SetRedraw(TRUE, TRUE);
}


void CInputGenFooting::SetControl()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();


	m_comboFooting.EnableWindow(pBri->IsBoxType() == FALSE);

	CExPileApp *pPile = GetPile();
	if(pPile && pPile->m_bIs)
	{
		// ���� ���� �޺�
		m_comboKindPile.EnableWindow(TRUE);
		m_comboKindPile.SetCurSel(pPile->m_nType);

		m_bApplyPileConstruct = pPile->m_bApplyConstruct;

		m_comboKindPile2.EnableWindow(pPile->m_bApplyConstruct);
		m_comboKindPile2.SetCurSel(pPile->m_nTypeConstruct);

		if(pPile->m_nType == PILE_TYPE_EHP)
		{
			m_comboKindPile2.EnableWindow(FALSE);
			m_comboKindPile2.SetCurSel(3);
		}

		// ������ ���� ���� 
		m_comboVertSu.EnableWindow(TRUE);
		m_comboVertSu.SetCurSel(pPile->m_nCountOrg-1);
		GetDlgItem(IDC_STATIC_KIND)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_VERT_SU)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_VERT_SU)->SetWindowText(_T("������ ���� ����"));
		GetDlgItem(IDC_CHECK_SAMEDIST)->SetWindowText(_T("���Ұ��� ��������"));

		// �������� ��ư
		if(pPile->IsPHC()) m_btnJewon.EnableWindow(TRUE);
		else m_btnJewon.EnableWindow(FALSE);

		//PHC �κμջ� ��ư
		if(pPile->m_nType == PILE_TYPE_PHC || pPile->m_nType == PILE_TYPE_PHC_DIGGING) 
			m_btnFce.EnableWindow(TRUE);
		else 
			m_btnFce.EnableWindow(FALSE);

		m_btnFinite.EnableWindow(TRUE);
		m_btnSame.EnableWindow(FALSE);
		BOOL	bHunta = pPile->m_nType == PILE_TYPE_HYUNJANG ? TRUE : FALSE;
		m_btnHyunta.EnableWindow(bHunta);

		m_checkSameDist.EnableWindow(TRUE);
		m_checkSameDist.SetCheck(pPile->m_bApplySameDist);
		pPile->m_nTypeSameDist	= 0;

		m_checkDist.EnableWindow(TRUE);

		UpdateData(FALSE);
	}
	else
	{
		BOOL bEarthAnchor = (pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR) ? TRUE : FALSE;

		m_comboKindPile.SetWindowText("���ұ��ʾƴ�");
		m_comboKindPile.EnableWindow(FALSE);

		m_comboKindPile2.SetWindowText("���ұ��ʾƴ�");
		m_comboKindPile2.EnableWindow(FALSE);

		if(bEarthAnchor)
		{
			m_comboVertSu.EnableWindow(TRUE);
			m_comboVertSu.SetCurSel(pBri->m_nCountAnchor - 1);
			GetDlgItem(IDC_STATIC_VERT_SU)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_VERT_SU)->SetWindowText(_T("������ ��Ŀ ����"));
			m_checkSameDist.EnableWindow(TRUE);
			m_checkSameDist.SetCheck(pBri->m_bAnchorSameValue);
			GetDlgItem(IDC_CHECK_SAMEDIST)->SetWindowText(_T("��Ŀ��ġ ��������"));
		}
		else
		{
			m_comboVertSu.SetWindowText("����");
			m_comboVertSu.EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_VERT_SU)->EnableWindow(FALSE);
			m_checkSameDist.EnableWindow(FALSE);
		}
		GetDlgItem(IDC_STATIC_KIND)->EnableWindow(FALSE);

		// ��ư
		m_btnJewon.EnableWindow(FALSE);
		m_btnFce.EnableWindow(FALSE);
		m_btnFinite.EnableWindow(FALSE);
		m_btnHyunta.EnableWindow(FALSE);


		m_btnSame.EnableWindow(TRUE);
		m_checkDist.EnableWindow(FALSE);
	}
}

void CInputGenFooting::DrawPlanView(CDomyun *pDomP)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	CFootingApp *pFooting = GetFooting();
	if(pFooting == NULL) return;

	CDomyun Dom(pDomP);

	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, m_pIn->m_pARcBridgeDataStd);

	m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	CExPileApp *pPile = GetPile();
	if(pBri->IsBoxType() == FALSE)
	{
		pFooting->GetTvPlan();
		pFooting->m_exFooting.GetTvPlan();
		DBStd.m_pDrawFooting->DrawPlan(&Dom, pFooting, FALSE);
		DBStd.m_pDrawExFooting->DrawPlan(&Dom, &pFooting->m_exFooting, FALSE, FALSE, TRUE, m_bCheckDist);

		// ��ü ��� �׸�
		long nJ	= m_comboFooting.GetCurSel();
		DBStd.m_pDrawPlane->DrawPlaneGenWallByJijum(&Dom, nJ, FALSE);
		if(pPile)
		{
			CDrawExPile drawExPile(pPile, m_pIn->m_pARoadOptionStd);

			long nIdxOrg = m_nTabPos-1;
			if(nIdxOrg > -1) 
				drawExPile.DimPlanSide(&Dom, 0, nIdxOrg, TRUE, TRUE);
			drawExPile.DimPlanOrg(&Dom, 0, TRUE, TRUE);
		}
	}
	else
	{
		// ���� ���
		DBStd.m_pDrawPlane->DrawPlaneCenterLineEachSlab(&Dom);
		DBStd.m_pDrawPlane->DrawPlaneSlabOnlyOutLine(&Dom);
			
		// ��ü �� ��� ���
		int nCountInWall = pBri->GetCountInWall();
		for(long i=0; i<nCountInWall; i++)
		{
			CWallApp *pWall = pBri->GetInWall(i);
			if(pWall == NULL) continue;

			pWall->GetTvPlan();
			DBStd.m_pDrawWall->DrawPlan(&Dom, pWall, FALSE);
			if(pWall->m_bTopIsColumn)
			{
				DBStd.m_pDrawColumn->DrawPlan(&Dom, &pWall->m_Column);
			}
		}

		// ������
		for(long nLR = iLEFT; nLR <= iRIGHT; nLR++)
		{
			pBri->GetTvWingWallPlane(nLR == iLEFT);
			DBStd.m_pDrawWingWall->DrawGenPlane(&Dom, nLR == iLEFT);
		}

		// ���� ���
		DBStd.m_pDrawExFooting->DrawPlan(&Dom, &pFooting->m_exFooting, FALSE, FALSE, TRUE, m_bCheckDist);
	}

	// ���� ���� �������� ȸ��
	DBStd.RotateDomByBridgeSttEndXy(&Dom);

	*pDomP << Dom;
}

void CInputGenFooting::DrawSectionView(CDomyun *pDomP)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	CDomyun Dom(pDomP);

	CFootingApp *pFooting = GetFooting();
	if(pFooting == NULL) return;

	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, m_pIn->m_pARcBridgeDataStd);

//	long nTypeJongdanForRebarOrg = pBri->m_nTypeJongdanForRebar;
//	pBri->m_nTypeJongdanForRebar = (pBri->IsBoxType())? 2 : 0;

	CDRect rectBridge;	// ������ �簢 ����
	BOOL bApplyAngle	= FALSE;
	pBri->GetTvVertSection_Std(0, bApplyAngle, FALSE, FALSE, (pBri->IsBoxType())? 2 : 0);
	rectBridge = pBri->m_tvArrVertSection.GetRect();

	// ���ܸ�
	CDRect rectVert;
	CDPoint xyVert(0, 0);

	// �߰���ü�������̸� �߰���ü�� �׸���
	CWallApp *pWall = GetInWall();
	if(pWall)
	{
		CTwinVectorArray tvTot, tvRect;

		tvTot.AddFromTvArray(pWall->m_tvArrFront);
		if(pWall->m_bTopIsColumn)
			tvTot.AddFromTvArray(pWall->m_Column.m_tvArrFront);
		tvTot.AddFromTvArray(pWall->m_footing.m_tvArrFront);
		tvTot.AddFromTvArray(pWall->m_footing.m_exFooting.m_tvArrFront);
		tvRect.AddFromTvArray(pWall->m_footing.m_tvArrFront);
		tvRect.AddFromTvArray(pWall->m_footing.m_exFooting.m_tvArrFront);

		if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		{
			CTwinVector tv;
			CTwinVectorArray tvArr;
			long n=0; for(n=0; n<tvTot.GetSize(); n++)
			{
				tv = tvTot.GetAt(n);
				if(tv.m_sInfo == _T("��ü�ϸ����") || tv.m_sInfo == _T("��ü�ϸ�����"))
					continue;
				tvArr.Add(tv);
			}
			tvTot.RemoveAll();
			tvTot = tvArr;
		}

		CDRect rect = tvRect.GetRect();
		rect.left -= 200;
		rect.right += 200;
		rect.top  = rectBridge.top + 100;
		rect.bottom -= 100;
		tvTot.AddFromTvArray(pBri->m_tvArrVertSection);


		m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
		DBStd.DrawCuttingTvArray(&Dom, tvTot, rect);
	}
	// ������ü �������̸� ������ü�� �׸���
	else
	{
		CTwinVectorArray tvTot, tvRect;
		tvTot.AddFromTvArray(pBri->m_tvArrVertSection);
		tvTot.AddFromTvArray(pFooting->m_tvArrFront);
		tvTot.AddFromTvArray(pFooting->m_exFooting.m_tvArrFront);
		tvRect.AddFromTvArray(pFooting->m_tvArrFront);
		tvRect.AddFromTvArray(pFooting->m_exFooting.m_tvArrFront);

		if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		{
			// ���ʿ� ��ü�� �̾����� �� ����� �ش�.
			BOOL b1=FALSE, b2=FALSE;
			CTwinVector tvFootingUp, tvFootingExt;

			if(m_comboFooting.GetCurSel()==0)
			{
				b1 = pFooting->m_tvArrFront.GetTvByInfo(_T("���ʻ�����"), tvFootingUp);
				b2 = pFooting->m_tvArrFront.GetTvByInfo(_T("���ʿ���Ȯ����"), tvFootingExt);
			}
			else
			{
				b1 = pFooting->m_tvArrFront.GetTvByInfo(_T("���ʻ������"), tvFootingUp);
				b2 = pFooting->m_tvArrFront.GetTvByInfo(_T("��������Ȯ����"), tvFootingExt);
			}

			CTwinVector tv;
			CTwinVectorArray tvArr;
			long n=0; for(n=0; n<tvTot.GetSize(); n++)
			{
				tv = tvTot.GetAt(n);
				if(tv.m_sInfo == _T("���ʿ������"))
				{
					if(b1 && b2)
					{
						tv.m_v1 = tvFootingUp.GetXyRight();
						tv.m_v2 = pBri->m_tvArrVertSectionRib[0].GetAt(0).m_v1;
						tvArr.Add(tv);
						tv.m_v1 = pBri->m_tvArrVertSectionRib[1].GetAt(0).m_v1;
						tv.m_v2 = tvFootingExt.GetXyLeft();
						tvArr.Add(tv);
						continue;
					}
				}
				else if(tv.m_sInfo == _T("�����������"))
				{
					if(b1 && b2)
					{
						long nRib0 = pBri->m_tvArrVertSectionRib[0].GetSize() - 1;
						long nRib1 = pBri->m_tvArrVertSectionRib[1].GetSize() - 1;

						tv.m_v1 = tvFootingUp.GetXyLeft();
						tv.m_v2 = pBri->m_tvArrVertSectionRib[0].GetAt(nRib0).m_v2;
						tv.m_v1.z	= 0;
						tv.m_v2.z	= 0;
						tvArr.Add(tv);
						tv.m_v2 = pBri->m_tvArrVertSectionRib[1].GetAt(nRib1).m_v2;
						tv.m_v1 = tvFootingExt.GetXyRight();
						tvArr.Add(tv);
						continue;
					}
				}
				tvArr.Add(tv);
			}
			tvTot.RemoveAll();
			tvTot = tvArr;
		}

		CDRect rect = tvRect.GetRect();
		if(m_comboFooting.GetCurSel() == 0)
		{
			rect.left -= 200;
			rect.right = rect.left + (max(pFooting->GetWidth(), pFooting->m_exFooting.GetWidth())+500);
		}
		else
		{
			rect.right += 200;
			rect.left = rect.right - (max(pFooting->GetWidth(), pFooting->m_exFooting.GetWidth())+500);
		}
		rect.top = rectBridge.top + 100;
		rect.bottom -= 100;

		m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
		DBStd.DrawCuttingTvArray(&Dom, tvTot, rect);			
	}

	BOOL bEarthAnchor = (pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR) ? TRUE : FALSE;

	CExPileApp *pPile = GetPile();
	if(bEarthAnchor || pPile)
	{
		CDrawExPile drawExPile(pPile, m_pIn->m_pARoadOptionStd);
		drawExPile.DrawFront(&Dom, FALSE, FALSE, bApplyAngle);
		drawExPile.DimFront(&Dom, 0, TRUE, FALSE, FALSE, TRUE, bApplyAngle);
		drawExPile.DimTextFront(&Dom, TRUE, FALSE, bApplyAngle);
	
		// �������� �׸�
		long nJ	= m_comboFooting.GetCurSel();
		CBoringDataEx *pBoringEx	= pBri->GetBoringDataExByJijumNumber(nJ);
		DBStd.m_pDrawLine->DrawBroing(&Dom, pBoringEx, TRUE);
	}

//	pBri->m_nTypeJongdanForRebar = nTypeJongdanForRebarOrg;
	pBri->GetTvVertSection_Std(0, bApplyAngle, FALSE, TRUE, (pBri->IsBoxType())? 2 : 0);

	*pDomP << Dom;
}

void CInputGenFooting::DrawInputDomyunView(BOOL bZoomAll)
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
 	pDom->SetScaleDim(m_pIn->GetScaleInputDialog());

	CFootingApp *pFooting = GetFooting();
	if(pFooting == NULL) return;

	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, m_pIn->m_pARcBridgeDataStd);

	//////////////////////////////////////////////////////////////////////////
	// ���ܸ�
	CDomyun DomVert(pDom);
	if(pBri->IsBoxType())
		DrawSectionBoxTypeView(&DomVert);
	else
		DrawSectionView(&DomVert);

	DomVert.SetCalcExtRect();
	CDRect rectVert;
	rectVert = DomVert.GetExtRect();

	// ���ݼ� �׸�
	DBStd.m_pDrawVertSection->DrawElByStation(&DomVert, rectVert.left-DomVert.Always(10), rectVert.right+DomVert.Always(10));
	DomVert.SetCalcExtRect();
	rectVert = DomVert.GetExtRect();

	CDPoint xyVert(0, 0);
	xyVert = CDPoint(rectVert.right, rectVert.bottom-rectVert.Height()/2);
	*pDom << DomVert;

	//////////////////////////////////////////////////////////////////////////
	// ���
	CDomyun DomPlan(pDom);
	CDRect rectPlan;
	CDPoint xyPlan(0, 0);
	DrawPlanView(&DomPlan);

	DomPlan.SetCalcExtRect();
	rectPlan = DomPlan.GetExtRect();
	xyPlan = CDPoint(rectPlan.left, rectPlan.bottom-rectPlan.Height()/2);
	DomPlan.Move(CDPoint(xyVert.x-xyPlan.x + 4000, xyVert.y-xyPlan.y));
	*pDom << DomPlan;

	m_pIn->DrawMarginPoint(pDom, 20.0);
	
	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

BOOL CInputGenFooting::IsValid()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	int nCountBri = 0;
	int nSizeBri = pStd->GetBridgeSize();
	for(int nBr=0; nBr<nSizeBri; nBr++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(nBr);

		if(pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB)
			nCountBri++;
	}

	// ������ ������ ��� �����걳�� ��� ��� ��ġ �Է� ������ �޴��� ǥ������ ����
	if(nSizeBri==nCountBri)
		return FALSE;

	return TRUE;
}

void CInputGenFooting::InitComboCtrl()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	CString str = _T("");
	
	// ����
	long nSize = pStd->GetBridgeSize();
	long nCountCombo = m_comboBridgeNo.GetCount();
	long i = 0; for(i = 0; i < nCountCombo; i++)
		m_comboBridgeNo.DeleteString(0);
	for(i = 0; i < nSize; i++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(i);
		str = pStd->GetStringBridgeOrder(i) + _T(" : ") + pBri->m_strBridgeName
			+ _T(" [") + pBri->GetStringBridgeType() + _T("]");
		m_comboBridgeNo.AddString(str);
	}
	m_comboBridgeNo.SetCurSel(m_pIn->m_nCurrentBridge);

	// ������ġ
	long nCount = m_comboFooting.GetCount();
	for(i = 0; i < nCount; i++)
	{
		m_comboFooting.DeleteString(0);
	}

	m_comboFooting.AddString("������");
	if(pBri->IsBoxType() == FALSE)
	{
		for(i = 0; i < pBri->GetCountInWall(); i++)
		{
			str.Format("�߰� ������ %d", i+1);
			m_comboFooting.AddString(str);
		}
		m_comboFooting.AddString("������");
	}
	m_comboFooting.SetCurSel(0);

	// ���� ����
	// ���� ���� �޺� ����(�ڵ�����) ����
	CExPileApp pile;
	nCount = m_comboKindPile.GetCount();
	for(i = 0; i < nCount; i++)
		m_comboKindPile.DeleteString(0);
	for(i = 0; i < PILE_TYPE_SU; i++)
	{
		// �Ѱ���¿��� �������� ���� ����.
		if( i == PILE_TYPE_EHP && m_bDesignLsd == TRUE )
			continue;
		pile.m_nType = i;
		m_comboKindPile.AddString(pile.GetStringPileType());
	}
	//
	nCount = m_comboKindPile2.GetCount();
	for(i = 0; i < nCount; i++)
		m_comboKindPile2.DeleteString(0);
	m_comboKindPile2.AddString("Ÿ�Ը���(Ÿ�ݰ���)");
	m_comboKindPile2.AddString("Ÿ�Ը���(���̺���ظӰ���)");
	m_comboKindPile2.AddString("����Ÿ������");
	m_comboKindPile2.AddString("���α�������");
	m_comboKindPile2.AddString("������������");
	m_comboKindPile2.AddString("���Ͻø�Ʈ����");

	// �ִ� ���� �� �� �����޺�
	nCount = m_comboVertSu.GetCount();
	for(i = 0; i < nCount; i++)
	{
		m_comboVertSu.DeleteString(0);
	}

	long nMax = (pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR) ? RCBRIDGE_ANCHOR_MAX : MAX_PILE_SU;
	for(i = 0; i < nMax; i++)
	{
		str.Format("%d��", i+1);
		m_comboVertSu.AddString(str);
	}

	SetControl();
}

// 
CExPileApp* CInputGenFooting::GetPile(long nIdxFooting)
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return NULL;

	if(nIdxFooting == -1) nIdxFooting = m_comboFooting.GetCurSel();
	if(nIdxFooting < 0) return NULL;

	if(nIdxFooting == 0) 
	{
		if(pBri->IsBoxType() == FALSE)
		{
			if(pBri->IsOutWall(TRUE) && pBri->m_footingStt.m_exFooting.m_nType == EXFOOTING_TYPE_PILE)
			{
				return &pBri->m_footingStt.m_exFooting.m_Pile;
			}
		}
		else
		{
			if(pBri->m_nTypeLower == BOXFOOTING_TYPE_PILE)
			{
				return &pBri->m_footingBox.m_exFooting.m_Pile;
			}
		}
	}
	else if(nIdxFooting < m_comboFooting.GetCount()-1)
	{
		nIdxFooting -= 1;
		if(nIdxFooting <= pBri->GetCountInWall() && nIdxFooting > -1)
		{
			CWallApp *pWall = pBri->GetInWall(nIdxFooting);
			if(pWall)
			{
				if(pWall->m_bIs && pWall->m_footing.m_exFooting.m_nType == EXFOOTING_TYPE_PILE)
					return &pWall->m_footing.m_exFooting.m_Pile;
			}
		}

	}
	else
	{
		if(pBri->IsOutWall(TRUE) && pBri->m_footingEnd.m_exFooting.m_nType == EXFOOTING_TYPE_PILE)
			return &pBri->m_footingEnd.m_exFooting.m_Pile;
	}

	return NULL;


}

// �Ϲ����� �Է³���
void CInputGenFooting::SetGridDataPileJewon()
{
	CExPileApp *pPile = GetPile();
	if(!pPile) return;

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;

	CStringArray sArr;
	CString str = _T("");
	CString strHelp = _T("");
	long row = 0;
	long col = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;

	BOOL bHyunTa = (pPile->m_nType == PILE_TYPE_HYUNJANG);
	BOOL bPHC	= pPile->m_nType == PILE_TYPE_PHC || pPile->m_nType == PILE_TYPE_PHC_DIGGING; // 20050322 added by jaeho
	BOOL bEHP	= pPile->m_nType == PILE_TYPE_EHP;
	BOOL bSteel	= (pPile->m_nType == PILE_TYPE_STEEL || pPile->m_nType == PILE_TYPE_STEEL_DIGGING) ;
	BOOL bPileASD = (pStd->IsApplyBoredPileASD(pPile) || pStd->IsApplyPhcPileASD(pPile))? TRUE : FALSE; // ���Ը��� �Ǵ� PHC������ ������¼���� ���� 

	m_Grid.InitGrid(pPile->m_dRebar_Fy);
	m_Grid.SetGridDefault(2, 2, 1, 2);
	m_Grid.SetCellSize(20, -1);

	m_Grid.AddCell(row, col++, "��  ��"); 
	m_Grid.AddCell(row, col++, "��  ��"); 
	m_Grid.AddCell(row, col++, "������"); 
	m_Grid.AddCell(row, col++, "��  ��");
	row++;
	col= 0;

	CString szUnit_CGS   = "";
	CString szUnit_Apply = "mm";

	CString sHead = _T("��������");
	m_Grid.AddCell(row, col++, sHead); 
	if(bEHP)
	{
		long i = 0; for(i = 0; i < EHP_KIND_SU; i++)
			sArr.Add(pPile->GetStringEHPType(i));

		m_Grid.AddCell(row, col++, "������ ����", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pPile->m_nKind_EHP, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
		m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	}
	else
	{
		m_Grid.AddCell(row, col++, "������ ����", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pPile->m_dDia, DT_RIGHT, nLine, "pD : PILE�� ����(mm)", CELL_TYPE_AUTO);	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
		m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	}
	row++;
	col= 0;

	m_Grid.AddCell(row, col++, sHead); 
	m_Grid.AddCell(row, col++, "������ ����", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pPile->m_dLength, DT_RIGHT, nLine, "pL : PILE�� ����(mm)", CELL_TYPE_AUTO);	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
	m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	row++;
	col= 0;
	// 2005-01-18 11:38���� ===>
	if(!bHyunTa)
	{
		if(bEHP)
		{
			sArr.RemoveAll();
			pPile->GetStringArrayEHPThick(sArr);
			m_Grid.AddCell(row, col++, sHead); 
			m_Grid.AddCell(row, col++, "������ �β�", GVHT_LEFT); 
			m_Grid.AddCell(row, col++, &pPile->m_nKind_EHP_Thick, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
			if(pPile->m_nKind_EHP == EHP_KIND_1200 || pPile->m_nKind_EHP == EHP_KIND_1450)
				m_Grid.SetCellBaseOfIndex(3);
			m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
			m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
			row++;
			col= 0;
		}
		else
		{
			m_Grid.AddCell(row, col++, sHead); 
			m_Grid.AddCell(row, col++, "������ �β�", GVHT_LEFT); 
			m_Grid.AddCell(row, col++, &pPile->m_dT1, DT_RIGHT, nLine, "pT : PILE�� �β�(mm)", CELL_TYPE_AUTO); m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
			m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
			row++;
			col= 0;
		}

		if(!bPHC)
		{
			m_Grid.AddCell(row, col++, sHead); 
			m_Grid.AddCell(row, col++, "������ �νĵβ�", GVHT_LEFT); 
			m_Grid.AddCell(row, col++, &pPile->m_dT2, DT_RIGHT, nLine, "cT : PILE�� �νĵβ�(mm)", CELL_TYPE_AUTO); m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
			m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
			row++;
			col= 0;
		}
	}
	// 2005-01-18 11:38���� <===

	sHead = _T("���Ҽ�ġ����");
	m_Grid.AddCell(row, col++, sHead); 
	m_Grid.AddCell(row, col++, "���Ҽ����� ���Ա���", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pPile->m_dDf, DT_RIGHT, nLine, "Df : PILE������ ���Ա���(mm)", CELL_TYPE_AUTO); m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
	m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	row++;
	col= 0;

	m_Grid.AddCell(row, col++, sHead); 
	m_Grid.AddCell(row, col++, "���������κ��� ����� ����", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pPile->m_dH0, DT_RIGHT, nLine, "Ho : ���������κ��� ����� ����(mm)", CELL_TYPE_AUTO);	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
	m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	row++;
	col= 0;

	nLine = m_bDesignLsd && pPile->m_nType == PILE_TYPE_PHC ? CELL_READONLY : CELL_LINE_NO;
	sHead = _T("��������Ư��");
	m_Grid.AddCell(row, col++, sHead); 
	m_Grid.AddCell(row, col++, "�������� �ڵ�����", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pBri->m_bPileNvalueAuto, DT_RIGHT, nLine, pBri->m_bPileNvalueAuto ? _T("����") : _T("�������"), CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
	row++;
	col= 0;

	nLine = CELL_LINE_NO;
	UINT nLineAuto = pBri->m_bPileNvalueAuto ? CELL_READONLY : CELL_LINE_NO;
	m_Grid.AddCell(row, col++, sHead); 
	m_Grid.AddCell(row, col++, "���ҸӸ��� ��� Nġ", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pPile->m_dBeta_N, DT_RIGHT, nLineAuto, "bN : PILE�Ӹ��� ��� Nġ", CELL_TYPE_AUTO);
	m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
	row++;
	col= 0;

	m_Grid.AddCell(row, col++, sHead); 
	m_Grid.AddCell(row, col++, "���Ҽ����� ��� Nġ", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pPile->m_dEdge_N, DT_RIGHT, nLineAuto, "eN : PILE������ ��� Nġ", CELL_TYPE_AUTO);
	m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
	row++;
	col= 0;

	if(m_bDesignLsd == FALSE)
	{
		szUnit_CGS   = "";
		szUnit_Apply = "mm";
		m_Grid.AddCell(row, col++, sHead); 
		m_Grid.AddCell(row, col++, "�й����� �β�", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pPile->m_dThickApmeel, DT_RIGHT, CELL_LINE_NO, "aT : �й����� �β�(mm)", CELL_TYPE_AUTO);	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
		m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
		row++;
		col= 0;

		sArr.RemoveAll();
		szUnit_CGS   = "kgf/cm��";
		szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
		sHead = _T("�κκ���");
		m_Grid.AddCell(row, col++, sHead); 
		m_Grid.AddCell(row, col++, "�κκ���ö�� �׺�����", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pPile->m_dRebar_Fy, DT_RIGHT, CELL_LINE_NO, "�κκ���ö�� �׺�����", CELL_TYPE_AUTO); m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
		m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
		row++;
		col= 0;

		m_Grid.AddCell(row, col++, sHead); 
		m_Grid.AddCell(row, col++, "�κκ���ö�� ����", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pPile->m_dRebar_Dia, DT_RIGHT, CELL_LINE_NO, "", CELL_TYPE_DIA);
		m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
		row++;
		col= 0;

		m_Grid.AddCell(row, col++, sHead); 
		m_Grid.AddCell(row, col++, "�κκ���ö�� ����", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pPile->m_dRebar_Ea, DT_RIGHT, CELL_LINE_NO, "", CELL_TYPE_AUTO);
		m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
		row++;
		col= 0;
	}

	if( bSteel == TRUE )
	{
		szUnit_Apply = "N/mm��";
		sHead = _T("��������");
		if(m_bDesignLsd == FALSE || bPileASD)
		{
			m_Grid.AddCell(row, col++, sHead); 
			m_Grid.AddCell(row, col++, "���������� ��� ������", GVHT_LEFT); 
			m_Grid.AddCell(row, col++, &pPile->m_dFa_Steel, DT_RIGHT, CELL_LINE_NO, "���������� ��� ������", CELL_TYPE_AUTO);	m_Grid.SetCellUnit(UNIT_CGS_KGF_CM2,pStd->m_szKgf_CM2); 
			m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
			row++;
			col= 0;

			m_Grid.AddCell(row, col++, sHead); 
			m_Grid.AddCell(row, col++, "���������� ��� ��������", GVHT_LEFT); 
			m_Grid.AddCell(row, col++, &pPile->m_dTa_Steel, DT_RIGHT, CELL_LINE_NO, "���������� ��� ��������", CELL_TYPE_AUTO); m_Grid.SetCellUnit(UNIT_CGS_KGF_CM2,pStd->m_szKgf_CM2); 
			m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
			row++;
			col= 0;
		}

		if( m_bDesignLsd == TRUE && bPileASD == FALSE)
		{
			m_Grid.AddCell(row, col++, sHead); 
			m_Grid.AddCell(row, col++, "���������� �׺�����(Fy)", GVHT_LEFT); 
			m_Grid.AddCell(row, col++, &pPile->m_dYieldStress_SteelPile, DT_RIGHT, CELL_LINE_NO, "���������� �׺�����(Fy)", CELL_TYPE_AUTO); m_Grid.SetCellUnit(UNIT_CGS_KGF_CM2, pStd->m_szKgf_CM2); 
			m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
			row++;
			col= 0;
		}
	}

	if(bEHP && m_bDesignLsd == FALSE)
	{
		szUnit_CGS   = "kgf/cm��";
		szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);

		sHead = _T("EHP");
		m_Grid.AddCell(row, col++, sHead); 
		m_Grid.AddCell(row, col++, "��ä�� ��ũ��Ʈ ����", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pPile->m_dFck_InnerFill, DT_RIGHT, nLine, "Fck : ��ũ��Ʈ ������� ����", CELL_TYPE_AUTO);	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
		m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
		row++;
		col= 0;

		m_Grid.AddCell(row, col++, sHead); 
		m_Grid.AddCell(row, col++, "��ä�� ��ũ��Ʈ ź�����", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pPile->m_dEp_InnerFill, DT_RIGHT, nLine, "Ep : ��ũ��Ʈ ź�����", CELL_TYPE_AUTO); m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
		m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
		row++;
		col= 0;
	}

	long nWidth1 = 250;
	long nWidth2 = 100;

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(100);
	m_Grid.SetColumnWidth(1, nWidth1);
	m_Grid.SetColumnWidth(2, nWidth2);
	m_Grid.SetColumnWidth(3, 80);
}

void CInputGenFooting::SetGridDataPileProperty()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	CFootingApp *pFooting = GetFooting();
	if(!pFooting) return;

	CExPileApp *pPile = GetPile();
	if(!pPile) return;

	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;
	if(!pStd) return;

	CStringArray sArr;
	CString str = _T("");
	CString strHelp = _T("");
	long row = 0;
	long col = 0;

	BOOL bLSD = pStd->IsLsdDesign();
	UINT nFormat = DT_RIGHT | DT_VCENTER;

	long nIdxFooting = m_comboFooting.GetCurSel();

	m_Grid.SetGridDefault(2, 2, 1, 2);
	m_Grid.SetCellSize(20, -1);

	m_Grid.AddCell(row, col++, _T("��  ��")); 
	m_Grid.AddCell(row, col++, _T("��  ��")); 
	m_Grid.AddCell(row, col++, _T("������"));
	m_Grid.AddCell(row, col++, _T("�� ��"));
	row++;
	col	= 0;

	CString sHead = _T("����Ư��ġ");
	m_Grid.AddCell(row, col++, sHead);
	m_Grid.AddCell(row, col++, _T("�ڵ����"), GVHT_LEFT);
	str = pBri->m_bAutoCalcPileProperty[nIdxFooting] ? _T("����") : _T("�������");
	m_Grid.AddCell(row, col++, &pBri->m_bAutoCalcPileProperty[nIdxFooting], DT_RIGHT, CELL_LINE_NO, str, CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
	row++;
	col	= 0;

	UINT nLineProperty = pBri->m_bAutoCalcPileProperty[nIdxFooting] ? CELL_READONLY : CELL_LINE_NO;

	CString szUnit_CGS   = _T("tonf/m");	
	CString szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS); 
	m_Grid.AddCell(row, col++, sHead);
	m_Grid.AddCell(row, col++, _T("������ ����� ���������� (Kv) : 0 = �ڵ����"), GVHT_LEFT);
	m_Grid.AddCell(row, col++, &pPile->m_dKv_Input_Pile, DT_RIGHT, nLineProperty);	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.SetCellDec(bLSD ? CELL_DEC_5 : CELL_DEC_3);
	m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	row++;
	col	= 0;

	szUnit_CGS   = _T("kgf/cm��");
	szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS); 
	long nIdx = bLSD ? PILE_CASE_ULTIMATE : PILE_CASE_NORMAL;
	m_Grid.AddCell(row, col++, sHead);
	m_Grid.AddCell(row, col++, _T("������� ���ݹݷ� ��� (Kh, ���) : 0 = �ڵ����"), GVHT_LEFT);
	m_Grid.AddCell(row, col++, &pPile->m_dKh_Input_Pile[nIdx], DT_RIGHT, nLineProperty);	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.SetCellDec(CELL_DEC_5);
	m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	row++;
	col	= 0;

	nIdx = bLSD ?  PILE_CASE_EXTREME : PILE_CASE_EARTH;
	m_Grid.AddCell(row, col++, sHead);
	m_Grid.AddCell(row, col++, _T("������� ���ݹݷ� ��� (Kh, ������) : 0 = �ڵ����"), GVHT_LEFT);
	m_Grid.AddCell(row, col++, &pPile->m_dKh_Input_Pile[nIdx], DT_RIGHT, nLineProperty);	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.SetCellDec(CELL_DEC_5);
	m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	row++;
	col	= 0;

	// m-1�� ���� ��ȯ�� �ȵǹǷ� ������ �Ųٷ� ���.
	szUnit_CGS		= bLSD ? _T("mm") : _T("m");
	szUnit_Apply	= _T("m");
	nIdx = bLSD ? PILE_CASE_ULTIMATE : PILE_CASE_NORMAL;
	m_Grid.AddCell(row, col++, sHead);
	m_Grid.AddCell(row, col++, _T("������ ź��ġ (��,���) : 0 = �ڵ����"), GVHT_LEFT);		
	m_Grid.AddCell(row, col++, &pPile->m_dBeta_Input_Pile[nIdx], DT_RIGHT, nLineProperty); m_Grid.SetCellDec(CELL_DEC_5);	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(row, col++, bLSD ? _T("mm-1") : _T("m-1"), GVHT_LEFT, CELL_READONLY);
	m_Grid.SetCellDec(CELL_DEC_5);
	row++;
	col	= 0;

	szUnit_CGS		= bLSD ? _T("mm") : _T("m");
	szUnit_Apply	= _T("m");
	nIdx = bLSD ?  PILE_CASE_EXTREME : PILE_CASE_EARTH;
	m_Grid.AddCell(row, col++, sHead);
	m_Grid.AddCell(row, col++, _T("������ ź��ġ (��,������) : 0 = �ڵ����"), GVHT_LEFT);
	m_Grid.AddCell(row, col++, &pPile->m_dBeta_Input_Pile[nIdx], DT_RIGHT, nLineProperty); m_Grid.SetCellDec(CELL_DEC_5);	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(row, col++, bLSD ? _T("mm-1") : _T("m-1"), GVHT_LEFT, CELL_READONLY);
	m_Grid.SetCellDec(CELL_DEC_5);
	row++;
	col	= 0;

	m_Grid.AddCell(row, col++, sHead);
	m_Grid.AddCell(row, col++, _T("�������(��) ���"), GVHT_LEFT);
	m_Grid.AddCell(row, col++, &pFooting->m_dFactorAlpha[0], nFormat);
	m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
	row++;
	col	= 0;

	m_Grid.AddCell(row, col++, sHead);
	m_Grid.AddCell(row, col++, _T("�������(��) ������"), GVHT_LEFT);
	m_Grid.AddCell(row, col++, &pFooting->m_dFactorAlpha[1], nFormat);
	m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
	row++;
	col	= 0;

	sArr.RemoveAll();
	sArr.Add(_T("�Է°� x N"));
	sArr.Add(_T("�Է°�"));
	//	sArr.Add(_T("�������� ����"));
	m_Grid.AddCell(row, col++, sHead);
	m_Grid.AddCell(row, col++, _T("���� ���ݺ������(Eo) ����"), GVHT_LEFT);
	m_Grid.AddCell(row, col++, &pBri->m_nSelect_Eo_HoriType[nIdxFooting], DT_RIGHT, TRUE, CELL_LINE_NO, _T(""), CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
	row++;
	col= 0;

	szUnit_CGS	 = _T("kgf/cm��");
	szUnit_Apply = pStd->IsSI() ? pStd->m_szTonf_M2 : pStd->m_szKgf_CM2;

	m_Grid.AddCell(row, col++, sHead);
	m_Grid.AddCell(row, col++, _T("���� ���ݺ������(Eo)"), GVHT_LEFT);
	if(pBri->m_nSelect_Eo_HoriType[nIdxFooting] == 1)
		m_Grid.AddCell(row, col++, &pFooting->m_dEo_Input_Hori, nFormat, pBri->m_nSelect_Eo_HoriType[nIdxFooting] == 0 ? CELL_READONLY : CELL_LINE_NO); 
	else
		m_Grid.AddCell(row, col++, &pFooting->m_dNValue_Eo_Hori, nFormat);
	m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply); m_Grid.SetCellDec(bLSD ? CELL_DEC_3 : CELL_DEC_0);
	m_Grid.AddCell(row, col++, pBri->m_nSelect_Eo_HoriType[nIdxFooting] == 1 ? szUnit_Apply : _T("x N"), GVHT_LEFT, CELL_READONLY);

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidth(0,100);
	m_Grid.SetColumnWidth(1,350);
	m_Grid.SetColumnWidth(2,120);
	m_Grid.SetColumnWidth(3,80);
}

// PHC ���� �Է³���
void CInputGenFooting::SetGridDataPHC()
{
	CExPileApp *pPile = GetPile();
	if(!pPile) return;

	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;

	CStringArray sArr;
	CString str = _T("");
	CString strHelp = _T("");
	long row = 0;
	long col = 0;
	// (ARCBRIDGE-2713) PHC���� ���� �Է� ���
	UINT nLine = CELL_LINE_NO;	// m_bDesignLsd? CELL_READONLY : CELL_LINE_NO;

	m_Grid.SetGridDefault(10, 3, 1, 1);
	m_Grid.SetCellSize(20, -1);

	CString szUnit_CGS   = "kgf/cm��";
	CString szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);

	m_Grid.AddCell(row, col, "��  ��"); m_Grid.AddCell(row, col+1, "������"); m_Grid.AddCell(row++, col+2, "��  ��");
	m_Grid.AddCell(row, col, "��ũ��Ʈ ������� ����", GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dFck_PHC, DT_RIGHT, nLine, "Fck : ��ũ��Ʈ ������� ����", CELL_TYPE_AUTO);
	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);

	m_Grid.AddCell(row, col, "��ũ��Ʈ ź�����", GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dEp_PHC, DT_RIGHT, nLine, "Ep : ��ũ��Ʈ ź�����", CELL_TYPE_AUTO);
	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);

	if(pPile->m_nType == PILE_TYPE_PHC || pPile->m_nType == PILE_TYPE_PHC_DIGGING)
	{
		m_Grid.AddCell(row, col, _T("��ȿ������Ʈ����(��ü)"), GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dFce1_PHC, DT_RIGHT, nLine, _T("Fce1 : ��ȿ������Ʈ����(��ü)"), CELL_TYPE_AUTO);
		m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);

		CString strMsg = _T("�ð��� ���ҵκθ� ����,�ļ����� �ʰų� �������� ������� �κ��� ��ȿ������Ʈ������ ���Ҹ� �����ϴ� ���� �������� ����� �Ǵ����� ��ȿ������Ʈ����(�κ�) ���� �����Ͻñ� �ٶ��ϴ�.");
		m_Grid.AddCell(row, col, _T("��ȿ������Ʈ����(�κ�)"), GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dFce2_PHC, DT_RIGHT, nLine, strMsg, CELL_TYPE_AUTO);
		m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);

		m_Grid.AddCell(row, col, _T("�κ��� ����(���һ�ܱ���)"), GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dLenHead_PHC, DT_RIGHT, nLine, _T("�κ��� ����(���һ�ܱ���)"), CELL_TYPE_AUTO);
		m_Grid.SetCellUnit(_T("mm"), UNIT_CGS_CM); m_Grid.AddCell(row++, col+2, UNIT_CGS_CM, GVHT_LEFT, CELL_READONLY);

	}
	else
	{
		// ���� PHC �Ǵ� SPC
		m_Grid.AddCell(row, col, _T("��ȿ������Ʈ����(��ü)"), GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dFce1_PHC, DT_RIGHT, nLine, _T("Fce : ��ȿ������Ʈ����"), CELL_TYPE_AUTO);
		m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);

	}

	szUnit_CGS		= "cm��";
	szUnit_Apply	= pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(row, col, "��ũ��Ʈ ȯ��ܸ���", GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dAe_PHC, DT_RIGHT, nLine, "Ae : ��ũ��Ʈ ȯ��ܸ���", CELL_TYPE_AUTO);
	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	
	szUnit_CGS		= "cm��";
	szUnit_Apply	= pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(row, col, "ȯ��ܸ� 2�� ���Ʈ", GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dIe_PHC, DT_RIGHT, nLine, "Ie : ȯ��ܸ� 2�� ���Ʈ", CELL_TYPE_AUTO);
	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);

	szUnit_CGS		= "cm��";
	szUnit_Apply	= pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(row, col, "ȯ��ܸ���", GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dZe_PHC, DT_RIGHT, nLine, "Ze : ȯ��ܸ���", CELL_TYPE_AUTO);
	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);

	sArr.RemoveAll();  sArr.Add("A"); sArr.Add("B"); sArr.Add("C"); 
	m_Grid.AddCell(row, col, "����", GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_nKind_PHC, DT_RIGHT, TRUE, nLine, "Type : ����", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+2, _T(""), GVHT_LEFT, CELL_READONLY);

	szUnit_CGS   = "kgf/cm��";
	szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(row, col, "��� �� ��������(����)", GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dFca_PHC[0], DT_RIGHT, nLine, "Fca1 : ��� �� ��������(����)", CELL_TYPE_AUTO);
	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);

	szUnit_CGS   = "kgf/cm��";
	szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(row, col, "��� �� ��������(������)", GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dFca_PHC[1], DT_RIGHT, nLine, "Fca2 : ��� �� ��������(������)", CELL_TYPE_AUTO);
	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);

	szUnit_CGS   = "kgf/cm��";
	szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(row, col, "��� �� ��������(����)", GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dFta_PHC[0], DT_RIGHT, nLine, "Fta1 : ��� �� ��������(����)", CELL_TYPE_AUTO);
	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);

	szUnit_CGS   = "kgf/cm��";
	szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(row, col, "��� �� ��������(������)", GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dFta_PHC[1], DT_RIGHT, nLine, "Fta2 : ��� �� ��������(������)", CELL_TYPE_AUTO);
	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);

	szUnit_CGS   = "kgf/cm��";
	szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(row, col, "��� ��������(����)", GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dTa_PHC[0], DT_RIGHT, nLine, "Ta1 : ��� ��������(����)", CELL_TYPE_AUTO);
	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	
	szUnit_CGS   = "kgf/cm��";
	szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(row, col, "��� ��������(������)", GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dTa_PHC[1], DT_RIGHT, nLine, "Ta2 : ��� ��������(������)", CELL_TYPE_AUTO);
	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	
	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(100);
	m_Grid.SetColumnWidth(0, 200);
}

BOOL CInputGenFooting::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		UpdateData();
		if(m_Grid.GetFocusCell().col == m_Grid.GetCols()-1 || m_GridPos.GetFocusCell().col == m_GridPos.GetCols()-1)
			pMsg->wParam = VK_RIGHT;
		else
			pMsg->wParam = VK_TAB;
	}

	if(pMsg->hwnd == m_Grid.GetSafeHwnd() && pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK)
	{
		CCellID next = m_Grid.GetFocusCell();		
		CFootingApp *pFooting = GetFooting();
		if(pFooting)
		{
			if(pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_LEAN || pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_MASS)
			{
				if(m_nTab == m_nTabFact)
				{
					if(next.col==0) m_dlgBaseFact.m_nType = 0;
					if(next.col==1) m_dlgBaseFact.m_nType = 1;
					if(next.col==2) m_dlgBaseFact.m_nType = 2;
					if(next.col==3) m_dlgBaseFact.m_nType = 3;
					if(next.col==4) m_dlgBaseFact.m_nType = 4;
				}
				if(m_nTab == m_nTabShapeFact)
				{
					if(next.col==0) m_dlgBaseShapeFact.m_nType = 0;
					if(next.col==1) m_dlgBaseShapeFact.m_nType = 1;
					if(next.col==2) m_dlgBaseShapeFact.m_nType = 2;
					if(next.col==3) m_dlgBaseShapeFact.m_nType = 3;
					if(next.col==4) m_dlgBaseShapeFact.m_nType = 4;
					if(next.col==5) m_dlgBaseShapeFact.m_nType = 5;
				}
			}
		}
	}		
	return CInputDialogBase::PreTranslateMessage(pMsg);
}

//  ������ ���� ��ġ �׸��� ����Ÿ
void CInputGenFooting::SetGridDataVert()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(pBri == NULL) return;

	BOOL bEarthAnchor = (pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR) ? TRUE : FALSE;

	CExPileApp *pPile = GetPile();
	if(!bEarthAnchor && (!pPile || pPile->m_bIs == FALSE)) return;

	CStringArray sArr;
	CString str = _T("");
	CString strHelp = _T("");
	long row = 0;
	long col = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;

	m_GridPos.SetGridDefault(2, 2, 1, 1);
	m_GridPos.SetCellSize(20, -1);

	if(bEarthAnchor == FALSE)
	{
		// ���ظ鿡 �ִ� �� ������ ù��° ������ ���� ǥ��
		m_GridPos.AddCell(row, col++, "����");
		m_GridPos.AddCell(row, col++, "����");
		m_GridPos.AddCell(row, col++, "����");
		m_GridPos.AddCell(row, col, "��簢");
		row++;
		col = 0;

		long i = 0; for(i = 0; i < pPile->m_nCountOrg; i++)
		{
			COrgInfo *pOrg = pPile->m_pArrOrgInfo.GetAt(i);
			CPileAttr *pPileAttr = pPile->GetPileAttr(i, 0);
			str.Format("D%d", i+1);

			if(pPileAttr && pOrg)
			{
				m_GridPos.AddCell(row, col++, str); 
				m_GridPos.AddCell(row, col++, &pOrg->countSide, nFormat, FALSE, nLine, str+"�� ��ġ�Ǵ� ������ ����", CELL_TYPE_AUTO);
				m_GridPos.AddCell(row, col++, &pOrg->dist, nFormat, nLine, str+"�� ���������� ����", CELL_TYPE_AUTO);
				m_GridPos.AddCell(row, col, &pPileAttr->ang, nFormat, nLine, str+"�� ��ġ�Ǵ� ������ ��� ����", CELL_TYPE_AUTO);
				row++;
				col = 0;
			}
		}
	}
	else
	{
		// ���ظ鿡 �ִ� �� ������ ù��° ������ ���� ǥ��
		m_GridPos.AddCell(row, col++, "����");
		m_GridPos.AddCell(row, col++, "�Ÿ�");
		m_GridPos.AddCell(row, col++, "����");
		m_GridPos.AddCell(row, col++, "����");
		m_GridPos.AddCell(row, col++, "����");
		m_GridPos.AddCell(row, col, "��Ŀ��");
		row++;
		col = 0;

		for(long ix = 0; ix < pBri->m_nCountAnchor; ix++)
		{
			str.Format("D%d", ix+1);

			m_GridPos.AddCell(row, col++, str); 
			m_GridPos.AddCell(row, col++, &pBri->m_dArrDistJongAnchor, ix, nFormat, nLine, str+"�� ���������� ����", CELL_TYPE_AUTO); m_GridPos.SetCellDec(CELL_DEC_1);
			m_GridPos.AddCell(row, col++, &pBri->m_dArrEaAnchor, ix, nFormat, (ix == 0 || pBri->m_bAnchorSameValue == FALSE) ?  nLine : CELL_READONLY, str+"�� ��ġ�Ǵ� Anchor�� ����", CELL_TYPE_AUTO);
			m_GridPos.AddCell(row, col++, &pBri->m_dArrDiaAnchor, ix, nFormat, (ix == 0 || pBri->m_bAnchorSameValue == FALSE) ?  nLine : CELL_READONLY, str+"�� ��ġ�Ǵ� Anchor�� ����", CELL_TYPE_AUTO);
			m_GridPos.AddCell(row, col++, &pBri->m_dArrLengthAnchor, ix, nFormat, (ix == 0 || pBri->m_bAnchorSameValue == FALSE) ?  nLine : CELL_READONLY, str+"�� ��ġ�Ǵ� Anchor�� ����", CELL_TYPE_AUTO);

			CString szUnit_CGS   = "tonf";
			CString szUnit_Apply = m_pIn->m_pARcBridgeDataStd->GetStringUnitType(szUnit_CGS);
			m_GridPos.AddCell(row, col, &pBri->m_dArrLoadAnchor, ix, nFormat, nLine, str+"�� ����Ǵ� Anchor ����", CELL_TYPE_AUTO);
			m_GridPos.SetCellUnit(szUnit_CGS,szUnit_Apply);
			m_GridPos.SetCellDec(CELL_DEC_1);
			row++;
			col = 0;
		}
	}


	m_GridPos.GetCellCount(row, col);
	m_GridPos.SetRows(row);
	m_GridPos.SetCols(col);
	m_GridPos.SetColumnWidthAll(60);
}

BOOL CInputGenFooting::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	return CInputDialogBase::DestroyWindow();
}

void CInputGenFooting::OnSelchangeComboVertSu() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	BOOL bEarthAnchor = (pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR) ? TRUE : FALSE;

	if(bEarthAnchor == FALSE)
	{
		CExPileApp *pPile = GetPile();
		if(pPile) pPile->m_nCountOrg = m_comboVertSu.GetCurSel() + 1;
	}
	else
	{
		pBri->m_nCountAnchor = m_comboVertSu.GetCurSel() + 1;
	}

	InitTabPosCtrl();
	SetDataSave();
	OnCheckSamedist();
	SetGridData();
	SetDataInit();
	DrawInputDomyunView(TRUE);	
}

void CInputGenFooting::OnButtonExamine() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	long nFooting = m_comboFooting.GetCurSel();
	CFootingApp *pFooting = GetFooting();		//pBri->GetFootingByJijumNumber(nFooting);
	if(!pFooting) return;

	BOOL bPileBase = (pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_PILE || (pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_PILE)) ? TRUE : FALSE;

	if(bPileBase)
	{
		BOOL bFemNormal = pBri->m_FemNormal.IsOutputLoaded();
		BOOL bFemEQ     = pBri->IsRcSlab() ? TRUE : (pBri->GetFemManage(COMBO_EARTHQUAKE)->IsOutputLoaded());
		
		if(!bFemNormal || (pBri->GetTypeDesingEQ() == 1 && !bFemEQ))
		{
			AfxMessageBox("�𵨸� �ؼ��� �����Ŀ� ������並 �Ͻñ� �ٶ��ϴ�.");
		}
		else
		{
			CInputCheckGrid Dlg(pBri,IDD_GEN_FOOTING, nFooting);
			Dlg.DoModal();
		}
	}
	else
	{
		AfxMessageBox("���ұ��ʰ� �ƴϹǷ� �����׸��� �����ϴ�.");
	}
	
}

void CInputGenFooting::SetGridDataCross(long nIdxOrg)
{
	m_GridPos.SetGridDefault(2, 2, 1, 1);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(pBri == NULL) return;

	BOOL bEarthAnchor = (pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR) ? TRUE : FALSE;

	CExPileApp *pPile = GetPile();
	if(bEarthAnchor == FALSE)
	{
		if(!pPile) return;
		if(nIdxOrg >= pPile->m_pArrOrgInfo.GetSize() || nIdxOrg < 0) return;
	}
	else
	{
		if(nIdxOrg >= pBri->m_nCountAnchor || nIdxOrg < 0) return;
	}
	
	CStringArray sArr;
	CString str = _T("");
	CString strHelp = _T("");
	long row = 0;
	long col = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;

	m_GridPos.SetGridDefault(2, 2, 1, 1);
	m_GridPos.SetCellSize(20, -1);

	// ���ظ鿡 �ִ� �� ������ ù��° ������ ���� ǥ��
	m_GridPos.AddCell(row, col++, "����");
	m_GridPos.AddCell(row, col++, "����");
	if(bEarthAnchor == FALSE)
	{
		m_GridPos.AddCell(row, col++, "��簢");
		m_GridPos.AddCell(row, col++, "����");
	}
	row++;
	col = 0;

	if(bEarthAnchor == FALSE)
	{
		// �������� ������ ���� ���� �Ÿ�
		COrgInfo *pOrg = pPile->m_pArrOrgInfo.GetAt(nIdxOrg);
		long i = 0; for(i = 0; i < pOrg->countSide; i++)
		{
			CPileAttr *pPileAttr = pPile->GetPileAttr(nIdxOrg, i);

			str.Format("y%d", i+1);

			m_GridPos.AddCell(row, col++, str); 
			if(pPileAttr)
			{
				m_GridPos.AddCell(row, col++, &pPileAttr->dist, nFormat, (pPile->m_bApplySameDist && nIdxOrg > 0) ? CELL_READONLY : CELL_LINE_NO, str+"�� ��ġ�Ǵ� ���Ұ� �������� ���Ұ��� ����", CELL_TYPE_AUTO);
				m_GridPos.AddCell(row, col++, &pPileAttr->ang, nFormat, CELL_LINE_NO, str+"�� ��ġ�Ǵ� ������ ��� ����", CELL_TYPE_AUTO);
				m_GridPos.AddCell(row, col, &pPileAttr->len , nFormat, CELL_LINE_NO, str+"�� ��ġ�Ǵ� ������ ����", CELL_TYPE_AUTO);
				row++;
				col = 0;
			}
		}
	}
	else
	{
		long nCountSide = (long)(pBri->m_dArrEaAnchor.GetAt(nIdxOrg));
		for(long ix = 0; ix < nCountSide; ix++)
		{
			str.Format("y%d", ix + 1);
			m_GridPos.AddCell(row, col++, str); 
			m_GridPos.AddCell(row, col, &pBri->m_dArrDistHDanAnchor[nIdxOrg], ix, nFormat, (pBri->m_bAnchorSameValue == FALSE || nIdxOrg == 0) ? nLine : CELL_READONLY, str+"�� ��ġ�Ǵ� ��Ŀ�� �������� ��Ŀ���� ����", CELL_TYPE_AUTO); m_GridPos.SetCellDec(CELL_DEC_1);

			row++;
			col = 0;
		}
	}

	m_GridPos.GetCellCount(row, col);
	m_GridPos.SetRows(row);
	m_GridPos.SetCols(col);
	m_GridPos.SetColumnWidthAll(60);
}


void CInputGenFooting::InitTabCtrl()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	m_Tab.RemoveAllTabs();
	m_Tab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_Tab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	BOOL bEarthQuakeDesign = pBri->GetTypeDesingEQ() != 0 ? TRUE : FALSE;

	CFootingApp *pFooting = GetFooting();
	if(!pFooting) return;

	long nTab = 0;
	m_nTabPileJewon = -1;
	m_nTabPileProperty = -1;
	m_nTabPHC = -1;
	m_nTabSelect = -1;
	m_nTabFootingGround	= -1;
	m_nTabModeling = -1;
	m_nTabFact	= -1;
	m_nTabShapeFact = -1;
	m_sArrTabName.RemoveAll();

	BOOL bPileBase = (pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_PILE || (pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_PILE)) ? TRUE : FALSE;
	if(bPileBase)
	{
		CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
		m_sArrTabName.Add("�������ݼ���"); m_nTabFootingGround= nTab++;
		m_sArrTabName.Add("���輱�û���"); m_nTabSelect = nTab++;
		m_sArrTabName.Add("����Ư��ġ");	m_nTabPileProperty = nTab++;
		m_sArrTabName.Add("��������");	m_nTabPileJewon = nTab++;
		if(pPile->IsPHC())
		{
			m_sArrTabName.Add("PHC����"); m_nTabPHC= nTab++;
		}
		if(pBri->IsBoxType())
		{
			m_sArrTabName.Add("�������Ǹ𵨸�"); 
			m_nTabModeling = nTab++;
		}
	}
	else
	{
		m_sArrTabName.Add("�������ݼ���"); m_nTabFootingGround = nTab++;
		if(pBri->IsBoxType())
		{
			m_sArrTabName.Add("�������Ǹ𵨸�"); 
			m_nTabModeling = nTab++;
		}
		if(bEarthQuakeDesign && pBri->IsBoxType() == FALSE)
		{
			m_sArrTabName.Add("������ ���԰��"); 
			m_nTabFact = nTab++;
			m_sArrTabName.Add("�簢�� ������ ������"); 
			m_nTabShapeFact = nTab++;
		}
	}

	
	for(long i = 0; i < m_sArrTabName.GetSize(); i++)
	{
		m_Tab.InsertItem(i, m_sArrTabName[i]);
	}

	if(m_Tab.GetItemCount() <= m_nTab) m_nTab = 0;
	m_Tab.SetCurSel(m_nTab);
}

void CInputGenFooting::SetGridDataModeling()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	CStringArray sArr;
	CString str = _T("");
	long row = 0;
	long col = 0;
	UINT nLine = CELL_LINE_NO;
	UINT nLineCur = CELL_LINE_NO;

	m_Grid.SetGridDefault(2, 2, 1, 2);
	m_Grid.SetCellSize(20, -1);

	m_Grid.AddCell(row, col++, "��  ��"); 
	m_Grid.AddCell(row, col++, "��  ��"); 
	m_Grid.AddCell(row, col++, "������");
	m_Grid.AddCell(row, col++, "�� ��");
	row++;
	col = 0;

	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;

	CString szUnit_CGS   = _T("tonf/m��");	//UNIT_CGS_TONF_M2;
	CString szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);

	sArr.RemoveAll();
	sArr.Add("Terzaghi ������");
	sArr.Add("Terzaghi ������ ����� �Է�");
	sArr.Add("�����������");
	m_Grid.AddCell(row, col++, "���ݹݷ°��(Kv) ������ Terzaghi ����", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, "���ݹݷ°��(Kv) ������ Terzaghi ����", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pBri->m_nGroundFactorKV, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	row++;
	col = 0;
	//
	m_Grid.AddCell(row, col++, "���ݹݷ°��(Kv) ������ Terzaghi ����", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, "���ݹݷ°��(Kv) ������ Terzaghi ����", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pBri->m_dGroundFactorKV, DT_RIGHT, pBri->m_nGroundFactorKV == 1 ? CELL_LINE_NO : CELL_READONLY);
	m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	row++;
	col = 0;
	//
	sArr.RemoveAll();
	sArr.Add("Ⱦ����(B:��������)");
	sArr.Add("������(B:������)");
	m_Grid.AddCell(row, col++, "����ȯ����(Bv)���� ��(B) ������", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, "����ȯ����(Bv)���� ��(B) ������", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pBri->m_nGroundFactorBv, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	row++;
	col = 0;
	//
	sArr.RemoveAll();
	sArr.Add(_T("����"));
	sArr.Add(_T("�������"));
	m_Grid.AddCell(row, col++,_T(" ����� ���߹� �ؼ��� �������ݰ�� ����"),GVHT_LEFT);
	m_Grid.AddCell(row, col++,_T(" ����� ���߹� �ؼ��� �������ݰ�� ����"),GVHT_LEFT);
	m_Grid.AddCell(row, col++, &pBri->m_nEquivalentApplyKsb, DT_RIGHT,TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	row++;
	col = 0;

	m_Grid.AddCell(row, col++,_T(" �������ݹݷ°�� ������ Ksb = �� �� Kv, �� = 1 / �Է°�"),GVHT_LEFT);
	m_Grid.AddCell(row, col++,_T(" �������ݹݷ°�� ������ Ksb = �� �� Kv, �� = 1 / �Է°�"),GVHT_LEFT);
	m_Grid.AddCell(row, col++,&pBri->m_dEquivalentApplyKsb,DT_RIGHT, pBri->m_nEquivalentApplyKsb==0 ? nLine : CELL_READONLY);
	m_Grid.SetCellDec(CELL_DEC_1); 
	row++;
	col = 0;

	// (40867) �ؼ����� Midas�� NLLINK ���� ���ϵ���
	if(pBri->m_nSelect2DFrameTool == SOLVE_MIDAS)
	{
		pBri->m_n2DGeneralSpringType = 1;
		pBri->m_n2DEQSpringType = 1;
		nLineCur = CELL_READONLY;
	}
	sArr.RemoveAll();
	sArr.Add("NLLink�� �̿��� �𵨸�");
	sArr.Add("Spring�� �̿��� �𵨸�");
	m_Grid.AddCell(row, col++, "���ݸ𵨸� ���", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, "2D ���� �𵨸�", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pBri->m_n2DGeneralSpringType, DT_RIGHT, TRUE, nLineCur, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	row++;
	col = 0;
	//
	m_Grid.AddCell(row, col++, "���ݸ𵨸� ���", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, "2D ������ �𵨸�", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pBri->m_n2DEQSpringType, DT_RIGHT, TRUE, nLineCur, "", CELL_TYPE_COMBO, sArr);
	row++;
	col = 0;
	//
	if(pBri->IsBoxType())
	{
		col = 0;
		m_Grid.AddCell(row, col++, "������ ���ݹݷ°�� ������ ��ü�β� ", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, "������ ���ݹݷ°�� ������ ��ü�β� ", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pBri->m_bSpringSpaceIncludeWallTick, DT_RIGHT,  CELL_LINE_NO, pBri->m_bSpringSpaceIncludeWallTick ? "����" : "������", CELL_TYPE_CHECK);
		row++;
		col = 0;
	}

	sArr.RemoveAll();
	sArr.Add("���������� �켼�� ���");
	sArr.Add("���������� �켼�� ���");
	sArr.Add("���������� �������� ȥ������ ���");
	m_Grid.AddCell(row, col++, "�������� ���� �ݷ°��", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, "�������� ���� �ݷ°��", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pBri->m_nVerticalFactor, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	row++;
	col = 0;
	//
	m_Grid.AddCell(row, col++, "Spring ���ذ��� (mm)", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, "�Ϻ� �𵨸�", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pBri->m_dSpringSpaceBottom, DT_RIGHT, CELL_LINE_NO);
	row++;
	col = 0;

	m_Grid.AddCell(row, col++, "Spring ���ذ��� (mm)", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, "��ü (������)", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pBri->m_dSpringSpaceWall, DT_RIGHT, CELL_LINE_NO);
	row++;
	col = 0;

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(160);
	m_Grid.SetColumnWidth(2, 250);
	m_Grid.SetColumnWidth(3, 80);
}

void CInputGenFooting::SetGridDataSelect()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	CExPileApp *pPile = GetPile();
	if(!pPile) return;

	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;

	CStringArray sArr;
	CString str = _T("");
	long row = 0;
	long col = 0;
	UINT nLine = CELL_LINE_NO;
	UINT nLineCur = CELL_LINE_NO;

	m_Grid.SetGridDefault(2, 2, 1, 2);
	m_Grid.SetCellSize(20, -1);
	
	m_Grid.AddCell(row, col++, "��  ��"); 
	m_Grid.AddCell(row, col++, "��  ��"); 
	m_Grid.AddCell(row, col++, "������");
	m_Grid.AddCell(row, col++, "�� ��");
	row++;
	col = 0;

	long nColWidth = 120;

	CString sHead = _T("������");

	// ������¼���� ����
	BOOL bPileASD = (pStd->IsApplyBoredPileASD(pPile) || pStd->IsApplyPhcPileASD(pPile)) ? TRUE : FALSE;
	if(m_bDesignLsd == FALSE)
	{
		m_Grid.AddCell(row, col++, sHead); 
		m_Grid.AddCell(row, col++, "�����ҿ��� ���", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pPile->m_bApplyGroupEffect, DT_RIGHT, nLine, pPile->m_bApplyGroupEffect ? _T("����") : _T("�������"), CELL_TYPE_CHECK);
		m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
		row++;
		col = 0;
	}		

	m_Grid.AddCell(row, col++, sHead); 
	m_Grid.AddCell(row, col++, "�ָ� ������ ���", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pPile->m_bApplyFriction, DT_RIGHT, nLine, pPile->m_bApplyFriction ? _T("����") : _T("�������"), CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
	row++;
	col = 0;

	if(m_bDesignLsd && bPileASD == FALSE && (pPile->m_nType == PILE_TYPE_HYUNJANG || pPile->m_nType == PILE_TYPE_PHC))
	{
		sArr.RemoveAll();
		sArr.Add(_T("�������"));
		sArr.Add(_T("��ũ��Ʈ�����߷� ���"));
		sArr.Add(_T("(��ũ��Ʈ-��)�����߷� ���"));
		m_Grid.AddCell(row, col++, sHead);
		m_Grid.AddCell(row, col++, _T("�����°���� ��������"), GVHT_LEFT);
		m_Grid.AddCell(row, col++, &pPile->m_nSelectExcludeSelfWeight, DT_RIGHT, TRUE, nLine , _T(""), CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
		row++;
		col = 0;
	}

	BOOL bSteel = (pPile->m_nType == PILE_TYPE_STEEL || pPile->m_nType == PILE_TYPE_STEEL_DIGGING) ? TRUE : FALSE;		// ����
	BOOL bHyunTa = (pPile->m_nType == PILE_TYPE_HYUNJANG);

	if(m_bDesignLsd && (pPile->m_nType == PILE_TYPE_PHC || pPile->m_nType == PILE_TYPE_PHC_DIGGING || bSteel==TRUE))
	{
		m_Grid.AddCell(row, col++, sHead);
		m_Grid.AddCell(row, col++, "����� ����", GVHT_LEFT);
		m_Grid.AddCell(row, col++, &pPile->m_bApplyNonDisplacementPile, DT_RIGHT, nLine, pPile->m_bApplyNonDisplacementPile ? _T("����") : _T("�������"), CELL_TYPE_CHECK);
		m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
		row++;
		col = 0;

		// ������¼���� ����
		BOOL bPileASD = (pStd->m_nApplyBoredPileASD && pPile->IsBoredPile()) || (pStd->m_nApplyPhcPileASD && pPile->IsPHC()) ? TRUE : FALSE; 
		if(bPileASD == FALSE)
		{
			sArr.RemoveAll();
			sArr.Add(_T("�������"));
			sArr.Add(_T("Broms ���"));
			m_Grid.AddCell(row, col++, sHead);
			m_Grid.AddCell(row, col++, _T("Ⱦ���������� ����"), GVHT_LEFT);
			m_Grid.AddCell(row, col++, &pPile->m_nTypeCheckHoriSupportForce, DT_RIGHT, TRUE, nLine , _T(""), CELL_TYPE_COMBO, sArr);
			m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
			row++;
			col = 0;

			nColWidth = 200;
			sArr.RemoveAll();
			sArr.Add(_T("�������"));
			sArr.Add(_T("���������ʼ���������� ����"));
			m_Grid.AddCell(row, col++, sHead);
			m_Grid.AddCell(row, col++, _T("ħ�� ����"), GVHT_LEFT);
			m_Grid.AddCell(row, col++, &pPile->m_nTypeCheckSubsidence, DT_RIGHT, TRUE, nLine , _T(""), CELL_TYPE_COMBO, sArr);
			m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
			row++;
			col = 0;
		}
	}

	sHead = _T("���Һ�ü");
	m_Grid.AddCell(row, col++, sHead); 
	m_Grid.AddCell(row, col++, _T("�������� �������"), GVHT_LEFT);
	m_Grid.AddCell(row, col++, &pPile->m_dLengthPileMax, DT_RIGHT, nLine);
	m_Grid.AddCell(row, col++, _T("mm"), GVHT_LEFT, CELL_READONLY);
	row++;
	col = 0;

	if( m_bDesignLsd == FALSE )
	{
		m_Grid.AddCell(row, col++, sHead); 
		m_Grid.AddCell(row, col++, _T("���� ������ ������� ���Һ�"), GVHT_LEFT);
		m_Grid.AddCell(row, col++, &pPile->m_dRateDeductPileStress, DT_RIGHT, nLine);
		m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
		row++;
		col = 0;
	}

	if(bHyunTa)
	{
		sHead = _T("����Ÿ������");
		m_Grid.AddCell(row, col++, sHead); 
		m_Grid.AddCell(row, col++, "���䳪��Ʈ���", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pPile->m_bApplyBentonite, DT_RIGHT, m_bDesignLsd ? CELL_READONLY : CELL_LINE_NO, pPile->m_bApplyBentonite ? _T("����") : _T("�������"), CELL_TYPE_CHECK);
		m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
		row++;
		col = 0;
	}
	if(pPile->IsPHC())
	{
		sHead = _T("PHC����");
		m_Grid.AddCell(row, col++, sHead); 
		m_Grid.AddCell(row, col++, "ft(�� ��������) ����� �ּ���� ����", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pPile->m_bApplyMinAxialAtStressCheckPHC, DT_RIGHT, m_bDesignLsd ? CELL_READONLY : CELL_LINE_NO, pPile->m_bApplyMinAxialAtStressCheckPHC ? _T("����") : _T("�������"), CELL_TYPE_CHECK);
		m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
		row++;
		col = 0;
	}
	if( m_bDesignLsd == FALSE )
	{
		sHead = _T("���ʰ��պ�");
		m_Grid.AddCell(row, col++, sHead); 
		m_Grid.AddCell(row, col++, "������ũ��Ʈ �ܸ� ���°���", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pPile->m_bCheckBaseConcStress, DT_RIGHT, nLine, pPile->m_bCheckBaseConcStress ? _T("����") : _T("�������"), CELL_TYPE_CHECK);
		m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
		row++;
		col = 0;
	}
		
	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidth(0,100);
	m_Grid.SetColumnWidth(1,320);
	m_Grid.SetColumnWidth(2,nColWidth);
	m_Grid.SetColumnWidth(3,80);
}

// ���� ���� �����
void CInputGenFooting::OnSelchangeComboKindPile() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CExPileApp *pPile = GetPile();
	if(pPile) pPile->m_nType = m_comboKindPile.GetCurSel();
	SetDataSave();
	InitTabCtrl();
	SetGridData();
	SetDataInit();
	SetControl();

	if(pPile->m_nType == PILE_TYPE_HYUNJANG)
	{
		pPile->m_dT1 = 0;
		pPile->m_dT2 = 0;
		pPile->SetDatDefaultHyunTaRebar();
	}

	if(pPile->m_nType == PILE_TYPE_PHC || pPile->m_nType == PILE_TYPE_PHC_DIGGING)
	{
		pPile->m_dT2 = 0;
		pPile->SetDataDefaultPHC();
	}

	// EHP�� ��쿡�� ���̾�α� ����
	if(pPile->m_nType == PILE_TYPE_EHP)
	{
		pPile->m_dT2 = 0;
		m_dlgEHPLink.DoModal();
	}
}

// ���� ���� �����
void CInputGenFooting::OnSelchangeComboKindPile2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CExPileApp *pPile = GetPile();
	if(pPile) pPile->m_nTypeConstruct = m_comboKindPile2.GetCurSel();

	if(m_pIn->m_pARcBridgeDataStd->m_nApplyBoredPileASD == 1)
	{
		SetGridData();
		SetDataInit();
	}
}


void CInputGenFooting::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	m_nTab = m_Tab.GetCurSel();
	m_nTabPos = m_TabPos.GetCurSel();

	SetGridData();
	SetDataInit();
	if(m_nTab == m_nTabPHC)
		AfxMessageBox(IDS_PHC_WARNING, MB_OK | MB_ICONINFORMATION);

	DrawInputDomyunView(FALSE);

	return;
}

// �ʿ��� ��Ʈ�ѵ鸸 ���̰� ��.
// ���ϱ����϶��� ���������϶�...
// void CInputGenFooting::MoveControl()
// {
// 	// ���ʰ� �ٲ𶧸��� �ʿ���� ��Ʈ�ѵ��� �Ⱥ��̴� ������ �̵���Ų��.
// 	CExPileApp *pPile = GetPile();
// 	if(pPile)
// 	{
// 		if(m_bOutPileControl)
// 		{
// 			MoveControlGroup(m_dArrID,  CPoint(-400, 0), 20, 10, 0.01);
// 			m_bOutPileControl = FALSE;
// 		}
// 	}
// 	else
// 	{
// 		if(!m_bOutPileControl)
// 		{
// 			MoveControlGroup(m_dArrID,  CPoint(400, 0), 20, 10, 0.01);
// 			m_bOutPileControl = TRUE;
// 		}
// 	}
// }

BOOL CInputGenFooting::OnInitDialog() 
{
	CInputDialogBase::OnInitDialog();

	// ó������ ������ ������ �� �� �����̹Ƿ� OnInitDialog�Ŀ� �ٽ� ���� �������ش�.
	InitTabPosCtrl();
	SetControl();

	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputGenFooting::OnButtonFinite() 
{
	// TODO: Add your control notification handler code here
	CExPileApp *pPile = GetPile();
	if(pPile)
	{
		CString str1 = _T("������ ������ ��� ������ ������������ ��������� �����Ͽ� ����[����2008 5.8.8.2(1)1)]�ϸ� ��ü�ۿ���� ������ ���л���� ����ϴ� �����帻���� ������ �������� �ʰ� �Ϲݽ�[����2008 �ؼ�ǥ 5.8.27]���� ���˴ϴ�.\n\n");
		CString str2 = _T("[���α��������2008 5.8.11�ؼ�(1)2)]���� ��L�� 2.5������ �ݹ������� ������ ����ص� ���ٰ� ǥ���ϰ� ������ ������ ��L�� 2.5���� ���� ���� ��������Է¿ܿ��� �������� �Ǵ��� �ʿ��� �� ���� ���Դϴ�.\n");
		CString str3 = _T("[�����帻�� ����]���� ��������� �Է��ϼž� �ϸ� �Է����� �ʰų� 1.0���� �����ϽŴٸ� ���� ���Ұ���� �ݹ����帻�Ұ� ������ ���������� ����˴ϴ�.\n\n");
		CString str4 = _T("������ ������ ������ ���������� ������ �ޱ⶧���� ���������� ����� �ʿ䰡 �ִ�. �ٸ� ������ �������� ���� ���� ������ ���Եȴٸ� �Ϲ������� ���������� ���� ���� ����.[���α���������ؼ�2008 5.8.8.2�ؼ�(1)2)]\n");

		AfxMessageBox(str1+str2+str3+str4);

		CFinitePileDlg dlg(pPile);
		dlg.DoModal();
	}
}

void CInputGenFooting::OnButtonJewon() 
{
	// TODO: Add your control notification handler code here
	CExPileApp *pExPile = GetPile();
	if(pExPile)
	{
		CPhcPileJewonDlg dlg;
		if(dlg.DoModal()==IDOK)
		{
			pExPile->m_dDia			= dlg.m_Dia;
			pExPile->m_dT1			= dlg.m_Thick;
			pExPile->m_dFce1_PHC	= dlg.m_fce;
			pExPile->m_dAe_PHC		= dlg.m_Ae;
			pExPile->m_dIe_PHC		= dlg.m_Ie;
			pExPile->m_dZe_PHC		= dlg.m_Ze;
			pExPile->m_nKind_PHC	= dlg.m_nType;
			SetDataInit();
			DrawInputDomyunView(TRUE);
		}
	}
}

// ���ʱ��� ���
void CInputGenFooting::SetGridDataBaseFactor()
{
	CFootingApp *pFooting = GetFooting();
	if(!pFooting) return;
	m_dlgBaseFact.m_pFooting = pFooting;

	CStringArray sArr;
	CString str = _T("");
	CString strHelp = _T("");
	long row = 0;
	long col = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;

	m_Grid.SetGridDefault(2, 2, 1, 0);
	m_Grid.SetCellSize(20, -1);

	row = 0;
	m_Grid.AddCell(row, col, "��������");  m_Grid.AddCell(row+1, col++, &pFooting->m_dEQSettleFactVerZ, nFormat, nLine, "", CELL_TYPE_AUTO, sArr, &m_dlgBaseFact);	m_Grid.SetCellDec(CELL_DEC_3);
	m_Grid.AddCell(row, col, "������");  m_Grid.AddCell(row+1, col++, &pFooting->m_dEQSettleFactHorXY, nFormat, nLine, "", CELL_TYPE_AUTO, sArr, &m_dlgBaseFact);	m_Grid.SetCellDec(CELL_DEC_3);
	m_Grid.AddCell(row, col, "��Ʋ��");    m_Grid.AddCell(row+1, col++, &pFooting->m_dEQSettleFactTorZ, nFormat, nLine, "", CELL_TYPE_AUTO, sArr, &m_dlgBaseFact);	m_Grid.SetCellDec(CELL_DEC_3);
	m_Grid.AddCell(row, col, "ȸ��(X��)"); m_Grid.AddCell(row+1, col++, &pFooting->m_dEQSettleFactRotX, nFormat, nLine, "", CELL_TYPE_AUTO, sArr, &m_dlgBaseFact);	m_Grid.SetCellDec(CELL_DEC_3);
	m_Grid.AddCell(row, col, "ȸ��(Y��)"); m_Grid.AddCell(row+1, col++, &pFooting->m_dEQSettleFactRotY, nFormat, nLine, "", CELL_TYPE_AUTO, sArr, &m_dlgBaseFact);	m_Grid.SetCellDec(CELL_DEC_3);

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(80);
}

CFootingApp* CInputGenFooting::GetFooting(long nIdxFooting)
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return NULL;

	if(nIdxFooting == -1) nIdxFooting = m_comboFooting.GetCurSel();
	if(nIdxFooting < 0) return NULL;

	if(pBri->IsBoxType())
	{
		return &pBri->m_footingBox;
	}

	if(nIdxFooting == 0) 
	{
		if(pBri->IsOutWall(TRUE) && pBri->m_footingStt.m_bIs)
			return &pBri->m_footingStt;
	}
	else if(nIdxFooting < m_comboFooting.GetCount()-1)
	{
		nIdxFooting -= 1;
		if(nIdxFooting < pBri->GetCountInWall() && nIdxFooting > -1)
		{
			CWallApp *pWall = pBri->GetInWall(nIdxFooting);
			if(pWall->m_bIs && pWall->m_footing.m_bIs)
				return &pWall->m_footing;
		}
	}
	else
	{
		if(pBri->IsOutWall(FALSE) && pBri->m_footingEnd.m_bIs)
			return &pBri->m_footingEnd;
	}

	return NULL;	
}

void CInputGenFooting::AddCellFootingGround(HGINT32 &row, HGINT32 &col)
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();

	CFootingApp *pFooting = GetFooting();
	if(!pFooting) return;

	UINT nFormat	= DT_RIGHT | DT_VCENTER;
	CString szUnit_CGS   = _T("kgf/cm��");
	CString szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	CStringArray sArr;

	HGINT32 nSelFooting = m_comboFooting.GetCurSel();
	if( nSelFooting < 0 ) return;
	
	CString sHead = _T("��������");

	// �ڵ�����
	m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "�������� �ڵ�����", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pBri->m_bAutoGroundInfo[nSelFooting], DT_RIGHT | DT_VCENTER, CELL_LINE_NO, pBri->m_bAutoGroundInfo[nSelFooting] ?  _T("����") : _T("�������"), CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);

	UINT nLineRefLine	= pBri->m_bAutoGroundInfo[nSelFooting]==TRUE ? CELL_READONLY : CELL_LINE_NO;

	// ���������� ����
	sArr.Add(_T("������"));
	sArr.Add(_T("������"));
	sArr.Add(_T("�Ϲ�"));
	m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "���������� ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pFooting->m_nTypeBaseGround, DT_RIGHT, TRUE, nLineRefLine , _T(""), CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+4, _T(""), nFormat, CELL_READONLY);

	// ���������� Nġ
	m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "���������� Nġ", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pFooting->m_dNValueBaseGround, nFormat, nLineRefLine);
	m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dNValueBaseGround), nFormat, nLineRefLine);
	m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);

	// ���������� ������
	szUnit_CGS = UNIT_CGS_TONF_M2;
	szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "���������� ������", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pFooting->m_C_BaseGround, nFormat, nLineRefLine);
	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_C_BaseGround), nFormat, nLineRefLine);
	m_Grid.AddCell(row++, col+4, szUnit_Apply, GVHT_LEFT, CELL_READONLY);

	if(pBri->IsBoxType() == FALSE)
	{
		// ���ݺ������
		szUnit_CGS = UNIT_CGS_KGF_CM2;
		szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS, 2);
		m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "���ݺ������(Eo)", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pFooting->m_dBoringEo, nFormat, nLineRefLine);
		m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
		m_Grid.SetCellDec(CELL_DEC_0);
		m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dBoringEo), nFormat, nLineRefLine);
		m_Grid.AddCell(row++, col+4, szUnit_Apply, GVHT_LEFT, CELL_READONLY);

		if(m_bDesignLsd)
		{
			// ���� ���Ƽۺ�
			m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
			m_Grid.AddCell(row, col+1, "�������Ƽۺ�(��)", GVHT_LEFT);
			m_Grid.AddCell(row, col+2, &pFooting->m_dBoringRatePoasong, nFormat, nLineRefLine);
			m_Grid.SetCellDec(CELL_DEC_3);
			m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);
		}
	}

	if( m_bDesignLsd == FALSE )
	{
		szUnit_CGS   = "tonf/m��";
		szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
		m_Grid.AddCell(row, col, sHead);
		m_Grid.AddCell(row, col+1, "�ִ���� ������", GVHT_LEFT);
		m_Grid.AddCell(row, col+4, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
		m_Grid.AddCell(row++, col+2, &pFooting->m_QaMax_BaseGround, DT_RIGHT);
		m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
		m_Grid.SetCellDec(CELL_DEC_2);
	}

	col = 0;
	sArr.RemoveAll();
	if( m_bDesignLsd == TRUE )
	{
		if( pFooting->m_nTypeBaseGround < 2)//������,������
			sArr.Add("ǥ�ذ��Խ���(SPT)���");
		else
		{
			sArr.Add("Hoek & Brown ���Ƚ�");
			sArr.Add("AASHTO ���Ƚ�"); 
			sArr.Add("Canadian Geotechnical Society");
			sArr.Add("��ü����");
		}
	}
	else
	{
		sArr.Add("Terzaghi��");
		sArr.Add("Meyerhof��");
		sArr.Add("Hansen  ��");

	}
	m_Grid.AddCell(row, col++, sHead);
	m_Grid.AddCell(row, col++, m_bDesignLsd==TRUE ? "��Ī������ ������" : "��������� ������", GVHT_LEFT);
	m_Grid.AddCell(row, col++, &pFooting->m_nTypeEquation_Qa, DT_RIGHT, TRUE, (sArr.GetCount() < 2) ? CELL_READONLY : CELL_LINE_NO, _T(""), CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col++, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_nTypeEquation_Qa), nFormat); 

	if( m_bDesignLsd )
	{
		m_xyButton.x	= col--;
		m_xyButton.y	= row;
	}		
	m_Grid.AddCell(row++, col++,  _T(""), GVHT_LEFT, CELL_READONLY);
	m_Grid.SetCellDec(CELL_DEC_2);
	col = 0;
}

void CInputGenFooting::AddCellCalcSupportForce(HGINT32 &row, HGINT32 &col)
{
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	CExPileApp *pPile	= GetPile();

	BOOL bPile = pPile == NULL ? FALSE : TRUE;
	BOOL bEarthQuakeDesign = pBri->GetTypeDesingEQ() != 0 ? TRUE : FALSE;
	BOOL bPileASD = (pStd->IsApplyBoredPileASD(pPile) || pStd->IsApplyPhcPileASD(pPile))? TRUE : FALSE; // ���Ը��� �Ǵ� PHC������ ������¼���� ���� 

	CFootingApp *pFooting = GetFooting();
	if(!pFooting) return;

	UINT nLine = CELL_LINE_NO;
	UINT nFormat	= DT_RIGHT | DT_VCENTER;

	CString sHead = _T("������");
	m_Grid.AddCell(row, col, sHead);
	m_Grid.AddCell(row, col+1, "�ڵ����", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pFooting->m_bAutoCalcSupportForce, DT_RIGHT | DT_VCENTER, nLine, pFooting->m_bAutoCalcSupportForce ?  _T("����") : _T("�������"), CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);

	CString str = _T("");
	CString szUnit_CGS   = "tonf/m��";
	CString szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);

	CString strStatus[3] = {_T(" : ����Ѱ����"), _T(" : �����Ѱ����"), _T(" : �شܻ�Ȳ�Ѱ����")};
	if(m_bDesignLsd == FALSE)
	{
		strStatus[0] = _T(" : ����");
		strStatus[1] = _T(" : ������");
	}

	CStringArray sArr;
	if(!bPile)
	{
		long nCountCase = m_bDesignLsd ? 3 : 2;

		for(long ix=0; ix<nCountCase; ix++)
		{
			if((m_bDesignLsd == FALSE && bEarthQuakeDesign == FALSE) && ix == 1) continue;
			str.Format(_T("%s%s"), _T("���� ���������"), strStatus[ix]);

			m_Grid.AddCell(row, col, sHead);
			m_Grid.AddCell(row, col+1, str, GVHT_LEFT);
			
			m_Grid.AddCell(row, col+2, &pFooting->m_Qa_Calculate[ix], DT_RIGHT, pFooting->m_bAutoCalcSupportForce ? CELL_READONLY : CELL_LINE_NO);
			m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply); 
			m_Grid.SetCellDec(m_bDesignLsd ? CELL_DEC_3 : CELL_DEC_2);
			
			m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_Qa_Calculate[ix]), nFormat); 
			m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
			m_Grid.SetCellDec(m_bDesignLsd? CELL_DEC_3 : CELL_DEC_2);

			m_Grid.AddCell(row++, col+4, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
			m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
			m_Grid.SetCellDec(CELL_DEC_2);
		}
	}
	else
	{
		// ���� �������� ���� ������
		CString szUnit_CGS   = "tonf";
		CString szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);

		// ���� ����
		long nCountCase = m_bDesignLsd ? 3 : 2;
		for(long ix=0; ix<nCountCase; ix++)
		{
			if((m_bDesignLsd == FALSE && bEarthQuakeDesign == FALSE) && ix == 1) continue;
			if(m_bDesignLsd && bPileASD && ix == 1) continue;
			if(m_bDesignLsd && bEarthQuakeDesign == FALSE && ix == 2) continue;

			m_Grid.AddCell(row, col, sHead);
			if(m_bDesignLsd)
				str.Format(_T("%s%s"), _T("���� ��������� : "), strStatus[ix]);
			else
				str.Format(_T("%s%s"), _T("���� ��������� : "), ix == 0 ? _T("����") : _T("������"));
			
			m_Grid.AddCell(row, col+1, str, GVHT_LEFT);

			m_Grid.AddCell(row, col+2, &pFooting->m_Qa_Calculate[ix], DT_RIGHT, pFooting->m_bAutoCalcSupportForce ? CELL_READONLY : CELL_LINE_NO);
			m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply); 
			m_Grid.SetCellDec(m_bDesignLsd ? CELL_DEC_0 : CELL_DEC_2);

			m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_Qa_Calculate[ix]), nFormat); 
			m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
			m_Grid.SetCellDec(m_bDesignLsd? CELL_DEC_0 : CELL_DEC_2);

			szUnit_CGS   = _T("tonf");
			szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
			str.Format(_T("%s/��"),szUnit_Apply);
			m_Grid.AddCell(row++, col+4, str, GVHT_LEFT, CELL_READONLY);
		}

		for(long ix=0; ix<nCountCase; ix++)
		{
			if((m_bDesignLsd == FALSE && bEarthQuakeDesign == FALSE) && ix == 1) continue;
			if(m_bDesignLsd && bPileASD && ix == 1) continue;

			m_Grid.AddCell(row, col, sHead);
			if(m_bDesignLsd)
				str.Format(_T("%s%s"), _T("���� ����ι߷� : "), strStatus[ix]);
			else
				str.Format(_T("%s%s"), _T("���� ����ι߷� : "), ix == 0 ? _T("����") : _T("������"));

			m_Grid.AddCell(row, col+1, str, GVHT_LEFT);

			m_Grid.AddCell(row, col+2, &pFooting->m_Pa_Calculate[ix], DT_RIGHT, pFooting->m_bAutoCalcSupportForce ? CELL_READONLY : CELL_LINE_NO);
			m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply); 
			m_Grid.SetCellDec(m_bDesignLsd ? CELL_DEC_0 : CELL_DEC_2);

			m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_Pa_Calculate[ix]), nFormat); 
			m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
			m_Grid.SetCellDec(m_bDesignLsd? CELL_DEC_0 : CELL_DEC_2);

			szUnit_CGS   = _T("tonf");
			szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
			str.Format(_T("%s/��"),szUnit_Apply);
			m_Grid.AddCell(row++, col+4, str, GVHT_LEFT, CELL_READONLY);
		}
	}
}

// ���� ���� ����
void CInputGenFooting::SetGridDataFootingGround()
{
	CFootingApp *pFooting = GetFooting();
	if(!pFooting) return;
	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;

	CRcBridgeApp *pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	CExPileApp *pPile	= GetPile();
	
	BOOL bPile	= pPile == NULL ? FALSE : TRUE;
	BOOL bRock	= pFooting->m_nTypeBaseGround == 2 ? TRUE : FALSE;
	BOOL bSteel = (bPile && (pPile->m_nType == PILE_TYPE_STEEL || pPile->m_nType == PILE_TYPE_STEEL_DIGGING)) ? TRUE : FALSE;
	BOOL bPileASD = (pStd->IsApplyBoredPileASD(pPile) || pStd->IsApplyPhcPileASD(pPile))? TRUE : FALSE; // ���Ը��� �Ǵ� PHC������ ������¼���� ���� 
	BOOL bHyunTa = (pPile && pPile->m_nType == PILE_TYPE_HYUNJANG);

	CStringArray sArr;
	CString str = _T("");
	CString strHelp = _T("");
	HGINT32 row = 0;
	HGINT32 col = 0;
	UINT nLine = CELL_LINE_NO;
	UINT nFormat = DT_CENTER | DT_VCENTER;

	m_Grid.SetGridDefault(2, 2, 1, 2);
	m_Grid.SetCellSize(20, -1);

	m_Grid.AddCell(row, col,	  "��  ��");
	m_Grid.AddCell(row, col+1,	  "��  ��");
	m_Grid.AddCell(row, col+2, "������");
	m_Grid.AddCell(row++, col+4, "��  ��");

	// ������ ����, ��������� ������
	AddCellCalcSupportForce(row, col);

	// ���� ����
	// (ARCBRIDGE-3498) ��������������� �Ϻ� �Է� ����
	if( !pPile )
	{
 		AddCellFootingGround(row, col);
	}

	CString sHead = _T("��������");
	CString szUnit_CGS   = _T("tonf/m��");	//UNIT_CGS_TONF_M2;
	CString szUnit_Apply = m_bDesignLsd ? _T("kN/m��")  : pStd->GetStringUnitType(szUnit_CGS);
	if(bPile)
	{
		if( m_bDesignLsd == FALSE )
		{
			CString strMsg = _T("���������� �������� ����� ���Ұ����� �������ǿ� ���� �޶����� �����Ƿ� Ȯ���� ����Ͻñ� �ٶ��ϴ�.");
			m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
			m_Grid.AddCell(row, col+1, "���������� ������ ��� (qd = 00 x N)", GVHT_LEFT, CELL_LINE_NO);
			m_Grid.AddCell(row, col+2, &pFooting->m_dFactorPile_qd, DT_RIGHT, CELL_LINE_NO, strMsg);
			m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
			m_Grid.SetCellDec(CELL_DEC_3);
			m_Grid.AddCell(row++, col+4, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
		}

		// ����, �Ѱ����
		if( (pPile->IsPHC() || pPile->m_nType == PILE_TYPE_STEEL || pPile->m_nType == PILE_TYPE_STEEL_DIGGING) && m_bDesignLsd )
		{
			sArr.RemoveAll();
			sArr.Add(_T("������"));
			sArr.Add(_T("������"));
			str	= _T("");

			m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
			m_Grid.AddCell(row, col+1, _T("������ Ⱦ���� ������ ���� ����"), GVHT_LEFT);
			m_Grid.AddCell(row, col+2, &pPile->m_nTypeGroundCrossCheck, DT_RIGHT | DT_VCENTER, TRUE, nLine , _T(""), CELL_TYPE_COMBO, sArr);
			m_Grid.AddCell(row++, col+4, str, GVHT_LEFT, CELL_READONLY);

			sArr.RemoveAll();
			sArr.Add(_T("0.4 Ncorr"));
			sArr.Add(_T("0.3 Ncorr"));
			sArr.Add(_T("������Է�"));
			str	= _T("");

			m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
			m_Grid.AddCell(row, col+1, _T("�Ѱ輱�������� (qt)"), GVHT_LEFT);
			m_Grid.AddCell(row, col+2, &pPile->m_nTypeLimitEdgeQt, DT_RIGHT | DT_VCENTER, TRUE, nLine , _T(""), CELL_TYPE_COMBO, sArr);
			m_Grid.AddCell(row++, col+4, str, GVHT_LEFT, CELL_READONLY);

			UINT nLineUser = pPile->m_nTypeLimitEdgeQt == 2? CELL_LINE_NO : CELL_READONLY;
			m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
			m_Grid.AddCell(row, col+1, _T("�Ѱ輱�������� (qt)"), GVHT_LEFT);

			m_Grid.AddCell(row, col+2, &pPile->m_dUserLimitEdgeQt, DT_RIGHT, nLineUser);
			m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply); 
			m_Grid.SetCellDec(CELL_DEC_5);

			m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pPile->m_dUserLimitEdgeQt), nFormat); 
			m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
			m_Grid.SetCellDec(CELL_DEC_5);

			m_Grid.AddCell(row++, col+4, _T("������Է°�"), GVHT_LEFT, CELL_READONLY);

			szUnit_CGS		= _T("");
			szUnit_Apply	= szUnit_CGS;
			m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
			m_Grid.AddCell(row, col+1, _T("�ð������ �������ǿ� ���� ���(Cp)"), GVHT_LEFT);

			m_Grid.AddCell(row, col+2, &pPile->m_dCpConstTypeAndGround, DT_RIGHT);
			m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply); 
			m_Grid.SetCellDec(CELL_DEC_5);

			m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pPile->m_dCpConstTypeAndGround), nFormat); 
			m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
			m_Grid.SetCellDec(CELL_DEC_5);

			m_Grid.AddCell(row++, col+4, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
			m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
			m_Grid.SetCellDec(CELL_DEC_5);
		}

		if( m_bDesignLsd == TRUE && bPileASD == FALSE)
		{
			if(bSteel || pPile->IsPHC())
			{
				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, "���� SPT Ÿ��Ƚ�� (Ncorr)", GVHT_LEFT); 
				m_Grid.AddCell(row, col+2, &pPile->m_dCountHitSPT_Ncorr, DT_RIGHT, nLine, _T(""), CELL_TYPE_AUTO);
				m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pPile->m_dCountHitSPT_Ncorr), nFormat); 
				m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);
			}
			
			if(bSteel)
			{
				sArr.RemoveAll(); 
				sArr.Add(_T("��ȣ")); 
				sArr.Add(_T("�ҷ�"));
				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, _T("���������� ���Ի���"), GVHT_LEFT); 
				m_Grid.AddCell(row, col+2, &pPile->m_nTypeIntrusionToSupport, DT_RIGHT, TRUE, nLine, _T("Type : ����"), CELL_TYPE_COMBO, sArr);
				m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);
			}
		}

		if(m_bDesignLsd && (pPile->m_nType == PILE_TYPE_STEEL_DIGGING || pPile->m_nType == PILE_TYPE_PHC_DIGGING) && bPileASD == FALSE)
		{
			m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
			m_Grid.AddCell(row, col+1, _T("Ÿ�Ը��ҿ� ���� ���Ը��� ���������� ȿ��"), GVHT_LEFT); 
			m_Grid.AddCell(row, col+2, &pPile->m_dEfficiencyQtVsTypePile, DT_RIGHT, nLine, _T(""), CELL_TYPE_AUTO);
			m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pPile->m_dEfficiencyQtVsTypePile), nFormat); 
			m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);

			m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
			m_Grid.AddCell(row, col+1, _T("Ÿ�Ը��ҿ� ���� ���Ը��� �ָ鸶���� ȿ��"), GVHT_LEFT); 
			m_Grid.AddCell(row, col+2, &pPile->m_dEfficiencyFrictionVsTypePile, DT_RIGHT, nLine, _T(""), CELL_TYPE_AUTO);
			m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pPile->m_dEfficiencyFrictionVsTypePile), nFormat); 
			m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);
		}
		// AAB-6153
		if(bHyunTa && m_bDesignLsd)
		{
			UINT nLineAuto = pBri->m_bPileNvalueAuto ? CELL_READONLY : CELL_LINE_NO;

			sArr.RemoveAll();
			sArr.Add(_T("������"));
			sArr.Add(_T("������")); 
			sArr.Add(_T("�Ϲ�"));
			m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
			m_Grid.AddCell(row, col+1, _T("���Ҽ����� ��������"), GVHT_LEFT); 
			m_Grid.AddCell(row, col+2, &pPile->m_nTypeGroundEdge, DT_RIGHT, TRUE, nLineAuto, _T(""), CELL_TYPE_COMBO, sArr);
			m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);

			if(pPile->m_nTypeGroundEdge == 0) // ������
			{
				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, _T("���Ҽ����� ���豸�� ������ ��� N��(N60)"), GVHT_LEFT); 
				m_Grid.AddCell(row, col+2, &pPile->m_dN60_Edge, DT_RIGHT, nLineAuto, _T(""), CELL_TYPE_AUTO);
				m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pPile->m_dN60_Edge), nFormat); 
				m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);

				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, _T("���Ҽ����� �߰������� ������ȿ(��v')"), GVHT_LEFT); 
				m_Grid.AddCell(row, col+2, &pPile->m_dSigma_V_Edge, DT_RIGHT, nLineAuto, _T(""), CELL_TYPE_AUTO);
				m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pPile->m_dSigma_V_Edge), nFormat); 
				m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);
			}
			else if(pPile->m_nTypeGroundEdge == 1) // ������
			{
				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, _T("���Ҽ����� ���� ���ܰ���(Cu)"), GVHT_LEFT); 
				m_Grid.AddCell(row, col+2, &pPile->m_dCu_Edge, DT_RIGHT, nLineAuto, _T(""), CELL_TYPE_AUTO);
				m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pPile->m_dCu_Edge), nFormat); 
				m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);
			}
			else // �Ϲ�
			{
				szUnit_CGS   = "kgf/cm��";
				szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);

				sArr.RemoveAll();
				sArr.Add(_T("2.5qu"));
				sArr.Add(_T("[��s + ��(m x ��s + s)]qu")); 
				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, _T("�Ϲ��� ����������(qp)"), GVHT_LEFT); 
				m_Grid.AddCell(row, col+2, &pPile->m_nTypeSupport_Edge, DT_RIGHT, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
				m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
				m_Grid.AddCell(row++, col+4, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
				//
				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, _T("���Ҽ����� ���� ������భ��(qu)"), GVHT_LEFT); 
				m_Grid.AddCell(row, col+2, &pPile->m_dqu_Edge, DT_RIGHT, nLineAuto, _T(""), CELL_TYPE_AUTO); m_Grid.SetCellDec(CELL_DEC_3);
				m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
				m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pPile->m_dqu_Edge), nFormat); 
				m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
				m_Grid.AddCell(row++, col+4, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
				//
				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, _T("�ļ�Ϲ� ����(s)"), GVHT_LEFT); 
				m_Grid.AddCell(row, col+2, &pPile->m_ds_Edge, DT_RIGHT, pPile->m_nTypeSupport_Edge==0 ? CELL_READONLY : nLine, _T(""), CELL_TYPE_AUTO); m_Grid.SetCellDec(CELL_DEC_7);
				m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pPile->m_ds_Edge), nFormat); 
				m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);
				//
				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, _T("�ļ�Ϲ� ����(m)"), GVHT_LEFT); 
				m_Grid.AddCell(row, col+2, &pPile->m_dm_Edge, DT_RIGHT, pPile->m_nTypeSupport_Edge==0 ? CELL_READONLY : nLine, _T(""), CELL_TYPE_AUTO); m_Grid.SetCellDec(CELL_DEC_4);
				m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pPile->m_dm_Edge), nFormat); 
				m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);
			}
		}
	}
	else
	{
		// (ARCBRIDGE-2777) ���� ���Ա��� Df �Է� �߰�. - ����������϶��� ����ȯ�濡�� �������� �ް� ������ �Ѱ�����϶��� ���ʺ��� ����.
		m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "������ ��ȿ ���Ա���", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pFooting->m_dDf, DT_RIGHT);m_Grid.SetCellDec(CELL_DEC_3);
		m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dDf), DT_RIGHT);m_Grid.SetCellDec(CELL_DEC_3);
		m_Grid.AddCell(row++, col+4, "m", GVHT_LEFT,CELL_READONLY);

		sArr.RemoveAll();
		sArr.Add("��12N + 15");
		sArr.Add("��12N + 20");
		sArr.Add("��12N + 25");
		sArr.Add("��15N + 15");
		sArr.Add("��20N + 15");
		sArr.Add("0.3N + 27");
		sArr.Add("������Է�"); // 6
		m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "���������� ���θ�����", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pFooting->m_nTypeEquation_Friction, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);

		m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "���������� ���θ����� ������Է�", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pFooting->m_dFrictionAngle_User, DT_RIGHT, pFooting->m_nTypeEquation_Friction==6 ? CELL_LINE_NO : CELL_READONLY);
		m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dFrictionAngle_User), nFormat); 
		m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);

		if(m_bDesignLsd)
		{
			if(bRock == FALSE)
			{
				szUnit_CGS   = _T("tonf/m��");
				szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);

				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, "SPT Ÿ����հ�(Ncorr)", GVHT_LEFT);
				m_Grid.AddCell(row, col+2, &pFooting->m_dCountHitSPT_Ncorr, DT_RIGHT);
				m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dCountHitSPT_Ncorr), nFormat); 
				m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);
				col = 0;			
			}
			else
			{
				szUnit_CGS   = "tonf/m��";
				szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);

				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, "���� ������భ��(qo)", GVHT_LEFT);

				m_Grid.AddCell(row, col+2, &pFooting->m_dUnconfinedCompressiveStrength_Rock, DT_RIGHT);
/*				m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply); */
				m_Grid.SetCellDec(CELL_DEC_3);

				m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dUnconfinedCompressiveStrength_Rock), nFormat); 
				m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
				m_Grid.SetCellDec(CELL_DEC_3);

				m_Grid.AddCell(row++, col+4, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
// 				m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
				m_Grid.SetCellDec(CELL_DEC_3);

// 				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
// 				m_Grid.AddCell(row, col+1, "Rock Quality Rating(RMR)", GVHT_LEFT);
// 				sArr.RemoveAll();
// 				sArr.Add(_T("100"));
// 				sArr.Add(_T("85"));
// 				sArr.Add(_T("65"));
// 				sArr.Add(_T("44"));
// 				sArr.Add(_T("������Է�"));
// 				m_Grid.AddCell(row, col+2, &pFooting->m_nRockMassRating, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
// 				m_Grid.AddCell(row++, col+4, _T(""), nFormat, CELL_READONLY);
// 
// 				//���� RMR�� ����� �Է��� ��� �Է��� �޵��� ����
// 				UINT nLineUser = pFooting->m_nRockMassRating == 4? CELL_LINE_NO : CELL_READONLY;
// 				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
// 				m_Grid.AddCell(row, col+1, "Rock Quality Rating(RMR)", GVHT_LEFT);
// 				m_Grid.AddCell(row, col+2, &pFooting->m_dUserRockMassRating, DT_RIGHT, nLineUser);
// 				m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dUserRockMassRating), nFormat, nLineUser); 
// 				m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);

				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, "Rock Quality Designation(RQD)", GVHT_LEFT);
				m_Grid.AddCell(row, col+4, _T("%"), GVHT_LEFT, CELL_READONLY);
				m_Grid.AddCell(row++, col+2, &pFooting->m_dRockQualityDesignation, DT_RIGHT, nLine);
// 				m_Grid.SetCellDec(CELL_DEC_3);
			}

			m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
			m_Grid.AddCell(row, col+1, "���ħ�Ϸ� (Sa)", GVHT_LEFT);
			m_Grid.AddCell(row, col+2, &pFooting->m_dAllowableSubsidence, DT_RIGHT);
			m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dAllowableSubsidence), nFormat); 
			m_Grid.AddCell(row++, col+4, _T("mm"), GVHT_LEFT, CELL_READONLY);

			m_Grid.AddCell(row, col++, sHead, GVHT_LEFT);
			m_Grid.AddCell(row, col++, "������ ���� ������(��z)", GVHT_LEFT);
			m_Grid.AddCell(row, col++, &pFooting->m_dFormFactorByShape, DT_RIGHT); m_Grid.SetCellDec(CELL_DEC_2);
			m_Grid.AddCell(row, col++, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dFormFactorByShape), nFormat); m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
			m_xyButtonFactor.x	= col--;
			m_xyButtonFactor.y	= row;
			m_Grid.AddCell(row++, col, _T(""), nFormat, CELL_READONLY);
			col = 0;
		}
	}

	szUnit_CGS   = "tonf/m��";
	szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "���������� �����߷� (r1)", GVHT_LEFT);

	m_Grid.AddCell(row, col+2, &pFooting->m_dUnitWeigthBaseGround, DT_RIGHT);
	m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply); 
	m_Grid.SetCellDec(m_bDesignLsd? CELL_DEC_6 : CELL_DEC_2);

	m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dUnitWeigthBaseGround), nFormat); 
	m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
	m_Grid.SetCellDec(m_bDesignLsd? CELL_DEC_6 : CELL_DEC_2);

	m_Grid.AddCell(row++, col+4, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.SetCellDec(m_bDesignLsd? CELL_DEC_6 : CELL_DEC_2);

	if(m_bDesignLsd==FALSE || bPile==FALSE)
	{
		m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "���������� �����߷� (r2)", GVHT_LEFT);

		m_Grid.AddCell(row, col+2, &pFooting->m_dDfGroundUnitWeight, DT_RIGHT);
		m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply); 
		m_Grid.SetCellDec(m_bDesignLsd? CELL_DEC_6 : CELL_DEC_2);

		m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dDfGroundUnitWeight), nFormat); 
		m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
		m_Grid.SetCellDec(m_bDesignLsd? CELL_DEC_6 : CELL_DEC_2);

		m_Grid.AddCell(row++, col+4, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
		m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
		m_Grid.SetCellDec(m_bDesignLsd? CELL_DEC_6 : CELL_DEC_2);
	}

	// (ARCBRIDGE-3488) �������������϶� �����Լ�(��) �ɼ� �߰�
	sHead = _T("���ݺ������");
	if(!bPile)
	{
		m_Grid.AddCell(row, col, sHead);
		m_Grid.AddCell(row, col+1, "�������(��) ���", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pFooting->m_dFactorAlpha[0], DT_RIGHT); m_Grid.SetCellDec(CELL_DEC_0);
		m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dFactorAlpha[0]), nFormat); 
		m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);

		m_Grid.AddCell(row, col, sHead);
		m_Grid.AddCell(row, col+1, "�������(��) ������", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pFooting->m_dFactorAlpha[1], DT_RIGHT); m_Grid.SetCellDec(CELL_DEC_0);
		m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dFactorAlpha[1]), nFormat); 
		m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);

		if(m_bDesignLsd)
		{
			if(bRock)
			{
				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, "�Ϲ��� �ҿ��Ӹ��� �󵵸� ����� ���װ��(��E)", GVHT_LEFT);
				sArr.RemoveAll();
				sArr.Add(_T("0.023(RQD) - 1.32"));
				sArr.Add(_T("������Է�"));
				m_Grid.AddCell(row, col+2, &pFooting->m_nCoefficientResistance_Rock, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
				m_Grid.AddCell(row, col+3, _T(""), GVHT_LEFT, CELL_READONLY);
				m_Grid.AddCell(row++, col+4, _T(""), nFormat, CELL_READONLY);

				// �Ϲ��� �ҿ��Ӹ��� �󵵸� ����� ���װ��(��E)�� ����� �Է��� ��� �Է��� �޵��� ����
				UINT nLineUser = pFooting->m_nCoefficientResistance_Rock == 1? CELL_LINE_NO : CELL_READONLY;
				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, "�Ϲ��� �ҿ��Ӹ��� �󵵸� ����� ���װ��(��E)", GVHT_LEFT);

				m_Grid.AddCell(row, col+2, &pFooting->m_dUserCoefficientResistance_Rock, DT_RIGHT, nLineUser);
				//m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply); 
				m_Grid.SetCellDec(CELL_DEC_3);

				m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dUserCoefficientResistance_Rock), nFormat, nLineUser); 
				m_Grid.SetCellDec(CELL_DEC_3);
				m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);

				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, "�Ϲ��� ������� (Em)", GVHT_LEFT);
				sArr.RemoveAll();
				sArr.Add(_T("��E ��Eo"));
				sArr.Add(_T("������Է�"));
				m_Grid.AddCell(row, col+3, _T(""), GVHT_LEFT, CELL_READONLY);
				m_Grid.AddCell(row, col+2, &pFooting->m_nModulusDeformation_Rock, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
				m_Grid.AddCell(row++, col+4, _T(""), nFormat, CELL_READONLY);


				// �Ϲ��� �Ϲ��� ������� (Em) �� ����� �Է��� ��� �Է��� �޵��� ����
				nLineUser = pFooting->m_nModulusDeformation_Rock == 1? CELL_LINE_NO : CELL_READONLY;
				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, "�Ϲ��� ������� (Em)", GVHT_LEFT);

				m_Grid.AddCell(row, col+2, &pFooting->m_dUserModulusDeformation_Rock, DT_RIGHT, nLineUser);
				m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply); 
				m_Grid.SetCellDec(CELL_DEC_3);

				m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dUserModulusDeformation_Rock), nFormat, nLineUser); 
				m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
				m_Grid.SetCellDec(CELL_DEC_3);
				m_Grid.AddCell(row++, col+4, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
			}
		}
		if(m_bDesignLsd==FALSE || pBri->IsBoxType())
		{
			long nIdxFooting = m_comboFooting.GetCurSel();

			sArr.RemoveAll();
			sArr.Add(_T("�Է°� x N"));
			sArr.Add(_T("�Է°�"));
			if(pBri->IsBoxType()==FALSE)
				sArr.Add(_T("�������� ����"));

			m_Grid.AddCell(row, col++, sHead, GVHT_LEFT);
			m_Grid.AddCell(row, col++, _T("���� ���ݺ������(Eo) ����"), GVHT_LEFT);
			m_Grid.AddCell(row, col++, &pBri->m_nSelect_Eo_VertType[nIdxFooting], DT_RIGHT, TRUE, CELL_LINE_NO, _T(""), CELL_TYPE_COMBO, sArr);
			m_Grid.AddCell(row, col++, m_pIn->GetStringArray(sArr,pBri->m_nSelect_Eo_VertType[nIdxFooting]), nFormat, nLine);
			m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
			row++;
			col= 0;

			szUnit_CGS   = UNIT_CGS_KGF_CM2;
			szUnit_Apply = pStd->IsSI() ? "kN/m��" : UNIT_CGS_KGF_CM2;
			if(m_bDesignLsd)
				szUnit_Apply = pStd->IsSI() ? pStd->m_szTonf_M2 : pStd->m_szKgf_CM2;

			m_Grid.AddCell(row, col++, sHead, GVHT_LEFT);
			m_Grid.AddCell(row, col++, _T("���� ���ݺ������(Eo)"), GVHT_LEFT);
			if(pBri->m_nSelect_Eo_VertType[nIdxFooting]==0)
			{
				m_Grid.AddCell(row, col++, &pFooting->m_dNValue_Eo_Vert, DT_RIGHT, CELL_LINE_NO);	m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply); m_Grid.SetCellDec(m_bDesignLsd ? CELL_DEC_3 : CELL_DEC_0);
				m_Grid.AddCell(row, col++, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dNValue_Eo_Vert), nFormat); m_Grid.SetCellDec(m_bDesignLsd ? CELL_DEC_3 : CELL_DEC_0);
				m_Grid.AddCell(row, col++, _T("x N"), GVHT_LEFT, CELL_READONLY);
			}
			else
			{
				m_Grid.AddCell(row, col++, &pFooting->m_dEo_Input_Vert, DT_RIGHT, pBri->m_nSelect_Eo_VertType[nIdxFooting]==2 ? CELL_READONLY : CELL_LINE_NO);	m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply); m_Grid.SetCellDec(CELL_DEC_0);
				m_Grid.AddCell(row, col++, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dEo_Input_Vert), nFormat); m_Grid.SetCellDec(CELL_DEC_0);
				m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
			}
		}
	}

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	
 	m_Grid.MergeGrid(0,row, 2,3, TRUE,FALSE);
	m_Grid.SetColumnWidthAll(100);
	m_Grid.SetColumnWidth(1, (!m_bDesignLsd && bPile) ? 320 : 300);
	m_Grid.SetColumnWidth(2, (m_bDesignLsd && bPile==FALSE) ? 100 : 80);
	m_Grid.SetColumnWidth(3, 80);	
}

// nIdxFooting : ���� �������� ������ �ε���
// ���� �������� �߰���ü ������ ����.
// ������ü�̸� NULL
CWallApp* CInputGenFooting::GetInWall(long nIdxFooting)
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return NULL;

	if(nIdxFooting == -1) nIdxFooting = m_comboFooting.GetCurSel();
	if(nIdxFooting < 0) return NULL;

	if(nIdxFooting == 0) 
	{
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2)
		{
			CWallApp *pWall = pBri->GetInWall(nIdxFooting);
			return pWall;
		}
		else
			return NULL;
	}
	else if(nIdxFooting < m_comboFooting.GetCount()-1)
	{
		nIdxFooting -= 1;
		if(nIdxFooting <= pBri->GetCountInWall() && nIdxFooting > -1)
		{
			CWallApp *pWall = pBri->GetInWall(nIdxFooting);
			return pWall;
		}
	}
	else
	{
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2)
		{
			CWallApp *pWall = pBri->GetInWall(pBri->GetCountInWall()-1);
			return pWall;
		}
		else
			return NULL;
	}

	return NULL;	
}

// ����������
void CInputGenFooting::SetGridDataBaseShapeFactor()
{
	CFootingApp *pFooting = GetFooting();
	if(!pFooting) return;
	m_dlgBaseShapeFact.m_pFooting = pFooting;

	CStringArray sArr;
	CString str = _T("");
	CString strHelp = _T("");
	long row = 0;
	long col = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;

	m_Grid.SetGridDefault(2, 2, 1, 0);
	m_Grid.SetCellSize(20, -1);

	m_Grid.AddCell(row, col, "������(X)"); m_Grid.AddCell(row+1, col++, &pFooting->m_dEQShapeFactHorX, nFormat, nLine, "", CELL_TYPE_AUTO, sArr, &m_dlgBaseShapeFact);		m_Grid.SetCellDec(CELL_DEC_3);
	m_Grid.AddCell(row, col, "������(Y)"); m_Grid.AddCell(row+1, col++, &pFooting->m_dEQShapeFactHorY, nFormat, nLine, "", CELL_TYPE_AUTO, sArr, &m_dlgBaseShapeFact);		m_Grid.SetCellDec(CELL_DEC_3);
	m_Grid.AddCell(row, col, "��������(Z)"); m_Grid.AddCell(row+1, col++, &pFooting->m_dEQShapeFactVerZ, nFormat, nLine, "", CELL_TYPE_AUTO, sArr, &m_dlgBaseShapeFact);		m_Grid.SetCellDec(CELL_DEC_3);
	m_Grid.AddCell(row, col, "ȸ����ŷ(X)"); m_Grid.AddCell(row+1, col++, &pFooting->m_dEQShapeFactRotX, nFormat, nLine, "", CELL_TYPE_AUTO, sArr, &m_dlgBaseShapeFact);		m_Grid.SetCellDec(CELL_DEC_3);
	m_Grid.AddCell(row, col, "ȸ����ŷ(Y)"); m_Grid.AddCell(row+1, col++, &pFooting->m_dEQShapeFactRotY, nFormat, nLine, "", CELL_TYPE_AUTO, sArr, &m_dlgBaseShapeFact);		m_Grid.SetCellDec(CELL_DEC_3);
	m_Grid.AddCell(row, col, "��Ʋ��(Z)");   m_Grid.AddCell(row+1, col++, &pFooting->m_dEQShapeFactTorZ, nFormat, nLine, "", CELL_TYPE_AUTO, sArr, &m_dlgBaseShapeFact);	m_Grid.SetCellDec(CELL_DEC_3);

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(80);
}

void CInputGenFooting::SetSizeTabAndGrid()
{
	// ��Ʈ�ѵ� ��� ������.. ���ʰ� ������ �ƴҶ��� �⺻ �ǰ� �׸��带 pos���� �����ʱ��� �÷��ش�.
	// �ƴ� ���� �⺻�ǰ� �׸��带 pos���� ���ʱ��� �ٿ��ش�.
	CRect rect(0, 0, 0, 0);
	CRect rectPos(0, 0, 0, 0);
	m_Tab.GetWindowRect(rect);
	m_TabPos.GetWindowRect(rectPos);

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;
	CExPileApp *pPile	= GetPile();

	BOOL bEarthAnchor = (pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR) ? TRUE : FALSE;

	if(bEarthAnchor == FALSE && (!pPile || pPile->m_bIs == FALSE))
	{
		// ������ �����ؾ� �Ǵ� ũ��
		double dDiff	= rectPos.right - rect.right;
		rect.right += (long)dDiff;

		// ��
		m_Tab.SetWindowPos(&wndTop, rect.left, rect.bottom, rect.Width(), rect.Height(), SWP_NOMOVE);

		// �׸���
		m_Grid.GetWindowRect(rect);
		rect.right += (long)dDiff;
		m_Grid.SetWindowPos(&wndTop, rect.left, rect.bottom, rect.Width(), rect.Height(), SWP_NOMOVE);

		// ���ҿ� ����
		m_GridPos.ShowWindow(FALSE);
		m_TabPos.ShowWindow(FALSE);
	}
	else
	{
		// ������ �����ؾ� �Ǵ� ũ��
		double dDiff	= rectPos.left - rect.right;
		rect.right += (long)dDiff;

		// ��
		m_Tab.SetWindowPos(&wndTop, rect.left, rect.bottom, rect.Width(), rect.Height(), SWP_NOMOVE);

		// �׸���
		m_Grid.GetWindowRect(rect);
		rect.right += (long)dDiff;
		m_Grid.SetWindowPos(&wndTop, rect.left, rect.bottom, rect.Width(), rect.Height(), SWP_NOMOVE);

		// ���ҿ� ����
		m_GridPos.ShowWindow(TRUE);
		m_TabPos.ShowWindow(TRUE);
	}
}


// ������ ���ʼ� ���� ���� ����
void CInputGenFooting::OnButtonSame() 
{
	// TODO: Add your control notification handler code here
	if(AfxMessageBox("��� ������ ���ʺ� ������ �����ϰ� �����Ͻðڽ��ϱ�?", MB_YESNO) != IDYES) return;

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	CFootingApp *pFootingCur	= GetFooting();
	if(!pFootingCur) return;
	if(!pFootingCur->m_bIs) return;
	if(pFootingCur->m_exFooting.m_nType != EXFOOTING_TYPE_LEAN) return;

	
	long nCountFooting	= pBri->GetCountFooting();
	long footing = 0; for(footing = 0; footing < nCountFooting; footing++)
	{
		CFootingApp *pFooting	= pBri->GetFooting(footing);
		if(!pFooting) continue;
		if(!pFooting->m_bIs) continue;

		// 1. �������ݼ���
		pFooting->m_dUnitWeigthBaseGround	= pFootingCur->m_dUnitWeigthBaseGround;
		pFooting->m_Qa_Calculate[0]			= pFootingCur->m_Qa_Calculate[0];
		pFooting->m_Qa_Calculate[1]			= pFootingCur->m_Qa_Calculate[1];
		pFooting->m_Qa_Calculate[0]			= pFootingCur->m_Qa_Calculate[0];
		pFooting->m_Qa_Calculate[1]			= pFootingCur->m_Qa_Calculate[1];
		pFooting->m_QaMax_BaseGround		= pFootingCur->m_QaMax_BaseGround;
		pFooting->m_dFactorPile_qd			= pFootingCur->m_dFactorPile_qd;
		pFooting->m_nTypeEquation_Qa		= pFootingCur->m_nTypeEquation_Qa;
		pFooting->m_nTypeEquation_Friction	= pFootingCur->m_nTypeEquation_Friction;

		// 2. ������ ���԰��
		pFooting->m_dEQSettleFactVerZ		= pFootingCur->m_dEQSettleFactVerZ;
		pFooting->m_dEQSettleFactHorXY		= pFootingCur->m_dEQSettleFactHorXY;
		pFooting->m_dEQSettleFactTorZ		= pFootingCur->m_dEQSettleFactTorZ;
		pFooting->m_dEQSettleFactRotX		= pFootingCur->m_dEQSettleFactRotX;
		pFooting->m_dEQSettleFactRotY		= pFootingCur->m_dEQSettleFactRotY;

		// 3. �簢�� ������ ������
		pFooting->m_dEQShapeFactHorX		= pFootingCur->m_dEQShapeFactHorX;
		pFooting->m_dEQShapeFactHorY		= pFootingCur->m_dEQShapeFactHorY;
		pFooting->m_dEQShapeFactVerZ		= pFootingCur->m_dEQShapeFactVerZ;
		pFooting->m_dEQShapeFactRotX		= pFootingCur->m_dEQShapeFactRotX;
		pFooting->m_dEQShapeFactTorZ		= pFootingCur->m_dEQShapeFactTorZ;
		pFooting->m_dEQSettleFactRotY		= pFootingCur->m_dEQSettleFactRotY;
	}
}


BOOL CInputGenFooting::IsFocusItem(CString &szTitle)
{


	if(GetDlgItem(IDC_GRID_POS)) 
	{
		CGridCtrlMng *pGrid = (CGridCtrlMng *)GetDlgItem(IDC_GRID_POS);
		if(pGrid->GetSafeHwnd())
		{
			for(long nRow=1; nRow<pGrid->GetRowCount(); nRow++)
			{
				long nColCheck	= 0;
				if(pGrid->GetTextMatrix(nRow,nColCheck)==szTitle)
				{
					return TRUE;					
				}
			}
		}
	}
	return FALSE;
}

void CInputGenFooting::SetFocusItem(CString szTitle)
{

	long nColFocus	= 2;
	if(m_nTabPos != 0)
		nColFocus	= 1;

	if(GetDlgItem(IDC_GRID_POS)) 
	{
		CGridCtrlMng *pGrid = (CGridCtrlMng *)GetDlgItem(IDC_GRID_POS);
		if(pGrid->GetSafeHwnd())
		{
			for(long nRow=1; nRow<pGrid->GetRowCount(); nRow++)
			{
				long nColCheck	= 0;
				if(pGrid->GetTextMatrix(nRow, nColCheck)==szTitle)
				{
					GetFocus();
					//pGrid->ResetSelection();
					pGrid->SetSelectedRange(-1,-1,-1,-1);
					pGrid->SetFocusCell(nRow,nColFocus);					
					pGrid->SendMessage(WM_KEYDOWN,VK_F2);
					break;
				}
			}
		}
	}
}

void CInputGenFooting::OnCheckSamedist() 
{
	// TODO: Add your control notification handler code here

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	BOOL bEarthAnchor = (pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR) ? TRUE : FALSE;

	if(bEarthAnchor)
	{
		pBri->m_bAnchorSameValue = m_checkSameDist.GetCheck();
		SetGridData();
		SetDataInit();
		DrawInputDomyunView(FALSE);
	}
	else
	{
		CExPileApp *pPile	= GetPile();
		if(pPile)
		{
			pPile->m_bApplySameDist	= m_checkSameDist.GetCheck();
			if(pPile->m_bApplySameDist)
			{
				pPile->ApplySameDist();
				DrawInputDomyunView(FALSE);
			}
		}
	}
}

void CInputGenFooting::SetNValueByBoring(CFootingApp *pFooting)
{
	if(pFooting==NULL)	return;
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
	if(pPile)
	{
		CBoringData *pBoring = NULL;
		CBoringDataEx *pBoringEx = m_pIn->GetRcBridgeRebar()->GetBoringDataExByJijumNumber(m_comboFooting.GetCurSel());
		if(pBoringEx)
		{
			double dElTop = pFooting->m_dEL - pFooting->GetHeight();
			pBoring = pBoringEx->GetBoringByElevation(toM(dElTop));
			pPile->m_dBeta_N = pBoring ? pBoring->m_nNValue : 0;

			pBoring = pBoringEx->GetBoringByElevation(toM(dElTop-pPile->m_dLength));
			pPile->m_dEdge_N = pBoring ? pBoring->m_nNValue : 0;
			pFooting->SetHyunTaPileByBoring(pBoring);
		}
	}
}

void CInputGenFooting::OnCheckPileKind() 
{
	UpdateData(TRUE);

	CExPileApp *pPile = GetPile();
	if(pPile) pPile->m_bApplyConstruct = m_bApplyPileConstruct;
	SetControl();
}


void CInputGenFooting::SetEpInnerFill(CExPileApp *pPile)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	double dWC = pStd->m_pBasicConcInfo->m_UWeightConcrete > 0 ? toTon(pStd->m_pBasicConcInfo->m_UWeightConcrete) : 2.350;
	double dFck	= pPile->m_dFck_InnerFill;
	pPile->m_dEp_InnerFill	= GetValueEcConc(dFck, dWC, pStd->m_pBasicConcInfo->IsSI(), pStd->m_nTypeKciCode==1);
}

void CInputGenFooting::OnCheckDist() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CExPileApp *pPile = GetPile();
	if(pPile && m_bCheckDist)
	{
		long nCount = pPile->GetCountCheckDistPile();
		CString str = _T("");
		if(nCount <= 0)
			str = _T("�����ǰ� �ִ� ������ �����ϴ�. ");
		else
			str.Format("[ %d ] ���� ������ �����ǰ� �ֽ��ϴ�. ��ġ�� ������ �ּ���.", nCount);

		AfxMessageBox(str);
	}

	DrawInputDomyunView(FALSE);
}


void CInputGenFooting::OnButtonFce()
{
	CDialogFce dlg;
	dlg.DoModal();
}

void CInputGenFooting::DrawSectionBoxTypeView( CDomyun *pDomP )
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	CDomyun Dom(pDomP);

	CFootingApp *pFooting = GetFooting();
	if(pFooting == NULL) return;

	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, m_pIn->m_pARcBridgeDataStd);

//	long nTypeJongdanForRebarOrg = pBri->m_nTypeJongdanForRebar;
//	pBri->m_nTypeJongdanForRebar = (pBri->IsBoxType())? 2 : 0;

	pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, (pBri->IsBoxType())? 2 : 0);

	DBStd.m_pDrawVertSection->DrawVertSection(&Dom, TRUE, &pBri->m_tvArrVertSection);
	DBStd.m_pDrawExFooting->DrawFront(&Dom, &pFooting->m_exFooting, FALSE);

	long nCount = m_pIn->GetRcBridge()->GetCountInWall();

	for(long ix = 0; ix < nCount; ix++)
	{
		CWallApp *pWall = m_pIn->GetRcBridge()->GetInWall(ix);
		if(pWall)
			DBStd.m_pDrawWall->DrawFront(&Dom, pWall, TRUE);
	}

	BOOL bEarthAnchor = (pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR) ? TRUE : FALSE;

	CExPileApp *pPile = GetPile();
	if(bEarthAnchor || pPile)
	{
		if(pBri->IsBoxType())
		{
			DBStd.m_pDrawVertSection->DrawExFootingBoxType(&Dom, FALSE);
		}
	
		// �������� �׸�
		for(long nJ=0; nJ<pBri->GetCountJijum(); nJ++)
		{
			CBoringDataEx *pBoringEx	= pBri->GetBoringDataExByJijumNumber(nJ);
			DBStd.m_pDrawLine->DrawBroing(&Dom, pBoringEx, TRUE);
		}
	}

	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE);
//	pBri->m_nTypeJongdanForRebar = nTypeJongdanForRebarOrg;

	*pDomP << Dom;
}

void CInputGenFooting::OnCellButtonClick( NMHDR* pNotifyStruct, LRESULT* result )
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	CFootingApp *pFooting = GetFooting();
	long nJ	= m_comboFooting.GetCurSel();
	CBoringData *pBoring = pBri->GetBoringDataJijumBase(nJ);
	if(pFooting == NULL || pBoring == NULL) return;

	NM_GRIDVIEW *pGridView = (NM_GRIDVIEW *)pNotifyStruct;			
	int nRow = pGridView->iRow;
// 	int nCol = pGridView->iColumn;

	if(nRow == m_xyButton.y) //���������� ��Ī������ ������ 
	{
		if(pFooting->m_nTypeBaseGround < 2)
		{
			CARcBridgeCalcStd CalcStd(pBri);
			CalcStd.m_pFooting->CalculateAll();

			CLsdFootingDesign *pLsdFooting = CalcStd.m_pFooting->m_pListLsdFootingSafe[nJ];
			if(pLsdFooting == NULL) return;
			double dMaxRate = pLsdFooting->GetMaxHV();

			CDialogNominalSPT dlg;

			dlg.m_bDefault = m_bSPTDefault;
			dlg.SetFooting(pFooting);
			dlg.SetLength(pFooting->GetWidth());
			dlg.SetRateHV(Round(dMaxRate, 3));

			if(dlg.DoModal() == IDOK)
			{
				SetGridData();
				SetDataInit();
				DrawInputDomyunView(FALSE);
			}

			m_bSPTDefault = FALSE;
		}
		else
		{
			CDialogNominalRock dlg;

			dlg.SetFooting(pFooting);
			if(dlg.DoModal() == IDOK)
			{
				SetGridData();
				SetDataInit();
				DrawInputDomyunView(FALSE);
			}
		}
	}
	//������ ���� ������
	else if(nRow == m_xyButtonFactor.y)
	{
		CDialogShapeFactor dlg;

		if(dlg.DoModal() == IDOK)
		{
			SetGridData();
			SetDataInit();
			DrawInputDomyunView(FALSE);
		}
	}
}


void CInputGenFooting::OnBnClickedButtonHyunta()
{
	CExPileApp *pPile = GetPile();
	if(pPile)
	{
		// ������ �ǰ�� ���� ���� ���¶�� �ǰ���� �� �ش�.
// 		if(pPile->m_pHyunTa->m_nHP_MainRebarDanSu == 0)
// 		{
// 			pPile->SetDatDefaultHyunTaRebar();
// 			pPile->m_pHyunTa->m_dHP_Fy	= m_pIn->GetCurrentAbut()->m_dFy.Get();
// 		}

		CInputGenHyunTa dlg;
		dlg.SetDataStd(m_pIn);
//		dlg.m_pDataStd = m_pIn->m_pAAbutPierDataStd;
// 		dlg.SetExPile(pPile, m_pIn->m_pARoadOptionStd);
		dlg.DoModal();
	}	
}