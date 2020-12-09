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
					m_pIn->SetComment("사용자가 입력한 지지력을 사용합니다. 별첨자료는 별도로 첨부해야합니다", TRUE);
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

		if(sTitle.Find(_T("지지력 산정")) != -1)
		{
			SetGridData();
			SetDataInit();
		}
		else
		{
			if(sTitle.Find(_T("자동계산")) != -1 || sTitle.Find(_T("기초지반의 내부마찰각")) != -1)
			{
				SetGridData();
				SetDataInit();
			}
		}

		if( (sHead.Find(_T("지반변형계수")) != -1 && sTitle.Find(_T("지반변형계수(Eo) 적용")) != -1) || (sHead.Find(_T("기초지반")) != -1 && sTitle.Find(_T("지반변형계수(Eo)")) != -1) )
		{
			if(pBri->m_nSelect_Eo_VertType[nIdxFooting]==2)
			{
				pFooting->m_dEo_Input_Vert = pFooting->m_dBoringEo;
				SetGridData();
				SetDataInit();
			}
		}

		if(sTitle == _T("지반정보 자동적용"))
		{
			HGINT32 nSelFooting = m_comboFooting.GetCurSel();
			if( nSelFooting < 0 ) return;

			if ( pBri->m_bAutoGroundInfo[nSelFooting] == TRUE )
				pBri->SetDataDefaultBaseGround(nSelFooting);

			SetGridData();
			SetDataInit();
		}
		else if(sTitle == _T("기초지반의 종류"))
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
		if(sTitle == _T("말뚝지반 자동설정"))
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
// 	// 설계 선택사항에서 "횡방향 말뚝간격 동일적용"을 변경하면 다시 그려야 됨.
// 	if(m_nTab == m_nTabSelect)
// 	{
// 		if(sTitle == "횡방향 말뚝간격 동일적용")
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
	// 일반제원에서 말뚝길이를 변경하면 모든 말뚝에 적용되어야 한다
		if(sTitle == "말뚝의 길이")
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
		else if(sTitle == "속채움 콘크리트 강도")
		{
			CExPileApp *pPile = GetPile();
			if(pPile)
			{
				SetDataSave();
				SetEpInnerFill(pPile);
				SetDataInit();
			}
		}
		else if(sTitle == "말뚝의 직경")
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
		else if( sTitle == "한계선단지지력 (qt)" || sTitle == "말뚝선단의 기초지반")
		{
			SetDataSave();
			SetGridData();
			SetDataInit();
		}
	}
	else if(m_nTab == m_nTabPileProperty)
	{
		if(sTitle.Find(_T("지반변형계수(Eo) 적용")) != -1 || sTitle.Find(_T("자동")) != -1 && sTitle.Find(_T("계산")) != -1)
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
		// 종방향말뚝배치에서 경사각을 바꿨다면.. 해당하는 열은 모두 경사각을 바꿔야 된다.
		if(m_nTabPos == m_nTabPosVert)
		{
			if(sTitle == "경사각")
			{
				CExPileApp *pPile = GetPile();
				if(pPile)
				{
					long nOrg = cell.row-1;
					CPileAttr *pPileAttrStt = pPile->GetPileAttr(nOrg, 0);	// 현재열의 첫번째 말뚝.
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
				// 첫번재 열의 간격을 바꿨다면.. 횡방향 동일간격적용이 해준다.
				if(sTitle == "간격")
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
			// 앵커 횡단배치를 동일하게 설정
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

	// 콤보들의 데이타값 저장
	CExPileApp *pPile = GetPile();
	if(pPile)
	{
		BOOL bHyunTa = (pPile->m_nType == PILE_TYPE_HYUNJANG);
		BOOL bPHC	= pPile->m_nType == PILE_TYPE_PHC || pPile->m_nType == PILE_TYPE_PHC_DIGGING;

		// 말뚝 타입에 따라 입력창이 없어지는 값들 초기화
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

		// 파일 길이는 지반에 따라서 다시 권고안 함. //////////
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

	m_sArrTabPosName.Add("종방향 배치"); m_nTabPosVert = nTab++;

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
	

	// 제원관련
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
	
	// 배치관련
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
		m_pIn->GridMessage(&m_Grid, "기초가 존재 하지 않습니다");
		m_pIn->GridMessage(&m_GridPos, "기초가 존재 하지 않습니다");
		return;
	}
//	if(pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_MASS) ; MASS기초 = 직접기초
//	{
//		m_pIn->GridMessage(&m_Grid, "MASS기초는 입력할 내용이 없습니다.");
//		m_pIn->GridMessage(&m_GridPos, "MASS기초는 입력할 내용이 없습니다.");
//		return;
//	}

	if(m_bDesignLsd && m_nTab == m_nTabFootingGround)
	{
		m_Grid.SetTextMatrix((int)m_xyButton.y, (int)m_xyButton.x, "설정");
		m_Grid.SetCellType((int)m_xyButton.y, (int)m_xyButton.x, CT_BUTTON);
		m_Grid.SetTextMatrix((int)m_xyButtonFactor.y, (int)m_xyButtonFactor.x, "기준보기");
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
		// 말뚝 종류 콤보
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

		// 종방향 말뚝 본수 
		m_comboVertSu.EnableWindow(TRUE);
		m_comboVertSu.SetCurSel(pPile->m_nCountOrg-1);
		GetDlgItem(IDC_STATIC_KIND)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_VERT_SU)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_VERT_SU)->SetWindowText(_T("종방향 말뚝 본수"));
		GetDlgItem(IDC_CHECK_SAMEDIST)->SetWindowText(_T("말뚝간격 동일적용"));

		// 제원참조 버튼
		if(pPile->IsPHC()) m_btnJewon.EnableWindow(TRUE);
		else m_btnJewon.EnableWindow(FALSE);

		//PHC 두부손상 버튼
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

		m_comboKindPile.SetWindowText("말뚝기초아님");
		m_comboKindPile.EnableWindow(FALSE);

		m_comboKindPile2.SetWindowText("말뚝기초아님");
		m_comboKindPile2.EnableWindow(FALSE);

		if(bEarthAnchor)
		{
			m_comboVertSu.EnableWindow(TRUE);
			m_comboVertSu.SetCurSel(pBri->m_nCountAnchor - 1);
			GetDlgItem(IDC_STATIC_VERT_SU)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_VERT_SU)->SetWindowText(_T("종방향 앵커 개수"));
			m_checkSameDist.EnableWindow(TRUE);
			m_checkSameDist.SetCheck(pBri->m_bAnchorSameValue);
			GetDlgItem(IDC_CHECK_SAMEDIST)->SetWindowText(_T("앵커배치 동일적용"));
		}
		else
		{
			m_comboVertSu.SetWindowText("없음");
			m_comboVertSu.EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_VERT_SU)->EnableWindow(FALSE);
			m_checkSameDist.EnableWindow(FALSE);
		}
		GetDlgItem(IDC_STATIC_KIND)->EnableWindow(FALSE);

		// 버튼
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

		// 벽체 평면 그림
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
		// 교량 평면
		DBStd.m_pDrawPlane->DrawPlaneCenterLineEachSlab(&Dom);
		DBStd.m_pDrawPlane->DrawPlaneSlabOnlyOutLine(&Dom);
			
		// 벽체 및 기둥 평면
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

		// 날개벽
		for(long nLR = iLEFT; nLR <= iRIGHT; nLR++)
		{
			pBri->GetTvWingWallPlane(nLR == iLEFT);
			DBStd.m_pDrawWingWall->DrawGenPlane(&Dom, nLR == iLEFT);
		}

		// 기초 평면
		DBStd.m_pDrawExFooting->DrawPlan(&Dom, &pFooting->m_exFooting, FALSE, FALSE, TRUE, m_bCheckDist);
	}

	// 교량 진행 방향으로 회전
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

	CDRect rectBridge;	// 교량의 사각 영역
	BOOL bApplyAngle	= FALSE;
	pBri->GetTvVertSection_Std(0, bApplyAngle, FALSE, FALSE, (pBri->IsBoxType())? 2 : 0);
	rectBridge = pBri->m_tvArrVertSection.GetRect();

	// 종단면
	CDRect rectVert;
	CDPoint xyVert(0, 0);

	// 중간벽체편집중이면 중간벽체를 그리고
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
				if(tv.m_sInfo == _T("벽체하면우측") || tv.m_sInfo == _T("벽체하면좌측"))
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
	// 외측벽체 편집중이면 외측벽체를 그린다
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
			// 기초와 벽체가 이어지는 선 만들어 준다.
			BOOL b1=FALSE, b2=FALSE;
			CTwinVector tvFootingUp, tvFootingExt;

			if(m_comboFooting.GetCurSel()==0)
			{
				b1 = pFooting->m_tvArrFront.GetTvByInfo(_T("기초상면우측"), tvFootingUp);
				b2 = pFooting->m_tvArrFront.GetTvByInfo(_T("기초우측확장상면"), tvFootingExt);
			}
			else
			{
				b1 = pFooting->m_tvArrFront.GetTvByInfo(_T("기초상면좌측"), tvFootingUp);
				b2 = pFooting->m_tvArrFront.GetTvByInfo(_T("기초좌측확장상면"), tvFootingExt);
			}

			CTwinVector tv;
			CTwinVectorArray tvArr;
			long n=0; for(n=0; n<tvTot.GetSize(); n++)
			{
				tv = tvTot.GetAt(n);
				if(tv.m_sInfo == _T("기초우측경사"))
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
				else if(tv.m_sInfo == _T("기초좌측경사"))
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
	
		// 지반정보 그림
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
	// 종단면
	CDomyun DomVert(pDom);
	if(pBri->IsBoxType())
		DrawSectionBoxTypeView(&DomVert);
	else
		DrawSectionView(&DomVert);

	DomVert.SetCalcExtRect();
	CDRect rectVert;
	rectVert = DomVert.GetExtRect();

	// 지반선 그림
	DBStd.m_pDrawVertSection->DrawElByStation(&DomVert, rectVert.left-DomVert.Always(10), rectVert.right+DomVert.Always(10));
	DomVert.SetCalcExtRect();
	rectVert = DomVert.GetExtRect();

	CDPoint xyVert(0, 0);
	xyVert = CDPoint(rectVert.right, rectVert.bottom-rectVert.Height()/2);
	*pDom << DomVert;

	//////////////////////////////////////////////////////////////////////////
	// 평면
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

	// 교량의 형식이 모두 슬래브교일 경우 기둥 위치 입력 이후의 메뉴를 표시하지 않음
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
	
	// 교량
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

	// 기초위치
	long nCount = m_comboFooting.GetCount();
	for(i = 0; i < nCount; i++)
	{
		m_comboFooting.DeleteString(0);
	}

	m_comboFooting.AddString("시점부");
	if(pBri->IsBoxType() == FALSE)
	{
		for(i = 0; i < pBri->GetCountInWall(); i++)
		{
			str.Format("중간 지점부 %d", i+1);
			m_comboFooting.AddString(str);
		}
		m_comboFooting.AddString("종점부");
	}
	m_comboFooting.SetCurSel(0);

	// 말뚝 종류
	// 파일 종류 콤보 설정(자동설정) ㅋㅋ
	CExPileApp pile;
	nCount = m_comboKindPile.GetCount();
	for(i = 0; i < nCount; i++)
		m_comboKindPile.DeleteString(0);
	for(i = 0; i < PILE_TYPE_SU; i++)
	{
		// 한계상태에선 내진말뚝 지원 안함.
		if( i == PILE_TYPE_EHP && m_bDesignLsd == TRUE )
			continue;
		pile.m_nType = i;
		m_comboKindPile.AddString(pile.GetStringPileType());
	}
	//
	nCount = m_comboKindPile2.GetCount();
	for(i = 0; i < nCount; i++)
		m_comboKindPile2.DeleteString(0);
	m_comboKindPile2.AddString("타입말뚝(타격공법)");
	m_comboKindPile2.AddString("타입말뚝(바이브로해머공법)");
	m_comboKindPile2.AddString("현장타설말뚝");
	m_comboKindPile2.AddString("내부굴착말뚝");
	m_comboKindPile2.AddString("프리보링말뚝");
	m_comboKindPile2.AddString("쏘일시멘트말뚝");

	// 최대 파일 열 수 설정콤보
	nCount = m_comboVertSu.GetCount();
	for(i = 0; i < nCount; i++)
	{
		m_comboVertSu.DeleteString(0);
	}

	long nMax = (pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR) ? RCBRIDGE_ANCHOR_MAX : MAX_PILE_SU;
	for(i = 0; i < nMax; i++)
	{
		str.Format("%d열", i+1);
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

// 일반제원 입력내용
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
	BOOL bPileASD = (pStd->IsApplyBoredPileASD(pPile) || pStd->IsApplyPhcPileASD(pPile))? TRUE : FALSE; // 매입말뚝 또는 PHC말뚝의 허용응력설계법 적용 

	m_Grid.InitGrid(pPile->m_dRebar_Fy);
	m_Grid.SetGridDefault(2, 2, 1, 2);
	m_Grid.SetCellSize(20, -1);

	m_Grid.AddCell(row, col++, "구  분"); 
	m_Grid.AddCell(row, col++, "구  분"); 
	m_Grid.AddCell(row, col++, "설정값"); 
	m_Grid.AddCell(row, col++, "단  위");
	row++;
	col= 0;

	CString szUnit_CGS   = "";
	CString szUnit_Apply = "mm";

	CString sHead = _T("말뚝제원");
	m_Grid.AddCell(row, col++, sHead); 
	if(bEHP)
	{
		long i = 0; for(i = 0; i < EHP_KIND_SU; i++)
			sArr.Add(pPile->GetStringEHPType(i));

		m_Grid.AddCell(row, col++, "말뚝의 직경", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pPile->m_nKind_EHP, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
		m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	}
	else
	{
		m_Grid.AddCell(row, col++, "말뚝의 직경", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pPile->m_dDia, DT_RIGHT, nLine, "pD : PILE의 직경(mm)", CELL_TYPE_AUTO);	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
		m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	}
	row++;
	col= 0;

	m_Grid.AddCell(row, col++, sHead); 
	m_Grid.AddCell(row, col++, "말뚝의 길이", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pPile->m_dLength, DT_RIGHT, nLine, "pL : PILE의 길이(mm)", CELL_TYPE_AUTO);	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
	m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	row++;
	col= 0;
	// 2005-01-18 11:38오전 ===>
	if(!bHyunTa)
	{
		if(bEHP)
		{
			sArr.RemoveAll();
			pPile->GetStringArrayEHPThick(sArr);
			m_Grid.AddCell(row, col++, sHead); 
			m_Grid.AddCell(row, col++, "말뚝의 두께", GVHT_LEFT); 
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
			m_Grid.AddCell(row, col++, "말뚝의 두께", GVHT_LEFT); 
			m_Grid.AddCell(row, col++, &pPile->m_dT1, DT_RIGHT, nLine, "pT : PILE의 두께(mm)", CELL_TYPE_AUTO); m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
			m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
			row++;
			col= 0;
		}

		if(!bPHC)
		{
			m_Grid.AddCell(row, col++, sHead); 
			m_Grid.AddCell(row, col++, "말뚝의 부식두께", GVHT_LEFT); 
			m_Grid.AddCell(row, col++, &pPile->m_dT2, DT_RIGHT, nLine, "cT : PILE의 부식두께(mm)", CELL_TYPE_AUTO); m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
			m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
			row++;
			col= 0;
		}
	}
	// 2005-01-18 11:38오전 <===

	sHead = _T("말뚝설치조건");
	m_Grid.AddCell(row, col++, sHead); 
	m_Grid.AddCell(row, col++, "말뚝선단의 근입깊이", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pPile->m_dDf, DT_RIGHT, nLine, "Df : PILE선단의 근입깊이(mm)", CELL_TYPE_AUTO); m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
	m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	row++;
	col= 0;

	m_Grid.AddCell(row, col++, sHead); 
	m_Grid.AddCell(row, col++, "원지반으로부터 돌출된 길이", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pPile->m_dH0, DT_RIGHT, nLine, "Ho : 원지반으로부터 돌출된 길이(mm)", CELL_TYPE_AUTO);	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
	m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	row++;
	col= 0;

	nLine = m_bDesignLsd && pPile->m_nType == PILE_TYPE_PHC ? CELL_READONLY : CELL_LINE_NO;
	sHead = _T("말뚝지반특성");
	m_Grid.AddCell(row, col++, sHead); 
	m_Grid.AddCell(row, col++, "말뚝지반 자동설정", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pBri->m_bPileNvalueAuto, DT_RIGHT, nLine, pBri->m_bPileNvalueAuto ? _T("적용") : _T("적용안함"), CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
	row++;
	col= 0;

	nLine = CELL_LINE_NO;
	UINT nLineAuto = pBri->m_bPileNvalueAuto ? CELL_READONLY : CELL_LINE_NO;
	m_Grid.AddCell(row, col++, sHead); 
	m_Grid.AddCell(row, col++, "말뚝머리의 평균 N치", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pPile->m_dBeta_N, DT_RIGHT, nLineAuto, "bN : PILE머리의 평균 N치", CELL_TYPE_AUTO);
	m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
	row++;
	col= 0;

	m_Grid.AddCell(row, col++, sHead); 
	m_Grid.AddCell(row, col++, "말뚝선단의 평균 N치", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pPile->m_dEdge_N, DT_RIGHT, nLineAuto, "eN : PILE선단의 평균 N치", CELL_TYPE_AUTO);
	m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
	row++;
	col= 0;

	if(m_bDesignLsd == FALSE)
	{
		szUnit_CGS   = "";
		szUnit_Apply = "mm";
		m_Grid.AddCell(row, col++, sHead); 
		m_Grid.AddCell(row, col++, "압밀층의 두께", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pPile->m_dThickApmeel, DT_RIGHT, CELL_LINE_NO, "aT : 압밀층의 두께(mm)", CELL_TYPE_AUTO);	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
		m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
		row++;
		col= 0;

		sArr.RemoveAll();
		szUnit_CGS   = "kgf/cm²";
		szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
		sHead = _T("두부보강");
		m_Grid.AddCell(row, col++, sHead); 
		m_Grid.AddCell(row, col++, "두부보강철근 항복강도", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pPile->m_dRebar_Fy, DT_RIGHT, CELL_LINE_NO, "두부보강철근 항복강도", CELL_TYPE_AUTO); m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
		m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
		row++;
		col= 0;

		m_Grid.AddCell(row, col++, sHead); 
		m_Grid.AddCell(row, col++, "두부보강철근 직경", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pPile->m_dRebar_Dia, DT_RIGHT, CELL_LINE_NO, "", CELL_TYPE_DIA);
		m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
		row++;
		col= 0;

		m_Grid.AddCell(row, col++, sHead); 
		m_Grid.AddCell(row, col++, "두부보강철근 개수", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pPile->m_dRebar_Ea, DT_RIGHT, CELL_LINE_NO, "", CELL_TYPE_AUTO);
		m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
		row++;
		col= 0;
	}

	if( bSteel == TRUE )
	{
		szUnit_Apply = "N/mm²";
		sHead = _T("강관말뚝");
		if(m_bDesignLsd == FALSE || bPileASD)
		{
			m_Grid.AddCell(row, col++, sHead); 
			m_Grid.AddCell(row, col++, "강관말뚝의 허용 휨응력", GVHT_LEFT); 
			m_Grid.AddCell(row, col++, &pPile->m_dFa_Steel, DT_RIGHT, CELL_LINE_NO, "강관말뚝의 허용 휨응력", CELL_TYPE_AUTO);	m_Grid.SetCellUnit(UNIT_CGS_KGF_CM2,pStd->m_szKgf_CM2); 
			m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
			row++;
			col= 0;

			m_Grid.AddCell(row, col++, sHead); 
			m_Grid.AddCell(row, col++, "강관말뚝의 허용 전단응력", GVHT_LEFT); 
			m_Grid.AddCell(row, col++, &pPile->m_dTa_Steel, DT_RIGHT, CELL_LINE_NO, "강관말뚝의 허용 전단응력", CELL_TYPE_AUTO); m_Grid.SetCellUnit(UNIT_CGS_KGF_CM2,pStd->m_szKgf_CM2); 
			m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
			row++;
			col= 0;
		}

		if( m_bDesignLsd == TRUE && bPileASD == FALSE)
		{
			m_Grid.AddCell(row, col++, sHead); 
			m_Grid.AddCell(row, col++, "강관말뚝의 항복응력(Fy)", GVHT_LEFT); 
			m_Grid.AddCell(row, col++, &pPile->m_dYieldStress_SteelPile, DT_RIGHT, CELL_LINE_NO, "강관말뚝의 항복응력(Fy)", CELL_TYPE_AUTO); m_Grid.SetCellUnit(UNIT_CGS_KGF_CM2, pStd->m_szKgf_CM2); 
			m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
			row++;
			col= 0;
		}
	}

	if(bEHP && m_bDesignLsd == FALSE)
	{
		szUnit_CGS   = "kgf/cm²";
		szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);

		sHead = _T("EHP");
		m_Grid.AddCell(row, col++, sHead); 
		m_Grid.AddCell(row, col++, "속채움 콘크리트 강도", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pPile->m_dFck_InnerFill, DT_RIGHT, nLine, "Fck : 콘크리트 설계기준 강도", CELL_TYPE_AUTO);	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
		m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
		row++;
		col= 0;

		m_Grid.AddCell(row, col++, sHead); 
		m_Grid.AddCell(row, col++, "속채움 콘크리트 탄성계수", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pPile->m_dEp_InnerFill, DT_RIGHT, nLine, "Ep : 콘크리트 탄성계수", CELL_TYPE_AUTO); m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
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

	m_Grid.AddCell(row, col++, _T("구  분")); 
	m_Grid.AddCell(row, col++, _T("구  분")); 
	m_Grid.AddCell(row, col++, _T("설정값"));
	m_Grid.AddCell(row, col++, _T("비 고"));
	row++;
	col	= 0;

	CString sHead = _T("말뚝특성치");
	m_Grid.AddCell(row, col++, sHead);
	m_Grid.AddCell(row, col++, _T("자동계산"), GVHT_LEFT);
	str = pBri->m_bAutoCalcPileProperty[nIdxFooting] ? _T("적용") : _T("적용안함");
	m_Grid.AddCell(row, col++, &pBri->m_bAutoCalcPileProperty[nIdxFooting], DT_RIGHT, CELL_LINE_NO, str, CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
	row++;
	col	= 0;

	UINT nLineProperty = pBri->m_bAutoCalcPileProperty[nIdxFooting] ? CELL_READONLY : CELL_LINE_NO;

	CString szUnit_CGS   = _T("tonf/m");	
	CString szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS); 
	m_Grid.AddCell(row, col++, sHead);
	m_Grid.AddCell(row, col++, _T("말뚝의 축방향 스프링정수 (Kv) : 0 = 자동계산"), GVHT_LEFT);
	m_Grid.AddCell(row, col++, &pPile->m_dKv_Input_Pile, DT_RIGHT, nLineProperty);	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.SetCellDec(bLSD ? CELL_DEC_5 : CELL_DEC_3);
	m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	row++;
	col	= 0;

	szUnit_CGS   = _T("kgf/cm³");
	szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS); 
	long nIdx = bLSD ? PILE_CASE_ULTIMATE : PILE_CASE_NORMAL;
	m_Grid.AddCell(row, col++, sHead);
	m_Grid.AddCell(row, col++, _T("수평방향 지반반력 계수 (Kh, 상시) : 0 = 자동계산"), GVHT_LEFT);
	m_Grid.AddCell(row, col++, &pPile->m_dKh_Input_Pile[nIdx], DT_RIGHT, nLineProperty);	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.SetCellDec(CELL_DEC_5);
	m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	row++;
	col	= 0;

	nIdx = bLSD ?  PILE_CASE_EXTREME : PILE_CASE_EARTH;
	m_Grid.AddCell(row, col++, sHead);
	m_Grid.AddCell(row, col++, _T("수평방향 지반반력 계수 (Kh, 지진시) : 0 = 자동계산"), GVHT_LEFT);
	m_Grid.AddCell(row, col++, &pPile->m_dKh_Input_Pile[nIdx], DT_RIGHT, nLineProperty);	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.SetCellDec(CELL_DEC_5);
	m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	row++;
	col	= 0;

	// m-1은 단위 변환이 안되므로 단위를 거꾸로 줬다.
	szUnit_CGS		= bLSD ? _T("mm") : _T("m");
	szUnit_Apply	= _T("m");
	nIdx = bLSD ? PILE_CASE_ULTIMATE : PILE_CASE_NORMAL;
	m_Grid.AddCell(row, col++, sHead);
	m_Grid.AddCell(row, col++, _T("말뚝의 탄성치 (β,상시) : 0 = 자동계산"), GVHT_LEFT);		
	m_Grid.AddCell(row, col++, &pPile->m_dBeta_Input_Pile[nIdx], DT_RIGHT, nLineProperty); m_Grid.SetCellDec(CELL_DEC_5);	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(row, col++, bLSD ? _T("mm-1") : _T("m-1"), GVHT_LEFT, CELL_READONLY);
	m_Grid.SetCellDec(CELL_DEC_5);
	row++;
	col	= 0;

	szUnit_CGS		= bLSD ? _T("mm") : _T("m");
	szUnit_Apply	= _T("m");
	nIdx = bLSD ?  PILE_CASE_EXTREME : PILE_CASE_EARTH;
	m_Grid.AddCell(row, col++, sHead);
	m_Grid.AddCell(row, col++, _T("말뚝의 탄성치 (β,지진시) : 0 = 자동계산"), GVHT_LEFT);
	m_Grid.AddCell(row, col++, &pPile->m_dBeta_Input_Pile[nIdx], DT_RIGHT, nLineProperty); m_Grid.SetCellDec(CELL_DEC_5);	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(row, col++, bLSD ? _T("mm-1") : _T("m-1"), GVHT_LEFT, CELL_READONLY);
	m_Grid.SetCellDec(CELL_DEC_5);
	row++;
	col	= 0;

	m_Grid.AddCell(row, col++, sHead);
	m_Grid.AddCell(row, col++, _T("변형계수(α) 상시"), GVHT_LEFT);
	m_Grid.AddCell(row, col++, &pFooting->m_dFactorAlpha[0], nFormat);
	m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
	row++;
	col	= 0;

	m_Grid.AddCell(row, col++, sHead);
	m_Grid.AddCell(row, col++, _T("변형계수(α) 지진시"), GVHT_LEFT);
	m_Grid.AddCell(row, col++, &pFooting->m_dFactorAlpha[1], nFormat);
	m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
	row++;
	col	= 0;

	sArr.RemoveAll();
	sArr.Add(_T("입력값 x N"));
	sArr.Add(_T("입력값"));
	//	sArr.Add(_T("지반정보 적용"));
	m_Grid.AddCell(row, col++, sHead);
	m_Grid.AddCell(row, col++, _T("수평 지반변형계수(Eo) 적용"), GVHT_LEFT);
	m_Grid.AddCell(row, col++, &pBri->m_nSelect_Eo_HoriType[nIdxFooting], DT_RIGHT, TRUE, CELL_LINE_NO, _T(""), CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
	row++;
	col= 0;

	szUnit_CGS	 = _T("kgf/cm²");
	szUnit_Apply = pStd->IsSI() ? pStd->m_szTonf_M2 : pStd->m_szKgf_CM2;

	m_Grid.AddCell(row, col++, sHead);
	m_Grid.AddCell(row, col++, _T("수평 지반변형계수(Eo)"), GVHT_LEFT);
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

// PHC 제원 입력내용
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
	// (ARCBRIDGE-2713) PHC말뚝 제원 입력 허용
	UINT nLine = CELL_LINE_NO;	// m_bDesignLsd? CELL_READONLY : CELL_LINE_NO;

	m_Grid.SetGridDefault(10, 3, 1, 1);
	m_Grid.SetCellSize(20, -1);

	CString szUnit_CGS   = "kgf/cm²";
	CString szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);

	m_Grid.AddCell(row, col, "구  분"); m_Grid.AddCell(row, col+1, "설정값"); m_Grid.AddCell(row++, col+2, "단  위");
	m_Grid.AddCell(row, col, "콘크리트 설계기준 강도", GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dFck_PHC, DT_RIGHT, nLine, "Fck : 콘크리트 설계기준 강도", CELL_TYPE_AUTO);
	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);

	m_Grid.AddCell(row, col, "콘크리트 탄성계수", GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dEp_PHC, DT_RIGHT, nLine, "Ep : 콘크리트 탄성계수", CELL_TYPE_AUTO);
	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);

	if(pPile->m_nType == PILE_TYPE_PHC || pPile->m_nType == PILE_TYPE_PHC_DIGGING)
	{
		m_Grid.AddCell(row, col, _T("유효프리스트레스(본체)"), GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dFce1_PHC, DT_RIGHT, nLine, _T("Fce1 : 유효프리스트레스(본체)"), CELL_TYPE_AUTO);
		m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);

		CString strMsg = _T("시공시 말뚝두부를 절단,파쇄하지 않거나 보강등의 방법으로 두부의 유효프리스트레스의 감소를 방지하는 경우는 설계자의 기술적 판단으로 유효프리스트레스(두부) 값을 조정하시기 바랍니다.");
		m_Grid.AddCell(row, col, _T("유효프리스트레스(두부)"), GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dFce2_PHC, DT_RIGHT, nLine, strMsg, CELL_TYPE_AUTO);
		m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);

		m_Grid.AddCell(row, col, _T("두부의 범위(말뚝상단기준)"), GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dLenHead_PHC, DT_RIGHT, nLine, _T("두부의 범위(말뚝상단기준)"), CELL_TYPE_AUTO);
		m_Grid.SetCellUnit(_T("mm"), UNIT_CGS_CM); m_Grid.AddCell(row++, col+2, UNIT_CGS_CM, GVHT_LEFT, CELL_READONLY);

	}
	else
	{
		// 굴착 PHC 또는 SPC
		m_Grid.AddCell(row, col, _T("유효프리스트레스(본체)"), GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dFce1_PHC, DT_RIGHT, nLine, _T("Fce : 유효프리스트레스"), CELL_TYPE_AUTO);
		m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);

	}

	szUnit_CGS		= "cm²";
	szUnit_Apply	= pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(row, col, "콘크리트 환산단면적", GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dAe_PHC, DT_RIGHT, nLine, "Ae : 콘크리트 환산단면적", CELL_TYPE_AUTO);
	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	
	szUnit_CGS		= "cm⁴";
	szUnit_Apply	= pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(row, col, "환산단면 2차 모멘트", GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dIe_PHC, DT_RIGHT, nLine, "Ie : 환산단면 2차 모멘트", CELL_TYPE_AUTO);
	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);

	szUnit_CGS		= "cm³";
	szUnit_Apply	= pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(row, col, "환산단면계수", GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dZe_PHC, DT_RIGHT, nLine, "Ze : 환산단면계수", CELL_TYPE_AUTO);
	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);

	sArr.RemoveAll();  sArr.Add("A"); sArr.Add("B"); sArr.Add("C"); 
	m_Grid.AddCell(row, col, "종별", GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_nKind_PHC, DT_RIGHT, TRUE, nLine, "Type : 종별", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+2, _T(""), GVHT_LEFT, CELL_READONLY);

	szUnit_CGS   = "kgf/cm²";
	szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(row, col, "허용 휨 압축응력(평상시)", GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dFca_PHC[0], DT_RIGHT, nLine, "Fca1 : 허용 휨 압축응력(평상시)", CELL_TYPE_AUTO);
	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);

	szUnit_CGS   = "kgf/cm²";
	szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(row, col, "허용 휨 압축응력(지진시)", GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dFca_PHC[1], DT_RIGHT, nLine, "Fca2 : 허용 휨 압축응력(지진시)", CELL_TYPE_AUTO);
	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);

	szUnit_CGS   = "kgf/cm²";
	szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(row, col, "허용 휨 인장응력(평상시)", GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dFta_PHC[0], DT_RIGHT, nLine, "Fta1 : 허용 휨 인장응력(평상시)", CELL_TYPE_AUTO);
	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);

	szUnit_CGS   = "kgf/cm²";
	szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(row, col, "허용 휨 인장응력(지진시)", GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dFta_PHC[1], DT_RIGHT, nLine, "Fta2 : 허용 휨 인장응력(지진시)", CELL_TYPE_AUTO);
	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);

	szUnit_CGS   = "kgf/cm²";
	szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(row, col, "허용 전단응력(평상시)", GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dTa_PHC[0], DT_RIGHT, nLine, "Ta1 : 허용 전단응력(평상시)", CELL_TYPE_AUTO);
	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); m_Grid.AddCell(row++, col+2, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	
	szUnit_CGS   = "kgf/cm²";
	szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(row, col, "허용 전단응력(지진시)", GVHT_LEFT); m_Grid.AddCell(row, col+1, &pPile->m_dTa_PHC[1], DT_RIGHT, nLine, "Ta2 : 허용 전단응력(지진시)", CELL_TYPE_AUTO);
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

//  종방향 말뚝 배치 그리드 데이타
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
		// 기준면에 있는 각 말뚝의 첫번째 말뚝의 정보 표시
		m_GridPos.AddCell(row, col++, "구분");
		m_GridPos.AddCell(row, col++, "개수");
		m_GridPos.AddCell(row, col++, "간격");
		m_GridPos.AddCell(row, col, "경사각");
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
				m_GridPos.AddCell(row, col++, &pOrg->countSide, nFormat, FALSE, nLine, str+"에 배치되는 말뚝의 개수", CELL_TYPE_AUTO);
				m_GridPos.AddCell(row, col++, &pOrg->dist, nFormat, nLine, str+"과 이전열과의 간격", CELL_TYPE_AUTO);
				m_GridPos.AddCell(row, col, &pPileAttr->ang, nFormat, nLine, str+"에 배치되는 말뚝의 경사 각도", CELL_TYPE_AUTO);
				row++;
				col = 0;
			}
		}
	}
	else
	{
		// 기준면에 있는 각 말뚝의 첫번째 말뚝의 정보 표시
		m_GridPos.AddCell(row, col++, "구분");
		m_GridPos.AddCell(row, col++, "거리");
		m_GridPos.AddCell(row, col++, "개수");
		m_GridPos.AddCell(row, col++, "직경");
		m_GridPos.AddCell(row, col++, "길이");
		m_GridPos.AddCell(row, col, "앵커력");
		row++;
		col = 0;

		for(long ix = 0; ix < pBri->m_nCountAnchor; ix++)
		{
			str.Format("D%d", ix+1);

			m_GridPos.AddCell(row, col++, str); 
			m_GridPos.AddCell(row, col++, &pBri->m_dArrDistJongAnchor, ix, nFormat, nLine, str+"과 이전열과의 간격", CELL_TYPE_AUTO); m_GridPos.SetCellDec(CELL_DEC_1);
			m_GridPos.AddCell(row, col++, &pBri->m_dArrEaAnchor, ix, nFormat, (ix == 0 || pBri->m_bAnchorSameValue == FALSE) ?  nLine : CELL_READONLY, str+"에 배치되는 Anchor의 개수", CELL_TYPE_AUTO);
			m_GridPos.AddCell(row, col++, &pBri->m_dArrDiaAnchor, ix, nFormat, (ix == 0 || pBri->m_bAnchorSameValue == FALSE) ?  nLine : CELL_READONLY, str+"에 배치되는 Anchor의 직경", CELL_TYPE_AUTO);
			m_GridPos.AddCell(row, col++, &pBri->m_dArrLengthAnchor, ix, nFormat, (ix == 0 || pBri->m_bAnchorSameValue == FALSE) ?  nLine : CELL_READONLY, str+"에 배치되는 Anchor의 길이", CELL_TYPE_AUTO);

			CString szUnit_CGS   = "tonf";
			CString szUnit_Apply = m_pIn->m_pARcBridgeDataStd->GetStringUnitType(szUnit_CGS);
			m_GridPos.AddCell(row, col, &pBri->m_dArrLoadAnchor, ix, nFormat, nLine, str+"에 적용되는 Anchor 하중", CELL_TYPE_AUTO);
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
			AfxMessageBox("모델링 해석을 수행후에 설계검토를 하시기 바랍니다.");
		}
		else
		{
			CInputCheckGrid Dlg(pBri,IDD_GEN_FOOTING, nFooting);
			Dlg.DoModal();
		}
	}
	else
	{
		AfxMessageBox("말뚝기초가 아니므로 검토항목이 없습니다.");
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

	// 기준면에 있는 각 말뚝의 첫번째 말뚝의 정보 표시
	m_GridPos.AddCell(row, col++, "구분");
	m_GridPos.AddCell(row, col++, "간격");
	if(bEarthAnchor == FALSE)
	{
		m_GridPos.AddCell(row, col++, "경사각");
		m_GridPos.AddCell(row, col++, "길이");
	}
	row++;
	col = 0;

	if(bEarthAnchor == FALSE)
	{
		// 시점에서 봤을때 왼쪽 연단 거리
		COrgInfo *pOrg = pPile->m_pArrOrgInfo.GetAt(nIdxOrg);
		long i = 0; for(i = 0; i < pOrg->countSide; i++)
		{
			CPileAttr *pPileAttr = pPile->GetPileAttr(nIdxOrg, i);

			str.Format("y%d", i+1);

			m_GridPos.AddCell(row, col++, str); 
			if(pPileAttr)
			{
				m_GridPos.AddCell(row, col++, &pPileAttr->dist, nFormat, (pPile->m_bApplySameDist && nIdxOrg > 0) ? CELL_READONLY : CELL_LINE_NO, str+"에 배치되는 말뚝과 이전행의 말뚝과의 간격", CELL_TYPE_AUTO);
				m_GridPos.AddCell(row, col++, &pPileAttr->ang, nFormat, CELL_LINE_NO, str+"에 배치되는 말뚝의 경사 각도", CELL_TYPE_AUTO);
				m_GridPos.AddCell(row, col, &pPileAttr->len , nFormat, CELL_LINE_NO, str+"에 배치되는 말뚝의 길이", CELL_TYPE_AUTO);
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
			m_GridPos.AddCell(row, col, &pBri->m_dArrDistHDanAnchor[nIdxOrg], ix, nFormat, (pBri->m_bAnchorSameValue == FALSE || nIdxOrg == 0) ? nLine : CELL_READONLY, str+"에 배치되는 앵커와 이전행의 앵커와의 간격", CELL_TYPE_AUTO); m_GridPos.SetCellDec(CELL_DEC_1);

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
		m_sArrTabName.Add("말뚝지반설정"); m_nTabFootingGround= nTab++;
		m_sArrTabName.Add("설계선택사항"); m_nTabSelect = nTab++;
		m_sArrTabName.Add("말뚝특성치");	m_nTabPileProperty = nTab++;
		m_sArrTabName.Add("말뚝제원");	m_nTabPileJewon = nTab++;
		if(pPile->IsPHC())
		{
			m_sArrTabName.Add("PHC제원"); m_nTabPHC= nTab++;
		}
		if(pBri->IsBoxType())
		{
			m_sArrTabName.Add("구속조건모델링"); 
			m_nTabModeling = nTab++;
		}
	}
	else
	{
		m_sArrTabName.Add("기초지반설정"); m_nTabFootingGround = nTab++;
		if(pBri->IsBoxType())
		{
			m_sArrTabName.Add("구속조건모델링"); 
			m_nTabModeling = nTab++;
		}
		if(bEarthQuakeDesign && pBri->IsBoxType() == FALSE)
		{
			m_sArrTabName.Add("기초의 근입계수"); 
			m_nTabFact = nTab++;
			m_sArrTabName.Add("사각형 기초의 형상계수"); 
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

	m_Grid.AddCell(row, col++, "구  분"); 
	m_Grid.AddCell(row, col++, "구  분"); 
	m_Grid.AddCell(row, col++, "설정값");
	m_Grid.AddCell(row, col++, "단 위");
	row++;
	col = 0;

	CARcBridgeDataStd *pStd = m_pIn->m_pARcBridgeDataStd;

	CString szUnit_CGS   = _T("tonf/m²");	//UNIT_CGS_TONF_M2;
	CString szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);

	sArr.RemoveAll();
	sArr.Add("Terzaghi 보정식");
	sArr.Add("Terzaghi 보정식 사용자 입력");
	sArr.Add("사용하지않음");
	m_Grid.AddCell(row, col++, "지반반력계수(Kv) 산정시 Terzaghi 보정", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, "지반반력계수(Kv) 산정시 Terzaghi 보정", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pBri->m_nGroundFactorKV, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	row++;
	col = 0;
	//
	m_Grid.AddCell(row, col++, "지반반력계수(Kv) 산정시 Terzaghi 보정", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, "지반반력계수(Kv) 산정시 Terzaghi 보정", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pBri->m_dGroundFactorKV, DT_RIGHT, pBri->m_nGroundFactorKV == 1 ? CELL_LINE_NO : CELL_READONLY);
	m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	row++;
	col = 0;
	//
	sArr.RemoveAll();
	sArr.Add("횡방향(B:교량연장)");
	sArr.Add("종방향(B:교량폭)");
	m_Grid.AddCell(row, col++, "기초환산폭(Bv)식의 폭(B) 적용방식", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, "기초환산폭(Bv)식의 폭(B) 적용방식", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pBri->m_nGroundFactorBv, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	row++;
	col = 0;
	//
	sArr.RemoveAll();
	sArr.Add(_T("적용"));
	sArr.Add(_T("적용안함"));
	m_Grid.AddCell(row, col++,_T(" 등가정적 하중법 해석시 전단지반계수 적용"),GVHT_LEFT);
	m_Grid.AddCell(row, col++,_T(" 등가정적 하중법 해석시 전단지반계수 적용"),GVHT_LEFT);
	m_Grid.AddCell(row, col++, &pBri->m_nEquivalentApplyKsb, DT_RIGHT,TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	row++;
	col = 0;

	m_Grid.AddCell(row, col++,_T(" 전단지반반력계수 산정시 Ksb = λ × Kv, λ = 1 / 입력값"),GVHT_LEFT);
	m_Grid.AddCell(row, col++,_T(" 전단지반반력계수 산정시 Ksb = λ × Kv, λ = 1 / 입력값"),GVHT_LEFT);
	m_Grid.AddCell(row, col++,&pBri->m_dEquivalentApplyKsb,DT_RIGHT, pBri->m_nEquivalentApplyKsb==0 ? nLine : CELL_READONLY);
	m_Grid.SetCellDec(CELL_DEC_1); 
	row++;
	col = 0;

	// (40867) 해석툴이 Midas면 NLLINK 선택 못하도록
	if(pBri->m_nSelect2DFrameTool == SOLVE_MIDAS)
	{
		pBri->m_n2DGeneralSpringType = 1;
		pBri->m_n2DEQSpringType = 1;
		nLineCur = CELL_READONLY;
	}
	sArr.RemoveAll();
	sArr.Add("NLLink를 이용한 모델링");
	sArr.Add("Spring을 이용한 모델링");
	m_Grid.AddCell(row, col++, "지반모델링 방식", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, "2D 평상시 모델링", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pBri->m_n2DGeneralSpringType, DT_RIGHT, TRUE, nLineCur, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col++, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
	row++;
	col = 0;
	//
	m_Grid.AddCell(row, col++, "지반모델링 방식", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, "2D 지진시 모델링", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pBri->m_n2DEQSpringType, DT_RIGHT, TRUE, nLineCur, "", CELL_TYPE_COMBO, sArr);
	row++;
	col = 0;
	//
	if(pBri->IsBoxType())
	{
		col = 0;
		m_Grid.AddCell(row, col++, "절점의 지반반력계수 산정시 벽체두께 ", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, "절점의 지반반력계수 산정시 벽체두께 ", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pBri->m_bSpringSpaceIncludeWallTick, DT_RIGHT,  CELL_LINE_NO, pBri->m_bSpringSpaceIncludeWallTick ? "포함" : "불포함", CELL_TYPE_CHECK);
		row++;
		col = 0;
	}

	sArr.RemoveAll();
	sArr.Add("사질토층이 우세한 경우");
	sArr.Add("점성토층이 우세한 경우");
	sArr.Add("사질토층와 점성토의 혼합층인 경우");
	m_Grid.AddCell(row, col++, "연직방향 지반 반력계수", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, "연직방향 지반 반력계수", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pBri->m_nVerticalFactor, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	row++;
	col = 0;
	//
	m_Grid.AddCell(row, col++, "Spring 기준간격 (mm)", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, "하부 모델링", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pBri->m_dSpringSpaceBottom, DT_RIGHT, CELL_LINE_NO);
	row++;
	col = 0;

	m_Grid.AddCell(row, col++, "Spring 기준간격 (mm)", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, "벽체 (지진시)", GVHT_LEFT); 
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
	
	m_Grid.AddCell(row, col++, "구  분"); 
	m_Grid.AddCell(row, col++, "구  분"); 
	m_Grid.AddCell(row, col++, "설정값");
	m_Grid.AddCell(row, col++, "단 위");
	row++;
	col = 0;

	long nColWidth = 120;

	CString sHead = _T("안정성");

	// 허용응력설계법 적용
	BOOL bPileASD = (pStd->IsApplyBoredPileASD(pPile) || pStd->IsApplyPhcPileASD(pPile)) ? TRUE : FALSE;
	if(m_bDesignLsd == FALSE)
	{
		m_Grid.AddCell(row, col++, sHead); 
		m_Grid.AddCell(row, col++, "군말뚝영향 고려", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pPile->m_bApplyGroupEffect, DT_RIGHT, nLine, pPile->m_bApplyGroupEffect ? _T("적용") : _T("적용안함"), CELL_TYPE_CHECK);
		m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
		row++;
		col = 0;
	}		

	m_Grid.AddCell(row, col++, sHead); 
	m_Grid.AddCell(row, col++, "주면 마찰력 고려", GVHT_LEFT); 
	m_Grid.AddCell(row, col++, &pPile->m_bApplyFriction, DT_RIGHT, nLine, pPile->m_bApplyFriction ? _T("적용") : _T("적용안함"), CELL_TYPE_CHECK);
	m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
	row++;
	col = 0;

	if(m_bDesignLsd && bPileASD == FALSE && (pPile->m_nType == PILE_TYPE_HYUNJANG || pPile->m_nType == PILE_TYPE_PHC))
	{
		sArr.RemoveAll();
		sArr.Add(_T("고려안함"));
		sArr.Add(_T("콘크리트단위중량 고려"));
		sArr.Add(_T("(콘크리트-흙)단위중량 고려"));
		m_Grid.AddCell(row, col++, sHead);
		m_Grid.AddCell(row, col++, _T("지지력검토시 말뚝자중"), GVHT_LEFT);
		m_Grid.AddCell(row, col++, &pPile->m_nSelectExcludeSelfWeight, DT_RIGHT, TRUE, nLine , _T(""), CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
		row++;
		col = 0;
	}

	BOOL bSteel = (pPile->m_nType == PILE_TYPE_STEEL || pPile->m_nType == PILE_TYPE_STEEL_DIGGING) ? TRUE : FALSE;		// 강관
	BOOL bHyunTa = (pPile->m_nType == PILE_TYPE_HYUNJANG);

	if(m_bDesignLsd && (pPile->m_nType == PILE_TYPE_PHC || pPile->m_nType == PILE_TYPE_PHC_DIGGING || bSteel==TRUE))
	{
		m_Grid.AddCell(row, col++, sHead);
		m_Grid.AddCell(row, col++, "비배토 말뚝", GVHT_LEFT);
		m_Grid.AddCell(row, col++, &pPile->m_bApplyNonDisplacementPile, DT_RIGHT, nLine, pPile->m_bApplyNonDisplacementPile ? _T("적용") : _T("적용안함"), CELL_TYPE_CHECK);
		m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
		row++;
		col = 0;

		// 허용응력설계법 적용
		BOOL bPileASD = (pStd->m_nApplyBoredPileASD && pPile->IsBoredPile()) || (pStd->m_nApplyPhcPileASD && pPile->IsPHC()) ? TRUE : FALSE; 
		if(bPileASD == FALSE)
		{
			sArr.RemoveAll();
			sArr.Add(_T("검토안함"));
			sArr.Add(_T("Broms 방법"));
			m_Grid.AddCell(row, col++, sHead);
			m_Grid.AddCell(row, col++, _T("횡방향지지력 검토"), GVHT_LEFT);
			m_Grid.AddCell(row, col++, &pPile->m_nTypeCheckHoriSupportForce, DT_RIGHT, TRUE, nLine , _T(""), CELL_TYPE_COMBO, sArr);
			m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
			row++;
			col = 0;

			nColWidth = 200;
			sArr.RemoveAll();
			sArr.Add(_T("검토안함"));
			sArr.Add(_T("구조물기초설계기준으로 검토"));
			m_Grid.AddCell(row, col++, sHead);
			m_Grid.AddCell(row, col++, _T("침하 검토"), GVHT_LEFT);
			m_Grid.AddCell(row, col++, &pPile->m_nTypeCheckSubsidence, DT_RIGHT, TRUE, nLine , _T(""), CELL_TYPE_COMBO, sArr);
			m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
			row++;
			col = 0;
		}
	}

	sHead = _T("말뚝본체");
	m_Grid.AddCell(row, col++, sHead); 
	m_Grid.AddCell(row, col++, _T("말뚝이음 적용길이"), GVHT_LEFT);
	m_Grid.AddCell(row, col++, &pPile->m_dLengthPileMax, DT_RIGHT, nLine);
	m_Grid.AddCell(row, col++, _T("mm"), GVHT_LEFT, CELL_READONLY);
	row++;
	col = 0;

	if( m_bDesignLsd == FALSE )
	{
		m_Grid.AddCell(row, col++, sHead); 
		m_Grid.AddCell(row, col++, _T("말뚝 이음시 허용응력 감소비"), GVHT_LEFT);
		m_Grid.AddCell(row, col++, &pPile->m_dRateDeductPileStress, DT_RIGHT, nLine);
		m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
		row++;
		col = 0;
	}

	if(bHyunTa)
	{
		sHead = _T("현장타설말뚝");
		m_Grid.AddCell(row, col++, sHead); 
		m_Grid.AddCell(row, col++, "벤토나이트사용", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pPile->m_bApplyBentonite, DT_RIGHT, m_bDesignLsd ? CELL_READONLY : CELL_LINE_NO, pPile->m_bApplyBentonite ? _T("적용") : _T("적용안함"), CELL_TYPE_CHECK);
		m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
		row++;
		col = 0;
	}
	if(pPile->IsPHC())
	{
		sHead = _T("PHC말뚝");
		m_Grid.AddCell(row, col++, sHead); 
		m_Grid.AddCell(row, col++, "ft(휨 인장응력) 검토시 최소축력 적용", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pPile->m_bApplyMinAxialAtStressCheckPHC, DT_RIGHT, m_bDesignLsd ? CELL_READONLY : CELL_LINE_NO, pPile->m_bApplyMinAxialAtStressCheckPHC ? _T("적용") : _T("적용안함"), CELL_TYPE_CHECK);
		m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
		row++;
		col = 0;
	}
	if( m_bDesignLsd == FALSE )
	{
		sHead = _T("기초결합부");
		m_Grid.AddCell(row, col++, sHead); 
		m_Grid.AddCell(row, col++, "가상콘크리트 단면 응력검토", GVHT_LEFT); 
		m_Grid.AddCell(row, col++, &pPile->m_bCheckBaseConcStress, DT_RIGHT, nLine, pPile->m_bCheckBaseConcStress ? _T("적용") : _T("적용안함"), CELL_TYPE_CHECK);
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

// 말뚝 종류 변경시
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

	// EHP인 경우에는 다이얼로그 띄우기
	if(pPile->m_nType == PILE_TYPE_EHP)
	{
		pPile->m_dT2 = 0;
		m_dlgEHPLink.DoModal();
	}
}

// 말뚝 종류 변경시
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

// 필요한 컨트롤들만 보이게 함.
// 파일기초일때와 직접기초일때...
// void CInputGenFooting::MoveControl()
// {
// 	// 기초가 바뀔때마다 필요없는 컨트롤들은 안보이는 곳으로 이동시킨다.
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

	// 처음에는 말뚝이 생성이 안 된 상태이므로 OnInitDialog후에 다시 탭을 설정해준다.
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
		CString str1 = _T("유한장 말뚝의 경우 말뚝의 스프링정수에 보정계수를 적용하여 지원[도설2008 5.8.8.2(1)1)]하며 본체작용력을 산정시 적분상수를 고려하는 유한장말뚝의 계산식은 지원하지 않고 일반식[도설2008 해설표 5.8.27]으로 계산됩니다.\n\n");
		CString str2 = _T("[도로교설계기준2008 5.8.11해설(1)2)]에서 βL이 2.5까지는 반무한장의 식으로 계산해도 좋다고 표기하고 있으며 때문에 βL이 2.5보다 작은 경우는 보정계수입력외에도 설계자의 판단이 필요할 수 있을 것입니다.\n");
		CString str3 = _T("[유한장말뚝 제원]에서 보정계수를 입력하셔야 하며 입력하지 않거나 1.0으로 적용하신다면 실제 말뚝검토는 반무한장말뚝과 동일한 설계방법으로 진행됩니다.\n\n");
		CString str4 = _T("유한장 말뚝은 선단의 지지조건의 영향을 받기때문에 지지조건을 고려할 필요가 있다. 다만 양질의 지지층에 말뚝 지름 정도로 근입된다면 일반적으로 선단힌지로 보는 것이 좋다.[도로교설계기준해설2008 5.8.8.2해설(1)2)]\n");

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

// 기초근입 계수
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
	m_Grid.AddCell(row, col, "수직변위");  m_Grid.AddCell(row+1, col++, &pFooting->m_dEQSettleFactVerZ, nFormat, nLine, "", CELL_TYPE_AUTO, sArr, &m_dlgBaseFact);	m_Grid.SetCellDec(CELL_DEC_3);
	m_Grid.AddCell(row, col, "수평변위");  m_Grid.AddCell(row+1, col++, &pFooting->m_dEQSettleFactHorXY, nFormat, nLine, "", CELL_TYPE_AUTO, sArr, &m_dlgBaseFact);	m_Grid.SetCellDec(CELL_DEC_3);
	m_Grid.AddCell(row, col, "비틀림");    m_Grid.AddCell(row+1, col++, &pFooting->m_dEQSettleFactTorZ, nFormat, nLine, "", CELL_TYPE_AUTO, sArr, &m_dlgBaseFact);	m_Grid.SetCellDec(CELL_DEC_3);
	m_Grid.AddCell(row, col, "회전(X축)"); m_Grid.AddCell(row+1, col++, &pFooting->m_dEQSettleFactRotX, nFormat, nLine, "", CELL_TYPE_AUTO, sArr, &m_dlgBaseFact);	m_Grid.SetCellDec(CELL_DEC_3);
	m_Grid.AddCell(row, col, "회전(Y축)"); m_Grid.AddCell(row+1, col++, &pFooting->m_dEQSettleFactRotY, nFormat, nLine, "", CELL_TYPE_AUTO, sArr, &m_dlgBaseFact);	m_Grid.SetCellDec(CELL_DEC_3);

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
	CString szUnit_CGS   = _T("kgf/cm²");
	CString szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	CStringArray sArr;

	HGINT32 nSelFooting = m_comboFooting.GetCurSel();
	if( nSelFooting < 0 ) return;
	
	CString sHead = _T("기초지반");

	// 자동적용
	m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "지반정보 자동적용", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pBri->m_bAutoGroundInfo[nSelFooting], DT_RIGHT | DT_VCENTER, CELL_LINE_NO, pBri->m_bAutoGroundInfo[nSelFooting] ?  _T("적용") : _T("적용안함"), CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);

	UINT nLineRefLine	= pBri->m_bAutoGroundInfo[nSelFooting]==TRUE ? CELL_READONLY : CELL_LINE_NO;

	// 기초지반의 종류
	sArr.Add(_T("사질토"));
	sArr.Add(_T("점성토"));
	sArr.Add(_T("암반"));
	m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "기초지반의 종류", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pFooting->m_nTypeBaseGround, DT_RIGHT, TRUE, nLineRefLine , _T(""), CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+4, _T(""), nFormat, CELL_READONLY);

	// 기초지반의 N치
	m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "기초지반의 N치", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pFooting->m_dNValueBaseGround, nFormat, nLineRefLine);
	m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dNValueBaseGround), nFormat, nLineRefLine);
	m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);

	// 기초지반의 점착력
	szUnit_CGS = UNIT_CGS_TONF_M2;
	szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "기초지반의 점착력", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pFooting->m_C_BaseGround, nFormat, nLineRefLine);
	m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.SetCellDec(CELL_DEC_0);
	m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_C_BaseGround), nFormat, nLineRefLine);
	m_Grid.AddCell(row++, col+4, szUnit_Apply, GVHT_LEFT, CELL_READONLY);

	if(pBri->IsBoxType() == FALSE)
	{
		// 지반변형계수
		szUnit_CGS = UNIT_CGS_KGF_CM2;
		szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS, 2);
		m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "지반변형계수(Eo)", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pFooting->m_dBoringEo, nFormat, nLineRefLine);
		m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
		m_Grid.SetCellDec(CELL_DEC_0);
		m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dBoringEo), nFormat, nLineRefLine);
		m_Grid.AddCell(row++, col+4, szUnit_Apply, GVHT_LEFT, CELL_READONLY);

		if(m_bDesignLsd)
		{
			// 지반 포아송비
			m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
			m_Grid.AddCell(row, col+1, "지반포아송비(γ)", GVHT_LEFT);
			m_Grid.AddCell(row, col+2, &pFooting->m_dBoringRatePoasong, nFormat, nLineRefLine);
			m_Grid.SetCellDec(CELL_DEC_3);
			m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);
		}
	}

	if( m_bDesignLsd == FALSE )
	{
		szUnit_CGS   = "tonf/m²";
		szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
		m_Grid.AddCell(row, col, sHead);
		m_Grid.AddCell(row, col+1, "최대허용 지지력", GVHT_LEFT);
		m_Grid.AddCell(row, col+4, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
		m_Grid.AddCell(row++, col+2, &pFooting->m_QaMax_BaseGround, DT_RIGHT);
		m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
		m_Grid.SetCellDec(CELL_DEC_2);
	}

	col = 0;
	sArr.RemoveAll();
	if( m_bDesignLsd == TRUE )
	{
		if( pFooting->m_nTypeBaseGround < 2)//사질토,점성토
			sArr.Add("표준관입시험(SPT)방법");
		else
		{
			sArr.Add("Hoek & Brown 제안식");
			sArr.Add("AASHTO 제안식"); 
			sArr.Add("Canadian Geotechnical Society");
			sArr.Add("전체검토");
		}
	}
	else
	{
		sArr.Add("Terzaghi식");
		sArr.Add("Meyerhof식");
		sArr.Add("Hansen  식");

	}
	m_Grid.AddCell(row, col++, sHead);
	m_Grid.AddCell(row, col++, m_bDesignLsd==TRUE ? "공칭지지력 산정식" : "허용지지력 산정식", GVHT_LEFT);
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
	BOOL bPileASD = (pStd->IsApplyBoredPileASD(pPile) || pStd->IsApplyPhcPileASD(pPile))? TRUE : FALSE; // 매입말뚝 또는 PHC말뚝의 허용응력설계법 적용 

	CFootingApp *pFooting = GetFooting();
	if(!pFooting) return;

	UINT nLine = CELL_LINE_NO;
	UINT nFormat	= DT_RIGHT | DT_VCENTER;

	CString sHead = _T("지지력");
	m_Grid.AddCell(row, col, sHead);
	m_Grid.AddCell(row, col+1, "자동계산", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pFooting->m_bAutoCalcSupportForce, DT_RIGHT | DT_VCENTER, nLine, pFooting->m_bAutoCalcSupportForce ?  _T("적용") : _T("적용안함"), CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);

	CString str = _T("");
	CString szUnit_CGS   = "tonf/m²";
	CString szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);

	CString strStatus[3] = {_T(" : 사용한계상태"), _T(" : 극한한계상태"), _T(" : 극단상황한계상태")};
	if(m_bDesignLsd == FALSE)
	{
		strStatus[0] = _T(" : 평상시");
		strStatus[1] = _T(" : 지진시");
	}

	CStringArray sArr;
	if(!bPile)
	{
		long nCountCase = m_bDesignLsd ? 3 : 2;

		for(long ix=0; ix<nCountCase; ix++)
		{
			if((m_bDesignLsd == FALSE && bEarthQuakeDesign == FALSE) && ix == 1) continue;
			str.Format(_T("%s%s"), _T("계산된 허용지지력"), strStatus[ix]);

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
		// 말뚝 지지력은 본당 지지력
		CString szUnit_CGS   = "tonf";
		CString szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);

		// 말뚝 공통
		long nCountCase = m_bDesignLsd ? 3 : 2;
		for(long ix=0; ix<nCountCase; ix++)
		{
			if((m_bDesignLsd == FALSE && bEarthQuakeDesign == FALSE) && ix == 1) continue;
			if(m_bDesignLsd && bPileASD && ix == 1) continue;
			if(m_bDesignLsd && bEarthQuakeDesign == FALSE && ix == 2) continue;

			m_Grid.AddCell(row, col, sHead);
			if(m_bDesignLsd)
				str.Format(_T("%s%s"), _T("계산된 허용지지력 : "), strStatus[ix]);
			else
				str.Format(_T("%s%s"), _T("계산된 허용지지력 : "), ix == 0 ? _T("평상시") : _T("지진시"));
			
			m_Grid.AddCell(row, col+1, str, GVHT_LEFT);

			m_Grid.AddCell(row, col+2, &pFooting->m_Qa_Calculate[ix], DT_RIGHT, pFooting->m_bAutoCalcSupportForce ? CELL_READONLY : CELL_LINE_NO);
			m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply); 
			m_Grid.SetCellDec(m_bDesignLsd ? CELL_DEC_0 : CELL_DEC_2);

			m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_Qa_Calculate[ix]), nFormat); 
			m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
			m_Grid.SetCellDec(m_bDesignLsd? CELL_DEC_0 : CELL_DEC_2);

			szUnit_CGS   = _T("tonf");
			szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
			str.Format(_T("%s/본"),szUnit_Apply);
			m_Grid.AddCell(row++, col+4, str, GVHT_LEFT, CELL_READONLY);
		}

		for(long ix=0; ix<nCountCase; ix++)
		{
			if((m_bDesignLsd == FALSE && bEarthQuakeDesign == FALSE) && ix == 1) continue;
			if(m_bDesignLsd && bPileASD && ix == 1) continue;

			m_Grid.AddCell(row, col, sHead);
			if(m_bDesignLsd)
				str.Format(_T("%s%s"), _T("계산된 허용인발력 : "), strStatus[ix]);
			else
				str.Format(_T("%s%s"), _T("계산된 허용인발력 : "), ix == 0 ? _T("평상시") : _T("지진시"));

			m_Grid.AddCell(row, col+1, str, GVHT_LEFT);

			m_Grid.AddCell(row, col+2, &pFooting->m_Pa_Calculate[ix], DT_RIGHT, pFooting->m_bAutoCalcSupportForce ? CELL_READONLY : CELL_LINE_NO);
			m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply); 
			m_Grid.SetCellDec(m_bDesignLsd ? CELL_DEC_0 : CELL_DEC_2);

			m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_Pa_Calculate[ix]), nFormat); 
			m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
			m_Grid.SetCellDec(m_bDesignLsd? CELL_DEC_0 : CELL_DEC_2);

			szUnit_CGS   = _T("tonf");
			szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
			str.Format(_T("%s/본"),szUnit_Apply);
			m_Grid.AddCell(row++, col+4, str, GVHT_LEFT, CELL_READONLY);
		}
	}
}

