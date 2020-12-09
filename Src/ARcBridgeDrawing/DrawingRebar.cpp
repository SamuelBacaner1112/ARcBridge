// DrawingRebar.cpp: implementation of the CDrawingRebar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"
#include "../../Src/ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "../ARcBridgeDrawingOption/ARcBridgeDrawingOption.h"
#include "ARcBridgeDrawing.h"
#include "DrawingRebar.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define MPE_STANDARD_SECTION			10		// 표준단면도
#define MPE_STANDARD_SECTION_TITLE		20		// 표준단면도 제목
#define MPE_MAINREBAR_ASSM				30		// 주철근 조립도
#define MPE_MAINREBAR_ASSM_TITLE		40		// 주철근 조립도 제목
#define MPE_BMD							50		// BMD
#define MPE_BMD_TITLE					60		// BMD 제목
#define MPE_SFD							70		// SFD
#define MPE_SFD_TITLE					80		// SFD 제목
#define MPE_REBAR_UPPERSLAB				90		// 상부슬래브 구조도
#define MPE_REBAR_UPPERSLAB_TITLE		100		// 상부슬래브 구조도 제목
#define MPE_REBAR_CROSSECTION			110		// 횡단면도 구조도
#define MPE_REBAR_CROSSECTION_TITLE		120		// 횡단면도 구조도 제목
#define MPE_REBAR_WALL					130		// 벽체 구조도
#define MPE_REBAR_WALL_RIGHT			135		// 벽체 구조도(경사교일경우 우측면)
#define MPE_REBAR_WALL_TITLE			140		// 벽체 구조도 제목
#define MPE_REBAR_WALL_TITLE_RIGHT		145		// 벽체 구조도 제목(경사교일경우 우측면)
#define MPE_REBAR_WALLSEC				150		// 벽체 단면 구조도
#define MPE_REBAR_WALLSEC_TITLE			160		// 벽체 단면 구조도 제목
#define MPE_REBAR_FOOTINGPLANE			170		// 기초 평면 구조도
#define MPE_REBAR_FOOTINGPLANE_TITLE	180		// 기초 평면 구조도 제목
#define MPE_REBAR_COLUMNPLANE			190		// 기둥 평면 구조도
#define MPE_REBAR_COLUMNPLANE_TITLE		200		// 기둥 평면 구조도 제목
#define MPE_REBAR_GIRDERSEC				210		// 거더 단면도 
#define MPE_REBAR_GIRDERSEC_TITLE		220		// 거더 단면도 제목
#define MPE_REBAR_LOWER_GIRDERSEC		230		// 거더(하부) 단면도 
#define MPE_REBAR_LOWER_GIRDERSEC_TITLE	240		// 거더(하부) 단면도 제목
#define MPE_REBAR_LOWERSLAB				250		// 하부슬래브 구조도
#define MPE_REBAR_LOWERSLAB_TITLE		260		// 하부슬래브 구조도 제목
#define MPE_REBAR_DETAIL_PRF			270		// 부상방지저판 상세도
#define MPE_REBAR_DETAIL_PRF_TITLE		280		// 부상방지저판 상세도 제목
#define MPE_REBAR_UPPER_HUNCH			290		// 상부슬래브 헌치 구조도
#define MPE_REBAR_UPPER_HUNCH_TITLE		300		// 상부슬래브 헌치 제목
#define MPE_REBAR_LOWER_HUNCH			310		// 하부슬래브 헌치
#define MPE_REBAR_LOWER_HUNCH_TITLE		320		// 하부슬래브 헌치 제목

#define MPE_REBAR_WING_FRONT			190		// 시점 좌측 날개벽 정면도(전면)
#define MPE_REBAR_WING_FRONT_TITLE		200		// 시점 좌측 날개벽 정면도(전면) 제목
#define MPE_REBAR_WING_BACK				210		// 시점 좌측 날개벽 정면도(배면)
#define MPE_REBAR_WING_BACK_TITLE		220		// 시점 좌측 날개벽 정면도(배면) 제목
#define MPE_REBAR_WING_SECT_A			230		// 시점 좌측 날개벽 단면 A
#define MPE_REBAR_WING_SECT_A_TITLE		240		// 시점 좌측 날개벽 단면 A 제목
#define MPE_REBAR_WING_SECT_B			250		// 시점 좌측 날개벽 단면 B
#define MPE_REBAR_WING_SECT_B_TITLE		260		// 시점 좌측 날개벽 단면 B 제목
#define MPE_REBAR_WING_SECT_C			270		// 종점 우측 날개벽 단면 C
#define MPE_REBAR_WING_SECT_C_TITLE		280		// 종점 우측 날개벽 단면 C 제목
#define MPE_REBAR_WING_DETAIL			360		// 날개벽 보강상세
#define MPE_REBAR_WING_DETAIL_TITLE		370		// 날개벽 보강상세 제목

#define MPE_REBAR_DETAIL				290		// 철근 상세도
#define MPE_REBAR_DETAIL_TITLE			390		// 철근 상세도 제목
// 재료표 define이 이렇게 많은 이유? 
// 부위별로 재료표를 출력하려면 많이 필요하다.(현재 5개부위로 나눠짐)
#define MPE_REBAR_TABLE1				400		// 철근 재료표
#define MPE_REBAR_TABLE_TITLE1			410		// 철근 재료표 제목
#define MPE_REBAR_TABLE2				420		// 철근 재료표
#define MPE_REBAR_TABLE_TITLE2			430		// 철근 재료표 제목
#define MPE_REBAR_TABLE3				440		// 철근 재료표
#define MPE_REBAR_TABLE_TITLE3			450		// 철근 재료표 제목
#define MPE_REBAR_TABLE4				460		// 철근 재료표
#define MPE_REBAR_TABLE_TITLE4			470		// 철근 재료표 제목
#define MPE_REBAR_TABLE5				480		// 철근 재료표
#define MPE_REBAR_TABLE_TITLE5			490		// 철근 재료표 제목

// Chair Block 상세
#define MPE_DETAIL_CHAIRBLOCK_COVER			500		// 피복유지
#define MPE_DETAIL_CHAIRBLOCK_COVER_TITLE	510		// 피복유지 제목
#define MPE_DETAIL_CHAIRBLOCK_SPACER		520		// spacer 설치 위치
#define MPE_DETAIL_CHAIRBLOCK_SPACER_TITLE	530		// spacer 설치 위치 제목
#define MPE_DETAIL_CHAIRBLOCK_SHEAR			540		// 전단철근 
#define MPE_DETAIL_CHAIRBLOCK_SHEAR_TITLE	550		// 전단철근 제목

// 상부슬래 둔각부 보강 상세
#define MPE_DETAIL_OBTUSE_STT_LEFT			560	// 둔각부(시점좌측) 상세
#define MPE_DETAIL_OBTUSE_STT_LEFT_TITLE	570	// 둔각부(시점좌측) 상세 제목
#define MPE_DETAIL_OBTUSE_STT_RIGHT			580	// 둔각부(시점우측) 상세
#define MPE_DETAIL_OBTUSE_STT_RIGHT_TITLE	590	// 둔각부(시점우측) 상세 제목
#define MPE_DETAIL_OBTUSE_END_LEFT			600	// 둔각부(종점좌측) 상세
#define MPE_DETAIL_OBTUSE_END_LEFT_TITLE	610	// 둔각부(종점좌측) 상세 제목
#define MPE_DETAIL_OBTUSE_END_RIGHT			620	// 둔각부(종점우측) 상세
#define MPE_DETAIL_OBTUSE_END_RIGHT_TITLE	630	// 둔각부(종점우측) 상세 제목

// 벽체하단 예각부  보강 상세
#define MPE_DETAIL_ACUTE_STT_LEFT			640
#define MPE_DETAIL_ACUTE_STT_LEFT_TITLE		650
#define MPE_DETAIL_ACUTE_STT_RIGHT			660
#define MPE_DETAIL_ACUTE_STT_RIGHT_TITLE	670
#define MPE_DETAIL_ACUTE_END_LEFT			680
#define MPE_DETAIL_ACUTE_END_LEFT_TITLE		690
#define MPE_DETAIL_ACUTE_END_RIGHT			700
#define MPE_DETAIL_ACUTE_END_RIGHT_TITLE	710

// 아치리브 구조도
#define MPE_REBAR_ARCHRIB_SUPPORT_REBAR				720	// 아치리브 배력철근 조립도
#define MPE_REBAR_ARCHRIB_SUPPORT_REBAR_TITLE		730	// 아치리브 배력철근 조립도 제목
#define MPE_REBAR_ARCHRIB_PLANE_REBAR				740	// 아치리브 평면도
#define MPE_REBAR_ARCHRIB_PLANE_REBAR_TITLE			750	// 아치리브 평면도 제목
#define MPE_REBAR_ARCHRIB_CROSSSECTION				760	// 아치리브 횡단면도
#define MPE_REBAR_ARCHRIB_CROSSSECTION_TITLE		770	// 아치리브 횡단면도 제목
#define MPE_REBAR_ARCHRIB_SECTION_A					780	// 아치리브 Section A
#define MPE_REBAR_ARCHRIB_SECTION_A_TITLE			790	// 아치리브 Section A 제목
#define MPE_REBAR_ARCHRIB_SECTION_B					800	// 아치리브 Section B
#define MPE_REBAR_ARCHRIB_SECTION_B_TITLE			810	// 아치리브 Section B 제목
#define MPE_REBAR_ARCHRIB_SECTION_C					820	// 아치리브 Section C
#define MPE_REBAR_ARCHRIB_SECTION_C_TITLE			830	// 아치리브 Section C 제목

// 아치리브 스프링깅부 상세
#define MPE_DETAIL_ARCHJIJUM				840	// 아치리브 스프링깅부 상세
#define MPE_DETAIL_ARCHJIJUM_TITLE			850	// 아치리브 스프링깅부 상세 제목

// 아치리브 주철근 조립도
#define MPE_MAINREBAR_ASSM_ARCHRIB			860	// 아치리브 주철근 조립도
#define MPE_MAINREBAR_ASSM_ARCHRIB_TITLE	870	// 아치리브 주철근 조립도 제목

// 아치리브 AFD
#define MPE_AFD							880		// AFD
#define MPE_AFD_TITLE						890		// AFD 제목

#define MPE_DETAIL_JONGGIRDER				900		// 종거더 상세
#define MPE_DETAIL_JONGGIRDER_TITLE			910		// 종거더 상세 제목

// 커플러 상세도
#define MPE_DETAIL_COUPLER					920		// 커플러 상세도
#define MPE_TABLE_COUPLER					930		// 커플러 테이블
#define MPE_TABLE_COUPLER_TITLE				940		// 커플러 제목

CDrawingRebar::CDrawingRebar()
{
	m_nBridge = -1;
}

CDrawingRebar::~CDrawingRebar()
{

}

long CDrawingRebar::GetPageSu()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;

	if(pStd->GetCountTotalPage()<=0)
		pStd->MakePageInfo();

	m_nPage	= pStd->GetCountCategoryPage(_T("구조도"));

	return m_nPage;
}

CString CDrawingRebar::GetTitle()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("구조도"), pStd->m_nCurPage-m_nSttPage);
	if(pInfo==NULL) return _T("");

	CString strTitle = pStd->GetStringTitle(pStd->m_pARcBridgeDataStd->GetBridge(m_nBridge), pInfo->m_strTitle);

	return strTitle;
}

double CDrawingRebar::GetScale()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pOptData = pOpt->GetDrawingOption(m_nBridge);

	m_Scale	= pOptData->m_dScaleRebarBase;
	return pOptData->m_dScaleRebarBase;
}

void CDrawingRebar::SetNumber(long nSubPage)
{
	m_nBridge = GetNumberBridge(nSubPage-m_nSttPage);
}

long CDrawingRebar::GetNumberBridge(long nCategoryPageNum)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("구조도"), nCategoryPageNum);
	if(pInfo==NULL) return -1;

	return pInfo->m_nBri;
}

void CDrawingRebar::DrawOnePage()
{
	TRACE("CDrawingRebar");
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	
	CARcBridgeDataStd* pDataStd = pStd->m_pARcBridgeDataStd;
	CDrawDogak* pDgk = pStd->GetDogak();	
	CDrawPageMng* pMng = pStd->GetPageManage();

	long nPage = pStd->m_nCurPage;
	SetNumber(pStd->m_nDrawPage);

	CARcBridgeDrawingOptionData* pOptData = pOpt->GetDrawingOption(m_nBridge);
	if(!pOptData) return;
	double dScale = GetScale();

	CDomyun* pDom = pStd->GetDomyun(nPage);
	pDom->SetScaleDim(dScale);

	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("구조도"), pStd->m_nDrawPage-m_nSttPage);
	if(pInfo==NULL) return;

	nPage = pStd->GetPageNum(pInfo);

	pStd->m_pOptStd->SetEnvType(pDom, HCAD_MIS3);
	pStd->m_pOptStd->SetEnvType(pDom, HCAD_SYMB);

	CDomyun Dom(pDom);
	Dom.SetScaleDim(dScale);

	pMng->SetProgressData(_T("도각 출력 중..."), 0);

	long nSttPage = 0;
	CString strDgkName = pOpt->m_strDgkName;
	CString strHeadNote = pOpt->m_strDomNumHead;
	CString strSmallTitle = GetTitle();
	CString strXRefPath = IsDxfOut() ? pOpt->m_strXRefPath : _T("");

	CRcBridgeRebar* pBri = pDataStd->GetBridgeRebar(m_nBridge);
	pDataStd->m_strTempName = pBri->m_strBridgeName;

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

	// 구조도 출력
	pBri->SyncBridgeRebar(FALSE, 0, TRUE);

	long nCountPageMainRebarAssm	= pStd->GetCountPageMainRebarAssm(m_nBridge);
	long nCountPageStdSect			= 1;
	long nCountPageArchRibRebar		= pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH ? 1 : 0;
	long nCountPageUpperSlab		= 1;
	long nCountPageLowerSlab		= pBri->IsBoxType()? 1 : 0;
	long nCountPageInWallRebarOfFront	= pStd->GetCountPageInWallRebar(m_nBridge, TRUE);
	long nCountPageInWallRebarOfBack	= pStd->GetCountPageInWallRebar(m_nBridge, FALSE);
	long nCountPageSttWallRebar		= pBri->IsOutWall(TRUE) ? 1 : 0;
	long nCountPageEndWallRebar		= pBri->IsOutWall(FALSE) ? 1 : 0;
	long nCountPageWingWall			= pStd->GetCountPageWingWallRebar(m_nBridge);
	long nCountPageRebarBMMain		= pOptData->m_bSeparateOutputRebarBMMain ? 2 : 1;
	
	long nPageNum	= pInfo->m_nPageNumber;	// 인덱스
	BOOL bDraw		= FALSE;
	
	// 주철근 조립도
	if(nPageNum < nCountPageMainRebarAssm && !bDraw)
	{
		AddPageMainRebarAssm(&Dom, nPageNum, rcDogak);
		bDraw	= TRUE;
	}
	else
		nPageNum -= nCountPageMainRebarAssm;

	// 표준단면도
	if(nPageNum < nCountPageStdSect && !bDraw)
	{
		AddPageStandardSection(&Dom, rcDogak);
		bDraw	= TRUE;
	}
	else
		nPageNum -= nCountPageStdSect;

	// 아치리브 구조도
	if(nPageNum < nCountPageArchRibRebar && !bDraw)
	{
		AddPageArchRibRebar(&Dom, rcDogak);
		bDraw	= TRUE;
	}
	else
		nPageNum -= nCountPageArchRibRebar;
	
	// 상부 슬래브 구조도
	if(nPageNum < nCountPageUpperSlab && !bDraw)
	{
		AddPageRebarUpperSlab(&Dom, rcDogak);
		bDraw	= TRUE;
	}
	else
		nPageNum -= nCountPageUpperSlab;

	// 하부 슬래브 구조도
	if(nPageNum < nCountPageLowerSlab && !bDraw)
	{
		AddPageRebarLowerSlab(&Dom, rcDogak);
		bDraw	= TRUE;
	}
	else
		nPageNum -= nCountPageLowerSlab;

	// 시점측 벽체 구조도
	if(nPageNum < nCountPageSttWallRebar && !bDraw)
	{
		AddPageOutWallRebar(&Dom, TRUE, rcDogak);
		bDraw	= TRUE;
	}
	else
		nPageNum -= nCountPageSttWallRebar;

	// 종점측 벽체 구조도
	if(nPageNum < nCountPageEndWallRebar && !bDraw)
	{
		AddPageOutWallRebar(&Dom, FALSE, rcDogak);
		bDraw	= TRUE;
	}
	else
		nPageNum -= nCountPageEndWallRebar;

	// 중간벽체 구조도
	if(nPageNum < nCountPageInWallRebarOfFront && !bDraw)
	{
		AddPageInWallRebar(&Dom, nPageNum, rcDogak, TRUE);
		bDraw	= TRUE;
	}
	else
		nPageNum -= nCountPageInWallRebarOfFront;

	// 날개벽 구조도
	if(!pOptData->m_bSeparateRebarBMWing)
	{
		if(nPageNum < nCountPageWingWall && !bDraw)
		{
			AddPageWingWallRebar(&Dom, nPageNum, rcDogak);

			bDraw	= TRUE;
		}
		else
			nPageNum -= nCountPageWingWall;
	}

	// 철근 재료표
	if(nPageNum < nCountPageRebarBMMain && !bDraw)
	{
		if(pOptData->m_bSeparateOutputRebarBMMain)
		{
			if(nPageNum < nCountPageRebarBMMain-1)
				AddPageRebarBM(&Dom, rcDogak, 0, TRUE, TRUE, 0, TRUE, FALSE);
			else
				AddPageRebarBM(&Dom, rcDogak, 0, TRUE, TRUE, 0, FALSE, TRUE);
		}
		else
		{
			AddPageRebarBM(&Dom, rcDogak, 0, TRUE, TRUE, 0, TRUE, TRUE);
		}
		bDraw	= TRUE;
	}
	else
		nPageNum -= nCountPageRebarBMMain;

	// 중간벽체 구조도
	if(nPageNum < nCountPageInWallRebarOfBack && !bDraw)
	{
		AddPageInWallRebar(&Dom, nPageNum, rcDogak, FALSE);
		bDraw	= TRUE;
	}
	else
		nPageNum -= nCountPageInWallRebarOfBack;

	// 날개벽 구조도
	if(pOptData->m_bSeparateRebarBMWing)
	{
		if(nPageNum < nCountPageWingWall && !bDraw)
		{
			AddPageWingWallRebar(&Dom, nPageNum, rcDogak);

//			bDraw	= TRUE;
		}
// 		else
// 			nPageNum -= nCountPageWingWall;
	}

	pMng->SetProgressData(_T("출력 완료  ..."), 100);

	*pDom << Dom;
}

void CDrawingRebar::AddPageOne(CDomyun* pDomP, long nSubPage)
{
	CDRect rcStandardSection;

	CDomyun Dom(pDomP);

	Dom.SetScaleDim(GetScale());
}

// 표준단면도 그림.
void CDrawingRebar::AddPageStandardSection(CDomyun *pDomP, CDRect rcDogak)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);

	BOOL bVertDir	= pBri->IsVertDir();
	//pBri->SyncBridge(0, !bVertDir);
	pBri->GetTvVertSection_Std(0, !bVertDir, FALSE);
	

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;

	double dScale	= GetScale();
	DWORD dOptDraw	= pData->m_dwDrawRebar;

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScale);

	CDPoint xyMove(0, 0);
	CDRect rect(0, 0, 0, 0);

	double dXMidOfDogak	= rcDogak.left+rcDogak.Width()/2;
	// 표준단면도 //////////////////////////////////////////////////////////////////////
	CDRect rcStandardSection(0, 0, 0, 0);	// 표준단면도 영역
	if(dOptDraw & REBAR_STD_SECT)
	{
		if(m_pStd->IsDrawByBlock(MPE_STANDARD_SECTION))
		{
			pPageMng->SetProgressData(_T("표준단면도 출력중..."), 50);
			rcStandardSection = DBStd.m_pDrawVertSection->DrawStandardSectionStd(&Dom, TRUE, TRUE, pData->m_bMainRebarNumberingInStandardSection);
			xyMove		= CDPoint(dXMidOfDogak-(rcStandardSection.left+rcStandardSection.Width()/2), rcDogak.bottom-rcStandardSection.bottom-Dom.Always(100)); 
			Dom.Move(xyMove);
			rcStandardSection.left		+= xyMove.x; rcStandardSection.right	+= xyMove.x;
			rcStandardSection.top		+= xyMove.y; rcStandardSection.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_STANDARD_SECTION));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_STANDARD_SECTION, _T("표준단면도"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_STANDARD_SECTION_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("표준단면도"), CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_STANDARD_SECTION_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_STANDARD_SECTION_TITLE, _T("표준단면도 타이틀"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcStandardSection.left		= rcDogak.left+Dom.Always(150);
		rcStandardSection.bottom	= rcDogak.bottom-Dom.Always(100);
		rcStandardSection.right		= rcStandardSection.left;
		rcStandardSection.top		= rcStandardSection.bottom;
	}

	//--부상방지저판
//	CDRect rcPRF(0, 0, 0, 0);	// 부상방지저판 상세도 영역
	double dScaleDetail = pData->m_dScaleRebarDetailPRF;
	Dom.SetScaleDim(dScaleDetail);

	if(pBri->m_bExistProtectionRiseFooting && dOptDraw & REBAR_DETAIL_PRF)
	{
		if(m_pStd->IsDrawByBlock(MPE_REBAR_DETAIL_PRF))
		{
			pPageMng->SetProgressData(_T("부상방지저판 상세도 출력중..."), 50);
			DBStd.m_pDrawVertSection->DrawBFootingSection(&Dom, TRUE);
 			DBStd.m_pDrawVertSection->DrawBFootingRebar(&Dom, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE);
			Dom.RedrawByScale(dScale/dScaleDetail);
			Dom.SetCalcExtRect();
			CDRect rcPRF	= Dom.GetExtRect();
			xyMove		= CDPoint(rcDogak.left - rcPRF.left + Dom.Always(50) ,rcStandardSection.top - rcPRF.bottom - Dom.Always(100)); 
			Dom.Move(xyMove);
			rcPRF.left	+= xyMove.x; rcPRF.right	+= xyMove.x;
			rcPRF.top	+= xyMove.y; rcPRF.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();
			
			Dom.Move(pMove->GetPoint(MPE_REBAR_DETAIL_PRF));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_DETAIL_PRF, _T("부상방지저판 상세도"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_REBAR_DETAIL_PRF_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("부상방지저판 상세도"), CDPoint(0,0), 0, dScaleDetail);
			Dom.RedrawByScale(dScale/dScaleDetail);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_REBAR_DETAIL_PRF_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_DETAIL_PRF_TITLE, _T("부상방지저판 상세도 타이틀"));
			*pDomP << Dom;
		}
	}

	pBri->GetTvVertSection_Std(0, !bVertDir, FALSE, TRUE);
	////////////////////////////////////////////////////////////////////////////////////

}

// 현재 그려야 되는 RcBridge리턴
CRcBridgeRebar* CDrawingRebar::GetRcBridgeRebar()
{
	CARcBridgeDrawingStd* pStd	= (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDataStd* pDataStd	= pStd->m_pARcBridgeDataStd;

	CRcBridgeRebar *pBri		= NULL;

	if(m_nBridge > -1 && pDataStd->GetBridgeSize() > m_nBridge)
	{
		pBri	= pDataStd->GetBridgeRebar(m_nBridge);
	}

	return pBri;
}

// 주철근 조립도 페이지 인덱스에 따라서 출력해야 되는 좌우측 cycle 정보를 가져옴.
void CDrawingRebar::GetInfoMainRebarAssm(long nIdxMainRebarAssmPage, long &nLeftCycle, long &nRightCycle)
{
	nLeftCycle	= -1;
	nRightCycle	= -1;

	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);

	long nType	= pData->m_nTypeMainRebarAssm;

	// 2Cycle일때
	if(pBri->GetCountCycleMainRebar() == 2)
	{
		// 표현방법 1
		if(nType == 0)
		{
			switch(nIdxMainRebarAssmPage)
			{
			case 0: 
				nLeftCycle	= 0;
				nRightCycle	= 0;
				break;
			case 1:
				nLeftCycle	= 1;
				nRightCycle	= 1;
			default: break;
			}
		}
		// 표현방법 2
		else if(nType == 1)
		{
			switch(nIdxMainRebarAssmPage)
			{
			case 0:
				nLeftCycle	= 0;
				nRightCycle	= 1;
				break;
			default: break;
			}
		}
	}
	// 4Cycle일때.
	else
	{
		// 표현방법 1
		if(nType == 0)
		{
			switch(nIdxMainRebarAssmPage)
			{
			case 0: 
				nLeftCycle	= 0;
				nRightCycle	= 0;
				break;
			case 1:
				nLeftCycle	= 2;
				nRightCycle	= 2;
				break;
			case 2: 
				nLeftCycle	= 1;
				nRightCycle	= 3;
				break;
			default: break;
			}
		}
		// 표현방법 2
		else if(nType == 1)
		{
			switch(nIdxMainRebarAssmPage)
			{
			case 0:
				nLeftCycle	= 0;
				nRightCycle	= 2;
				break;
			case 1:
				nLeftCycle	= 1;
				nRightCycle	= 3;
				break;
			default: break;
			}
		}
	}
}

