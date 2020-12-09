// DrawingVerticalPlane.cpp: implementation of the CDrawingVerticalPlane class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"
#include "../../Src/ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "../ARcBridgeDrawingOption/ARcBridgeDrawingOption.h"
#include "ARcBridgeDrawing.h"
#include "DrawingVerticalPlane.h"

#define MPE_PLANE		10
#define MPE_CROSS		20
#define MPE_LONGI		30
#define MPE_CANT		40
#define MPE_LONGI_SLOPE	50
#define MPE_LEGEND		60
#define MPE_NOTE		80

#define MPE_TITLE_PLANE			11
#define MPE_TITLE_CROSS			21
#define MPE_TITLE_LONGI			31
#define MPE_TITLE_CANT			41
#define MPE_TITLE_LONGI_SLOPE	51
#define MPE_TITLE_LEGEND		61
#define MPE_TITLE_NOTE			81

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawingVerticalPlane::CDrawingVerticalPlane()
{
	m_nBridge = -1;
}

CDrawingVerticalPlane::~CDrawingVerticalPlane()
{

}

long CDrawingVerticalPlane::GetPageSu()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;

	if(pStd->GetCountTotalPage()<=0)
		pStd->MakePageInfo();

	m_nPage	= pStd->GetCountCategoryPage(_T("종평면도"));

	return m_nPage;
}

CString CDrawingVerticalPlane::GetTitle()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("종평면도"), pStd->m_nCurPage-m_nSttPage);
	if(pInfo==NULL) return _T("");

	CString strTitle = pStd->GetStringTitle(pStd->m_pARcBridgeDataStd->GetBridge(m_nBridge), pInfo->m_strTitle);

	return strTitle;
}

double CDrawingVerticalPlane::GetScale()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pOptData = pOpt->GetDrawingOption(m_nBridge);

	m_Scale	= pOptData->m_dScaleVPBase;
	return pOptData->m_dScaleVPBase;
}

void CDrawingVerticalPlane::SetNumber(long nSubPage)
{
	m_nBridge = GetNumberBridge(nSubPage-m_nSttPage);
}

long CDrawingVerticalPlane::GetNumberBridge(long nCategoryPageNum)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("종평면도"), nCategoryPageNum);
	if(pInfo==NULL) return -1;

	return pInfo->m_nBri;
}

void CDrawingVerticalPlane::DrawOnePage()
{
	TRACE("CDrawingVerticalPlane");
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDataStd* pDataStd = pStd->m_pARcBridgeDataStd;
	CDrawDogak* pDgk = pStd->GetDogak();
	CDrawPageMng* pMng = pStd->GetPageManage();

	long nPage = pStd->m_nCurPage;
	long nSubPage = nPage;

	SetNumber(pStd->m_nDrawPage);
	double dScale = GetScale();

	CDomyun* pDom = pStd->GetDomyun(nPage);
	pDom->SetScaleDim(dScale);

	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("종평면도"), pStd->m_nDrawPage-m_nSttPage);
	if(pInfo==NULL) return;

	nPage = pStd->GetPageNum(pInfo);

	pStd->m_pOptStd->SetEnvType(pDom, HCAD_MIS3);
	pStd->m_pOptStd->SetEnvType(pDom, HCAD_SYMB);

	CDomyun Dom(pDom);

	pMng->SetProgressData(_T("도각 출력 중..."), 0);

	if(m_bDrawForMe)
	{
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

		*pDom << Dom;
	}

	// 종평면도 출력
	AddPageOne(&Dom, nSubPage);

	pMng->SetProgressData(_T("출력 완료  ..."), 100);

	*pDom << Dom;
}

