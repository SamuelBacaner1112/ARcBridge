// DrawingGeneral.cpp: implementation of the CDrawingGeneral class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"
#include "../ARcBridgeDrawingOption/ARcBridgeDrawingOption.h"
#include "../../Src/ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "ARcBridgeDrawing.h"
#include "DrawingGeneral.h"

#define MPE_PLANE				10		// 평면도
#define MPE_CROSS				20		// 횡단면도
#define MPE_LONGI				30		// 종단면도
#define MPE_DETAIL_A			40		// 상세 "A"
#define MPE_NOTCH				50		// NOTCH 상세
#define MPE_ORDER				60		// 시공 순서도
#define MPE_FRONT				70		// 정면도
#define MPE_FOOT_PLANE			130		// 기초 평면도
#define MPE_WING_FRONT_BL		190		// 시점 좌측 날개벽
#define MPE_WING_SECT_BL		200		// 시점 좌측 날개벽 단면도
#define MPE_WING_PLANE_BL		205		// 시점 좌측 날개벽 평면도
#define MPE_WING_FRONT_ER		210		// 종점 우측 날개벽
#define MPE_WING_SECT_ER		220		// 종점 우측 날개벽 단면도
#define MPE_WING_PLANE_ER		225		// 종점 우측 날개벽 평면도
#define MPE_WING_FRONT_BR		230		// 시점 우측 날개벽
#define MPE_WING_SECT_BR		240		// 시점 우측 날개벽 단면도
#define MPE_WING_PLANE_BR		245		// 시점 우측 날개벽 평면도
#define MPE_WING_FRONT_EL		250		// 종점 좌측 날개벽
#define MPE_WING_SECT_EL		260		// 종점 좌측 날개벽 단면도
#define MPE_WING_PLANE_EL		265		// 종점 좌측 날개벽 평면도
#define MPE_DETAIL_B			270		// 상세 "B"
#define MPE_NOTE				280		// NOTE
#define MPE_CONCJOINT			290		// 시공이음 상세
#define MPE_WALL_PLANE			300		// 벽체 평면도
#define MPE_DETAIL_FXJOINT_S	310		// 신축이음 상세도(시점)
#define MPE_DETAIL_FXJOINT_E	320		// 신축이음 상세도(종점)
#define MPE_DETAIL_CRACK		330		// 균열유발용 줄눈 상세
#define MPE_PILELEN_TABLE		340		// 말뚝길이 테이블

#define MPE_TITLE_PLANE			11		// 평면도 제목
#define MPE_TITLE_CROSS			21		// 횡단면도 제목
#define MPE_TITLE_LONGI			31		// 종단면도 제목
#define MPE_TITLE_DETAIL_A		41		// 상세 "A" 제목
#define MPE_TITLE_NOTCH			51		// NOTCH 상세 제목
#define MPE_TITLE_ORDER			61		// 시공 순서도 제목
#define MPE_TITLE_FRONT			71		// 정면도 제목
#define MPE_TITLE_FOOT_PLANE	131		// 기초 평면도 제목
#define MPE_TITLE_WING_FRONT_BL	191		// 시점 좌측 날개벽 제목
#define MPE_TITLE_WING_SECT_BL	201		// 시점 좌측 날개벽 단면도 제목
#define MPE_TITLE_WING_PLANE_BL	202		// 시점 좌측 날개벽 평면도 제목
#define MPE_TITLE_WING_FRONT_ER	211		// 종점 우측 날개벽 제목
#define MPE_TITLE_WING_SECT_ER	221		// 종점 우측 날개벽 단면도 제목
#define MPE_TITLE_WING_PLANE_ER	222		// 종점 우측 날개벽 평면도 제목
#define MPE_TITLE_WING_FRONT_BR	231		// 시점 우측 날개벽 제목
#define MPE_TITLE_WING_SECT_BR	241		// 시점 우측 날개벽 단면도 제목
#define MPE_TITLE_WING_PLANE_BR	242		// 시점 우측 날개벽 평면도 제목
#define MPE_TITLE_WING_FRONT_EL	251		// 종점 좌측 날개벽 제목
#define MPE_TITLE_WING_SECT_EL	261		// 종점 좌측 날개벽 단면도 제목
#define MPE_TITLE_WING_PLANE_EL	262		// 종점 좌측 날개벽 평면도 제목
#define MPE_TITLE_DETAIL_B		271		// 상세 "B" 제목
#define MPE_TITLE_NOTE			281		// NOTE
#define MPE_TITLE_CONCJOINT		291		// 시공이음 상세 제목
#define MPE_TITLE_WALL_PLANE	301		// 벽체 평면도 제목
#define MPE_TITLE_DETAIL_FXJOINT_S	311		// 신축이음 상세도(시점) 제목
#define MPE_TITLE_DETAIL_FXJOINT_E	321		// 신축이음 상세도(종점) 제목
#define MPE_TITLE_DETAIL_CRACK		331		// 균열 유발용 줄눈 상세 제목
#define MPE_TITLE_PILELEN_TABLE		341		// 말뚝길이 테이블 제목
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawingGeneral::CDrawingGeneral()
{
	m_nBridge = -1;
}

CDrawingGeneral::~CDrawingGeneral()
{

}

long CDrawingGeneral::GetPageSu()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;

	if(pStd->GetCountTotalPage()<=0)
		pStd->MakePageInfo();

	m_nPage	= pStd->GetCountCategoryPage(_T("일반도"));

	return m_nPage;
}

CString CDrawingGeneral::GetTitle()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("일반도"), pStd->m_nCurPage-m_nSttPage);
	if(pInfo==NULL) return _T("");

	CString strTitle = pStd->GetStringTitle(pStd->m_pARcBridgeDataStd->GetBridge(m_nBridge), pInfo->m_strTitle);

	return strTitle;
}

double CDrawingGeneral::GetScale()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pOptData = pOpt->GetDrawingOption(m_nBridge);

	m_Scale	= pOptData->m_dScaleGenBase;
	return pOptData->m_dScaleGenBase;
}

void CDrawingGeneral::SetNumber(long nSubPage)
{
	m_nBridge = GetNumberBridge(nSubPage-m_nSttPage);
}

long CDrawingGeneral::GetNumberBridge(long nCategoryPageNum)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("일반도"), nCategoryPageNum);
	if(pInfo==NULL) return -1;

	return pInfo->m_nBri;
}

// 현재 그려야 되는 RcBridge리턴
CRcBridgeRebar* CDrawingGeneral::GetRcBridgeRebar()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDataStd* pDataStd	= pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = NULL;

	if(m_nBridge>-1 && pDataStd->GetBridgeSize()>m_nBridge)
		pBri = pDataStd->GetBridgeRebar(m_nBridge);

	return pBri;
}

void CDrawingGeneral::DrawOnePage()
{
	TRACE("CDrawingGeneral");
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDataStd* pDataStd = pStd->m_pARcBridgeDataStd;
	CDrawDogak* pDgk = pStd->GetDogak();	
	CDrawPageMng* pMng = pStd->GetPageManage();

	long nPage = pStd->m_nCurPage;

	SetNumber(pStd->m_nDrawPage);
	double dScale = GetScale();

	CDomyun* pDom = pStd->GetDomyun(nPage);
	pDom->SetScaleDim(dScale);

	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("일반도"), pStd->m_nDrawPage-m_nSttPage);
	if(pInfo==NULL) return;

	nPage = pStd->GetPageNum(pInfo);

	pStd->m_pOptStd->SetEnvType(pDom, HCAD_MIS3);
	pStd->m_pOptStd->SetEnvType(pDom, HCAD_SYMB);

	CDomyun Dom(pDom);

	pMng->SetProgressData(_T("도각 출력 중..."), 0);

	long nSttPage = 0;
	CString strDgkName = pOpt->m_strDgkName;
	CString strHeadNote = pOpt->m_strDomNumHead;
	CString strSmallTitle = GetTitle();
	CString strXRefPath = IsDxfOut() ? pOpt->m_strXRefPath : _T("");

	CRcBridgeApp* pBri = pDataStd->GetBridge(m_nBridge);
	pStd->m_pARcBridgeDataStd->m_strTempName = pBri->m_strBridgeName;

	pDgk->Add(&Dom, nPage, dScale, strDgkName, strHeadNote, strSmallTitle, nSttPage, strXRefPath);
	pDgk->AddTitle(&Dom, "", strSmallTitle, _T(""), FALSE);

	pDgk->SetHCADExtendedData(pDataStd->m_strProjectName, pBri->m_strBridgeName, pDataStd->m_strEngName,
							  pDataStd->m_strConstructorName, pDataStd->m_strSpotName, _T(""));

	// 도각 크기를 페이지에 넘겨줘서 최대한 적절하게 배치 되도록 도움을 준다.
	Dom.SetCalcExtRect();
	CDRect rcDogak	= Dom.GetExtRect();
	if(m_bDrawForMe)
		*pDom << Dom;
	else
		Dom.ClearEtt(TRUE, FALSE);

	// 출력
	BOOL bWingWall = FALSE;
	if(pBri->IsWingWall(TRUE, TRUE))	bWingWall = TRUE;
	if(pBri->IsWingWall(TRUE, FALSE))	bWingWall = TRUE;
	if(pBri->IsWingWall(FALSE, TRUE))	bWingWall = TRUE;
	if(pBri->IsWingWall(FALSE, FALSE))	bWingWall = TRUE;

	long nPageNum = pInfo->m_nPageNumber;
	long nCountCatePage = pStd->GetCountBriCategoryPage(m_nBridge, _T("일반도"));
	if(nPageNum==1)
		AddPageGeneral1(&Dom, rcDogak);
	else if(nPageNum==nCountCatePage && bWingWall)	// 날개벽이 있는 경우만 출력
		AddPageGeneral3(&Dom, rcDogak);
	else
		AddPageGeneral2(&Dom, rcDogak, nPageNum);

	pMng->SetProgressData(_T("출력 완료  ..."), 100);

	*pDom << Dom;
}