// 주철근 조립도 그리기.
void CDrawingRebar::AddPageMainRebarAssm(CDomyun *pDomP, long nIdxMainRebarAssmPage, CDRect rcDogak)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);

	BOOL bVertDir	= pBri->IsVertDir();

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;
	double dScale	= GetScale();
	DWORD dOptDraw	= pData->m_dwDrawRebar;

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScale);

	CString str	= _T("");
	CDPoint xyMove(0, 0);
	CDRect rect(0, 0, 0, 0);
	int Pos	= 0;
	int nOffsetPos	= 100 / 3;
	double dXMidOfDogak	= rcDogak.left+rcDogak.Width()/2;
	
	// 주철근 조립도 //////////////////////////////////////////////////////////////////////
	CDRect rcMainRebarAssm(0, 0, 0, 0);	// 주철근 조립도 영역 영역
	if(dOptDraw & REBAR_MAIN_ASSM)
	{
		if(m_pStd->IsDrawByBlock(MPE_MAINREBAR_ASSM+nIdxMainRebarAssmPage))
		{
			pPageMng->SetProgressData(_T("주철근 조립도 출력중..."), Pos+=nOffsetPos);
			long nCycleLeft		= 0;
			long nCycleRight	= 0;
			GetInfoMainRebarAssm(nIdxMainRebarAssmPage, nCycleLeft, nCycleRight);
			rcMainRebarAssm = DBStd.m_pDrawVertSection->DrawMainRebarAssmStd(&Dom, nCycleLeft, nCycleRight, bVertDir, FALSE);
			xyMove	= CDPoint(dXMidOfDogak-(rcMainRebarAssm.left+rcMainRebarAssm.Width()/2), rcDogak.bottom-rcMainRebarAssm.bottom-Dom.Always(100));
			Dom.Move(xyMove);
			rcMainRebarAssm.left	+= xyMove.x; rcMainRebarAssm.right	+= xyMove.x;
			rcMainRebarAssm.top		+= xyMove.y; rcMainRebarAssm.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_MAINREBAR_ASSM+nIdxMainRebarAssmPage));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_MAINREBAR_ASSM+nIdxMainRebarAssmPage, _T("주철근조립도")+COMMA(nIdxMainRebarAssmPage+1));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_MAINREBAR_ASSM_TITLE+nIdxMainRebarAssmPage))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("주철근조립도")+COMMA(nIdxMainRebarAssmPage+1), CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_MAINREBAR_ASSM_TITLE+nIdxMainRebarAssmPage));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_MAINREBAR_ASSM_TITLE+nIdxMainRebarAssmPage, _T("주철근조립도 타이틀"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcMainRebarAssm.left	= rcDogak.left + Dom.Always(50);
		rcMainRebarAssm.bottom	= rcDogak.bottom - Dom.Always(100);
		rcMainRebarAssm.right	= rcMainRebarAssm.left;
		rcMainRebarAssm.top		= rcMainRebarAssm.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////

	// 아치리브 주철근 조립도 //////////////////////////////////////////////////////////////////////
	CDRect rcMainRebarAssmArchRib(0, 0, 0, 0);	// 아치리브 주철근조립도 영역 영역
	if((dOptDraw & REBAR_MAIN_ASSM) && (pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH))
	{
		if(m_pStd->IsDrawByBlock(MPE_MAINREBAR_ASSM_ARCHRIB+nIdxMainRebarAssmPage))
		{
			pPageMng->SetProgressData(_T("아치리브 주철근 조립도 출력중..."), Pos+=nOffsetPos);
			long nCycleLeft		= 0;
			long nCycleRight	= 0;
			GetInfoMainRebarAssm(nIdxMainRebarAssmPage, nCycleLeft, nCycleRight);
			rcMainRebarAssmArchRib = DBStd.m_pDrawVertSection->DrawRebarAssmArchRibStd(&Dom, nCycleLeft, nCycleRight);
			xyMove	= CDPoint(dXMidOfDogak-(rcMainRebarAssmArchRib.left+rcMainRebarAssmArchRib.Width()/2), rcDogak.bottom-rcMainRebarAssmArchRib.bottom-Dom.Always(150)-rcMainRebarAssm.Height());
			Dom.Move(xyMove);
			rcMainRebarAssmArchRib.left		+= xyMove.x; rcMainRebarAssmArchRib.right	+= xyMove.x;
			rcMainRebarAssmArchRib.top		+= xyMove.y; rcMainRebarAssmArchRib.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_MAINREBAR_ASSM_ARCHRIB+nIdxMainRebarAssmPage));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_MAINREBAR_ASSM_ARCHRIB+nIdxMainRebarAssmPage, _T("아치리브 주철근조립도")+COMMA(nIdxMainRebarAssmPage+1));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_MAINREBAR_ASSM_ARCHRIB_TITLE+nIdxMainRebarAssmPage))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("아치리브 주철근조립도")+COMMA(nIdxMainRebarAssmPage+1), CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_MAINREBAR_ASSM_ARCHRIB_TITLE+nIdxMainRebarAssmPage));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_MAINREBAR_ASSM_ARCHRIB_TITLE+nIdxMainRebarAssmPage, _T("아치리브 주철근조립도 타이틀"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcMainRebarAssmArchRib	= rcMainRebarAssm;
	}
	////////////////////////////////////////////////////////////////////////////////////

	// BMD와 SFD는 주철근 조립도 첫번째 장에만 출력한다.
	CDRect rcBMD(0, 0, 0, 0);
	CDRect rcSFD(0, 0, 0, 0);
	double dXMidofMainRebarAssm	= rcMainRebarAssmArchRib.left+rcMainRebarAssmArchRib.Width()/2;
	if(nIdxMainRebarAssmPage == 0)
	{
		Dom.SetScaleDim(pData->m_dScaleBMD);
		// BMD /////////////////////////////////////////////////////////////////////////////
		if(dOptDraw & REBAR_BMD)
		{
			if(m_pStd->IsDrawByBlock(MPE_BMD))
			{
				pPageMng->SetProgressData(_T("BMD 출력중..."), Pos+=nOffsetPos);
				rcBMD	= DBStd.m_pDrawVertSection->DrawDiagramBMD_SFD(&Dom, TRUE, FALSE, FALSE, TRUE, FALSE);
				xyMove	= CDPoint(dXMidofMainRebarAssm-rcBMD.right-Dom.Always(10), rcMainRebarAssmArchRib.top-rcBMD.bottom-Dom.Always(30));
				Dom.Move(xyMove);
				rcBMD.left	+= xyMove.x; rcBMD.right	+= xyMove.x;
				rcBMD.top	+= xyMove.y; rcBMD.bottom	+= xyMove.y;
				Dom.SetCalcExtRect();
				rect	= Dom.GetExtRect();

				Dom.Move(pMove->GetPoint(MPE_BMD));
				pStd->ChangeSeperateLayer(&Dom);
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BMD, _T("BMD"));
				*pDomP << Dom;
			}

			if(m_pStd->IsDrawByBlock(MPE_BMD_TITLE))
			{
				pStd->AddTitleOnDom(&Dom, m_nBridge, _T("BMD"), CDPoint(0,0), 0, pData->m_dScaleBMD);
				Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
				Dom.Move(pMove->GetPoint(MPE_BMD_TITLE));
				pStd->ChangeSeperateLayer(&Dom);
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_BMD_TITLE, _T("BMD 타이틀"));
				*pDomP << Dom;
			}
		}
		else
		{
			rcBMD.left		= rcMainRebarAssm.left;
			rcBMD.bottom	= rcMainRebarAssm.top - Dom.Always(30);
			rcBMD.right		= rcBMD.left;
			rcBMD.top		= rcBMD.bottom;
		}
		////////////////////////////////////////////////////////////////////////////////////

		Dom.SetScaleDim(pData->m_dScaleSFD);
		// SFD /////////////////////////////////////////////////////////////////////////////
		if(dOptDraw & REBAR_SFD)
		{
			if(m_pStd->IsDrawByBlock(MPE_SFD))
			{
				pPageMng->SetProgressData(_T("SFD 출력중..."), Pos+=nOffsetPos);
				rcSFD	= DBStd.m_pDrawVertSection->DrawDiagramBMD_SFD(&Dom, FALSE, TRUE, FALSE, TRUE, FALSE);
				xyMove	= CDPoint(dXMidofMainRebarAssm-rcSFD.left+Dom.Always(10), rcBMD.bottom-rcSFD.bottom);
				Dom.Move(xyMove);
				rcSFD.left	+= xyMove.x; rcSFD.right	+= xyMove.x;
				rcSFD.top	+= xyMove.y; rcSFD.bottom	+= xyMove.y;
				Dom.SetCalcExtRect();
				rect	= Dom.GetExtRect();

				Dom.Move(pMove->GetPoint(MPE_SFD));
				pStd->ChangeSeperateLayer(&Dom);
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_SFD, _T("SFD"));
				*pDomP << Dom;
			}

			if(m_pStd->IsDrawByBlock(MPE_SFD_TITLE))
			{
				pStd->AddTitleOnDom(&Dom, m_nBridge, _T("SFD"), CDPoint(0,0), 0, pData->m_dScaleSFD);
				Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
				Dom.Move(pMove->GetPoint(MPE_SFD_TITLE));
				pStd->ChangeSeperateLayer(&Dom);
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_SFD_TITLE, _T("SFD 타이틀"));
				*pDomP << Dom;
			}
		}
		else
		{
			rcSFD.left		= rcBMD.left+Dom.Always(20);
			rcSFD.bottom	= rcBMD.bottom;
			rcSFD.right		= rcSFD.left;
			rcSFD.top		= rcSFD.bottom;
		}
		////////////////////////////////////////////////////////////////////////////////////
	}
	
	*pDomP << Dom;
}

void CDrawingRebar::AddPageRebarUpperSlab(CDomyun *pDomP, CDRect rcDogak)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;
	double dScale	= GetScale();
	DWORD dOptDraw	= pData->m_dwDrawRebar;

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScale);

	BOOL bVertDir	= pBri->IsVertDir();
	//pBri->SyncBridge(0, !bVertDir);
	pBri->GetTvVertSection_Std(0, !bVertDir, FALSE);
	pBri->SyncBridgeRebar(FALSE, bVertDir?-1:1, TRUE);
	
	CString str	= _T("");

	// (ARCBRIDGE-3516) 수평철근 치수 기준위치 옵션 추가(0: 좌우측 끝단, 1: 선형위치)
	DBStd.m_pDrawPlane->m_nPosBaseDimSupportRebar = pData->m_nBasePosPlaneSupportRebarDim;
	
	CDPoint xyMove(0, 0);
	CDRect rect(0, 0, 0, 0);
	// 상부슬래브 구조도 조립도 //////////////////////////////////////////////////////////////////////
	CDRect rcUpperSlab[2]	= {(0, 0, 0, 0), (0, 0, 0, 0)};	// 주철근 조립도 영역 영역
	for(long upper = 0; upper < 2; upper++)
	{
		BOOL bUpper	= upper == iUPPER;
		if(!pData->m_bSepUpperAndLowerSlabRebar && !bUpper) continue;

		CString sTitle	= "평면도";
		if(pData->m_bSepUpperAndLowerSlabRebar)
			sTitle	= bUpper ? "평면도(상면)" : "평면도(하면)";


		if(dOptDraw & REBAR_UPPER_SLAB)
		{
			if(m_pStd->IsDrawByBlock(MPE_REBAR_UPPERSLAB+upper))
			{
 				pPageMng->SetProgressData(sTitle + " 출력중...", 30);
				if(pData->m_bSepUpperAndLowerSlabRebar)
					rcUpperSlab[upper] = DBStd.m_pDrawPlane->DrawRebar_Plane_UpperSlabStd(&Dom, -1, bUpper, bUpper, TRUE, TRUE, TRUE, pData->m_nDirMarkUpperSlabMainRebar, FALSE, !pData->m_bDrawHunchRebarOnly);
				else
					rcUpperSlab[upper] = DBStd.m_pDrawPlane->DrawRebar_Plane_UpperSlabStd(&Dom, -1, TRUE, FALSE, TRUE, TRUE, TRUE, pData->m_nDirMarkUpperSlabMainRebar, FALSE, !pData->m_bDrawHunchRebarOnly);
				if(bUpper)
					xyMove		= CDPoint(rcDogak.left-rcUpperSlab[upper].left+Dom.Always(300), rcDogak.bottom-rcUpperSlab[upper].bottom-Dom.Always(150)); 
				else
					xyMove		= CDPoint(rcDogak.left-rcUpperSlab[upper].left+Dom.Always(300), rcUpperSlab[iUPPER].bottom-rcUpperSlab[upper].top+Dom.Always(50)); 
				Dom.Move(xyMove);
				rcUpperSlab[upper].left	+= xyMove.x; rcUpperSlab[upper].right	+= xyMove.x;
				rcUpperSlab[upper].top		+= xyMove.y; rcUpperSlab[upper].bottom	+= xyMove.y;
				Dom.SetCalcExtRect();
				rect	= Dom.GetExtRect();
				
				Dom.Move(pMove->GetPoint(MPE_REBAR_UPPERSLAB+upper));
				pStd->ChangeSeperateLayer(&Dom);
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_UPPERSLAB+upper, sTitle);
				*pDomP << Dom;
			}

			if(m_pStd->IsDrawByBlock(MPE_REBAR_UPPERSLAB_TITLE+upper))
			{
				CString sTltieOnDom;
				if(pData->m_bSepUpperAndLowerSlabRebar)
					sTltieOnDom = bUpper ? "상부슬래브(상면) 구조도" : "상부슬래브(하면) 구조도";
				else
					sTltieOnDom = "상부슬래브 구조도";
				pStd->AddTitleOnDom(&Dom, m_nBridge, _T(sTltieOnDom), CDPoint(0,0), 0, dScale);
				Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
				Dom.Move(pMove->GetPoint(MPE_REBAR_UPPERSLAB_TITLE+upper));
				pStd->ChangeSeperateLayer(&Dom);
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_UPPERSLAB_TITLE+upper, sTitle + " 타이틀");
				*pDomP << Dom;
			}
		}
		else
		{
			rcUpperSlab[upper].left	= rcDogak.left+Dom.Always(300);
			rcUpperSlab[upper].right	= rcUpperSlab[upper].left;
			rcUpperSlab[upper].bottom	= rcDogak.bottom - Dom.Always(150);
			rcUpperSlab[upper].top		= rcUpperSlab[upper].bottom;
		}
	}

	// 헌치 철근 구조도 따로 출력
	if(pData->m_bDrawHunchRebarOnly)
	{
		CString sTitle	= "상부슬래브 헌치부 상세";
		if(m_pStd->IsDrawByBlock(MPE_REBAR_UPPER_HUNCH))
		{
			pPageMng->SetProgressData(sTitle + " 출력중...", 30);
			
			CDRect rcHunch = DBStd.m_pDrawPlane->DrawRebar_Plane_SlabHunch(&Dom, TRUE, TRUE, TRUE, TRUE, pData->m_nDirMarkUpperSlabMainRebar);
			
			xyMove		= CDPoint(rcDogak.right-rcHunch.left+Dom.Always(300), rcDogak.bottom-rcHunch.bottom-Dom.Always(150)); 
			
			Dom.Move(xyMove);
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_REBAR_UPPER_HUNCH));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_UPPER_HUNCH, sTitle);
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_REBAR_UPPER_HUNCH_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, sTitle, CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_REBAR_UPPER_HUNCH_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_UPPER_HUNCH_TITLE, sTitle + " 타이틀");
			*pDomP << Dom;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////

	// 횡단면도 구조도 조립도 //////////////////////////////////////////////////////////////////////
	CDRect rcCrossSection(0, 0, 0, 0);	// 주철근 조립도 영역 영역
	if(dOptDraw & REBAR_CROSSSECTION)
	{
		BOOL bExcludeDoupleMainRebar	= pData->m_bExcludeDoupleMainRebarCrossSection;

		double dStaLeft = pBri->GetStationOnJijum(0);
		double dStaRight = (pBri->GetStationOnJijum(0, 0)+pBri->GetStationOnJijum(1, 0))/2;
		if(!pBri->IsOutWall(TRUE))
			dStaLeft	= pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, FALSE);

		if(m_pStd->IsDrawByBlock(MPE_REBAR_CROSSECTION))
		{
			pPageMng->SetProgressData(_T("횡단면도 출력중..."), 60);

			// 확폭인경우..
			if(pBri->IsExtLine())
			{
				rcCrossSection	= DBStd.m_pDrawCrossSection->DrawRebar_CrossSectionStd(&Dom, dStaLeft, TRUE, TRUE, TRUE, TRUE, TRUE, bExcludeDoupleMainRebar);
			}
			else
			{
				rcCrossSection	= DBStd.m_pDrawCrossSection->DrawRebar_CrossSectionStd(&Dom, dStaLeft, dStaRight, TRUE, TRUE, TRUE, TRUE, TRUE, bExcludeDoupleMainRebar);
			}
			xyMove			= CDPoint(rcUpperSlab[iUPPER].left-rcCrossSection.right-Dom.Always(100), rcUpperSlab[iUPPER].top-rcCrossSection.top); 
			if(pBri->m_bIsExp)
			{
				double dDiff	= max(pBri->m_dExpSttH[1], pBri->m_dExpEndH[1]);
				xyMove.y += dDiff;
			}
			Dom.Move(xyMove);
			rcCrossSection.left	+= xyMove.x; rcCrossSection.right	+= xyMove.x;
			rcCrossSection.top	+= xyMove.y; rcCrossSection.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();
			
			Dom.Move(pMove->GetPoint(MPE_REBAR_CROSSECTION));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_CROSSECTION, _T("횡단면도"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_REBAR_CROSSECTION_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("횡단면도"), CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_REBAR_CROSSECTION_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_CROSSECTION_TITLE, _T("횡단면도 타이틀"));
			*pDomP << Dom;
		}
	}
	else
	{	
		rcCrossSection.right = rcUpperSlab[iUPPER].left - Dom.Always(100);
		rcCrossSection.bottom	= rcUpperSlab[iUPPER].top;
		rcCrossSection.left	= rcCrossSection.right;
		rcCrossSection.top	= rcCrossSection.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////

	// 확폭인 경우 종점측 횡단면도도 그려준다.
	if(pBri->IsExtLine())
	{
		long nAddCross	= 1;
		CDRect rcCrossSectionEnd(0, 0, 0, 0);
		if(dOptDraw & REBAR_CROSSSECTION)
		{
			BOOL bExcludeDoupleMainRebar	= pData->m_bExcludeDoupleMainRebarCrossSection;

			double dStaLeft = pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, FALSE);

			//#0031385 dStaLeft과 철근의 단면과 교차점을 찾게된다. 그러나 철근은 90도 각도로 임의로 그려져 있기 때문에 교차점을 찾기 위하여 스큐가 적용된 각도만큼 보정해준다.
			// (ARCBRIDGE-2735) 시점은 그냥 시점Station에서 그리는데 왜?
			// 그냥 종점 Station에서 리자.
			dStaLeft = pBri->m_dStationBridgeStt + pBri->m_dLengthBridge;

// 			if(dStaLeft > pBri->m_dStationBridgeStt + pBri->m_dLengthBridge)
// 			{
// 				CDPoint vAng = pBri->GetJijum(pBri->m_nQtyJigan)->m_vAngle;
// 
// 				dStaLeft = pBri->m_dStationBridgeStt + (pBri->m_dLengthBridge * vAng.y);
// 			}

			if(m_pStd->IsDrawByBlock(MPE_REBAR_CROSSECTION+nAddCross))
			{
				pPageMng->SetProgressData(_T("횡단면도 출력중..."), 60);

				// 확폭인경우..
				rcCrossSectionEnd	= DBStd.m_pDrawCrossSection->DrawRebar_CrossSectionStd(&Dom, dStaLeft, TRUE, TRUE, TRUE, TRUE, TRUE, bExcludeDoupleMainRebar);
				xyMove			= CDPoint(rcUpperSlab[iUPPER].right-rcCrossSectionEnd.right+Dom.Always(200), rcUpperSlab[iUPPER].top-rcCrossSectionEnd.top); 
				Dom.Move(xyMove);
				rcCrossSectionEnd.left	+= xyMove.x; rcCrossSectionEnd.right	+= xyMove.x;
				rcCrossSectionEnd.top	+= xyMove.y; rcCrossSectionEnd.bottom	+= xyMove.y;
				Dom.SetCalcExtRect();
				rect	= Dom.GetExtRect();
				
				Dom.Move(pMove->GetPoint(MPE_REBAR_CROSSECTION+nAddCross));
				pStd->ChangeSeperateLayer(&Dom);
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_CROSSECTION+nAddCross, _T("횡단면도"));
				*pDomP << Dom;
			}

			if(m_pStd->IsDrawByBlock(MPE_REBAR_CROSSECTION_TITLE+nAddCross))
			{
				pStd->AddTitleOnDom(&Dom, m_nBridge, _T("횡단면도(종점슬래브끝단)"), CDPoint(0,0), 0, dScale);
				Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
				Dom.Move(pMove->GetPoint(MPE_REBAR_CROSSECTION_TITLE+nAddCross));
				pStd->ChangeSeperateLayer(&Dom);
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_CROSSECTION_TITLE+nAddCross, _T("횡단면도 타이틀"));
				*pDomP << Dom;
			}
		}

		// 중앙부(헌치 없는 부분)
		nAddCross++;
		CDRect rcCrossSectionCen(0, 0, 0, 0);
		if(dOptDraw & REBAR_CROSSSECTION)
		{
			BOOL bExcludeDoupleMainRebar	= pData->m_bExcludeDoupleMainRebarCrossSection;

			double dStaLeft = pBri->GetStationOnJijum(0, 0) + pBri->GetLengthJigan(0)/2;
			if(m_pStd->IsDrawByBlock(MPE_REBAR_CROSSECTION+nAddCross))
			{
				pPageMng->SetProgressData(_T("횡단면도 출력중..."), 60);

				// 확폭인경우..
				rcCrossSectionCen	= DBStd.m_pDrawCrossSection->DrawRebar_CrossSectionStd(&Dom, dStaLeft, TRUE, TRUE, TRUE, TRUE, TRUE, bExcludeDoupleMainRebar);
				xyMove			= CDPoint(rcCrossSectionEnd.right-rcCrossSectionCen.right+Dom.Always(150), rcUpperSlab[iUPPER].top-rcCrossSectionCen.top); 
				Dom.Move(xyMove);
				rcCrossSectionCen.left	+= xyMove.x; rcCrossSectionCen.right	+= xyMove.x;
				rcCrossSectionCen.top	+= xyMove.y; rcCrossSectionCen.bottom	+= xyMove.y;
				Dom.SetCalcExtRect();
				rect	= Dom.GetExtRect();
				
				Dom.Move(pMove->GetPoint(MPE_REBAR_CROSSECTION+nAddCross));
				pStd->ChangeSeperateLayer(&Dom);
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_CROSSECTION+nAddCross, _T("횡단면도"));
				*pDomP << Dom;
			}

			if(m_pStd->IsDrawByBlock(MPE_REBAR_CROSSECTION_TITLE+nAddCross))
			{
				pStd->AddTitleOnDom(&Dom, m_nBridge, _T("횡단면도(지간1중앙부)"), CDPoint(0,0), 0, dScale);
				Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
				Dom.Move(pMove->GetPoint(MPE_REBAR_CROSSECTION_TITLE+nAddCross));
				pStd->ChangeSeperateLayer(&Dom);
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_CROSSECTION_TITLE+nAddCross, _T("횡단면도 타이틀"));
				*pDomP << Dom;
			}
		}
	}

	// 보기둥 단면도에서 표현이 불가능한 거더 상세는 여기서 출력한다.
	CDPoint xyLast	= CDPoint(rcUpperSlab[iUPPER].left, rcUpperSlab[iUPPER].top-Dom.Always(150));
	CDRect rcGirder(xyLast.x, xyLast.y, xyLast.x, xyLast.y);	// 거더 단면도
	long nIdxGirder	= 0;
	for(long jijum = 0; jijum < pBri->GetCountJijum(); jijum++)
	{
		if(!pBri->IsGirderRebar(jijum, TRUE)) continue;
		if((jijum != 0 && jijum != pBri->m_nQtyJigan) && pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB) continue;

		// 기둥평면도 오른쪽에 거더 단면도 넣는다
		// 거더 단면도 //////////////////////////////////////////////////////////////
		double dScaleDetail	= pData->m_dScaleRebarDetailGirderSection;
		Dom.SetScaleDim(dScaleDetail);

		
		//pBri->SyncBridge(0, TRUE);
		pBri->GetTvVertSection_Std(0, TRUE, FALSE);
		if(dOptDraw & REBAR_GIRDER_SECT && pBri->IsGirderRebar(jijum, TRUE))
		{
			if(m_pStd->IsDrawByBlock(MPE_REBAR_GIRDERSEC+nIdxGirder))
			{
				pPageMng->SetProgressData(_T("거더 단면도 출력중..."), 100);
				DBStd.m_pDrawVertSection->DrawRebarAll_SlabGirder(&Dom, jijum, TRUE, TRUE, -1, FALSE, FALSE);
 				DBStd.m_pDrawVertSection->DrawDetail_Girder(&Dom, jijum);
				Dom.RedrawByScale(dScale/dScaleDetail);
				Dom.SetCalcExtRect();
				rcGirder	= Dom.GetExtRect();
				xyMove		= CDPoint(xyLast.x-rcGirder.left+Dom.Always(150), xyLast.y-rcGirder.bottom); 
				Dom.Move(xyMove);
				rcGirder.left	+= xyMove.x; rcGirder.right		+= xyMove.x;
				rcGirder.top	+= xyMove.y; rcGirder.bottom	+= xyMove.y;
				Dom.SetCalcExtRect();
				rect	= Dom.GetExtRect();
				
				Dom.SetCalcExtRect();
				Dom.Move(pMove->GetPoint(MPE_REBAR_GIRDERSEC+nIdxGirder));
				pStd->ChangeSeperateLayer(&Dom);
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_GIRDERSEC+nIdxGirder, _T("거더 단면도"));
				*pDomP << Dom;
			}

			if(m_pStd->IsDrawByBlock(MPE_REBAR_GIRDERSEC_TITLE+nIdxGirder))
			{
				pStd->AddTitleOnDom(&Dom, m_nBridge, _T("거더 단면도"), CDPoint(0,0), 0, dScaleDetail);
				Dom.RedrawByScale(dScale/dScaleDetail);
				Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
				Dom.Move(pMove->GetPoint(MPE_REBAR_GIRDERSEC_TITLE+nIdxGirder));
				pStd->ChangeSeperateLayer(&Dom);
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_GIRDERSEC_TITLE+nIdxGirder, _T("거더 단면도 타이틀"));
				*pDomP << Dom;
			}
			nIdxGirder++;
		}
		else
		{
			rcGirder.left		= xyLast.x + Dom.Always(150);
			rcGirder.bottom		= xyLast.y;
			rcGirder.right		= rcGirder.left;
			rcGirder.top		= rcGirder.bottom;
		}
		xyLast.x	= rcGirder.right;
		xyLast.y	= rcGirder.bottom;

		pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE);
		/////////////////////////////////////////////////////////////////////////////
	}


	// 둔각부 보강 상세
	
	xyLast	= CDPoint(rcUpperSlab[iUPPER].left, rcGirder.top - Dom.Always(70));
	long nIdx	= 0;
	for(long stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;

		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT;

			if(!pBri->m_bIsObtuseAngle_UpperSlab[stt][left]) continue;

			long nMPE	= MPE_DETAIL_OBTUSE_STT_LEFT;
			long nMPETitle	= MPE_DETAIL_OBTUSE_STT_LEFT_TITLE;
			CString sTitle	= _T("");
			CDRect rectObtuse[2];
			rectObtuse[0]	= CDRect(0, 0, 0, 0);
			rectObtuse[1]	= CDRect(0, 0, 0, 0);
			for(long upper = 0; upper < 2; upper++)
			{
				BOOL bUpper	= upper == iUPPER;

				CString sUpper	= bUpper ? "(상면)" : "(하면)";
				if(bStt)
				{
					nMPE		= bLeft ? MPE_DETAIL_OBTUSE_STT_LEFT+upper : MPE_DETAIL_OBTUSE_STT_RIGHT+upper;
					nMPETitle	= bLeft ? MPE_DETAIL_OBTUSE_STT_LEFT_TITLE+upper : MPE_DETAIL_OBTUSE_STT_RIGHT_TITLE+upper;
					sTitle		= bLeft ? "둔각부(시점좌측) 보강 상세"+sUpper : "둔각부(시점우측) 보강 상세"+sUpper;
				}
				else
				{
					nMPE		= bLeft ? MPE_DETAIL_OBTUSE_END_LEFT+upper: MPE_DETAIL_OBTUSE_END_RIGHT+upper;
					nMPETitle	= bLeft ? MPE_DETAIL_OBTUSE_END_LEFT_TITLE+upper : MPE_DETAIL_OBTUSE_END_RIGHT_TITLE+upper;
					sTitle		= bLeft ? "둔각부(종점좌측) 보강 상세"+sUpper : "둔각부(종점우측) 보강 상세"+sUpper;
				}

				nIdx++;
				double dScaleDetail	= pData->m_dScaleRebarDetailSlabAcute;
				Dom.SetScaleDim(dScaleDetail);

				if(m_pStd->IsDrawByBlock(nMPE))
				{
					pPageMng->SetProgressData(sTitle+" 출력중...", 60+((nIdx+1)*5));
					DBStd.m_pDrawPlane->DrawPlaneGenObtuse(&Dom, bStt, bLeft, bUpper);
					DBStd.m_pDrawPlane->DrawRebarPlane_Slab_Obtuse(&Dom, TRUE, bStt, bLeft, bUpper, TRUE, TRUE, FALSE);
					Dom.RedrawByScale(dScale/dScaleDetail);

					// 회전
					CDPoint vAng	= pBri->GetAngleAzimuthJijum(bStt ? 0 : pBri->m_nQtyJigan);
					Dom.Rotate(CDPoint(0, 0), ToDPointFrDegree(90-vAng.GetAngleDegree()+180));

					
					Dom.SetCalcExtRect();
					rectObtuse[upper]	= Dom.GetExtRect();

					if(bUpper)
						xyMove	= CDPoint(xyLast.x - rectObtuse[upper].left, xyLast.y - rectObtuse[upper].bottom);
					else
						xyMove	= CDPoint(xyLast.x - rectObtuse[upper].left, rectObtuse[iUPPER].top - rectObtuse[upper].bottom);
					Dom.Move(xyMove);
					rectObtuse[upper].left	+= xyMove.x; rectObtuse[upper].right	+= xyMove.x;
					rectObtuse[upper].top	+= xyMove.y; rectObtuse[upper].bottom	+= xyMove.y;
					Dom.SetCalcExtRect();
					rect	= Dom.GetExtRect();
					
					Dom.Move(pMove->GetPoint(nMPE));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), nMPE, sTitle);
					*pDomP << Dom;	
				}


				if(m_pStd->IsDrawByBlock(nMPETitle))
				{
					pStd->AddTitleOnDom(&Dom, m_nBridge, sTitle, CDPoint(0,0), 0, dScaleDetail);
					Dom.RedrawByScale(dScale/dScaleDetail);
					Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
					Dom.Move(pMove->GetPoint(nMPETitle));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), nMPETitle, sTitle + " 타이틀");
					*pDomP << Dom;
				}
			}

			// 종단면 상세
			if(bStt)
			{
				nMPE		= bLeft ? MPE_DETAIL_OBTUSE_STT_LEFT+2 : MPE_DETAIL_OBTUSE_STT_RIGHT+2;
				nMPETitle	= bLeft ? MPE_DETAIL_OBTUSE_STT_LEFT_TITLE+2 : MPE_DETAIL_OBTUSE_STT_RIGHT_TITLE+2;
				sTitle		= bLeft ? "둔각부(시점좌측) 보강 상세(종단면)" : "둔각부(시점우측) 보강 상세(종단면)";
			}
			else
			{
				nMPE		= bLeft ? MPE_DETAIL_OBTUSE_END_LEFT+2: MPE_DETAIL_OBTUSE_END_RIGHT+2;
				nMPETitle	= bLeft ? MPE_DETAIL_OBTUSE_END_LEFT_TITLE+2 : MPE_DETAIL_OBTUSE_END_RIGHT_TITLE+2;
				sTitle		= bLeft ? "둔각부(종점좌측) 보강 상세(종단면)" : "둔각부(종점우측) 보강 상세(종단면)";
			}

			CDRect rectObtuseVert	= rectObtuse[iLOWER];
			double dScaleDetail	= pData->m_dScaleRebarDetailSlabAcute;
			if(m_pStd->IsDrawByBlock(nMPE))
			{
				DBStd.m_pDrawVertSection->DrawRebarVertSectionObtuseStd(&Dom, bStt, bLeft, TRUE, TRUE);
				Dom.RedrawByScale(dScale/dScaleDetail);

				Dom.SetCalcExtRect();
				rectObtuseVert	= Dom.GetExtRect();
				xyMove	= CDPoint(rectObtuse[iLOWER].left - rectObtuseVert.left, rectObtuse[iLOWER].top - rectObtuseVert.bottom - Dom.Always(20));
				Dom.Move(xyMove);
				rectObtuseVert.left	+= xyMove.x; rectObtuseVert.right	+= xyMove.x;
				rectObtuseVert.top	+= xyMove.y; rectObtuseVert.bottom	+= xyMove.y;
				Dom.SetCalcExtRect();
				rect	= Dom.GetExtRect();

				Dom.Move(pMove->GetPoint(nMPE));
				pStd->ChangeSeperateLayer(&Dom);
				pMove->AddMPE(&Dom, Dom.GetExtRect(), nMPE, sTitle);
				*pDomP << Dom;	
			}

			if(m_pStd->IsDrawByBlock(nMPETitle))
			{
				pStd->AddTitleOnDom(&Dom, m_nBridge, sTitle, CDPoint(0,0), 0, dScaleDetail);
				Dom.RedrawByScale(dScale/dScaleDetail);
				Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
				Dom.Move(pMove->GetPoint(nMPETitle));
				pStd->ChangeSeperateLayer(&Dom);
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom, Dom.GetExtRect(), nMPETitle, sTitle + " 타이틀");
				*pDomP << Dom;			
				xyLast.x	= Dom.GetExtRect().right + Dom.Always(20);
			}
		}
	}

	// 종거더 상세도
	long nMPE		= MPE_DETAIL_JONGGIRDER - 1;
	long nMPETitle	= MPE_DETAIL_JONGGIRDER_TITLE - 1;
	for(long gagak = 0; gagak < 2; gagak++)
	{
		BOOL bGagak	= gagak == 0;
		CString sGagak	= bGagak ? "가각부" : "";
		for(long stt = 0; stt < 2;  stt++)
		{
			BOOL bStt	= stt == iSTT;
			CString sStt	= bGagak ? (bStt ? "시점" : "종점") : "";

			for(long left = 0; left < 2; left++)
			{
				BOOL bLeft	= left == iLEFT;

				nMPE++;
				nMPETitle++;

				if(!bGagak && !bStt) continue;
				if(!pBri->IsGirderRebarJong(bStt, bLeft, bGagak)) continue;
				CString sLeft	= bLeft ? "좌측 종거더 상세도" : "우측 종거더 상세도";
				CString sTitle	= sGagak + sStt + sLeft;

				double dScaleDetail	= pData->m_dScaleRebarDetailGirderSection;
				Dom.SetScaleDim(dScaleDetail);
				CDRect rectJong;
				if(m_pStd->IsDrawByBlock(nMPE))
				{
					pPageMng->SetProgressData(sTitle + _T(" 출력중..."), 100);
		
					DBStd.m_pDrawVertSection->DrawDetail_Girder(&Dom, 0, bStt, bLeft, bGagak, TRUE);
					// 스트럽 철근 관련 그리기
					DBStd.m_pDrawVertSection->DrawRebarAll_SlabGirder(&Dom, 0, TRUE, TRUE, 0, FALSE, FALSE, bStt, bLeft, bGagak, TRUE);
					Dom.RedrawByScale(dScale/dScaleDetail);
					Dom.SetCalcExtRect();
					rectJong	= Dom.GetExtRect();
					xyMove		= CDPoint(xyLast.x-rectJong.left+Dom.Always(150), xyLast.y-rectJong.bottom); 
					Dom.Move(xyMove);
					rectJong.left	+= xyMove.x; rectJong.right		+= xyMove.x;
					rectJong.top	+= xyMove.y; rectJong.bottom	+= xyMove.y;
					Dom.SetCalcExtRect();
					rect	= Dom.GetExtRect();
					
					Dom.SetCalcExtRect();
					Dom.Move(pMove->GetPoint(nMPE));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), nMPE, sTitle);
					*pDomP << Dom;
				}

				if(m_pStd->IsDrawByBlock(nMPETitle))
				{
					pStd->AddTitleOnDom(&Dom, m_nBridge, sTitle, CDPoint(0,0), 0, dScaleDetail);
					Dom.RedrawByScale(dScale/dScaleDetail);
					Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
					Dom.Move(pMove->GetPoint(nMPETitle));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), nMPETitle, sTitle + " 타이틀");
					*pDomP << Dom;

					xyLast	= CDPoint(rectJong.right, rectJong.bottom);
				}
			}
		}
	}

	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE);
}