// 기초 지반 설정
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
	BOOL bPileASD = (pStd->IsApplyBoredPileASD(pPile) || pStd->IsApplyPhcPileASD(pPile))? TRUE : FALSE; // 매입말뚝 또는 PHC말뚝의 허용응력설계법 적용 
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

	m_Grid.AddCell(row, col,	  "구  분");
	m_Grid.AddCell(row, col+1,	  "구  분");
	m_Grid.AddCell(row, col+2, "설정값");
	m_Grid.AddCell(row++, col+4, "단  위");

	// 지지력 산정, 허용지지력 산정시
	AddCellCalcSupportForce(row, col);

	// 기초 지반
	// (ARCBRIDGE-3498) 강도설계법에서도 일부 입력 적용
	if( !pPile )
	{
 		AddCellFootingGround(row, col);
	}

	CString sHead = _T("기초지반");
	CString szUnit_CGS   = _T("tonf/m²");	//UNIT_CGS_TONF_M2;
	CString szUnit_Apply = m_bDesignLsd ? _T("kN/m²")  : pStd->GetStringUnitType(szUnit_CGS);
	if(bPile)
	{
		if( m_bDesignLsd == FALSE )
		{
			CString strMsg = _T("선단지지력 산정시의 계수는 말뚝공법과 지반조건에 따라 달라질수 있으므로 확인후 사용하시기 바랍니다.");
			m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
			m_Grid.AddCell(row, col+1, "선단지지력 산정시 계수 (qd = 00 x N)", GVHT_LEFT, CELL_LINE_NO);
			m_Grid.AddCell(row, col+2, &pFooting->m_dFactorPile_qd, DT_RIGHT, CELL_LINE_NO, strMsg);
			m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
			m_Grid.SetCellDec(CELL_DEC_3);
			m_Grid.AddCell(row++, col+4, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
		}

		// 강관, 한계상태
		if( (pPile->IsPHC() || pPile->m_nType == PILE_TYPE_STEEL || pPile->m_nType == PILE_TYPE_STEEL_DIGGING) && m_bDesignLsd )
		{
			sArr.RemoveAll();
			sArr.Add(_T("사질토"));
			sArr.Add(_T("점성토"));
			str	= _T("");

			m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
			m_Grid.AddCell(row, col+1, _T("말뚝의 횡방향 지지력 검토 지반"), GVHT_LEFT);
			m_Grid.AddCell(row, col+2, &pPile->m_nTypeGroundCrossCheck, DT_RIGHT | DT_VCENTER, TRUE, nLine , _T(""), CELL_TYPE_COMBO, sArr);
			m_Grid.AddCell(row++, col+4, str, GVHT_LEFT, CELL_READONLY);

			sArr.RemoveAll();
			sArr.Add(_T("0.4 Ncorr"));
			sArr.Add(_T("0.3 Ncorr"));
			sArr.Add(_T("사용자입력"));
			str	= _T("");

			m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
			m_Grid.AddCell(row, col+1, _T("한계선단지지력 (qt)"), GVHT_LEFT);
			m_Grid.AddCell(row, col+2, &pPile->m_nTypeLimitEdgeQt, DT_RIGHT | DT_VCENTER, TRUE, nLine , _T(""), CELL_TYPE_COMBO, sArr);
			m_Grid.AddCell(row++, col+4, str, GVHT_LEFT, CELL_READONLY);

			UINT nLineUser = pPile->m_nTypeLimitEdgeQt == 2? CELL_LINE_NO : CELL_READONLY;
			m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
			m_Grid.AddCell(row, col+1, _T("한계선단지지력 (qt)"), GVHT_LEFT);

			m_Grid.AddCell(row, col+2, &pPile->m_dUserLimitEdgeQt, DT_RIGHT, nLineUser);
			m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply); 
			m_Grid.SetCellDec(CELL_DEC_5);

			m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pPile->m_dUserLimitEdgeQt), nFormat); 
			m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
			m_Grid.SetCellDec(CELL_DEC_5);

			m_Grid.AddCell(row++, col+4, _T("사용자입력값"), GVHT_LEFT, CELL_READONLY);

			szUnit_CGS		= _T("");
			szUnit_Apply	= szUnit_CGS;
			m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
			m_Grid.AddCell(row, col+1, _T("시공방법과 지반조건에 따른 계수(Cp)"), GVHT_LEFT);

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
				m_Grid.AddCell(row, col+1, "수정 SPT 타격횟수 (Ncorr)", GVHT_LEFT); 
				m_Grid.AddCell(row, col+2, &pPile->m_dCountHitSPT_Ncorr, DT_RIGHT, nLine, _T(""), CELL_TYPE_AUTO);
				m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pPile->m_dCountHitSPT_Ncorr), nFormat); 
				m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);
			}
			
			if(bSteel)
			{
				sArr.RemoveAll(); 
				sArr.Add(_T("양호")); 
				sArr.Add(_T("불량"));
				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, _T("강관말뚝의 관입상태"), GVHT_LEFT); 
				m_Grid.AddCell(row, col+2, &pPile->m_nTypeIntrusionToSupport, DT_RIGHT, TRUE, nLine, _T("Type : 종별"), CELL_TYPE_COMBO, sArr);
				m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);
			}
		}

		if(m_bDesignLsd && (pPile->m_nType == PILE_TYPE_STEEL_DIGGING || pPile->m_nType == PILE_TYPE_PHC_DIGGING) && bPileASD == FALSE)
		{
			m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
			m_Grid.AddCell(row, col+1, _T("타입말뚝에 대한 매입말뚝 선단지지력 효율"), GVHT_LEFT); 
			m_Grid.AddCell(row, col+2, &pPile->m_dEfficiencyQtVsTypePile, DT_RIGHT, nLine, _T(""), CELL_TYPE_AUTO);
			m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pPile->m_dEfficiencyQtVsTypePile), nFormat); 
			m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);

			m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
			m_Grid.AddCell(row, col+1, _T("타입말뚝에 대한 매입말뚝 주면마찰력 효율"), GVHT_LEFT); 
			m_Grid.AddCell(row, col+2, &pPile->m_dEfficiencyFrictionVsTypePile, DT_RIGHT, nLine, _T(""), CELL_TYPE_AUTO);
			m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pPile->m_dEfficiencyFrictionVsTypePile), nFormat); 
			m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);
		}
		// AAB-6153
		if(bHyunTa && m_bDesignLsd)
		{
			UINT nLineAuto = pBri->m_bPileNvalueAuto ? CELL_READONLY : CELL_LINE_NO;

			sArr.RemoveAll();
			sArr.Add(_T("사질토"));
			sArr.Add(_T("점성토")); 
			sArr.Add(_T("암반"));
			m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
			m_Grid.AddCell(row, col+1, _T("말뚝선단의 기초지반"), GVHT_LEFT); 
			m_Grid.AddCell(row, col+2, &pPile->m_nTypeGroundEdge, DT_RIGHT, TRUE, nLineAuto, _T(""), CELL_TYPE_COMBO, sArr);
			m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);

			if(pPile->m_nTypeGroundEdge == 0) // 사질토
			{
				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, _T("말뚝선단의 설계구역 지층의 평균 N값(N60)"), GVHT_LEFT); 
				m_Grid.AddCell(row, col+2, &pPile->m_dN60_Edge, DT_RIGHT, nLineAuto, _T(""), CELL_TYPE_AUTO);
				m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pPile->m_dN60_Edge), nFormat); 
				m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);

				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, _T("말뚝선단층 중간에서의 연직유효(σv')"), GVHT_LEFT); 
				m_Grid.AddCell(row, col+2, &pPile->m_dSigma_V_Edge, DT_RIGHT, nLineAuto, _T(""), CELL_TYPE_AUTO);
				m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pPile->m_dSigma_V_Edge), nFormat); 
				m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);
			}
			else if(pPile->m_nTypeGroundEdge == 1) // 점성토
			{
				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, _T("말뚝선단의 비배수 전단강도(Cu)"), GVHT_LEFT); 
				m_Grid.AddCell(row, col+2, &pPile->m_dCu_Edge, DT_RIGHT, nLineAuto, _T(""), CELL_TYPE_AUTO);
				m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pPile->m_dCu_Edge), nFormat); 
				m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);
			}
			else // 암반
			{
				szUnit_CGS   = "kgf/cm²";
				szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);

				sArr.RemoveAll();
				sArr.Add(_T("2.5qu"));
				sArr.Add(_T("[√s + √(m x √s + s)]qu")); 
				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, _T("암반의 선단지지력(qp)"), GVHT_LEFT); 
				m_Grid.AddCell(row, col+2, &pPile->m_nTypeSupport_Edge, DT_RIGHT, TRUE, nLine, _T(""), CELL_TYPE_COMBO, sArr);
				m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
				m_Grid.AddCell(row++, col+4, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
				//
				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, _T("말뚝선단의 암의 일축압축강도(qu)"), GVHT_LEFT); 
				m_Grid.AddCell(row, col+2, &pPile->m_dqu_Edge, DT_RIGHT, nLineAuto, _T(""), CELL_TYPE_AUTO); m_Grid.SetCellDec(CELL_DEC_3);
				m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
				m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pPile->m_dqu_Edge), nFormat); 
				m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply); 
				m_Grid.AddCell(row++, col+4, szUnit_Apply, GVHT_LEFT, CELL_READONLY);
				//
				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, _T("파쇄암반 정수(s)"), GVHT_LEFT); 
				m_Grid.AddCell(row, col+2, &pPile->m_ds_Edge, DT_RIGHT, pPile->m_nTypeSupport_Edge==0 ? CELL_READONLY : nLine, _T(""), CELL_TYPE_AUTO); m_Grid.SetCellDec(CELL_DEC_7);
				m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pPile->m_ds_Edge), nFormat); 
				m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);
				//
				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, _T("파쇄암반 정수(m)"), GVHT_LEFT); 
				m_Grid.AddCell(row, col+2, &pPile->m_dm_Edge, DT_RIGHT, pPile->m_nTypeSupport_Edge==0 ? CELL_READONLY : nLine, _T(""), CELL_TYPE_AUTO); m_Grid.SetCellDec(CELL_DEC_4);
				m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pPile->m_dm_Edge), nFormat); 
				m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);
			}
		}
	}
	else
	{
		// (ARCBRIDGE-2777) 기초 근입깊이 Df 입력 추가. - 강도설계법일때는 설계환경에서 교량별로 받고 있으니 한계상태일때만 기초별로 받자.
		m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "기초의 유효 근입깊이", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pFooting->m_dDf, DT_RIGHT);m_Grid.SetCellDec(CELL_DEC_3);
		m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dDf), DT_RIGHT);m_Grid.SetCellDec(CELL_DEC_3);
		m_Grid.AddCell(row++, col+4, "m", GVHT_LEFT,CELL_READONLY);

		sArr.RemoveAll();
		sArr.Add("√12N + 15");
		sArr.Add("√12N + 20");
		sArr.Add("√12N + 25");
		sArr.Add("√15N + 15");
		sArr.Add("√20N + 15");
		sArr.Add("0.3N + 27");
		sArr.Add("사용자입력"); // 6
		m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "기초지반의 내부마찰각", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pFooting->m_nTypeEquation_Friction, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
		m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);

		m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "기초지반의 내부마찰각 사용자입력", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pFooting->m_dFrictionAngle_User, DT_RIGHT, pFooting->m_nTypeEquation_Friction==6 ? CELL_LINE_NO : CELL_READONLY);
		m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dFrictionAngle_User), nFormat); 
		m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);

		if(m_bDesignLsd)
		{
			if(bRock == FALSE)
			{
				szUnit_CGS   = _T("tonf/m²");
				szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);

				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, "SPT 타격평균값(Ncorr)", GVHT_LEFT);
				m_Grid.AddCell(row, col+2, &pFooting->m_dCountHitSPT_Ncorr, DT_RIGHT);
				m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dCountHitSPT_Ncorr), nFormat); 
				m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);
				col = 0;			
			}
			else
			{
				szUnit_CGS   = "tonf/m²";
				szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);

				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, "암의 일축압축강도(qo)", GVHT_LEFT);

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
// 				sArr.Add(_T("사용자입력"));
// 				m_Grid.AddCell(row, col+2, &pFooting->m_nRockMassRating, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
// 				m_Grid.AddCell(row++, col+4, _T(""), nFormat, CELL_READONLY);
// 
// 				//상위 RMR이 사용자 입력인 경우 입력을 받도록 수정
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
			m_Grid.AddCell(row, col+1, "허용침하량 (Sa)", GVHT_LEFT);
			m_Grid.AddCell(row, col+2, &pFooting->m_dAllowableSubsidence, DT_RIGHT);
			m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dAllowableSubsidence), nFormat); 
			m_Grid.AddCell(row++, col+4, _T("mm"), GVHT_LEFT, CELL_READONLY);

			m_Grid.AddCell(row, col++, sHead, GVHT_LEFT);
			m_Grid.AddCell(row, col++, "강성에 따른 형상계수(βz)", GVHT_LEFT);
			m_Grid.AddCell(row, col++, &pFooting->m_dFormFactorByShape, DT_RIGHT); m_Grid.SetCellDec(CELL_DEC_2);
			m_Grid.AddCell(row, col++, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dFormFactorByShape), nFormat); m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply);
			m_xyButtonFactor.x	= col--;
			m_xyButtonFactor.y	= row;
			m_Grid.AddCell(row++, col, _T(""), nFormat, CELL_READONLY);
			col = 0;
		}
	}

	szUnit_CGS   = "tonf/m³";
	szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);
	m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "기초지반의 단위중량 (r1)", GVHT_LEFT);

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
		m_Grid.AddCell(row, col+1, "근입지반의 단위중량 (r2)", GVHT_LEFT);

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

	// (ARCBRIDGE-3488) 직접기초형식일때 변형게수(α) 옵션 추가
	sHead = _T("지반변형계수");
	if(!bPile)
	{
		m_Grid.AddCell(row, col, sHead);
		m_Grid.AddCell(row, col+1, "변형계수(α) 상시", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pFooting->m_dFactorAlpha[0], DT_RIGHT); m_Grid.SetCellDec(CELL_DEC_0);
		m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dFactorAlpha[0]), nFormat); 
		m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);

		m_Grid.AddCell(row, col, sHead);
		m_Grid.AddCell(row, col+1, "변형계수(α) 지진시", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pFooting->m_dFactorAlpha[1], DT_RIGHT); m_Grid.SetCellDec(CELL_DEC_0);
		m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dFactorAlpha[1]), nFormat); 
		m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);

		if(m_bDesignLsd)
		{
			if(bRock)
			{
				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, "암반의 불연속면의 빈도를 고려한 저항계수(αE)", GVHT_LEFT);
				sArr.RemoveAll();
				sArr.Add(_T("0.023(RQD) - 1.32"));
				sArr.Add(_T("사용자입력"));
				m_Grid.AddCell(row, col+2, &pFooting->m_nCoefficientResistance_Rock, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
				m_Grid.AddCell(row, col+3, _T(""), GVHT_LEFT, CELL_READONLY);
				m_Grid.AddCell(row++, col+4, _T(""), nFormat, CELL_READONLY);

				// 암반의 불연속면의 빈도를 고려한 저항계수(αE)이 사용자 입력인 경우 입력을 받도록 수정
				UINT nLineUser = pFooting->m_nCoefficientResistance_Rock == 1? CELL_LINE_NO : CELL_READONLY;
				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, "암반의 불연속면의 빈도를 고려한 저항계수(αE)", GVHT_LEFT);

				m_Grid.AddCell(row, col+2, &pFooting->m_dUserCoefficientResistance_Rock, DT_RIGHT, nLineUser);
				//m_Grid.SetCellUnit(szUnit_CGS, szUnit_Apply); 
				m_Grid.SetCellDec(CELL_DEC_3);

				m_Grid.AddCell(row, col+3, m_Grid.GetStrDouble(m_Grid.GetCellLast(), pFooting->m_dUserCoefficientResistance_Rock), nFormat, nLineUser); 
				m_Grid.SetCellDec(CELL_DEC_3);
				m_Grid.AddCell(row++, col+4, _T(""), GVHT_LEFT, CELL_READONLY);

				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, "암반의 변형계수 (Em)", GVHT_LEFT);
				sArr.RemoveAll();
				sArr.Add(_T("αE ㆍEo"));
				sArr.Add(_T("사용자입력"));
				m_Grid.AddCell(row, col+3, _T(""), GVHT_LEFT, CELL_READONLY);
				m_Grid.AddCell(row, col+2, &pFooting->m_nModulusDeformation_Rock, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
				m_Grid.AddCell(row++, col+4, _T(""), nFormat, CELL_READONLY);


				// 암반의 암반의 변형계수 (Em) 이 사용자 입력인 경우 입력을 받도록 수정
				nLineUser = pFooting->m_nModulusDeformation_Rock == 1? CELL_LINE_NO : CELL_READONLY;
				m_Grid.AddCell(row, col, sHead, GVHT_LEFT);
				m_Grid.AddCell(row, col+1, "암반의 변형계수 (Em)", GVHT_LEFT);

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
			sArr.Add(_T("입력값 x N"));
			sArr.Add(_T("입력값"));
			if(pBri->IsBoxType()==FALSE)
				sArr.Add(_T("지반정보 적용"));

			m_Grid.AddCell(row, col++, sHead, GVHT_LEFT);
			m_Grid.AddCell(row, col++, _T("수직 지반변형계수(Eo) 적용"), GVHT_LEFT);
			m_Grid.AddCell(row, col++, &pBri->m_nSelect_Eo_VertType[nIdxFooting], DT_RIGHT, TRUE, CELL_LINE_NO, _T(""), CELL_TYPE_COMBO, sArr);
			m_Grid.AddCell(row, col++, m_pIn->GetStringArray(sArr,pBri->m_nSelect_Eo_VertType[nIdxFooting]), nFormat, nLine);
			m_Grid.AddCell(row, col++, _T(""), GVHT_LEFT, CELL_READONLY);
			row++;
			col= 0;

			szUnit_CGS   = UNIT_CGS_KGF_CM2;
			szUnit_Apply = pStd->IsSI() ? "kN/m²" : UNIT_CGS_KGF_CM2;
			if(m_bDesignLsd)
				szUnit_Apply = pStd->IsSI() ? pStd->m_szTonf_M2 : pStd->m_szKgf_CM2;

			m_Grid.AddCell(row, col++, sHead, GVHT_LEFT);
			m_Grid.AddCell(row, col++, _T("수직 지반변형계수(Eo)"), GVHT_LEFT);
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

// nIdxFooting : 현재 편집중인 기초의 인덱스
// 현재 편집중인 중간벽체 포인터 리턴.
// 외측벽체이면 NULL
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

// 기초형상계수
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

	m_Grid.AddCell(row, col, "수평변위(X)"); m_Grid.AddCell(row+1, col++, &pFooting->m_dEQShapeFactHorX, nFormat, nLine, "", CELL_TYPE_AUTO, sArr, &m_dlgBaseShapeFact);		m_Grid.SetCellDec(CELL_DEC_3);
	m_Grid.AddCell(row, col, "수평변위(Y)"); m_Grid.AddCell(row+1, col++, &pFooting->m_dEQShapeFactHorY, nFormat, nLine, "", CELL_TYPE_AUTO, sArr, &m_dlgBaseShapeFact);		m_Grid.SetCellDec(CELL_DEC_3);
	m_Grid.AddCell(row, col, "수직변위(Z)"); m_Grid.AddCell(row+1, col++, &pFooting->m_dEQShapeFactVerZ, nFormat, nLine, "", CELL_TYPE_AUTO, sArr, &m_dlgBaseShapeFact);		m_Grid.SetCellDec(CELL_DEC_3);
	m_Grid.AddCell(row, col, "회전로킹(X)"); m_Grid.AddCell(row+1, col++, &pFooting->m_dEQShapeFactRotX, nFormat, nLine, "", CELL_TYPE_AUTO, sArr, &m_dlgBaseShapeFact);		m_Grid.SetCellDec(CELL_DEC_3);
	m_Grid.AddCell(row, col, "회전로킹(Y)"); m_Grid.AddCell(row+1, col++, &pFooting->m_dEQShapeFactRotY, nFormat, nLine, "", CELL_TYPE_AUTO, sArr, &m_dlgBaseShapeFact);		m_Grid.SetCellDec(CELL_DEC_3);
	m_Grid.AddCell(row, col, "비틀림(Z)");   m_Grid.AddCell(row+1, col++, &pFooting->m_dEQShapeFactTorZ, nFormat, nLine, "", CELL_TYPE_AUTO, sArr, &m_dlgBaseShapeFact);	m_Grid.SetCellDec(CELL_DEC_3);

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(80);
}

