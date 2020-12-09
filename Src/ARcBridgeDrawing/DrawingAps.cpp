// DrawingAps.cpp: implementation of the CDrawingAps class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"
#include "../../Src/ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "../ARcBridgeDrawingOption/ARcBridgeDrawingOption.h"
#include "ARcBridgeDrawing.h"
#include "DrawingAps.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define MPE_REBAR_SLABPLANE				10		// 슬래브 평면 구조도
#define MPE_REBAR_SLABPLANE_TITLE		20		// 슬래브 평면 구조도 제목
#define MPE_REBAR_SLABFRONT				30		// 슬래브 정면 구조도
#define MPE_REBAR_SLABFRONT_TITLE		40		// 슬래브 정면 구조도 제목
#define MPE_REBAR_SLABSIDE				50		// 슬래브 측면 구조도
#define MPE_REBAR_SLABSIDE_TITLE		60		// 슬래브 측면 구조도 제목
#define MPE_REBAR_SLABDETAIL			70		// 슬래브 철근상세
#define MPE_REBAR_SLABDETAIL_TITLE		80		// 슬래브 철근상세 제목
#define MPE_REBAR_SLABDETAILA			90		// 슬래브 상세 A
#define MPE_REBAR_SLABDETAILA_TITLE		100		// 슬래브 상세 A 제목
#define MPE_REBAR_SLABTABLEM			110		// 슬래브 재료표
#define MPE_REBAR_SLABTABLEM_TITLE		120		// 슬래브 재료표 제목
#define MPE_REBAR_SLABTABLE				130		// 슬래브 철근 재료표
#define MPE_REBAR_SLABTABLE_TITLE		140		// 슬래브 철근 재료표 제목
#define MPE_REBAR_SLABNOTE				150		// 슬래브 기본노트
#define MPE_REBAR_SLABNOTE_TITLE		160		// 슬래브 기본 노트 제목


CDrawingAps::CDrawingAps()
{
	m_nBridge = -1;
}

CDrawingAps::~CDrawingAps()
{

}

long CDrawingAps::GetPageSu()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;

	if(pStd->GetCountTotalPage()<=0)
		pStd->MakePageInfo();

	m_nPage	= pStd->GetCountCategoryPage(_T("접속 슬래브"));

	return m_nPage;
}

CString CDrawingAps::GetTitle()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("접속 슬래브"), pStd->m_nCurPage-m_nSttPage);
	if(pInfo==NULL) return _T("");

	CString strTitle = pStd->GetStringTitle(pStd->m_pARcBridgeDataStd->GetBridge(m_nBridge), pInfo->m_strTitle);

	return strTitle;
}

double CDrawingAps::GetScale()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pOptData = pOpt->GetDrawingOption(m_nBridge);

	m_Scale	= pOptData->m_dScaleRebarAps;
	return pOptData->m_dScaleRebarAps;
}

void CDrawingAps::SetNumber(long nSubPage)
{
	m_nBridge = GetNumberBridge(nSubPage-m_nSttPage);
}

long CDrawingAps::GetNumberBridge(long nCategoryPageNum)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("접속 슬래브"), nCategoryPageNum);
	if(pInfo==NULL) return -1;

	return pInfo->m_nBri;
}


