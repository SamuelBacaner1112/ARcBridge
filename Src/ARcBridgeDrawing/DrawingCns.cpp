// DrawingCns.cpp: implementation of the CDrawingCns class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"
#include "../../Src/ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "../ARcBridgeDrawingOption/ARcBridgeDrawingOption.h"
#include "ARcBridgeDrawing.h"
#include "DrawingCns.h"

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

CDrawingCns::CDrawingCns()
{
	m_nBridge = -1;
}

CDrawingCns::~CDrawingCns()
{

}

long CDrawingCns::GetPageSu()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;

	if(pStd->GetCountTotalPage()<=0)
		pStd->MakePageInfo();

	m_nPage	= pStd->GetCountCategoryPage(_T("완충 슬래브"));

	return m_nPage;
}

CString CDrawingCns::GetTitle()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("완충 슬래브"), pStd->m_nCurPage-m_nSttPage);
	if(pInfo==NULL) return _T("");

	CString strTitle = pStd->GetStringTitle(pStd->m_pARcBridgeDataStd->GetBridge(m_nBridge), pInfo->m_strTitle);

	return strTitle;
}

double CDrawingCns::GetScale()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pOptData = pOpt->GetDrawingOption(m_nBridge);

	m_Scale	= pOptData->m_dScaleRebarAps;
	return pOptData->m_dScaleRebarAps;
}

void CDrawingCns::SetNumber(long nSubPage)
{
	m_nBridge = GetNumberBridge(nSubPage-m_nSttPage);
}

long CDrawingCns::GetNumberBridge(long nCategoryPageNum)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("완충 슬래브"), nCategoryPageNum);
	if(pInfo==NULL) return -1;

	return pInfo->m_nBri;
}

void CDrawingCns::DrawOnePage()
{
	TRACE("CDrawingCns");
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

	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("완충 슬래브"), pStd->m_nDrawPage-m_nSttPage);
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

	// 접속 슬래브 출력
	long nPageNum	= pInfo->m_nPageNumber-1;	// 인덱스
	BOOL bMerge12	= pOptData->m_nTypeCnsRebar == 0 ? TRUE : FALSE;	// 완충슬래브 1,2 동시표현 여부

	BOOL bDraw		= FALSE;

	long nCountCnsSttLeft	= pBri->IsCns(TRUE, TRUE, 0) ? 1 : 0;
	long nCountCnsSttLeft1	= pBri->IsCns(TRUE, TRUE, 1) ? 1 : 0;
	long nCountCnsSttRight	= pBri->IsCns(TRUE, FALSE, 0) ? 1 : 0;
	long nCountCnsSttRight1	= pBri->IsCns(TRUE, FALSE, 1) ? 1 : 0;
	long nCountCnsEndLeft	= pBri->IsCns(FALSE, TRUE, 0) ? 1 : 0;
	long nCountCnsEndLeft1	= pBri->IsCns(FALSE, TRUE, 1) ? 1 : 0;
	long nCountCnsEndRight	= pBri->IsCns(FALSE, FALSE, 0) ? 1 : 0;
	long nCountCnsEndRight1	= pBri->IsCns(FALSE, FALSE, 1) ? 1 : 0;

	if(bMerge12)
	{
		nCountCnsSttLeft1	= 0;
		nCountCnsSttRight1	= 0;
		nCountCnsEndLeft1	= 0;
		nCountCnsEndRight1	= 0;
	}
	
	// 시점, 좌측 완충슬래브1
	if(nPageNum < nCountCnsSttLeft && !bDraw)
	{
		AddPageCns(&Dom, TRUE, TRUE, bMerge12 ? -1 : 0, rcDogak);
		bDraw	= TRUE;
	}
	else
		nPageNum -= nCountCnsSttLeft;

	// 시점, 좌측 완충슬래브2
	if(nPageNum < nCountCnsSttLeft1 && !bDraw)
	{
		AddPageCns(&Dom, TRUE, TRUE, bMerge12 ? -1 : 1, rcDogak);
		bDraw	= TRUE;
	}
	else
		nPageNum -= nCountCnsSttLeft1;

	// 시점, 우측 완충슬래브1
	if(nPageNum < nCountCnsSttRight && !bDraw)
	{
		AddPageCns(&Dom, TRUE, FALSE, bMerge12 ? -1 : 0, rcDogak);
		bDraw	= TRUE;
	}
	else
		nPageNum -= nCountCnsSttRight;

	// 시점, 우측 완충슬래브2
	if(nPageNum < nCountCnsSttRight1 && !bDraw)
	{
		AddPageCns(&Dom, TRUE, FALSE, bMerge12 ? -1 : 1, rcDogak);
		bDraw	= TRUE;
	}
	else
		nPageNum -= nCountCnsSttRight1;

	// 종점, 좌측 완충슬래브1
	if(nPageNum < nCountCnsEndLeft && !bDraw)
	{
		AddPageCns(&Dom, FALSE, TRUE, bMerge12 ? -1 : 0, rcDogak);
		bDraw	= TRUE;
	}
	else
		nPageNum -= nCountCnsEndLeft;

	// 종점, 좌측 완충슬래브2
	if(nPageNum < nCountCnsEndLeft1 && !bDraw)
	{
		AddPageCns(&Dom, FALSE, TRUE, bMerge12 ? -1 : 1, rcDogak);
		bDraw	= TRUE;
	}
	else
		nPageNum -= nCountCnsEndLeft1;

	// 종점, 우측 완충슬래브1
	if(nPageNum < nCountCnsEndRight && !bDraw)
	{
		AddPageCns(&Dom, FALSE, FALSE, bMerge12 ? -1 : 0, rcDogak);
		bDraw	= TRUE;
	}
	else
		nPageNum -= nCountCnsEndRight;

	// 종점, 우측 완충슬래브2
	if(nPageNum < nCountCnsEndRight1 && !bDraw)
	{
		AddPageCns(&Dom, FALSE, FALSE, bMerge12 ? -1 : 1, rcDogak);
// 		bDraw	= TRUE;
	}
