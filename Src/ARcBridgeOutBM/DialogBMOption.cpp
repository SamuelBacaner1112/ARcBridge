// DialogBMOption.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "resource.h"
#include "arcbridgeoutbm.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"


// CDialogBMOption 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogBMOption, CXTResizeDialog)

CDialogBMOption::CDialogBMOption(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CDialogBMOption::IDD, pParent)
{
 	m_nTabOut	 = -1;
 	m_nTabNormal = -1;
 	m_nTabForm	 = -1;
 	m_nTab		 = 0;
	m_nBri		 = 0;
 	m_pStd		 = NULL;

	m_nCurPier = -1;
	m_nCurAbut = -1;
}

CDialogBMOption::~CDialogBMOption()
{
}

void CDialogBMOption::DoDataExchange(CDataExchange* pDX)
{
 	CDialog::DoDataExchange(pDX);
 	DDX_Control(pDX, IDC_BUTTON_BM_SAME, m_btnSame);
 	DDX_Control(pDX, IDC_BUTTON_BM_RECOMMAND, m_btnRecommand);
 	DDX_Control(pDX, IDC_COMBO, m_comboBridge);
 	DDX_Control(pDX, IDOK, m_btnOK);
 	DDX_Control(pDX, IDCANCEL, m_btnCancel);
 	DDX_Control(pDX, IDC_TAB_OPT_OUTBM, m_ctlTab);
 	DDX_GridControl(pDX, IDC_GRID_ORG_OUTBM, m_Grid);
}


BEGIN_MESSAGE_MAP(CDialogBMOption, CXTResizeDialog)
 	ON_CBN_SELCHANGE(IDC_COMBO, OnSelchangeCombo)
 	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_OPT_OUTBM, OnTabSelchange)
 	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID_ORG_OUTBM, OnCellChangedData)
 	ON_BN_CLICKED(IDC_BUTTON_BM_RECOMMAND, OnButtonRecommand)
 	ON_BN_CLICKED(IDC_BUTTON_BM_SAME, OnButtonSame)
END_MESSAGE_MAP()


void CDialogBMOption::OnCancel() 
{
	// TODO: Add extra cleanup here

	CXTResizeDialog::OnCancel();
}

void CDialogBMOption::OnOK() 
{
	// TODO: Add extra validation here
	m_Grid.UpdateData(TRUE);
	CXTResizeDialog::OnOK();
}