// 중간벽체 구조도 추가.
// 기둥식인 경우 재료표를 출력해야 될 경우도 있다.
// BOOL bFrontOfRebarBMMain : 전체철근재료표 앞인지 뒤인지?
void CDrawingRebar::AddPageInWallRebar(CDomyun *pDomP, long nIdxInWallRebarPage, CDRect rcDogak, BOOL bFrontOfRebarBMMain)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;
	double dScale	= GetScale();

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScale);
	DWORD dOptDraw	= pData->m_dwDrawRebar;

	CString str	= _T("");
	// 해당되는 중간벽체 가져 온다. /////////////////////////////////////////////////////////////////////////////
	// 단, nIdxInWallRebarPage는 중간벽체중에 기둥식이 아닌것의 인덱스이므로 실제 벽체 인덱스와 다를 수 있다.
	// 주의 해서 가져와야 됨.
	CWallApp *pWall			= NULL;
	long nIdxInWall			= 0;
	long nJ					= 0;
	BOOL bRebarBM			= FALSE;
	BOOL bSeparateBMColumn	= pData->m_bSeparateRebarBMColumn;

	long nIdx				= 0;
	long i = 0; for(i = 0; i < pBri->GetCountInWall(); i++)
	{
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		{
			if(i+1 == pBri->GetJijumConnectRibAndSlab())
				continue;
		}
		CWallApp *pWallTmp	= pBri->GetInWall(i);
		if(pWallTmp)
		{
			if(!pWallTmp->m_bIs)
			{
				nIdx++;
				continue;
			}

			if(bFrontOfRebarBMMain)
			{
				if(bSeparateBMColumn && pWallTmp->m_bTopIsColumn)
					continue;
			}
			else
			{
				if(!bSeparateBMColumn || !pWallTmp->m_bTopIsColumn)
					continue;
			}

			if(pWallTmp->m_bIs && nIdx==nIdxInWallRebarPage)
			{
				pWall		= pWallTmp;
				nIdxInWall	= i;
				break;
			}
			else
				nIdx++;
			
			// 기둥식의 재료표 출력해야 되는지도 체크
			if(bSeparateBMColumn && pWallTmp->m_bTopIsColumn)
			{
				if(nIdx==nIdxInWallRebarPage)
				{
					bRebarBM	= TRUE;
					pWall		= pWallTmp;
					nIdxInWall	= i;
					break;
				}
				else
					nIdx++;
			}
		}
	}
	
	if(!pWall) return;
	nJ	= nIdxInWall + 1;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 기둥식의 재료표 출력용인지 판단해서 재료표 출력 //////////////////////
	if(bRebarBM)
	{
		AddPageRebarBM(pDomP, rcDogak, 2, TRUE, TRUE, nIdxInWall);
		return;
	}
	//////////////////////////////////////////////////////////////////////////

	long nCountDom	= 4;
	double dAddPer	= 100 / nCountDom;
	double dCurPer	= 0;

	
	CDPoint xyMove(0, 0);
	CDRect rect(0, 0, 0, 0);
	BOOL bVertDir	= FALSE;//pBri->IsVertDir(); // 벽체정면도는 항상 사로 그림.
	// pBri->SyncBridge(0, !bVertDir, FALSE);
	pBri->GetTvVertSection_Std(0, !bVertDir, FALSE);
	pBri->SyncBridgeRebar(FALSE);

	BOOL bIsDivideFrontAndBackWall	= DBStd.IsDivideFrontAndBackWall(nJ);
	// 중간벽체 구조도 조립도 //////////////////////////////////////////////////////////////////////
	CDRect rcInWall(0, 0, 0, 0);	// 중간벽체 영역
	if(dOptDraw & REBAR_INWALL_FRONT)
	{
		if(m_pStd->IsDrawByBlock(MPE_REBAR_WALL))
		{
			pPageMng->SetProgressData(_T("중간벽체 구조도 출력중..."), (int)(dCurPer+=dAddPer));
			BOOL bLeftAtLeft	= TRUE;	//bIsDivideFrontAndBackWall ? TRUE : TRUE;
			BOOL bLeftAtRight	= bIsDivideFrontAndBackWall ? TRUE : FALSE;
			CString sLeft		= bLeftAtLeft ? "좌측" : "우측";
			CString sRight		= bLeftAtRight ? "좌측" : "우측";
			rcInWall	= DBStd.m_pDrawCrossSection->DrawRebar_InWall_SideStd(&Dom, nJ, bLeftAtLeft, bLeftAtRight, TRUE, TRUE, sLeft, sRight, TRUE);
			if(!pWall->IsVertWall())
			{
				pStd->DrawNoteBottomOfDomyun(&Dom, "()안의 세로치수는 벽체 경사를 고려하지 않음 수직거리임.");
			}
	
			xyMove		= CDPoint(rcDogak.left-rcInWall.left+Dom.Always(100), rcDogak.bottom-rcInWall.bottom-Dom.Always(100)); 
			Dom.Move(xyMove);
			rcInWall.left	+= xyMove.x; rcInWall.right		+= xyMove.x;
			rcInWall.top	+= xyMove.y; rcInWall.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();
			
			Dom.Move(pMove->GetPoint(MPE_REBAR_WALL));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_WALL, _T("중간벽체 구조도"));
			*pDomP << Dom;
		}
	
		if(m_pStd->IsDrawByBlock(MPE_REBAR_WALL_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("중간벽체 구조도"), CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_REBAR_WALL_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_WALL_TITLE, _T("중간벽체 구조도 타이틀"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcInWall.left	= rcDogak.left+Dom.Always(100);
		rcInWall.bottom	= rcDogak.bottom-Dom.Always(100);
		rcInWall.right	= rcInWall.left;
		rcInWall.top	= rcInWall.bottom;
	}


	// 경사교일 경우 배면(우측면)을 출력 하는 경우도 있음
	CDRect rcInWall_Back(0, 0, 0, 0);	// 중간벽체 영역
	if(dOptDraw & REBAR_INWALL_FRONT && bIsDivideFrontAndBackWall)
	{
		if(m_pStd->IsDrawByBlock(MPE_REBAR_WALL_RIGHT))
		{
			pPageMng->SetProgressData(_T("중간벽체 구조도 출력중..."), (int)(dCurPer+=dAddPer));
			rcInWall_Back	= DBStd.m_pDrawCrossSection->DrawRebar_InWall_SideStd(&Dom, nJ, FALSE, FALSE, TRUE, TRUE, "우측", "우측", TRUE);
			if(!pWall->IsVertWall())
			{
				pStd->DrawNoteBottomOfDomyun(&Dom, "()안치 세로치수는 벽체 경사를 고려하지 않음 수직거리임.");
			}
	
	
			xyMove		= CDPoint(rcInWall.right-rcInWall_Back.left+Dom.Always(70), rcDogak.bottom-rcInWall_Back.bottom-Dom.Always(100)); 
			Dom.Move(xyMove);
			rcInWall_Back.left	+= xyMove.x; rcInWall_Back.right	+= xyMove.x;
			rcInWall_Back.top	+= xyMove.y; rcInWall_Back.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();
			
			Dom.Move(pMove->GetPoint(MPE_REBAR_WALL_RIGHT));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_WALL_RIGHT, _T("중간벽체 구조도"));
			*pDomP << Dom;
		}
	
		if(m_pStd->IsDrawByBlock(MPE_REBAR_WALL_TITLE_RIGHT))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("중간벽체 구조도"), CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_REBAR_WALL_TITLE_RIGHT));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_WALL_TITLE_RIGHT, _T("중간벽체 구조도 타이틀"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcInWall_Back	 = rcInWall;
	}
	pBri->GetTvVertSection_Std(0, !bVertDir, FALSE, TRUE);
	////////////////////////////////////////////////////////////////////////////////////

	// 중간벽체 단면A-A 구조도 조립도 //////////////////////////////////////////////////////////////////////
	CDRect rcInWallSec(0, 0, 0, 0);	// 중간벽체 단면A-A 영역
	if(dOptDraw & REBAR_INWALL_SECT)
	{
		bVertDir	= pBri->IsVertDir();
		//pBri->SyncBridge(0, !bVertDir);
		pBri->GetTvVertSection_Std(0, !bVertDir, FALSE);
		pBri->SyncBridgeRebar(FALSE, bVertDir?-1:1, TRUE);
	
		if(m_pStd->IsDrawByBlock(MPE_REBAR_WALLSEC))
		{
			pPageMng->SetProgressData(_T("단면 A-A 출력중..."), (int)(dCurPer+=dAddPer));
			rcInWallSec = DBStd.m_pDrawVertSection->DrawRebar_InWall_VertStd(&Dom, pWall, TRUE, 0);
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
				xyMove		= CDPoint(rcInWall_Back.right-rcInWallSec.left+Dom.Always(150), (rcInWall_Back.bottom-rcInWall.Height()/2)-(rcInWallSec.bottom-rcInWallSec.Height()/2)); 
			else
				xyMove		= CDPoint(rcInWall_Back.right-rcInWallSec.left+Dom.Always(150), rcInWall_Back.top-rcInWallSec.top); 
			Dom.Move(xyMove);
			rcInWallSec.left	+= xyMove.x; rcInWallSec.right		+= xyMove.x;
			rcInWallSec.top		+= xyMove.y; rcInWallSec.bottom		+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();
	
			Dom.Move(pMove->GetPoint(MPE_REBAR_WALLSEC));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_WALLSEC, _T("단면 A-A"));
			*pDomP << Dom;
		}
	
		if(m_pStd->IsDrawByBlock(MPE_REBAR_WALLSEC_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("단면 A-A"), CDPoint(0,0), 0, dScale);
			Dom.Move(rcInWallSec.left+rcInWallSec.Width()/2.0, rcInWallSec.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_REBAR_WALLSEC_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_WALLSEC_TITLE, _T("단면 A-A 타이틀"));
			*pDomP << Dom;
		}
		pBri->GetTvVertSection_Std(0, !bVertDir, FALSE, TRUE);
	}
	else
	{
		rcInWallSec.left	= rcInWall_Back.right+Dom.Always(150);
		rcInWallSec.bottom	= rcInWall_Back.bottom;
		rcInWallSec.right	= rcInWallSec.left;
		rcInWallSec.top		= rcInWallSec.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////
	
	// 기초평면도 //////////////////////////////////////////////////////////////////////
	// 기초 평면도는 무조건 직방향으로 철근을 입력받으므로 직거리 싱크를 한번 한다.
	bVertDir	= TRUE;
	//pBri->SyncBridge(0, !bVertDir);
	pBri->GetTvVertSection_Std(0, !bVertDir, FALSE);
	pBri->SyncBridgeRebar(FALSE, bVertDir?-1:1, TRUE);
	
	CFootingApp *pFooting	= &pWall->m_footing;
	CDRect rcFooting[2]	= {(0, 0, 0, 0), (0,0,0,0)};	// 기초평면도
	for(long upper = 0; upper < 2; upper++)
	{
		BOOL bUpper	= upper == iUPPER;
	
		if(pBri->IsBoxType()) continue;
		if(!pData->m_bSepUpperAndLowerFootingRebar && !bUpper) continue;
		CString sTitle	= "기초평면도";
		if(pData->m_bSepUpperAndLowerFootingRebar)
			sTitle	= bUpper ? "기초평면도(상면)" : "기초평면도(하면)";
	
		if(dOptDraw & REBAR_FOOTING_PLANE && pFooting->m_bIs)
		{
			if(m_pStd->IsDrawByBlock(MPE_REBAR_FOOTINGPLANE+upper))
			{
				pPageMng->SetProgressData(sTitle + "출력중...", (int)(dCurPer+=dAddPer));
				// 벽체를 수평으로 놓이도록 회전
				DBStd.m_pDrawWall->DrawPlan(&Dom, pWall, FALSE, TRUE, -1);
				double dRotate	= 180-pFooting->m_tvPathPlan.GetXyDir().GetAngleDegree();
				Dom.Rotate(CDPoint(0, 0), ToDPointFrDegree(dRotate));
	
				if(pData->m_bSepUpperAndLowerFootingRebar)
					rcFooting[upper]	= DBStd.m_pDrawFooting->DrawRebar_PlaneStd(&Dom, pFooting, bUpper, bUpper, TRUE, TRUE, -1, FALSE, FALSE);
				else
					rcFooting[upper]	= DBStd.m_pDrawFooting->DrawRebar_PlaneStd(&Dom, pFooting, FALSE, TRUE, TRUE, TRUE, -1, FALSE, FALSE);
	
				if(bUpper)
					xyMove		= CDPoint(rcInWall.left-rcFooting[upper].left, rcInWall.top-rcFooting[upper].bottom-Dom.Always(80)); 
				else
					xyMove		= CDPoint(rcInWall.left-rcFooting[upper].left, rcFooting[iUPPER].top-rcFooting[upper].bottom-Dom.Always(80)); 
				Dom.Move(xyMove);
				rcFooting[upper].left		+= xyMove.x; rcFooting[upper].right		+= xyMove.x;
				rcFooting[upper].top		+= xyMove.y; rcFooting[upper].bottom		+= xyMove.y;
				Dom.SetCalcExtRect();
				rect	= Dom.GetExtRect();
				
				Dom.SetCalcExtRect();
				Dom.Move(pMove->GetPoint(MPE_REBAR_FOOTINGPLANE+upper));
				pStd->ChangeSeperateLayer(&Dom);
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_FOOTINGPLANE+upper,sTitle);
				*pDomP << Dom;
			}
	
			if(m_pStd->IsDrawByBlock(MPE_REBAR_FOOTINGPLANE_TITLE+upper))
			{
				pStd->AddTitleOnDom(&Dom, m_nBridge, sTitle, CDPoint(0,0), 0, dScale);
				Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
				Dom.Move(pMove->GetPoint(MPE_REBAR_FOOTINGPLANE_TITLE+upper));
				pStd->ChangeSeperateLayer(&Dom);
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_FOOTINGPLANE_TITLE+upper, sTitle +" 타이틀");
				*pDomP << Dom;
			}
		}
		else
		{
			if(bUpper)
			{
				rcFooting[upper].left		= rcInWallSec.left;
				rcFooting[upper].bottom	= rcInWallSec.top - Dom.Always(80);
				rcFooting[upper].right		= rcFooting[upper].left;
				rcFooting[upper].top		= rcFooting[upper].bottom;
			}
			else
			{
				rcFooting[upper].left		= rcInWallSec.left;
				rcFooting[upper].bottom	= rcFooting[iUPPER].top - Dom.Always(80);
				rcFooting[upper].right		= rcFooting[upper].left;
				rcFooting[upper].top		= rcFooting[upper].bottom;
			}
	
		}
	}

	pBri->GetTvVertSection_Std(0, !bVertDir, FALSE, TRUE);
	////////////////////////////////////////////////////////////////////////////////////
	double dScaleDetail	= pWall->m_bTopIsColumn? pData->m_dScaleRebarDetailColumnSection : pData->m_dScaleRebarBase;
	Dom.SetScaleDim(dScaleDetail);
	
	// 기초 오른쪽에 기둥평면도 넣는다.
	// 기둥평면도 ////////////////////////////////////////////////////////////
	// 기둥은 가운데 있는 기둥을 사용한다.
	CColumnApp *pColumn	= pWall->GetColumn(pWall->m_nCountColumn/2);
	CDRect rcColumn(0, 0, 0, 0);	// 기둥평면도
	if((dOptDraw & REBAR_COLUMN_PLANE) && (pWall->m_bTopIsColumn || pWall->IsRoundRectWall()))
	{
		if(m_pStd->IsDrawByBlock(MPE_REBAR_COLUMNPLANE))
		{
			pPageMng->SetProgressData(_T("단면 B-B 출력중..."), (int)(dCurPer+=dAddPer));
			DBStd.m_pDrawColumn->DrawRebar_PlaneStd(&Dom, pWall, TRUE, TRUE, TRUE, FALSE);
			Dom.RedrawByScale(dScale/dScaleDetail);
			Dom.SetCalcExtRect();
			rcColumn	= Dom.GetExtRect();
			if(pWall->IsRoundRectWall())
			{
				xyMove		= CDPoint(rcInWallSec.right-rcColumn.left+Dom.Always(150), rcInWallSec.bottom-rcColumn.bottom); 
			}
			else if(pColumn->m_footing.m_bIs)
				xyMove		= CDPoint(rcFooting[iUPPER].right-rcColumn.left+Dom.Always(150), rcFooting[iUPPER].top-rcColumn.top); 
			else
				xyMove		= CDPoint(rcInWall.left-rcColumn.left, rcInWall.top-rcColumn.bottom-Dom.Always(100));
			Dom.Move(xyMove);
			rcColumn.left	+= xyMove.x; rcColumn.right		+= xyMove.x;
			rcColumn.top	+= xyMove.y; rcColumn.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();
			
			Dom.SetCalcExtRect();
			Dom.Move(pMove->GetPoint(MPE_REBAR_COLUMNPLANE));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_COLUMNPLANE, _T("단면 B-B"));
			*pDomP << Dom;
		}
	
		if(m_pStd->IsDrawByBlock(MPE_REBAR_COLUMNPLANE_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("단면 B-B"), CDPoint(0,0), 0, dScaleDetail);
			Dom.RedrawByScale(dScale/dScaleDetail);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_REBAR_COLUMNPLANE_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_COLUMNPLANE_TITLE, _T("단면 B-B 타이틀"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcColumn.left		= rcFooting[iUPPER].right+Dom.Always(150);
		rcColumn.bottom		= rcFooting[iUPPER].top;
		rcColumn.right		= rcColumn.left;
		rcColumn.top		= rcColumn.bottom;
	}
	//////////////////////////////////////////////////////////////////////////
	
	// 기둥평면도 오른쪽에 거더 단면도 넣는다
	// 거더 단면도 //////////////////////////////////////////////////////////////
	dScaleDetail	= pData->m_dScaleRebarDetailGirderSection;
	Dom.SetScaleDim(dScaleDetail);
	
	CDRect rcGirder(0, 0, 0, 0), rcLowerGirder(0, 0, 0, 0);	// 거더 단면도
	//pBri->SyncBridge(0, TRUE);
	pBri->GetTvVertSection_Std(0, TRUE, FALSE);
	if(dOptDraw & REBAR_GIRDER_SECT && pBri->IsGirderRebar(nJ, TRUE))
	{
		//상부거더
		if(m_pStd->IsDrawByBlock(MPE_REBAR_GIRDERSEC))
		{
			pPageMng->SetProgressData(_T("거더 단면도 출력중..."), (int)(dCurPer+=dAddPer));
			DBStd.m_pDrawVertSection->DrawRebarAll_SlabGirder(&Dom, nJ, TRUE, TRUE, -1, FALSE, FALSE,TRUE, TRUE, TRUE, FALSE, -1, FALSE, TRUE, TRUE);
			rcGirder	= DBStd.m_pDrawVertSection->DrawDetail_Girder(&Dom, nJ);
			Dom.RedrawByScale(dScale/dScaleDetail);
			Dom.SetCalcExtRect();
			rcGirder	= Dom.GetExtRect();
			xyMove		= CDPoint(rcColumn.right-rcGirder.left+Dom.Always(150), rcColumn.top-rcGirder.top); 
			Dom.Move(xyMove);
			rcGirder.left	+= xyMove.x; rcGirder.right		+= xyMove.x;
			rcGirder.top	+= xyMove.y; rcGirder.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();
			
			Dom.SetCalcExtRect();
			Dom.Move(pMove->GetPoint(MPE_REBAR_GIRDERSEC));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_GIRDERSEC, _T("단면 C-C"));
			*pDomP << Dom;
		}
	
		if(m_pStd->IsDrawByBlock(MPE_REBAR_GIRDERSEC_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("단면 C-C"), CDPoint(0,0), 0, dScaleDetail);
			Dom.RedrawByScale(dScale/dScaleDetail);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_REBAR_GIRDERSEC_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_GIRDERSEC_TITLE, _T("거더 단면도 타이틀"));
			*pDomP << Dom;
		}
	
		//하부거더
		if(pBri->IsBoxType())
		{
			if(m_pStd->IsDrawByBlock(MPE_REBAR_GIRDERSEC))
			{
				DBStd.m_pDrawVertSection->DrawRebarAll_SlabGirder(&Dom, nJ, TRUE, TRUE, -1, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, -1, FALSE, FALSE, TRUE);
				rcLowerGirder	= DBStd.m_pDrawVertSection->DrawDetail_Girder(&Dom, nJ, TRUE, TRUE, TRUE, FALSE, FALSE);
				Dom.RedrawByScale(dScale/dScaleDetail);
				Dom.SetCalcExtRect();
				rcLowerGirder	= Dom.GetExtRect();
				xyMove		= CDPoint(rcColumn.right-rcLowerGirder.left+Dom.Always(150), rcGirder.top-rcLowerGirder.bottom-Dom.Always(100)); 
				Dom.Move(xyMove);
				rcLowerGirder.left	+= xyMove.x; rcLowerGirder.right	+= xyMove.x;
				rcLowerGirder.top	+= xyMove.y; rcLowerGirder.bottom	+= xyMove.y;
				Dom.SetCalcExtRect();
				rect	= Dom.GetExtRect();
	
				Dom.SetCalcExtRect();
				Dom.Move(pMove->GetPoint(MPE_REBAR_LOWER_GIRDERSEC));
				pStd->ChangeSeperateLayer(&Dom);
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_LOWER_GIRDERSEC, _T("단면 D-D"));
				*pDomP << Dom;
			}
			if(m_pStd->IsDrawByBlock(MPE_REBAR_GIRDERSEC_TITLE))
			{
				pStd->AddTitleOnDom(&Dom, m_nBridge, _T("단면 D-D"), CDPoint(0,0), 0, dScaleDetail);
				Dom.RedrawByScale(dScale/dScaleDetail);
				Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
				Dom.Move(pMove->GetPoint(MPE_REBAR_LOWER_GIRDERSEC_TITLE));
				pStd->ChangeSeperateLayer(&Dom);
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_LOWER_GIRDERSEC_TITLE, _T("거더 단면도 타이틀"));
				*pDomP << Dom;
			}
		}
	}
	else
	{
		rcGirder.left		= rcColumn.right+Dom.Always(150);
		rcGirder.bottom		= rcColumn.top;
		rcGirder.right		= rcGirder.left;
		rcGirder.top		= rcGirder.bottom;
	}

	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE);
	/////////////////////////////////////////////////////////////////////////////
}