// 일반도(1)
void CDrawingGeneral::AddPageGeneral1(CDomyun *pDomP, CDRect rcDogak)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	double dScale = GetScale();
	double dScaleDetailA = pData->m_dScaleGenDetailA;
	DWORD dOptDraw	= pData->m_dwDrawGen;

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(GetScale());

	CDRect rcBase(0, 0, 0, 0);
	CDRect rcPlane(0, 0, 0, 0);		// 평면도 영역
	CDRect rcCross(0, 0, 0, 0);		// 횡단면도 영역
	CDRect rcLongi(0, 0, 0, 0);		// 종단면도 영역
	CDRect rcDetailA(0, 0, 0, 0);	// 상세 "A" 영역
	CDRect rcNotch(0, 0, 0, 0);		// NOTCH 상세 영역
	CDRect rcConcJoint(0, 0, 0, 0);	// 시공이음 상세 영역
	CDRect rcOrder(0, 0, 0, 0);		// 시공 순서도 영역
	CDRect rcDetailB(0, 0, 0, 0);	// 상세 "B" 영역
	CDRect rcNote(0, 0, 0, 0);		// Note
	CDRect rcDetailFx[2] = {CDRect(0, 0, 0, 0), CDRect(0, 0, 0, 0)};	// 신축이음 상세 영역
	CDRect rcCrack(0, 0, 0, 0);		// 균열 유발용 줄눈 상세 영역

	pBri->m_nTypeJongdanForRebarOld	= pBri->m_nTypeJongdanForRebar;
	pBri->m_bFirstCalledGetTvVertSection_Std	= FALSE;

	// 평면도 //////////////////////////////////////////////////////////////////////
	if(dOptDraw & GEN_PLANE)
	{
		if(m_pStd->IsDrawByBlock(MPE_PLANE))
		{
			pPageMng->SetProgressData(_T("평면도 출력중..."), 5);
			AddPlane(&Dom);
			Dom.SetCalcExtRect();
			rcBase = Dom.GetExtRect();

			Dom.Move(Dom.Always(300), Dom.Always(530)-rcBase.Height()); 
			Dom.SetCalcExtRect();
			rcPlane	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_PLANE));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_PLANE, _T("평면도"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_TITLE_PLANE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("평면도"), CDPoint(0,0), 0, dScale);
			Dom.Move(Dom.Always(300)+rcPlane.Width()/2.0, Dom.Always(530)+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_TITLE_PLANE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_PLANE, _T("평면도 제목"));
			*pDomP << Dom;
		}
	}

	// 횡단면도 //////////////////////////////////////////////////////////////////////
	if(dOptDraw & GEN_CROSS)
	{
		if(m_pStd->IsDrawByBlock(MPE_CROSS))
		{
			pPageMng->SetProgressData(_T("횡단면도 출력중..."), 18);
			AddCross(&Dom);
			Dom.SetCalcExtRect();
			rcCross	= Dom.GetExtRect();

			Dom.Move(Dom.Always(80), Dom.Always(530)-rcCross.Height()); 
			Dom.SetCalcExtRect();
			rcCross	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_CROSS));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_CROSS, _T("횡단면도"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_TITLE_CROSS))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("횡단면도"), CDPoint(0,0), 0, dScale);
			Dom.Rotate(CDPoint(0,0), CDPoint(0, 1));
			Dom.Move(Dom.Always(80)-GAP_TITLE, Dom.Always(530)-rcCross.Height()/2);
			Dom.Move(pMove->GetPoint(MPE_TITLE_CROSS));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_CROSS, _T("횡단면도 제목"));
			*pDomP << Dom;
		}
	}
	
	// 상세 "A" //////////////////////////////////////////////////////////////////////
	if(dOptDraw & GEN_DETAIL_A)
	{
		CDPoint xyTemp(0, 0);
		if(m_pStd->IsDrawByBlock(MPE_DETAIL_A))
		{
			double dTemp = 0;
			pPageMng->SetProgressData(_T("상세 \"A\" 출력중..."), 29);
			AddDetailA(&Dom, xyTemp, dTemp);
			Dom.SetCalcExtRect();
// 			rcDetailA = Dom.GetExtRect();

			Dom.Move(Dom.Always(70), Dom.Always(80));
			Dom.SetCalcExtRect();
			rcDetailA = Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_DETAIL_A));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_DETAIL_A, _T("상세 \"A\""));
			*pDomP << Dom;
		}


		if(m_pStd->IsDrawByBlock(MPE_TITLE_DETAIL_A))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("상세 \"A\""), CDPoint(0,0), 0, dScaleDetailA);
			Dom.Move(Dom.Always(70)+rcDetailA.Width()/2.0, Dom.Always(80)+rcDetailA.Height()+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_TITLE_DETAIL_A));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_DETAIL_A, _T("상세 \"A\" 제목"));
			*pDomP << Dom;
		}
	}
	
	// NOTCH 상세 //////////////////////////////////////////////////////////////////////
	if(dOptDraw & GEN_NOTCH)
	{
		if(m_pStd->IsDrawByBlock(MPE_NOTCH))
		{
			pPageMng->SetProgressData(_T("NOTCH 상세 출력중..."), 41);
			AddNotch(&Dom);
			Dom.SetCalcExtRect();
// 			rcNotch = Dom.GetExtRect();

			Dom.Move(Dom.Always(950), Dom.Always(220));
			Dom.SetCalcExtRect();
			rcNotch = Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_NOTCH));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_NOTCH, _T("NOTCH 상세"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_TITLE_NOTCH))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("NOTCH 상세"), CDPoint(0,0), 0, 0);
			Dom.Move(Dom.Always(950)+rcNotch.Width()/2.0, Dom.Always(220)+rcNotch.Height()+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_TITLE_NOTCH));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_NOTCH, _T("NOTCH 상세 제목"));
			*pDomP << Dom;
		}
	}

	// 시공이음 상세 //////////////////////////////////////////////////////////////////////
	if(dOptDraw & GEN_CONCJOINT)
	{
		if(m_pStd->IsDrawByBlock(MPE_CONCJOINT))
		{
			pPageMng->SetProgressData(_T("시공이음 상세 출력중..."), 50);
			AddDetailConcJoint(&Dom);
			Dom.SetCalcExtRect();
// 			rcConcJoint = Dom.GetExtRect();
			
			Dom.Move(Dom.Always(850), Dom.Always(300));
			Dom.SetCalcExtRect();
			rcConcJoint = Dom.GetExtRect();
			
			Dom.Move(pMove->GetPoint(MPE_CONCJOINT));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_CONCJOINT, _T("시공이음 상세"));
			*pDomP << Dom;
		}
		
		if(m_pStd->IsDrawByBlock(MPE_TITLE_CONCJOINT))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("시공이음 상세"), CDPoint(0,0), 0, 0);
			Dom.Move(Dom.Always(850)+rcConcJoint.Width()/2.0, Dom.Always(300)+rcConcJoint.Height()+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_TITLE_CONCJOINT));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_CONCJOINT, _T("시공이음 상세 제목"));
			*pDomP << Dom;
		}
	}


	// 균열시공이음 상세 //////////////////////////////////////////////////////////////////////
	if(dOptDraw & GEN_DETAIL_CRACK)
	{
		if(m_pStd->IsDrawByBlock(MPE_DETAIL_CRACK))
		{
			pPageMng->SetProgressData(_T("균열유발용 줄눈 상세 출력중..."), 50);

			AddDetailAntiCrack(&Dom);
			Dom.SetCalcExtRect();
// 			rcCrack = Dom.GetExtRect();
			
			Dom.Move(Dom.Always(850), Dom.Always(450));
			Dom.SetCalcExtRect();
			rcCrack = Dom.GetExtRect();
			
			Dom.Move(pMove->GetPoint(MPE_DETAIL_CRACK));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_DETAIL_CRACK, _T("균열유발용 줄눈 상세"));
			*pDomP << Dom;
		}
		
		if(m_pStd->IsDrawByBlock(MPE_TITLE_DETAIL_CRACK))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("균열유발용 줄눈 상세"), CDPoint(0,0), 0, 0);
			Dom.Move(Dom.Always(850)+rcCrack.Width()/2.0, Dom.Always(450)+rcCrack.Height()+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_TITLE_DETAIL_CRACK));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_DETAIL_CRACK, _T("균열유발용 줄눈 상세 제목"));
			*pDomP << Dom;
		}
	}


	// 시공 순서도 //////////////////////////////////////////////////////////////////////
	if(dOptDraw & GEN_ORDER)
	{
		if(m_pStd->IsDrawByBlock(MPE_ORDER))
		{
			pPageMng->SetProgressData(_T("시공 순서도 출력중..."), 58);
			AddOrder(&Dom);
			Dom.SetCalcExtRect();
// 			rcOrder	= Dom.GetExtRect();

			Dom.Move(Dom.Always(1000), Dom.Always(80)); 
			Dom.SetCalcExtRect();
			rcOrder	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_ORDER));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_ORDER, _T("시공 순서도"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_TITLE_ORDER))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("콘크리트 타설순서도"), CDPoint(0,0), 0, pData->m_dScaleGenOrder);
			Dom.Move(Dom.Always(1000)+rcOrder.Width()/2.0, Dom.Always(80)+rcOrder.Height()+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_TITLE_ORDER));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_ORDER, _T("시공 순서도 제목"));
			*pDomP << Dom;
		}
	}

	// 종단면도 //////////////////////////////////////////////////////////////////////
	if(dOptDraw & GEN_LONGI)
	{
		if(m_pStd->IsDrawByBlock(MPE_LONGI))
		{
			pPageMng->SetProgressData(_T("종단면도 출력중..."), 71);
			AddLongi(&Dom);
			Dom.SetCalcExtRect();
//			rcLongi	= Dom.GetExtRect();

			Dom.Move(Dom.Always(300), Dom.Always(70)); 
			Dom.SetCalcExtRect();
			rcLongi	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_LONGI));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_LONGI, _T("종단면도"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_TITLE_LONGI))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("종단면도"), CDPoint(0,0), 0, dScale);
			Dom.Move(Dom.Always(330)+rcLongi.Width()/2.0, Dom.Always(70)+rcLongi.Height()+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_TITLE_LONGI));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_LONGI, _T("종단면도 제목"));
			*pDomP << Dom;
		}
	}

	// 상세 "B" ////////////////////////////////////////////////////////////////////// 브라켓 상세
	if(dOptDraw & GEN_DETAIL_B && (pBri->IsBracket(TRUE) || pBri->IsBracket(FALSE)))
	{
		if(m_pStd->IsDrawByBlock(MPE_DETAIL_B))
		{
			pPageMng->SetProgressData(_T("상세 \"B\" 출력중..."), 83);
			CDPoint xyTemp(0, 0);
			double dTemp = 0;
			AddDetailB(&Dom, xyTemp, dTemp);
			Dom.SetCalcExtRect();
// 			rcDetailB = Dom.GetExtRect();
			
			Dom.Move(Dom.Always(950), Dom.Always(300));
			Dom.SetCalcExtRect();
			rcDetailB = Dom.GetExtRect();
			
			Dom.Move(pMove->GetPoint(MPE_DETAIL_B));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_DETAIL_B, _T("상세 \"B\""));
			*pDomP << Dom;
		}
		
		if(m_pStd->IsDrawByBlock(MPE_TITLE_DETAIL_B))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("상세 \"B\""), CDPoint(0,0), 0, pData->m_dScaleGenDetailB);
			Dom.Move(Dom.Always(950)+rcDetailB.Width()/2.0, Dom.Always(300)+rcDetailB.Height()+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_TITLE_DETAIL_B));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_DETAIL_B, _T("상세 \"B\" 제목"));
			*pDomP << Dom;
		}
	}

	// 신축이음 상세 /////////////////////////////////////////////////////////////
	if(dOptDraw & GEN_DETAIL_FXJOINT && (pBri->IsBlockOut(TRUE) || pBri->IsBlockOut(FALSE)))
	{
		pPageMng->SetProgressData(_T("신축이음 상세 출력중..."), 86);
		double dScaleDetailFx	= pData->m_dScaleGenDetailB;
		for(long stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			CString str	= _T("");
			str.Format("상세 \"%s\"", GetStrSectionNumberFlexibleJoint(bStt));


			if(m_pStd->IsDrawByBlock(bStt ? MPE_DETAIL_FXJOINT_S : MPE_DETAIL_FXJOINT_E))
			{
				AddDetailFlexibleJoint(&Dom, stt == iSTT, "D");
				Dom.SetCalcExtRect();
				rcDetailFx[stt] = Dom.GetExtRect();
				
				Dom.Move(Dom.Always(bStt ? 850 : 950), Dom.Always(450));
				Dom.SetCalcExtRect();
				rcDetailFx[stt] = Dom.GetExtRect();
				
				Dom.Move(pMove->GetPoint(bStt ? MPE_DETAIL_FXJOINT_S : MPE_DETAIL_FXJOINT_E));
				pStd->ChangeSeperateLayer(&Dom);
				pMove->AddMPE(&Dom, Dom.GetExtRect(), bStt ? MPE_DETAIL_FXJOINT_S : MPE_DETAIL_FXJOINT_E, str);
				*pDomP << Dom;
			}
			
			if(m_pStd->IsDrawByBlock(bStt ? MPE_TITLE_DETAIL_FXJOINT_S : MPE_TITLE_DETAIL_FXJOINT_E))
			{
				pStd->AddTitleOnDom(&Dom, m_nBridge, str, CDPoint(0,0), 0, dScaleDetailFx);
				Dom.Move(Dom.Always(bStt ? 850 : 950)+rcDetailFx[stt].Width()/2.0, Dom.Always(450)+rcDetailFx[stt].Height()+GAP_TITLE);
				Dom.Move(pMove->GetPoint(bStt ? MPE_TITLE_DETAIL_FXJOINT_S : MPE_TITLE_DETAIL_FXJOINT_E));
				pStd->ChangeSeperateLayer(&Dom);
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom, Dom.GetExtRect(), bStt ? MPE_TITLE_DETAIL_FXJOINT_S : MPE_TITLE_DETAIL_FXJOINT_E, str+" 제목");
				*pDomP << Dom;
			}
		}

	}

	// NOTE //////////////////////////////////////////////////////////////////////
	if(dOptDraw & GEN_NOTE)
	{
		if(m_pStd->IsDrawByBlock(MPE_NOTE))
		{
			pPageMng->SetProgressData(_T("Note 출력중..."), 92);

			AddNote(&Dom);
			Dom.SetCalcExtRect();
// 			rcNote = Dom.GetExtRect();
			
			Dom.Move(Dom.Always(850), Dom.Always(80));
			Dom.SetCalcExtRect();
			rcNote = Dom.GetExtRect();
			
			Dom.Move(pMove->GetPoint(MPE_NOTE));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_NOTE, _T("NOTE"));
			*pDomP << Dom;
		}
		
		if(m_pStd->IsDrawByBlock(MPE_TITLE_NOTE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("NOTE"), CDPoint(0,0), -1, 0);
			Dom.Move(Dom.Always(850)+rcNote.Width()/2.0, Dom.Always(80)+rcNote.Height()+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_TITLE_NOTE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_NOTE, _T("NOTE 제목"));
			*pDomP << Dom;
		}
	}
}

