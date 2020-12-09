// DrawWingWall.cpp: implementation of the CDrawWingWall class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARcBridgeDBDraw.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define MARK_DIST 400

CDrawWingWall::CDrawWingWall()
{

}

CDrawWingWall::~CDrawWingWall()
{

}

void CDrawWingWall::DrawGenCutBriSect(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut, BOOL bRebar)
{
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	int se = bStt ? -1 : 1;
//	int nLeft = bLeft ? -se : se;	// 시점일 경우 교량 진행 방향의 좌우와 날개벽의 좌우가 반대임.
//	pBri->GetTvVertSection(nLeft, TRUE, TRUE);		// 날개벽 각도로 재계산
	//pBri->GetTvVertSection(nLeft, TRUE, FALSE);		// 날개벽 각도로 재계산 안함

	CDRect rectBridge;
	rectBridge = pBri->m_tvArrBriSect.GetRect();
	
	CTwinVector tvWing;
	CTwinVectorArray tvArrWall, tvArrWing;
//  날개벽을 그릴때는 이미 종단타입을 맞춰놨을테니 다시 하지 말자.. 두단계이상 들어가서 복구가 잘못되고 있다.
//	pBri->GetTvVertSection_Std((bLeft==bStt)? 1 : -1, TRUE, TRUE, FALSE, 2);
	tvArrWall = pBri->m_tvArrBriSect;

	//#38590 BOx형에서 선형이 곡선인경우 두께가 맞지 않음.. #
	//L2가있는경우는 상관없음. 시점벽체아래선을 연장, 또는 잘라줌
	if(pBri->IsBoxType() && pWing->m_dL2 == 0)
	{
		CString strBottom = bStt ? _T("하부슬래브하면(시점벽체아래)") : _T("하부슬래브하면(종점벽체아래)");
		CTwinVectorArray tvArrWing;
		CTwinVector tvHori, tvVert;
		CDPoint xyMatch;

		pBri->GetTvWingWallFront(bStt, bLeft, nOut, tvArrWing);
		tvArrWing.GetTvByInfo(_T("날개벽정면끝단하부"), tvVert);
		
		//슬래브하면 벽체 아래 Index찾기
		long ix = 0;
		for(ix = 0; ix < tvArrWall.GetSize(); ix ++)
		{
			if(tvArrWall.GetAt(ix).m_sInfo == strBottom)
			{
				tvHori = tvArrWall.GetAt(ix);
				break;
			}
			
		}
		//교차점 찾아서 슬래브하면 벽체 좌표 세팅
		if(GetXyMatchLineAndLine(tvHori.m_v1, CDPoint(1, 0), tvVert.m_v1, CDPoint(0, 1), xyMatch))
		{
			if(bStt) tvHori.m_v1.x = xyMatch.x;
			else	tvHori.m_v2.x = xyMatch.x;
			
			tvArrWall.SetAt(ix, tvHori);
		}

	}

	pBri->ModifyVertSectionForWing(tvArrWall, bStt, bLeft);

	BOOL bDrawCutLine = TRUE;
	CDRect rect = tvArrWall.GetRect();
	double dSpare = 2000.0;
	
	if(bStt)
	{
		rect.left -= 100;
		rect.right = rect.left + dSpare;
	}
	else
	{
		rect.right += 100;
		rect.left = rect.right - dSpare;
	}
	rect.top = rectBridge.top + 100;
	rect.bottom -= 100;

	//BOX경우에 바깥쪽 벽체도 rect에 포함되므로 안쪽벽체 좌표로 다시 세팅.
	CDPoint xyInStt, xyInEnd;
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		xyInStt = pBri->GetXySectionWingWallStt(bStt, TRUE);
		xyInEnd = pBri->GetXySectionWingWallStt(bStt, FALSE);

		if(bStt) rect.right = xyInStt.x + dSpare;
		else rect.left = xyInEnd.x - dSpare;

		bDrawCutLine = FALSE;
	}
	
	CFootingApp* pFooting = NULL;
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
		pFooting = &pBri->m_footingBox;
	else if(bStt)
		pFooting = &pBri->m_footingStt;
	else
		pFooting = &pBri->m_footingEnd;
	if(!pFooting) return;

	
	CDPoint vAng = pBri->GetAngleWingWallBetweenWall(bStt, bLeft);
	
	BOOL bSide = (pWing->m_nAttachPos == 0)? FALSE : TRUE;
	/*double dXCutLine = 0;*/

	CDomyun Dom(pDomP);
	pBri->GetTvWingWallPlane(bStt);
	if(bSide)
	{
		BOOL bHeel = FALSE; // 앞굽뒷굽 상관없이 날개벽 방향으로 구하므로 무조건 FALSE // (vAng.GetAngleDegree() < 180)? FALSE : TRUE;

//		double dW = (bStt ? pBri->m_dWS : pBri->m_dWE)/vAng.y;
		double dFB = pBri->GetLengthFootingWingWall(bStt, bLeft, FALSE, bHeel, nOut);		// 뒷굽 길이
		double dFH = pFooting->m_dFH + ((bStt)? pFooting->m_dFSRH:pFooting->m_dFSLH);
		double dEB = pBri->GetLengthFootingWingWall(bStt, bLeft, TRUE, bHeel, nOut);		// 뒷굽 길이
		double dEH = pFooting->m_exFooting.m_dEFHR;

		pBri->GetTvWingWallFront(bStt, bLeft, nOut, tvArrWing);
		tvArrWing.GetTvByInfo(_T("날개벽정면하단"), tvWing);

		CDPoint xyStt = pBri->GetXySectionWingWallStt(bStt, TRUE);
		CDPoint xyEnd = pBri->GetXySectionWingWallStt(bStt, FALSE);
		xyStt.y += pWing->m_dH3;
		Dom.LineTo(xyStt, CDPoint(xyStt.x-dSpare*se, xyStt.y));
		Dom.LineTo(xyEnd.x+dFB*se, xyEnd.y, xyEnd.x-dSpare*se, xyEnd.y);
		Dom.LineTo(xyEnd.x+dFB*se, xyEnd.y, xyEnd.x+dFB*se, xyEnd.y-dFH);
		
		// 날개벽이 기초 밖으로 나갈경우 날개벽 하면은 따로 그려준다.
		if(!pBri->IsBoxType())
			Dom.LineTo(tvWing.m_v1, CDPoint(xyEnd.x-dSpare*se, xyEnd.y));
		
		//Box형일때는 정면하단부터 확대기초까지 그려준다.
		if(pBri->IsBoxType() && !pWing->m_dH2)
			Dom.LineTo(xyEnd.x, xyEnd.y, tvWing.m_v1.x, tvWing.m_v2.y);

		if(!bRebar)		// 철근 구조도 에서는 확대기초는 표시하지 않는다.
		{
			Dom.LineTo(xyEnd.x+dEB*se, xyEnd.y-dFH, xyEnd.x-dSpare*se, xyEnd.y-dFH);
			Dom.LineTo(xyEnd.x+dEB*se, xyEnd.y-dFH, xyEnd.x+dEB*se, xyEnd.y-(dFH+dEH));
			Dom.LineTo(xyEnd.x+dEB*se, xyEnd.y-(dFH+dEH), xyEnd.x-dSpare*se, xyEnd.y-(dFH+dEH));
		}
		else
		{
			Dom.LineTo(xyEnd.x+dFB*se, xyEnd.y-dFH, xyEnd.x-dSpare*se, xyEnd.y-dFH);
		}
		

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CUTL);
		Dom.CutLightning(xyEnd.x-dSpare*se, xyEnd.y-(dFH+dEH)-Dom.Always(5), xyStt.x-dSpare*se, xyStt.y+Dom.Always(5));
	}
	else
	{
		double dW = (bStt ? pBri->m_dWS : pBri->m_dWE)/vAng.y;
		double dFF = pBri->GetLengthFootingWingWall(bStt, bLeft, FALSE, FALSE, nOut);		// 앞굽 길이
		double dFB = pBri->GetLengthFootingWingWall(bStt, bLeft, FALSE, TRUE, nOut);		// 뒷굽 길이
		double dFH = pFooting->m_dFH + ((bStt)? pFooting->m_dFSRH:pFooting->m_dFSLH);
		double dEF = pBri->GetLengthFootingWingWall(bStt, bLeft, TRUE, FALSE, nOut);		// 앞굽 길이
		double dEB = pBri->GetLengthFootingWingWall(bStt, bLeft, TRUE, TRUE, nOut);		// 뒷굽 길이
		double dEH = pFooting->m_exFooting.m_dEFHR;
		double dL2 = pWing->m_dL2;
		
		// 날개벽은 좌우측 편경사 적용된 종단으로 그려야 되지만, 현재 선형에는 중심기준으로만 종단을 그리므로
		// 잘리는 부분과 날개벽 시작되는 부분에 대해서는 종단을 직선으로 따로 구해서 넣어준다.
		if(bStt)
		{
			long nIdx	= tvArrWall.GetIdxByInfo("슬래브상면(외측벽체안쪽)");
			if(nIdx > -1)
			{
				CTwinVector tv	= tvArrWall.GetAt(nIdx);
				tv.m_v1.z	= 0;
		
				tvArrWall.SetAt(nIdx, tv);
			}
		}
		else
		{
			long nIdx	= tvArrWall.GetIdxByInfo("슬래브상면(외측벽체안쪽)");
			if(nIdx > -1)
			{
				CTwinVector tv	= tvArrWall.GetAt(nIdx);
		
				tvArrWall.SetAt(nIdx, tv);
			}
			
		}
		
		CDRect rectTmp	= tvArrWall.GetRect();
		rect.top = rectTmp.top + 100;
		rect.bottom		= rectTmp.bottom - 100;
		
		pBri->GetTvWingWallFront(bStt, bLeft, nOut, tvArrWing);
		tvArrWing.GetTvByInfo(_T("날개벽정면하단"), tvWing);

		// 벽체부
		m_pStd->DrawCuttingTvArray(&Dom, tvArrWall, rect, bDrawCutLine);
		//pBri->GetTvVertSection(nLeft, TRUE);		// 다시 되돌림
		
		//BOX일경우cutLine따로 그려줌.
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
		{
			double se = bStt? 1 : -1;
			CTwinVector tvLowerSlabTop, tvTopSlabBottom;
			CDPoint xyLowerSlab, xyTopSlab;
			pBri->m_tvArrBriSect.GetTvByInfo(_T("하부슬래브상면"), tvLowerSlabTop);
			pBri->m_tvArrBriSect.GetTvByInfo(_T("슬래브하면"), tvTopSlabBottom);
			GetXyMatchSegAndLine(tvLowerSlabTop.m_v1, tvLowerSlabTop.m_v2, xyInStt + dSpare, CDPoint(0,1), xyLowerSlab);
			GetXyMatchSegAndLine(tvTopSlabBottom.m_v1, tvTopSlabBottom.m_v2, xyInStt + dSpare, CDPoint(0,1), xyTopSlab);
			
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CUTL);
			Dom.CutLightning(xyInStt.x + dSpare *se, xyLowerSlab.y + Dom.Always(5), xyInStt.x + dSpare *se, xyLowerSlab.y - pBri->m_dTS_Lower - Dom.Always(5));
			Dom.CutLightning(xyInStt.x + dSpare *se, xyTopSlab.y - Dom.Always(5), xyInStt.x + dSpare *se, xyTopSlab.y + pBri->m_dTS + Dom.Always(5));
		}

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
		// 기초부
		CDPoint xyTemp = tvWing.m_v1;
		CDPoint vX(1, 0), vY(0, 1);
		CDPoint xy1(0, 0), xy2(0, 0);
		CDPoint xyStt = pBri->GetXySectionWingWallStt(bStt, TRUE);
		CDPoint xyEndHun = pBri->GetXySectionWingWallStt(bStt, FALSE, TRUE);
		double dXCutLine = xyStt.x - 1900*se;
		
		// 벽체밑에 실제로 기초가 없어도 그려주기위해서...
		double dVwall = MIN(0, dFF-dW);
		if(pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
		{
			if(dL2==0)
			{
				Dom.MoveTo(tvWing.m_v1);
				Dom.LineToEx((dFF-(dW+dVwall))*se, 0);
				xyTemp.x += (dFF-(dW+dVwall))*se;
			}
			else
			{
				Dom.MoveTo(tvWing.m_v1);
				Dom.LineToEx((dFF-tvWing.GetLength()-(dW+dVwall))*se, 0);
				xyTemp.x += (dFF-tvWing.GetLength()-(dW+dVwall))*se;
			}
			Dom.LineToEx(0, -dFH);
			xyTemp.y -= dFH;
		}
		
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH || pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2)
		{
			double dYCutUpper = xyEndHun.y;
			double dYCutLower = dYCutUpper - dFH;
		
			Dom.LineTo(xyTemp.x, xyTemp.y, dXCutLine, xyTemp.y);
			xyTemp.x = dXCutLine;
			xyTemp.y += dFH;
			Dom.LineTo(xyTemp.x, xyTemp.y, xyEndHun.x, xyEndHun.y);
		
			// 확대 기초
			if(!bRebar)
			{
				if(dL2==0)
				{
					Dom.MoveTo(tvWing.m_v1.x+(dFF-dW)*se, tvWing.m_v1.y-dFH);
					xyTemp = CDPoint(tvWing.m_v1.x+(dFF-dW)*se, tvWing.m_v1.y-dFH);
				}
				else
				{
					Dom.MoveTo(tvWing.m_v1.x-(tvWing.GetLength()+dW-dFF)*se, tvWing.m_v1.y-dFH);
					xyTemp = CDPoint(tvWing.m_v1.x-(tvWing.GetLength()+dW-dFF)*se, tvWing.m_v1.y-dFH);
				}
				Dom.LineToEx((dEF-dFF)*se, 0);
				Dom.LineToEx(0, -dEH);
				xyTemp += CDPoint((dEF-dFF)*se, -dEH);
				Dom.LineTo(xyTemp.x, xyTemp.y, dXCutLine, xyTemp.y);
				xyTemp.x = dXCutLine;
				dYCutLower -= dEH;
			}
		
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CUTL);
			Dom.CutLightning(dXCutLine, dYCutLower-Dom.Always(5), dXCutLine, dYCutUpper+Dom.Always(5));
		}
		else if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
		{
			if(!bRebar)
			{
				CTwinVector tvLowerSlab;
				CVector tvExFStt;
				CString stsrLowerSlab = bStt? _T("하부슬래브하면(시점벽체아래)") : _T("하부슬래브하면(종점벽체아래)");
				double dEFW = bStt? pFooting->m_exFooting.m_dEFWL : pFooting->m_exFooting.m_dEFWR; //확대기초연장
		
				//부상방지저판이 있으면 확대기초 연장에 부상방지저판 Width만큼 더해줌.
				if(pBri->m_bExistProtectionRiseFooting)
					dEFW += pBri->m_dPRF_W;
		
				tvArrWall.GetTvByInfo(stsrLowerSlab, tvLowerSlab);
				tvExFStt = bStt ? tvLowerSlab.m_v1 : tvLowerSlab.m_v2;
		
				CDPoint xyLowerSlabAng = tvLowerSlab.GetXyDir();
				double dExFLength = dEFW / xyLowerSlabAng.x;
		
				//날개벽 각도있을때 벽체 늘어난 길이
				CDPoint vAng	= pBri->GetAngleWingWallBetweenWall(bStt, bLeft);
				double dWall = bStt ? -pBri->m_dWS/vAng.y : pBri->m_dWE/vAng.y; //날개벽각도먹은 두께
				double dAngleWall = dWall / xyLowerSlabAng.x * se; //종단선형방향에서의 길이
		
				Dom.MoveTo(tvExFStt);
				Dom.LineToEx(dExFLength * xyLowerSlabAng * se);
				Dom.LineToEx(0, -pFooting->m_exFooting.m_dEFHR);
				Dom.LineToEx((dExFLength + dAngleWall + dSpare) * xyLowerSlabAng * -se);
			}
		}
		else
		{
			Dom.LineToEx(-(dFF+dFB-dVwall)*se, 0);
			Dom.LineToEx(0, dFH);
			Dom.LineToEx(dFB*se, 0);
		
			// 확대 기초
			if(!bRebar)
			{
				if(dL2==0)
					Dom.MoveTo(tvWing.m_v1.x+(dFF-(dW+dVwall))*se, tvWing.m_v1.y-dFH);
				else
					Dom.MoveTo(tvWing.m_v1.x-(tvWing.GetLength()+(dW+dVwall)-dFF)*se, tvWing.m_v1.y-dFH);
				Dom.LineToEx((dEF-dFF)*se, 0);
				Dom.LineToEx(0, -dEH);
				Dom.LineToEx(-(dEF+dEB-dVwall)*se, 0);
				Dom.LineToEx(0, dEH);
				Dom.LineToEx((dEB-dFB)*se, 0);
			}
		}
	}
//	pBri->GetTvVertSection_Std((bLeft==bStt)? 1 : -1, TRUE, TRUE, TRUE, 2);

	// 날개벽 상단 단차가 있을때는 막아준다.
	if(Compare(pWing->m_dH3, 0.0, _T("="), 0.1) == FALSE)
	{
		CDPoint xyStt = pBri->GetXySectionWingWallStt(bStt, TRUE);

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
		Dom.MoveTo(xyStt);
		Dom.LineTo(xyStt + CDPoint(0, pWing->m_dH3));
	}
	
	*pDomP << Dom;
}

void CDrawWingWall::DrawGenFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut, BOOL bConcSym, BOOL bHunch, BOOL bForBM)
{
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);
	CARcBridgeDataStd *pStd	= pBri->m_pARcBridgeDataStd;

	if(!pWing->m_bExist) return;

	CHgBaseDrawStd baseDraw;
	CTwinVector tv;
	CTwinVectorArray tvArr;
	pBri->GetTvWingWallFront(bStt, bLeft, nOut, tvArr, TRUE, bHunch, FALSE, bForBM);
	int nSize = tvArr.GetSize();

	CDomyun Dom(pDomP);

	//BOX형일때는 정면하단도 그려줘야함. 기초가 없으니까.
	for(long i=0; i<nSize; i++)
	{
		tv = tvArr.GetAt(i);
		if(tv.m_sInfo == "날개벽정면하단" && pBri->m_nTypeBridge != BRIDGE_TYPE_BOX) continue;
		if(tv.m_v1.z==5)
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONH);
		else
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
		
		tv.m_v1.z = 0;
		baseDraw.DrawTv(&Dom, tv, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());
	}

	*pDomP << Dom;

	// 콘크리트 심볼
	if(bConcSym)
	{
		CConcSymbol Sym;
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		CTwinVector tvUpper, tvLower, tvTail;
		tvArr.GetTvByInfo(_T("날개벽정면상단"), tvUpper);
		tvArr.GetTvByInfo(_T("날개벽정면하단"), tvLower);
		tvArr.GetTvByInfo(_T("날개벽정면노리"), tvTail);

		const EPartOfBridge ePart = bStt? ePartWingWallStt : ePartWingWallEnd;
		CString	str = m_pStd->GetStringTextDimFck(pBri->GetValueFck(ePart), pBri->GetValueAggregate(ePart), pBri->GetValueSlump(ePart));
		CStringArray strArr;
		strArr.Add("구체 콘크리트");
		strArr.Add(str);

		CDPoint xyCen(0, 0), vRotate(0, 0);
		if(pWing->m_dL2==0)
		{
			xyCen.x = tvTail.GetXyMid().x;
			xyCen.y = tvTail.m_v2.y + (tvTail.m_v1.y-tvTail.m_v2.y)*3.0/4.0;
			vRotate = bStt ? tvTail.GetXyDir() : -tvTail.GetXyDir();
		}
		else
		{
			xyCen.x = tvLower.GetXyMid().x;
			xyCen.y = (tvUpper.m_v1.y+tvLower.m_v2.y)/2;
			vRotate = bStt ? CDPoint(0, 1) : CDPoint(0, -1);
		}

		Sym.DrawSymbolConCArrow(&Dom, xyCen, strArr);

		Dom.Rotate(xyCen, vRotate);

		*pDomP << Dom;
	}
}

void CDrawWingWall::DrawGenSection(CDomyun* pDomP, BOOL bStt, BOOL bLeft, double dSpare, BOOL bRebar)
{
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	if(pWing->m_dL2==0)
		dSpare = 1000;

	CTwinVectorArray tvArr1, tvArr2;
	CTwinVector tv;
	CDPoint xy1(0, 0), xy2(0, 0);
	CDPoint xyCut1(0, 0), xyCut2(0, 0);

	double dT2 = pWing->m_dT2;
	double dHuW = pWing->m_dHuW1;
	double lr = bLeft ? -1.0 : 1.0;
	
	double dHeight = pBri->GetHeightWingWall(bStt);
	double dCutLen = dT2 + dHuW + dSpare;
	double dFH = bStt ? (pBri->m_footingStt.m_dFH + pBri->m_footingStt.m_dFSRH) : (pBri->m_footingEnd.m_dFH + pBri->m_footingEnd.m_dFSLH);
	double dEF = bStt ? pBri->m_footingStt.m_exFooting.m_dEFWL : pBri->m_footingEnd.m_exFooting.m_dEFWR;
	double dEH = bStt ? pBri->m_footingStt.m_exFooting.GetMaxHeight() : pBri->m_footingEnd.m_exFooting.GetMaxHeight();
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		//BOx는 기초가 없으니까 FH는 없다
		dFH = 0;
		dEF = pBri->m_footingBox.m_exFooting.m_dEFWL;
		dEH = pBri->m_footingBox.m_exFooting.GetMaxHeight();
	}

	CDPoint xyStt = pBri->GetXySectionWingWallStt(bStt, TRUE);
	CDPoint xyEnd = pBri->GetXySectionWingWallStt(bStt, FALSE);

	// 외측선
	pBri->GetTvWingWallSection(bStt, bLeft, tvArr1, dSpare);

	if(!bRebar)
	{
		// 버림 콘크리트
		xy1 = CDPoint(xyStt.x-dCutLen*lr, xyEnd.y-dFH);
		xy2 = CDPoint(xyStt.x+dEF*lr, xy1.y);
		tv.m_v1 = xy1; tv.m_v2 = xy2;
		tvArr2.Add(tv);
		
		xy1 = xy2;
		xy2 = CDPoint(xy1.x, xy1.y-dEH);
		tv.m_v1 = xy1; tv.m_v2 = xy2;
		tvArr2.Add(tv);
		
		xy1 = xy2;
		xy2 = CDPoint(xyStt.x-dCutLen*lr, xy1.y);
		tv.m_v1 = xy1; tv.m_v2 = xy2;
		tvArr2.Add(tv);
	}
	else
	{
		xy1 = CDPoint(xyStt.x-dCutLen*lr, xyEnd.y-dFH);
		xy2 = CDPoint(xyStt.x+dEF*lr, xy1.y);
	}

	//부상방지판이 있다면 부상방지판 높이만큼 컷팅라인을 높여준다.
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX && pBri->m_bExistProtectionRiseFooting)
	{
		double dHeightPRF = pBri->m_dPRF_H1 + pBri->m_dPRF_H2;
		dHeight -= dHeightPRF;
	}
	xyCut1 = CDPoint(xyStt.x-dCutLen*lr, xy2.y-500.0);	// 컷팅 라인
	xyCut2 = CDPoint(xyStt.x-dCutLen*lr, xyStt.y-dHeight+500.0);	// 컷팅 라인

	CDomyun Dom(pDomP);

	m_pStd->DrawTvArray(&Dom, tvArr1, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());
	m_pStd->DrawTvArray(&Dom, tvArr2);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CUTL);
	if(dSpare > 0)
		Dom.CutLightning(xyCut1.x, xyCut1.y, xyCut2.x, xyCut2.y);

	*pDomP << Dom;
}

void CDrawWingWall::DrawGenPlane(CDomyun* pDomP, BOOL bStt, BOOL bDrawWallHidden)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	CWingWall* pWingL = pBri->GetWingWall(bStt, TRUE);
	CWingWall* pWingR = pBri->GetWingWall(bStt, FALSE);
	CLineInfo* pLineL = pBri->GetLineLeftRight(!bStt);
	CLineInfo* pLineR = pBri->GetLineLeftRight(bStt);

	CTwinVector tv;
	CDPoint A[4];
	if(bStt)
		pBri->GetXySss(BRIDGE_OFF_STT_STT, A);
	else
		pBri->GetXySss(BRIDGE_OFF_END_END, A);

	double dW = bStt ? pBri->m_dWS : pBri->m_dWE;	// 벽체 두께
	double dBW = bStt ? pBri->m_dBWS : pBri->m_dBWE;	// 브라켓 폭

	// 슬래브 끝단 좌표(시종점 좌표가 아님)
	double nCountJijum = pBri->GetCountJijum();
	CJijum* pJ = bStt ? pBri->GetJijum(0) : pBri->GetJijum((int)(nCountJijum - 1));
	CDPoint vAng = pJ->m_vAngle;
	double se = bStt ? -1.0 : 1.0;
	double dSta = bStt ? pBri->m_dStationBridgeStt : pBri->m_dStationBridgeStt+pBri->m_dLengthBridge;
	
	// (ARCBRIDGE-3019) 곡선선형인경우 브라켓은 Station과 지점각도를 가지고 그리면 안된다. 벽체에서 offset해서 그려야한다.
	CDPoint xySlabL = pBri->GetXySlabEdgeAct(dSta, vAng, bStt ? FALSE : TRUE);
	CDPoint xySlabR = pBri->GetXySlabEdgeAct(dSta, vAng, bStt ? TRUE : FALSE);
	Offset(xySlabL, xySlabR, -dW); // * se
	// 브라켓 좌표
	CDPoint xyBrkL = xySlabL;
	CDPoint xyBrkR = xySlabR;
	Offset(xyBrkL, xyBrkR, -dBW);

	// 날개벽 시작점 좌표
	CDPoint xyOutSttL = bStt ? A[3] : A[0];
	CDPoint xyOutSttR = bStt ? A[0] : A[3];

	CDomyun Dom(pDomP);
	CHgBaseDrawStd baseDraw;

	// 날개벽 표현
	CString szNameSlab = "슬래브끝단외측";
	pBri->GetTvWingWallPlane(bStt, TRUE, TRUE, TRUE);

	CTwinVector tvSlab = pBri->m_tvArrPlaneWingWall.GetTvByInfo(szNameSlab);
	if(bDrawWallHidden)
	{
		long nIdxSlab = pBri->m_tvArrPlaneWingWall.GetIdxByInfo(szNameSlab);
		pBri->m_tvArrPlaneWingWall.RemoveAt(nIdxSlab);
	}
	m_pStd->DrawTvArray(&Dom, pBri->m_tvArrPlaneWingWall, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());
	if(bDrawWallHidden)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONH);
		baseDraw.DrawTv(&Dom, tvSlab, m_pStd->IsViewVectorInfo());
	}

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	if(!pWingL->m_bExist && !pBri->IsGagakForWing(bStt, TRUE))
	{
		double dStaL1 = 0, dStaL2 = 0;
		double dDist = bStt ? pBri->m_dWidthSlabRight : pBri->m_dWidthSlabLeft;
		pLineL->GetStationMatchVerticalByXy(xySlabL, dStaL1);
		pLineL->GetStationMatchVerticalByXy(xyOutSttL, dStaL2);
		pLineL->DrawLS(&Dom, dStaL1, dStaL2, dDist);
	}
	else if(pWingL->m_bExist && pWingL->m_dL2!=0)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONH);
		pBri->GetTvWingWallBoundAD(bStt, TRUE, tv, TRUE);
		Dom.LineTo(tv.m_v1, tv.m_v2);
	}

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	if(!pWingR->m_bExist && !pBri->IsGagakForWing(bStt, FALSE))
	{
		double dStaR1 = 0, dStaR2 = 0;
		double dDist = bStt ? pBri->m_dWidthSlabLeft : pBri->m_dWidthSlabRight;
		pLineR->GetStationMatchVerticalByXy(xySlabR, dStaR1);
		pLineR->GetStationMatchVerticalByXy(xyOutSttR, dStaR2);
		pLineR->DrawLS(&Dom, dStaR1, dStaR2, dDist);
	}
	else if(pWingR->m_bExist && pWingR->m_dL2!=0)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONH);
		pBri->GetTvWingWallBoundAD(bStt, FALSE, tv, TRUE);
		Dom.LineTo(tv.m_v1, tv.m_v2);
	}

	*pDomP << Dom;
}

void CDrawWingWall::DrawGenCutPlane(CDomyun* pDomP, BOOL bStt, BOOL bLeft, double dSpare, BOOL bUpper)
{
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	double dW = bStt ? pBri->m_dWS : pBri->m_dWE;
	double dBW = bStt ? pBri->m_dBWS : pBri->m_dBWE;
	double se = bStt ? -1.0 : 1.0;
	double lr = bLeft ? -1.0 : 1.0;

	CTwinVectorArray tvArr, tvArrCut;
	CTwinVector tv, tvSlab;

	pBri->GetTvWingWallPlane(bStt, TRUE, bUpper, bUpper);
	tvArr = pBri->m_tvArrPlaneWingWall;
	int nSize = tvArr.GetSize();

	tvArr.GetTvByInfo(_T("슬래브끝단외측"), tvSlab);
	CDPoint vSlab = tvSlab.GetXyDir();
	CDPoint vCut = vSlab.Rotate90()*lr;
	CDPoint xyOrigin = bLeft ? tvSlab.m_v1 : tvSlab.m_v2;
	CDPoint xyCutStt = xyOrigin - dSpare*vSlab*lr;
	CDPoint xyCutEnd = xyCutStt + (dW+dBW)*vCut*lr;
	CDPoint xyStt = xyCutStt - 10*vCut*lr;
	CDPoint xyEnd = xyCutEnd + 10*vCut*lr;
	CDPoint xyCut(0, 0);

	BOOL bDir1 = FALSE, bDir2 = FALSE;
	int i=0; for(i=0; i<nSize; i++)
	{
		tv = tvArr.GetAt(i);
		if(bLeft)
		{
			bDir1 = GetDirPointByLine(tv.m_v1, xyStt, xyEnd);
			bDir2 = GetDirPointByLine(tv.m_v2, xyStt, xyEnd);
		}
		else
		{
			bDir1 = GetDirPointByLine(tv.m_v1, xyEnd, xyStt);
			bDir2 = GetDirPointByLine(tv.m_v2, xyEnd, xyStt);
		}
		if(bDir1 || bDir2)
		{
			if(!bDir1 || !bDir2)
			{
				if(GetXyMatchSegAndSeg(tv.m_v1, tv.m_v2, xyStt, xyEnd, xyCut))
				{
					if(bLeft)
						tv.m_v2 = xyCut;
					else
						tv.m_v1 = xyCut;
				}
			}
			tvArrCut.Add(tv);
		}
	}

	CDomyun Dom(pDomP);

	// 컷팅 라인
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CUTL);
	Dom.CutLightning(xyCutStt.x-500, xyCutStt.y, xyCutStt.x+dW+dBW+500, xyCutStt.y, FALSE);
	Dom.Rotate(xyCutStt, vCut*lr);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	m_pStd->DrawTvArray(&Dom, tvArrCut, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());
	if(pWing->m_bExist && pWing->m_dL2!=0)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONH);
		pBri->GetTvWingWallBoundAD(bStt, bLeft, tv, bUpper);
		Dom.LineTo(tv.m_v1, tv.m_v2);
	}

	CDPoint vAngWing = pBri->GetAngleAzimuthWingWall(bStt, bLeft);
	CDPoint vRotate = vAngWing.BetweenAngle(CDPoint(se, 0));
	Dom.Rotate(xyOrigin, vRotate);
	Dom.Move(-xyOrigin);

	*pDomP << Dom;
}

void CDrawWingWall::DimGenFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, BOOL bInput)
{
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	CDPoint xy1(0, 0), xy2(0, 0);

	double dL1 = pWing->m_dL1;
	double dL2 = pWing->m_dL2;
	double dHL = pWing->m_dHL;
	double dH2 = pWing->m_dH2;
	double dH3 = pWing->m_dH3;
	double dSW = pWing->m_dSW;
	double dElEnd = pWing->GetEndEl(FALSE);
	double se = bStt ? -1.0 : 1.0;

	double dHeight = pBri->GetHeightWingWall(bStt);
	double dFH = bStt ? (pBri->m_footingStt.m_dFH + pBri->m_footingStt.m_dFSRH) : (pBri->m_footingEnd.m_dFH + pBri->m_footingEnd.m_dFSLH);
	double dEH = bStt ? pBri->m_footingStt.m_exFooting.GetMaxHeight() : pBri->m_footingEnd.m_exFooting.GetMaxHeight();
	if(pBri->IsBoxType())
	{
		dFH = 0;
		dEH = pBri->m_footingBox.m_exFooting.GetMaxHeight();
	}

	CDPoint xyStt = pBri->GetXySectionWingWallStt(bStt, TRUE);
	CDPoint xyEnd = pBri->GetXySectionWingWallStt(bStt, FALSE);
	CDPoint xyBot = CDPoint(xyStt.x + (dL1+dL2)*se, xyEnd.y - dFH - dEH);

	double dHeightEnd = dHeight - (xyStt.y-dElEnd);

	double dHL1	= dL1 / dSW;
	if(dL2 == 0 && dL1 > 0)
	{
		CTwinVectorArray tvArrWing;
		pBri->GetTvWingWallFront(bStt, bLeft, -1, tvArrWing, TRUE, FALSE, FALSE, FALSE);

		CTwinVector tvTmp;
		tvTmp	= tvArrWing.GetTvByInfo("날개벽정면노리");
		dHL1	= Round(tvTmp.GetVerLength(), 0);		// 0.01보다 작아도 치수가 0으로 표시되면서 들어가는거 방지.
	}
	
	double dMidH = dHeightEnd - dHL - dHL1;
	double dPRFHeight = pBri->m_dPRF_H1 + pBri->m_dPRF_H2;
	//부상방지저판이 있으면 높이만큼 빼준다.
	if(pBri->m_bExistProtectionRiseFooting)
		dMidH -= dPRFHeight;
	//H2가있는경우에도 높이만큼 빼준다.
	if(!pBri->m_bExistProtectionRiseFooting && pWing->m_dL2 > 0 && dH2 > 0) 
		dMidH -= dH2;

	CString str1 = _T(""), str2 = _T("");

	CDomyun Dom(pDomP);
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);

	CString strDir = bStt ? _T("LEFT") : _T("RIGHT");
	Dom.SetDirection(strDir);

	CFootingApp *pFooting = &pBri->m_footingStt;
	if(pFooting->m_exFooting.m_nType==EXFOOTING_TYPE_MASS)
		Dom.DimMoveTo(xyBot.x, xyBot.y+dEH, 0);
	else
	{
		Dom.DimMoveTo(xyBot.x, xyBot.y, 0, COMMA(dEH));
		Dom.DimLineTo(dEH, 0, _T(""));
	}
	Dom.DimLineTo(dFH, 0, COMMA(dFH));
	if(pBri->m_bExistProtectionRiseFooting)
		Dom.DimLineTo(dPRFHeight, 0, COMMA(dPRFHeight));
	if(!pBri->m_bExistProtectionRiseFooting && pWing->m_dL2 > 0 && dH2 > 0) 
	{
		str1 = bInput ? _T("H2") : COMMA(dH2);
		str2 = bInput ? COMMA(dH2) : _T("");
		Dom.DimLineToExtend(dH2, 0, str1, str2);
	}
	Dom.DimLineTo(dMidH, 0, COMMA(dMidH));
	Dom.DimLineTo(dHL1, 0, COMMA(dHL1));
	str1 = bInput ? _T("H1") : COMMA(dHL);
	str2 = bInput ? COMMA(dHL) : _T("");
	Dom.DimLineToExtend(dHL, 0, str1, str2);

	if(Compare(dH3, 0.0, _T("="), 0.1) == FALSE)
	{
		str1 = bInput ? _T("HP") : COMMA(dH3);
		str2 = bInput ? COMMA(dH3) : _T("");
		Dom.DimLineToExtend(dH3, 0, str1, str2);
	}

	Dom.DimMoveTo(xyBot.x, xyBot.y+dEH, 1);
	Dom.DimLineTo(dHeightEnd+dFH, 1, COMMA(dHeightEnd+dFH));

	*pDomP << Dom;

	Dom.SetDirection(_T("TOP"));

	Dom.DimMoveTo(xyStt.x, xyStt.y, 1);
	str1 = bInput ? _T("L2") : COMMA(dL2);
	str2 = bInput ? COMMA(dL2) : _T("");
	Dom.DimLineToExtend(dL2, 1, str1, str2);
	str1 = bInput ? _T("L1") : COMMA(dL1);
	str2 = bInput ? COMMA(dL1) : _T("");
	Dom.DimLineToExtend(dL1, 1, str1, str2);

	Dom.DimMoveTo(xyStt.x, xyStt.y, 2);
	Dom.DimLineTo(dL1+dL2, 2, COMMA(dL1+dL2));

	if(bStt)
		Dom.Mirror(xyStt.x, TRUE);

	*pDomP << Dom;
}

void CDrawWingWall::DimGenSection(CDomyun* pDomP, BOOL bStt, BOOL bLeft, BOOL bInput)
{
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	CDPoint xy1(0, 0), xy2(0, 0);
	CDPoint xyCut1(0, 0), xyCut2(0, 0);

	double dT1 = pWing->m_dT1;
	double dT2 = pWing->m_dT2;
	double dLT = pWing->m_dLT;
	int nDirHunch = dT1==dT2 ? 1 : pWing->m_nDirHunch;
	if(nDirHunch==2)
	{
		dT1 = dT2;
		dLT = 0.0;
	}
	double dDT = dT2-dT1;
	double dSH = pWing->m_dSH;
	double dDS = fabs(dDT*dSH);
	double dHuW = pWing->m_dHuW1;
	double dHuH = pWing->m_dHuH1;
	double lr = bLeft ? -1.0 : 1.0;

	double dHeight = pBri->GetHeightWingWall(bStt);
	double dMidH = dHeight - (dLT+dDS+dHuH);
	double dSpare = max(dT1, dT2) + dHuW + 1000.0;
	double dFH = bStt ? (pBri->m_footingStt.m_dFH + pBri->m_footingStt.m_dFSRH) : (pBri->m_footingEnd.m_dFH + pBri->m_footingEnd.m_dFSLH);
	double dEH = bStt ? pBri->m_footingStt.m_exFooting.GetMaxHeight() : pBri->m_footingEnd.m_exFooting.GetMaxHeight();
	if(pBri->IsBoxType())
	{
		dFH = 0;
		dEH = pBri->m_footingBox.m_exFooting.GetMaxHeight();
	}

	CDPoint xyStt = pBri->GetXySectionWingWallStt(bStt, TRUE);
	CDPoint xyEnd = pBri->GetXySectionWingWallStt(bStt, FALSE);
	CDPoint xyBot = CDPoint(xyStt.x-dSpare*lr, xyEnd.y-dFH-dEH);

	CString str1 = _T(""), str2 = _T("");

	CDomyun Dom(pDomP);

	CString strDir = bLeft ? _T("RIGHT") : _T("LEFT");
	Dom.SetDirection(strDir);

	CFootingApp *pFooting = &pBri->m_footingStt;
	if(pFooting->m_exFooting.m_nType==EXFOOTING_TYPE_MASS)
		Dom.DimMoveTo(xyBot.x, xyBot.y+dEH, 0);
	else
	{
		Dom.DimMoveTo(xyBot.x, xyBot.y, 0, COMMA(dEH));
		Dom.DimLineTo(dEH, 0, _T(""));
	}

	//박스형일떄 H2가있으면 헌치 그리지 않음. 
	if(dHuW || dHuH)
	{
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX && pWing->m_dL2 > 0 && pWing->m_dH2 > 0)
		{

		}
		else
		{
			Dom.DimLineTo(dFH, 0, COMMA(dFH));
			Dom.DimLineToExtend(dHuH, 0, _T(""), COMMA(dHuH));
			Dom.DimLineTo(dMidH, 0, COMMA(dMidH));
			Dom.DimLineTo(dDS, 0, COMMA(dDS));
			str1 = bInput ? _T("TL") : COMMA(dLT);
			str2 = bInput ? COMMA(dLT) : _T("");
			Dom.DimLineToExtend(dLT, 0, str1, str2);
		}
	}
	
	Dom.DimMoveTo(xyBot.x, xyBot.y+dEH, 1);
	Dom.DimLineTo(dHeight+dFH, 1, COMMA(dHeight+dFH));

	*pDomP << Dom;

	Dom.SetDirection(_T("TOP"));

	Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
	if(dDT>0)
	{
		str1 = bInput ? _T("T1") : COMMA(dT1);
		str2 = bInput ? COMMA(dT1) : _T("");
		Dom.DimLineToExtend(dT1, 0, str1, str2);
		Dom.DimLineToOver(dDT, 0, COMMA(dDT));

		Dom.DimMoveTo(xyStt.x, xyStt.y, 1);
		str1 = bInput ? _T("T2") : COMMA(dT2);
		str2 = bInput ? COMMA(dT2) : _T("");
		Dom.DimLineToExtend(dT2, 1, str1, str2);
	}
	else if(dDT<0)
	{
		str1 = bInput ? _T("T2") : COMMA(dT2);
		str2 = bInput ? COMMA(dT2) : _T("");
		Dom.DimLineToExtend(dT2, 0, str1, str2);
		Dom.DimLineToOver(-dDT, 0, COMMA(-dDT));

		Dom.DimMoveTo(xyStt.x, xyStt.y, 1);
		str1 = bInput ? _T("T1") : COMMA(dT1);
		str2 = bInput ? COMMA(dT1) : _T("");
		Dom.DimLineToExtend(dT1, 1, str1, str2);
	}
	else
	{
		str1 = bInput ? _T("T2") : COMMA(dT2);
		str2 = bInput ? COMMA(dT2) : _T("");
		Dom.DimLineToExtend(dT2, 0, str1, str2);

		if(bInput && nDirHunch==1)
		{
			Dom.DimMoveTo(xyStt.x, xyStt.y, 1);
			Dom.DimLineToExtend(dT1, 1, _T("T1"), COMMA(dT1));
		}
	}

	if(!bLeft)
		Dom.Mirror(xyStt.x, TRUE);

	*pDomP << Dom;
}

void CDrawWingWall::DimGenPlane(CDomyun* pDomP, BOOL bStt, BOOL bLeft, BOOL bInput)
{
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	int nCountJijum = pBri->GetCountJijum();
	int nJ = bStt ? 0 : nCountJijum-1;
	double se = bStt ? -1.0 : 1.0;
	double lr = bLeft ? -1.0 : 1.0;
	double dT1 = pWing->m_dT1;
	double dT2 = pWing->m_dT2;
	double dLT = pWing->m_dLT;
	BOOL bRAngEnd = pWing->m_bRAngEnd;
	BOOL bSide	= (pWing->m_nAttachPos != 0)? TRUE : FALSE;
	int nDirHunch = dT1==dT2 ? 1 : pWing->m_nDirHunch;
	if(nDirHunch==1)
	{
		dT1 = dT2 = min(dT1, dT2);
		dLT = 0.0;
	}
	double dDT = dT2-dT1;

	CDPoint vX = CDPoint(1, 0);
	CDPoint xyOrigin(0, 0);

	CTwinVectorArray tvArr;
	CTwinVector tv;

	tvArr = pBri->m_tvArrPlaneWingWall;

	tvArr.GetTvByInfo(_T("슬래브끝단외측"), tv);
	xyOrigin = bLeft ? tv.m_v1 : tv.m_v2;

	CDPoint vAngWing = pBri->GetAngleAzimuthWingWall(bStt, bLeft);
	CDPoint vRotate = vAngWing.BetweenAngle(vX*se);
	tvArr.Rotate(xyOrigin, vRotate);
	tvArr.Move(-xyOrigin);

	CDPoint vWing = pWing->m_vAng; if(bLeft) vWing.MirrorHorz();
	CDPoint vAng = pBri->GetAngleAzimuthJijum(nJ).Rotate(vRotate);
	CDPoint xyOutStt(0, 0), xyOutEnd(0, 0);		// 날개벽 외측 시종점 좌표
	CDPoint xyInStt(0, 0), xyInEnd(0, 0);		// 날개벽 내측 시종점 좌표
	CDPoint xyInStt1(0, 0), xyInStt2(0, 0);		// 날개벽 시점 헌치 좌표
	CDPoint xyHunch1(0, 0), xyHunch2(0, 0);		// 날개벽 내부 헌치 좌표

	CString strDir = bLeft ? _T("좌측") : _T("우측");
	
	tvArr.GetTvByInfo(strDir+_T("날개벽외측"), tv);
	xyOutStt = tv.m_v1;
	xyOutEnd = tv.m_v2;
	tvArr.GetTvByInfo(strDir+_T("날개벽끝단"), tv);
	xyInEnd = tv.m_v2;
	tvArr.GetTvByInfo(strDir+_T("날개벽내부헌치"), tv);
	xyHunch1 = tv.m_v1;
	xyHunch2 = tv.m_v2;
	tvArr.GetTvByInfo(strDir+_T("날개벽시점헌치"), tv);
	xyInStt1 = tv.m_v1;
	xyInStt2 = tv.m_v2;

	
	GetXyMatchLineAndLine(xyInStt1, vX, xyInStt2, bSide? vWing:vAng, xyInStt);

	CDomyun Dom(pDomP);

	CString str1 = _T(""), str2 = _T("");
	strDir = se*lr>0 ? _T("BOTTOM") : _T("TOP");
	Dom.SetDirection(strDir);
	double dDistSum = 0.0;
	CDPoint xyStt = xyOutStt;

	Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
	double dDist = (xyInStt.x - xyOutStt.x) * se;
	if(dDist >= 0)
	{
		dDistSum += dDist;
		Dom.DimLineTo(dDist, 0, COMMA(dDist));
		dDist = fabs(xyInStt.x-xyInStt1.x);
		dDistSum += dDist;
		Dom.DimLineTo(dDist, 0, COMMA(dDist));
	}
	else
	{
		xyInStt1 = xyOutStt;
	}
	
	if(nDirHunch==1)
		dDist = min(fabs(xyInStt1.x-xyInEnd.x), fabs(xyInStt1.x-xyOutEnd.x));
	else
		dDist = fabs(xyInStt1.x-xyHunch2.x);
	dDistSum += dDist;
	Dom.DimLineTo(dDist, 0, COMMA(dDist));
	dDist = fabs(xyHunch2.x-xyHunch1.x);
	dDistSum += dDist;
	Dom.DimLineTo(dDist, 0, COMMA(dDist));
	dDist = Round(fabs(xyHunch1.x-xyOutEnd.x), 5);
	dDistSum += dDist;
	if(nDirHunch==1)
		Dom.DimLineTo(dDist, 0, COMMA(dDist));
	else
	{
		str1 = bInput ? _T("LT") : COMMA(dDist);
		str2 = bInput ? COMMA(dDist) : _T("");
		Dom.DimLineToExtend(dDist, 0, str1, str2);
	}

	Dom.DimMoveTo(xyStt.x, xyStt.y, 1);
	Dom.DimLineTo(dDistSum, 1, COMMA(dDistSum));

	if(nDirHunch==2 && !bRAngEnd && dLT>0.0)
	{
		strDir = se*lr>0 ? _T("TOP") : _T("BOTTOM");
		Dom.SetDirection(strDir);
		dDistSum = 0.0;

		xyStt.x = xyOutStt.x + fabs(xyOutStt.x - xyHunch1.x);
		xyStt.y = xyHunch1.y;
		Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
		dDist = min(fabs(xyHunch1.x-xyInEnd.x), fabs(xyHunch1.x-xyOutEnd.x));
		dDistSum += dDist;
		Dom.DimLineTo(dDist, 0, COMMA(dDist));
		dDist = Round(fabs(xyInEnd.x-xyOutEnd.x), 5);
		dDistSum += dDist;
		Dom.DimLineTo(dDist, 0, COMMA(dDist));

		Dom.DimMoveTo(xyStt.x, xyStt.y, 1);
		Dom.DimLineTo(dDistSum, 1, COMMA(dDistSum));
	}

	if(bStt)
		Dom.Mirror(0, TRUE);

	*pDomP << Dom;

	strDir = bStt ? _T("LEFT") : _T("RIGHT");
	Dom.SetDirection(strDir);

	xyStt.x = bStt ? min(xyInEnd.x, xyOutEnd.x) : max(xyInEnd.x, xyOutEnd.x);
	xyStt.y = se*lr>0 ? xyOutEnd.y : min(xyHunch1.y, xyHunch2.y);
	if(dDT>0)
	{
		str1 = bInput ? _T("T1") : COMMA(dT1);
		str2 = bInput ? COMMA(dT1) : _T("");
		if(se*lr>0)
		{
			Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
			Dom.DimLineToExtend(dT1, 0, str1, str2);
			Dom.DimLineToOver(dDT, 0, COMMA(dDT));
		}
		else
		{
			Dom.DimMoveTo(xyStt.x, xyStt.y, 0, COMMA(dDT));
			Dom.DimLineTo(dDT, 0, _T(""));
			Dom.DimLineToExtend(dT1, 0, str1, str2);
		}

		Dom.DimMoveTo(xyStt.x, xyStt.y, 1);
		str1 = bInput ? _T("T2") : COMMA(dT2);
		str2 = bInput ? COMMA(dT2) : _T("");
		Dom.DimLineToExtend(dT2, 1, str1, str2);
	}
	else if(dDT<0)
	{
		str1 = bInput ? _T("T2") : COMMA(dT2);
		str2 = bInput ? COMMA(dT2) : _T("");
		if(se*lr>0)
		{
			Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
			Dom.DimLineToExtend(dT2, 0, str1, str2);
			Dom.DimLineToOver(-dDT, 0, COMMA(-dDT));
		}
		else
		{
			Dom.DimMoveTo(xyStt.x, xyStt.y, 0, COMMA(-dDT));
			Dom.DimLineTo(-dDT, 0, _T(""));
			Dom.DimLineToExtend(dT2, 0, str1, str2);
		}

		Dom.DimMoveTo(xyStt.x, xyStt.y, 1);
		str1 = bInput ? _T("T1") : COMMA(dT1);
		str2 = bInput ? COMMA(dT1) : _T("");
		Dom.DimLineToExtend(dT1, 1, str1, str2);
	}
	else
	{
		// T1 == T2 인경우이므로 T1만 표시하면 된다.
		Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
		CString str = _T("T1");
		str1 = bInput ? str : COMMA(dT1);
		str2 = bInput ? COMMA(dT1) : _T("");
		Dom.DimLineToExtend(dT1, 0, str1, str2);
	}

	*pDomP << Dom;
}

void CDrawWingWall::MarkGenFrontEl(CDomyun* pDomP, BOOL bStt, BOOL bLeft, BOOL bInput)
{
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	CDPoint vAng = pBri->GetAngleJijum(bStt? 0 : pBri->m_nQtyJigan);
	double dL1 = pWing->m_dL1;
	double dL2 = pWing->m_dL2;
	double se = bStt ? -1.0 : 1.0;
	double dElEnd = pWing->GetEndEl(TRUE);
	double dW  = bStt ? pBri->m_dWS/vAng.y : pBri->m_dWE/vAng.y;
	double dFW = bStt ? pBri->m_footingStt.m_dFWR/vAng.y : pBri->m_footingEnd.m_dFWL/vAng.y;
	double dFH = bStt ? (pBri->m_footingStt.m_dFH + pBri->m_footingStt.m_dFSRH) : (pBri->m_footingEnd.m_dFH + pBri->m_footingEnd.m_dFSLH);
	if(pBri->IsBoxType()) //BoxType은 Footing 정보가 없음.
	{
		dFW = 0;
		dFH = 0;
	}

	CDPoint xyStt = pBri->GetXySectionWingWallStt(bStt, TRUE);
	CDPoint xyEnd = pBri->GetXySectionWingWallStt(bStt, FALSE);
	CDPoint xyEndWing = CDPoint(xyStt.x + (dL1+dL2)*se, dElEnd);
	CDPoint xyBot = CDPoint(xyStt.x + dFW/2 , xyEnd.y - dFH);
	CDPoint xyMid(0, 0);

	CDomyun Dom(pDomP);
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);

	CString strEl;
	strEl.Format(_T("EL. %.3f"), toM(xyStt.y + pWing->m_dH3));
	Sym.ArrowElevation(&Dom, strEl, _T(""), xyStt + CDPoint(0, pWing->m_dH3), !bStt);

	strEl.Format(_T("EL. %.3f"), toM(dElEnd));
	CString str1 = bInput ? _T("ELE") : strEl;
	CString str2 = bInput ? strEl : _T("");
	Sym.ArrowElevation(&Dom, str1, str2, xyEndWing, bStt);

	strEl.Format(_T("EL. %.3f"), toM(xyBot.y));
	Sym.ArrowElevation(&Dom, strEl, _T(""), xyBot, !bStt);

	// 슬래브 끝단측 EL
	if(pWing->m_nAttachPos == 0)
	{
		GetXyMatchSegAndLine(xyStt, xyEndWing-CDPoint(0, pWing->m_dH3), CDPoint(xyStt.x+dW*se, xyStt.y), CDPoint(0, 1), xyMid);
		strEl.Format(_T("EL. %.3f"), toM(xyMid.y));
		Sym.ArrowElevation(&Dom, strEl, _T(""), xyMid, bStt);
	}

	*pDomP << Dom;
}

void CDrawWingWall::MarkGenFrontSection(CDomyun* pDomP, BOOL bStt, BOOL bLeft, BOOL bGenDom, BOOL bInput, CString str, BOOL bSecVer, BOOL bSecHor)
{
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	char ch = str==_T("") ? 0 : str.GetAt(0);
	double dSpare1 = 500.0;
// 	double dSpare2 = 1900.0;
	double se = bStt ? -1.0 : 1.0;
	double dL1 = pWing->m_dL1;
	double dL2 = pWing->m_dL2;
	double dL = dL1 + dL2;
	double dFH = bStt ? pBri->m_footingStt.m_dFH : pBri->m_footingEnd.m_dFH;
	double dFF = bStt ? pBri->m_footingStt.m_dFWR : pBri->m_footingEnd.m_dFWL;
	double dEH = bStt ? pBri->m_footingStt.m_exFooting.GetMaxHeight() : pBri->m_footingEnd.m_exFooting.GetMaxHeight();
	double dElEnd = pBri->GetElWingWallEnd(bStt, bLeft);
	if(!bGenDom) dEH = 0;

	CDPoint xyTop = pBri->GetXySectionWingWallStt(bStt, TRUE);
	CDPoint xyBot = pBri->GetXySectionWingWallStt(bStt, FALSE);
	CDPoint xyBotC = xyBot;
	CDPoint xyStt = CDPoint(xyTop.x, min(dElEnd, xyTop.y));
	CDPoint xyEnd = CDPoint(xyStt.x+dL*se, xyStt.y);

	CDPoint vAngA = bStt ? CDPoint(1, 0) : CDPoint(-1, 0);
	CDPoint vAngB(0, 1), xy(0, 0);
	double dSpace = 0;

	CDomyun Dom(pDomP);
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	
	if(bSecVer)
	{
		if(bGenDom)
		{
			xyTop.y += (Dom.Always(Dom.GetDimExo()*2.0+Dom.GetDimVLen()*2.0)+dSpare1);
			xyBot.y -= (dFH+dEH+dSpare1);
		}
		else
		{
			xyTop.y += (Dom.Always(Dom.GetDimExo()*2.0+Dom.GetDimVLen())+dSpare1);
			if(pWing->m_dL2==0)
				xyBot.y -= (dFH+dEH+dSpare1);
			else
				xyBot.y -= (dFH+dEH+Dom.Always(Dom.GetDimExo()+Dom.GetDimVLen())+dSpare1);
		}		
		xy = bStt ? xyTop : xyBot;
		dSpace = xyTop.y-xyBot.y;
		CString strA = str==_T("") ? _T("") : CString(++ch);
		Sym.ArrowSection(&Dom, strA, xy, dSpace, vAngA);
	}
	
	if(bSecHor)
	{
		if(bGenDom)
		{
			xyStt.x -= (dSpare1+dFF+500)*se;
			xyEnd.x += (Dom.Always(Dom.GetDimExo()+Dom.GetDimVLen()*2.0)+dSpare1)*se;
		}
		else
		{
			double dSpare2 = 1900.0;

			xyStt.x -= (Dom.Always(Dom.GetDimExo()*2.0+Dom.GetDimVLen())+dSpare1+dSpare2)*se;
			xyEnd.x += (Dom.Always(Dom.GetDimExo()*2.0+Dom.GetDimVLen())+dSpare1)*se;
		}
		xy = bStt ? xyEnd : xyStt;
		dSpace = fabs(xyStt.x-xyEnd.x);
		CString strB = str==_T("") ? _T("") : CString(++ch);
		Sym.ArrowSection(&Dom, strB, xy, dSpace, vAngB);
		
		if(!bGenDom && !bInput && dL2!=0)
		{
			xyStt = CDPoint(xyTop.x-dSpare1*se, xyBotC.y+dSpare1);
			xyEnd = CDPoint(xyStt.x+(dL2+dSpare1*2)*se, xyStt.y);
			
			xy = bStt ? xyEnd : xyStt;
			dSpace = fabs(xyStt.x-xyEnd.x);
			CString strC = str==_T("") ? _T("") : CString(++ch);
			Sym.ArrowSection(&Dom, strC, xy, dSpace, vAngB);
		}
	}

	*pDomP << Dom;
}

void CDrawWingWall::MarkGenFrontSlope(CDomyun* pDomP, BOOL bStt, BOOL bLeft, BOOL bInput)
{
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	double dL1 = pWing->m_dL1;
	double dHL = pWing->m_dHL;
	double dSW = pWing->m_dSW;
	double se = bStt ? -1.0 : 1.0;
	double dSpace = 100.0;
	double dWidth = pWing->m_dL1 + pWing->m_dL2;
	CDPoint xyStt = pBri->GetXySectionWingWallStt(bStt, TRUE);
	CDPoint xyEnd = CDPoint(xyStt.x+dWidth*se, pWing->GetEndEl(FALSE));
	CDPoint xyMid = (xyStt+xyEnd)/2.0;

	double dSlope = (Round(xyStt.y, 0)-xyEnd.y)/dWidth;
	CString strSlope = _T("");
	strSlope.Format(_T("%.3f%%"), dSlope*100.0);

	CDomyun Dom(pDomP);
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);

	// 날개벽 상단 경사
	double dLenText = Dom.GetTextWidth(strSlope, 4);
	xyMid.x += dLenText/2.0*se;
	xyMid.y += dSpace;
	CDPoint vDir = CDPoint(-se, dSlope).Unit();

	Sym.ArrowDirectionForSlope(&Dom, xyMid, strSlope, vDir);

	*pDomP << Dom;

	// 날개벽 꼬리 경사
	if(dL1!=0 && !pWing->m_bSWHor)
	{
		CFootingApp * pFoot = pBri->GetFooting(bStt? 0:pBri->GetCountFooting()-1);
		if(!pFoot) return;
		double dElBottom = pFoot->m_dEL-pFoot->GetHeight();
		CDPoint xyTemp(0, 0);
		CDPoint xyTailTop = CDPoint(xyEnd.x, xyEnd.y-dHL);
		CDPoint xyTailBot = CDPoint(xyTailTop.x-dL1*se, xyTailTop.y-dL1/dSW);
		if(GetXyMatchSegAndLine(xyTailTop, xyTailBot, CDPoint(xyTailBot.x, dElBottom), CDPoint(1, 0), xyTemp))
			xyTailBot = xyTemp;
		CDPoint vTail = bStt ? (xyTailBot-xyTailTop).Unit() : (xyTailTop-xyTailBot).Unit();
		CDPoint xyTailMid = (xyTailTop+xyTailBot)/2.0 - (Dom.GetTextHeight()+Dom.Always(1))*vTail.Rotate90();

		strSlope = _T("1:SW");
		if(!bInput)
			strSlope.Format(_T("1:%.1f"), dSW);
		Dom.SetTextAngle(vTail.GetAngleDegree());
		Dom.TextOut(xyTailMid, strSlope);
	}

	*pDomP << Dom;
}

void CDrawWingWall::MarkGenSection(CDomyun* pDomP, BOOL bStt, BOOL bLeft, BOOL bInput)
{
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	double dT1 = pWing->m_dT1;
	double dT2 = pWing->m_dT2;
	double dLT = pWing->m_dLT;
	if(pWing->m_nDirHunch==2)
	{
		dT1 = dT2;
		dLT = 0.0;
	}
	double dDT = dT2-dT1;
	double dSH = pWing->m_dSH;
	double dDS = fabs(dDT*dSH);
	double dHuW = pWing->m_dHuW1;
	double dHuH = pWing->m_dHuH1;
	double lr = bLeft ? -1.0 : 1.0;

	double dHeight = pBri->GetHeightWingWall(bStt);

	CDPoint xyStt = pBri->GetXySectionWingWallStt(bStt, TRUE);

	CDomyun Dom(pDomP);
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);

	// 단면 변화부 경사
	if(dDT!=0)
	{
		CDPoint xyVarTop = CDPoint(xyStt.x-dT1*lr, xyStt.y-dLT);
		CDPoint xyVarBot = CDPoint(xyVarTop.x-dDT*lr, xyVarTop.y-dDS);
		CDPoint vVar = bLeft ? (xyVarBot-xyVarTop).Unit() : (xyVarTop-xyVarBot).Unit();
		CDPoint xyVarMid = (xyVarTop+xyVarBot)/2.0 + (Dom.Always(1))*vVar.Rotate90();

		CString strSlope = _T("1:SH");
		if(!bInput)
			strSlope.Format(_T("1:%.1f"), dSH);
		Dom.SetTextAngle(vVar.GetAngleDegree());
		Dom.TextOut(xyVarMid, strSlope);
	}

	//박스형일떄 H2가있으면 헌치 그리지 않음. 
	// 헌치
	if(dHuW!=0 && dHuH!=0)
	{
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX && pWing->m_dL2 > 0 && pWing->m_dH2 > 0)
		{

		}
		else
		{
			CDPoint xyHuTop = CDPoint(xyStt.x-dT2*lr, xyStt.y-dHeight+dHuH);
			CDPoint xyHuBot = CDPoint(xyHuTop.x-dHuW*lr, xyHuTop.y-dHuH);
			CDPoint xyHuMid = (xyHuTop+xyHuBot)/2.0;

			//박스형일때 부상방지판이 있으면 부상방지판 높이만큼 올려준다.
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX && pBri->m_bExistProtectionRiseFooting)
				xyHuMid.y += (pBri->m_dPRF_H1 + pBri->m_dPRF_H2);

			int nDir = bLeft ? 1 : 3;
			CString str1 = bInput ? _T("HuW1 X HuH1") : COMMA(dHuW)+_T(" X ")+COMMA(dHuH);
			CString str2 = bInput ? COMMA(dHuW)+_T(" X ")+COMMA(dHuH) : _T("");
			Sym.GiSiArrowExtendAng(&Dom, xyHuMid.x, xyHuMid.y, Dom.Always(10), -lr, nDir, str1, str2, CDPoint(0, 0));
		}
	}

	*pDomP << Dom;
}

void CDrawWingWall::MarkGenPlane(CDomyun* pDomP, BOOL bStt, BOOL bLeft, BOOL bInput)
{
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	double se = bStt ? -1.0 : 1.0;

	CDPoint vX = CDPoint(1, 0);
	CDPoint xyOrigin(0, 0);

	CTwinVectorArray tvArr;
	CTwinVector tv;

	tvArr = pBri->m_tvArrPlaneWingWall;

	tvArr.GetTvByInfo(_T("슬래브끝단외측"), tv);
	xyOrigin = bLeft ? tv.m_v1 : tv.m_v2;

	CDPoint vAngWing = pBri->GetAngleAzimuthWingWall(bStt, bLeft);
	CDPoint vRotate = vAngWing.BetweenAngle(vX*se);
	tvArr.Rotate(xyOrigin, vRotate);
	tvArr.Move(-xyOrigin);

	double dT1 = pWing->m_dT1;
	double dT2 = pWing->m_dT2;
	int nDirHunch = dT1==dT2 ? 1 : pWing->m_nDirHunch;
	if(nDirHunch==1)
	{
		dT1 = dT2;
	}
	double dDT = dT2-dT1;
	double dSH = pWing->m_dSH;
	double dHuW = pWing->m_dHuW2;
	double dHuH = pWing->m_dHuH2;

	CString strDir = bLeft ? _T("좌측") : _T("우측");

	CDomyun Dom(pDomP);
	CConcSymbol Sym;

	// 단면 변화부 경사
	if(dDT!=0)
	{
		tvArr.GetTvByInfo(strDir+_T("날개벽내부헌치"), tv);
		CDPoint xyVarFir = tv.m_v1;
		CDPoint xyVarSec = tv.m_v2;
		CDPoint vVar = bLeft ? (xyVarSec-xyVarFir).Unit() : (xyVarFir-xyVarSec).Unit();
		CDPoint xyVarMid = (xyVarFir+xyVarSec)/2.0 + (Dom.Always(1))*vVar.Rotate90();

		CString strSlope = _T("1:SH");
		if(!bInput)
			strSlope.Format(_T("1:%.1f"), dSH);
		Dom.SetTextAngle(vVar.GetAngleDegree());
		Dom.TextOut(xyVarMid, strSlope);
	}

	// 헌치
	if(dHuW!=0 && dHuH!=0)
	{
		tvArr.GetTvByInfo(strDir+_T("날개벽시점헌치"), tv);
		CDPoint xyHuFir = tv.m_v1;
		CDPoint xyHuSec = tv.m_v2;
		CDPoint xyHuMid = (xyHuFir+xyHuSec)/2.0;

		int nDir = 0;
		if(bStt)
			nDir = bLeft ? 3 : 5;
		else
			nDir = bLeft ? 7 : 1;
		CString str1 = bInput ? _T("HuW2 X HuH2") : COMMA(dHuW)+_T(" X ")+COMMA(dHuH);
		CString str2 = bInput ? COMMA(dHuW)+_T(" X ")+COMMA(dHuH) : _T("");
		Sym.GiSiArrowExtendAng(&Dom, xyHuMid.x, xyHuMid.y, Dom.Always(10), se, nDir, str1, str2, CDPoint(0, 0));
	}

	*pDomP << Dom;
}

// 날개벽 각도
void CDrawWingWall::DimGenAngle(CDomyun* pDomP, BOOL bStt, BOOL bInput)
{
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;
	CWingWall* pWingL = pBri->GetWingWall(bStt, TRUE);
	CWingWall* pWingR = pBri->GetWingWall(bStt, FALSE);

	CTwinVector tv;
	CTwinVectorArray tvArr;
	pBri->GetTvWingWallPlane(bStt);
	tvArr = pBri->m_tvArrPlaneWingWall;

	CDPoint vAngL = pBri->GetAngleAzimuthWingWall(bStt, TRUE);
	CDPoint vAngR = pBri->GetAngleAzimuthWingWall(bStt, FALSE);

	int nCountJijum = pBri->GetCountJijum();
	int nJ = bStt ? 0 : nCountJijum-1;
	int se = bStt ? -1 : 1;
	CDPoint vAngTanL = pBri->GetAngleAzimuthTanOnJijum(nJ, -se);
	CDPoint vAngTanR = pBri->GetAngleAzimuthTanOnJijum(nJ, se);

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetDimExo(0.0);

	double dR = Dom.Always(1.0);

	tvArr.GetTvByInfo(_T("슬래브끝단외측"), tv);
	CDPoint xy0(0, 0), xy1(0, 0), xy2(0, 0);
	CDPoint xyTemp(0, 0), vTemp(0, 0);
	CString str = _T(""), strAng = _T("");

	// 좌측
	if(pWingL->m_bExist)
	{
		xy0 = tv.m_v1;
		xy1 = bStt ? xy0 - dR*vAngTanL : xy0 + dR*vAngTanL;
		xy2 = xy0 + dR*vAngL;
		if(pWingL->m_vAng.y<0)
		{
			xyTemp = xy1;
			xy1 = xy2;
			xy2 = xyTemp;
		}
		xy1 = CDPoint(Round(xy1.x, 8), Round(xy1.y, 8));
		xy2 = CDPoint(Round(xy2.x, 8), Round(xy2.y, 8));
		vTemp = CDPoint(pWingL->m_vAng.x, fabs(pWingL->m_vAng.y));
		strAng.Format(_T("(%.3f˚)"), vTemp.GetAngleDegree());
		str = bInput ? _T("AWL")+strAng : vTemp.GetDegreeString(1, TRUE);
		if(bInput || vTemp.GetAngleDegree()!=0.0)
		{
			Dom.DimArcMoveTo(xy0.x, xy0.y, dR, xy1.x, xy1.y, 1);
			Dom.DimArcLineTo(xy2.x, xy2.y, 1, str);
		}
	}

	// 우측
	if(pWingR->m_bExist)
	{
		xy0 = tv.m_v2;
		xy1 = xy0 + dR*vAngR;
		xy2 = bStt ? xy0 - dR*vAngTanR : xy0 + dR*vAngTanR;
		if(pWingR->m_vAng.y<0)
		{
			xyTemp = xy1;
			xy1 = xy2;
			xy2 = xyTemp;
		}
		xy1 = CDPoint(Round(xy1.x, 8), Round(xy1.y, 8));
		xy2 = CDPoint(Round(xy2.x, 8), Round(xy2.y, 8));
		vTemp = CDPoint(pWingR->m_vAng.x, fabs(pWingR->m_vAng.y));
		strAng.Format(_T("(%.3f˚)"), vTemp.GetAngleDegree());
		str = bInput ? _T("AWR")+strAng : vTemp.GetDegreeString(1, TRUE);
		if(bInput || vTemp.GetAngleDegree()!=0.0)
		{
			Dom.DimArcMoveTo(xy0.x, xy0.y, dR, xy1.x, xy1.y, 1);
			Dom.DimArcLineTo(xy2.x, xy2.y, 1, str);
		}
	}

	*pDomP << Dom;
}

void CDrawWingWall::DimRebarFrontUpper(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut, BOOL bInput)
{
	CRcBridgeRebar* pBriRebar = m_pStd->m_pRcBridgeRebar;
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);
	
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	if(!pWing->m_bExist) return;

	BOOL bEnableUpper = (pWing->m_nCountLayerA[nOut] == 2 || pWing->m_nCountLayerD[nOut] == 2);
	CString strDan = _T("");
	CDPoint xyStt(0, 0);
	double dWidth = 0;
	long nDan = 0;
	long nMaxDan = pBriRebar->GetMaxWingWallDan(bStt, bLeft, nOut);
	for(nDan = 0; nDan < nMaxDan; nDan++)
	{
		if(bEnableUpper) strDan = (nDan == 0) ? _T("1단") : _T("2단");
		if(nDan == 1 && !bEnableUpper) continue;
		CDPointArray xyArr = pWing->m_xyArrUpper[nOut][nDan];
		CTwinVectorArray tvArr;
		CTwinVector tv;
		pBri->GetTvWingWallFront(bStt, bLeft, nOut, tvArr);

		tvArr.GetTvByInfo(_T("날개벽정면상단"), tv);

		//// new
		xyStt = tv.m_v2;
		xyStt.y = max(tv.m_v1.y, tv.m_v2.y);
		CDPoint vAng = pBri->GetAngleWingWallBetweenWall(bStt, bLeft);
		dWidth = fabs(tv.m_v2.x-tv.m_v1.x);
		double dCoverWingEnd = pBri->GetCoverWingWallEnd(bStt, bLeft);
		double dLenDim = 0;
		int nSize = xyArr.GetSize();

		CDPoint xyR(0, 0);
		CString str = _T(""), strLow = _T("");
		CString str1 = _T(""), str2 = _T("");

		Dom.DimMoveTo(xyStt.x, xyStt.y, nDan, COMMA(dCoverWingEnd));
		Dom.DimLineTo(dCoverWingEnd, nDan, _T(""));
		dLenDim += dCoverWingEnd;
		int i=0; for(i=0; i<nSize; i++)
		{
			xyR = xyArr.GetAt(i);
			str.Format(_T("U%d"), i+1);
			strLow = xyR.x==1 ? COMMA(xyR.y) : COMMA(xyR.x) + _T("@") + COMMA(xyR.y) + _T("=") + COMMA(xyR.x*xyR.y);
			str1 = bInput ? str : strLow;
			str2 = bInput ? strLow : _T("");
			Dom.DimLineToExtend(xyR.x*xyR.y, nDan, str1, str2);
			dLenDim += xyR.x*xyR.y;
		}
		if(tv.GetHorLength() > dLenDim) 
			Dom.DimLineTo(tv.GetHorLength()-dLenDim, nDan, COMMA(tv.GetHorLength()-dLenDim));

		if(bEnableUpper)
			Dom.DimLineToOver(strDan.GetLength(), nDan, strDan);
	}
	if(nMaxDan == 2 && !bEnableUpper) nDan--;
	Dom.DimMoveTo(xyStt.x, xyStt.y, nDan);
	Dom.DimLineTo(dWidth, nDan, COMMA(dWidth));
	
	if(!bStt)
		Dom.Mirror(xyStt.x, TRUE);
	
	*pDomP << Dom;
}

void CDrawWingWall::DimRebarFrontLower(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut, BOOL bInput)
{
	CRcBridgeRebar* pBriRebar = m_pStd->m_pRcBridgeRebar;
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;
	if(pWing->m_dL2==0) return;
	BOOL bBox = pBri->m_nTypeBridge == BRIDGE_TYPE_BOX;

	CTwinVectorArray tvArr;
	CTwinVector tv1, tv2;
	pBri->GetTvWingWallFront(bStt, bLeft, nOut, tvArr);

	tvArr.GetTvByInfo(_T("날개벽정면상단"), tv1);
	tvArr.GetTvByInfo(_T("날개벽정면끝단하부"), tv2);

	double dFH = bStt ? pBri->m_footingStt.m_dFH : pBri->m_footingEnd.m_dFH;
	
	//BOX형일때는 FH가 없으니. 부상방지판높이나 H2높이로 설정
	if(bBox && pBri->m_bExistProtectionRiseFooting)
		dFH = pBri->m_dPRF_H1;
	if(bBox && pWing->m_dL2 > 0 && pWing->m_dH2 > 0 && !pBri->m_bExistProtectionRiseFooting)
		dFH = pWing->m_dH2;

	CDPoint xyStt = CDPoint(tv2.m_v2.x, tv2.m_v2.y-dFH);
	CDPoint vAng = pBri->GetAngleWingWallBetweenWall(bStt, bLeft);
	double dWidth = fabs(tv2.m_v2.x-tv1.m_v1.x);
	double dCoverWingEnd = pBri->GetCoverWingWallEnd(bStt, bLeft);
	

	CString strDan = _T("");
	CDPoint xyR(0, 0);
	CString str = _T(""), strLow = _T("");
	CString str1 = _T(""), str2 = _T("");
	
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetDirection(_T("BOTTOM"));
	
	BOOL bEnableLower = (pWing->m_nCountLayerB[nOut] == 2 || pWing->m_nCountLayerC[nOut] == 2);
	long nMaxDan = pBriRebar->GetMaxWingWallDan(bStt, bLeft, nOut);
	long nDan = 0;
	for(nDan = 0 ; nDan < nMaxDan; nDan++)
	{
		double dLenDim = 0;
		if(bEnableLower) strDan = (nDan == 0) ? _T("1단") : _T("2단");
		if(nDan == 1 && !bEnableLower) continue;

		CDPointArray xyArr = pWing->m_xyArrLower[nOut][nDan];
		int nSize = xyArr.GetSize();
		Dom.DimMoveTo(xyStt.x, xyStt.y, nDan, COMMA(dCoverWingEnd));
		Dom.DimLineTo(dCoverWingEnd, nDan, _T(""));
		dLenDim += dCoverWingEnd;
		int i=0; for(i=0; i<nSize; i++)
		{
			xyR = xyArr.GetAt(i);
			str.Format(_T("L%d"), i+1);
			strLow = xyR.x==1 ? COMMA(xyR.y) : COMMA(xyR.x) + _T("@") + COMMA(xyR.y) + _T("=") + COMMA(xyR.x*xyR.y);
			str1 = bInput ? str : strLow;
			str2 = bInput ? strLow : _T("");
			Dom.DimLineToExtend(xyR.x*xyR.y, nDan, str1, str2);
			dLenDim += xyR.x*xyR.y;
		}
		if(dWidth > dLenDim) Dom.DimLineTo(dWidth-dLenDim, nDan, COMMA(dWidth-dLenDim));
		if(bEnableLower)
			Dom.DimLineToOver(strDan.GetLength(), nDan, strDan);
	}
	
	if(nMaxDan == 2 && !bEnableLower) nDan--;
	Dom.DimMoveTo(xyStt.x, xyStt.y, nDan);
	Dom.DimLineTo(dWidth, nDan, COMMA(dWidth));

	if(!bStt)
		Dom.Mirror(xyStt.x, TRUE);

	*pDomP << Dom;
}

void CDrawWingWall::DimRebarFrontSide(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut, BOOL bInput)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	CDPointArray xyArr;
	CTwinVectorArray tvArr;
	CTwinVector tvUpper, tvLower;
	pBri->GetTvWingWallFront(bStt, bLeft, nOut, tvArr);

	tvArr.GetTvByInfo(_T("날개벽정면상단"), tvUpper);
	tvArr.GetTvByInfo(_T("날개벽정면끝단하부"), tvLower);

	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX && pWing->m_dL2 > 0 && pWing->m_dH2 > 0)
		tvArr.GetTvByInfo(_T("날개벽정면하단벽체"), tvLower);
	
// 	double dPRFMin = 0;
	CTwinVectorArray tvArrPRF;
	CString strPRFBottom = bStt ? _T("시점부상방지저판하단") : _T("종점부상방지저판하단");
	CString strPRFTop = bStt ? _T("시점부상방지저판상면") : _T("종점부상방지저판상면");
	CTwinVector tvPRFBttom, tvPRFTop;
	CDPoint xyPRFBottomPos;
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX && pBri->m_bExistProtectionRiseFooting)
	{
		pBri->GetTvPRF_VertSection(tvArrPRF, bStt, bLeft, pBri->m_bSlopeLowerSlab);
		tvArrPRF.GetTvByInfo(strPRFBottom, tvPRFBttom);
		tvArrPRF.GetTvByInfo(strPRFTop, tvPRFTop);

		double dPRFMin = min(tvPRFBttom.m_v1.y, tvPRFBttom.m_v2.y);
		tvLower.m_v2.y = dPRFMin;
		if(pBri->IsWingWallOnPRF(bStt, bLeft))
		{
			GetXyMatchSegAndLine(tvPRFBttom.m_v1, tvPRFBttom.m_v2, tvLower.m_v2, CDPoint(0, 1), xyPRFBottomPos);
			tvLower.m_v2.y = xyPRFBottomPos.y;
		}
	}
	double dHgtStt = tvUpper.m_v1.y-tvLower.m_v2.y;
	double dHgtEnd = tvUpper.m_v2.y-tvLower.m_v2.y;
	double dHgtMax = max(dHgtStt, dHgtEnd);
	double dHgtMin = min(dHgtStt, dHgtEnd);
	BOOL bWingEnd = dHgtMax==dHgtEnd ? TRUE : FALSE;
	BOOL bSelMainCTCD = pWing->m_bSelMainCTCD;

	double dFH = bStt ? (pBri->m_footingStt.m_dFH + pBri->m_footingStt.m_dFSRH) : (pBri->m_footingEnd.m_dFH + pBri->m_footingEnd.m_dFSLH);
	if(pBri->IsBoxType()) dFH = 0;
	double dCoverUpper = pBri->m_dCoverWingUpper;
	double dCoverLower = pBri->GetCoverFooting(bStt, FALSE, 0);//pBri->m_dCoverLower_Footing[0]; 재호가 수정

	double dSpare = bStt ? 1900.0 : -1900.0;
	CDPoint xyR(0, 0);
	CString str = _T(""), strLow = _T("");
	CString str1 = _T(""), str2 = _T("");

	CDPoint xyStt(0, 0);

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	BOOL bDimStt = FALSE;
	int nDan = 0;
	long nMaxDan = pBri->GetMaxWingWallDan(bStt, bLeft, nOut);
	CString strDan = _T("");
	// 최대측 치수선
 	for(nDan = 0; nDan < nMaxDan; nDan++)
 	{
		xyArr = pWing->m_xyArrSide[nOut][nDan];

		CString strDirIn = dHgtStt>=dHgtEnd ? _T("LEFT") : _T("RIGHT");
		if(nMaxDan == 2) strDan = (nDan == 0) ? _T("  1단") : _T("  2단");

		if(bStt)
			strDirIn = dHgtStt<dHgtEnd ? _T("LEFT") : _T("RIGHT");
		Dom.SetDirection(strDirIn);

		xyStt.x = dHgtStt>=dHgtEnd ? tvUpper.m_v1.x+dSpare : tvUpper.m_v2.x;
		xyStt.y = tvLower.m_v2.y - dFH;

		bDimStt = FALSE;
		if(pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
		{
			Dom.DimMoveTo(xyStt.x, xyStt.y, nDan, COMMA(dCoverLower));
			Dom.DimLineToOver(dCoverLower, nDan, _T(""), 0);
			Dom.DimLineTo(dFH-dCoverLower, nDan, COMMA(dFH-dCoverLower));
		}
		else
		{
			double dPRFH = pBri->m_dPRF_H1;
			//BOX H2가 있을경우
			if(pWing->m_dL2 > 0 && pWing->m_dH2 && !pBri->m_bExistProtectionRiseFooting)
			{
				//치수선 이쁘게 하기 위해서 300이하는 밖으로 빼자.
				if(pWing->m_dH2 < 300)
				{
					Dom.DimMoveTo(xyStt.x, xyStt.y, nDan, COMMA(pWing->m_dH2));
					Dom.DimLineToOver(pWing->m_dH2, nDan, _T(""), 0);
				}
				else
				{
					Dom.DimMoveTo(xyStt.x, xyStt.y, nDan);
					Dom.DimLineTo(pWing->m_dH2, nDan, COMMA(pWing->m_dH2));
				}
			}
			//부상방지판이 있는데 L1이 큰경우
			else if(pBri->m_bExistProtectionRiseFooting && !pBri->IsWingWallOnPRF(bStt, bLeft))
			{
				Dom.DimMoveTo(xyStt.x, xyStt.y, nDan, COMMA(dCoverLower));
				Dom.DimLineToOver(dCoverLower, nDan, _T(""), 0);
				Dom.DimLineTo(dPRFH - dCoverLower, nDan, COMMA(dPRFH - dCoverLower));
			} 
			//부상장지판위에 날개벽(L1)
			else if(pBri->m_bExistProtectionRiseFooting && pBri->IsWingWallOnPRF(bStt, bLeft))
			{
				CDPoint xyPRFPos;
				GetXyMatchSegAndLine(tvPRFTop.m_v1, tvPRFTop.m_v2, tvLower.m_v2, CDPoint(0,1), xyPRFPos);
				double dPosHeight = xyPRFPos.y - tvPRFBttom.m_v1.y;
				if(pBri->m_bSlopeLowerSlab)
					dPosHeight = xyPRFPos.y - xyPRFBottomPos.y;

				Dom.DimMoveTo(xyStt.x, xyStt.y, nDan, COMMA(dCoverLower));
				Dom.DimLineToOver(dCoverLower, nDan, _T(""), 0);
				Dom.DimLineTo(dPosHeight - dCoverLower, nDan, COMMA(dPosHeight - dCoverLower));
			}
			else //기초도 없고, 그냥 s1부터 바로 시작하는애.
			{
				bDimStt = TRUE;
			}
		}

		int nSize = xyArr.GetSize();
		int i=0; for(i=0; i<nSize; i++)
		{
			xyR = xyArr.GetAt(i);
			str.Format(_T("S%d"), i+1);
			strLow = xyR.x==1 ? COMMA(xyR.y) : COMMA(xyR.x) + _T("@") + COMMA(xyR.y) + _T("=") + COMMA(xyR.x*xyR.y);
			if(i == nSize-1)
			{
				str1 = strLow;
				str2 = _T("");
			}
			else
			{
				str1 = bInput ? str : strLow;
				str2 = bInput ? strLow : _T("");
			}

			if(i == 0 && bDimStt && xyR.x*xyR.y < 300)
			{
				Dom.DimMoveToExt(xyStt.x, xyStt.y, nDan, str1, str2);
				Dom.DimLineToOver(xyR.x*xyR.y, nDan, _T(""), 0);
			}
			else if(i == 0 && bDimStt && xyR.x*xyR.y >= 300)
			{
				Dom.DimMoveTo(xyStt.x, xyStt.y, nDan);
				Dom.DimLineToExtend(xyR.x*xyR.y, nDan, str1, str2);
			}
			else
				Dom.DimLineToExtend(xyR.x*xyR.y, nDan, str1, str2);
		}
		Dom.DimLineToOver(dCoverUpper, nDan, COMMA(dCoverUpper) + strDan);
 	}

	bDimStt = FALSE;
	// 최대측 보조 치수선
	if(nOut==0 && bWingEnd && bSelMainCTCD && pWing->m_dL1>0)
	{
		pBri->GetDimArrayRebarWingWallMaxSide(xyArr, bStt, bLeft, nOut, 0);
		
		if(pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
		{
			Dom.DimMoveTo(xyStt.x, xyStt.y, nDan, COMMA(dCoverLower));
			Dom.DimLineToOver(dCoverLower, nDan, _T(""), 0);
			Dom.DimLineTo(dFH-dCoverLower, nDan, COMMA(dFH-dCoverLower));
		}
		else
		{
			double dPRFH = pBri->m_dPRF_H1;
			//BOX H2가 있을경우
			if(pWing->m_dL2 > 0 && pWing->m_dH2 && !pBri->m_bExistProtectionRiseFooting)
			{
				//치수선 이쁘게 하기 위해서 300이하는 밖으로 빼자.
				if(pWing->m_dH2 < 300)
				{
					Dom.DimMoveTo(xyStt.x, xyStt.y, nDan, COMMA(pWing->m_dH2));
					Dom.DimLineToOver(pWing->m_dH2, nDan, _T(""), 0);
				}
				else
				{
					Dom.DimMoveTo(xyStt.x, xyStt.y, nDan);
					Dom.DimLineTo(pWing->m_dH2, nDan, COMMA(pWing->m_dH2));
				}
			}
			//부상방지판이 있는데 L1이 큰경우
			else if(pBri->m_bExistProtectionRiseFooting && !pBri->IsWingWallOnPRF(bStt, bLeft))
			{
				Dom.DimMoveTo(xyStt.x, xyStt.y, nDan, COMMA(dCoverLower));
				Dom.DimLineToOver(dCoverLower, nDan, _T(""), 0);
				Dom.DimLineTo(dPRFH - dCoverLower, nDan, COMMA(dPRFH - dCoverLower));
			} 
			//부상장지판위에 날개벽(L1)있는경우
			else if(pBri->m_bExistProtectionRiseFooting && pBri->IsWingWallOnPRF(bStt, bLeft))
			{
				CDPoint xyPRFPos;
				GetXyMatchSegAndLine(tvPRFTop.m_v1, tvPRFTop.m_v2, tvLower.m_v2, CDPoint(0,1), xyPRFPos);
				double dPosHeight = xyPRFPos.y - tvPRFBttom.m_v1.y;
				if(pBri->m_bSlopeLowerSlab)
					dPosHeight = xyPRFPos.y - xyPRFBottomPos.y;

				Dom.DimMoveTo(xyStt.x, xyStt.y, nDan, COMMA(dCoverLower));
				Dom.DimLineToOver(dCoverLower, nDan, _T(""), 0);
				Dom.DimLineTo(dPosHeight - dCoverLower, nDan, COMMA(dPosHeight - dCoverLower));
			}
			else //기초도 없고, 그냥 s1부터 바로 시작하는애.
			{
				bDimStt = TRUE;
			}
		}

		long nSize = xyArr.GetSize();
		for(long i=0; i<nSize; i++)
		{
			xyR = xyArr.GetAt(i);
			str = xyR.x==1 ? COMMA(xyR.y) : COMMA(xyR.x) + _T("@") + COMMA(xyR.y) + _T("=") + COMMA(xyR.x*xyR.y);

			if(i == 0 && bDimStt && xyR.x*xyR.y < 300)
			{
				Dom.DimMoveTo(xyStt.x, xyStt.y, nDan, str);
				Dom.DimLineToOver(xyR.x*xyR.y, nDan, _T(""), 0);

			}
			else if(i == 0 && bDimStt && xyR.x*xyR.y >= 300)
			{
				Dom.DimMoveTo(xyStt.x, xyStt.y, nDan);
				Dom.DimLineTo(xyR.x*xyR.y, nDan, str);
			}
			else
				Dom.DimLineTo(xyR.x*xyR.y, nDan, str);
		}
		Dom.DimLineToOver(dCoverUpper, nDan, COMMA(dCoverUpper));
		nDan++;
	}

	//전체 치수선
	Dom.DimMoveTo(xyStt.x, xyStt.y, nDan);
	Dom.DimLineTo(dHgtMax+dFH, nDan, COMMA(dHgtMax+dFH));
	
	for(nDan = 0; nDan < nMaxDan; nDan++)
	{
		// 최소측 치수선
		bDimStt = FALSE;
		pBri->GetDimArrayRebarWingWallMinSide(xyArr, bStt, bLeft, nOut, nDan);
		CString strDirIn = dHgtStt<dHgtEnd ? _T("LEFT") : _T("RIGHT");
		if(bStt)
			strDirIn = dHgtStt>=dHgtEnd ? _T("LEFT") : _T("RIGHT");
		Dom.SetDirection(strDirIn);
	
		xyStt.x = dHgtStt<dHgtEnd ? tvUpper.m_v1.x+dSpare : tvUpper.m_v2.x;
		xyStt.y = tvLower.m_v2.y - dFH;
	
	
		if(pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
		{
			Dom.DimMoveTo(xyStt.x, xyStt.y, 0, COMMA(dCoverLower));
			Dom.DimLineToOver(dCoverLower, 0, _T(""), 0);
			Dom.DimLineTo(dFH-dCoverLower, 0, COMMA(dFH-dCoverLower));
		}
		else
		{
			double dPRFH = pBri->m_dPRF_H1;
			//BOX H2가 있을경우
			if(pWing->m_dL2 > 0 && pWing->m_dH2 && !pBri->m_bExistProtectionRiseFooting)
			{
				//치수선 이쁘게 하기 위해서 300이하는 밖으로 빼자.
				if(pWing->m_dH2 < 300)
				{
					Dom.DimMoveTo(xyStt.x, xyStt.y, 0, COMMA(pWing->m_dH2));
					Dom.DimLineToOver(pWing->m_dH2, 0, _T(""), 0);
				}
				else
				{
					Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
					Dom.DimLineTo(pWing->m_dH2, 0, COMMA(pWing->m_dH2));
				}
			}
			//부상방지판이 있는데 L1이 큰경우
			else if(pBri->m_bExistProtectionRiseFooting && !pBri->IsWingWallOnPRF(bStt, bLeft))
			{
				Dom.DimMoveTo(xyStt.x, xyStt.y, 0, COMMA(dCoverLower));
				Dom.DimLineToOver(dCoverLower, 0, _T(""), 0);
				Dom.DimLineTo(dPRFH - dCoverLower, 0, COMMA(dPRFH - dCoverLower));
			} 
			//부상장지판위에 날개벽(L1)있는경우
			else if(pBri->m_bExistProtectionRiseFooting && pBri->IsWingWallOnPRF(bStt, bLeft))
			{
				CDPoint xyPRFPos;
				GetXyMatchSegAndLine(tvPRFTop.m_v1, tvPRFTop.m_v2, tvLower.m_v2, CDPoint(0,1), xyPRFPos);
				double dPosHeight = xyPRFPos.y - tvPRFBttom.m_v1.y;
				if(pBri->m_bSlopeLowerSlab)
					dPosHeight = xyPRFPos.y - xyPRFBottomPos.y;
	
				Dom.DimMoveTo(xyStt.x, xyStt.y, 0, COMMA(dCoverLower));
				Dom.DimLineToOver(dCoverLower, 0, _T(""), 0);
				Dom.DimLineTo(dPosHeight - dCoverLower, 0, COMMA(dPosHeight - dCoverLower));
			}
			else
			{
				bDimStt = TRUE;
			}
		}
	
		long nSize = xyArr.GetSize();
		for(long i=0; i<nSize; i++)
		{
			xyR = xyArr.GetAt(i);
			str = xyR.x==1 ? COMMA(xyR.y) : COMMA(xyR.x) + _T("@") + COMMA(xyR.y) + _T("=") + COMMA(xyR.x*xyR.y);
	
			if(i == 0 && bDimStt && xyR.x*xyR.y < 300)
			{
				Dom.DimMoveTo(xyStt.x, xyStt.y, nDan, str);
				Dom.DimLineToOver(xyR.x*xyR.y, nDan, _T(""), 0);
			}
			else if(i == 0 && bDimStt && xyR.x*xyR.y >= 300)
			{
				Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
				Dom.DimLineTo(xyR.x*xyR.y, nDan, str);
			}
			else
				Dom.DimLineTo(xyR.x*xyR.y, nDan, str);
		}
		Dom.DimLineToOver(dCoverUpper, nDan, COMMA(dCoverUpper));
	}
	
	Dom.DimMoveTo(xyStt.x, xyStt.y, nDan);
	Dom.DimLineTo(dHgtMin+dFH, nDan, COMMA(dHgtMin+dFH));

	*pDomP << Dom;
}

// A구간 주철근(수평철근)
void CDrawWingWall::DrawRebarBlockAMainFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut, int nDan)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	CTwinVectorArray tvArr;
	pBri->GetTvArrayRebarWingWallBlockAMainFront(tvArr, bStt, bLeft, nOut, nDan);

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	m_pStd->DrawTvArray(&Dom, tvArr, m_pStd->IsViewVectorInfo());

	*pDomP << Dom;
}

// A구간 배력철근(수직철근)
void CDrawWingWall::DrawRebarBlockASupportFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut, int nDan)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	CTwinVectorArray tvArr;
	pBri->GetTvArrayRebarWingWallBlockASupportFront(tvArr, bStt, bLeft, nOut, nDan);

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	m_pStd->DrawTvArray(&Dom, tvArr, m_pStd->IsViewVectorInfo());

	*pDomP << Dom;
}

// B구간 주철근(수평철근)
void CDrawWingWall::DrawRebarBlockBMainFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut, int nDan)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	CTwinVectorArray tvArr;
	pBri->GetTvArrayRebarWingWallBlockBMainFront(tvArr, bStt, bLeft, nOut, nDan);

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	m_pStd->DrawTvArray(&Dom, tvArr, m_pStd->IsViewVectorInfo());

	*pDomP << Dom;
}

// C구간 주철근(수직철근)
void CDrawWingWall::DrawRebarBlockCMainFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut, int nDan)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	CTwinVectorArray tvArr1, tvArr2;
	pBri->GetTvArrayRebarWingWallBlockCMainFront(tvArr1, tvArr2, bStt, bLeft, nOut, nDan);

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARH);
	m_pStd->DrawTvArray(&Dom, tvArr1, m_pStd->IsViewVectorInfo());	// 곡선부

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	m_pStd->DrawTvArray(&Dom, tvArr2, m_pStd->IsViewVectorInfo());	// 직선부

	*pDomP << Dom;
}

// D구간 주철근(수평철근)
void CDrawWingWall::DrawRebarBlockDMainFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut, int nDan)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	CTwinVectorArray tvArr;
	pBri->GetTvArrayRebarWingWallBlockDMainFront(tvArr, bStt, bLeft, nOut, nDan);

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	m_pStd->DrawTvArray(&Dom, tvArr, m_pStd->IsViewVectorInfo());

	*pDomP << Dom;
}

// D구간 배력철근(수직철근)
void CDrawWingWall::DrawRebarBlockDSupportFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut, int nDan)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	CTwinVectorArray tvArr;
	pBri->GetTvArrayRebarWingWallBlockDSupportFront(tvArr, bStt, bLeft, nOut, nDan);

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	m_pStd->DrawTvArray(&Dom, tvArr, m_pStd->IsViewVectorInfo());

	*pDomP << Dom;
}

void CDrawWingWall::DrawBlockBoundary(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut)
{
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	double dL1 = pWing->m_dL1;
	double dL2 = pWing->m_dL2;
	double se = bStt ? -1.0 : 1.0;
	CString strPRFTop = bStt ? _T("시점부상방지저판상면") : _T("종점부상방지저판상면");
	CString strPRFBottom = bStt ? _T("시점부상방지저판하단") : _T("종점부상방지저판하단");

	CTwinVectorArray tvArr;
	CTwinVector tv;

	pBri->GetTvWingWallFront(bStt, bLeft, nOut, tvArr);

	// A구간
	CDPoint xyA1(0, 0), xyA2(0, 0);
	tvArr.GetTvByInfo(_T("날개벽정면노리"), tv);
	CDPoint xyWingTail = tv.m_v1;
	xyA1.x = tv.m_v1.x - pWing->m_dL1*se;
	xyA1.y = tv.m_v2.y;
//	xyA1 = tv.m_v2;
	tvArr.GetTvByInfo(_T("날개벽정면상단"), tv);
	GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, xyA1, CDPoint(0, 1), xyA2);

	// B구간
	CDPoint xyB1(0, 0), xyB2(0, 0);
	double dHgtB = pBri->GetLengthWingWallBlockBC(bStt, bLeft);
	CTwinVector tvEnd	= tvArr.GetTvByInfo("날개벽정면끝단하부");
	//double dHEnd	= tvEnd.GetVerLength();

	tvArr.GetTvByInfo(_T("날개벽정면하단"), tv);
	//BOX형이면 날개벽정면하단을 부상방지판 상면있는쪽으로 맞춰줌.
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX && pBri->m_bExistProtectionRiseFooting)
	{
		CTwinVectorArray tvArrPRF;
		CTwinVector tvPRFTop, tvPRFBottom;
		CDPoint xyPos;
		pBri->GetTvPRF_VertSection(tvArrPRF, bStt, bLeft, pBri->m_bSlopeLowerSlab);
		tvArrPRF.GetTvByInfo(strPRFTop, tvPRFTop);
		tvArrPRF.GetTvByInfo(strPRFBottom, tvPRFBottom);

		GetXyMatchSegAndLine(tvPRFTop.m_v1, tvPRFTop.m_v2, tvEnd.m_v1, CDPoint(0,1), xyPos);
		double dHeightH2 = tvPRFTop.GetVerLength();
		tv.m_v2.x -= pBri->m_dPRF_W * se;
		
		//날개벽이(L1)이 부상방지판 상면에 있을경우 W는 만나는점에서부터 벽체까지.
		if(pBri->IsWingWallOnPRF(bStt, bLeft))
		{
			CVector tvOutWall = bStt? tvPRFBottom.m_v2 : tvPRFBottom.m_v1;
			double dLengthCutPRF = fabs(xyPos.x - tvOutWall.x);

			tv.m_v2.x += dLengthCutPRF * se;
			dHeightH2 = tvPRFTop.m_v2.y - xyPos.y;
		}

		tv.m_v1.y += dHeightH2;
		tv.m_v2.y += dHeightH2;
	}

	// (ARCBRIDGE-2455) B구간 위치에표기
	// 날개벽 끝단의 높이가 dHgtB보다 작으면 날개벽 끝단의 높이로 dHgtB를 대신함(블럭표시 할때만.. 실제 철근은 정상적으로 구해지고있음)
	// if(dHEnd < dHgtB)
	//	 dHgtB	= dHEnd;
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX && pBri->m_bExistProtectionRiseFooting)
		dHgtB -= pBri->m_dPRF_H2;

	xyB1 = CDPoint(xyA1.x, tv.m_v1.y+dHgtB);
	xyB2 = CDPoint(tv.m_v2.x, tv.m_v2.y+dHgtB);
	xyA1.y = tv.m_v2.y;

	// C구간
	CDPoint xyC1(0, 0), xyC2(0, 0);
	xyC1 = CDPoint(xyB1.x, tv.m_v1.y+dHgtB);
	xyC2 = tv.m_v2;

	// 텍스트
	CDPoint xyTextA(0, 0), xyTextB(0, 0), xyTextC(0, 0), xyTextD(0, 0);
	CDPoint xyTemp(0, 0);
	tv.m_v1 = xyA2;
	tv.m_v2 = xyB2;
	xyTextA = tv.GetXyMid();

	BOOL bMatB = GetXyMatchSegAndLine(xyB1, xyC2, xyB2, CDPoint(-se, 1), xyTemp);
	if(bMatB)
	{
		tv.m_v1 = xyB2;
		tv.m_v2 = xyTemp;
		xyTextB = tv.GetXyMid();

		tv.m_v1 = CDPoint(xyB1.x, xyB1.y-dHgtB);
		tv.m_v2 = xyTemp;
		xyTextC = tv.GetXyMid();
	}	

	tv.m_v1 = xyWingTail;
	tv.m_v2 = CDPoint(xyA2.x, xyA2.y-(xyA2.y-xyA1.y)/4);
	xyTextD = tv.GetXyMid();

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONH);
	Dom.SetTextAlignHorz(TA_CENTER);
	Dom.SetTextHeight(4);

	if(dL1!=0)
	{
		if(pBri->IsExistWingWallBlockA(bStt, bLeft))
			Dom.LineTo(xyA1, xyA2);
		Dom.TextOut(xyTextD, _T("D"));
	}
	if(dL2!=0)
	{
		Dom.LineTo(xyC1, xyC2);
		
		if(pBri->IsExistWingWallBlockA(bStt, bLeft))
		{
			Dom.LineTo(xyB1, xyB2);
			Dom.TextOut(xyTextA, _T("A"));
		}
		if(bMatB)
		{
			Dom.TextOut(xyTextB, _T("B"));
			Dom.TextOut(xyTextC, _T("C"));
		}
	}

	*pDomP << Dom;
}

void CDrawWingWall::DrawRebarBlockAShearFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	CTwinVectorArray tvArr;
	CTwinVector tv;
	pBri->GetTvArrayRebarWingWallShearAFront(tvArr, bStt, bLeft, nOut);
	int nSize = tvArr.GetSize();
	BOOL bBack = nOut==0 ? FALSE : TRUE;

	if(nSize==0) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CRebarPlacing rb;
	GetTvArrShearRebar(tvArr, TRUE, FALSE, bBack, pWing->m_RebarShearDetailA.m_nType);

	CDRect rect	= tvArr.GetRect();
	CDPoint xyCen	= CDPoint(rect.left+rect.Width()/2.,rect.bottom+fabs(rect.Height())/2.);
	double dR	= min(fabs(rect.Width()), fabs(rect.Height()))/2. + 20;
	rb.GetTvArrayByCircleArea(tvArr, xyCen, dR, TRUE);
	
	CHgBaseDrawStd baseDraw;
	baseDraw.DrawTvArray(&Dom, tvArr);
	
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
	Dom.Circle(xyCen.x, xyCen.y, dR);

	*pDomP << Dom;
}

void CDrawWingWall::DrawRebarBlockBCShearFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	CTwinVectorArray tvArr;
	CTwinVector tv;
	pBri->GetTvArrayRebarWingWallShearBCFront(tvArr, bStt, bLeft, nOut);
	int nSize = tvArr.GetSize();
	BOOL bBack = nOut==0 ? FALSE : TRUE;

	if(nSize==0) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CRebarPlacing rb;
	GetTvArrShearRebar(tvArr, TRUE, FALSE, bBack, pWing->m_RebarShearDetailB.m_nType, pWing->m_RebarShearDetailC.m_nType);
		
	CDRect rect	= tvArr.GetRect();
	CDPoint xyCen	= CDPoint(rect.left+rect.Width()/2.,rect.bottom+fabs(rect.Height())/2.);
	double dR	= min(fabs(rect.Width()), fabs(rect.Height()))/2. + 20;
	rb.GetTvArrayByCircleArea(tvArr, xyCen, dR, TRUE);

	CHgBaseDrawStd baseDraw;
	baseDraw.DrawTvArray(&Dom, tvArr);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
	Dom.Circle(xyCen.x, xyCen.y, dR);

	*pDomP << Dom;
}

void CDrawWingWall::DrawRebarBlockDShearFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	CTwinVectorArray tvArr;
	CTwinVector tv;
	pBri->GetTvArrayRebarWingWallShearDFront(tvArr, bStt, bLeft, nOut);
	int nSize = tvArr.GetSize();
	BOOL bBack = nOut==0 ? FALSE : TRUE;

	if(nSize==0) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CRebarPlacing rb;
	GetTvArrShearRebar(tvArr, TRUE, FALSE, bBack, pWing->m_RebarShearDetailD.m_nType);

	CDRect rect	= tvArr.GetRect();
	CDPoint xyCen	= CDPoint(rect.left+rect.Width()/2.,rect.bottom+fabs(rect.Height())*2./3.);
	double dR	= min(fabs(rect.Width()), fabs(rect.Height()))/4. + 20;
	if(nSize == 1)
		dR = pWing->m_dStdVerCTC;
	rb.GetTvArrayByCircleArea(tvArr, xyCen, dR, TRUE);
	
	CHgBaseDrawStd baseDraw;
	baseDraw.DrawTvArray(&Dom, tvArr);
	
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
	Dom.Circle(xyCen.x, xyCen.y, dR);

	*pDomP << Dom;
}

void CDrawWingWall::DrawRebarHunchFootingFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist || nOut==1) return;

	CTwinVectorArray tvArr;
	pBri->GetTvArrayRebarWingWallHunchFootingFront(tvArr, bStt, bLeft);

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARH);

	m_pStd->DrawTvArray(&Dom, tvArr, m_pStd->IsViewVectorInfo());

	*pDomP << Dom;
}

void CDrawWingWall::DrawRebarHunchWallFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist || nOut==1) return;
	if(!(pWing->m_dHuH2 > 0 && pWing->m_dHuW2 > 0)) return;

	CTwinVectorArray tvArr;
	pBri->GetTvArrayRebarWingWallHunchWallFront(tvArr, bStt, bLeft);

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARH);

	m_pStd->DrawTvArray(&Dom, tvArr, m_pStd->IsViewVectorInfo());

	*pDomP << Dom;
}

void CDrawWingWall::DrawRebarHunchFootingSect(CDomyun* pDomP, BOOL bStt, BOOL bLeft)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist || pWing->m_dL2==0) return;
	if(pWing->m_dHuH1==0 || pWing->m_dHuW1==0) return;

	CTwinVectorArray tvArr;
	pBri->GetTvArrayRebarWingWallHunchFootingSect(tvArr, bStt, bLeft);

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	m_pStd->DrawTvArray(&Dom, tvArr);

	*pDomP << Dom;
}

void CDrawWingWall::DrawRebarHunchWallSect(CDomyun* pDomP, BOOL bStt, BOOL bLeft, BOOL bUpper)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	CTwinVectorArray tvArr;
	pBri->GetTvArrayRebarWingWallHunchWallPlane(tvArr, bStt, bLeft, bUpper);

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	double dDia = bUpper? pWing->m_dDiaHuUpper : pWing->m_dDiaHuLower;
	if(pWing->m_dHuH2 > 0 && pWing->m_dHuW2 > 0 && dDia > 0)
	{
		m_pStd->DrawTvArray(&Dom, tvArr);
	}

	*pDomP << Dom;
}

void CDrawWingWall::DrawRebarSection(CDomyun* pDomP, BOOL bStt, BOOL bLeft)
{
	CARcBridgeDataStd *pStd	= m_pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	CTwinVectorArray tvArrGen;
	CTwinVector tvOut, tvHuPrev, tvHuNext, tvHunch, tvEnd, tvLower, tvFoot, tvFootHunch;
	pBri->GetTvWingWallSection(bStt, bLeft, tvArrGen);

	tvArrGen.GetTvByInfo(_T("날개벽단면외측"), tvOut);
	tvArrGen.GetTvByInfo(_T("날개벽단면내측헌치전"), tvHuPrev);
	tvArrGen.GetTvByInfo(_T("날개벽단면내측헌치후"), tvHuNext);
	tvArrGen.GetTvByInfo(_T("날개벽단면내측헌치"), tvHunch);
	tvArrGen.GetTvByInfo(_T("날개벽단면노리끝"), tvEnd);
	tvArrGen.GetTvByInfo(_T("날개벽단면하단"), tvLower);
	tvArrGen.GetTvByInfo(_T("날개벽단면기초"), tvFoot);
	tvArrGen.GetTvByInfo(_T("날개벽단면기초헌치"), tvFootHunch);

	long nDan = 0;
	double dCoverWingOut = pBri->m_dCoverWingOut[0];
	double dCoverWingIn = pBri->m_dCoverWingIn[0];
	double dCoverWingTop = pBri->m_dCoverWingUpper;
	double dCoverWingEnd = pBri->m_dCoverWingEnd;
	double dCoverFootLower = pBri->GetCoverFooting(bStt, FALSE, 0); //pBri->m_dCoverLower_Footing[0]; 재호가 수정
	double dRF = pBri->GetWingWallVertRebarCurveRadius(bStt, bLeft, 1, nDan);
	double dRB = pBri->GetWingWallVertRebarCurveRadius(bStt, bLeft, 0, nDan);
	double dL2 = pWing->m_dL2;
	double dT1 = pWing->m_dT1;
	double dT2 = pWing->m_dT2;
	CDPoint vEnd = CDPoint(pWing->m_dSW, 1).Unit();
	double dDia = pWing->m_dDiaSupportA[0][nDan];
	double dCTC = pWing->m_dStdVerCTC/2;
	double dSettle = pStd->GetValueSettle(dDia, SETTLE_TENS_NORMAL, toCm(dCoverWingIn), toCm(dCTC));
	double lr = bLeft ? -1.0 : 1.0;
	CDPoint vX(1, 0), vY(0, 1), xyVar[3], xyVarDan[3];
	pBri->GetXyRebarWingWallVarySect(xyVar, bStt, bLeft);

	BOOL bBendF = pBri->IsWindWallSectionExistFoot(bStt, bLeft, 1, nDan);
	BOOL bBendB = pBri->IsWindWallSectionExistFoot(bStt, bLeft, 0, nDan);	

	//부상방지판이 있으면 날개벽단면기초에서 부상방지판 H2만큼 빼줘야함.
	CTwinVectorArray tvArrFront;
	CTwinVector tvNori;
	pBri->GetTvWingWallFront(bStt, bLeft, TRUE, tvArrFront);

	tvNori = tvArrFront.GetTvByInfo(_T("노리"), FALSE);
	BOOL bHorNori = tvNori.GetLength() > 0 && tvNori.GetVerLength() < 2? TRUE : FALSE;

	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		CTwinVector tvFrontLower;
		if(pBri->m_bExistProtectionRiseFooting)
		{
			tvArrFront.GetTvByInfo(_T("날개벽정면하단"), tvFrontLower);
			tvFoot.m_v1.y = tvFrontLower.m_v1.y;
		}
		else
		{
			tvArrFront.GetTvByInfo(_T("날개벽정면끝단하부"), tvFrontLower);
			tvFoot.m_v1.y = tvFrontLower.m_v2.y;
		}
	}
	
	int nDirHunch = pWing->m_nDirHunch;
	if(nDirHunch==2)
		dT1 = dT2;

	Offset(tvOut.m_v1, tvOut.m_v2, dCoverWingOut*lr);
	Offset(tvHuNext.m_v1, tvHuNext.m_v2, -dCoverWingIn*lr);
	Offset(tvHunch.m_v1, tvHunch.m_v2, -dCoverWingIn*lr);
	tvOut.m_v1.y -= dCoverWingTop;
	tvOut.m_v2.y += dCoverFootLower;
	tvHuPrev.m_v1 += (dCoverWingIn*vX*lr-dCoverWingTop*vY);
	tvHuNext.m_v2.y = tvOut.m_v2.y;
	if(pWing->m_bMatchBottom)
	{
		tvEnd.m_v1 += (-dCoverWingOut*vX*lr+dCoverWingTop*vY);
		tvEnd.m_v2 += (dCoverWingIn*vX*lr+dCoverWingTop*vY);
	}
	else
	{
		tvEnd.m_v1 += (-dCoverWingOut*vX*lr+dCoverWingEnd/vEnd.x*vY);
		tvEnd.m_v2 += (dCoverWingIn*vX*lr+dCoverWingEnd/vEnd.x*vY);
	}
	CDPoint xyCenF(tvOut.m_v2.x-dRF*lr, tvOut.m_v2.y+dRF);
	CDPoint xyCenB(tvHuNext.m_v2.x-dRB*lr, tvHuNext.m_v2.y+dRB);
	CDPoint vHu = tvHunch.GetXyDir();

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	//최대단수 찾기 
	long nMaxDan[2] = {0, 0}; //0:배면, 1:전면
	for(long iFB = 1; iFB >= 0; iFB--)
	{
		nMaxDan[iFB] = pBri->GetMaxWingWallDan(bStt, bLeft, iFB);
	}

	//A, B구간 수직철근 길이 (2단배근때문에)
	CTwinVectorArray tvArrVertRebar, tvArrCurve;
	double dALength[2] = {0,0}, dBLength[2] = {0,0};
	for(long iFB =1; iFB >= 0; iFB--)
	{
		pBri->GetTvArrayRebarWingWallBlockASupportFront(tvArrVertRebar, bStt, bLeft, iFB, 0);
		dALength[iFB] = tvArrVertRebar.GetLengthMax();
		pBri->GetTvArrayRebarWingWallBlockCMainFront(tvArrCurve, tvArrVertRebar, bStt, bLeft, iFB, 0);
		dBLength[iFB] = tvArrVertRebar.GetLengthMax();
	}

	CTwinVector tvDanOut, tvDanEnd;
	CDPoint xyDanCenF(0,0);
	// 전면 수직 철근
	for(long nDan = 0; nDan < nMaxDan[1]; nDan++)
	{
		//원래값 변경되지 않게 옮겨서 사용
		tvDanOut = tvOut;
		tvDanEnd = tvEnd;
		xyDanCenF = xyCenF;
		if(nDan == 1) //2단일때 1단과의 거리 
		{
			tvDanOut.m_v1.x -= 100*lr;
			tvDanOut.m_v2.x -= 100*lr;
			tvDanEnd.m_v1.x -= 100*lr;
			tvDanEnd.m_v2.x -= 100*lr;
			xyDanCenF.x -= 100*lr;
		}
	
		Dom.LineTo(tvDanOut.m_v1, tvHuPrev.m_v1);
		if(dL2==0)
		{
			Dom.LineTo(tvDanOut.m_v1, tvDanEnd.m_v1);
			Dom.LineTo(tvDanEnd.m_v1, tvDanEnd.m_v2);
		}
		else
		{
			//1단일때는 수직철근 다 그리지만, 
			//2단은 구간이 2단설정되있는지 확인해봐야함
			if(nDan == 0)
			{
				Dom.LineTo(tvDanOut.m_v1, tvDanOut.m_v2+dRF*vY);
				if(dRF > 0 || (bBendF && dRF ==0))
					Dom.LineTo(xyDanCenF.x, tvDanOut.m_v2.y, xyDanCenF.x-500*lr, tvDanOut.m_v2.y);
	
				if(bLeft)
					Dom.Arc(xyDanCenF, dRF, 180, 90);
				else
					Dom.Arc(xyDanCenF, dRF, 0, -90);
			}
			else
			{
				//A구간에 수직철근이 2단인지
				if(pWing->m_nCountLayerA[1] == 2)
				{
					Dom.LineTo(tvDanOut.m_v1, tvDanOut.m_v1 - vY * dALength[1]);
				}
				//B구간에 수직철근이 2단인지
				if(pWing->m_nCountLayerB[1] == 2)
				{
					Dom.LineTo(tvDanOut.m_v2+(dRF + dBLength[1])*vY, tvDanOut.m_v2+dRF*vY);
					if(dRF > 0|| (bBendF && dRF ==0))
						Dom.LineTo(xyDanCenF.x, tvDanOut.m_v2.y, xyDanCenF.x-500*lr, tvDanOut.m_v2.y);
	
					if(bLeft)
						Dom.Arc(xyDanCenF, dRF, 180, 90);
					else
						Dom.Arc(xyDanCenF, dRF, 0, -90);
				}
			}
		}
	}

	// 배면 수직 철근
	CTwinVector tvDanHuPrev, tvDanHuNext;
	CDPoint xyDanCenB(0,0);
	for(long nDan = 0; nDan < nMaxDan[0]; nDan++)
	{
		tvDanHuPrev = tvHuPrev;
		tvDanHuNext = tvHuNext;
		tvDanEnd = tvEnd;
		xyDanCenB = xyCenB;
	
		pBri->GetXyRebarWingWallVarySect(xyVarDan, bStt, bLeft, nDan);

		if(nDan == 1)
		{
			tvDanHuPrev.m_v1.x += 100*lr;
			tvDanHuPrev.m_v2.x += 100*lr;
			tvDanHuNext.m_v1.x += 100*lr;
			tvDanHuNext.m_v2.x += 100*lr;
			tvDanEnd.m_v1.x += 100*lr;
			tvDanEnd.m_v2.x += 100*lr;
			xyDanCenB.x += 100*lr;
		}
	
		if(dT1<dT2)
		{
			if(nDan == 0 || pWing->m_nCountLayerA[0] == 2)
			{
				Dom.LineTo(tvDanHuPrev.m_v1, xyVarDan[1]-dSettle*vY);
				Dom.LineTo(xyVarDan[2], xyVarDan[0]);
			}
			
			if(nDan == 0 || pWing->m_nCountLayerB[0] == 2)
			{
				if(nDan == 0 || pWing->m_nCountLayerA[0] == 2)
				{
					if(dL2==0)
						Dom.LineTo(xyVarDan[0], tvDanEnd.m_v2);
					else
					{
						Dom.LineTo(xyVarDan[0], tvDanHuNext.m_v2+dRB*vY);
						Dom.LineTo(xyDanCenB.x, tvDanHuNext.m_v2.y, xyDanCenB.x-500*lr, tvDanHuNext.m_v2.y);
					}
				}
				else
				{
					if(dL2==0)
						Dom.LineTo(xyVarDan[0], tvDanEnd.m_v2);
					else
					{
						Dom.LineTo(tvDanHuNext.m_v2+(dRB + dBLength[0])*vY, tvDanHuNext.m_v2+dRB*vY);
						Dom.LineTo(xyDanCenB.x, tvDanHuNext.m_v2.y, xyDanCenB.x-500*lr, tvDanHuNext.m_v2.y);
					}
				}
			}
		}
		else if(dT1>dT2)
		{
			Dom.LineTo(tvDanHuPrev.m_v1, xyVarDan[0]);
			Dom.LineTo(xyVarDan[0], xyVarDan[2]);
			if(dL2==0)
				Dom.LineTo(xyVarDan[1]+dSettle*vY, tvDanEnd.m_v2);
			else
			{
				Dom.LineTo(xyVarDan[1]+dSettle*vY, tvDanHuNext.m_v2+dRB*vY);
				Dom.LineTo(xyDanCenB.x, tvDanHuNext.m_v2.y, xyDanCenB.x-500*lr, tvDanHuNext.m_v2.y);
			}
		}
		else
		{
			if(dL2==0)
				Dom.LineTo(tvDanHuNext.m_v1-dCoverWingTop*vY, tvDanEnd.m_v2);
			else
			{
				if(nDan == 0)
				{
					Dom.LineTo(tvDanHuNext.m_v1-dCoverWingTop*vY, tvDanHuNext.m_v2+dRB*vY);
					if(dRB > 0 || (bBendB && dRB ==0))
						Dom.LineTo(xyDanCenB.x, tvDanHuNext.m_v2.y, xyDanCenB.x-500*lr, tvDanHuNext.m_v2.y);
				}
				else
				{
					//A구간에 수직철근이 2단인지
					if(pWing->m_nCountLayerA[0] == 2)
					{
						Dom.LineTo(tvDanHuNext.m_v1-dCoverWingTop*vY, tvDanHuNext.m_v1-(dCoverWingTop + dALength[0])*vY);
					}
					//B구간에 수직철근이 2단인지
					if(pWing->m_nCountLayerB[0] == 2)
					{
						Dom.LineTo(tvDanHuNext.m_v2+dRB*vY, tvDanHuNext.m_v2+(dRB + dBLength[0])*vY);
						if(dRB > 0 || (bBendB && dRB ==0))
							Dom.LineTo(xyDanCenB.x, tvDanHuNext.m_v2.y, xyDanCenB.x-500*lr, tvDanHuNext.m_v2.y);
					}
				}
			}
		}
	
		if(nDan == 0 || (nDan == 1 && pWing->m_nCountLayerB[0] == 2))
		{
			if(bLeft)
				Dom.Arc(xyDanCenB, dRB, 180, 90);
			else
				Dom.Arc(xyDanCenB, dRB, 0, -90);
		}
	}

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_BARX);
	CDPointArray xyArr;
	CDPoint xyR(0, 0), xy(0, 0);
	int i = 0, nSize = 0;
	double dFristRadius = bHorNori? +2.0 : +1.0;

	// 전면 수평 철근
	for(long nDan = 0; nDan < nMaxDan[1]; nDan++)
	{
		tvDanOut = tvOut;
		if(nDan == 1)
		{
			tvDanOut.m_v1.x -= 100*lr;
			tvDanOut.m_v2.x -= 100*lr;
		}

		xy = CDPoint(tvDanOut.m_v2.x-REBAR_RADIUS*lr, tvFoot.m_v1.y);
		pBri->GetDimArrayRebarWingWallSttSide(xyArr, bStt, bLeft, 1, nDan);
		nSize = xyArr.GetSize();
		
		for(i=0; i<nSize; i++)
		{
			xyR = xyArr.GetAt(i);
			for(int j=0; j<xyR.x; j++)
			{
				xy.y += xyR.y;
				if(bHorNori && i ==0 && j == 0)
					xy.y += REBAR_RADIUS;

				if((i==nSize-1 && j==xyR.x-1))
					xy.y -= REBAR_RADIUS;

				CHgBaseDrawStd::InsertHCadBlock(&Dom, HCAD_E05, xy, Dom.GetScaleDim());	

				if(bHorNori && i ==0 && j == 0)
					xy.y -= REBAR_RADIUS;
			}
		}
	}
	
	// 배면 수평 철근
	double dBound1 = dT1<=dT2 ? xyVar[0].y : xyVar[1].y;
	double dBound2 = dT1<=dT2 ? xyVar[1].y : xyVar[0].y;
	for(long nDan = 0; nDan < nMaxDan[0]; nDan++)
	{
		tvDanHuPrev = tvHuPrev;
		tvDanHuNext = tvHuNext;

		pBri->GetXyRebarWingWallVarySect(xyVarDan, bStt, bLeft, nDan);

		if(nDan == 1)
		{
			tvDanHuPrev.m_v1.x += 100*lr;
			tvDanHuPrev.m_v2.x += 100*lr;
			tvDanHuNext.m_v1.x += 100*lr;
			tvDanHuNext.m_v2.x += 100*lr;
		}
	
		xy = CDPoint(tvDanHuNext.m_v2.x+REBAR_RADIUS*lr, tvFoot.m_v1.y);
		pBri->GetDimArrayRebarWingWallSttSide(xyArr, bStt, bLeft, 0, nDan);
		nSize = xyArr.GetSize();
	
		for(i=0; i<nSize; i++)
		{
			xyR = xyArr.GetAt(i);
			for(int j=0; j<xyR.x; j++)
			{
				xy.y += xyR.y;
				if(xy.y>dBound1 && xy.y<dBound2)
				{
					GetXyMatchSegAndLine(xyVarDan[0], xyVarDan[1], xy, vX, xy);
					xy.x -= REBAR_RADIUS/vHu.y*lr;
				}
				else if(xy.y>=dBound2)
					xy.x = tvDanHuPrev.m_v1.x + REBAR_RADIUS*lr;

				if(bHorNori && i ==0 && j == 0)
					xy.y += REBAR_RADIUS;

				if(i==nSize-1 && j==xyR.x-1)
					xy.y -= REBAR_RADIUS;

				CHgBaseDrawStd::InsertHCadBlock(&Dom, HCAD_E05, xy, Dom.GetScaleDim());	

				if(bHorNori && i ==0 && j == 0)
					xy.y -= REBAR_RADIUS;
			}
		}
	}
	// 전단 철근
	CTwinVectorArray tvArrA, tvArrShear;
	if(dL2==0)
		pBri->GetTvArrayRebarWingWallShearDFront(tvArrShear, bStt, bLeft, 0);
	else
	{
		pBri->GetTvArrayRebarWingWallShearAFront(tvArrA, bStt, bLeft, 0);
		pBri->GetTvArrayRebarWingWallShearBCFront(tvArrShear, bStt, bLeft, 0);
		tvArrShear.AddFromTvArray(tvArrA);
		
		if(tvArrShear.IsEmpty())
		{
			pBri->GetTvArrayRebarWingWallShearDFront(tvArrShear, bStt, bLeft, 0);
		}
	}

	CDPoint xy1(tvOut.m_v2.x, 0), xy2(tvHuNext.m_v2.x, 0);
	nSize = tvArrShear.GetSize();
	if(nSize > 0)
	{
		int nCount = 0;
		double dSttY = 0;

		for(i=0; i<nSize; i++)
		{
			xy1.y = xy2.y = tvArrShear.GetAt(i).m_v1.y;
			if(xy1.y!=dSttY)
				dSttY = xy1.y;
			else continue;
			if(xy1.y>dBound1 && xy1.y<dBound2)
				GetXyMatchSegAndLine(xyVar[0], xyVar[1], xy2, vX, xy2);
			else if(xy1.y>=dBound2)
				xy2.x = tvHuPrev.m_v1.x;

			if(nCount%2==0)
				m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
			else
				m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_CENTER2_LINE);
			Dom.LineTo(xy1, xy2);
			nCount++;
		}
		// 가장 위쪽의 전단 철근 표현
		if(IsNeedTopRebarShearSection(pWing))
		{
			xy1.y = xy2.y = tvArrShear.GetAt(nSize-1).m_v2.y;
			if(xy1.y>dBound1 && xy1.y<dBound2)
				GetXyMatchSegAndLine(xyVar[0], xyVar[1], xy2, vX, xy2);
			else if(xy1.y>=dBound2)
				xy2.x = tvHuPrev.m_v1.x;
			if(nCount%2==0)
				m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
			else
				m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_CENTER2_LINE);
			Dom.LineTo(xy1, xy2);
		}
	}

	// 헌치 철근
	DrawRebarHunchFootingSect(&Dom, bStt, bLeft);

	*pDomP << Dom;
}

void CDrawWingWall::DimRebarSectIn(CDomyun* pDomP, BOOL bStt, BOOL bLeft)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	BOOL bSide		= (pWing->m_nAttachPos == 0 )? FALSE : TRUE;
	CDPointArray xyArr;
	CTwinVectorArray tvArr, tvArrHu;
	CTwinVector tvUpper, tvLower;
	pBri->GetTvArrayRebarWingWallHunchFootingSect(tvArrHu, bStt, bLeft);
	CDPoint xyHu1 = tvArrHu.GetAt(0).m_v1;
	CDPoint xyHu2 = tvArrHu.GetAt(2).m_v2;
	double dT2 = pWing->m_dT2;
	double dHuW = pWing->m_dHuW1;
	double dHuH = pWing->m_dHuH1;
	double dCoverWingOut = pBri->m_dCoverWingOut[0];
	double dSpare = (!bSide)? fabs(xyHu1.x-xyHu2.x) + dCoverWingOut*2 - (dT2+dHuW)
		: fabs(xyHu1.x-xyHu2.x) + dCoverWingOut*2 + (dT2+dHuW)+1000;
	
	if(pWing->m_dL2==0 || dHuW==0 || dHuH==0)
		dSpare = 1000;

	pBri->GetTvWingWallSection(bStt, bLeft, tvArr, dSpare);
	tvArr.GetTvByInfo(_T("날개벽단면상단"), tvUpper);
	tvArr.GetTvByInfo(_T("날개벽단면하단"), tvLower);

	CDPoint xyR(0, 0);
	CString str1 = _T(""), str2 = _T("");
	double dFH = bStt ? (pBri->m_footingStt.m_dFH + pBri->m_footingStt.m_dFSRH) : (pBri->m_footingEnd.m_dFH + pBri->m_footingEnd.m_dFSLH);
	double dCoverUpper = pBri->m_dCoverWingUpper;
	double dCoverLower = bStt ? pBri->m_footingStt.m_dCoverLower[0] : pBri->m_footingEnd.m_dCoverLower[0];//pBri->m_dCoverLower_Footing[0];
	CDPoint xyStt = tvLower.m_v2;
	BOOL bEnable2Dan = (pWing->m_nCountLayerA[0] == 2 || pWing->m_nCountLayerB[0]  == 2 || pWing->m_nCountLayerD[0] == 2);

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	long nDan = 0;
	long nMaxDan = pBri->GetMaxWingWallDan(bStt, bLeft, FALSE);
	for(nDan = 0; nDan < nMaxDan; nDan++)
	{
		pBri->GetDimArrayRebarWingWallSttSide(xyArr, bStt, bLeft, 0, nDan);
		CString strDir = bLeft ? _T("RIGHT") : _T("LEFT");
		CString strDan = _T("");
		if(bEnable2Dan)	
			strDan = (nDan == 0) ? _T("  1단") : _T("  2단");
		Dom.SetDirection(strDir);

		BOOL bDimStt = FALSE;
		if(pWing->m_dL2==0 && pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
		{
			Dom.DimMoveTo(xyStt.x, xyStt.y, nDan);
			Dom.DimLineTo(dFH, nDan, COMMA(dFH));
		}
		else
		{
			if(pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
			{
				Dom.DimMoveTo(xyStt.x, xyStt.y, nDan, COMMA(dCoverLower));
				Dom.DimLineToOver(dCoverLower, nDan, _T(""), 0);
				Dom.DimLineTo(dFH-dCoverLower, nDan, COMMA(dFH-dCoverLower));
			}
			else
			{
				//BOX H2있을경우
				if(pWing->m_dL2 > 0 && pWing->m_dH2 > 0)
				{
					//치수선 이쁘게 하기 위해서 300이하는 밖으로 빼자.
					if(pWing->m_dH2 < 300)
					{
						Dom.DimMoveTo(xyStt.x, xyStt.y, nDan, COMMA(pWing->m_dH2));
						Dom.DimLineToOver(pWing->m_dH2, nDan, _T(""), 0);
					}
					else
					{
						Dom.DimMoveTo(xyStt.x, xyStt.y, nDan);
						Dom.DimLineTo(pWing->m_dH2, nDan, COMMA(pWing->m_dH2));
					}
				}
				else if(pBri->m_bExistProtectionRiseFooting)
				{
					CTwinVectorArray tvArrFront;
					pBri->GetTvWingWallFront(bStt, bLeft, TRUE, tvArrFront);

					CTwinVector tvFrontLower;
					tvArrFront.GetTvByInfo(_T("날개벽정면하단"), tvFrontLower);

					double dPRFH = tvFrontLower.m_v1.y - tvLower.m_v1.y;

					Dom.DimMoveTo(xyStt.x, xyStt.y, nDan, COMMA(dCoverLower));
					Dom.DimLineToOver(dCoverLower, nDan, _T(""), 0);
					Dom.DimLineTo(dPRFH - dCoverLower, nDan, COMMA(dPRFH - dCoverLower));
				}
				else
				{
					bDimStt = TRUE;
				}
			}
		}
		int nSize = xyArr.GetSize();
		int i=0; for(i=0; i<nSize; i++)
		{
			xyR = xyArr.GetAt(i);
			str1 = xyR.x==1 ? COMMA(xyR.y) : COMMA(xyR.x) + _T("@") + COMMA(xyR.y) + _T("=") + COMMA(xyR.x*xyR.y);

			if(i == 0 && bDimStt && xyR.x*xyR.y < 300)
			{
				Dom.DimMoveTo(xyStt.x, xyStt.y, nDan, str1);
				Dom.DimLineToOver(xyR.x*xyR.y, nDan, _T(""), 0);

			}
			else if(i == 0 && bDimStt && xyR.x*xyR.y >= 300)
			{
				Dom.DimMoveTo(xyStt.x, xyStt.y, nDan);
				Dom.DimLineTo(xyR.x*xyR.y, nDan, str1);
			}
			else
				Dom.DimLineTo(xyR.x*xyR.y, nDan, str1);
		}
		Dom.DimLineToOver(dCoverUpper, nDan, COMMA(dCoverUpper) + strDan);
	}

	Dom.DimMoveTo(xyStt.x, xyStt.y, nDan);
	Dom.DimLineTo(tvUpper.m_v2.y-tvLower.m_v2.y, nDan, COMMA(tvUpper.m_v2.y-tvLower.m_v2.y));

	*pDomP << Dom;
}

void CDrawWingWall::DimRebarSectOut(CDomyun* pDomP, BOOL bStt, BOOL bLeft)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	CDPointArray xyArr;
	CTwinVectorArray tvArr;
	CTwinVector tvUpper, tvLower;
	pBri->GetTvWingWallSection(bStt, bLeft, tvArr);
	tvArr.GetTvByInfo(_T("날개벽단면상단"), tvUpper);
	tvArr.GetTvByInfo(_T("날개벽단면하단"), tvLower);

	double dFH = bStt ? (pBri->m_footingStt.m_dFH + pBri->m_footingStt.m_dFSRH) : (pBri->m_footingEnd.m_dFH + pBri->m_footingEnd.m_dFSLH);
	double dCoverUpper = pBri->m_dCoverWingUpper;
	double dCoverLower = pBri->GetCoverFooting(bStt, FALSE, 0);//pBri->m_dCoverLower_Footing[0]; 재호가 수정
	CDPoint xyStt = tvLower.m_v1;
	BOOL bEnable2Dan = (pWing->m_nCountLayerA[1] == 2 || pWing->m_nCountLayerB[1]  == 2 || pWing->m_nCountLayerD[1] == 2);

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	long nMaxDan = pBri->GetMaxWingWallDan(bStt, bLeft, TRUE);
	long nDan = 0;
	for(nDan = 0; nDan < nMaxDan; nDan++)
	{
		pBri->GetDimArrayRebarWingWallSttSide(xyArr, bStt, bLeft, 1, nDan);
		CString strDir = bLeft ? _T("LEFT") : _T("RIGHT");
		CString strDan = _T("");
		if(bEnable2Dan)
			strDan = (nDan == 0) ? _T("  1단") : _T("  2단");
		Dom.SetDirection(strDir);

		BOOL bDimStt = FALSE;
		if(pWing->m_dL2==0 && pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
		{
			Dom.DimMoveTo(xyStt.x, xyStt.y, nDan+1);
			Dom.DimLineTo(dFH, nDan+1, COMMA(dFH));
		}
		else
		{
			if(pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
			{
				Dom.DimMoveTo(xyStt.x, xyStt.y, nDan+1, COMMA(dCoverLower));
				Dom.DimLineToOver(dCoverLower, nDan+1, _T(""), 0);
				Dom.DimLineTo(dFH-dCoverLower, nDan+1, COMMA(dFH-dCoverLower));
			}
			else
			{
				//BOX H2있을경우
				if(pWing->m_dL2 > 0 && pWing->m_dH2 > 0)
				{
					if(pWing->m_dH2 < 300)
					{
						Dom.DimMoveTo(xyStt.x, xyStt.y, nDan+1, COMMA(pWing->m_dH2));
						Dom.DimLineToOver(pWing->m_dH2, nDan+1, _T(""), 0);
					}
					else
					{
						Dom.DimMoveTo(xyStt.x, xyStt.y, nDan+1);
						Dom.DimLineTo(pWing->m_dH2, nDan+1, COMMA(pWing->m_dH2));
					}
				}
				else if(pBri->m_bExistProtectionRiseFooting)
				{
					CTwinVectorArray tvArrFront;
					pBri->GetTvWingWallFront(bStt, bLeft, TRUE, tvArrFront);

					CTwinVector tvFrontLower;
					tvArrFront.GetTvByInfo(_T("날개벽정면하단"), tvFrontLower);

					double dPRFH = tvFrontLower.m_v1.y - tvLower.m_v1.y;

					Dom.DimMoveTo(xyStt.x, xyStt.y, nDan+1, COMMA(dCoverLower));
					Dom.DimLineToOver(dCoverLower, nDan+1, _T(""), 0);
					Dom.DimLineTo(dPRFH - dCoverLower, nDan+1, COMMA(dPRFH - dCoverLower));
				}
				else
				{
					bDimStt = TRUE;
				}
			}
		}
		int nSize = xyArr.GetSize();
		CDPoint xyR(0, 0);
		CString str1 = _T(""), str2 = _T("");
		int i=0; for(i=0; i<nSize; i++)
		{
			xyR = xyArr.GetAt(i);
			str1 = xyR.x==1 ? COMMA(xyR.y) : COMMA(xyR.x) + _T("@") + COMMA(xyR.y) + _T("=") + COMMA(xyR.x*xyR.y);

			if(i == 0 && bDimStt && xyR.x*xyR.y < 300)
			{
				Dom.DimMoveTo(xyStt.x, xyStt.y, nDan+1, str1);
				Dom.DimLineToOver(xyR.x*xyR.y, nDan+1, _T(""), 0);
			}
			else if(i == 0 && bDimStt && xyR.x*xyR.y >= 300)
			{
				Dom.DimMoveTo(xyStt.x, xyStt.y, 1);
				Dom.DimLineTo(xyR.x*xyR.y, nDan+1, str1);
			}
			else
				Dom.DimLineTo(xyR.x*xyR.y, nDan+1, str1);
		}
		Dom.DimLineToOver(dCoverUpper, nDan+1, COMMA(dCoverUpper) + strDan);

	}
	Dom.DimMoveTo(xyStt.x, xyStt.y, nDan + 1);
	Dom.DimLineTo(tvUpper.m_v2.y-tvLower.m_v2.y, nDan+1, COMMA(tvUpper.m_v2.y-tvLower.m_v2.y));
	*pDomP << Dom;
}

void CDrawWingWall::DimRebarSectTop(CDomyun* pDomP, BOOL bStt, BOOL bLeft)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	CDPointArray xyArr;
	CTwinVectorArray tvArr;
	CTwinVector tvUpper;
	pBri->GetTvWingWallSection(bStt, bLeft, tvArr);
	tvArr.GetTvByInfo(_T("날개벽단면상단"), tvUpper);

	double dT1 = pWing->m_dT1;
	double dT2 = pWing->m_dT2;
	if(dT1>dT2)
	{
		dT1 = pWing->m_dT2;
		dT2 = pWing->m_dT1;
	}

	int nDirHunch = pWing->m_nDirHunch;
	if(nDirHunch==2)
		dT1 = dT2;

	double dCoverIn = pBri->m_dCoverWingIn[0];
	double dCoverOut = pBri->m_dCoverWingOut[0];
	CDPoint xyStt = tvUpper.m_v1;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	Dom.SetDirection(_T("TOP"));
	Dom.DimMoveTo(xyStt.x, xyStt.y, 0, COMMA(dCoverOut));
	Dom.DimLineTo(dCoverOut, 0, _T(""));
	Dom.DimLineTo(dT1-dCoverOut-dCoverIn, 0, COMMA(dT1-dCoverOut-dCoverIn));
	Dom.DimLineTo(dT2-dT1, 0, COMMA(dT2-dT1));
	Dom.DimLineToOver(dCoverIn, 0, COMMA(dCoverIn));

	Dom.DimMoveTo(xyStt.x, xyStt.y, 1);
	Dom.DimLineTo(dT2, 1, COMMA(dT2));

	if(!bLeft)
		Dom.Mirror(xyStt.x, TRUE);

	*pDomP << Dom;
}

void CDrawWingWall::DrawRebarPlane(CDomyun* pDomP, BOOL bStt, BOOL bLeft, BOOL bUpper)
{
	// 2단 철근이잘못되 있음.. 일단 일반적으로 사용하는 배면 하부만 2단철근 적용할때만 수정해 놓겠음.
	CARcBridgeDataStd *pStd	= m_pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	CTwinVectorArray tvArrGen;
	CTwinVector tvOut, tvEnd, tvHuPrev, tvHuNext, tvHunch, tvSlab, tvFoot;

	CTwinVector tvSlabNoOffset;	// tvSlab의 offset하지 않은 정보(tvWallSide 구할때 사용 added by jaeho 2009.04.13)
	
	pBri->GetTvWingWallPlane(bStt, TRUE, bUpper, bUpper);
	tvArrGen = pBri->m_tvArrPlaneWingWall;

	CString strDir = bLeft ? _T("좌측") : _T("우측");
	tvArrGen.GetTvByInfo(strDir+_T("날개벽외측"), tvOut);
	tvArrGen.GetTvByInfo(strDir+_T("날개벽끝단"), tvEnd);
	tvArrGen.GetTvByInfo(strDir+_T("날개벽내부헌치전"), tvHuPrev);
	tvArrGen.GetTvByInfo(strDir+_T("날개벽내부헌치"), tvHunch);
	tvArrGen.GetTvByInfo(strDir+_T("날개벽내부헌치후"), tvHuNext);
	tvArrGen.GetTvByInfo(_T("슬래브끝단외측"), tvSlab);
	tvSlabNoOffset	= tvSlab;

	BOOL bSide		= (pWing->m_nAttachPos == 0 )? FALSE : TRUE;
	BOOL bRangEnd = pWing->m_bRAngEnd;

	long nDan = 0;
	double dCoverWingOut = pBri->m_dCoverWingOut[0];
	double dCoverWingIn = pBri->m_dCoverWingIn[0];
	double dCoverWingEnd = pBri->GetCoverWingWallEnd(bStt, bLeft, FALSE);
	double dCoverSlab = pBri->m_dCoverWallIn[0];
	double dT1 = pWing->m_dT1;
	double dT2 = pWing->m_dT2;
	double dLT = bUpper ? pWing->m_dLT : 0.0;
	double dDiaVar = pWing->m_dL1>0 ? pWing->m_dDiaMainD[0][nDan] : pWing->m_dDiaMainA[0][nDan];
	double dDiaIn = bUpper ? pWing->m_dDiaMainA[0][nDan] : pWing->m_dDiaMainB[0][nDan];
	double dDiaOut = bUpper ? pWing->m_dDiaMainA[1][nDan] : pWing->m_dDiaMainB[1][nDan];
	if(pWing->m_dL2==0)
	{
		dDiaVar = pWing->m_dDiaMainD[0][nDan];
		dDiaIn = pWing->m_dDiaMainD[0][nDan];
		dDiaOut = pWing->m_dDiaMainD[1][nDan];
	}
	double dCTCIn = pWing->m_dMainCTCA/2;
	double dCTCOut = pWing->m_dMainCTCA;
	double dSettleVar = pStd->GetValueSettle(dDiaVar, SETTLE_TENS_NORMAL, toCm(dCoverWingIn), toCm(dCTCIn));
	double dSettleOut = pStd->GetValueSettle(dDiaOut, SETTLE_TENS_NORMAL, toCm(dCoverSlab), toCm(dCTCOut));
	double dSettleIn = pStd->GetValueSettle(dDiaIn, SETTLE_TENS_NORMAL, toCm(dCoverSlab), toCm(dCTCIn));
	double se = bStt ? -1.0 : 1.0;
	double lr = bLeft ? -1.0 : 1.0;
	CDPoint vX(1, 0), vY(0, 1);
	CDPoint xyHu[3];
	pBri->GetXyRebarWingWallVaryPlane(xyHu, bStt, bLeft);

	int nDirHunch = dT1==dT2 ? 1 : pWing->m_nDirHunch;
	if(nDirHunch==1)
	{
		dT1 = dT2 = bUpper ? pWing->m_dT1 : pWing->m_dT2;
		dLT = 0.0;
	}

	CDPoint xyOrigin = bLeft ? tvSlab.m_v1 : tvSlab.m_v2;
	CDPoint xyShearStt = tvOut.m_v1;
	Offset(tvSlab.m_v1, tvSlab.m_v2, -dCoverSlab);
	CDPoint xySttOut(0, 0), xySttIn(0, 0);
	xySttOut = tvEnd.m_v1;
	xySttIn = tvEnd.m_v2;

	Offset(tvOut.m_v1, tvOut.m_v2, -dCoverWingOut*lr);
	Offset(tvEnd.m_v1, tvEnd.m_v2, -dCoverWingEnd*lr);
	Offset(tvHuNext.m_v1, tvHuNext.m_v2, -dCoverWingIn*lr);
	Offset(tvHunch.m_v1, tvHunch.m_v2, -dCoverWingIn*lr);
	Offset(tvHuPrev.m_v1, tvHuPrev.m_v2, -dCoverWingIn*lr);

	CDPoint vOut = tvOut.GetXyDir();
	CDPoint vEnd = tvEnd.GetXyDir();
	CDPoint vHuPrev = tvHuPrev.GetXyDir();
	CDPoint vHu = tvHunch.GetXyDir();
	CDPoint vHuNext = tvHuNext.GetXyDir();
	CDPoint vSlab = tvSlab.GetXyDir();

	CDPoint xySetOut(0, 0), xySetIn(0, 0);
	if(bSide)
	{
		CTwinVector tvSlabIn, tvWallSide;
		tvArrGen.GetTvByInfo(_T("슬래브끝단내측"), tvSlabIn);
		tvWallSide.m_v1 = (bLeft)? tvSlabNoOffset.m_v1 : tvSlabNoOffset.m_v2;
		tvWallSide.m_v2 = (bLeft)? tvSlabIn.m_v1 : tvSlabIn.m_v2;
		CDPoint vWallSide = tvWallSide.GetXyDir();
		
		GetXyMatchLineAndLine(tvOut.m_v1, vOut, tvWallSide.m_v1, vWallSide, tvOut.m_v1);
		GetXyMatchLineAndLine(tvHuNext.m_v2, vHuNext, tvWallSide.m_v1, vWallSide, tvHuNext.m_v2);
		GetXyMatchLineAndLine(tvOut.m_v1, vOut, tvEnd.m_v1, vEnd, tvEnd.m_v1);
		if(dLT==0)
			GetXyMatchLineAndLine(tvHuNext.m_v2, vHuNext, tvEnd.m_v1, vEnd, tvEnd.m_v2);
		else
			GetXyMatchLineAndLine(tvHuPrev.m_v2, vHuPrev, tvEnd.m_v1, vEnd, tvEnd.m_v2);

		xySetOut = tvOut.m_v1 - dSettleOut*vSlab*lr;
		xySetIn = tvHuNext.m_v2 - dSettleIn*vSlab*lr;
	}
	else
	{
		GetXyMatchLineAndLine(tvOut.m_v1, vOut, tvSlab.m_v1, vSlab, tvOut.m_v1);
		GetXyMatchLineAndLine(tvHuNext.m_v2, vHuNext, tvSlab.m_v1, vSlab, tvHuNext.m_v2);
		GetXyMatchLineAndLine(tvOut.m_v1, vOut, tvEnd.m_v1, vEnd, tvEnd.m_v1);
		if(dLT==0)
			GetXyMatchLineAndLine(tvHuNext.m_v2, vHuNext, tvEnd.m_v1, vEnd, tvEnd.m_v2);
		else
			GetXyMatchLineAndLine(tvHuPrev.m_v2, vHuPrev, tvEnd.m_v1, vEnd, tvEnd.m_v2);

		xySetOut = tvOut.m_v1 - dSettleOut*vSlab*lr;
		xySetIn = tvHuNext.m_v2 - dSettleIn*vSlab*lr;
	}

	if(!bUpper)
		xyHu[0] = xyHu[1] = xyHu[2] = tvEnd.m_v2;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	//A, D구간 최대길이 가져오기
	CTwinVectorArray tvArr;
	double dLengthA[2] = {0, 0}, dLengthD[2] = {0, 0}, dLengthB[2] = {0, 0}; //배면, 전면
	for(long nDan = 0; nDan < 2; nDan++)
	{
		pBri->GetTvArrayRebarWingWallBlockAMainFront(tvArr, bStt, bLeft, nDan, 1);
		dLengthA[nDan] = tvArr.GetLengthMax();
		pBri->GetTvArrayRebarWingWallBlockDMainFront(tvArr, bStt, bLeft, nDan, 1);
		dLengthD[nDan] = tvArr.GetLengthMax();
		pBri->GetTvArrayRebarWingWallBlockBMainFront(tvArr, bStt, bLeft, nDan, 1);
		dLengthB[nDan] = tvArr.GetLengthMin();
	}

	CTwinVector tv2Dan;
	double nMaxDan = pBri->GetMaxWingWallDan(bStt, bLeft, TRUE);
	// 전면 수평 철근
	Dom.LineTo(tvEnd.m_v1, tvEnd.m_v2);
	Dom.LineTo(tvOut.m_v1, tvEnd.m_v1);
	Dom.LineTo(tvOut.m_v1, xySetOut);
	if(nMaxDan == 2)
	{
		//1단과 2단철근 간격만큼 offset
		tv2Dan.m_v1 = tvOut.m_v1;
		tv2Dan.m_v2 = tvEnd.m_v1;
		tv2Dan.m_v1.y += 100*se*lr;
		tv2Dan.m_v2.y += 100*se*lr;
		
		//(상부평면도)
		if(bUpper)
		{
			//D구역이 2단인지
			if(pWing->m_nCountLayerD[1] == 2)
			{
				Dom.LineTo(tv2Dan.m_v2, tv2Dan.m_v2 - vX * dLengthD[1] * se);
			}
			//A구역이 2단인지
			if(pWing->m_nCountLayerA[1] == 2)
			{
				Dom.LineTo(tv2Dan.m_v1, tv2Dan.m_v1 + vX * dLengthA[1] * se);
			}
		}
		else//(하부평면도)
		{
			//B구역이 2단인지
			if(pWing->m_nCountLayerB[1] == 2)
			{
				Dom.LineTo(tv2Dan.m_v1, tv2Dan.m_v1 + vX * dLengthB[1] * se);
			}
		}
		
	}

	nMaxDan = pBri->GetMaxWingWallDan(bStt, bLeft, FALSE);
	// 배면 수평 철근
	if(dT1<dT2)
	{
		Dom.LineTo(tvEnd.m_v2, xyHu[1]+dSettleVar*vHuPrev);
		Dom.LineTo(xyHu[2], xyHu[0]);
		Dom.LineTo(xyHu[0], tvHuNext.m_v2);

		if(nMaxDan == 2)
		{
			if(pWing->m_nCountLayerA[0] == 2 || (pWing->m_dL2 == 0 && pWing->m_nCountLayerD[0] == 2 && bUpper))
			{
				tv2Dan.m_v1 = tvEnd.m_v2;
				tv2Dan.m_v2 = xyHu[1]+dSettleVar*vHuPrev;
				CDPoint xyDir = tv2Dan.GetXyDir();
				tv2Dan.m_v1 += 100*lr * xyDir.Rotate90();
				tv2Dan.m_v2 += 100*lr * xyDir.Rotate90();

				Dom.LineTo(tv2Dan.m_v1, tv2Dan.m_v2);
			}
			//D구역이 2단인지
			if(pWing->m_nCountLayerD[0] == 2 && bUpper)
			{
				CTwinVector tv2Dan2;
				tv2Dan.m_v1 = xyHu[2];
				tv2Dan.m_v2 = xyHu[0];
				CDPoint xyDir = tv2Dan.GetXyDir();
				tv2Dan.m_v1 += 100*lr * xyDir.Rotate90();
				tv2Dan.m_v2 += 100*lr * xyDir.Rotate90();

				tv2Dan2.m_v1 = xyHu[0];
				tv2Dan2.m_v2 = tvHuNext.m_v2;
				xyDir = tv2Dan2.GetXyDir();
				tv2Dan2.m_v1 += 100*lr * xyDir.Rotate90();
				tv2Dan2.m_v2 += 100*lr * xyDir.Rotate90();

				GetXyMatchLineAndLine(tv2Dan.m_v1, tv2Dan.GetXyDir(), tv2Dan2.m_v2, xyDir, tv2Dan.m_v2);
				tv2Dan2.m_v1 = tv2Dan.m_v2;
				GetXyMatchLineAndLine(tv2Dan.m_v2, tv2Dan.GetXyDir(), xyHu[2], xyDir, tv2Dan.m_v1);
				
				Dom.LineTo(tv2Dan.m_v1, tv2Dan.m_v2);
				Dom.LineTo(tv2Dan2.m_v1, tv2Dan2.m_v2);
			}
		}
	}
	else if(dT1>dT2)
	{
		Dom.LineTo(tvEnd.m_v2, xyHu[0]);
		Dom.LineTo(xyHu[0], xyHu[2]);
		Dom.LineTo(xyHu[1]-dSettleVar*vHuNext, tvHuNext.m_v2);
	}
	else
	{
		Dom.LineTo(tvEnd.m_v2, tvHuNext.m_v2);
		if(nMaxDan == 2)
		{
			tv2Dan.m_v1 = tvEnd.m_v2;
			tv2Dan.m_v2 = tvHuNext.m_v2;

			CDPoint xyDir = tv2Dan.GetXyDir();
			// (ARCBRIDGE-1492) 날개벽의 좌우 위치는 시점과 종점이 방향에 따라 각각 적용되므로 se가 적용될 필요가 없겠다.
			// tv2Dan.m_v1 -= 100*se*lr * xyDir.Rotate90();
			// tv2Dan.m_v2 -= 100*se*lr * xyDir.Rotate90();
			tv2Dan.m_v1 += 100*lr * xyDir.Rotate90();
			tv2Dan.m_v2 += 100*lr * xyDir.Rotate90();

			if(bUpper)
			{
				//D구역이 2단인지
				if(pWing->m_nCountLayerD[0] == 2 && bUpper)
				{
					Dom.LineTo(tv2Dan.m_v1, tv2Dan.m_v1 + xyDir * dLengthD[0]);
				}
				//A구역이 2단인지
				if(pWing->m_nCountLayerA[0] == 2)
				{
					Dom.LineTo(tv2Dan.m_v2, tv2Dan.m_v2 - xyDir * dLengthA[0]);
				}
			}
			else
			{
				//B구역이 2단인지
				if(pWing->m_nCountLayerB[0] == 2)
				{
					GetXyMatchLineAndLine(tv2Dan.m_v1, tv2Dan.GetXyDir(), tvSlab.m_v1, vSlab, tv2Dan.m_v2);
					GetXyMatchLineAndLine(tv2Dan.m_v2, tv2Dan.GetXyDir(), tvEnd.m_v1, vEnd, tv2Dan.m_v1);
					Dom.LineTo(tv2Dan.m_v1, tv2Dan.m_v2);
					Dom.LineTo(tv2Dan.m_v2, tv2Dan.m_v2 - dSettleIn * vSlab *lr);
				}
			}
		}
	}
	Dom.LineTo(tvHuNext.m_v2, xySetIn);

	BOOL bEnable2Dan = bUpper ? (pWing->m_nCountLayerA[1] == 2 ||pWing->m_nCountLayerD[1] == 2)
							 : (pWing->m_nCountLayerB[1] == 2 ||pWing->m_nCountLayerC[1] == 2);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_BARX);
	// 전면 수직 철근
	CDPoint xy(0, 0), xyR(0, 0);
	CDPointArray xyArr;
	CDPoint vBet1 = vEnd.BetweenAngle(vOut);
	CDPoint vBet2 = vOut.BetweenAngle(vEnd);
	xySttOut -= (dCoverWingEnd)/vBet2.y*vOut*lr;
	xySttIn -= (dCoverWingEnd)/vBet2.y*vOut*lr;

	long i = 0, nSize = 0;
	nMaxDan = pBri->GetMaxWingWallDan(bStt, bLeft, 1);
	for(long nDan = 0; nDan < nMaxDan; nDan++)
	{
		if(bUpper)
			pBri->GetDimArrayRebarWingWallUpper(xyArr, bStt, bLeft, 1, nDan);
		else
			xyArr = pWing->m_xyArrLower[1][nDan];
		
		xy = xySttOut - (dCoverWingOut+REBAR_RADIUS)/vBet1.y*vEnd*lr;
		if(nDan == 1) xy.y += 100*se*lr;
		CDPoint xyFirst = xy - REBAR_RADIUS/vBet2.y*vOut*lr; 

		//2단일때는 상면이 2단이여아함.
		if(nDan == 0 || (nDan == 1 && bEnable2Dan))
			CHgBaseDrawStd::InsertHCadBlock(&Dom, HCAD_E05, xyFirst, Dom.GetScaleDim());
		
		nSize = xyArr.GetSize();
		for(i=0; i<nSize; i++)
		{
			xyR = xyArr.GetAt(i);
			for(int j=0; j<xyR.x; j++)
			{
				xy -= xyR.y*vOut;
				CHgBaseDrawStd::InsertHCadBlock(&Dom, HCAD_E05, xy, Dom.GetScaleDim());
			}
		}
	}

	// 배면 수직 철근
	CTwinVector tvBound1, tvBound2;
	tvBound1.m_v1 = (dT1<=dT2 ? xyHu[0] : xyHu[1]) - vEnd*lr;
	tvBound1.m_v2 = (dT1<=dT2 ? xyHu[0] : xyHu[1]);
	tvBound2.m_v1 = (dT1<=dT2 ? xyHu[1] : xyHu[0]) - vEnd*lr;
	tvBound2.m_v2 = (dT1<=dT2 ? xyHu[1] : xyHu[0]);
	// 소수점 이하 자리수 때문에 철근위치에 오차가 있음.  - 없는게 맞는거 같은데 왜 있는거지?
//	tvBound1.m_v1.x = Round(tvBound1.m_v1.x, 0);
//	tvBound1.m_v2.x = Round(tvBound1.m_v2.x, 0);
//	tvBound2.m_v1.x = Round(tvBound2.m_v1.x, 0);
//	tvBound2.m_v2.x = Round(tvBound2.m_v2.x, 0);

	vBet1 = vEnd.BetweenAngle(vHuNext);
	vBet2 = vHuNext.BetweenAngle(vEnd);
	CDPoint vOutVert = vOut.Rotate90();
	CDPoint vBetHu = vEnd.BetweenAngle(vHu);
	CDPoint vBetHu2 = vOutVert.BetweenAngle(vHu);
	CDPoint xyTemp(0, 0);
	BOOL bInner1 = FALSE, bInner2 = FALSE;

	bEnable2Dan = bUpper ? (pWing->m_nCountLayerA[0] == 2 ||pWing->m_nCountLayerD[0] == 2)
						: (pWing->m_nCountLayerB[0] == 2 ||pWing->m_nCountLayerC[0] == 2);
	nMaxDan = pBri->GetMaxWingWallDan(bStt, bLeft, 0);
	for(long nDan = 0 ; nDan < nMaxDan; nDan++)
	{
		xy = xySttIn - (dCoverWingIn+REBAR_RADIUS)/vBet1.y*vEnd*lr;
		if(nDan == 1)
		{
			xy -= 100/vBet1.y * vEnd*lr;
		}
		CDPoint xyFirst = xy + REBAR_RADIUS/vBet2.y*vOut*lr;

		//2단일때는 상면이 2단이여아함.
		if(nDan == 0 || (nDan == 1 && bEnable2Dan))
			CHgBaseDrawStd::InsertHCadBlock(&Dom, HCAD_E05, xyFirst, Dom.GetScaleDim());

		if(bUpper)
			pBri->GetDimArrayRebarWingWallUpper(xyArr, bStt, bLeft, 0, nDan);
		else
		{
			xyArr = pWing->m_xyArrLower[0][nDan];
			//	끝단 사각처리이고 변단면일경우 T2로주면 실제 입력한 배면혹은 전면과 맞지 안는다.
			//	그래서(pWing->m_nDirHunch==1 && !pWing->m_bRAngEnd)일때
			//	가상의 배근 치수로 변환 해서 철근만 그려준다.
			if(pWing->m_nDirHunch==1 && !pWing->m_bRAngEnd) 
				pBri->SetXyArrConvertRebarVirtualLow(xyArr, bStt, bLeft);
		}
		nSize = xyArr.GetSize();

		bInner1 = FALSE, bInner2 = FALSE;
		for(i=0; i<nSize; i++)
		{
			xyR = xyArr.GetAt(i);
			xyR.y = Round(xyR.y, 0);

			for(int j=0; j<xyR.x; j++)
			{
				xy += xyR.y*vHuNext;
				bInner1 = GetDirPointByLine(xy, tvBound1.m_v1, tvBound1.m_v2);
				bInner2 = GetDirPointByLine(xy, tvBound2.m_v1, tvBound2.m_v2);

				double dCover2Dan = 100;
				if(!bInner1)
				{
					if(bInner2)
					{
						if(nDan == 1)
						{
							CDPoint xy11 = (xyHu[1] - xyHu[0]).Unit() - vOutVert.Unit();
							double dA1 = (xyHu[1] - xyHu[0]).Unit().GetAngleDegree();
							double dA2 = vOutVert.GetAngleDegree();
							double dAngDegree = xy11.Unit().GetAngleDegree();
							dCover2Dan = fabs(100 / xy11.x);
						}
						GetXyMatchSegAndLine(xyHu[0], xyHu[1], xy, vOutVert, xyTemp);
						if(bRangEnd)
						{
							xyTemp -= REBAR_RADIUS/vBetHu.y*vEnd*lr;
						}
						else
						{
							xyTemp -= REBAR_RADIUS/vBetHu2.y*vOutVert*lr;
						}

						BOOL bLower = !GetDirPointByLine(xyTemp, tvEnd.m_v2, xyHu[1]+dSettleVar*vHuPrev);
						if(bLeft? bLower : !bLower)
						{
							GetXyMatchSegAndLine(tvBound2.m_v2, tvEnd.m_v2, xy, vOutVert, xy);
							if(bRangEnd) xy -= REBAR_RADIUS/vBet1.y*vEnd*lr;
							else xy -= REBAR_RADIUS*vOutVert*lr;
						}
						else if(GetDirPointByLine(xyTemp, tvBound2.m_v1, tvBound2.m_v2))
						{
							xy = xyTemp;
						}
						else
						{
							GetXyMatchSegAndLine(tvBound1.m_v2, tvHuNext.m_v2, xy, vOutVert, xy);
							if(bRangEnd) xy -= REBAR_RADIUS/vBet1.y*vEnd*lr;
							else xy -= REBAR_RADIUS*vOutVert*lr;
						}
					}
					else
					{
						GetXyMatchSegAndLine(tvBound2.m_v2, tvEnd.m_v2, xy, vOutVert, xy);
						if(bRangEnd) xy -= REBAR_RADIUS/vBet1.y*vEnd*lr;
						else xy -= REBAR_RADIUS*vOutVert*lr;
					}
				}
				else
				{
					GetXyMatchSegAndLine(tvBound1.m_v2, tvHuNext.m_v2, xy, vOutVert, xy);

					if(bRangEnd) xy -= REBAR_RADIUS/vBet1.y*vEnd*lr;
					else xy -= REBAR_RADIUS*vOutVert*lr;
				}
				if(nDan == 1)
				{
					if(bRangEnd) 
						xy -= dCover2Dan/vBet1.y * vEnd*lr;
					else
						xy -= dCover2Dan/vBet1.y * vOutVert*lr;
				}

				CHgBaseDrawStd::InsertHCadBlock(&Dom, HCAD_E05, xy, Dom.GetScaleDim());	

			}
		}
	}
	
	// 전단 철근
	CDoubleArray dArrA, dArrD, dArr;
	if(bUpper)
	{
		pBri->GetLengthArrayShearFront(dArrA, 'A', bStt, bLeft);
		pBri->GetLengthArrayShearFront(dArrD, 'D', bStt, bLeft);

		if(dArrA.GetSize() == 0)
		{
			pBri->GetLengthArrayShearFront(dArrA, 'B', bStt, bLeft);
		}

		for(i=0; i<dArrA.GetSize(); i++)
			dArr.Add(dArrA.GetAt(i));
		for(i=0; i<dArrD.GetSize(); i++)
			dArr.Add(dArrD.GetAt(i));
	}
	else
		pBri->GetLengthArrayShearFront(dArr, 'B', bStt, bLeft);

	CDPoint vAng(0, 0);
	if(bSide)
	{
		vAng = ToDPointFrDegree(vOut.GetAngleDegree()-vEnd.GetAngleDegree());
	}
	else
	{
		vAng = pBri->GetAngleWingWallBetweenWall(bStt, bLeft);
	}
	
	CDPoint vDirShear = vOut.Rotate90();
	CDPoint xy1(0, 0), xy2(0, 0);

	double d1 = REBAR_RADIUS/tan(ConstPi-vAng.GetAngleRadian());
	int nCount = 0;
	nSize = dArr.GetSize();

	GetXyMatchLineAndLine(tvOut.m_v1, vOut, xyShearStt, vEnd, xyShearStt);
	if(!pWing->m_bRAngEnd) xyShearStt -= vOut*d1;

	for(i=0; i<nSize; i++)
	{
		xy1 = xyShearStt + dArr.GetAt(i)*vOut;

		BOOL bMatch = TRUE;
		GetXyMatchLineAndLine(tvHuNext.m_v2, vHuNext, xy1, vDirShear, xy2);
		bInner1 = GetDirPointByLine(xy1, tvBound1.m_v1, tvBound1.m_v2+dT1*vDirShear);
		bInner2 = GetDirPointByLine(xy1, tvBound2.m_v1, tvBound2.m_v2+dT1*vDirShear);
		if(!bInner1)
		{
			if(bInner2)
				GetXyMatchSegAndLine(xyHu[0], xyHu[1], xy1, vDirShear, xy2);
			else
				bMatch = GetXyMatchSegAndLine(tvHuPrev.m_v1, tvHuPrev.m_v2, xy1, vDirShear, xy2);
		}

		if(nCount%2==0)
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
		else
			m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_CENTER2_LINE);
		if(bMatch) Dom.LineTo(xy1, xy2);
		nCount++;
	}

	// 헌치 철근
	DrawRebarHunchWallSect(&Dom, bStt, bLeft, bUpper);

	// 흉벽보강 철근
	if(pWing->m_bAddRebarReinforce && !bSide)
	{
		CDPoint xyRf[3];
		pBri->GetXyRebarWingWallReinforce(xyRf, bStt, bLeft);
		
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);
		Dom.LineTo(xyRf[0], xyRf[1]);
		Dom.LineTo(xyRf[1], xyRf[2]);
	}

	CDPoint vAngWing = pBri->GetAngleAzimuthWingWall(bStt, bLeft);
	CDPoint vRotate = vAngWing.BetweenAngle(CDPoint(se, 0));
	Dom.Rotate(xyOrigin, vRotate);
	Dom.Move(-xyOrigin);

	*pDomP << Dom;
}

void CDrawWingWall::DimRebarPlaneLower(CDomyun* pDomP, BOOL bStt, BOOL bLeft, BOOL bUpper)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CARcBridgeDataStd* pStd = pBri->m_pARcBridgeDataStd;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	double se = bStt ? -1.0 : 1.0;
	double lr = bLeft ? -1.0 : 1.0;

	CTwinVectorArray tvArr;
	CTwinVector tv, tvSlab, tvOut, tvEnd;

	pBri->GetTvWingWallPlane(bStt, TRUE, bUpper, bUpper);
	tvArr = pBri->m_tvArrPlaneWingWall;
	BOOL bSide		= (pWing->m_nAttachPos == 0 )? FALSE : TRUE;

	tvArr.GetTvByInfo(_T("슬래브끝단외측"), tv);
	CDPoint vX(1, 0), vY(0, 1);
	CDPoint xyOrigin = bLeft ? tv.m_v1 : tv.m_v2;
	CDPoint vAngWing = pBri->GetAngleAzimuthWingWall(bStt, bLeft);
	CDPoint vRotate = vAngWing.BetweenAngle(vX*se);
	tvArr.Rotate(xyOrigin, vRotate);
	tvArr.Move(-xyOrigin);

	CDPoint vAng = pBri->GetAngleWingWallBetweenWall(bStt, bLeft);
	CDPoint xyOutStt(0, 0), xyInStt(0, 0), xyOutEnd(0, 0), xyInEnd(0, 0);
	
// 	double dSpareT = 0;
// 	if(bSide)
// 	{
// 		double dAngle	= vAng.GetAngleDegree();
// 		double dAngJijum = pBri->GetAngleJijum(bStt? 0 : pBri->m_nQtyJigan).GetAngleDegree();
// 		double dAngA1 = (dAngle - dAngJijum) * (bLeft? -1 : 1);
// 
// 		dSpareT = -(pWing->m_nDirHunch==2 ? dT2 : dT)/tan(ToRadian(dAngA1));
// 	}
// 	else dSpareT = (pWing->m_nDirHunch==2 ? dT2 : dT)/vAng.y*vAng.x;

	CString strDir = bLeft ? _T("좌측") : _T("우측");
	CString strDan = _T("");
	tvArr.GetTvByInfo(_T("슬래브끝단외측"), tv);
	tvArr.GetTvByInfo(strDir+_T("날개벽외측"), tvOut);
	tvArr.GetTvByInfo(strDir+_T("날개벽끝단"), tvEnd);
	xyOutStt = tvEnd.m_v1;
	xyInStt = tvEnd.m_v2;
	xyOutEnd = tvOut.m_v1;
//	xyInEnd = xyOutEnd + dSpareT*vX; // + dT*vY*lr*se;
	if(bSide) 
	{
		CTwinVector tvTemp;
		tvArr.GetTvByInfo(_T("슬래브끝단내측"), tvTemp);
		GetXyMatchSegAndLine(bLeft? tv.m_v1:tv.m_v2, bLeft? tvTemp.m_v1:tvTemp.m_v2, xyInStt, vX, xyInEnd);
	}
	else GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, xyInStt, vX, xyInEnd);

	double dLenIn = fabs(xyInStt.x-xyInEnd.x);
	double dLenOut = fabs(xyOutStt.x-xyOutEnd.x);

	CDomyun Dom(pDomP);

	CString strDirOut = se*lr>0 ? _T("BOTTOM") : _T("TOP");
	CString strDirIn = se*lr>0 ? _T("TOP") : _T("BOTTOM");

	CDPoint xyR(0, 0);
	double dCoverWingEnd = pBri->GetCoverWingWallEnd(bStt, bLeft);
	CString str1 = _T(""), str2 = _T("");
	long nSize = 0, i = 0;
	double dTotalLen = 0;
	// 배면
	Dom.SetDirection(strDirIn);
	int nDimDan = (pWing->m_bAddRebarReinforce && !bSide)? 3 : 2;
	CDPointArray xyArr;

	BOOL bEnable2Dan =  bUpper ? (pWing->m_nCountLayerA[0] == 2 || pWing->m_nCountLayerD[0] == 2)
								:(pWing->m_nCountLayerB[0] == 2 || pWing->m_nCountLayerC[0] == 2);
	long nMaxDan = pBri->GetMaxWingWallDan(bStt, bLeft, 0);
	for(long nDan = 0; nDan < nMaxDan; nDan++)
	{
		if(bEnable2Dan)
			strDan = (nDan == 0) ? _T("  1단") : _T("  2단");
		if(nDan == 1 && !bEnable2Dan) continue;
	
		if(bUpper)
			pBri->GetDimArrayRebarWingWallUpper(xyArr, bStt, bLeft, 0, nDan);
		else
		{
			xyArr = pWing->m_xyArrLower[0][nDan];
			//	끝단 사각처리이고 변단면일경우 T2로주면 실제 입력한 배면혹은 전면과 맞지 안는다.
			//	그래서(pWing->m_nDirHunch==1 && !pWing->m_bRAngEnd)일때
			//	가상의 배근 치수로 변환 해서 철근만 그려준다.
			if(pWing->m_nDirHunch==1 && !pWing->m_bRAngEnd) pBri->SetXyArrConvertRebarVirtualLow(xyArr, bStt, bLeft);
		}
		nSize = xyArr.GetSize();
		Dom.DimMoveTo(xyInStt.x, xyInStt.y, nDimDan, COMMA(dCoverWingEnd));
		Dom.DimLineTo(dCoverWingEnd, nDimDan, _T(""));
		dTotalLen = dCoverWingEnd;
		for(i=0; i<nSize; i++)
		{
			xyR = xyArr.GetAt(i);
			str1 = xyR.x==1 ? COMMA(xyR.y) : COMMA(xyR.x) + _T("@") + COMMA(xyR.y) + _T("=") + COMMA(xyR.x*xyR.y);
			Dom.DimLineTo(xyR.x*xyR.y, nDimDan, str1);
			dTotalLen += xyR.x*xyR.y;
		}
		if(dLenIn > dTotalLen)
		{
			Dom.DimLineTo(dLenIn-dTotalLen, nDimDan, COMMA(dLenIn-dTotalLen));
			dTotalLen = dLenIn;
		}
		if(bEnable2Dan)
			Dom.DimLineToOver(strDan.GetLength(), nDimDan, strDan);
		nDimDan++;
	}
	
	// 전체 치수
	Dom.DimMoveTo(xyInStt.x, xyInStt.y, nDimDan);
	Dom.DimLineTo(dTotalLen, nDimDan, COMMA(dTotalLen));
	if(!bStt)
		Dom.Mirror(xyInStt.x, TRUE);
	
	*pDomP << Dom;

	// 전면
	Dom.SetDirection(strDirOut);

	bEnable2Dan = bUpper ? (pWing->m_nCountLayerA[1] == 2 || pWing->m_nCountLayerD[1] == 2)
						: (pWing->m_nCountLayerB[1] == 2 || pWing->m_nCountLayerC[1] == 2);
	nMaxDan = pBri->GetMaxWingWallDan(bStt, bLeft, 1);
	for(long nDan = 0; nDan < nMaxDan; nDan++)
	{
		if(bEnable2Dan)
			strDan = (nDan == 0) ? _T("  1단") : _T("  2단");
		if(nDan == 1 && !bEnable2Dan) continue;

		nDimDan = nDan;
		if(bUpper)
			pBri->GetDimArrayRebarWingWallUpper(xyArr, bStt, bLeft, 1, nDan);
		else
			xyArr = pWing->m_xyArrLower[1][nDan];
		nSize = xyArr.GetSize();
		Dom.DimMoveTo(xyOutStt.x, xyOutStt.y, nDimDan, COMMA(dCoverWingEnd));
		Dom.DimLineTo(dCoverWingEnd, nDimDan, _T(""));
		dTotalLen = dCoverWingEnd;
		for(i=0; i<nSize; i++)
		{
			xyR = xyArr.GetAt(i);
			str1 = xyR.x==1 ? COMMA(xyR.y) : COMMA(xyR.x) + _T("@") + COMMA(xyR.y) + _T("=") + COMMA(xyR.x*xyR.y);
			Dom.DimLineTo(xyR.x*xyR.y, nDimDan, str1);
			dTotalLen += xyR.x*xyR.y;
		}
		if(dLenOut  > dTotalLen)
		{
			Dom.DimLineTo(dLenOut-dTotalLen, nDimDan, COMMA(dLenOut-dTotalLen));
			dTotalLen = dLenOut;
		}
		if(bEnable2Dan)
			Dom.DimLineToOver(strDan.GetLength(), nDimDan, strDan);
	}
	
	// 전체 치수
	nDimDan++;
	Dom.DimMoveTo(xyOutStt.x, xyOutStt.y, nDimDan);
	Dom.DimLineTo(dTotalLen, nDimDan, COMMA(dTotalLen));

	if(!bStt)
		Dom.Mirror(xyOutStt.x, TRUE);

	*pDomP << Dom;
}

void CDrawWingWall::DimRebarPlaneEnd(CDomyun* pDomP, BOOL bStt, BOOL bLeft, BOOL bUpper)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	double se = bStt ? -1.0 : 1.0;
	double lr = bLeft ? -1.0 : 1.0;
	double dT1 = pWing->m_dT1;
	double dT2 = pWing->m_dT2;
	double dT = bUpper ? dT1 : dT2;
	double dCoverIn = pBri->m_dCoverWingIn[0];
	double dCoverOut = pBri->m_dCoverWingOut[0];
	
	CTwinVector tv, tvEnd;
	CTwinVectorArray tvArr;
	pBri->GetTvWingWallPlane(bStt, TRUE, bUpper, bUpper);
	tvArr = pBri->m_tvArrPlaneWingWall;

	tvArr.GetTvByInfo(_T("슬래브끝단외측"), tv);
	CDPoint xyOrigin = bLeft ? tv.m_v1 : tv.m_v2;
	CDPoint vAngWing = pBri->GetAngleAzimuthWingWall(bStt, bLeft);
	CDPoint vRotate = vAngWing.BetweenAngle(CDPoint(1, 0)*se);
	tvArr.Rotate(xyOrigin, vRotate);
	tvArr.Move(-xyOrigin);
	
	CString strDir = bLeft ? _T("좌측") : _T("우측");
	tvArr.GetTvByInfo(strDir+_T("날개벽끝단"), tvEnd);
	CDPoint vX(1, 0), vY(0, 1);

	CDomyun Dom(pDomP);
	Dom.SetDirection(bStt ? _T("LEFT") : _T("RIGHT"));

	CDPoint xy = se*lr>0 ? tvEnd.m_v1 : tvEnd.m_v2;
	double dCover1 = se*lr>0 ? dCoverOut : dCoverIn;
	double dCover2 = se*lr>0 ? dCoverIn : dCoverOut;
	Dom.DimMoveTo(xy.x, xy.y, 0, COMMA(dCover1));
	Dom.DimLineTo(dCover1, 0, _T(""));
	Dom.DimLineTo(dT-dCoverIn-dCoverOut, 0, COMMA(dT-dCoverIn-dCoverOut));
	Dom.DimLineToOver(dCover2, 0, COMMA(dCover2));

	// 전체 치수
	Dom.DimMoveTo(xy.x, xy.y, 1);
	Dom.DimLineTo(dT, 1, COMMA(dT));

	*pDomP << Dom;
}

void CDrawWingWall::DimGenSodan(CDomyun* pDomP, BOOL bStt, BOOL bLeft)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	CDomyun Dom(pDomP);
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);

	CDPoint cpEnd = pBri->GetXySectionWingWallStt(bStt, FALSE);

	CTwinVectorArray pTvArr;
	CTwinVector tv;

	long nDimDan = 0;
	int iDir = (bStt)? 1:-1;
	double dx, dy, dl = 0;
	CString str;
		
	pTvArr.RemoveAll();
	pBri->GetTvWingSlopeLine(pTvArr, bStt, bLeft, frM(pWing->m_dElSodanBottom));

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CString strDir = _T("TOP");
	Dom.SetDirection(strDir);
	pTvArr.GetTvByInfo("1단", tv);

	*pDomP << Dom;
	
	str = _T("1단(1:n)");
	dx = tv.m_v1.x + (tv.GetHorLength()/2+Dom.Always(1))*iDir;
	dy = tv.m_v1.y - tv.GetVerLength()/2 +Dom.Always(1);
	
	Dom.SetTextAlignHorz(TA_CENTER);
	Dom.TextOut(dx, dy, str);
	Dom.Rotate(CDPoint(dx, dy), CDPoint(1,-1/pWing->m_dSlopeNoriFirst*iDir));
	*pDomP << Dom;

	// HorLength
	dx = tv.m_v1.x - pWing->m_dDistNoriStart*iDir;
	dy = tv.m_v1.y + Dom.Always(3);
	Dom.DimMoveTo(dx, dy, nDimDan, "", FALSE);
	Dom.DimLineToExtend(pWing->m_dDistNoriStart*iDir, nDimDan, "sD", COMMA(pWing->m_dDistNoriStart),TRUE);
	dl += pWing->m_dDistNoriStart;
	int nCnt = pTvArr.GetSize();
	int i=0; for(i=0; i<nCnt; i++)
	{
		tv = pTvArr.GetPAt(i);

		if(tv.GetHorLength()>0) Dom.DimLineToExtend(tv.GetHorLength()*iDir,nDimDan,COMMA(tv.GetHorLength()),"",TRUE,FALSE);
		dl += tv.GetHorLength();
	}
	Dom.DimMoveTo(dx, dy, 1, "", FALSE);
	Dom.DimLineToExtend(dl*iDir, 1, COMMA(dl), "", TRUE);

	dl = 0;
	strDir = (bStt)? _T("LEFT"):_T("RIGHT");
	Dom.SetDirection(strDir);
	for(i=0; i<nCnt; i++)
	{
		tv = pTvArr.GetPAt(i);
		if(i == 0)
		{
			dx = tv.m_v1.x -Dom.Always(10)*iDir;
			dy = tv.m_v1.y;
			Dom.DimMoveTo(dx, dy, nDimDan, "", FALSE);
		}

		if(tv.GetVerLength()>0) Dom.DimLineToExtend(-tv.GetVerLength(),nDimDan,COMMA(tv.GetVerLength()),"",TRUE,FALSE);
		dl += tv.GetVerLength();
	}
	if(pWing->m_bRetainWallFront)
	{
		CDPoint cpEnd = pBri->GetXySectionWingWallStt(bStt, FALSE);
		double dh = tv.m_v2.y - cpEnd.y;

		Dom.DimLineToExtend(-dh,nDimDan, "옹벽설치 높이", COMMA(dh), TRUE, FALSE);
		dl += dh;
	}
	Dom.DimMoveTo(dx, dy, 1, "", FALSE);
	Dom.DimLineToExtend(-dl, 1, COMMA(dl), "", TRUE, FALSE);
	
	strDir = _T("TOP");
	Dom.SetDirection(strDir);
	if(pTvArr.GetTvByInfo("벽체전면소단", tv))
	{
		dx = tv.m_v1.x;
		dy = tv.m_v1.y;

		Dom.DimMoveTo(dx, dy, nDimDan, "", FALSE);
		Dom.DimLineToExtend(tv.GetHorLength()*iDir, nDimDan, "벽체전면소단폭", COMMA(pWing->m_dWidthSodanWallFront),TRUE);

		str.Format("%0.1f%%", pWing->m_dSlopeSodan);
		dx = tv.m_v1.x + tv.GetHorLength()/10 *iDir;
		dy = tv.m_v1.y+ Dom.Always(2);
		CDPoint cptemp = CDPoint(iDir, 0).Unit();
		Sym.ArrowDirectionForSlope(&Dom, CDPoint(dx, dy), str, cptemp);
	}
	
	if(pTvArr.GetTvByInfo("2단이상", tv))
	{
		*pDomP << Dom;
		
		str = _T("2단이상(1:n)");
		dx = tv.m_v1.x + (tv.GetHorLength()/2+Dom.Always(1))*iDir;
		dy = tv.m_v1.y - tv.GetVerLength()/2 +Dom.Always(1);
		
		Dom.SetTextAlignHorz(TA_CENTER);
		Dom.TextOut(dx, dy, str);
		Dom.Rotate(CDPoint(dx, dy), CDPoint(1,-1/pWing->m_dSlopeNoriNext*iDir));
		*pDomP << Dom;
	}

	if(pTvArr.GetTvByInfo("소단", tv))
	{
		dx = tv.m_v1.x;
		dy = tv.m_v1.y;

		Dom.DimMoveTo(dx, dy, nDimDan, "", FALSE);
		Dom.DimLineToExtend(tv.GetHorLength()*iDir, nDimDan, "소단폭", COMMA(pWing->m_dWidthSodan),TRUE);

		str.Format("%0.1f%%", pWing->m_dSlopeSodan);
		dx = tv.m_v1.x;
		dy = tv.m_v1.y+ Dom.Always(2);
		CDPoint cptemp = CDPoint(iDir, 0).Unit();
		Sym.ArrowDirectionForSlope(&Dom, CDPoint(dx, dy), str, cptemp);
	}
	
	//////////////////////////////////////////////////////////////////////////
	double dL1 = pWing->m_dL1;
	double dL2 = pWing->m_dL2;
	double se = bStt ? -1.0 : 1.0;
	double dElEnd = pWing->GetEndEl(FALSE);
	double dFH = bStt ? pBri->m_footingStt.m_dFH : pBri->m_footingEnd.m_dFH;
	double dEH = bStt ? pBri->m_footingStt.m_exFooting.GetMaxHeight() : pBri->m_footingEnd.m_exFooting.GetMaxHeight();

	CDPoint xyStt = pBri->GetXySectionWingWallStt(bStt, TRUE);
	CDPoint xyEnd = pBri->GetXySectionWingWallStt(bStt, FALSE);
	CDPoint xyBot = CDPoint(xyStt.x + (dL1+dL2)*se, xyEnd.y - dFH - dEH);

	CString strEl;
	strEl.Format(_T("EL. %.3f"), toM(xyStt.y));
	Sym.ArrowElevation(&Dom, strEl, _T(""), xyStt, !bStt);

	strEl.Format(_T("EL. %.3f"), toM(dElEnd));
	Sym.ArrowElevation(&Dom, strEl, _T(""), CDPoint(xyBot.x, dElEnd), bStt);
	//////////////////////////////////////////////////////////////////////////
	
	*pDomP << Dom;
}

void CDrawWingWall::DrawGenSodan(CDomyun* pDomP, BOOL bStt, BOOL bLeft)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CConcSymbol SymDom(m_pStd->m_pARoadOptionStd);

	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	double dEl = frM(pWing->m_dElSodanBottom);
	CDomyun Dom(pDomP);

	// 노리
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_MIS2);
	CTwinVectorArray tvArr;
	pBri->GetTvWingSlopeLine(tvArr, bStt, bLeft, dEl);
	m_pStd->DrawTvArray(&Dom, tvArr, FALSE, FALSE);

	// 옹벽설치 체크시 옹벽표시
	if(pWing->m_bRetainWallFront)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
		CDRect crWall;
		CTwinVector tv;
		tvArr.GetLast(tv);
		CDPoint cpEnd = pBri->GetXySectionWingWallStt(bStt, FALSE);
		
		crWall.right = (bStt)? (tv.m_v2.x-200):(tv.m_v2.x);
		crWall.left = (bStt)? (tv.m_v2.x):(tv.m_v2.x+200);
		crWall.top = tv.m_v2.y;
		crWall.bottom = cpEnd.y;

		Dom.Rectangle(crWall);
		Dom.HatchRect(crWall, "", 0);
	}

	*pDomP << Dom;
	// 지반선
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_MIS1);
	CDRect rect = pDomP->GetExtRect();
	m_pStd->m_pDrawVertSection->DrawElByStation(&Dom, rect.left - Dom.Always(10) , rect.right + Dom.Always(10));

	CString strEl = _T("");
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	strEl.Format("EL. %0.3f", toM(dEl));
	SymDom.ArrowElevation(&Dom, "EL", strEl, CDPoint(rect.left+Dom.Always(3), dEl),FALSE);


	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_MIS3);
	Dom.MoveTo(rect.left - Dom.Always(10), dEl);
	Dom.LineTo(rect.right + Dom.Always(10), dEl);

	*pDomP << Dom;
}

// 날개벽 치수(수량 출력용)
// void CDrawWingWall::DimGenFrontForBM(CDomyun *pDomP, BOOL bStt, BOOL bLeft)
// {
// 	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
// 	if(!pBri) return;
// 	if(!pBri->IsWingWall(bStt, bLeft)) return;
// 	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);
// 	if(!pWing) return;
// 	CTypedPtrArray <CObArray, CTwinVectorAreaShape*> *pArrAreaConc	= &pWing->m_pArrAreaConc;
// 
// 	CDomyun Dom(pDomP);
// 	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
// 	CHgBaseDrawStd baseDraw;
// 
// 	CDoubleArray dArrDim;
// 	CStringArray sArrText;
// 	CStringArray sArrUnderText;
// 	CString sDir	= _T("TOP");
// 	CDPoint xyStt	= CDPoint(0, 0);
// 	
// 	// 상, 좌
// 	CDoubleArray dArrX;
// 	CDoubleArray dArrY;
// 	long i = 0; for(i = 0; i < pArrAreaConc->GetSize(); i++)
// 	{
// 		CTwinVectorAreaShape *pArea	= pArrAreaConc->GetAt(i);
// 		CTwinVector tvLeft	= pArea->GetTvLeft();
// 		CTwinVector tvRight	= pArea->GetTvRight();
// 
// 		dArrX.Add(tvLeft.m_v1.x);
// 		dArrX.Add(tvLeft.m_v2.x);
// 		dArrX.Add(tvRight.m_v1.x);
// 		dArrX.Add(tvRight.m_v2.x);
// 
// 		dArrY.Add(tvLeft.m_v1.y);
// 		dArrY.Add(tvLeft.m_v2.y);
// 		dArrY.Add(tvRight.m_v1.y);
// 		dArrY.Add(tvRight.m_v2.y);
// 	}
// 
// 	dArrX.Sort();
// 	dArrY.Sort();
// 	xyStt.x	= dArrX.GetAt(0);
// 	xyStt.y	= dArrY.GetAt(dArrY.GetSize()-1);
// 
// 	// 상 치수 기입 
// 	sDir	= _T("TOP");
// 	for(i = 0; i < dArrX.GetSize()-1; i++)
// 		dArrDim.Add(dArrX.GetAt(i+1) - dArrX.GetAt(i));
// 
// 	baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, 0, sDir, -1, TRUE, FALSE, FALSE);
// 
// 	// 좌 치수 기입
// 	xyStt.y	= dArrY.GetAt(0);
// 	dArrDim.RemoveAll();
// 	sDir	= _T("LEFT");
// 	for(i = 0; i < dArrY.GetSize()-1; i++)
// 		dArrDim.Add(dArrY.GetAt(i+1) - dArrY.GetAt(i));
// 
// 	baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, 0, sDir, -1, TRUE, FALSE, FALSE);
// 
// 	*pDomP << Dom;
// }

void CDrawWingWall::MarkRebarBlockAFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri || !pBri->IsWingWall(bStt, bLeft)) return;

	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	CTwinVector tv;
	CTwinVectorArray tvArrMain, tvArrSupp, tvArrBlock;

	long nDan = 0;
	pBri->GetTvArrayRebarWingWallBlockAMainFront(tvArrMain, bStt, bLeft, nOut, nDan);
	pBri->GetTvArrayRebarWingWallBlockASupportFront(tvArrSupp, bStt, bLeft, nOut, nDan);
	int i=0; for(i=0; i<tvArrSupp.GetSize(); i++)
	{
		if(tvArrSupp.GetAt(i).m_sInfo.Left(1)==_T("*"))
		{
			tvArrSupp.RemoveAt(i);
			i--;
		}
	}
	int nSizeMain = tvArrMain.GetSize();
	int nSizeSupp = tvArrSupp.GetSize();

	double se = bStt ? -1.0 : 1.0;
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CDPoint vAngMark = bStt ? CDPoint(0, 1) : CDPoint(1, 0);
	CDPoint xy(0, 0), xyB(0, 0), xyE(0, 0);
	vector<double> y_slope, y_hor, y_var_sec, y_var_sec_block, x_ver, x_var;

	// (37340) 철근 길이에 따라 위치를 바꿔주면 뒤에서 마킹선 위치가 맞지 않는다.
	double dHorX_S = 0;
	double dHorX_1 = 0;
	double dHorX_2 = 0;
	double dHorX_3 = 0;
	// 주철근
	for(i=0; i<nSizeMain; i++)
	{
		tv = tvArrMain.GetAt(i);
		GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, CDPoint(tv.m_v1.x+MARK_DIST*se, 0), CDPoint(0, 1), xy);
		if(tv.m_sInfo==_T("상단경사A"))
		{
			dHorX_S = xy.x;
			y_slope.push_back(xy.y);
			Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
		}
		if(tv.m_sInfo==_T("수평A"))
		{
			dHorX_1 = xy.x;
			y_hor.push_back(xy.y);
			Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
		}
		if(tv.m_sInfo==_T("수평변단면A"))
		{
			dHorX_2 = xy.x;
			y_var_sec.push_back(xy.y);
			Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
		}
		if(tv.m_sInfo==_T("수평변단면구간A"))
		{
			GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, CDPoint(tv.m_v1.x+MARK_DIST*1.5*se, 0), CDPoint(0, 1), xy);
			dHorX_3 = xy.x;
			y_var_sec_block.push_back(xy.y);
			Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
			tvArrBlock.Add(tv);
		}
	}
	double dMarkEnd = dHorX_1-MARK_DIST*se;
	double dHalfCTC = nOut==0 && pWing->m_bSelMainCTCD ? pWing->m_dMainCTCA/4 : 0;

	if(nSizeMain>0)
	{
		//.. 상단경사A
		int nRBIndex = nOut==0 ? WW_REBAR_SLOPE_A_IN : WW_REBAR_SLOPE_A_OUT;
		CRebar* pRB = pWing->m_pArrRebar[nRBIndex];
		if(pRB->IsTrue() && !y_slope.empty())
		{
			Dom.LineTo(dHorX_S, y_slope.front(), dHorX_S, y_slope.back());
			xyE = CDPoint(dHorX_S, y_slope.back());
			// Sym.RebarMarkLineUpper(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), !bStt, 0, FALSE);

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = pRB->m_sMark;
			mark_input_data.dia = pRB->GetStringDia();
			mark_input_data.position = xyE;
			mark_input_data.ctc = nOut==0 ? pWing->m_dMainCTCA/2 : pWing->m_dMainCTCA;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.left_dir =  !bStt;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.mark_pos = eMarkUpper;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}

		//.. 수평A
		nRBIndex = nOut==0 ? WW_REBAR_HOR_A_IN : WW_REBAR_HOR_A_OUT;
		pRB = pWing->m_pArrRebar[nRBIndex];
		if(pRB->IsTrue() && !y_hor.empty())
		{
			Dom.LineTo(dHorX_1, y_hor.front(), dHorX_1, y_hor.back());
			double dMarkY = pBri->m_Util.GetPosRebarMark(tvArrMain, TRUE, y_hor.size()-2);
			xyB = CDPoint(dHorX_1, dMarkY-dHalfCTC);
			xyE = CDPoint(dMarkEnd, xyB.y);
			Dom.LineTo(xyB, xyE);
			// Sym.RebarMarkLineSide(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), !bStt, 0, FALSE);

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = pRB->m_sMark;
			mark_input_data.dia = pRB->GetStringDia();
			mark_input_data.position = xyE;
			mark_input_data.ctc = nOut==0 ? pWing->m_dMainCTCA/2 : pWing->m_dMainCTCA;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.left_dir =  !bStt;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.mark_pos = eMarkSide;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}

		//.. 수평변단면A
		if(nOut==0)
		{
			nRBIndex = WW_REBAR_HOR_A_IN_VAR_SEC;
			pRB = pWing->m_pArrRebar[nRBIndex];
			if(pRB->IsTrue() && !y_var_sec.empty())
			{
				Dom.LineTo(dHorX_2, y_var_sec.front(), dHorX_2, y_var_sec.back());
				double dMarkY = pBri->m_Util.GetPosRebarMark(tvArrMain, TRUE, y_var_sec.size()-2);
				xyB = CDPoint(dHorX_2, dMarkY);
				xyE = CDPoint(dMarkEnd, xyB.y);
				Dom.LineTo(xyB, xyE);
				// Sym.RebarMarkLineSide(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), !bStt, 0, FALSE);
				
				SRebarMarkInputData mark_input_data;
				SRebarMarkviewData mark_view_data;
				mark_input_data.mark = pRB->m_sMark;
				mark_input_data.dia = pRB->GetStringDia();
				mark_input_data.position = xyE;
				mark_input_data.ctc = nOut==0 ? pWing->m_dMainCTCA/2 : pWing->m_dMainCTCA;
				mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
				mark_input_data.left_dir =  !bStt;
				mark_input_data.height = 0;

				mark_view_data.count_mark = FALSE;
				mark_view_data.mark_pos = eMarkSide;

				Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
			}
		}

		//.. 수평변단면구간A
		nRBIndex = nOut==0 ? WW_REBAR_HOR_A_IN_VAR_SEC_BLOCK : WW_REBAR_HOR_A_OUT_VAR_SEC_BLOCK;
		pRB = pWing->m_pArrRebar[nRBIndex];
		if(pRB->IsTrue() && !y_var_sec_block.empty())
		{
			Dom.LineTo(dHorX_3, y_var_sec_block.front(), dHorX_3, y_var_sec_block.back());
			double dMarkY = pBri->m_Util.GetPosRebarMark(tvArrBlock, TRUE, y_var_sec_block.size()-1);
			xyB = CDPoint(dHorX_3, dMarkY);
			xyE = CDPoint(dMarkEnd, xyB.y);
			Dom.LineTo(xyB, xyE);
			// Sym.RebarMarkLineSide(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), !bStt, 0, FALSE);

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = pRB->m_sMark;
			mark_input_data.dia = pRB->GetStringDia();
			mark_input_data.position = xyE;
			mark_input_data.ctc = nOut==0 ? pWing->m_dMainCTCA/2 : pWing->m_dMainCTCA;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.left_dir =  !bStt;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.mark_pos = eMarkSide;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}
	}

	// 배력 철근
	if(nSizeSupp>0)
	{
		CTwinVectorArray tvArrGen;
		CTwinVector tvUpperTop, tvLowerBot;
		pBri->GetTvWingWallFront(bStt, bLeft, nOut, tvArrGen);
		tvArrGen.GetTvByInfo(_T("날개벽정면상단"), tvUpperTop);
		tvArrGen.GetTvByInfo(_T("날개벽정면끝단하부"), tvLowerBot);

		double dJoint(0);
		double dCoverWingIn = pBri->m_dCoverWingIn[0];
		double dDiaA = pWing->m_dDiaSupportA[0][nDan];
		double dDia = max(dDiaA, pWing->m_dDiaMainC[nOut][nDan]);
		double dCTC = nOut==0 ? pWing->m_dStdVerCTC/2 : pWing->m_dStdVerCTC;
		double dCoverWing = nOut==0 ? dCoverWingIn : pBri->m_dCoverWingOut[0];
		
		if(pBri->m_pARcBridgeDataStd->IsLsdDesign())
		{
			SRebarEnvOption LsdRebarOpt;
			pBri->SetRebarLsdOptionWingWall(&LsdRebarOpt, bStt, bLeft, nOut==0, WINGWALL_SECTA, FALSE);
			dJoint = pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->GetValueJoint(dDia, &LsdRebarOpt);
		}
		else
		{
			dJoint = pBri->m_pARcBridgeDataStd->GetValueJoint(dDia, JOINT_TENS_NORMAL_B, toCm(dCoverWing), toCm(dCTC));
		}

		double dHeightMin = min(tvUpperTop.m_v2.y-tvLowerBot.m_v2.y, tvUpperTop.m_v1.y-tvLowerBot.m_v2.y);
		double dHgtB = pBri->GetLengthWingWallBlockBC(bStt, bLeft);	// B구간의 높이

		BOOL bIsShort = (dHeightMin-dHgtB < dJoint/2)? TRUE : FALSE;

		CTwinVector tvUpper;
		CTwinVectorArray tvArrMark;
		BOOL bMerged = pBri->IsMergedRebarWingWall(bStt, bLeft, nOut, nDan, 'A', 'D');
		if(bMerged)
			pBri->GetTvArrayRebarWingWallBlockDMainFront(tvArrMark, bStt, bLeft, nOut, nDan);
		else
			tvArrMark = tvArrMain;
		
		int nSizeMark = tvArrMark.GetSize();
		if(nSizeMark > 0)
		{
			int nCountHor = 0, nCountHorVar = 0;

			int i=0; for(i=0; i<nSizeMark; i++)
			{
				tv = tvArrMark.GetAt(i);
				xy = CDPoint(tv.m_v1.x, 0);
				if(tv.m_sInfo==_T("수평A") || tv.m_sInfo==_T("수평D"))
					nCountHor++;
				if(tv.m_sInfo.Left(8)==_T("수평변화"))
					nCountHorVar++;
			}
			dMarkEnd = xy.x;
			tvUpper = tvArrMark.GetAt(nSizeMark-1);
			double dVerY = pBri->m_Util.GetPosRebarMark(tvArrMark, TRUE, nCountHorVar+nCountHor-1) + dHalfCTC;
			double dVarY = pBri->m_Util.GetPosRebarMark(tvArrMark, TRUE, bMerged ? nCountHorVar+1 : nCountHor/2) + dHalfCTC;

			if(bIsShort)
			{
				dVerY = tvArrSupp.GetXyBottom().y + (tvArrSupp.GetXyTop().y - tvArrSupp.GetXyBottom().y)/2;
				dVarY = dVerY;
			}

			for(i=0; i<nSizeSupp; i++)
			{
				tv = tvArrSupp.GetAt(i);
				if(tv.m_sInfo==_T("수직A"))
				{
					x_ver.push_back(tv.m_v2.x);
					xy = CDPoint(tv.m_v2.x, dVerY);
					Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
				}
				if(tv.m_sInfo==_T("변단면수직A"))
				{
					x_var.push_back(tv.m_v2.x);
					xy = CDPoint(tv.m_v2.x, dVarY);
					Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
				}
			}

			// 수직A
			int nRBIndex = nOut==0 ? WW_REBAR_VER_A_IN : WW_REBAR_VER_A_OUT;
			CRebar* pRB = pWing->m_pArrRebar[nRBIndex];
			if(pRB->IsTrue() && !x_ver.empty())
			{
				Dom.LineTo(x_ver.front(), dVerY, x_ver.back(), dVerY);
				double dMarkX = pBri->m_Util.GetPosRebarMark(tvArrSupp, FALSE, nSizeSupp/2);
				xyB = CDPoint(dMarkX, dVerY);
				GetXyMatchSegAndLine(tvUpper.m_v1, tvUpper.m_v2, xyB, CDPoint(0, 1), xyE);
				Dom.LineTo(xyB, xyE);
				// Sym.RebarMarkLineUpper(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), !bStt, 0, FALSE);

				SRebarMarkInputData mark_input_data;
				SRebarMarkviewData mark_view_data;
				mark_input_data.mark = pRB->m_sMark;
				mark_input_data.dia = pRB->GetStringDia();
				mark_input_data.position = xyE;
				mark_input_data.ctc = nOut==0 ? pWing->m_dStdVerCTC/2 : pWing->m_dStdVerCTC;
				mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
				mark_input_data.left_dir =  !bStt;
				mark_input_data.height = 0;

				mark_view_data.count_mark = FALSE;
				mark_view_data.mark_pos = eMarkUpper;

				Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
			}

			// 변단면수직A
			if(nOut==0)
			{
				nRBIndex = WW_REBAR_VER_A_IN_VAR_SEC;
				pRB = pWing->m_pArrRebar[nRBIndex];
				if(pRB->IsTrue() && !x_var.empty())
				{
					Dom.LineTo(x_var.front(), dVarY, x_var.back(), dVarY);
					xyB = CDPoint(x_var.front(), dVarY);
					xyE = CDPoint(dMarkEnd, dVarY);
					Dom.LineTo(xyB, xyE);
					// Sym.RebarMarkLineSide(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), !bStt, 0, FALSE);

					SRebarMarkInputData mark_input_data;
					SRebarMarkviewData mark_view_data;
					mark_input_data.mark = pRB->m_sMark;
					mark_input_data.dia = pRB->GetStringDia();
					mark_input_data.position = xyE;
					mark_input_data.ctc = nOut==0 ? pWing->m_dStdVerCTC/2 : pWing->m_dStdVerCTC;
					mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
					mark_input_data.left_dir =  !bStt;
					mark_input_data.height = 0;

					mark_view_data.count_mark = FALSE;
					mark_view_data.mark_pos = eMarkSide;

					Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
				}
			}
		}
	}

	*pDomP << Dom;
}

void CDrawWingWall::MarkRebarBlockBCFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri || !pBri->IsWingWall(bStt, bLeft)) return;

	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	CTwinVector tv;
	CTwinVectorArray tvArrB, tvArrC, tvArrD, tvArrCurve, tvArrMark;

	long nDan = 0;
	pBri->GetTvArrayRebarWingWallBlockBMainFront(tvArrB, bStt, bLeft, nOut, nDan);
	pBri->GetTvArrayRebarWingWallBlockCMainFront(tvArrCurve, tvArrC, bStt, bLeft, nOut, nDan);
	pBri->GetTvArrayRebarWingWallBlockDSupportFront(tvArrD, bStt, bLeft, nOut, nDan);
	int nSizeB = tvArrB.GetSize();
	int nSizeC = tvArrC.GetSize();
	int nSizeD = tvArrD.GetSize();

	double se = bStt ? -1.0 : 1.0;
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CDPoint vAngMark = bStt ? CDPoint(0, 1) : CDPoint(1, 0);
	CDPoint xy(0, 0), xyB(0, 0), xyE(0, 0);
	vector<double> y_hor, y_var, y_var_sec, y_var_sec_var, x_ver, x_ver_out;

	// B구간 수평 철근
	int i=0; for(i=0; i<nSizeD; i++)
	{
		tv = tvArrD.GetAt(i);
		if(tv.m_sInfo==_T("수직D") || tv.m_sInfo==_T("수직변단면구간D"))
			tvArrMark.Add(tv);
	}
	double dHorXD = pBri->m_Util.GetPosRebarMark(tvArrMark, FALSE, nSizeD);
	if(nSizeB>0)
	{
		int i=0; for(i=0; i<nSizeB; i++)
		{
			tv = tvArrB.GetAt(i);
			xy = CDPoint(tv.m_v1.x+MARK_DIST*se, tv.m_v1.y);
			if(tv.m_sInfo==_T("수평변화B"))
			{
				y_var.push_back(tv.m_v1.y);
				Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
			}
			if(tv.m_sInfo==_T("수평B"))
			{
				y_hor.push_back(tv.m_v1.y);
				Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
			}
			if(tv.m_sInfo==_T("수평변단면B"))
			{
				y_var_sec.push_back(tv.m_v1.y);
				Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
			}
			if(tv.m_sInfo==_T("수평변단면변화B"))
			{
				GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, CDPoint(dHorXD, 0), CDPoint(0, 1), xy);
				y_var_sec_var.push_back(tv.m_v1.y);
				Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
			}
		}

		// 수평변화B
		int nRBIndex = nOut==0 ? WW_REBAR_HOR_B_IN_VAR : WW_REBAR_HOR_B_OUT_VAR;
		CRebar* pRB = pWing->m_pArrRebar[nRBIndex];
		if(pRB->IsTrue() && !y_var.empty())
		{
			Dom.LineTo(xy.x, y_var.front(), xy.x, y_var.back());
			double dMarkY = pBri->m_Util.GetPosRebarMark(tvArrB, TRUE, y_hor.size()+y_var.size()/2);
			xyB = CDPoint(xy.x, dMarkY);
			xyE = CDPoint(xyB.x-MARK_DIST*se, xyB.y);
			Dom.LineTo(xyB, xyE);
			// Sym.RebarMarkLineSide(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), !bStt, 0, FALSE);

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = pRB->m_sMark;
			mark_input_data.dia = pRB->GetStringDia();
			mark_input_data.position = xyE;
			mark_input_data.ctc = nOut==0 ? pWing->m_dMainCTCB/2 : pWing->m_dMainCTCB;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.left_dir =  !bStt;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.mark_pos = eMarkSide;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}

		// 수평B
		nRBIndex = nOut==0 ? WW_REBAR_HOR_B_IN : WW_REBAR_HOR_B_OUT;
		pRB = pWing->m_pArrRebar[nRBIndex];
		if(pRB->IsTrue() && !y_hor.empty())
		{
			Dom.LineTo(xy.x, y_hor.front(), xy.x, y_hor.back());
			double dMarkY = pBri->m_Util.GetPosRebarMark(tvArrB, TRUE, y_hor.size()/2);
			xyB = CDPoint(xy.x, dMarkY);
			xyE = CDPoint(xyB.x-MARK_DIST*se, xyB.y);
			Dom.LineTo(xyB, xyE);
			// Sym.RebarMarkLineSide(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), !bStt, 0, FALSE);

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = pRB->m_sMark;
			mark_input_data.dia = pRB->GetStringDia();
			mark_input_data.position = xyE;
			mark_input_data.ctc = nOut==0 ? pWing->m_dMainCTCB/2 : pWing->m_dMainCTCB;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.left_dir =  !bStt;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.mark_pos = eMarkSide;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}

		// 수평변단면B
		if(nOut==0)
		{
			nRBIndex = WW_REBAR_HOR_B_IN_VAR_SEC;
			pRB = pWing->m_pArrRebar[nRBIndex];
			if(pRB->IsTrue() && !y_var_sec.empty())
			{
				Dom.LineTo(xy.x, y_var_sec.front(), xy.x, y_var_sec.back());
				double dMarkY = pBri->m_Util.GetPosRebarMark(tvArrB, TRUE, y_var.size()+y_hor.size()+y_var_sec.size()/2);
				xyB = CDPoint(xy.x, dMarkY);
				xyE = CDPoint(xyB.x-MARK_DIST*se, xyB.y);
				Dom.LineTo(xyB, xyE);
				// Sym.RebarMarkLineSide(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), !bStt, 0, FALSE);

				SRebarMarkInputData mark_input_data;
				SRebarMarkviewData mark_view_data;
				mark_input_data.mark = pRB->m_sMark;
				mark_input_data.dia = pRB->GetStringDia();
				mark_input_data.position = xyE;
				mark_input_data.ctc = nOut==0 ? pWing->m_dMainCTCB/2 : pWing->m_dMainCTCB;
				mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
				mark_input_data.left_dir =  !bStt;
				mark_input_data.height = 0;

				mark_view_data.count_mark = FALSE;
				mark_view_data.mark_pos = eMarkSide;

				Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
			}
		}

		//.. 수평변단면변화B
		if(nOut==0)
		{
			nRBIndex = WW_REBAR_HOR_B_IN_VAR_SEC_VAR;
			pRB = pWing->m_pArrRebar[nRBIndex];
			if(pRB->IsTrue() && !y_var_sec_var.empty())
			{
				Dom.LineTo(dHorXD, y_var_sec_var.front(), dHorXD, y_var_sec_var.back());
				CTwinVector tvTail;
				tvTail = tvArrD.GetAt(nSizeD-1);
				xyB = CDPoint(dHorXD, y_var_sec_var.front());
				GetXyMatchSegAndLine(tvTail.m_v1, tvTail.m_v2, xyB, CDPoint(0, 1), xyE);
				Dom.LineTo(xyB, xyE);
				// Sym.RebarMarkLineLower(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), bStt, 0, FALSE);
				SRebarMarkInputData mark_input_data;
				SRebarMarkviewData mark_view_data;
				mark_input_data.mark = pRB->m_sMark;
				mark_input_data.dia = pRB->GetStringDia();
				mark_input_data.position = xyE;
				mark_input_data.ctc = nOut==0 ? pWing->m_dMainCTCB/2 : pWing->m_dMainCTCB;
				mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
				mark_input_data.left_dir =  bStt;
				mark_input_data.height = 0;

				mark_view_data.count_mark = FALSE;
				mark_view_data.mark_pos = eMarkLower;

				Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
			}
		}
	}

	// C구간 수직 철근
	if(nSizeC>0)
	{
		double dVerY = pBri->m_Util.GetPosRebarMark(tvArrB, TRUE, 3);
		double dVerOutY = pBri->m_Util.GetPosRebarMark(tvArrB, TRUE, 7);
		if(nSizeB == 0 && nSizeC > 0)
		{
			double dMidY = tvArrC.GetXyBottom().y + (tvArrC.GetXyTop().y - tvArrC.GetXyBottom().y)/6; 
			dVerY			= dMidY; 
			dVerOutY		= dMidY;
		}

		int i=0; for(i=0; i<nSizeC; i++)
		{
			tv = tvArrC.GetAt(i);
			if(tv.m_sInfo==_T("수직C"))
			{
				xy = CDPoint(tv.m_v1.x, dVerY);
				x_ver.push_back(tv.m_v1.x);
			}
			if(tv.m_sInfo==_T("기초외수직C"))
			{
				xy = CDPoint(tv.m_v1.x, dVerOutY);
				x_ver_out.push_back(tv.m_v1.x);
			}
			Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
		}

		// 수직C
		int nRBIndex = nOut==0 ? WW_REBAR_VER_C_IN : WW_REBAR_VER_C_OUT;
		CRebar* pRB = pWing->m_pArrRebar[nRBIndex];
		if(pRB->IsTrue() && !x_ver.empty())
		{
			Dom.LineTo(x_ver.front(), dVerY, x_ver.back(), dVerY);
			xyB = CDPoint(x_ver.front(), dVerY);
			xyE = CDPoint(xy.x, dVerY);
			Dom.LineTo(xyB, xyE);
			// Sym.RebarMarkLineSide(&Dom, xyB, pRB->m_sMark, pRB->GetStringDia(), bStt, 0, FALSE);

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = pRB->m_sMark;
			mark_input_data.dia = pRB->GetStringDia();
			mark_input_data.position = xyB;
			mark_input_data.ctc = nOut==0 ? pWing->m_dStdVerCTC/2 : pWing->m_dStdVerCTC;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.left_dir =  bStt;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.mark_pos = eMarkSide;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}

		// 기초외수직C
		nRBIndex = nOut==0 ? WW_REBAR_VER_C_IN_EXC_FOOT : WW_REBAR_VER_C_OUT_EXC_FOOT;
		pRB = pWing->m_pArrRebar[nRBIndex];
		if(pRB->IsTrue() && !x_ver_out.empty())
		{
			Dom.LineTo(x_ver_out.front(), dVerOutY, x_ver_out.back(), dVerOutY);
			xyB = CDPoint(x_ver_out.front(), dVerOutY);
			xyE = CDPoint(xy.x, dVerOutY);
			Dom.LineTo(xyB, xyE);
//			Sym.RebarMarkLineSide(&Dom, xyB, pRB->m_sMark, pRB->GetStringDia(), bStt, 0, FALSE);

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = pRB->m_sMark;
			mark_input_data.dia = pRB->GetStringDia();
			mark_input_data.position = xyB;
			mark_input_data.ctc = nOut==0 ? pWing->m_dStdVerCTC/2 : pWing->m_dStdVerCTC;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.left_dir =  bStt;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.mark_pos = eMarkSide;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}
	}

	*pDomP << Dom;
}

void CDrawWingWall::MarkRebarBlockDFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri || !pBri->IsWingWall(bStt, bLeft)) return;
	
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	CTwinVector tv, tvUpper;
	CTwinVectorArray tvArrMain, tvArrSupp, tvArrMark, tvArrMarkVar, tvArrMarkVarUp, tvArrMarkVarHu, tvArrMarkD, tvArrWing, tvArrMark1, tvArrMark2;

	long nDan = 0;
	pBri->GetTvArrayRebarWingWallBlockDMainFront(tvArrMain, bStt, bLeft, nOut, nDan);
	pBri->GetTvArrayRebarWingWallBlockDSupportFront(tvArrSupp, bStt, bLeft, nOut, nDan);
	int i=0; for(i=0; i<tvArrMain.GetSize(); i++)
	{
		if(tvArrMain.GetAt(i).m_sInfo.Left(1)==_T("*"))
		{
			tvArrMain.RemoveAt(i);
			i--;
		}
	}
	int nSizeMain = tvArrMain.GetSize();
	int nSizeSupp = tvArrSupp.GetSize();

	if(nSizeMain==0 && nSizeSupp==0) return;

	tvUpper = tvArrMain.GetAt(nSizeMain-1);

	// 경사철근의 갯수  - 철근마크 위치를 잡아줄때 제대로된 위치를 잡기 위해 경사철근 갯수를 빼줘야한다.
	// 변경 == > 철근마크 위치 잡을때 날개벽정면끝단상부 중간 기준으로 잡는다.
	long nRebarSlope = pWing->GetCountRebarSlop(nOut);
	pBri->GetTvWingWallFront(bStt, bLeft, nOut, tvArrWing, TRUE, FALSE, TRUE, FALSE);
	tv = tvArrWing.GetTvByInfo("날개벽정면끝단상부");
	CDPoint xyWingMid = tv.GetXyMid();
	double dWingYMid = xyWingMid.y;

	double se = bStt ? -1.0 : 1.0;
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CDPoint vAngMark = bStt ? CDPoint(0, 1) : CDPoint(1, 0);
	CDPoint xy(0, 0), xyB(0, 0), xyE(0, 0), xyTail(0, 0);
	vector<double> y_slope, y_hor, y_var,y_var2, y_var_sec, y_var_sec_block, y_var_under, x_ver, x_var, x_var_sec_block, x_ver_up, x_var_hu, x_ver2, x_var2;

	// 주철근
	for(i=0; i<nSizeSupp; i++)
	{
		tv = tvArrSupp.GetAt(i);
		if(tv.m_sInfo==_T("수직D") || tv.m_sInfo==_T("수직D_2"))
		{
			tvArrMarkD.Add(tv);
			tvArrMark.Add(tv);
			if(tv.m_sInfo == _T("수직D"))
				tvArrMark1.Add(tv);
			else
				tvArrMark2.Add(tv);
		}
		if(tv.m_sInfo==_T("수직변단면구간D"))
			tvArrMarkVar.Add(tv);
		if(tv.m_sInfo==_T("변단면상부수직D"))
		{
			tvArrMarkD.Add(tv);
			tvArrMarkVarUp.Add(tv);
		}
		if(tv.m_sInfo==_T("수직변단면헌치D"))
			tvArrMarkVarHu.Add(tv);
	}
	BOOL bMerged = pBri->IsMergedRebarWingWall(bStt, bLeft, nOut, nDan, 'A', 'D');
	BOOL bMergedBD = pBri->IsMergedRebarWingWall(bStt, bLeft, nOut, nDan, 'B', 'D');
	BOOL bCrossVar = pWing->m_nDirHunch==2 && nOut==0;
	int nSizeMark = tvArrMark.GetSize();
	int nSizeMarkVar = tvArrMarkVar.GetSize();
	int nSizeMarkVarUp = tvArrMarkVarUp.GetSize();
	double dMarkDist = nOut==0 && pWing->m_bSelMainCTCD ? MARK_DIST*1.5 : MARK_DIST;
	double dHalfCTC = (nOut==0 && pWing->m_bSelMainCTCD == FALSE) ? pWing->m_dMainCTCA/4 : pWing->m_dMainCTCA/2;
	double dHalfCTC2 = (nOut==0) ? pWing->m_dMainCTCA/4 : pWing->m_dMainCTCA/2;
	double dSlopeX = bMerged && !bCrossVar ? tvArrMain.GetAt(0).m_v1.x+dMarkDist*se : pBri->m_Util.GetPosRebarMark(tvArrMarkD, FALSE, nSizeMark-2);
	double dHorX = bMerged && !bCrossVar ? tvUpper.m_v1.x+dMarkDist*se : pBri->m_Util.GetPosRebarMark(tvArrMarkD, FALSE, 3);
	double dHorXVar = tvArrMain.GetAt(0).m_v1.x+dMarkDist*se;
	double dHorXVar2 = 0, dHorXVar2_End =0, dMaxYVar2=0, dMinYVar2 =0;
	double dDistExt = nOut == 1? 0 : -300;
	double dMarkY = 0;
	for(i=0; i<nSizeMain; i++)
	{
		tv = tvArrMain.GetAt(i);
		if(tv.m_sInfo==_T("상단경사D"))
		{
			GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, CDPoint(dSlopeX, 0), CDPoint(0, 1), xy);
			y_slope.push_back(xy.y);
			Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
		}
		if(tv.m_sInfo==_T("수평변화D") ) 
		{
			double dHorXTemp = (bMerged && !bCrossVar)? dHorX : pBri->m_Util.GetPosRebarMark(tvArrMarkD, FALSE, tvArrMarkD.GetSize()-2);
			if(bCrossVar) dHorXTemp = pBri->m_Util.GetPosRebarMark(tvArrMarkD, FALSE, (long)(pWing->m_dLT/(pWing->m_dStdVerCTC/2)));
			if(GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, CDPoint(dHorXTemp, 0), CDPoint(0, 1), xy))
			{
				Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
			}
			y_var.push_back(xy.y);
		}
		if(tv.m_sInfo==_T("수평변단면구간D"))
		{
			GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, CDPoint(dHorX+MARK_DIST/2*se, 0), CDPoint(0, 1), xy);
			y_var_sec_block.push_back(xy.y);
			Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
		}
		if(tv.m_sInfo==_T("수평D"))
		{
			GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, CDPoint(dHorX, 0), CDPoint(0, 1), xy);
			y_hor.push_back(xy.y);
			Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
		}
		if(tv.m_sInfo==_T("수평변단면D"))
		{
			GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, CDPoint(dHorXVar, 0), CDPoint(0, 1), xy);
			y_var_sec.push_back(xy.y);
			Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
		}
		if(tv.m_sInfo==_T("수평변단면하단D"))
		{
			GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, CDPoint(dHorXVar, 0), CDPoint(0, 1), xy);
			y_var_under.push_back(xy.y);
			Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
		}
		if(tv.m_sInfo==_T("수평변화D2") )//ARCBRIDGE-2724 (철근 제원 추가)
		{
			BOOL bFirst = (dMaxYVar2 == 0 && dMinYVar2 == 0)? TRUE : FALSE;
			
			if(bFirst)
			{				
				dHorXVar2		= bMergedBD ?  tv.m_v1.x+MARK_DIST*se + dDistExt : pBri->m_Util.GetPosRebarMark(tvArrMarkD, FALSE, tvArrMarkD.GetSize()-1);
				dHorXVar2_End	= tv.m_v1.x;
				dMaxYVar2	    =  tv.GetXyTop().y;
				dMinYVar2		= tv.GetXyBottom().y;
			}
			else
			{
				dMaxYVar2		 = max(dMaxYVar2, tv.GetXyTop().y);
				dMinYVar2		 = min(dMinYVar2, tv.GetXyBottom().y);
			}		

			if(GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, CDPoint(dHorXVar2, 0), CDPoint(0, 1), xy))
			{
				Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
			}
			y_var2.push_back(xy.y);
		}

		if(max(tv.m_v1.y, tv.m_v2.y) < dWingYMid)
			dMarkY = max(tv.m_v1.y, tv.m_v2.y) + dHalfCTC;
	}

	if(dMarkY == 0 && nSizeMain > 0)
	{
		tv = tvArrMain.GetAt(0);
		dMarkY = max(tv.m_v1.y, tv.m_v2.y) + dHalfCTC;
	}

	if(nSizeMain>0)
	{
		//.. 상단경사D
		int nRBIndex = nOut==0 ? WW_REBAR_SLOPE_D_IN : WW_REBAR_SLOPE_D_OUT;
		CRebar* pRB = pWing->m_pArrRebar[nRBIndex];
		if(pRB->IsTrue() && !y_slope.empty())
		{
			Dom.LineTo(dSlopeX, y_slope.front(), dSlopeX, y_slope.back());
			xyE = CDPoint(dSlopeX, y_slope.back());
			// Sym.RebarMarkLineUpper(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), bMerged ? !bStt : bStt, 0, FALSE);

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = pRB->m_sMark;
			mark_input_data.dia = pRB->GetStringDia();
			mark_input_data.position = xyE;
			mark_input_data.ctc = (nOut==0 && !pWing->m_bSelMainCTCD) ? pWing->m_dMainCTCA/2 : pWing->m_dMainCTCA;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.left_dir =  bMerged ? !bStt : bStt;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.mark_pos = eMarkUpper;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}

		//.. 수평변화D
		nRBIndex = nOut==0 ? WW_REBAR_HOR_D_IN_VAR : WW_REBAR_HOR_D_OUT_VAR;
		pRB = pWing->m_pArrRebar[nRBIndex];
		if(pRB->IsTrue() && !y_var.empty())
		{
			if(bMerged && !bCrossVar)
			{
				Dom.LineTo(dHorX, y_var.front(), dHorX, y_var.back());
				double dMarkY = pBri->m_Util.GetPosRebarMark(tvArrMain, TRUE, y_var2.size() + y_var.size()/2);
				xyB = CDPoint(dHorX, dMarkY+dHalfCTC2);
				xyE = CDPoint(xyB.x-dMarkDist*se, xyB.y);
				Dom.LineTo(xyB, xyE);
				// Sym.RebarMarkLineSide(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), !bStt, 0, FALSE);
			}
			else
			{
				double dHorXTemp = pBri->m_Util.GetPosRebarMark(tvArrMarkD, FALSE, tvArrMarkD.GetSize()-2);
				if(bCrossVar) dHorXTemp = pBri->m_Util.GetPosRebarMark(tvArrMarkD, FALSE, (long)(pWing->m_dLT/(pWing->m_dStdVerCTC/2)));
				Dom.LineTo(dHorXTemp, y_var.front(), dHorXTemp, y_var.back());
				CTwinVector tvTail;
				tvTail = tvArrSupp.GetAt(nSizeSupp-1);
				xyB = CDPoint(dHorXTemp, y_var.front());
				GetXyMatchSegAndLine(tvTail.m_v1, tvTail.m_v2, xyB, CDPoint(0, 1), xyE);
				Dom.LineTo(xyB, xyE);
				// Sym.RebarMarkLineLower(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), bStt, 0, FALSE);
			}

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = pRB->m_sMark;
			mark_input_data.dia = pRB->GetStringDia();
			mark_input_data.position = xyE;
			mark_input_data.ctc = (nOut==0 && !pWing->m_bSelMainCTCD) ? pWing->m_dMainCTCA/2 : pWing->m_dMainCTCA;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.left_dir =  (bMerged && !bCrossVar)? !bStt : bStt;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.mark_pos = (bMerged && !bCrossVar)? eMarkSide : eMarkLower;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}

		//.. 수평변화D2
		nRBIndex = nOut==0 ? WW_REBAR_HOR_D_IN_VAR2 : WW_REBAR_HOR_D_OUT_VAR2;
		pRB = pWing->m_pArrRebar[nRBIndex];
		if(pRB->IsTrue() && !y_var2.empty())
		{
			//
			Dom.LineTo(dHorXVar2, y_var2.front(), dHorXVar2, y_var2.back());

			if(bMergedBD)
			{
				double dMarkY = dMinYVar2 + (dMaxYVar2 - dMinYVar2)/2 + dDistExt;
				xyB = CDPoint(dHorXVar2, dMarkY-dHalfCTC);
				xyE = CDPoint(dHorXVar2_End-dDistExt, xyB.y);
				Dom.LineTo(xyB, xyE);
			}
			else
			{
				xyE = CDPoint(dHorXVar2, y_var2.front());
			}
			// Sym.RebarMarkLineSide(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), !bStt, 0, FALSE);

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = pRB->m_sMark;
			mark_input_data.dia = pRB->GetStringDia();
			mark_input_data.position = xyE;
			mark_input_data.ctc = (nOut==0 && !pWing->m_bSelMainCTCD) ? pWing->m_dMainCTCA/2 : pWing->m_dMainCTCA;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.left_dir = bMergedBD ? !bStt : bStt;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.mark_pos = bMergedBD ? eMarkSide : eMarkLower;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}


		//.. 수평변단면구간D
		nRBIndex = nOut==0 ? WW_REBAR_HOR_D_IN_VAR_SEC_BLOCK : WW_REBAR_HOR_D_OUT_VAR_SEC_BLOCK;
		pRB = pWing->m_pArrRebar[nRBIndex];
		if(pRB->IsTrue() && !y_var_sec_block.empty())
		{
			Dom.LineTo(dHorX+MARK_DIST/2*se, y_var_sec_block.front(), dHorX+MARK_DIST/2*se, y_var_sec_block.back());
			if(bMerged)
			{
				double dMarkY = pBri->m_Util.GetPosRebarMark(tvArrMain, TRUE, y_var2.size() + y_var.size()+y_var_sec_block.size()/2);
				xyB = CDPoint(dHorX+MARK_DIST/2*se, dMarkY-dHalfCTC2);
				xyE = CDPoint(xyB.x-(dMarkDist+MARK_DIST/2)*se, xyB.y);
				Dom.LineTo(xyB, xyE);
				// Sym.RebarMarkLineSide(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), !bStt, 0, FALSE);
			}
			else
			{
				xyB = CDPoint(dHorX+MARK_DIST/2*se, y_var_sec_block.back());
				GetXyMatchSegAndLine(tvUpper.m_v1, tvUpper.m_v2, xyB, CDPoint(0, 1), xyE);
				Dom.LineTo(xyB, xyE);
				// Sym.RebarMarkLineUpper(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), !bStt, 0, FALSE);
			}

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = pRB->m_sMark;
			mark_input_data.dia = pRB->GetStringDia();
			mark_input_data.position = xyE;
			mark_input_data.ctc = (nOut==0 && !pWing->m_bSelMainCTCD) ? pWing->m_dMainCTCA/2 : pWing->m_dMainCTCA;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.left_dir = !bStt;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.mark_pos = bMerged ? eMarkSide : eMarkUpper;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}


		//.. 수평D
		nRBIndex = nOut==0 ? WW_REBAR_HOR_D_IN : WW_REBAR_HOR_D_OUT;
		pRB = pWing->m_pArrRebar[nRBIndex];
		if(pRB->IsTrue() && !y_hor.empty())
		{
			Dom.LineTo(dHorX, y_hor.front(), dHorX, y_hor.back());
			if(bMerged && !bCrossVar)
			{
				double dMarkY = pBri->m_Util.GetPosRebarMark(tvArrMain, TRUE, y_var2.size()+y_var.size()+y_var_sec_block.size()+y_hor.size()/2);
				xyB = CDPoint(dHorX, dMarkY+dHalfCTC2);
				xyE = CDPoint(xyB.x-dMarkDist*se, xyB.y);
				Dom.LineTo(xyB, xyE);
				// Sym.RebarMarkLineSide(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), !bStt, 0, FALSE);
			}
			else
			{
				xyB = CDPoint(dHorX, y_hor.back());
				GetXyMatchSegAndLine(tvUpper.m_v1, tvUpper.m_v2, xyB, CDPoint(0, 1), xyE);
				Dom.LineTo(xyB, xyE);
				// Sym.RebarMarkLineUpper(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), bStt, 0, FALSE);
			}

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = pRB->m_sMark;
			mark_input_data.dia = pRB->GetStringDia();
			mark_input_data.position = xyE;
			mark_input_data.ctc = (nOut==0 && !pWing->m_bSelMainCTCD) ? pWing->m_dMainCTCA/2 : pWing->m_dMainCTCA;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.left_dir = (bMerged && !bCrossVar) ? !bStt : bStt;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.mark_pos = (bMerged && !bCrossVar) ? eMarkSide : eMarkUpper;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}

		//.. 수평변단면D
		if(nOut==0)
		{
			nRBIndex = WW_REBAR_HOR_D_IN_VAR_SEC;
			pRB = pWing->m_pArrRebar[nRBIndex];
			if(pRB->IsTrue() && !y_var_sec.empty())
			{
				Dom.LineTo(dHorXVar, y_var_sec.front(), dHorXVar, y_var_sec.back());
				double dMarkY = pBri->m_Util.GetPosRebarMark(tvArrMain, TRUE, y_var2.size() + y_var_under.size()+y_var_sec.size()/2);
				xyB = CDPoint(dHorXVar, dMarkY+dHalfCTC2);
				xyE = CDPoint(xyB.x-dMarkDist*se, xyB.y);
				Dom.LineTo(xyB, xyE);
				// Sym.RebarMarkLineSide(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), !bStt, 0, FALSE);

				SRebarMarkInputData mark_input_data;
				SRebarMarkviewData mark_view_data;
				mark_input_data.mark = pRB->m_sMark;
				mark_input_data.dia = pRB->GetStringDia();
				mark_input_data.position = xyE;
				mark_input_data.ctc = (nOut==0 && !pWing->m_bSelMainCTCD) ? pWing->m_dMainCTCA/2 : pWing->m_dMainCTCA;
				mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
				mark_input_data.left_dir = !bStt;
				mark_input_data.height = 0;

				mark_view_data.count_mark = FALSE;
				mark_view_data.mark_pos = eMarkSide;

				Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
			}
		}

		//.. 수평변화D 변단면구간하부..
		if(nOut==0)
		{
			nRBIndex = WW_REBAR_HOR_D_IN_VAR_UNDER;
			pRB = pWing->m_pArrRebar[nRBIndex];

			if(pRB->IsTrue() && !y_var_under.empty())
			{
				Dom.LineTo(dHorXVar, y_var_under.front(), dHorXVar, y_var_under.back());
				double dMarkY = pBri->m_Util.GetPosRebarMark(tvArrMain, TRUE, y_var2.size() + y_var_under.size()/2);
				xyB = CDPoint(dHorXVar, dMarkY+dHalfCTC2);
				xyE = CDPoint(xyB.x-dMarkDist*se, xyB.y);
				Dom.LineTo(xyB, xyE);
				// Sym.RebarMarkLineSide(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), !bStt, 0, FALSE);

				SRebarMarkInputData mark_input_data;
				SRebarMarkviewData mark_view_data;
				mark_input_data.mark = pRB->m_sMark;
				mark_input_data.dia = pRB->GetStringDia();
				mark_input_data.position = xyE;
				mark_input_data.ctc = (nOut==0 && !pWing->m_bSelMainCTCD) ? pWing->m_dMainCTCA/2 : pWing->m_dMainCTCA;
				mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
				mark_input_data.left_dir = !bStt;
				mark_input_data.height = 0;

				mark_view_data.count_mark = FALSE;
				mark_view_data.mark_pos = eMarkSide;

				Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
			}
		}
	}

	// 배력 철근
	//#0027990
	//long nVariableSize = y_var.capacity(); //변화철근의 철근갯수를 세어 변화구간이 없는 수직구간의 인덱스를 찾는다
	//double dVerY = pBri->m_Util.GetPosRebarMark(tvArrMain, TRUE, nSizeMain-2-nRebarSlope-nVariableSize) + dHalfCTC;
	//double dVerY = pBri->m_Util.GetPosRebarMark(tvArrMain, TRUE, nSizeMain-2-nRebarSlope) + dHalfCTC;
	double dVerY = dMarkY;
	double dVarY = pBri->m_Util.GetPosRebarMark(tvArrMain, TRUE, y_var2.size() + y_var.size()-nRebarSlope);
	double dVarY2 = pBri->m_Util.GetPosRebarMark(tvArrMain, TRUE, y_var2.size() + y_var.size()+4-nRebarSlope);
	if(nOut == 0)
	{
		dVerY += pWing->m_dMainCTCA/4;
	}

	if(nSizeSupp>0)
	{
		for(i=0; i<nSizeSupp; i++)
		{
			tv = tvArrSupp.GetAt(i);
			if(tv.m_sInfo.Left(4)==_T("꼬리"))
			{
				if(tv.m_sInfo==_T("꼬리경사"))
				{
					xyTail = tv.GetXyMid();
					Sym.RebarCountMarkOne(&Dom, xyTail, vAngMark);
				}
			}
			else
			{
				if(tv.m_sInfo==_T("수직D"))
				{
					x_ver.push_back(tv.m_v2.x);
					xy = CDPoint(tv.m_v2.x, dVerY);
					Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
				}
				if(tv.m_sInfo==_T("수직D_2"))
				{
					x_ver2.push_back(tv.m_v2.x);
					xy = CDPoint(tv.m_v2.x, dVerY);
					Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
				}
				if(tv.m_sInfo==_T("변단면수직D"))
				{
					x_var.push_back(tv.m_v2.x);
					if(GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, CDPoint(0, dVarY), CDPoint(1, 0), xy))
					{
						Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
					}
				}
				if(tv.m_sInfo==_T("변단면수직D_2"))
				{
					x_var2.push_back(tv.m_v2.x);
					xy = CDPoint(tv.m_v2.x, dVarY);
					Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
				}
				if(tv.m_sInfo==_T("수직변단면구간D"))
				{
					x_var_sec_block.push_back(tv.m_v2.x);
					xy = CDPoint(tv.m_v2.x, dVerY);
					Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
				}
				if(tv.m_sInfo==_T("변단면상부수직D"))
				{
					x_ver_up.push_back(tv.m_v2.x);
					xy = CDPoint(tv.m_v2.x, dVerY);
					Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
				}
				if(tv.m_sInfo==_T("수직변단면헌치D"))
				{
					x_var_hu.push_back(tv.m_v2.x);
					xy = CDPoint(tv.m_v2.x, dVarY2);
					Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
				}
			}
		}

		//.. 수직D
		int nRBIndex = nOut==0 ? WW_REBAR_VER_D_IN : WW_REBAR_VER_D_OUT;
		CRebar* pRB = pWing->m_pArrRebar[nRBIndex];
		if(pRB->IsTrue() && !x_ver.empty())
		{
			Dom.LineTo(x_ver.front(), dVerY, x_ver.back(), dVerY);
			double dHorX = pBri->m_Util.GetPosRebarMark(tvArrMark1, FALSE, tvArrMark1.GetSize()/2);
			xyB = CDPoint(dHorX, dVerY);
			GetXyMatchSegAndLine(tvUpper.m_v1, tvUpper.m_v2, xyB, CDPoint(0, 1), xyE);
			Dom.LineTo(xyB, xyE);
			// Sym.RebarMarkLineUpper(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), bStt, 0, FALSE);

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = pRB->m_sMark;
			mark_input_data.dia = pRB->GetStringDia();
			mark_input_data.position = xyE;
			mark_input_data.ctc = (nOut==0 && !pWing->m_bSelMainCTCD) ? pWing->m_dStdVerCTC/2 : pWing->m_dStdVerCTC;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.left_dir = bStt;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.mark_pos = eMarkUpper;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}

		nRBIndex = nOut==0 ? WW_REBAR_VER_D_IN_FLAT : WW_REBAR_VER_D_OUT_FLAT;
		pRB = pWing->m_pArrRebar[nRBIndex];
		if(pRB->IsTrue() && !x_ver2.empty())
		{
			Dom.LineTo(x_ver2.front(), dVerY, x_ver2.back(), dVerY);
			double dHorX = pBri->m_Util.GetPosRebarMark(tvArrMark2, FALSE, tvArrMark2.GetSize()/2);
			xyB = CDPoint(dHorX, dVerY);
			GetXyMatchSegAndLine(tvUpper.m_v1, tvUpper.m_v2, xyB, CDPoint(0, 1), xyE);
			Dom.LineTo(xyB, xyE);
			// Sym.RebarMarkLineUpper(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), bStt, 0, FALSE);

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = pRB->m_sMark;
			mark_input_data.dia = pRB->GetStringDia();
			mark_input_data.position = xyE;
			mark_input_data.ctc = (nOut==0 && !pWing->m_bSelMainCTCD) ? pWing->m_dStdVerCTC/2 : pWing->m_dStdVerCTC;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.left_dir = bStt;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.mark_pos = eMarkUpper;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}

		//.. 변단면수직D
		if(nOut==0)
		{
			nRBIndex = WW_REBAR_VER_D_IN_VAR_SEC;
			pRB = pWing->m_pArrRebar[nRBIndex];
			if(pRB->IsTrue() && !x_var.empty())
			{
				Dom.LineTo(x_var.front(), dVarY, x_var.back(), dVarY);
				xyE = CDPoint(x_var.front(), dVarY);
				// Sym.RebarMarkLineSide(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), bStt, fabs(tvUpper.m_v2.x-x_var.front()), FALSE);
				SRebarMarkInputData mark_input_data;
				SRebarMarkviewData mark_view_data;
				mark_input_data.mark = pRB->m_sMark;
				mark_input_data.dia = pRB->GetStringDia();
				mark_input_data.position = xyE;
				mark_input_data.ctc = (nOut==0 && !pWing->m_bSelMainCTCD) ? pWing->m_dStdVerCTC/2 : pWing->m_dStdVerCTC;
				mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
				mark_input_data.left_dir = bStt;
				mark_input_data.height = fabs(tvUpper.m_v2.x-x_var.front());

				mark_view_data.count_mark = FALSE;
				mark_view_data.mark_pos = eMarkSide;

				Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
			}

			nRBIndex = WW_REBAR_VER_D_IN_VAR_SEC_FLAT;
			pRB = pWing->m_pArrRebar[nRBIndex];
			if(pRB->IsTrue() && !x_var2.empty())
			{
				Dom.LineTo(x_var2.front(), dVarY, x_var2.back() - 1000*se, dVarY);
				xyE = CDPoint(x_var2.back() - 1000*se, dVarY);
				// Sym.RebarMarkLineSide(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), !bStt, fabs(tvUpper.m_v2.x-x_var.front()), FALSE);
				SRebarMarkInputData mark_input_data;
				SRebarMarkviewData mark_view_data;
				mark_input_data.mark = pRB->m_sMark;
				mark_input_data.dia = pRB->GetStringDia();
				mark_input_data.position = xyE;
				mark_input_data.ctc = (nOut==0 && !pWing->m_bSelMainCTCD) ? pWing->m_dStdVerCTC/2 : pWing->m_dStdVerCTC;
				mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
				mark_input_data.left_dir = !bStt;
				mark_input_data.height = fabs(tvUpper.m_v2.x-x_var.front());

				mark_view_data.count_mark = FALSE;
				mark_view_data.mark_pos = eMarkSide;

				Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
			}
		}

		//.. 수직변단면구간D
		nRBIndex = nOut==0 ? WW_REBAR_VER_D_IN_VAR_SEC_BLOCK : WW_REBAR_VER_D_OUT_VAR_SEC_BLOCK;
		pRB = pWing->m_pArrRebar[nRBIndex];
		if(pRB->IsTrue() && !x_var_sec_block.empty())
		{
			Dom.LineTo(x_var_sec_block.front(), dVerY, x_var_sec_block.back(), dVerY);
			double dHorX = pBri->m_Util.GetPosRebarMark(tvArrMarkVar, FALSE, nSizeMarkVar/2);
			xyB = CDPoint(dHorX, dVerY);
			GetXyMatchSegAndLine(tvUpper.m_v1, tvUpper.m_v2, xyB, CDPoint(0, 1), xyE);
			Dom.LineTo(xyB, xyE);
			// Sym.RebarMarkLineUpper(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), !bStt, 0, FALSE);

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = pRB->m_sMark;
			mark_input_data.dia = pRB->GetStringDia();
			mark_input_data.position = xyE;
			mark_input_data.ctc = (nOut==0 && !pWing->m_bSelMainCTCD) ? pWing->m_dStdVerCTC/2 : pWing->m_dStdVerCTC;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.left_dir = !bStt;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.mark_pos = eMarkUpper;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}

		//.. 변단변 상부수직철근D
		if(nOut==0)
		{
			nRBIndex = WW_REBAR_VER_D_IN_VAR_UP;
			pRB = pWing->m_pArrRebar[nRBIndex];
			if(pRB->IsTrue() && !x_ver_up.empty())
			{
				Dom.LineTo(x_ver_up.front(), dVerY, x_ver_up.back(), dVerY);
				double dHorX = pBri->m_Util.GetPosRebarMark(tvArrMarkVarUp, FALSE, nSizeMarkVarUp/2);
				xyB = CDPoint(dHorX, dVerY);
				GetXyMatchSegAndLine(tvUpper.m_v1, tvUpper.m_v2, xyB, CDPoint(0, 1), xyE);
				Dom.LineTo(xyB, xyE);
				// Sym.RebarMarkLineUpper(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), bStt, 0, FALSE);

				SRebarMarkInputData mark_input_data;
				SRebarMarkviewData mark_view_data;
				mark_input_data.mark = pRB->m_sMark;
				mark_input_data.dia = pRB->GetStringDia();
				mark_input_data.position = xyE;
				mark_input_data.ctc = (nOut==0 && !pWing->m_bSelMainCTCD) ? pWing->m_dStdVerCTC/2 : pWing->m_dStdVerCTC;
				mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
				mark_input_data.left_dir = bStt;
				mark_input_data.height = 0;

				mark_view_data.count_mark = FALSE;
				mark_view_data.mark_pos = eMarkUpper;

				Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
			}
		}

		//.. 변단면 수직 헌치철근D
		if(nOut==0)
		{
			nRBIndex = WW_REBAR_VER_D_IN_VAR_HU;
			pRB = pWing->m_pArrRebar[nRBIndex];
			if(pRB->IsTrue() && !x_var_hu.empty())
			{
				Dom.LineTo(x_var_hu.front(), dVarY2, x_var_hu.back(), dVarY2);
				xyE = CDPoint(x_var_hu.front(), dVarY2);
				// Sym.RebarMarkLineSide(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), bStt, fabs(tvUpper.m_v2.x-x_var_hu.front()), FALSE);

				SRebarMarkInputData mark_input_data;
				SRebarMarkviewData mark_view_data;
				mark_input_data.mark = pRB->m_sMark;
				mark_input_data.dia = pRB->GetStringDia();
				mark_input_data.position = xyE;
				mark_input_data.ctc = (nOut==0 && !pWing->m_bSelMainCTCD) ? pWing->m_dStdVerCTC/2 : pWing->m_dStdVerCTC;
				mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
				mark_input_data.left_dir = bStt;
				mark_input_data.height = fabs(tvUpper.m_v2.x-x_var_hu.front());

				mark_view_data.count_mark = FALSE;
				mark_view_data.mark_pos = eMarkSide;

				Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
			}
		}

		//.. 끝단철근D
		nRBIndex = nOut==0 ? WW_REBAR_TAIL_END_IN : WW_REBAR_TAIL_END_OUT;
		pRB = pWing->m_pArrRebar[nRBIndex];
		if(pRB->IsTrue())
		{
			// Sym.RebarMarkLineLower(&Dom, xyTail, pRB->m_sMark, pRB->GetStringDia(), bStt, 0, FALSE);

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = pRB->m_sMark;
			mark_input_data.dia = pRB->GetStringDia();
			mark_input_data.position = xyTail;
			mark_input_data.ctc = 0;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.left_dir = bStt;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.mark_pos = eMarkLower;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}
	}

	*pDomP << Dom;
}

void CDrawWingWall::MarkRebarHunchWallFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri || !pBri->IsWingWall(bStt, bLeft)) return;

	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	CTwinVector tv;
	CTwinVectorArray tvArr;
	pBri->GetTvArrayRebarWingWallHunchWallFront(tvArr, bStt, bLeft);

	for (long ix =0; ix < tvArr.GetSize(); ix++)
	{
		CTwinVector tvHunRebar = tvArr.GetAt(ix);
		double dLen = tvHunRebar.GetLength();
		if(Compare(dLen, 0.0, _T("<="), 0.1))
		{
			tvArr.RemoveAt(ix);
			ix--;
		}
	}

	int nSize = tvArr.GetSize();
	if(nSize==0) return;
	if(!(pWing->m_dHuH2 > 0 && pWing->m_dHuW2 > 0)) return;
	

	double dB = pBri->GetLengthWingWallBlockBC(bStt, bLeft, TRUE);
	double se = bStt ? -1.0 : 1.0;
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	double dXA = 0, dXB = 0;
	CDPoint vAngMark = bStt ? CDPoint(0, 1) : CDPoint(1, 0);
	CDPoint xy(0, 0), xyB(0, 0), xyE(0, 0);
	vector<double> y_hun1, y_hun2;

	// 벽체헌치철근1
	int nRBIndex = WW_REBAR_HUNCH_WALL_UP;
	CRebar* pRBup = pWing->m_pArrRebar[nRBIndex];
	nRBIndex = WW_REBAR_HUNCH_WALL_LO;
	CRebar* pRBlo = pWing->m_pArrRebar[nRBIndex];

	int i=0; for(i=0; i<nSize; i++)
	{
		tv = tvArr.GetAt(i);
		xy = CDPoint(tv.m_v1.x+MARK_DIST/2*se, tv.m_v1.y);
		Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
		if(tv.m_sInfo==_T("벽체헌치"))
		{
			if(tv.m_v1.y>dB || !(pRBlo->IsTrue()))
			{
				y_hun1.push_back(tv.m_v1.y);
				dXA = xy.x;
			}
			else
			{
				y_hun2.push_back(tv.m_v1.y);
				dXB = xy.x;
			}
		}
	}

	if(pRBup->IsTrue() && !y_hun1.empty())
	{
		xy.x = dXA;
		Dom.LineTo(xy.x, y_hun1.front(), xy.x, y_hun1.back());
		double dVerY = pBri->m_Util.GetPosRebarMark(tvArr, TRUE, y_hun2.size()+1);
		xyB = CDPoint(xy.x, dVerY);
		xyE = CDPoint(xyB.x-MARK_DIST/2*se, xyB.y);
		Dom.LineTo(xyB, xyE);
		// Sym.RebarMarkLineSide(&Dom, xyE, pRBup->m_sMark, pRBup->GetStringDia(), !bStt, 0, FALSE);

		SRebarMarkInputData mark_input_data;
		SRebarMarkviewData mark_view_data;
		mark_input_data.mark = pRBup->m_sMark;
		mark_input_data.dia = pRBup->GetStringDia();
		mark_input_data.position = xyE;
		mark_input_data.ctc = (pWing->m_nHunchCTC == 0) ? pWing->m_dMainCTCA / 2 : pWing->m_dMainCTCA;
		mark_input_data.type_rebar = pRBup? pRBup->m_nTypeRebar : -1;
		mark_input_data.left_dir = !bStt;
		mark_input_data.height = 0;

		mark_view_data.count_mark = FALSE;
		mark_view_data.mark_pos = eMarkSide;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}

	// 벽체헌치철근2
	if(pRBlo->IsTrue() && !y_hun2.empty())
	{
		xy.x = dXB;
		Dom.LineTo(xy.x, y_hun2.front(), xy.x, y_hun2.back());
		double dVerY = pBri->m_Util.GetPosRebarMark(tvArr, TRUE, 4);
		xyB = CDPoint(xy.x, dVerY);
		xyE = CDPoint(xyB.x-MARK_DIST/2*se, xyB.y);
		Dom.LineTo(xyB, xyE);
		// Sym.RebarMarkLineSide(&Dom, xyE, pRBlo->m_sMark, pRBlo->GetStringDia(), !bStt, 0, FALSE);

		SRebarMarkInputData mark_input_data;
		SRebarMarkviewData mark_view_data;
		mark_input_data.mark = pRBlo->m_sMark;
		mark_input_data.dia = pRBlo->GetStringDia();
		mark_input_data.position = xyE;
		mark_input_data.ctc = (pWing->m_nHunchCTC == 0) ? pWing->m_dMainCTCA / 2 : pWing->m_dMainCTCA;
		mark_input_data.type_rebar = pRBlo? pRBlo->m_nTypeRebar : -1;
		mark_input_data.left_dir = !bStt;
		mark_input_data.height = 0;

		mark_view_data.count_mark = FALSE;
		mark_view_data.mark_pos = eMarkSide;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}

	*pDomP << Dom;
}

void CDrawWingWall::MarkRebarHunchFootingFront(CDomyun* pDomP, BOOL bStt, BOOL bLeft)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri || !pBri->IsWingWall(bStt, bLeft)) return;

	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	CTwinVector tv;
	CTwinVectorArray tvArr;
	pBri->GetTvArrayRebarWingWallHunchFootingFront(tvArr, bStt, bLeft);
	int nSize = tvArr.GetSize();
	if(nSize==0) return;

	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CDPoint vAngMark = bStt ? CDPoint(0, 1) : CDPoint(1, 0);
	CDPoint xy(0, 0);
	vector<double> x_hun;
	int i=0; for(i=0; i<nSize; i++)
	{
		tv = tvArr.GetAt(i);
		xy = CDPoint(tv.m_v1.x, tv.m_v1.y+MARK_DIST);
		Sym.RebarCountMarkOne(&Dom, xy, vAngMark);
		if(tv.m_sInfo==_T("기초헌치"))
			x_hun.push_back(tv.m_v1.x);
	}

	// 기초헌치철근
	int nRBIndex = WW_REBAR_HUNCH_FOOT;
	CRebar* pRB = pWing->m_pArrRebar[nRBIndex];
	if(pRB->IsTrue() && !x_hun.empty())
	{
		xy.x = x_hun.front();
		Dom.LineTo(x_hun.front(), xy.y, x_hun.back(), xy.y);
		// Sym.RebarMarkLineSide(&Dom, xy, pRB->m_sMark, pRB->GetStringDia(), bStt, 0, FALSE);

		SRebarMarkInputData mark_input_data;
		SRebarMarkviewData mark_view_data;
		mark_input_data.mark = pRB->m_sMark;
		mark_input_data.dia = pRB->GetStringDia();
		mark_input_data.position = xy;
		mark_input_data.ctc = (pWing->m_nHunchCTC == 0) ? pWing->m_dStdVerCTC / 2 : pWing->m_dStdVerCTC;
		mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
		mark_input_data.left_dir = bStt;
		mark_input_data.height = 0;

		mark_view_data.count_mark = FALSE;
		mark_view_data.mark_pos = eMarkSide;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}

	*pDomP << Dom;
}

void CDrawWingWall::MarkRebarSection(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri || !pBri->IsWingWall(bStt, bLeft)) return;

	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	CTwinVectorArray tvArrGen;
	CTwinVector tvOut, tvHuPrev, tvHuNext;
	pBri->GetTvWingWallSection(bStt, bLeft, tvArrGen);

	tvArrGen.GetTvByInfo(_T("날개벽단면외측"), tvOut);
	tvArrGen.GetTvByInfo(_T("날개벽단면내측헌치전"), tvHuPrev);
	tvArrGen.GetTvByInfo(_T("날개벽단면내측헌치후"), tvHuNext);

	BOOL bCrossVar = pWing->m_nDirHunch==2 && nOut==0;
	double dL2 = pWing->m_dL2;
	double lr = bLeft ? -1.0 : 1.0;
	double io = nOut ? 1.0 : -1.0;
	double dCoverWing = nOut==0 ? pBri->m_dCoverWingIn[0] : pBri->m_dCoverWingOut[0];
	CDPoint xyMark(0, 0);

	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CTwinVector tv;
	CTwinVectorArray tvArrMain, tvArrSupp;

	long nMaxDan = nOut ? pBri->GetMaxWingWallDan(bStt, bLeft, 1) : pBri->GetMaxWingWallDan(bStt, bLeft, 0);
	for(long nDan = 0; nDan < nMaxDan; nDan++)
	{
		int nDanIdx = (nDan == 0) ? 0 : WW_REBAR_COUNT;
		// A or D구간
		BOOL bMerged = pBri->IsMergedRebarWingWall(bStt, bLeft, nOut, nDan, 'A', 'D');
		if(bMerged || dL2==0)
			pBri->GetTvArrayRebarWingWallBlockDMainFront(tvArrMain, bStt, bLeft, nOut, nDan);
		else
			pBri->GetTvArrayRebarWingWallBlockAMainFront(tvArrMain, bStt, bLeft, nOut, nDan);
		if(dL2==0)
			pBri->GetTvArrayRebarWingWallBlockDSupportFront(tvArrSupp, bStt, bLeft, nOut, nDan);
		else
			pBri->GetTvArrayRebarWingWallBlockASupportFront(tvArrSupp, bStt, bLeft, nOut, nDan);
		int i=0; for(i=0; i<tvArrSupp.GetSize(); i++)
		{
			if(tvArrSupp.GetAt(i).m_sInfo.Left(1)==_T("*"))
			{
				tvArrSupp.RemoveAt(i);
				i--;
			}
		}
		int nSizeMain = tvArrMain.GetSize();
		int nSizeSupp = tvArrSupp.GetSize();
		double dCTC = nOut==0 ? pWing->m_dMainCTCA/2 : pWing->m_dMainCTCA;

		vector<double> y_slope, y_hor, y_var, y_var_sec, y_var_under;
		for(i=0; i<nSizeMain; i++)
		{
			tv = tvArrMain.GetAt(i);
			if(tv.m_sInfo==_T("상단경사A") || tv.m_sInfo==_T("상단경사D"))
				y_slope.push_back(tv.m_v1.y);
			if(tv.m_sInfo==_T("수평A") || tv.m_sInfo==_T("수평D"))
				y_hor.push_back(tv.m_v1.y);
			if(tv.m_sInfo==_T("수평변화D"))
				y_var.push_back(tv.m_v1.y);
			if(tv.m_sInfo==_T("수평변단면D"))
				y_var_sec.push_back(tv.m_v1.y);
			if(tv.m_sInfo==_T("수평변단면하단D"))
				y_var_under.push_back(tv.m_v1.y);
		}
		
		if(nSizeMain>0)
		{
			//.. 상단경사A or D
			int nRBIndex = nOut==0 ? WW_REBAR_SLOPE_A_IN : WW_REBAR_SLOPE_A_OUT;
			if(bMerged || dL2==0)
				nRBIndex = nOut==0 ? WW_REBAR_SLOPE_D_IN : WW_REBAR_SLOPE_D_OUT;
			CRebar* pRB = pWing->m_pArrRebar[nRBIndex + nDanIdx];
			if(pRB->IsTrue() && !y_slope.empty())
			{
				xyMark.x = nOut==0 ? tvHuPrev.m_v1.x+(dCoverWing+REBAR_RADIUS)*lr : tvOut.m_v1.x-(dCoverWing+REBAR_RADIUS)*lr;
				xyMark.y = y_slope.back() - REBAR_RADIUS;
				if(nDan == 1)
				{
					xyMark.x -= 100 * lr * io;
					xyMark.y = y_slope.back() - REBAR_RADIUS;
				}
				// Sym.RebarMarkLineUpper(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), nOut==0 ? !bLeft : bLeft, 0, FALSE);

				SRebarMarkInputData mark_input_data;
				SRebarMarkviewData mark_view_data;
				mark_input_data.mark = pRB->m_sMark;
				mark_input_data.dia = pRB->GetStringDia();
				mark_input_data.position = xyMark;
				mark_input_data.ctc = (nOut == 0) ? pWing->m_dMainCTCA / 2 : pWing->m_dMainCTCA;
				mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
				mark_input_data.left_dir = nOut==0 ? !bLeft : bLeft;
				mark_input_data.height = 0;

				mark_view_data.count_mark = FALSE;
				mark_view_data.mark_pos = eMarkUpper;

				Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
			}
			if(dL2==0 && bCrossVar)
			{
				//.. 수평변단면 D
				nRBIndex = WW_REBAR_HOR_D_IN_VAR_SEC;

				pRB = pWing->m_pArrRebar[nRBIndex + nDanIdx];
				if(pRB->IsTrue() && !y_var_sec.empty())
				{
					double dSttX = bMerged || dL2==0 ? tvHuPrev.m_v1.x : tvHuNext.m_v1.x;
					xyMark.x = nOut==0 ? dSttX+(dCoverWing+REBAR_RADIUS)*lr : tvOut.m_v1.x-(dCoverWing+REBAR_RADIUS)*lr;
					xyMark.y = y_var_sec.size()>2 ? y_var_sec.at(y_var_sec.size()/2) : y_var_sec.at(0);
					if(nDan == 1)
					{ 
						xyMark.x -= 100 * lr * io;;
						xyMark.y = y_var_sec.size()>2 ? y_var_sec.at(y_var_sec.size()/2 + 1) : y_var_sec.at(0);
					}
					// Sym.RebarMarkPointSide(&Dom, xyMark, dCTC, dCTC, pRB->m_sMark, pRB->GetStringDia(), nOut==0 ? !bLeft : bLeft, 0, CDPoint(0, 0), FALSE);

					SRebarMarkInputData mark_input_data;
					SRebarMarkviewData mark_view_data;
					mark_input_data.mark = pRB->m_sMark;
					mark_input_data.dia = pRB->GetStringDia();
					mark_input_data.position = xyMark;
					mark_input_data.ctc = dCTC;
					mark_input_data.dist_left_mark = dCTC;
					mark_input_data.dist_right_mark = dCTC;
					mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
					mark_input_data.left_dir = nOut==0 ? !bLeft : bLeft;
					mark_input_data.height = 0;

					mark_view_data.count_mark = FALSE;
					mark_view_data.line_rebar = FALSE;
					mark_view_data.mark_pos = eMarkSide;

					Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
				}
				//.. 수평변화D 변단면 하부
				nRBIndex = WW_REBAR_HOR_D_IN_VAR_UNDER;
				pRB = pWing->m_pArrRebar[nRBIndex + nDanIdx];
				if(pRB->IsTrue() && !y_var_under.empty())
				{
					xyMark.x = nOut==0 ? tvHuNext.m_v1.x+(dCoverWing+REBAR_RADIUS)*lr : tvOut.m_v1.x-(dCoverWing+REBAR_RADIUS)*lr;
					xyMark.y = y_var_under.size()>1 ? y_var_under.at(1) : y_var_under.at(0);
					if(nDan == 1)
					{
						 xyMark.x -= 100 * lr * io;
						 xyMark.y = y_var_under.size()>2 ? y_var_under.at(2) : y_var_under.at(0);
					}
					// Sym.RebarMarkPointSide(&Dom, xyMark, dCTC, dCTC, pRB->m_sMark, pRB->GetStringDia(), nOut==0 ? !bLeft : bLeft, 0, CDPoint(0, 0), FALSE);

					SRebarMarkInputData mark_input_data;
					SRebarMarkviewData mark_view_data;
					mark_input_data.mark = pRB->m_sMark;
					mark_input_data.dia = pRB->GetStringDia();
					mark_input_data.position = xyMark;
					mark_input_data.ctc = dCTC;
					mark_input_data.dist_left_mark = dCTC;
					mark_input_data.dist_right_mark = dCTC;
					mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
					mark_input_data.left_dir = nOut==0 ? !bLeft : bLeft;
					mark_input_data.height = 0;

					mark_view_data.count_mark = FALSE;
					mark_view_data.line_rebar = FALSE;
					mark_view_data.mark_pos = eMarkSide;

					Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
				}
			}
			else
			{
				//.. 수평A or D
				nRBIndex = nOut==0 ? WW_REBAR_HOR_A_IN : WW_REBAR_HOR_A_OUT;
				if(bMerged || dL2==0)
					nRBIndex = nOut==0 ? WW_REBAR_HOR_D_IN : WW_REBAR_HOR_D_OUT;
				pRB = pWing->m_pArrRebar[nRBIndex + nDanIdx];
				if(pRB->IsTrue() && !y_hor.empty())
				{
					double dSttX = bMerged || dL2==0 ? tvHuPrev.m_v1.x : tvHuNext.m_v1.x;
					xyMark.x = nOut==0 ? dSttX+(dCoverWing+REBAR_RADIUS)*lr : tvOut.m_v1.x-(dCoverWing+REBAR_RADIUS)*lr;
					xyMark.y = y_hor.size()>1 ? y_hor.at(1) : y_hor.at(0);
					if(nDan == 1)
					{
						xyMark.x -= 100 * lr * io;
						xyMark.y = y_hor.size()>2 ? y_hor.at(2) : y_hor.at(0);
					}
					// Sym.RebarMarkPointSide(&Dom, xyMark, dCTC, dCTC, pRB->m_sMark, pRB->GetStringDia(), nOut==0 ? !bLeft : bLeft, 0, CDPoint(0, 0), FALSE);

					SRebarMarkInputData mark_input_data;
					SRebarMarkviewData mark_view_data;
					mark_input_data.mark = pRB->m_sMark;
					mark_input_data.dia = pRB->GetStringDia();
					mark_input_data.position = xyMark;
					mark_input_data.ctc = dCTC;
										
					if(nOut == iFRONT)
					{
						if(pRB->m_nEa > 1)		mark_input_data.dist_left_mark = dCTC;
						if(pRB->m_nEa > 2)		mark_input_data.dist_right_mark = dCTC;
					}
					else
					{
						if(pRB->m_nEa > 2)		mark_input_data.dist_left_mark = dCTC;
						if(pRB->m_nEa > 1)		mark_input_data.dist_right_mark = dCTC;
					}
					
					mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
					mark_input_data.left_dir = nOut==0 ? !bLeft : bLeft;
					mark_input_data.height = 0;

					mark_view_data.count_mark = FALSE;
					mark_view_data.line_rebar = FALSE;
					mark_view_data.mark_pos = eMarkSide;

					Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
				}
				//.. 수평변화D
				nRBIndex = nOut==0 ? WW_REBAR_HOR_D_IN_VAR : WW_REBAR_HOR_D_OUT_VAR;
				pRB = pWing->m_pArrRebar[nRBIndex + nDanIdx];
				if(pRB->IsTrue() && !y_var.empty())
				{
					xyMark.y = y_var.size()>1 ? y_var.at(1) : y_var.at(0);

					if(nOut == 0)
					{
						double dSttX = (xyMark.y > tvHuPrev.m_v2.y)? tvHuPrev.m_v1.x : tvHuNext.m_v1.x;
						xyMark.x = dSttX + (dCoverWing+REBAR_RADIUS)*lr;
					}
					else
						xyMark.x = tvOut.m_v1.x-(dCoverWing+REBAR_RADIUS)*lr;

					if(nDan == 1) 
					{
						xyMark.x -= 100 * lr * io;;
						xyMark.y = y_var.size()>2 ? y_var.at(2) : y_var.at(0);
					}
					// Sym.RebarMarkPointSide(&Dom, xyMark, dCTC, dCTC, pRB->m_sMark, pRB->GetStringDia(), nOut==0 ? !bLeft : bLeft, 0, CDPoint(0, 0), FALSE);

					SRebarMarkInputData mark_input_data;
					SRebarMarkviewData mark_view_data;
					mark_input_data.mark = pRB->m_sMark;
					mark_input_data.dia = pRB->GetStringDia();
					mark_input_data.position = xyMark;
					mark_input_data.ctc = dCTC;
					mark_input_data.dist_left_mark = dCTC;
					mark_input_data.dist_right_mark = dCTC;
					mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
					mark_input_data.left_dir = nOut==0 ? !bLeft : bLeft;
					mark_input_data.height = 0;

					mark_view_data.count_mark = FALSE;
					mark_view_data.line_rebar = FALSE;
					mark_view_data.mark_pos = eMarkSide;

					Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
				}
			}
		}

		if(nSizeSupp>0)
		{
			//.. 수직A or D
			int nRBIndex = nOut==0 ? WW_REBAR_VER_A_IN : WW_REBAR_VER_A_OUT;
			if(dL2==0)
				nRBIndex = nOut==0 ? WW_REBAR_VER_D_IN : WW_REBAR_VER_D_OUT;
			CRebar* pRB = pWing->m_pArrRebar[nRBIndex + nDanIdx];
			if(pRB->IsTrue())
			{
				xyMark.x = nOut==0 ? tvHuPrev.m_v1.x+dCoverWing*lr : tvOut.m_v1.x-dCoverWing*lr;
				xyMark.y = pBri->m_Util.GetPosRebarMark(tvArrMain, TRUE, y_var.size()+y_hor.size()-1);
				if(nDan == 1) 
				{
					xyMark.x -= 100 * lr * io;
					xyMark.y = pBri->m_Util.GetPosRebarMark(tvArrMain, TRUE, y_var.size()+y_hor.size());
				}
				// Sym.RebarMarkLineSide(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), nOut==0 ? !bLeft : bLeft);
				CDPoint vTick = nOut==0 ? CDPoint(0, 1) : CDPoint(1, 0);
				Sym.RebarCountMarkOne(&Dom, xyMark, vTick);

				SRebarMarkInputData mark_input_data;
				SRebarMarkviewData mark_view_data;
				mark_input_data.mark = pRB->m_sMark;
				mark_input_data.dia = pRB->GetStringDia();
				mark_input_data.position = xyMark;
				mark_input_data.ctc = ((nOut==0 && dL2 != 0) || (nOut==0 && dL2 == 0 && !pWing->m_bSelMainCTCD)) ? pWing->m_dStdVerCTC/2 : pWing->m_dStdVerCTC;
				mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
				mark_input_data.left_dir = nOut==0 ? !bLeft : bLeft;
				mark_input_data.height = 0;

				mark_view_data.count_mark = FALSE;
				mark_view_data.mark_pos = eMarkSide;

				Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
			}

			//.. 변단면수직A or D
			if(nOut==0)
			{
				nRBIndex = WW_REBAR_VER_A_IN_VAR_SEC;
				if(dL2==0)
					nRBIndex = WW_REBAR_VER_D_IN_VAR_SEC;
				pRB = pWing->m_pArrRebar[nRBIndex + nDanIdx];
				if(pRB->IsTrue())
				{
					xyMark.y = pBri->m_Util.GetPosRebarMark(tvArrMain, TRUE, bMerged ? y_var.size()-1 : y_hor.size()/2);
					double dSttX = (xyMark.y > tvHuPrev.m_v2.y)? tvHuPrev.m_v1.x : tvHuNext.m_v1.x;
					xyMark.x = dSttX+dCoverWing*lr;
					
					if(nDan == 1) 
					{
						xyMark.x -= 100 * lr * io;
						xyMark.y = pBri->m_Util.GetPosRebarMark(tvArrMain, TRUE, bMerged ? y_var.size()-1 : y_hor.size()/2+1);
					}
					// Sym.RebarMarkLineSide(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), !bLeft);

					SRebarMarkInputData mark_input_data;
					SRebarMarkviewData mark_view_data;
					mark_input_data.mark = pRB->m_sMark;
					mark_input_data.dia = pRB->GetStringDia();
					mark_input_data.position = xyMark;
					mark_input_data.ctc = ((nOut==0 && dL2 != 0) || (nOut==0 && dL2 == 0 && !pWing->m_bSelMainCTCD)) ? pWing->m_dStdVerCTC/2 : pWing->m_dStdVerCTC;
					mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
					mark_input_data.left_dir = nOut==0 ? !bLeft : bLeft;
					mark_input_data.height = 0;

					mark_view_data.count_mark = FALSE;
					mark_view_data.mark_pos = eMarkSide;

					Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
				}
			}
		}


		// B구간
		pBri->GetTvArrayRebarWingWallBlockBMainFront(tvArrMain, bStt, bLeft, nOut, nDan);
		nSizeMain = tvArrMain.GetSize();
		dCTC = nOut==0 ? pWing->m_dMainCTCB/2 : pWing->m_dMainCTCB;

		y_hor.clear();
		y_var.clear();
		for(i=0; i<nSizeMain; i++)
		{
			tv = tvArrMain.GetAt(i);
			if(tv.m_sInfo==_T("수평변화B"))
				y_var.push_back(tv.m_v1.y);
			if(tv.m_sInfo==_T("수평B"))
				y_hor.push_back(tv.m_v1.y);
		}

		if(nSizeMain>0)
		{
			//.. 수평변화B
			int nRBIndex = nOut==0 ? WW_REBAR_HOR_B_IN_VAR : WW_REBAR_HOR_B_OUT_VAR;
			CRebar* pRB = pWing->m_pArrRebar[nRBIndex + nDanIdx];
			if(pRB->IsTrue() && !y_var.empty())
			{
				xyMark.x = nOut==0 ? tvHuNext.m_v1.x+(dCoverWing+REBAR_RADIUS)*lr : tvOut.m_v1.x-(dCoverWing+REBAR_RADIUS)*lr;
				xyMark.y = y_var.size()>1 ? y_var.at(1) : y_var.at(0);
				if(nDan == 1) 
				{
					xyMark.x -= 100 * lr * io;
					xyMark.y = y_var.size()>2 ? y_var.at(2) : y_var.at(0);
				}
				// Sym.RebarMarkPointSide(&Dom, xyMark, dCTC, dCTC, pRB->m_sMark, pRB->GetStringDia(), nOut==0 ? !bLeft : bLeft, 0, CDPoint(0, 0), FALSE);

				SRebarMarkInputData mark_input_data;
				SRebarMarkviewData mark_view_data;
				mark_input_data.mark = pRB->m_sMark;
				mark_input_data.dia = pRB->GetStringDia();
				mark_input_data.position = xyMark;
				mark_input_data.ctc = dCTC;
				mark_input_data.dist_left_mark = dCTC;
				mark_input_data.dist_right_mark = dCTC;
				mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
				mark_input_data.left_dir = nOut==0 ? !bLeft : bLeft;
				mark_input_data.height = 0;

				mark_view_data.count_mark = FALSE;
				mark_view_data.line_rebar = FALSE;
				mark_view_data.mark_pos = eMarkSide;

				Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
			}
			//.. 수평B
			nRBIndex = nOut==0 ? WW_REBAR_HOR_B_IN : WW_REBAR_HOR_B_OUT;
			pRB = pWing->m_pArrRebar[nRBIndex + nDanIdx];
			if(pRB->IsTrue() && !y_hor.empty())
			{
				xyMark.x = nOut==0 ? tvHuNext.m_v1.x+(dCoverWing+REBAR_RADIUS)*lr : tvOut.m_v1.x-(dCoverWing+REBAR_RADIUS)*lr;
				xyMark.y = y_hor.size()>1 ? y_hor.at(1) : y_hor.at(0);
				if(nDan == 1)
				{ 
					xyMark.x -= 100 * lr * io;
					xyMark.y = y_hor.size()>2 ? y_hor.at(2) : y_hor.at(0);
				}
				// Sym.RebarMarkPointSide(&Dom, xyMark, dCTC, dCTC, pRB->m_sMark, pRB->GetStringDia(), nOut==0 ? !bLeft : bLeft, 0, CDPoint(0, 0), FALSE);

				SRebarMarkInputData mark_input_data;
				SRebarMarkviewData mark_view_data;
				mark_input_data.mark = pRB->m_sMark;
				mark_input_data.dia = pRB->GetStringDia();
				mark_input_data.position = xyMark;
				mark_input_data.ctc = dCTC;
				mark_input_data.dist_left_mark = dCTC;
				mark_input_data.dist_right_mark = dCTC;
				mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
				mark_input_data.left_dir = nOut==0 ? !bLeft : bLeft;
				mark_input_data.height = 0;

				mark_view_data.count_mark = FALSE;
				mark_view_data.line_rebar = FALSE;
				mark_view_data.mark_pos = eMarkSide;

				Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
			}
		}

		// C구간
		int nRBIndex = nOut==0 ? WW_REBAR_VER_C_IN : WW_REBAR_VER_C_OUT;
		CRebar* pRB = pWing->m_pArrRebar[nRBIndex + nDanIdx];
		if(pRB->IsTrue() && tvArrMain.GetSize() > 0)
		{
			xyMark.x = nOut==0 ? tvHuNext.m_v1.x+dCoverWing*lr : tvOut.m_v1.x-dCoverWing*lr;
			xyMark.y = pBri->m_Util.GetPosRebarMark(tvArrMain, TRUE, 7);
			if(nDan == 1) 
			{
				xyMark.x -= 100 * lr * io;
				xyMark.y = pBri->m_Util.GetPosRebarMark(tvArrMain, TRUE, 5);
			}
			// Sym.RebarMarkLineSide(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), nOut==0 ? !bLeft : bLeft);
			CDPoint vTick = nOut==0 ? CDPoint(0, 1) : CDPoint(1, 0);
			Sym.RebarCountMarkOne(&Dom, xyMark, vTick);

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = pRB->m_sMark;
			mark_input_data.dia = pRB->GetStringDia();
			mark_input_data.position = xyMark;
			mark_input_data.ctc = (nOut==0) ? pWing->m_dStdVerCTC/2 : pWing->m_dStdVerCTC;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.left_dir = nOut==0 ? !bLeft : bLeft;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.mark_pos = eMarkSide;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}

		if(nOut==1)
		{
			// 전단 철근
			double dT1 = pWing->m_dT1;
			double dT2 = pWing->m_dT2;
			vector<double> y_upper, y_lower;
			CTwinVectorArray tvArrShearUpper, tvArrShearLower;
			CDPoint xyB(0, 0), xyE(0, 0);

			if(dL2==0)
				pBri->GetTvArrayRebarWingWallShearDFront(tvArrShearUpper, bStt, bLeft, 0);
			else
			{
				pBri->GetTvArrayRebarWingWallShearAFront(tvArrShearUpper, bStt, bLeft, 0);
				pBri->GetTvArrayRebarWingWallShearBCFront(tvArrShearLower, bStt, bLeft, 0);
			}
			int nSizeUpper = tvArrShearUpper.GetSize();
			int nSizeLower = tvArrShearLower.GetSize();
			xyMark.x = tvOut.m_v1.x - (pWing->m_nDirHunch==2 ? dT2 : dT1)/2*lr;

			if(nSizeLower>0)
			{
				double dSttY = 0;
				for(i=0; i<nSizeLower; i++)
				{
					xyMark.y = tvArrShearLower.GetAt(i).m_v1.y;
					if(xyMark.y!=dSttY)
						dSttY = xyMark.y;
					else continue;
					Sym.RebarCountMarkOne(&Dom, xyMark);
					y_lower.push_back(xyMark.y);
				}

				std::sort(y_upper.begin(), y_upper.end());
				std::sort(y_lower.begin(), y_lower.end());

				int nRBIndex = (pWing->m_nTypeShearBC != 1)? WW_REBAR_SHEAR_B : WW_REBAR_SHEAR_C;
				CRebar* pRB = pWing->m_pArrRebar[nRBIndex + nDanIdx];
				if(pRB->IsTrue())
				{
					Dom.LineTo(xyMark.x, y_lower.front(), xyMark.x, y_lower.back());
					double dY = y_lower.size() == 1 ? y_lower.at(0) : pBri->m_Util.GetPosRebarMark(y_lower, y_lower.size()/2);
					xyB = CDPoint(xyMark.x, dY);
					xyE = CDPoint(tvOut.m_v1.x-dCoverWing*lr, dY);
					Dom.LineTo(xyB, xyE);
					// Sym.RebarMarkLineSide(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), bLeft, 0, FALSE);

					SRebarMarkInputData mark_input_data;
					SRebarMarkviewData mark_view_data;
					mark_input_data.mark = pRB->m_sMark;
					mark_input_data.dia = pRB->GetStringDia();
					mark_input_data.position = xyE;
					mark_input_data.ctc = (pWing->m_nTypeShearBC != 1)? pWing->m_dSpaceShearB : pWing->m_dSpaceShearC;
					mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
					mark_input_data.left_dir = bLeft;
					mark_input_data.height = 0;

					mark_view_data.count_mark = FALSE;
					mark_view_data.mark_pos = eMarkSide;

					Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
				}
			}

			if(nSizeUpper>0)
			{
				double dSttY = 0;
				for(i=0; i<nSizeUpper; i++)
				{
					xyMark.y = tvArrShearUpper.GetAt(i).m_v1.y;
					if(xyMark.y!=dSttY)
						dSttY = xyMark.y;
					else continue;
					Sym.RebarCountMarkOne(&Dom, xyMark);
					y_upper.push_back(xyMark.y);
				}
				// 가장 위쪽의 전단 철근 표현
				if(IsNeedTopRebarShearSection(pWing))
				{
					xyMark.y = tvArrShearUpper.GetAt(nSizeUpper-1).m_v2.y;
					Sym.RebarCountMarkOne(&Dom, xyMark);
					y_upper.push_back(xyMark.y);
				}

				int nRBIndex = dL2==0 ? WW_REBAR_SHEAR_D : WW_REBAR_SHEAR_A;
				CRebar* pRB = pWing->m_pArrRebar[nRBIndex + nDanIdx];
				if(pRB->IsTrue() && y_upper.size() > 0)
				{
					Dom.LineTo(xyMark.x, y_upper.front(), xyMark.x, y_upper.back());
					double dY = pBri->m_Util.GetPosRebarMark(y_upper, y_upper.size() == 1? 1 : y_upper.size()/2);
					xyB = CDPoint(xyMark.x, dY);
					xyE = CDPoint(tvOut.m_v1.x-dCoverWing*lr, dY);
					Dom.LineTo(xyB, xyE);
					// Sym.RebarMarkLineSide(&Dom, xyE, pRB->m_sMark, pRB->GetStringDia(), bLeft, 0, FALSE);

					SRebarMarkInputData mark_input_data;
					SRebarMarkviewData mark_view_data;
					mark_input_data.mark = pRB->m_sMark;
					mark_input_data.dia = pRB->GetStringDia();
					mark_input_data.position = xyE;
					mark_input_data.ctc = (dL2==0)? pWing->m_dSpaceShearD : pWing->m_dSpaceShearA;
					mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
					mark_input_data.left_dir = bLeft;
					mark_input_data.height = 0;

					mark_view_data.count_mark = FALSE;
					mark_view_data.mark_pos = eMarkSide;

					Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
				}
			}

			// 기초헌치
			pBri->GetTvArrayRebarWingWallHunchFootingSect(tvArrMain, bStt, bLeft);
			nRBIndex = WW_REBAR_HUNCH_FOOT;
			pRB = pWing->m_pArrRebar[nRBIndex + nDanIdx];
			if(pRB->IsTrue())
			{
				tv = tvArrMain.GetAt(1);
				xyMark = tv.GetXyMid();
				double dMarkLen = xyMark.y - tvOut.m_v2.y + MARK_DIST*1.5;
				CDPoint vAngMark = bLeft ? CDPoint(0, 1) : CDPoint(1, 0);
				Sym.RebarCountMarkOne(&Dom, xyMark, vAngMark);
				// Sym.RebarMarkLineLower(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), nOut==0 ? !bLeft : bLeft, dMarkLen, FALSE);

				SRebarMarkInputData mark_input_data;
				SRebarMarkviewData mark_view_data;
				mark_input_data.mark = pRB->m_sMark;
				mark_input_data.dia = pRB->GetStringDia();
				mark_input_data.position = xyMark;
				mark_input_data.ctc = (pWing->m_nHunchCTC == 0)? pWing->m_dStdVerCTC/2 : pWing->m_dStdVerCTC;
				mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
				mark_input_data.left_dir = nOut==0 ? !bLeft : bLeft;
				mark_input_data.height = dMarkLen;

				mark_view_data.count_mark = FALSE;
				mark_view_data.mark_pos = eMarkLower;

				Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
			}
		}
	}
	

	*pDomP << Dom;
}

void CDrawWingWall::MarkRebarPlaneUpper(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	double se = bStt ? -1.0 : 1.0;
	double lr = bLeft ? -1.0 : 1.0;
	double io = nOut==0 ? -1.0 : 1.0;

	CTwinVectorArray tvArr;
	CTwinVector tv, tvEnd, tvOut;

	pBri->GetTvWingWallPlane(bStt, TRUE, TRUE, TRUE);
	tvArr = pBri->m_tvArrPlaneWingWall;

	tvArr.GetTvByInfo(_T("슬래브끝단외측"), tv);
	CDPoint vX(1, 0), vY(0, 1);
	CDPoint xyOrigin = bLeft ? tv.m_v1 : tv.m_v2;
	CDPoint vAngWing = pBri->GetAngleAzimuthWingWall(bStt, bLeft);
	CDPoint vRotate = vAngWing.BetweenAngle(vX*se);
	tvArr.Rotate(xyOrigin, vRotate);
	tvArr.Move(-xyOrigin);

	CString strDir = bLeft ? _T("좌측") : _T("우측");
	tvArr.GetTvByInfo(strDir+_T("날개벽외측"), tvOut);
	tvArr.GetTvByInfo(strDir+_T("날개벽끝단"), tvEnd);

	CDPoint vAng = pBri->GetAngleWingWallBetweenWall(bStt, bLeft);
	double dCoverWing = nOut==0 ? pBri->m_dCoverWingIn[0] : pBri->m_dCoverWingOut[0];
	double dCoverEnd  = pBri->m_dCoverWingEnd;
	double dT1 = pWing->m_dT1;
	double dT2 = pWing->m_dT2;
	double dCTC = nOut==0 ? pWing->m_dStdVerCTC/2 : pWing->m_dStdVerCTC;

	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML); 

	CDPoint xyMark(0, 0);
	CDPoint xy1(0, 0), xy2(0, 0);
	CDoubleArray dArrShear;
	CDPoint xyShearStt = tvOut.m_v1;
	CDPoint vEnd(0, 0);
	CDPoint vOut = tvOut.GetVDir();
	int nEaD = 0;
	if(pWing->m_nAttachPos == 0)
	{
		vEnd = pWing->m_bRAngEnd ? vY : vAng;
	}
	else
	{
		vEnd = pWing->m_bRAngEnd ? vY : tvEnd.GetXyDir();
		vEnd = vEnd.GetMirrorHorz();
	}

	CDPoint xyOutStt = tvEnd.m_v1 ;
	CDPoint xyInStt = tvEnd.m_v2;
	CDPoint xyStt = nOut==0 ? xyInStt : xyOutStt;
	xyStt.x -= dCoverEnd/vEnd.y*se - (dCoverWing+REBAR_RADIUS)/tan(vEnd.GetAngleRadian())*se*io;

	int nDan = 0;
	CDPointArray xyArr;
	pBri->GetDimArrayRebarWingWallUpper(xyArr, bStt, bLeft, nOut, nDan);

	// D구간
	//.. 수직 철근 폐합
	int nRBIndex = nOut==0 ? WW_REBAR_VER_D_IN : WW_REBAR_VER_D_OUT;
	CRebar* pRB = pWing->m_pArrRebar[nRBIndex];
	if(pRB->IsTrue())
	{
		nEaD = pRB->m_nEa;
		double dX = pBri->m_Util.GetPosRebarMark(xyArr, 4, FALSE);

		xyMark = xyStt - dX*vX*se + (dCoverWing+REBAR_RADIUS)*vY*io*lr*se;

		SRebarMarkInputData mark_input_data;
		SRebarMarkviewData mark_view_data;
		mark_input_data.mark = pRB->m_sMark;
		mark_input_data.dia = pRB->GetStringDia();
		mark_input_data.position = xyMark;
		mark_input_data.ctc = dCTC;
		mark_input_data.dist_left_mark = dCTC;
		mark_input_data.dist_right_mark = dCTC;
		mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
		mark_input_data.left_dir = bStt;
		mark_input_data.height = 0;

		mark_view_data.count_mark = FALSE;
		mark_view_data.line_rebar = FALSE;
		mark_view_data.mark_pos = (io*lr*se>0)? eMarkLower : eMarkUpper;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}
	//.. 수직철근 연결
	if(nOut == 0)
	{
		int nRBIndex = WW_REBAR_VER_D_IN_VAR_UP;
		CRebar* pRB = pWing->m_pArrRebar[nRBIndex];
		if(pRB->IsTrue())
		{
			long nEaD2 = pRB->m_nEa;
			nEaD += nEaD2;
			double dX = pBri->m_Util.GetPosRebarMark(xyArr, nEaD-4, FALSE);
			
			xyMark = xyStt - dX*vX*se + (dCoverWing+REBAR_RADIUS)*vY*io*lr*se;

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = pRB->m_sMark;
			mark_input_data.dia = pRB->GetStringDia();
			mark_input_data.position = xyMark;
			mark_input_data.ctc = dCTC;
			mark_input_data.dist_left_mark = dCTC;
			mark_input_data.dist_right_mark = dCTC;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.left_dir = bStt;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.line_rebar = FALSE;
			mark_view_data.mark_pos = (io*lr*se>0)? eMarkLower : eMarkUpper;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}
	}
	
	//.. 수평 철근
	nRBIndex = nOut==0 ? WW_REBAR_HOR_D_IN : WW_REBAR_HOR_D_OUT;
	pRB = pWing->m_pArrRebar[nRBIndex];
	if(pRB->IsTrue())
	{
		double dX = pBri->m_Util.GetPosRebarMark(xyArr, nEaD/2, TRUE);
		xyMark = xyStt - dX*vX*se + dCoverWing*vY*io*lr*se;
		CDPoint vTick = nOut==0 ? CDPoint(0, 1) : CDPoint(1, 0);
		Sym.RebarCountMarkOne(&Dom, xyMark, vTick);

		SRebarMarkInputData mark_input_data;
		SRebarMarkviewData mark_view_data;
		mark_input_data.mark = pRB->m_sMark;
		mark_input_data.dia = pRB->GetStringDia();
		mark_input_data.position = xyMark;
		mark_input_data.ctc = (nOut==0 && !pWing->m_bSelMainCTCD)? pWing->m_dMainCTCA/2 : pWing->m_dMainCTCA;
		mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
		mark_input_data.left_dir = bStt;
		mark_input_data.height = 0;

		mark_view_data.count_mark = FALSE;
		mark_view_data.mark_pos = (io*lr*se>0)? eMarkLower : eMarkUpper;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}

	// A구간
	if(pWing->m_dL2>0)
	{
		//.. 수직 철근
		BOOL bVar = pBri->IsWingWallVariableSect(bStt, bLeft);
		BOOL bMergedAC = pBri->IsMergedRebarWingWall(bStt, bLeft, nOut, nDan, 'A', 'C');	// C구간 주철근과 병합되는지..

		nRBIndex = nOut==0 ? WW_REBAR_VER_A_IN : WW_REBAR_VER_A_OUT;
		if(((nOut==0 && !bVar) || nOut==1) && bMergedAC)
			nRBIndex = nOut==0 ? WW_REBAR_VER_C_IN : WW_REBAR_VER_C_OUT;
		pRB = pWing->m_pArrRebar[nRBIndex];
		
		int nEaA = 0;
		if(pRB->IsTrue())
		{
			nEaA = pRB->m_nEa;
			double dX = pBri->m_Util.GetPosRebarMark(xyArr, nEaD+3, FALSE);
			if(nOut==0 && pWing->m_nDirHunch!=1)
				xyStt.y = xyInStt.y+ (dT2-dT1)*lr*se;
			xyMark = xyStt - dX*vX*se + (dCoverWing+REBAR_RADIUS)*vY*io*lr*se;

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = pRB->m_sMark;
			mark_input_data.dia = pRB->GetStringDia();
			mark_input_data.position = xyMark;
			mark_input_data.ctc = dCTC;
			mark_input_data.dist_left_mark = dCTC;
			mark_input_data.dist_right_mark = dCTC;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.left_dir = !bStt;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.line_rebar = FALSE;
			mark_view_data.mark_pos = (io*lr*se>0)? eMarkLower : eMarkUpper;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}

		//.. 수평 철근
		nRBIndex = nOut==0 ? WW_REBAR_HOR_A_IN : WW_REBAR_HOR_A_OUT;
		pRB = pWing->m_pArrRebar[nRBIndex];
		if(pRB->IsTrue())
		{
			double dX = pBri->m_Util.GetPosRebarMark(xyArr, nEaD+nEaA/2, TRUE);
			xyMark = xyStt - dX*vX*se + dCoverWing*vY*io*lr*se;

			CDPoint vTick = nOut==0 ? CDPoint(0, 1) : CDPoint(1, 0);
			Sym.RebarCountMarkOne(&Dom, xyMark, vTick);

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = pRB->m_sMark;
			mark_input_data.dia = pRB->GetStringDia();
			mark_input_data.position = xyMark;
			mark_input_data.ctc = (nOut==0)? pWing->m_dMainCTCA/2 : pWing->m_dMainCTCA;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.left_dir = !bStt;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.mark_pos = (io*lr*se>0)? eMarkLower : eMarkUpper;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}
	}

	

	if(nOut==1)
	{
		//.. 전단 철근
		nRBIndex = WW_REBAR_SHEAR_A;
		pRB = pWing->m_pArrRebar[nRBIndex];
		if(pRB->IsTrue())
		{
			pBri->GetLengthArrayShearFront(dArrShear, 'A', bStt, bLeft);
			int nSize = dArrShear.GetSize();
			GetXyMatchLineAndLine(tvOut.m_v1, vX, xyShearStt, vEnd, xyShearStt);

			vector<double> x_shear;
			int i=0; for(i=0; i<nSize; i++)
			{
				xyMark = xyShearStt + dArrShear.GetAt(i)*vX*se + dT1/2*vY*lr*se;
				xyMark.x += (dCoverWing+REBAR_RADIUS)/tan(vEnd.GetAngleRadian())*se;

				Sym.RebarCountMarkOne(&Dom, xyMark);
				x_shear.push_back(xyMark.x);
			}
			if(x_shear.size() > 0)
			{
				xy1 = CDPoint(x_shear.front(), xyMark.y);
				xy2 = CDPoint(x_shear.back(), xyMark.y);
				Dom.LineTo(xy1, xy2);
				xyMark.x = pBri->m_Util.GetPosRebarMark(x_shear, nSize/3);

				SRebarMarkInputData mark_input_data;
				SRebarMarkviewData mark_view_data;
				mark_input_data.mark = pRB->m_sMark;
				mark_input_data.dia = pRB->GetStringDia();
				mark_input_data.position = xyMark;
				mark_input_data.ctc = pWing->m_dSpaceShearA;
				mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
				mark_input_data.left_dir = !bStt;
				mark_input_data.height = 0;

				mark_view_data.count_mark = FALSE;
				mark_view_data.mark_pos = (io*lr*se>0)? eMarkLower : eMarkUpper;

				Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
			}
		}

		nRBIndex = WW_REBAR_SHEAR_D;
		pRB = pWing->m_pArrRebar[nRBIndex];
		pBri->GetLengthArrayShearFront(dArrShear, 'D', bStt, bLeft);
		int nSize = dArrShear.GetSize();
		if(pRB->IsTrue() && nSize>0)
		{
			GetXyMatchLineAndLine(tvOut.m_v1, vX, xyShearStt, vEnd, xyShearStt);
			vector<double> x_shear;
			int i=0; for(i=0; i<nSize; i++)
			{
				xyMark = xyShearStt + dArrShear.GetAt(i)*vX*se + pWing->m_dT1/2*vY*lr*se;
				xyMark.x += (dCoverWing+REBAR_RADIUS)/tan(vEnd.GetAngleRadian())*se;
				Sym.RebarCountMarkOne(&Dom, xyMark);
				x_shear.push_back(xyMark.x);
			}
			if(nSize == 1)
			{
				xyMark.x = pBri->m_Util.GetPosRebarMark(x_shear, 1);
			}
			else
			{
				xy1 = CDPoint(x_shear.front(), xyMark.y);
				xy2 = CDPoint(x_shear.back(), xyMark.y);
				Dom.LineTo(xy1, xy2);
				xyMark.x = pBri->m_Util.GetPosRebarMark(x_shear, nSize/3);
			}

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = pRB->m_sMark;
			mark_input_data.dia = pRB->GetStringDia();
			mark_input_data.position = xyMark;
			mark_input_data.ctc = pWing->m_dSpaceShearD;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.left_dir = !bStt;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.mark_pos = (io*lr*se>0)? eMarkLower : eMarkUpper;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}

		//.. 헌치 철근
		nRBIndex = WW_REBAR_HUNCH_WALL_UP;
		pRB = pWing->m_pArrRebar[nRBIndex];
		if(pRB->IsTrue())
		{
			CTwinVectorArray tvArrHu;
			pBri->GetTvArrayRebarWingWallHunchWallPlane(tvArrHu, bStt, bLeft, TRUE);
			tvArrHu.Rotate(xyOrigin, vRotate);
			tvArrHu.Move(-xyOrigin);
			xyMark = tvArrHu.GetAt(1).GetXyMid();

			CDPoint vTick = vAng.x>0 ? CDPoint(0, 1) : CDPoint(1, 0);
			Sym.RebarCountMarkOne(&Dom, xyMark, vTick);

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = pRB->m_sMark;
			mark_input_data.dia = pRB->GetStringDia();
			mark_input_data.position = xyMark;
			mark_input_data.ctc = (pWing->m_nHunchCTC == 0)? pWing->m_dMainCTCA/2 : pWing->m_dMainCTCA;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.left_dir = bStt;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.mark_pos = (io*lr*se>0)? eMarkLower : eMarkUpper;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}

		// 흉벽보강철근
		if(pWing->m_bAddRebarReinforce && pWing->m_nAttachPos==0)
		{
			nRBIndex = WW_REBAR_REINFORCE_A;
			pRB = pWing->m_pArrRebar[nRBIndex];
			if(pRB->IsTrue())
			{
				CTwinVector tvSlab;
				tvArr.GetTvByInfo(strDir+_T("날개벽외측"), tvOut);
				tvArr.GetTvByInfo(_T("슬래브끝단외측"), tvSlab);
				
				double dCoverWallOut = pBri->m_dCoverWallOut[0];
				double dCoverSlab = pBri->m_dCoverWallIn[0];
				double dTickWall = (bStt)? pBri->m_dWS : pBri->m_dWE;
				double dDistOffset = dTickWall - dCoverSlab - dCoverWallOut;
				Offset(tvOut.m_v1, tvOut.m_v2, -(pBri->m_dCoverWingOut[0])*lr);
				Offset(tvSlab.m_v1, tvSlab.m_v2, -dDistOffset);
				CDPoint vOut = tvOut.GetXyDir();
				CDPoint vSlab = tvSlab.GetXyDir();
				CDPoint vTick = vAng.x>0 ? CDPoint(0, 1) : CDPoint(1, 0);
				
				GetXyMatchLineAndLine(tvOut.m_v1, vOut, tvSlab.m_v1, vSlab, tv.m_v1);
				tv.m_v2 = tv.m_v1 - vSlab*3000*lr;
				xyMark = tv.GetXyMid();
				
				Sym.RebarCountMarkOne(&Dom, xyMark, vTick);
				// Sym.RebarMarkLineSide(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), se*lr>0, dTickWall/vSlab.y, FALSE);
				SRebarMarkInputData mark_input_data;
				SRebarMarkviewData mark_view_data;
				mark_input_data.mark = pRB->m_sMark;
				mark_input_data.dia = pRB->GetStringDia();
				mark_input_data.position = xyMark;
				mark_input_data.ctc = pWing->m_dMainCTCA/2;;
				mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
				mark_input_data.left_dir = se*lr>0;
				mark_input_data.height = dTickWall/vSlab.y;

				mark_view_data.count_mark = FALSE;
				mark_view_data.mark_pos = eMarkSide;

				Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
			}
		}
	}

	*pDomP << Dom;
}

void CDrawWingWall::MarkRebarPlaneLower(CDomyun* pDomP, BOOL bStt, BOOL bLeft, int nOut)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	double se = bStt ? -1.0 : 1.0;
	double lr = bLeft ? -1.0 : 1.0;
	double io = nOut==0 ? -1.0 : 1.0;

	CTwinVectorArray tvArr;
	CTwinVector tv, tvEnd, tvOut;

	pBri->GetTvWingWallPlane(bStt, TRUE, FALSE, FALSE);
	tvArr = pBri->m_tvArrPlaneWingWall;

	tvArr.GetTvByInfo(_T("슬래브끝단외측"), tv);
	CDPoint vX(1, 0), vY(0, 1);
	CDPoint xyOrigin = bLeft ? tv.m_v1 : tv.m_v2;
	CDPoint vAngWing = pBri->GetAngleAzimuthWingWall(bStt, bLeft);
	CDPoint vRotate = vAngWing.BetweenAngle(vX*se);
	tvArr.Rotate(xyOrigin, vRotate);
	tvArr.Move(-xyOrigin);

	CString strDir = bLeft ? _T("좌측") : _T("우측");
	tvArr.GetTvByInfo(strDir+_T("날개벽외측"), tvOut);
	tvArr.GetTvByInfo(strDir+_T("날개벽끝단"), tvEnd);

	CDPoint vAng = pBri->GetAngleWingWallBetweenWall(bStt, bLeft);
	double dCoverWing = nOut==0 ? pBri->m_dCoverWingIn[0] : pBri->m_dCoverWingOut[0];
	double dCoverEnd  = pBri->m_dCoverWingEnd;
	double dT1 = pWing->m_dT1;
	double dT2 = pWing->m_dT2;
	double dT = pWing->m_nDirHunch==0 ? dT1 : dT2;
	double dCTC = nOut==0 ? pWing->m_dStdVerCTC/2 : pWing->m_dStdVerCTC;

	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML); 

	CDPoint xyMark(0, 0);
	CDPoint xy1(0, 0), xy2(0, 0);
	CDPointArray xyArr;
	CDoubleArray dArrShear;
	CDPoint xyShearStt = tvOut.m_v1;
	CDPoint vEnd(0, 0);
	int nEaA = 0;
	CDPoint vOut = tvOut.GetVDir();
	if(pWing->m_nAttachPos == 0)
	{
		vEnd = pWing->m_bRAngEnd ? vY : vAng;
	}
	else
	{
		vEnd = pWing->m_bRAngEnd ? vY : tvEnd.GetXyDir();
		vEnd = vEnd.GetMirrorHorz();
	}

	CDPoint xyOutStt = tvEnd.m_v1 ;
	CDPoint xyInStt = tvEnd.m_v2;
	CDPoint xyStt = nOut==0 ? xyInStt : xyOutStt;
	xyStt.x -= dCoverEnd/vEnd.y*se - (dCoverWing+REBAR_RADIUS)/tan(vEnd.GetAngleRadian())*se*io;

	xyArr = pWing->m_xyArrLower[nOut];
	if(pWing->m_nDirHunch==1 && !pWing->m_bRAngEnd)
	{
		//	끝단 사각처리이고 변단면일경우 T2로주면 실제 입력한 배면혹은 전면과 맞지 안는다.
		//	그래서(pWing->m_nDirHunch==1 && !pWing->m_bRAngEnd)일때
		//	가상의 배근 치수로 변환 해서 철근만 그려준다.
		pBri->SetXyArrConvertRebarVirtualLow(xyArr, bStt, bLeft);
	}
	//.. 수직 철근
	int nRBIndex = nOut==0 ? WW_REBAR_VER_C_IN : WW_REBAR_VER_C_OUT;
	CRebar* pRB = pWing->m_pArrRebar[nRBIndex];
	if(pRB->IsTrue())
	{
		nEaA = pRB->m_nEa;
		double dX = pBri->m_Util.GetPosRebarMark(xyArr, 3, FALSE);
		xyMark = xyStt - dX*vX*se + (dCoverWing+REBAR_RADIUS)*vY*io*lr*se;
// 		if(io*lr*se>0)
// 			Sym.RebarMarkPointLower(&Dom, xyMark, dCTC, dCTC, pRB->m_sMark, pRB->GetStringDia(), !bStt);
// 		else
// 			Sym.RebarMarkPointUpper(&Dom, xyMark, dCTC, dCTC, pRB->m_sMark, pRB->GetStringDia(), !bStt);

		SRebarMarkInputData mark_input_data;
		SRebarMarkviewData mark_view_data;
		mark_input_data.mark = pRB->m_sMark;
		mark_input_data.dia = pRB->GetStringDia();
		mark_input_data.position = xyMark;
		mark_input_data.ctc = dCTC;
		mark_input_data.dist_left_mark = dCTC;
		mark_input_data.dist_right_mark = dCTC;
		mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
		mark_input_data.left_dir = !bStt;
		mark_input_data.height = 0;

		mark_view_data.count_mark = FALSE;
		mark_view_data.line_rebar = FALSE;
		mark_view_data.mark_pos = (io*lr*se>0)? eMarkLower : eMarkUpper;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}

	//.. 수평 철근
	nRBIndex = nOut==0 ? WW_REBAR_HOR_B_IN : WW_REBAR_HOR_B_OUT;
	pRB = pWing->m_pArrRebar[nRBIndex];
	if(pRB->IsTrue())
	{
		double dX = pBri->m_Util.GetPosRebarMark(xyArr, nEaA/2, TRUE);
		xyMark = xyStt - dX*vX*se + dCoverWing*vY*io*lr*se;
// 		if(io*lr*se>0)
// 			Sym.RebarMarkLineLower(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), !bStt);
// 		else
// 			Sym.RebarMarkLineUpper(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), !bStt);

		SRebarMarkInputData mark_input_data;
		SRebarMarkviewData mark_view_data;
		mark_input_data.mark = pRB->m_sMark;
		mark_input_data.dia = pRB->GetStringDia();
		mark_input_data.position = xyMark;
		mark_input_data.ctc = (nOut==0)? pWing->m_dMainCTCB/2 : pWing->m_dMainCTCB;
		mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
		mark_input_data.left_dir = !bStt;
		mark_input_data.height = 0;

		mark_view_data.count_mark = FALSE;
		mark_view_data.mark_pos = (io*lr*se>0)? eMarkLower : eMarkUpper;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}

	if(nOut==1)
	{
		//.. 전단 철근
		char cShearType = 'B';
		nRBIndex = WW_REBAR_SHEAR_B;
		pRB = pWing->m_pArrRebar[nRBIndex];
		if(pRB->IsTrue() == FALSE)
		{
			nRBIndex = WW_REBAR_SHEAR_C;
			pRB = pWing->m_pArrRebar[nRBIndex];
			cShearType = 'C';
		}
		if(pRB->IsTrue())
		{
			pBri->GetLengthArrayShearFront(dArrShear, cShearType, bStt, bLeft);
			int nSize = dArrShear.GetSize();
			GetXyMatchLineAndLine(tvOut.m_v1, vX, xyShearStt, vEnd, xyShearStt);
			vector<double> x_shear;
			
			//#0031291 - 2 전단철근 마킹표현 x_shear가 생성후 if문안에서 x_shear의 데이터를 채워주는데 if문의 조건이 x_shear의 갯수가 1개 이상일때만 들어가게 되어 있어서 수정.
			int i=0; for(i=0; i<nSize; i++)
			{
				xyMark = xyShearStt + dArrShear.GetAt(i)*vX*se + dT/2*vY*lr*se;
				xyMark.x += (dCoverWing+REBAR_RADIUS)/tan(vEnd.GetAngleRadian())*se;
				Sym.RebarCountMarkOne(&Dom, xyMark);
				x_shear.push_back(xyMark.x);
			}
			
			if(x_shear.size() > 0)
			{
				xy1 = CDPoint(x_shear.front(), xyMark.y);
				xy2 = CDPoint(x_shear.back(), xyMark.y);
				Dom.LineTo(xy1, xy2);
				xyMark.x = pBri->m_Util.GetPosRebarMark(x_shear, nSize/3);
// 				if(io*lr*se>0)
// 					Sym.RebarMarkLineLower(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), !bStt, 0, FALSE);
// 				else
// 					Sym.RebarMarkLineUpper(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), !bStt, 0, FALSE);

				SRebarMarkInputData mark_input_data;
				SRebarMarkviewData mark_view_data;
				mark_input_data.mark = pRB->m_sMark;
				mark_input_data.dia = pRB->GetStringDia();
				mark_input_data.position = xyMark;
				mark_input_data.ctc = (nRBIndex == WW_REBAR_SHEAR_C)? pWing->m_dSpaceShearC : pWing->m_dSpaceShearB;
				mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
				mark_input_data.left_dir = !bStt;
				mark_input_data.height = 0;

				mark_view_data.count_mark = FALSE;
				mark_view_data.mark_pos = (io*lr*se>0)? eMarkLower : eMarkUpper;

				Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
			}
		}

		//.. 헌치 철근
		nRBIndex = WW_REBAR_HUNCH_WALL_LO;
		pRB = pWing->m_pArrRebar[nRBIndex];
		if(pRB->IsTrue())
		{
			CTwinVectorArray tvArrHu;
			pBri->GetTvArrayRebarWingWallHunchWallPlane(tvArrHu, bStt, bLeft, FALSE);
			tvArrHu.Rotate(xyOrigin, vRotate);
			tvArrHu.Move(-xyOrigin);
			xyMark = tvArrHu.GetAt(1).GetXyMid();
			CDPoint vTick = vAng.x>0 ? CDPoint(0, 1) : CDPoint(1, 0);
			Sym.RebarCountMarkOne(&Dom, xyMark, vTick);
// 			if(io*lr*se>0)
// 				Sym.RebarMarkLineUpper(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), bStt, 0, FALSE);
// 			else
// 				Sym.RebarMarkLineLower(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), bStt, 0, FALSE);

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = pRB->m_sMark;
			mark_input_data.dia = pRB->GetStringDia();
			mark_input_data.position = xyMark;
			mark_input_data.ctc = (pWing->m_nHunchCTC == 0)? pWing->m_dMainCTCB/2 : pWing->m_dMainCTCB;
			mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
			mark_input_data.left_dir = bStt;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.mark_pos = (io*lr*se>0)? eMarkLower : eMarkUpper;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}

		// 흉벽보강철근
		if(pWing->m_bAddRebarReinforce && pWing->m_nAttachPos==0)
		{
			nRBIndex = WW_REBAR_REINFORCE_B;
			pRB = pWing->m_pArrRebar[nRBIndex];
			if(pRB->IsTrue())
			{
				CTwinVector tvSlab;
				tvArr.GetTvByInfo(strDir+_T("날개벽외측"), tvOut);
				tvArr.GetTvByInfo(_T("슬래브끝단외측"), tvSlab);
				
				double dCoverWallOut = pBri->m_dCoverWallOut[0];
				double dCoverSlab = pBri->m_dCoverWallIn[0];
				double dTickWall = (bStt)? pBri->m_dWS : pBri->m_dWE;
				double dDistOffset = dTickWall - dCoverSlab - dCoverWallOut;
				Offset(tvOut.m_v1, tvOut.m_v2, -(pBri->m_dCoverWingOut[0])*lr);
				Offset(tvSlab.m_v1, tvSlab.m_v2, -dDistOffset);
				CDPoint vOut = tvOut.GetXyDir();
				CDPoint vSlab = tvSlab.GetXyDir();
				CDPoint vTick = vAng.x>0 ? CDPoint(0, 1) : CDPoint(1, 0);
				
				GetXyMatchLineAndLine(tvOut.m_v1, vOut, tvSlab.m_v1, vSlab, tv.m_v1);
				tv.m_v2 = tv.m_v1 - vSlab*3000*lr;
				xyMark = tv.GetXyMid();
				
				Sym.RebarCountMarkOne(&Dom, xyMark, vTick);
				// Sym.RebarMarkLineSide(&Dom, xyMark, pRB->m_sMark, pRB->GetStringDia(), se*lr>0, dTickWall/vSlab.y, FALSE);

				SRebarMarkInputData mark_input_data;
				SRebarMarkviewData mark_view_data;
				mark_input_data.mark = pRB->m_sMark;
				mark_input_data.dia = pRB->GetStringDia();
				mark_input_data.position = xyMark;
				mark_input_data.ctc = pWing->m_dMainCTCB/2;;
				mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
				mark_input_data.left_dir = se*lr>0;
				mark_input_data.height = dTickWall/vSlab.y;

				mark_view_data.count_mark = FALSE;
				mark_view_data.mark_pos = eMarkSide;

				Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
			}
		}
	}

	*pDomP << Dom;
}

// 피복 입력용 치수
void CDrawWingWall::DimForCoverInput(CDomyun *pDomP, CString sArea, BOOL bStt, BOOL bLeft)
{
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CWingWall* pWing	= pBri->GetWingWall(bStt, bLeft);
	if(!pWing) return;
	if(!pWing->m_bExist) return;
	
	CDomyun Dom(pDomP);
	CHgBaseDrawStd baseDraw;

	CDoubleArray dArr;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	long nDimDan	= 0;
	CString sDir	= _T("");
	CDPoint xyStt	= CDPoint(0, 0);

	
	if(sArea == "날개벽 외면" || sArea == "날개벽 내면")
	{
		CTwinVectorArray tvArr;
		CTwinVector tvUpper;
		pBri->GetTvWingWallSection(bStt, bLeft, tvArr);
		tvArr.GetTvByInfo(_T("날개벽단면상단"), tvUpper);
	
		double dCoverIn = pBri->m_dCoverWingIn[0];
		double dCoverOut = pBri->m_dCoverWingOut[0];
		double dT1 = pWing->m_dT1;
		double dT2 = pWing->m_dT2;
		if(dT1>dT2)
		{
			//dT1 = pWing->m_dT2;
			dT2 = pWing->m_dT1;
		}

		dArr.Add(dCoverOut);
		sArrText.Add("날개벽 외면");

		dArr.Add(dT2-dCoverOut-dCoverIn);
		sArrText.Add("0");

		dArr.Add(dCoverIn);
		sArrText.Add("날개벽 내면");


		sDir	= _T("TOP");
		xyStt	= tvUpper.GetXyLeft();
		baseDraw.AutoDimCont(&Dom, xyStt, dArr, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
	}
	else if(sArea == "날개벽 상면")
	{
		CTwinVectorArray tvArr;
		CTwinVector tvUpper;
		pBri->GetTvWingWallSection(bStt, bLeft, tvArr);
		tvArr.GetTvByInfo(_T("날개벽단면상단"), tvUpper);
		
		dArr.Add(pBri->m_dCoverWingUpper);
		sArrText.Add("날개벽 상단");

		sDir	= _T("LEFT");
		xyStt	= tvUpper.GetXyLeft() + CDPoint(0, -pBri->m_dCoverWingUpper);
		baseDraw.AutoDimCont(&Dom, xyStt, dArr, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
	}
	else if(sArea == "날개벽 끝단")
	{
		BOOL bUpper	= TRUE;
		double se = bStt ? -1.0 : 1.0;
		double lr = bLeft ? -1.0 : 1.0;
		double dL1 = pWing->m_dL1;
		double dL2 = pWing->m_dL2;
		double dT1 = pWing->m_dT1;
		double dT2 = pWing->m_dT2;
		double dT = bUpper ? dT1 : dT2;

		CTwinVectorArray tvArr;
		CTwinVector tv, tvSlab, tvOut;

		tvArr = pBri->m_tvArrPlaneWingWall;

		tvArr.GetTvByInfo(_T("슬래브끝단외측"), tv);
		CDPoint vX(1, 0), vY(0, 1);
		CDPoint xyOrigin = bLeft ? tv.m_v1 : tv.m_v2;
		CDPoint vAngWing = pBri->GetAngleAzimuthWingWall(bStt, bLeft);
		CDPoint vRotate = vAngWing.BetweenAngle(vX*se);
		tvArr.Rotate(xyOrigin, vRotate);
		tvArr.Move(-xyOrigin);

		CDPoint vAng = pBri->GetAngleWingWallBetweenWall(bStt, bLeft);
		CDPoint xyOutStt(0, 0), xyInStt(0, 0);
		double dSpareT = (pWing->m_nDirHunch==2 ? dT2 : dT)/vAng.y*vAng.x;

		CString strDir = bLeft ? _T("좌측") : _T("우측");
		tvArr.GetTvByInfo(strDir+_T("날개벽외측"), tvOut);
		xyOutStt = tvOut.m_v1;
		xyInStt = xyOutStt + dSpareT*vX + dT*vY*lr*se;

		CDomyun Dom(pDomP);

		CString strDirOut = se*lr>0 ? _T("BOTTOM") : _T("TOP");
		CString strDirIn = se*lr>0 ? _T("TOP") : _T("BOTTOM");

		CDPoint xyR(0, 0);
		double dCoverWingEnd = pBri->GetCoverWingWallEnd(bStt, bLeft);
		CString str1 = _T(""), str2 = _T("");

		// 배면
		Dom.SetDirection(strDirIn);
		int nDan = 1;

		// 피복 치수
		Dom.DimMoveTo(xyOutStt.x + dL1 + dL2 - dCoverWingEnd, xyOutStt.y, nDan);
		Dom.DimLineTo(dCoverWingEnd, nDan, "날개벽 끝단");

		if(bStt)
			Dom.Mirror(xyOutStt.x, TRUE);

		*pDomP << Dom;
	}


	*pDomP << Dom;
}

void CDrawWingWall::DrawDetailReinforceRebar(CDomyun* pDomP, BOOL bStt, BOOL bLeft)
{
	CARcBridgeDataStd *pStd	= m_pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = m_pStd->m_pRcBridgeRebar;
	CWingWall* pWing = pBri->GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	CRebarPlacing rp;

	CTwinVectorArray tvArrGen;
	CTwinVector tvOut, tvEnd, tvHuPrev, tvHuNext, tvHunch, tvSlab;

	pBri->GetTvWingWallPlane(bStt, TRUE, TRUE, TRUE);
	tvArrGen = pBri->m_tvArrPlaneWingWall;

	CString strDir = bLeft ? _T("좌측") : _T("우측");
	tvArrGen.GetTvByInfo(strDir+_T("날개벽외측"), tvOut);
	tvArrGen.GetTvByInfo(strDir+_T("날개벽끝단"), tvEnd);
	tvArrGen.GetTvByInfo(strDir+_T("날개벽내부헌치전"), tvHuPrev);
	tvArrGen.GetTvByInfo(strDir+_T("날개벽내부헌치"), tvHunch);
	tvArrGen.GetTvByInfo(strDir+_T("날개벽내부헌치후"), tvHuNext);
	tvArrGen.GetTvByInfo(_T("슬래브끝단외측"), tvSlab);

	long nDan = 0;
	double dCoverWingOut = pBri->m_dCoverWingOut[0];
	double dCoverWingIn = pBri->m_dCoverWingIn[0];
	double dCoverWingEnd = pBri->GetCoverWingWallEnd(bStt, bLeft, FALSE);
	double dCoverSlab = pBri->m_dCoverWallIn[0];
	double dCoverWallOut = pBri->m_dCoverWallOut[0];
	double dT1 = pWing->m_dT1;
	double dT2 = pWing->m_dT2;
	double dLT = pWing->m_dLT;
	double dDiaVar = pWing->m_dL1>0 ? pWing->m_dDiaMainD[0][nDan] : pWing->m_dDiaMainA[0][nDan];
	double dDiaIn = pWing->m_dDiaMainA[0][nDan];
	double dDiaOut = pWing->m_dDiaMainA[1][nDan];
	if(pWing->m_dL2==0)
	{
		dDiaVar = pWing->m_dDiaMainD[0][nDan];
		dDiaIn = pWing->m_dDiaMainD[0][nDan];
		dDiaOut = pWing->m_dDiaMainD[1][nDan];
	}
	double dCTCIn = pWing->m_dMainCTCA/2;
	double dCTCOut = pWing->m_dMainCTCA;
	double dSettleVar = pStd->GetValueSettle(dDiaVar, SETTLE_TENS_NORMAL, toCm(dCoverWingIn), toCm(dCTCIn));
	double dSettleOut = pStd->GetValueSettle(dDiaOut, SETTLE_TENS_NORMAL, toCm(dCoverSlab), toCm(dCTCOut));
	double dSettleIn = pStd->GetValueSettle(dDiaIn, SETTLE_TENS_NORMAL, toCm(dCoverSlab), toCm(dCTCIn));
	double lr = bLeft ? -1.0 : 1.0;
	CDPoint xyHu[3];
	pBri->GetXyRebarWingWallVaryPlane(xyHu, bStt, bLeft);

	int nDirHunch = dT1==dT2 ? 1 : pWing->m_nDirHunch;
	if(nDirHunch==1)
	{
		dT1 = dT2 = pWing->m_dT1;
		dLT = 0.0;
	}

	CDPoint xyOrigin = bLeft ? tvSlab.m_v1 : tvSlab.m_v2;
	CDPoint xyShearStt = tvOut.m_v1;
	Offset(tvSlab.m_v1, tvSlab.m_v2, -dCoverSlab);
	CDPoint xyCen(0, 0);
	
	Offset(tvOut.m_v1, tvOut.m_v2, -dCoverWingOut*lr);
	Offset(tvEnd.m_v1, tvEnd.m_v2, -dCoverWingEnd*lr);
	Offset(tvHuNext.m_v1, tvHuNext.m_v2, -dCoverWingIn*lr);
	Offset(tvHunch.m_v1, tvHunch.m_v2, -dCoverWingIn*lr);
	Offset(tvHuPrev.m_v1, tvHuPrev.m_v2, -dCoverWingIn*lr);

	CDPoint vOut = tvOut.GetXyDir();
	CDPoint vEnd = tvEnd.GetXyDir();
	CDPoint vHuPrev = tvHuPrev.GetXyDir();
	CDPoint vHuNext = tvHuNext.GetXyDir();
	CDPoint vSlab = tvSlab.GetXyDir();

	GetXyMatchLineAndLine(tvOut.m_v1, vOut, tvSlab.m_v1, vSlab, tvOut.m_v1);
	GetXyMatchLineAndLine(tvHuNext.m_v2, vHuNext, tvSlab.m_v1, vSlab, tvHuNext.m_v2);
	GetXyMatchLineAndLine(tvOut.m_v1, vOut, tvEnd.m_v1, vEnd, tvEnd.m_v1);
	if(dLT==0)
		GetXyMatchLineAndLine(tvHuNext.m_v2, vHuNext, tvEnd.m_v1, vEnd, tvEnd.m_v2);
	else
		GetXyMatchLineAndLine(tvHuPrev.m_v2, vHuPrev, tvEnd.m_v1, vEnd, tvEnd.m_v2);
	CDPoint xySetOut = tvOut.m_v1 - dSettleOut*vSlab*lr;
	CDPoint xySetIn = tvHuNext.m_v2 - dSettleIn*vSlab*lr;

	//로테이트가 잘못될 경우때문에 따로따로 설정
	CDomyun Dom(pDomP);
	CDomyun Dom2(pDomP);
	CDomyun Dom3(pDomP);
	CDomyun Dom4(pDomP);
	CDomyun DomDim(pDomP);
	CDomyun DomDim2(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom3, HCAD_CONC);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom4, HCAD_CONR);

	//평면도
	DrawGenPlane(&Dom3, bStt);	

	//날개벽 피복선 
	CTwinVectorArray tvArrRearHori;
	CTwinVector tvRebarHori;
	tvRebarHori.SetFromPoint(tvEnd.m_v1, tvEnd.m_v2);
	tvArrRearHori.Add(tvRebarHori);
	tvRebarHori.SetFromPoint(tvOut.m_v1, tvEnd.m_v1);
	tvArrRearHori.Add(tvRebarHori);
	tvRebarHori.SetFromPoint(tvOut.m_v1, xySetOut);
	tvArrRearHori.Add(tvRebarHori);

	if(dT1<dT2)
	{
		tvRebarHori.SetFromPoint(tvEnd.m_v2, xyHu[1]+dSettleVar*vHuPrev);
		tvArrRearHori.Add(tvRebarHori);
		tvRebarHori.SetFromPoint(xyHu[2], xyHu[0]);
		tvArrRearHori.Add(tvRebarHori);
		tvRebarHori.SetFromPoint(xyHu[0], tvHuNext.m_v2);
		tvArrRearHori.Add(tvRebarHori);
	}
	else if(dT1>dT2)
	{
		tvRebarHori.SetFromPoint(tvEnd.m_v2, xyHu[0]);
		tvArrRearHori.Add(tvRebarHori);
		tvRebarHori.SetFromPoint(xyHu[0], xyHu[2]);
		tvArrRearHori.Add(tvRebarHori);
		tvRebarHori.SetFromPoint(xyHu[1]-dSettleVar*vHuNext, tvHuNext.m_v2);
		tvArrRearHori.Add(tvRebarHori);
	}
	else
	{
		tvRebarHori.SetFromPoint(tvEnd.m_v2, tvHuNext.m_v2);	
		tvArrRearHori.Add(tvRebarHori);
	}		
	tvRebarHori.SetFromPoint(tvHuNext.m_v2, xySetIn);
	tvArrRearHori.Add(tvRebarHori);

	m_pStd->DrawTvArray(&Dom4, tvArrRearHori);
	
	// 헌치 철근
	DrawRebarHunchWallSect(&Dom2, bStt, bLeft, TRUE);

	// 흉벽보강철근
	CTwinVectorArray tvArrRebarB;
	CTwinVector tvRebarB;

	CDPoint xyRf1(0, 0), xyRf2(0, 0), xyRf3(0, 0);
	double dTickWall = (bStt)? pBri->m_dWS : pBri->m_dWE;
	double dDistOffset = dTickWall - dCoverSlab - dCoverWallOut;

	Offset(tvOut.m_v1, tvOut.m_v2, -REBAR_RADIUS*lr);
	Offset(tvSlab.m_v1, tvSlab.m_v2, -dDistOffset);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom2, HCAD_BARC);

	BOOL bMatWall = GetXyMatchLineAndLine(tvOut.m_v1, vOut, tvSlab.m_v1, vSlab, xyRf2);
	if(bMatWall)
	{
		xyRf1 = xyRf2 - vSlab*3000*lr;
		xyRf3 = xyRf2 + vOut*1000;		

		tvRebarB.SetFromPoint(xyRf1, xyRf2);
		tvArrRebarB.Add(tvRebarB);
		tvRebarB.SetFromPoint(xyRf2, xyRf3);
		tvArrRebarB.Add(tvRebarB);
	}
	else
	{
		//흉벽(반대)+날개벽 전면과 평행 상태 => 본체만 표기
		CTwinVectorArray tvArrHunch, tvArrVal;
		pBri->GetTvArrayRebarWingWallHunchWallPlane(tvArrHunch, bStt, bLeft, TRUE);

		if(tvArrHunch.GetSize() > 0)
		{
			long nIdx = (long)RoundUp(tvArrHunch.GetSize() / 2, 3);
			tvArrVal.Add(tvArrHunch.GetAt(nIdx <=0? 0 : nIdx));
		}
		else
		{
			tvArrVal.Add(tvHuNext);
		}
		
		CTwinVector tvSlabExt = tvSlab;
		tvSlabExt.m_v1 = tvSlabExt.m_v1 + (tvSlabExt.GetXyDir(TRUE) * 3000);
		tvSlabExt.m_v2 = tvSlabExt.m_v2 + (tvSlabExt.GetXyDir() * 3000);

		CDPointArray xyArrMat;
		rp.GetXyMatchTvArrAndTv(tvArrVal, tvSlabExt, xyArrMat, FALSE);


		if(xyArrMat.GetSize() > 0)
		{
			CDPoint xyStart = xyArrMat.GetAt(0);			

			if(bLeft)
			{
				tvSlabExt.m_v1 = xyStart;
				tvSlabExt.m_v2 = tvSlabExt.m_v1 + (tvSlabExt.GetXyDir() * 3000);
				tvArrRebarB.Add(tvSlabExt);
			}
			else
			{
				tvSlabExt.m_v2 = xyStart;
				tvSlabExt.m_v1 = tvSlabExt.m_v2 + (tvSlabExt.GetXyDir(TRUE) * 3000);
				tvArrRebarB.Add(tvSlabExt);
			}	

			xyRf2 = xyStart;
		}	
	}	
	m_pStd->DrawTvArray(&Dom2, tvArrRebarB);

	//트림
	xyCen = xyRf2;
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CUTL);
	Dom.Circle(xyCen.x, xyCen.y, 3500);
	Dom.TrimByCircle(xyCen, 3500, TRUE);
	Dom3.TrimByCircle(xyCen, 3500, TRUE);
	Dom4.TrimByCircle(xyCen, 3500, TRUE);

	//그린 도면 Rotate (치수선만 예외)	
	CDPoint vRotate = vSlab.BetweenAngle(CDPoint(1, 0));
	CDPoint vTemp = pBri->GetAngleWingWallBetweenWall(bStt, bLeft);
	CDPoint vAngbet = ToDPointFrDegree(180-vTemp.GetAngleDegree()*lr);

	Dom.Rotate(xyCen, vRotate);
	Dom.Move(-xyCen);
	Dom2.Rotate(xyCen, vRotate);
	Dom2.Move(-xyCen);
	Dom3.Rotate(xyCen, vRotate);
	Dom3.Move(-xyCen);
	Dom4.Rotate(xyCen, vRotate);
	Dom4.Move(-xyCen);

	CRebarPlacing rb;
	rb.RotateTvArr(tvArrGen, xyCen, vRotate.GetAngleDegree());
	rb.MoveRebar(tvArrGen, -xyCen);
	CTwinVector tvRotEnd;
	tvArrGen.GetTvByInfo(strDir+_T("날개벽끝단"), tvRotEnd);

	// 치수 및 철근Mark
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	xyOrigin -= xyCen;
	xyCen = CDPoint(0, 0);
	m_pStd->m_pARoadOptionStd->SetEnvType(&DomDim, HCAD_DIML);
	m_pStd->m_pARoadOptionStd->SetEnvType(&DomDim2, HCAD_DIML);
	DomDim.SetDirection("BOTTOM");
	DomDim2.SetDirection("BOTTOM");
	DomDim.DimMoveTo(xyCen.x, xyCen.y, 0);
	DomDim.DimLineTo(3000*lr*-1, 0, COMMA(3000));

	int nRBIndex = WW_REBAR_REINFORCE_A;
	CRebar* pRB = pWing->m_pArrRebar[nRBIndex];
	if(pRB->IsTrue())
	{
		xyRf2 = xyCen + CDPoint(1, 0)*(3000*lr*-1)/2;	
		// Sym.RebarMarkLineUpper(&Dom, xyRf2, pRB->m_sMark, pRB->GetStringDia(), !bLeft, 1000);

		SRebarMarkInputData mark_input_data;
		SRebarMarkviewData mark_view_data;
		mark_input_data.mark = pRB->m_sMark;
		mark_input_data.dia = pRB->GetStringDia();
		mark_input_data.position = xyRf2;
		mark_input_data.ctc = pWing->m_dMainCTCA/2;;
		mark_input_data.type_rebar = pRB? pRB->m_nTypeRebar : -1;
		mark_input_data.left_dir = !bLeft;
		mark_input_data.height = 1000;

		mark_view_data.count_mark = FALSE;
		mark_view_data.mark_pos = eMarkUpper;

		Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}

	tvArrGen.GetTvByInfo(_T("슬래브끝단외측"), tvSlab);
	if(bMatWall)
		xyOrigin = xyCen;
	else
		xyOrigin = tvRotEnd.GetXyMid();

	double dExtendWing = bMatWall? 2500 : 0;
	
	BOOL bMarkLeft = (lr == -1)? TRUE:FALSE;
	CDPoint xyMarkWall = xyCen - CDPoint(1, 0)*2000*lr;
	CDPoint xyMarkWing = xyOrigin + vAngbet*dExtendWing*lr;
	DomDim.GiSiArrow(xyMarkWall.x, xyMarkWall.y-800, 500*-1*lr, 800, 2, "흉벽");
	DomDim.GiSiArrow(xyMarkWing.x+1300*lr, xyMarkWing.y, 500*-1*lr, 800, (bMarkLeft)? 0:4, "날개벽");
	
	if(bMatWall)
	{
		DomDim2.DimMoveTo(xyCen.x, xyCen.y, 0);
		DomDim2.DimLineTo(1000*lr, 0, COMMA(1000));
		DomDim2.Rotate(xyCen, vAngbet);
	}	
	

	*pDomP << Dom;
	*pDomP << Dom2;
	*pDomP << Dom3;
	*pDomP << Dom4;
	*pDomP << DomDim;
	*pDomP << DomDim2;
	
}

void CDrawWingWall::GetTvArrShearRebar(CTwinVectorArray &tvArr, BOOL bDownShape, BOOL bOneLine, BOOL bBack, long nShearType, long nShearTypeC)
{
	// 전단철근 공통 변수 
	const double a  = 30;	//세로
	const double b  = 50;	//가로
	const double ud = bDownShape ? -1.0 : 1.0;
	
	CRebarPlacing rb;
	
	CTwinVectorArray tvArrShear;
	CTwinVector tv, tvShear;
	CDPoint rotate(0, 0);
	BOOL bTypeDi = (nShearType == TYPE_SHEAR_DIGUT || nShearType == TYPE_SHEAR_SINGLE_HOOK) ? TRUE : FALSE;
	BOOL bSpacer = nShearType == TYPE_SHEAR_SPACER ? TRUE : FALSE;
	BOOL bTypeDiC = (nShearTypeC == TYPE_SHEAR_DIGUT || nShearTypeC == TYPE_SHEAR_SINGLE_HOOK) ? TRUE : FALSE;
	BOOL bSpacerC = nShearTypeC == TYPE_SHEAR_SPACER ? TRUE : FALSE;
	
	long nSize	= tvArr.GetSize();
	long i = 0; for(i = 0; i < nSize; i++)
	{
		tv	= tvArr.GetAt(i);
		if(tv.m_sInfo=="전단C")
		{
			rotate = CDPoint(1, 0);
			bTypeDi = (bTypeDiC || bSpacerC);
		}
		else rotate = CDPoint(0, 1);

		if(bOneLine)	// 한줄로만 긋는 형상.
		{
			tvShear.m_v1	= tv.m_v1;
			tvShear.m_v2	= tv.m_v1 + CDPoint(b, 0);
		
			// 우측으로 이동
			tvShear.m_v1	= tvShear.m_v1 + CDPoint(30, 0);
			tvShear.m_v2	= tvShear.m_v2 + CDPoint(30, 0);

			// 회전
			rb.RotateTv(tvShear, tv.m_v1, rotate.GetAngleDegree());
			tvArrShear.Add(tvShear);

		}
		else if(bTypeDi || bSpacer)
		{
			tvShear.m_v1	= tv.m_v1;
			tvShear.m_v2	= tv.m_v1 + CDPoint(b, 0);
			
			// 우측으로 이동
			tvShear.m_v1	= tvShear.m_v1 + CDPoint(0, -30);
			tvShear.m_v2	= tvShear.m_v2 + CDPoint(0, -30);
			// 회전
			tvShear = rb.RotateTv(tvShear, tv.m_v1, rotate.GetAngleDegree());
			tvArrShear.Add(tvShear);
		}
		else if(bBack)
		{
			tvShear.m_v1	= tv.m_v1;
			tvShear.m_v2	= tv.m_v1 + CDPoint(b, 0);
			
			// 우측으로 이동
			tvShear.m_v1	= tvShear.m_v1 + CDPoint(0, -30);
			tvShear.m_v2	= tvShear.m_v2 + CDPoint(0, -30);
			// 회전
			tvShear = rb.RotateTv(tvShear, tv.m_v1, rotate.GetAngleDegree());
			tvArrShear.Add(tvShear);
			
			tvShear.m_v1	= CDPoint(tv.m_v1.x+tv.GetLength(), tv.m_v1.y);
			tvShear.m_v2	= tvShear.m_v1 + CDPoint(-b, 0);
			
			// 우측으로 이동
			tvShear.m_v1	= tvShear.m_v1 + CDPoint(0, -30);
			tvShear.m_v2	= tvShear.m_v2 + CDPoint(0, -30);
			// 회전
			tvShear = rb.RotateTv(tvShear, tv.m_v1, rotate.GetAngleDegree());
			tvArrShear.Add(tvShear);
		}
		else
		{
			CTwinVectorArray tvArrTmp;
			tvShear.m_v1	= tv.m_v1;
			tvShear.m_v2	= tv.m_v1 + CDPoint(b, a*ud);
			tvArrTmp.Add(tvShear);
			
			tvShear.m_v1	= tvShear.m_v2;
			tvShear.m_v2	= tvShear.m_v1 + CDPoint(tv.GetLength()-b*2, 0);
			tvArrTmp.Add(tvShear);
			
			tvShear.m_v1	= tvShear.m_v2;
			tvShear.m_v2	= tvShear.m_v1 + CDPoint(b, -a*ud);
			tvArrTmp.Add(tvShear);
			
			rb.RotateTvArr(tvArrTmp, tv.m_v1, rotate.GetAngleDegree());
			
			tvArrShear.AddFromTvArray(tvArrTmp);
		}
	}
	
	tvArr	= tvArrShear;
}

// 날개벽 단면의 최 상단 전단철근 표현이 필요한지?
// A,D구간의 전단철근이 ㄷ형태가 아닌게 있어야 함.
BOOL CDrawWingWall::IsNeedTopRebarShearSection(CWingWall *pWing)
{
	if(pWing->m_dL2 > 0 && pWing->m_RebarShearDetailA.m_nType != TYPE_SHEAR_DIGUT && pWing->m_RebarShearDetailA.m_nType != TYPE_SHEAR_SINGLE_HOOK) return TRUE;
	if(pWing->m_dL1 > 0 && pWing->m_RebarShearDetailD.m_nType !=  TYPE_SHEAR_DIGUT && pWing->m_RebarShearDetailD.m_nType !=  TYPE_SHEAR_SINGLE_HOOK) return TRUE;

	return FALSE;
}