void CDrawingRebar::AddPageOutWallRebar(CDomyun *pDomP, BOOL bStt, CDRect rcDogak)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);

	if(!pBri->IsOutWall(bStt)) return;

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;
	double dScale	= GetScale();
	double dScaleDetail	= dScale/2;

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScale);
	
	BOOL bVertDir	= pBri->IsVertDir();
	pBri->m_bSettingVertSection	= FALSE;
	pBri->SyncBridge(0, !bVertDir);
	//pBri->GetTvVertSection_Std(0, !bVertDir, FALSE);
	pBri->SyncBridgeRebar(FALSE, bVertDir?-1:1, TRUE);
	
	CString str		= _T("");

	long nCountDom	= 6;
	double dAddPer	= 100 / nCountDom;
	double dCurPer	= 0;

	CDPoint xyMove(0, 0);
	CDRect rect(0, 0, 0, 0);

	BOOL IsDivideFrontAndBackWall = DBStd.IsDivideFrontAndBackWall(bStt ? 0 : pBri->m_nQtyJigan);
	CString sWall	= bStt ? _T("시점측벽체") : _T("종점측벽체");
	if(IsDivideFrontAndBackWall)
	{
		sWall	= bStt ? _T("시점측벽체(외측)") : _T("종점측벽체(외측)");
	}
	

	// 벽체 구조도 조립도 //////////////////////////////////////////////////////////////////////
	CDRect rcWall(0, 0, 0, 0);	// 벽체 영역
	if(m_pStd->IsDrawByBlock(MPE_REBAR_WALL))
	{
		pPageMng->SetProgressData(sWall+_T(" 구조도 출력중..."), (int)(dCurPer+=dAddPer));
		if(IsDivideFrontAndBackWall)
		{
			rcWall	= DBStd.m_pDrawCrossSection->DrawRebar_OutWall_CrossSectionStd(&Dom, bStt, FALSE, FALSE, TRUE, TRUE);
		}
		else
		{
			rcWall	= DBStd.m_pDrawCrossSection->DrawRebar_OutWall_CrossSectionStd(&Dom, bStt, bStt ? FALSE : TRUE, bStt ? TRUE : FALSE, TRUE, TRUE);
		}
		xyMove	= CDPoint(rcDogak.left-rcWall.left+Dom.Always(100), rcDogak.bottom-rcWall.bottom-Dom.Always(100)); 
		Dom.Move(xyMove);
		rcWall.left	+= xyMove.x; rcWall.right	+= xyMove.x;
		rcWall.top	+= xyMove.y; rcWall.bottom	+= xyMove.y;
		Dom.SetCalcExtRect();
		rect	= Dom.GetExtRect();

		Dom.Move(pMove->GetPoint(MPE_REBAR_WALL));
		pStd->ChangeSeperateLayer(&Dom);
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_WALL, sWall+_T(" 구조도"));
		*pDomP << Dom;
	}

	if(m_pStd->IsDrawByBlock(MPE_REBAR_WALL_TITLE))
	{
		pStd->AddTitleOnDom(&Dom, m_nBridge, sWall+_T(" 구조도"), CDPoint(0,0), 0, dScale);
		Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
		Dom.Move(pMove->GetPoint(MPE_REBAR_WALL_TITLE));
		pStd->ChangeSeperateLayer(&Dom);
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_WALL_TITLE, sWall+_T(" 구조도 타이틀"));
		*pDomP << Dom;
	}

	// 전면배면 폭이 다르면 따로 출력함
	CDRect rcWall_Back(0, 0, 0, 0);	// 벽체 영역
	if(IsDivideFrontAndBackWall)
	{
		sWall	= bStt ? _T("시점측벽체(내측)") : _T("종점측벽체(내측)");

		if(m_pStd->IsDrawByBlock(MPE_REBAR_WALL_RIGHT))
		{
			pPageMng->SetProgressData(sWall+_T(" 구조도 출력중..."), (int)(dCurPer+=dAddPer));
			rcWall_Back	= DBStd.m_pDrawCrossSection->DrawRebar_OutWall_CrossSectionStd(&Dom, bStt, TRUE, TRUE, TRUE, TRUE);
			xyMove		= CDPoint(rcWall.right-rcWall_Back.left+Dom.Always(150), rcDogak.bottom-rcWall_Back.bottom-Dom.Always(100)); 
			Dom.Move(xyMove);
			rcWall_Back.left	+= xyMove.x; rcWall_Back.right	+= xyMove.x;
			rcWall_Back.top	+= xyMove.y; rcWall_Back.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();
			
			Dom.Move(pMove->GetPoint(MPE_REBAR_WALL_RIGHT));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_WALL_RIGHT, sWall+_T(" 구조도"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_REBAR_WALL_TITLE_RIGHT))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, sWall+_T(" 구조도"), CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_REBAR_WALL_TITLE_RIGHT));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_WALL_TITLE_RIGHT, sWall+_T(" 구조도 타이틀"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcWall_Back	 = rcWall;
	}
	
	////////////////////////////////////////////////////////////////////////////////////

	// 벽체 단면A-A 구조도 조립도 //////////////////////////////////////////////////////////////////////
	bVertDir	= pBri->IsVertDir();
	pBri->GetTvVertSection_Std(0, !bVertDir, FALSE);

	CDRect rcWallSec(0, 0, 0, 0);	// 벽체 단면A-A 영역
	if(m_pStd->IsDrawByBlock(MPE_REBAR_WALLSEC))
	{
		pPageMng->SetProgressData(_T("단면 A-A 출력중..."), (int)(dCurPer+=dAddPer));
		rcWallSec	= DBStd.m_pDrawVertSection->DrawRebar_OutWall_VertStd(&Dom, bStt, TRUE, TRUE, FALSE, 0);
		xyMove		= CDPoint(rcWall_Back.right-rcWallSec.left+Dom.Always(150), rcWall_Back.top-rcWallSec.top); 
		Dom.Move(xyMove);
		rcWallSec.left	+= xyMove.x; rcWallSec.right	+= xyMove.x;
		rcWallSec.top	+= xyMove.y; rcWallSec.bottom	+= xyMove.y;
		Dom.SetCalcExtRect();
		rect	= Dom.GetExtRect();
		
		Dom.Move(pMove->GetPoint(MPE_REBAR_WALLSEC));
		pStd->ChangeSeperateLayer(&Dom);
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_WALLSEC, _T("단면 A-A"));
		*pDomP << Dom;
	}

	if(m_pStd->IsDrawByBlock(MPE_REBAR_WALLSEC_TITLE))
	{
		pStd->AddTitleOnDom(&Dom, m_nBridge, _T("단면 A-A"), CDPoint(0,0), 0, dScale);
		Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
		Dom.Move(pMove->GetPoint(MPE_REBAR_WALLSEC_TITLE));
		pStd->ChangeSeperateLayer(&Dom);
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_WALLSEC_TITLE, _T("단면 A-A 타이틀"));
		*pDomP << Dom;
	}
	pBri->GetTvVertSection_Std(0, !bVertDir, FALSE, TRUE);
	////////////////////////////////////////////////////////////////////////////////////

	// 기초평면도 //////////////////////////////////////////////////////////////////////
	// 기초 평면도는 무조건 직방향으로 철근을 입력받으므로 직거리 싱크를 한번 한다.
	//pBri->SyncBridge(0, FALSE)
	pBri->GetTvVertSection_Std(0, FALSE, FALSE);
	pBri->SyncBridgeRebar(FALSE, -1, TRUE);
	CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;
	CDRect rcFooting[2]	= {(0, 0, 0, 0), (0, 0, 0, 0)};	// 기초평면도
	for(long upper = 0; upper < 2; upper++)
	{
		BOOL bUpper	= upper == iUPPER;
		if(!pData->m_bSepUpperAndLowerFootingRebar && !bUpper) continue;
		if(pBri->IsBoxType()) continue;

		CString sTitle	= "기초평면도";
		if(pData->m_bSepUpperAndLowerFootingRebar)
			sTitle	= bUpper ? "기초평면도(상면)" : "기초평면도(하면)";
		if(m_pStd->IsDrawByBlock(MPE_REBAR_FOOTINGPLANE+upper))
		{
			pPageMng->SetProgressData(sTitle + " 출력중...", (int)(dCurPer+=dAddPer));
			// 벽체 평면 그려서 회전
			DBStd.m_pDrawPlane->DrawPlaneGenOutWall(&Dom, bStt, TRUE);
			double dRotate	= bStt ? -pFooting->m_tvPathPlan.GetXyDir().GetAngleDegree() : 180-pFooting->m_tvPathPlan.GetXyDir().GetAngleDegree();
			Dom.Rotate(CDPoint(0, 0), ToDPointFrDegree(dRotate));

			if(!pData->m_bSepUpperAndLowerFootingRebar)
				rcFooting[upper]	= DBStd.m_pDrawFooting->DrawRebar_PlaneStd(&Dom, pFooting, FALSE, TRUE, TRUE, TRUE, bStt ? 1 : -1, FALSE, FALSE);
			else
				rcFooting[upper]	= DBStd.m_pDrawFooting->DrawRebar_PlaneStd(&Dom, pFooting, bUpper, bUpper, TRUE, TRUE, bStt ? 1 : -1, FALSE, FALSE);

			if(bUpper)
				xyMove		= CDPoint(rcWall.left-rcFooting[upper].left, rcWall.top-rcFooting[upper].bottom-Dom.Always(80)); 
			else
				xyMove		= CDPoint(rcWall.left-rcFooting[upper].left, rcFooting[iUPPER].top-rcFooting[upper].bottom-Dom.Always(80)); 
			Dom.Move(xyMove);
			rcFooting[upper].left	+= xyMove.x; rcFooting[upper].right	+= xyMove.x;
			rcFooting[upper].top	+= xyMove.y; rcFooting[upper].bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();
			

			Dom.Move(pMove->GetPoint(MPE_REBAR_FOOTINGPLANE+upper));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_FOOTINGPLANE+upper,sTitle);
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_REBAR_FOOTINGPLANE_TITLE+upper))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, sTitle, CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_REBAR_FOOTINGPLANE_TITLE+upper));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_FOOTINGPLANE_TITLE+upper, sTitle + " 타이틀");
			*pDomP << Dom;
		}
	}
	
	////////////////////////////////////////////////////////////////////////////////////

	// 벽체 상단 예각부 상세
	dScaleDetail	= pData->m_dScaleRebarDetailWallObtuse;
	Dom.SetScaleDim(dScaleDetail);
	CDPoint xyLast	= CDPoint(rcFooting[iUPPER].right+Dom.Always(50), rcFooting[iUPPER].bottom);
	long nIdx	= 0;
	long stt	= bStt ? iSTT : iEND;
	if(pBri->IsBoxType() == FALSE)
	{
		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT;

			CString sTitle	= _T("");
			long nMPE	= MPE_DETAIL_ACUTE_STT_LEFT;
			long nMPETitle	= MPE_DETAIL_ACUTE_STT_LEFT_TITLE;
			if(bStt)
			{
				nMPE		= bLeft ? MPE_DETAIL_ACUTE_STT_LEFT : MPE_DETAIL_ACUTE_STT_RIGHT;
				nMPETitle	= bLeft ? MPE_DETAIL_ACUTE_STT_LEFT_TITLE : MPE_DETAIL_ACUTE_STT_RIGHT_TITLE;
				sTitle		= bLeft ? "벽체하단 예각부(시점좌측) 보강 상세" : "벽체하단 예각부(시점우측) 보강 상세";
			}
			else
			{
				nMPE		= bLeft ? MPE_DETAIL_ACUTE_END_LEFT : MPE_DETAIL_ACUTE_END_RIGHT;
				nMPETitle	= bLeft ? MPE_DETAIL_ACUTE_END_LEFT_TITLE : MPE_DETAIL_ACUTE_END_RIGHT_TITLE;
				sTitle		= bLeft ? "벽체하단 예각부(종점좌측) 보강 상세" : "벽체하단 예각부(종점우측) 보강 상세";
			}

			nIdx++;
			if(!pBri->m_bIsAcuteAngle_OutWall[stt][left]) continue;

			CDRect rectAcute = CDRect(0, 0, 0, 0);
			if(m_pStd->IsDrawByBlock(nMPE))
			{
				pPageMng->SetProgressData(sTitle+" 출력중...", (int)(dCurPer+=dAddPer));
				DBStd.m_pDrawCrossSection->DrawRebarCrossSection_AcuteStd(&Dom, bStt, bLeft, TRUE, TRUE, FALSE);
				Dom.RedrawByScale(dScale/dScaleDetail);

				Dom.SetCalcExtRect();
				rectAcute	= Dom.GetExtRect();

				xyMove	= CDPoint(xyLast.x - rectAcute.left, xyLast.y - rectAcute.bottom);
				Dom.Move(xyMove);
				rectAcute.left	+= xyMove.x; rectAcute.right	+= xyMove.x;
				rectAcute.top	+= xyMove.y; rectAcute.bottom	+= xyMove.y;
				Dom.SetCalcExtRect();
				rect	= Dom.GetExtRect();

				Dom.Move(pMove->GetPoint(nMPE));
				pStd->ChangeSeperateLayer(&Dom);
				pMove->AddMPE(&Dom, Dom.GetExtRect(), nMPE, sTitle);
				*pDomP << Dom;	
			}


			if(m_pStd->IsDrawByBlock(nMPETitle))
			{
				pStd->AddTitleOnDom(&Dom, m_nBridge, sTitle, CDPoint(0,0), 0, dScaleDetail);
				Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
				Dom.Move(pMove->GetPoint(nMPETitle));
				pStd->ChangeSeperateLayer(&Dom);
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom, Dom.GetExtRect(), nMPETitle, sTitle + " 타이틀");
				*pDomP << Dom;
			}

			// 기초평면 둔각부 상세
			if(bStt)
			{
				nMPE		= bLeft ? MPE_DETAIL_ACUTE_STT_LEFT+1 : MPE_DETAIL_ACUTE_STT_RIGHT+1;
				nMPETitle	= bLeft ? MPE_DETAIL_ACUTE_STT_LEFT_TITLE+1 : MPE_DETAIL_ACUTE_STT_RIGHT_TITLE+1;
				sTitle		= bLeft ? "기초평면 둔각부(시점좌측) 보강 상세" : "기초평면 둔각부(시점우측) 보강 상세";
			}
			else
			{
				nMPE		= bLeft ? MPE_DETAIL_ACUTE_END_LEFT+1 : MPE_DETAIL_ACUTE_END_RIGHT+1;
				nMPETitle	= bLeft ? MPE_DETAIL_ACUTE_END_LEFT_TITLE+1 : MPE_DETAIL_ACUTE_END_RIGHT_TITLE+1;
				sTitle		= bLeft ? "기초평면 둔각부(종점좌측) 보강 상세" : "기초평면 둔각부(종점우측) 보강 상세";
			}

			dScaleDetail	= pData->m_dScaleRebarDetailFootingAcute;
			Dom.SetScaleDim(dScaleDetail);
			CDRect rectAcuteFooting	= rectAcute;
			BOOL bFront	= bStt ? bLeft : !bLeft;
			BOOL bDimForTopIsRight	= bStt ? FALSE : TRUE;
			long nRotate	= bStt ? 1 : -1;

			if(m_pStd->IsDrawByBlock(nMPE))
			{
				long nDrawUpper	= 0;

				DBStd.m_pDrawFooting->DrawRebar_Plane_ObtuseStd(&Dom, pFooting, bFront, TRUE, TRUE, FALSE, nDrawUpper, bDimForTopIsRight, nRotate, FALSE);
				Dom.RedrawByScale(dScale/dScaleDetail);
				Dom.SetCalcExtRect();
				rectAcuteFooting	= Dom.GetExtRect();
				xyMove	= CDPoint(rectAcute.left - rectAcuteFooting.left, rectAcute.top - rectAcuteFooting.bottom - Dom.Always(20));
				Dom.Move(xyMove);
				rectAcuteFooting.left	+= xyMove.x; rectAcuteFooting.right	+= xyMove.x;
				rectAcuteFooting.top	+= xyMove.y; rectAcuteFooting.bottom	+= xyMove.y;
				Dom.SetCalcExtRect();
				rect	= Dom.GetExtRect();

				Dom.Move(pMove->GetPoint(nMPE));
				pStd->ChangeSeperateLayer(&Dom);
				pMove->AddMPE(&Dom, Dom.GetExtRect(), nMPE, sTitle);
				*pDomP << Dom;			
			}

			if(m_pStd->IsDrawByBlock(nMPETitle))
			{
				pStd->AddTitleOnDom(&Dom, m_nBridge, sTitle, CDPoint(0,0), 0, dScaleDetail);
				Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
				Dom.Move(pMove->GetPoint(nMPETitle));
				pStd->ChangeSeperateLayer(&Dom);
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom, Dom.GetExtRect(), nMPETitle, sTitle + " 타이틀");
				*pDomP << Dom;			

				xyLast.x	= Dom.GetExtRect().right + Dom.Always(20);
			}
		}
	}


	// 아치리브 스프링깅부 상세
	dScaleDetail	= pData->m_dScaleRebarDetailArchRibSpringging;
	Dom.SetScaleDim(dScaleDetail);
	CDRect rcDetailArchJijum(0, 0, 0, 0);	// 아치리브 스프링깅부 상세
	if(1 && pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		if(m_pStd->IsDrawByBlock(MPE_DETAIL_ARCHJIJUM))
		{
			pPageMng->SetProgressData(_T("아치리브 스프링깅부 상세 출력중..."), (int)(dCurPer+=dAddPer));
			
 			DBStd.m_pDrawVertSection->DrawRebarDetailFooting_Front_ArchJijum(&Dom, bStt, dScaleDetail);
			Dom.RedrawByScale(dScale/dScaleDetail);
			Dom.SetCalcExtRect();
			rcDetailArchJijum	= Dom.GetExtRect();
			xyMove		= CDPoint(rcFooting[iUPPER].left-rcDetailArchJijum.left, rcFooting[iUPPER].top-rcDetailArchJijum.bottom-Dom.Always(80)); 
			Dom.Move(xyMove);
			rcDetailArchJijum.left	+= xyMove.x; rcDetailArchJijum.right	+= xyMove.x;
			rcDetailArchJijum.top	+= xyMove.y; rcDetailArchJijum.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();
			
			Dom.Move(pMove->GetPoint(MPE_DETAIL_ARCHJIJUM));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_DETAIL_ARCHJIJUM, _T("아치리브 스프링깅부 상세"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_DETAIL_ARCHJIJUM_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("아치리브 스프링깅부 상세"), CDPoint(0,0), 0, dScaleDetail);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_DETAIL_ARCHJIJUM_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_DETAIL_ARCHJIJUM_TITLE, _T("아치리브 스프링깅부 상세 타이틀"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcDetailArchJijum.left		= rcFooting[iUPPER].left;
		rcDetailArchJijum.bottom	= rcFooting[iUPPER].top;
		rcDetailArchJijum.right		= rcDetailArchJijum.left;
		rcDetailArchJijum.top		= rcDetailArchJijum.bottom;
	}
	Dom.SetScaleDim(dScale);

	pBri->GetTvVertSection_Std(0, FALSE, FALSE, TRUE);
}

// 날개벽 구조도를 출력하는 곳인데, 재료표를 출력해야 되는지 잘 판단해야 됨
void CDrawingRebar::AddPageWingWallRebar(CDomyun *pDomP, long nIdxWingWallRebarPage, CDRect rcDogak)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);
	CHgBaseDrawStd baseDraw;

	long nIdxWingWall	= nIdxWingWallRebarPage;
