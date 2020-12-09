// InputGenPlane.cpp : implementation file
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
// CInputGenPlane dialog


CInputGenPlane::CInputGenPlane(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputGenPlane::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputGenPlane)
	m_nCurrentDim = 0;
	m_bFootingShape = FALSE;
	m_bCheckfootLenFloat = FALSE;

	m_nLineReadOnlyBaseL = 0;
	m_nLineReadOnlyAWL = 0;
	m_nLineReadOnlyBaseR = 0;
	m_nLineReadOnlyAWR = 0;

	m_pProgress	= new CDlgProgress;
	//}}AFX_DATA_INIT
}

CInputGenPlane::~CInputGenPlane()
{
	delete m_pProgress;
}

void CInputGenPlane::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputGenPlane)
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_cbBridgeNo);
	DDX_Control(pDX, IDC_BUTTON_FOOTING_OPTIMIZE, m_btnFootingOptimize);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	DDX_Radio(pDX, IDC_RADIO_CURRENT_DIM, m_nCurrentDim);
	DDX_Check(pDX, IDC_CHECK_FOOTING_SHAPE, m_bFootingShape);
	DDX_Check(pDX, IDC_CHECK_FOOTING_LEN_FLOAT, m_bCheckfootLenFloat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputGenPlane, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputGenPlane)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_BN_CLICKED(IDC_BUTTON_FOOTING_OPTIMIZE, OnButtonFootingOptimize)
	ON_BN_CLICKED(IDC_RADIO_CURRENT_DIM, OnRadioCurrentDim)
	ON_BN_CLICKED(IDC_RADIO_WHOLE_DIM, OnRadioWholeDim)
	ON_BN_CLICKED(IDC_CHECK_FOOTING_SHAPE, OnCheckFootingShape)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
	ON_BN_CLICKED(IDC_CHECK_FOOTING_LEN_FLOAT, OnCheckFootingLenFloat)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputGenPlane message handlers

BOOL CInputGenPlane::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		UpdateData();
		if(m_Grid.GetFocusCell().row == m_Grid.GetCols()-1)
			pMsg->wParam = VK_RIGHT;
		else
			pMsg->wParam = VK_TAB;
	}

	return CInputDialogBase::PreTranslateMessage(pMsg);
}

