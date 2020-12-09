// InputBaseShoePosition.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "../ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "FixShoeSelectInputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputBaseShoePosition dialog


CInputBaseShoePosition::CInputBaseShoePosition(CWnd* pParent /*=NULL*/)
	: CInputDialogBase(CInputBaseShoePosition::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputBaseShoePosition)
	m_nCrossSect = 0;
	m_nTangent   = 0;
	m_bViewAllDir = TRUE;
	m_bUsePadShoe = FALSE;
	//}}AFX_DATA_INIT
}


void CInputBaseShoePosition::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputBaseShoePosition)
	DDX_Control(pDX, IDC_TAB, m_ctlTab);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_cbBridgeNo);
	DDX_Control(pDX, IDC_COMBO_SHOE_EA, m_cbShoeEa);
	DDX_Control(pDX, IDC_BUTTON_SELECTSHOEFIX, m_btFixShoeSelect);
	DDX_Control(pDX, IDC_BUTTON_SAMEAPPLY, m_btSameApply);
	DDX_Radio(pDX, IDC_RADIO_CROSS, m_nCrossSect);
	DDX_Radio(pDX, IDC_RADIO_TANGENT, m_nTangent);
	DDX_Check(pDX, IDC_CHECK_VIEW_COUNTER_DIR, m_bViewAllDir);
	DDX_Check(pDX, IDC_CHECK_PAD_SHOE, m_bUsePadShoe);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputBaseShoePosition, CInputDialogBase)
	//{{AFX_MSG_MAP(CInputBaseShoePosition)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_BN_CLICKED(IDC_RADIO_PLANE, OnRadioPlane)
	ON_BN_CLICKED(IDC_RADIO_TANGENT, OnRadioTangent)
	ON_BN_CLICKED(IDC_RADIO_FIXEDSHOE, OnRadioFixedShoe)
	ON_BN_CLICKED(IDC_RADIO_RIGHTANG, OnRadioRightAng)
	ON_BN_CLICKED(IDC_RADIO_CROSS, OnRadioCrossSect)
	ON_BN_CLICKED(IDC_RADIO_CURRENT, OnRadioCurrent)
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED,  IDC_GRID, OnCellChanged)
	ON_BN_CLICKED(IDC_BUTTON_SELECTSHOEFIX, OnButtonSelectShoeFix)
	ON_BN_CLICKED(IDC_BUTTON_SAMEAPPLY, OnButtonSameApply)
	ON_BN_CLICKED(IDC_CHECK_VIEW_COUNTER_DIR, OnCheckViewCounterDir)
	ON_BN_CLICKED(IDC_CHECK_PAD_SHOE, OnCheckPadShoe)
	ON_CBN_SELCHANGE(IDC_COMBO_SHOE_EA, OnSelchangeComboShoeEa)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelchange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelchanging)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputBaseShoePostion message handlers

BOOL CInputBaseShoePosition::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message==WM_KEYDOWN && (pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE)) 
	{
		UpdateData();
		if(m_Grid.GetFocusCell().col == m_Grid.GetCols()-1)
			pMsg->wParam = VK_RIGHT;
		else
			pMsg->wParam = VK_TAB;
	}
	
	return CInputDialogBase::PreTranslateMessage(pMsg);
}

void CInputBaseShoePosition::OnPreInitDialog()
{
	SetEnableWindow();
	

	SetResize(IDC_GRID, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_TAB, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_VIEW, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RADIO_CROSS, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RADIO_PLANE, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_DIR, SZ_TOP_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_RADIO_TANGENT, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RADIO_RIGHTANG, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RADIO_FIXEDSHOE, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_RADIO_CURRENT, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_SELECTSHOEFIX, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_SAMEAPPLY, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_VIEW_COUNTER_DIR, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_PAD_SHOE, SZ_TOP_LEFT, SZ_TOP_LEFT);

	SetHyperHelp(m_pIn->GetManualPath());

	InitComboCtrl();
}

void CInputBaseShoePosition::InitComboCtrl()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	long nSize = pStd->GetBridgeSize();
	CString str = _T("");

	long i=0; for(i=0; i<nSize; i++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(i);
		str = pStd->GetStringBridgeOrder(i) + _T(" : ") + pBri->m_strBridgeName
			+ _T(" [") + pBri->GetStringBridgeType() + _T("]");
		m_cbBridgeNo.AddString(str);
	}

	m_cbBridgeNo.SetCurSel(m_pIn->m_nCurrentBridge);
}

