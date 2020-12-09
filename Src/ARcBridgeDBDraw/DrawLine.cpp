// DrawLine.cpp: implementation of the CDrawLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARcBridgeDBDraw.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define DIKE_UPPER_WIDTH	115.0
#define DIKE_UPPER_FRONT	43.0
#define DIKE_UPPER_HEIGHT	150.0
#define DIKE_LOWER_HEIGHT	300.0

#define JONG_TABLE_PICK		1.66667

CDrawLine::CDrawLine()
{

}

CDrawLine::~CDrawLine()
{

}

void CDrawLine::DrawCant(CDomyun *pDomP)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;
	CConcSymbol sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);

	CLineInfo* pLine = pBri->m_pARcBridgeDataStd->GetLineApp();

	// 그리기 시작	
// 	double dSta = 0;
	double dWidthSolid = Dom.Always(35);
	double dFactorY = 150;
//////////////////////////////////////////////////////////////////////////
	int nCnt = pBri->GetCountJijum();
	int i=0; for(i=0; i<nCnt; i++)
	{
		double dSta = pBri->GetStationOnJijum(i, 0);						// ok

		double dRslope, dLslope;
		double dValueX, dValueY;

		dRslope = pLine->GetPyungubaeRight(dSta, TRUE);
		dLslope = pLine->GetPyungubaeRight(dSta, FALSE);

		dValueX = Dom.Always(0);
		dValueY = Dom.Always(-30*i);

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		Dom.MoveTo(dValueX+dWidthSolid, dValueY);
		Dom.LineTo(dValueX+Dom.Always(100), dValueY);

		Dom.Solid(dValueX, dValueY, 
			dValueX-dWidthSolid, dValueY+Dom.Always(dFactorY*dLslope), 
			dValueX-dWidthSolid, dValueY, 
			dValueX, dValueY);
		Dom.Solid(dValueX, dValueY, 
			dValueX+dWidthSolid, dValueY+Dom.Always(dFactorY*dRslope), 
			dValueX+dWidthSolid, dValueY, 
			dValueX, dValueY);

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT); // 중심선

		Dom.MoveTo(dValueX, dValueY+Dom.Always(-8));
		Dom.LineTo(dValueX, dValueY+Dom.Always(8));

		// TEXT
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_TXTG); // 일반문자

		Dom.SetTextAlignHorz(TA_CENTER);
		CString str(_T(""));
		if(dSta == pBri->m_dStationBridgeStt) str = _T("교 량 시 점");
		else if(dSta == pBri->m_dStationBridgeStt+pBri->m_dLengthBridge) str = _T("교 량 종 점");
		else str.Format("교 각 %d", i);
		Dom.TextOut(dValueX+Dom.Always(80), dValueY+Dom.Always(2), str);

		str.Format("STA.%dK + %.3f", (int)dSta/1000000, dSta/1000-((int)(dSta/1000000)*1000));
		Dom.TextOut(dValueX+Dom.Always(80), dValueY+Dom.Always(-4), str);

		Dom.SetTextAlignHorz(TA_LEFT);
		sym.DrawSymbolCLMark(&Dom, dValueX, dValueY+Dom.Always(9.2),TRUE," OF ROAD");

		Dom.SetTextAlignHorz(TA_CENTER);
		int ioffset;

		if(dLslope >= 0) ioffset = 1;
		else ioffset = -4;
		str.Format("%.3f%%", dLslope*100);
		Dom.TextOut(dValueX-Dom.Always(45), dValueY+Dom.Always(dFactorY*dLslope+ioffset),str);

		if(dRslope >= 0) ioffset = 1;
		else ioffset = -4;
		str.Format("%.3f%%", dRslope*100);
		Dom.TextOut(dValueX+Dom.Always(45), dValueY+Dom.Always(dFactorY*dRslope+ioffset),str);
	}
	
//////////////////////////////////////////////////////////////////////////
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SYMB); // 심볼
	CDRect dr;

	Dom.SetCalcExtRect();
	dr = Dom.GetExtRect();

	dr.left   -= Dom.Always(6);
	dr.top	  -= Dom.Always(8);
	dr.bottom += Dom.Always(8);
	dr.right  += Dom.Always(6);
	Dom.Rectangle(dr);

	*pDomP << Dom;
	
}

void CDrawLine::DrawBoringLegend(CDomyun *pDomP, double dStaStt, double dStaEnd)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CConcSymbol sym(m_pStd->m_pARoadOptionStd);
	CARcBridgeDataStd* pDataStd = pBri->m_pARcBridgeDataStd;

	CStringArray arrStr;
	arrStr.RemoveAll();
// 그리기 시작	
	long nCnt = 0;	

	// 전에는 위의 조건(지반선 범위내에 있는것만 출력한다.)이었으나
	// 멀리 떨어져 있어도 계산에서 적용은된다.
	// 따라서 멀리 떨어져 있어도 도면출력할때도 출력이 되어야 한다.
	long nCount	= pDataStd->m_pArrBoringEx.GetSize();
	for(long boring = 0; boring < nCount; boring++)
	{	
		CBoringDataEx *pBoringEx = pDataStd->m_pArrBoringEx.GetAt(boring);

		// 보링이 교량에 포함되고 있는지 판단해서
		// 없으면 패스
		BOOL bIsInclude	= FALSE;
		long nCountJijum	= pBri->GetCountJijum();
		for(long j = 0; j < nCountJijum; j++)
		{
			if(pDataStd->IsLsdDesign())
			{
				CFootingApp *pFooting = pBri->GetFootingByJijumNumber(j);
				if(pFooting)
				{
					if(boring == pFooting->GetBoringIndex())
					{
						bIsInclude = TRUE;
						break;
					}
				}
			}
			else
			{
				double dStation = pBri->GetStationOnJijum(j,0);
				 
				CBoringDataEx *pBoringExTmp = pBri->GetBoringDataExByStation(dStation);
				if(pBoringEx == pBoringExTmp)
				{
					bIsInclude	= TRUE;
					break;
				}
			}
		}
		if(!bIsInclude) continue;

		long nSize = pBoringEx->m_pArrBoring.GetSize();
		if(nSize == 0) continue;

		for(int i = 0; i<pBoringEx->m_pArrBoring.GetSize(); i++)
		{
			CBoringData *Bdata = pBoringEx->m_pArrBoring.GetAt(i);
			CDRect cr;
			BOOL bNew = TRUE;
			
			CString sName = sym.m_strSoilName[Bdata->m_nNameSoil];
			CString sPatSoil = sym.m_strSoilPatternName[Bdata->m_nNameSoil];
			
			for(int j=0; j<arrStr.GetSize(); j++)
			{
				if(!strcmp(sName, arrStr.GetAt(j))) bNew = FALSE; 
			}

			if(bNew)
			{
				m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SYMB); // 심볼
				
				cr.top = Dom.Always(-15 * (nCnt/2));
				cr.left = Dom.Always(50 * (nCnt%2));
				cr.right = cr.left+Dom.Always(10);
				cr.bottom = cr.top-Dom.Always(10);
				
				Dom.Rectangle(cr);
				Dom.HatchRect(cr, sPatSoil, 2000);
				
				m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_TXTG); // 일반문자
				
				Dom.SetTextAlignHorz(TA_LEFT);
				Dom.TextOut(cr.right+Dom.Always(3), cr.bottom, sName);
				
				arrStr.Add(sName);
				nCnt ++;
			}
		}
	}
	
	arrStr.RemoveAll();