void CDrawingVerticalPlane::AddPageOne(CDomyun* pDomP, long nSubPage)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);

	CRcBridgeRebar *pBri = pStd->m_pARcBridgeDataStd->GetBridgeRebar(m_nBridge);

	CDrawPageMng* pPageMng = pStd->GetPageManage();
	COutMovePoint* pMove = pStd->GetMovePoint(pStd->m_nCurPage);

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pBri->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	double dScale = GetScale();
	double dScaleCross = pData->m_dScaleVPCross;
	double dLengthDraw = DBStd.m_pDrawLine->GetStationJongDanEnd() - DBStd.m_pDrawLine->GetStationJongDanStt();

	DWORD dOptDraw = pData->m_dwDrawVP;

	CDomyun Dom(pDomP);

	Dom.SetScaleDim(dScale);

	// 평면도
	if(dOptDraw & VP_PLANE)
	{
		if(m_pStd->IsDrawByBlock(MPE_PLANE))
		{
			pPageMng->SetProgressData(_T("평면도 출력 중..."), 20);
			AddPlane(&Dom);
			pStd->ChangeSeperateLayer(&Dom);
			Dom.Move(Dom.Always(120)+(dLengthDraw-pBri->m_dLengthBridge)/2, Dom.Always(450));
			Dom.SetCalcExtRect();
			Dom.Move(pMove->GetPoint(MPE_PLANE));
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_PLANE, _T("평면도"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_TITLE_PLANE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("평 면 도"), CDPoint(0,0), 0, dScale);
			Dom.Move(Dom.Always(120)+(dLengthDraw-pBri->m_dLengthBridge)/2, Dom.Always(550));
			Dom.Move(pMove->GetPoint(MPE_TITLE_PLANE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_PLANE, _T("평면도 제목"));
			*pDomP << Dom;
		}
	}

	// 횡단면도
	if(dOptDraw & VP_CROSS)
	{
		if(m_pStd->IsDrawByBlock(MPE_TITLE_PLANE))
		{
			pPageMng->SetProgressData(_T("횡단면도 출력 중..."), 40);
			AddCrossSection(&Dom);
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			Dom.Move(Dom.Always(550), Dom.Always(530));
			Dom.Move(pMove->GetPoint(MPE_CROSS));
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_CROSS, _T("횡단면도"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_TITLE_PLANE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("횡 단 면 도"), CDPoint(0,0), 0, dScaleCross);
			Dom.Move(Dom.Always(550) + pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt,CDPoint(0,1)), Dom.Always(550));
			Dom.Move(pMove->GetPoint(MPE_TITLE_CROSS));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_CROSS, _T("횡단면도 제목"));
			*pDomP << Dom;
		}
	}
	
	// 종단면도
	if(dOptDraw & VP_LONGI)
	{
		if(m_pStd->IsDrawByBlock(MPE_LONGI))
		{
			pPageMng->SetProgressData(_T("종단면도 출력 중..."), 60);
			AddJongDan(&Dom);
			pStd->ChangeSeperateLayer(&Dom);
			Dom.Move(Dom.Always(120)+(dLengthDraw-pBri->m_dLengthBridge)/2, Dom.Always(280));
			Dom.SetCalcExtRect();
			Dom.Move(pMove->GetPoint(MPE_LONGI));
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_LONGI, _T("종단면도"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_TITLE_LONGI))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("종 단 면 도"), CDPoint(0,0), 0, dScale);
			Dom.Move(Dom.Always(120)+(dLengthDraw-pBri->m_dLengthBridge)/2, Dom.Always(370));
			Dom.Move(pMove->GetPoint(MPE_TITLE_LONGI));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_LONGI, _T("종단면도 제목"));
			*pDomP << Dom;
		}
	}

	// 편경사도
	CDRect rcPyun(0, 0, 0, 0);
	if(dOptDraw & VP_CANT)
	{
		if(m_pStd->IsDrawByBlock(MPE_CANT))
		{
			pPageMng->SetProgressData(_T("편경사도 출력 중..."), 70);
			AddCant(&Dom);
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			Dom.Move(Dom.Always(620), Dom.Always(350));
			Dom.Move(pMove->GetPoint(MPE_CANT));
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_CANT, _T("편경사도"));
			*pDomP << Dom;
		}
			
		if(m_pStd->IsDrawByBlock(MPE_TITLE_CANT))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("편 경 사 도"), CDPoint(0,0), 0, 0);
			Dom.Move(Dom.Always(620)+Dom.Always(80), Dom.Always(370));
			Dom.Move(pMove->GetPoint(MPE_TITLE_CANT));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_CANT, _T("편경사도 제목"));
			*pDomP << Dom;
		}
	}
	// 종경사도
	if(dOptDraw & VP_LONGI_SLOPE)
	{
		if(m_pStd->IsDrawByBlock(MPE_LONGI_SLOPE))
		{
			pPageMng->SetProgressData(_T("종경사도 출력 중..."), 80);
			AddVLineDom(&Dom);
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			Dom.Move(Dom.Always(620), Dom.Always(450));
			Dom.Move(pMove->GetPoint(MPE_LONGI_SLOPE));
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_LONGI_SLOPE, _T("종경사도"));
			*pDomP << Dom;
		}
			
		if(m_pStd->IsDrawByBlock(MPE_TITLE_LONGI_SLOPE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("종 경 사 도"), CDPoint(0,0), 0, 0);
			Dom.Move(Dom.Always(620)+Dom.Always(80), Dom.Always(470));
			Dom.Move(pMove->GetPoint(MPE_TITLE_LONGI_SLOPE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_LONGI_SLOPE, _T("종경사도 제목"));
			*pDomP << Dom;
		}
	}

	// 주상도 범례
	if(dOptDraw & VP_LEGEND)
	{
		if(m_pStd->IsDrawByBlock(MPE_LEGEND))
		{
			pPageMng->SetProgressData(_T("범례 출력 중..."), 85);
			AddBoringLegend(&Dom);
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			Dom.Move(Dom.Always(500), Dom.Always(350));
			Dom.Move(pMove->GetPoint(MPE_LEGEND));
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_LEGEND, _T("범례"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_TITLE_LEGEND))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("범 례"), CDPoint(0,0), 0, -1);
			Dom.Move(Dom.Always(500)+Dom.Always(60), Dom.Always(355));
			Dom.Move(pMove->GetPoint(MPE_TITLE_LEGEND));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_LEGEND, _T("범례 제목"));
			*pDomP << Dom;
		}
	}

	// NOTE
	if(dOptDraw & VP_NOTE)
	{
		if(m_pStd->IsDrawByBlock(MPE_NOTE))
		{
			pPageMng->SetProgressData(_T("NOTE 출력 중..."), 90);
			AddNote(&Dom);
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			Dom.Move(Dom.Always(620), Dom.Always(250));
			Dom.Move(pMove->GetPoint(MPE_NOTE));
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_NOTE, _T("NOTE"));
			*pDomP << Dom;
		}
			
		if(m_pStd->IsDrawByBlock(MPE_TITLE_NOTE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("NOTE"), CDPoint(0,0), 0, -1);
			Dom.SetCalcExtRect();
			Dom.Move(Dom.Always(620)+Dom.Always(80), Dom.Always(255));
			Dom.Move(pMove->GetPoint(MPE_TITLE_NOTE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_NOTE, _T("NOTE"));
			*pDomP << Dom;
		}
	}
}