void CInputGenFooting::SetSizeTabAndGrid()
{
	// 컨트롤들 모두 생성후.. 기초가 말뚝이 아닐때는 기본 탭과 그리드를 pos탭의 오른쪽까지 늘려준다.
	// 아닌 경우는 기본탭과 그리드를 pos탭의 왼쪽까지 줄여준다.
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
		// 사이즈 조정해야 되는 크기
		double dDiff	= rectPos.right - rect.right;
		rect.right += (long)dDiff;

		// 탭
		m_Tab.SetWindowPos(&wndTop, rect.left, rect.bottom, rect.Width(), rect.Height(), SWP_NOMOVE);

		// 그리드
		m_Grid.GetWindowRect(rect);
		rect.right += (long)dDiff;
		m_Grid.SetWindowPos(&wndTop, rect.left, rect.bottom, rect.Width(), rect.Height(), SWP_NOMOVE);

		// 말뚝용 숨김
		m_GridPos.ShowWindow(FALSE);
		m_TabPos.ShowWindow(FALSE);
	}
	else
	{
		// 사이즈 조정해야 되는 크기
		double dDiff	= rectPos.left - rect.right;
		rect.right += (long)dDiff;

		// 탭
		m_Tab.SetWindowPos(&wndTop, rect.left, rect.bottom, rect.Width(), rect.Height(), SWP_NOMOVE);

		// 그리드
		m_Grid.GetWindowRect(rect);
		rect.right += (long)dDiff;
		m_Grid.SetWindowPos(&wndTop, rect.left, rect.bottom, rect.Width(), rect.Height(), SWP_NOMOVE);

		// 말뚝용 보임
		m_GridPos.ShowWindow(TRUE);
		m_TabPos.ShowWindow(TRUE);
	}
}