// 일반도(2)
void CDrawingGeneral::AddPageGeneral2(CDomyun *pDomP, CDRect rcDogak, int nCategoryPage)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	double dScale = GetScale();
	DWORD dOptDraw	= pData->m_dwDrawGen;

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(GetScale());

	double dSpaceVer = Dom.Always(60);	// 객체 세로 간격

	CDRect rcFront(0, 0, 0, 0);			// 정면도 영역
	CDRect rcFootPlane(0, 0, 0, 0);		// 기초 평면도 영역
	double dDomX = 0;
	CString str	= _T(""), strSub = _T("");

	int nCountJijum = pBri->GetCountJijum();
	int nJ = 0;
	int nSttJijum = (nCategoryPage-2)*2;	// (nCategoryPage-(정면도가 시작되는 페이지))*(페이지당 정면도 수)
	int nEndJijum = nSttJijum>nCountJijum-2 ?  nCountJijum : nSttJijum+2;	// (시작 지점 인덱스) + (페이지당 정면도 수)
	int i=0; for(i=nSttJijum; i<nEndJijum; i++)
	{
		if(i==0)
			nJ = i;
		else if(i==1)
			nJ = nCountJijum-1;
		else
			nJ = i - 1;
		if(nJ==0)
			strSub = _T("(A1)");
		else if(nJ==nCountJijum-1)
			strSub = _T("(A2)");
		else
			strSub.Format(_T("(P%d)"), nJ);

		// 아치교의 접합부분이면 출력하지 않음
		// 코드가 복잡해 짐을 방지하기 위해서 구조도처럼 페이지 정리는 하지 않음
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		{
			if(pBri->GetJijumConnectRibAndSlab() == nJ) continue;
		}



		// 정면도 //////////////////////////////////////////////////////////////////////
		if(dOptDraw & GEN_FRONT)
		{
			str = _T("정면도") + strSub;
			if(m_pStd->IsDrawByBlock(MPE_FRONT+i*10))
			{
				pPageMng->SetProgressData(str+_T(" 출력중..."), 10+20*(i%2));
				AddFront(&Dom, nJ);
				if(nJ > 0 && nJ < pBri->m_nQtyJigan)
				{
					CWallApp *pWall	= pBri->GetInWall(nJ-1);
					if(pWall)
					{
						if(!pWall->IsVertWall())
						{
							pStd->DrawNoteBottomOfDomyun(&Dom, "()안의 세로치수는 벽체 경사를 고려하지 않음 수직거리임.");
						}
					}
				}
				Dom.SetCalcExtRect();
				rcFront	= Dom.GetExtRect();

				if(i%2==0)
					dDomX = rcDogak.left-rcFront.left+GAP_LEFT*2;
				else
					dDomX = rcDogak.right-rcFront.right-GAP_LEFT*2;
				Dom.Move(dDomX, rcDogak.bottom-rcFront.bottom-dSpaceVer*2);
				Dom.SetCalcExtRect();
				rcFront	= Dom.GetExtRect();

				Dom.Move(pMove->GetPoint(MPE_FRONT+i*10));
				pStd->ChangeSeperateLayer(&Dom);
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_FRONT+i*10, str);
				*pDomP << Dom;
			}

			if(m_pStd->IsDrawByBlock(MPE_TITLE_FRONT+i*10))
			{
				pStd->AddTitleOnDom(&Dom, m_nBridge, str, CDPoint(0,0), 0, dScale);
				Dom.Move(rcFront.left+rcFront.Width()/2.0, rcFront.bottom+GAP_TITLE);
				Dom.Move(pMove->GetPoint(MPE_TITLE_FRONT+i*10));
				pStd->ChangeSeperateLayer(&Dom);
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_FRONT+i*10, str+_T(" 제목"));
				*pDomP << Dom;
			}
		}

		if(pBri->IsBoxType() && (pBri->m_nTypeLower != LOWERSLAB_TYPE_PILE || nJ > 0))
		{
			// BOX타입은 기초 평면도를 그리지 않는다.
			// 말뚝기초라면 말뚝평면도만 하나 그려주자.
			continue;
		}

		// 기초 평면도 //////////////////////////////////////////////////////////////////////
		if(dOptDraw & GEN_FOOT_PLANE)
		{
			CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(nJ);
			if(pFooting)
			{
				if(pBri->IsBoxType())
					str = _T("하부슬래브 평면도");
				else
					str = _T("기초 평면도") + strSub;
				if(m_pStd->IsDrawByBlock(MPE_FOOT_PLANE+i*10))
				{
					pPageMng->SetProgressData(str+_T(" 출력중..."), 20+20*(i%2));

					if(pBri->IsBoxType())
						AddLowerSlabPlane(&Dom);
					else
						AddFootPlane(&Dom, nJ);

					Dom.SetCalcExtRect();
					rcFootPlane	= Dom.GetExtRect();

					Dom.Move(dDomX, rcFront.top-rcFootPlane.bottom-dSpaceVer);
					Dom.SetCalcExtRect();
					rcFootPlane	= Dom.GetExtRect();

					Dom.Move(pMove->GetPoint(MPE_FOOT_PLANE+i*10));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_FOOT_PLANE+i*10, str);
					*pDomP << Dom;
				}

				if(m_pStd->IsDrawByBlock(MPE_TITLE_FOOT_PLANE+i*10))
				{
					pStd->AddTitleOnDom(&Dom, m_nBridge, str, CDPoint(0,0), 0, dScale);
					Dom.Move(rcFootPlane.left+rcFootPlane.Width()/2.0, rcFootPlane.bottom+GAP_TITLE);
					Dom.Move(pMove->GetPoint(MPE_TITLE_FOOT_PLANE+i*10));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_FOOT_PLANE+i*10, str+_T(" 제목"));
					*pDomP << Dom;
				}
			}
		}

		// 기초가 말뚝이고 말뚝길이가 var이면 말뚝길이 테이블 출력
		if(dOptDraw & GEN_FOOT_PLANE)
		{
			CDRect rcTable(0, 0, 0, 0);
			CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(nJ);
			if(pFooting && pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_PILE && pFooting->m_exFooting.m_Pile.IsDiffPileLen())
			{
				str = _T("말뚝길이 테이블") + strSub;
				if(m_pStd->IsDrawByBlock(MPE_PILELEN_TABLE+i*10))
				{
					pPageMng->SetProgressData(str+_T(" 출력중..."), 20+20*(i%2));
					CDrawExPile drawExPile(&pFooting->m_exFooting.m_Pile, pStd->m_pARoadOptionStd);
					drawExPile.DrawPileLengthTable(&Dom, TRUE);
					Dom.SetCalcExtRect();
					rcTable	= Dom.GetExtRect();

					Dom.Move(rcFootPlane.left - rcTable.left, rcDogak.top - rcTable.bottom - dSpaceVer);
					Dom.SetCalcExtRect();
					rcTable	= Dom.GetExtRect();

					Dom.Move(pMove->GetPoint(MPE_PILELEN_TABLE+i*10));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_PILELEN_TABLE+i*10, str);
					*pDomP << Dom;
				}

				if(m_pStd->IsDrawByBlock(MPE_TITLE_PILELEN_TABLE+i*10))
				{
					pStd->AddTitleOnDom(&Dom, m_nBridge, str, CDPoint(0,0), 0, dScale);
					Dom.Move(rcTable.left+rcTable.Width()/2.0, rcTable.bottom+GAP_TITLE);
					Dom.Move(pMove->GetPoint(MPE_TITLE_PILELEN_TABLE+i*10));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_PILELEN_TABLE+i*10, str+_T(" 제목"));
					*pDomP << Dom;
				}
			}
		}


		// 벽체 평면도 살짝 그려줌
		CDRect rcWallPlane	= CDRect(0, 0, 0, 0);
		if(nJ > 0 && nJ < pBri->m_nQtyJigan && dOptDraw & GEN_WALL_PLANE)
		{
			CWallApp *pWall	= pBri->GetInWall(nJ-1);
			if(pWall)
			{
				str = _T(pWall->m_bTopIsColumn ? "기둥 평면도" : "벽체 평면도") + strSub;
				if(m_pStd->IsDrawByBlock(MPE_WALL_PLANE+i*10))
				{
					pPageMng->SetProgressData(str+_T(" 출력중..."), 20+20*(i%2));
					if(pWall->m_bTopIsColumn)
					{
						CTwinVectorArray tvArrTmp;
						DBStd.m_pDrawColumn->DrawPlan(&Dom, &pWall->m_Column, &tvArrTmp, TRUE, FALSE);
						DBStd.m_pDrawColumn->DimPlan(&Dom, &pWall->m_Column, 0, FALSE, FALSE);
					}
					else
					{
						DBStd.m_pDrawWall->DrawPlan(&Dom, pWall, TRUE, FALSE);
						//DBStd.m_pDrawWall->DimLeftPlane(&Dom, pWall, FALSE);
						DBStd.m_pDrawWall->DimRightPlane(&Dom, pWall, 0, FALSE, TRUE);
											
						CDPoint vAng	= pBri->GetXyDirRotateDomByBridgeSttEndXy();
						Dom.Rotate(CDPoint(0, 0), vAng.RotateInv90());
					}
					Dom.SetCalcExtRect();
					rcWallPlane	= Dom.GetExtRect();

					Dom.Move(rcFootPlane.CenterPoint().x-rcWallPlane.CenterPoint().x, rcFootPlane.top-rcWallPlane.bottom-dSpaceVer);
					Dom.SetCalcExtRect();
					rcWallPlane	= Dom.GetExtRect();

					Dom.Move(pMove->GetPoint(MPE_WALL_PLANE+i*10));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_WALL_PLANE+i*10, str);
					*pDomP << Dom;
				}

				if(m_pStd->IsDrawByBlock(MPE_TITLE_WALL_PLANE+i*10))
				{
					pStd->AddTitleOnDom(&Dom, m_nBridge, str, CDPoint(0,0), 0, dScale);
					Dom.Move(rcWallPlane.left+rcWallPlane.Width()/2.0, rcWallPlane.bottom+GAP_TITLE);
					Dom.Move(pMove->GetPoint(MPE_TITLE_WALL_PLANE+i*10));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_WALL_PLANE+i*10, str+_T(" 제목"));
					*pDomP << Dom;
				}
			}
		}
	}
}