void CInputBaseShoePosition::OnSelchangeComboBridgeNo() 
{
	m_pIn->m_nCurrentBridge = m_cbBridgeNo.GetCurSel();

	InitTabCtrl();

	SetEnableWindow();
	SetDataInit();
	SetPaneTitle();

	DrawInputDomyunView();
}

void CInputBaseShoePosition::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	SetDataSave();

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);

	if(pBri->m_bUsePadShoe)
	{
		long nTab = max(m_ctlTab.GetCurSel(),0);
		long nJijum  = GetNumberJijumByTabIndex(nTab);
		long nVerPos = GetNumberVerPosByTabIndex(nTab);
		if(nJijum != -1 && nVerPos != -1)
		{
			CJijum* pJ = pBri->GetJijum(nJijum);
			long nQtyShoe = pJ->m_nQtyShoe[nVerPos];

			double Kv = pJ->m_dKv[nVerPos][0];
			double Kh = pJ->m_dKh[nVerPos][0];
			for(long ix = 1; ix < nQtyShoe; ix++)
			{
				pJ->m_dKv[nVerPos][ix] = Kv;
				pJ->m_dKh[nVerPos][ix] = Kh;
			}

		}
	}

	SetDataInit();

	CInputDialogBase::OnCellChangedData(pNotifyStruct, result);

	//DrawInputDomyunView();
}

void CInputBaseShoePosition::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;
	
	int nRow = pGridView->iRow;
	int nCol = pGridView->iColumn;

	CString str = (m_Grid.GetTextMatrix(nRow, 0)).Left(3);
	if(str.Right(1)==_T(" "))
		str = str.Left(2);
	if(m_nCrossSect==1)
		str = (m_Grid.GetTextMatrix(0, nCol)).Left(2);
//	DrawInputDomyunView();
	m_pView->GetDomyun()->STMakeCursor(str);

	CString strComment = _T("");
	CString strCol  = m_Grid.GetTextMatrix(0,nCol);
	if(strCol=="거리") strComment.Format("%d번째 교량받침의 도로중심에 대한 거리 , 단위 : mm",nRow);
	if(strCol=="Kv")   strComment.Format("%d번째 교량받침의 압축 스프링 계수 , 단위 : %s/m  ← 탄성받침인 경우 입력",nRow,m_pIn->m_pARcBridgeDataStd->m_szTonf);
	if(strCol=="Kh")   strComment.Format("%d번째 교량받침의 전단 스프링 계수 , 단위 : %s/m  ← 탄성받침인 경우 입력",nRow,m_pIn->m_pARcBridgeDataStd->m_szTonf);

	m_pIn->SetComment(strComment);}

BOOL CInputBaseShoePosition::IsValid()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	long nCountBri = 0;
	long nSizeBri  = pStd->GetBridgeSize();
	for(long nBr=0; nBr<nSizeBri; nBr++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(nBr);

		if(pBri->IsOutsideWall())
			nCountBri++;
	}

	// 교량의 형식이 모두 문형 라멘교이거나 박스교일 경우 이 입력 상자를 표시하지 않음
	if(nSizeBri==nCountBri)
		return FALSE;

	return TRUE;
}

void CInputBaseShoePosition::DrawInputDomyunView(BOOL bZoomAll)
{
	CDomyun *pDom = m_pView->GetDomyun();

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	CJijum* pJ = NULL;

	pDom->ClearEtt(TRUE, FALSE);
	if(pBri->m_nTypeBridge==BRIDGE_TYPE_PORTAL || pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH)
	{
		CInputDialogBase::DrawInputDomyunView(bZoomAll);
		return;
	}
	int nSeparBri = pBri->m_nSeparBri;
	CDomyun Dom(pDom);
	if(m_nCrossSect==1)	//평면도
	{
		CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);
		double dScale = MAX(100, toM(pBri->m_dLengthBridge)/30*100); // 30M교량을 기준으로 1:100, 최소 100;
		pDom->SetScaleDim(dScale);
		DBStd.m_pDrawPlane->DrawPlaneGen(&Dom,5000,TRUE,TRUE,FALSE,FALSE,FALSE);

		if(nSeparBri != 0 && m_pIn->m_bViewAllDir == TRUE)
		{
			if(nSeparBri == 1)
				pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge+1);	//상행선 일 경우, 하행선(상행선+1)도 표현, 현재 교량과 하행선 표현
			else
				pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge-1);	//하행선 일 경우, 상행선(하행선-1)도 표현, 현재 교량과 상행선 표현

			CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);
			DBStd.m_pDrawPlane->DrawPlaneGen(&Dom,5000,TRUE,TRUE,FALSE,FALSE,FALSE);
		}
		
		CEnvRegist env;		
		BOOL bRotate = env.LoadEnvInt(CEnvRegist::INPUTDOM_ROTATE);
		if(bRotate)
		{
			DBStd.RotateDomByBridgeSttEndXy(&Dom);
		}
		*pDom << Dom;

		CInputDialogBase::DrawInputDomyunView(bZoomAll);
		return;
	}

	pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge);
	CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);
	pDom->SetScaleDim(m_pIn->GetScaleInputDialog());