void CDrawingAps::DrawOnePage()
{
	TRACE("CDrawingAps");
	CARcBridgeDrawingStd* pStd				= (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDataStd* pDataStd			= pStd->m_pARcBridgeDataStd;
	CDrawDogak* pDgk						= pStd->GetDogak();	
	CDrawPageMng* pMng					= pStd->GetPageManage();
	

	long nPage = pStd->m_nCurPage;

	SetNumber(pStd->m_nDrawPage);
	double dScale = GetScale();

	CDomyun* pDom = pStd->GetDomyun(nPage);
	pDom->SetScaleDim(dScale);

	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("접속 슬래브"), pStd->m_nDrawPage-m_nSttPage);
	if(pInfo==NULL) return;

	nPage = pStd->GetPageNum(pInfo);

	pStd->m_pOptStd->SetEnvType(pDom, HCAD_MIS3);
	pStd->m_pOptStd->SetEnvType(pDom, HCAD_SYMB);

	CDomyun Dom(pDom);

	pMng->SetProgressData(_T("도각 출력 중..."), 0);

	pOpt->SyncApsDowelRebarSeperateRebarOption(m_nBridge);

	long nSttPage = 0;
	CString strDgkName = pOpt->m_strDgkName;
	CString strHeadNote = pOpt->m_strDomNumHead;
	CString strSmallTitle = GetTitle();
	CString strXRefPath = IsDxfOut() ? pOpt->m_strXRefPath : _T("");

	CRcBridgeApp* pBri = pDataStd->GetBridge(m_nBridge);
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

	// 접속 슬래브 출력
	long nPageNum	= pInfo->m_nPageNumber-1;	// 인덱스
	BOOL bDraw		= FALSE;

	long nCountApsSttLeft	= pBri->IsAps(TRUE, TRUE) ? 1 : 0;
	long nCountApsSttRight	= pBri->IsAps(TRUE, FALSE) ? 1 : 0;
	long nCountApsEndLeft	= pBri->IsAps(FALSE, TRUE) ? 1 : 0;
	long nCountApsEndRight	= pBri->IsAps(FALSE, FALSE) ? 1 : 0;
	
	// 시점, 좌측 접속슬래브
	if(nPageNum < nCountApsSttLeft && !bDraw && nCountApsSttLeft > 0)
	{
		AddPageAps(&Dom, TRUE, TRUE, rcDogak);
		bDraw	= TRUE;
	}
	else if(nCountApsSttLeft > 0)
		nPageNum -= 1;

	// 시점, 우측 접속슬래브
	if(nPageNum < nCountApsSttRight && !bDraw && nCountApsSttRight > 0)
	{
		AddPageAps(&Dom, TRUE, FALSE, rcDogak);
		bDraw	= TRUE;
	}
	else if(nCountApsSttRight > 0)
		nPageNum -= 1;

	// 종점, 좌측 접속슬래브
	if(nPageNum < nCountApsEndLeft && !bDraw && nCountApsEndLeft > 0)
	{
		AddPageAps(&Dom, FALSE, TRUE, rcDogak);
		bDraw	= TRUE;
	}
	else if(nCountApsEndLeft > 0)
		nPageNum -= 1;

	// 종점, 우측 접속슬래브
	if(nPageNum < nCountApsEndRight && !bDraw && nCountApsEndRight > 0)
	{
		AddPageAps(&Dom, FALSE, FALSE, rcDogak);
	}


	pMng->SetProgressData(_T("출력 완료  ..."), 100);

	*pDom << Dom;
}

void CDrawingAps::AddPageOne(CDomyun* pDomP, long nSubPage)
{

}