// 일반도(3)
void CDrawingGeneral::AddPageGeneral3(CDomyun *pDomP, CDRect rcDogak)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	double dScaleWing = pData->m_dScaleGenWingWall;
	DWORD dOptDraw	= pData->m_dwDrawGen;

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(GetScale());

	double dSpaceHor = Dom.Always(30);	// 객체 가로 간격
	double dSpaceVer = Dom.Always(60);	// 객체 세로 간격

	CDRect rcBase(0, 0, 0, 0);
	CDRect rcSect(0, 0, 0, 0);	// 단면도 영역
	CDRect rcPlane(0, 0, 0, 0);	// 평면도 영역
	CDRect rcFront[4];			// 정면도 영역
	int i=0; for(i=0; i<4; i++)
		rcFront[i] = CDRect(0, 0, 0, 0);

	CWingWall* pWing = NULL;
	CString str = _T(""), strTitle = _T("");
	char ch = 64;
	double dDomX = 0;

	//long nTypeJongdanForRebarOrg = pBri->m_nTypeJongdanForRebar;
	//pBri->m_nTypeJongdanForRebar = 2;
	// 시점 좌측 날개벽 //////////////////////////////////////////////////////////////////////
	if(dOptDraw & GEN_WING_BL)
	{
		pWing = pBri->GetWingWall(TRUE, TRUE);
		if(pWing->m_bExist)
		{
			//pBri->SyncBridge(1, TRUE);
			pBri->GetTvVertSection_Std(1, TRUE, FALSE, FALSE, 2);

			// 정면도
			if(pData->m_bDrawGenWingWall[0][0])
			{
				if(m_pStd->IsDrawByBlock(MPE_WING_FRONT_BL))
				{
					str = ch;
					pPageMng->SetProgressData(_T("시점 좌측 날개벽 출력중..."), 8);
					AddWingWallFront(&Dom, TRUE, TRUE, str);
					Dom.SetCalcExtRect();
					rcBase = Dom.GetExtRect();

					dDomX = rcDogak.left-rcBase.left+GAP_LEFT*5;
					Dom.Move(dDomX, rcDogak.bottom-rcBase.bottom-GAP_TOP); 
					Dom.SetCalcExtRect();
					rcFront[0] = Dom.GetExtRect();

					Dom.Move(pMove->GetPoint(MPE_WING_FRONT_BL));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_WING_FRONT_BL, _T("시점 좌측 날개벽"));
					*pDomP << Dom;
				}

				if(m_pStd->IsDrawByBlock(MPE_TITLE_WING_FRONT_BL))
				{
					pStd->AddTitleOnDom(&Dom, m_nBridge, _T("시점 좌측 날개벽"), CDPoint(0,0), 0, dScaleWing);
					Dom.Move(rcFront[0].left+rcFront[0].Width()/2.0, rcFront[0].bottom+GAP_TITLE);
					Dom.Move(pMove->GetPoint(MPE_TITLE_WING_FRONT_BL));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_WING_FRONT_BL, _T("시점 좌측 날개벽 제목"));
					*pDomP << Dom;
				}
			}

			// 단면도
			if(pData->m_bDrawGenWingWall[0][1])
			{
				if(m_pStd->IsDrawByBlock(MPE_WING_SECT_BL))
				{
					str = ++ch;
					strTitle = _T("단면 ") + str + _T("-") + str;
					pPageMng->SetProgressData(strTitle+_T(" 출력중..."), 16);
					AddWingWallSection(&Dom, TRUE, TRUE);
					Dom.SetCalcExtRect();
					rcSect = Dom.GetExtRect();

					Dom.Move(rcFront[0].left-rcSect.right-dSpaceHor*2, rcDogak.bottom-rcBase.bottom-GAP_TOP); 
					Dom.SetCalcExtRect();
					rcSect = Dom.GetExtRect();

					Dom.Move(pMove->GetPoint(MPE_WING_SECT_BL));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_WING_SECT_BL, strTitle);
					*pDomP << Dom;
				}

				if(m_pStd->IsDrawByBlock(MPE_TITLE_WING_SECT_BL))
				{
					pStd->AddTitleOnDom(&Dom, m_nBridge, strTitle, CDPoint(0,0), 0, dScaleWing);
					Dom.Move(rcSect.left+rcSect.Width()/2.0, rcSect.bottom+GAP_TITLE);
					Dom.Move(pMove->GetPoint(MPE_TITLE_WING_SECT_BL));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_WING_SECT_BL, strTitle+_T(" 제목"));
					*pDomP << Dom;
				}
			}

			// 평면도
			if(pData->m_bDrawGenWingWall[0][2])
			{
				if(m_pStd->IsDrawByBlock(MPE_WING_PLANE_BL))
				{
					str = ++ch;
					strTitle = _T("단면 ") + str + _T("-") + str;
					pPageMng->SetProgressData(strTitle+_T(" 출력중..."), 24);
					AddWingWallPlane(&Dom, TRUE, TRUE);
					Dom.SetCalcExtRect();
					rcPlane = Dom.GetExtRect();

					Dom.Move(dDomX, rcFront[0].top-rcPlane.bottom-dSpaceVer/3);
					Dom.SetCalcExtRect();
					rcPlane = Dom.GetExtRect();

					Dom.Move(pMove->GetPoint(MPE_WING_PLANE_BL));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_WING_PLANE_BL, strTitle);
					*pDomP << Dom;
				}

				if(m_pStd->IsDrawByBlock(MPE_TITLE_WING_PLANE_BL))
				{
					pStd->AddTitleOnDom(&Dom, m_nBridge, strTitle, CDPoint(0,0), 0, dScaleWing);
					Dom.Move(rcPlane.left+rcPlane.Width()/2.0, rcPlane.bottom+GAP_TITLE);
					Dom.Move(pMove->GetPoint(MPE_TITLE_WING_PLANE_BL));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_WING_PLANE_BL, strTitle+_T(" 제목"));
					*pDomP << Dom;
				}
			}

			pBri->GetTvVertSection_Std(1, TRUE, FALSE, TRUE);
		}
	}

	// 종점 우측 날개벽 //////////////////////////////////////////////////////////////////////
	if(dOptDraw & GEN_WING_ER)
	{
		pWing = pBri->GetWingWall(FALSE, FALSE);
		if(pWing->m_bExist)
		{
			//pBri->SyncBridge(1, TRUE);
			pBri->GetTvVertSection_Std(1, TRUE, FALSE, FALSE, 2);

			// 정면도
			if(pData->m_bDrawGenWingWall[3][0])
			{
				if(m_pStd->IsDrawByBlock(MPE_WING_FRONT_ER))
				{
					str = ch;
					pPageMng->SetProgressData(_T("종점 우측 날개벽 출력중..."), 32);
					AddWingWallFront(&Dom, FALSE, FALSE, str);
					Dom.SetCalcExtRect();
					rcBase = Dom.GetExtRect();

					dDomX = rcFront[0].right-rcBase.left+dSpaceHor*3;
					Dom.Move(dDomX, rcDogak.bottom-rcBase.bottom-GAP_TOP); 
					Dom.SetCalcExtRect();
					rcFront[1] = Dom.GetExtRect();

					Dom.Move(pMove->GetPoint(MPE_WING_FRONT_ER));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_WING_FRONT_ER, _T("종점 우측 날개벽"));
					*pDomP << Dom;
				}

				if(m_pStd->IsDrawByBlock(MPE_TITLE_WING_FRONT_ER))
				{
					pStd->AddTitleOnDom(&Dom, m_nBridge, _T("종점 우측 날개벽"), CDPoint(0,0), 0, dScaleWing);
					Dom.Move(rcFront[1].left+rcFront[1].Width()/2.0, rcFront[1].bottom+GAP_TITLE);
					Dom.Move(pMove->GetPoint(MPE_TITLE_WING_FRONT_ER));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_WING_FRONT_ER, _T("종점 우측 날개벽 제목"));
					*pDomP << Dom;
				}
			}

			// 단면도
			if(pData->m_bDrawGenWingWall[3][1])
			{
				if(m_pStd->IsDrawByBlock(MPE_WING_SECT_ER))
				{
					str = ++ch;
					strTitle = _T("단면 ") + str + _T("-") + str;
					pPageMng->SetProgressData(strTitle+_T(" 출력중..."), 40);
					AddWingWallSection(&Dom, FALSE, FALSE);
					Dom.SetCalcExtRect();
					rcSect = Dom.GetExtRect();

					Dom.Move(rcFront[1].right-rcSect.left+dSpaceHor*2, rcDogak.bottom-rcBase.bottom-GAP_TOP); 
					Dom.SetCalcExtRect();
					rcSect = Dom.GetExtRect();

					Dom.Move(pMove->GetPoint(MPE_WING_SECT_ER));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_WING_SECT_ER, strTitle);
					*pDomP << Dom;
				}

				if(m_pStd->IsDrawByBlock(MPE_TITLE_WING_SECT_ER))
				{
					pStd->AddTitleOnDom(&Dom, m_nBridge, strTitle, CDPoint(0,0), 0, dScaleWing);
					Dom.Move(rcSect.left+rcSect.Width()/2.0, rcSect.bottom+GAP_TITLE);
					Dom.Move(pMove->GetPoint(MPE_TITLE_WING_SECT_ER));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_WING_SECT_ER, strTitle+_T(" 제목"));
					*pDomP << Dom;
				}
			}

			// 평면도
			if(pData->m_bDrawGenWingWall[3][2])
			{
				if(m_pStd->IsDrawByBlock(MPE_WING_PLANE_ER))
				{
					str = ++ch;
					strTitle = _T("단면 ") + str + _T("-") + str;
					pPageMng->SetProgressData(strTitle+_T(" 출력중..."), 48);
					AddWingWallPlane(&Dom, FALSE, FALSE);
					Dom.SetCalcExtRect();
					rcPlane = Dom.GetExtRect();

					Dom.Move(dDomX, rcFront[1].top-rcPlane.bottom-dSpaceVer/3);
					Dom.SetCalcExtRect();
					rcPlane = Dom.GetExtRect();

					Dom.Move(pMove->GetPoint(MPE_WING_PLANE_ER));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_WING_PLANE_ER, strTitle);
					*pDomP << Dom;
				}

				if(m_pStd->IsDrawByBlock(MPE_TITLE_WING_PLANE_ER))
				{
					pStd->AddTitleOnDom(&Dom, m_nBridge, strTitle, CDPoint(0,0), 0, dScaleWing);
					Dom.Move(rcPlane.left+rcPlane.Width()/2.0, rcPlane.bottom+GAP_TITLE);
					Dom.Move(pMove->GetPoint(MPE_TITLE_WING_PLANE_ER));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_WING_PLANE_ER, strTitle+_T(" 제목"));
					*pDomP << Dom;
				}
			}

			pBri->GetTvVertSection_Std(1, TRUE, FALSE, TRUE);
		}
	}

	// 시점 우측 날개벽 //////////////////////////////////////////////////////////////////////
	if(dOptDraw & GEN_WING_BR)
	{
		pWing = pBri->GetWingWall(TRUE, FALSE);
		if(pWing->m_bExist)
		{
			//pBri->SyncBridge(-1, TRUE);
			pBri->GetTvVertSection_Std(-1, TRUE, FALSE, FALSE, 2);

			// 정면도
			if(pData->m_bDrawGenWingWall[1][0])
			{
				if(m_pStd->IsDrawByBlock(MPE_WING_FRONT_BR))
				{
					str = ch;
					pPageMng->SetProgressData(_T("시점 우측 날개벽 출력중..."), 56);
					AddWingWallFront(&Dom, TRUE, FALSE, str);
					Dom.SetCalcExtRect();
					rcBase = Dom.GetExtRect();

					dDomX = rcDogak.left-rcBase.left+GAP_LEFT*5;
					Dom.Move(dDomX, rcDogak.bottom-rcDogak.Height()/2-rcBase.bottom-dSpaceVer); 
					Dom.SetCalcExtRect();
					rcFront[2] = Dom.GetExtRect();

					Dom.Move(pMove->GetPoint(MPE_WING_FRONT_BR));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_WING_FRONT_BR, _T("시점 우측 날개벽"));
					*pDomP << Dom;
				}

				if(m_pStd->IsDrawByBlock(MPE_TITLE_WING_FRONT_BR))
				{
					pStd->AddTitleOnDom(&Dom, m_nBridge, _T("시점 우측 날개벽"), CDPoint(0,0), 0, dScaleWing);
					Dom.Move(rcFront[2].left+rcFront[2].Width()/2.0, rcFront[2].bottom+GAP_TITLE);
					Dom.Move(pMove->GetPoint(MPE_TITLE_WING_FRONT_BR));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_WING_FRONT_BR, _T("시점 우측 날개벽 제목"));
					*pDomP << Dom;
				}
			}

			// 단면도
			if(pData->m_bDrawGenWingWall[1][1])
			{
				if(m_pStd->IsDrawByBlock(MPE_WING_SECT_BR))
				{
					str = ++ch;
					strTitle = _T("단면 ") + str + _T("-") + str;
					pPageMng->SetProgressData(strTitle+_T(" 출력중..."), 64);
					AddWingWallSection(&Dom, TRUE, FALSE);
					Dom.SetCalcExtRect();
					rcSect = Dom.GetExtRect();

					Dom.Move(rcFront[2].left-rcSect.right-dSpaceHor*2, rcDogak.bottom-rcDogak.Height()/2-rcBase.bottom-dSpaceVer); 
					Dom.SetCalcExtRect();
					rcSect = Dom.GetExtRect();

					Dom.Move(pMove->GetPoint(MPE_WING_SECT_BR));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_WING_SECT_BR, strTitle);
					*pDomP << Dom;
				}

				if(m_pStd->IsDrawByBlock(MPE_TITLE_WING_SECT_BR))
				{
					pStd->AddTitleOnDom(&Dom, m_nBridge, strTitle, CDPoint(0,0), 0, dScaleWing);
					Dom.Move(rcSect.left+rcSect.Width()/2.0, rcSect.bottom+GAP_TITLE);
					Dom.Move(pMove->GetPoint(MPE_TITLE_WING_SECT_BR));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_WING_SECT_BR, strTitle+_T(" 제목"));
					*pDomP << Dom;
				}
			}

			// 평면도
			if(pData->m_bDrawGenWingWall[1][2])
			{
				if(m_pStd->IsDrawByBlock(MPE_WING_PLANE_BR))
				{
					str = ++ch;
					strTitle = _T("단면 ") + str + _T("-") + str;
					pPageMng->SetProgressData(strTitle+_T(" 출력중..."), 72);
					AddWingWallPlane(&Dom, TRUE, FALSE);
					Dom.SetCalcExtRect();
					rcPlane = Dom.GetExtRect();

					Dom.Move(dDomX, rcFront[2].top-rcPlane.bottom-dSpaceVer/3);
					Dom.SetCalcExtRect();
					rcPlane = Dom.GetExtRect();

					Dom.Move(pMove->GetPoint(MPE_WING_PLANE_BR));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_WING_PLANE_BR, strTitle);
					*pDomP << Dom;
				}

				if(m_pStd->IsDrawByBlock(MPE_TITLE_WING_PLANE_BR))
				{
					pStd->AddTitleOnDom(&Dom, m_nBridge, strTitle, CDPoint(0,0), 0, dScaleWing);
					Dom.Move(rcPlane.left+rcPlane.Width()/2.0, rcPlane.bottom+GAP_TITLE);
					Dom.Move(pMove->GetPoint(MPE_TITLE_WING_PLANE_BR));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_WING_PLANE_BR, strTitle+_T(" 제목"));
					*pDomP << Dom;
				}
			}

			pBri->GetTvVertSection_Std(-1, TRUE, FALSE, TRUE);
		}
	}

	// 종점 좌측 날개벽 //////////////////////////////////////////////////////////////////////
	if(dOptDraw & GEN_WING_EL)
	{
		pWing = pBri->GetWingWall(FALSE, TRUE);
		if(pWing->m_bExist)
		{
			//pBri->SyncBridge(-1, TRUE);
			pBri->GetTvVertSection_Std(-1, TRUE, FALSE, FALSE, 2);

			// 정면도
			if(pData->m_bDrawGenWingWall[2][0])
			{
				if(m_pStd->IsDrawByBlock(MPE_WING_FRONT_EL))
				{
					str = ch;
					pPageMng->SetProgressData(_T("종점 좌측 날개벽 출력중..."), 80);
					AddWingWallFront(&Dom, FALSE, TRUE, str);
					Dom.SetCalcExtRect();
					rcFront[3] = Dom.GetExtRect();

					dDomX = rcFront[2].right-rcFront[3].left+dSpaceHor*3;
					Dom.Move(dDomX, rcDogak.bottom-rcDogak.Height()/2-rcBase.bottom-dSpaceVer); 
					Dom.SetCalcExtRect();
					rcFront[3] = Dom.GetExtRect();

					Dom.Move(pMove->GetPoint(MPE_WING_FRONT_EL));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_WING_FRONT_EL, _T("종점 좌측 날개벽"));
					*pDomP << Dom;
				}

				if(m_pStd->IsDrawByBlock(MPE_TITLE_WING_FRONT_EL))
				{
					pStd->AddTitleOnDom(&Dom, m_nBridge, _T("종점 좌측 날개벽"), CDPoint(0,0), 0, dScaleWing);
					Dom.Move(rcFront[3].left+rcFront[3].Width()/2.0, rcFront[3].bottom+GAP_TITLE);
					Dom.Move(pMove->GetPoint(MPE_TITLE_WING_FRONT_EL));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_WING_FRONT_EL, _T("종점 좌측 날개벽 제목"));
					*pDomP << Dom;
				}
			}

			// 단면도
			if(pData->m_bDrawGenWingWall[2][1])
			{
				if(m_pStd->IsDrawByBlock(MPE_WING_SECT_EL))
				{
					str = ++ch;
					strTitle = _T("단면 ") + str + _T("-") + str;
					pPageMng->SetProgressData(strTitle+_T(" 출력중..."), 88);
					AddWingWallSection(&Dom, FALSE, TRUE);
					Dom.SetCalcExtRect();
					rcSect = Dom.GetExtRect();

					Dom.Move(rcFront[3].right-rcSect.left+dSpaceHor*2, rcDogak.bottom-rcDogak.Height()/2-rcBase.bottom-dSpaceVer); 
					Dom.SetCalcExtRect();
					rcSect = Dom.GetExtRect();

					Dom.Move(pMove->GetPoint(MPE_WING_SECT_EL));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_WING_SECT_EL, strTitle);
					*pDomP << Dom;
				}

				if(m_pStd->IsDrawByBlock(MPE_TITLE_WING_SECT_EL))
				{
					pStd->AddTitleOnDom(&Dom, m_nBridge, strTitle, CDPoint(0,0), 0, dScaleWing);
					Dom.Move(rcSect.left+rcSect.Width()/2.0, rcSect.bottom+GAP_TITLE);
					Dom.Move(pMove->GetPoint(MPE_TITLE_WING_SECT_EL));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_WING_SECT_EL, strTitle+_T(" 제목"));
					*pDomP << Dom;
				}
			}

			// 평면도
			if(pData->m_bDrawGenWingWall[2][2])
			{
				if(m_pStd->IsDrawByBlock(MPE_WING_PLANE_EL))
				{
					str = ++ch;
					strTitle = _T("단면 ") + str + _T("-") + str;
					pPageMng->SetProgressData(strTitle+_T(" 출력중..."), 96);
					AddWingWallPlane(&Dom, FALSE, TRUE);
					Dom.SetCalcExtRect();
					rcPlane = Dom.GetExtRect();

					Dom.Move(dDomX, rcFront[3].top-rcPlane.bottom-dSpaceVer/3);
					Dom.SetCalcExtRect();
					rcPlane = Dom.GetExtRect();

					Dom.Move(pMove->GetPoint(MPE_WING_PLANE_EL));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_WING_PLANE_EL, strTitle);
					*pDomP << Dom;
				}

				if(m_pStd->IsDrawByBlock(MPE_TITLE_WING_PLANE_EL))
				{
					pStd->AddTitleOnDom(&Dom, m_nBridge, strTitle, CDPoint(0,0), 0, dScaleWing);
					Dom.Move(rcPlane.left+rcPlane.Width()/2.0, rcPlane.bottom+GAP_TITLE);
					Dom.Move(pMove->GetPoint(MPE_TITLE_WING_PLANE_EL));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_WING_PLANE_EL, strTitle+_T(" 제목"));
					*pDomP << Dom;
				}
			}

			pBri->GetTvVertSection_Std(-1, TRUE, FALSE, TRUE);
		}
	}
	//pBri->m_nTypeJongdanForRebar = nTypeJongdanForRebarOrg;
}

void CDrawingGeneral::AddPlane(CDomyun* pDomP)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();
	pBri->SyncBridge_DesignData();

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	double dScale = GetScale();

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScale);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	//pBri->SyncBridge(0, FALSE, TRUE, TRUE);
	pBri->GetTvVertSection_Std(0, FALSE, FALSE);
	pBri->SyncBridgeAps(0, TRUE);

	// 교량 평면

	DBStd.m_pDrawPlane->DrawPlaneGen(&Dom, 5000, FALSE,TRUE,FALSE,pData->m_bDrawGenCoordinateXY,pData->m_bDrawGenCoordinateXY,FALSE);
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		DBStd.m_pDrawPlane->DimPlaneArchRibWidth(&Dom);

	DBStd.m_pDrawPlane->DimPlaneJijumLength(&Dom, FALSE, TRUE);
	DBStd.m_pDrawPlane->DimPlaneJijumAngle(&Dom, FALSE);
	DBStd.m_pDrawPlane->MarkGenCrossSlope(&Dom);
	DBStd.m_pDrawPlane->MakrPlaneConcFck(&Dom);

	// (ARCBRIDGE-3166) 평면도에 가각치수 추가.
	DBStd.m_pDrawPlane->DimPlaneJijumGagak(&Dom, FALSE);

	// 벽체 및 기둥 평면
	CTwinVectorArray tvArrCen;
	pBri->GetTvCenterPlane_UpperSlab(tvArrCen, TRUE, TRUE, 1000, FALSE);

	int nCountInWall = pBri->GetCountInWall();
	int i=0; for(i=0; i<nCountInWall; i++)
	{
		CWallApp *pWall = pBri->GetInWall(i);
		if(pBri->GetJijumConnectRibAndSlab() == i+1) 
		{
			// 아치리브 폭 치수 넣는다
			DBStd.m_pDrawPlane->DimPlaneArchRibWidth(&Dom);
			continue;
		}
		if(pWall && pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB)
		{
			for(long hidden = 0; hidden < 2; hidden++)
			{
				BOOL bHidden	= hidden == 0;

				DBStd.m_pDrawWall->DrawPlan(&Dom, pWall, FALSE, TRUE, 0, bHidden ? -1 : 1, tvArrCen);
				if(pWall->m_bTopIsColumn)
				{
					CTwinVectorArray tvArrReturnColumn;
					DBStd.m_pDrawColumn->DrawPlan(&Dom, &pWall->m_Column, &tvArrReturnColumn, TRUE, TRUE, bHidden, bHidden ? -1 : 1, tvArrCen);
				}
			}
		}
	}
	
	// 날개벽 평면
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
	DBStd.m_pDrawPlane->DimPlaneWingWall(&Dom);

	// 기초 평면(점선)
	if(pBri->IsBoxType())
	{
		// Box형 일때는 확대기초 평면만 그려준다.
		CFootingApp *pFooting	= pBri->GetFooting(0);
		if(pFooting)
			DBStd.m_pDrawExFooting->DrawPlan(&Dom, &pFooting->m_exFooting, FALSE, TRUE, FALSE);
	}
	else
	{
		long nCountFooting	= pBri->GetCountFooting();
		for(long footing = 0; footing < nCountFooting; footing++)
		{
			CFootingApp *pFooting	= pBri->GetFooting(footing);
			if(!pFooting) continue;
			DBStd.m_pDrawFooting->DrawPlan(&Dom, pFooting, FALSE, FALSE, FALSE, TRUE);
			DBStd.m_pDrawExFooting->DrawPlan(&Dom, &pFooting->m_exFooting, FALSE, TRUE, FALSE);

			DBStd.m_pDrawFooting->MakrPlaneConcFck(&Dom, pFooting);
		}
	}

	// 접속 슬래브 평면
	for(int nStt=0; nStt<2; nStt++)
	{
		for(int nLeft=0; nLeft<2; nLeft++)
		{
			CApsApp* pAps = nStt==0 ? &pBri->m_apsStt[nLeft] : &pBri->m_apsEnd[nLeft];
			if(pAps->m_bIs)
			{
				DBStd.m_pDrawAps->DrawPlane(&Dom, pAps, DBStd.IsViewVectorInfo());
				DBStd.m_pDrawAps->DrawPlaneDowelBar(&Dom, pAps);

				DBStd.m_pDrawAps->DimPlaneSide(&Dom, pAps, 0, FALSE);
				DBStd.m_pDrawAps->DimPlaneTop(&Dom, pAps, 0, FALSE);
				DBStd.m_pDrawAps->DimTextPlaneDowelBar(&Dom, pAps);

				DBStd.m_pDrawAps->MakrPlaneConcFck(&Dom, pAps, pStd->m_pARcBridgeDataStd->m_nTypeUnit, TRUE);

				// 완충슬래브 평면도
				for(int nCns=0; nCns<MAX_CNS; nCns++)
				{
					CApsApp* pCns = nStt==0 ? &pBri->m_cnsStt[nLeft][nCns] : &pBri->m_cnsEnd[nLeft][nCns];
					if(pCns)
					{
						DBStd.m_pDrawAps->DrawPlane(&Dom, pCns);

						DBStd.m_pDrawAps->DimPlaneTop(&Dom, pCns, 0, FALSE);
						if(nCns==0)
							DBStd.m_pDrawAps->DimPlaneSide(&Dom, pCns, 0, FALSE);
					}
				}
			}
		}
	}
	
	// 교량 진행 방향으로 회전
	DBStd.RotateDomByBridgeSttEndXy(&Dom, TRUE);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	int nCountJijum = pBri->GetCountJijum();
	CDPoint vAngS = pBri->GetAngleJijum(0);
	vAngS = CDPoint(Round(vAngS.x, 5), Round(vAngS.y, 5));
	CDPoint vAngE = pBri->GetAngleJijum(nCountJijum-1);
	vAngE = CDPoint(Round(vAngE.x, 5), Round(vAngE.y, 5));
	if(!Compare(vAngS.GetAngleDegree(), 90.0, "=") || !Compare(vAngE.GetAngleDegree(), 90.0, "="))
	{
		Dom.SetCalcExtRect();
		CDRect rc = Dom.GetExtRect();
		Dom.SetTextAlignHorz(TA_RIGHT);
		Dom.TextOut(rc.right, rc.top, _T("* () 안의 치수는 직거리 치수임."));
	}

	Dom.SetCalcExtRect();
	CDRect rc = Dom.GetExtRect();
	Dom.Move(-rc.left, -rc.top);

	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE);
	*pDomP << Dom;
}