void CDrawingVerticalPlane::AddPlane(CDomyun* pDomP)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);

	CARcBridgeDataStd* pDataStd = pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pDataStd->GetBridgeRebar(m_nBridge);
	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	double dScale = GetScale();

	CDomyun Dom(pDomP);

	Dom.SetScaleDim(dScale);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	//pBri->SyncBridge(0, FALSE, TRUE, TRUE);
	pBri->GetTvVertSection_Std(0, FALSE, FALSE);
	pBri->SyncBridgeAps(0, TRUE);
	
	// 교량 평면
	DBStd.m_pDrawPlane->DrawPlaneSlabOnlyOutLine(&Dom);
	if(pData->m_bDrawVPCoordinateXY)
	{
		long nQtyJijum = pBri->GetCountJijum();
		for(long nJ=0; nJ<nQtyJijum; nJ++)
			DBStd.m_pDrawPlane->MarkPlaneCoordinates(&Dom,nJ,TRUE,FALSE);
	}

	DBStd.m_pDrawPlane->MarkPlaneArrowDirection(&Dom);
	DBStd.m_pDrawLine->DimSpanLengthPlan(&Dom,TRUE,TRUE);

	// 벽체 및 기둥 평면
	int nCountInWall = pBri->GetCountInWall();
	int i=0; for(i=0; i<nCountInWall; i++)
	{
		CWallApp *pWall = pBri->GetInWall(i);
		if(pWall)
		{
			DBStd.m_pDrawWall->DrawPlan(&Dom, pWall, FALSE);
			if(pWall->m_bTopIsColumn)
				DBStd.m_pDrawColumn->DrawPlan(&Dom, &pWall->m_Column);
		}
	}

	// 날개벽 평면
	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	pBri->GetTvWingWallPlane(TRUE);
	DBStd.m_pDrawWingWall->DrawGenPlane(&Dom, TRUE, TRUE);
	pBri->GetTvWingWallPlane(FALSE);
	DBStd.m_pDrawWingWall->DrawGenPlane(&Dom, FALSE, TRUE);

	// 접속 슬래브 평면
	for(int nStt=0; nStt<2; nStt++)
	{
		for(int nLeft=0; nLeft<2; nLeft++)
		{
			CApsApp* pAps = nStt==0 ? &pBri->m_apsStt[nLeft] : &pBri->m_apsEnd[nLeft];
			if(pAps->m_bIs)
			{
				DBStd.m_pDrawAps->DrawPlane(&Dom, pAps);
				// 완충슬래브 평면도
				for(int nCns=0; nCns<MAX_CNS; nCns++)
				{
					CApsApp* pCns = nStt==0 ? &pBri->m_cnsStt[nLeft][nCns] : &pBri->m_cnsEnd[nLeft][nCns];
					if(pCns)
						DBStd.m_pDrawAps->DrawPlane(&Dom, pCns);
				}
			}
		}
	}

	CRcBridgeRebar* pBriMatch = dynamic_cast<CRcBridgeRebar*>(pDataStd->GetBridgeMatch(pBri));
	if(pBriMatch)
	{
		CARcBridgeDBStandard DBStd(pBriMatch, pStd->m_pARoadOptionStd, pDataStd);
		DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
		pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
		pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_DASH_LINE);
		//pBriMatch->SyncBridge(0, FALSE, TRUE, TRUE);
		pBriMatch->GetTvVertSection_Std(0, FALSE, FALSE);
		pBriMatch->SyncBridgeAps(0, TRUE);
		
		CDomyun DomCount(pDomP);
	// 교량 평면
		DBStd.m_pDrawPlane->DrawPlaneSlabOnlyOutLine(&DomCount);

		// 벽체 및 기둥 평면
		int nCountInWall = pBriMatch->GetCountInWall();
		int i=0; for(i=0; i<nCountInWall; i++)
		{
			CWallApp *pWall = pBriMatch->GetInWall(i);
			if(pWall)
			{
				DBStd.m_pDrawWall->DrawPlan(&DomCount, pWall, FALSE);
				if(pWall->m_bTopIsColumn)
					DBStd.m_pDrawColumn->DrawPlan(&DomCount, &pWall->m_Column);
			}
		}

		// 날개벽 평면
		pBriMatch->GetTvWingWallPlane(TRUE);
		DBStd.m_pDrawWingWall->DrawGenPlane(&DomCount, TRUE);
		pBriMatch->GetTvWingWallPlane(FALSE);
		DBStd.m_pDrawWingWall->DrawGenPlane(&DomCount, FALSE);

		// 접속 슬래브 평면
		for(int nStt=0; nStt<2; nStt++)
		{
			for(int nLeft=0; nLeft<2; nLeft++)
			{
				CApsApp* pAps = nStt==0 ? &pBriMatch->m_apsStt[nLeft] : &pBriMatch->m_apsEnd[nLeft];
				if(pAps->m_bIs)
				{
					DBStd.m_pDrawAps->DrawPlane(&DomCount, pAps);
					// 완충슬래브 평면도
					for(int nCns=0; nCns<MAX_CNS; nCns++)
					{
						CApsApp* pCns = nStt==0 ? &pBriMatch->m_cnsStt[nLeft][nCns] : &pBriMatch->m_cnsEnd[nLeft][nCns];
						if(pCns)
							DBStd.m_pDrawAps->DrawPlane(&DomCount, pCns);
					}
				}
			}
		}
		// 강재로 도면 점선으로 만들기...
		POSITION rkey;
		POSITION pos = DomCount.m_DrawObjMap.GetStartPosition();
		CDrawObject* pEtt;
		while( pos )
		{
			DomCount.m_DrawObjMap.GetNextAssoc( pos, (void*&)rkey, (void*&)pEtt ) ;
			if(pEtt->GetEttKind() == ETT_LINE)			
				((CObLine *)pEtt)->SetsLineType("HIDDEN");			
			if(pEtt->GetEttKind() == ETT_POLYLINE)			
				((CObPolyLine *)pEtt)->SetsLineType("HIDDEN");			
			if(pEtt->GetEttKind() == ETT_ARC)			
				((CObArc *)pEtt)->SetsLineType("HIDDEN");						
		}
		pBriMatch->GetTvVertSection_Std(0, FALSE, FALSE, TRUE);
		Dom << DomCount;
	}	

	// 도로중심선
	double staStt = DBStd.m_pDrawLine->GetStationJongDanStt();
	double staEnd = DBStd.m_pDrawLine->GetStationJongDanEnd();
	
	CDomyun DomText(pDomP);
	pStd->m_pARoadOptionStd->SetEnvType(&DomText, HCAD_TXTG);
	CString szLineName = pDataStd->GetLineApp()->GetNameLineEach();
	CDPoint xy = pDataStd->GetLineApp()->GetXyLineDis(staStt, -DomText.GetTextHeight()*1.1);
	DomText.SetTextAlignHorz(TA_LEFT);
	DomText.TextOut(xy, szLineName);
	DomText.Rotate(xy, pDataStd->GetLineApp()->GetAngleAzimuth(staStt));
	Dom << DomText;

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
	pDataStd->GetLineApp()->DrawLS(&Dom, staStt, staEnd, 0);


	// 평면현황
	if(!pData->m_szDxfPlanName.IsEmpty())
	{
		Dom.DxfIn(pData->m_szDxfPlanName);
	}

	pBri->GetTvVertSection_Std(0, FALSE, FALSE, TRUE);
	//회전 교량 진행 방향으로 회전
	DBStd.RotateDomByBridgeSttEndXy(&Dom,TRUE);

	*pDomP << Dom;
}