//	if(pData->m_bSeparateRebarBMWing)
//	{
//		nIdxWingWall	= nIdxWingWall / 2;
//	}

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;

	double dScaleWing = pData->m_dScaleRebarWingWall;
	DWORD dOptDraw	= pData->m_dwDrawRebar;

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(GetScale());

	CDRect rcBase1(0, 0, 0, 0);
	CDRect rcBase2(0, 0, 0, 0);
	CDRect rcSect(0, 0, 0, 0);		// 단면도 영역
	CDRect rcFront1(0, 0, 0, 0);	// 정면도(전면) 영역
	CDRect rcFront2(0, 0, 0, 0);	// 정면도(배면) 영역

	CString str = _T(""), strTitle = _T(""), strTitleSect = _T("");
	DWORD dwDraw = 0x00000000;
	BOOL bStt = FALSE, bLeft = FALSE;
	int nLeft = 1;

	CWingWall* pWingBL = pBri->GetWingWall(TRUE, TRUE);
	CWingWall* pWingBR = pBri->GetWingWall(TRUE, FALSE);
	CWingWall* pWingEL = pBri->GetWingWall(FALSE, TRUE);
	CWingWall* pWingER = pBri->GetWingWall(FALSE, FALSE);
	CWingWall* pW[4] = {pWingBL, pWingBR, pWingEL, pWingER};
	int nCountWing = 0;
	int i=0; for(i=0; i<4; i++)
	{
		if(pW[i]->m_bExist)
			nCountWing++;
		if(nCountWing==nIdxWingWall+1)
			break;
	}

	switch(i)
	{
	case 0:
		dwDraw = REBAR_WING_BL;
		bStt = TRUE;
		bLeft = TRUE;
		strTitle = _T("시점 좌측 ");
		break;
	case 1:
		dwDraw = REBAR_WING_BR;
		bStt = TRUE;
		bLeft = FALSE;
		nLeft = -1;
		strTitle = _T("시점 우측 ");
		break;
	case 2:
		dwDraw = REBAR_WING_EL;
		bStt = FALSE;
		bLeft = TRUE;
		nLeft = -1;
		strTitle = _T("종점 좌측 ");
		break;
	case 3:
		dwDraw = REBAR_WING_ER;
		bStt = FALSE;
		bLeft = FALSE;
		strTitle = _T("종점 우측 ");
		break;
	}

	// 재료표를 출력해야 되는지 판단함 ////////////////
//	if(pData->m_bSeparateRebarBMWing)
//	{
//		if(nIdxWingWallRebarPage % 2 == 1)	// 두번째 장은 재료표임
//		{
//			AddPageRebarBM(pDomP, rcDogak, 1, bStt, bLeft, 0);
//			return;
//		}
//	}
	////////////////////////////////////////////////////

	

	if(dOptDraw & dwDraw)
	{
		CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);
		if(pWing->m_bExist)
		{
			//long nTypeJongdanForRebarOrg = pBri->m_nTypeJongdanForRebar;
			//pBri->m_nTypeJongdanForRebar = 2;
			//pBri->SyncBridge(nLeft, TRUE);
			pBri->GetTvVertSection_Std(nLeft, TRUE, FALSE, FALSE, 2);
			pBri->GetTvWingWallPlane(bStt);

			char ch = 64;

			if(pData->m_bDrawRebarWingWall[i][0])
			{
				
				str = ch;
				// 정면도(전면)
				if(m_pStd->IsDrawByBlock(MPE_REBAR_WING_FRONT))
				{
					pPageMng->SetProgressData(strTitle+_T("날개벽(전면) 출력중..."), 5);
					AddRebarWingWallFront(&Dom, bStt, bLeft, 1, str);
					Dom.SetCalcExtRect();
					rcBase1 = Dom.GetExtRect();
					
					Dom.Move(rcDogak.left-rcBase1.left+GAP_LEFT*4, rcDogak.bottom-rcBase1.bottom-GAP_TOP*1.5); 
					Dom.SetCalcExtRect();
					rcFront1 = Dom.GetExtRect();
					
					Dom.Move(pMove->GetPoint(MPE_REBAR_WING_FRONT));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_WING_FRONT, strTitle+_T("날개벽(전면)"));
					*pDomP << Dom;
				}
					
				if(m_pStd->IsDrawByBlock(MPE_REBAR_WING_FRONT_TITLE))
				{
					pStd->AddTitleOnDom(&Dom, m_nBridge, strTitle+_T("날개벽(전면)"), CDPoint(0,0), 0, dScaleWing);
					Dom.Move(rcFront1.left+rcFront1.Width()/2.0, rcFront1.bottom+GAP_TITLE);
					Dom.Move(pMove->GetPoint(MPE_REBAR_WING_FRONT_TITLE));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_WING_FRONT_TITLE, strTitle+_T("날개벽(전면) 제목"));
					*pDomP << Dom;
				}
					
					// 정면도(배면)
				if(m_pStd->IsDrawByBlock(MPE_REBAR_WING_BACK))
				{
					pPageMng->SetProgressData(strTitle+_T("날개벽(배면) 출력중..."), 24);
					AddRebarWingWallFront(&Dom, bStt, bLeft, 0, str);
					Dom.SetCalcExtRect();
					rcBase2 = Dom.GetExtRect();
					
					Dom.Move(rcDogak.left-rcBase2.left+GAP_LEFT*11, rcDogak.bottom-rcBase1.bottom-GAP_TOP*1.5); 
					Dom.SetCalcExtRect();
					rcFront2 = Dom.GetExtRect();
					
					Dom.Move(pMove->GetPoint(MPE_REBAR_WING_BACK));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_WING_BACK, strTitle+_T("날개벽(배면)"));
					*pDomP << Dom;
				}
					
				if(m_pStd->IsDrawByBlock(MPE_REBAR_WING_BACK_TITLE))
				{
					pStd->AddTitleOnDom(&Dom, m_nBridge, strTitle+_T("날개벽(배면)"), CDPoint(0,0), 0, dScaleWing);
					Dom.Move(rcFront2.left+rcFront2.Width()/2.0, rcFront2.bottom+GAP_TITLE);
					Dom.Move(pMove->GetPoint(MPE_REBAR_WING_BACK_TITLE));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_WING_BACK_TITLE, strTitle+_T("날개벽(배면) 제목"));
					*pDomP << Dom;
				}
			}

			if(pData->m_bDrawRebarWingWall[i][1])
			{
				str = ++ch;
				// 단면 A-A
				strTitleSect = _T("단면 ") + str + _T("-") + str;
				if(m_pStd->IsDrawByBlock(MPE_REBAR_WING_SECT_A))
				{
					pPageMng->SetProgressData(strTitleSect+_T(" 출력중..."), 43);
					AddRebarWingWallSectionA(&Dom, bStt, bLeft);
					Dom.SetCalcExtRect();
// 					rcSect = Dom.GetExtRect();
					
					Dom.Move(rcDogak.left-rcBase1.left-GAP_LEFT*2, rcDogak.bottom-rcBase1.bottom-GAP_TOP*1.5); 
					Dom.SetCalcExtRect();
					rcSect = Dom.GetExtRect();
					
					Dom.Move(pMove->GetPoint(MPE_REBAR_WING_SECT_A));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_WING_SECT_A, strTitleSect);
					*pDomP << Dom;
				}
					
				if(m_pStd->IsDrawByBlock(MPE_REBAR_WING_SECT_A_TITLE))
				{
					pStd->AddTitleOnDom(&Dom, m_nBridge, strTitleSect, CDPoint(0,0), 0, dScaleWing);
					Dom.Move(rcSect.left+rcSect.Width()/2.0, rcSect.bottom+GAP_TITLE);
					Dom.Move(pMove->GetPoint(MPE_REBAR_WING_SECT_A_TITLE));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_WING_SECT_A_TITLE, strTitleSect+_T(" 제목"));
					*pDomP << Dom;
				}
			}

			if(pData->m_bDrawRebarWingWall[i][2])
			{
				// 단면 B-B
				str = ++ch;
				strTitleSect = _T("단면 ") + str + _T("-") + str;
				if(m_pStd->IsDrawByBlock(MPE_REBAR_WING_SECT_B))
				{
					pPageMng->SetProgressData(strTitleSect+_T(" 출력중..."), 62);
					AddRebarWingWallSectionB(&Dom, bStt, bLeft);
					Dom.SetCalcExtRect();
// 					rcSect = Dom.GetExtRect();
					
					Dom.Move(rcDogak.left-rcBase1.left+GAP_LEFT*4, rcDogak.top - rcBase1.bottom); 
					Dom.SetCalcExtRect();
					rcSect = Dom.GetExtRect();
					
					Dom.Move(pMove->GetPoint(MPE_REBAR_WING_SECT_B));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_WING_SECT_B, strTitleSect);
					*pDomP << Dom;
				}
					
				if(m_pStd->IsDrawByBlock(MPE_REBAR_WING_SECT_B_TITLE))
				{
					pStd->AddTitleOnDom(&Dom, m_nBridge, strTitleSect, CDPoint(0,0), 0, dScaleWing);
					Dom.Move(rcSect.left+rcSect.Width()/2.0, rcSect.bottom+GAP_TITLE);
					Dom.Move(pMove->GetPoint(MPE_REBAR_WING_SECT_B_TITLE));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_WING_SECT_B_TITLE, strTitleSect+_T(" 제목"));
					*pDomP << Dom;
				}
				
				// 단면 C-C
				if(pWing->m_dL2>0)
				{
					str = ++ch;
					strTitleSect = _T("단면 ") + str + _T("-") + str;
					if(m_pStd->IsDrawByBlock(MPE_REBAR_WING_SECT_C))
					{
						pPageMng->SetProgressData(strTitleSect+_T(" 출력중..."), 81);
						AddRebarWingWallSectionC(&Dom, bStt, bLeft);
						Dom.SetCalcExtRect();
// 						rcSect = Dom.GetExtRect();
						
						Dom.Move(rcDogak.left-rcBase1.left+GAP_LEFT*11, rcDogak.top - rcBase1.bottom); 
						Dom.SetCalcExtRect();
						rcSect = Dom.GetExtRect();
						
						Dom.Move(pMove->GetPoint(MPE_REBAR_WING_SECT_C));
						pStd->ChangeSeperateLayer(&Dom);
						pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_WING_SECT_C, strTitleSect);
						*pDomP << Dom;
					}
						
					if(m_pStd->IsDrawByBlock(MPE_REBAR_WING_SECT_C_TITLE))
					{
						pStd->AddTitleOnDom(&Dom, m_nBridge, strTitleSect, CDPoint(0,0), 0, dScaleWing);
						Dom.Move(rcSect.left+rcSect.Width()/2.0, rcSect.bottom+GAP_TITLE);
						Dom.Move(pMove->GetPoint(MPE_REBAR_WING_SECT_C_TITLE));
						pStd->ChangeSeperateLayer(&Dom);
						Dom.SetCalcExtRect();
						pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_WING_SECT_C_TITLE, strTitleSect+_T(" 제목"));
						*pDomP << Dom;
					}
				}
			}

			if(pWing->m_bAddRebarReinforce && pData->m_bDrawRebarWingWall[i][3])
			{
				// 흉벽보강 상세
				CDRect rcDetail;
				CString str1, str2;
				str1 = bStt? _T("(시점") : _T("(종점");
				str2 = bLeft? _T("좌측)") : _T("우측)");
				strTitleSect = _T("흉벽보강상세") + str1 + str2;
				if(m_pStd->IsDrawByBlock(MPE_REBAR_WING_DETAIL))
				{
					pPageMng->SetProgressData(strTitleSect+_T(" 출력중..."), 62);
					AddRebarWingWallReinforcDetail(&Dom, bStt, bLeft);
					Dom.SetCalcExtRect();
					rcDetail = Dom.GetExtRect();
					
					Dom.Move(rcDogak.right-rcDetail.right, rcDogak.bottom-rcDetail.bottom-GAP_TOP*1.5); 
					Dom.SetCalcExtRect();
					rcDetail = Dom.GetExtRect();
					
					Dom.Move(pMove->GetPoint(MPE_REBAR_WING_DETAIL));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_WING_DETAIL, strTitleSect);
					*pDomP << Dom;
				}
					
				if(m_pStd->IsDrawByBlock(MPE_REBAR_WING_DETAIL_TITLE))
				{
					pStd->AddTitleOnDom(&Dom, m_nBridge, strTitleSect, CDPoint(0,0), 0, dScaleWing);
					Dom.Move(rcDetail.left+rcDetail.Width()/2.0, rcDetail.bottom+GAP_TITLE);
					Dom.Move(pMove->GetPoint(MPE_REBAR_WING_DETAIL_TITLE));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_WING_DETAIL_TITLE, strTitleSect+_T(" 제목"));
					*pDomP << Dom;
				}
			}
		}
		pBri->GetTvVertSection_Std(nLeft, TRUE, FALSE, TRUE);
	}
	
	if(pData->m_bSeparateRebarBMWing)
	{
		CHgBaseConcStd baseConc;
		CTypedPtrArray <CObArray, CRebar*> arrRebar;	// 재료표용 철근리스트

		CDRect rcBMDetail, rcBMTable;

		long nType = 1;			// 날개벽
		MakeRebarList(&arrRebar, nType, bStt, bLeft, nIdxWingWall+1, 0);	// 상세도는 항상 전체 출력(날개벽과 보기둥은 상황에 따라 빠질 수 있음)
		baseConc.ShellSortRebarList(&arrRebar, &CRebar::CompareForList);
		

		CDPoint xyBase(0);
		if(pOpt->m_nTypeRebarTable == 0) //기본타입
		{
			// 철근상세 //////////////////////////////////////////////////////////////////////
			pPageMng->SetProgressData("철근상세 출력중...",75);
			DrawRebarDetail(&Dom, &arrRebar, 220, 50);
			Dom.SetCalcExtRect();

			CDRect rect	= Dom.GetExtRect();
			*pDomP << Dom;

			xyBase = CDPoint(Dom.Always(660),Dom.Always(240));
		}
		else
			xyBase = CDPoint(Dom.Always(0),Dom.Always(235));

		// 재료표 출력 //////////////////////////////////////////////////////////////////
		baseConc.SetRebarIndexBySortRebarList(&arrRebar, TRUE);//이형철근상세 인덱스보고 테이블에 번호 세팅

		pPageMng->SetProgressData("철근재료표 출력중...",80);
		if(pData->m_bRebarTableSepByFy)
		{
			double dFy = 1;
			while(dFy != 0)
			{
				CTypedPtrArray<CObArray, CRebar*> arrRebarGet;
				dFy = pStd->GetRebarListByFy(&arrRebar, &arrRebarGet, dFy);
				if(dFy == 0) break;
				if(arrRebarGet.GetSize() == 0) break;

				baseConc.ShellSortRebarList(&arrRebarGet, &CRebar::CompareForTable);
				if(arrRebar.GetSize() > 0)
					xyBase = DrawRebarTable(&Dom, &arrRebarGet, "", 0, xyBase, rcDogak);
			}
		}
		else
		{
			baseConc.ShellSortRebarList(&arrRebar, &CRebar::CompareForTable);
			if(arrRebar.GetSize() > 0)
				xyBase	= DrawRebarTable(&Dom, &arrRebar, "", 0, xyBase, rcDogak);
		}
		
		*pDomP << Dom;

		DrawRebarDetailTable_RawRebar(pDomP, xyBase, 1, bStt, bLeft);

		if(pOpt->m_nTypeRebarTable == 1) //도면간소화 타입
		{
			pPageMng->SetProgressData("철근상세 출력중...", 80);
			baseConc.ShellSortRebarList(&arrRebar, &CRebar::CompareForList);
			baseDraw.DrawRebarListDetailSimple(&Dom, &arrRebar, xyBase, pOpt->m_dWidthRebarTableRect, pOpt->m_dHeightRebarTableRect);

			Dom.SetCalcExtRect();
			Dom.Move(pMove->GetPoint(MPE_REBAR_DETAIL));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_DETAIL, hggettext("철근상세 제목"));

			*pDomP << Dom;
		}

		AhTPADelete(&arrRebar,	(CRebar*)0);
	}
	
	//pBri->m_nTypeJongdanForRebar = nTypeJongdanForRebarOrg;
}

// 본체(전체) - 날개벽과 보기둥은 옵션에 따라 빠질 수 있음
// 철근 재료표(철근상세, 재료표)
// nType -> 0 : 본체, 1 : 날개벽, 2 : 보기둥
// bStt, bLeft : 날개벽일경우 옵션
// nIdxInWall : 보기둥일경우 옵션
// BOOL bRebarBMDetail, BOOL bRebarBMTable : 본체의 경우 상세와 재료표를 따로 출력하는 옵션이 있음
void CDrawingRebar::AddPageRebarBM(CDomyun *pDomP, CDRect rcDogak, long nType, BOOL bStt, BOOL bLeft, long nIdxInWall, BOOL bRebarBMDetail, BOOL bRebarBMTable)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);
	CHgBaseDrawStd baseDraw;

	CHgBaseConcStd baseConc;
	CTypedPtrArray <CObArray, CRebar*> arrRebar;	// 재료표용 철근리스트
	CDomyun Dom(pDomP);

	MakeRebarList(&arrRebar, nType, bStt, bLeft, nIdxInWall+1, 0);	// 상세도는 항상 전체 출력(날개벽과 보기둥은 상황에 따라 빠질 수 있음)
	baseConc.ShellSortRebarList(&arrRebar, &CRebar::CompareForList);

	CDRect rect(0);
	if(pOpt->m_nTypeRebarTable == 0) //기본타입 일때 출력하는 철근상세모양
	{
		// 철근상세 //////////////////////////////////////////////////////////////////////
		if(bRebarBMDetail)
		{
			pPageMng->SetProgressData("철근상세 출력중...",30);
			DrawRebarDetail(&Dom, &arrRebar);
			Dom.SetCalcExtRect();
		}
		rect = Dom.GetExtRect();
	}

	// 재료표는 옵션에 따라서 1~3개까지 구분되어 출력될 수 있다.
	CDPoint xyBase = CDPoint(Dom.Always(650),Dom.Always(525));
	
	// 상세가 너무 많은 경우 재료표가 밀릴 수 있다. 
	if(rect.right + 1000 > xyBase.x)
		xyBase.x = rect.right + 1000;

	// 만약 상세는 출력하지 않고 재료표만 출력한다면 재료표를 앞으로 당긴다.
	// || 간소화타입이라면 상세가 그려지지 않으니 재료표를 앞으로 당긴다.
	if((!bRebarBMDetail && bRebarBMTable) || pOpt->m_nTypeRebarTable == 1)
	{
		xyBase.x = Dom.Always(70);
	}

	CTypedPtrArray<CObArray, CRebar*> arrRebarGet;
	CString sTitle[2] = {_T(""), _T("")};
	const long nCountTable = pData->m_nTypeOutRebarTable + 1;
	long nIdxTable = 0;
	
	for(long table = 0; table < nCountTable; table++)
	{
		if(pData->m_nTypeOutRebarTable == 1) //주철근, 기타철근
		{
			sTitle[0] = _T("주철근");
			sTitle[1] = _T("기타철근");

			MakeRebarList(&arrRebar, nType, bStt, bLeft, nIdxInWall+1, table==0 ? 1 : 2);
		}
			
		if(arrRebar.GetSize() == 0) continue;

		baseConc.SetRebarIndexBySortRebarList(&arrRebar, TRUE);  //테이블에 번호 세팅

		if(pData->m_bRebarTableSepByFy)  //강종분리일 경우
		{
			double dFy	= 1;
			while(dFy != 0)
			{
				dFy	= pStd->GetRebarListByFy(&arrRebar, &arrRebarGet, dFy);
				if(dFy == 0) break;
				if(arrRebarGet.GetSize() == 0) break;

				if(bRebarBMTable)
				{
					pPageMng->SetProgressData("철근재료표 출력중...",80);
					baseConc.ShellSortRebarList(&arrRebarGet, &CRebar::CompareForTable);
					xyBase	= DrawRebarTable(&Dom, &arrRebarGet, sTitle[table], nIdxTable++, xyBase, rcDogak);

					*pDomP << Dom;
				}

				if( bRebarBMDetail )
				{
					if(pOpt->m_nTypeRebarTable == 1) //간소화타입 일때 철근상세
					{
						pPageMng->SetProgressData("철근상세 출력중...", 80);
						baseConc.ShellSortRebarList(&arrRebarGet, &CRebar::CompareForList);
						baseDraw.DrawRebarListDetailSimple(&Dom, &arrRebarGet, xyBase, pOpt->m_dWidthRebarTableRect, pOpt->m_dHeightRebarTableRect);

						Dom.SetCalcExtRect();
						Dom.Move(pMove->GetPoint(MPE_REBAR_DETAIL + table));
						pStd->ChangeSeperateLayer(&Dom);
						pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_DETAIL + table, hggettext("철근상세 제목"));

						CDRect rect = Dom.GetExtRect();
						xyBase.x += fabs(rect.Width()+ Dom.Always(40));

						*pDomP << Dom;
					}
				}
			}
		}
		else
		{
			if(bRebarBMTable)
			{
				pPageMng->SetProgressData("철근재료표 출력중...",80);
				baseConc.ShellSortRebarList(&arrRebar, &CRebar::CompareForTable);
				xyBase	= DrawRebarTable(&Dom, &arrRebar, sTitle[table], nIdxTable++, xyBase, rcDogak);

				*pDomP << Dom;

			}
				
			if( bRebarBMDetail )
			{
				if(pOpt->m_nTypeRebarTable == 1) //간소화타입 일때 철근상세
				{
					pPageMng->SetProgressData("철근상세 출력중...", 80);
					baseConc.ShellSortRebarList(&arrRebar, &CRebar::CompareForList);
					baseDraw.DrawRebarListDetailSimple(&Dom, &arrRebar, xyBase, pOpt->m_dWidthRebarTableRect, pOpt->m_dHeightRebarTableRect);

					Dom.SetCalcExtRect();
					Dom.Move(pMove->GetPoint(MPE_REBAR_DETAIL + table));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_DETAIL + table, hggettext("철근상세 제목"));

					CDRect rect = Dom.GetExtRect();
					xyBase.x += fabs(rect.Width()+ Dom.Always(40));

					*pDomP << Dom;
				}
			}
		}
	}
		
	*pDomP << Dom;

	DrawRebarDetailTable_RawRebar(pDomP, xyBase, nType, TRUE, TRUE, nIdxInWall+1);

	xyBase.x += Dom.Always(50);
	xyBase.y -= Dom.Always(50);
	DrawDetail_ChairBlock(&Dom, xyBase);
	*pDomP << Dom;		   
	

	AhTPADelete(&arrRebar,	(CRebar*)0);
}