void CDrawingGeneral::AddCross(CDomyun* pDomP)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();
	pBri->SyncBridge();

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	double dScale = GetScale();
	BOOL bDrawRubble = FALSE; //pBri->IsBoxType() && pBri->m_dThickCobbStone > 0? TRUE : FALSE;
	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScale);

	long nJ	= 0;
	double dSta = pBri->GetStationOnJijum(0, 0);
	CDPoint vAng	= CDPoint(0, 1);
	if(nJ == 0 || nJ == pBri->m_nQtyJigan)
	{
		if(pBri->m_nTypeWallForRebarGen == 0)
		{
			dSta	= pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, TRUE);
		}
		else if(pBri->m_nTypeWallForRebarGen == 1)
		{
			dSta	= pBri->GetStationByTarget(BRIDGE_OFF_STT_STT, 0, TRUE);
		}
	}
	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DBStd.m_pDrawCrossSection->DrawCrossSection(&Dom, dSta, CDPoint(0, 1), TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, TRUE, -1, -1, TRUE, bDrawRubble);
	DBStd.m_pDrawCrossSection->DrawCrossSlabHunch(&Dom, dSta, CDPoint(0, 1), TRUE, -1);

	double nJijum = pBri->GetJijumNumberByStation(dSta);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	DBStd.m_pDrawCrossSection->DimCrossSection(&Dom, dSta, CDPoint(0, 1), FALSE);
	DBStd.m_pDrawCrossSection->MarkGenCrossArrowDirection(&Dom, dSta, CDPoint(0, 1), TRUE);
	DBStd.m_pDrawCrossSection->DimThickCrossSection(&Dom, dSta, CDPoint(0, 1));

	
	Dom.Mirror(0, TRUE);
	// 10671
	BOOL bLeftDim = FALSE;
	BOOL bRightDim = FALSE;
	BOOL bCenterDim = TRUE;
	if((nJijum == 0 && fabs(pBri->m_dWidthSlabLeft) > fabs(pBri->m_dWidthSlabRight)) || 
		(nJijum != 0 && fabs(pBri->m_dWidthSlabRight) > fabs(pBri->m_dWidthSlabLeft))
		)
	{
		bRightDim = TRUE;
		bCenterDim = FALSE;
	}
	DBStd.m_pDrawCrossSection->DimFrontSide(&Dom, (int)nJijum,FALSE,bRightDim,bCenterDim,TRUE, FALSE);		// 측면 치수
	DBStd.m_pDrawCrossSection->DimCrossSide(&Dom, dSta, CDPoint(0, 1), !bLeftDim, !bRightDim);
	DBStd.m_pDrawCrossSection->MarkFrontConcFck(&Dom, (int)nJijum,1);	// 콘크리트 심볼
	

	CLineInfo *pLine	= pBri->GetLineBase();
	// 벽체 중심 마크
	if(pLine)
	{
		double dCenOfRoad	= pBri->GetMidCrossSection(TRUE);
		CFootingApp* pFoot = pBri->GetFooting(0);
		double dEL = pFoot ? pFoot->m_dEL : pLine->GetElevationPlanEl(pBri->m_dStationBridgeStt);	// 기초 상단 EL
		double dELBottom = pFoot? pFoot->m_exFooting.m_dEL-pFoot->m_exFooting.GetMaxHeight():dEL;
		if(pBri->IsBoxType())
		{
			dELBottom = pBri->GetElevationBoxBottom(nJ);
		}
		double dTP = pBri->GetThickPave(FALSE);
		double dElCenter = pBri->GetXySlabUpperActByDist(dSta, 0).y;

		double dHeightCentralWall	= pBri->GetMaxHeightCentralWall();
		dElCenter += dHeightCentralWall;

		CDPoint xyCenter = CDPoint(-dCenOfRoad, dElCenter+dTP+Dom.Always(15));

		// 구조물 중심선
		if(!Compare(dCenOfRoad, 0.0, "="))
		{
			DBStd.MarkCenterOfRoad(&Dom, xyCenter, (dElCenter-dELBottom)+dTP+Dom.Always(25), _T(""), FALSE);
		}

		CConcSymbol sym;
		xyCenter.y -= Dom.GetTextHeight() * 3;
		sym.ArrowSide(&Dom, "중앙부", "지점부", xyCenter);
	}


	Dom.Rotate(CDPoint(0, 0), CDPoint(0, 1));
	
	// DBStd.m_pDrawCrossSection->DimCrossSide(&Dom);		// 슬래브 우측 치수

	// 상세 "A" 원
	CDPoint xyCen(0, 0), xyBound(0, 0);
	double dRadius = 0;
	AddDetailA(&Dom, xyCen, dRadius, TRUE);
	xyCen = xyCen.Rotate90();
	xyCen.MirrorVert();
	xyBound = xyCen + dRadius*CDPoint(1, -1).Unit();

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DETL);
	Dom.Circle(xyCen, dRadius);
	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.GiSiArrowExt(xyBound.x, xyBound.y, Dom.Always(8), Dom.Always(15), 7, _T("상세 \"A\""));

	Dom.SetCalcExtRect();
	CDRect rc = Dom.GetExtRect();
	Dom.Move(-rc.left, -rc.top);

	*pDomP << Dom;

	DBStd.m_pDrawCrossSection->MarkGenCrossSlope(&Dom, dSta, CDPoint(0, 1), TRUE, TRUE, TRUE, TRUE, TRUE);	// 편경사
	DBStd.m_pDrawCrossSection->MarkGenCrossSlope(&Dom, dSta, CDPoint(0, 1), TRUE, TRUE, TRUE, TRUE, FALSE, TRUE);	// 편경사
	//Dom.Mirror(0, FALSE);
	Dom.Move(-rc.left, -rc.top);
	*pDomP << Dom;
}

void CDrawingGeneral::AddLongi(CDomyun* pDomP)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	double dScale = GetScale();

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScale);

	// 일반도는 무조건 선형따라 그림 /////////////////////////////
//	long nOldTypeJongDanRebar	= pBri->m_nTypeJongdanForRebar;
//	pBri->m_nTypeJongdanForRebar	= 2;
	////////////////////////////////////////////////////////////////
	//pBri->SyncBridge(0, TRUE);
	pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 2);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DBStd.m_pDrawVertSection->DrawVertSectionAll(&Dom, TRUE, TRUE, TRUE, FALSE, TRUE);
	DBStd.m_pDrawVertSection->DrawVertSectionCenterLine(&Dom, FALSE, TRUE);
	
	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	DBStd.m_pDrawCrossSection->DimLongi(&Dom);
	DBStd.m_pDrawVertSection->DimThickVertSection(&Dom);

	

	CDPoint vAng(0, 0);
	BOOL bParen = FALSE;
	int nCountJijum = pBri->GetCountJijum();
	int i=0; for(i=0; i<nCountJijum; i++)
	{
		vAng = pBri->GetAngleJijum(i);
		vAng = CDPoint(Round(vAng.x, 5), Round(vAng.y, 5));
		if(Compare(vAng.GetAngleDegree(), 90.0, "="))
			bParen = TRUE;
	}
	if(bParen)
	{
		Dom.SetCalcExtRect();
		CDRect rc = Dom.GetExtRect();
		Dom.SetTextAlignHorz(TA_RIGHT);
		Dom.TextOut(rc.right, rc.top-Dom.Always(5), _T("* () 안의 치수는 직거리 치수임."));
	}

	//pBri->m_nTypeJongdanForRebar	= nOldTypeJongDanRebar;
	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE);

	// 상세 "B" 원
	if(pBri->IsBracket(TRUE) || pBri->IsBracket(FALSE))
	{
		CDPoint xyCen(0, 0), xyBound(0, 0);
		CTwinVector tv1;
		BOOL bStt	= pBri->IsBracket(TRUE) ? TRUE : FALSE;
		if(pBri->m_tvArrVertSection.GetTvByInfo(bStt ? _T("시점브라켓상면") : _T("종점브라켓상면"), tv1))
		{
			//////////////////////////////////////////////////////////////////////////
			
			double dRadius = 0;
			AddDetailB(&Dom, xyCen, dRadius, TRUE);

			CDPoint xyCenCircle	= bStt ? tv1.GetXyRight() : tv1.GetXyLeft();
			xyBound = xyCenCircle + dRadius*CDPoint(-1, -1).Unit();
			
			pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DETL);
			Dom.Circle(xyCenCircle, dRadius);
			pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
			Dom.GiSiArrowExt(xyBound.x, xyBound.y, Dom.Always(8), -Dom.Always(15), 5, _T("상세 \"B\""));
		}
	}

	// 신축이음 상세 원
	for(long stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		if(pBri->IsBlockOut(bStt))
		{
			CDPoint xyCen(0, 0), xyBound(0, 0);
			CTwinVector tv;
			if(pBri->m_tvArrVertSection.GetTvByInfo(bStt ? _T("시점슬래브측면") : _T("종점슬래브측면"), tv))
			{
				//////////////////////////////////////////////////////////////////////////
				double dRadius = tv.GetLength() * 2;
			

				CDPoint xyCenCircle	= tv.GetXyMid();
				xyBound = xyCenCircle + dRadius*CDPoint(-1, -1).Unit();
				
				pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DETL);
				Dom.Circle(xyCenCircle, dRadius);
				pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
				CString str	= _T("");
				str.Format("상세 \"%s\"", GetStrSectionNumberFlexibleJoint(bStt));
				Dom.GiSiArrowExt(xyBound.x, xyBound.y, Dom.Always(8), -Dom.Always(15), 5, str);
			}
		}
	}

	//단면제원
	if(pBri->IsBoxType())
		DBStd.m_pDrawVertSection->DimTextVertSectionLowerSlab(&Dom);

	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE);
	DBStd.m_pDrawVertSection->DimTextVertSectionUpperSlabFck(&Dom, 0);
	Dom.SetCalcExtRect();
	CDRect rc = Dom.GetExtRect();
	Dom.Move(-rc.left, -rc.top);

	*pDomP << Dom;
}

void CDrawingGeneral::AddDetailA(CDomyun* pDomP, CDPoint &xyCen, double &dRadius, BOOL bOnlyCalcCen)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	double dScaleDetailA = pData->m_dScaleGenDetailA;

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScaleDetailA);

	int nCountGW = pBri->GetQtyHDanTotalGuardFence();	// 가드 개수
	int nCountFW = pBri->GetQtyHDanRoadFootway();		// 보도 개수
	int nCountHDan = pBri->GetQtyHDan();	// 단 개수
	int nHDan = nCountHDan;
	if(nCountFW>0)
		nHDan = nCountHDan - 2;

	CDRect rect(0, 0, 0, 0);
	if(nCountGW!=0 || nCountFW!=0)
	{
		double dSta = pBri->GetStationOnJijum(0, 0);
		CDPoint vAng = CDPoint(0, 1);

		CDPoint A[30];
		pBri->GetXySlabUpperAct(dSta, vAng, A);

		CTwinVectorArray tvArr;
		pBri->GetTvCrossSection(tvArr, dSta, vAng, TRUE, TRUE, TRUE, 0, FALSE, TRUE, FALSE, FALSE, FALSE, TRUE);
		

		int nHDanAct = pBri->GetNumSlabNodeAct(dSta, vAng, nCountHDan);

		// 보도가 너무 크면 원이 너무 커지므로 적당히 잘라준다.
		if(pBri->m_nSeparBri == 2)
		{
			nHDan		= nCountHDan-1;
			nHDanAct	= nCountHDan;
		}
		else
		{
			nHDan		= 0;
			nHDanAct	= 1;
		}

		rect = tvArr.GetRect();
		rect.left = A[nHDan].x - Dom.Always(25);
		rect.top += 100.0;
		rect.right = A[nHDanAct].x + Dom.Always(50);
		rect.bottom -= 100.0;

		if(nHDan == 0)
			DBStd.m_pDrawCrossSection->DimCrossSectionSlabEnd(&Dom, TRUE, dSta, vAng, FALSE, TRUE, FALSE);
		else if(nHDan == nCountHDan-1)
			DBStd.m_pDrawCrossSection->DimCrossSectionSlabEnd(&Dom, FALSE, dSta, vAng, FALSE, TRUE, FALSE);


		xyCen = rect.CenterPoint() + CDPoint(245.0, -100.0);
		dRadius = (rect.Width()-490.0)/2.0 + 750.0;

		pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
		DBStd.DrawCuttingTvArray(&Dom, tvArr, rect);

		pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		long nIdxGuard	= pBri->m_nSeparBri == 2 ? pBri->GetQtyGuardWall()-1: 0;
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nIdxGuard);
		if(pGW && !pGW->IsTypeHDanOnlyNone())
			DBStd.m_pDrawCrossSection->DimGuardWall(&Dom, pGW,dSta, FALSE, pGW->IsLeftGuardFence(), !pGW->IsLeftGuardFence());

		//Dom.Mirror(0, TRUE);

		// 방호벽 측면 치수 다시 그림
		//DBStd.m_pDrawCrossSection->DimGuardWallSide(&Dom);

		// 노치상세원
		pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DETL);
		CTwinVectorArray tvArrNotch;
		tvArr.GetTvArrByInfo(nHDan == 0 ? "노치(좌)" : "노치(우)", tvArrNotch, FALSE, TRUE);
		CDRect rect	= tvArrNotch.GetRect();
		GetSwap(rect.top, rect.bottom);
		CDPoint xyCen	= rect.CenterPoint();
		double dRadius	= max(rect.Width(), rect.Height()) * 5;
		CDPoint xyBound	= nHDan == 0 ? xyCen + ToDPointFrDegree(315) * dRadius : xyCen + ToDPointFrDegree(225) * dRadius;

		Dom.Circle(xyCen, dRadius);
		pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		Dom.GiSiArrowExt(xyBound.x, xyBound.y, Dom.Always(8), nHDan == 0 ? Dom.GetTextWidth("NOTCH 상세") : -Dom.GetTextWidth("NOTCH 상세"), nHDan == 0 ? 7 : 5, _T("NOTCH 상세"));

	}

	Dom.RedrawByScale(GetScale()/dScaleDetailA);

	Dom.SetCalcExtRect();
	CDRect rc = Dom.GetExtRect();
	Dom.Move(-rc.left, -rc.top);

	if(!bOnlyCalcCen)
		*pDomP << Dom;
}