//	double dEL = 0;
	double dSta = 0;
// 	double dDist = 0;
	int nCountShoe = 0;
	CString str = _T("");

	long nTab = max(m_ctlTab.GetCurSel(),0);
	long nJijum  = GetNumberJijumByTabIndex(nTab);
	long nVerPos = GetNumberVerPosByTabIndex(nTab);
	pJ = pBri->GetJijum(nJijum);

	dSta = pBri->GetStationShoeOnJijum(nJijum, 0, nVerPos);
	DBStd.m_pDrawCrossSection->DrawSimpleCross(&Dom,dSta, TRUE, TRUE, TRUE);
	nCountShoe = pJ->m_nQtyShoe[nVerPos];
	for(int nShoe=0; nShoe<nCountShoe; nShoe++)
	{
		double dDist = pJ->m_dDistShoe[nVerPos][nShoe];
		double  dEL = pBri->GetXySlabUpperActByDist(dSta, dDist).y - pBri->m_dTS;
		DBStd.m_pDrawCrossSection->DrawSimpleShoe(&Dom, dEL, dDist);
	}
	DBStd.m_pDrawCrossSection->DimCrossDistShoe(&Dom, pJ->m_strJijumShoe[nVerPos], TRUE);

	if(nSeparBri != 0 && m_pIn->m_bViewAllDir == TRUE)
	{
		if(nSeparBri == 1)
			pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge+1);	//상행선 일 경우, 하행선(상행선+1)도 표현, 현재 교량과 하행선 표현
		else
			pBri = pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge-1);	//하행선 일 경우, 상행선(하행선-1)도 표현, 현재 교량과 상행선 표현
		CARcBridgeDBStandard DBStd(pBri, m_pIn->m_pARoadOptionStd, pStd);

		pDom->SetScaleDim(m_pIn->GetScaleInputDialog());
		DBStd.m_pDrawCrossSection->DimCrossSectionFullLeftRight(&Dom, pBri, pStd->GetBridgeRebar(m_pIn->m_nCurrentBridge), -1);
// 		 dEL = 0;
		double dSta = 0;
//		double dDist = 0;

		pJ = pBri->GetJijum(nJijum);
		dSta = pBri->GetStationShoeOnJijum(nJijum, 0, nVerPos);
		DBStd.m_pDrawCrossSection->DrawSimpleCross(&Dom,dSta, FALSE, TRUE, TRUE);
		nCountShoe = pJ->m_nQtyShoe[nVerPos];
		for(int nShoe=0; nShoe<nCountShoe; nShoe++)
		{
			double dDist = pJ->m_dDistShoe[nVerPos][nShoe];
			double dEL = pBri->GetXySlabUpperActByDist(dSta, dDist).y - pBri->m_dTS;
			DBStd.m_pDrawCrossSection->DrawSimpleShoe(&Dom, dEL, dDist);
		}
		DBStd.m_pDrawCrossSection->DimCrossDistShoe(&Dom, pJ->m_strJijumShoe[nVerPos], FALSE);
	}
	*pDom << Dom;

	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