// 0 : 본체
// 1 : 날개벽
// 2 : 보기둥
// bStt, bLeft : 날개벽일경우 옵션
// nIdxInWall : 보기둥일경우 옵션
// long nRebarMain -> 0 : 주철근+기타철근, 1 : 주철근, 2 : 기타철근
void CDrawingRebar::MakeRebarList(CTypedPtrArray <CObArray, CRebar*> *pArrRebar, long nType, BOOL bStt, BOOL bLeft, long nJ, long nRebarMain)
{
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	
	AhTPADelete(pArrRebar, (CRebar*)0);
// 	CRebar *pRB	= NULL;
	long nList	= 0;
	BOOL bOK	= TRUE;

	// 전체
	// 날개벽과 보기둥은 옵션에 따라서 붙을수도 안 붙을 수도 있다.
	if(nType == 0)
	{
		pBri->MakeRebarList(pArrRebar, 0, TRUE, TRUE, 0);	// 본체
		long i = 0; for(i = 0; i <= pBri->m_nQtyJigan; i++)
		{
			pBri->MakeRebarList(pArrRebar, 1, TRUE, TRUE, i);	// 중간벽체
			pBri->MakeRebarList(pArrRebar, 6, TRUE, TRUE, i);	// 기둥위가 아닌 거더철근	
			
			if(!pData->m_bSeparateRebarBMColumn)	// 기둥 분리가 아니면
			{
				pBri->MakeRebarList(pArrRebar, 5, TRUE, TRUE, i);	// 기둥
				pBri->MakeRebarList(pArrRebar, 7, TRUE, TRUE, i);	// 기둥위 거더철근
			}
		}

		// (ARCBRIDGE-1702) 단일현장타설말뚝 철근 제외
		pBri->MakeRebarList(pArrRebar, 2, TRUE, TRUE, 0, FALSE);	// 기초
		if(!pData->m_bSeparateRebarBMWing)
		{
			for(long stt = 0; stt < 2; stt++)
			{
				for(long left = 0; left < 2; left++)
					pBri->MakeRebarList(pArrRebar, 3, stt==iSTT, left==iLEFT, 0);	// 날개벽
			}
		}

		// 기타철근 분리 출력이 아니면
		if(!pData->m_bSeparateRebarBMOther)
		{
			pBri->MakeRebarList(pArrRebar, 4, TRUE, TRUE, -1);	// 교명주철근
			pBri->MakeRebarList(pArrRebar, 8, TRUE, TRUE, -1);	// 방호벽철근
		}
	}

	// 날개벽
	if(nType == 1)
	{
		pBri->MakeRebarList(pArrRebar, 3, bStt, bLeft, 0);	// 날개벽
	}

	// 보기둥(거더포함)
	if(nType == 2)
	{
		pBri->MakeRebarList(pArrRebar, 5, bStt, bLeft, nJ);	// 기둥
		pBri->MakeRebarList(pArrRebar, 7, bStt, bLeft, nJ);	// 거더 철근
	}

	// 유효한것만 골라냄
	for(nList = 0; nList < pArrRebar->GetSize(); nList++)
	{
//		CRebar *pRB	= NULL;

		CRebar *pRB = pArrRebar->GetAt(nList);

		// 주철근 기타철근 골라냄
		bOK	= nRebarMain == 0 || (nRebarMain==1 && pRB->m_bMainRebar) || (nRebarMain==2 && !pRB->m_bMainRebar);

		if(!pRB->IsTrue() || !bOK)
		{
			delete pRB;
			pArrRebar->RemoveAt(nList);
			nList--;
		}
	}
}

// 상세도 그려 넣기
void CDrawingRebar::DrawRebarDetail(CDomyun *pDomP, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, double dMaxYIn, double dMinYIn)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);

	CConcSymbol symDB(pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);
	CHgBaseDrawStd HGDraw;
	double dSpace = pData->m_dSpaceBetweenRebarDetail;
	double dMaxY  = Dom.Always(dMaxYIn);
	double dMinY  = Dom.Always(dMinYIn);
	double dYPlus = 0;
	CDPoint xy(Dom.Always(70),dMaxY);
	CRebar *pRB;

	if(m_pStd->IsDrawByBlock(MPE_REBAR_DETAIL_TITLE))
	{
		pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
		Dom.TextOut(xy.x+Dom.Always(50),xy.y+Dom.GetTextHeight()/2,"철 근 상 세");
		Dom.Move(pMove->GetPoint(MPE_REBAR_DETAIL_TITLE));
		pStd->ChangeSeperateLayer(&Dom);
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_DETAIL_TITLE, "철근상세 제목");
		*pDomP << Dom;
	}

	pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_DIML);

	long nSize  = pArrRebar->GetSize();
	long nRow   = 0;
	long nType  = 0;
	long nDetailCol = 0;
	BOOL bJoint = FALSE;
	long i=0; for(i=0; i<nSize; i++)
	{
		pRB		= pArrRebar->GetAt(i);
		if(pRB->m_nTypeRebar!=nType || bJoint!=(pRB->m_nJoint>0))
		{
			nRow += 2;
			nType	= pRB->m_nTypeRebar;
			bJoint	= (pRB->m_nJoint>0) ? TRUE : FALSE;
			if((dMaxY-dSpace*nRow) < dMinY)
			{
				if(m_pStd->IsDrawByBlock(MPE_REBAR_DETAIL+nDetailCol*1))
				{
					Dom.Move(pMove->GetPoint(MPE_REBAR_DETAIL+nDetailCol*1));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_DETAIL+nDetailCol*1, "철근상세 "+COMMA(1+nDetailCol));
					*pDomP << Dom;
				}
				else
				{
					Dom.ClearEtt();
				}
				nDetailCol++;
				xy.x += Dom.Always(120);
				xy.y  = dMaxY;
				nRow  = 3;
			}

			CDPoint xyDrawPoint = CDPoint(xy.x+Dom.Always(50),xy.y+Dom.Always(10)+dYPlus);
			nRow = HGDraw.GetPlusCntRowDrawSymbolHeight(&Dom, pStd->m_pARoadOptionStd, nRow, nType, dSpace,  i, bJoint, xyDrawPoint);

			xy.y   = dMaxY - dSpace*nRow;
			dYPlus = (nType==1) ? Dom.Always(8) : 0;
			symDB.RebarDetailBasic(&Dom,nType,CDPoint(xy.x+Dom.Always(50),xy.y+Dom.Always(10)+dYPlus),bJoint);
		}
		else
			xy.y = dMaxY - dSpace*nRow;
		if(xy.y < dMinY)
		{
			if(m_pStd->IsDrawByBlock(MPE_REBAR_DETAIL+nDetailCol*1))
			{
				Dom.Move(pMove->GetPoint(MPE_REBAR_DETAIL+nDetailCol*1));
				pStd->ChangeSeperateLayer(&Dom);
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_DETAIL+nDetailCol*1, "철근상세 "+COMMA(1+nDetailCol));
				*pDomP << Dom;
			}
			else
			{
				Dom.ClearEtt(TRUE, FALSE);
			}
			nDetailCol++;
			xy.x += Dom.Always(120);
			xy.y  = dMaxY;
			nRow  = 0;
		}
		symDB.DrawJewonBar(&Dom,pRB,xy,pStd->m_pARcBridgeDataStd->m_pBasicConcInfo->m_nRoundRebarLength);
		nRow++;
	}
	if(m_pStd->IsDrawByBlock(MPE_REBAR_DETAIL+nDetailCol*10))
	{
		Dom.Move(pMove->GetPoint(MPE_REBAR_DETAIL+nDetailCol*10));
		pStd->ChangeSeperateLayer(&Dom);
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_DETAIL+nDetailCol*10, "철근상세 "+COMMA(1+nDetailCol));
		*pDomP << Dom;
	}
}

// 철근재료표 그리기
// CString sSubTitle : 부제목, 
// long nIdxTable : 테이블 순서. 부위별로 출력하면 여러개가 나올 수 있기 때문에..
CDPoint CDrawingRebar::DrawRebarTable(CDomyun *pDomP, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, CString sSubTitle, long nIdxTable, CDPoint xyBase, CDRect rcDogak)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);
	
	CGridDomyunEx Dom(pDomP);
	
	long nDefTableTitle	= MPE_REBAR_TABLE_TITLE1;
	switch(nIdxTable)
	{
		case 0: nDefTableTitle	= MPE_REBAR_TABLE_TITLE1; break;
		case 1: nDefTableTitle	= MPE_REBAR_TABLE_TITLE2; break;
		case 2: nDefTableTitle	= MPE_REBAR_TABLE_TITLE3; break;
		case 3: nDefTableTitle	= MPE_REBAR_TABLE_TITLE4; break;
		case 4: nDefTableTitle	= MPE_REBAR_TABLE_TITLE5; break;
		default: nDefTableTitle	= MPE_REBAR_TABLE_TITLE1; break;
	}
	
	long nTableType = pOpt->m_nTypeRebarTable;
	long nType =  pData->m_nTypeOutRebarTable;

	//기본타입이거나, 간소화인데 테이블 나눠지는 경우만 출력
	if(nTableType == 0 || (nTableType == 1 && nType == 1))
	{
		if(m_pStd->IsDrawByBlock(nDefTableTitle))
		{
			pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
			CString sTitle = "철 근 재 료 표";
			if(sSubTitle != "") sTitle += "(" + sSubTitle + ")";
			Dom.TextOut(xyBase.x+Dom.Always(155/2),xyBase.y+Dom.Always(10), sTitle);
			Dom.Move(pMove->GetPoint(nDefTableTitle));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), nDefTableTitle, "철근재료표 제목");
			*pDomP << Dom;
		}
	}

	double dLengthSum		= 0;
	double dLengthSumAdd	= 0;
	double dLengthTotal		= 0;
	long nMaxRow			= pData->m_nCountRowEachTable;
	long nDefTable			= MPE_REBAR_TABLE1;
	double dWeightTotal		= 0;
	double dWeightTotalAdd	= 0;

	switch(nIdxTable)
	{
		case 0: nDefTable	= MPE_REBAR_TABLE1; break;
		case 1: nDefTable	= MPE_REBAR_TABLE2; break;
		case 2: nDefTable	= MPE_REBAR_TABLE3; break;
		case 3: nDefTable	= MPE_REBAR_TABLE4; break;
		case 4: nDefTable	= MPE_REBAR_TABLE5; break;
		default: nDefTable	= MPE_REBAR_TABLE1; break;
	}

	CBasicConcInfo *pBCI = pStd->m_pARcBridgeDataStd->m_pBasicConcInfo;
	CHgBaseDrawStd baseDraw;
	double Fy = pData->m_bRebarTableSepByFy && pArrRebar->GetSize() > 0 ? pArrRebar->GetAt(0)->m_Fy : pBri->m_pARcBridgeDataStd->m_Fy;

	// 철근재료표 그릴때 단위계 적용
	if(pBri->m_pARcBridgeDataStd->m_nTypeUnit == 1)
		Fy = Fy * 10;

	BOOL bShadeOutLone = TRUE;//(pOpt->m_dDrawGen & GEBDRAW_GEN_TABLESHADE) ? TRUE : FALSE;
	long nIndex = 0;
	long nTableSu = 0;
	while(1)
	{
		if(nMaxRow <= 0) break;
		if(nIndex >= pArrRebar->GetSize()) break;
	
		if(nTableType == 0) //일반타입
			nIndex = baseDraw.DrawRebarTable(&Dom, nIndex, nMaxRow, xyBase, dLengthSum, dLengthSumAdd, dLengthTotal, dWeightTotal, dWeightTotalAdd, *pArrRebar, Fy, bShadeOutLone, "", pBCI, pBCI->m_nRoundRebarLength);
		else //간소화타입
			nIndex = baseDraw.DrawRebarTableSimple(&Dom, nIndex, nMaxRow, xyBase, dLengthSum, dLengthSumAdd, dLengthTotal, dWeightTotal, dWeightTotalAdd, *pArrRebar, Fy, bShadeOutLone, "", pBCI, pBCI->m_nRoundRebarLength);
		
		Dom.SetCalcExtRect();
		CDRect rtTable = Dom.GetExtRect();

		if(m_pStd->IsDrawByBlock(nDefTable+nTableSu))
		{
			Dom.Move(pMove->GetPoint(nDefTable+nTableSu));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), nDefTable+nTableSu, "철근재료표");
			*pDomP << Dom;
		}
		else
		{
			Dom.ClearEtt(TRUE, FALSE);
			*pDomP << Dom;
		}
		xyBase.x += rtTable.Width() + Dom.Always(10);
		nTableSu++;
	}		

	// 커플러 수량
	AddPageCoupler(&Dom, rcDogak, pArrRebar);
	*pDomP << Dom;

	return xyBase;
}

// 날개벽 정면도
void CDrawingRebar::AddRebarWingWallFront(CDomyun *pDomP, BOOL bStt, BOOL bLeft, int nOut, CString str)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;

	double dScaleWing = pData->m_dScaleRebarWingWall;
	double se = bStt ? -1.0 : 1.0;

	CDPoint vAng = pBri->GetAngleWingWallBetweenWall(bStt, bLeft);
	double dT1 = pWing->m_dT1;
	double dT2 = pWing->m_dT2;
	double dSpareT = nOut==0 ? (((Compare(vAng.y, 0.0, "=") || Compare(vAng.x, 0.0, "="))? max(dT1, dT2) : max(dT1, dT2)/vAng.y*vAng.x)) : 0.0;

	long nIndex = (bStt)? ((bLeft)? 0:1):((bLeft)? 2:3);

	long nTypeJongdanForRebarOrg = pBri->m_nTypeJongdanForRebar;
	pBri->m_nTypeJongdanForRebar = 2;


	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScaleWing);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DBStd.m_pDrawWingWall->DrawGenFront(&Dom, bStt, bLeft, nOut);
	DBStd.m_pDrawWingWall->DrawGenCutBriSect(&Dom, bStt, bLeft, nOut, TRUE);

	long nDan = 0;
	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	DBStd.m_pDrawWingWall->DrawRebarBlockAMainFront(&Dom, bStt, bLeft, nOut, nDan);
	DBStd.m_pDrawWingWall->DrawRebarBlockBMainFront(&Dom, bStt, bLeft, nOut, nDan);
	DBStd.m_pDrawWingWall->DrawRebarBlockCMainFront(&Dom, bStt, bLeft, nOut, nDan);
	DBStd.m_pDrawWingWall->DrawRebarBlockDMainFront(&Dom, bStt, bLeft, nOut, nDan);

	DBStd.m_pDrawWingWall->DrawRebarBlockASupportFront(&Dom, bStt, bLeft, nOut, nDan);
	DBStd.m_pDrawWingWall->DrawRebarBlockDSupportFront(&Dom, bStt, bLeft, nOut, nDan);

	DBStd.m_pDrawWingWall->DrawRebarBlockAShearFront(&Dom, bStt, bLeft, nOut);
	DBStd.m_pDrawWingWall->DrawRebarBlockBCShearFront(&Dom, bStt, bLeft, nOut);
	DBStd.m_pDrawWingWall->DrawRebarBlockDShearFront(&Dom, bStt, bLeft, nOut);

	DBStd.m_pDrawWingWall->DrawRebarHunchFootingFront(&Dom, bStt, bLeft, nOut);
	DBStd.m_pDrawWingWall->DrawRebarHunchWallFront(&Dom, bStt, bLeft, nOut);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	DBStd.m_pDrawWingWall->DimRebarFrontUpper(&Dom, bStt, bLeft, nOut, FALSE);
	DBStd.m_pDrawWingWall->DimRebarFrontLower(&Dom, bStt, bLeft, nOut, FALSE);
	DBStd.m_pDrawWingWall->DimRebarFrontSide(&Dom, bStt, bLeft, nOut, FALSE);
	if(nOut==1)
		DBStd.m_pDrawWingWall->MarkGenFrontSection(&Dom, bStt, bLeft, FALSE, FALSE, str, pData->m_bDrawRebarWingWall[nIndex][1], pData->m_bDrawRebarWingWall[nIndex][2]);

	DBStd.m_pDrawWingWall->MarkRebarBlockAFront(&Dom, bStt, bLeft, nOut);
	DBStd.m_pDrawWingWall->MarkRebarBlockBCFront(&Dom, bStt, bLeft, nOut);
	DBStd.m_pDrawWingWall->MarkRebarBlockDFront(&Dom, bStt, bLeft, nOut);
	if(nOut==0)
	{
		DBStd.m_pDrawWingWall->MarkRebarHunchWallFront(&Dom, bStt, bLeft);
		DBStd.m_pDrawWingWall->MarkRebarHunchFootingFront(&Dom, bStt, bLeft);
	}
	
	Dom.Move(dSpareT*se, 0);

	Dom.RedrawByScale(GetScale()/dScaleWing);

	*pDomP << Dom;

	pBri->m_nTypeJongdanForRebar	= nTypeJongdanForRebarOrg;
}

// 날개벽 단면 A
void CDrawingRebar::AddRebarWingWallSectionA(CDomyun *pDomP, BOOL bStt, BOOL bLeft)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;

	double dScaleWing = pData->m_dScaleRebarWingWall;

	CTwinVectorArray tvArrHu;
	pBri->GetTvArrayRebarWingWallHunchFootingSect(tvArrHu, bStt, bLeft);
	CDPoint xyHu1 = tvArrHu.GetAt(0).m_v1;
	CDPoint xyHu2 = tvArrHu.GetAt(2).m_v2;
	double dHuW = pWing->m_dHuW1;
	double dHuH = pWing->m_dHuH1;
	double dCoverWingOut = pBri->m_dCoverWingOut[0];
	double dT2 = pWing->m_dT2;
	double dSpare = fabs(xyHu1.x-xyHu2.x) + dCoverWingOut*2 - (dT2+dHuW);
	if(dHuW==0 || dHuH==0) dSpare = 1500;

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScaleWing);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DBStd.m_pDrawWingWall->DrawGenSection(&Dom, bStt, bLeft, dSpare);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	DBStd.m_pDrawWingWall->DrawRebarSection(&Dom, bStt, bLeft);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	DBStd.m_pDrawWingWall->DimRebarSectIn(&Dom, bStt, bLeft);
	DBStd.m_pDrawWingWall->DimRebarSectOut(&Dom, bStt, bLeft);
	DBStd.m_pDrawWingWall->DimRebarSectTop(&Dom, bStt, bLeft);

	DBStd.m_pDrawWingWall->MarkRebarSection(&Dom, bStt, bLeft, 0);
	DBStd.m_pDrawWingWall->MarkRebarSection(&Dom, bStt, bLeft, 1);

	Dom.RedrawByScale(GetScale()/dScaleWing);

	*pDomP << Dom;
}

// 날개벽 단면 B
void CDrawingRebar::AddRebarWingWallSectionB(CDomyun *pDomP, BOOL bStt, BOOL bLeft)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CWingWall *pWing = pBri->GetWingWall(bStt, bLeft);
	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;

	BOOL bSide		= (pWing->m_nAttachPos == 0 )? FALSE : TRUE;
	double dScaleWing = pData->m_dScaleRebarWingWall;

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
	
	CTwinVectorArray tvArrHu;
	pBri->GetTvArrayRebarWingWallHunchWallPlane(tvArrHu, bStt, bLeft, TRUE);
	CDPoint xyHu1 = tvArrHu.GetAt(0).m_v1;
	CDPoint xyHu2 = tvArrHu.GetAt(2).m_v2;
	CDPoint vHu1 = tvArrHu.GetAt(0).GetXyDir();
	CDPoint vHu2 = tvArrHu.GetAt(2).GetXyDir();
	if(!bSide) GetXyMatchLineAndLine(xyHu1, vHu1, xyHu2, vHu2, xyHu1);
	double dCoverWingOut = pBri->m_dCoverWingOut[0];
	double dSpare = max(~(xyHu1-xyHu2)+dCoverWingOut*2, dCutLen);
	if(pWing->m_bAddRebarReinforce && pWing->m_nAttachPos==0)
	{
		CDPoint xyRf[3];
		pBri->GetXyRebarWingWallReinforce(xyRf, bStt, bLeft);
		dSpare = max(~(xyRf[0]-xyRf[1])+200, dSpare);
	}

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScaleWing);

	pBri->GetTvWingWallPlane(bStt, TRUE, TRUE);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DBStd.m_pDrawWingWall->DrawGenCutPlane(&Dom, bStt, bLeft, dSpare, TRUE);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	DBStd.m_pDrawWingWall->DrawRebarPlane(&Dom, bStt, bLeft);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	DBStd.m_pDrawWingWall->DimRebarPlaneLower(&Dom, bStt, bLeft);
	DBStd.m_pDrawWingWall->DimRebarPlaneEnd(&Dom, bStt, bLeft);

	DBStd.m_pDrawWingWall->MarkRebarPlaneUpper(&Dom, bStt, bLeft, 0);
	DBStd.m_pDrawWingWall->MarkRebarPlaneUpper(&Dom, bStt, bLeft, 1);

	CDPoint xyMove = pBri->GetXySectionWingWallStt(bStt, TRUE);
	Dom.Move(xyMove);

	Dom.RedrawByScale(GetScale()/dScaleWing);

	*pDomP << Dom;
}

// 날개벽 단면 C
void CDrawingRebar::AddRebarWingWallSectionC(CDomyun *pDomP, BOOL bStt, BOOL bLeft)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CWingWall *pWing = pBri->GetWingWall(bStt, bLeft);
	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;

	BOOL bSide		= (pWing->m_nAttachPos == 0 )? FALSE : TRUE;
	double dScaleWing = pData->m_dScaleRebarWingWall;

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScaleWing);

	pBri->GetTvWingWallPlane(bStt, TRUE, FALSE, FALSE);

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
	
	CTwinVectorArray tvArrHu;
	pBri->GetTvArrayRebarWingWallHunchWallPlane(tvArrHu, bStt, bLeft, FALSE);
	CDPoint xyHu1 = tvArrHu.GetAt(0).m_v1;
	CDPoint xyHu2 = tvArrHu.GetAt(2).m_v2;
	CDPoint vHu1 = tvArrHu.GetAt(0).GetXyDir();
	CDPoint vHu2 = tvArrHu.GetAt(2).GetXyDir();
	if(!bSide) GetXyMatchLineAndLine(xyHu1, vHu1, xyHu2, vHu2, xyHu1);
	double dCoverWingOut = pBri->m_dCoverWingOut[0];
	double dSpare = max(~(xyHu1-xyHu2)+dCoverWingOut*2, dCutLen);
	if(pWing->m_bAddRebarReinforce && pWing->m_nAttachPos==0)
	{
		CDPoint xyRf[3];
		pBri->GetXyRebarWingWallReinforce(xyRf, bStt, bLeft);
		dSpare = max(~(xyRf[0]-xyRf[1])+200, dSpare);
	}

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DBStd.m_pDrawWingWall->DrawGenCutPlane(&Dom, bStt, bLeft, dSpare);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	DBStd.m_pDrawWingWall->DrawRebarPlane(&Dom, bStt, bLeft, FALSE);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	DBStd.m_pDrawWingWall->DimRebarPlaneLower(&Dom, bStt, bLeft, FALSE);
	DBStd.m_pDrawWingWall->DimRebarPlaneEnd(&Dom, bStt, bLeft, FALSE);

	DBStd.m_pDrawWingWall->MarkRebarPlaneLower(&Dom, bStt, bLeft, 0);
	DBStd.m_pDrawWingWall->MarkRebarPlaneLower(&Dom, bStt, bLeft, 1);

	CDPoint xyMove = pBri->GetXySectionWingWallStt(bStt, TRUE);
	Dom.Move(xyMove);

	Dom.RedrawByScale(GetScale()/dScaleWing);

	*pDomP << Dom;
}