void CDrawingGeneral::AddNotch(CDomyun* pDomP)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	double dScaleNotch = pData->m_dScaleGenNotch;

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScaleNotch);

	double dNotchH	= pBri->m_dNotchHeight;
	double dNotchW1	= pBri->m_dNotchWidth1;
	double dNotchW2 = pBri->m_dNotchWidth2;
	double dLen		= (dNotchW1-dNotchW2)/2;
	double dNotchT	= 5;
	double dNotchL	= 50;
	double dNotchL2	= dNotchL-dNotchT;

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	//////////////////////////////////////////////////////////////////////////
	CDPoint xyStt(0, 0);
	CDPoint xyTemp(0, 0);
	CDPoint xyPin(0, 0);
	CDPoint xyTop(0, 0);

	CTwinVectorArray tvArr;
	CTwinVector tvTemp;
	tvArr.RemoveAll();
	
	tvTemp.m_v1 = xyStt;
	tvTemp.m_v2 = CDPoint(dNotchL, 0);
	tvArr.Add(tvTemp);
	tvTemp.m_v1 = tvTemp.m_v2;
	tvTemp.m_v2.x = tvTemp.m_v1.x+dLen;
	tvTemp.m_v2.y = tvTemp.m_v1.y+(dNotchH-5);
	tvArr.Add(tvTemp);
	tvTemp.m_v1 = tvTemp.m_v2;
	tvTemp.m_v2.x = tvTemp.m_v1.x+dNotchW1;
	tvTemp.m_v2.y = tvTemp.m_v1.y;
	tvArr.Add(tvTemp);
	tvTemp.m_v1 = tvTemp.m_v2;
	tvTemp.m_v2.x = tvTemp.m_v1.x+dLen;
	tvTemp.m_v2.y = tvTemp.m_v1.y-(dNotchH-5);
	tvArr.Add(tvTemp);
	tvTemp.m_v1 = tvTemp.m_v2;
	tvTemp.m_v2.x = tvTemp.m_v1.x+dNotchL;
	tvTemp.m_v2.y = tvTemp.m_v1.y;
	tvArr.Add(tvTemp);
	
	DBStd.DrawTvArray(&Dom, tvArr);
	CRebarPlacing rb;
	rb.GetOffsetTvArray(tvArr, -dNotchT);
	DBStd.DrawTvArray(&Dom, tvArr);	
	//////////////////////////////////////////////////////////////////////////
	
	Dom.MoveTo(xyStt.x, xyStt.y);
	xyTemp.y += dNotchT; 
	Dom.LineTo(xyTemp.x, xyTemp.y);
	xyTemp.x += dNotchL*2+dNotchW1;
	Dom.MoveTo(xyTemp.x, xyTemp.y);
	xyTemp.y -= dNotchT;
	Dom.LineTo(xyTemp.x, xyTemp.y);

	xyTemp = xyStt;
	xyTemp.x += dNotchL2/2-5;
	Dom.MoveTo(xyTemp.x, xyTemp.y);
	xyTemp.y += dNotchT;
	Dom.LineTo(xyTemp.x, xyTemp.y);
	xyTemp.x += 10;
	Dom.MoveTo(xyTemp.x, xyTemp.y);
	xyTemp.y -= dNotchT;
	Dom.LineTo(xyTemp.x, xyTemp.y);
	xyTemp.x -= 5;
	xyTemp.y += dNotchT+5;
	Dom.MoveTo(xyTemp.x, xyTemp.y);
	xyTemp.y -= (dNotchT+10);
	Dom.LineTo(xyTemp.x, xyTemp.y);
	
	xyTemp = xyStt;
	xyTemp.x += dNotchL2+dNotchW1+2*dNotchT+dNotchL2/2-5;
	Dom.MoveTo(xyTemp.x, xyTemp.y);
	xyTemp.y += dNotchT;
	Dom.LineTo(xyTemp.x, xyTemp.y);
	xyTemp.x += 10;
	Dom.MoveTo(xyTemp.x, xyTemp.y);
	xyTemp.y -= dNotchT;
	Dom.LineTo(xyTemp.x, xyTemp.y);
	xyTemp.x -= 5;
	xyTemp.y += dNotchT+5;
	xyPin = xyTemp;
	Dom.MoveTo(xyTemp.x, xyTemp.y);
	xyTemp.y -= (dNotchT+10);
	Dom.LineTo(xyTemp.x, xyTemp.y);

	//////////////////////////////////////////////////////////////////////////
	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(xyStt.x, xyStt.y, 0, "");
	Dom.DimLineTo(dNotchH, 0, COMMA(dNotchH));

	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(xyStt.x, xyStt.y, 0, "");
	Dom.DimLineTo(dNotchL, 0, COMMA(dNotchL));
	Dom.DimLineTo(dNotchW1, 0, COMMA(dNotchW1));
	Dom.DimLineTo(dNotchL, 0, COMMA(dNotchL));

	xyTop = CDPoint(xyStt.x+dNotchL2+dNotchW1/2, xyStt.y+dNotchH);
	CString str = _T("");
	str.Format("t=%.0fmm", dNotchT);
	Dom.GiSiArrowExtend(xyTop.x, xyTop.y, 15, -50, 2, "알루미늄판", str);
	Dom.GiSiArrowExtend(xyPin.x, xyPin.y, 20, 50, 1, "고정용 핀", "1,000 CTC");
	//////////////////////////////////////////////////////////////////////////
	Dom.RedrawByScale(GetScale()/dScaleNotch);

	Dom.SetCalcExtRect();
	CDRect rc = Dom.GetExtRect();
	Dom.Move(-rc.left, -rc.top);

	*pDomP << Dom;
}

void CDrawingGeneral::AddOrder(CDomyun* pDomP)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	double dScaleOrder = pData->m_dScaleGenOrder;

	CDomyun Dom(pDomP), DomGisi(pDomP);
	Dom.SetScaleDim(dScaleOrder);
	DomGisi.SetScaleDim(dScaleOrder);

	//pBri->SyncBridge(0, TRUE);
	if(pBri->IsBoxType())
	{
		if(pBri->m_bSlopeLowerSlab)
			pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 0);
		else
			pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, -1);
	}
	else
	{
		pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 0);
	}
	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DBStd.m_pDrawVertSection->DrawVertSectionAll(&Dom, TRUE, FALSE, FALSE, TRUE);
	//////////////////////////////////////////////////////////////////////////
	CTwinVector tvLeft, tvRight, tvU, tvL;
	CDPoint xy1(0, 0), xy2(0, 0), xyStt(0, 0);
 
	double dDist = 0;
	Dom.SetTextHeight(2);
	double dWidthText = DomGisi.GetTextWidth("시공이음")+DomGisi.Always(2);
	
	if(1)//pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB)
	{
		CTwinVector tvLower	= pBri->m_tvArrVertSection.GetTvByInfo("슬래브하면");
		CDPoint xy1 = (tvLower.m_v1+tvLower.m_v2*3)/4;
		CDPoint xy2 = (tvLower.m_v1*3+tvLower.m_v2)/4;
		xy1.y += pBri->m_dTS;
		xy2.y += pBri->m_dTS;
		
		//xyStt = CDPoint(xy2.x+(xy1.y-xy2.y)/2+dWidthText, xy2.y+(xy1.y-xy2.y)/2);
		dDist = fabs(xy1.x-xy2.x)/2;
		
		pStd->m_pARoadOptionStd->SetEnvType(&DomGisi, HCAD_DIML);
		DomGisi.GiSiArrow(xy1.x+dWidthText+dDist, xy1.y+dDist, -dWidthText, dDist, 5, "시공이음");
		DomGisi.GiSiArrow(xy2.x-dDist, xy2.y+dDist+(xy1.y-xy2.y), 0, dDist, 7, "");
		

	}
	else
	{
		if(pBri->IsOutsideWall())
		{
			pBri->m_tvArrVertSection.GetTvByInfo(_T("시점벽체우측면"), tvRight);

			xy1 = CDPoint(tvRight.m_v1.x, tvRight.m_v2.y+(tvRight.m_v1.y-tvRight.m_v2.y)*2/3);
			xy2 = CDPoint(tvRight.m_v2.x, tvRight.m_v2.y);
		}
		else
		{
			CWallApp *pWall = pBri->GetInWall(0);
			if(!pWall) return;
			if(!pWall->m_bIs) return;
			
			pWall->m_tvArrFront.GetTvByInfo(_T("벽체좌측면"), tvLeft);
			pWall->m_tvArrFront.GetTvByInfo(_T("벽체우측면"), tvRight);

			CDPoint xyMatch1(0, 0), xyMatch2(0, 0);
			CDPoint vAng	= tvLeft.GetXyDir().Rotate90();
			GetXyMatchLineAndLine(tvLeft.m_v1, vAng, tvRight.m_v1, tvRight.GetXyDir(), xy2);
			GetXyMatchLineAndLine(tvLeft.m_v1 + tvLeft.GetXyDir() * tvLeft.GetLength()/2, vAng, tvRight.m_v1, tvRight.GetXyDir(), xy1);

			/*
			xy1 = CDPoint(tvRight.m_v2.x, tvRight.m_v1.y+(tvRight.m_v2.y-tvRight.m_v1.y)*2/3);
			xy2 = CDPoint(tvRight.m_v1.x, tvRight.m_v1.y);
			*/
		}

		//xyStt = CDPoint(xy2.x+(xy1.y-xy2.y)/2+dWidthText, xy2.y+(xy1.y-xy2.y)/2);
		xyStt = CDPoint(xy2.x+~(xy1-xy2)/2+dWidthText, xy2.y+~(xy1-xy2)/2);
		dDist = ~(xy1-xy2)/2;//(xy1.y-xy2.y)/2;
		
		pStd->m_pARoadOptionStd->SetEnvType(&DomGisi, HCAD_DIML);
		DomGisi.GiSiArrow(xyStt.x, xyStt.y, -dWidthText, dDist, 3, "시공이음");
		DomGisi.GiSiArrow(xyStt.x, xyStt.y, -dWidthText, dDist, 5, "");

		DomGisi.Rotate(xy2, ToDPointFrDegree(tvLeft.GetXyDir().GetAngleDegree()-90));
	}

	//////////////////////////////////////////////////////////////////////////
	Dom.RedrawByScale(GetScale()/dScaleOrder);
	DomGisi.RedrawByScale(GetScale()/dScaleOrder);

	Dom.SetCalcExtRect();
	CDRect rc = Dom.GetExtRect();
	Dom.Move(-rc.left, -rc.top);

	DomGisi.Move(-rc.left, -rc.top);
	
	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE);
	*pDomP << Dom;
	*pDomP << DomGisi;
}