void CDrawingVerticalPlane::AddCrossSection(CDomyun* pDomP)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);

	CARcBridgeDataStd* pDataStd = pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pDataStd->GetBridgeRebar(m_nBridge);
	pBri->SyncBridge(0);
	//pBri->GetTvVertSection_Std(0, FALSE, FALSE);

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	double dScaleCross = pData->m_dScaleVPCross;

	// 종평에 있는 횡단면도는 도로폭 관련 한 도면이므로 직거리로 그림

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScaleCross);

	double dSta = pBri->GetStationOnJijum(0, 0);
	double nJijum = 0;
	CDPoint vAng	= CDPoint(0,1);
	if(pBri->m_nTypeWallForRebarGen == 0)
	{
		dSta = pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, TRUE);
	}
	else if(pBri->m_nTypeWallForRebarGen == 1)
	{
		dSta = pBri->GetStationByTarget(BRIDGE_OFF_STT_STT, 0, TRUE);
	}
	

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DBStd.m_pDrawCrossSection->DrawCrossSection(&Dom, dSta, vAng, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, TRUE, -1);
	DBStd.m_pDrawCrossSection->DrawCrossSlabHunch(&Dom, dSta, vAng, TRUE, -1);
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		DBStd.m_pDrawCrossSection->DrawCrossSection_ArchRibStd(&Dom, TRUE, 1, TRUE, TRUE);
		DBStd.m_pDrawCrossSection->DrawCrossSection_ArchRibStd(&Dom, FALSE, 1, TRUE, FALSE, TRUE);

		// 아치리브에 의해서 점선으로 표시되어야 하는 기초 분리해서 그림 ////////////
		CTwinVectorArray tvArrLeftSide, tvArrLeftSideHidden;
		pBri->m_footingStt.GetTvLeftSide(tvArrLeftSide, tvArrLeftSideHidden, FALSE, 1, TRUE);
		
		CTwinVectorArray tvArrCrossArchRib;
		pBri->GetTvCrossSection_ArchRib(tvArrCrossArchRib, TRUE, 0);

		CHgBaseDrawStd baseDraw;
		baseDraw.DrawTvArrayApplyOverlapShape(&Dom, tvArrLeftSide, tvArrCrossArchRib);
		baseDraw.DrawTvArrayApplyOverlapShape(&Dom, tvArrLeftSideHidden, tvArrCrossArchRib);
		////////////////////////////////////////////////////////////////////////////
		
		
		DBStd.m_pDrawExFooting->DrawLeftSide(&Dom, &pBri->m_footingStt.m_exFooting, 1);
	}

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	DBStd.m_pDrawCrossSection->DimCrossSection(&Dom, dSta, vAng, FALSE);
	DBStd.m_pDrawCrossSection->MarkGenCrossArrowDirection(&Dom, dSta, vAng, TRUE);
	Dom.Mirror(0, TRUE);

	// 10671
	BOOL bLeftDim = FALSE;
	BOOL bRightDim = FALSE;
	BOOL bCenterDim = TRUE;
	if(nJijum == 0 && fabs(pBri->m_dWidthSlabLeft) > fabs(pBri->m_dWidthSlabRight))
	{
		bRightDim = TRUE;
		bCenterDim = FALSE;
	}
	DBStd.m_pDrawCrossSection->DimFrontSide(&Dom, (int)nJijum,FALSE,bRightDim,bCenterDim,TRUE, FALSE);		// 측면 치수
	DBStd.m_pDrawCrossSection->DimCrossSide(&Dom, dSta, CDPoint(0, 1), !bLeftDim, !bRightDim);

	DBStd.m_pDrawCrossSection->MarkGenCrossSlope(&Dom, dSta, vAng, TRUE, FALSE, FALSE, TRUE, TRUE);	// 편경사