void CDrawingRebar::AddRebarWingWallReinforcDetail(CDomyun *pDomP, BOOL bStt, BOOL bLeft)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();
	
	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;
	
	double dScaleWing = pData->m_dScaleRebarWingWall;
	
	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScaleWing);
	
	DBStd.m_pDrawWingWall->DrawDetailReinforceRebar(&Dom, bStt, bLeft);
	
	CDPoint xyMove = pBri->GetXySectionWingWallStt(bStt, TRUE);
	Dom.Move(xyMove);

	Dom.RedrawByScale(GetScale()/dScaleWing);
	
	*pDomP << Dom;
}

// chair block 상세도 그리기
void CDrawingRebar::DrawDetail_ChairBlock(CDomyun *pDomP, CDPoint xyLeftTop)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);


	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;

	double dScale	= GetScale();
	DWORD dOptDraw	= pData->m_dwDrawRebar;

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScale);

	CDPoint xyMove(0, 0);
	CDRect rect(0, 0, 0, 0);

	// chair block //////////////////////////////////////////////////////////////////////
	CString strDxfName;
	CString strDxfFileName;
	CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	if(dOptDraw & REBAR_DETAIL_CHAIR_BLOCK)
	{
		CDRect rect(0, 0, 0, 0);
		
		// 피복유지
		CDRect rcCover(0, 0, 0, 0);
		if(m_pStd->IsDrawByBlock(MPE_DETAIL_CHAIRBLOCK_COVER))
		{
			pPageMng->SetProgressData(_T("피복유지 출력중..."), 50);

			strDxfFileName	   = "chairblock_피복유지_상세.dxf";
			strDxfName.Format("%s\dxf\\%s",szPath,strDxfFileName);
			
			if(!Dom.DxfIn(strDxfName)) AfxMessageBox("상세도 가져오기 실패!");
			Dom.SetCalcExtRect();
			rcCover	= Dom.GetExtRect();
			xyMove	= xyLeftTop;
			Dom.Move(xyMove);
			rcCover.left	+= xyMove.x;	rcCover.right	+= xyMove.x;
			rcCover.top		+= xyMove.y;	rcCover.bottom	+= xyMove.y;
			
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_DETAIL_CHAIRBLOCK_COVER));
			pStd->ChangeSeperateLayer(&Dom);
			
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_DETAIL_CHAIRBLOCK_COVER, "피복유지");
			*pDomP << Dom;
		}

		
		if(m_pStd->IsDrawByBlock(MPE_DETAIL_CHAIRBLOCK_COVER_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("피복유지"), CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_DETAIL_CHAIRBLOCK_COVER_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_DETAIL_CHAIRBLOCK_COVER_TITLE, _T("피복유지 제목"));
			*pDomP << Dom;
		}


		// Spacer 설치위치
		CDRect rcSpacer(0, 0, 0, 0);
		if(m_pStd->IsDrawByBlock(MPE_DETAIL_CHAIRBLOCK_SPACER))
		{
			pPageMng->SetProgressData(_T("SPACER 설치위치 출력중..."), 50);

			strDxfFileName	   = "chairblock_Spacer설치위치_상세.dxf";
			strDxfName.Format("%s\dxf\\%s",szPath,strDxfFileName);
			
			if(!Dom.DxfIn(strDxfName)) AfxMessageBox("상세도 가져오기 실패!");
			Dom.SetCalcExtRect();
			rcSpacer	= Dom.GetExtRect();
			xyMove		= CDPoint(rcCover.right - rcSpacer.left + Dom.Always(10), rcCover.bottom-rcSpacer.bottom);
			Dom.Move(xyMove);
			rcSpacer.left	+= xyMove.x;	rcSpacer.right	+= xyMove.x;
			rcSpacer.top	+= xyMove.y;	rcSpacer.bottom	+= xyMove.y;
			
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_DETAIL_CHAIRBLOCK_SPACER));
			pStd->ChangeSeperateLayer(&Dom);
			
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_DETAIL_CHAIRBLOCK_SPACER, "SPACER 설치위치");
			*pDomP << Dom;
		}

		
		if(m_pStd->IsDrawByBlock(MPE_DETAIL_CHAIRBLOCK_SPACER_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("SPACER 설치위치"), CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_DETAIL_CHAIRBLOCK_SPACER_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_DETAIL_CHAIRBLOCK_SPACER_TITLE, _T("SPACER 설치위치 제목"));
			*pDomP << Dom;
		}

	
		// 전단철근 상세
		CDRect rcShear(0, 0, 0, 0);
		if(m_pStd->IsDrawByBlock(MPE_DETAIL_CHAIRBLOCK_SHEAR))
		{
			pPageMng->SetProgressData(_T("전단철근 상세 출력중..."), 50);

			strDxfFileName	   = "chairblock_전단철근_상세.dxf";
			strDxfName.Format("%s\dxf\\%s",szPath,strDxfFileName);
			
			if(!Dom.DxfIn(strDxfName)) AfxMessageBox("상세도 가져오기 실패!");
			Dom.SetCalcExtRect();
			rcShear	= Dom.GetExtRect();
			xyMove		= CDPoint(rcSpacer.right - rcShear.left + Dom.Always(10), rcSpacer.bottom-rcShear.bottom);
			Dom.Move(xyMove);
			rcShear.left	+= xyMove.x;	rcShear.right	+= xyMove.x;
			rcShear.top		+= xyMove.y;	rcShear.bottom	+= xyMove.y;
			
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_DETAIL_CHAIRBLOCK_SHEAR));
			pStd->ChangeSeperateLayer(&Dom);
			
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_DETAIL_CHAIRBLOCK_SHEAR, "전단철근 상세");
			*pDomP << Dom;
		}

		
		if(m_pStd->IsDrawByBlock(MPE_DETAIL_CHAIRBLOCK_SHEAR_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("전단철근 상세"), CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_DETAIL_CHAIRBLOCK_SHEAR_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_DETAIL_CHAIRBLOCK_SHEAR_TITLE, _T("전단철근 상세 제목"));
			*pDomP << Dom;
		}


	}

	////////////////////////////////////////////////////////////////////////////////////
}

void CDrawingRebar::AddPageArchRibRebar(CDomyun *pDomP, CDRect rcDogak)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);

	BOOL bVertDir	= pBri->IsVertDir();
	//pBri->SyncBridge(0, !bVertDir);
	pBri->GetTvVertSection_Std(0, !bVertDir, FALSE);
	

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;

	double dScale	= GetScale();
	DWORD dOptDraw	= pData->m_dwDrawRebar;

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScale);

	CDPoint xyMove(0, 0);
	CDRect rect(0, 0, 0, 0);

	double dXMidOfDogak	= rcDogak.left+rcDogak.Width()/2;
	// 아치리브 배력철근 조립도 //////////////////////////////////////////////////////////////////////
	CDRect rcSupportRebar(0, 0, 0, 0);	// 아치리브 배력철근 조립도 영역
	if(1)//dOptDraw & REBAR_STD_SECT)
	{
		if(m_pStd->IsDrawByBlock(MPE_REBAR_ARCHRIB_SUPPORT_REBAR))
		{
			pPageMng->SetProgressData(_T("아치리브 배력철근 조립도 출력중..."), 20);
			rcSupportRebar = DBStd.m_pDrawVertSection->DrawSupportRebarArchRibStd(&Dom, TRUE, TRUE, pData->m_bMainRebarNumberingInStandardSection, 0);
			xyMove		= CDPoint(dXMidOfDogak-(rcSupportRebar.left+rcSupportRebar.Width()/2), rcDogak.bottom-rcSupportRebar.bottom-Dom.Always(100)); 
			Dom.Move(xyMove);
			rcSupportRebar.left		+= xyMove.x; rcSupportRebar.right	+= xyMove.x;
			rcSupportRebar.top		+= xyMove.y; rcSupportRebar.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_REBAR_ARCHRIB_SUPPORT_REBAR));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_ARCHRIB_SUPPORT_REBAR, _T("아치리브 배력철근 조립도"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_REBAR_ARCHRIB_SUPPORT_REBAR_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("아치리브 배력철근 조립도"), CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_REBAR_ARCHRIB_SUPPORT_REBAR_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_ARCHRIB_SUPPORT_REBAR_TITLE, _T("아치리브 배력철근 조립도 타이틀"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcSupportRebar.left		= rcDogak.left+Dom.Always(150);
		rcSupportRebar.bottom	= rcDogak.bottom-Dom.Always(100);
		rcSupportRebar.right	= rcSupportRebar.left;
		rcSupportRebar.top		= rcSupportRebar.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////

	Dom.SetScaleDim(pData->m_dScaleAFD);
	// rectAFD /////////////////////////////////////////////////////////////////////////////
	CDRect rectAFD(0, 0, 0, 0);
	if(dOptDraw & REBAR_AFD)
	{
		if(m_pStd->IsDrawByBlock(MPE_AFD))
		{
			pPageMng->SetProgressData(_T("AFD 출력중..."), 30);
			rectAFD	= DBStd.m_pDrawVertSection->DrawDiagramBMD_SFD(&Dom, FALSE, FALSE, TRUE, TRUE, FALSE);
			xyMove	= CDPoint(rcSupportRebar.CenterPoint().x-rectAFD.CenterPoint().x, rcSupportRebar.top-rectAFD.bottom-Dom.Always(30));
			Dom.Move(xyMove);
			rectAFD.left	+= xyMove.x; rectAFD.right	+= xyMove.x;
			rectAFD.top	+= xyMove.y; rectAFD.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_AFD));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_AFD, _T("AFD"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_AFD_TITLE))
		{
			Dom.SetScaleDim(pData->m_dScaleAFD);
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("AFD"), CDPoint(0,0), 0, pData->m_dScaleAFD);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_AFD_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_AFD_TITLE, _T("AFD 타이틀"));
			*pDomP << Dom;
		}
	}
	else
	{
		rectAFD.left		= rcSupportRebar.left;
		rectAFD.bottom	= rcSupportRebar.bottom;
		rectAFD.right		= rcSupportRebar.left;
		rectAFD.top		= rcSupportRebar.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////

	// 아치리브 평면도 //////////////////////////////////////////////////////////////////////
	CDRect rcPlane(0, 0, 0, 0);	// 아치리브 평면도 영역
	Dom.SetScaleDim(dScale);
	if(1)//dOptDraw & REBAR_STD_SECT)
	{
		if(m_pStd->IsDrawByBlock(MPE_REBAR_ARCHRIB_PLANE_REBAR))
		{
			pPageMng->SetProgressData(_T("아치리브 평면도 출력중..."), 40);
			rcPlane = DBStd.m_pDrawPlane->DrawRebar_Plane_ArchRibStd(&Dom, -1, TRUE, FALSE, TRUE, TRUE);
			xyMove		= CDPoint((rcSupportRebar.left+rcSupportRebar.right)/2-(rcPlane.left+rcPlane.right)/2, rectAFD.top-rcPlane.bottom-Dom.Always(50)); 
			Dom.Move(xyMove);
			rcPlane.left	+= xyMove.x; rcPlane.right	+= xyMove.x;
			rcPlane.top		+= xyMove.y; rcPlane.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_REBAR_ARCHRIB_PLANE_REBAR));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_ARCHRIB_PLANE_REBAR, _T("아치리브 평면도"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_REBAR_ARCHRIB_PLANE_REBAR_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("아치리브 평면도"), CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_REBAR_ARCHRIB_PLANE_REBAR_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_ARCHRIB_PLANE_REBAR_TITLE, _T("아치리브 평면도 타이틀"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcPlane.left	= (rcSupportRebar.left + rcSupportRebar.right)/2;
		rcPlane.bottom	= rcSupportRebar.bottom - Dom.Always(50);;
		rcPlane.right	= rcPlane.left;
		rcPlane.top		= rcPlane.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////

	// 아치리브 횡단면도 SECTION A-A //////////////////////////////////////////////////////////////////////
	char ch	= 'A';
	CString sSection	= _T("");
	CDRect rcCrossSectionA(0, 0, 0, 0);	// 아치리브 횡단면도 SECTION A-A 영역
	BOOL bOK	= TRUE;
	double dXDisOfLeft	= DBStd.m_pDrawCrossSection->GetDistForCrossSection_ArchRib(0, bOK);
	double dXDisOfRight	= dXDisOfLeft;
	if(bOK && dOptDraw & REBAR_STD_SECT)
	{
		sSection.Format("SECTION %c-%c", ch, ch);
		ch++;

		if(m_pStd->IsDrawByBlock(MPE_REBAR_ARCHRIB_SECTION_A))
		{
			pPageMng->SetProgressData(_T(sSection + " 출력중..."), 60);
			rcCrossSectionA	= DBStd.m_pDrawCrossSection->DrawRebarCrossSection_ArchRib_Std(&Dom, dXDisOfLeft, dXDisOfRight, TRUE, TRUE);
			xyMove		= CDPoint(rcPlane.left - rcCrossSectionA.right - Dom.Always(150), rcPlane.bottom - rcCrossSectionA.bottom);
			Dom.Move(xyMove);
			rcCrossSectionA.left		+= xyMove.x; rcCrossSectionA.right	+= xyMove.x;
			rcCrossSectionA.top		+= xyMove.y; rcCrossSectionA.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_REBAR_ARCHRIB_SECTION_A));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_ARCHRIB_SECTION_A, _T(sSection));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_REBAR_ARCHRIB_SECTION_A_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T(sSection), CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_REBAR_ARCHRIB_SECTION_A_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_ARCHRIB_SECTION_A_TITLE, _T(sSection + " 타이틀"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcCrossSectionA.left	= (rcPlane.left + rcPlane.right)/2;
		rcCrossSectionA.bottom	= rcPlane.bottom - Dom.Always(50);;
		rcCrossSectionA.right	= rcCrossSectionA.left;
		rcCrossSectionA.top		= rcCrossSectionA.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////


	// 아치리브 횡단면도 SECTION B-B //////////////////////////////////////////////////////////
	CDRect rcCrossSectionB(0, 0, 0, 0);	// SECTION B-B 영역
	bOK	= TRUE;
	dXDisOfLeft	= DBStd.m_pDrawCrossSection->GetDistForCrossSection_ArchRib(1, bOK);
	dXDisOfRight	= dXDisOfLeft;
	if(bOK)//dOptDraw & REBAR_STD_SECT)
	{
		sSection.Format("SECTION %c-%c", ch, ch);
		ch++;

		if(m_pStd->IsDrawByBlock(MPE_REBAR_ARCHRIB_SECTION_B))
		{
			pPageMng->SetProgressData(_T(sSection + " 출력중..."), 80);
			rcCrossSectionB	= DBStd.m_pDrawCrossSection->DrawRebarCrossSection_ArchRib_Std(&Dom, dXDisOfLeft, dXDisOfRight, TRUE, TRUE);
			xyMove		= CDPoint(rcPlane.right - rcCrossSectionB.left + Dom.Always(150), rcPlane.bottom - rcCrossSectionB.bottom);
			Dom.Move(xyMove);
			rcCrossSectionB.left		+= xyMove.x; rcCrossSectionB.right	+= xyMove.x;
			rcCrossSectionB.top		+= xyMove.y; rcCrossSectionB.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_REBAR_ARCHRIB_SECTION_B));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_ARCHRIB_SECTION_B, _T(sSection));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_REBAR_ARCHRIB_SECTION_B_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T(sSection), CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_REBAR_ARCHRIB_SECTION_B_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_ARCHRIB_SECTION_B_TITLE, _T(sSection + " 타이틀"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcCrossSectionB.left	= rcPlane.right;
		rcCrossSectionB.bottom	= rcPlane.bottom;
		rcCrossSectionB.right	= rcCrossSectionB.left;
		rcCrossSectionB.top		= rcCrossSectionB.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////

	// 아치리브 횡단면도 SECTION C-C //////////////////////////////////////////////////////////
	CDRect rcCrossSectionC(0, 0, 0, 0);	// SECTION C-C 영역
	bOK	= TRUE;
	dXDisOfLeft	= DBStd.m_pDrawCrossSection->GetDistForCrossSection_ArchRib(2, bOK);
	dXDisOfRight	= dXDisOfLeft;
	if(bOK)//dOptDraw & REBAR_STD_SECT)
	{
		sSection.Format("SECTION %c-%c", ch, ch);
		ch++;

		if(m_pStd->IsDrawByBlock(MPE_REBAR_ARCHRIB_SECTION_C))
		{
			pPageMng->SetProgressData(_T(sSection + " 출력중..."), 80);
			rcCrossSectionC	= DBStd.m_pDrawCrossSection->DrawRebarCrossSection_ArchRib_Std(&Dom, dXDisOfLeft, dXDisOfRight, TRUE, TRUE);
			xyMove		= CDPoint(rcCrossSectionB.right - rcCrossSectionC.left + Dom.Always(150), rcCrossSectionB.bottom - rcCrossSectionC.bottom);
			Dom.Move(xyMove);
			rcCrossSectionC.left		+= xyMove.x; rcCrossSectionC.right	+= xyMove.x;
			rcCrossSectionC.top		+= xyMove.y; rcCrossSectionC.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_REBAR_ARCHRIB_SECTION_C));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_ARCHRIB_SECTION_C, _T(sSection));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_REBAR_ARCHRIB_SECTION_C_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T(sSection), CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_REBAR_ARCHRIB_SECTION_C_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_ARCHRIB_SECTION_C_TITLE, _T(sSection + " 타이틀"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcCrossSectionC.left	= (rcCrossSectionB.left + rcCrossSectionB.right)/2;
		rcCrossSectionC.bottom	= rcCrossSectionB.bottom - Dom.Always(50);;
		rcCrossSectionC.right	= rcCrossSectionC.left;
		rcCrossSectionC.top		= rcCrossSectionC.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////
	pBri->GetTvVertSection_Std(0, !bVertDir, FALSE, TRUE);
}

// 커플러 상세도 추가
void CDrawingRebar::AddPageCoupler(CDomyun *pDom, CDRect rcDogak, CTypedPtrArray <CObArray, CRebar*> *pArrRebar)
{
	CARcBridgeDrawingStd* pStd	= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri		= GetRcBridgeRebar();
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	DWORD dOptDraw	= pData->m_dwDrawRebar;
	if(!pBri) return;
	if(!pBri->IsCoupler(pArrRebar)) return;

	CDomyun Dom(pDom);

	// Coupler 상세도  /////////////////////////////////////////////////////////////////////////
	CDRect rect(0, 0, 0, 0);
// 	CString sTitle	= _T("");
	CDPoint xyMove(0, 0);
	CDRect rectCouplerDetail(0, 0, 0, 0);
	CDRect rectCouplerDetailTitle(0, 0, 0, 0);
	CString sTitle	= _T("COUPLER 상세도");
	if(dOptDraw & REBAR_DETAIL_COUPLER && pStd->IsDrawByBlock(MPE_DETAIL_COUPLER))
	{
		//pPageMng->SetProgressData(_T(sTitle + " 출력중..."), nProgress += nOffset);

		pStd->DrawCouplerDetail(&Dom, pArrRebar);
		
		Dom.SetCalcExtRect();
		rectCouplerDetail	= Dom.GetExtRect();

		xyMove		= CDPoint(rcDogak.right - rectCouplerDetail.left + Dom.Always(30), rcDogak.top - rectCouplerDetail.top + Dom.Always(50));
		Dom.Move(xyMove);
		rectCouplerDetail.left	+= xyMove.x; rectCouplerDetail.right	+= xyMove.x;
		rectCouplerDetail.top	+= xyMove.y; rectCouplerDetail.bottom	+= xyMove.y;
		Dom.SetCalcExtRect();
		rect	= Dom.GetExtRect();

		Dom.Move(pMove->GetPoint(MPE_DETAIL_COUPLER));
		pStd->ChangeSeperateLayer(&Dom);
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_DETAIL_COUPLER, sTitle);
	
		*pDom << Dom;
	}
	else
	{
		rectCouplerDetail.left		= rcDogak.right - Dom.Always(30);
		rectCouplerDetail.right		= rectCouplerDetail.left;
		rectCouplerDetail.bottom	= rcDogak.top;
		rectCouplerDetail.top		= rcDogak.top;
	}

	// 커플러 규격(COUPLER SIZE)  ////////////////////////////////////////////////////////////
	CDRect rectCouplerTable(0, 0, 0, 0);
	CDRect rectCouplerTableTitle(0, 0, 0, 0);
	sTitle	= _T("커플러 규격(COUPLER SIZE)");
	if(dOptDraw & REBAR_DETAIL_COUPLER && pStd->IsDrawByBlock(MPE_TABLE_COUPLER))
	{
		//pPageMng->SetProgressData(_T(sTitle + " 출력중..."), nProgress += nOffset);

		pStd->DrawTableCouplerDetail(&Dom, pArrRebar);
		
		Dom.SetCalcExtRect();
		rectCouplerTable	= Dom.GetExtRect();

		xyMove		= CDPoint(rectCouplerDetail.left - rectCouplerTable.left, rectCouplerDetail.top - rectCouplerTable.bottom - Dom.Always(30));
		Dom.Move(xyMove);
		rectCouplerDetail.left	+= xyMove.x; rectCouplerDetail.right	+= xyMove.x;
		rectCouplerDetail.top	+= xyMove.y; rectCouplerDetail.bottom	+= xyMove.y;
		Dom.SetCalcExtRect();
		rect	= Dom.GetExtRect();

		Dom.Move(pMove->GetPoint(MPE_TABLE_COUPLER));
		pStd->ChangeSeperateLayer(&Dom);
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TABLE_COUPLER, sTitle);
	
		*pDom << Dom;
	}
	else
	{
		rectCouplerTable.left		= rectCouplerDetail.left;
		rectCouplerTable.right		= rectCouplerDetail.left;
		rectCouplerTable.bottom		= rectCouplerDetail.top;
		rectCouplerTable.top		= rectCouplerDetail.top;
	}


	if(dOptDraw & REBAR_DETAIL_COUPLER && pStd->IsDrawByBlock(MPE_TABLE_COUPLER_TITLE))
	{
		pStd->AddTitleOnDom(&Dom, m_nBridge, sTitle, CDPoint(0, 0), 0, Dom.GetScaleDim());
		Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
		Dom.Move(pMove->GetPoint(MPE_TABLE_COUPLER_TITLE));
		pStd->ChangeSeperateLayer(&Dom);
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TABLE_COUPLER_TITLE, sTitle + "타이틀");
		*pDom << Dom;
	}
	else
		rectCouplerTableTitle	= rectCouplerDetail;
	//////////////////////////////////////////////////////////////////////////////////

	*pDom << Dom;
}