void CInputBaseShoePosition::SetDataDefault()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
		
	for(long bri = 0; bri < pStd->GetBridgeSize(); bri++)
	{
		if(!IsDefaulted(bri, 0x00000001) || IsClickDefaultButton(bri))
		{
			CRcBridgeRebar* pBri = pStd->GetBridgeRebar(bri);
			if(!pBri) continue;
			
			pBri->m_nTypeShoeAngle = 0;
			pBri->SetDataDefaultCountShoe();	// 교량 받침 개수 기본값
			pBri->SetDataDefaultDistShoe();		// 교량 받침 거리 기본값 설정해 주어야 함

			// 슬래브 주철근과 배력근 좌표 초기화
			pBri->m_bCaledRebarMain_Plane	= FALSE;
			pBri->m_bCaledRebarSupport_Plane	= FALSE;
		}
	}
}

void CInputBaseShoePosition::SetGridTitle()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	long nTab    = max(m_ctlTab.GetCurSel(),0);
	long nJijum  = GetNumberJijumByTabIndex(nTab);
	long nVerPos = GetNumberVerPosByTabIndex(nTab);

	if(nJijum==-1 || nVerPos==-1) return;

	CJijum* pJ = pBri->GetJijum(nJijum);
	long nQtyShoe = pJ->m_nQtyShoe[nVerPos];

	if(nQtyShoe==0) 
	{
		m_Grid.SetRowCount(1);
		m_Grid.SetColumnCount(1);
		m_Grid.SetFixedRowCount();
		m_Grid.SetFixedColumnCount();
		m_Grid.SetColumnWidthAll(500);
		m_Grid.SetRowHeightAll(100);
		m_Grid.SetTextMatrix(0,0,"입력할 교량받침이 없습니다.");
		return;
	}

	m_Grid.EnableInsertRow(FALSE);
	m_Grid.EnableInsertCol(FALSE);
	m_Grid.SetRowCount(pBri->m_bUsePadShoe ? 4 : 2);
	m_Grid.SetColumnCount(nQtyShoe + 1);
	m_Grid.SetFixedRowCount();
	m_Grid.SetFixedColumnCount();
	m_Grid.SetRowHeightAll(20);
	m_Grid.SetColumnWidthAll(70);
	m_Grid.SetTextBkColor(CLR_BACK);

	//열
	CString str = _T("");
	long nCol = 0;
	m_Grid.SetTextMatrix(0, nCol, _T("구 분"));
	m_Grid.SetTextMatrix(1, nCol, _T("거리"));
	if(pBri->m_bUsePadShoe)
	{
		m_Grid.SetTextMatrix(2, nCol, _T("Kv"));
		m_Grid.SetTextMatrix(3, nCol, _T("Kh"));
	}

	//행
	nCol = 0;
	long i=0; for(i=1; i<=nQtyShoe; i++)
	{
		str.Format("L%d", i);
		m_Grid.SetTextMatrix(0, nCol + i, str);
	}
	m_Grid.SetRedraw(TRUE,TRUE);
}

void CInputBaseShoePosition::SetDataInit()
{
	SetGridTitle();

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	CString str = _T("");
	long nTab    = max(m_ctlTab.GetCurSel(),0);
	long nJijum  = GetNumberJijumByTabIndex(nTab);
	long nVerPos = GetNumberVerPosByTabIndex(nTab);

	if(nJijum==-1 || nVerPos==-1) return;

	CJijum* pJ = pBri->GetJijum(nJijum);
	long nQtyShoe = pJ->m_nQtyShoe[nVerPos];

	if(nQtyShoe==0) return;

	long nCol = 0;
//	double Kv=0,Kh=0;
	for(long i=1; i<=nQtyShoe; i++)
	{
		double Kv = GetValueUnitChange(pJ->m_dKv[nVerPos][i-1],UNIT_CGS_TONF_M,pStd->m_szTonf_M);
		double Kh = GetValueUnitChange(pJ->m_dKh[nVerPos][i-1],UNIT_CGS_TONF_M,pStd->m_szTonf_M);

		str.Format("%.1f",pJ->m_dDistShoe[nVerPos][i-1]);
		m_Grid.SetTextMatrix(1, nCol + i, str);

		if(pBri->m_bUsePadShoe)
		{
			str.Format("%.1f",Kv);
			m_Grid.SetTextMatrix(2, nCol + i, str);
			str.Format("%.1f",Kh);
			m_Grid.SetTextMatrix(3, nCol + i, str);
		}
	}

	if(pBri->m_bUsePadShoe)
	{
		long nCol = 1;
		for(long ix = 1; ix < nQtyShoe; ix++)
		{
			for(long nRow = 2; nRow <= 3; nRow++)
			{
				m_Grid.SetItemState(nRow, nCol + ix, m_Grid.GetItemState(nRow, nCol + ix) | GVIS_READONLY);
				m_Grid.SetItemBkColour(nRow, nCol + ix, CLR_READONLY);
				m_Grid.SetItemFgColour(nRow, nCol + ix, RGB(128, 128, 128));
			}
		}
	}

	m_cbShoeEa.SetCurSel(nQtyShoe);

	m_nTangent = pBri->m_nTypeShoeAngle;
	m_bViewAllDir  = m_pIn->m_bViewAllDir;
	m_bUsePadShoe = pBri->m_bUsePadShoe;
	UpdateData(FALSE);
}