// return
	CDRect dr;

	Dom.SetCalcExtRect();
	dr = Dom.GetExtRect();

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SYMB); // 심볼
	
	dr.left -= Dom.Always(6);
	dr.top	-= Dom.Always(6);
	dr.bottom += Dom.Always(6);
	dr.right += Dom.Always(6);
	Dom.Rectangle(dr);
	*pDomP << Dom;
}

// 지중라멘교 - 도로횡단구성
// void CDrawLine::DimRoadCrossSection(CDomyun *pDomP, double sta, BOOL bInput)
// {
// 	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
// 	if(pBri==NULL || !pBri->m_bUnderGroundRahmen) return;
// 
// 	CLineInfoApp* pLine = pBri->m_pARcBridgeDataStd->GetLineApp();
// 
// 	double dElCen   = pLine->GetElevationPlanEl(sta);
// 	double dElLeft  = pBri->GetElevationRoadSectionLeft(sta);
// 	double dElRight = pBri->GetElevationRoadSectionRight(sta);
// 	double dDistDim = pBri->m_dWidthSlabLeft;
// 	double dElDim   = max(max(dElCen,dElLeft),dElRight);
// 
// 	CDomyun Dom(pDomP);
// 
// 	CString str = _T("");
// 
// 	long nDimDan = 0;
// 	double dLenDim = 0;
// 	Dom.DimMoveTo(dDistDim,dElDim,nDimDan);
// 	dLenDim = pBri->m_dWidthRoadLeft - pBri->m_dWidthSlabLeft;
// 	if(dLenDim>0)
// 	{
// 		Dom.DimLineTo(dLenDim,nDimDan,COMMA(dLenDim));
// 	}
// 	double staJijum = pBri->GetStationOnJijum(0, 0);
// 	CDPoint vAng = CDPoint(0, 1);
// 	for(long nHDanUnder=0; nHDanUnder<pBri->GetQtyHDan(TRUE); nHDanUnder++)
// 	{
// 		dLenDim = pBri->GetLengthHDanAct(staJijum, vAng, nHDanUnder,TRUE);
// 		if(bInput)
// 		{
// 			str.Format("L%d",nHDanUnder+1);
// 			Dom.DimLineToExtend(dLenDim,nDimDan,str, Comma(dLenDim));
// 		}
// 		else
// 		{
// 			Dom.DimLineTo(dLenDim,nDimDan, Comma(dLenDim));
// 		}
// 	}
// 	dLenDim = pBri->m_dWidthSlabRight - pBri->m_dWidthRoadRight;
// 	if(dLenDim>0)
// 	{
// 		Dom.DimLineTo(dLenDim,nDimDan,COMMA(dLenDim));
// 	}
// 
// 	nDimDan++;
// 	dDistDim = pBri->m_dWidthRoadLeft;
// 	Dom.DimMoveTo(dDistDim,dElDim,nDimDan);
// 	dLenDim = pBri->m_dWidthRoadRight - pBri->m_dWidthRoadLeft;
// 	Dom.DimLineTo(dLenDim,nDimDan,COMMA(dLenDim));
// 
// 	*pDomP << Dom;
// }

void CDrawLine::DrawBroing(CDomyun *pDomP, CBoringDataEx *pBoringEx, BOOL bLeft, BOOL bMove, BOOL bFixWidth)
{
	CConcSymbol sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);
	if(!pBoringEx) return;
	
	double dWidthRect = bFixWidth ? 1000 : 0;

	if(pBoringEx->m_pArrBoring.GetSize()>0)
		{
			sym.DrawBoringSymbol(&Dom, &pBoringEx->m_pArrBoring, pBoringEx->m_EL, frM(pBoringEx->m_dStation), 1, FALSE, 0, TRUE, pBoringEx->m_sNameBoring,dWidthRect);
			Dom.Move(frM(pBoringEx->m_dStation),frM(pBoringEx->m_EL));
		}

	CDRect rc, rc2;
	rc = pDomP->GetExtRect();
	rc2 = Dom.GetExtRect();

	if(bMove)
	{
		if(bLeft)
		{
			Dom.Move(rc.left-rc2.right-Dom.Always(15), 0);
		}
		else
		{
			Dom.Move(rc.right-rc2.left, 0);
		}
	}
	*pDomP << Dom;
}


// 도로공사 표준도의  다이크 형식-2로 그림.
// 구조물 작업범위가 아니므로 치수등은 의미가 없음
// void CDrawLine::DrawCentGuardWall(CDomyun *pDomP, double sta)
// {
// }