void CInputGenPlane::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	
	int nRow = pGridView->iRow;
	int nCol = pGridView->iColumn;

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar * pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	BOOL bOutWall = pBri->IsOutsideWall();
	int nTab = m_Tab.GetCurSel();
	long nJ	= bOutWall ? nTab : nTab+1;
	// Box Type일때는 내측지점 Tab이 없다.
	if(pBri->IsBoxType()) nJ = (nTab == 0)? 0 : pBri->m_nQtyJigan;
	BOOL bWing	= bOutWall && (nJ==0 || nJ==pBri->m_nQtyJigan);
	long nColStt	= bWing ? 2 : 0;

	CDPoint vAngPre(0, 0);
	long nColCheckStt = (pBri->IsBoxType())? 2 : 7;
	if(nCol==nColCheckStt || nCol==nColCheckStt+1)
	{
		BOOL bLeft = (nCol == nColCheckStt)? TRUE:FALSE;
		BOOL bStt = (nTab == 0)? TRUE:FALSE;
		
		CWingWall *pWing = pBri->GetWingWall(bStt, bLeft);
		vAngPre = pWing->m_vAng;

		CString str = _T("");
		double dAng =m_Grid.GetTextMatrixDouble(nRow, nCol);
		BOOL bSide = (pWing->m_nAttachPos==1)? TRUE:FALSE;
		BOOL bValidAng = IsWingWallAngleValid(bStt, bLeft, ToDPointFrDegree(dAng));
		
		if(!bValidAng)
		{
			if(bSide) AfxMessageBox("날개벽 설치 위치를\n [벽체 배면]으로 변경하세요");
			else AfxMessageBox("날개벽 설치 위치를\n [벽체 측면]으로 변경하세요");
			
			str.Format("%.3f", vAngPre.GetAngleDegree());
			m_Grid.SetTextMatrix(nRow, nCol, str);
			pWing->m_vAng = vAngPre;
		}
	}
	SetDataSave();

	CFootingApp* pFooting = pBri->GetFootingByJijumNumber(nJ);
	if(pFooting)
	{
		if(nCol==0+nColStt)
		{
			double dAngTanL = pBri->GetAngleAzimuthTanOnJijum(nJ, -1).GetAngleDegree();
			double dAngTanR = pBri->GetAngleAzimuthTanOnJijum(nJ, 1).GetAngleDegree();
			CString str = m_Grid.GetTextMatrix(1, nCol);
			if(str==_T("좌측만 적용"))
			{
				pFooting->m_vAngLeft = pBri->GetAngleRectFootingEnd(nJ, TRUE);
				pFooting->m_vAngRight = ToDPointFrRadian(ToRadian(dAngTanR));
			}
			else if(str==_T("우측만 적용"))
			{
				pFooting->m_vAngLeft = ToDPointFrRadian(ToRadian(dAngTanL));
				pFooting->m_vAngRight = pBri->GetAngleRectFootingEnd(nJ, FALSE);
			}
			else if(str==_T("양측 모두 적용"))
			{
				pFooting->m_vAngLeft = pBri->GetAngleRectFootingEnd(nJ, TRUE);
				pFooting->m_vAngRight = pBri->GetAngleRectFootingEnd(nJ, FALSE);
			}
			else
			{
				pFooting->m_vAngLeft = ToDPointFrRadian(ToRadian(dAngTanL));
				pFooting->m_vAngRight = ToDPointFrRadian(ToRadian(dAngTanR));
			}
			pFooting->m_exFooting.m_vAngLeft = pFooting->m_vAngLeft;
			pFooting->m_exFooting.m_vAngRight = pFooting->m_vAngRight;
			

			pFooting->m_dMarginLeft = pBri->GetLengthFootingMargin(nJ, TRUE);
			pFooting->m_dMarginRight = pBri->GetLengthFootingMargin(nJ, FALSE);
			pFooting->m_exFooting.m_dMarginLeft = pFooting->m_dMarginLeft;
			pFooting->m_exFooting.m_dMarginRight = pFooting->m_dMarginRight;
		}

		if(nCol>2+nColStt)
		{
			pFooting->m_dMarginLeft = pBri->GetLengthFootingMargin(nJ, TRUE);
			pFooting->m_dMarginRight = pBri->GetLengthFootingMargin(nJ, FALSE);
			pFooting->m_exFooting.m_dMarginLeft = pFooting->m_dMarginLeft;
			pFooting->m_exFooting.m_dMarginRight = pFooting->m_dMarginRight;
		}
	}

	if((nCol==0 || nCol==1) && bWing)
	{
		BOOL bFirst = !IsUsedFlag();
		BOOL bStt = nTab==0 ? TRUE : FALSE;
		BOOL bLeft = nCol==0 ? TRUE : FALSE;
		
		CWingWall *pWing = pBri->GetWingWall(bStt, bLeft);

		long nPos = pWing->m_nAttachPos;
		CDPoint vAngJ = pBri->GetAngleJijum(bStt? 0:pBri->m_nQtyJigan);
		
		CDPoint vAngWing = (nPos==0)? CDPoint(1, 0) : bLeft? vAngJ:ToDPointFrDegree(180 - vAngJ.GetAngleDegree());
		
		pWing->m_vAng = vAngWing;
		pBri->SetDataDefaultWingWall(bStt, bLeft, bFirst);
		int nCountFooting = pBri->GetCountInWall() + 2;
		int nF = bStt ? 0 : nCountFooting-1;
		pBri->SetFootingEndShape(nF);

		// 날개벽 철근 재배치
		long nDan = 0;
		pBri->SetDataDefaultRebarWingWallUpper(bStt, bLeft, 1, nDan);
		pBri->SetDataDefaultRebarWingWallUpper(bStt, bLeft, 0, nDan);
		pBri->SetDataDefaultRebarWingWallLower(bStt, bLeft, 1, nDan);
		pBri->SetDataDefaultRebarWingWallLower(bStt, bLeft, 0, nDan);
		pBri->SetDataDefaultRebarWingWallSide(bStt, bLeft, 1, nDan);
		pBri->SetDataDefaultRebarWingWallSide(bStt, bLeft, 0, nDan);
		
		pBri->SetDataDefaultRebarJewonWingWall(bStt, bLeft);
	}
	if(nCol==nColCheckStt || nCol==nColCheckStt+1)
	{
		if(pBri->m_bAutoWingEl)
			pBri->SetDataDefaultWingWallEl();

		if(m_bFootingShape)
			pBri->SetFootingEndShape(nJ);

		// 날개벽에 대한 종단면을 다시 구하도록
		pBri->m_bSettingVertSection = FALSE;
	}

	SetGridData();
	SetDataInit();

	//DrawInputDomyunView(FALSE);
	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);
}

void CInputGenPlane::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	
	int nRow = pGridView->iRow;
	int nCol = pGridView->iColumn;

	CString str = m_Grid.GetTextMatrix(0, nCol);
	if(str.Right(4)==_T("(˚)"))
		str = str.Left(3) + _T("(") + m_Grid.GetTextMatrix(nRow, nCol) + _T("%%d)");
	m_pView->GetDomyun()->STMakeCursor(str);
}

void CInputGenPlane::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SetShowWindow();
	SetGridData();
	SetDataInit();
	DrawInputDomyunView();
	
	return;
}

void CInputGenPlane::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SetDataSave();
	
	return;
}

void CInputGenPlane::OnPreInitDialog()
{

	SetResize(IDC_CHECK_FOOTING_SHAPE,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_VIEW,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RADIO_CURRENT_DIM,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RADIO_WHOLE_DIM,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_FOOTING_OPTIMIZE,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_GRID,	SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_TAB,	SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	SetHyperHelp(m_pIn->GetManualPath());
	m_bCheckfootLenFloat = m_pIn->m_pARcBridgeDataStd->m_bFootLenInputFloat;
	UpdateData(FALSE);
	m_Grid.InitGrid();

	InitComboCtrl();
	InitTabCtrl();
	SetGridData();
	SetDataInit();
	SetShowWindow();
}

void CInputGenPlane::InitComboCtrl()
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

void CInputGenPlane::InitTabCtrl()
{
	m_Tab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_Tab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	m_Tab.RemoveAllTabs();

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	int nCountInWall = pBri->GetCountInWall();
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX) nCountInWall = 0;
	BOOL bOutWall = pBri->IsOutsideWall();

	int nTabs = bOutWall ? nCountInWall+2 : nCountInWall;
	int nTab = 0;
	CString str = _T("");

	if(bOutWall)
	{
		m_Tab.InsertItem(nTab++, _T("시점측"));
		m_Tab.InsertItem(nTabs-1, _T("종점측"));
	}
	
	int i=0; for(i=0; i<nCountInWall; i++)
	{
		str.Format(_T("내측 %d"), i+1);
		m_Tab.InsertItem(nTab++, str);
	}
}