// 지점별 기초수 설정 동일 적용
void CInputGenFooting::OnButtonSame() 
{
	// TODO: Add your control notification handler code here
	if(AfxMessageBox("모든 지점의 기초부 설정을 동일하게 적용하시겠습니까?", MB_YESNO) != IDYES) return;

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

		// 1. 기초지반설정
		pFooting->m_dUnitWeigthBaseGround	= pFootingCur->m_dUnitWeigthBaseGround;
		pFooting->m_Qa_Calculate[0]			= pFootingCur->m_Qa_Calculate[0];
		pFooting->m_Qa_Calculate[1]			= pFootingCur->m_Qa_Calculate[1];
		pFooting->m_Qa_Calculate[0]			= pFootingCur->m_Qa_Calculate[0];
		pFooting->m_Qa_Calculate[1]			= pFootingCur->m_Qa_Calculate[1];
		pFooting->m_QaMax_BaseGround		= pFootingCur->m_QaMax_BaseGround;
		pFooting->m_dFactorPile_qd			= pFootingCur->m_dFactorPile_qd;
		pFooting->m_nTypeEquation_Qa		= pFootingCur->m_nTypeEquation_Qa;
		pFooting->m_nTypeEquation_Friction	= pFootingCur->m_nTypeEquation_Friction;

		// 2. 기초의 근입계수
		pFooting->m_dEQSettleFactVerZ		= pFootingCur->m_dEQSettleFactVerZ;
		pFooting->m_dEQSettleFactHorXY		= pFootingCur->m_dEQSettleFactHorXY;
		pFooting->m_dEQSettleFactTorZ		= pFootingCur->m_dEQSettleFactTorZ;
		pFooting->m_dEQSettleFactRotX		= pFootingCur->m_dEQSettleFactRotX;
		pFooting->m_dEQSettleFactRotY		= pFootingCur->m_dEQSettleFactRotY;

		// 3. 사각형 기초의 형상계수
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
			str = _T("간섭되고 있는 말뚝이 없습니다. ");
		else
			str.Format("[ %d ] 개의 말뚝이 간섭되고 있습니다. 위치를 조정해 주세요.", nCount);

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
	
		// 지반정보 그림
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

	if(nRow == m_xyButton.y) //기초지반의 공칭지지력 산정식 
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
	//강성에 따른 형상계수
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
		// 말뚝이 권고안 되지 않은 상태라면 권고안을 해 준다.
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