void CDrawingGeneral::AddFront(CDomyun* pDomP, int nJijum)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	double dScale = GetScale();
	double dSta = pBri->GetStationOnJijum(nJijum, 0);
	CDPoint vAng = pBri->GetAngleJijum(nJijum);
	CDPoint vAngWall	= CDPoint(0, 1);
	if(nJijum == 0 || nJijum == pBri->m_nQtyJigan)
	{
		if(pBri->m_nTypeWallForRebarGen == 0)
		{
			dSta	= pBri->GetStationByTarget(nJijum ==0 ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0, TRUE);
			vAng	= pBri->GetAngleByTarget(nJijum == 0 ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0);
		}
		else if(pBri->m_nTypeWallForRebarGen == 1)
		{
			dSta	= pBri->GetStationByTarget(nJijum == 0 ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_END_END, 0, TRUE);
			vAng	= pBri->GetAngleByTarget(nJijum == 0 ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_END_END, 0);
		}
	}
	else
	{
		// 경사교의 경우에는 좌측을 기준으로 횡단면도 그린다.
		CWallApp *pWall	= pBri->GetInWall(nJijum-1);
		if(pWall->m_nType == WALL_TYPE_SLOPE)
		{
			CLineInfo *pLine	= pBri->GetLineBase();
			CDPoint xyOrg		= pWall->GetXyOrgTopForFront(pWall->IsLeftSlope());
			CDPoint xyMatch		= xyOrg;
			CDPoint vAngWall	= ToDPointFrDegree(pWall->GetAngleWallByHor(TRUE));
			double dTP			= pBri->GetThickPave();
			pLine->GetXyMatchByJongDanXyAndAngle(xyOrg, vAngWall, xyMatch, -dTP);
			dSta	= xyMatch.x;
		}
		vAngWall	= ToDPointFrDegree(pWall->GetAngleWallByHor(TRUE));
	}



	long nTypeJongdanForRebar	= pBri->m_nTypeJongdanForRebar;
	pBri->m_nTypeJongdanForRebar	= 2;
	pBri->m_bSettingVertSection	= FALSE;
	pBri->SyncBridge(0, TRUE);
	
	// 말뚝인지?? 말뚝인 경우 하면 치수 dan을 높여야 됨.
	BOOL bPile	= FALSE;
	CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(nJijum);
	if(pFooting)
		if(pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_PILE) bPile	= TRUE;


	// 경사교는 세워서 그린다. /////////////////////////////////////////
	// 따라서 상부슬래브 횡단면도가 길어져야 됨.
	double dHeightUpperSlab	= pBri->GetHeightUpperSlab(nJijum, 0, vAngWall);
	double dHeightHunch	= dHeightUpperSlab - pBri->m_dTS;
	double dHDiff		= 0;
	if(nJijum > 0 && nJijum < pBri->m_nQtyJigan)
	{
		CWallApp *pWall	= pBri->GetInWall(nJijum-1);
		if(pWall)
		{
			if(!pWall->IsVertWall())
			{
				double dAng	= pWall->GetAngleByAngle(pWall->m_dAngle, pWall->m_dAnglePath);
				CDPoint vAng	= ToDPointFrDegree(dAng);
				dHDiff	= pBri->m_dTS/vAng.y - pBri->m_dTS;
				pBri->m_dTS += dHDiff;
				dHeightHunch -= dHDiff;
				pWall->m_dThickHunch -= dHDiff;
				pWall->m_dThickHunchL -= dHDiff;
				pWall->m_dThickHunchR -= dHDiff;
				pWall->m_dThickUppeSlab += dHDiff;
			}
		}
	}
	////////////////////////////////////////////////////////////////




	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScale);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DBStd.m_pDrawCrossSection->DrawCrossSection(&Dom, dSta, vAng, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, TRUE, TRUE, 0, nJijum);
	DBStd.m_pDrawCrossSection->DrawCrossSlabHunch(&Dom, dSta, vAng, TRUE, 0, FALSE, TRUE, vAngWall, FALSE, dHeightHunch);
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH && (nJijum == 0 || nJijum == pBri->m_nQtyJigan))
	{
		DBStd.m_pDrawCrossSection->DrawCrossSection_Section_ArchRib(&Dom, nJijum==0);
	}


	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	DBStd.m_pDrawCrossSection->DimCrossSection(&Dom, dSta, vAng, FALSE, FALSE, TRUE);
	DBStd.m_pDrawCrossSection->MarkGenCrossArrowDirection(&Dom, dSta, vAng, nJijum==0, TRUE);
	DBStd.m_pDrawCrossSection->DimFrontBottom(&Dom, nJijum, bPile ? 1 : 0);	// 하면 치수

	if(nJijum==0)
		Dom.Mirror(0, TRUE);

	if(pBri->m_bIsExp && (nJijum == 0 || nJijum == pBri->m_nQtyJigan))
	{
		CDomyun DomExp(pDomP);

		BOOL bStt	= nJijum == 0;
		BOOL bLeft	= bStt ? FALSE : TRUE;
		BOOL bInnerAtLeft	= pBri->m_nTypeWallForRebarGen == 0 ? FALSE : TRUE;
		BOOL bInnerAtRight	= bInnerAtLeft;

		DBStd.m_pDrawCrossSection->DrawCrossSection_OutWall_Exp(&DomExp, bStt, bInnerAtLeft, bInnerAtRight, bLeft);
		if(bStt)
		{
			DomExp.Move(CDPoint(-pBri->GetMidCrossSection()*2, 0));
		}
		*pDomP << DomExp;
	}

	DBStd.m_pDrawCrossSection->MarkGenCrossSlope(&Dom, dSta, vAng, nJijum==0, FALSE, FALSE, nJijum == 0);	// 편경사
	DBStd.m_pDrawCrossSection->DimFrontSide(&Dom, nJijum,TRUE,TRUE,TRUE,TRUE);		// 측면 치수
	DBStd.m_pDrawCrossSection->MarkFrontEl(&Dom, nJijum,TRUE,TRUE,TRUE);		// EL 마크
	DBStd.m_pDrawCrossSection->MarkFrontConcFck(&Dom, nJijum,TRUE);	// 콘크리트 심볼
	DBStd.m_pDrawCrossSection->DimMarkCenterOfRoadCrossSection(&Dom, nJijum, FALSE, TRUE, fabs(rect.Height()));

	if(!Compare(vAng.GetAngleDegree(), 90.0, "="))
	{
		Dom.SetCalcExtRect();
		CDRect rc = Dom.GetExtRect();
		Dom.SetTextAlignHorz(TA_RIGHT);
		Dom.TextOut(rc.right, rc.top-Dom.Always(5), _T("* () 안의 치수는 직거리 치수임."));
	}


	// 경사교는 세워서 그린다.
	// 따라서 상부슬래브 횡단면도가 길어져야 됨.
	pBri->m_dTS	-= dHDiff;
	if(nJijum > 0 && nJijum < pBri->m_nQtyJigan)
	{
		CWallApp *pWall	= pBri->GetInWall(nJijum-1);
		if(pWall)
		{
			if(!pWall->IsVertWall())
			{
				pWall->m_dThickHunch += dHDiff;
				pWall->m_dThickHunchL += dHDiff;
				pWall->m_dThickHunchR += dHDiff;
				pWall->m_dThickUppeSlab -= dHDiff;
			}
		}
	}


	pBri->m_nTypeJongdanForRebar	= nTypeJongdanForRebar;
	*pDomP << Dom;
}

void CDrawingGeneral::AddFootPlane(CDomyun* pDomP, int nJijum)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	double dScale = GetScale();

	CFootingApp* pFoot = pBri->GetFootingByJijumNumber(nJijum);
	if(!pFoot) return;
	if(!pFoot->m_bIs) return;
	if(nJijum > 0 && nJijum < pBri->m_nQtyJigan)
	{
		
		CWallApp *pWall = pBri->GetInWall(nJijum-1);
		if(pWall)
		{
			pBri->SyncWall(nJijum, pWall, TRUE);
			pWall->m_footing.SyncFooting(TRUE);
		}
	}


	BOOL bDimTablePile	= pData->m_bDimTableForPilePlane;	// 말뚝 배치 간격 테이블 처리
	int nCountJijum = pBri->GetCountJijum();
	BOOL bInWall = nJijum!=0 && nJijum!=nCountJijum-1;

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScale);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	// 기초 평면
	pFoot->GetTvPlan();
	DBStd.m_pDrawFooting->DrawPlan(&Dom, pFoot, FALSE);
	DBStd.m_pDrawExFooting->DrawPlan(&Dom, &pFoot->m_exFooting);

	// 아치교 스프링깅
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH && (nJijum==0 || nJijum==nCountJijum-1))
	{
		DBStd.m_pDrawPlane->DrawPlane_Section_Arch(&Dom, nJijum==0);
	}

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONH);
	if(bInWall)
	{
		// 벽체 평면
		CWallApp *pWall = pBri->GetInWall(nJijum-1);
		if(pWall!=NULL)
		{
			DBStd.m_pDrawWall->DrawPlan(&Dom, pWall, TRUE, TRUE, -1);
		}
	}
	else
	{
		// 날개벽 평면
		CRebarPlacing rb;
		CTwinVectorArray tvArr;
		pBri->GetTvWingWallPlane(nJijum==0, FALSE);
		CTwinVectorArray tvArrPlane;
		tvArrPlane = pFoot->m_tvArrplan;
		tvArrPlane.Sort();
		tvArr	= pBri->m_tvArrPlaneWingWall;
		rb.GetTvArrayByCrossTvArrayArea(tvArr, tvArrPlane, TRUE);
		DBStd.DrawTvArray(&Dom, tvArr);
	}

	CDPoint xyJijum = pBri->GetXyOnJijum(nJijum);
	CDPoint vAng = pBri->GetAngleAzimuthJijum(nJijum);
	vAng = CDPoint(Round(vAng.x, 5), Round(vAng.y, 5));
	if(nJijum==0) vAng *= -1;
	Dom.Rotate(xyJijum, vAng.GetMirrorVert());

	DBStd.m_pDrawExFooting->DimFootPlane(&Dom, nJijum, bDimTablePile);
	DBStd.m_pDrawExFooting->MarkFootPlaneCenterOfRoad(&Dom, nJijum);	// 도로 중심 마크
	Dom.Move(-xyJijum.x, -xyJijum.y);

	if(vAng!=CDPoint(0, 1) && !(pFoot->m_bRAngLeft && pFoot->m_bRAngRight))
	{
		Dom.SetCalcExtRect();
		CDRect rc = Dom.GetExtRect();
		Dom.SetTextAlignHorz(TA_RIGHT);
		Dom.TextOut(rc.right, rc.top-Dom.Always(5), _T("* () 안의 치수는 사거리 치수임."));
	}

	*pDomP << Dom;
}

void CDrawingGeneral::AddWingWallSection(CDomyun* pDomP, BOOL bStt, BOOL bLeft)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	double dScaleWing = pData->m_dScaleGenWingWall;

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScaleWing);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DBStd.m_pDrawWingWall->DrawGenSection(&Dom, bStt, bLeft);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	DBStd.m_pDrawWingWall->DimGenSection(&Dom, bStt, bLeft, FALSE);
	DBStd.m_pDrawWingWall->MarkGenSection(&Dom, bStt, bLeft, FALSE);

	Dom.RedrawByScale(GetScale()/dScaleWing);

	*pDomP << Dom;
}

void CDrawingGeneral::AddWingWallPlane(CDomyun* pDomP, BOOL bStt, BOOL bLeft)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	double dScaleWing = pData->m_dScaleGenWingWall;

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScaleWing);

	pBri->GetTvWingWallPlane(bStt);

	CTwinVector tvEnd, tvOut, tvSlab;
	CString strDir = bLeft ? _T("좌측") : _T("우측");
	pBri->m_tvArrPlaneWingWall.GetTvByInfo(strDir+_T("날개벽끝단"), tvEnd);
	pBri->m_tvArrPlaneWingWall.GetTvByInfo(strDir+_T("날개벽외측"), tvOut);
	pBri->m_tvArrPlaneWingWall.GetTvByInfo(_T("슬래브끝단외측"), tvSlab);
	CDPoint vSlab = tvSlab.GetXyDir().Rotate90();
	CDPoint xyMid(0, 0), xyCut(0, 0);
	xyMid = tvEnd.GetXyMid();
	BOOL bCross = GetXyMatchSegAndLine(tvSlab.m_v1, tvSlab.m_v2, xyMid, vSlab, xyCut);
	double dCutLen = bCross ? ~(xyCut-tvOut.m_v1) : 1500;
	if(dCutLen<1500)
		dCutLen = 1500;

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DBStd.m_pDrawWingWall->DrawGenCutPlane(&Dom, bStt, bLeft, dCutLen, TRUE);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	DBStd.m_pDrawWingWall->DimGenPlane(&Dom, bStt, bLeft, FALSE);
	DBStd.m_pDrawWingWall->MarkGenPlane(&Dom, bStt, bLeft, FALSE);

	CDPoint xyMove = pBri->GetXySectionWingWallStt(bStt, TRUE);
	xyMove.y = 0;
	Dom.Move(xyMove);

	Dom.RedrawByScale(GetScale()/dScaleWing);

	*pDomP << Dom;
}

void CDrawingGeneral::AddWingWallFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, CString str)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	double dScaleWing = pData->m_dScaleGenWingWall;

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScaleWing);

	long nIndex = (bStt)? ((bLeft)? 0:1):((bLeft)? 2:3);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DBStd.m_pDrawWingWall->DrawGenFront(&Dom, bStt, bLeft, 1, TRUE);
	DBStd.m_pDrawWingWall->DrawGenCutBriSect(&Dom, bStt, bLeft);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	DBStd.m_pDrawWingWall->DimGenFront(&Dom, bStt, bLeft, FALSE);
	DBStd.m_pDrawWingWall->MarkGenFrontEl(&Dom, bStt, bLeft, FALSE);
	DBStd.m_pDrawWingWall->MarkGenFrontSection(&Dom, bStt, bLeft, TRUE, FALSE, str, pData->m_bDrawGenWingWall[nIndex][1], pData->m_bDrawGenWingWall[nIndex][2]);
	DBStd.m_pDrawWingWall->MarkGenFrontSlope(&Dom, bStt, bLeft, FALSE);

	Dom.RedrawByScale(GetScale()/dScaleWing);

	*pDomP << Dom;
}



void CDrawingGeneral::AddDetailB(CDomyun* pDomP, CDPoint &xyCen, double &dRadius, BOOL bOnlyCalcCen)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	if(!pBri->IsBracket(TRUE) && !pBri->IsBracket(FALSE)) return;
	
	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	
	double dScaleDetailB = pData->m_dScaleGenDetailB;
	
	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScaleDetailB);
	

	CTwinVector tvUpper, tvUpperSide, tvSide, tvHunch;
	CTwinVectorArray tvArr; 

	BOOL bStt	= pBri->IsBracket(TRUE) ? TRUE : FALSE;


	//long nTypeJongdan	= pBri->m_nTypeJongdanForRebar;
	//pBri->m_nTypeJongdanForRebar	= 2;
	//pBri->SyncBridge(0, TRUE);
	pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 2);
	//pBri->m_nTypeJongdanForRebar	= nTypeJongdan;
	tvArr = pBri->m_tvArrVertSection;

	CString sStt	= bStt ? "시점" : "종점";
	tvArr.GetTvByInfo(sStt+_T("브라켓상면"), tvUpper);
	tvArr.GetTvByInfo(sStt+_T("브라켓상부측면"), tvUpperSide);
	tvArr.GetTvByInfo(sStt+_T("브라켓측면"), tvSide);
	tvArr.GetTvByInfo(sStt+_T("브라켓헌치"), tvHunch);

	
	CDRect rect(0, 0, 0, 0);
	
	if(bStt)
	{
		rect.left = tvSide.m_v2.x - 100.0;
		rect.top = tvUpperSide.m_v2.y + 1000.0;
		rect.right = tvUpper.m_v2.x + pBri->m_dWS + pBri->m_dHHS+100;
		rect.bottom = tvHunch.m_v1.y -1000.0;
	}
	else
	{
		rect.right = tvUpper.m_v2.x + 100.0;
		rect.top = tvUpperSide.m_v2.y + 1000.0;
		rect.left = tvUpper.m_v1.x - pBri->m_dWE - pBri->m_dHHE-100;
		rect.bottom = tvHunch.m_v1.y -1000.0;
	}
	
	xyCen = rect.CenterPoint() + CDPoint(245.0, -100.0);
	dRadius = (rect.Width()-490.0)/2.0 + 750.0;
	
	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DBStd.DrawCuttingTvArray(&Dom, tvArr, rect);
	
	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	DBStd.m_pDrawVertSection->DimVerSectionBracketHunch(&Dom, 0, TRUE, FALSE);

	CHgBaseDrawStd baseDraw;
	CDPointArray xyArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	long nDimDan	= 0;
	CString sDir	= bStt ? "LEFT" : "RIGHT";

	xyArrDim.Add(tvHunch.GetXyBottom());
	xyArrDim.Add(tvSide.GetXyBottom());
	xyArrDim.Add(tvSide.GetXyTop());
	xyArrDim.Add(tvUpperSide.GetXyTop());

	baseDraw.AutoDimCont(&Dom, CDPoint(tvSide.m_v1.x, xyArrDim.GetAt(0).y), xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, FALSE, TRUE);



	/*
	Dom.SetDirection(bStt ? "LEFT" : "RIGHT");
	Dom.DimMoveTo(tvSide.m_v1.x, tvSide.m_v1.y, 0, "");
	Dom.DimLineTo(tvSide.GetVerLength(), 0, COMMA(tvSide.GetVerLength()));
	Dom.DimLineTo(tvUpperSide.GetVerLength(), 0, COMMA(tvUpperSide.GetVerLength()));
	*/

	//Dom.DimMoveTo(tvHunch.m_v2.x, tvHunch.m_v1.y, 1, "");
	//Dom.DimLineTo(tvHunch.GetVerLength()+tvSide.GetVerLength(), 1, COMMA(tvHunch.GetVerLength()+tvSide.GetVerLength()));

	Dom.RedrawByScale(GetScale()/dScaleDetailB);
	
	Dom.SetCalcExtRect();
	CDRect rc = Dom.GetExtRect();
	Dom.Move(-rc.left, -rc.top);

	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE);

	if(!bOnlyCalcCen)
		*pDomP << Dom;
}