void CInputGenPlane::OnSelchangeComboBridgeNo() 
{
	m_pIn->m_nCurrentBridge = m_cbBridgeNo.GetCurSel();

	InitTabCtrl();
	SetGridData();
	SetDataInit();
	SetEnableWindow();

	SetPaneTitle();

	DrawInputDomyunView();
}

void CInputGenPlane::DrawInputDomyunView(BOOL bZoomAll)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	CDomyun *pDom = m_pView->GetDomyun();
	double Scale = MAX(100,toM(pBri->m_dLengthBridge)/30*100); // 30M교량을 기준으로 1:100, 최소 100;

	pDom->ClearEtt(TRUE, FALSE);
	pDom->SetScaleDim(Scale);

	CDomyun Dom(pDom);

	int nSeparBri = pBri->m_nSeparBri;
	for(int nSepar=-1; nSepar<2; nSepar++)
	{
		if(nSeparBri==0 && nSepar!=0) continue;
		if(nSeparBri==1 && nSepar==-1) continue;
		if(nSeparBri==2 && nSepar==1) continue;

		pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge+nSepar);
		CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);

		BOOL bOutWall = pBri->IsOutsideWall();
		int nTab = m_Tab.GetCurSel();
		long nJ	= bOutWall ? nTab : nTab+1;

		if(pBri->m_nTypeBridge!=BRIDGE_TYPE_SLAB)
		{
			//pBri->SyncBridge();

			// 교량 평면
			DBStd.m_pDrawPlane->DrawPlaneCenterLineEachSlab(&Dom);
			DBStd.m_pDrawPlane->DrawPlaneSlabOnlyOutLine(&Dom);
			
			// 벽체 및 기둥 평면
			int nCountInWall = pBri->GetCountInWall();
			int i=0; for(i=0; i<nCountInWall; i++)
			{
				CWallApp *pWall = pBri->GetInWall(i);
				if(pWall!=NULL)
				{
					pWall->GetTvPlan();
					DBStd.m_pDrawWall->DrawPlan(&Dom, pWall, FALSE);
					if(pWall->m_bTopIsColumn)
						DBStd.m_pDrawColumn->DrawPlan(&Dom, &pWall->m_Column);
				}
			}
			
			// 기초 평면
			m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
			{
				// BOX형은 기초없이 확대기초만 있으니깐 여기서 확대기초만 그려주자.
				DBStd.m_pDrawExFooting->DrawPlan(&Dom, &pBri->m_footingBox.m_exFooting);
			}
			else
			{
				for(long i=0; i<pBri->GetCountJijum(); i++)
				{
					CFootingApp* pFooting = pBri->GetFootingByJijumNumber(i);
					if(pFooting==NULL)
						continue;

					pFooting->GetTvPlan();
					DBStd.m_pDrawFooting->DrawPlan(&Dom, pFooting);
					pFooting->m_exFooting.GetTvPlan();
					DBStd.m_pDrawExFooting->DrawPlan(&Dom, &pFooting->m_exFooting);

					// 아치교는 기초위에 스프링깅부 표시
					if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH && (i == 0 || i == pBri->GetCountJijum()-1))
					{
						DBStd.m_pDrawPlane->DrawPlane_Section_Arch(&Dom, i==0);

					}

					if(nSepar==0)
					{
						if(m_nCurrentDim==0)
						{
							if(i==nJ)
							{
								DBStd.m_pDrawFooting->DimPlaneInput(&Dom, i);
								DBStd.m_pDrawFooting->DimPlaneAngle(&Dom, i);
							}
						}
						else
						{
							DBStd.m_pDrawFooting->DimPlaneInput(&Dom, i);
							DBStd.m_pDrawFooting->DimPlaneAngle(&Dom, i);
						}
					}
				}
			}

			// 날개벽 평면
			pBri->GetTvWingWallPlane(TRUE);
			DBStd.m_pDrawWingWall->DrawGenPlane(&Dom, TRUE);
			
			if(nSepar==0)
			{
				if(m_nCurrentDim==0)
				{
					if(nJ==0)
						DBStd.m_pDrawWingWall->DimGenAngle(&Dom, TRUE);
				}
				else
					DBStd.m_pDrawWingWall->DimGenAngle(&Dom, TRUE);
			}

			pBri->GetTvWingWallPlane(FALSE);
			DBStd.m_pDrawWingWall->DrawGenPlane(&Dom, FALSE);
			if(nSepar==0)
			{
				if(m_nCurrentDim==0)
				{
					if(nJ==pBri->m_nQtyJigan)
						DBStd.m_pDrawWingWall->DimGenAngle(&Dom, FALSE);
				}
				else
					DBStd.m_pDrawWingWall->DimGenAngle(&Dom, FALSE);
			}