// 	else
// 		nPageNum -= nCountCnsEndRight1;


	pMng->SetProgressData(_T("출력 완료  ..."), 100);

	*pDom << Dom;
}

void CDrawingCns::AddPageOne(CDomyun* pDomP, long nSubPage)
{

}

// 접속슬래브 도면 추가
// nIdx 가 -1이면 완충슬래브 1, 2를 함께 그림.
void CDrawingCns::AddPageCns(CDomyun *pDomP, BOOL bStt, BOOL bLeft, long nIdx, CDRect rcDogak)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);

	DWORD dwOptDraw = pData->m_dwDrawRebarCns;

	long nIdxCns	= nIdx == -1 ? 0 : nIdx;
	long nIdxCns1	= nIdx == -1 ? 1 : nIdx;
	long left		= bLeft ? iLEFT : iRIGHT;
	CApsApp *pCns	= bStt ? &pBri->m_cnsStt[left][nIdxCns] : &pBri->m_cnsEnd[left][nIdxCns];
	CApsApp *pCns1	= bStt ? &pBri->m_cnsStt[left][nIdxCns1] : &pBri->m_cnsEnd[left][nIdxCns1];
	if(!pCns->m_bIs) return;
	
	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	double dScale	= GetScale();

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScale);
	
	BOOL bDrawCns2	= nIdx == -1 && nIdxCns != nIdxCns1;	// 완충슬래브를 2개 한꺼번에 그릴지?
	if(!pCns1->m_bIs) bDrawCns2	= FALSE;					// 완충슬래브가 하나만 있으면 무조건 한개만 그림.(당연한 말씀)
	CString str		= _T("");
	CString sCns	= bStt ? "시점측 접속슬래브" : "종점측 접속슬래브";
	sCns			+= bLeft ? "(좌측)" : "(우측)";
	CString sNum	= nIdx == -1 ? "1, 2" : "";
	if(nIdx != -1) sNum.Format("%d", nIdx+1);
	sCns			+= sNum;


	long nCountDom	= 3;
	if(nIdx == -1 && nIdxCns != nIdxCns1)
		nCountDom++;
	double dAddPer	= 100 / nCountDom;
	double dCurPer	= 0;

	CDPoint xyMove(0, 0);
	// 평면도 //////////////////////////////////////////////////////////////////////
	CDRect rcPlane(0, 0, 0, 0);	// 평면도 영역
	pPageMng->SetProgressData(_T("평면도 출력중..."), (int)(dCurPer+=dAddPer));
	if(dwOptDraw & REBAR_CNS_PLANE)
	{
		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABPLANE))
		{
			rcPlane	= DBStd.m_pDrawAps->DrawRebar_PlaneStd(&Dom, pCns, FALSE);
			if(bDrawCns2)
			{
				CDomyun domCns1(&Dom);
				CDRect rcPlane1	= DBStd.m_pDrawAps->DrawRebar_PlaneStd(&domCns1, pCns1, FALSE);

				CDPoint xyMove1(0, 0);
				if(bStt)
					xyMove	= CDPoint(rcPlane.left - rcPlane1.right - Dom.Always(100), rcPlane.top - rcPlane1.top);
				else
					xyMove	= CDPoint(rcPlane.right - rcPlane1.left + Dom.Always(100), rcPlane.top - rcPlane1.top);

				domCns1.Move(xyMove);
				rcPlane1.left	+= xyMove.x; rcPlane1.right		+= xyMove.x;
				rcPlane1.top	+= xyMove.y; rcPlane1.bottom	+= xyMove.y;
				rcPlane			= DBStd.MergeRectAndRect(rcPlane, rcPlane1);

				Dom << domCns1;
			}
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
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABPLANE_TITLE, sCns+_T("평면도 타이틀"));
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
	if(dwOptDraw & REBAR_CNS_FRONT)
	{
		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABFRONT))
		{
			rcFront	= DBStd.m_pDrawAps->DrawRebar_FrontStd(&Dom, pCns, FALSE);
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
	if(dwOptDraw & REBAR_CNS_SIDE)
	{
		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABSIDE))
		{
			pPageMng->SetProgressData(_T("단면 B-B 출력중..."), (int)(dCurPer+=dAddPer));
			rcSide	= DBStd.m_pDrawAps->DrawRebar_SideStd(&Dom, pCns, FALSE);

			if(bDrawCns2)
			{
				CDomyun domCns1(&Dom);
				CDRect rcSide1	= DBStd.m_pDrawAps->DrawRebar_SideStd(&domCns1, pCns1, FALSE);

				CDPoint xyMove1(0, 0);
				if(bStt)
					xyMove	= CDPoint(rcSide.left - rcSide1.right - Dom.Always(100), rcSide.top - rcSide1.top);
				else
					xyMove	= CDPoint(rcSide.right - rcSide1.left + Dom.Always(100), rcSide.top - rcSide1.top);

				domCns1.Move(xyMove);
				rcSide1.left	+= xyMove.x; rcSide1.right	+= xyMove.x;
				rcSide1.top		+= xyMove.y; rcSide1.bottom	+= xyMove.y;
				rcSide			= DBStd.MergeRectAndRect(rcSide, rcSide1);

				Dom << domCns1;
			}

			xyMove	= CDPoint(rcPlane.left-rcSide.left, rcPlane.top-rcSide.bottom-Dom.Always(100)); 
			Dom.Move(xyMove);
			rcSide.left		+= xyMove.x; rcSide.right	+= xyMove.x;
			rcSide.top		+= xyMove.y; rcSide.bottom	+= xyMove.y;

			Dom.Move(pMove->GetPoint(MPE_REBAR_SLABSIDE));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABSIDE, _T("단면 B-B "));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABSIDE_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("단면 B-B "), CDPoint(0,0), 0, dScale);
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
	CHgBaseDrawStd baseDraw;
	CDRect rcDetailRB(0, 0, 0, 0);
	pPageMng->SetProgressData(_T("철근상세 출력중..."), (int)(dCurPer+=dAddPer));
	CTypedPtrArray <CObArray, CRebar*> arrRebar;	// 재료표용 철근리스트
	MakeRebarList(pCns, &arrRebar);
	if(bDrawCns2)
		MakeRebarList(pCns1, &arrRebar);

	baseConc.ShellSortRebarList(&arrRebar, &CRebar::CompareForList);

	if(pOpt->m_nTypeRebarTable == 0) //일반타입일 경우 철근 상세
	{
		if(dwOptDraw & REBAR_CNS_DETAIL_RB)
		{
			DrawRebarDetail(&Dom, pCns, &arrRebar, CDPoint(rcPlane.right + Dom.Always(50), rcPlane.bottom));
			Dom.SetCalcExtRect();
			rcDetailRB	= Dom.GetExtRect();
			*pDomP << Dom;
		}
	}

	// 기본타입용 철근상세가 출력되지 않거나, 도면간소화타입이면 x좌표 왼쪽으로 땡겨줌
	if((pOpt->m_nTypeRebarTable == 0 &&!(dwOptDraw & REBAR_CNS_DETAIL_RB))
		|| pOpt->m_nTypeRebarTable == 1)
	{
		rcDetailRB.left		= rcPlane.left + Dom.Always(100);
		rcDetailRB.bottom	= rcPlane.bottom;
		rcDetailRB.right	= rcDetailRB.left;
		rcDetailRB.top		= rcDetailRB.bottom;
	}

	////////////////////////////////////////////////////////////////////////////////////


	// 재료표   ///////////////////////////////////////////////////////////////////////
	CDRect rcTableM(0, 0, 0, 0);
	pPageMng->SetProgressData(_T("재료표 출력중..."), (int)(dCurPer+=dAddPer));
	if(dwOptDraw & REBAR_CNS_TABLE_M)
	{
		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABTABLEM))
		{
			DBStd.m_pDrawAps->DrawMaterialTable(&Dom, pCns, TRUE, pBri->m_pARcBridgeDataStd->m_nTypeUnit);
			Dom.SetCalcExtRect();
			rcTableM	= Dom.GetExtRect();
			xyMove	= CDPoint(rcDetailRB.right-rcTableM.left+Dom.Always(50), rcDetailRB.bottom - rcTableM.bottom - Dom.Always(20)); 
			Dom.Move(xyMove);
			rcTableM.left		+= xyMove.x; rcTableM.right	+= xyMove.x;
			rcTableM.top		+= xyMove.y; rcTableM.bottom	+= xyMove.y;

			Dom.Move(pMove->GetPoint(MPE_REBAR_SLABTABLEM));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
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
		rcTableM.left		= rcDetailRB.right + Dom.Always(50);
		rcTableM.bottom		= rcDetailRB.bottom;
		rcTableM.right		= rcTableM.left;
		rcTableM.top		= rcTableM.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////

	// 철근 재료표   ///////////////////////////////////////////////////////////////////////
	CDRect rcTableRB(0, 0, 0, 0);
	CDPoint xyBase(0);
	baseConc.SetRebarIndexBySortRebarList(&arrRebar, TRUE);  //이형철근상세 인덱스보고 테이블에 번호 세팅

	if(dwOptDraw & REBAR_CNS_TABLE_RB)
	{
		pPageMng->SetProgressData(_T("철근재료표 출력중..."), (int)(dCurPer+=dAddPer));
		xyBase = DrawRebarTable(&Dom, pCns, &arrRebar, "철근재료표", CDPoint(rcTableM.left, rcTableM.top - Dom.Always(100)));
		Dom.SetCalcExtRect();
		rcDetailRB	= Dom.GetExtRect();

		*pDomP << Dom;
	}
	////////////////////////////////////////////////////////////////////////////////////

	if(dwOptDraw & REBAR_CNS_DETAIL_RB) //도면간소화용 철근상세
	{
		if(pOpt->m_nTypeRebarTable == 1) //도면간소화타입
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

	AhTPADelete(&arrRebar,	(CRebar*)0);

	if(dwOptDraw & REBAR_CNS_TABLE_RB)
	{
		DrawRebarDetailTable_RawRebar(pDomP, pCns, 0, xyBase);
	}
}

// 현재 그려야 되는 RcBridge리턴
CRcBridgeRebar* CDrawingCns::GetRcBridgeRebar()
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

void CDrawingCns::DrawRebarDetail(CDomyun *pDomP, CApsApp *pAps, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, CDPoint xyBase)
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
			nRow = HGDraw.GetPlusCntRowDrawSymbolHeight(&Dom, pStd->m_pARoadOptionStd, nRow, nType, dSpace,  i, bJoint, xyDrawPoint);


			xy.y   = dMaxY - dSpace*nRow;
			dYPlus = (nType==1) ? Dom.Always(8) : 0;

			symDB.RebarDetailBasic(&Dom,nType,CDPoint(xy.x+Dom.Always(50),xy.y+Dom.Always(10)+dYPlus),bJoint);
		}
		else
			xy.y = dMaxY - dSpace*nRow;
		if(xy.y < dMinY)
		{
			if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABDETAIL+nDetailCol*10))
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