//	DBStd.m_pDrawCrossSection->MarkFrontEl(&Dom, nJijum,TRUE,TRUE,FALSE);		// EL 마크
	DBStd.m_pDrawCrossSection->MarkFrontConcFck(&Dom, (int)nJijum,1);	// 콘크리트 심볼

	// 벽체 중심 마크
	CLineInfo *pLine	= pBri->GetLineBase();
	if(pLine)
	{
		double dCenOfRoad	= pBri->GetMidCrossSection(TRUE);
		CFootingApp* pFoot = pBri->GetFooting(0);
		double dEL = pFoot ? pFoot->m_dEL : pLine->GetElevationPlanEl(pBri->m_dStationBridgeStt);	// 기초 상단 EL
		double dELBottom = pFoot? pFoot->m_exFooting.m_dEL-pFoot->m_exFooting.GetMaxHeight():dEL;
		if(pBri->IsBoxType())
		{
			dELBottom = pBri->GetElevationBoxBottom(nJijum);
		}
		double dTP = pBri->GetThickPave(FALSE);
		double dElCenter = pBri->GetXySlabUpperActByDist(dSta, 0).y;
		CDPoint xyCenter = CDPoint(-dCenOfRoad, dElCenter+dTP+Dom.Always(15));
		DBStd.MarkCenterOfRoad(&Dom, xyCenter, (dElCenter-dELBottom)+dTP+Dom.Always(25), _T(""), FALSE);

		CConcSymbol sym;
		sym.ArrowSide(&Dom, "중앙부", "지점부", CDPoint(-dCenOfRoad, (dElCenter+dEL)/2-Dom.Always(15)));

// 		CDPoint vAngJijum = pBri->GetAngleJijum(0);
		CDPoint vAngJijum = CDPoint(Round(vAng.x, 5), Round(vAng.y, 5));
		if(vAngJijum!=CDPoint(0, 1))
		{
			Dom.SetCalcExtRect();
			CDRect rc = Dom.GetExtRect();
			Dom.SetTextAlignHorz(TA_RIGHT);
			Dom.TextOut(rc.right, rc.top, _T("* () 안의 치수는 사거리 치수임."));
		}
	}

	