// 차선표시
// void CDrawLine::DrawLineMarking(CDomyun *pDomP, double sta)
// {
// 	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
// 	if(pBri==NULL || !pBri->m_bUnderGroundRahmen) return;
// 
// 	CDomyun Dom(pDomP);
// 
// 	double dDist   = pBri->m_dWidthRoadLeft;
// 	double dElLeft = pBri->GetElevationRoadSectionLeft(sta);
// 
// 	double dWidth  = 100;
// 	double dHeight = 100;
// 	CDPoint xy1(0,0),xy2(0,0);
// 	CDPoint vAng = CDPoint(0, 1);
// 	for(long nHDanUnder=0; nHDanUnder<pBri->GetQtyHDan(TRUE); nHDanUnder++)
// 	{
// 		// 차선시작위치
// 		BOOL bDraw = (pBri->GetTypeHDan(nHDanUnder, TRUE)==HDAN_TYPE_ROAD_NONE) ? TRUE : FALSE;
// 		if(bDraw)
// 		{
// 			xy1 = CDPoint(dDist-dWidth/2,dElLeft+dHeight/2);
// 			xy2 = CDPoint(dDist+dWidth/2,dElLeft-dHeight/4);
// 			Dom.Solid(xy1.x,xy1.y,xy2.x,xy1.y,xy2.x,xy2.y,xy1.x,xy2.y);
// 		}
// 		dDist += pBri->GetLengthHDanAct(sta, vAng, nHDanUnder,TRUE);
// 		if(dDist<=0)
// 			dElLeft -= pBri->GetLengthHDanAct(sta, vAng, nHDanUnder,TRUE) * pBri->GetSlopeHDanByStation(sta, nHDanUnder,TRUE) * 0.01;
// 		else
// 			dElLeft += pBri->GetLengthHDanAct(sta, vAng, nHDanUnder,TRUE) * pBri->GetSlopeHDanByStation(sta, nHDanUnder,TRUE) * 0.01;
// 		// 차선종료위치
// 		if(bDraw && nHDanUnder<pBri->GetQtyHDan(TRUE)-1 && pBri->GetTypeHDan(nHDanUnder+1,TRUE) != HDAN_TYPE_ROAD_NONE)
// 		{
// 			xy1 = CDPoint(dDist-dWidth/2,dElLeft+dHeight/2);
// 			xy2 = CDPoint(dDist+dWidth/2,dElLeft-dHeight/4);
// 			Dom.Solid(xy1.x,xy1.y,xy2.x,xy1.y,xy2.x,xy2.y,xy1.x,xy2.y);
// 		}
// 	}
// 	*pDomP << Dom;
// }

// void CDrawLine::GetPointDikeFront(double sta, long nHDanUnder, CDPoint &xy1, CDPoint &xy2)
// {
// 	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
// 	if(pBri==NULL || !pBri->m_bUnderGroundRahmen) return;
// 	if(pBri->GetTypeHDan(nHDanUnder, TRUE)!=HDAN_TYPE_ROAD_DIKE) return;
// 
// 	double dDist   = pBri->m_dWidthRoadLeft;
// 	double dElLeft = pBri->GetElevationRoadSectionLeft(sta);
// 
// 	double H1 = DIKE_UPPER_HEIGHT;
// 	double H2 = max(DIKE_LOWER_HEIGHT,pBri->m_dUnderGroundPaveHeightPyo + pBri->m_dUnderGroundPaveHeightGee);
// 
// 	CDPoint xyDike(0,0);
// 	CDPoint vAng = CDPoint(0, 1);
// 	long i=0; for(i=0; i<=nHDanUnder; i++)
// 	{
// 		dDist += pBri->GetLengthHDanAct(sta, vAng, i,TRUE);
// 		if(dDist<=0)
// 			dElLeft -= pBri->GetLengthHDanAct(sta, vAng, i,TRUE) * pBri->GetSlopeHDanByStation(sta, i,TRUE) * 0.01;
// 		else
// 			dElLeft += pBri->GetLengthHDanAct(sta, vAng, i,TRUE) * pBri->GetSlopeHDanByStation(sta, i,TRUE) * 0.01;
// 	}
// 	BOOL bLeftSide = (dDist<0) ? TRUE : FALSE;
// 	double sw = bLeftSide ? 1.0 : -1.0;
// 	CDPoint vDirWall = CDPoint(sw*DIKE_UPPER_FRONT,-DIKE_UPPER_HEIGHT).Unit();
// 	if(bLeftSide)
// 	{
// 		xyDike.x = dDist;
// 		xyDike.y = dElLeft;
// 	}
// 	else
// 	{
// 		xyDike.x = dDist   - pBri->GetLengthHDanAct(sta, vAng, nHDanUnder,TRUE);
// 		xyDike.y = dElLeft - pBri->GetLengthHDanAct(sta, vAng, nHDanUnder,TRUE) * pBri->GetSlopeHDanByStation(sta, nHDanUnder,TRUE) * 0.01;
// 	}
// 	xy1 = xyDike - fabs(H1/vDirWall.y)*vDirWall;
// 	xy2 = xyDike + fabs(H2/vDirWall.y)*vDirWall;
// }


// void CDrawLine::GetPointDikeBack(double sta, long nHDanUnder, CDPoint &xy1, CDPoint &xy2)
// {
// 	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
// 	if(pBri==NULL || !pBri->m_bUnderGroundRahmen) return;
// 	if(pBri->GetTypeHDan(nHDanUnder, TRUE)!=HDAN_TYPE_ROAD_DIKE) return;
// 
// 	double dDist   = pBri->m_dWidthRoadLeft;
// 	double dElLeft = pBri->GetElevationRoadSectionLeft(sta);
// 
// 	double H1 = DIKE_UPPER_HEIGHT;
// 	double H2 = max(DIKE_LOWER_HEIGHT,pBri->m_dUnderGroundPaveHeightPyo + pBri->m_dUnderGroundPaveHeightGee);
// 	double W1 = DIKE_UPPER_WIDTH;
// 
// 	CDPoint xyDike(0,0);
// 	CDPoint vAng = CDPoint(0, 1);
// 	long i=0; for(i=0; i<=nHDanUnder; i++)
// 	{
// 		dDist += pBri->GetLengthHDanAct(sta, vAng, i,TRUE);
// 		if(dDist<=0)
// 			dElLeft -= pBri->GetLengthHDanAct(sta, vAng, i,TRUE) * pBri->GetSlopeHDanByStation(sta, i,TRUE) * 0.01;
// 		else
// 			dElLeft += pBri->GetLengthHDanAct(sta, vAng, i,TRUE) * pBri->GetSlopeHDanByStation(sta, i,TRUE) * 0.01;
// 	}
// 	BOOL bLeftSide = (dDist<0) ? TRUE : FALSE;
// 	double sw = bLeftSide ? 1.0 : -1.0;
// 	CDPoint vDirWall = CDPoint(sw*DIKE_UPPER_FRONT,-DIKE_UPPER_HEIGHT).Unit();
// 	if(bLeftSide)
// 	{
// 		xyDike.x = dDist;
// 		xyDike.y = dElLeft;
// 	}
// 	else
// 	{
// 		xyDike.x = dDist   - pBri->GetLengthHDanAct(sta, vAng, nHDanUnder,TRUE);
// 		xyDike.y = dElLeft - pBri->GetLengthHDanAct(sta, vAng, nHDanUnder,TRUE) * pBri->GetSlopeHDanByStation(sta, nHDanUnder,TRUE) * 0.01;
// 	}
// 
// 	CDPoint xyFront1(0,0),xyFront2(0,0);
// 	GetPointDikeFront(sta,nHDanUnder,xyFront1,xyFront2);
// 	xy1 = CDPoint(xyFront1.x - W1*sw, xyFront1.y);
// 	xy2 = CDPoint(xy1.x,xy1.y-H1-H2);
// }