CDPoint CDrawingCns::DrawRebarTable(CDomyun *pDomP, CApsApp *pAps, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, CString sSubTitle, CDPoint xyBase)
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
		long nDefTableTitle	= MPE_REBAR_SLABTABLE_TITLE;

		if(m_pStd->IsDrawByBlock(nDefTableTitle))
		{
			pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
			CString sTitle = "철 근 재 료 표";
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
	long nDefTable			= MPE_REBAR_SLABTABLE;
	double dWeightTotal		= 0;
	double dWeightTotalAdd	= 0;

	CBasicConcInfo *pBCI = pStd->m_pARcBridgeDataStd->m_pBasicConcInfo;
	CHgBaseDrawStd baseDraw;
	double Fy = pBri->m_pARcBridgeDataStd->m_Fy;
	
	// 철근재료표 그릴때 단위계 적용
	if(pBri->m_pARcBridgeDataStd->m_nTypeUnit == 1)
		Fy = Fy * 10;

	BOOL bShadeOutLone = TRUE;//(pOpt->m_dDrawGen & GEBDRAW_GEN_TABLESHADE) ? TRUE : FALSE;

	CDRect rtTable;
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

	return xyBase;}

void CDrawingCns::MakeRebarList(CApsApp *pAps, CTypedPtrArray <CObArray, CRebar*> *pArrRebar)
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