#ifdef _DEBUG
/*			// 비계 평면
			CHgBaseDrawStd draw;
			m_pIn->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
			CTwinVectorArray tvArr, tvArr2, tvArr3, tvArr4;
			
			BOOL bStt	= FALSE;
			BOOL bLeft	= TRUE;
			pBri->GetTvScaffoldWingPlane(tvArr, tvArr2, tvArr3, tvArr4, bStt, bLeft);
			
			
			draw.DrawTvArray(&Dom, tvArr);
			draw.DrawTvArray(&Dom, tvArr2);

			pBri->GetTvScaffoldWingPlane(tvArr, tvArr2, tvArr3, tvArr4, bStt, !bLeft);
			
			
			draw.DrawTvArray(&Dom, tvArr);
			draw.DrawTvArray(&Dom, tvArr2);

			pBri->GetTvScaffoldPlane(tvArr, tvArr2, bStt);
			draw.DrawTvArray(&Dom, tvArr);
*/
#endif
		}
	}
	CEnvRegist env;		
	BOOL bRotate = env.LoadEnvInt(CEnvRegist::INPUTDOM_ROTATE);
	if(pBri->m_nTypeBridge!=BRIDGE_TYPE_SLAB && bRotate)
	{
		// 교량 진행 방향으로 회전
		pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
		CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);
		DBStd.RotateDomByBridgeSttEndXy(&Dom);
	}

	*pDom << Dom;

	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

BOOL CInputGenPlane::IsValid()
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

void CInputGenPlane::SetDataDefault()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	for(long bri = 0; bri < pStd->GetBridgeSize(); bri++)
	{
		CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(bri);
		if(!pBri) continue;

		BOOL bFirst   = IsFirst(bri);
		pBri->SetDefaultedState(GetIDD(), 0x00000001);
		if(!bFirst && !IsClickDefaultButton(bri)) continue;

		BOOL bOutWall = pBri->IsOutsideWall();
		int nTab = m_Tab.GetCurSel();
		long nJ	= bOutWall ? nTab : nTab+1;

		CWingWall* pWing = NULL;
		if(bFirst || (nJ==0 && IsClickDefaultButton(bri)))
		{
			CDPoint vAngJ = pBri->GetAngleJijum(0);
			pWing = pBri->GetWingWall(TRUE, TRUE);
			BOOL bExistOld	= pWing->m_bExist;
			pWing->m_bExist = pBri->m_nSeparBri!=1;
			if(!bExistOld && pWing->m_bExist)
				pWing->m_nAttachPos	= 0;

			if(pWing->m_bExist)
				pWing->m_vAng = pBri->GetDefaultAngleWingWall(TRUE, TRUE);

			pWing = pBri->GetWingWall(TRUE, FALSE);
			bExistOld	= pWing->m_bExist;
			pWing->m_bExist = pBri->m_nSeparBri!=2;
			if(!bExistOld && pWing->m_bExist)
				pWing->m_nAttachPos	= 0;

			if(pWing->m_bExist)
				pWing->m_vAng = pBri->GetDefaultAngleWingWall(TRUE, FALSE);
		}
		if(bFirst || (nJ==pBri->m_nQtyJigan && IsClickDefaultButton(bri)))
		{
			CDPoint vAngJ = pBri->GetAngleJijum(pBri->m_nQtyJigan);
			pWing = pBri->GetWingWall(FALSE, TRUE);
			BOOL bExistOld	= pWing->m_bExist;
			pWing->m_bExist = pBri->m_nSeparBri!=2;
			if(!bExistOld && pWing->m_bExist)
				pWing->m_nAttachPos	= 0;

			if(pWing->m_bExist)
				pWing->m_vAng = pBri->GetDefaultAngleWingWall(FALSE, TRUE);

			pWing = pBri->GetWingWall(FALSE, FALSE);
			bExistOld	= pWing->m_bExist;
			pWing->m_bExist = pBri->m_nSeparBri!=1;
			if(!bExistOld && pWing->m_bExist)
				pWing->m_nAttachPos	= 0;

			if(pWing->m_bExist)
				pWing->m_vAng = pBri->GetDefaultAngleWingWall(FALSE, FALSE);
		}

		if(bFirst || IsClickDefaultButton(bri))
		{
			int nSttFooting = bFirst ? 0 : nJ;
			int nEndFooting = bFirst ? pBri->GetCountJijum()-1 : nJ;

			int j=0; for(j=nSttFooting; j<=nEndFooting; j++)
			{
				CFootingApp* pFooting = pBri->GetFootingByJijumNumber(j);
				if(pFooting==NULL)
					continue;

				pFooting->m_bFootingEndToWing = FALSE;
				pFooting->m_exFooting.m_bFootingEndToWing = pFooting->m_bFootingEndToWing;

				pBri->SetFootingEndShape(j);
			}
		}


		// bFirst일때는 날개벽 권고안을 한번 해준다.
		if(bFirst)
		{
			// 날개벽 권고안 표시해줌
			pBri->SetDefaultedState(IDD_GEN_WINGWALL, 0x00000001);
			BOOL bStt = TRUE;
			BOOL bEnd = TRUE;
			BOOL bLeft = TRUE;
			BOOL bRight = TRUE;

			CWingWall* pWing = NULL;
			if(bStt)
			{
				if(bLeft)
				{
					pWing = pBri->GetWingWall(TRUE, TRUE);
					pWing->m_bExist = pBri->m_nSeparBri!=1;
					pBri->SetDataDefaultWingWall(TRUE, TRUE, bFirst);
				}
				if(bRight)
				{
					pWing = pBri->GetWingWall(TRUE, FALSE);
					pWing->m_bExist = pBri->m_nSeparBri!=2;
					pBri->SetDataDefaultWingWall(TRUE, FALSE, bFirst);
				}
			}
			if(bEnd)
			{
				if(bLeft)
				{
					pWing = pBri->GetWingWall(FALSE, TRUE);
					pWing->m_bExist = pBri->m_nSeparBri!=2;
					pBri->SetDataDefaultWingWall(FALSE, TRUE, bFirst);
				}
				if(bRight)
				{
					pWing = pBri->GetWingWall(FALSE, FALSE);
					pWing->m_bExist = pBri->m_nSeparBri!=1;
					pBri->SetDataDefaultWingWall(FALSE, FALSE, bFirst);
				}
			}
		}

		if(bFirst || IsClickDefaultButton(bri))
		{
			for(long stt = 0; stt < 2; stt++)
			{
				BOOL bStt	= stt == iSTT;
				if(!pBri->IsOutWall(bStt))
				{
					pWing	= pBri->GetWingWall(bStt, TRUE);
					pWing->m_bExist	= FALSE;
					
					pWing	= pBri->GetWingWall(bStt, FALSE);
					pWing->m_bExist	= FALSE;
				}
			}
		}
	}

	SetGridData();
	SetDataInit();
}