// void CDrawLine::GetTvArrayDike(double sta, long nHDanUnder, CTwinVectorArray &tvArr)
// {
// 	tvArr.RemoveAll();
// 
// 	CDPoint xyDike[4];
// 	long nIndex = 0;
// 	GetPointDikeFront(sta,nIndex,xyDike[0],xyDike[1]);
// 	GetPointDikeBack(sta,nIndex,xyDike[2],xyDike[3]);
// 
// 	CTwinVector tv;
// 	tv.m_v1 = CVector(xyDike[1]);
// 	tv.m_v2 = CVector(xyDike[0]);
// 	tvArr.Add(tv);
// 	tv.m_v1 = CVector(xyDike[0]);
// 	tv.m_v2 = CVector(xyDike[2]);
// 	tvArr.Add(tv);
// 	tv.m_v1 = CVector(xyDike[0]);
// 	tv.m_v2 = CVector(xyDike[3]);
// 	tvArr.Add(tv);
// 	tv.m_v1 = CVector(xyDike[3]);
// 	tv.m_v2 = CVector(xyDike[1]);
// 	tvArr.Add(tv);
// 	tvArr.Sort();
// }

void CDrawLine::DrawJongDanTable(CDomyun *pDomP, CDPoint ptBase, double dStaStt, double dStaEnd)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(pBri==NULL) return;

	CLineInfo *pLine = pBri->m_pARcBridgeDataStd->GetLineApp();

	CDomyun Dom(pDomP);

	Dom.SetTextAngle(0);
	Dom.SetTextAlignHorzEx(DOM_TEXT_JUST_LEFT);
		
	double dRowHeight  = Dom.GetTextWidth("0000.0000");
	double dStaBaseStt = GetStationJongDanStt();
	double dStaBaseEnd = GetStationJongDanEnd();
	double dStaPre = dStaBaseStt;
	double dStaHead = (long)dStaBaseStt / 1000000;	

	CDoubleArray dArraySta;
	
	// 기본 측점 삽입
	for(double dSta=dStaBaseStt; dSta<=dStaBaseEnd; dSta += 20000)
	{
		dArraySta.Add(dSta);
	}
	// 특정 측점 삽입
	// 각 측점
	long n=0; for(n=0; n<pBri->m_nQtyJigan+1; n++)
	{
		dArraySta.Add(pBri->GetStationOnJijum(n));
	}
	// VIP 점
	for(n=0; n<pLine->m_VIPsu; n++)
	{
		if(dStaBaseStt < pLine->m_VIPData[n][0] && pLine->m_VIPData[n][0] < dStaBaseEnd)
			dArraySta.Add(pLine->m_VIPData[n][0]);		
	}
	// 종단 변화점	
	for(n = 0; n < pLine->m_VEarthLine.GetSize(); n++)	
	{
		if(dStaBaseStt < pLine->m_VEarthLine.GetAt(n)->dSta && pLine->m_VEarthLine.GetAt(n)->dSta < dStaBaseEnd)
			dArraySta.Add(pLine->m_VEarthLine.GetAt(n)->dSta);	
	}
	
	dArraySta.Sort();

	double dPickH = Dom.Always(JONG_TABLE_PICK); // 테이블내의 구분선 "|"
	double dBaseY = ptBase.y;
	char *Title[] = { "측    점", "거    리", "지 반 고", "계 획 고", "구    배" };
	for(n=0; n<6; n++)
	{
		Dom.LineTo(dStaBaseStt-Dom.Always(30), dBaseY-dRowHeight/2+dPickH/2, dStaBaseEnd+Dom.Always(10), dBaseY-dRowHeight/2+dPickH/2);
		if(n!=5)
			Dom.TextOut(dStaBaseStt-Dom.Always(26), dBaseY-dRowHeight/2+dPickH/2 + (dRowHeight-Dom.GetTextHeight(TRUE))/2, Title[n]);
		dBaseY += dRowHeight;
	}	
	Dom.LineTo(dStaBaseStt-Dom.Always(30), ptBase.y-dRowHeight/2+dPickH/2, dStaBaseStt-Dom.Always(30), ptBase.y+dRowHeight*5-dRowHeight/2+dPickH/2);
	Dom.LineTo(dStaBaseStt-Dom.Always(22)+Dom.GetTextWidth(Title[0]), ptBase.y-dRowHeight/2+dPickH/2, dStaBaseStt-Dom.Always(22)+Dom.GetTextWidth(Title[0]), ptBase.y+dRowHeight*5-dRowHeight/2+dPickH/2);
	Dom.LineTo(dStaBaseEnd+Dom.Always(10), ptBase.y-dRowHeight/2+dPickH/2, dStaBaseEnd+Dom.Always(10), ptBase.y+dRowHeight*5-dRowHeight/2+dPickH/2);		

	// VIP 점 표시
	for(n=0; n<pLine->m_VIPsu; n++)
	{
		// 양쪽이 벗어나는 경우
		if(n!=pLine->m_VIPsu-1 && 
			pLine->m_VIPData[n][0] < dStaBaseStt &&
			pLine->m_VIPData[n+1][0] > dStaBaseEnd)
		{
			double dStaPre = pLine->m_VIPData[n][0];
			double dElePre = pLine->m_VIPData[n][1];
			double dStaNxt = pLine->m_VIPData[n+1][0];
			double dEleNxt = pLine->m_VIPData[n+1][1];

			Dom.SetTextAngle(0);
			Dom.SetTextAlignHorzEx(DOM_TEXT_JUST_CENTER);

			if(dEleNxt < dElePre)
				Dom.LineTo(dStaBaseStt, ptBase.y+dRowHeight*5-dRowHeight/2+dPickH/2,
						   dStaBaseEnd, ptBase.y+dRowHeight*4-dRowHeight/2+dPickH/2);
			else
				Dom.LineTo(dStaBaseStt, ptBase.y+dRowHeight*4-dRowHeight/2+dPickH/2,
						   dStaBaseEnd, ptBase.y+dRowHeight*5-dRowHeight/2+dPickH/2);

			CString strText;
			strText.Format("S=%.3f%%", (dEleNxt - dElePre) / (dStaNxt - dStaPre) * 100);
			Dom.TextOut((dStaBaseStt + dStaBaseEnd)/2, ptBase.y+dRowHeight*4+dPickH/2 + Dom.GetTextHeight(), strText);
			strText.Format("L=%.3fm, H=%.3fm", toM(fabs(dStaNxt - dStaPre)), toM(fabs(dEleNxt - dElePre)));
			Dom.TextOut((dStaBaseStt + dStaBaseEnd)/2, ptBase.y+dRowHeight*4+dPickH/2 - Dom.GetTextHeight()*2, strText);			
		}
		if(dStaBaseStt <= pLine->m_VIPData[n][0] && pLine->m_VIPData[n][0] <= dStaBaseEnd)
		{
			Dom.SetTextAngle(90);
			Dom.SetTextAlignHorzEx(DOM_TEXT_JUST_MIDDLE);

			// Circle 과 EL 기입
			CString strEL;
			strEL.Format("%.3f", toM(pLine->m_VIPData[n][1]));
			Dom.Circle(pLine->m_VIPData[n][0], ptBase.y+dRowHeight*4+dPickH/2, dRowHeight/2);	
			Dom.TextOut(pLine->m_VIPData[n][0], ptBase.y+dRowHeight*4-dRowHeight/2+dPickH/2 + (dRowHeight-Dom.GetTextHeight(TRUE))/2, strEL);

			// 선그리기
			// 1. 앞에선
			if(n!=0)
			{
				double dStaPre = pLine->m_VIPData[n-1][0];
				double dElePre = pLine->m_VIPData[n-1][1];
				double dStaNxt = pLine->m_VIPData[n][0];
				double dEleNxt = pLine->m_VIPData[n][1];

				Dom.SetTextAngle(0);
				Dom.SetTextAlignHorzEx(DOM_TEXT_JUST_CENTER);

				if(dStaPre < dStaBaseStt)
				{
					if(dEleNxt < dElePre)
						Dom.LineTo(dStaBaseStt, ptBase.y+dRowHeight*5-dRowHeight/2+dPickH/2,
						           dStaNxt, ptBase.y+dRowHeight*4-dRowHeight/2+dPickH/2);
					else
						Dom.LineTo(dStaBaseStt, ptBase.y+dRowHeight*4-dRowHeight/2+dPickH/2,
						           dStaNxt, ptBase.y+dRowHeight*5-dRowHeight/2+dPickH/2);

					CString strText;
					strText.Format("S=%.3f%%", (dEleNxt - dElePre) / (dStaNxt - dStaPre) * 100);
					Dom.TextOut((dStaBaseStt + dStaNxt)/2, ptBase.y+dRowHeight*4+dPickH/2 + Dom.GetTextHeight(), strText);
					strText.Format("L=%.3fm, H=%.3fm", toM(fabs(dStaNxt - dStaPre)), toM(fabs(dEleNxt - dElePre)));
					Dom.TextOut((dStaBaseStt + dStaNxt)/2, ptBase.y+dRowHeight*4+dPickH/2 - Dom.GetTextHeight()*2, strText);
				}
				else
				{
					if(dEleNxt < dElePre)
						Dom.LineTo(dStaPre, ptBase.y+dRowHeight*5-dRowHeight/2+dPickH/2,
						           dStaNxt, ptBase.y+dRowHeight*4-dRowHeight/2+dPickH/2);
					else
						Dom.LineTo(dStaPre, ptBase.y+dRowHeight*4-dRowHeight/2+dPickH/2,
						           dStaNxt, ptBase.y+dRowHeight*5-dRowHeight/2+dPickH/2);

					CString strText;
					strText.Format("S=%.3f%%", (dEleNxt - dElePre) / (dStaNxt - dStaPre) * 100);
					Dom.TextOut((dStaPre + dStaNxt)/2, ptBase.y+dRowHeight*4+dPickH/2 + Dom.GetTextHeight(), strText);
					strText.Format("L=%.3fm, H=%.3fm", toM(fabs(dStaNxt - dStaPre)), toM(fabs(dEleNxt - dElePre)));
					Dom.TextOut((dStaPre + dStaNxt)/2, ptBase.y+dRowHeight*4+dPickH/2 - Dom.GetTextHeight()*2, strText);
				}
			}
			// 2. 뒤에선
			if(n!=pLine->m_VIPsu-1)
			{
				double dStaPre = pLine->m_VIPData[n][0];
				double dElePre = pLine->m_VIPData[n][1];
				double dStaNxt = pLine->m_VIPData[n+1][0];
				double dEleNxt = pLine->m_VIPData[n+1][1];

				Dom.SetTextAngle(0);
				Dom.SetTextAlignHorzEx(DOM_TEXT_JUST_CENTER);

				if(dStaNxt > dStaBaseEnd)
				{
					if(dEleNxt < dElePre)
						Dom.LineTo(dStaPre, ptBase.y+dRowHeight*5-dRowHeight/2+dPickH/2,
						           dStaBaseEnd, ptBase.y+dRowHeight*4-dRowHeight/2+dPickH/2);
					else
						Dom.LineTo(dStaPre, ptBase.y+dRowHeight*4-dRowHeight/2+dPickH/2,
						           dStaBaseEnd, ptBase.y+dRowHeight*5-dRowHeight/2+dPickH/2);

					CString strText;
					strText.Format("S=%.3f%%", (dEleNxt - dElePre) / (dStaNxt - dStaPre) * 100);
					Dom.TextOut((dStaPre + dStaBaseEnd)/2, ptBase.y+dRowHeight*4+dPickH/2 + Dom.GetTextHeight(), strText);
					strText.Format("L=%.3fm, H=%.3fm", toM(fabs(dStaNxt - dStaPre)), toM(fabs(dEleNxt - dElePre)));
					Dom.TextOut((dStaPre + dStaBaseEnd)/2, ptBase.y+dRowHeight*4+dPickH/2 - Dom.GetTextHeight()*2, strText);
				}
				else
				{
					if(dEleNxt < dElePre)
						Dom.LineTo(dStaPre, ptBase.y+dRowHeight*5-dRowHeight/2+dPickH/2,
						           dStaNxt, ptBase.y+dRowHeight*4-dRowHeight/2+dPickH/2);
					else
						Dom.LineTo(dStaPre, ptBase.y+dRowHeight*4-dRowHeight/2+dPickH/2,
						           dStaNxt, ptBase.y+dRowHeight*5-dRowHeight/2+dPickH/2);

					CString strText;
					strText.Format("S=%.3f%%", (dEleNxt - dElePre) / (dStaNxt - dStaPre) * 100);
					Dom.TextOut((dStaPre + dStaNxt)/2, ptBase.y+dRowHeight*4+dPickH/2 + Dom.GetTextHeight(), strText);
					strText.Format("L=%.3fm, H=%.3fm", toM(fabs(dStaNxt - dStaPre)), toM(fabs(dEleNxt - dElePre)));
					Dom.TextOut((dStaPre + dStaNxt)/2, ptBase.y+dRowHeight*4+dPickH/2 - Dom.GetTextHeight()*2, strText);
				}
			}
		}
	}	

	dBaseY += dRowHeight;

	Dom.SetTextAngle(90);
	Dom.SetTextAlignHorzEx(DOM_TEXT_JUST_MIDDLE);

