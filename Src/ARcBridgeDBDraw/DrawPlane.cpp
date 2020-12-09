// DrawPlane.cpp: implementation of the CDrawPlane class.
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

CDrawPlane::CDrawPlane()
{
	m_bApplyCutAreaHunch = TRUE;
	m_nPosBaseDimSupportRebar = 0;
}

CDrawPlane::~CDrawPlane()
{

}

//<summary>
//입력 평면도를 그림
//
//슈 제원이 입력되었다면 슈도 그림
//</summary>
//<param name="pDomP"></param>
// BOOL bUseDrawing : 도면출력용인지..
// BOOL bIncludeWall : 외측벽체를 포함한.. 
void CDrawPlane::DrawPlaneGen(CDomyun* pDomP, double dLenSpare, BOOL bInput, BOOL bGenDom, BOOL bIncludeWall, BOOL bMarkSlabXY, BOOL bMarkJijumXY, BOOL bMarkShoe)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	CDomyun Dom(pDomP);
	
	if(bInput || (!pBri->IsOutWall(TRUE) && !pBri->IsOutWall(FALSE)))
		DrawPlaneSlabSttEnd(&Dom);
	DrawPlaneSlabOnlyOutLine(&Dom);
	DrawPlaneCenterLineEachSlab(&Dom, dLenSpare, bInput, bGenDom);
	if(!bInput && bGenDom)
	{
		DrawPlaneSlabCrossBlock(&Dom);
		DrawPlaneSlabHunch(&Dom);
	}
	if(bInput)
		DrawPlaneSttEndLine(&Dom);
	
	if(pBri->m_bExpJoint[0]==TRUE)
	{
		if(bInput || (!pBri->IsOutWall(TRUE) && !pBri->IsOutWall(FALSE)))
			DrawPlaneSttEndLine(&Dom);
		DrawPlaneShoeSttEnd(&Dom);
	}
	for(long nJ = 0; nJ <= pBri->m_nQtyJigan; nJ++)
	{
		DrawPlanePierLine(&Dom, nJ);
		DrawPlaneShoeLineOnPier(&Dom, nJ);
		DrawPlaneUganLineOnPier(&Dom, nJ);
		if(bMarkSlabXY || bMarkJijumXY)
			MarkPlaneCoordinates(&Dom, nJ, bMarkJijumXY, bMarkSlabXY);
		if(bInput)
			MarkPlaneJijum(&Dom, nJ);
	}
	
	MarkPlaneArrowDirection(&Dom, dLenSpare-3000);
	
	if(bMarkShoe)
		DrawPlaneShoeDirLine(&Dom);

	*pDomP << Dom;
}

void CDrawPlane::DrawPlaneShoeDirLine(CDomyun* pDomP)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	CJijum* pJ   = NULL; 
	CDPoint vAng = CDPoint(0,0);
	
	long nCountShoe  = 0;
	long nCountJijum = pBri->GetCountJijum();
	long nSlab = 0;
	CDomyun Dom(pDomP);

	long nJijumStt=0;
	long nJijumEnd=nCountJijum-1;
		
	for(long nJ=nJijumStt; nJ<=nJijumEnd; nJ++)
	{
		if(pBri->m_nTypeBridge>0 && pBri->m_nTypeBridge<4)	//슬래브가 없는 라멘교 일때(Type=1,2,3)
		{
			if(nJ>nJijumStt && nJ<nJijumEnd) 
				continue;
		}
		pJ = pBri->GetJijum(nJ);
		for(long nLeft=0; nLeft<2; nLeft++) // nLeft 시종점 교량받침열...
		{
			if(nJ==nJijumStt && nLeft==0) continue;
			if(nJ==nJijumEnd && nLeft==1) break;
			nCountShoe = pJ->m_nQtyShoe[nLeft];
			for(long nRow=0; nRow<nCountShoe; nRow++)
			{
				long Dir = pJ->m_nDirShoe[nLeft][nRow];
				CDPoint xyShoe = pBri->GetXyShoe(nJ, nLeft, nRow);			
				vAng = pBri->GetAngleShoe(nJ, nLeft, nRow, Dir, nSlab, nJijumStt, nJijumEnd);
				m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);

				//고정슈 기준
				if (pBri->m_nTypeShoeAngle==2 && pBri->IsExistFixedJijum(nSlab))
				{
					long nJijumFix = 0;
					long nLeftFix  = 0;
					long nRowFix   = 0;
					pBri->GetIndexNearestFixShoe(xyShoe,nJijumFix,nLeftFix,nRowFix,nJijumStt,nJijumEnd);					
					CDPoint xyFixShoe = pBri->GetXyShoe(nJijumFix,nLeftFix,nRowFix);
					Dom.LineTo(xyShoe,xyFixShoe);
				}

				//현 기준
				if (pBri->m_nTypeShoeAngle==3 && pBri->IsExistFixedJijum(nSlab))
				{
					long nJijumFix = 0;
					long nLeftFix  = 0;
					long nRowFix   = 0;
					pBri->GetIndexNearestFixShoe(xyShoe,nJijumFix,nLeftFix,nRowFix,nJijumStt,nJijumEnd);
					CDPoint xyShoeTarget(0,0);
					if(nJijumFix==nJ)
					{
						xyShoeTarget = pBri->GetXyShoe(nJijumFix,nLeftFix,nRowFix);
					}
					else
					{
						CJijum* pJ = pBri->GetJijum(nJijumFix);
						long nQtyJijumShoe = pJ->m_nQtyShoe[nLeftFix];
						long nRowJijumShoe = min(nQtyJijumShoe-1,nRow);
						xyShoeTarget = pBri->GetXyShoe(nJijumFix,nLeftFix,nRowJijumShoe);
					}
					Dom.LineTo(xyShoe,xyShoeTarget);
				}
					
				//shoe symbol
				m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SYMB);
				DrawPlaneShoeSymbol(&Dom, Dir, xyShoe.x, xyShoe.y, vAng);
					
				//shoe번호쓰기
				CDomyun TextDom(pDomP);
				m_pStd->m_pARoadOptionStd->SetEnvType(&TextDom, HCAD_BARX);
					
				CLineInfo *pLine    = pBri->GetLineBase();
				double	  staJijum  = pBri->GetStationShoeOnJijum(nJ, 0, nLeft);	//도로중심의 슈 스테이션
				CDPoint	  vAngJijum = pBri->GetAngleShoeOnJijum(nJ, 0, nLeft);		//도로중심의 슈 위치 지점각도
				double	  dDistShoe = pJ->m_dDistShoe[nLeft][nRow];
				double	  staShoe   = pLine->GetStationByScInLine(staJijum, dDistShoe, vAngJijum);
				CDPoint   vTextAng  = pLine->GetAngleAzimuth(staShoe);
					
				CString str;
				str.Format("%d", nRow+1);
					
				if(nLeft)
					TextDom.TextCircle(xyShoe.x+1000, xyShoe.y-10, str, 3, FALSE, FALSE);
				else
					TextDom.TextCircle(xyShoe.x-1000, xyShoe.y-10, str, 3, FALSE, FALSE);
					
				TextDom.Rotate(xyShoe,vTextAng);
					
				Dom << TextDom;
			}
		}
	}
	
	*pDomP << Dom;
}

//<summary>
//도로중심 선형, 슬래브 좌우측 선형의 중심선을 그림(확폭이 아닐경우 도로중심 선형만 그림)
//</summary>
//<param name="*pDomP"></param>
//<param name="bGirder"></param>
// BOOL bUseDrawing : 도면 출력용인지..
void CDrawPlane::DrawPlaneCenterLineEachSlab(CDomyun *pDomP, double dLenSpare, BOOL bInput, BOOL bGenDom)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);

	CLineInfo* pLine = NULL;

	if(bGenDom || bInput)
	{
		// 선형 중심선 그림
		for(int nLeft = -1 ; nLeft < 2; nLeft++)
		{
			pLine = pBri->GetLineBySlab(nLeft);
			if(nLeft != 0 && pLine == pBri->GetLineBase())
				continue;

			double staStt = pBri->GetStationAtSlabEnd(TRUE, nLeft) - dLenSpare;
			double staEnd = pBri->GetStationAtSlabEnd(FALSE, nLeft) + dLenSpare;
			
			CDomyun DomText(pDomP);

			m_pStd->m_pARoadOptionStd->SetEnvType(&DomText, HCAD_TXTG);
			CString szLineName = pLine->GetNameLineEach();
			CDPoint xy = pLine->GetXyLineDis(staStt, -DomText.GetTextHeight()*1.1);
			DomText.SetTextAlignHorz(TA_LEFT);
			DomText.TextOut(xy, szLineName);
			DomText.Rotate(xy, pLine->GetAngleAzimuth(staStt));
			*pDomP << DomText;

			if(nLeft==0)
			{
				CDPoint xy = pLine->GetXyLineDis(staStt-DomText.Always(2),0);
				m_pStd->MarkCenterOfRoad(&DomText,xy);
				DomText.Rotate(xy, pLine->GetAngleAzimuth(staStt).Rotate90());
				*pDomP << DomText;
			}

			pLine->DrawLS(&Dom, staStt, staEnd, 0);
		}
	}
	else
	{
		CRcBridgeRebar *pBriRebar = m_pStd->m_pRcBridgeRebar;
		// 구조도 용이란 말이지?
		CHgBaseDrawStd baseDraw;
		CTwinVectorArray tvArrCenter;
		pBri->GetTvCenterPlane_UpperSlab(tvArrCenter, TRUE, TRUE, dLenSpare, pBriRebar->IsVertDir());
		baseDraw.DrawTvArray(&Dom, tvArrCenter);
		*pDomP << Dom;

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SYMB);

		pLine	= pBri->GetLineBase();
		double dSta1(0), dSta2(0);
		CTwinVector tvCen	= tvArrCenter.GetAt(0);
		pLine->GetStationMatchVerticalByXy(tvCen.m_v1, dSta1);
		pLine->GetStationMatchVerticalByXy(tvCen.m_v2, dSta2);

		double dStaOrg	= min(dSta1, dSta2);
		double dDist	= pLine->GetDisMatchVerticalByXy(dSta1<dSta2 ? tvCen.m_v1 :  tvCen.m_v2);
		CDPoint xyOrg	= pLine->GetXyLineDis(dStaOrg+dLenSpare/2, dDist);
		CDPoint xyDirJijum	= pLine->GetAngleAzimuth(dStaOrg).Rotate90();

		CConcSymbol sym;
		sym.ArrowSide(&Dom, "하 면", "상 면", xyOrg);
		Dom.Rotate(xyOrg, xyDirJijum);
	}

	*pDomP << Dom;
}

//<summary>
//교각선 그리기, 교대선은 그리지 않음.
//</summary>
//<param name="*pDomP"></param>
//<param name="nPier"></param>
void CDrawPlane::DrawPlanePierLine(CDomyun *pDomP, long nJ)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(nJ <= 0 || nJ >= pBri->m_nQtyJigan)	return;	//시종점 교대는 그리지 않음.

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);

	CTwinVector tv = pBri->GetTvJijum(nJ, 0);
	Dom.LineTo(tv.m_v1, tv.m_v2);

	*pDomP << Dom;
}

//<summary>
//좌우측 슬래브 끝단 그리기
//</summary>
//<param name="*pDomP"></param>
void CDrawPlane::DrawPlaneSlabOnlyOutLine(CDomyun *pDomP, BOOL bForBM, CString str)
{	
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	CLineInfo *pLineL = pBri->GetLineLeftRight(TRUE);
	CLineInfo *pLineR = pBri->GetLineLeftRight(FALSE);
	CLineInfoApp* pLineApp = static_cast<CLineInfoApp*>(pBri->GetLineBase());

	if(bForBM)
	{
		CDimLineDomyun DimDom(pDomP, pLineApp);
		m_pStd->m_pARoadOptionStd->SetEnvType(&DimDom, HCAD_DIML);
		m_pStd->ApplyDimSetting(pDomP, &DimDom);
		double dScale = DimDom.GetScaleDim();
		DimDom.SetScaleDim(dScale*2);

		CString strDir = _T("TOP");
		DimDom.SetDirection(strDir);
		double	dLenBri  = pBri->m_dLengthBridge;
		double	dStaStt  = pBri->m_dStationBridgeStt;
		long	nSizeDim = pBri->m_arrJijumLen.GetSize();
		long	dSttGap  = nSizeDim > 0 ? (long)pBri->m_arrJijumLen.GetAt(0) : 0;
		long	dEndGap  = nSizeDim > 0 ? (long)pBri->m_arrJijumLen.GetAt(nSizeDim-1) : 0;

		double	dDistDim = 0;
		DimDom.DimMoveToOnLine(dStaStt, dDistDim);
		DimDom.DimLineToOnLine(dSttGap, COMMA(dSttGap), _T(""));
		dDistDim = dLenBri-dSttGap-dEndGap;
		DimDom.DimLineToOnLine(dDistDim, COMMA(dDistDim), str);
		DimDom.DimLineToOnLine(dEndGap, COMMA(dEndGap), _T(""));
		*pDomP << DimDom;
	}

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	CTwinVectorArray tvArrPlane;
	pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1, TRUE);

	CTwinVectorArray tvArrLeft, tvArrRight;
	CTwinVector tv;
	long i = 0; for(i = 0; i < tvArrPlane.GetSize(); i++)
	{
		tv	= tvArrPlane.GetAt(i);
		if(tv.m_sInfo.Find("좌측", 0) != -1) tvArrLeft.Add(tv);
		else if(tv.m_sInfo.Find("우측", 0) != -1) tvArrRight.Add(tv);
	}

	
	CHgBaseDrawStd baseDraw;
	baseDraw.DrawTvArray(&Dom, tvArrLeft, m_pStd->IsViewVectorInfo());
	baseDraw.DrawTvArray(&Dom, tvArrRight, m_pStd->IsViewVectorInfo());

	*pDomP << Dom;
	return;


	CDPoint slabA[4],EndA[4];
	pBri->GetXySss(BRIDGE_OFF_STT_SLAB,slabA);
	pBri->GetXySss(BRIDGE_OFF_END_SLAB,EndA);

	// 슬래브 좌측라인
	BOOL bSttGagak = (pBri->m_dExpSttW[0]>0 && pBri->m_dExpSttH[0] && pBri->m_bIsExp) ? TRUE : FALSE;
	BOOL bEndGagak = (pBri->m_dExpEndW[0]>0 && pBri->m_dExpEndH[0] && pBri->m_bIsExp) ? TRUE : FALSE;
	double staStt = pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB,-1,FALSE) + (bSttGagak ? pBri->m_dExpSttW[0] : 0);
	double staEnd = pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB,-1,FALSE) - (bEndGagak ? pBri->m_dExpEndW[0] : 0);
	CDPoint xyW = pLineL->GetXyLineDis(staStt,pBri->m_dWidthSlabLeft);
	CDPoint xySlabSttEnd = slabA[0];
	CDPoint xySegEnd1 = pLineL->GetXyLineDis(staEnd,pBri->m_dWidthSlabLeft);
	CDPoint xySegEnd2 = EndA[0];
	CDPoint xyCenStt(0,0),xySegStt(0,0),xyCenEnd(0,0),xySegEnd(0,0);
	if(bSttGagak)
		pLineL->GetXyForRc(pBri->m_dWidthSlabLeft,pBri->m_dExpSttR[0],xyW,xySlabSttEnd,TRUE ,TRUE, staStt,xyCenStt,xySegStt);
	if(bEndGagak)
		pLineL->GetXyForRc(pBri->m_dWidthSlabLeft,pBri->m_dExpEndR[0],xySegEnd1,xySegEnd2,FALSE,TRUE, staEnd,xyCenEnd,xySegEnd);
	//
	pLineL->DrawLS(&Dom, staStt, staEnd, pBri->m_dWidthSlabLeft);
/*
/////////////////////////  가각 Match 좌표 점검용 코드
	CDPoint xyMatchTemp = pBri->GetXySlabEdgeActByGagak(pBri->GetStationOnJijum(0)+pBri->m_dExpSttW[0]/2, pBri->GetAngleJijum(0), TRUE);
	CDPoint xyLineTemp = pBri->GetLineBase()->GetXyLine(pBri->GetStationOnJijum(0));
	Dom.LineTo(xyMatchTemp, xyLineTemp);
	xyMatchTemp = pBri->GetXySlabEdgeActByGagak(pBri->GetStationOnJijum(0)+pBri->m_dExpSttW[0]/2, pBri->GetAngleJijum(0), FALSE);
	xyLineTemp = pBri->GetLineBase()->GetXyLine(pBri->GetStationOnJijum(0));
	Dom.LineTo(xyMatchTemp, xyLineTemp);
	xyMatchTemp = pBri->GetXySlabEdgeActByGagak(pBri->GetStationOnJijum(1)-pBri->m_dExpEndW[0]/2, pBri->GetAngleJijum(1), TRUE);
	xyLineTemp = pBri->GetLineBase()->GetXyLine(pBri->GetStationOnJijum(1));
	Dom.LineTo(xyMatchTemp, xyLineTemp);
	xyMatchTemp = pBri->GetXySlabEdgeActByGagak(pBri->GetStationOnJijum(1)-pBri->m_dExpEndW[0]/2, pBri->GetAngleJijum(1), FALSE);
	xyLineTemp = pBri->GetLineBase()->GetXyLine(pBri->GetStationOnJijum(1));
	Dom.LineTo(xyMatchTemp, xyLineTemp);
	double disLeftTemp = pBri->GetDisSlabLeftActByGagak(pBri->GetStationOnJijum(0)+pBri->m_dExpSttW[0]/2, pBri->GetAngleJijum(0));
	double disRighTemp = pBri->GetDisSlabRighActByGagak(pBri->GetStationOnJijum(0)+pBri->m_dExpSttW[0]/2, pBri->GetAngleJijum(0));
	double widthTemp = pBri->GetWidthSlabActByGagak(pBri->GetStationOnJijum(0)+pBri->m_dExpSttW[0]/2, pBri->GetAngleJijum(0));
	double disLeftTemp1 = pBri->GetDisSlabLeftActByGagak(pBri->GetStationOnJijum(0), pBri->GetAngleJijum(0));
	double disRighTemp1 = pBri->GetDisSlabRighActByGagak(pBri->GetStationOnJijum(0), pBri->GetAngleJijum(0));
	double widthTemp1 = pBri->GetWidthSlabActByGagak(pBri->GetStationOnJijum(0), pBri->GetAngleJijum(0));
	double disLeftTemp2 = pBri->GetDisSlabLeftActByGagak(pBri->GetStationOnJijum(0)+pBri->m_dExpSttW[0]*2, pBri->GetAngleJijum(0));
	double disRighTemp2 = pBri->GetDisSlabRighActByGagak(pBri->GetStationOnJijum(0)+pBri->m_dExpSttW[0]*2, pBri->GetAngleJijum(0));
	double widthTemp2 = pBri->GetWidthSlabActByGagak(pBri->GetStationOnJijum(0)+pBri->m_dExpSttW[0]*2, pBri->GetAngleJijum(0));
////////////////////////////////////////////////////////
*/

	if(bSttGagak)
	{
		Dom.LineTo(xySegStt,xySlabSttEnd);
		if(pBri->m_dExpSttR[0]>0)
		{
			CDPoint xyArcEnd = pLineL->GetXyLineDis(staStt,pBri->m_dWidthSlabLeft);
			double dAngStt = (xySegStt-xyCenStt).Unit().GetAngleDegree();
			double dAngEnd = (xyArcEnd-xyCenStt).Unit().GetAngleDegree();
			if(dAngEnd<dAngStt)
				dAngEnd += 360;
			Dom.Arc(xyCenStt,pBri->m_dExpSttR[0],dAngStt,dAngEnd-dAngStt);
		}
	}
	if(bEndGagak)
	{
		Dom.LineTo(xySegEnd,xySegEnd2);
		if(pBri->m_dExpEndR[0]>0)
		{
			CDPoint xyArcStt = pLineL->GetXyLineDis(staEnd,pBri->m_dWidthSlabLeft);
			double dAngStt = (xyArcStt-xyCenEnd).Unit().GetAngleDegree();
			double dAngEnd = (xySegEnd-xyCenEnd).Unit().GetAngleDegree();
			if(dAngEnd<dAngStt)
				dAngEnd += 360;
			Dom.Arc(xyCenEnd,pBri->m_dExpEndR[0],dAngStt,dAngEnd-dAngStt);
		}
	}

	// 슬래브 우측라인
	bSttGagak = (pBri->m_dExpSttW[1]>0 && pBri->m_dExpSttH[1] && pBri->m_bIsExp) ? TRUE : FALSE;
	bEndGagak = (pBri->m_dExpEndW[1]>0 && pBri->m_dExpEndH[1] && pBri->m_bIsExp) ? TRUE : FALSE;
	staStt = pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB,1,FALSE) + (bSttGagak ? pBri->m_dExpSttW[1] : 0);
	staEnd = pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB,1,FALSE) - (bEndGagak ? pBri->m_dExpEndW[1] : 0);
	xyW = pLineR->GetXyLineDis(staStt,pBri->m_dWidthSlabRight);
	xySlabSttEnd = slabA[3];
	xySegEnd1 = pLineR->GetXyLineDis(staEnd,pBri->m_dWidthSlabRight);
	xySegEnd2 = EndA[3];
	if(bSttGagak)
		pLineR->GetXyForRc(pBri->m_dWidthSlabRight,pBri->m_dExpSttR[1],xyW,xySlabSttEnd,TRUE ,FALSE, staStt,xyCenStt,xySegStt);
	if(bEndGagak)
		pLineR->GetXyForRc(pBri->m_dWidthSlabRight,pBri->m_dExpEndR[1],xySegEnd1,xySegEnd2,FALSE,FALSE, staEnd,xyCenEnd,xySegEnd);
	//
	pLineR->DrawLS(&Dom, staStt, staEnd, pBri->m_dWidthSlabRight);
	if(bSttGagak)
	{
		Dom.LineTo(xySegStt,xySlabSttEnd);
		if(pBri->m_dExpSttR[1]>0)
		{
			CDPoint xyArcEnd = pLineR->GetXyLineDis(staStt,pBri->m_dWidthSlabRight);
			double dAngStt = (xySegStt-xyCenStt).Unit().GetAngleDegree();
			double dAngEnd = (xyArcEnd-xyCenStt).Unit().GetAngleDegree();
			Dom.Arc(xyCenStt,pBri->m_dExpSttR[1],dAngStt,dAngEnd-dAngStt);
		}
	}
	if(bEndGagak)
	{
		Dom.LineTo(xySegEnd,xySegEnd2);
		if(pBri->m_dExpEndR[1]>0)
		{
			CDPoint xyArcStt = pLineR->GetXyLineDis(staEnd,pBri->m_dWidthSlabRight);
			double dAngStt = (xyArcStt-xyCenEnd).Unit().GetAngleDegree();
			double dAngEnd = (xySegEnd-xyCenEnd).Unit().GetAngleDegree();
			Dom.Arc(xyCenEnd,pBri->m_dExpEndR[1],dAngStt,dAngEnd-dAngStt);
		}
	}

	*pDomP << Dom;
}

//<summary>
//시종점 교대 선 그리기
//</summary>
//<param name="*pDomP"></param>
void CDrawPlane::DrawPlaneSttEndLine(CDomyun *pDomP)
{	
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);


	CHgBaseDrawStd baseDraw;
	CTwinVectorArray tvArr;

	pBri->GetTvPlaneSttEndLine(tvArr, TRUE, FALSE);
	baseDraw.DrawTvArray(&Dom, tvArr, FALSE, FALSE);

	pBri->GetTvPlaneSttEndLine(tvArr, FALSE, FALSE);
	baseDraw.DrawTvArray(&Dom, tvArr, FALSE, FALSE);


	/* 위 내용으로 재호가 수정
	CDPoint A[4];
	pBri->GetXySss(BRIDGE_OFF_STT_STT, A);
	Dom.MoveTo(A[0]);
	long n = 0; for(n = 1; n < 4; n++)
		Dom.LineTo(A[n]);

	pBri->GetXySss(BRIDGE_OFF_END_END, A);
	Dom.MoveTo(A[0]);
	for(n = 1; n < 4; n++)
		Dom.LineTo(A[n]);
	*/

	*pDomP << Dom;
}

//<summary>
//시종점 슬래브 교축 직각방향 직선 그리기
//</summary>
//<param name="*pDomP"></param>
void CDrawPlane::DrawPlaneSlabSttEnd(CDomyun *pDomP)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	CHgBaseDrawStd baseDraw;
	CTwinVectorArray tvArrStt, tvArrEnd;
	CTwinVectorArray tvArrPlane;
	pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1, TRUE);
	CTwinVector tv;
	long i = 0; for(i = 0; i < tvArrPlane.GetSize(); i++)
	{
		tv	= tvArrPlane.GetAt(i);
		if(tv.m_sInfo == "시점") tvArrStt.Add(tv);
		else if(tv.m_sInfo == "종점") tvArrEnd.Add(tv);
	}


	//pBri->GetTvPlaneSttEndLine(tvArr, TRUE, TRUE);
	baseDraw.DrawTvArray(&Dom, tvArrStt, FALSE, FALSE);

	//pBri->GetTvPlaneSttEndLine(tvArr, FALSE, TRUE);
	baseDraw.DrawTvArray(&Dom, tvArrEnd, FALSE, FALSE);

	/* 재호가 수정
	CDPoint A[4];
	pBri->GetXySss(BRIDGE_OFF_STT_SLAB, A);
	Dom.MoveTo(A[0]);
	long n = 0; for(n = 1; n < 4; n++)
		Dom.LineTo(A[n]);

	pBri->GetXySss(BRIDGE_OFF_END_SLAB, A);
	Dom.MoveTo(A[0]);
	for(n = 1; n < 4; n++)
		Dom.LineTo(A[n]);
	*/

	*pDomP << Dom;
}

//<summary>
//시종점 슈 직선 그리기
//</summary>
//<param name="*pDomP"></param>
void CDrawPlane::DrawPlaneShoeSttEnd(CDomyun *pDomP)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);


	CDPoint A[4];
	pBri->GetXySss(BRIDGE_OFF_STT_SHOE, A);
	Dom.MoveTo(A[0]);
	long n = 0; for(n = 1; n < 4; n++)
		Dom.LineTo(A[n]);

	pBri->GetXySss(BRIDGE_OFF_END_SHOE, A);
	Dom.MoveTo(A[0]);
	for(n = 1; n < 4; n++)
		Dom.LineTo(A[n]);

	*pDomP << Dom;
}

void CDrawPlane::DrawPlaneShoeSymbol(CDomyun* pDomP, long nType, double BX, double BY, CDPoint vAng, double ScaleFactor)
{
	if(nType<0 || nType>3)	return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_RCON);

	double radius = 200*ScaleFactor;	// 원의 반지름
	double Length = 500*ScaleFactor;	// 원의 중심 ~ 삼각형의 꼭지점
	double triLen = 150*ScaleFactor;	// 삼각형의 높이
	
	//if(nType==0) radius *= 1.50;
	Dom.Circle(0, 0, radius, !nType);
	if(!nType) Dom.Circle(0, 0, radius);

	if(nType==3 || nType==2) // 상하로 그리기
	{
		Dom.LineTo(0, -Length, 0, Length);
		Dom.Solid(0, Length, -triLen/3, Length-triLen, triLen/3, Length-triLen, 0, Length);
		Dom.Solid(0, -Length, -triLen/3, -Length+triLen, triLen/3, -Length+triLen, 0, -Length);
	}

	if(nType==3 || nType==1) // 좌우로 그리기
	{
		Dom.LineTo(-Length, 0, Length, 0);
		Dom.Solid(-Length, 0, -Length+triLen, -triLen/3, -Length+triLen, triLen/3, -Length, 0);
		Dom.Solid(Length, 0, Length-triLen, -triLen/3, Length-triLen, triLen/3,  Length, 0);
	}

	Dom.Rotate(CDPoint(0,0), vAng);
	Dom.Move(BX, BY);

	*pDomP << Dom;
}

//<summary>
//중앙 교각부의 슈 직선 그리기
//</summary>
//<param name="*pDomP"></param>
void CDrawPlane::DrawPlaneShoeLineOnPier(CDomyun *pDomP, long nJ)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(nJ <= 0 || nJ >= pBri->m_nQtyJigan)	return;	//시종점 교대는 그리지 않음.

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);

	CTwinVector tv;
	CJijum *pJ = pBri->GetJijum(nJ);
	double dLenMove = pJ->m_dLengthUgan[0] + pJ->m_dLengthSlabToShoe[0];
	if(dLenMove > 0)
	{
		tv = pBri->GetTvJijum(nJ, -dLenMove);
		Dom.LineTo(tv.m_v1, tv.m_v2);
	}
	dLenMove = pJ->m_dLengthUgan[1] + pJ->m_dLengthSlabToShoe[1];
	if(dLenMove > 0)
	{
		tv = pBri->GetTvJijum(nJ, dLenMove);
		Dom.LineTo(tv.m_v1, tv.m_v2);
	}

	///슈 좌표 테스트
	CDPoint xyShoe = pBri->GetXyShoe(nJ, 0, 0);

	*pDomP << Dom;
}

//<summary>
//중앙 교각부의 유간 직선 그리기
//</summary>
//<param name="*pDomP"></param>
void CDrawPlane::DrawPlaneUganLineOnPier(CDomyun *pDomP, long nJ)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(nJ <= 0 || nJ >= pBri->m_nQtyJigan)	return;	//시종점 교대는 그리지 않음.

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);

	CTwinVector tv;
	CJijum *pJ = pBri->GetJijum(nJ);
	double dLenMove = pJ->m_dLengthUgan[0];
	if(dLenMove > 0)
	{
		tv = pBri->GetTvJijum(nJ, -dLenMove);
		Dom.LineTo(tv.m_v1, tv.m_v2);
	}
	dLenMove = pJ->m_dLengthUgan[1];
	if(dLenMove > 0)
	{
		tv = pBri->GetTvJijum(nJ, dLenMove);
		Dom.LineTo(tv.m_v1, tv.m_v2);
	}

	*pDomP << Dom;
}

void CDrawPlane::MarkPlaneJijum(CDomyun *pDomP, long nJ)
{
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;
	if(nJ < 0 || nJ > pBri->m_nQtyJigan) return;
	CLineInfoApp *pLine = static_cast<CLineInfoApp*>(pBri->GetLineLeftRight(TRUE));

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	long   nSlabLeft = -1;
	double dStaLeft  = pBri->GetStationOnJijum(nJ,nSlabLeft);
	double TextHeight= Dom.GetTextHeight();
	CDPoint xyMark   = pLine->GetXyLineDis(dStaLeft,pBri->m_dWidthSlabLeft-TextHeight*4);
	CDPoint vRotate  = pLine->GetAngleAzimuth(dStaLeft);

	CString szMark = "A1";
	if(nJ==pBri->m_nQtyJigan) szMark = "A2";
	if(nJ>0 && nJ<pBri->m_nQtyJigan) szMark.Format("P%d",nJ);
	Dom.TextCircle(xyMark.x,xyMark.y,szMark,3);
	Dom.Rotate(xyMark,vRotate);

	*pDomP << Dom;
}

void CDrawPlane::MarkPlaneCoordinates(CDomyun *pDomP, long nJ, BOOL bMarkJijum, BOOL bMarkSlabSttEnd)
{
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;

	int nJijum = pBri->GetCountJijum();
	CDPoint A[4], B[4], xy1(0, 0), xy2(0, 0);
	CString strX = _T(""), strY = _T("");
	CDPoint vAngL = pBri->GetAngleAzimuthTanOnJijum(nJ,-1);
	CDPoint vAngC = pBri->GetAngleAzimuthTanOnJijum(nJ, 0);
	CDPoint vAngR = pBri->GetAngleAzimuthTanOnJijum(nJ, 1);
	double dSta = pBri->GetStationOnJijum(nJ, 0);
	CLineInfo* pLine = pBri->GetLineBase();
	
	CConcSymbol sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	double dLenGisi = Dom.Always(14);

	if(nJ==0 && (bMarkJijum || bMarkSlabSttEnd))
	{
		long nDirLeft  = !pBri->IsOutWall(TRUE) ? -1 : 1;
		long nDirMark2 = 0;
		pBri->GetXySss(BRIDGE_OFF_STT_STT, A);
		pBri->GetXySss(BRIDGE_OFF_STT_SLAB, B);

		if(pBri->IsGagakForLine(TRUE, TRUE) && pBri->IsCutEndOfExp(TRUE, TRUE))
		{
			B[0] = A[0] - pBri->GetThickWall(0, TRUE) * pBri->GetAngleAzimuthTanOnJijum(nJ, 0);
		}
		if(pBri->IsGagakForLine(TRUE, FALSE) && pBri->IsCutEndOfExp(TRUE, FALSE))
		{
			B[3] = A[3] - pBri->GetThickWall(0, TRUE) * pBri->GetAngleAzimuthTanOnJijum(nJ, 0);
		}

		if((bMarkJijum && bMarkSlabSttEnd)==FALSE) nDirLeft = -1; // 둘중에 하나만 표기하는 경우
		if(bMarkJijum)
		{
			long nDirMark1 = (bMarkJijum && bMarkSlabSttEnd && pBri->IsOutWall(TRUE)) ? 1 : 3;
			nDirMark2 = (bMarkJijum && bMarkSlabSttEnd && pBri->IsOutWall(TRUE)) ? 7 : 5;

			// 교량 시점(좌)
			strX.Format(_T(" X : %.6f "), toM(A[0].y));
			strY.Format(_T(" Y : %.6f "), toM(A[0].x));
			sym.GiSiArrowExtendAng(&Dom, A[0].x, A[0].y, dLenGisi, nDirLeft, nDirMark1, strX, strY, vAngL);
			// 교량 시점(우)
			strX.Format(_T(" X : %.6f "), toM(A[3].y));
			strY.Format(_T(" Y : %.6f "), toM(A[3].x));
			sym.GiSiArrowExtendAng(&Dom, A[3].x, A[3].y, dLenGisi, nDirLeft, nDirMark2, strX, strY, vAngR);
			// 교량 시점 : 도로중심
			nDirMark1 = (pBri->m_dWidthSlabRight<0) ? (pBri->IsOutWall(TRUE) ? 1 : 3) : (pBri->IsOutWall(TRUE) ? 7 : 5);
			nDirMark2 = (pBri->m_dWidthSlabRight<0) ? (pBri->IsOutWall(TRUE) ? 3 : 1) : (pBri->IsOutWall(TRUE) ? 5 : 7);
			
			if((bMarkJijum && bMarkSlabSttEnd)==FALSE) 
			{
				nDirMark1 = (pBri->m_dWidthSlabRight<0) ? 3 : 5;
			}
			xy1 = pLine->GetXyLine(dSta);
			strX.Format(_T(" X : %.6f "), toM(xy1.y));
			strY.Format(_T(" Y : %.6f "), toM(xy1.x));
			sym.GiSiArrowExtendAng(&Dom, xy1.x, xy1.y, dLenGisi, nDirLeft, nDirMark1, strX, strY, vAngC);
		}
		if(bMarkSlabSttEnd)
		{
			// 슬래브 시점(좌)
			if(A[0]!=B[0])
			{
				strX.Format(_T(" X : %.6f "), toM(B[0].y));
				strY.Format(_T(" Y : %.6f "), toM(B[0].x));
				sym.GiSiArrowExtendAng(&Dom, B[0].x, B[0].y, dLenGisi, -nDirLeft, pBri->IsOutWall(TRUE) ? 3 : 1, strX, strY, vAngL);
			}
			// 슬래브 시점(우)
			if(A[3]!=B[3])
			{
				strX.Format(_T(" X : %.6f "), toM(B[3].y));
				strY.Format(_T(" Y : %.6f "), toM(B[3].x));
				sym.GiSiArrowExtendAng(&Dom, B[3].x, B[3].y, dLenGisi, -nDirLeft, pBri->IsOutWall(TRUE) ? 5 : 7, strX, strY, vAngR);
			}

			// 슬래브 시점 : 도로중심
			double dLenUgan = pBri->GetLengthUgan(nJ, 1);//pJ->m_dLengthUgan[1];
			xy2 = pLine->GetXyLine(dSta+dLenUgan);
			if(xy1!=xy2)
			{
				strX.Format(_T(" X : %.6f "), toM(xy2.y));
				strY.Format(_T(" Y : %.6f "), toM(xy2.x));
				sym.GiSiArrowExtendAng(&Dom, xy2.x, xy2.y, dLenGisi, -nDirLeft, nDirMark2, strX, strY, vAngC);
			}
		}
	}
	else if(nJ==nJijum-1)
	{
		long nDirLeft  = !pBri->IsOutWall(FALSE)  ? 1 : -1;
		long nDirMark2 = 0;
		pBri->GetXySss(BRIDGE_OFF_END_END, A);
		pBri->GetXySss(BRIDGE_OFF_END_SLAB, B);

		if(pBri->IsGagakForLine(FALSE, TRUE) && pBri->IsCutEndOfExp(FALSE, TRUE))
		{
			B[0] = A[0] + pBri->GetThickWall(nJ, TRUE) * pBri->GetAngleAzimuthTanOnJijum(nJ, 0);
		}
		if(pBri->IsGagakForLine(FALSE, FALSE) && pBri->IsCutEndOfExp(FALSE, FALSE))
		{
			B[3] = A[3] + pBri->GetThickWall(nJ, TRUE) * pBri->GetAngleAzimuthTanOnJijum(nJ, 0);
		}

		if((bMarkJijum && bMarkSlabSttEnd)==FALSE) nDirLeft = 1;
		if(bMarkJijum)
		{
			long nDirMark1 = (bMarkJijum && bMarkSlabSttEnd && pBri->IsOutWall(FALSE)) ? 3 : 1;
			nDirMark2 = (bMarkJijum && bMarkSlabSttEnd && pBri->IsOutWall(FALSE)) ? 5 : 7;
			// 교량 종점(좌)
			strX.Format(_T(" X : %.6f "), toM(A[0].y));
			strY.Format(_T(" Y : %.6f "), toM(A[0].x));
			sym.GiSiArrowExtendAng(&Dom, A[0].x, A[0].y, dLenGisi, nDirLeft, nDirMark1, strX, strY, vAngL);
			// 교량 종점(우)
			strX.Format(_T(" X : %.6f "), toM(A[3].y));
			strY.Format(_T(" Y : %.6f "), toM(A[3].x));
			sym.GiSiArrowExtendAng(&Dom, A[3].x, A[3].y, dLenGisi, nDirLeft, nDirMark2, strX, strY, vAngR);
			// 교량 종점 : 도로중심
			nDirMark1 = (pBri->m_dWidthSlabRight<0) ? (pBri->IsOutWall(FALSE) ? 3 : 5) : (pBri->IsOutWall(FALSE) ? 5 : 7);
			nDirMark2 = (pBri->m_dWidthSlabRight<0) ? (pBri->IsOutWall(FALSE) ? 1 : 3) : (pBri->IsOutWall(FALSE) ? 7 : 5);
			if((bMarkJijum && bMarkSlabSttEnd)==FALSE) 
			{
				nDirMark1 = (pBri->m_dWidthSlabRight<0) ? 1 : 7;
			}
			xy1 = pLine->GetXyLine(dSta);
			strX.Format(_T(" X : %.6f "), toM(xy1.y));
			strY.Format(_T(" Y : %.6f "), toM(xy1.x));
			sym.GiSiArrowExtendAng(&Dom, xy1.x, xy1.y, dLenGisi, nDirLeft, nDirMark1, strX, strY, vAngC);
		}
		if(bMarkSlabSttEnd)
		{
			// 슬래브 종점(좌)
			if(A[0]!=B[0])
			{
				strX.Format(_T(" X : %.6f "), toM(B[0].y));
				strY.Format(_T(" Y : %.6f "), toM(B[0].x));
				sym.GiSiArrowExtendAng(&Dom, B[0].x, B[0].y, dLenGisi, -nDirLeft, pBri->IsOutWall(FALSE) ? 1 : 3, strX, strY, vAngL);
			}

			// 슬래브 종점(우)
			if(A[0]!=B[0])
			{
				strX.Format(_T(" X : %.6f "), toM(B[3].y));
				strY.Format(_T(" Y : %.6f "), toM(B[3].x));
				sym.GiSiArrowExtendAng(&Dom, B[3].x, B[3].y, dLenGisi, -nDirLeft, pBri->IsOutWall(FALSE) ? 7 : 5, strX, strY, vAngR);
			}

			// 슬래브 종점 : 도로중심
			double dLenUgan = pBri->GetLengthUgan(nJ, 0);//pJ->m_dLengthUgan[0];
			xy2 = pLine->GetXyLine(dSta-dLenUgan);
			if(xy1!=xy2)
			{
				strX.Format(_T(" X : %.6f "), toM(xy2.y));
				strY.Format(_T(" Y : %.6f "), toM(xy2.x));
				sym.GiSiArrowExtendAng(&Dom, xy2.x, xy2.y, dLenGisi, -nDirLeft, nDirMark2, strX, strY, vAngC);
			}
		}
	}
	else if(nJ>0 && nJ<nJijum-1)
	{
		CTwinVector tv;
		// 유간 선
		double dLenUganL = pBri->GetLengthUgan(nJ, 0);//pJ->m_dLengthUgan[0];
		double dLenUganR = pBri->GetLengthUgan(nJ, 1);//pJ->m_dLengthUgan[1];
			// 지점 좌측
		if(dLenUganL>0 && bMarkSlabSttEnd)
		{
			// 진행 방향 좌측
			tv = pBri->GetTvJijum(nJ, -dLenUganL);
			strX.Format(_T(" X : %.6f "), toM(tv.m_v1.y));
			strY.Format(_T(" Y : %.6f "), toM(tv.m_v1.x));
			sym.GiSiArrowExtendAng(&Dom, tv.m_v1.x, tv.m_v1.y, dLenGisi, -1, 3, strX, strY, vAngL);
			// 진행 방향 우측
			strX.Format(_T(" X : %.6f "), toM(tv.m_v2.y));
			strY.Format(_T(" Y : %.6f "), toM(tv.m_v2.x));
			sym.GiSiArrowExtendAng(&Dom, tv.m_v2.x, tv.m_v2.y, dLenGisi, -1, 5, strX, strY, vAngR);
			// 중심
			long nDirMark = (pBri->m_dWidthSlabRight<0) ? 3 : 5;
			xy1 = pLine->GetXyLine(dSta-dLenUganL);
			strX.Format(_T(" X : %.6f "), toM(xy1.y));
			strY.Format(_T(" Y : %.6f "), toM(xy1.x));
			sym.GiSiArrowExtendAng(&Dom, xy1.x, xy1.y, dLenGisi, -1, nDirMark, strX, strY, vAngC);
		}
			// 지점 우측
		if(dLenUganR>0 && bMarkSlabSttEnd)
		{
			// 진행 방향 좌측
			tv = pBri->GetTvJijum(nJ, dLenUganR);
			strX.Format(_T(" X : %.6f "), toM(tv.m_v1.y));
			strY.Format(_T(" Y : %.6f "), toM(tv.m_v1.x));
			sym.GiSiArrowExtendAng(&Dom, tv.m_v1.x, tv.m_v1.y, dLenGisi, 1, 1, strX, strY, vAngL);
			// 진행 방향 우측
			strX.Format(_T(" X : %.6f "), toM(tv.m_v2.y));
			strY.Format(_T(" Y : %.6f "), toM(tv.m_v2.x));
			sym.GiSiArrowExtendAng(&Dom, tv.m_v2.x, tv.m_v2.y, dLenGisi, 1, 7, strX, strY, vAngR);
			// 중심
			long nDirMark = (pBri->m_dWidthSlabRight<0) ? 1 : 7;
			xy1 = pLine->GetXyLine(dSta+dLenUganR);
			strX.Format(_T(" X : %.6f "), toM(xy1.y));
			strY.Format(_T(" Y : %.6f "), toM(xy1.x));
			sym.GiSiArrowExtendAng(&Dom, xy1.x, xy1.y, dLenGisi, 1, nDirMark, strX, strY, vAngC);
		}
		if(bMarkJijum)
		{
			// 지점 선
			tv = pBri->GetTvJijum(nJ, 0);
				// 진행 방향 좌측
			strX.Format(_T(" X : %.6f "), toM(tv.m_v1.y));
			strY.Format(_T(" Y : %.6f "), toM(tv.m_v1.x));
			sym.GiSiArrowExtendAng(&Dom, tv.m_v1.x, tv.m_v1.y, dLenGisi, -1, 3, strX, strY, vAngL);
				// 진행 방향 우측
			strX.Format(_T(" X : %.6f "), toM(tv.m_v2.y));
			strY.Format(_T(" Y : %.6f "), toM(tv.m_v2.x));
			sym.GiSiArrowExtendAng(&Dom, tv.m_v2.x, tv.m_v2.y, dLenGisi, -1, 5, strX, strY, vAngR);
				// 중심
			long nDirMark = (pBri->m_dWidthSlabRight<0) ? 3 : 5;
			xy1 = pLine->GetXyLine(dSta);
			strX.Format(_T(" X : %.6f "), toM(xy1.y));
			strY.Format(_T(" Y : %.6f "), toM(xy1.x));
			sym.GiSiArrowExtendAng(&Dom, xy1.x, xy1.y, dLenGisi, -1, nDirMark, strX, strY, vAngC);
		}
	}
/*
	// 지점 스테이션
	CString str = _T("");
	double dSta = pBri->GetStationOnJijum(nJ, 0);
	str = GetStationForStringOut(dSta, 3, TRUE, FALSE);
	CDPoint xyJijum = pBri->GetXyOnJijum(nJ);
	CDPoint vAng = pBri->GetAngleAzimuthTanOnJijum(nJ, 0);
	sym.GiSiArrowExtendAng(&Dom, xyJijum.x, xyJijum.y, Dom.Always(10), -1, 3, str, _T(""), vAng);
*/
	*pDomP << Dom;
}

void CDrawPlane::MarkPlaneArrowDirection(CDomyun *pDomP, double dLenSpare)
{
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;
	CLineInfoApp* pLineApp = static_cast<CLineInfoApp*>(pBri->GetLineBase());

	CDPoint A[4], B[4];
	pBri->GetXySss(BRIDGE_OFF_STT_STT, A);
	pBri->GetXySss(BRIDGE_OFF_END_END, B);

	double dWidthL = pBri->m_dWidthSlabLeft;
	double dWidthR = pBri->m_dWidthSlabRight;
	double dStaStt = pBri->m_dStationBridgeStt - dLenSpare;
	double dStaEnd = pBri->m_dStationBridgeStt + pBri->m_dLengthBridge + dLenSpare;
	CDPoint xyStt   = pLineApp->GetXyLine(dStaStt);
	CDPoint xyEnd   = pLineApp->GetXyLine(dStaEnd);
	CDPoint vRotate = (xyEnd-xyStt).Unit();

	CDPoint vLineStt = pLineApp->GetAngleAzimuth(dStaStt);
	CDPoint vStt = vLineStt.BetweenAngle((A[1]-A[2]).Unit());
	CDPoint vLineEnd = pLineApp->GetAngleAzimuth(dStaEnd);
	CDPoint vEnd = vLineEnd.BetweenAngle((B[1]-B[2]).Unit());

	CDPoint xySttL = pBri->GetXySlabEdgeAct(dStaStt, vStt, TRUE);
	CDPoint xyEndR = pBri->GetXySlabEdgeAct(dStaEnd, vEnd, FALSE);

	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CString str = _T("");
	BOOL bRotateText = vRotate.GetAngleRadian()>ConstPi/2 && vRotate.GetAngleRadian()<ConstPi*3/2;

	if(dWidthL<0)
	{
		str = m_pStd->m_pARcBridgeDataStd->m_strLineRevDirName;
		CDPoint xyS = (xySttL+xyStt)/2 - dLenSpare*vLineStt;
		Sym.ArrowDirection(&Dom, xyS, str, vLineStt.Rotate(CDPoint(-1,0)), 0, bRotateText);
	}

	if(dWidthR>0)
	{
		str = m_pStd->m_pARcBridgeDataStd->m_strLineDirName;
		CDPoint xyE = (xyEnd+xyEndR)/2 + dLenSpare*vLineEnd;
		Sym.ArrowDirection(&Dom, xyE, str, vLineEnd, 0, bRotateText);
	}

	*pDomP << Dom;
}

void CDrawPlane::DimPlaneJijumLength(CDomyun *pDomP, BOOL bInput, BOOL bGenDom, BOOL bVerticalPlan)
{
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;
	CLineInfoApp* pLineApp = static_cast<CLineInfoApp*>(pBri->GetLineBase());

	CString str = _T("");
	CString strUpper = _T("");
	CString strLower = _T("");
	double dLen = 0;
	int nSize = pBri->m_arrJijumLen.GetSize();

	CDimLineDomyun DomTest(pDomP, pLineApp);
	m_pStd->m_pARoadOptionStd->SetEnvType(&DomTest, HCAD_DIML);

	CDimDomyun Dom(pDomP, pLineApp);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CString strDir = (bInput || bVerticalPlan) ? _T("TOP") : _T("BOTTOM");
	Dom.SetDirection(strDir);
	double dLen1 = (bInput || bVerticalPlan) ? -Dom.Always(30) : Dom.Always(30) + pBri->m_dWidthSlabRight;
	double dLen2 = (bInput || bVerticalPlan) ? -Dom.Always(25) : Dom.Always(25) + pBri->m_dWidthSlabRight;
	double dBWS = pBri->IsOutWall(TRUE) ? pBri->m_dBWS : 0;
	double dBWE = pBri->IsOutWall(FALSE) ? pBri->m_dBWE : 0;
	double dWS = pBri->IsOutWall(TRUE) ? pBri->m_dWS : 0;
	double dWE = pBri->IsOutWall(FALSE) ? pBri->m_dWE : 0;
	double dLenBri = pBri->m_dLengthBridge;
	int nCountJijum = pBri->GetCountJijum();
	CDPoint vAngS = pBri->GetAngleJijum(0);
	vAngS = CDPoint(Round(vAngS.x, 5), Round(vAngS.y, 5));
	CDPoint vAngE = pBri->GetAngleJijum(nCountJijum-1);
	vAngE = CDPoint(Round(vAngE.x, 5), Round(vAngE.y, 5));

	double dStaStt = (bInput || bVerticalPlan) ? pBri->m_dStationBridgeStt 
		                                       : pBri->m_dStationBridgeStt-(dWS+dBWS)/vAngS.y;
	Dom.DimBaseCurvePoint(dStaStt, dLen1, 0);
	if(pBri->IsOutsideWall())
	{
		if(bGenDom)
		{
			strUpper = COMMA(dBWS/vAngS.y);
			strLower = vAngS==CDPoint(0, 1) ? _T("") : _T("(")+COMMA(dBWS)+_T(")");
			Dom.DimCurveMoveTo(dStaStt, dLen2, 0, FALSE, strUpper, strLower);
			Dom.DimCurveLineTo(dBWS/vAngS.y, dLen2, 0, _T(""), _T(""));
			strUpper = COMMA(dWS/vAngS.y);
			strLower = vAngS==CDPoint(0, 1) ? _T("") : _T("(")+COMMA(dWS)+_T(")");
			Dom.DimCurveLineTo(dWS/vAngS.y, dLen2, 0, strUpper, strLower);
		}
		else
			Dom.DimCurveMoveTo(dStaStt, dLen2, 0);
		int i=0; for(i=0; i<nSize; i++)
		{
			dLen = pBri->m_arrJijumLen.GetAt(i);
			str = bInput ? pBri->m_arrStrJijumLen.GetAt(i) : _T("");
			strUpper = bInput ? str : COMMA(dLen);
			strLower = bInput ? COMMA(dLen) : pBri->IsEnableDisplayVertLengthOfJigan() ? COMMA2(dLen*vAngS.y) : _T("");
			if(bVerticalPlan)
			{
				strUpper = COMMA(dLen);
				strLower = _T("");
			}
			Dom.DimCurveLineTo(dLen, dLen2, 0, strUpper, strLower);
		}
		if(bGenDom)
		{
			strUpper = COMMA(dWE/vAngE.y);
			strLower = vAngE==CDPoint(0, 1) ? _T("") : _T("(")+COMMA(dWE)+_T(")");
			Dom.DimCurveLineTo(dWE/vAngE.y, dLen2, 0, strUpper, strLower);
			strUpper = COMMA(dBWE/vAngE.y);
			strLower = vAngE==CDPoint(0, 1) ? _T("") : _T("(")+COMMA(dBWE)+_T(")");
			Dom.DimCurveLineTo(dBWE/vAngE.y, dLen2, 0, _T(""), _T(""));
			Dom.DimCurveLineTo(Dom.GetTextWidth(strUpper), dLen2, 0, strUpper, strLower, FALSE, TRUE);

			dLen1 = Dom.Always(44.5)+pBri->m_dWidthSlabRight;
			dLen2 = Dom.Always(39.5)+pBri->m_dWidthSlabRight;
			Dom.DimBaseCurvePoint(dStaStt, dLen1, 0);
			Dom.DimCurveMoveTo(dStaStt, dLen2, 0);
			strUpper = COMMA((dWS+dBWS)/vAngS.y);
			strLower = vAngS==CDPoint(0, 1) ? _T("") : _T("(")+COMMA(dWS+dBWS)+_T(")");
			Dom.DimCurveLineTo((dWS+dBWS)/vAngS.y, dLen2, 0, strUpper, strLower);
			Dom.DimCurveLineTo(dLenBri, dLen2, 0, COMMA(dLenBri), _T(""));
			strUpper = COMMA((dWE+dBWE)/vAngE.y);
			strLower = vAngE==CDPoint(0, 1) ? _T("") : _T("(")+COMMA(dWE+dBWE)+_T(")");
			Dom.DimCurveLineTo((dWE+dBWE)/vAngE.y, dLen2, 0, strUpper, strLower);
		}
//		else
//		{
//			DomTest.DimMoveToOnLine(dStaStt,0,0);
//			DomTest.DimLineToOnLine(dLenBri,COMMA(dLenBri));
//		}		
	}
	else
	{
		if(pBri->m_arrStrJijumLen.GetSize() > 0)
			str = pBri->m_arrStrJijumLen.GetAt(0);
		if(pBri->m_arrJijumLen.GetSize() > 0)
			dLen = pBri->m_arrJijumLen.GetAt(0);

		strUpper = bInput ? str : COMMA(dLen);
		strLower = bInput ? COMMA(dLen) : _T("");
		if(bVerticalPlan)
		{
			strUpper = COMMA(dLen);
			strLower = _T("");
		}
		Dom.DimCurveMoveTo(dStaStt, dLen2, 0, FALSE, strUpper, strLower);
		nSize	= pBri->m_arrStrJijumLen.GetSize();
		int i=0; for(i=0; i<nSize; i++)
		{
			str = pBri->m_arrStrJijumLen.GetAt(i);
			dLen = pBri->m_arrJijumLen.GetSize() > i ? pBri->m_arrJijumLen.GetAt(i) : 0;
			strUpper = bInput ? str : COMMA(dLen);
			strLower = bInput ? COMMA(dLen) : _T("");
			if(bVerticalPlan)
			{
				strUpper = COMMA(dLen);
				strLower = _T("");
			}
			if(i==0)
				Dom.DimCurveLineTo(dLen, dLen2, 0, _T(""), _T(""));
			else if(i==nSize-1)
				Dom.DimCurveLineTo(dLen, dLen2, 0, strUpper, strLower, FALSE, FALSE, TRUE);
			else
				Dom.DimCurveLineTo(dLen, dLen2, 0, strUpper, strLower);
		}
	}

	*pDomP << DomTest;
	*pDomP << Dom;
}
//<summary>
//가각에 대한 치수선을 그림
//</summary>
//<param name="bInput">입력 여부</param>
//<param name="bGenDom">현재는 적용하지 않음</param>
void CDrawPlane::DimPlaneJijumGagak(CDomyun *pDomP, BOOL bInput, BOOL bGenDom)
{
	DimPlaneJijumGagakByEach(pDomP, bInput, bGenDom, TRUE, TRUE);
	DimPlaneJijumGagakByEach(pDomP, bInput, bGenDom, FALSE, TRUE);
	DimPlaneJijumGagakByEach(pDomP, bInput, bGenDom, TRUE, FALSE);
	DimPlaneJijumGagakByEach(pDomP, bInput, bGenDom, FALSE, FALSE);
	return;
}
//<summary>
//가각에 대한 치수선을 그림
//</summary>
//<param name="bInput">입력 여부</param>
//<param name="bGenDom">현재는 적용하지 않음</param>
//<param name="bLeft">TRUE = 좌측 슬래브, FALSE = 우측 슬래브</param>
//<param name="bStt">TRUE = 교량 시점, FALSE = 교량 종점</param>
void CDrawPlane::DimPlaneJijumGagakByEach(CDomyun *pDomP, BOOL bInput, BOOL bGenDom, BOOL bLeft, BOOL bStt)
{
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;
	CLineInfoApp* pLineApp	= static_cast<CLineInfoApp*>(pBri->GetLineBase());

	double W = 0;
	double H = 0;
	double R = 0;
	CDPoint slabA[4];
	double staSlab = 0;
	double staSlabW = 0;
	double staStt = 0;
	double disSlab = 0;
	CDPoint xySlabSttEnd;
	CDPoint xyCenStt(0,0),xySegStt(0,0);
	CString strD;
	CString strH;
	CString strR;
	CString strTopBot;
	CString strLeftRigh;
	long nSide = 1;
	CDPoint xyGagakStt;
	CLineInfo *pLine = pBri->GetLineLeftRight(bLeft);

	if(bStt && bLeft)
	{
		W = pBri->m_dExpSttW[0];
		H = pBri->m_dExpSttH[0];
		if(W == 0 || H == 0)
			return;
		R = pBri->m_dExpSttR[0];
		disSlab = pBri->m_dWidthSlabLeft;
		pBri->GetXySss(BRIDGE_OFF_STT_SLAB,slabA);
		staSlab = pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB,-1,FALSE);
		//staSlabW = staSlab + W;
		//staSlab	= pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, FALSE);
		staSlabW	= pLine->GetStationByDisLength(staSlab, W, disSlab);
		staStt = staSlab;
		xySlabSttEnd = slabA[0];
		strD = "sDL";
		strH = "sHL";
		strR = "sRL";
		strTopBot = "BOTTOM";
		strLeftRigh = "RIGHT";
		xyGagakStt = pLine->GetXyLineDis(pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB,-1,TRUE), disSlab-H);
	}
	else if(bStt && !bLeft)
	{
		W = pBri->m_dExpSttW[1];
		H = pBri->m_dExpSttH[1];
		if(W == 0 || H == 0)
			return;
		R = pBri->m_dExpSttR[1];
		disSlab = pBri->m_dWidthSlabRight;
		pBri->GetXySss(BRIDGE_OFF_STT_SLAB,slabA);
		staSlab = pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB,1,FALSE);
		//staSlabW = staSlab + W;
		///staSlab	= pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, FALSE);
		staSlabW	= pLine->GetStationByDisLength(staSlab, W, disSlab);
		staStt = staSlab;
		xySlabSttEnd = slabA[3];
		strD = "sDR";
		strH = "sHR";
		strR = "sRR";
		strTopBot = "TOP";
		strLeftRigh = "RIGHT";
		nSide = -1;
		xyGagakStt = pLine->GetXyLineDis(pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB,1,TRUE), disSlab+H);
	}
	else if(!bStt && bLeft)
	{
		W = pBri->m_dExpEndW[0];
		H = pBri->m_dExpEndH[0];
		if(W == 0 || H == 0)
			return;
		R = pBri->m_dExpEndR[0];
		disSlab = pBri->m_dWidthSlabLeft;
		pBri->GetXySss(BRIDGE_OFF_END_SLAB,slabA);
		staSlab = pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB,-1,FALSE);
		//staSlabW = staSlab - W;
		//staSlab	= pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, FALSE);
		staSlabW	= pLine->GetStationByDisLength(staSlab, -W, disSlab);
		staStt = staSlabW;
		xySlabSttEnd = slabA[0];
		strD = "eDL";
		strH = "eHL";
		strR = "eRL";
		strTopBot = "BOTTOM";
		strLeftRigh = "LEFT";
		xyGagakStt = pLine->GetXyLineDis(pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB,-1,TRUE), disSlab-H);
	}
	else if(!bStt && !bLeft)
	{
		W = pBri->m_dExpEndW[1];
		H = pBri->m_dExpEndH[1];
		if(W == 0 || H == 0)
			return;
		R = pBri->m_dExpEndR[1];
		disSlab = pBri->m_dWidthSlabRight;
		pBri->GetXySss(BRIDGE_OFF_END_SLAB,slabA);
		staSlab = pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB,1,FALSE);
		//staSlabW = staSlab - W;
		//staSlab	= pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, FALSE);
		staSlabW	= pLine->GetStationByDisLength(staSlab, -W, disSlab);
		staStt = staSlabW;
		xySlabSttEnd = slabA[3];
		strD = "eDR";
		strH = "eHR";
		strR = "eRR";
		strTopBot = "TOP";
		strLeftRigh = "LEFT";
		nSide = -1;
		xyGagakStt = pLine->GetXyLineDis(pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB,1,TRUE), disSlab+H);
	}

	CDPoint xyW = pLine->GetXyLineDis(staSlabW, disSlab);
	CDPoint vAngAbs = pLine->GetAngleAzimuth(staSlabW);
	double staGagakEnd;
	pLine->GetXyForRc(disSlab, R, xyW, xySlabSttEnd, bStt ,bLeft, staGagakEnd,xyCenStt,xySegStt);
	CDPoint xyArcEnd = pLine->GetXyLineDis(staGagakEnd, disSlab);

	CString strUpper = _T("");
	CString strLower = _T("");
	CDimDomyun Dom(pDomP, pLineApp);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	long nL	= bLeft ? pBri->m_nLineLeft : pBri->m_nLineRight;

	Dom.SetDirection(strTopBot);
	Dom.DimBaseCurvePoint(staStt, disSlab +Dom.Always(10)*nSide, 0);
	if(bInput)
	{
		strUpper = strD;
		strLower = COMMA(W);
		Dom.LineTo(xyW, xyGagakStt);
		Dom.LineTo(xyW, xyArcEnd);
	}
	else
	{
		strUpper = COMMA(W);
		strLower = "";
	}
	double staGagakEndNoARc	= 0;
	pLine->GetStationMatchVerticalByXy(xyW, staGagakEndNoARc);
	if(bStt)
	{
		Dom.DimCurveMoveTo(staStt, disSlab+Dom.Always(10)*nSide, nL);
		Dom.DimCurveLineTo(staGagakEndNoARc-staSlab, disSlab+Dom.Always(10)*nSide, nL, strUpper, strLower);
	}
	else
	{
		Dom.DimCurveMoveTo(staSlab, disSlab+Dom.Always(10)*nSide, nL);
		Dom.DimCurveLineTo(staGagakEndNoARc-staSlab, disSlab+Dom.Always(10)*nSide, nL, strUpper, strLower);
	}

	Dom.DimBaseCurvePoint(staStt, disSlab + Dom.Always(20)*nSide, 1);

	strUpper	= COMMA(fabs(pLine->GetLengthByDis(staGagakEnd, staSlab, disSlab)));
	Dom.DimCurveMoveTo(staSlab, disSlab+Dom.Always(10)*nSide, nL);
	Dom.DimCurveLineTo(staGagakEnd - staSlab, disSlab+Dom.Always(10)*nSide, nL, strUpper, "");
	*pDomP << Dom;

	if(R > 0)
	{
		if(GetDirectionRotateByPoint(xySegStt, xyArcEnd, xyCenStt)==-1)	// 시계방향
		{
			CDPoint xyArcEndPre = xyArcEnd;
			xyArcEnd = xySegStt;
			xySegStt = xyArcEndPre;
		}
		CDPoint vAngArcCenter = (xyCenStt - xyW).Unit();
		double dAngStt = (xySegStt-xyCenStt).Unit().GetAngleDegree();
		double dAngEnd = (xyArcEnd-xyCenStt).Unit().GetAngleDegree();
		CDPoint xyResult1, xyResult2;
		GetXyMatchLineAndArc(xyCenStt, vAngArcCenter, xyCenStt, R, ToRadian(dAngStt), ToRadian(dAngEnd), xyResult1, xyResult2);
		CDPoint xyArcCenterMatch = xyResult1;
		double lenGisi = Dom.Always(50);
		if(bInput)
		{
			strUpper = strR;
			strLower = COMMA(R);
		}
		else
		{
			strUpper = "R = " + COMMA(R);
			strLower = "";
			lenGisi = Dom.Always(25);
		}

		Dom.GiSiArrowExtend(xyArcCenterMatch.x, xyArcCenterMatch.y, 0, lenGisi, 0, strUpper, strLower);
		Dom.Rotate(xyArcCenterMatch, vAngArcCenter);

		*pDomP << Dom;
	}

	if(bInput)
	{
		Dom.SetDirection(strLeftRigh);
		strUpper = strH;
		strLower = COMMA(H);
		Dom.DimMoveTo(xyW.x, xyW.y, -1);
		Dom.DimLineToExtend(H*nSide, -1, strUpper, strLower);
		Dom.Rotate(xyW, vAngAbs);
		*pDomP << Dom;
	}
}

void CDrawPlane::DimPlaneJijumAngle(CDomyun *pDomP, BOOL bInput)
{
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CConcSymbol sym(m_pStd->m_pARoadOptionStd);

	CDPoint xy0(0, 0), xy1(0, 0), xy2(0, 0), vAng(0, 0), vAngAzi(0, 0), vAngTan(0, 0);
	double dR = Dom.Always(1.0);
	CString str = _T("");
	int nCountJijum = pBri->GetCountJijum();
	int i=0; for(i=0; i<nCountJijum; i++)
	{
		vAng = pBri->GetAngleJijum(i);
		vAngAzi = pBri->GetAngleAzimuthJijum(i);
		vAngTan = pBri->GetAngleAzimuthTanOnJijum(0, 0);
		if(vAng.GetAngleDegree()==90.0) continue;

		xy0 = pBri->GetXyOnJijum(i);
		xy1 = xy0 + dR*vAngTan;
		xy2 = xy0 - dR*vAngAzi;

		sym.DrawAngleNotation(&Dom, xy0, xy1, xy2, dR, 0, !bInput);
	}

	*pDomP << Dom;
}

void CDrawPlane::MarkPlaneBridgeSttEnd(CDomyun *pDomP)
{
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;

	double dStaBridgeStt = pBri->m_dStationBridgeStt;
	double dStaBridgeEnd = dStaBridgeStt + pBri->m_dLengthBridge;
	int nCountJijum = pBri->GetCountJijum();

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	// 교량 시점 표기
	CString csText = _T("교량 시점");
	CString csSta = _T(" ") + GetStationForStringOut(dStaBridgeStt, 3, TRUE, FALSE) + _T(" ");

	double dSpace = Dom.Always(30);
	double dLen1 = Dom.Always(2);
	double dLen2 = Dom.GetTextWidth(csSta,0);
	double dTextHeight = Dom.GetTextHeight();
	double dSpaceText = dTextHeight/4.0;

	CDPoint xyJijum = pBri->GetXyOnJijum(0);
	CDPoint vAng = pBri->GetAngleAzimuthTanOnJijum(0, 0);
	CDPoint vAngStt = vAng.Rotate90();
	CDPoint vAng30 = CDPoint(1, 1.5).Unit();
	CDPoint vAngText = vAng30.Rotate(vAngStt);
	CDPoint xy0 = xyJijum + vAngStt*dSpace;
	CDPoint xy1 = xy0 + vAngStt*dLen1;
	CDPoint xy2 = xy1 + vAngText*dLen2;
	CDPoint xyText = (xy1+xy2)/2;

	Dom.LineTo(xy0, xy1);
	Dom.LineTo(xy1, xy2);

	Dom.SetTextAlignHorz(TA_CENTER);
	vAngText = vAng30.Rotate(-vAngStt);
	double dAngText = vAngText.GetAngleDegree();
	Dom.SetTextAngle(dAngText);
	Dom.TextOut(xyText+vAngText.Rotate90()*dSpaceText, csText);
	Dom.TextOut(xyText+vAngText.RotateInv90()*(dSpaceText+dTextHeight), csSta);

	// 교량 종점 표기
	csText = _T("교량 종점");
	csSta = _T(" ") + GetStationForStringOut(dStaBridgeEnd, 3, TRUE, FALSE) + _T(" ");

	xyJijum = pBri->GetXyOnJijum(nCountJijum-1);
	vAng = pBri->GetAngleAzimuthTanOnJijum(nCountJijum-1, 0);
	CDPoint vAngEnd = vAng.Rotate90();
	vAng30 = CDPoint(1, -1.5).Unit();
	vAngText = vAng30.Rotate(vAngEnd);
	xy0 = xyJijum + vAngEnd*dSpace;
	xy1 = xy0 + vAngEnd*dLen1;
	xy2 = xy1 + vAngText*dLen2;
	xyText = (xy1+xy2)/2;

	Dom.LineTo(xy0, xy1);
	Dom.LineTo(xy1, xy2);

	dAngText = vAngText.GetAngleDegree();
	Dom.SetTextAngle(dAngText);
	Dom.TextOut(xyText+vAngText.Rotate90()*dSpaceText, csText);
	Dom.TextOut(xyText+vAngText.RotateInv90()*(dSpaceText+dTextHeight), csSta);

	*pDomP << Dom;
}

// 평면도 상부슬래브 주철근 그리기
// long nDrawLeft : -1 - 진행방향 좌측만 그림, 0 - 진행방향 좌우측 모두 그림, 1 - 진행방향 우측만 그림
void CDrawPlane::DrawRebarMain_Plane_UpperSlab(CDomyun *pDomP, BOOL bUpper, long nDan, long nCycle, long nDrawLeft, BOOL bDimRebar, BOOL bMarkRebar, BOOL bDimTotal, long nDirMark)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	// 주철근 간격이 구해진 상태인지?
	BOOL bCalDistMainRebar	= FALSE;	

	// 사보강 배치를 대비해서 사방향 철근이 들어가는 구간의 거리를 미리 설정함.
	pBri->SetDataDefaultDistLeftDirPlacing();
	
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	// 철근 가져오기
	CHgBaseDrawStd baseDraw;
	baseDraw.m_pLine	= pBri->GetLineBase();
	
	// 치수기입(nDrawLeft가 -1이나 0일때는 시점에, 1일때는 종점에 기입함)
	CLineInfo *pLine = pBri->GetLineBase(pBri->IsVertDir());
	CLineInfo *pLineL	= pBri->GetLineLeftRight(TRUE);
	CLineInfo *pLineR	= pBri->GetLineLeftRight(FALSE);

	if(bDimRebar)
	{
		*pDomP << Dom;
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		BOOL bStt	= (nDrawLeft == 0 || nDrawLeft == -1) ? TRUE : FALSE;

		// vert == 0 : 직방향 등 기본 철근
		// vert == 1 : 사보강 배치시 사방향 철근
		CTwinVectorArray tvArr, tvArrAngLeft, tvArrAngRight;
		CTwinVectorArray tvArrJoint, tvArrHidden;
		pBri->GetTvRebarMain_Plane_UpperSlab_TypeNormal_Std(tvArr, tvArrAngLeft,tvArrAngRight, bUpper, -1, nDan, 0, 0, FALSE, FALSE, FALSE, tvArrJoint, tvArrHidden, tvArrHidden, tvArrHidden, FALSE);

		// 전체 치수를 넣을 시기를 결정함
		long nDimTotal	= 0;
		BOOL bAngleRebar	= FALSE;
		if(tvArrAngLeft.GetSize() > 0 || tvArrAngRight.GetSize() > 0) bAngleRebar	= TRUE;
		CTwinVector tvStt, tvEnd;
		pBri->GetTvSttEndOfSlab(tvStt, tvEnd);
		GetSwap(tvStt.m_v1, tvStt.m_v2);
		GetSwap(tvEnd.m_v1, tvEnd.m_v2);

		for(long vert = 0; vert < 3; vert++)
		{
			CDoubleArray dArrHunch;
			CDoubleArray dArrDim;
			CStringArray sArrText;
			CStringArray sArrUnderText;
			CString sDir	= bStt ? "TOP" : "BOTTOM";
			CDPoint xyStt	= CDPoint(0, 0);
			long nDimDan	= nDan+2;
			if(bAngleRebar && vert == 0) nDimDan++;
			
			// 치수기입 기준위치
			CDPoint vAng	= pBri->GetAngleJijum(0);
			double dSta		= pBri->GetStationByTarget(bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0, TRUE);
			CTwinVector tvJigan	= bStt ? tvStt : tvEnd;

			// 사방향 철근은 사방향 배근된 부분만 치수를 기입한다.
			// 따라서 tvJigan을 사방향 배근된 만큼만 살려 놓는다.
			CLineInfo *pLineCur	= pLine;
			if(vert == 1)
			{
				pLineCur	= pLineL;
				tvArr.RemoveAll();
				tvArr.AddFromTvArray(tvArrAngLeft);
				
				if(tvArr.GetSize() == 0) continue;

				CTwinVector tv	= tvArr.GetAt(tvArr.GetSize()-1);
				if(pBri->m_nTypeSideRebarMain_UpperSlabSabogang == 0)
				{
					GetXyMatchLineAndLine(tv.m_v1, tv.GetXyDir(), tvJigan.m_v1, tvJigan.GetXyDir(), tvJigan.m_v2);
				}
				else
				{
					double dDist	= pLineCur->GetDisMatchVerticalByXy(tv.m_v1);
					double sta		= dSta;
					pLineCur->GetStationMatchByXyAndAngle(tvJigan.m_v1, tvJigan.GetXyDir(), sta, dDist);
					tvJigan.m_v2	= pLineCur->GetXyLineDis(sta, dDist);
				}
			}
			else if(vert == 2)
			{
				pLineCur	= pLineR;
				tvArr.RemoveAll();
				tvArr.AddFromTvArray(tvArrAngRight);
				pBri->SortTvArrByLinePlane(tvArr);
				
				if(tvArr.GetSize() == 0) continue;

				CTwinVector tv	= tvArr.GetAt(0);
				if(pBri->m_nTypeSideRebarMain_UpperSlabSabogang == 0)
				{
					GetXyMatchLineAndLine(tv.m_v1, tv.GetXyDir(), tvJigan.m_v1, tvJigan.GetXyDir(), tvJigan.m_v1);
				}
				else
				{
					double dDist	= pLineCur->GetDisMatchVerticalByXy(tv.m_v1);
					double sta		= dSta;
					pLineCur->GetStationMatchByXyAndAngle(tvJigan.m_v1, tvJigan.GetXyDir(), sta, dDist);
					tvJigan.m_v1	= pLineCur->GetXyLineDis(sta, dDist);
				}
			}

			xyStt	= tvJigan.m_v2;
			// 주철근을 기준위치로 뻗어서 거리 구함.
			bCalDistMainRebar	= TRUE;

			// 가각 철근이 선형방향 배치면 치수를 포함시킨다.
			// 단, 주철근이 선형방향 배치이어야 한다
			if(pBri->m_nTypePlacingMainRebar_UpperSlab == 0)
			{
				long left = 0; for(left = 0; left < 2; left++)
				{
					BOOL bLeft	= left == iLEFT;
					if(pBri->m_bLineDirRebarOfExp[bStt ? iSTT : iEND][left])
					{
						CTwinVectorArrayArray pTvArrCycle1[2];
						CTwinVectorArrayArray pTvArrCycle2[2];

						pBri->GetTvRebarMain_Plane_Slab_Exp(TRUE, &pTvArrCycle1[0], &pTvArrCycle2[0], &pTvArrCycle1[1], &pTvArrCycle2[1], bStt, bLeft, 0, bUpper);

						long i = 0; for(i = 0; i < pTvArrCycle2[0].GetSize(); i++)
						{
							if(pTvArrCycle2[0].GetAt(i)->GetSize() == 0) continue;
							tvArr.Add(pTvArrCycle2[0].GetAt(i)->GetAt(0));
						}


						for(long type = 0; type < 2; type++)
						{
							AhTPADelete(&pTvArrCycle1[type], (CTwinVectorArray*)0);
							AhTPADelete(&pTvArrCycle2[type], (CTwinVectorArray*)0);
						}				
					}
				}
			}

			// 가각 철근 치수가 주철근치수에 겹치면 빼버림
			CDPointArray xyArr;
			CDPoint xyMatch(0, 0);
			CDPointArray xyArrMatch;
			CTwinVector tv;
			CDPoint vAngDim	= pBri->GetAngleByTarget(bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0); //pBri->GetAngleJijum(nJ, bVertDir);
			BOOL bPlanArc = FALSE;
			long i = 0; for(i = 0; i < tvArr.GetSize(); i++)
			{
				tv	= tvArr.GetAt(i);
				if(tv.m_v1.z != 0)
				{
					if(vert == 0 || pBri->m_nTypeSideRebarMain_UpperSlabSabogang == 0)
					{
						double dDist	= pLine->GetDisMatchVerticalByXy(tv.m_v1);

						xyMatch	= pLine->GetXyLineByLengthAndAngle(dSta, dDist/vAngDim.y, vAngDim);

						// Arc가 적용된 실제 배근된 치수를 넣으려면 아래와 같이 해야하나
						// 치수가 지저분하게 나와서 하지 않는 것으로 함
						//xyMatch	= pLine->GetXyLineDisByAng(dSta, dDist, vAngDim);
						xyArr.Add(xyMatch);
						bPlanArc = TRUE;
					}
					else
					{
						double dDist	= pLineCur->GetDisMatchVerticalByXy(tv.m_v1);
						double sta		= dSta;
						pLineCur->GetStationMatchByXyAndAngle(tvJigan.m_v1, tvJigan.GetXyDir(), sta, dDist);
						xyMatch	= pLineCur->GetXyLineDis(sta, dDist);
						xyArr.Add(xyMatch);
					}
				}
				else
				{
					if(GetXyMatchSegAndLine(tvJigan.m_v1, tvJigan.m_v2, tv.m_v1, tv.GetXyDir(), xyMatch))
						xyArr.Add(xyMatch);
				}
			}

			//#0029484 - 전체 치수를 표현할때는 xyMatch를 넣을필요가 없다.
			if(bPlanArc && vert != 0)
			{
				tv	= tvArr.GetAt(0);
				double dDist	= pLine->GetDisMatchVerticalByXy(tv.m_v1);
				if(dDist < 0)
					dDist -= pBri->m_dCoverSide_UpperSlab[nDan];
				else
					dDist += pBri->m_dCoverSide_UpperSlab[nDan];

				xyMatch	= pLine->GetXyLineByLengthAndAngle(dSta, dDist/vAngDim.y, vAngDim);				
				xyArr.Add(xyMatch);				
			}

			long nSize	= xyArr.GetSize();
			if(nSize > 0)
			{
				CDPoint xyLast(0, 0);
				CDPoint xy(0, 0);
				CDoubleArray dArrDimTmp;

				double dDistMax	= tvJigan.GetLength();
				double dDist	= 0;

				long i = 0; for(i = 0; i < nSize; i++)
				{
					xy	= xyArr.GetAt(i);
					dDist	= ~(xy - tvJigan.m_v2);

					dArrDimTmp.Add(dDist);
				}

				//#0029484 - 전체 치수를 표현할때는 tvJigan의 길이를 넣어야 한다.
				if(!bPlanArc ||  vert == 0)
					dArrDimTmp.Add(tvJigan.GetLength());

				dArrDimTmp.Sort(TRUE);


				nSize	= dArrDimTmp.GetSize();
				if(dArrDimTmp.GetSize() > 0)
				{
					dDist	= dArrDimTmp.GetAt(0);
					dArrDim.Add(dDist);
					for(i = 1; i < nSize; i++)
					{
						dDistMax	= dArrDimTmp.GetAt(i);
						dDist		= dArrDimTmp.GetAt(i-1);

						if(dDistMax - dDist > 0)
							dArrDim.Add(dDistMax-dDist);
					}
				}
			}

			// 치수 기입
			// 주철근이 선형방향배근이라면 치수선에 obq 적용
			double dObq	=  0;
			if(pBri->m_nTypePlacingMainRebar_UpperSlab == 0)
			{
				double dAng		= pBri->GetAngleByTarget(bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0).GetAngleDegree();// pBri->GetAngleJijum(bStt ? 0 : pBri->m_nQtyJigan).GetAngleDegree();
				dObq	= bStt ? dAng - 90 : 90 - dAng;
				Dom.SetDimObq(dObq);
			}
			else	// obq적용 안되는 경우 사각에 따라 시작위치 sta 조정 필요...
			{
				double dAng		= pBri->GetAngleByTarget(bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0).GetAngleDegree();
				if((bStt && dAng > 90) || (!bStt && dAng < 90))
				{
					CLineInfo *pLineB	= pBri->GetLineBase();
					if(!pLineB) return;	// program 다운 방지를 위해 깔끔하게 리턴시켜버림

					double dSta	= 0;
					if(pBri->m_nTypePlacingMainRebar_UpperSlab == 1 || pBri->m_nTypePlacingMainRebar_UpperSlab == 2)
						pLineB->GetStationMatchVerticalByXy(tvJigan.m_v2, dSta);	// 중심선형에서의 기점
					else
						pLineB->GetStationMatchVerticalByXy(tvJigan.m_v1, dSta);	// 중심선형에서의 기점
//  					xyStt	= pBri->GetXySlabEdgeAct(dSta, CDPoint(0, 1), FALSE);

					xyStt	= tvJigan.m_v2;
				}
			}
			baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, bDimTotal&&vert==nDimTotal, TRUE, FALSE);

			// 방향에 맞게 회전(시종점 접선의 직각방향으로 회전)
			CDPoint xyDir	= pBri->GetLineBase()->GetAngleAzimuth(dSta).Rotate90();
			Dom.Rotate(xyStt, ToDPointFrDegree(xyDir.GetAngleDegree()+(bStt ? dObq : -dObq)));

			// 사보강 배치인 경우 
			if(pBri->m_nTypePlacingMainRebar_UpperSlab == 1 || pBri->m_nTypePlacingMainRebar_UpperSlab == 2)
			{
				Dom.Rotate(xyStt, pBri->GetAngleByTarget(bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0).RotateInv90());
			}

			*pDomP << Dom;
		}
	}

	// 절단원 적용해서 그리기.
	CTwinVectorArray tvArr, tvArrAngLeft, tvArrAngRight;
	CTwinVectorArray tvArrAngLeftHalf, tvArrAngRightHalf;
	CTwinVectorArray tvArrJoint, tvArrHidden;
	
	// (ARCBRIDGE-2017) 1지간에만 2,3단철근 표현하던걸 옵션으로 각지간에 모두 표현
	BOOL bDrawAll23Dan = m_pStd->IsDrawUpperSlabRebar2DanAll();
	if(bDrawAll23Dan && nDan > 0)
	{
		long icJijum = bUpper? pBri->m_nQtyJigan+1 : pBri->m_nQtyJigan;
		for(long ix=0; ix<icJijum; ++ix)
		{
			pBri->GetTvRebarMain_Plane_UpperSlab_TypeNormal_Std(tvArr, tvArrAngLeft, tvArrAngRight, bUpper, nCycle, nDan, nDrawLeft, -1, FALSE, FALSE, FALSE, tvArrJoint, tvArrHidden, tvArrHidden, tvArrHidden, TRUE, tvArrAngLeftHalf, tvArrAngRightHalf, TRUE);

			ApplyCuttingArea_Plane(&Dom, tvArr, bUpper, nDan, FALSE, nDan==0 && nCycle==0 && nDrawLeft != 1, FALSE, FALSE, FALSE, ix, FALSE, FALSE, pBri->GetLineBase());
			ApplyCuttingArea_Plane(&Dom, tvArrAngLeft, bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, ix, FALSE, FALSE, pBri->GetLineLeftRight(TRUE));
			ApplyCuttingArea_Plane(&Dom, tvArrAngRight, bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, ix, FALSE, FALSE, pBri->GetLineLeftRight(FALSE));
			ApplyCuttingArea_Plane(&Dom, tvArrJoint, bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, ix);
			ApplyCuttingArea_Plane(&Dom, tvArrHidden, bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, ix);
			ApplyCuttingArea_Plane(&Dom, tvArrAngLeftHalf, bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, ix, FALSE, FALSE, pBri->GetLineLeftRight(TRUE));
			ApplyCuttingArea_Plane(&Dom, tvArrAngRightHalf, bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, ix, FALSE, FALSE, pBri->GetLineLeftRight(FALSE));

			DrawMainRebarUpperSlabByTvArr(&Dom, tvArr, tvArrAngLeft, tvArrAngRight, tvArrAngLeftHalf, tvArrAngRightHalf, tvArrJoint, tvArrHidden);
		}
	}
	else
	{
		pBri->GetTvRebarMain_Plane_UpperSlab_TypeNormal_Std(tvArr, tvArrAngLeft, tvArrAngRight, bUpper, nCycle, nDan, nDrawLeft, -1, FALSE, FALSE, FALSE, tvArrJoint, tvArrHidden, tvArrHidden, tvArrHidden, TRUE, tvArrAngLeftHalf, tvArrAngRightHalf, TRUE);

		ApplyCuttingArea_Plane(&Dom, tvArr, bUpper, nDan, FALSE, nDan==0 && nCycle==0 && nDrawLeft != 1, FALSE, FALSE, FALSE, 0, FALSE, FALSE, pBri->GetLineBase());
		ApplyCuttingArea_Plane(&Dom, tvArrAngLeft, bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, 0, FALSE, FALSE, pBri->GetLineLeftRight(TRUE));
		ApplyCuttingArea_Plane(&Dom, tvArrAngRight, bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, 0, FALSE, FALSE, pBri->GetLineLeftRight(FALSE));
		ApplyCuttingArea_Plane(&Dom, tvArrJoint, bUpper, nDan, FALSE, FALSE);
		ApplyCuttingArea_Plane(&Dom, tvArrHidden, bUpper, nDan, FALSE, FALSE);
		ApplyCuttingArea_Plane(&Dom, tvArrAngLeftHalf, bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, 0, FALSE, FALSE, pBri->GetLineLeftRight(TRUE));
		ApplyCuttingArea_Plane(&Dom, tvArrAngRightHalf, bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, 0, FALSE, FALSE, pBri->GetLineLeftRight(FALSE));

		DrawMainRebarUpperSlabByTvArr(&Dom, tvArr, tvArrAngLeft, tvArrAngRight, tvArrAngLeftHalf, tvArrAngRightHalf, tvArrJoint, tvArrHidden);
	}
	
	if(bMarkRebar)
	{

		CString sMark(_T(""));
		CTwinVectorArray tvArrTmp;
		// 사보강 배치가 아닐때는 선형용 치수기입 함수를 사용하고
		// 사보강 배치일때는 기초에서 복사해온 일반 치수기입용 함수를 사용한다. 
		if(pBri->m_nTypePlacingMainRebar_UpperSlab == 0)
		{
			
			long upper	= bUpper ? iUPPER : iLOWER;
			long nSize	= pBri->m_pArrRCMainUpperSlab[nCycle][upper][nDan].GetSize();
			
			long i = 0; for(i = 0; i < nSize; i++)
			{
				CRebarInfoCycle *pRC	= NULL;
				pRC	= pBri->GetRebarInfoCycleUpperSlab(nCycle, nDan, bUpper, i);
				if(pRC)
				{
					if(pRC->m_bExist)
						DimRebarInfoCycle_Plane_UpperSlab(&Dom, pRC, bUpper, nDan, nCycle, FALSE, nDrawLeft, FALSE, FALSE, FALSE, nDirMark);
				}
			}
		}
		else if(pBri->m_nTypePlacingMainRebar_UpperSlab == 1 || pBri->m_nTypePlacingMainRebar_UpperSlab == 2)
		{
			CRebarPlacing rb;
			CTwinVectorArray tvArrSup, tvArrSupAngStt, tvArrSupAngEnd, tvArrRAngStt, tvArrRAngEnd;
			pBri->GetTvRebarSupport_Plane_Slab_Std(TRUE, tvArrSup, tvArrSupAngStt, tvArrSupAngEnd, tvArrRAngStt, tvArrRAngEnd, bUpper, 0, 0, TRUE);

			CTwinVector tv;
			CRebarInfoCycle *pRC	= NULL;
			long upper			= bUpper ? iUPPER : iLOWER;
			long nSize			= pBri->m_pArrRCMainUpperSlab[nCycle][upper][nDan].GetSize();
			long dCountCycle	= pBri->GetCountCycleMainRebar();

			for(long r = 0; r < nSize; r++)
			{
				// 주철근 구함.
				pRC	= pBri->GetRebarInfoCycleUpperSlab(nCycle, nDan, bUpper, r);
				if(!pRC) continue;
				if(!pRC->m_bExist) continue;

				pBri->GetTvRebarMain_Plane_UpperSlab_TypeNormal_Std(tvArr, tvArrAngLeft, tvArrAngRight, bUpper, nCycle, nDan, nDrawLeft, r, FALSE, FALSE, FALSE, tvArrTmp, tvArrTmp, tvArrTmp, tvArrTmp, FALSE, tvArrAngLeftHalf, tvArrAngRightHalf, !bCalDistMainRebar);

				rb.ReverseRebar(tvArr);
				rb.ReverseRebar(tvArrAngLeft);
				rb.ReverseRebar(tvArrAngRight);
				rb.ReverseRebar(tvArrAngLeftHalf);
				rb.ReverseRebar(tvArrAngRightHalf);
	
				// 수직보강..
				sMark	= pRC->m_szMark;
				if(tvArr.GetSize() > 0 && m_pStd->IsMarkRebar(sMark))
				{
					MarkRebarMain_Plane_UpperSlab_ForVert(&Dom, tvArr, tvArrAngLeftHalf, tvArrAngRightHalf, tvArrSup, r, bUpper, nDan, nCycle, FALSE, FALSE, FALSE, nDrawLeft);
				}
			}
		}

		// (ARCBRIDGE-2661) 사보강배치가 아니더라도 확폭등에 의한 사보강 철근이 들어갈수 있다.
		if(pBri->IsVertDir() || pBri->IsExtLine())
		{
			CRebarPlacing rb;
			CTwinVectorArray tvArrSup, tvArrSupAngStt, tvArrSupAngEnd, tvArrRAngStt, tvArrRAngEnd;
			pBri->GetTvRebarSupport_Plane_Slab_Std(TRUE, tvArrSup, tvArrSupAngStt, tvArrSupAngEnd, tvArrRAngStt, tvArrRAngEnd, bUpper, 0, 0, TRUE);

			CTwinVector tv;
			CRebarInfoCycle *pRC	= NULL;
			long upper			= bUpper ? iUPPER : iLOWER;
			long nSize			= pBri->m_pArrRCMainUpperSlab[nCycle][upper][nDan].GetSize();
			long dCountCycle	= pBri->GetCountCycleMainRebar();

			for(long r = 0; r < nSize; r++)
			{
				// 주철근 구함.
				pRC	= pBri->GetRebarInfoCycleUpperSlab(nCycle, nDan, bUpper, r);
				if(!pRC) continue;
				if(!pRC->m_bExist) continue;
				CRebar *pRb = pBri->GetRebarMainByDescription(pRC->m_sDescription);

				if(pBri->IsVertDir() == FALSE || pBri->m_nTypePlacingMainRebar_UpperSlab > 0)
				{
					pBri->GetTvRebarMain_Plane_UpperSlab_TypeNormal_Std(tvArr, tvArrAngLeft, tvArrAngRight, bUpper, nCycle, nDan, nDrawLeft, r, FALSE, FALSE, FALSE, tvArrTmp, tvArrTmp, tvArrTmp, tvArrTmp, FALSE, tvArrAngLeftHalf, tvArrAngRightHalf, !bCalDistMainRebar);

					rb.ReverseRebar(tvArr);
					rb.ReverseRebar(tvArrAngLeft);
					rb.ReverseRebar(tvArrAngRight);
					rb.ReverseRebar(tvArrAngLeftHalf);
					rb.ReverseRebar(tvArrAngRightHalf);
				}

				// 사보강 (좌측)
				sMark.Format("%s-1", pRC->m_szMark);
				if(tvArrAngLeft.GetSize() > 0 && m_pStd->IsMarkRebar(sMark) && nDan == 0)	// 1단만 마크해도 복잡한데, 2,3단까지 전부 마크하면 너무 지저분해짐
				{
					CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
					mark.SetRebar(tvArrAngLeft, tvArrSup);

					// 설정
					mark.m_sMark					= sMark;
					mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pBri->GetValueFy(ePartUpperSlab));
					mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_END;
					mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
					mark.m_dPosScaleMarkStt			= 1./(dCountCycle+nSize) * (nCycle+r+1)+0.2;
					mark.m_dPosScale				= 1./(dCountCycle+nSize) * (nCycle+r+1)+0.2;
					mark.m_dPosScaleSpace			= 0.5 - (1./dCountCycle)/2;

					CDPointArray xyArr;
					mark.GetXyCountMark(xyArr);
					// 사보강 배치의 사방향 철근
					if(pBri->m_nTypeSideRebarMain_UpperSlabSabogang == 1 && xyArr.GetSize() > 0)
					{
						CDPoint xyDir;
						if(xyArr.GetSize() > 1)
						{
							CDPoint xyOrg1	= xyArr.GetAt(0);
							CDPoint xyOrg2	= xyArr.GetAt(1);
							xyDir = (xyOrg1-xyOrg2).Unit();
						}
						else
						{
							xyDir = mark.GetXyDirLine(TRUE);
						}
						
						long nSize		= xyArr.GetSize();
						long i = 0; for(i = 0; i < tvArrAngLeft.GetSize(); i++)
						{
							if(nSize <= i) break;
							CDPoint xyMark		= xyArr.GetAt(i);
							CDPoint xyMarkDim	= tvArrAngLeft.GetAt(i).m_v1;
							double dDist	= pLineL->GetDisMatchVerticalByXy(xyMarkDim);
							double sta		= 0;
							pLineL->GetStationMatchByXyAndAngle(xyMark, xyDir, sta, dDist);
							xyMark	= pLineL->GetXyLineDis(sta, dDist);
							xyArr.SetAt(i, xyMark);
						}
					}

					ApplyCuttingArea_Plane(&Dom, xyArr, bUpper, nDan, FALSE, FALSE, FALSE, FALSE);
					mark.m_xyArrCountMark	= xyArr;


					if(xyArr.GetSize() > 0)
					{
						CLineInfo *pLine	= pBri->GetLineBase();
						if(pLine)
						{
							CDPoint xyMark	= mark.GetXyMark(FALSE);
							CDPoint vAng;
							if(xyArr.GetSize() > 1)
								vAng	= (xyArr.GetAt(0)-xyArr.GetAt(1)).Unit();
							else
							{
								vAng	= mark.GetXyDirLine(TRUE);
								mark.m_xyDirCountMark = vAng;
							}

							CDPoint vAngJ	= pBri->GetAngleJijum(0);
							double dSta		= xyMark.x;
							pLine->GetStationMatchByXyAndAngle(xyMark, vAng, dSta, 0);
							double dWidth	= pBri->GetWidthSlabActByGagak(dSta, vAngJ, -1);
							CDPoint xyMatch	= pLine->GetXyLineByLengthAndAngle(dSta, pBri->IsRightFromRoadCenter() ? dWidth : -dWidth, vAngJ);

							mark.m_dAddHeight	= ~(xyMatch-xyMark) + Dom.GetTextHeight();
						}
						mark.m_dCtc						= pBri->GetCtcRebarMain(TRUE);
						mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;

						mark.MarkRebar(FALSE);	
					}
				}

				// 사보강 (우측)
				sMark.Format("%s-2", pRC->m_szMark);
				if(tvArrAngRight.GetSize() > 0 && m_pStd->IsMarkRebar(sMark) && nDan == 0)	// 1단만 마크해도 복잡한데, 2,3단까지 전부 마크하면 너무 지저분해짐
				{
					CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
					mark.SetRebar(tvArrAngRight, tvArrSup);

					// 설정
					mark.m_sMark					= sMark;
					mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pBri->GetValueFy(ePartUpperSlab));
					mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_STT;
					mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
					mark.m_dPosScaleMarkStt			= 1./(dCountCycle+nSize) * (nCycle+r+1)+0.2;
					mark.m_dPosScale				= 1./(dCountCycle+nSize) * (nCycle+r+1)+0.2;
					mark.m_dPosScaleSpace			= 0.5 - (1./dCountCycle)/2;
					
					CDPointArray xyArr;
					mark.GetXyCountMark(xyArr);

					// 사보강 배치의 사방향 철근
					if(pBri->m_nTypeSideRebarMain_UpperSlabSabogang == 1 && xyArr.GetSize() > 0)
					{
						CDPoint xyDir;
						if(xyArr.GetSize() > 1)
						{
							CDPoint xyOrg1	= xyArr.GetAt(0);
							CDPoint xyOrg2	= xyArr.GetAt(1);
							xyDir = (xyOrg1-xyOrg2).Unit();
						}
						else
						{
							xyDir = mark.GetXyDirLine(TRUE);
						}
						long nSize		= xyArr.GetSize();
						long i = 0; for(i = 0; i < tvArrAngRight.GetSize(); i++)
						{
							if(nSize <= i) break;
							CDPoint xyMark		= xyArr.GetAt(i);
							CDPoint xyMarkDim	= tvArrAngRight.GetAt(i).m_v1;
							double dDist	= pLineR->GetDisMatchVerticalByXy(xyMarkDim);
							double sta		= 0;
							pLineR->GetStationMatchByXyAndAngle(xyMark, xyDir, sta, dDist);
							xyMark	= pLineR->GetXyLineDis(sta, dDist);
							xyArr.SetAt(i, xyMark);
						}
					}
					
					ApplyCuttingArea_Plane(&Dom, xyArr, bUpper, nDan, FALSE, FALSE, FALSE, FALSE);
					mark.m_xyArrCountMark	= xyArr;

					if(xyArr.GetSize() > 0)
					{
						CLineInfo *pLine	= pBri->GetLineBase();
						if(pLine)
						{
							long nSize		= xyArr.GetSize();
							CDPoint xyMark	= mark.GetXyMark(FALSE);
							CDPoint vAng;
							if(xyArr.GetSize() > 1)
								vAng	= (xyArr.GetAt(nSize-2)-xyArr.GetAt(nSize-1)).Unit();
							else
							{
								vAng	= mark.GetXyDirLine(TRUE);
								mark.m_xyDirCountMark = vAng;
							}

							CDPoint vAngJ	= pBri->GetAngleJijum(0);
							double dSta		= xyMark.x;
							pLine->GetStationMatchByXyAndAngle(xyMark, vAng, dSta, 0);
							double dWidth	= pBri->GetWidthSlabAct(dSta, vAngJ, 1);
							CDPoint xyMatch	= pLine->GetXyLineByLengthAndAngle(dSta, dWidth, vAngJ);
							mark.m_dAddHeight	= ~(xyMatch-xyMark)+Dom.GetTextHeight();
						}
						mark.m_dCtc						= pBri->GetCtcRebarMain(TRUE);
						mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;

						mark.MarkRebar(FALSE);	
					}
				}
			}
		}
	}

	*pDomP << Dom;
}

// 평면도 상부슬래브 배력철근 그리기
// long nDrawLeft : -1 - 진행방향 좌측만 그림, 0 - 진행방향 좌우측 모두 그림, 1 - 진행방향 우측만 그림
void CDrawPlane::DrawRebarSupport_Plane_UpperSlab(CDomyun *pDomP, BOOL bUpper, long nDan, long nDrawLeft, BOOL bDimRebar, BOOL bMarkRebar)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	// 클로소이드 구간에서 배력근 간격이 모두 제각각인 경우가 발생할 수 있다. //////
	// 따라서 dDimLimit이하는 버려서 최대한 동일한 간격으로 맞춰준다.
	
	////////////////////////////////////////////////////////////////////////////////

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CHgBaseDrawStd baseDraw;
	CRebarPlacing rb;

	// 배력철근 좌표 구함.
	// 치수 기입을 위해서 피복 없이 좌표를 구함
	// 철근 그릴때는 피복 적용된 외곽선에 맞게 잘라냄
	CTwinVectorArray tvArr, tvArrAngStt, tvArrAngEnd, tvArrRAngStt, tvArrRAngEnd;
	pBri->GetTvRebarSupport_Plane_Slab_Std(TRUE, tvArr, tvArrAngStt, tvArrAngEnd, tvArrRAngStt, tvArrRAngEnd, bUpper, nDan, nDrawLeft);

	// nDrawLeft 가 -1, 0이면 진행방향 좌측에, 1이면 우측에 치수 기입
	if(bDimRebar)
	{
		const double dDimLimit	= 1;	

		*pDomP << Dom;
		
		long nDimDan	= nDan+1;
		// 사방향 배력철근이 있다면 치수 단을 하나 더 띄운다.
		if(tvArrAngEnd.GetSize() > 0 || tvArrAngStt.GetSize() > 0)
			nDimDan	+= 3;
		
		BOOL bBaseLine = m_nPosBaseDimSupportRebar == 1 ? TRUE : FALSE;
		BOOL bLeft		= (nDrawLeft == -1 || nDrawLeft == 0);
		CLineInfoApp* pLineApp	= static_cast<CLineInfoApp*>(pBri->GetLineLeftRight(bLeft));// ->GetLineBase();

		double dWidth	= bLeft ? pBri->m_dWidthSlabLeft : pBri->m_dWidthSlabRight;
		double dDist	= 0;//pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, CDPoint(0, 1), bLeft ? -1:1);
		
		dDist	= dWidth;
		if(pBri->m_bIsExp)
			dDist += bLeft ? -max(pBri->m_dExpSttH[0], pBri->m_dExpEndH[0]) : max(pBri->m_dExpSttH[1], pBri->m_dExpEndH[1]);

		if(bLeft)
			dDist -= Dom.Always(Dom.GetDimExo()+Dom.GetDimLevelDis()*(nDimDan+1));
		else
			dDist += Dom.Always(Dom.GetDimExo()+Dom.GetDimLevelDis()*(nDimDan+1));
		//if(bLeft) dDist	= -dDist;

		
		CString sDir	= bLeft ? _T("TOP") : _T("BOTTOM");

		CDimDomyun dimDom(pDomP, pBri->m_pLineApp);
		m_pStd->m_pARoadOptionStd->SetEnvType(&dimDom, HCAD_DIML);
		dimDom.SetDirection(sDir);
		long nL	= bLeft ? pBri->m_nLineLeft : pBri->m_nLineRight;// >m_nLineBase;
		if(bBaseLine)
			nL = pBri->m_nLineBase;
		
		// 수직보강철근 치수(사보강포함)  ///////////////////////////////////////////////////////////
		// dim == 0 : 수직보강철근 치수
		// dim == 1 : 시점측 사보강 치수
		// dim == 2 : 종점측 사보강 치수
		CLineInfo *pLine	= pBri->GetLineBase();

		// 철근들어가는 평면 좌표들 ////////
		CTwinVectorArray tvArrPlane;
		CTwinVectorArray tvArrPlane1Dan;
		CTwinVectorArray tvArrLeft1Dan[2];
		pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane1Dan, 0, FALSE);
		CTwinVector tv;
		long i = 0; for(i = 0; i < tvArrPlane1Dan.GetSize(); i++)
		{
			tv	= tvArrPlane1Dan.GetAt(i);
			if(tv.m_sInfo.Find("좌측", 0) != -1) tvArrLeft1Dan[iLEFT].Add(tv);
			else if(tv.m_sInfo.Find("우측", 0) != -1) tvArrLeft1Dan[iRIGHT].Add(tv);
		}

		
		CTwinVector tvJiganStt, tvJiganEnd;
		pBri->GetTvSttEndOfSlab(tvJiganStt, tvJiganEnd, FALSE);

		GetSwap(tvJiganStt.m_v1, tvJiganStt.m_v2);
		GetSwap(tvJiganEnd.m_v1, tvJiganEnd.m_v2);
		//////////////////////////////////////
		
		double dStaStt	= pBri->GetStationAtSlabEnd(TRUE, bBaseLine ?  0 :(bLeft ? -1 : 1), 0);
		double dStaEnd	= pBri->GetStationAtSlabEnd(FALSE, bBaseLine ?  0 :(bLeft ? -1 : 1), 0);
		CDPoint vAngStt	= pBri->GetAngleByTarget(BRIDGE_OFF_STT_SLAB, 0);
		CDPoint vAngEnd	= pBri->GetAngleByTarget(BRIDGE_OFF_END_SLAB, 0);
		double dCover	= pBri->GetCoverSide_UpperSlab(0);
		
		CDoubleArray dArrSta, dArrStaOut;	// 수직보강철근과 중심선형과 만나는 sta 모음
		CDoubleArray dArrDist, dArrDistText;
		CTwinVectorArray *pTvArr	= NULL;
		tvJiganStt.m_v1 += tvJiganStt.GetXyDir(FALSE) * dCover/vAngStt.y;
		tvJiganStt.m_v2 += tvJiganStt.GetXyDir(TRUE) * dCover/vAngStt.y;
		tvJiganEnd.m_v1 += tvJiganEnd.GetXyDir(FALSE) * dCover/vAngEnd.y;
		tvJiganEnd.m_v2 += tvJiganEnd.GetXyDir(TRUE) * dCover/vAngEnd.y;
	
		if(bBaseLine)
		{
			dArrSta.Add(dStaStt);
			dArrSta.Add(dStaEnd);
		}
		else
		{
			pLineApp->GetStationMatchVerticalByXy(bLeft ? tvJiganStt.m_v1 : tvJiganStt.m_v2, dStaStt);
			pLineApp->GetStationMatchVerticalByXy(bLeft ? tvJiganEnd.m_v1 : tvJiganEnd.m_v2, dStaEnd);
		}

		

		// 가각 없는 형태로 잘라내서 치수 기입함.(그렇지 않으면 치수가 지저분 하게 나옴)
		CTwinVectorArray tvArrPlane1DanNoExp;
		pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane1DanNoExp, 0);
		
		CTwinVectorArray tvArrDim;
		tvArrDim	= tvArr;
		rb.GetTvArrayByCrossTvArrayArea(tvArrDim, tvArrPlane1DanNoExp, TRUE);

		// 사보강배치 절반일 경우 변화구간직철근도 치수를 넣자.
		// 단, 슬래브 좌우측에 포함되어 있는 철근만 치수에 넣어야 함.
		if(pBri->m_nTypePlacingSupportRebar_UpperSlab == 3)
		{
			pTvArr	= &tvArrLeft1Dan[bLeft ? iLEFT : iRIGHT];
			CDPointArray xyArrMatch;
			long i = 0;
			CTwinVector *pTv	= NULL;
			for(i = 0; i < tvArrRAngStt.GetSize(); i++)
			{
				pTv	= tvArrRAngStt.GetPAt(i);
				rb.GetXyMatchTvArrAndLine(*pTvArr, pTv->m_v1, pTv->GetXyDir(), xyArrMatch);
				if(xyArrMatch.GetSize() > 0)
					tvArrDim.Add(pTv);
			}

			for(i = 0; i < tvArrRAngEnd.GetSize(); i++)
			{
				pTv	= tvArrRAngEnd.GetPAt(i);
				rb.GetXyMatchTvArrAndLine(*pTvArr, pTv->m_v1, pTv->GetXyDir(), xyArrMatch);
				if(xyArrMatch.GetSize() > 0)
					tvArrDim.Add(pTv);
			}
		}
		

		pTvArr	= &tvArrDim;
		long nSize	= pTvArr->GetSize();
		if(nSize > 0)
		{
			long i = 0; for(i = 0; i < nSize; i++)
			{
				tv	= pTvArr->GetAt(i);
				double dSta		= 0;
				if(bBaseLine)
				{
					// 중심선형 기준일때는 중심선형에서의 스테이션을 찾는다.
					if(pLine->GetStationMatchByXyAndAngle(tv.m_v1, tv.GetXyDir(), dSta))
						dArrSta.Add(dSta);
				}
				else
				{
					// 좌우측 일때는 좌우측 선형에서 직각인위치.
					if(pLineApp->GetStationMatchVerticalByXy(bLeft ? tv.m_v2 : tv.m_v1, dSta))
						dArrSta.Add(dSta);
				}
			}

			// 피복치수 추가.
			{
				double dSta1(0);	// 아크면 시작점이 반대일 수 있다.
				CDoubleArray dArrStaTmp;
				pTvArr	= &tvArrLeft1Dan[bLeft ? iLEFT : iRIGHT];
				long i = 0;
				
				for(i = 0; i < pTvArr->GetSize(); i++)
				{
					CTwinVector *pTv	= NULL;
					pTv	= pTvArr->GetPAt(i);
					if(bBaseLine)
						dSta1 = dStaStt + pBri->m_dCoverWallOut[0] / vAngStt.y;	// pLine->GetStationMatchByXyAndAngle(pTv->m_v1, tvJiganStt.GetXyDir(), dSta1);
					else
						pLineApp->GetStationMatchVerticalByXy(pTv->m_v1, dSta1);
					dArrStaTmp.Add(dSta1);

					if(i == pTvArr->GetSize()-1)
					{
						if(bBaseLine)
							dSta1 = dStaEnd - pBri->m_dCoverWallOut[0] / vAngEnd.y;	// pLine->GetStationMatchByXyAndAngle(pTv->m_v2, tvJiganEnd.GetXyDir(), dSta1);
						else
							pLineApp->GetStationMatchVerticalByXy(pTv->m_v2, dSta1);
						dArrStaTmp.Add(dSta1);
					}
				}

				dArrStaTmp.Sort(TRUE);

				if(dArrStaTmp.GetSize() > 1)
				{
					dArrSta.Add(dArrStaTmp.GetAt(0));
					dArrSta.Add(dArrStaTmp.GetAt(dArrStaTmp.GetSize()-1));
				}
			}

			// 구조물선 치수 추가
			{
				CTwinVectorArray tvArrLeft[2];
				CTwinVectorArray tvArrPlan;
				pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlan, -1);
				CTwinVector *pTv	= NULL;
				long i = 0; for(i = 0; i < tvArrPlan.GetSize(); i++)
				{
					pTv	= tvArrPlan.GetPAt(i);
					if(pTv->m_sInfo.Find("좌측", 0) != -1) tvArrLeft[iLEFT].Add(*pTv);
					else if(pTv->m_sInfo.Find("우측", 0) != -1) tvArrLeft[iRIGHT].Add(*pTv);
				}

				CTwinVectorArray *pTvArrOut	= &tvArrLeft[bLeft ? iLEFT : iRIGHT];

				if(pTvArrOut->GetSize() > 0)
				{
					double dSta1(0), dSta2(0);	// 아크면 시작점이 반대일 수 있다.
					pTv		= pTvArrOut->GetPAt(0);
					// (ARCBRIDGE-3516) 선형중심 기준 치수일때는 외곽선 치수를 따로 표기한다.
					if(bBaseLine)
					{
						if(pLine->GetStationMatchVerticalByXy(pTv->m_v1, dSta1) && pLine->GetStationMatchVerticalByXy(pTv->m_v2, dSta2))
							dArrStaOut.Add(min(dSta1, dSta2));
					}
					else
					{
						if(pLineApp->GetStationMatchVerticalByXy(pTv->m_v1, dSta1) && pLineApp->GetStationMatchVerticalByXy(pTv->m_v2, dSta2))
							dArrSta.InsertAt(1, min(dSta1, dSta2));
					}

					pTv	= pTvArrOut->GetPAt(pTvArrOut->GetSize()-1);
					if(bBaseLine)
					{
						if(pLine->GetStationMatchVerticalByXy(pTv->m_v1, dSta1) && pLine->GetStationMatchVerticalByXy(pTv->m_v2, dSta2))
							dArrStaOut.Add(max(dSta1, dSta2));
					}
					else
					{
						if(pLineApp->GetStationMatchVerticalByXy(pTv->m_v1, dSta1) && pLineApp->GetStationMatchVerticalByXy(pTv->m_v2, dSta2))
							dArrSta.Add(max(dSta1, dSta2));
					}
				}
			}
			
			
			dArrSta.Sort(TRUE);
			nSize	= dArrSta.GetSize();
			for(i = 1; i < nSize; i++)
			{
				dArrDist.Add(dArrSta[i]-dArrSta[i-1]);
				double dLenText = bBaseLine ? pLine->GetLengthByDis(dArrSta[i-1], dArrSta[i], 0) : pLineApp->GetLengthByDis(dArrSta[i-1], dArrSta[i], 0);
				dArrDistText.Add(dLenText);
			}

			// 치수기입 정보 가져옴
			CDoubleArray dArrDim;
			CStringArray sArrText;
			CStringArray sArrUnderText;
			m_pStd->GetDimInfoAtDistArray(dArrDim, sArrText, sArrUnderText, dArrDist, "", FALSE, TRUE, TRUE, &Dom, dDimLimit, TRUE, TRUE, dArrDistText);
				
			// 치수 기입
			double dStaCur	= dArrSta.GetAt(0);
			dimDom.DimBaseCurvePoint(dStaCur, dDist, nDimDan);

			
			CString str	= _T("");
			CString strUnder	= _T("");
			nSize		= dArrDim.GetSize();
			BOOL bUnderTextPrev	= FALSE;
			for(i = 0; i < nSize; i++)
			{
				double dLen	= 0;

				str			= sArrText.GetAt(i);
				strUnder	= sArrUnderText.GetAt(i);
				dLen		= dArrDim.GetAt(i);
				
				if(i == 0 || i == nSize-1)	// 시작과 끝은 어짜피 피복 치수이므로 밖으로 뺀다.
				{
					if(i == 0)
					{
						dimDom.DimCurveMoveTo(dStaCur, dDist, nL, FALSE, str);
						dimDom.DimCurveLineTo(dLen, dDist, nL, "", "");
					}
					else
					{
						if(!pBri->IsIncludeOnlyLineInBridge())
						{
							str += bUpper ? "(상면), (도로중심치수임)" : "(하면),(도로중심치수임)";
						}
						else
						{
							str += bUpper ? "(상면)" : "(하면)";
						}
						dimDom.DimCurveMoveTo(dStaCur, dDist, nL, FALSE);
						dimDom.DimCurveLineTo(dLen, dDist, nL, str, strUnder, FALSE, FALSE, TRUE);
					}
				}
				else
				{
					dimDom.DimCurveMoveTo(dStaCur, dDist, nL, FALSE);
					if((dLen < dimDom.GetTextWidth(str)) && !bUnderTextPrev)
					{
						dimDom.DimCurveLineTo(dLen, dDist, nL, str, strUnder);
						bUnderTextPrev	= TRUE;
					}
					else
					{
						dimDom.DimCurveLineTo(dLen, dDist, nL, str, strUnder);
						bUnderTextPrev	= FALSE;
					}
				}
				
				dStaCur += dLen;
			}
			
			nDimDan++;

			// 전체 치수
			//dDist	= pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, CDPoint(0, 1), bLeft ? -1:1);
			dDist	= dWidth;
			if(pBri->m_bIsExp)
				dDist += bLeft ? -max(pBri->m_dExpSttH[0], pBri->m_dExpEndH[0]) : max(pBri->m_dExpSttH[1], pBri->m_dExpEndH[1]);
			if(bLeft)
				dDist -= Dom.Always(Dom.GetDimExo()+Dom.GetDimLevelDis()*(nDimDan+1));
			else
				dDist += Dom.Always(Dom.GetDimExo()+Dom.GetDimLevelDis()*(nDimDan+1));
			//if(bLeft) dDist	= -dDist;

			dStaStt	= dArrSta.GetAt(0);
			dStaEnd	= dArrSta.GetAt(dArrSta.GetSize()-1);
			dimDom.DimBaseCurvePoint(dStaStt, dDist, nDimDan);
			dimDom.DimCurveMoveTo(dStaStt, dDist, nL, FALSE);
			dimDom.DimCurveLineTo((dStaEnd-dStaStt), dDist, nL, COMMA(pLineApp->GetLengthByDis(dStaStt, dStaEnd, dWidth)), "");

			if(bBaseLine)
			{
				// 선형중심 치수일때는 구조물 치수를 하나 더 추가해준다.
				if(dArrStaOut.GetSize() > 1)
				{
					if(bLeft)
						dDist -= Dom.Always(Dom.GetDimLevelDis());
					else
						dDist += Dom.Always(Dom.GetDimLevelDis());

					dStaStt	= dArrStaOut.GetAt(0);
					dStaEnd	= dArrStaOut.GetAt(dArrStaOut.GetSize()-1);

					dimDom.DimBaseCurvePoint(dStaStt, dDist, nDimDan);
					dimDom.DimCurveMoveTo(dStaStt, dDist, nL, FALSE);
					dimDom.DimCurveLineTo((dStaEnd-dStaStt), dDist, nL, COMMA(pLine->GetLengthByDis(dStaStt, dStaEnd, dWidth)), "");
				}
			}

			*pDomP << dimDom;
		}
		///////////////////////////////////////////////////////////////////////////////////


		// 사보강 배치 치수 기입(시점측 종점측) //////////////////////////////////////////
		for(long dim = 0; dim < 2; dim++)
		{
			pTvArr	= NULL;
			switch(dim)
			{
				case 0: pTvArr	= &tvArrAngStt; break;
				case 1: pTvArr	= &tvArrAngEnd; break;
				default: break;
			}
			if(!pTvArr) continue;
			long nSize	= pTvArr->GetSize();
			if(nSize == 0) continue;

			dArrDist.RemoveAll();
			dArrSta.RemoveAll();

			// 거리구함(각 철근간의 선형거리로 구함)
			double dSta1	= 0;
			double dSta2	= 0;
//			double dLen		= 0;
			CDPoint xy1		= CDPoint(0, 0);
			CDPoint xy2		= CDPoint(0, 0);

			long i = 0; for(i = 0; i < nSize-1; i++)
			{
				xy1		= bLeft ? pTvArr->GetAt(i).m_v2 : pTvArr->GetAt(i).m_v1;
				xy2		= bLeft ? pTvArr->GetAt(i+1).m_v2 : pTvArr->GetAt(i+1).m_v1;
				dDist	= pLine->GetDisMatchVerticalByXy(xy1);
				pLine->GetStationMatchVerticalByXy(xy1, dSta1);
				pLine->GetStationMatchVerticalByXy(xy2, dSta2);
				double dLen		= 0;
				dLen	= pLine->GetLengthByDis(dSta1, dSta2, dDist);
				dArrDist.Add(dLen);
			}

			// 치수기입 정보 가져옴
			CDoubleArray dArrDim;
			CStringArray sArrText;
			CStringArray sArrUnderText;
			CDoubleArray dArrTmp;
			m_pStd->GetDimInfoAtDistArray(dArrDim, sArrText, sArrUnderText, dArrDist, "", FALSE, TRUE, TRUE, &Dom, dDimLimit, TRUE, TRUE, dArrTmp);
				
			// 치수 기입
			tv		= pTvArr->GetAt(0);
			CDPoint xyStt	= bLeft ? tv.m_v2 : tv.m_v1; 
			pLine->GetStationMatchVerticalByXy(xyStt, dStaStt);

			nDimDan		= 1;
			dDist	= pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, CDPoint(0, 1), bLeft ? -1:1);
			if(pBri->m_bIsExp)
				dDist += bLeft ? max(pBri->m_dExpSttH[0], pBri->m_dExpEndH[0]) : max(pBri->m_dExpSttH[1], pBri->m_dExpEndH[1]);
			dDist += Dom.Always(Dom.GetDimExo()+Dom.GetDimLevelDis()*(nDimDan+1));
			if(bLeft) dDist	= -dDist;

			// 현재 선형 치수 기입시 obq가 적용되진 않지만, 나중에 적용될 것을 고려해서 obq를 설정해 놓음
			//double dObq	= 90-pBri->GetAngleJijum(0).GetAngleDegree();
			//dimDom.SetDimObq(dObq);	// 엔진을 고치려니 너무 오래 걸림.. 그냥 두자

			dimDom.DimBaseCurvePoint(dStaStt, dDist, nDimDan);
			double dStaCur	= dStaStt;

			BOOL bUnderTextPrev	= TRUE;
			for(long j = 0; j < dArrDim.GetSize(); j++)
			{
				double dLen	= dArrDim.GetAt(j);
				CString str	= sArrText.GetAt(j);

				dimDom.DimCurveMoveTo(dStaCur, dDist, 0);
				if(dimDom.GetTextWidth(str) > dLen && !bUnderTextPrev)
				{
					dimDom.DimCurveLineTo(dLen, dDist, 0, "", str);
					bUnderTextPrev	= TRUE;
				}
				else
				{
					dimDom.DimCurveLineTo(dLen, dDist, 0, str, ""); 
					bUnderTextPrev	= FALSE;
				}

				dStaCur += dLen;
			}

			*pDomP << dimDom;
		}
		///////////////////////////////////////////////////////////////////////////////////
		
	}

	BOOL bDrawAll23Dan = m_pStd->IsDrawUpperSlabRebar2DanAll();
	if(bDrawAll23Dan && nDan > 0)
	{
		long icJijum = bUpper? pBri->m_nQtyJigan+1 : pBri->m_nQtyJigan;
		for(long ix=0; ix<icJijum; ++ix)
		{
			pBri->GetTvRebarSupport_Plane_Slab_Std(TRUE, tvArr, tvArrAngStt, tvArrAngEnd, tvArrRAngStt, tvArrRAngEnd, bUpper, nDan, nDrawLeft);

			ApplyCuttingArea_Plane(&Dom, tvArr, bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, ix);
			ApplyCuttingArea_Plane(&Dom, tvArrAngStt, bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, ix);
			ApplyCuttingArea_Plane(&Dom, tvArrAngEnd, bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, ix);
			ApplyCuttingArea_Plane(&Dom, tvArrRAngStt, bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, ix);
			ApplyCuttingArea_Plane(&Dom, tvArrRAngEnd, bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, ix);

			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
			baseDraw.DrawTvArray(&Dom, tvArr);
			baseDraw.DrawTvArray(&Dom, tvArrAngStt);
			baseDraw.DrawTvArray(&Dom, tvArrAngEnd);
			baseDraw.DrawTvArray(&Dom, tvArrRAngStt);
			baseDraw.DrawTvArray(&Dom, tvArrRAngEnd);
		}
	}
	else
	{
		// 절단원 적용시켜서 그림.
		ApplyCuttingArea_Plane(&Dom, tvArr, bUpper, nDan, FALSE, FALSE);
		ApplyCuttingArea_Plane(&Dom, tvArrAngStt, bUpper, nDan, FALSE, FALSE);
		ApplyCuttingArea_Plane(&Dom, tvArrAngEnd, bUpper, nDan, FALSE, FALSE);
		ApplyCuttingArea_Plane(&Dom, tvArrRAngStt, bUpper, nDan, FALSE, FALSE);
		ApplyCuttingArea_Plane(&Dom, tvArrRAngEnd, bUpper, nDan, FALSE, FALSE);

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
		baseDraw.DrawTvArray(&Dom, tvArr);
		baseDraw.DrawTvArray(&Dom, tvArrAngStt);
		baseDraw.DrawTvArray(&Dom, tvArrAngEnd);
		baseDraw.DrawTvArray(&Dom, tvArrRAngStt);
		baseDraw.DrawTvArray(&Dom, tvArrRAngEnd);
	}

	// 배력 철근 마크
	CString sDescription	= _T("");
	if(bUpper)
		sDescription.Format("상부슬래브 상면 %d단 배력철근", nDan+1);
	else
		sDescription.Format("상부슬래브 하면 %d단 배력철근2", nDan+1);
	CString sMark	= pBri->GetMarkRebarSupport(0, FALSE, FALSE, FALSE, sDescription);
	if(bMarkRebar && tvArr.GetSize() > 0)// && m_pStd->IsMarkRebar(sMark) && tvArr.GetSize() > 0)
	{
		CTwinVectorArray tvArrExpSttLAndR, tvArrExpSttLOrR;
		CTwinVectorArray tvArrExpEndLAndR, tvArrExpEndLOrR;
		pBri->DivideRebarSupport_Plane_UpperSlabExpSection(tvArr, tvArrExpSttLAndR, tvArrExpSttLOrR, tvArrExpEndLAndR, tvArrExpEndLOrR);

		// 수직철근에 대해서 ...
		// 수직철근은 모두 마크후에 종점쪽으로 뻗어서 마크한다.
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		if(pBri->m_nTypePlacingMainRebar_UpperSlab == 0)
		{
			CLineInfoApp *pLine	= static_cast<CLineInfoApp*>(pBri->GetLineBase());
			if(!pLine) return;


			// ang == 0 : 기본철근(사보강시 직방향철근)
			// ang == 1 : 시점방향 철근(사방향)
			// ang == 2 : 종점방향 철근(사방향)
			// ang == 3 : 시점방향 철근(직방향변화구간)
			// ang == 4 : 종점방향 철근(직방향변화구간)
			// ang == 5 : 기본철근(시점좌우측가각부)
			// ang == 6 : 기본철근(시점좌나우측가각부)
			// ang == 7 : 기본철근(종점좌우측가각부)
			// ang == 8 : 기본철근(종점좌나우측가각부)
			for(long ang = 0; ang < 9 ; ang++)
			{

				// 가각지원 아니면 가각에 대해서 마크하지 않음
				if(!pBri->m_bIsExp && ang > 4) break;

				CDPointArray xyArrMark;	// countmark할 위치들.

				// 배력근을 마크해야 되는 dist, sta등의 정보를 얻음.
				CTwinVectorArray	tvArrSup;
				CDPoint xyDir	= CDPoint(0, 1);
				double dSta		= pBri->GetStationOnJijum(0);
				double dDistMid	= pBri->GetDistMidOfSlab(dSta);
				double dWid		= pBri->GetWidthSlabAct(dSta, xyDir, 0)/2;
				double dWidLeft	= pBri->GetWidthSlabAct(dSta, xyDir, -1);
				double dWidRight	= pBri->GetWidthSlabAct(dSta, xyDir, 1);
				double dDist	= bUpper ? -dWid/2 : dWid/2;
								
				switch(ang)
				{
				case 0: 
					dDist		= bUpper ? -dWid/2 : dWid/2;
					tvArrSup	= tvArr;
					if(bUpper) sDescription.Format("상부슬래브 상면 %d단 배력철근", nDan+1);
					else sDescription.Format("상부슬래브 하면 %d단 배력철근2", nDan+1);
					break;
				case 1:
					dDist		= bUpper ? -dWid/5 : dWid/5;
					tvArrSup	= tvArrAngStt;
					if(bUpper) sDescription.Format("상부슬래브 상면 %d단 배력철근2", nDan+1);
					else sDescription.Format("상부슬래브 하면 %d단 배력철근2_2", nDan+1);
					break;
				case 2: 
					dDist		= bUpper ? -dWid/5 : dWid/5;
					tvArrSup	= tvArrAngEnd;
					if(bUpper) sDescription.Format("상부슬래브 상면 %d단 배력철근3", nDan+1);
					else sDescription.Format("상부슬래브 하면 %d단 배력철근2_3", nDan+1);
					break;
				case 3: 
					dDist		= bUpper ? -dWid/5*4 : dWid/5*4;
					tvArrSup	= tvArrRAngStt;
					if(bUpper) sDescription.Format("상부슬래브 상면 %d단 배력철근4", nDan+1);
					else sDescription.Format("상부슬래브 하면 %d단 배력철근2_4", nDan+1);
					break;
				case 4: 
					dDist		= bUpper ? -dWid/5*4 : dWid/5*4;
					tvArrSup	= tvArrRAngEnd;
					if(bUpper) sDescription.Format("상부슬래브 상면 %d단 배력철근5", nDan+1);
					else sDescription.Format("상부슬래브 하면 %d단 배력철근2_5", nDan+1);
					break;
				case 5:
					dDist		= bUpper ? -dWid/5*2 : dWid/5*2;
					tvArrSup	= tvArrExpSttLAndR;
					if(bUpper) sDescription.Format("상부슬래브 상면 %d단 배력철근6", nDan+1);
					else sDescription.Format("상부슬래브 하면 %d단 배력철근2_6", nDan+1);
					break;
				case 6:
					dDist		= bUpper ? -dWid/5*3 : dWid/5*3;
					tvArrSup	= tvArrExpSttLOrR;
					if(bUpper) sDescription.Format("상부슬래브 상면 %d단 배력철근7", nDan+1);
					else sDescription.Format("상부슬래브 하면 %d단 배력철근2_7", nDan+1);
					break;
				case 7:
					dDist		= bUpper ? -dWid/5*2 : dWid/5*2;
					tvArrSup	= tvArrExpEndLAndR;
					if(bUpper) sDescription.Format("상부슬래브 상면 %d단 배력철근8", nDan+1);
					else sDescription.Format("상부슬래브 하면 %d단 배력철근2_8", nDan+1);
					break;
				case 8:
					dDist		= bUpper ? -dWid/5*3 : dWid/5*3;
					tvArrSup	= tvArrExpEndLOrR;
					if(bUpper) sDescription.Format("상부슬래브 상면 %d단 배력철근9", nDan+1);
					else sDescription.Format("상부슬래브 하면 %d단 배력철근2_9", nDan+1);
					break;
				}
				
				dDist += (pBri->m_dWidthSlabRight + pBri->m_dWidthSlabLeft) / 4;


				if(tvArrSup.GetSize() == 0) continue;

				// 철근의 가운데
				CTwinVector tvSup	= tvArrSup.GetAt(0);
				
				sMark	= pBri->GetMarkRebarSupport(0, FALSE, FALSE, FALSE, sDescription);
				// (ARCBRIDGE-2725) 이렇게 하면 선형의 도로 중심과 교량 중심이 다른경우 마킹위치를 못잡는 경우가 생긴다.
				if(fabs(dDistMid) < 10)
					dDist	= bUpper ? (dDistMid - dWidLeft/4) : (dDistMid + dWidRight/4);
				else
					dDist	= bUpper ? (dDistMid - dDistMid/4) : (dDistMid + dDistMid/4);
				if(pBri->m_bIsExp)
					dDist += (ang * dWid/36);
				else
					dDist += (ang * dWid/18);

				// 2,3단은 절단원 사이에 마크해야 됨.
				if(nDan != 0)
				{
					CTwinVectorArray tvArrCuttingArea;
					pBri->GetTvCuttingArea_Plane_UpperSlab(tvArrCuttingArea, bUpper, nDan);
					if(tvArrCuttingArea.GetSize() > 1)
					{
						CTwinVector tvCutting	= tvArrCuttingArea[1];
						dDist	= pLine->GetDisMatchVerticalByXy(tvCutting.m_v1+tvCutting.GetLength()/3*tvCutting.GetXyDir());
					}
				}

				if(!pLine->GetStationMatchVerticalByXy(bUpper ? tvArrSup[0].m_v2 : tvArrSup[0].m_v1, dSta)) return;

				// 1cycle과 2cycle 주철근을 구함. /////////////////////////
				// 정확한 dDist를 구하기 위해서(주철근과 겹치지 않도록 하기 위해.)
				// 배력근 마킹은 dSta에서 하지만, 주철근은 2cycle을 반드시 고려 해야 되므로
				// 상면하면에 따라서 반드시 2cycle이 있을 만한 장소의 주철근 간격을 가져온다.
				// 단, 2,3단 하부의 경우 어짜피 선이 트랙을 빠져 나오면서 1단주철근을 거쳐 와야 되므로 1단 1,2cycle
				// 철근도 고려해야 한다.
				CDoubleArray dArrDistMain, dArrDistHunch;
				CDoubleArray dArrDistMain2, dArrDistHunch2;
				CDoubleArray dArrTmp;

				pBri->GetDistRebarMain_CrossSection_Slab(TRUE, dArrDistMain, dArrDistHunch, dArrDistMain2, dArrDistHunch2, dArrTmp, dArrTmp, dArrTmp, dArrTmp, dSta, bUpper, nDan, TRUE, FALSE);
				long i = 0; for(i = 0; i < dArrDistMain2.GetSize(); i++)
					dArrDistMain.Add(dArrDistMain2.GetAt(i));

				// 2,3단 하부의 경우 1단주철근에 대해서도 고려함. //////////
				if(nDan > 0 && !bUpper)
				{
					CDoubleArray dArrDistMain0, dArrDistHunch0;
					CDoubleArray dArrDistMain02, dArrDistHunch02;
					pBri->GetDistRebarMain_CrossSection_Slab(TRUE, dArrDistMain0, dArrDistHunch0, dArrDistMain02, dArrDistHunch02, dArrTmp, dArrTmp, dArrTmp, dArrTmp, dSta, bUpper, 0, TRUE, FALSE);
					
					long i = 0; for(i = 0; i < dArrDistMain0.GetSize(); i++)
						dArrDistMain.Add(dArrDistMain0[i]);
					for(i = 0; i < dArrDistMain02.GetSize(); i++)
						dArrDistMain.Add(dArrDistMain02[i]);
				}
				/////////////////////////////////////////////////////////////

				// 마크할 위치 찾음
 				GetXyCountMarkSupport_Plane_Slab(xyArrMark, tvArrSup, bUpper, dDist, dArrDistMain);

				// 마크할 위치에 대해서 절단원 적용
				ApplyCuttingArea_Plane(&Dom, xyArrMark, bUpper, nDan, FALSE, FALSE);

				// count mark하고 선을 그음.
				CDPoint xyRotate	= pBri->GetXyDirRotateDomByBridgeSttEndXy();
				CDPoint xyDirLine	= pLine->GetAngleAzimuth(dSta);
				CConcSymbol sym(m_pStd->m_pARoadOptionStd);
				for(i = 0; i < xyArrMark.GetSize(); i++)
					sym.RebarCountMarkOne(&Dom, xyArrMark.GetAt(i), xyDirLine);
			
				long nSize	= xyArrMark.GetSize();
				if(nSize > 0)
				{
					double dStaCur	= dSta;
					CDPoint xyStt	= xyArrMark.GetAt(0);
					CDPoint xyEnd	= xyArrMark.GetAt(nSize-1);
					// 1단일 경우 종점측으로 뻗고, 2,3단은 시점측으로 뻗는다.
					// 단, 1단이면서 시점방향 철근이면 시점으로 뻗는다.
					BOOL bExtendToStt	= nDan == 0 && (ang == 1 || ang == 3 || ang == 5 || ang == 6);
					if(!bExtendToStt)
					{
						dStaCur	= pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, FALSE);//pBri->m_dStationBridgeStt+pBri->m_dLengthBridge+pBri->m_dWE+pBri->m_dBWE;
						xyDir	= pBri->GetAngleByTarget(BRIDGE_OFF_END_SLAB, 0);// >GetAngleJijum(pBri->m_nQtyJigan);
					}
					else
					{
						dStaCur	= pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, FALSE);// pBri->m_dStationBridgeStt-pBri->m_dWS;
						GetSwap(xyStt, xyEnd);
						xyDir	= pBri->GetAngleByTarget(BRIDGE_OFF_STT_SLAB, 0);//pBri->GetAngleJijum(0);
					}

					// 사방향 배치는 마크위치 구할때 부터 사각 적용하므로 여기선 하지 않음
					if(pBri->m_nTypePlacingSupportRebar_UpperSlab == 2)
						xyDir	= pBri->GetAngleJijum(0);

					dDist	= pLine->GetDisMatchVerticalByXy(xyStt);
					xyEnd	= pLine->GetXyLineByLengthAndAngle(dStaCur, dDist/xyDir.y, xyDir);
					

					double dStaStt(0), dStaEnd(0);
					pLine->GetStationMatchVerticalByXy(xyStt, dStaStt);
					pLine->GetStationMatchVerticalByXy(xyEnd, dStaEnd);
					double dDist	= pLine->GetDisMatchVerticalByXy(xyStt);
					
					pLine->DrawLS(&Dom, dStaStt, dStaEnd, dDist);

					xyEnd	= pLine->GetXyLineDis(dStaEnd, dDist);
					
					// 마킹함.
					*pDomP << Dom;

					long upper	= bUpper ? iUPPER : iLOWER;
					CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_SupportRebar_UpperSlab[upper][nDan];
					CString sDia	= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(ePartUpperSlab));
					// sym.RebarMarkLineSide(&Dom, xyEnd, sMark, sDia, bExtendToStt, 0, FALSE);

					CRebar *pRb = pBri->GetRebarSupportByDescription(sDescription);
					SRebarMarkInputData mark_input_data;
					SRebarMarkviewData mark_view_data;
					mark_input_data.mark = sMark;
					mark_input_data.dia = sDia;
					mark_input_data.position = xyEnd;
					mark_input_data.ctc = pBri->GetCTCRebarSupport_Slab(TRUE, bUpper, nDan, TRUE);
					mark_input_data.type_rebar = pRb? pRb->m_nTypeRebar : -1;
					mark_input_data.left_dir = bExtendToStt;
					mark_input_data.height = 0;

					mark_view_data.count_mark = FALSE;
					mark_view_data.line_rebar = TRUE;
					mark_view_data.mark_pos = eMarkSide;

					sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

					CDPoint xyDirLine	= pLine->GetAngleAzimuth(dStaCur);
					Dom.Rotate(xyEnd, xyDirLine);
					*pDomP << Dom;
				}

			}
		}
		else if(pBri->m_nTypePlacingMainRebar_UpperSlab == 1 || pBri->m_nTypePlacingMainRebar_UpperSlab == 2)
		{
			// (34554) 8번째 인자인 Index를 0을주면 주철근 위치를 못찾고 마킹이 날라가는 경우가 있다. -1을줘서 전체 주철근을 찾자.
			CTwinVectorArray tvArrMain1, tvArrMain2, tvArrMain, tvArrAngleLeft, tvArrAngleRight;	// 주철근 좌표
			pBri->GetTvRebarMain_Plane_UpperSlab_TypeNormal_Std(tvArrMain1, tvArrAngleLeft, tvArrAngleRight, bUpper, 0, 0, 0, -1);
			pBri->GetTvRebarMain_Plane_UpperSlab_TypeNormal_Std(tvArrMain2, tvArrAngleLeft, tvArrAngleRight, bUpper, 1, 0, 0, -1);
			
			if(tvArrMain2.GetSize() > 0)
			{
				long i = 0; for(i = 0; i < tvArrMain1.GetSize(); i++)
				{
					tvArrMain.Add(tvArrMain1.GetAt(i));
					if(i < tvArrMain2.GetSize())
						tvArrMain.Add(tvArrMain2.GetAt(i));
				}
			}
			else
				tvArrMain	= tvArrMain1;


			long upper	= bUpper ? iUPPER : iLOWER;
			CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_SupportRebar_UpperSlab[upper][nDan];
			
			// ang == 0 : 기본철근(사보강시 직방향철근)
			// ang == 1 : 시점방향 철근(사방향)
			// ang == 2 : 종점방향 철근(사방향)
			// ang == 3 : 시점방향 철근(직방향변화구간)
			// ang == 4 : 종점방향 철근(직방향변화구간)
			double dPosScale	= 0.5;
			for(long ang = 0; ang < 5; ang++)
			{
				CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
				switch(ang)
				{
				case 0:
					if(bUpper) sDescription.Format("상부슬래브 상면 %d단 배력철근", nDan+1);
					else sDescription.Format("상부슬래브 하면 %d단 배력철근2", nDan+1);
					mark.SetRebar(tvArr, tvArrMain);
					dPosScale	= 0.5;
					break;
				case 1:
					if(bUpper) sDescription.Format("상부슬래브 상면 %d단 배력철근2", nDan+1);
					else sDescription.Format("상부슬래브 하면 %d단 배력철근2_2", nDan+1);
					mark.SetRebar(tvArrAngStt, tvArrMain);
					dPosScale	= 0.2;
					break;
				case 2:
					if(bUpper) sDescription.Format("상부슬래브 상면 %d단 배력철근3", nDan+1);
					else sDescription.Format("상부슬래브 하면 %d단 배력철근2_3", nDan+1);
					mark.SetRebar(tvArrAngStt, tvArrMain);
					dPosScale	= 0.8;
					break;
				}
				sMark	= pBri->GetMarkRebarSupport(0, FALSE, FALSE, FALSE, sDescription);
				CRebar *pRb = pBri->GetRebarSupportByDescription(sDescription);

				// 설정
				mark.m_sMark					= sMark;
				mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(ePartUpperSlab));
				mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_END;
				mark.m_dPosScale				= dPosScale;
				if(tvArrMain.GetSize() > 0)
				{
					CDPoint xyMark				= mark.GetXyMark();
					CDPoint xyMatch				= xyMark;

					CTwinVectorArray tvArrPlane, tvArrEnd;
					CTwinVector tv;
					pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1);
					long i = 0; for(i = 0; i < tvArrPlane.GetSize(); i++)
					{
						tv	= tvArrPlane.GetAt(i);
						if(tv.m_sInfo == "종점") 
							tvArrEnd.Add(tv);
					}

					for(i = 0; i < tvArrEnd.GetSize(); i++)
					{
						tv	= tvArrEnd.GetAt(i);
						if(GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, xyMark, tvArrMain.GetAt(0).GetXyDir(), xyMatch))
							break;
					}
					// BHWE를 더하는 이유? 
					// 브라켓있는 경우 브라켓도 감안을 해줘야 하므로...
					mark.m_dAddHeight			= ~(xyMark - xyMatch) + pBri->m_dBHWE;
				}
				mark.m_dCtc						= pBri->GetCTCRebarSupport_Slab(TRUE, bUpper, nDan, TRUE);
				mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;

				mark.MarkRebar();
			}
		}

	}


	*pDomP << Dom;
}

// rebarinfocycle 하나 치수 기입(주철근)
// 방법은 ....
// 1. 해당 인포 사이클의 중간 sta구함
// 2. 배력철근 sta들 모두 구함
// 3. 중간 sta에서 가장 가까운 배력철근 sta 구함.
// 4. 해서 prev와 next 배력철근 sta 구함.
// 5. 거기서 원하는 개수만큼 마크함.
// BOOL bUpper : 상면철근인지 하면철근인지만 구별하면 됨.
// BOOL bHunch : 헌치주철근인지?
// 보완할 점 1 : 신축이음 사이 쯤에는 배력근이 배근 되지 않아 철근 마크할 sta를 정상적으로
// 찾지 못 해서 countmark를 제대로 하지 못 하는 경우가 있다.
// 반드시 깔끔하게 보완해야 됨 - 2004.12.28 이재호
// 보완할 점 2 : 주철근마크 위치를 잡을때 총 사이클 수와 각 사이클별 철근의 길이를 고려해서 위치를 등간격으로 잡으려면
// 상당히 많은 알고리즘과 코드가 필요함. 현재는 단순히 사이클별 비율로 위치를 잡게 해놔서 철근 길이가 각각 다르면 이상한 위치에 잡혀서
// 약간 문제가 있다. 그러나 이는 큰 문제는 아니라고 판단됨(일반적인 초기작업이 끝나는 데로 위치를 잡기 위해 노력하겠음)
void CDrawPlane::DimRebarInfoCycle_Plane_UpperSlab(CDomyun *pDomP, CRebarInfoCycle *pRC, BOOL bUpper, long nDan, long nCycle, BOOL bGirder, long nDrawLeft, BOOL bSttBracket, BOOL bEndBracket, BOOL bHunch, long nDirMark, BOOL bApplyCutArea)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(!pRC) return;
	CLineInfo *pLine	= pBri->GetLineBase();
	if(!pLine) return;

	// 에러 방지를 위해 헌치부일 경우 무조건 하부 ctc로 설정한다.
	if(bHunch)
	{
		bUpper	= FALSE;
		nDan	= 0;
	}
	else if(bSttBracket || bEndBracket)
	{
		bUpper	= TRUE;
	}

	// 해당 rc의 중간 sta구함
	double dStaStt	= pRC->m_xyStt.x;	// rc의 시작 sta
	double dStaEnd	= pRC->m_nPosStt != pRC->m_nPosEnd ? pRC->m_xyEnd.x : pRC->m_xyMid1.x;	// rc의 끝 sta
	if(pRC->m_bUseTvArrRebar)
	{
		dStaStt	= pRC->m_tvArrRebar.GetXyLeft().x;
		dStaEnd	= pRC->m_tvArrRebar.GetXyRight().x;
	}
	if(dStaStt > dStaEnd) GetSwap(dStaStt, dStaEnd);

	// 마크할 대략의 위치 찾기 /////////////////////////////////////
	//long nJijum = pBri->GetJijumNumberByStation(dStaStt);
	double dStaMid	= dStaStt + (dStaEnd-dStaStt)/4.0*1.5;	// 중간 sta
	if(bGirder || bSttBracket || bEndBracket || bHunch)
		dStaMid	= dStaStt + (dStaEnd-dStaStt)/5.0*2.0;
	else if(!bGirder && !bHunch && nCycle == 2)
	{
		dStaMid	= dStaStt + (dStaEnd-dStaStt)/4.0*2.5;
	}

	if(bHunch)
	{
		// 헌치 철근 위치가 벗어나서 마킹이 안된다.
		dStaMid	= dStaStt + (dStaEnd-dStaStt)/4.0*1.5;
	}

	if(!bGirder && !bSttBracket && !bEndBracket && !bHunch && (nCycle == 1 || nCycle == 3))
	{
		// 2cycle일때는 좌측에서 1/8 위치
		if(nCycle == 1)
			dStaMid	= dStaStt + (dStaEnd-dStaStt)/8*2;
		// 4Cycle일때는 좌측에서 7/8 위치
		if(nCycle == 3)
			dStaMid	= dStaStt + (dStaEnd-dStaStt)/8*6;

		// 찾은 대략의 위치가 하필이면 슬래브교의 신축이음 사이일때
		// 그러면 주철근이 없어서 마크가 제대로 되지 않는다.
		double dStaSttExtJoint	= 0;
		double dStaEndExtJoint	= 0;
		long i = 0; for(i = 0; i < pBri->m_nQtyJigan+1; i++)
		{
			CJijum *pJ	= pBri->GetJijum(i);
			if(pJ && pBri->m_bExpJoint[i])
			{
				dStaSttExtJoint	= pBri->GetStationOnJijum(i, 0);
				dStaEndExtJoint	= pBri->GetStationOnJijum(i, 0);
				dStaSttExtJoint -= pJ->m_dLengthUgan[0]-pBri->GetCoverSide_UpperSlab(0);
				dStaEndExtJoint	+= pJ->m_dLengthUgan[1]+pBri->GetCoverSide_UpperSlab(0);

				// 신축이음 사이이면..
				if(dStaMid >= dStaSttExtJoint && dStaMid <= dStaEndExtJoint)
				{
					if(nCycle == 1)
						dStaMid	= dStaEndExtJoint + 1000;
					else if(nCycle == 3)
						dStaMid	= dStaSttExtJoint - 1000;
				}
			}
		}
		///////////////////////////////////////////////////////////
	}

	// 배력철근 sta들 모두 구함.
	CDoubleArray dArrStaSup;
	
	if(bGirder)
	{
	}
	else if(bSttBracket || bEndBracket)
	{
		BOOL bStt	= bSttBracket ? TRUE : FALSE;
		pBri->GetStaRebarSupport_UpperSlabBracket(dArrStaSup, bStt);
	}
	else if(bHunch)
	{
		dArrStaSup	= pBri->m_dArrStaSupRebar[iLOWER];
		if(dArrStaSup.GetSize() == 0)
			pBri->GetStaRebarSupport_VertSection_SlabHunch(TRUE, dArrStaSup);
		// 헌치 철근 위치에 있는 Station만 걸러낸다.
		for(long ix=0; ix< dArrStaSup.GetSize(); ++ix)
		{
			double dSta = dArrStaSup.GetAt(ix);
			if(dSta < dStaStt || dSta > dStaEnd)
			{
				dArrStaSup.RemoveAt(ix, 1);
				--ix;
			}
		}
	}
	else
	{
		dArrStaSup	= pBri->m_dArrStaSupRebar[bUpper ? iUPPER : iLOWER];
		if(dArrStaSup.GetSize() == 0)
			pBri->GetStaRebarSupport_VertSection_Slab(dArrStaSup, bUpper, nDan);
	}

	// 중간 sta에서 가장 가까운 배력철근 sta 구해서 prev와 next를 구함.
	// 위치가 어중간해서 못 찾으면 그냥 마크함
	BOOL bFind		= FALSE;
	double dSta		= 0;
	double dStaPrev	= dStaMid;
	double dStaNext	= dStaMid;
	long nIdxPrev	= 0;
	long nIdxNext	= 0;
	long nSize		= dArrStaSup.GetSize();
	long i = 0; for(i = 0; i < nSize; i++)
	{
		dSta	= dArrStaSup[i];
		if(dSta < dStaMid)
		{
			dStaPrev	= dSta;
			nIdxPrev	= i;
		}
		else
		{
			dStaNext	= dSta;
			nIdxNext	= i;
			bFind		= TRUE;
			break;
		}
	}

	if(!bFind && nSize > 1)
	{
		dStaPrev	= dStaStt;
		dStaNext	= dStaEnd;
		nIdxPrev	= nSize-2;	// 배력근의 각도를 따라서 마크해야 되므로 배력근 인덱스는 잇어야 된다.
		nIdxNext	= nSize-1;
		bFind	= TRUE;
	}

	if(dStaPrev > dStaNext) GetSwap(dStaNext, dStaPrev);
	double dStaMark	= bFind ? dStaPrev + (dStaNext-dStaPrev)/2 : dStaMid;
	
	// 헌치부 sta를 구하는 경우 이전헌치에서 다음 헌치로 바로 넘어오는 경우가 있다. ////
	// 그럴 경우를 대비해서 마크위치가 철근을 벗어나면 다음 철근 sta으로 옮기자
	if(bHunch && dArrStaSup.GetSize() > 0)
	{
		if(!(dStaStt <= dStaMark && dStaEnd >= dStaMark))
		{
			if(nIdxNext+1 < nSize)
			{
				nIdxPrev++;
				nIdxNext++;

				dStaPrev	= dArrStaSup.GetAt(nIdxPrev);
				dStaNext	= dArrStaSup.GetAt(nIdxNext);

				dStaMark	= dStaPrev + (dStaNext-dStaPrev)/2;
			}
			else
			{
				dStaMark	= dStaMid;
			}
		}
		// 헌치 철근은 수평철근 갯수가 몇개 안되기 때문에 마킹이 몰린다. 시종점 부 헌치의 경우에는 마킹 위치를 바깥쪽으로 옮겨주자.
		long nJHunch	= pRC->m_nPosHunch;
		if(nJHunch == 0 || nJHunch == pBri->m_nQtyJigan)
		{
			dStaMark = dArrStaSup.GetAt(nJHunch == 0 ? 0 : (dArrStaSup.GetSize()-1)) + (nJHunch == 0 ? -300 : 300);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////

	double dDistMid	= pBri->GetDistMidOfSlab(dStaMid);


	// 주철근 거리 구함.
	CDoubleArray dArrDistMain, dArrDistHunch;
	CDoubleArray dArrDistMainTmp[MAX_REBAR_CYCLE], dArrDistHunchTmp[MAX_REBAR_CYCLE];
	pBri->GetDistRebarMain_CrossSection_Slab(TRUE, dArrDistMainTmp[0], dArrDistHunchTmp[0], dArrDistMainTmp[1], dArrDistHunchTmp[1], dArrDistMainTmp[2], dArrDistHunchTmp[2], dArrDistMainTmp[3], dArrDistHunchTmp[3], dStaMark, bUpper, nDan);
	dArrDistMain	= dArrDistMainTmp[nCycle];
	dArrDistHunch	= dArrDistHunchTmp[nCycle];
	dArrDistMain.Sort(FALSE);
	dArrDistHunch.Sort(FALSE);
	
	// 시종점 헌치는 주철근과 같다. -> (39755) 시종점 현치도 헌치 거리 고려... 
	// 평면도 일반도에 헌치선 표현요청.. 이건 작업량이 많기 때문에 나중에..
	// if(bHunch && !(nJijum == 0 || nJijum == pBri->m_nQtyJigan))
	if(bHunch)
		dArrDistMain	= dArrDistHunch;

	// 마크함
	// nDrawLeft가 -1이면 거리가 음수인거만, nDrawLeft가 1이면 거리가 양수인거만, nDrawLeft가 0이면 모두 마크함.
	CConcSymbol sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CDPoint xyDirRotate	= pBri->GetXyDirRotateDomByBridgeSttEndXy();
	CDPoint xyDirMark	= CDPoint(0, 1);	
	CDPoint xyMarkStt	= CDPoint(0, 0);
	CDPoint xyMarkEnd	= CDPoint(0, 0);
	BOOL bMarkFirst		= TRUE;
	CDPoint xyMark		= CDPoint(0, 0);
	

	// 헌치부이거나 브라켓부일 경우 마크 각도는 지점 각도와 같다.
	BOOL bDrawAll23Dan = m_pStd->IsDrawUpperSlabRebar2DanAll();
	long nJ	= pRC->m_nPosHunch;
	if(bDrawAll23Dan && !bGirder && !bSttBracket && !bEndBracket && !bHunch)
	{
		nJ = pBri->GetJijumNumberByStation(dStaMark);
	}

	if(bGirder || bSttBracket || bEndBracket || bHunch)
	{
		xyDirMark	= pBri->GetAngleJijum(nJ);
	}
	else
	{
		long nDir	= pBri->GetDirSupportRebar(dStaMark, bUpper);
		switch(nDirMark)
		{
			case 0: nDir	= -1; break;
			case 1: nDir	= 1; break;
		}

		if(nDir == -1)		xyDirMark	= pLine->GetAngleByOffset(pBri->m_dStationBridgeStt, dStaMark, pBri->GetAngleJijum(0));
		else if(nDir == 1)	xyDirMark	= pLine->GetAngleByOffset(pBri->m_dStationBridgeStt+pBri->m_dLengthBridge, dStaMark, pBri->GetAngleJijum(pBri->m_nQtyJigan));
		else				xyDirMark	= CDPoint(0, 1);
	}

	CDPointArray xyArrMark;	// 마크할 위치를 저장해서 슬래브 안쪽에 있는 것만 표현한다.
	nSize	= dArrDistMain.GetSize();
	// 철근 마크 방향으로 거리를 계산할때
	// 사보강이면 마크방향으로 직각인 거리가적용된다.
	CDPoint xyDirLen	= xyDirMark;
	// 거더, 브라켓, 헌치 주철근은 선형모양이 아니기 때문에 일반 마크하듯이 좌표를 구해야된다.
	if(bGirder)/// || bSttBracket || bEndBracket || bHunch)
	{

	}
	else
	{
		for(i = 0; i < nSize; i++)
		{
			double dDist		= 0;
			dDist	= dArrDistMain[i];
			if(nDrawLeft == -1 && dDist >= dDistMid) continue;
			if(nDrawLeft == 1 && dDist <= dDistMid) continue;

			double dStaTmp	= pLine->GetStationByScInLine(dStaMark, dDist, xyDirMark, 0);
			xyMark	= pLine->GetXyLineByLengthAndAngle(dStaTmp, dDist, CDPoint(0, 1));

			if(bMarkFirst) 
			{
				xyMarkStt	= xyMark;
				bMarkFirst	= FALSE;
			}
			else
				xyMarkEnd	= xyMark;
			xyArrMark.Add(xyMark);
		}
	}

	// 마크한줄을 만들었으면 슬래브 안쪽에 있는것만 골라낸다. /////
	CRebarPlacing rb;
	CTwinVectorArray tvArrPlane;
	CVectorArray vecArrArea;
	pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, 0);
	tvArrPlane.GetToVectorArray(vecArrArea);
	rb.GetXyByVectorArrayArea(xyArrMark, vecArrArea, TRUE);

	// 절단원에 대해서 적용함.
	if(bApplyCutArea)
		ApplyCuttingArea_Plane(&Dom, xyArrMark, bUpper, nDan, bGirder, FALSE, bSttBracket, bEndBracket, bHunch, nJ);

	if(xyArrMark.GetSize() == 0) return;
	///////////////////////////////////////////////////////////////

	// 마크한다.
	CDPoint xyDirLine	= pLine->GetAngleAzimuth(dStaMark);
	if(pBri->GetAngleByStation(dStaMark).GetAngleDegree() > 90)
		xyDirLine	= xyDirLine.Rotate(CDPoint(0, 1));
	nSize	= xyArrMark.GetSize();
	for(i = 0; i < nSize; i++)
		sym.RebarCountMarkOne(&Dom, xyArrMark[i], xyDirLine);

	BOOL bLeftDir	= (bHunch && nJ == pBri->m_nQtyJigan) ? TRUE : FALSE;//nCycle==0 || nCycle==2 ? TRUE : FALSE;
	CString sMark	= pRC->m_szMark;
	CString sDia	= m_pStd->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pBri->GetValueFy(ePartUpperSlab));
	if((nDan == 0 || (bDrawAll23Dan && (nJ != 0 && nJ != pBri->m_nQtyJigan))) && !bSttBracket && !bEndBracket)
	{
		// xyMarkEnd-xyMarkStt 하면 방위각이 나온다.
		// 그러나 선형에 적용하기 위해선 선형에 대한 상대각이 필요함
		CDPoint xyDirMarkLine		= xyDirMark;	// 선형에 적용하는 각도
		CDPoint xyDirMarkAzimuth	= xyDirMark;	// 실제 방위각
		if(bUpper)
		{
			xyDirMarkAzimuth	= (xyMarkEnd - xyMarkStt).Unit();
			xyDirMarkLine		= xyDirMark;

			double dWid	= pBri->m_bIsExp ? pBri->GetWidthSlabActByGagak(dStaMark, xyDirMarkLine, -1) + MARK_BASEHEIGHT_UPPER : 
											pBri->GetWidthSlabAct(dStaMark, xyDirMarkLine, -1) + MARK_BASEHEIGHT_UPPER;
			xyMarkEnd	= pLine->GetXyLineByLengthAndAngle(dStaMark, -dWid, xyDirMarkLine);
	//		xyMarkEnd	+= MARK_BASEHEIGHT_UPPER * xyDirMarkAzimuth;	// 선형에 적용하지 않는 각도는 직접 선으로 구해야 됨

			Dom.LineTo(xyMarkStt, xyMarkEnd);
			*pDomP << Dom;

			// sym.RebarMarkLineSide(&Dom, xyMarkEnd, sMark, sDia, bLeftDir, 0, FALSE, "", FALSE);
			CRebar *pRb = pBri->GetRebarMainByDescription(pRC->m_sDescription);
			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = sMark;
			mark_input_data.dia = sDia;
			mark_input_data.position = xyMarkEnd;
			mark_input_data.ctc = pBri->GetCtcRebarMain(TRUE);
			mark_input_data.type_rebar = pRb? pRb->m_nTypeRebar : -1;
			mark_input_data.left_dir = bLeftDir;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.line_rebar = TRUE;
			mark_view_data.mark_pos = eMarkSide;
			mark_view_data.apply_basic_height = FALSE;

			sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

			pLine->GetStationMatchVerticalByXy(xyMarkEnd, dSta);
			CDPoint xyDirAzimuth	= pLine->GetAngleAzimuth(dSta);
			Dom.Rotate(xyMarkEnd, ToDPointFrDegree((xyDirAzimuth*-1).GetAngleDegree()-180));
		}
		else
		{
			if(bHunch && !bApplyCutArea && (nJ == 0 || nJ == pBri->m_nQtyJigan))
			{
				bLeftDir = nJ == 0 ? TRUE : FALSE;
				long nIndexPos = xyArrMark.GetSize() / 2;
				xyMarkEnd	= (xyArrMark.GetAt(nIndexPos) + xyArrMark.GetAt(nIndexPos+1)) / 2.;
				double dDist = pLine->GetDisMatchVerticalByXy(xyMarkEnd);
				double dSta = pBri->GetStationAtSlabEnd(nJ == 0, 0) + Dom.Always(10) * (nJ == 0 ? -1 : 1);
				xyMarkStt	= pLine->GetXyLineDisByAng(dSta, dDist, pBri->GetAngleJijum(nJ));

				if(nSize > 0)
				{
					Dom.LineTo(xyArrMark[0], xyArrMark[nSize-1]);
				}
			}
			else
			{
				xyMarkEnd	= xyArrMark.GetAt(xyArrMark.GetSize()-1);

				xyDirMarkAzimuth	= (xyMarkStt - xyMarkEnd).Unit();
				xyDirMarkLine		= xyDirMark;

				double dWid	= pBri->m_bIsExp ? pBri->GetWidthSlabActByGagak(dStaMark, xyDirMarkLine, 1) : 
					pBri->GetWidthSlabAct(dStaMark, xyDirMarkLine, 1);
				if(bHunch)
					dWid	+= MARK_BASEHEIGHT_UPPER;
				else
					dWid	+= MARK_BASEHEIGHT_LOWER;
				xyMarkStt	= pLine->GetXyLineByLengthAndAngle(dStaMark, dWid, xyDirMarkLine);
			}

			Dom.LineTo(xyMarkStt, xyMarkEnd);
			*pDomP << Dom;
			// sym.RebarMarkLineSide(&Dom, xyMarkStt, sMark, sDia, bLeftDir, 0, FALSE, "", FALSE);

			CRebar *pRb = pBri->GetRebarMainByDescription(pRC->m_sDescription);
			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = sMark;
			mark_input_data.dia = sDia;
			mark_input_data.position = xyMarkStt;
			mark_input_data.ctc = pBri->GetCtcRebarMain(TRUE);
			mark_input_data.type_rebar = pRb? pRb->m_nTypeRebar : -1;
			mark_input_data.left_dir = bLeftDir;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.line_rebar = TRUE;
			mark_view_data.mark_pos = eMarkSide;
			mark_view_data.apply_basic_height = FALSE;

			sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

			pLine->GetStationMatchVerticalByXy(xyMarkStt, dSta);
			CDPoint xyDirAzimuth	= pLine->GetAngleAzimuth(dSta);
			Dom.Rotate(xyMarkStt, ToDPointFrDegree((xyDirAzimuth*-1).GetAngleDegree()-180));
		}
	}
	else	// 2,3단은 시점측으로 뻗어서 마크 기입
	{	
		// 마크를 시점측으로 뻗을지 종점으로 뻗을지?
		BOOL bExtendStt	= TRUE;
		if(bSttBracket || bEndBracket)
			bExtendStt	= FALSE;
		else if(bDrawAll23Dan) 
		{
			bExtendStt = (nJ == 0)? TRUE : FALSE;
		}

		long nSize	= xyArrMark.GetSize();
		if(nSize > 1)
		{
			CHgBaseDrawStd baseDraw;
			baseDraw.m_pLine	= pLine;
			CDPoint xyDir	= pBri->GetAngleJijum(0);

			// count mark를 잇는 선
			Dom.LineTo(xyArrMark[0], xyArrMark[nSize-1]);

			// count mark의 중심에서 철근마크까지 잇는 선.
			if(bExtendStt)
			{
				dStaStt	= pBri->m_dStationBridgeStt-pBri->m_dWS;
			}
			else
			{
				dStaStt	= pBri->m_dStationBridgeStt+pBri->m_dLengthBridge+pBri->m_dWE + 1000;
			}
			

			long nIdx	= nSize/2;
			if(nIdx == 1) nIdx	= 0;
			CDPoint xyPrev	= xyArrMark.GetAt(nIdx),
					xyNext	= xyArrMark.GetAt(nIdx+1);
			// 철근 마크 하는 위치는 2cycle지원일때와 4cycle지원일때가 달라야 된다.
			CDPoint xyMark	= xyPrev+(xyNext-xyPrev).Unit()*(~(xyPrev-xyNext))/4;
			if(!bSttBracket && !bEndBracket)
			{
				if(pBri->GetCountCycleMainRebar() == 4)
					xyMark	= xyPrev+(xyNext-xyPrev).Unit()*(~(xyPrev-xyNext))/4.0*1.5;
			}
			else
			{
				xyMark	= xyPrev+(xyNext-xyPrev).Unit()*(~(xyPrev-xyNext))/2;
			}

			double dDist	= pLine->GetDisMatchVerticalByXy(xyMark);
			CDPoint xyStt	= pLine->GetXyLineByLengthAndAngle(dStaStt, dDist/xyDir.y, xyDir);

			CTwinVectorArray tvArr;
			CTwinVector tv;
			tv.m_v1	= xyStt;
			tv.m_v2	= xyMark;
			tv.m_v1.z	= 4;
			tvArr.Add(tv);
			baseDraw.DrawTvArray(&Dom, tvArr);
						
			// 철근 마크 하기.
			*pDomP << Dom;

			xyMark	= xyStt;
			// sym.RebarMarkLineSide(&Dom, xyMark, sMark, sDia, bExtendStt ? TRUE : FALSE, 0, FALSE, "", FALSE);

			CRebar *pRb = pBri->GetRebarMainByDescription(pRC->m_sDescription);
			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = sMark;
			mark_input_data.dia = sDia;
			mark_input_data.position = xyMark;
			mark_input_data.ctc = pBri->GetCtcRebarMain(TRUE);
			mark_input_data.type_rebar = pRb? pRb->m_nTypeRebar : -1;
			mark_input_data.left_dir = bExtendStt ? TRUE : FALSE;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.line_rebar = TRUE;
			mark_view_data.mark_pos = eMarkSide;
			mark_view_data.apply_basic_height = FALSE;

			sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

			pLine->GetStationMatchVerticalByXy(xyMark, dSta);
			CDPoint xyDirAzimuth	= pLine->GetAngleAzimuth(dSta);
			Dom.Rotate(xyMark, ToDPointFrDegree((xyDirAzimuth*-1).GetAngleDegree()-180));
		}
	}
		
	*pDomP << Dom;
}

// 평면도 상부슬래브 철근 그리기
// BOOL bRotate : 슬래브를 수평으로 회전할지..
// BOOL bForSlabStandard : 슬래브 표준도용인지?
CDRect CDrawPlane::DrawRebar_Plane_UpperSlabStd(CDomyun *pDomP, long nCycle, BOOL bUpperLeft, BOOL bUpperRight, BOOL bDimRebar, BOOL bMarkRebar, BOOL bRotate, long nDirMark, BOOL bForSlabStandard, BOOL bDrawHunchRebar)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return CDRect(0, 0, 0, 0);
	m_pStd->SetMarkForMarkRebar("");
	m_bApplyCutAreaHunch = bDrawHunchRebar;

	CDomyun Dom(pDomP);

	long nCycleStt	= nCycle == -1 ? 0 : nCycle;
	long nCycleEnd	= nCycle == -1 ? pBri->GetCountCycleMainRebar() : nCycle+1;
	
	CLineInfo *pLine    = pBri->GetLineBase();
	CHgBaseDrawStd baseDraw;
	// 평면도 일반도
	CTwinVectorArray tvArrPlane;
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1, TRUE);

	// 평면도 일반도는 브라켓 부분 고려해서 그려야 된다.
	CTwinVectorArray tvArrCuttingBracket;
	pBri->GetTvCuttingArea_Plane_UpperSlabBracket(tvArrCuttingBracket);
	baseDraw.DrawTvArrayApplyOverlapShape(&Dom, tvArrPlane, tvArrCuttingBracket);

	// (39755) 하면 표현할때 헌치 위치 표현하기
	if(bUpperLeft == FALSE)
	{
		CTwinVectorArray tvArrLeft;
		tvArrPlane.GetTvArrByInfo(_T("좌측"), tvArrLeft, FALSE, FALSE, FALSE);
		if(tvArrLeft.GetSize() > 0)
		{
			tvArrLeft.Sort();
			CTwinVector tvStt, tvEnd;
			CTwinVectorArray tvArrTemp, tvArrTemp2;
			tvArrPlane.GetTvArrByInfo(_T("시점"), tvArrTemp, FALSE, FALSE, FALSE);
			tvArrTemp.GetTvArrByInfo(_T("가각"), tvArrTemp2, TRUE , FALSE, FALSE); 
			tvStt.m_v1 = tvArrTemp.GetAt(0).m_v1;
			tvStt.m_v2 = tvArrTemp.GetAt(tvArrTemp.GetSize()-1).m_v2;

			tvArrPlane.GetTvArrByInfo(_T("종점"), tvArrTemp, FALSE, FALSE, FALSE);
			tvArrTemp.GetTvArrByInfo(_T("가각"), tvArrTemp2, TRUE , FALSE, FALSE);
			tvEnd.m_v1 = tvArrTemp.GetAt(0).m_v1;
			tvEnd.m_v2 = tvArrTemp.GetAt(tvArrTemp.GetSize()-1).m_v2;

			CRebarPlacing rb;
			// CTwinVectorArray::AddFromLineSplit() 함수의 문제로 시종점이 바뀌어 있는 경우가 있다.
			if(~(tvStt.GetXyMid() - tvArrLeft.GetAt(0).m_v1) > ~(tvStt.GetXyMid() - tvArrLeft.GetAt(tvArrLeft.GetSize()-1).m_v2))
			{
				rb.ReverseRebar(tvArrLeft);
			}
			rb.GetOffsetTvArray(tvArrLeft, pBri->m_dDH[iLEFT]);

			CTwinVectorArray tvArrCopy;
			tvArrCopy.AddFromTvArray(tvArrLeft);

			double dExt = 5000;
			double dStaSttOri(0), dStaEndOri(0), dStaSttCurr(0), dStaEndCurr(0);


			pLine->GetStationMatchVerticalByXy(tvArrLeft.GetAt(0).m_v1, dStaSttOri);
			pLine->GetStationMatchVerticalByXy(tvArrLeft.GetAt(tvArrLeft.GetSize()-1).m_v2, dStaEndOri);

			rb.StretchTvArr(tvArrLeft, dExt,dExt);
			rb.TrimTvArrayByTv(tvArrLeft, &tvStt, TRUE);
			rb.TrimTvArrayByTv(tvArrLeft, &tvEnd, TRUE);

			pLine->GetStationMatchVerticalByXy(tvArrLeft.GetAt(0).m_v1, dStaSttCurr);
			pLine->GetStationMatchVerticalByXy(tvArrLeft.GetAt(tvArrLeft.GetSize()-1).m_v2, dStaEndCurr);
			dStaSttCurr -= 500;		
			dStaEndCurr += 500;
			if(Compare(dStaSttOri, dStaSttCurr, _T("<"),10) || Compare(dStaEndOri, dStaEndCurr, _T(">"),10))
			{
				tvArrLeft.RemoveAll();
				tvArrLeft.AddFromTvArray(tvArrCopy);

				CTwinVector *pTv = tvArrLeft.GetPAt(0);
				GetXyMatchLineAndLine(tvStt.m_v1, tvStt.GetXyDir(), pTv->m_v2, pTv->GetXyDir(), pTv->m_v1);
				pTv = tvArrLeft.GetPAt(tvArrLeft.GetSize()-1);
				GetXyMatchLineAndLine(tvEnd.m_v1, tvEnd.GetXyDir(), pTv->m_v1, pTv->GetXyDir(), pTv->m_v2);
			}

			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONH);
			baseDraw.DrawTvArrayApplyOverlapShape(&Dom, tvArrLeft, tvArrCuttingBracket);
		}
	}
	if(bUpperRight == FALSE)
	{
		CTwinVectorArray tvArrRight;
		tvArrPlane.GetTvArrByInfo(_T("우측"), tvArrRight, FALSE, FALSE, FALSE);

		if(tvArrRight.GetSize() > 0)
		{
			CTwinVector tvStt, tvEnd;
			CTwinVectorArray tvArrTemp, tvArrTemp2;
			tvArrPlane.GetTvArrByInfo(_T("시점"), tvArrTemp, FALSE, FALSE, FALSE);
			tvArrTemp.GetTvArrByInfo(_T("가각"), tvArrTemp2, TRUE, FALSE, FALSE);
			tvStt.m_v1 = tvArrTemp.GetAt(0).m_v1;
			tvStt.m_v2 = tvArrTemp.GetAt(tvArrTemp.GetSize()-1).m_v2;

			tvArrPlane.GetTvArrByInfo(_T("종점"), tvArrTemp, FALSE, FALSE, FALSE);
			tvArrTemp.GetTvArrByInfo(_T("가각"), tvArrTemp2, TRUE, FALSE, FALSE);
			tvEnd.m_v1 = tvArrTemp.GetAt(0).m_v1;
			tvEnd.m_v2 = tvArrTemp.GetAt(tvArrTemp.GetSize()-1).m_v2;

			CRebarPlacing rb;
			tvArrRight.Sort();
			// CTwinVectorArray::AddFromLineSplit() 함수의 문제로 시종점이 바뀌어 있는 경우가 있다.
			if(~(tvStt.GetXyMid() - tvArrRight.GetAt(0).m_v1) > ~(tvStt.GetXyMid() - tvArrRight.GetAt(tvArrRight.GetSize()-1).m_v2))
			{
				rb.ReverseRebar(tvArrRight);
			}
			rb.GetOffsetTvArray(tvArrRight, -pBri->m_dDH[iRIGHT]);

			CTwinVectorArray tvArrCopy;
			tvArrCopy.AddFromTvArray(tvArrRight);

			double dExt = 5000;
			double dStaSttOri(0), dStaEndOri(0), dStaSttCurr(0), dStaEndCurr(0);


			pLine->GetStationMatchVerticalByXy(tvArrRight.GetAt(0).m_v1, dStaSttOri);
			pLine->GetStationMatchVerticalByXy(tvArrRight.GetAt(tvArrRight.GetSize()-1).m_v2, dStaEndOri);

			rb.StretchTvArr(tvArrRight, dExt,dExt);
			rb.TrimTvArrayByTv(tvArrRight, &tvStt, TRUE);
			rb.TrimTvArrayByTv(tvArrRight, &tvEnd, TRUE);

			pLine->GetStationMatchVerticalByXy(tvArrRight.GetAt(0).m_v1, dStaSttCurr);
			pLine->GetStationMatchVerticalByXy(tvArrRight.GetAt(tvArrRight.GetSize()-1).m_v2, dStaEndCurr);
			dStaSttCurr -= 500;		
			dStaEndCurr += 500;
			if(Compare(dStaSttOri, dStaSttCurr, _T("<"),10) || Compare(dStaEndOri, dStaEndCurr, _T(">"),10))
			{
				tvArrRight.RemoveAll();
				tvArrRight.AddFromTvArray(tvArrCopy);

				CTwinVector tv = tvArrRight.GetAt(0);
				GetXyMatchLineAndLine(tvStt.m_v1, tvStt.GetXyDir(), tv.m_v1, tv.GetXyDir(), tv.m_v1);
				tvArrRight.SetAt(0, tv);
				tv = tvArrRight.GetAt(tvArrRight.GetSize()-1);
				GetXyMatchLineAndLine(tvEnd.m_v1, tvEnd.GetXyDir(), tv.m_v2, tv.GetXyDir(), tv.m_v2);
				tvArrRight.SetAt(tvArrRight.GetSize()-1, tv);
			}

			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONH);
			baseDraw.DrawTvArrayApplyOverlapShape(&Dom, tvArrRight, tvArrCuttingBracket);
		}
	}

	CDRect rectTot;
	if(bRotate)
	{
		m_pStd->RotateDomByBridgeSttEndXy(&Dom);
		Dom.SetCalcExtRect();
		rectTot	= Dom.GetExtRect();
		*pDomP << Dom;
	}


	// 중심선
	double dLenExt	= Dom.Always(Dom.GetDimExo());
	if(bUpperLeft != bUpperRight)
		DrawPlaneCenterLineEachSlab(&Dom, dLenExt, FALSE, FALSE);	

	// (ARCBRIDGE-3517) 일반도용 선형중심선도 추가해준다.
	DrawPlaneCenterLineEachSlab(&Dom, dLenExt, FALSE, TRUE);


	// 배근도 구하기 전 준비 작업 (속도 개선을 위한 중복작업 일괄 처리 )
	pBri->SettingValueRebarForSlab();
	////////////////////////////////////////////////////////////////////
	
	long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		// 평면도 주철 주철근
		for(long cycle = nCycleStt; cycle < nCycleEnd; cycle++)
		{
			if(bUpperLeft != bUpperRight)
			{
				DrawRebarMain_Plane_UpperSlab(&Dom, bUpperLeft, dan, cycle, -1, dan==0&&cycle==0&&bDimRebar, bMarkRebar, dan==0&&cycle==0&&bDimRebar, nDirMark);
				DrawRebarMain_Plane_UpperSlab(&Dom, bUpperRight, dan, cycle, 1, dan==0&&cycle==0&&TRUE, TRUE, dan==0&&cycle==0&&TRUE, nDirMark);
			}
			else
			{
				DrawRebarMain_Plane_UpperSlab(&Dom, bUpperLeft, dan, cycle, 0, dan==0&&cycle==0&&bDimRebar, bMarkRebar, dan==0&&cycle==0&&bDimRebar, nDirMark);
			}
		}
		
		// 가각부 주철근
		if(bUpperLeft != bUpperRight)
		{
			DrawRebarMain_Plane_Slab_Exp(&Dom, TRUE, bUpperLeft, dan, dan==0, dan==0, -1);
			DrawRebarMain_Plane_Slab_Exp(&Dom, TRUE, bUpperRight, dan, dan==0, dan==0, 1);
		}
		else
		{
			DrawRebarMain_Plane_Slab_Exp(&Dom, TRUE, bUpperLeft, dan, dan==0, dan==0, 0);
		}
		

		if(!bForSlabStandard)
		{
			// 평면도 배력철근
			if(bUpperLeft != bUpperRight)
			{
				DrawRebarSupport_Plane_UpperSlab(&Dom, bUpperLeft, dan, -1, dan==0&&bDimRebar, bMarkRebar);
				DrawRebarSupport_Plane_UpperSlab(&Dom, bUpperRight, dan, 1, dan==0&&bDimRebar, bMarkRebar);
			}
			else
			{
				DrawRebarSupport_Plane_UpperSlab(&Dom, bUpperLeft, dan, 0, dan==0&&bDimRebar, bMarkRebar);
			}
		}
	}
	
	// 평면도 거더철근/헌치부철근 배력/주철근
	// 교대,교각부, 기둥부 거더 철근 모두 그림
	for(long nJ = 0; nJ < pBri->GetCountJijum(); nJ++)
	{
		// 거더부 철근 (배력철근)
		DrawRebarSupport_Plane_UpperSlabGirderAndHunch(&Dom, nJ, TRUE, 0, TRUE, FALSE, FALSE, -1);

		// 거더부 철근 (주철근)
		DrawRebarMain_Plane_SlabGirder(&Dom, TRUE, nJ, FALSE, FALSE, -1);

		// 헌치부 철근 (배력철근)
		DrawRebarSupport_Plane_UpperSlabGirderAndHunch(&Dom, nJ, bUpperRight, 0, FALSE, FALSE, bMarkRebar, 1);

		// 헌치부 철근 (주철근)
    	for(long cycle = nCycleStt; cycle < nCycleEnd; cycle++)
   			DrawRebarMain_Plane_UpperSlabHunch(&Dom, cycle, nJ, bUpperRight, 0, FALSE, bMarkRebar, 0, nDirMark);
	}

	// 종거더
	long left = 0; for(left = 0; left < 2; left++)
	{
		BOOL bLeft	= left == iLEFT;
		for(long gagak = 0; gagak < 2; gagak++)
		{
			BOOL bGagak	= gagak == 1;
			long stt = 0; for(stt = 0; stt < 2; stt++)
			{
				BOOL bStt	= stt == iSTT;
				if(!bGagak)
				{
					if(stt == 1) continue;
				}

				DrawRebarSupport_Plane_UpperSlabGirderJong(&Dom, bStt, bLeft, bGagak, TRUE, FALSE, TRUE, FALSE);
				DrawRebarStirrup_Plane_UpperSlabGirderJong(&Dom, bStt, bLeft, bGagak, FALSE, TRUE, FALSE);
			}
		}
	}

	// 평면도 브라켓부 일반도와 철근
	if(!bForSlabStandard)
	{
		DrawPlaneGenBracket(&Dom, FALSE);
		DrawRebarMain_Plane_UpperSlabBracket(&Dom, FALSE, FALSE, TRUE, -1);
		DrawRebarSupport_Plane_UpperSlabBracket(&Dom, FALSE, FALSE, TRUE, -1);

		// 평면도 전단철근
		DrawRebarShear_Plane_UpperSlab(&Dom, -1);

		// 내측벽체와 헌치선 평면도(점선)
		DrawPlaneGenInWallAndHunch(&Dom);
	}

  
	if(bRotate)
		m_pStd->RotateDomByBridgeSttEndXy(&Dom);
	
	*pDomP << Dom;

	// 배근도 구하기 전 준비 작업한거 초기화 (속도 개선을 위한 중복작업 일괄 처리 )
	pBri->InitValueRebarForUpperSlab();
	////////////////////////////////////////////////////////////////////


	return rectTot;
}

void CDrawPlane::ApplyCuttingArea_Plane(CDomyun *pDomP, CTwinVectorArrayArray *pTvArr, BOOL bUpper, long nDan, BOOL bGirder, BOOL bDrawCuttingArea, BOOL bSttBracekt, BOOL bEndBracket, BOOL bHunch, long nJ, BOOL bJongGirder, BOOL bGagak, CLineInfo *pLine, BOOL bUpperSlab)
{
	if(!pTvArr) return;
	long i = 0; for(i = 0; i < pTvArr->GetSize(); i++)
	{
		CTwinVectorArray tvArr;
		tvArr	= *pTvArr->GetAt(i);
		ApplyCuttingArea_Plane(pDomP, tvArr, bUpper, nDan, bGirder, bDrawCuttingArea, bSttBracekt, bEndBracket, bHunch, nJ, bJongGirder, bGagak, pLine, bUpperSlab);
		*pTvArr->GetAt(i) = tvArr;
	}
}
// 상부슬래브 평면도 철근에 절단원을 적용시킨다.
// BOOL bGirder : 거더부(기둥식의 처음 벽체)
// BOOL bHunch : 헌치부(종점벽체나, 벽식의 처음 벽체)
// BOOL bJongGirder : 종거더(bUpper -> bLeft)
void CDrawPlane::ApplyCuttingArea_Plane(CDomyun *pDomP, CTwinVectorArray &tvArr, BOOL bUpper, long nDan, BOOL bGirder, BOOL bDrawCuttingArea, BOOL bSttBracekt, BOOL bEndBracket, BOOL bHunch, long nJ, BOOL bJongGirder, BOOL bGagak, CLineInfo *pLine, BOOL bUpperSlab)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(bGagak) return;

	CDomyun Dom(pDomP);
	long envType	= m_pStd->m_pARoadOptionStd->GetEnvType(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	BOOL bDrawAll23Dan = m_pStd->IsDrawUpperSlabRebar2DanAll();
	CRebarPlacing rb;
	rb.m_pLine	= pLine ? pLine : pBri->GetLineBase();

	// 커팅라인이 살아 있어야 되는 부분 영역 구함 ////////////
	BOOL bCuttingByPlane	= FALSE;	// 헌치부나 거더부에 대해서만 평면을 기준으로 잘라낸다.
	CTwinVectorArray tvArrPlane;
	pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1, TRUE);
	tvArrPlane.Sort();
	//rb.GetOffsetTvArray(tvArrPlane, -Dom.Always(5));
	//////////////////////////////////////////////////////////

	// 1단일 경우 2,3단의 모든 cuttingArea를 잘라낸다. ////////////////////////////
	// 2,3단일 경우 해당 cuttingArea만 그린다.
	
	CHgBaseDrawStd baseDraw;
	if(nDan == 0 && !bGirder && !bSttBracekt && !bEndBracket && !bHunch && !bJongGirder)
	{
		// 잘라내야될 영역 개수
		// 2, 3단, 거더부(지점개수), 브라켓, 헌치(지점개수), 종거더(2개, 가각제외)
		long nCount23Dan	= bDrawAll23Dan? (bUpper? pBri->m_nQtyJigan + 1 : pBri->m_nQtyJigan) : 1;
		long nCountJijum	= pBri->m_nQtyJigan + 1;
		long nCountCutting	= MAX_REBAR_DAN*nCount23Dan+nCountJijum+1+nCountJijum+2;
		long nJOfGirder		= 0;
		long nJOfHunch		= 0;
		BOOL bLeftJongGirder	= TRUE;

		for(long cut = 1; cut < nCountCutting; cut++)
		{
			CTwinVectorArray tvArrCuttingArea;
			bCuttingByPlane	= FALSE;

			if(cut < MAX_REBAR_DAN * nCount23Dan)	// 2,3단 철근인지?
			{
				if(nCount23Dan > 1)
				{
					long nCutDan = cut%MAX_REBAR_DAN;
					long nPosDan = cut/MAX_REBAR_DAN;
					if(nCutDan == 0) continue;

					pBri->GetTvCuttingArea_Plane_UpperSlab(tvArrCuttingArea, bUpper, nCutDan, nPosDan);
				}
				else
				{
					pBri->GetTvCuttingArea_Plane_UpperSlab(tvArrCuttingArea, bUpper, cut);
				}
			}
			else if(cut < MAX_REBAR_DAN * nCount23Dan + nCountJijum)	// 거더부 철근인지?
			{
				bCuttingByPlane	= TRUE;
				pBri->GetTvCuttingArea_Plane_UpperSlabGirder(tvArrCuttingArea, nJOfGirder++);
			}
			else if(cut < MAX_REBAR_DAN * nCount23Dan + nCountJijum+1)	// 브라켓 또는 부상방지저판
			{
				if(bUpperSlab)
					pBri->GetTvCuttingArea_Plane_UpperSlabBracket(tvArrCuttingArea);
				else
					pBri->GetTvCuttingArea_Plane_LowerSlabPRF(tvArrCuttingArea);
			}
			else if(cut < MAX_REBAR_DAN * nCount23Dan + nCountJijum+1+nCountJijum)	// 헌치
			{
				if(m_bApplyCutAreaHunch)
				{
					bCuttingByPlane	= TRUE;
					pBri->GetTvCuttingArea_Plane_SlabHunch(tvArrCuttingArea, nJOfHunch++, bUpperSlab);
				}
				else
				{
					bCuttingByPlane	= FALSE;
				}
			}
			else if(cut < MAX_REBAR_DAN * nCount23Dan + nCountJijum+1+nCountJijum+2)
			{
				bCuttingByPlane	= TRUE;
				pBri->GetTvCuttingArea_Plane_UpperSlabGirder(tvArrCuttingArea, 0, TRUE, bLeftJongGirder, bGagak, TRUE);
				bLeftJongGirder	= FALSE;
			}

			if(tvArrCuttingArea.GetSize() > 0)
			{
				rb.GetTvArrayByCrossTvArrayArea(tvArr, tvArrCuttingArea, FALSE);
				if(bDrawCuttingArea)
				{
					m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);

					if(bCuttingByPlane)
					{
						rb.GetTvArrayByCrossTvArrayArea(tvArrCuttingArea, tvArrPlane, TRUE);
						tvArrCuttingArea.Sort();

						StretchCurringArea(tvArrCuttingArea, Dom.Always(5));
					}
					else
					{
						long nIdx	= tvArrCuttingArea.GetIdxByInfo("그릴때제거1");
						if(nIdx > -1)
							tvArrCuttingArea.RemoveAt(nIdx);
						nIdx	= tvArrCuttingArea.GetIdxByInfo("그릴때제거2");
						if(nIdx > -1)
							tvArrCuttingArea.RemoveAt(nIdx);
						nIdx	= tvArrCuttingArea.GetIdxByInfo("그릴때제거3");
						if(nIdx > -1)
							tvArrCuttingArea.RemoveAt(nIdx);
						nIdx	= tvArrCuttingArea.GetIdxByInfo("그릴때제거4");
						if(nIdx > -1)
							tvArrCuttingArea.RemoveAt(nIdx);
					}
					baseDraw.DrawTvArray(&Dom, tvArrCuttingArea, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());
				}
			}
		}
	}
	else
	{
		// 2, 3단일 경우는 cuttingArea안만 골라 내야 되는데.
		CTwinVectorArray tvArrCuttingArea;
		if(!bGirder && !bSttBracekt && !bEndBracket && !bHunch && !bJongGirder)
		{
			if(bDrawAll23Dan)
			{
				pBri->GetTvCuttingArea_Plane_UpperSlab(tvArrCuttingArea, bUpper, nDan, nJ);
			}
			else
			{
				pBri->GetTvCuttingArea_Plane_UpperSlab(tvArrCuttingArea, bUpper, nDan);
			}
		}
		else if(bGirder)	// 거더부인지.?
		{
			bCuttingByPlane	= TRUE;
			pBri->GetTvCuttingArea_Plane_UpperSlabGirder(tvArrCuttingArea, nJ);
		}
		else if(bSttBracekt || bEndBracket) // 브라켓
		{
			if(bUpperSlab)
				pBri->GetTvCuttingArea_Plane_UpperSlabBracket(tvArrCuttingArea);
			else
				pBri->GetTvCuttingArea_Plane_LowerSlabPRF(tvArrCuttingArea);
		}
		else if(bHunch) // 헌치
		{
			if(m_bApplyCutAreaHunch)
			{
				bCuttingByPlane	= TRUE;
				pBri->GetTvCuttingArea_Plane_SlabHunch(tvArrCuttingArea, nJ, bUpperSlab);
			}
			else
			{
				bCuttingByPlane	= FALSE;
			}
		}
		else if(bJongGirder)
		{
			bCuttingByPlane	= TRUE;
			BOOL bStt	= TRUE;
			BOOL bLeft	= bUpper;
			if(bGagak)
			{
				bStt	= nJ == 0;
			}
			pBri->GetTvCuttingArea_Plane_UpperSlabGirder(tvArrCuttingArea, nJ, bStt, bLeft, bGagak, TRUE);
		}

		// 2,3단은 항상 원안에 그려야 되므로 영역이 없다면 아예 삭제.
 		if(tvArrCuttingArea.GetSize() == 0)
		{
			tvArr.RemoveAll();
		}
		else 
		{
			rb.GetTvArrayByCrossTvArrayArea(tvArr, tvArrCuttingArea, TRUE);
			if(bDrawCuttingArea)
			{
				m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);

				if(bCuttingByPlane)
				{
					rb.GetTvArrayByCrossTvArrayArea(tvArrCuttingArea, tvArrPlane, TRUE);
					StretchCurringArea(tvArrCuttingArea, Dom.Always(5));
				}
				else
				{
					long nIdx	= tvArrCuttingArea.GetIdxByInfo("그릴때제거1");
					if(nIdx > -1)
						tvArrCuttingArea.RemoveAt(nIdx);
					nIdx	= tvArrCuttingArea.GetIdxByInfo("그릴때제거2");
					if(nIdx > -1)
						tvArrCuttingArea.RemoveAt(nIdx);
					nIdx	= tvArrCuttingArea.GetIdxByInfo("그릴때제거3");
					if(nIdx > -1)
						tvArrCuttingArea.RemoveAt(nIdx);
					nIdx	= tvArrCuttingArea.GetIdxByInfo("그릴때제거4");
					if(nIdx > -1)
						tvArrCuttingArea.RemoveAt(nIdx);
				}
				baseDraw.DrawTvArray(&Dom, tvArrCuttingArea, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////////

	*pDomP << Dom;
	m_pStd->m_pARoadOptionStd->SetEnvType(pDomP, envType);
}

// 상부슬래브 평면도 철근에 절단원을 적용시킨다.
// BOOL bGirder : 거더부(기둥식의 처음 벽체)
// BOOL bHunch : 헌치부(종점벽체나, 벽식의 처음 벽체)
void CDrawPlane::ApplyCuttingArea_Plane(CDomyun *pDomP, CDPointArray &xyArr, BOOL bUpper, long nDan, BOOL bGirder, BOOL bDrawCuttingArea, BOOL bSttBracekt, BOOL bEndBracket, BOOL bHunch, long nJ, BOOL bJongGirder, BOOL bGagak, CLineInfo *pLine, BOOL bUpperSlab)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	long envType	= m_pStd->m_pARoadOptionStd->GetEnvType(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	BOOL bDrawAll23Dan = m_pStd->IsDrawUpperSlabRebar2DanAll();
	CRebarPlacing rb;
	rb.m_pLine	= pLine ? pLine : pBri->GetLineBase();

	// 커팅라인이 살아 있어야 되는 부분 영역 구함 ////////////
	BOOL bCuttingByPlane	= FALSE;	// 헌치부나 거더부에 대해서만 평면을 기준으로 잘라낸다.
	CTwinVectorArray tvArrPlane;
	pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1, TRUE);
	tvArrPlane.Sort();
	//rb.GetOffsetTvArray(tvArrPlane, -Dom.Always(5));
	//////////////////////////////////////////////////////////


	// 1단일 경우 2,3단의 모든 cuttingArea를 잘라낸다. ////////////////////////////
	// 2,3단일 경우 해당 cuttingArea만 그린다.
	CHgBaseDrawStd baseDraw;
	if(nDan == 0 && !bGirder && !bSttBracekt && !bEndBracket && !bHunch)
	{
		// 잘라내야될 영역 개수
		// 2, 3단, 거더부(지점개수), 브라켓, 헌치(지점개수), 종거더(2개, 가각제외)
		long nCount23Dan	= bDrawAll23Dan? (bUpper? pBri->m_nQtyJigan + 1 : pBri->m_nQtyJigan) : 1;
		long nCountJijum	= pBri->m_nQtyJigan + 1;
		long nCountCutting	= MAX_REBAR_DAN*nCount23Dan+nCountJijum+1+nCountJijum+2;
		long nJOfGirder		= 0;
		long nJOfHunch		= 0;
		BOOL bLeftJongGirder	= TRUE;

		for(long cut = 1; cut < nCountCutting; cut++)
		{
			CTwinVectorArray tvArrCuttingArea;
			bCuttingByPlane	= FALSE;

			if(cut < MAX_REBAR_DAN * nCount23Dan)	// 2,3단 철근인지?
			{
				if(nCount23Dan > 1)
				{
					long nCutDan = cut%MAX_REBAR_DAN;
					long nPosDan = cut/MAX_REBAR_DAN;
					if(nCutDan == 0) continue;

					pBri->GetTvCuttingArea_Plane_UpperSlab(tvArrCuttingArea, bUpper, nCutDan, nPosDan);
				}
				else
				{
					pBri->GetTvCuttingArea_Plane_UpperSlab(tvArrCuttingArea, bUpper, cut);
				}
			}
			else if(cut < MAX_REBAR_DAN * nCount23Dan + nCountJijum)	// 거더부 철근인지?
			{
				bCuttingByPlane	= TRUE;
				pBri->GetTvCuttingArea_Plane_UpperSlabGirder(tvArrCuttingArea, nJOfGirder++);
			}
			else if(cut < MAX_REBAR_DAN * nCount23Dan + nCountJijum+1)	// 브라켓
			{
				if(bUpperSlab)
					pBri->GetTvCuttingArea_Plane_UpperSlabBracket(tvArrCuttingArea);
				else
					pBri->GetTvCuttingArea_Plane_LowerSlabPRF(tvArrCuttingArea);
			}
			else if(cut < MAX_REBAR_DAN * nCount23Dan + nCountJijum+1+nCountJijum)	// 헌치
			{
				if(m_bApplyCutAreaHunch)
				{
					bCuttingByPlane	= TRUE;
					pBri->GetTvCuttingArea_Plane_SlabHunch(tvArrCuttingArea, nJOfHunch++, bUpperSlab);
				}
				else
				{
					bCuttingByPlane	= TRUE;
				}
			}
			else if(cut < MAX_REBAR_DAN * nCount23Dan + nCountJijum+1+nCountJijum+2)
			{
				if(bUpperSlab)
				{
					bCuttingByPlane	= TRUE;
					pBri->GetTvCuttingArea_Plane_UpperSlabGirder(tvArrCuttingArea, 0, TRUE, bLeftJongGirder, bGagak, TRUE);
					bLeftJongGirder	= FALSE;
				}
				else
				{
					tvArrCuttingArea.RemoveAll();
				}
			}

			if(tvArrCuttingArea.GetSize() > 0)
			{
				CVectorArray vecArea;
				tvArrCuttingArea.GetToVectorArray(vecArea);

				rb.GetXyByVectorArrayArea(xyArr, vecArea, FALSE);
				if(bDrawCuttingArea)
				{
					m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);

					if(bCuttingByPlane)
					{
						rb.GetTvArrayByCrossTvArrayArea(tvArrCuttingArea, tvArrPlane, TRUE);
						StretchCurringArea(tvArrCuttingArea, Dom.Always(5));
					}
					else
					{
						long nIdx	= tvArrCuttingArea.GetIdxByInfo("그릴때제거1");
						if(nIdx > -1)
							tvArrCuttingArea.RemoveAt(nIdx);
						nIdx	= tvArrCuttingArea.GetIdxByInfo("그릴때제거2");
						if(nIdx > -1)
							tvArrCuttingArea.RemoveAt(nIdx);
						nIdx	= tvArrCuttingArea.GetIdxByInfo("그릴때제거3");
						if(nIdx > -1)
							tvArrCuttingArea.RemoveAt(nIdx);
						nIdx	= tvArrCuttingArea.GetIdxByInfo("그릴때제거4");
						if(nIdx > -1)
							tvArrCuttingArea.RemoveAt(nIdx);
					}
					baseDraw.DrawTvArray(&Dom, tvArrCuttingArea, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());
				}
			}
		}
	}
	else
	{
		// 2, 3단일 경우는 cuttingArea안만 골라 내야 되는데.
		CTwinVectorArray tvArrCuttingArea;
		if(!bGirder && !bSttBracekt && !bEndBracket && !bHunch && !bJongGirder)
		{
			if(bDrawAll23Dan)
			{
				pBri->GetTvCuttingArea_Plane_UpperSlab(tvArrCuttingArea, bUpper, nDan, nJ);
			}
			else
			{
				pBri->GetTvCuttingArea_Plane_UpperSlab(tvArrCuttingArea, bUpper, nDan);
			}
		}
		else if(bGirder)	// 거더부인지.?
		{
			bCuttingByPlane	= TRUE;
			pBri->GetTvCuttingArea_Plane_UpperSlabGirder(tvArrCuttingArea, nJ);
		}
		else if(bSttBracekt || bEndBracket)	// 브라켓
		{
			pBri->GetTvCuttingArea_Plane_UpperSlabBracket(tvArrCuttingArea);
		}
		else if(bHunch)	// 헌치
		{
			if(m_bApplyCutAreaHunch)
			{
				bCuttingByPlane	= TRUE;
				pBri->GetTvCuttingArea_Plane_SlabHunch(tvArrCuttingArea, nJ, bUpperSlab);
			}
			else
			{
				bCuttingByPlane	= TRUE;
			}
		}
		else if(bJongGirder)
		{
			bCuttingByPlane	= TRUE;
			BOOL bStt	= TRUE;
			BOOL bLeft	= bUpper;
			if(bGagak)
			{
				bStt	= nJ == 0;
			}
			pBri->GetTvCuttingArea_Plane_UpperSlabGirder(tvArrCuttingArea, nJ, bStt, bLeft, bGagak, TRUE);
		}

		// 2,3단은 항상 원안에 그려야 되므로 영역이 없다면 아예 삭제.
		if(tvArrCuttingArea.GetSize() == 0)
		{
			xyArr.RemoveAll();
		}
		else
		{
			CVectorArray vecArea;
			tvArrCuttingArea.GetToVectorArray(vecArea);

			rb.GetXyByVectorArrayArea(xyArr, vecArea, TRUE);
			if(bDrawCuttingArea)
			{
				m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);

				if(bCuttingByPlane)
				{
					rb.GetTvArrayByCrossTvArrayArea(tvArrCuttingArea, tvArrPlane, TRUE);
					StretchCurringArea(tvArrCuttingArea, Dom.Always(5));
				}
				else
				{
					long nIdx	= tvArrCuttingArea.GetIdxByInfo("그릴때제거1");
					if(nIdx > -1)
						tvArrCuttingArea.RemoveAt(nIdx);
					nIdx	= tvArrCuttingArea.GetIdxByInfo("그릴때제거2");
					if(nIdx > -1)
						tvArrCuttingArea.RemoveAt(nIdx);
					nIdx	= tvArrCuttingArea.GetIdxByInfo("그릴때제거3");
					if(nIdx > -1)
						tvArrCuttingArea.RemoveAt(nIdx);
					nIdx	= tvArrCuttingArea.GetIdxByInfo("그릴때제거4");
					if(nIdx > -1)
						tvArrCuttingArea.RemoveAt(nIdx);
				}
				baseDraw.DrawTvArray(&Dom, tvArrCuttingArea, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////////

	*pDomP << Dom;
	m_pStd->m_pARoadOptionStd->SetEnvType(pDomP, envType);
}

// 평면도 상부 슬래브 거더 철근 그리기(상부거더는 주철근, 헌치부는 배력철근에 해당됨)
// 들어가는 그림 : 거더상부1단배력근, 헌치부 배력근
// long nDrawLeft : -1 - 진행방향 좌측만 그림, 0 - 진행방향 좌우측 모두 그림, 1 - 진행방향 우측만 그림
// 해당 지점에 대해서만 그림
void CDrawPlane::DrawRebarSupport_Plane_UpperSlabGirderAndHunch(CDomyun *pDomP, long nJ, BOOL bUpper, long nDan, BOOL bGirder, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft, BOOL bApplyCutArea)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);

	// draw == 0 : 거더상부1단 배력근 
	// draw == 1 : 헌치부 배력근
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CTwinVectorArray tvArr;
	if(bGirder)
	{
		pBri->GetTvRebarSupport_Plane_SlabGirder(tvArr, TRUE, nJ, nDan, bUpper, nDrawLeft);
	}
	else
	{
		pBri->GetTvRebarSupport_Plane_SlabHunch(tvArr, TRUE, nDrawLeft, nJ);
	}

	if(tvArr.GetSize() == 0) return;


	// 치수 기입
	if(bDimRebar)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	}

	// 절단원 적용해서 그림 
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	if(bApplyCutArea)
		ApplyCuttingArea_Plane(&Dom, tvArr, TRUE, 0, bGirder, FALSE, FALSE, FALSE, !bGirder, nJ);
	baseDraw.DrawTvArray(&Dom, tvArr);

	
	// 헌치부 배력근에서 복사해서 약간 수정한 코드임.
	// (39675) 슬래브 헌치 배력철근인데 왜 HS라고 마킹하는지 알수 없음. 철근 찾아서 마킹하도록 수정.
// 	CString sMark	= _T("HS");
	CString sMark	= pBri->GetMarkRebarSupport(0, FALSE, FALSE, FALSE, _T("상부슬래브 헌치부 배력철근"));
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark) && tvArr.GetSize() > 0)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		CLineInfoApp *pLine	= static_cast<CLineInfoApp*>(pBri->GetLineBase());
		if(!pLine) return;
		CDPointArray xyArrMark;	// countmark할 위치들.

		// 배력근을 마크해야 되는 dist, sta등의 정보를 얻음.
// 		CDPoint xyDir	= tvArr.GetAt(0).GetXyDir(FALSE);
		double dSta		= 0;
		pLine->GetStationMatchByXyAndAngle(tvArr.GetAt(0).m_v1, tvArr.GetAt(0).GetXyDir(), dSta, 0);
		CDPoint xyDir			= pBri->GetAngleJijum(nJ);
		double dWid		= (pBri->m_bIsExp ? pBri->GetWidthSlabActByGagak(dSta, xyDir, 1) : pBri->GetWidthSlabAct(dSta, xyDir, 1));
		double dDist	= dWid*2/3;

		if(bApplyCutArea)
		{
			// 절단원 사이에 마크해야 됨.
			CTwinVectorArray tvArrCuttingArea;
			if(bGirder)
				pBri->GetTvCuttingArea_Plane_UpperSlabGirder(tvArrCuttingArea, nJ);
			else
				pBri->GetTvCuttingArea_Plane_SlabHunch(tvArrCuttingArea, nJ, TRUE);

			if(tvArrCuttingArea.GetSize() > 1)
			{
				CTwinVector tvCutting	= tvArrCuttingArea[1];
				if(bGirder)
					dDist	= pLine->GetDisMatchVerticalByXy(tvCutting.GetXyMid());
				else
					dDist	= pLine->GetDisMatchVerticalByXy(tvCutting.GetXyMid()) + fabs(tvCutting.m_v2.z/2);
				dSta	= tvCutting.GetXyMid().x;
			}
		}
		else
		{
			dSta = tvArr.GetXyMid().x;
		}

		if(!pLine->GetStationMatchVerticalByXy(tvArr[0].m_v1, dSta)) return;

		// 1cycle과 2cycle 주철근을 구함. /////////////////////////
		// 정확한 dDist를 구하기 위해서(주철근과 겹치지 않도록 하기 위해.)
		// 배력근 마킹은 dSta에서 하지만, 주철근은 2cycle을 반드시 고려 해야 되므로
		// 상면하면에 따라서 반드시 2cycle이 있을 만한 장소의 주철근 간격을 가져온다.
		CDoubleArray dArrDistMain, dArrDistHunch;
		CDoubleArray dArrDistMain2, dArrDistHunch2;
		CDoubleArray dArrDistMain3, dArrDistHunch3;
		CDoubleArray dArrDistMain4, dArrDistHunch4;
		pBri->GetDistRebarMain_CrossSection_Slab(TRUE, dArrDistMain, dArrDistHunch, dArrDistMain2, dArrDistHunch2, dArrDistMain3, dArrDistHunch3, dArrDistMain4, dArrDistHunch4, dSta, FALSE, 0, TRUE, FALSE);
		
		long i = 0; for(i = 0; i < dArrDistMain2.GetSize(); i++)
			dArrDistMain.Add(dArrDistMain2.GetAt(i));
		for(i = 0; i < dArrDistMain3.GetSize(); i++)
			dArrDistMain.Add(dArrDistMain3.GetAt(i));
		for(i = 0; i < dArrDistMain4.GetSize(); i++)
			dArrDistMain.Add(dArrDistMain4.GetAt(i));
		/////////////////////////////////////////////////////////////

		// 마킹할 위치 찾아옴
		GetXyCountMarkSupport_Plane_Slab(xyArrMark, tvArr, FALSE, dDist, dArrDistMain);

		// count mark하고 선을 그음.
		CDPoint xyDirRotate	= pBri->GetXyDirRotateDomByBridgeSttEndXy();
		CDPoint xyDirLine	= pLine->GetAngleAzimuth(dSta);
		CConcSymbol sym(m_pStd->m_pARoadOptionStd);
		for(i = 0; i < xyArrMark.GetSize(); i++)
			sym.RebarCountMarkOne(&Dom, xyArrMark[i], xyDirLine);
	
		long nSize	= xyArrMark.GetSize();
		if(nSize > 0)
		{
			CDPoint xyStt			= xyArrMark.GetAt(0);
			CDPoint xyEnd			= xyArrMark.GetAt(nSize-1);
			CDPoint xyDirAzimuth	= pBri->GetAngleAzimuthJijum(nJ);

			CTwinVector tv;
			tv.m_v1	= CVector(xyStt.x, xyStt.y, 4);
			tv.m_v2	= xyEnd;
			baseDraw.DrawTv(&Dom, tv);

			// count mark의 3/2지점쯤에 철근표시함.
			long nIdxPrev	= nSize > 1 ? nSize-2 : 0;
			long nIdxNext	= nIdxPrev+1;
			if(nIdxNext < 0) nIdxNext	= nIdxPrev;
			double dStaPrev(0), dStaNext(0), dStaMark(0);
			pLine->GetStationMatchByXyAndAngle(xyArrMark[nIdxPrev], xyDirAzimuth, dStaPrev);
			if(xyArrMark.GetSize() > nIdxNext)
				pLine->GetStationMatchByXyAndAngle(xyArrMark[nIdxNext], xyDirAzimuth, dStaNext);
			else
				dStaNext	= dStaPrev;
			dStaMark	= (dStaPrev+dStaNext)/2;

			// dDist에도 xyDir.y를 나눠야 되지만, 이미 GetXyCountMarkSupport_Plane_UpperSlab에서 
			// xyDir.y가 적용되서 계산된 길이가 나오므로.. 그냥 사용한다.
			dWid		= (pBri->m_bIsExp ? pBri->GetWidthSlabActByGagak(dStaMark, xyDir, 1) : pBri->GetWidthSlabAct(dStaMark, xyDir, 1));

			CDPoint xyMarkStt	= pLine->GetXyLineByLengthAndAngle(dStaMark, dDist, xyDir);
			CDPoint xyMarkEnd	= pLine->GetXyLineByLengthAndAngle(dStaMark, dWid/xyDir.y, xyDir);
			xyMarkEnd	+= MARK_BASEHEIGHT_LOWER * xyDirAzimuth;
			Dom.LineTo(xyMarkStt, xyMarkEnd);
			
			// 마킹함.
			*pDomP << Dom;
			// 왜 무조건 18이 들어가는거?? 그리고 18철근이 있음??
			CString sDia	= m_pStd->m_pARcBridgeDataStd->GetStringDia(18, pBri->GetValueFy(ePartUpperSlab));
			CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_SupportRebar_UpperSlab[iLOWER][0];
			if(pPlace)
				sDia	= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(ePartUpperSlab));
			// sym.RebarMarkLineSide(&Dom, xyMarkEnd, sMark, sDia, bGirder ? TRUE : FALSE, 0, FALSE);

			CRebar *pRb = pBri->GetRebarSupportByDescription(_T("상부슬래브 헌치부 배력철근"));
			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = sMark;
			mark_input_data.dia = sDia;
			mark_input_data.position = xyMarkEnd;
			mark_input_data.ctc = pBri->GetCTCRebarSupport_UpperSlabHunch();
			mark_input_data.type_rebar = pRb? pRb->m_nTypeRebar : -1;
			mark_input_data.left_dir = bGirder ? TRUE : FALSE;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.line_rebar = TRUE;
			mark_view_data.mark_pos = eMarkSide;

			sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

			xyDirAzimuth	= pLine->GetAngleAzimuth(dStaMark);
			Dom.Rotate(xyMarkEnd, ToDPointFrDegree((xyDirAzimuth*-1).GetAngleDegree()-180));
		}
	}

	*pDomP << Dom;
}

// 상부슬래브 헌치부 주철근
void CDrawPlane::DrawRebarMain_Plane_UpperSlabHunch(CDomyun *pDomP, long nCycle, long nJ, BOOL bUpper, long nLeftRebar, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft, long nDirMark, BOOL bApplyCutArea)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CTwinVectorArray tvArr, tvArrAngLeft, tvArrAngRight;
	CTwinVectorArray tvArrLeftHalf, tvArrRightHalf;
	double dSta	= pBri->GetStationOnJijum(nJ, 0, pBri->IsVertDir());
	pBri->GetTvRebarMain_Plane_SlabHunch(tvArr, tvArrAngLeft, tvArrAngRight, nCycle, dSta, nLeftRebar, nDrawLeft);

	//#0031780 - 4번수정 라운드 부분 철근 제외
	if(bUpper == FALSE)
	{
 		CDPoint xyDir	= pBri->GetAngleJijum(nJ, FALSE);
 		double dLenL	= (pBri->m_dDH[0] + pBri->m_dCantilB[0]);
 		double dLenR	= (pBri->m_dDH[1] + pBri->m_dCantilB[1]);

		CRebarPlacing rb;

		CTwinVectorArray tvArrPlane;
		pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1, TRUE);

		CTwinVectorArray tvArrLeft, tvArrRight;
		tvArrPlane.GetTvArrByInfo(_T("좌측"), tvArrLeft, FALSE, TRUE);
		tvArrPlane.GetTvArrByInfo(_T("우측"), tvArrRight, FALSE, TRUE);

		rb.GetOffsetTvArray(tvArrLeft, dLenL);
		rb.GetOffsetTvArray(tvArrRight, -dLenR);

		rb.TrimTvArrayByTvArray(tvArrAngRight, tvArrLeft, TRUE, TRUE);
		rb.TrimTvArrayByTvArray(tvArrAngRight, tvArrRight, FALSE, TRUE);

		rb.TrimTvArrayByTvArray(tvArrAngLeft, tvArrLeft, TRUE, TRUE);
		rb.TrimTvArrayByTvArray(tvArrAngLeft, tvArrRight, FALSE, TRUE);
	}

	if(tvArr.GetSize() == 0 && tvArrAngLeft.GetSize() == 0 && tvArrAngRight.GetSize() == 0) return;
	if(pBri->m_nTypePlacingMainRebar_UpperSlab == 2)
	{
		pBri->m_Util.DivideVertMainRebarBySabogang(tvArr, tvArrLeftHalf, tvArrRightHalf);
	}
	
	if(bDimRebar && !bApplyCutArea)
	{
		*pDomP << Dom;
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		BOOL bStt	= nJ == 0 ? TRUE : FALSE;

		// vert == 0 : 직방향 등 기본 철근
		// vert == 1 : 사보강 배치시 사방향 철근

		// 전체 치수를 넣을 시기를 결정함
		long nDimTotal	= 0;
		CTwinVector tvStt, tvEnd;
		pBri->GetTvSttEndOfSlab(tvStt, tvEnd);
		GetSwap(tvStt.m_v1, tvStt.m_v2);
		GetSwap(tvEnd.m_v1, tvEnd.m_v2);

		for(long vert = 0; vert < 3; vert++)
		{
			CDoubleArray dArrHunch;
			CDoubleArray dArrDim;
			CStringArray sArrText;
			CStringArray sArrUnderText;
			CString sDir	= bStt ? "TOP" : "BOTTOM";
			CDPoint xyStt	= CDPoint(0, 0);
			long nDimDan	= 1;

			// 치수기입 기준위치
			CDPoint vAng	= pBri->GetAngleJijum(nJ);
			double dSta		= pBri->GetStationByTarget(bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0, TRUE);
			CTwinVector tvJigan	= bStt ? tvStt : tvEnd;

			xyStt	= tvJigan.m_v2;

			// 가각 철근이 선형방향 배치면 치수를 포함시킨다.
			// 단, 주철근이 선형방향 배치이어야 한다
			if(pBri->m_nTypePlacingMainRebar_UpperSlab == 0)
			{
				long left = 0; for(left = 0; left < 2; left++)
				{
					BOOL bLeft	= left == iLEFT;
					if(pBri->IsGagakForLine(bStt, bLeft))
					{
						CTwinVectorArrayArray pTvArrTemp1, pTvArrTemp2, pTvArrTemp3, pTvArrTemp4;
						CTwinVectorArray tvArrCycleHunch[2];
						pBri->GetTvRebarMain_Plane_Slab_Exp(TRUE, &pTvArrTemp1, &pTvArrTemp2, &pTvArrTemp3, &pTvArrTemp4, bStt, bLeft, 0, FALSE, tvArrCycleHunch[0], tvArrCycleHunch[1]);

						long i = 0; for(i = 0; i < tvArrCycleHunch[0].GetSize(); i++)
						{
							tvArr.Add(tvArrCycleHunch[0].GetAt(i));
						}
					}
				}
			}

			// 가각 철근 치수가 주철근치수에 겹치면 빼버림
			CDPointArray xyArr;
			CDPoint xyMatch(0, 0);
			CDPointArray xyArrMatch;
			CTwinVector tv;
			CDPoint vAngDim	= pBri->GetAngleByTarget(bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0); //pBri->GetAngleJijum(nJ, bVertDir);

			long i = 0; for(i = 0; i < tvArr.GetSize(); i++)
			{
				tv	= tvArr.GetAt(i);

				if(tv.m_v1.z !=0)
				{
					double dDist	= pBri->GetLineBase()->GetDisMatchVerticalByXy(tv.m_v1);

					xyMatch	= pBri->GetLineBase()->GetXyLineByLengthAndAngle(dSta, dDist/vAngDim.y, vAngDim);
					xyArr.Add(xyMatch);
				}
				else
				{
					if(GetXyMatchSegAndLine(tvJigan.m_v1, tvJigan.m_v2, tv.m_v1, tv.GetXyDir(), xyMatch))
						xyArr.Add(xyMatch);
				}				
			}

			long nSize	= xyArr.GetSize();
			if(nSize > 0)
			{
				CDPoint xyLast(0, 0);
				CDPoint xy(0, 0);
				CDoubleArray dArrDimTmp;

				double dDistMax	= tvJigan.GetLength();
				double dDist	= 0;

				long i = 0; for(i = 0; i < nSize; i++)
				{
					xy	= xyArr.GetAt(i);
					dDist	= ~(xy - tvJigan.m_v2);

					dArrDimTmp.Add(dDist);
				}

				dArrDimTmp.Add(tvJigan.GetLength());

				dArrDimTmp.Sort(TRUE);


				nSize	= dArrDimTmp.GetSize();
				if(dArrDimTmp.GetSize() > 0)
				{
					dDist	= dArrDimTmp.GetAt(0);
					dArrDim.Add(dDist);
					for(i = 1; i < nSize; i++)
					{
						dDistMax	= dArrDimTmp.GetAt(i);
						dDist		= dArrDimTmp.GetAt(i-1);

						if(dDistMax - dDist > 0)
							dArrDim.Add(dDistMax-dDist);
					}
				}
			}

			// 치수 기입
			// 주철근이 선형방향배근이라면 치수선에 obq 적용
			double dObq	=  0;
			if(pBri->m_nTypePlacingMainRebar_UpperSlab == 0)
			{
				double dAng		= pBri->GetAngleByTarget(bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0).GetAngleDegree();// pBri->GetAngleJijum(bStt ? 0 : pBri->m_nQtyJigan).GetAngleDegree();
				dObq	= bStt ? dAng - 90 : 90 - dAng;
				Dom.SetDimObq(dObq);
			}
			else	// obq적용 안되는 경우 사각에 따라 시작위치 sta 조정 필요...
			{
				double dAng		= pBri->GetAngleByTarget(bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0).GetAngleDegree();
				if((bStt && dAng > 90) || (!bStt && dAng < 90))
				{
					CLineInfo *pLineB	= pBri->GetLineBase();
					if(!pLineB) return;	// program 다운 방지를 위해 깔끔하게 리턴시켜버림

					double dSta	= 0;
					if(pBri->m_nTypePlacingMainRebar_UpperSlab == 1 || pBri->m_nTypePlacingMainRebar_UpperSlab == 2)
						pLineB->GetStationMatchVerticalByXy(tvJigan.m_v2, dSta);	// 중심선형에서의 기점
					else
						pLineB->GetStationMatchVerticalByXy(tvJigan.m_v1, dSta);	// 중심선형에서의 기점
					// 					xyStt	= pBri->GetXySlabEdgeAct(dSta, CDPoint(0, 1), FALSE);

					xyStt	= tvJigan.m_v2;
				}
			}
			baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, TRUE, FALSE);

			// 방향에 맞게 회전(시종점 접선의 직각방향으로 회전)
			CDPoint xyDir	= pBri->GetLineBase()->GetAngleAzimuth(dSta).Rotate90();
			Dom.Rotate(xyStt, ToDPointFrDegree(xyDir.GetAngleDegree()+(bStt ? dObq : -dObq)));

			// 사보강 배치인 경우 
			if(pBri->m_nTypePlacingMainRebar_UpperSlab == 1 || pBri->m_nTypePlacingMainRebar_UpperSlab == 2)
			{
				Dom.Rotate(xyStt, pBri->GetAngleByTarget(bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0).RotateInv90());
			}

			*pDomP << Dom;
		}
	}

	// 절단원 적용해서 그림 
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_HIDDEN_LINE);
	
	if(bApplyCutArea)
	{
		ApplyCuttingArea_Plane(&Dom, tvArr, TRUE, 0, FALSE, FALSE, FALSE, FALSE, TRUE, nJ);
		ApplyCuttingArea_Plane(&Dom, tvArrLeftHalf, TRUE, 0, FALSE, FALSE, FALSE, FALSE, TRUE, nJ, FALSE, FALSE, pBri->GetLineLeftRight(TRUE));
		ApplyCuttingArea_Plane(&Dom, tvArrRightHalf, TRUE, 0, FALSE, FALSE, FALSE, FALSE, TRUE, nJ, FALSE, FALSE, pBri->GetLineLeftRight(FALSE));
		ApplyCuttingArea_Plane(&Dom, tvArrAngLeft, TRUE, 0, FALSE, FALSE, FALSE, FALSE, TRUE, nJ, FALSE, FALSE, pBri->GetLineLeftRight(TRUE));
		ApplyCuttingArea_Plane(&Dom, tvArrAngRight, TRUE, 0, FALSE, FALSE, FALSE, FALSE, TRUE, nJ, FALSE, FALSE, pBri->GetLineLeftRight(FALSE));
	}
	
	m_pStd->DrawTvArrWidthHidden(&Dom, tvArr, pBri->GetLineBase());
	m_pStd->DrawTvArrWidthHidden(&Dom, tvArrLeftHalf, pBri->GetLineBase());
	m_pStd->DrawTvArrWidthHidden(&Dom, tvArrRightHalf, pBri->GetLineBase());
	m_pStd->DrawTvArrWidthHidden(&Dom, tvArrAngLeft, pBri->GetLineBase());
	m_pStd->DrawTvArrWidthHidden(&Dom, tvArrAngRight, pBri->GetLineBase());
	
	if(bMarkRebar)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		// 사보강 배치가 아닐때는 선형용 치수기입 함수를 사용하고(주철근 함수 내용 복사함)
		// 사보강 배치일때는 기초에서 복사해온 일반 치수기입용 함수를 사용한다. 
		if(pBri->m_nTypePlacingMainRebar_UpperSlab == 0)
		{
			long nCount	= pBri->m_pArrRCMainUpperSlabHunch[nCycle].GetSize();
			long i = 0; for(i = 0; i < nCount; i++)
			{
				CRebarInfoCycle *pRC	= pBri->m_pArrRCMainUpperSlabHunch[nCycle].GetAt(i);
				DimRebarInfoCycle_Plane_UpperSlab(&Dom, pRC, FALSE, 0, nCycle, FALSE, nDrawLeft, FALSE, FALSE, TRUE, 2, bApplyCutArea);
			}
		}
		else if(pBri->m_nTypePlacingMainRebar_UpperSlab == 1 || pBri->m_nTypePlacingMainRebar_UpperSlab == 2)
		{
			CLineInfo *pLineVert	= pBri->GetLineBase(pBri->IsVertDir());
			CRebarPlacing rb;
			CTwinVectorArray tvArrSup, tvArrSupAngStt, tvArrSupAngEnd, tvArrRAngStt, tvArrRAngEnd;
			pBri->GetTvRebarSupport_Plane_SlabHunch(tvArrSup, TRUE, 0, nJ);
			pBri->GetTvRebarSupport_Plane_Slab_Std(TRUE, tvArrSup, tvArrSupAngStt, tvArrSupAngEnd, tvArrRAngStt, tvArrRAngEnd, FALSE, 0, 0, TRUE);

			CTwinVector tv;
			long nSize			= pBri->m_pArrRCMainUpperSlabHunch[nCycle].GetSize();
			long dCountCycle	= pBri->GetCountCycleMainRebar();
			
			long i = 0; for(i = 0; i < nSize; i++)
			{
				CRebarInfoCycle *pRC	= pBri->m_pArrRCMainUpperSlabHunch[nCycle].GetAt(i);
				if(!pRC) continue;
				if(!pRC->m_bExist) continue;
				if(pRC->m_nPosHunch != nJ) continue;

				CRebar *pRb = pBri->GetRebarMainByDescription(pRC->m_sDescription);

				// 수직보강..
				CString sMark	= pRC->m_szMark;
				if(tvArr.GetSize() > 0 && m_pStd->IsMarkRebar(sMark))
				{
					//#0031780 - 2상면철근에 하면 철근이 표현됨 상부인경우는 마킹을 하지 않는다.
					if(bUpper == FALSE) MarkRebarMain_Plane_UpperSlab_ForVert(&Dom, tvArr, tvArrLeftHalf, tvArrRightHalf, tvArrSup, i, FALSE, 0, nCycle, TRUE, FALSE, FALSE, nDrawLeft);
				}


				// 사보강 (좌측)
				sMark.Format("%s-1", pRC->m_szMark);
				if(tvArrAngLeft.GetSize() > 0 && m_pStd->IsMarkRebar(sMark))	// 1단만 마크해도 복잡한데, 2,3단까지 전부 마크하면 너무 지저분해짐
				{
					rb.ReverseRebar(tvArrAngLeft);

					CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
					mark.SetRebar(tvArrAngLeft, tvArrSup);

					// 설정
					mark.m_sMark					= sMark;
					mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pBri->GetValueFy(ePartUpperSlab));
					mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_END;
					mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
					mark.m_dPosScaleMarkStt			= 1./(dCountCycle+nSize) * (nCycle+i+1)+0.2;
					mark.m_dPosScale				= 1./(dCountCycle+nSize) * (nCycle+i+1)+0.2;
					mark.m_dPosScaleSpace			= 0.5 - (1./dCountCycle)/2;

					CDPointArray xyArr;
					mark.GetXyCountMark(xyArr);
					mark.m_xyArrCountMark	= xyArr;

					if(xyArr.GetSize() > 1)
					{
						CLineInfo *pLine	= pBri->GetLineBase();
						if(pLine)
						{
							CDPoint xyMark	= mark.GetXyMark(FALSE);
							CDPoint vAng	= (xyArr.GetAt(0)-xyArr.GetAt(1)).Unit();
							CDPoint vAngJ	= pBri->GetAngleJijum(0);
							double dSta		= xyMark.x;
							pLine->GetStationMatchByXyAndAngle(xyMark, vAng, dSta, 0);
							double dWidth	= pBri->GetWidthSlabAct(dSta, vAngJ, -1);
							CDPoint xyMatch	= pLine->GetXyLineByLengthAndAngle(dSta, -dWidth, vAngJ);
							mark.m_dAddHeight	= ~(xyMatch-xyMark) + Dom.GetTextHeight();
						}
						mark.m_dCtc						= pBri->GetCtcRebarMain(TRUE);
						mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;

						mark.MarkRebar(FALSE);	
					}
				}

				// 사보강 (우측)
				sMark.Format("%s-2", pRC->m_szMark);
				if(tvArrAngRight.GetSize() > 0 && m_pStd->IsMarkRebar(sMark))	// 1단만 마크해도 복잡한데, 2,3단까지 전부 마크하면 너무 지저분해짐
				{
					rb.ReverseRebar(tvArrAngRight);
					CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
					mark.SetRebar(tvArrAngRight, tvArrSup);

					// 설정
					mark.m_sMark					= sMark;
					mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pBri->GetValueFy(ePartUpperSlab));
					mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_STT;
					mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
					mark.m_dPosScaleMarkStt			= 1./(dCountCycle+nSize) * (nCycle+i+1)+0.2;
					mark.m_dPosScale				= 1./(dCountCycle+nSize) * (nCycle+i+1)+0.2;
					mark.m_dPosScaleSpace			= 0.5 - (1./dCountCycle)/2;
					
					CDPointArray xyArr;
					mark.GetXyCountMark(xyArr);
					mark.m_xyArrCountMark	= xyArr;

					if(xyArr.GetSize() > 1)
					{
						CLineInfo *pLine	= pBri->GetLineBase();
						if(pLine)
						{
							long nSize		= xyArr.GetSize();
							CDPoint xyMark	= mark.GetXyMark(FALSE);
							CDPoint vAng	= (xyArr.GetAt(nSize-2)-xyArr.GetAt(nSize-1)).Unit();
							CDPoint vAngJ	= pBri->GetAngleJijum(0);
							double dSta		= xyMark.x;
							pLine->GetStationMatchByXyAndAngle(xyMark, vAng, dSta, 0);
							double dWidth	= pBri->GetWidthSlabAct(dSta, vAngJ, 1);
							CDPoint xyMatch	= pLine->GetXyLineByLengthAndAngle(dSta, dWidth, vAngJ);
							mark.m_dAddHeight	= ~(xyMatch-xyMark)+Dom.GetTextHeight();
						}
						mark.m_dCtc						= pBri->GetCtcRebarMain(TRUE);
						mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;

						mark.MarkRebar(FALSE);	
					}
				}
			}
		}
	}

	// 가각부
	if(!bUpper && (nJ == 0 || nJ == pBri->m_nQtyJigan))
	{
		BOOL bStt = nJ == 0 ? TRUE : FALSE;
		for(long ilr=0; ilr<2; ++ilr)
		{
			BOOL bLeft = ilr == iLEFT ? TRUE : FALSE;
			if(nDrawLeft == -1 && !bLeft) continue;
			if(nDrawLeft == 1 && bLeft) continue;

			if(pBri->IsGagakForLine(bStt, bLeft))
			{
				CTwinVectorArrayArray pTvArrTemp1, pTvArrTemp2, pTvArrTemp3, pTvArrTemp4;
				CTwinVectorArray tvArrCycleHunch[2];
				pBri->GetTvRebarMain_Plane_Slab_Exp(TRUE, &pTvArrTemp1, &pTvArrTemp2, &pTvArrTemp3, &pTvArrTemp4, bStt, bLeft, 0, FALSE, tvArrCycleHunch[0], tvArrCycleHunch[1]);
				if(tvArrCycleHunch[nCycle].GetSize() > 0)
				{
					if(bApplyCutArea)
					{
						ApplyCuttingArea_Plane(&Dom, tvArrCycleHunch[nCycle], FALSE, 0, FALSE, FALSE, FALSE, FALSE, TRUE, nJ, FALSE, FALSE, pBri->GetLineLeftRight(TRUE));
					}

					m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
					m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_HIDDEN_LINE);
					m_pStd->DrawTvArray(&Dom, tvArrCycleHunch[nCycle]);

					if(bMarkRebar)
					{
						CRebarPlacing rb;
						CString sMark(_T("")), sDescription(_T(""));
						double dDia(0);

						pBri->GetInfoMainRebarExp_UpperSlabHunch(bStt, bLeft, nCycle, sMark, dDia, sDescription);

						CRebar *pRb = pBri->GetRebarMainByDescription(sDescription);

						CTwinVectorArray tvArrSup;
						pBri->GetTvRebarSupport_Plane_SlabHunch(tvArrSup, TRUE, 0, nJ);
						if(tvArrSup.GetSize() > 0)
						{
							// 헌치 철근 갯수가 적어서 마킹 위치가 뭉친다. 가상 위치의 헌치 철근을 넣어서 마킹하자.
							CTwinVector tvTemp = tvArrSup.GetAt(bStt ? 0 : (tvArrSup.GetSize()-1));
							double dOffset = bStt ? -300 : 300;
							rb.Offset(tvTemp.m_v1, tvTemp.m_v2, dOffset);
							if(bStt)
								tvArrSup.InsertAt(0, tvTemp);
							else
								tvArrSup.Add(tvTemp);
						}
						else
						{
							CTwinVector tvTemp;
							CDPoint xyMid = tvArrCycleHunch[nCycle].GetXyMid();
							CDPoint xyTop = tvArrCycleHunch[nCycle].GetXyTop();
							CDPoint xyBot = tvArrCycleHunch[nCycle].GetXyBottom();
							tvTemp.m_v1 = CDPoint(xyMid.x, xyTop.y);
							tvTemp.m_v2 = CDPoint(xyMid.x, xyBot.y);
							tvArrSup. Add(tvTemp);

							double dOffset = bStt ? -300 : 300;
							rb.Offset(tvTemp.m_v1, tvTemp.m_v2, dOffset);
							if(bStt)
								tvArrSup.InsertAt(0, tvTemp);
							else
								tvArrSup.Add(tvTemp);
						}

						rb.ReverseRebar(tvArrCycleHunch[nCycle]);

						CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
						mark.SetRebar(tvArrCycleHunch[nCycle], tvArrSup);

						// 설정
						mark.m_sMark					= sMark;
						mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(dDia, pBri->GetValueFy(ePartUpperSlab));
						if(bApplyCutArea)
							mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_STT;
						else
							mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_POS;
						mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
						mark.m_dPosScaleMarkStt			= 0.5;
						mark.m_dPosScale				= bStt ? 0.2 : 0.8;
						mark.m_dPosScaleSpace			= 0.5;
						mark.m_dAngRotCount				= 90;
						mark.m_nTypeMarkDir				= bStt ? TYPE_MARKDIR_LEFT : TYPE_MARKDIR_RIGHT;

						CDPointArray xyArr;
						mark.GetXyCountMark(xyArr);
						mark.m_xyArrCountMark	= xyArr;

						if(xyArr.GetSize() > 1)
						{
							CLineInfo *pLine	= pBri->GetLineBase();
							if(pLine)
							{
								CDPoint xyMark	= mark.GetXyMark(FALSE);
								CDPoint vAng	= (xyArr.GetAt(0)-xyArr.GetAt(1)).Unit();
								CDPoint vAngJ	= pBri->GetAngleJijum(nJ);
								double dSta		= xyMark.x;
								
								if(!bApplyCutArea)
								{
									double dDist = pLine->GetDisMatchVerticalByXy(xyMark);
									double dStaEnd = pBri->GetStationAtSlabEnd(bStt, 0) + Dom.Always(10) * (bStt ? -1 : 1);
									CDPoint xyMarkStt	= pLine->GetXyLineDisByAng(dStaEnd, dDist, pBri->GetAngleJijum(bStt ? 0 : pBri->m_nQtyJigan));
									mark.m_dAddHeight	= ~(xyMark - xyMarkStt);
								}
								else
								{
									//double dWid	= pBri->GetWidthSlabActByGagak(dSta, mark.m_xyDirCountMark, bLeft ? 1 : -1);// + (bLeft ? MARK_BASEHEIGHT_UPPER : MARK_BASEHEIGHT_LOWER);
									//CDPoint xyMarkStt	= pLine->GetXyLineByLengthAndAngle(dSta, dWid, vAng);
									mark.m_dAddHeight	= bLeft ? MARK_BASEHEIGHT_UPPER : MARK_BASEHEIGHT_LOWER;
								}
							}
							mark.m_dCtc						= pBri->GetCtcRebarMain(TRUE);
							mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;

							mark.MarkRebar(FALSE);
						}
					}
				}
			}
		}
	}

	*pDomP << Dom;
}


// 부슬래브 평면도에 count mark해야 되는 적절한 위치를 찾아 준다 - 상부슬래브, 하부슬래브 같이사용.
// BOOL bUpper : tvArrSup가 상부슬래브의 하부에 그려진건지 상부에 그려진건지... 이에 따라 중심선형의 위치를 결정함
// dDistMark : countmark할 위치(중심선형에서 떨어진 거리) - dArrDistMain을 참조해서 정확하게 재계산됨
// dArrDistMain : dDistMark의위치를 재 조정 하기 위한 주철근의 dist배열
// tvArrSup : 실제로 마크해야 되는 배력근의 좌표
// xyArr : count mark할 위치 (리턴용)
// BOOL bOnlyMatchLine : 선과 교차되는거만 마크를 함.
void CDrawPlane::GetXyCountMarkSupport_Plane_Slab(CDPointArray &xyArr, CTwinVectorArray &tvArrSup, BOOL bUpper, double &dDistMark, CDoubleArray &dArrDistMain, BOOL bOnlyMatchLine)
{
	xyArr.RemoveAll();

	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	CLineInfoApp *pLine	= static_cast<CLineInfoApp*>(pBri->GetLineBase());
	if(!pLine) return;

	
	double dSta	= 0;
	if(!pLine->GetStationMatchVerticalByXy(bUpper ? tvArrSup[0].m_v2 : tvArrSup[0].m_v1, dSta)) return;
	CDPoint xyDirLine	= pLine->GetAngleAzimuth(dSta);
	CDPoint xyDir	= CDPoint(0, 1);
	if(tvArrSup.GetSize() > 0)
	{
		CTwinVector tv	= tvArrSup.GetAt(0);
		xyDir	= ToDPointFrDegree(tv.GetXyDir().GetAngleDegree()-xyDirLine.GetAngleDegree());
	}


	dDistMark	/= xyDir.y;

	// 주철근이 있다면 기존 셋팅된 dDist와 가장 가까운 위치를 찾음.
	dArrDistMain.Sort(FALSE);	// 아래에서 위로 가도록 정렬
				
	// 중복되는거 제거
	long i = 0; for(i = 0; i < dArrDistMain.GetSize()-1; i++)
	{
		if(Compare(dArrDistMain[i], dArrDistMain[i+1], "="))
		{
			dArrDistMain.RemoveAt(i+1);
			i--;
		}
	}

	if(dArrDistMain.GetSize() > 1)
	{
		long nSize	= dArrDistMain.GetSize();
		long i = 0; for(i = 0; i < nSize-1; i++)
		{
			double dDistPrev	= dArrDistMain[i]/xyDir.y;
			double dDistNext	= dArrDistMain[i+1]/xyDir.y;
			if(dDistPrev > dDistNext)
				GetSwap(dDistPrev, dDistNext);
			if(dDistMark >= dDistPrev && dDistMark <= dDistNext)
			{
				dDistMark	= (dDistNext+dDistPrev)/2;
				break;
			}
		}
	}
	double dDistMarkOrg	= dDistMark * xyDir.y;
	////////////////////////////////////////////////////////////////////

	// 마크할 위치를 수집함.
	// dDistMark는 선형에서 수직을 기준으로 한 dist이다.
	// 그러나 여기서 마크 위치 수집할때는 선형에서 xyDir방향으로 위치를 찾으므로 
	// dDistMark에 xyDir에 대한 길이를 적용해줘야 할 것이다.
	CTwinVector tv;
	double dStaCur	= dSta;
	CDPoint xyMark;
	for(i = 0; i < tvArrSup.GetSize(); i++)
	{
		tv	= tvArrSup.GetAt(i);
		if(pLine->GetStationMatchByXyAndAngle(tv.m_v1, tv.GetXyDir(), dStaCur, 0))	//  이 함수는 특이하게도 상대각이 아닌 방위각을 사용한다. ㅡㅡ;
		{
			CDPoint xyDirLine	= pLine->GetAngleAzimuth(dStaCur);
			xyDir	= ToDPointFrDegree(tv.GetXyDir().GetAngleDegree()-xyDirLine.GetAngleDegree());
			dDistMark	= dDistMarkOrg / xyDir.y;
			
			xyMark	= pLine->GetXyLineByLengthAndAngle(dStaCur, dDistMark, xyDir);
			if(bOnlyMatchLine)
			{
				CDPoint xyMatch(0, 0);
				if(!GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, xyMark, tv.GetXyDir().Rotate90(), xyMatch)) continue;
			}
			xyArr.Add(xyMark);
		}
	}
}

// 평면도 상부슬래브 전단철근 그리기
void CDrawPlane::DrawRebarShear_Plane_UpperSlab(CDomyun *pDomP, long nDrawLeft)
{
	if(!pDomP) return;

	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	CHgBaseDrawStd baseDraw;

	CTwinVectorArray tvArr;
	pBri->GetTvRebarShear_Plane_Slab_Real(TRUE, tvArr);
	

	// 전단철근 모양으로 만들기
	CRebarPlacing rb;
	rb.GetTvArrShearRebar(tvArr, FALSE, pBri->m_RebarShearDetail_UpperSlab.m_nType == TYPE_SHEAR_DIGUT || pBri->m_RebarShearDetail_UpperSlab.m_nType == TYPE_SHEAR_SPACER || pBri->m_RebarShearDetail_UpperSlab.m_nType == TYPE_SHEAR_SINGLE_HOOK, FALSE);

	// 절단원에 대해서 적용(1단 주철근과 같은 옵션임)
	//ApplyCuttingArea_Plane(&Dom, tvArr, TRUE, 0, FALSE, FALSE);	// 구지 호출 할 필요가 없어서 막아둠.

	// nDrawLeft에 따라서 필요한 것만 골라냄
	if(nDrawLeft != 0)
	{
		CTwinVectorArray tvArrArea;
		pBri->GetTvCuttingArea_UpperSlab_Shear(tvArrArea, nDrawLeft);

		rb.GetTvArrayByCrossTvArrayArea(tvArr, tvArrArea, TRUE);

		// 자르는 타입이 원인 경우 자르는 영역을 그려준다.
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
		baseDraw.DrawTvArray(&Dom, tvArrArea);
	}

	// 전단철근 그리기
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);	
	baseDraw.DrawTvArray(&Dom, tvArr);
	
	*pDomP << Dom;
}

// 평면도 브라켓 주철근
void CDrawPlane::DrawRebarMain_Plane_UpperSlabBracket(CDomyun *pDomP, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	for(long cycle = 0; cycle < 2; cycle++)
	{
		if(!pBri->IsBracketRebar(bStt, cycle)) continue;

		CTwinVectorArray tvArr, tvArrTmp;
		pBri->GetTvRebarMain_Plane_UpperSlabBracket(tvArr, bStt, cycle);
			
		if(bDimRebar)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		}

		// 절단원 적용해서 그림 
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
		ApplyCuttingArea_Plane(&Dom, tvArr, TRUE, 0, FALSE, FALSE, bStt, !bStt);
		baseDraw.DrawTvArray(&Dom, tvArr);

		
		if(bMarkRebar && tvArr.GetSize() > 0)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
			CTwinVectorArray tvArrSup, tvArrSupAngLeft, tvArrSupAngRight, tvArrRAngStt, tvArrRAngEnd;
			
			pBri->GetTvRebarSupport_Plane_UpperSlabBracket(tvArrSup, bStt, 1);
			if(tvArrSup.GetSize() > 1 && cycle == 1)
			{
				tvArrSup.RemoveAt(2, tvArrSup.GetSize()-2);
			}
			CRebarPlacing rb;
			rb.ReverseRebar(tvArr);
			MarkRebarMain_Plane_UpperSlab_ForVert(&Dom, tvArr, tvArrTmp, tvArrTmp, tvArrSup, 0, TRUE, 0, cycle, FALSE, bStt, !bStt, nDrawLeft);
		}
	}



	*pDomP << Dom;
}

// 평면도 브라켓 배력철근
void CDrawPlane::DrawRebarSupport_Plane_UpperSlabBracket(CDomyun *pDomP, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CTwinVectorArray tvArr;
	//#0031858 직방향의 브라켓으 그려지면 안됨
	pBri->GetTvRebarSupport_Plane_UpperSlabBracket(tvArr, bStt, 1);

	// 절단원 적용해서 그림
	ApplyCuttingArea_Plane(&Dom, tvArr, FALSE, 0, FALSE, FALSE, bStt, !bStt);
	baseDraw.DrawTvArray(&Dom, tvArr);

	if(bDimRebar)
	{

	}



	CString sDescription	= _T("");
// 	CString sMark			= _T("");
	sDescription.Format("%s 브라켓 배력철근", bStt ? "시점" : "종점");
	CString sMark	= pBri->GetMarkRebarEtc(0, FALSE, FALSE, FALSE, sDescription);
	long stt	= bStt ? iSTT : iEND;
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &pBri->m_pArrRCMainOutWallBracket_Cycle1[stt];
	if(pArrRC->GetSize() == 0) pArrRC = &pBri->m_pArrRCMainOutWallBracket_Cycle2[stt];
	if(pArrRC->GetSize() == 0) return;


	long nSize = tvArr.GetSize();

	if(bMarkRebar && m_pStd->IsMarkRebar(sMark) && nSize > 0 && pArrRC->GetSize() > 0)
	{
		*pDomP << Dom;
		
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		CConcSymbol sym(m_pStd->m_pARoadOptionStd);

		CTwinVector tv;
		tv	= bStt ? tvArr.GetAt(0) : tvArr.GetAt(nSize-1);
		double dCtc = 150;
		if(nSize > 1)
		{
			dCtc = fabs(tvArr.GetAt(0).m_v1.y - tvArr.GetAt(1).m_v1.y);
		}

		CString sDia	= m_pStd->m_pARcBridgeDataStd->GetStringDia(pBri->m_dDiaRebarSupport_Bracket[stt], pBri->GetValueFy(ePartUpperSlab));
		
		// sym.RebarMarkLineSide(&Dom, tv.GetXyMid(), sMark, sDia, bStt);

		CRebar *pRb = pBri->GetRebarEtcByDescription(sDescription, FALSE);
		SRebarMarkInputData mark_input_data;
		SRebarMarkviewData mark_view_data;
		mark_input_data.mark = sMark;
		mark_input_data.dia = sDia;
		mark_input_data.position = tv.GetXyMid();
		mark_input_data.ctc = dCtc;
		mark_input_data.type_rebar = pRb? pRb->m_nTypeRebar : -1;
		mark_input_data.left_dir = bStt;
		mark_input_data.height = 0;

		mark_view_data.count_mark = TRUE;
		mark_view_data.line_rebar = TRUE;
		mark_view_data.mark_pos = eMarkSide;

		sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

		long nJ	= bStt ? 0 : pBri->m_nQtyJigan;
		CDPoint xyRotate	= ToDPointFrDegree(pBri->GetAngleAzimuthJijum(nJ).GetAngleDegree() + 90);

		Dom.Rotate(tv.GetXyMid(), xyRotate);
	}

	*pDomP << Dom;
}

// 브라켓 평면도 일반도
// 항상 절단원 적용해야 됨
void CDrawPlane::DrawPlaneGenBracket(CDomyun *pDomP, BOOL bStt, BOOL bHidden)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, bHidden ? HCAD_CONH : HCAD_CONC);

	CTwinVectorArray tvArr;
	pBri->GetTvPlaneOutWall(tvArr, bStt, TRUE, FALSE);

	ApplyCuttingArea_Plane(&Dom, tvArr, TRUE, 0, FALSE, FALSE, bStt, !bStt);
	baseDraw.DrawTvArray(&Dom, tvArr);

	*pDomP << Dom;
}
// 평면 비계 그리기
// 치수도 넣어야 됨(수량산출서용)
void CDrawPlane::DrawPlaneScaffold(CDomyun *pDomP)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	double dRot = pBri->GetXyDirRotateDomByBridgeSttEndXy().GetAngleDegree();

	// 시종점 본체 비계
	CHgBaseDrawStd baseDraw;
	CRebarPlacing rb;

	CDomyun Dom(pDomP);
	CDomyun DomDim(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	m_pStd->ApplyDimSetting(pDomP, &Dom);
	m_pStd->ApplyDimSetting(pDomP, &DomDim);

	double dScaleDom = toM(pBri->m_dLengthBridge)/35 * 100;
	DomDim.SetScaleDim(dScaleDom);
		
	// 평면도
	DrawPlaneSlabOnlyOutLine(&Dom);
	DrawPlaneSlabSttEnd(&Dom);
	DrawPlaneSttEndLine(&Dom);

	// 날개벽 평면도
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT ? TRUE : FALSE;

		pBri->GetTvWingWallPlane(bStt);
		m_pStd->m_pDrawWingWall->DrawGenPlane(&Dom, bStt);
	}
	
	
	// 시종점 날개벽 비계
	for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT ? TRUE : FALSE;

		CTwinVectorArray tvArrScaffold, tvArrCenter;
		pBri->GetTvScaffoldPlane(tvArrScaffold, tvArrCenter, bStt);
		tvArrScaffold.Sort();
		if(tvArrScaffold.GetSize() == 2)	// 단힌 사각형으로 만들어줌
		{
			CTwinVector tv1, tv2;
			tv1.m_v1 = tvArrScaffold.GetAt(0).m_v1;
			tv1.m_v2 = tvArrScaffold.GetAt(1).m_v1;
			tv2.m_v1 = tvArrScaffold.GetAt(0).m_v2;
			tv2.m_v2 = tvArrScaffold.GetAt(1).m_v2;
			tvArrScaffold.Add(tv1);
			tvArrScaffold.Add(tv2);
			tvArrScaffold.Sort();
		}
		
		//m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_SOLID);
		baseDraw.HatchTvArray(&Dom, tvArrScaffold, 500, 45-dRot, TRUE);
		
		//m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_CENTER2_LINE);
		if(!bStt)
			rb.ReverseRebar(tvArrCenter);
		baseDraw.DrawTvArray(&Dom, tvArrCenter);
		baseDraw.DimTvArray(&DomDim, tvArrCenter, bStt, 0);
		
		// 시종점 날개벽 비계
		BOOL bUpper	= TRUE;
		long left = 0; for(left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT ? TRUE : FALSE;

			CTwinVectorArray tvArrOut, tvArrCenOut, tvArrSideOut;
			CTwinVectorArray tvArrIn, tvArrCenIn, tvArrSideIn;
			pBri->GetTvScaffoldWingPlane(tvArrOut, tvArrIn, tvArrCenOut, tvArrCenIn, tvArrSideOut, tvArrSideIn, bStt, bLeft);
			tvArrOut.Sort();
			tvArrIn.Sort();

			// 외측
			bUpper	= bLeft;
			if(bStt)
			{
				rb.ReverseRebar(tvArrCenOut);
				bUpper	= !bLeft;
			}
			baseDraw.HatchTvArray(&Dom, tvArrOut, 500, 45-dRot, TRUE);
			baseDraw.DrawTvArray(&Dom, tvArrCenOut);
			baseDraw.DimTvArray(&DomDim, tvArrCenOut, bUpper, 0);
			
			// 내측
			bUpper	= bLeft;
			if(!bStt)
			{
				rb.ReverseRebar(tvArrCenIn);
				bUpper	= !bLeft;
			}

			baseDraw.HatchTvArray(&Dom, tvArrIn, 500, 45-dRot, TRUE);
			baseDraw.DrawTvArray(&Dom, tvArrCenIn);
			baseDraw.DimTvArray(&DomDim, tvArrCenIn, bUpper, 0);

			//측면
			if(tvArrSideOut.GetSize() > 0 && tvArrSideIn.GetSize() > 0)
			{
				bUpper	= bLeft;
				if(bStt)
				{
					rb.ReverseRebar(tvArrSideOut);
					bUpper	= !bLeft;
				}
				baseDraw.HatchTvArray(&Dom, tvArrSideOut, 500, 45-dRot, TRUE);
				baseDraw.DrawTvArray(&Dom, tvArrSideOut);
				baseDraw.DimTvArray(&DomDim, tvArrSideOut, bUpper, 0);

				// 내측
				bUpper	= bLeft;
				if(!bStt)
				{
					rb.ReverseRebar(tvArrSideIn);
					bUpper	= !bLeft;
				}

				baseDraw.HatchTvArray(&Dom, tvArrSideIn, 500, 45-dRot, TRUE);
				baseDraw.DrawTvArray(&Dom, tvArrSideIn);
				baseDraw.DimTvArray(&DomDim, tvArrSideIn, bUpper, 0);
			}
		}
	}
	                                                         
	*pDomP << Dom;
	*pDomP << DomDim;
}

// 주철근은 원래 선형방으로 배치되지만
// 사보강방식을 택했을때 지점각도의 직각으로 배치되기도 한다
// 이 때는 이 함수를 이용해서 직각방향 철근에 대해서 치수를 기입 해줘야 한다.
// 혹, 상하부 슬래브 주철근의 경우 사방향 철근도 있는데 이는 따로 알아서 기입해야 됨.
// 여기서는 직각방향 철근만..
// 단, 배력근은 구해서 넘겨 줘야 됨(여기서 매번 구하면 속도저하의 원인)
void CDrawPlane::MarkRebarMain_Plane_UpperSlab_ForVert(CDomyun *pDomP, CTwinVectorArray &tvArrMain, CTwinVectorArray &tvArrMainLeft, CTwinVectorArray &tvArrMainRight, CTwinVectorArray &tvArrSup, long nIdxRC, BOOL bUpper, long nDan, long nCycle, BOOL bHunch, BOOL bSttBracket, BOOL bEndBracket, long nDrawLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(nIdxRC < 0) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CRebarPlacing rb;

	if(bHunch)
		rb.ReverseRebar(tvArrMain);

	CRebarInfoCycle *pRC	= NULL;
	long nCountRC	= 1;
	if(!bHunch && !bSttBracket && !bEndBracket)
	{
		long upper	= bUpper ? iUPPER : iLOWER;
		pRC	= pBri->GetRebarInfoCycleUpperSlab(nCycle, nDan, bUpper, nIdxRC);
		nCountRC	= pBri->m_pArrRCMainUpperSlab[nCycle][upper][nDan].GetSize();
	}
	else
	{
		if(bHunch)
		{
			if(pBri->m_pArrRCMainUpperSlabHunch[nCycle].GetSize() <= nIdxRC)
				return;
			pRC	= pBri->m_pArrRCMainUpperSlabHunch[nCycle].GetAt(nIdxRC);
		}
		else if(bSttBracket || bEndBracket)
		{
			long stt	= bSttBracket ? iSTT : iEND;
			CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = nCycle == 0 ? &pBri->m_pArrRCMainOutWallBracket_Cycle1[stt] : &pBri->m_pArrRCMainOutWallBracket_Cycle2[stt];
			if(pArrRC->GetSize() <= nIdxRC) 
				return;
			pRC	= pArrRC->GetAt(nIdxRC);
		}
	}

	if(!pRC) return;


	// 어디로 뻗을지 결정 해서 선 가져옴
	BOOL bExtendToSttOfMain	= TRUE;
	CString sStt	= _T("시점");
	if(!bHunch && !bSttBracket && bEndBracket)
	{
		sStt	= _T("종점");
	}
	// 일반 주철근이지만, 종점쪽에 가까우면 종점에 마킹한다.
	else if((pRC->m_xyStt.x+pRC->m_xyEnd.x)/2 > pBri->m_dStationBridgeStt+pBri->m_dLengthBridge/2)
	{
		sStt	= _T("종점");
		bExtendToSttOfMain	= FALSE;
	}

	
	CTwinVectorArray tvArrPlane, tvArrEnd, tvArrLeft, tvArrRight;
	CTwinVector tv;
	pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1);
	long i = 0; for(i = 0; i < tvArrPlane.GetSize(); i++)
	{
		tv	= tvArrPlane.GetAt(i);
		if(tv.m_sInfo == sStt) tvArrEnd.Add(tv);
		else if(tv.m_sInfo == "좌측") tvArrLeft.Add(tv);
		else if(tv.m_sInfo == "우측") tvArrRight.Add(tv);
	}
	
	long dCountCycle	= pBri->GetCountCycleMainRebar();
	
	// 직각방향 철근
	// 중요 : 상부슬래브 주철근일 경우 사보강(절반)을 선택하면 좌우로 끊어진 철근이 존재 할 수 있다.
	// 따로 마킹해준다. (좌측은 "-3", 우측은 "-4"를 붙여준다.
	long nIdxStt	= 0;
	long nIdxEnd	= !bSttBracket && !bEndBracket ? 2 : 0;

	CRebarInfoCycle *pRCForMark	= pBri->GetOwerRebarInfoCycleSlab(TRUE, nCycle, nDan, bUpper, nIdxRC);

	for(i = nIdxStt; i <= nIdxEnd; i++)
	{
		CString sMark	= (pRCForMark && !bHunch) ? pRCForMark->m_szMark : pRC->m_szMark;

		CString sDescription	 = (pRCForMark && !bHunch) ? pRCForMark->m_sDescription : pRC->m_sDescription;
		if(bSttBracket || bEndBracket)
		{
			sDescription.Format("%s 브라켓 주철근%s", bSttBracket ? "시점" : "종점", m_pStd->m_pARcBridgeDataStd->GetStringCircleNum(nCycle+1));
			sMark	= pBri->GetMarkRebarEtc(0, FALSE, FALSE, FALSE, sDescription);
		}
		
		if(tvArrMain.GetSize() > 0 && m_pStd->IsMarkRebar(sMark))
		{
			CMarkDomyun mark;//(&Dom, m_pStd->m_pARoadOptionStd);
			mark.SetDomyun(&Dom, m_pStd->m_pARoadOptionStd);
			if(i == 0)
				mark.SetRebar(tvArrMain, tvArrSup);
			else if(i == 1)
				mark.SetRebar(tvArrMainLeft, tvArrSup);
			else if(i == 2)
				mark.SetRebar(tvArrMainRight, tvArrSup);

			// 설정
			if(i == 0)
				mark.m_sMark					= sMark;
			else
				mark.m_sMark.Format("%s-%d", sMark, i+2);				
			
			mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pBri->GetValueFy(ePartUpperSlab));
			mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
			mark.m_dPosScaleMarkStt			= 1./(dCountCycle+nCountRC) * (nCycle+nIdxRC+1);
			mark.m_dPosScale				= 1./(dCountCycle+nCountRC) * (nCycle+nIdxRC+1);
			mark.m_dPosScaleSpace			= 0.5 - (1./dCountCycle)/2;
			mark.m_xyDirCountMark			= pBri->GetAngleAzimuthJijum(0).Rotate(CDPoint(-1,0));

			if(i == 1)
			{
				mark.m_nTypeMarkPosLineRebar			= TYPE_MARKPOS_END;
			}
			else if(i == 2)
			{
				mark.m_nTypeMarkPosLineRebar			= TYPE_MARKPOS_STT;
			}
			else
			{
				mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_POS;
				mark.m_nTypeMarkDir				= TYPE_MARKDIR_RIGHTTAN;
				if(bExtendToSttOfMain)
					mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_UPPER;
				else 
					mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_LOWER;

				if(bSttBracket || bEndBracket)
				{
					if(tvArrSup.GetSize() > 1)
					{
						CTwinVector tv[2]	= {tvArrSup.GetAt(0), tvArrSup.GetAt(1)};
						CTwinVector tvMid	= tv[0];
						tvMid.m_v1			= (tv[0].m_v1 + tv[1].m_v1)/2;
						tvMid.m_v2			= (tv[0].m_v2 + tv[1].m_v2)/2;
						tv[nCycle]			= tvMid;
						tvArrSup.RemoveAll();
						tvArrSup.Add(tv[0]);
						tvArrSup.Add(tv[1]);
						mark.SetRebar(tvArrMain, tvArrSup);
					}

					mark.m_nTypeMarkDir				= nCycle == 0 ? TYPE_MARKDIR_LEFTTAN : TYPE_MARKDIR_RIGHTTAN;
					mark.m_dPosScaleMarkStt			= nCycle == 0 ? 0.25 : 0.75;
					mark.m_dPosScale				= 0.5;
					mark.m_dPosScaleSpace			= 0.25;
				}
			}

			// 마크선 연장
			CDPoint xyMark					= mark.GetXyMark();	// 이 함수에서 마크할 위치가 구해짐
			CDPoint xyMatch(0, 0);
			CDPointArray xyArrMatch;
			if(bSttBracket || bEndBracket)
			{
				pBri->GetTvPlaneOutWall(tvArrEnd, bSttBracket, FALSE, FALSE);
			}
		
			if(mark.m_xyArrCountMark.GetSize() > 1)
			{
				CDPoint xyDir	= (mark.m_xyArrCountMark.GetAt(0) - mark.m_xyArrCountMark.GetAt(1)).Unit();
				if(i == 0)
					rb.GetXyMatchTvArrAndLine(tvArrEnd, xyMark, tvArrMain.GetAt(0).GetXyDir(), xyArrMatch);
				else if(i == 1)
					rb.GetXyMatchTvArrAndLine(tvArrLeft, xyMark, xyDir, xyArrMatch);
				else if(i == 2)
					rb.GetXyMatchTvArrAndLine(tvArrRight, xyMark, xyDir, xyArrMatch);
			}

			if(xyArrMatch.GetSize() > 0)
			{
				xyMatch	= xyArrMatch.GetAt(0);
				mark.m_dAddHeight				= ~(xyMark - xyMatch) + Dom.Always(MARK_BASE_HEIGHT);
			}

			if(!bSttBracket && !bEndBracket && i == 0)
			{
				if(nDan > 0)
					mark.m_dAddHeight	+= Dom.Always(MARK_BASE_HEIGHT);

				if(nCycle > 0)
					mark.m_dAddHeight	+= Dom.Always(MARK_BASE_HEIGHT);
			}

			if(!bExtendToSttOfMain && i == 0)
				mark.m_dAddHeight	+= Dom.Always(MARK_BASE_HEIGHT);

			if(bEndBracket)
			{
				if(nCycle == 0)
					mark.m_dAddHeight	+= pBri->m_dWE/4*3;
				mark.m_dAddHeight -= pBri->m_dWE;
			}
			
			// 다 구한뒤 마크할 위치에 대해서 절단원 적용
			// (ARCBRIDGE-2564) 이부분이 주석 처리 되있어서 철근 마킹이 겹치고 있다.
			if(!bHunch && !bSttBracket && !bEndBracket)
			{
				//ApplyCuttingArea_Plane(&Dom, mark.m_xyArrCountMark, bUpper, nDan, FALSE, FALSE, bSttBracket, bEndBracket, bHunch, pRC->m_nPosHunch);
			}

			if(mark.m_xyArrCountMark.GetSize() > 0)
			{
				CRebar *pRb = (bSttBracket || bEndBracket)? pBri->GetRebarEtcByDescription(sDescription, FALSE) : pBri->GetRebarMainByDescription(sDescription);
				mark.m_dCtc						= pBri->GetCtcRebarMain(TRUE);
				mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;

				mark.MarkRebar(i == 0 ? FALSE : TRUE);	
			}
		}
	}

	*pDomP << Dom;
}

// 횡단 구간 그리기
void CDrawPlane::DrawPlaneSlabCrossBlock(CDomyun* pDomP)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	CLineInfo *pLine = pBri->GetLineBase();

	// 확폭일때는 어짜피 안맞으니까 그리지 말자.
	if(pBri->IsExtLine())
		return;

	int nQtyHDan = pBri->GetQtyHDan(FALSE);
	double dSta = pBri->GetStationBridgeMid(); //pBri->GetStationOnJijum(0, 0);
	double staStt(0), staEnd(0);

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	double dDist = 0;
	
	double dDistLeft	= -pBri->GetWidthSlabAct(dSta, CDPoint(0, 1), -1);
	int i=0; for(i=0; i<nQtyHDan-1; i++)
	{
		dDist += pBri->GetLengthHDanAct(dSta, CDPoint(0, 1), i);
		double dWidth = dDistLeft + dDist;

		CGuardWallRC* pGW1 = pBri->GetGuardWallByHDan(i);
		if(!pGW1)	continue;
		BOOL bLaneCurr = pGW1->IsTypeHDanOnlyNone();
		CGuardWallRC* pGW2 = pBri->GetGuardWallByHDan(i+1);
		if(!pGW2)	continue;
		BOOL bLaneNext = pGW2->IsTypeHDanOnlyNone();

		if(bLaneCurr && bLaneNext) continue;
		
		staStt = pBri->GetStationAtSlabEnd(TRUE, 0, dWidth);
		staEnd = pBri->GetStationAtSlabEnd(FALSE, 0, dWidth);
		pLine->DrawLS(&Dom, staStt, staEnd, dWidth);
	}

	*pDomP << Dom;
}

// 슬래브 하부 헌치 그리기
void CDrawPlane::DrawPlaneSlabHunch(CDomyun* pDomP)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONH);

	CHgBaseDrawStd baseDraw;
	CTwinVectorArray tvArr;
	CTwinVector tv;
	double dHW(0), dOffset(0);
	BOOL bStt = FALSE;

	CRebarPlacing rb;
	CTwinVectorArray tvArrCen;
	pBri->GetTvCenterPlane_UpperSlab(tvArrCen, TRUE, TRUE, 1000, FALSE);

	// 시종점
	int i=0; for(i=0; i<0/*2*/; i++)
	{
		bStt = i==0;
		double se = i==0 ? -1 : 1;
		dHW = i==0 ? pBri->m_dHWS : pBri->m_dHWE;
		dOffset = dHW/3.0;
		for(int j=0; j<3; j++)
		{
			pBri->GetTvPlaneSttEndLine(tvArr, bStt, FALSE);
			CDPoint xyMatch(0, 0);
			long k=0; for(k=0; k<3; k++)
			{
				tv = tvArr.GetAt(k);
				Offset(tv.m_v1, tv.m_v2, dOffset*(j+1)*se);
				tvArr.SetAt(k, tv);
			}
			CTwinVector tv1, tv2;
			for(k=0; k<2; k++)
			{
				tv1 = tvArr.GetAt(k);
				tv2 = tvArr.GetAt(k+1);
				if(GetXyMatchLineAndLine(tv1.m_v1, tv1.GetXyDir(), tv2.m_v2, tv2.GetXyDir(), xyMatch))
				{
					tv1.m_v2 = tv2.m_v1 = xyMatch;
					tvArr.SetAt(k, tv1);
					tvArr.SetAt(k+1, tv2);
				}
			}

			double dSta(0);
			double dDistL = pBri->m_dWidthSlabLeft;
			double dDistR = pBri->m_dWidthSlabRight;

			CLineInfo* pLineL = pBri->GetLineLeftRight(TRUE);
			CDPoint xy1 = tvArr.GetAt(0).m_v1;
			CDPoint xy2 = tvArr.GetAt(0).m_v2;
			int nIndex = xy1==xy2 ? 1 : 0;
			tv = tvArr.GetAt(nIndex);
			pLineL->GetStationMatchByXyAndAngle(tv.m_v1, tv.GetXyDir(), dSta, dDistL);
			tv.m_v1 = pLineL->GetXyLineDis(dSta, dDistL);
			tvArr.SetAt(nIndex, tv);

			CLineInfo* pLineR = pBri->GetLineLeftRight(FALSE);
			xy1 = tvArr.GetAt(2).m_v1;
			xy2 = tvArr.GetAt(2).m_v2;
			nIndex = xy1==xy2 ? 1 : 2;
			tv = tvArr.GetAt(nIndex);
			pLineR->GetStationMatchByXyAndAngle(tv.m_v2, tv.GetXyDir(), dSta, dDistR);
			tv.m_v2 = pLineR->GetXyLineDis(dSta, dDistR);
			tvArr.SetAt(nIndex, tv);

			// 상면은 점선, 하면은 실선으로 그림
			CTwinVectorArray tvArrLeft, tvArrRight;
			tvArrLeft	= tvArr;
			tvArrRight	= tvArr;
			rb.TrimTvArrayByTvArray(tvArrLeft, tvArrCen, FALSE, TRUE);
			rb.TrimTvArrayByTvArray(tvArrRight, tvArrCen, TRUE, TRUE);

			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONH);
			baseDraw.DrawTvArray(&Dom, tvArrLeft, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());

			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
			baseDraw.DrawTvArray(&Dom, tvArrRight, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());
		}
	}

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONH);
	// 중간 벽체
	tvArr.RemoveAll();
	int nCountInWall = pBri->GetCountInWall();
	for(i=0; i<nCountInWall; i++)
	{
		CWallApp *pWall = pBri->GetInWall(i);
		if(pWall)
		{
			// 기둥식일 경우 벽체평면 좌표는 기둥 좌표를 가지므로 헌치를 
			// 구하기 위해서 기둥식을 벽식으로 속여서 평면을 구한뒤 복구한다.
			BOOL bTopIsColumn	= pWall->m_bTopIsColumn;
			if(bTopIsColumn)
			{
				pWall->m_bTopIsColumn	= FALSE;
				pWall->GetTvPlan();
			}
			
			// 끝단 라운드 벽체일 경우 일반 벽체로 속여서 그린뒤 복구한다.///
			BOOL bRoundRect	= pWall->IsEnableRoundRectWall() && pWall->m_bRoundRect;
			CTwinVectorArray tvArrPlane;
			tvArrPlane	= pWall->m_tvArrPlan;
			if(bRoundRect)
			{
				pWall->m_bRoundRect	= FALSE;
				pWall->GetTvPlan();
			}

			if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH)
			{
				for(int j=0; j<2; j++)
				{
					CString strDir = j==0 ? _T("좌측") : _T("우측");
					
					//Issue Number: #27985 
					CTwinVector tv, tvTemp, tvLeftVertDirHunch, tvRightVertDirHunch;
					CDPoint xyCenVertDirHunch;
					CString strLeft = _T("");
					CString strRight = _T("");

// 					double dPierOffSetDis = 0.0;

					strLeft.Format(_T("%d번째내측헌치좌측면"), i + 1);
					strRight.Format(_T("%d번째내측헌치우측면"), i + 1);

					pBri->m_tvArrVertSection.GetTvByInfo(strLeft, tvLeftVertDirHunch);
					pBri->m_tvArrVertSection.GetTvByInfo(strRight, tvRightVertDirHunch);
					
					xyCenVertDirHunch = (tvLeftVertDirHunch.m_v1+tvRightVertDirHunch.m_v2) / 2;
					double dPierOffSetDis = xyCenVertDirHunch.x - tvLeftVertDirHunch.m_v1.x;

					if(j != 0)
						dPierOffSetDis = tvRightVertDirHunch.m_v2.x -  xyCenVertDirHunch.x;

					dHW = j==0 ? pBri->m_dHLW[i][iUPPER] : pBri->m_dHRW[i][iUPPER];
					dOffset = dHW/3.0;
					CDoubleArray dArrOffSet;
					
					dArrOffSet.Add(dPierOffSetDis);

					for(long nIdx = 0; nIdx < 3; nIdx++)
					{
						double dValue = dOffset*(nIdx+1) + dPierOffSetDis;
						dArrOffSet.Add(dValue);
					}
					//
				
					for(long k=0; k<dArrOffSet.GetSize(); k++)
					{
						double dOffSetDis = dArrOffSet.GetAt(k);
						if(dOffSetDis > 0.0)
						{
							pWall->m_tvArrPlan.GetTvByInfo(_T("벽체평면")+strDir, tv);
							Offset(tv.m_v1, tv.m_v2, dOffSetDis);
							tvTemp = tv;
							double dSta(0);
							double dDistL = pBri->m_dWidthSlabLeft + pBri->m_dDH[0];
							double dDistR = pBri->m_dWidthSlabRight - pBri->m_dDH[1];

							CLineInfo* pLineL = pBri->GetLineLeftRight(TRUE);
							pLineL->GetStationMatchByXyAndAngle(tv.m_v1, tv.GetXyDir(), dSta, dDistL);
							tvTemp.m_v1 = pLineL->GetXyLineDis(dSta, dDistL);

							CLineInfo* pLineR = pBri->GetLineLeftRight(FALSE);
							pLineR->GetStationMatchByXyAndAngle(tv.m_v2, tv.GetXyDir(), dSta, dDistR);
							tvTemp.m_v2 = pLineR->GetXyLineDis(dSta, dDistR);

							tvArr.Add(tvTemp);
						}
						
					}

				}
				
			}
			else
				tvArr	= tvArrPlane;

			// 모두 점선으로 그림
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONH);
			baseDraw.DrawTvArray(&Dom, tvArr, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());

			// 기둥식일 경우 벽체평면 좌표는 기둥 좌표를 가지므로 헌치를 
			// 구하기 위해서 기둥식을 벽식으로 속여서 평면을 구한뒤 복구한다.
			pWall->m_bTopIsColumn	= bTopIsColumn;
			pWall->GetTvPlan();
			if(bRoundRect)
			{
				pWall->m_bRoundRect	= bRoundRect;
				pWall->GetTvPlan();
			}

		}
	}

	*pDomP << Dom;
}

void CDrawPlane::MarkGenCrossSlope(CDomyun *pDomP)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;

	CDomyun Dom(pDomP);
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	double dSpace = Dom.Always(10);
	CString strSlope = _T("");
	CDPoint xy(0, 0);

	CLineInfo* pLine = pBri->GetLineBase();
	double dStaFir = pBri->GetStationOnJijum(0, 0);
	double dStaSec = pBri->GetStationOnJijum(1, 0);
	double dSta = (dStaFir+dStaSec)/2.0;
	CDPoint vAng = pLine->GetAngleAzimuth(dSta);

	// 좌측 편경사
	if(pBri->m_dWidthSlabLeft<0.0)
	{
		double staStt  = pBri->m_dStationBridgeStt;
		double staEnd  = pBri->m_dStationBridgeStt + pBri->m_dLengthBridge;
		double dSlope1 = pBri->GetLineBase()->GetPyungubae(staStt,-1000);
		double dSlope2 = pBri->GetLineBase()->GetPyungubae(staEnd,-1000);
		if(Compare(dSlope1,dSlope2,"="))
			strSlope.Format(_T("%.3f%%"), dSlope1*100.0);
		else
			strSlope = "VAR";
		xy = pLine->GetXyLineDis(dSta, -dSpace);
		Sym.ArrowDirectionForSlope(&Dom, xy, strSlope, CDPoint(1, 0));
		Dom.Rotate(xy, vAng.Rotate90());
	}
	*pDomP << Dom;

	// 우측 편경사
	if(pBri->m_dWidthSlabRight>0.0)
	{
		double staStt  = pBri->m_dStationBridgeStt;
		double staEnd  = pBri->m_dStationBridgeStt + pBri->m_dLengthBridge;
		double dSlope1 = pBri->GetLineBase()->GetPyungubae(staStt,+1000);
		double dSlope2 = pBri->GetLineBase()->GetPyungubae(staEnd,+1000);
		if(Compare(dSlope1,dSlope2,"="))
			strSlope.Format(_T("%.3f%%"), dSlope1*100.0);
		else
			strSlope = "VAR";
		xy = pLine->GetXyLineDis(dSta, dSpace);
		Sym.ArrowDirectionForSlope(&Dom, xy, strSlope, CDPoint(-1, 0));
		Dom.Rotate(xy, vAng.Rotate90());
	}
	*pDomP << Dom;
}
// 중간벽체와 내측헌치 평면도(점선)
void CDrawPlane::DrawPlaneGenInWallAndHunch(CDomyun *pDomP, BOOL bUpperSlab)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONH);
	
	
	CHgBaseDrawStd baseDraw;
	long nCountInWall	= pBri->GetCountInWall();
	long i = 0; for(i = 0; i < nCountInWall; i++)
	{
		CWallApp *pWall	= pBri->GetInWall(i);
		if(!pWall) continue;
	
		// 중간벽체
		m_pStd->m_pDrawWall->DrawPlan(&Dom, pWall, FALSE);

		// 내측헌치
		CTwinVectorArray tvArrLeft, tvArrRight;
		pBri->GetTvPlaneInWallHunch(i, tvArrLeft, tvArrRight, bUpperSlab);

		baseDraw.DrawTvArray(&Dom, tvArrLeft);
		baseDraw.DrawTvArray(&Dom, tvArrRight);
	}

	*pDomP << Dom;
}
void CDrawPlane::DrawPlaneGenOutWall(CDomyun *pDomP, BOOL bStt, BOOL bHidden)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, bHidden ? HCAD_CONH : HCAD_CONC);

	CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;

	CTwinVectorArray tvArr;
	pBri->GetTvWingWallPlane(bStt, FALSE);
	CTwinVectorArray tvArrPlane;
	tvArrPlane = pFooting->m_tvArrplan;
	tvArrPlane.Sort();
	CRebarPlacing rb;
	tvArr	= pBri->m_tvArrPlaneWingWall;
	rb.GetTvArrayByCrossTvArrayArea(tvArr, tvArrPlane, TRUE);
	//m_pStd->GetTvArrTrimByPolygon(pBri->m_tvArrPlaneWingWall, tvArrPlane, TRUE, tvArr);
	baseDraw.DrawTvArray(&Dom, tvArr);


	*pDomP << Dom;
}

// 벽체 평면도 그리기(기초에 의해 잘리는 부분 표현)
void CDrawPlane::DrawPlaneGenWallByJijum(CDomyun *pDomP, long nJ, BOOL bHidden)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);

	if(nJ == 0 || nJ == pBri->m_nQtyJigan)
	{
		BOOL bStt	= nJ == 0;
		DrawPlaneGenOutWall(&Dom, bStt, bHidden);
	}
	else
	{
		long nIdxInWall	= nJ - 1;
		CWallApp *pWall	= pBri->GetInWall(nIdxInWall);
		if(pWall)
		{
			if(pWall->m_bIs)
			{
				m_pStd->m_pDrawWall->DrawPlan(&Dom, pWall, FALSE, bHidden);
			}
		}
	}

	*pDomP << Dom;
}

// 거더 철근
// 슬래브 좌측에 지점마다 그림.
void CDrawPlane::DrawRebarMain_Plane_SlabGirder(CDomyun *pDomP, BOOL bUpperSlab, long nJ, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CTwinVectorArray tvArr, tvArrAngLeft, tvArrAngRight;
	pBri->GetTvRebarMain_Plane_SlabGirder(bUpperSlab, tvArr, nJ, nDrawLeft);

	if(tvArr.GetSize() == 0) return;
	
	if(bDimRebar)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	}

	// 절단원 적용해서 그림 
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_HIDDEN_LINE);
	ApplyCuttingArea_Plane(&Dom, tvArr, TRUE, 0, TRUE, FALSE, FALSE, FALSE, FALSE, nJ);
	baseDraw.DrawTvArray(&Dom, tvArr);
		
	if(bMarkRebar)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	}

	*pDomP << Dom;
}
// 가각부 주철근 그리기.
void CDrawPlane::DrawRebarMain_Plane_Slab_Exp(CDomyun *pDomP, BOOL bUpperSlab, BOOL bUpper, long nDan, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft, BOOL bInput)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(!pBri->m_bIsExp) return;


	CHgBaseDrawStd baseDraw;
	CRebarPlacing rb;
	CDomyun Dom(pDomP);

	// 가각부 배근을 위해서 배력철근을 미리 구해둠
	CTwinVectorArray tvArrSup, tvArrAngStt, tvArrAngEnd, tvArrRAngStt, tvArrRAngEnd;
	pBri->GetTvRebarSupport_Plane_Slab_Std(bUpperSlab, tvArrSup, tvArrAngStt, tvArrAngEnd, tvArrRAngStt, tvArrRAngEnd, bUpper, 0, 0, TRUE);
	tvArrSup.AddFromTvArray(tvArrAngStt);
	tvArrSup.AddFromTvArray(tvArrAngEnd);
	tvArrSup.AddFromTvArray(tvArrRAngStt);
	tvArrSup.AddFromTvArray(tvArrRAngEnd);

	CTwinVector tvStt, tvEnd;
	pBri->GetTvSttEndOfSlab(tvStt, tvEnd);
			

	long type = 0;
	CTwinVectorArrayArray pTvArrCycle1[2];
	CTwinVectorArrayArray pTvArrCycle2[2];
		
	CTwinVectorArrayArray pTvArrCycle1Hidden;
	CTwinVectorArrayArray pTvArrCycle2Hidden;

	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		long left = 0; for(left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT;
			if(nDrawLeft == -1 && !bLeft) continue;
			if(nDrawLeft == 1 && bLeft) continue;


			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

			CTwinVectorArray tvArrCycleHunch1, tvArrCycleHunch2;
			CDPointArray xyArrCycle1, xyArrCycle2;
			CDPointArray xyArrDimPoint;
			pBri->GetTvRebarMain_Plane_Slab_Exp(bUpperSlab, &pTvArrCycle1[0], &pTvArrCycle2[0], &pTvArrCycle1[1], &pTvArrCycle2[1], bStt, bLeft, nDan, bUpper, tvArrCycleHunch1, tvArrCycleHunch2, FALSE, xyArrCycle1, xyArrCycle2, xyArrDimPoint); 

			for(type = 0; type < 2; type++)
			{
				if(bDimRebar)
				{
					m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
					CDPointArray xyArrDim;
					CStringArray sArrText;
					CStringArray sArrUnderText;
					long nDimDan	= 1;
					CString sDir	= bStt ? "TOP" : "BOTTOM";
					CDPoint xyStt	= CDPoint(0, 0);

					xyArrDim	= xyArrDimPoint;
					if(xyArrDim.GetSize() > 0)
					{
						double dL	= bStt ? pBri->m_dExpSttW[bLeft ? 0 : 1] : pBri->m_dExpEndW[bLeft ? 0 : 1];
						double dH	= bStt ? pBri->m_dExpSttH[bLeft ? 0 : 1] : pBri->m_dExpEndH[bLeft ? 0 : 1];
						double dObq = 90 - ToDegree(atan(fabs(dL)/fabs(dH)));
						Dom.SetDimObq(bLeft ? dObq : -dObq);

						if(bLeft)
							rb.ReverseRebar(xyArrDim);
						xyStt	= xyArrDim.GetAt(0);
						baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, TRUE, FALSE, TRUE, TRUE);
					}

					*pDomP << Dom;

					// 철근 길이 치수(처음 철근 하나만 기입하자)
					xyArrDim.RemoveAll();
					sArrText.RemoveAll();

					
					if(pBri->m_nTypeRebarMainRebarOfExp[stt][left] == 0 && pTvArrCycle1[type].GetSize() > 0)
					{
						CTwinVectorArray *pTvArrTmp1	= pTvArrCycle1[type].GetAt(0);

						CDPointArray xyArrMatch;
						CTwinVectorArray tvArrOrg;
						long nTypeOld	= pBri->m_nTypeOrgMainRebarOfExp[stt][left];
						pBri->m_nTypeOrgMainRebarOfExp[stt][left]	= 0;
						pBri->GetTvMainRebarOrgOfExp(tvArrOrg, bStt, bLeft);
						pBri->m_nTypeOrgMainRebarOfExp[stt][left]	= nTypeOld;
						
						if(tvArrOrg.GetSize() > 0 && pTvArrTmp1->GetSize() > 0)
						{
							CTwinVector tvOrg	= tvArrOrg.GetAt(0);
							CTwinVector tv		= pTvArrTmp1->GetAt(0);
							tv.m_v2	= pTvArrTmp1->GetAt(pTvArrTmp1->GetSize()-1).m_v2;
							CDPoint xyMatch(0, 0);
							if(!bStt)
								GetSwap(tv.m_v1, tv.m_v2);

							sDir	= bLeft ? "TOP" : "BOTTOM";
							nDimDan	= 0;
							CString str	= _T("");
							str.Format("%s%s", bStt ? "S" : "E", bLeft ? "L" : "R");

							xyArrDim.Add(tv.m_v1);
							sArrText.Add("0");

							rb.GetXyMatchTvArrAndLine(tvArrOrg, tv.m_v1, tv.GetXyDir(), xyArrMatch);
							if(xyArrMatch.GetSize() > 0)
							{
								if(xyArrMatch.GetSize() == 1)
									xyMatch	= xyArrMatch.GetAt(0);
								else
									xyMatch	= ~(xyArrMatch.GetAt(0) - tv.m_v1) < ~(xyArrMatch.GetAt(1) - tv.m_v2) ? xyArrMatch.GetAt(0) : xyArrMatch.GetAt(1);

								xyArrDim.Add(xyMatch);
								sArrText.Add(bStt ? "0" : str);
							}
							
							xyArrDim.Add(tv.m_v2);
							sArrText.Add(bStt ? str : "0");
		

							if(!bInput)
								sArrText.RemoveAll();

							if(xyArrDim.GetSize() > 0)
							{
								xyStt	= xyArrDim.GetAt(0);

								Dom.SetDimObq(0);
								baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, TRUE, FALSE, TRUE, TRUE);
							}
						}

						*pDomP << Dom;
					}
				}

				// 절단원 적용해서 그림  ////////////////////////////
				m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
				double dLenRound	= pBri->GetLenRebarAtEdge(pBri->GetDiaMainRebarGagak(bStt, bLeft));
				// 가각철근을 선형방향으로 하면 1Cycle에는 보강철근이
				// 2Cycle에는 선형방향 철근이 들어온다.(선형방향 철근만 라운드 표시 함)
				if((bUpper == bUpperSlab) && !pBri->m_bLineDirRebarOfExp[stt][left])
				{
					// (ARCBRIDGE-2691) 가각부 철근 배치 방법에 따라 철근의 방향이 다르다. 
					if(pBri->m_bLineDirRebarOfExp[stt][left])
						m_pStd->DivideTvArrWithRound(&pTvArrCycle1[type], &pTvArrCycle1Hidden, bStt? dLenRound : 0, bStt? 0 : dLenRound);
					else
						m_pStd->DivideTvArrWithRound(&pTvArrCycle1[type], &pTvArrCycle1Hidden, dLenRound, 0);
				}
				if(bUpper == bUpperSlab)
				{
					if(pBri->m_bLineDirRebarOfExp[stt][left])
						m_pStd->DivideTvArrWithRound(&pTvArrCycle2[type], &pTvArrCycle2Hidden, bStt? dLenRound : 0, bStt? 0 : dLenRound);
					else
						m_pStd->DivideTvArrWithRound(&pTvArrCycle2[type], &pTvArrCycle2Hidden, dLenRound, 0);
				}

				if(!bInput)
				{
					ApplyCuttingArea_Plane(&Dom, &pTvArrCycle1[type], bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, 0, FALSE, FALSE, NULL, bUpperSlab);
					if(type == 0)
						ApplyCuttingArea_Plane(&Dom, &pTvArrCycle1Hidden, bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, 0, FALSE, FALSE, NULL, bUpperSlab);
					ApplyCuttingArea_Plane(&Dom, &pTvArrCycle2[type], bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, 0, FALSE, FALSE, NULL, bUpperSlab);
					if(type == 0)
						ApplyCuttingArea_Plane(&Dom, &pTvArrCycle2Hidden, bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, 0, FALSE, FALSE, NULL, bUpperSlab);


					if(type == 0)
					{
						ApplyCuttingArea_Plane(&Dom, tvArrCycleHunch1, FALSE, nDan, FALSE, FALSE, FALSE, FALSE, TRUE, bStt ? 0 : pBri->m_nQtyJigan, FALSE, FALSE, NULL, bUpperSlab);
						ApplyCuttingArea_Plane(&Dom, tvArrCycleHunch2, FALSE, nDan, FALSE, FALSE, FALSE, FALSE, TRUE, bStt ? 0 : pBri->m_nQtyJigan, FALSE, FALSE, NULL, bUpperSlab);

						ApplyCuttingArea_Plane(&Dom, xyArrCycle1, FALSE, nDan, FALSE, FALSE, FALSE, FALSE, TRUE, bStt ? 0 : pBri->m_nQtyJigan, FALSE, FALSE, NULL, bUpperSlab);
						ApplyCuttingArea_Plane(&Dom, xyArrCycle2, FALSE, nDan, FALSE, FALSE, FALSE, FALSE, TRUE, bStt ? 0 : pBri->m_nQtyJigan, FALSE, FALSE, NULL, bUpperSlab);
					}
				}

				// 가각부 마크 - 절단원을 적용한후에 마킹해야..
				if(bMarkRebar)
				{
					m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
					CDPoint xyDirMain(0, 0);
					CDPointArray xyArrCount;
					CTwinVectorArray tvArr;
					
					CConcSymbol sym(m_pStd->m_pARoadOptionStd);

					for(long cycle = 0; cycle < 2; cycle++)
					{
						CTwinVectorArrayArray *pTvArr	= NULL;
						pTvArr	= (cycle == 0) ? &pTvArrCycle1[type] : &pTvArrCycle2[type];

						if(pTvArr->GetSize() == 0) continue;
						if(pTvArr->GetAt(pTvArr->GetSize()/2)->GetSize() == 0) continue;
						CTwinVectorArray tvArrTmp;
						tvArrTmp	= *pTvArr->GetAt(0);
						if(tvArrTmp.GetLength() == 0)
						{
							long i = 0; for(i = 1; i < pTvArr->GetSize(); i++)
							{
								tvArrTmp	= *pTvArr->GetAt(i);
								if(tvArrTmp.GetLength() > 0) break;
							}
						}
						if(tvArrTmp.GetSize() == 0) continue;
						CTwinVector tvMain	= tvArrTmp.GetAt(0);
						if(tvMain.m_v1.z == -1 || tvMain.m_v1.z == 1)
						{
							CDPoint xyCen	= tvMain.GetXyCircleCenter();
							xyDirMain	= (bStt && bLeft) || (!bStt && !bLeft) ? (tvMain.m_v1 - xyCen).Unit().Rotate90() : (tvMain.m_v1 - xyCen).Unit().RotateInv90();
						}
						else
						{
							xyDirMain	= tvMain.GetXyDir();
						}
					
						CString sMark	= _T("");
						CString sDescription	= _T("");
						double dDia		= 0;
						if(bUpperSlab)
							pBri->GetInfoMainRebarExp_UpperSlab(bStt, bLeft, bUpper, cycle, nDan, type, sMark, dDia, sDescription);
						else
							pBri->GetInfoMainRebarExp_LowerSlab(bStt, bLeft, bUpper, cycle, nDan, type, sMark, dDia, sDescription);

						CRebar *pRb = pBri->GetRebarMainByDescription(sDescription);
						// count
						CMarkDomyun mark;//(&Dom, m_pStd->m_pARoadOptionStd);
						mark.SetDomyun(&Dom, m_pStd->m_pARoadOptionStd);
						mark.SetRebar(pTvArr, tvArrSup);
						mark.m_sMark		= sMark;
						mark.m_sDia			= pBri->m_pARcBridgeDataStd->GetStringDia(dDia, pBri->GetValueFy(bUpperSlab? ePartUpperSlab : ePartLowerSlab));
						mark.m_dPosScale	= bStt ? (cycle==0?0.7:0.9) : (cycle==1?0.1:0.3);
						mark.GetXyCountMark(xyArrCount);
						mark.m_dCtc = pBri->GetCtcRebarMain(TRUE);
						mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;

						if(xyArrCount.GetSize() > 0)
						{
							mark.MarkRebar(FALSE, TRUE);

							// 넘버링
							CDPoint xyStt(0, 0), xyEnd(0, 0);
							long nSize	= xyArrCount.GetSize();
							if(nSize > 0)
							{
								if(nSize == 1)
								{
									xyStt	= xyArrCount.GetAt(0);

									if(bStt)
										GetXyMatchLineAndLine(xyStt, xyDirMain, tvStt.m_v1, tvStt.GetXyDir(), xyEnd);
									else
										GetXyMatchLineAndLine(xyStt, xyDirMain, tvEnd.m_v1, tvEnd.GetXyDir(), xyEnd);

									Dom.LineTo(xyStt, xyEnd);
								}
								else
								{
									CDPoint xyPrev	= xyArrCount.GetAt(cycle==0?nSize/4:nSize/4*2);
									CDPoint xyNext	= xyArrCount.GetAt(cycle==0?nSize/4+1:nSize/4*2+1);
									xyStt	= xyPrev + (xyNext-xyPrev).Unit() * ~(xyNext-xyPrev)/4;

									CDPoint vAng	= (xyNext - xyPrev).Unit();
									CTwinVectorArray tvArrLine;
									CDPointArray xyArrMatch;
									CDPoint xyMatch(0, 0);
									long i = 0; for(i = 0; i < pTvArr->GetSize(); i++)
									{
										tvArrLine	= *pTvArr->GetAt(i);
										rb.GetXyMatchTvArrAndLine(tvArrLine, xyPrev, vAng, xyArrMatch);
										if(xyArrMatch.GetSize() > 0) break;
									}
									if(xyArrMatch.GetSize() > 0)
									{
										xyMatch	= xyArrMatch.GetAt(0);
										CDPoint xyMove	= (xyStt - xyMatch);

										rb.MoveRebar(tvArrLine, xyMove);

										CTwinVectorArray tvArrTrim;
										CTwinVector tvTrim;
										tvTrim.m_v1	= xyStt;
										tvTrim.m_v2	= tvTrim.m_v1 + pBri->GetAngleAzimuthJijum(bStt ? 0 : pBri->m_nQtyJigan) * 100;
										tvTrim.m_v1 += tvTrim.GetXyDir(TRUE) * 1000;
										tvTrim.m_v2 += tvTrim.GetXyDir() * 1000;
										tvArrTrim.Add(tvTrim);
										rb.TrimAndStretchByTvAndLength(tvArrLine, tvArrTrim, 0);
										baseDraw.DrawTvArray(&Dom, tvArrLine);

										xyEnd	= tvArrLine.GetAt(0).m_v1;
									}
									
								}
							}
							*pDomP << Dom;

											
							// sym.RebarMarkLineSide(&Dom, xyEnd, mark.m_sMark, mark.m_sDia, bStt, 0, FALSE);

							SRebarMarkInputData mark_input_data;
							SRebarMarkviewData mark_view_data;
							mark_input_data.mark = mark.m_sMark;
							mark_input_data.dia = mark.m_sDia;
							mark_input_data.position = xyEnd;
							mark_input_data.ctc = mark.m_dCtc;
							mark_input_data.type_rebar = mark.m_nTypeRebar;
							mark_input_data.left_dir = bStt;
							mark_input_data.height = 0;

							mark_view_data.count_mark = FALSE;
							mark_view_data.line_rebar = TRUE;
							mark_view_data.mark_pos = eMarkSide;

							sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

							if(bStt)
								Dom.Rotate(xyEnd, ToDPointFrDegree(xyDirMain.GetAngleDegree()));
							else
								Dom.Rotate(xyEnd, ToDPointFrDegree(180+xyDirMain.GetAngleDegree()));
							*pDomP << Dom;
						}
					}
				}

				m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
				m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_SOLID);
				baseDraw.DrawTvArray(&Dom, &pTvArrCycle1[type]);
				baseDraw.DrawTvArray(&Dom, &pTvArrCycle2[type]);

				m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_HIDDEN_LINE);
				baseDraw.DrawTvArray(&Dom, &pTvArrCycle1Hidden);
				baseDraw.DrawTvArray(&Dom, &pTvArrCycle2Hidden);

				

				// 하면이고 type==0이면 헌치부 철근도 마킹(하부슬래브는 상면)
				//if((!bUpper == bUpperSlab) && type == 0)
				// (ARCBRIDGE-3195) 헌치부 철근은 DrawRebarMain_Plane_UpperSlabHunch() 함수에서처리
				if(FALSE)
				{
					baseDraw.DrawTvArray(&Dom, tvArrCycleHunch1);
					baseDraw.DrawTvArray(&Dom, tvArrCycleHunch2);

					if(bMarkRebar)
					{
						CString sMark	= _T("");
						CString sDia	= _T("");
						CString sDescription	 = _T("");
						double dDia	= 0;
						

						long cycle	= 0;
						for(cycle = 0; cycle < 2; cycle++)
						{
							if(bUpperSlab)
								pBri->GetInfoMainRebarExp_UpperSlabHunch(bStt, bLeft, cycle, sMark, dDia, sDescription);
							else
								pBri->GetInfoMainRebarExp_LowerSlabHunch(bStt, bLeft, cycle, sMark, dDia, sDescription);
							sDia	= pBri->m_pARcBridgeDataStd->GetStringDia(dDia, pBri->GetValueFy(bUpperSlab? ePartUpperSlab : ePartLowerSlab));

							CTwinVectorArray *pTvArr	= cycle == 0 ? &tvArrCycleHunch1 : &tvArrCycleHunch2;
							rb.ReverseRebar(*pTvArr);

							CRebar *pRb = pBri->GetRebarMainByDescription(sDescription);
							CMarkDomyun mark;
							mark.SetDomyun(&Dom, m_pStd->m_pARoadOptionStd);
							mark.SetRebar(*pTvArr, tvArrSup);
							mark.m_sMark	= sMark;
							mark.m_sDia		= sDia;
							mark.m_dPosScale	= cycle == 0 ? 0.4 : 0.6;
							mark.m_dAddHeight	= pBri->m_dCTC_MainRebar*2;
							mark.m_dCtc = pBri->GetCtcRebarMain(TRUE);
							mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;

							mark.MarkRebar();
						}
					}
				}


				if(!bInput)
				{
  					m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);
					baseDraw.DrawPointRebar(&Dom, xyArrCycle1);
					baseDraw.DrawPointRebar(&Dom, xyArrCycle2);
				}
				*pDomP << Dom;
				////////////////////////////////////////////////////////
			}
		}
	}

	for(type = 0; type < 2; type++)
	{
		AhTPADelete(&pTvArrCycle1[type], (CTwinVectorArray*)0);
		AhTPADelete(&pTvArrCycle2[type], (CTwinVectorArray*)0);
	}

	AhTPADelete(&pTvArrCycle1Hidden, (CTwinVectorArray*)0);
	AhTPADelete(&pTvArrCycle1Hidden, (CTwinVectorArray*)0);

	*pDomP << Dom;
}

void CDrawPlane::MakrPlaneConcFck(CDomyun *pDomP)
{
	CRcBridgeApp *pBri  = m_pStd->m_pRcBridge;
	CLineInfo    *pLine = pBri->GetLineBase();

	long nJijum  = 0;
	CDPoint vAng = pBri->GetAngleJijum(nJijum);

	// 상부슬래브 (본체)
	double dDist13   = pBri->m_dWidthSlabLeft + (pBri->m_dWidthSlabRight-pBri->m_dWidthSlabLeft)/3.0;
	double dStaJijum = pBri->GetStationOnJijum(nJijum);
	double dStaMark  = pLine->GetStationByScInLine(dStaJijum,dDist13,vAng) + pBri->GetLengthJigan(nJijum)/2.0;

	CDPoint xyMark = pLine->GetXyLineDis(dStaMark,dDist13);

	CString	str = m_pStd->GetStringTextDimFck(pBri->GetValueFck(ePartUpperSlab), pBri->GetValueAggregate(ePartUpperSlab), pBri->GetValueSlump(ePartUpperSlab));
	CStringArray strArr;
	strArr.Add("구체 콘크리트");
	strArr.Add(str);

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SYMB);

	CConcSymbol Sym;
	Sym.DrawSymbolConCArrow(&Dom, xyMark, strArr);

	CDPoint vAngLine = pLine->GetAngleAzimuth(dStaMark);
	double dWidth = Dom.GetExtWidth();
	CHgBaseDrawStd::InsertHCadBlock(&Dom, "CGRECON", xyMark-dWidth/2*vAngLine, Dom.GetScaleDim()*2/3);
	//CHgBaseDrawStd::InsertHCadBlock(&Dom, "CGRECON", xyMark+dWidth/2*vAngLine, Dom.GetScaleDim()*2/3);

	Dom.Rotate(xyMark,vAngLine);
	Dom.Rotate(-ConstPi*0.25,xyMark.x,xyMark.y);

	*pDomP << Dom;

	// 접속슬래브
}

// 가각부 철근의 기준 선 그림
void CDrawPlane::DrawMainRebarOrgLineOfExp(CDomyun *pDomP, BOOL bStt, BOOL bLeft)
{
	CRcBridgeRebar *pBri  = m_pStd->m_pRcBridgeRebar;

	CTwinVectorArray tvArrOrg;
	pBri->GetTvMainRebarOrgOfExp(tvArrOrg, bStt, bLeft);

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SYMB, LT_DASHDOT_LINE);
	CHgBaseDrawStd baseDraw;

	baseDraw.DrawTvArray(&Dom, tvArrOrg);

	
	*pDomP << Dom;

}

void CDrawPlane::DrawRebarSupport_Plane_UpperSlabGirderJong(CDomyun *pDomP, BOOL bStt, BOOL bLeft, BOOL bGagak, BOOL bUpper, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	CLineInfoApp *pLineApp	= static_cast<CLineInfoApp*>(pBri->GetLineBase(0));
	CLineInfo *pLine		= pBri->GetLineLeftRight(bLeft);

	CHgBaseDrawStd baseDraw;
	baseDraw.m_pLine	= pLine;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CTwinVectorArray tvArr;
	pBri->GetTvRebarSupport_Plane_UpperSlabGirderJong(tvArr, bStt, bLeft, bGagak, bUpper, TRUE);

	if(tvArr.GetSize() == 0) return;
	
	if(bDimRebar && !bGagak)
	{
		long nL	= bLeft ? pBri->m_nLineLeft : pBri->m_nLineRight;
		long left	= bLeft ? iLEFT : iRIGHT;
		CDPoint vAngStt	= pBri->GetAngleByTarget(BRIDGE_OFF_STT_SLAB, 0);
		CDPoint vAngEnd	= pBri->GetAngleByTarget(BRIDGE_OFF_END_SLAB, 0);
		double dStaStt	= pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, FALSE);
		double dStaEnd	= pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, FALSE);

		CDPoint xyStt	= pBri->GetXySlabEdgeAct(dStaStt, vAngStt, bLeft);
		CDPoint xyEnd	= pBri->GetXySlabEdgeAct(dStaEnd, vAngEnd, bLeft);

		double dStaSttDim	= dStaStt;
		double dStaEndDim	= dStaEnd;
		double dDistSttDim	= pLine->GetDisMatchVerticalByXy(xyStt);
		double dDistEndDim	= pLine->GetDisMatchVerticalByXy(xyEnd);
		
		pLine->GetStationMatchVerticalByXy(xyStt, dStaSttDim);
		pLine->GetStationMatchVerticalByXy(xyEnd, dStaEndDim);
		dStaStt	= dStaSttDim;
		dStaEnd	= dStaEndDim;
	
		dStaSttDim += pBri->m_dDistGirderRebarJong[left][iSTT];
		dStaEndDim -= pBri->m_dDistGirderRebarJong[left][iEND];

		CDimDomyun DomDim(pDomP, pLineApp);
		m_pStd->m_pARoadOptionStd->SetEnvType(&DomDim, HCAD_DIML);
		DomDim.SetDirection(bLeft ? "TOP" : "BOTTOM");
		
		double dDistOffset	= DomDim.GetTextHeight() * 2;
		dDistSttDim -= bLeft ? dDistOffset * 3 : -dDistOffset * 3;;
		dDistEndDim -= bLeft ? dDistOffset * 3 : -dDistOffset * 3;
		DomDim.DimBaseCurvePoint(dStaStt, dDistSttDim);
		DomDim.DimCurveMoveTo(dStaStt, dDistSttDim, nL);
		DomDim.DimCurveLineTo(dStaSttDim-dStaStt, dDistSttDim, nL, bInput ? "L1" : COMMA(pBri->m_dDistGirderRebarJong[left][iSTT]), "", TRUE);

		DomDim.DimBaseCurvePoint(dStaEndDim, dDistEndDim);
		DomDim.DimCurveMoveTo(dStaEndDim, dDistEndDim, nL);
		DomDim.DimCurveLineTo(dStaEnd-dStaEndDim, dDistEndDim, nL, bInput ? "L2" : COMMA(pBri->m_dDistGirderRebarJong[left][iEND]), "", TRUE);

		*pDomP << DomDim;
		

	}

	// 절단원 적용해서 그림 
	long nJ	= bStt ? 0 : pBri->m_nQtyJigan;
	if(!bInput)
		ApplyCuttingArea_Plane(&Dom, tvArr, bLeft, 0, FALSE, TRUE, FALSE, FALSE, FALSE, nJ, TRUE, bGagak);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	baseDraw.DrawTvArray(&Dom, tvArr);
		
	CString sLeft	= bLeft ? "좌측" : "우측";
	CString sStt	= bGagak ? (bStt ? "시점 " : "종점 ") : "";
	CString sGagak	= bGagak ? "가각부 " : "";
	CString sUpper	= bUpper ? "상면" : "하면";
	long upper	= bUpper ? iUPPER : iLOWER;
	
	CString sDescription	= _T("");
	sDescription.Format("%s%s%s 종거더 %s 주철근 %d단", sGagak, sStt, sLeft, sUpper, 1);
	CString sMark	= pBri->GetMarkRebarEtc(0, FALSE, FALSE, FALSE, sDescription, TRUE);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		CGirderSectionRebar *pGirderRB	= pBri->GetGirderRebarJong(bStt ,bLeft, bGagak);
		CString sDia	= m_pStd->m_pARcBridgeDataStd->GetStringDia(pGirderRB->m_dDiaUL[upper][0], pBri->GetValueFy(ePartUpperSlab));

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		CRebar *pRb = pBri->GetRebarEtcByDescription(sDescription, TRUE);
		CRebarPlacing rb;
		CTwinVectorArray tvArrStirrup;
		pBri->GetTvRebarStirrup_Plane_UpperSlabGirderJong(tvArrStirrup, bStt, bLeft, bGagak);
		CTwinVectorArray tvArrCuttingArea;
		pBri->GetTvCuttingArea_Plane_UpperSlabGirder(tvArrCuttingArea, 0, bStt, bLeft, bGagak, TRUE);
		rb.GetTvArrayByCrossTvArrayArea(tvArrStirrup, tvArrCuttingArea, TRUE);

		long nSize	= tvArrStirrup.GetSize();
		if(nSize > 1)
		{
			if(bGagak)
			{
				if(bLeft)
					rb.ReverseRebar(tvArr);
				CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
				mark.SetRebar(tvArr, tvArrStirrup);

				// 설정
				mark.m_sMark					= sMark;
				mark.m_sDia						= sDia;
				
				if(bStt && bLeft)
				{
					mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_END;
					mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_UPPER;
					mark.m_nTypeMarkDir				= TYPE_MARKDIR_LEFTTAN;
					mark.m_dAddHeight				= Dom.Always(MARK_BASE_HEIGHT);
				}
				else if(!bStt && bLeft)
				{
					mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_END;
					mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_UPPER;
					mark.m_nTypeMarkDir				= TYPE_MARKDIR_RIGHTTAN;
					mark.m_dAddHeight				= Dom.Always(MARK_BASE_HEIGHT);
				}
				else if(bStt && !bLeft)
				{
					mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_STT;
					mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_LOWER;
					mark.m_nTypeMarkDir				= TYPE_MARKDIR_LEFTTAN;
					mark.m_dAddHeight				= Dom.Always(MARK_BASE_RADIUS*1.5);
				}
				else if(!bStt && !bLeft)
				{
					mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_STT;
					mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_LOWER;
					mark.m_nTypeMarkDir				= TYPE_MARKDIR_RIGHTTAN;
					mark.m_dAddHeight				= Dom.Always(MARK_BASE_RADIUS*1.5);
				}

				mark.m_dPosScale				= 0.2;

				CDPoint xyMark	= mark.GetXyMark(TRUE);
				CTwinVectorArray tvArrExp;
				pBri->GetTvPlaneOfExp(tvArrExp, bStt, bLeft);
				if(tvArrExp.GetSize() > 0)
				{
					CDPointArray xyArrCount;
					CDPointArray xyArrMatch;
					xyArrCount	= mark.m_xyArrCountMark;
					
					CDPoint xyDirCountMark	= xyArrCount.GetSize() > 1 ? (xyArrCount.GetAt(0) - xyArrCount.GetAt(1)).Unit() : CDPoint(1, 0);
					rb.GetXyMatchTvArrAndLine(tvArrExp, xyMark, xyDirCountMark.Rotate90(), xyArrMatch);
					if(xyArrMatch.GetSize() > 0)
					{
						mark.m_dAddHeight	+= ~(xyArrMatch.GetAt(0) - xyMark);
					}
				}
				mark.m_dCtc = pGirderRB->m_dC[upper][0];
				mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;

				mark.MarkRebar(FALSE);	
			}
			else
			{
				// 마킹할 스테이션 찾음.
				double dSta1(0), dSta2(0);
				double dStaMark(0);
				CTwinVector tv1	= tvArrStirrup.GetAt(nSize/9),
							tv2	= tvArrStirrup.GetAt(nSize/9+1);
				
				pLine->GetStationMatchVerticalByXy(tv1.m_v1, dSta1);
				pLine->GetStationMatchVerticalByXy(tv2.m_v1, dSta2);
				dStaMark	= (dSta1 + dSta2)/2;

				
				// 마킹함.
				CConcSymbol sym(m_pStd->m_pARoadOptionStd);
				CTwinVector tv;
				CDPoint xy(0, 0);
				nSize	= tvArr.GetSize();
				CDPoint xyFir(0, 0), xyLast(0, 0);
				long i = 0; for(i = 0; i < nSize; i++)
				{
					tv		= tvArr.GetAt(i);
					double dDist	= pLine->GetDisMatchVerticalByXy(tv.m_v1);

					xy	= pLine->GetXyLineDis(dStaMark, dDist);
					if(i == 0)
						xyLast	= xy;
					else if(i == nSize-1)
						xyFir	= xy;

					sym.RebarCountMarkOne(&Dom, xy);
				}

				if(nSize > 1)
					Dom.LineTo(xyFir, xyLast);
				*pDomP << Dom;

				// 마킹
				CDPoint xyMark	= xyLast;
				
				double dAddHeight	= pGirderRB->m_dDist;
				if(bLeft)
				{
					dAddHeight += Dom.Always(MARK_BASE_HEIGHT);
					// sym.RebarMarkLineUpper(&Dom, xyMark, sMark, sDia, TRUE, dAddHeight, FALSE);
				}
				else
				{
					dAddHeight += Dom.Always(MARK_BASE_HEIGHT+MARK_BASE_RADIUS*1.5);
					// sym.RebarMarkLineLower(&Dom, xyMark, sMark, sDia, TRUE, dAddHeight, FALSE);
				}

				SRebarMarkInputData mark_input_data;
				SRebarMarkviewData mark_view_data;
				mark_input_data.mark = sMark;
				mark_input_data.dia = sDia;
				mark_input_data.position = xyMark;
				mark_input_data.ctc = pGirderRB->m_dC[upper][0];
				mark_input_data.type_rebar = pRb? pRb->m_nTypeRebar : -1;
				mark_input_data.left_dir = TRUE;
				mark_input_data.height = dAddHeight;

				mark_view_data.count_mark = FALSE;
				mark_view_data.line_rebar = TRUE;
				mark_view_data.mark_pos = bLeft? eMarkUpper : eMarkLower;

				sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

				double dRot	= tv1.GetXyDir(FALSE).GetAngleDegree() - 270;
				Dom.Rotate(xyMark, ToDPointFrDegree(dRot));
			}
		}
	}

	*pDomP << Dom;
}

void CDrawPlane::DrawRebarStirrup_Plane_UpperSlabGirderJong(CDomyun *pDomP, BOOL bStt, BOOL bLeft, BOOL bGagak, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	CLineInfo *pLine		= pBri->GetLineLeftRight(bLeft);

	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CTwinVectorArray tvArr;
	pBri->GetTvRebarStirrup_Plane_UpperSlabGirderJong(tvArr, bStt, bLeft, bGagak);

	if(tvArr.GetSize() == 0) return;
	
	if(bDimRebar)
	{
		

	}

	// 절단원 적용해서 그림 
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	// 절단원 적용해서 그림 
	long nJ	= bStt ? 0 : pBri->m_nQtyJigan;
	if(!bInput)
		ApplyCuttingArea_Plane(&Dom, tvArr, bLeft, 0, FALSE, TRUE, FALSE, FALSE, FALSE, nJ, TRUE, bGagak);

	baseDraw.DrawTvArray(&Dom, tvArr);
		
	CString sLeft	= bLeft ? "좌측" : "우측";
	CString sStt	= bGagak ? (bStt ? "시점 " : "종점 ") : "";
	CString sGagak	= bGagak ? "가각부 " : "";
	
	CString sDescription	= _T("");
	sDescription.Format("%s%s%s 종거더 수직전단철근", sGagak, sStt, sLeft);
	CString sMark	= pBri->GetMarkRebarEtc(0, FALSE, FALSE, FALSE, sDescription, TRUE);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		CGirderSectionRebar *pGirderRB	= pBri->GetGirderRebarJong(bStt, bLeft, bGagak);
		if(!pGirderRB) return;
		CString sDia	= m_pStd->m_pARcBridgeDataStd->GetStringDia(pGirderRB->m_dDiaStirrup, pBri->GetValueFy(ePartUpperSlab, TRUE));

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		CConcSymbol sym(m_pStd->m_pARoadOptionStd);

		CRebar *pRb = pBri->GetRebarEtcByDescription(sDescription, TRUE);
		if(bGagak)
		{
			CTwinVectorArray tvArrSup;
			pBri->GetTvRebarSupport_Plane_UpperSlabGirderJong(tvArrSup, bStt, bLeft, bGagak, TRUE);
			if(!bInput)
				ApplyCuttingArea_Plane(&Dom, tvArr, bLeft, 0, FALSE, TRUE, FALSE, FALSE, FALSE, nJ, TRUE, bGagak);

			CRebarPlacing rb;
			rb.ReverseRebar(tvArrSup);
			if(!bStt)
				rb.ReverseRebar(tvArr);

			CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
			mark.SetRebar(tvArr, tvArrSup);

			// 설정
			mark.m_sMark					= sMark;
			mark.m_sDia						= sDia;
			mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_POS;
			mark.m_nTypeMarkPointUpAndDown	= bLeft ? TYPE_MARKPOINT_UPPER : TYPE_MARKPOINT_LOWER;
			mark.m_nTypeMarkDir				= TYPE_MARKDIR_RIGHTTAN;
			mark.m_dPosScale				= 0.5;
			CDPoint xyMark	= mark.GetXyMark(TRUE);
			CTwinVectorArray tvArrExp;
			pBri->GetTvPlaneOfExp(tvArrExp, bStt, bLeft);
			if(tvArrExp.GetSize() > 0)
			{
				CDPointArray xyArrCount;
				CDPointArray xyArrMatch;
				
				xyArrCount	= mark.m_xyArrCountMark;
				CDPoint xyDirCountMark	= xyArrCount.GetSize() > 1 ? (xyArrCount.GetAt(0) - xyArrCount.GetAt(1)).Unit() : CDPoint(1, 0);
				rb.GetXyMatchTvArrAndLine(tvArrExp, xyMark, xyDirCountMark.Rotate90(), xyArrMatch);
				if(xyArrMatch.GetSize() > 0)
				{
					mark.m_dAddHeight	= ~(xyArrMatch.GetAt(0) - xyMark) + Dom.Always(MARK_BASE_HEIGHT);
					if(!bLeft)
						mark.m_dAddHeight	+= Dom.Always(MARK_BASE_RADIUS*1.5);
				}
			}
			mark.m_dCtc = pGirderRB->m_dCTC;
			mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;
			
			mark.MarkRebar(FALSE);	
		}
		else
		{
			double dDistMark	= pGirderRB->m_dC1[iUPPER][0] + pGirderRB->m_nB[iUPPER][0]/2*pGirderRB->m_dC[iUPPER][0] + pGirderRB->m_dC2[iUPPER][0]/2 + REBAR_RADIUS*2;
			double dDistMarkOld	= dDistMark;
			CDPointArray xyArrMark;
			CDPoint xy(0, 0), xyFir(0, 0), xyLast(0, 0);
			CTwinVector tv;
			long nSize	= tvArr.GetSize();
			if(nSize > 0)
			{
				tv	= tvArr.GetAt(nSize/2);
				dDistMark	= pLine->GetDisMatchVerticalByXy(tv.m_v1) + dDistMark;
			}

			double dSta	= 0;
			double dStaMark	= 0;
			long i = 0; for(i = 0; i < nSize; i++)
			{
				tv	= tvArr.GetAt(i);
				pLine->GetStationMatchVerticalByXy(tv.m_v1, dSta);
				xy	= pLine->GetXyLineDis(dSta, dDistMark);
				
				sym.RebarCountMarkOne(&Dom, xy);
				if(i == 0) xyFir = xy;
				else if(i == nSize-1) xyLast = xy;
			}

			if(nSize > 1)
			{
				double dSta1(0), dSta2(0);
				tv	= tvArr.GetAt(nSize/9*7);
				pLine->GetStationMatchVerticalByXy(tv.m_v1, dSta1);
				tv	= tvArr.GetAt(nSize/9*7+1);
				pLine->GetStationMatchVerticalByXy(tv.m_v1, dSta2);
				dStaMark	= (dSta1+dSta2)/2;
			}

			
			tv.m_v1	= xyFir;
			tv.m_v2	= xyLast;
			tv.m_v1.z	= 4;
			baseDraw.m_pLine	= pLine;
			baseDraw.DrawTv(&Dom, tv);
			*pDomP << Dom;

			// 마킹
			CDPoint xyMark	= pLine->GetXyLineDis(dStaMark, dDistMark);
			
			double dAddHeight	= pGirderRB->m_dDist + dDistMarkOld;
			if(bLeft)
			{
				dAddHeight += Dom.Always(MARK_BASE_HEIGHT);
				// sym.RebarMarkLineUpper(&Dom, xyMark, sMark, sDia, FALSE, dAddHeight);
			}
			else
			{
				dAddHeight += Dom.Always(MARK_BASE_HEIGHT+MARK_BASE_RADIUS*1.5);
				// sym.RebarMarkLineLower(&Dom, xyMark, sMark, sDia, FALSE, dAddHeight);
			}

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = sMark;
			mark_input_data.dia = sDia;
			mark_input_data.position = xyMark;
			mark_input_data.ctc = pGirderRB->m_dCTC;
			mark_input_data.type_rebar = pRb? pRb->m_nTypeRebar : -1;		// 18을 고정으로 써도 된다.
			mark_input_data.left_dir = FALSE;
			mark_input_data.height = dAddHeight;

			mark_view_data.count_mark = FALSE;
			mark_view_data.line_rebar = TRUE;
			mark_view_data.mark_pos = bLeft? eMarkUpper : eMarkLower;

			sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

			CDPoint xyMarkTmp	= pLine->GetXyLineDis(dStaMark, bLeft ? dDistMark + 100 : dDistMark - 100);
			CDPoint xyDir	= (xyMarkTmp - xyMark).Unit();
			double dRot	= xyDir.GetAngleDegree() + 90;
			if(!bLeft)
				dRot += 180;
			Dom.Rotate(xyMark, ToDPointFrDegree(dRot));
		}
	}

	*pDomP << Dom;
}
// 둔각부 보강용 평면도
void CDrawPlane::DrawPlaneGenObtuse(CDomyun *pDomP, BOOL bStt, BOOL bLeft, BOOL bUpper, BOOL bLowerSlab)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	long stt	= bStt ? iSTT : iEND;
	long left	= bLeft ? iLEFT : iRIGHT;
	long upper	= bUpper ? iUPPER : iLOWER;
	BOOL bBoxType = pBri->IsBoxType();

	if(bLowerSlab && !bBoxType) return;
	if(bLowerSlab && !pBri->m_bIsObtuseAngle_LowerSlab[stt][left]) return;
	else if(!pBri->m_bIsObtuseAngle_UpperSlab[stt][left]) return;

	CRebarPlacing rb;
	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	CTwinVectorArray tvArrLeftRight, tvArrSttEnd;
	pBri->GetTvArrPlane_UpperSlab_Obtuse(tvArrLeftRight, tvArrSttEnd, bStt, bLeft, -1,bLowerSlab);

	double dStaMid	= pBri->GetStationBridgeMid();
	
	// 선형방향 그릴 영역
	double dLen		= bLowerSlab? pBri->m_dLenObtuseAngle_LowerSlab[stt][left][upper] : pBri->m_dLenObtuseAngle_UpperSlab[stt][left][upper];
	double dCover	= bLowerSlab? pBri->m_dCoverObtuseAngle_LowerSlab_Plane[stt][left][0] : pBri->m_dCoverObtuseAngle_UpperSlab_Plane[stt][left][0];

	CPlaceRebarByBlock *pPlace	= bLowerSlab? &pBri->m_placeByBlock_ObtuseAngle_Main_LowerSlab[stt][left][upper] : &pBri->m_placeByBlock_ObtuseAngle_Main_UpperSlab[stt][left][upper];
	double dLenRound	= bUpper ? pBri->GetLenRebarAtEdge(pPlace->m_dDia, FALSE, 0, !bLowerSlab) : 0;
	if(bStt)
		dStaMid	= pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, FALSE) + dLen + dCover * 2 + dLenRound;
	else
		dStaMid	= pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, FALSE) - dLen - dCover * 2 - dLenRound;

	long nJ			= pBri->GetJijumNumberByStation(dStaMid);
	CDPoint vAng	= pBri->GetAngleJijum(nJ);
	CDPoint xyMid	= pBri->GetXySlabEdgeAct(dStaMid, vAng, bLeft, FALSE);
	CDPoint xyCen	= (xyMid + tvArrSttEnd.GetAt(0).m_v1)/2;
	double dR	= ~(xyMid-xyCen);

	rb.GetTvArrayByCircleArea(tvArrLeftRight, xyCen, dR, TRUE);
	/*
	CTwinVectorArray tvArrMid;
	CTwinVector tvMid;
	tvMid.m_v1	= pBri->GetXySlabEdgeAct(dStaMid, vAng, TRUE);
	tvMid.m_v2	= pBri->GetXySlabEdgeAct(dStaMid, vAng, FALSE);
	if(!bStt)
		GetSwap(tvMid.m_v1, tvMid.m_v2);
	tvArrMid.Add(tvMid);
	rb.TrimAndStretchByTvAndLength(tvArrLeftRight, tvArrMid, 0);
	*/

	// 횡방향 그릴 영역
	pPlace	= bLowerSlab? &pBri->m_placeByBlock_ObtuseAngle_Main_LowerSlab[stt][left][upper] : &pBri->m_placeByBlock_ObtuseAngle_Main_UpperSlab[stt][left][upper];
	dLen	= 0;
	pPlace->FitCount();
	long i = 0; for(i = 0; i < pPlace->m_nCountBlock; i++)
	{
		dLen	+= pPlace->m_xyArr.GetAt(i).x * pPlace->m_xyArr.GetAt(i).y;
	}

	dCover	= pBri->m_dCoverObtuseAngle_UpperSlab_Plane[stt][left][1];
	rb.StretchTvArr(tvArrSttEnd, 0, -(tvArrSttEnd.GetLength() - (dLen + dCover*2 + Dom.Always(10))));
	
	// 구조물
	baseDraw.DrawTvArray(&Dom, tvArrSttEnd);
	baseDraw.DrawTvArray(&Dom, tvArrLeftRight);

	// 절단표시
	if(tvArrLeftRight.GetSize() > 0 && tvArrSttEnd.GetSize() > 0)
	{
		double dExt	= 500;
		CTwinVector tvCutSttEnd;
		CTwinVector tvFirst	= tvArrLeftRight.GetAt(0);
		CTwinVector tvLast	= tvArrLeftRight.GetAt(tvArrLeftRight.GetSize()-1);
		CDPoint xyOrg	= tvArrSttEnd.GetAt(0).m_v1;
		CDPoint xyFirst	= ~(CDPoint(tvFirst.m_v1) - xyOrg) < ~(CDPoint(tvFirst.m_v2) - xyOrg) ? tvFirst.m_v1 : tvFirst.m_v2;
		CDPoint xyLast	= ~(CDPoint(tvLast.m_v1) - xyOrg) < ~(CDPoint(tvLast.m_v2) - xyOrg) ? tvFirst.m_v2 : tvFirst.m_v1;

		CDPoint xyMove	= xyLast - xyFirst;
		tvCutSttEnd.m_v1	= tvArrSttEnd.GetAt(0).m_v1;
		tvCutSttEnd.m_v2	= tvArrSttEnd.GetAt(tvArrSttEnd.GetSize()-1).m_v2;
		tvCutSttEnd.m_v1	+= tvCutSttEnd.GetXyDir(TRUE) * dExt;
		tvCutSttEnd.m_v2	+= tvCutSttEnd.GetXyDir() * dExt;
		tvCutSttEnd.m_v1	+= xyMove;
		tvCutSttEnd.m_v2	+= xyMove;


		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CUTL);
		Dom.CutLightningAnyDir(tvCutSttEnd.m_v1, tvCutSttEnd.m_v2, 500);


		CTwinVector tvCutLeftRight;
		xyMove	= tvArrSttEnd.GetAt(tvArrSttEnd.GetSize()-1).m_v2 - tvArrSttEnd.GetAt(0).m_v1;
		tvCutLeftRight.m_v1	= tvArrLeftRight.GetAt(0).m_v1;
		tvCutLeftRight.m_v2	= tvArrLeftRight.GetAt(tvArrLeftRight.GetSize()-1).m_v2;
		tvCutLeftRight.m_v1	+= tvCutLeftRight.GetXyDir(TRUE) * dExt;
		tvCutLeftRight.m_v2	+= tvCutLeftRight.GetXyDir() * dExt;
		tvCutLeftRight.m_v1	+= xyMove;
		tvCutLeftRight.m_v2	+= xyMove;


		Dom.CutLightningAnyDir(tvCutLeftRight.m_v1, tvCutLeftRight.m_v2, 500);
	}
	*pDomP << Dom;
}

// 둔각부 보강
void CDrawPlane::DrawRebarPlane_Slab_Obtuse(CDomyun *pDomP, BOOL bUpperSlab, BOOL bStt, BOOL bLeft, BOOL bUpper, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	long stt			= bStt ? iSTT : iEND;
	long left			= bLeft ? iLEFT : iRIGHT;
	long upper		= bUpper ? iUPPER : iLOWER;
	BOOL bBoxType = pBri->IsBoxType();

	if(bBoxType)
	{
		if(!bUpperSlab && !pBri->m_bIsObtuseAngle_LowerSlab[stt][left]) 
			return;	
	}
	else if(!pBri->m_bIsObtuseAngle_UpperSlab[stt][left]) 
		return;

	CLineInfo *pLine	= pBri->GetLineBase();
	CRebarPlacing rb;
	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);


	CTwinVectorArrayArray pTvArrMainRebar;
	CTwinVectorArray tvArrSupportRebar;
	pBri->GetTvRebarMain_Plane_Slab_Obtuse(&pTvArrMainRebar, tvArrSupportRebar, bStt, bLeft, bUpper, !bUpperSlab);
	if(pTvArrMainRebar.GetSize() == 0 || tvArrSupportRebar.GetSize() == 0) return;
	
	baseDraw.DrawTvArray(&Dom, &pTvArrMainRebar);
	baseDraw.DrawTvArray(&Dom, tvArrSupportRebar);

	if(bDimRebar)
	{
		*pDomP << Dom;

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		CTwinVectorArray tvArrLineDir, tvArrCrossDir;
		pBri->GetTvArrPlane_UpperSlab_Obtuse(tvArrLineDir, tvArrCrossDir, bStt, bLeft, -1, !bUpperSlab);

		// 횡방향
		CDoubleArray dArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		CString sDir	= bStt ? _T("LEFT") : _T("RIGHT");
		CDPoint xyStt	= tvArrCrossDir.GetAt(0).m_v1;
		long nDimDan	= 1;
		CString str		= _T("");

		CPlaceRebarByBlock *pPlace	= bUpperSlab? &pBri->m_placeByBlock_ObtuseAngle_Main_UpperSlab[stt][left][upper] : &pBri->m_placeByBlock_ObtuseAngle_Main_LowerSlab[stt][left][upper];
		if(pPlace == NULL)	return;

		double dVal = bUpperSlab? pBri->m_dCoverObtuseAngle_UpperSlab_Plane[stt][left][1] : pBri->m_dCoverObtuseAngle_LowerSlab_Plane[stt][left][1];
		dArrDim.Add(dVal);
		sArrText.Add(bInput ? "D4" : COMMA(dVal));
		sArrUnderText.Add(COMMA(dVal));

		CDPoint xy	= CDPoint(0, 0);
		if(pPlace->m_xyArr.GetSize() > 0)
		{
			xy	= pPlace->m_xyArr.GetAt(0);
			if(xy.x * xy.y > 0) 
			{
				if(bInput)
					str	= "F1";
				else
					str	= COMMA(xy.x*xy.y);//.Format("%.0f@%s=%s", xy.x, COMMA(xy.y), COMMA(xy.x*xy.y));

				dArrDim.Add(xy.x * xy.y);
				sArrText.Add(str);
				sArrUnderText.Add(COMMA(xy.x * xy.y));
			}
		}

		if(pPlace->m_xyArr.GetSize() > 1)
		{
			xy	= pPlace->m_xyArr.GetAt(1);
			if(xy.x * xy.y > 0)
			{
				if(bInput)
					str.Format("E@F=%s", COMMA(xy.x*xy.y));
				else
					str.Format("%.0f@%s=%s", xy.x, COMMA(xy.y), COMMA(xy.x*xy.y));

				dArrDim.Add(xy.x * xy.y);
				sArrText.Add(str);
				sArrUnderText.Add(COMMA(xy.x * xy.y));
			}
		}

		if(pPlace->m_xyArr.GetSize() > 2)
		{
			xy	= pPlace->m_xyArr.GetAt(2);
			if(xy.x * xy.y > 0) 
			{
				if(bInput)
					str	= "F2";
				else
					str	= COMMA(xy.x*xy.y);//.Format("%.0f@%s=%s", xy.x, COMMA(xy.y), COMMA(xy.x*xy.y));

				dArrDim.Add(xy.x * xy.y);
				sArrText.Add(str);
				sArrUnderText.Add(COMMA(xy.x * xy.y));
			}
		}

		if(bLeft)
		{
			CTwinVectorArray tvArrLineDirRebar;
			tvArrLineDirRebar	= *pTvArrMainRebar.GetAt(pTvArrMainRebar.GetSize()-1);

			xyStt	= tvArrLineDirRebar.GetAt(0).m_v1;

			CDoubleArray dArrDimTmp;
			CStringArray sArrTextTmp;
			CStringArray sArrUnderTextTmp;

			long i = 0; for(i = dArrDim.GetSize()-1; i >= 0; i--)
			{
				dArrDimTmp.Add(dArrDim.GetAt(i));
				sArrTextTmp.Add(sArrText.GetAt(i));
				sArrUnderTextTmp.Add(sArrUnderText.GetAt(i));
			}

			dArrDim.RemoveAll();
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();

			for(i = 0; i < dArrDimTmp.GetSize(); i++)
			{
				dArrDim.Add(dArrDimTmp.GetAt(i));
				sArrText.Add(sArrTextTmp.GetAt(i));
				sArrUnderText.Add(sArrUnderTextTmp.GetAt(i));
			}
		}
		else
		{
			xyStt	= tvArrCrossDir.GetAt(0).m_v1;
		}

		if(!bInput)
		{
			//sArrText.RemoveAll();
			//long i = 0; for(i = 0; i < sArrUnderText.GetSize(); i++)
			//	sArrText.Add(sArrUnderText.GetAt(i));
			sArrUnderText.RemoveAll();
		}


		double dAng	= pBri->GetAngleJijum(bStt ? 0 : pBri->m_nQtyJigan).GetAngleDegree();
		Dom.SetDimObq(bStt ? dAng - 90 : 90 - dAng);
		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, FALSE, FALSE);

		CTwinVector tvSttEnd	= tvArrSupportRebar.GetAt(0);
		if(bLeft)
			GetSwap(tvSttEnd.m_v1, tvSttEnd.m_v2);

		CDPoint vAngRot	= ToDPointFrDegree(tvSttEnd.GetXyDir().GetAngleDegree() - 90);
		Dom.Rotate(xyStt, vAngRot);
		*pDomP << Dom;



		CLineInfoApp *pLineApp	= static_cast<CLineInfoApp*>(pBri->GetLineBase());
		long nL					= pBri->m_nLineBase;


		CDimDomyun DimDom(pDomP, pLineApp);
		m_pStd->m_pARoadOptionStd->SetEnvType(&DimDom, HCAD_DIML);
		sDir	= bLeft ? _T("TOP") : _T("BOTTOM");
		DimDom.SetDirection(sDir);

		dArrDim.RemoveAll();
		sArrText.RemoveAll();
		sArrUnderText.RemoveAll();

		pPlace	= bUpperSlab? &pBri->m_placeByBlock_ObtuseAngle_Support_UpperSlab[stt][left][upper] : &pBri->m_placeByBlock_ObtuseAngle_Support_LowerSlab[stt][left][upper];
		if(pPlace == NULL) return;

		double dVal2 = bUpperSlab? pBri->m_dCoverObtuseAngle_UpperSlab_Plane[stt][left][0] : pBri->m_dCoverObtuseAngle_LowerSlab_Plane[stt][left][0];
		dArrDim.Add(dVal2);
		sArrText.Add(bInput ? "D3" : COMMA(dVal2));
		sArrUnderText.Add(COMMA(dVal2));

		if(pPlace->m_xyArr.GetSize() > 0)
		{
			xy	= pPlace->m_xyArr.GetAt(0);
			if(xy.x * xy.y > 0) 
			{
				if(bInput)
					str	= "C1";
				else
					str	= COMMA(xy.x*xy.y);//.Format("%.0f@%s=%s", xy.x, COMMA(xy.y), COMMA(xy.x*xy.y));

				dArrDim.Add(xy.x * xy.y);
				sArrText.Add(str);
				sArrUnderText.Add(COMMA(xy.x * xy.y));
			}
		}


		if(pPlace->m_xyArr.GetSize() > 1)
		{
			xy	= pPlace->m_xyArr.GetAt(1);
			if(xy.x * xy.y > 0)
			{
				if(bInput)
					str.Format("B@C=%s", COMMA(xy.x*xy.y));
				else
					str.Format("%.0f@%s=%s", xy.x, COMMA(xy.y), COMMA(xy.x*xy.y));

				dArrDim.Add(xy.x * xy.y);
				sArrText.Add(str);
				sArrUnderText.Add(COMMA(xy.x * xy.y));
			}
		}

		if(pPlace->m_xyArr.GetSize() > 2)
		{
			xy	= pPlace->m_xyArr.GetAt(2);
			if(xy.x * xy.y > 0) 
			{
				if(bInput)
					str	= "C2";
				else
					str	= COMMA(xy.x*xy.y);//.Format("%.0f@%s=%s", xy.x, COMMA(xy.y), COMMA(xy.x*xy.y));

				dArrDim.Add(xy.x * xy.y);
				sArrText.Add(str);
				sArrUnderText.Add(COMMA(xy.x * xy.y));
			}
		}


		if(!bStt)
		{
			xyStt	= tvArrSupportRebar.GetAt(tvArrSupportRebar.GetSize()-1).m_v1;

			CDoubleArray dArrDimTmp;
			CStringArray sArrTextTmp;
			CStringArray sArrUnderTextTmp;

			long i = 0; for(i = dArrDim.GetSize()-1; i >= 0; i--)
			{
				dArrDimTmp.Add(dArrDim.GetAt(i));
				sArrTextTmp.Add(sArrText.GetAt(i));
				sArrUnderTextTmp.Add(sArrUnderText.GetAt(i));
			}

			dArrDim.RemoveAll();
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();

			for(i = 0; i < dArrDimTmp.GetSize(); i++)
			{
				dArrDim.Add(dArrDimTmp.GetAt(i));
				sArrText.Add(sArrTextTmp.GetAt(i));
				sArrUnderText.Add(sArrUnderTextTmp.GetAt(i));
			}
		}
		else
		{
			xyStt	= tvArrCrossDir.GetAt(0).m_v1;//tvArrLineDir.GetAt(0).m_v1;
		}

		if(!bInput)
		{
			//sArrText.RemoveAll();
			//long i = 0; for(i = 0; i < sArrUnderText.GetSize(); i++)
			//	sArrText.Add(sArrUnderText.GetAt(i));
			sArrUnderText.RemoveAll();
		}

		double dSta		= 0;
		double dDist	= 0;
		pLine->GetStationMatchVerticalByXy(xyStt, dSta);
		dDist	= pLine->GetDisMatchVerticalByXy(xyStt);
		
		if(bLeft)
		{
			dDist -= Dom.Always(Dom.GetDimLevelDis() * nDimDan);
			dDist -= Dom.Always((Dom.GetDimExe() + Dom.GetDimExo()));
		}
		else
		{
			dDist += Dom.Always(Dom.GetDimLevelDis() * nDimDan);
			dDist += Dom.Always((Dom.GetDimExe() + Dom.GetDimExo()));
		}
		
		DimDom.DimBaseCurvePoint(dSta, 0, nDimDan);
		
		long i = 0; for(i = 0; i < dArrDim.GetSize(); i++)
		{
			BOOL bExtendMove	= FALSE;
			BOOL bExtendLine	= FALSE;
			CString sText	= sArrText.GetAt(i);
			CString sUnderText	= sArrUnderText.GetSize() > i ? sArrUnderText.GetAt(i) : "";
			if(dArrDim.GetAt(i) <= Dom.GetTextWidth(sArrText.GetAt(i)))
			{
				if(i == 0)
					bExtendMove	= TRUE;
				else if(i == dArrDim.GetSize()-1)
					bExtendLine	= TRUE;
			}

			bExtendMove	= FALSE;
			bExtendLine	= FALSE;

			if(bExtendMove)
			{
				DimDom.DimCurveMoveTo(dSta, 0, nL, TRUE, sText, sUnderText, 0, TRUE, bExtendMove);
				DimDom.DimCurveLineTo(dArrDim.GetAt(i), 0, nL, "", "");
			}
			else
			{
				DimDom.DimCurveMoveTo(dSta, 0, nL);
				DimDom.DimCurveLineTo(dArrDim.GetAt(i), 0, nL, sText, sUnderText, FALSE, bExtendLine);
			}
			dSta += dArrDim.GetAt(i);
		}

		pLine->GetStationMatchVerticalByXy(xyStt, dSta);
		CDPoint xyMove	= pLine->GetXyLineDis(dSta, dDist) - pLine->GetXyLineDis(dSta, 0);
		DimDom.Move(xyMove);
		*pDomP << DimDom;
	}

	if(bMarkRebar && tvArrSupportRebar.GetSize() > 1 && pTvArrMainRebar.GetSize() > 1)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		CMarkDomyun mark;
		mark.SetDomyun(&Dom, m_pStd->m_pARoadOptionStd);

		long stt	= bStt ? iSTT : iEND;
		long left	= bLeft ? iLEFT : iRIGHT;

		CRebarInfoCycle *pRC	= pBri->GetOwnerRebarInfoCycleObtuseAngle(bStt, bLeft, bUpper, !bUpperSlab);
		if(pRC == NULL) return;

		CRebar *pRb = pBri->GetRebarMainByDescription(pRC->m_sDescription);
		if(pRb == NULL) return;

		mark.SetRebar(&pTvArrMainRebar, tvArrSupportRebar);
		mark.m_sMark	= pRC->m_szMark;
		mark.m_sDia		= m_pStd->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pBri->GetValueFy(bUpperSlab? ePartUpperSlab : ePartLowerSlab));
		mark.m_dCtc		= bUpperSlab? pBri->m_placeByBlock_ObtuseAngle_Main_UpperSlab[stt][left][upper].GetCTC() : pBri->m_placeByBlock_ObtuseAngle_Main_LowerSlab[stt][left][upper].GetCTC();
		mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;
		mark.MarkRebar();

		CMarkDomyun mark1(&Dom, m_pStd->m_pARoadOptionStd);
		BOOL bSttOwner	= bStt;
		BOOL bLeftOwner	= bLeft;
		pBri->GetOwnerRebarSupportObtuseAngle(bStt, bLeft, bSttOwner, bLeftOwner);

		CString sStt	= bSttOwner ? "시점" : "종점";
		CString sLeft	= bLeftOwner ? "좌측" : "우측";
		CString sUpper	= bUpper ? "상면" : "하면";
		CString sDecription	= _T("");
// 		CString sMark		= _T("");
		CString sUL = _T("");
		if(pBri->IsBoxType())
		{
			sUL.Format(bUpperSlab? _T("상부슬래브") : _T("하부슬래브"));
			sDecription.Format("%s 둔각부(%s%s) 배력철근(%s)", sUL, sStt, sLeft, sUpper);
		}
		else
		{
			sDecription.Format("둔각부(%s%s) 배력철근(%s)", sStt, sLeft, sUpper);
		}
		

		CString sMark	=	bInput ? (bUpper ? "SDU" : "SDL") : pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDecription);

		CPlaceRebarByBlock *pPlace	= bUpperSlab? &pBri->m_placeByBlock_ObtuseAngle_Support_UpperSlab[stt][left][upper] : &pBri->m_placeByBlock_ObtuseAngle_Support_LowerSlab[stt][left][upper];
		if(pPlace == NULL) return;

		pRb = pBri->GetRebarSupportByDescription(sDecription);
		mark1.SetRebar(tvArrSupportRebar, &pTvArrMainRebar);
		mark1.m_sMark		= sMark;
		mark1.m_sDia		= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(bUpperSlab? ePartUpperSlab : ePartLowerSlab));
		mark1.m_dCtc		= pPlace->GetCTC();
		mark1.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;
		mark1.MarkRebar();
	}


	*pDomP << Dom;


	AhTPADelete(&pTvArrMainRebar, (CTwinVectorArray*)0);
}

// 평면도 위치도
void CDrawPlane::DrawPlanePosSign(CDomyun *pDomP, BOOL bStt, BOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	

	CWingWall *pWing	= pBri->GetWingWall(bStt, bLeft);
	CDomyun Dom(pDomP);

	double dScale = 250.0;
	double dRatio = 50.0/dScale;
	Dom.SetScaleDim(dScale);
	pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 2);

	// 교량 평면
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DrawPlaneCenterLineEachSlab(&Dom);
	DrawPlaneSlabOnlyOutLine(&Dom);
	
	// 벽체 및 기둥 평면
	int nCountInWall = pBri->GetCountInWall();
	int i=0; for(i=0; i<nCountInWall; i++)
	{
		CWallApp *pWall = pBri->GetInWall(i);
		if(pWall)
		{
			m_pStd->m_pDrawWall->DrawPlan(&Dom, pWall, FALSE);
			if(pWall->m_bTopIsColumn)
				m_pStd->m_pDrawColumn->DrawPlan(&Dom, &pWall->m_Column);
		}
	}

	// 날개벽 평면
	pBri->GetTvWingWallPlane(TRUE);
	m_pStd->m_pDrawWingWall->DrawGenPlane(&Dom, TRUE);
	pBri->GetTvWingWallPlane(FALSE);
	m_pStd->m_pDrawWingWall->DrawGenPlane(&Dom, FALSE);

	CDPoint xyC = pBri->GetXyCenterWingWallPlane(bStt, bLeft);
	double dSpare1 = 1000.0;
	double dSpare2 = 2000.0;
	double dRadius = pWing->m_bExist ? (pWing->m_dL1+pWing->m_dL2+dSpare2)/2.0 : dSpare2;
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.Circle(xyC, dRadius);

	// 교량 진행 방향으로 회전
	m_pStd->RotateDomByBridgeSttEndXy(&Dom);

	Dom.SetCalcExtRect();
	CDRect rcLoc = Dom.GetExtRect();

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SMALL_TITLE);
	Dom.SetTextAlignHorz(TA_CENTER);
	Dom.SetTextHeight(7.5);
	Dom.TextOut(rcLoc.CenterPoint().x, rcLoc.bottom+dSpare1/dRatio, _T("위 치 도"));

	/*
	if(bStt)
		Dom.Move(-rcLoc.left, -rcLoc.top);
	else
		Dom.Move(-(rcLoc.left+rcLoc.Width()), -rcLoc.top);
	Dom.RedrawByScale(dRatio);

	if(bStt)
		Dom.Move(rcFront.right+dSpare1, rcFront.top+dSpare2);
	else
		Dom.Move(rcFront.left-dSpare1, rcFront.top+dSpare2);
		*/
	Dom.RedrawByScale(dRatio);
	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, 2);

	*pDomP << Dom;
}

void CDrawPlane::DrawRebarMain_Plane_ArchRib(CDomyun *pDomP, long nCycle, BOOL bUpper, long nDan, long nDrawLeft, BOOL bDimRebar, BOOL bMarkRebar)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	CLineInfo *pLine	= pBri->GetLineBase();

	long upper	= bUpper ? iUPPER : iLOWER;
	CHgBaseDrawStd baseDraw;

	CDomyun Dom(pDomP);
	CTwinVectorArray tvArrSup;
	pBri->GetTvRebarSupport_Plane_ArchRib(tvArrSup, bUpper, 0, 0);

	long nSize	= pBri->m_pArrRCMainRib[nCycle][upper][nDan].GetSize();
	long i = 0; for(i = 0; i < nSize; i++)
	{
		CTwinVectorArray tvArr, tvArrJoint;
		pBri->GetTvRebarMain_Plane_ArchRib(tvArr, tvArrJoint, nCycle, bUpper, nDan, i, nDrawLeft);
		if(tvArr.GetSize() == 0) continue;
		
		if(bDimRebar && ((bUpper && i == 0) || (!bUpper && i == nSize-1)) && nCycle == 0)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
			long nCountDanMainRebar	= pBri->GetDanMainRebarRib(1, bUpper);
			CDoubleArray dArrDist;
			CDoubleArray dArrDim;
			CStringArray sArrText;
			CStringArray sArrUnderText;
			long nDimDan	= nCountDanMainRebar - nDan - 1;
			CString sDir	= bUpper ? _T("LEFT") : ("RIGHT");
			CDPoint xyStt	= CDPoint(0, 0);

			dArrDim.Add(pBri->GetCoverSideArchRib(0));
			if(nDan > 0)
				dArrDim.Add(pBri->GetCoverSideArchRib(1) - pBri->GetCoverSideArchRib(0));
			if(nDan > 1)
				dArrDim.Add(pBri->GetCoverSideArchRib(2) - pBri->GetCoverSideArchRib(1));
			
			pBri->GetDistArrayMainRebarArchRib(dArrDist, -1, 0);
			for(long j = 0; j < dArrDist.GetSize()-1; j++)
			{
				dArrDim.Add(dArrDist.GetAt(j+1)-dArrDist.GetAt(j));
			}

			if(nDan > 1)
				dArrDim.Add(pBri->GetCoverSideArchRib(2) - pBri->GetCoverSideArchRib(1));
			if(nDan > 0)
				dArrDim.Add(pBri->GetCoverSideArchRib(1) - pBri->GetCoverSideArchRib(0));
			dArrDim.Add(pBri->GetCoverSideArchRib(0));

			CTwinVectorArray tvArrPlane;
			pBri->GetTvPlaneArchRib(tvArrPlane, bUpper, 0);

			CRebarInfoCycle *pRC	= pBri->GetRebarInfoCycleArchRib(0, 0, TRUE, bUpper ? 0 : -1);
			CParabola pbRebar;
			pBri->GetRebarRib(pbRebar, TRUE, 0);
			xyStt.x	= bUpper ? pbRebar.m_xyCrown.x - pbRebar.GetLenByDistToDist(pbRebar.GetDistByXPos(pRC->m_xyMid2.x), 0) : 
								pbRebar.m_xyCrown.x + pbRebar.GetLenByDistToDist(0, pbRebar.GetDistByXPos(pRC->m_xyMid1.x));
			xyStt.y	= tvArrPlane.GetRect().bottom;
			xyStt.x += -pLine->m_dStationBp;

			baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, nDan == nCountDanMainRebar-1, TRUE, FALSE);
		}


		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
		ApplyCuttingArea_Plane_ArchRib_Normal(&Dom, tvArr, nDrawLeft, nCycle==0&&nDan==0&&i==0);
		ApplyCuttingArea_Plane_ArchRib_Normal(&Dom, tvArrJoint, nDrawLeft, FALSE);

		ApplyCuttingArea_Plane_ArchRib_ForDan(&Dom, tvArr, bUpper, nDan, nDrawLeft, nCycle==0&&nDan==0&&i==0);
		ApplyCuttingArea_Plane_ArchRib_ForDan(&Dom, tvArrJoint, bUpper, nDan, nDrawLeft, nCycle==0&&nDan==0&&i==0);

		baseDraw.DrawTvArray(&Dom, tvArr);
		baseDraw.DrawTvArray(&Dom, tvArrJoint);

		CRebarInfoCycle *pRC	= pBri->GetRebarInfoCycleArchRib(nCycle, nDan, bUpper, i);
		CString sDescription	= pRC->m_sDescription;
		CString sMark	= pBri->GetMarkRebarMain(0, FALSE, nCycle, FALSE, FALSE, sDescription, 0);
		if(bMarkRebar)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

			CRebar *pRb = pBri->GetRebarMainByDescription(sDescription);
			CMarkDomyun mark;
			mark.SetDomyun(&Dom, m_pStd->m_pARoadOptionStd);
			mark.SetRebar(tvArr, tvArrSup);
			mark.m_sMark	= sMark;
			mark.m_sDia		= m_pStd->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pBri->GetValueFy(ePartUpperSlab));
			mark.m_nTypeMarkPosLineRebar	= bUpper ? TYPE_MARKPOS_END : TYPE_MARKPOS_STT;
			mark.m_dPosScale	= 0.5;
			if(pBri->GetCountCycleMainRebar() == 2)
			{
				switch(nCycle)
				{
				case 0: mark.m_dPosScale	= 0.3; break;
				case 1: mark.m_dPosScale	= 0.7; break;
				}
			}
			else
			{
				switch(nCycle)
				{
				case 0: mark.m_dPosScale	= 0.2; break;
				case 1: mark.m_dPosScale	= 0.4; break;
				case 2: mark.m_dPosScale	= 0.6; break;
				case 3: mark.m_dPosScale	= 0.8; break;
				}
			}

			if(1)
			{
				CDPoint xyMark	= mark.GetXyMark();
				CTwinVectorArray tvArrPlane;
				pBri->GetTvPlaneArchRib(tvArrPlane, TRUE, 0);
				CDRect rect	= tvArrPlane.GetRect();

				mark.m_dAddHeight	= bUpper ? rect.top - xyMark.y : xyMark.y - rect.bottom;
			}
			mark.m_dCtc = pBri->GetCtcRebarMain(FALSE);
			mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;
			
			mark.MarkRebar();
		}

	}

	*pDomP << Dom;
}

CDRect CDrawPlane::DrawRebar_Plane_ArchRibStd(CDomyun *pDomP, long nCycle, BOOL bUpperLeft, BOOL bUpperRight, BOOL bDimRebar, BOOL bMarkRebar)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return CDRect(0, 0, 0, 0);
	m_pStd->SetMarkForMarkRebar("");

	CDomyun Dom(pDomP);

	long nCycleStt	= nCycle == -1 ? 0 : nCycle;
	long nCycleEnd	= nCycle == -1 ? pBri->GetCountCycleMainRebar() : nCycle+1;
	
	CHgBaseDrawStd baseDraw;

	// 평면도 일반도
	CTwinVectorArray tvArrPlane;
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	if(bUpperLeft == bUpperRight)
	{
		pBri->GetTvPlaneArchRib(tvArrPlane, bUpperLeft, 0);
		baseDraw.DrawTvArray(&Dom, tvArrPlane);
	}
	else
	{
		CTwinVectorArray tvArrPlane1, tvArrPlane2;
		CRebarPlacing rb;
		pBri->GetTvPlaneArchRib(tvArrPlane1, bUpperLeft, -1);
		pBri->GetTvPlaneArchRib(tvArrPlane2, bUpperRight, 1);
		rb.UnionTvArrAndTvArr(tvArrPlane1, tvArrPlane2, tvArrPlane);
			
		baseDraw.DrawTvArray(&Dom, tvArrPlane, m_pStd->IsViewVectorInfo());
	}



	Dom.SetCalcExtRect();
	CDRect rectTot	= Dom.GetExtRect();
	*pDomP << Dom;
	
	// 중심선 /////////////////////
	double dLenExt	= Dom.Always(Dom.GetDimExo());
	double dWidthL	= pBri->GetWidthArchRib(-1);
	double dWidthR	= pBri->GetWidthArchRib(1);
	CTwinVector tvCen;
	tvCen.m_v1.x	= rectTot.left - dLenExt;
	tvCen.m_v2.x	= rectTot.right + dLenExt;
	tvCen.m_v1.y	= -(dWidthR - dWidthL)/2;
	tvCen.m_v2.y	= tvCen.m_v1.y;
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
	baseDraw.DrawTv(&Dom, tvCen);
	*pDomP << Dom;

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SYMB);
	CConcSymbol sym;
	CDPoint xyOrg	= tvCen.m_v1;
	xyOrg.x += dLenExt/2;
	sym.ArrowSide(&Dom, "하 면", "상 면", xyOrg);
	Dom.Rotate(xyOrg, CDPoint(0, 1));
	*pDomP << Dom;
	/////////////////////////////
	
	long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		// 평면도 주철 주철근
		for(long cycle = nCycleStt; cycle < nCycleEnd; cycle++)
		{
			DrawRebarMain_Plane_ArchRib(&Dom, cycle, bUpperLeft, dan, -1, cycle==0&&dan==0, TRUE);
			DrawRebarMain_Plane_ArchRib(&Dom, cycle, bUpperRight, dan, 1, cycle==0&&dan==0, TRUE);
		}

		DrawRebarSupport_Plane_ArchRib(&Dom, bUpperLeft, dan, dan==0, dan==0, -1);
		DrawRebarSupport_Plane_ArchRib(&Dom, bUpperRight, dan, dan==0, dan==0, 1);
	}

	// 전단철근
	DrawRebarShear_Plane_ArchRib(&Dom, bUpperLeft ? -1 : 1);
	*pDomP << Dom;
	
	return rectTot;
}

void CDrawPlane::DrawRebarSupport_Plane_ArchRib(CDomyun *pDomP, BOOL bUpper, long nDan, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CHgBaseDrawStd baseDraw;

	CDomyun Dom(pDomP);

	CTwinVectorArray tvArr;
	pBri->GetTvRebarSupport_Plane_ArchRib(tvArr, bUpper, nDan, nDrawLeft);
	
	if(bDimRebar)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		CDPointArray xyArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		long nDimDan	= 0;
		CString sDir	= bUpper ? "TOP" : "BOTTOM";
		CDPoint xyStt	= CDPoint(0, 0);

		CTwinVectorArray tvArrMain, tvArrJoint;
		pBri->GetTvRebarMain_Plane_ArchRib(tvArrMain, tvArrJoint, 0, bUpper, 0, 0, nDrawLeft);
		
		if(tvArrMain.GetSize() > 0)
			xyArrDim.Add(tvArrMain.GetAt(0).m_v1);
		long i = 0; for(i = 0; i < tvArr.GetSize(); i++)
			xyArrDim.Add(tvArr.GetAt(i).m_v1);

		pBri->GetTvRebarMain_Plane_ArchRib(tvArrMain, tvArrJoint, 0, bUpper, 0, -1, nDrawLeft);
		if(tvArrMain.GetSize() > 0)
			xyArrDim.Add(tvArrMain.GetAt(0).m_v2);

		CTwinVectorArray tvArrPlane;
		pBri->GetTvPlaneArchRib(tvArrPlane, bUpper, 0);
		xyStt	= xyArrDim.GetAt(0);
		xyStt.y	= bUpper ? tvArrPlane.GetRect().top : tvArrPlane.GetRect().bottom;

		m_pStd->AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, TRUE, FALSE, TRUE, FALSE, FALSE, TRUE, TRUE, 90, TRUE, "", NULL, 3);
	}


	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	ApplyCuttingArea_Plane_ArchRib_Normal(&Dom, tvArr, nDrawLeft, FALSE);

	ApplyCuttingArea_Plane_ArchRib_ForDan(&Dom, tvArr, bUpper, nDan, nDrawLeft, FALSE);
	baseDraw.DrawTvArray(&Dom, tvArr);
	
	CString sDescription	= _T("");
	sDescription.Format("아치리브 %s %d단 배력철근", bUpper ? "배선" : "복선", nDan+1);
	CString sMark	= pBri->GetMarkRebarSupportArchRib(0, FALSE, FALSE, FALSE, sDescription);
	if(bMarkRebar)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		long upper	= bUpper ? iUPPER : iLOWER;
		CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_SupportRebar_ArchRib[upper][nDan][iLEFT];

		CTwinVectorArray tvArrMain, tvArrJoint;
		pBri->GetTvRebarMain_Plane_ArchRib(tvArrMain, tvArrJoint, 0, TRUE,0, 0, nDrawLeft);

		CRebar *pRb = pBri->GetRebarSupportByDescription(sDescription);
		CMarkDomyun mark;
		mark.SetDomyun(&Dom, m_pStd->m_pARoadOptionStd);
		mark.SetRebar(tvArr, tvArrMain);
		mark.m_sMark	= sMark;
		mark.m_sDia		= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(ePartUpperSlab));
		mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_END;
		mark.m_dCtc = pPlace->GetCTC();
		mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;

		mark.MarkRebar();

	}


	*pDomP << Dom;
}

void CDrawPlane::ApplyCuttingArea_Plane_ArchRib_Normal(CDomyun *pDomP, CTwinVectorArray &tvArr, long nDrawLeft, BOOL bDrawCuttingArea)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	

	CDPoint xyCen(0, 0);
	double dR	= 0;
	double dAngStt	= 0;
	double dAngEnd	= 0;
	CRebarPlacing rb;
	pBri->GetInfoCuttingCircle_Plane_ArchRib(xyCen, dR, dAngStt, dAngEnd, nDrawLeft);
	if(dR > 0)
	{
		rb.GetTvArrayByCircleArea(tvArr, xyCen, dR, TRUE);

		if(bDrawCuttingArea)
		{
			CDomyun Dom(pDomP);
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
			Dom.Arc(xyCen, dR, dAngStt, fabs(dAngEnd-dAngStt));
			*pDomP << Dom;
		}
	}
}

void CDrawPlane::ApplyCuttingArea_Plane_ArchRib_ForDan(CDomyun *pDomP, CTwinVectorArray &tvArr, BOOL bUpper, long nDan, long nDrawLeft, BOOL bDrawCuttingArea)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
	
	CRebarPlacing rb;
	if(nDan == 0)
	{
		CDPoint xyCen(0, 0);
		double dR	= 0;
		double dAngStt	= 0;
		double dAngEnd	= 0;
		long dan = 0; for(dan = 1; dan < MAX_REBAR_DAN; dan++)
		{
			pBri->GetInfoCuttingCircle_Plane_ArchRib_ForDan(xyCen, dR, dAngStt, dAngEnd, bUpper, dan, nDrawLeft);
			if(dR > 0)
			{
				rb.GetTvArrayByCircleArea(tvArr, xyCen, dR, FALSE);
	
				if(bDrawCuttingArea)
				{
					Dom.Arc(xyCen, dR, dAngStt, fabs(dAngEnd-dAngStt));
				}
			}
		}
	}
	else
	{
		CDPoint xyCen(0, 0);
		double dR	= 0;
		double dAngStt	= 0;
		double dAngEnd	= 0;
		pBri->GetInfoCuttingCircle_Plane_ArchRib_ForDan(xyCen, dR, dAngStt, dAngEnd, bUpper, nDan, nDrawLeft);
		if(dR > 0)
		{
			rb.GetTvArrayByCircleArea(tvArr, xyCen, dR, TRUE);

			if(bDrawCuttingArea)
			{
				Dom.Arc(xyCen, dR, dAngStt, fabs(dAngEnd-dAngStt));
			}
		}
	}

	*pDomP << Dom;
}

// 아치리브 평면 전단철근
void CDrawPlane::DrawRebarShear_Plane_ArchRib(CDomyun *pDomP, long nDrawLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	CHgBaseDrawStd baseDraw;

	CTwinVectorArray tvArr;
	pBri->GetTvRebarShear_Plane_ArchRib(tvArr, nDrawLeft);

	// 전단철근 모양으로 만들기
	CRebarPlacing rb;
	rb.GetTvArrShearRebar(tvArr, FALSE, pBri->m_RebarShearDetail_ArchRib.m_nType == TYPE_SHEAR_DIGUT_BEND, FALSE);

	// nDrawLeft에 따라서 필요한 것만 골라냄
	if(nDrawLeft != 0)
	{
		CDPoint xyCen(0, 0);
		double dR;
		pBri->GetInfoCuttingCircle_Plane_ArchRib_Shear(xyCen, dR);

		rb.GetTvArrayByCircleArea(tvArr, xyCen, dR, TRUE);

		// 자르는 타입이 원인 경우 자르는 영역을 그려준다.
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
		Dom.Circle(xyCen, dR);
	}

	// 전단철근 그리기
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);	
	baseDraw.DrawTvArray(&Dom, tvArr);
	
	*pDomP << Dom;
}

void CDrawPlane::DrawPlane_Section_Arch(CDomyun *pDomP, BOOL bStt)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	CHgBaseDrawStd baseDraw;

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	CTwinVectorArray tvArrPlane;
	pBri->GetTvPlane_Section_ArchRib(tvArrPlane, bStt);
	baseDraw.DrawTvArray(&Dom, tvArrPlane, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());

	

	// 텍스트
	Dom.SetTextAlignHorz(TA_CENTER);
	Dom.SetTextAlignVert(TA_CENTER);
	CTwinVector tv	= tvArrPlane.GetTvByInfo("아치리브횡단상면");
	CTwinVector tvLeft	= tvArrPlane.GetTvByInfo("아치리브횡단좌측면");
	CDPoint xyDir	= tv.GetXyDir();
	CDPoint xyCen	= bStt ? tv.GetXyMid() + xyDir.RotateInv90() * tvLeft.GetLength()/2 : 
							tv.GetXyMid() + xyDir.Rotate90() * tvLeft.GetLength()/2;
	//Dom.Rotate(xyCen, ToDPointFrDegree(90-xyDir.GetAngleDegree()));
	*pDomP << Dom;

	Dom.TextOut(xyCen, "SPRING GING");
	Dom.Rotate(xyCen, xyDir);

	if(bStt)
	{
		Dom.Rotate(xyCen, CDPoint(-1, 0));
	}
	*pDomP << Dom;
}

// 아치리브 평면 치수
void CDrawPlane::DimPlaneArchRibWidth(CDomyun *pDomP)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);


	if(pBri->GetJijumConnectRibAndSlab() > 0)
	{
		CWallApp *pWall	= pBri->GetInWall(pBri->GetJijumConnectRibAndSlab()-1);
		
		CTwinVector tv	= pWall->m_tvArrPlan.GetTvByInfo("벽체평면우측");
		CDPoint xy	= tv.m_v1;

		Dom.SetDirection("RIGHT");
		Dom.DimMoveTo(xy.x, xy.y, 0);
		Dom.DimLineTo(tv.GetLength(), 0, COMMA(tv.GetLength()));

		Dom.Rotate(xy, tv.GetXyDir().RotateInv90());
	}

	*pDomP << Dom;
}

void CDrawPlane::StretchCurringArea(CTwinVectorArray &tvArr, double dLenStretch)
{
	if(tvArr.GetSize() == 0) return;

	CRebarPlacing rb;
	rb.StretchTvArr(tvArr, dLenStretch, dLenStretch);
}
// 일반도 출력시 날개벽 평면 치수
void CDrawPlane::DimPlaneWingWall(CDomyun *pDom)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	
	CDomyun Dom(pDom);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;

		pBri->GetTvWingWallPlane(bStt);
		long left = 0; for(left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT;
			if(!pBri->IsWingWall(bStt, bLeft)) continue;
			CWingWall *pWing	= pBri->GetWingWall(bStt, bLeft);
			if(!pWing) continue;
			if(!pWing->m_bExist) continue;

			CString sLeft	= bLeft ? "좌측" : "우측";
			CString sSide	= _T("");
			CString sEnd	= _T("");
			sSide.Format("%s날개벽외측", sLeft);
			sEnd.Format("%s날개벽끝단", sLeft);

			CTwinVector tvSide, tvEnd;
			if(pBri->m_tvArrPlaneWingWall.GetTvByInfo(sSide, tvSide))
			{
				Dom.SetDirection("BOTTOM");
				if(bStt && !bLeft || !bStt && bLeft)
					Dom.SetDirection("TOP");

				if(bStt)
					GetSwap(tvSide.m_v1, tvSide.m_v2);
				
				Dom.DimMoveTo(tvSide.m_v1.x, tvSide.m_v1.y, 0);
				Dom.DimLineTo(tvSide.GetLength(), 0, COMMA(tvSide.GetLength()));
				Dom.Rotate(tvSide.m_v1, tvSide.GetXyDir());
				*pDom << Dom;
			}

			if(pBri->m_tvArrPlaneWingWall.GetTvByInfo(sEnd, tvEnd))
			{
				Dom.SetDirection("TOP");

				if(!bStt)
					Dom.SetDirection("BOTTOM");

				if(bStt && !bLeft || !bStt && bLeft)
					GetSwap(tvEnd.m_v1, tvEnd.m_v2);
				
				Dom.DimMoveTo(tvEnd.m_v1.x, tvEnd.m_v1.y, 0);
				Dom.DimLineTo(tvEnd.GetLength(), 0, COMMA(tvEnd.GetLength()));
				Dom.Rotate(tvEnd.m_v1, tvEnd.GetXyDir());
				*pDom << Dom;
			}
		}
	}

	*pDom << Dom;
}

// test용
// void CDrawPlane::DrawRebar_Plane_UpperSlabStdForTest(CDomyun *pDomP, long nCycle, BOOL bUpperLeft, BOOL bUpperRight, BOOL bDimRebar, BOOL bMarkRebar, BOOL bRotate, long nDirMark)
// {
// 	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
// 	if(!pBri) return;
// 	pBri->m_bCaledRebarMain_Plane	= FALSE;
// 	m_pStd->SetMarkForMarkRebar("");
// 
// 	CDomyun Dom(pDomP);
// 
// 	long nCycleStt	= nCycle == -1 ? 0 : nCycle;
// 	long nCycleEnd	= nCycle == -1 ? pBri->GetCountCycleMainRebar() : nCycle+1;
// 	
// 	CHgBaseDrawStd baseDraw;
// 	// 평면도 일반도
// 	CTwinVectorArray tvArrPlane;
// 	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
// 	pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1, TRUE);
// 
// 	// 평면도 일반도는 브라켓 부분 고려해서 그려야 된다.
// 	CTwinVectorArray tvArrCuttingBracket;
// 	pBri->GetTvCuttingArea_Plane_UpperSlabBracket(tvArrCuttingBracket);
// 	baseDraw.DrawTvArrayApplyOverlapShape(&Dom, tvArrPlane, tvArrCuttingBracket);
// 
// 	CDRect rectTot;
// 	if(bRotate)
// 	{
// 		m_pStd->RotateDomByBridgeSttEndXy(&Dom);
// 		Dom.SetCalcExtRect();
// 		rectTot	= Dom.GetExtRect();
// 		*pDomP << Dom;
// 	}
// 
// 
// 	// 중심선
// 	double dLenExt	= Dom.Always(Dom.GetDimExo());
// 	DrawPlaneCenterLineEachSlab(&Dom, dLenExt, FALSE, FALSE);	
// 
// 
// 	// 배근도 구하기 전 준비 작업 (속도 개선을 위한 중복작업 일괄 처리 )
// 	pBri->SettingValueRebarForSlab();
// 	////////////////////////////////////////////////////////////////////
// 
// 	pBri->m_bCaledRebarMain_Plane	= FALSE;
// 	pBri->m_bCaledRebarSupport_Plane	= FALSE;
// 	
// 	long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
// 	{
// 		// 평면도 주철 주철근
// 		for(long cycle = nCycleStt; cycle < nCycleEnd; cycle++)
// 		{
// 			//DrawRebarMain_Plane_UpperSlab(&Dom, bUpperLeft, dan, cycle, -1, dan==0&&cycle==0&&bDimRebar, bMarkRebar, dan==0&&cycle==0&&bDimRebar, nDirMark);
// 			DrawRebarMain_Plane_UpperSlab(&Dom, bUpperRight, dan, cycle, 1, dan==0&&cycle==0&&bDimRebar, TRUE, dan==0&&cycle==0&&TRUE, nDirMark);
// 		}
// 
// 		// 가각부 주철근
// 		//DrawRebarMain_Plane_UpperSlab_Exp(&Dom, bUpperLeft, dan, dan==0, dan==0, -1);
// 		//DrawRebarMain_Plane_UpperSlab_Exp(&Dom, bUpperRight, dan, dan==0, dan==0, 1);
// 		
// 
// 		// 평면도 배력철근
// 		/*DrawRebarSupport_Plane_UpperSlab(&Dom, bUpperLeft, dan, -1, dan==0&&bDimRebar, bMarkRebar);
// 		DrawRebarSupport_Plane_UpperSlab(&Dom, bUpperRight, dan, 1, dan==0&&bDimRebar, bMarkRebar);*/
// 	}
// 	// 평면도 거더철근/헌치부철근 배력/주철근
// 	// 교대,교각부, 기둥부 거더 철근 모두 그림
// 	for(long nJ = 0; nJ < pBri->GetCountJijum(); nJ++)
// 	{
// 		// 거더부 철근 (배력철근)
// 		//DrawRebarSupport_Plane_UpperSlabGirderAndHunch(&Dom, nJ, TRUE, 0, TRUE, FALSE, FALSE, -1);
// 
// 		// 거더부 철근 (주철근)
// 		//DrawRebarMain_Plane_UpperSlabGirder(&Dom, nJ, FALSE, FALSE, -1);
// 
// 		// 헌치부 철근 (배력철근)
// 		//DrawRebarSupport_Plane_UpperSlabGirderAndHunch(&Dom, nJ, TRUE, 0, FALSE, FALSE, FALSE, 1);
// 
// 		// 헌치부 철근 (주철근)
// // 		for(long cycle = nCycleStt; cycle < nCycleEnd; cycle++)
// // 			DrawRebarMain_Plane_UpperSlabHunch(&Dom, cycle, nJ, 0, FALSE, TRUE, 0, nDirMark);
// 	}
// 
// 	// 종거더
// 	long left = 0; for(left = 0; left < 2; left++)
// 	{
// 		for(long gagak = 0; gagak < 2; gagak++)
// 		{
// 			BOOL bGagak	= gagak == 1;
// 			long stt = 0; for(stt = 0; stt < 2; stt++)
// 			{
// 				if(!bGagak)
// 				{
// 					if(stt == 1) continue;
// 				}
// 
// // 				DrawRebarSupport_Plane_UpperSlabGirderJong(&Dom, bStt, bLeft, bGagak, TRUE, FALSE, TRUE, FALSE);
// // 				DrawRebarStirrup_Plane_UpperSlabGirderJong(&Dom, bStt, bLeft, bGagak, FALSE, TRUE, FALSE);
// 			}
// 		}
// 	}
// 	// 평면도 브라켓부 일반도와 철근
// 	//DrawPlaneGenBracket(&Dom, FALSE);
// 	//DrawRebarMain_Plane_UpperSlabBracket(&Dom, FALSE, FALSE, TRUE, -1);
// 	//DrawRebarSupport_Plane_UpperSlabBracket(&Dom, FALSE, FALSE, TRUE, -1);
// 
// 	// 평면도 전단철근
// 	//DrawRebarShear_Plane_UpperSlab(&Dom, 0);
// 
// 	// 내측벽체와 헌치선 평면도(점선)
// 	DrawPlaneGenInWallAndHunch(&Dom);
//   
// 	if(bRotate)
// 		m_pStd->RotateDomByBridgeSttEndXy(&Dom);
// 	
// 	*pDomP << Dom;
// 
// 	// 배근도 구하기 전 준비 작업한거 초기화 (속도 개선을 위한 중복작업 일괄 처리 )
// 	pBri->InitValueRebarForUpperSlab();
// 	////////////////////////////////////////////////////////////////////
// }

CDRect CDrawPlane::DrawRebar_Plane_LowerSlabStd( CDomyun *pDomP, long nCycle, BOOL bUpperLeft, BOOL bUpperRight, BOOL bDimRebar, BOOL bMarkRebar, BOOL bRotate, long nDirMark/*=2*/, BOOL bForSlabStandard/*=FALSE*/, BOOL bDrawHunchRebar )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return CDRect(0, 0, 0, 0);
	m_pStd->SetMarkForMarkRebar("");
	m_bApplyCutAreaHunch = bDrawHunchRebar;

	CDomyun Dom(pDomP);

	long nCycleStt	= nCycle == -1 ? 0 : nCycle;
	long nCycleEnd	= nCycle == -1 ? pBri->GetCountCycleMainRebar() : nCycle+1;

	CHgBaseDrawStd baseDraw;
	// 평면도 일반도
	CTwinVectorArray tvArrPlane;
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1, TRUE);

	// 하부슬래브 평면도 일반도는 부상방지저판 부분 고려해서 그려야 된다.
	CTwinVectorArray tvArrCuttingPRF;
	pBri->GetTvCuttingArea_Plane_LowerSlabPRF(tvArrCuttingPRF);
	baseDraw.DrawTvArrayApplyOverlapShape(&Dom, tvArrPlane, tvArrCuttingPRF);

	CDRect rectTot;
	if(bRotate)
	{
		m_pStd->RotateDomByBridgeSttEndXy(&Dom);
		Dom.SetCalcExtRect();
		rectTot	= Dom.GetExtRect();
		*pDomP << Dom;
	}


	// 중심선
	double dLenExt	= Dom.Always(Dom.GetDimExo());
	if(bUpperLeft != bUpperRight)
		DrawPlaneCenterLineEachSlab(&Dom, dLenExt, FALSE, FALSE);	

	// (ARCBRIDGE-3517) 일반도용 선형중심선도 추가해준다.
	DrawPlaneCenterLineEachSlab(&Dom, dLenExt, FALSE, TRUE);


	// 배근도 구하기 전 준비 작업 (속도 개선을 위한 중복작업 일괄 처리 )
	pBri->SettingValueRebarForSlab();
	////////////////////////////////////////////////////////////////////

	long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		// 평면도 주철 주철근
		for(long cycle = nCycleStt; cycle < nCycleEnd; cycle++)
		{
			if(bUpperLeft != bUpperRight)
			{
				DrawRebarMain_Plane_LowerSlab(&Dom, bUpperLeft, dan, cycle, -1, dan==0&&cycle==0&&bDimRebar, bMarkRebar, dan==0&&cycle==0&&bDimRebar, nDirMark);
				DrawRebarMain_Plane_LowerSlab(&Dom, bUpperRight, dan, cycle, 1, dan==0&&cycle==0&&TRUE, TRUE, dan==0&&cycle==0&&TRUE, nDirMark);
			}
			else
			{
				DrawRebarMain_Plane_LowerSlab(&Dom, bUpperLeft, dan, cycle, 0, dan==0&&cycle==0&&bDimRebar, bMarkRebar, dan==0&&cycle==0&&bDimRebar, nDirMark);
			}
		}

		// 가각부 주철근
		if(bUpperLeft != bUpperRight)
		{
			DrawRebarMain_Plane_Slab_Exp(&Dom, FALSE, bUpperLeft, dan, dan==0, dan==0, -1);
			DrawRebarMain_Plane_Slab_Exp(&Dom, FALSE, bUpperRight, dan, dan==0, dan==0, 1);
		}
		else
		{
			DrawRebarMain_Plane_Slab_Exp(&Dom, FALSE, bUpperLeft, dan, dan==0, dan==0, 0);
		}


		if(!bForSlabStandard)
		{
			// 평면도 배력철근
			if(bUpperLeft != bUpperRight)
			{
				DrawRebarSupport_Plane_LowerSlab(&Dom, bUpperLeft, dan, -1, dan==0&&bDimRebar, bMarkRebar);
				DrawRebarSupport_Plane_LowerSlab(&Dom, bUpperRight, dan, 1, dan==0&&bDimRebar, bMarkRebar);
			}
			else
			{
				DrawRebarSupport_Plane_LowerSlab(&Dom, bUpperLeft, dan, 0, dan==0&&bDimRebar, bMarkRebar);
			}
		}
	}

	// 평면도 거더철근/헌치부철근 배력/주철근
	// 교대,교각부, 기둥부 거더 철근 모두 그림
	for(long nJ = 0; nJ < pBri->GetCountJijum(); nJ++)
	{
		// 거더부 철근 (배력철근)
		DrawRebarSupport_Plane_LowerSlabGirderAndHunch(&Dom, nJ, TRUE, 0, TRUE, FALSE, FALSE, -1);

		// 거더부 철근 (주철근)
		DrawRebarMain_Plane_SlabGirder(&Dom, FALSE, nJ, FALSE, TRUE, -1);

		// 헌치부 철근 (배력철근)
		DrawRebarSupport_Plane_LowerSlabGirderAndHunch(&Dom, nJ, bUpperLeft, 0, FALSE, FALSE, bMarkRebar, -1);

		// 헌치부 철근 (주철근)
		for(long cycle = nCycleStt; cycle < nCycleEnd; cycle++)
			DrawRebarMain_Plane_LowerSlabHunch(&Dom, cycle, nJ, bUpperLeft, 0, FALSE, bMarkRebar, 0, nDirMark);
	}

	// 평면도 부상방지저판 일반도와 철근
	if(!bForSlabStandard)
	{
		if(pBri->m_bExistProtectionRiseFooting)
		{
			DrawPlaneGenLowerSlabPRF(&Dom, FALSE, TRUE, FALSE);
			DrawRebarMain_Plane_LowerSlabPRF(&Dom, FALSE, FALSE, TRUE, -1);
			DrawRebarSupport_Plane_LowerSlabPRF(&Dom, FALSE, FALSE, TRUE, -1);
		}

		// 평면도 전단철근
		DrawRebarShear_Plane_LowerSlab(&Dom, -1);

		// 내측벽체와 헌치선 평면도(점선)
		 DrawPlaneGenInWallAndHunch(&Dom, FALSE);
	}


	if(bRotate)
		m_pStd->RotateDomByBridgeSttEndXy(&Dom);

	*pDomP << Dom;

	// 배근도 구하기 전 준비 작업한거 초기화 (속도 개선을 위한 중복작업 일괄 처리 )
	pBri->InitValueRebarForUpperSlab();
	////////////////////////////////////////////////////////////////////


	return rectTot;
}

void CDrawPlane::DrawRebarMain_Plane_LowerSlab( CDomyun *pDomP, BOOL bUpper, long nDan, long nCycle, long nDrawLeft, BOOL bDimRebar, BOOL bMarkRebar, BOOL bDimTotal, long nDirMark/*=2*/ )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	// 주철근 간격이 구해진 상태인지?
	BOOL bCalDistMainRebar	= FALSE;	

	// 사보강 배치를 대비해서 사방향 철근이 들어가는 구간의 거리를 미리 설정함.
	pBri->SetDataDefaultDistLeftDirPlacing();

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	BOOL bVertDir	= pBri->IsVertDir();
	BOOL bExtLine	= pBri->IsExtLine();

	// 철근 가져오기
	CHgBaseDrawStd baseDraw;
	baseDraw.m_pLine	= pBri->GetLineBase();

	// 치수기입(nDrawLeft가 -1이나 0일때는 시점에, 1일때는 종점에 기입함)
	CLineInfo *pLine = pBri->GetLineBase(pBri->IsVertDir());
	CLineInfo *pLineL	= pBri->GetLineLeftRight(TRUE);
	CLineInfo *pLineR	= pBri->GetLineLeftRight(FALSE);

	if(bDimRebar)
	{
		*pDomP << Dom;
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		BOOL bStt	= (nDrawLeft == 0 || nDrawLeft == -1) ? TRUE : FALSE;

		// vert == 0 : 직방향 등 기본 철근
		// vert == 1 : 사보강 배치시 사방향 철근
		CTwinVectorArray tvArr, tvArrAngLeft, tvArrAngRight;
		CTwinVectorArray tvArrJoint, tvArrHidden;
		pBri->GetTvRebarMain_Plane_LowerSlab_TypeNormal_Std(tvArr, tvArrAngLeft,tvArrAngRight, bUpper, -1, nDan, 0, 0, FALSE, FALSE, FALSE, tvArrJoint, tvArrHidden, tvArrHidden, tvArrHidden, FALSE);

		// 전체 치수를 넣을 시기를 결정함
		long nDimTotal	= 0;
		BOOL bAngleRebar	= FALSE;
		if(tvArrAngLeft.GetSize() > 0 || tvArrAngRight.GetSize() > 0) bAngleRebar	= TRUE;
		CTwinVector tvStt, tvEnd;
		pBri->GetTvSttEndOfSlab(tvStt, tvEnd);
		GetSwap(tvStt.m_v1, tvStt.m_v2);
		GetSwap(tvEnd.m_v1, tvEnd.m_v2);

		for(long vert = 0; vert < 3; vert++)
		{
			CDoubleArray dArrHunch;
			CDoubleArray dArrDim;
			CStringArray sArrText;
			CStringArray sArrUnderText;
			CString sDir	= bStt ? "TOP" : "BOTTOM";
			CDPoint xyStt	= CDPoint(0, 0);
			long nDimDan	= nDan+2;
			if(bAngleRebar && vert == 0) nDimDan++;

			// 치수기입 기준위치
			CDPoint vAng	= pBri->GetAngleJijum(0);
			double dSta		= pBri->GetStationByTarget(bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0, TRUE);
			CTwinVector tvJigan	= bStt ? tvStt : tvEnd;

			// 사방향 철근은 사방향 배근된 부분만 치수를 기입한다.
			// 따라서 tvJigan을 사방향 배근된 만큼만 살려 놓는다.
			CLineInfo *pLineCur	= pLine;
			if(vert == 1)
			{
				pLineCur	= pLineL;
				tvArr.RemoveAll();
				tvArr.AddFromTvArray(tvArrAngLeft);

				if(tvArr.GetSize() == 0) continue;

				CTwinVector tv	= tvArr.GetAt(tvArr.GetSize()-1);
				if(pBri->m_nTypeSideRebarMain_UpperSlabSabogang == 0)
				{
					GetXyMatchLineAndLine(tv.m_v1, tv.GetXyDir(), tvJigan.m_v1, tvJigan.GetXyDir(), tvJigan.m_v2);
				}
				else
				{
					double dDist	= pLineCur->GetDisMatchVerticalByXy(tv.m_v1);
					double sta		= dSta;
					pLineCur->GetStationMatchByXyAndAngle(tvJigan.m_v1, tvJigan.GetXyDir(), sta, dDist);
					tvJigan.m_v2	= pLineCur->GetXyLineDis(sta, dDist);
				}
			}
			else if(vert == 2)
			{
				pLineCur	= pLineR;
				tvArr.RemoveAll();
				tvArr.AddFromTvArray(tvArrAngRight);
				pBri->SortTvArrByLinePlane(tvArr);

				if(tvArr.GetSize() == 0) continue;

				CTwinVector tv	= tvArr.GetAt(0);
				if(pBri->m_nTypeSideRebarMain_UpperSlabSabogang == 0)
				{
					GetXyMatchLineAndLine(tv.m_v1, tv.GetXyDir(), tvJigan.m_v1, tvJigan.GetXyDir(), tvJigan.m_v1);
				}
				else
				{
					double dDist	= pLineCur->GetDisMatchVerticalByXy(tv.m_v1);
					double sta		= dSta;
					pLineCur->GetStationMatchByXyAndAngle(tvJigan.m_v1, tvJigan.GetXyDir(), sta, dDist);
					tvJigan.m_v1	= pLineCur->GetXyLineDis(sta, dDist);
				}
			}

			xyStt	= tvJigan.m_v2;
			// 주철근을 기준위치로 뻗어서 거리 구함.
			bCalDistMainRebar	= TRUE;

			// 가각 철근이 선형방향 배치면 치수를 포함시킨다.
			// 단, 주철근이 선형방향 배치이어야 한다
			if(pBri->m_nTypePlacingMainRebar_UpperSlab == 0)
			{
				long left = 0; for(left = 0; left < 2; left++)
				{
					BOOL bLeft	= left == iLEFT;
					if(pBri->m_bLineDirRebarOfExp[bStt ? iSTT : iEND][left])
					{
						CTwinVectorArrayArray pTvArrCycle1[2];
						CTwinVectorArrayArray pTvArrCycle2[2];

						pBri->GetTvRebarMain_Plane_Slab_Exp(FALSE, &pTvArrCycle1[0], &pTvArrCycle2[0], &pTvArrCycle1[1], &pTvArrCycle2[1], bStt, bLeft, 0, bUpper);

						long i = 0; for(i = 0; i < pTvArrCycle2[0].GetSize(); i++)
						{
							if(pTvArrCycle2[0].GetAt(i)->GetSize() == 0) continue;
							tvArr.Add(pTvArrCycle2[0].GetAt(i)->GetAt(0));
						}


						for(long type = 0; type < 2; type++)
						{
							AhTPADelete(&pTvArrCycle1[type], (CTwinVectorArray*)0);
							AhTPADelete(&pTvArrCycle2[type], (CTwinVectorArray*)0);
						}				
					}
				}
			}

			// 가각 철근 치수가 주철근치수에 겹치면 빼버림
			CDPointArray xyArr;
			CDPoint xyMatch(0, 0);
			CDPointArray xyArrMatch;
			CTwinVector tv;
			CDPoint vAngDim	= pBri->GetAngleByTarget(bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0); //pBri->GetAngleJijum(nJ, bVertDir);
			BOOL bPlanArc = FALSE;
			long i = 0; for(i = 0; i < tvArr.GetSize(); i++)
			{
				tv	= tvArr.GetAt(i);
				if(tv.m_v1.z != 0)
				{
					if(vert == 0 || pBri->m_nTypeSideRebarMain_UpperSlabSabogang == 0)
					{
						double dDist	= pLine->GetDisMatchVerticalByXy(tv.m_v1);

						xyMatch	= pLine->GetXyLineByLengthAndAngle(dSta, dDist/vAngDim.y, vAngDim);

						// Arc가 적용된 실제 배근된 치수를 넣으려면 아래와 같이 해야하나
						// 치수가 지저분하게 나와서 하지 않는 것으로 함
						//xyMatch	= pLine->GetXyLineDisByAng(dSta, dDist, vAngDim);
						xyArr.Add(xyMatch);
						bPlanArc = TRUE;
					}
					else
					{
						double dDist	= pLineCur->GetDisMatchVerticalByXy(tv.m_v1);
						double sta		= dSta;
						pLineCur->GetStationMatchByXyAndAngle(tvJigan.m_v1, tvJigan.GetXyDir(), sta, dDist);
						xyMatch	= pLineCur->GetXyLineDis(sta, dDist);
						xyArr.Add(xyMatch);
					}
				}
				else
				{
					if(GetXyMatchSegAndLine(tvJigan.m_v1, tvJigan.m_v2, tv.m_v1, tv.GetXyDir(), xyMatch))
						xyArr.Add(xyMatch);
				}
			}

			//#0029484 - 전체 치수를 표현할때는 xyMatch를 넣을필요가 없다.
			if(bPlanArc && vert != 0)
			{
				tv	= tvArr.GetAt(0);
				double dDist	= pLine->GetDisMatchVerticalByXy(tv.m_v1);
				if(dDist < 0)
					dDist -= pBri->m_dCoverSide_UpperSlab[nDan];
				else
					dDist += pBri->m_dCoverSide_UpperSlab[nDan];

				xyMatch	= pLine->GetXyLineByLengthAndAngle(dSta, dDist/vAngDim.y, vAngDim);				
				xyArr.Add(xyMatch);				
			}

			long nSize	= xyArr.GetSize();
			if(nSize > 0)
			{
				CDPoint xyLast(0, 0);
				CDPoint xy(0, 0);
				CDoubleArray dArrDimTmp;

				double dDistMax	= tvJigan.GetLength();
				double dDist	= 0;

				long i = 0; for(i = 0; i < nSize; i++)
				{
					xy	= xyArr.GetAt(i);
					dDist	= ~(xy - tvJigan.m_v2);

					dArrDimTmp.Add(dDist);
				}

				//#0029484 - 전체 치수를 표현할때는 tvJigan의 길이를 넣어야 한다.
				if(!bPlanArc ||  vert == 0)
					dArrDimTmp.Add(tvJigan.GetLength());

				dArrDimTmp.Sort(TRUE);


				nSize	= dArrDimTmp.GetSize();
				if(dArrDimTmp.GetSize() > 0)
				{
					dDist	= dArrDimTmp.GetAt(0);
					dArrDim.Add(dDist);
					for(i = 1; i < nSize; i++)
					{
						dDistMax	= dArrDimTmp.GetAt(i);
						dDist		= dArrDimTmp.GetAt(i-1);

						if(dDistMax - dDist > 0)
							dArrDim.Add(dDistMax-dDist);
					}
				}
			}

			// 치수 기입
			// 주철근이 선형방향배근이라면 치수선에 obq 적용
			double dObq	=  0;
			if(pBri->m_nTypePlacingMainRebar_UpperSlab == 0)
			{
				double dAng		= pBri->GetAngleByTarget(bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0).GetAngleDegree();// pBri->GetAngleJijum(bStt ? 0 : pBri->m_nQtyJigan).GetAngleDegree();
				dObq	= bStt ? dAng - 90 : 90 - dAng;
				Dom.SetDimObq(dObq);
			}
			else	// obq적용 안되는 경우 사각에 따라 시작위치 sta 조정 필요...
			{
				double dAng		= pBri->GetAngleByTarget(bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0).GetAngleDegree();
				if((bStt && dAng > 90) || (!bStt && dAng < 90))
				{
					CLineInfo *pLineB	= pBri->GetLineBase();
					if(!pLineB) return;	// program 다운 방지를 위해 깔끔하게 리턴시켜버림

					double dSta	= 0;
					if(pBri->m_nTypePlacingMainRebar_UpperSlab == 1 || pBri->m_nTypePlacingMainRebar_UpperSlab == 2)
						pLineB->GetStationMatchVerticalByXy(tvJigan.m_v2, dSta);	// 중심선형에서의 기점
					else
						pLineB->GetStationMatchVerticalByXy(tvJigan.m_v1, dSta);	// 중심선형에서의 기점
// 					xyStt	= pBri->GetXySlabEdgeAct(dSta, CDPoint(0, 1), FALSE);

					xyStt	= tvJigan.m_v2;
				}
			}
			baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, bDimTotal&&vert==nDimTotal, TRUE, FALSE);

			// 방향에 맞게 회전(시종점 접선의 직각방향으로 회전)
			CDPoint xyDir	= pBri->GetLineBase()->GetAngleAzimuth(dSta).Rotate90();
			Dom.Rotate(xyStt, ToDPointFrDegree(xyDir.GetAngleDegree()+(bStt ? dObq : -dObq)));

			// 사보강 배치인 경우 
			if(pBri->m_nTypePlacingMainRebar_UpperSlab == 1 || pBri->m_nTypePlacingMainRebar_UpperSlab == 2)
			{
				Dom.Rotate(xyStt, pBri->GetAngleByTarget(bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0).RotateInv90());
			}

			*pDomP << Dom;
		}
	}

	// 절단원 적용해서 그리기.
	CTwinVectorArray tvArr, tvArrAngLeft, tvArrAngRight;
	CTwinVectorArray tvArrAngLeftHalf, tvArrAngRightHalf;
	CTwinVectorArray tvArrJoint, tvArrHidden;
	pBri->GetTvRebarMain_Plane_LowerSlab_TypeNormal_Std(tvArr, tvArrAngLeft, tvArrAngRight, bUpper, nCycle, nDan, nDrawLeft, -1, FALSE, FALSE, FALSE, tvArrJoint, tvArrHidden, tvArrHidden, tvArrHidden, TRUE, tvArrAngLeftHalf, tvArrAngRightHalf, TRUE);

	ApplyCuttingArea_Plane(&Dom, tvArr, bUpper, nDan, FALSE, nDan==0, FALSE, FALSE, FALSE, 0, FALSE, FALSE, pBri->GetLineBase(), FALSE);
	ApplyCuttingArea_Plane(&Dom, tvArrAngLeft, bUpper, nDan, FALSE, nDan==0, FALSE, FALSE, FALSE, 0, FALSE, FALSE, pBri->GetLineLeftRight(TRUE), FALSE);
	ApplyCuttingArea_Plane(&Dom, tvArrAngRight, bUpper, nDan, FALSE, nDan==0, FALSE, FALSE, FALSE, 0, FALSE, FALSE, pBri->GetLineLeftRight(FALSE), FALSE);
	ApplyCuttingArea_Plane(&Dom, tvArrJoint, bUpper, nDan, FALSE, nDan==0, FALSE, FALSE, FALSE, 0, FALSE, FALSE, NULL, FALSE);
	ApplyCuttingArea_Plane(&Dom, tvArrHidden, bUpper, nDan, FALSE, nDan==0, FALSE, FALSE, FALSE, 0, FALSE, FALSE, NULL, FALSE);
	ApplyCuttingArea_Plane(&Dom, tvArrAngLeftHalf, bUpper, nDan, FALSE, nDan==0, FALSE, FALSE, FALSE, 0, FALSE, FALSE, pBri->GetLineLeftRight(TRUE), FALSE);
	ApplyCuttingArea_Plane(&Dom, tvArrAngRightHalf, bUpper, nDan, FALSE, nDan==0, FALSE, FALSE, FALSE, 0, FALSE, FALSE, pBri->GetLineLeftRight(FALSE), FALSE);
	//
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	m_pStd->DrawTvArrWidthHidden(&Dom, tvArr, pBri->GetLineBase());
	m_pStd->DrawTvArrWidthHidden(&Dom, tvArrAngLeft, pBri->GetLineLeftRight(TRUE));
	m_pStd->DrawTvArrWidthHidden(&Dom, tvArrAngRight, pBri->GetLineLeftRight(FALSE));

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);	
	if((pBri->m_nTypeSideRebarMain_UpperSlabSabogang == 0 || !bVertDir) && !bExtLine)
	{
		baseDraw.m_pLine	= pBri->GetLineBase(bVertDir);
		baseDraw.DrawTvArray(&Dom, tvArrJoint);
	}
	else
	{
		// 사보강배치에 좌우측선형형태 철근이면 각각의 선형별 이음을 구분 해야 된다.
		CTwinVectorArray tvArrJointCen, tvArrJointLeft, tvArrJointRight;
		long i = 0; for(i = 0; i < tvArrJoint.GetSize(); i++)
		{
			CTwinVector tv	= tvArrJoint.GetAt(i);
			if(tv.m_sInfoHor == "좌측이음") tvArrJointLeft.Add(tv);
			else if(tv.m_sInfoHor == "우측이음") tvArrJointRight.Add(tv);
			else tvArrJointCen.Add(tv);
		}

		baseDraw.m_pLine	= pBri->GetLineLeftRight(TRUE);
		baseDraw.DrawTvArray(&Dom, tvArrJointLeft);
		baseDraw.m_pLine	= pBri->GetLineLeftRight(FALSE);
		baseDraw.DrawTvArray(&Dom, tvArrJointRight);
		baseDraw.m_pLine	= pBri->GetLineBase(bVertDir);
		baseDraw.DrawTvArray(&Dom, tvArrJointCen);
	}

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	if((pBri->m_nTypeSideRebarMain_UpperSlabSabogang == 0 || !bVertDir) && !bExtLine)
	{
		m_pStd->DrawTvArrWidthHidden(&Dom, tvArrHidden, pBri->GetLineBase(bVertDir));
	}
	else
	{
		// 사보강배치에 좌우측선형형태 철근이면 각각의 선형별 이음을 구분 해야 된다.
		CTwinVectorArray tvArrHiddenCen, tvArrHiddenLeft, tvArrHiddenRight;
		long i = 0; for(i = 0; i < tvArrHidden.GetSize(); i++)
		{
			CTwinVector tv	= tvArrHidden.GetAt(i);
			if(tv.m_sInfoHor == "좌측점선") tvArrHiddenLeft.Add(tv);
			else if(tv.m_sInfoHor == "우측점선") tvArrHiddenRight.Add(tv);
			else tvArrHiddenCen.Add(tv);
		}
		m_pStd->DrawTvArrWidthHidden(&Dom, tvArrHiddenLeft, pBri->GetLineLeftRight(TRUE));
		m_pStd->DrawTvArrWidthHidden(&Dom, tvArrHiddenRight, pBri->GetLineLeftRight(FALSE));
		m_pStd->DrawTvArrWidthHidden(&Dom, tvArrHiddenCen, pBri->GetLineBase(bVertDir));
	}



	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	m_pStd->DrawTvArrWidthHidden(&Dom, tvArrAngLeftHalf, pBri->GetLineLeftRight(TRUE));
	m_pStd->DrawTvArrWidthHidden(&Dom, tvArrAngRightHalf, pBri->GetLineLeftRight(FALSE));

	if(bMarkRebar)
	{
		CTwinVectorArray tvArrTmp;
		// 사보강 배치가 아닐때는 선형용 치수기입 함수를 사용하고
		// 사보강 배치일때는 기초에서 복사해온 일반 치수기입용 함수를 사용한다. 
		if(pBri->m_nTypePlacingMainRebar_UpperSlab == 0)
		{
			
			long upper	= bUpper ? iUPPER : iLOWER;
			long nSize	= pBri->m_pArrRCMainLowerSlab[nCycle][upper][nDan].GetSize();

			long i = 0; for(i = 0; i < nSize; i++)
			{
				CRebarInfoCycle *pRC	= NULL;
				pRC	= pBri->GetRebarInfoCycleLowerSlab(nCycle, nDan, bUpper, i);
				if(pRC)
				{
					if(pRC->m_bExist)
						DimRebarInfoCycle_Plane_LowerSlab(&Dom, pRC, bUpper, nDan, nCycle, FALSE, nDrawLeft, FALSE, FALSE, FALSE, nDirMark);
				}
			}
		}
		else if(pBri->m_nTypePlacingMainRebar_UpperSlab == 1 || pBri->m_nTypePlacingMainRebar_UpperSlab == 2)
		{
			CRebarPlacing rb;
			CTwinVectorArray tvArrSup, tvArrSupAngStt, tvArrSupAngEnd, tvArrRAngStt, tvArrRAngEnd;
			pBri->GetTvRebarSupport_Plane_Slab_Std(FALSE, tvArrSup, tvArrSupAngStt, tvArrSupAngEnd, tvArrRAngStt, tvArrRAngEnd, bUpper, 0, 0, TRUE);

			CTwinVector tv;
			CRebarInfoCycle *pRC	= NULL;
			long upper			= bUpper ? iUPPER : iLOWER;
			long nSize			= pBri->m_pArrRCMainLowerSlab[nCycle][upper][nDan].GetSize();
			long dCountCycle	= pBri->GetCountCycleMainRebar();

			for(long r = 0; r < nSize; r++)
			{
				// 주철근 구함.
				pRC	= pBri->GetRebarInfoCycleLowerSlab(nCycle, nDan, bUpper, r);
				if(!pRC) continue;
				if(!pRC->m_bExist) continue;

				CRebar *pRb = pBri->GetRebarMainByDescription(pRC->m_sDescription);

				pBri->GetTvRebarMain_Plane_LowerSlab_TypeNormal_Std(tvArr, tvArrAngLeft, tvArrAngRight, bUpper, nCycle, nDan, nDrawLeft, r, FALSE, FALSE, FALSE, tvArrTmp, tvArrTmp, tvArrTmp, tvArrTmp, FALSE, tvArrAngLeftHalf, tvArrAngRightHalf, !bCalDistMainRebar);

				rb.ReverseRebar(tvArr);
				rb.ReverseRebar(tvArrAngLeft);
				rb.ReverseRebar(tvArrAngRight);
				rb.ReverseRebar(tvArrAngLeftHalf);
				rb.ReverseRebar(tvArrAngRightHalf);

				// 수직보강..
				CString sMark	= pRC->m_szMark;
				if(tvArr.GetSize() > 0 && m_pStd->IsMarkRebar(sMark))
				{
					MarkRebarMain_Plane_LowerSlab_ForVert(&Dom, tvArr, tvArrAngLeftHalf, tvArrAngRightHalf, tvArrSup, r, bUpper, nDan, nCycle, FALSE, FALSE, FALSE, nDrawLeft);
				}
			}
		}

		if(pBri->IsVertDir() || pBri->IsExtLine())
		{
			// 사보강 (좌측)
			CString sMark = _T("");

			CRebarPlacing rb;
			CTwinVectorArray tvArrSup, tvArrSupAngStt, tvArrSupAngEnd, tvArrRAngStt, tvArrRAngEnd;
			pBri->GetTvRebarSupport_Plane_Slab_Std(TRUE, tvArrSup, tvArrSupAngStt, tvArrSupAngEnd, tvArrRAngStt, tvArrRAngEnd, bUpper, 0, 0, TRUE);

			CTwinVector tv;
			CRebarInfoCycle *pRC	= NULL;
			long upper			= bUpper ? iUPPER : iLOWER;
			long nSize			= pBri->m_pArrRCMainUpperSlab[nCycle][upper][nDan].GetSize();
			long dCountCycle	= pBri->GetCountCycleMainRebar();
			for(long r = 0; r < nSize; r++)
			{
				// 주철근 구함.
				
				pRC	= pBri->GetRebarInfoCycleLowerSlab(nCycle, nDan, bUpper, r);
				if(!pRC) continue;
				if(!pRC->m_bExist) continue;
				CRebar *pRb = pBri->GetRebarMainByDescription(pRC->m_sDescription);

				if(pBri->IsVertDir() == FALSE || pBri->m_nTypePlacingMainRebar_UpperSlab > 0)
				{
					pBri->GetTvRebarMain_Plane_LowerSlab_TypeNormal_Std(tvArr, tvArrAngLeft, tvArrAngRight, bUpper, nCycle, nDan, nDrawLeft, r, FALSE, FALSE, FALSE, tvArrTmp, tvArrTmp, tvArrTmp, tvArrTmp, FALSE, tvArrAngLeftHalf, tvArrAngRightHalf, !bCalDistMainRebar);

					rb.ReverseRebar(tvArr);
					rb.ReverseRebar(tvArrAngLeft);
					rb.ReverseRebar(tvArrAngRight);
					rb.ReverseRebar(tvArrAngLeftHalf);
					rb.ReverseRebar(tvArrAngRightHalf);
				}

				sMark.Format("%s-1", pRC->m_szMark);
				if(tvArrAngLeft.GetSize() > 0 && m_pStd->IsMarkRebar(sMark) && nDan == 0)	// 1단만 마크해도 복잡한데, 2,3단까지 전부 마크하면 너무 지저분해짐
				{
					CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
					mark.SetRebar(tvArrAngLeft, tvArrSup);

					// 설정
					mark.m_sMark					= sMark;
					mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pBri->GetValueFy(ePartLowerSlab));
					mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_END;
					mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
					mark.m_dPosScaleMarkStt			= 1./(dCountCycle+nSize) * (nCycle+r+1)+0.2;
					mark.m_dPosScale				= 1./(dCountCycle+nSize) * (nCycle+r+1)+0.2;
					mark.m_dPosScaleSpace			= 0.5 - (1./dCountCycle)/2;

					CDPointArray xyArr;
					mark.GetXyCountMark(xyArr);
					// 사보강 배치의 사방향 철근
					if(pBri->m_nTypeSideRebarMain_UpperSlabSabogang == 1 && xyArr.GetSize() > 1)
					{
						CDPoint xyOrg1	= xyArr.GetAt(0);
						CDPoint xyOrg2	= xyArr.GetAt(1);
						CDPoint xyDir	= (xyOrg1-xyOrg2).Unit();
						long nSize		= xyArr.GetSize();
						long i = 0; for(i = 0; i < tvArrAngLeft.GetSize(); i++)
						{
							if(nSize <= i) break;
							CDPoint xyMark		= xyArr.GetAt(i);
							CDPoint xyMarkDim	= tvArrAngLeft.GetAt(i).m_v1;
							double dDist	= pLineL->GetDisMatchVerticalByXy(xyMarkDim);
							double sta		= 0;
							pLineL->GetStationMatchByXyAndAngle(xyMark, xyDir, sta, dDist);
							xyMark	= pLineL->GetXyLineDis(sta, dDist);
							xyArr.SetAt(i, xyMark);
						}
					}

					ApplyCuttingArea_Plane(&Dom, xyArr, bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, 0, FALSE, FALSE, NULL, FALSE);
					mark.m_xyArrCountMark	= xyArr;


					if(xyArr.GetSize() > 1)
					{
						CLineInfo *pLine	= pBri->GetLineBase();
						if(pLine)
						{
							CDPoint xyMark	= mark.GetXyMark(FALSE);
							CDPoint vAng	= (xyArr.GetAt(0)-xyArr.GetAt(1)).Unit();
							CDPoint vAngJ	= pBri->GetAngleJijum(0);
							double dSta		= xyMark.x;
							pLine->GetStationMatchByXyAndAngle(xyMark, vAng, dSta, 0);
							double dWidth	= pBri->GetWidthSlabActByGagak(dSta, vAngJ, -1);
							CDPoint xyMatch	= pLine->GetXyLineByLengthAndAngle(dSta, pBri->IsRightFromRoadCenter() ? dWidth : -dWidth, vAngJ);

							mark.m_dAddHeight	= ~(xyMatch-xyMark) + Dom.GetTextHeight();
						}
						mark.m_dCtc = pBri->GetCtcRebarMain(TRUE);
						mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;

						mark.MarkRebar(FALSE);	
					}
				}

				// 사보강 (우측)
				sMark.Format("%s-2", pRC->m_szMark);
				if(tvArrAngRight.GetSize() > 0 && m_pStd->IsMarkRebar(sMark) && nDan == 0)	// 1단만 마크해도 복잡한데, 2,3단까지 전부 마크하면 너무 지저분해짐
				{
					CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
					mark.SetRebar(tvArrAngRight, tvArrSup);

					// 설정
					mark.m_sMark					= sMark;
					mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pBri->GetValueFy(ePartLowerSlab));
					mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_STT;
					mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
					mark.m_dPosScaleMarkStt			= 1./(dCountCycle+nSize) * (nCycle+r+1)+0.2;
					mark.m_dPosScale				= 1./(dCountCycle+nSize) * (nCycle+r+1)+0.2;
					mark.m_dPosScaleSpace			= 0.5 - (1./dCountCycle)/2;

					CDPointArray xyArr;
					mark.GetXyCountMark(xyArr);

					// 사보강 배치의 사방향 철근
					if(pBri->m_nTypeSideRebarMain_UpperSlabSabogang == 1 && xyArr.GetSize() > 1)
					{
						CDPoint xyOrg1	= xyArr.GetAt(0);
						CDPoint xyOrg2	= xyArr.GetAt(1);
						CDPoint xyDir	= (xyOrg1-xyOrg2).Unit();
						long nSize		= xyArr.GetSize();
						long i = 0; for(i = 0; i < tvArrAngRight.GetSize(); i++)
						{
							if(nSize <= i) break;
							CDPoint xyMark		= xyArr.GetAt(i);
							CDPoint xyMarkDim	= tvArrAngRight.GetAt(i).m_v1;
							double dDist	= pLineR->GetDisMatchVerticalByXy(xyMarkDim);
							double sta		= 0;
							pLineR->GetStationMatchByXyAndAngle(xyMark, xyDir, sta, dDist);
							xyMark	= pLineR->GetXyLineDis(sta, dDist);
							xyArr.SetAt(i, xyMark);
						}
					}

					ApplyCuttingArea_Plane(&Dom, xyArr, bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, 0, FALSE, FALSE, NULL, FALSE);
					mark.m_xyArrCountMark	= xyArr;

					if(xyArr.GetSize() > 1)
					{
						CLineInfo *pLine	= pBri->GetLineBase();
						if(pLine)
						{
							long nSize		= xyArr.GetSize();
							CDPoint xyMark	= mark.GetXyMark(FALSE);
							CDPoint vAng	= (xyArr.GetAt(nSize-2)-xyArr.GetAt(nSize-1)).Unit();
							CDPoint vAngJ	= pBri->GetAngleJijum(0);
							double dSta		= xyMark.x;
							pLine->GetStationMatchByXyAndAngle(xyMark, vAng, dSta, 0);
							double dWidth	= pBri->GetWidthSlabAct(dSta, vAngJ, 1);
							CDPoint xyMatch	= pLine->GetXyLineByLengthAndAngle(dSta, dWidth, vAngJ);
							mark.m_dAddHeight	= ~(xyMatch-xyMark)+Dom.GetTextHeight();
						}
						mark.m_dCtc = pBri->GetCtcRebarMain(TRUE);
						mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;

						mark.MarkRebar(FALSE);	
					}
				}
			}		
		}		
	}

	*pDomP << Dom;
}

void CDrawPlane::DrawRebarSupport_Plane_LowerSlab( CDomyun *pDomP, BOOL bUpper, long nDan, long nDrawLeft, BOOL bDimRebar, BOOL bMarkRebar )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	// 클로소이드 구간에서 배력근 간격이 모두 제각각인 경우가 발생할 수 있다. //////
	// 따라서 dDimLimit이하는 버려서 최대한 동일한 간격으로 맞춰준다.
/*	const double dDimLimit	= 1;	*/
	////////////////////////////////////////////////////////////////////////////////

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CHgBaseDrawStd baseDraw;
	CRebarPlacing rb;

	// 배력철근 좌표 구함.
	// 치수 기입을 위해서 피복 없이 좌표를 구함
	// 철근 그릴때는 피복 적용된 외곽선에 맞게 잘라냄
	CTwinVectorArray tvArr, tvArrAngStt, tvArrAngEnd, tvArrRAngStt, tvArrRAngEnd;
	pBri->GetTvRebarSupport_Plane_Slab_Std(FALSE, tvArr, tvArrAngStt, tvArrAngEnd, tvArrRAngStt, tvArrRAngEnd, bUpper, nDan, nDrawLeft);

	// nDrawLeft 가 -1, 0이면 진행방향 좌측에, 1이면 우측에 치수 기입
	if(bDimRebar)
	{
		const double dDimLimit	= 1;	

		*pDomP << Dom;

		long nDimDan	= nDan+1;
		// 사방향 배력철근이 있다면 치수 단을 하나 더 띄운다.
		if(tvArrAngEnd.GetSize() > 0 || tvArrAngStt.GetSize() > 0)
			nDimDan	+= 3;

		BOOL bBaseLine = m_nPosBaseDimSupportRebar == 1 ? TRUE : FALSE;
		BOOL bLeft		= (nDrawLeft == -1 || nDrawLeft == 0);
		CLineInfoApp* pLineApp	= static_cast<CLineInfoApp*>(pBri->GetLineLeftRight(bLeft));// ->GetLineBase();

		double dWidth	= bLeft ? pBri->m_dWidthSlabLeft : pBri->m_dWidthSlabRight;
		double dDist	= 0;//pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, CDPoint(0, 1), bLeft ? -1:1);

		dDist	= dWidth;
		if(pBri->m_bIsExp)
			dDist += bLeft ? -max(pBri->m_dExpSttH[0], pBri->m_dExpEndH[0]) : max(pBri->m_dExpSttH[1], pBri->m_dExpEndH[1]);

		if(bLeft)
			dDist -= Dom.Always(Dom.GetDimExo()+Dom.GetDimLevelDis()*(nDimDan+1));
		else
			dDist += Dom.Always(Dom.GetDimExo()+Dom.GetDimLevelDis()*(nDimDan+1));
		//if(bLeft) dDist	= -dDist;


		CString sDir	= bLeft ? _T("TOP") : _T("BOTTOM");

		CDimDomyun dimDom(pDomP, pBri->m_pLineApp);
		m_pStd->m_pARoadOptionStd->SetEnvType(&dimDom, HCAD_DIML);
		dimDom.SetDirection(sDir);
		long nL	= bLeft ? pBri->m_nLineLeft : pBri->m_nLineRight;// >m_nLineBase;
		if(bBaseLine)
			nL = pBri->m_nLineBase;

		// 수직보강철근 치수(사보강포함)  ///////////////////////////////////////////////////////////
		// dim == 0 : 수직보강철근 치수
		// dim == 1 : 시점측 사보강 치수
		// dim == 2 : 종점측 사보강 치수
		CLineInfo *pLine	= pBri->GetLineBase();

		// 철근들어가는 평면 좌표들 ////////
		CTwinVectorArray tvArrPlane;
		CTwinVectorArray tvArrPlane1Dan;
		CTwinVectorArray tvArrLeft1Dan[2];
		pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane1Dan, 0, FALSE);
		CTwinVector tv;
		long i = 0; for(i = 0; i < tvArrPlane1Dan.GetSize(); i++)
		{
			tv	= tvArrPlane1Dan.GetAt(i);
			if(tv.m_sInfo.Find("좌측", 0) != -1) tvArrLeft1Dan[iLEFT].Add(tv);
			else if(tv.m_sInfo.Find("우측", 0) != -1) tvArrLeft1Dan[iRIGHT].Add(tv);
		}


		CTwinVector tvJiganStt, tvJiganEnd;
		pBri->GetTvSttEndOfSlab(tvJiganStt, tvJiganEnd, FALSE);

		GetSwap(tvJiganStt.m_v1, tvJiganStt.m_v2);
		GetSwap(tvJiganEnd.m_v1, tvJiganEnd.m_v2);
		//////////////////////////////////////

		double dStaStt	= pBri->GetStationAtSlabEnd(TRUE, bBaseLine ?  0 :(bLeft ? -1 : 1), 0);
		double dStaEnd	= pBri->GetStationAtSlabEnd(FALSE, bBaseLine ?  0 :(bLeft ? -1 : 1), 0);
		CDPoint vAngStt	= pBri->GetAngleByTarget(BRIDGE_OFF_STT_SLAB, 0);
		CDPoint vAngEnd	= pBri->GetAngleByTarget(BRIDGE_OFF_END_SLAB, 0);
		double dCover	= pBri->GetCoverSide_UpperSlab(0);
//		double dSta		= 0;
		CDoubleArray dArrSta, dArrStaOut;	// 수직보강철근과 중심선형과 만나는 sta 모음
		CDoubleArray dArrDist, dArrDistText;
		CTwinVectorArray *pTvArr	= NULL;
		tvJiganStt.m_v1 += tvJiganStt.GetXyDir(FALSE) * dCover/vAngStt.y;
		tvJiganStt.m_v2 += tvJiganStt.GetXyDir(TRUE) * dCover/vAngStt.y;
		tvJiganEnd.m_v1 += tvJiganEnd.GetXyDir(FALSE) * dCover/vAngEnd.y;
		tvJiganEnd.m_v2 += tvJiganEnd.GetXyDir(TRUE) * dCover/vAngEnd.y;

		if(bBaseLine)
		{
			dArrSta.Add(dStaStt);
			dArrSta.Add(dStaEnd);
		}
		else
		{
			pLineApp->GetStationMatchVerticalByXy(bLeft ? tvJiganStt.m_v1 : tvJiganStt.m_v2, dStaStt);
			pLineApp->GetStationMatchVerticalByXy(bLeft ? tvJiganEnd.m_v1 : tvJiganEnd.m_v2, dStaEnd);
		}

		// 가각 없는 형태로 잘라내서 치수 기입함.(그렇지 않으면 치수가 지저분 하게 나옴)
		CTwinVectorArray tvArrPlane1DanNoExp;
		pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane1DanNoExp, 0);

		CTwinVectorArray tvArrDim;
		tvArrDim	= tvArr;
		rb.GetTvArrayByCrossTvArrayArea(tvArrDim, tvArrPlane1DanNoExp, TRUE);

		// 사보강배치 절반일 경우 변화구간직철근도 치수를 넣자.
		// 단, 슬래브 좌우측에 포함되어 있는 철근만 치수에 넣어야 함.
		if(pBri->m_nTypePlacingSupportRebar_UpperSlab == 3)
		{
			pTvArr	= &tvArrLeft1Dan[bLeft ? iLEFT : iRIGHT];
			CDPointArray xyArrMatch;
			long i = 0;
			CTwinVector *pTv	= NULL;
			for(i = 0; i < tvArrRAngStt.GetSize(); i++)
			{
				pTv	= tvArrRAngStt.GetPAt(i);
				rb.GetXyMatchTvArrAndLine(*pTvArr, pTv->m_v1, pTv->GetXyDir(), xyArrMatch);
				if(xyArrMatch.GetSize() > 0)
					tvArrDim.Add(pTv);
			}

			for(i = 0; i < tvArrRAngEnd.GetSize(); i++)
			{
				pTv	= tvArrRAngEnd.GetPAt(i);
				rb.GetXyMatchTvArrAndLine(*pTvArr, pTv->m_v1, pTv->GetXyDir(), xyArrMatch);
				if(xyArrMatch.GetSize() > 0)
					tvArrDim.Add(pTv);
			}
		}


		pTvArr	= &tvArrDim;
		long nSize	= pTvArr->GetSize();
		if(nSize > 0)
		{
			long i = 0; for(i = 0; i < nSize; i++)
			{
				double dSta		= 0;
				tv	= pTvArr->GetAt(i);
				if(bBaseLine)
				{
					// 중심선형 기준일때는 중심선형에서의 스테이션을 찾는다.
					if(pLine->GetStationMatchByXyAndAngle(tv.m_v1, tv.GetXyDir(), dSta))
						dArrSta.Add(dSta);
				}
				else
				{
					// 좌우측 일때는 좌우측 선형에서 직각인위치.
					if(pLineApp->GetStationMatchVerticalByXy(bLeft ? tv.m_v2 : tv.m_v1, dSta))
						dArrSta.Add(dSta);
				}
			}

			// 피복치수 추가.
			{
				double dSta1(0);	// 아크면 시작점이 반대일 수 있다.
				CDoubleArray dArrStaTmp;
				pTvArr	= &tvArrLeft1Dan[bLeft ? iLEFT : iRIGHT];
				long i = 0;
				
				for(i = 0; i < pTvArr->GetSize(); i++)
				{
					CTwinVector *pTv	= NULL;
					pTv	= pTvArr->GetPAt(i);
					if(bBaseLine)
						pLine->GetStationMatchByXyAndAngle(pTv->m_v1, tvJiganStt.GetXyDir(), dSta1);
					else
						pLineApp->GetStationMatchVerticalByXy(pTv->m_v1, dSta1);
					dArrStaTmp.Add(dSta1);

					if(i == pTvArr->GetSize()-1)
					{
						if(bBaseLine)
							pLine->GetStationMatchByXyAndAngle(pTv->m_v2, tvJiganEnd.GetXyDir(), dSta1);
						else
							pLineApp->GetStationMatchVerticalByXy(pTv->m_v2, dSta1);
						dArrStaTmp.Add(dSta1);
					}
				}

				dArrStaTmp.Sort(TRUE);

				if(dArrStaTmp.GetSize() > 1)
				{
					dArrSta.Add(dArrStaTmp.GetAt(0));
					dArrSta.Add(dArrStaTmp.GetAt(dArrStaTmp.GetSize()-1));
				}
			}

			// 구조물선 치수 추가
			{
				CTwinVectorArray tvArrLeft[2];
				CTwinVectorArray tvArrPlan;
				pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlan, -1);
				CTwinVector *pTv	= NULL;
				long i = 0; for(i = 0; i < tvArrPlan.GetSize(); i++)
				{
					pTv	= tvArrPlan.GetPAt(i);
					if(pTv->m_sInfo.Find("좌측", 0) != -1) tvArrLeft[iLEFT].Add(*pTv);
					else if(pTv->m_sInfo.Find("우측", 0) != -1) tvArrLeft[iRIGHT].Add(*pTv);
				}

				CTwinVectorArray *pTvArrOut	= &tvArrLeft[bLeft ? iLEFT : iRIGHT];

				if(pTvArrOut->GetSize() > 0)
				{
					double dSta1(0), dSta2(0);	// 아크면 시작점이 반대일 수 있다.
					pTv		= pTvArrOut->GetPAt(0);
					// (ARCBRIDGE-3516) 선형중심 기준 치수일때는 외곽선 치수를 따로 표기한다.
					if(bBaseLine)
					{
						if(pLine->GetStationMatchVerticalByXy(pTv->m_v1, dSta1) && pLine->GetStationMatchVerticalByXy(pTv->m_v2, dSta2))
							dArrStaOut.Add(min(dSta1, dSta2));
					}
					else
					{
						if(pLineApp->GetStationMatchVerticalByXy(pTv->m_v1, dSta1) && pLineApp->GetStationMatchVerticalByXy(pTv->m_v2, dSta2))
							dArrSta.InsertAt(1, min(dSta1, dSta2));
					}

					pTv	= pTvArrOut->GetPAt(pTvArrOut->GetSize()-1);
					if(bBaseLine)
					{
						if(pLine->GetStationMatchVerticalByXy(pTv->m_v1, dSta1) && pLine->GetStationMatchVerticalByXy(pTv->m_v2, dSta2))
							dArrStaOut.Add(max(dSta1, dSta2));
					}
					else
					{
						if(pLineApp->GetStationMatchVerticalByXy(pTv->m_v1, dSta1) && pLineApp->GetStationMatchVerticalByXy(pTv->m_v2, dSta2))
							dArrSta.Add(max(dSta1, dSta2));
					}
				}
			}


			dArrSta.Sort(TRUE);
			nSize	= dArrSta.GetSize();
			for(i = 1; i < nSize; i++)
			{
				dArrDist.Add(dArrSta[i]-dArrSta[i-1]);
				double dLenText = bBaseLine? pLine->GetLengthByDis(dArrSta[i-1], dArrSta[i], 0) : pLineApp->GetLengthByDis(dArrSta[i-1], dArrSta[i], 0);
				dArrDistText.Add(dLenText);
			}

			// 치수기입 정보 가져옴
			CDoubleArray dArrDim;
			CStringArray sArrText;
			CStringArray sArrUnderText;
			m_pStd->GetDimInfoAtDistArray(dArrDim, sArrText, sArrUnderText, dArrDist, "", FALSE, TRUE, TRUE, &Dom, dDimLimit, TRUE, TRUE, dArrDistText);

			// 치수 기입
			double dStaCur	= dArrSta.GetAt(0);
			dimDom.DimBaseCurvePoint(dStaCur, dDist, nDimDan);

			CString str	= _T("");
			CString strUnder	= _T("");
			nSize		= dArrDim.GetSize();
			BOOL bUnderTextPrev	= FALSE;
			for(i = 0; i < nSize; i++)
			{
				str			= sArrText.GetAt(i);
				strUnder	= sArrUnderText.GetAt(i);
				double dLen		= dArrDim.GetAt(i);

				if(i == 0 || i == nSize-1)	// 시작과 끝은 어짜피 피복 치수이므로 밖으로 뺀다.
				{
					if(i == 0)
					{
						dimDom.DimCurveMoveTo(dStaCur, dDist, nL, FALSE, str);
						dimDom.DimCurveLineTo(dLen, dDist, nL, "", "");
					}
					else
					{
						if(!pBri->IsIncludeOnlyLineInBridge())
						{
							str += bUpper ? "(상면), (도로중심치수임)" : "(하면),(도로중심치수임)";
						}
						else
						{
							str += bUpper ? "(상면)" : "(하면)";
						}
						dimDom.DimCurveMoveTo(dStaCur, dDist, nL, FALSE);
						dimDom.DimCurveLineTo(dLen, dDist, nL, str, strUnder, FALSE, FALSE, TRUE);
					}
				}
				else
				{
					dimDom.DimCurveMoveTo(dStaCur, dDist, nL, FALSE);
					if((dLen < dimDom.GetTextWidth(str)) && !bUnderTextPrev)
					{
						dimDom.DimCurveLineTo(dLen, dDist, nL, str, strUnder);
						bUnderTextPrev	= TRUE;
					}
					else
					{
						dimDom.DimCurveLineTo(dLen, dDist, nL, str, strUnder);
						bUnderTextPrev	= FALSE;
					}
				}

				dStaCur += dLen;
			}

			nDimDan++;

			// 전체 치수
			//dDist	= pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, CDPoint(0, 1), bLeft ? -1:1);
			dDist	= dWidth;
			if(pBri->m_bIsExp)
				dDist += bLeft ? -max(pBri->m_dExpSttH[0], pBri->m_dExpEndH[0]) : max(pBri->m_dExpSttH[1], pBri->m_dExpEndH[1]);
			if(bLeft)
				dDist -= Dom.Always(Dom.GetDimExo()+Dom.GetDimLevelDis()*(nDimDan+1));
			else
				dDist += Dom.Always(Dom.GetDimExo()+Dom.GetDimLevelDis()*(nDimDan+1));
			//if(bLeft) dDist	= -dDist;

			dStaStt	= dArrSta.GetAt(0);
			dStaEnd	= dArrSta.GetAt(dArrSta.GetSize()-1);
			dimDom.DimBaseCurvePoint(dStaStt, dDist, nDimDan);
			dimDom.DimCurveMoveTo(dStaStt, dDist, nL, FALSE);
			dimDom.DimCurveLineTo((dStaEnd-dStaStt), dDist, nL, COMMA(pLineApp->GetLengthByDis(dStaStt, dStaEnd, dWidth)), "");

			if(bBaseLine)
			{
				// 선형중심 치수일때는 구조물 치수를 하나 더 추가해준다.
				if(dArrStaOut.GetSize() > 1)
				{
					if(bLeft)
						dDist -= Dom.Always(Dom.GetDimLevelDis());
					else
						dDist += Dom.Always(Dom.GetDimLevelDis());

					dStaStt	= dArrStaOut.GetAt(0);
					dStaEnd	= dArrStaOut.GetAt(dArrStaOut.GetSize()-1);

					dimDom.DimBaseCurvePoint(dStaStt, dDist, nDimDan);
					dimDom.DimCurveMoveTo(dStaStt, dDist, nL, FALSE);
					dimDom.DimCurveLineTo((dStaEnd-dStaStt), dDist, nL, COMMA(pLine->GetLengthByDis(dStaStt, dStaEnd, dWidth)), "");
				}
			}

			*pDomP << dimDom;
		}
		///////////////////////////////////////////////////////////////////////////////////


		// 사보강 배치 치수 기입(시점측 종점측) //////////////////////////////////////////
		for(long dim = 0; dim < 2; dim++)
		{
			pTvArr	= NULL;
			switch(dim)
			{
			case 0: pTvArr	= &tvArrAngStt; break;
			case 1: pTvArr	= &tvArrAngEnd; break;
			default: break;
			}
			if(!pTvArr) continue;
			long nSize	= pTvArr->GetSize();
			if(nSize == 0) continue;

			dArrDist.RemoveAll();
			dArrSta.RemoveAll();

			// 거리구함(각 철근간의 선형거리로 구함)
			double dSta1	= 0;
			double dSta2	= 0;
//			double dLen		= 0;
			CDPoint xy1		= CDPoint(0, 0);
			CDPoint xy2		= CDPoint(0, 0);

			long i = 0; for(i = 0; i < nSize-1; i++)
			{
				xy1		= bLeft ? pTvArr->GetAt(i).m_v2 : pTvArr->GetAt(i).m_v1;
				xy2		= bLeft ? pTvArr->GetAt(i+1).m_v2 : pTvArr->GetAt(i+1).m_v1;
				dDist	= pLine->GetDisMatchVerticalByXy(xy1);
				pLine->GetStationMatchVerticalByXy(xy1, dSta1);
				pLine->GetStationMatchVerticalByXy(xy2, dSta2);

				double dLen	= pLine->GetLengthByDis(dSta1, dSta2, dDist);
				dArrDist.Add(dLen);
			}

			// 치수기입 정보 가져옴
			CDoubleArray dArrDim;
			CStringArray sArrText;
			CStringArray sArrUnderText;
			CDoubleArray dArrTmp;
			m_pStd->GetDimInfoAtDistArray(dArrDim, sArrText, sArrUnderText, dArrDist, "", FALSE, TRUE, TRUE, &Dom, dDimLimit, TRUE, TRUE, dArrTmp);

			// 치수 기입
			tv		= pTvArr->GetAt(0);
			CDPoint xyStt	= bLeft ? tv.m_v2 : tv.m_v1; 
			pLine->GetStationMatchVerticalByXy(xyStt, dStaStt);

			nDimDan		= 1;
			dDist	= pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, CDPoint(0, 1), bLeft ? -1:1);
			if(pBri->m_bIsExp)
				dDist += bLeft ? max(pBri->m_dExpSttH[0], pBri->m_dExpEndH[0]) : max(pBri->m_dExpSttH[1], pBri->m_dExpEndH[1]);
			dDist += Dom.Always(Dom.GetDimExo()+Dom.GetDimLevelDis()*(nDimDan+1));
			if(bLeft) dDist	= -dDist;

			// 현재 선형 치수 기입시 obq가 적용되진 않지만, 나중에 적용될 것을 고려해서 obq를 설정해 놓음
			//double dObq	= 90-pBri->GetAngleJijum(0).GetAngleDegree();
			//dimDom.SetDimObq(dObq);	// 엔진을 고치려니 너무 오래 걸림.. 그냥 두자

			dimDom.DimBaseCurvePoint(dStaStt, dDist, nDimDan);
			double dStaCur	= dStaStt;

			BOOL bUnderTextPrev	= TRUE;
			for(long j = 0; j < dArrDim.GetSize(); j++)
			{
				double dLen	= dArrDim.GetAt(j);
				CString str	= sArrText.GetAt(j);

				dimDom.DimCurveMoveTo(dStaCur, dDist, 0);
				if(dimDom.GetTextWidth(str) > dLen && !bUnderTextPrev)
				{
					dimDom.DimCurveLineTo(dLen, dDist, 0, "", str);
					bUnderTextPrev	= TRUE;
				}
				else
				{
					dimDom.DimCurveLineTo(dLen, dDist, 0, str, ""); 
					bUnderTextPrev	= FALSE;
				}

				dStaCur += dLen;
			}

			*pDomP << dimDom;
		}
		///////////////////////////////////////////////////////////////////////////////////

	}

	// 절단원 적용시켜서 그림.
	ApplyCuttingArea_Plane(&Dom, tvArr, bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, 0, FALSE, FALSE, NULL, FALSE);
	ApplyCuttingArea_Plane(&Dom, tvArrAngStt, bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, 0, FALSE, FALSE, NULL, FALSE);
	ApplyCuttingArea_Plane(&Dom, tvArrAngEnd, bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, 0, FALSE, FALSE, NULL, FALSE);
	ApplyCuttingArea_Plane(&Dom, tvArrRAngStt, bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, 0, FALSE, FALSE, NULL, FALSE);
	ApplyCuttingArea_Plane(&Dom, tvArrRAngEnd, bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, 0, FALSE, FALSE, NULL, FALSE);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	baseDraw.DrawTvArray(&Dom, tvArr);
	baseDraw.DrawTvArray(&Dom, tvArrAngStt);
	baseDraw.DrawTvArray(&Dom, tvArrAngEnd);
	baseDraw.DrawTvArray(&Dom, tvArrRAngStt);
	baseDraw.DrawTvArray(&Dom, tvArrRAngEnd);

	// 배력 철근 마크
	CString sDescription	= _T("");
	if(bUpper)
		sDescription.Format("하부슬래브 상면 %d단 배력철근2", nDan+1);
	else
		sDescription.Format("하부슬래브 하면 %d단 배력철근", nDan+1);
	CString sMark	= pBri->GetMarkRebarSupport(0, FALSE, FALSE, FALSE, sDescription);
	if(bMarkRebar && tvArr.GetSize() > 0)// && m_pStd->IsMarkRebar(sMark) && tvArr.GetSize() > 0)
	{
		CTwinVectorArray tvArrExpSttLAndR, tvArrExpSttLOrR;
		CTwinVectorArray tvArrExpEndLAndR, tvArrExpEndLOrR;
		pBri->DivideRebarSupport_Plane_UpperSlabExpSection(tvArr, tvArrExpSttLAndR, tvArrExpSttLOrR, tvArrExpEndLAndR, tvArrExpEndLOrR);

		// 수직철근에 대해서 ...
		// 수직철근은 모두 마크후에 종점쪽으로 뻗어서 마크한다.
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		if(pBri->m_nTypePlacingMainRebar_UpperSlab == 0)
		{
			CLineInfoApp *pLine	= static_cast<CLineInfoApp*>(pBri->GetLineBase());
			if(!pLine) return;


			// ang == 0 : 기본철근(사보강시 직방향철근)
			// ang == 1 : 시점방향 철근(사방향)
			// ang == 2 : 종점방향 철근(사방향)
			// ang == 3 : 시점방향 철근(직방향변화구간)
			// ang == 4 : 종점방향 철근(직방향변화구간)
			// ang == 5 : 기본철근(시점좌우측가각부)
			// ang == 6 : 기본철근(시점좌나우측가각부)
			// ang == 7 : 기본철근(종점좌우측가각부)
			// ang == 8 : 기본철근(종점좌나우측가각부)
			for(long ang = 0; ang < 9 ; ang++)
			{

				// 가각지원 아니면 가각에 대해서 마크하지 않음
				if(!pBri->m_bIsExp && ang > 4) break;

				CDPointArray xyArrMark;	// countmark할 위치들.

				// 배력근을 마크해야 되는 dist, sta등의 정보를 얻음.
				CTwinVectorArray	tvArrSup;
				CDPoint xyDir	= CDPoint(0, 1);
				double dSta		= pBri->GetStationOnJijum(0);
				double dDistMid	= pBri->GetDistMidOfSlab(dSta);
				double dWid		= pBri->GetWidthSlabAct(dSta, xyDir, 0)/2;
				double dWidLeft	= pBri->GetWidthSlabAct(dSta, xyDir, -1);
				double dWidRight	= pBri->GetWidthSlabAct(dSta, xyDir, 1);
				double dDist	= bUpper ? -dWid/2 : dWid/2;

				switch(ang)
				{
				case 0: 
					dDist		= bUpper ? -dWid/2 : dWid/2;
					tvArrSup	= tvArr;
					if(bUpper) sDescription.Format("하부슬래브 상면 %d단 배력철근2", nDan+1);
					else sDescription.Format("하부슬래브 하면 %d단 배력철근", nDan+1);
					break;
				case 1:
					dDist		= bUpper ? -dWid/5 : dWid/5;
					tvArrSup	= tvArrAngStt;
					if(bUpper) sDescription.Format("하부슬래브 상면 %d단 배력철근2_2", nDan+1);
					else sDescription.Format("하부슬래브 하면 %d단 배력철근2", nDan+1);
					break;
				case 2: 
					dDist		= bUpper ? -dWid/5 : dWid/5;
					tvArrSup	= tvArrAngEnd;
					if(bUpper) sDescription.Format("하부슬래브 상면 %d단 배력철근2_3", nDan+1);
					else sDescription.Format("하부슬래브 하면 %d단 배력철근3", nDan+1);
					break;
				case 3: 
					dDist		= bUpper ? -dWid/5*4 : dWid/5*4;
					tvArrSup	= tvArrRAngStt;
					if(bUpper) sDescription.Format("하부슬래브 상면 %d단 배력철근2_4", nDan+1);
					else sDescription.Format("하부슬래브 하면 %d단 배력철근4", nDan+1);
					break;
				case 4: 
					dDist		= bUpper ? -dWid/5*4 : dWid/5*4;
					tvArrSup	= tvArrRAngEnd;
					if(bUpper) sDescription.Format("하부슬래브 상면 %d단 배력철근2_5", nDan+1);
					else sDescription.Format("하부슬래브 하면 %d단 배력철근5", nDan+1);
					break;
				case 5:
					dDist		= bUpper ? -dWid/5*2 : dWid/5*2;
					tvArrSup	= tvArrExpSttLAndR;
					if(bUpper) sDescription.Format("하부슬래브 상면 %d단 배력철근2_6", nDan+1);
					else sDescription.Format("하부슬래브 하면 %d단 배력철근6", nDan+1);
					break;
				case 6:
					dDist		= bUpper ? -dWid/5*3 : dWid/5*3;
					tvArrSup	= tvArrExpSttLOrR;
					if(bUpper) sDescription.Format("하부슬래브 상면 %d단 배력철근2_7", nDan+1);
					else sDescription.Format("하부슬래브 하면 %d단 배력철근7", nDan+1);
					break;
				case 7:
					dDist		= bUpper ? -dWid/5*2 : dWid/5*2;
					tvArrSup	= tvArrExpEndLAndR;
					if(bUpper) sDescription.Format("하부슬래브 상면 %d단 배력철근2_8", nDan+1);
					else sDescription.Format("하부슬래브 하면 %d단 배력철근8", nDan+1);
					break;
				case 8:
					dDist		= bUpper ? -dWid/5*3 : dWid/5*3;
					tvArrSup	= tvArrExpEndLOrR;
					if(bUpper) sDescription.Format("하부슬래브 상면 %d단 배력철근2_9", nDan+1);
					else sDescription.Format("하부슬래브 하면 %d단 배력철근9", nDan+1);
					break;
				}

				dDist += (pBri->m_dWidthSlabRight + pBri->m_dWidthSlabLeft) / 4;


				if(tvArrSup.GetSize() == 0) continue;

				// 철근의 가운데
				CTwinVector tvSup	= tvArrSup.GetAt(0);

				sMark	= pBri->GetMarkRebarSupport(0, FALSE, FALSE, FALSE, sDescription);
				dDist	= bUpper ? (dDistMid - dWidLeft/4) : (dDistMid + dWidRight/4);
				
				if(pBri->m_bIsExp)
					dDist += (ang * dWid/36);
				else
					dDist += (ang * dWid/18);

				// 2,3단은 절단원 사이에 마크해야 됨.
				if(nDan != 0)
				{
					CTwinVectorArray tvArrCuttingArea;
					pBri->GetTvCuttingArea_Plane_UpperSlab(tvArrCuttingArea, bUpper, nDan);
					if(tvArrCuttingArea.GetSize() > 1)
					{
						CTwinVector tvCutting	= tvArrCuttingArea[1];
						dDist	= pLine->GetDisMatchVerticalByXy(tvCutting.m_v1+tvCutting.GetLength()/3*tvCutting.GetXyDir());
					}
				}

				if(!pLine->GetStationMatchVerticalByXy(bUpper ? tvArrSup[0].m_v2 : tvArrSup[0].m_v1, dSta)) return;

				// 1cycle과 2cycle 주철근을 구함. /////////////////////////
				// 정확한 dDist를 구하기 위해서(주철근과 겹치지 않도록 하기 위해.)
				// 배력근 마킹은 dSta에서 하지만, 주철근은 2cycle을 반드시 고려 해야 되므로
				// 상면하면에 따라서 반드시 2cycle이 있을 만한 장소의 주철근 간격을 가져온다.
				// 단, 2,3단 하부의 경우 어짜피 선이 트랙을 빠져 나오면서 1단주철근을 거쳐 와야 되므로 1단 1,2cycle
				// 철근도 고려해야 한다.
				CDoubleArray dArrDistMain, dArrDistHunch;
				CDoubleArray dArrDistMain2, dArrDistHunch2;
				CDoubleArray dArrTmp;

				pBri->GetDistRebarMain_CrossSection_Slab(FALSE, dArrDistMain, dArrDistHunch, dArrDistMain2, dArrDistHunch2, dArrTmp, dArrTmp, dArrTmp, dArrTmp, dSta, bUpper, nDan, TRUE, FALSE);
				long i = 0; for(i = 0; i < dArrDistMain2.GetSize(); i++)
					dArrDistMain.Add(dArrDistMain2.GetAt(i));

				// 2,3단 하부의 경우 1단주철근에 대해서도 고려함. //////////
				if(nDan > 0 && !bUpper)
				{
					CDoubleArray dArrDistMain0, dArrDistHunch0;
					CDoubleArray dArrDistMain02, dArrDistHunch02;
					pBri->GetDistRebarMain_CrossSection_Slab(FALSE, dArrDistMain0, dArrDistHunch0, dArrDistMain02, dArrDistHunch02, dArrTmp, dArrTmp, dArrTmp, dArrTmp, dSta, bUpper, 0, TRUE, FALSE);

					long i = 0; for(i = 0; i < dArrDistMain0.GetSize(); i++)
						dArrDistMain.Add(dArrDistMain0[i]);
					for(i = 0; i < dArrDistMain02.GetSize(); i++)
						dArrDistMain.Add(dArrDistMain02[i]);
				}
				/////////////////////////////////////////////////////////////

				// 마크할 위치 찾음
				GetXyCountMarkSupport_Plane_Slab(xyArrMark, tvArrSup, bUpper, dDist, dArrDistMain);

				// 마크할 위치에 대해서 절단원 적용
				ApplyCuttingArea_Plane(&Dom, xyArrMark, bUpper, nDan, FALSE, FALSE, FALSE, FALSE, FALSE, 0, FALSE, FALSE, NULL, FALSE);

				// count mark하고 선을 그음.
				CDPoint xyRotate	= pBri->GetXyDirRotateDomByBridgeSttEndXy();
				CDPoint xyDirLine	= pLine->GetAngleAzimuth(dSta);
				CConcSymbol sym(m_pStd->m_pARoadOptionStd);
				for(i = 0; i < xyArrMark.GetSize(); i++)
					sym.RebarCountMarkOne(&Dom, xyArrMark.GetAt(i), xyDirLine);

				long nSize	= xyArrMark.GetSize();
				if(nSize > 0)
				{
					double dStaCur	= dSta;
					CDPoint xyStt	= xyArrMark.GetAt(0);
					CDPoint xyEnd	= xyArrMark.GetAt(nSize-1);
					// 1단일 경우 종점측으로 뻗고, 2,3단은 시점측으로 뻗는다.
					// 단, 1단이면서 시점방향 철근이면 시점으로 뻗는다.
					BOOL bExtendToStt	= nDan == 0 && (ang == 1 || ang == 3 || ang == 5 || ang == 6);
					if(!bExtendToStt)
					{
						dStaCur	= pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, FALSE);//pBri->m_dStationBridgeStt+pBri->m_dLengthBridge+pBri->m_dWE+pBri->m_dBWE;
						xyDir	= pBri->GetAngleByTarget(BRIDGE_OFF_END_SLAB, 0);// >GetAngleJijum(pBri->m_nQtyJigan);
					}
					else
					{
						dStaCur	= pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, FALSE);// pBri->m_dStationBridgeStt-pBri->m_dWS;
						GetSwap(xyStt, xyEnd);
						xyDir	= pBri->GetAngleByTarget(BRIDGE_OFF_STT_SLAB, 0);//pBri->GetAngleJijum(0);
					}

					// 사방향 배치는 마크위치 구할때 부터 사각 적용하므로 여기선 하지 않음
					if(pBri->m_nTypePlacingSupportRebar_UpperSlab == 2)
						xyDir	= pBri->GetAngleJijum(0);

					dDist	= pLine->GetDisMatchVerticalByXy(xyStt);
					xyEnd	= pLine->GetXyLineByLengthAndAngle(dStaCur, dDist/xyDir.y, xyDir);


					double dStaStt(0), dStaEnd(0);
					pLine->GetStationMatchVerticalByXy(xyStt, dStaStt);
					pLine->GetStationMatchVerticalByXy(xyEnd, dStaEnd);
					double dDist	= pLine->GetDisMatchVerticalByXy(xyStt);

					pLine->DrawLS(&Dom, dStaStt, dStaEnd, dDist);

					xyEnd	= pLine->GetXyLineDis(dStaEnd, dDist);

					// 마킹함.
					*pDomP << Dom;

					long upper	= bUpper ? iUPPER : iLOWER;
					CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_SupportRebar_LowerSlab[upper][nDan];
					CString sDia	= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(ePartLowerSlab));
					// sym.RebarMarkLineSide(&Dom, xyEnd, sMark, sDia, bExtendToStt, pBri->m_bExistProtectionRiseFooting? pBri->m_dPRF_W : 0, FALSE);

					CRebar *pRb = pBri->GetRebarSupportByDescription(sDescription);
					SRebarMarkInputData mark_input_data;
					SRebarMarkviewData mark_view_data;
					mark_input_data.mark = sMark;
					mark_input_data.dia = sDia;
					mark_input_data.position = xyEnd;
					mark_input_data.ctc = pBri->GetCTCRebarSupport_Slab(FALSE, bUpper, nDan, TRUE);
					mark_input_data.type_rebar = pRb? pRb->m_nTypeRebar : -1;		// 18을 고정으로 써도 된다.
					mark_input_data.left_dir = bExtendToStt;
					mark_input_data.height = pBri->m_bExistProtectionRiseFooting? pBri->m_dPRF_W : 0;

					mark_view_data.count_mark = FALSE;
					mark_view_data.line_rebar = TRUE;
					mark_view_data.mark_pos = eMarkSide;

					sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

					CDPoint xyDirLine	= pLine->GetAngleAzimuth(dStaCur);
					Dom.Rotate(xyEnd, xyDirLine);
					*pDomP << Dom;
				}

			}
		}
		else if(pBri->m_nTypePlacingMainRebar_UpperSlab == 1 || pBri->m_nTypePlacingMainRebar_UpperSlab == 2)
		{
			// (34554) 8번째 인자인 Index를 0을주면 주철근 위치를 못찾고 마킹이 날라가는 경우가 있다. -1을줘서 전체 주철근을 찾자.
			CTwinVectorArray tvArrMain1, tvArrMain2, tvArrMain, tvArrAngleLeft, tvArrAngleRight;	// 주철근 좌표
			pBri->GetTvRebarMain_Plane_LowerSlab_TypeNormal_Std(tvArrMain1, tvArrAngleLeft, tvArrAngleRight, bUpper, 0, 0, 0, -1);
			pBri->GetTvRebarMain_Plane_LowerSlab_TypeNormal_Std(tvArrMain2, tvArrAngleLeft, tvArrAngleRight, bUpper, 1, 0, 0, -1);

			if(tvArrMain2.GetSize() > 0)
			{
				long i = 0; for(i = 0; i < tvArrMain1.GetSize(); i++)
				{
					tvArrMain.Add(tvArrMain1.GetAt(i));
					if(i < tvArrMain2.GetSize())
						tvArrMain.Add(tvArrMain2.GetAt(i));
				}
			}
			else
				tvArrMain	= tvArrMain1;


			long upper	= bUpper ? iUPPER : iLOWER;
			CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_SupportRebar_LowerSlab[upper][nDan];

			// ang == 0 : 기본철근(사보강시 직방향철근)
			// ang == 1 : 시점방향 철근(사방향)
			// ang == 2 : 종점방향 철근(사방향)
			// ang == 3 : 시점방향 철근(직방향변화구간)
			// ang == 4 : 종점방향 철근(직방향변화구간)
			double dPosScale	= 0.5;
			for(long ang = 0; ang < 5; ang++)
			{
				CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
				switch(ang)
				{
				case 0:
					if(bUpper) sDescription.Format("하부슬래브 상면 %d단 배력철근2", nDan+1);
					else sDescription.Format("하부슬래브 하면 %d단 배력철근", nDan+1);
					mark.SetRebar(tvArr, tvArrMain);
					dPosScale	= 0.5;
					break;
				case 1:
					if(bUpper) sDescription.Format("하부슬래브 상면 %d단 배력철근2_2", nDan+1);
					else sDescription.Format("하부슬래브 하면 %d단 배력철근2", nDan+1);
					mark.SetRebar(tvArrAngStt, tvArrMain);
					dPosScale	= 0.2;
					break;
				case 2:
					if(bUpper) sDescription.Format("하부슬래브 상면 %d단 배력철근2_3", nDan+1);
					else sDescription.Format("하부슬래브 하면 %d단 배력철근3", nDan+1);
					mark.SetRebar(tvArrAngStt, tvArrMain);
					dPosScale	= 0.8;
					break;
				}
				sMark	= pBri->GetMarkRebarSupport(0, FALSE, FALSE, FALSE, sDescription);
				CRebar *pRb = pBri->GetRebarSupportByDescription(sDescription);

				// 설정
				mark.m_sMark					= sMark;
				mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(ePartLowerSlab));
				mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_END;
				mark.m_dPosScale				= dPosScale;
				if(tvArrMain.GetSize() > 0)
				{
					CDPoint xyMark				= mark.GetXyMark();
					CDPoint xyMatch				= xyMark;

					CTwinVectorArray tvArrPlane, tvArrEnd;
					CTwinVector tv;
					pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1);
					long i = 0; for(i = 0; i < tvArrPlane.GetSize(); i++)
					{
						tv	= tvArrPlane.GetAt(i);
						if(tv.m_sInfo == "종점") 
							tvArrEnd.Add(tv);
					}

					for(i = 0; i < tvArrEnd.GetSize(); i++)
					{
						tv	= tvArrEnd.GetAt(i);
						if(GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, xyMark, tvArrMain.GetAt(0).GetXyDir(), xyMatch))
							break;
					}
					// m_dPRF_W를 더하는 이유? 
					// 부상방지저판있는 경우 부상방지저판도 감안을 해줘야 하므로...
					mark.m_dAddHeight			= ~(xyMark - xyMatch) + pBri->m_dPRF_W;
				}
				mark.m_dCtc = pBri->GetCTCRebarSupport_Slab(FALSE, bUpper, nDan, TRUE);
				mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;

				mark.MarkRebar();
			}
		}
	}

	*pDomP << Dom;
}

void CDrawPlane::DrawRebarMain_Plane_LowerSlabHunch( CDomyun *pDomP, long nCycle, long nJ, BOOL bUpper, long nLeftRebar, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft, long nDirMark/*=2*/, BOOL bApplyCutArea )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CLineInfo *pLine	= pBri->GetLineBase();

	CTwinVectorArray tvArr, tvArrAngLeft, tvArrAngRight;
	CTwinVectorArray tvArrLeftHalf, tvArrRightHalf;
	double dSta	= pBri->GetStationOnJijum(nJ, 0, pBri->IsVertDir());
	pBri->GetTvRebarMain_Plane_SlabHunch(tvArr, tvArrAngLeft, tvArrAngRight, nCycle, dSta, nLeftRebar, nDrawLeft, FALSE);

	if(bUpper == TRUE)
	{
		CDPoint xyDir	= pBri->GetAngleJijum(nJ, FALSE);
		double dLenL	= (pBri->m_dDH[0] + pBri->m_dCantilB[0]);
		double dLenR	= (pBri->m_dDH[1] + pBri->m_dCantilB[1]);

		CRebarPlacing rb;

		CTwinVectorArray tvArrPlane;
		pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1, TRUE);

		CTwinVectorArray tvArrLeft, tvArrRight;
		tvArrPlane.GetTvArrByInfo(_T("좌측"), tvArrLeft, FALSE, TRUE);
		tvArrPlane.GetTvArrByInfo(_T("우측"), tvArrRight, FALSE, TRUE);

		rb.GetOffsetTvArray(tvArrLeft, dLenL);
		rb.GetOffsetTvArray(tvArrRight, -dLenR);

		rb.TrimTvArrayByTvArray(tvArrAngRight, tvArrLeft, TRUE, TRUE);
		rb.TrimTvArrayByTvArray(tvArrAngRight, tvArrRight, FALSE, TRUE);

		rb.TrimTvArrayByTvArray(tvArrAngLeft, tvArrLeft, TRUE, TRUE);
		rb.TrimTvArrayByTvArray(tvArrAngLeft, tvArrRight, FALSE, TRUE);
	}

	if(tvArr.GetSize() == 0 && tvArrAngLeft.GetSize() == 0 && tvArrAngRight.GetSize() == 0) return;
	if(pBri->m_nTypePlacingMainRebar_UpperSlab == 2)
	{
		pBri->m_Util.DivideVertMainRebarBySabogang(tvArr, tvArrLeftHalf, tvArrRightHalf);
	}

	if(bDimRebar && !bApplyCutArea)
	{
		*pDomP << Dom;
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		BOOL bStt	= nJ == 0 ? TRUE : FALSE;

		// vert == 0 : 직방향 등 기본 철근
		// vert == 1 : 사보강 배치시 사방향 철근
	
		// 전체 치수를 넣을 시기를 결정함
		long nDimTotal	= 0;
		CTwinVector tvStt, tvEnd;
		pBri->GetTvSttEndOfSlab(tvStt, tvEnd);
		GetSwap(tvStt.m_v1, tvStt.m_v2);
		GetSwap(tvEnd.m_v1, tvEnd.m_v2);

		for(long vert = 0; vert < 3; vert++)
		{
			CDoubleArray dArrHunch;
			CDoubleArray dArrDim;
			CStringArray sArrText;
			CStringArray sArrUnderText;
			CString sDir	= bStt ? "TOP" : "BOTTOM";
			CDPoint xyStt	= CDPoint(0, 0);
			long nDimDan	= 1;
			
			// 치수기입 기준위치
			CDPoint vAng	= pBri->GetAngleJijum(nJ);
			double dSta		= pBri->GetStationByTarget(bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0, TRUE);
			CTwinVector tvJigan	= bStt ? tvStt : tvEnd;

			xyStt	= tvJigan.m_v2;

			// 가각 철근이 선형방향 배치면 치수를 포함시킨다.
			// 단, 주철근이 선형방향 배치이어야 한다
			if(pBri->m_nTypePlacingMainRebar_UpperSlab == 0)
			{
				long left = 0; for(left = 0; left < 2; left++)
				{
					BOOL bLeft	= left == iLEFT;
					if(pBri->IsGagakForLine(bStt, bLeft))
					{
						CTwinVectorArrayArray pTvArrTemp1, pTvArrTemp2, pTvArrTemp3, pTvArrTemp4;
						CTwinVectorArray tvArrCycleHunch[2];
						pBri->GetTvRebarMain_Plane_Slab_Exp(FALSE, &pTvArrTemp1, &pTvArrTemp2, &pTvArrTemp3, &pTvArrTemp4, bStt, bLeft, 0, TRUE, tvArrCycleHunch[0], tvArrCycleHunch[1]);

						long i = 0; for(i = 0; i < tvArrCycleHunch[0].GetSize(); i++)
						{
							tvArr.Add(tvArrCycleHunch[0].GetAt(i));
						}
					}
				}
			}

			// 가각 철근 치수가 주철근치수에 겹치면 빼버림
			CDPointArray xyArr;
			CDPoint xyMatch(0, 0);
			CDPointArray xyArrMatch;
			CTwinVector tv;
			CDPoint vAngDim	= pBri->GetAngleByTarget(bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0); //pBri->GetAngleJijum(nJ, bVertDir);

			long i = 0; for(i = 0; i < tvArr.GetSize(); i++)
			{
				tv	= tvArr.GetAt(i);
				
				if(GetXyMatchSegAndLine(tvJigan.m_v1, tvJigan.m_v2, tv.m_v1, tv.GetXyDir(), xyMatch))
					xyArr.Add(xyMatch);
			}

			long nSize	= xyArr.GetSize();
			if(nSize > 0)
			{
				CDPoint xyLast(0, 0);
				CDPoint xy(0, 0);
				CDoubleArray dArrDimTmp;

				double dDistMax	= tvJigan.GetLength();
				double dDist	= 0;

				long i = 0; for(i = 0; i < nSize; i++)
				{
					xy	= xyArr.GetAt(i);
					dDist	= ~(xy - tvJigan.m_v2);

					dArrDimTmp.Add(dDist);
				}

				dArrDimTmp.Add(tvJigan.GetLength());

				dArrDimTmp.Sort(TRUE);


				nSize	= dArrDimTmp.GetSize();
				if(dArrDimTmp.GetSize() > 0)
				{
					dDist	= dArrDimTmp.GetAt(0);
					dArrDim.Add(dDist);
					for(i = 1; i < nSize; i++)
					{
						dDistMax	= dArrDimTmp.GetAt(i);
						dDist		= dArrDimTmp.GetAt(i-1);

						if(dDistMax - dDist > 0)
							dArrDim.Add(dDistMax-dDist);
					}
				}
			}

			// 치수 기입
			// 주철근이 선형방향배근이라면 치수선에 obq 적용
			double dObq	=  0;

			double dAng		= pBri->GetAngleByTarget(bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0).GetAngleDegree();
			if((bStt && dAng > 90) || (!bStt && dAng < 90))
			{
				CLineInfo *pLineB	= pBri->GetLineBase();
				if(!pLineB) return;	// program 다운 방지를 위해 깔끔하게 리턴시켜버림

				double dSta	= 0;
				if(pBri->m_nTypePlacingMainRebar_UpperSlab == 1 || pBri->m_nTypePlacingMainRebar_UpperSlab == 2)
					pLineB->GetStationMatchVerticalByXy(tvJigan.m_v2, dSta);	// 중심선형에서의 기점
				else
					pLineB->GetStationMatchVerticalByXy(tvJigan.m_v1, dSta);	// 중심선형에서의 기점
				// 					xyStt	= pBri->GetXySlabEdgeAct(dSta, CDPoint(0, 1), FALSE);

				xyStt	= tvJigan.m_v2;
			}
			baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, TRUE, FALSE);

			// 방향에 맞게 회전(시종점 접선의 직각방향으로 회전)
			CDPoint xyDir	= pBri->GetLineBase()->GetAngleAzimuth(dSta).Rotate90();
			Dom.Rotate(xyStt, ToDPointFrDegree(xyDir.GetAngleDegree()+(bStt ? dObq : -dObq)));

			// 사보강 배치인 경우 
			if(pBri->m_nTypePlacingMainRebar_UpperSlab == 1 || pBri->m_nTypePlacingMainRebar_UpperSlab == 2)
			{
				Dom.Rotate(xyStt, pBri->GetAngleByTarget(bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0).RotateInv90());
			}

			*pDomP << Dom;
		}
	}

	// 절단원 적용해서 그림 
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_HIDDEN_LINE);

	if(bApplyCutArea)
	{
		ApplyCuttingArea_Plane(&Dom, tvArr, TRUE, 0, FALSE, FALSE, FALSE, FALSE, TRUE, nJ, FALSE, FALSE, NULL, FALSE);
		ApplyCuttingArea_Plane(&Dom, tvArrLeftHalf, TRUE, 0, FALSE, FALSE, FALSE, FALSE, TRUE, nJ, FALSE, FALSE, pBri->GetLineLeftRight(TRUE), FALSE);
		ApplyCuttingArea_Plane(&Dom, tvArrRightHalf, TRUE, 0, FALSE, FALSE, FALSE, FALSE, TRUE, nJ, FALSE, FALSE, pBri->GetLineLeftRight(FALSE), FALSE);
		ApplyCuttingArea_Plane(&Dom, tvArrAngLeft, TRUE, 0, FALSE, FALSE, FALSE, FALSE, TRUE, nJ, FALSE, FALSE, pBri->GetLineLeftRight(TRUE), FALSE);
		ApplyCuttingArea_Plane(&Dom, tvArrAngRight, TRUE, 0, FALSE, FALSE, FALSE, FALSE, TRUE, nJ, FALSE, FALSE, pBri->GetLineLeftRight(FALSE), FALSE);
	}

	m_pStd->DrawTvArrWidthHidden(&Dom, tvArr, pBri->GetLineBase());
	m_pStd->DrawTvArrWidthHidden(&Dom, tvArrLeftHalf, pBri->GetLineBase());
	m_pStd->DrawTvArrWidthHidden(&Dom, tvArrRightHalf, pBri->GetLineBase());
	m_pStd->DrawTvArrWidthHidden(&Dom, tvArrAngLeft, pBri->GetLineBase());
	m_pStd->DrawTvArrWidthHidden(&Dom, tvArrAngRight, pBri->GetLineBase());

	if(bMarkRebar)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);


		// 사보강 배치가 아닐때는 선형용 치수기입 함수를 사용하고(주철근 함수 내용 복사함)
		// 사보강 배치일때는 기초에서 복사해온 일반 치수기입용 함수를 사용한다. 
		if(pBri->m_nTypePlacingMainRebar_UpperSlab == 0)
		{
			long nCount	= pBri->m_pArrRCMainLowerSlabHunch[nCycle].GetSize();
			long i = 0; for(i = 0; i < nCount; i++)
			{
				CRebarInfoCycle *pRC	= pBri->m_pArrRCMainLowerSlabHunch[nCycle].GetAt(i);
				DimRebarInfoCycle_Plane_LowerSlab(&Dom, pRC, FALSE, 0, nCycle, FALSE, nDrawLeft, FALSE, FALSE, TRUE, 2, bApplyCutArea);
			}
		}
		else if(pBri->m_nTypePlacingMainRebar_UpperSlab == 1 || pBri->m_nTypePlacingMainRebar_UpperSlab == 2)
		{
			CLineInfo *pLineVert	= pBri->GetLineBase(pBri->IsVertDir());
			CRebarPlacing rb;
			CTwinVectorArray tvArrSup, tvArrSupAngStt, tvArrSupAngEnd, tvArrRAngStt, tvArrRAngEnd;
			pBri->GetTvRebarSupport_Plane_SlabHunch(tvArrSup, TRUE, 0, nJ);
			pBri->GetTvRebarSupport_Plane_Slab_Std(TRUE, tvArrSup, tvArrSupAngStt, tvArrSupAngEnd, tvArrRAngStt, tvArrRAngEnd, FALSE, 0, 0, TRUE);

			CTwinVector tv;
			long nSize			= pBri->m_pArrRCMainUpperSlabHunch[nCycle].GetSize();
			long dCountCycle	= pBri->GetCountCycleMainRebar();


			long i = 0; for(i = 0; i < nSize; i++)
			{
				CRebarInfoCycle *pRC	= pBri->m_pArrRCMainUpperSlabHunch[nCycle].GetAt(i);
				if(!pRC) continue;
				if(!pRC->m_bExist) continue;
				if(pRC->m_nPosHunch != nJ) continue;

				CRebar *pRb = pBri->GetRebarMainByDescription(pRC->m_sDescription);

				// 수직보강..
				CString sMark	= pRC->m_szMark;
				if(tvArr.GetSize() > 0 && m_pStd->IsMarkRebar(sMark))
				{
					//#0031780 - 2상면철근에 하면 철근이 표현됨 상부인경우는 마킹을 하지 않는다.
					if(bUpper == FALSE) MarkRebarMain_Plane_LowerSlab_ForVert(&Dom, tvArr, tvArrLeftHalf, tvArrRightHalf, tvArrSup, i, FALSE, 0, nCycle, TRUE, FALSE, FALSE, nDrawLeft);
				}


				// 사보강 (좌측)
				sMark.Format("%s-1", pRC->m_szMark);
				if(tvArrAngLeft.GetSize() > 0 && m_pStd->IsMarkRebar(sMark))	// 1단만 마크해도 복잡한데, 2,3단까지 전부 마크하면 너무 지저분해짐
				{
					rb.ReverseRebar(tvArrAngLeft);
					CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
					mark.SetRebar(tvArrAngLeft, tvArrSup);

					// 설정
					mark.m_sMark					= sMark;
					mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pBri->GetValueFy(ePartLowerSlab));
					mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_END;
					mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
					mark.m_dPosScaleMarkStt			= 1./(dCountCycle+nSize) * (nCycle+i+1)+0.2;
					mark.m_dPosScale				= 1./(dCountCycle+nSize) * (nCycle+i+1)+0.2;
					mark.m_dPosScaleSpace			= 0.5 - (1./dCountCycle)/2;

					CDPointArray xyArr;
					mark.GetXyCountMark(xyArr);
					mark.m_xyArrCountMark	= xyArr;

					if(xyArr.GetSize() > 1)
					{
						CLineInfo *pLine	= pBri->GetLineBase();
						if(pLine)
						{
							CDPoint xyMark	= mark.GetXyMark(FALSE);
							CDPoint vAng	= (xyArr.GetAt(0)-xyArr.GetAt(1)).Unit();
							CDPoint vAngJ	= pBri->GetAngleJijum(0);
							double dSta		= xyMark.x;
							pLine->GetStationMatchByXyAndAngle(xyMark, vAng, dSta, 0);
							double dWidth	= pBri->GetWidthSlabAct(dSta, vAngJ, -1);
							CDPoint xyMatch	= pLine->GetXyLineByLengthAndAngle(dSta, -dWidth, vAngJ);
							mark.m_dAddHeight	= ~(xyMatch-xyMark) + Dom.GetTextHeight();
						}
						mark.m_dCtc = pBri->GetCtcRebarMain(TRUE);
						mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;

						mark.MarkRebar(FALSE);	
					}
				}

				// 사보강 (우측)
				sMark.Format("%s-2", pRC->m_szMark);
				if(tvArrAngRight.GetSize() > 0 && m_pStd->IsMarkRebar(sMark))	// 1단만 마크해도 복잡한데, 2,3단까지 전부 마크하면 너무 지저분해짐
				{
					rb.ReverseRebar(tvArrAngRight);
					CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
					mark.SetRebar(tvArrAngRight, tvArrSup);

					// 설정
					mark.m_sMark					= sMark;
					mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pBri->GetValueFy(ePartLowerSlab));
					mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_STT;
					mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
					mark.m_dPosScaleMarkStt			= 1./(dCountCycle+nSize) * (nCycle+i+1)+0.2;
					mark.m_dPosScale				= 1./(dCountCycle+nSize) * (nCycle+i+1)+0.2;
					mark.m_dPosScaleSpace			= 0.5 - (1./dCountCycle)/2;

					CDPointArray xyArr;
					mark.GetXyCountMark(xyArr);
					mark.m_xyArrCountMark	= xyArr;

					if(xyArr.GetSize() > 1)
					{
						CLineInfo *pLine	= pBri->GetLineBase();
						if(pLine)
						{
							long nSize		= xyArr.GetSize();
							CDPoint xyMark	= mark.GetXyMark(FALSE);
							CDPoint vAng	= (xyArr.GetAt(nSize-2)-xyArr.GetAt(nSize-1)).Unit();
							CDPoint vAngJ	= pBri->GetAngleJijum(0);
							double dSta		= xyMark.x;
							pLine->GetStationMatchByXyAndAngle(xyMark, vAng, dSta, 0);
							double dWidth	= pBri->GetWidthSlabAct(dSta, vAngJ, 1);
							CDPoint xyMatch	= pLine->GetXyLineByLengthAndAngle(dSta, dWidth, vAngJ);
							mark.m_dAddHeight	= ~(xyMatch-xyMark)+Dom.GetTextHeight();
						}
						mark.m_dCtc = pBri->GetCtcRebarMain(TRUE);
						mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;

						mark.MarkRebar(FALSE);	
					}
				}
			}
		}
	}

	// 가각부
	if(bUpper && (nJ == 0 || nJ == pBri->m_nQtyJigan))
	{
		BOOL bStt = nJ == 0 ? TRUE : FALSE;
		for(long ilr=0; ilr<2; ++ilr)
		{
			BOOL bLeft = ilr == iLEFT ? TRUE : FALSE;
			if(nDrawLeft == -1 && bLeft) continue;
			if(nDrawLeft == 1 && !bLeft) continue;

			if(pBri->IsGagakForLine(bStt, bLeft))
			{
				CTwinVectorArrayArray pTvArrTemp1, pTvArrTemp2, pTvArrTemp3, pTvArrTemp4;
				CTwinVectorArray tvArrCycleHunch[2];
				pBri->GetTvRebarMain_Plane_Slab_Exp(FALSE, &pTvArrTemp1, &pTvArrTemp2, &pTvArrTemp3, &pTvArrTemp4, bStt, bLeft, 0, TRUE, tvArrCycleHunch[0], tvArrCycleHunch[1]);
				if(tvArrCycleHunch[nCycle].GetSize() > 0)
				{
					if(bApplyCutArea)
					{
						ApplyCuttingArea_Plane(&Dom, tvArrCycleHunch[nCycle], TRUE, 0, FALSE, FALSE, FALSE, FALSE, TRUE, nJ, FALSE, FALSE, pBri->GetLineLeftRight(TRUE), FALSE);
					}

					m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
					m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_HIDDEN_LINE);
					m_pStd->DrawTvArray(&Dom, tvArrCycleHunch[nCycle]);

					if(bMarkRebar)
					{
						CRebarPlacing rb;
						CString sMark(_T("")), sDescription(_T(""));
						double dDia(0);

						pBri->GetInfoMainRebarExp_LowerSlabHunch(bStt, bLeft, nCycle, sMark, dDia, sDescription);

						CRebar *pRb = pBri->GetRebarMainByDescription(sDescription);

						CTwinVectorArray tvArrSup;
						pBri->GetTvRebarSupport_Plane_SlabHunch(tvArrSup, FALSE, 0, nJ);
						if(tvArrSup.GetSize() > 0)
						{
							// 헌치 철근 갯수가 적어서 마킹 위치가 뭉친다. 가상 위치의 헌치 철근을 넣어서 마킹하자.
							CTwinVector tvTemp = tvArrSup.GetAt(bStt ? 0 : (tvArrSup.GetSize()-1));
							double dOffset = bStt ? -300 : 300;
							rb.Offset(tvTemp.m_v1, tvTemp.m_v2, dOffset);
							if(bStt)
								tvArrSup.InsertAt(0, tvTemp);
							else
								tvArrSup.Add(tvTemp);
						}
						else
						{
							CTwinVector tvTemp;
							CDPoint xyMid = tvArrCycleHunch[nCycle].GetXyMid();
							CDPoint xyTop = tvArrCycleHunch[nCycle].GetXyTop();
							CDPoint xyBot = tvArrCycleHunch[nCycle].GetXyBottom();
							tvTemp.m_v1 = CDPoint(xyMid.x, xyTop.y);
							tvTemp.m_v2 = CDPoint(xyMid.x, xyBot.y);
							tvArrSup. Add(tvTemp);

							double dOffset = bStt ? -300 : 300;
							rb.Offset(tvTemp.m_v1, tvTemp.m_v2, dOffset);
							if(bStt)
								tvArrSup.InsertAt(0, tvTemp);
							else
								tvArrSup.Add(tvTemp);
						}

						rb.ReverseRebar(tvArrCycleHunch[nCycle]);

						CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
						mark.SetRebar(tvArrCycleHunch[nCycle], tvArrSup);

						// 설정
						mark.m_sMark					= sMark;
						mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(dDia, pBri->GetValueFy(ePartUpperSlab));
						if(bApplyCutArea)
							mark.m_nTypeMarkPosLineRebar	= bLeft ? TYPE_MARKPOS_STT : TYPE_MARKPOS_END;
						else
							mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_POS;
						mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
						mark.m_dPosScaleMarkStt			= 0.5;
						mark.m_dPosScale				= bStt ? 0.2 : 0.8;
						mark.m_dPosScaleSpace			= 0.5;
						mark.m_dAngRotCount				= 90;
						mark.m_nTypeMarkDir				= bStt ? TYPE_MARKDIR_LEFT : TYPE_MARKDIR_RIGHT;

						CDPointArray xyArr;
						mark.GetXyCountMark(xyArr);
						mark.m_xyArrCountMark	= xyArr;

						if(xyArr.GetSize() > 1)
						{
							if(pLine)
							{
								CDPoint xyMark	= mark.GetXyMark(FALSE);
								CDPoint vAng	= (xyArr.GetAt(0)-xyArr.GetAt(1)).Unit();
								CDPoint vAngJ	= pBri->GetAngleJijum(nJ);
								double dSta		= xyMark.x;

								if(!bApplyCutArea)
								{
									double dDist = pLine->GetDisMatchVerticalByXy(xyMark);
									double dStaEnd = pBri->GetStationAtSlabEnd(bStt, 0) + Dom.Always(10) * (bStt ? -1 : 1);
									CDPoint xyMarkStt	= pLine->GetXyLineDisByAng(dStaEnd, dDist, pBri->GetAngleJijum(bStt ? 0 : pBri->m_nQtyJigan));
									mark.m_dAddHeight	= ~(xyMark - xyMarkStt);
								}
								else
								{
									//double dWid	= pBri->GetWidthSlabActByGagak(dSta, mark.m_xyDirCountMark, bLeft ? -1 : 1) + (bLeft ? MARK_BASEHEIGHT_UPPER : MARK_BASEHEIGHT_LOWER);
									//CDPoint xyMarkStt	= pLine->GetXyLineByLengthAndAngle(dSta, dWid, mark.m_xyDirCountMark);
									mark.m_dAddHeight	= bLeft ? MARK_BASEHEIGHT_UPPER : MARK_BASEHEIGHT_LOWER;
								}
							}
							mark.m_dCtc						= pBri->GetCtcRebarMain(TRUE);
							mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;

							mark.MarkRebar(FALSE);
						}
					}
				}
			}
		}
	}

	*pDomP << Dom;
}

void CDrawPlane::DrawRebarSupport_Plane_LowerSlabGirderAndHunch( CDomyun *pDomP, long nJ, BOOL bUpper, long nDan, BOOL bGirder, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft, BOOL bApplyCutArea )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);

	// draw == 0 : 거더상부1단 배력근 
	// draw == 1 : 헌치부 배력근
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CTwinVectorArray tvArr;
	if(bGirder)
	{
		pBri->GetTvRebarSupport_Plane_SlabGirder(tvArr, FALSE, nJ, nDan, bUpper, nDrawLeft);
	}
	else
	{
		pBri->GetTvRebarSupport_Plane_SlabHunch(tvArr, FALSE, nDrawLeft, nJ);
	}

	if(tvArr.GetSize() == 0) return;


	// 치수 기입
	if(bDimRebar)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	}

	// 절단원 적용해서 그림 
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	if(bApplyCutArea)
		ApplyCuttingArea_Plane(&Dom, tvArr, TRUE, 0, bGirder, FALSE, FALSE, FALSE, !bGirder, nJ, FALSE, FALSE, NULL, FALSE);
	baseDraw.DrawTvArray(&Dom, tvArr);


	// 헌치부 배력근에서 복사해서 약간 수정한 코드임.
	// (39675) 슬래브 헌치 배력철근인데 왜 HS라고 마킹하는지 알수 없음. 철근 찾아서 마킹하도록 수정.
// 	CString sMark	= _T("HS");
	CString sMark	= pBri->GetMarkRebarSupport(0, FALSE, FALSE, FALSE, _T("하부슬래브 헌치부 배력철근"));
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark) && tvArr.GetSize() > 0)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		CLineInfoApp *pLine	= static_cast<CLineInfoApp*>(pBri->GetLineBase());
		if(!pLine) return;
		CDPointArray xyArrMark;	// countmark할 위치들.

		// 배력근을 마크해야 되는 dist, sta등의 정보를 얻음.
// 		CDPoint xyDir	= tvArr.GetAt(0).GetXyDir(FALSE);
		double dSta		= 0;
		pLine->GetStationMatchByXyAndAngle(tvArr.GetAt(0).m_v1, tvArr.GetAt(0).GetXyDir(), dSta, 0);
		CDPoint xyDir			= pBri->GetAngleJijum(nJ);
		double dWid		= (pBri->m_bIsExp ? pBri->GetWidthSlabActByGagak(dSta, xyDir, bGirder? 1 : -1) : pBri->GetWidthSlabAct(dSta, xyDir, bGirder? 1 : -1)) * (bGirder? 1 : -1);
		double dDist	= dWid*2/3;

		if(bApplyCutArea)
		{
			// 절단원 사이에 마크해야 됨.
			CTwinVectorArray tvArrCuttingArea;
			if(bGirder)
				pBri->GetTvCuttingArea_Plane_UpperSlabGirder(tvArrCuttingArea, nJ);
			else
				pBri->GetTvCuttingArea_Plane_SlabHunch(tvArrCuttingArea, nJ, FALSE);

			if(tvArrCuttingArea.GetSize() > 1)
			{
				CTwinVector tvCutting	= tvArrCuttingArea[1];
				CDPoint xyCen = tvArrCuttingArea.GetXyCenter();
				if(bGirder)
					dDist	= pLine->GetDisMatchVerticalByXy(xyCen) + tvCutting.m_v2.z/2;
				else
					dDist	= pLine->GetDisMatchVerticalByXy(xyCen) + tvCutting.m_v2.z*3/2;
				dSta	= tvCutting.GetXyMid().x;
			}
		}
		else
		{
			dSta = tvArr.GetXyMid().x;
		}

		if(!pLine->GetStationMatchVerticalByXy(tvArr[0].m_v1, dSta)) return;

		// 1cycle과 2cycle 주철근을 구함. /////////////////////////
		// 정확한 dDist를 구하기 위해서(주철근과 겹치지 않도록 하기 위해.)
		// 배력근 마킹은 dSta에서 하지만, 주철근은 2cycle을 반드시 고려 해야 되므로
		// 상면하면에 따라서 반드시 2cycle이 있을 만한 장소의 주철근 간격을 가져온다.
		CDoubleArray dArrDistMain, dArrDistHunch;
		CDoubleArray dArrDistMain2, dArrDistHunch2;
		CDoubleArray dArrDistMain3, dArrDistHunch3;
		CDoubleArray dArrDistMain4, dArrDistHunch4;
		pBri->GetDistRebarMain_CrossSection_Slab(FALSE, dArrDistMain, dArrDistHunch, dArrDistMain2, dArrDistHunch2, dArrDistMain3, dArrDistHunch3, dArrDistMain4, dArrDistHunch4, dSta, TRUE, 0, TRUE, FALSE);

		long i = 0; for(i = 0; i < dArrDistMain2.GetSize(); i++)
			dArrDistMain.Add(dArrDistMain2.GetAt(i));
		for(i = 0; i < dArrDistMain3.GetSize(); i++)
			dArrDistMain.Add(dArrDistMain3.GetAt(i));
		for(i = 0; i < dArrDistMain4.GetSize(); i++)
			dArrDistMain.Add(dArrDistMain4.GetAt(i));
		/////////////////////////////////////////////////////////////

		// 마킹할 위치 찾아옴
		GetXyCountMarkSupport_Plane_Slab(xyArrMark, tvArr, TRUE, dDist, dArrDistMain);

		// count mark하고 선을 그음.
		CDPoint xyDirLine	= pLine->GetAngleAzimuth(dSta);
		CConcSymbol sym(m_pStd->m_pARoadOptionStd);
		for(i = 0; i < xyArrMark.GetSize(); i++)
			sym.RebarCountMarkOne(&Dom, xyArrMark[i], xyDirLine);

		long nSize	= xyArrMark.GetSize();
		if(nSize > 0)
		{
			CDPoint xyStt			= xyArrMark.GetAt(0);
			CDPoint xyEnd			= xyArrMark.GetAt(nSize-1);
			CDPoint xyDirAzimuth	= pBri->GetAngleAzimuthJijum(nJ);

			CTwinVector tv;
			tv.m_v1	= CVector(xyStt.x, xyStt.y, 4);
			tv.m_v2	= xyEnd;
			baseDraw.DrawTv(&Dom, tv);

			// count mark의 3/2지점쯤에 철근표시함.
			long nIdxPrev	= nSize > 1 ? nSize-2 : 0;
			long nIdxNext	= nIdxPrev+1;
			if(nIdxNext < 0) nIdxNext	= nIdxPrev;
			double dStaPrev(0), dStaNext(0), dStaMark(0);
			pLine->GetStationMatchByXyAndAngle(xyArrMark[nIdxPrev], xyDirAzimuth, dStaPrev);
			if(xyArrMark.GetSize() > nIdxNext)
				pLine->GetStationMatchByXyAndAngle(xyArrMark[nIdxNext], xyDirAzimuth, dStaNext);
			else
				dStaNext	= dStaPrev;
			dStaMark	= (dStaPrev+dStaNext)/2;

			// dDist에도 xyDir.y를 나눠야 되지만, 이미 GetXyCountMarkSupport_Plane_UpperSlab에서 
			// xyDir.y가 적용되서 계산된 길이가 나오므로.. 그냥 사용한다.
			dWid		= (pBri->m_bIsExp ? pBri->GetWidthSlabActByGagak(dStaMark, xyDir, bGirder? 1 : -1) : pBri->GetWidthSlabAct(dStaMark, xyDir, bGirder? 1 : -1)) * (bGirder? 1 : -1);

			CDPoint xyMarkStt	= pLine->GetXyLineByLengthAndAngle(dStaMark, dDist, xyDir);
			CDPoint xyMarkEnd	= pLine->GetXyLineByLengthAndAngle(dStaMark, dWid/xyDir.y * (nDrawLeft? -1:1), xyDir);
			xyMarkEnd	-= MARK_BASEHEIGHT_LOWER * xyDirAzimuth;
			Dom.LineTo(xyMarkStt, xyMarkEnd);

			// 마킹함.
			*pDomP << Dom;
			CString sDia	= m_pStd->m_pARcBridgeDataStd->GetStringDia(18, pBri->GetValueFy(ePartLowerSlab));
			CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_SupportRebar_LowerSlab[iUPPER][0];
			if(pPlace)
				sDia	= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(ePartLowerSlab));
			// sym.RebarMarkLineSide(&Dom, xyMarkEnd, sMark, sDia, bGirder ? TRUE : FALSE, 0, FALSE);

			CRebar *pRb = pBri->GetRebarSupportByDescription(_T("하부슬래브 헌치부 배력철근"));
			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = sMark;
			mark_input_data.dia = sDia;
			mark_input_data.position = xyMarkEnd;
			mark_input_data.ctc = pBri->GetCTCRebarSupport_Slab(FALSE, TRUE, 0, TRUE);
			mark_input_data.type_rebar = pRb? pRb->m_nTypeRebar : -1;
			mark_input_data.left_dir = bGirder ? TRUE : FALSE;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.line_rebar = TRUE;
			mark_view_data.mark_pos = eMarkSide;

			sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

			xyDirAzimuth	= pLine->GetAngleAzimuth(dStaMark);
			Dom.Rotate(xyMarkEnd, ToDPointFrDegree((xyDirAzimuth*-1).GetAngleDegree()-180));
		}
	}

	*pDomP << Dom;
}

void CDrawPlane::DimRebarInfoCycle_Plane_LowerSlab( CDomyun *pDomP, CRebarInfoCycle *pRC, BOOL bUpper, long nDan, long nCycle, BOOL bGirder, long nDrawLeft, BOOL bSttPRF/*=FALSE*/, BOOL bEndPRF/*=FALSE*/, BOOL bHunch/*=FALSE*/, long nDirMark/*=2*/, BOOL bApplyCutArea )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(!pRC) return;
	CLineInfo *pLine	= pBri->GetLineBase();
	if(!pLine) return;

	// 에러 방지를 위해 헌치부일 경우 무조건 상부 ctc로 설정한다.
	if(bHunch)
	{
		bUpper	= TRUE;
		nDan	= 0;
	}
	else if(bSttPRF || bEndPRF)
	{
		bUpper	= FALSE;
	}

	// 해당 rc의 중간 sta구함
	double dStaStt	= pRC->m_xyStt.x;	// rc의 시작 sta
	double dStaEnd	= pRC->m_nPosStt != pRC->m_nPosEnd ? pRC->m_xyEnd.x : pRC->m_xyMid1.x;	// rc의 끝 sta
	if(pRC->m_bUseTvArrRebar)
	{
		dStaStt	= pRC->m_tvArrRebar.GetXyLeft().x;
		dStaEnd	= pRC->m_tvArrRebar.GetXyRight().x;
	}
	if(dStaStt > dStaEnd) GetSwap(dStaStt, dStaEnd);
	// 마크할 대략의 위치 찾기 /////////////////////////////////////
	double dStaMid	= dStaStt + (dStaEnd-dStaStt)/4.0*1.5;	// 중간 sta
	if(bGirder || bSttPRF || bEndPRF || bHunch)
		dStaMid	= dStaStt + (dStaEnd-dStaStt)/5.0*2.0;
	else if(!bGirder && !bHunch && nCycle == 2)
	{
		dStaMid	= dStaStt + (dStaEnd-dStaStt)/4.0*2.5;
	}

	if(!bGirder && !bSttPRF && !bEndPRF && !bHunch && (nCycle == 1 || nCycle == 3))
	{
		// 2cycle일때는 좌측에서 1/8 위치
		if(nCycle == 1)
			dStaMid	= dStaStt + (dStaEnd-dStaStt)/8*2;
		// 4Cycle일때는 좌측에서 7/8 위치
		if(nCycle == 3)
			dStaMid	= dStaStt + (dStaEnd-dStaStt)/8*6;

		///////////////////////////////////////////////////////////
	}

	// 배력철근 sta들 모두 구함.
	CDoubleArray dArrStaSup;

	if(bGirder)
	{
	}
	else if(bSttPRF || bEndPRF)
	{
//		BOOL bStt	= bSttPRF ? TRUE : FALSE;
//		pBri->GetStaRebarSupport_UpperSlabBracket(dArrStaSup, bStt);
	}
	else if(bHunch)
	{
		dArrStaSup	= pBri->m_dArrStaSupRebarLowerSlab[bUpper ? iUPPER : iLOWER];
		if(dArrStaSup.GetSize() == 0)
			pBri->GetStaRebarSupport_VertSection_SlabHunch(FALSE, dArrStaSup);

		// 헌치 철근 위치에 있는 Station만 걸러낸다.
		for(long ix=0; ix< dArrStaSup.GetSize(); ++ix)
		{
			double dSta = dArrStaSup.GetAt(ix);
			if(dSta < dStaStt || dSta > dStaEnd)
			{
				dArrStaSup.RemoveAt(ix, 1);
				--ix;
			}
		}
	}
	else
	{
		dArrStaSup	= pBri->m_dArrStaSupRebarLowerSlab[bUpper ? iUPPER : iLOWER];
		if(dArrStaSup.GetSize() == 0)
			pBri->GetStaRebarSupport_VertSection_Slab(dArrStaSup, bUpper, nDan, FALSE);
	}

	// 중간 sta에서 가장 가까운 배력철근 sta 구해서 prev와 next를 구함.
	// 위치가 어중간해서 못 찾으면 그냥 마크함
	BOOL bFind		= FALSE;
	double dSta		= 0;
	double dStaPrev	= dStaMid;
	double dStaNext	= dStaMid;
	long nIdxPrev	= 0;
	long nIdxNext	= 0;
	long nSize		= dArrStaSup.GetSize();
	long i = 0; for(i = 0; i < nSize; i++)
	{
		dSta	= dArrStaSup[i];
		if(dSta < dStaMid)
		{
			dStaPrev	= dSta;
			nIdxPrev	= i;
		}
		else
		{
			dStaNext	= dSta;
			nIdxNext	= i;
			bFind		= TRUE;
			break;
		}
	}

	if(!bFind && nSize > 1)
	{
		dStaPrev	= dStaStt;
		dStaNext	= dStaEnd;
		nIdxPrev	= nSize-2;	// 배력근의 각도를 따라서 마크해야 되므로 배력근 인덱스는 잇어야 된다.
		nIdxNext	= nSize-1;
		bFind	= TRUE;
	}

	if(dStaPrev > dStaNext) GetSwap(dStaNext, dStaPrev);
	double dStaMark	= bFind ? dStaPrev + (dStaNext-dStaPrev)/2 : dStaMid;

	// 헌치부 sta를 구하는 경우 이전헌치에서 다음 헌치로 바로 넘어오는 경우가 있다. ////
	// 그럴 경우를 대비해서 마크위치가 철근을 벗어나면 다음 철근 sta으로 옮기자
	if(bHunch)
	{
		if(!(dStaStt <= dStaMark && dStaEnd >= dStaMark))
		{
			if(nIdxNext+1 < nSize)
			{
				nIdxPrev++;
				nIdxNext++;

				dStaPrev	= dArrStaSup.GetAt(nIdxPrev);
				dStaNext	= dArrStaSup.GetAt(nIdxNext);

				dStaMark	= dStaPrev + (dStaNext-dStaPrev)/2;
			}
			else
			{
				dStaMark	= dStaMid;
			}
		}
		// 헌치 철근은 수평철근 갯수가 몇개 안되기 때문에 마킹이 몰린다. 시종점 부 헌치의 경우에는 마킹 위치를 바깥쪽으로 옮겨주자.
		long nJHunch	= pRC->m_nPosHunch;
		if(nJHunch == 0 || nJHunch == pBri->m_nQtyJigan)
		{
			dStaMark = dArrStaSup.GetAt(nJHunch == 0 ? 0 : (dArrStaSup.GetSize()-1)) + (nJHunch == 0 ? -300 : 300);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////

	double dDistMid	= pBri->GetDistMidOfSlab(dStaMid);


	// 주철근 거리 구함.
	CDoubleArray dArrDistMain, dArrDistHunch;
	CDoubleArray dArrDistMainTmp[MAX_REBAR_CYCLE], dArrDistHunchTmp[MAX_REBAR_CYCLE];
	pBri->GetDistRebarMain_CrossSection_Slab(FALSE, dArrDistMainTmp[0], dArrDistHunchTmp[0], dArrDistMainTmp[1], dArrDistHunchTmp[1], dArrDistMainTmp[2], dArrDistHunchTmp[2], dArrDistMainTmp[3], dArrDistHunchTmp[3], dStaMark, bUpper, nDan);
	dArrDistMain	= dArrDistMainTmp[nCycle];
	dArrDistHunch	= dArrDistHunchTmp[nCycle];
	dArrDistMain.Sort(FALSE);
	dArrDistHunch.Sort(FALSE);

	// 시종점 헌치는 주철근과 같다. -> (39755) 시종점 현치도 헌치 거리 고려... 
	// 평면도 일반도에 헌치선 표현요청.. 이건 작업량이 많기 때문에 나중에..
	// long nJijum = pBri->GetJijumNumberByStation(dStaStt);
	// if(bHunch && !(nJijum == 0 || nJijum == pBri->m_nQtyJigan))
	if(bHunch)
		dArrDistMain	= dArrDistHunch;
	
	// 마크함
	// nDrawLeft가 -1이면 거리가 음수인거만, nDrawLeft가 1이면 거리가 양수인거만, nDrawLeft가 0이면 모두 마크함.
	CConcSymbol sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CDPoint xyDirRotate	= pBri->GetXyDirRotateDomByBridgeSttEndXy();
	CDPoint xyDirMark	= CDPoint(0, 1);	
	CDPoint xyMarkStt	= CDPoint(0, 0);
	CDPoint xyMarkEnd	= CDPoint(0, 0);
	BOOL bMarkFirst		= TRUE;
	CDPoint xyMark		= CDPoint(0, 0);
//	double dDist		= 0;

	// 헌치부이거나 브라켓부일 경우 마크 각도는 지점 각도와 같다.
	long nJ	= pRC->m_nPosHunch;
	if(bGirder || bSttPRF || bEndPRF || bHunch)
	{
		xyDirMark	= pBri->GetAngleJijum(nJ);
	}
	else
	{
		long nDir	= pBri->GetDirSupportRebar(dStaMark, bUpper);
		switch(nDirMark)
		{
		case 0: nDir	= -1; break;
		case 1: nDir	= 1; break;
		}

		if(nDir == -1)		xyDirMark	= pLine->GetAngleByOffset(pBri->m_dStationBridgeStt, dStaMark, pBri->GetAngleJijum(0));
		else if(nDir == 1)	xyDirMark	= pLine->GetAngleByOffset(pBri->m_dStationBridgeStt+pBri->m_dLengthBridge, dStaMark, pBri->GetAngleJijum(pBri->m_nQtyJigan));
		else				xyDirMark	= CDPoint(0, 1);
	}

	CDPointArray xyArrMark;	// 마크할 위치를 저장해서 슬래브 안쪽에 있는 것만 표현한다.
	nSize	= dArrDistMain.GetSize();
	// 철근 마크 방향으로 거리를 계산할때
	// 사보강이면 마크방향으로 직각인 거리가적용된다.
	CDPoint xyDirLen	= xyDirMark;
	// 거더, 브라켓, 헌치 주철근은 선형모양이 아니기 때문에 일반 마크하듯이 좌표를 구해야된다.
	if(bGirder)/// || bSttBracket || bEndBracket || bHunch)
	{

	}
	else
	{
		for(i = 0; i < nSize; i++)
		{
			double dDist	= dArrDistMain[i];
			if(nDrawLeft == -1 && dDist >= dDistMid) continue;
			if(nDrawLeft == 1 && dDist <= dDistMid) continue;

			double dStaTmp	= pLine->GetStationByScInLine(dStaMark, dDist, xyDirMark, 0);
			xyMark	= pLine->GetXyLineByLengthAndAngle(dStaTmp, dDist, CDPoint(0, 1));

			if(bMarkFirst) 
			{
				xyMarkStt	= xyMark;
				bMarkFirst	= FALSE;
			}
			else
				xyMarkEnd	= xyMark;
			xyArrMark.Add(xyMark);
		}
	}

	// 마크한줄을 만들었으면 슬래브 안쪽에 있는것만 골라낸다. /////
	CRebarPlacing rb;
	CTwinVectorArray tvArrPlane;
	CVectorArray vecArrArea;
	pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, 0);
	tvArrPlane.GetToVectorArray(vecArrArea);
	rb.GetXyByVectorArrayArea(xyArrMark, vecArrArea, TRUE);

	// 절단원에 대해서 적용함.
	if(bApplyCutArea)
		ApplyCuttingArea_Plane(&Dom, xyArrMark, bUpper, nDan, bGirder, FALSE, bSttPRF, bEndPRF, bHunch, nJ, FALSE, FALSE, NULL, FALSE);

	if(xyArrMark.GetSize() == 0) return;
	///////////////////////////////////////////////////////////////

	// 마크한다.
	CDPoint xyDirLine	= pLine->GetAngleAzimuth(dStaMark);
	if(pBri->GetAngleByStation(dStaMark).GetAngleDegree() > 90)
		xyDirLine	= xyDirLine.Rotate(CDPoint(0, 1));
	nSize	= xyArrMark.GetSize();
	for(i = 0; i < nSize; i++)
		sym.RebarCountMarkOne(&Dom, xyArrMark[i], xyDirLine);

	BOOL bLeftDir	= FALSE;//nCycle==0 || nCycle==2 ? TRUE : FALSE;
	CString sMark	= pRC->m_szMark;
	CString sDia	= m_pStd->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pBri->GetValueFy(ePartLowerSlab));
	if(nDan == 0 && !bSttPRF && !bEndPRF)
	{
		// xyMarkEnd-xyMarkStt 하면 방위각이 나온다.
		// 그러나 선형에 적용하기 위해선 선형에 대한 상대각이 필요함
		CDPoint xyDirMarkLine		= xyDirMark;	// 선형에 적용하는 각도
		CDPoint xyDirMarkAzimuth	= xyDirMark;	// 실제 방위각
		if(bUpper)
		{
			if(bHunch && !bApplyCutArea && (nJ == 0 || nJ == pBri->m_nQtyJigan))
			{
				bLeftDir = nJ == 0 ? TRUE : FALSE;
				long nIndexPos = xyArrMark.GetSize() / 2;
				xyMarkStt	= (xyArrMark.GetAt(nIndexPos) + xyArrMark.GetAt(nIndexPos+1)) / 2.;
				double dDist = pLine->GetDisMatchVerticalByXy(xyMarkStt);
				double dSta = pBri->GetStationAtSlabEnd(nJ == 0, 0) + Dom.Always(10) * (nJ == 0 ? -1 : 1);
				xyMarkEnd	= pLine->GetXyLineDisByAng(dSta, dDist, pBri->GetAngleJijum(nJ));

				if(nSize > 0)
				{
					Dom.LineTo(xyArrMark[0], xyArrMark[nSize-1]);
				}
			}
			else
			{
				xyDirMarkAzimuth	= (xyMarkEnd - xyMarkStt).Unit();
				xyDirMarkLine		= xyDirMark;

				double dWid	= pBri->m_bIsExp ? pBri->GetWidthSlabActByGagak(dStaMark, xyDirMarkLine, -1) + MARK_BASEHEIGHT_UPPER : 
					pBri->GetWidthSlabAct(dStaMark, xyDirMarkLine, -1) + MARK_BASEHEIGHT_UPPER;
				xyMarkEnd	= pLine->GetXyLineByLengthAndAngle(dStaMark, -dWid, xyDirMarkLine);
				//		xyMarkEnd	+= MARK_BASEHEIGHT_UPPER * xyDirMarkAzimuth;	// 선형에 적용하지 않는 각도는 직접 선으로 구해야 됨
			}

			Dom.LineTo(xyMarkStt, xyMarkEnd);
			*pDomP << Dom;

			// sym.RebarMarkLineSide(&Dom, xyMarkEnd, sMark, sDia, bLeftDir, 0, FALSE, "", FALSE);

			CRebar *pRb = pBri->GetRebarMainByDescription(pRC->m_sDescription);
			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = sMark;
			mark_input_data.dia = sDia;
			mark_input_data.position = xyMarkEnd;
			mark_input_data.ctc = pBri->GetCtcRebarMain(TRUE);
			mark_input_data.type_rebar = pRb? pRb->m_nTypeRebar : -1;
			mark_input_data.left_dir = bLeftDir;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.line_rebar = TRUE;
			mark_view_data.mark_pos = eMarkSide;
			mark_view_data.apply_basic_height = FALSE;

			sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

			pLine->GetStationMatchVerticalByXy(xyMarkEnd, dSta);
			CDPoint xyDirAzimuth	= pLine->GetAngleAzimuth(dSta);
			Dom.Rotate(xyMarkEnd, ToDPointFrDegree((xyDirAzimuth*-1).GetAngleDegree()-180));
		}
		else
		{
			xyMarkEnd	= xyArrMark.GetAt(xyArrMark.GetSize()-1);

			xyDirMarkLine		= xyDirMark;

			double dWid	= pBri->m_bIsExp ? pBri->GetWidthSlabActByGagak(dStaMark, xyDirMarkLine, 1) : 
				pBri->GetWidthSlabAct(dStaMark, xyDirMarkLine, 1);
			if(bHunch)
				dWid	+= MARK_BASEHEIGHT_UPPER;
			else
				dWid	+= MARK_BASEHEIGHT_LOWER;
			xyMarkStt	= pLine->GetXyLineByLengthAndAngle(dStaMark, dWid, xyDirMarkLine);


			Dom.LineTo(xyMarkStt, xyMarkEnd);
			*pDomP << Dom;

			// sym.RebarMarkLineSide(&Dom, xyMarkStt, sMark, sDia, bLeftDir, 0, FALSE, "", FALSE);

			CRebar *pRb = pBri->GetRebarMainByDescription(pRC->m_sDescription);
			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = sMark;
			mark_input_data.dia = sDia;
			mark_input_data.position = xyMarkStt;
			mark_input_data.ctc = pBri->GetCtcRebarMain(TRUE);
			mark_input_data.type_rebar = pRb? pRb->m_nTypeRebar : -1;
			mark_input_data.left_dir = bLeftDir;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.line_rebar = TRUE;
			mark_view_data.mark_pos = eMarkSide;
			mark_view_data.apply_basic_height = FALSE;

			sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

			pLine->GetStationMatchVerticalByXy(xyMarkStt, dSta);
			CDPoint xyDirAzimuth	= pLine->GetAngleAzimuth(dSta);
			Dom.Rotate(xyMarkStt, ToDPointFrDegree((xyDirAzimuth*-1).GetAngleDegree()-180));
		}
	}
	else	// 2,3단은 시점측으로 뻗어서 마크 기입
	{
		// 마크를 시점측으로 뻗을지 종점으로 뻗을지?
		BOOL bExtendStt	= TRUE;
		if(bSttPRF || bEndPRF)
			bExtendStt	= FALSE;

		long nSize	= xyArrMark.GetSize();
		if(nSize > 1)
		{
			CHgBaseDrawStd baseDraw;
			baseDraw.m_pLine	= pLine;
			CDPoint xyDir	= pBri->GetAngleJijum(0);

			// count mark를 잇는 선
			Dom.LineTo(xyArrMark[0], xyArrMark[nSize-1]);

			// count mark의 중심에서 철근마크까지 잇는 선.
			if(bExtendStt)
			{
				dStaStt	= pBri->m_dStationBridgeStt-pBri->m_dWS;
			}
			else
			{
				dStaStt	= pBri->m_dStationBridgeStt+pBri->m_dLengthBridge+pBri->m_dWE;
			}


			long nIdx	= nSize/2;
			if(nIdx == 1) nIdx	= 0;
			CDPoint xyPrev	= xyArrMark.GetAt(nIdx),
				xyNext	= xyArrMark.GetAt(nIdx+1);
			// 철근 마크 하는 위치는 2cycle지원일때와 4cycle지원일때가 달라야 된다.
			CDPoint xyMark	= xyPrev+(xyNext-xyPrev).Unit()*(~(xyPrev-xyNext))/4;
			if(!bSttPRF && !bEndPRF)
			{
				if(pBri->GetCountCycleMainRebar() == 4)
					xyMark	= xyPrev+(xyNext-xyPrev).Unit()*(~(xyPrev-xyNext))/4.0*1.5;
			}
			else
			{
				xyMark	= xyPrev+(xyNext-xyPrev).Unit()*(~(xyPrev-xyNext))/2;
			}

			double dDist	= pLine->GetDisMatchVerticalByXy(xyMark);
			CDPoint xyStt	= pLine->GetXyLineByLengthAndAngle(dStaStt, dDist/xyDir.y, xyDir);

			CTwinVectorArray tvArr;
			CTwinVector tv;
			tv.m_v1	= xyStt;
			tv.m_v2	= xyMark;
			tv.m_v1.z	= 4;
			tvArr.Add(tv);
			baseDraw.DrawTvArray(&Dom, tvArr);

			// 철근 마크 하기.
			*pDomP << Dom;

			xyMark	= xyStt;
			// sym.RebarMarkLineSide(&Dom, xyMark, sMark, sDia, bExtendStt ? TRUE : FALSE, 0, FALSE, "", FALSE);

			CRebar *pRb = pBri->GetRebarMainByDescription(pRC->m_sDescription);
			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = sMark;
			mark_input_data.dia = sDia;
			mark_input_data.position = xyMark;
			mark_input_data.ctc = pBri->GetCtcRebarMain(TRUE);
			mark_input_data.type_rebar = pRb? pRb->m_nTypeRebar : -1;
			mark_input_data.left_dir =  bExtendStt ? TRUE : FALSE;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.line_rebar = TRUE;
			mark_view_data.mark_pos = eMarkSide;
			mark_view_data.apply_basic_height = FALSE;

			sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

			pLine->GetStationMatchVerticalByXy(xyMark, dSta);
			CDPoint xyDirAzimuth	= pLine->GetAngleAzimuth(dSta);
			Dom.Rotate(xyMark, ToDPointFrDegree((xyDirAzimuth*-1).GetAngleDegree()-180));
		}
	}

	*pDomP << Dom;
}

void CDrawPlane::DrawRebarShear_Plane_LowerSlab( CDomyun *pDomP, long nDrawLeft )
{
	if(!pDomP) return;

	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	CHgBaseDrawStd baseDraw;

	CTwinVectorArray tvArr;
	pBri->GetTvRebarShear_Plane_Slab_Real(FALSE, tvArr);
	//pBri->GetTvRebarShear_Plane_UpperSlab(tvArr);

	// 전단철근 모양으로 만들기
	CRebarPlacing rb;
	rb.GetTvArrShearRebar(tvArr, FALSE, pBri->m_RebarShearDetail_LowerSlab.m_nType == TYPE_SHEAR_DIGUT || pBri->m_RebarShearDetail_LowerSlab.m_nType == TYPE_SHEAR_SPACER || pBri->m_RebarShearDetail_LowerSlab.m_nType == TYPE_SHEAR_SINGLE_HOOK, FALSE);

	// 절단원에 대해서 적용(1단 주철근과 같은 옵션임)
	//ApplyCuttingArea_Plane(&Dom, tvArr, TRUE, 0, FALSE, FALSE);	// 구지 호출 할 필요가 없어서 막아둠.

	// nDrawLeft에 따라서 필요한 것만 골라냄
	if(nDrawLeft != 0)
	{
		CTwinVectorArray tvArrArea;
		pBri->GetTvCuttingArea_UpperSlab_Shear(tvArrArea, nDrawLeft);

		rb.GetTvArrayByCrossTvArrayArea(tvArr, tvArrArea, TRUE);

		// 자르는 타입이 원인 경우 자르는 영역을 그려준다.
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
		baseDraw.DrawTvArray(&Dom, tvArrArea);
	}

	// 전단철근 그리기
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);	
	baseDraw.DrawTvArray(&Dom, tvArr);

	*pDomP << Dom;
}

void CDrawPlane::DrawRebarSupport_Plane_LowerSlabPRF( CDomyun *pDomP, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CTwinVectorArray tvArr;
	pBri->GetTvRebarSupport_Plane_LowerSlabPRF(tvArr, bStt, 1);

	// 절단원 적용해서 그림
	ApplyCuttingArea_Plane(&Dom, tvArr, FALSE, 0, FALSE, FALSE, bStt, !bStt, FALSE, 0, FALSE, FALSE, NULL, FALSE);
	baseDraw.DrawTvArray(&Dom, tvArr);

	CString sDescription	= _T("");
// 	CString sMark			= _T("");
	sDescription.Format("부상방지저판 수평철근");
	CString sMark	= pBri->GetMarkRebarSupport(0, FALSE, FALSE, FALSE, sDescription);
	long stt	= bStt ? iSTT : iEND;
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &pBri->m_pArrRCMainBoxLowerPRF_Cycle1[stt];
	if(pArrRC->GetSize() == 0) pArrRC = &pBri->m_pArrRCMainBoxLowerPRF_Cycle2[stt];
	if(pArrRC->GetSize() == 0) return;


	long nSize = tvArr.GetSize();

	if(bMarkRebar && m_pStd->IsMarkRebar(sMark) && nSize > 0 && pArrRC->GetSize() > 0)
	{
		*pDomP << Dom;

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		CConcSymbol sym(m_pStd->m_pARoadOptionStd);

		CTwinVector tv;
		tv	= bStt ? tvArr.GetAt(0) : tvArr.GetAt(nSize-1);

		CString sDia	= m_pStd->m_pARcBridgeDataStd->GetStringDia(pBri->m_dPRF_Dia_Support, pBri->GetValueFy(ePartLowerSlab));

		// sym.RebarMarkLineSide(&Dom, tv.GetXyMid(), sMark, sDia, bStt);

		CRebar *pRb = pBri->GetRebarSupportByDescription(sDescription);
		SRebarMarkInputData mark_input_data;
		SRebarMarkviewData mark_view_data;
		mark_input_data.mark = sMark;
		mark_input_data.dia = sDia;
		mark_input_data.position = tv.GetXyMid();
		mark_input_data.ctc = pBri->m_dPRF_CTC_Support;
		mark_input_data.type_rebar = pRb? pRb->m_nTypeRebar : -1;
		mark_input_data.left_dir =  bStt;
		mark_input_data.height = 0;

		mark_view_data.count_mark = TRUE;
		mark_view_data.line_rebar = TRUE;
		mark_view_data.mark_pos = eMarkSide;

		sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

		long nJ	= bStt ? 0 : pBri->m_nQtyJigan;
		CDPoint xyRotate	= ToDPointFrDegree(pBri->GetAngleAzimuthJijum(nJ).GetAngleDegree() + 90);

		Dom.Rotate(tv.GetXyMid(), xyRotate);
	}

	*pDomP << Dom;
}

void CDrawPlane::DrawRebarMain_Plane_LowerSlabPRF( CDomyun *pDomP, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	for(long cycle = 0; cycle < 2; cycle++)
	{
		if(!pBri->m_bExistProtectionRiseFooting || (cycle > 0 && pBri->m_bPRF_SameCTC_Main==0)) continue;

		CTwinVectorArray tvArr, tvArrTmp;
		pBri->GetTvRebarMain_Plane_LowerSlabPRF(tvArr, bStt, cycle);

		if(bDimRebar)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		}

		// 절단원 적용해서 그림 
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
		ApplyCuttingArea_Plane(&Dom, tvArr, TRUE, 0, FALSE, FALSE, bStt, !bStt, FALSE, 0, FALSE, FALSE, NULL, FALSE);
		baseDraw.DrawTvArray(&Dom, tvArr);

		if(bMarkRebar && tvArr.GetSize() > 0)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
			CTwinVectorArray tvArrSup, tvArrSupAngLeft, tvArrSupAngRight, tvArrRAngStt, tvArrRAngEnd;

			pBri->GetTvRebarSupport_Plane_LowerSlabPRF(tvArrSup, bStt);
			if(tvArrSup.GetSize() > 1 && cycle == 1)
			{
				tvArrSup.RemoveAt(2, tvArrSup.GetSize()-2);
			}
			CRebarPlacing rb;
			rb.ReverseRebar(tvArr);
			MarkRebarMain_Plane_LowerSlab_ForVert(&Dom, tvArr, tvArrTmp, tvArrTmp, tvArrSup, 0, TRUE, 0, cycle, FALSE, bStt, !bStt, nDrawLeft);
		}
	}

	*pDomP << Dom;
}

void CDrawPlane::DrawPlaneGenLowerSlabPRF( CDomyun *pDomP, BOOL bStt, BOOL bCuttingCircle, BOOL bHidden/*=FALSE*/ )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, bHidden ? HCAD_CONH : HCAD_CONC);

	CTwinVectorArray tvArr;
	pBri->GetTvPlaneOutWall(tvArr, bStt, FALSE, FALSE, TRUE);

	if(bCuttingCircle)
	{
		ApplyCuttingArea_Plane(&Dom, tvArr, TRUE, 0, FALSE, FALSE, bStt, !bStt, FALSE, 0, FALSE, FALSE, NULL, FALSE);
	}
	baseDraw.DrawTvArray(&Dom, tvArr);

	*pDomP << Dom;
}

void CDrawPlane::MarkRebarMain_Plane_LowerSlab_ForVert( CDomyun *pDomP, CTwinVectorArray &tvArrMain, CTwinVectorArray &tvArrMainLeft, CTwinVectorArray &tvArrMainRight, CTwinVectorArray &tvArrSup, long nIdxRC, BOOL bUpper, long nDan, long nCycle, BOOL bHunch, BOOL bSttPRF, BOOL bEndPRF, long nDrawLeft )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(nIdxRC < 0) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CRebarPlacing rb;

	if(bHunch)
		rb.ReverseRebar(tvArrMain);

	CRebarInfoCycle *pRC	= NULL;
	long nCountRC	= 1;
	if(!bHunch && !bSttPRF && !bEndPRF)
	{
		long upper	= bUpper ? iUPPER : iLOWER;
		pRC	= pBri->GetRebarInfoCycleLowerSlab(nCycle, nDan, bUpper, nIdxRC);
		nCountRC	= pBri->m_pArrRCMainLowerSlab[nCycle][upper][nDan].GetSize();
	}
	else
	{
		if(bHunch)
		{
			if(pBri->m_pArrRCMainLowerSlabHunch[nCycle].GetSize() <= nIdxRC)
				return;
			pRC	= pBri->m_pArrRCMainLowerSlabHunch[nCycle].GetAt(nIdxRC);
		}
		else if(bSttPRF || bEndPRF)
		{
			long stt	= bSttPRF ? iSTT : iEND;
			CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = nCycle == 0 ? &pBri->m_pArrRCMainBoxLowerPRF_Cycle1[stt] : &pBri->m_pArrRCMainBoxLowerPRF_Cycle2[stt];
			if(pArrRC->GetSize() <= nIdxRC) 
				return;
			pRC	= pArrRC->GetAt(nIdxRC);
		}
	}

	if(!pRC) return;


	// 어디로 뻗을지 결정 해서 선 가져옴
	BOOL bExtendToSttOfMain	= TRUE;
	CString sStt	= _T("시점");
	if(!bHunch && !bSttPRF && bEndPRF)
	{
		sStt	= _T("종점");
	}
	// 일반 주철근이지만, 종점쪽에 가까우면 종점에 마킹한다.
	else if((pRC->m_xyStt.x+pRC->m_xyEnd.x)/2 > pBri->m_dStationBridgeStt+pBri->m_dLengthBridge/2)
	{
		sStt	= _T("종점");
		bExtendToSttOfMain	= FALSE;
	}


	CTwinVectorArray tvArrPlane, tvArrEnd, tvArrLeft, tvArrRight;
	CTwinVector tv;
	pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1);
	long i = 0; for(i = 0; i < tvArrPlane.GetSize(); i++)
	{
		tv	= tvArrPlane.GetAt(i);
		if(tv.m_sInfo == sStt) tvArrEnd.Add(tv);
		else if(tv.m_sInfo == "좌측") tvArrLeft.Add(tv);
		else if(tv.m_sInfo == "우측") tvArrRight.Add(tv);
	}

	long dCountCycle	= pBri->GetCountCycleMainRebar();

	// 직각방향 철근
	// 중요 : 상부슬래브 주철근일 경우 사보강(절반)을 선택하면 좌우로 끊어진 철근이 존재 할 수 있다.
	// 따로 마킹해준다. (좌측은 "-3", 우측은 "-4"를 붙여준다.
	long nIdxStt	= 0;
	long nIdxEnd	= !bSttPRF && !bEndPRF ? 2 : 0;

	CRebarInfoCycle *pRCForMark	= pBri->GetOwerRebarInfoCycleSlab(FALSE, nCycle, nDan, bUpper, nIdxRC);

	for(i = nIdxStt; i <= nIdxEnd; i++)
	{
		CString sMark	= pRCForMark ? pRCForMark->m_szMark : pRC->m_szMark;
		CString sDescription	 = pRCForMark ? pRCForMark->m_sDescription : pRC->m_sDescription;

		if(bSttPRF || bEndPRF)
		{
			sDescription.Format("부상방지저판 주철근", m_pStd->m_pARcBridgeDataStd->GetStringCircleNum(nCycle+1));
			sMark	= pBri->GetMarkRebarMain(0, FALSE, 0, FALSE, FALSE, sDescription);
		}

		if(tvArrMain.GetSize() > 0 && m_pStd->IsMarkRebar(sMark))
		{
			CMarkDomyun mark;//(&Dom, m_pStd->m_pARoadOptionStd);
			mark.SetDomyun(&Dom, m_pStd->m_pARoadOptionStd);
			if(i == 0)
				mark.SetRebar(tvArrMain, tvArrSup);
			else if(i == 1)
				mark.SetRebar(tvArrMainLeft, tvArrSup);
			else if(i == 2)
				mark.SetRebar(tvArrMainRight, tvArrSup);

			// 설정
			if(i == 0)
				mark.m_sMark					= sMark;
			else
				mark.m_sMark.Format("%s-%d", sMark, i+2);				

			mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pBri->GetValueFy(ePartLowerSlab));
			mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
			mark.m_dPosScaleMarkStt			= 1./(dCountCycle+nCountRC) * (nCycle+nIdxRC+1);
			mark.m_dPosScale				= 1./(dCountCycle+nCountRC) * (nCycle+nIdxRC+1);
			mark.m_dPosScaleSpace			= 0.5 - (1./dCountCycle)/2;
			mark.m_xyDirCountMark			= pBri->GetAngleAzimuthJijum(0).Rotate(CDPoint(-1,0));

			if(i == 1)
			{
				mark.m_nTypeMarkPosLineRebar			= TYPE_MARKPOS_END;
			}
			else if(i == 2)
			{
				mark.m_nTypeMarkPosLineRebar			= TYPE_MARKPOS_STT;
			}
			else
			{
				mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_POS;
				mark.m_nTypeMarkDir				= TYPE_MARKDIR_RIGHTTAN;
				if(bExtendToSttOfMain)
					mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_UPPER;
				else 
					mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_LOWER;

				if(bSttPRF || bEndPRF)
				{
					if(tvArrSup.GetSize() > 1)
					{
						CTwinVector tv[2]	= {tvArrSup.GetAt(0), tvArrSup.GetAt(1)};
						CTwinVector tvMid	= tv[0];
						tvMid.m_v1			= (tv[0].m_v1 + tv[1].m_v1)/2;
						tvMid.m_v2			= (tv[0].m_v2 + tv[1].m_v2)/2;
						tv[nCycle]			= tvMid;
						tvArrSup.RemoveAll();
						tvArrSup.Add(tv[0]);
						tvArrSup.Add(tv[1]);
						mark.SetRebar(tvArrMain, tvArrSup);
					}

					mark.m_nTypeMarkDir				= nCycle == 0 ? TYPE_MARKDIR_LEFTTAN : TYPE_MARKDIR_RIGHTTAN;
					mark.m_dPosScaleMarkStt			= nCycle == 0 ? 0.25 : 0.75;
					mark.m_dPosScale				= 0.5;
					mark.m_dPosScaleSpace			= 0.25;
				}
			}

			// 마크선 연장
			CDPoint xyMark					= mark.GetXyMark();	// 이 함수에서 마크할 위치가 구해짐
			CDPoint xyMatch(0, 0);
			CDPointArray xyArrMatch;
			if(bSttPRF || bEndPRF)
			{
				pBri->GetTvPlaneOutWall(tvArrEnd, bSttPRF, FALSE, FALSE);
			}

			if(mark.m_xyArrCountMark.GetSize() > 1)
			{
				CDPoint xyDir	= (mark.m_xyArrCountMark.GetAt(0) - mark.m_xyArrCountMark.GetAt(1)).Unit();
				if(i == 0)
					rb.GetXyMatchTvArrAndLine(tvArrEnd, xyMark, tvArrMain.GetAt(0).GetXyDir(), xyArrMatch);
				else if(i == 1)
					rb.GetXyMatchTvArrAndLine(tvArrLeft, xyMark, xyDir, xyArrMatch);
				else if(i == 2)
					rb.GetXyMatchTvArrAndLine(tvArrRight, xyMark, xyDir, xyArrMatch);
			}

			if(xyArrMatch.GetSize() > 0)
			{
				xyMatch	= xyArrMatch.GetAt(0);
				mark.m_dAddHeight				= ~(xyMark - xyMatch) + Dom.Always(MARK_BASE_HEIGHT);
			}

			if(!bSttPRF && !bEndPRF && i == 0)
			{
				if(nDan > 0)
					mark.m_dAddHeight	+= Dom.Always(MARK_BASE_HEIGHT);

				if(nCycle > 0)
					mark.m_dAddHeight	+= Dom.Always(MARK_BASE_HEIGHT);
			}

			if(!bExtendToSttOfMain && i == 0)
				mark.m_dAddHeight	+= Dom.Always(MARK_BASE_HEIGHT);

			if(bEndPRF)
			{
				if(nCycle == 0)
					mark.m_dAddHeight	+= pBri->m_dWE/4*3;
				mark.m_dAddHeight -= pBri->m_dWE;
			}

			// 다 구한뒤 마크할 위치에 대해서 절단원 적용
			//ApplyCuttingArea_Plane(&Dom, mark.m_xyArrCountMark, bUpper, nDan, FALSE, FALSE, bSttBracket, bEndBracket, bHunch, pRC->m_nPosHunch);

			if(mark.m_xyArrCountMark.GetSize() > 0)
			{
				CRebar *pRb = pBri->GetRebarMainByDescription(sDescription);

				mark.m_dCtc = pBri->GetCtcRebarMain(FALSE);
				mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;
				mark.MarkRebar(i == 0 ? FALSE : TRUE);	
			}
		}
	}

	*pDomP << Dom;
}

void CDrawPlane::DrawMainRebarUpperSlabByTvArr( CDomyun* pDomP, CTwinVectorArray &tvArr, CTwinVectorArray &tvArrAngLeft, CTwinVectorArray &tvArrAngRight, CTwinVectorArray &tvArrAngLeftHalf, CTwinVectorArray &tvArrAngRightHalf, CTwinVectorArray &tvArrJoint, CTwinVectorArray &tvArrHidden )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	CHgBaseDrawStd baseDraw;

	BOOL bVertDir	= pBri->IsVertDir();
	BOOL bExtLine	= pBri->IsExtLine();

	m_pStd->m_pARoadOptionStd->SetEnvType(pDomP, HCAD_BARC);
	m_pStd->DrawTvArrWidthHidden(pDomP, tvArr, pBri->GetLineBase());
	m_pStd->DrawTvArrWidthHidden(pDomP, tvArrAngLeft, pBri->GetLineLeftRight(TRUE));
	m_pStd->DrawTvArrWidthHidden(pDomP, tvArrAngRight, pBri->GetLineLeftRight(FALSE));

	m_pStd->m_pARoadOptionStd->SetEnvType(pDomP, HCAD_BARA);	
	if((pBri->m_nTypeSideRebarMain_UpperSlabSabogang == 0 || !bVertDir) && !bExtLine)
	{
		baseDraw.m_pLine	= pBri->GetLineBase(bVertDir);
		baseDraw.DrawTvArray(pDomP, tvArrJoint);
	}
	else
	{
		// 사보강배치에 좌우측선형형태 철근이면 각각의 선형별 이음을 구분 해야 된다.
		CTwinVectorArray tvArrJointCen, tvArrJointLeft, tvArrJointRight;
		long i = 0; for(i = 0; i < tvArrJoint.GetSize(); i++)
		{
			CTwinVector tv	= tvArrJoint.GetAt(i);
			if(tv.m_sInfoHor == "좌측이음") tvArrJointLeft.Add(tv);
			else if(tv.m_sInfoHor == "우측이음") tvArrJointRight.Add(tv);
			else tvArrJointCen.Add(tv);
		}

		baseDraw.m_pLine	= pBri->GetLineLeftRight(TRUE);
		baseDraw.DrawTvArray(pDomP, tvArrJointLeft);
		baseDraw.m_pLine	= pBri->GetLineLeftRight(FALSE);
		baseDraw.DrawTvArray(pDomP, tvArrJointRight);
		baseDraw.m_pLine	= pBri->GetLineBase(bVertDir);
		baseDraw.DrawTvArray(pDomP, tvArrJointCen);
	}

	m_pStd->m_pARoadOptionStd->SetEnvType(pDomP, HCAD_BARC);
	if((pBri->m_nTypeSideRebarMain_UpperSlabSabogang == 0 || !bVertDir) && !bExtLine)
	{
		m_pStd->DrawTvArrWidthHidden(pDomP, tvArrHidden, pBri->GetLineBase(bVertDir));
	}
	else
	{
		// 사보강배치에 좌우측선형형태 철근이면 각각의 선형별 이음을 구분 해야 된다.
		CTwinVectorArray tvArrHiddenCen, tvArrHiddenLeft, tvArrHiddenRight;
		long i = 0; for(i = 0; i < tvArrHidden.GetSize(); i++)
		{
			CTwinVector tv	= tvArrHidden.GetAt(i);
			if(tv.m_sInfoHor == "좌측점선") tvArrHiddenLeft.Add(tv);
			else if(tv.m_sInfoHor == "우측점선") tvArrHiddenRight.Add(tv);
			else tvArrHiddenCen.Add(tv);
		}
		m_pStd->DrawTvArrWidthHidden(pDomP, tvArrHiddenLeft, pBri->GetLineLeftRight(TRUE));
		m_pStd->DrawTvArrWidthHidden(pDomP, tvArrHiddenRight, pBri->GetLineLeftRight(FALSE));
		m_pStd->DrawTvArrWidthHidden(pDomP, tvArrHiddenCen, pBri->GetLineBase(bVertDir));
	}

	m_pStd->m_pARoadOptionStd->SetEnvType(pDomP, HCAD_BARC);
	m_pStd->DrawTvArrWidthHidden(pDomP, tvArrAngLeftHalf, pBri->GetLineLeftRight(TRUE));
	m_pStd->DrawTvArrWidthHidden(pDomP, tvArrAngRightHalf, pBri->GetLineLeftRight(FALSE));
}

void CDrawPlane::DrawPlaneLane(CDomyun* pDomP, HGBOOL bRoadNotSlab)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if (pBri == NULL)
		return;

	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);
	
	const HGINT32 icHDan = pBri->GetQtyHDan(bRoadNotSlab);
	const HGINT32 icLane = bRoadNotSlab ? icHDan + 1 : icHDan;
	const HGINT32 iStt = bRoadNotSlab ? 0 : 1;
	for (HGINT32 ixHDan = iStt; ixHDan < icLane; ++ixHDan)
	{
		CTwinVectorArray tvArr;

		if (pBri->GetTvArrayLaneForPlane(ixHDan, tvArr, bRoadNotSlab) == FALSE)
			continue;

		baseDraw.DrawTvArray(&Dom, tvArr, m_pStd->IsViewVectorInfo());
	}

	*pDomP << Dom;
}

void CDrawPlane::DrawPlaneNori(CDomyun* pDomP)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if (pBri == NULL)
		return;

	if (pBri->m_bUnderGroundRahmen == FALSE)
		return;

	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);

	if (pBri->m_nSeparBri == 0 || pBri->m_nSeparBri == 1)
	{
		CTwinVectorArray tvArr1;
		pBri->GetTvArrayNoriOutLineForPlane(tvArr1, TRUE);
		baseDraw.DrawTvArray(&Dom, tvArr1, m_pStd->IsViewVectorInfo());

		CTwinVectorArray tvArrLane1;
		pBri->GetTvArrayLaneForPlane(0, tvArrLane1, TRUE);

		DrawPlaneNoriRuler(&Dom, tvArrLane1, tvArr1, FALSE);
	}
	
	if (pBri->m_nSeparBri == 0 || pBri->m_nSeparBri == 2)
	{
		CTwinVectorArray tvArr2;
		pBri->GetTvArrayNoriOutLineForPlane(tvArr2, FALSE);
		baseDraw.DrawTvArray(&Dom, tvArr2, m_pStd->IsViewVectorInfo());

		const HGINT32 icHDan = pBri->GetQtyHDan(TRUE);
		CTwinVectorArray tvArrLane2;
		pBri->GetTvArrayLaneForPlane(icHDan, tvArrLane2, TRUE);


		DrawPlaneNoriRuler(&Dom, tvArrLane2, tvArr2, TRUE);
	}	

	*pDomP << Dom;
}

void CDrawPlane::DrawPlaneNoriRuler(CDomyun *pDom, CTwinVectorArray &tvArrLane, CTwinVectorArray &tvArrNori, HGBOOL bCCW)
{
	CDomyun Dom(pDom);

	HGINT32 iCnt = 0;
	CRebarPlacing rb;
	const HGDOUBLE dLenLane = tvArrLane.GetLength();

	for (HGDOUBLE dPos = 0; dPos < dLenLane; dPos += 1000)
	{
		long nIndex = 0;
		CDPoint dpStt = tvArrLane.GetXyDistFromStt(dPos);
		if (tvArrLane.GetTvSelect(dpStt, nIndex) == FALSE)
			continue;

		CTwinVector tv = tvArrLane.GetAt(nIndex);
		CVector vDir = bCCW ? tv.GetXyDir().RotateInv90() : tv.GetXyDir().Rotate90();
		CDPoint dpEnd = dpStt + vDir * 1000;

		if (iCnt == 5)
		{
			iCnt = 0;

			CDPointArray dpArr;
			rb.GetXyMatchTvArrAndLine(tvArrNori, dpStt, vDir, dpArr);
			if (dpArr.IsEmpty() == FALSE)
			{
				dpEnd = dpArr.GetAt(0);
			}
		}

		Dom.MoveTo(dpStt);
		Dom.LineTo(dpEnd);

		++iCnt;
	}

	*pDom << Dom;
}

CDRect CDrawPlane::DrawRebar_Plane_SlabHunch( CDomyun *pDomP, BOOL bUpperSlab, BOOL bDimRebar, BOOL bMarkRebar, BOOL bRotate, long nDirMark/*=2*/ )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return CDRect(0, 0, 0, 0);
	m_pStd->SetMarkForMarkRebar("");
	CLineInfo *pLine    = pBri->GetLineBase();
	CDomyun Dom(pDomP);

	CHgBaseDrawStd baseDraw;
	// 평면도 일반도
	CTwinVectorArray tvArrPlane;
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	
	pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1, TRUE);
	baseDraw.DrawTvArray(&Dom, tvArrPlane);
	
	// 상부 슬래브 일때는 슬래브 하단측면선 표현해준다.
	if(bUpperSlab)
	{
		CTwinVectorArray tvArrLeft;
		tvArrPlane.GetTvArrByInfo(_T("좌측"), tvArrLeft, FALSE, FALSE, FALSE);
		if(tvArrLeft.GetSize() > 0)
		{
			tvArrLeft.Sort();
			CTwinVector tvStt, tvEnd;
			CTwinVectorArray tvArrTemp, tvArrTemp2;
			tvArrPlane.GetTvArrByInfo(_T("시점"), tvArrTemp, FALSE, FALSE, FALSE);
			tvArrTemp.GetTvArrByInfo(_T("가각"), tvArrTemp2, TRUE , FALSE, FALSE); 
			tvStt.m_v1 = tvArrTemp.GetAt(0).m_v1;
			tvStt.m_v2 = tvArrTemp.GetAt(tvArrTemp.GetSize()-1).m_v2;

			tvArrPlane.GetTvArrByInfo(_T("종점"), tvArrTemp, FALSE, FALSE, FALSE);
			tvArrTemp.GetTvArrByInfo(_T("가각"), tvArrTemp2, TRUE , FALSE, FALSE);
			tvEnd.m_v1 = tvArrTemp.GetAt(0).m_v1;
			tvEnd.m_v2 = tvArrTemp.GetAt(tvArrTemp.GetSize()-1).m_v2;

			CRebarPlacing rb;
			// CTwinVectorArray::AddFromLineSplit() 함수의 문제로 시종점이 바뀌어 있는 경우가 있다.
			if(~(tvStt.GetXyMid() - tvArrLeft.GetAt(0).m_v1) > ~(tvStt.GetXyMid() - tvArrLeft.GetAt(tvArrLeft.GetSize()-1).m_v2))
			{
				rb.ReverseRebar(tvArrLeft);
			}
			rb.GetOffsetTvArray(tvArrLeft, pBri->m_dDH[iLEFT]);

			CTwinVectorArray tvArrCopy;
			tvArrCopy.AddFromTvArray(tvArrLeft);

			double dExt = 5000;
			double dStaSttOri(0), dStaEndOri(0), dStaSttCurr(0), dStaEndCurr(0);

			
			pLine->GetStationMatchVerticalByXy(tvArrLeft.GetAt(0).m_v1, dStaSttOri);
			pLine->GetStationMatchVerticalByXy(tvArrLeft.GetAt(tvArrLeft.GetSize()-1).m_v2, dStaEndOri);

			rb.StretchTvArr(tvArrLeft, dExt,dExt);
			rb.TrimTvArrayByTv(tvArrLeft, &tvStt, TRUE);
			rb.TrimTvArrayByTv(tvArrLeft, &tvEnd, TRUE);

			pLine->GetStationMatchVerticalByXy(tvArrLeft.GetAt(0).m_v1, dStaSttCurr);
			pLine->GetStationMatchVerticalByXy(tvArrLeft.GetAt(tvArrLeft.GetSize()-1).m_v2, dStaEndCurr);
			dStaSttCurr -= 500;		
			dStaEndCurr += 500;
			if(Compare(dStaSttOri, dStaSttCurr, _T("<"),10) || Compare(dStaEndOri, dStaEndCurr, _T(">"),10))
			{
				tvArrLeft.RemoveAll();
				tvArrLeft.AddFromTvArray(tvArrCopy);

				CTwinVector *pTv = tvArrLeft.GetPAt(0);
				GetXyMatchLineAndLine(tvStt.m_v1, tvStt.GetXyDir(), pTv->m_v2, pTv->GetXyDir(), pTv->m_v1);
				pTv = tvArrLeft.GetPAt(tvArrLeft.GetSize()-1);
				GetXyMatchLineAndLine(tvEnd.m_v1, tvEnd.GetXyDir(), pTv->m_v1, pTv->GetXyDir(), pTv->m_v2);
			}
			

			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONH);
			baseDraw.DrawTvArray(&Dom, tvArrLeft);
		}
	}
	if(bUpperSlab)
	{
		CTwinVectorArray tvArrRight;
		tvArrPlane.GetTvArrByInfo(_T("우측"), tvArrRight, FALSE, FALSE, FALSE);

		if(tvArrRight.GetSize() > 0)
		{
			CTwinVector tvStt, tvEnd;
			CTwinVectorArray tvArrTemp, tvArrTemp2;
			tvArrPlane.GetTvArrByInfo(_T("시점"), tvArrTemp, FALSE, FALSE, FALSE);
			tvArrTemp.GetTvArrByInfo(_T("가각"), tvArrTemp2, TRUE, FALSE, FALSE);
			tvStt.m_v1 = tvArrTemp.GetAt(0).m_v1;
			tvStt.m_v2 = tvArrTemp.GetAt(tvArrTemp.GetSize()-1).m_v2;

			tvArrPlane.GetTvArrByInfo(_T("종점"), tvArrTemp, FALSE, FALSE, FALSE);
			tvArrTemp.GetTvArrByInfo(_T("가각"), tvArrTemp2, TRUE, FALSE, FALSE);
			tvEnd.m_v1 = tvArrTemp.GetAt(0).m_v1;
			tvEnd.m_v2 = tvArrTemp.GetAt(tvArrTemp.GetSize()-1).m_v2;

			CRebarPlacing rb;
			// CTwinVectorArray::AddFromLineSplit() 함수의 문제로 시종점이 바뀌어 있는 경우가 있다.
			if(~(tvStt.GetXyMid() - tvArrRight.GetAt(0).m_v1) > ~(tvStt.GetXyMid() - tvArrRight.GetAt(tvArrRight.GetSize()-1).m_v2))
			{
				rb.ReverseRebar(tvArrRight);
			}
			tvArrRight.Sort();
			rb.GetOffsetTvArray(tvArrRight, -pBri->m_dDH[iRIGHT]);

			//////
			CTwinVectorArray tvArrCopy;
			tvArrCopy.AddFromTvArray(tvArrRight);

			double dExt = 5000;
			double dStaSttOri(0), dStaEndOri(0), dStaSttCurr(0), dStaEndCurr(0);


			pLine->GetStationMatchVerticalByXy(tvArrRight.GetAt(0).m_v1, dStaSttOri);
			pLine->GetStationMatchVerticalByXy(tvArrRight.GetAt(tvArrRight.GetSize()-1).m_v2, dStaEndOri);

			rb.StretchTvArr(tvArrRight, dExt,dExt);
			rb.TrimTvArrayByTv(tvArrRight, &tvStt, TRUE);
			rb.TrimTvArrayByTv(tvArrRight, &tvEnd, TRUE);

			pLine->GetStationMatchVerticalByXy(tvArrRight.GetAt(0).m_v1, dStaSttCurr);
			pLine->GetStationMatchVerticalByXy(tvArrRight.GetAt(tvArrRight.GetSize()-1).m_v2, dStaEndCurr);
			dStaSttCurr -= 500;		
			dStaEndCurr += 500;
			if(Compare(dStaSttOri, dStaSttCurr, _T("<"),10) || Compare(dStaEndOri, dStaEndCurr, _T(">"),10))
			{
				tvArrRight.RemoveAll();
				tvArrRight.AddFromTvArray(tvArrCopy);

				CTwinVector tv = tvArrRight.GetAt(0);
				GetXyMatchLineAndLine(tvStt.m_v1, tvStt.GetXyDir(), tv.m_v1, tv.GetXyDir(), tv.m_v1);
				tvArrRight.SetAt(0, tv);
				tv = tvArrRight.GetAt(tvArrRight.GetSize()-1);
				GetXyMatchLineAndLine(tvEnd.m_v1, tvEnd.GetXyDir(), tv.m_v2, tv.GetXyDir(), tv.m_v2);
				tvArrRight.SetAt(tvArrRight.GetSize()-1, tv);
			}
			///////

			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONH);
			baseDraw.DrawTvArray(&Dom, tvArrRight);
		}
	}
	
	CDRect rectTot;
	if(bRotate)
	{
		m_pStd->RotateDomByBridgeSttEndXy(&Dom);
		Dom.SetCalcExtRect();
		rectTot	= Dom.GetExtRect();
		*pDomP << Dom;
	}

	// 배근도 구하기 전 준비 작업 (속도 개선을 위한 중복작업 일괄 처리 )
	pBri->SettingValueRebarForSlab();
	////////////////////////////////////////////////////////////////////

	// 평면도 거더철근/헌치부철근 배력/주철근
	// 교대,교각부, 기둥부 거더 철근 모두 그림
	for(long nJ = 0; nJ < pBri->GetCountJijum(); nJ++)
	{
		// 헌치부 철근 (배력철근)
		if(bUpperSlab)
			DrawRebarSupport_Plane_UpperSlabGirderAndHunch(&Dom, nJ, FALSE, 0, FALSE, FALSE, TRUE, 0, FALSE);
		else
			DrawRebarSupport_Plane_LowerSlabGirderAndHunch(&Dom, nJ, TRUE, 0, FALSE, FALSE, TRUE, 0, FALSE);

		// 헌치부 철근 (주철근)
		for(long cycle = 0; cycle < 2; cycle++)
		{
			if(bUpperSlab)
				DrawRebarMain_Plane_UpperSlabHunch(&Dom, cycle, nJ, FALSE, 0, TRUE, TRUE, 0, nDirMark, FALSE);
			else
				DrawRebarMain_Plane_LowerSlabHunch(&Dom, cycle, nJ, TRUE, 0, TRUE, TRUE, 0, nDirMark, FALSE);
		}
	}

	if(bRotate)
		m_pStd->RotateDomByBridgeSttEndXy(&Dom);

	*pDomP << Dom;

	// 배근도 구하기 전 준비 작업한거 초기화 (속도 개선을 위한 중복작업 일괄 처리 )
	pBri->InitValueRebarForUpperSlab();
	////////////////////////////////////////////////////////////////////

	return rectTot;
}