void CInputGenPlane::SetGridData()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	if (pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB)
		return;
	
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 1, 0);

	long row = 0;
	long col = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;
	
	BOOL bOutWall = pBri->IsOutsideWall();
	int nTab = m_Tab.GetCurSel();
	long nJ	= bOutWall ? nTab : nTab+1;
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX && nJ > 0)
	{
		nJ = pBri->m_nQtyJigan;
	}
	CFootingApp *pFooting = pBri->GetFootingByJijumNumber(nJ);
	
	sArr1, sArr2;
	sArr1.RemoveAll();
	sArr1.Add(_T("벽체배면 설치"));
	sArr1.Add(_T("벽체측면 설치"));
	sArr1.Add(_T("설치안함"));
	sArr2.RemoveAll();
	sArr2.Add(_T("좌측만 적용"));
	sArr2.Add(_T("우측만 적용"));
	sArr2.Add(_T("양측 모두 적용"));
	sArr2.Add(_T("적용 안함"));
	
	
	//기초단부 직각처리
	strCombo = _T("");
	m_nLineReadOnlyBaseL = CELL_LINE_NO;
	m_nLineReadOnlyBaseR = CELL_LINE_NO;
	if(pFooting)
	{
 		BOOL bRAngLeft = pFooting->m_bRAngLeft;
 		BOOL bRAngRight = pFooting->m_bRAngRight;
 		if(bRAngLeft && bRAngRight)
 		{
 			strCombo = _T("양측 모두 적용");
 			m_nLineReadOnlyBaseL = CELL_READONLY;
 			m_nLineReadOnlyBaseR = CELL_READONLY;
 		}
 		else if(bRAngLeft && !bRAngRight)
 		{
 			strCombo = _T("좌측만 적용");
 			m_nLineReadOnlyBaseL = CELL_READONLY;
 		}
 		else if(!bRAngLeft && bRAngRight)
 		{
 			strCombo = _T("우측만 적용");
 			m_nLineReadOnlyBaseR = CELL_READONLY;
 		}
 		else
 		{
 			strCombo = _T("적용 안함");
 		}
	}
	
	//AFL, ALR 계산
	strAFL = _T(""), strAFR = _T("");
	if(pFooting && pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
	{
		CDPoint vAngTan = pBri->GetAngleAzimuthTanOnJijum(nJ, -1);
		double dAngL = pFooting->m_vAngLeft.GetAngleDegree() - vAngTan.GetAngleDegree();
		if(dAngL > 180.0)
			dAngL -= 360.0;
		strAFL.Format(_T("%.3f"), Round(dAngL, 3));
	
		vAngTan = pBri->GetAngleAzimuthTanOnJijum(nJ, 1);
		double dAngR = pFooting->m_vAngRight.GetAngleDegree() - vAngTan.GetAngleDegree();
		if(dAngR > 180.0)
			dAngR -= 360.0;
		strAFR.Format(_T("%.3f"), Round(dAngR, 3));
	}
	
	//Cell 추가--
	
	strWL = _T(""), strWR = _T("");
	//좌측날개벽, 우측날개벽
	if(nJ == 0 || nJ == pBri->m_nQtyJigan)
	{
		BOOL bStt = nJ == 0;
	
		CWingWall *pWingL	= pBri->GetWingWall(bStt, TRUE);
		strWL = (pWingL->m_bExist)? ((pWingL->m_nAttachPos==0)? _T("벽체배면 설치"):_T("벽체측면 설치")) : _T("설치안함");
		CWingWall *pWingR	= pBri->GetWingWall(bStt, FALSE);
		strWR = (pWingR->m_bExist)? ((pWingR->m_nAttachPos==0)? _T("벽체배면 설치"):_T("벽체측면 설치")) : _T("설치안함");
	
		m_Grid.AddCell(row, col, _T("좌측날개벽")); m_Grid.AddCell(row+1, col++, &strWL, nFormat, nLine, "", CELL_TYPE_COMBO, sArr1);
		m_Grid.AddCell(row, col, _T("우측날개벽")); m_Grid.AddCell(row+1, col++, &strWR, nFormat, nLine, "", CELL_TYPE_COMBO, sArr1);
	}
	
	dMarginL = _T(""), dMarginR = _T("");
	if(pFooting && pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
	{
		m_Grid.AddCell(row, col, _T("기초 단부 직각 처리")); m_Grid.AddCell(row+1, col++,  &strCombo, nFormat, nLine, "", CELL_TYPE_COMBO, sArr2);
	
		m_bCheckfootLenFloat ? dMarginL.Format(_T("%.5f"), pFooting->m_dMarginLeft) : dMarginL.Format(_T("%.f"), pFooting->m_dMarginLeft);
		m_bCheckfootLenFloat ? dMarginR.Format(_T("%.5f"), pFooting->m_dMarginRight): dMarginR.Format(_T("%.f"), pFooting->m_dMarginRight);
	
		m_Grid.AddCell(row, col, _T("DFL")); m_Grid.AddCell(row+1, col++,  &dMarginL);
		m_Grid.AddCell(row, col, _T("DFR")); m_Grid.AddCell(row+1, col++,  &dMarginR);
		m_Grid.AddCell(row, col, _T("AFL(˚)")); m_Grid.AddCell(row+1, col++,  &strAFL, nFormat, m_nLineReadOnlyBaseL);
		m_Grid.AddCell(row, col, _T("AFR(˚)")); m_Grid.AddCell(row+1, col++,  &strAFR, nFormat, m_nLineReadOnlyBaseR);
	}
	
	//AWL, AWR
	strAWL = _T(""), strAWR = _T("");
	if(bOutWall && (nJ == 0 || nJ == pBri->m_nQtyJigan))
	{
		m_nLineReadOnlyAWL = CELL_LINE_NO, m_nLineReadOnlyAWR = CELL_LINE_NO;
		BOOL bStt = nJ == 0 ? TRUE : FALSE;
	
		CWingWall* pWingL = pBri->GetWingWall(bStt, TRUE);
		if(!pWingL->m_bExist)
			m_nLineReadOnlyAWL = CELL_READONLY;
	
		double dWAngL = pWingL->m_vAng.GetAngleDegree();
		if(dWAngL > 180.0)
			dWAngL -= 360.0;
		strAWL.Format(_T("%.3f"), dWAngL);
	
		CWingWall* pWingR = pBri->GetWingWall(bStt, FALSE);
		if(!pWingR->m_bExist)
			m_nLineReadOnlyAWR = CELL_READONLY;
	
		double dWAngR = pWingR->m_vAng.GetAngleDegree();
		if(dWAngR > 180.0)
			dWAngR -= 360.0;
		strAWR.Format(_T("%.3f"), dWAngR);
	
		m_Grid.AddCell(row, col, _T("AWL(˚)")); m_Grid.AddCell(row+1, col++,  &strAWL, nFormat, m_nLineReadOnlyAWL);
		m_Grid.AddCell(row, col, _T("AWR(˚)")); m_Grid.AddCell(row+1, col++,  &strAWR, nFormat, m_nLineReadOnlyAWR);
	}
	

	m_Grid.GetCellCount(row, col);
	if(row > 0 && col > 0)  //아치교인경우 내측 그리드가 하나도 없음.
	{
		m_Grid.SetRows(row);
		m_Grid.SetCols(col);
		m_Grid.SetColumnWidthAll(60);

		if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
		{
			m_Grid.SetColumnWidth(0,100);
			m_Grid.SetColumnWidth(1,100);
		}
		else if((nJ == 0 || nJ == pBri->m_nQtyJigan) && col > 2)
		{
			m_Grid.SetColumnWidth(0,100);
			m_Grid.SetColumnWidth(1,100);
			m_Grid.SetColumnWidth(2,140);
		}
		else
		{
			m_Grid.SetColumnWidth(0,140);
		}
	}
}

void CInputGenPlane::SetDataInit()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	if (pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB)
	{
		m_Tab.DeleteAllItems();
		m_Grid.InitGrid();
	}

	m_Grid.UpdateData(FALSE);
}