// 	double dStaCur;	
	CString szFormat;
	for(n=0; n<dArraySta.GetSize(); n++)
	{		
		double dStaCur = dArraySta.GetAt(n);
		if(n!=0 && dStaPre==dStaCur) continue;
		if(n==0)
		{
			szFormat.Format("%.0f+", dStaHead);
			Dom.TextOut(dStaCur-Dom.GetTextHeight(), ptBase.y, szFormat);	
		}

		double dStaHeadCur = (long)dStaCur / 1000000;
		
		if(dStaHeadCur != dStaHead)
		{
			dStaHead = dStaHeadCur;
			szFormat.Format("%.0f+", dStaHead);
			Dom.TextOut(dStaCur-Dom.GetTextHeight(), ptBase.y, szFormat);	
		}

		dBaseY = ptBase.y;
		// 1. 측점
		Dom.LineTo(dStaCur, ptBase.y-dRowHeight/2+dPickH/2, dStaCur, ptBase.y-dRowHeight/2+dPickH);
		szFormat.Format("%.3f", dStaCur/1000-dStaHead*1000);
		Dom.TextOut(dStaCur, ptBase.y, szFormat);	
		Dom.LineTo(dStaCur, dBaseY+dRowHeight/2+dPickH, dStaCur, dBaseY+dRowHeight/2);
		dBaseY += dRowHeight;
		
		// 2. 거리
		if(n==0)
		{			
			Dom.TextOut(dStaCur, ptBase.y+dRowHeight, "20.000");	
		}
		else
		{
			szFormat.Format("%.3f", (dStaCur - dStaPre)/1000);
			Dom.TextOut(dStaCur, ptBase.y+dRowHeight, szFormat);	
		}
		
		Dom.LineTo(dStaCur, dBaseY+dRowHeight/2+dPickH, dStaCur, dBaseY+dRowHeight/2);
		dBaseY += dRowHeight;

		// 3. 지반고
		double dResult;
		if(pLine->GetElevationErathOnStation(dStaCur, dResult))
			szFormat.Format("%.3f", dResult/1000);
		else
			szFormat.Format("0.000");
		Dom.TextOut(dStaCur, ptBase.y+dRowHeight*2, szFormat);	
		
		Dom.LineTo(dStaCur, dBaseY+dRowHeight/2+dPickH, dStaCur, dBaseY+dRowHeight/2);
		dBaseY += dRowHeight;

		// 4. 계획고
		szFormat.Format("%.3f", pLine->GetElevationPlanEl(dStaCur)/1000);
		Dom.TextOut(dStaCur, ptBase.y+dRowHeight*3, szFormat);	
		
		Dom.LineTo(dStaCur, dBaseY+dRowHeight/2+dPickH/2, dStaCur, dBaseY+dRowHeight/2);
		dBaseY += dRowHeight;

		dStaPre = dStaCur;
	}

	//////////////////////////////////////////////////////////////////////////
	// 종단 POLE 그리기

	//double dStationPole  = dStaBaseStt - Dom.Always(22) + Dom.GetTextWidth(Title[0]);
	//double dElevationTop = GetElevationJongDanTop();
	double dElevationBot = GetElevationJongDanBottom();
	//double dElevationPole= 0;
	//double dElevationUnit= 2000; // 2M
	//double dWidthPole    = 400;

	double yMove =  dElevationBot - (dRowHeight*4.5 + dPickH/2 + Dom.Always(2));