//	DimCrossSide(&Dom);		// 슬래브 우측 치수

	Dom.RedrawByScale(GetScale()/dScaleCross);

	Dom.SetCalcExtRect();
	CDRect rc = Dom.GetExtRect();

	Dom.Move(-rc.left, -rc.bottom);

	*pDomP << Dom;
}

void CDrawingVerticalPlane::AddCant(CDomyun* pDomP)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);

	CARcBridgeDataStd* pDataStd = pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pDataStd->GetBridgeRebar(m_nBridge);
	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	double dScale = 300;
	CDomyun Dom(pDomP);

	Dom.SetScaleDim(dScale);

	DBStd.m_pDrawLine->DrawCant(&Dom);

	Dom.RedrawByScale(GetScale()/dScale);

	Dom.SetCalcExtRect();
	CDRect rc = Dom.GetExtRect();
	Dom.Move(-rc.left, -rc.bottom);

	*pDomP << Dom;
}

void CDrawingVerticalPlane::AddVLineDom(CDomyun* pDomP)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);

	CARcBridgeDataStd* pDataStd = pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pDataStd->GetBridgeRebar(m_nBridge);
	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	CLineInfo *pLine = pDataStd->GetLineApp();

	double dScale = 400;
	CDomyun Dom(pDomP);

	Dom.SetScaleDim(dScale);

	double dStationStt = max(pBri->m_dStationBridgeStt - 20000,pLine->m_dStationBp);
	double dStationEnd = min(pBri->m_dStationBridgeStt + pBri->m_dLengthBridge + 20000,pLine->GetStationEpLine());
	// 입력된 종단선형을 벗어난 경우.
	double dStaJongSttReal	= pLine->m_VIPData[0][0];
	double dStaJongEndReal	= pLine->m_VIPData[pLine->m_VIPsu-1][0];
	if(dStationStt < dStaJongSttReal || dStationStt > dStaJongEndReal) dStationStt	= dStaJongSttReal;
	if(dStationEnd < dStaJongSttReal || dStationEnd > dStaJongEndReal) dStationEnd	= dStaJongEndReal;

	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
	pLine->DrawJongBridge(&Dom,dStationStt,dStationEnd,pBri->m_dStationBridgeStt,pBri->m_dStationBridgeStt + pBri->m_dLengthBridge);

	CDRect dr;
	Dom.SetCalcExtRect();
	dr = Dom.GetExtRect();
	dr.left   -= Dom.Always(6);
	dr.top	  -= Dom.Always(8);
	dr.bottom += Dom.Always(8);
	dr.right  += Dom.Always(6);
	Dom.Rectangle(dr);

	Dom.RedrawByScale(GetScale()/dScale);

	Dom.SetCalcExtRect();
	CDRect rc = Dom.GetExtRect();
	Dom.Move(-rc.left, -rc.bottom);

	*pDomP << Dom;
}