void CInputGenPlane::SetDataSave()
{
	m_Grid.UpdateData(TRUE);

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	BOOL bOutWall = pBri->IsOutsideWall();
	int nTab = m_Tab.GetCurSel();
	long nJ	= bOutWall ? nTab : nTab + 1;
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX && nJ > 0)
	{
		nJ = pBri->m_nQtyJigan;
	}
	long nColStt	= bOutWall && (nJ == 0 || nJ == pBri->m_nQtyJigan) ? 2 : 0;

	CString str = _T("");

	if(nJ==0 || nJ==pBri->m_nQtyJigan)
	{
		BOOL bStt = nTab==0 ? TRUE : FALSE;
		CWingWall* pWingL = pBri->GetWingWall(bStt, TRUE);
		CWingWall* pWingR = pBri->GetWingWall(bStt, FALSE);

		long nColBox = 0;
		//박스일경우는 2,3이 AWL, AWR이다. ...5,6기준에서 빼니까...음수나올일은 없을듯.
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
		{
			nColStt = 0;
			nColBox = -3;
		}

		pWingL->m_vAng = ToDPointFrRadian(ToRadian(m_Grid.GetTextMatrixDouble(1, 5+nColStt+nColBox)));
		pWingR->m_vAng = ToDPointFrRadian(ToRadian(m_Grid.GetTextMatrixDouble(1, 6+nColStt+nColBox)));
	}

	// 날개벽 설치 유무
	if(nJ == 0 || nJ == pBri->m_nQtyJigan)
	{
		BOOL bStt	= nJ == 0;
		CWingWall* pWingL = pBri->GetWingWall(bStt, TRUE);
		CWingWall* pWingR = pBri->GetWingWall(bStt, FALSE);

		str = m_Grid.GetTextMatrix(1, 0);
		pWingL->m_bExist	= !(str == "설치안함");
		pWingL->m_nAttachPos = (str == "벽체배면 설치")? 0 : 1;
		str = m_Grid.GetTextMatrix(1, 1);
		pWingR->m_bExist	= !(str == "설치안함");
		pWingR->m_nAttachPos = (str == "벽체배면 설치")? 0 : 1;
	}

	CFootingApp *pFooting = pBri->GetFootingByJijumNumber(nJ);
	if(pFooting && pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
	{
		str = m_Grid.GetTextMatrix(1, 0+nColStt);
		if(str==_T("양측 모두 적용"))
		{
			pFooting->m_bRAngLeft = TRUE;
			pFooting->m_bRAngRight = TRUE;
		}
		else if(str==_T("좌측만 적용"))
		{
			pFooting->m_bRAngLeft = TRUE;
			pFooting->m_bRAngRight = FALSE;
		}
		else if(str==_T("우측만 적용"))
		{
			pFooting->m_bRAngLeft = FALSE;
			pFooting->m_bRAngRight = TRUE;
		}
		else
		{
			pFooting->m_bRAngLeft = FALSE;
			pFooting->m_bRAngRight = FALSE;
		}
		pFooting->m_exFooting.m_bRAngLeft = pFooting->m_bRAngLeft;
		pFooting->m_exFooting.m_bRAngRight = pFooting->m_bRAngRight;

		pFooting->m_dMarginLeft = m_Grid.GetTextMatrixDouble(1, 1+nColStt);
		pFooting->m_dMarginRight = m_Grid.GetTextMatrixDouble(1, 2+nColStt);
		pFooting->m_exFooting.m_dMarginLeft = pFooting->m_dMarginLeft;
		pFooting->m_exFooting.m_dMarginRight = pFooting->m_dMarginRight;

		double dAngTan = pBri->GetAngleAzimuthTanOnJijum(nJ, -1).GetAngleDegree();
		double dAng = m_Grid.GetTextMatrixDouble(1, 3+nColStt);
		pFooting->m_vAngLeft = ToDPointFrRadian(ToRadian(dAngTan+dAng));

		dAngTan = pBri->GetAngleAzimuthTanOnJijum(nJ, 1).GetAngleDegree();
		dAng = m_Grid.GetTextMatrixDouble(1, 4+nColStt);
		pFooting->m_vAngRight = ToDPointFrRadian(ToRadian(dAngTan+dAng));
	}
}