/*
	long nCountDraw = (long)((dElevationTop - dElevationBot) / dElevationUnit);

	Dom.SetTextAngle(0);
	Dom.SetTextAlignHorz(TA_LEFT);
	CDPoint xyDraw(dStationPole-Dom.Always(2),0);
	CDPoint xy1(0,0),xy2(0,0),xy3(0,0),xy4(0,0),vX(1,0),vY(0,1);
	long i=0; for(i=0; i<nCountDraw; i++)
	{
		xyDraw.y = ptBase.y + dRowHeight*4.5 + dPickH/2 + Dom.Always(2) + i*dElevationUnit; // 좌측하단점
		dElevationPole = dElevationBot + i*dElevationUnit;
		if(((long)(dElevationPole)%10000)==0)
		{
			szFormat.Format("%.2f",toM(dElevationPole));
			xy1 = xyDraw - dWidthPole*2*vX;
			xy2 = xy1 - (dWidthPole*2 + Dom.GetTextWidth(szFormat))*vX ;
			Dom.LineTo(xy1,xy2);
			Dom.TextOut(xy2 + Dom.GetTextHeight()*0.3,szFormat);
		}
		if((i==nCountDraw-1) && ((long)(dElevationPole+dElevationUnit)%10000)==0)
		{
			szFormat.Format("%.2f",toM(dElevationPole+dElevationUnit));
			xy1 = xyDraw + dElevationUnit*vY - dWidthPole*2*vX;
			xy2 = xy1 - (dWidthPole*2 + Dom.GetTextWidth(szFormat))*vX ;
			Dom.LineTo(xy1,xy2);
			Dom.TextOut(xy2 + Dom.GetTextHeight()*0.3,szFormat);
		}
		if((i%2)==0)
		{
			xy1 = xyDraw;
			xy2 = xy1 - dWidthPole*vX;
			xy3 = xy2 + dElevationUnit*vY;
			xy4 = xy3 + dWidthPole*vX;
			Dom.Solid(xy1.x,xy1.y,xy2.x,xy2.y,xy3.x,xy3.y,xy4.x,xy4.y);
			xy1 = xyDraw - dWidthPole*vX;
			xy2 = xy1 - dWidthPole*vX;
			xy3 = xy2 + dElevationUnit*vY;
			xy4 = xy3 + dWidthPole*vX;
			Dom.Rectangle(xy1,xy3);
		}
		else
		{
			xy1 = xyDraw;
			xy2 = xy1 - dWidthPole*vX;
			xy3 = xy2 + dElevationUnit*vY;
			xy4 = xy3 + dWidthPole*vX;
			Dom.Rectangle(xy1,xy3);
			xy1 = xyDraw - dWidthPole*vX;
			xy2 = xy1 - dWidthPole*vX;
			xy3 = xy2 + dElevationUnit*vY;
			xy4 = xy3 + dWidthPole*vX;
			Dom.Solid(xy1.x,xy1.y,xy2.x,xy2.y,xy3.x,xy3.y,xy4.x,xy4.y);
		}
	}
*/
	Dom.Move(0,yMove);

	*pDomP << Dom;
}