BOOL CDialogBMOption::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();

	// TODO: Add extra initialization here
	SetResize(IDC_GRID_ORG_OUTBM, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_TAB_OPT_OUTBM, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK,  SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,  SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_BM_RECOMMAND,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_BM_SAME,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	GetDlgItem(IDOK)->SetWindowText(hggettext("확인"));
	GetDlgItem(IDCANCEL)->SetWindowText(hggettext("취소"));
	GetDlgItem(IDC_BUTTON_BM_RECOMMAND)->SetWindowText(hggettext("권고안"));
	GetDlgItem(IDC_BUTTON_BM_SAME)->SetWindowText(hggettext("모든 교량 동일적용"));

	InitComboCtrl();
	InitTabCtrl();
	EnableControl();
	SetGridData();
	SetDataInit();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogBMOption::SetGridData()
{
 	if(m_nTab == m_nTabOut) SetGridDataOut();
 	else if(m_nTab == m_nTabNormal) SetGridDataNormal();
 	else if(m_nTab == m_nTabForm) SetGridDataForm();
}


void CDialogBMOption::SetDataInit()
{
 	m_Grid.UpdateData(FALSE);
 
 	long nRows(0), nCols(0);
 	m_Grid.GetCellCount(nRows, nCols);

	if(m_nTab == m_nTabOut)
	{
		m_Grid.MergeGrid(1, nRows-1, 1, 2, TRUE, FALSE);
		m_Grid.MergeGrid(0, nRows-1, 3, 4, TRUE, FALSE);
	}
	else if (m_nTab == m_nTabNormal)
	{
		m_Grid.MergeGrid(1, nRows-1, 0, 1, TRUE, FALSE);
	}
	
	m_Grid.MergeGrid(0,0,0,nCols-1);
	m_Grid.MergeGridEx(0, nRows - 1, 0, 0, TRUE, TRUE);
}

void CDialogBMOption::SetDataSave()
{
 	m_Grid.UpdateData(TRUE);
}

void CDialogBMOption::InitTabCtrl()
{
 	m_ctlTab.SendInitialUpdate(TRUE);
 
 	// Clip children of the tab control from paint routines to reduce flicker.
 	m_ctlTab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
 
 	m_ctlTab.DeleteAllItems();
 
 	m_nTabOut		= -1;
 	m_nTabNormal	= -1;
 	m_nTabForm		= -1;
 	m_nTab			= 0;
 	long nIdx		= 0;
 
 	m_ctlTab.InsertItem(nIdx, hggettext("출력옵션")); m_nTabOut	= nIdx++;
 	m_ctlTab.InsertItem(nIdx, hggettext("일반수량 옵션")); m_nTabNormal	= nIdx++;
 	m_ctlTab.InsertItem(nIdx, hggettext("거푸집수량 옵션")); m_nTabForm	= nIdx++;
 
 	m_ctlTab.SetCurSel(m_nTab);
}

//출력 옵션
void CDialogBMOption::SetGridDataOut()
{
	CARcBridgeDataStd *pDataStd	= m_pStd->m_pARcBridgeDataStd;

	// 그리드 초기화
	m_Grid.SetGridDefault(2, 5, 1, 1);
	m_Grid.SetColumnWidth(0,250);
	m_Grid.SetColumnWidth(1,60);
	m_Grid.SetColumnWidth(2,200);

	UINT nFormat = DT_CENTER|DT_VCENTER;
	
	CStringArray sArr;
	long row = 0;
	long col = 0;
	m_Grid.AddCell(row, col,	  "구  분");
	m_Grid.AddCell(row, col+1, "설정값");
	m_Grid.AddCell(row++, col+2, "설정값");
	
	m_Grid.AddCell(row, col, "  본체와 교명주 철근수량 분리집계", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, &m_pStd->m_bSeparateExcludeRebarMonument,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+2, m_pStd->m_bSeparateExcludeRebarMonument ? "집계함" : "집계안함", DT_RIGHT);

	m_Grid.AddCell(row, col, "  삽도에 원지반 선 표현", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, &m_pStd->m_bDrawElEarth,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+2, m_pStd->m_bDrawElEarth ? "표현함" : "표현안함", DT_RIGHT);

	m_Grid.AddCell(row, col, "  엑셀 출력시 산출근거 수식변환", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, &m_pStd->m_bConvertToForm,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+2, m_pStd->m_bConvertToForm ? "변환함" : "변환안함", DT_RIGHT);

	m_Grid.AddCell(row, col, "  집계표간 셀 참조", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, &m_pStd->m_bRefCellBMSum,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+2, m_pStd->m_bRefCellBMSum ? "참조함" : "참조안함", DT_RIGHT);

	m_Grid.AddCell(row, col, "  산출시 기초를 바닥 및 보와 분리", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, &pDataStd->m_bSepFootingForSiljuk,nFormat,CELL_LINE_NO, "", CELL_TYPE_CHECK);
	m_Grid.AddCell(row++, col+2, pDataStd->m_bSepFootingForSiljuk ? "분리함" : "분리안함", DT_RIGHT);

	sArr.RemoveAll(); 
	sArr.Add("일반수량산출기준");
	sArr.Add("실적수량산출기준(공통수량분리)");
	sArr.Add("실적수량산출기준(공통수량포함)");
	m_Grid.AddCell(row, col, "  수량산출 적용기준", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, &pDataStd->m_nTypeBMApply, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row++, col+2, GetStringArray(sArr, pDataStd->m_nTypeBMApply), DT_RIGHT);
}

//일반수량
void CDialogBMOption::SetGridDataNormal()
{
	CARcBridgeDataStd *pStd	= m_pStd->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_nBri);
	if(!pBri) return;

	const HGBOOL bLsd = pStd->IsLsdDesign();

	// 그리드 초기화
	m_Grid.SetGridDefault(3, 5, 1, 2);
	m_Grid.SetColumnWidth(0,150);
	m_Grid.SetColumnWidth(1,220);
	m_Grid.SetColumnWidth(2,240);
	m_Grid.SetColumnWidth(3,50);
	m_Grid.SetColumnWidth(4,40);

	CStringArray sArr;
	long row = 0;
	long col = 0;
	m_Grid.AddCell(row, col,	"구  분");
	m_Grid.AddCell(row, col+1, "구  분");
	m_Grid.AddCell(row, col+2, "설정값");
	m_Grid.AddCell(row, col+3, "단  위");
	m_Grid.AddCell(row++, col+4, "비  고");

	if(pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB)
	{
		m_Grid.AddCell(row, col, "  콘크리트 양생", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  본체, 날개벽", GVHT_LEFT);
	}
	else
	{
		m_Grid.AddCell(row, col, "  콘크리트 양생", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  본체", GVHT_LEFT);
	}
	sArr.RemoveAll();
	sArr.Add("적용");
	sArr.Add("적용안함");
	m_Grid.AddCell(row++, col+2, &pBri->m_bSelectBM_ConcCare_Main, DT_RIGHT, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

	if(pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB)
	{
		m_Grid.AddCell(row, col, "  콘크리트 양생", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  접속슬래브,완충슬래브", GVHT_LEFT);
		m_Grid.AddCell(row++, col+2, &pBri->m_bSelectBM_ConcCare_Aps, DT_RIGHT, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

		m_Grid.AddCell(row, col, "  콘크리트 양생", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  콘크리트 양생 산출방법", GVHT_LEFT);
		sArr.RemoveAll();
		sArr.Add("기초를 제외한 거푸집수량+상부수평면");
		sArr.Add("기초를 포함한 수평면");
		m_Grid.AddCell(row++, col+2, &pBri->m_nSelectBM_ConcCare_Cal, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

		m_Grid.AddCell(row, col, "  교대보호블럭 설치", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  교대보호블럭 설치", GVHT_LEFT);
		sArr.RemoveAll();
		sArr.Add("없음");
		sArr.Add("하천용");
		sArr.Add("육교용");
		m_Grid.AddCell(row++, col+2, &pBri->m_nTypeRcBridgeBlock, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

		m_Grid.AddCell(row, col, "  교대보호블럭 외측설치각도", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  교대보호블럭 외측설치각도", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pBri->m_dAngleRcBridgeBlock, DT_RIGHT);
		m_Grid.AddCell(row++, col+3, "도", DT_LEFT);

		m_Grid.AddCell(row, col, "  수량집계에 접속슬래브,완충슬래브 포함", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  수량집계에 접속슬래브,완충슬래브 포함", GVHT_LEFT);
		sArr.RemoveAll();
		sArr.Add("적용");
		sArr.Add("적용안함");
		m_Grid.AddCell(row++, col+2, &pBri->m_bSelectBM_ApsCnsInclude, DT_RIGHT, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	}

	sArr.RemoveAll();
	sArr.Add("도로중심 높이 기준");
	sArr.Add("평균 높이 기준((최대+최소)/2)");
	sArr.Add("최대 높이 기준");
	sArr.Add("지점별 평균 높이 기준((최대+최소)/2)");
	m_Grid.AddCell(row, col, "  벽체 기준 높이", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  벽체 기준 높이", GVHT_LEFT);
	m_Grid.AddCell(row++, col+2, &pBri->m_nTypeHeightWall, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

	sArr.RemoveAll();
	sArr.Add("직거리");
	sArr.Add("선형거리");
	m_Grid.AddCell(row, col, "  슬래브 길이 기준", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  슬래브 길이 기준", GVHT_LEFT);
	m_Grid.AddCell(row++, col+2, &pBri->m_nTypeSlabLength, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

	
	sArr.RemoveAll();
	sArr.Add("침투식");
	sArr.Add("쉬트방수");
	m_Grid.AddCell(row, col, "  교면방수 타입", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  교면방수 타입", GVHT_LEFT);
	m_Grid.AddCell(row++, col+2, &pBri->m_nTypeSurfaceProof, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

	sArr.RemoveAll();
	sArr.Add("기초상단 기준");
	sArr.Add("지반고 기준");
	sArr.Add("기준위치 입력");
	m_Grid.AddCell(row, col, "  동바리 수량 산출 기준위치", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  동바리 수량 산출 기준위치", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pBri->m_nTypePostBaseLine, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col+3, "EL.(m)", GVHT_LEFT);
	m_Grid.AddCell(row++, col+4, &pBri->m_dPostLineEL, DT_RIGHT, pBri->m_nTypePostBaseLine != 2 ? CELL_READONLY : CELL_LINE_NO);

	m_Grid.AddCell(row, col, "비계\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  비계 수량 산출 기준위치", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pBri->m_nTypeScaffoldBaseLine, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	m_Grid.AddCell(row, col+3, "EL.(m)", GVHT_LEFT);
	m_Grid.AddCell(row++, col+4, &pBri->m_dScaffoldLineEL, DT_RIGHT, pBri->m_nTypeScaffoldBaseLine != 2 ? CELL_READONLY : CELL_LINE_NO);

	m_Grid.AddCell(row, col, "비계\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  비계 두께", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pBri->m_dThickScaffold, DT_RIGHT, CELL_LINE_NO);
	m_Grid.AddCell(row++, col+3, "mm", GVHT_LEFT);

	CStringArray sArrWing;
	sArrWing.Add(hggettext("날개벽측면 포함(설계실무요령.2016)"));
	sArrWing.Add(hggettext("날개벽측면 제외"));
	m_Grid.AddCell(row, col, "비계\n", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  비계 수량 산출 위치", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pBri->m_nTypeScaffoldSideLocation, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArrWing);
	m_Grid.AddCell(row++, col+4, "", DT_RIGHT, CELL_READONLY);
		
	sArr.RemoveAll();
	sArr.Add("육교용");
	sArr.Add("일반용");
	m_Grid.AddCell(row, col, "  본체 동바리 타입", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  본체 동바리 타입", GVHT_LEFT);
	m_Grid.AddCell(row++, col+2, &pBri->m_nTypePost, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

	sArr.RemoveAll();
	sArr.Add("공제안함");
	sArr.Add("좌측공제");
	sArr.Add("우측공제");
	m_Grid.AddCell(row, col, "  인접구조물", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  인접구조물 거푸집 수량공제", GVHT_LEFT);
	m_Grid.AddCell(row++, col+2, &pBri->m_nDeductNearFormBM, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

	m_Grid.AddCell(row, col, "  인접구조물", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  인접구조물 터파기 수량공제", GVHT_LEFT);
	m_Grid.AddCell(row++, col+2, &pBri->m_nDeductNearTupagiBM, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

	if (bLsd)
	{
		m_Grid.AddCell(row, col, "  재료 물성치 상세 입력", DT_LEFT);
		m_Grid.AddCell(row, col+1, "  재료 물성치 상세 입력", DT_LEFT);

		sArr.RemoveAll();
		sArr.Add("적용");
		sArr.Add("미적용");
		m_Grid.AddCell(row++, col+2, &pBri->m_bAggregate_SlumpDetail, DT_RIGHT, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

		const HGINT32 iEnd = pBri->m_bAggregate_SlumpDetail ? ePartCount : ePartUpperSlab + 1;
		for (HGINT32 ix = ePartUpperSlab; ix < iEnd; ++ix)
		{
			CString strPart;

			switch (ix)
			{
			case ePartUpperSlab:
				strPart = "본체/상부슬래브";
				break;
			case ePartInWall:
				strPart = "중간지점부:벽체";
				break;
			case ePartInFoot:
				strPart = "중간지점부:기초";
				break;
			default:
				strPart = pBri->GetStringPart(static_cast<EPartOfBridge>(ix));
			}

			m_Grid.AddCell(row, col, "  콘크리트골재최대치수", DT_LEFT);
			m_Grid.AddCell(row, col+1, "  " + strPart, DT_LEFT,CELL_LINE_NO);
			m_Grid.AddCell(row, col+2, &pBri->m_dAggregate[ix], DT_RIGHT);
			m_Grid.AddCell(row++, col+3, "mm", GVHT_LEFT);
		}

		if (pBri->m_bAggregate_SlumpDetail == FALSE)
		{
			m_Grid.AddCell(row, col, "  콘크리트골재최대치수", DT_LEFT);
			m_Grid.AddCell(row, col+1, "  " + pBri->GetStringPart(ePartApsStt), DT_LEFT,CELL_LINE_NO);
			m_Grid.AddCell(row, col+2, &pBri->m_dAggregate[ePartApsStt], DT_RIGHT);
			m_Grid.AddCell(row++, col+3, "mm", GVHT_LEFT);
		}
	}
	else
	{
		// 골재최대치수
		m_Grid.AddCell(row, col, "  콘크리트골재최대치수", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  기초", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pBri->m_dAggregate_Footing, DT_RIGHT, CELL_LINE_NO);
		m_Grid.AddCell(row++, col+3, "mm", GVHT_LEFT);

		m_Grid.AddCell(row, col, "  콘크리트골재최대치수", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  구체", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pBri->m_dAggregate_Main, DT_RIGHT, CELL_LINE_NO);
		m_Grid.AddCell(row++, col+3, "mm", GVHT_LEFT);

		m_Grid.AddCell(row, col, "  콘크리트골재최대치수", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  접속슬래브", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pBri->m_dAggregate_Aps, DT_RIGHT, CELL_LINE_NO);
		m_Grid.AddCell(row++, col+3, "mm", GVHT_LEFT);
	}

	m_Grid.AddCell(row, col, "  콘크리트골재최대치수", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  버림콘크리트", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, bLsd ? &pBri->m_dAggregate_LeanLSD : &pBri->m_dAggregate_Lean, DT_RIGHT, CELL_LINE_NO);
	m_Grid.AddCell(row++, col+3, "mm", GVHT_LEFT);

	m_Grid.AddCell(row, col, "  콘크리트골재최대치수", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  MASS", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, bLsd ? &pBri->m_dAggregate_MassLSD : &pBri->m_dAggregate_Mass, DT_RIGHT, CELL_LINE_NO);
	m_Grid.AddCell(row++, col+3, "mm", GVHT_LEFT);

	m_Grid.AddCell(row, col, "  콘크리트골재최대치수", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  현장타설말뚝", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, bLsd ? &pBri->m_dAggregate_HyuntaLSD : &pBri->m_dAggregate_Hyunta, DT_RIGHT, CELL_LINE_NO);
	m_Grid.AddCell(row++, col+3, "mm", GVHT_LEFT);

	m_Grid.AddCell(row, col, "  콘크리트골재최대치수", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  보호블럭", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, bLsd ? &pBri->m_dAggregate_EtcLSD : &pBri->m_dAggregate_Etc, DT_RIGHT, CELL_LINE_NO);
	m_Grid.AddCell(row++, col+3, "mm", GVHT_LEFT);

	if (bLsd)
	{
		const HGINT32 iEnd = pBri->m_bAggregate_SlumpDetail ? ePartCount : ePartUpperSlab + 1;
		for (HGINT32 ix = ePartUpperSlab; ix < iEnd; ++ix)
		{
			CString strPart;

			switch (ix)
			{
			case ePartUpperSlab:
				strPart = "본체/상부슬래브";
				break;
			case ePartInWall:
				strPart = "중간지점부:벽체";
				break;
			case ePartInFoot:
				strPart = "중간지점부:기초";
				break;
			default:
				strPart = pBri->GetStringPart(static_cast<EPartOfBridge>(ix));
			}

			m_Grid.AddCell(row, col, "  콘크리트Slump", DT_LEFT);
			m_Grid.AddCell(row, col+1, "  " + strPart, DT_LEFT,CELL_LINE_NO);
			m_Grid.AddCell(row, col+2, &pBri->m_dSlump[ix], DT_RIGHT);
			m_Grid.AddCell(row++, col+3, "cm", GVHT_LEFT);
		}

		if (pBri->m_bAggregate_SlumpDetail == FALSE)
		{
			m_Grid.AddCell(row, col, "  콘크리트Slump", DT_LEFT);
			m_Grid.AddCell(row, col+1, "  " + pBri->GetStringPart(ePartApsStt), DT_LEFT,CELL_LINE_NO);
			m_Grid.AddCell(row, col+2, &pBri->m_dSlump[ePartApsStt], DT_RIGHT);
			m_Grid.AddCell(row++, col+3, "mm", GVHT_LEFT);
		}
	}
	else
	{
		// slump
		// slump는 무조건 mm(단위계 영향 없음) 2007.11.26 수정
		CString szUnit_CGS   = "cm";
		CString szUnit_Apply = pStd->GetStringUnitType(szUnit_CGS);

		m_Grid.AddCell(row, col, "  콘크리트Slump", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  기초", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pBri->m_dSlump_Footing, DT_RIGHT, CELL_LINE_NO);// m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
		m_Grid.AddCell(row++, col+3, "cm", GVHT_LEFT);

		m_Grid.AddCell(row, col, "  콘크리트Slump", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  구체", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pBri->m_dSlump_Main, DT_RIGHT, CELL_LINE_NO); // m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
		m_Grid.AddCell(row++, col+3, "cm", GVHT_LEFT);

		m_Grid.AddCell(row, col, "  콘크리트Slump", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  접속슬래브", GVHT_LEFT);
		m_Grid.AddCell(row, col+2, &pBri->m_dSlump_Aps, DT_RIGHT, CELL_LINE_NO); // m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
		m_Grid.AddCell(row++, col+3, "cm", GVHT_LEFT);
	}

	m_Grid.AddCell(row, col, "  콘크리트Slump", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  버림콘크리트", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, bLsd ? &pBri->m_dSlump_LeanLSD : &pBri->m_dSlump_Lean, DT_RIGHT, CELL_LINE_NO); // m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(row++, col+3, "cm", GVHT_LEFT);

	m_Grid.AddCell(row, col, "  콘크리트Slump", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  MASS", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, bLsd ? &pBri->m_dSlump_MassLSD : &pBri->m_dSlump_Mass, DT_RIGHT, CELL_LINE_NO); // m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(row++, col+3, "cm", GVHT_LEFT);

	m_Grid.AddCell(row, col, "  콘크리트Slump", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  현장타설말뚝", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, bLsd ? &pBri->m_dSlump_HyuntaLSD : &pBri->m_dSlump_Hyunta, DT_RIGHT, CELL_LINE_NO); // m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(row++, col+3, "cm", GVHT_LEFT);

	m_Grid.AddCell(row, col, "  콘크리트Slump", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  방호벽등", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, bLsd ? &pBri->m_dSlump_EtcLSD : &pBri->m_dSlump_Etc, DT_RIGHT, CELL_LINE_NO); // m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(row++, col+3, "cm", GVHT_LEFT);

	sArr.RemoveAll();
	sArr.Add("적용안함");
	sArr.Add("1개");
	sArr.Add("2개");
	sArr.Add("3개");
	sArr.Add("4개");

// (ARCBRIDGE-3182) 교명주 받침 제원 입력 으로 이동	
// 	m_Grid.AddCell(row, col, "  교량 시종점부 교명주 받침 설치", GVHT_LEFT);
// 	m_Grid.AddCell(row, col+1, "  교량 시종점부 교명주 받침 설치", GVHT_LEFT);
// 	m_Grid.AddCell(row++, col+2, &pBri->m_nMonumenu, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

	m_Grid.AddCell(row, col, "  설명판 설치", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  설명판 설치", GVHT_LEFT);
	m_Grid.AddCell(row++, col+2, &pBri->m_nDesPanel, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

	m_Grid.AddCell(row, col, "  교명판 설치", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  교명판 설치", GVHT_LEFT);
	m_Grid.AddCell(row++, col+2, &pBri->m_nNamePanel, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

	sArr.RemoveAll();
	sArr.Add("슬래브 분할 높이에 따라");
	sArr.Add(" 0 ~ 7m");
	sArr.Add(" 7 ~ 10m");
	sArr.Add("10 ~ 13m");
	sArr.Add("13 ~ 16m");
	sArr.Add("16 ~ 19m");
	sArr.Add("19m 이상");
	m_Grid.AddCell(row, col, "  슬래브 거푸집 수량산출시 높이", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  슬래브 거푸집 수량산출시 높이", GVHT_LEFT);
	m_Grid.AddCell(row++, col+2, &pBri->m_nHeightForSlabBM, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

// 	// 신축이음 수량 #38979
// 	for(long stt = 0; stt < 2; stt++)
// 	{
// 		BOOL bStt	= stt == iSTT;
// 		CString sStt	= bStt ? "  신축이음(시점)" : "  신축이음(종점)";
// 		CFlexibleJoint *pFx	= bStt ? &pBri->m_fxJointStt : &pBri->m_fxJointEnd;
// 
// 		m_Grid.AddCell(row, col, sStt, GVHT_LEFT);
// 		m_Grid.AddCell(row, col+1, "  명칭", GVHT_LEFT);
// 		m_Grid.AddCell(row++, col+2, &pFx->m_sName);
// 
// 		m_Grid.AddCell(row, col, sStt, GVHT_LEFT);
// 		m_Grid.AddCell(row, col+1, "  철근량1", GVHT_LEFT);
// 		m_Grid.AddCell(row++, col+2, &pFx->m_rb[0].m_dDia, DT_RIGHT, CELL_LINE_NO, "", CELL_TYPE_DIA);
// 
// 		m_Grid.AddCell(row, col, sStt, GVHT_LEFT);
// 		m_Grid.AddCell(row, col+1, "  철근량1", GVHT_LEFT);
// 		m_Grid.AddCell(row, col+2, &pFx->m_rb[0].m_dLengthTotal);
// 		m_Grid.AddCell(row++, col+3, "tonf");
// 
// 		m_Grid.AddCell(row, col, sStt, GVHT_LEFT);
// 		m_Grid.AddCell(row, col+1, "  철근량2", GVHT_LEFT);
// 		m_Grid.AddCell(row++, col+2, &pFx->m_rb[1].m_dDia, DT_RIGHT, CELL_LINE_NO, "", CELL_TYPE_DIA);
// 		
// 		m_Grid.AddCell(row, col, sStt, GVHT_LEFT);
// 		m_Grid.AddCell(row, col+1, "  철근량2", GVHT_LEFT);
// 		m_Grid.AddCell(row, col+2, &pFx->m_rb[1].m_dLengthTotal);
// 		m_Grid.AddCell(row++, col+3, "tonf");
// 	}

	m_Grid.AddCell(row, col, "  택 코팅", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  택 코팅", GVHT_LEFT);
	m_Grid.AddCell(row++, col+2, &pBri->m_dTackCotting, DT_RIGHT, CELL_LINE_NO);

	sArr.RemoveAll();
	sArr.Add("π(D+0.2)²/4 x L");
	sArr.Add("(π(D+0.2)²/4 - πD²/4) x L");
	m_Grid.AddCell(row, col, "  내부굴착말뚝 시멘트 밀크량 산정식", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  내부굴착말뚝 시멘트 밀크량 산정식", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pBri->m_nTypePileMilkBM, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	
	m_Grid.AddCell(row, col, "  철근가공조립에서 다웰바 수량 제외", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  철근가공조립에서 다웰바 수량 제외", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pBri->m_bExcludeApsDowelRebar, DT_RIGHT, CELL_LINE_NO, "", CELL_TYPE_CHECK);	
	m_Grid.AddCell(row++, col+3, pBri->m_bExcludeApsDowelRebar ? "적용" : "적용안함", DT_RIGHT);

	m_Grid.AddCell(row, col, "  물푸기", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  콘크리트 양생, 거푸집 해체", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pBri->m_dTimeConcForm, DT_RIGHT, CELL_LINE_NO);
	m_Grid.AddCell(row++, col+3, "hr", GVHT_LEFT);

	m_Grid.AddCell(row, col, "  용수터파기 적용", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  용수터파기 적용", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pBri->m_bApplyYongSu, DT_RIGHT, CELL_LINE_NO, "", CELL_TYPE_CHECK);	
	m_Grid.AddCell(row++, col+3, pBri->m_bApplyYongSu ? "적용" : "적용안함", DT_RIGHT);

	m_Grid.AddCell(row, col, "  현장타설말뚝", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  현장타설말뚝 잔토처리", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pBri->m_dSurplusSoil_HyunTaPile, DT_RIGHT, CELL_LINE_NO);// m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(row++, col+3, "%", GVHT_LEFT);

	m_Grid.AddCell(row, col, "  현장타설말뚝", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  현장타설말뚝 콘크리트 타설(토사)", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pBri->m_dConcreteSoil_HyunTaPile, DT_RIGHT, CELL_LINE_NO); // m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(row++, col+3, "할증(%)", GVHT_LEFT);

	m_Grid.AddCell(row, col, "  현장타설말뚝", GVHT_LEFT);
	m_Grid.AddCell(row, col+1, "  현장타설말뚝 콘크리트 타설(암반)", GVHT_LEFT);
	m_Grid.AddCell(row, col+2, &pBri->m_dConcreteRock_HyunTaPile, DT_RIGHT, CELL_LINE_NO); // m_Grid.SetCellUnit(szUnit_CGS,szUnit_Apply);
	m_Grid.AddCell(row++, col+3, "할증(%)", GVHT_LEFT);
}

//거푸집
void CDialogBMOption::SetGridDataForm()
{
	CARcBridgeDataStd *pStd	= m_pStd->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_nBri);
	if(!pBri) return;

	// 그리드 초기화
	m_Grid.SetGridDefault(2, 5, 1, 2);
	m_Grid.SetColumnWidth(0,150);
	m_Grid.SetColumnWidth(1,200);
	m_Grid.SetColumnWidth(2,190);
	m_Grid.SetColumnWidth(3,40);
	m_Grid.SetColumnWidth(4,40);

	CStringArray sArr;
	long row = 0;
	long col = 0;
	m_Grid.AddCell(row, col,	"구  분");
	m_Grid.AddCell(row, col+1, "구  분");
	m_Grid.AddCell(row, col+2, "설정값");
	m_Grid.AddCell(row, col+3, "단  위");
	m_Grid.AddCell(row++, col+4, "비  고");

// 	if(pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB)
// 	{
// 		m_Grid.AddCell(row, col, "  거푸집", GVHT_LEFT);
// 	}
// 	else
// 	{
		m_Grid.AddCell(row, col, "  거푸집", GVHT_LEFT);
// 	}
	m_Grid.AddCell(row, col+1, "  일반콘크리트", GVHT_LEFT);
	sArr.RemoveAll();
	if(pStd->m_nTypeBMApply == BMTYPE_NORMAL)
	{
		long i=0; for(i=0; i<MAX_BM_FORM_TYPE_NORMAL; i++)
			sArr.Add(pStd->GetStringBM_FormType(i));
	}
	else
	{
		long i=0; for(i=0; i<MAX_BM_FORM_TYPE; i++)
			sArr.Add(pStd->GetStringBM_FormType(i));
	}
	m_Grid.AddCell(row++, col+2, &pBri->m_nFormType_ConcNormal, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

	// 슬래브교일때 하부측 제원은 입력받지 않음
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB)
	{
		m_Grid.AddCell(row, col, "  거푸집", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  기초부", GVHT_LEFT);
		m_Grid.AddCell(row++, col+2, &pBri->m_nFormType_Footing, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

		m_Grid.AddCell(row, col, "  거푸집", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  벽체전면", GVHT_LEFT);
		m_Grid.AddCell(row++, col+2, &pBri->m_nFormType_WallFront, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

		m_Grid.AddCell(row, col, "  거푸집", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  벽체배면", GVHT_LEFT);
		m_Grid.AddCell(row++, col+2, &pBri->m_nFormType_WallBack, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

		for(long stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			for(long left = 0; left < 2; left++)
			{
				BOOL bLeft	= left == iLEFT;
				CString sWing	= _T("");
				sWing.Format("  %s 날개벽(%s)", bStt ? "시점" : "종점", bLeft ? "좌" : "우");

				m_Grid.AddCell(row, col, "  거푸집", GVHT_LEFT);
				m_Grid.AddCell(row, col+1, sWing+"전면", GVHT_LEFT);
				m_Grid.AddCell(row++, col+2, &pBri->m_nFormType_WingFront[stt][left], DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

				m_Grid.AddCell(row, col, "  거푸집", GVHT_LEFT);
				m_Grid.AddCell(row, col+1, sWing+"배면", GVHT_LEFT);
				m_Grid.AddCell(row++, col+2, &pBri->m_nFormType_WingBack[stt][left], DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
			}
		}

		m_Grid.AddCell(row, col, "  거푸집", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  접속슬래브", GVHT_LEFT);
		m_Grid.AddCell(row++, col+2, &pBri->m_nFormType_Aps, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

		m_Grid.AddCell(row, col, "  거푸집", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  MASS 콘크리트", GVHT_LEFT);
		m_Grid.AddCell(row++, col+2, &pBri->m_nFormType_MassConc, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);

		m_Grid.AddCell(row, col, "  거푸집", GVHT_LEFT);
		m_Grid.AddCell(row, col+1, "  LEAN 콘크리트", GVHT_LEFT);
		m_Grid.AddCell(row++, col+2, &pBri->m_nFormType_LeanConc, DT_RIGHT, TRUE, CELL_LINE_NO, "", CELL_TYPE_COMBO, sArr);
	}
}

 void CDialogBMOption::SetOutBMStd(CARcBridgeOutBMStd *pStd)
 {
	m_pStd = pStd;
 }

void CDialogBMOption::OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult) 
{
 	m_nTab	= m_ctlTab.GetCurSel();

	EnableControl();
 	SetGridData();
 	SetDataInit();
 
 	return;
}

void CDialogBMOption::OnSelchangeCombo() 
{
	m_nBri	= m_comboBridge.GetCurSel();
	SetGridData();
	SetDataInit();
}

void CDialogBMOption::InitComboCtrl()
{
	CARcBridgeDataStd *pStd	= m_pStd->m_pARcBridgeDataStd;
	if(!pStd) return;

	// 콤보생성
	long i = 0; for(i = 0; i < m_comboBridge.GetCount(); i++)
		m_comboBridge.DeleteString(0);

	long nCountBridge	= pStd->GetBridgeSize();
	for(i = 0; i < nCountBridge; i++)
		m_comboBridge.AddString(pStd->GetBridgeTotalName(i));
	
	m_comboBridge.SetCurSel(m_nBri);

}

void CDialogBMOption::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{	
	CARcBridgeDataStd* pStd = m_pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_nBri);
	if (pBri == NULL)
		return;

	CCellID cell = m_Grid.GetFocusCell();
	CString str  = m_Grid.GetTextMatrix(cell.row, 0);
	CString str1  = m_Grid.GetTextMatrix(cell.row, 1);

	if (str == "  재료 물성치 상세 입력")
	{
		SyncAggregateAndSlump(pBri);
	}
	
	if (pBri->m_bAggregate_SlumpDetail == FALSE)
	{
		if (str1 == "  본체/상부슬래브" || str1 == "  " + pBri->GetStringPart(ePartApsStt))
		{
			SyncAggregateAndSlump(pBri);
		}
		
	}
	
	SetDataSave();

	if(str ==  _T("  물푸기"))	pBri->m_dTimeConcForm = pBri->m_dTimeConcForm < 0? 0 : pBri->m_dTimeConcForm;
 	SetGridData();
 	SetDataInit();

}

BOOL CDialogBMOption::PreTranslateMessage(MSG* pMsg) 
{
	//TODO: Add your specialized code here and/or call the base class
 	m_Grid.ViewGridCellForCombo(pMsg);
 	
 	return CXTResizeDialog::PreTranslateMessage(pMsg);
}

CString CDialogBMOption::GetStringArray(CStringArray &sArr, long nIdx)
{
 	if(nIdx < 0 || nIdx >= sArr.GetSize()) return _T("");
 	return sArr.GetAt(nIdx);
}

void CDialogBMOption::OnButtonRecommand() 
{
 	if(AfxMessageBox(hggettext("현재 입력 상자의 데이터가 모두 기본값으로 바뀝니다. 계속 하시겠습니까?  "), MB_YESNO | MB_ICONQUESTION)==IDNO)
 		return;	
 	
	CARcBridgeDataStd *pStd	= m_pStd->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(m_nBri);
	if(!pBri) return;

 	if(m_nTab == m_nTabOut)
 		SetDataDefaultOut();
 	else if(m_nTab == m_nTabNormal)
 		pBri->SetDataDefaultBM(1);
 	else if(m_nTab == m_nTabForm)
 		pBri->SetDataDefaultBM(0);
 
 	SetGridData();
 	SetDataInit();
}

void CDialogBMOption::OnButtonSame() 
{
	// TODO: Add your control notification handler code here
	// 입력사항 추가시 항목을 추가해주어야함....
	CARcBridgeDataStd* pStd = m_pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pStd->GetBridgeRebar(m_nBri);
	if(!pBri) return;

	CString strMsg = _T("");
	strMsg.Format("모든 교량의 전체 입력사항이 %s과(와) 동일하게 설정됩니다.\n계속하시겠습니까 ?",pBri->m_strBridgeName);
	if(AfxMessageBox(strMsg,MB_YESNO)==IDYES)
	{
		long nQtyBri = pStd->GetBridgeSize();

		long i=0; for(i=0; i<nQtyBri; i++)
		{
			if(i == m_nBri) continue;

			CRcBridgeRebar* pBri2 = pStd->GetBridgeRebar(i);

			MakeSameBM(pBri2,pBri);
		}
	}	
}

void CDialogBMOption::SetDataDefaultOut()
{
	CARcBridgeDataStd *pDataStd	= m_pStd->m_pARcBridgeDataStd;

	m_pStd->m_bSeparateExcludeRebarMonument = FALSE;
	m_pStd->m_bDrawElEarth = TRUE;
	m_pStd->m_bConvertToForm = TRUE;
	m_pStd->m_bRefCellBMSum = FALSE;
	pDataStd->m_bSepFootingForSiljuk = FALSE;
}

void CDialogBMOption::MakeSameBM(CRcBridgeApp* pBri1, CRcBridgeApp* pBri2)
{
	if(pBri1==NULL || pBri2==NULL) return;

	// 수량선택사항(일반수량, 거푸집)
	pBri1->m_nFormType_ConcNormal		= pBri2->m_nFormType_ConcNormal;
	pBri1->m_nFormType_Footing			= pBri2->m_nFormType_Footing;
	pBri1->m_nFormType_WallFront		= pBri2->m_nFormType_WallFront;
	pBri1->m_nFormType_WallBack			= pBri2->m_nFormType_WallBack;
	for(long stt = 0; stt < 2; stt++)
	{
		for(long left = 0; left < 2; left++)
		{
			pBri1->m_nFormType_WingFront[stt][left]	= pBri2->m_nFormType_WingFront[stt][left];
			pBri1->m_nFormType_WingBack[stt][left]	= pBri2->m_nFormType_WingBack[stt][left];
		}
	}
	pBri1->m_nFormType_Aps				= pBri2->m_nFormType_Aps;
	pBri1->m_nFormType_MassConc			= pBri2->m_nFormType_MassConc;
	pBri1->m_nFormType_LeanConc			= pBri2->m_nFormType_LeanConc;
	pBri1->m_bSelectBM_ConcCare_Main	= pBri2->m_bSelectBM_ConcCare_Main;
	pBri1->m_bSelectBM_ConcCare_Aps		= pBri2->m_bSelectBM_ConcCare_Aps;
	pBri1->m_nSelectBM_ConcCare_Cal		= pBri2->m_nSelectBM_ConcCare_Cal;
	pBri1->m_nTypeRcBridgeBlock			= pBri2->m_nTypeRcBridgeBlock;
	pBri1->m_dAngleRcBridgeBlock		= pBri2->m_dAngleRcBridgeBlock;
	pBri1->m_bSelectBM_ApsCnsInclude	= pBri2->m_bSelectBM_ApsCnsInclude;
	pBri1->m_dPavementThick				= pBri2->m_dPavementThick;
	pBri1->m_dNosangThick				= pBri2->m_dNosangThick;
	pBri1->m_nTypePost					= pBri2->m_nTypePost;
	pBri1->m_nTypeHeightWall			= pBri2->m_nTypeHeightWall;
	pBri1->m_nTypeSlabLength			= pBri2->m_nTypeSlabLength;
	pBri1->m_nTypeSurfaceProof			= pBri2->m_nTypeSurfaceProof;
	pBri1->m_nTypePostBaseLine			= pBri2->m_nTypePostBaseLine;
	pBri1->m_dThickPostUpper			= pBri2->m_dThickPostUpper;
	pBri1->m_dThickPostSide				= pBri2->m_dThickPostSide;
	pBri1->m_dThickScaffold				= pBri2->m_dThickScaffold;
	pBri1->m_nDeductNearFormBM			= pBri2->m_nDeductNearFormBM;
	pBri1->m_nDeductNearTupagiBM		= pBri2->m_nDeductNearTupagiBM;
	pBri1->m_dAggregate_Footing			= pBri2->m_dAggregate_Footing;
	pBri1->m_dAggregate_Main			= pBri2->m_dAggregate_Main;
	pBri1->m_dAggregate_Aps				= pBri2->m_dAggregate_Aps;
	pBri1->m_dAggregate_Lean			= pBri2->m_dAggregate_Lean;
	pBri1->m_dAggregate_Mass			= pBri2->m_dAggregate_Mass;
	pBri1->m_dAggregate_Hyunta			= pBri2->m_dAggregate_Hyunta;
	pBri1->m_dAggregate_Etc				= pBri2->m_dAggregate_Etc;
	pBri1->m_dSlump_Footing				= pBri2->m_dSlump_Footing;
	pBri1->m_dSlump_Main				= pBri2->m_dSlump_Main;
	pBri1->m_dSlump_Aps					= pBri2->m_dSlump_Aps;
	pBri1->m_dSlump_Lean				= pBri2->m_dSlump_Lean;
	pBri1->m_dSlump_Mass				= pBri2->m_dSlump_Mass;
	pBri1->m_dSlump_Hyunta				= pBri2->m_dSlump_Hyunta;
	pBri1->m_dSlump_Etc					= pBri2->m_dSlump_Etc;

	pBri1->m_bAggregate_SlumpDetail		= pBri2->m_bAggregate_SlumpDetail;
	for (HGINT32 ix = 0; ix < ePartCount; ++ix)
	{
		pBri1->m_dAggregate[ix]			= pBri2->m_dAggregate[ix];
		pBri1->m_dSlump[ix]				= pBri2->m_dSlump[ix];
	}
	
	pBri1->m_dAggregate_LeanLSD			= pBri2->m_dAggregate_LeanLSD;
	pBri1->m_dAggregate_MassLSD			= pBri2->m_dAggregate_MassLSD;
	pBri1->m_dAggregate_HyuntaLSD		= pBri2->m_dAggregate_HyuntaLSD;
	pBri1->m_dAggregate_EtcLSD			= pBri2->m_dAggregate_EtcLSD;
	pBri1->m_dSlump_LeanLSD				= pBri2->m_dSlump_LeanLSD;
	pBri1->m_dSlump_MassLSD				= pBri2->m_dSlump_MassLSD;
	pBri1->m_dSlump_HyuntaLSD			= pBri2->m_dSlump_HyuntaLSD;
	pBri1->m_dSlump_EtcLSD				= pBri2->m_dSlump_EtcLSD;
}

void CDialogBMOption::SyncAggregateAndSlump(CRcBridgeApp *pBri)
{
	if (pBri == NULL)
		return;

	for (HGINT32 ix = ePartUpperSlab + 1; ix < ePartCount; ++ix)
	{
		if (ix == ePartApsStt || ix == ePartApsEnd)
			continue;

		pBri->m_dAggregate[ix]	 = pBri->m_dAggregate[ePartUpperSlab];
		pBri->m_dSlump[ix]	 = pBri->m_dSlump[ePartUpperSlab];
	}

	pBri->m_dAggregate[ePartApsEnd] = pBri->m_dAggregate[ePartApsStt];
	pBri->m_dSlump[ePartApsEnd] = pBri->m_dSlump[ePartApsStt];
}

void CDialogBMOption::EnableControl()
{
	if (m_nTab == m_nTabOut)
	{
		m_comboBridge.EnableWindow(FALSE);
	}
	else
	{
		m_comboBridge.EnableWindow(TRUE);
	}
}