void CInputGenPlane::OnButtonFootingOptimize() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	BOOL bOutWall = pBri->IsOutsideWall();
	int nTab = m_Tab.GetCurSel();
	long nJ	= bOutWall ? nTab : nTab+1;


	CFootingApp* pFooting = pBri->GetFootingByJijumNumber(nJ);
	if(pFooting)
	{
		pFooting->m_dMarginLeft = pBri->GetLengthFootingMargin(nJ, TRUE);
		pFooting->m_dMarginRight = pBri->GetLengthFootingMargin(nJ, FALSE);
		pFooting->m_exFooting.m_dMarginLeft = pFooting->m_dMarginLeft;
		pFooting->m_exFooting.m_dMarginRight = pFooting->m_dMarginRight;
	}

	SetGridData();
	SetDataInit();	
	DrawInputDomyunView();
}

void CInputGenPlane::OnRadioCurrentDim() 
{
	UpdateData();

	DrawInputDomyunView();
}

void CInputGenPlane::OnRadioWholeDim() 
{
	UpdateData();

	DrawInputDomyunView();
}

void CInputGenPlane::OnCheckFootingShape() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	BOOL bOutWall = pBri->IsOutsideWall();
	int nTab = m_Tab.GetCurSel();
	long nJ	= bOutWall ? nTab : nTab+1;


	UpdateData();
	CFootingApp* pFooting = pBri->GetFootingByJijumNumber(nJ);
	if(!pFooting) return;
	pFooting->m_bFootingEndToWing = m_bFootingShape;
	pFooting->m_exFooting.m_bFootingEndToWing = pFooting->m_bFootingEndToWing;

	pBri->SetFootingEndShape(nJ);

	SetGridData();
	SetDataInit();
	SetDataSave();

	DrawInputDomyunView();
}