// 접속슬래브 도면 추가
/*
#define REBAR_APS_SIDE		0x00000001	// 단면도
#define REBAR_APS_FRONT		0x00000002	// 정면도
#define REBAR_APS_PLANE		0x00000004	// 평면도
#define REBAR_APS_DETAIL_RB	0x00000008	// 철근상세
#define REBAR_APS_DETAIL_A	0x00000010	// 상세A
#define REBAR_APS_TABLE_M	0x00000020	// 재료표
#define REBAR_APS_TABLE_RB	0x00000040	// 철근 재료표
#define REBAR_APS_NOTE		0x00000080	// 기본노트
*/
void CDrawingAps::AddPageAps(CDomyun *pDomP, BOOL bStt, BOOL bLeft, CDRect rcDogak)
{
	CARcBridgeDrawingStd* pStd				= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri						= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove					= pStd->GetMovePoint(pStd->m_nCurPage);
	DWORD dwOptDraw						= pData->m_dwDrawRebarAps;
	CHgBaseDrawStd baseDraw;
	

	

	long left		= bLeft ? iLEFT : iRIGHT;
	CApsApp *pAps	= bStt ? &pBri->m_apsStt[left] : &pBri->m_apsEnd[left];
	if(!pAps->m_bIs) return;
		
	pBri->GetTvVertSection_Std(0, TRUE, FALSE);
	pBri->SyncBridgeAps(0, FALSE);

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	double dScale	= GetScale();

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScale);
	
	CString str		= _T("");
	CString sAps	= bStt ? "시점측 접속슬래브" : "종점측 접속슬래브";
	sAps			+= bLeft ? "(좌측)" : "(우측)";

	long nCountDom	= 8;
	double dAddPer	= 100 / nCountDom;
	double dCurPer	= 0;

	CDPoint xyMove(0, 0);
	// 평면도 //////////////////////////////////////////////////////////////////////
	CDRect rcPlane(0, 0, 0, 0);	// 평면도 영역
	pPageMng->SetProgressData(_T("평면도 출력중..."), (int)(dCurPer+=dAddPer));
	if(dwOptDraw & REBAR_APS_PLANE)
	{
		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABPLANE))
		{
			rcPlane	= DBStd.m_pDrawAps->DrawRebar_PlaneStd(&Dom, pAps, FALSE);
			xyMove	= CDPoint(rcDogak.left-rcPlane.left+Dom.Always(250), rcDogak.bottom-rcPlane.bottom-Dom.Always(150));
			Dom.Move(xyMove);
			rcPlane.left	+= xyMove.x; rcPlane.right	+= xyMove.x; 
			rcPlane.top		+= xyMove.y; rcPlane.bottom	+= xyMove.y;

			Dom.Move(pMove->GetPoint(MPE_REBAR_SLABPLANE));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABPLANE, _T("평면도"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABPLANE_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("평면도"), CDPoint(0,0), 0, dScale);
			Dom.Move(rcPlane.left+rcPlane.Width()/2.0, rcPlane.bottom+Dom.Always(50));
			Dom.Move(pMove->GetPoint(MPE_REBAR_SLABPLANE_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABPLANE_TITLE, _T("평면도 타이틀"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcPlane.left	= rcDogak.left + Dom.Always(250);
		rcPlane.bottom	= rcDogak.bottom - Dom.Always(150);
		rcPlane.right	= rcPlane.left;
		rcPlane.top		= rcPlane.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////

	// 정면도(A-A) //////////////////////////////////////////////////////////////////////
	CDRect rcFront(0, 0, 0, 0);	// 정면도(A-A) 영역
	pPageMng->SetProgressData(_T("단면 A-A 출력중..."), (int)(dCurPer+=dAddPer));
	if(dwOptDraw & REBAR_APS_FRONT)
	{
		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABFRONT))
		{
			rcFront	= DBStd.m_pDrawAps->DrawRebar_FrontStd(&Dom, pAps, FALSE);
			xyMove	= CDPoint(rcPlane.left-rcFront.right-Dom.Always(100), rcPlane.bottom-rcFront.bottom);
			Dom.Move(xyMove);
			rcFront.left	+= xyMove.x; rcFront.right	+= xyMove.x;
			rcFront.top		+= xyMove.y; rcFront.bottom	+= xyMove.y;
			
			Dom.Move(pMove->GetPoint(MPE_REBAR_SLABFRONT));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABFRONT, _T("단면 A-A"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABFRONT_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("단면 A-A"), CDPoint(0,0), 0, dScale);
			Dom.Move(rcFront.left+rcFront.Width()/2.0, rcFront.bottom+Dom.Always(50));
			Dom.Move(pMove->GetPoint(MPE_REBAR_SLABFRONT_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABFRONT_TITLE, _T("단면 A-A 타이틀"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcFront.left	= rcPlane.left + Dom.Always(100);
		rcFront.bottom	= rcPlane.bottom;
		rcFront.right	= rcFront.left;
		rcFront.top		= rcFront.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////

	// 측면도(B-B) //////////////////////////////////////////////////////////////////////
	CDRect rcSide(0, 0, 0, 0);	// 측면도(B-B) 영역
	pPageMng->SetProgressData(_T("단면 B-B 출력중..."), (int)(dCurPer+=dAddPer));
	if(dwOptDraw & REBAR_APS_SIDE)
	{
		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABSIDE))
		{
			rcSide	= DBStd.m_pDrawAps->DrawRebar_SideStd(&Dom, pAps, FALSE);
			xyMove	= CDPoint(rcPlane.left-rcSide.left, rcPlane.top-rcSide.bottom-Dom.Always(100)); 
			Dom.Move(xyMove);
			rcSide.left		+= xyMove.x; rcSide.right	+= xyMove.x;
			rcSide.top		+= xyMove.y; rcSide.bottom	+= xyMove.y;

			Dom.Move(pMove->GetPoint(MPE_REBAR_SLABSIDE));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABSIDE, _T("단면 B-B"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABSIDE_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("단면 B-B"), CDPoint(0,0), 0, dScale);
			Dom.Move(rcSide.left+rcSide.Width()/2.0, rcSide.bottom+Dom.Always(50));
			Dom.Move(pMove->GetPoint(MPE_REBAR_SLABSIDE_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABSIDE_TITLE, _T("단면 B-B 타이틀"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcSide.left		= rcPlane.left;
		rcSide.top		= rcPlane.bottom - Dom.Always(100);
		rcSide.right	= rcSide.left;
		rcSide.bottom	= rcSide.top;
	}
	////////////////////////////////////////////////////////////////////////////////////

	// 철근 상세 ///////////////////////////////////////////////////////////////////////
	CHgBaseConcStd baseConc;
	CDRect rcDetailRB(0, 0, 0, 0);
	CTypedPtrArray <CObArray, CRebar*> arrRebar;	// 재료표용 철근리스트
	MakeRebarList(pAps, &arrRebar);

	if(pOpt->m_nTypeRebarTable == 0) //기본타입
	{
		if(dwOptDraw & REBAR_APS_DETAIL_RB)
		{
			pPageMng->SetProgressData(_T("철근상세 출력중..."), (int)(dCurPer+=dAddPer));
			baseConc.ShellSortRebarList(&arrRebar, &CRebar::CompareForList);
			DrawRebarDetail(&Dom, pAps, &arrRebar, CDPoint(rcPlane.right + Dom.Always(50), rcPlane.bottom));
			Dom.SetCalcExtRect();
			rcDetailRB	= Dom.GetExtRect();
			*pDomP << Dom;
		}
		
	}

	// 철근상세가 출력되지 않거나, 도면간소화타입이면 x좌표 왼쪽으로 땡겨줌
	if(!(dwOptDraw & REBAR_APS_DETAIL_RB) || pOpt->m_nTypeRebarTable == 1)
	{
		rcDetailRB.left		= rcPlane.left + Dom.Always(250);
		rcDetailRB.bottom	= rcPlane.bottom;
		rcDetailRB.right	= rcDetailRB.left;
		rcDetailRB.top		= rcDetailRB.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////


	// 상세도 A /////////////////////////////////////////////////////////////////////////
	CDRect rcDetailA(0, 0, 0, 0);
	pPageMng->SetProgressData(_T("다웰바 상세도 출력중..."), (int)(dCurPer+=dAddPer));
	if(dwOptDraw & REBAR_APS_DETAIL_A)
	{
		CDrawApproachSlab drawAps;
		drawAps.m_pARoadOptionStd	= pStd->m_pARoadOptionStd;

		// 교량 브라켓 부분을 컷팅해서 그려줌
		CTwinVectorArray tvArrTot;
		CDRect rect;
		if(bStt)
		{
			pBri->m_tvArrVertSection.GetTvArrByInfoRange("시점브라켓헌치", "시점브라켓상부측면", tvArrTot);
			rect = tvArrTot.GetRect();
			rect.left -= 50;
			rect.top += 1000;
			rect.bottom -= 1000;
			rect.right += 2000;
		}
		else
		{
			pBri->m_tvArrVertSection.GetTvArrByInfoRange("종점브라켓상부측면", "종점브라켓헌치", tvArrTot);
			rect = tvArrTot.GetRect();
			rect.left -= 2000;
			rect.top += 1000;
			rect.bottom -= 1000;
			rect.right += 50;
		}

		tvArrTot = pBri->m_tvArrVertSection;
		pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
		DBStd.DrawCuttingTvArray(&Dom, tvArrTot, rect);

		// 접속슬래브 상세 그림.
		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABDETAILA))
		{
			drawAps.DrawDetailA(&Dom, pAps, TRUE);
			Dom.SetCalcExtRect();
			rcDetailA	= Dom.GetExtRect();
			xyMove	= CDPoint(rcDetailRB.right-rcDetailA.left+Dom.Always(50), rcDetailRB.bottom - rcDetailA.bottom - Dom.Always(20)); 
			Dom.Move(xyMove);
			rcDetailA.left		+= xyMove.x; rcDetailA.right	+= xyMove.x;
			rcDetailA.top		+= xyMove.y; rcDetailA.bottom	+= xyMove.y;

			Dom.Move(pMove->GetPoint(MPE_REBAR_SLABDETAILA));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABDETAILA, _T("다웰바 상세도"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABDETAILA_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("다웰바 상세도"), CDPoint(0,0), 0, dScale);
			Dom.Move(rcDetailA.left+rcDetailA.Width()/2.0, rcDetailA.bottom+Dom.Always(30));
			Dom.Move(pMove->GetPoint(MPE_REBAR_SLABDETAILA_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABDETAILA_TITLE, _T("다웰바 상세도 타이틀"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcDetailA.left		= rcDetailRB.right + Dom.Always(50);
		rcDetailA.bottom	= rcDetailRB.bottom;
		rcDetailA.right	=	 rcDetailA.left;
		rcDetailA.top		= rcDetailA.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////

	// 재료표   ///////////////////////////////////////////////////////////////////////
	CDRect rcTableM(0, 0, 0, 0);
	pPageMng->SetProgressData(_T("재료표 출력중..."), (int)(dCurPer+=dAddPer));
	if(dwOptDraw & REBAR_APS_TABLE_M)
	{
		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABTABLEM))
		{
			DBStd.m_pDrawAps->DrawMaterialTable(&Dom, pAps, TRUE, pBri->m_pARcBridgeDataStd->m_nTypeUnit);
			Dom.SetCalcExtRect();
			rcTableM	= Dom.GetExtRect();
			xyMove	= CDPoint(rcDetailA.left-rcTableM.left, rcDetailA.top-rcTableM.bottom-Dom.Always(50)); 
			Dom.Move(xyMove);
			rcTableM.left		+= xyMove.x; rcTableM.right	+= xyMove.x;
			rcTableM.top		+= xyMove.y; rcTableM.bottom	+= xyMove.y;

			Dom.Move(pMove->GetPoint(MPE_REBAR_SLABTABLEM));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABTABLEM, _T("재료표"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABTABLEM_TITLE))
		{
			pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
			Dom.TextOut(0, 0, "재료표");
			Dom.Move(rcTableM.left+rcTableM.Width()/2.0, rcTableM.bottom);
			Dom.Move(pMove->GetPoint(MPE_REBAR_SLABTABLEM_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABTABLEM_TITLE, _T("재료표 타이틀"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcTableM.left		= rcDetailA.left;
		rcTableM.bottom		= rcDetailA.top - Dom.Always(50);
		rcTableM.right		= rcTableM.left;
		rcTableM.top		= rcTableM.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////

	// 철근 재료표   ///////////////////////////////////////////////////////////////////////
	CDPoint xyBase	= CDPoint(rcTableM.left, rcTableM.top - Dom.Always(100));
	//도면간소화타입 이면 재료표 위로 올려서 출력		
	//위쪽에 뭐 그려져있으면 옆으로 옮기자 
	if((dwOptDraw & REBAR_APS_DETAIL_A) || (dwOptDraw & REBAR_APS_TABLE_M))
		xyBase.x += Dom.Always(180);
	xyBase.y = rcDogak.bottom - Dom.Always(120);

	//도면 간소화(다웰바 철근을 따로 분리하지 않는다)
	if(pOpt->m_nTypeRebarTable == 1)
	{
		CDRect rcTableRB(0, 0, 0, 0);
		CTypedPtrArray <CObArray, CRebar*> arrRebarGet;


		if(pData->m_bRebarTableSepByFyAps)//강종별(도면간소화)
		{
			double dFy	= 1;
			while(dFy != 0)
			{
				dFy	= pStd->GetRebarListByFy(&arrRebar, &arrRebarGet, dFy);
				if(dFy == 0) break;
				if(arrRebarGet.GetSize() == 0) break;

				baseConc.SetRebarIndexBySortRebarList(&arrRebarGet, TRUE);//테이블에 번호 세팅


				if(dwOptDraw & REBAR_APS_TABLE_RB)
				{
					pPageMng->SetProgressData(_T("철근재료표 출력중..."), (int)(dCurPer+=dAddPer));
					baseConc.ShellSortRebarList(&arrRebarGet, &CRebar::CompareForTable);
					xyBase	= DrawRebarTable(&Dom, pAps, &arrRebarGet, "철근재료표", xyBase);

					*pDomP << Dom;
				}

				if(dwOptDraw & REBAR_APS_DETAIL_RB)
				{
					pPageMng->SetProgressData("철근상세 출력중...", 80);
					baseConc.ShellSortRebarList(&arrRebarGet, &CRebar::CompareForList);
					baseDraw.DrawRebarListDetailSimple(&Dom, &arrRebarGet, xyBase, pOpt->m_dWidthRebarTableRect, pOpt->m_dHeightRebarTableRect);

					Dom.SetCalcExtRect();
					Dom.Move(pMove->GetPoint(MPE_REBAR_SLABDETAIL));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABDETAIL, hggettext("철근상세 제목"));

					CDRect rect = Dom.GetExtRect();
					xyBase.x += fabs(rect.Width()+ Dom.Always(30));

					*pDomP << Dom;
				}
			}
		}
		else	//일반(도면 간소화)
		{
			baseConc.SetRebarIndexBySortRebarList(&arrRebar, TRUE);//테이블에 번호 세팅

			if(dwOptDraw & REBAR_APS_TABLE_RB)
			{
				pPageMng->SetProgressData(_T("철근재료표 출력중..."), (int)(dCurPer+=dAddPer));
				baseConc.ShellSortRebarList(&arrRebar, &CRebar::CompareForTable);
				xyBase	= DrawRebarTable(&Dom, pAps, &arrRebar, "철근재료표", xyBase);

				*pDomP << Dom;
			}

			if(dwOptDraw & REBAR_APS_DETAIL_RB)
			{
				pPageMng->SetProgressData("철근상세 출력중...", 80);
				baseConc.ShellSortRebarList(&arrRebar, &CRebar::CompareForList);
				baseDraw.DrawRebarListDetailSimple(&Dom, &arrRebar, xyBase, pOpt->m_dWidthRebarTableRect, pOpt->m_dHeightRebarTableRect);

				Dom.SetCalcExtRect();
				Dom.Move(pMove->GetPoint(MPE_REBAR_SLABDETAIL));
				pStd->ChangeSeperateLayer(&Dom);
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABDETAIL, hggettext("철근상세 제목"));

				*pDomP << Dom;

			}
		}		

		Dom.SetCalcExtRect();
		rcDetailRB	= Dom.GetExtRect();

		*pDomP << Dom;

		pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE);
		////////////////////////////////////////////////////////////////////////////////////



		AhTPADelete(&arrRebar,	(CRebar*)0);
		AhTPADelete(&arrRebarGet, (CRebar*)0);

		if(dwOptDraw & REBAR_APS_TABLE_RB)
		{
			DrawRebarDetailTable_RawRebar(pDomP, pAps, left, xyBase);
		}
	}
	else
	{
		CTypedPtrArray <CObArray, CRebar*> arrRebarDowel;
		CDRect rcTableRB(0, 0, 0, 0);
		
		long nCntTable = 1;
		if(pBri->m_bApsDowelSeparateRebar)
		{
			pStd->m_pARcBridgeDataStd->SeparateRebarListByDescription(&arrRebar, &arrRebarDowel, _T("다웰바"));
			nCntTable++;
		}
		
		long nTableIdx = 0;
		CString sTitle = _T("");
		for (long nTable =0; nTable < nCntTable; nTable++)
		{
			CTypedPtrArray <CObArray, CRebar*> *pArrRebarOrg = (nTable == 0) ? &arrRebar : &arrRebarDowel;
			if(pArrRebarOrg->GetSize() == 0)	continue;

			sTitle = (nTable == 1) ? _T("다웰바") : _T("");

			if(pData->m_bRebarTableSepByFyAps)//강종별
			{
				double dFy	= 1;
				while(dFy != 0)
				{
					CTypedPtrArray <CObArray, CRebar*> arrRebarGet;
					dFy	= pStd->GetRebarListByFy(pArrRebarOrg, &arrRebarGet, dFy);
					if(dFy == 0) break;
					if(arrRebarGet.GetSize() == 0) break;

					baseConc.ShellSortRebarList(&arrRebarGet, &CRebar::CompareForTable);
					xyBase	= DrawRebarTable(&Dom, pAps, &arrRebarGet, sTitle, xyBase, nTableIdx++);	
				}
			}
			else//일반
			{
				baseConc.ShellSortRebarList(pArrRebarOrg, &CRebar::CompareForTable);
				xyBase	= DrawRebarTable(&Dom, pAps, pArrRebarOrg, sTitle, xyBase, nTableIdx++);
			}	
		}
		Dom.SetCalcExtRect();
		rcDetailRB	= Dom.GetExtRect();

		*pDomP << Dom;

		pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE);
		////////////////////////////////////////////////////////////////////////////////////
		
		AhTPADelete(&arrRebar,	(CRebar*)0);	

		DrawRebarDetailTable_RawRebar(pDomP, pAps, left, xyBase);
	}	

}

// 현재 그려야 되는 RcBridge리턴
CRcBridgeRebar* CDrawingAps::GetRcBridgeRebar()
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

void CDrawingAps::DrawRebarDetail(CDomyun *pDomP, CApsApp *pAps, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, CDPoint xyBase)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);
	CHgBaseDrawStd HGDraw;
	CConcSymbol symDB(pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);

	double dSpace = pData->m_dSpaceBetweenRebarDetailAps;
	double dMaxY  = Dom.Always(525);
	double dMinY  = Dom.Always(60);
	double dYPlus = 0;
	CDPoint xy	= CDPoint(xyBase.x, dMaxY);
	CRebar *pRB;

	if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABDETAIL_TITLE))
	{
		pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
		Dom.TextOut(xy.x+Dom.Always(50),xy.y+Dom.GetTextHeight()/2,"철 근 상 세");
		Dom.Move(pMove->GetPoint(MPE_REBAR_SLABDETAIL_TITLE));
		pStd->ChangeSeperateLayer(&Dom);
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABDETAIL_TITLE, "철근상세 제목");
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
				if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABDETAIL+nDetailCol))
				{
					Dom.Move(pMove->GetPoint(MPE_REBAR_SLABDETAIL+nDetailCol));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABDETAIL+nDetailCol, "철근상세 "+COMMA(1+nDetailCol));
					*pDomP << Dom;
				}
				nDetailCol++;
				xy.x += Dom.Always(120);
				xy.y  = dMaxY;
				nRow  = 3;
			}

			CDPoint xyDrawPoint = CDPoint(xy.x+Dom.Always(50),xy.y+Dom.Always(10)+dYPlus);
			nRow = HGDraw.GetPlusCntRowDrawSymbolHeight(&Dom,pStd->m_pARoadOptionStd, nRow, nType, dSpace,  i, bJoint, xyDrawPoint);

			xy.y   = dMaxY - dSpace*nRow;
			dYPlus = (nType==1) ? Dom.Always(8) : 0;
			symDB.RebarDetailBasic(&Dom,nType,CDPoint(xy.x+Dom.Always(50),xy.y+Dom.Always(10)+dYPlus),bJoint);
		}
		else
			xy.y = dMaxY - dSpace*nRow;
		if(xy.y < dMinY)
		{
			if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABDETAIL+nDetailCol))
			{
				Dom.Move(pMove->GetPoint(MPE_REBAR_SLABDETAIL+nDetailCol*10));
				pStd->ChangeSeperateLayer(&Dom);
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABDETAIL+nDetailCol, "철근상세 "+COMMA(1+nDetailCol));
				*pDomP << Dom;
			}
			nDetailCol++;
			xy.x += Dom.Always(120);
			xy.y  = dMaxY;
			nRow  = 0;	
		}
		symDB.DrawJewonBar(&Dom,pRB,xy,pStd->m_pARcBridgeDataStd->m_pBasicConcInfo->m_nRoundRebarLength);
		nRow++;
	}

	if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABDETAIL+nDetailCol))
	{
		Dom.Move(pMove->GetPoint(MPE_REBAR_SLABDETAIL+nDetailCol));
		pStd->ChangeSeperateLayer(&Dom);
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABDETAIL+nDetailCol, "철근상세 "+COMMA(1+nDetailCol));
		*pDomP << Dom;
	}
}