void CDrawingCns::DrawRebarDetailTable_RawRebar( CDomyun *pDom, CApsApp *pCns, long nIdxTable, CDPoint &xyBase )
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

		MakeRebarList(pCns, &arrRebar);
		
		long nCountRaw = baseConc.GetRawRebarList(&arrRebar, &arrRawRebar, pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->GetLenghtRebarMax());
		if(nCountRaw > 0)
		{
			if(FALSE)
			{
				double dFy	= 1;
				while(dFy != 0)
				{
					CTypedPtrArray <CObArray, CRebar*> arrRebarGet;
					dFy	= pStd->GetRebarListByFy(&arrRawRebar, &arrRebarGet, dFy);
					if(dFy == 0) break;
					if(arrRebarGet.GetSize() == 0) break;

					baseConc.ShellSortRebarList(&arrRebarGet, &CRebar::CompareForTable);
					xyBase = DrawRebarTable(pDom, pCns, &arrRebar, _T("미가공 철근"), xyBase);
				}
			}
			else
			{
				baseConc.ShellSortRebarList(&arrRawRebar, &CRebar::CompareForTable);
				xyBase = DrawRebarTable(pDom, pCns, &arrRebar, _T("미가공 철근"), xyBase);
			}
		}

		AhTPADelete(&arrRawRebar, (CRebar*)0);
	}
}