void CInputGenPlane::SetShowWindow()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	BOOL bOutWall = pBri->IsOutsideWall();
	int nTab = m_Tab.GetCurSel();
	long nJ	= bOutWall ? nTab : nTab+1;

	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		GetDlgItem(IDC_CHECK_FOOTING_LEN_FLOAT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_FOOTING_SHAPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_FOOTING_OPTIMIZE)->ShowWindow(SW_HIDE);
	}
	else
	{
		if(nJ==0 || nJ==pBri->m_nQtyJigan)
			GetDlgItem(IDC_CHECK_FOOTING_SHAPE)->ShowWindow(SW_SHOW);
		else
			GetDlgItem(IDC_CHECK_FOOTING_SHAPE)->ShowWindow(SW_HIDE);
	}
}

void CInputGenPlane::SetEnableWindow()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	if(pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB)
	{
		m_Tab.DeleteAllItems();
		m_Grid.DeleteAllItems();
		GetDlgItem(IDC_CHECK_FOOTING_SHAPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_VIEW)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CURRENT_DIM)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_WHOLE_DIM)->EnableWindow(FALSE);
		m_btnFootingOptimize.EnableWindow(FALSE);
	}
	else
	{
		InitTabCtrl();
		GetDlgItem(IDC_CHECK_FOOTING_SHAPE)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_VIEW)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CURRENT_DIM)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_WHOLE_DIM)->EnableWindow(TRUE);
		m_btnFootingOptimize.EnableWindow(TRUE);
	}
}

BOOL CInputGenPlane::IsWingWallAngleValid(BOOL bStt, BOOL bLeft, CDPoint vAng)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return FALSE;

	BOOL bValid = FALSE;
	CWingWall *pWing = pBri->GetWingWall(bStt, bLeft);
	if(pWing==NULL || pWing->m_bExist == FALSE) return FALSE;

	CDPoint vAngPre = pWing->m_vAng;
	CDPoint vAngBet(0, 0);

	pWing->m_vAng = vAng;
	pBri->GetTvWingWallPlane(bStt, TRUE, TRUE, TRUE);

	BOOL bSide = (pWing->m_nAttachPos==1)? TRUE:FALSE;
	vAngBet = pBri->GetAngleWingWallBetweenWall(bStt, bLeft);


	if(bSide)
	{
		if(vAngBet.GetAngleDegree() <= 140)
			bValid = FALSE;
		else bValid = TRUE;
	}
	else if(!bSide && vAngBet.GetAngleDegree() >= 180)
	{
		bValid = FALSE;
	}
	else
	{
		CString strSide = (bLeft)? "좌측":"우측";
		CTwinVectorArray tvArr;
		CTwinVector tvSlab, tvOut;

		CDPoint xyRet(0, 0);
		double dT = pWing->m_dT1;
		double dW = (bStt)? pBri->m_dWS+pBri->m_dBWS:pBri->m_dWE+pBri->m_dBWE;

		tvArr = pBri->m_tvArrPlaneWingWall;
		tvSlab = tvArr.GetTvByInfo("슬래브끝단외측");
		tvOut = tvArr.GetTvByInfo(strSide+"날개벽외측");

		CDPoint vSlab = (tvSlab.m_v2-tvSlab.m_v1).Unit();	// 슬래브 끝단 각도

		CRebarPlacing rb;

		rb.Offset(tvSlab.m_v1, tvSlab.m_v2, -dW);
		rb.Offset(tvOut.m_v1, tvOut.m_v2, dT * (bLeft? 1 : -1));

		if(GetXyMatchSegAndLine(tvOut.m_v1, tvOut.m_v2, tvSlab.m_v1, vSlab, xyRet))
			bValid = TRUE;
		else bValid = FALSE;
	}

	pWing->m_vAng = vAngPre;
	pBri->GetTvWingWallPlane(bStt, TRUE, TRUE, TRUE);

	return bValid;
}

BOOL CInputGenPlane::IsOKMoveOtherDlg()
{
	if(!m_pIn->CheckStation()) return FALSE;

	return TRUE;
}


void CInputGenPlane::OnCheckFootingLenFloat() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	
//	BOOL bOutWall = pBri->IsOutsideWall();
// 	int nTab = m_Tab.GetCurSel();

	m_pIn->m_pARcBridgeDataStd->m_bFootLenInputFloat = m_bCheckfootLenFloat;
	SetGridData();
	SetDataInit();
}

void CInputGenPlane::KillFocusDlg()
{
	// 종단제원에서 벗어날때면 좌우중심,직방향사방향에 대한 종단 좌표를 모두 구해 놓는다.
	// 다른 곳에서 종단 제원을 여러가지 방법으로 많이 이용하기 때문에 속도 개선을 위해서 구해놓는것임
	if(!m_pProgress->GetSafeHwnd())
	{
		m_pProgress->Create(IDD_DIALOG_HGPROGRESS, this);
	}
	m_pProgress->ShowWindow(SW_SHOW);
	m_pProgress->CenterWindow();
	m_pProgress->InitProgressData();

	for(long bri = 0; bri < m_pIn->m_pARcBridgeDataStd->GetBridgeSize(); bri++)
	{
		CRcBridgeRebar *pBri	= m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(bri);
		if(!pBri) continue;

		m_pProgress->SetProgressPos("교량 종단제원 동기화 중......", 60);
		pBri->SettingVertSection();
	}

	m_pProgress->SetProgressPos("동기화 완료", 100);
	m_pProgress->DestroyWindow();
}