void CDrawingGeneral::AddNote(CDomyun* pDomP)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	
	CStringArray szArrNo;
	CStringArray szArrNot;

	szArrNo.RemoveAll();
	szArrNot.RemoveAll();
	
	szArrNo.Add("1.");
	szArrNot.Add("시공 이음부에는 구 콘크리트 레이턴스나 흔들리는");
	szArrNo.Add("");
	szArrNot.Add("골재 등을 완전히 제거시킨 후 신 콘크리트를 타설해야 한다.");
	szArrNo.Add("2.");
	szArrNot.Add("③,④번 구간은 콘크리트가 굳지 않은 상태에서 연속 타설");
	szArrNo.Add("");
	szArrNot.Add("해야 한다.");
	szArrNo.Add("3.");
	szArrNot.Add("진동 다짐시 과진동 다짐은 골재분리에 의한 콘크리트 강도"); 
	szArrNo.Add("");
	szArrNot.Add("저하가 발생하므로 주의하여야 한다.");
	szArrNo.Add("4.");
	szArrNot.Add("거푸집 및 동바리 설치시에는 콘크리트 하중에 의한 처짐을");
	szArrNo.Add("");
	szArrNot.Add("고려하여 시공하여야 한다.");
	szArrNo.Add("5.");
	szArrNot.Add("시공이음위치는 헌치철근을 고려하여 시공하여야 한다.");
	szArrNo.Add("6.");
	szArrNot.Add("교량 시공전 각 공정별 세부사항을 확인하여, 안전성을 확보한");
	szArrNo.Add("");
	szArrNot.Add("후 시공하여야한다.");

	long nSize = szArrNot.GetSize();
	CGridDomyunEx Dom(pDomP);
	pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_NOTE);
	double dWidthTxt = Dom.GetTextWidth(szArrNot.GetAt(9), 0);
	double dWidthNo = Dom.GetTextWidth(szArrNo.GetAt(0),0);

	double dSpaceWidFirst = Dom.Always(20);
	double dSpaceWidLast = dWidthTxt + dWidthNo + Dom.Always(20);

	Dom.SetBasePoint(CDPoint(0,0));
	Dom.SetGridLines(GVL_NONE);
	Dom.SetColumnCount(2);
	Dom.SetRowCount(nSize+2);
	Dom.SetColumnWidth(0, dSpaceWidFirst);
	Dom.SetColumnWidth(1, dSpaceWidLast);
	Dom.SetRowHeightAll(Dom.GetTextHeight()*2.5);
	Dom.SetRowHeight(0,Dom.Always(5));
	Dom.SetRowHeight(nSize+1,Dom.Always(5));

	CString szNo = _T("");
	CString str = _T("");
	long i=0; for(i=1; i<nSize+1; i++)
	{
		szNo = szArrNo.GetAt(i-1);
		str = szArrNot.GetAt(i-1);
		Dom.SetTextMatrix(i,0,szNo,GRID_ALIGN_RIGHT);
		Dom.SetTextMatrix(i,1,str,GRID_ALIGN_LEFT);
	}
	
	Dom.Draw(FALSE);
	Dom.DrawOutLine();
	Dom.DrawOutLineShade();
	pStd->ChangeSeperateLayer(&Dom);

	Dom.SetCalcExtRect();
	CDRect rc = Dom.GetExtRect();
	Dom.Move(-rc.left, -rc.top);

	*pDomP << Dom;
}

void CDrawingGeneral::AddDetailConcJoint(CDomyun* pDomP)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	double dScaleJoint = pData->m_dScaleGenConcJoint;

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScaleJoint);

	double dJointH	= pBri->m_dConsJointHeight;
	double dJointW1	= pBri->m_dConsJointWidth1;
	double dJointW2 = pBri->m_dConsJointWidth2;
	double dLen		= (dJointW2-dJointW1)/2;
	double dDist	= 5;
	double dL		= dJointW2;
	double dB		= dJointW2+dL*2;
	double dH		= dJointW2*2;
	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	//////////////////////////////////////////////////////////////////////////
	CDPoint xyStt(0, 0);
	CDPoint xyTemp(0, 0);
	CDPoint xyGisi(dL+dJointW2/2, -dJointH+dDist);
	CDPoint xyDim(dL, -dJointH);

	CDRect rcDom(-10, dH-10, dB+10, -(dH-10));
	CTwinVectorArray tvArr;
	CTwinVector tvTemp;
	tvTemp.m_v1 = xyStt;
	tvTemp.m_v2 = CDPoint(0, -dH);
	tvArr.Add(tvTemp);
	tvTemp.m_v1 = CDPoint(dB, 0);
	tvTemp.m_v2 = CDPoint(dB, -dH);
	tvArr.Add(tvTemp);
	tvTemp.m_v1 = CDPoint(0, dDist);
	tvTemp.m_v2 = CDPoint(0, dDist+dH);
	tvArr.Add(tvTemp);
	tvTemp.m_v1 = CDPoint(dB, dDist);
	tvTemp.m_v2 = CDPoint(dB, dDist+dH);
	tvArr.Add(tvTemp);

	DBStd.DrawCuttingTvArray(&Dom, tvArr, rcDom, TRUE);

	tvArr.RemoveAll();
	tvTemp.m_v1 = xyStt;
	tvTemp.m_v2 = CDPoint(dL, 0);
	tvArr.Add(tvTemp);
	tvTemp.m_v1 = tvTemp.m_v2;
	tvTemp.m_v2.x = tvTemp.m_v1.x+dLen;
	tvTemp.m_v2.y = tvTemp.m_v1.y-dJointH;
	tvArr.Add(tvTemp);
	tvTemp.m_v1 = tvTemp.m_v2;
	tvTemp.m_v2.x = tvTemp.m_v1.x+dJointW1;
	tvTemp.m_v2.y = tvTemp.m_v1.y;
	tvArr.Add(tvTemp);
	tvTemp.m_v1 = tvTemp.m_v2;
	tvTemp.m_v2.x = tvTemp.m_v1.x+dLen;
	tvTemp.m_v2.y = tvTemp.m_v1.y+dJointH;
	tvArr.Add(tvTemp);
	tvTemp.m_v1 = tvTemp.m_v2;
	tvTemp.m_v2.x = tvTemp.m_v1.x+dL;
	tvTemp.m_v2.y = tvTemp.m_v1.y;
	tvArr.Add(tvTemp);

	DBStd.DrawTvArray(&Dom, tvArr);
	CRebarPlacing rb;
	rb.GetOffsetTvArray(tvArr, -dDist);
	DBStd.DrawTvArray(&Dom, tvArr);	
	//////////////////////////////////////////////////////////////////////////
	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetDirection("LEFT");
	Dom.DimMoveTo(xyDim.x, xyDim.y, 0, "");
	Dom.DimLineTo(dJointH, 0, COMMA(dJointH));

	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(xyDim.x, xyDim.y, 0, "");
	Dom.DimLineToExtend(dLen, 0, "", COMMA(dLen));
	Dom.DimLineToExtend(dJointW1, 0, COMMA(dJointW2), "");
	Dom.DimLineToExtend(dLen, 0, "", COMMA(dLen));

	Dom.GiSiArrowExtend(xyGisi.x, xyGisi.y, 70, 200, 2, "수팽창지수재", "");
	//////////////////////////////////////////////////////////////////////////
	Dom.RedrawByScale(GetScale()/dScaleJoint);

	*pDomP << Dom;
}

// 신축이음 상세
void CDrawingGeneral::AddDetailFlexibleJoint(CDomyun *pDomP, BOOL bStt, CString sSectionNum)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	if(!pBri->IsBlockOut(bStt)) return;
	
	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	
	double dScaleDetail = pData->m_dScaleFXJoint;
	
	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScaleDetail);

	CFlexibleJoint *pFx	= bStt ? &pBri->m_fxJointStt : &pBri->m_fxJointEnd;
	CDRect rect	= pBri->m_tvArrVertSection.GetRect();
	if(bStt)
	{
		rect.right	= pFx->m_tvArrSection.GetRect().right + pBri->m_dWS + pBri->m_dHWS;
		rect.left	-= 100;
	}
	else
	{
		rect.left	= pFx->m_tvArrSection.GetRect().left - pBri->m_dWE - pBri->m_dHWE;
		rect.right	+= 100;
	}
	rect.top += 500;
	rect.bottom -= 500;

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DBStd.DrawCuttingTvArray(&Dom, pBri->m_tvArrVertSection, rect, TRUE);
	DBStd.m_pDrawVertSection->DrawVertSectionFlexibleJoint(&Dom, bStt);
	CTwinVectorArray tvArrPave;
	pBri->GetTvPave_VertSection(tvArrPave);
	DBStd.DrawCuttingTvArray(&Dom, tvArrPave, rect, TRUE);

	// 치수기입
	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	if(bStt)
		DBStd.m_pDrawVertSection->DimVerSectionSideStt(&Dom, 0, FALSE, FALSE);
	else
		DBStd.m_pDrawVertSection->DimVerSectionSideEnd(&Dom, 0, FALSE, FALSE);

	Dom.SetDirection("TOP");
	CTwinVector tv	= pFx->m_tvArrSection.GetTvByInfo(bStt ? "시점신축이음상면" : "종점신축이음상면");


	Dom.DimMoveTo(tv.GetXyLeft().x, tv.GetXyLeft().y, 1, "");
	long nJ	= bStt ? 0 : pBri->m_nQtyJigan;
	if(ComparePt(pBri->GetAngleJijum(nJ), CDPoint(0, 1), "="))
	{
		Dom.DimLineTo(tv.GetHorLength(TRUE), 1, COMMA(tv.GetHorLength(TRUE)));
	}
	else
	{
		Dom.DimLineToExtend(tv.GetHorLength(TRUE), 1, COMMA(tv.GetHorLength(TRUE)), COMMA2(tv.GetHorLength(FALSE)));
	}


	// 지시선
	CDPoint xyOrg	= tv.GetXyMid();
	CString strU	= "무수축 CON'C";
	CString strL	= "";
	strL.Format("fck=%0.f kgf/㎠", 400.0);
	double dDis2	= max(Dom.GetTextWidth(strU), Dom.GetTextWidth(strL));
	Dom.GiSiArrowExtend(xyOrg.x, xyOrg.y, Dom.GetTextHeight()*4, bStt ? dDis2 : -dDis2, bStt ? 1 : 3, strU, strL);

	DBStd.m_pDrawVertSection->DimTextVertSectionUpperSlabFck(&Dom, bStt ? -1 : 1);


	Dom.RedrawByScale(GetScale()/dScaleDetail);
	
	Dom.SetCalcExtRect();
	CDRect rc = Dom.GetExtRect();
	Dom.Move(-rc.left, -rc.top);	

	*pDomP << Dom;
}

// 신축이음 상세 번호
CString CDrawingGeneral::GetStrSectionNumberFlexibleJoint(BOOL bStt)
{
	CRcBridgeRebar *pBri = GetRcBridgeRebar();
	CString str	= _T("");
	
	if(pBri->IsBracket(TRUE) || pBri->IsBracket(FALSE))
	{
		str	= bStt ? "C" : "D";
	}
	else
	{
		str	= bStt ? "B" : "C";
	}


	return str;
}

// 균열 방지용 줄눈 상세
void CDrawingGeneral::AddDetailAntiCrack(CDomyun *pDom)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	double dScaleCrack = pData->m_dScaleGenCrack;

	CDomyun Dom(pDom);
	Dom.SetScaleDim(dScaleCrack);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	/////////////////////////////////////////////////////////////////////
	
	double dH	= 35;
	double dW	= 35;
	double dDist	= 200;

	Dom.MoveTo(0, 0);
	Dom.LineToEx(dDist, 0);
	Dom.LineToEx(dW/2, dH);
	Dom.LineToEx(dW/2, -dH);
	Dom.LineToEx(dDist, 0);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(dDist, 0, 0);
	Dom.DimLineTo(dW, 0, COMMA(dW));

	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(dDist + dW, 0, 0);
	Dom.DimLineTo(dH, 0, COMMA(dH));

	// 컷팅 라인
	double dExt	= 25;

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CUTL);
	Dom.CutLightning(0, -dExt, 0, dH * 3 + dExt, TRUE, 1);	// 좌
	Dom.CutLightning(-dExt, dH * 3, dDist * 2 + dW + dExt, dH * 3, FALSE, 1);	// 상
	Dom.CutLightning(dDist * 2 + dW, -dExt, dDist * 2 + dW, dH * 3 + dExt, TRUE, 1);	// 우


	Dom.RedrawByScale(GetScale()/dScaleCrack);
	*pDom << Dom;
}

void CDrawingGeneral::AddLowerSlabPlane( CDomyun* pDomP )
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	double dScale = GetScale();

	CFootingApp* pFoot = &pBri->m_footingBox;
	if(!pFoot) return;
	
//	BOOL bDimTablePile	= pData->m_bDimTableForPilePlane;	// 말뚝 배치 간격 테이블 처리
	
	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScale);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	// 기초 평면
	DBStd.m_pDrawPlane->DrawPlaneGen(&Dom, 5000, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE);
	DBStd.m_pDrawPlane->DrawPlaneGenOutWall(&Dom, TRUE, TRUE);
	DBStd.m_pDrawPlane->DrawPlaneGenOutWall(&Dom, FALSE, TRUE);
	DBStd.m_pDrawPlane->DrawPlaneGenInWallAndHunch(&Dom);
	DBStd.m_pDrawExFooting->DrawPlan(&Dom, &pFoot->m_exFooting, FALSE, FALSE);
	DBStd.m_pDrawExFooting->DimFootPlaneBoxType(&Dom);

	DBStd.RotateDomByBridgeSttEndXy(&Dom, TRUE);

	*pDomP << Dom;
}