void CDrawingVerticalPlane::AddBoringLegend(CDomyun* pDomP)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);

	CARcBridgeDataStd* pDataStd = pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pDataStd->GetBridgeRebar(m_nBridge);
	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	double dScale = 300;
	CDomyun Dom(pDomP);

	Dom.SetScaleDim(dScale);

	DBStd.m_pDrawLine->DrawBoringLegend(&Dom, DBStd.m_pDrawLine->GetStationJongDanStt(), DBStd.m_pDrawLine->GetStationJongDanEnd());

	Dom.RedrawByScale(GetScale()/dScale);
	
	Dom.SetCalcExtRect();
	CDRect rc = Dom.GetExtRect();
	Dom.Move(-rc.left, -rc.bottom);

	*pDomP << Dom;
}

void CDrawingVerticalPlane::AddJongDan(CDomyun *pDomP)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);

	CARcBridgeDataStd* pDataStd = pStd->m_pARcBridgeDataStd;
	CLineInfo *pLine = pDataStd->GetLineApp();

	CRcBridgeRebar* pBri = pDataStd->GetBridgeRebar(m_nBridge);
	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	double dScale = GetScale();

	CDomyun Dom(pDomP);

	Dom.SetScaleDim(dScale);

	CDPoint xyBase(0,0);
	// 종단테이블
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_DIML);
	DBStd.m_pDrawLine->DrawJongDanTable(&Dom,xyBase,pBri->m_dStationBridgeStt,pBri->m_dStationBridgeStt+pBri->m_dLengthBridge);

	// 종단면도
	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_CONC);
	DBStd.m_pDrawLine->DrawJongDan(&Dom);
	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();

	// 이동 : 시점스테이션기준

	Dom.Move(-pBri->m_dStationBridgeStt,-pLine->GetElevationPlanEl(pBri->m_dStationBridgeStt));

	*pDomP << Dom;
}