CDPoint CDrawingAps::DrawRebarTable(CDomyun *pDomP, CApsApp *pAps, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, CString sSubTitle, CDPoint xyBase, long nTableIdx)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);
	
	CGridDomyunEx Dom(pDomP);
	
	long nTableType = pOpt->m_nTypeRebarTable ;

	if(nTableType == 0) //기본타입일때만 제목 출력
	{
		long nDefTableTitle	= MPE_REBAR_SLABTABLE_TITLE+ nTableIdx;

		pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
		CString sTitle = "철 근 재 료 표";
		if(sSubTitle != _T(""))	sTitle += _T("(") + sSubTitle + _T(")");
		Dom.TextOut(xyBase.x+Dom.Always(155/2),xyBase.y+Dom.Always(10), sTitle);
		Dom.Move(pMove->GetPoint(nDefTableTitle));
		pStd->ChangeSeperateLayer(&Dom);
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom, Dom.GetExtRect(), nDefTableTitle, "철근재료표 제목");
		*pDomP << Dom;
	}

	double dLengthSum		= 0;
	double dLengthSumAdd	= 0;
	double dLengthTotal		= 0;
	long nMaxRow			= pData->m_nCountRowEachTable;
	long nDefTable			= MPE_REBAR_SLABTABLE+ nTableIdx;
	double dWeightTotal		= 0;
	double dWeightTotalAdd	= 0;

	CBasicConcInfo *pBCI = pStd->m_pARcBridgeDataStd->m_pBasicConcInfo;
	CHgBaseDrawStd baseDraw;
	double Fy = pData->m_bRebarTableSepByFy && pArrRebar->GetSize() > 0 ? pArrRebar->GetAt(0)->m_Fy : pAps->m_dFy;

	// 철근재료표 그릴때 단위계 적용
	if(pBri->m_pARcBridgeDataStd->m_nTypeUnit == 1)
		Fy = Fy * 10;

	BOOL bShadeOutLone = TRUE;//(pOpt->m_dDrawGen & GEBDRAW_GEN_TABLESHADE) ? TRUE : FALSE;

	CDRect rtTable(0);
	long nIndex = 0;
	long nTableSu = 0;
	while(1)
	{
		if(nIndex >= pArrRebar->GetSize()) break;
		if(m_pStd->IsDrawByBlock(nDefTable+nTableSu))
		{
			if(nTableType == 0) //기본타입
				nIndex = baseDraw.DrawRebarTable(&Dom, nIndex, nMaxRow, xyBase, dLengthSum, dLengthSumAdd, dLengthTotal, dWeightTotal, dWeightTotalAdd, *pArrRebar, Fy, bShadeOutLone, "", pBCI, pBCI->m_nRoundRebarLength);
			else //간소화타입
				nIndex = baseDraw.DrawRebarTableSimple(&Dom, nIndex, nMaxRow, xyBase, dLengthSum, dLengthSumAdd, dLengthTotal, dWeightTotal, dWeightTotalAdd, *pArrRebar, Fy, bShadeOutLone, "", pBCI, pBCI->m_nRoundRebarLength);
			
			Dom.SetCalcExtRect();
			rtTable = Dom.GetExtRect();
			Dom.Move(pMove->GetPoint(nDefTable+nTableSu));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), nDefTable+nTableSu, "철근재료표");
			*pDomP << Dom;
		}
		xyBase.x += rtTable.Width() + Dom.Always(10);
		nTableSu++;
	}		

	return xyBase;
}