void CInputBaseShoePosition::SetDataSave()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	UpdateData(TRUE);

	pBri->m_nTypeShoeAngle = m_nTangent;
	m_pIn->m_bViewAllDir = m_bViewAllDir;

	long nTab    = max(m_ctlTab.GetCurSel(),0);
	long nJijum  = GetNumberJijumByTabIndex(nTab);
	long nVerPos = GetNumberVerPosByTabIndex(nTab);

	if(nJijum==-1 || nVerPos==-1) return;

	CJijum* pJ = pBri->GetJijum(nJijum);
	long nQtyShoe = pJ->m_nQtyShoe[nVerPos];

	if(nQtyShoe==0) return;

	long nCol = 0;
	nCol = 0;
	long i=0; for(i=1; i<=nQtyShoe; i++)
	{
		pJ->m_dDistShoe[nVerPos][i-1] = m_Grid.GetTextMatrixDouble(1, nCol + i);

		if(pBri->m_bUsePadShoe)
		{
			pJ->m_dKv[nVerPos][i-1] = GetValueUnitChange(m_Grid.GetTextMatrixDouble(2, nCol + i),pStd->m_szTonf_M,UNIT_CGS_TONF_M);
			pJ->m_dKh[nVerPos][i-1] = GetValueUnitChange(m_Grid.GetTextMatrixDouble(3, nCol + i),pStd->m_szTonf_M,UNIT_CGS_TONF_M);
		}
	}


	// 기존에 입려되어 있다가 형식이 바뀌면서 값이 초기화 되지 않은 경우가 생길 수 있음
	// 따라서 데이타 저장시 항상 체크 해줌
	BOOL bNoneShoe	= FALSE;
	for(long j = 0; j < pBri->GetCountJijum(); j++)
	{
		CJijum *pJ	= pBri->GetJijum(j);
		if(!pJ) continue;

		if(j == 0)
		{
			bNoneShoe	= pBri->IsOutWall(TRUE);
		}
		else if(j == pBri->GetCountJijum()-1)
		{
			bNoneShoe	= pBri->IsOutWall(FALSE);
		}
		else
		{
			bNoneShoe	= FALSE;
			CWallApp *pWall	= pBri->GetInWall(j-1);
			if(pWall)
			{
				if(pWall->m_bIs)
					bNoneShoe	= TRUE;
			}
		}

		if(bNoneShoe)
		{
			pJ->m_nQtyShoe[0]	= 0;
			pJ->m_nQtyShoe[1]	= 0;
		}
		if(j==0) pJ->m_nQtyShoe[0]	= 0; // 시점부 전열
		if(j==pBri->GetCountJijum()-1) pJ->m_nQtyShoe[1] = 0; // 종점부 후열
	}

}

void CInputBaseShoePosition::OnRadioCrossSect() 
{
	UpdateData();
	DrawInputDomyunView();
}

void CInputBaseShoePosition::OnRadioPlane() 
{
	UpdateData();
	DrawInputDomyunView();
}

void CInputBaseShoePosition::OnRadioTangent() 
{
	UpdateData();

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	pBri->m_nTypeShoeAngle = m_nTangent;
	DrawInputDomyunView(FALSE);	
}

void CInputBaseShoePosition::OnRadioRightAng() 
{
	UpdateData();

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	pBri->m_nTypeShoeAngle = m_nTangent;
	DrawInputDomyunView(FALSE);	
}