void CDrawingVerticalPlane::AddNote(CDomyun* pDomP)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	
	CStringArray szArrNo;
	CStringArray szArrNot;
	
	szArrNo.RemoveAll();
	szArrNot.RemoveAll();

	long nNumber = 1;
	
	szArrNo.Add(COMMA(nNumber++)+".");
	szArrNot.Add("모든 치수는 mm단위임.");
	szArrNo.Add(COMMA(nNumber++)+".");
	szArrNot.Add("종단계획고상의 EL은 교량 아스팔트 표층상단으로 교량중심선의 EL임.");
	szArrNo.Add(COMMA(nNumber++)+".");
	szArrNot.Add("콘크리트 타설전 구조물의 위치및 EL을 반드시 확인하여야 한다.");
	szArrNo.Add(COMMA(nNumber++)+".");
	szArrNot.Add("콘크리트 타설순서 및 주요 인장재는 시공전 재확인 검토후");
	szArrNo.Add("");
	szArrNot.Add("감독관(감리자)의 승인하에 시공하여야 한다.");
	szArrNo.Add(COMMA(nNumber++)+".");
	szArrNot.Add("모든구조물의 예각부는 모따기를 하여야 한다.");
	szArrNo.Add(COMMA(nNumber++)+".");
	szArrNot.Add("기초 공사시 지하매설물에 손상을 주지 않도록 충분한");
	szArrNo.Add("");
	szArrNot.Add("사전조사를 시행하여야 한다.");
	szArrNo.Add(COMMA(nNumber++)+".");
	szArrNot.Add("기초콘크리트 타설시 지하수의 영향이 없는 건조한 상태로 타설하여야 한다.");
	szArrNo.Add(COMMA(nNumber++)+".");
	szArrNot.Add("모든기초의 심도는 지반여건에 따라 변경될 수 있으므로 기초 시공시");
	szArrNo.Add("");
	szArrNot.Add("지반조건을 확인한후 확실한 지지층에 시공하영야 하며, 설계N치와 상이할");
	szArrNo.Add("");
	szArrNot.Add("경우 감독관의 승인을 거쳐 변경하여야 한다.");
	szArrNo.Add(COMMA(nNumber++)+".");
	szArrNot.Add("PILE기초는 지반조사 결과에 대한 추정치 이므로 시공시 시항타와");
	szArrNo.Add("");
	szArrNot.Add("PILE재하시험을 시행하여 PILE길이와 지지력을 설계도와 확인하여야");
	szArrNo.Add("");
	szArrNot.Add("하며, 변경시에는 감독관의 승인하에 현장여건에 맞게 시공하여야 한다.");
	szArrNo.Add(COMMA(nNumber++)+".");
	szArrNot.Add("콘크리트 부재 단면이 두꺼운 경우 타설전 충분히 온도 균열방지 방안을");
	szArrNo.Add("");
	szArrNot.Add("검토 수렴한 후 시공대책을 강구하여야 한다.");
	szArrNo.Add(COMMA(nNumber++)+".");
	szArrNot.Add("상부구조 시공시 하부도로의 교통소통 및 안전관리에 유의하여야 하며,");
	szArrNo.Add("");
	szArrNot.Add("낙하물 방지공을 필요시 설치한다.");
	szArrNo.Add(COMMA(nNumber++)+".");
	szArrNot.Add("기초 굴착후 굴착 지반에 대한 FACE MAPPING을 시행토록 하고");
	szArrNo.Add("");
	szArrNot.Add("기초지반의 절리나 불연속에 대한 정밀 검사의 필요시 감독관(김리자)의");
	szArrNo.Add("");
	szArrNot.Add("승인을 득한 후 보강(GROUTING)을 실시한 후 시공하여야 한다.");
	szArrNo.Add(COMMA(nNumber++)+".");
	szArrNot.Add("가설방법 및 가시설물(가시설물, 교량상부 동바리, 가도 , 축도 등...)은");
	szArrNo.Add("");
	szArrNot.Add("기본설계 개념이므로 시공전 현장여건에 맞게 서부설계르 하여 감독원의");
	szArrNo.Add("");
	szArrNot.Add("승인을 득한 후 시공하여야 한다.");
	szArrNo.Add(COMMA(nNumber++)+".");
	szArrNot.Add("공법상 특기사항은 공사시방서에 준하며, 감독원의 승인하에");
	szArrNo.Add("");
	szArrNot.Add("변경 시행할 수 있다.");
	szArrNo.Add(COMMA(nNumber++)+".");
	szArrNot.Add("시공자는 설계도에 의한 구조물을 현장여건에 맞게 축조하기 위하여");
	szArrNo.Add("");
	szArrNot.Add("안전성과 내구성을 고려한 독립적인 검토를 시행하여 시공상세도를");
	szArrNo.Add("");
	szArrNot.Add("작성하고 감독원의 승인을 받아야 한다.");
//	szArrNo.Add(COMMA(nNumber++)+".");
//	szArrNot.Add("교면 방수 : 아스팔트 방수");
//	if(pBri->IsPileBase(-1))
//	{
//		szArrNo.Add(COMMA(nNumber++)+".");
//		szArrNot.Add("말뚝 시공법  : 천공항타 공법");
//		szArrNo.Add(COMMA(nNumber++)+".");
//		szArrNot.Add("말뚝 두부보강 상세도 : 부대공(공통) 참조");
//	}
	
	long nSize = szArrNot.GetSize();
	CGridDomyunEx Dom(pDomP);
	double dWidthTxt = Dom.GetTextWidth(szArrNot.GetAt(8), 0);
	double dWidthNo = Dom.GetTextWidth(szArrNo.GetAt(0),0);
	pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_NOTE);
	
	double dSpaceWidFirst = Dom.Always(20);
	double dSpaceWidLast = dWidthTxt + Dom.Always(20) + dWidthNo + Dom.Always(20);

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
	Dom.Move(-rc.left, -rc.bottom);
	*pDomP << Dom;
}