void CDrawingAps::MakeRebarList(CApsApp *pAps, CTypedPtrArray <CObArray, CRebar*> *pArrRebar)
{
	AhTPADelete(pArrRebar,	(CRebar*)0);
	long nSize	= pAps->m_pArrRebar.GetSize();

	long i = 0; for(i = 0; i < nSize; i++)
	{
		CRebar *pRB	= pAps->m_pArrRebar.GetAt(i);
		if(pRB->IsTrue())
		{
			CRebar *pRBTmp = new CRebar;
			*pRBTmp	= *pRB;
			pArrRebar->Add(pRBTmp);
		}
	}
}

void CDrawingAps::DrawRebarDetailTable_RawRebar( CDomyun *pDom, CApsApp *pAps, long nLeft, CDPoint &xyBase )
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);

	if(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->IsApplyExtraChargeAtRebarJoint() == FALSE && pOpt->m_bAddRawRebarTable)
	{
		CTypedPtrArray <CObArray, CRebar*> arrRebar;
		CTypedPtrArray <CObArray, CRebar*> arrRawRebar;

		CHgBaseConcStd baseConc;
		long nIdxTable = 3;

		MakeRebarList(pAps, &arrRebar);

		long nCountRaw = baseConc.GetRawRebarList(&arrRebar, &arrRawRebar, pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->GetLenghtRebarMax());
		if(nCountRaw > 0)
		{
			if(pData->m_bRebarTableSepByFyAps)
			{
				double dFy	= 1;
				while(dFy != 0)
				{
					CTypedPtrArray <CObArray, CRebar*> arrRebarGet;
					dFy	= pStd->GetRebarListByFy(&arrRawRebar, &arrRebarGet, dFy);
					if(dFy == 0) break;
					if(arrRebarGet.GetSize() == 0) break;

					baseConc.ShellSortRebarList(&arrRebarGet, &CRebar::CompareForTable);
					xyBase = DrawRebarTable(pDom, pAps, &arrRebarGet, _T("미가공 철근"), xyBase, ++nIdxTable);
				}
			}
			else
			{
				baseConc.ShellSortRebarList(&arrRawRebar, &CRebar::CompareForTable);
				xyBase = DrawRebarTable(pDom, pAps, &arrRawRebar, _T("미가공 철근"), xyBase, ++nIdxTable);
			}
		}

		AhTPADelete(&arrRawRebar, (CRebar*)0);
		AhTPADelete(&arrRebar,	(CRebar*)0);
	}
}