void CInputBaseShoePosition::OnRadioFixedShoe() 
{
	UpdateData();

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	
	long nSlab = -1;

	if(pBri->IsExistFixedJijum(nSlab))
	{
		pBri->m_nTypeShoeAngle = m_nTangent;
		DrawInputDomyunView(FALSE);	
	}
	else
	{
		if(AfxMessageBox("고정단 받침을 먼저 선택하십시요.     ", MB_OK|MB_ICONINFORMATION) == IDOK) 
		{
			m_nTangent = 0;
			pBri->m_nTypeShoeAngle = m_nTangent;
			UpdateData(FALSE);
		}
	}
}

void CInputBaseShoePosition::OnRadioCurrent() 
{
	UpdateData();

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	long nSlab = -1;
	if(pBri->IsExistFixedJijum(nSlab))
	{
		pBri->m_nTypeShoeAngle = m_nTangent;
		DrawInputDomyunView(FALSE);	
	}
	else
	{
		if(AfxMessageBox("고정단 받침을 먼저 선택하십시요.     ", MB_OK|MB_ICONINFORMATION) == IDOK) 
		{
			m_nTangent = 0;
			pBri->m_nTypeShoeAngle = m_nTangent;
			UpdateData(FALSE);
		}
	}
}

void CInputBaseShoePosition::SetEnableWindow()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	if(pBri->IsOutsideWall())
	{
		m_cbShoeEa.EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_VIEW)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CROSS)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PLANE)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_DIR)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_TANGENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_RIGHTANG)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_FIXEDSHOE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CURRENT)->EnableWindow(FALSE);
	}
	else
	{
		m_cbShoeEa.EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_VIEW)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CROSS)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_PLANE)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_DIR)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_TANGENT)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_RIGHTANG)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_FIXEDSHOE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CURRENT)->EnableWindow(TRUE);
	}
}

void CInputBaseShoePosition::OnButtonSelectShoeFix() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	
	CFixShoeSelectInputDlg Dlg(m_pIn, GetParent());
	if(Dlg.DoModal()==IDOK)
	{
		if((m_nTangent==2) || (m_nTangent==3))
		{
			long nSlab = -1;
			if(!pBri->IsExistFixedJijum(nSlab))
			{
				m_nTangent = 0;
				pBri->m_nTypeShoeAngle = m_nTangent;
				UpdateData(FALSE);
			}
		}
		DrawInputDomyunView();
	}	
}

void CInputBaseShoePosition::OnButtonSameApply() 
{
	if(AfxMessageBox("기존의 데이타는 모두 지워집니다.\n계속 하시겠읍니까?.",
		MB_YESNO|MB_ICONQUESTION) == IDNO) return;

	CRcBridgeApp* pBri = m_pIn->m_pARcBridgeDataStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	long nTab    = max(m_ctlTab.GetCurSel(),0);
	long nJijum  = GetNumberJijumByTabIndex(nTab);

	CJijum *pJ	= pBri->GetJijum(nJijum);
	for(long j = 0; j <= pBri->m_nQtyJigan; j++)
	{
		if(j == nJijum) continue;
		if(j > 0 && j < pBri->m_nQtyJigan) continue;

		CJijum *pJTar	= pBri->GetJijum(j);
		for(long verpos = 0; verpos < 2; verpos++)
		{
			pJTar->m_nQtyShoe[verpos]	= pJ->m_nQtyShoe[1 - verpos];
			long i = 0; for(i = 0; i < pJTar->m_nQtyShoe[verpos]; i++)
			{
				pJTar->m_dDistShoe[verpos][i]	= pJ->m_dDistShoe[1 - verpos][i];
				pJTar->m_dKh[verpos][i]			= pJ->m_dKh[1 - verpos][i];
				pJTar->m_dKv[verpos][i]			= pJ->m_dKv[1 - verpos][i];
			}
		}
	}
	SetDataInit();
	
	DrawInputDomyunView();

}
void CInputBaseShoePosition::OnCheckViewCounterDir() 
{
	UpdateData(TRUE);
	m_pIn->m_bViewAllDir = m_bViewAllDir;
	DrawInputDomyunView();

}

void CInputBaseShoePosition::OnCheckPadShoe() 
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	UpdateData(TRUE);
	pBri->m_bUsePadShoe = m_bUsePadShoe;

	SetGridTitle();
	SetDataInit();
}