void CDrawingRebar::AddPageRebarLowerSlab( CDomyun *pDomP, CDRect rcDogak )
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;
	double dScale	= GetScale();
	DWORD dOptDraw	= pData->m_dwDrawRebar;

	// (ARCBRIDGE-3516) 수평철근 치수 기준위치 옵션 추가(0: 좌우측 끝단, 1: 선형위치)
	DBStd.m_pDrawPlane->m_nPosBaseDimSupportRebar = pData->m_nBasePosPlaneSupportRebarDim;

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScale);

	BOOL bVertDir	= pBri->IsVertDir();
	//pBri->SyncBridge(0, !bVertDir);
	pBri->GetTvVertSection_Std(0, !bVertDir, FALSE);
	pBri->SyncBridgeRebar(FALSE, bVertDir?-1:1, TRUE);

	CString str	= _T("");

	CDPoint xyMove(0, 0);
	CDRect rect(0, 0, 0, 0);
	// 하부슬래브 구조도 조립도 //////////////////////////////////////////////////////////////////////
	CDRect rcLowerSlab[2]	= {(0, 0, 0, 0), (0, 0, 0, 0)};	// 주철근 조립도 영역 영역
	for(long upper = 0; upper < 2; upper++)
	{
		BOOL bUpper	= upper == iUPPER;
		if(!pData->m_bSepUpperAndLowerSlabRebar && !bUpper) continue;

		CString sTitle	= "평면도";
		if(pData->m_bSepUpperAndLowerSlabRebar)
			sTitle	= bUpper ? "평면도(상면)" : "평면도(하면)";

		if(dOptDraw & REBAR_LOWER_SLAB)
		{
			if(m_pStd->IsDrawByBlock(MPE_REBAR_LOWERSLAB+upper))
			{
				pPageMng->SetProgressData(sTitle + " 출력중...", 30);
				if(pData->m_bSepUpperAndLowerSlabRebar)
					rcLowerSlab[upper] = DBStd.m_pDrawPlane->DrawRebar_Plane_LowerSlabStd(&Dom, -1, bUpper, bUpper, TRUE, TRUE, TRUE, pData->m_nDirMarkUpperSlabMainRebar, FALSE, !pData->m_bDrawHunchRebarOnly);
				else
					rcLowerSlab[upper] = DBStd.m_pDrawPlane->DrawRebar_Plane_LowerSlabStd(&Dom, -1, TRUE, FALSE, TRUE, TRUE, TRUE, pData->m_nDirMarkUpperSlabMainRebar, FALSE, !pData->m_bDrawHunchRebarOnly);
				if(bUpper)
					xyMove		= CDPoint(rcDogak.left-rcLowerSlab[upper].left+Dom.Always(300), rcDogak.bottom-rcLowerSlab[upper].bottom-Dom.Always(150)); 
				else
					xyMove		= CDPoint(rcDogak.left-rcLowerSlab[upper].left+Dom.Always(300), rcLowerSlab[iUPPER].bottom-rcLowerSlab[upper].top+Dom.Always(50)); 
				Dom.Move(xyMove);
				rcLowerSlab[upper].left	+= xyMove.x; rcLowerSlab[upper].right	+= xyMove.x;
				rcLowerSlab[upper].top		+= xyMove.y; rcLowerSlab[upper].bottom	+= xyMove.y;
				Dom.SetCalcExtRect();
				rect	= Dom.GetExtRect();

				Dom.Move(pMove->GetPoint(MPE_REBAR_LOWERSLAB+upper));
				pStd->ChangeSeperateLayer(&Dom);
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_LOWERSLAB+upper, sTitle);
				*pDomP << Dom;
			}

			if(m_pStd->IsDrawByBlock(MPE_REBAR_LOWERSLAB_TITLE+upper))
			{
				CString sTitleOnDom;
				if(pData->m_bSepUpperAndLowerSlabRebar)
					sTitleOnDom = bUpper ? "하부슬래브(상면) 구조도" : "하부슬래브(하면) 구조도";
				else
					sTitleOnDom = "하부슬래브 구조도";
				pStd->AddTitleOnDom(&Dom, m_nBridge, _T(sTitleOnDom), CDPoint(0,0), 0, dScale);
				Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
				Dom.Move(pMove->GetPoint(MPE_REBAR_LOWERSLAB_TITLE+upper));
				pStd->ChangeSeperateLayer(&Dom);
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_LOWERSLAB_TITLE+upper, sTitle + " 타이틀");
				*pDomP << Dom;
			}
		}
		else
		{
			rcLowerSlab[upper].left	= rcDogak.left+Dom.Always(300);
			rcLowerSlab[upper].right	= rcLowerSlab[upper].left;
			rcLowerSlab[upper].bottom	= rcDogak.bottom - Dom.Always(150);
			rcLowerSlab[upper].top		= rcLowerSlab[upper].bottom;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////
	// 헌치 철근 구조도 따로 출력
	if(pData->m_bDrawHunchRebarOnly)
	{
		CString sTitle	= "하부슬래브 헌치부 상세";
		if(m_pStd->IsDrawByBlock(MPE_REBAR_LOWER_HUNCH))
		{
			pPageMng->SetProgressData(sTitle + " 출력중...", 30);

			CDRect rcHunch = DBStd.m_pDrawPlane->DrawRebar_Plane_SlabHunch(&Dom, FALSE, TRUE, TRUE, TRUE, pData->m_nDirMarkUpperSlabMainRebar);

			xyMove		= CDPoint(rcDogak.right-rcHunch.left+Dom.Always(300), rcDogak.bottom-rcHunch.bottom-Dom.Always(150)); 

			Dom.Move(xyMove);
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_REBAR_LOWER_HUNCH));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_LOWER_HUNCH, sTitle);
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_REBAR_LOWER_HUNCH_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, sTitle, CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_REBAR_LOWER_HUNCH_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_LOWER_HUNCH_TITLE, sTitle + " 타이틀");
			*pDomP << Dom;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////

	// 횡단면도 구조도 조립도 //////////////////////////////////////////////////////////////////////
	CDRect rcCrossSection(0, 0, 0, 0);	// 주철근 조립도 영역 영역
	if(dOptDraw & REBAR_CROSSSECTION)
	{
		BOOL bExcludeDoupleMainRebar	= pData->m_bExcludeDoupleMainRebarCrossSection;

		double dStaLeft = pBri->GetStationOnJijum(0);
		double dStaRight = (pBri->GetStationOnJijum(0, 0)+pBri->GetStationOnJijum(1, 0))/2;
		if(!pBri->IsOutWall(TRUE))
			dStaLeft	= pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, FALSE);

		if(m_pStd->IsDrawByBlock(MPE_REBAR_CROSSECTION))
		{
			pPageMng->SetProgressData(_T("횡단면도 출력중..."), 60);

			// 확폭인경우..
			if(pBri->IsExtLine())
			{
				rcCrossSection	= DBStd.m_pDrawCrossSection->DrawRebar_CrossSectionStd_LowerSlab(&Dom, dStaLeft, TRUE, TRUE, TRUE, TRUE, TRUE, bExcludeDoupleMainRebar);
			}
			else
			{
				rcCrossSection	= DBStd.m_pDrawCrossSection->DrawRebar_CrossSectionStd_LowerSlab(&Dom, dStaLeft, dStaRight, TRUE, TRUE, TRUE, TRUE, TRUE, bExcludeDoupleMainRebar);
			}
			xyMove			= CDPoint(rcLowerSlab[iUPPER].left-rcCrossSection.right-Dom.Always(100), rcLowerSlab[iUPPER].top-rcCrossSection.top); 
			if(pBri->m_bIsExp)
			{
				double dDiff	= max(pBri->m_dExpSttH[1], pBri->m_dExpEndH[1]);
				xyMove.y += dDiff;
			}
			Dom.Move(xyMove);
			rcCrossSection.left	+= xyMove.x; rcCrossSection.right	+= xyMove.x;
			rcCrossSection.top	+= xyMove.y; rcCrossSection.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_REBAR_CROSSECTION));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_CROSSECTION, _T("횡단면도"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_REBAR_CROSSECTION_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("횡단면도"), CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_REBAR_CROSSECTION_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_CROSSECTION_TITLE, _T("횡단면도 타이틀"));
			*pDomP << Dom;
		}
	}
	else
	{	
		rcCrossSection.right = rcLowerSlab[iUPPER].left - Dom.Always(100);
		rcCrossSection.bottom	= rcLowerSlab[iUPPER].top;
		rcCrossSection.left	= rcCrossSection.right;
		rcCrossSection.top	= rcCrossSection.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////

	// 확폭인 경우 종점측 횡단면도도 그려준다.
	if(pBri->IsExtLine())
	{
		long nAddCross	= 1;
		CDRect rcCrossSectionEnd(0, 0, 0, 0);
		if(dOptDraw & REBAR_CROSSSECTION)
		{
			BOOL bExcludeDoupleMainRebar	= pData->m_bExcludeDoupleMainRebarCrossSection;

			double dStaLeft = pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, FALSE);

			//#0031385 dStaLeft과 철근의 단면과 교차점을 찾게된다. 그러나 철근은 90도 각도로 임의로 그려져 있기 때문에 교차점을 찾기 위하여 스큐가 적용된 각도만큼 보정해준다.
			if(dStaLeft > pBri->m_dStationBridgeStt + pBri->m_dLengthBridge)
			{
				CDPoint vAng = pBri->GetJijum(pBri->m_nQtyJigan)->m_vAngle;

				dStaLeft = pBri->m_dStationBridgeStt + (pBri->m_dLengthBridge * vAng.y);
			}

			if(m_pStd->IsDrawByBlock(MPE_REBAR_CROSSECTION+nAddCross))
			{
				pPageMng->SetProgressData(_T("횡단면도 출력중..."), 60);

				// 확폭인경우..
				rcCrossSectionEnd	= DBStd.m_pDrawCrossSection->DrawRebar_CrossSectionStd_LowerSlab(&Dom, dStaLeft, TRUE, TRUE, TRUE, TRUE, TRUE, bExcludeDoupleMainRebar);
				xyMove			= CDPoint(rcLowerSlab[iUPPER].right-rcCrossSectionEnd.right+Dom.Always(200), rcLowerSlab[iUPPER].top-rcCrossSectionEnd.top); 
				Dom.Move(xyMove);
				rcCrossSectionEnd.left	+= xyMove.x; rcCrossSectionEnd.right	+= xyMove.x;
				rcCrossSectionEnd.top	+= xyMove.y; rcCrossSectionEnd.bottom	+= xyMove.y;
				Dom.SetCalcExtRect();
				rect	= Dom.GetExtRect();

				Dom.Move(pMove->GetPoint(MPE_REBAR_CROSSECTION+nAddCross));
				pStd->ChangeSeperateLayer(&Dom);
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_CROSSECTION+nAddCross, _T("횡단면도"));
				*pDomP << Dom;
			}

			if(m_pStd->IsDrawByBlock(MPE_REBAR_CROSSECTION_TITLE+nAddCross))
			{
				pStd->AddTitleOnDom(&Dom, m_nBridge, _T("횡단면도(종점슬래브끝단)"), CDPoint(0,0), 0, dScale);
				Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
				Dom.Move(pMove->GetPoint(MPE_REBAR_CROSSECTION_TITLE+nAddCross));
				pStd->ChangeSeperateLayer(&Dom);
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_CROSSECTION_TITLE+nAddCross, _T("횡단면도 타이틀"));
				*pDomP << Dom;
			}
		}

		// 중앙부(헌치 없는 부분)
		nAddCross++;
		CDRect rcCrossSectionCen(0, 0, 0, 0);
		if(dOptDraw & REBAR_CROSSSECTION)
		{
			BOOL bExcludeDoupleMainRebar	= pData->m_bExcludeDoupleMainRebarCrossSection;

			double dStaLeft = pBri->GetStationOnJijum(0, 0) + pBri->GetLengthJigan(0)/2;
			if(m_pStd->IsDrawByBlock(MPE_REBAR_CROSSECTION+nAddCross))
			{
				pPageMng->SetProgressData(_T("횡단면도 출력중..."), 60);

				// 확폭인경우..
				rcCrossSectionCen	= DBStd.m_pDrawCrossSection->DrawRebar_CrossSectionStd_LowerSlab(&Dom, dStaLeft, TRUE, TRUE, TRUE, TRUE, TRUE, bExcludeDoupleMainRebar);
				xyMove			= CDPoint(rcCrossSectionEnd.right-rcCrossSectionCen.right+Dom.Always(150), rcLowerSlab[iUPPER].top-rcCrossSectionCen.top); 
				Dom.Move(xyMove);
				rcCrossSectionCen.left	+= xyMove.x; rcCrossSectionCen.right	+= xyMove.x;
				rcCrossSectionCen.top	+= xyMove.y; rcCrossSectionCen.bottom	+= xyMove.y;
				Dom.SetCalcExtRect();
				rect	= Dom.GetExtRect();

				Dom.Move(pMove->GetPoint(MPE_REBAR_CROSSECTION+nAddCross));
				pStd->ChangeSeperateLayer(&Dom);
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_CROSSECTION+nAddCross, _T("횡단면도"));
				*pDomP << Dom;
			}

			if(m_pStd->IsDrawByBlock(MPE_REBAR_CROSSECTION_TITLE+nAddCross))
			{
				pStd->AddTitleOnDom(&Dom, m_nBridge, _T("횡단면도(지간1중앙부)"), CDPoint(0,0), 0, dScale);
				Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
				Dom.Move(pMove->GetPoint(MPE_REBAR_CROSSECTION_TITLE+nAddCross));
				pStd->ChangeSeperateLayer(&Dom);
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_CROSSECTION_TITLE+nAddCross, _T("횡단면도 타이틀"));
				*pDomP << Dom;
			}
		}
	}
	pBri->GetTvVertSection_Std(0, !bVertDir, FALSE, TRUE);

	//////////추가 함형일 경우 하부 슬래브 둔각부
	if(pBri->IsBoxType())
	{
		CDPoint xyLast	= CDPoint(rcLowerSlab[iUPPER].left, rcLowerSlab[iUPPER].top-Dom.Always(70));
		long nIdx	= 0;
		for(long stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;

			for(long left = 0; left < 2; left++)
			{
				BOOL bLeft	= left == iLEFT;

				if(!pBri->m_bIsObtuseAngle_LowerSlab[stt][left]) continue;

				long nMPE	= MPE_DETAIL_OBTUSE_STT_LEFT;
				long nMPETitle	= MPE_DETAIL_OBTUSE_STT_LEFT_TITLE;
				CString sTitle	= _T("");
				CDRect rectObtuse[2];
				rectObtuse[0]	= CDRect(0, 0, 0, 0);
				rectObtuse[1]	= CDRect(0, 0, 0, 0);
				for(long upper = 0; upper < 2; upper++)
				{
					BOOL bUpper	= upper == iUPPER;

					CString sUpper	= bUpper ? "(상면)" : "(하면)";
					if(bStt)
					{
						nMPE		= bLeft ? MPE_DETAIL_OBTUSE_STT_LEFT+upper : MPE_DETAIL_OBTUSE_STT_RIGHT+upper;
						nMPETitle	= bLeft ? MPE_DETAIL_OBTUSE_STT_LEFT_TITLE+upper : MPE_DETAIL_OBTUSE_STT_RIGHT_TITLE+upper;
						sTitle		= bLeft ? "둔각부(시점좌측) 보강 상세"+sUpper : "둔각부(시점우측) 보강 상세"+sUpper;
					}
					else
					{
						nMPE		= bLeft ? MPE_DETAIL_OBTUSE_END_LEFT+upper: MPE_DETAIL_OBTUSE_END_RIGHT+upper;
						nMPETitle	= bLeft ? MPE_DETAIL_OBTUSE_END_LEFT_TITLE+upper : MPE_DETAIL_OBTUSE_END_RIGHT_TITLE+upper;
						sTitle		= bLeft ? "둔각부(종점좌측) 보강 상세"+sUpper : "둔각부(종점우측) 보강 상세"+sUpper;
					}

					nIdx++;
					double dScaleDetail	= pData->m_dScaleRebarDetailSlabAcute;
					Dom.SetScaleDim(dScaleDetail);

					if(m_pStd->IsDrawByBlock(nMPE))
					{
						pPageMng->SetProgressData(sTitle+" 출력중...", 60+((nIdx+1)*5));
						DBStd.m_pDrawPlane->DrawPlaneGenObtuse(&Dom, bStt, bLeft, bUpper, TRUE);
						DBStd.m_pDrawPlane->DrawRebarPlane_Slab_Obtuse(&Dom, FALSE, bStt, bLeft, bUpper, TRUE, TRUE, FALSE);
						Dom.RedrawByScale(dScale/dScaleDetail);

						// 회전
						CDPoint vAng	= pBri->GetAngleAzimuthJijum(bStt ? 0 : pBri->m_nQtyJigan);
						Dom.Rotate(CDPoint(0, 0), ToDPointFrDegree(90-vAng.GetAngleDegree()+180));


						Dom.SetCalcExtRect();
						rectObtuse[upper]	= Dom.GetExtRect();

						if(bUpper)
							xyMove	= CDPoint(xyLast.x - rectObtuse[upper].left, xyLast.y - rectObtuse[upper].bottom);
						else
							xyMove	= CDPoint(xyLast.x - rectObtuse[upper].left, rectObtuse[iUPPER].top - rectObtuse[upper].bottom);
						Dom.Move(xyMove);
						rectObtuse[upper].left	+= xyMove.x; rectObtuse[upper].right	+= xyMove.x;
						rectObtuse[upper].top	+= xyMove.y; rectObtuse[upper].bottom	+= xyMove.y;
						Dom.SetCalcExtRect();
						rect	= Dom.GetExtRect();

						Dom.Move(pMove->GetPoint(nMPE));
						pStd->ChangeSeperateLayer(&Dom);
						pMove->AddMPE(&Dom, Dom.GetExtRect(), nMPE, sTitle);
						*pDomP << Dom;	
					}


					if(m_pStd->IsDrawByBlock(nMPETitle))
					{
						pStd->AddTitleOnDom(&Dom, m_nBridge, sTitle, CDPoint(0,0), 0, dScaleDetail);
						Dom.RedrawByScale(dScale/dScaleDetail);
						Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
						Dom.Move(pMove->GetPoint(nMPETitle));
						pStd->ChangeSeperateLayer(&Dom);
						Dom.SetCalcExtRect();
						pMove->AddMPE(&Dom, Dom.GetExtRect(), nMPETitle, sTitle + " 타이틀");
						*pDomP << Dom;
					}
				}

				// 종단면 상세
				if(bStt)
				{
					nMPE		= bLeft ? MPE_DETAIL_OBTUSE_STT_LEFT+2 : MPE_DETAIL_OBTUSE_STT_RIGHT+2;
					nMPETitle	= bLeft ? MPE_DETAIL_OBTUSE_STT_LEFT_TITLE+2 : MPE_DETAIL_OBTUSE_STT_RIGHT_TITLE+2;
					sTitle		= bLeft ? "둔각부(시점좌측) 보강 상세(종단면)" : "둔각부(시점우측) 보강 상세(종단면)";
				}
				else
				{
					nMPE		= bLeft ? MPE_DETAIL_OBTUSE_END_LEFT+2: MPE_DETAIL_OBTUSE_END_RIGHT+2;
					nMPETitle	= bLeft ? MPE_DETAIL_OBTUSE_END_LEFT_TITLE+2 : MPE_DETAIL_OBTUSE_END_RIGHT_TITLE+2;
					sTitle		= bLeft ? "둔각부(종점좌측) 보강 상세(종단면)" : "둔각부(종점우측) 보강 상세(종단면)";
				}

				CDRect rectObtuseVert	= rectObtuse[iLOWER];
				double dScaleDetail	= pData->m_dScaleRebarDetailSlabAcute;
				if(m_pStd->IsDrawByBlock(nMPE))
				{
					DBStd.m_pDrawVertSection->DrawRebarVertSectionObtuseStd(&Dom, bStt, bLeft, TRUE, TRUE, TRUE);
					Dom.RedrawByScale(dScale/dScaleDetail);

					Dom.SetCalcExtRect();
					rectObtuseVert	= Dom.GetExtRect();
					xyMove	= CDPoint(rectObtuse[iLOWER].left - rectObtuseVert.left, rectObtuse[iLOWER].top - rectObtuseVert.bottom - Dom.Always(20));
					Dom.Move(xyMove);
					rectObtuseVert.left	+= xyMove.x; rectObtuseVert.right	+= xyMove.x;
					rectObtuseVert.top	+= xyMove.y; rectObtuseVert.bottom	+= xyMove.y;
					Dom.SetCalcExtRect();
					rect	= Dom.GetExtRect();

					Dom.Move(pMove->GetPoint(nMPE));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), nMPE, sTitle);
					*pDomP << Dom;	
				}

				if(m_pStd->IsDrawByBlock(nMPETitle))
				{
					pStd->AddTitleOnDom(&Dom, m_nBridge, sTitle, CDPoint(0,0), 0, dScaleDetail);
					Dom.RedrawByScale(dScale/dScaleDetail);
					Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
					Dom.Move(pMove->GetPoint(nMPETitle));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), nMPETitle, sTitle + " 타이틀");
					*pDomP << Dom;			
					xyLast.x	= Dom.GetExtRect().right + Dom.Always(20);
				}
			}
		}

	}





	// 보기둥 단면도에서 표현이 불가능한 거더 상세는 여기서 출력한다.
	CDPoint xyLast	= CDPoint(rcLowerSlab[iUPPER].left, rcLowerSlab[iUPPER].top-Dom.Always(150));
	CDRect rcGirder(xyLast.x, xyLast.y, xyLast.x, xyLast.y);	// 거더 단면도
	long nIdxGirder	= 0;
	for(long jijum = 0; jijum < pBri->GetCountJijum(); jijum++)
	{
		if(!pBri->IsGirderRebar(jijum, FALSE)) continue;
		if((jijum != 0 && jijum != pBri->m_nQtyJigan) && pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB) continue;

		// 기둥평면도 오른쪽에 거더 단면도 넣는다
		// 거더 단면도 //////////////////////////////////////////////////////////////
		double dScaleDetail	= pData->m_dScaleRebarDetailGirderSection;
		Dom.SetScaleDim(dScaleDetail);


		//pBri->SyncBridge(0, TRUE);
		pBri->GetTvVertSection_Std(0, TRUE, FALSE);
		if(dOptDraw & REBAR_GIRDER_SECT && pBri->IsGirderRebar(jijum, FALSE))
		{
			if(m_pStd->IsDrawByBlock(MPE_REBAR_GIRDERSEC+nIdxGirder))
			{
				pPageMng->SetProgressData(_T("거더 단면도 출력중..."), 100);
				DBStd.m_pDrawVertSection->DrawRebarAll_SlabGirder(&Dom, jijum, TRUE, TRUE, -1, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, -1, FALSE, FALSE);
 				DBStd.m_pDrawVertSection->DrawDetail_Girder(&Dom, jijum);
				Dom.RedrawByScale(dScale/dScaleDetail);
				Dom.SetCalcExtRect();
				rcGirder	= Dom.GetExtRect();
				xyMove		= CDPoint(xyLast.x-rcGirder.left+Dom.Always(150), xyLast.y-rcGirder.bottom); 
				Dom.Move(xyMove);
				rcGirder.left	+= xyMove.x; rcGirder.right		+= xyMove.x;
				rcGirder.top	+= xyMove.y; rcGirder.bottom	+= xyMove.y;
				Dom.SetCalcExtRect();
				rect	= Dom.GetExtRect();

				Dom.SetCalcExtRect();
				Dom.Move(pMove->GetPoint(MPE_REBAR_GIRDERSEC+nIdxGirder));
				pStd->ChangeSeperateLayer(&Dom);
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_GIRDERSEC+nIdxGirder, _T("거더 단면도"));
				*pDomP << Dom;
			}

			if(m_pStd->IsDrawByBlock(MPE_REBAR_GIRDERSEC_TITLE+nIdxGirder))
			{
				pStd->AddTitleOnDom(&Dom, m_nBridge, _T("거더 단면도"), CDPoint(0,0), 0, dScaleDetail);
				Dom.RedrawByScale(dScale/dScaleDetail);
				Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
				Dom.Move(pMove->GetPoint(MPE_REBAR_GIRDERSEC_TITLE+nIdxGirder));
				pStd->ChangeSeperateLayer(&Dom);
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_GIRDERSEC_TITLE+nIdxGirder, _T("거더 단면도 타이틀"));
				*pDomP << Dom;
			}
			nIdxGirder++;
		}
		else
		{
			rcGirder.left		= xyLast.x + Dom.Always(150);
			rcGirder.bottom		= xyLast.y;
			rcGirder.right		= rcGirder.left;
			rcGirder.top		= rcGirder.bottom;
		}
		xyLast.x	= rcGirder.right;
		xyLast.y	= rcGirder.bottom;
		/////////////////////////////////////////////////////////////////////////////
		pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE);
	}
}

void CDrawingRebar::DrawRebarDetailTable_RawRebar( CDomyun *pDom, CDPoint &xyBase, long nType, BOOL bStt, BOOL bLeft, long nJ )
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);

	if(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->IsApplyExtraChargeAtRebarJoint() == FALSE && pOpt->m_bAddRawRebarTable)
	{
		CTypedPtrArray <CObArray, CRebar*> arrRebar;
		CTypedPtrArray <CObArray, CRebar*> arrRawRebar;

		CDRect rcDogak;
		CHgBaseConcStd baseConc;
		long nIdxTable = nType == 1 ? 1 : 4;

		CString sTitle[2] = {_T(""), _T("")};
		const long nCountTable = pData->m_nTypeOutRebarTable + 1;

		for(long table = 0; table < nCountTable; table++)
		{
			if(pData->m_nTypeOutRebarTable == 1) //주철근, 기타철근
			{
				sTitle[0] = _T("주철근");
				sTitle[1] = _T("기타철근");

				MakeRebarList(&arrRebar, nType, bStt, bLeft, nJ, table==0 ? 1 : 2);
			}
			else
			{
				MakeRebarList(&arrRebar, nType, bStt, bLeft, nJ, 0);
			}

			long nCountRaw = baseConc.GetRawRebarList(&arrRebar, &arrRawRebar, pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->GetLenghtRebarMax());
			if(nCountRaw > 0)
			{
				if(pData->m_bRebarTableSepByFy)
				{
					double dFy	= 1;
					while(dFy != 0)
					{
						CTypedPtrArray <CObArray, CRebar*> arrRebarGet;
						dFy	= pStd->GetRebarListByFy(&arrRawRebar, &arrRebarGet, dFy);
						if(dFy == 0) break;
						if(arrRebarGet.GetSize() == 0) break;

						baseConc.ShellSortRebarList(&arrRebarGet, &CRebar::CompareForTable);
						xyBase = DrawRebarTable(pDom, &arrRebarGet, sTitle[table]+_T("미가공 철근"), nIdxTable++, xyBase, rcDogak);
					}
				}
				else
				{
					baseConc.ShellSortRebarList(&arrRawRebar, &CRebar::CompareForTable);
					xyBase = DrawRebarTable(pDom, &arrRawRebar, sTitle[table]+_T("미가공 철근"), nIdxTable++, xyBase, rcDogak);
				}
			}

			AhTPADelete(&arrRawRebar, (CRebar*)0);
			AhTPADelete(&arrRebar,	(CRebar*)0);
		}
	}
}
