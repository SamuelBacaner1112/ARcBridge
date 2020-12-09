// DrawCrossSection.cpp: implementation of the CDrawCrossSection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARcBridgeDBDraw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawCrossSection::CDrawCrossSection()
{

}

CDrawCrossSection::~CDrawCrossSection()
{

}

//<summary>
//슬래브 선을 그림(지중라멘 일 경우에 도로 횡단은 따로 그림
//</summary>
//<param name="sta">선형중심 스테이션</param>
//<param name="bMarkCenter">선형중심 지시선 그림</param>
//<param name="bMarkLeft">좌측 선형중심 지시선 그림</param>
//<param name="bMarkRight">우측 선형중심 지시선 그림</param>
void CDrawCrossSection::DrawSimpleCross(CDomyun* pDomP, double sta, BOOL bMarkCenter, BOOL bMarkLeft, BOOL bMarkRight)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	CDomyun Dom(pDomP);

	CTwinVectorArray tvArr;
	pBri->GetTvCrossSection(tvArr, sta, CDPoint(0, 1), FALSE, TRUE, FALSE, 0, FALSE, FALSE);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	m_pStd->DrawTvArray(&Dom, tvArr);

	// 도로 중심선
	CLineInfo* pLineC = pBri->GetLineBase();		// 중심 선형

	CString strC = pLineC->GetNameLineEach();
	double dDistC = 0;
	double dLupper = 1200;			// 중심선 길이
	double dLlower = 1500;			// 중심선 길이
	double dExtraStr = Dom.Always(4);
	double dY = pBri->GetElMaxSlabUpper(sta, CDPoint(0, 1));
	if(pBri->m_bUnderGroundRahmen)
	{
		double dElRoad   = m_pStd->m_pARcBridgeDataStd->GetLineApp()->GetElevationPlanEl(sta);
		double dElBridge = pLineC->GetElevationPlanEl(sta);
		dY += (dElRoad - dElBridge);
		dLlower +=  (dElRoad - dElBridge);
	}

	double dSttSta = pBri->GetStationBridgeMid();
	
	// 좌측단 선형
	CLineInfo* pLine = pBri->GetLineLeftRight(TRUE);
	CDPoint vAng(0, 1);
	double dDistL = pBri->GetDistLineLeftOrRight(sta, vAng, TRUE);
	CString strL = pLine->GetNameLineEach();

	// 우측단 선형
	pLine = pBri->GetLineLeftRight(FALSE);
	double dDistR = pBri->GetDistLineLeftOrRight(sta, vAng, FALSE);
	CString strR = pLine->GetNameLineEach();

	// 그리기
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
	if(bMarkCenter)
	{
		Dom.LineTo(dDistC, dY-dLlower, dDistC, dY+dLupper);
	}
	if(bMarkLeft && strC!=strL)
	{
		Dom.LineTo(dDistL, dY-dLlower, dDistL, dY+dLupper);
	}
	if(bMarkRight && strC!=strR)
	{
		Dom.LineTo(dDistR, dY-dLlower, dDistR, dY+dLupper);
	}

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	if(bMarkCenter)
	{
		Dom.TextOut(dDistC, dY+dLupper+dExtraStr, _T("도로 중심"));
		Dom.TextOut(dDistC, dY+dLupper+dExtraStr-Dom.GetTextHeight()-Dom.Always(1),"("+strC+")");
	}
	if(bMarkLeft && strC!=strL)
	{
		Dom.TextOut(dDistL, dY+dLupper+dExtraStr, strL);
	}
	if(bMarkRight && strC!=strR)
	{
		Dom.TextOut(dDistR, dY+dLupper+dExtraStr, strR);
	}

	*pDomP << Dom;
}

// BOOL bOtherBridge : 상대교량일 경우 SL, Sr값 출력 dim을 아래로 표현한다.
void CDrawCrossSection::DimCrossDistSlab(CDomyun* pDomP, BOOL bInput , BOOL bDimAllSection, BOOL bOtherBridge, double dSta)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	double dL = 3000;			// 중심선 길이
	CDPoint vAng(0, 1);
	double sta = dSta;//pBri->GetStationBridgeMid();
	/*
	if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pBri->GetJijum(0)->m_dLengthUgan[1];
	if(pBri->m_bIsExp || pBri->IsExtLine())
		sta = pBri->GetStationBridgeMid();
		*/

	double dWidth = pBri->GetWidthSlabAct(sta, vAng);
	double dY = pBri->GetElMaxSlabUpper(sta, vAng);
	double dYMin = pBri->GetElMinSlabUpper(sta, vAng);
	double dSL = pBri->GetWidthSlabFrLineLeftOrRight(sta, vAng, TRUE);
	double dSR = pBri->GetWidthSlabFrLineLeftOrRight(sta, vAng, FALSE);
	double dDistL = pBri->GetDistLineLeftOrRight(sta, vAng, TRUE);
	double dDistR = pBri->GetDistLineLeftOrRight(sta, vAng, FALSE);
	if(bDimAllSection)
	{
		CRcBridgeApp *pBriMatch = m_pStd->m_pARcBridgeDataStd->GetBridgeMatch(pBri);
// 		double dY_Match = 0;
		if(pBriMatch)
		{
			double dY_Match	= pBriMatch->GetElMaxSlabUpper(sta, vAng);
			dY			= max(dY_Match,dY);

			dY_Match	= pBriMatch->GetElMinSlabUpper(sta, vAng);
			dYMin		= min(dY_Match, dYMin);
		}
	}

	CDomyun Dom(pDomP);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.DimMoveTo(dDistL+dSL, dY+dL/2, 1);
	Dom.DimLineTo(dWidth, 1, COMMA(dWidth));

	if(bOtherBridge)
	{
		Dom.SetDirection("BOTTOM");
		dY	= dYMin;
		dL	= -dL;
	}

	if(bInput)
	{
		if(pBri->IsExtLine())
		{
			Dom.DimMoveTo(dDistL, dY+dL/2, 0);
			Dom.DimLineToExtend(dSL, 0, _T("SL"), COMMA(dSL)+"("+COMMA(pBri->m_dWidthSlabLeft)+")");
			Dom.DimMoveTo(dDistR, dY+dL/2, 0);
			Dom.DimLineToExtend(dSR, 0, _T("SR"), COMMA(dSR)+"("+COMMA(pBri->m_dWidthSlabRight)+")");
		}
		else
		{
			Dom.DimMoveTo(dDistL, dY+dL/2, 0);
			Dom.DimLineToExtend(dSL, 0, _T("SL"), COMMA(dSL));
			Dom.DimMoveTo(dDistR, dY+dL/2, 0);
			Dom.DimLineToExtend(dSR, 0, _T("SR"), COMMA(dSR));
		}
	}
	else
	{
		Dom.DimMoveTo(dDistL, dY+dL/2, 0);
		Dom.DimLineTo(dSL, 0, COMMA(dSL));
		Dom.DimMoveTo(dDistR, dY+dL/2, 0);
		Dom.DimLineTo(dSR, 0, COMMA(dSR));
	}

	*pDomP << Dom;
}

void CDrawCrossSection::DrawSimpleShoe(CDomyun* pDomP, double dEL, double dDist)
{
	CDomyun Dom(pDomP);

	double dW = 500;
	double dH = 400;

	Dom.SetLineColor(2);
	Dom.Rectangle(-150+dDist, dEL, 150+dDist, dEL-150);
	Dom.Rectangle(-dW/2+dDist, dEL-150, dW/2+dDist, dEL-dH);

	*pDomP << Dom;
}

void CDrawCrossSection::DimCrossDistShoe(CDomyun* pDomP, CString strJiJum, BOOL bInput)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	CJijum* pJ = NULL;

	CDomyun Dom(pDomP);
	Dom.SetDirection("BOTTOM");
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	
	CDPoint vAng(0, 1);
	int nCountJijum = pBri->GetCountJijum();
	int i=0; for(i=0; i<nCountJijum; i++)
	{
		pJ = pBri->GetJijum(i);
		for(int nShoe=0; nShoe<2; nShoe++)
		{
			if(pJ->m_strJijumShoe[nShoe]==strJiJum)
			{
				CDPoint xyR(0, 0);
				CDPointArray xyArr;
				CString str = _T("");
				pBri->GetDimArrayShoeDist(xyArr, i, nShoe);
				int nSize = xyArr.GetSize();
				double sta = pBri->GetStationShoeOnJijum(i, 0, nShoe);
				double dLeftDist = pBri->GetDisSlabLeftAct(sta, vAng);
				double dEL = pBri->GetElMinSlabUpper(sta, vAng) - pBri->m_dTS;

				Dom.DimMoveTo(dLeftDist, dEL, 0);
				long k=0; for(k=0; k<nSize; k++)
				{
					xyR = xyArr.GetAt(k);
					str = xyR.x==1 ? COMMA(xyR.y) : COMMA(xyR.x)+_T("@")+COMMA(xyR.y)+_T("=")+COMMA(xyR.x*xyR.y);
					Dom.DimLineTo(xyR.x*xyR.y, 0, str);
				}

				double dSlabWidth = pBri->GetWidthSlabAct(sta, vAng);
				Dom.DimMoveTo(dLeftDist, dEL, 1);
				Dom.DimLineTo(dSlabWidth, 1, COMMA(dSlabWidth));

				*pDomP << Dom;
			}
		}
	}

	if(bInput)
	{
		int i=0; for(i=0; i<nCountJijum; i++)
		{
			pJ = pBri->GetJijum(i);
			for(int nLeft=0; nLeft<2; nLeft++)
			{
				if(pJ->m_strJijumShoe[nLeft]==strJiJum)
				{
					long nCountShoe = pJ->m_nQtyShoe[nLeft];
					for(int nShoe=0; nShoe<nCountShoe; nShoe++)
					{
						double sta = pBri->GetStationShoeOnJijum(i, 0, nShoe);
						double dEL = pBri->GetElMinSlabUpper(sta, CDPoint(0, 1)) - pBri->m_dTS;
						double dDist = pJ->m_dDistShoe[nLeft][nShoe];
						Dom.SetDirection(_T("BOTTOM"));
						Dom.DimMoveTo(0, dEL, nShoe+2);
						Dom.DimLineToExtend(dDist, nShoe+2, _T("L")+COMMA(nShoe+1), Comma(dDist));
						if(dDist==0)
						{
							double dSpare = Dom.Always(Dom.GetDimExo()+Dom.GetDimVLen()-Dom.GetDimTvp());
							Dom.TextOut(0, dEL-dSpare, _T("L")+COMMA(nShoe+1));
						}
					}
				}
			}
		}
		*pDomP << Dom;
	}

}
//<summary>
//횡단 슬래브, 벽체, 기초, 보도, 슬래브 하면, 방호벽 그리기
//</summary>
//<param name="sta">기준선형 스테이션</param>
//<param name="vAng">기준선형 각도</param>
//<param name="bInclSubStruct">벽체, 기초 그리기</param>
//<param name="bInclFootway">보도 그리기</param>
//<param name="bInclSlabLower">슬래브 하면 그리기</param>
//<param name="bInclGuardWall">방호벽 그리기</param>
//<param name="bInclPave">포장 그리기</param>
//<param name="bDrawPile">기초 그리기(bInclSubStruct == TRUE 일 경우에만 적용됨)</param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
// long nDrawLeftOfSubStruct : 하부구조물 그리는 영역
void CDrawCrossSection::DrawCrossSection(CDomyun* pDomP, double sta, CDPoint vAng, BOOL bInclSubStruct, 
										 BOOL bInclFootway, BOOL bInclSlabLower, BOOL bInclGuardWall, BOOL bInclPave, BOOL bDrawPile, BOOL bRoadNotSlab, BOOL bExp, BOOL bInner, long nDrawLeftOfSubStruct, long nJSubStruct, BOOL bHatch, BOOL bRubble)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri->m_bUnderGroundRahmen && bRoadNotSlab)	return;
	if(!pBri->IsExp(sta))
		bExp	= FALSE;

	if(pBri->IsBoxType())
	{
		DrawCrossSectionBoxType(pDomP, sta, vAng, bInclSubStruct, bInclFootway, bInclSlabLower, bInclGuardWall, bInclPave, bDrawPile, bRoadNotSlab, bExp, bInner, nDrawLeftOfSubStruct, nJSubStruct, bHatch, bRubble);
		return;
	}

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DrawSlabDosang(pDomP, sta, vAng, bRoadNotSlab);
	CTwinVectorArray tvArr;
	pBri->GetTvCrossSection(tvArr, sta, vAng, bInclFootway, bInclSlabLower, bInclGuardWall, 0, FALSE, bInclPave, bRoadNotSlab, bExp);
	m_pStd->DrawTvArray(&Dom, tvArr, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());

	if(/*pBri->m_nTypeBridge==BRIDGE_TYPE_PORTAL && */bInclSubStruct)
	{
		BOOL bVert	= Compare(vAng.GetAngleDegree(), 90.0, "=");
		CTwinVector tv;
		int nJ = pBri->GetJijumNumberByStation(sta);
		CDPoint vAngJ	= pBri->GetAngleJijum(nJ);
		long nCountJijum	= pBri->GetCountJijum();

		nJ	= -1;
// 		double dWL	= 0;
// 		double dWR	= 0;
		long i = 0; for(i = 0; i < nCountJijum; i++)
		{
			double dWL	= 0;
			double dWR	= 0;
			double dStaTmp	= pBri->GetStationOnJijum(i);
			if(i == 0) 
			{
				if(Compare(dStaTmp, sta, ">=")) 
				{
					nJ	= i;
					break;
				}
				dWL	= pBri->m_dWS/vAngJ.y;
				dWR	= 0;
			}
			else if(i == nCountJijum-1)
			{
				if(Compare(dStaTmp, sta, "<="))
				{
					nJ	= i;
					break;
				}
				dWL	= 0;
				dWR	= pBri->m_dWE/vAngJ.y;
			}
			else 
			{
				CWallApp *pWall	= pBri->GetInWall(i-1);
				if(pWall)
				{
					dWL	= pWall->GetWidthWall()/vAngJ.y/ToDPointFrDegree(pWall->GetAngleWallByHor(TRUE)).y/2;
					dWR	= dWL;
				}
			}

			if(Compare(sta, dStaTmp-dWL, ">=") && Compare(sta, dStaTmp+dWR, "<="))
			{
				nJ	= i;
				break;
			}
		}

		if(nJSubStruct != -1)
			nJ	= nJSubStruct;

		if(nJ>=0)
		{
			BOOL bLeft	= TRUE;
			BOOL bDrawFooting	= TRUE;
			CWallApp wall;
			// 벽체
			if(nJ == 0 || nJ == pBri->GetCountJijum()-1)
			{
				if(pBri->IsOutWall(nJ==0))
				{
					pBri->MakeWallByOutWall(wall, nJ==0, !bVert, bVert, bVert, 0, sta);
					
					// 가각부 처리
					if(bExp)
					{
						double dWidthLeftExp	= pBri->GetWidthDiffSlabActOrgAndGagak(nJ == 0, TRUE, FALSE);
						double dWidthRightExp	= pBri->GetWidthDiffSlabActOrgAndGagak(nJ == 0, FALSE, FALSE);
						wall.m_dWidthExp[iLEFT]		= dWidthLeftExp;
						wall.m_dWidthExp[iRIGHT]	= dWidthRightExp;

						double dWidthOverRebarExpLeft	= pBri->GetDiffLeftRightDistForGagakSabogang(nJ == 0, TRUE);
						double dWidthOverRebarExpRight	= pBri->GetDiffLeftRightDistForGagakSabogang(nJ == 0, FALSE);
						wall.m_dWidthOverRebarExp[iLEFT]	= dWidthOverRebarExpLeft;
						wall.m_dWidthOverRebarExp[iRIGHT]	= dWidthOverRebarExpRight;
					}

					m_pStd->m_pDrawWall->DrawLeftSide(&Dom, &wall, FALSE, FALSE, bLeft, nDrawLeftOfSubStruct, FALSE, TRUE);
				}
			}
			else
			{
				CWallApp *pWall	= pBri->GetInWall(nJ-1);
				if(pWall)
				{
					if(pWall->m_bIs)
					{
						bLeft	= pWall->IsLeftSlope();
						m_pStd->m_pDrawWall->DrawLeftSide(&Dom, pWall, FALSE, FALSE, bLeft, nDrawLeftOfSubStruct, FALSE, TRUE, TRUE, TRUE);

						if(pWall->m_nType == WALL_TYPE_SLOPE)
						{
							bDrawFooting	= FALSE;
							CTwinVectorArray tvArrFooting, tvArrHidden;
							CFootingApp *pFooting	= pWall->GetFootingByRotateToVert(bLeft);
							pFooting->GetTvLeftSide(tvArrFooting, tvArrHidden, FALSE, 0, bLeft);
														
							CHgBaseDrawStd baseDraw;
							baseDraw.DrawTvArray(&Dom, tvArrFooting);

							pFooting->m_exFooting.GetTvFront(!bVert, TRUE, bLeft);
							baseDraw.DrawTvArray(&Dom, pFooting->m_exFooting.m_tvArrSide);
						}
					}
				}
			}

			// 기초/확대기초
			CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(nJ);
			if((nJ == 0 || nJ == pBri->m_nQtyJigan) && pBri->IsOutWall(nJ==0))
			{
				pFooting	= &wall.m_footing;
			}
			if(pFooting && bDrawFooting)
			{
				if(pFooting->m_bIs || pFooting->IsVirtualFooting())
				{
					// (ARCBRIDGE-3237) 벽체 배면위치의 기초를 표현 하기 위해서 벽체 전면에서의 Offset을 구해서 처리한다.
					double dOffsetDist(0);
					if(pBri->m_nTypeWallForRebarGen == 0)
					{
						if(nJ == 0 || nJ == pBri->m_nQtyJigan)
						{
							dOffsetDist = wall.m_dW * (nJ == 0 ? 1 : -1);
						}
						if(Compare(dOffsetDist, 0.0, _T("=")) == FALSE)
						{
							pFooting->SyncFooting(Compare(vAng.y, 0.0, _T("=")), pBri->GetAngleJijum(nJ).GetAngleDegree(), FALSE, FALSE, dOffsetDist);
						}
					}
					m_pStd->m_pDrawFooting->DrawLeftSide(&Dom, pFooting, FALSE,  nDrawLeftOfSubStruct, bLeft, FALSE, dOffsetDist);
					m_pStd->m_pDrawExFooting->DrawLeftSide(&Dom, &pFooting->m_exFooting, nDrawLeftOfSubStruct, bDrawPile, bDrawPile, !bVert, bLeft);
				}
			}

			if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH && nJ > 0 && nJ < pBri->m_nQtyJigan)
			{
				CWallApp *pWall	= pBri->GetInWall(nJ-1);
				if(pWall)
				{
					double dXDis	= pWall->GetXyOrgTopForFront(bLeft).x;
					if(pWall->m_bTopIsColumn)
						dXDis	= (dXDis + pWall->GetXyOrgTopForFront(!bLeft).x)/2;
					double dX		= pBri->m_pbRib[iUPPER].GetDistByXPos(dXDis);
					DrawCrossSection_ArchRib(&Dom, dX, 0, TRUE);
				}
			}
		}
	}

	if(bRoadNotSlab)
	{
		// (ARCBRIDGE-3611) 상하행 분리라도 성토부가 붙어있지 않을수도 있다. 그냥 다 그린다. 게다가 하행좌측 상행 우측을 안그리는건 방향도 트리다.상행이 오른쪽에 위치하기 때문에 왼쪽을 안그려야....
		// if(pBri->m_nSeparBri != 2)	// 하행인 경우 좌측을 그리지 않음(분리교량이 아닌 경우는 모두 그림)
		{
			pBri->GetTvCrossSectionUnderNori(tvArr, sta, vAng, TRUE);// 도로좌측의 성토사면
			m_pStd->DrawTvArray(&Dom, tvArr, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());
		}
		// if(pBri->m_nSeparBri != 1)	// 상행인 경우 우측을 그리지 않음(분리교량이 아닌 경우는 모두 그림)
		{
			pBri->GetTvCrossSectionUnderNori(tvArr, sta, vAng, FALSE);// 도로우측의 성토사면
			m_pStd->DrawTvArray(&Dom, tvArr, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());
		}

		if(bHatch)
			DrawCrossSectionUnderHeapSoilSymbol(&Dom, sta, vAng);
	}

	*pDomP << Dom;
}

//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
void CDrawCrossSection::DrawSlabDosang(CDomyun *pDomP, double sta, CDPoint vAng, BOOL bRoadNotSlab)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!m_pStd->m_pARcBridgeDataStd->IsRailDesign())
		return;
	if(pBri->m_bUnderGroundRahmen && !bRoadNotSlab)	//지중라멘교이고 슬래브면을 출력하는 경우
		return;
	CDomyun Dom(pDomP);
	long nQtyHDan = pBri->GetQtyHDan(bRoadNotSlab);

	for(long nHDan = 0; nHDan < nQtyHDan; nHDan++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan,bRoadNotSlab);

		if(!pGW)	continue;
		if(pGW->IsTypeHDanRailTotalDosang())	//직결식도상, 자갈도상 이면
		{
			DrawSlabDosangByHDan(pDomP, sta, vAng, nHDan,bRoadNotSlab);
		}
	}
	long n = 0; for(n = 0; n < pBri->GetQtyRailNode(); n++)	
	{
		DrawSlabDosangRailCenter(pDomP, sta, vAng, n);	//레일 센터 십자 심볼 그리기
		DrawRailSymbol(pDomP, sta, vAng, n);			//레일 궤도 심볼 그리기
	}

	*pDomP << Dom;
}

//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
void CDrawCrossSection::DrawSlabDosangByHDan(CDomyun *pDomP, double sta, CDPoint vAng, long nHDan, BOOL bRoadNotSlab)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan,bRoadNotSlab);
	if(!pGW) return;
	if(!pGW->IsTypeHDanRailTotalDosang())	return;	//도상이 아니면

	CDomyun Dom(pDomP);
	CDPoint A[6]; 
	pBri->GetXyDosang(sta, vAng, nHDan, A,bRoadNotSlab);// 도상의 포인트
	long n = 0; for(n = 1; n < 4; n++)
	{
		if(A[n] != A[n+1])
		{
			Dom.LineTo(A[n], A[n+1]);
		}
	}

	*pDomP << Dom;
}

void CDrawCrossSection::DrawSlabDosangRailCenter(CDomyun *pDomP, double sta, CDPoint vAng, long nRail)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(nRail >= pBri->GetQtyRailNode())	return;	//전체 레일 개소수 보다 클 경우

	double lenSymbol = 100;
	double disRail = pBri->GetLengthRailByLineByCenLine(sta, vAng, nRail);
	CDPoint xyResult = CDPoint(0, 0);
	BOOL	bMatch = pBri->IsMatchDosangByDis(sta, vAng, disRail, xyResult);
	if(!bMatch)	return;
	
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_BARC);
	xyResult.y += pBri->m_dHeightRail;
	Dom.LineTo(xyResult.x-lenSymbol, xyResult.y, xyResult.x+lenSymbol, xyResult.y);
	Dom.LineTo(xyResult.x, xyResult.y-lenSymbol, xyResult.x, xyResult.y+lenSymbol);

	*pDomP << Dom;
}

void CDrawCrossSection::DrawRailSymbol(CDomyun *pDomP, double sta, CDPoint vAng, long nRail)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(nRail >= pBri->GetQtyRailNode())	return;	//전체 레일 개소수 보다 클 경우

	CDPoint xyResult = CDPoint(0, 0);
	double disRail = pBri->GetLengthRailByLineByCenLine(sta, vAng, nRail);
	double disRailBetween = pBri->GetLineBase()->m_dRailwayGauge/2;

	BOOL	bMatch = pBri->IsMatchDosangByDis(sta, vAng, disRail - disRailBetween, xyResult);
	if(!bMatch)	return;
	
	double giulDosang = pBri->GetLineBase()->GetGiulCant(sta, disRail);
	CDomyun Dom(pDomP);
	DrawRailSymbolUnit(&Dom, TRUE);	//좌측 레일 그리기
	Dom.Rotate(atan(giulDosang), 0, 0);
	Dom.Move(xyResult);
	*pDomP << Dom;

	bMatch = pBri->IsMatchDosangByDis(sta, vAng, disRail + disRailBetween, xyResult);
	if(!bMatch)	return;
	Dom.ClearEtt(TRUE, FALSE);
	DrawRailSymbolUnit(&Dom, FALSE);	//우측 레일 그리기
	Dom.Rotate(atan(giulDosang), 0, 0);
	Dom.Move(xyResult);

	*pDomP << Dom;
}

//레일 궤도 심볼(형상) 1개만 그리는 함수
//CDomyun 으로 옮길 ㄳ
void CDrawCrossSection::DrawRailSymbolUnit(CDomyun *pDomP, BOOL bLeft)
{
	CDPoint xyBase = CDPoint(60.4668, 0);
	CDomyun Dom(pDomP);
	Dom.MoveTo(xyBase);
	Dom.LineTo(1.9045, 0);
	Dom.LineTo(0, 1.9045);
	Dom.LineTo(0, 9.8461);
	Dom.LineTo(1.7150, 11.7411);
	Dom.LineTo(16.5572, 13.2254);
	Dom.LineTo(21.1706, 14.2775);
	Dom.LineTo(42.4437, 22.0131);
	Dom.LineTo(51.8002, 34.2944);
	Dom.LineTo(53.0498, 88.5577);
	Dom.LineTo(44.1899, 104.4920);
	Dom.LineTo(53.0498, 88.5577);
	Dom.LineTo(44.1899, 104.4920);
	Dom.LineTo(42.2904, 105.6418);
	Dom.LineTo(30.8065, 109.8178);
	Dom.LineTo(29.5534, 111.6552);
	Dom.LineTo(30.0780, 132.6390);
	Dom.LineTo(40.4485, 144.5452);
	Dom.LineTo(49.9922, 145.4996);
	Dom.LineTo(60.4668, 145.6917);
	if(bLeft)
		Dom.Move(-91.3802, 0);
	else
		Dom.Move(-29.5534, 0);
	*pDomP << Dom;

	Dom.ClearEtt(TRUE, FALSE);
	Dom.MoveTo(xyBase);
	Dom.LineTo(1.9045, 0);
	Dom.LineTo(0, 1.9045);
	Dom.LineTo(0, 9.8461);
	Dom.LineTo(1.7150, 11.7411);
	Dom.LineTo(16.5572, 13.2254);
	Dom.LineTo(21.1706, 14.2775);
	Dom.LineTo(42.4437, 22.0131);
	Dom.LineTo(51.8002, 34.2944);
	Dom.LineTo(53.0498, 88.5577);
	Dom.LineTo(44.1899, 104.4920);
	Dom.LineTo(53.0498, 88.5577);
	Dom.LineTo(44.1899, 104.4920);
	Dom.LineTo(42.2904, 105.6418);
	Dom.LineTo(30.8065, 109.8178);
	Dom.LineTo(29.5534, 111.6552);
	Dom.LineTo(30.0780, 132.6390);
	Dom.LineTo(40.4485, 144.5452);
	Dom.LineTo(49.9922, 145.4996);
	Dom.LineTo(60.4668, 145.6917);
	Dom.Mirror(60.4668, TRUE);

	if(bLeft)
		Dom.Move(-91.3802, 0);
	else
		Dom.Move(-29.5534, 0);


	*pDomP << Dom;

}

// bDrawSlopPattern : 헌치부 외곽만 그릴지 패턴까지 그릴지?? added by jaeho 2004.10.11
// sta에 따라서 헌치를 그릴 수도 안그릴 수도 있음(그리고 헌치 높이가 변할 수도 있음) - modifyed by jaeho 2004.10.11
// 벽체,기둥 구조도를 그리다 보면, 헌치 높이를 LEVEL로 된 교량에서 교해야 되는 겨우가 있다.
// 이때는 벽체나, 기둥에 지정된 헌치 높이를 그대로 사용해서 그리기도 한다.(그래야 구조도가 맞음)
void CDrawCrossSection::DrawCrossSlabHunch(CDomyun* pDomP, double sta, CDPoint vAng, BOOL bDrawSlopPattern, long nDrawLeft, BOOL bMirror, BOOL bExp, CDPoint vAngWall, BOOL bCalHeightHunch, double dHeightHunch)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri->IsExp(sta))
		bExp	= FALSE;

	CTwinVectorArray tvArr;
	pBri->GetTvCrossHunch(tvArr, sta, vAng, bDrawSlopPattern, nDrawLeft, bMirror, bExp, vAngWall, bCalHeightHunch, dHeightHunch);

	CDomyun Dom(pDomP);

	m_pStd->DrawTvArray(&Dom, tvArr);

	*pDomP << Dom;
}

//슬래브면(교면)의 횡단 치수
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>

void CDrawCrossSection::DimCrossSection(CDomyun* pDomP, double sta, CDPoint vAng, BOOL bInput, BOOL bRoadNotSlab, BOOL bExp)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	long nQtyHDan = pBri->GetQtyHDan(bRoadNotSlab);
	CString str = _T(""), strLow = _T(""), strSub = _T("");
	CString str1 = _T(""), str2 = _T("");

	CDPoint A[30];
	pBri->GetXySlabUpperAct(sta, vAng, A,bRoadNotSlab, bExp);

	long nJijum = pBri->GetJijumNumberByStation(sta);
	double dTP = pBri->GetThickPave(bRoadNotSlab);
	double dTS = pBri->m_dTS;
	double dHeightUpperSlab	= pBri->GetHeightUpperSlab(nJijum, 0, vAng) - dTS;
	double dWidth = bExp ? pBri->GetWidthSlabActByGagak(sta, vAng, 0, bRoadNotSlab) : pBri->GetWidthSlabAct(sta, vAng,0,FALSE,FALSE,bRoadNotSlab);

	// 가각인 경우 직거리 사거리 개념이 애매한 관계로, 직거리 표현을 무조건 하지 않음
	// 현재 스테이션에 가각이 들어 있는지?
	BOOL bIsExpCurSta	= pBri->GetWidthSlabActByGagak(sta, vAng, 0, bRoadNotSlab) != pBri->GetWidthSlabAct(sta, vAng,0,FALSE,FALSE,bRoadNotSlab);
	
	CDPoint vAngSub = vAng==CDPoint(0, 1) ? pBri->GetAngleByStation(sta) : CDPoint(0, 1);
	
	//vAngSub = CDPoint(Round(vAngSub.x, 5), Round(vAngSub.y, 5));
	//double dSin = vAng==CDPoint(0, 1) ? 1/vAngSub.y : vAngSub.y;

	CDomyun Dom(pDomP);

	// 상부
// 	double dLen(0), dStrLen(0), dLenSub(0);
	BOOL bDimSub = FALSE;
	double dY = 0;
	if(pBri->m_bUnderGroundRahmen && !bRoadNotSlab)
	{
		dY = pBri->GetElMinSlabUpper(sta, vAng,bRoadNotSlab);
		Dom.SetDirection("BOTTOM");
	}
	else
	{
		dY = pBri->GetElMaxSlabUpper(sta, vAng,bRoadNotSlab);
		dY += pBri->GetMaxHeightCentralWall();
		Dom.SetDirection("TOP");
	}

	Dom.DimMoveTo(A[0].x, dY, 1);

	for(int n = 0; n<nQtyHDan; n++)
	{
		str.Format("L%d", n+1);
		double dLen = pBri->GetLengthHDanAct(sta, vAng, n,bRoadNotSlab, bExp);
		double dLenSub	= pBri->GetLengthHDanAct(sta, vAngSub, n,bRoadNotSlab, bExp);
		strSub = (Compare(vAng.GetAngleDegree(), 90.0, "=") && Compare(vAngSub.GetAngleDegree(), 90.0, "=")) ? _T("") : _T("(")+COMMA(dLenSub)+_T(")");
		double dStrLen = Dom.GetTextWidth(COMMA(dLen)+strSub);
		bDimSub = dStrLen>dLen;

		strLow = (bInput || (vAng==CDPoint(0, 1) && vAngSub==CDPoint(0, 1)) || bDimSub) ? COMMA(dLen) : COMMA(dLen)+strSub;
		if(bIsExpCurSta)
			strLow	= COMMA(dLen);

		str1 = bInput ? str : strLow;
		str2 = bInput ? strLow : (!bDimSub || bIsExpCurSta ? _T("") : strSub);
		Dom.DimLineToExtend(dLen, 1, str1, str2);
	}

	double dWidthSub = bExp ? pBri->GetWidthSlabActByGagak(sta, vAngSub, 0) : pBri->GetWidthSlabAct(sta, vAngSub,0,FALSE,FALSE,bRoadNotSlab);
	Dom.DimMoveTo(A[0].x, dY, 2);
	str = bInput || (vAng==CDPoint(0, 1) && vAngSub==CDPoint(0, 1)) ? COMMA(dWidth) : COMMA(dWidth)+_T("(")+COMMA(dWidthSub)+_T(")");
	if(bIsExpCurSta)
		str	= COMMA(dWidth);
	Dom.DimLineTo(dWidth, 2, str);
	*pDomP << Dom;

	if(!pBri->m_bUnderGroundRahmen && !bRoadNotSlab)
	{
		CHgBaseDrawStd baseDraw;
		CDPointArray xyArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		CDPoint xyStt	= CDPoint(A[0].x, A[0].y - dTS);
		CDPoint xyPos = xyStt;

		xyArrDim.Add(xyPos);

		xyPos.x += pBri->m_dCantilB[iLEFT];
		xyArrDim.Add(xyPos);

		xyPos.x += dWidth - (pBri->m_dCantilB[iLEFT] + pBri->m_dCantilB[iRIGHT]);
		xyArrDim.Add(xyPos);

		xyPos.x += pBri->m_dCantilB[iRIGHT];
		xyArrDim.Add(xyPos);
	
		baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, 0, "BOTTOM", -1, FALSE, FALSE, FALSE);
		*pDomP << Dom;
	}

	// 슬래브 연단 형상이 캔틸레버인 곳이 있으면 측면 치수를 넣어준다. 
	if(pBri->m_bCantilSlab[iLEFT])
	{
		CHgBaseDrawStd baseDraw;
		CDPointArray xyArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		CDPoint xyStt = CDPoint(A[0].x, A[0].y - dTS);
		CDPoint xyPos = xyStt;

		xyArrDim.Add(xyPos);

		xyPos.y += pBri->m_dCantilH1[iLEFT];
		xyArrDim.Add(xyPos);

		xyPos.y += pBri->m_dCantilH2[iLEFT];
		xyArrDim.Add(xyPos);

		if(dTS - (pBri->m_dCantilH1[iLEFT] + pBri->m_dCantilH2[iLEFT]) > 0)
		{
			xyPos.y += dTS - (pBri->m_dCantilH1[iLEFT] + pBri->m_dCantilH2[iLEFT]);
			xyArrDim.Add(xyPos);
		}

		baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, 0, _T("LEFT"), -1, FALSE, FALSE, FALSE);
		*pDomP << Dom;
	}

	if(pBri->m_bCantilSlab[iRIGHT])
	{
		CHgBaseDrawStd baseDraw;
		CDPointArray xyArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		CDPoint xyStt = CDPoint(A[0].x + dWidth, A[0].y - dTS);
		CDPoint xyPos = xyStt;

		xyArrDim.Add(xyPos);

		xyPos.y += pBri->m_dCantilH1[iRIGHT];
		xyArrDim.Add(xyPos);

		xyPos.y += pBri->m_dCantilH2[iRIGHT];
		xyArrDim.Add(xyPos);

		if(dTS - (pBri->m_dCantilH1[iRIGHT] + pBri->m_dCantilH2[iRIGHT]) > 0)
		{
			xyPos.y += dTS - (pBri->m_dCantilH1[iRIGHT] + pBri->m_dCantilH2[iRIGHT]);
			xyArrDim.Add(xyPos);
		}

		baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, 0, _T("RIGHT"), -1, FALSE, FALSE, FALSE);
		*pDomP << Dom;
	}


	// 횡단 두께
	if(!bRoadNotSlab)
	{
		long nCountRoad = 0;
		long nFirRoad = 0;
		long nSecRoad = 0;
		for(long nHDan=0; nHDan<nQtyHDan; nHDan++)
		{
			CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan,bRoadNotSlab);
			if(!pGW) continue;
			if(pGW->IsTypeHDanOnlyNone() || pGW->IsTypeHDanRailTotalDosang())
			{
				nCountRoad++;
				if(nCountRoad==1)
					nFirRoad = nHDan;
				if(nCountRoad==2)
					nSecRoad = nHDan;
			}
		}

		if(nCountRoad==0)
			return;

		double dFirLen = pBri->GetLengthHDanAct(sta, vAng, nFirRoad,bRoadNotSlab, bExp);
		double dSecLen = pBri->GetLengthHDanAct(sta, vAng, nSecRoad,bRoadNotSlab, bExp);
		CDPoint xyFir(0, 0), xySec(0, 0);
		if(nCountRoad==1)
		{
			xyFir.x = A[nFirRoad].x + dFirLen/4;
			xyFir.y = pBri->GetXySlabUpperActByDistAngle(sta, xyFir.x, vAng,bRoadNotSlab, bExp).y + dTP;
			xySec.x = A[nFirRoad].x + dFirLen*3/4;
			if(pBri->IsExtLine()==FALSE && pBri->m_dWidthSlabLeft==-pBri->m_dWidthSlabRight)
			{
				xySec.x = 0;
			}
			xySec.y = pBri->GetXySlabUpperActByDistAngle(sta, xySec.x, vAng,bRoadNotSlab, bExp).y - dTS - dHeightUpperSlab;
		}
		else
		{
			xyFir.x = A[nFirRoad].x + dFirLen/2 - Dom.Always(2);
			xyFir.y = pBri->GetXySlabUpperActByDistAngle(sta, xyFir.x, vAng,bRoadNotSlab, bExp).y + dTP;
			xySec.x = A[nSecRoad].x + dSecLen/2;
			if(pBri->IsExtLine()==FALSE && pBri->m_dWidthSlabLeft==-pBri->m_dWidthSlabRight)
			{
				xySec.x = 0;
			}
			xySec.y = pBri->GetXySlabUpperActByDistAngle(sta, xySec.x, vAng,bRoadNotSlab, bExp).y - dTS - dHeightUpperSlab;
		}

		if(bInput)
		{
			Dom.SetDirection("TOP");
			str.Format(_T("(T=%.fmm)"), dTP);
			CString strMat = pBri->m_nRoadMaterial==0 ? _T("ASCON 포장") : _T("LMC 포장");
			if(pBri->m_bUnderGroundRahmen)
			{
				strMat	= "몰탈";
				str.Format(_T("(T=%.fmm)"), pBri->m_dUnderGroundHeightMortar);
			}
			Dom.GiSiArrowExtend(xyFir.x, xyFir.y, Dom.Always(7), -Dom.Always(25), 3, strMat, str);

			Dom.SetDirection("RIGHT");
			Dom.SetDimExo(0);
			Dom.SetDimObq(20);
			Dom.DimMoveTo(xySec.x, xySec.y, 0);
			str = COMMA(dHeightUpperSlab);
			strLow = _T("");
			Dom.DimLineToExtend(dHeightUpperSlab, 0, str, strLow);
			str = _T("TS");
			strLow = COMMA(dTS);
			Dom.DimLineToExtend(dTS, 0, str, strLow);

			if(pBri->m_pARcBridgeDataStd->IsRailDesign() && pBri->m_dDisBaseElevationRL > 0)
			{
				//if(pBri->m_bUnderGroundRahmen)
				{
					xySec.y = pBri->GetElMaxSlabUpper(sta,vAng,TRUE);
					Dom.DimMoveTo(xySec.x, xySec.y, 0);
				}

				str = _T("TP");
				strLow = COMMA(dTP);
				Dom.DimLineToExtend(dTP, 0, str, strLow);

				str = _T("");
				strLow = COMMA(pBri->m_dDisBaseElevationRL - pBri->m_dHeightRail - dTP);
				Dom.DimLineToExtend(pBri->m_dDisBaseElevationRL - pBri->m_dHeightRail - dTP, 0, str, strLow);

				str = _T("HR");
				strLow = COMMA(pBri->m_dHeightRail);
				Dom.DimLineToOverExtend(pBri->m_dHeightRail, 0, str, strLow);

				Dom.SetDirection("LEFT");
				Dom.SetDimExe(0);
				Dom.SetDimObq(20);
				Dom.DimMoveTo(xySec.x, xySec.y, 0);

				str = _T("R.L");
				strLow = COMMA(pBri->m_dDisBaseElevationRL);
				Dom.DimLineToExtend(pBri->m_dDisBaseElevationRL, 0, str, strLow);
			}
			else
			{
				str = _T("TP");
				strLow = COMMA(dTP);
				Dom.DimLineToOverExtend(dTP, 0, str, strLow);
			}

			double eleSta = pBri->GetLineBase()->GetElevationPlanEl(sta);
			Dom.LineTo(-1000, eleSta, 1000, eleSta);
		}

		*pDomP << Dom;
	}

	if(bInput && pBri->m_dDisBaseElevation != 0)
	{
		double dY = 0;
		long i = 0; for(i = 0; i < nQtyHDan; i++)
		{
			if(i == 0)
				dY	= A[0].y;
			else
				dY	= min(A[i].y, dY);
		}
		dY -= (pBri->m_dTS + pBri->GetThickPave(bRoadNotSlab) + pBri->m_dHHS);
		CString str	= _T("");
		str.Format("F.L. 기준위치 = %.3fmm", pBri->m_dDisBaseElevation);
		CConcSymbol sym(m_pStd->m_pARoadOptionStd);
		sym.ArrowElevation(&Dom, str, "", CDPoint(pBri->m_dDisBaseElevation, dY), FALSE, FALSE);
		

		*pDomP << Dom;
	}
}

//상하행 분리 교량일 경우 상하행 합친 전체 횡단면도 치수 그림
void CDrawCrossSection::DimCrossSectionFullLeftRight(CDomyun* pDomP, CRcBridgeRebar* pBri1, CRcBridgeRebar* pBri2, long nDan)
{
	int nSeparBri1 = pBri1->m_nSeparBri;// 상하행 분리 형태(0:상하행 일체, 1:상행선, 2:하행선)
	int nSeparBri2 = pBri2->m_nSeparBri;// 상하행 분리 형태(0:상하행 일체, 1:상행선, 2:하행선)

	if((nSeparBri1 == 0 || nSeparBri2 == 0) || (nSeparBri1 == 1 && nSeparBri2 == 1) || (nSeparBri1 == 2 && nSeparBri2 == 2))
		return;
	double dDistL = 0;	//좌측교량 좌측
	double dDistR = 0;	//우측교량 우측
	double dDistLR = 0;	//좌측교량 우측
	double dDistRL = 0;	//우측교량 좌측
	double dL = 2000;			// 중심선 길이
	double sta = pBri1->GetStationBridgeMid();
	CDPoint vAng = CDPoint(0,1);
	double dY = MAX(pBri1->GetElMaxSlabUpper(sta, vAng), pBri2->GetElMaxSlabUpper(sta, vAng));
	if(nSeparBri1 == 1)
	{
		dDistL = pBri1->GetDistLineLeftOrRight(sta, vAng, TRUE) + pBri1->GetWidthSlabFrLineLeftOrRight(sta, vAng, TRUE);//좌측교량, 상행선
		dDistR = pBri2->GetDistLineLeftOrRight(sta, vAng, FALSE) + pBri2->GetWidthSlabFrLineLeftOrRight(sta, vAng, FALSE);//우측교량, 하행선
		dDistLR = pBri1->GetDistLineLeftOrRight(sta, vAng, TRUE) + pBri1->GetWidthSlabFrLineLeftOrRight(sta, vAng, FALSE);//좌측교량
		dDistRL = pBri2->GetDistLineLeftOrRight(sta, vAng, FALSE) + pBri2->GetWidthSlabFrLineLeftOrRight(sta, vAng, TRUE);//우측교량
	}
	else
	{
		dDistL = pBri2->GetDistLineLeftOrRight(sta, vAng, TRUE) + pBri2->GetWidthSlabFrLineLeftOrRight(sta, vAng, TRUE);//좌측교량, 상행선
		dDistR = pBri1->GetDistLineLeftOrRight(sta, vAng, FALSE) + pBri1->GetWidthSlabFrLineLeftOrRight(sta, vAng, FALSE);//우측교량, 하행선
		dDistLR = pBri2->GetDistLineLeftOrRight(sta, vAng, TRUE) + pBri2->GetWidthSlabFrLineLeftOrRight(sta, vAng, FALSE);//좌측교량
		dDistRL = pBri1->GetDistLineLeftOrRight(sta, vAng, FALSE) + pBri1->GetWidthSlabFrLineLeftOrRight(sta, vAng, TRUE);//우측교량
	}

	CDomyun Dom(pDomP);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.DimMoveTo(MIN(dDistL,dDistR), dY+dL/2, nDan);
	Dom.DimLineTo(fabs(dDistR-dDistL), nDan, COMMA(fabs(dDistR-dDistL)));

	nDan++;
	Dom.DimMoveTo(MIN(dDistLR,dDistRL), dY+dL/2, nDan);
	Dom.DimLineTo(fabs(dDistRL-dDistLR), nDan, COMMA(fabs(dDistRL-dDistLR)));

	*pDomP << Dom;
}

// BOOL bDimLowerNotch : 하면 치수를노치 중심까지 할지..
void CDrawCrossSection::DimCrossSectionSlabEnd(CDomyun* pDomP, BOOL bLeft, double sta, CDPoint vAng, BOOL bInput, BOOL bDimLowerNotch, BOOL bDimSlabThick)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	CDPoint A[30];
	pBri->GetXySlabUpperAct(sta, vAng, A);
	long nCount = pBri->GetQtyHDanAct(sta,vAng);

	int nLeft = bLeft ? 0 : 1;
	double dTS = pBri->m_dTS;
	int nChamfer = pBri->m_nSlabChemfer[nLeft];
	double dCH1 = pBri->m_dCantilH1[nLeft];
	double dCH2 = pBri->m_dCantilH2[nLeft];
	double dCB = pBri->m_dCantilB[nLeft];
	double dDH = pBri->m_dDH[nLeft];
	double dRE1 = pBri->m_dRE1[nLeft];
	double dRE2 = pBri->m_dRE2[nLeft];
	double dTCW1 = pBri->m_dTCW1[nLeft];
	double dTCH1 = pBri->m_dTCH1[nLeft];
	double dTCW2 = pBri->m_dTCW2[nLeft];
	double dTCH2 = pBri->m_dTCH2[nLeft];
	CDPoint xyStt = bLeft ? A[0] : A[nCount];

	CTwinVector tv;
	CTwinVectorArray tvArr;
	pBri->GetTvCrossSection(tvArr, sta, vAng, FALSE, TRUE, FALSE);
	int nSize1 = tvArr.GetSize();

	CDPointArray xyArr;
	pBri->GetPointArraySlabLowerAct(sta, vAng, xyArr);
	int nSize2 = xyArr.GetSize();

	// 모따기 및 라운드
	CDPoint xyPrev(0, 0), xyCur(0, 0), xyNext(0, 0);
	CDPoint xyCen(0, 0), xy1(0, 0), xy2(0, 0);
	CDPoint xyR1(0, 0), xyTemp(0, 0);
	int nGisi = bLeft ? 1 : 3;
	
	CString str = _T(""), strLow = _T("");
	CString str1 = _T(""), str2 = _T("");
	CString strDir = bLeft ? _T("좌측") : _T("우측");

	CDPoint xyL1(0, 0), xyU1(0, 0), xyL2(0, 0), xyU2(0, 0);
	int i=0; for(i=0; i<nSize1; i++)
	{
		tv = tvArr.GetAt(i);
		if(tv.m_sInfo==strDir+_T("슬래브모따기2"))
		{
			xyU1 = tv.m_v1;
			xyU2 = tv.m_v2;
		}
		if(tv.m_sInfo==strDir+_T("슬래브모따기1"))
		{
			xyL1 = tv.m_v1;
			xyL2 = tv.m_v2;
		}
	}

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	double dGisiLen = bLeft ? Dom.Always(15) : -Dom.Always(15);
	if(nChamfer<2)
	{
		if(dTCW1!=0 && dTCH1!=0)
		{
			xyR1 = (xyL1+xyL2)/2;
			str = _T("CW1 X CH1");
			strLow.Format(_T("%.f X %.f"), dTCW1, dTCH1);
			str1 = bInput ? str : strLow;
			str2 = bInput ? strLow : _T("");
			Dom.GiSiArrowExtend(xyR1.x, xyR1.y, Dom.Always(5), dGisiLen, nGisi, str1, str2);
		}
		if(dTCW2!=0 && dTCH2!=0)
		{
			xyR1 = (xyU1+xyU2)/2;
			str = _T("CW2 X CH2");
			strLow.Format(_T("%.f X %.f"), dTCW2, dTCH2);
			str1 = bInput ? str : strLow;
			str2 = bInput ? strLow : _T("");
			Dom.GiSiArrowExtend(xyR1.x, xyR1.y, Dom.Always(5), dGisiLen, nGisi, str1, str2);
		}
	}
	else
	{
		double radAng1 = 0, radAng2 = 0;
		if(dRE1!=0)
		{
			xyPrev = xyL2;
			xyCur = bLeft ? xyArr.GetAt(0) : xyArr.GetAt(nSize2-1);
			xyNext = xyL1;
			if(bLeft)
				GetInSideTangentCircle(xyCur, xyPrev, xyNext, dRE1, xyCen, xy1, xy2);
			else
				GetInSideTangentCircle(xyCur, xyNext, xyPrev, dRE1, xyCen, xy1, xy2);
			radAng1 = (xy1-xyCen).GetAngleRadian();
			radAng2 = (xy2-xyCen).GetAngleRadian();

			GetXyMatchLineAndArc(xyCen, (xyL1+xyL2)/2-xyCen, xyCen, dRE1, radAng1, radAng2, xyR1, xyTemp);
			str.Format(_T("R1 = %.f"), dRE1);
			Dom.GiSiArrowExt(xyR1.x, xyR1.y, Dom.Always(5), dGisiLen, nGisi, str);
		}
		if(dRE2!=0)
		{
			xyPrev = xyU2;
			xyCur = CDPoint(xyStt.x, xyStt.y-(dTS-dCH1-dCH2));
			xyNext = xyU1;
			if(bLeft)
				GetInSideTangentCircle(xyCur, xyPrev, xyNext, dRE2, xyCen, xy1, xy2);
			else
				GetInSideTangentCircle(xyCur, xyNext, xyPrev, dRE2, xyCen, xy1, xy2);
			radAng1 = (xy1-xyCen).GetAngleRadian();
			radAng2 = (xy2-xyCen).GetAngleRadian();

			GetXyMatchLineAndArc(xyCen, (xyU1+xyU2)/2-xyCen, xyCen, dRE2, radAng1, radAng2, xyR1, xyTemp);
			str.Format(_T("R2 = %.f"), dRE2);
			Dom.GiSiArrowExt(xyR1.x, xyR1.y, Dom.Always(5), dGisiLen, nGisi, str);
		}
	}

	// 하부
	Dom.SetDirection("BOTTOM");
	if(dCB!=0 || dDH!=0)
	{
		Dom.DimMoveTo(xyStt.x, xyStt.y-dTS, 0);
		if(bDimLowerNotch)
		{
			double dNotchW1	= pBri->m_dNotchWidth1;
			double dNotchW2 = pBri->m_dNotchWidth2;
			double dNotchW	= (dNotchW1+dNotchW2)/2;
			double dDist	= bLeft ? dRE1 + 50 + dNotchW/2 : dRE1 + 50 + dNotchW/2;
			str	= COMMA(dDist);
			Dom.DimLineTo(bLeft ? dDist : -dDist, 0, str); 
		}
		else
		{
			str = _T("B");
			strLow = COMMA(dCB);
			str1 = bInput ? str : strLow;
			str2 = bInput ? strLow : _T("");
			if(bLeft)
				Dom.DimLineToExtend(dCB, 0, str1, str2);
			else
				Dom.DimLineToExtend(-dCB, 0, str1, str2);
			str = _T("DH");
			strLow = COMMA(dDH);
			str1 = bInput ? str : strLow;
			str2 = bInput ? strLow : _T("");
			if(bLeft)
				Dom.DimLineToExtend(dDH, 0, str1, str2);
			else
				Dom.DimLineToExtend(-dDH, 0, str1, str2);
		}
	}

	// 측면
	if(bDimSlabThick)
	{
		Dom.SetDirection(bLeft ? "LEFT" : "RIGHT");
		Dom.DimMoveTo(xyStt.x, xyStt.y-dTS, 0);
		str = _T("H1");
		strLow = COMMA(dCH1);
		str1 = bInput ? str : strLow;
		str2 = bInput ? strLow : _T("");
		Dom.DimLineToExtend(dCH1, 0, str1, str2);
		str = _T("H2");
		strLow = COMMA(dCH2);
		str1 = bInput ? str : strLow;
		str2 = bInput ? strLow : _T("");
		Dom.DimLineToExtend(dCH2, 0, str1, str2);
		Dom.DimLineTo(dTS-dCH1-dCH2, 0, COMMA(dTS-dCH1-dCH2));
	}

	*pDomP << Dom;
}

void CDrawCrossSection::DimCrossSectionStationAndElevation(CDomyun* pDomP, HGDOUBLE dSta, HGBOOL bLeft, HGBOOL bRight, HGBOOL bCenter, HGBOOL bSta, CDPoint vAng)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	CDomyun Dom(pDomP);
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);

	CDPoint A[30];
	pBri->GetXySlabUpperAct(dSta, vAng, A, FALSE, FALSE);
	
	HGDOUBLE dTP = pBri->GetThickPave(FALSE);
	HGDOUBLE dX = Dom.Always(3);

	if (bLeft)
	{
		CDPoint xyLeft   = A[0];
		xyLeft.x   -= dX;
		CString strLeft   = m_pStd->GetStringEL(toM(xyLeft.y));

		Sym.ArrowElevation(&Dom, strLeft, _T(""), xyLeft);
		Dom.LineTo(xyLeft-CDPoint(Dom.Always(2), 0), xyLeft+CDPoint(Dom.Always(2), 0));
	}
	
	if (bRight)
	{
		const HGINT32 iCountHDan = pBri->GetQtyHDanAct(dSta,vAng);

		CDPoint xyRight  = A[iCountHDan];
		xyRight.x  += dX;
		CString strRight  = m_pStd->GetStringEL(toM(xyRight.y));

		Sym.ArrowElevation(&Dom, strRight, _T(""), xyRight, FALSE);
		Dom.LineTo(xyRight-CDPoint(Dom.Always(2), 0), xyRight+CDPoint(Dom.Always(2), 0));
	}
	
	if (bCenter || bSta)
	{
		CDPoint xyCenter = pBri->GetXySlabUpperActByDist(dSta, 0);
		xyCenter.y += dTP;
		CString strCenter;
		CString strSta;
		if (bSta)
		{
			strSta = CStationFormatTextSingleTon::GetInstance()->GetStringStation(dSta);
		}

		if (bCenter)
		{
			strCenter = m_pStd->GetStringEL(toM(xyCenter.y));
		}

		Sym.ArrowElevation(&Dom, strCenter, strSta, xyCenter);
	}

	*pDomP << Dom;
}

//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
void CDrawCrossSection::DimGuardWall(CDomyun* pDomP, CGuardWallRC* pGW, double dSta, BOOL bInput, BOOL bWriteLeft, BOOL bWriteRight, BOOL bRoadNotSlab)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	CDPoint vAng(0,1);

	CDomyun Dom(pDomP);
	m_pStd->ApplyDimSetting(pDomP, &Dom);

	pGW->DimGuardWallBase(&Dom, bInput, bWriteLeft, bWriteRight);
	CTwinVectorArray tvArr;
	CDPoint xyLeft;
	xyLeft = pBri->GetXySlabHDanLeft(dSta, vAng, pGW->m_nNumberHDan, bRoadNotSlab);
	double diffHeight = 0;
	if(pGW->IsTypeHDanRoadRailDike())
	{
		diffHeight = -(pGW->GetHeight()-150);
	}
	else
	{
		pBri->GetTvGuardWall(pGW, tvArr, dSta, vAng, FALSE, bRoadNotSlab);

		if(tvArr.GetAt(1).m_v1.y >= tvArr.GetAt(13).m_v2.y)
			diffHeight = tvArr.GetAt(1).m_v1.y - tvArr.GetAt(0).m_v1.y;
		else
			diffHeight = tvArr.GetAt(13).m_v2.y - tvArr.GetAt(0).m_v1.y;
	}

	Dom.Move(xyLeft.x, xyLeft.y+diffHeight);

	*pDomP << Dom;
}

void CDrawCrossSection::DrawRebarGuardFence(CDomyun* pDomP, long nHDan, BOOL bInclPave, BOOL bInclFootway, BOOL bInput)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	CRcBridgeRebar *pBriRebar = m_pStd->m_pRcBridgeRebar;
	
	CDPoint vAng(0,1);
	double sta = pBri->GetStationBridgeMid();
	CJijum* pJL = pBri->GetJijum(0);
	if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pJL->m_dLengthUgan[1];

	CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan);
	if(!pGW)	return;

	// 외형 Arr 그리기
	long nHDanAct = pBri->GetNumSlabNodeAct(sta, vAng, nHDan);
	double dLenHDan = pBri->GetLengthHDanAct(sta, vAng, nHDan);

	CTwinVectorArray tvArrRect;
	tvArrRect.RemoveAll();	
	CDPoint A[30];
	pBri->GetXySlabUpperAct(sta, vAng, A);
	pBri->GetTvCrossSection(tvArrRect, sta, vAng, FALSE, TRUE, TRUE, 0, FALSE, FALSE);
	CDRect rect = tvArrRect.GetRect();
	rect.left = A[nHDanAct].x - 800.0;
	rect.top += 100.0;
	rect.right = A[nHDanAct].x + dLenHDan + 800.0;
	rect.bottom -= 100.0;

	CDomyun Dom(pDomP);
	
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	m_pStd->DrawCuttingTvArray(&Dom, tvArrRect, rect);
	
	*pDomP << Dom;
	
	// 배근 Arr	
	CTwinVectorArray tvArr;
	CTwinVectorArray tvArrRebar;
	CDPointArray xyPointRebar;
	tvArr.RemoveAll();
	tvArrRebar.RemoveAll();	
	xyPointRebar.RemoveAll();

	pBri->GetTvGuardWall(pGW, tvArr, sta, vAng);
	pBriRebar->m_Util.GetTvRebarGuardWall(pGW, tvArr, tvArrRebar, xyPointRebar, Dom.GetScaleDim());	
	
	// 철근이 똑같은 방호벽이 있는지 검사
	BOOL bCheck = FALSE;

	// Arr 그리기
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	m_pStd->DrawTvArray(&Dom, tvArrRebar, m_pStd->IsViewVectorInfo());
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);
	m_pStd->DrawPointRebar(&Dom, xyPointRebar);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	DrawRebarMarkGuardFence(&Dom, pGW, tvArrRebar, xyPointRebar, bInput, bCheck);
	DimRebarGuardFence(&Dom, pGW, tvArr, bInput);

	*pDomP << Dom;
}

void CDrawCrossSection::DrawRebarMarkGuardFence(CDomyun* pDomP, CGuardWallRC *pGW, CTwinVectorArray& tvArrRebar, CDPointArray& xyPointRebar, BOOL bInput, BOOL bCheck)
{
	CDomyun Dom(pDomP);
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	
	double	dH1 = pGW->m_dH1;
	double	dH2 = pGW->m_dH2;
	double	dH3 = pGW->m_dH3;
	double	dW1 = pGW->m_dW1;
	double	dW2 = pGW->m_dW2;
	double	dW4 = pGW->m_dW4;
	double	dW5 = pGW->m_dW5;
	
	// 각 철근 개수
	//long nSize = tvArrRebar.GetSize();
	long nSideRebar = 2;
	if(dH1>0)	nSideRebar++;
	if(dH2>0)	nSideRebar++;
	if(dH3>0)	nSideRebar++;
	long nTopRebar = 3;
	long nRebar2 = 0;
	if((dH1>0 && dH2>0 && dH3>0) && ((dW1>0 && dW2>0) || (dW4>0 && dW5>0)))
		nRebar2 = 4;

	CTwinVector tvSideVert;
	tvSideVert.m_v1.x  = tvSideVert.m_v2.x = pGW->IsLeftGuardFence()? tvArrRebar.GetXyLeft().x : tvArrRebar.GetXyRight().x;
	tvSideVert.m_v1.y  = tvArrRebar.GetXyBottom().y;
	tvSideVert.m_v2.y  = tvArrRebar.GetXyTop().y;

	CDPointArray xyArrVerMat;
	for (long ix =0; ix < xyPointRebar.GetSize(); ix++)
	{
		CDPoint xyVal = xyPointRebar.GetAt(ix);
		double dGap = fabs(xyVal.x - tvSideVert.m_v1.x);
		if(dGap < 15)
			xyArrVerMat.Add(xyVal);
	}

	long nDotRebar = xyArrVerMat.GetSize();
	long nMarkDotRebar = (long)((pGW->m_dSC * 2) + 2);	// Mark 를 그릴 Dot Rebar_ 개수 (한묶음)

	BOOL bLeft = pGW->IsLeftGuardFence();
	CDPoint xy(0,0);
	CString sDia(_T("")), sMark(_T("")), sCTC(_T(""));
	CTwinVector tv;
	CDPoint vUnit(0,0);
	double dLength = 0;	
	double dMargin = 100;
	
	// VR_1
	CRebar* pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_1)? pGW->m_pArrRebar[GW_REBAR_VERT_1] : NULL;
	tv = tvArrRebar.GetAt(nSideRebar-1);
	vUnit = (tv.m_v2 - tv.m_v1).Unit();
	dLength = ~(tv.m_v2 - tv.m_v1) * 0.6;
	xy = tv.m_v1+(vUnit*dLength);
	sDia  = pGW->GetStringDia(pGW->m_dVR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dVR);
	sCTC.Format(_T("C.T.C %.0f"), pGW->m_dCTC);
	if(bLeft)
	{
		
		sMark = (bInput || pRB==NULL) ? _T("VR") : pRB->m_sMark;

		Sym.RebarMarkLineSide(&Dom, xy, sMark, sDia, bLeft, 0, TRUE, sCTC);
	}
	else
	{
		sMark = (bInput || pRB==NULL) ? _T("VR") : pRB->m_sMark;

		Sym.RebarMarkLineSide(&Dom, xy, sMark, sDia, !bLeft, 0, TRUE, sCTC);
	}
	
	xy.y -= 50;
	
	SRebarMarkInputData mark_input_data;
	SRebarMarkviewData mark_view_data;

	// VR_2
	if(pGW->m_nTypeRebar == 0)
	{
		pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_2)? pGW->m_pArrRebar[GW_REBAR_VERT_2] : NULL;

		tv = tvArrRebar.GetAt(nSideRebar+1);
		vUnit = (tv.m_v2 - tv.m_v1).Unit();
		dLength = ~(tv.m_v2 - tv.m_v1) * 0.5;
		xy = tv.m_v2-(vUnit*dLength);
		sDia  = pGW->GetStringDia(pGW->m_dVR);

		sMark = (bInput || pRB==NULL) ? _T("VR") : pRB->m_sMark;
		
		mark_input_data.mark = sMark;
		mark_input_data.dia = sDia;
		mark_input_data.position = xy;
		mark_input_data.ctc = pGW->m_dCTC;
		mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
		mark_input_data.height	= 0;
		mark_input_data.left_dir = bLeft;
		
		mark_view_data.count_mark = TRUE;
		mark_view_data.line_rebar = TRUE;
		mark_view_data.mark_pos = eMarkUpper;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}

	// VR_3
	pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_3)? pGW->m_pArrRebar[GW_REBAR_VERT_3] : NULL;

	tv = tvArrRebar.GetAt(nSideRebar+nTopRebar+nSideRebar-1);
	vUnit = (tv.m_v2 - tv.m_v1).Unit();
	dLength = ~(tv.m_v2 - tv.m_v1) * 0.6;
	xy = tv.m_v1+(vUnit*dLength);
	sDia  = pGW->GetStringDia(pGW->m_dVR);
	sCTC.Format(_T("C.T.C %.0f"), pGW->m_dCTC);

	sMark = (bInput || pRB==NULL) ? _T("VR") : pRB->m_sMark;
	
	mark_input_data.mark = sMark;
	mark_input_data.dia = sDia;
	mark_input_data.position = xy;
	mark_input_data.ctc = pGW->m_dCTC;
	mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
	mark_input_data.height	= 0;
	mark_input_data.left_dir = FALSE;//bLeft;
	mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

	mark_view_data.count_mark = TRUE;
	mark_view_data.line_rebar = TRUE;
	mark_view_data.mark_pos = eMarkSide;

	Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

	// VR_4 : 보강 철근
	if((dH1>0 && dH2>0 && dH3>0) && ((dW1>0 && dW2>0) || (dW4>0 && dW5>0)))
	{
		pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_4)? pGW->m_pArrRebar[GW_REBAR_VERT_4] : NULL;

		tv = tvArrRebar.GetAt(nSideRebar+nTopRebar+nSideRebar+nRebar2-1);
		vUnit = (tv.m_v2 - tv.m_v1).Unit();
		dLength = ~(tv.m_v1 - tv.m_v2) * 0.9;
		xy = tv.m_v1+(vUnit*dLength);
		sDia  = pGW->GetStringDia(pGW->m_dVR);
		sCTC.Format(_T("C.T.C %.0f"), pGW->m_dCTC);

		sMark = (bInput || pRB==NULL) ? _T("VR") : pRB->m_sMark;

		
		mark_input_data.mark = sMark;
		mark_input_data.dia = sDia;
		mark_input_data.position = xy;
		mark_input_data.ctc = pGW->m_dCTC;
		mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
		mark_input_data.height	= 0;
		mark_input_data.left_dir = !bLeft;
		mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;


		mark_view_data.count_mark = TRUE;
		mark_view_data.line_rebar = TRUE;
		mark_view_data.mark_pos = eMarkSide;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}


	// HR_3
	if(xyArrVerMat.GetSize() > 0)
	{
		pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_HORI_1)? pGW->m_pArrRebar[GW_REBAR_HORI_1] : NULL;

		CDPoint xyTarget = !bLeft ? CDPoint(tvArrRebar.GetXyRight().x + pGW->m_dL1, tvArrRebar.GetXyTop().y - dMargin)
			: CDPoint(tvArrRebar.GetXyLeft().x - pGW->m_dL1, tvArrRebar.GetXyTop().y - dMargin);
		double nMarkSu = nDotRebar-nMarkDotRebar >=0 ? nDotRebar-nMarkDotRebar : 0;

		long i=0; for(i=nDotRebar-1; i>nMarkSu-1; i--)
		{
			xy = xyArrVerMat.GetAt(i);
			Dom.MoveTo(xy);
			Dom.LineTo(xyTarget);
		}
		sDia  = pGW->GetStringDia(pGW->m_dHR);
		sMark = (bInput || pRB==NULL) ? _T("HR") : pRB->m_sMark;

		mark_input_data.mark = sMark;
		mark_input_data.dia = sDia;
		mark_input_data.position = xyTarget;
		mark_input_data.ctc = pGW->m_dFH;
		mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
		mark_input_data.height	= 0;
		mark_input_data.left_dir = bLeft;

		mark_view_data.count_mark = FALSE;
		mark_view_data.line_rebar = TRUE;
		mark_view_data.mark_pos = eMarkSide;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}
	

	// CR_6,7,8,9...
	CDPointArray xyArr[4];
	m_pStd->m_pRcBridgeRebar->m_Util.GetXyRebarCR6789OfGueardFence(pGW, xyArr[0], xyArr[1], xyArr[2], xyArr[3], tvArrRebar);

	sDia  = pGW->GetStringDia(pGW->m_dSR);
	long nIdx =6;
	for(long sr = 0; sr < 4; sr++)
	{
		CDPointArray *pXyArr	= &xyArr[sr];
		if(pXyArr->GetSize() == 0) continue;
		pXyArr->Sort(FALSE);

		for(long i = 0; i < pXyArr->GetSize(); i++)
		{
			Sym.RebarCountMarkOne(&Dom, pXyArr->GetAt(i));
		}

		CDPoint xyBottom	= pXyArr->GetAt(0);
		CDPoint xyTop		= pXyArr->GetAt(pXyArr->GetSize()-1);
	
		CTwinVector tvLine(xyBottom, xyTop);
		tvLine.m_v2 = tvLine.m_v2 + (tvLine.GetXyDir()*dMargin/2);
		xyTop = tvLine.m_v2;

		Dom.LineTo(tvLine.m_v1, tvLine.m_v2);

		pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_SHEAR_1+sr)? pGW->m_pArrRebar[GW_REBAR_SHEAR_1+sr] : NULL;

		/////////////////MakeGuardFenceRebarJewon()와 같이 수정
// 		switch(sr)
// 		{
// 			case 0: sMark	= bLeft ? "CL6" : "CR6"; break;
// 			case 1: sMark	= bLeft ? "CL7" : "CR7"; break;
// 			case 2: sMark	= bLeft ? "CL8" : "CR8"; break;
// 			case 3: sMark	= bLeft ? "CL9" : "CR9"; break;
// 		}
		
		CString sDirVal = bLeft? _T("CL") : _T("CR");
		sMark.Format(_T("%s%d"), sDirVal, nIdx);
		nIdx++;
		////////////////////////////////////////////////////////////

		if(!bInput && pRB)
			sMark = pRB->m_sMark;

		mark_input_data.mark = sMark;
		mark_input_data.dia = sDia;
		mark_input_data.position = xyTop;
		mark_input_data.ctc = pGW->m_dCTC;
		mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
		mark_input_data.height	= 300;
		mark_input_data.left_dir = !bLeft;

		mark_view_data.count_mark = FALSE;
		mark_view_data.line_rebar = TRUE;
		mark_view_data.mark_pos = eMarkSide;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}	
	*pDomP << Dom;
}

void CDrawCrossSection::DimRebarGuardFence(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArr, BOOL bInput)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	CDPoint vOrigin = pGW->IsLeftGuardFence() ? tvArr.GetAt(tvArr.GetSize()-1).m_v2 : tvArr.GetAt(1).m_v1;
	CDPoint vAng(0,1);
//	double sta = pBri->GetStationBridgeMid();
//	CJijum* pJL = pBri->GetJijum(0);
//	if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
//		sta += pJL->m_dLengthUgan[1];
	
	BOOL bRailFence=pGW->IsTypeHDanRailGuardFence();
	// 외형
	double dD  = pGW->m_dDistFrLeft;
	double dH1 = pGW->m_dH1;
	double dH2 = pGW->m_dH2;
	double dH3 = pGW->m_dH3;
	double dW1 = pGW->m_dW1;
	double dW2 = pGW->m_dW2;
	double dW4 = pGW->m_dW4;
	double dW5 = pGW->m_dW5;
	
	// 배근
	double ddc1 = pGW->m_ddc1;
	double ddc2 = pGW->m_ddc2;
	double ddc3 = pGW->m_ddc3;
	double dF1  = pGW->m_dF1;
	double dF2  = pGW->m_dF2;
	double dF3  = pGW->m_dF3;
	double dE   = pGW->m_dE;
	double dF   = pGW->m_dF;
	double dL1  = pGW->m_dL1;
	double dW3R = pGW->m_dW3;
	double heightTotal = pGW->GetHeight();
	double dW	= pGW->GetWidth();
	double dTopWidth    = dW3R-ddc2-ddc3;
	double dBottomWidth = dW-ddc2-ddc3;

	double dE_Height = heightTotal - dF2 - dF3 - ddc1;

	CString str = _T("");
	CDomyun Dom(pDomP);
	Dom.SetScaleDim(20);
		
	double dDimX = 0;
	long nDimDan = 0;

	if(pGW->IsLeftGuardFence())
	{
		// 외형
		nDimDan = 2;
		Dom.SetDirection("RIGHT");
		
		Dom.DimMoveTo(dDimX, 0, nDimDan);
		Dom.DimLineTo(dH1, nDimDan, COMMA(dH1));
		Dom.DimLineTo(dH2, nDimDan, COMMA(dH2));
		Dom.DimLineTo(dH3, nDimDan++, COMMA(dH3));

		//높이 2개 이상이 "0"보다 클 경우
		if((dH1 > 0 && dH2 > 0) || (dH2 > 0 && dH3 > 0) || (dH3 > 0 && dH1 > 0))
		{
			Dom.DimMoveTo(dDimX, 0, nDimDan);
			Dom.DimLineTo(dH1+dH2+dH3, nDimDan, COMMA(dH1+dH2+dH3));
		}
		
		// 측면
		nDimDan = pGW->IsTypeHDanRailDetectGravel() ? 5 : 4;
		Dom.SetDirection("LEFT");
		
		Dom.DimMoveTo(dDimX, -dF1, nDimDan);
		bInput ? Dom.DimLineToExtend(dF1, nDimDan, _T("F1"), COMMA(dF1)) : Dom.DimLineTo(dF1, nDimDan, COMMA(dF1));
		bInput ? Dom.DimLineToExtend(dF2, nDimDan, _T("F2"), COMMA(dF2)) : Dom.DimLineTo(dF2, nDimDan, COMMA(dF2));
		bInput ? Dom.DimLineToExtend(dF3, nDimDan, _T("F3"), COMMA(dF3)) : Dom.DimLineTo(dF3, nDimDan, COMMA(dF3));
		if(dE_Height > 0)
		{
			bInput ? str.Format("EA@%.0f=%.0f",dF,dE*dF) : str.Format("%.0f@%.0f=%.0f",dE,dF,dE*dF);
			Dom.DimLineTo(dE_Height, nDimDan, str);
		}
		bInput ? Dom.DimLineToOverExtend(ddc1, nDimDan, _T("DC1"), COMMA(ddc1)) : Dom.DimLineToOver(ddc1, nDimDan, COMMA(ddc1));
		
		Dom.DimMoveTo(dDimX, 0, nDimDan+1);
		Dom.DimLineTo(heightTotal, nDimDan+1, COMMA(heightTotal));

		// 상면    <-----| 왼쪽으로 감
		nDimDan = 1;
		Dom.SetDirection("TOP");
		
		Dom.DimMoveTo(dDimX, heightTotal, nDimDan);
		Dom.DimLineTo(-dW5, nDimDan, COMMA(dW5));
		Dom.DimLineTo(-dW4, nDimDan, COMMA(dW4));
		bInput ? Dom.DimLineToExtend(-ddc3, nDimDan, _T(""), COMMA(ddc3)) : Dom.DimLineTo(-ddc3, nDimDan, COMMA(ddc3));
		Dom.DimLineTo(-dTopWidth, nDimDan, COMMA(dTopWidth));
		bInput ? Dom.DimLineToExtend(-ddc2, nDimDan, _T(""), COMMA(ddc2)) : Dom.DimLineTo(-ddc2, nDimDan, COMMA(ddc2));
		Dom.DimLineToOver(-dD, nDimDan, COMMA(dD), 2);
		
		Dom.DimMoveTo(dDimX-dW5-dW4, heightTotal, nDimDan+1);
		Dom.DimLineTo(-(ddc3+dTopWidth+ddc2), nDimDan+1, COMMA(ddc3+dTopWidth+ddc2));
		 
		// 하면  <-----| 왼쪽으로 감
		nDimDan = 1;
		Dom.SetDirection("BOTTOM");
		
		if(dH1>0 && dH2>0)
		{
			Dom.DimMoveTo(dDimX-ddc3, 0, nDimDan);			
			bInput ? Dom.DimLineToExtend(dL1, nDimDan, _T("L1"), COMMA(dL1)) : Dom.DimLineTo(dL1, nDimDan, COMMA(dL1));
		}
		else
		{
			CTwinVector tvFind;
			long nFindIDx = -1;
			for (long ix =0; ix < tvArr.GetSize(); ix++)
			{
				tvFind = tvArr.GetAt(ix);
				double dGapX = fabs(tvFind.GetXyLeft().x - tvFind.GetXyRight().x);
				double dGapY = fabs(tvFind.GetXyTop().y - tvFind.GetXyBottom().y);

				if(dGapX >= ddc3 && dGapY > dF1)
				{
					nFindIDx = ix;
					break;
				}
			}

			Offset(tvFind.m_v1, tvFind.m_v2, ddc3);
			tvFind.m_v1 = tvFind.m_v1 + (tvFind.GetXyDir(TRUE)*1000);
			tvFind.m_v2 = tvFind.m_v2 + (tvFind.GetXyDir()*1000);
			
			CDPoint xyBaseBot = vOrigin;
			xyBaseBot.y -= dF1;

			CDPoint xyMat;
			BOOL bMat = GetXyMatchSegAndLine(tvFind.m_v1, tvFind.m_v2, xyBaseBot, CDPoint(1,0), xyMat);

			if(bMat)
			{
				double dFixedDimX = xyMat.x - vOrigin.x;

				Dom.DimMoveTo(dFixedDimX, 0, nDimDan);			
				bInput ? Dom.DimLineToExtend(dL1, nDimDan, _T("L1"), COMMA(dL1)) : Dom.DimLineTo(dL1, nDimDan, COMMA(dL1));
			}			
		}
		
		Dom.DimMoveTo(dDimX-dW+ddc2, 0, nDimDan);
		bInput ? Dom.DimLineToExtend(dL1, nDimDan, _T("L1"), COMMA(dL1)) : Dom.DimLineTo(dL1, nDimDan, COMMA(dL1));
		
		Dom.DimMoveTo(dDimX-ddc3, 0, nDimDan+2);
		bInput ? Dom.DimLineToOver(ddc3, nDimDan+2, _T("DC3")) : Dom.DimLineToOver(ddc3, nDimDan+2, COMMA(ddc3));
		Dom.DimMoveTo(dDimX-ddc3, 0, nDimDan+2);
		Dom.DimLineTo(-dBottomWidth, nDimDan+2, COMMA(dBottomWidth));
		bInput ? Dom.DimLineToOver(-ddc2, nDimDan+2, _T("DC2"), 2) : Dom.DimLineToOver(-ddc2, nDimDan+2, COMMA(ddc2), 2);

		Dom.DimMoveTo(dDimX, 0, nDimDan+3);
		Dom.DimLineTo(-(ddc3+dBottomWidth+ddc2), nDimDan+3, COMMA(ddc3+dBottomWidth+ddc2));
	}
	else
	{	
		// 외형
		nDimDan = 2;
		Dom.SetDirection("LEFT");
	
		Dom.DimMoveTo(dDimX, 0, nDimDan);
		Dom.DimLineTo(dH1, nDimDan, COMMA(dH1));
		Dom.DimLineTo(dH2, nDimDan, COMMA(dH2));
		Dom.DimLineTo(dH3, nDimDan, COMMA(dH3));

		//높이 2개 이상이 "0"보다 클 경우
		if((dH1 > 0 && dH2 > 0) || (dH2 > 0 && dH3 > 0) || (dH3 > 0 && dH1 > 0))
		{
			Dom.DimMoveTo(dDimX, 0, nDimDan+1);
			Dom.DimLineTo(dH1+dH2+dH3, nDimDan+1, COMMA(dH1+dH2+dH3));
		}
		
		// 측면
		nDimDan = pGW->IsTypeHDanRailDetectGravel() ? 5 : 4;
		Dom.SetDirection("RIGHT");
	
		Dom.DimMoveTo(dDimX, -dF1, nDimDan);
		bInput ? Dom.DimLineToExtend(dF1, nDimDan, _T("F1"), COMMA(dF1)) : Dom.DimLineTo(dF1, nDimDan, COMMA(dF1));
		bInput ? Dom.DimLineToExtend(dF2, nDimDan, _T("F2"), COMMA(dF2)) : Dom.DimLineTo(dF2, nDimDan, COMMA(dF2));
		bInput ? Dom.DimLineToExtend(dF3, nDimDan, _T("F3"), COMMA(dF3)) : Dom.DimLineTo(dF3, nDimDan, COMMA(dF3));
		if(dE_Height > 0)
		{
			bInput ? str.Format("EA@%.0f=%.0f",dF,dE*dF) : str.Format("%.0f@%.0f=%.0f",dE,dF,dE*dF);
			Dom.DimLineTo(dE_Height, nDimDan, str);
		}
		bInput ? Dom.DimLineToOverExtend(ddc1, nDimDan, _T("DC1"), COMMA(ddc1)) : Dom.DimLineToOver(ddc1, nDimDan, COMMA(ddc1));
		
		Dom.DimMoveTo(dDimX, 0, nDimDan+1);
		Dom.DimLineTo(heightTotal, nDimDan+1, COMMA(heightTotal));

		// 상면   |-----> 오른쪽으로 감
		nDimDan = 1;	
		double dDistOnRight = pBri->GetLengthHDan(pGW->m_nNumberHDan) - dW;	
		Dom.SetDirection("TOP");
	
		Dom.DimMoveTo(dDimX, heightTotal, nDimDan);
		Dom.DimLineTo(dW1, nDimDan, COMMA(dW1));
		Dom.DimLineTo(dW2, nDimDan, COMMA(dW2));
		bInput ? Dom.DimLineToExtend(ddc2, nDimDan, _T(""), COMMA(ddc2)) : Dom.DimLineTo(ddc2, nDimDan, COMMA(ddc2));
		Dom.DimLineTo(dTopWidth, nDimDan, COMMA(dTopWidth));
		bInput ? Dom.DimLineToExtend(ddc3, nDimDan, _T(""), COMMA(ddc3)) : Dom.DimLineTo(ddc3, nDimDan, COMMA(ddc3));
		Dom.DimLineToOver(dDistOnRight, nDimDan, COMMA(dDistOnRight));

		Dom.DimMoveTo(dDimX+dW1+dW2, heightTotal, nDimDan+1);
		Dom.DimLineTo(ddc2+dTopWidth+ddc3, nDimDan+1, COMMA(ddc2+dTopWidth+ddc3));
		
		// 하면   |-----> 오른쪽으로 감
		nDimDan = 1;
		Dom.SetDirection("BOTTOM");
	
		if(dH1>0 && dH2>0)
		{
			Dom.DimMoveTo(dDimX+ddc2, 0, nDimDan);			
			bInput ? Dom.DimLineToExtend(-dL1, nDimDan, _T("L1"), COMMA(dL1)) : Dom.DimLineTo(-dL1, nDimDan, COMMA(dL1));
		}
		else 
		{
			CTwinVector tvFind;
			for(long ix =0; ix < tvArr.GetSize(); ix++)
			{
				tvFind = tvArr.GetAt(ix);
				double dGapX = fabs(tvFind.GetXyLeft().x - tvFind.GetXyRight().x);
				double dGapY = fabs(tvFind.GetXyTop().y - tvFind.GetXyBottom().y);

				if(dGapX >= ddc3 && dGapY > dF1)
				{
					break;
				}
			}

			Offset(tvFind.m_v1, tvFind.m_v2, ddc3);
			tvFind.m_v1 = tvFind.m_v1 + (tvFind.GetXyDir(TRUE)*1000);
			tvFind.m_v2 = tvFind.m_v2 + (tvFind.GetXyDir()*1000);

			CDPoint xyBaseBot = vOrigin;
			xyBaseBot.y -= dF1;

			CDPoint xyMat;
			BOOL bMat = GetXyMatchSegAndLine(tvFind.m_v1, tvFind.m_v2, xyBaseBot, CDPoint(-1,0), xyMat);

			if(bMat)
			{
				double dFixedDimX = xyMat.x - vOrigin.x;

				Dom.DimMoveTo(dFixedDimX, 0, nDimDan);			
				bInput ? Dom.DimLineToExtend(-dL1, nDimDan, _T("L1"), COMMA(dL1)) : Dom.DimLineTo(-dL1, nDimDan, COMMA(dL1));
			}
		}
		
		Dom.DimMoveTo(dDimX+dW-ddc2, 0, nDimDan);
		bInput ? Dom.DimLineToExtend(-dL1, nDimDan, _T("L1"), COMMA(dL1)) : Dom.DimLineTo(-dL1, nDimDan, COMMA(dL1));

		Dom.DimMoveTo(dDimX+ddc2, 0, nDimDan+2);
		bInput ? Dom.DimLineToOver(-ddc2, nDimDan+2, _T("DC2"), 2) : Dom.DimLineToOver(-ddc2, nDimDan+2, COMMA(ddc2), 2);
		Dom.DimMoveTo(dDimX+ddc2, 0, nDimDan+2);
		Dom.DimLineTo(dBottomWidth, nDimDan+2, COMMA(dBottomWidth));
		bInput ? Dom.DimLineToOver(ddc3, nDimDan+2, _T("DC3")) : Dom.DimLineToOver(ddc3, nDimDan+2, COMMA(ddc3));
		
		Dom.DimMoveTo(dDimX, 0, nDimDan+3);
		Dom.DimLineTo(ddc2+dBottomWidth+ddc3, nDimDan+3, COMMA(ddc2+dBottomWidth+ddc3));
	}		
		
	// Dom Move
	Dom.Move(vOrigin.x, vOrigin.y);
	*pDomP << Dom;
}

void CDrawCrossSection::DrawRebarCenterGuard(CDomyun* pDomP, long nHDan, BOOL bInclPave, BOOL bInclFootway, BOOL bInput)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	CRcBridgeRebar *pBriRebar = m_pStd->m_pRcBridgeRebar;
	
	CDPoint vAng(0,1);
	double sta = pBri->GetStationBridgeMid();
	CJijum* pJL = pBri->GetJijum(0);
	if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pJL->m_dLengthUgan[1];

	CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan);
	if(!pGW)	return;

	// 외형 Arr 그리기
	long nHDanAct = pBri->GetNumSlabNodeAct(sta, vAng, nHDan);
	double dLenHDan = pBri->GetLengthHDanAct(sta, vAng, nHDan);

	CTwinVectorArray tvArrRect;
	tvArrRect.RemoveAll();	
	CDPoint A[30];
	pBri->GetXySlabUpperAct(sta, vAng, A);
	pBri->GetTvCrossSection(tvArrRect, sta, vAng, FALSE, TRUE, TRUE, 0, FALSE, FALSE);
	CDRect rect = tvArrRect.GetRect();
	rect.left = A[nHDanAct].x - 800.0;
	rect.top += 100.0;
	rect.right = A[nHDanAct].x + dLenHDan + 800.0;
	rect.bottom -= 100.0;

	CDomyun Dom(pDomP);
	
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	m_pStd->DrawCuttingTvArray(&Dom, tvArrRect, rect);
	
	*pDomP << Dom;
	
	// 배근 Arr	
	CTwinVectorArray tvArr;
	CTwinVectorArray tvArrRebar;
	CDPointArray xyPointRebar;
	tvArr.RemoveAll();
	tvArrRebar.RemoveAll();	
	xyPointRebar.RemoveAll();

	pBri->GetTvGuardWall(pGW, tvArr, sta, vAng);
	pBriRebar->m_Util.GetTvRebarGuardWall(pGW, tvArr, tvArrRebar, xyPointRebar, Dom.GetScaleDim());	
	
	// 철근이 똑같은 방호벽이 있는지 검사
	long nQtyHDan		= pBri->GetQtyHDan();
	CGuardWallRC* pCurrGW = pBri->GetGuardWallByHDan(nHDan);
	BOOL bCheck = FALSE;
	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		if(i!=nHDan)
		{
			CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i); 
			if(!pGW)	continue;
			if(pGW->IsTypeHDanRoadCentralWallFull())
			{
				if(*pCurrGW == *pGW)
					bCheck = TRUE;
			}
		}
	}

	// Arr 그리기
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	m_pStd->DrawTvArray(&Dom, tvArrRebar);
	
	if(xyPointRebar.GetSize()>0)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);
		m_pStd->DrawPointRebar(&Dom, xyPointRebar);
	}
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	if(pGW->m_bGWhalf)	// 일체형
	{
		DrawRebarMarkCenterGuard(&Dom, pGW, tvArrRebar, xyPointRebar, bInput, bCheck);
		DimRebarCenterGuard(&Dom, pGW, tvArr, bInput);
	}
	else
	{
		DrawRebarMarkCenterGuard2(&Dom, pGW, tvArrRebar, xyPointRebar, bInput, bCheck);
		DimRebarCenterGuard2(&Dom, pGW, tvArr, tvArrRebar, bInput);
	}
	*pDomP << Dom;
}

void CDrawCrossSection::DrawRebarMarkCenterGuard(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArrRebar, CDPointArray& xyPointRebar, BOOL bInput, BOOL bCheck)
{
	CDomyun Dom(pDomP);
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);

	double dLength = tvArrRebar.GetAt(0).GetLength();
	double dx = tvArrRebar.GetAt(0).m_v2.x;
	double dy = tvArrRebar.GetAt(0).m_v2.y;
	CString strUp(_T("")), strDn(_T(""));
	CString sDia  = pGW->GetStringDia(pGW->m_dVR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dVR);
	strUp.Format(_T("%s-%.0f"), sDia, dLength);
	strDn.Format(_T("C.T.C %.0f"), pGW->m_dCTC);
	if(bInput)
		Dom.GiSiArrowExtend(dx, dy, Dom.Always(20), Dom.Always(20), 1, strUp, strDn);
	else
	{
		// Sym.RebarMarkLineSide(pDomP, CDPoint(dx,dy-(dLength*0.2)), _T("A1"), sDia, FALSE, 0, TRUE, strDn);

		CRebar *pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_1)? pGW->m_pArrRebar[GW_REBAR_VERT_1] : NULL;

		SRebarMarkInputData mark_input_data;
		SRebarMarkviewData mark_view_data;

		mark_input_data.mark = _T("A1");
		mark_input_data.dia = sDia;
		mark_input_data.position = CDPoint(dx,dy-(dLength*0.2));
		mark_input_data.ctc = pGW->m_dCTC;
		mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
		mark_input_data.height	= 0;
		mark_input_data.left_dir = FALSE;
		mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : strDn;

		mark_view_data.count_mark = TRUE;
		mark_view_data.line_rebar = TRUE;
		mark_view_data.mark_pos = eMarkSide;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}
	
	*pDomP << Dom;
}

void CDrawCrossSection::DimRebarCenterGuard(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArr, BOOL bInput)
{
	CDPoint vOrigin = pGW->IsLeftGuardFence() ? tvArr.GetAt(tvArr.GetSize()-1).m_v2 : tvArr.GetAt(1).m_v1;
	
	// 외형
	double dH1 = pGW->m_dH1;
	double dH2 = pGW->m_dH2;
	double dH3 = pGW->m_dH3;
	double dW1 = pGW->m_dW1;
	double dW2 = pGW->m_dW2;
	double dW3 = pGW->m_dW3;
	double dW4 = pGW->m_dW4;
	double dW5 = pGW->m_dW5;
	
	// 배근
	double dF1  = pGW->m_dF1;
	double dF2  = pGW->m_dF2;
	double heightTotal = pGW->GetHeight();

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(20);
		
	double dDimX = 0;
	long nDimDan = 0;

	// 외형
	nDimDan = 3;
	Dom.SetDirection("LEFT");
	
	Dom.DimMoveTo(dDimX, 0, nDimDan);
	Dom.DimLineTo(dH1, nDimDan, COMMA(dH1));
	Dom.DimLineTo(dH2, nDimDan, COMMA(dH2));
	Dom.DimLineTo(dH3, nDimDan, COMMA(dH3));

	//높이 2개 이상이 "0"보다 클 경우
	if((dH1 > 0 && dH2 > 0) || (dH2 > 0 && dH3 > 0) || (dH3 > 0 && dH1 > 0))
	{
		Dom.DimMoveTo(dDimX, 0, nDimDan+1);
		Dom.DimLineTo(dH1+dH2+dH3, nDimDan+1, COMMA(dH1+dH2+dH3));
	}

	// 배근
	nDimDan = 0;
	Dom.SetDirection("RIGHT");
	
	Dom.DimMoveTo(dDimX, -dF1, nDimDan);
	bInput ? Dom.DimLineToExtend(dF1, nDimDan, _T("F1"), COMMA(dF1)) : Dom.DimLineTo(dF1, nDimDan, COMMA(dF1));
	bInput ? Dom.DimLineToExtend(dF2, nDimDan, _T("F2"), COMMA(dF2)) : Dom.DimLineTo(dF2, nDimDan, COMMA(dF2));
	Dom.DimMoveTo(dDimX, -dF1, ++nDimDan);
	Dom.DimLineTo(dF1+dF2, nDimDan, COMMA(dF1+dF2));

	// 상면    <-----| 왼쪽으로 감
	nDimDan = 0;
	Dom.SetDirection("TOP");
	
	Dom.DimMoveTo(dDimX, heightTotal, nDimDan);
	Dom.DimLineTo(-dW1, nDimDan, COMMA(dW1));
	Dom.DimLineTo(-dW2, nDimDan, COMMA(dW2));
	Dom.DimLineTo(-dW3, nDimDan, COMMA(dW3));
	Dom.DimLineTo(-dW4, nDimDan, COMMA(dW4));
	Dom.DimLineTo(-dW5, nDimDan, COMMA(dW5));
	Dom.DimMoveTo(-dDimX, heightTotal, ++nDimDan);
	Dom.DimLineTo(-(dW1+dW2+dW3+dW4+dW5), nDimDan, COMMA(dW1+dW2+dW3+dW4+dW5));

	// Dom Move
	Dom.Move(vOrigin.x, vOrigin.y);
	*pDomP << Dom;
}

void CDrawCrossSection::DrawRebarMarkCenterGuard2(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArrRebar, CDPointArray& xyPointRebar, BOOL bInput, BOOL bCheck)
{
	CDomyun Dom(pDomP);
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	
	// 각 철근 개수
	long nSize = tvArrRebar.GetSize();
	long nSideRebar = 2;
	long nTopRebar = 3;
	long nRebar2 = 4;
	long nHorRebar = nSize - nSideRebar - nSideRebar - nRebar2 - nRebar2 - nTopRebar;

	CDPoint xy(0,0);
	CString sDia(_T("")), sMark(_T("")), sCTC(_T(""));
	CTwinVector tv;
	CDPoint vUnit(0,0);
	double dLength = 0;	
	double dMargin = 100;
	
	// VR_1
	tv = tvArrRebar.GetAt(nSideRebar+nTopRebar);
	vUnit = (tv.m_v2 - tv.m_v1).Unit();
	dLength = ~(tv.m_v2 - tv.m_v1) * 0.7;
	xy = tv.m_v2-(vUnit*dLength);
	sDia  = pGW->GetStringDia(pGW->m_dVR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dVR);
	sMark = bInput ? _T("VR") : _T("A1");
	if(!bInput && bCheck)
		sMark = _T("A1");
	// Sym.RebarMarkLineSide(&Dom, xy, sMark, sDia, 0, 0, TRUE, sCTC);

	SRebarMarkInputData mark_input_data;
	SRebarMarkviewData mark_view_data;

	CRebar *pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_1)? pGW->m_pArrRebar[GW_REBAR_VERT_1] : NULL;
	mark_input_data.mark = sMark;
	mark_input_data.dia = sDia;
	mark_input_data.position = xy;
	mark_input_data.ctc = pGW->m_dCTC;
	mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
	mark_input_data.height	= 0;
	mark_input_data.left_dir = FALSE;
	mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

	mark_view_data.count_mark = TRUE;
	mark_view_data.line_rebar = TRUE;
	mark_view_data.mark_pos = eMarkSide;

	Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	
	// VR_2
	tv = tvArrRebar.GetAt(nSideRebar+1);
	vUnit = (tv.m_v2 - tv.m_v1).Unit();
	dLength = ~(tv.m_v2 - tv.m_v1) * 0.5;
	xy = tv.m_v2-(vUnit*dLength);
	sDia  = pGW->GetStringDia(pGW->m_dVR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dVR);
	sMark = bInput ? _T("VR") : _T("A2");
	if(!bInput && bCheck)
		sMark = _T("A2");

	// Sym.RebarMarkLineUpper(&Dom, xy, sMark, sDia, 0, 0, TRUE);
	pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_2)? pGW->m_pArrRebar[GW_REBAR_VERT_2] : NULL;
	mark_input_data.mark = sMark;
	mark_input_data.dia = sDia;
	mark_input_data.position = xy;
	mark_input_data.ctc = pGW->m_dCTC;
	mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
	mark_input_data.height	= 0;
	mark_input_data.left_dir = FALSE;
	mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

	mark_view_data.count_mark = TRUE;
	mark_view_data.line_rebar = TRUE;
	mark_view_data.mark_pos = eMarkUpper;

	Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

	// VR_3, 4 : 보강 Rebar
	long nRebar = nSideRebar+nTopRebar+nSideRebar+3;
	tv = tvArrRebar.GetAt(nRebar);
	vUnit = (tv.m_v2 - tv.m_v1).Unit();
	dLength = ~(tv.m_v2 - tv.m_v1) * 0.2;
	xy = tv.m_v1+(vUnit*dLength);
	sDia  = pGW->GetStringDia(pGW->m_dVR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dVR);
	sCTC.Format(_T("C.T.C %.0f"), pGW->m_dCTC);
	sMark = bInput ? _T("VR") : _T("A3");
	if(!bInput && bCheck)
		sMark = _T("A3");
	Sym.RebarCountMarkOne(&Dom, xy, CDPoint(1,0));
	// Sym.RebarMarkLineSide(&Dom, xy, sMark, sDia, 1, 0, FALSE, sCTC);
	pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_3)? pGW->m_pArrRebar[GW_REBAR_VERT_3] : NULL;
	mark_input_data.mark = sMark;
	mark_input_data.dia = sDia;
	mark_input_data.position = xy;
	mark_input_data.ctc = pGW->m_dCTC;
	mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
	mark_input_data.height	= 0;
	mark_input_data.left_dir = TRUE;
	mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

	mark_view_data.count_mark = FALSE;
	mark_view_data.line_rebar = TRUE;
	mark_view_data.mark_pos = eMarkUpper;

	Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	
	// HR_3
	if(xyPointRebar.GetSize() > 3)
	{
		CDPoint xy1 = xyPointRebar.GetAt(1);
		CDPoint xy2 = xyPointRebar.GetAt(3);
		CDPoint xyTarget = CDPoint(xy1.x+Dom.Always(1), xy2.y+Dom.Always(5));

		Dom.MoveTo(xy1);
		Dom.LineTo(xyTarget);
		
		Dom.MoveTo(xy2);
		Dom.LineTo(xyTarget);

		sDia  = pGW->GetStringDia(pGW->m_dHR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dHR);
		sMark = bInput ? _T("HR") : _T("A4");
		if(!bInput && bCheck)
			sMark = _T("A4");
		// Sym.RebarMarkLineSide(&Dom, xyTarget, sMark, sDia, 0, 0, FALSE);
		pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_HORI_1)? pGW->m_pArrRebar[GW_REBAR_HORI_1] : NULL;
		mark_input_data.mark = sMark;
		mark_input_data.dia = sDia;
		mark_input_data.position = xyTarget;
		mark_input_data.ctc = pGW->m_dFH;
		mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
		mark_input_data.height	= 0;
		mark_input_data.left_dir = FALSE;
		
		mark_view_data.count_mark = FALSE;
		mark_view_data.line_rebar = TRUE;
		mark_view_data.mark_pos = eMarkSide;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}

	// SR_4,5,6...
	long nMarkNo = 5;
	char ch[5];
	CDPointArray xyArr;
	xyArr.RemoveAll();
	
	long nGugan=0, nPrevGugan=0, nShearRebar=0;
	long nSttShearNo = nSize-nHorRebar;
	long i=0; for(i=nSttShearNo; i<nSize; i++)
	{
		tv = tvArrRebar.GetAt(i);
		vUnit = (tv.m_v2 - tv.m_v1).Unit();
		dLength = ~(tv.m_v1 - tv.m_v2) * 0.5;
		xy = tv.m_v1+(vUnit*dLength);
		sDia  = pGW->GetStringDia(pGW->m_dSR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dSR);

		// 구간
		double dOriginY = tvArrRebar.GetAt(1).m_v1.y + pGW->m_dF1;
		if(tv.m_v1.y < dOriginY + pGW->m_dCrossF2-0.5)
			nGugan = 0;
		else if(tv.m_v1.y >= (dOriginY+pGW->m_dCrossF2-0.5) && tv.m_v1.y <= (dOriginY+pGW->m_dCrossF2+pGW->m_dCrossF3+0.5))
			nGugan = 1;
		else
			nGugan = 2;

		if(i>nSttShearNo && nPrevGugan!=nGugan)
			_ITOT_S(nMarkNo++, ch, sizeof(ch), 10);

		CString str(_T(""));
		str.Format(_T("A%s"), ch); 
		sMark = bInput ? _T("SR") : str;
		if(!bInput && bCheck)
			sMark.Format(_T("A%s"), ch);
		
		// 구간에 따라 마크 그리기
		if(i==nSttShearNo || nPrevGugan==nGugan)
			xyArr.Add(xy);
		else
		{
			double dx=0, dy=0;
			for(long k=xyArr.GetSize()-1; k>=0; k--)
			{
				if(k==xyArr.GetSize()-1)
				{
					dx = xyArr.GetAt(k).x;
					dy = xyArr.GetAt(k).y;
					
					Dom.MoveTo(dx, dy);
					Dom.LineTo(dx, dy+(dMargin*0.5));
					// Sym.RebarMarkLineSide(&Dom, CDPoint(dx, dy+(dMargin*0.5)), sMark, sDia, 1, 0, FALSE);
					
					pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_SHEAR_1+nShearRebar)? pGW->m_pArrRebar[GW_REBAR_SHEAR_1+nShearRebar] : NULL;
					mark_input_data.mark = sMark;
					mark_input_data.dia = sDia;
					mark_input_data.position = CDPoint(dx, dy+(dMargin*0.5));
					mark_input_data.ctc = pGW->m_dCTC;
					mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
					mark_input_data.height	= 0;
					mark_input_data.left_dir = TRUE;
					
					mark_view_data.count_mark = FALSE;
					mark_view_data.line_rebar = TRUE;
					mark_view_data.mark_pos = eMarkSide;

					Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

					Dom.MoveTo(dx, dy);
					++nShearRebar;
				}
				dy = xyArr.GetAt(k).y;
				Sym.RebarCountMarkOne(&Dom, CDPoint(dx,dy));
				
				if(k==0)
					Dom.LineTo(xyArr.GetAt(xyArr.GetSize()-1).x, xyArr.GetAt(k).y);
			}
			
			xyArr.RemoveAll();
			xyArr.Add(xy);
		}
		if(i==nSize-1)
		{
			_ITOT_S(nMarkNo++, ch, sizeof(ch), 10);
			str.Format(_T("A%s"), ch); 
			sMark = bInput ? _T("SR") : str;
			if(!bInput && bCheck)
				sMark.Format(_T("A%s"), ch);

			double dx=0, dy=0;
			for(long k=xyArr.GetSize()-1; k>=0; k--)
			{
				if(k==xyArr.GetSize()-1)
				{
					dx = xyArr.GetAt(k).x;
					dy = xyArr.GetAt(k).y;
					
					Dom.MoveTo(dx, dy);
					Dom.LineTo(dx, dy+(dMargin*0.5));
					// Sym.RebarMarkLineSide(&Dom, CDPoint(dx, dy+(dMargin*0.5)), sMark, sDia, 1, 0, FALSE);

					pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_SHEAR_1+nShearRebar)? pGW->m_pArrRebar[GW_REBAR_SHEAR_1+nShearRebar] : NULL;
					mark_input_data.mark = sMark;
					mark_input_data.dia = sDia;
					mark_input_data.position = CDPoint(dx, dy+(dMargin*0.5));
					mark_input_data.ctc = pGW->m_dCTC;
					mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
					mark_input_data.height	= 0;
					mark_input_data.left_dir = TRUE;
					
					mark_view_data.count_mark = FALSE;
					mark_view_data.line_rebar = TRUE;
					mark_view_data.mark_pos = eMarkSide;

					Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

					Dom.MoveTo(dx, dy);
					++nShearRebar;
				}
				dy = xyArr.GetAt(k).y;
				Sym.RebarCountMarkOne(&Dom, CDPoint(dx,dy));
				if(k==0)
					Dom.LineTo(xyArr.GetAt(xyArr.GetSize()-1).x, xyArr.GetAt(k).y);
			}
			xyArr.RemoveAll();
		}
		nPrevGugan = nGugan;
	}

	*pDomP << Dom;
}

void CDrawCrossSection::DimRebarCenterGuard2(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArr, CTwinVectorArray& tvArrRebar, BOOL bInput)
{
	CDPoint vOrigin = tvArr.GetAt(1).m_v1;
	
	// 외형
	double dH1 = pGW->m_dH1;
	double dH2 = pGW->m_dH2;
	double dH3 = pGW->m_dH3;
	double dW1 = pGW->m_dW1;
	double dW2 = pGW->m_dW2;
	double dW3 = pGW->m_dW3;
	
	// 배근
	double ddc1 = pGW->m_ddc1;
	double ddc2 = pGW->m_ddc2;
	double ddc3 = pGW->m_ddc3;
	double dF1  = pGW->m_dF1;
	double dF2  = pGW->m_dF2;
	double dF3  = pGW->m_dF3;
	double dE   = pGW->m_dE;
	double dF   = pGW->m_dF;
	double dL1  = pGW->m_dL1;
	CTwinVectorArray tvArrG;
	pGW->GetTvGuardWallBase(tvArrG, FALSE, FALSE);
	CDRect rect	= tvArrG.GetRect();
	double heightTotal = rect.top - rect.bottom;//pGW->GetHeight();
	double dW	= pGW->GetWidth();
	double dTopWidth    = dW3-ddc2-ddc3;
	double dBottomWidth = dW-ddc2-ddc3;
	double dE_Height = heightTotal - dF2 - dF3 - ddc1;

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(20);
		
	CString str(_T(""));
	double dDimX = 0;
	long nDimDan = 0;

	// 외형
	nDimDan = 2;
	Dom.SetDirection("LEFT");
	
	Dom.DimMoveTo(dDimX, -dF1, nDimDan);
	bInput ? Dom.DimLineToExtend(dF1, nDimDan, _T("F1"), COMMA(dF1)) : Dom.DimLineTo(dF1, nDimDan, COMMA(dF1));
	bInput ? Dom.DimLineToExtend(dF2, nDimDan, _T("F2"), COMMA(dF2)) : Dom.DimLineTo(dF2, nDimDan, COMMA(dF2));
	bInput ? Dom.DimLineToExtend(dF3, nDimDan, _T("F3"), COMMA(dF3)) : Dom.DimLineTo(dF3, nDimDan, COMMA(dF3));
	bInput ? str.Format("EA@%.0f=%.0f",dF,dE*dF) : str.Format("%.0f@%.0f=%.0f",dE,dF,dE*dF);
	Dom.DimLineTo(dE_Height, nDimDan, str);
	bInput ? Dom.DimLineToOverExtend(ddc1, nDimDan, _T("DC1"), COMMA(ddc1)) : Dom.DimLineToOver(ddc1, nDimDan, COMMA(ddc1));
	
	Dom.DimMoveTo(dDimX, 0, nDimDan+1);
	Dom.DimLineTo(heightTotal, nDimDan+1, COMMA(heightTotal));

	nDimDan = 5;
	Dom.SetDirection("RIGHT");
	
	Dom.DimMoveTo(dDimX, 0, nDimDan);
	Dom.DimLineTo(dH1, nDimDan, COMMA(dH1));
	Dom.DimLineTo(dH2, nDimDan, COMMA(dH2));
	Dom.DimLineTo(dH3, nDimDan, COMMA(dH3));

	// 상면
	dDimX = dW1+dW2;
	nDimDan = 0;
	Dom.SetDirection("TOP");
	Dom.DimMoveTo(dDimX, heightTotal, nDimDan, COMMA(ddc2));
	Dom.DimLineTo(ddc2, nDimDan, COMMA(0));
	Dom.DimLineTo(dTopWidth, nDimDan, COMMA(dTopWidth));
	Dom.DimLineToOver(ddc3, nDimDan, COMMA(ddc3));
	
	Dom.DimMoveTo(dDimX, heightTotal, nDimDan+1);
	Dom.DimLineTo(dTopWidth+ddc2+ddc3, nDimDan+1, COMMA(dTopWidth+ddc2+ddc3));
	
	// 하면
	dDimX = 0;
	nDimDan = 1;
	Dom.SetDirection("BOTTOM");
	double dDist = fabs(tvArrRebar.GetAt(1).m_v1.x - vOrigin.x);
	Dom.DimMoveTo(dDimX+dDist, 0, nDimDan);
	bInput ? Dom.DimLineToExtend(-dL1, nDimDan, _T("L1"), COMMA(dL1)) : Dom.DimLineTo(-dL1, nDimDan, COMMA(dL1));
	Dom.DimMoveTo(dDimX+ddc2+dBottomWidth, 0, nDimDan);
	bInput ? Dom.DimLineToExtend(dL1, nDimDan, _T("L1"), COMMA(dL1)) : Dom.DimLineTo(dL1, nDimDan, COMMA(dL1));
	Dom.DimMoveTo(dDimX+ddc2, 0, nDimDan+2);
	bInput ? Dom.DimLineToOver(-ddc2, nDimDan+2, _T("DC2"), 2) : Dom.DimLineToOver(-ddc2, nDimDan+2, COMMA(ddc2), 2);
	Dom.DimMoveTo(dDimX+ddc2, 0, nDimDan+2);
	Dom.DimLineTo(dBottomWidth, nDimDan+2, COMMA(dBottomWidth));
	bInput ? Dom.DimLineToOver(ddc3, nDimDan+2, _T("DC3")) : Dom.DimLineToOver(ddc3, nDimDan+2, COMMA(ddc3));

	Dom.DimMoveTo(dDimX, 0, nDimDan+3);
	Dom.DimLineTo(dBottomWidth+ddc2+ddc3, nDimDan+3, COMMA(dBottomWidth+ddc2+ddc3));
	
	// Dom Move
	Dom.Move(vOrigin.x, vOrigin.y);
	*pDomP << Dom;
}

void CDrawCrossSection::DrawRebarCenterGuardHalf(CDomyun* pDomP, long nHDan, BOOL bInclPave, BOOL bInclFootway, BOOL bInput)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	CRcBridgeRebar *pBriRebar = m_pStd->m_pRcBridgeRebar;
	
	CDPoint vAng(0,1);
	double sta = pBri->GetStationBridgeMid();
	CJijum* pJL = pBri->GetJijum(0);
	if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pJL->m_dLengthUgan[1];

	CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan);
	if(!pGW)	return;

	// 외형 Arr 그리기
	long nHDanAct = pBri->GetNumSlabNodeAct(sta, vAng, nHDan);
	double dLenHDan = pBri->GetLengthHDanAct(sta, vAng, nHDan);

	CTwinVectorArray tvArrRect;
	tvArrRect.RemoveAll();	
	CDPoint A[30];
	pBri->GetXySlabUpperAct(sta, vAng, A);
	pBri->GetTvCrossSection(tvArrRect, sta, vAng, FALSE, TRUE, TRUE, 0, FALSE, FALSE);
	CDRect rect = tvArrRect.GetRect();
	rect.left = A[nHDanAct].x - 800.0;
	rect.top += 100.0;
	rect.right = A[nHDanAct].x + dLenHDan + 800.0;
	rect.bottom -= 100.0;

	CDomyun Dom(pDomP);
	
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	m_pStd->DrawCuttingTvArray(&Dom, tvArrRect, rect);
	
	*pDomP << Dom;
	
	// 배근 Arr	
	CTwinVectorArray tvArr;
	CTwinVectorArray tvArrRebar;
	CDPointArray xyPointRebar;
	tvArr.RemoveAll();
	tvArrRebar.RemoveAll();	
	xyPointRebar.RemoveAll();

	pBri->GetTvGuardWall(pGW, tvArr, sta, vAng);
	pBriRebar->m_Util.GetTvRebarGuardWall(pGW, tvArr, tvArrRebar, xyPointRebar, Dom.GetScaleDim());	
	
	// 철근이 똑같은 방호벽이 있는지 검사
	long nQtyHDan = pBri->GetQtyHDan();
	CGuardWallRC* pCurrGW = pBri->GetGuardWallByHDan(nHDan);
	BOOL bCheck = FALSE;
	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		if(i!=nHDan)
		{
			CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i); 
			if(!pGW)	continue;
			if(pGW->IsTypeHDanRoadCentralWallHalf())
			{
				if(*pCurrGW == *pGW)
					bCheck = TRUE;
			}
		}
	}

	// Arr 그리기
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	m_pStd->DrawTvArray(&Dom, tvArrRebar);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);
	m_pStd->DrawPointRebar(&Dom, xyPointRebar);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	DrawRebarMarkCenterGuardHalf(&Dom, pGW, tvArrRebar, xyPointRebar, bInput, bCheck);
	DimRebarCenterGuardHalf(&Dom, pGW, tvArr, bInput);

	*pDomP << Dom;
}

void CDrawCrossSection::DrawRebarMarkCenterGuardHalf(CDomyun* pDomP, CGuardWallRC *pGW, CTwinVectorArray& tvArrRebar, CDPointArray& xyPointRebar, BOOL bInput, BOOL bCheck)
{
	CDomyun Dom(pDomP);
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	
	// 각 철근 개수
	long nSize = tvArrRebar.GetSize();
	long nSideRebar = 3;
	long nRebar2 = 4;
	long nHorRebar = nSize - nSideRebar - nRebar2;

	BOOL bLeft = pGW->IsLeftGuardFence();
	CDPoint xy(0,0);
	CString sDia(_T("")), sMark(_T("")), sCTC(_T(""));
	CTwinVector tv;
	CDPoint vUnit(0,0);
	double dLength = 0;	
	double dMargin = 100;
	
	// VR_1
	tv = tvArrRebar.GetAt(0);
	vUnit = (tv.m_v1 - tv.m_v2).Unit();
	dLength = bLeft ? ~(tv.m_v1 - tv.m_v2) * 0.9 : ~(tv.m_v1 - tv.m_v2) * 0.78;
	xy = tv.m_v2+(vUnit*dLength);
	sDia  = pGW->GetStringDia(pGW->m_dVR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dVR);
	sCTC.Format(_T("C.T.C %.0f"), pGW->m_dCTC);
	if(bLeft)
	{
		sMark = bInput ? _T("VR") : _T("BL1");
		if(!bInput && bCheck)
			sMark = _T("B1");
		// Sym.RebarMarkLineSide(&Dom, xy, sMark, sDia, bLeft, 0, TRUE, sCTC);
	}
	else
	{
		sMark = bInput ? _T("VR") : _T("BR1");
		if(!bInput && bCheck)
			sMark = _T("B1");
		// Sym.RebarMarkLineSide(&Dom, xy, sMark, sDia, !bLeft, 0, TRUE, sCTC);
	}

	SRebarMarkInputData mark_input_data;
	SRebarMarkviewData mark_view_data;

	CRebar *pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_1)? pGW->m_pArrRebar[GW_REBAR_VERT_1] : NULL;
	mark_input_data.mark = sMark;
	mark_input_data.dia = sDia;
	mark_input_data.position = xy;
	mark_input_data.ctc = pGW->m_dCTC;
	mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
	mark_input_data.height	= 0;
	mark_input_data.left_dir = TRUE;
	mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

	mark_view_data.count_mark = TRUE;
	mark_view_data.line_rebar = TRUE;
	mark_view_data.mark_pos = eMarkSide;

	Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	
	// VR_2
	tv = bLeft ? tvArrRebar.GetAt(nSideRebar-1) : tvArrRebar.GetAt(nSideRebar-2);
	vUnit = (tv.m_v2 - tv.m_v1).Unit();
	dLength = bLeft ? ~(tv.m_v2 - tv.m_v1) * 0.78 : ~(tv.m_v2 - tv.m_v1) * 0.9;
	xy = tv.m_v2-(vUnit*dLength);
	sDia  = pGW->GetStringDia(pGW->m_dVR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dVR);
	if(bLeft)
	{
		sMark = bInput ? _T("VR") : _T("BL2");
		if(!bInput && bCheck)
			sMark = _T("B2");
		// Sym.RebarMarkLineSide(&Dom, xy, sMark, sDia, !bLeft, 0, TRUE, sCTC);
	}
	else
	{
		sMark = bInput ? _T("VR") : _T("BR2");
		if(!bInput && bCheck)
			sMark = _T("B2");
		// Sym.RebarMarkLineSide(&Dom, xy, sMark, sDia, bLeft, 0, TRUE, sCTC);
	}

	pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_2)? pGW->m_pArrRebar[GW_REBAR_VERT_2] : NULL;
	mark_input_data.mark = sMark;
	mark_input_data.dia = sDia;
	mark_input_data.position = xy;
	mark_input_data.ctc = pGW->m_dCTC;
	mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
	mark_input_data.height	= 0;
	mark_input_data.left_dir = FALSE;
	mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

	mark_view_data.count_mark = TRUE;
	mark_view_data.line_rebar = TRUE;
	mark_view_data.mark_pos = eMarkSide;

	Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	
	// VR_3
	tv = tvArrRebar.GetAt(nSideRebar+nRebar2-1);
	vUnit = (tv.m_v2 - tv.m_v1).Unit();
	dLength = ~(tv.m_v2 - tv.m_v1) * 0.4;
	xy = tv.m_v1+(vUnit*dLength);
	sDia  = pGW->GetStringDia(pGW->m_dVR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dVR);
	sCTC.Format(_T("C.T.C %.0f"), pGW->m_dCTC);
	if(bLeft)
	{
		sMark = bInput ? _T("VR") : _T("BL3");
		Sym.RebarCountMarkOne(&Dom, xy, CDPoint(0,1));
	}
	else
	{
		sMark = bInput ? _T("VR") : _T("BR3");
	}
	if(!bInput && bCheck)
		sMark = _T("B3");
	// Sym.RebarMarkLineSide(&Dom, xy, sMark, sDia, !bLeft, 0, TRUE, sCTC);
	pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_3)? pGW->m_pArrRebar[GW_REBAR_VERT_3] : NULL;
	mark_input_data.mark = sMark;
	mark_input_data.dia = sDia;
	mark_input_data.position = xy;
	mark_input_data.ctc = pGW->m_dCTC;
	mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
	mark_input_data.height	= 0;
	mark_input_data.left_dir = !bLeft;
	mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

	mark_view_data.count_mark = TRUE;
	mark_view_data.line_rebar = TRUE;
	mark_view_data.mark_pos = eMarkSide;

	Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

	// HR_3
	if(xyPointRebar.GetSize() > 3)
	{
		CDPoint xy1 = xyPointRebar.GetAt(1);
		CDPoint xy2 = xyPointRebar.GetAt(3);
		CDPoint xyTarget = bLeft ? CDPoint((xy1.x+xy2.x)*0.5 - dMargin, (xy1.y+xy2.y)*0.5)
								 : CDPoint((xy1.x+xy2.x)*0.5 + dMargin, (xy1.y+xy2.y)*0.5);

		Dom.MoveTo(xy1);
		Dom.LineTo(xyTarget);
		
		Dom.MoveTo(xy2);
		Dom.LineTo(xyTarget);

		sDia  = pGW->GetStringDia(pGW->m_dHR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dHR);
		if(bLeft)
			sMark = bInput ? _T("HR") : _T("BL4");
		else
			sMark = bInput ? _T("HR") : _T("BR4");
			
		if(!bInput && bCheck)
			sMark = _T("B4");
		
		// Sym.RebarMarkLineSide(&Dom, xyTarget, sMark, sDia, bLeft, 0, FALSE);
		pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_HORI_1)? pGW->m_pArrRebar[GW_REBAR_HORI_1] : NULL;
		mark_input_data.mark = sMark;
		mark_input_data.dia = sDia;
		mark_input_data.position = xyTarget;
		mark_input_data.ctc = pGW->m_dFH;
		mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
		mark_input_data.height	= 0;
		mark_input_data.left_dir = bLeft;

		mark_view_data.count_mark = FALSE;
		mark_view_data.line_rebar = TRUE;
		mark_view_data.mark_pos = eMarkSide;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}

	// SR_6,7,8...
	long nMarkNo = 5;
	char ch[5];
	CDPointArray xyArr;
	xyArr.RemoveAll();
	
	_ITOT_S(nMarkNo, ch, sizeof(ch), 10);
	long nGugan=0, nPrevGugan=0;
	long nSttShearNo = nSize-nHorRebar;
	long i=0; for(i=nSttShearNo; i<nSize; i++)
	{
		if(bLeft)
		{
			tv = tvArrRebar.GetAt(i);
			vUnit = (tv.m_v2 - tv.m_v1).Unit();
			dLength = ~(tv.m_v1 - tv.m_v2) * 0.5;
			xy = tv.m_v1+(vUnit*dLength);
		}
		else
		{
			tv = tvArrRebar.GetAt(i);
			vUnit = (tv.m_v1 - tv.m_v2).Unit();
			dLength = ~(tv.m_v1 - tv.m_v2) * 0.5;
			xy = tv.m_v2+(vUnit*dLength);
		}
		sDia  = pGW->GetStringDia(pGW->m_dSR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dSR);

		CString str(_T(""));
		if(bLeft)
			str.Format(_T("BL%s"), ch); 
		else
			str.Format(_T("BR%s"), ch); 
		
		sMark = bInput ? _T("SR") : str;
		if(!bInput && bCheck)
			sMark.Format(_T("B%s"), ch);
		
		// 구간에 따라 마크 그리기
		if(i==nSttShearNo || nPrevGugan==nGugan)
			xyArr.Add(xy);
		else
		{
			double dx=0, dy=0;
			for(long k=xyArr.GetSize()-1; k>=0; k--)
			{
				if(k==xyArr.GetSize()-1)
				{
					dx = xyArr.GetAt(k).x;
					dy = xyArr.GetAt(k).y;
					
					Dom.MoveTo(dx, dy);
					Dom.LineTo(dx, dy+(dMargin*0.5));
					// Sym.RebarMarkLineSide(&Dom, CDPoint(dx, dy+(dMargin*0.5)), sMark, sDia, bLeft, 0, FALSE);

					pRB = (pGW->m_pArrRebar.GetSize() > i)? pGW->m_pArrRebar[i] : NULL;
					mark_input_data.mark = sMark;
					mark_input_data.dia = sDia;
					mark_input_data.position = CDPoint(dx, dy+(dMargin*0.5));
					mark_input_data.ctc = pGW->m_dCTC;
					mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
					mark_input_data.height	= 0;
					mark_input_data.left_dir = bLeft;
					
					mark_view_data.count_mark = FALSE;
					mark_view_data.line_rebar = TRUE;
					mark_view_data.mark_pos = eMarkSide;

					Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

					Dom.MoveTo(dx, dy);
				}
				dy = xyArr.GetAt(k).y;
				Sym.RebarCountMarkOne(&Dom, CDPoint(dx,dy));
				
				if(k==0)
					Dom.LineTo(xyArr.GetAt(xyArr.GetSize()-1).x, xyArr.GetAt(k).y);
			}
			
			xyArr.RemoveAll();
			xyArr.Add(xy);
		}
		if(i==nSize-1)
		{
			double dx=0, dy=0;
			for(long k=xyArr.GetSize()-1; k>=0; k--)
			{
				if(k==xyArr.GetSize()-1)
				{
					dx = xyArr.GetAt(k).x;
					dy = xyArr.GetAt(k).y;
					
					Dom.MoveTo(dx, dy);
					Dom.LineTo(dx, dy+(dMargin*0.5));
					// Sym.RebarMarkLineSide(&Dom, CDPoint(dx, dy+(dMargin*0.5)), sMark, sDia, bLeft, 0, FALSE);
					pRB = (pGW->m_pArrRebar.GetSize() > i)? pGW->m_pArrRebar[i] : NULL;
					mark_input_data.mark = sMark;
					mark_input_data.dia = sDia;
					mark_input_data.position = CDPoint(dx, dy+(dMargin*0.5));
					mark_input_data.ctc = pGW->m_dCTC;
					mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
					mark_input_data.height	= 0;
					mark_input_data.left_dir = bLeft;
					mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

					mark_view_data.count_mark = FALSE;
					mark_view_data.line_rebar = TRUE;
					mark_view_data.mark_pos = eMarkSide;

					Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

					Dom.MoveTo(dx, dy);
				}
				dy = xyArr.GetAt(k).y;
				Sym.RebarCountMarkOne(&Dom, CDPoint(dx,dy));
				if(k==0)
					Dom.LineTo(xyArr.GetAt(xyArr.GetSize()-1).x, xyArr.GetAt(k).y);
			}
			xyArr.RemoveAll();
		}
		nPrevGugan = nGugan;
	}

	*pDomP << Dom;
}

void CDrawCrossSection::DimRebarCenterGuardHalf(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArr, BOOL bInput)
{
	CDPoint vOrigin = pGW->IsLeftGuardFence() ? tvArr.GetAt(tvArr.GetSize()-1).m_v2 : tvArr.GetAt(1).m_v1;
	
	// 외형
	double dH1 = pGW->m_dH1;
	double dH2 = pGW->m_dH2;
	double dH3 = pGW->m_dH3;
	double dW1 = pGW->m_dW1;
	double dW2 = pGW->m_dW2;
	double dW4 = pGW->m_dW4;
	double dW5 = pGW->m_dW5;
	
	// 배근
	double ddc1 = pGW->m_ddc1;
	double ddc2 = pGW->m_ddc2;
	double ddc3 = pGW->m_ddc3;
	double dF1  = pGW->m_dF1;
	double dF2  = pGW->m_dF2;
	double dF3  = pGW->m_dF3;
	double dE   = pGW->m_dE;
	double dF   = pGW->m_dF;
	double dL1  = pGW->m_dL1;
	double dW3R = pGW->m_dW3;
	double heightTotal = pGW->GetHeight();
	double dW	= pGW->GetWidth();

	double dTopWidth    = dW3R-ddc2-ddc3;
	double dBottomWidth = dW-ddc2-ddc3;

	double dE_Height = heightTotal - dF2 - dF3 - ddc1;

	CString str = _T("");
	CDomyun Dom(pDomP);
	Dom.SetScaleDim(20);
		
	double dDimX = 0;
	long nDimDan = 0;

	if(pGW->IsLeftGuardFence())
	{
		// 외형
		nDimDan = 1;
		Dom.SetDirection("RIGHT");
		
		Dom.DimMoveTo(dDimX, 0, nDimDan);
		str = COMMA(dH1);
		Dom.DimLineTo(dH1, nDimDan, str);
		str = COMMA(dH2);
		Dom.DimLineTo(dH2, nDimDan, str);
		str = COMMA(dH3);
		Dom.DimLineTo(dH3, nDimDan++, str);

		//높이 2개 이상이 "0"보다 클 경우
		if((dH1 > 0 && dH2 > 0) || (dH2 > 0 && dH3 > 0) || (dH3 > 0 && dH1 > 0))
		{
			Dom.DimMoveTo(dDimX, 0, nDimDan);
			Dom.DimLineTo(dH1+dH2+dH3, nDimDan, COMMA(dH1+dH2+dH3));
		}
		
		// 측면
		nDimDan = 4;
		Dom.SetDirection("LEFT");
		
		Dom.DimMoveTo(dDimX, -dF1, nDimDan);
		bInput ? Dom.DimLineToExtend(dF1, nDimDan, _T("F1"), COMMA(dF1)) : Dom.DimLineTo(dF1, nDimDan, COMMA(dF1));
		bInput ? Dom.DimLineToExtend(dF2, nDimDan, _T("F2"), COMMA(dF2)) : Dom.DimLineTo(dF2, nDimDan, COMMA(dF2));
		bInput ? Dom.DimLineToExtend(dF3, nDimDan, _T("F3"), COMMA(dF3)) : Dom.DimLineTo(dF3, nDimDan, COMMA(dF3));
		bInput ? str.Format("EA@%.0f=%.0f",dF,dE*dF) : str.Format("%.0f@%.0f=%.0f",dE,dF,dE*dF);
		Dom.DimLineTo(dE_Height, nDimDan, str);
		bInput ? Dom.DimLineToOverExtend(ddc1, nDimDan, _T("DC1"), COMMA(ddc1)) : Dom.DimLineToOver(ddc1, nDimDan, COMMA(ddc1));
		
		Dom.DimMoveTo(dDimX, 0, nDimDan+1);
		Dom.DimLineTo(heightTotal, nDimDan+1, COMMA(heightTotal));

		// 상면    <-----| 왼쪽으로 감
		nDimDan = 0;
		Dom.SetDirection("TOP");
		
		Dom.DimMoveTo(dDimX, heightTotal, nDimDan);
		Dom.DimLineTo(-dW5, nDimDan, COMMA(dW5));
		Dom.DimLineTo(-dW4, nDimDan, COMMA(dW4));
		Dom.DimLineToExtend(-ddc3, nDimDan, _T(""), COMMA(ddc3));
		Dom.DimLineTo(-dTopWidth, nDimDan, COMMA(dTopWidth));
		Dom.DimLineToExtend(-ddc2, nDimDan, _T(""), COMMA(ddc2));
		//Dom.DimLineToOver(-dD, nDimDan, COMMA(dD), 2);
		
		Dom.DimMoveTo(dDimX-dW5-dW4, heightTotal, nDimDan+1);
		Dom.DimLineTo(-(ddc3+dTopWidth+ddc2), nDimDan+1, COMMA(ddc3+dTopWidth+ddc2));

		// 하면  <-----| 왼쪽으로 감
		nDimDan = 0;
		Dom.SetDirection("BOTTOM");
		
		Dom.DimMoveTo(dDimX-ddc3, -pGW->m_dF1, nDimDan);
		bInput ? Dom.DimLineToExtend(dL1, nDimDan, _T("L1"), COMMA(dL1)) : Dom.DimLineTo(dL1, nDimDan, COMMA(dL1));
		//Dom.Rotate(-ToRadian(5), dDimX-ddc3, -pGW->m_dF1);
		
		Dom.DimMoveTo(dDimX-dW+ddc2, -pGW->m_dF1, nDimDan);
		bInput ? Dom.DimLineToExtend(dL1, nDimDan, _T("L1"), COMMA(dL1)) : Dom.DimLineTo(dL1, nDimDan, COMMA(dL1));
		//Dom.Rotate(-ToRadian(5), dDimX-dW+ddc2, -pGW->m_dF1);

		nDimDan = 1;
		Dom.DimMoveTo(dDimX-ddc3, 0, nDimDan+2);
		bInput ? Dom.DimLineToOver(ddc3, nDimDan+2, _T("DC3")) : Dom.DimLineTo(ddc3, nDimDan+2, COMMA(ddc3));
		Dom.DimMoveTo(dDimX-ddc3, 0, nDimDan+2);
		Dom.DimLineTo(-dBottomWidth, nDimDan+2, COMMA(dBottomWidth));
		bInput ? Dom.DimLineToOver(-ddc2, nDimDan+2, _T("DC2"), 2) : Dom.DimLineTo(-ddc2, nDimDan+2, COMMA(ddc2), 2);

		Dom.DimMoveTo(dDimX, 0, nDimDan+3);
		Dom.DimLineTo(-(ddc3+dBottomWidth+ddc2), nDimDan+3, COMMA(ddc3+dBottomWidth+ddc2));
	}
	else
	{	
		// 외형
		nDimDan = 1;
		Dom.SetDirection("LEFT");
	
		Dom.DimMoveTo(dDimX, 0, nDimDan);
		Dom.DimLineTo(dH1, nDimDan, COMMA(dH1));
		Dom.DimLineTo(dH2, nDimDan, COMMA(dH2));
		Dom.DimLineTo(dH3, nDimDan, COMMA(dH3));

		//높이 2개 이상이 "0"보다 클 경우
		if((dH1 > 0 && dH2 > 0) || (dH2 > 0 && dH3 > 0) || (dH3 > 0 && dH1 > 0))
		{
			Dom.DimMoveTo(dDimX, 0, nDimDan+1);
			Dom.DimLineTo(dH1+dH2+dH3, nDimDan+1, COMMA(dH1+dH2+dH3));
		}
		
		// 측면
		nDimDan = 4;
		Dom.SetDirection("RIGHT");
	
		Dom.DimMoveTo(dDimX, -dF1, nDimDan);
		bInput ? Dom.DimLineToExtend(dF1, nDimDan, _T("F1"), COMMA(dF1)) : Dom.DimLineTo(dF1, nDimDan, COMMA(dF1));
		bInput ? Dom.DimLineToExtend(dF2, nDimDan, _T("F2"), COMMA(dF2)) : Dom.DimLineTo(dF2, nDimDan, COMMA(dF2));
		bInput ? Dom.DimLineToExtend(dF3, nDimDan, _T("F3"), COMMA(dF3)) : Dom.DimLineTo(dF3, nDimDan, COMMA(dF3));
		bInput ? str.Format("EA@%.0f=%.0f",dF,dE*dF) : str.Format("%.0f@%.0f=%.0f",dE,dF,dE*dF);
		Dom.DimLineTo(dE_Height, nDimDan, str);
		bInput ? Dom.DimLineToOverExtend(ddc1, nDimDan, _T("DC1"), COMMA(ddc1)) : Dom.DimLineToOver(ddc1, nDimDan, COMMA(ddc1));
		
		Dom.DimMoveTo(dDimX, 0, nDimDan+1);
		Dom.DimLineTo(heightTotal, nDimDan+1, COMMA(heightTotal));

		// 상면   |-----> 오른쪽으로 감
		nDimDan = 0;	
		//double dDistOnRight = pBri->GetLengthHDan(pGW->m_nNumberHDan) - dW;	
		Dom.SetDirection("TOP");
	
		Dom.DimMoveTo(dDimX, heightTotal, nDimDan);
		Dom.DimLineTo(dW1, nDimDan, COMMA(dW1));
		Dom.DimLineTo(dW2, nDimDan, COMMA(dW2));
		Dom.DimLineToExtend(ddc2, nDimDan, _T(""), COMMA(ddc2));
		Dom.DimLineTo(dTopWidth, nDimDan, COMMA(dTopWidth));
		Dom.DimLineToExtend(ddc3, nDimDan, _T(""), COMMA(ddc3));
		//Dom.DimLineToOver(dDistOnRight, nDimDan, COMMA(dDistOnRight));

		Dom.DimMoveTo(dDimX+dW1+dW2, heightTotal, nDimDan+1);
		Dom.DimLineTo(ddc2+dTopWidth+ddc3, nDimDan+1, COMMA(ddc2+dTopWidth+ddc3));
		
		// 하면   |-----> 오른쪽으로 감
		nDimDan = 0;
		Dom.SetDirection("BOTTOM");
	
		Dom.DimMoveTo(dDimX+ddc2, -pGW->m_dF1, nDimDan);
		bInput ? Dom.DimLineToExtend(-dL1, nDimDan, _T("L1"), COMMA(dL1)) : Dom.DimLineTo(-dL1, nDimDan, COMMA(dL1));
		//Dom.Rotate(ToRadian(5), dDimX+ddc2, -pGW->m_dF1);

		Dom.DimMoveTo(dDimX+dW-ddc2, -pGW->m_dF1, nDimDan);
		bInput ? Dom.DimLineToExtend(-dL1, nDimDan, _T("L1"), COMMA(dL1)) : Dom.DimLineTo(-dL1, nDimDan, COMMA(dL1));
		//Dom.Rotate(ToRadian(5), dDimX+dW-ddc2, -pGW->m_dF1);

		nDimDan = 1;
		Dom.DimMoveTo(dDimX+ddc2, 0, nDimDan+2);
		bInput ? Dom.DimLineToOver(-ddc2, nDimDan+2, _T("DC2"), 2) : Dom.DimLineToOver(-ddc2, nDimDan+2, COMMA(ddc2), 2);
		Dom.DimMoveTo(dDimX+ddc2, 0, nDimDan+2);
		Dom.DimLineTo(dBottomWidth, nDimDan+2, COMMA(dBottomWidth));
		bInput ? Dom.DimLineToOver(ddc3, nDimDan+2, _T("DC3")) : Dom.DimLineToOver(ddc3, nDimDan+2, COMMA(ddc3));
		
		Dom.DimMoveTo(dDimX, 0, nDimDan+3);
		Dom.DimLineTo(ddc2+dBottomWidth+ddc3, nDimDan+3, COMMA(ddc2+dBottomWidth+ddc3));
	}	
	// Dom Move
	Dom.Move(vOrigin.x, vOrigin.y);
	*pDomP << Dom;	
}

void CDrawCrossSection::DrawRebarSoundProof(CDomyun* pDomP, long nHDan, BOOL bInclPave, BOOL bInclFootway, BOOL bInput)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	CRcBridgeRebar *pBriRebar = m_pStd->m_pRcBridgeRebar;
	
	CDPoint vAng(0,1);
	double sta = pBri->GetStationBridgeMid();
	CJijum* pJL = pBri->GetJijum(0);
	if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pJL->m_dLengthUgan[1];

	CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan);
	if(!pGW)	return;

	// 외형 Arr 그리기
	long nHDanAct = pBri->GetNumSlabNodeAct(sta, vAng, nHDan);
	double dLenHDan = pBri->GetLengthHDanAct(sta, vAng, nHDan);

	CTwinVectorArray tvArrRect;
	tvArrRect.RemoveAll();	
	CDPoint A[30];
	pBri->GetXySlabUpperAct(sta, vAng, A);
	pBri->GetTvCrossSection(tvArrRect, sta, vAng, FALSE, TRUE, TRUE, 0, FALSE, FALSE);
	CDRect rect = tvArrRect.GetRect();
	rect.left = A[nHDanAct].x - 800.0;
	rect.top += 100.0;
	rect.right = A[nHDanAct].x + dLenHDan + 800.0;
	rect.bottom -= 100.0;

	CDomyun Dom(pDomP);
	
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	m_pStd->DrawCuttingTvArray(&Dom, tvArrRect, rect);
	
	*pDomP << Dom;
	
	// 배근 Arr	
	CTwinVectorArray tvArr;
	CTwinVectorArray tvArrRebar;
	CDPointArray xyPointRebar;
	tvArr.RemoveAll();
	tvArrRebar.RemoveAll();	
	xyPointRebar.RemoveAll();

	pBri->GetTvGuardWall(pGW, tvArr, sta, vAng);
	pBriRebar->m_Util.GetTvRebarGuardWall(pGW, tvArr, tvArrRebar, xyPointRebar, Dom.GetScaleDim());	
	
	// 철근이 똑같은 방호벽이 있는지 검사
	long nQtyHDan		= pBri->GetQtyHDan();
	CGuardWallRC* pCurrGW = pBri->GetGuardWallByHDan(nHDan);
	BOOL bCheck = FALSE;
	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		if(i!=nHDan)
		{
			CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i);
			if(!pGW)	continue;
			if(pGW->IsTypeHDanRoadSoundProof() || pGW->m_bSoundProof)
			{
				if(*pCurrGW == *pGW)
					bCheck = TRUE;
			}
		}
	}

	// Arr 그리기
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	m_pStd->DrawTvArray(&Dom, tvArrRebar);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);
	m_pStd->DrawPointRebar(&Dom, xyPointRebar);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	DrawRebarMarkSoundProof(&Dom, pGW, tvArrRebar, xyPointRebar, bInput, bCheck);
	DimRebarSoundProof(&Dom, pGW, tvArr, bInput);

	*pDomP << Dom;
}

void CDrawCrossSection::DrawRebarMarkSoundProof(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArrRebar, CDPointArray& xyPointRebar, BOOL bInput, BOOL bCheck)
{
	if(tvArrRebar.GetSize() < 4) return;
	CDomyun Dom(pDomP);
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);

	BOOL bLeft = pGW->IsLeftGuardFence();
	CDPoint xy(0,0);
	CString sDia(_T("")), sMark(_T("")), sCTC(_T(""));
	CTwinVector tv;
	CDPoint vUnit(0,0);
	double dLength = 0;	
	double dMargin = 50;
	long nSize = tvArrRebar.GetSize();
	
	// VR_1
	CRebar* pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_1)? pGW->m_pArrRebar[GW_REBAR_VERT_1] : NULL;

	tv = tvArrRebar.GetAt(3);
	vUnit = (tv.m_v1 - tv.m_v2).Unit();
	dLength = ~(tv.m_v1 - tv.m_v2) * 0.5;
	xy = tv.m_v2+(vUnit*dLength);
	sDia  = pGW->GetStringDia(pGW->m_dVR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dVR);
	sCTC.Format(_T("C.T.C %.0f"), pGW->m_dCTC);

	if(bInput || pRB == NULL)
		sMark = _T("VR");
	else
		sMark = pRB->m_sMark;

	// Sym.RebarMarkLineSide(&Dom, xy, sMark, sDia, !bLeft, 0, TRUE, sCTC);
	SRebarMarkInputData mark_input_data;
	SRebarMarkviewData mark_view_data;

	mark_input_data.mark = sMark;
	mark_input_data.dia = sDia;
	mark_input_data.position = xy;
	mark_input_data.ctc = pGW->m_dCTC;
	mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
	mark_input_data.height	= 0;
	mark_input_data.left_dir = !bLeft;
	mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

	mark_view_data.count_mark = TRUE;
	mark_view_data.line_rebar = TRUE;
	mark_view_data.mark_pos = eMarkSide;

	Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

	// HR_1
	pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_HORI_1)? pGW->m_pArrRebar[GW_REBAR_HORI_1] : NULL;

	CDPoint xyTarget = bLeft ? CDPoint(tvArrRebar.GetXyRight().x, tvArrRebar.GetXyTop().y - dMargin)
							 : CDPoint(tvArrRebar.GetXyLeft().x, tvArrRebar.GetXyTop().y - dMargin);

	nSize	= xyPointRebar.GetSize();
	if(nSize > 1)
	{
		long nIdxPrev	= 0;
		long nIdxNext	= 1;
		if(nSize > 3)
		{
			nIdxPrev	= 1;
			nIdxNext	= 3;
		}
		sDia  = pGW->GetStringDia(pGW->m_dHR);
		xy = xyPointRebar.GetAt(nIdxPrev);
		Dom.MoveTo(xy);
		Dom.LineTo(xyTarget);
		xy = xyPointRebar.GetAt(nIdxNext);
		Dom.MoveTo(xy);
		Dom.LineTo(xyTarget);

		if(bInput || pRB == NULL)
			sMark = _T("HR");
		else
			sMark = pRB->m_sMark;

		// Sym.RebarMarkLineSide(&Dom, xyTarget, sMark, sDia, !bLeft, 0, FALSE);

		pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_HORI_1)? pGW->m_pArrRebar[GW_REBAR_HORI_1] : NULL;
		mark_input_data.mark = sMark;
		mark_input_data.dia = sDia;
		mark_input_data.position = xyTarget;
		mark_input_data.ctc = pGW->m_dFH;
		mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
		mark_input_data.height	= 0;
		mark_input_data.left_dir = !bLeft;
		
		mark_view_data.count_mark = FALSE;
		mark_view_data.line_rebar = TRUE;
		mark_view_data.mark_pos = eMarkSide;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}

	// SR_1
	pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_SHEAR_1)? pGW->m_pArrRebar[GW_REBAR_SHEAR_1] : NULL;

	nSize = tvArrRebar.GetSize();
	long nS = (long)(pGW->m_nSGugan/pGW->m_dSC);
	tv = tvArrRebar.GetAt(nSize-1);
	vUnit = (tv.m_v1 - tv.m_v2).Unit();
	dLength = ~(tv.m_v1 - tv.m_v2) * 0.5;
	xy = tv.m_v2+(vUnit*dLength);
	sDia  = pGW->GetStringDia(pGW->m_dSR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dSR);
	sCTC.Format(_T("C.T.C %.0f"), pGW->m_dCTC * pGW->m_dEC);

	if(bInput || pRB == NULL)
		sMark = _T("SR");
	else
		sMark = pRB->m_sMark;
	
	CDPoint xyLast(0,0);
	long i=0; for(i=0; i<nS; i++)
	{
		tv = tvArrRebar.GetAt(nSize-1-i);
		if(pGW->m_nS[0] != 0)
			Sym.RebarCountMarkOne(&Dom, CDPoint(xy.x, tv.m_v2.y));
		if(i==nS-1)
			xyLast = CDPoint(xy.x, tv.m_v2.y);
	}
	if(nS>1 && pGW->m_nS[0] != 0)
		Dom.LineTo(xy, xyLast);
	
	// 32804 
	//구간의 숫자 첫번째가 존재하지 않는다면 이 철근은 존재하기 않기 때문에 마킹해 주지 않는다
	if(pGW->m_nS[0] != 0) 
	{
		// Sym.RebarMarkLineLower(&Dom, xy, sMark, sDia, bLeft, Dom.Always(12), FALSE, FALSE, sCTC);

		mark_input_data.mark = sMark;
		mark_input_data.dia = sDia;
		mark_input_data.position = xy;
		mark_input_data.ctc = pGW->m_dCTC;
		mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
		mark_input_data.height	= Dom.Always(12);
		mark_input_data.left_dir = bLeft;
		mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

		mark_view_data.count_mark = FALSE;
		mark_view_data.line_rebar = TRUE;
		mark_view_data.mark_pos = eMarkLower;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}

	*pDomP << Dom;
}

void CDrawCrossSection::DimRebarSoundProof(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArr, BOOL bInput)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	CDPoint vOrigin = pGW->IsLeftGuardFence() ? tvArr.GetAt(tvArr.GetSize()-1).m_v2 : tvArr.GetAt(1).m_v1;
	
	// 외형
	double dD  = pGW->m_dDistFrLeft;
	
	// 배근
	long nUGugan = pGW->m_nUGugan;
	long nSGugan = pGW->m_nSGugan;
	long nU[MAX_REBAR_GUGAN];
	long nS[MAX_REBAR_GUGAN];
	double dU[MAX_REBAR_GUGAN];
	double dS[MAX_REBAR_GUGAN];
	double dF1  = pGW->m_dF1;
	double dL1  = pGW->m_dL1;
	double ddc1 = pGW->m_ddc1;
	double ddc2 = pGW->m_ddc2;
	double ddc3 = pGW->m_ddc3;
	double heightTotal = pGW->GetHeight();
	double dW	= pGW->GetWidth();
	long i=0; for(i=0; i<nUGugan; i++)
	{
		nU[i] = pGW->m_nU[i];
		dU[i] = pGW->m_dU[i];
	}
	for(i=0; i<nSGugan; i++)
	{
		nS[i] = pGW->m_nS[i];
		dS[i] = pGW->m_dS[i];
	}

	double dBottomWidth = dW-ddc2-ddc3;

	CString str = _T("");
	CDomyun Dom(pDomP);
	//Dom.SetScaleDim(20);
		
	double dDimX = 0;
	long nDimDan = 0;

	if(pGW->IsLeftGuardFence())
	{
		// 측면
		nDimDan = 3;
		Dom.SetDirection("LEFT");
		
		Dom.DimMoveTo(dDimX-dD, heightTotal-ddc1, nDimDan);
		bInput ? Dom.DimLineToOverExtend(ddc1, nDimDan, _T("DC1"), COMMA(ddc1)) : Dom.DimLineToOver(ddc1, nDimDan, COMMA(ddc1));
		Dom.DimMoveTo(dDimX-dD, heightTotal-ddc1, nDimDan);
		double dist = ddc1;
		long i=0; for(i=0; i<nSGugan; i++)
		{
			CString str1(_T(""));
			CString str2(_T(""));
			str1.Format(_T("S%d"), i+1);
			if(nS[i]>1)
				str2.Format(_T("%d@%.f=%.f"), nS[i], dS[i], dS[i]*nS[i]);
			else
				str2 = COMMA(dS[i]);
			bInput ? Dom.DimLineToExtend(-dS[i]*nS[i], nDimDan, str1, str2) : Dom.DimLineTo(-dS[i]*nS[i], nDimDan, COMMA(dS[i]*nS[i]));
			dist +=  dS[i]*nS[i];
		}
		double dGap1 = heightTotal - dist;
		double dGap2 = dF1;
		bInput ? Dom.DimLineToExtend(-dGap1, nDimDan, _T(""), COMMA(dGap1)) : Dom.DimLineTo(-dGap1, nDimDan, COMMA(dGap1));
		bInput ? Dom.DimLineToExtend(-dGap2, nDimDan, _T(""), COMMA(dGap2)) : Dom.DimLineTo(-dGap2, nDimDan, COMMA(dGap2));
		Dom.DimMoveTo(dDimX-dD, -dF1, nDimDan+1);
		Dom.DimLineTo(heightTotal+dF1, nDimDan+1, COMMA(heightTotal+dF1));

		// 측면
		nDimDan = 1;
		Dom.SetDirection("RIGHT");
		Dom.DimMoveTo(dDimX, -dF1, nDimDan);
		bInput ? Dom.DimLineToExtend(dF1, nDimDan, _T("F1"), COMMA(dF1)) : Dom.DimLineTo(dF1, nDimDan, COMMA(dF1));

		// 상면    |-----> 오른쪽으로 감
		nDimDan = 0;
		Dom.SetDirection("TOP");
		
		Dom.DimMoveTo(dDimX-dW+ddc2, heightTotal, nDimDan);
		Dom.DimLineToOver(-ddc2, nDimDan, COMMA(ddc2), 2);
		dist = ddc2;
		Dom.DimMoveTo(dDimX-dW+ddc2, heightTotal, nDimDan);
		for(i=0; i<nUGugan; i++)
		{
			CString str1(_T(""));
			CString str2(_T(""));
			str1.Format(_T("U%d"), i+1);
			if(nU[i]>1)
				str2.Format(_T("%d@%.f=%.f"), nU[i], dU[i], dU[i]*nU[i]);
			else
				str2 = COMMA(dU[i]);
			bInput ? Dom.DimLineToExtend(dU[i]*nU[i], nDimDan, str1, str2) : Dom.DimLineTo(dU[i]*nU[i], nDimDan, COMMA(dU[i]*nU[i]));
			dist +=  dU[i]*nU[i];
		}
		dGap1 = dW - dist - ddc3;
		bInput ? Dom.DimLineToExtend(dGap1, nDimDan, _T(""), COMMA(dGap1)) : Dom.DimLineTo(dGap1, nDimDan, COMMA(dGap1));
		Dom.DimLineToOver(ddc3, nDimDan, COMMA(ddc3));
		Dom.DimMoveTo(dDimX, heightTotal, nDimDan+1);
		Dom.DimLineTo(-dW, nDimDan+1, COMMA(dW));

		// 하면  <-----| 왼쪽으로 감
		nDimDan = 1;
		Dom.SetDirection("BOTTOM");
		
		Dom.DimMoveTo(dDimX-ddc3, 0, nDimDan);
		bInput ? Dom.DimLineToExtend(dL1, nDimDan, _T("L1"), COMMA(dL1)) : Dom.DimLineTo(dL1, nDimDan, COMMA(dL1));
		Dom.DimMoveTo(dDimX-dW+ddc2, 0, nDimDan);
		bInput ? Dom.DimLineToExtend(dL1, nDimDan, _T("L1"), COMMA(dL1)) : Dom.DimLineTo(dL1, nDimDan, COMMA(dL1));
		
		Dom.DimMoveTo(dDimX-ddc3, 0, nDimDan+2);
		bInput ? Dom.DimLineToOver(ddc3, nDimDan+2, _T("DC3")) : Dom.DimLineToOver(ddc3, nDimDan+2, COMMA(ddc3));
		Dom.DimMoveTo(dDimX-ddc3, 0, nDimDan+2);
		Dom.DimLineTo(-dBottomWidth, nDimDan+2, COMMA(dBottomWidth));
		bInput ? Dom.DimLineToOver(-ddc2, nDimDan+2, _T("DC2"), 2) : Dom.DimLineToOver(-ddc2, nDimDan+2, COMMA(ddc2), 2);

		Dom.DimMoveTo(dDimX, 0, nDimDan+3);
		Dom.DimLineTo(-(ddc3+dBottomWidth+ddc2), nDimDan+3, COMMA(ddc3+dBottomWidth+ddc2));
	}
	else
	{	
		// 측면
		nDimDan = 3;
		Dom.SetDirection("RIGHT");

		Dom.DimMoveTo(dDimX + dW, heightTotal-ddc1, nDimDan);
		bInput ? Dom.DimLineToOverExtend(ddc1, nDimDan, _T("DC1"), COMMA(ddc1)) : Dom.DimLineToOver(ddc1, nDimDan, COMMA(ddc1));
		Dom.DimMoveTo(dDimX + dW, heightTotal-ddc1, nDimDan);
		double dist = ddc1;
		long i=0; for(i=0; i<nSGugan; i++)
		{
			CString str1(_T(""));
			CString str2(_T(""));
			str1.Format(_T("S%d"), i+1);
			if(nS[i]>1)
				str2.Format(_T("%d@%.f=%.f"), nS[i], dS[i], dS[i]*nS[i]);
			else
				str2 = COMMA(dS[i]);
			bInput ? Dom.DimLineToExtend(-dS[i]*nS[i], nDimDan, str1, str2) : Dom.DimLineTo(-dS[i]*nS[i], nDimDan, COMMA(dS[i]*nS[i]));
			dist +=  dS[i]*nS[i];
		}
		double dGap1 = heightTotal - dist + pBri->m_dCoverUpper_UpperSlab[0];
		double dGap2 = dF1 - pBri->m_dCoverUpper_UpperSlab[0];
		// 원래 dGap2가 상부슬래브 상부 피복에서 철근 아래까지 길이이나 필요없는 치수로 판단하여
		// dGap1로 몰아서 한번에 기입함.
		dGap1 += dGap2;
		dGap2	= 0;
		bInput ? Dom.DimLineToExtend(-dGap1, nDimDan, _T(""), COMMA(dGap1)) : Dom.DimLineTo(-dGap1, nDimDan, COMMA(dGap1));
		bInput ? Dom.DimLineToExtend(-dGap2, nDimDan, _T(""), COMMA(dGap2)) : Dom.DimLineTo(-dGap2, nDimDan, COMMA(dGap2));
		Dom.DimMoveTo(dDimX + dW, -dF1, nDimDan+1);
		Dom.DimLineTo(heightTotal+dF1, nDimDan+1, COMMA(heightTotal+dF1));

		// 측면
		nDimDan = 1;
		Dom.SetDirection("LEFT");
		Dom.DimMoveTo(dDimX - (dL1 - ddc2), -dF1, nDimDan);
		bInput ? Dom.DimLineToExtend(dF1, nDimDan, _T("F1"), COMMA(dF1)) : Dom.DimLineTo(dF1, nDimDan, COMMA(dF1));

		// 상면   <-----| 왼쪽으로 감
		nDimDan = 0;
		Dom.SetDirection("TOP");

		Dom.DimMoveTo(dDimX+dW-ddc3, heightTotal, nDimDan);
		Dom.DimLineToOver(ddc3, nDimDan, COMMA(ddc3));
		dist = ddc3;
		Dom.DimMoveTo(dDimX+dW-ddc3, heightTotal, nDimDan);
		for(i=0; i<nUGugan; i++)
		{
			CString str1(_T(""));
			CString str2(_T(""));
			str1.Format(_T("U%d"), i+1);
			if(nU[i]>1)
				str2.Format(_T("%d@%.f=%.f"), nU[i], dU[i], dU[i]*nU[i]);
			else
				str2 = COMMA(dU[i]);
			bInput ? Dom.DimLineToExtend(-dU[i]*nU[i], nDimDan, str1, str2) : Dom.DimLineTo(-dU[i]*nU[i], nDimDan, COMMA(dU[i]*nU[i]));
			dist +=  dU[i]*nU[i];
		}
		dGap1 = dW - dist - ddc2;
		Dom.DimLineToExtend(-dGap1, nDimDan, _T(""), COMMA(dGap1));
		Dom.DimLineToOver(-ddc2, nDimDan, COMMA(ddc2), 2);
		Dom.DimMoveTo(dDimX, heightTotal, nDimDan+1);
		Dom.DimLineTo(dW, nDimDan+1, COMMA(dW));

		// 하면   |-----> 오른쪽으로 감
		nDimDan = 1;
		Dom.SetDirection("BOTTOM");
	
		Dom.DimMoveTo(dDimX+ddc2, -dF1, nDimDan);
		bInput ? Dom.DimLineToExtend(-dL1, nDimDan, _T("L1"), COMMA(dL1)) : Dom.DimLineTo(-dL1, nDimDan, COMMA(dL1));

		Dom.DimMoveTo(dDimX+ddc2, -dF1, nDimDan);
		Dom.DimLineTo(dBottomWidth, nDimDan, COMMA(dBottomWidth));

		Dom.DimMoveTo(dDimX+ddc2, -dF1, nDimDan+2);
		bInput ? Dom.DimLineToOver(-ddc2, nDimDan+2, _T("DC2"), 2) : Dom.DimLineToOver(-ddc2, nDimDan+2, COMMA(ddc2), 2);
		Dom.DimMoveTo(dDimX+ddc2, -dF1, nDimDan+2);
		Dom.DimLineTo(dBottomWidth, nDimDan+2, COMMA(dBottomWidth));
		bInput ? Dom.DimLineToOver(ddc3, nDimDan+2, _T("DC3")) : Dom.DimLineToOver(ddc3, nDimDan+2, COMMA(ddc3));
		
		Dom.DimMoveTo(dDimX, -dF1, nDimDan+3);
		Dom.DimLineTo(ddc2+dBottomWidth+ddc3, nDimDan+3, COMMA(ddc2+dBottomWidth+ddc3));
	}		
		
	// Dom Move
	Dom.Move(vOrigin.x, vOrigin.y);
	*pDomP << Dom;
}

void CDrawCrossSection::DrawRebarRetainingWall(CDomyun* pDomP, long nHDan, BOOL bInclPave, BOOL bInclFootway, BOOL bInput)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	CRcBridgeRebar *pBriRebar = m_pStd->m_pRcBridgeRebar;
	
	CDPoint vAng(0,1);
	double sta = pBri->GetStationBridgeMid();
	CJijum* pJL = pBri->GetJijum(0);
	if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pJL->m_dLengthUgan[1];

	CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan);
	if(!pGW)	return;

	// 외형 Arr 그리기
	long nHDanAct = pBri->GetNumSlabNodeAct(sta, vAng, nHDan);
	double dLenHDan = pBri->GetLengthHDanAct(sta, vAng, nHDan);

	CTwinVectorArray tvArrRect;
	tvArrRect.RemoveAll();	
	CDPoint A[30];
	pBri->GetXySlabUpperAct(sta, vAng, A);
	pBri->GetTvCrossSection(tvArrRect, sta, vAng, FALSE, TRUE, TRUE, 0, FALSE, FALSE);
	CDRect rect = tvArrRect.GetRect();
	rect.left = A[nHDanAct].x - 800.0;
	rect.top += 100.0;
	rect.right = A[nHDanAct].x + dLenHDan + 800.0;
	rect.bottom -= 100.0;

	CDomyun Dom(pDomP);
	
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	m_pStd->DrawCuttingTvArray(&Dom, tvArrRect, rect);
	
	*pDomP << Dom;
	
	// 배근 Arr	
	CTwinVectorArray tvArr;
	CTwinVectorArray tvArrRebar;
	CDPointArray xyPointRebar;
	tvArr.RemoveAll();
	tvArrRebar.RemoveAll();	
	xyPointRebar.RemoveAll();

	pBri->GetTvGuardWall(pGW, tvArr, sta, vAng);
	pBriRebar->m_Util.GetTvRebarGuardWall(pGW, tvArr, tvArrRebar, xyPointRebar, Dom.GetScaleDim());	
	
	// 철근이 똑같은 방호벽이 있는지 검사
	long nQtyHDan		= pBri->GetQtyHDan();
	CGuardWallRC* pCurrGW = pBri->GetGuardWallByHDan(nHDan);
	BOOL bCheck = FALSE;
	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		if(i!=nHDan)
		{
			CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i); 
			if(!pGW)	continue;
			if(pGW->IsTypeHDanRoadRetainingWall())
			{
				if(*pCurrGW == *pGW)
					bCheck = TRUE;
			}
		}
	}

	// Arr 그리기
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	m_pStd->DrawTvArray(&Dom, tvArrRebar);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);
	m_pStd->DrawPointRebar(&Dom, xyPointRebar);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	DrawRebarMarkRetainingWall(&Dom, pGW, tvArrRebar, xyPointRebar, bInput, bCheck);
	DimRebarRetainingWall(&Dom, pGW, tvArr, bInput);

	*pDomP << Dom;
}

void CDrawCrossSection::DrawRebarMarkRetainingWall(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArrRebar, CDPointArray& xyPointRebar, BOOL bInput, BOOL bCheck)
{
	CDomyun Dom(pDomP);
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	
	// 각 철근 개수
	long nSize = tvArrRebar.GetSize();
	long nSizeVR = 8;
	long nDotRebar = xyPointRebar.GetSize();

	BOOL bLeft = pGW->IsLeftGuardFence();
	CDPoint xy(0,0);
	CString sDia(_T("")), sMark(_T("")), sCTC(_T(""));
	CTwinVector tv;
	CDPoint vUnit(0,0);
	double dLength = 0;	
	
	// VR_1
	tv = bLeft ? tvArrRebar.GetAt(1) : tvArrRebar.GetAt(0);
	vUnit = (tv.m_v1 - tv.m_v2).Unit();
	dLength = ~(tv.m_v1 - tv.m_v2) * 0.5;
	xy = tv.m_v2+(vUnit*dLength);
	sDia  = pGW->GetStringDia(pGW->m_dVR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dVR);
	sCTC.Format(_T("C.T.C %.0f"), pGW->m_dCTC);
	if(bLeft)
	{
		sMark = bInput ? _T("VR") : _T("JL1");
		if(!bInput && bCheck)
			sMark = _T("J1");
		// Sym.RebarMarkLineSide(&Dom, xy, sMark, sDia, !bLeft, 0, TRUE, sCTC);
	}
	else
	{
		sMark = bInput ? _T("VR") : _T("JR1");
		if(!bInput && bCheck)
			sMark = _T("J1");
		// Sym.RebarMarkLineSide(&Dom, xy, sMark, sDia, bLeft, 0, TRUE, sCTC);
	}

	CRebar *pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_1)? pGW->m_pArrRebar[GW_REBAR_VERT_1] : NULL;

	SRebarMarkInputData mark_input_data;
	SRebarMarkviewData mark_view_data;

	mark_input_data.mark = sMark;
	mark_input_data.dia = sDia;
	mark_input_data.position = xy;
	mark_input_data.ctc = pGW->m_dCTC;
	mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
	mark_input_data.height	= 0;
	mark_input_data.left_dir = FALSE;
	mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

	mark_view_data.count_mark = TRUE;
	mark_view_data.line_rebar = TRUE;
	mark_view_data.mark_pos = eMarkSide;

	Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	
	// VR_2
	tv = tvArrRebar.GetAt(3);
	vUnit = (tv.m_v1 - tv.m_v2).Unit();
	dLength = ~(tv.m_v1 - tv.m_v2) * 0.5;
	xy = tv.m_v2+(vUnit*dLength);
	sDia  = pGW->GetStringDia(pGW->m_dVR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dVR);
	sCTC.Format(_T("C.T.C %.0f"), pGW->m_dCTC);
	if(bLeft)
	{
		sMark = bInput ? _T("VR") : _T("JL2");
		if(!bInput && bCheck)
			sMark = _T("J2");
		// Sym.RebarMarkLineSide(&Dom, xy, sMark, sDia, bLeft, 0, TRUE, sCTC);
	}
	else
	{
		sMark = bInput ? _T("VR") : _T("JR2");
		if(!bInput && bCheck)
			sMark = _T("J2");
		// Sym.RebarMarkLineSide(&Dom, xy, sMark, sDia, !bLeft, 0, TRUE, sCTC);
	}

	pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_2)? pGW->m_pArrRebar[GW_REBAR_VERT_2] : NULL;

	mark_input_data.mark = sMark;
	mark_input_data.dia = sDia;
	mark_input_data.position = xy;
	mark_input_data.ctc = pGW->m_dCTC;
	mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
	mark_input_data.height	= 0;
	mark_input_data.left_dir = TRUE;
	mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

	mark_view_data.count_mark = TRUE;
	mark_view_data.line_rebar = TRUE;
	mark_view_data.mark_pos = eMarkSide;

	Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	
	// VR_3
	if(tvArrRebar.GetSize() < 7) return;

	tv = tvArrRebar.GetAt(6);
	vUnit = (tv.m_v1 - tv.m_v2).Unit();
	dLength = ~(tv.m_v1 - tv.m_v2) * 0.5;
	xy = tv.m_v2+(vUnit*dLength);
	sDia  = pGW->GetStringDia(pGW->m_dVR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dVR);
	sCTC.Format(_T("C.T.C %.0f"), pGW->m_dCTC);
	
	if(bLeft)
		sMark = bInput ? _T("VR") : _T("JL3");
	else
		sMark = bInput ? _T("VR") : _T("JR3");
		
	if(!bInput && bCheck)
		sMark = _T("J3");
	
	// Sym.RebarMarkLineUpper(&Dom, xy, sMark, sDia, !bLeft, Dom.Always(15), TRUE, FALSE, sCTC);

	pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_3)? pGW->m_pArrRebar[GW_REBAR_VERT_3] : NULL;

	mark_input_data.mark = sMark;
	mark_input_data.dia = sDia;
	mark_input_data.position = xy;
	mark_input_data.ctc = pGW->m_dCTC;
	mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
	mark_input_data.height	= Dom.Always(15);
	mark_input_data.left_dir = !bLeft;
	mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

	mark_view_data.count_mark = TRUE;
	mark_view_data.line_rebar = TRUE;
	mark_view_data.mark_pos = eMarkUpper;

	Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	
	// VR_4
	tv = tvArrRebar.GetAt(7);
	vUnit = (tv.m_v1 - tv.m_v2).Unit();
	dLength = bLeft ? ~(tv.m_v1 - tv.m_v2) * 0.7 : ~(tv.m_v1 - tv.m_v2) * 0.3;
	xy = tv.m_v2+(vUnit*dLength);
	sDia  = pGW->GetStringDia(pGW->m_dVR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dVR);
	sCTC.Format(_T("C.T.C %.0f"), pGW->m_dCTC);
	
	if(bLeft)
		sMark = bInput ? _T("VR") : _T("JL4");
	else
		sMark = bInput ? _T("VR") : _T("JR4");
		
	if(!bInput && bCheck)
		sMark = _T("J4");
	
	// Sym.RebarMarkLineLower(&Dom, xy, sMark, sDia, bLeft, 0, TRUE, FALSE, sCTC);

	pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_4)? pGW->m_pArrRebar[GW_REBAR_VERT_4] : NULL;

	mark_input_data.mark = sMark;
	mark_input_data.dia = sDia;
	mark_input_data.position = xy;
	mark_input_data.ctc = pGW->m_dCTC;
	mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
	mark_input_data.height	= 0;
	mark_input_data.left_dir = bLeft;
	mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

	mark_view_data.count_mark = TRUE;
	mark_view_data.line_rebar = TRUE;
	mark_view_data.mark_pos = eMarkLower;

	Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	
	// HR_5
	double disUpper = 0;
	double disLower = 0;
	long nDotV = nDotRebar - 4 - ( (long)pGW->m_dSC * (long)pGW->m_dEH );
	if(bLeft && xyPointRebar.GetSize() > nDotV+4 || !bLeft && xyPointRebar.GetSize() > nDotV+5)
	{
		xy = bLeft ? xyPointRebar.GetAt(nDotV+4) : xyPointRebar.GetAt(nDotV+5);
		disUpper = pGW->m_dF;
		disLower = pGW->m_dF;
		sDia  = pGW->GetStringDia(pGW->m_dHR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dHR);
		
		if(bLeft)
			sMark = bInput ? _T("HR") : _T("JL5");
		else
			sMark = bInput ? _T("HR") : _T("JR5");
			
		if(!bInput && bCheck)
			sMark = _T("J5");
		
		// Sym.RebarMarkPointSide(&Dom, xy, disUpper, disLower, sMark, sDia, bLeft, 0, CDPoint(0,0));

		pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_HORI_1)? pGW->m_pArrRebar[GW_REBAR_HORI_1] : NULL;

		mark_input_data.mark = sMark;
		mark_input_data.dia = sDia;
		mark_input_data.position = xy;
		mark_input_data.ctc = pGW->m_dFH;
		mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
		mark_input_data.height	= 0;
		mark_input_data.left_dir = bLeft;
		mark_input_data.dist_left_mark = disUpper;
		mark_input_data.dist_right_mark = disLower;

		mark_view_data.count_mark = TRUE;
		mark_view_data.line_rebar = FALSE;
		mark_view_data.mark_pos = eMarkSide;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}
	
	// HR_6
	long nDotH = 8;
	if(bLeft && xyPointRebar.GetSize() > nDotH || !bLeft && xyPointRebar.GetSize() > nDotH)
	{
		xy = bLeft ? xyPointRebar.GetAt(nDotH) : xyPointRebar.GetAt(nDotH);
		disUpper = pGW->m_dFH;
		disLower = pGW->m_dFH;
		sDia  = pGW->GetStringDia(pGW->m_dHR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dHR);
		if(bLeft)
			sMark = bInput ? _T("HR") : _T("JL5");
		else
			sMark = bInput ? _T("HR") : _T("JR5");
			
		if(!bInput && bCheck)
			sMark = _T("J5");
		
		// Sym.RebarMarkPointLower(&Dom, xy, disUpper, disLower, sMark, sDia, bLeft);

		pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_HORI_1)? pGW->m_pArrRebar[GW_REBAR_HORI_1] : NULL;

		mark_input_data.mark = sMark;
		mark_input_data.dia = sDia;
		mark_input_data.position = xy;
		mark_input_data.ctc = pGW->m_dFH;
		mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
		mark_input_data.height	= 0;
		mark_input_data.left_dir = bLeft;
		mark_input_data.dist_left_mark = disUpper;
		mark_input_data.dist_right_mark = disLower;

		mark_view_data.count_mark = TRUE;
		mark_view_data.line_rebar = FALSE;
		mark_view_data.mark_pos = eMarkLower;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}

	// SR
	sDia  = pGW->GetStringDia(pGW->m_dSR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dSR);
	
	if(bLeft)
		sMark = bInput ? _T("SR") : _T("JL6");
	else
		sMark = bInput ? _T("SR") : _T("JR6");
		
	if(!bInput && bCheck)
		sMark = _T("J6");
	
	long nH = ( ( (long)pGW->m_dSC * (long)pGW->m_dEH ) / ( (long)pGW->m_dSC * 2 ) ) - 1;
	long nV = ( nDotV / ( (long)pGW->m_dSC * 2 ) ) - 1;
	long nShear = nSize - nSizeVR;
	CDPoint vP1, vP2;
	long i=0; for(i=0; i<nShear; i++)
	{
		if(tvArrRebar.GetSize() <= nSizeVR+i) break;

		tv = tvArrRebar.GetAt(nSizeVR+i);
		vUnit = (tv.m_v1 - tv.m_v2).Unit();
		dLength = ~(tv.m_v1 - tv.m_v2) * 0.5;
		xy = tv.m_v2+(vUnit*dLength);
		Sym.RebarCountMarkOne(&Dom, xy);
		
		if(i==0)
		{	
			// Sym.RebarMarkLineSide(&Dom, xy, sMark, sDia, !bLeft, Dom.Always(30), FALSE);

			pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_SHEAR_1+i)? pGW->m_pArrRebar[GW_REBAR_SHEAR_1+i] : NULL;

			mark_input_data.mark = sMark;
			mark_input_data.dia = sDia;
			mark_input_data.position = xy;
			mark_input_data.ctc = pGW->m_dCTC;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.height	= Dom.Always(30);
			mark_input_data.left_dir = !bLeft;
			
			mark_view_data.count_mark = FALSE;
			mark_view_data.line_rebar = TRUE;
			mark_view_data.mark_pos = eMarkSide;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

			Dom.MoveTo(xy.x, xy.y);
		}
		if(i+1 == nH)
		{	
			Dom.LineTo(xy);	
		}
		if(i == nH)
		{
			vP1 = xy;
			Dom.MoveTo(xy.x, xy.y);
		}
		if(i+1 == nV+nH)
		{
			vP2 = xy;
			Dom.LineTo(xy);
			CDPoint vMid = (vP1+vP2)/2;
			vMid = CDPoint(vMid.x, vMid.y+Dom.Always(5));
			// Sym.RebarMarkLineSide(&Dom, vMid, sMark, sDia, bLeft, Dom.Always(20), FALSE);

			pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_SHEAR_1+i)? pGW->m_pArrRebar[GW_REBAR_SHEAR_1+i] : NULL;

			mark_input_data.mark = sMark;
			mark_input_data.dia = sDia;
			mark_input_data.position = vMid;
			mark_input_data.ctc = pGW->m_dCTC;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.height	= Dom.Always(20);
			mark_input_data.left_dir = bLeft;
			
			mark_view_data.count_mark = FALSE;
			mark_view_data.line_rebar = TRUE;
			mark_view_data.mark_pos = eMarkSide;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}
	}
	*pDomP << Dom;
}

void CDrawCrossSection::DimRebarRetainingWall(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArr, BOOL bInput)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	long nSize = tvArr.GetSize();
	CDPoint vOrigin1 = pGW->IsLeftGuardFence() ? tvArr.GetAt(nSize-1).m_v2 : tvArr.GetAt(1).m_v1;
	CDPoint vOrigin2 = pGW->IsLeftGuardFence() ? CDPoint(tvArr.GetAt(5).m_v1.x, tvArr.GetAt(7).m_v1.y)
											   : CDPoint(tvArr.GetAt(9).m_v1.x, tvArr.GetAt(7).m_v1.y);

	// 외형
	double dD  = pGW->m_dDistFrLeft;
	double dH1 = pGW->m_dH1;
	double dW1 = pGW->m_dW1;
	double dW2 = pGW->m_dW2;
	double dW4 = pGW->m_dW4;
	double dW5 = pGW->m_dW5;
	
	// 배근
	double ddc1 = pGW->m_ddc1;
	double ddc2 = pGW->m_ddc2;
	double ddc3 = pGW->m_ddc3;
	double dF1  = pGW->m_dF1;
	double dE   = pGW->m_dE;
	double dEH  = pGW->m_dEH;
	double dF   = pGW->m_dF;
	double dFH  = pGW->m_dFH;
	double dW3R = pGW->m_dW3;
	double heightTotal = pGW->GetHeight();
	double dW	= pGW->GetWidth();

	double dTopWidth    = dW3R-ddc2-ddc3;
	double dBottomWidth = dW-ddc2-ddc3;

	CString str = _T("");
	CDomyun Dom(pDomP);
	Dom.SetScaleDim(20);
		
	double dDimX = 0;
	long nDimDan = 0;

	if(pGW->IsLeftGuardFence())
	{
		// 외형
		nDimDan = 2;
		Dom.SetDirection("RIGHT");
		Dom.DimMoveTo(dDimX, 0, nDimDan);
		Dom.DimLineTo(ddc1, nDimDan, COMMA(ddc1));
		Dom.DimLineTo(dH1-ddc1-ddc3, nDimDan, COMMA(dH1-ddc1-ddc3));
		Dom.DimLineTo(ddc3, nDimDan, COMMA(ddc3));
		Dom.DimLineTo(heightTotal-dH1, nDimDan, COMMA(heightTotal-dH1));
		Dom.DimMoveTo(dDimX, 0, nDimDan+1);
		Dom.DimLineTo(heightTotal, nDimDan+1, COMMA(heightTotal));
		// Dom Move
		Dom.Move(vOrigin1.x, vOrigin1.y);
		*pDomP << Dom;
		
		// 측면
		nDimDan = 2;
		Dom.SetDirection("LEFT");
		Dom.DimMoveTo(dDimX, -ddc1, nDimDan);
		bInput ? Dom.DimLineToOverExtend(ddc1, nDimDan, _T("DC1"), COMMA(ddc1)) : Dom.DimLineTo(-ddc1, nDimDan, COMMA(ddc1));
		Dom.DimMoveTo(dDimX, -ddc1, nDimDan);
		bInput ? str.Format(_T("VEA@%.0f=%.0f"),dF,dE*dF) : str.Format(_T("%.0f@%.0f=%.0f"),dE,dF,dE*dF);
		Dom.DimLineTo(-dE*dF, nDimDan, str);
		str.Format(_T("%.0f"),dF1);
		Dom.DimLineTo(-dF1, nDimDan, str);
		bInput ? Dom.DimLineToOverExtend(-ddc1, nDimDan, _T("DC1"), COMMA(ddc1), 2) : Dom.DimLineToOver(-ddc1, nDimDan, COMMA(ddc1), 2);
		// Dom Move
		Dom.Move(vOrigin2.x, vOrigin2.y);
		*pDomP << Dom;
		
		// 상면    <-----| 왼쪽으로 감
		nDimDan = 0;
		Dom.SetDirection("TOP");
		Dom.DimMoveTo(dDimX, heightTotal, nDimDan);
		Dom.DimLineTo(-dW5, nDimDan, COMMA(dW5));
		Dom.DimLineTo(-dW4, nDimDan, COMMA(dW4));
		bInput ? Dom.DimLineToExtend(-ddc3, nDimDan, _T(""), COMMA(ddc3)) : Dom.DimLineTo(-ddc3, nDimDan, COMMA(ddc3));
		Dom.DimLineTo(-dTopWidth, nDimDan, COMMA(dTopWidth));
		bInput ? Dom.DimLineToExtend(-ddc2, nDimDan, _T(""), COMMA(ddc2)) : Dom.DimLineTo(-ddc2, nDimDan, COMMA(ddc2));
		Dom.DimLineToOver(-dD, nDimDan, COMMA(dD), 2);
		Dom.DimMoveTo(dDimX-dW5-dW4, heightTotal, nDimDan+1);
		Dom.DimLineTo(-(ddc3+dTopWidth+ddc2), nDimDan+1, COMMA(ddc3+dTopWidth+ddc2));
		 
		// 하면  <-----| 왼쪽으로 감
		nDimDan = 1;
		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(dDimX-ddc3, 0, nDimDan);
		bInput ? Dom.DimLineToOver(ddc3, nDimDan, _T("DC3")) : Dom.DimLineToOver(ddc3, nDimDan, COMMA(ddc3));
		Dom.DimMoveTo(dDimX-ddc3, 0, nDimDan);
		bInput ? str.Format(_T("HEA@%.0f=%.0f"),dFH,dEH*dFH) : str.Format(_T("%.0f@%.0f=%.0f"),dEH,dFH,dEH*dFH);
		Dom.DimLineTo(-dEH*dFH, nDimDan, str);
		Dom.DimLineTo(-(dBottomWidth - dEH*dFH), nDimDan, COMMA(dBottomWidth - dEH*dFH));
		bInput ? Dom.DimLineToOver(-ddc2, nDimDan, _T("DC2"), 2) : Dom.DimLineToOver(-ddc2, nDimDan, COMMA(ddc2), 2);
		Dom.DimMoveTo(dDimX, 0, nDimDan+1);
		Dom.DimLineTo(-(ddc3+dBottomWidth+ddc2), nDimDan+1, COMMA(ddc3+dBottomWidth+ddc2));
		// Dom Move
		Dom.Move(vOrigin1.x, vOrigin1.y);
		*pDomP << Dom;
	}
	else
	{	
		// 외형
		nDimDan = 2;
		Dom.SetDirection("LEFT");
		Dom.DimMoveTo(dDimX, 0, nDimDan);
		Dom.DimLineTo(ddc1, nDimDan, COMMA(ddc1));
		Dom.DimLineTo(dH1-ddc1-ddc2, nDimDan, COMMA(dH1-ddc1-ddc2));
		Dom.DimLineTo(ddc2, nDimDan, COMMA(ddc2));
		Dom.DimLineTo(heightTotal-dH1, nDimDan, COMMA(heightTotal-dH1));
		Dom.DimMoveTo(dDimX, 0, nDimDan+1);
		Dom.DimLineTo(heightTotal, nDimDan+1, COMMA(heightTotal));
		// Dom Move
		Dom.Move(vOrigin1.x, vOrigin1.y);
		*pDomP << Dom;
		
		// 측면
		nDimDan = 2;
		Dom.SetDirection("RIGHT");
		Dom.DimMoveTo(dDimX, -ddc1, nDimDan);
		bInput ? Dom.DimLineToOverExtend(ddc1, nDimDan, _T("DC1"), COMMA(ddc1)) : Dom.DimLineTo(-ddc1, nDimDan, COMMA(ddc1));
		Dom.DimMoveTo(dDimX, -ddc1, nDimDan);
		bInput ? str.Format(_T("VEA@%.0f=%.0f"),dF,dE*dF) : str.Format(_T("%.0f@%.0f=%.0f"),dE,dF,dE*dF);
		Dom.DimLineTo(-dE*dF, nDimDan, str);
		str.Format(_T("%.0f"),dF1);
		Dom.DimLineTo(-dF1, nDimDan, str);
		bInput ? Dom.DimLineToOverExtend(-ddc1, nDimDan, _T("DC1"), COMMA(ddc1), 2) : Dom.DimLineToOver(-ddc1, nDimDan, COMMA(ddc1), 2);
		// Dom Move
		Dom.Move(vOrigin2.x, vOrigin2.y);
		*pDomP << Dom;

		// 상면   |-----> 오른쪽으로 감
		nDimDan = 0;	
		double dDistOnRight = pBri->GetLengthHDan(pGW->m_nNumberHDan) - dW;	
		Dom.SetDirection("TOP");
		Dom.DimMoveTo(dDimX, heightTotal, nDimDan);
		Dom.DimLineTo(dW1, nDimDan, COMMA(dW1));
		Dom.DimLineTo(dW2, nDimDan, COMMA(dW2));
		bInput ? Dom.DimLineToExtend(ddc2, nDimDan, _T(""), COMMA(ddc2)) : Dom.DimLineTo(ddc2, nDimDan, COMMA(ddc2));
		Dom.DimLineTo(dTopWidth, nDimDan, COMMA(dTopWidth));
		bInput ? Dom.DimLineToExtend(ddc3, nDimDan, _T(""), COMMA(ddc3)) : Dom.DimLineTo(ddc3, nDimDan, COMMA(ddc3));
		Dom.DimLineToOver(dDistOnRight, nDimDan, COMMA(dDistOnRight));
		Dom.DimMoveTo(dDimX+dW1+dW2, heightTotal, nDimDan+1);
		Dom.DimLineTo(ddc2+dTopWidth+ddc3, nDimDan+1, COMMA(ddc2+dTopWidth+ddc3));
		
		// 하면   |-----> 오른쪽으로 감
		nDimDan = 1;
		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(dDimX+ddc2, 0, nDimDan);
		bInput ? Dom.DimLineToOver(-ddc2, nDimDan, _T("DC2"), 2) : Dom.DimLineToOver(-ddc2, nDimDan, COMMA(ddc2), 2);
		Dom.DimMoveTo(dDimX+ddc2, 0, nDimDan);
		bInput ? str.Format(_T("HEA@%.0f=%.0f"),dFH,dEH*dFH) : str.Format(_T("%.0f@%.0f=%.0f"),dEH,dFH,dEH*dFH);
		Dom.DimLineTo(dEH*dFH, nDimDan, str);
		Dom.DimLineTo((dBottomWidth - dEH*dFH), nDimDan, COMMA(dBottomWidth - dEH*dFH));
		bInput ? Dom.DimLineToOver(ddc3, nDimDan, _T("DC3")) : Dom.DimLineToOver(ddc3, nDimDan, COMMA(ddc3));
		Dom.DimMoveTo(dDimX, 0, nDimDan+1);
		Dom.DimLineTo(ddc2+dBottomWidth+ddc3, nDimDan+1, COMMA(ddc2+dBottomWidth+ddc3));
		// Dom Move
		Dom.Move(vOrigin1.x, vOrigin1.y);
		*pDomP << Dom;
	}		
}

void CDrawCrossSection::DrawRebarHandRail(CDomyun* pDomP, long nHDan, BOOL bInclPave, BOOL bInclFootway, BOOL bInput)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	CRcBridgeRebar *pBriRebar = m_pStd->m_pRcBridgeRebar;
	
	CDPoint vAng(0,1);
	double sta = pBri->GetStationBridgeMid();
	CJijum* pJL = pBri->GetJijum(0);
	if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pJL->m_dLengthUgan[1];

	CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan);
	if(!pGW)	return;

	// 외형 Arr 그리기
	long nHDanAct = pBri->GetNumSlabNodeAct(sta, vAng, nHDan);
	double dLenHDan = pBri->GetLengthHDanAct(sta, vAng, nHDan);

	CTwinVectorArray tvArrRect;
	tvArrRect.RemoveAll();	
	CDPoint A[30];
	pBri->GetXySlabUpperAct(sta, vAng, A);
	pBri->GetTvCrossSection(tvArrRect, sta, vAng, FALSE, TRUE, TRUE, 0, FALSE, FALSE);
	CDRect rect = tvArrRect.GetRect();
	rect.left = A[nHDanAct].x - 800.0;
	rect.top += 100.0;
	rect.right = A[nHDanAct].x + dLenHDan + 800.0;
	rect.bottom -= 100.0;

	CDomyun Dom(pDomP);
	
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	m_pStd->DrawCuttingTvArray(&Dom, tvArrRect, rect);
	
	*pDomP << Dom;
	
	// 배근 Arr	
	CTwinVectorArray tvArr;
	CTwinVectorArray tvArrRebar;
	CDPointArray xyPointRebar;
	tvArr.RemoveAll();
	tvArrRebar.RemoveAll();	
	xyPointRebar.RemoveAll();

	pBri->GetTvGuardWall(pGW, tvArr, sta, vAng);
	pBriRebar->m_Util.GetTvRebarGuardWall(pGW, tvArr, tvArrRebar, xyPointRebar, Dom.GetScaleDim());	
	
	// 철근이 똑같은 방호벽이 있는지 검사
	long nQtyHDan		= pBri->GetQtyHDan();
	CGuardWallRC* pCurrGW = pBri->GetGuardWallByHDan(nHDan);
	BOOL bCheck = FALSE;
	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		if(i!=nHDan)
		{
			CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i); 
			if(!pGW)	continue;
			if(pGW->IsTypeHDanRoadHandRail())
			{
				if(*pCurrGW == *pGW)
					bCheck = TRUE;
			}
		}
	}

	// Arr 그리기
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	m_pStd->DrawTvArray(&Dom, tvArrRebar);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);
	m_pStd->DrawPointRebar(&Dom, xyPointRebar);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	DrawRebarMarkHandRail(&Dom, pGW, tvArrRebar, xyPointRebar, bInput, bCheck);
	DimRebarHandRail(&Dom, pGW, tvArr, bInput);

	*pDomP << Dom;
}

void CDrawCrossSection::DrawRebarMarkHandRail(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArrRebar, CDPointArray& xyPointRebar, BOOL bInput, BOOL bCheck)
{
	CDomyun Dom(pDomP);
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	
	// 각 철근 개수
	long nSize = tvArrRebar.GetSize();
	long nDotRebar = xyPointRebar.GetSize();
	long nMarkDotRebar = (long)((pGW->m_dSC * 2) + 2);	// Mark 를 그릴 Dot Rebar_ 개수 (한묶음)
	nMarkDotRebar > 4 ?  nMarkDotRebar = 4 : nMarkDotRebar;

	BOOL bLeft = pGW->IsLeftGuardFence();
	CDPoint xy(0,0);
	CString sDia(_T("")), sMark(_T("")), sCTC(_T(""));
	CTwinVector tv;
	CDPoint vUnit(0,0);
	double dLength = 0;	
	double dMargin = 50;
	long nNum = 1;

	SRebarMarkInputData mark_input_data;
	SRebarMarkviewData mark_view_data;

	// VR_1 : 좌측 수직
	tv = tvArrRebar.GetAt(1);
	vUnit = (tv.m_v1 - tv.m_v2).Unit();
	dLength = ~(tv.m_v1 - tv.m_v2) * 0.5;
	xy = tv.m_v2+(vUnit*dLength);
	sDia  = pGW->GetStringDia(pGW->m_dVR);
	sCTC.Format(_T("C.T.C %.0f"), pGW->m_dCTC);

	CRebar *pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_1)? pGW->m_pArrRebar[GW_REBAR_VERT_1] : NULL;

	if(pRB)
	{
		if(bInput)	sMark  =  _T("VR");
		else		sMark = pRB->m_sMark;
		nNum++;

		mark_input_data.mark = sMark;
		mark_input_data.dia = sDia;
		mark_input_data.position = xy;
		mark_input_data.ctc = pGW->m_dCTC;
		mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
		mark_input_data.height	= 0;
		mark_input_data.left_dir = bLeft;
		mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

		mark_view_data.count_mark = TRUE;
		mark_view_data.line_rebar = TRUE;
		mark_view_data.mark_pos = eMarkSide;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}
	

	// VR_2 : 우측 
	CTwinVector tvM;
	BOOL bFindRebar = FALSE;
	for(long ix = 0; ix < nSize; ix++)
	{
		tvM = tvArrRebar.GetAt(ix);
		if(tvM.m_sInfo.Find(_T("주철근2")) == -1) continue;
		if(tvM.GetVerLength() == 0) continue;
		tv = tvM;
		bFindRebar = TRUE;
	}
	if(bFindRebar)
	{
		vUnit = (tv.m_v2 - tv.m_v1).Unit();
		dLength = ~(tv.m_v2 - tv.m_v1) * 0.5;
		xy = tv.m_v2-(vUnit*dLength);
		sDia  = pGW->GetStringDia(pGW->m_dVR);

		pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_2)? pGW->m_pArrRebar[GW_REBAR_VERT_2] : NULL;

		if(pRB)
		{
			if(bInput)		sMark =  _T("VR");
			else			sMark = pRB->m_sMark;
			nNum++;

			mark_input_data.mark = sMark;
			mark_input_data.dia = sDia;
			mark_input_data.position = xy;
			mark_input_data.ctc = pGW->m_dCTC;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.height	= 0;
			mark_input_data.left_dir = !bLeft;
			mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

			mark_view_data.count_mark = TRUE;
			mark_view_data.line_rebar = TRUE;
			mark_view_data.mark_pos = eMarkSide;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}		
	}
	
	// VR_3
	bFindRebar = FALSE;
	for(long ix = 0; ix < nSize; ix++)
	{
		tvM = tvArrRebar.GetAt(ix);
		if(tvM.m_sInfo.Find(_T("주철근3")) == -1) continue;
		if(tvM.GetVerLength() == 0) continue;
		tv = tvM;
		bFindRebar = TRUE;
	}
	if(bFindRebar)
	{
		vUnit = (tv.m_v2 - tv.m_v1).Unit();
		dLength = bLeft ? ~(tv.m_v2 - tv.m_v1) * 0.4 : ~(tv.m_v2 - tv.m_v1) * 0.6;
		xy = tv.m_v1+(vUnit*dLength);
		sDia  = pGW->GetStringDia(pGW->m_dVR);
		sCTC.Format(_T("C.T.C %.0f"), pGW->m_dCTC);

		pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_3)? pGW->m_pArrRebar[GW_REBAR_VERT_3] : NULL;
		
		if(pRB)
		{
			if(bInput)		sMark =  _T("VR");
			else			sMark = pRB->m_sMark;
			nNum++;

			mark_input_data.mark = sMark;
			mark_input_data.dia = sDia;
			mark_input_data.position = xy;
			mark_input_data.ctc = pGW->m_dCTC;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.height	= 0;
			mark_input_data.left_dir = !bLeft;
			mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

			mark_view_data.count_mark = TRUE;
			mark_view_data.line_rebar = TRUE;
			mark_view_data.mark_pos = eMarkSide;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}		
	}
	
	// HR_3
	tv = tvArrRebar.GetAt(2); // 좌측수직철근의 상면
	CDPoint xyTarget = (tv.m_v1 + tv.m_v2)/2 + (pGW->m_ddc1 + dMargin*2) * CDPoint(0,1);
	double nMarkSu = nDotRebar-nMarkDotRebar >=0 ? nDotRebar-nMarkDotRebar : 0;
	for(long i=nDotRebar-1; i>nMarkSu-1; i--)
	{
		xy = xyPointRebar.GetAt(i);
		Dom.MoveTo(xy);
		Dom.LineTo(xyTarget);
	}

	sDia  = pGW->GetStringDia(pGW->m_dHR);

	pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_HORI_1)? pGW->m_pArrRebar[GW_REBAR_HORI_1] : NULL;
	if(pRB)
	{
		if(bInput)	sMark =  _T("HR") ;
		else		sMark = pRB->m_sMark;	
		nNum++;

		mark_input_data.mark = sMark;
		mark_input_data.dia = sDia;
		mark_input_data.position = xyTarget;
		mark_input_data.ctc = pGW->m_dFH;
		mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
		mark_input_data.height	= 0;
		mark_input_data.left_dir = !bLeft;

		mark_view_data.count_mark = FALSE;
		mark_view_data.line_rebar = TRUE;
		mark_view_data.mark_pos = eMarkSide;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}
	

	// SR_5,6,7,...
	//SR, 전단철근은 시작하는 구간을 찾고 시작
	CLongArray nArrIdx;
	 for (long nIdx =0; nIdx < pGW->m_pArrRebar.GetSize(); nIdx++)
	 {
		 CString sVal = pGW->m_pArrRebar.GetAt(nIdx)->m_sDescription;
		 if(sVal.Find(_T("전단")) > -1)
		 {
			 nArrIdx.Add(nIdx);
		 }
	 }
	
	if(nArrIdx.GetSize() > 0)
	{
		nArrIdx.Sort(TRUE);
		nNum = nArrIdx.GetAt(0);
	}
	

	CDPointArray xyArr;
	xyArr.RemoveAll();
	
	long nGugan=0, nPrevGugan=0;
	BOOL bFirstShear = TRUE;
	long nIdxNum = nNum;

	for(long ix = 0; ix < nSize; ix++)
	{
		tv = tvArrRebar.GetAt(ix);
		if(tv.m_sInfo.Find(_T("전단")) == -1) continue;
		if(bLeft)
		{
			vUnit = (tv.m_v2 - tv.m_v1).Unit();
			dLength = ~(tv.m_v1 - tv.m_v2) * 0.5;
			xy = tv.m_v1+(vUnit*dLength);
		}
		else
		{
			vUnit = (tv.m_v1 - tv.m_v2).Unit();
			dLength = ~(tv.m_v1 - tv.m_v2) * 0.5;
			xy = tv.m_v2+(vUnit*dLength);
		}
		sDia  = pGW->GetStringDia(pGW->m_dSR);

		// 구간
		double dOriginY = tvArrRebar.GetAt(0).m_v1.y + pGW->m_dF1;
		if(tv.m_v1.y < dOriginY + pGW->m_dCrossF2-0.5)
			nGugan = 0;
		else if(tv.m_v1.y >= (dOriginY+pGW->m_dCrossF2-0.5) && tv.m_v1.y <= (dOriginY+pGW->m_dCrossF2+pGW->m_dCrossF3+0.5))
			nGugan = 1;
		else
			nGugan = 2;


		CString str(_T(""));

		// 구간에 따라 마크 그리기
		if(bFirstShear || nPrevGugan==nGugan)
			xyArr.Add(xy);
		else
		{
			double dx=0, dy=0;
			for(long k=xyArr.GetSize()-1; k>=0; k--)
			{
				if(k==xyArr.GetSize()-1)
				{
					dx = xyArr.GetAt(k).x;
					dy = xyArr.GetAt(k).y;
					
					Dom.MoveTo(dx, dy);
					Dom.LineTo(dx, dy+dMargin);
					// Sym.RebarMarkLineSide(&Dom, CDPoint(dx, dy+dMargin), sMark, sDia, bLeft, 0, FALSE);

					pRB = (pGW->m_pArrRebar.GetSize() > nIdxNum)? pGW->m_pArrRebar[nIdxNum] : NULL;
					
					if(pRB)
					{
						if(bInput)	sMark = _T("SR");
						else		sMark = pRB->m_sMark;
						nIdxNum++;


						mark_input_data.mark = sMark;
						mark_input_data.dia = sDia;
						mark_input_data.position = CDPoint(dx, dy+dMargin);
						mark_input_data.ctc = pGW->m_dCTC;
						mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
						mark_input_data.height	= 0;
						mark_input_data.left_dir = bLeft;

						mark_view_data.count_mark = FALSE;
						mark_view_data.line_rebar = TRUE;
						mark_view_data.mark_pos = eMarkSide;

						Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

						Dom.MoveTo(dx, dy);
					}					
				}
				dy = xyArr.GetAt(k).y;
				Sym.RebarCountMarkOne(&Dom, CDPoint(dx,dy));
				
				if(k==0)
					Dom.LineTo(xyArr.GetAt(xyArr.GetSize()-1).x, xyArr.GetAt(k).y);
			}
			
			xyArr.RemoveAll();
			xyArr.Add(xy);
		}
		if(ix==nSize-1)
		{
			double dx=0, dy=0;
			for(long k=xyArr.GetSize()-1; k>=0; k--)
			{
				if(k==xyArr.GetSize()-1)
				{
					dx = xyArr.GetAt(k).x;
					dy = xyArr.GetAt(k).y;
					
					Dom.MoveTo(dx, dy);
					Dom.LineTo(dx, dy+dMargin);
					// Sym.RebarMarkLineSide(&Dom, CDPoint(dx, dy+dMargin), sMark, sDia, bLeft, 0, FALSE);

					pRB = (pGW->m_pArrRebar.GetSize() > nIdxNum)? pGW->m_pArrRebar[nIdxNum] : NULL;

					if(pRB)
					{
						if(bInput)	sMark = _T("SR");
						else		sMark = pRB->m_sMark;
						nIdxNum++;

						mark_input_data.mark = sMark;
						mark_input_data.dia = sDia;
						mark_input_data.position = CDPoint(dx, dy+dMargin);
						mark_input_data.ctc = pGW->m_dCTC;
						mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
						mark_input_data.height	= 0;
						mark_input_data.left_dir = bLeft;

						mark_view_data.count_mark = FALSE;
						mark_view_data.line_rebar = TRUE;
						mark_view_data.mark_pos = eMarkSide;

						Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

						Dom.MoveTo(dx, dy);
					}					
				}
				dy = xyArr.GetAt(k).y;
				Sym.RebarCountMarkOne(&Dom, CDPoint(dx,dy));
				if(k==0)
					Dom.LineTo(xyArr.GetAt(xyArr.GetSize()-1).x, xyArr.GetAt(k).y);
			}
			xyArr.RemoveAll();
		}
		nPrevGugan = nGugan;
	}
	*pDomP << Dom;
}

void CDrawCrossSection::DimRebarHandRail(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArr, BOOL bInput)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	CDPoint vOrigin = pGW->IsLeftGuardFence() ? tvArr.GetAt(tvArr.GetSize()-1).m_v2 : tvArr.GetAt(1).m_v1;
	
	// 외형
	double dD  = pGW->m_dDistFrLeft;
	double dH1 = pGW->m_dH1;
	double dH2 = pGW->m_dH2;
	double dH3 = pGW->m_dH3;
	double dW1 = pGW->m_dW1;
	double dW2 = pGW->m_dW2;
	double dW4 = pGW->m_dW4;
	double dW5 = pGW->m_dW5;
	
	// 배근
	double ddc1 = pGW->m_ddc1;
	double ddc2 = pGW->m_ddc2;
	double ddc3 = pGW->m_ddc3;
	double dF1  = pGW->m_dF1;
	double dF2  = pGW->m_dF2;
	double dF3  = pGW->m_dF3;
	double dE   = pGW->m_dE;
	double dF   = pGW->m_dF;
	double dL1  = pGW->m_dL1;
	double dW3R = pGW->m_dW3;
	double heightTotal = pGW->GetHeight();
	double dW	= pGW->GetWidth();

	double dTopWidth    = dW3R-ddc2-ddc3;
	double dBottomWidth = dW-ddc2-ddc3;

	double dE_Height = heightTotal - dF2 - dF3 - ddc1;

	CString str = _T(""), str2 = _T("");
	CDomyun Dom(pDomP);
	Dom.SetScaleDim(20);
		
	double dDimX = 0;
	long nDimDan = 0;

	if(pGW->IsLeftGuardFence())
	{
		// 외형
		nDimDan = 1;
		Dom.SetDirection("RIGHT");
		Dom.DimMoveTo(dDimX, 0, nDimDan);
		Dom.DimLineTo(dH1, nDimDan, COMMA(dH1));
		Dom.DimLineTo(dH2, nDimDan, COMMA(dH2));
		Dom.DimLineTo(dH3, nDimDan, COMMA(dH3));

		//높이 2개 이상이 "0"보다 클 경우
		if((dH1 > 0 && dH2 > 0) || (dH2 > 0 && dH3 > 0) || (dH3 > 0 && dH1 > 0))
		{
			Dom.DimMoveTo(dDimX, 0, nDimDan+1);
			Dom.DimLineTo(dH1+dH2+dH3, nDimDan+1, COMMA(dH1+dH2+dH3));
		}
		
		// 측면
		nDimDan = 3;
		Dom.SetDirection("LEFT");
		Dom.DimMoveTo(dDimX, -dF1, nDimDan);
		bInput ? Dom.DimLineToExtend(dF1, nDimDan, _T("F1"), COMMA(dF1)) : Dom.DimLineTo(dF1, nDimDan, COMMA(dF1));
		bInput ? Dom.DimLineToExtend(dF2, nDimDan, _T("F2"), COMMA(dF2)) : Dom.DimLineTo(dF2, nDimDan, COMMA(dF2));
		bInput ? Dom.DimLineToExtend(dF3, nDimDan, _T("F3"), COMMA(dF3)) : Dom.DimLineTo(dF3, nDimDan, COMMA(dF3));

		if(dE > 0)
		{
			str.Format("%.0f@%.0f",dE,dF); //bInput ? str.Format("EA@%.0f",dF) : str.Format("%.0f@%.0f",dE,dF);
			str2.Format("=%.0f",dE*dF);
		}
		else
		{
			str.Format("%.0f", dE_Height);
			str2 = _T("");
		}
		Dom.DimLineToExtend(dE_Height, nDimDan, str, str2);

		bInput ? Dom.DimLineToOverExtend(ddc1, nDimDan, _T("DC1"), COMMA(ddc1)) : Dom.DimLineToOver(ddc1, nDimDan, COMMA(ddc1));
		Dom.DimMoveTo(dDimX, 0, nDimDan+1);
		Dom.DimLineTo(heightTotal, nDimDan+1, COMMA(heightTotal));

		// 상면    <-----| 왼쪽으로 감
		nDimDan = 0;
		Dom.SetDirection("TOP");
		Dom.DimMoveTo(dDimX, heightTotal, nDimDan);
		Dom.DimLineTo(-dW5, nDimDan, COMMA(dW5));
		Dom.DimLineTo(-dW4, nDimDan, COMMA(dW4));
		bInput ? Dom.DimLineToExtend(-ddc3, nDimDan, _T(""), COMMA(ddc3)) : Dom.DimLineTo(-ddc3, nDimDan, COMMA(ddc3));
		Dom.DimLineTo(-dTopWidth, nDimDan, COMMA(dTopWidth));
		bInput ? Dom.DimLineToExtend(-ddc2, nDimDan, _T(""), COMMA(ddc2)) : Dom.DimLineTo(-ddc2, nDimDan, COMMA(ddc2));
		Dom.DimLineToOver(-dD, nDimDan, COMMA(dD), 2);
		Dom.DimMoveTo(dDimX-dW5-dW4, heightTotal, nDimDan+1);
		Dom.DimLineTo(-(ddc3+dTopWidth+ddc2), nDimDan+1, COMMA(ddc3+dTopWidth+ddc2));
		 
		// 하면  <-----| 왼쪽으로 감
		nDimDan = 1;
		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(dDimX-ddc3, 0, nDimDan);
		bInput ? Dom.DimLineToExtend(dL1, nDimDan, _T("L1"), COMMA(dL1)) : Dom.DimLineTo(dL1, nDimDan, COMMA(dL1));
		Dom.DimMoveTo(dDimX-dW+ddc2, 0, nDimDan);
		bInput ? Dom.DimLineToExtend(dL1, nDimDan, _T("L1"), COMMA(dL1)) : Dom.DimLineTo(dL1, nDimDan, COMMA(dL1));
		Dom.DimMoveTo(dDimX-ddc3, 0, nDimDan+2);
		bInput ? Dom.DimLineToOver(ddc3, nDimDan+2, _T("DC3")) : Dom.DimLineToOver(ddc3, nDimDan+2, COMMA(ddc3));
		Dom.DimMoveTo(dDimX-ddc3, 0, nDimDan+2);
		Dom.DimLineTo(-dBottomWidth, nDimDan+2, COMMA(dBottomWidth));
		bInput ? Dom.DimLineToOver(-ddc2, nDimDan+2, _T("DC2"), 2) : Dom.DimLineToOver(-ddc2, nDimDan+2, COMMA(ddc2), 2);
		Dom.DimMoveTo(dDimX, 0, nDimDan+3);
		Dom.DimLineTo(-(ddc3+dBottomWidth+ddc2), nDimDan+3, COMMA(ddc3+dBottomWidth+ddc2));
	}
	else
	{	
		// 외형
		nDimDan = 1;
		Dom.SetDirection("LEFT");
		Dom.DimMoveTo(dDimX, 0, nDimDan);
		Dom.DimLineTo(dH1, nDimDan, COMMA(dH1));
		Dom.DimLineTo(dH2, nDimDan, COMMA(dH2));
		Dom.DimLineTo(dH3, nDimDan, COMMA(dH3));

		//높이 2개 이상이 "0"보다 클 경우
		if((dH1 > 0 && dH2 > 0) || (dH2 > 0 && dH3 > 0) || (dH3 > 0 && dH1 > 0))
		{
			Dom.DimMoveTo(dDimX, 0, nDimDan+1);
			Dom.DimLineTo(dH1+dH2+dH3, nDimDan+1, COMMA(dH1+dH2+dH3));
		}
		
		// 측면
		nDimDan = 3;
		Dom.SetDirection("RIGHT");
		Dom.DimMoveTo(dDimX, -dF1, nDimDan);
		bInput ? Dom.DimLineToExtend(dF1, nDimDan, _T("F1"), COMMA(dF1)) : Dom.DimLineTo(dF1, nDimDan, COMMA(dF1));
		bInput ? Dom.DimLineToExtend(dF2, nDimDan, _T("F2"), COMMA(dF2)) : Dom.DimLineTo(dF2, nDimDan, COMMA(dF2));
		bInput ? Dom.DimLineToExtend(dF3, nDimDan, _T("F3"), COMMA(dF3)) : Dom.DimLineTo(dF3, nDimDan, COMMA(dF3));
		str.Format("%.0f@%.0f=%.0f",dE,dF,dE*dF);
		Dom.DimLineTo(dE_Height, nDimDan, str);
		bInput ? Dom.DimLineToOverExtend(ddc1, nDimDan, _T("DC1"), COMMA(ddc1)) : Dom.DimLineToOver(ddc1, nDimDan, COMMA(ddc1));
		Dom.DimMoveTo(dDimX, 0, nDimDan+1);
		Dom.DimLineTo(heightTotal, nDimDan+1, COMMA(heightTotal));

		// 상면   |-----> 오른쪽으로 감
		nDimDan = 0;	
		double dDistOnRight = pBri->GetLengthHDan(pGW->m_nNumberHDan) - dW;	
		Dom.SetDirection("TOP");
		Dom.DimMoveTo(dDimX, heightTotal, nDimDan);
		Dom.DimLineTo(dW1, nDimDan, COMMA(dW1));
		Dom.DimLineTo(dW2, nDimDan, COMMA(dW2));
		bInput ? Dom.DimLineToExtend(ddc2, nDimDan, _T(""), COMMA(ddc2)) : Dom.DimLineTo(ddc2, nDimDan, COMMA(ddc2));
		Dom.DimLineTo(dTopWidth, nDimDan, COMMA(dTopWidth));
		bInput ? Dom.DimLineToExtend(ddc3, nDimDan, _T(""), COMMA(ddc3)) : Dom.DimLineTo(ddc3, nDimDan, COMMA(ddc3));;
		Dom.DimLineToOver(dDistOnRight, nDimDan, COMMA(dDistOnRight));

		Dom.DimMoveTo(dDimX+dW1+dW2, heightTotal, nDimDan+1);
		Dom.DimLineTo(ddc2+dTopWidth+ddc3, nDimDan+1, COMMA(ddc2+dTopWidth+ddc3));
		
		// 하면   |-----> 오른쪽으로 감
		nDimDan = 1;
		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(dDimX+ddc2, 0, nDimDan);
		bInput ? Dom.DimLineToExtend(-dL1, nDimDan, _T("L1"), COMMA(dL1)) : Dom.DimLineTo(-dL1, nDimDan, COMMA(dL1));
		Dom.DimMoveTo(dDimX+dW-ddc2, 0, nDimDan);
		bInput ? Dom.DimLineToExtend(-dL1, nDimDan, _T("L1"), COMMA(dL1)) : Dom.DimLineTo(-dL1, nDimDan, COMMA(dL1));
		Dom.DimMoveTo(dDimX+ddc2, 0, nDimDan+2);
		bInput ? Dom.DimLineToOver(-ddc2, nDimDan+2, _T("DC2"), 2) : Dom.DimLineToOver(-ddc2, nDimDan+2, COMMA(ddc2), 2);
		Dom.DimMoveTo(dDimX+ddc2, 0, nDimDan+2);
		Dom.DimLineTo(dBottomWidth, nDimDan+2, COMMA(dBottomWidth));
		bInput ? Dom.DimLineToOver(ddc3, nDimDan+2, _T("DC3")) : Dom.DimLineToOver(ddc3, nDimDan+2, COMMA(ddc3));
		Dom.DimMoveTo(dDimX, 0, nDimDan+3);
		Dom.DimLineTo(ddc2+dBottomWidth+ddc3, nDimDan+3, COMMA(ddc2+dBottomWidth+ddc3));
	}		
		
	// Dom Move
	Dom.Move(vOrigin.x, vOrigin.y);
	*pDomP << Dom;
}

void CDrawCrossSection::DrawRebarRailGuardFence(CDomyun* pDomP, long nHDan, BOOL bInclPave, BOOL bInclFootway, BOOL bInput)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	CRcBridgeRebar *pBriRebar = m_pStd->m_pRcBridgeRebar;
	
	CDPoint vAng(0,1);
	double sta = pBri->GetStationBridgeMid();
	CJijum* pJL = pBri->GetJijum(0);
	if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pJL->m_dLengthUgan[1];

	CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan);
	if(!pGW)	return;

	// 외형 Arr 그리기
	long nHDanAct = pBri->GetNumSlabNodeAct(sta, vAng, nHDan);
	double dLenHDan = pBri->GetLengthHDanAct(sta, vAng, nHDan);

	CTwinVectorArray tvArrRect;
	tvArrRect.RemoveAll();	
	CDPoint A[30];
	pBri->GetXySlabUpperAct(sta, vAng, A);
	pBri->GetTvCrossSection(tvArrRect, sta, vAng, FALSE, TRUE, TRUE, 0, FALSE, FALSE);
	CDRect rect = tvArrRect.GetRect();
	rect.left = A[nHDanAct].x - 800.0;
	rect.top += 100.0;
	rect.right = A[nHDanAct].x + dLenHDan + 800.0;
	rect.bottom -= 100.0;

	CDomyun Dom(pDomP);
	
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	m_pStd->DrawCuttingTvArray(&Dom, tvArrRect, rect);
	
	*pDomP << Dom;
	
	// 배근 Arr	
	CTwinVectorArray tvArr;
	CTwinVectorArray tvArrRebar;
	CDPointArray xyPointRebar;
	tvArr.RemoveAll();
	tvArrRebar.RemoveAll();	
	xyPointRebar.RemoveAll();

	pBri->GetTvGuardWall(pGW, tvArr, sta, vAng);
	pBriRebar->m_Util.GetTvRebarGuardWall(pGW, tvArr, tvArrRebar, xyPointRebar, Dom.GetScaleDim());
	
	// 철근이 똑같은 방호벽이 있는지 검사
	long nQtyHDan		= pBri->GetQtyHDan();
	CGuardWallRC* pCurrGW = pBri->GetGuardWallByHDan(nHDan);
	BOOL bCheck = FALSE;
	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		if(i!=nHDan)
		{
			CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i); 
			if(!pGW)	continue;
			if(pGW->IsTypeHDanRailGuardFence())
			{
				if(*pCurrGW == *pGW)
					bCheck = TRUE;
			}
		}
	}

	// Arr 그리기
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	m_pStd->DrawTvArray(&Dom, tvArrRebar);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);
	m_pStd->DrawPointRebar(&Dom, xyPointRebar);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	DrawRebarMarkGuardFence(&Dom, pGW, tvArrRebar, xyPointRebar, bInput, bCheck); 
	DimRebarGuardFence(&Dom, pGW, tvArr, bInput);

	*pDomP << Dom;
}

void CDrawCrossSection::DrawRebarMarkRailGuardFence(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArrRebar, CDPointArray& xyPointRebar, BOOL bInput, BOOL bCheck)
{
	CDomyun Dom(pDomP);
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);

	BOOL bLeft = pGW->IsLeftGuardFence();
	CDPoint xy(0,0);
	CString sDia(_T("")), sMark(_T("")), sCTC(_T(""));
	CTwinVector tv;
	CDPoint vUnit(0,0);
	double dLength = 0;	
// 	double dMargin = 50;
	long nSize = tvArrRebar.GetSize();

	SRebarMarkInputData mark_input_data;
	SRebarMarkviewData mark_view_data;

	// VR_1
	CRebar* pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_1)? pGW->m_pArrRebar[GW_REBAR_VERT_1] : NULL;

	if(nSize > 2)
	{
		tv = tvArrRebar.GetAt(2);
		vUnit = (tv.m_v1 - tv.m_v2).Unit();
		dLength = ~(tv.m_v1 - tv.m_v2) * 0.5;
		xy = tv.m_v2+(vUnit*dLength);
		sDia  = pGW->GetStringDia(pGW->m_dVR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dVR);
		sCTC.Format(_T("C.T.C %.0f"), pGW->m_dCTC);

		if(bInput || pRB == NULL)
			sMark = _T("VR");
		else
			sMark = pRB->m_sMark;

		// bLeft ? Sym.RebarMarkLineSide(&Dom, xy, sMark, sDia, bLeft, 0, TRUE, sCTC)
		// 	: Sym.RebarMarkLineSide(&Dom, xy, sMark, sDia, !bLeft, 0, TRUE, sCTC);

		mark_input_data.mark = sMark;
		mark_input_data.dia = sDia;
		mark_input_data.position = xy;
		mark_input_data.ctc = pGW->m_dCTC;
		mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
		mark_input_data.height	= 0;
		mark_input_data.left_dir = TRUE;
		mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

		mark_view_data.count_mark = TRUE;
		mark_view_data.line_rebar = TRUE;
		mark_view_data.mark_pos = eMarkSide;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}

	// VR_2
	pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_2) ? pGW->m_pArrRebar[GW_REBAR_VERT_2] : NULL;
	if((bLeft && nSize > 4) || (!bLeft && nSize > 5))
	{
		tv = bLeft ? tvArrRebar.GetAt(4) : tvArrRebar.GetAt(5);
		vUnit = (tv.m_v1 - tv.m_v2).Unit();
		dLength = ~(tv.m_v1 - tv.m_v2) * 0.5;
		xy = tv.m_v2+(vUnit*dLength);
		sDia  = pGW->GetStringDia(pGW->m_dVR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dVR);
		sCTC.Format(_T("C.T.C %.0f"), pGW->m_dCTC);

		if(bInput || pRB == NULL)
			sMark = _T("VR");
		else
			sMark = pRB->m_sMark;

		// bLeft ? Sym.RebarMarkLineUpper(&Dom, xy, sMark, sDia, bLeft, Dom.Always(12), TRUE, FALSE, sCTC)
		//	: Sym.RebarMarkLineUpper(&Dom, xy, sMark, sDia, !bLeft, Dom.Always(12), TRUE, FALSE, sCTC);

		mark_input_data.mark = sMark;
		mark_input_data.dia = sDia;
		mark_input_data.position = xy;
		mark_input_data.ctc = pGW->m_dCTC;
		mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
		mark_input_data.height	= Dom.Always(12);
		mark_input_data.left_dir = TRUE;
		mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

		mark_view_data.count_mark = TRUE;
		mark_view_data.line_rebar = TRUE;
		mark_view_data.mark_pos = eMarkSide;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}

	// VR_3
	pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_2) ? pGW->m_pArrRebar[GW_REBAR_VERT_2] : NULL;
	if((bLeft && nSize > 8) || (!bLeft && nSize > 9))
	{
		tv = bLeft ? tvArrRebar.GetAt(8) : tvArrRebar.GetAt(9);
		vUnit = (tv.m_v1 - tv.m_v2).Unit();
		dLength = ~(tv.m_v1 - tv.m_v2) * 0.5;
		xy = tv.m_v2+(vUnit*dLength);
		sDia  = pGW->GetStringDia(pGW->m_dVR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dVR);
		sCTC.Format(_T("C.T.C %.0f"), pGW->m_dCTC);


		if(bInput || pRB == NULL)
			sMark = _T("VR");
		else
			sMark = pRB->m_sMark;

		// bLeft ? Sym.RebarMarkLineSide(&Dom, xy, sMark, sDia, !bLeft, 0, TRUE, sCTC)
		//	: Sym.RebarMarkLineSide(&Dom, xy, sMark, sDia, bLeft, 0, TRUE, sCTC);

		mark_input_data.mark = sMark;
		mark_input_data.dia = sDia;
		mark_input_data.position = xy;
		mark_input_data.ctc = pGW->m_dCTC;
		mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
		mark_input_data.height	= 0;
		mark_input_data.left_dir = FALSE;
		mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

		mark_view_data.count_mark = TRUE;
		mark_view_data.line_rebar = TRUE;
		mark_view_data.mark_pos = eMarkSide;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}
	
	// SR_1
	pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_SHEAR_1)? pGW->m_pArrRebar[GW_REBAR_SHEAR_1] : NULL;

	long nS = nSize-9-1;
	tv = tvArrRebar.GetAt(nSize-1);
	vUnit = (tv.m_v1 - tv.m_v2).Unit();
	dLength = ~(tv.m_v1 - tv.m_v2) * 0.5;
	xy = tv.m_v2+(vUnit*dLength);
	sDia  = pGW->GetStringDia(pGW->m_dSR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dSR);
	sCTC.Format(_T("C.T.C %.0f"), pGW->m_dCTC * pGW->m_dEC);

	if(bInput || pRB == NULL)
		sMark = _T("SR");
	else
		sMark = pRB->m_sMark;
	
	CDPoint xyLast(0,0);
	long i=0; for(i=0; i<nS; i++)
	{
		tv = tvArrRebar.GetAt(nSize-1-i);
		Sym.RebarCountMarkOne(&Dom, CDPoint(xy.x, tv.m_v2.y));
		if(i==nS-1)
			xyLast = CDPoint(xy.x, tv.m_v2.y);
	}
	if(nS>1)
		Dom.LineTo(xy, xyLast);
	
	// bLeft ? Sym.RebarMarkLineLower(&Dom, xy, sMark, sDia, bLeft, Dom.Always(12), FALSE, FALSE, sCTC)
	//	  : Sym.RebarMarkLineLower(&Dom, xy, sMark, sDia, !bLeft, Dom.Always(12), FALSE, FALSE, sCTC);
	
	mark_input_data.mark = sMark;
	mark_input_data.dia = sDia;
	mark_input_data.position = xy;
	mark_input_data.ctc = pGW->m_dCTC;
	mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
	mark_input_data.height	= Dom.Always(12);
	mark_input_data.left_dir = TRUE;
	mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

	mark_view_data.count_mark = FALSE;
	mark_view_data.line_rebar = TRUE;
	mark_view_data.mark_pos = eMarkLower;

	Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);


	// HR_1
	pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_HORI_1)? pGW->m_pArrRebar[GW_REBAR_HORI_1] : NULL;

	if(xyPointRebar.GetSize() > 5)
	{
		double dMargin = 50;

		CDPoint xyTarget = bLeft ? CDPoint(tvArrRebar.GetXyRight().x, tvArrRebar.GetXyTop().y - dMargin)
								 : CDPoint(tvArrRebar.GetXyLeft().x, tvArrRebar.GetXyTop().y - dMargin);

		sDia  = pGW->GetStringDia(pGW->m_dHR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dHR);
		xy = bLeft ? xyPointRebar.GetAt(3) : xyPointRebar.GetAt(2);
		Dom.MoveTo(xy);
		Dom.LineTo(xyTarget);
		xy = bLeft ? xyPointRebar.GetAt(5) : xyPointRebar.GetAt(4);
		Dom.MoveTo(xy);
		Dom.LineTo(xyTarget);

		if(bInput || pRB == NULL)
			sMark = _T("HR");
		else
			sMark = pRB->m_sMark;

		// Sym.RebarMarkLineSide(&Dom, xyTarget, sMark, sDia, !bLeft, 0, FALSE);

		mark_input_data.mark = sMark;
		mark_input_data.dia = sDia;
		mark_input_data.position = xyTarget;
		mark_input_data.ctc = pGW->m_dFH;
		mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
		mark_input_data.height	= 0;
		mark_input_data.left_dir = TRUE;
		
		mark_view_data.count_mark = FALSE;
		mark_view_data.line_rebar = TRUE;
		mark_view_data.mark_pos = eMarkSide;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}

	*pDomP << Dom;
}

void CDrawCrossSection::DrawRebarFootwayBlock(CDomyun* pDomP, long nHDan, BOOL bInclPave, BOOL bInclFootway, BOOL bInput, BOOL bCover)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	CRcBridgeRebar *pBriRebar = m_pStd->m_pRcBridgeRebar;
	
	CDPoint vAng(0,1);
	double sta = pBri->GetStationBridgeMid();
	CJijum* pJL = pBri->GetJijum(0);
	if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pJL->m_dLengthUgan[1];

	CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan);
	if(!pGW)	return;

	// 외형 Arr 그리기
	long nHDanAct = pBri->GetNumSlabNodeAct(sta, vAng, nHDan);
	double dLenHDan = pBri->GetLengthHDanAct(sta, vAng, nHDan);

	CTwinVectorArray tvArrRect;
	tvArrRect.RemoveAll();	
	CDPoint A[30];
	pBri->GetXySlabUpperAct(sta, vAng, A);
	pBri->GetTvCrossSection(tvArrRect, sta, vAng, FALSE, TRUE, TRUE, 0, FALSE, FALSE);
	CDRect rect = tvArrRect.GetRect();
	rect.left = A[nHDanAct].x - 800.0;
	rect.top += 100.0;
	rect.right = A[nHDanAct].x + dLenHDan + 800.0;
	rect.bottom -= 100.0;

	CDomyun Dom(pDomP);
	
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	m_pStd->DrawCuttingTvArray(&Dom, tvArrRect, rect);
	
	*pDomP << Dom;
	
	// 배근 Arr	
	CTwinVectorArray tvArr;
	CTwinVectorArray tvArrRebar;
	CDPointArray xyPointRebar;
	tvArr.RemoveAll();
	tvArrRebar.RemoveAll();	
	xyPointRebar.RemoveAll();

	pBri->GetTvGuardWall(pGW, tvArr, sta, vAng);
	pBriRebar->m_Util.GetTvRebarGuardWall(pGW, tvArr, tvArrRebar, xyPointRebar, Dom.GetScaleDim(), bCover);	
	
	// 철근이 똑같은 방호벽이 있는지 검사
	long nQtyHDan		= pBri->GetQtyHDan();
	CGuardWallRC* pCurrGW = pBri->GetGuardWallByHDan(nHDan);
	BOOL bCheck = FALSE;
	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		if(i!=nHDan)
		{
			CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i);
			if(!pGW)	continue;
			if(pGW->IsTypeHDanRailFootwayBlock())
			{
				if(*pCurrGW == *pGW)
					bCheck = TRUE;
			}
		}
	}

	// Arr 그리기
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	m_pStd->DrawTvArray(&Dom, tvArrRebar);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);
	m_pStd->DrawPointRebar(&Dom, xyPointRebar);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	DrawRebarMarkFootwayBlock(&Dom, pGW, tvArrRebar, xyPointRebar, bInput, bCheck, bCover);
	DimRebarFootwayBlock(&Dom, pGW, tvArr, bInput, bCover);

	*pDomP << Dom;
}

void CDrawCrossSection::DrawRebarMarkFootwayBlock(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArrRebar, CDPointArray& xyPointRebar, BOOL bInput, BOOL bCheck, BOOL bCover)
{
	CDomyun Dom(pDomP);
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	
	// 각 철근 개수
	long nSize = tvArrRebar.GetSize();
	long nDotRebar = xyPointRebar.GetSize();

	BOOL bLeft = pGW->IsLeftGuardFence();
	CDPoint xy(0,0);
	CString sDia(_T("")), sMark(_T("")), sCTC(_T(""));
	CTwinVector tv;
	CDPoint vUnit(0,0);
	double dLength = 0;	

	CRebar *pRB = NULL;

	SRebarMarkInputData mark_input_data;
	SRebarMarkviewData mark_view_data;
	
	// VR_1
	if(nSize > 2)
	{
		tv = tvArrRebar.GetAt(2);
		vUnit = (tv.m_v1 - tv.m_v2).Unit();
		dLength = ~(tv.m_v1 - tv.m_v2) * 0.5;
		xy = tv.m_v2+(vUnit*dLength);
		sDia  = pGW->GetStringDia(pGW->m_dVR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dVR);
		sCTC.Format(_T("C.T.C %.0f"), pGW->m_dCTC);
		if(bLeft)
		{
			sMark = bInput ? _T("VR") : _T("KL1");
			if(!bInput && bCheck)
				sMark = _T("K1");
			// Sym.RebarMarkLineUpper(&Dom, xy, sMark, sDia, bLeft, Dom.Always(12), TRUE, FALSE, sCTC);
		}
		else
		{
			sMark = bInput ? _T("VR") : _T("KR1");
			if(!bInput && bCheck)
				sMark = _T("K1");
			// Sym.RebarMarkLineUpper(&Dom, xy, sMark, sDia, !bLeft, Dom.Always(12), TRUE, FALSE, sCTC);
		}

		pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_1)? pGW->m_pArrRebar[GW_REBAR_VERT_1] : NULL;

		mark_input_data.mark = sMark;
		mark_input_data.dia = sDia;
		mark_input_data.position = xy;
		mark_input_data.ctc = pGW->m_dCTC;
		mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
		mark_input_data.height	= Dom.Always(12);
		mark_input_data.left_dir = TRUE;
		mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

		mark_view_data.count_mark = TRUE;
		mark_view_data.line_rebar = TRUE;
		mark_view_data.mark_pos = eMarkSide;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}

	// VR_2
	if(nSize > 5)
	{
		tv = tvArrRebar.GetAt(5);
		vUnit = (tv.m_v1 - tv.m_v2).Unit();
		dLength = ~(tv.m_v1 - tv.m_v2) * 0.5;
		xy = tv.m_v2+(vUnit*dLength);
		sDia  = pGW->GetStringDia(pGW->m_dVR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dVR);
		sCTC.Format(_T("C.T.C %.0f"), pGW->m_dCTC);

		if(bLeft)
			sMark = bInput ? _T("VR") : bCover ? _T("KL4") : _T("KL2");
		else
			sMark = bInput ? _T("VR") : bCover ? _T("KR4") : _T("KR2");

		if(!bInput && bCheck)
			sMark = bCover ? _T("K4") : _T("K2");

		// bCover ? Sym.RebarMarkLineSide(&Dom, xy, sMark, sDia, !bLeft, 0, TRUE, sCTC)
		//	: Sym.RebarMarkLineUpper(&Dom, xy, sMark, sDia, !bLeft, Dom.Always(12), TRUE, FALSE, sCTC);

		pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_2)? pGW->m_pArrRebar[GW_REBAR_VERT_2] : NULL;

		mark_input_data.mark = sMark;
		mark_input_data.dia = sDia;
		mark_input_data.position = xy;
		mark_input_data.ctc = pGW->m_dCTC;
		mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
		mark_input_data.height	= bCover? 0 : Dom.Always(12);
		mark_input_data.left_dir = !bLeft;
		mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

		mark_view_data.count_mark = TRUE;
		mark_view_data.line_rebar = TRUE;
		mark_view_data.mark_pos = bCover? eMarkSide : eMarkLower;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}
	
	if(bCover)
	{
		// VR_2
		if(nSize > 7)
		{
			tv = tvArrRebar.GetAt(7);
			vUnit = (tv.m_v1 - tv.m_v2).Unit();
			dLength = ~(tv.m_v1 - tv.m_v2) * 0.2;
			xy = tv.m_v2+(vUnit*dLength);
			sDia  = pGW->GetStringDia(pGW->m_dVR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dVR);
			sCTC.Format(_T("C.T.C %.0f"), pGW->m_dCTC);
			if(bLeft)
				sMark = bInput ? _T("VR") : _T("KL5");
			else
				sMark = bInput ? _T("VR") :  _T("KR5");
			
			if(!bInput && bCheck)
				sMark = _T("K5");
		
			// Sym.RebarMarkLineUpper(&Dom, xy, sMark, sDia, !bLeft, Dom.Always(12), TRUE, FALSE, sCTC);	

			pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_2)? pGW->m_pArrRebar[GW_REBAR_VERT_2] : NULL;

			mark_input_data.mark = sMark;
			mark_input_data.dia = sDia;
			mark_input_data.position = xy;
			mark_input_data.ctc = pGW->m_dCTC;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.height	= Dom.Always(12);
			mark_input_data.left_dir = !bLeft;
			mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

			mark_view_data.count_mark = TRUE;
			mark_view_data.line_rebar = TRUE;
			mark_view_data.mark_pos = eMarkUpper;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}
	}
	// HR_5
	xy = bLeft ? xyPointRebar.GetAt(nDotRebar-2) : xyPointRebar.GetAt(nDotRebar-2);
	double disUpper = (pGW->m_dH1 - pGW->m_ddc3) / pGW->m_dF2;
	double disLower = (pGW->m_dH1 - pGW->m_ddc3) / pGW->m_dF2;
	if(pGW->m_dF2<2)	disLower = 0;
	sDia  = pGW->GetStringDia(pGW->m_dHR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dHR);
	
	if(bLeft)
		sMark = bInput ? _T("HR") : bCover ? _T("KL6") : _T("KL3");
	else
		sMark = bInput ? _T("HR") : bCover ? _T("KR6") : _T("KL3");
		
	if(!bInput && bCheck)
		sMark = bCover ? _T("K6") : _T("K3");
	
	// Sym.RebarMarkPointSide(&Dom, xy, disUpper, disLower, sMark, sDia, bLeft, 0, CDPoint(0,0));

	pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_HORI_1)? pGW->m_pArrRebar[GW_REBAR_HORI_1] : NULL;

	mark_input_data.mark = sMark;
	mark_input_data.dia = sDia;
	mark_input_data.position = xy;
	mark_input_data.ctc = disUpper;
	mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
	mark_input_data.height	= 0;
	mark_input_data.left_dir = bLeft;
	mark_input_data.dist_left_mark = disUpper;
	mark_input_data.dist_right_mark = disLower;

	mark_view_data.count_mark = TRUE;
	mark_view_data.line_rebar = FALSE;
	mark_view_data.mark_pos = eMarkSide;

	Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	
	if(bCover)
	{
		// SR_6
		tv = tvArrRebar.GetAt(nSize-1);
		vUnit = (tv.m_v1 - tv.m_v2).Unit();
		dLength = ~(tv.m_v1 - tv.m_v2) * 0.5;
		xy = tv.m_v2+(vUnit*dLength);
		sDia  = pGW->GetStringDia(pGW->m_dSR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dSR);
		sCTC.Format(_T("C.T.C %.0f"), pGW->m_dCTC * pGW->m_dEC);
		
		if(bLeft)
			sMark = bInput ? _T("SR") : _T("KL7");
		else
			sMark = bInput ? _T("SR") : _T("KR7");
			
		if(!bInput && bCheck)
			sMark = _T("K7");

		// Sym.RebarMarkLineUpper(&Dom, xy, sMark, sDia, bLeft, Dom.Always(5), TRUE, FALSE, sCTC);

		pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_SHEAR_1)? pGW->m_pArrRebar[GW_REBAR_SHEAR_1] : NULL;

		mark_input_data.mark = sMark;
		mark_input_data.dia = sDia;
		mark_input_data.position = xy;
		mark_input_data.ctc = pGW->m_dCTC;
		mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
		mark_input_data.height	= Dom.Always(5);
		mark_input_data.left_dir = bLeft;
		mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;
		
		mark_view_data.count_mark = TRUE;
		mark_view_data.line_rebar = TRUE;
		mark_view_data.mark_pos = eMarkUpper;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}
	
	*pDomP << Dom;
}

void CDrawCrossSection::DimRebarFootwayBlock(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArr, BOOL bInput, BOOL bCover)
{
	CDPoint vOrigin = pGW->IsLeftGuardFence() ? tvArr.GetAt(tvArr.GetSize()-7).m_v2 : tvArr.GetAt(1).m_v1;
	
	// 외형
	double dD  = pGW->m_dDistFrLeft;
	double dH1 = pGW->m_dH1;
	double dH2 = pGW->m_dH2;
	double dH3 = pGW->m_dH3;
	double dW1 = pGW->m_dW1;
	double dW3 = pGW->m_dW3;
	double dW5 = pGW->m_dW5;
	double dW6 = pGW->m_dW6;
	double dW7 = pGW->m_dW7;
	
	// 배근
	double ddc1 = pGW->m_ddc1;
	double ddc2 = pGW->m_ddc2;
	double ddc3 = pGW->m_ddc3;
	double dE1  = pGW->m_dF1;
	double dE2  = pGW->m_dF2;
	double dE3  = pGW->m_dF3;
	double dE4  = pGW->m_dF;
	double dL1  = pGW->m_dL1;
	double heightTotal = pGW->GetHeight();
	double dW	= pGW->GetWidth();

	double dBottomWidth = dW-ddc2-ddc3;

	CString str = _T("");
	CDomyun Dom(pDomP);
	Dom.SetScaleDim(20);
		
	double dDimX = 0;
	long nDimDan = 0;

	if(pGW->IsLeftGuardFence())
	{
		// 우측
		nDimDan = 1;
		Dom.SetDirection("RIGHT");
		
		Dom.DimMoveTo(dDimX, 0, nDimDan);
		bInput ? str.Format(_T("E4@%.0f=%.0f"), (dH1-ddc3)/dE4, dH1-ddc3) : str.Format(_T("%.0f@%.0f=%.0f"), dE4, (dH1-ddc3)/dE4, dH1-ddc3);
		Dom.DimLineTo(dH1-ddc3, nDimDan, str);
		bInput ? Dom.DimLineToOverExtend(ddc3, nDimDan, _T("DC3"), COMMA(ddc3)) : Dom.DimLineToOver(ddc3, nDimDan, COMMA(ddc3));

		Dom.DimMoveTo(dDimX, 0, nDimDan+1);
		Dom.DimLineTo(dH1, nDimDan+1, COMMA(dH1));
		Dom.DimLineTo(dH3, nDimDan+1, COMMA(dH3));

		//높이 2개 이상이 "0"보다 클 경우
		if((dH1 > 0 && dH2 > 0) || (dH2 > 0 && dH3 > 0) || (dH3 > 0 && dH1 > 0))
		{
			Dom.DimMoveTo(dDimX, 0, nDimDan+2);
			Dom.DimLineTo(dH1+dH3, nDimDan+2, COMMA(dH1+dH3));
		}
		
		// 좌측
		nDimDan = 6;
		Dom.SetDirection("LEFT");

		Dom.DimMoveTo(dDimX, 0, nDimDan);
		bInput ? str.Format(_T("E2@%.0f=%.0f"), (dH1-ddc3)/dE2, dH1-ddc3) : str.Format(_T("%.0f@%.0f=%.0f"), dE2, (dH1-ddc3)/dE2, dH1-ddc3);
		Dom.DimLineTo(dH1-ddc3, nDimDan, str);
		bInput ? str.Format(_T("E1@%.0f=%.0f"), (dH3+ddc3-ddc1)/dE1, dH3+ddc3-ddc1) : str.Format(_T("%.0f@%.0f=%.0f"), dE1, (dH3+ddc3-ddc1)/dE1, dH3+ddc3-ddc1);
		Dom.DimLineTo(dH3+ddc3-ddc1, nDimDan, str);
		bInput ? Dom.DimLineToOverExtend(ddc1, nDimDan, _T("DC1"), COMMA(ddc1)) : Dom.DimLineToOver(ddc1, nDimDan, COMMA(ddc1));
		Dom.DimMoveTo(dDimX, 0, nDimDan);
		bInput ? Dom.DimLineToExtend(-dL1, nDimDan, _T("L1"), COMMA(dL1)) : Dom.DimLineTo(-dL1, nDimDan, COMMA(dL1));

		Dom.DimMoveTo(dDimX, 0, nDimDan+1);
		Dom.DimLineTo(heightTotal, nDimDan+1, COMMA(heightTotal));
		
		// 상면    <-----| 왼쪽으로 감
		Dom.SetDirection("TOP");
		nDimDan = 2;
		Dom.DimMoveTo(dDimX, heightTotal, nDimDan);
		Dom.DimLineTo(-dW5, nDimDan, COMMA(dW5));
		Dom.DimLineTo(-dW3, nDimDan, COMMA(dW3));
		nDimDan = 1;
		Dom.DimMoveTo(dDimX, heightTotal, nDimDan);
		Dom.DimLineTo(-ddc3, nDimDan, COMMA(ddc3));
		Dom.DimMoveTo(dDimX, heightTotal, nDimDan);
		if(bCover)
		{
			Dom.DimLineTo(-(dD+dW3+dW5-dW6+50) , nDimDan, COMMA(dD+dW3+dW5-ddc3-dW6+50));
			Dom.DimLineTo(-(dW5-(dD+dW3+dW5-dW6+50)) , nDimDan, COMMA(dW5-(dD+dW3+dW5-dW6+50)));
			Dom.DimLineTo(-ddc3, nDimDan, COMMA(ddc3));
		}
		else
		{
			bInput ? str.Format(_T("E3@%.0f=%.0f"), dW5/dE3, dW5) : str.Format(_T("%.0f@%.0f=%.0f"), dE3, dW5/dE3, dW5);
			Dom.DimLineTo(-dW5-ddc3, nDimDan, str);
		}
		
		Dom.DimLineTo(-dW3+ddc2+ddc3, nDimDan, COMMA(dW3-ddc2-ddc3));
		Dom.DimLineTo(-ddc2, nDimDan, COMMA(ddc2));
		Dom.DimLineToOver(-dD, nDimDan, COMMA(dD));
		
		// 하면  <-----| 왼쪽으로 감
		nDimDan = 1;
		Dom.SetDirection("BOTTOM");		
		Dom.DimMoveTo(dDimX-ddc3, 0, nDimDan);
		bInput ? Dom.DimLineToOver(ddc3, nDimDan, _T("DC3")) : Dom.DimLineToOver(ddc3, nDimDan, COMMA(ddc3));
		Dom.DimMoveTo(dDimX-ddc3, 0, nDimDan);
		Dom.DimLineTo(-dBottomWidth, nDimDan, COMMA(dBottomWidth));
		bInput ? Dom.DimLineToOver(-ddc2, nDimDan, _T("DC2"), 2) : Dom.DimLineToOver(-ddc2, nDimDan, COMMA(ddc2), 2);
		Dom.DimMoveTo(dDimX, 0, nDimDan+1);
		Dom.DimLineTo(-(ddc3+dBottomWidth+ddc2), nDimDan+1, COMMA(ddc3+dBottomWidth+ddc2));
	}
	else
	{	
		// 우측
		nDimDan = 1;
		Dom.SetDirection("LEFT");

		Dom.DimMoveTo(dDimX, 0, nDimDan);
		bInput ? str.Format(_T("E4@%.0f=%.0f"), (dH1-ddc3)/dE4, dH1-ddc3) : str.Format(_T("%.0f@%.0f=%.0f"), dE4, (dH1-ddc3)/dE4, dH1-ddc3);
		Dom.DimLineTo(dH1-ddc3, nDimDan, str);
		bInput ? Dom.DimLineToOverExtend(ddc3, nDimDan, _T("DC3"), COMMA(ddc3)) : Dom.DimLineToOver(ddc3, nDimDan, COMMA(ddc3));
				
		Dom.DimMoveTo(dDimX, 0, nDimDan+1);
		Dom.DimLineTo(dH1, nDimDan+1, COMMA(dH1));
		Dom.DimLineTo(dH3, nDimDan+1, COMMA(dH3));

		//높이 2개 이상이 "0"보다 클 경우
		if((dH1 > 0 && dH2 > 0) || (dH2 > 0 && dH3 > 0) || (dH3 > 0 && dH1 > 0))
		{
			Dom.DimMoveTo(dDimX, 0, nDimDan+2);
			Dom.DimLineTo(dH1+dH3, nDimDan+2, COMMA(dH1+dH3));
		}
		
		// 좌측
		nDimDan = 6;
		Dom.SetDirection("RIGHT");

		Dom.DimMoveTo(dDimX, 0, nDimDan);
		bInput ? str.Format(_T("E2@%.0f=%.0f"), (dH1-ddc3)/dE2, dH1-ddc3) : str.Format(_T("%.0f@%.0f=%.0f"), dE2, (dH1-ddc3)/dE2, dH1-ddc3);
		Dom.DimLineTo(dH1-ddc3, nDimDan, str);
		bInput ? str.Format(_T("E1@%.0f=%.0f"), (dH3+ddc3-ddc1)/dE1, dH3+ddc3-ddc1) : str.Format(_T("%.0f@%.0f=%.0f"), dE1, (dH3+ddc3-ddc1)/dE1, dH3+ddc3-ddc1);
		Dom.DimLineTo(dH3+ddc3-ddc1, nDimDan, str);
		bInput ? Dom.DimLineToOverExtend(ddc1, nDimDan, _T("DC1"), COMMA(ddc1)) : Dom.DimLineToOver(ddc1, nDimDan, COMMA(ddc1));
		Dom.DimMoveTo(dDimX, 0, nDimDan);
		bInput ? Dom.DimLineToExtend(-dL1, nDimDan, _T("L1"), COMMA(dL1)) : Dom.DimLineTo(-dL1, nDimDan, COMMA(dL1));

		Dom.DimMoveTo(dDimX, 0, nDimDan+1);
		Dom.DimLineTo(heightTotal, nDimDan+1, COMMA(heightTotal));
		
		// 상면    <-----| 왼쪽으로 감
		Dom.SetDirection("TOP");
		nDimDan = 2;
		Dom.DimMoveTo(dDimX+dW1+dW3, heightTotal, nDimDan);
		Dom.DimLineTo(-dW3, nDimDan, COMMA(dW1));
		Dom.DimLineTo(-dW1, nDimDan, COMMA(dW1));
		nDimDan = 1;
		Dom.DimMoveTo(dDimX+dW1+dW3, heightTotal, nDimDan);
		Dom.DimLineTo(-ddc3, nDimDan, COMMA(ddc3));
		Dom.DimMoveTo(dDimX+dW1+dW3-ddc3, heightTotal, nDimDan);
		Dom.DimLineTo(-dW3+ddc2+ddc3, nDimDan, COMMA(dW3-ddc2-ddc3));
		if(bCover)
		{
			Dom.DimLineTo(-ddc2, nDimDan, COMMA(ddc2));
			Dom.DimLineTo(-(dW1-dW6-dW7-50) , nDimDan, COMMA(dW1-dW6-dW7-50));
			Dom.DimLineTo(-(dW1-(dW1-dW6-dW7-50)-ddc2) , nDimDan, COMMA(dW1-(dW1-dW6-dW7-50)-ddc2));
		}
		else
		{
			bInput ? str.Format(_T("E3@%.0f=%.0f"), dW1/dE3, dW1) : str.Format(_T("%.0f@%.0f=%.0f"), dE3, dW1/dE3, dW1);
			Dom.DimLineTo(-dW1-ddc3, nDimDan, str);
		}
		Dom.DimLineTo(-ddc2, nDimDan, COMMA(ddc2));
		 
		// 하면  <-----| 왼쪽으로 감
		Dom.SetDirection("BOTTOM");	
		nDimDan = 1;
		Dom.DimMoveTo(dDimX+ddc2+dBottomWidth, 0, nDimDan);
		bInput ? Dom.DimLineToOver(ddc3, nDimDan, _T("DC3")) : Dom.DimLineToOver(ddc3, nDimDan, COMMA(ddc3));
		Dom.DimMoveTo(dDimX+ddc2+dBottomWidth, 0, nDimDan);
		Dom.DimLineTo(-dBottomWidth, nDimDan, COMMA(dBottomWidth));
		bInput ? Dom.DimLineToOver(-ddc2, nDimDan, _T("DC2"), 2) : Dom.DimLineToOver(-ddc2, nDimDan, COMMA(ddc2), 2);
		Dom.DimMoveTo(dDimX, 0, nDimDan+1);
		Dom.DimLineTo(ddc2+dBottomWidth+ddc3, nDimDan+1, COMMA(ddc3+dBottomWidth+ddc2));
	}		
		
	// Dom Move
	Dom.Move(vOrigin.x, vOrigin.y);
	*pDomP << Dom;
}

void CDrawCrossSection::DrawRebarTotalHole(CDomyun* pDomP, long nHDan, BOOL bInclPave, BOOL bInclFootway, BOOL bInput, BOOL bCover)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	CRcBridgeRebar *pBriRebar = m_pStd->m_pRcBridgeRebar;
	
	CDPoint vAng(0,1);
	double sta = pBri->GetStationBridgeMid();
	CJijum* pJL = pBri->GetJijum(0);
	if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pJL->m_dLengthUgan[1];

	CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan);
	if(!pGW)	return;

	// 외형 Arr 그리기
	long nHDanAct = pBri->GetNumSlabNodeAct(sta, vAng, nHDan);
	double dLenHDan = pBri->GetLengthHDanAct(sta, vAng, nHDan);

	CTwinVectorArray tvArrRect;
	tvArrRect.RemoveAll();	
	CDPoint A[30];
	pBri->GetXySlabUpperAct(sta, vAng, A);
	pBri->GetTvCrossSection(tvArrRect, sta, vAng, FALSE, TRUE, TRUE, 0, FALSE, FALSE);
	CDRect rect = tvArrRect.GetRect();
	rect.left = A[nHDanAct].x - 800.0;
	rect.top += 100.0;
	rect.right = A[nHDanAct].x + dLenHDan + 800.0;
	rect.bottom -= 100.0;

	CDomyun Dom(pDomP);
	
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	m_pStd->DrawCuttingTvArray(&Dom, tvArrRect, rect);
	
	*pDomP << Dom;
	
	// 배근 Arr	
	CTwinVectorArray tvArr;
	CTwinVectorArray tvArrRebar;
	CDPointArray xyPointRebar;
	tvArr.RemoveAll();
	tvArrRebar.RemoveAll();	
	xyPointRebar.RemoveAll();

	pBri->GetTvGuardWall(pGW, tvArr, sta, vAng);
	pBriRebar->m_Util.GetTvRebarGuardWall(pGW, tvArr, tvArrRebar, xyPointRebar, Dom.GetScaleDim(), bCover);	
	
	// 철근이 똑같은 방호벽이 있는지 검사
	long nQtyHDan		= pBri->GetQtyHDan();
	CGuardWallRC* pCurrGW = pBri->GetGuardWallByHDan(nHDan);
	BOOL bCheck = FALSE;
	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		if(i!=nHDan)
		{
			CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i); 
			if(!pGW)	continue;
			if(pGW->IsTypeHDanRailTotalHole())
			{
				if(*pCurrGW == *pGW)
					bCheck = TRUE;
			}
		}
	}

	// Arr 그리기
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	m_pStd->DrawTvArray(&Dom, tvArrRebar);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);
	m_pStd->DrawPointRebar(&Dom, xyPointRebar);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	DrawRebarMarkTotalHole(&Dom, pGW, tvArrRebar, xyPointRebar, bInput, bCheck, bCover);
	DimRebarTotalHole(&Dom, pGW, tvArr, bInput, bCover);

	*pDomP << Dom;
}

void CDrawCrossSection::DrawRebarMarkTotalHole(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArrRebar, CDPointArray& xyPointRebar, BOOL bInput, BOOL bCheck, BOOL bCover)
{
	CDomyun Dom(pDomP);
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	
	BOOL bLeft = pGW->IsLeftGuardFence();
	CDPoint xy(0,0);
	CString sDia(_T("")), sMark(_T("")), sCTC(_T(""));
	CTwinVector tv;
	CDPoint vUnit(0,0);
	double dLength = 0;	
	double dMargin = 50;

	CRebar *pRB = NULL;

	SRebarMarkInputData mark_input_data;
	SRebarMarkviewData mark_view_data;
	
	// VR_1
	tv = tvArrRebar.GetAt(2);
	vUnit = (tv.m_v1 - tv.m_v2).Unit();
	dLength = ~(tv.m_v1 - tv.m_v2) * 0.5;
	xy = tv.m_v2+(vUnit*dLength);
	sDia  = pGW->GetStringDia(pGW->m_dVR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dVR);
	sCTC.Format(_T("C.T.C %.0f"), pGW->m_dCTC);
	if(bLeft)
	{
		sMark = bInput ? _T("VR") : _T("NL1");
		if(!bInput && bCheck)
			sMark = _T("N1");
		// Sym.RebarMarkLineUpper(&Dom, xy, sMark, sDia, bLeft, Dom.Always(12), TRUE, FALSE, sCTC);
	}
	else
	{
		sMark = bInput ? _T("VR") : _T("NR1");
		if(!bInput && bCheck)
			sMark = _T("N1");
		// Sym.RebarMarkLineUpper(&Dom, xy, sMark, sDia, !bLeft, Dom.Always(12), TRUE, FALSE, sCTC);
	}

	pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_1)? pGW->m_pArrRebar[GW_REBAR_VERT_1] : NULL;

	mark_input_data.mark = sMark;
	mark_input_data.dia = sDia;
	mark_input_data.position = xy;
	mark_input_data.ctc = pGW->m_dCTC;
	mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
	mark_input_data.height	= Dom.Always(12);
	mark_input_data.left_dir = TRUE;
	mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

	mark_view_data.count_mark = TRUE;
	mark_view_data.line_rebar = TRUE;
	mark_view_data.mark_pos = eMarkUpper;

	Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

	// VR_2
	tv = bCover ? tvArrRebar.GetAt(5) : tvArrRebar.GetAt(6);
	vUnit = (tv.m_v1 - tv.m_v2).Unit();
	dLength = ~(tv.m_v1 - tv.m_v2) * 0.5;
	xy = tv.m_v2+(vUnit*dLength);
	sDia  = pGW->GetStringDia(pGW->m_dVR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dVR);
	sCTC.Format(_T("C.T.C %.0f"), pGW->m_dCTC);
	if(bLeft)
		sMark = bInput ? _T("VR") : bCover ? _T("NL4") :_T("NL2");
	else
		sMark = bInput ? _T("VR") : bCover ? _T("NR4") :_T("NR2");
	if(!bInput && bCheck)
		sMark =  bCover ? _T("N4") :_T("N2");
	// Sym.RebarMarkLineSide(&Dom, xy, sMark, sDia, !bLeft, 0, TRUE, sCTC);

	pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_VERT_2)? pGW->m_pArrRebar[GW_REBAR_VERT_2] : NULL;

	mark_input_data.mark = sMark;
	mark_input_data.dia = sDia;
	mark_input_data.position = xy;
	mark_input_data.ctc = pGW->m_dCTC;
	mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
	mark_input_data.height	= 0;
	mark_input_data.left_dir = !bLeft;
	mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

	mark_view_data.count_mark = TRUE;
	mark_view_data.line_rebar = TRUE;
	mark_view_data.mark_pos = eMarkSide;

	Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

	// HR_5
	CDPoint xyTarget = bLeft ? CDPoint(tvArrRebar.GetXyLeft().x - pGW->m_dL1*0.5, tvArrRebar.GetXyTop().y - dMargin)
							 : CDPoint(tvArrRebar.GetXyRight().x + pGW->m_dL1*0.5, tvArrRebar.GetXyTop().y - dMargin);

	long nSizePoint = xyPointRebar.GetSize();
	xy = xyPointRebar.GetAt(nSizePoint-3);
	Dom.MoveTo(xy);
	Dom.LineTo(xyTarget);
	if(nSizePoint>1)
	{
		xy = xyPointRebar.GetAt(nSizePoint-4);
		Dom.MoveTo(xy);
		Dom.LineTo(xyTarget);	
	}
	sDia  = pGW->GetStringDia(pGW->m_dHR);//m_pStd->m_pARcBridgeDataStd->GetStringDia(pGW->m_dHR);
	if(bLeft)
		sMark = bInput ? _T("HR") : bCover ? _T("NL5") : _T("NL3");
	else
		sMark = bInput ? _T("HR") : bCover ? _T("NR5") : _T("NR3");
		
	if(!bInput && bCheck)
		sMark = bCover ? _T("N5") :_T("N3");

	// Sym.RebarMarkLineSide(&Dom, xyTarget, sMark, sDia, bLeft, 0, FALSE);

	pRB = (pGW->m_pArrRebar.GetSize() > GW_REBAR_HORI_1)? pGW->m_pArrRebar[GW_REBAR_HORI_1] : NULL;

	mark_input_data.mark = sMark;
	mark_input_data.dia = sDia;
	mark_input_data.position = xyTarget;
	mark_input_data.ctc = pGW->m_dFH;
	mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
	mark_input_data.height	= 0;
	mark_input_data.left_dir = bLeft;
	mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCTC;

	mark_view_data.count_mark = FALSE;
	mark_view_data.line_rebar = TRUE;
	mark_view_data.mark_pos = eMarkSide;
	
	*pDomP << Dom;
}

void CDrawCrossSection::DimRebarTotalHole(CDomyun* pDomP, CGuardWallRC* pGW, CTwinVectorArray& tvArr, BOOL bInput, BOOL bCover)
{
	CDPoint vOrigin = pGW->IsLeftGuardFence() ? tvArr.GetAt(tvArr.GetSize()-7).m_v2 : tvArr.GetAt(1).m_v1;
	
	// 외형
	double dD  = pGW->m_dDistFrLeft;
	double dW3 = pGW->m_dW3;
	double dW6 = pGW->m_dW6;
	double dW7 = pGW->m_dW7;
	
	// 배근
	double ddc1 = pGW->m_ddc1;
	double ddc2 = pGW->m_ddc2;
	double ddc3 = pGW->m_ddc3;
	double dE1  = pGW->m_dF1;
	double dE2  = pGW->m_dF2;
	double dE3  = pGW->m_dF3;
	double dL1  = pGW->m_dL1;
	double heightTotal = pGW->GetHeight();
	double dW	= pGW->GetWidth();

	double dBottomWidth = dW-ddc2-ddc3;

	CString str = _T("");
	CDomyun Dom(pDomP);
	Dom.SetScaleDim(17);
		
	double dDimX = 0;
	long nDimDan = 0;

	if(pGW->IsLeftGuardFence())
	{
		// 우측
		nDimDan = 2;
		Dom.SetDirection("RIGHT");
		
		Dom.DimMoveTo(dDimX, 0, nDimDan);
		bInput ? str.Format(_T("E3@%.0f=%.0f"), (heightTotal-ddc1)/dE3, heightTotal-ddc1)
			   : str.Format(_T("%.0f@%.0f=%.0f"), dE3, (heightTotal-ddc1)/dE3, heightTotal-ddc1);
		Dom.DimLineTo(heightTotal-ddc1, nDimDan, str);
		Dom.DimLineToOver(ddc1, nDimDan, COMMA(ddc1));

		Dom.DimMoveTo(dDimX, 0, nDimDan+1);
		Dom.DimLineTo(heightTotal, nDimDan+1, COMMA(heightTotal));
		
		// 좌측
		nDimDan = 9;
		Dom.SetDirection("LEFT");
		Dom.DimMoveTo(dDimX, 0, nDimDan);
		bInput ? str.Format(_T("E1@%.0f=%.0f"), (heightTotal-ddc1)/dE1, heightTotal-ddc1)
			   : str.Format(_T("%.0f@%.0f=%.0f"), dE1, (heightTotal-ddc1)/dE1, heightTotal-ddc1);
		if(dE1==1 && !bInput)
			str.Format(_T("%.0f"), heightTotal-ddc1);
		Dom.DimLineTo(heightTotal-ddc1, nDimDan, str);
		bInput ? Dom.DimLineToOverExtend(ddc1, nDimDan, _T("DC1"), COMMA(ddc1)) : Dom.DimLineTo(ddc1, nDimDan, COMMA(ddc1));
		Dom.DimMoveTo(dDimX, 0, nDimDan);
		bInput ? Dom.DimLineToExtend(-dL1, nDimDan, _T("L1"), COMMA(dL1)) : Dom.DimLineTo(-dL1, nDimDan, COMMA(dL1));
		
		// 상면    <-----| 왼쪽으로 감
		Dom.SetDirection("TOP");
		nDimDan = 2;
		Dom.DimMoveTo(dDimX, heightTotal, nDimDan);
		Dom.DimLineTo(-dW3, nDimDan, COMMA(dW3));
		nDimDan = 1;
		Dom.DimMoveTo(dDimX, heightTotal, nDimDan);
		Dom.DimLineTo(-ddc3, nDimDan, COMMA(ddc3));
		double dLength = dW3-ddc3-dW6+ddc2;
		if(!bCover)
		{
			bInput ? str.Format(_T("E2@%.0f=%.0f"), dLength/dE2, dLength) : str.Format(_T("%.0f@%.0f=%.0f"), dE2, dLength/dE2, dLength);
			Dom.DimLineTo(-dLength, nDimDan, str);
		}
		else
			Dom.DimLineTo(-dLength, nDimDan, COMMA(dLength));
		Dom.DimLineTo(-(dW6-ddc2-ddc2), nDimDan, COMMA(dW6-ddc2-ddc2));
		Dom.DimLineTo(-ddc2, nDimDan, COMMA(ddc2));
		if(dD>0)
			Dom.DimLineToOver(-dD, nDimDan, COMMA(dD), 2);
		
		// 하면  <-----| 왼쪽으로 감
		nDimDan = 1;
		Dom.SetDirection("BOTTOM");		
		Dom.DimMoveTo(dDimX-ddc3, 0, nDimDan);
		bInput ? Dom.DimLineToOver(ddc3, nDimDan, _T("DC3")) : Dom.DimLineToOver(ddc3, nDimDan, COMMA(ddc3));
		Dom.DimMoveTo(dDimX-ddc3, 0, nDimDan);
		Dom.DimLineTo(-dBottomWidth, nDimDan, COMMA(dBottomWidth));
		bInput ? Dom.DimLineToOver(-ddc2, nDimDan, _T("DC2"), 2) : Dom.DimLineToOver(-ddc2, nDimDan, COMMA(ddc2), 2);
		Dom.DimMoveTo(dDimX, 0, nDimDan+1);
		Dom.DimLineTo(-(ddc3+dBottomWidth+ddc2), nDimDan+1, COMMA(ddc3+dBottomWidth+ddc2));
	}
	else
	{	
		// 우측
		nDimDan = 2;
		Dom.SetDirection("LEFT");
		
		Dom.DimMoveTo(dDimX, 0, nDimDan);
		bInput ? str.Format(_T("E3@%.0f=%.0f"), (heightTotal-ddc1)/dE3, heightTotal-ddc1)
			   : str.Format(_T("%.0f@%.0f=%.0f"), dE3, (heightTotal-ddc1)/dE3, heightTotal-ddc1);
		Dom.DimLineTo(heightTotal-ddc1, nDimDan, str);
		Dom.DimLineToOver(ddc1, nDimDan, COMMA(ddc1));

		Dom.DimMoveTo(dDimX, 0, nDimDan+1);
		Dom.DimLineTo(heightTotal, nDimDan+1, COMMA(heightTotal));
		
		// 좌측
		nDimDan = 9;
		Dom.SetDirection("RIGHT");
		Dom.DimMoveTo(dDimX, 0, nDimDan);
		bInput ? str.Format(_T("E1@%.0f=%.0f"), (heightTotal-ddc1)/dE1, heightTotal-ddc1)
			   : str.Format(_T("%.0f@%.0f=%.0f"), dE1, (heightTotal-ddc1)/dE1, heightTotal-ddc1);
		if(dE1==1 && !bInput)
			str.Format(_T("%.0f"), heightTotal-ddc1);
		Dom.DimLineTo(heightTotal-ddc1, nDimDan, str);
		bInput ? Dom.DimLineToOverExtend(ddc1, nDimDan, _T("DC1"), COMMA(ddc1)) : Dom.DimLineTo(ddc1, nDimDan, COMMA(ddc1));
		Dom.DimMoveTo(dDimX, 0, nDimDan);
		bInput ? Dom.DimLineToExtend(-dL1, nDimDan, _T("L1"), COMMA(dL1)) : Dom.DimLineTo(-dL1, nDimDan, COMMA(dL1));
		
		// 상면    |-----> 오른쪽으로 감
		Dom.SetDirection("TOP");
		nDimDan = 1;
		Dom.DimMoveTo(dDimX, heightTotal, nDimDan);
		Dom.DimLineTo(ddc3, nDimDan, COMMA(ddc3));
		
		double dLength = dW6-ddc2+dW7+ddc3;
		if(!bCover)
		{
			bInput ? str.Format(_T("E2@%.0f=%.0f"), dLength/dE2, dLength) : str.Format(_T("%.0f@%.0f=%.0f"), dE2, dLength/dE2, dLength);
			Dom.DimLineTo(dLength, nDimDan, str);
		}
		else
			Dom.DimLineTo(dLength, nDimDan, COMMA(dLength));
		Dom.DimLineTo(dW3-dW6-dW7-ddc3-ddc3, nDimDan, COMMA(dW3-dW6-dW7-ddc3-ddc3));
		Dom.DimLineTo(ddc3, nDimDan, COMMA(ddc3));
		if(dD>0)
			Dom.DimLineToOver(dD, nDimDan, COMMA(dD));

		Dom.DimMoveTo(dDimX, heightTotal, nDimDan+1);
		Dom.DimLineTo(dW3, nDimDan+1, COMMA(dW3));
		 
		// 하면  <-----| 왼쪽으로 감
		Dom.SetDirection("BOTTOM");	
		nDimDan = 1;
		Dom.DimMoveTo(dDimX+ddc2+dBottomWidth, 0, nDimDan);
		bInput ? Dom.DimLineToOver(ddc3, nDimDan, _T("DC3")) : Dom.DimLineToOver(ddc3, nDimDan, COMMA(ddc3));
		Dom.DimMoveTo(dDimX+ddc2+dBottomWidth, 0, nDimDan);
		Dom.DimLineTo(-dBottomWidth, nDimDan, COMMA(dBottomWidth));
		bInput ? Dom.DimLineToOver(-ddc2, nDimDan, _T("DC2"), 2) : Dom.DimLineToOver(-ddc2, nDimDan, COMMA(ddc2), 2);
		Dom.DimMoveTo(dDimX, 0, nDimDan+1);
		Dom.DimLineTo(ddc2+dBottomWidth+ddc3, nDimDan+1, COMMA(ddc3+dBottomWidth+ddc2));
	}		
		
	// Dom Move
	Dom.Move(vOrigin.x, vOrigin.y);
	*pDomP << Dom;
}

void CDrawCrossSection::MakeRebarList(CTypedPtrArray <CObArray, CRebar*>  &pArrRebar, CGuardWallRC *pGW)
{
	//clear
	long nSize = pArrRebar.GetSize();
	long i=0; for(i=0; i<nSize; i++) 
		delete pArrRebar[i];
	pArrRebar.RemoveAll();

	//배열 정리
//	CRebar *pRB = NULL;
	nSize = pGW->m_pArrRebar.GetSize();
	for(long nList=0; nList<nSize; nList++)
	{
		CRebar *pRB = pGW->m_pArrRebar.GetAt(nList);
		if(pRB->IsTrue()) pArrRebar.Add(new CRebar(pRB));
	}
}

// 횡단면 상부슬래브 주철근(점)
// BOOL bUseMatchSta : sta와 일치하는 위치의 철근들을 사용함(확폭의 경우 실제 sta 횡단면을 그리기 때문)
// 확폭이 아니라면 어딜 그리나 마찬가지 이므로 미리 구해진 시점부 철근을 사용한다. 
void CDrawCrossSection::DrawRebarMain_CrossSection_UpperSlab(CDomyun *pDomP, double sta, BOOL bUpper, long nDan, BOOL bMarkRebar, BOOL bDimRebar, long nDimDan, long nCycle, long nDrawLeft, BOOL bExp, BOOL bExcludeDoupleMainRebar, BOOL bUseMatchSta)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(!pBri->IsExp(sta))
		bExp	= FALSE;
	
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	long upper	= bUpper ? iUPPER : iLOWER;

	
	CTwinVectorArray tvArrCrossSection, tvArrCrossSectionHunch;
	if(bUseMatchSta)
	{
		pBri->GetTvCrossSection(tvArrCrossSection, sta, CDPoint(0, 1), FALSE, TRUE, TRUE, 0, FALSE, TRUE, FALSE, bExp);
		pBri->GetTvCrossHunch(tvArrCrossSectionHunch, sta, CDPoint(0, 1), FALSE, 0, FALSE, bExp);
	}
	else
	{
		pBri->GetTvCrossSection(tvArrCrossSection, pBri->m_dStationBridgeStt, CDPoint(0, 1), FALSE, TRUE, TRUE, 0);
		pBri->GetTvCrossHunch(tvArrCrossSectionHunch, pBri->m_dStationBridgeStt, CDPoint(0, 1), FALSE, 0, FALSE, FALSE);
	}
	CDRect rectCross(0, 0, 0, 0), rectHunch(0, 0, 0, 0);
	rectCross	= tvArrCrossSection.GetRect();
	rectHunch	= tvArrCrossSectionHunch.GetRect();
	CDRect rect	= m_pStd->MergeRectAndRect(rectCross, rectHunch);


	// 철근 가져와서 그리기(상부슬래브)
	long leftSta	= Compare(pBri->m_dStaLeftCross, sta, "=") ? iLEFT : iRIGHT;
	CHgBaseDrawStd baseDraw;
	CRebarPlacing rb;
	CVectorArray xyArr, xyArrHunch;
	if(bUseMatchSta)
	{
		pBri->GetXyRebarMain_CrossSection_UpperSlab(xyArr, xyArrHunch, sta, bUpper, nDan, nCycle, TRUE, bUseMatchSta);
	}
	else
	{
		xyArr	= pBri->m_xyArrMainRebarCross[nCycle][upper][nDan][leftSta];
		xyArrHunch	= pBri->m_xyArrMainRebarCrossHunch[nCycle][leftSta];
	}

	xyArr.Sort();
	xyArrHunch.Sort();

	CDPointArray xyArrCycle1Exp[2], xyArrCycle2Exp[2];
	if(bExp)
	{
		pBri->GetXyRebarMain_CrossSection_SlabExp(xyArrCycle1Exp[iLEFT], xyArrCycle2Exp[iLEFT], TRUE, TRUE, TRUE, bUpper, nDan);
		pBri->GetXyRebarMain_CrossSection_SlabExp(xyArrCycle1Exp[iRIGHT], xyArrCycle2Exp[iRIGHT], TRUE, TRUE, FALSE, bUpper, nDan);
	}

	// 치수 기입시 치수 기입용으로 따로 구해놓음
	CDPointArray xyArrForDim[3], xyArrHunchForDim[3];
	CDPointArray xyArrForDimExp1, xyArrForDimExp2;
	if(bDimRebar)
	{
		if(bUseMatchSta)
		{
			for(long cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
			{
				CVectorArray vecArrTmp, vecArrHunchTmp;
				pBri->GetXyRebarMain_CrossSection_UpperSlab(vecArrTmp, vecArrHunchTmp, sta, bUpper, nDan, cycle, TRUE, bUseMatchSta);

				// [3] : 0 :좌, 1 : 기본, 2:우
				CVector vec(0, 0, 0);
				long i = 0; for(i = 0; i < vecArrTmp.GetSize(); i++)
				{
					vec	= vecArrTmp.GetAt(i);
					xyArrForDim[(long)(vec.z)+1].Add(vec);
				}

				for(i = 0; i < vecArrHunchTmp.GetSize(); i++)
				{
					vec	= vecArrHunchTmp.GetAt(i);
					xyArrHunchForDim[(long)(vec.z)+1].Add(vec);
				}
			}			
		}
		else
		{
			for(long cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
			{
				// [3] : 0 :좌, 1 : 기본, 2:우
				CVector vec(0, 0, 0);
// 				long nIdx = 0;
				long i = 0; for(i = 0; i < pBri->m_xyArrMainRebarCross[cycle][upper][nDan][leftSta].GetSize(); i++)
				{
					vec	= pBri->m_xyArrMainRebarCross[cycle][upper][nDan][leftSta].GetAt(i);
					long nIdx = (long)(vec.z)+1;
					xyArrForDim[nIdx].Add(vec);
				}

				for(i = 0; i < pBri->m_xyArrMainRebarCrossHunch[cycle][leftSta].GetSize(); i++)
				{
					vec	= pBri->m_xyArrMainRebarCrossHunch[cycle][leftSta].GetAt(i);
					xyArrHunchForDim[(long)(vec.z)+1].Add(vec);
				}
			}
		}
		

		if(bExp && pBri->m_bIsExp)
		{
			pBri->GetXyRebarMain_CrossSection_SlabExp(xyArrForDimExp1, xyArrForDimExp2, TRUE, TRUE, TRUE, bUpper, nDan);
			long i = 0; for(i = 0; i < xyArrForDimExp1.GetSize(); i++)
				xyArrForDim[1].Add(xyArrForDimExp1.GetAt(i));
			for(i = 0; i < xyArrForDimExp2.GetSize(); i++)
				xyArrForDim[1].Add(xyArrForDimExp2.GetAt(i));

			pBri->GetXyRebarMain_CrossSection_SlabExp(xyArrForDimExp1, xyArrForDimExp2, TRUE, TRUE, FALSE, bUpper, nDan);
			for(i = 0; i < xyArrForDimExp1.GetSize(); i++)
				xyArrForDim[1].Add(xyArrForDimExp1.GetAt(i));
			for(i = 0; i < xyArrForDimExp2.GetSize(); i++)
				xyArrForDim[1].Add(xyArrForDimExp2.GetAt(i));
		}

		long s = 0; for(s = 0; s < 3; s++)
		{
			xyArrForDim[s].Sort();
			xyArrHunchForDim[s].Sort();
			rb.RemoveSamePoint(xyArrForDim[s], TRUE);
			rb.RemoveSamePoint(xyArrHunchForDim[s], TRUE);
		}
	}
	
	// 헌치부가 있는지...
	BOOL bIsHunch	= xyArrHunch.GetSize() > 0;

	// 치수 기입
	long nDimDanAddLower	= 0;
	double dYForDim	= 0;
	if(bDimRebar)
	{
		CDRect rect(0, 0, 0, 0);
		CTwinVectorArray tvArrCross;
		pBri->GetTvCrossSection(tvArrCross, sta, CDPoint(0, 1), TRUE, TRUE, FALSE);
		rect	= tvArrCross.GetRect();
		rect.bottom -= pBri->IsOutWall(TRUE) ? pBri->m_dHHS : pBri->m_dHHS;
		dYForDim	= rect.bottom;

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		CTwinVectorArray tvArr, tvArrSupLeft, tvArrSupRight;
		CTwinVectorArray tvArrRebar, tvArrSupLeftRebar, tvArrSupRightRebar;
		pBri->GetTvRebarSupport_CrossSection_UpperSlab(tvArr, tvArrSupLeft, tvArrSupRight, sta, TRUE, -1, 0, TRUE, FALSE, bExp, 0, TRUE);
		pBri->GetTvRebarSupport_CrossSection_UpperSlab(tvArrRebar, tvArrSupLeftRebar, tvArrSupRightRebar, sta, TRUE, 0, 0, TRUE, FALSE, bExp, 0, TRUE);
		double dAngle	= pBri->GetAngleByTarget(BRIDGE_OFF_STT_SLAB, 0).GetAngleDegree();

		long s=0, s_stt=0, s_end=3; 
		if(nDrawLeft==-1)	// 단부 - 32311
			s_stt = 1;
		if(nDrawLeft==1)		// 일반부 - 32311
			s_end = 1;

		for(s = s_stt; s < s_end; s++)
		{
			long nSize		= xyArrForDim[s].GetSize();
			if(nSize == 0) continue;

			CDPointArray xyArrDim;
			CStringArray sArrText;
			CStringArray sArrUnderText;
			CString sDir	= bUpper ? "TOP" : "BOTTOM";
			CDPoint xyStt	= CDPoint(0, 0);
			nDimDanAddLower	= s == 1 && (xyArrForDim[0].GetSize() > 0 || xyArrForDim[2].GetSize() > 0) ? 1 : 0;
	
			if(s == 1 || s == 2)
			{
				// 사보강 배치시는 기본 철근에 피복 안 넣어줌.
				xyArrDim.Add(tvArr.GetXyLeft());
				if(s == 2 || pBri->m_nTypePlacingMainRebar_UpperSlab == 0)
				{
					xyArrDim.Add(tvArrRebar.GetXyLeft());
				}
			}

			long i = 0; for(i = 0; i < nSize; i++)
			{
				xyStt	= xyArrForDim[s].GetAt(i);
				xyArrDim.Add(xyStt);
			}

			
			if(s == 0 || s == 1)
			{
				// 사보강 배치시는 기본 철근에 피복 안 넣어줌.
				if(s == 0 || pBri->m_nTypePlacingMainRebar_UpperSlab == 0)
				{
					xyArrDim.Add(tvArrRebar.GetXyRight());
				}
				xyArrDim.Add(tvArr.GetXyRight());
			}
			xyArrDim.Sort(TRUE);

			xyStt	= CDPoint(xyArrDim.GetAt(0).x, bUpper ? rect.top : rect.bottom);

			BOOL bIncludeSttRep	= s == 0 || s == 1;
			BOOL bIncludeEndRep	= s == 1 || s == 2;
			CString sMsgStt	= _T("");
			// 사보강 배치면 직철근인지 사철근인지 표시해준다.
			if(0)//pBri->m_nTypePlacingMainRebar_UpperSlab != 0)
			{
				if(s == 2) sMsgStt = _T("(사방향철근)");
				else if(s == 1) sMsgStt = _T("(직방향철근)");
			}
			// 사거리 표현 뺌(사보강배치일때만 뺌)
			// 도저히 정확한 표현이 안되므로, 사용자가 평면도의 사거리를 사용해서 직접 넣기를 권함.
			// 2005.10.08 이혜철차장 & 이재호대리
			if(pBri->m_nTypePlacingMainRebar_UpperSlab == 1 || pBri->m_nTypePlacingMainRebar_UpperSlab == 2)
			{
				dAngle	= 90;
			}
			
			baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan+nDimDanAddLower, sDir, -1, s==1, TRUE, FALSE, TRUE, FALSE, FALSE, bIncludeSttRep, bIncludeEndRep, dAngle, TRUE, sMsgStt, 0.0001);
		}
		nDimDanAddLower	= (xyArrForDim[0].GetSize() > 0 || xyArrForDim[2].GetSize() > 0) ? 1 : 0;
	}

	// ndrawLeft에 따라서 필요한 것만 골라냄.
	if(nDrawLeft != 0)
	{
		double dXMid	= pBri->GetMidCrossSection(TRUE);//
		
		long i = 0; for(i = 0; i < xyArr.GetSize(); i++)
		{
			if((xyArr[i].x < dXMid) && nDrawLeft == -1) continue;
			if((xyArr[i].x > dXMid) && nDrawLeft == 1) continue;
			xyArr.RemoveAt(i, 1);
			i--;
		}

		long left = 0; for(left = 0; left < 2; left++)
		{
			long i = 0; for(i = 0; i < xyArrCycle1Exp[left].GetSize(); i++)
			{
				if((xyArrCycle1Exp[left][i].x < dXMid) && nDrawLeft == -1) continue;
				if((xyArrCycle1Exp[left][i].x > dXMid) && nDrawLeft == 1) continue;
				xyArrCycle1Exp[left].RemoveAt(i, 1);
				i--;
			}
			
			for(i = 0; i < xyArrCycle2Exp[left].GetSize(); i++)
			{
				if((xyArrCycle2Exp[left][i].x < dXMid) && nDrawLeft == -1) continue;
				if((xyArrCycle2Exp[left][i].x > dXMid) && nDrawLeft == 1) continue;
				xyArrCycle2Exp[left].RemoveAt(i, 1);
				i--;
			}
		}
	}

	CDPoint xyDirScale	= bUpper ? CDPoint(0, -1) : CDPoint(0, 1);
	CDPointArray xyArrDraw[2];
	CVector vec(0, 0, 0);
	long i = 0; for(i = 0; i < xyArr.GetSize(); i++)
	{
		vec	= xyArr.GetAt(i);
		xyArrDraw[vec.z == 0 ? 0 : 1].Add(vec);
	}

	// 겹치는 철근 제거
	if(bExcludeDoupleMainRebar)
	{
		if(xyArrDraw[0].GetSize() > 0)
		{
			double dLeftX	= xyArrDraw[0].GetAt(0).x;
			double dRightX	= xyArrDraw[0].GetAt(xyArrDraw[0].GetSize()-1).x;

			CDPoint xy(0, 0);
			for(i = 0; i < xyArrDraw[1].GetSize(); i++)
			{
				xy	= xyArrDraw[1].GetAt(i);
				if(xy.x >= dLeftX && xy.x <= dRightX)
				{
					xyArrDraw[1].RemoveAt(i);
					i--;
				}
			}
		}
	}


	long s = 0; for(s = 0; s < 2; s++)
	{
		baseDraw.DrawPointRebar(&Dom, xyArrDraw[s], xyDirScale, -1, -1, FALSE, s);
	}

	long left = 0; for(left = 0; left < 2; left++)
	{
		baseDraw.DrawPointRebar(&Dom, xyArrCycle1Exp[left], xyDirScale, -1, -1);
		baseDraw.DrawPointRebar(&Dom, xyArrCycle2Exp[left], xyDirScale, -1, -1);
	}

	double dHDiff	= bIsHunch && !bUpper ? pBri->m_dHHS : 0;
	// 마킹
	if(bMarkRebar)
	{
		double dSta	= sta;
		// 지점부 횡단을 그릴때는 슬래브 끝단 sta를 기준으로 그리므로 철근을 못 찾음
		// 그래서 지점부 횡단일 경우 시점으로 옮긴다.
		double dStaSlab	= pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, TRUE);
		CDPoint vAng	= pBri->GetAngleByTarget(BRIDGE_OFF_STT_SLAB, 0);
		if(dSta < dStaSlab+pBri->m_dCoverWallOut[0]/vAng.y)
			dSta	= dStaSlab + pBri->m_dCoverWallOut[0]/vAng.y;
			
		CRebarInfoCycle *pRC	= pBri->GetRCMain_Slab(TRUE, dSta, bUpper, nDan, nCycle);
		if(pRC)
		{
			if(pRC->m_bExist)
			{
				CString sMark	= pRC->m_szMark;
				if(m_pStd->IsMarkRebar(sMark))
				{
					CRebar *pRb = pBri->GetRebarMainByDescription(pRC->m_sDescription);

					CMarkDomyun mark;
					mark.SetDomyun(&Dom, m_pStd->m_pARoadOptionStd);
					mark.SetRebar(xyArrDraw[0]);

					// 설정
					mark.m_sMark					= sMark;
					mark.m_sDia						= pBri->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pBri->GetValueFy(ePartUpperSlab));
					mark.m_nTypeMarkPointUpAndDown	= bUpper ? TYPE_MARKPOINT_UPPER : TYPE_MARKPOINT_LOWER;
					mark.m_nTypeMarkDir				= TYPE_MARKDIR_RIGHT;
					mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
					mark.m_dPosScaleMarkStt			= nCycle == 0 ? 0.3 : 0.6;
					if(pBri->GetCountCycleMainRebar() == 4)
					{
						switch(nCycle)
						{
						case 0: mark.m_dPosScaleMarkStt	= 0.2; break;
						case 1: mark.m_dPosScaleMarkStt	= 0.4; break;
						case 2: mark.m_dPosScaleMarkStt	= 0.6; break;
						case 3: mark.m_dPosScaleMarkStt	= 0.8; break;
						}
					}
					if(bIsHunch && !bUpper)	// 헌치부가 있는 경우 하부 마크는 위치 조정
					{
						mark.m_dPosScaleMarkStt		= nCycle == 0 ? 0.2 : 0.4;
						if(pBri->GetCountCycleMainRebar() == 4)
						{
							switch(nCycle)
							{
							case 0: mark.m_dPosScaleMarkStt	= 0.2; break;
							case 1: mark.m_dPosScaleMarkStt	= 0.4; break;
							case 2: mark.m_dPosScaleMarkStt	= 0.6; break;
							case 3: mark.m_dPosScaleMarkStt	= 0.8; break;
							}
						}
					}
					if(nDan == 1)
					{
						mark.m_dPosScaleMarkStt += 0.1;
						dHDiff += bUpper ? MARK_BASEHEIGHT_UPPER : MARK_BASEHEIGHT_LOWER;
					}
					else if(nDan == 2)
					{
						mark.m_dPosScaleMarkStt += 0.3;
						dHDiff += (bUpper ? MARK_BASEHEIGHT_UPPER : MARK_BASEHEIGHT_LOWER) * 2;
					}
					mark.m_dAddHeight				= dHDiff;
					mark.m_dCtc						= pBri->GetCtcRebarMain(TRUE);
					mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;
					mark.MarkRebar();
				}
			}
		}
	}

	// 헌치부 주철근  ///////////////////////////////////////////////////////////////////
	// 치수 기입
	if(bIsHunch)
	{
		// 헌치 치수는 원래 안 하는 거라고 해서 막음
		// 필요시 풀면 바로 적용됨.
		if(0)//bDimRebar)
		{
			CTwinVectorArray tvArr, tvArrSupLeft, tvArrSupRight;
			pBri->GetTvRebarSupport_CrossSection_UpperSlab(tvArr, tvArrSupLeft, tvArrSupRight, sta, TRUE, -1, 0, TRUE, FALSE, bExp);

			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
			double dAngle	= pBri->GetAngleByTarget(BRIDGE_OFF_STT_SLAB, 0).GetAngleDegree();

			long s = 0; for(s = 0; s < 3; s++)
			{
				long nSize		= xyArrHunchForDim[s].GetSize();
				if(nSize == 0) continue;
				
				CDPointArray xyArrDim;
				CStringArray sArrText;
				CStringArray sArrUnderText;
				CString sDir	= "BOTTOM";
				CDPoint xyStt	= CDPoint(0, 0);
				long nDimDanAdd	= s == 1 && (xyArrHunchForDim[0].GetSize() > 0 || xyArrHunchForDim[2].GetSize() > 0) ? 1 : 0;
				

				if(s == 1 || s == 2)
					xyArrDim.Add(tvArr.GetXyLeft());

				nSize	= xyArrHunchForDim[s].GetSize();
				long i = 0; for(i = 0; i < nSize; i++)
					xyArrDim.Add(xyArrHunchForDim[s].GetAt(i));
				if(s == 0 || s == 1)
					xyArrDim.Add(tvArr.GetXyRight());
				xyArrDim.Sort(TRUE);

				xyStt	= CDPoint(xyArrDim.GetAt(0).x, dYForDim);
				
				baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan+nDimDanAddLower+1+nDimDanAdd, sDir, -1, s==1, TRUE, FALSE, TRUE, FALSE, FALSE, TRUE, TRUE, dAngle);
			}
		}


		// ndrawLeft에 따라서 필요한 것만 골라냄.
		if(nDrawLeft != 0)
		{
			double dXMid	= pBri->GetMidCrossSection(TRUE);//
		
			long i = 0; for(i = 0; i < xyArrHunch.GetSize(); i++)
			{
				if((xyArrHunch[i].x < dXMid) && nDrawLeft == -1) continue;
				if((xyArrHunch[i].x > dXMid) && nDrawLeft == 1) continue;
				xyArrHunch.RemoveAt(i, 1);
				i--;
			}
		}

		// 철근 가져와서 그리기(상부슬래브 헌치부)
		xyArrDraw[0].RemoveAll();
		xyArrDraw[1].RemoveAll();
		for(i = 0; i < xyArrHunch.GetSize(); i++)
		{
			vec	= xyArrHunch.GetAt(i);
			xyArrDraw[vec.z == 0 ? 0 : 1].Add(xyArrHunch[i]);
		}

		// 겹치는 철근 제거
		if(bExcludeDoupleMainRebar)
		{
			if(xyArrDraw[0].GetSize() > 0)
			{
				double dLeftX	= xyArrDraw[0].GetAt(0).x;
				double dRightX	= xyArrDraw[0].GetAt(xyArrDraw[0].GetSize()-1).x;

				CDPoint xy(0, 0);
				for(i = 0; i < xyArrDraw[1].GetSize(); i++)
				{
					xy	= xyArrDraw[1].GetAt(i);
					if(xy.x >= dLeftX && xy.x <= dRightX)
					{
						xyArrDraw[1].RemoveAt(i);
						i--;
					}
				}
			}
		}

		for(s = 0; s < 2; s++)
		{
			baseDraw.DrawPointRebar(&Dom, xyArrDraw[s], xyDirScale, -1, -1, FALSE, s);
		}

		// 마킹
		if(bMarkRebar && xyArrDraw[0].GetSize() > 0)
		{
			CRebarInfoCycle *pRC	= pBri->GetRCMain_SlabHunch(TRUE, sta, nCycle);
			if(pRC)
			{
				CString sMark	= pRC->m_szMark;
				if(m_pStd->IsMarkRebar(sMark))
				{
					CRebar *pRb = pBri->GetRebarMainByDescription(pRC->m_sDescription);

					CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
					mark.SetRebar(xyArrDraw[0]);

					// 설정
					mark.m_sMark					= sMark;
					mark.m_sDia						= pBri->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pBri->GetValueFy(ePartUpperSlab));
					mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_LOWER;
					mark.m_nTypeMarkDir				= TYPE_MARKDIR_RIGHT;
					mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
					mark.m_dPosScaleMarkStt			= nCycle == 0 ? 0.6 : 0.8;
					mark.m_dCtc						= pBri->GetCtcRebarMain(TRUE);
					mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;
					mark.MarkRebar();
				}
			}
		}
	}


	*pDomP << Dom;
}

// 횡단면 배력철근(외곽) - sta에 따라서 헌치가 있으면 헌치도 그림.
// BOOL bUseMatchSta : sta와 일치하는 위치의 철근들을 사용함(확폭의 경우 실제 sta 횡단면을 그리기 때문)
// 확폭이 아니라면 어딜 그리나 마찬가지 이므로 미리 구해진 시점부 철근을 사용한다. 
void CDrawCrossSection::DrawRebarSupport_CrossSection_UpperSlab(CDomyun *pDomP, double sta, BOOL bUpper, long nDan, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft, BOOL bExp, BOOL bUseMatchSta)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	long nMaxDan	= pBri->GetDanMainRebarUpperSlab(0, bUpper, 0);
	if(nDan == -1) nDan = nMaxDan-1;
	if(nDan < 0 || nDan > nMaxDan-1) return;

	if(!pBri->IsExp(sta))
		bExp	= FALSE;


	// 주철근이 없으면 배력근도 그리지 않는다.
	double dStaCheck	= pBri->GetStationModifyForRebarCheck(sta);
	long nIdxFind	= 0;
	if(!pBri->IsRebarMainBySta(TRUE, dStaCheck, 0, bUpper, nDan, nIdxFind)) return;
	///////////////////////////

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CHgBaseDrawStd baseDraw;
	long upper	= bUpper ? iUPPER : iLOWER;
	

	// 상부 슬래브
	// 상면 ///////////////////////////////////////////////////////////////////////
	CTwinVectorArray tvArrUpper, tvArrSupLeft, tvArrSupRight;
	CTwinVectorArray tvArrUpperAll, tvArrSupLeftAll, tvArrSupRightAll;
	pBri->GetTvRebarSupport_CrossSection_UpperSlab(tvArrUpper, tvArrSupLeft, tvArrSupRight, sta, bUpper, nDan, nDrawLeft, TRUE, TRUE, bExp, 0, TRUE);
	if(bMarkRebar)
		pBri->GetTvRebarSupport_CrossSection_UpperSlab(tvArrUpperAll, tvArrSupLeftAll, tvArrSupRightAll, sta, bUpper, nDan, 0, TRUE, TRUE, bExp, 0, TRUE);
	baseDraw.DrawTvArray(&Dom, tvArrUpper, FALSE, FALSE);
	baseDraw.DrawTvArray(&Dom, tvArrSupLeft, FALSE, FALSE);
	baseDraw.DrawTvArray(&Dom, tvArrSupRight, FALSE, FALSE);

	// 상면이든 하면이든 무조건 오른쪽 끝부분에 하면 됨
	CVectorArray xyArr1, xyArrHunch1;	// 1 cycle
	CVectorArray xyArr2, xyArrHunch2;	// 2 cycle
	if(bUseMatchSta)
	{
		pBri->GetXyRebarMain_CrossSection_UpperSlab(xyArr1, xyArrHunch1, sta, bUpper, nDan, 0);
		pBri->GetXyRebarMain_CrossSection_UpperSlab(xyArr2, xyArrHunch2, sta, bUpper, nDan, 0);
	}
	else
	{
		long leftSta	= Compare(pBri->m_dStaLeftCross, sta, "=") ? iLEFT : iRIGHT;
		xyArr1	= pBri->m_xyArrMainRebarCross[0][upper][nDan][leftSta];
		xyArr2	= pBri->m_xyArrMainRebarCross[1][upper][nDan][leftSta];
		xyArrHunch1	= pBri->m_xyArrMainRebarCrossHunch[0][leftSta];
		xyArrHunch2	= pBri->m_xyArrMainRebarCrossHunch[1][leftSta];
	}
	

	BOOL bHunch	= !bUpper && xyArrHunch1.GetSize() > 0;

	// 치수기입
	if(bDimRebar)
	{
		*pDomP << Dom;
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		CTwinVectorArray tvArrCross;
		pBri->GetTvCrossSection(tvArrCross, sta, CDPoint(0, 1), FALSE, FALSE, FALSE, 0, TRUE, FALSE, FALSE, FALSE);

		CDoubleArray dArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		CString sDir	= nDrawLeft == -1 ? "TOP" : "BOTTOM";
		CDPoint xyStt	= nDrawLeft == -1 ? tvArrCross.GetXyRight() : tvArrCross.GetXyLeft();
		xyStt.y	-= pBri->m_dTS;

		double dCoverLower	= pBri->m_dCoverLower_UpperSlab[0];
		double dCoverUpper	= pBri->m_dCoverUpper_UpperSlab[0];
		

		
		
		dArrDim.Add(dCoverUpper);
		dArrDim.Add(pBri->m_dTS - dCoverLower - dCoverUpper);
		dArrDim.Add(dCoverLower);

		if(bHunch)
		{
			long nJ	= pBri->GetJijumNumberByStation(sta);
			double dHeightHunch	= pBri->GetHeightHunchByJijum(nJ);
			xyStt.y	-= dHeightHunch;
			
			dArrDim.Add(dHeightHunch-dCoverLower);
			dArrDim.Add(dCoverLower);
		}

		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, 0, sDir, -1, TRUE, FALSE, FALSE);
		Dom.Rotate(xyStt, CDPoint(0, -1));
		Dom.Move(CDPoint(0, 1) * dArrDim.Sum());
		*pDomP << Dom;
	}
		

	// 철근 마크
	// 배력 철근 마크
	CString sDescription	= _T("");
	if(bUpper)
		sDescription.Format("상부슬래브 상면 %d단 배력철근", nDan+1);
	else
		sDescription.Format("상부슬래브 하면 %d단 배력철근2", nDan+1);
	
	BOOL bSkewSttExpOnly = FALSE, bSkewEndExpOnly = FALSE;
	CString sMark	= pBri->GetMarkRebarSupport(0, FALSE, FALSE, FALSE, sDescription);
	if(sMark == _T("") && pBri->m_bIsExp)
	{
		CTwinVectorArray tvArr, tvArrAngStt, tvArrAngEnd, tvArrRAngStt, tvArrRAngEnd;
		CTwinVectorArray tvArrExpSttLAndR, tvArrExpSttLOrR, tvArrExpEndLAndR, tvArrExpEndLOrR;
		pBri->GetTvRebarSupport_Plane_Slab_Std(TRUE, tvArr, tvArrAngStt, tvArrAngEnd, tvArrRAngStt, tvArrRAngEnd, bUpper, nDan, 0);
		pBri->DivideRebarSupport_Plane_UpperSlabExpSection(tvArr, tvArrExpSttLAndR, tvArrExpSttLOrR, tvArrExpEndLAndR, tvArrExpEndLOrR);

		long nSize				= tvArr.GetSize();
		long nSizeStt_And	= tvArrExpSttLAndR.GetSize();
		long nSizeStt_Or		= tvArrExpSttLOrR.GetSize();
		long nSizeEnd_And = tvArrExpEndLAndR.GetSize();
		long nSizeEnd_Or   = tvArrExpEndLOrR.GetSize();

		long nSizeSttAng	= tvArrAngStt.GetSize();
		long nSizeEndAng	= tvArrAngEnd.GetSize();
		long nSizeRSttAng	= tvArrRAngStt.GetSize();
		long nSizeRAngEnd= tvArrRAngEnd.GetSize();


		if(nSize ==0 && nSizeStt_And ==0 && nSizeStt_Or ==0 && (nSizeSttAng > 0 || nSizeRSttAng > 0))
			bSkewSttExpOnly = TRUE;
		if(nSize ==0 && nSizeEnd_And ==0 && nSizeEnd_Or ==0 && (nSizeEndAng > 0 || nSizeRAngEnd > 0))
			bSkewEndExpOnly = TRUE;


		CString sUpdn = bUpper? _T("상면") : _T("하면");
		
		for (long ang = 1 ; ang < 5; ang++)
		{
			if(ang == 1 && nSizeStt_And == 0) continue;
			else if(ang == 2 && nSizeStt_Or == 0) continue;
			else if(ang == 3 && nSizeEnd_And == 0) continue;
			else if(ang == 4 && nSizeEnd_Or == 0) continue;

			if(bUpper)
				sDescription.Format("상부슬래브 %s %d단 배력철근%d", sUpdn, nDan+1, 5+ang);
			else
				sDescription.Format("상부슬래브 %s %d단 배력철근2_%d", sUpdn, nDan+1, 5+ang);
			sMark	= pBri->GetMarkRebarSupport(0, FALSE, FALSE, FALSE, sDescription);

			if(sMark == _T("")) continue;
			else break;
		}		

		if(sMark == _T("") && (bSkewSttExpOnly || bSkewEndExpOnly))
		{
			CString sMarkStt = _T(""), sMarkEnd = _T("");
			CString sDescriptionStt = _T(""), sDescriptionEnd = _T("");

			for (long nStt = 0 ; nStt <= iEND; nStt++)
			{
				CString sUL = bUpper? _T("상면") : _T("하면");
				if(nStt == iSTT)
				{
					if(bUpper)	sDescriptionStt.Format("상부슬래브 %s %d단 배력철근%d", sUL, nDan+1, nStt+2);
					else			sDescriptionStt.Format("상부슬래브 %s %d단 배력철근2_%d", sUL, nDan+1, nStt+2);
					sMarkStt = pBri->GetMarkRebarSupport(0, FALSE, FALSE, FALSE, sDescriptionStt);
				}
				else
				{
					if(bUpper)	sDescriptionEnd.Format("상부슬래브 %s %d단 배력철근%d",sUL, nDan+1, nStt+2);
					else			sDescriptionEnd.Format("상부슬래브 %s %d단 배력철근2_%d", sUL, nDan+1, nStt+2);
					sMarkEnd = pBri->GetMarkRebarSupport(0, FALSE, FALSE, FALSE, sDescriptionEnd);
				}
			}
			
			sMark = nDrawLeft == -1? sMarkStt : sMarkEnd;
		}
	}

	if(bMarkRebar && m_pStd->IsMarkRebar(sMark) && tvArrUpper.GetSize() > 0)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		// ndrawLeft에 따라서 필요한 것만 골라냄.(DrawRebarMain_CrossSection_UpperSlab에서 가져온 코드) //////
		if(nDrawLeft != 0)
		{
			double dXMid	= pBri->GetMidCrossSection(TRUE);//
			
			long i = 0; for(i = 0; i < xyArr1.GetSize(); i++)
			{
				if((xyArr1[i].x < dXMid) && nDrawLeft == -1) continue;
				if((xyArr1[i].x > dXMid) && nDrawLeft == 1) continue;
				xyArr1.RemoveAt(i, 1);
				i--;
			}
			for(i = 0; i < xyArrHunch1.GetSize(); i++)
			{
				if((xyArrHunch1[i].x < dXMid) && nDrawLeft == -1) continue;
				if((xyArrHunch1[i].x > dXMid) && nDrawLeft == 1) continue;
				xyArrHunch1.RemoveAt(i, 1);
				i--;
			}

			for(i = 0; i < xyArr2.GetSize(); i++)
			{
				if((xyArr2[i].x < dXMid) && nDrawLeft == -1) continue;
				if((xyArr2[i].x > dXMid) && nDrawLeft == 1) continue;
				xyArr2.RemoveAt(i, 1);
				i--;
			}
			for(i = 0; i < xyArrHunch2.GetSize(); i++)
			{
				if((xyArrHunch2[i].x < dXMid) && nDrawLeft == -1) continue;
				if((xyArrHunch2[i].x > dXMid) && nDrawLeft == 1) continue;
				xyArrHunch2.RemoveAt(i, 1);
				i--;
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////

		// 2cycle이 있으면 1cycle와 2cycle 마지막거 중간에 기입하고 없으면 1cycle 마지막 2개 사이에 기입
		// 단. 좌측을 그릴는 거라면 시작 2개..
		long upper	= bUpper ? iUPPER : iLOWER;
		BOOL bLeft	= nDrawLeft == -1 || nDrawLeft == 0 ? TRUE : FALSE;
		CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_SupportRebar_UpperSlab[upper][nDan];

		
		CString sDia	= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(ePartUpperSlab));
		CDPoint xyMark(0, 0);

		// 상/하 배력철근
		long	nSize1	= xyArr1.GetSize(),
				nSize2	= xyArr2.GetSize();
		long i = 0; for(i = 0; i < nSize2; i++)
			xyArr1.Add(xyArr2.GetAt(i));
		nSize1	= xyArr1.GetSize();
		xyArr1.Sort(TRUE);

		CDPoint xyMove	= bUpper ? CDPoint(0, -1) * REBAR_RADIUS : CDPoint(0, 1) * REBAR_RADIUS;
		if(nSize1 > 1)
		{
			CVector v1 = xyArr1[bLeft ? 0 : (int)(nSize1 * 0.8 - 1)]; 
			CVector v2 = xyArr1[bLeft ? 1 : (int)(nSize1 * 0.8 - 2)];
			xyMark = (CDPoint(v1.x, v1.y) + CDPoint(v2.x, v2.y))/2 + xyMove;
		}
		else 
		{
			xyMark	= tvArrUpper[0].GetXyMid();
		}

		CRebarPlacing rb;
		CDPointArray xyArrMatch;
		rb.GetXyMatchTvArrAndLine(tvArrUpperAll, xyMark, CDPoint(0, 1), xyArrMatch);
		if(xyArrMatch.GetSize() > 0)
			xyMark	= xyArrMatch.GetAt(0);


		double dAddHeight	= 0;
		if(nDan == 1)
			dAddHeight += (bUpper ? MARK_BASEHEIGHT_UPPER : MARK_BASEHEIGHT_LOWER);
		else if(nDan == 2)
			dAddHeight += (bUpper ? MARK_BASEHEIGHT_UPPER : MARK_BASEHEIGHT_LOWER) * 2;
		xyMark += dAddHeight * CDPoint(0, bUpper ? 1 : -1);

		CConcSymbol sym(m_pStd->m_pARoadOptionStd);
// 		if(bUpper)
// 			sym.RebarMarkLineUpper(&Dom, xyMark, sMark, sDia, bLeft);
// 		else
// 			sym.RebarMarkLineLower(&Dom, xyMark, sMark, sDia, bLeft);

		CRebar *pRb =  pBri->GetRebarSupportByDescription(sDescription);

		SRebarMarkInputData mark_input_data;
		SRebarMarkviewData mark_view_data;

		mark_input_data.mark = sMark;
		mark_input_data.dia = sDia;
		mark_input_data.position = xyMark;
		mark_input_data.ctc = pBri->m_dCTC_SupportRebar[iUPPER];
		mark_input_data.type_rebar = pRb? pRb->m_nTypeRebar : -1;
		mark_input_data.left_dir = bLeft;
		
		mark_view_data.count_mark = TRUE;
		mark_view_data.line_rebar = TRUE;
		mark_view_data.mark_pos = bUpper? eMarkUpper : eMarkLower;
		
		sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

		// 헌치부 배력철근

		if(!bUpper && xyArrHunch2.GetSize() > 0 && nDrawLeft != 1)
		{
			sDescription.Format("상부슬래브 헌치부 배력철근");
			sMark	= pBri->GetMarkRebarSupport(0, FALSE, FALSE, FALSE, sDescription);
			if(m_pStd->IsMarkRebar(sMark))
			{
				long	nSize1	= xyArrHunch1.GetSize(),
						nSize2	= xyArrHunch2.GetSize();

				long i = 0; for(i = 0; i < nSize2; i++)
					xyArrHunch1.Add(xyArrHunch2.GetAt(i));
				nSize1	= xyArrHunch1.GetSize();
				xyArrHunch1.Sort(TRUE);
			
				if(nSize1 > 1)
					xyMark	= (xyArrHunch1[nSize1-1]+xyArrHunch1[nSize1-2])/2 + xyMove;
				else 
					xyMark	= tvArrUpper[tvArrUpper.GetSize()-1].GetXyMid();

				// sym.RebarMarkLineLower(&Dom, xyMark, sMark, sDia, FALSE);

				pRb =  pBri->GetRebarSupportByDescription(sDescription);

				mark_input_data.mark = sMark;
				mark_input_data.dia = sDia;
				mark_input_data.position = xyMark;
				mark_input_data.ctc = pBri->m_dCTC_SupportRebar[iUPPER];
				mark_input_data.type_rebar = pRb? pRb->m_nTypeRebar : -1;
				mark_input_data.left_dir = FALSE;

				mark_view_data.count_mark = TRUE;
				mark_view_data.line_rebar = TRUE;
				mark_view_data.mark_pos = eMarkLower;

				sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
			}
		}
	}

	*pDomP << Dom;
}

// BOOL bUseMatchSta : sta와 일치하는 위치의 철근들을 사용함(확폭의 경우 실제 sta 횡단면을 그리기 때문)
// 확폭이 아니라면 어딜 그리나 마찬가지 이므로 미리 구해진 시점부 철근을 사용한다. 
void CDrawCrossSection::DrawRebarShear_CrossSection_UpperSlab(CDomyun *pDomP, double sta, BOOL bMarkRebar, BOOL bDimRebar, long nDrawLeft, BOOL bUseMatchSta)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);
	
	// 철근 가져와서 그리기
	CHgBaseDrawStd baseDraw;
	CTwinVectorArray tvArr, tvArr2;
	CTwinVector tv;
	double dStaOrg	= sta;
	pBri->GetTvRebarShear_CrossSection_UpperSlab(tvArr, tvArr2, dStaOrg, nDrawLeft, FALSE, FALSE, FALSE, TRUE);
	long nSize	= tvArr.GetSize();
	BOOL bHidden	= FALSE;
	for(long d = 0; d < 2; d++)
	{
		CTwinVectorArray *pTvArr	= d == 0 ? &tvArr : &tvArr2;
		nSize	= pTvArr->GetSize();
		long i = 0; for(i = 0; i < nSize; i++)
		{
			if(!bHidden)
				m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_SOLID);
			else
				m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_HIDDEN2_LINE);
			
			Dom.LineTo(pTvArr->GetAt(i).m_v1, pTvArr->GetAt(i).m_v2);
			bHidden	= !bHidden;
		}
	}

	CString sMark	= "S1";
	CString sDescription	= "상부슬래브 전단철근1";
	
	
	// sta에 헌치 철근이 있다면 헌치 전단 철근 마킹함 
	// 헌치 철근이 존재 하는지 검사
	double dStaCheck	= pBri->GetStationModifyForRebarCheck(sta);
	BOOL bFind	= FALSE;
	long i = 0; for(i = 0; i < pBri->m_pArrRCMainUpperSlabHunch[0].GetSize(); i++)
	{
		CRebarInfoCycle *pRC	= pBri->m_pArrRCMainUpperSlabHunch[0].GetAt(i);
		double dLeft	= pRC->m_tvArrRebar.GetXyLeft().x;
		double dRight	= pRC->m_tvArrRebar.GetXyRight().x;
		if((fabs(dStaCheck-dLeft) <= 100 || Compare(dStaCheck, dLeft, ">=")) && (fabs(dStaCheck-dRight) <= 100 || Compare(dStaCheck, dRight, "<=")))
		{
			bFind	= TRUE;
			break;
		}
	}

	if(bFind)
	{
		sDescription	= "상부슬래브 전단철근2";
	}
	
	
	sMark	= pBri->GetMarkRebarShear(0, FALSE, FALSE, FALSE, sDescription);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		CTwinVectorArray tvArrCrossUpper;
		pBri->GetTvCrossSection(tvArrCrossUpper, sta, CDPoint(0, 1), FALSE, FALSE, FALSE, 0, FALSE, FALSE, FALSE, FALSE);

		CTwinVectorArray tvArrShearSameLen, tvArrShearSameLen2;
		// 마크를 하기 위해서는 길이가 동일한 철근을 다시 가져 와야 됨.
		pBri->GetTvRebarShear_CrossSection_UpperSlab(tvArrShearSameLen, tvArrShearSameLen2, dStaOrg, nDrawLeft, FALSE, FALSE, TRUE, TRUE);

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		CConcSymbol sym(m_pStd->m_pARoadOptionStd);

		CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_ShearRebar_UpperSlab;
		CString sDia	= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(ePartUpperSlab, TRUE));
		CDPoint xy		= CDPoint(0, 0);
		CDPoint xyMark	= CDPoint(0, 0);
		BOOL bLeft		= nDrawLeft == -1 || nDrawLeft == 0;

		for(long type = 0; type < 2; type++)
		{
			CTwinVectorArray *pTvArr	= type == 0 ? &tvArrShearSameLen : &tvArrShearSameLen2;
			
			if(type == 1)
			{
				sDescription.Format("상부슬래브 전단철근1_1");
				sMark	= pBri->GetMarkRebarShear(0, FALSE, FALSE, FALSE, sDescription);
			}

			if(sMark==_T("")) continue;

			// 마크
			CDPointArray xyArrCount;
			nSize	= pTvArr->GetSize();
			long i = 0; for(i = 0; i < nSize; i++)
				xyArrCount.Add(pTvArr->GetAt(i).GetXyMid());

			for(i = 0; i < xyArrCount.GetSize(); i++)
				sym.RebarCountMarkOne(&Dom, xyArrCount.GetAt(i));
			xyArrCount.Sort(TRUE);

			
			nSize	= xyArrCount.GetSize();
			if(nSize > 0)
			{
				// 마크할 위치 찾음
				if(nSize == 1)
					xyMark	= xyArrCount.GetAt(0);
				else
					xyMark	= bLeft ?	(xyArrCount.GetAt(1)+xyArrCount.GetAt(0))/2 :
										(xyArrCount.GetAt(nSize-1)+xyArrCount.GetAt(nSize-2))/2;

				// 선 긋기
				// 직선으로 만듬
				for(i = 0; i < xyArrCount.GetSize()-1; i++)
				{
					Dom.LineTo(xyArrCount[i], xyArrCount[i+1]);
				}

				// 마크함.
// 				double dHeightShear	= tvArr.GetSize() > 0 ? tvArr.GetAt(0).GetVerLength() : pBri->m_dTS;
				CDPoint xyMatch(0, 0);
				CDRect rectCross	= tvArrCrossUpper.GetRect();
				double dHeightShear		= rectCross.top - xyMark.y;
				double dH			= dHeightShear+Dom.Always(MARK_BASE_HEIGHT);
				// sym.RebarMarkLineUpper(&Dom, xyMark, sMark, sDia, bLeft, dH, FALSE);

				SRebarMarkInputData mark_input_data;
				SRebarMarkviewData mark_view_data;

				mark_input_data.mark = sMark;
				mark_input_data.dia = sDia;
				mark_input_data.position = xyMark;
				mark_input_data.ctc = pBri->m_dCTC_SupportRebar[iUPPER];
				mark_input_data.type_rebar = pBri->m_RebarShearDetail_UpperSlab.GetRebarType();
				mark_input_data.left_dir = bLeft;
				mark_input_data.height = dH;

				mark_view_data.count_mark = FALSE;
				mark_view_data.line_rebar = TRUE;
				mark_view_data.mark_pos = eMarkUpper;

				sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
			}
		}
	} 

	if(bDimRebar)
	{
		
	}

	*pDomP << Dom;
}


// 횡단면도 구조도 총괄
// 횡단면도는 항상 직방향으로 그린다.
// 1. 평면배근을 시점기준 수직으로 세운뒤
// 2. 철근들을 수평으로 뻗어서 걸리는 곳에 만듬
// 3. 시점에서 바라본 모양으로 그려야 됨.(실제 입력받은 좌표들을 mirror해서 그림)
CDRect CDrawCrossSection::DrawRebar_CrossSectionStd(CDomyun *pDomP, double dStaLeft, double dStaRight, BOOL bDimRebarU, BOOL bDimRebarL, BOOL bMarkRebar, BOOL bRotate, BOOL bMirror, BOOL bExcludeDoupleMainRebar)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return CDRect(0, 0, 0, 0);

	CDomyun DomLeft(pDomP);
	CDomyun DomRight(pDomP);
	CDomyun DomLeftHunch(pDomP);
	CDomyun DomRightHunch(pDomP);
	CHgBaseDrawStd baseDraw;

	// 좌우측 가각부 적용 여부
	BOOL bExpLeft	= TRUE;
	BOOL bExpRight	= FALSE;

	BOOL bSameStaLR	= FALSE;	// 좌우측 동일 sta을 그리는 경우인지?
	if(Compare(dStaLeft, dStaRight, "="))
	{
		bSameStaLR	= TRUE;
		bExpRight	= TRUE;
	}


	// 일단 횡단면도 가각적용 무조건 안함 ///////////
	bExpLeft	= FALSE;
	bExpRight	= FALSE;
	//////////////////////////////////////////////////


	// 횡단면도 /////////////////////////////
	m_pStd->m_pARoadOptionStd->SetEnvType(&DomLeft, HCAD_CONC);
	m_pStd->m_pARoadOptionStd->SetEnvType(&DomRight, HCAD_CONC);
	m_pStd->m_pARoadOptionStd->SetEnvType(&DomLeftHunch, HCAD_CONC);
	m_pStd->m_pARoadOptionStd->SetEnvType(&DomRightHunch, HCAD_CONC);

	// 횡단면도는 가각부 적용이 되면 안된다.
//	double dStaSmallestWidth	= pBri->GetStationSmallestWidthOfUpperSlab();

	// 횡단면도를 그릴때 수직으로 잘라서 그림
	long nDrawLeft	= bSameStaLR ? 0 : -1;
	CDPoint vAng	= CDPoint(0, 1);

	CTwinVectorArray tvArrCross;
	pBri->GetTvCrossSection(tvArrCross, dStaLeft, vAng, FALSE, TRUE, FALSE, nDrawLeft, bMirror, FALSE, FALSE, bExpLeft);
	baseDraw.DrawTvArray(&DomLeft, tvArrCross);
	if(!bSameStaLR)
	{
		pBri->GetTvCrossSection(tvArrCross, dStaRight, vAng, FALSE, TRUE, FALSE, 1, bMirror, FALSE, FALSE, bExpRight);
		baseDraw.DrawTvArray(&DomRight, tvArrCross);
	}

// 	double dTHL	= pBri->GetHeightUpperSlab_OldOrNotUse(dStaLeft) - pBri->m_dTS;
// 	double dTHR	= pBri->GetHeightUpperSlab_OldOrNotUse(dStaRight) - pBri->m_dTS;
	double dTHL	= pBri->GetHeightUpperSlab(pBri->GetJijumNumberByStation(dStaLeft), 0, vAng) - pBri->m_dTS;
	double dTHR	= pBri->GetHeightUpperSlab(pBri->GetJijumNumberByStation(dStaRight), 0, vAng) - pBri->m_dTS;
	if(Compare(dTHL, 0.0, ">") && pBri->IsHunchRebarPlane(dStaLeft, FALSE))
	{
		DrawCrossSlabHunch(&DomLeftHunch, dStaLeft, vAng, FALSE, nDrawLeft, bMirror, bExpLeft);
	}

	if(Compare(dTHR, 0.0, ">") && pBri->IsHunchRebarPlane(dStaRight, FALSE))
	{
		if(!bSameStaLR)
		{
			DrawCrossSlabHunch(&DomRightHunch, dStaRight, vAng, FALSE, 1, bMirror, bExpRight);
		}
	}

	// 중심선
	// sLeft와 sRight는 확률상인 데이타일뿐..
	// 실제로 정확하게 지점부인지 fr중앙부인지는 모름
	// 대략 sLeft가 더 작으면 시점부를 선택했을 것이라 가정하고 데이타를 결정함.
	CString sLeft	= dStaLeft > dStaRight ? "중앙부" : "지점부";
	CString sRight	= dStaLeft < dStaRight ? "중앙부" : "지점부";

	if(!bSameStaLR)
	{
		BOOL bVert	= Compare(vAng.GetAngleDegree(), 90.0, "=");
		m_pStd->m_pARoadOptionStd->SetEnvType(&DomLeft,HCAD_CENT);
		double dLenExtUpper	= DomLeft.Always(DomLeft.GetDimExo() + DomLeft.GetDimVLen()) / 3 * 2;
		double dLenExtLower	= DomLeft.Always(15);
		
		DomLeft.SetCalcExtRect();
		CDRect rectLeft	= DomLeft.GetExtRect();
		CDRect rectRight = DomRight.GetExtRect();
		CDRect rect	= m_pStd->MergeRectAndRect(rectLeft, rectRight);
		double dMid	= pBri->GetMidCrossSection(bVert);
		DomLeft.LineTo(dMid, rect.bottom+dLenExtUpper, dMid, rect.top-dLenExtLower);

		// 중앙부 지점부 표시
		m_pStd->m_pARoadOptionStd->SetEnvType(&DomLeft, HCAD_STRS);
		CConcSymbol sym(m_pStd->m_pARoadOptionStd);
		sym.ArrowSide(&DomLeft, sLeft, sRight, CDPoint(dMid, rect.bottom+(dLenExtUpper*3.5/5)));
	}

	double dHDiff	= 0;
	CLineInfo *pLine	= pBri->GetLineBase();
	// 헌치는 dStaSmallest의 높이에 맞게 이동 //
	/*
	dHDiff	= pLine->GetElevationPlanEl(dStaSmallestWidth) - pLine->GetElevationPlanEl(dStaLeft);
	DomLeftHunch.Move(CDPoint(0, dHDiff));
	dHDiff	= pLine->GetElevationPlanEl(dStaSmallestWidth) - pLine->GetElevationPlanEl(dStaRight);
	DomRightHunch.Move(CDPoint(0, dHDiff));
	*/
	

	// 우측을 좌측에 맞게 이동
	if(!bSameStaLR)
	{
		CLineInfo *pLine	= pBri->GetLineBase();
		dHDiff	= pLine->GetElevationPlanEl(dStaLeft) - pLine->GetElevationPlanEl(dStaRight);
		DomRight.Move(CDPoint(0, dHDiff));
	}


	CDRect rectTot;
	if(bRotate)
	{
		CDPoint xyOrg	= CDPoint(0, 0);
		DomLeft.Rotate(xyOrg, CDPoint(0, 1));
		DomRight.Rotate(xyOrg, CDPoint(0, 1));
		DomLeftHunch.Rotate(xyOrg, CDPoint(0, 1));
		DomRightHunch.Rotate(xyOrg, CDPoint(0, 1));
	}

	DomLeft.SetCalcExtRect();
	rectTot	= DomLeft.GetExtRect();
	
	if(!bSameStaLR)
	{
		DomRight.SetCalcExtRect();
		rectTot	= m_pStd->MergeRectAndRect(rectTot, DomRight.GetExtRect());
	}

	*pDomP << DomLeft;
	*pDomP << DomRight;
	*pDomP << DomLeftHunch;
	*pDomP << DomRightHunch;

	if(bMirror)
	{
		pBri->InitValueRebarForCrossSection();
		pBri->SettingValueRebarForCrossSection(dStaLeft, dStaRight);

		long nDimDanUpper	= pBri->GetDanMainRebarUpperSlab(0, TRUE, 0);
		long nDimDanLower	= pBri->GetDanMainRebarUpperSlab(0, FALSE, 0);

		long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			for(long cycle = 0; cycle < pBri->GetCountCycleMainRebar(); cycle++)
			{
				// 횡단면도 주철근
				DrawRebarMain_CrossSection_UpperSlab(&DomLeft, dStaLeft, TRUE, dan, bMarkRebar, cycle==0&&bDimRebarU&&dan==0, nDimDanUpper, cycle, nDrawLeft, bExpLeft, bExcludeDoupleMainRebar);
				DrawRebarMain_CrossSection_UpperSlab(&DomLeft, dStaLeft, FALSE, dan, bMarkRebar, cycle==0&&bDimRebarU&&dan==0, nDimDanLower, cycle, nDrawLeft, bExpLeft, bExcludeDoupleMainRebar);
				if(!bSameStaLR)
				{
					DrawRebarMain_CrossSection_UpperSlab(&DomRight, dStaRight, TRUE, dan, bMarkRebar, cycle==0&&bDimRebarU&&dan==0, nDimDanUpper, cycle, 1, bExpRight, bExcludeDoupleMainRebar);
					DrawRebarMain_CrossSection_UpperSlab(&DomRight, dStaRight, FALSE, dan, bMarkRebar, cycle==0&&bDimRebarU&&dan==0, nDimDanLower, cycle, 1, bExpRight, bExcludeDoupleMainRebar);
				}
			}

			// 횡단면도 배력철근
			DrawRebarSupport_CrossSection_UpperSlab(&DomLeft, dStaLeft, TRUE, dan, FALSE, bMarkRebar, nDrawLeft, bExpLeft);
			DrawRebarSupport_CrossSection_UpperSlab(&DomLeft, dStaLeft, FALSE, dan, bDimRebarL, bMarkRebar, nDrawLeft, bExpLeft);
			if(!bSameStaLR)
			{
 				DrawRebarSupport_CrossSection_UpperSlab(&DomRight, dStaRight, TRUE, dan, FALSE, bMarkRebar, 1, bExpRight);
 				DrawRebarSupport_CrossSection_UpperSlab(&DomRight, dStaRight, FALSE, dan, bDimRebarL, bMarkRebar, 1, bExpRight);
			}

			// 횡단면도 헌치부 배력철근
			if(dan==0)		// #32311
			{
				DrawRebarSupport_CrossSection_SlabHunch(&DomLeft, TRUE, dStaLeft, dan, FALSE, bMarkRebar, nDrawLeft, bExpLeft);
				if(!bSameStaLR)
				{
					DrawRebarSupport_CrossSection_SlabHunch(&DomRight, TRUE, dStaRight, dan, FALSE, bMarkRebar, 1, bExpRight);
				}
			}
		}

		// 횡단면도 전단철근
		DrawRebarShear_CrossSection_UpperSlab(&DomLeft, dStaLeft, bMarkRebar, bDimRebarU, nDrawLeft);
		if(!bSameStaLR)
		{
			DrawRebarShear_CrossSection_UpperSlab(&DomRight, dStaRight, bMarkRebar, bDimRebarU, 1);
		}

		// 종거더 외곽선 표현
		DrawVirtualRebarJongGirder(&DomLeft, dStaLeft, TRUE, TRUE);
		DrawVirtualRebarJongGirder(&DomRight, dStaRight, FALSE, TRUE);
	}


	


	// 우측을 좌측에 맞게 이동
	if(!bSameStaLR)
	{
		DomRight.Move(CDPoint(0, dHDiff));
	}

	DomLeft.SetCalcExtRect();
	DomRight.SetCalcExtRect();

	// 사보강 배치에선 사거리 치수 뺌
	if(pBri->m_nTypePlacingMainRebar_UpperSlab == 1 || pBri->m_nTypePlacingMainRebar_UpperSlab == 2)
	{
		if(pBri->GetAngleJijum(0) != CDPoint(0, 1))
		{
			CDRect rect	= DomLeft.GetExtRect();
			if(!bSameStaLR)
				rect	= m_pStd->MergeRectAndRect(rect, DomRight.GetExtRect());

			m_pStd->m_pARoadOptionStd->SetEnvType(&DomLeft, HCAD_DIML);
			DomLeft.SetTextAlignHorz(TA_RIGHT);
			DomLeft.TextOut(rect.right, rect.top-DomLeft.GetTextHeight()*2, _T("* () 안의 치수는 사거리 치수임."));
		}
	}


	if(bRotate)
	{
		CDPoint xyOrg	= CDPoint(0, 0);
		DomLeft.Rotate(xyOrg, CDPoint(0, 1));
		DomRight.Rotate(xyOrg, CDPoint(0, 1));
	}

	*pDomP << DomLeft;
	*pDomP << DomRight;


	return rectTot;
}

// 헌치 철근 그리기(배력) 
// BOOL bUseMatchSta : sta와 일치하는 위치의 철근들을 사용함(확폭의 경우 실제 sta 횡단면을 그리기 때문)
// 확폭이 아니라면 어딜 그리나 마찬가지 이므로 미리 구해진 시점부 철근을 사용한다. 
void CDrawCrossSection::DrawRebarSupport_CrossSection_SlabHunch(CDomyun *pDomP, BOOL bUpperSlab, double sta, long nDan, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft, BOOL bExp, BOOL bUseMatchSta)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;

	if(!pBri->IsExp(sta))
		bExp	= FALSE;


	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CHgBaseDrawStd baseDraw;
	
	// 헌치
	CTwinVectorArray tvArrHunch;
	if(bUpperSlab)
	{
		pBri->GetTvRebarSupport_CrossSection_UpperSlabHunch(tvArrHunch, sta, nDan, nDrawLeft, TRUE, bExp, FALSE, 0, TRUE);
	}
	else
	{
		pBri->GetTvRebarSupport_CrossSection_LowerSlabHunch(tvArrHunch, sta, nDan, nDrawLeft, TRUE, bExp, FALSE, 0, TRUE);
	}
	
	baseDraw.DrawTvArray(&Dom, tvArrHunch, FALSE, FALSE);

	if(bDimRebar)
	{}

	if(bMarkRebar)
	{}

	*pDomP << Dom;
}

// 횡단면 외측벽체 전단철근 그리기
// void CDrawCrossSection::DrawRebarShear_CrossSection_OutWall(CDomyun *pDomP, BOOL bStt)
// {	
// 	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
// 	if(!pBri) return;
// 
// 	CDomyun Dom(pDomP);
// 	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);
// 	CConcSymbol sym(m_pStd->m_pARoadOptionStd);
// 
// 	CTwinVectorArray tvArr;	// 철근 좌표
// 	pBri->GetTvRebarShear_CrossSection_OutWall(tvArr, bStt);
// 	
// 	CTwinVector tv;
// 	long nSize		= tvArr.GetSize();
// 	if(nSize > 0)
// 	{
// 		tv				= tvArr.GetAt(0);
// 		double dLen		= tv.GetLength();
// 		CDPoint xyDir	= tv.GetXyDir();
// 		long i = 0; for(i = 0; i < nSize; i++)
// 		{
// 			tv	= tvArr.GetAt(i);
// 			sym.DrawShearRebar(&Dom, tv.m_v1, dLen, xyDir);
// 		}
// 	}
// 
// 	*pDomP << Dom;
// }
	
// 현재 이 함수를 입력받기 위해 쓰는 곳은 종단면제원 입력밖에 없으므로
// 횡단면 상단 치수에 대해서는 항상 입력용이 아닌 것으로 쓴다.
// 실제 모양을 그림.
void CDrawCrossSection::DrawCrossAllAtJijum(CDomyun* pDomP, long nJ, BOOL bApplyAngle, BOOL bDim, BOOL bInput, BOOL bDrawPile, BOOL bDrawRubble)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	double sta = pBri->GetStationOnJijum(nJ);// >GetStationBridgeMid();


	CDPoint vAng = bApplyAngle ? pBri->GetAngleJijum(nJ) : CDPoint(0, 1);
	CDPoint vAngWall	= CDPoint(0, 1);
	//vAng = CDPoint(Round(vAng.x, 5), Round(vAng.y, 5));

	if(nJ == 0 || nJ == pBri->m_nQtyJigan)
	{
		if(pBri->m_nTypeWallForRebarGen == 0)
		{
			sta		= pBri->GetStationByTarget(nJ ==0 ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0, TRUE);
			vAng	= pBri->GetAngleByTarget(nJ == 0 ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0);
		}
		else if(pBri->m_nTypeWallForRebarGen == 1)
		{
			sta		= pBri->GetStationByTarget(nJ == 0 ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_END_END, 0, TRUE);
			vAng	= pBri->GetAngleByTarget(nJ == 0 ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_END_END, 0);
		}
	}
	else
	{
		// 경사교의 경우에는 좌측을 기준으로 횡단면도 그린다.
		CWallApp *pWall	= pBri->GetInWall(nJ-1);
		
		if(pWall->m_nType == WALL_TYPE_SLOPE)
		{
			CLineInfo *pLine	= pBri->GetLineBase();
			CDPoint xyOrg		= pWall->GetXyOrgTopForFront(TRUE);
			CDPoint xyMatch		= xyOrg;
			CDPoint vAngWall	= ToDPointFrDegree(pWall->GetAngleWallByHor(TRUE));
			double dTP			= pBri->GetThickPave();
			pLine->GetXyMatchByJongDanXyAndAngle(xyOrg, vAngWall, xyMatch, -dTP);
			sta	= xyMatch.x;
		}
		vAngWall	= ToDPointFrDegree(pWall->GetAngleWallByHor(TRUE));
	}

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DrawCrossSection(&Dom, sta, vAng, TRUE, TRUE, TRUE, TRUE, TRUE, bDrawPile, FALSE, TRUE, TRUE, 0, nJ, TRUE, bDrawRubble);
	DrawCrossSlabHunch(&Dom, sta, vAng, TRUE, 0, FALSE, TRUE, vAngWall);

	if(bDim)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		DimCrossSection(&Dom, sta, vAng, FALSE, FALSE, TRUE);
		MarkGenCrossArrowDirection(&Dom, sta, vAng, nJ==0);
		DimCrossBottom(&Dom, nJ, bInput);	// 하면 치수
	}

	*pDomP << Dom;
}

void CDrawCrossSection::MarkGenCrossArrowDirection(CDomyun *pDomP, double sta, CDPoint vAng, BOOL bStt, BOOL bExp)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	CDPoint A[30];
	pBri->GetXySlabUpperAct(sta, vAng, A, FALSE, bExp);

	int nCountHDan = pBri->GetQtyHDanAct(sta,vAng);
	double dMaxY = pBri->GetElMaxSlabUpper(sta, vAng);
	CString strDown = _T("(") + m_pStd->m_pARcBridgeDataStd->m_strLineRevDirName + _T(")");
	CString strUp = _T("(") + m_pStd->m_pARcBridgeDataStd->m_strLineDirName + _T(")");
	double se = bStt ? -1 : 1;

	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);

	Dom.SetTextAlignHorz(TA_CENTER);

	double dLenArrow = Dom.Always(7.5);
	CDPoint xyDownDir = CDPoint(A[0].x/2, dMaxY+Dom.Always(12)+dLenArrow);
	CDPoint xyUpDir = CDPoint(A[nCountHDan].x/2, dMaxY+Dom.Always(12));
	if(bStt)
	{
		xyDownDir.y = dMaxY+Dom.Always(12);
		xyUpDir.y = dMaxY+Dom.Always(12)+dLenArrow;
	}

	// 하행
	Sym.ArrowDirection(&Dom, xyDownDir, _T(""), CDPoint(0, -1*se), 0, FALSE, dLenArrow-Dom.Always(4.5));
	if(bStt)
		Dom.TextOut(xyDownDir.x, xyDownDir.y+dLenArrow, strDown);
	else
		Dom.TextOut(xyDownDir, strDown);
	// 상행
	Sym.ArrowDirection(&Dom, xyUpDir, _T(""), CDPoint(0, 1*se), 0, FALSE, dLenArrow-Dom.Always(4.5));
	if(bStt)
		Dom.TextOut(xyUpDir, strUp);
	else
		Dom.TextOut(xyUpDir.x, xyUpDir.y+dLenArrow, strUp);
	
	*pDomP << Dom;
}

// bAllBridge : 교량구간 전체에 대한 편경사를 표기한다. -> 변화되는 경우 VAR로 표기
// BOOL bOnlyFootway : 보도에만 구배 표시(보도가 없으면 표기 안함)
void CDrawCrossSection::MarkGenCrossSlope(CDomyun *pDomP, double sta, CDPoint vAng, BOOL bStt, BOOL bRotate90, BOOL bAlignCenter, BOOL bMoveForMirro, BOOL bAllBridge, BOOL bOnlyFootway)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	CDomyun Dom(pDomP);
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	double dSpaceY = Dom.Always(3);
	double dSlope = 0;
	CString strSlope = _T("");
	CDPoint xy(0, 0), vDir(0, 0);
	CDPoint xy2(0, 0), xyMark(0, 0), xyMarkRot(0,0);
	double se = bStt ? -1 : 1;
	long nQtyHDan	= pBri->GetQtyHDan(FALSE);

	long nHDan	= -1;
	long nHDan2	= -1;

	CString strSlopeLeft  = _T("");
	CString strSlopeRight = _T("");
	if(bAllBridge && !bOnlyFootway)
	{
		// 좌측편구배
		double staStt  = pBri->m_dStationBridgeStt;
		double staEnd  = pBri->m_dStationBridgeStt + pBri->m_dLengthBridge;
		double dSlope1 = pBri->GetLineBase()->GetPyungubae(staStt,-1000);
		double dSlope2 = pBri->GetLineBase()->GetPyungubae(staEnd,-1000);
		if(Compare(dSlope1,dSlope2,"="))
			strSlopeLeft.Format(_T("%.3f%%"), dSlope1*100.0);
		else
			strSlopeLeft = "VAR";
		// 우측편구배
		dSlope1 = pBri->GetLineBase()->GetPyungubae(staStt,+1000);
		dSlope2 = pBri->GetLineBase()->GetPyungubae(staEnd,+1000);
		if(Compare(dSlope1,dSlope2,"="))
			strSlopeRight.Format(_T("%.3f%%"), dSlope1*100.0);
		else
			strSlopeRight = "VAR";
	}

	// 좌측 편경사 표시할 곳 찾음 ////////////
	// 차도중 중심에 가장 가까운 차도를 찾고, 이미 찾은 경우 3미터 미만의 차도는 무시한다.
	long i = 0; for(i = 0; i < nQtyHDan; i++)
	{
		CGuardWallRC *pGW	= pBri->GetGuardWallByHDan(i, FALSE);
		if(!pGW) continue;

		if(i+1 < nQtyHDan)
			xy2 = pBri->GetXySlabHDanLeft(sta, vAng, i+1, FALSE);
		else
		{
			if(nHDan > -1) break;
			xy2	= xy;
		}

		if((!bOnlyFootway && (pGW->IsTypeHDanOnlyNone() || pBri->m_pARcBridgeDataStd->IsRailDesign())) || (bOnlyFootway && (pGW->IsTypeHDanRailFootway() || pGW->IsTypeHDanRoadFootway())))
		{
			xy = pBri->GetXySlabHDanLeft(sta, vAng, i, FALSE);
			/*
			if(i+1 < nQtyHDan)
				xy2 = pBri->GetXySlabHDanLeft(sta, vAng, i+1, FALSE);
			else
				xy2	= xy;
				*/
			xyMark	= (xy+xy2)/2;
			if(Compare(xyMark.x, 0.0, "<="))
			{
				// 이미 찾은 경우 3미터 미만은 무시한다.
				if(nHDan > -1)
					if(fabs(xy.x-xy2.x) < 3000) continue;

				nHDan	= i;
				if(!ComparePt(xy2, xy, "="))
					nHDan2	= i+1;
				else
					nHDan2	= nHDan;
			}
		}

	}
	////////////////////////////////////////

	// 좌측 편경사
	if(nHDan > -1)
	{
		if(nHDan == nHDan2)
		{
			if(nHDan > 0)
				nHDan--;
		}

		if(nHDan != nHDan2)
		{
			xy  = pBri->GetXySlabHDanLeft(sta, vAng, nHDan, FALSE);
			xy2 = pBri->GetXySlabHDanLeft(sta, vAng, nHDan2, FALSE);

			if(xy.x < 0 && xy2.x > 0)
			{
				CLineInfo *pLine	= pBri->GetLineBase();
				double dTP	= pBri->GetThickPave(FALSE);
				xy2.x	= 0;
				xy2.y	= pLine->GetElevationPlanEl(sta)-dTP;
			}

			xyMark	= (xy+xy2)/2;

			xyMark.y += dSpaceY;
			// 보도는 보도 위에다가 찍자.
			if(bOnlyFootway)
			{
				CGuardWallRC *pGW	= pBri->GetGuardWallByHDan(nHDan, FALSE);
				xyMark.y += pGW->GetHeight();
			}
			if(bMoveForMirro)
				xyMark.x = 0 - xyMark.x;
			if(bRotate90)
				xyMark = xyMark.Rotate90();
			//		dSlope = pBri->GetSlopeHDanByStation(sta,nHDan,FALSE);
			dSlope = Round((xy.y-xy2.y) / (xy2.x-xy.x),5);

			if(bOnlyFootway)
			{
				CGuardWallRC *pGW	= pBri->GetGuardWallByHDan(nHDan, FALSE);
				dSlope	= Round(-pGW->m_dSlopeUpper, 5);
			}

			// 보도는 무조건 -로 표기함
			if(bAllBridge)
				strSlope = strSlopeLeft;
			else
				strSlope.Format(_T("%.3f%%"), bOnlyFootway ? dSlope*100 : dSlope*100.0);
			vDir = CDPoint(-se, dSlope).Unit();

			Sym.ArrowDirectionForSlope(&Dom, xyMark, strSlope, vDir, bAlignCenter);
			if(bRotate90)
				Dom.Rotate(xyMark, CDPoint(0, 1));
		}
	}

	*pDomP << Dom;

	// 우측 편경사 표시할 곳 찾음 ////////////
	// 차도중 중심에 가장 가까운 차도를 찾고, 이미 찾은 경우 3미터 미만의 차도는 무시한다.
	nHDan = -1;
	nHDan2 = -1;
	for(i = nQtyHDan-1; i >= 0; i--)
	{
		CGuardWallRC *pGW	= pBri->GetGuardWallByHDan(i, FALSE);
		if(!pGW) continue;
		if(i+1 < nQtyHDan)
			xy2 = pBri->GetXySlabHDanLeft(sta, vAng, i+1, FALSE);
		else
			xy2	= xy;
		if((!bOnlyFootway && (pGW->IsTypeHDanOnlyNone() || pBri->m_pARcBridgeDataStd->IsRailDesign())) || (bOnlyFootway && (pGW->IsTypeHDanRailFootway() || pGW->IsTypeHDanRoadFootway())))
		{
			xy = pBri->GetXySlabHDanLeft(sta, vAng, i, FALSE);
			xyMark	= (xy+xy2)/2;
			if(Compare(xyMark.x, 0.0, ">="))
			{
				// 이미 찾은 경우 3미터 미만은 무시한다.
				if(nHDan > -1)
					if(fabs(xy.x-xy2.x) < 3000) continue;

				nHDan	= i;
				if(!ComparePt(xy2, xy, "="))
					nHDan2	= i+1;
				else
					nHDan2	= nHDan;

			}
		}
	}
	////////////////////////////////////////
	
	// 우측 편경사
	if(nHDan > -1)
	{
		if(nHDan == nHDan2)
		{
			if(nHDan > 0)
				nHDan--;
		}
		
		if(nHDan != nHDan2)
		{
			xy = pBri->GetXySlabHDanLeft(sta, vAng, nHDan, FALSE);
			xy2 = pBri->GetXySlabHDanLeft(sta, vAng, nHDan2, FALSE);
			if(xy.x < 0 && xy2.x > 0)
			{
				CLineInfo *pLine	= pBri->GetLineBase();
				double dTP	= pBri->GetThickPave(FALSE);
				xy.x	= 0;
				xy.y	= pLine->GetElevationPlanEl(sta)-dTP;
			}
			xyMark	= (xy+xy2)/2;

			xyMark.y += dSpaceY;
			// 보도는 보도 위에다가 찍자.
			if(bOnlyFootway)
			{
				CGuardWallRC *pGW	= pBri->GetGuardWallByHDan(nHDan, FALSE);
				xyMark.y += pGW->GetHeight();
			}

			if(bMoveForMirro)
				xyMark.x = 0 - xyMark.x;
			if(bRotate90)
				xyMark = xyMark.Rotate90();
			//		dSlope = pBri->GetSlopeHDanByStation(sta,nHDan,FALSE);

			dSlope = Round((xy2.y-xy.y) / (xy2.x-xy.x),5);
			if(bOnlyFootway)
			{
				CGuardWallRC *pGW	= pBri->GetGuardWallByHDan(nHDan, FALSE);
				dSlope	= Round(pGW->m_dSlopeUpper, 5);
			}

			if(bAllBridge)
				strSlope = strSlopeRight;
			else
				strSlope.Format(_T("%.3f%%"), bOnlyFootway ? dSlope*100 : dSlope*100.0);
			vDir = CDPoint(se, dSlope).Unit();
			Sym.ArrowDirectionForSlope(&Dom, xyMark, strSlope, vDir, bAlignCenter);
			if(bRotate90)
				Dom.Rotate(xyMark, CDPoint(0, 1));
		}
	}

	*pDomP << Dom;
}

// 교명주 받침 정면도
void CDrawCrossSection::DrawMonumentFront(CDomyun* pDomP)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	CTwinVector tv;
	CTwinVectorArray tvArr;

	double dH1 = pBri->m_dMonuH1;
	double dH2 = pBri->m_dMonuH2;
	double dH3 = pBri->m_dMonuH3;
	double dD = pBri->m_dMonuD;
	double dW = pBri->m_dMonuW;
	CDPoint vX(1, 0), vY(0, 1);

	tv.m_v1 = CDPoint(0, 0);
	tv.m_v2 = tv.m_v1 + dH1*vY;
	tvArr.Add(tv);
	tv.m_v1 = tv.m_v2;
	tv.m_v2 = tv.m_v1 - dW*vX;
	tvArr.Add(tv);
	tv.m_v1 = tv.m_v2;
	tv.m_v2 = tv.m_v1 - dH2*vY;
	tvArr.Add(tv);
	tv.m_v1 = tv.m_v2;
	tv.m_v2 = tv.m_v1 + dD*vX - dH3*vY;
	tvArr.Add(tv);
	tv.m_v1 = tv.m_v2;
	tv.m_v2 = tv.m_v1 + (dH2+dH3-dH1)*vY;
	tv.m_v1.z = 5;
	tvArr.Add(tv);
	tv.m_v1 = tv.m_v2;
	tv.m_v2 = tv.m_v1 + (dW-dD)*vX;
	tv.m_v1.z = 5;
	tvArr.Add(tv);

	CDomyun Dom(pDomP);
	m_pStd->ApplyDimSetting(pDomP, &Dom);

	int nSize = tvArr.GetSize();
	int i=0; for(i=0; i<nSize; i++)
	{
		tv = tvArr.GetAt(i);
		if(tv.m_v1.z==5)
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONH);
		else
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
		Dom.LineTo(tv.m_v1, tv.m_v2);
	}

	// 날개벽
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	double widthTotal = 500.0;		// 날개벽 두께
	double dSpare = 350.0;
	Dom.LineTo(0, 0, widthTotal-dW+dD, 0);
	Dom.LineTo(widthTotal-dW+dD, 0, widthTotal-dW+dD, dH1-dH2-dH3-dSpare);
	Dom.LineTo(dD-dW, dH1-dH2-dH3, dD-dW, dH1-dH2-dH3-dSpare);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CUTL);
	double dSpareCut = 100.0;
	Dom.CutLightning(dD-dW-dSpareCut, dH1-dH2-dH3-dSpare, widthTotal-dW+dD+dSpareCut, dH1-dH2-dH3-dSpare, FALSE, 1);

	// 날개벽 표시
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.CircleArrow(0, (dH1-dH2-dH3-dSpare)*5/6, widthTotal-dW+dD+Dom.Always(10), Dom.Always(15), 0, _T("벽체"), _T(""));

	*pDomP << Dom;
}

// 교명주 받침 평면도
void CDrawCrossSection::DrawMonumentPlane(CDomyun* pDomP)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	double dD = pBri->m_dMonuD;
	double dW = pBri->m_dMonuW;
	double dL = pBri->m_dMonuL;

	CDomyun Dom(pDomP);
	m_pStd->ApplyDimSetting(pDomP, &Dom);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	Dom.Rectangle(-dW, dL, 0, 0);

	// 날개벽
	double widthTotal = 500.0;		// 날개벽 두께
	double dSpare = 350.0;
	Dom.LineTo(0, 0, widthTotal-dW+dD, 0);
	Dom.LineTo(widthTotal-dW+dD, 0, widthTotal-dW+dD, dL+dSpare);
	Dom.LineTo(dD-dW, dL, dD-dW, dL+dSpare);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CUTL);
	double dSpareCut = 100.0;
	Dom.CutLightning(dD-dW-dSpareCut, dL+dSpare, widthTotal-dW+dD+dSpareCut, dL+dSpare, FALSE, 1);

	// 날개벽 표시
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.CircleArrow(widthTotal/2-dW+dD, dL+dSpare/2, Dom.Always(30), Dom.Always(15), 0, _T("벽체"), _T(""));

	*pDomP << Dom;
}

// 교명주 받침 정면도 치수
void CDrawCrossSection::DimMonumentFront(CDomyun* pDomP, BOOL bInput)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	double dH1 = pBri->m_dMonuH1;
	double dH2 = pBri->m_dMonuH2;
	double dH3 = pBri->m_dMonuH3;
	double dD = pBri->m_dMonuD;
	double dW = pBri->m_dMonuW;
	double widthTotal = 500.0;		// 날개벽 두께

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDomP, &Dom);

	// 좌측
	Dom.SetDirection(_T("LEFT"));
	Dom.DimMoveTo(-dW, dH1-dH2-dH3, 0);
	if(bInput)
	{
		Dom.DimLineToExtend(dH3, 0, _T("H3"), COMMA(dH3));
		Dom.DimLineToExtend(dH2, 0, _T("H2"), COMMA(dH2));
	}
	else
	{
		Dom.DimLineTo(dH3, 0, COMMA(dH3));
		Dom.DimLineTo(dH2, 0, COMMA(dH2));
	}

	Dom.DimMoveTo(-dW, dH1-dH2-dH3, 1);
	Dom.DimLineTo(dH2+dH3, 1, COMMA(dH2+dH3));

	// 우측
	Dom.SetDirection(_T("RIGHT"));
	Dom.DimMoveTo(widthTotal-dW+dD, 0, 0);
	if(bInput)
		Dom.DimLineToExtend(dH1, 0, _T("H1"), COMMA(dH1));
	else
		Dom.DimLineTo(dH1, 0, COMMA(dH1));

	// 상단
	Dom.SetDirection(_T("TOP"));
	Dom.DimMoveTo(-dW, dH1, 0);
	if(bInput)
		Dom.DimLineToExtend(dD, 0, _T("D"), COMMA(dD));
	else
		Dom.DimLineTo(dD, 0, COMMA(dD));
	Dom.DimLineTo(dW-dD, 0, COMMA(dW-dD));

	Dom.DimMoveTo(-dW, dH1, 1);
	if(bInput)
		Dom.DimLineToExtend(dW, 1, _T("W"), COMMA(dW));
	else
		Dom.DimLineTo(dW, 1, COMMA(dW));

	*pDomP << Dom;
}

// 교명주 받침 평면도 치수
void CDrawCrossSection::DimMonumentPlane(CDomyun* pDomP, BOOL bInput)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	double dW = pBri->m_dMonuW;
	double dL = pBri->m_dMonuL;

	CDomyun Dom(pDomP);
	
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDomP, &Dom);


	Dom.SetDirection(_T("LEFT"));
	Dom.DimMoveTo(-dW, 0, 0);
	if(bInput)
		Dom.DimLineToExtend(dL, 0, _T("L"), COMMA(dL));
	else
		Dom.DimLineTo(dL, 0, COMMA(dL));

	Dom.SetDirection(_T("BOTTOM"));
	Dom.DimMoveTo(-dW, 0, 0);
	if(bInput)
		Dom.DimLineToExtend(dW, 0, _T("W"), COMMA(dW));
	else
		Dom.DimLineTo(dW, 0, COMMA(dW));

	*pDomP << Dom;
}

// 교명주 받침 정면 구조도
void CDrawCrossSection::DrawRebarMonumentFront(CDomyun* pDomP)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;

	double dW = pBri->m_dMonuW;
	double dH1 = pBri->m_dMonuH1;
	double dRA1 = pBri->m_dMonuR_A1;
	double dRC1 = pBri->m_dMonuR_C1;
	CDPoint xyL[3], xyR[3], xy(0, 0), xyRe(0, 0);
	pBri->GetXyRebarMonuFront(xyL, TRUE);
	pBri->GetXyRebarMonuFront(xyR, FALSE);

	CDPointArray xyArr;

	CDomyun Dom(pDomP);

	// 선철근
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	Dom.LineTo(xyL[0], xyL[1]);
	Dom.LineTo(xyL[1], xyL[2]);
	Dom.LineTo(xyR[0], xyR[1]);
	Dom.LineTo(xyR[1], xyR[2]);

	// 점철근
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);
	double dRebarRadius = REBAR_RADIUS*Dom.GetScaleDim()/50;

	//.. 측면
	xy = CDPoint(dRC1-dW-dRebarRadius, dH1-dRA1);
	pBri->GetDimArrayRebarMonuFrontSide(xyArr);
	int nSize = xyArr.GetSize();
	int i=0; for(i=0; i<nSize; i++)
	{
		xyRe = xyArr.GetAt(i);
		for(int j=0; j<xyRe.x; j++)
		{
			xy.y -= xyRe.y;
			CHgBaseDrawStd::InsertHCadBlock(&Dom, HCAD_E05, xy, Dom.GetScaleDim());
		}
	}

	//.. 하단
	xy = xyL[1];
	pBri->GetDimArrayRebarMonuFrontLower(xyArr);
	nSize = xyArr.GetSize();
	CDPoint vDir = (xyL[1]-xyL[2]).Unit();
	for(i=0; i<nSize; i++)
	{
		xyRe = xyArr.GetAt(i);
		for(int j=0; j<xyRe.x; j++)
		{
			xy += xyRe.y/vDir.x*vDir;
			CHgBaseDrawStd::InsertHCadBlock(&Dom, HCAD_E05, xy+dRebarRadius*vDir.Rotate90(), Dom.GetScaleDim());
		}
	}

	//.. 상단
	xy = xyR[0];
	xy.x -= dRC1;
	xy.y += dRebarRadius;

	CDPoint xyDom(0, 0);
	pBri->GetDimArrayRebarMonuPlaneVer(xyArr);
	nSize = xyArr.GetSize();
	for(i=0; i<nSize; i++)
	{
		xyRe = xyArr.GetAt(i);
		for(int j=0; j<xyRe.x; j++)
		{
			xy.x += xyRe.y;
			xyDom = xy;
			if(i==0 && j==0)
				xyDom.x += dRebarRadius;
			if(i==nSize-1 && j==xyRe.x-1)
				xyDom.x -= dRebarRadius;
			CHgBaseDrawStd::InsertHCadBlock(&Dom, HCAD_E05, xyDom, Dom.GetScaleDim());
		}
	}

	*pDomP << Dom;
}

// 교명주 받침 평면 구조도
void CDrawCrossSection::DrawRebarMonumentPlane(CDomyun* pDomP)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;

	double dW = pBri->m_dMonuW;
	double dRC1 = pBri->m_dMonuR_C1;
	double dRC2 = pBri->m_dMonuR_C2;
	double dRC3 = pBri->m_dMonuR_C3;
	double dRC = pBri->m_dMonuR_C;
	int nRnC = pBri->m_nMonuR_nC;
	double dRD1 = pBri->m_dMonuR_D1;
	double dRD2 = pBri->m_dMonuR_D2;
	double dRD3 = pBri->m_dMonuR_D3;
	double dRD = pBri->m_dMonuR_D;
	int nRnD = pBri->m_nMonuR_nD;
	double dLenHor = dRC2 + dRC3 + nRnC*dRC;
	double dLenVer = dRD2 + dRD3 + nRnD*dRD;

	CDPoint xyR(0, 0), xy1(0, 0), xy2(0, 0);
	CDPointArray xyArr;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	// 수평 철근
	pBri->GetDimArrayRebarMonuPlaneHor(xyArr);

	xy1.x = dRC1 - dW;
	xy2.x = xy1.x + dLenHor;

	int nSize = xyArr.GetSize();
	int i=0; for(i=0; i<nSize; i++)
	{
		xyR = xyArr.GetAt(i);
		for(int j=0; j<xyR.x; j++)
		{
			xy1.y += xyR.y;
			xy2.y = xy1.y;
			Dom.LineTo(xy1, xy2);
		}
	}

	// 수직 철근
	pBri->GetDimArrayRebarMonuPlaneVer(xyArr);

	xy1.x = xy2.x = -dW;
	xy1.y = dRD1;
	xy2.y = xy1.y + dLenVer;

	nSize = xyArr.GetSize();
	for(i=0; i<nSize; i++)
	{
		xyR = xyArr.GetAt(i);
		for(int j=0; j<xyR.x; j++)
		{
			xy1.x += xyR.y;
			xy2.x = xy1.x;
			Dom.LineTo(xy1, xy2);
		}
	}

	*pDomP << Dom;
}

// 교명주 받침 정면 구조도 치수
void CDrawCrossSection::DimRebarMonumentFront(CDomyun* pDomP, BOOL bInput)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;

	double dH1 = pBri->m_dMonuH1;
	double dH2 = pBri->m_dMonuH2;
	double dH3 = pBri->m_dMonuH3;
	double dD = pBri->m_dMonuD;
	double dW = pBri->m_dMonuW;
	double dRH = pBri->m_dMonuR_H;
	double dRL = pBri->m_dMonuR_L;
	int nRnA = pBri->m_nMonuR_nA;
	double dRA = pBri->m_dMonuR_A;
	double dRA1 = pBri->m_dMonuR_A1;
	int nRnB = pBri->m_nMonuR_nB;
	double dRB = pBri->m_dMonuR_B;
	int nRnC = pBri->m_nMonuR_nC;
	double dRC = pBri->m_dMonuR_C;
	double dRC1 = pBri->m_dMonuR_C1;
	double dRC2 = pBri->m_dMonuR_C2;
	double dRC3 = pBri->m_dMonuR_C3;
	double dSpare = 200.0;
	double widthTotal = 500.0;		// 날개벽 두께
	CDPoint xyL[3];
	pBri->GetXyRebarMonuFront(xyL, TRUE);

	CString str1(""), str2("");

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	// 상단
	Dom.SetDirection(_T("TOP"));
	if(bInput)
	{
		Dom.DimMoveTo(-dW, dH1, 0, _T("C1"));
		Dom.DimLineToExtend(dRC1, 0, _T(""), COMMA(dRC1));
		Dom.DimLineToExtend(dRC2, 0, _T("C2"), COMMA(dRC2));
	}
	else
	{
		Dom.DimMoveTo(-dW, dH1, 0, COMMA(dRC1));
		Dom.DimLineTo(dRC1, 0, _T(""));
		Dom.DimLineTo(dRC2, 0, COMMA(dRC2));
	}
	str1 = _T("nC@C");
	str2 = COMMA(nRnC) + _T("@") + COMMA(dRC) + _T("=") + COMMA(nRnC*dRC);
	if(!bInput)
	{
		str1	= str2;
		str2	= _T("");
	}
	Dom.DimLineToExtend(nRnC*dRC, 0, str1, str2);
	Dom.DimLineToExtend(dRC3, 0, bInput ? _T("C3") : COMMA(dRC3), bInput ? COMMA(dRC3) : "");
	double dRE = dW - (dRC1+dRC2+nRnC*dRC+dRC3);
	Dom.DimLineToOver(dRE, 0, COMMA(dRE));

	Dom.DimMoveTo(-dW, dH1, 1);
	Dom.DimLineTo(dRC1+dRC2+nRnC*dRC+dRC3+dRE, 1, COMMA(dRC1+dRC2+nRnC*dRC+dRC3+dRE));

	// 하단
	Dom.SetDirection(_T("BOTTOM"));
	Dom.DimMoveTo(-dW, dH1-dH2-dH3-dSpare, 0, bInput ? _T("C1") : COMMA(dRC1));
	Dom.DimLineToExtend(dRC1, 0, _T(""), bInput ? COMMA(dRC1) : "");
	str1 = _T("nB@B");
	str2 = COMMA(nRnB) + _T("@") + COMMA(dRB) + _T("=") + COMMA(nRnB*dRB);
	if(!bInput)
	{
		str1	= str2;
		str2	= _T("");
	}
	Dom.DimLineToExtend(nRnB*dRB, 0, str1, str2);
	dRE = dD - (dRC1+nRnB*dRB);
	Dom.DimLineToOver(dRE, 0, COMMA(dRE));

	Dom.DimMoveTo(-dW, dH1-dH2-dH3-dSpare, 1);
	Dom.DimLineTo(dD, 1, COMMA(dD));

	// 좌측
	Dom.SetDirection(_T("LEFT"));
	double dR = xyL[1].y - (dH1-dH2);
	Dom.DimMoveTo(-dW, dH1-dH2, 1, COMMA(dR));
	Dom.DimLineTo(dR, 1, _T(""));
	str1 = _T("nA@A");
	str2 = COMMA(nRnA) + _T("@") + COMMA(dRA) + _T("=") + COMMA(nRnA*dRA);
	if(!bInput)
	{
		str1	= str2;
		str2	= _T("");
	}
	Dom.DimLineToExtend(nRnA*dRA, 1, str1, str2);
	dRE = dH2 - (dRA1+nRnA*dRA+dR);
	Dom.DimLineTo(dRE, 1, COMMA(dRE));
	Dom.DimLineToOverExtend(dRA1, 1, bInput ? _T("A1") : COMMA(dRA1), bInput ? COMMA(dRA1) : "");

	Dom.DimMoveTo(-dW, dH1-dH2, 2);
	Dom.DimLineTo(dH2, 2, COMMA(dH2));

	// 우측
	Dom.SetDirection(_T("RIGHT"));
	Dom.DimMoveTo(dD-dW+widthTotal, dH1-dRH-dRL, 0);
	Dom.DimLineToExtend(dRL, 0, bInput ? _T("L") : COMMA(dRL), bInput ? COMMA(dRL) : "");
	Dom.DimLineToExtend(dRH, 0, bInput ? _T("H") : COMMA(dRH), bInput ? COMMA(dRH) : "");

	Dom.DimMoveTo(dD-dW+widthTotal, 0, 1);
	Dom.DimLineTo(dH1, 1, COMMA(dH1));

	*pDomP << Dom;
}

// 교명주 받침 평면 구조도 치수
void CDrawCrossSection::DimRebarMonumentPlane(CDomyun* pDomP, BOOL bInput)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	double dW = pBri->m_dMonuW;
	double dL = pBri->m_dMonuL;
	int nRnC = pBri->m_nMonuR_nC;
	double dRC = pBri->m_dMonuR_C;
	double dRC1 = pBri->m_dMonuR_C1;
	double dRC2 = pBri->m_dMonuR_C2;
	double dRC3 = pBri->m_dMonuR_C3;
	int nRnD = pBri->m_nMonuR_nD;
	double dRD = pBri->m_dMonuR_D;
	double dRD1 = pBri->m_dMonuR_D1;
	double dRD2 = pBri->m_dMonuR_D2;
	double dRD3 = pBri->m_dMonuR_D3;

	CString str1(""), str2("");

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	// 수평 철근
	Dom.SetDirection(_T("LEFT"));
	if(bInput)
	{
		Dom.DimMoveTo(-dW, 0, 0, _T("D1"));
		Dom.DimLineToExtend(dRD1, 0, _T(""), COMMA(dRD1));
		Dom.DimLineToExtend(dRD2, 0, _T("D2"), COMMA(dRD2));
	}
	else
	{
		Dom.DimMoveTo(-dW, 0, 0, COMMA(dRD1));
		Dom.DimLineTo(dRD1, 0, COMMA(dRD1));
		Dom.DimLineTo(dRD2, 0, COMMA(dRD2));
	}
	str1 = _T("nD@D");
	str2 = COMMA(nRnD) + _T("@") + COMMA(dRD) + _T("=") + COMMA(nRnD*dRD);
	if(!bInput)
	{
		str1	= str2;
		str2	= _T("");
	}
	Dom.DimLineToExtend(nRnD*dRD, 0, str1, str2);
	Dom.DimLineToExtend(dRD3, 0, bInput ? _T("D3") : COMMA(dRD3), bInput ? COMMA(dRD3) : "");
	double dRE = dL - (dRD1+dRD2+nRnD*dRD+dRD3);
	Dom.DimLineToOver(dRE, 0, COMMA(dRE));

	Dom.DimMoveTo(-dW, 0, 1);
	Dom.DimLineTo(dRD1+dRD2+nRnD*dRD+dRD3+dRE, 1, COMMA(dRD1+dRD2+nRnD*dRD+dRD3+dRE));

	// 수직 철근
	Dom.SetDirection(_T("BOTTOM"));
	if(bInput)
	{
		Dom.DimMoveTo(-dW, 0, 0, _T("C1"));
		Dom.DimLineToExtend(dRC1, 0, _T(""), COMMA(dRC1));
		Dom.DimLineToExtend(dRC2, 0, _T("C2"), COMMA(dRC2));
	}
	else
	{
		Dom.DimMoveTo(-dW, 0, 0, COMMA(dRC1));
		Dom.DimLineTo(dRC1, 0, COMMA(dRC1));
		Dom.DimLineTo(dRC2, 0, COMMA(dRC2));
	}
	str1 = _T("nC@C");
	str2 = COMMA(nRnC) + _T("@") + COMMA(dRC) + _T("=") + COMMA(nRnC*dRC);
	if(!bInput)
	{
		str1	= str2;
		str2	= _T("");
	}
	Dom.DimLineToExtend(nRnC*dRC, 0, str1, str2);
	Dom.DimLineToExtend(dRC3, 0, bInput ? _T("C3") : COMMA(dRC3), bInput ? COMMA(dRC3) : "");
	dRE = dW - (dRC1+dRC2+nRnC*dRC+dRC3);
	Dom.DimLineToOver(dRE, 0, COMMA(dRE));

	Dom.DimMoveTo(-dW, 0, 1);
	Dom.DimLineTo(dRC1+dRC2+nRnC*dRC+dRC3+dRE, 1, COMMA(dRC1+dRC2+nRnC*dRC+dRC3+dRE));

	*pDomP << Dom;
}

// 교명주 받침 정면도 철근 마크
void CDrawCrossSection::MarkRebarMonuFront(CDomyun* pDomP, BOOL bInput)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;

	double dW = pBri->m_dMonuW;
	double dH1 = pBri->m_dMonuH1;
	double dRH = pBri->m_dMonuR_H;
	double dRA = pBri->m_dMonuR_A;
	double dRB = pBri->m_dMonuR_B;
	double dRC1 = pBri->m_dMonuR_C1;
	double dRC2 = pBri->m_dMonuR_C2;
	double dRC = pBri->m_dMonuR_C;
	CString strDiaVer = pBri->GetStringDiaMu(pBri->m_dMonuR_DiaVer);
	CString strDiaHor = pBri->GetStringDiaMu(pBri->m_dMonuR_DiaHor);
	CDPoint xy(0, 0);
	CDPoint xyL[3];
	pBri->GetXyRebarMonuFront(xyL, TRUE);
	CDPoint vDir = (xyL[2]-xyL[1]).Unit();

	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	double dRebarRadius = REBAR_RADIUS*Dom.GetScaleDim()/50;
	double dLenGisi = Dom.Always(8);

	SRebarMarkInputData mark_input_data;
	SRebarMarkviewData mark_view_data;

	// 상단
	xy = CDPoint(dRC1+dRC2+dRC*2-dW, dH1-dRH+dRebarRadius);
	CRebar* pRB = pBri->m_pArrRebarMonument[MO_REBAR_S_U];
	CString str = bInput ? _T("HR") : pRB->m_sMark;
	// Sym.RebarMarkLineUpper(&Dom, xy, str, strDiaHor, TRUE, dH1, FALSE);

	mark_input_data.mark = str;
	mark_input_data.dia = strDiaHor;
	mark_input_data.position = xy;
	mark_input_data.ctc = pBri->m_dMonuR_C;
	mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
	mark_input_data.left_dir = TRUE;
	mark_input_data.height = dH1;

	mark_view_data.count_mark = FALSE;
	mark_view_data.line_rebar = TRUE;
	mark_view_data.mark_pos = eMarkUpper;

	Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

	CDPoint xy0 = xy + dLenGisi*CDPoint(0, 1);
	CDPoint xy1 = CDPoint(dRC1+dRC2+dRC-dW, xy.y);
	CDPoint xy2 = CDPoint(dRC1+dRC2+dRC*3-dW, xy.y);
	Dom.LineTo(xy0, xy1);
	Dom.LineTo(xy0, xy2);

	pRB = pBri->m_pArrRebarMonument[MO_REBAR_M2];
	str = bInput ? _T("VR") : pRB->m_sMark;
	xy = CDPoint(dRC1+dRC2+dRC*4.5-dW, dH1-dRH);
	// Sym.RebarMarkLineUpper(&Dom, xy, str, strDiaVer, FALSE, dH1+dRebarRadius);

	mark_input_data.mark = str;
	mark_input_data.dia = strDiaVer;
	mark_input_data.position = xy;
	mark_input_data.ctc = pBri->m_dMonuR_D;
	mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
	mark_input_data.left_dir = FALSE;
	mark_input_data.height = dH1+dRebarRadius;

	mark_view_data.count_mark = TRUE;
	mark_view_data.line_rebar = TRUE;
	mark_view_data.mark_pos = eMarkUpper;

	Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

	// 측면
	pRB = pBri->m_pArrRebarMonument[MO_REBAR_S_S];
	str = bInput ? _T("HR") : pRB->m_sMark;
	xy = CDPoint(xyL[1].x-dRebarRadius, xyL[1].y+dRA*2);
	// Sym.RebarMarkPointSide(&Dom, xy, dRA, dRA, str, strDiaHor, TRUE, 0, CDPoint(0, 0));

	mark_input_data.mark = str;
	mark_input_data.dia = strDiaHor;
	mark_input_data.position = xy;
	mark_input_data.ctc = pBri->m_dMonuR_A;
	mark_input_data.dist_left_mark = dRA;
	mark_input_data.dist_right_mark = dRA;
	mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
	mark_input_data.left_dir = TRUE;
	mark_input_data.height = 0;

	mark_view_data.count_mark = FALSE;
	mark_view_data.line_rebar = FALSE;
	mark_view_data.mark_pos = eMarkSide;

	Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

	// 하단
	pRB = pBri->m_pArrRebarMonument[MO_REBAR_S_L];
	str = bInput ? _T("HR") : pRB->m_sMark;
	xy = xyL[1] + dRB*2/vDir.x*vDir - dRebarRadius*vDir.Rotate90();
	// Sym.RebarMarkLineLower(&Dom, xy, str, strDiaHor, TRUE, dLenGisi*vDir.x, FALSE, FALSE, _T(""), vDir.Rotate90());

	mark_input_data.mark = str;
	mark_input_data.dia = strDiaHor;
	mark_input_data.position = xy;
	mark_input_data.ctc = pBri->m_dMonuR_B;
	mark_input_data.dist_left_mark = 0;
	mark_input_data.dist_right_mark = 0;
	mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
	mark_input_data.left_dir = TRUE;
	mark_input_data.height = dLenGisi*vDir.x;

	mark_view_data.count_mark = FALSE;
	mark_view_data.line_rebar = TRUE;
	mark_view_data.mark_pos = eMarkLower;
	mark_view_data.dir_leadr_line = vDir.Rotate90();

	Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);


	xy0 = xy - dLenGisi*vDir.Rotate90();
	xy1 = xyL[1] + dRB/vDir.x*vDir - dRebarRadius*vDir.Rotate90();
	xy2 = xyL[1] + dRB*3/vDir.x*vDir - dRebarRadius*vDir.Rotate90();
	Dom.LineTo(xy0, xy1);
	Dom.LineTo(xy0, xy2);

	pRB = pBri->m_pArrRebarMonument[MO_REBAR_M1];
	str = bInput ? _T("VR") : pRB->m_sMark;
	xy = xyL[1] + dRB*3.5/vDir.x*vDir;
	// Sym.RebarMarkLineLower(&Dom, xy, str, strDiaVer, TRUE, 0, FALSE);
	Sym.RebarCountMarkOne(&Dom, xy, CDPoint(0, 1));

	mark_input_data.mark = str;
	mark_input_data.dia = strDiaVer;
	mark_input_data.position = xy;
	mark_input_data.ctc = pBri->m_dMonuR_D;
	mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
	mark_input_data.left_dir = TRUE;
	mark_input_data.height = 0;

	mark_view_data.count_mark = FALSE;
	mark_view_data.line_rebar = TRUE;
	mark_view_data.mark_pos = eMarkLower;
	mark_view_data.dir_leadr_line = CDPoint(0, 1);;

	Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

	*pDomP << Dom;
}

// 교명주 받침 평면도 철근 마크
void CDrawCrossSection::MarkRebarMonuPlane(CDomyun* pDomP, BOOL bInput)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;

	double dW = pBri->m_dMonuW;
	double dRC1 = pBri->m_dMonuR_C1;
	double dRC2 = pBri->m_dMonuR_C2;
	double dRC = pBri->m_dMonuR_C;
	double dRD1 = pBri->m_dMonuR_D1;
	double dRD2 = pBri->m_dMonuR_D2;
	double dRD = pBri->m_dMonuR_D;
	CDPoint xy(0, 0), xyR(0, 0);
	CDPointArray xyArr;

	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	SRebarMarkInputData mark_input_data;
	SRebarMarkviewData mark_view_data;

	// 수평 철근
	xy.x = dRC1+dRC2+dRC*1.5-dW;
	pBri->GetDimArrayRebarMonuPlaneHor(xyArr);
	int nSize = xyArr.GetSize();
	int i=0; for(i=0; i<nSize; i++)
	{
		xyR = xyArr.GetAt(i);
		for(int j=0; j<xyR.x; j++)
		{
			xy.y += xyR.y;
			Sym.RebarCountMarkOne(&Dom, xy);
		}
	}
	Dom.LineTo(xy.x, dRD1, xy.x, xy.y);
	CRebar* pRB = pBri->m_pArrRebarMonument[MO_REBAR_M2];
	CString str = bInput ? _T("VR") : pRB->m_sMark;
	CString strDia = pBri->GetStringDiaMu(pBri->m_dMonuR_DiaVer);
	// Sym.RebarMarkLineUpper(&Dom, xy, str, strDia, TRUE, 0, FALSE);
	mark_input_data.mark = str;
	mark_input_data.dia = strDia;
	mark_input_data.position = xy;
	mark_input_data.ctc = pBri->m_dMonuR_C;
	mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
	mark_input_data.left_dir = TRUE;
	mark_input_data.height = 0;

	mark_view_data.count_mark = FALSE;
	mark_view_data.line_rebar = TRUE;
	mark_view_data.mark_pos = eMarkUpper;

	Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

	// 수직 철근
	xy = CDPoint(-dW, dRD1+dRD2+dRD*3.5);
	pBri->GetDimArrayRebarMonuPlaneVer(xyArr);
	nSize = xyArr.GetSize();
	for(i=0; i<nSize; i++)
	{
		xyR = xyArr.GetAt(i);
		for(int j=0; j<xyR.x; j++)
		{
			xy.x += xyR.y;
			Sym.RebarCountMarkOne(&Dom, xy);
		}
	}
	Dom.LineTo(dRC1-dW, xy.y, 0, xy.y);
	pRB = pBri->m_pArrRebarMonument[MO_REBAR_S_U];
	str = bInput ? _T("HR") : pRB->m_sMark;
	strDia = pBri->GetStringDiaMu(pBri->m_dMonuR_DiaHor);
	// Sym.RebarMarkLineSide(&Dom, CDPoint(0, xy.y), str, strDia, FALSE, 0, FALSE);
	mark_input_data.mark = str;
	mark_input_data.dia = strDia;
	mark_input_data.position = CDPoint(0, xy.y);
	mark_input_data.ctc = pBri->m_dMonuR_D;
	mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
	mark_input_data.left_dir = FALSE;
	mark_input_data.height = 0;

	mark_view_data.count_mark = FALSE;
	mark_view_data.line_rebar = TRUE;
	mark_view_data.mark_pos = eMarkSide;

	Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

	*pDomP << Dom;
}

void CDrawCrossSection::DrawTogongLine(CDomyun *pDomP, long iIndex, BOOL bEL, BOOL bJusang, BOOL bInput, long nDeductLeft)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;
	CLineInfoApp *pLine = m_pStd->m_pARcBridgeDataStd->GetLineApp();
	if(!pLine) return;

	CFootingApp *pFooting;
	CTwinVectorArray tvArr;
	
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		pFooting =  &(pBri->m_footingBox);
	}
	else if(iIndex == 0)
	{
		pFooting =  &(pBri->m_footingStt);
	}
	else if(iIndex == pBri->m_nQtyJigan)
	{
		pFooting = &(pBri->m_footingEnd);
	}
	else
	{
		pFooting = &(pBri->GetInWall(iIndex-1)->m_footing);
	}
	pBri->GetTvTogongLine(tvArr, iIndex, TRUE, FALSE, FALSE, TRUE, nDeductLeft);

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CUTL);
	m_pStd->ApplyDimSetting(pDomP, &Dom);

	m_pStd->DrawTvArray(&Dom, tvArr, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());
	CDRect rect = Dom.GetExtRect();

// 	// 지반선
// 	if(bEL)
// 	{
// 		double dStd = pBri->GetStationOnJijum(iIndex, 0);
// 		//DrawElByStation(&Dom, rect.left - Dom.Always(10) , rect.right + Dom.Always(10));
// 	}
	
	// 터파기 EL 
	double dExt	= 1000;
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_MIS4);
	m_pStd->ApplyDimSetting(pDomP, &Dom);
	Dom.MoveTo(rect.left-dExt, pFooting->m_dEL_DiggingFirst);
	Dom.LineTo(rect.right+dExt, pFooting->m_dEL_DiggingFirst);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDomP, &Dom);
	CConcSymbol SymDom(m_pStd->m_pARoadOptionStd);
	CString strEl;
	strEl.Format("EL. %0.3f", toM(pFooting->m_dEL_DiggingFirst));
	if(bInput)
 		SymDom.ArrowElevation(&Dom, "EL1", strEl, CDPoint(rect.left+Dom.Always(3), pFooting->m_dEL_DiggingFirst),FALSE);
	else
		SymDom.ArrowElevation(&Dom, strEl, "", CDPoint(rect.left+Dom.Always(3), pFooting->m_dEL_DiggingFirst),FALSE);

	// 지하수위 표시
	double dFootingH = pFooting->GetHeight()+pFooting->m_exFooting.GetMaxHeight();
	if(pFooting->m_dEL-dFootingH <= pFooting->m_dEL_DiggingWater)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DASH);
		m_pStd->ApplyDimSetting(pDomP, &Dom);
		Dom.MoveTo(rect.left-dExt, pFooting->m_dEL_DiggingWater);
		Dom.LineTo(rect.right+dExt, pFooting->m_dEL_DiggingWater);

		Dom.SetTextAlignHorz(TA_CENTER);
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_TXTG);
		m_pStd->ApplyDimSetting(pDomP, &Dom);

		CString sTupagi = _T("육상터파기");
		CString sWaterTupagi = pBri->m_bApplyYongSu? _T("용수터파기") : _T("수중터파기");

		Dom.TextOut(rect.right-dExt, pFooting->m_dEL_DiggingWater+Dom.Always(1), sTupagi);
		Dom.TextOut(rect.right-dExt, pFooting->m_dEL_DiggingWater-Dom.Always(1)-Dom.GetTextHeight(), sWaterTupagi);

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		m_pStd->ApplyDimSetting(pDomP, &Dom);
		strEl.Format("EL. %0.3f", toM(pFooting->m_dEL_DiggingWater));
		if(bInput)
			SymDom.ArrowElevation(&Dom, "EL2", strEl, CDPoint(rect.left+Dom.Always(3), pFooting->m_dEL_DiggingWater),FALSE,FALSE);
		else
			SymDom.ArrowElevation(&Dom, strEl, "", CDPoint(rect.left+Dom.Always(3), pFooting->m_dEL_DiggingWater),FALSE,FALSE);
	}

	// 주상도
	if(bJusang)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CUTL);
		m_pStd->ApplyDimSetting(pDomP, &Dom);
		CBoringDataEx *pBoring;
		
		pBoring = pBri->GetBoringDataExByJijumNumber(iIndex);
		m_pStd->m_pDrawLine->DrawBroing(&Dom, pBoring, TRUE);
	}


	*pDomP << Dom;
}

void CDrawCrossSection::DimTogongLine(CDomyun *pDomP, long iIndex, BOOL bInput, BOOL bForUseBM, long nDeductLeft)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;
	CLineInfoApp *pLine = m_pStd->m_pARcBridgeDataStd->GetLineApp();
	if(!pLine) return;
	
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDomP, &Dom);

	CTwinVectorArray pTvArr;
	CTwinVectorArray tvArrText;	// 치수넣을 용도로 구하는 토공라인
	CTwinVector tv;
	CTwinVector tvText;
	CDPoint xyStt(0,0);
	CDoubleArray dArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CStringArray sArrUnderText2;
	CString sDir = _T("BOTTOM");

	long nDimDan = 1;
		
	pBri->GetTvTogongLine(pTvArr,iIndex, TRUE, FALSE, FALSE, TRUE, nDeductLeft);
	pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 2, TRUE);
	pBri->GetTvTogongLine(tvArrText, iIndex, TRUE, FALSE, FALSE, FALSE, nDeductLeft);
	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, 2, TRUE);

	pTvArr.GetTvByInfo("왼쪽 이격", tv);
	tvArrText.GetTvByInfo("왼쪽 이격", tvText);
	dArrDim.Add(tv.GetHorLength());
	sArrText.Add("Pf");
	sArrUnderText.Add(COMMA(tv.GetHorLength()));
	sArrUnderText2.Add(COMMA2(tvText.GetHorLength()));
	xyStt = tv.m_v2;

	pTvArr.GetTvByInfo("기초폭", tv);
	tvArrText.GetTvByInfo("기초폭", tvText);
	dArrDim.Add(tv.GetHorLength());
	sArrText.Add("기초폭");
	sArrUnderText.Add(COMMA(tv.GetHorLength()));
	sArrUnderText2.Add(COMMA2(tvText.GetHorLength()));
	
	pTvArr.GetTvByInfo("오른쪽 이격", tv);
	tvArrText.GetTvByInfo("오른쪽 이격", tvText);
	dArrDim.Add(tv.GetHorLength());
	sArrText.Add("Pf");
	sArrUnderText.Add(COMMA(tv.GetHorLength()));
	sArrUnderText2.Add(COMMA2(tvText.GetHorLength()));

 	if(!bInput)
	{
		sArrText.Copy(sArrUnderText);
		sArrUnderText.RemoveAll();
		sArrUnderText.Copy(sArrUnderText2);
	}

	CHgBaseDrawStd baseDraw;
	baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, 0, FALSE, FALSE, TRUE);
	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();

	*pDomP << Dom;

	// 토공 Slope 치수
	dArrDim.RemoveAll();
	sArrText.RemoveAll();
	sArrUnderText.RemoveAll();

	sDir = _T("RIGHT");
	nDimDan = 1;
	long nIndex = pTvArr.GetIdxByInfo(_T("오른쪽 이격"));
	int nCnt = pTvArr.GetSize();
	
	int i=0; for(i=nIndex+1; i<nCnt; i++)
	{
		tv = pTvArr.GetAt(i);
		dArrDim.Add(tv.GetVerLength());
		sArrText.Add(tv.m_sInfo);
		sArrUnderText.Add(COMMA(tv.GetVerLength()));

		if(i == nIndex+1) xyStt.y = tv.m_v1.y;
	}

	xyStt.x = tv.m_v2.x+Dom.Always(5);
	baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, bForUseBM ? 0 : -1, dArrDim.GetSize()>1, FALSE, FALSE);
	*pDomP << Dom;

	Dom.SetDirection(_T("TOP"));
	long nSize = pTvArr.GetSize();
	long nSizeText	= tvArrText.GetSize();
	double dL = pTvArr.GetAt(nSize-1).GetXyRight().x - pTvArr.GetAt(0).GetXyLeft().x;
	double dLText	= tvArrText.GetAt(nSizeText-1).GetXyRight().x - tvArrText.GetAt(0).GetXyLeft().x;

	tv = pTvArr.GetAt(0);
	nDimDan = 1;
	Dom.DimMoveTo(tv.GetXyLeft().x, tv.GetXyLeft().y, nDimDan);
	Dom.DimLineToExtend(dL, nDimDan, COMMA(dL), COMMA2(dLText));		
	*pDomP << Dom;

	Dom.SetTextAlignHorz(TA_RIGHT);
	Dom.TextOut(CDPoint(rect.right, rect.top - Dom.GetTextHeight()*2), "*()안의 치수는 기초 전면과 배면의 평균폭입니다");
	*pDomP << Dom;
}

void CDrawCrossSection::DimCrossBottom(CDomyun *pDomP, long nJ, BOOL bInput)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	Dom.SetDirection(_T("BOTTOM"));

	if(1)//pBri->m_nTypeBridge==BRIDGE_TYPE_PORTAL)
	{
		CFootingApp *pFooting	= NULL;
		if(nJ == 0 || nJ == pBri->m_nQtyJigan)
			pFooting	= pBri->GetFootingByJijumNumber(nJ);
		else
		{
			if(pBri->GetJijumConnectRibAndSlab() != nJ)
			{
				CWallApp *pWall	= pBri->GetInWall(nJ-1);
				if(pWall )
				{
					if(!pWall->IsVertWall())
						pFooting	= pWall->GetFootingByRotateToVert(TRUE);
					else
						pFooting	= &pWall->m_footing;
				}				
			}
		}

		if(pFooting)
		{
			if(pFooting->m_bIs)
			{
				m_pStd->m_pDrawExFooting->DimFrontSide(&Dom, &pFooting->m_exFooting, TRUE, 0, bInput, TRUE);
				m_pStd->m_pDrawExFooting->DimFrontSide(&Dom, &pFooting->m_exFooting, FALSE, 0, bInput, TRUE);
				m_pStd->m_pDrawExFooting->DimFrontLower(&Dom, &pFooting->m_exFooting, 0, bInput, TRUE);
			}
		}
	}

	*pDomP << Dom;
}

void CDrawCrossSection::DrawRebarShear_CrossSection_SlabGirder(CDomyun *pDomP, long nJ, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CGirderSectionRebar *pGirder	= &pBri->m_SlabGirderRebar[iUPPER][nJ];
	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CTwinVectorArray tvArrShearCross;
	pBri->GetTvRebarShear_CrossSection_UpperSlabGirder(tvArrShearCross, nJ, TRUE, TRUE, TRUE, FALSE);
	baseDraw.DrawTvArray(&Dom, tvArrShearCross);


	if(bDimRebar)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		
		CTwinVectorArray tvArrUpperCross;
		CDPoint vAng	= pBri->GetAngleJijum(nJ);
		pBri->GetTvCrossSection(tvArrUpperCross, pBri->GetStationOnJijum(nJ), vAng, FALSE, FALSE, FALSE, 0, FALSE);
		
		CDPointArray xyArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		long nDimDan	= 0;
		CDPoint xyStt	= CDPoint(0, 0);
		CString sDir	= _T("TOP");
		
		xyArrDim.Add(tvArrUpperCross.GetXyLeft());
		xyArrDim.Add(tvArrUpperCross.GetXyLeft() + (CDPoint(1,0) * pGirder->m_dCoverSide));
		long nSize	= tvArrShearCross.GetSize();
		long i = 0; for(i = 0; i < nSize; i++)
		{
			xyArrDim.Add(tvArrShearCross.GetAt(i).m_v1);
		}
		xyArrDim.Add(tvArrUpperCross.GetXyRight() - (CDPoint(1,0) * pGirder->m_dCoverSide));
		xyArrDim.Add(tvArrUpperCross.GetXyRight());
		
		xyStt	= xyArrDim.GetAt(0);
		xyStt.y	= tvArrUpperCross.GetXyTop().y;
		baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, TRUE, FALSE, TRUE, FALSE, FALSE, FALSE);
	} 

	CString strSlab = (pBri->IsBoxType()) ? _T("상부") : _T("");
	if(bMarkRebar && tvArrShearCross.GetSize() > 0)
	{
		CString sDescription	= _T("");
		sDescription.Format("지점%d %s거더부 수직전단철근", nJ+1, strSlab);
		CString sMark	= pBri->GetMarkRebarEtc(G_REBAR_ST, FALSE, FALSE, FALSE, sDescription, TRUE);
		if(m_pStd->IsMarkRebar(sMark))
		{
			// 슬래브 두께에 맞춰서 나머지도 자른다.
			CTwinVector tvShearOrg	= tvArrShearCross.GetAt(0);
			double dHeight	= pBri->m_dTS;
			CTwinVector tv;
			long nSize	= tvArrShearCross.GetSize();
			long i = 0; for(i = 0; i < nSize; i++)
			{
				tv	= tvArrShearCross.GetAt(i);
				tv.m_v2	= tv.m_v1 + tv.GetXyDir() * dHeight;
				tvArrShearCross.SetAt(i, tv);
			}
			tvShearOrg	= tvArrShearCross.GetAt(0);

			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
			CTwinVectorArray tvArrSupCross;
			pBri->GetTvRebarSupport_CrossSection_UpperSlabGirder(tvArrSupCross, nJ);

			// 배력근은 편경사 때문에 한 줄당 여러개가 나올 수 있다. (그래서 첫번째 1개씩만 남김)
			CString sOldInfo	= _T("");
			for(i = 0; i < tvArrSupCross.GetSize(); i++)
			{
				if(i > 0)
				{
					if(tvArrSupCross.GetAt(i).m_sInfo == sOldInfo)
					{
						tvArrSupCross.RemoveAt(i);
						i--;
					}
					else
					{
						sOldInfo	= tvArrSupCross.GetAt(i).m_sInfo;
					}
				}
			}

			// 배력철근은 첫번째 전단철근 사이에 있는 것중 대~충 중간꺼 두개의 위치만 남긴다.
			CTwinVector tvLimit;
			for(i = 0; i < tvArrSupCross.GetSize(); i++)
			{
				tv	= tvArrSupCross.GetAt(i);
				if(!(tv.m_v1.y >= tvShearOrg.GetXyBottom().y && tv.m_v1.y <= tvShearOrg.GetXyTop().y))
				{
					tvArrSupCross.RemoveAt(i);
					i--;
				}
			}

			nSize	= tvArrSupCross.GetSize();
			if(nSize > 2)
			{
				CTwinVector tvSup1	= tvArrSupCross.GetAt(nSize/2);
				CTwinVector tvSup2	= tvArrSupCross.GetAt(nSize/2+1);
				tvLimit.m_v1	= tvSup1.m_v1;
				tvLimit.m_v2	= tvSup2.m_v1;
			}
			else
			{
				tvLimit	= tvShearOrg;
			}

			// tvLimit사이에 마킹할 위치 찾음
			double dToDown	= tvShearOrg.m_v1.y - tvLimit.GetXyMid().y;
			CDPointArray xyArrMark;
			for(i = 0; i < tvArrShearCross.GetSize(); i++)
			{
				tv	= tvArrShearCross.GetAt(i);
				xyArrMark.Add(tv.m_v1 + tv.GetXyDir() * dToDown);
			}

			// 찾은 위치로 마킹함.
			CConcSymbol sym(m_pStd->m_pARoadOptionStd);

			nSize	= xyArrMark.GetSize();
			CDPoint xy1(0, 0), xy2(0, 0);
			for(i = 0; i < nSize; i++)
			{
				xy1	= xyArrMark.GetAt(i);
				
				sym.RebarCountMarkOne(&Dom, xyArrMark.GetAt(i));
				if(i < nSize-1)
				{
					xy2	= xyArrMark.GetAt(i+1);
					Dom.LineTo(xy1, xy2);
				}
			}

			// 마킹한 적당한 위치에 표기(마지막 기둥과 오른쪽 끝 중간쯤)
			double dX	= 0;
			CDRect rect	= tvArrShearCross.GetRect();
			if(pBri->GetTypeColumnByJijum(nJ) == -1)	// 기둥식이 아니면 대충 2/3지점 
			{
				dX	= rect.left + rect.Width()/3*2;
			}
			else
			{
				CWallApp *pWall	= pBri->GetInWall(nJ-1);

				CDPoint vAng	= pBri->GetAngleJijum(nJ);
				CTwinVectorArray tvArrCross;
				pBri->GetTvCrossSection(tvArrCross, pBri->GetStationOnJijum(nJ), vAng, FALSE, FALSE, FALSE, 0, FALSE);
				rect	= tvArrCross.GetRect();
				dX = rect.left;
				for(i = 0; i < pWall->m_nCountColumn; i++)
					dX	+= pWall->m_dDistColumn[i];
				dX = (dX + rect.right)/2;
			}

			// dX쯤에 걸리는 곳에 표기
			CRebarPlacing rb;
			for(i = 0; i < xyArrMark.GetSize()-1; i++)
			{
				xy1	= xyArrMark.GetAt(i);
				xy2	= xyArrMark.GetAt(i+1);
				if(dX >= xy1.x && dX <= xy2.x)
				{
					CDPoint xyMark	= (xy1+xy2)/2;
					CDPointArray xyArrMatch;
					CTwinVectorArray tvArrUpperCross;
					CDPoint vAng	= pBri->GetAngleJijum(nJ);
					pBri->GetTvCrossSection(tvArrUpperCross, pBri->GetStationOnJijum(nJ), vAng, FALSE, FALSE, FALSE, 0, FALSE);
					rb.GetXyMatchTvArrAndLine(tvArrUpperCross, xyMark, CDPoint(0, 1), xyArrMatch);

					double dHeight	= 0;
					if(xyArrMatch.GetSize() > 0)
					{
						dHeight = xyArrMatch.GetAt(0).y - xyMark.y + Dom.Always(MARK_BASE_HEIGHT); 
					}
					
					//sym.RebarMarkLineUpper(&Dom, (xy1+xy2)/2, sMark, m_pStd->m_pARcBridgeDataStd->GetStringDia(pGirder->m_dDiaStirrup), FALSE, dHeight, FALSE);

					SRebarMarkInputData mark_input_data;
					SRebarMarkviewData mark_view_data;
					mark_input_data.mark = sMark;
					mark_input_data.dia = m_pStd->m_pARcBridgeDataStd->GetStringDia(pGirder->m_dDiaStirrup, pBri->GetValueFy(ePartUpperSlab, TRUE));
					mark_input_data.position = (xy1+xy2)/2;
					mark_input_data.ctc = pGirder->m_dCTC;
					mark_input_data.type_rebar = 18;		// 거더 스트럽철근은 18번 고정
					mark_input_data.left_dir = FALSE;
					mark_input_data.height = dHeight;

					mark_view_data.count_mark = FALSE;
					mark_view_data.line_rebar = TRUE;
					mark_view_data.mark_pos = eMarkSide;

					sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

					break;
				}
			}
		}
	}

	*pDomP << Dom;
}

//박스형 하부슬래브 수직거더철근
void CDrawCrossSection::DrawRebarShear_CrossSection_LowerSlabGirder(CDomyun *pDomP, long nJ, double dSation, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CGirderSectionRebar *pGirder	= &pBri->m_SlabGirderRebar[iLOWER][nJ];
	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CTwinVectorArray tvArrShearCross;
	pBri->GetTvRebarShear_CrossSection_LowerSlabGirder(tvArrShearCross, nJ, dSation, 0);
	baseDraw.DrawTvArray(&Dom, tvArrShearCross);

	CDPoint vAng	= pBri->GetAngleJijum(nJ);
	if(bDimRebar)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	
		CTwinVectorArray tvArrLowerSlab;
		pBri->GetTv_CrossSection_LowerSlab(tvArrLowerSlab, nJ, dSation, vAng, nDrawLeft);
		CDPointArray xyArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		long nDimDan	= 0;
		CDPoint xyStt	= CDPoint(0, 0);
		CString sDir	= _T("BOTTOM");
	
		xyArrDim.Add(tvArrLowerSlab.GetXyLeft());
		xyArrDim.Add(tvArrLowerSlab.GetXyLeft() + (CDPoint(1,0) * pGirder->m_dCoverSide));
		long nSize	= tvArrShearCross.GetSize();
		long i = 0; for(i = 0; i < nSize; i++)
		{
			xyArrDim.Add(tvArrShearCross.GetAt(i).m_v1);
		}
	
		xyArrDim.Add(tvArrLowerSlab.GetXyRight() - (CDPoint(1,0) * pGirder->m_dCoverSide));
		xyArrDim.Add(tvArrLowerSlab.GetXyRight());
		

		xyStt	= xyArrDim.GetAt(0);
		xyStt.y	= tvArrLowerSlab.GetXyBottom().y;
		baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, TRUE, FALSE, TRUE, FALSE, FALSE, FALSE);
	} 
	
	CString strSlab = (pBri->IsBoxType()) ?  _T("하부") : _T("");
	if(bMarkRebar && tvArrShearCross.GetSize() > 0)
	{
		CString sDescription	= _T("");
		sDescription.Format("지점%d %s거더부 수직전단철근", nJ+1, strSlab);
		CString sMark	= pBri->GetMarkRebarEtc(G_REBAR_ST, FALSE, FALSE, FALSE, sDescription, TRUE);
		if(m_pStd->IsMarkRebar(sMark))
		{
			// 슬래브 두께에 맞춰서 나머지도 자른다.
			CTwinVector tvShearOrg	= tvArrShearCross.GetAt(0);
			double dHeight	= pBri->m_dTS_Lower;
			CTwinVector tv;
			long nSize	= tvArrShearCross.GetSize();
			long i = 0; for(i = 0; i < nSize; i++)
			{
				tv	= tvArrShearCross.GetAt(i);
				tv.m_v2	= tv.m_v1 + tv.GetXyDir() * dHeight;
				tvArrShearCross.SetAt(i, tv);
			}
			tvShearOrg	= tvArrShearCross.GetAt(0);
	
			
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	
	
			CTwinVectorArray tvArrSupCross;
			pBri->GetTvRebarSupport_CrossSection_LowerSlabGirder(tvArrSupCross, nJ, dSation, vAng, nDrawLeft);
	
		

			//첫번째수직철근하하고 간격에 절반만큼 y위치 잡아줌.
			CDPointArray xyArrMark;
			double dMarkY = pGirder->m_dF1 + pGirder->m_dF/2;
			for(i = 0; i < tvArrShearCross.GetSize(); i++)
			{
				tv	= tvArrShearCross.GetAt(i);
				xyArrMark.Add(tv.m_v1 + tv.GetXyDir() * dMarkY);
			}
	
			// 찾은 위치로 마킹함.
			CConcSymbol sym(m_pStd->m_pARoadOptionStd);
	
			nSize	= xyArrMark.GetSize();
			CDPoint xy1(0, 0), xy2(0, 0);
			for(i = 0; i < nSize; i++)
			{
				xy1	= xyArrMark.GetAt(i);
	
				sym.RebarCountMarkOne(&Dom, xyArrMark.GetAt(i));
				if(i < nSize-1)
				{
					xy2	= xyArrMark.GetAt(i+1);
					Dom.LineTo(xy1, xy2);
				}
			}
	
			// 마킹한 적당한 위치에 표기(마지막 기둥과 오른쪽 끝 중간쯤)
			double dX	= 0;
			CDRect rect	= tvArrShearCross.GetRect();
			if(pBri->GetTypeColumnByJijum(nJ) == -1)	// 기둥식이 아니면 대충 2/3지점 
			{
				dX	= rect.left + rect.Width()/3*2;
			}
			else
			{
				CWallApp *pWall	= pBri->GetInWall(nJ-1);
			
				CDPoint vAng	= pBri->GetAngleJijum(nJ);
				CTwinVectorArray tvArrCross;
				pBri->GetTv_CrossSection_LowerSlab(tvArrCross, nJ, dSation, vAng, nDrawLeft);
				rect	= tvArrCross.GetRect();
				dX = rect.left;
				for(i = 0; i < pWall->m_nCountColumn; i++)
					dX	+= pWall->m_dDistColumn[i];
				dX = (dX + rect.right)/2;
			}
			
			// dX쯤에 걸리는 곳에 표기
			CRebarPlacing rb;
			for(i = 0; i < xyArrMark.GetSize()-1; i++)
			{
				xy1	= xyArrMark.GetAt(i);
				xy2	= xyArrMark.GetAt(i+1);
				if(dX >= xy1.x && dX <= xy2.x)
				{
					CDPoint xyMark	= (xy1+xy2)/2;
					CDPointArray xyArrMatch;
					CTwinVectorArray tvArrUpperCross;
					CDPoint vAng	= pBri->GetAngleJijum(nJ);
					pBri->GetTv_CrossSection_LowerSlab(tvArrUpperCross, nJ, dSation, vAng, nDrawLeft);
					rb.GetXyMatchTvArrAndLine(tvArrUpperCross, xyMark, CDPoint(0, -1), xyArrMatch);
			
					double dHeight	= 0;
					if(xyArrMatch.GetSize() > 0)
					{
						dHeight = dMarkY + pGirder->m_dC2[iLOWER][0] + Dom.Always(MARK_BASE_HEIGHT*1.5); 
					}
			
					// sym.RebarMarkLineLower(&Dom, (xy1+xy2)/2, sMark, m_pStd->m_pARcBridgeDataStd->GetStringDia(pGirder->m_dDiaStirrup), FALSE, dHeight, FALSE);

					SRebarMarkInputData mark_input_data;
					SRebarMarkviewData mark_view_data;
					mark_input_data.mark = sMark;
					mark_input_data.dia = m_pStd->m_pARcBridgeDataStd->GetStringDia(pGirder->m_dDiaStirrup, pBri->GetValueFy(ePartLowerSlab, TRUE));
					mark_input_data.position = (xy1+xy2)/2;
					mark_input_data.ctc = pGirder->m_dCTC;
					mark_input_data.type_rebar = 18;		// 거더 스트럽철근은 18번 고정
					mark_input_data.left_dir = FALSE;
					mark_input_data.height = dHeight;

					mark_view_data.count_mark = FALSE;
					mark_view_data.line_rebar = TRUE;
					mark_view_data.mark_pos = eMarkSide;

					sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
					break;
				}
			}
		}
	}

	*pDomP << Dom;
}
// 거더부 수평 전단철근(상면, 하면, 측면 동시 표현)
// 마크하는 부분 언뜻보면 복잡할 수 있으나, 천천히 분석하면 아주 쉬운 코드임..(A/S 할때 지레 겁먹지 말자!!!)
void CDrawCrossSection::DrawRebarSupport_CrossSection_UpperSlabGirder(CDomyun *pDomP, long nJ, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CTwinVectorArray tvArrSupportCross;
	pBri->GetTvRebarSupport_CrossSection_UpperSlabGirder(tvArrSupportCross, nJ);
	baseDraw.DrawTvArray(&Dom, tvArrSupportCross, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());


	if(bDimRebar)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	} 

	CString strSlab = (pBri->IsBoxType()) ? _T("상부") : _T("");
	
	if(bMarkRebar && tvArrSupportCross.GetSize() > 0)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		// sup -> 0 : 상, 1 : 중, 2 : 하
		for(long sup = 0; sup < 3; sup++)
		{
			CString sDescription	= _T("");
			if(sup == 1)
			{
				sDescription.Format("지점%d %s거더부 수평전단철근", nJ+1, strSlab);
			}
			else
			{
				CString sUpper	= sup == 0 ? "상면" : "하면";
				sDescription.Format("지점%d %s거더부 %s 주철근 %d단", nJ+1, strSlab, sUpper, 1);
			}

			CString sMark	= pBri->GetMarkRebarEtc(G_REBAR_HORV, FALSE, FALSE, FALSE, sDescription, TRUE);

			if(m_pStd->IsMarkRebar(sMark))
			{
				CGirderSectionRebar *pGirder	= &pBri->m_SlabGirderRebar[iUPPER][nJ];

				CTwinVectorArray tvArrShearCross;
				pBri->GetTvRebarShear_CrossSection_UpperSlabGirder(tvArrShearCross, nJ);

				// 마킹한 적당한 위치 찾기
				// 거더부 수평 철근(측면) -> (마지막 기둥과 오른쪽 끝 중간쯤)
				// 거더부 수평 철근(상면과 하면) -> (왼쪽과 첫번째 기둥 중간쯤)
				double dX	= 0;
				CDRect rect	= tvArrShearCross.GetRect();
				if(pBri->GetTypeColumnByJijum(nJ) == -1)	// 기둥식이 아니면 대충 2/3지점 
				{
					if(sup == 1)
						dX	= rect.left + rect.Width()/3*2+pGirder->m_dCTC;
					else
						dX	= rect.left + rect.Width()/3*1+pGirder->m_dCTC;
				}
				else
				{
					CWallApp *pWall	= pBri->GetInWall(nJ-1);

					CDPoint vAng	= pBri->GetAngleJijum(nJ);
					CTwinVectorArray tvArrCross;
					pBri->GetTvCrossSection(tvArrCross, pBri->GetStationOnJijum(nJ), vAng, FALSE, FALSE, FALSE, 0, FALSE);
					rect	= tvArrCross.GetRect();
					dX = rect.left;
					if(sup == 1)
					{
						long i = 0; for(i = 0; i < pWall->m_nCountColumn; i++)
							dX	+= pWall->m_dDistColumn[i];
						dX = (dX + rect.right)/2+pGirder->m_dCTC;
					}
					else 
					{
						long i = 0; for(i = 0; i < 1; i++)
							dX	+= pWall->m_dDistColumn[i];
						dX = (rect.left + dX)/2+pGirder->m_dCTC;
					}

					
				}

				// 적당한 위치의 전단철근 사이 찾기.
				CTwinVector tv1, tv2;
				long i = 0; for(i = 0; i < tvArrShearCross.GetSize()-1; i++)
				{
					tv1	= tvArrShearCross.GetAt(i);
					tv2	= tvArrShearCross.GetAt(i+1);
					if(tv1.m_v1.x <= dX && tv2.m_v1.x >= dX)
					{
						dX	= (tv1.m_v1.x+tv2.m_v1.x)/2;
						break;
					}
				}

				// 찾은 위치에 마크할 좌표 구함
				CDPointArray xyArrMark;
				CRebarPlacing rb;
				rb.GetXyMatchTvArrAndLine(tvArrSupportCross, CDPoint(dX, 0), CDPoint(0, 1), xyArrMark);
				xyArrMark.Sort(FALSE, FALSE);

				if(xyArrMark.GetSize() > 1)
				{
					if(sup == 0)	// 상
					{
						xyArrMark.RemoveAt(1, xyArrMark.GetSize()-1);
					}
					else if(sup == 1)	// 중
					{
						xyArrMark.RemoveAt(0);
						xyArrMark.RemoveAt(xyArrMark.GetSize()-1);
					}
					else	// 하
					{
						xyArrMark.RemoveAt(0, xyArrMark.GetSize()-1);
					}

					// 마크할 위치에 마크하고 기호표기
					long nSize	= xyArrMark.GetSize();
					if(nSize == 0) continue;

					CConcSymbol sym(m_pStd->m_pARoadOptionStd);
					for(i = 0; i < nSize; i++)
					{
						sym.RebarCountMarkOne(&Dom, xyArrMark.GetAt(i));
					}
					if(nSize > 1)
						Dom.LineTo(xyArrMark.GetAt(0), xyArrMark.GetAt(nSize-1));
					
					double dHeight	= fabs(pBri->GetThickHunch(nJ, TRUE, TRUE) - pBri->GetThickHunch(nJ, FALSE, TRUE)) + pGirder->m_dCoverLower[0] + Dom.Always(MARK_BASE_HEIGHT+MARK_BASE_RADIUS*1.5);
					if(sup == 0 || sup == 2)
						dHeight	= 0;

					SRebarMarkInputData mark_input_data;
					SRebarMarkviewData mark_view_data;
					if(sup == 0)
					{
						// sym.RebarMarkLineUpper(&Dom, xyArrMark.GetAt(0), sMark, m_pStd->m_pARcBridgeDataStd->GetStringDia(pGirder->m_dDiaUL[iUPPER][0]), TRUE, dHeight, FALSE);
						mark_input_data.dia = m_pStd->m_pARcBridgeDataStd->GetStringDia(pGirder->m_dDiaUL[iUPPER][0], pBri->GetValueFy(ePartUpperSlab));
						mark_input_data.position = xyArrMark.GetAt(0);
						mark_input_data.ctc = pGirder->m_dC[iUPPER][0];
						mark_input_data.left_dir = TRUE;
						mark_view_data.mark_pos = eMarkUpper;
					}
					else
					{
						// sym.RebarMarkLineLower(&Dom, xyArrMark.GetAt(nSize-1), sMark, m_pStd->m_pARcBridgeDataStd->GetStringDia(sup==1 ? pGirder->m_dDiaSide : pGirder->m_dDiaUL[iLOWER][0]), sup == 1 ? FALSE : TRUE, dHeight, FALSE);
						mark_input_data.dia = m_pStd->m_pARcBridgeDataStd->GetStringDia(sup==1 ? pGirder->m_dDiaSide : pGirder->m_dDiaUL[iLOWER][0], pBri->GetValueFy(ePartUpperSlab, sup==1));
						mark_input_data.position = xyArrMark.GetAt(nSize-1);
						mark_input_data.ctc = sup==1 ? pGirder->m_dF : pGirder->m_dC[iLOWER][0];
						mark_input_data.left_dir = sup == 1 ? FALSE : TRUE;
						mark_view_data.mark_pos = eMarkLower;
					}

					mark_input_data.mark = sMark;
					mark_input_data.type_rebar = sup==1 ? 22 : 1;		// 거더 상하부 철근은 1, 측면철근은 22
					mark_input_data.height = dHeight;

					mark_view_data.count_mark = FALSE;
					mark_view_data.line_rebar = TRUE;

					sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
				}
			}
		}
	}

	*pDomP << Dom;
}

void CDrawCrossSection::DrawRebarSupport_CrossSection_LowerSlabGirder(CDomyun *pDomP, long nJ, double dStation, CDPoint vAng, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CTwinVectorArray tvArrSupportCross;
	pBri->GetTvRebarSupport_CrossSection_LowerSlabGirder(tvArrSupportCross, nJ, dStation, vAng, nDrawLeft);
	baseDraw.DrawTvArray(&Dom, tvArrSupportCross, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());

	if(bDimRebar)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	} 

	CString strSlab = (pBri->IsBoxType()) ? _T("하부") : _T("");

	if(bMarkRebar && tvArrSupportCross.GetSize() > 0)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		// sup -> 0 : 상, 1 : 중, 2 : 하
		for(long sup = 1; sup < 2; sup++)
		{
			CString sDescription	= _T("");
			if(sup == 1)
			{
				sDescription.Format("지점%d %s거더부 수평전단철근", nJ+1, strSlab);
			}
			else
			{
				CString sUpper	= sup == 0 ? "상면" : "하면";
				sDescription.Format("지점%d %s거더부 %s 주철근 %d단", nJ+1, strSlab, sUpper, 1);
			}

			CString sMark	= pBri->GetMarkRebarEtc(G_REBAR_HORV, FALSE, FALSE, FALSE, sDescription, TRUE);

			if(m_pStd->IsMarkRebar(sMark))
			{
				CGirderSectionRebar *pGirder	= &pBri->m_SlabGirderRebar[iUPPER][nJ];

				CTwinVectorArray tvArrShearCross;
				pBri->GetTvRebarShear_CrossSection_UpperSlabGirder(tvArrShearCross, nJ);

				// 마킹한 적당한 위치 찾기
				// 거더부 수평 철근(측면) -> (마지막 기둥과 오른쪽 끝 중간쯤)
				// 거더부 수평 철근(상면과 하면) -> (왼쪽과 첫번째 기둥 중간쯤)
				double dX	= 0;
				CDRect rect	= tvArrShearCross.GetRect();
				if(pBri->GetTypeColumnByJijum(nJ) == -1)	// 기둥식이 아니면 대충 2/3지점 
				{
					if(sup == 1)
						dX	= rect.left + rect.Width()/3*2+pGirder->m_dCTC;
					else
						dX	= rect.left + rect.Width()/3*1+pGirder->m_dCTC;
				}
				else
				{
					CWallApp *pWall	= pBri->GetInWall(nJ-1);

					CDPoint vAng	= pBri->GetAngleJijum(nJ);
					CTwinVectorArray tvArrCross;
					pBri->GetTvCrossSection(tvArrCross, pBri->GetStationOnJijum(nJ), vAng, FALSE, FALSE, FALSE, 0, FALSE);
					rect	= tvArrCross.GetRect();
					dX = rect.left;
					if(sup == 1)
					{
						long i = 0; for(i = 0; i < pWall->m_nCountColumn; i++)
							dX	+= pWall->m_dDistColumn[i];
						dX = (dX + rect.right)/2+pGirder->m_dCTC;
					}
					else 
					{
						long i = 0; for(i = 0; i < 1; i++)
							dX	+= pWall->m_dDistColumn[i];
						dX = (rect.left + dX)/2+pGirder->m_dCTC;
					}


				}

				// 적당한 위치의 전단철근 사이 찾기.
				CTwinVector tv1, tv2;
				long i = 0; for(i = 0; i < tvArrShearCross.GetSize()-1; i++)
				{
					tv1	= tvArrShearCross.GetAt(i);
					tv2	= tvArrShearCross.GetAt(i+1);
					if(tv1.m_v1.x <= dX && tv2.m_v1.x >= dX)
					{
						dX	= (tv1.m_v1.x+tv2.m_v1.x)/2;
						break;
					}
				}

				// 찾은 위치에 마크할 좌표 구함
				CDPointArray xyArrMark;
				CRebarPlacing rb;
				rb.GetXyMatchTvArrAndLine(tvArrSupportCross, CDPoint(dX, 0), CDPoint(0, 1), xyArrMark);
				xyArrMark.Sort(FALSE, FALSE);

				if(xyArrMark.GetSize() > 1)
				{
					if(sup == 0)	// 상
					{
						xyArrMark.RemoveAt(1, xyArrMark.GetSize()-1);
					}
					else if(sup == 1)	// 중
					{
						xyArrMark.RemoveAt(0);
						xyArrMark.RemoveAt(xyArrMark.GetSize()-1);
					}
					else	// 하
					{
						xyArrMark.RemoveAt(0, xyArrMark.GetSize()-1);
					}

					// 마크할 위치에 마크하고 기호표기
					long nSize	= xyArrMark.GetSize();
					if(nSize == 0) continue;

					CConcSymbol sym(m_pStd->m_pARoadOptionStd);
					for(i = 0; i < nSize; i++)
					{
						sym.RebarCountMarkOne(&Dom, xyArrMark.GetAt(i));
					}
					if(nSize > 1)
						Dom.LineTo(xyArrMark.GetAt(0), xyArrMark.GetAt(nSize-1));

					double dHeight	= pGirder->m_dCoverUpper[0] + Dom.Always(MARK_BASE_HEIGHT+MARK_BASE_RADIUS);
					if(sup == 0 || sup == 2)
						dHeight	= 0;

					SRebarMarkInputData mark_input_data;
					SRebarMarkviewData mark_view_data;
					if(sup == 0)
					{
						// sym.RebarMarkLineUpper(&Dom, xyArrMark.GetAt(0), sMark, m_pStd->m_pARcBridgeDataStd->GetStringDia(pGirder->m_dDiaUL[iUPPER][0]), TRUE, dHeight, FALSE);
						mark_input_data.dia = m_pStd->m_pARcBridgeDataStd->GetStringDia(pGirder->m_dDiaUL[iUPPER][0], pBri->GetValueFy(ePartLowerSlab));
						mark_input_data.position = xyArrMark.GetAt(0);
						mark_input_data.ctc = pGirder->m_dC[iUPPER][0];
						mark_input_data.left_dir = TRUE;
						
					}
					else
					{
						// sym.RebarMarkLineUpper(&Dom, xyArrMark.GetAt(0), sMark, m_pStd->m_pARcBridgeDataStd->GetStringDia(sup==1 ? pGirder->m_dDiaSide : pGirder->m_dDiaUL[iLOWER][0]), sup == 1 ? FALSE : TRUE, dHeight, FALSE);
						mark_input_data.dia = m_pStd->m_pARcBridgeDataStd->GetStringDia(sup==1 ? pGirder->m_dDiaSide : pGirder->m_dDiaUL[iLOWER][0], pBri->GetValueFy(ePartLowerSlab, sup==1));
						mark_input_data.position = xyArrMark.GetAt(0);
						mark_input_data.ctc = sup==1 ? pGirder->m_dF : pGirder->m_dC[iLOWER][0];
						mark_input_data.left_dir = sup == 1 ? FALSE : TRUE;
			
					}

					mark_input_data.mark = sMark;
					mark_input_data.type_rebar = sup==1 ? 22 : 1;		// 거더 상하부 철근은 1, 측면철근은 22
					mark_input_data.height = dHeight;

					mark_view_data.count_mark = FALSE;
					mark_view_data.line_rebar = TRUE;
					mark_view_data.mark_pos = eMarkUpper;

					sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
				}
			}
		}
	}
	*pDomP << Dom;
}

// nPosDraw = -1 : 정면도상의 좌측
// nPosDraw =  0 : 정면도상의 중앙
// nPosDraw =  1 : 정면도상의 우측
void CDrawCrossSection::MarkFrontConcFck(CDomyun* pDomP, int nJijum, int nPosDraw)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	double sta = pBri->GetStationOnJijum(nJijum, 0);
	CDPoint vAng = pBri->GetAngleJijum(nJijum);
	if(nJijum == 0 || nJijum == pBri->m_nQtyJigan)
	{
		if(pBri->m_nTypeWallForRebarGen == 0)
		{
			sta		= pBri->GetStationByTarget(nJijum ==0 ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0, TRUE);
			vAng	= pBri->GetAngleByTarget(nJijum == 0 ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0);
		}
		else if(pBri->m_nTypeWallForRebarGen == 1)
		{
			sta		= pBri->GetStationByTarget(nJijum == 0 ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_END_END, 0, TRUE);
			vAng	= pBri->GetAngleByTarget(nJijum == 0 ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_END_END, 0);
		}
	}

	CDPoint A[30];
	pBri->GetXySlabUpperAct(sta, vAng, A);
	
	int nCountHDan = pBri->GetQtyHDan(FALSE);
	CDPoint xyLeft = A[0];
	CDPoint xyRight = A[nCountHDan];

	CFootingApp* pFoot = pBri->GetFootingByJijumNumber(nJijum);
	if(!pFoot) return;
	if(!pFoot->m_bIs) return;
	double dFH = pFoot->m_dFH;
	double dEL = pFoot->m_dEL - dFH;	// pFoot->m_dEL은 기초 상단 EL
	double dHeight = pBri->GetXySlabUpperActByDist(sta, 0).y - dEL;

	EPartOfBridge ePart = (nJijum==0)? ePartOutWallStt : ((nJijum==pBri->m_nQtyJigan)? ePartOutWallEnd : ePartInWall);
	CString	str = m_pStd->GetStringTextDimFck(pBri->GetValueFck(ePart), pBri->GetValueAggregate(ePart), pBri->GetValueSlump(ePart));
	CStringArray strArr;
	strArr.Add("구체 콘크리트");
	strArr.Add(str);
	
	CDPoint xyCen(0, 0);
	xyCen.x = nJijum==0 ? -xyRight.x+(xyRight.x-xyLeft.x)*2/5 : xyLeft.x+(xyRight.x-xyLeft.x)*2/5;
	xyCen.y = dEL + dHeight/2.0;
	if(nPosDraw==1)
	{
		xyCen.x = nJijum==0 ? -xyRight.x+(xyRight.x-xyLeft.x)*3/4 : xyLeft.x+(xyRight.x-xyLeft.x)*1/4;
	}
	if(nPosDraw==-1)
	{
		xyCen.x = nJijum==0 ? -xyRight.x+(xyRight.x-xyLeft.x)*1/4 : xyLeft.x+(xyRight.x-xyLeft.x)*3/4;
	}

	CDomyun Dom(pDomP);
	CConcSymbol Sym;

	Sym.DrawSymbolConCArrow(&Dom, xyCen, strArr);

	Dom.Rotate(-120, xyCen.x, xyCen.y);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SYMB);
	double dWidth = Dom.GetExtWidth();
	CHgBaseDrawStd::InsertHCadBlock(&Dom, "CGRECON", xyCen-CDPoint(dWidth/2, 0), Dom.GetScaleDim()*2/3);
	CHgBaseDrawStd::InsertHCadBlock(&Dom, "CGRECON", xyCen+CDPoint(dWidth/2, 0), Dom.GetScaleDim()*2/3);

	*pDomP << Dom;
}

void CDrawCrossSection::DimFrontSide(CDomyun* pDomP, int nJijum, BOOL bLeft, BOOL bRight, BOOL bCenterDim, BOOL bCenterMark, BOOL bAllBridge)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	double sta = pBri->GetStationOnJijum(nJijum, 0);
	CDPoint vAng = pBri->GetAngleJijum(nJijum);
	CDPoint vAngWall	= CDPoint(0, 1);
	CFootingApp* pFoot = NULL;
	if(nJijum == 0 || nJijum == pBri->m_nQtyJigan)
	{
		if(pBri->m_nTypeWallForRebarGen == 0)
		{
			sta		= pBri->GetStationByTarget(nJijum ==0 ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0, TRUE);
			vAng	= pBri->GetAngleByTarget(nJijum == 0 ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0);
		}
		else if(pBri->m_nTypeWallForRebarGen == 1)
		{
			sta		= pBri->GetStationByTarget(nJijum == 0 ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_END_END, 0, TRUE);
			vAng	= pBri->GetAngleByTarget(nJijum == 0 ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_END_END, 0);
		}

		pFoot = pBri->GetFootingByJijumNumber(nJijum);
	}
	else
	{
		if(pBri->IsBoxType())
		{
			pFoot = pBri->GetFootingByJijumNumber(nJijum);
		}
		else
		{
			// 경사교의 경우에는 좌측을 기준으로 횡단면도 그린다.
			CWallApp *pWall	= pBri->GetInWall(nJijum-1);
			if(pWall->m_nType == WALL_TYPE_SLOPE)
			{
				sta	= pWall->GetXyOrgTopForFront(pWall->IsLeftSlope()).x;
			}

			vAngWall	= ToDPointFrDegree(pWall->GetAngleWallByHor());
			pFoot	= pWall->GetFootingByRotateToVert(pWall->IsLeftSlope());
		}
	}
	if(!pFoot) return;

	CDPoint A[30];
	pBri->GetXySlabUpperAct(sta, vAng, A, FALSE, TRUE);

	//CDPoint ANotExp[30];
	//pBri->GetXySlabUpperAct(sta, vAng, ANotExp, FALSE, FALSE);


	double dFH = pBri->IsBoxType() ? pBri->m_dTS_Lower : (pFoot->m_bIs ? pFoot->GetHeight() : 0);
	double dFHu = pBri->IsBoxType() ? pBri->GetHeightHunchByJijum(nJijum, FALSE) : 0;
	if(pBri->IsBoxType() && pBri->m_bSlopeLowerSlab)
	{
		double dHeightHunchDiff = pBri->GetHeightUpperSlab(nJijum, 0, vAng) - pBri->m_dTS - pBri->GetHeightHunchByJijum(nJijum, TRUE);
		// 헌치가 없는경우는 단차가 생기지만 치수표현하지 않는다.
		if(dFHu > 0)
			dFHu += dHeightHunchDiff;
	}

	double dFHEx	= pFoot->m_bIs ? pFoot->GetHeight(FALSE) : 0;
	double dEH = pFoot->m_bIs ? pFoot->m_exFooting.m_dEFHR : 0;
	double dEL = pBri->GetElevationFootingLower(sta, FALSE);
	double dTP = pBri->GetThickPave(FALSE);
	double dTS = pBri->m_dTS;
	double dHeightUpperSlab	= pBri->GetHeightUpperSlab(nJijum, 0, vAngWall);
	double dHeightHunch	= dHeightUpperSlab - pBri->m_dTS;
	double heightTotal = dHeightHunch;
	int nCountHDan = pBri->GetQtyHDanAct(sta, vAng,FALSE);
	CDPoint xyLeft = A[0];
	CDPoint xyRight = A[nCountHDan];

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	// 좌측
	Dom.SetDirection(_T("LEFT"));
	double dDimX = nJijum==0 ? -A[nCountHDan].x : A[0].x;
	double dDimY = dEL - dEH;
	double dHeight = (nJijum==0 ? xyRight.y : xyLeft.y) - dEL;
	double dLenWall = dHeight - (dTS + heightTotal + dFH + dFHu);
	
	if(bLeft)
	{
		Dom.DimMoveTo(dDimX, dDimY, 0, COMMA(dEH));
		Dom.DimLineTo(dEH, 0, _T(""));
		if(dFHEx > 0)
		{
			Dom.DimLineTo(dFHEx, 0, COMMA(dFHEx));
			Dom.DimLineTo(dFH-dFHEx, 0, COMMA(dFH-dFHEx));
		}
		else
		{
			Dom.DimLineTo(dFH, 0, COMMA(dFH));
		}
		if(dFHu > 0)
		{
			Dom.DimLineTo(dFHu, 0, COMMA(dFHu));
		}
		Dom.DimLineTo(dLenWall, 0, COMMA(dLenWall));
		Dom.DimLineToExtend(heightTotal, 0, _T(""), COMMA(heightTotal));
		Dom.DimLineTo(dTS, 0, COMMA(dTS));

		Dom.DimMoveTo(dDimX, dDimY+dEH, 1);
		Dom.DimLineTo(dHeight, 1, COMMA(dHeight));
	}

	// 우측
	Dom.SetDirection(_T("RIGHT"));
	dDimX = nJijum==0 ? -A[0].x : A[nCountHDan].x;
	dDimY = dEL - dEH;
	dHeight = (nJijum==0 ? xyLeft.y : xyRight.y) - dEL;
	dLenWall = dHeight - (dTS + heightTotal + dFH + dFHu);
	if(bRight)
	{
		Dom.DimMoveTo(dDimX, dDimY, 1, COMMA(dEH));
		Dom.DimLineTo(dEH, 1, _T(""));
		if(dFHEx > 0)
		{
			Dom.DimLineTo(dFHEx, 1, COMMA(dFHEx));
			Dom.DimLineTo(dFH-dFHEx, 1, COMMA(dFH-dFHEx));
		}
		else
		{
			Dom.DimLineTo(dFH, 1, COMMA(dFH));
		}
		if(dFHu > 0)
		{
			Dom.DimLineTo(dFHu, 1, COMMA(dFHu));
		}
		Dom.DimLineTo(dLenWall, 1, COMMA(dLenWall));
		Dom.DimLineToExtend(heightTotal, 1, _T(""), COMMA(heightTotal));
		Dom.DimLineTo(dTS, 1, COMMA(dTS));

		Dom.DimMoveTo(dDimX, dDimY+dEH, 2);
		Dom.DimLineTo(dHeight, 2, COMMA(dHeight));
	}

	// 도로 중심
	double dElCenter = pBri->GetXySlabUpperActByDist(sta, 0, FALSE, FALSE, FALSE, TRUE).y;
	// 경사교의 경우 도로 중심 치수기입시 경사길이 말고 수직높이 치수를 괄호안에 기입해 준다.
	BOOL bDimVert		= !Compare(vAngWall.GetAngleDegree(), 90.0, "=");

	CString sDir = _T("RIGHT");
	if(nJijum == 0 && fabs(pBri->m_dWidthSlabRight) > fabs(pBri->m_dWidthSlabLeft))
		sDir = _T("LEFT");
	else if(nJijum != 0 && fabs(pBri->m_dWidthSlabLeft) > fabs(pBri->m_dWidthSlabRight))
		sDir = _T("LEFT");

	Dom.SetDirection(sDir);
	Dom.SetDimExo(0);
	Dom.SetDimObq(15);
	dDimX = 0;//-pBri->GetMidCrossSection(TRUE);
	dDimY = dEL - dEH;
	dHeight = dElCenter - dEL;
	dLenWall = dHeight - (dTS + heightTotal + dFH + dFHu);
	if(bCenterDim)
	{
		CDoubleArray dArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		CDPoint xyStt	= CDPoint(dDimX, dDimY);
		long nDimDan	= 0;


		dArrDim.Add(dEH);
		sArrText.Add(COMMA(dEH));
		sArrUnderText.Add(COMMA2(dEH*vAngWall.y));

		if(dFHEx > 0)
		{
			dArrDim.Add(dFHEx);
			sArrText.Add(COMMA(dFHEx));
			sArrUnderText.Add(COMMA2(dFHEx*vAngWall.y));

			dArrDim.Add(dFH-dFHEx);
			sArrText.Add(COMMA(dFH-dFHEx));
			sArrUnderText.Add(COMMA2((dFH-dFHEx)*vAngWall.y));
		}
		else
		{
			dArrDim.Add(dFH);
			sArrText.Add(COMMA(dFH));
			sArrUnderText.Add(COMMA2(dFH*vAngWall.y));
		}

		if(dFHu > 0)
		{
			dArrDim.Add(dFHu);
			sArrText.Add(COMMA(dFHu));
			sArrUnderText.Add(COMMA2(dFHu*vAngWall.y));
		}

		dArrDim.Add(dLenWall);
		sArrText.Add(bAllBridge ? COMMA(dLenWall) : "VAR");
		sArrUnderText.Add(bAllBridge ? COMMA2(dLenWall*vAngWall.y) : "(VAR)");

		dArrDim.Add(heightTotal);
		sArrText.Add(COMMA(heightTotal));
		sArrUnderText.Add(COMMA2(heightTotal*vAngWall.y));

		dArrDim.Add(dTS);
		sArrText.Add(COMMA(dTS));
		sArrUnderText.Add(COMMA2(dTS*vAngWall.y));

		dArrDim.Add(dTP);
		sArrText.Add(COMMA(dTP));
		sArrUnderText.Add(COMMA2(dTP*vAngWall.y));

		if(!bDimVert)
		{
			sArrUnderText.RemoveAll();
		}

		CHgBaseDrawStd baseDraw;
		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);

		/*

		Dom.DimMoveTo(dDimX, dDimY, 0, COMMA(dEH));
		Dom.DimLineTo(dEH, 0, _T(""));
		Dom.DimLineTo(dFH, 0, COMMA(dFH));
		bAllBridge ? Dom.DimLineTo(dLenWall, 0, COMMA(dLenWall)) : Dom.DimLineTo(dLenWall, 0, _T("VAR"));
		Dom.DimLineToExtend(heightTotal, 0, _T(""), COMMA(heightTotal));
		Dom.DimLineTo(dTS, 0, COMMA(dTS));
		Dom.DimLineToOver(dTP, 0, COMMA(dTP));
		*/

		Dom.DimMoveTo(dDimX, dDimY+dEH, 1);
		if(bDimVert)
		{
			bAllBridge ? Dom.DimLineToExtend(dHeight+dTP, 1, COMMA(dHeight+dTP), COMMA2((dHeight+dTP)*vAngWall.y)) : Dom.DimLineTo(dHeight+dTP, 1, _T("VAR"));
		}
		else
		{
			bAllBridge ? Dom.DimLineTo(dHeight+dTP, 1, COMMA(dHeight+dTP)) : Dom.DimLineTo(dHeight+dTP, 1, _T("VAR"));
		}
	}

	// 도로 중심 마크
	if(bCenterMark)
	{
		double dHeightCentralWall	= pBri->GetMaxHeightCentralWall();
		dElCenter += dHeightCentralWall;

		CDPoint xyCenter = CDPoint(dDimX, dElCenter+dTP+Dom.Always(15));
		
		m_pStd->MarkCenterOfRoad(&Dom, xyCenter, dElCenter+dTP-dEL+Dom.Always(25));
	}

	*pDomP << Dom;
}

void CDrawCrossSection::DimFrontBottom(CDomyun* pDomP, int nJijum, long nDimDan, BOOL bInput)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetDirection(_T("BOTTOM"));

	CFootingApp* pFoot = pBri->GetFootingByJijumNumber(nJijum);
	if(!pFoot) return;

	if(pBri->m_nTypeBridge==BRIDGE_TYPE_PORTAL)
	{
		if(!pFoot->m_bIs) return;

		double sta = pBri->GetStationOnJijum(nJijum, 0);
		CDPoint vAng = pBri->GetAngleJijum(nJijum);

		// (ARCBRIDGE-3237) 벽체 배면위치의 기초를 표현 하기 위해서 벽체 전면에서의 Offset을 구해서 처리한다.
		double dOffsetDist(0);
		if(nJijum == 0 || nJijum == pBri->m_nQtyJigan)
		{
			if(pBri->m_nTypeWallForRebarGen == 0)
			{
				dOffsetDist = (pFoot->m_dML + pFoot->m_dMR) * (nJijum == 0 ? 1 : -1);
			}
		}
		if(Compare(dOffsetDist, 0.0, _T("=")) == FALSE)
		{
			pFoot->SyncFooting(Compare(vAng.y, 0.0, _T("=")), vAng.GetAngleDegree(), FALSE, FALSE, dOffsetDist);
		}

		CTwinVectorArray tvArr;
		pBri->GetTvCrossSection(tvArr, sta, vAng, FALSE, TRUE, FALSE);

		CTwinVector tvLeft, tvRight;
		double dFH = pFoot->m_dFH;
		double dEL = pFoot->m_dEL - dFH;	// pFoot->m_dEL은 기초 상단 EL
		double dDFL = Round(pFoot->m_dMarginLeft, 0);
		double dDFR = Round(pFoot->m_dMarginRight, 0);
		CDPoint vLeft = pFoot->m_vAngLeft.BetweenAngle(vAng);
		CDPoint vRight = pFoot->m_vAngRight.BetweenAngle(vAng);
		vLeft.y = fabs(vLeft.y);
		vRight.y = fabs(vRight.y);
		
		CTwinVectorArray tvArrFrontExFooting;
		pFoot->m_exFooting.GetTvFront(TRUE, TRUE);
		CTwinVector tvLeftEx, tvRightEx;
		pFoot->m_exFooting.m_tvArrSide.GetTvByInfo("확대기초좌측면", tvLeftEx);
		pFoot->m_exFooting.m_tvArrSide.GetTvByInfo("확대기초우측면", tvRightEx);

		double dEH = pFoot->m_exFooting.m_dEFHR;
		tvArr.GetTvByInfo(_T("슬래브좌측면 횡단0"), tvLeft);
		tvArr.GetTvByInfo(_T("슬래브우측면 횡단0"), tvRight);
		CDPoint xyLeft = tvLeft.m_v2;
		CDPoint xyRight = tvRight.m_v2;

		CTwinVectorArray tvArrSideFooting, tvArrHidden;
		pFoot->GetTvLeftSide(tvArrSideFooting, tvArrHidden, FALSE, 0, 1, FALSE, FALSE, FALSE, dOffsetDist);
		CTwinVector tvLowerFooting;
		if(tvArrSideFooting.GetTvByInfo("기초하면", tvLowerFooting))
		{
			xyLeft	= tvLowerFooting.GetXyLeft();
			xyRight	= tvLowerFooting.GetXyRight();
		}

		CDPoint xyStt(tvLeftEx.GetXyBottom().x, dEL-dEH);
		CDoubleArray dArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		long nDimDan	= 0;

		// 32691 
		CTwinVectorArray tvArrSide;
		CTwinVector tvExFootLeft, tvExFootRight;

		tvArrSide.AddFromTvArray(pFoot->m_exFooting.m_tvArrSide);

		tvExFootLeft = tvArrSide.GetTvByInfo(_T("확대기초좌측면"));
		tvExFootRight = tvArrSide.GetTvByInfo(_T("확대기초우측면"));

		double dExFootLeftBottom = 0 , dExFootRightBottom = 0;

		dExFootLeftBottom = tvExFootLeft.GetXyBottom().y;
		dExFootRightBottom = tvExFootRight.GetXyBottom().y;

		if(!Compare(dExFootLeftBottom, dExFootRightBottom, _T("="),0.001))
			nDimDan = 2;
		


		CString sDir	= _T("BOTTOM");
		CHgBaseDrawStd baseDraw;
		if(pFoot->m_exFooting.m_nType == EXFOOTING_TYPE_PILE)
		{
			xyStt.y -= pFoot->m_exFooting.m_Pile.GetCuttingLength(pFoot->m_exFooting.m_Pile.m_dLength);
		}
		

		double dXLeft	= xyLeft.x;
		double dXLeftEx	= tvLeftEx.GetXyBottom().x;
		double dXRight	= xyRight.x;
		double dXRightEx	= tvRightEx.GetXyBottom().x;

		if(dXLeftEx > dXLeft)
			GetSwap(dXLeft, dXLeftEx);
		if(dXRightEx < dXRight)
			GetSwap(dXRight, dXRightEx);


		dArrDim.Add(dXLeft - dXLeftEx);
		if(dDFL > 0)
			dArrDim.Add(dDFL);

		dArrDim.Add((dXRight-dXLeft)-(dDFL > 0 ? dDFL : 0) - (dDFR > 0 ? dDFR : 0));
		if(dDFR > 0)
			dArrDim.Add(dDFR);
		dArrDim.Add(dXRightEx - dXRight);
		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, FALSE, FALSE);
		

		/*
		Dom.DimMoveTo(xy.x, xy.y, nDimDan, COMMA(xyLeft.x - tvLowerEx.GetXyLeft().x));
		Dom.DimLineTo(xyLeft.x - tvLowerEx.GetXyLeft().x, nDimDan, _T(""));
		Dom.DimLineTo(dDFL, nDimDan, COMMA(dDFL));
		Dom.DimLineTo((xyRight.x-xyLeft.x)-dDFL-dDFR, nDimDan, COMMA((xyRight.x-xyLeft.x)-dDFL-dDFR));
		Dom.DimLineTo(dDFR, nDimDan, COMMA(dDFR));
		Dom.DimLineToOver(tvLowerEx.GetXyRight().x - xyRight.x, nDimDan, COMMA(tvLowerEx.GetXyRight().x - xyRight.x));

		if(dDFL>0 || dDFR>0)
		{
			Dom.DimMoveTo(tvLowerEx.GetXyLeft().x, xy.y, nDimDan+1);
			Dom.DimLineTo(tvLowerEx.GetHorLength(), nDimDan+1, COMMA(tvLowerEx.GetHorLength()));
		}
		*/
	}
	else if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		double dSta = pBri->GetStationOnJijum(nJijum, 0);
		CDPoint vAng = pBri->GetAngleJijum(nJijum);

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

		CDPoint A[30];
		pBri->GetXySlabUpperAct(dSta, vAng, A, FALSE, TRUE);

		long nQtyHdan = pBri->GetQtyHDanAct(dSta, vAng);
		double dLenSlab = A[nQtyHdan].x - A[0].x;
		double dTotLenSlab = dLenSlab + pFoot->m_exFooting.m_dEFWB + pFoot->m_exFooting.m_dEFWF;

		BOOL bPile = pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_PILE;
		double dDrawLen = bPile ? 3000 : Dom.Always(5);

		Dom.DimMoveTo(A[0].x - pFoot->m_exFooting.m_dEFWB, pFoot->m_dEL - pFoot->GetHeight() - dDrawLen, 0);
		Dom.DimLineTo(pFoot->m_exFooting.m_dEFWB, 0, COMMA(pFoot->m_exFooting.m_dEFWB));
		Dom.DimLineTo(dLenSlab, 0, COMMA(dLenSlab));
		Dom.DimLineTo(pFoot->m_exFooting.m_dEFWF, 0, COMMA(pFoot->m_exFooting.m_dEFWF));

		Dom.DimMoveTo(A[0].x - pFoot->m_exFooting.m_dEFWB, pFoot->m_dEL - pFoot->GetHeight() - dDrawLen, 1);
		Dom.DimLineTo(dTotLenSlab, 1, COMMA(dTotLenSlab));
	}

	*pDomP << Dom;
}



void CDrawCrossSection::MarkFrontEl(CDomyun* pDomP, int nJijum, BOOL bLeft, BOOL bRight, BOOL bAngleApply)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	double sta = pBri->GetStationOnJijum(nJijum, 0);
	CDPoint vAng = bAngleApply ? pBri->GetAngleJijum(nJijum) : CDPoint(0,1);

	// 경사교의 경우 벽체를 세워서 그리기 때문에
	// 그리는 것과 실체 EL은 차이가 있다.
	CDPoint xyFootingLower	= CDPoint(0, 0);
	CFootingApp* pFoot = pBri->GetFootingByJijumNumber(nJijum);
	if(!pFoot) return;
	if(!pFoot->m_bIs) return;
	xyFootingLower.y	= pBri->GetElevationFootingLower(sta, TRUE);
	if(nJijum == 0 || nJijum == pBri->m_nQtyJigan)
	{
		if(pBri->m_nTypeWallForRebarGen == 0)
		{
			sta		= pBri->GetStationByTarget(nJijum ==0 ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0, TRUE);
			vAng	= pBri->GetAngleByTarget(nJijum == 0 ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0);
		}
		else if(pBri->m_nTypeWallForRebarGen == 1)
		{
			sta		= pBri->GetStationByTarget(nJijum == 0 ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_END_END, 0, TRUE);
			vAng	= pBri->GetAngleByTarget(nJijum == 0 ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_END_END, 0);
		}
		xyFootingLower.y = pBri->GetElevationFootingLower(sta, TRUE);
	}
	else 
	{
		// 경사교의 경우에는 좌측을 기준으로 횡단면도 그린다.
		CWallApp *pWall	= pBri->GetInWall(nJijum-1);
		if(pWall->m_nType == WALL_TYPE_SLOPE)
		{
			CLineInfo *pLine	= pBri->GetLineBase();
			CDPoint xyOrg		= pWall->GetXyOrgTopForFront(TRUE);
			CDPoint xyMatch		= xyOrg;
			CDPoint vAngWall	= ToDPointFrDegree(pWall->GetAngleWallByHor(TRUE));
			double dTP			= pBri->GetThickPave();
			pLine->GetXyMatchByJongDanXyAndAngle(xyOrg, vAngWall, xyMatch, -dTP);
			sta	= xyMatch.x;

			CRebarPlacing rb;
			GetXyMatchLineAndLine(xyMatch, vAngWall, xyFootingLower, CDPoint(1, 0), xyFootingLower);
			rb.RotateXy(xyFootingLower, xyOrg, 90-vAngWall.GetAngleDegree());
		}
	}
	if(!bAngleApply)
		vAng	= CDPoint(0, 1);

	CDPoint A[30];
	pBri->GetXySlabUpperAct(sta, vAng, A, FALSE, TRUE);


	double dEL = pBri->GetElevationFootingLower(sta, TRUE);	// pFoot->m_dEL은 기초 상단 EL
	double dEF = pFoot->m_exFooting.m_dEFWL;
	double dTP = pBri->GetThickPave(FALSE);
	int nCountHDan = pBri->GetQtyHDanAct(sta,vAng);

	CDPoint xyLeft   = A[0];
	CDPoint xyRight  = A[nCountHDan];
	CDPoint xyCenter = pBri->GetXySlabUpperActByDist(sta, 0);
	CDPoint xyFoot   = CDPoint(nJijum==0 ? xyLeft.x : xyRight.x, xyFootingLower.y);

	CDomyun Dom(pDomP);
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);

	double dX = Dom.Always(3);
	xyLeft.x   -= dX;
	xyRight.x  += dX;
	xyCenter.y += dTP;
	xyFoot.x   += (nJijum==0 ? -dX : dX);

	CString strLeft   = m_pStd->GetStringEL(toM(xyLeft.y));
	CString strRight  = m_pStd->GetStringEL(toM(xyRight.y));
	CString strCenter = m_pStd->GetStringEL(toM(xyCenter.y));
	CString strFoot   = m_pStd->GetStringEL(toM(dEL));

	Sym.ArrowElevation(&Dom, strLeft, _T(""), xyLeft);
	Dom.LineTo(xyLeft-CDPoint(Dom.Always(2), 0), xyLeft+CDPoint(Dom.Always(2), 0));

	Sym.ArrowElevation(&Dom, strRight, _T(""), xyRight, FALSE);
	Dom.LineTo(xyRight-CDPoint(Dom.Always(2), 0), xyRight+CDPoint(Dom.Always(2), 0));

	Sym.ArrowElevation(&Dom, strCenter, _T(""), xyCenter, nJijum!=0);
	// 기초
	Sym.ArrowElevation(&Dom, strFoot, _T(""), xyFoot, nJijum==0);
	Dom.LineTo(xyFoot-CDPoint(Dom.Always(2), 0), xyFoot+CDPoint(Dom.Always(2), 0));

	if(nJijum==0)
		Dom.Mirror(0, TRUE);

	*pDomP << Dom;
}

void CDrawCrossSection::DimLongi(CDomyun* pDomP)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	CDomyun Dom(pDomP);
	long nDimDanFootingLowerForPile	= 0;	// 말뚝일때 기초 하면 치수 단 수

	// 말뚝 비율을 기본값으로
	pBri->m_footingEnd.m_exFooting.m_Pile.InitScaleCutting();

	// 모든 객체들의 동기화를 해줌.
	//pBri->SyncBridge(0, TRUE);
//	pBri->GetTvVertSection_Std(0, TRUE, FALSE);
	pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 2);
	
	int nCount = pBri->GetCountInWall();
//	int nIdxHunch = 0;
	int i=0; for(i=0; i<nCount; i++)
	{
		CWallApp *pWall = pBri->GetInWall(i);

		if(pWall->m_nType == WALL_TYPE_SLOPE)
		{
			m_pStd->m_pDrawWall->DimMidFront(&Dom, pWall, FALSE);
		}
		else
		{
			m_pStd->m_pDrawWall->DimLeftFront(&Dom, pWall);
		}
		if(pWall->m_footing.m_bIs && !pWall->m_footing.IsVirtualFooting())
		{
			m_pStd->m_pDrawExFooting->DimFrontLower(&Dom, &pWall->m_footing.m_exFooting, pWall->m_footing.m_exFooting.m_nType == EXFOOTING_TYPE_PILE ? nDimDanFootingLowerForPile : 0);
			m_pStd->m_pDrawFooting->DimTextFrontEL(&Dom, &pWall->m_footing, FALSE, FALSE);
			m_pStd->m_pDrawFooting->DimFrontUpper(&Dom, &pWall->m_footing);
			
			if(pBri->GetJijumConnectRibAndSlab() != i+1)
				DimLongiInWallSide(&Dom, pWall);
			else
			{
				m_pStd->m_pDrawFooting->DimFrontSide(&Dom, &pWall->m_footing, 0, TRUE, FALSE);
			}
		}
		else
		{

		}
	}

	// 종단면 치수
	// 교량
	m_pStd->m_pDrawVertSection->DimVertSectionJongSlope(&Dom);
	m_pStd->m_pDrawVertSection->DimTextVerSectionEL(&Dom);
	m_pStd->m_pDrawVertSection->DimVertSectionUpper(&Dom, 1, TRUE, FALSE);
	m_pStd->m_pDrawVertSection->DimVertSectionUpperSlabThick(&Dom, TRUE, 0);
	m_pStd->m_pDrawVertSection->DimVerSectionHunch(&Dom, 0, TRUE, FALSE);
	m_pStd->m_pDrawVertSection->DimVerSectionHunch(&Dom, 0, FALSE, FALSE);

	if(pBri->IsBoxType())
	{
		m_pStd->m_pDrawVertSection->DimVertSectionLowerSlab(&Dom, 0, TRUE, TRUE, FALSE);
		m_pStd->m_pDrawVertSection->DimVerSectionHunch(&Dom, 0, TRUE, FALSE, FALSE);
		m_pStd->m_pDrawVertSection->DimVerSectionHunch(&Dom, 0, FALSE, FALSE, FALSE);
		m_pStd->m_pDrawVertSection->DimVerSectionBoxSideStt(&Dom, 0, FALSE);
		m_pStd->m_pDrawVertSection->DimVerSectionBoxSideEnd(&Dom, 0, FALSE);
		m_pStd->m_pDrawVertSection->DimVerSectionLower(&Dom, 0);
	}
	else
	{
		m_pStd->m_pDrawVertSection->DimVerSectionSideStt(&Dom, 0, FALSE);
		m_pStd->m_pDrawVertSection->DimVerSectionSideEnd(&Dom, 0, FALSE);
	}

	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		m_pStd->m_pDrawVertSection->DimTextRibInfo(&Dom);
		m_pStd->m_pDrawVertSection->DimVertSectionArchRibThick(&Dom);
	}

	// 기초
	m_pStd->m_pDrawFooting->DimTextFrontEL(&Dom, &pBri->m_footingStt, FALSE, FALSE);
	m_pStd->m_pDrawFooting->DimTextFrontEL(&Dom, &pBri->m_footingEnd, TRUE, FALSE);
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
	{
		m_pStd->m_pDrawFooting->DimFrontUpper(&Dom, &pBri->m_footingStt);
		m_pStd->m_pDrawFooting->DimFrontUpper(&Dom, &pBri->m_footingEnd, 0, FALSE, TRUE);
	}
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 || pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		m_pStd->m_pDrawFooting->DimFrontSide(&Dom, &pBri->m_footingStt, 0, TRUE, FALSE, FALSE);
		m_pStd->m_pDrawFooting->DimFrontSide(&Dom, &pBri->m_footingEnd, 0, TRUE, FALSE, FALSE);
	}
	
	// 확대기초
	m_pStd->m_pDrawExFooting->DimFrontLower(&Dom, &pBri->m_footingStt.m_exFooting, pBri->m_footingStt.m_exFooting.m_nType == EXFOOTING_TYPE_PILE ? nDimDanFootingLowerForPile : 0);
	m_pStd->m_pDrawExFooting->DimFrontLower(&Dom, &pBri->m_footingEnd.m_exFooting, pBri->m_footingEnd.m_exFooting.m_nType == EXFOOTING_TYPE_PILE ? nDimDanFootingLowerForPile : 0);

	// 내측헌치
	for(i=0; i<nCount; i++)
	{
		CWallApp *pWall = pBri->GetInWall(i);

		int nIdxHunch = pBri->GetIdxHunchInWall(i, TRUE);
		if(nIdxHunch>-1) 
		{
			m_pStd->m_pDrawVertSection->DimVerSectionInHunch(&Dom, 0, nIdxHunch, FALSE);
			if(pWall->m_nType == WALL_TYPE_V)
				m_pStd->m_pDrawVertSection->DimVerSectionInHunch(&Dom, 0, nIdxHunch+1, FALSE);
			if(pBri->IsBoxType())
				m_pStd->m_pDrawVertSection->DimVerSectionInHunch(&Dom, 0, nIdxHunch, FALSE, FALSE);
		}
	}
	

	// 아치
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		m_pStd->m_pDrawVertSection->DimVertSectionArchRiblAndf(&Dom, 0, FALSE);
	}

	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE);
	*pDomP << Dom;
}

void CDrawCrossSection::DimLongiInWallSide(CDomyun* pDomP, CWallApp* pWall)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	if(!pBri) return;
	if(!pBri->IsOutWall(TRUE)) return;
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDomP, &Dom);
	
	CTwinVector tv;

	CDoubleArray dArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("LEFT");
	CDPoint xyStt(0, 0);
	CDPoint xySttTot(0, 0);

	// 치수기입 시작위치는 기초가 있을 경우 기초의 확대기초를 기준으로 한다. 
	CExFootingApp *pExFooting	= &pWall->m_footing.m_exFooting;
	CDRect rect	= pExFooting->m_tvArrFront.GetRect();
	
	if(pExFooting->m_nType==EXFOOTING_TYPE_MASS) 
	{
		CExFootingApp *pExFooting = &pWall->m_footing.m_exFooting;
		double dH = pExFooting->GetMaxHeight();
		double dHCur = 0;
		long i=0; for(i=0; i<pExFooting->m_nCountDan; i++)
		{
			dArrDim.Add(pExFooting->m_dEFH[i]);
			dHCur += pExFooting->m_dEFH[i];
		}

		dArrDim.Add(dH-dHCur);
	}
	else 
	{
		double dH = rect.top - rect.bottom;
		dArrDim.Add(dH);
	}
	xyStt = CDPoint(rect.left, rect.bottom);

	pWall->m_footing.m_tvArrFront.GetTvByInfo("기초좌측면", tv);
	dArrDim.Add(tv.GetVerLength());
	xySttTot.y = tv.GetXyBottom().y;

	int nIndex = pBri->GetIdxInWall(pWall);
	double sta = pBri->GetStationOnJijum(nIndex+1, 0);
	double dElWall = pBri->GetXyMatchLineAndSlabUpper(CDPoint(sta, 0), CDPoint(0, 1), 0, 0).y;
	
	dArrDim.Add(dElWall-pBri->m_dTS-tv.m_v2.y);
	dArrDim.Add(pBri->m_dTS);

	// 포장 두께
	dArrDim.Add(pBri->GetThickPave(FALSE));

	CHgBaseDrawStd baseDraw;
	baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, 0, sDir, -1, FALSE, FALSE, TRUE);

	// 교량측면 치수
	Dom.SetDirection("LEFT");
	xyStt.y = xySttTot.y;
	double dist = dElWall-xySttTot.y; 
	if(dist>0)
	{
		Dom.DimMoveTo(xyStt.x, xyStt.y, 1);
		Dom.DimLineTo(dist, 1, COMMA(dist));
	}

	*pDomP << Dom;
}

void CDrawCrossSection::DimCrossSide(CDomyun* pDomP)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	CDPoint A[30];
	double sta = pBri->GetStationBridgeMid();
	pBri->GetXySlabUpperAct(sta, CDPoint(0, 1), A);

	CGuardWallRC* pGW = pBri->GetGuardWallByHDan(0);
	if(!pGW) return;

	double dH = pGW->m_dH1+pGW->m_dH2+pGW->m_dH3;	// 전체 높이
	double dTS = pBri->m_dTS;
	double heightTotal = pBri->m_dHHS;
	CDPoint xyDim = A[0].Rotate90();
	xyDim.MirrorVert();

	CDomyun Dom(pDomP);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.DimMoveTo(xyDim.x-dH, xyDim.y, 0);
	Dom.DimLineTo(dH, 0, COMMA(dH));
	Dom.DimLineTo(dTS, 0, COMMA(dTS));
	Dom.DimLineTo(heightTotal, 0, COMMA(heightTotal));

	Dom.DimMoveTo(xyDim.x, xyDim.y, 1);
	Dom.DimLineTo(dTS+heightTotal, 1, COMMA(dTS+heightTotal));

	*pDomP << Dom;
}

// void CDrawCrossSection::DimGuardWallSide(CDomyun* pDomP)
// {
// 	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
// 
// 	CDPoint A[30];
// 	CDPoint vAng(0, 1);
// 	double sta = pBri->GetStationBridgeMid();	
// 	
// 	pBri->GetXySlabUpperAct(sta, vAng, A);
// 
// 	int nCountHDan = pBri->GetQtyHDan(FALSE);
// 	long nGW	= pBri->m_nSeparBri == 2 ? pBri->GetQtyGuardWall()-1: 0;
// 	CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nGW);
// 	if(!pGW) return;
// 	int nHDanAct = pBri->GetNumSlabNodeAct(sta, vAng, nCountHDan);
// 
// 	// 보도가 너무 크면 원이 너무 커지므로 적당히 잘라준다.
// 	if(pBri->m_nSeparBri == 2)
// 		nHDanAct	= nCountHDan;
// 	else
// 		nHDanAct	= 1;
// 
// 	A[nHDanAct].MirrorHorz();
// 	CDPoint xyDim = A[nHDanAct];
// 	double dH1 = pGW->m_dH1;
// 	double dH2 = pGW->m_dH2;
// 	double dH3 = pGW->m_dH3;
// 	double heightTotal = dH1+dH2+dH3;				// 전체 높이
// 
// 	CDomyun Dom(pDomP);
// 
// 	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
// 
// 	Dom.SetDirection("LEFT");
// 	Dom.DimMoveTo(xyDim.x, xyDim.y, 0);
// 	Dom.DimLineTo(dH1, 0, COMMA(dH1));
// 	Dom.DimLineTo(dH2, 0, COMMA(dH2));
// 	Dom.DimLineTo(dH3, 0, COMMA(dH3));
// 
// 	// 방호벽
// 	long nQtyDim = 0;
// 	if(dH1 > 0)		nQtyDim++;
// 	if(dH2 > 0)		nQtyDim++;
// 	if(dH2 > 0)		nQtyDim++;
// 
// 	if(nQtyDim > 1)
// 	{
// 		Dom.DimMoveTo(xyDim.x, xyDim.y, 1);
// 		Dom.DimLineTo(heightTotal, 1, COMMA(heightTotal));
// 	}
// 
// 	*pDomP << Dom;
// }

void CDrawCrossSection::DimForCoverInput(CDomyun *pDomP, CString sArea, long nJ)
{
	CRcBridgeRebar *pBri	= m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CConcSymbol sym;
	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);

	CDoubleArray dArr;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir	= _T("");
	CDPoint xyStt	= CDPoint(0, 0);
	
	// 슬래브 상면
	if(sArea == "슬래브 측면")
	{
		sDir	= "TOP";
		double dCover	= pBri->m_dCoverSide_UpperSlab[0];
		double sta	= pBri->GetStationOnJijum(nJ);
		double dW	= pBri->GetWidthSlabAct(sta, CDPoint(0, 1), 0, FALSE, FALSE);

		dArr.Add(dCover);
		sArrText.Add("슬래브 측면");

		dArr.Add(dW - dCover - dCover);
		sArrText.Add("0");

		dArr.Add(dCover);
		sArrText.Add("슬래브 측면");

		CTwinVectorArray tvArrCross;
		pBri->GetTvCrossSection(tvArrCross, sta, CDPoint(0, 1), FALSE, TRUE, FALSE, 0, FALSE);
		CDRect rect	= tvArrCross.GetRect();
		xyStt.x	= rect.left;
		xyStt.y	= rect.top;
		long nDimDan	= 0;

		baseDraw.AutoDimCont(&Dom, xyStt, dArr, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
	}

	*pDomP << Dom;
}

// 성토사면 표시
void CDrawCrossSection::DrawCrossSectionUnderHeapSoilSymbol(CDomyun *pDomP, double sta, CDPoint vAng)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(pBri==NULL || !pBri->m_bUnderGroundRahmen) return;

	CDomyun Dom(pDomP);

	CTwinVectorArray tvArr,tvArrUnderRoad,tvArrNoriLeft,tvArrNoriRight;
	CRebarPlacing rPlace;

	pBri->GetTvCrossSection(tvArrUnderRoad,sta,vAng,FALSE,FALSE,FALSE,0,FALSE,FALSE,TRUE);	//지중라멘의 도로 철도 상면
	if(tvArrUnderRoad.GetSize() == 0) return;

	CDPoint xyRoadLeft = tvArrUnderRoad.GetAt(0).m_v1;
	CDPoint xyRoadRigh = tvArrUnderRoad.GetAt(tvArrUnderRoad.GetSize()-1).m_v2;
	long i=0; for(i=0; i<tvArrUnderRoad.GetSize(); i++)
		tvArr.Add(tvArrUnderRoad.GetAt(i));

	CTwinVector tv;
	pBri->GetTvCrossSectionUnderNori(tvArrNoriLeft,sta,vAng,TRUE);		//지중라멘의 좌측 노리선
	for(i=0; i<tvArrNoriLeft.GetSize(); i++)
		tvArr.Add(tvArrNoriLeft.GetAt(i));
	pBri->GetTvCrossSectionUnderNori(tvArrNoriRight,sta,vAng,FALSE);	//지중라멘의 우측 노리선
	for(i=0; i<tvArrNoriRight.GetSize(); i++)
		tvArr.Add(tvArrNoriRight.GetAt(i));

	if(tvArrNoriLeft.GetSize()==0)  return;
	if(tvArrNoriRight.GetSize()==0) return;
	
	CDPoint xyNoriLowLeft  = tvArrNoriLeft.GetAt(tvArrNoriLeft.GetSize()-1).m_v2;
	CDPoint xyNoriLowRight = tvArrNoriRight.GetAt(tvArrNoriRight.GetSize()-1).m_v2;

	double dElCenter= m_pStd->m_pARcBridgeDataStd->GetLineApp()->GetElevationPlanEl(sta);
	double dElSlab= pBri->GetLineBase()->GetElevationPlanEl(sta);
	tv.m_v1 = xyNoriLowLeft;
	tv.m_v2 = CDPoint(0, dElSlab);
	tvArr.Add(tv);
	tv.m_v1 = CDPoint(0, dElSlab);
	tv.m_v2 = xyNoriLowRight;
	tvArr.Add(tv);

	tvArr.Sort();

	CVectorArray vecArr;
	tvArr.GetToVectorArray(vecArr);
	//
	CTwinVectorArray tvArrDikeL,tvArrDikeR;
	//////////////////////////////////////////////////////////////////////////
	double dUpperEL = max(max(xyRoadLeft.y,xyRoadRigh.y),dElCenter) - pBri->m_dUnderGroundHeightPavement-100;
	double dLowerEL = min(xyNoriLowLeft.y,xyNoriLowRight.y) + pBri->GetThickPave(FALSE);
	double dLeftStt = xyNoriLowLeft.x;
	double dRightEnd= xyNoriLowRight.x;

	CDPoint xyCen   = CDPoint(dLeftStt,dLowerEL);
	double dOffsetX = 1000;
	double dOffsetY =  200;
	double dWidth   =  300;
	double dHeight  =   90;
	long nCountDraw = 0;
	// 아래에서 위로
	CDPoint xy[4];
	CDPoint vX(1,0),vY(0,1),xyArc1(0,0),xyArc2(0,0),xyArc3(0,0),xyMeet(0,0);
	while(1)
	{
		xyCen.y = dLowerEL + dHeight + dOffsetY*nCountDraw;
		xyCen.x = (nCountDraw%2) ? dLeftStt - dOffsetX : dLeftStt - dOffsetX/2;
		// 좌측에서 우측으로
		nCountDraw++;
		while(1)
		{
			xyCen.x += dOffsetX;
			xy[0] = CDPoint(xyCen.x-dWidth,xyCen.y+dHeight);
			xy[1] = CDPoint(xyCen.x+dWidth-230,xyCen.y+dHeight);
			xy[2] = CDPoint(xyCen.x+dWidth-230,xyCen.y-dHeight);
			xy[3] = CDPoint(xyCen.x-dWidth,xyCen.y-dHeight);
			BOOL bDrawSymbol = TRUE;
			long nCheck = 4;
			// 성토단면에 대한 CHECK
			long i=0; for(i=0; i<nCheck; i++)
			{
				bDrawSymbol = rPlace.CheckXyByVectorArrayArea(xy[i],vecArr,TRUE);
				if(!bDrawSymbol)
					break;
			}
			if(bDrawSymbol)
			{
				xyArc1 = xyCen - dWidth*vX;
				xyArc1 = xyArc1 - (dHeight*0.1)*vY;
				xyArc2 = xyCen;
				xyArc3 = (xyArc1+xyArc2)/2 + dHeight*vY;
				Dom.Arc(xyArc2.x,xyArc2.y,xyArc3.x,xyArc3.y,xyArc1.x,xyArc1.y);
				xyArc1 = xyCen + (dWidth*0.6)*vX;
				xyArc1 = xyArc1 + (dHeight*0.1)*vY;
				xyArc2 = xyCen;
				xyArc3 = (xyArc1+xyArc2)/2 - dHeight*0.6*vY;
				Dom.Arc(xyArc2.x,xyArc2.y,xyArc3.x,xyArc3.y,xyArc1.x,xyArc1.y);
				//
				Dom.Circle(xyCen-(dWidth*0.4)*vX,dHeight*0.2,TRUE);
			}
			if(xyCen.x>dRightEnd+300)	//hgh20050418 우측 여유치를 더 부여함.
				break;
		}
		if(xyCen.y>dUpperEL) 
			break;
	}

	*pDomP << Dom;
}

// 외측벽체 측면 구조도
// 외측벽체 제원을 내측벽체에 맞도록 convert해서 내측벽체 그리듯 그린다.
// 잘 되야 할텐데.
// 이렇게 하는 이유..
// 따로 외측벽체 내측벽체 따로 만들면 수정시 중복일이 될 수 있고, 벽체 구조도 자체가 일관성이 없어 질 수 있음.
CDRect CDrawCrossSection::DrawRebar_OutWall_CrossSectionStd(CDomyun *pDomP, BOOL bStt, BOOL bInnerAtLeft, BOOL bInnerAtRight, BOOL bMarkRebar, BOOL bDimRebar, BOOL bDrawMainRebar, BOOL bDrawSupportRebar, BOOL bDrawShearRebar, BOOL bInput)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return CDRect(0, 0, 0, 0);
	if(!pBri->IsOutWall(bStt)) return CDRect(0, 0, 0, 0);

	CDomyun Dom(pDomP);

	CWallApp wall;

	// convert : 외측벽체를 CWallApp에 맞게 제원을 설정해서 벽체를 만들어 냄.
	long nJ		= bStt ? 0 : pBri->m_nQtyJigan;
	BOOL bIsDivideFrontAndBackWall	= m_pStd->IsDivideFrontAndBackWall(nJ);
	// 전면 배면 폭이 다른 경우 벽체 기준이 의미가 없음
	long nTypeWallForRebarGen	= pBri->m_nTypeWallForRebarGen;
	if(bIsDivideFrontAndBackWall)
	{
		if(bInnerAtLeft)
			pBri->m_nTypeWallForRebarGen	= 1;
		else
			pBri->m_nTypeWallForRebarGen	= 0;
	}

	pBri->SyncBridge_CrossSection_SttEndFooting(TRUE);
	pBri->MakeWallByOutWall(wall, bStt, TRUE, FALSE);

	if(pBri->IsOutWallHunch(bStt))
	{
		CTwinVectorArray tvArrCutting;
		long nDrawLeft	= bInnerAtLeft ? -1 : (bInnerAtRight ? 1 : 0);
		if(nDrawLeft != 0)
			pBri->GetTvCuttingArea_Side_OutWallHunch(tvArrCutting, bStt, nDrawLeft);
		wall.m_tvArrCuttingArea_Side_For1Dan	= tvArrCutting;
	}

	// 그린다.
	CString sLeft	= !bInnerAtLeft ? "배면" : "전면";
	CString sRight	= !bInnerAtRight ? "배면" : "전면";

	// 가각부 처리
	double dWidthLeftExp	= pBri->GetWidthDiffSlabActOrgAndGagak(bStt, TRUE, bInnerAtLeft);
	double dWidthRightExp	= pBri->GetWidthDiffSlabActOrgAndGagak(bStt, FALSE, bInnerAtRight);
	double dWidthOverRebarExpLeft	= pBri->GetDiffLeftRightDistForGagakSabogang(bStt, TRUE);
	double dWidthOverRebarExpRight	= pBri->GetDiffLeftRightDistForGagakSabogang(bStt, FALSE);
	if(pBri->m_bIsExp)
	{
		wall.m_dWidthExp[iLEFT]		= bStt ? dWidthRightExp : dWidthLeftExp;
		wall.m_dWidthExp[iRIGHT]	= bStt ? dWidthLeftExp : dWidthRightExp;

		
		wall.m_dWidthOverRebarExp[iLEFT]	= dWidthOverRebarExpLeft;
		wall.m_dWidthOverRebarExp[iRIGHT]	= dWidthOverRebarExpRight;
	}
	
	BOOL bDrawCrossSection	 = !bIsDivideFrontAndBackWall;
	double dSta	= bInnerAtLeft ? pBri->GetStationByTarget(bStt ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_END_END, 0, TRUE) : pBri->GetStationByTarget(bStt ? BRIDGE_OFF_STT_SLAB: BRIDGE_OFF_END_SLAB, 0, TRUE);
	CDPoint vAng	= bInnerAtLeft ? pBri->GetAngleByTarget(bStt ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_END_END, 0) : pBri->GetAngleByTarget(bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0);
	double dDistL	= pBri->GetDistLineLeftOrRight(dSta, vAng, TRUE) + pBri->GetWidthSlabFrLineLeftOrRight(dSta, vAng, TRUE);
	double dDistR	= pBri->GetDistLineLeftOrRight(dSta, vAng, FALSE) + pBri->GetWidthSlabFrLineLeftOrRight(dSta, vAng, FALSE);
	
	double dDistFromCenter	= (dDistL + dDistR)/2;
	if(!bStt)
		dDistFromCenter *= -1;
	
	CDRect rect(0,0,0,0);
	if(pBri->IsBoxType())
	{
		// Box형은 벽체에서 일반도를 다 그릴수 없으니까 따로 그려준다.
		// 전면배면 분리일때도 그려줘야 하니까 무조건 그리는걸로~
//		if(bDrawCrossSection)
		{
			DrawCrossSection(&Dom, dSta, vAng, TRUE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE);
			DrawCrossSlabHunch(&Dom, dSta, vAng, FALSE, 0, FALSE, TRUE);

			long nLeft = (bStt == bInnerAtLeft)? 1 : 0;
			if(wall.m_dDiffTop[nLeft] != 0)
				Dom.Move(CDPoint(0, -wall.m_dDiffTop[nLeft]));

			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();
		}
		if(bStt)
			Dom.Mirror(wall.m_dCenterOfWall, TRUE);

		m_pStd->m_pDrawWall->DrawRebar_SideStd(&Dom, &wall, bStt ? !bInnerAtLeft : bInnerAtLeft, bStt ? !bInnerAtRight : bInnerAtRight, bMarkRebar, bDimRebar, sLeft, sRight, FALSE, bStt ? FALSE : TRUE, bDrawMainRebar, bDrawSupportRebar, bDrawShearRebar, bInput, dDistFromCenter);
	}
	else
	{
		rect	= m_pStd->m_pDrawWall->DrawRebar_SideStd(&Dom, &wall, bStt ? !bInnerAtLeft : bInnerAtLeft, bStt ? !bInnerAtRight : bInnerAtRight, bMarkRebar, bDimRebar, sLeft, sRight, bDrawCrossSection, bStt ? FALSE : TRUE, bDrawMainRebar, bDrawSupportRebar, bDrawShearRebar, bInput, dDistFromCenter);
	}

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	if(!bDrawCrossSection && !pBri->IsBoxType())
	{
		*pDomP << Dom;
		DrawCrossSection(&Dom, dSta, vAng, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE, TRUE);
		DrawCrossSlabHunch(&Dom, dSta, vAng, TRUE, 0, FALSE, TRUE, CDPoint(0 ,1));

		long nLeft = (bStt == bInnerAtLeft)? 1 : 0;
		if(wall.m_dDiffTop[nLeft] != 0)
			Dom.Move(CDPoint(0, -wall.m_dDiffTop[nLeft]));
		if(bStt)
			Dom.Mirror(wall.m_dCenterOfWall, TRUE);
		*pDomP << Dom;
	}

	// 헌치부 철근은 따로 그려줌
	// 배력근
	if(bInnerAtLeft)
		DrawRebarSuppor_CrossSection_OutWallHunch(&Dom, bStt, bMarkRebar, -1);
	if(bInnerAtLeft != bInnerAtRight && bInnerAtRight)
		DrawRebarSuppor_CrossSection_OutWallHunch(&Dom, bStt, bMarkRebar, 1);

	// 주철근
	for(long cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
	{
		if(bInnerAtLeft)
			DrawRebarMain_CrossSection_OutWallHunch(&Dom, cycle, bStt, bMarkRebar, -1);
		if(bInnerAtLeft != bInnerAtRight && bInnerAtRight)
			DrawRebarMain_CrossSection_OutWallHunch(&Dom, cycle, bStt, bMarkRebar, 1);
	}

	// 가각부 지원인 경우..
	BOOL bIsGagak	= wall.m_dWidthExp[iLEFT] > 0 || wall.m_dWidthExp[iRIGHT];
	if(pBri->m_bIsExp && bIsGagak)
	{
		// 일반도
		DrawCrossSection_OutWall_Exp(&Dom, bStt, bInnerAtLeft, bInnerAtRight, !bStt);

		// 철근
		DrawRebarMain_CrossSection_OutWallExp(&Dom, bStt, 0, bInnerAtLeft, bInnerAtRight, 0, TRUE, TRUE, !bStt, wall.m_tvArrRebarMainSideForDim);
	}


	*pDomP << Dom;

	pBri->m_nTypeWallForRebarGen	= nTypeWallForRebarGen;
	return rect;
}

// 가각부 일반도 횡단면
CDRect CDrawCrossSection::DrawCrossSection_OutWall_Exp(CDomyun *pDomP, BOOL bStt, BOOL bInnerAtLeft, BOOL bInnerAtRight, BOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return CDRect(0, 0, 0, 0);
	if(!pBri->IsOutWall(bStt)) return CDRect(0, 0, 0, 0);

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	CHgBaseDrawStd baseDraw;

	CTwinVectorArray tvArrLeft, tvArrRight;

	pBri->GetTvCrossSection_OutWall_Exp(tvArrLeft, bStt, TRUE, bInnerAtLeft, bLeft);
	pBri->GetTvCrossSection_OutWall_Exp(tvArrRight, bStt, FALSE, bInnerAtRight, bLeft);
	
	long nIdx	= tvArrLeft.GetIdxByInfo(bLeft ? "벽체우측" : "벽체좌측");
	if(nIdx > -1)
		tvArrLeft.RemoveAt(nIdx);

	nIdx	= tvArrRight.GetIdxByInfo(bLeft ? "벽체좌측" : "벽체우측");
	if(nIdx > -1)
		tvArrRight.RemoveAt(nIdx);

	baseDraw.DrawTvArray(&Dom, tvArrLeft, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());
	baseDraw.DrawTvArray(&Dom, tvArrRight, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());

	CDRect rect(0, 0, 0, 0);
	Dom.SetCalcExtRect();
	rect	= Dom.GetExtRect();

	*pDomP << Dom;

	return rect;
}

// 횡단면 가각부 배근도 그림
void CDrawCrossSection::DrawRebarMain_CrossSection_OutWallExp(CDomyun *pDomP, BOOL bStt, long nLeftWall, BOOL bInnerAtLeft, BOOL bInnerAtRight, long nDan, BOOL bDimRebar, BOOL bMarkRebar, BOOL bLeft, CTwinVectorArray &tvArrRebarMainSide)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(!pBri->IsOutWall(bStt)) return;

	CDomyun Dom(pDomP);
	CHgBaseDrawStd baseDraw;

	BOOL bLeftWall = bLeft ? nLeftWall == iLEFT : nLeftWall != iLEFT;


	// 철근 마크를 위해서 배력근 구함. //////
	CTwinVectorArray tvArrSup, tvArrSupAngle, tvArrSupAngleAdd;
	CWallApp wall;
	if(bMarkRebar)
	{
		
		// convert : 외측벽체를 CWallApp에 맞게 제원을 설정해서 벽체를 만들어 냄.
		pBri->MakeWallByOutWall(wall, bStt, TRUE);

		wall.GetTvRebarSupport_Side(tvArrSup, tvArrSupAngle, tvArrSupAngleAdd, bLeftWall, 0, TRUE, FALSE);
	}
	//////////////////////////////////////////

	// (ARCBRIDGE-2819) 주철근 평면 정보가 있어야 가각철근 범위를 구할 수 있다.
	pBri->SettingTvArrRebarMainPlane();

	// 치수 기입시 사용할 철근들..
	CTwinVectorArray tvArrRebar;

	long left = 0; for(left = 0; left < 2; left++)
	{
		BOOL bLeftWall	= left == iLEFT;

		long stt	= bStt ? iSTT : iEND;
		BOOL bInner	= bLeftWall ? (bStt ? bInnerAtRight : bInnerAtLeft) : (bStt ? bInnerAtLeft : bInnerAtRight);
//		long inner	= bInner ? iINNER : iOUTTER;
		long nSide = (bStt != bInner)? iLEFT : iRIGHT; // 벽체의 좌측면인지 우측면인지
//		long nCountRebar1	= pBri->m_pArrRCMainOutWall[0][stt][inner][nDan].GetSize()+1;
//		long nCountRebar2	= pBri->m_pArrRCMainOutWall[1][stt][inner][nDan].GetSize()+1;
		long nCountRebar1	= wall.m_pArrRCMain[0][nSide][nDan].GetSize();
		long nCountRebar2	= wall.m_pArrRCMain[1][nSide][nDan].GetSize();

		long nCountRebar	= max(nCountRebar1, nCountRebar2);
		CTwinVectorArray tvArrCycle1[2], tvArrCycle2[2], tvArrJoint;
		BOOL bUpperSlabRebar	= FALSE;
		BOOL bLowerSlabRebar	= FALSE;
		for(long rc = 0; rc < nCountRebar; rc++)
		{
 			pBri->GetTvRebarMain_CrossSection_OutWallExp(tvArrCycle1[0], tvArrCycle2[0], tvArrCycle1[1], tvArrCycle2[1], tvArrJoint, bStt, bLeftWall, bInner, nDan, rc, bLeft);
		
			for(long type = 0; type < 2; type++)
			{
				if(bDimRebar && rc == 0 && type == 0)
				{
					tvArrRebar.AddFromTvArray(tvArrCycle1[type]);
					tvArrRebar.AddFromTvArray(tvArrCycle2[type]);
				}

				CTwinVectorArray tvArrLine;
				for(long cycle = 0; cycle < 2; cycle++)
				{
					tvArrLine	= cycle == 0 ? tvArrCycle1[type] : tvArrCycle2[type];
					if(tvArrLine.GetSize() == 0) continue;
				
					// 철근 그리기
					m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
					long nCountRebarCur	= cycle == 0 ? nCountRebar1 : nCountRebar2;
					if(rc == 0 || rc == nCountRebarCur-1)
					{
						double dLenRound	= 0;
						CRebarInfoCycle *pRC	= wall.GetRebarInfoCycle(cycle, nDan, (bStt != bInner), rc);
						
						if(pRC)
						{
							dLenRound	= rc == 0 ? (pRC->m_xyMid2.y - pRC->m_xyMid1.y) : 
													(pRC->m_bUseEndExt ? (pRC->m_xyMidExt.y - pRC->m_xyEnd.y) : (pRC->m_xyMid2.y - pRC->m_xyMid1.y));
							
							bUpperSlabRebar	= (pRC->m_nPosStt == REBARINFO_POS_UPPERSLAB_UPPER || pRC->m_nPosEnd == REBARINFO_POS_UPPERSLAB_UPPER)? TRUE : FALSE;
							bLowerSlabRebar	= (pRC->m_nPosStt == REBARINFO_POS_LOWERSLAB_LOWER || pRC->m_nPosEnd == REBARINFO_POS_LOWERSLAB_LOWER)? TRUE : FALSE;

							//이음이 없을 경우
							if(fabs(dLenRound) > 0 && pRC->m_nPosStt == REBARINFO_POS_STTWALL_OUTTER && pRC->m_nPosEnd == REBARINFO_POS_ENDWALL_OUTTER && !bInner)
							{
								if(rc == nCountRebarCur-1 && pRC->GetLengthEnd())
								{
									bUpperSlabRebar = TRUE;
									dLenRound = fabs(dLenRound);
								}
								else
								{
									bLowerSlabRebar = TRUE;								
									dLenRound = fabs(dLenRound);
								}
							}
						}
						
						m_pStd->DrawTvArrWidthRound(&Dom, tvArrLine, dLenRound, rc == 0);
					}
					else
					{
						baseDraw.DrawTvArray(&Dom, tvArrLine, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());
					}

			

					// 벽체 외측면인 경우임 ////////////////////////////////
					// 가각철근이 선형방향 배치인 경우
					// 벽체 정면도 가각부에는 선형방향 철근만 표현되고
					// 선형방향 철근의 경우 1,2cycle 구분 없이 통째 집계되므로 
					// 벽체 정면도 철근 1,2cycle 모두 합쳐서 마킹을 해야 되고
					// 마킹 번호도 선형방향 번호로 바꿔야 됨
					BOOL bMarkLineDirGagak	= FALSE;
					BOOL bMarkEnable	= TRUE;
					if(!bInner && pBri->m_bLineDirRebarOfExp[stt][left])
					{
						bMarkLineDirGagak	= TRUE;
						
						/*#0031731 1, 2cycle가 같은 형태의 철근이라도 따로 마킹되도록 변경
						if(cycle == 1)
							bMarkEnable	= TRUE;
						else
							bMarkEnable	= FALSE;

						if(bMarkEnable)
						{
							if(tvArrCycle1[0].GetSize() > 0 && tvArrCycle2[0].GetSize())
							{
								tvArrLine.RemoveAll();
								tvArrLine.AddFromTvArray(tvArrCycle1[0]);
								for(long j = 0; j < tvArrCycle2[0].GetSize(); j++)
								{
									if((j*2)+1 < tvArrCycle2[0].GetSize())
										tvArrLine.InsertAt((j*2)+1, tvArrCycle2[0].GetAt(j));
									else
										tvArrLine.Add(tvArrCycle2[0].GetAt(j));
								}
							}
						}
						*/
					}
					/////////////////////////////////////////////////////////
					if(bMarkRebar && bMarkEnable)
					{
						m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
						CString sMark			= _T("");
						CString sDescription	= _T("");
						double dDia				= 0;
						if(bUpperSlabRebar)
							pBri->GetInfoMainRebarExp_UpperSlab(bStt, bLeftWall, TRUE, bMarkLineDirGagak ? 1 : cycle, nDan, type, sMark, dDia, sDescription);
						else if(bLowerSlabRebar)
							pBri->GetInfoMainRebarExp_LowerSlab(bStt, bLeftWall, FALSE, bMarkLineDirGagak ? 1 : cycle, nDan, type, sMark, dDia, sDescription);
						else
							pBri->GetInfoMainRebarExp_OutWall(bStt, bLeftWall, bInner, cycle, rc, sMark, dDia, sDescription, FALSE);

						CRebar *pRb = pBri->GetRebarMainByDescription(sDescription);

						CTwinVectorArray tvArrExp;
						pBri->GetTvCrossSection_OutWall_Exp(tvArrExp, bStt, bLeftWall, bInner, bLeft);
						CString sWall	= _T("");
						BOOL nTypeMarkDir	= TYPE_MARKDIR_LEFT;
						if(!bLeft)
						{
							sWall	= bLeftWall ? "벽체우측" : "벽체좌측";
							nTypeMarkDir	= bLeftWall ? TYPE_MARKDIR_RIGHT : TYPE_MARKDIR_LEFT;
						}
						else
						{
							sWall	= bLeftWall ? "벽체좌측" : "벽체우측";
							nTypeMarkDir	= bLeftWall ? TYPE_MARKDIR_LEFT : TYPE_MARKDIR_RIGHT;
						}

						CTwinVector tvOut	= tvArrExp.GetTvByInfo(sWall);
						CDPoint xyMatch(0, 0);

						CMarkDomyun mark;
						mark.SetDomyun(&Dom, m_pStd->m_pARoadOptionStd);
						CTwinVectorArray tvArrTmp;
						tvArrTmp	= tvArrLine;
						// (ARCBRIDGE-2822) 마지막 철근은 피복을 위한 철근이다. 마킹하지 않는다.
						tvArrTmp.RemoveAt(0);

						mark.m_tvArrLineRebar	= tvArrTmp;
						mark.SetRebar(tvArrTmp, tvArrSup);
						mark.m_sMark	= sMark;
						mark.m_sDia		= pBri->m_pARcBridgeDataStd->GetStringDia(dDia, pBri->GetValueFy(bStt? ePartOutWallStt : ePartOutWallEnd));

						mark.m_dPosScale				= type == 0 ? (cycle == 0 ? 0.3 : 0.6) : (cycle == 0 ? 0.5 : 0.8);
						mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_LOWER;
						mark.m_nTypeMarkDir				= nTypeMarkDir;
						CDPoint xyMark					= mark.GetXyMark();
						GetXyMatchLineAndLine(xyMark, CDPoint(1, 0), tvOut.m_v1, tvOut.GetXyDir(), xyMatch);
						mark.m_dAddHeight				= ~(xyMark - xyMatch);
						mark.m_dCtc						= pBri->GetCtcRebarMain(FALSE);
						mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;
						mark.MarkRebar();
					}
				}
			}

			// 이음 그리기
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);
			baseDraw.DrawTvArray(&Dom, tvArrJoint, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());
		}
	}

	// (ARCBRIDGE-2819) 벽체 주철근과 치수를 같이 넣었었는데 따로 넣는걸로 변경
	if(bDimRebar)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		CDPointArray xyArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		CString sDir	= _T("TOP");
		CDPoint xyStt	= CDPoint(0, 0);
		long nDimDan	= 0;

		CTwinVectorArray tvArrCrossExp;
		CTwinVector tvLeft, tvRight;
		if(nLeftWall != 0)
		{
			pBri->GetTvCrossSection_OutWall_Exp(tvArrCrossExp, bStt, bLeftWall, bLeft ? bInnerAtLeft : bInnerAtRight, bLeft);
			tvLeft	= tvArrCrossExp.GetTvByInfo("벽체좌측");
			tvRight	= tvArrCrossExp.GetTvByInfo("벽체우측");
		}
		else
		{
			pBri->GetTvCrossSection_OutWall_Exp(tvArrCrossExp, bStt, bLeft ? TRUE : FALSE, bInnerAtLeft, bLeft);
			tvLeft	= tvArrCrossExp.GetTvByInfo("벽체좌측");
			pBri->GetTvCrossSection_OutWall_Exp(tvArrCrossExp, bStt, bLeft ? FALSE : TRUE, bInnerAtRight, bLeft);
			tvRight	= tvArrCrossExp.GetTvByInfo("벽체우측");
		}

		
		xyStt	= tvLeft.GetXyTop();
		if(wall.m_tvArrLeftUpperSlab.GetSize() > 0)
			xyStt.y		= wall.m_tvArrLeftUpperSlab.GetRect().top;
		
//		(ARCBRIDGE-2819) 
// 		tvArrRebar.AddFromTvArray(tvArrRebarMainSide);

		if(bLeft == bStt)
		{
			xyArrDim.Add(tvLeft.GetXyTop());
		}
		
		long i = 0; for(i = 0; i < tvArrRebar.GetSize(); i++)
			xyArrDim.Add(tvArrRebar.GetAt(i).GetXyTop());

		if(bLeft != bStt)
		{
			xyArrDim.Add(tvRight.GetXyTop());
			xyArrDim.Sort(TRUE);
		}

		xyStt.x	= xyArrDim.GetAt(0).x;

		m_pStd->AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, TRUE, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, 90, TRUE, "", NULL, 0.00001);
	}


	*pDomP << Dom;
}

//슬래브면(교면)의 횡단 치수
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
void CDrawCrossSection::DimCrossSectionRailCenter(CDomyun* pDomP, double sta, CDPoint vAng, BOOL bInput)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	CString str = _T(""), strLow = _T(""), strSub = _T("");
	CString str1 = _T(""), str2 = _T("");

	CDPoint A[30];
	pBri->GetXySlabUpperAct(sta, vAng, A,pBri->m_bUnderGroundRahmen);

	CDPoint vAngSub = pBri->GetAngleByStation(sta);
	//vAngSub = CDPoint(Round(vAngSub.x, 5), Round(vAngSub.y, 5));
	double dSin = vAng==CDPoint(0, 1) ? 1/vAngSub.y : vAngSub.y;

	CDomyun Dom(pDomP);

	// 상부
// 	double dLen(0), dStrLen(0);
	BOOL bDimSub = FALSE;
	double dY = pBri->GetElMaxSlabUpper(sta, vAng,pBri->m_bUnderGroundRahmen)-Dom.Always(5);
	Dom.SetDirection("TOP");
	long nQtyRail = pBri->GetQtyRailNode();

	for(int n = 0; n<nQtyRail; n++)
	{
		str.Format("C%d", n+1);
		double dLen = pBri->GetLengthRailByLine(n)/vAng.y;
		strSub = vAngSub==CDPoint(0, 1) ? _T("") : _T("(")+COMMA(dLen*dSin)+_T(")");
		double dStrLen = Dom.GetTextWidth(COMMA(dLen)+strSub);
		bDimSub = dStrLen>dLen;

		strLow = (bInput || vAngSub==CDPoint(0, 1) || bDimSub) ? COMMA(dLen) : COMMA(dLen)+strSub;
		str1 = bInput ? str : strLow;
		str2 = bInput ? strLow : (!bDimSub ? _T("") : strSub);
		Dom.DimMoveTo(0, dY, n+1);
		Dom.DimLineToExtend(dLen, n+1, str1, str2);
	}
	*pDomP << Dom;
}

CDRect CDrawCrossSection::DrawRebar_InWall_SideStd(CDomyun *pDomP, long nJ, BOOL bLeftAtLeft, BOOL bLeftAtRight, BOOL bMarkRebar, BOOL bDimRebar, CString sLeft, CString sRight, BOOL bDrawCrossSection, BOOL bDrawMainRebar, BOOL bDrawSupportRebar, BOOL bDrawShearRebar, BOOL bInput)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;

	CWallApp *pWall	= pBri->GetInWall(nJ-1);
	if(!pWall) return CDRect(0, 0, 0, 0); 
	if(!pWall->m_bIs) return CDRect(0, 0, 0, 0); 
	if(pWall->m_bTopIsColumn)
	{
		return DrawRebar_Column_SideStd(pDomP, nJ, bMarkRebar, bDimRebar);
	}

	// 치수넣는 타입 동기화 /////////////////////////
	// 가각 지원인 경우 가운데를 판다
	if(pWall->m_dWidthExp[iLEFT] > 0 || pWall->m_dWidthExp[iRIGHT] > 0)
		pWall->m_nTypeOutputMainRebar_Side	= 1;
	else
		pWall->m_nTypeOutputMainRebar_Side	= 0;
	//////////////////////////////////////////////////


	double dSta	= pWall->m_xyOrg.x;
	// 경사교일 경우 상부슬래브 상면으로 뻗은 위치의 sta에서 횡단면도를 그려야 된다.
	CDPoint vAngWall	= ToDPointFrDegree(pWall->GetAngleWallByHor(TRUE));
	if(pWall->m_nType == WALL_TYPE_SLOPE && (bLeftAtLeft == bLeftAtRight))
	{
		// 경사교의 경우에는 좌측을 기준으로 횡단면도 그린다.
		CLineInfo *pLine	= pBri->GetLineBase();
		CDPoint xyOrg		= pWall->GetXyOrgTopForFront(bLeftAtLeft);
		CDPoint xyMatch		= xyOrg;
		double dTP			= pBri->GetThickPave();
		pLine->GetXyMatchByJongDanXyAndAngle(xyOrg, vAngWall, xyMatch, -dTP);
		dSta	= xyMatch.x;
	}
	if(pBri->IsBoxType() && !Compare(pWall->m_dDiffTop[0], pWall->m_dDiffTop[1], _T("="), 0.1))
	{
		// 좌측면과 우측면의 높이가 다르다면 그려지는 Station도 좌우측으로 롬겨주자.
		CDPoint vAng	= ToDPointFrDegree(pWall->GetAngleWallByHor(TRUE));
		dSta += pWall->m_dW/vAng.y/2. * (bLeftAtLeft? -1 : 1);
	}

	// 경사교는 세워서 그린다. /////////////////////////////////////////
	// 따라서 상부슬래브 횡단면도가 길어져야 됨.
//	double dHeightUpperSlab	= pBri->GetHeightUpperSlab_OldOrNotUse(dSta, vAngWall);
	double dHeightUpperSlab	= pBri->GetHeightUpperSlab(pBri->GetJijumNumberByStation(dSta), 0, vAngWall);
	double dHeightHunch	= dHeightUpperSlab - pBri->m_dTS;
	double dHDiff		= 0;
	if(nJ > 0 && nJ < pBri->m_nQtyJigan)
	{
		CWallApp *pWall	= pBri->GetInWall(nJ-1);
		if(pWall)
		{
			if(!pWall->IsVertWall())
			{
				dHDiff	= pBri->m_dTS/vAngWall.y - pBri->m_dTS;
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


	BOOL bSameLR	= bLeftAtLeft==bLeftAtRight;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	// 외곽선 그리기
	BOOL bIncludeFooting	= FALSE;
	BOOL bIncludeUpperSlab	= FALSE;

	BOOL bLeftW		= TRUE;
	BOOL bRightW	= TRUE;
	if(pWall->m_nType == WALL_TYPE_SLOPE || pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		bLeftW	= bLeftAtLeft;
		bRightW	= bLeftAtRight;
	}
	if(!pBri->IsBoxType())
	{
		m_pStd->m_pDrawWall->DrawLeftSide(&Dom, pWall, bIncludeFooting, bIncludeUpperSlab, bLeftW, -1, FALSE, TRUE, FALSE, TRUE);
		m_pStd->m_pDrawWall->DrawLeftSide(&Dom, pWall, bIncludeFooting, bIncludeUpperSlab, bRightW, 1, FALSE, TRUE, FALSE, TRUE);
	}

	Dom.SetCalcExtRect();
	CDRect rectTot	= Dom.GetExtRect();

	// 기초 횡단면도
	// 기초 주철근
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		// 아치교는  해당 위치에 아치 단면을 잘라서 그려준다.
		double dXDisOfUpper	= pBri->GetStationOnJijum(nJ);
		if(bLeftAtLeft)
			dXDisOfUpper	-= pWall->m_dW/2;
		else
			dXDisOfUpper	+= pWall->m_dW/2;
		dXDisOfUpper	= pBri->m_pbRib[iUPPER].GetDistByXPos(dXDisOfUpper);

		double dELWallBot = pWall->m_tvArrFront.GetTvByInfo(_T("벽체좌측면")).m_v1.y;
		double dHeightBlock = dELWallBot - pBri->m_pbRib[iUPPER].GetXyByDist(dXDisOfUpper).y;

		m_pStd->m_pDrawCrossSection->DrawCrossSection_ArchRib(&Dom, dXDisOfUpper, 0, TRUE, dHeightBlock);
	}
	else
	{
		CTwinVectorArray tvArrLeftSideFooting, tvArrLeftSideHidden;
		pWall->GetFootingByRotateToVert(bLeftW)->GetTvLeftSide(tvArrLeftSideFooting, tvArrLeftSideHidden, FALSE, 0, bLeftW, TRUE);
		CHgBaseDrawStd baseDraw;
		baseDraw.DrawTvArray(&Dom, tvArrLeftSideFooting);
	}
	*pDomP << Dom;

	// 상부슬래브 횡단면도
	if(bDrawCrossSection)
	{
		CDPoint vAngWall	= ToDPointFrDegree(pWall->GetAngleWallByHor());
		if(pBri->IsBoxType())
		{
			DrawCrossSection(&Dom, dSta, ToDPointFrDegree(pWall->m_dAnglePath), TRUE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE);
			DrawCrossSlabHunch(&Dom, dSta, ToDPointFrDegree(pWall->m_dAnglePath), FALSE, 0, FALSE, TRUE, vAngWall, FALSE, dHeightHunch);

			Dom.SetCalcExtRect();
			rectTot	= Dom.GetExtRect();
		}
		else
		{
			DrawCrossSection(&Dom, dSta, ToDPointFrDegree(pWall->m_dAnglePath), FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE);
			DrawCrossSlabHunch(&Dom, dSta, ToDPointFrDegree(pWall->m_dAnglePath), FALSE, 0, FALSE, TRUE, vAngWall, FALSE, dHeightHunch);
		}

		double dELDiff	= pWall->m_dDiffTop[bLeftW? 0 : 1];// pBri->GetDiffELSttAndJijumForVertSection(nJ, TRUE);
		Dom.Move(CDPoint(0, -dELDiff));

		*pDomP << Dom;
	}

	long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		// 배력철근
		if(bDrawSupportRebar)
		{
			// 좌측 그리기
			m_pStd->m_pDrawWall->DrawRebarSupport_Side(&Dom, pWall, bLeftAtLeft, 0, dan, bMarkRebar, dan==0, bSameLR ? 0 : -1, TRUE, bInput);
			if(!bSameLR)
				m_pStd->m_pDrawWall->DrawRebarSupport_Side(&Dom, pWall, bLeftAtRight, 0, dan, bMarkRebar, dan==0, 1, TRUE, bInput);
		}

		// 주철근
		if(bDrawMainRebar)
		{
			for(long cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
			{
				m_pStd->m_pDrawWall->DrawRebarMain_Side(&Dom, pWall, bLeftAtLeft, cycle, dan, bMarkRebar, dan==0&&cycle==0, bSameLR ? 0 : -1);
				if(!bSameLR)
				{
					m_pStd->m_pDrawWall->DrawRebarMain_Side(&Dom, pWall, bLeftAtRight, cycle, dan, bMarkRebar, dan==0&&cycle==0, bSameLR ? 0 : 1);
				}
			}
		}
	}

	// 전단철근
	if(bDrawShearRebar)
	{
		m_pStd->m_pDrawWall->DrawRebarShear_Side(&Dom, pWall, -1, bLeftAtLeft);
	}
	

	if(!bSameLR)
	{
		// 중심선 그림
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
		CTwinVector tvCenL	= pWall->GetTvCenter_Side(TRUE, 500);
		CTwinVector tvCenR	= pWall->GetTvCenter_Side(FALSE, 500);
		CTwinVector tv		= tvCenL;
		tv.m_v1.y			= min(tvCenL.GetXyBottom().y, tvCenR.GetXyBottom().y);
		tv.m_v2.y			= max(tvCenL.GetXyTop().y, tvCenR.GetXyTop().y);
		Dom.LineTo(tv.m_v1, tv.m_v2);

		// 좌우 위치 표시
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SMALL_TITLE);
		CString sTextLeft	= bLeftAtLeft ? sLeft : sRight;
		CString sTextRight	= bLeftAtRight ? sLeft : sRight;

		// 좌
		CDPoint xyText	= CDPoint(tv.m_v2.x - Dom.Always(10) - Dom.GetTextWidth(sTextLeft)*2, tv.m_v2.y);
		CObInsert *pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, "HGPOSMARK", xyText, Dom.GetScaleDim());	
		CHgBaseDrawStd::SetAttrib(&Dom, pIns, "DIR", sTextLeft);

		// 우
		xyText	= CDPoint(tv.m_v2.x + Dom.Always(10) + Dom.GetTextWidth(sTextRight)*3, tv.m_v2.y);
		pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, "HGPOSMARK", xyText, Dom.GetScaleDim());	
		CHgBaseDrawStd::SetAttrib(&Dom, pIns, "DIR", sTextRight);
	}
	else
	{
		CTwinVector tvCenL	= pWall->GetTvCenter_Side(TRUE, 500);

		// 좌우 위치 표시
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SMALL_TITLE);
		
		CDPoint xyText	= CDPoint(tvCenL.m_v2.x - Dom.GetTextWidth(sLeft)/2, tvCenL.m_v2.y);
		CObInsert *pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, "HGPOSMARK", xyText, Dom.GetScaleDim());	
		CHgBaseDrawStd::SetAttrib(&Dom, pIns, "DIR", sLeft);
	}

	// 단면 마크 "A"
	m_pStd->m_pDrawWall->DrawSectionMark_Side(&Dom, pWall);

	*pDomP << Dom;


	// 경사교는 세워서 그린다.
	// 따라서 상부슬래브 횡단면도가 길어져야 됨.
	pBri->m_dTS	-= dHDiff;
	if(nJ > 0 && nJ < pBri->m_nQtyJigan)
	{
		CWallApp *pWall	= pBri->GetInWall(nJ-1);
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

	return rectTot;
}

CDRect CDrawCrossSection::DrawRebar_Column_SideStd(CDomyun *pDomP, long nJ, BOOL bMarkRebar, BOOL bDimRebar)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	CWallApp *pWall	= pBri->GetInWall(nJ-1);

	if(!pWall) return CDRect(0, 0, 0, 0); 
	if(!pWall->m_bIs) return CDRect(0, 0, 0, 0); 
	if(!pWall->m_bTopIsColumn) return CDRect(0, 0, 0, 0);
	double dSta	= pWall->m_xyOrg.x;
	CDPoint vAngWall	= ToDPointFrDegree(pWall->GetAngleWallByHor());
	CDPoint vAng	= pBri->GetAngleJijum(nJ);

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	// 외곽선 그리기
	m_pStd->m_pDrawWall->DrawLeftSide(&Dom, pWall, FALSE, FALSE, TRUE, 0, FALSE, TRUE, FALSE, FALSE);
	if(pBri->IsBoxType())
	{
		m_pStd->m_pDrawExFooting->DrawLeftSideLowerBox(&Dom, pBri, dSta, vAng, 0, FALSE, FALSE, FALSE, TRUE, FALSE);
	}
	else
	{
		m_pStd->m_pDrawFooting->DrawLeftSide(&Dom, &pWall->m_footing, FALSE);
	}

	Dom.SetCalcExtRect();
	CDRect rectTot	= Dom.GetExtRect();
	rectTot.left	= pWall->m_footing.m_tvPathLeft.GetXyLeft().x;

	for(long col = 0; col < pWall->m_nCountColumn; col++)
	{
		CColumnApp *pColumn	= pWall->GetColumn(col);
		BOOL bLastCol	= col == pWall->m_nCountColumn-1;
		
		long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			BOOL bDim	= bLastCol && bDimRebar; 
			BOOL bMark	= col==0 && bMarkRebar;
			BOOL bApplyCuttingCircle	= col==0;
			
			m_pStd->m_pDrawColumn->DrawRebarMain_Front(&Dom, pColumn, TRUE, dan, bDim, bMark, FALSE, FALSE, bApplyCuttingCircle);
			m_pStd->m_pDrawColumn->DrawRebarRound_Front(&Dom, pColumn, nJ, TRUE, dan, bDim, bMark, FALSE, FALSE, bApplyCuttingCircle);
			
			if(bLastCol && dan == 0)
			{
				m_pStd->m_pDrawColumn->DimRebarCrossTie_Front(&Dom, pColumn, nJ, pColumn->GetCountDan_RebarMain()+1, TRUE, FALSE, TRUE);
			}
		}
	}
	
	// 기초 횡단면도
	// 기초 주철근
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		// 아치교는  해당 위치에 아치 단면을 잘라서 그려준다.
		double dXDisOfUpper	= pBri->GetStationOnJijum(nJ);
		dXDisOfUpper	= pBri->m_pbRib[iUPPER].GetDistByXPos(dXDisOfUpper);

		double dELWallBot = pWall->m_tvArrFront.GetTvByInfo(_T("벽체좌측면")).m_v1.y;
		double dHeightBlock = dELWallBot - pBri->m_pbRib[iUPPER].GetXyByDist(dXDisOfUpper).y;
		m_pStd->m_pDrawCrossSection->DrawCrossSection_ArchRib(&Dom, dXDisOfUpper, 0, TRUE, dHeightBlock);
	}
	else
	{
		m_pStd->m_pDrawFooting->DrawRebar_SideStd(&Dom, &pWall->m_footing, bDimRebar, bMarkRebar, FALSE);
	}

	*pDomP << Dom;
	// 상부슬래브 횡단면도
	
	double dELDiff	= pBri->GetDiffELSttAndJijumForVertSection(nJ, TRUE, FALSE);

	m_pStd->m_pDrawCrossSection->DrawCrossSection(&Dom, dSta, vAng, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE);
	m_pStd->m_pDrawCrossSection->DrawCrossSlabHunch(&Dom, dSta, vAng, FALSE, 0, FALSE, TRUE, vAngWall, FALSE, pWall->m_dThickHunch);
	
	m_pStd->m_pDrawCrossSection->DrawRebarShear_CrossSection_SlabGirder(&Dom, nJ, TRUE, TRUE, 0);
	m_pStd->m_pDrawCrossSection->DrawRebarSupport_CrossSection_UpperSlabGirder(&Dom, nJ, TRUE, TRUE, 0);

	Dom.Move(CDPoint(0, -dELDiff));
	*pDomP << Dom;

	if(pBri->IsBoxType())
	{
		m_pStd->m_pDrawCrossSection->DrawRebarShear_CrossSection_LowerSlabGirder(&Dom, nJ, dSta,TRUE, TRUE, 0);
		m_pStd->m_pDrawCrossSection->DrawRebarSupport_CrossSection_LowerSlabGirder(&Dom, nJ, dSta, vAng, TRUE, TRUE, 0);
		if(pBri->m_bSlopeLowerSlab == FALSE) dELDiff = 0;

		// Box타입일때 상부 거더와 하부거더는 이동 거리가 각각 다를수 있기 때문에 따로 Move시켜준다.
		Dom.Move(CDPoint(0, -dELDiff));
		*pDomP << Dom;
	}
	
//	다 그리고나서 왜또 이걸 하지? 이러니 중복되서 그려지지..
//	m_pStd->m_pDrawCrossSection->DrawRebar_CrossSectionStd(&Dom, dSta, dSta, bDimRebar, FALSE, bMarkRebar, FALSE, FALSE);

	// 단면 마크 "A"
	m_pStd->m_pDrawWall->DrawSectionMark_Side(&Dom, pWall);

	*pDomP << Dom;

	return rectTot;
}

void CDrawCrossSection::DrawRebarSuppor_CrossSection_OutWallHunch(CDomyun *pDomP, BOOL bStt, BOOL bMarkRebar, long nDrawLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;	
	if(!pBri->IsOutWallHunch(bStt)) return;

	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	

	CTwinVectorArray tvArr;
	pBri->GetTvRebarSupport_CrossSection_OutWallHunch(tvArr, bStt);


	// nDrawLeft에 따라서 걸러냄.
	if(nDrawLeft != 0)
	{
		CWallApp wall;
		pBri->MakeWallByOutWallOnlyShape(wall, bStt, TRUE);

		BOOL bLeft	= bStt ? FALSE : TRUE;
		CTwinVector tvCenter	= wall.GetTvCenter_Side(bLeft, 100);
		CRebarPlacing rb;
		double dDiff	= nDrawLeft == -1 ? -1 : 1;
		tvCenter.m_v1.x += dDiff;
		tvCenter.m_v2.x += dDiff;
		rb.TrimTvArrayByLine(tvArr, tvCenter.m_v1, tvCenter.m_v2, nDrawLeft==1, TRUE, FALSE);
	}

	CRebarPlacing rb;
	CTwinVectorArray tvArrCutting;
	pBri->GetTvCuttingArea_Side_OutWallHunch(tvArrCutting, bStt, nDrawLeft);
	rb.GetTvArrayByCrossTvArrayArea(tvArr, tvArrCutting, TRUE);

	baseDraw.DrawTvArray(&Dom, tvArr);
	if(bMarkRebar)
	{

	}


	*pDomP << Dom;
}

void CDrawCrossSection::DrawRebarMain_CrossSection_OutWallHunch(CDomyun *pDomP, long nCycle, BOOL bStt, BOOL bMarkRebar, long nDrawLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;	
	if(!pBri->IsOutWallHunch(bStt)) return;

	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_HIDDEN2_LINE);

	CTwinVectorArray tvArr;
	pBri->GetTvRebarMain_Side_OutWallHunch(tvArr, nCycle, bStt);


	// nDrawLeft에 따라서 걸러냄.
	if(nDrawLeft != 0)
	{
		CWallApp wall;
		pBri->MakeWallByOutWallOnlyShape(wall, bStt, TRUE);

		BOOL bLeft	= bStt ? FALSE : TRUE;
		CTwinVector tvCenter	= wall.GetTvCenter_Side(bLeft, 100);
		CRebarPlacing rb;
		double dDiff	= nDrawLeft == -1 ? -1 : 1;
		tvCenter.m_v1.x += dDiff;
		tvCenter.m_v2.x += dDiff;
		rb.TrimTvArrayByLine(tvArr, tvCenter.m_v1, tvCenter.m_v2, nDrawLeft==1, TRUE, FALSE);
	}

	CRebarPlacing rb;
	CTwinVectorArray tvArrCutting;
	pBri->GetTvCuttingArea_Side_OutWallHunch(tvArrCutting, bStt, nDrawLeft);
	rb.GetTvArrayByCrossTvArrayArea(tvArr, tvArrCutting, TRUE);

	baseDraw.DrawTvArray(&Dom, tvArr);

	long stt	= bStt ? iSTT : iEND;
	CRebarInfoCycle *pRC	= &pBri->m_rebarInfoCycleOutWallHunch[nCycle][stt];
	CString sMark	= pBri->GetMarkRebarMain(0, FALSE, nCycle, FALSE, FALSE, pRC->m_sDescription, 0);
	if(bMarkRebar && tvArr.GetSize() > 0)
	{
		CTwinVectorArray tvArrSup;
		pBri->GetTvRebarSupport_CrossSection_OutWallHunch(tvArrSup, bStt);
		double dCountCycle	= pBri->GetCountCycleMainRebar();
		CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;
		CTwinVectorArray tvArrSide, tvArrHidden;
		pFooting->GetTvLeftSide(tvArrSide, tvArrHidden, FALSE, 0, TRUE);
		CTwinVector tvLower	= tvArrSide.GetTvByInfo("기초하면");
		
		CRebar *pRb = pBri->GetRebarMainByDescription(pRC->m_sDescription);

		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
		mark.SetRebar(tvArr, tvArrSup);
		// 설정
		mark.m_sMark					= sMark;
		mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pBri->GetValueFy(ePartUpperSlab));
		mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_POS;
		mark.m_nTypeMarkDir				= TYPE_MARKDIR_RIGHTTAN;
		mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
		mark.m_dPosScaleMarkStt			= 1./dCountCycle * (nCycle+1);
		mark.m_dPosScale				= 1./dCountCycle * (nCycle+1);
		mark.m_dPosScaleSpace			= 0.25;//0.5 - (1./(dCountCycle-1))/2;
		mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_LOWER;
		
		// 마크선 연장
		CDPoint xyMark					= mark.GetXyMark();
		CDPoint xyMatch(0, 0);
		GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), xyMark, tvArr.GetAt(0).GetXyDir(), xyMatch);
		mark.m_dAddHeight				= ~(xyMark - xyMatch) + Dom.Always(MARK_BASE_HEIGHT) * 2;
		mark.m_dCtc						= pBri->GetCtcRebarMain(FALSE);
		mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;
		mark.MarkRebar();
	}


	*pDomP << Dom;
}
// 종거더 그리기 통합
// CDRect CDrawCrossSection::DrawRebar_CrossSection_GirderJongStd(CDomyun *pDomP, BOOL bDimRebar, BOOL bMarkRebar, BOOL bStt, BOOL bLeft, BOOL bGagak, BOOL bInput)
// {
// 	CDRect rect(0, 0, 0, 0);
// 	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;	
// 	if(!pBri->IsGirderRebarJong(bStt, bLeft, bGagak)) return rect;
// 
// 	
// 	CDomyun Dom(pDomP);
// 	double dSta	= pBri->GetStationForInputJongGirder(bStt, bLeft, bGagak);
// 	CDPoint vAng	= CDPoint(0, 1);	// 무조건 직거리로 그림
// 
// 	// 횡단 그림
// 	DrawCrossSection(&Dom, dSta, vAng, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, bGagak, TRUE, 0);
// 	
// 
// 
// 	*pDomP << Dom;
// 
// 
// 	return rect;
// }

// 횡단 도로 중심 마크
void CDrawCrossSection::DimMarkCenterOfRoadCrossSection(CDomyun *pDomP, long nJ, BOOL bCenterOfRoad, BOOL bApplyAngle, double dHeight)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;	
	double dSta	= pBri->GetStationOnJijum(nJ, 0, FALSE);

	CDomyun Dom(pDomP);

	// 벽체 중심 마크
	CLineInfo *pLine	= pBri->GetLineBase();
	if(pLine)
	{
		double dCenOfRoad	= pBri->GetMidCrossSection(!bApplyAngle);
		
		double dElCenter = pBri->GetXySlabUpperActByDist(dSta, 0).y;
		double dELBottom = dElCenter - dHeight;
		double dTP = pBri->GetThickPave(FALSE);
		CDPoint xyCenter = CDPoint(nJ == 0 ? -dCenOfRoad : dCenOfRoad, dElCenter+dTP+Dom.Always(15));
		m_pStd->MarkCenterOfRoad(&Dom, xyCenter, (dElCenter-dELBottom)+dTP+Dom.Always(25), _T(""), FALSE);
	}

	*pDomP << Dom;
}

// 종거더 외곽선 표현
void CDrawCrossSection::DrawVirtualRebarJongGirder(CDomyun *pDomP, double dSta, BOOL bLeft, BOOL bMirror)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;	
	if(!pBri->IsGirderRebarJong(TRUE, bLeft, FALSE)) return;
	
	
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);

	CTwinVectorArray tvArrGirder;
	pBri->GetTvRebarStirrup_SlabGrider(tvArrGirder, 0, TRUE, bLeft, FALSE, TRUE);
	

	CDRect rect	= tvArrGirder.GetRect();
	Dom.Rectangle(rect);

	if(bMirror)
	{
		double dMid	= pBri->GetDistMidOfSlab(dSta, TRUE);
		Dom.Mirror(dMid, TRUE);
	}
		

	*pDomP << Dom;
}

// 벽체 예각부
CDRect CDrawCrossSection::DrawRebarCrossSection_AcuteStd(CDomyun *pDomP, BOOL bStt, BOOL bLeft, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput)
{
	long stt	= bStt ? iSTT : iEND;
	long left	= bLeft ? iLEFT : iRIGHT;

	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;	
	if(!pBri->m_bIsAcuteAngle_OutWall[stt][left]) return CDRect(0, 0, 0, 0);

	
	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);

	CWallApp wall;
	pBri->MakeWallByOutWall(wall, bStt, TRUE, FALSE, TRUE, bLeft);

	CTwinVectorArray tvArrLeft;
	wall.GetTvLeftSide(tvArrLeft, FALSE, TRUE, TRUE, !bStt, 0, FALSE, FALSE, FALSE);
	CDRect rect	= tvArrLeft.GetRect();
	GetSwap(rect.top, rect.bottom);

	CRebarPlacing rb;
	CDPoint xyOrg(wall.m_dCenterOfWall, 0);

	// 주철근 그리기
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	CTwinVectorArray tvArrRebarMain;
	pBri->GetTvRebarMain_Side_OutWall_Acute(tvArrRebarMain, bStt, bLeft);
	if(bStt)
	{
		rb.Mirror(tvArrRebarMain, xyOrg, TRUE);
	}
	baseDraw.DrawTvArray(&Dom, tvArrRebarMain);

	if(bDimRebar)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		CDPointArray xyArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		CString sDir	= _T("BOTTOM");
		CDPoint xyStt	= CDPoint(0, 0);
		long nDimDan	= bInput ? 1 : 0;

		CTwinVector tvLeft	= tvArrLeft.GetTvByInfo("벽체좌측");
		CTwinVector tvRight	= tvArrLeft.GetTvByInfo("벽체우측");

		if(bLeft)
			xyArrDim.Add(tvLeft.GetXyBottom());
		long i = 0; for(i = 0; i < tvArrRebarMain.GetSize(); i++)
			xyArrDim.Add(tvArrRebarMain.GetAt(i).m_v1);
		if(!bLeft)
			xyArrDim.Add(tvRight.GetXyBottom());

		rb.RemoveSamePoint(xyArrDim);
		xyArrDim.Sort(TRUE);

		xyStt	= xyArrDim.GetAt(0);
		xyStt.y	= tvLeft.GetXyBottom().y;

		baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, TRUE, FALSE, TRUE, FALSE, FALSE, FALSE);

		
		
		// 입력시 배력근 범위에 대해서 치수기입
		if(bInput)
		{
			long stt	= bStt ? iSTT : iEND;
			long left	= bLeft ? iLEFT : iRIGHT;
			double dRange	= pBri->m_dRange_AcuteAngle_OutWall[stt][left];
			CTwinVector tvOrg	= bLeft ? tvLeft : tvRight;

			xyArrDim.RemoveAll();
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();
			nDimDan = 0;
			xyArrDim.Add(tvOrg.GetXyBottom());
			xyArrDim.Add(tvOrg.GetXyBottom() + (bLeft ? CDPoint(1, 0) : CDPoint(-1, 0)) * dRange);
			xyArrDim.Sort(TRUE);
			sArrText.Add("WMR");sArrText.Add("WMR");
			sArrUnderText.Add(COMMA(dRange));sArrUnderText.Add(COMMA(dRange));
			xyStt	= xyArrDim.GetAt(0);

			baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
		}
	}


	// 배력근 그리기
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	CTwinVectorArray tvArrRebarSupport, tvArrRebarSupportAngle;
	pBri->GetTvRebarSupport_CrossSection_OutWall_Acute(tvArrRebarSupport, tvArrRebarSupportAngle, bStt, bLeft);
	if(bStt)
	{
		rb.Mirror(tvArrRebarSupport, xyOrg, TRUE);
		rb.Mirror(tvArrRebarSupportAngle, xyOrg, TRUE);
	}
	baseDraw.DrawTvArray(&Dom, tvArrRebarSupport);

	if(bDimRebar)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		CDPointArray xyArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		CString sDir	= bLeft ? _T("LEFT") : _T("RIGHT");
		CDPoint xyStt	= CDPoint(0, 0);
		long nDimDan	= 0;

		CTwinVector tvLeft	= tvArrLeft.GetTvByInfo("벽체좌측");
		CTwinVector tvRight	= tvArrLeft.GetTvByInfo("벽체우측");		
		CTwinVector tvOrg	= bLeft ? tvLeft : tvRight;
		CDPoint xyMatch(0, 0);
		CTwinVector tv;
		xyArrDim.Add(tvOrg.GetXyBottom());
		long i = 0; for(i = 0; i < tvArrRebarSupport.GetSize(); i++)
		{
			tv	= tvArrRebarSupport.GetAt(i);
			GetXyMatchLineAndLine(tv.m_v1, tv.GetXyDir(), tvOrg.m_v1, tvOrg.GetXyDir(), xyMatch);
			xyArrDim.Add(xyMatch);
		}
		//xyArrDim.Add(tvOrg.GetXyTop());
		xyArrDim.Sort(FALSE);

		xyStt	= tvOrg.GetXyBottom();
		baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, TRUE, FALSE, TRUE, FALSE, FALSE, FALSE);

	}

	if(bMarkRebar)
	{
		// 철근이 좌에서 우로, 하에서 상으로 가도록 sort
		rb.ReverseRebar(tvArrRebarMain);
		if(!bStt)
		{
			rb.ReverseRebar(tvArrRebarSupport);
			rb.ReverseRebar(tvArrRebarSupportAngle);
		}
		CTwinVector tv;
		long i = 0; for(i = 0; i < tvArrRebarMain.GetSize(); i++)
		{
			tv	= tvArrRebarMain.GetAt(i).Sort(FALSE);
			tvArrRebarMain.SetAt(i, tv);
		}
		for(i = 0; i < tvArrRebarSupport.GetSize(); i++)
		{
			tv	= tvArrRebarSupport.GetAt(i).Sort(TRUE);
			tvArrRebarSupport.SetAt(i, tv);
		}
		for(i = 0; i < tvArrRebarSupportAngle.GetSize(); i++)
		{
			tv	= tvArrRebarSupportAngle.GetAt(i).Sort(TRUE);
			tvArrRebarSupportAngle.SetAt(i, tv);
		}

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		
		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
		long stt	= bStt ? iSTT : iEND;
		long left	= bLeft ? iLEFT : iRIGHT;

		CRebarInfoCycle *pRC	= &pBri->m_rebarInfoCycle_AcuteAngle_OutWall[stt][left];
		CRebar *pRb = pBri->GetRebarMainByDescription(pRC->m_sDescription);

		mark.SetRebar(tvArrRebarMain, tvArrRebarSupport.GetSize() > 0 ? tvArrRebarSupport : tvArrRebarSupportAngle);
		mark.m_sMark	= pRC->m_szMark;
		mark.m_sDia		= m_pStd->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pBri->GetValueFy(bStt? ePartOutWallStt : ePartOutWallEnd));
		mark.m_dCtc						= pBri->GetCTCRebarSupport_OutWall(bStt, bLeft, 0);
		mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;
		mark.MarkRebar();

		CTwinVectorArray tvArrSup;
		for(long sup = 0; sup < 2; sup++)
		{
			tvArrSup = sup == 0 ? tvArrRebarSupport : tvArrRebarSupportAngle;
			
			CMarkDomyun mark1(&Dom, m_pStd->m_pARoadOptionStd);
			CString sStt	= bStt ? "시점" : "종점";
			CString sLeft	= bLeft ? "좌측" : "우측";
			CString sDecription	= _T("");
// 			CString sMark		= _T("");
			sDecription.Format("%s벽체 %s예각부 배력철근%d", sStt, sLeft, sup+1);
			
			CString sMark	=	pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDecription);
			pRb = pBri->GetRebarSupportByDescription(sDecription);
			CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_AcuteAngle_SupportRebar_OutWall[stt][left];
			mark1.SetRebar(tvArrSup, tvArrRebarMain);
			mark1.m_sMark		= sMark;
			mark1.m_sDia		= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(bStt? ePartOutWallStt : ePartOutWallEnd));
			mark1.m_dCtc						= pPlace->GetCTC();
			mark1.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;
			mark1.MarkRebar();
		}
	}

	// 일반도 그리기
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	CDRect rectMain	= tvArrRebarMain.GetRect();
	CDRect rectSup1	= tvArrRebarSupport.GetRect();
	CDRect rectSup2	= tvArrRebarSupportAngle.GetRect();
	CDRect rectTot	= rectMain;
	if(tvArrRebarSupport.GetSize() > 0)
		rectTot	= m_pStd->MergeRectAndRect(rectTot, rectSup1);
	if(tvArrRebarSupportAngle.GetSize() > 0)
		rectTot	= m_pStd->MergeRectAndRect(rectTot, rectSup2);

	CDRect rectCut	= tvArrLeft.GetRect();
	rectCut.top = rectTot.top + Dom.Always(5);
	if(bLeft)
	{
		rectCut.left -= Dom.Always(5);
		rectCut.right = rectTot.right + Dom.Always(5);
		rect.right	= rectCut.right;
	}
	else
	{
		rectCut.left = rectTot.left - Dom.Always(5);
		rectCut.right += Dom.Always(5);
		rect.left	= rectCut.left;
	}
	rect.bottom	= rectCut.top;
	rectCut.bottom -= Dom.Always(5);
	
	m_pStd->DrawCuttingTvArray(&Dom, tvArrLeft, rectCut, TRUE);


	*pDomP << Dom;

	return rect;
}

// 아치리브 횡단 구조도 통합
CDRect CDrawCrossSection::DrawRebarCrossSection_ArchRib_Std(CDomyun *pDomP, double dXDisOfLeft, double dXDisOfRight, BOOL bDimRebar, BOOL bMarkRebar)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;	
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH) return CDRect(0, 0, 0, 0);

	CDomyun Dom(pDomP);

	BOOL bSameLeftRight	= Compare(dXDisOfLeft, dXDisOfRight, "=");
	// 횡단일반도
	DrawCrossSection_ArchRib(&Dom, dXDisOfLeft, bSameLeftRight ? 0 : -1);
	if(!bSameLeftRight)
		DrawCrossSection_ArchRib(&Dom, dXDisOfRight, 1);

	// 중심선
	Dom.SetCalcExtRect();
	CDRect rectGen	= Dom.GetExtRect();

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
	double dExt	= Dom.GetTextHeight()*3;
	CTwinVector tv;
	tv.m_v1.x			= rectGen.left+rectGen.Width()/2;
	tv.m_v2.x			= tv.m_v1.x;
	tv.m_v1.y			= rectGen.top - dExt;
	tv.m_v2.y			= rectGen.bottom + dExt;
	Dom.LineTo(tv.m_v1, tv.m_v2);
	
	if(bSameLeftRight)
	{
		// 구조물 중심 표시
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_STRS);
		CDPoint xyOrg	= tv.m_v2;
		xyOrg.y -= Dom.GetTextHeight();
		Dom.TextOut(xyOrg.x, xyOrg.y, "CL of Arch Rib");
	}
	else
	{
		// 좌우 위치 표시
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SYMB);
		CString sTextRight	= "SECTION B-B";
		CString sTextLeft	= "아치 크라운부";
		
		CDPoint xyOrg	= tv.m_v2;
		xyOrg.y	 -= Dom.GetTextHeight();

		CConcSymbol sym(m_pStd->m_pARoadOptionStd);
		sym.ArrowSide(&Dom, sTextLeft, sTextRight, xyOrg);
	}

	// 90도 회전
	Dom.Rotate(CDPoint(0, 0), ToDPointFrDegree(90));
	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();
	*pDomP << Dom;

	// 주철근/배력근
	for(long cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
	{
		for(long upper = 0; upper < 2; upper++)
		{
			BOOL bUpper	= upper == iUPPER;

			long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				DrawRebarMain_CrossSection_ArchRib(&Dom, dXDisOfLeft, cycle, bUpper, dan, cycle==0, TRUE, bSameLeftRight ? 0 : -1);
				if(cycle == 0)
					DrawRebarSupport_CrossSection_ArchRib(&Dom, dXDisOfLeft, bUpper, dan, dan==0, TRUE, bSameLeftRight ? 0 : -1);

				if(!bSameLeftRight)
				{
					DrawRebarMain_CrossSection_ArchRib(&Dom, dXDisOfRight, cycle, bUpper, dan, FALSE, TRUE, 1);
					if(cycle == 0)
						DrawRebarSupport_CrossSection_ArchRib(&Dom, dXDisOfRight, bUpper, dan, dan==0, TRUE, 1);
				}
			}
		}
	}
	
	// 전단철근
	DrawRebarShear_CrossSection_ArchRib(&Dom, dXDisOfLeft, FALSE, TRUE, bSameLeftRight ? 0 : -1);
	if(!bSameLeftRight)
		DrawRebarShear_CrossSection_ArchRib(&Dom, dXDisOfRight, FALSE, FALSE, 1);

	// 90도 회전
	Dom.Rotate(CDPoint(0, 0), ToDPointFrDegree(90));
	*pDomP << Dom;

	return rect;
}

// 아치리브 횡단 일반도
// BOOL bVerticalSection : 수직으로 자른 단면
void CDrawCrossSection::DrawCrossSection_ArchRib(CDomyun *pDomP, double dXDisOfUpper, long nDrawLeft, BOOL bVerticalSection, double dHeightBlock)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;	
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	CHgBaseDrawStd baseDraw;

	CTwinVectorArray tvArr;
	pBri->GetTvCrossSection_ArchRib(tvArr, dXDisOfUpper, nDrawLeft, bVerticalSection, dHeightBlock);
	baseDraw.DrawTvArray(&Dom, tvArr);

	*pDomP << Dom;
}

// 아치리브 횡단 주철근
void CDrawCrossSection::DrawRebarMain_CrossSection_ArchRib(CDomyun *pDomP, double dXDisOfUpper, long nCycle, BOOL bUpper, long nDan, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;	
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH) return;

	CDomyun Dom(pDomP);
	CHgBaseDrawStd baseDraw;

	long upper	= bUpper ? iUPPER : iLOWER;

	CDPointArray xyArr;
	CRebarInfoCycle *pRC	= pBri->GetXyRebarMain_CrossSection_ArchRib(xyArr, dXDisOfUpper, nCycle, bUpper, nDan, nDrawLeft);
	if(!pRC) return;

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);
	baseDraw.DrawPointRebar(&Dom, xyArr, bUpper ? CDPoint(0, 1) : CDPoint(0, -1));

	CString sDescription	= pRC->m_sDescription;
	CString sMark			= pBri->GetMarkRebarMain(0, FALSE, nCycle, FALSE, FALSE, sDescription, nDan);
	if(bMarkRebar)
	{
		CRebar *pRb = pBri->GetRebarMainByDescription(sDescription);

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		CMarkDomyun mark;
		mark.SetDomyun(&Dom, m_pStd->m_pARoadOptionStd);
		mark.SetRebar(xyArr);
		mark.m_sMark	= sMark;
		mark.m_sDia		= m_pStd->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pBri->GetValueFy(ePartUpperSlab));
		mark.m_dPosScaleMarkStt	= 0.5;
		if(pBri->GetCountCycleMainRebar() == 2)
		{
			switch(nCycle)
			{
			case 0: mark.m_dPosScaleMarkStt	= 0.3; break;
			case 1: mark.m_dPosScaleMarkStt	= 0.6; break;
			}
			mark.m_dPosScaleMarkStt += 0.1 * nDan;
		}
		else
		{
			switch(nCycle)
			{
			case 0: mark.m_dPosScaleMarkStt	= 0.2; break;
			case 1: mark.m_dPosScaleMarkStt	= 0.4; break;
			case 2: mark.m_dPosScaleMarkStt	= 0.6; break;
			case 3: mark.m_dPosScaleMarkStt	= 0.8; break;
			}
			mark.m_dPosScaleMarkStt += 0.05 * nDan;
		}
		mark.m_nTypeMarkPointUpAndDown	= bUpper ? TYPE_MARKPOINT_UPPER : TYPE_MARKPOINT_LOWER;
		mark.m_nTypeMarkDir				= TYPE_MARKDIR_RIGHT;
		mark.m_dAddHeight				= pBri->m_dCoverRib[upper][nDan] - REBAR_RADIUS;
		mark.m_dCtc						= pBri->GetCtcRebarMain(FALSE);
		mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;
		mark.MarkRebar();
	}


	if(bDimRebar && nCycle == 0)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		pBri->GetXyRebarMain_CrossSection_ArchRib(xyArr, dXDisOfUpper, -1, bUpper, nDan, 0);

		CTwinVectorArray tvArrCross;
		pBri->GetTvCrossSection_ArchRib(tvArrCross, dXDisOfUpper, 0);
		CDRect rect	= tvArrCross.GetRect();
		xyArr.InsertAt(0, CDPoint(rect.left, bUpper ? rect.top : rect.bottom));
		xyArr.Add(CDPoint(rect.right, bUpper ? rect.top : rect.bottom));
		xyArr.Sort(TRUE);

		CStringArray sArrText;
		CStringArray sArrUnderText;
		CDPoint xyStt	= xyArr.GetAt(0);
		CString sDir	= bUpper ? "TOP" : "BOTTOM";
		long nDimDan	= pBri->GetCountDanMainRebar_ArchRib(dXDisOfUpper, bUpper) - nDan - 1;

		baseDraw.AutoDimCont(&Dom, xyStt, xyArr, sArrText, sArrUnderText, nDimDan, sDir, -1, nDan==0, TRUE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE);
	}

	*pDomP << Dom;
}

void CDrawCrossSection::DrawRebarSupport_CrossSection_ArchRib(CDomyun *pDomP, double dXDisOfUpper, BOOL bUpper, long nDan, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;	
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	CHgBaseDrawStd baseDraw;

	CTwinVector tvRebar	= pBri->GetTvOffset_ArchRib(dXDisOfUpper, bUpper, nDan, nDrawLeft);
	if(tvRebar.GetLength() == 0) return;
	baseDraw.DrawTv(&Dom, tvRebar);

	// 상면철근 그릴때는 좌우측도 그려준다.
	if(bUpper)
	{
		double dCoverU	= pBri->m_dCoverRib[iUPPER][nDan];
		double dCoverL	= pBri->m_dCoverRib[iLOWER][nDan];
		double dThick	= pBri->GetThickArchRib(dXDisOfUpper);

		if(nDrawLeft == 1 || nDrawLeft == 0)
			Dom.LineTo(tvRebar.m_v1, tvRebar.m_v1+(CDPoint(0, -(dThick-dCoverU-dCoverL))));
		if(nDrawLeft == -1 || nDrawLeft == 0)
			Dom.LineTo(tvRebar.m_v2, tvRebar.m_v2+(CDPoint(0, -(dThick-dCoverU-dCoverL))));
	}

	if(bDimRebar && nDan == 0)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		CTwinVectorArray tvArrCross;
		pBri->GetTvCrossSection_ArchRib(tvArrCross, dXDisOfUpper, 0);
		CDRect	rect	= tvArrCross.GetRect();

		double dCoverU[3]={0, 0, 0};
		double dCoverL[3]={0, 0, 0};
		
		// 하면 피복들
		if(pBri->GetRebarInfoCycleArchRibByDist(-1, 0, FALSE, dXDisOfUpper))
			dCoverL[0]	= pBri->m_dCoverRib[iLOWER][0];
		if(pBri->GetRebarInfoCycleArchRibByDist(-1, 1, FALSE, dXDisOfUpper))
			dCoverL[1]	= pBri->m_dCoverRib[iLOWER][1]-pBri->m_dCoverRib[iLOWER][0];
		if(pBri->GetRebarInfoCycleArchRibByDist(-1, 2, FALSE, dXDisOfUpper))
			dCoverL[2]	= pBri->m_dCoverRib[iLOWER][2]-pBri->m_dCoverRib[iLOWER][1];

		// 상면 피복들
		if(pBri->GetRebarInfoCycleArchRibByDist(-1, 0, TRUE, dXDisOfUpper))
			dCoverU[0]	= pBri->m_dCoverRib[iUPPER][0];
		if(pBri->GetRebarInfoCycleArchRibByDist(-1, 1, TRUE, dXDisOfUpper))
			dCoverU[1]	= pBri->m_dCoverRib[iUPPER][1]-pBri->m_dCoverRib[iUPPER][0];
			if(pBri->GetRebarInfoCycleArchRibByDist(-1, 2, TRUE, dXDisOfUpper))
			dCoverU[2]	= pBri->m_dCoverRib[iUPPER][2]-pBri->m_dCoverRib[iUPPER][1];

		double dThick	= pBri->GetThickArchRib(dXDisOfUpper) - (dCoverL[0]+dCoverL[1]+dCoverL[2]) - (dCoverU[0]+dCoverU[1]+dCoverU[2]);
		
		// 횡단우측
		if(nDrawLeft == -1 || nDrawLeft == 0)
		{
			*pDomP << Dom;
			CDoubleArray dArrDim;
			CStringArray sArrText;
			CStringArray sArrUnderText;
			CString sDir	= "TOP";
			CDPoint xyStt	= CDPoint(rect.right, rect.top);

			dArrDim.Add(dCoverU[0]);
			dArrDim.Add(dCoverU[1]);
			dArrDim.Add(dCoverU[2]);
			dArrDim.Add(dThick);
			dArrDim.Add(dCoverL[2]);
			dArrDim.Add(dCoverL[1]);
			dArrDim.Add(dCoverL[0]);

			baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, 0, sDir, -1, TRUE, FALSE, FALSE);

			Dom.Rotate(xyStt, ToDPointFrDegree(-90));
			*pDomP << Dom;
		}

				
		// 횡단좌측
		if(nDrawLeft == 1 || nDrawLeft == 0)
		{
			*pDomP << Dom;

			CDoubleArray dArrDim;
			CStringArray sArrText;
			CStringArray sArrUnderText;
			CString sDir	= "BOTTOM";
			CDPoint xyStt	= CDPoint(rect.left, rect.top);

			dArrDim.Add(dCoverU[0]);
			dArrDim.Add(dCoverU[1]);
			dArrDim.Add(dCoverU[2]);
			dArrDim.Add(dThick);
			dArrDim.Add(dCoverL[2]);
			dArrDim.Add(dCoverL[1]);
			dArrDim.Add(dCoverL[0]);

			baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, 0, sDir, -1, TRUE, FALSE, FALSE);

			Dom.Rotate(xyStt, ToDPointFrDegree(-90));
			*pDomP << Dom;
		}
	}


	if(bMarkRebar)
	{}
	
	*pDomP << Dom;
}

void CDrawCrossSection::DrawRebarShear_CrossSection_ArchRib(CDomyun *pDomP, double dXDisOfUpper, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);
	
	// 철근 가져와서 그리기
	CHgBaseDrawStd baseDraw;
	CTwinVectorArray tvArr;
	CTwinVector tv;

	pBri->GetTvRebarShear_CrossSection_ArchRib(tvArr, dXDisOfUpper, nDrawLeft);
	long nSize	= tvArr.GetSize();
	long i = 0; for(i = 0; i < nSize; i++)
	{
		if(i % 2 == 0)
			m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_SOLID);
		else
			m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_HIDDEN2_LINE);
		
		Dom.LineTo(tvArr.GetAt(i).m_v1, tvArr.GetAt(i).m_v2);
	}


	if(bDimRebar)
	{
	
	}


	CString sDescription	= _T("아치리브 전단철근");
	CString sMark			= pBri->GetMarkRebarShearArchRib(0, FALSE, FALSE, FALSE, sDescription);
	CRebarShearDetail *pDetail	= &pBri->m_RebarShearDetail_ArchRib;
	if(bMarkRebar)
	{
		CTwinVectorArray tvArrSup;
		tvArrSup.Add(pBri->GetTvOffset_ArchRib(dXDisOfUpper, FALSE, 0, 0));
		tvArrSup.Add(pBri->GetTvOffset_ArchRib(dXDisOfUpper, TRUE, 0, 0));

		pBri->GetTvRebarShear_CrossSection_ArchRib(tvArr, dXDisOfUpper, 0);

		CMarkDomyun mark;
		mark.SetDomyun(&Dom, m_pStd->m_pARoadOptionStd);
		mark.SetRebar(tvArr, tvArrSup);
		mark.m_sMark	= sMark;
		mark.m_sDia		= m_pStd->m_pARcBridgeDataStd->GetStringDia(pDetail->m_dDia, pBri->GetValueFy(ePartUpperSlab, TRUE));
		mark.m_dPosScaleSpace	= 0.25;
		mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_POS;
		mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
		mark.m_dPosScaleMarkStt			= 0.05;
		mark.m_nTypeMarkDir				= TYPE_MARKDIR_LEFTTAN;
		mark.m_dAddHeight				= pBri->GetThickArchRib(dXDisOfUpper)/2;
		mark.m_dCtc						= pBri->GetCTCRebarShear_ArchRib();
		mark.m_nTypeRebar				= pDetail->GetRebarType();
		mark.MarkRebar();
	}

	*pDomP << Dom;
}

// nType == 0 : 기초 근처
// nType == 1 : 2단주철근이 있는 위치
// nType == 2 : 크라운부
double CDrawCrossSection::GetDistForCrossSection_ArchRib(long nType, BOOL &bOK)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return -5000;
	
	double dXDisOfLeft	= -5000;
	if(nType == 0)
	{
		CTwinVector tvUpper	= pBri->m_footingStt.GetTvOffset_Front(0, TRUE, TRUE);
		dXDisOfLeft	= pBri->m_pbRib[iUPPER].GetDistByXPos(pBri->m_pbRib[iUPPER].GetXyMatchXyAndAngle(tvUpper.m_v1, CDPoint(1, 0)).x);

		bOK	= TRUE;
	}
	else if(nType == 1)
	{
		dXDisOfLeft	= -5000;
		bOK	= FALSE;
		// 2단 주철근이 있는 위치
		long i = 0; for(i = 0; i < pBri->m_pArrRCMainRib[0][iUPPER][1].GetSize(); i++)
		{
			CRebarInfoCycle *pRC	= pBri->GetRebarInfoCycleArchRib(0, 1, TRUE, i);
			if(!pRC) continue;
			if(!pRC->m_bExist) continue;

			if(i == 0)
				dXDisOfLeft	= pBri->m_pbRib[iUPPER].GetDistByXPos((pRC->m_xyMid2.x+pRC->m_xyEnd.x)/2);
			else
				dXDisOfLeft	= pBri->m_pbRib[iUPPER].GetDistByXPos((pRC->m_xyStt.x+pRC->m_xyMid1.x)/2);
			bOK	= TRUE;
		}
	}
	else if(nType == 2)
	{
		dXDisOfLeft	= 0;

		bOK	= TRUE;
	}

	return dXDisOfLeft;
}

// 아치리브 횡단면
void CDrawCrossSection::DrawCrossSection_ArchRibStd(CDomyun *pDomP, BOOL bUpper, long nDrawLeft, BOOL bDrawOnlyArchRib, BOOL bHidden, BOOL bDrawPattern)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDPoint vAng	= CDPoint(0, 1);
	
	CDomyun Dom(pDomP);

	// 슬래브 횡단면도 
	CTwinVectorArray tvArrCrossSection;
	long nJ	= pBri->GetJijumConnectRibAndSlab();

	if(nJ > -1)
	{
		double dSta		= pBri->GetStationOnJijum(nJ);
		pBri->GetTvCrossSection(tvArrCrossSection, dSta, vAng, FALSE, TRUE, FALSE, 0, FALSE, FALSE, FALSE, TRUE);
		if(!bDrawOnlyArchRib)
		{
			DrawCrossAllAtJijum(&Dom, nJ, FALSE, TRUE, FALSE, FALSE);
		}
	}

	// 아치리브 횡단면도 
	CTwinVectorArray tvArrArchRib, tvArrArchRibPattern;
	pBri->GetTvCrossSection_ArchRib(tvArrArchRib, tvArrArchRibPattern, bUpper, nDrawLeft);


	CHgBaseDrawStd baseDraw;
	if(bHidden)
	{
		long nIdx	= tvArrArchRib.GetIdxByInfo("하면");
		if(nIdx != -1)
		{
			CTwinVector tvLower	= tvArrArchRib.GetAt(nIdx);
			tvArrArchRib.RemoveAt(nIdx);

			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
			m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_HIDDEN_LINE);
			baseDraw.DrawTv(&Dom, tvLower);
		}
	}
		

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	CRebarPlacing rb;
	rb.Mirror(tvArrCrossSection, CDPoint(0, 0), TRUE);
	baseDraw.DrawTvArrayApplyOverlapShape(&Dom, tvArrArchRib, tvArrCrossSection);

	// 패턴 그리기
	if(bDrawPattern)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
		baseDraw.DrawTvArrayApplyOverlapShape(&Dom, tvArrArchRibPattern, tvArrCrossSection);
	}

	*pDomP << Dom;
}

void CDrawCrossSection::DrawCrossSection_Section_ArchRib(CDomyun *pDomP, BOOL bStt)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	CHgBaseDrawStd baseDraw;

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	CTwinVectorArray tvArr;
	pBri->GetTvSection_Front_ArchRib(tvArr, bStt);
	baseDraw.DrawTvArray(&Dom, tvArr, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());

	// 텍스트
	Dom.SetTextAlignHorz(TA_CENTER);
	CTwinVector tv	= tvArr.GetTvByInfo("아치리브횡단상면");
	CTwinVector tvLeft	= tvArr.GetTvByInfo("아치리브횡단좌측면");
	CDPoint xyCen	= tv.GetXyMid() - CDPoint(0, tvLeft.GetLength()/2+Dom.GetTextHeight()/2);
	Dom.TextOut(xyCen, "SPRING GING");
	*pDomP << Dom;	
}

// 횡단면 구조도 그림
// 단 확폭의 경우 좌우 반반씩 그릴 수가 없으므로 
// dSta에 따라 좌우를 같은 dSta를 다 그림
// 단, dSta가 시점이하이거나 종점이상인 경우 슬래브 끝단을 그림.
// 철근은 시작철근 끝철근 사용함.
CDRect CDrawCrossSection::DrawRebar_CrossSectionStd(CDomyun *pDomP, double dSta, BOOL bDimRebarU, BOOL bDimRebarL, BOOL bMarkRebar, BOOL bRotate, BOOL bMirror, BOOL bExcludeDoupleMainRebar)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return CDRect(0, 0, 0, 0);

	CDomyun Dom(pDomP);
	CDomyun DomHunch(pDomP);
	CHgBaseDrawStd baseDraw;

	// 좌우측 가각부 적용 여부
	BOOL bExp	= TRUE;

	// 횡단면도 /////////////////////////////
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	m_pStd->m_pARoadOptionStd->SetEnvType(&DomHunch, HCAD_CONC);
	
	// 횡단면도를 그릴때 수직으로 잘라서 그림
	long nDrawLeft	= 0;
	CDPoint vAng	= CDPoint(0, 1);

	CTwinVectorArray tvArrCross;
	pBri->GetTvCrossSection(tvArrCross, dSta, vAng, FALSE, TRUE, FALSE, nDrawLeft, bMirror, FALSE, FALSE, bExp);
	baseDraw.DrawTvArray(&Dom, tvArrCross);
	

//	double dTH	= pBri->GetHeightUpperSlab_OldOrNotUse(dSta) - pBri->m_dTS;
	double dTH	= pBri->GetHeightUpperSlab(pBri->GetJijumNumberByStation(dSta), 0, vAng) - pBri->m_dTS;
	if(Compare(dTH, 0.0, ">"))
	{
		DrawCrossSlabHunch(&DomHunch, dSta, vAng, FALSE, nDrawLeft, bMirror, bExp);
	}

	// 중심선
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
	double dLenExtUpper	= Dom.Always(Dom.GetDimExo() + Dom.GetDimVLen()) / 3 * 2;
	double dLenExtLower	= Dom.Always(15);
	
	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();
	double dMid	= rect.CenterPoint().x;
	Dom.LineTo(dMid, rect.bottom+dLenExtUpper, dMid, rect.top-dLenExtLower);


	CDRect rectTot;
	if(bRotate)
	{
		CDPoint xyOrg	= CDPoint(0, 0);
		Dom.Rotate(xyOrg, CDPoint(0, 1));
		DomHunch.Rotate(xyOrg, CDPoint(0, 1));
	}

	Dom.SetCalcExtRect();
	rectTot	= Dom.GetExtRect();
	*pDomP << Dom;
	*pDomP << DomHunch;

	// 배근도
	if(bMirror)
	{
		for(long cycle = 0; cycle < pBri->GetCountCycleMainRebar(); cycle++)
		{
			// 횡단면도 주철근
			DrawRebarMain_CrossSection_UpperSlab(&Dom, dSta, TRUE, 0, bMarkRebar, cycle==0&&bDimRebarU, 0, cycle, nDrawLeft, bExp, bExcludeDoupleMainRebar, TRUE);
			DrawRebarMain_CrossSection_UpperSlab(&Dom, dSta, FALSE, 0, bMarkRebar, cycle==0&&bDimRebarL, 0, cycle, nDrawLeft, bExp, bExcludeDoupleMainRebar, TRUE);
		}

		// 횡단면도 배력철근
		DrawRebarSupport_CrossSection_UpperSlab(&Dom, dSta, TRUE, 0, FALSE, bMarkRebar, nDrawLeft, bExp, TRUE);
		DrawRebarSupport_CrossSection_UpperSlab(&Dom, dSta, FALSE, 0, bDimRebarL, bMarkRebar, nDrawLeft, bExp, TRUE);

		// 횡단면도 헌치부 배력철근
		DrawRebarSupport_CrossSection_SlabHunch(&Dom, TRUE, dSta, 0, FALSE, bMarkRebar, nDrawLeft, bExp);

		// 횡단면도 전단철근
		DrawRebarShear_CrossSection_UpperSlab(&Dom, dSta, bMarkRebar, bDimRebarU, nDrawLeft);

		// 종거더 외곽선 표현
		DrawVirtualRebarJongGirder(&Dom, dSta, TRUE, TRUE);
		DrawVirtualRebarJongGirder(&Dom, dSta, FALSE, TRUE);
	}

	Dom.SetCalcExtRect();
	if(pBri->GetAngleJijum(0) != CDPoint(0, 1))
	{
		CDRect rect	= Dom.GetExtRect();

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		Dom.SetTextAlignHorz(TA_RIGHT);
		Dom.TextOut(rect.right, rect.top-Dom.GetTextHeight()*2, _T("* () 안의 치수는 사거리 치수임."));
	}


	if(bRotate)
	{
		CDPoint xyOrg	= CDPoint(0, 0);
		Dom.Rotate(xyOrg, CDPoint(0, 1));
	}

	*pDomP << Dom;

	return rectTot;	
}

void CDrawCrossSection::DrawCrossGuardWall(CDomyun *pDomP, long nHDan)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	
	CDPoint vAng(0,1);
	double sta = pBri->GetStationBridgeMid();
	CJijum* pJL = pBri->GetJijum(0);
	if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pJL->m_dLengthUgan[1];

	CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan);
	if(!pGW)	return;

	// 외형 Arr 그리기
	long nHDanAct = pBri->GetNumSlabNodeAct(sta, vAng, nHDan);
	double dLenHDan = pBri->GetLengthHDanAct(sta, vAng, nHDan);

	CTwinVectorArray tvArrRect;
	tvArrRect.RemoveAll();	
	CDPoint A[30];
	pBri->GetXySlabUpperAct(sta, vAng, A);
	pBri->GetTvCrossSection(tvArrRect, sta, vAng, TRUE, TRUE, TRUE, 0, FALSE, FALSE);
	CDRect rect = tvArrRect.GetRect();
	rect.left = A[nHDanAct].x - 800.0;
	rect.top += 100.0;
	rect.right = A[nHDanAct].x + dLenHDan + 800.0;
	rect.bottom -= 100.0;

	CDomyun Dom(pDomP);
	
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	m_pStd->DrawCuttingTvArray(&Dom, tvArrRect, rect);
	
	*pDomP << Dom;
	
}

// 슬래브두께, 포장두께 표현
void CDrawCrossSection::DimThickCrossSection(CDomyun *pDom, double dSta, CDPoint vAng, BOOL bLeftDir)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	CDomyun Dom(pDom);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	// bLeftDir : 왼쪽에서부터 첫번째 차도 2/3 지점에 마킹한다.
	// !bLeftDir : 오른쪽에서 첫번째 차도 2/3 지점에 마킹
	BOOL bFind	= FALSE;
	CDPoint xyLeft(0, 0), xyRight(0, 0);
	long nQtyHDan	= pBri->GetQtyHDan(FALSE);
	if(!bLeftDir)
	{
		long i = 0; for(i = nQtyHDan-1; i > 0; i--)
		{
			CGuardWallRC *pGW	= pBri->GetGuardWallByHDan(i, FALSE);
			if(!pGW) continue;
			if(!pGW->IsTypeHDanOnlyNone()) continue;
			bFind	= TRUE;

			xyLeft	= pBri->GetXySlabHDanLeft(dSta, vAng, i, FALSE, FALSE);
			xyRight	= pBri->GetXySlabHDanLeft(dSta, vAng, i+1, FALSE, FALSE);

			break;

		}
	}
	else
	{
		long i = 0; for(i = 0; i < nQtyHDan; i++)
		{
			CGuardWallRC *pGW	= pBri->GetGuardWallByHDan(i, FALSE);
			if(!pGW) continue;
			if(!pGW->IsTypeHDanOnlyNone()) continue;
			bFind	= TRUE;

			xyLeft	= pBri->GetXySlabHDanLeft(dSta, vAng, i, FALSE, FALSE);
			xyRight	= pBri->GetXySlabHDanLeft(dSta, vAng, i+1, FALSE, FALSE);

			break;
		}
	}

	if(!bFind) return;

	
	double dLen	= ~(xyRight-xyLeft);
	CDPoint xyMatch	= xyLeft + (xyRight-xyLeft).Unit() * (dLen/2);
	if(!bLeftDir)
		xyMatch	= xyLeft + (xyRight - xyLeft).Unit() * dLen/4*3;
	

	CString sThickPave	= _T("");
	CString sThickSlab	= _T("");
	CString sPave	= _T("");
	if(pBri->m_bUnderGroundRahmen)
	{
		if(pBri->m_nTypePavement == 0) sPave	= "A.S.P";
		else sPave	= "CON'C";
	}
	else
	{
		if(pBri->m_nRoadMaterial == 0) sPave	= "ASCON";
		else sPave	= "LMC";
	}
	sThickPave.Format("%s:t=%smm", sPave, COMMA(pBri->GetThickPave()));
	sThickSlab.Format("슬래브:t=%smm", COMMA(pBri->m_dTS));
	if(pBri->GetThickPave() == 0)
	{
		GetSwap(sThickPave, sThickSlab);
		sThickSlab	= "";
	}

	double sw	= bLeftDir ? 1 : -1;
	Dom.GiSiArrowExtend(xyMatch.x, xyMatch.y - pBri->m_dTS/2, pBri->m_dTS/2 + Dom.Always(Dom.GetDimExo()+Dom.GetDimExe()+Dom.GetDimVLen()), max(Dom.GetTextWidth(sThickPave), Dom.GetTextWidth(sThickSlab)) * sw, 2, sThickPave, sThickSlab);

	if(pBri->GetThickPave() > 0)
	{
		Dom.GiSiArrowExtend(xyMatch.x, xyMatch.y + pBri->GetThickPave()/2, -pBri->GetThickPave()/2 + Dom.Always(Dom.GetDimExo()+Dom.GetDimExe()+Dom.GetDimVLen()), 0, 2, "", "");
	}
	

	*pDom << Dom;
}

void CDrawCrossSection::DimCrossSide(CDomyun *pDom, double dSta, CDPoint vAng, BOOL bLeftDim, BOOL bRightDim)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	CDPoint A[30];
	pBri->GetXySlabUpperAct(dSta, vAng, A);
	long nQtyHDan	= pBri->GetQtyHDanAct(dSta,vAng, FALSE);

	CGuardWallRC* pGW = pBri->GetGuardWallByHDan(0);

	double dH = pGW && !pGW->IsTypeHDanOnlyNone() ? pGW->m_dH1+pGW->m_dH2+pGW->m_dH3 : pBri->GetThickPave(FALSE);	// 전체 높이
	double dTS = pBri->m_dTS;
	double dHHS = pBri->m_dHHS;
	CDPoint xyDim = A[0];
	
	CDomyun Dom(pDom);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	if(bRightDim)
	{
		Dom.SetDirection("TOP");
		CDPoint xyStt	= CDPoint(-xyDim.x, xyDim.y-dTS-dHHS);
		Dom.DimMoveTo(xyStt.x, xyStt.y, 0, COMMA(dH));
		Dom.DimLineTo(dH, 0, "");
		Dom.DimLineTo(dTS, 0, COMMA(dTS));
		if(dHHS < Dom.GetTextWidth(COMMA(dHHS)))
			Dom.DimLineToOver(dHHS, 0, COMMA(dHHS));
		else
			Dom.DimLineTo(dH, 0, COMMA(dHHS));
		Dom.Rotate(xyStt, CDPoint(0, -1));
		Dom.Move(CDPoint(0, 1) * (dH + dTS + dHHS));
		*pDom << Dom;
	}

	
	if(bLeftDim)
	{
		Dom.SetDirection("BOTTOM");
		pGW = pBri->GetGuardWallByHDan(pBri->GetQtyHDan(FALSE)-1);
		dH	= pGW && !pGW->IsTypeHDanOnlyNone() ? pGW->m_dH1 + pGW->m_dH2 + pGW->m_dH3 : pBri->GetThickPave(FALSE);
		xyDim	= A[nQtyHDan];
		CDPoint xyStt	= CDPoint(-xyDim.x, xyDim.y-dTS);
		if(dH < Dom.GetTextWidth(COMMA(dH)))
		{
			Dom.DimMoveTo(xyStt.x, xyStt.y, 0, COMMA(dH));
			Dom.DimLineTo(dH, 0, "");
		}
		else
		{
			Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
			Dom.DimLineTo(dH, 0, COMMA(dH));
		}
		
		Dom.DimLineTo(dTS, 0, COMMA(dTS));
		
		Dom.Rotate(xyStt, CDPoint(0, -1));
		Dom.Move(CDPoint(0, 1) * (dH + dTS));
	}


	*pDom << Dom;
}

CDRect CDrawCrossSection::DrawRebar_CrossSectionStd_LowerSlab( CDomyun *pDomP, double dSta, BOOL bDimRebarU, BOOL bDimRebarL, BOOL bMarkRebar, BOOL bRotate, BOOL bMirror, BOOL bExcludeDoupleMainRebar )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return CDRect(0, 0, 0, 0);

	CDomyun Dom(pDomP);
	CDomyun DomHunch(pDomP);
	CHgBaseDrawStd baseDraw;

	// 좌우측 가각부 적용 여부
	BOOL bExp	= TRUE;

	// 횡단면도 /////////////////////////////
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	m_pStd->m_pARoadOptionStd->SetEnvType(&DomHunch, HCAD_CONC);

	// 횡단면도를 그릴때 수직으로 잘라서 그림
	long nDrawLeft	= 0;
	CDPoint vAng	= CDPoint(0, 1);

	CTwinVectorArray tvArrCross;
	pBri->GetTvCrossSectionLowerSlab(tvArrCross, dSta, vAng, nDrawLeft, bMirror, bExp);
	baseDraw.DrawTvArray(&Dom, tvArrCross);


	//	double dTH	= pBri->GetHeightUpperSlab_OldOrNotUse(dSta) - pBri->m_dTS;
	double dTH	= pBri->GetHeightLowerSlab(pBri->GetJijumNumberByStation(dSta), 0, vAng) - pBri->m_dTS_Lower;
	if(Compare(dTH, 0.0, ">"))
	{
		DrawCrossLowerSlabHunch(&DomHunch, dSta, vAng, FALSE, nDrawLeft, bMirror, bExp);
	}

	// 중심선
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
	double dLenExtUpper	= Dom.Always(Dom.GetDimExo() + Dom.GetDimVLen()) / 3 * 2;
	double dLenExtLower	= Dom.Always(15);

	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();
	double dMid	= rect.CenterPoint().x;
	Dom.LineTo(dMid, rect.bottom+dLenExtUpper, dMid, rect.top-dLenExtLower);


	CDRect rectTot;
	if(bRotate)
	{
		CDPoint xyOrg	= CDPoint(0, 0);
		Dom.Rotate(xyOrg, CDPoint(0, 1));
		DomHunch.Rotate(xyOrg, CDPoint(0, 1));
	}

	Dom.SetCalcExtRect();
	rectTot	= Dom.GetExtRect();
	*pDomP << Dom;
	*pDomP << DomHunch;

	// 배근도
	if(bMirror)
	{
		for(long cycle = 0; cycle < pBri->GetCountCycleMainRebar(); cycle++)
		{
			// 횡단면도 주철근
			DrawRebarMain_CrossSection_LowerSlab(&Dom, dSta, TRUE, 0, bMarkRebar, cycle==0&&bDimRebarU, 0, cycle, nDrawLeft, bExp, bExcludeDoupleMainRebar, TRUE);
			DrawRebarMain_CrossSection_LowerSlab(&Dom, dSta, FALSE, 0, bMarkRebar, cycle==0&&bDimRebarL, 0, cycle, nDrawLeft, bExp, bExcludeDoupleMainRebar, TRUE);
		}

		// 횡단면도 배력철근
		DrawRebarSupport_CrossSection_LowerSlab(&Dom, dSta, TRUE, 0, FALSE, bMarkRebar, nDrawLeft, bExp, TRUE);
		DrawRebarSupport_CrossSection_LowerSlab(&Dom, dSta, FALSE, 0, bDimRebarL, bMarkRebar, nDrawLeft, bExp, TRUE);

		// 횡단면도 헌치부 배력철근
		DrawRebarSupport_CrossSection_SlabHunch(&Dom, FALSE, dSta, 0, FALSE, bMarkRebar, nDrawLeft, bExp);

		// 횡단면도 전단철근
		DrawRebarShear_CrossSection_LowerSlab(&Dom, dSta, bMarkRebar, bDimRebarU, nDrawLeft);
	}

	Dom.SetCalcExtRect();
	if(pBri->GetAngleJijum(0) != CDPoint(0, 1))
	{
		CDRect rect	= Dom.GetExtRect();

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		Dom.SetTextAlignHorz(TA_RIGHT);
		Dom.TextOut(rect.right, rect.top-Dom.GetTextHeight()*2, _T("* () 안의 치수는 사거리 치수임."));
	}


	if(bRotate)
	{
		CDPoint xyOrg	= CDPoint(0, 0);
		Dom.Rotate(xyOrg, CDPoint(0, 1));
	}

	*pDomP << Dom;

	return rectTot;	
}

CDRect CDrawCrossSection::DrawRebar_CrossSectionStd_LowerSlab( CDomyun *pDomP, double dStaLeft, double dStaRight, BOOL bDimRebarU, BOOL bDimRebarL, BOOL bMarkRebar, BOOL bRotate, BOOL bMirror/*=TRUE*/, BOOL bExcludeDoupleMainRebar/*=FALSE*/ )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return CDRect(0, 0, 0, 0);

	CDomyun DomLeft(pDomP);
	CDomyun DomRight(pDomP);
	CDomyun DomLeftHunch(pDomP);
	CDomyun DomRightHunch(pDomP);
	CHgBaseDrawStd baseDraw;

	// 좌우측 가각부 적용 여부
	BOOL bExpLeft	= TRUE;
	BOOL bExpRight	= FALSE;

	BOOL bSameStaLR	= FALSE;	// 좌우측 동일 sta을 그리는 경우인지?
	if(Compare(dStaLeft, dStaRight, "="))
	{
		bSameStaLR	= TRUE;
		bExpRight	= TRUE;
	}


	// 일단 횡단면도 가각적용 무조건 안함 ///////////
	bExpLeft	= FALSE;
	bExpRight	= FALSE;
	//////////////////////////////////////////////////


	// 횡단면도 /////////////////////////////
	m_pStd->m_pARoadOptionStd->SetEnvType(&DomLeft, HCAD_CONC);
	m_pStd->m_pARoadOptionStd->SetEnvType(&DomRight, HCAD_CONC);
	m_pStd->m_pARoadOptionStd->SetEnvType(&DomLeftHunch, HCAD_CONC);
	m_pStd->m_pARoadOptionStd->SetEnvType(&DomRightHunch, HCAD_CONC);

	// 횡단면도는 가각부 적용이 되면 안된다.
//	double dStaSmallestWidth	= pBri->GetStationSmallestWidthOfUpperSlab();

	// 횡단면도를 그릴때 수직으로 잘라서 그림
	long nDrawLeft	= bSameStaLR ? 0 : -1;
	CDPoint vAng	= CDPoint(0, 1);

	CTwinVectorArray tvArrCross;
	pBri->GetTvCrossSectionLowerSlab(tvArrCross, dStaLeft, vAng, nDrawLeft, bMirror, bExpLeft);
	baseDraw.DrawTvArray(&DomLeft, tvArrCross);
	if(!bSameStaLR)
	{
		pBri->GetTvCrossSectionLowerSlab(tvArrCross, dStaRight, vAng, 1, bMirror, bExpRight);
		baseDraw.DrawTvArray(&DomRight, tvArrCross);
	}

// 	double dTHL	= pBri->GetHeightUpperSlab_OldOrNotUse(dStaLeft) - pBri->m_dTS;
// 	double dTHR	= pBri->GetHeightUpperSlab_OldOrNotUse(dStaRight) - pBri->m_dTS;
	double dTHL	= pBri->GetHeightLowerSlab(pBri->GetJijumNumberByStation(dStaLeft), 0, vAng) - pBri->m_dTS_Lower;
	double dTHR	= pBri->GetHeightLowerSlab(pBri->GetJijumNumberByStation(dStaRight), 0, vAng) - pBri->m_dTS_Lower;
	if(Compare(dTHL, 0.0, ">") && pBri->IsHunchRebarPlane(dStaLeft, TRUE))
	{
		DrawCrossLowerSlabHunch(&DomLeftHunch, dStaLeft, vAng, FALSE, nDrawLeft, bMirror, bExpLeft);
	}

	if(Compare(dTHR, 0.0, ">") && pBri->IsHunchRebarPlane(dStaRight, TRUE))
	{
		if(!bSameStaLR)
		{
			DrawCrossLowerSlabHunch(&DomRightHunch, dStaRight, vAng, FALSE, 1, bMirror, bExpRight);
		}
	}

	// 중심선
	// sLeft와 sRight는 확률상인 데이타일뿐..
	// 실제로 정확하게 지점부인지 fr중앙부인지는 모름
	// 대략 sLeft가 더 작으면 시점부를 선택했을 것이라 가정하고 데이타를 결정함.
	CString sLeft	= dStaLeft > dStaRight ? "중앙부" : "지점부";
	CString sRight	= dStaLeft < dStaRight ? "중앙부" : "지점부";

	if(!bSameStaLR)
	{
		BOOL bVert	= Compare(vAng.GetAngleDegree(), 90.0, "=");
		m_pStd->m_pARoadOptionStd->SetEnvType(&DomLeft,HCAD_CENT);
		double dLenExtUpper	= DomLeft.Always(DomLeft.GetDimExo() + DomLeft.GetDimVLen()) / 3 * 2;
		double dLenExtLower	= DomLeft.Always(15);
		
		DomLeft.SetCalcExtRect();
		CDRect rectLeft	= DomLeft.GetExtRect();
		CDRect rectRight = DomRight.GetExtRect();
		CDRect rect	= m_pStd->MergeRectAndRect(rectLeft, rectRight);
		double dMid	= pBri->GetMidCrossSection(bVert);
		DomLeft.LineTo(dMid, rect.bottom+dLenExtUpper, dMid, rect.top-dLenExtLower);

		// 중앙부 지점부 표시
		m_pStd->m_pARoadOptionStd->SetEnvType(&DomLeft, HCAD_STRS);
		CConcSymbol sym(m_pStd->m_pARoadOptionStd);
		sym.ArrowSide(&DomLeft, sLeft, sRight, CDPoint(dMid, rect.bottom+(dLenExtUpper*3.5/5)));
	}

	double dHDiff	= 0;
	//CLineInfo *pLine	= pBri->GetLineBase();
	// 헌치는 dStaSmallest의 높이에 맞게 이동 //
	/*
	dHDiff	= pLine->GetElevationPlanEl(dStaSmallestWidth) - pLine->GetElevationPlanEl(dStaLeft);
	DomLeftHunch.Move(CDPoint(0, dHDiff));
	dHDiff	= pLine->GetElevationPlanEl(dStaSmallestWidth) - pLine->GetElevationPlanEl(dStaRight);
	DomRightHunch.Move(CDPoint(0, dHDiff));
	*/
	

	// 우측을 좌측에 맞게 이동
	if(!bSameStaLR)
	{
		dHDiff	= pBri->GetXyLowerSlabBottomByOffsetFrStation(dStaLeft, 0, 0).y - pBri->GetXyLowerSlabBottomByOffsetFrStation(dStaRight, 0, 0).y;
		DomRight.Move(CDPoint(0, dHDiff));
	}


	CDRect rectTot;
	if(bRotate)
	{
		CDPoint xyOrg	= CDPoint(0, 0);
		DomLeft.Rotate(xyOrg, CDPoint(0, 1));
		DomRight.Rotate(xyOrg, CDPoint(0, 1));
		DomLeftHunch.Rotate(xyOrg, CDPoint(0, 1));
		DomRightHunch.Rotate(xyOrg, CDPoint(0, 1));
	}

	DomLeft.SetCalcExtRect();
	rectTot	= DomLeft.GetExtRect();
	
	if(!bSameStaLR)
	{
		DomRight.SetCalcExtRect();
		rectTot	= m_pStd->MergeRectAndRect(rectTot, DomRight.GetExtRect());
	}

	*pDomP << DomLeft;
	*pDomP << DomRight;
	*pDomP << DomLeftHunch;
	*pDomP << DomRightHunch;

	if(bMirror)
	{
		pBri->InitValueRebarForCrossSection();
		pBri->SettingValueRebarForCrossSection(dStaLeft, dStaRight);

		long nDimDanUpper	= pBri->GetDanMainRebarLowerSlab(0, TRUE, 0);
		long nDimDanLower	= pBri->GetDanMainRebarLowerSlab(0, FALSE, 0);

		long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			for(long cycle = 0; cycle < pBri->GetCountCycleMainRebar(); cycle++)
			{
				// 횡단면도 주철근
				DrawRebarMain_CrossSection_LowerSlab(&DomLeft, dStaLeft, TRUE, dan, bMarkRebar, cycle==0&&bDimRebarU&&dan==0, nDimDanUpper, cycle, nDrawLeft, bExpLeft, bExcludeDoupleMainRebar);
				DrawRebarMain_CrossSection_LowerSlab(&DomLeft, dStaLeft, FALSE, dan, bMarkRebar, cycle==0&&bDimRebarU&&dan==0, nDimDanLower, cycle, nDrawLeft, bExpLeft, bExcludeDoupleMainRebar);
				if(!bSameStaLR)
				{
					DrawRebarMain_CrossSection_LowerSlab(&DomRight, dStaRight, TRUE, dan, bMarkRebar, cycle==0&&bDimRebarU&&dan==0, nDimDanUpper, cycle, 1, bExpRight, bExcludeDoupleMainRebar);
					DrawRebarMain_CrossSection_LowerSlab(&DomRight, dStaRight, FALSE, dan, bMarkRebar, cycle==0&&bDimRebarU&&dan==0, nDimDanLower, cycle, 1, bExpRight, bExcludeDoupleMainRebar);
				}
			}

			// 횡단면도 배력철근
			DrawRebarSupport_CrossSection_LowerSlab(&DomLeft, dStaLeft, TRUE, dan, bDimRebarL, bMarkRebar, nDrawLeft, bExpLeft);
			DrawRebarSupport_CrossSection_LowerSlab(&DomLeft, dStaLeft, FALSE, dan, FALSE, bMarkRebar, nDrawLeft, bExpLeft);
			if(!bSameStaLR)
			{
				DrawRebarSupport_CrossSection_LowerSlab(&DomRight, dStaRight, TRUE, dan, bDimRebarL, bMarkRebar, 1, bExpRight);
				DrawRebarSupport_CrossSection_LowerSlab(&DomRight, dStaRight, FALSE, dan, FALSE, bMarkRebar, 1, bExpRight);
			}

			// 횡단면도 헌치부 배력철근
			if(dan==0)		// #32311
			{
				DrawRebarSupport_CrossSection_SlabHunch(&DomLeft, FALSE, dStaLeft, dan, FALSE, bMarkRebar, nDrawLeft, bExpLeft);
				if(!bSameStaLR)
				{
					DrawRebarSupport_CrossSection_SlabHunch(&DomRight, FALSE, dStaRight, dan, FALSE, bMarkRebar, 1, bExpRight);
				}
			}
		}

		// 횡단면도 전단철근
		DrawRebarShear_CrossSection_LowerSlab(&DomLeft, dStaLeft, bMarkRebar, bDimRebarU, nDrawLeft);
		if(!bSameStaLR)
		{
			DrawRebarShear_CrossSection_LowerSlab(&DomRight, dStaRight, bMarkRebar, bDimRebarU, 1);
		}
	}

	// 우측을 좌측에 맞게 이동
	if(!bSameStaLR)
	{
		DomRight.Move(CDPoint(0, dHDiff));
	}

	DomLeft.SetCalcExtRect();
	DomRight.SetCalcExtRect();

	// 사보강 배치에선 사거리 치수 뺌
	if(pBri->m_nTypePlacingMainRebar_UpperSlab == 1 || pBri->m_nTypePlacingMainRebar_UpperSlab == 2)
	{
		if(pBri->GetAngleJijum(0) != CDPoint(0, 1))
		{
			CDRect rect	= DomLeft.GetExtRect();
			if(!bSameStaLR)
				rect	= m_pStd->MergeRectAndRect(rect, DomRight.GetExtRect());

			m_pStd->m_pARoadOptionStd->SetEnvType(&DomLeft, HCAD_DIML);
			DomLeft.SetTextAlignHorz(TA_RIGHT);
			DomLeft.TextOut(rect.right, rect.top-DomLeft.GetTextHeight()*2, _T("* () 안의 치수는 사거리 치수임."));
		}
	}


	if(bRotate)
	{
		CDPoint xyOrg	= CDPoint(0, 0);
		DomLeft.Rotate(xyOrg, CDPoint(0, 1));
		DomRight.Rotate(xyOrg, CDPoint(0, 1));
	}

	*pDomP << DomLeft;
	*pDomP << DomRight;


	return rectTot;
}

void CDrawCrossSection::DrawCrossLowerSlabHunch( CDomyun* pDomP, double sta, CDPoint vAng /*= CDPoint(0,1)*/, BOOL bDrawSlopPattern/*=TRUE*/, long nDrawLeft/*=0*/, BOOL bMirror/*=FALSE*/, BOOL bExp/*=FALSE*/, CDPoint vAngWall/*=CDPoint(0, 1)*/, BOOL bCalHeightHunch/*=TRUE*/, double dHeightHunch/*=0*/ )
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri->IsExp(sta))
		bExp	= FALSE;

	CTwinVectorArray tvArr;
	pBri->GetTvCrossLowerHunch(tvArr, sta, vAng, bDrawSlopPattern, nDrawLeft, bMirror, bExp, vAngWall, bCalHeightHunch, dHeightHunch);

	CDomyun Dom(pDomP);

	m_pStd->DrawTvArray(&Dom, tvArr);

	*pDomP << Dom;
}

void CDrawCrossSection::DrawRebarShear_CrossSection_LowerSlab( CDomyun *pDomP, double sta, BOOL bMarkRebar, BOOL bDimRebar, long nDrawLeft, BOOL bMatchUseSta/*=FALSE*/ )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);

	// 철근 가져와서 그리기
	CHgBaseDrawStd baseDraw;
	CTwinVectorArray tvArr, tvArr2;
	CTwinVector tv;
	double dStaOrg	= sta;
	pBri->GetTvRebarShear_CrossSection_LowerSlab(tvArr, tvArr2, dStaOrg, nDrawLeft, FALSE, FALSE, FALSE, TRUE);
	long nSize	= tvArr.GetSize();
	BOOL bHidden	= FALSE;
	for(long d = 0; d < 2; d++)
	{
		CTwinVectorArray *pTvArr	= d == 0 ? &tvArr : &tvArr2;
		nSize	= pTvArr->GetSize();
		long i = 0; for(i = 0; i < nSize; i++)
		{
			if(!bHidden)
				m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_SOLID);
			else
				m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_HIDDEN2_LINE);

			Dom.LineTo(pTvArr->GetAt(i).m_v1, pTvArr->GetAt(i).m_v2);
			bHidden	= !bHidden;
		}
	}

	CString sMark	= "S1";
	CString sDescription	= "하부슬래브 전단철근1";


	// sta에 헌치 철근이 있다면 헌치 전단 철근 마킹함 
	// 헌치 철근이 존재 하는지 검사
	double dStaCheck	= pBri->GetStationModifyForRebarCheck(sta);
	BOOL bFind	= FALSE;
	long i = 0; for(i = 0; i < pBri->m_pArrRCMainLowerSlabHunch[0].GetSize(); i++)
	{
		CRebarInfoCycle *pRC	= pBri->m_pArrRCMainLowerSlabHunch[0].GetAt(i);
		double dLeft	= pRC->m_tvArrRebar.GetXyLeft().x;
		double dRight	= pRC->m_tvArrRebar.GetXyRight().x;
		if((fabs(dStaCheck-dLeft) <= 100 || Compare(dStaCheck, dLeft, ">=")) && (fabs(dStaCheck-dRight) <= 100 || Compare(dStaCheck, dRight, "<=")))
		{
			bFind	= TRUE;
			break;
		}
	}

	if(bFind)
	{
		sDescription	= "하부슬래브 전단철근2";
	}


	sMark	= pBri->GetMarkRebarShear(0, FALSE, FALSE, FALSE, sDescription);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		CTwinVectorArray tvArrCrossUpper;
		pBri->GetTvCrossSectionLowerSlab(tvArrCrossUpper, sta, CDPoint(0, 1), 0, FALSE, FALSE);

		CTwinVectorArray tvArrShearSameLen, tvArrShearSameLen2;
		// 마크를 하기 위해서는 길이가 동일한 철근을 다시 가져 와야 됨.
		pBri->GetTvRebarShear_CrossSection_LowerSlab(tvArrShearSameLen, tvArrShearSameLen2, dStaOrg, nDrawLeft, FALSE, FALSE, TRUE, TRUE);

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		CConcSymbol sym(m_pStd->m_pARoadOptionStd);

		CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_ShearRebar_LowerSlab;
		CString sDia	= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(ePartLowerSlab, TRUE));
		CDPoint xy		= CDPoint(0, 0);
		CDPoint xyMark	= CDPoint(0, 0);
		BOOL bLeft		= nDrawLeft == -1 || nDrawLeft == 0;

		for(long type = 0; type < 2; type++)
		{
			CTwinVectorArray *pTvArr	= type == 0 ? &tvArrShearSameLen : &tvArrShearSameLen2;

			if(type == 1)
			{
				sDescription.Format("하부슬래브 전단철근1_1");
				sMark	= pBri->GetMarkRebarShear(0, FALSE, FALSE, FALSE, sDescription);
			}

			if(sMark==_T("")) continue;

			// 마크
			CDPointArray xyArrCount;
			nSize	= pTvArr->GetSize();
			long i = 0; for(i = 0; i < nSize; i++)
				xyArrCount.Add(pTvArr->GetAt(i).GetXyMid());

			for(i = 0; i < xyArrCount.GetSize(); i++)
				sym.RebarCountMarkOne(&Dom, xyArrCount.GetAt(i));
			xyArrCount.Sort(TRUE);


			nSize	= xyArrCount.GetSize();
			if(nSize > 0)
			{
				// 마크할 위치 찾음
				if(nSize == 1)
					xyMark	= xyArrCount.GetAt(0);
				else
					xyMark	= bLeft ?	(xyArrCount.GetAt(1)+xyArrCount.GetAt(0))/2 :
					(xyArrCount.GetAt(nSize-1)+xyArrCount.GetAt(nSize-2))/2;

				// 선 긋기
				// 직선으로 만듬
				for(i = 0; i < xyArrCount.GetSize()-1; i++)
				{
					Dom.LineTo(xyArrCount[i], xyArrCount[i+1]);
				}

				// 마크함.
// 				double dHeightShear	= tvArr.GetSize() > 0 ? tvArr.GetAt(0).GetVerLength() : pBri->m_dTS;
				CDPoint xyMatch(0, 0);
				CDRect rectCross	= tvArrCrossUpper.GetRect();
				double dHeightShear		= rectCross.top - xyMark.y;
				double dH			= dHeightShear+Dom.Always(MARK_BASE_HEIGHT);
				// sym.RebarMarkLineUpper(&Dom, xyMark, sMark, sDia, bLeft, dH, FALSE);

				CRebar *pRb = pBri->GetRebarShearByDescription(sDescription);
				SRebarMarkInputData mark_input_data;
				SRebarMarkviewData mark_view_data;
				mark_input_data.mark = sMark;
				mark_input_data.dia = sDia;
				mark_input_data.position = xyMark;
				mark_input_data.ctc = pBri->GetCTCRebarShear_LowerSlab();
				mark_input_data.type_rebar = pRb? pRb->m_nTypeRebar : -1;
				mark_input_data.left_dir = bLeft;
				mark_input_data.height = dH;

				mark_view_data.count_mark = FALSE;
				mark_view_data.line_rebar = TRUE;
				mark_view_data.mark_pos = eMarkSide;

				sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
			}
		}
	} 

	if(bDimRebar)
	{

	}

	*pDomP << Dom;
}

void CDrawCrossSection::DrawRebarSupport_CrossSection_LowerSlab( CDomyun *pDomP, double sta, BOOL bUpper, long nDan, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft, BOOL bExp/*=FALSE*/, BOOL bUseMatchSta/*=FALSE*/ )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	long nMaxDan	= pBri->GetDanMainRebarLowerSlab(0, bUpper, 0);
	if(nDan == -1) nDan = nMaxDan-1;
	if(nDan < 0 || nDan > nMaxDan-1) return;

	if(!pBri->IsExp(sta))
		bExp	= FALSE;


	// 주철근이 없으면 배력근도 그리지 않는다.
	double dStaCheck	= pBri->GetStationModifyForRebarCheck(sta);
	long nIdxFind	= 0;
	if(!pBri->IsRebarMainBySta(FALSE, dStaCheck, 0, bUpper, nDan, nIdxFind)) return;
	///////////////////////////

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CHgBaseDrawStd baseDraw;
	long upper	= bUpper ? iUPPER : iLOWER;


	// 상부 슬래브
	// 상면 ///////////////////////////////////////////////////////////////////////
	CTwinVectorArray tvArrUpper, tvArrSupLeft, tvArrSupRight;
	CTwinVectorArray tvArrUpperAll, tvArrSupLeftAll, tvArrSupRightAll;
	pBri->GetTvRebarSupport_CrossSection_LowerSlab(tvArrUpper, tvArrSupLeft, tvArrSupRight, sta, bUpper, nDan, nDrawLeft, TRUE, TRUE, bExp, 0, TRUE);
	if(bMarkRebar)
		pBri->GetTvRebarSupport_CrossSection_LowerSlab(tvArrUpperAll, tvArrSupLeftAll, tvArrSupRightAll, sta, bUpper, nDan, 0, TRUE, TRUE, bExp, 0, TRUE);
	baseDraw.DrawTvArray(&Dom, tvArrUpper, FALSE, FALSE);
	baseDraw.DrawTvArray(&Dom, tvArrSupLeft, FALSE, FALSE);
	baseDraw.DrawTvArray(&Dom, tvArrSupRight, FALSE, FALSE);

	// 상면이든 하면이든 무조건 오른쪽 끝부분에 하면 됨
	CVectorArray xyArr1, xyArrHunch1;	// 1 cycle
	CVectorArray xyArr2, xyArrHunch2;	// 2 cycle
	if(bUseMatchSta)
	{
		pBri->GetXyRebarMain_CrossSection_LowerSlab(xyArr1, xyArrHunch1, sta, bUpper, nDan, 0);
		pBri->GetXyRebarMain_CrossSection_LowerSlab(xyArr2, xyArrHunch2, sta, bUpper, nDan, 0);
	}
	else
	{
		long leftSta	= Compare(pBri->m_dStaLeftCross, sta, "=") ? iLEFT : iRIGHT;
		xyArr1	= pBri->m_xyArrMainRebarCrossLowerSlab[0][upper][nDan][leftSta];
		xyArr2	= pBri->m_xyArrMainRebarCrossLowerSlab[1][upper][nDan][leftSta];
		xyArrHunch1	= pBri->m_xyArrMainRebarCrossHunchLowerSlab[0][leftSta];
		xyArrHunch2	= pBri->m_xyArrMainRebarCrossHunchLowerSlab[1][leftSta];
	}


	BOOL bHunch	= bUpper && xyArrHunch1.GetSize() > 0;

	// 치수기입
	if(bDimRebar)
	{
		*pDomP << Dom;
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		CTwinVectorArray tvArrCross;
		pBri->GetTvCrossSectionLowerSlab(tvArrCross, sta, CDPoint(0, 1), 0);

		CDoubleArray dArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		CString sDir	= nDrawLeft == -1 ? "BOTTOM" : "TOP";
		CDPoint xyStt	= nDrawLeft == -1 ? tvArrCross.GetXyLeft() : tvArrCross.GetXyRight();
		xyStt.y	-= pBri->m_dTS_Lower;

		double dCoverLower	= pBri->m_dCoverLower_LowerSlab[0];
		double dCoverUpper	= pBri->m_dCoverUpper_LowerSlab[0];

		dArrDim.Add(dCoverUpper);
		if(bHunch)
		{
			long nJ	= pBri->GetJijumNumberByStation(sta);
			double dHeightHunch	= pBri->GetHeightHunchByJijum(nJ);

			dArrDim.Add(dHeightHunch-dCoverUpper);
			dArrDim.Add(dCoverUpper);
		}
		dArrDim.Add(pBri->m_dTS_Lower - dCoverLower - dCoverUpper);
		dArrDim.Add(dCoverLower);

		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, 0, sDir, -1, TRUE, FALSE, FALSE);
		Dom.Rotate(xyStt, CDPoint(0, -1));
		Dom.Move(CDPoint(0, 1) * dArrDim.Sum());
		*pDomP << Dom;
	}


	// 철근 마크
	// 배력 철근 마크
	CString sDescription	= _T("");
	if(bUpper)
		sDescription.Format("하부슬래브 상면 %d단 배력철근2", nDan+1);
	else
		sDescription.Format("하부슬래브 하면 %d단 배력철근", nDan+1);
	CString sMark	= pBri->GetMarkRebarSupport(0, FALSE, FALSE, FALSE, sDescription);

	//확폭or가각일 경우 해당 배력 철근을 못찾을수 있음
	if(pBri->m_bIsExp && sMark == _T(""))
	{
		CTwinVectorArray tvArr, tvArrAngStt, tvArrAngEnd, tvArrRAngStt, tvArrRAngEnd;
		CTwinVectorArray tvArrExpSttLAndR, tvArrExpSttLOrR, tvArrExpEndLAndR, tvArrExpEndLOrR;
		pBri->GetTvRebarSupport_Plane_Slab_Std(FALSE, tvArr, tvArrAngStt, tvArrAngEnd, tvArrRAngStt, tvArrRAngEnd, bUpper, nDan, 0);
		pBri->DivideRebarSupport_Plane_UpperSlabExpSection(tvArr, tvArrExpSttLAndR, tvArrExpSttLOrR, tvArrExpEndLAndR, tvArrExpEndLOrR);

		long nSize			= tvArr.GetSize();
		long nSizeStt_And	= tvArrExpSttLAndR.GetSize();
		long nSizeStt_Or		= tvArrExpSttLOrR.GetSize();
		long nSizeEnd_And = tvArrExpEndLAndR.GetSize();
		long nSizeEnd_Or   = tvArrExpEndLOrR.GetSize();

		CString sUpdn = bUpper? _T("상면") : _T("하면");

		for (long ang = 1 ; ang < 5; ang++)
		{
			if(ang == 1 && nSizeStt_And == 0) continue;
			else if(ang == 2 && nSizeStt_Or == 0) continue;
			else if(ang == 3 && nSizeEnd_And == 0) continue;
			else if(ang == 4 && nSizeEnd_Or == 0) continue;

			if(bUpper)
				sDescription.Format("하부슬래브 %s %d단 배력철근2_%d", sUpdn, nDan+1, 5+ang);
			else
				sDescription.Format("하부슬래브 %s %d단 배력철근%d", sUpdn, nDan+1, 5+ang);
			sMark	= pBri->GetMarkRebarSupport(0, FALSE, FALSE, FALSE, sDescription);

			if(sMark == _T("")) continue;
			else	break;
		}

	}



	if(bMarkRebar && m_pStd->IsMarkRebar(sMark) && tvArrUpper.GetSize() > 0)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		// ndrawLeft에 따라서 필요한 것만 골라냄.(DrawRebarMain_CrossSection_UpperSlab에서 가져온 코드) //////
		if(nDrawLeft != 0)
		{
			double dXMid	= pBri->GetMidCrossSection(TRUE);//

			long i = 0; for(i = 0; i < xyArr1.GetSize(); i++)
			{
				if((xyArr1[i].x < dXMid) && nDrawLeft == -1) continue;
				if((xyArr1[i].x > dXMid) && nDrawLeft == 1) continue;
				xyArr1.RemoveAt(i, 1);
				i--;
			}
			for(i = 0; i < xyArrHunch1.GetSize(); i++)
			{
				if((xyArrHunch1[i].x < dXMid) && nDrawLeft == -1) continue;
				if((xyArrHunch1[i].x > dXMid) && nDrawLeft == 1) continue;
				xyArrHunch1.RemoveAt(i, 1);
				i--;
			}

			for(i = 0; i < xyArr2.GetSize(); i++)
			{
				if((xyArr2[i].x < dXMid) && nDrawLeft == -1) continue;
				if((xyArr2[i].x > dXMid) && nDrawLeft == 1) continue;
				xyArr2.RemoveAt(i, 1);
				i--;
			}
			for(i = 0; i < xyArrHunch2.GetSize(); i++)
			{
				if((xyArrHunch2[i].x < dXMid) && nDrawLeft == -1) continue;
				if((xyArrHunch2[i].x > dXMid) && nDrawLeft == 1) continue;
				xyArrHunch2.RemoveAt(i, 1);
				i--;
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////

		// 2cycle이 있으면 1cycle와 2cycle 마지막거 중간에 기입하고 없으면 1cycle 마지막 2개 사이에 기입
		// 단. 좌측을 그릴는 거라면 시작 2개..
		long upper	= bUpper ? iUPPER : iLOWER;
		BOOL bLeft	= nDrawLeft == -1 || nDrawLeft == 0 ? TRUE : FALSE;
		CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_SupportRebar_LowerSlab[upper][nDan];


		CString sDia	= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(ePartLowerSlab));
		CDPoint xyMark(0, 0);

		// 상/하 배력철근
		long	nSize1	= xyArr1.GetSize(),
			nSize2	= xyArr2.GetSize();
		long i = 0; for(i = 0; i < nSize2; i++)
			xyArr1.Add(xyArr2.GetAt(i));
		nSize1	= xyArr1.GetSize();
		xyArr1.Sort(TRUE);

		CDPoint xyMove	= bUpper ? CDPoint(0, -1) * REBAR_RADIUS : CDPoint(0, 1) * REBAR_RADIUS;
		if(nSize1 > 1)
		{
			CVector v1 = xyArr1[(bLeft==bUpper) ? (int)(nSize1 * 0.1) : (int)(nSize1 * 0.8 - 1)]; 
			CVector v2 = xyArr1[(bLeft==bUpper) ? (int)(nSize1 * 0.1 + 1) : (int)(nSize1 * 0.8 - 2)];
			xyMark = (CDPoint(v1.x, v1.y) + CDPoint(v2.x, v2.y))/2 + xyMove;
		}
		else 
		{
			xyMark	= tvArrUpper[0].GetXyMid();
		}

		CRebarPlacing rb;
		CDPointArray xyArrMatch;
		rb.GetXyMatchTvArrAndLine(tvArrUpperAll, xyMark, CDPoint(0, 1), xyArrMatch);
		if(xyArrMatch.GetSize() > 0)
			xyMark	= xyArrMatch.GetAt(0);

		double dHeight = (bHunch)? (Dom.Always(MARK_BASE_HEIGHT) + pBri->m_dWSHH) : 0;
		double dAddHeight	= 0;
		if(nDan == 1)
			dAddHeight += (bUpper ? MARK_BASEHEIGHT_UPPER : MARK_BASEHEIGHT_LOWER);
		else if(nDan == 2)
			dAddHeight += (bUpper ? MARK_BASEHEIGHT_UPPER : MARK_BASEHEIGHT_LOWER) * 2;
		xyMark += dAddHeight * CDPoint(0, bUpper ? 1 : -1);

		CConcSymbol sym(m_pStd->m_pARoadOptionStd);
// 		if(bUpper)
// 			sym.RebarMarkLineUpper(&Dom, xyMark, sMark, sDia, bLeft, dHeight);
// 		else
// 			sym.RebarMarkLineLower(&Dom, xyMark, sMark, sDia, bLeft);

		CRebar *pRb = pBri->GetRebarSupportByDescription(sDescription);
		SRebarMarkInputData mark_input_data;
		SRebarMarkviewData mark_view_data;
		mark_input_data.mark = sMark;
		mark_input_data.dia = sDia;
		mark_input_data.position = xyMark;
		mark_input_data.ctc = pBri->GetCTCRebarSupport_Slab(FALSE, bUpper, nDan, TRUE);
		mark_input_data.type_rebar = pRb? pRb->m_nTypeRebar : -1;
		mark_input_data.left_dir = bLeft;
		mark_input_data.height = bUpper? dHeight : 0;

		mark_view_data.count_mark = TRUE;
		mark_view_data.line_rebar = TRUE;
		mark_view_data.mark_pos = bUpper? eMarkUpper : eMarkLower;

		sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

		// 헌치부 배력철근

		if(!bUpper && xyArrHunch2.GetSize() > 0 && nDrawLeft != 1)
		{
			sDescription.Format("하부슬래브 헌치부 배력철근");
			sMark	= pBri->GetMarkRebarSupport(0, FALSE, FALSE, FALSE, sDescription);
			if(m_pStd->IsMarkRebar(sMark))
			{
				long	nSize1	= xyArrHunch1.GetSize(),
					nSize2	= xyArrHunch2.GetSize();

				long i = 0; for(i = 0; i < nSize2; i++)
					xyArrHunch1.Add(xyArrHunch2.GetAt(i));
				nSize1	= xyArrHunch1.GetSize();
				xyArrHunch1.Sort(TRUE);

				if(nSize1 > 1)
					xyMark	= (xyArrHunch1[nSize1-1]+xyArrHunch1[nSize1-2])/2 + xyMove;
				else 
					xyMark	= tvArrUpper[tvArrUpper.GetSize()-1].GetXyMid();

				sym.RebarMarkLineLower(&Dom, xyMark, sMark, sDia, FALSE);

				pRb = pBri->GetRebarSupportByDescription(sDescription);
				mark_input_data.mark = sMark;
				mark_input_data.dia = sDia;
				mark_input_data.position = xyMark;
				mark_input_data.ctc = pBri->GetCTCRebarSupport_UpperSlabHunch();
				mark_input_data.type_rebar = pRb? pRb->m_nTypeRebar : -1;
				mark_input_data.left_dir = FALSE;
				mark_input_data.height = 0;

				mark_view_data.count_mark = TRUE;
				mark_view_data.line_rebar = TRUE;
				mark_view_data.mark_pos = eMarkLower;

				sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
			}
		}
	}

	*pDomP << Dom;
}

void CDrawCrossSection::DrawRebarMain_CrossSection_LowerSlab( CDomyun *pDomP, double sta, BOOL bUpper, long nDan, BOOL bMarkRebar, BOOL bDimRebar, long nDimDan, long nCycle, long nDrawLeft, BOOL bExp/*=FALSE*/, BOOL bExcludeDoupleMainRebar/*=FALSE*/, BOOL bUseMatchSta/*=FALSE*/ )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(!pBri->IsExp(sta))
		bExp	= FALSE;

	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	long upper	= bUpper ? iUPPER : iLOWER;


	CTwinVectorArray tvArrCrossSection, tvArrCrossSectionHunch;
	if(bUseMatchSta)
	{
		pBri->GetTvCrossSectionLowerSlab(tvArrCrossSection, sta, CDPoint(0, 1),  0, FALSE, bExp);
		pBri->GetTvCrossLowerHunch(tvArrCrossSectionHunch, sta, CDPoint(0, 1), FALSE, 0, FALSE, bExp);
	}
	else
	{
		pBri->GetTvCrossSectionLowerSlab(tvArrCrossSection, pBri->m_dStationBridgeStt, CDPoint(0, 1), 0);
		pBri->GetTvCrossLowerHunch(tvArrCrossSectionHunch, pBri->m_dStationBridgeStt, CDPoint(0, 1), FALSE, 0, FALSE, FALSE);
	}
	CDRect rectCross(0, 0, 0, 0), rectHunch(0, 0, 0, 0);
	rectCross	= tvArrCrossSection.GetRect();
	rectHunch	= tvArrCrossSectionHunch.GetRect();
	CDRect rect	= m_pStd->MergeRectAndRect(rectHunch, rectCross);


	// 철근 가져와서 그리기(하부슬래브)
	long leftSta	= Compare(pBri->m_dStaLeftCross, sta, "=") ? iLEFT : iRIGHT;
	CHgBaseDrawStd baseDraw;
	CRebarPlacing rb;
	CVectorArray xyArr, xyArrHunch;
	if(bUseMatchSta)
	{
		pBri->GetXyRebarMain_CrossSection_LowerSlab(xyArr, xyArrHunch, sta, bUpper, nDan, nCycle, TRUE, bUseMatchSta);
	}
	else
	{
		xyArr	= pBri->m_xyArrMainRebarCrossLowerSlab[nCycle][upper][nDan][leftSta];
		xyArrHunch	= pBri->m_xyArrMainRebarCrossHunchLowerSlab[nCycle][leftSta];
	}

	xyArr.Sort();
	xyArrHunch.Sort();

	CDPointArray xyArrCycle1Exp[2], xyArrCycle2Exp[2];
	if(bExp)
	{
		pBri->GetXyRebarMain_CrossSection_SlabExp(xyArrCycle1Exp[iLEFT], xyArrCycle2Exp[iLEFT], FALSE, TRUE, TRUE, bUpper, nDan);
		pBri->GetXyRebarMain_CrossSection_SlabExp(xyArrCycle1Exp[iRIGHT], xyArrCycle2Exp[iRIGHT], FALSE, TRUE, FALSE, bUpper, nDan);
	}

	// 치수 기입시 치수 기입용으로 따로 구해놓음
	CDPointArray xyArrForDim[3], xyArrHunchForDim[3];
	CDPointArray xyArrForDimExp1, xyArrForDimExp2;
	if(bDimRebar)
	{
		if(bUseMatchSta)
		{
			for(long cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
			{
				CVectorArray vecArrTmp, vecArrHunchTmp;
				pBri->GetXyRebarMain_CrossSection_LowerSlab(vecArrTmp, vecArrHunchTmp, sta, bUpper, nDan, cycle, TRUE, bUseMatchSta);

				// [3] : 0 :좌, 1 : 기본, 2:우
				CVector vec(0, 0, 0);
				long i = 0; for(i = 0; i < vecArrTmp.GetSize(); i++)
				{
					vec	= vecArrTmp.GetAt(i);
					xyArrForDim[(long)(vec.z)+1].Add(vec);
				}

				for(i = 0; i < vecArrHunchTmp.GetSize(); i++)
				{
					vec	= vecArrHunchTmp.GetAt(i);
					xyArrHunchForDim[(long)(vec.z)+1].Add(vec);
				}
			}			
		}
		else
		{
			for(long cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
			{
				// [3] : 0 :좌, 1 : 기본, 2:우
				CVector vec(0, 0, 0);
				long i = 0; for(i = 0; i < pBri->m_xyArrMainRebarCrossLowerSlab[cycle][upper][nDan][leftSta].GetSize(); i++)
				{
					vec	= pBri->m_xyArrMainRebarCrossLowerSlab[cycle][upper][nDan][leftSta].GetAt(i);
					long nIdx = (long)(vec.z)+1;
					xyArrForDim[nIdx].Add(vec);
				}

				for(i = 0; i < pBri->m_xyArrMainRebarCrossHunchLowerSlab[cycle][leftSta].GetSize(); i++)
				{
					vec	= pBri->m_xyArrMainRebarCrossHunchLowerSlab[cycle][leftSta].GetAt(i);
					xyArrHunchForDim[(long)(vec.z)+1].Add(vec);
				}
			}
		}


		if(bExp && pBri->m_bIsExp)
		{
			pBri->GetXyRebarMain_CrossSection_SlabExp(xyArrForDimExp1, xyArrForDimExp2, FALSE, TRUE, TRUE, bUpper, nDan);
			long i = 0; for(i = 0; i < xyArrForDimExp1.GetSize(); i++)
				xyArrForDim[1].Add(xyArrForDimExp1.GetAt(i));
			for(i = 0; i < xyArrForDimExp2.GetSize(); i++)
				xyArrForDim[1].Add(xyArrForDimExp2.GetAt(i));

			pBri->GetXyRebarMain_CrossSection_SlabExp(xyArrForDimExp1, xyArrForDimExp2, FALSE, TRUE, FALSE, bUpper, nDan);
			for(i = 0; i < xyArrForDimExp1.GetSize(); i++)
				xyArrForDim[1].Add(xyArrForDimExp1.GetAt(i));
			for(i = 0; i < xyArrForDimExp2.GetSize(); i++)
				xyArrForDim[1].Add(xyArrForDimExp2.GetAt(i));
		}

		long s = 0; for(s = 0; s < 3; s++)
		{
			xyArrForDim[s].Sort();
			xyArrHunchForDim[s].Sort();
			rb.RemoveSamePoint(xyArrForDim[s], TRUE);
			rb.RemoveSamePoint(xyArrHunchForDim[s], TRUE);
		}
	}

	// 헌치부가 있는지...
	BOOL bIsHunch	= xyArrHunch.GetSize() > 0;

	// 치수 기입
	long nDimDanAddLower	= 0;
	double dYForDim	= 0;
	if(bDimRebar)
	{
		CDRect rect(0, 0, 0, 0);
		CTwinVectorArray tvArrCross;
		pBri->GetTvCrossSectionLowerSlab(tvArrCross, sta, CDPoint(0, 1), TRUE, TRUE, FALSE);
		rect	= tvArrCross.GetRect();
		rect.top += pBri->IsOutWall(TRUE) ? pBri->m_dWSHH : pBri->m_dWEHH;
		dYForDim	= rect.bottom;

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		CTwinVectorArray tvArr, tvArrSupLeft, tvArrSupRight;
		CTwinVectorArray tvArrRebar, tvArrSupLeftRebar, tvArrSupRightRebar;
		pBri->GetTvRebarSupport_CrossSection_LowerSlab(tvArr, tvArrSupLeft, tvArrSupRight, sta, TRUE, -1, 0, TRUE, FALSE, bExp, 0, TRUE);
		pBri->GetTvRebarSupport_CrossSection_LowerSlab(tvArrRebar, tvArrSupLeftRebar, tvArrSupRightRebar, sta, TRUE, 0, 0, TRUE, FALSE, bExp, 0, TRUE);
		double dAngle	= pBri->GetAngleByTarget(BRIDGE_OFF_STT_SLAB, 0).GetAngleDegree();

		long s=0, s_stt=0, s_end=3; 
		if(nDrawLeft==-1)	// 단부 - 32311
			s_stt = 1;
		if(nDrawLeft==1)		// 일반부 - 32311
			s_end = 1;

		for(s = s_stt; s < s_end; s++)
		{
			long nSize		= xyArrForDim[s].GetSize();
			if(nSize == 0) continue;

			CDPointArray xyArrDim;
			CStringArray sArrText;
			CStringArray sArrUnderText;
			CString sDir	= bUpper ? "TOP" : "BOTTOM";
			CDPoint xyStt	= CDPoint(0, 0);
			nDimDanAddLower	= s == 1 && (xyArrForDim[0].GetSize() > 0 || xyArrForDim[2].GetSize() > 0) ? 1 : 0;

			if(s == 1 || s == 2)
			{
				// 사보강 배치시는 기본 철근에 피복 안 넣어줌.
				xyArrDim.Add(tvArr.GetXyLeft());
				if(s == 2 || pBri->m_nTypePlacingMainRebar_UpperSlab == 0)
				{
					xyArrDim.Add(tvArrRebar.GetXyLeft());
				}
			}

			long i = 0; for(i = 0; i < nSize; i++)
			{
				xyStt	= xyArrForDim[s].GetAt(i);
				xyArrDim.Add(xyStt);
			}


			if(s == 0 || s == 1)
			{
				// 사보강 배치시는 기본 철근에 피복 안 넣어줌.
				if(s == 0 || pBri->m_nTypePlacingMainRebar_UpperSlab == 0)
				{
					xyArrDim.Add(tvArrRebar.GetXyRight());
				}
				xyArrDim.Add(tvArr.GetXyRight());
			}
			xyArrDim.Sort(TRUE);

			xyStt	= CDPoint(xyArrDim.GetAt(0).x, bUpper ? rect.top : rect.bottom);

			BOOL bIncludeSttRep	= s == 0 || s == 1;
			BOOL bIncludeEndRep	= s == 1 || s == 2;
			CString sMsgStt	= _T("");
			// 사보강 배치면 직철근인지 사철근인지 표시해준다.
			if(0)//pBri->m_nTypePlacingMainRebar_UpperSlab != 0)
			{
				if(s == 2) sMsgStt = _T("(사방향철근)");
				else if(s == 1) sMsgStt = _T("(직방향철근)");
			}
			// 사거리 표현 뺌(사보강배치일때만 뺌)
			// 도저히 정확한 표현이 안되므로, 사용자가 평면도의 사거리를 사용해서 직접 넣기를 권함.
			// 2005.10.08 이혜철차장 & 이재호대리
			if(pBri->m_nTypePlacingMainRebar_UpperSlab == 1 || pBri->m_nTypePlacingMainRebar_UpperSlab == 2)
			{
				dAngle	= 90;
			}
			
			baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan+nDimDanAddLower, sDir, -1, s==1, TRUE, FALSE, TRUE, FALSE, FALSE, bIncludeSttRep, bIncludeEndRep, dAngle, TRUE, sMsgStt, 0.0001);
		}
		nDimDanAddLower	= (xyArrForDim[0].GetSize() > 0 || xyArrForDim[2].GetSize() > 0) ? 1 : 0;
	}

	// ndrawLeft에 따라서 필요한 것만 골라냄.
	if(nDrawLeft != 0)
	{
		double dXMid	= pBri->GetMidCrossSection(TRUE);//

		long i = 0; for(i = 0; i < xyArr.GetSize(); i++)
		{
			if((xyArr[i].x < dXMid) && nDrawLeft == -1) continue;
			if((xyArr[i].x > dXMid) && nDrawLeft == 1) continue;
			xyArr.RemoveAt(i, 1);
			i--;
		}

		long left = 0; for(left = 0; left < 2; left++)
		{
			long i = 0; for(i = 0; i < xyArrCycle1Exp[left].GetSize(); i++)
			{
				if((xyArrCycle1Exp[left][i].x < dXMid) && nDrawLeft == -1) continue;
				if((xyArrCycle1Exp[left][i].x > dXMid) && nDrawLeft == 1) continue;
				xyArrCycle1Exp[left].RemoveAt(i, 1);
				i--;
			}

			for(i = 0; i < xyArrCycle2Exp[left].GetSize(); i++)
			{
				if((xyArrCycle2Exp[left][i].x < dXMid) && nDrawLeft == -1) continue;
				if((xyArrCycle2Exp[left][i].x > dXMid) && nDrawLeft == 1) continue;
				xyArrCycle2Exp[left].RemoveAt(i, 1);
				i--;
			}
		}
	}

	CDPoint xyDirScale	= bUpper ? CDPoint(0, -1) : CDPoint(0, 1);
	CDPointArray xyArrDraw[2];
	CVector vec(0, 0, 0);
	long i = 0; for(i = 0; i < xyArr.GetSize(); i++)
	{
		vec	= xyArr.GetAt(i);
		xyArrDraw[vec.z == 0 ? 0 : 1].Add(vec);
	}

	// 겹치는 철근 제거
	if(bExcludeDoupleMainRebar)
	{
		if(xyArrDraw[0].GetSize() > 0)
		{
			double dLeftX	= xyArrDraw[0].GetAt(0).x;
			double dRightX	= xyArrDraw[0].GetAt(xyArrDraw[0].GetSize()-1).x;

			CDPoint xy(0, 0);
			for(i = 0; i < xyArrDraw[1].GetSize(); i++)
			{
				xy	= xyArrDraw[1].GetAt(i);
				if(xy.x >= dLeftX && xy.x <= dRightX)
				{
					xyArrDraw[1].RemoveAt(i);
					i--;
				}
			}
		}
	}


	long s = 0; for(s = 0; s < 2; s++)
	{
		baseDraw.DrawPointRebar(&Dom, xyArrDraw[s], xyDirScale, -1, -1, FALSE, s);
	}

	long left = 0; for(left = 0; left < 2; left++)
	{
		baseDraw.DrawPointRebar(&Dom, xyArrCycle1Exp[left], xyDirScale, -1, -1);
		baseDraw.DrawPointRebar(&Dom, xyArrCycle2Exp[left], xyDirScale, -1, -1);
	}

	double dHDiff	= bIsHunch && bUpper ? pBri->m_dWSHH : 0;
	// 마킹
	if(bMarkRebar)
	{
		double dSta	= sta;
		// 지점부 횡단을 그릴때는 슬래브 끝단 sta를 기준으로 그리므로 철근을 못 찾음
		// 그래서 지점부 횡단일 경우 시점으로 옮긴다.
		double dStaSlab	= pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, TRUE);
		CDPoint vAng	= pBri->GetAngleByTarget(BRIDGE_OFF_STT_SLAB, 0);
		if(dSta < dStaSlab+pBri->m_dCoverWallOut[0]/vAng.y)
			dSta	= dStaSlab + pBri->m_dCoverWallOut[0]/vAng.y;

		CRebarInfoCycle *pRC	= pBri->GetRCMain_Slab(FALSE, dSta, bUpper, nDan, nCycle);
		if(pRC)
		{
			if(pRC->m_bExist)
			{
				CString sMark	= pRC->m_szMark;
				if(m_pStd->IsMarkRebar(sMark))
				{
					CRebar *pRb = pBri->GetRebarMainByDescription(pRC->m_sDescription);

					CMarkDomyun mark;
					mark.SetDomyun(&Dom, m_pStd->m_pARoadOptionStd);
					mark.SetRebar(xyArrDraw[0]);

					// 설정
					mark.m_sMark					= sMark;
					mark.m_sDia						= pBri->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pBri->GetValueFy(ePartLowerSlab));
					mark.m_nTypeMarkPointUpAndDown	= bUpper ? TYPE_MARKPOINT_UPPER : TYPE_MARKPOINT_LOWER;
					mark.m_nTypeMarkDir				= TYPE_MARKDIR_RIGHT;
					mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
					mark.m_dPosScaleMarkStt			= nCycle == 0 ? 0.3 : 0.6;
					if(pBri->GetCountCycleMainRebar() == 4)
					{
						switch(nCycle)
						{
						case 0: mark.m_dPosScaleMarkStt	= 0.2; break;
						case 1: mark.m_dPosScaleMarkStt	= 0.4; break;
						case 2: mark.m_dPosScaleMarkStt	= 0.6; break;
						case 3: mark.m_dPosScaleMarkStt	= 0.8; break;
						}
					}
					if(bIsHunch && !bUpper)	// 헌치부가 있는 경우 하부 마크는 위치 조정
					{
						mark.m_dPosScaleMarkStt		= nCycle == 0 ? 0.2 : 0.4;
						if(pBri->GetCountCycleMainRebar() == 4)
						{
							switch(nCycle)
							{
							case 0: mark.m_dPosScaleMarkStt	= 0.2; break;
							case 1: mark.m_dPosScaleMarkStt	= 0.4; break;
							case 2: mark.m_dPosScaleMarkStt	= 0.6; break;
							case 3: mark.m_dPosScaleMarkStt	= 0.8; break;
							}
						}
					}
					if(nDan == 1)
					{
						mark.m_dPosScaleMarkStt += 0.1;
						dHDiff += bUpper ? MARK_BASEHEIGHT_UPPER : MARK_BASEHEIGHT_LOWER;
					}
					else if(nDan == 2)
					{
						mark.m_dPosScaleMarkStt += 0.3;
						dHDiff += (bUpper ? MARK_BASEHEIGHT_UPPER : MARK_BASEHEIGHT_LOWER) * 2;
					}
					mark.m_dAddHeight				= dHDiff;
					mark.m_dCtc						= pBri->GetCtcRebarMain(FALSE);
					mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;
					mark.MarkRebar();
				}
			}
		}
	}

	// 헌치부 주철근  ///////////////////////////////////////////////////////////////////
	// 치수 기입
	if(bIsHunch)
	{
		// 헌치 치수는 원래 안 하는 거라고 해서 막음
		// 필요시 풀면 바로 적용됨.
		if(0)//bDimRebar)
		{
			CTwinVectorArray tvArr, tvArrSupLeft, tvArrSupRight;
			pBri->GetTvRebarSupport_CrossSection_LowerSlab(tvArr, tvArrSupLeft, tvArrSupRight, sta, TRUE, -1, 0, TRUE, FALSE, bExp);

			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
			double dAngle	= pBri->GetAngleByTarget(BRIDGE_OFF_STT_SLAB, 0).GetAngleDegree();

			long s = 0; for(s = 0; s < 3; s++)
			{
				long nSize		= xyArrHunchForDim[s].GetSize();
				if(nSize == 0) continue;

				CDPointArray xyArrDim;
				CStringArray sArrText;
				CStringArray sArrUnderText;
				CString sDir	= "BOTTOM";
				CDPoint xyStt	= CDPoint(0, 0);
				long nDimDanAdd	= s == 1 && (xyArrHunchForDim[0].GetSize() > 0 || xyArrHunchForDim[2].GetSize() > 0) ? 1 : 0;


				if(s == 1 || s == 2)
					xyArrDim.Add(tvArr.GetXyLeft());

				nSize	= xyArrHunchForDim[s].GetSize();
				long i = 0; for(i = 0; i < nSize; i++)
					xyArrDim.Add(xyArrHunchForDim[s].GetAt(i));
				if(s == 0 || s == 1)
					xyArrDim.Add(tvArr.GetXyRight());
				xyArrDim.Sort(TRUE);

				xyStt	= CDPoint(xyArrDim.GetAt(0).x, dYForDim);

				baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan+nDimDanAddLower+1+nDimDanAdd, sDir, -1, s==1, TRUE, FALSE, TRUE, FALSE, FALSE, TRUE, TRUE, dAngle);
			}
		}


		// ndrawLeft에 따라서 필요한 것만 골라냄.
		if(nDrawLeft != 0)
		{
			double dXMid	= pBri->GetMidCrossSection(TRUE);//

			long i = 0; for(i = 0; i < xyArrHunch.GetSize(); i++)
			{
				if((xyArrHunch[i].x < dXMid) && nDrawLeft == -1) continue;
				if((xyArrHunch[i].x > dXMid) && nDrawLeft == 1) continue;
				xyArrHunch.RemoveAt(i, 1);
				i--;
			}
		}

		// 철근 가져와서 그리기(상부슬래브 헌치부)
		xyArrDraw[0].RemoveAll();
		xyArrDraw[1].RemoveAll();
		for(i = 0; i < xyArrHunch.GetSize(); i++)
		{
			vec	= xyArrHunch.GetAt(i);
			xyArrDraw[vec.z == 0 ? 0 : 1].Add(xyArrHunch[i]);
		}

		// 겹치는 철근 제거
		if(bExcludeDoupleMainRebar)
		{
			if(xyArrDraw[0].GetSize() > 0)
			{
				double dLeftX	= xyArrDraw[0].GetAt(0).x;
				double dRightX	= xyArrDraw[0].GetAt(xyArrDraw[0].GetSize()-1).x;

				CDPoint xy(0, 0);
				for(i = 0; i < xyArrDraw[1].GetSize(); i++)
				{
					xy	= xyArrDraw[1].GetAt(i);
					if(xy.x >= dLeftX && xy.x <= dRightX)
					{
						xyArrDraw[1].RemoveAt(i);
						i--;
					}
				}
			}
		}

		for(s = 0; s < 2; s++)
		{
			baseDraw.DrawPointRebar(&Dom, xyArrDraw[s], xyDirScale, -1, -1, FALSE, s);
		}

		// 마킹
		if(bMarkRebar && xyArrDraw[0].GetSize() > 0)
		{
			CRebarInfoCycle *pRC	= pBri->GetRCMain_SlabHunch(FALSE, sta, nCycle);
			if(pRC)
			{
				CString sMark	= pRC->m_szMark;
				if(m_pStd->IsMarkRebar(sMark))
				{
					CRebar *pRb = pBri->GetRebarMainByDescription(pRC->m_sDescription);

					CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
					mark.SetRebar(xyArrDraw[0]);

					// 설정
					mark.m_sMark					= sMark;
					mark.m_sDia						= pBri->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pBri->GetValueFy(ePartLowerSlab));
					mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_UPPER;
					mark.m_nTypeMarkDir				= TYPE_MARKDIR_RIGHT;
					mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
					mark.m_dPosScaleMarkStt			= nCycle == 0 ? 0.6 : 0.8;
					mark.m_dCtc						= pBri->GetCtcRebarMain(FALSE);
					mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;
					mark.MarkRebar();
				}
			}
		}
	}

	*pDomP << Dom;
}

void CDrawCrossSection::DrawCrossSectionBoxType( CDomyun* pDomP, double sta, CDPoint vAng /*= CDPoint(0,1)*/, BOOL bInclSubStruct/*=FALSE*/, BOOL bInclFootway/*=TRUE*/, BOOL bInclSlabLower/*=TRUE*/, BOOL bInclGuardWall/*=TRUE*/, BOOL bInclPave/*=TRUE*/, BOOL bDrawPile/*=TRUE*/, BOOL bRoadNotSlab/*=FALSE*/, BOOL bExp/*=FALSE*/, BOOL bInner/*=TRUE*/, long nDrawLeftOfSubStruct/*=0*/, long nJSubStruct/*=-1*/, BOOL bHatch/*=TRUE*/, BOOL bRubble /*=FALSE*/ )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri->m_bUnderGroundRahmen && bRoadNotSlab)	return;
	if(!pBri->IsExp(sta))
		bExp	= FALSE;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DrawSlabDosang(pDomP, sta, vAng, bRoadNotSlab);
	CTwinVectorArray tvArr;
	pBri->GetTvCrossSection(tvArr, sta, vAng, bInclFootway, bInclSlabLower, bInclGuardWall, 0, FALSE, bInclPave, bRoadNotSlab, bExp);
	m_pStd->DrawTvArray(&Dom, tvArr, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());

	// Box타입은 상단과 하단의 이동위치가 다르기 때문에 상부슬래브와 벽체를 따로 이동시켜준다.
	*pDomP << Dom;

	if(/*pBri->m_nTypeBridge==BRIDGE_TYPE_PORTAL && */bInclSubStruct)
	{
		BOOL bVert	= Compare(vAng.GetAngleDegree(), 90.0, "=");
		CTwinVector tv;
		int nJ = pBri->GetJijumNumberByStation(sta);
		CDPoint vAngJ	= pBri->GetAngleJijum(nJ);
		long nCountJijum	= pBri->GetCountJijum();

		nJ	= -1;
		long i = 0; for(i = 0; i < nCountJijum; i++)
		{
			double dWL	= 0;
			double dWR	= 0;
			double dStaTmp	= pBri->GetStationOnJijum(i);
			if(i == 0) 
			{
				if(Compare(dStaTmp, sta, ">=")) 
				{
					nJ	= i;
					break;
				}
				dWL	= pBri->m_dWS/vAngJ.y;
				dWR	= 0;
			}
			else if(i == nCountJijum-1)
			{
				if(Compare(dStaTmp, sta, "<="))
				{
					nJ	= i;
					break;
				}
				dWL	= 0;
				dWR	= pBri->m_dWE/vAngJ.y;
			}
			else 
			{
				CWallApp *pWall	= pBri->GetInWall(i-1);
				if(pWall)
				{
					dWL	= pWall->GetWidthWall()/vAngJ.y/ToDPointFrDegree(pWall->GetAngleWallByHor(TRUE)).y/2;
					dWR	= dWL;
				}
			}

			if(Compare(sta, dStaTmp-dWL, ">=") && Compare(sta, dStaTmp+dWR, "<="))
			{
				nJ	= i;
				break;
			}
		}

		if(nJSubStruct != -1)
			nJ	= nJSubStruct;

		// 도면이 그려진 뒤에 일괄적으로 내려주기 때문에 여기서 미리 벽체와 기초만 올려준다.
		double dDiffTop = 0;
		if(nJ>=0)
		{
			BOOL bLeft	= TRUE;
			CWallApp wall;
			// 벽체
			if(nJ == 0 || nJ == pBri->GetCountJijum()-1)
			{
				if(pBri->IsOutWall(nJ==0))
				{
					pBri->MakeWallByOutWall(wall, nJ==0, !bVert, bVert, bVert, 0, sta);

					// 가각부 처리
					if(bExp)
					{
						double dWidthLeftExp	= pBri->GetWidthDiffSlabActOrgAndGagak(nJ == 0, TRUE, FALSE);
						double dWidthRightExp	= pBri->GetWidthDiffSlabActOrgAndGagak(nJ == 0, FALSE, FALSE);
						wall.m_dWidthExp[iLEFT]		= dWidthLeftExp;
						wall.m_dWidthExp[iRIGHT]	= dWidthRightExp;

						double dWidthOverRebarExpLeft	= pBri->GetDiffLeftRightDistForGagakSabogang(nJ == 0, TRUE);
						double dWidthOverRebarExpRight	= pBri->GetDiffLeftRightDistForGagakSabogang(nJ == 0, FALSE);
						wall.m_dWidthOverRebarExp[iLEFT]	= dWidthOverRebarExpLeft;
						wall.m_dWidthOverRebarExp[iRIGHT]	= dWidthOverRebarExpRight;
					}

					m_pStd->m_pDrawWall->DrawLeftSide(&Dom, &wall, FALSE, FALSE, bLeft, nDrawLeftOfSubStruct, FALSE, TRUE);
					dDiffTop = wall.m_dDiffTop[bLeft? 0 : 1];
				}
			}
			else
			{
				CWallApp *pWall	= pBri->GetInWall(nJ-1);
				if(pWall)
				{
					if(pWall->m_bIs)
					{
						bLeft	= pWall->IsLeftSlope();
						// 지점 
						// (39571) 하부슬래브가 경사이고 헌치가 없을때는 벽체가 짧게 그려지기 때문에 길이를 보정해서 그려준다.
						BOOL bCorrectionEl = (pBri->m_bSlopeLowerSlab && pBri->m_nTypeJongdanForRebar != 0 && pBri->m_dHHL[nJ-1][iLOWER] == 0 && pBri->m_dHHR[nJ-1][iLOWER] == 0)? TRUE : FALSE;
						if(bCorrectionEl) pWall->m_dBottomElCorrect = pBri->GetXyLowerSlabBottomByOffsetFrStation(sta, 0, 0).y + pBri->m_dTS_Lower;
						m_pStd->m_pDrawWall->DrawLeftSide(&Dom, pWall, FALSE, FALSE, bLeft, nDrawLeftOfSubStruct, FALSE, TRUE, TRUE, TRUE, bCorrectionEl);
					}
					dDiffTop = pWall->m_dDiffTop[bLeft? 0 : 1];
				}
			}

			// 기초 확대기초
			m_pStd->m_pDrawExFooting->DrawLeftSideLowerBox(&Dom, pBri, sta, vAng, nDrawLeftOfSubStruct, bDrawPile, bDrawPile, !bVert, bLeft, TRUE, TRUE, bRubble);
			// Box타입은 중간지점부에도 하부 슬래브가 있기때문에 그려줘야 한다. 높이를 맞추기위해 Station은 그대로 두고 헌치없이 기초만 그려주자.
			if(nDrawLeftOfSubStruct != 0)
				m_pStd->m_pDrawExFooting->DrawLeftSideLowerBox(&Dom, pBri, sta, vAng, (nDrawLeftOfSubStruct==-1)? 1 : -1, bDrawPile, bDrawPile, !bVert, bLeft, TRUE, FALSE, bRubble);			
		}

		if(dDiffTop != 0)
			Dom.Move(CDPoint(0, dDiffTop));

		*pDomP << Dom;
	}

	if(bRoadNotSlab)
	{
		if(pBri->m_nSeparBri != 2)	// 하행인 경우 좌측을 그리지 않음(분리교량이 아닌 경우는 모두 그림)
		{
			pBri->GetTvCrossSectionUnderNori(tvArr, sta, vAng, TRUE);// 도로좌측의 성토사면
			m_pStd->DrawTvArray(&Dom, tvArr, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());
		}
		if(pBri->m_nSeparBri != 1)	// 상행인 경우 우측을 그리지 않음(분리교량이 아닌 경우는 모두 그림)
		{
			pBri->GetTvCrossSectionUnderNori(tvArr, sta, vAng, FALSE);// 도로우측의 성토사면
			m_pStd->DrawTvArray(&Dom, tvArr, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());
		}

		if(bHatch)
			DrawCrossSectionUnderHeapSoilSymbol(&Dom, sta, vAng);

		*pDomP << Dom;
	}	
}