void CInputBaseShoePosition::InitTabCtrl()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;

	m_ctlTab.RemoveAllTabs();
	m_ctlTab.SendInitialUpdate(TRUE);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_ctlTab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	long nTab = 0;
	long nCountJijum = pBri->GetCountJijum();
	CString str = _T("");
	for(long nJ=0; nJ<nCountJijum; nJ++)
	{
		CJijum *pJ = pBri->GetJijum(nJ);
		if(nJ==0) 
		{
			str = "시점";
			m_ctlTab.InsertItem(nTab++,str);
		}
		else if(nJ==nCountJijum-1) 
		{
			str = "종점";
			m_ctlTab.InsertItem(nTab++,str);
		}
		else
		{
			if(pBri->IsRcSlab() && pBri->m_bExpJoint[nJ])
			{
				str.Format("P%d(전열)",nJ);
				m_ctlTab.InsertItem(nTab++,str);
				str.Format("P%d(후열)",nJ);
				m_ctlTab.InsertItem(nTab++,str);
			}
			else if(pBri->IsRcSlab())
			{
				str.Format("P%d",nJ);
				m_ctlTab.InsertItem(nTab++,str);
			}
		}
	}
}

long CInputBaseShoePosition::GetNumberJijumByTabIndex(long nTab)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return -1;

	long nCheck = 0;

	BOOL bShoeFirst  = TRUE;
	BOOL bShoeSecond = TRUE;

	long nQtyJijum = pBri->GetCountJijum();
	long i=0; for(i=0; i<nQtyJijum; i++)
	{
		CJijum *pJ = pBri->GetJijum(i);

		bShoeFirst  = FALSE;
		bShoeSecond = FALSE;

		if(i==0) 
			bShoeSecond = TRUE;
		else if(i==nQtyJijum-1) 
			bShoeFirst = TRUE;
		else if(pBri->IsRcSlab())
		{
			bShoeFirst = TRUE;
			if(pBri->m_bExpJoint[i])
				bShoeSecond = TRUE;
		}

		if(bShoeFirst)
		{ 
			if(nCheck==nTab) 
				return i;
			else
				nCheck++;
		}
		if(bShoeSecond)
		{ 
			if(nCheck==nTab) 
				return i;
			else
				nCheck++;
		}
	}

	return -1;
}

long CInputBaseShoePosition::GetNumberVerPosByTabIndex(long nTab)
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return -1;

	long nCheck = 0;

	BOOL bShoeFirst  = TRUE;
	BOOL bShoeSecond = TRUE;

	long nQtyJijum = pBri->GetCountJijum();
	long i=0; for(i=0; i<nQtyJijum; i++)
	{
		CJijum *pJ = pBri->GetJijum(i);

		bShoeFirst  = FALSE;
		bShoeSecond = FALSE;

		if(i==0) 
			bShoeSecond = TRUE;
		else if(i==nQtyJijum-1) 
			bShoeFirst = TRUE;
		else if(pBri->IsRcSlab())
		{
			bShoeFirst = TRUE;
			if(pBri->m_bExpJoint[i])
				bShoeSecond = TRUE;
		}

		if(bShoeFirst)
		{ 
			if(nCheck==nTab) 
				return 0;
			else
				nCheck++;
		}
		if(bShoeSecond)
		{ 
			if(nCheck==nTab) 
				return 1;
			else
				nCheck++;
		}
	}

	return -1;
}

void CInputBaseShoePosition::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SetDataInit();

	DrawInputDomyunView();

//	m_pView->GetDomyun()->STMakeCursor(str);

	return;
}

void CInputBaseShoePosition::OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult) 
{
	SetDataSave();

	return;
}

void CInputBaseShoePosition::OnSelchangeComboShoeEa() 
{
	// TODO: Add your control notification handler code here
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pStd->GetBridge(m_pIn->m_nCurrentBridge);
	if(!pBri) return;
	
	UpdateData();

	long nTab    = m_ctlTab.GetCurSel();
	long nJijum  = GetNumberJijumByTabIndex(nTab);
	long nVerPos = GetNumberVerPosByTabIndex(nTab);

	if(nJijum==-1 || nVerPos==-1) return;

	CJijum* pJ = pBri->GetJijum(nJijum);
	pJ->m_nQtyShoe[nVerPos] = m_cbShoeEa.GetCurSel();

	SetDataInit();
}

BOOL CInputBaseShoePosition::OnInitDialog() 
{
	CInputDialogBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitTabCtrl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