void CDrawLine::DrawJongDan(CDomyun *pDomP)
{
	CRcBridgeRebar *pBri = dynamic_cast<CRcBridgeRebar*>(m_pStd->m_pRcBridge);
	if(pBri == NULL) return;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CLineInfo *pLine = pBri->m_pARcBridgeDataStd->GetLineApp();

	CDomyun Dom(pDomP);

	// 입력된 구간을 벗어난 경우...
	double dStaBaseStt = max(GetStationJongDanStt()-Dom.Always(22)+Dom.GetTextWidth("측    점"),pLine->m_dStationBp);
	double dStaBaseEnd = min(GetStationJongDanEnd()+Dom.Always(10),pLine->GetStationEpLine());

	// 입력된 종단선형을 벗어난 경우.
	double dStaJongSttReal	= pLine->m_VIPData[0][0];
	double dStaJongEndReal	= pLine->m_VIPData[pLine->m_VIPsu-1][0];
	if(dStaBaseStt < dStaJongSttReal || dStaBaseStt > dStaJongEndReal) dStaBaseStt	= dStaJongSttReal;
	if(dStaBaseEnd < dStaJongSttReal || dStaBaseEnd > dStaJongEndReal) dStaBaseEnd	= dStaJongEndReal;
	

	double dSttEL = pLine->GetElevationPlanEl(dStaBaseStt);
	double dEndEL = pLine->GetElevationPlanEl(dStaBaseEnd);
	double dBotEL = GetElevationJongDanBottom();

	//////////////////////////////////////////////////////////////////////////
	// 종단면도
	long nOldTypeJongDanRebar	= pBri->m_nTypeJongdanForRebar;
	pBri->m_nTypeJongdanForRebar	= 2;
	pBri->m_bSettingVertSection	= FALSE;
	pBri->SyncBridge(0, TRUE);
	pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 2);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	m_pStd->m_pDrawVertSection->DrawVertSectionAll(&Dom, TRUE, TRUE, TRUE, FALSE);
	m_pStd->m_pDrawVertSection->DrawVertSectionCenterLine(&Dom, FALSE, TRUE, FALSE);
	
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->m_pDrawVertSection->DimTextVerSectionEL(&Dom,TRUE);
	m_pStd->m_pDrawVertSection->DimVertSectionUpper(&Dom, 1, TRUE, FALSE,TRUE,TRUE);
	if(pBri->IsBoxType())
		m_pStd->m_pDrawVertSection->DimVertSectionLowerSlab(&Dom, 0, FALSE, TRUE,TRUE);
	else
	{
		long nCntFooting = pBri->GetCountFooting();
		long i=0; for(i=0; i<nCntFooting; i++)
		{
			CFootingApp *pFooting = pBri->GetFooting(i);
			if(pFooting)
			{
				m_pStd->m_pDrawExFooting->DimTextFrontEL(&Dom, &pFooting->m_exFooting, FALSE);
			}
		}
	}

	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, 2);
	pBri->m_nTypeJongdanForRebar = nOldTypeJongDanRebar;

	//////////////////////////////////////////////////////////////////////////
	// 계획고
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	pLine->DrawJongDanCurve(&Dom,dStaBaseStt,dStaBaseEnd,1.0,TRUE);

	// 기본 측점
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	for(double dSta=GetStationJongDanStt(); dSta<=GetStationJongDanEnd(); dSta += 20000)
	{
		double EL = pLine->GetElevationPlanEl(dSta);
		Dom.LineTo(dSta,EL,dSta,dBotEL);
	}


	//////////////////////////////////////////////////////////////////////////
	// 지반고
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARS);
	if(pLine->m_VEarthLine.GetSize())
	{			
		double dEleStt, dEleEnd;

		// dStaBasEnd에서 0.1을 빼는 이유는 pLine->GetElevationErathOnStation함수에서 마지막 sta비교할때 < 를 사용하기 때문임.
		dStaBaseStt	= max(pLine->m_VEarthLine.GetAt(0)->dSta, dStaBaseStt);
		dStaBaseEnd	= min(pLine->m_VEarthLine.GetAt(pLine->m_VEarthLine.GetSize()-1)->dSta, dStaBaseEnd) - 0.1;

		
		if(pLine->GetElevationErathOnStation(dStaBaseStt,dEleStt) &&
		   pLine->GetElevationErathOnStation(dStaBaseEnd,dEleEnd))
		{
			Dom.MoveTo(dStaBaseStt, dEleStt);
			long i = 0; for(i = 0; i < pLine->m_VEarthLine.GetSize(); i++)	
			{
				if(pLine->m_VEarthLine.GetAt(i)->dSta < dStaBaseStt) continue;
				if(pLine->m_VEarthLine.GetAt(i)->dSta > dStaBaseEnd) break;
				Dom.LineTo(pLine->m_VEarthLine.GetAt(i)->dSta, pLine->m_VEarthLine.GetAt(i)->dEle);	
			}
			Dom.LineTo(dStaBaseEnd, dEleEnd);
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 지반정보
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SYMB);
	// 전에는 위의 조건(지반선 범위내에 있는것만 출력한다.)이었으나
	// 멀리 떨어져 있어도 계산에서 적용은된다.
	// 따라서 멀리 떨어져 있어도 도면출력할때도 출력이 되어야 한다.
	long nCountJijum	= pBri->GetCountJijum();
	for(long j = 0; j < nCountJijum; j++)
	{
		CBoringDataEx *pBoringEx	= pBri->GetBoringDataExByJijumNumber(j);
		if(!pBoringEx) 
			continue;
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CUTL);
		m_pStd->ApplyDimSetting(pDomP, &Dom);
		DrawBroing(&Dom, pBoringEx, FALSE, FALSE, TRUE);
	}

	
	//////////////////////////////////////////////////////////////////////////
	// 외곽선
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_PAPR);
	Dom.LineTo(dStaBaseStt,dSttEL,dStaBaseStt,dBotEL);
	Dom.LineTo(dStaBaseEnd,dEndEL,dStaBaseEnd,dBotEL);
	Dom.LineTo(dStaBaseStt,dBotEL,dStaBaseEnd,dBotEL);

	*pDomP << Dom;

	// 시종점 표시
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	double dStaStt	= pBri->m_dStationBridgeStt;
	double dStaEnd	= pBri->m_dStationBridgeStt + pBri->m_dLengthBridge;
	m_pStd->m_pDrawVertSection->DimJongDanBridge(&Dom, dStaStt, dStaEnd, 1, TRUE, FALSE);
	double dMove	= Dom.Always(Dom.GetDimVLen()+Dom.GetDimExo()+Dom.GetDimLevelDis()*2);
	Dom.Move(CDPoint(0, dMove));
	*pDomP << Dom;

	// 경사표시
	m_pStd->m_pDrawVertSection->DimVertSectionJongSlope(&Dom);
	*pDomP << Dom;
}

double CDrawLine::GetElevationJongDanBottom()
{
	CRcBridgeRebar *pBri = dynamic_cast<CRcBridgeRebar*>(m_pStd->m_pRcBridge);
	CLineInfo *pLine = pBri->m_pARcBridgeDataStd->GetLineApp();

	double dMaxEL = 0;
	for(long nJijum=0; nJijum<pBri->GetCountJijum(); nJijum++)
	{
		double sta = pBri->GetStationOnJijum(nJijum);
		double dJijumEL = pLine->GetElevationPlanEl(sta);
		if(dJijumEL>dMaxEL)
		{
			dMaxEL = dJijumEL;
		}
	}
	double dElevationBot = frM(RoundDown(toM(dMaxEL - 25000)/2.0,0)*2.0); // 25M -

	return dElevationBot;
}

double CDrawLine::GetStationJongDanStt()
{
	CRcBridgeRebar *pBri = dynamic_cast<CRcBridgeRebar*>(m_pStd->m_pRcBridge);

//	double dStaBaseStt = (long)(pBri->m_dStationBridgeStt - 30000*1)/20000 * 20000;
	double dStaBaseStt = (long)(pBri->m_dStationBridgeStt - 30000*1);

	return dStaBaseStt;
}

double CDrawLine::GetStationJongDanEnd()
{
	CRcBridgeRebar *pBri = dynamic_cast<CRcBridgeRebar*>(m_pStd->m_pRcBridge);

//	double dStaBaseEnd = (long)(pBri->m_dStationBridgeStt + pBri->m_dLengthBridge + 30000*1)/20000 * 20000;
	double dStaBaseEnd = (long)(pBri->m_dStationBridgeStt + pBri->m_dLengthBridge + 30000*1);

	return dStaBaseEnd;
}

// double CDrawLine::GetElevationJongDanTop()
// {
// 	CRcBridgeRebar *pBri = dynamic_cast<CRcBridgeRebar*>(m_pStd->m_pRcBridge);
// 	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
// 	CLineInfo *pLine = pBri->m_pARcBridgeDataStd->GetLineApp();
// 
// 	double dMaxEL = 0;
// 	for(long nJijum=0; nJijum<pBri->GetCountJijum(); nJijum++)
// 	{
// 		double sta = pBri->GetStationOnJijum(nJijum);
// 		double dJijumEL = pLine->GetElevationPlanEl(sta);
// 		if(dJijumEL>dMaxEL)
// 		{
// 			dMaxEL = dJijumEL;
// 		}
// 	}
// 
// 	double dElevationTop = frM(RoundUp(toM(dMaxEL + 10000)/2.0,0)*2.0); // 10M +
// 
// 	return dElevationTop;
// }

void CDrawLine::DimSpanLengthPlan(CDomyun *pDomP, BOOL bSttMark, BOOL bEndMark)
{
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;
	CLineInfoApp* pLineApp = dynamic_cast<CLineInfoApp*>(pBri->GetLineBase());

	CString str = _T("");
	int nSizeDim = pBri->m_arrJijumLen.GetSize();

	CDimLineDomyun DimDom(pDomP, pLineApp);
	m_pStd->m_pARoadOptionStd->SetEnvType(&DimDom, HCAD_DIML);

	CString strDir = _T("TOP");
	DimDom.SetDirection(strDir);
	double dLenBri  = pBri->m_dLengthBridge;
	double dStaStt  = pBri->m_dStationBridgeStt;
	double dDistDim = 0;
// 	double dLenDim  = 0;

	long nDimDan = 0;
	DimDom.DimMoveToOnLine(dStaStt, dDistDim, nDimDan++);
	long i=0; for(i=0; i<nSizeDim; i++)
	{
		double dLenDim = pBri->m_arrJijumLen.GetAt(i);
		DimDom.DimLineToOnLine(dLenDim,COMMA(dLenDim),_T(""));
	}

	// 전체치수
	if(nSizeDim>1)
	{
		DimDom.DimMoveToOnLine(dStaStt, dDistDim, nDimDan++);
		DimDom.DimLineToOnLine(dLenBri,COMMA(dLenBri),_T(""));
	}
	*pDomP << DimDom;

	if(bSttMark)
	{
		CDPoint xyMark(0,0);
		CDPoint vAng = pLineApp->GetAngleAzimuth(dStaStt);
		dDistDim = -DimDom.Always(DimDom.GetDimExo()+DimDom.GetDimVLen()+DimDom.GetDimLevelDis()*(nDimDan-1)+DimDom.GetDimExe());
		xyMark = pLineApp->GetXyLineDis(dStaStt,dDistDim);
		m_pStd->MarkStationJijum(&DimDom, 0, xyMark);
		DimDom.Rotate(xyMark,vAng);
		*pDomP << DimDom;
	}
	if(bEndMark)
	{
		CDPoint xyMark(0,0);
		CDPoint vAng = pLineApp->GetAngleAzimuth(dStaStt+dLenBri);
		dDistDim = -DimDom.Always(DimDom.GetDimExo()+DimDom.GetDimVLen()+DimDom.GetDimLevelDis()*(nDimDan-1)+DimDom.GetDimExe());
		xyMark = pLineApp->GetXyLineDis(dStaStt+dLenBri,dDistDim);
		m_pStd->MarkStationJijum(&DimDom, pBri->m_nQtyJigan, xyMark);
		DimDom.Rotate(xyMark,vAng);
		*pDomP << DimDom;
	}
}
