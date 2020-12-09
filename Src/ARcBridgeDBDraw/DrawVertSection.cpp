// DrawRcBridge.cpp: implementation of the CDrawVertSection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARcBridgeDBDraw.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawVertSection::CDrawVertSection()
{

}

CDrawVertSection::~CDrawVertSection()
{

}

// 종단면도 그리기
// 포장면은 옵션에 따라 그림
void CDrawVertSection::DrawVertSection(CDomyun *pDomP, BOOL bDrawPave, CTwinVectorArray *tvArrReturn, CDRect rectLimit, BOOL bDrawConstJoint)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	CHgBaseDrawStd baseDraw;
	baseDraw.m_pLine	= pBri->GetLineBase();

	// 종단면 TEST
	//pBri->GetTvVertSection_Std(1, TRUE, TRUE);

	if(rectLimit == CDRect(0, 0, 0, 0))
	{
		baseDraw.DrawTvArray(&Dom, pBri->m_tvArrVertSection, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo(), 0);
		tvArrReturn->ChangePos(pBri->m_tvArrVertSection);

		if(bDrawPave)
		{
			CTwinVectorArray tvArrPave;
			pBri->GetTvPave_VertSection(tvArrPave);
 			baseDraw.DrawTvArray(&Dom, tvArrPave, FALSE, FALSE, 0);
		}
		if(bDrawConstJoint)	// 시공 이음(시공 순서도에서만 필요)
		{
			CTwinVector tvLeft, tvRight, tvU, tvL;
			CDPoint xy(0, 0), xyNum(0, 0);
			double dTS = pBri->m_dTS;
			Dom.SetTextHeight(2);

			// 시점 벽체
			if(pBri->m_tvArrVertSection.GetTvByInfo(_T("시점벽체좌측면"), tvLeft))
			{
				pBri->m_tvArrVertSection.GetTvByInfo(_T("시점벽체우측면"), tvRight);
				double dW = tvRight.m_v1.x - tvLeft.m_v1.x;
				xy = CDPoint(tvLeft.m_v1.x, tvRight.m_v2.y+(tvRight.m_v1.y-tvRight.m_v2.y)*2/3);
				m_pStd->DrawConstJointHor(&Dom, xy, dW);

				xyNum = (xy+tvLeft.m_v1)/2 + CDPoint(dW/2, 0);
				if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
					Dom.TextCircle(xyNum.x, xyNum.y, _T("3"), 3, FALSE, TRUE);
				else
					Dom.TextCircle(xyNum.x, xyNum.y, _T("2"), 3, FALSE, TRUE);
				double dHuH = pBri->m_dHHS;
				xyNum = tvRight.m_v1 + CDPoint(-dW/2, (dHuH+dTS)/2);
				Dom.TextCircle(xyNum.x, xyNum.y, _T("4"), 3, FALSE, TRUE);
			}

			// 종점 벽체
			if(pBri->m_tvArrVertSection.GetTvByInfo(_T("종점벽체좌측면"), tvLeft))
			{
				pBri->m_tvArrVertSection.GetTvByInfo(_T("종점벽체우측면"), tvRight);
				double dW = tvRight.m_v1.x - tvLeft.m_v1.x;
				xy = CDPoint(tvLeft.m_v1.x, tvLeft.m_v1.y+(tvLeft.m_v2.y-tvLeft.m_v1.y)*2/3);
				m_pStd->DrawConstJointHor(&Dom, xy, dW);

				xyNum = (xy+tvLeft.m_v1)/2 + CDPoint(dW/2, 0);
				if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
					Dom.TextCircle(xyNum.x, xyNum.y, _T("3"), 3, FALSE, TRUE);
				else
					Dom.TextCircle(xyNum.x, xyNum.y, _T("2"), 3, FALSE, TRUE);
				double dHuH = pBri->m_dHHE;
				xyNum = tvLeft.m_v2 + CDPoint(dW/2, (dHuH+dTS)/2);
				Dom.TextCircle(xyNum.x, xyNum.y, _T("4"), 3, FALSE, TRUE);
			}

			// 슬래브
			CString str = _T("");
			if(!pBri->m_tvArrVertSection.GetTvByInfo(_T("슬래브상면(외측벽체안쪽)"), tvU))
				pBri->m_tvArrVertSection.GetTvByInfo("슬래브상면(1번째지간)", tvU);

			int nSize = pBri->m_tvArrVertSection.GetSize();
			int i=0; for(i=0; i<nSize; i++)
			{
				tvL = pBri->m_tvArrVertSection.GetAt(i);
				str = tvL.m_sInfo;
				if(str.Right(10)==_T("슬래브하면"))
				{
					CDPoint xy1 = (tvL.m_v1+tvL.m_v2*3)/4;
					GetXyMatchSegAndLine(tvU.m_v1, tvU.m_v2, xy1, CDPoint(0, 1), xy);
					m_pStd->DrawConstJointVer(&Dom, xy, dTS, TRUE);
					CDPoint xy2 = (tvL.m_v1*3+tvL.m_v2)/4;
					GetXyMatchSegAndLine(tvU.m_v1, tvU.m_v2, xy2, CDPoint(0, 1), xy);
					m_pStd->DrawConstJointVer(&Dom, xy, dTS, FALSE);

					xyNum = (xy1+xy2)/2 + CDPoint(0, dTS/2);
					if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
						Dom.TextCircle(xyNum.x, xyNum.y, _T("5"), 3, FALSE, TRUE);
					else
						Dom.TextCircle(xyNum.x, xyNum.y, _T("3"), 3, FALSE, TRUE);
				}
			}

			CString strLeft(_T("")), strRight(_T(""));
			int nCountJijum = pBri->GetCountJijum();
			for(i=1; i<nCountJijum-1; i++)
			{
				strLeft.Format(_T("%d번째내측헌치좌측면"), i);
				strRight.Format(_T("%d번째내측헌치우측면"), i);
				pBri->m_tvArrVertSection.GetTvByInfo(strLeft, tvLeft);
				pBri->m_tvArrVertSection.GetTvByInfo(strRight, tvRight);

				double dHuH = max(pBri->m_dHHL[i-1][iUPPER], pBri->m_dHHR[i-1][iUPPER]);
				xyNum = (tvLeft.m_v1+tvRight.m_v2)/2 + CDPoint(0, (dHuH+dTS)/2);
				Dom.TextCircle(xyNum.x, xyNum.y, _T("4"), 3, FALSE, TRUE);
			}

			//박스형
			if(pBri->IsBoxType())
			{
				CTwinVector tvStt, tvEnd, tvLowerSlab;

				pBri->m_tvArrVertSection.GetTvByInfo(_T("시점벽체하부헌치"), tvStt);
				pBri->m_tvArrVertSection.GetTvByInfo(_T("종점벽체하부헌치"), tvEnd);
				
				m_pStd->DrawConstJointHor(&Dom, tvStt.m_v1 - CDPoint(1,0)*pBri->m_dWS, pBri->m_dWS);
				m_pStd->DrawConstJointHor(&Dom, tvEnd.m_v2, pBri->m_dWE);

				for(i=1; i<nCountJijum-1; i++)
				{
					CWallApp *pWall = pBri->GetInWall(i-1);
					strLeft.Format(_T("%d번째내측하부헌치좌측면"), i);
					pBri->m_tvArrVertSection.GetTvByInfo(strLeft, tvLeft);
					m_pStd->DrawConstJointHor(&Dom, tvLeft.m_v2, pWall->m_dW);
				}

				pBri->m_tvArrVertSection.GetTvByInfo(_T("하부슬래브하면(외측벽체안쪽)"), tvLowerSlab);
				Dom.TextCircle(tvLowerSlab.GetXyMid().x, tvLowerSlab.GetXyMid().y + pBri->m_dTS_Lower/2, _T("1"), 3, FALSE, TRUE);
			}

			// 아치리브
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
			{
				// 리브를 네 등분으로 나눠서 좌우측 1/4씩 "2"번으로 지정
				long nSizeU	= pBri->m_tvArrVertSectionRib[iUPPER].GetSize();
				double xDisStt	= pBri->m_pbRib[iUPPER].GetDistByXPos(pBri->m_tvArrVertSectionRib[iUPPER].GetAt(0).m_v1.x);
				double xDisEnd	= pBri->m_pbRib[iUPPER].GetDistByXPos(pBri->m_tvArrVertSectionRib[iUPPER].GetAt(nSizeU-1).m_v1.x);
				double dLenTot	= pBri->m_pbRib[iUPPER].GetLenByDistToDist(xDisStt, xDisEnd);

				double xDisMarkLeft		= xDisStt + (dLenTot/4);
				double xDisMarkRight	= xDisEnd - (dLenTot/4);
				CDPoint xyMarkLeft	= pBri->m_pbRib[iUPPER].GetXyByDist(xDisMarkLeft);
				CDPoint xyMarkRight	= pBri->m_pbRib[iUPPER].GetXyByDist(xDisMarkRight);
				CDPoint xyDirLeft	= pBri->m_pbRib[iUPPER].GetXyDirPerByDist(xDisMarkLeft, TRUE);
				CDPoint xyDirRight	= pBri->m_pbRib[iUPPER].GetXyDirPerByDist(xDisMarkRight, TRUE);
				double dThickLeft	= pBri->GetThickArchRib(xDisMarkLeft);
				double dThickRight	= pBri->GetThickArchRib(xDisMarkRight);

				m_pStd->DrawConstJointVer(&Dom, xyMarkLeft, dThickLeft, TRUE, xyDirLeft.GetAngleDegree()-270);
				m_pStd->DrawConstJointVer(&Dom, xyMarkRight, dThickRight, TRUE, xyDirRight.GetAngleDegree()-270);

				// 숫자 써 넣기
				xDisMarkLeft	= xDisStt + (dLenTot/8);
				xDisMarkRight	= xDisEnd - (dLenTot/8);
				xyMarkLeft	= pBri->m_pbRib[iUPPER].GetXyByDist(xDisMarkLeft);
				xyMarkRight	= pBri->m_pbRib[iUPPER].GetXyByDist(xDisMarkRight);
				xyDirLeft	= pBri->m_pbRib[iUPPER].GetXyDirPerByDist(xDisMarkLeft, TRUE);
				xyDirRight	= pBri->m_pbRib[iUPPER].GetXyDirPerByDist(xDisMarkRight, TRUE);
				dThickLeft	= pBri->GetThickArchRib(xDisMarkLeft);
				dThickRight	= pBri->GetThickArchRib(xDisMarkRight);

				xyMarkLeft	+= (xyDirLeft * dThickLeft/2);
				xyMarkRight	+= (xyDirRight * dThickRight/2);
				Dom.TextCircle(xyMarkLeft.x, xyMarkLeft.y, _T("2"), 3, FALSE, TRUE);
				Dom.TextCircle(xyMarkRight.x, xyMarkRight.y, _T("2"), 3, FALSE, TRUE);
			}
		}
	}
	else
	{
		BOOL bCutLine = TRUE;
		CTwinVectorArray tvArr;
		tvArr	= pBri->m_tvArrVertSection;
			
		if(bDrawPave)
		{
			CTwinVectorArray tvArrPave;
			pBri->GetTvPave_VertSection(tvArrPave);
			tvArr.AddFromTvArray(tvArrPave);
		}

		if(rectLimit.top < rectLimit.bottom) GetSwap(rectLimit.top, rectLimit.bottom);
		rectLimit.top += 10;
		rectLimit.bottom -= 10;

		//BOX형일때는 위아래 따로 커팅 라인 그려줘야함.
		if(pBri->IsBoxType())
		{
			CLineInfoApp *pLine = m_pStd->m_pARcBridgeDataStd->GetLineApp();
		
			CDPoint xyLBottom = pBri->GetXyLowerSlabBottomByOffsetFrStation(rectLimit.left, 0, 0);
			CDPoint xyRBottom = pBri->GetXyLowerSlabBottomByOffsetFrStation(rectLimit.right, 0, 0);
			double dLTop = pLine->GetElevationPlanEl(rectLimit.left) - pBri->GetThickPave();
			double dRTop = pLine->GetElevationPlanEl(rectLimit.right) - pBri->GetThickPave();

			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CUTL);
			if(tvArr.GetXyLeft().x < rectLimit.left)
			{
				Dom.CutLightning(rectLimit.left, dLTop + Dom.Always(5), rectLimit.left, dLTop - pBri->m_dTS - Dom.Always(5));
				Dom.CutLightning(xyLBottom.x, xyLBottom.y + pBri->m_dTS_Lower + Dom.Always(5), xyLBottom.x, xyLBottom.y - Dom.Always(5));
			}
			if(tvArr.GetXyRight().x > rectLimit.right)
			{
				Dom.CutLightning(rectLimit.right, dRTop + Dom.Always(5), rectLimit.right, dRTop - pBri->m_dTS - Dom.Always(5));
				Dom.CutLightning(rectLimit.right, xyRBottom.y + pBri->m_dTS_Lower + Dom.Always(5), rectLimit.right, xyRBottom.y - Dom.Always(5));
			}

			bCutLine = FALSE;
		}

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
		m_pStd->DrawCuttingTvArray(&Dom, tvArr, rectLimit, bCutLine);
	}


	*pDomP << Dom;
}

// 종단면도 상면 치수
void CDrawVertSection::DimVertSectionUpper(CDomyun *pDomP, long nDimDan, BOOL bStt, BOOL bInput, BOOL bOnlyJijum, BOOL bSttEndMark)
{
	// 지점치수 전용 함수.
	if(bOnlyJijum)
	{
		DimVertSectionUpperOnlyJijum(pDomP, nDimDan, 0);
		return;
	}

	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDomP, &Dom);
	
	if(nDimDan < 0) nDimDan = 0;
	CTwinVector tv;
	CTwinVectorArray *pTvArr = &pBri->m_tvArrVertSection;
	
	CDoubleArray dArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("TOP");
	CDPoint xyStt(0, 0);
	CDPoint xySttTot(0, 0), xyEndTot(0, 0);
	
	pTvArr->GetTvByInfo("시점브라켓상면", tv);
	xyStt.x = tv.m_v1.x;
	
	BOOL bApplyAngle	= tv.m_bApplyAngle;

	double dOldX	= 0;
	if(!bOnlyJijum)
	{
		dArrDim.Add(tv.GetHorLength(bApplyAngle));
		if(bStt)
		{
			sArrText.Add(bInput ? "BW" : "0");
			sArrUnderText.Add(bInput ? COMMA3(tv.GetHorLength(TRUE), tv.GetHorLength(FALSE)) : COMMA2(tv.GetHorLength(FALSE)));
		}
		else
		{
			sArrText.Add(COMMA(tv.GetHorLength(bApplyAngle)));
			sArrUnderText.Add(bApplyAngle ? COMMA2(tv.GetHorLength(FALSE)) : "0");
		}
			
		pTvArr->GetTvByInfo("슬래브상면(시점벽체위)", tv);
		dArrDim.Add(tv.GetHorLength(bApplyAngle));
		sArrText.Add(COMMA(tv.GetHorLength(bApplyAngle)));
		sArrUnderText.Add(bApplyAngle ? COMMA2(tv.GetHorLength(FALSE)) : "0");

		dOldX = tv.m_v2.x;
	}
	else
	{
		pTvArr->GetTvByInfo("슬래브상면(외측벽체안쪽)", tv);
		xyStt.x	= tv.m_v1.x;
		dOldX	= xyStt.x;
	}

	xyStt.y = max(pBri->GetXySectionSttEnd(TRUE).y, pBri->GetXySectionSttEnd(FALSE).y);
	xySttTot.x = tv.m_v1.x;

	// 내측벽체 
	if(!pBri->IsOutWall(TRUE)) 
	{
		if(pTvArr->GetTvByInfo("시점슬래브측면", tv))
			xyStt.x	= tv.m_v2.x;
		else
			xyStt.x = pBri->GetXySectionSttEnd(TRUE).x;

		xySttTot.x = xyStt.x;
		dOldX = xyStt.x;

		if(pBri->m_bExpJoint[0])
		{
			CJijum *pJ	= pBri->GetJijum(0);
			dArrDim.Add(pJ->m_dLengthUgan[1]);
			sArrText.Add("0");
			sArrUnderText.Add("0");

			xyStt.x		-= pJ->m_dLengthUgan[1];
			xySttTot.x	-= pJ->m_dLengthUgan[1];
		}
	}

	long i = 0; for(i = 0; i < pBri->GetCountInWall(); i++)
	{
		CWallApp *pWall = pBri->GetInWall(i);
		BOOL bRoundHunch	= pBri->IsRoundHunch(i+1, TRUE, TRUE) || pBri->IsRoundHunch(i+1, FALSE, TRUE);
		if(pWall->m_bIs && !bOnlyJijum && !bRoundHunch)
		{
			if(i == 0)
			{
				CTwinVector tvExp;
				if(pBri->m_fxJointStt.m_tvArrSection.GetTvByInfo("시점신축이음상면", tvExp))
					dOldX	= tvExp.GetXyRight().x;
		
			}
			if(pWall->m_tvArrFront.GetTvByInfo("벽체좌측면", tv))
			{
				double dLength	= tv.m_v2.x-dOldX;
				CDPoint vAng	= pBri->GetAngleJijum(i);
 				dArrDim.Add(dLength);
				sArrText.Add(COMMA(dLength));
				sArrUnderText.Add(bApplyAngle && pBri->IsEnableDisplayVertLengthOfJigan() ? COMMA2(dLength*vAng.y) : "0");
				dOldX = tv.m_v2.x;
			}

			if(pWall->m_tvArrFront.GetTvByInfo("벽체우측면", tv))
			{
				double dLength	= tv.m_v2.x-dOldX;
				CDPoint vAng	= pBri->GetAngleJijum(i+1);
 				dArrDim.Add(dLength);
				sArrText.Add(COMMA(dLength));
				sArrUnderText.Add(bApplyAngle && pBri->IsEnableDisplayVertLengthOfJigan() ? COMMA2(dLength*vAng.y) : "0");
				dOldX = tv.m_v2.x;
			}
		}
		// 슬래브인 경우 벽체 중심을 찍어줌..(즉 지점을 찍어줌)
		else
		{
			double dSta	= pBri->GetStationOnJijum(i+1, 0, !bApplyAngle);

			if(pBri->m_fxJointStt.m_tvArrSection.GetTvByInfo("시점신축이음상면", tv) && i == 0)
			{
				if(tv.GetHorLength() > 0)
				{
					dOldX	= tv.GetXyRight().x;
				}
			}

			CString sHunch[2];
			sHunch[iLEFT].Format("%d번째내측헌치좌측면", i+1);
			sHunch[iRIGHT].Format("%d번째내측헌치우측면", i+1);
			CTwinVector tvHunchL	= pBri->m_tvArrVertSection.GetTvByInfo(sHunch[iLEFT]);
			CTwinVector tvHunchR	= pBri->m_tvArrVertSection.GetTvByInfo(sHunch[iRIGHT]);

			dArrDim.Add(tvHunchL.GetXyTop().x-dOldX);
			sArrText.Add("0");
			sArrUnderText.Add("0");
			dOldX	= tvHunchL.GetXyTop().x;
				
			dArrDim.Add(dSta-dOldX);
			sArrText.Add("0");
			sArrUnderText.Add("0");
			dOldX	= dSta;

			dArrDim.Add(tvHunchR.GetXyTop().x-dOldX);
			sArrText.Add("0");
			sArrUnderText.Add("0");
			dOldX	= tvHunchR.GetXyTop().x;
		}
	}

	if(pTvArr->GetTvByInfo("슬래브상면(외측벽체안쪽)", tv))
	{
		double dLength = tv.m_v2.x-dOldX;
		CDPoint vAng = pBri->GetAngleJijum(0);
		dArrDim.Add(dLength);
//		sArrText.Add("0");
//		sArrUnderText.Add("0");
		sArrText.Add(COMMA(dLength));
		sArrUnderText.Add(bApplyAngle && pBri->IsEnableDisplayVertLengthOfJigan() ? COMMA2(dLength*vAng.y) : "0");
	}
	else if(pTvArr->GetTvByInfo("종점슬래브측면", tv))
	{
		double dLength	= tv.m_v2.x - dOldX;
		CDPoint vAng	= pBri->GetAngleJijum(pBri->GetCountJijum()-1);
		dArrDim.Add(dLength);
		sArrText.Add(COMMA(dLength));
		sArrUnderText.Add(bApplyAngle && pBri->IsEnableDisplayVertLengthOfJigan() ? COMMA2(dLength*vAng.y) : "0");
	}
	
	if(!bOnlyJijum)
	{
		if(pTvArr->GetTvByInfo("슬래브상면(종점벽체위)", tv))
		{
			dArrDim.Add(tv.GetHorLength(bApplyAngle));
			sArrText.Add(COMMA(tv.GetHorLength(bApplyAngle)));
			sArrUnderText.Add(bApplyAngle ? COMMA2(tv.GetHorLength(FALSE)) : "0");
		}
		else
		{
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB)
			{
				CString str	= _T("");
				if(pBri->m_nQtyJigan > 1)
				{
					str.Format("%d번째내측헌치우측슬래브하면", pBri->m_nQtyJigan-1);
				}
				else
				{
					str	= "슬래브상면(1번째지간)";
				}
				if(pTvArr->GetTvByInfo(str, tv))
				{
					if(pBri->m_nQtyJigan > 1)
					{
						CTwinVector tvTmp;
						str	= "종점슬래브측면";
						pTvArr->GetTvByInfo(str, tvTmp);
						tv.m_v1	= tvTmp.m_v2;
					}

					CTwinVector tvExp;
					if(0)//pBri->m_fxJointEnd.m_tvArrSection.GetTvByInfo("종점신축이음상면", tvExp))
					{
						dArrDim.Add(tv.GetHorLength(bApplyAngle) - tvExp.GetHorLength(bApplyAngle));
						sArrText.Add(COMMA(tv.GetHorLength(bApplyAngle) - tvExp.GetHorLength(bApplyAngle)));
						sArrUnderText.Add(bApplyAngle && pBri->IsEnableDisplayVertLengthOfJigan() ? COMMA2(tv.GetHorLength(FALSE) - tvExp.GetHorLength(FALSE)) : "0");
					}
				}
			}
		}

		xyEndTot.x = tv.m_v2.x;

		if(pTvArr->GetTvByInfo("종점브라켓상면", tv))
		{
			dArrDim.Add(tv.GetHorLength(bApplyAngle));
			if(!bStt)
			{
				sArrText.Add(bInput ? "BW" : COMMA(tv.GetHorLength(bApplyAngle)));
				sArrUnderText.Add(bInput ? COMMA3(tv.GetHorLength(TRUE), tv.GetHorLength(FALSE)) : COMMA2(tv.GetHorLength(FALSE)));
			}
			else
			{
				sArrText.Add(COMMA(tv.GetHorLength(bApplyAngle)));
				sArrUnderText.Add(bApplyAngle ? COMMA2(tv.GetHorLength(FALSE)) : "0");
			}
		}
	}
	else
	{
		if(!pBri->IsOutWall(FALSE) && pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB)
		{
			pTvArr->GetTvByInfo("종점슬래브측면", tv);
			dArrDim.Add(tv.m_v2.x-dOldX);
			sArrText.Add("0");
			sArrUnderText.Add("0");
		}

		xyEndTot.x	= tv.m_v2.x;
	}

	// 시작쯤 끝점 체크(벽체가 없는경우 치수기입 위치를 다시 지정해줘야 됨.
	if(!pBri->IsOutWall(FALSE))
	{
		if(pTvArr->GetTvByInfo("종점슬래브측면", tv))
			xyEndTot.x	= tv.m_v1.x;
		else
			xyEndTot.x	= pBri->GetXySectionSttEnd(FALSE).x;

		if(pBri->m_bExpJoint[pBri->GetCountJijum()-1])
		{
			CJijum *pJ	= pBri->GetJijum(pBri->GetCountJijum()-1);
			dArrDim.Add(pJ->m_dLengthUgan[0]);
			sArrText.Add("0");
			sArrUnderText.Add("0");

			xyEndTot.x += pJ->m_dLengthUgan[0];
		}
	}

	if(dArrDim.GetSize()==0)
	{
		if(!pTvArr->GetTvByInfo("슬래브상면(외측벽체안쪽)", tv))
			pTvArr->GetTvByInfo("슬래브상면(1번째지간)", tv);
		
		dArrDim.Add(tv.GetHorLength(bApplyAngle));
		sArrText.Add("0");
		sArrUnderText.Add("0");
	}

	// 길이가 0인거 일단 제거
	for(i = 0; i < dArrDim.GetSize(); i++)
	{
		if(dArrDim.GetAt(i) == 0)
		{
			dArrDim.RemoveAt(i);
			sArrText.RemoveAt(i);
			sArrUnderText.RemoveAt(i);

			i--;
		}
	}

	// 신축이음에 대해서 여기서 임의로 넣어준다.
	if(pBri->IsBlockOut(TRUE) && dArrDim.GetSize() > 1)
	{
		pBri->m_fxJointStt.m_tvArrSection.GetTvByInfo("시점신축이음상면", tv);
		
		double dLen	= tv.GetHorLength(bApplyAngle);

		dArrDim.InsertAt(1, dLen);
		sArrText.InsertAt(1, bInput && bStt ? "FXW" : "0");
		sArrUnderText.InsertAt(1, bApplyAngle ? COMMA2(tv.GetHorLength(FALSE)) : "0");

		if(pBri->m_nQtyJigan == 1)
		{
			if(dArrDim.GetSize() > 1 && sArrText.GetSize() > 1 && sArrUnderText.GetSize() > 1)
			{
				dArrDim.SetAt(2, dArrDim.GetAt(2)-dLen);
				sArrText.SetAt(2, "0");
				sArrUnderText.SetAt(2, "0");
			}
		}
	}

	if(pBri->IsBlockOut(FALSE) && dArrDim.GetSize() > 2)
	{
		pBri->m_fxJointEnd.m_tvArrSection.GetTvByInfo("종점신축이음상면", tv);
		
		double dLen	= tv.GetHorLength(bApplyAngle);
		double dLenUnder	= tv.GetHorLength(FALSE);

		dArrDim.InsertAt(dArrDim.GetSize()-1, dLen);
		sArrText.InsertAt(sArrText.GetSize()-1, bInput && !bStt ? "FXW" : "0");
		sArrUnderText.InsertAt(sArrUnderText.GetSize()-1, bApplyAngle ? COMMA2(dLenUnder) : "0");

		if(dArrDim.GetSize() > 3 && sArrText.GetSize() > 3 && sArrUnderText.GetSize() > 3)
		{
//			dArrDim.SetAt(dArrDim.GetSize()-3, dArrDim.GetAt(dArrDim.GetSize()-3)-dLen);
//			sArrText.SetAt(sArrText.GetSize()-3, "0");
//			sArrUnderText.SetAt(sArrUnderText.GetSize()-3, "0");
		}
	}

	// 모두 직교일 경우 괄호안 치수는 의미가 없다. //////////
	m_pStd->ModifyInfoAutoDimConc(sArrText, sArrUnderText, bInput);
	///////////////////////////////////////////////////////////

	if(m_pStd->m_bDisplayAngLenStd && !pBri->IsIncludeOnlyLineInBridge() && m_pStd->IsDimByAngleLen())
	{
		if(sArrText.GetSize() < dArrDim.GetSize())
		{
			long i = 0; for(i = 0; i < dArrDim.GetSize() - sArrText.GetSize(); i++)
			{
				sArrText.Add("0");
			}
		}

		/*
		CString sMsg	= sArrText.GetAt(sArrText.GetSize()-1);
		if(sMsg == "0")
			sMsg	= COMMA(dArrDim.GetAt(dArrDim.GetSize()-1));
		sMsg = sMsg + "(선형기준 사거리)";
		sArrText.SetAt(sArrText.GetSize()-1, sMsg);
		*/
		dArrDim.Add(0.0001);
		sArrText.Add("(선형기준 사거리)");
	}

	CHgBaseDrawStd baseDraw;
	baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE);

	if(m_pStd->m_bDisplayAngLenStd && !pBri->IsIncludeOnlyLineInBridge() && m_pStd->IsDimByAngleLen())
	{
		dArrDim.RemoveAt(dArrDim.GetSize()-1);
		sArrText.RemoveAt(sArrText.GetSize()-1);
	}

	if(nDimDan)	//DimVertSectionUpper(&Dom, bForOutBM ? 0 : 1, TRUE, FALSE);
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		Dom.SetDirection(sDir);
		
		// 교량상면 치수(브라켓 제외)
		long nCount = 0;
		for(i = 0; i < dArrDim.GetSize(); i++)
		{
			if(dArrDim.GetAt(i) > 0) nCount++;
			if(nCount > 1) break;
		}
		if(nCount > 1)
		{
			nDimDan++;
			xyStt.x = xySttTot.x;
			double dist = xyEndTot.x-xySttTot.x; 
			if(dist > 0)
			{
				Dom.DimMoveTo(xyStt.x, xyStt.y, nDimDan);
				Dom.DimLineTo(dist, nDimDan, COMMA(dist));
			}
		}

		//
		if(bSttEndMark)
		{
			CDPoint xyMark(0,0);
			xyMark.x = xySttTot.x;
			xyMark.y = xyStt.y + Dom.Always(Dom.GetDimExo() + Dom.GetDimVLen() + Dom.GetDimLevelDis()*nDimDan + Dom.GetDimExe());
			m_pStd->MarkStationJijum(&Dom, 0, xyMark);
			xyMark.x = xyEndTot.x;
			m_pStd->MarkStationJijum(&Dom, pBri->m_nQtyJigan, xyMark);
		}
	}

	*pDomP << Dom;
}

/**
	@brief
		수량 산출시 슬래브 길이를 선형길이로 할 경우의 슬래브 길이 치수 
		1. 벽체 치수는 각각 벽체중간에 기입하고
		2. 슬래브 길이는 통으로 하나만 기입한다.
*/
void CDrawVertSection::DimVertSectionUpperForBMSlabLengthTyp1(CDomyun *pDom)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;
	CDomyun Dom(pDom);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDom, &Dom);

	CTwinVectorArray *pTvArr	= &pBri->m_tvArrVertSection;

	CTwinVector tvStt;
	if(!pTvArr->GetTvByInfo("시점브라켓상부측면", tvStt))
	{
		if(!pTvArr->GetTvByInfo("시점슬래브측면", tvStt))
			return;
	}

	CTwinVector tvEnd;
	if(!pTvArr->GetTvByInfo("종점브라켓상부측면", tvEnd))
	{
		if(!pTvArr->GetTvByInfo("종점슬래브측면", tvEnd))
			return;
	}


	CDPoint xyStt	= tvStt.GetXyTop();
	CDPoint xyEnd	= tvEnd.GetXyTop();
	double dLenBridge	= pBri->GetLengthSlabReal(0);

	Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
	Dom.DimLineTo(xyEnd.x - xyStt.x, 0, COMMA(dLenBridge));
	

	*pDom << Dom;
}


// 종단면도 시점측 치수
// 외측벽체 없으면 치수 안 넣음.. (종점측에만 들어가면 됨)
void CDrawVertSection::DimVerSectionSideStt(CDomyun *pDomP, long nDimDan, BOOL bInput, BOOL bForOutBM)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDomP, &Dom);
	
	if(nDimDan < 0) nDimDan = 0;
	
	CTwinVector tv;
	CTwinVectorArray *pTvArr = &pBri->m_tvArrVertSection;

	CDoubleArray dArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("LEFT");
	CDPoint xyStt(0, 0);
	CDPoint xySttTot(0, 0), xyEndTot(0, 0);
	double dH = 0;

	// 벽체가 없으면 슬래브 두께 치수만 넣음
	if(!pBri->IsOutWall(TRUE))
	{
		CDPointArray xyArrDim;
		CTwinVector tv;
		CTwinVector tvLower, tvHunch;
		pTvArr->GetTvByInfo("시점슬래브측면", tv);

		xyArrDim.Add(tv.m_v1);
		xyArrDim.Add(tv.m_v2);
		xyStt	= tv.m_v1;

		if(pTvArr->GetTvByInfo("시점벽체헌치", tvHunch))
		{
			xyArrDim.Add(tvHunch.GetXyTop());
		}

		xyArrDim.Sort(FALSE);

		if(pBri->IsBlockOut(TRUE) && pBri->m_fxJointStt.m_tvArrSection.GetTvByInfo("시점신축이음하면", tv))
		{
			xyArrDim.Add(tv.GetXyLeft());
			xyArrDim.Sort(FALSE);

			// 신축이음 있는 경우 젤 위에 있는 치수를 포장 포함한 치수로 변경
			double dTP	= pBri->GetThickPave(FALSE);
			xyArrDim.SetAt(xyArrDim.GetSize()-1, xyArrDim.GetAt(xyArrDim.GetSize()-1)+CDPoint(0, dTP));


			if(bInput)
			{
				long i = 0; for(i = 0; i < xyArrDim.GetSize(); i++)
				{
					sArrText.Add("0");
					sArrUnderText.Add("0");
				}
				sArrText.InsertAt(sArrText.GetSize()-1, "FXH");
				tv	= pBri->m_fxJointStt.m_tvArrSection.GetTvByInfo("시점신축이음좌측면");
				sArrUnderText.InsertAt(sArrUnderText.GetSize()-1, COMMA(tv.GetVerLength()));
			}
		}

		CHgBaseDrawStd baseDraw;
		baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE);

		*pDomP << Dom;

		pTvArr->GetTvByInfo("시점헌치하면", tvLower);
		if(tvLower.GetHorLength() > 0 || tvHunch.GetHorLength() > 0)
		{
			xyArrDim.RemoveAll();
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();
			if(tvHunch.GetHorLength() > 0)
			{
				xyArrDim.Add(tvHunch.m_v1);
				xyArrDim.Add(tvHunch.m_v2);
				xyStt	= tvHunch.GetXyLeft();
			}
			if(tvLower.GetHorLength() > 0)
			{
				xyArrDim.Add(tvLower.m_v2);
				xyStt	= tvLower.GetXyLeft();
			}

			xyArrDim.Sort(TRUE);
			
			sDir	= _T("BOTTOM");
			baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE);
			*pDomP << Dom;
		}
		

		return;
	}

	// 치수기입 시작위치는 기초가 있을 경우 기초의 확대기초를 기준으로 한다. 
	CExFootingApp *pExFooting	= (pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)? &pBri->m_footingBox.m_exFooting : &pBri->m_footingStt.m_exFooting;
	CDRect rect	= pExFooting->m_tvArrFront.GetRect();
	
	if(pExFooting->m_nType == EXFOOTING_TYPE_MASS && pExFooting->m_bFrontDanMass) 
	{
		CExFootingApp *pExFooting	= &pBri->m_footingStt.m_exFooting;
		CString str		= _T("");
		double dH		= pExFooting->GetMaxHeight();
		double dHCur	= 0;
		long i = 0; for(i = 0; i < pExFooting->m_nCountDan; i++)
		{
			str.Format("EH%d", i+1);
			dArrDim.Add(pExFooting->m_dEFH[i]);
			sArrText.Add(str);
			sArrUnderText.Add("");
			dHCur	+= pExFooting->m_dEFH[i];
		}

		dArrDim.Add(dH-dHCur);
		sArrText.Add("0");
		sArrUnderText.Add("");
	}
	else 
	{
		double dH	= rect.top - rect.bottom;
		dArrDim.Add(dH);
		sArrText.Add("EH");
		sArrUnderText.Add(COMMA(dH));
	}
	xyStt = CDPoint(rect.left, rect.bottom);

	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		if(pBri->m_bExistProtectionRiseFooting)
		{
			dArrDim.Add(pBri->m_dPRF_H1);
			sArrText.Add("H1");
			sArrUnderText.Add(COMMA(pBri->m_dPRF_H1));
			dArrDim.Add(pBri->m_dPRF_H2);
			sArrText.Add("H2");
			sArrUnderText.Add(COMMA(pBri->m_dPRF_H2));
		}

		xySttTot.y = pBri->m_dELSB;
	}
	else
	{
		pBri->m_footingStt.m_tvArrFront.GetTvByInfo("기초좌측면", tv);
		dArrDim.Add(tv.GetVerLength());
		if(pBri->m_footingStt.m_nType != FOOTING_TYPE_BASIC_RIGHTSLOPE)
		{
			sArrText.Add("FH");
			sArrUnderText.Add(COMMA(tv.GetVerLength()));
		}
		else
		{
			sArrText.Add("0");
			sArrUnderText.Add("0");
		}
		
		xySttTot.y = tv.GetXyBottom().y;
	}

	pTvArr->GetTvByInfo("시점벽체좌측면", tv);
	dArrDim.Add(tv.GetVerLength());
	sArrText.Add("0");
	sArrUnderText.Add("0");

	pTvArr->GetTvByInfo("시점브라켓헌치", tv);
	dH = tv.GetVerLength();
	if(!bInput)
	{
		dArrDim.Add(dH);
		sArrText.Add("0");
		sArrUnderText.Add(COMMA(dH));
	}
	pTvArr->GetTvByInfo("시점브라켓측면", tv);
	if(bInput)
		dH += tv.GetVerLength();
	else
		dH = tv.GetVerLength();
	dArrDim.Add(dH);
	sArrText.Add("BH");
	sArrUnderText.Add(COMMA(dH));

	pTvArr->GetTvByInfo("시점브라켓상부측면", tv);
	dArrDim.Add(tv.GetVerLength());
	sArrText.Add("BTH");
	sArrUnderText.Add(COMMA(tv.GetVerLength()));

	// 포장 두께(수량 산출시는 넣지 않음)
	if(!bForOutBM)
	{
		dArrDim.Add(pBri->GetThickPave(FALSE));
		sArrText.Add("0");
		sArrUnderText.Add("0");
	}

	xyEndTot.y = tv.m_v2.y;

	if(!bInput)
	{
		sArrText.RemoveAll();
		sArrUnderText.RemoveAll();
	}
	CHgBaseDrawStd baseDraw;
 	baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE);

	// 전체 치수	
	{
		// 교량측면 치수
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		m_pStd->ApplyDimSetting(pDomP, &Dom);
		Dom.SetDirection(sDir);
		nDimDan++;
		xyStt.y = xySttTot.y;
		double dist = xyEndTot.y-xySttTot.y; 
		if(dist > 0)
		{
			Dom.DimMoveTo(xyStt.x, xyStt.y, nDimDan);
			Dom.DimLineTo(dist, nDimDan, COMMA(dist));
		}
	}

	*pDomP << Dom;
}

// 종단면도 시점측 치수
void CDrawVertSection::DimVerSectionSideEnd(CDomyun *pDomP, long nDimDan, BOOL bInput, BOOL bForOutBM)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDomP, &Dom);
	
	if(nDimDan < 0) nDimDan = 0;
	
	CTwinVector tv;
	CTwinVectorArray *pTvArr = &pBri->m_tvArrVertSection;

	
	CDoubleArray dArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("RIGHT");
	CDPoint xyStt(0, 0);
	CDPoint xySttTot(0, 0), xyEndTot(0, 0);
	double dH = 0;

	// 벽체가 없으면 슬래브 두께 치수만 넣음
	if(!pBri->IsOutWall(FALSE))
	{
		CDPointArray xyArrDim;
		CTwinVector tv, tvHunch, tvLower;
		pTvArr->GetTvByInfo("종점슬래브측면", tv);

		xyArrDim.Add(tv.m_v1);
		xyArrDim.Add(tv.m_v2);
		xyStt	= tv.m_v2;

		if(pTvArr->GetTvByInfo("종점벽체헌치", tvHunch))
		{
			xyArrDim.Add(tvHunch.GetXyTop());
		}
		xyArrDim.Sort(FALSE);

		if(pBri->IsBlockOut(FALSE) && pBri->m_fxJointEnd.m_tvArrSection.GetTvByInfo("종점신축이음하면", tv))
		{
			xyArrDim.Add(tv.GetXyRight());
			xyArrDim.Sort(FALSE);

			// 신축이음 있는 경우 젤 위에 있는 치수를 포장 포함한 치수로 변경
			double dTP	= pBri->GetThickPave(FALSE);
			xyArrDim.SetAt(xyArrDim.GetSize()-1, xyArrDim.GetAt(xyArrDim.GetSize()-1)+CDPoint(0, dTP));

			if(bInput)
			{
				long i = 0; for(i = 0; i < xyArrDim.GetSize(); i++)
				{
					sArrText.Add("0");
					sArrUnderText.Add("0");
				}
				sArrText.InsertAt(sArrText.GetSize()-1, "FXH");
				tv	= pBri->m_fxJointEnd.m_tvArrSection.GetTvByInfo("종점신축이음우측면");
				sArrUnderText.InsertAt(sArrUnderText.GetSize()-1, COMMA(tv.GetVerLength()));
			}
		}


		CHgBaseDrawStd baseDraw;
		baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE);

		*pDomP << Dom;
		pTvArr->GetTvByInfo("종점헌치하면", tvLower);

		if(tvLower.GetHorLength() > 0 || tvHunch.GetHorLength() > 0)
		{
			xyArrDim.RemoveAll();
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();
			if(tvLower.GetHorLength() > 0)
			{
				xyArrDim.Add(tvLower.m_v1);
				xyStt	= tvLower.GetXyLeft();
			}
			if(tvHunch.GetHorLength() > 0)
			{
				xyArrDim.Add(tvHunch.m_v1);
				xyArrDim.Add(tvHunch.m_v2);
				xyStt	= tvHunch.GetXyLeft();
			}

			xyArrDim.Sort(TRUE);
			
			sDir	= _T("BOTTOM");
			baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE);
			*pDomP << Dom;
		}
		return;
	}

	// 치수기입 시작위치는 기초가 있을 경우 기초의 확대기초를 기준으로 한다.
	CFootingApp *pFooting = (pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)? &pBri->m_footingBox : &pBri->m_footingEnd;
	CExFootingApp *pExFooting	= &pFooting->m_exFooting;

	CDRect rect	= pExFooting->m_tvArrFront.GetRect();
	
	if(pExFooting->m_nType == EXFOOTING_TYPE_MASS)
	{
		CString str		= _T("");
		double dH		= pExFooting->GetMaxHeight();
		double dHCur	= 0;
		long i = 0; for(i = 0; i < pExFooting->m_nCountDan; i++)
		{
			str.Format("EH%d", i+1);
			dArrDim.Add(pExFooting->m_dEFH[i]);
			sArrText.Add(str);
			sArrUnderText.Add("");
			dHCur	+= pExFooting->m_dEFH[i];
		}

		dArrDim.Add(dH-dHCur);
		sArrText.Add("0");
		sArrUnderText.Add("");
	}
	else
	{
		double dH	= rect.top - rect.bottom;
		dArrDim.Add(dH);
		sArrText.Add("EH");
		sArrUnderText.Add(COMMA(dH));
	}
	xyStt = CDPoint(rect.right, rect.bottom);
	
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		if(pBri->m_bExistProtectionRiseFooting)
		{
			dArrDim.Add(pBri->m_dPRF_H1);
			sArrText.Add("H1");
			sArrUnderText.Add(COMMA(pBri->m_dPRF_H1));
			dArrDim.Add(pBri->m_dPRF_H2);
			sArrText.Add("H2");
			sArrUnderText.Add(COMMA(pBri->m_dPRF_H2));
		}

		xySttTot.y = pBri->m_dELSB;
	}
	else
	{
		pFooting->m_tvArrFront.GetTvByInfo("기초우측면", tv);
		dArrDim.Add(tv.GetVerLength());
		if(pFooting->m_nType != FOOTING_TYPE_BASIC_LEFTSLOPE)
		{
			sArrText.Add("FH");
			sArrUnderText.Add(COMMA(tv.GetVerLength()));
		}
		else
		{
			sArrText.Add("0");
			sArrUnderText.Add("0");
		}
		xySttTot.y = tv.GetXyBottom().y;
	}

	pTvArr->GetTvByInfo("종점벽체우측면", tv);
	dArrDim.Add(tv.GetVerLength());
	sArrText.Add("0");
	sArrUnderText.Add("0");

	pTvArr->GetTvByInfo("종점브라켓헌치", tv);
	dH = tv.GetVerLength();
	if(!bInput)
	{
		dArrDim.Add(dH);
		sArrText.Add("0");
		sArrUnderText.Add(COMMA(dH));
	}
	pTvArr->GetTvByInfo("종점브라켓측면", tv);
	if(bInput)
		dH += tv.GetVerLength();
	else
		dH = tv.GetVerLength();
	dArrDim.Add(dH);
	sArrText.Add("BH");
	sArrUnderText.Add(COMMA(dH));

	pTvArr->GetTvByInfo("종점브라켓상부측면", tv);
	dArrDim.Add(tv.GetVerLength());
	sArrText.Add("BTH");
	sArrUnderText.Add(COMMA(tv.GetVerLength()));

	// 포장 두께
	if(!bForOutBM)
	{
		dArrDim.Add(pBri->GetThickPave(FALSE));
		sArrText.Add("0");
		sArrUnderText.Add("0");
	}
	xyEndTot.y = tv.m_v1.y;

	if(!bInput)
	{
		sArrText.RemoveAll();
		sArrUnderText.RemoveAll();
	}
	CHgBaseDrawStd baseDraw;
	baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE);

	// 전체 치수
	{
		// 교량측면 치수
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		m_pStd->ApplyDimSetting(pDomP, &Dom);
		Dom.SetDirection(sDir);
		nDimDan++;
		xyStt.y = xySttTot.y;
		double dist = xyEndTot.y-xySttTot.y; 
		if(dist > 0)
		{
			Dom.DimMoveTo(xyStt.x, xyStt.y, nDimDan);
			Dom.DimLineTo(dist, nDimDan, COMMA(dist));
		}
	}

	*pDomP << Dom;
}

// EL 기입
// bOnlyJijum : 시종점 및 교각위치만 기입
// 포장두께 고려 해서 포장두께만큼 위에 기입해야 정상적으로 나옴.
void CDrawVertSection::DimTextVerSectionEL(CDomyun *pDomP, BOOL bOnlyJijum)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;

	CConcSymbol sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDomP, &Dom);
	

	double dTP	= pBri->GetThickPave(FALSE);
	CString sEL = _T("");
	CDPoint xyStt(0, 0);
	CTwinVector tv;

	// 각 지점별 EL
	BOOL bLeft			= TRUE;
	long nCountJijum	= pBri->GetCountJijum();
	for(long jijum = 0; jijum < nCountJijum; jijum++)
	{
		if(jijum == 0) bLeft = FALSE;
		else if(jijum == nCountJijum-1) bLeft = TRUE;
		else bLeft	= FALSE;
		if(bOnlyJijum) 	bLeft = (jijum==0) ? TRUE : FALSE;

		xyStt.x	= pBri->GetStationOnJijum(jijum, 0);
		xyStt.y	= pBri->GetXyMatchLineAndSlabUpper(xyStt, CDPoint(0, 1), 0, 0).y + dTP;
		sEL		= m_pStd->GetStringEL(toM(xyStt.y));
		sym.ArrowElevation(&Dom, sEL, "", xyStt, bLeft, TRUE);
	}

	if(!bOnlyJijum)
	{
		// 시 종점 벽체 끝 상단 EL
		if(pBri->IsOutWall(TRUE))
		{
			xyStt.x	= pBri->GetLineBase()->GetStationByOffsetWidth(pBri->GetStationOnJijum(0, 0), -pBri->m_dWS, pBri->GetAngleJijum(0));
			xyStt.y	= pBri->GetXyMatchLineAndSlabUpper(xyStt, CDPoint(0, 1), 0, 0).y + dTP;
			sEL		= m_pStd->GetStringEL(toM(xyStt.y));
			sym.ArrowElevation(&Dom, sEL, "", xyStt, TRUE, TRUE);
		}

		if(pBri->IsOutWall(FALSE))
		{
			xyStt.x	= pBri->GetLineBase()->GetStationByOffsetWidth(pBri->GetStationOnJijum(nCountJijum-1, 0), pBri->m_dWE, pBri->GetAngleJijum(pBri->GetCountJijum()-1));
			xyStt.y	= pBri->GetXyMatchLineAndSlabUpper(xyStt, CDPoint(0, 1), 0, 0).y + dTP;
			sEL		= m_pStd->GetStringEL(toM(xyStt.y));
			sym.ArrowElevation(&Dom, sEL, "", xyStt, FALSE, TRUE);
		}
	}

	*pDomP << Dom;
}

//부상방지저판
void CDrawVertSection::DimVertSectionPRF(CDomyun *pDomp, BOOL bInput)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;
	CDomyun Dom(pDomp);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	if(pBri->m_nTypeBridge != BRIDGE_TYPE_BOX) return;
	if(pBri->m_bExistProtectionRiseFooting == FALSE) return;

	CLineInfo* pLine = pBri->GetLineBase();
	CDPoint xyStt(0,0), xyPRF(0,0);
	CDPoint tv;
	CString sText(_T("")), sUnderText(_T(""));
	double dStaJijum = pBri->GetStationOnJijum(0);
	CDPoint vAngJijum = pBri->GetAngleJijum(0);

	//벽체 기준점
	xyStt.x	= pLine->GetStationByOffsetWidth(dStaJijum, -pBri->m_dWS, vAngJijum);
	xyStt.y	= pBri->m_dELSB;

	// 부상방지저판 끝단
	double dGiulJong = pBri->m_bSlopeLowerSlab ?  pLine->GetJongDanGiul(xyStt.x) : 0.0;
	xyPRF.x = xyStt.x - pBri->m_dPRF_W / vAngJijum.y;
	xyPRF.y = xyStt.y - pBri->m_dPRF_W / vAngJijum.y * dGiulJong;

	if(pBri->m_dPRF_W > 0)
	{
		if(bInput){ sText = _T("brW"); sUnderText = COMMA(pBri->m_dPRF_W);}
		else{sText = COMMA(pBri->m_dPRF_W); sUnderText = _T("");}

		Dom.SetDirection("TOP");
		tv = xyStt + (pBri->m_dPRF_H1 + pBri->m_dPRF_H2) * CDPoint(0,1);
		Dom.DimMoveTo(tv.x, tv.y, 0);
		Dom.DimLineToExtend(-pBri->m_dPRF_W / vAngJijum.y, 0, sText, sUnderText);
	}

	*pDomp << Dom;
}

// 종단면 기초 상면
// void CDrawVertSection::DimVerSectionFootingUpper(CDomyun *pDomP, long nDimDan, BOOL bStt, BOOL bInput)
// {
// 	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
// 	if(!pBri) return;
// 	if(!pBri->IsOutWall(bStt)) return;
// 	if(!bStt && pBri->m_nTypeBridge == BRIDGE_TYPE_BOX) return;
// 
// 	CDomyun Dom(pDomP);
// 	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
// 	
// 	if(nDimDan < 0) nDimDan = 0;
// 	CTwinVector tv;
// 	CTwinVectorArray *pTvArr = &pBri->m_tvArrVertSection;
// 
// 	CDoubleArray dArrDim;
// 	CStringArray sArrText;
// 	CStringArray sArrUnderText;
// 	CString sDir = _T("TOP");
// 	CDPoint xyStt(0, 0);
// 
// 	CString str = bStt ? "시점" : "종점";
// 	pTvArr->GetTvByInfo(str+"기초좌측상면", tv);
// 	dArrDim.Add(tv.GetHorLength());
// 	sArrText.Add("FL");
// 	sArrUnderText.Add(COMMA(tv.GetHorLength()));
// 	
// 	xyStt = tv.m_v1;
// 
// 	dArrDim.Add(bStt ? pBri->m_dWS : pBri->m_dWE);
// 	sArrText.Add("W");
// 	sArrUnderText.Add(COMMA(bStt ? pBri->m_dWS : pBri->m_dWE));
// 
// 
// 	pTvArr->GetTvByInfo(str+"기초우측상면", tv);
// 	dArrDim.Add(tv.GetHorLength());
// 	sArrText.Add("FR");
// 	sArrUnderText.Add(COMMA(tv.GetHorLength()));
// 	
// 	if(bStt && pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
// 	{
// 		pTvArr->GetTvByInfo("기초상면", tv);
// 		dArrDim.Add(tv.GetHorLength());
// 		sArrText.Add("0");
// 		sArrUnderText.Add("0");
// 
// 		dArrDim.Add(pBri->m_dWE);
// 		sArrText.Add("0");
// 		sArrUnderText.Add("0");
// 
// 		pTvArr->GetTvByInfo("종점기초우측상면", tv);
// 		dArrDim.Add(tv.GetHorLength());
// 		sArrText.Add("FR");
// 		sArrUnderText.Add(COMMA(tv.GetHorLength()));
// 	}
// 
// 	if(!bInput)
// 	{
// 		sArrText.RemoveAll();
// 		sArrUnderText.RemoveAll();
// 	}
// 	CHgBaseDrawStd baseDraw;
// 	baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE);
// 
// 	*pDomP << Dom;
// 		
// }

// 외측벽체 헌치
void CDrawVertSection::DimVerSectionHunch(CDomyun *pDomP, long nDimDan, BOOL bStt, BOOL bInput, BOOL bUpper)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;
	if(bUpper == FALSE && pBri->m_nTypeBridge != BRIDGE_TYPE_BOX) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDomP, &Dom);
	
	if(nDimDan < 0) nDimDan = 0;
	CString sStt = bStt ? "시점" : "종점";
	CString sHunch = bUpper? "벽체헌치" : "벽체하부헌치";
	CTwinVectorArray *pTvArr = &pBri->m_tvArrVertSection;
	CTwinVector tv;
	CString sText = _T("");
	CString sUnderText = _T("");
		

	if(pTvArr->GetTvByInfo(sStt+sHunch, tv))
	{
		if(Compare(tv.GetHorLength(), 0.0, "<=") || Compare(tv.GetVerLength(), 0.0, "<=")) return;

		BOOL bApplyAngle	= tv.m_bApplyAngle;
		double rad			= fabs(sin(ToRadian(pBri->GetAngleJijum(bStt ? 0 : pBri->m_nQtyJigan).GetAngleDegree()-180)));
		if(!bApplyAngle || Compare(rad, 0.0, "=")) 
			rad	= 1;
		double dR			= tv.m_v1.z == 0 ? 0 : tv.m_v2.z;

		CDPoint xyStt = tv.GetXyMid();
		
		if(bStt)
		{
			if(Compare(dR, 0.0, "<="))
			{
				if(bInput)
				{
					sText.Format(bUpper? "HHxHW":"LHHxLHW");
					sUnderText.Format("(%sx%s)", COMMA(bUpper? pBri->m_dHHS : pBri->m_dWSHH), COMMA((bUpper? pBri->m_dHWS:pBri->m_dWSHW)/rad));
				}
				else
				{
					sText.Format("%sx%s", COMMA(bUpper? pBri->m_dHHS : pBri->m_dWSHH), COMMA((bUpper? pBri->m_dHWS:pBri->m_dWSHW)/rad));
					sUnderText.Format("(%sx%s)", COMMA(bUpper? pBri->m_dHHS : pBri->m_dWSHH), COMMA(bUpper? pBri->m_dHWS:pBri->m_dWSHW));
				}
			}
			else
			{
				sText	= bInput ? "HR" : "R = " + COMMA(pBri->m_dHRS);
				sUnderText	= bInput ? "R = " + COMMA(pBri->m_dHRS) : "0";
			}
		}
		else
		{
			if(Compare(dR, 0.0, "<="))
			{
				if(bInput)
				{
					sText.Format(bUpper? "HHxHW":"LHHxLHW");
					sUnderText.Format("(%sx%s)", COMMA(bUpper? pBri->m_dHHE:pBri->m_dWEHH), COMMA((bUpper? pBri->m_dHWE:pBri->m_dWEHW)/rad));
				}
				else
				{
					sText.Format("%sx%s", COMMA(bUpper? pBri->m_dHHE:pBri->m_dWEHH), COMMA((bUpper? pBri->m_dHWE:pBri->m_dWEHW)/rad));
					sUnderText.Format("(%sx%s)", COMMA(bUpper? pBri->m_dHHE:pBri->m_dWEHH), COMMA(bUpper? pBri->m_dHWE:pBri->m_dWEHW));
				}
			}
			else
			{
				sText	= bInput ? "HR" : "R = " + COMMA(pBri->m_dHRE);
				sUnderText	= bInput ? "R = " + COMMA(pBri->m_dHRE) : "0";
			}
		}
		// 모두 직교일 경우 괄호안 치수는 의미가 없다. //////////
		if(!bInput && !m_pStd->IsDimByAngleLen())
			sUnderText	= _T("");
		///////////////////////////////////////////////////////////

		double dDis1 = max(Dom.GetTextWidth(sUnderText), Dom.GetTextWidth(sText));

		if(Compare(dR, 0.0, "<="))
		{
			if(bUpper)
				Dom.GiSiArrowExtend(xyStt.x, xyStt.y, Dom.GetTextHeight()*3, bStt ? dDis1 : -dDis1, bStt ? 7 : 5, sText, sUnderText);
			else
				Dom.GiSiArrowExtend(xyStt.x, xyStt.y, Dom.GetTextHeight()*3, bStt ? dDis1 : -dDis1, bStt ? 1 : 3, sText, sUnderText);
		}
		else
		{
			CRebarPlacing rb;
			CDPoint xyCen	= rb.GetXyCircleCenter(tv.m_v1, tv.m_v2, tv.m_v2.z, tv.m_v1.z == -1);
			double dAngStt	= (tv.m_v1 - xyCen).GetAngleDegree();
			double dAngEnd	= (tv.m_v2 - xyCen).GetAngleDegree();
			double dAngMid	= (dAngEnd+dAngStt)/2;

			CDPointArray xyArrMatch;
			rb.GetXyMatchLineAndArc2(xyCen, ToDPointFrDegree(dAngMid), tv.m_v1, tv.m_v2, tv.m_v2.z, tv.m_v1.z==-1, TRUE, xyArrMatch, TRUE);
			if(xyArrMatch.GetSize() > 0)
			{
				xyStt	= xyArrMatch.GetAt(0);
			}

			Dom.GiSiArrowExtend(xyStt.x, xyStt.y, Dom.GetTextHeight()*3, bStt ? dDis1 : -dDis1, bStt ? 0 : 4, sText, sUnderText);
			Dom.Rotate(xyStt, ToDPointFrDegree(bStt ? (dAngMid-180) : dAngMid));
		}
	}

	*pDomP << Dom;

	// 경사교2 인 경우 벽체하부헌치도 있다.
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 || pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		CString sStt = bStt ? "시점" : "종점";
		CTwinVectorArray *pTvArr = &pBri->m_tvArrVertSection;
		CTwinVector tv;
		CString sText = _T("");
		CString sUnderText = _T("");
			

		if(pTvArr->GetTvByInfo(sStt+"벽체하부헌치", tv))
		{
			if(Compare(tv.GetHorLength(), 0.0, "<=") || Compare(tv.GetVerLength(), 0.0, "<=")) return;

			BOOL bApplyAngle	= tv.m_bApplyAngle;
			double rad			= fabs(sin(ToRadian(pBri->GetAngleJijum(bStt ? 0 : pBri->m_nQtyJigan).GetAngleDegree()-180)));
			if(!bApplyAngle || Compare(rad, 0.0, "=")) 
				rad	= 1;

			CDPoint xyStt = tv.GetXyMid();
			
			if(bStt) 
			{
				if(bInput)
				{
					sText.Format("WHHxWHW");
					sUnderText.Format("(%sx%s)", COMMA(pBri->m_dWSHH), COMMA(pBri->m_dWSHW/rad));
				}
				else
				{
					sText.Format("%sx%s", COMMA(pBri->m_dWSHH), COMMA(pBri->m_dWSHW/rad));
					sUnderText.Format("(%sx%s)", COMMA(pBri->m_dWSHH), COMMA(pBri->m_dWSHW));
				}
			}
			else
			{
				if(bInput)
				{
					sText.Format("WHHxWHW");
					sUnderText.Format("(%sx%s)", COMMA(pBri->m_dWEHH), COMMA(pBri->m_dWEHW/rad));
				}
				else
				{
					sText.Format("%sx%s", COMMA(pBri->m_dWEHH), COMMA(pBri->m_dWEHW/rad));
					sUnderText.Format("(%sx%s)", COMMA(pBri->m_dWEHH), COMMA(pBri->m_dWEHW));
				}
			}
			// 모두 직교일 경우 괄호안 치수는 의미가 없다. //////////
			if(!bInput && !m_pStd->IsDimByAngleLen())
				sUnderText	= _T("");
			///////////////////////////////////////////////////////////

			double dDis1 = max(Dom.GetTextWidth(sUnderText), Dom.GetTextWidth(sText));

			CRebarPlacing rb;
			CDPoint xyCen	= rb.GetXyCircleCenter(tv.m_v1, tv.m_v2, tv.m_v2.z, tv.m_v1.z == -1);
			double dAngStt	= (tv.m_v1 - xyCen).GetAngleDegree();
			double dAngEnd	= (tv.m_v2 - xyCen).GetAngleDegree();
			double dAngMid	= (dAngEnd+dAngStt)/2;

			CDPointArray xyArrMatch;
			rb.GetXyMatchLineAndArc2(xyCen, ToDPointFrDegree(dAngMid), tv.m_v1, tv.m_v2, tv.m_v2.z, tv.m_v1.z==-1, TRUE, xyArrMatch, TRUE);
			if(xyArrMatch.GetSize() > 0)
			{
				xyStt	= xyArrMatch.GetAt(0);
			}

			Dom.GiSiArrowExtend(xyStt.x, xyStt.y, Dom.GetTextHeight()*3, bStt ? dDis1 : -dDis1, bStt ? 1 : 3, sText, sUnderText);
		}
	}

	*pDomP << Dom;
}

// 종단선형 그리기
void CDrawVertSection::DrawElEarthLine(CDomyun *pDomP, double dStationStt, double dStationEnd,BOOL bDrawBoring, BOOL bBridgeZone, BOOL bMarkGround, double dStaMarkG, HGINT32 ixJijum)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;
	CLineInfoApp *pLine = m_pStd->m_pARcBridgeDataStd->GetLineApp();
	if(!pLine) return;
	
	CConcSymbol sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_BARC);
	Dom.SetLineColor(6);

	CTwinVectorArray tvArrElEarth;
	pBri->GetTvElEarthLine(tvArrElEarth,dStationStt,dStationEnd,bBridgeZone);
	m_pStd->DrawTvArray(&Dom, tvArrElEarth);

	if(bMarkGround && dStaMarkG > dStationStt && dStationEnd > dStaMarkG)
	{
		CDPoint xyP(dStaMarkG, 0);
		CTwinVector tvEarth;
		long nSize = tvArrElEarth.GetSize();
		long n=0; for(n=0; n<nSize; n++)
		{
			tvEarth = tvArrElEarth.GetAt(n);
			if(tvEarth.m_v1.x < dStaMarkG && tvEarth.m_v2.x > dStaMarkG) break;
		}
		CDPoint vTemp = tvEarth.GetXyDir();
		double dAngDeg = vTemp.GetAngleDegree();
		GetXyMatchSegAndLine(tvEarth.m_v1, tvEarth.m_v2, xyP, CDPoint(0, 1), xyP);
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_STLC);
		CHgBaseDrawStd::InsertHCadBlock(&Dom, "CGEARTH", xyP, Dom.GetScaleDim(), dAngDeg);
	}
	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();
	
	*pDomP << Dom;

	// 보링데이타 출력
	// 지반선 범위내에 있는것만 출력한다.
	// 전에는 위의 조건(지반선 범위내에 있는것만 출력한다.)이었으나
	// 멀리 떨어져 있어도 계산에서 적용은된다.
	// 따라서 멀리 떨어져 있어도 도면출력할때도 출력이 되어야 한다.
	//보링공 draw(tab별)
	if(bDrawBoring)
	{
		// 여러지점에서 같은 지질주상도를 선택했을때 중첩해서 그려지는걸 막는다.
		CStringArray sArrDrawBoring;

		long nCountJijum(ixJijum + 1);
		if( ixJijum == -1 )
		{
			ixJijum = 0;
			nCountJijum	= pBri->GetCountJijum();
		}

		for(long j = ixJijum; j < nCountJijum; j++)
		{
			CBoringDataEx *pBoringEx	= pBri->GetBoringDataExByJijumNumber(j);
			if(!pBoringEx) 
				continue;

			BOOL bSkip(FALSE);
			for(long ix=0; ix<sArrDrawBoring.GetSize(); ++ix)
			{
				if(sArrDrawBoring.GetAt(ix) == pBoringEx->m_sNameBoring)
				{
					bSkip = TRUE;
					break;
				}
			}
			if(bSkip)
				continue;

			sArrDrawBoring.Add(pBoringEx->m_sNameBoring);

			double dSta	= frM(pBoringEx->m_dStation);
			sym.DrawBoringSymbol(&Dom, &pBoringEx->m_pArrBoring, pBoringEx->m_EL, dSta, 1, FALSE, 0, TRUE, pBoringEx->m_sNameBoring);
				
			//Station은 미터로 입력되고 밀리미터로 처리된다. EL은 미터로 입력되고 처리되므로 변환해준다.
			Dom.Move(dSta, frM(pBoringEx->m_EL));				
			*pDomP << Dom;
		}	
	}
}

// 외측벽체 브라켓 헌치 치수 기입
void CDrawVertSection::DimVerSectionBracketHunch(CDomyun *pDomP, long nDimDan, BOOL bStt, BOOL bInput)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;
	if(!pBri->IsOutWall(bStt)) return;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDomP, &Dom);

	if(nDimDan < 0) nDimDan = 0;
	
	CTwinVector tv;
	CTwinVectorArray *pTvArr = &pBri->m_tvArrVertSection;

	CDoubleArray dArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("BOTTOM");
	CDPoint xyStt(0, 0);
	CString str = bStt ? "시점" : "종점";

	BOOL bApplyAngle	= pBri->IsApplyAngle();
	pTvArr->GetTvByInfo(str+"브라켓헌치", tv);
	
	// 가로
	dArrDim.Add(tv.GetHorLength(bApplyAngle));
	if(bInput)
	{
		sArrText.Add("BHW");
		sArrUnderText.Add(COMMA3(tv.GetHorLength(TRUE), tv.GetHorLength(FALSE)));
	}
	else
	{
		sArrText.Add(COMMA(tv.GetHorLength(bApplyAngle)));
		sArrUnderText.Add(COMMA2(tv.GetHorLength(FALSE)));
	}
	xyStt = tv.m_v2;
	xyStt.y = min(tv.m_v1.y, tv.m_v2.y);

	// 모두 직교일 경우 괄호안 치수는 의미가 없다. //////////
	m_pStd->ModifyInfoAutoDimConc(sArrText, sArrUnderText, bInput);
	///////////////////////////////////////////////////////////

	CHgBaseDrawStd baseDraw;
	baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE, bStt);

	// 세로
	dArrDim.RemoveAll();
	sArrText.RemoveAll();
	sArrUnderText.RemoveAll();

	sDir = bStt ? _T("LEFT") : _T("RIGHT");
	dArrDim.Add(tv.GetVerLength());
	sArrText.Add("BHH");
	sArrUnderText.Add(COMMA(tv.GetVerLength()));
	xyStt = bStt ? tv.m_v1 : tv.m_v2;
	xyStt.x = bStt ? min(tv.m_v1.x, tv.m_v2.x) : max(tv.m_v1.x, tv.m_v2.x);

	if(!bInput)
	{
		sArrText.RemoveAll();
		sArrUnderText.RemoveAll();
	}

	baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE);
	*pDomP << Dom;
}

void CDrawVertSection::DimVerSectionInHunch(CDomyun *pDomP, long nDimDan, long nIdxHunch, BOOL bInput, BOOL bUpper)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDomP, &Dom);

	if(nDimDan < 0) nDimDan = 0;
	
	CTwinVector tvLeft, tvRight, tv;
	CTwinVectorArray *pTvArr = &pBri->m_tvArrVertSection;

	CDoubleArray dArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = bUpper? _T("BOTTOM") : _T("TOP");
	CDPoint xyStt(0, 0);
	CString str	= _T("");
	str.Format("%d번째내측%s헌치", nIdxHunch+1, bUpper? _T("") : _T("하부"));

	long nIdxInWall		= 0;
	BOOL bLeft			= TRUE;
	pBri->GetInfoHunch(nIdxHunch, nIdxInWall, bLeft);

	// 가로치수
	if(bUpper)
	{
		pTvArr->GetTvByInfo(str+"아래면좌측", tvLeft);
		pTvArr->GetTvByInfo(str+"아래면우측", tvRight);
	}
	else
	{
		pTvArr->GetTvByInfo(str+"윗면좌측", tvLeft);
		pTvArr->GetTvByInfo(str+"윗면우측", tvRight);
	}

	BOOL bApplyAngle	= tvLeft.m_bApplyAngle;
	double rad			= fabs(sin(ToRadian(pBri->GetAngleJijum(nIdxInWall+1).GetAngleDegree()-180)));
	if(!bApplyAngle || Compare(rad, 0.0, "=")) 
		rad	= 1;

	// 슬래브일때 신축이음을 대비해서 신축이음 고려한 위치와 길이로 치수를 기입한다.
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB)
	{
		CDPoint xyOrg(0, 0);
		CJijum *pJ = pBri->GetJijum(nIdxHunch+1);
		double dUganLeft = 0;
		double dUganRight = 0;
		if(pBri->m_bExpJoint[nIdxHunch+1])
		{
			dUganLeft = pJ->m_dLengthUgan[0];
			dUganRight = pJ->m_dLengthUgan[1];
		}

		pTvArr->GetTvByInfo(str+"아래면좌측", tvLeft);
		pTvArr->GetTvByInfo(str+"아래면우측", tvRight);
		tvLeft.m_v1.x += dUganLeft;
		tvRight.m_v2.x -= dUganRight;
	}

	if(bInput)
	{
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		{
			double dW		= tvLeft.GetHorLength(bApplyAngle) + tvRight.GetHorLength(bApplyAngle);
			double dWFix	= tvLeft.GetHorLength(FALSE) + tvRight.GetHorLength(FALSE);
			dArrDim.Add(dW);
			if(bInput)
			{
				sArrText.Add("W");
				sArrUnderText.Add(COMMA3(dW, dWFix));
			}
			else
			{
				sArrText.Add(COMMA(dW));
				sArrUnderText.Add(COMMA2(dWFix));
			}
			xyStt = tvLeft.m_v2;
		}
		else
		{
			// 아랫면 좌측 치수
			dArrDim.Add(tvLeft.GetHorLength(bApplyAngle));
			if(bInput && pBri->m_nTypeBridge != BRIDGE_TYPE_TRAP && pBri->m_nTypeBridge != BRIDGE_TYPE_TRAP2)
			{
				sArrText.Add(bUpper? _T("WL") : _T("LWL"));
				sArrUnderText.Add(COMMA3(tvLeft.GetHorLength(TRUE), tvLeft.GetHorLength(FALSE)));
			}
			else
			{
				sArrText.Add(COMMA(tvLeft.GetHorLength(bApplyAngle)));
				sArrUnderText.Add(COMMA2(tvLeft.GetHorLength(FALSE)));
			}
			xyStt = bUpper? tvLeft.m_v2 : tvLeft.m_v1;

			// 아랫면 우측 치수
			dArrDim.Add(tvRight.GetHorLength(bApplyAngle));
			if(bInput && pBri->m_nTypeBridge != BRIDGE_TYPE_TRAP && pBri->m_nTypeBridge != BRIDGE_TYPE_TRAP2)
			{
				sArrText.Add(bUpper? _T("WR") : _T("LWR"));
				sArrUnderText.Add(COMMA3(tvRight.GetHorLength(TRUE), tvRight.GetHorLength(FALSE)));
			}
			else
			{
				sArrText.Add(COMMA(tvRight.GetHorLength(bApplyAngle)));
				sArrUnderText.Add(COMMA2(tvRight.GetHorLength(FALSE)));	
			}
		}

		// 모두 직교일 경우 괄호안 치수는 의미가 없다. //////////
		m_pStd->ModifyInfoAutoDimConc(sArrText, sArrUnderText, bInput);
		///////////////////////////////////////////////////////////

		CHgBaseDrawStd baseDraw;
		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE);
		*pDomP << Dom;
	}


	// 좌측헌치 지시선
	CString sText = _T("");
	CString sUnderText = _T("");
	CString sTextR = _T("");
	CString sUnderTextR = _T("");
	if(pBri->m_dHLW[nIdxHunch][bUpper? iUPPER:iLOWER] != 0 || pBri->m_dHHL[nIdxHunch][bUpper? iUPPER:iLOWER] != 0)
	{
		pTvArr->GetTvByInfo(str+"좌측면", tv);
		xyStt = tv.GetXyMid();
		double dR	= tv.m_v1.z == 0 ? 0 : tv.m_v2.z;

		if(bInput)
		{
			sText.Format(bUpper? _T("HHLxHLW") : _T("LLHxLLW"));
			sUnderText.Format("(%sx%s)", COMMA(pBri->m_dHHL[nIdxHunch][bUpper? iUPPER:iLOWER]), COMMA(pBri->m_dHLW[nIdxHunch][bUpper? iUPPER:iLOWER]/rad));
		}
		else
		{
			sText.Format("%sx%s", COMMA(pBri->m_dHHL[nIdxHunch][bUpper? iUPPER:iLOWER]), COMMA(pBri->m_dHLW[nIdxHunch][bUpper? iUPPER:iLOWER]/rad));
			sUnderText.Format("(%sx%s)", COMMA(pBri->m_dHHL[nIdxHunch][bUpper? iUPPER:iLOWER]), COMMA(pBri->m_dHLW[nIdxHunch][bUpper? iUPPER:iLOWER]));
		}

		if(dR != 0)
		{
			sTextR	= bInput ? (bUpper? _T("HLR") : _T("LLR")) : "R = " + COMMA(pBri->m_dHLR[nIdxHunch][bUpper? iUPPER:iLOWER]);
			sUnderTextR	= bInput ? "R = " + COMMA(pBri->m_dHLR[nIdxHunch][bUpper? iUPPER:iLOWER]) : "0";
		}

		// 모두 직교일 경우 괄호안 치수는 의미가 없다. //////////
		if(!bInput && !m_pStd->IsDimByAngleLen())
		{
			sUnderText	= _T("");
			sUnderTextR	= _T("");
		}
		///////////////////////////////////////////////////////////

		double dDis1 = max(Dom.GetTextWidth(sText), Dom.GetTextWidth(sUnderText));

		if(Compare(dR, 0.0, "<="))
		{
			Dom.GiSiArrowExtend(xyStt.x, xyStt.y, Dom.GetTextHeight()*3, -dDis1, bUpper? 5:3, sText, sUnderText);
		}
		else
		{
			Dom.SetDirection("TOP");
			if(!bInput)
				sUnderTextR	= "";

			CRebarPlacing rb;
			CDPoint xyCen	= rb.GetXyCircleCenter(tv.m_v1, tv.m_v2, tv.m_v2.z, tv.m_v1.z == -1);
			double dAngStt	= (tv.m_v1 - xyCen).GetAngleDegree();
			double dAngEnd	= (tv.m_v2 - xyCen).GetAngleDegree();
			if(dAngStt > dAngEnd)
				dAngEnd += 360;
			double dAngMid	= (dAngEnd+dAngStt)/2;

			CDPointArray xyArrMatch;
			rb.GetXyMatchLineAndArc2(xyCen, ToDPointFrDegree(dAngMid), tv.m_v1, tv.m_v2, tv.m_v2.z, tv.m_v1.z==-1, TRUE, xyArrMatch, TRUE);
			if(xyArrMatch.GetSize() > 0)
			{
				xyStt	= xyArrMatch.GetAt(0);
			}

			Dom.GiSiArrowExtend(xyStt.x, xyStt.y, Dom.GetTextHeight()*3, -dDis1, 4, sTextR, sUnderTextR);
			Dom.Rotate(xyStt, ToDPointFrDegree(dAngMid));
			*pDomP << Dom;


			// 가로
			if(pBri->m_nTypeRoundOfHunchInWall[nIdxInWall][iLEFT] != 0)
			{
				Dom.SetDirection("BOTTOM");
				Dom.DimMoveTo(tv.GetXyLeft().x, tv.GetXyBottom().y, 0);
				if(sUnderText == "")
					Dom.DimLineTo(tv.GetHorLength(bApplyAngle), 0, sText);
				else
					Dom.DimLineToExtend(tv.GetHorLength(bApplyAngle), 0, sText, sUnderText);

				// 세로
				Dom.SetDirection("LEFT");
				Dom.DimMoveTo(tv.GetXyLeft().x, tv.GetXyBottom().y, 0);
				if(sUnderText == "")
					Dom.DimLineTo(tv.GetVerLength(), 0, sText);
				else
					Dom.DimLineToExtend(tv.GetVerLength(), 0, sText, sUnderText);

				*pDomP << Dom;
			}
			
		}
	}

	// 우측헌치 지시선
	if(pBri->m_dHRW[nIdxHunch][bUpper? iUPPER:iLOWER] != 0 || pBri->m_dHHR[nIdxHunch][bUpper? iUPPER:iLOWER] != 0)
	{
		pTvArr->GetTvByInfo(str+"우측면", tv);
		xyStt = tv.GetXyMid();
		double dR	= tv.m_v1.z == 0 ? 0 : tv.m_v2.z;

		if(bInput)
		{
			sText.Format(bUpper? _T("HHRxHRW") : _T("LRHxLRW"));
			sUnderText.Format("(%sx%s)", COMMA(pBri->m_dHHR[nIdxHunch][bUpper? iUPPER:iLOWER]), COMMA(pBri->m_dHRW[nIdxHunch][bUpper? iUPPER:iLOWER]/rad));
		}
		else
		{
			sText.Format("%sx%s", COMMA(pBri->m_dHHR[nIdxHunch][bUpper? iUPPER:iLOWER]), COMMA(pBri->m_dHRW[nIdxHunch][bUpper? iUPPER:iLOWER]/rad));
			sUnderText.Format("(%sx%s)", COMMA(pBri->m_dHHR[nIdxHunch][bUpper? iUPPER:iLOWER]), COMMA(pBri->m_dHRW[nIdxHunch][bUpper? iUPPER:iLOWER]));
		}
		
		if(Compare(dR, 0.0, ">="))
		{
			// 하단 헌치가 있을때는 R을 입력할수 없으니 하단헌치 고려 안해도 됨.
			sTextR	= bInput ? "HRR" : "R = " + COMMA(pBri->m_dHRR[nIdxHunch][iUPPER]);
			sUnderTextR	= bInput ? "R = " + COMMA(pBri->m_dHRR[nIdxHunch][iUPPER]) : "0";
		}

		// 모두 직교일 경우 괄호안 치수는 의미가 없다. //////////
		if(!bInput && !m_pStd->IsDimByAngleLen())
		{
			sUnderText	= _T("");
			sUnderTextR	= _T("");
		}
		///////////////////////////////////////////////////////////


		double dDis1 = max(Dom.GetTextWidth(sText), Dom.GetTextWidth(sUnderText));

		if(Compare(dR, 0.0, "<="))
		{
			Dom.GiSiArrowExtend(xyStt.x, xyStt.y, Dom.GetTextHeight()*3, dDis1, bUpper? 7:1, sText, sUnderText);
		}
		else
		{
			Dom.SetDirection("TOP");
			if(!bInput)
				sUnderTextR	= "";
			CRebarPlacing rb;
			CDPoint xyCen	= rb.GetXyCircleCenter(tv.m_v1, tv.m_v2, tv.m_v2.z, tv.m_v1.z == -1);
			double dAngStt	= (tv.m_v1 - xyCen).GetAngleDegree();
			double dAngEnd	= (tv.m_v2 - xyCen).GetAngleDegree();
			if(dAngEnd < dAngStt)
				dAngEnd += 360;
			double dAngMid	= (dAngEnd+dAngStt)/2;

			CDPointArray xyArrMatch;
			rb.GetXyMatchLineAndArc2(xyCen, ToDPointFrDegree(dAngMid), tv.m_v1, tv.m_v2, tv.m_v2.z, tv.m_v1.z==-1, TRUE, xyArrMatch, TRUE);
			if(xyArrMatch.GetSize() > 0)
			{
				xyStt	= xyArrMatch.GetAt(0);
			}

			Dom.GiSiArrowExtend(xyStt.x, xyStt.y, Dom.GetTextHeight()*3, dDis1, 0, sTextR, sUnderTextR);
			Dom.Rotate(xyStt, ToDPointFrDegree(dAngMid-180));
			*pDomP << Dom;

			// 가로
			if(pBri->m_nTypeRoundOfHunchInWall[nIdxInWall][iRIGHT] != 0)
			{
				Dom.SetDirection("BOTTOM");
				Dom.DimMoveTo(tv.GetXyLeft().x, tv.GetXyBottom().y, 0);
				if(sUnderText == "")
					Dom.DimLineTo(tv.GetHorLength(bApplyAngle), 0, sText);
				else
					Dom.DimLineToExtend(tv.GetHorLength(bApplyAngle), 0, sText, sUnderText);

				// 세로
				Dom.SetDirection("RIGHT");
				Dom.DimMoveTo(tv.GetXyRight().x, tv.GetXyBottom().y, 0);
				if(sUnderText == "")
					Dom.DimLineTo(tv.GetVerLength(), 0, sText);
				else
					Dom.DimLineToExtend(tv.GetVerLength(), 0, sText, sUnderText);
				*pDomP << Dom;
			}
		}
	}

	// 헌치아래수직면 치수
	if(pBri->m_dHBH[nIdxHunch][bUpper? iUPPER:iLOWER] != 0)
	{
		str.Format("%d번째내측%s좌측면", nIdxHunch+1, bUpper? _T("헌치아래") : _T("하부헌치위"));
		CTwinVector tv;
		if(pTvArr->GetTvByInfo(str, tv))
		{
			str.Format(bUpper? _T("HB") : _T("LHB"));
			Dom.SetDirection("LEFT");
			Dom.SetDimObq(-30);
			Dom.DimMoveTo(tv.m_v1.x, tv.m_v1.y, nDimDan);
			if(bInput)
				Dom.DimLineToExtend(tv.GetVerLength(), nDimDan, str, COMMA(tv.GetVerLength()));
			else
				Dom.DimLineTo(tv.GetVerLength(), nDimDan, COMMA(tv.GetVerLength()));
		}
	}

	// 신축이음에 대한 치수기입 
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB)
	{
		CString sLeftSide	= _T("");
		CString sRightSide	= _T("");
		CTwinVector tvLeftSide, tvRightSide;

		sLeftSide.Format("%d번째내측헌치신축이음좌측", nIdxHunch+1);
		sRightSide.Format("%d번째내측헌치신축이음우측", nIdxHunch+1);

		if(pTvArr->GetTvByInfo(sLeftSide, tvLeftSide) && pTvArr->GetTvByInfo(sRightSide, tvRightSide))
		{
			dArrDim.RemoveAll();
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();

			dArrDim.Add(tvRightSide.GetXyTop().x-tvLeftSide.GetXyTop().x);
			sDir = _T("TOP");
			xyStt	= tvLeftSide.GetXyTop();

			CHgBaseDrawStd baseDraw;
			baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE);
		}
	}

	*pDomP << Dom;
}

// 상부 슬래브 주철근 그리기
// long nVertDir : 이미 철근은 제대로 만들어 졌으므로 피복 치수 기입시에만 고려 하면 됨
void CDrawVertSection::DrawRebarMain_VertSection_UpperSlab(CDomyun *pDomP, long nCycle, long nDan, BOOL bDrawJoint, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft, CDRect rectLimit, BOOL bInput, long nVertDir)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(nCycle == -1) nCycle = pBri->GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > pBri->GetCountCycleMainRebar()-1) return;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;

	BOOL bFindOwner	= pBri->m_pARcBridgeDataStd->m_nTypeProcess==TYPE_PROCESS_SLAB_STANDARD_OUTPUT;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	// 상부 슬래브
	// 상면 ///////////////////////////////////////////////////////////////////////
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &pBri->m_pArrRCMainUpperSlab[nCycle][iUPPER][nDan];
	// RebarInfoCycle 그리기
	m_pStd->DrawRebarInfoCycleArray(&Dom, pArrRC, nDrawLeft, rectLimit, !bInput);

	// 이음 그리기
	if(bDrawJoint)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);

		// 이음 정보 가져옴
		CJointArray arrJoint;
		pBri->MakeJointMainRebarUpperSlab(arrJoint, nCycle, nDan, TRUE);
		
		// 이음 그리기
		m_pStd->DrawJointArray(&Dom, arrJoint, nDrawLeft, rectLimit);
	}

	// 치수기입
	if(bDimRebar)
	{
		DimRebarMain_VertSection_UpperSlab_Upper_Cycle(&Dom, bMarkRebar ? 1 : 0, TRUE, nCycle, nVertDir, nDan);
	}

	// 철근 마크
	if(bMarkRebar)
	{
		double dScalePos	= 0.5;
		switch(nDan)
		{
			case 0: dScalePos	= 0.5; break;
			case 1: dScalePos	= 0.2; break;
			case 2: dScalePos	= 0.8; break;
		}
		
		m_pStd->MarkRebarInfoCycleArray(&Dom, pArrRC, TRUE, FALSE, pBri->m_dCoverUpper_UpperSlab[nDan], 2, 0, nDrawLeft, CDRect(0, 0, 0, 0), dScalePos, bInput, &pBri->m_pArrRebarMain, NULL, bFindOwner);
	}



	// 하면 /////////////////////////////////////////////////////////////////////
	pArrRC = &pBri->m_pArrRCMainUpperSlab[nCycle][iLOWER][nDan];

	// RebarInfoCycle 그리기
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	m_pStd->DrawRebarInfoCycleArray(&Dom, pArrRC, nDrawLeft, rectLimit);

	// 이음 그리기
	if(bDrawJoint)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);

		// 이음 정보 가져옴
		CJointArray arrJoint;
		pBri->MakeJointMainRebarUpperSlab(arrJoint, nCycle, nDan, FALSE);
		
		// 이음 그리기
		m_pStd->DrawJointArray(&Dom, arrJoint, nDrawLeft, rectLimit);	
	}

	// 치수기입
	if(bDimRebar)
		DimRebarMain_VertSection_UpperSlab_Lower_Cycle1(&Dom, bMarkRebar ? 1 : 0, TRUE, nCycle, nVertDir, nDan);

	// 철근 마크
	if(bMarkRebar)
	{
		double dScalePos	= 0.5;
		switch(nDan)
		{
			case 0: dScalePos	= 0.5; break;
			case 1: dScalePos	= 0.2; break;
			case 2: dScalePos	= 0.8; break;
		}
		m_pStd->MarkRebarInfoCycleArray(&Dom, pArrRC, FALSE, FALSE, pBri->m_dCoverLower_UpperSlab[nDan], 2, 0, nDrawLeft, CDRect(0, 0, 0, 0), dScalePos, bInput, &pBri->m_pArrRebarMain, NULL, bFindOwner);
	}

	*pDomP << Dom;
}

// 리브 주철근 그리기
// long nVertDir : 이미 철근은 제대로 만들어 졌으므로 피복 치수 기입시에만 고려 하면 됨
void CDrawVertSection::DrawRebarMain_VertSection_ArchRib(CDomyun *pDomP, long nCycle, long nDan, BOOL bDrawJoint, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft, CDRect rectLimit, BOOL bInput, long nVertDir)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(nCycle == -1) nCycle = pBri->GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > pBri->GetCountCycleMainRebar()-1) return;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	// 리브
	for(long upper = 0; upper < 2; upper++)
	{
		BOOL bUpper	= upper == iUPPER;

		CParabola pb;
		double dCover	= bUpper ? -pBri->m_dCoverRib[upper][nDan] : pBri->m_dCoverRib[upper][nDan];
		pBri->m_pbRib[upper].GetOffsetParabola(pb, dCover);

		CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &pBri->m_pArrRCMainRib[nCycle][upper][nDan];
		// RebarInfoCycle 그리기
		m_pStd->DrawRebarInfoCycleArray(&Dom, pArrRC, nDrawLeft, rectLimit, TRUE, &pb);

		// 이음 그리기
		if(bDrawJoint)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);

			// 이음 정보 가져옴
			CJointArray arrJoint;
			pBri->MakeJointMainRebarRib(arrJoint, nCycle, nDan, bUpper);
			
			// 이음 그리기
			m_pStd->DrawJointArray(&Dom, arrJoint, nDrawLeft, rectLimit);
		}

		// 치수기입
		if(bDimRebar)
		{
			DimRebarMain_VertSection_ArchRib(&Dom, nCycle, bUpper, nDan, 0, bInput, nVertDir);
		}

		// 철근 마크
		if(bMarkRebar)
		{
			double dScalePos	= 0.5;
			switch(nDan)
			{
				case 0: dScalePos	= 0.5; break;
				case 1: dScalePos	= 0.2; break;
				case 2: dScalePos	= 0.8; break;
			}
			CParabola pbRebar;
			pBri->GetRebarRib(pbRebar, bUpper, nDan);
			m_pStd->MarkRebarInfoCycleArray(&Dom, pArrRC, bUpper, FALSE, pBri->m_dCoverUpper_UpperSlab[nDan], 2, 0, nDrawLeft, CDRect(0, 0, 0, 0), dScalePos, bInput, &pBri->m_pArrRebarMain, &pbRebar);
		}
	}


	*pDomP << Dom;
}

// BOOL bForStandardSection : 표준단면도용인지?(기초에 들어가는 벽체 철근 R 표시 안함)
void CDrawVertSection::DrawRebarMain_VertSection_OutWall(CDomyun *pDomP, long nCycle, long nDan, BOOL bDrawJoint, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft, CDRect rectLimit, BOOL bInput, BOOL bForStandardSection, long nVertDir)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(nCycle == -1) nCycle = pBri->GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > pBri->GetCountCycleMainRebar()-1) return;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;
	
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	// 시점측 벽체
	BOOL	bStt	= TRUE,
			bInner	= TRUE;
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		bStt	= (stt == iSTT);
		for(long inner = 0; inner < 2; inner++)
		{
			bInner	= (inner == iINNER);
			CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &pBri->m_pArrRCMainOutWall[nCycle][stt][inner][nDan];

			// RebarInfoCycle 그리기
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
			m_pStd->DrawRebarInfoCycleArray(&Dom, pArrRC, nDrawLeft, rectLimit, !bInput);

			// 이음 그리기
			if(bDrawJoint)
			{
				m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);

				// 이음 정보 가져옴
				CJointArray arrJoint;
				pBri->MakeJointMainRebarOutWall(arrJoint, nCycle, nDan, bStt, bInner);
				m_pStd->DrawJointArray(&Dom, arrJoint, nDrawLeft, rectLimit);
			}

			// 철근 마크
			if(bMarkRebar)
			{
				*pDomP << Dom;
				// 마크방향(시점기준)
				BOOL bLeft	= (inner==iINNER) ? FALSE : TRUE;
				if(stt == iEND) bLeft = !bLeft;	// 종점 기준.
				double dScalePos	= 0.5;
				switch(nDan)
				{
					case 0: dScalePos	= 0.5; break;
					case 1: dScalePos	= 0.2; break;
					case 2: dScalePos	= 0.8; break;
				}
				m_pStd->MarkRebarInfoCycleArray(&Dom, pArrRC, bLeft, TRUE, (inner==iINNER) ? pBri->m_dCoverWallOut[nDan] : pBri->m_dCoverWallIn[nDan], 2, 0, nDrawLeft, rectLimit, dScalePos, bInput, &pBri->m_pArrRebarMain);

				// 기초에 들어가는 철근중 R이 있으면 R표기 해준다
				if(pArrRC->GetSize() > 0 && !bForStandardSection)
				{
					CRebarInfoCycle *pRC	= pArrRC->GetAt(0);
					m_pStd->MarkRadiusRebarInfoCycle(&Dom, pRC, rectLimit);
				}
			}
		}

		// 치수 기입
		if(bDimRebar)
			DimRebarMain_VertSection_OutWall_Cycle(&Dom, 1, TRUE, stt==iSTT, nCycle, nVertDir, nDan);
	}


	*pDomP << Dom;
}

// 상부슬래브 1단, 1,2사이클, 상면 주철근 치수
void CDrawVertSection::DimRebarMain_VertSection_UpperSlab_Upper_Cycle(CDomyun *pDomP, long nDimDan, BOOL bInput, long nCycle, long nVertDir, long nDan)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(nCycle == -1) nCycle = pBri->GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > pBri->GetCountCycleMainRebar()-1) return;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	BOOL bVertDir	= pBri->IsVertDir();


	// 치수기입용 변수
	CDPointArray xyArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("TOP");
	CDPoint xyStt(0, 0);

	// 치수정보 분석해서 가져옴
	BOOL bApplyEndIs2	= FALSE;
	CDPoint vAngStt			= bVertDir ? CDPoint(0, 1) : pBri->GetAngleJijum(0);
	CDPoint vAngEnd			= bVertDir ? CDPoint(0, 1) : pBri->GetAngleJijum(pBri->m_nQtyJigan);
	double dCoverSideStt	= pBri->GetLengthBynVertDir(pBri->m_dCoverWallOut[0] / vAngStt.y, nVertDir);
	double dCoverSideEnd	= pBri->GetLengthBynVertDir(pBri->m_dCoverWallOut[0] / vAngEnd.y, nVertDir);
	if(pBri->IsBlockOut(TRUE))
	{
		dCoverSideStt	+= pBri->GetLengthBynVertDir(pBri->m_fxJointStt.m_dWidth / vAngStt.y, nVertDir);
	}
	if(pBri->IsBlockOut(FALSE))
	{
		dCoverSideEnd	+= pBri->GetLengthBynVertDir(pBri->m_fxJointEnd.m_dWidth / vAngEnd.y, nVertDir);
	}

	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &pBri->m_pArrRCMainUpperSlab[nCycle][iUPPER][nDan];

	// 예외사항 처리
	// 1. 시점외측벽체와 상부슬래브가 연결된 형태인 경우 첫번째 철근은 연결 안된 형태로 변형해서 치수 기입한다.
	CRebarInfoCycle rcFirstUpperSlab;
	CRebarInfoCycle *pRCFirstUpperSlab	= NULL;
	if(pArrRC->GetSize() > 0)
	{
		pRCFirstUpperSlab	= pArrRC->GetAt(0);
		rcFirstUpperSlab	= *pRCFirstUpperSlab;
		BOOL bSttRebar	= TRUE;
		if(pBri->IsMainRebarJointUpperSlabAndOutWall(pRCFirstUpperSlab, bSttRebar))
		{
			if(bSttRebar)
			{
				pRCFirstUpperSlab->m_xyStt		= pRCFirstUpperSlab->m_xyMid2;
				pRCFirstUpperSlab->m_xyMid1		= pRCFirstUpperSlab->m_xyEnd;
				pRCFirstUpperSlab->m_xyMid2		= pRCFirstUpperSlab->m_xyMidExt;
				pRCFirstUpperSlab->m_xyEnd		= pRCFirstUpperSlab->m_xyEndExt;
				pRCFirstUpperSlab->m_xyCen		= pRCFirstUpperSlab->m_xyCenExt;
				pRCFirstUpperSlab->m_bUseCenExt	= FALSE;
				pRCFirstUpperSlab->m_bUseCenExt	= FALSE;
				pRCFirstUpperSlab->m_bUseCenExt	= FALSE;
				pRCFirstUpperSlab->m_nPosStt	= REBARINFO_POS_STTWALL_OUTTER;
			}
		}
	}


	if(1)//nCycle == 0 || nCycle == 2)
	{
		bApplyEndIs2	= TRUE;
		m_pStd->GetDimInfoAtRebarInfoCycle(pArrRC, TRUE, xyArrDim, sArrText, FALSE, TRUE, bApplyEndIs2, FALSE, dCoverSideStt, dCoverSideEnd, nVertDir);
	}
	else
	{
		if(pBri->IsJointMainRebarUpperSlab_Upper_Cycle2(-1)) bApplyEndIs2 = TRUE;

		m_pStd->GetDimInfoAtRebarInfoCycle(pArrRC, TRUE, xyArrDim, sArrText, FALSE, FALSE, bApplyEndIs2, FALSE, dCoverSideStt, dCoverSideEnd, nVertDir);
	}

	if(xyArrDim.GetSize() > 0)
	{
		// 가져온 정보를 입력에 맞게 기호로 변경
		if(bInput)
		{
			// 지간 정보 삽입
			long nType	= 0;
			InsertJijumInfo(xyArrDim, sArrText, nType, 0, TRUE, bVertDir);

			// 입력정보로 변경
			long nSize	= sArrText.GetSize();
			CString str	= _T("");
			long nNum	= 1;
			sArrUnderText.Add("");
			long i = 0; for(i = 1; i < nSize-1; i++)
			{
				if(sArrText.GetAt(i) == "1")
				{
					str.Format("UD%d", nNum++);
					sArrText.SetAt(i, str);
					if(bInput)
						sArrUnderText.Add(COMMA2(xyArrDim.GetAt(i).x-xyArrDim.GetAt(i-1).x));
				}
				else
				{
					sArrText.SetAt(i, "0");
					sArrUnderText.Add("");
				}
			}
		}
		else
		{
			sArrText.RemoveAll();
		}

		CDPoint xyStt  = xyArrDim.GetAt(0);
		for(long ix=1; ix<xyArrDim.GetSize(); ix++)
		{
			xyStt.y = max(xyStt.y, xyArrDim.GetAt(ix).y);
		}
		
		CHgBaseDrawStd baseDraw;
		baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, TRUE);
	}


	// 치수기입을 위해서 변형했던거 복구.
	if(pArrRC->GetSize() > 0 && pRCFirstUpperSlab)
	{
		*pRCFirstUpperSlab = rcFirstUpperSlab;
	}

	*pDomP << Dom;
}

// 상부슬래브 1단, 1사이클, 상면 주철근 치수
void CDrawVertSection::DimRebarMain_VertSection_UpperSlab_Lower_Cycle1(CDomyun *pDomP, long nDimDan, BOOL bInput, long nCycle, long nVertDir, long nDan)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(nCycle == -1) nCycle = pBri->GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > pBri->GetCountCycleMainRebar()-1) return;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	BOOL bVertDir	= pBri->IsVertDir();


	// 치수기입용 변수
	CDPointArray xyArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("BOTTOM");
	CDPoint xyStt(0, 0);

	// 치수정보 분석해서 가져옴
	BOOL bReverse = FALSE;
	CDPoint vAngStt			= bVertDir ? CDPoint(0, 1) : pBri->GetAngleJijum(0);
	CDPoint vAngEnd			= bVertDir ? CDPoint(0, 1) : pBri->GetAngleJijum(pBri->m_nQtyJigan);
	double dCoverSideStt	= pBri->GetLengthBynVertDir(pBri->m_dCoverWallOut[0] / vAngStt.y, nVertDir);
	double dCoverSideEnd	= pBri->GetLengthBynVertDir(pBri->m_dCoverWallOut[0] / vAngEnd.y, nVertDir);
	m_pStd->GetDimInfoAtRebarInfoCycle(&pBri->m_pArrRCMainUpperSlab[nCycle][iLOWER][nDan], TRUE, xyArrDim, sArrText, FALSE, TRUE, TRUE, bReverse, dCoverSideStt, dCoverSideEnd, nVertDir);

		
	if(xyArrDim.GetSize() > 0)
	{
		// 2사이클과 4사이클은 시작과 끝에 좌표를 하나 더 넣어 줘야 된다.
		// 왜? 빈공간이라서 치수정보에서 시작과 끝위치가 빠져버리기 때문이지...
		if(0)//nCycle == 1 || nCycle == 3)
		{
			// 피복 정보 제거후 
			xyArrDim.RemoveAt(0);
			xyArrDim.RemoveAt(xyArrDim.GetSize()-1);
			sArrText.RemoveAt(0);
			sArrText.RemoveAt(sArrText.GetSize()-1);


			CTwinVector tvSlab	= pBri->GetTvOffset_UpperSlab(0, FALSE, -1, -1, bVertDir);

			// 시작위치
			xyArrDim.InsertAt(0, tvSlab.m_v1);
			sArrText.InsertAt(0, "0");
			sArrText.SetAt(1, "1");

			// 끝위치
			xyArrDim.Add(tvSlab.m_v2);
			sArrText.Add("1");

			// 피복 정보 추가한다.
			// 처음과 끝부분에 피복 정보 추가한다. ////////////////
			CDPoint xy1	= xyArrDim.GetAt(0);
			CDPoint xy2	= xyArrDim.GetAt(xyArrDim.GetSize()-1);

			xy1 = xy1 + (xy1-xy2).Unit() * dCoverSideStt;
			xyArrDim.InsertAt(0, xy1);
			sArrText.InsertAt(0, "0");

			xy2	= xy2 + (xy2-xy1).Unit() * dCoverSideEnd;
			xyArrDim.Add(xy2);
			sArrText.Add("0");
			////////////////////////////////////////////////////////			
		}


		// 가져온 정보를 입력에 맞게 기호로 변경
		if(bInput)
		{
			// 지간 정보 삽입
			long nType	= 0;
			InsertJijumInfo(xyArrDim, sArrText, nType, nCycle, FALSE, bVertDir);

			// 입력정보로 변경
			long nSize	= sArrText.GetSize();
			CString str	= _T("");
			long nNum	= 1;
			sArrUnderText.Add("");
			long i = 0; for(i = 1; i < nSize-1; i++)
			{
				if(sArrText.GetAt(i) == "1")
				{
					str.Format("LD%d", nNum++);
					sArrText.SetAt(i, str);
					sArrUnderText.Add(COMMA2(xyArrDim[i].x-xyArrDim[i-1].x));
				}
				else
				{
					sArrText.SetAt(i, "0");
					sArrUnderText.Add("");
				}
			}
		}
		else
		{
			sArrText.RemoveAll();
		}

		CDPoint xyStt  = xyArrDim.GetAt(0);
		for(long ix=1; ix<xyArrDim.GetSize(); ix++)
		{
			xyStt.y = min(xyStt.y, xyArrDim.GetAt(ix).y);
		}

		CHgBaseDrawStd baseDraw;
		baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, TRUE);
	}


	// 시작과 끝 철근은 신축이음에 의한 치수 기입 함.
		// 시종점 신축이음 때문에 시점은 시작점이 mid보다 우측으로 간 경우
	// 종점은 끝점이 mid보다 좌측으로 간 경우가 있다. 이거 무조건 mid에 붙임 
	long i = 0; for(i = 0; i < pBri->m_pArrRCMainUpperSlab[nCycle][iLOWER][nDan].GetSize(); i++)
	{
		CRebarInfoCycle *pRC	= pBri->m_pArrRCMainUpperSlab[nCycle][iLOWER][nDan].GetAt(i);
		if(pBri->IsBlockOut(TRUE) && pRC->m_nPosStt == REBARINFO_POS_UPPERSLAB_SIDE && pRC->m_nPosEnd	== REBARINFO_POS_UPPERSLAB_LOWER)
		{
			Dom.DimMoveTo(pRC->m_xyMid1.x, pRC->m_xyMid1.y, 0);
			if(bInput)
				Dom.DimLineToExtend(pRC->GetLengthStt(), 0, "FXSL", COMMA(pRC->GetLengthStt()));
			else
				Dom.DimLineTo(pRC->GetLengthStt(), 0, COMMA(pRC->GetLengthStt()));
		}
		else if(pBri->IsBlockOut(FALSE) && pRC->m_nPosStt == REBARINFO_POS_UPPERSLAB_LOWER && pRC->m_nPosEnd == REBARINFO_POS_UPPERSLAB_SIDE)
		{
			Dom.DimMoveTo(pRC->m_xyEnd.x, pRC->m_xyEnd.y, 0);
			if(bInput)
				Dom.DimLineToExtend(pRC->GetLengthEnd(), 0, "FXEL", COMMA(pRC->GetLengthEnd()));
			else
				Dom.DimLineTo(pRC->GetLengthEnd(), 0, COMMA(pRC->GetLengthEnd()));
		}
	}

	*pDomP << Dom;
}

// 시, 종점 외측벽체 치수 기입
void CDrawVertSection::DimRebarMain_VertSection_OutWall_Cycle(CDomyun *pDomP, long nDimDan, BOOL bInput, BOOL bStt, long nCycle, long nVertDir, long nDan)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(nCycle == -1) nCycle = pBri->GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > pBri->GetCountCycleMainRebar()-1) return;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	BOOL bVertDir	= pBri->IsVertDir();
	long stt		= bStt ? iSTT : iEND;
	CString sNum	= bStt ? "S" : "E";
	long nNum		= 1;

	// 치수기입용 변수
	CDPointArray xyArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("");
	CDPoint xyStt(0, 0);

	// 외측
	sDir = bStt ? _T("LEFT") : _T("RIGHT");
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &pBri->m_pArrRCMainOutWall[nCycle][stt][iOUTTER][nDan];

	if(pArrRC)
	{
		// 벽체 외측 치수기입시 상부슬래브의 첫번째 철근을 가져와서 임시로 붙여 준다.
		// (이유:치수기입시 외측벽체와 상부슬래브가 연결되기 때문에...)
		CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRCSlab	= &pBri->m_pArrRCMainUpperSlab[nCycle][iUPPER][nDan];

		if(pArrRCSlab)
		{
			long nSizeSlab	= pArrRCSlab->GetSize();
			if(nSizeSlab > 0)
			{
				CRebarInfoCycle	*pRC = new CRebarInfoCycle;
				*pRC = *pArrRCSlab->GetAt(bStt ? 0 : nSizeSlab-1);

				// 외측벽체와 상부슬래브가 연결된 형태인 경우 두개로 쪼개서 사용한다.
				if(0)//pRC->m_bUseEndExt)
				{
					if(bStt)
					{
						if(pBri->IsBoxType() == FALSE)
						{
							// 기초하면부터 외측벽체까지.
							CRebarInfoCycle *pRC1	= new CRebarInfoCycle;
							*pRC1	= *pRC;
							pRC1->m_bUseCenExt	= FALSE;
							pRC1->m_bUseEndExt	= FALSE;
							pRC1->m_bUseMidExt	= FALSE;
							pRC1->m_nPosEnd		= REBARINFO_POS_STTWALL_OUTTER;
							pArrRC->Add(pRC1);
						}
					
						// 외측벽체에서 상부슬래브까지.
						
						pRC->m_xyStt		= pRC->m_xyEnd;
						pRC->m_xyMid1		= pRC->m_xyEnd;
						pRC->m_xyMid2		= pRC->m_xyMid2;
						pRC->m_xyEnd		= pRC->m_xyEndExt;
						pRC->m_xyCen		= pRC->m_xyCenExt;
						pRC->m_nPosStt		= REBARINFO_POS_STTWALL_OUTTER;
						pRC->m_bUseCenExt	= FALSE;
						pRC->m_bUseEndExt	= FALSE;
						pRC->m_bUseMidExt	= FALSE;
						pArrRC->Add(pRC);
					}
					else
					{
						if(pBri->IsBoxType() == FALSE)
						{
							// 기초하면부터 외측벽체까지.
							CRebarInfoCycle *pRC1	= new CRebarInfoCycle;
							*pRC1	= *pRC;
							pRC1->m_bUseCenExt	= FALSE;
							pRC1->m_bUseEndExt	= FALSE;
							pRC1->m_bUseMidExt	= FALSE;
							pRC1->m_nPosStt		= REBARINFO_POS_FOOTING_LOWER;
							pRC1->m_xyStt		= pRC->m_xyEndExt;
							pRC1->m_xyMid1		= pRC->m_xyMidExt;
							pRC1->m_xyMid2		= pRC->m_xyEnd;
							pRC1->m_xyEnd		= pRC->m_xyMid2;
							pRC1->m_xyCen		= pRC->m_xyCenExt;
							pArrRC->Add(pRC1);
						}

						// 외측벽체에서 상부슬래브까지.
						CRebarInfoCycle rcTmp;
						rcTmp	= *pRC;
						pRC->m_xyEnd		= rcTmp.m_xyStt;
						pRC->m_xyMid2		= rcTmp.m_xyMid1;
						pRC->m_xyMid1		= rcTmp.m_xyMid2;
						pRC->m_xyStt		= rcTmp.m_xyEnd;
						pRC->m_nPosStt		= REBARINFO_POS_ENDWALL_OUTTER;
						pRC->m_nPosEnd		= REBARINFO_POS_UPPERSLAB_UPPER;
						pRC->m_bUseCenExt	= FALSE;
						pRC->m_bUseEndExt	= FALSE;
						pRC->m_bUseMidExt	= FALSE;
						pArrRC->Add(pRC);
					}
				}
				else
				{
					if(!bStt) pBri->m_Util.InvertRebarInfoCycle(pRC);
					
					// 상부슬래브 R의 높이는 고정되어야 한다.
					// 사거리와 직거리로 높이가 변화되게 만들어 져서 여기서 보정해줌
					double dHDiff	= pBri->GetLenRebarAtEdge(pRC->m_Dia, FALSE, 0, TRUE) - pBri->GetLenRebarAtEdge(pRC->m_Dia, FALSE, nVertDir, TRUE);
					pRC->m_xyStt.y -= dHDiff;
					pRC->m_xyMid1.y -= dHDiff;

					pArrRC->Add(pRC);
				}
			}
		}
		if(pBri->IsBoxType())
		{
			CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRCLowerSlab	= &pBri->m_pArrRCMainLowerSlab[nCycle][iLOWER][nDan];
			if(pArrRCLowerSlab)
			{
				long nSizeSlab	= pArrRCLowerSlab->GetSize();
				if(nSizeSlab > 0)
				{
					CRebarInfoCycle	*pRC = new CRebarInfoCycle;
					*pRC = *pArrRCLowerSlab->GetAt(bStt ? 0 : nSizeSlab-1);

					if(bStt)
					{
						// 하부 슬래브에서 벽체까지
						CRebarInfoCycle rcTmp;
						rcTmp	= *pRC;

						pRC->m_xyEnd		= rcTmp.m_xyStt;
						pRC->m_xyMid2		= rcTmp.m_xyMid1;
						pRC->m_xyMid1		= rcTmp.m_xyMid2;
						pRC->m_xyStt		= rcTmp.m_xyEnd;
						pRC->m_nPosStt		= REBARINFO_POS_STTWALL_OUTTER;
						pRC->m_bUseCenExt	= FALSE;
						pRC->m_bUseEndExt	= FALSE;
						pRC->m_bUseMidExt	= FALSE;
						pArrRC->InsertAt(0, pRC);
					}
					else
					{
						pRC->m_xyStt		= pRC->m_xyStt;
						pRC->m_xyMid1		= pRC->m_xyMid1;
						pRC->m_xyMid2		= pRC->m_xyMid2;
						pRC->m_xyEnd		= pRC->m_xyEnd;
						pRC->m_xyCen		= pRC->m_xyEnd;
						pRC->m_nPosStt		= REBARINFO_POS_STTWALL_OUTTER;
						pRC->m_bUseCenExt	= FALSE;
						pRC->m_bUseEndExt	= FALSE;
						pRC->m_bUseMidExt	= FALSE;
						pArrRC->InsertAt(0, pRC);
					}
				}
			}
		}

		// 치수정보 분석해서 가져옴
		CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;
		double dCoverStt	= pFooting->m_dCoverLower[0];
		if(pBri->IsBoxType()) dCoverStt	= pBri->m_dCoverLower_LowerSlab[0];
		double dCoverEnd	= pBri->m_dCoverUpper_UpperSlab[0];
		BOOL bApplyEndIs2	= TRUE;//(nCycle == 0 || nCycle == 2) ? TRUE : FALSE;
		BOOL bApplyJoint	= bApplyEndIs2;
		m_pStd->GetDimInfoAtRebarInfoCycle(pArrRC, TRUE, xyArrDim, sArrText, FALSE, bApplyJoint, bApplyEndIs2, FALSE, dCoverStt, dCoverEnd);

		if(pArrRC->GetSize() > 0)
		{
			CRebarInfoCycle *pRC = pArrRC->GetAt(pArrRC->GetSize()-1);
			delete pRC;
			pArrRC->RemoveAt(pArrRC->GetSize()-1);
		}
		if(pBri->IsBoxType() && pArrRC->GetSize() > 0)
		{
			CRebarInfoCycle *pRC = pArrRC->GetAt(0);
			delete pRC;
			pArrRC->RemoveAt(0);
		}

		if(xyArrDim.GetSize() > 0)
		{
			// 가져온 정보를 입력에 맞게 기호로 변경
			if(bInput)
			{
				long nSize	= sArrText.GetSize();
				CString str	= _T("");
				sArrUnderText.Add("");
				long i = 0; for(i = 1; i < nSize-1; i++)
				{
					if(sArrText.GetAt(i) == "1")
					{
						str.Format("%sD%d", sNum, nNum++);
						sArrText.SetAt(i, str);
						sArrUnderText.Add(COMMA2(~(xyArrDim[i]-xyArrDim[i-1])));
					}
					else
					{
						sArrText.SetAt(i, "0");
						sArrUnderText.Add("");
					}
				}
			}
			else
			{
				sArrText.RemoveAll();
			}
			CHgBaseDrawStd baseDraw;
			baseDraw.AutoDimCont(&Dom, xyArrDim.GetAt(0), xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE);
		}
	}

	// 내측
	sDir = bStt ? _T("RIGHT") : _T("LEFT");
	pArrRC = &pBri->m_pArrRCMainOutWall[nCycle][stt][iINNER][nDan];

	if(pArrRC)
	{
		if(pArrRC->GetSize() > 0)
		{
			// 치수정보 분석해서 가져옴
			CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;
			double dCoverStt	= pFooting->m_dCoverLower[0];
			if(pBri->IsBoxType()) dCoverStt	= pBri->m_dCoverLower_LowerSlab[0];
			double dCoverEnd	= pBri->m_dCoverUpper_UpperSlab[0];
			BOOL bApplyEndIs2	= TRUE;//(nCycle == 0 || nCycle == 2) ? TRUE : FALSE;
			BOOL bApplyJoint	= bApplyEndIs2;
			BOOL bReverse		= !bApplyEndIs2;

			m_pStd->GetDimInfoAtRebarInfoCycle(pArrRC, TRUE, xyArrDim, sArrText, FALSE, bApplyJoint, bApplyEndIs2, bReverse, 0, 0);

			// 내측 2, 4 사이클은 기초하면과 슬래브 상면 좌표를 추가해줘야 된다.
			if(0)//nCycle == 1 || nCycle == 3)
			{
				CTwinVector tvWall	= pBri->GetTvOffset_OutWall(0, bStt, TRUE, bVertDir);

				// 시작위치 피복
				xyArrDim.InsertAt(0, CDPoint(tvWall.m_v1.x, tvWall.m_v1.y - dCoverStt));
				sArrText.InsertAt(0, "0");

				// 시작위치
				xyArrDim.InsertAt(1, tvWall.m_v1);
				sArrText.InsertAt(1, "0");

				sArrText.SetAt(2, "1");

				// 끝위치
				xyArrDim.Add(tvWall.m_v2);
				sArrText.Add("1");

				// 끝위치 피복
				xyArrDim.Add(CDPoint(tvWall.m_v1.x, tvWall.m_v2.y+dCoverEnd));
				sArrText.Add("0");
			}
			
			if(xyArrDim.GetSize() > 0)
			{
				// 가져온 정보를 입력에 맞게 기호로 변경
				if(bInput)
				{
					sArrUnderText.RemoveAll();
					long nSize	= sArrText.GetSize();
					CString str	= _T("");
					long i = 0; for(i = 0; i < nSize; i++)
					{
						if(sArrText.GetAt(i) == "1")
						{
							str.Format("%sD%d", sNum, nNum++);
							sArrText.SetAt(i, str);
							if(i > 0)
								sArrUnderText.Add(COMMA2(~(xyArrDim[i]-xyArrDim[i-1])));
						}
						else
						{
							sArrText.SetAt(i, "0");
							sArrUnderText.Add("");
						}
					}
				}
				else
				{
					sArrText.RemoveAll();
				}
				CHgBaseDrawStd baseDraw;
				baseDraw.AutoDimCont(&Dom, xyArrDim.GetAt(0), xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE);
			}
		}
	}

	*pDomP << Dom;
}

// rebarinfocycle을 분석한 치수기입 정보에 지점정보를 삽입한다.(지점에 걸리는 치수를 둘로 쪼갬)
// nType : 삽입방법 
// nType == 0 : 둘로 쪼갠것중 앞에것은 정보를 "2"로 만듬
// nType == 1 : 1. 그 지점이 연결된 것을 경우 쪼개지 않음(정보삽입안함) - nCycle bUpper 슬래브 정보를 참고
//              2. 쪼개더라도 앞에것을 "2"로 만들지 않음 "1" 그대로 둠. 

// nCycle : nType == 1 일때 참고 할 사이클 번호, nType == 0일때는 아무 소용 없음.
// bUpper : nType == 1 일때 참고 할 사이클이 상면인지 하면인지?
void CDrawVertSection::InsertJijumInfo(CDPointArray &xyArrDim, CStringArray &sArrText, long nType, long nCycle, BOOL bUpper, BOOL bVertDir)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	// 지간 정보를 배열에 모두 넣음 ////////////
	// 단 처음과 마지막 지점은 빼야된다.
	CDoubleArray dArrJijum;
	long i = 0; for(i = 1; i < pBri->m_nQtyJigan; i++)
	{
		dArrJijum.Add(pBri->GetStationOnJijum(i, 0, bVertDir));
	}
	// 각 지점을 지나가는 철근을 지점으로 나누어 줌.
	for(long jijum = 0; jijum < dArrJijum.GetSize(); jijum++)
	{
		double dJijum = dArrJijum.GetAt(jijum);

		// 지점을 지나가는 철근이 있는지 검사.
		BOOL bJoint	= bUpper 
			? pBri->IsJointMainRebarUpperSlab_Upper_Cycle2(jijum) : pBri->IsJointMainRebarUpperSlab_Lower_Cycle2(dJijum);
		long i = 0; for(i = 0; i < xyArrDim.GetSize()-1; i++)
		{
			CDPoint xy1 = xyArrDim.GetAt(i);
			CDPoint xy2 = xyArrDim.GetAt(i+1);
			if(nType == 1 && bJoint) continue;

			if(xy1.x < dJijum && xy2.x >= dJijum)
			{
				CString sInfo = sArrText.GetAt(i+1);
				xyArrDim.SetAt(i+1, CDPoint(dJijum, xy2.y));
				switch(nType)
				{
					case 0: sArrText.SetAt(i+1, "2"); break;
					case 1: sArrText.SetAt(i+1, "1"); break;
					default: break;
				}

				xyArrDim.InsertAt(i+2, xy2);
				sArrText.InsertAt(i+2, sInfo);
			}
		}
	}
	// 지간 정보를 배열에 모두 넣음 ////////////
}


// 마킹을 위로 할지 아래로 할지 판단하는 함수.
// BOOL CDrawVertSection::IsUpperMarkRebar(CRebarInfoCycle *pRC)
// {
// 	BOOL bUpper		= TRUE;
// 	long nPosStt	= pRC->m_nPosStt;
// 	long nPosEnd	= pRC->m_nPosEnd;
// 	// 시작이나 끝이 상부 슬래브 상면이면 무조건 위로
// 	if(nPosStt == REBARINFO_POS_UPPERSLAB_UPPER || nPosEnd == REBARINFO_POS_UPPERSLAB_LOWER)
// 	{
// 		bUpper	= TRUE;
// 	}
// 	// 시작이나 끝이 상부 슬래브 하면이면 무조건 아래로
// 	else if(nPosStt == REBARINFO_POS_UPPERSLAB_LOWER || nPosEnd == REBARINFO_POS_UPPERSLAB_LOWER)
// 	{
// 		bUpper	= FALSE;
// 	}
// 	// 시작이나 끝이 시점측 벽체 외측면이면 무조건 위로
// 	else if(nPosStt == REBARINFO_POS_STTWALL_OUTTER || nPosEnd == REBARINFO_POS_STTWALL_OUTTER)
// 	{
// 		bUpper	= TRUE;
// 	}
// 	// 시작이나 끝이 시점측 벽체 내측면이면 무조건 아래로.
// 	else if(nPosStt == REBARINFO_POS_STTWALL_INNER || nPosEnd == REBARINFO_POS_STTWALL_INNER)
// 	{
// 		bUpper	= FALSE;
// 	}
// 	// 시작이나 끝이 시점측 벽체 외측면이면 무조건 아래로
// 	else if(nPosStt == REBARINFO_POS_ENDWALL_OUTTER || nPosEnd == REBARINFO_POS_ENDWALL_OUTTER)
// 	{
// 		bUpper	= FALSE;
// 	}
// 	// 시작이나 끝이 시점측 벽체 내측면이면 무조건 위로.
// 	else if(nPosStt == REBARINFO_POS_ENDWALL_INNER || nPosEnd == REBARINFO_POS_ENDWALL_INNER)
// 	{
// 		bUpper	= TRUE;
// 	}
// 
// 	return bUpper;
// }

// 상부슬래브 헌치부 주철근 그리기
void CDrawVertSection::DrawRebarMain_VertSection_UpperSlabHunch(CDomyun *pDomP, long nCycle, BOOL bMarkRebar, long nDrawLeft, CDRect rectLimit, BOOL bInput)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	if(nCycle == -1) nCycle	= pBri->GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > pBri->GetCountCycleMainRebar()-1) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	// 상부 슬래브
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &pBri->m_pArrRCMainUpperSlabHunch[nCycle];
	if(!pArrRC) return;

	// RebarInfoCycle 그리기
	m_pStd->DrawRebarInfoCycleArray(&Dom, pArrRC, nDrawLeft, rectLimit);

	if(bMarkRebar)
	{
		m_pStd->MarkRebarInfoCycleArray(&Dom, pArrRC, FALSE, FALSE, pBri->m_dCoverLower_UpperSlab[0], 2, 0, nDrawLeft, CDRect(0, 0, 0, 0), 0.9, bInput, &pBri->m_pArrRebarMain);
	}


	*pDomP << Dom;
}

// 종단면도 모두 그리기.
// BOOL bPatternRoundRectWall : 라운드 벽체 무늬 그릴지?
// BOOL bApplyAngle : 파일 그릴때 각도 적용하는지?
CDRect CDrawVertSection::DrawVertSectionAll(CDomyun *pDomP, BOOL bDrawExFooting, BOOL bDrawPile, BOOL bDrawPave, BOOL bDrawConstJoint, BOOL bDrawCuttingPile, BOOL bPatternRoundRectWall, BOOL bApplyAngle, BOOL bFlexibleJoint)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return CDRect(0, 0, 0, 0);

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	// 종단면 그림
	CTwinVectorArray tvArrTmp;
	DrawVertSection(&Dom, bDrawPave, &staticTvArrReturnBridge, CDRect(0, 0, 0, 0), bDrawConstJoint);
	DrawVertSectionArchHunchHidden(&Dom);


	if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		if(bDrawExFooting)
			m_pStd->m_pDrawExFooting->DrawFront(&Dom, &pBri->m_footingBox.m_exFooting, FALSE, bDrawCuttingPile, &tvArrTmp, bDrawPile, bDrawPile, bApplyAngle);
		if(bDrawPile)
			DrawExFootingBoxType(&Dom, bDrawCuttingPile);	
	}
	else
	{
		m_pStd->m_pDrawFooting->DrawFront(&Dom, &pBri->m_footingStt, &staticTvArrReturnFooting, bDrawConstJoint);
		m_pStd->m_pDrawFooting->DrawFront(&Dom, &pBri->m_footingEnd, &staticTvArrReturnFooting, bDrawConstJoint);
		if(bDrawExFooting)
		{
			m_pStd->m_pDrawExFooting->DrawFront(&Dom, &pBri->m_footingStt.m_exFooting, FALSE, bDrawCuttingPile, &tvArrTmp, bDrawPile, bDrawPile, bApplyAngle);
			m_pStd->m_pDrawExFooting->DrawFront(&Dom, &pBri->m_footingEnd.m_exFooting, FALSE, bDrawCuttingPile, &tvArrTmp, bDrawPile, bDrawPile, bApplyAngle);
		}
	}
	
	long nCount = pBri->GetCountInWall();
	long i = 0; for(i = 0; i < nCount; i++)
	{
		CWallApp *pWall = pBri->GetInWall(i);
		if(!pWall) continue;
		if(!pWall->m_bIs) continue;

		BOOL bDrawPattern	= bPatternRoundRectWall && !pWall->m_bTopIsColumn || pWall->m_bTopIsColumn;
		m_pStd->m_pDrawWall->DrawFront(&Dom, pWall, bDrawPattern, &staticTvArrReturnInWall, bDrawConstJoint);
		m_pStd->m_pDrawFooting->DrawFront(&Dom, &pWall->m_footing, &staticTvArrReturnFooting, bDrawConstJoint);
		if(bDrawExFooting)
			m_pStd->m_pDrawExFooting->DrawFront(&Dom, &pWall->m_footing.m_exFooting, FALSE, bDrawCuttingPile, &tvArrTmp, bDrawPile, bDrawPile, bApplyAngle);
	}

	if(bFlexibleJoint)
	{
		long stt = 0; for(stt = 0; stt < 2; stt++)
		{
			DrawVertSectionFlexibleJoint(&Dom, stt == iSTT);
		}
	}


	*pDomP << Dom;

	Dom.SetCalcExtRect();
	return Dom.GetExtRect();
}

// 종단면 주철근 모두 그림'
// CRebar *pRB : 재호의 필살기..
// pRB 가 NULL인 경우 일반적으로 그냥 그린다.
// 그러나!!! pRB가 존재하는 경우, 이미 집계가 된 상황이라 판단하며..
// pRB와 RebarInfoCycle은 이미 기호가  match되어 있다...
// 그러므로 pRB와 RebarInfoCycle 의 기호를 비교해서 같은거만 그린다.
// 이거는 철근 집계 창에서 해당 철근만 보기위해서 그려준다.
void CDrawVertSection::DrawRebarMain_VertSectionAll(CDomyun *pDomP, long nCycle, BOOL bDrawJoint, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft, BOOL bInput, BOOL bForStandardSection)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	CDRect rectLimit	= CDRect(0, 0, 0, 0);
	long nCountWall = pBri->GetCountInWall();
	

	long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		DrawRebarMain_VertSection_UpperSlab(&Dom, nCycle, dan, bDrawJoint, bDimRebar, bMarkRebar, nDrawLeft, rectLimit, bInput);
		DrawRebarMain_VertSection_OutWall(&Dom, nCycle, dan, bDrawJoint, bDimRebar, bMarkRebar, nDrawLeft, rectLimit, bInput, bForStandardSection);
		DrawRebarMain_VertSection_ArchRib(&Dom, nCycle, dan, bDrawJoint, bDimRebar, bMarkRebar, nDrawLeft, rectLimit, bInput, 0);
		DrawRebarMain_VertSection_LowerSlab(&Dom, nCycle, dan, bDrawJoint, bDimRebar, bMarkRebar, nDrawLeft, rectLimit, bInput);
		
		// 시종점 기초
		for(long upper = 0; upper < 2; upper++)
		{
			m_pStd->m_pDrawFooting->DrawRebarMain_Front(&Dom, &pBri->m_footingStt, nCycle, dan, upper, bDimRebar, bMarkRebar, nDrawLeft, bInput);
			m_pStd->m_pDrawFooting->DrawRebarMain_Front(&Dom, &pBri->m_footingEnd, nCycle, dan, upper, bDimRebar, bMarkRebar, nDrawLeft, bInput);
		}
		m_pStd->m_pDrawFooting->DrawRebarMain_FrontSlope(&Dom, &pBri->m_footingStt, dan, bDimRebar, bMarkRebar&&dan==0, bInput);
		m_pStd->m_pDrawFooting->DrawRebarMain_FrontSlope(&Dom, &pBri->m_footingEnd, dan, bDimRebar, bMarkRebar&&dan==0, bInput);

		// 벽체들 철근 그리기.(벽체의 기초 포함)
		for(long wall = 0; wall < nCountWall; wall++)
		{
			CWallApp *pWall	= pBri->GetInWall(wall);
			if(pWall)
			{
				long left = 0; for(left = 0; left < 2; left++)
				{
					m_pStd->m_pDrawWall->DrawRebarMain_Front(&Dom, pWall, nCycle, dan, left, bMarkRebar, bDimRebar, nDrawLeft, TRUE, bInput, bForStandardSection);
				}
				for(long upper = 0; upper < 2; upper++)
				{
					m_pStd->m_pDrawFooting->DrawRebarMain_Front(&Dom, &pWall->m_footing, nCycle, dan, upper, bDimRebar, bMarkRebar, nDrawLeft, bInput);
				}
			}
		}
	}
	DrawRebarMain_VertSection_OutWall_Hunch(&Dom, nCycle, bMarkRebar, nDrawLeft, rectLimit, bInput, bForStandardSection, 0);

	DrawRebarMain_VertSection_Danbu(&Dom, bDimRebar, bMarkRebar, bInput, nCycle);
	DrawRebarMain_VertSection_UpperSlabHunch(&Dom, nCycle, bMarkRebar, nDrawLeft, CDRect(0, 0, 0, 0), bInput);
	DrawRebarMain_VertSection_LowerSlabHunch(&Dom, nCycle, bMarkRebar, nDrawLeft, CDRect(0, 0, 0, 0), bInput);
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		DrawRebarMain_VertSection_Bracket(&Dom, bStt, bDimRebar, bMarkRebar, FALSE, nDrawLeft, rectLimit, nCycle);
		DrawBFootingRebar(&Dom, bStt, FALSE, FALSE, TRUE, FALSE, FALSE);
	}

	// 헌치부에 삐딱하게 그려진 전단철근을 여기서 그려줌..
	for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		DrawRebarShear_VertSection_UpperSlabDanbu(&Dom, nCycle, bStt, !bForStandardSection && bMarkRebar, FALSE, FALSE);
		DrawRebarShear_VertSection_LowerSlabDanbu(&Dom, nCycle, bStt, !bForStandardSection && bMarkRebar, FALSE, FALSE);
	}

	*pDomP << Dom;
}

// 종단면도 상부 슬래브 배력철근 
// long nVertDir=0 : 자동(원래설정), -1 : 직거리, 1 : 사거리
// BOOL bCalSupRebar : 다시 그리기만 함.. 좌표 다시 구하지 않음
void CDrawVertSection::DrawRebarSupport_VertSection_UpperSlab(CDomyun *pDomP, BOOL bUpper, long nDan, BOOL bMarkRebar, BOOL bDimRebar, long nDimDan, BOOL bInput, CDRect rectLimit, long nVertDir, BOOL bCalSupRebar)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	// 철근 가져와서 그리기
	CHgBaseDrawStd baseDraw;
	CDPointArray xyArr;
	pBri->GetXyRebarSupport_VertSection_UpperSlab(xyArr, bUpper, nDan, nVertDir, bCalSupRebar);

	// rectLimit 적용
	if(rectLimit != CDRect(0, 0, 0, 0))
	{
		CTwinVectorArray tvArrLimit;
		CVectorArray vecArrLimit;
		tvArrLimit.SetFromRect(rectLimit);
		tvArrLimit.GetToVectorArray(vecArrLimit);

		CRebarPlacing rb;
		rb.GetXyByVectorArrayArea(xyArr, vecArrLimit, TRUE);
	}

	CDPoint xyDirScale	= bUpper ? CDPoint(0, 1) : CDPoint(0, -1);
	baseDraw.DrawPointRebar(&Dom, xyArr, xyDirScale);

	long upper	= bUpper ? iUPPER : iLOWER;
	BOOL bVertDir	= nVertDir == 0 ? pBri->IsVertDir() : (nVertDir == -1 ? TRUE : FALSE);
	CTwinVector tvUpper	= pBri->GetTvOffset_UpperSlab(0, bUpper, -1, -1, bVertDir, FALSE);
	// 치수 기입
	if(bDimRebar)
	{
		if(xyArr.GetSize() > 0)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

			CDPointArray xyArrDim;
			CDPoint xyStt	= xyArr.GetAt(0);
			CStringArray sArrText, sArrUnderText;
			CString sDir	= bUpper ? "TOP" : "BOTTOM";

			// 치수에 피복과 R정보 추가해야 됨. //////////////////////
			CTwinVector tvUpperStt, tvUpperEnd;
			CRebarInfoCycle *pRCStt	= NULL,
							*pRCEnd	= NULL;
			
			// 정보 추가(시작부)
			if(pBri->m_tvArrVertSection.GetTvByInfo("시점브라켓상부측면", tvUpperStt))
			{
				xyArrDim.Add(bUpper ? tvUpperStt.m_v2 : tvUpperStt.m_v1);	sArrText.Add("0"); sArrUnderText.Add("0");
				xyStt.y	= bUpper ? tvUpperStt.GetXyTop().y : tvUpperStt.GetXyBottom().y;
			}
			else if(pBri->m_tvArrVertSection.GetTvByInfo("시점슬래브측면", tvUpperStt))
			{
				xyArrDim.Add(bUpper ? tvUpperStt.m_v2 : tvUpperStt.m_v1);	sArrText.Add("0"); sArrUnderText.Add("0");
				xyStt.y	= bUpper ? tvUpperStt.GetXyTop().y : tvUpperStt.GetXyBottom().y;
			}

			pRCStt	= pBri->GetRebarInfoCycleUpperSlab(0, 0, bUpper, 0); 
			if(pRCStt)
			{
				BOOL bSttRebar	= TRUE;
				if(pBri->IsMainRebarJointUpperSlabAndOutWall(pRCStt, bSttRebar))
				{
					xyArrDim.Add(pRCStt->m_xyEnd); sArrText.Add("0"); sArrUnderText.Add("0");
				}
				else
				{
					if(!pBri->IsBlockOut(TRUE) || !bUpper)
					{
						xyArrDim.Add(pRCStt->m_xyMid1); sArrText.Add("0"); sArrUnderText.Add("0");
					}
					else
					{
						xyArrDim.Add(tvUpper.m_v1); sArrText.Add("0"); sArrUnderText.Add("0");
					}
				}
			} 
						
			// 치수 기입 정보 가져옴
			CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_SupportRebar_UpperSlab[upper][nDan];
			m_pStd->GetDimInfoAtPlaceRebarByBlock(xyArr, xyArrDim, sArrText, sArrUnderText, pPlace, bUpper ? "U" : "L", xyStt, CDPoint(1, 0), bInput, TRUE);

			// 정보 추가(끝부)
			pRCEnd	= pBri->GetRebarInfoCycleUpperSlab(0, 0, bUpper, -1); 
			if(pRCEnd)
			{
				long nCntJoint =0;
				for (long nJi =0; nJi < pBri->m_nQtyJigan; nJi++)
					nCntJoint += pBri->GetCountJointMainRebarUpperSlab(0,nDan, bUpper, nJi);
				if(pBri->IsBoxType() && nCntJoint == 0)
				{
					xyArrDim.Add(bUpper? pRCEnd->m_xyEndExt : pRCEnd->m_xyEnd); sArrText.Add("0"); sArrUnderText.Add("0");
				}
				else
				{
					if(!pBri->IsBlockOut(FALSE) || !bUpper)
					{
						if(bUpper)
						{
							xyArrDim.Add(pRCEnd->m_xyMid1); sArrText.Add("0"); sArrUnderText.Add("0");
						}

						xyArrDim.Add(pRCEnd->m_xyMid2); sArrText.Add("0"); sArrUnderText.Add("0");
					}
					else
					{
						xyArrDim.Add(tvUpper.m_v2); sArrText.Add("0"); sArrUnderText.Add("0");
					}
				}
				
			}
			if(pBri->m_tvArrVertSection.GetTvByInfo("종점브라켓상부측면", tvUpperEnd))
			{
				xyArrDim.Add(bUpper ? tvUpperEnd.m_v1 : tvUpperEnd.m_v2); sArrText.Add("0"); sArrUnderText.Add("0");
				xyStt.y	= bUpper ? max(tvUpperEnd.GetXyTop().y, xyStt.y) : min(tvUpperEnd.GetXyBottom().y, xyStt.y);
			}
			else if(pBri->m_tvArrVertSection.GetTvByInfo("종점슬래브측면", tvUpperEnd))
			{
				xyArrDim.Add(bUpper ? tvUpperEnd.m_v1 : tvUpperEnd.m_v2); sArrText.Add("0"); sArrUnderText.Add("0");
				xyStt.y	= bUpper ? max(tvUpperEnd.GetXyTop().y, xyStt.y) : min(tvUpperEnd.GetXyBottom().y, xyStt.y);
			}

			if(!bInput)
			{
				//sArrText.RemoveAll();
				sArrUnderText.RemoveAll();
			}

			// 치수 기입
			if(xyArrDim.GetSize() > 1)
			{
				xyStt.x	= xyArrDim.GetAt(0).x;
				baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, bUpper ? nDimDan : nDimDan+1, sDir, -1, bUpper, FALSE, FALSE);
			}
		}
	}


	// 마킹
	CString sDescription	= _T("");
	if(bUpper)
		sDescription.Format("상부슬래브 상면 %d단 배력철근", nDan+1);	
	else
		sDescription.Format("상부슬래브 하면 %d단 배력철근2", nDan+1);
	CString sMark	= pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescription);


	//ARCBRIDGE-2583 상부슬래브 상면의 끝단 철근이 이격거리가 있을시 해당 부위의 마킹을 해주기 위함
	CString sMarkStt = _T(""), sMarkEnd = _T("");
	CString sDescriptionStt = _T(""), sDescriptionEnd = _T("");

	for (long nStt = 0 ; nStt <= iEND; nStt++)
	{
		CString sUL = bUpper? _T("상면") : _T("하면");
		if(nStt == iSTT)
		{
			if(bUpper)	sDescriptionStt.Format("상부슬래브 %s %d단 배력철근%d", sUL, nDan+1, nStt+2);
			else			sDescriptionStt.Format("상부슬래브 %s %d단 배력철근2_%d", sUL, nDan+1, nStt+2);
			sMarkStt = pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescriptionStt);
		}
		else
		{
			if(bUpper)	sDescriptionEnd.Format("상부슬래브 %s %d단 배력철근%d",sUL, nDan+1, nStt+2);
			else			sDescriptionEnd.Format("상부슬래브 %s %d단 배력철근2_%d", sUL, nDan+1, nStt+2);
			sMarkEnd = pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescriptionEnd);
		}
	}
	
	//확폭or가각일 경우 해당 배력 철근을 못찾을수 있음
	BOOL bSkewSttExpOnly	= FALSE;
	BOOL bSkewEndExpOnly	= FALSE;
	if(pBri->m_bIsExp )
	{
		CTwinVectorArray tvArr, tvArrAngStt, tvArrAngEnd, tvArrRAngStt, tvArrRAngEnd;
		CTwinVectorArray tvArrExpSttLAndR, tvArrExpSttLOrR, tvArrExpEndLAndR, tvArrExpEndLOrR;
		pBri->GetTvRebarSupport_Plane_Slab_Std(TRUE, tvArr, tvArrAngStt, tvArrAngEnd, tvArrRAngStt, tvArrRAngEnd, bUpper, nDan, 0);
		pBri->DivideRebarSupport_Plane_UpperSlabExpSection(tvArr, tvArrExpSttLAndR, tvArrExpSttLOrR, tvArrExpEndLAndR, tvArrExpEndLOrR);

		long nSize			= tvArr.GetSize();
		long nSizeStt_And	= tvArrExpSttLAndR.GetSize();
		long nSizeStt_Or		= tvArrExpSttLOrR.GetSize();
		long nSizeEnd_And = tvArrExpEndLAndR.GetSize();
		long nSizeEnd_Or   = tvArrExpEndLOrR.GetSize();
		
		long nSizeSttAng	= tvArrAngStt.GetSize();
		long nSizeEndAng	= tvArrAngEnd.GetSize();
		long nSizeRSttAng	= tvArrRAngStt.GetSize();
		long nSizeRAngEnd= tvArrRAngEnd.GetSize();
		
		if(pBri->m_bIsExp)
		{
			if(nSize ==0 && nSizeStt_And ==0 && nSizeStt_Or ==0 && (nSizeSttAng > 0 || nSizeRSttAng > 0))
				bSkewSttExpOnly = TRUE;
			if(nSize ==0 && nSizeEnd_And ==0 && nSizeEnd_Or ==0 && (nSizeEndAng > 0 || nSizeRAngEnd > 0))
				bSkewEndExpOnly = TRUE;
		}

		CString sUpdn = bUpper? _T("상면") : _T("하면");

		if(sMark == _T("") )
		{
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
				sMark	= pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescription);

				if(sMark == _T("")) continue;
				else break;
			}
		}
		if(sMarkStt == _T(""))
		{
			long nIndexStt = nSizeStt_Or > 0? 2 : 3;
			sDescriptionStt.Format("상부슬래브 상면 %d단 배력철근%d", nDan+1, nIndexStt+5);
			sMarkStt = pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescriptionStt);
		}
		if(sMarkEnd == _T(""))
		{
			long nIndexEnd = nSizeEnd_And > 0? 3 : 4;
			sDescriptionEnd.Format("상부슬래브 상면 %d단 배력철근%d", nDan+1, nIndexEnd+5);
			sMarkEnd = pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescriptionEnd);
		}

	}


	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		// 입력은 1단만 받는다
		CPlaceRebarByBlock *pPlace		= &pBri->m_placeByBlock_SupportRebar_UpperSlab[upper][nDan];
		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);

		//#0028210
		{
			CRebarPlacing rp;

			//표준 단면 배근 - 배력철근입력에 입력된 좌표의 이격거리에서 최대값을 구함.
			// 배근 간격 입력은 1단에서만 받기 때문에 해당단의 pPlace를 사용하지 않고 1단의 입력 정보를 사용해서 최대 거리를 구한다.
			CDPointArray xySupp;
			xySupp = pBri->m_placeByBlock_SupportRebar_UpperSlab[upper][0].m_xyArr;

			xySupp.Sort(FALSE, FALSE);

			double dMaxDist = (xySupp.GetSize() == 0) ? 0 : xySupp.GetAt(0).y;

			//배치된 ctc보다 큰 이격거리가 나오게 되면 마킹을 따로 해준다. 마킹이 길어져 깨져보인다.
			CTypedPtrArray<CObArray, CDPointArray*> xyDArr;

			BOOL bExistSttAdd = FALSE;
			BOOL bExistEndAdd = FALSE;
			CDPointArray *pXyArr = NULL;

			if(xyArr.GetSize() > 0) pXyArr = new CDPointArray;
			double dLenAll = xyArr.GetSize() > 0 ? rp.GetDistPointToPoint(xyArr[0], xyArr[xyArr.GetSize() - 1]) : 0;

			for(long ix = 0; ix < xyArr.GetSize() - 1; ix ++)
			{
				double dLen = rp.GetDistPointToPoint(xyArr[ix], xyArr[ix+1]);

				if(Compare(dLen, dMaxDist+1 , ">", 0.1) ) //최대값보다 큰값이 있으면 마킹을 하나더 한다.
				{
					if(rp.GetDistPointToPoint(xyArr[0], xyArr[ix]) < dLenAll / 2)
						bExistSttAdd = TRUE;
					else
						bExistEndAdd = TRUE;

					xyDArr.Add(pXyArr);

					pXyArr = new CDPointArray;
					continue;
				}
				else if((Compare(dLen, dMaxDist/2 , "=", 0.1) && bSkewSttExpOnly) || (Compare(dLen, dMaxDist , "=", 0.1) && bSkewEndExpOnly))
				{
					if(rp.GetDistPointToPoint(xyArr[0], xyArr[ix]) < dLenAll / 2)
						bExistSttAdd = TRUE;
					else
						bExistEndAdd = TRUE;
				}

				pXyArr->Add(xyArr[ix]);
			}

			if(pXyArr && pXyArr->GetSize() > 0) xyDArr.Add(pXyArr);

			for(long ix = 0; ix < xyDArr.GetSize(); ix ++)
			{
				pXyArr = xyDArr.GetAt(ix);

				mark.SetRebar(*pXyArr);

				BOOL bUpperStt		= (sMarkStt.GetLength() > 0 && ix == 0 && bExistSttAdd)? TRUE : FALSE;
				BOOL bUpperEnd	= (sMarkEnd.GetLength() > 0 && ix == xyDArr.GetSize()-1 && bExistEndAdd)? TRUE : FALSE;

				CRebar *pRb = NULL;
				if(bUpperStt)
				{
					pRb = pBri->GetRebarSupportByDescription(sDescriptionStt);
					mark.m_sMark	= sMarkStt;
				}
				else if(bUpperEnd)
				{
					pRb = pBri->GetRebarSupportByDescription(sDescriptionEnd);
					mark.m_sMark	= sMarkEnd;
				}
				else
				{
					pRb = pBri->GetRebarSupportByDescription(sDescription);
					mark.m_sMark	= sMark;
				}
				mark.m_sDia						= pBri->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(ePartUpperSlab));
				mark.m_nTypeMarkPointUpAndDown	= bUpper ? TYPE_MARKPOINT_UPPER : TYPE_MARKPOINT_LOWER;
				mark.m_nTypeMarkDir				= TYPE_MARKDIR_LEFT;
				mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
				mark.m_dPosScaleMarkStt			= 0.3;
				mark.m_dCtc = pBri->GetCTCRebarSupport_Slab(TRUE, bUpper, nDan, TRUE);
				mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;

				mark.MarkRebar();

				*pDomP << Dom;
			}
			AhTPADelete(&xyDArr, (CDPointArray*)0);
		}
	}


	*pDomP << Dom;
}

// 종단면도 외측벽체 배력철근
void CDrawVertSection::DrawRebarSupport_VertSection_OutWall(CDomyun *pDomP, BOOL bStt, BOOL bInner, long nDan, BOOL bMarkRebar, BOOL bDimRebar, BOOL bInput, CDRect rectLimit, long nVertDir)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	BOOL bVertDir	= pBri->IsVertDir();
	if(nVertDir != 0)
		bVertDir	= nVertDir == -1 ? TRUE : FALSE;
	// 철근 가져와서 그리기
	CHgBaseDrawStd baseDraw;
	CDPointArray xyArr;
	pBri->GetXyRebarSupport_VertSection_OutWall(xyArr, bStt, bInner, nDan, nVertDir);
	CDPoint xyDirScale	= (bStt && !bInner) || (!bStt && bInner) ? CDPoint(-1, 0) : CDPoint(1, 0);

	// rectLimit 적용
	if(rectLimit != CDRect(0, 0, 0, 0))
	{
		CTwinVectorArray tvArrLimit;
		CVectorArray vecArrLimit;
		tvArrLimit.SetFromRect(rectLimit);
		tvArrLimit.GetToVectorArray(vecArrLimit);

		CRebarPlacing rb;
		rb.GetXyByVectorArrayArea(xyArr, vecArrLimit, TRUE);
	}

	baseDraw.DrawPointRebar(&Dom, xyArr, xyDirScale);

	long stt	= bStt ? iSTT : iEND;
	long inner	= bInner ? iINNER : iOUTTER;
	double se = bStt ? -1 : 1;
	// 치수 기입
	if(bDimRebar)
	{
		if(xyArr.GetSize() > 0)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

			CDPointArray xyArrDim;
			CDPoint xyStt	= xyArr.GetAt(0);
			CStringArray sArrText, sArrUnderText;
			CString sDir	= _T("");
			
			if(bStt)
			{
				sDir	= bInner ? "RIGHT" : "LEFT";	
			}
			else
			{
				sDir	= bInner ? "LEFT" : "RIGHT";
			}

			// 치수에 피복과 R정보 추가해야 됨. //////////////////////
			CTwinVector tvLower, tvUpper;
			CRebarInfoCycle *pRCLower	= NULL;
			CRebarInfoCycle *pRCUpper	= NULL;

			if(pBri->IsBoxType())
			{
				CTwinVector tvWall = pBri->GetTvOffset_OutWall(0, bStt, bInner, bVertDir);
				pBri->m_tvArrVertSection.GetTvByInfo(bStt? _T("하부슬래브하면(시점벽체아래)") : _T("하부슬래브하면(종점벽체아래)"), tvLower);
				CDPoint xyDimStt  = pBri->GetXyMatchLineAndLowerSlab(tvWall.m_v1, tvWall.GetXyDir(), 0, 0, bVertDir);
				xyArrDim.Add(xyDimStt); sArrText.Add("0"); sArrUnderText.Add("0");

				pRCLower	= pBri->GetRebarInfoCycleLowerSlab(0, 0, FALSE, bStt ? 0 : -1);
				if(!pRCLower || pRCLower->m_nPosStt == pRCLower->m_nPosEnd)
					pRCLower	= pBri->GetRebarInfoCycleOutWall(0, 0, bStt, bInner, 0);
			}
			else
			{
				CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;

				// 정보 추가(시작부)
				if(pFooting->m_bIs)
				{
					if(pFooting->m_tvArrFront.GetTvByInfo("기초하면", tvLower))
					{
						xyArrDim.Add(tvLower.m_v1); sArrText.Add("0"); sArrUnderText.Add("0");
					}
				}
				pRCLower	= pBri->GetRebarInfoCycleOutWall(0, 0, bStt, bInner, 0);
				if(!pRCLower)
					pRCLower	= pBri->GetRebarInfoCycleUpperSlab(0, 0, TRUE, bStt ? 0 : -1);
			}

			BOOL bSttRebar	= TRUE;
			if(pBri->IsMainRebarJointUpperSlabAndOutWall(pRCLower, bSttRebar))
			{
				if(bSttRebar)
				{
					xyArrDim.Add(pRCLower->m_xyMid1); sArrText.Add("0"); sArrUnderText.Add("0");
					xyArrDim.Add(pRCLower->m_xyMid2); sArrText.Add("0"); sArrUnderText.Add("0");
				}
				else
				{
					xyArrDim.Add(pRCLower->m_xyMidExt); sArrText.Add("0"); sArrUnderText.Add("0");
					xyArrDim.Add(pRCLower->m_xyEnd); sArrText.Add("0"); sArrUnderText.Add("0");
				}
			}
			else if(pRCLower)
			{
				if(pBri->IsBoxType())
				{
					CDPoint xyBase = xyArrDim.GetAt(0);
					xyBase.y += pBri->m_dCoverLower_LowerSlab[0];
					xyArrDim.Add(xyBase); sArrText.Add("0"); sArrUnderText.Add("0");
				}
				else
				{
					xyArrDim.Add(pRCLower->m_xyMid1); sArrText.Add("0"); sArrUnderText.Add("0");
					xyArrDim.Add(pRCLower->m_xyMid2); sArrText.Add("0"); sArrUnderText.Add("0");
				}
			}

			// 치수 기입 정보 가져옴
			CDPoint xyTmp	= xyArrDim.GetSize() > 0 ? xyArrDim.GetAt(xyArrDim.GetSize()-1) : xyArr.GetAt(0);
			CTwinVector tvWall			= pBri->GetTvOffset_OutWall(0, bStt, bInner, bVertDir);
			if(nDan == 0)
			{
				CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_SupportRebar_OutWall[stt][inner][0];
				m_pStd->GetDimInfoAtPlaceRebarByBlock(xyArr, xyArrDim, sArrText, sArrUnderText, pPlace, bInner ? "I" : "O", xyTmp, tvWall.GetXyDir(), bInput, TRUE);
			}
			else
			{
				CDPointArray xyArrTmp;
				xyArrTmp	= xyArr;
				xyArrTmp.InsertAt(0, pRCLower->m_xyMid2);
	
				xyArrTmp.RemoveSameValue(0.01);
				xyArrTmp.Sort(FALSE);

				CRebarPlacing rb;
				CPlaceRebarByBlock place;
				rb.ConvertPointArrayToOffsetInfo(xyArrTmp, place.m_xyArr, 1);
				place.m_nCountBlock	= place.m_xyArr.GetSize();
				
				m_pStd->GetDimInfoAtPlaceRebarByBlock(xyArr, xyArrDim, sArrText, sArrUnderText, &place, bInner ? "I" : "O", xyTmp, tvWall.GetXyDir(), bInput, TRUE);
			}

			// 정보 추가(끝부) - 상부 슬래브 첫번째 철근
			CDPoint vAngUpper	= pBri->GetTvOffset_UpperSlab(0, TRUE, -1, -1, bVertDir).GetXyDir();
			pRCUpper	= pBri->GetRebarInfoCycleUpperSlab(0, 0, TRUE, bStt ? 0 : -1);
			if(pRCUpper)
			{
				BOOL bSttRebar	= TRUE;
				if(pBri->IsMainRebarJointUpperSlabAndOutWall(pRCUpper, bSttRebar))
				{
					if(bSttRebar)
					{
						xyArrDim.Add(pRCUpper->m_xyEnd); sArrText.Add("0"); sArrUnderText.Add("0");	
						xyArrDim.Add(pRCUpper->m_xyMidExt); sArrText.Add("0"); sArrUnderText.Add("0");	
					}
					else
					{
						xyArrDim.Add(pRCUpper->m_xyMid2); sArrText.Add("0"); sArrUnderText.Add("0");	
						xyArrDim.Add(pRCUpper->m_xyMid1); sArrText.Add("0"); sArrUnderText.Add("0");	
					}
				}
				else
				{
					long nCntJointSlab =0;
					for (long nJi =0; nJi < pBri->m_nQtyJigan; nJi++)
						nCntJointSlab += pBri->GetCountJointMainRebarUpperSlab(0,nDan, TRUE, nJi);
					
					if(!bInner)
					{
						if(nCntJointSlab ==0 && bStt == FALSE && pBri->IsBoxType())
						{
							xyArrDim.Add(bStt ? pRCUpper->m_xyMid1 : pRCUpper->m_xyMidExt); sArrText.Add("0"); sArrUnderText.Add("0");	
						}
						else
						{
							xyArrDim.Add(bStt ? pRCUpper->m_xyMid1 : pRCUpper->m_xyMid2); sArrText.Add("0"); sArrUnderText.Add("0");	
						}
					}
					
 
					CDPoint xyLast	= xyArrDim.GetAt(xyArrDim.GetSize()-1);
					CDPoint xy		= bStt ? pRCUpper->m_xyMid2 : pRCUpper->m_xyMid1;
					if(nCntJointSlab ==0 && bStt == FALSE && pBri->IsBoxType())
						xy  = pRCUpper->m_xyMid2;

					GetXyMatchLineAndLine(xyLast, CDPoint(0, 1), xy, vAngUpper, xy);

					xyArrDim.Add(xy); sArrText.Add("0"); sArrUnderText.Add("0");
				}
			}


			if(pBri->m_tvArrVertSection.GetTvByInfo(bStt ? "시점브라켓상부측면" : "종점브라켓상부측면", tvUpper))
			{
				CDPoint xyLast	= xyArrDim.GetAt(xyArrDim.GetSize()-1);
				CDPoint xy	= bStt ? tvUpper.m_v2 : tvUpper.m_v1;
				GetXyMatchLineAndLine(xyLast, CDPoint(0, 1), xy, vAngUpper, xy);
				xyArrDim.Add(xy); sArrText.Add("0"); sArrUnderText.Add("0");
			}

			if(sArrUnderText.GetSize() > 0)
			{
				long nSize	= sArrUnderText.GetSize();
				CString sDan	= _T("");
				sDan.Format("%s(%d단)", bInner ? "내측" : "외측", nDan+1);
				sArrUnderText.SetAt(nSize-1, sDan);
			}

			// 치수 기입
			if(xyArrDim.GetSize() > 1)
			{
				if(bStt)
					xyStt	= CDPoint(bInner ? tvLower.GetXyRight().x : tvLower.GetXyLeft().x, xyArrDim.GetAt(0).y);
				else
					xyStt	= CDPoint(bInner ? tvLower.GetXyLeft().x : tvLower.GetXyRight().x, xyArrDim.GetAt(0).y);

				if(pBri->m_bExistProtectionRiseFooting && !bInner)
					xyStt.x += pBri->m_dPRF_W * se;
				if(!bInput)
				{
					// 뒤좌표가 더 작으면 앞좌표 삭제
					long i = 0; for(i = 1; i < xyArrDim.GetSize(); i++)
					{
						if(xyArrDim[i].y < xyArrDim[i-1].y)
						{
							xyArrDim.RemoveAt(i-1);
							i--;
						}
					}
					
				}

				baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDan, sDir, -1, FALSE, FALSE, FALSE);
			}
		}
	}


	// 마킹
	CString sDescription	= _T("");
	sDescription.Format("%s %s %d단 배력철근", bStt ? "시점측벽체" : "종점측벽체", bInner ? "내측" : "외측", nDan+1);
	CString sMark			= pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescription);
	if(sMark == "")
	{
		sDescription.Format("%s %s %d단 배력철근2", bStt ? "시점측벽체" : "종점측벽체", bInner ? "내측" : "외측", nDan+1);
		sMark			= pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescription);
	}
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		CRebar *pRb = pBri->GetRebarSupportByDescription(sDescription);
		CPlaceRebarByBlock *pPlace		= &pBri->m_placeByBlock_SupportRebar_OutWall[stt][inner][nDan];
		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
		mark.SetRebar(xyArr);

		// 설정
		mark.m_sMark					= sMark;
		mark.m_sDia						= pBri->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(bStt? ePartOutWallStt : ePartOutWallEnd));
		if(bStt)
		{
			mark.m_nTypeMarkPointUpAndDown	= bInner ? TYPE_MARKPOINT_LOWER : TYPE_MARKPOINT_UPPER;
		}
		else
		{
			mark.m_nTypeMarkPointUpAndDown	= bInner ? TYPE_MARKPOINT_UPPER : TYPE_MARKPOINT_LOWER;
		}
		mark.m_nTypeMarkDir				= bInner ? TYPE_MARKDIR_LEFTTAN : TYPE_MARKDIR_LEFTTAN;
		mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
		mark.m_dPosScaleMarkStt			= 0.75;
		mark.m_dCtc = pBri->GetCTCRebarSupport_OutWall(bStt, bInner, nDan, TRUE);
		mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;

		mark.MarkRebar();
	}

	*pDomP << Dom;
}

// 종단면도 상부슬래브 전단철근
void CDrawVertSection::DrawRebarShear_VertSection_UpperSlab(CDomyun *pDomP, BOOL bMarkRebar, BOOL bDimRebar, BOOL bInput, CDRect rectLimit, long nVertDir)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);

	BOOL bVertDir	= pBri->IsVertDir();
	if(nVertDir != 0)
		bVertDir	= nVertDir == -1 ? TRUE : FALSE;

	// 철근 가져와서 그리기
	CTwinVectorArray tvArr, tvArrHunch, tvArrRib;
	pBri->GetTvRebarShear_VertSection_UpperSlab(tvArr, tvArrHunch, tvArrRib, nVertDir);

	// rectLimit 적용
	if(rectLimit != CDRect(0, 0, 0, 0))
	{
		CTwinVectorArray tvArrLimit;
		CVectorArray vecArrLimit;
		tvArrLimit.SetFromRect(rectLimit);
		tvArrLimit.GetToVectorArray(vecArrLimit);

		CRebarPlacing rb;
		rb.GetTvArrayByCrossVectorArrayArea(tvArr, vecArrLimit, TRUE);
		rb.GetTvArrayByCrossVectorArrayArea(tvArrHunch, vecArrLimit, TRUE);
		rb.GetTvArrayByCrossVectorArrayArea(tvArrRib, vecArrLimit, TRUE);
	}

	// 전단 철근 그리기
	m_pStd->DrawTvArrayWidthDashDot(&Dom, tvArr, FALSE);
	m_pStd->DrawTvArrayWidthDashDot(&Dom, tvArrHunch, FALSE);
	m_pStd->DrawTvArrayWidthDashDot(&Dom, tvArrRib, FALSE);

	// 마크시 리브부는 2 헌치부는 1 아닌거는 0
	for(long hunch = 0; hunch < 3; hunch++)
	{
		CTwinVectorArray *pTvArr;//	= bHunch ? &tvArrHunch : &tvArr;
		CString sDescription	= _T("");
		switch(hunch)
		{
		case 0 : 
			pTvArr	= &tvArr; 
			sDescription	= "상부슬래브 전단철근1";
			break;
		case 1 : 
			pTvArr	= &tvArrHunch; 
			sDescription	= "상부슬래브 전단철근2";
			break;
		case 2 : 
			pTvArr	= &tvArrRib; 
			sDescription	= "상부슬래브 전단철근3";
			break;
		}

		
		CString sMark	= pBri->GetMarkRebarShear(0, bInput, FALSE, FALSE, sDescription);
		if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
		{
			double dTS	= pBri->m_dTS;
			// 철근진행방향이 반대로 되어야 함.
			CRebarPlacing rb;
			CTwinVectorArray tvArrTmp;
			tvArrTmp	= *pTvArr;
			//rb.ReverseRebar(tvArrTmp);

			CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_ShearRebar_UpperSlab;
			CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);

			CTwinVectorArray tvTmp;
			CTwinVector tvWall	= pBri->GetTvOffset_UpperSlab(0, TRUE, -1.0, -1.0, bVertDir);
			if(hunch == 1)
			{
				tvWall.m_v1.y -= dTS/4;
				tvWall.m_v2.y -= dTS/4;
			}
			tvTmp.Add(tvWall);
			tvWall	= pBri->GetTvOffset_UpperSlab(0, FALSE, -1.0, -1.0, bVertDir);
			if(hunch == 0)
			{
				tvWall.m_v1.y += dTS/4;
				tvWall.m_v2.y += dTS/4;
			}
			else if(hunch == 2)
			{
				tvWall.m_v1.y += dTS/2;
				tvWall.m_v2.y += dTS/2;
			}
			tvTmp.Add(tvWall);
			
			mark.SetRebar(tvArrTmp, tvTmp);
			// 설정
			mark.m_sMark	= sMark;
			mark.m_sDia		= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(ePartUpperSlab, TRUE));
			mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_POS;	// 지름 표시위치 찾는 방법
			mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;	// TYPE_MARKPOS_POS일때 옵션
			mark.m_dPosScaleMarkStt			= pBri->m_nQtyJigan == 1 ? 0.8 : 0.6;
			mark.m_nTypeMarkDir				= TYPE_MARKDIR_LEFTTAN;
			mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_UPPER;
			switch(hunch)
			{
			case 0 : mark.m_dAddHeight	= dTS/2 - dTS/4/3; break;
			case 1 : mark.m_dAddHeight	= dTS/2 + dTS/4/4; break;
			case 2 : mark.m_dAddHeight	= dTS/2 - dTS/4/2; break;
			}
			mark.m_dCtc = pBri->GetCTCRebarShear_UpperSlab();
			mark.m_nTypeRebar = pBri->m_RebarShearDetail_UpperSlab.GetRebarType();
	
			mark.MarkRebar();
		}
	}

	
	
	if(bDimRebar)
	{
		// 치수 기입할때는 헌치부와 아닌거를 합쳐야 된다.
		// 합쳐서 정렬도 해야 겠지?(정
		CTwinVectorArray tvArrTot;
		tvArrTot.AddFromTvArray(tvArr);
		tvArrTot.AddFromTvArray(tvArrHunch);

		long nSize	= tvArrTot.GetSize();
		CDPointArray xyArr;
		long i = 0; for(i = 0; i < nSize; i++)
			xyArr.Add(tvArrTot.GetAt(i).m_v1);
		xyArr.Sort(TRUE);
		
		if(nSize > 0)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

			CDPointArray xyArrDim;
			CDPoint xyStt	= tvArrTot.GetAt(0).m_v1;
			CStringArray sArrText, sArrUnderText;
			CString sDir	= _T("TOP");

			// 치수에 피복과 R정보 추가해야 됨. //////////////////////
			CTwinVector tvStt, tvEnd;
						
			xyStt.y = tvArrTot.GetXyTop().y;
			// 정보 추가(시작부)
			if(pBri->m_tvArrVertSection.GetTvByInfo("시점브라켓상부측면", tvStt))
			{
				xyArrDim.Add(tvStt.m_v2); sArrText.Add("0"); sArrUnderText.Add("0");
				xyStt	= xyArrDim.GetAt(0);
				xyStt.y = tvArrTot.GetXyTop().y;
			}
			else if(pBri->m_tvArrVertSection.GetTvByInfo("시점슬래브측면", tvStt))
			{
				xyArrDim.Add(tvStt.m_v2); sArrText.Add("0"); sArrUnderText.Add("0");
				xyStt	= xyArrDim.GetAt(0);
				xyStt.y = tvArrTot.GetXyTop().y;
			}
			
			// 치수 기입 정보 가져옴
			CRebarPlacing rb;
			CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_ShearRebar_UpperSlab;
			m_pStd->GetDimInfoAtPlaceRebarByBlock(xyArr, xyArrDim, sArrText, sArrUnderText, pPlace, "V", xyStt, CDPoint(1, 0), bInput, TRUE);

			// 정보 추가(끝부)
			if(pBri->m_tvArrVertSection.GetTvByInfo("종점브라켓상부측면", tvEnd))
			{
				xyArrDim.Add(tvEnd.m_v1); sArrText.Add("0"); sArrUnderText.Add("0");
			}
			else if(pBri->m_tvArrVertSection.GetTvByInfo("종점슬래브측면", tvEnd))
			{
				xyArrDim.Add(tvEnd.m_v1); sArrText.Add("0"); sArrUnderText.Add("0");
			}


			// 치수 기입
			if(xyArrDim.GetSize() > 1)
			{
				xyStt	= xyArrDim.GetAt(0);
				xyStt.y = tvArrTot.GetXyTop().y;
				CHgBaseDrawStd baseDraw;
				baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, 0, sDir, -1, FALSE, FALSE, FALSE);
			}
		}
	}

	*pDomP << Dom;
}

// long nVertDir : -1 : 직거리, 0 : 자동(원래설정), 1 : 사거리
void CDrawVertSection::DrawRebarShear_VertSection_OutWall(CDomyun *pDomP, BOOL bStt, long nDimDan, BOOL bMarkRebar, BOOL bDimRebar, BOOL bDimTotal, BOOL bInput, long nVertDir)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);

	BOOL bVertDir	= pBri->IsVertDir();

	// 철근 가져와서 그리기
	CTwinVectorArray tvArr;
	pBri->GetTvRebarShear_VertSection_OutWall(tvArr, bStt, nVertDir);

	// 전단철근 그리기
	m_pStd->DrawTvArrayWidthDashDot(&Dom, tvArr, FALSE);

	CString sDescription	= _T("");
	sDescription.Format("%s 전단철근", bStt ? "시점측벽체" : "종점측벽체");
	CString sMark	= pBri->GetMarkRebarShear(0, bInput, FALSE, FALSE, sDescription);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_ShearRebar_OutWall[bStt ? iSTT : iEND];
		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);

		CTwinVectorArray tvTmp;
		CTwinVector tvWall	= pBri->GetTvOffset_OutWall(0, bStt, FALSE, bVertDir);
		tvTmp.Add(tvWall);
		tvWall	= pBri->GetTvOffset_OutWall(0, bStt, TRUE, bVertDir);
		tvTmp.Add(tvWall);

		
		mark.SetRebar(tvArr, tvTmp);
		// 설정
		mark.m_sMark	= sMark;
		mark.m_sDia		= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(bStt? ePartOutWallStt:ePartOutWallEnd, TRUE));
		mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_POS;
		mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
		mark.m_nTypeMarkDir				= bStt ? TYPE_MARKDIR_RIGHT : TYPE_MARKDIR_LEFT;
		mark.m_dAddHeight				= bStt ? pBri->m_dWS/2 : pBri->m_dWE/2;
		mark.m_dPosScaleMarkStt			= 0.5;
		// 외측벽체 주철근과 피하기 위해
		// 주철근 첫번째 이음있는 곳에 마크한다.(주철근 마크가 이음위치에 올 확률은 거의 0에 가깝기 때문 한 100사이클 되면 올지도 모름.. ㅋㅋ)
		CRebarInfoCycle *pRC	= pBri->GetRebarInfoCycleOutWall(0, 0, bStt, TRUE, 0);
		CRebarInfoCycle *pRCNext	= pBri->GetRebarInfoCycleOutWall(0, 0, bStt, TRUE, 1);
		if(pRC)
		{
			double dLenJoint	= pRCNext ? pBri->GetValueJoint(pRC, pRCNext->m_Dia) : 0;
			CDRect rect	= tvArr.GetRect();
			mark.m_dPosScaleMarkStt			= fabs((pRC->m_xyEnd.y - dLenJoint/2) - rect.bottom) / fabs(rect.Height());

			// 혹시나 이음이 없어서 마크위치가 중간을 넘어간다 싶으면 그냥 밑으로 내려서 마크한다.
			if(mark.m_dPosScaleMarkStt >= 0.5)
				mark.m_dPosScaleMarkStt = 0.25;
		}
		mark.m_dCtc = pBri->m_dCTC_ShearRebarOutWall[bStt? iSTT:iEND];
		mark.m_nTypeRebar = pBri->m_RebarShearDetail_OutWall[bStt? iSTT:iEND].GetRebarType();

		mark.MarkRebar();
	}

	if(bDimRebar)
	{
		long nSize	= tvArr.GetSize();
		if(tvArr.GetSize() > 0)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

			CDPointArray xyArrDim;
			CDPoint xyStt	= tvArr.GetAt(0).m_v1;
			CStringArray sArrText, sArrUnderText;
			CString sDir	= bStt ? _T("LEFT") : _T("RIGHT");
			double	se		= bStt ? -1 : 1;

			// 치수에 피복과 R정보 추가해야 됨. //////////////////////
			CTwinVector tvWall		= pBri->GetTvOffset_OutWall(0, bStt, FALSE, bVertDir);
			CTwinVector tvLower;
			if(pBri->IsBoxType())
			{
				pBri->m_tvArrVertSection.GetTvByInfo(bStt? _T("하부슬래브하면(시점벽체아래)") : _T("하부슬래브하면(종점벽체아래)"), tvLower);
				CDPoint xyDimStt  = pBri->GetXyMatchLineAndLowerSlab(tvWall.m_v1, tvWall.GetXyDir(), 0, 0, bVertDir);
				tvLower.m_v1 = tvLower.m_v2 = xyDimStt;
				xyArrDim.Add(xyDimStt); sArrText.Add("0"); sArrUnderText.Add("0");
			}
			else
			{
				CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;

				// 정보 추가(시작부)
				if(pFooting->m_bIs && pFooting->m_tvArrFront.GetTvByInfo("기초하면", tvLower))
				{
					xyArrDim.Add(CDPoint(tvWall.m_v1.x, tvWall.m_v1.y-pFooting->m_dCoverLower[0]));
					sArrText.Add("0"); sArrUnderText.Add("0");
				}
			}
			
			// 치수 기입 정보 가져옴
			CRebarPlacing rb;
			CDPointArray xyArr;
			long i = 0; for(i = 0; i < nSize; i++)
				xyArr.Add(tvArr.GetAt(i).m_v1);
			
			CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_ShearRebar_OutWall[bStt ? iSTT : iEND];
			m_pStd->GetDimInfoAtPlaceRebarByBlock(xyArr, xyArrDim, sArrText, sArrUnderText, pPlace, "V", xyArrDim.GetAt(0), CDPoint(0, 1), bInput, TRUE);

			// 정보 추가(끝부)
			xyArrDim.Add(CDPoint(tvWall.m_v2.x, tvWall.m_v2.y+pBri->m_dCoverUpper_UpperSlab[0]));
			sArrText.Add("0"); sArrUnderText.Add("0");
			
			// 치수 기입
			if(xyArrDim.GetSize() > 1)
			{
				xyStt	= bStt ? tvLower.GetXyLeft() : tvLower.GetXyRight();
				if(pBri->m_bExistProtectionRiseFooting)
					xyStt.x += pBri->m_dPRF_W * se;
				CHgBaseDrawStd baseDraw;
				baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, bDimTotal, FALSE, FALSE);
			}
		}
	}

	*pDomP << Dom;
}

// 종단면도 상면슬래브 헌치부 철근
// long nCycle : #0031622에 의해 추가한 인수
void CDrawVertSection::DrawRebarShear_VertSection_UpperSlabDanbu(CDomyun *pDomP, long nCycle, BOOL bStt, BOOL bMarkRebar, 
	BOOL bDimRebar, BOOL bInput)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);

	CString strSlab = _T("");
	if(pBri->IsBoxType()) strSlab = _T("상부슬래브");

	// 철근 가져와서 그리기
	CTwinVectorArray tvArr;
	pBri->GetTvRebarShear_VertSection_UpperSlabDanbu(tvArr, bStt, nCycle);

	// 전단철근 그리기
	m_pStd->DrawTvArrayWidthDashDot(&Dom, tvArr, FALSE);

	long i = 0; for(i = 0; i < tvArr.GetSize(); i++)
	{
		CString sDescription	= _T("");
		sDescription.Format("%s %s 우각부 보강 조립철근%d", strSlab, bStt ? "시점" : "종점", i+1);
		CString sMark	= pBri->GetMarkRebarShear(0, bInput, FALSE, FALSE, sDescription);
		if(bMarkRebar && m_pStd->IsMarkRebar(sMark) && tvArr.GetSize() > 0)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
			CConcSymbol sym(m_pStd->m_pARoadOptionStd);
			CTwinVector tv	= tvArr.GetAt(i);
			long nRate	= i+1;//i == 0 ? 1 : 4;
			CDPoint xy	= tv.m_v1 + tv.GetXyDir() * tv.GetLength()/(tvArr.GetSize()+1)*nRate;
			sym.RebarCountMarkOne(&Dom, xy, tv.GetXyDir());

			// 선
			CDPoint xy1	= tvArr.GetAt(0).GetXyMid();
			CDPoint xy2	= tvArr.GetAt(tvArr.GetSize()-1).GetXyMid();
			if(!bStt)
				GetSwap(xy1, xy2);

			double dLen	= ~(xy1-xy2);
			CDPoint xyDir	= (xy2-xy1).Unit();
			xy1	= xy;
			xy2	= xy1 + xyDir * dLen;
			
			CString sWall	= bStt ? "시점벽체좌측면" : "종점벽체우측면";
			
			if(pBri->IsOutWall(bStt))
			{
				CTwinVector tv	= pBri->GetTvOffset_OutWall(0, bStt, FALSE, 0);
				GetXyMatchLineAndLine(tv.m_v1, tv.GetXyDir(), xy1, (xy1-xy2).Unit(), xy2);
			}
			
			Dom.LineTo(xy1, xy2);

			// 마킹
			CString sDia	= pBri->m_pARcBridgeDataStd->GetStringDia(pBri->GetDiaRebarStirrupDanbu(bStt, i, TRUE), pBri->GetValueFy(ePartUpperSlab, TRUE));
			// sym.RebarMarkLineSide(&Dom, xy2, sMark, sDia, bStt, 0, FALSE);

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = sMark;
			mark_input_data.dia = sDia;
			mark_input_data.position = xy2;
			mark_input_data.ctc = pBri->m_dCTC_MainRebar/2 * pBri->m_nCountStirrupCtcMainRebar[iUPPER][bStt? 0:1];;
			mark_input_data.type_rebar = 18;
			mark_input_data.left_dir =  bStt;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.line_rebar = TRUE;
			mark_view_data.mark_pos = eMarkSide;
			
			sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}
	}

	if(bDimRebar)
	{

	}

	*pDomP << Dom;
}

// 종단면도 하면슬래브 헌치부 철근
void CDrawVertSection::DrawRebarShear_VertSection_LowerSlabDanbu(CDomyun *pDomP, long nCycle, BOOL bStt, BOOL bMarkRebar, 
	BOOL bDimRebar, BOOL bInput)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(!pBri->IsBoxType()) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);

	// 철근 가져와서 그리기
	CTwinVectorArray tvArr;
	pBri->GetTvRebarShear_VertSection_LowerSlabDanbu(tvArr, bStt, nCycle);

	// 전단철근 그리기
	m_pStd->DrawTvArrayWidthDashDot(&Dom, tvArr, FALSE);

	long i = 0; for(i = 0; i < tvArr.GetSize(); i++)
	{
		CString sDescription	= _T("");
		sDescription.Format("하부슬래브 %s 우각부 보강 조립철근%d", bStt ? "시점" : "종점", i+1);
		CString sMark	= pBri->GetMarkRebarShear(0, bInput, FALSE, FALSE, sDescription);
		if(bMarkRebar && m_pStd->IsMarkRebar(sMark) && tvArr.GetSize() > 0)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
			CConcSymbol sym(m_pStd->m_pARoadOptionStd);
			CTwinVector tv	= tvArr.GetAt(i);
			long nRate	= i+1;//i == 0 ? 1 : 4;
			CDPoint xy	= tv.m_v1 + tv.GetXyDir() * tv.GetLength()/(tvArr.GetSize()+1)*nRate;
			sym.RebarCountMarkOne(&Dom, xy, tv.GetXyDir());

			// 선
			CDPoint xy1	= tvArr.GetAt(0).GetXyMid();
			CDPoint xy2	= tvArr.GetAt(tvArr.GetSize()-1).GetXyMid();
			if(!bStt)
				GetSwap(xy1, xy2);

			double dLen	= ~(xy1-xy2);
			CDPoint xyDir	= (xy2-xy1).Unit();
			xy1	= xy;
			xy2	= xy1 + xyDir * dLen;

			CString sWall	= bStt ? "시점벽체좌측면" : "종점벽체우측면";

			if(pBri->IsOutWall(bStt))
			{
				CTwinVector tv	= pBri->GetTvOffset_OutWall(0, bStt, FALSE, 0);
				GetXyMatchLineAndLine(tv.m_v1, tv.GetXyDir(), xy1, (xy1-xy2).Unit(), xy2);
			}

			Dom.LineTo(xy1, xy2);

			// 마킹
			CString sDia	= pBri->m_pARcBridgeDataStd->GetStringDia(pBri->GetDiaRebarStirrupDanbu(bStt, i, FALSE), pBri->GetValueFy(ePartLowerSlab, TRUE));
			// sym.RebarMarkLineSide(&Dom, xy2, sMark, sDia, bStt, 0, FALSE);
			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = sMark;
			mark_input_data.dia = sDia;
			mark_input_data.position = xy2;
			mark_input_data.ctc = pBri->m_dCTC_MainRebar/2 * pBri->m_nCountStirrupCtcMainRebar[iLOWER][bStt? 0:1];;
			mark_input_data.type_rebar = 18;
			mark_input_data.left_dir =  bStt;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.line_rebar = TRUE;
			mark_view_data.mark_pos = eMarkSide;

			sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}
	}

	if(bDimRebar)
	{

	}

	*pDomP << Dom;
}

// 상부슬래브 단부철근에 배력철근 그림
void CDrawVertSection::DrawRebarSupport_VertSection_UpperSlabDanBu(CDomyun *pDomP, BOOL bStt, BOOL bMarkRebar, BOOL bDimRebar, BOOL bInput)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	// 철근 가져와서 그리기
	CHgBaseDrawStd baseDraw;
	CDPointArray xyArr;
	pBri->GetXyRebarSupport_VertSection_UpperSlabDanBu(xyArr, bStt);
	CDPoint xyDirScale	= bStt ? ToDPointFrDegree(135) : ToDPointFrDegree(45);
	baseDraw.DrawPointRebar(&Dom, xyArr, xyDirScale);

	// 치수 기입
	if(bDimRebar)
	{

	}


	// 마킹
	if(bMarkRebar && m_pStd->IsMarkRebar(""))
	{

	}


	*pDomP << Dom;
}

void CDrawVertSection::DrawRebarSupport_VertSection_LowerSlabDanBu(CDomyun *pDomP, BOOL bStt, BOOL bMarkRebar, BOOL bDimRebar, BOOL bInput)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(!pBri->IsBoxType()) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	// 철근 가져와서 그리기
	CHgBaseDrawStd baseDraw;
	CDPointArray xyArr;
	pBri->GetXyRebarSupport_VertSection_LowerSlabDanBu(xyArr, bStt);
	CDPoint xyDirScale	= bStt ? ToDPointFrDegree(135) : ToDPointFrDegree(45);
	baseDraw.DrawPointRebar(&Dom, xyArr, xyDirScale);

	// 치수 기입
	if(bDimRebar)
	{

	}

	// 마킹
	if(bMarkRebar && m_pStd->IsMarkRebar(""))
	{

	}


	*pDomP << Dom;
}

// 종단면 상부 슬래브 헌치부 배력철근 그리기
void CDrawVertSection::DrawRebarSupport_VertSection_UpperSlabHunch(CDomyun *pDomP, long nIdxHunch, BOOL bMarkRebar, BOOL bDimRebar, BOOL bInput, CDRect rectLimit)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	// 철근 가져와서 그리기
	CDPointArray xyArrLeft, xyArrRight;
	pBri->GetXyRebarSupport_VertSection_UpperSlabHunch(xyArrLeft, xyArrRight, nIdxHunch);

	// rectLimit 적용
	if(rectLimit != CDRect(0, 0, 0, 0))
	{
		CTwinVectorArray tvArrLimit;
		CVectorArray vecArrLimit;
		tvArrLimit.SetFromRect(rectLimit);
		tvArrLimit.GetToVectorArray(vecArrLimit);

		CRebarPlacing rb;
		rb.GetXyByVectorArrayArea(xyArrLeft, vecArrLimit, TRUE);
		rb.GetXyByVectorArrayArea(xyArrRight, vecArrLimit, TRUE);
	}

	CHgBaseDrawStd baseDraw;
	baseDraw.DrawPointRebar(&Dom, xyArrLeft);
	baseDraw.DrawPointRebar(&Dom, xyArrRight);
	
	// 치수 기입
	if(bDimRebar)
	{
	
	}


	// 마킹
	CDPointArray xyArrTot;
	long i = 0; for(i = 0; i < xyArrLeft.GetSize(); i++)
		xyArrTot.Add(xyArrLeft.GetAt(i));
	for(i = 0; i < xyArrRight.GetSize(); i++)
		xyArrTot.Add(xyArrRight.GetAt(i));
	CString sDescription	= _T("");
	sDescription.Format("상부슬래브 헌치부 배력철근");
	CString sMark	= pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescription);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		CRebar *pRb = pBri->GetRebarSupportByDescription(sDescription);
		CPlaceRebarByBlock *pPlace		= &pBri->m_placeByBlock_SupportRebar_UpperSlab[iLOWER][0];
		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
		mark.SetRebar(xyArrTot);

		// 설정
		mark.m_sMark					= sMark;
		mark.m_sDia						= pBri->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(ePartUpperSlab));
		mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_LOWER;
		mark.m_nTypeMarkDir				= nIdxHunch==0 ? TYPE_MARKDIR_RIGHT : TYPE_MARKDIR_LEFT;
		mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
		mark.m_dPosScaleMarkStt			= 0.5;
		mark.m_dCtc = pBri->GetCTCRebarSupport_UpperSlabHunch();
		mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;
		mark.MarkRebar();
	}


	*pDomP << Dom;
}

// 종단면 상부슬래브 스트럽 철근 그림.(각 벽체별)
// BOOL bVirtual : 스트럽 철근 가상 외곽만 그림.
// long nIdxInputStirrup : 입력중인 스터럽 철근
// bDrawDim : Input용치수선과 도면용치수선이 달라서 인자 추가. 
//  InPut에서는 B@C 로 표현하고 도면에서는 5@250 
void CDrawVertSection::DrawRebarAll_SlabGirder(CDomyun *pDomP, long nJ, BOOL bDimRebar, BOOL bMarkRebar, long nDanDim, BOOL bInput, BOOL bVirtual, BOOL bStt, BOOL bLeft, BOOL bGagak, BOOL bJongGirder, long nIdxInputStirrup, BOOL bApplyVert, BOOL bUpperSlabGirder, BOOL bDrawDim)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(bJongGirder)
	{
		if(!pBri->IsGirderRebarJong(bStt, bLeft, bGagak)) return;
	}
	else
	{
		if(!pBri->IsGirderRebar(nJ, bUpperSlabGirder)) return;
	}

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);
	CHgBaseDrawStd baseDraw;

	double dScaleDom = Dom.GetScaleDim();
	double dRebarRadius = REBAR_RADIUS * dScaleDom / 50.;
	long dan(0), upper(0);
	CRebarPlacing rb;
	CTwinVectorArray tvArrStrup;
	CDPointArray xyArr, xyArrDirApplyScale;
	CString strSlab = _T("");
	if(pBri->IsBoxType())
	{
		strSlab = bUpperSlabGirder ? _T("상부") : _T("하부");
	}
	// 거더철근 기준점 정함
	pBri->SetDataDefaultRebarGirder(nJ, TRUE, TRUE, bStt, bLeft, bGagak, bJongGirder, bApplyVert, bUpperSlabGirder);
	
	// 스트럽 철근 (수직전단철근) /////////////////////////////////
	CGirderSectionRebar *pGirderRB		= bJongGirder ? pBri->GetGirderRebarJong(bStt, bLeft, bGagak) : &pBri->m_SlabGirderRebar[bUpperSlabGirder ? iUPPER : iLOWER][nJ];
	pBri->GetTvRebarStirrup_SlabGrider(tvArrStrup, nJ, bStt, bLeft, bGagak, bJongGirder, TRUE, bUpperSlabGirder, dScaleDom);
	if(tvArrStrup.GetSize() == 0) return;
	

	if(bVirtual)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);

		CDRect rect;
		rect	= tvArrStrup.GetRect();
		Dom.Rectangle(rect);
		*pDomP << Dom;
		return;
	}
	else
	{
		// 스트럽 철근은 주철근을 감싸도록 그려야 됨.
		//rb.GetOffsetTvArray(tvArrStrup, -REBAR_RADIUS);
		if(!bInput || nIdxInputStirrup == -1)
		{
			baseDraw.DrawTvArray(&Dom, tvArrStrup);
		}
		else
		{
			CDRect rect	= tvArrStrup.GetRect();

			CString sNum	= _T("");
			sNum.Format("%d", nIdxInputStirrup+1);
			long i = 0; for(i = 0; i < tvArrStrup.GetSize(); i++)
			{
				CTwinVector tv	= tvArrStrup.GetAt(i);
				
				if(tv.m_sInfo.Right(tv.m_sInfo.GetLength()-2) == sNum)
				{
					if(tv.m_sInfo.Find("하", 0) != -1)
					{
						m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
						double dXStt	= rect.left + dRebarRadius;
						double dXSttCur	= tv.GetXyLeft().x + dRebarRadius;
						double dLenCur	= tv.GetHorLength() - dRebarRadius*2;

						CDoubleArray dArrDim;
						CStringArray sArrText;
						CStringArray sArrUnderText;
						long nDimDan	= 0;
						CString sDir	= _T("BOTTOM");
						CDPoint xyStt	= CDPoint(dXStt, tv.m_v1.y);

						dArrDim.Add(dXSttCur-dXStt);
						sArrText.Add("A"+sNum);
						sArrUnderText.Add(COMMA(dXSttCur-dXStt));
						
						dArrDim.Add(dLenCur);
						sArrText.Add("B"+sNum);
						sArrUnderText.Add(COMMA(dLenCur));

						baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
					}

					m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);
				}
				else
				{
					m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);
				}
				baseDraw.DrawTv(&Dom, tv);
			}
		}

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);
		CString sDescription	= _T("");
		if(bJongGirder)
		{
			sDescription.Format("%s 종거더 수직전단철근", bLeft ? "좌측" : "우측");
		}
		else
		{
			sDescription.Format("지점%d %s거더부 수직전단철근", nJ+1, strSlab);
		}
		CString sMark	= pBri->GetMarkRebarEtc(G_REBAR_ST, bInput, FALSE, FALSE, sDescription, TRUE);
		if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
		{
			// 수직선과 수평선을 분리.
			CTwinVectorArray tvArrHor;
			CTwinVectorArray tvArrVer;
			CTwinVector tv;
			long i = 0; for(i = 0; i < tvArrStrup.GetSize(); i++)
			{
				tv	= tvArrStrup.GetAt(i);
				if(tv.GetVerLength() > 0 && Compare(tv.GetHorLength(), 0.0, "="))
					tvArrVer.Add(tv);
				else
					tvArrHor.Add(tv);
			}

			// 구해진 철근들을 마크하기 좋게 정렬 //////////
			for(i = 0; i < tvArrVer.GetSize(); i++)
			{
				tv	= tvArrVer.GetAt(i);
				if(tv.m_v1.y > tv.m_v2.y)
				{
					GetSwap(tv.m_v1, tv.m_v2);
				}
			}

			CTwinVector tv1;
			for(i = 0; i < tvArrVer.GetSize()-1; i++)
			{
				for(long j = i; j < tvArrVer.GetSize(); j++)
				{
					tv	= tvArrVer.GetAt(i);
					tv1	= tvArrVer.GetAt(j);
					if(tv.m_v1.x > tv1.m_v1.x)
					{
						tvArrVer.SetAt(i, tv1);
						tvArrVer.SetAt(j, tv);
					}
				}
			}
			//////////////////////////////////////////////////
			
			

			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
			CRebar *pRb = pBri->GetRebarEtcByDescription(sDescription, TRUE);

			CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
			mark.SetRebar(tvArrVer, tvArrHor);
			mark.m_sMark					= sMark;
			mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pGirderRB->m_dDiaStirrup, pBri->GetValueFy(bUpperSlabGirder? ePartUpperSlab:ePartLowerSlab));
			mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_END;	// 지름 표시위치 찾는 방법
			mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;	// TYPE_MARKPOS_POS일때 옵션
			mark.m_dPosScaleMarkStt			= 0.5;
			mark.m_nTypeMarkDir				= TYPE_MARKDIR_RIGHT;
			mark.m_dCtc = pGirderRB->m_dCTC;
			mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;
			mark.MarkRebar();
		}
	}

	// 스트럽 내부철근(보강철근임)
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);
	for(upper = 0; upper < 2; upper++)
	{
		BOOL bUpper	= (upper == iUPPER);
		for(dan = 1; dan < MAX_REBAR_DAN; dan++)
		{
			pBri->GetTvRebarMain_SlabGirder(tvArrStrup, nJ, upper, dan, bStt, bLeft, bGagak, bJongGirder, bUpperSlabGirder);

			// 스트럽 내부 철근(선)
			// 2단부터는 배력근의 아래위로 이동해야 됨.
			long i = 0; for(i = 0; i < tvArrStrup.GetSize(); i++)
			{
				CTwinVector tv	= tvArrStrup.GetAt(i);
				CTwinVector tvTmp	= tv;
				CDPoint xyDir	= ToDPointFrDegree(tv.GetXyDir(bUpper).GetAngleDegree()-90);
				tv.m_v1			+= (xyDir * dRebarRadius);
				tv.m_v2			+= (xyDir * dRebarRadius);
				GetXyMatchLineAndLine(tvTmp.m_v1, CDPoint(0, -1), tv.m_v2, tv.GetXyDir(TRUE), tv.m_v1);
				GetXyMatchLineAndLine(tvTmp.m_v2, CDPoint(0, -1), tv.m_v1, tv.GetXyDir(TRUE), tv.m_v2);

				tvArrStrup.SetAt(i, tv);
			}
			m_pStd->DrawTvArray(&Dom, tvArrStrup);
		}
	}


	// 주철근(내외부) ///////////////////////////////////
	// 상하.
	double dCoverFromt1Dan	= 0;	// 1단에서부터의 거리

	for(upper = 0; upper < 2; upper++)
	{
		BOOL bUpper		= (upper == iUPPER);
		
		//상부슬래이브상면, 하부슬래이브하면인경우 bOutter = TRUE:
		BOOL bOutter = TRUE;
		if(bUpperSlabGirder)
			bOutter = bUpper;
		else
			bOutter = !bUpper;

		for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

			// 주철근
			if(dan == 0)
				pBri->GetxyRebarSupport_SlabGirder(xyArr, xyArrDirApplyScale, upper, nJ, bStt, bLeft, bGagak, bJongGirder, bUpperSlabGirder, TRUE, dScaleDom);
			else
				pBri->GetXyRebarSupport_SlabGirderInner(xyArr, nJ, upper, dan, bStt, bLeft, bGagak, bJongGirder, bUpperSlabGirder);
			baseDraw.DrawPointRebar(&Dom, xyArr, xyArrDirApplyScale);
			
			if(xyArr.GetSize() == 0) continue;

			// 1단에서 현재 단까지의 거리(치수 높이를 맞춰주기 위함)
			dCoverFromt1Dan	= bUpper ? (pGirderRB->m_dCoverUpper[dan]-pGirderRB->m_dCoverUpper[0]) : (pGirderRB->m_dCoverLower[dan]-pGirderRB->m_dCoverLower[0]);

			// 치수 기입
			if(bDimRebar && xyArr.GetSize() > 0 && (nDanDim == dan || nDanDim == -1))
			{
				BOOL bTotal	= nDanDim != -1 ? TRUE : FALSE;
				// 전체 단 치수 기입할때는 다음 단의 철근이 있는지 검사해서 없을때 전체 치수를 넣는다.
				if(nDanDim == -1)
				{
					CDPointArray xyArrTmp;
					pBri->GetXyRebarSupport_SlabGirderInner(xyArrTmp, nJ, upper, dan+1, bStt, bLeft, bGagak, bJongGirder, bUpperSlabGirder);
					if(xyArrTmp.GetSize() == 0 || dan == 2)
						bTotal	= TRUE;
				}
				m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

				CDoubleArray dArrDim;
				CStringArray sArrText;
				CStringArray sArrUnderText;
				CDPoint xyStt	= CDPoint(0, 0);
				CString sDir	= _T("");

				if(pGirderRB->m_nType == 0)
				{
					dArrDim.Add(pGirderRB->m_dDist - pGirderRB->m_dCoverSide);
					sArrText.Add("D");
					sArrUnderText.Add("");
				}

				// 시작부 피복
				dArrDim.Add(pGirderRB->m_dCoverSide);
				sArrText.Add("0");
				sArrUnderText.Add("");
				
				// C1
				dArrDim.Add(pGirderRB->m_dC1[upper][dan]);
				sArrText.Add(bOutter ? "C1" : "aC1");
				sArrUnderText.Add("");
				
				// B*C
				if(bDrawDim)
				{
					for(long ix = 0; ix < pGirderRB->m_nB[upper][dan]; ix++)
						dArrDim.Add(pGirderRB->m_dC[upper][dan]);
				}
				else
				{
					dArrDim.Add(pGirderRB->m_nB[upper][dan] * pGirderRB->m_dC[upper][dan]);
				}
				sArrText.Add(bOutter ? "B@C" : "aB@aC");
				sArrUnderText.Add("");

				// C2
				dArrDim.Add(pGirderRB->m_dC2[upper][dan]);
				sArrText.Add(bOutter ? "C2" : "aC2");
				sArrUnderText.Add("");

				// 마지막부 피복
				CString sDan	= _T("");
				//sDan.Format("%s%d단", bUpper ? "상면" : "하면", dan+1);	// 현재는 1단만 지원하므로 아래줄과 같이 변경
				sDan.Format("%s", bUpper ? "상면" : "하면");
				dArrDim.Add(pGirderRB->m_dCoverSide);
				sArrText.Add("0");
				sArrUnderText.Add(sDan);

				// 
				if(pGirderRB->m_nType == 1)
				{
					dArrDim.Add(pGirderRB->m_dDist - pGirderRB->m_dCoverSide);
					sArrText.Add("D");
					sArrUnderText.Add("");
				}

				if(pGirderRB->m_nType == 0)
				{
					xyStt	= pGirderRB->m_xyOrg;
					if(xyArr.GetSize() > 0)
					{
						xyStt.y	= xyArr.GetAt(0).y;
						xyStt.y	+= bUpper ? dCoverFromt1Dan : -dCoverFromt1Dan;
					}
				}
				else
				{
					if(xyArr.GetSize() > 0)
					{
						xyStt	= xyArr.GetAt(0);
						xyStt.y	+= bUpper ? dCoverFromt1Dan : -dCoverFromt1Dan;
					}
					xyStt.x -= pGirderRB->m_dCoverSide;
				}
				sDir	= bUpper ? "TOP" : "BOTTOM";
				if(!bInput)
				{
					sArrText.RemoveAll();
					//sArrUnderText.RemoveAll(); 
				}

				baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDanDim==-1 ? dan+1 : 1, sDir, -1, bTotal, TRUE, TRUE);
			}

			// 마킹
			CString sDescription	= _T("");
			CString sUpper			= bUpper ? "상면" : "하면";
			long nTypeRebar			= bUpper ? G_REBAR_M_U1+dan : G_REBAR_M_L1+dan;
			if(bJongGirder)
			{
				sDescription.Format("%s 종거더 %s 주철근 %d단", bLeft ? "좌측" : "우측", sUpper, dan+1);
			}
			else
			{
				sDescription.Format("지점%d %s거더부 %s 주철근 %d단", nJ+1, strSlab, sUpper, dan+1);
			}
			CString sMark	= pBri->GetMarkRebarEtc(nTypeRebar, bInput, FALSE, FALSE, sDescription, TRUE);
			if(bMarkRebar && m_pStd->IsMarkRebar(sMark) && (nDanDim == dan || nDanDim == -1))
			{
				m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

				CRebar *pRb = pBri->GetRebarEtcByDescription(sDescription, TRUE);
				CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
				
				// 주철근
				mark.SetRebar(xyArr);
				mark.m_nTypeMarkPointUpAndDown	= bUpper ? TYPE_MARKPOINT_UPPER : TYPE_MARKPOINT_LOWER;
				mark.m_nTypeMarkDir				= (dan == 1) ? TYPE_MARKDIR_RIGHT : TYPE_MARKDIR_LEFT;
				mark.m_sMark					= sMark;
				mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_POS;
				mark.m_dPosScaleMarkStt			= (dan == 1) ? 0.9 : 0.3;
				mark.m_sDia						= pBri->m_pARcBridgeDataStd->GetStringDia(pGirderRB->m_dDiaUL[upper][dan], pBri->GetValueFy(bUpperSlabGirder? ePartUpperSlab:ePartLowerSlab));
				mark.m_dAddHeight				= dCoverFromt1Dan;
				mark.m_dCtc = pGirderRB->m_dC[upper][dan];
				mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;

				mark.MarkRebar();
			}
		}
	}

	// 좌우(수평전단철근임)
	pBri->GetxyRebarSupport_SlabGirder(xyArr, xyArrDirApplyScale, 2, nJ, bStt, bLeft, bGagak, bJongGirder, bUpperSlabGirder);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);
	baseDraw.DrawPointRebar(&Dom, xyArr, xyArrDirApplyScale);

	if(bDimRebar && xyArr.GetSize() > 0)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		CDoubleArray dArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		CDPoint xyStt	= CDPoint(0, 0);
		CString sDir	= _T("");

		if(bUpperSlabGirder)
		{
			// 아래쪽 거리
			dArrDim.Add(pGirderRB->m_dCoverLower[0]);
			sArrText.Add("dc'");

			// F1
			dArrDim.Add(pGirderRB->m_dF1);
			sArrText.Add("F1");

			// E*F
			if(bDrawDim)
			{
				for(long ix = 0; ix < pGirderRB->m_nE; ix++)
					dArrDim.Add(pGirderRB->m_dF);
			}
			else
			{
				dArrDim.Add(pGirderRB->m_nE * pGirderRB->m_dF);
			}
			sArrText.Add("E@F");

			// F2
			dArrDim.Add(pGirderRB->m_dF2);
			sArrText.Add("F2");

			double dRemain	= pBri->m_dTS + pBri->GetHeightHunchByJijum(nJ, bUpperSlabGirder) - (pGirderRB->m_dCoverLower[0] + pGirderRB->m_dF1 + (pGirderRB->m_nE * pGirderRB->m_dF) + pGirderRB->m_dF2 + pGirderRB->m_dCoverUpper[0]);
			if(dRemain > 0)
			{
				dArrDim.Add(dRemain);
				sArrText.Add("0");
			}


			// 위쪽 거리
			dArrDim.Add(pGirderRB->m_dCoverUpper[0]);
			sArrText.Add("dc");

			xyStt	= rb.GetXyEdgePoint(xyArr, RIGHTDOWN_AREA);
			xyStt.y	-= pGirderRB->m_dF1;
			xyStt.y -= pGirderRB->m_dCoverLower[0];
			sDir	= "RIGHT";
		}
		else //하부슬래이브일 경우에는 뒤집어진다.
		{
			// 아래쪽 거리
			dArrDim.Add(pGirderRB->m_dCoverLower[0]);
			sArrText.Add("dc");

			// F1
			dArrDim.Add(pGirderRB->m_dF2);
			sArrText.Add("F2");

			// E*F
			if(bDrawDim)
			{
				for(long ix = 0; ix < pGirderRB->m_nE; ix++)
					dArrDim.Add(pGirderRB->m_dF);
			}
			else
			{
				dArrDim.Add(pGirderRB->m_nE * pGirderRB->m_dF);
			}
			sArrText.Add("E@F");

			// F2
			dArrDim.Add(pGirderRB->m_dF1);
			sArrText.Add("F1");

			double dRemain	= pBri->m_dTS - (pGirderRB->m_dCoverLower[0] + pGirderRB->m_dF1 + (pGirderRB->m_nE * pGirderRB->m_dF) + pGirderRB->m_dF2 + pGirderRB->m_dCoverUpper[0]);
			if(dRemain > 0)
			{
				dArrDim.Add(dRemain);
				sArrText.Add("0");
			}

			// 위쪽 거리
			dArrDim.Add(pGirderRB->m_dCoverUpper[0]);
			sArrText.Add("dc'");

			xyStt	= rb.GetXyEdgePoint(xyArr, RIGHTDOWN_AREA);
			xyStt.y	-= pGirderRB->m_dF2;
			xyStt.y -= pGirderRB->m_dCoverLower[0];
			sDir	= "RIGHT";
		}

		CDRect rect	= GetRectCuttingForGirderDetail(nJ, bStt, bLeft, bGagak, bJongGirder);
		xyStt.x	= rect.right;

		if(!bInput)
		{
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll(); 
		}

		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, 1, sDir, -1, TRUE, TRUE, TRUE);
	}

	
	if(bMarkRebar)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		CString sDescription	= _T("");
		if(bJongGirder)
		{
			sDescription.Format("%s 종거더 수평전단철근", bLeft ? "좌측" : "우측");
		}
		else
		{
			sDescription.Format("지점%d %s거더부 수평전단철근", nJ+1, strSlab);
		}
		CString sMark	= pBri->GetMarkRebarEtc(G_REBAR_HORV, bInput, FALSE, FALSE, sDescription, TRUE);
		if(m_pStd->IsMarkRebar(sMark))
		{
			// 철근 마크를 좌측 끝에만 하기 위해서 좌측 끝에 있는 철근과 x위치가 같은거만 골라낸다.
			CDPointArray xyArrOut;
			CDPoint xyLeft	= rb.GetXyEdgePoint(xyArr, LEFTDOWN_AREA);
			long i = 0; for(i = 0; i < xyArr.GetSize(); i++)
			{
				if(Compare(xyArr[i].x, xyLeft.x, "="))
				{
					xyArrOut.Add(xyArr[i]);
				}
			}
			xyArrOut.Sort(FALSE);

			CRebar *pRb = pBri->GetRebarEtcByDescription(sDescription, TRUE);
			CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
			
			// 주철근
			mark.SetRebar(xyArrOut);
			mark.m_sMark					= sMark;
			mark.m_sDia						= pBri->m_pARcBridgeDataStd->GetStringDia(pGirderRB->m_dDiaSide, pBri->GetValueFy(bUpperSlabGirder? ePartUpperSlab:ePartLowerSlab, TRUE));
			mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_UPPER;
			mark.m_nTypeMarkDir				= TYPE_MARKDIR_LEFT;
			mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_POS;
			mark.m_dPosScaleMarkStt			= 0.2;
			mark.m_dCtc = pGirderRB->m_dF;
			mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;

			mark.MarkRebar();
		}

		if(bJongGirder)
		{
			sDescription.Format("%s 종거더 내측 수평전단철근", bLeft ? "좌측" : "우측");
		}
		else
		{
			sDescription.Format("지점%d %s거더부 내측 수평전단철근", nJ+1, strSlab);
		}
		sMark	= pBri->GetMarkRebarEtc(G_REBAR_HORV_I, bInput, FALSE, FALSE, sDescription, TRUE);
		if(m_pStd->IsMarkRebar(sMark))
		{
			// 철근 마크를 내측만 하기 위해 좌측과 우측 끝에 있는거 빼고 다 골라냄.
			CDPointArray xyArrIn;
			CDPoint xyLeft	= rb.GetXyEdgePoint(xyArr, LEFTDOWN_AREA);
			CDPoint xyRight	= rb.GetXyEdgePoint(xyArr, RIGHTDOWN_AREA);
			long i = 0; for(i = 0; i < xyArr.GetSize(); i++)
			{
				if(!Compare(xyArr[i].x, xyLeft.x, "=") && !Compare(xyArr[i].x, xyRight.x, "="))
				{
					xyArrIn.Add(xyArr[i]);
				}
			}

			// 다 골라내고 나면 한 줄만 남김
			CDPoint xyOrg	= xyArrIn.GetSize() > 0 ? xyArrIn.GetAt(0) : CDPoint(0, 0);
			for(i = 0; i < xyArrIn.GetSize(); i++)
			{
				if(!Compare(xyOrg.x, xyArrIn[i].x, "="))
				{
					xyArrIn.RemoveAt(i);
					i--;
				}
			}

			CRebar *pRb = pBri->GetRebarEtcByDescription(sDescription, TRUE);
			CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
			
			// 주철근
			mark.SetRebar(xyArrIn);
			mark.m_sMark					= sMark;
			mark.m_sDia						= pBri->m_pARcBridgeDataStd->GetStringDia(pGirderRB->m_dDiaSide_I, pBri->GetValueFy(bUpperSlabGirder? ePartUpperSlab:ePartLowerSlab, TRUE));
			mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_UPPER;
			mark.m_nTypeMarkDir				= TYPE_MARKDIR_LEFT;
			mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_POS;
			mark.m_dPosScaleMarkStt			= 0.8;
			mark.m_dAddHeight				= fabs(xyLeft.x - xyOrg.x);
			mark.m_dCtc = pGirderRB->m_dF_I;
			mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;
			mark.MarkRebar();

		}

		
	}
	
	*pDomP << Dom;
}

// 거더 상세도 그리기
CDRect CDrawVertSection::DrawDetail_Girder(CDomyun *pDomP, long nJ, BOOL bStt, BOOL bLeft, BOOL bGagak, BOOL bJongGirder, BOOL bUpperSlabGirder)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	
	if(!pBri) return CDRect(0, 0, 0, 0);
	if(bJongGirder)
	{
		if(!pBri->IsGirderRebarJong(bStt, bLeft, bGagak)) return CDRect(0, 0, 0, 0);
	}
	else
	{
		if(!pBri->IsGirderRebar(nJ,bUpperSlabGirder)) return CDRect(0, 0, 0, 0);
	}

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	// 잘라낼 영역
	CDRect rect	= GetRectCuttingForGirderDetail(nJ, bStt, bLeft, bGagak, bJongGirder, bUpperSlabGirder);

	// 종단면도를 잘라낼 영역만큼 잘라내서 그림.
	if(bJongGirder)
	{
		double dSta	= pBri->GetStationForInputJongGirder(bStt, bLeft, bGagak);
		CDPoint vAng	= CDPoint(0, 1);
		CTwinVectorArray tvArrCross;
		pBri->GetTvCrossSection(tvArrCross, dSta, vAng, FALSE, TRUE, FALSE, 0, FALSE, FALSE, FALSE, bGagak);
		m_pStd->DrawCuttingTvArray(&Dom, tvArrCross, rect, FALSE);
	}
	else
	{
		CTwinVectorArray tvArrSect;
		// 지점에 skew가 있는경우 직각 방향으로 보여주기 위해 슬래브 형상을 그냥 그려준다.
		CGirderSectionRebar *pGirderRB	= &pBri->m_SlabGirderRebar[bUpperSlabGirder ? iUPPER : iLOWER][nJ];
		if(!(nJ==0 || nJ == pBri->m_nQtyJigan) && pGirderRB)
		{
			CDPoint xyBase = pGirderRB->m_xyOrg;
			CTwinVector tvAdd;

			long nIndexInWall = nJ - 1;
			long nIdxHunch = pBri->GetIdxHunchInWall(nIndexInWall, TRUE);
			long nPos = bUpperSlabGirder? iUPPER:iLOWER;

			double dDirY = bUpperSlabGirder? +1.0 : -1.0;
			double dPlusHeight = pBri->m_dHBH[nIdxHunch][nPos];
			

			// 상부 거더=>상부의 하면, 하부거더=>하부의 하면
			tvAdd.m_v1 = xyBase;
			tvAdd.m_v1.y += (pBri->m_dHHL[nIdxHunch][nPos] + (bUpperSlabGirder? pBri->m_dTS : pBri->m_dTS_Lower)) * dDirY;
			tvAdd.m_v1.y += dPlusHeight * dDirY;
			tvAdd.m_v2 = tvAdd.m_v1;
			tvAdd.m_v1.x -= 10000;
			tvAdd.m_v2.x += 10000;
			tvArrSect.Add(tvAdd);
						
			// 좌측
			tvAdd.m_v1 = xyBase;
			tvAdd.m_v2 = xyBase - pBri->m_dWL[nIdxHunch][nPos] * CDPoint(1, 0);
			tvArrSect.Add(tvAdd);
			//좌측: 중간에 높이가 들어갈 경우 추가
			if(dPlusHeight !=0)
			{
				tvAdd.m_v1 = tvAdd.m_v2;
				tvAdd.m_v2.y += dPlusHeight * dDirY;
				tvArrSect.Add(tvAdd);
			}
			tvAdd.m_v1 = tvAdd.m_v2;
			tvAdd.m_v2.x -= pBri->m_dHLW[nIdxHunch][nPos];
			tvAdd.m_v2.y += pBri->m_dHHL[nIdxHunch][nPos]* dDirY;
			tvArrSect.Add(tvAdd);
			tvAdd.m_v1 = tvAdd.m_v2;
			tvAdd.m_v2.x -= 10000;
			tvArrSect.Add(tvAdd);
			
			// 우측
			tvAdd.m_v1 = xyBase;
			tvAdd.m_v2 = xyBase + pBri->m_dWR[nIdxHunch][nPos] * CDPoint(1, 0);
			tvArrSect.Add(tvAdd);
			//좌측: 중간에 높이가 들어갈 경우 추가
			if(dPlusHeight !=0)
			{
				tvAdd.m_v1 = tvAdd.m_v2;
				tvAdd.m_v2.y += dPlusHeight * dDirY;
				tvArrSect.Add(tvAdd);
			}
			tvAdd.m_v1 = tvAdd.m_v2;
			tvAdd.m_v2.x += pBri->m_dHRW[nIdxHunch][nPos];
			tvAdd.m_v2.y += pBri->m_dHHL[nIdxHunch][nPos]* dDirY;
			tvArrSect.Add(tvAdd);
			tvAdd.m_v1 = tvAdd.m_v2;
			tvAdd.m_v2.x += 10000;
			tvArrSect.Add(tvAdd);
			
		}
		else
		{
			tvArrSect = pBri->m_tvArrVertSection;
		}

		m_pStd->DrawCuttingTvArray(&Dom, tvArrSect, rect, FALSE);
		pBri->GetTvVertSectionFxJoint(TRUE, FALSE);
		pBri->GetTvVertSectionFxJoint(FALSE, FALSE);
		if(nJ == 0 && pBri->IsBlockOut(TRUE))
			m_pStd->DrawCuttingTvArray(&Dom, pBri->m_fxJointStt.m_tvArrSection, rect, FALSE);
		else if(nJ == pBri->m_nQtyJigan && pBri->IsBlockOut(FALSE))
			m_pStd->DrawCuttingTvArray(&Dom, pBri->m_fxJointEnd.m_tvArrSection, rect, FALSE);
	}

	// 주철근 가상라인도 그림.
	if(bUpperSlabGirder)
		DrawVirtualRebarMain_VertSection_UpperSlab(&Dom, rect);
	else
		DrawVirtualRebarMain_VertSection_LowerSlab(&Dom, rect);

	Dom.SetCalcExtRect();
	CDRect rectTot	= Dom.GetExtRect();

	*pDomP << Dom;

	return rectTot;
}

// 거더랑 기둥  상세도 그림.
void CDrawVertSection::DrawDetail_Column_VertSection(CDomyun *pDomP, long nIdxWall)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	long nCountInWall	= pBri->GetCountInWall();
	if(nIdxWall == -1) nIdxWall	= nCountInWall-1;
	if(nIdxWall < 0 || nIdxWall > nCountInWall-1) return;
	
	CWallApp *pWall	= pBri->GetInWall(nIdxWall);
	if(!pWall) return;

	// 여기선 일단 기둥의 무늬는 뺀다. ㅡㅡ 
	if(pWall->m_bTopIsColumn)
		pWall->m_Column.GetTvFront(FALSE);

	CTwinVectorArray tvArrVertSection;
	CTwinVectorArray tvArrInWall;
	CTwinVectorArray tvArrFooting;
	CTwinVectorArray tvTot;
	tvArrVertSection	= pBri->m_tvArrVertSection;
	if(pWall->m_bTopIsColumn)
		tvArrInWall	= pWall->m_Column.m_tvArrFront;
	else
		tvArrInWall	= pWall->m_tvArrFront;
	tvArrFooting		= pWall->m_footing.m_tvArrFront;

	tvTot.AddFromTvArray(tvArrVertSection);
	tvTot.AddFromTvArray(tvArrInWall);
	tvTot.AddFromTvArray(tvArrFooting);
	
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	// 기둥에서 좌우로 dMarginHor, 슬래브 상/초하에서 dMarginVer씩 띄워서그림.
	// 잘라낼 영역
	const double dMarginHor	= 1000;
	const double dMarginVer	= 1000;
	CDRect rectColumn	= tvArrInWall.GetRect();
	CDRect rectVert		= tvArrVertSection.GetRect();
	CDRect rectFooting	= tvArrFooting.GetRect();
	
	//Box형일때는 Footing이 없으니 단면도Bottom을 Footing으로 씀.
	if(pBri->IsBoxType())
		rectFooting = rectVert;

	CDRect rect(rectColumn.left - dMarginHor, rectVert.top + dMarginVer, rectColumn.right + dMarginHor, rectFooting.bottom - dMarginVer);

	// 종단면도를 잘라낼 영역만큼 잘라내서 그림.
	m_pStd->DrawCuttingTvArray(&Dom, tvTot, rect, FALSE);

	// 주철근 가상라인도 그려줌.
	DrawVirtualRebarMain_VertSection_UpperSlab(&Dom, rect);
	if(pBri->IsBoxType())
		DrawVirtualRebarMain_VertSection_LowerSlab(&Dom, rect);
	*pDomP << Dom;
}


// 종단면도 주철근 조립도 치수 기입.
void CDrawVertSection::DimRebarMainAssm_VertSection_Upper(CDomyun *pDomP, long nDimDan, long nCycle, long nDrawLeft, long nDan)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(nCycle == -1) nCycle = pBri->GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > pBri->GetCountCycleMainRebar()-1) return;	
	if(!pBri->m_bDetailInput23DanMainRebar)
		nDan	= 0;

	CDPoint xyStt(0, 0),
			xyEnd(0, 0);
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> arrRC;
	
	// 시점부터 종점까지 rc 결합 ////////////
	if(pBri->IsOutWall(TRUE))
	{
		pBri->m_Util.AddRebarInfoCycle(&arrRC, &pBri->m_pArrRCMainOutWall[nCycle][iSTT][iOUTTER][nDan], FALSE);
	}

	pBri->m_Util.AddRebarInfoCycle(&arrRC, &pBri->m_pArrRCMainUpperSlab[nCycle][iUPPER][nDan], FALSE);

	// 종점측은 뒤집어서 합친다.
	if(pBri->IsOutWall(FALSE))
	{
		pBri->m_Util.AddRebarInfoCycle(&arrRC, &pBri->m_pArrRCMainOutWall[nCycle][iEND][iOUTTER][nDan], TRUE);
	}
	//////////////////////////////////////////


	// 시점측과 종점측 철근의 시작과 끝부분중 라운드부 제거
	long nSize	= arrRC.GetSize();
	if(nSize > 0)
	{
		// 상부슬래브 철근이 일체형일 경우 반을 잘라서 두개로 만들어 준다
		// 그래야 치수가 이쁘게 나옴
		long i = 0; for(i = 0; i < arrRC.GetSize(); i++)
		{
			CRebarInfoCycle *pRC	= arrRC.GetAt(i);
			if(pBri->IsMainRebarJointUpperSlabAndUpperSlab(pRC))
			{
				CRebarInfoCycle *pRCNext	= new CRebarInfoCycle;
				pRCNext->m_xyStt	= pRC->m_xyEnd;
				pRCNext->m_xyMid1	= pRCNext->m_xyStt;
				pRCNext->m_xyMid2	= pRC->m_xyMidExt;
				pRCNext->m_xyCen	= pRC->m_xyCenExt;
				pRCNext->m_xyEnd	= pRC->m_xyEndExt;
				pRCNext->m_R_Mid	= pRC->m_R_Mid;

				pRCNext->m_nPosStt	= REBARINFO_POS_UPPERSLAB_UPPER;
				pRCNext->m_nPosEnd	= REBARINFO_POS_ENDWALL_OUTTER;
				arrRC.InsertAt(i+1, pRCNext);

				pRC->m_nPosEnd	= REBARINFO_POS_UPPERSLAB_UPPER;
			}
		}

		if(pBri->IsOutWall(TRUE))
		{
			CRebarInfoCycle *pRC	= arrRC.GetAt(0);
			
			BOOL bSttRebar	= TRUE;
			if(pBri->IsMainRebarJointUpperSlabAndOutWall(pRC, bSttRebar))
			{
				// 라운드 부는 직선으로 분리해서 추가
				CRebarInfoCycle *pRCPrev	= new CRebarInfoCycle;
				pRCPrev->m_nPosStt		= REBARINFO_POS_STTWALL_OUTTER;
				pRCPrev->m_nPosEnd		= REBARINFO_POS_STTWALL_OUTTER;
				pRCPrev->m_xyStt		= CDPoint(pRC->m_xyMid2.x, pRC->m_xyMid1.y);
				pRCPrev->m_xyMid1		= pRC->m_xyMid2;
				arrRC.InsertAt(0, pRCPrev);

				pRC->m_xyStt		= pRC->m_xyMid2;
				pRC->m_xyMid1		= pRC->m_xyEnd;
				pRC->m_xyMid2		= pRC->m_xyMidExt;
				pRC->m_xyEnd		= pRC->m_xyEndExt;
				pRC->m_xyCen		= pRC->m_xyCenExt;
				pRC->m_R_Mid		= pRC->m_R_MidExt;
				pRC->m_nPosStt		= REBARINFO_POS_STTWALL_OUTTER;
				pRC->m_bUseCenExt	= FALSE;
				pRC->m_bUseEndExt	= FALSE;
				pRC->m_bUseMidExt	= FALSE;
			}
			else
			{
				// 라운드 부는 직선으로 분리해서 추가
				CRebarInfoCycle *pRCPrev	= new CRebarInfoCycle;
				pRCPrev->m_nPosStt		= pRC->m_nPosEnd;
				pRCPrev->m_nPosEnd		= pRC->m_nPosEnd;
				pRCPrev->m_xyStt		= CDPoint(pRC->m_xyMid2.x, pRC->m_xyMid1.y);
				pRCPrev->m_xyMid1		= pRC->m_xyMid2;
				arrRC.InsertAt(0, pRCPrev);

				pRC->m_nPosStt			= pRC->m_nPosEnd;
				pRC->m_xyStt			= pRC->m_xyMid2;
				pRC->m_xyMid1			= pRC->m_xyEnd;
				arrRC.SetAt(1, pRC);
			}
		}
		else
		{
			if(1)//nCycle == 0 || nCycle == 2)
			{
				CRebarInfoCycle *pRC	= arrRC.GetAt(0);

				// 상부슬래브 시점 측면 제거 (측면 치수는 따로 기입 하기 때문)
				pRC->m_nPosStt	= pRC->m_nPosEnd;
				pRC->m_xyStt	= pRC->m_xyMid2;
				pRC->m_xyMid1	= pRC->m_xyEnd;
				pRC->m_xyMid2	= pRC->m_xyMid1;
				pRC->m_xyEnd	= pRC->m_xyMid1;
				arrRC.SetAt(0, pRC);
			}
		}

		if(pBri->IsOutWall(FALSE))
		{
			nSize	= arrRC.GetSize();
			CRebarInfoCycle *pRC	= arrRC.GetAt(nSize-1);

			BOOL bSttRebar	= TRUE;
			if(pBri->IsMainRebarJointUpperSlabAndOutWall(pRC, bSttRebar))
			{
				// 종점 기초에 묻히는 라운드부를 직선으로 추가
				CRebarInfoCycle *pRCNext	= new CRebarInfoCycle;
				pRCNext->m_nPosStt			= REBARINFO_POS_ENDWALL_OUTTER;
				pRCNext->m_nPosEnd			= REBARINFO_POS_ENDWALL_OUTTER;
				pRCNext->m_xyStt			= pRC->m_xyEnd;
				pRCNext->m_xyMid1			= CDPoint(pRC->m_xyEnd.x, pRC->m_xyMidExt.y);
				pRCNext->m_xyMid2			= pRCNext->m_xyMid1;
				pRCNext->m_xyEnd			= pRCNext->m_xyMid1;
				arrRC.Add(pRCNext);


				pRC->m_bUseCenExt	= FALSE;
				pRC->m_bUseEndExt	= FALSE;
				pRC->m_bUseMidExt	= FALSE;
				pRC->m_nPosEnd		= REBARINFO_POS_ENDWALL_OUTTER;
			}
			else
			{
				// 종점 기초에 묻히는 라운드부를 직선으로 추가
				CRebarInfoCycle *pRCNext	= new CRebarInfoCycle;
				pRCNext->m_nPosStt		= pRC->m_nPosEnd;
				pRCNext->m_nPosEnd		= pRC->m_nPosEnd;
				pRCNext->m_xyStt		= pRC->m_xyMid1;
				pRCNext->m_xyMid1		= CDPoint(pRC->m_xyMid1.x, pRC->m_xyMid2.y);

				pRC->m_nPosEnd	= pRC->m_nPosStt;
				arrRC.SetAt(nSize-1, pRC);

				arrRC.Add(pRCNext);
			}
		}
		else
		{
			if(1)//nCycle == 0 || nCycle == 2)
			{
				nSize	= arrRC.GetSize();
				CRebarInfoCycle *pRC	= arrRC.GetAt(nSize-1);

				// 상부슬래브 시점 측면 제거 (측면 치수는 따로 기입 하기 때문)
				pRC->m_nPosEnd	= pRC->m_nPosStt;
				pRC->m_xyMid2	= pRC->m_xyMid1;
				pRC->m_xyEnd	= pRC->m_xyMid1;
				arrRC.SetAt(nSize-1, pRC);
			}
		}

		xyStt	= arrRC.GetAt(0)->m_xyStt;
		CRebarInfoCycle *pRC	= arrRC.GetAt(arrRC.GetSize()-1);
		if(pRC->m_nPosStt == pRC->m_nPosEnd)
			xyEnd	= pRC->m_xyMid1;
		else
			xyEnd	= pRC->m_xyEnd;
				
		
		// 시작위치에 피복 적용
		if(pBri->IsOutWall(TRUE))
			xyStt.y -= pBri->m_footingStt.m_dCoverLower[0];
		else
		{
			CTwinVector tv	= pBri->m_tvArrVertSection.GetTvByInfo("시점슬래브측면");
			xyStt.x	= tv.m_v1.x;
		}

		// 마지막위치에 피복적용
		if(pBri->IsOutWall(FALSE))
			xyEnd.y -= pBri->m_footingEnd.m_dCoverLower[0];
		else
		{
			CTwinVector tv	= pBri->m_tvArrVertSection.GetTvByInfo("종점슬래브측면");
			xyEnd.x	= tv.m_v2.x;
		}


		CDomyun Dom(pDomP);
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		m_pStd->DimRebarInfoCycleArray(&Dom, &arrRC, xyStt, xyEnd, TRUE, nDimDan, nDrawLeft);

		if(!pBri->IsOutWall(TRUE) && !pBri->IsOutWall(FALSE))
		{
			*pDomP << Dom;
			double dObq	= (xyEnd-xyStt).Unit().GetAngleDegree();
			Dom.SetDimObq(dObq);
			Dom.DimMoveTo(xyStt.x, xyStt.y, nDimDan+1);
			Dom.DimLineTo(xyEnd.x-xyStt.x, nDimDan+1, COMMA(xyEnd.x-xyStt.x));

			Dom.Rotate(xyStt, ToDPointFrDegree(dObq));
		}
		*pDomP << Dom;
	}

	AhTPADelete(&arrRC, (CRebarInfoCycle*)0);

}

void CDrawVertSection::DimRebarMainAssm_VertSection_Lower(CDomyun *pDomP, long nDimDan, long nCycle, long nDrawLeft, long nDan, BOOL bUpperSlab)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(nCycle == -1) nCycle = pBri->GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > pBri->GetCountCycleMainRebar()-1) return;	
	if(!pBri->m_bDetailInput23DanMainRebar)
		nDan	= 0;

	CDPoint xyStt(0, 0),
			xyEnd(0, 0);
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> arrRC;

	// Box형일때 하부 슬래브는 상부 슬래브와 반대이므로 상부슬래브의 하면은 하부슬래브의 상면이다.
	if(bUpperSlab)
		pBri->m_Util.AddRebarInfoCycle(&arrRC, &pBri->m_pArrRCMainUpperSlab[nCycle][iLOWER][nDan], FALSE);
	else
		pBri->m_Util.AddRebarInfoCycle(&arrRC, &pBri->m_pArrRCMainLowerSlab[nCycle][iUPPER][nDan], FALSE);

	long nSize	= arrRC.GetSize();
	if(nSize > 0)
	{
		CRebarInfoCycle *pRC	= arrRC.GetAt(0);
		CRebarInfoCycle *pRCLast	= arrRC.GetAt(nSize-1);
		if(1)//nCycle == 0 || nCycle == 2)
		{
			pRC->m_xyStt	= pRC->m_xyMid2;
			pRC->m_xyMid1	= pRC->m_xyMid2;

			pRCLast->m_xyMid2	= pRCLast->m_xyMid1;
			pRCLast->m_xyEnd	= pRCLast->m_xyMid1;
		}

		xyStt	= pRC->m_xyStt;
		pRC		= arrRC.GetAt(arrRC.GetSize()-1);
		xyEnd	= pRC->m_xyMid1;

		CDomyun Dom(pDomP);
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		// 시작좌표 끝 좌표를 좌우측 벽체 끝에 닫게 한다.
		if(pBri->IsOutWall(TRUE))
		{
			CTwinVector tvWall	= pBri->m_tvArrVertSection.GetTvByInfo("시점벽체좌측면");
			GetXyMatchLineAndLine(tvWall.m_v1, tvWall.GetXyDir(), xyStt, CDPoint(-1, 0), xyStt);
		}
		else 
		{
			CTwinVector tvSide	= pBri->m_tvArrVertSection.GetTvByInfo("시점슬래브측면");
			GetXyMatchLineAndLine(tvSide.m_v1, tvSide.GetXyDir(), xyStt, CDPoint(-1, 0), xyStt);
		}

		if(pBri->IsOutWall(FALSE))
		{
			CTwinVector tvWall	= pBri->m_tvArrVertSection.GetTvByInfo("종점벽체우측면");
			GetXyMatchLineAndLine(tvWall.m_v1, tvWall.GetXyDir(), xyEnd, CDPoint(1, 0), xyEnd);
		}
		else
		{
			CTwinVector tvSide	= pBri->m_tvArrVertSection.GetTvByInfo("종점슬래브측면");
			GetXyMatchLineAndLine(tvSide.m_v1, tvSide.GetXyDir(), xyEnd, CDPoint(1, 0), xyEnd);
		}
		
		m_pStd->DimRebarInfoCycleArray(&Dom, &arrRC, xyStt, xyEnd, bUpperSlab? FALSE:TRUE, nDimDan, nDrawLeft);

		*pDomP << Dom;
	}

	AhTPADelete(&arrRC, (CRebarInfoCycle*)0);
}

// 시점측종점측 벽체 
void CDrawVertSection::DimRebarMainAssm_VertSection_OutWall(CDomyun *pDomP, long nDimDan, long nCycle, BOOL bStt, long nDrawLeft, long nDan)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(nCycle == -1) nCycle = pBri->GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > pBri->GetCountCycleMainRebar()-1) return;	
	if(!pBri->IsOutWall(bStt)) return;
	if(!pBri->m_bDetailInput23DanMainRebar)
		nDan	= 0;

	CDPoint xyStt(0, 0),
			xyEnd(0, 0);
	long stt	= bStt ? iSTT : iEND;
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> arrRC;
	pBri->m_Util.AddRebarInfoCycle(&arrRC, &pBri->m_pArrRCMainOutWall[nCycle][stt][iINNER][nDan], FALSE);

	long nSize	= arrRC.GetSize();
	if(nSize > 0)
	{
		// 첫번째꺼 라운드부 편집
		// 라운드부는 따로 기입함
		CRebarInfoCycle *pRC	= arrRC.GetAt(0);
		if(pRC->m_R_Mid > 0)
		{
			// 라운드 부는 직선으로 분리해서 추가
			CRebarInfoCycle *pRcPrev	= new CRebarInfoCycle;
			pRcPrev->m_nPosStt		= pRC->m_nPosEnd;
			pRcPrev->m_nPosEnd		= pRC->m_nPosEnd;
			pRcPrev->m_bExist		= pRC->m_bExist;
			if(pRC->m_nPosStt != pRC->m_nPosEnd)
			{
				pRcPrev->m_xyStt		= CDPoint(pRC->m_xyMid2.x, pRC->m_xyMid1.y);
				pRcPrev->m_xyMid1		= pRC->m_xyMid2;
			}
			else
			{
				pRcPrev->m_xyStt		= CDPoint(pRC->m_xyStt.x, pRC->m_xyStt.y);
				pRcPrev->m_xyMid1		= pRC->m_xyMid1;
			}
			arrRC.InsertAt(0, pRcPrev);

			if(ComparePt(pRC->m_xyMid2,  pRC->m_xyEnd, _T("=")) == FALSE)
			{
				pRC->m_nPosStt			= pRC->m_nPosEnd;
				pRC->m_xyStt			= pRC->m_xyMid2;
				pRC->m_xyMid1			= pRC->m_xyEnd;
				arrRC.SetAt(1, pRC);
			}
		}
		else
		{
			//#ARCBRIDGE-3725 외측벽체 내측철근 치수선 중 하단 피복 위치 조정
			if(pRC->m_nPosStt != pRC->m_nPosEnd)
			{
				pRC->m_xyStt		= pRC->m_xyMid1;
			}
		}

		xyStt	= arrRC.GetAt(0)->m_xyStt;
		
		pRC	= arrRC.GetAt(arrRC.GetSize()-1);
		xyEnd	= pRC->m_xyEnd;

		// 시작위치에 피복 적용
		CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;
		if(nCycle == 0 || nCycle == 2)
		{
			if(pBri->IsBoxType())
				xyStt.y -= pBri->m_dCoverLower_LowerSlab[0];
			else
				xyStt.y -= pFooting->m_dCoverLower[0];
		}
		else if(nCycle == 1 || nCycle == 3)
		{
			CDPoint xyMatch(0, 0);
			CTwinVector tvLower	= pFooting->m_tvArrFront.GetTvByInfo("기초하면");
			xyStt.y = tvLower.m_v1.y;
			if(pBri->IsBoxType())
			{
				tvLower = pBri->m_tvArrVertSection.GetTvByInfo(bStt? _T("하부슬래브하면(시점벽체아래)") : _T("하부슬래브하면(종점벽체아래)"));
				GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), xyStt, CDPoint(0, 1), xyStt);
			}
			//GetXyMatchLineAndLine(pRC->m_xyStt, (pRC->m_xyStt-pRC->m_xyMid1).Unit(), tvLower.m_v1, tvLower.GetXyDir(), xyMatch);
			
		}

		// 마지막 위치에 피복 적용
		if(nCycle == 0 || nCycle == 2)
			xyEnd.y += pBri->m_dCoverUpper_UpperSlab[0];
		else if(nCycle == 1 || nCycle == 3)
		{
			CDPoint xyMatch(0, 0);
			xyMatch	= pBri->GetXyMatchLineAndSlabUpper(xyEnd, CDPoint(0, 1), 0, 0);
			xyEnd.y	= xyMatch.y;
		}
		

		CDomyun Dom(pDomP);
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		m_pStd->DimRebarInfoCycleArray(&Dom, &arrRC, xyStt, xyEnd, bStt ? FALSE : TRUE, nDimDan, nDrawLeft);

		*pDomP << Dom;
	}

	AhTPADelete(&arrRC, (CRebarInfoCycle*)0);
}

// 주철근 조립도 그림.
// nCycle : 그릴 cycle
// nDrawLeft : -1 -> 왼쪽만 그림, 0 : 좌우 모두 그림, 1 : 오른쪽만 그림.
void CDrawVertSection::DrawMainRebarAssm(CDomyun *pDomP, long nCycle, long nDrawLeft, BOOL bDrawVertSection, BOOL bVertDir, BOOL bInput)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(nCycle == -1) nCycle = pBri->GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > pBri->GetCountCycleMainRebar()-1) return;		

	CDomyun Dom(pDomP);
	long nDimDan	= 2;

	// 말뚝 비율을 기본값으로
	pBri->m_footingEnd.m_exFooting.m_Pile.InitScaleCutting();

	// 모든 객체들의 동기화를 해줌.
	//pBri->SyncBridge(0, !bVertDir);
	pBri->GetTvVertSection_Std(0, !bVertDir, FALSE);
	
	// 종단면 그림
	BOOL bPatternRoundRectWall	= FALSE;
	CWallApp *pWall	= pBri->GetInWall(0);
	if(pWall)
	{
		bPatternRoundRectWall	= pWall->m_bTopIsColumn;

	}
	if(bDrawVertSection)
		DrawVertSectionAll(&Dom, FALSE, FALSE, FALSE, FALSE, FALSE, bPatternRoundRectWall);

	// 철근 그리기(치수, 이음 포함)
	long nCountWall	= pBri->GetCountInWall();

	pBri->SyncBridgeRebar(FALSE, bVertDir?-1:1, TRUE);
	
	// 주철근 그림.
	DrawRebarMain_VertSectionAll(&Dom, nCycle, TRUE, FALSE, TRUE, nDrawLeft, bInput);

	// 슬래브 치수 기입
	if(pBri->IsBoxType())
	{
		DimRebarMainAssm_VertSection_BoxOutSide(&Dom, nDimDan, nCycle, nDrawLeft);
		// 하부슬래브의 상면
		DimRebarMainAssm_VertSection_Lower(&Dom, nDimDan, nCycle, nDrawLeft, 0, FALSE);
	}
	else
	{
		DimRebarMainAssm_VertSection_Upper(&Dom, nDimDan, nCycle, nDrawLeft);
		DimRebarMainAssm_VertSection_FootingLower(&Dom, nDimDan, nCycle, nDrawLeft);
		DimRebarMainAssm_VertSection_UpperSlabSide(&Dom, nDimDan, nCycle, nDrawLeft);
	}
	// 상부슬래브의 하면
	DimRebarMainAssm_VertSection_Lower(&Dom, nDimDan, nCycle, nDrawLeft);
	DimRebarMainAssm_VertSection_OutWall(&Dom, nDimDan, nCycle, TRUE, nDrawLeft);
	DimRebarMainAssm_VertSection_OutWall(&Dom, nDimDan, nCycle, FALSE, nDrawLeft);
	
	// 벽체들
	for(long wall = 0; wall < nCountWall; wall++)
	{
		CWallApp *pWall	= pBri->GetInWall(wall);
		if(pWall)
		{
			m_pStd->m_pDrawWall->DimRebarMainAssm_Front(&Dom, pWall, nDimDan, nCycle, TRUE, nDrawLeft);
			m_pStd->m_pDrawWall->DimRebarMainAssm_Front(&Dom, pWall, nDimDan, nCycle, FALSE, nDrawLeft);
		}
	}

	// 기초들
	long nCountFooting	= pBri->GetCountFooting();
	for(long footing = 0; footing < nCountFooting; footing++)
	{
		CFootingApp *pFooting	= pBri->GetFooting(footing);
		if(pFooting)
		{
			m_pStd->m_pDrawFooting->DimRebarMainAssm_Front(&Dom, pFooting, nDimDan, nCycle, nDrawLeft);
		}
	}

	CHgBaseConcStd baseConc;
	
	// 사이클 정보 표시
	CString sCycle	= _T("");
	CString sCTC	= _T("");
	sCycle.Format("%s CYCLE", m_pStd->m_pARcBridgeDataStd->GetStringCircleNum(nCycle+1));
	sCTC.Format("C.T.C %.0f", pBri->m_dCTC_MainRebar);

	// 글자 쓸 위치
	Dom.SetTextAlignHorz(TA_CENTER);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SMALL_TITLE);
	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();
	double dStaCuttingOrg	= m_pStd->GetStaCuttingOrg();
	CDPoint xyText	= CDPoint(dStaCuttingOrg, rect.bottom+Dom.GetTextHeight()*2);
	if(nDrawLeft == -1) xyText.x -= Dom.GetTextWidth(sCTC);
	else if(nDrawLeft == 1) xyText.x += Dom.GetTextWidth(sCTC);

	// 글자 씀
	Dom.TextOut(xyText, sCycle);
	*pDomP << Dom;

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_STRESS_TEXT);
	xyText.y -= Dom.GetTextHeight()*1.5;
	Dom.TextOut(xyText, sCTC);

	pBri->GetTvVertSection_Std(0, !bVertDir, FALSE, TRUE);
	*pDomP << Dom;
}

// 
CDRect CDrawVertSection::DrawMainRebarAssmStd(CDomyun *pDomP, long nCycleLeft, long nCycleRight, BOOL bVertDir, BOOL bInput)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return CDRect(0, 0, 0, 0);
	
	CDomyun Dom(pDomP);

	CDRect rect(0, 0, 0, 0);
	if(nCycleLeft == nCycleRight)
	{
		DrawMainRebarAssm(&Dom, nCycleLeft, 0, TRUE, bVertDir, bInput);

		Dom.SetCalcExtRect();
		rect				= Dom.GetExtRect();
	}
	else
	{
		DrawMainRebarAssm(&Dom, nCycleLeft, -1, TRUE, bVertDir, bInput);
		DrawMainRebarAssm(&Dom, nCycleRight, 1, FALSE, bVertDir, bInput);

		// 중심선
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_CENT);
		
		Dom.SetCalcExtRect();
		rect				= Dom.GetExtRect();
		double dStaCuttingOrg	= m_pStd->GetStaCuttingOrg();

		double dLenExt	= Dom.Always(5);
		double dLenMinusBottom	= Dom.Always(Dom.GetDimVLen()+Dom.GetDimExo()+Dom.GetDimLevelDis());
		CFootingApp *pFooting	= pBri->GetFooting(0);
		if(pFooting)
		{
			dLenMinusBottom += pFooting->GetHeight();
		}

		Dom.LineTo(dStaCuttingOrg, rect.bottom+dLenExt, dStaCuttingOrg, rect.top+dLenMinusBottom);
	}

	*pDomP << Dom;

	
	return rect;
}

// 표준 단면도 그리기(도면 출력용)
// long nVertDir : -1 : 직거리, 0 : 자동(원래설정), 1 : 사거리
CDRect CDrawVertSection::DrawStandardSectionStd(CDomyun *pDomP, BOOL bDimRebar, BOOL bMarkRebar, BOOL bMarkMainRebar, long nVertDir)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return CDRect(0, 0, 0, 0);
	
	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	
	CDRect rect(0, 0, 0, 0);
	CDomyun Dom(pDomP);
	long nLeft	= 0;

	BOOL bVertDir	= pBri->IsVertDir();
	if(nVertDir != 0)
	{
		bVertDir	= nVertDir == -1 ? TRUE : FALSE;
	}
	nVertDir	= bVertDir ? -1 : 1;

	// 종단면도 그리기
	// 말뚝 비율을 기본값으로
	pBri->m_footingEnd.m_exFooting.m_Pile.InitScaleCutting();
	// 모든 객체들의 동기화를 해줌.
	//pBri->SyncBridge(nLeft, !bVertDir);
	pBri->GetTvVertSection_Std(nLeft, !bVertDir, FALSE);
	pBri->SyncBridgeRebar(FALSE, nVertDir);

	// 종단면 그림
	DrawVertSectionAll(&Dom, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE);

	// 주철근 1cycle 그리기
	DrawRebarMain_VertSectionAll(&Dom, 0, TRUE, FALSE, bMarkMainRebar, 0, FALSE, TRUE);

	// 배력철근/전단철근 그리기
	long dan(0), upper(0), stt(0), wall(0), left(0), i(0);
	BOOL bUpper(TRUE), bStt(TRUE), bLeft(TRUE);
	
	// 상부슬래브
	long nDimDan = 0;
	for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		for(upper = 0; upper < 2; upper++)
		{
			bUpper = (upper==iUPPER);
			//#0028210
			DrawRebarSupport_VertSection_UpperSlab(&Dom, bUpper, dan, TRUE/*dan==0&&bMarkRebar*/, dan==0&&bDimRebar, dan, FALSE, CDRect(0, 0, 0, 0), 0, dan==0);

			if(pBri->IsBoxType())
				DrawRebarSupport_VertSection_LowerSlab(&Dom, bUpper, dan, TRUE/*dan==0&&bMarkRebar*/, dan==0&&bDimRebar, dan, FALSE, CDRect(0, 0, 0, 0), 0, dan==0);

			// 리브
			DrawRebarSupport_VertSection_ArchRib(&Dom, bUpper, dan, bMarkRebar, FALSE, dan, FALSE, CDRect(0, 0, 0, 0), nVertDir);
		}
	}
	DrawRebarShear_VertSection_UpperSlab(&Dom, bMarkRebar, FALSE, FALSE);

	if(pBri->IsBoxType())
		DrawRebarShear_VertSection_LowerSlab(&Dom, bMarkRebar, FALSE, FALSE);

	// 리브
	DrawRebarShear_VertSection_ArchRib(&Dom, bMarkRebar, FALSE, FALSE, CDRect(0, 0, 0, 0), nVertDir);
	DrawRebarStirrup_VertSection_ConnectSection(&Dom, FALSE, bMarkRebar, FALSE);

	for(stt = 0; stt < 2; stt++)
	{
		bStt	= (stt==iSTT);
		DrawRebarSupport_VertSection_UpperSlabDanBu(&Dom, bStt, bMarkRebar, bDimRebar, FALSE);
		if(pBri->IsBoxType())
			DBStd.m_pDrawVertSection->DrawRebarSupport_VertSection_LowerSlabDanBu(&Dom, bStt, bMarkRebar, bDimRebar, FALSE);
	}

	// 외측벽체
	for(stt = 0; stt < 2; stt++)
	{
		bStt	= (stt==iSTT);
		for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			for(long inner = 0; inner < 2; inner++)
			{
				BOOL bInner	= inner == iINNER;
				DrawRebarSupport_VertSection_OutWall(&Dom, bStt, bInner, dan, bMarkRebar, bDimRebar, FALSE, rect, nVertDir);
			}
		}

		long nDimDan	= pBri->GetDanMainRebarOutWall(0, bStt, FALSE);
		long nDimDanSlab	= pBri->GetDanMainRebarUpperSlab(0, TRUE, bStt ? 0 : -1);
		nDimDan	= max(nDimDan, nDimDanSlab);
		DrawRebarShear_VertSection_OutWall(&Dom, bStt, nDimDan, bMarkRebar, bDimRebar, TRUE, FALSE, nVertDir);

		DrawRebarSupport_VertSection_OutWallHunch(&Dom, bStt, bMarkRebar, FALSE, rect, nVertDir);
	}
	
	// 브라켓 & 부상방지저판
	for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		DrawRebarSupport_VertSection_Bracket(&Dom, bStt, bDimRebar, bMarkRebar, FALSE);
		DrawRebarShear_VertSection_Bracket(&Dom, bStt, bDimRebar, bMarkRebar, FALSE);	
		DrawBFootingRebar(&Dom, bStt, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE);
	}

	// 내측벽체
	long nCountWall = pBri->GetCountInWall();
	for(wall = 0; wall < nCountWall; wall++)
	{
		CWallApp *pWall	= pBri->GetInWall(wall);
		if(pWall)
		{
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				for(left = 0; left < 2; left++)
				{
					bLeft	= (left==iLEFT);
					DBStd.m_pDrawWall->DrawRebarSupport_Front(&Dom, pWall, bLeft, dan, bMarkRebar&&dan==0, bDimRebar&&dan==0, FALSE, bVertDir);
				}
			}

			DBStd.m_pDrawWall->DrawRebarShear_Front(&Dom, pWall, 0, bMarkRebar, FALSE, FALSE, FALSE, bVertDir);
		}
	}

	// 헌치부 배력철근
	long nCountHunchRebar	= pBri->m_pArrRCMainUpperSlabHunch[0].GetSize();
	for(i = 0; i < nCountHunchRebar; i++)
	{
		DrawRebarSupport_VertSection_UpperSlabHunch(&Dom, i, bMarkRebar, bDimRebar, FALSE);
	}

	nCountHunchRebar	= pBri->m_pArrRCMainLowerSlabHunch[0].GetSize();
	for(i = 0; i < nCountHunchRebar; i++)
	{
		DrawRebarSupport_VertSection_LowerSlabHunch(&Dom, i, bMarkRebar, bDimRebar, FALSE);
	}

	// 기초
	long nCountFooting	= pBri->GetCountFooting();
	for(i = 0; i < nCountFooting; i++)
	{
		CFootingApp *pFooting	= pBri->GetFooting(i);
		if(pFooting)
		{
			for(upper = 0; upper < 2; upper++)
			{
				for(dan = 0; dan < MAX_REBAR_DAN; dan++)
				{
					DBStd.m_pDrawFooting->DrawRebarSupport_Front(&Dom, pFooting, upper, dan, bDimRebar&&dan==0, bMarkRebar&&dan==0, bDimRebar&&dan==0&&!bUpper, FALSE, bVertDir?-1:1);	
				}
			}
			DBStd.m_pDrawFooting->DrawRebarShear_Front(&Dom, pFooting, TRUE, bMarkRebar, FALSE, TRUE, 0, FALSE, bVertDir?-1:1);

			// 측면철근
			// 경사형일 때만 치수 기입
			BOOL bDimSideRebar	= pFooting->m_nType != FOOTING_TYPE_BASIC && pFooting->m_nType != FOOTING_TYPE_BASIC_BOTH;
			BOOL bLeftSlope	= pFooting->m_nType != FOOTING_TYPE_RIGHTSLOPE && pFooting->m_nType != FOOTING_TYPE_BASIC_RIGHTSLOPE;
			DBStd.m_pDrawFooting->DrawRebarSideOut_Front(&Dom, pFooting, TRUE, TRUE, bDimSideRebar, TRUE, FALSE, bLeftSlope ? 2 : 3);
			DBStd.m_pDrawFooting->DrawRebarSideOut_Front(&Dom, pFooting, FALSE, TRUE, bDimSideRebar, TRUE, FALSE, bLeftSlope ? 3 : 2);
		}
	}


	// 거더 가상 철근 표현
	for(long nJ = 0; nJ < pBri->GetCountJijum(); nJ++)
	{
		DrawRebarAll_SlabGirder(&Dom, nJ, FALSE, FALSE, 0, FALSE, TRUE,TRUE,TRUE,TRUE,FALSE,-1,TRUE);
	}

	// 예각부 보강철근
	if(pBri->IsBoxType() == FALSE)
	{
		for(stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			long left = 0; for(left = 0; left < 2; left++)
			{
				BOOL bLeft	= left == iLEFT;
				DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_OutWallAcute(&Dom, bStt, bLeft, FALSE, FALSE, TRUE);
				//DBStd.m_pDrawFooting->DrawRebarMain_Front_Obtuse(&Dom, pFooting, TRUE, TRUE, FALSE, TRUE);

				DBStd.m_pDrawVertSection->DrawRebarSupport_VertSection_OutWallAcuteAngle(&Dom, bStt, bLeft, FALSE, FALSE, TRUE, 0);
				//DBStd.m_pDrawFooting->DrawRebarSupport_FrontObtuse(&Dom, pFooting, bFront, TRUE, TRUE, 0, TRUE);
			}
		}
	}	
	
	Dom.SetCalcExtRect();
	CDRect rectTot	= Dom.GetExtRect();

	*pDomP << Dom;


	// 모든 기초가 직거리로 그려진게 아니라면 ()안의 치수는 직거리임 표시 해줌
	if(!bVertDir)
	{
		BOOL bOnlyVertFooting	= TRUE;
		for(long footing = 0; footing < pBri->GetCountFooting(); footing++)
		{
			CFootingApp *pFooting	= pBri->GetFooting(footing);
			if(pFooting)
			{
				if(pFooting->m_dAngle != 90)
				{
					bOnlyVertFooting	= FALSE;
					break;
				}
			}
		}

		if(!bOnlyVertFooting)
		{
			CDRect rect	= rectTot;
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
			Dom.SetTextAlignHorz(TA_RIGHT);
			Dom.TextOut(rect.right, rect.top-Dom.GetTextHeight()*2, _T("* () 안의 치수는 직거리 치수임."));
			*pDomP << Dom;
		}
	}

	pBri->GetTvVertSection_Std(nLeft, !bVertDir, FALSE, TRUE);
	return rectTot;
}

// 종단면도 치수 모두 넣기
// BOOL bForOutBM : 수량산출서 출력용인지?
void CDrawVertSection::DimVertSectionAll(CDomyun *pDomP, BOOL bForOutBM)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	
	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	DBStd.m_bApplyDimSettingOfBaseDom	= m_pStd->m_bApplyDimSettingOfBaseDom;

	CDomyun Dom(pDomP);
	m_pStd->ApplyDimSetting(pDomP, &Dom);

	// 교량
	if(!bForOutBM)
		DimTextVerSectionEL(&Dom);
	if(bForOutBM)
	{
		if(pBri->m_nTypeSlabLength == 0)
			DimVertSectionUpper(&Dom, bForOutBM ? 0 : 1, TRUE, FALSE);
		else
			DimVertSectionUpperForBMSlabLengthTyp1(&Dom);	// 선형길이로 슬래브 길이 산정할 경우 삽도
	}
	else
	{
		DimVertSectionUpper(&Dom, bForOutBM ? 0 : 1, TRUE, FALSE);
	}

	DimVerSectionSideStt(&Dom, 0, FALSE, bForOutBM);
	DimVerSectionSideEnd(&Dom, 0, FALSE, bForOutBM);
	if(!bForOutBM)
	{
		DimVertSectionUpperSlabThick(&Dom, TRUE, 0);
		DimVerSectionHunch(&Dom, 0, TRUE, FALSE);
		DimVerSectionHunch(&Dom, 0, FALSE, FALSE);
	}
	DimVerSectionBracketHunch(&Dom, 0, TRUE, FALSE);
	DimVerSectionBracketHunch(&Dom, 0, FALSE, FALSE);

	//DimTextRibInfo(&Dom);
	
	// 기초
	for(long nJ = 0; nJ < pBri->GetCountJijum(); nJ++)
	{
		CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(nJ);
		if(!pFooting) continue;
		if(!pFooting->m_bIs) continue;

		if(!bForOutBM)
		{
			DBStd.m_pDrawFooting->DimTextFrontEL(&Dom, pFooting, nJ==pBri->GetCountJijum()-1, FALSE);
		}

		if(pBri->m_nTypeBridge != BRIDGE_TYPE_BOX && (!bForOutBM || pBri->m_nTypeSlabLength == 1))
		{
			DBStd.m_pDrawFooting->DimFrontUpper(&Dom, pFooting, 0, FALSE, nJ==pBri->GetCountJijum()-1);
			// 확대기초
			DBStd.m_pDrawExFooting->DimFrontLower(&Dom, &pFooting->m_exFooting, 0, FALSE);
			DBStd.m_pDrawExFooting->DimFrontSide(&Dom, &pFooting->m_exFooting, nJ==pBri->GetCountJijum()-1, 0, FALSE);
		}
	}

	// 내측헌치
	long nCount	= pBri->m_nCountInWallHunch;
// 	long nIdxHunch	= 0;
	long i = 0; for(i = 0; i < nCount; i++)
	{
		if(bForOutBM)
			break;

		CWallApp *pWall = pBri->GetInWall(i);
		long nIdxHunch = pBri->GetIdxHunchInWall(i, TRUE);
		if(nIdxHunch > -1) 
		{
			DimVerSectionInHunch(&Dom, 0, nIdxHunch, FALSE);
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
				DimVerSectionInHunch(&Dom, 0, nIdxHunch, FALSE, FALSE);

			if(pWall->m_nType == WALL_TYPE_V)
				DimVerSectionInHunch(&Dom, 0, nIdxHunch+1, FALSE);
		}
	}

	*pDomP << Dom;
}

// 중간 벽체 부 단면(A-A) 구조도..
// 중간벽체 구조도 들어갈때 A-A 단면으로 사용.
CDRect CDrawVertSection::DrawRebar_InWall_VertStd(CDomyun *pDomP, CWallApp *pWall, BOOL bInput, long nVertDir)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return CDRect(0, 0, 0, 0);
	if(!pWall) return CDRect(0, 0, 0, 0);
	
	BOOL bVertDir	= pBri->IsVertDir();
	if(nVertDir != 0)
	{
		bVertDir	= nVertDir == -1 ? TRUE : FALSE;
	}
	nVertDir	= bVertDir ? -1 : 1;
	long nIdxWall		= pBri->GetIdxInWall(pWall);
	long nIdxLeftHunch	= pBri->GetIdxHunchInWall(nIdxWall, TRUE);
	long nIdxRightHunch	= pBri->GetIdxHunchInWall(nIdxWall, FALSE);
	long nJ	= nIdxWall+1;


	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	CDomyun Dom(pDomP);
	
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		if(pWall->m_bTopIsColumn)
		{
			DrawDetail_Column_VertSection(&Dom, nIdxWall);
			long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				BOOL bDim	= dan < 2; 
				BOOL bIsTotalDim	= pWall->m_Column.GetCountDan_RebarMain()-1 == dan;
				DBStd.m_pDrawColumn->DrawRebarMain_Front(&Dom, &pWall->m_Column, FALSE, dan, FALSE, TRUE, FALSE);
				DBStd.m_pDrawColumn->DrawRebarRound_Front(&Dom, &pWall->m_Column, nJ, FALSE, dan, bDim, TRUE, FALSE, bIsTotalDim);
			}

			*pDomP << Dom;
			Dom.SetCalcExtRect();
			return Dom.GetExtRect();
		}
		else
		{
			DrawDetail_Column_VertSection(&Dom, nIdxWall);
		}
	}


	// 단면도 A-A가 그려질 영역 계산.
	CDRect rectFooting	= pWall->m_footing.m_tvArrFront.GetRect();
	CDRect rect(0, 0, 0, 0);
	rect	= pBri->m_tvArrVertSection.GetRect();

	if(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP || pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2)
	{
		rect.left	= (pBri->GetStationOnJijum(nJ-1, 0, bVertDir) + pBri->GetStationOnJijum(nJ, 0, bVertDir)) / 2;
		rect.right	= (pBri->GetStationOnJijum(nJ, 0, bVertDir) + pBri->GetStationOnJijum(nJ+1, 0, bVertDir)) / 2;
	}
	else if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		rect.left	= pBri->GetStationOnJijum(nJ, 0, bVertDir) - 3000;
		rect.right	= pBri->GetStationOnJijum(nJ, 0, bVertDir) + 3000;
	}
	else
	{
		rect.left	= rectFooting.left;
		rect.right	= rectFooting.right;
	}
	rect.top += 1000;
	
	// 상부슬래브부 의 단면도와 구조도 그림.
	CTwinVectorArray tvArrTmp;
	DrawVertSection(&Dom, FALSE, &tvArrTmp, rect);
	Dom.SetCalcExtRect();
	CDRect rectTot	= Dom.GetExtRect();

	long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		for(long upper = 0; upper < 2; upper++)
		{
			BOOL bUpper	= upper == iUPPER ? TRUE : FALSE;
			for(long cycle = 0; cycle < pBri->GetCountCycleMainRebar(); cycle++)
			{
				DrawRebarMain_VertSection_UpperSlab(&Dom, cycle, dan, TRUE, FALSE, FALSE, 0, rect, bInput, nVertDir);
				DrawRebarMain_VertSection_UpperSlabHunch(&Dom, cycle, FALSE, 0, rect, bInput);
				DrawRebarMain_VertSection_LowerSlab(&Dom, cycle, dan, TRUE, FALSE, FALSE, 0, rect, bInput, nVertDir);
				DrawRebarMain_VertSection_LowerSlabHunch(&Dom, cycle, FALSE, 0, rect, bInput);
				if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
					DrawRebarMain_VertSection_ArchRib(&Dom, cycle, dan, TRUE, FALSE, FALSE, 0, rect, bInput, nVertDir);
			}
			DrawRebarSupport_VertSection_UpperSlab(&Dom, bUpper, dan, TRUE, FALSE, 0, FALSE, rect, nVertDir, dan==0);
			DrawRebarSupport_VertSection_LowerSlab(&Dom, bUpper, dan, TRUE, FALSE, 0, FALSE, rect, nVertDir, dan==0);
			DrawRebarSupport_VertSection_UpperSlabHunch(&Dom, nIdxLeftHunch, FALSE, FALSE, FALSE, rect);
			DrawRebarSupport_VertSection_LowerSlabHunch(&Dom, nIdxLeftHunch, FALSE, FALSE, FALSE, rect);
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
				DrawRebarSupport_VertSection_ArchRib(&Dom, bUpper, dan, FALSE, FALSE, dan, bInput, rect, nVertDir);

			if(nIdxLeftHunch != nIdxRightHunch)
			{
				DrawRebarSupport_VertSection_UpperSlabHunch(&Dom, nIdxRightHunch, FALSE, FALSE, FALSE, rect);
				DrawRebarSupport_VertSection_LowerSlabHunch(&Dom, nIdxRightHunch, FALSE, FALSE, FALSE, rect);
			}
		}
	}
	DrawRebarShear_VertSection_UpperSlab(&Dom, FALSE, FALSE, FALSE, rect, nVertDir);
	DrawRebarShear_VertSection_LowerSlab(&Dom, FALSE, FALSE, FALSE, rect, nVertDir);
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		DrawRebarShear_VertSection_ArchRib(&Dom, FALSE, FALSE, bInput, rect, nVertDir);
	
	// 내측벽체 단면도
	DBStd.m_pDrawWall->DrawRebar_FrontSectionStd(&Dom, pWall, bInput, TRUE, FALSE, bVertDir);
	DrawRebarAll_SlabGirder(&Dom, nJ, FALSE, FALSE, 0, FALSE, TRUE,TRUE,TRUE,TRUE,FALSE,-1,TRUE);
	if(pBri->IsBoxType())
		DrawRebarAll_SlabGirder(&Dom, nJ, FALSE, FALSE, 0, FALSE, TRUE,TRUE,TRUE,TRUE,FALSE,-1,TRUE, FALSE);
	// 기초 단면도
	//if(pWall->m_footing.m_bIs)
	{
		BOOL bVirtual	= pWall->m_footing.m_bVirtualFooting;
		BOOL bIs		= pWall->m_footing.m_bIs;

		if(bVirtual || bIs)
		{
			pWall->m_footing.m_bVirtualFooting	= FALSE;
			pWall->m_footing.m_bIs	= TRUE;
			CDRect rectFooting	= DBStd.m_pDrawFooting->DrawRebar_FrontSectionStd(&Dom, &pWall->m_footing, FALSE, !bVirtual, !bVirtual, "", bInput, nVertDir);
			rectTot	= m_pStd->MergeRectAndRect(rectTot, rectFooting);

			pWall->m_footing.m_bVirtualFooting	= bVirtual;
			pWall->m_footing.m_bIs	= bIs;
		}
	}

	*pDomP << Dom;

	return rectTot;
}


CDRect CDrawVertSection::DrawRebar_OutWall_VertStd(CDomyun *pDomP, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, long nVertDir, BOOL bDrawOnlyMainRebar)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return CDRect(0, 0, 0, 0);
	if(!pBri->IsOutWall(bStt)) return CDRect(0, 0, 0, 0);
	
	BOOL bVertDir	= pBri->IsVertDir();
	if(nVertDir != 0)
	{
		bVertDir	= nVertDir == -1 ? TRUE : FALSE;
	}
	nVertDir	= bVertDir ? -1 : 1;

	
	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	CDomyun Dom(pDomP);

	CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;
	// 단면도 A-A가 그려질 영역 계산.
	CDRect rect(0, 0, 0, 0);
	rect		= pBri->m_tvArrVertSection.GetRect();
	if(pBri->IsBoxType())
	{
		double dWidth = 3000 + (pBri->m_bExistProtectionRiseFooting? pBri->m_dPRF_W : 0);
		double dPosBase = bStt? rect.left : rect.right;
		rect.left	= bStt? dPosBase - 100 : dPosBase - dWidth;
		rect.right	= bStt? dPosBase + dWidth : dPosBase + 100;
	}
	else
	{
		CDRect rectFooting	= pFooting->m_tvArrFront.GetRect();
		rect.left	= rectFooting.left;
		rect.right	= rectFooting.right;
		rect.bottom	-= pFooting->GetHeight()*2;
	}
	
	// 상부슬래브부 의 단면도와 구조도 그림.
	CTwinVectorArray tvArrTmp;
	DrawVertSection(&Dom, FALSE, &tvArrTmp, rect);

	Dom.SetCalcExtRect();
	CDRect rectTot	= Dom.GetExtRect();

	long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		for(long upper = 0; upper < 2; upper++)
		{
			BOOL bUpper	= upper == iUPPER ? TRUE : FALSE;

			DrawRebarMain_VertSection_UpperSlab(&Dom, 0, dan, TRUE, FALSE, FALSE, 0, rect, bInput, nVertDir);
			DrawRebarMain_VertSection_UpperSlabHunch(&Dom, 0, FALSE, 0, rect, bInput);
			DrawRebarMain_VertSection_LowerSlab(&Dom, 0, dan, TRUE, FALSE, FALSE, 0, rect, bInput, nVertDir);
			DrawRebarMain_VertSection_LowerSlabHunch(&Dom, 0, FALSE, 0, rect, bInput);
			

			if(!bDrawOnlyMainRebar)
			{
				DrawRebarSupport_VertSection_UpperSlab(&Dom, bUpper, dan, bMarkRebar, FALSE, 0, FALSE, rect, nVertDir, dan==0);
				DrawRebarSupport_VertSection_UpperSlabHunch(&Dom, bStt ? 0 : -1, FALSE, FALSE, bInput, rect);
				DrawRebarSupport_VertSection_LowerSlab(&Dom, bUpper, dan, bMarkRebar, FALSE, 0, FALSE, rect, nVertDir, dan==0);
				DrawRebarSupport_VertSection_LowerSlabHunch(&Dom, bStt ? 0 : -1, FALSE, FALSE, bInput, rect);
				if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
					DrawRebarSupport_VertSection_ArchRib(&Dom, bUpper, dan, FALSE, FALSE, 0, bInput, rect, nVertDir);
			}
		}
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
			DrawRebarMain_VertSection_ArchRib(&Dom, 0, dan, TRUE, FALSE, FALSE, 0, rect, bInput, nVertDir);
	}

	if(!bDrawOnlyMainRebar)
	{
		DrawRebarShear_VertSection_UpperSlab(&Dom, FALSE, FALSE, FALSE, rect, nVertDir);
		DrawRebarShear_VertSection_LowerSlab(&Dom, FALSE, FALSE, FALSE, rect, nVertDir);
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
			DrawRebarShear_VertSection_ArchRib(&Dom, FALSE, FALSE, bInput, rect, nVertDir);
	}

	// 외측벽체 ..
	for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		DrawRebarMain_VertSection_OutWall(&Dom, 0, dan, TRUE, FALSE, FALSE, 0, rect, bInput);

		for(long inner = 0; inner < 2; inner++)
		{
			BOOL bInner	= inner == iINNER;
				
			if(!bDrawOnlyMainRebar)
			{
				DrawRebarSupport_VertSection_OutWall(&Dom, bStt, bInner, dan, bMarkRebar, bDimRebar, FALSE, rect, nVertDir);
			}

			// 경사교2타입인 경우.. 내측벽체도 같이 그려줌
			if(0)//pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2)
			{
				CWallApp *pWall	= pBri->GetInWall(bStt ? 0 : pBri->m_nQtyJigan-2);
				if(pWall)
				{
					m_pStd->m_pDrawWall->DrawRebar_FrontSectionStd(&Dom, pWall, bInput, FALSE);
				}
			}
		}
		DrawRebarMain_VertSection_OutWall_Hunch(&Dom, 0, FALSE, 0, rect, FALSE, FALSE, nVertDir);
		if(!bDrawOnlyMainRebar)
		{
			DrawRebarSupport_VertSection_OutWallHunch(&Dom, bStt, bMarkRebar, bInput, rect, nVertDir);
		}
	}
	
	long nDimDan	= pBri->GetDanMainRebarOutWall(0, bStt, FALSE);
	long nDimDanSlab	= pBri->GetDanMainRebarUpperSlab(0, TRUE, bStt ? 0 : -1);
	nDimDan	= max(nDimDan, nDimDanSlab);
	if(!bDrawOnlyMainRebar)
	{
		DrawRebarShear_VertSection_OutWall(&Dom, bStt, nDimDan, bMarkRebar, bDimRebar, TRUE, FALSE, nVertDir);
	}

	// 브라켓
	DrawRebarMain_VertSection_Bracket(&Dom, bStt, FALSE, FALSE, FALSE, 0, rect);
	if(!bDrawOnlyMainRebar)
	{
		DrawRebarSupport_VertSection_Bracket(&Dom, bStt, FALSE, bMarkRebar, FALSE, rect);
		DrawRebarShear_VertSection_Bracket(&Dom, bStt, FALSE, bMarkRebar, FALSE, rect);
	}
	
	// 단 절점부
	DrawRebarMain_VertSection_Danbu(&Dom, FALSE, FALSE, FALSE, 0, bStt ? -1 : 1);
	if(!bDrawOnlyMainRebar)
	{
		DrawRebarSupport_VertSection_UpperSlabDanBu(&Dom, bStt, bMarkRebar, FALSE, FALSE);
		if(pBri->IsBoxType())
			DBStd.m_pDrawVertSection->DrawRebarSupport_VertSection_LowerSlabDanBu(&Dom, bStt, bMarkRebar, FALSE, FALSE);
	}

	// 헌치부 전단철근
	if(!bDrawOnlyMainRebar)
	{
		DrawRebarShear_VertSection_UpperSlabDanbu(&Dom, 0, bStt, FALSE, FALSE, FALSE);
		DrawRebarShear_VertSection_LowerSlabDanbu(&Dom, 0, bStt, FALSE, FALSE, FALSE);
	}
	

	// 예각부 보강철근
	if(pBri->IsBoxType() == FALSE)
	{
		long left = 0; for(left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT;
			DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_OutWallAcute(&Dom, bStt, bLeft, FALSE, FALSE, TRUE);
			//DBStd.m_pDrawFooting->DrawRebarMain_Front_Obtuse(&Dom, pFooting, TRUE, TRUE, FALSE, TRUE);

			DBStd.m_pDrawVertSection->DrawRebarSupport_VertSection_OutWallAcuteAngle(&Dom, bStt, bLeft, FALSE, FALSE, TRUE, 0);
			//DBStd.m_pDrawFooting->DrawRebarSupport_FrontObtuse(&Dom, pFooting, bFront, TRUE, TRUE, 0, TRUE);
		}
	}	

	// 기초 단면도
	if(pFooting->m_bIs)
	{
		CDRect rectFooting	= DBStd.m_pDrawFooting->DrawRebar_FrontSectionStd(&Dom, pFooting, FALSE, bDimRebar, bMarkRebar, "", bInput, nVertDir, bDrawOnlyMainRebar);
		rectTot	= m_pStd->MergeRectAndRect(rectTot, rectFooting);
	}

	*pDomP << Dom;

	return rectTot;
}

void CDrawVertSection::DrawRebarMain_VertSection_Danbu(CDomyun *pDomP, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, long nCycle, long nDrawLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	CConcSymbol sym(m_pStd->m_pARoadOptionStd);
	BOOL bDrawLengthInfo	= m_pStd->IsDrawingLengthInfo();

	long nSlabCount = 1;
	if(pBri->IsBoxType()) nSlabCount = 2;

	for(long nSlab = iUPPER; nSlab < nSlabCount; nSlab++)
	{
		long stt = 0; for(stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			if(!pBri->IsOutWall(bStt)) continue;
			if(nDrawLeft == -1 && !bStt) continue;
			if(nDrawLeft == 1 && bStt) continue;

			long dan = 0; for(dan = 0; dan < 3; dan++)
			{
				if(pBri->IsDanbuRebar(bStt, dan, nCycle, nSlab == iUPPER))
				{
					CRebarInfoCycle * pRC = NULL;
					if(nSlab == iUPPER)
						pRC	= (nCycle == 0) ? &pBri->m_rebarInfoCycleDanbu_UpperCycle1[stt][dan] : &pBri->m_rebarInfoCycleDanbu_UpperCycle2[stt][dan];
					else
						pRC	= (nCycle == 0) ? &pBri->m_rebarInfoCycleDanbu_LowerCycle1[stt][dan] : &pBri->m_rebarInfoCycleDanbu_LowerCycle2[stt][dan];

					m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
					m_pStd->DrawRebarInfoCycle(&Dom, pRC);

					if(bDimRebar)
					{}

					CString sMark	= bInput ? pRC->m_szMark : pBri->GetMarkRebarMain(0, FALSE, 0, FALSE, FALSE, pRC->m_sDescription, dan);
					if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
					{
						*pDomP << Dom;

						m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
						CRebar *pRb = pBri->GetRebarMainByDescription(pRC->m_sDescription);

						CDPoint xyMark	= (pRC->m_xyMid2+pRC->m_xyMid1)/2;
						CString sDia	= m_pStd->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pBri->GetValueFy(nSlab == iUPPER?ePartUpperSlab : ePartLowerSlab));
						double dHeight	= fabs(pRC->m_xyMid2.x-pRC->m_xyMid1.x)/2+pBri->m_dCoverWallOut[0];
						dHeight += bStt ? MARK_BASEHEIGHT_UPPER : MARK_BASEHEIGHT_LOWER;


						if(dan == 0)
						{
							BOOL bDir = FALSE;
							if(nSlab == iLOWER) bDir = TRUE; 
							
// 							if(bStt)
// 							{
// 								if(bDrawLengthInfo)
// 									sym.RebarMarkLineUpperEx(&Dom, xyMark, sMark, sDia, pRC->GetLength(), 0, bDir, dHeight);
// 								else
// 									sym.RebarMarkLineUpper(&Dom, xyMark, sMark, sDia, bDir, dHeight);
// 							}
// 							else
// 							{
// 								if(bDrawLengthInfo)
// 									sym.RebarMarkLineLowerEx(&Dom, xyMark, sMark, sDia, pRC->GetLength(), 0, bDir, dHeight);
// 								else
// 									sym.RebarMarkLineLower(&Dom, xyMark, sMark, sDia, bDir, dHeight);
// 							}

							SRebarMarkInputData mark_input_data;
							SRebarMarkviewData mark_view_data;
							mark_input_data.mark = sMark;
							mark_input_data.dia = sDia;
							mark_input_data.position = xyMark;
							mark_input_data.ctc = pBri->GetCtcRebarMain(FALSE);
							mark_input_data.type_rebar = pRb? pRb->m_nTypeRebar : -1;
							mark_input_data.left_dir =  bDir;
							mark_input_data.height = dHeight;
							mark_input_data.rebar_length = bDrawLengthInfo? pRC->GetLength() : 0;

							mark_view_data.count_mark = TRUE;
							mark_view_data.mark_pos = bStt? eMarkUpper : eMarkLower;

							sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

							Dom.Rotate(xyMark, CDPoint(0, 1));
						}
						else if(dan == 1)
						{
							BOOL bDir = TRUE;
							if(nSlab == iLOWER) bDir = FALSE; 
// 							if(bStt)
// 							{
// 								if(bDrawLengthInfo)
// 									sym.RebarMarkLineUpperEx(&Dom, xyMark, sMark, sDia, pRC->GetLength(), 0, bDir, dHeight);
// 								else
// 									sym.RebarMarkLineUpper(&Dom, xyMark, sMark, sDia, bDir, dHeight);
// 							}
// 							else
// 							{
// 								if(bDrawLengthInfo)
// 									sym.RebarMarkLineLowerEx(&Dom, xyMark, sMark, sDia, pRC->GetLength(), 0, bDir, dHeight);
// 								else
// 									sym.RebarMarkLineLower(&Dom, xyMark, sMark, sDia, bDir, dHeight);
// 							}

							SRebarMarkInputData mark_input_data;
							SRebarMarkviewData mark_view_data;
							mark_input_data.mark = sMark;
							mark_input_data.dia = sDia;
							mark_input_data.position = xyMark;
							mark_input_data.ctc = pBri->GetCtcRebarMain(FALSE);
							mark_input_data.type_rebar = pRb? pRb->m_nTypeRebar : -1;
							mark_input_data.left_dir =  bDir;
							mark_input_data.height = dHeight;
							mark_input_data.rebar_length = bDrawLengthInfo? pRC->GetLength() : 0;

							mark_view_data.count_mark = TRUE;
							mark_view_data.mark_pos = bStt? eMarkUpper : eMarkLower;

							sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

							Dom.Rotate(xyMark, CDPoint(0, 1));
						}
						else if(dan == 2)
						{
// 							if(bDrawLengthInfo)
// 								sym.RebarMarkLineSideEx(&Dom, xyMark, sMark, sDia, pRC->GetLength(), 0, bStt, dHeight);
// 							else
// 								sym.RebarMarkLineSide(&Dom, xyMark, sMark, sDia, bStt, dHeight);

							SRebarMarkInputData mark_input_data;
							SRebarMarkviewData mark_view_data;
							mark_input_data.mark = sMark;
							mark_input_data.dia = sDia;
							mark_input_data.position = xyMark;
							mark_input_data.ctc = pBri->GetCtcRebarMain(FALSE);
							mark_input_data.type_rebar = pRb? pRb->m_nTypeRebar : -1;
							mark_input_data.left_dir =  bStt;
							mark_input_data.height = dHeight;
							mark_input_data.rebar_length = bDrawLengthInfo? pRC->GetLength() : 0;

							mark_view_data.count_mark = TRUE;
							mark_view_data.mark_pos = eMarkSide;

							sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
						}
					}
				}
			}
		}
	}

	*pDomP << Dom;
}

// 상부슬래브 주철근 1단에 대한 가상 라인을 그린다.
// 이는 기둥 상단 거더 단면철근을 배근할때 참고하기 위해 그려주는 것임.
// CONC라인 중심선으로 그려줌.
// 가상라인을 그릴때는 주철근을 모두 머지해서 그려야 됨.
// 그렇지 않은경우 중심선이 끊어져서 보이므로 지저분해 질 수 있음.
void CDrawVertSection::DrawVirtualRebarMain_VertSection_UpperSlab(CDomyun *pDomP, CDRect rectLimit)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARR, LT_DASHDOT_LINE);

	// 상면 
	CRebarInfoCycle *pRCUpperStt	= pBri->GetRebarInfoCycleUpperSlab(0, 0, TRUE, 0),
					*pRCUpperEnd	= pBri->GetRebarInfoCycleUpperSlab(0, 0, TRUE, -1);
	CRebarInfoCycle rcUpperTot;

	if(pRCUpperStt && pRCUpperEnd)
	{
		rcUpperTot.m_xyStt	= pRCUpperStt->m_xyMid2;
		rcUpperTot.m_xyMid1	= pRCUpperEnd->m_xyMid1;
		m_pStd->DrawRebarInfoCycle(&Dom, &rcUpperTot, 0, rectLimit);
	}

	// 하면 
	CRebarInfoCycle *pRCLowerStt	= pBri->GetRebarInfoCycleUpperSlab(0, 0, FALSE, 0),
					*pRCLowerEnd	= pBri->GetRebarInfoCycleUpperSlab(0, 0, FALSE, -1);
	CRebarInfoCycle rcLowerTot;
	if(pRCLowerStt && pRCLowerEnd)
	{
		rcLowerTot.m_xyStt	= pRCLowerStt->m_xyMid2;
		rcLowerTot.m_xyMid1	= pRCLowerEnd->m_xyMid1;
		m_pStd->DrawRebarInfoCycle(&Dom, &rcLowerTot, 0, rectLimit);
	}

	*pDomP << Dom;
}

// 하부슬래브 주철근 1단에 대한 가상 라인을 그린다.
// 이는 기둥 하단 거더 단면철근을 배근할때 참고하기 위해 그려주는 것임.
// CONC라인 중심선으로 그려줌.
// 가상라인을 그릴때는 주철근을 모두 머지해서 그려야 됨.
// 그렇지 않은경우 중심선이 끊어져서 보이므로 지저분해 질 수 있음.
void CDrawVertSection::DrawVirtualRebarMain_VertSection_LowerSlab(CDomyun *pDomP, CDRect rectLimit)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARR, LT_DASHDOT_LINE);
	// 상면 
	CRebarInfoCycle *pRCUpperStt	= pBri->GetRebarInfoCycleLowerSlab(0, 0, TRUE, 0),
		*pRCUpperEnd	= pBri->GetRebarInfoCycleLowerSlab(0, 0, TRUE, -1);
	CRebarInfoCycle rcUpperTot;

	if(pRCUpperStt && pRCUpperEnd)
	{
		rcUpperTot.m_xyStt	= pRCUpperStt->m_xyMid2;
		rcUpperTot.m_xyMid1	= pRCUpperEnd->m_xyMid1;
		m_pStd->DrawRebarInfoCycle(&Dom, &rcUpperTot, 0, rectLimit);
	}

	// 하면 
	CRebarInfoCycle *pRCLowerStt	= pBri->GetRebarInfoCycleLowerSlab(0, 0, FALSE, 0),
		*pRCLowerEnd	= pBri->GetRebarInfoCycleLowerSlab(0, 0, FALSE, -1);
	CRebarInfoCycle rcLowerTot;
	if(pRCLowerStt && pRCLowerEnd)
	{
		rcLowerTot.m_xyStt	= pRCLowerStt->m_xyMid2;
		rcLowerTot.m_xyMid1	= pRCLowerEnd->m_xyMid1;
		m_pStd->DrawRebarInfoCycle(&Dom, &rcLowerTot, 0, rectLimit);
	}

	*pDomP << Dom;
}

//BOOL bForUseBM : 수량용(뒷채움선이 포장이 빠진채 그려진다.)
//long iIndex : 지점
//pBriBox : Box형은 수량용단면을 사용해야하므로 인자로 넘겨받음.
void CDrawVertSection::DrawTogongLine(CDomyun *pDomP, CRcBridgeRebar *pBriBox, long iIndex, BOOL bEL, BOOL bJusang, BOOL bBackFill, BOOL bInput, BOOL bTyopagi, BOOL bForUseBM)
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
	
	pBri->GetTvTogongLine(tvArr, iIndex);

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CUTL);
	m_pStd->ApplyDimSetting(pDomP, &Dom);

	// 터파기 EL 
	CDRect rect = tvArr.GetRect();
	GetSwap(rect.top, rect.bottom);
	CString strEl	= _T("");
	CConcSymbol SymDom(m_pStd->m_pARoadOptionStd);

	if(bTyopagi)
	{
		double dExt	= 1000;

		if(pFooting->m_dEL_DiggingFirst > pFooting->m_dEL-pFooting->GetHeight())
			m_pStd->DrawTvArray(&Dom, tvArr, FALSE, FALSE, 0);

		// 지반선
		if(bEL)
		{
			DrawElByStation(&Dom, rect.left - dExt , rect.right + dExt);
		}

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_MIS4);
		m_pStd->ApplyDimSetting(pDomP, &Dom);

		Dom.MoveTo(rect.left-dExt, pFooting->m_dEL_DiggingFirst);
		Dom.LineTo(rect.right+dExt, pFooting->m_dEL_DiggingFirst);

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		m_pStd->ApplyDimSetting(pDomP, &Dom);

		strEl.Format("EL. %0.3f", toM(pFooting->m_dEL_DiggingFirst));
		SymDom.ArrowElevation(&Dom, bInput ? "EL1" : strEl, bInput ? strEl : "", CDPoint(rect.left+Dom.Always(3), pFooting->m_dEL_DiggingFirst),FALSE);

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
			CString sWaterTupagi =pBri->m_bApplyYongSu? _T("용수터파기") : _T("수중터파기");

			Dom.TextOut(rect.right-dExt, pFooting->m_dEL_DiggingWater+Dom.Always(1), sTupagi);
			Dom.TextOut(rect.right-dExt, pFooting->m_dEL_DiggingWater-Dom.Always(1)-Dom.GetTextHeight(), sWaterTupagi);

			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
			m_pStd->ApplyDimSetting(pDomP, &Dom);

			strEl.Format("EL. %0.3f", toM(pFooting->m_dEL_DiggingWater));
			SymDom.ArrowElevation(&Dom, bInput ? "EL2" : strEl, bInput ? strEl : "", CDPoint(rect.left+Dom.Always(3), pFooting->m_dEL_DiggingWater),FALSE,FALSE);
		}
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

	// 뒷채움선
	if(bBackFill)
	{
		//Box형일때는 수량용 Bridge가 따로 있음.
		if(pBri->IsBoxType())
		{
			if(!pBriBox) return;
			pBri = pBriBox;
		}
		pBri->m_pLineApp;

		CTwinVector tv;
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DASH);
		m_pStd->ApplyDimSetting(pDomP, &Dom);

		//BOX형인 경우 양쪽다 그려준다.
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX && !bForUseBM)
		{
			if(pBri->m_nDeductNearTupagiBM != 1)
			{
				tv	= pBri->GetTvBackFill(TRUE, bForUseBM ? 4 : 0, TRUE); //좌
				Dom.LineTo(tv.m_v1, tv.m_v2);
			}
			if(pBri->m_nDeductNearTupagiBM != 2)
			{
				tv	= pBri->GetTvBackFill(FALSE, bForUseBM ? 4 : 0, TRUE); //우
				Dom.LineTo(tv.m_v1, tv.m_v2);
			}			
		}
		else
		{
			if(iIndex == 0)
			{
				tv	= pBri->GetTvBackFill(TRUE, bForUseBM ? 4 : 0, TRUE);
				Dom.LineTo(tv.m_v1, tv.m_v2);
			}
			else if(iIndex == pBri->m_nQtyJigan)
			{
				tv	= pBri->GetTvBackFill(FALSE, bForUseBM ? 4 : 0, TRUE);
				Dom.LineTo(tv.m_v1, tv.m_v2);
			}		
		}

	}

	*pDomP << Dom;
}

// BOOL bOnlyFooting : 기초는 전부 그리고 벽체는 잘라서 조금만 그림 (상부 슬래브 안나옴)
// 경사교2, 아치교는 기초에 벽체가 붙어있으로 조금 그려줘야한다.
void CDrawVertSection::DrawPartByJijum(CDomyun *pDomP, long iIndex, BOOL bOnlyFooting)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;
	if(iIndex < 0 || iIndex > pBri->m_nQtyJigan) return;
	CFootingApp *pFooting;
	CDRect cRect, rtemp;

	CTwinVectorArray tvArr, tvArrTot, tvArrOld;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	m_pStd->ApplyDimSetting(pDomP, &Dom);

	int nCnt = pBri->GetCountJijum();
	if(iIndex >= nCnt) return;

	CTwinVector tv;
	tvArrTot.RemoveAll();
	long i = 0; for(i = 0; i < pBri->m_tvArrVertSection.GetSize(); i++)
	{
		tv	= pBri->m_tvArrVertSection.GetAt(i);
		if(!bOnlyFooting)
			tvArrTot.Add(tv);
		else	// 수평으로 자르는 부분에서 문제가 있어서 곡선은 넣지 않는다.
		{
			if(tv.m_v1.z == 0)
				tvArrTot.Add(tv);				
		}
	}
	
	CTwinVector tvWallLowerR, tvWallLowerL;
	// 벽체
	if(iIndex == 0)
	{
		pFooting = &(pBri->m_footingStt);

		if(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 || pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		{
			CWallApp *pWall;
			pWall = pBri->GetInWall(iIndex);
			tvArrOld = pWall->m_tvArrFront;

			// 31720
			CTwinVectorArray tvArrVertSection;
			tvArrVertSection.AddFromTvArray(pBri->m_tvArrVertSection);

			pWall->GetTvFront(tvArrVertSection, FALSE);
			// 중복된 면 제거
			long nSize = pWall->m_tvArrFront.GetSize();
			for(i=0; i<nSize; i++)
			{
				tv = pWall->m_tvArrFront.GetAt(i);
				if(tv.m_sInfo == _T("벽체하면우측"))
				{
					tvWallLowerR = tv;
					continue;
				}
				if(tv.m_sInfo == _T("벽체하면좌측"))
				{
					tvWallLowerL = tv;
					continue;
				}
				tvArrTot.Add(tv);
			}	
			//tvArrTot.AddFromTvArray(pWall->m_tvArrFront);
			pWall->m_tvArrFront = tvArrOld;
		}
	}
	else if(iIndex == nCnt-1)
	{
		pFooting = &(pBri->m_footingEnd);

		if(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 || pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		{
			CWallApp *pWall;
			pWall = pBri->GetInWall(pBri->GetCountInWall()-1);
			tvArrOld = pWall->m_tvArrFront;

			//31720
			CTwinVectorArray tvArrVertSection;
			tvArrVertSection.AddFromTvArray(pBri->m_tvArrVertSection); 

			pWall->GetTvFront(tvArrVertSection, FALSE);
			// 중복된 면 제거
			long nSize = pWall->m_tvArrFront.GetSize();
			for(i=0; i<nSize; i++)
			{
				tv = pWall->m_tvArrFront.GetAt(i);
				if(tv.m_sInfo == _T("벽체하면우측"))
				{
					tvWallLowerR = tv;
					continue;
				}
				if(tv.m_sInfo == _T("벽체하면좌측"))
				{
					tvWallLowerL = tv;
					continue;
				}
				tvArrTot.Add(tv);
			}
			//tvArrTot.AddFromTvArray(pWall->m_tvArrFront);
			pWall->m_tvArrFront = tvArrOld;
		}
	}
	else	//  경사교 벽체
	{
		CWallApp *pWall;
		pWall = pBri->GetInWall(iIndex-1);
		tvArrOld = pWall->m_tvArrFront;

		//31720
		CTwinVectorArray tvArrVertSection;
		tvArrVertSection.AddFromTvArray(pBri->m_tvArrVertSection); 

		pWall->GetTvFront(tvArrVertSection, FALSE);
		// 중복된 면 제거
		long nSize = pWall->m_tvArrFront.GetSize();
		for(i=0; i<nSize; i++)
		{
			tv = pWall->m_tvArrFront.GetAt(i);
			if(tv.m_sInfo == _T("벽체하면우측") || tv.m_sInfo == _T("벽체하면좌측"))
				continue;
			tvArrTot.Add(tv);
		}
		//tvArrTot.AddFromTvArray(pWall->m_tvArrFront);
		
		pWall->m_tvArrFront = tvArrOld;
		pFooting = &(pWall->m_footing);
	}
	
	// 기초
	CTwinVector tvFootingUp, tvFootingExt;
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 || pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		if(iIndex==0)
		{
			pFooting->m_tvArrFront.GetTvByInfo(_T("기초상면우측"), tvFootingUp);
			pFooting->m_tvArrFront.GetTvByInfo(_T("기초우측확장상면"), tvFootingExt);
		}
		else
		{
			pFooting->m_tvArrFront.GetTvByInfo(_T("기초상면좌측"), tvFootingUp);
			pFooting->m_tvArrFront.GetTvByInfo(_T("기초좌측확장상면"), tvFootingExt);
		}
	}

	// 기초와 벽체가 이어지는 선 만들어 준다.
	pFooting->GetTvFront(FALSE);
	tvArrTot.AddFromTvArray(pFooting->m_tvArrFront);
	
	tvArrOld = pFooting->m_exFooting.m_tvArrFront;
	pFooting->m_exFooting.GetTvFront(FALSE);
	tvArrTot.AddFromTvArray(pFooting->m_exFooting.m_tvArrFront);
	pFooting->m_exFooting.m_tvArrFront = tvArrOld;

	rtemp = tvArrTot.GetRect();
	cRect.top = rtemp.top+500;
	cRect.bottom = rtemp.bottom-500;

	// 기초 너비에 맞춰서 자름
	CDPoint vAngSkew	= pBri->GetAngleJijum(iIndex);
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP)
	{
		rtemp = pFooting->m_tvArrFront.GetRect();
		cRect.left	= rtemp.left - pBri->GetLengthJigan(iIndex)/2;
		cRect.right	= rtemp.right + pBri->GetLengthJigan(iIndex)/2;

		CRebarPlacing rb;
		rb.m_pLine	= pBri->GetLineBase();
		CTwinVectorArray tvArrArea;
		CTwinVector tvR;
		
		tvR.m_v1 = cRect.BottomLeft() + CDPoint(10, -10);
		tvR.m_v2 = cRect.BottomRight() + CDPoint(-10, -10);
		tvArrArea.Add(tvR);
		
		tvR.m_v1 = tvR.m_v2;
		tvR.m_v2 = cRect.TopRight() + CDPoint(-10, 10);
		tvArrArea.Add(tvR);

		tvR.m_v1 = tvR.m_v2;
		tvR.m_v2 = cRect.TopLeft() + CDPoint(10, 10);
		tvArrArea.Add(tvR);

		tvR.m_v1 = tvR.m_v2;
		tvR.m_v2 = cRect.BottomLeft() + CDPoint(10, -10);
		tvArrArea.Add(tvR);

		CDRect rr = tvArrArea.GetRect();
		tvArrArea.RemoveAll();
		tvArrArea.SetFromRect(rr);

		rb.GetTvArrayByCrossTvArrayArea(tvArrTot, tvArrArea, TRUE);
	}
	else
	{
		rtemp = pFooting->m_tvArrFront.GetRect();
		
		cRect.left	= rtemp.left - fabs(500/vAngSkew.y);
		cRect.right	= rtemp.right + fabs(500/vAngSkew.y);
	}

	/*
	// 터파기 너비에 따라서 상부슬래브를 넓거나 좁게 자르기 위해 
	// 터파기 높이를 좌우로 더해준다
	CTwinVectorArray tvArrTyopagi;
	pBri->GetTvTogongLine(tvArrTyopagi, iIndex, FALSE, FALSE, FALSE);
	CDRect rectTyopagi	= tvArrTyopagi.GetRect();
	cRect.left = rectTyopagi.left;//rtemp.left - fabs(dHeightTyopagi) - 500;
	cRect.right = rectTyopagi.right;//rtemp.right + fabs(dHeightTyopagi) + 500;
	*/

	if(bOnlyFooting)
		cRect.top	= pFooting->m_tvArrFront.GetRect().top + pFooting->GetHeight();
	
	m_pStd->DrawCuttingTvArray(&Dom, tvArrTot, cRect, TRUE);

	*pDomP << Dom;
}

void CDrawVertSection::DrawElByStation(CDomyun *pDomP, double dStd1, double dStd2)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;
	CLineInfoApp *pLine = m_pStd->m_pARcBridgeDataStd->GetLineApp();
	if(!pLine) return;
	
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_STLC);
	Dom.SetLineColor(6);

	long nLine = 0;
		
	CLineInfo *pL = pLine->GetLine(nLine);

	if(pL->m_VEarthLine.GetSize())
	{			
		double dEleStt, dEleEnd;
		if(pLine->GetLine(nLine)->GetElevationErathOnStation(dStd1,dEleStt) &&
		   pLine->GetLine(nLine)->GetElevationErathOnStation(dStd2,dEleEnd))
		{
			Dom.MoveTo(dStd1, dEleStt);
			long i = 0; for(i = 0; i < pL->m_VEarthLine.GetSize(); i++)	
			{
				if(pL->m_VEarthLine.GetAt(i)->dSta < dStd1) continue;
				if(pL->m_VEarthLine.GetAt(i)->dSta > dStd2) break;
				Dom.LineTo(pL->m_VEarthLine.GetAt(i)->dSta, pL->m_VEarthLine.GetAt(i)->dEle);	
			}
			Dom.LineTo(dStd2, dEleEnd);
		}
	}
	*pDomP << Dom;
}

//BOOL bForUseBM : 수량용(뒷채움선이 포장이 빠진채 그려진다.)
//pBriBox : Box형은 수량용단면을 사용해야하므로 인자로 넘겨받음.
void CDrawVertSection::DimTogongLine(CDomyun *pDomP, CRcBridgeRebar *pBriBox, long iIndex, BOOL bDimTypagi, BOOL bDimBackFill, BOOL bInput, BOOL bForUseBM)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;
	CLineInfoApp *pLine = m_pStd->m_pARcBridgeDataStd->GetLineApp();
	if(!pLine) return;
	
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDomP, &Dom);

	CTwinVectorArray pTvArr;
	CTwinVector tv, tvUpperG;
	CDPoint xyStt(0,0);
	CDoubleArray dArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("BOTTOM");

	long nDimDan = 0;

	if(bDimTypagi)
	{
		pBri->GetTvTogongLine(pTvArr,iIndex, FALSE, FALSE, FALSE, FALSE, pBri->m_nDeductNearTupagiBM);
		if(pTvArr.GetSize() == 0) return;

		pTvArr.GetTvByInfo("왼쪽 이격", tv);
		dArrDim.Add(tv.GetHorLength());
		sArrText.Add("Pf");
		sArrUnderText.Add(COMMA(tv.GetHorLength()));
		xyStt = tv.m_v2;

		pTvArr.GetTvByInfo("기초폭", tv);
		dArrDim.Add(tv.GetHorLength());
		sArrText.Add("기초폭");
		sArrUnderText.Add(COMMA(tv.GetHorLength()));
		
		pTvArr.GetTvByInfo("오른쪽 이격", tv);
		dArrDim.Add(tv.GetHorLength());
		sArrText.Add("Pf");
		sArrUnderText.Add(COMMA(tv.GetHorLength()));

		if(!bInput)
		{
			sArrText.Copy(sArrUnderText);
			sArrUnderText.RemoveAll();
		}

		CHgBaseDrawStd baseDraw;
		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, 0, FALSE, FALSE, TRUE);
		*pDomP << Dom;

		long nSize = pTvArr.GetSize();
		double dL = pTvArr.GetAt(nSize-1).GetXyRight().x - pTvArr.GetAt(0).GetXyLeft().x;
		tv = pTvArr.GetAt(0);
		nDimDan = 3;
		Dom.DimMoveTo(tv.GetXyLeft().x, tv.GetXyLeft().y, nDimDan);
		Dom.DimLineTo(dL, nDimDan, COMMA(dL));		
		*pDomP << Dom;

		CString strSlope(_T(""));
		tv = pTvArr.GetAt(nSize-1);
		CFootingApp *pFooting;

		if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
			pFooting =  &(pBri->m_footingBox);
		else if(iIndex == 0)
			pFooting =  &(pBri->m_footingStt);
		else if(iIndex == pBri->m_nQtyJigan)
			pFooting = &(pBri->m_footingEnd);
		else
			pFooting = &(pBri->GetInWall(iIndex-1)->m_footing);		
		if(tv.GetXyTop().y > pFooting->m_dEL)
		{
			double dSlope = tv.GetVerLength() / tv.GetHorLength();
			strSlope.Format(_T("1:%.1f"),dSlope);
			double dAng = (tv.m_v2-tv.m_v1).GetAngleRadian();
			double dx = (tv.m_v1.x+tv.m_v2.x)/2;
			double dy = (tv.m_v1.y+tv.m_v2.y)/2;
			Dom.TextOut(dx,dy,strSlope);
			Dom.Rotate(dAng,dx,dy);
			*pDomP << Dom;
		}
	}

	if(bDimBackFill)
	{
		if(pBri->IsBoxType())
		{
			if(!pBriBox) return;
			pBri = pBriBox;
			pLine = pBriBox->m_pLineApp;
		}
		dArrDim.RemoveAll();
		sArrText.RemoveAll();
		sArrUnderText.RemoveAll();
		
		// 지반선
		CTwinVectorArray tvArrTypagi;
		pBri->GetTvTogongLine(tvArrTypagi, iIndex, FALSE, TRUE, TRUE, FALSE, pBri->m_nDeductNearTupagiBM);
		CTwinVector tvEL;
		tvArrTypagi.GetTvByInfo("상면", tvEL);
		
		double dDist = 0;
		// 뒷채움선
		CHgBaseDrawStd baseDraw;
		if(iIndex == 0)
		{
			CFootingApp *pFooting = NULL;
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
				pFooting =  &(pBri->m_footingBox);
			else
				pFooting =  &(pBri->m_footingStt);
			if(!pFooting) return;
			
			double dx, dy, dh, dv, dEl;
			CTwinVector tv	= pBri->GetTvBackFill(TRUE, bForUseBM ? 4 : 0, TRUE);
 			//dx = pBri->m_dStationBridgeStt - pBri->m_footingStt.m_dML - pBri->m_footingStt.m_dDistOffsetBackFill;
			dx	= tv.GetXyBottom().x;
			dy = pFooting->m_dEL;
			dEl = pLine->GetElevationPlanEl(pBri->m_dStationBridgeStt);
			dh = dEl-dy;
			dv = dh * pFooting->m_dSlopeBackFill;
			
			tvUpperG.m_v1.x = pBri->m_dStationBridgeStt + 2000;
			tvUpperG.m_v1.y = dEl;
			
			sDir = _T("TOP");
			dArrDim.Add(pFooting->m_dDistOffsetBackFill);
			sArrText.Add("Db");
			sArrUnderText.Add(COMMA(pFooting->m_dDistOffsetBackFill));
			xyStt = CDPoint(dx, dy);
			
			if(!bInput)
			{
				sArrText.Copy(sArrUnderText);
				sArrUnderText.RemoveAll();
			}
			nDimDan=0;
			baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, 0, FALSE, FALSE, TRUE);
			*pDomP << Dom;
			
			dArrDim.RemoveAll();
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();
			dx = tv.GetXyTop().x;
			dy = tvEL.GetXyTop().y;
			CDPoint xyResult;
			BOOL bMatch = GetXyMatchSegAndSeg(tv.m_v1,tv.m_v2,tvEL.m_v1,tvEL.m_v2,xyResult);
			if(bMatch)
			{
				dDist = dx - xyResult.x;
				dArrDim.Add(dDist);
				xyStt = CDPoint(dx-dDist, dy);
				baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, 0, FALSE, FALSE, TRUE);
				*pDomP << Dom;
			
				sDir = _T("RIGHT");
				dArrDim.RemoveAll();
				sArrText.RemoveAll();
				sArrUnderText.RemoveAll();
				dx = pBri->m_dStationBridgeStt - pFooting->m_dML;
				dy = pFooting->m_dEL;
				dDist = tvEL.GetXyTop().y - dy;
				dArrDim.Add(dDist);
				xyStt = CDPoint(pBri->m_dStationBridgeStt, dy);
				baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, 0, FALSE, FALSE, TRUE);
				*pDomP << Dom;
			}
			
			CString str("1:n(V:H)");
			if(!bInput)
				str.Format("1:%.1f", dv/dh);
			
			//BOX인경우 왼쪽에서 DistOffset만큼만 뺴주면됨.
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
				dx = pBri->m_dStationBridgeStt - pFooting->m_dDistOffsetBackFill;
			
			Dom.SetTextAlignHorz(TA_CENTER);
			Dom.TextOut(dx-dv/2-Dom.Always(1), dy+dh/2-Dom.Always(1), str);
			Dom.Rotate(CDPoint(dx-dv/2-Dom.Always(1), dy+dh/2-Dom.Always(1)), CDPoint(1,-1/pFooting->m_dSlopeBackFill));
			*pDomP << Dom;
						
			// H1, H2
			dArrDim.RemoveAll();
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();
			
			dx = pBri->m_dStationBridgeStt - pBri->m_dWS - Dom.Always(5);
			dy = pLine->GetElevationPlanEl(pBri->m_dStationBridgeStt) - pBri->GetThickPave(FALSE);
			if(pBri->m_bUnderGroundRahmen) dy += pBri->GetThickPave(FALSE);
			
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DASH);
			m_pStd->ApplyDimSetting(pDomP, &Dom);
			
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
			{
				long i = 0; for(i = 0; i < 3; i++)
				{
					if(pBri->m_nDeductNearTupagiBM != 1)
						baseDraw.DrawTv(&Dom, pBri->GetTvBackFillHorLine(TRUE, i));
					if(pBri->m_nDeductNearTupagiBM != 2)
						baseDraw.DrawTv(&Dom, pBri->GetTvBackFillHorLine(FALSE, i));
				}
			}
			else
			{
				long i = 0; for(i = 0; i < 3; i++)
				{
					baseDraw.DrawTv(&Dom, pBri->GetTvBackFillHorLine(TRUE, i));
				}
			}

			
			
			BOOL bStt	= TRUE;
			double dLen	= bStt ? -3000 : 3000;
			double dGap	= bStt ? -(500+pBri->m_dBWS) : (500+pBri->m_dBWE);
			
			dx += (dLen + dGap);//Dom.Always(30);
			
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
			m_pStd->ApplyDimSetting(pDomP, &Dom);
			sDir = _T("LEFT");
			
			xyStt = CDPoint(dx, dy);
			dArrDim.Add(-(pFooting->m_dHeightPavement));
			sArrText.Add(bInput ? "H1" : COMMA(fabs(pFooting->m_dHeightPavement)));
			sArrUnderText.Add("포장층");
			
			dArrDim.Add(-(pFooting->m_dHeightNosang));
			sArrText.Add(bInput ? "H2" : COMMA(fabs(pFooting->m_dHeightNosang)));
			sArrUnderText.Add("노상층");
			
			dh = dy - pFooting->m_dHeightPavement - pFooting->m_dHeightNosang - pFooting->m_dEL;
			dArrDim.Add(-(dh));
			sArrText.Add(COMMA(dh));
			sArrUnderText.Add("노체층");
			
			baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, 0, sDir, bForUseBM ? 0 : -1, FALSE, FALSE, TRUE);

			// 상
			Dom.SetDirection("TOP");
			CTwinVector tvBack	= pBri->GetTvBackFill(TRUE, bForUseBM ? 4 : 0, TRUE);
			dDist	= pFooting->m_dDistOffsetBackFill;
			Dom.DimMoveTo(tvBack.m_v2.x, tvBack.m_v2.y, 0);
			Dom.DimLineTo(tvBack.GetHorLength() + dDist, 0, COMMA(tvBack.GetHorLength()+dDist));

			tvUpperG.m_v2 = tvUpperG.m_v1;
			tvUpperG.m_v2.x = tvBack.m_v2.x - 2000;
			
			if(pBri->m_bUnderGroundRahmen)
			{
				m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_MIS4);
				Dom.LineTo(tvUpperG.m_v1, tvUpperG.m_v2);
				CHgBaseDrawStd::InsertHCadBlock(&Dom, "CGEARTH", tvUpperG.GetXyMid(), Dom.GetScaleDim());
			}
		}
		else if(iIndex == pBri->m_nQtyJigan)
		{ 
			double dx, dy, dh, dv, dEl;
			CTwinVector tv	= pBri->GetTvBackFill(FALSE, bForUseBM ? 4 : 0, TRUE);
			dx = tv.m_v1.x;
			dy = tv.m_v1.y;
			dEl = pLine->GetElevationPlanEl(pBri->m_dStationBridgeStt+pBri->m_dLengthBridge);
			dh = dEl-dy;
			dv = dh * pBri->m_footingEnd.m_dSlopeBackFill;
			
			tvUpperG.m_v1.x = pBri->m_dStationBridgeStt + 2000;
			tvUpperG.m_v1.y = dEl;

			sDir = _T("TOP");
			dArrDim.Add(-pBri->m_footingEnd.m_dDistOffsetBackFill);
			sArrText.Add("Db");
			sArrUnderText.Add(COMMA(pBri->m_footingEnd.m_dDistOffsetBackFill));
			xyStt = CDPoint(dx, dy);

			if(!bInput)
			{
				sArrText.Copy(sArrUnderText);
				sArrUnderText.RemoveAll();
			}
			nDimDan = 0;
			baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, bForUseBM ? 0 : -1, FALSE, FALSE, TRUE);
			*pDomP << Dom;
			
			dArrDim.RemoveAll();
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();
			dy = tvEL.GetXyTop().y;
			CDPoint xyResult;
			BOOL bMatch = GetXyMatchSegAndSeg(tv.m_v1,tv.m_v2,tvEL.m_v1,tvEL.m_v2,xyResult);
			if(bMatch)
			{
				dDist = xyResult.x - dx + pBri->m_footingStt.m_dDistOffsetBackFill;
				dArrDim.Add(dDist);
				dx -= pBri->m_footingStt.m_dDistOffsetBackFill;
				xyStt = CDPoint(dx, dy);
				baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, 0, FALSE, FALSE, TRUE);
				*pDomP << Dom;

				sDir = _T("LEFT");
				dArrDim.RemoveAll();
				sArrText.RemoveAll();
				sArrUnderText.RemoveAll();
				dx = tv.m_v1.x - pBri->m_footingStt.m_dDistOffsetBackFill - pBri->m_footingStt.m_dML;
				dy = tv.m_v1.y;
				dDist = tvEL.GetXyTop().y - dy;
				dArrDim.Add(dDist);
				xyStt = CDPoint(dx, dy);
				baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, 0, FALSE, FALSE, TRUE);
				*pDomP << Dom;
			}

			dx = tv.m_v1.x;
			dy = tv.m_v1.y;
			CString str("1:n(V:H)"); 
			if(!bInput)
				str.Format("1:%.1f", dv/dh);
			
			Dom.SetTextAlignHorz(TA_CENTER);
			Dom.TextOut(dx+dv/2-Dom.Always(1), dy+dh/2+Dom.Always(1), str);
			Dom.Rotate(CDPoint(dx+dv/2-Dom.Always(1), dy+dh/2+Dom.Always(1)), CDPoint(1,1/pBri->m_footingEnd.m_dSlopeBackFill));
			
			// H1, H2
			dArrDim.RemoveAll();
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();

//			dx = pBri->m_dStationBridgeStt + pBri->m_dLengthBridge + pBri->m_dWE +Dom.Always(5);
			dx += Dom.Always(5);
			dy = pLine->GetElevationPlanEl(pBri->m_dStationBridgeStt+pBri->m_dLengthBridge) - pBri->GetThickPave(FALSE);
			if(pBri->m_bUnderGroundRahmen) dy += pBri->GetThickPave(FALSE);

			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DASH);
			m_pStd->ApplyDimSetting(pDomP, &Dom);
			
			long i = 0; for(i = 0; i < 3; i++)
			{
				baseDraw.DrawTv(&Dom, pBri->GetTvBackFillHorLine(FALSE, i));
			}

			BOOL bStt	= FALSE;
			double dLen	= bStt ? -3000 : 3000;
			double dGap	= bStt ? -(500+pBri->m_dBWS) : (500+pBri->m_dBWE);
	
			dx += dLen + dGap;//Dom.Always(30);

			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
			m_pStd->ApplyDimSetting(pDomP, &Dom);
			sDir = _T("RIGHT");

			xyStt = CDPoint(dx, dy);
			dArrDim.Add(-(pBri->m_footingEnd.m_dHeightPavement));
			sArrText.Add(bInput ? "H1" : COMMA(fabs(pBri->m_footingEnd.m_dHeightPavement)));
			sArrUnderText.Add("포장층");

			dArrDim.Add(-(pBri->m_footingEnd.m_dHeightNosang));
			sArrText.Add(bInput ? "H2" : COMMA(fabs(pBri->m_footingEnd.m_dHeightNosang)));
			sArrUnderText.Add("노상층");
			
			dh = dy - pBri->m_footingEnd.m_dHeightPavement - pBri->m_footingEnd.m_dHeightNosang - pBri->m_footingEnd.m_dEL;
			dArrDim.Add(-(dh));
			sArrText.Add(COMMA(dh));
			sArrUnderText.Add("노체층");

			baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, 0, sDir, bForUseBM ? 0 : -1, FALSE, FALSE, TRUE);

			// 상
			CTwinVector tvBack	= pBri->GetTvBackFill(FALSE, bForUseBM ? 4 : 0, TRUE);
			dDist	= pBri->m_footingEnd.m_dDistOffsetBackFill;
			Dom.SetDirection("TOP");
			Dom.DimMoveTo(tvBack.m_v1.x-dDist, tvBack.m_v2.y, 0);
			Dom.DimLineTo(tvBack.GetHorLength()+dDist, 0, COMMA(tvBack.GetHorLength()+dDist));

			tvUpperG.m_v2 = tvUpperG.m_v1;
			tvUpperG.m_v2.x = tvBack.m_v2.x + 2000;
			
			if(pBri->m_bUnderGroundRahmen)
			{
				m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_MIS4);
				Dom.LineTo(tvUpperG.m_v1, tvUpperG.m_v2);
				CHgBaseDrawStd::InsertHCadBlock(&Dom, "CGEARTH", tvUpperG.GetXyMid(), Dom.GetScaleDim());
			}
		}
	}
	
	// 토공 Slope 치수
	if(bDimTypagi)
	{
		CFootingApp *pFooting;
		CTwinVectorArray tvArr;
		if(pBri->m_nTypeBridge ==BRIDGE_TYPE_BOX)
		{
			pFooting = &(pBri->m_footingBox);
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
		
		if(pFooting->m_dEL_DiggingFirst > pFooting->m_dEL-pFooting->GetHeight())
		{
			pBri->GetTvTogongLine(pTvArr,iIndex, FALSE, FALSE, FALSE, FALSE, pBri->m_nDeductNearTupagiBM);
			
			dArrDim.RemoveAll();
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();
			
			long nIndex = pTvArr.GetIdxByInfo(_T("오른쪽 이격"));
			int nCnt = pTvArr.GetSize();
			
			sDir = _T("RIGHT");
			
			int i=0; for(i=nIndex+1; i<nCnt; i++)
			{
				tv = pTvArr.GetAt(i);
				dArrDim.Add(tv.GetVerLength());
				sArrText.Add(tv.m_sInfo);
				sArrUnderText.Add(COMMA(tv.GetVerLength()));
				
				if(i == nIndex+1) xyStt.y = tv.m_v1.y;
			}
			
			nDimDan = 1;
			xyStt.x = tv.m_v2.x+Dom.Always(5);
			CHgBaseDrawStd baseDraw;
			baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, bForUseBM ? 0 : -1, dArrDim.GetSize()>1, FALSE, FALSE);			
		}
	}
	
	*pDomP << Dom;
}
// 외측벽체 없는 경우 시종점측 가상 교대.
void CDrawVertSection::DrawVertSectionAbut(CDomyun *pDomP)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	CHgBaseDrawStd baseDraw;

	CTwinVectorArray tvArr;

	// 시점측 ////////////////////
	if(!pBri->IsOutWall(TRUE))
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_CONC);
		pBri->GetTvVertSectionAbut(tvArr, TRUE);
		baseDraw.DrawTvArray(&Dom, tvArr, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());

		// 컷팅라인
		CDRect rect	= tvArr.GetRect();
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_CUTL);
		Dom.CutLightningAnyDir(CDPoint(rect.left, rect.bottom), CDPoint(rect.right, rect.bottom), 500);

		// 슈
		CTwinVector tv	= tvArr.GetTvByInfo("슈받침부");
		CJijum *pJ	= pBri->GetJijum(0);
		double dX	= tv.GetXyLeft().x + pJ->m_dLengthSlabToShoe[1];
		DrawSimpleShoe(&Dom, CDPoint(dX, tv.GetXyMid().y+400));
	}

	// 종점측 //////////////////////
	if(!pBri->IsOutWall(FALSE))
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_CONC);
		pBri->GetTvVertSectionAbut(tvArr, FALSE);
		baseDraw.DrawTvArray(&Dom, tvArr, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());

		// 컷팅라인
		CDRect rect	= tvArr.GetRect();
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_CUTL);
		Dom.CutLightningAnyDir(CDPoint(rect.left, rect.bottom), CDPoint(rect.right, rect.bottom), 500);

		// 슈
		CTwinVector tv	= tvArr.GetTvByInfo("슈받침부");
		CJijum *pJ	= pBri->GetJijum(pBri->m_nQtyJigan);
		double dX	= tv.GetXyLeft().x + pJ->m_dLengthSlabToShoe[0];
		DrawSimpleShoe(&Dom, CDPoint(dX, tv.GetXyMid().y+400));
	}

	*pDomP << Dom;
}

// 부상방지판 주철근 
void CDrawVertSection::DrawRebarMain_VertSection_BFooting(CDomyun *pDomP, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, CDRect rectLimit, long nCycle)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	CHgBaseDrawStd baseDraw;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	double dScale = Dom.GetScaleDim();
	long stt = bStt ? iSTT : iEND;
	long sw = bStt? 1 : -1;
	CString strStt = bStt ? _T("시점") : _T("종점");
	
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = nCycle == 0 ? &pBri->m_pArrRCMainBoxLowerPRF_Cycle1[stt] : &pBri->m_pArrRCMainBoxLowerPRF_Cycle2[stt];

	if(pArrRC->GetSize() == 0) return;
	CRebarInfoCycle *pRC	= pArrRC->GetAt(0);
	m_pStd->DrawRebarInfoCycle(&Dom, pRC, 0, rectLimit);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	if(bDimRebar)
	{
		CDoubleArray dArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
// 		CDPoint xyStt = CDPoint(0,0);
		CDPointArray xyArrSupport;
		long nCountHori, nCountVert;
		CTwinVector tvTop, tvSide, tvBottom;

		nCountHori = pBri->GetXyRebarSupport_VertSection_BFooting(xyArrSupport, bStt, Dom.GetScaleDim());
		//시작 위치 구하기
		pRC->m_tvArrRebar.GetTvByInfo(strStt + _T("부상방지저판상면"), tvTop);
		pRC->m_tvArrRebar.GetTvByInfo(strStt + _T("부상방지저판바깥측면"), tvSide);
		pRC->m_tvArrRebar.GetTvByInfo(strStt + _T("부상방지저판하단"), tvBottom);
		CDPoint xyStt = tvBottom.m_v2 ;
		xyStt.x -= sw * pBri->m_dCoverWallIn[0];
		xyStt.y -= pBri->m_dCoverWallIn[0];

		//수평철근 마지막부터 주철근 끝까지 거리
		double dSupportEnd = tvTop.m_v1.x + sw * ((nCountHori-1) * pBri->m_dPRF_CTC_Support);
		double dMainEnd = pBri->GetStationOnJijum(bStt ? 0 : pBri->m_nQtyJigan) - pBri->m_dCoverWallIn[0];
		double dDist = fabs(dMainEnd - dSupportEnd);
		CString strDimDir = bStt ? _T("LEFT") : _T("RIGHT");
		//점철근 - 수평배치
		if(bStt)
		{
			dArrDim.Add(pBri->m_dCoverWallIn[0]);
			if(nCountHori > 0)
			{
				for(long ix = 0; ix < nCountHori-1; ix ++)
					dArrDim.Add(pBri->m_dPRF_CTC_Support);
			}
			dArrDim.Add(dDist);
		}
		else
		{
			xyStt.x = dMainEnd;
			dArrDim.Add(dDist);
			if(nCountHori > 0)
			{
				for(long ix = 0; ix < nCountHori-1; ix ++)
					dArrDim.Add(pBri->m_dPRF_CTC_Support);
			}
			dArrDim.Add(pBri->m_dCoverWallIn[0]);
		}
		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, 0, "BOTTOM", 100, FALSE, TRUE, FALSE);

		//점철근 - 수직배치
		CDPointArray xyArrVert;
		dArrDim.RemoveAll();

		//수직으로 된 철근들만 골라낸다.
		for(long ix = 0; ix < xyArrSupport.GetSize(); ix++)
		{
			if(xyArrSupport.GetAt(ix).y < tvSide.m_v2.y - REBAR_SYMBOL_RADIUS*dScale/50.0&& xyArrSupport.GetAt(ix).y > tvSide.m_v1.y + REBAR_SYMBOL_RADIUS*dScale/50.0)
			{
				//경사가 있을수가 있으니 x좌표도 한번 확인..
				double dXPos = tvSide.m_v1.x + (pBri->m_dCoverWallIn[0] + REBAR_SYMBOL_RADIUS * 2) * sw;
				if((bStt && xyArrSupport.GetAt(ix).x < dXPos) || (!bStt && xyArrSupport.GetAt(ix).x > dXPos))
				{
						xyArrVert.Add(xyArrSupport.GetAt(ix));
				}
			}
		}
		nCountVert = xyArrVert.GetSize();
		dDist = fabs(tvSide.m_v1.y - (tvSide.m_v2.y - (nCountVert * pBri->m_dPRF_CTC_SupportVert)));
		xyStt = tvSide.m_v1 - CDPoint(0,1) * pBri->m_dCoverWallIn[0];
		
		dArrDim.Add(pBri->m_dCoverWallIn[0]);
		dArrDim.Add(dDist);
		for(long ix = 0; ix < nCountVert; ix++)
			dArrDim.Add(pBri->m_dPRF_CTC_SupportVert);
		if(nCountVert == 1)
			dArrDim.Add(pBri->m_dPRF_CTC_SupportVert);
		dArrDim.Add(pBri->m_dCoverWallIn[0]);

		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, 1, strDimDir, 100, TRUE, TRUE, FALSE);
	}

	if(bMarkRebar)
	{
		CString sDescription	= _T("");
		sDescription.Format("부상방지저판 주철근%s", pBri->m_pARcBridgeDataStd->GetStringCircleNum(nCycle+1));
		CString sMark = pBri->GetMarkRebarEtc(BF_REBAR_M, TRUE, FALSE, FALSE, sDescription);
		if(!bInput) sMark = _T("BO1");
		if(m_pStd->IsMarkRebar(sMark))
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

			CString sDia		= pBri->m_pARcBridgeDataStd->GetStringDia(pBri->m_dPRF_Dia_Main, pBri->GetValueFy(ePartLowerSlab));
			CString sCtc("");
			if(bInput)
				sCtc.Format("C.T.C %.0f", pBri->GetCTCRebarPRF_Main());

			CTwinVector tvUpper	;
			if(pRC->m_tvArrRebar.GetTvByInfo(strStt+"부상방지저판상면", tvUpper))
			{
				CConcSymbol sym(m_pStd->m_pARoadOptionStd);
				// sym.RebarMarkLineUpper(&Dom, tvUpper.GetXyMid(), sMark, sDia, bStt, 0, TRUE, FALSE, sCtc);

				CRebar *pRb = pBri->GetRebarEtcByDescription(sDescription, FALSE);

				SRebarMarkInputData mark_input_data;
				SRebarMarkviewData mark_view_data;
				mark_input_data.mark = sMark;
				mark_input_data.dia = sDia;
				mark_input_data.position = tvUpper.GetXyMid();
				mark_input_data.ctc = pBri->GetCtcRebarMain(FALSE);
				mark_input_data.type_rebar = pRb? pRb->m_nTypeRebar : -1;
				mark_input_data.left_dir =  bStt;
				mark_input_data.height = 0;
				mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCtc;

				mark_view_data.count_mark = TRUE;
				mark_view_data.mark_pos = eMarkUpper;

				sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
			}
		}
	}
	
	*pDomP << Dom;
}
// 부상방지판 수평철근 
void CDrawVertSection::DrawRebarSupport_VertSection_BFooting(CDomyun *pDomP, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, CDRect rectLimit)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	// 철근 가져와서 그리기
	CHgBaseDrawStd baseDraw;
	CDPointArray xyArr;
	double dDomSacle = Dom.GetScaleDim();
	pBri->GetXyRebarSupport_VertSection_BFooting(xyArr, bStt, dDomSacle);
	long stt = bStt ? iSTT : iEND;
	CString strStt = bStt ? _T("시점") : _T("종점");
	double sw = bStt ? -1.0 : 1.0;
	double dScale = Dom.GetScaleDim();
	// rectLimit 적용
	if(rectLimit != CDRect(0, 0, 0, 0))
	{
		CTwinVectorArray tvArrLimit;
		CVectorArray vecArrLimit;
		tvArrLimit.SetFromRect(rectLimit);
		tvArrLimit.GetToVectorArray(vecArrLimit);

		CRebarPlacing rb;
		rb.GetXyByVectorArrayArea(xyArr, vecArrLimit, TRUE);
	}

	CDPoint xyDirScale	= bStt ? CDPoint(1, 0) : CDPoint(-1, 0);
	baseDraw.DrawPointRebar(&Dom, xyArr, xyDirScale);

	// 치수 기입
	if(bDimRebar)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC =&pBri->m_pArrRCMainBoxLowerPRF_Cycle1[stt];

		if(pArrRC->GetSize() == 0) return;
		CRebarInfoCycle *pRC	= pArrRC->GetAt(0);
		CDPointArray xyArrSupport;
		CTwinVector tvSide;
		pRC->m_tvArrRebar.GetTvByInfo(strStt + _T("부상방지저판바깥측면"), tvSide);
		pBri->GetXyRebarSupport_VertSection_BFooting(xyArrSupport, bStt, Dom.GetScaleDim());

		CDPointArray xyArr;
		double dCtcVert = pBri->m_dPRF_CTC_SupportVert;
		long nRebarCount =1;
		CDPoint xyLeft, xyMark;
		CString sMark, sDia;
		while(1)
		{
			if(dCtcVert<0) break;
			double dYPos = tvSide.m_v2.y - nRebarCount*dCtcVert;
			if(dYPos<=tvSide.m_v1.y + MIN_REBAR_SPACE) break;
			GetXyMatchLineAndLine(tvSide.m_v1, tvSide.GetVDir(), CDPoint(tvSide.m_v2.x, dYPos), CDPoint(1,0), xyLeft);

			//수직으로된 수평철근 개수
			xyLeft.x -= REBAR_SYMBOL_RADIUS*dScale/50.0*sw;
			xyArr.Add(CDPoint(xyLeft));
			nRebarCount++;
		}
		xyMark = (nRebarCount > 1) ? tvSide.m_v2 - dCtcVert*CDPoint(0,1) : tvSide.m_v2 - REBAR_SYMBOL_RADIUS*CDPoint(0,1)*dScale/50;
		sMark  = "BO2";
		sDia = pBri->m_pARcBridgeDataStd->GetStringDia(pBri->m_dPRF_Dia_Support, pBri->GetValueFy(ePartLowerSlab));
		
		double dDisUpper = (nRebarCount > 1) ? dCtcVert-REBAR_SYMBOL_RADIUS*dScale/50 : 0;
		double dDisLower = 0;
		if(nRebarCount > 2)
			dDisLower = dCtcVert-REBAR_SYMBOL_RADIUS*dScale/50;
		else
			dDisLower = ~(xyMark - tvSide.m_v1) - REBAR_SYMBOL_RADIUS*dScale/50;

		double dHeightMark = 200;
		
		CConcSymbol sym(m_pStd->m_pARoadOptionStd);
		// sym.RebarMarkPointSide(&Dom, xyMark, dDisUpper, dDisLower, sMark, sDia, bStt, dHeightMark, CDPoint(0, 0));

		SRebarMarkInputData mark_input_data;
		SRebarMarkviewData mark_view_data;
		mark_input_data.mark = sMark;
		mark_input_data.dia = sDia;
		mark_input_data.position = xyMark;
		mark_input_data.dist_left_mark = dDisUpper;
		mark_input_data.dist_right_mark = dDisLower;
		mark_input_data.ctc = dCtcVert;
		mark_input_data.type_rebar = 1;
		mark_input_data.left_dir =  bStt;
		mark_input_data.height = dHeightMark;
		
		mark_view_data.count_mark = FALSE;
		mark_view_data.line_rebar = FALSE;
		mark_view_data.mark_pos = eMarkSide;

		sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
	}
	
	*pDomP << Dom;
}

//부상방지판 전단철근 
void CDrawVertSection::DrawRebarShear_VertSection_BFooting(CDomyun *pDomP, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, long nCycle, CDRect rectLimit)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	CHgBaseDrawStd baseDraw;
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);
	// 철근 가져와서 그리기
	CTwinVectorArray tvArrShear;
	long nCountShear = pBri->GetTvRebarShear_VertSection_BFooting(tvArrShear, bStt);
	baseDraw.DrawTvArray(&Dom, tvArrShear);

	CString sDescription	= _T("");
	sDescription.Format("%s 부상방지저판 전단철근%s", bStt ? "시점" : "종점", pBri->m_pARcBridgeDataStd->GetStringCircleNum(nCycle+1));
	CString sMark = _T("B03");
	if(bMarkRebar)
	{
		if(m_pStd->IsMarkRebar(sMark))
		{
			if(nCountShear < 0) return;

			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
			CString sDia		= pBri->m_pARcBridgeDataStd->GetStringDia(pBri->m_dPRF_Dia_Shear, pBri->GetValueFy(ePartLowerSlab, TRUE));
			CString sCtc("");
			double dCtcShear = pBri->m_dPRF_CTC_Shear;

			CDPoint xyStt, xyEnd;
			for(long ix = 0; ix < nCountShear; ix++)
			{
				CTwinVector tv = tvArrShear.GetAt(ix);
				tv.m_v2.y += (pBri->m_dPRF_H1 - pBri->m_dCoverWallIn[0] * 2) / 2;
				Sym.RebarCountMarkOne(&Dom, tv.m_v2);

				if(ix == 0) xyStt = tv.m_v2;
				xyEnd = tv.m_v2;
			}

			Dom.LineTo(xyStt, xyEnd);
			double dHeightMark = Dom.Always(12) + pBri->m_dPRF_H1/2;
			//Sym.RebarMarkLineLower(&Dom, xyStt + CDPoint(1,0)*dCtcShear/2, sMark, sDia, TRUE, dHeightMark, FALSE, FALSE, sCtc);

			CRebarShearDetail sd;

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = sMark;
			mark_input_data.dia = sDia;
			mark_input_data.position = xyStt + CDPoint(1,0)* (nCountShear > 1 ? dCtcShear/2 : 0);
			mark_input_data.ctc = dCtcShear;
			mark_input_data.type_rebar = sd.GetRebarType(pBri->m_nPRF_Type_Shear);
			mark_input_data.left_dir =  TRUE;
			mark_input_data.height = dHeightMark;
			mark_input_data.text = m_pStd->m_pARoadOptionStd->m_DimSet.SimpleMarking? _T("") : sCtc;

			mark_view_data.count_mark = FALSE;
			mark_view_data.mark_pos = eMarkLower;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}
	}

	*pDomP << Dom;	
}

// 브라켓 주철근 
void CDrawVertSection::DrawRebarMain_VertSection_Bracket(CDomyun *pDomP, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, long nDrawLeft, CDRect rectLimit, long nCycle)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	long stt	= bStt ? iSTT : iEND;
	nCycle = (nCycle % 2) == 0 ? 0 : 1;	// ARCBRIDGE-3632
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = nCycle == 0 ? &pBri->m_pArrRCMainOutWallBracket_Cycle1[stt] : &pBri->m_pArrRCMainOutWallBracket_Cycle2[stt];

	if(pArrRC->GetSize() == 0) return;
	CRebarInfoCycle *pRC	= pArrRC->GetAt(0);
	
	// RebarInfoCycle 그리기
	m_pStd->DrawRebarInfoCycle(&Dom, pRC, nDrawLeft, rectLimit);

	CString sDescription	= _T("");
	sDescription.Format("%s 브라켓 주철근%s", bStt ? "시점" : "종점", pBri->m_pARcBridgeDataStd->GetStringCircleNum(nCycle+1));
	CString sMark			= pBri->GetMarkRebarEtc(BR_REBAR_M, bInput, FALSE, FALSE, sDescription);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		CString sStt		= bStt ? "시점" : "종점";
		CString sDia		= m_pStd->m_pARcBridgeDataStd->GetStringDia(nCycle == 0 ? pBri->m_dDiaRebarMain_Bracket_Cycle1[stt] : pBri->m_dDiaRebarMain_Bracket_Cycle2[stt], pBri->GetValueFy(bStt? ePartOutWallStt : ePartOutWallEnd));
		CDPoint vAngGisi	= bStt ? ToDPointFrDegree(45) : ToDPointFrDegree(135);
		CTwinVector tvHunch	;
		
		if(pRC->m_tvArrRebar.GetTvByInfo(sStt+"브라켓헌치", tvHunch))
		{
			CConcSymbol sym(m_pStd->m_pARoadOptionStd);
			sym.RebarCountMarkOne(&Dom, tvHunch.GetXyMid(), CDPoint(-1, 1));
			// sym.RebarMarkLineLower(&Dom, tvHunch.GetXyMid(), sMark, sDia, bStt, 0, FALSE, FALSE, "", vAngGisi);

			CRebar *pRb = pBri->GetRebarEtcByDescription(sDescription, FALSE);
			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;
			mark_input_data.mark = sMark;
			mark_input_data.dia = sDia;
			mark_input_data.position = tvHunch.GetXyMid();
			mark_input_data.ctc = pBri->m_dCTC_MainRebar;
			mark_input_data.type_rebar = pRb? pRb->m_nTypeRebar : -1;
			mark_input_data.left_dir =  TRUE;
			mark_input_data.height = 0;

			mark_view_data.count_mark = FALSE;
			mark_view_data.mark_pos = eMarkLower;
			mark_view_data.dir_leadr_line = vAngGisi;

			sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}
	}

	*pDomP << Dom;
}

// 브라켓 배력근
void CDrawVertSection::DrawRebarSupport_VertSection_Bracket(CDomyun *pDomP, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, CDRect rectLimit)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	// 철근 가져와서 그리기
	CHgBaseDrawStd baseDraw;
	CDPointArray xyArr;
	pBri->GetXyRebarSupport_VertSection_Bracket(xyArr, bStt);
	
	// rectLimit 적용
	if(rectLimit != CDRect(0, 0, 0, 0))
	{
		CTwinVectorArray tvArrLimit;
		CVectorArray vecArrLimit;
		tvArrLimit.SetFromRect(rectLimit);
		tvArrLimit.GetToVectorArray(vecArrLimit);

		CRebarPlacing rb;
		rb.GetXyByVectorArrayArea(xyArr, vecArrLimit, TRUE);
	}

	CDPoint xyDirScale	= bStt ? CDPoint(1, 0) : CDPoint(-1, 0);
	baseDraw.DrawPointRebar(&Dom, xyArr, xyDirScale);
	
	// 치수 기입
	if(bDimRebar)
	{

	}

	// 마킹
	CString sDescription	= _T("");
	sDescription.Format("%s 브라켓 배력철근", bStt ? "시점" : "종점");
	CString sMark			= pBri->GetMarkRebarEtc(BR_REBAR_S, bInput, FALSE, FALSE, sDescription);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		CRebarPlacing rb;
		rb.ReverseRebar(xyArr);

		double dCtc = 0;
		if(xyArr.GetSize() > 1)
		{
			dCtc = ~(xyArr.GetAt(0) - xyArr.GetAt(1));
		}

		CRebar *pRb = pBri->GetRebarEtcByDescription(sDescription, FALSE);
		long stt	= bStt ? iSTT : iEND;
		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
		mark.SetRebar(xyArr);
		mark.m_sMark		= sMark;
		mark.m_sDia			= pBri->m_pARcBridgeDataStd->GetStringDia(pBri->m_dDiaRebarSupport_Bracket[stt], pBri->GetValueFy(bStt? ePartOutWallStt : ePartOutWallEnd));
		mark.m_nTypeMarkDir	= bStt ? TYPE_MARKDIR_RIGHTTAN : TYPE_MARKDIR_RIGHTTAN;
		mark.m_nTypeMarkPointUpAndDown	= bStt ? TYPE_MARKPOINT_UPPER : TYPE_MARKPOINT_LOWER;
		mark.m_dCtc = dCtc;
		mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;
		mark.MarkRebar();
	}

	*pDomP << Dom;
}

void CDrawVertSection::DrawRebarShear_VertSection_Bracket(CDomyun *pDomP, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, CDRect rectLimit)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	CHgBaseDrawStd baseDraw;

	// 철근 가져와서 그리기
	CTwinVectorArray tvArr;
	pBri->GetTvRebarShear_VertSection_Bracket(tvArr, bStt);

	if(bDimRebar)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		long stt	= bStt ? iSTT : iEND;
		if(pBri->m_pArrRCMainOutWallBracket_Cycle1[stt].GetSize() > 0)
		{
			CRebarInfoCycle *pRC	= pBri->m_pArrRCMainOutWallBracket_Cycle1[stt].GetAt(0);
			CDRect rect	= pRC->m_tvArrRebar.GetRect();

			CDPointArray xyArrDim;
			CStringArray sArrText;
			CStringArray sArrUnderText;
			long nDimDan	= 2;
			CString sDir	= bStt ? "LEFT" : "RIGHT";
			CDPoint xyStt	= CDPoint(bStt ? rect.left : rect.right, rect.bottom);

			CDPointArray xyArr;
			long i = 0; for(i = 0; i < tvArr.GetSize(); i++)
				xyArr.Add(tvArr.GetAt(i).m_v1);

			CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_ShearRebar_Bracket[stt];
			m_pStd->GetDimInfoAtPlaceRebarByBlock(xyArr, xyArrDim, sArrText, sArrUnderText, pPlace, bStt ? "VS" : "VE", xyStt, CDPoint(0, 1), bInput, TRUE, 90, 90);

			CTwinVector tv	= pBri->m_tvArrVertSection.GetTvByInfo(bStt ? "시점브라켓상면" : "종점브라켓상면");

			xyArrDim.Add(CDPoint(xyStt.x, rect.top));
			sArrText.Add("0");
			sArrUnderText.Add("0");

			xyArrDim.Add(CDPoint(xyStt.x, tv.m_v1.y));
			sArrText.Add("0");
			sArrUnderText.Add("0");

			baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, TRUE, FALSE);
		}
	}

	// 전단철근 그리기
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);
	m_pStd->DrawTvArrayWidthDashDot(&Dom, tvArr, FALSE);

	CString sDescription	= _T("");
	sDescription.Format("%s 브라켓 전단철근", bStt ? "시점" : "종점");
	CString sMark	= pBri->GetMarkRebarEtc(BR_REBAR_V, bInput, FALSE, FALSE, sDescription);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark) && tvArr.GetSize() > 0)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		CTwinVectorArray tvArrTmp;
		CTwinVector tvRebar	= tvArr.GetAt(0);
		CTwinVector tv;
		tv.m_v1	= tvRebar.GetXyLeft();
		tv.m_v2	= tv.m_v1 + CDPoint(0, -1) * 1000;
		tvArrTmp.Add(tv);

		tv.m_v1	= tvRebar.GetXyRight();
		tv.m_v2	= tv.m_v1 + CDPoint(0, -1) * 1000;
		tvArrTmp.Add(tv);

		CRebar *pRb = pBri->GetRebarEtcByDescription(sDescription, FALSE);
		long stt	= bStt ? iSTT : iEND;
		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
		mark.SetRebar(tvArr, tvArrTmp);
		mark.m_sMark					= sMark;
		mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pBri->m_RebarShearDetail_Bracket[stt].m_dDia, pBri->GetValueFy(bStt? ePartOutWallStt : ePartOutWallEnd, TRUE));
		mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_POS;
		mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
		mark.m_nTypeMarkDir				= bStt ? TYPE_MARKDIR_LEFT : TYPE_MARKDIR_RIGHT;
		mark.m_dPosScaleMarkStt			= 0.5;
		mark.m_dAddHeight				= bStt ? pBri->m_dWS/2+pBri->m_dBWS : pBri->m_dWE/2+pBri->m_dBWE;
		mark.m_dCtc = pBri->m_dCTC_ShearRebarBracket;
		mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;

		mark.MarkRebar();
	}

	if(bDimRebar)
	{

	}

	*pDomP << Dom;	
}

// 지점별로 헌치부 표식 그리기
// 현재는 그냥 동그라미로 그림
// BOOL bLeft : V형 벽체를 위해 미리 만들어둠
void CDrawVertSection::DrawSignHunchByJijum(CDomyun *pDomP, long nJijum, BOOL bLeft, BOOL bUpper)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(nJijum < 0 || nJijum > pBri->m_nQtyJigan) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	// 원 제원
	CDPoint xyCen	= CDPoint(0, 0);
	double dR		= 0;

	// 중심좌표 구하기 /////////////////////////////////
	if(nJijum == 0 || nJijum == pBri->m_nQtyJigan)
	{
		BOOL bStt	= nJijum == 0;
		CString sStt = _T("");

		if(!pBri->IsOutWall(bStt))
		{
			sStt	= bStt ? "시점슬래브측면" : "종점슬래브측면";
			CTwinVector tv	= pBri->m_tvArrVertSection.GetTvByInfo(sStt);
			xyCen	= tv.GetXyMid();
		}
		else
		{
			double ul = bUpper ? -1.0 : 1.0;
			double dThickSlab = bUpper ? pBri->m_dTS : pBri->m_dTS_Lower;

			if(bUpper) 	
				sStt	= bStt ? "슬래브상면(시점벽체위)" : "슬래브상면(종점벽체위)";
			else	
				sStt	= bStt ? "하부슬래브하면(시점벽체아래)" : "하부슬래브하면(종점벽체아래)";

			CTwinVector tv = pBri->m_tvArrVertSection.GetTvByInfo(sStt);
			xyCen = CDPoint(tv.GetXyMid().x,  tv.GetXyMid().y + dThickSlab/2 * ul);
		}
	}
	else
	{
		long nIdxHunch	= pBri->GetIdxHunchInWall(nJijum-1, bLeft);
		CString strL	= _T(""),
				strR	= _T("");
		if(bUpper)
		{
			strL.Format("%d번째내측헌치좌측면", nIdxHunch+1);
			strR.Format("%d번째내측헌치우측면", nIdxHunch+1);
		}
		else
		{
			strL.Format("%d번째내측하부헌치좌측면", nIdxHunch+1);
			strR.Format("%d번째내측하부헌치우측면", nIdxHunch+1);
		}

		CTwinVector tvL	= pBri->m_tvArrVertSection.GetTvByInfo(strL),
					tvR	= pBri->m_tvArrVertSection.GetTvByInfo(strR);
		xyCen	= (tvL.GetXyLeft() + tvR.GetXyRight())/2;
	}

	// 반지름은 각 지점중 가장 큰 값을 사용함 ///////////
	// 시점
	CTwinVector tv	= pBri->m_tvArrVertSection.GetTvByInfo("시점벽체헌치");
	dR				= max(tv.GetVerLength(), tv.GetHorLength());

	// 종점
	tv	= pBri->m_tvArrVertSection.GetTvByInfo("종점벽체헌치");
	dR				= max(dR, max(tv.GetVerLength(), tv.GetHorLength()));

	// 중간지점
	long nCountInWall	= pBri->GetCountInWall();
	for(long wall = 0; wall < nCountInWall; wall++)
	{
		long nIdxHunch	= pBri->GetIdxHunchInWall(wall, bLeft);
		CString strL	= _T(""),
				strR	= _T("");
		strL.Format("%d번째내측헌치좌측면", nIdxHunch+1);
		strR.Format("%d번째내측헌치우측면", nIdxHunch+1);

		CTwinVector tvL	= pBri->m_tvArrVertSection.GetTvByInfo(strL),
					tvR	= pBri->m_tvArrVertSection.GetTvByInfo(strR);
		double dLenH	= fabs(tvL.GetXyLeft().x - tvR.GetXyRight().x) / 2;
		double dLenV	= tvL.GetVerLength();
		dR		= max(dR, max(dLenH, dLenV));
	}

	// 그림
	Dom.Circle(xyCen, dR, FALSE);
	
	*pDomP << Dom;
}

// 동바리 치수 기입(수량출력용)
void CDrawVertSection::DimPostForBM(CDomyun *pDomP)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	CARcBridgeDataStd *pDataStd	= m_pStd->m_pARcBridgeDataStd;
	if(!pDataStd) return;
	
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDomP, &Dom);

	// 육교용 동바리 크기
	const double dWidthHeight	= 4000;

	CTwinVector tv;
	CDoubleArray dArrY;
	CDoubleArray dArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CHgBaseDrawStd baseDraw;

	long nDimDan	= 0;
	long nTypePost  = pBri->m_nTypePost;
	long nCountFooting	= pBri->GetCountFooting();
	CString sDir	= _T("");
	CDPoint xyStt	= CDPoint(0, 0);
	BOOL bBoxType = (pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)? TRUE : FALSE;

	// 좌, 우
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		CString sStt	= bStt ? "시점" : "종점";
		sDir	= bStt ? "LEFT" : "RIGHT";
		BOOL bTotal	= TRUE;
		
		dArrY.RemoveAll();
		dArrDim.RemoveAll();
		
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_PORTAL || pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 || pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH || pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
		{
			CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
				pFooting = &pBri->m_footingBox;

			CTwinVectorArray tvArrPost;
			pBri->GetTvPostBracket(tvArrPost, TRUE);
			if(bBoxType)
			{
				double dLower = pFooting->m_exFooting.m_tvArrFront.GetXyTop().y;
				dArrY.Add(pFooting->m_exFooting.m_tvArrFront.GetXyBottom().y);
				dArrY.Add(dLower);
				if(pBri->m_bExistProtectionRiseFooting)
				{
					dArrY.Add(dLower + pBri->m_dPRF_H1);
					dArrY.Add(dLower + pBri->m_dPRF_H1 + pBri->m_dPRF_H2);
				}
			}
			else
			{
				dArrY.Add(pFooting->m_exFooting.m_tvArrFront.GetXyBottom().y);
				dArrY.Add(pFooting->m_tvArrFront.GetXyBottom().y);
				dArrY.Add(pFooting->m_tvArrFront.GetXyTop().y);
			}
		
			if(tvArrPost.GetSize()>0)
				dArrY.Add(tvArrPost.GetXyBottom().y);
			if(nTypePost==0)	// 육교용
			{
				if(pBri->m_nTypePostBaseLine == 2)
					dArrY.Add(frM(pBri->m_dPostLineEL)+dWidthHeight);
			}
			if(tvArrPost.GetSize()>0)
				dArrY.Add(tvArrPost.GetXyTop().y);
			if(pBri->m_tvArrVertSection.GetTvByInfo(sStt+"브라켓상면", tv))
				dArrY.Add(tv.GetXyLeft().y);
			if(pBri->m_tvArrVertSection.GetTvByInfo(sStt+"브라켓상부측면", tv))
				dArrY.Add(tv.GetXyTop().y);

			bTotal	= TRUE;
			if(bBoxType)
			{
				CString sEnd = _T("");
				if(pBri->m_bExistProtectionRiseFooting)
					sEnd.Format(_T("%s부상방지저판바깥측면"), sStt);
				else
					sEnd.Format(_T("하부슬래브하면(%s벽체아래)"), sStt);
				CTwinVector tvEnd	= pBri->m_tvArrVertSection.GetTvByInfo(sEnd);
				xyStt.x	= bStt ? tvEnd.m_v1.x : tvEnd.m_v2.x;
			}
			else
			{
				xyStt.x	= bStt ? pFooting->m_tvArrFront.GetRect().left : pFooting->m_tvArrFront.GetRect().right;
			}
		}
		else
		{
			// 일단 시, 종점에서만 지반고 구하기
			CTwinVector tvHunchL, tvHunchR;
			CLineInfo *pLine = pBri->GetLineBase();
			pBri->m_tvArrVertSection.GetTvByInfo("시점슬래브측면", tvHunchL);
			tvHunchL.m_v1	= tvHunchL.GetXyBottom();
			tvHunchL.m_v2	= tvHunchL.GetXyBottom();
			pBri->m_tvArrVertSection.GetTvByInfo("종점슬래브측면", tvHunchR);
			tvHunchR.m_v1	= tvHunchR.GetXyBottom();
			tvHunchR.m_v2	= tvHunchR.GetXyBottom();
			// 기초
			CFootingApp *pFooting = pBri->GetFooting( bStt ? 0 : nCountFooting-1);
			if(pFooting)
			{
				dArrY.Add(pFooting->m_exFooting.m_tvArrFront.GetXyBottom().y);
				dArrY.Add(pFooting->m_tvArrFront.GetXyBottom().y);
				
				// 기초 상단이 큰지 지반고가 큰지?
				double dEL=0;
				double dFootingTop = pFooting->m_tvArrFront.GetXyTop().y;
				if(bStt)	pLine->GetElevationErathOnStation(tvHunchL.GetXyLeft().x, dEL);
				else		pLine->GetElevationErathOnStation(tvHunchR.GetXyRight().x, dEL);
				if(dEL>dFootingTop)
				{
					dArrY.Add(dFootingTop);
					dArrY.Add(dEL);
				}
				else
				{
					dArrY.Add(dEL);
					dArrY.Add(dFootingTop);
				}
				pBri->m_tvArrVertSection.GetTvByInfo(sStt+"슬래브측면", tv);
				dArrY.Add(tv.GetXyBottom().y);
				dArrY.Add(tv.GetXyTop().y);

				bTotal	= TRUE;
				xyStt.x	= tv.m_v1.x;
			}
		}

		if(dArrY.GetSize() > 0)
		{
			dArrY.Sort();
			xyStt.y	= dArrY.GetAt(0);
			long i = 0; for(i = 0; i < dArrY.GetSize()-1; i++)
				dArrDim.Add(dArrY.GetAt(i+1)-dArrY.GetAt(i));
				
			baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, bTotal, FALSE, FALSE);
		}
	}
	
	// 상
	CDoubleArray dArrX;
	dArrDim.RemoveAll();
	if(pBri->IsOutWall(TRUE))
	{
		if(pBri->m_tvArrVertSection.GetTvByInfo("시점브라켓상면", tv))
		{
			dArrX.Add(tv.GetXyLeft().x);
			dArrX.Add(tv.GetXyRight().x);
		}
		if(pBri->m_tvArrVertSection.GetTvByInfo("시점벽체우측면", tv))
			dArrX.Add(tv.m_v2.x);
	}
	else
	{
		if(pBri->m_tvArrVertSection.GetTvByInfo("시점슬래브측면", tv))
			dArrX.Add(tv.m_v2.x);
		CFootingApp *pFooting = pBri->GetFooting(0);
		if(pFooting && pFooting->m_tvArrFront.GetTvByInfo("기초좌측면", tv))
			dArrX.Add(tv.m_v1.x);
	}

	long nJigan = 0;
	for(long wall = 0; wall < pBri->GetCountInWall(); wall++)
	{
		CWallApp *pWall	= pBri->GetInWall(wall);
		if(!pWall) continue;
		if(!pWall->m_bIs) continue;
		
		CTwinVectorArray tvArrUpper, tvArrLeft, tvArrRight, tvArrMid, tvArrArch, tvArrMid2;
		BOOL bOK = pBri->GetTvPostJigan(tvArrUpper, tvArrLeft, tvArrRight, tvArrMid, tvArrArch, tvArrMid2, nJigan++);
		if(tvArrUpper.GetSize() < 2) continue;
		if(!bOK)
		{
			tv = tvArrUpper.GetAt(1);
			dArrX.Add(tv.GetXyLeft().x);
		}
		if(pWall->m_tvArrFront.GetTvByInfo("벽체좌측면", tv))
			dArrX.Add(tv.m_v2.x);
		if(!bOK)
		{
			long nSize = tvArrUpper.GetSize();
			tv = tvArrUpper.GetAt(nSize-1);
			dArrX.Add(tv.GetXyRight().x);
		}		
		if(pWall->m_tvArrFront.GetTvByInfo("벽체우측면", tv))
			dArrX.Add(tv.m_v2.x);
	}

	if(pBri->IsOutWall(FALSE))
	{
		CTwinVectorArray tvArrUpper, tvArrLeft, tvArrRight, tvArrMid, tvArrArch, tvArrMid2;
		BOOL bOK = pBri->GetTvPostJigan(tvArrUpper, tvArrLeft, tvArrRight, tvArrMid, tvArrArch, tvArrMid2, nJigan++);
		if(tvArrUpper.GetSize() > 1 || tvArrMid.GetSize() > 1)
		{
			if(!bOK)
			{
				tv = tvArrUpper.GetSize() > 1 ? tvArrUpper.GetAt(1) : tvArrMid.GetAt(1);
				dArrX.Add(tv.GetXyLeft().x);
				dArrX.Add(tv.GetXyRight().x);
			}
			if(pBri->m_tvArrVertSection.GetTvByInfo("종점벽체좌측면", tv))
				dArrX.Add(tv.m_v1.x);

			if(pBri->m_tvArrVertSection.GetTvByInfo("종점브라켓상면", tv))
			{
				dArrX.Add(tv.GetXyLeft().x);
				dArrX.Add(tv.GetXyRight().x);
			}
		}
	}
	else
	{
		CFootingApp *pFooting = pBri->GetFooting(nCountFooting-1);
		if(pFooting && pFooting->m_tvArrFront.GetTvByInfo("기초우측면", tv))
			dArrX.Add(tv.m_v1.x);
		if(pBri->m_tvArrVertSection.GetTvByInfo("종점슬래브측면", tv))
			dArrX.Add(tv.m_v1.x);
	}

	dArrX.Sort(TRUE);
	if(dArrX.GetSize() > 0)
	{
		sDir	= _T("TOP");
		xyStt.x	= dArrX.GetAt(0);
		xyStt.y	= pBri->m_tvArrVertSection.GetRect().top;
		long i = 0; for(i = 0; i < dArrX.GetSize()-1; i++)
			dArrDim.Add(dArrX.GetAt(i+1) - dArrX.GetAt(i));

		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, FALSE, FALSE);
	}

	if(bBoxType)
	{
// 		dArrX.RemoveAll();
// 		// 하면 치수
// 		CString sEnd = _T("");
// 		for(long nstt=0; nstt<2; nstt++)
// 		{
// 			if(pBri->m_bExistProtectionRiseFooting)
// 				sEnd.Format(_T("%s부상방지저판하단"), sStt);
// 			else
// 				sEnd.Format(_T("하부슬래브(%s벽체아래)"), sStt);
// 			CTwinVector tvEnd	= pBri->m_tvArrVertSection.GetTvByInfo(sEnd);
// 
// 			if(pBri->m_bExistProtectionRiseFooting)
// 			{
// 
// 			}
// 		}
// 
// 		xyStt.x	= bStt ? tvEnd.m_v1.x : tvEnd.m_v2.x;
	}
	*pDomP << Dom;
}

void CDrawVertSection::DimScaffoldForBM(CDomyun *pDomP)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	CARcBridgeDataStd *pDataStd	= m_pStd->m_pARcBridgeDataStd;
	if(!pDataStd) return;
	
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);	
	m_pStd->ApplyDimSetting(pDomP, &Dom);
	CHgBaseDrawStd baseDraw;

	CDoubleArray dArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir	= _T("TOP");
	CDPoint xyStt	= CDPoint(0, 0);
	long nDimDan	= 0;
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt = stt == iSTT;
		CTwinVectorArray tvArr;
		pBri->GetTvScaffoldSection(tvArr, bStt);
		if(tvArr.GetSize() == 0) continue;

		// 상
		dArrDim.RemoveAll();
		dArrDim.Add(tvArr.GetHorLength());
		sDir	= _T("TOP");
		xyStt.x	= tvArr.GetXyLeft().x;
		xyStt.y	= tvArr.GetXyTop().y;

		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, 500, FALSE, FALSE, FALSE);

		// 측면 
		dArrDim.RemoveAll();
		dArrDim.Add(tvArr.GetVerLength());
		sDir	= bStt ? _T("LEFT") : _T("RIGHT");
		xyStt.x	= bStt ? tvArr.GetXyLeft().x : tvArr.GetXyRight().x;
		xyStt.y	= tvArr.GetXyBottom().y;

		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, 500, FALSE, FALSE, FALSE);
	}

	*pDomP << Dom;
}

// 교각 그리기
void CDrawVertSection::DrawVertSectionPier(CDomyun *pDomP)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	CHgBaseDrawStd baseDraw;

	CTwinVectorArray tvArr;

	long nCountJijum	= pBri->GetCountJijum();

	for(long nJ = 0; nJ < nCountJijum; nJ++)
	{
		if(nJ == pBri->GetJijumConnectRibAndSlab()) continue;

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_CONC);
		pBri->GetTvVertSectionPier(tvArr, nJ);
		baseDraw.DrawTvArray(&Dom, tvArr, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());
		
		if(tvArr.GetSize() > 0)
		{
			// 컷팅라인
			CDRect rect	= tvArr.GetRect();
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_CUTL);
			Dom.CutLightningAnyDir(CDPoint(rect.left, rect.bottom), CDPoint(rect.right, rect.bottom), 500);

			// 슈
			DrawSimpleShoe(&Dom, CDPoint((rect.left+rect.right)/2, rect.top+400));
		}
	}

	*pDomP << Dom;
}

// xy : 슈 상부 중심
void CDrawVertSection::DrawSimpleShoe(CDomyun *pDomP, CDPoint xy)
{
	CDomyun Dom(pDomP);

	const double dWU	= 150;
	const double dHU	= 150;
	const double dWL	= 250;
	const double dHL	= 250;

	Dom.SetLineColor(2);
	Dom.Rectangle(xy.x-dWU, xy.y, xy.x+dWU, xy.y-dWU);
	Dom.Rectangle(xy.x-dWL, xy.y-dHU, xy.x+dWL, xy.y-dHU-dHL);

	*pDomP << Dom;
}

// 모멘트도/전단력도 그리기
// 다이어그램의 기준점 : x : 벽체가 하나라도 있다면 벽체의 중간, 없으면  슬래브 시작점
//                       y : 모든 기초중 가장 낮은 곳에 있는 기초의 상단
// BOOL bWhenDiagramIsNoneDrawRect : 다이어그램이 안그려 졌을때 그냥 사각형 그릴지?
CDRect CDrawVertSection::DrawDiagramBMD_SFD(CDomyun *pDomP, BOOL bBMD, BOOL bSFD, BOOL bAXIAL, BOOL bWhenDiagramIsNoneDrawRect, BOOL bInput)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return CDRect(0, 0, 0, 0);
	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	CDRect rectOrg(0, 0, 0, 0);
	if(bBMD == FALSE && bSFD == FALSE && bAXIAL == FALSE) return rectOrg;

	pDomP->SetCalcExtRect();
	rectOrg	= pDomP->GetExtRect();

	// 기준점 찾기 ////////////////////////////
	CDPoint xyMove	= CDPoint(0, 0);

	if(pBri->IsOutWall(TRUE))
	{
		xyMove.x	= pBri->GetStationOnJijum(0, 0, FALSE) - pBri->m_dWS/2;
	}
	else
	{
		long nCountInWall	= pBri->GetCountInWall();

		if(pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB || nCountInWall == 0)
		{
			xyMove.x	= pBri->GetStationOnJijum(0, 0, FALSE);
		}
		else
		{
			xyMove.x	= pBri->GetStationOnJijum(1, 0, FALSE);
		}
	}

	if(pBri->GetCountFooting() > 0)
	{
		if(pBri->IsBoxType())
		{
			CFootingApp *pFooting	= pBri->GetFooting(0);
			xyMove.y	= pFooting->m_dEL + pBri->m_dTS_Lower / 2.0;
		}
		else
		{
			for(long footing = 0; footing < pBri->GetCountFooting(); footing++)
			{
				CFootingApp *pFooting	= pBri->GetFooting(footing);
				if(pFooting)
				{
					if(footing == 0)
						xyMove.y	= pFooting->m_dEL - pFooting->m_dELDiff;
					else
						xyMove.y	= min(pFooting->m_dEL - pFooting->m_dELDiff, xyMove.y);
				}
			}
		}
	}
	else
	{
		xyMove.y	= pBri->GetXySectionSttEnd(TRUE, 0, FALSE).y;
	}
	//////////////////////////////////////////
	
	double dScaleUnit = bLsd ? 1.0 : 1000;
	double dScale	= pDomP->GetScaleDim();
	CFEMManage *pFemNormal = &pBri->m_FemNormal;
	if(!pFemNormal->IsOutputLoaded()) 
	{
		bBMD	= FALSE;
		bSFD	= FALSE;
		bAXIAL	= FALSE;
	}

	CForceDiagram ForceDiagram(pFemNormal);
	ForceDiagram.SetScaleDim(bLsd ? dScale : dScale/1000.0);

	long nIdx = pFemNormal->GetLoadCaseByString(bLsd? (pBri->m_bEtaCombinEach ? "STTOP MAX" : "STGEN MAX") : "ENVULT MAX");	
	CDRect rect(0, 0, 0, 0);

	if(bBMD)
	{
		ForceDiagram.DrawDiagramForce(nIdx, ELE_MOMENT2, FALSE, m_pStd->m_pARoadOptionStd, bLsd? 1000 : 1);
		ForceDiagram.RedrawByScale(bInput ? dScaleUnit : dScaleUnit*(50/dScale));
		ForceDiagram.Move(xyMove);
		ForceDiagram.SetCalcExtRect();
		rect	= ForceDiagram.GetExtRect();

		if(bInput)
		{
			double dMoveX	= rectOrg.CenterPoint().x - rect.CenterPoint().x;
			ForceDiagram.Move(CDPoint(dMoveX, 0));
			rect	= ForceDiagram.GetExtRect();
		}
		*pDomP << ForceDiagram;
	}

	if(bSFD)
	{
		ForceDiagram.DrawDiagramForce(nIdx, ELE_SHEAR_2, FALSE, m_pStd->m_pARoadOptionStd, bLsd? 1000 : 1);	
		ForceDiagram.RedrawByScale(bInput ? dScaleUnit : dScaleUnit*(50/dScale));
		ForceDiagram.Move(xyMove);
		ForceDiagram.SetCalcExtRect();
		rect	= ForceDiagram.GetExtRect();
		if(bInput)
		{
			double dMoveX	= rectOrg.CenterPoint().x - rect.CenterPoint().x;
			ForceDiagram.Move(CDPoint(dMoveX, 0));
		}
		
		if(bBMD)
			rect	= m_pStd->MergeRectAndRect(rect, ForceDiagram.GetExtRect());
		else
			rect	= ForceDiagram.GetExtRect();



		*pDomP << ForceDiagram;
	}
	
	if(bAXIAL)
	{
		ForceDiagram.DrawDiagramForce(nIdx, ELE_AXFORCE, FALSE, m_pStd->m_pARoadOptionStd, bLsd? 1000 : 1);	
		ForceDiagram.RedrawByScale(bInput ? dScaleUnit : dScaleUnit*(50/dScale));
		ForceDiagram.Move(xyMove);
		ForceDiagram.SetCalcExtRect();
		rect	= ForceDiagram.GetExtRect();
		if(bInput)
		{
			double dMoveX	= rectOrg.CenterPoint().x - rect.CenterPoint().x;
			ForceDiagram.Move(CDPoint(dMoveX, 0));
		}
		
		if(bBMD || bSFD)
			rect	= m_pStd->MergeRectAndRect(rect, ForceDiagram.GetExtRect());
		else
			rect	= ForceDiagram.GetExtRect();

		*pDomP << ForceDiagram;
	}

	
	if(bWhenDiagramIsNoneDrawRect && Compare(rect.Height(), 0.0, "=") && Compare(rect.Width(), 0.0, "="))
	{
		CDomyun Dom(pDomP);
		rect.left	= -5000;
		rect.right	= 5000;
		rect.top	= 2000;
		rect.bottom	= -2000;
		//rect	= pBri->m_tvArrVertSection.GetRect();
		Dom.Rectangle(rect);
		Dom.SetCalcExtRect();
		rect	= Dom.GetExtRect();
		*pDomP << Dom;
	}



	
	return rect;
}


// 상부 슬래브 두께 치수
// 지간 중앙마다 하나씩 기입
void CDrawVertSection::DimVertSectionUpperSlabThick(CDomyun *pDomP, BOOL bDimPave, long nLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	
	CDomyun Dom(pDomP);
	
	m_pStd->ApplyDimSetting(pDomP, &Dom);
	
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetDirection("RIGHT");
	
	CTwinVector tv;

	// 일반도는 직거리로 안 그리므로 항상 FALSE
	BOOL bIsVertDir	= FALSE;

// 	long nJStt	= 0;
// 	long nJEnd	= 0;
// 	double dStaMid	= 0;
	CDPoint xy(0, 0);
	double dObq	= 30;
	Dom.SetDimObq(dObq);
	Dom.SetDimExo(0);
	for(long jigan = 0; jigan < pBri->m_nQtyJigan; jigan++)
	{
		long nJStt	= jigan;
		long nJEnd	= nJStt+1;
		double dStaMid	= (pBri->GetStationOnJijum(nJStt, nLeft, bIsVertDir) + pBri->GetStationOnJijum(nJEnd, nLeft, bIsVertDir))/2;
		xy	= pBri->GetXyMatchLineAndSlabUpper(CDPoint(dStaMid, 0), CDPoint(0, 1), nLeft, 0, bIsVertDir);

		// 슬래브 두께
		Dom.DimMoveTo(xy.x, xy.y - pBri->m_dTS, 0);
		Dom.DimLineTo(pBri->m_dTS, 0, COMMA(pBri->m_dTS));

		// 콘크리트 두께
		if(bDimPave)
		{
			if(pBri->GetThickPave(FALSE) > 0)
				Dom.DimLineToOver(pBri->GetThickPave(FALSE), 0, COMMA(pBri->GetThickPave(FALSE)));
		}
	}
	
	*pDomP << Dom;
}

// 기본적인 치수 기입한뒤 예외사항에 대한 치수 기입해줌
// 1, 기초하면 라운드/정착장 치수 기입
void CDrawVertSection::DimRebarMainAssm_VertSection_FootingLower(CDomyun *pDomP, long nDimDan, long nCycle, long nDrawLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetDimExo(3);
	Dom.SetDimVLen(5);
	
	for(long nJ = 0; nJ < pBri->GetCountJijum(); nJ++)
	{
		CRebarInfoCycle *pRC	= NULL;
		long left = 0; for(left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT;
			if(nJ == 0 )
			{
				BOOL bInner	= !bLeft;
				if(!pBri->IsOutWall(TRUE)) continue;
				pRC	= pBri->GetRebarInfoCycleOutWall(nCycle, 0, TRUE, bInner, 0);
				if(!pRC) continue;
			}
			else if(nJ == pBri->GetCountJijum()-1)
			{
				BOOL bInner	= bLeft;
				if(!pBri->IsOutWall(FALSE)) continue;
				pRC	= pBri->GetRebarInfoCycleOutWall(nCycle, 0, FALSE, bInner, 0);
				if(!pRC) continue;
			}
			else
			{
				CWallApp *pWall	= pBri->GetInWall(nJ-1);
				if(!pWall) continue;
				if(!pWall->m_bIs) continue;
				pRC	= pWall->GetRebarInfoCycle(nCycle, 0, bLeft, 0);
				if(!pRC) continue;
			}

			if(pRC->m_nPosStt == pRC->m_nPosEnd) continue;

			CDPoint xyStt	= pRC->m_xyStt;
			CDPoint xyMid1	= pRC->m_xyMid1;
			CDPoint xyMid2	= pRC->m_xyMid2;
			CDPoint xyEnd	= pRC->m_xyEnd;
			CDPoint xyCen	= pRC->m_xyCen;
			double dRMid	= pRC->m_R_Mid;

			double dGap	= 50;
			xyStt.y		+= dGap;
			xyMid1.y	+= dGap;
			xyMid2.y	+= dGap;
			xyCen.y		+= dGap;

			if(nJ < pBri->GetCountJijum()-1)
			{
				if(0)//pRC->m_xyStt.x < pRC->m_xyMid1.x)
				{
					if(dRMid > 0)
					{
						Dom.SetDirection("BOTTOM");
						Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
						Dom.DimLineTo(xyMid1.x-xyStt.x, 0, COMMA(xyMid1.x-xyStt.x));
						Dom.SetDirection("TOP");
						Dom.DimArcMoveTo(xyCen.x, xyCen.y, dRMid, xyMid1.x, xyMid1.y, 0);
						Dom.DimArcLineToExtend(xyMid2.x, xyMid2.y, 0, COMMA(pRC->GetLengthMid()), "R="+COMMA(dRMid));
					}
					else
					{

					}
				}
				else
				{
					Dom.SetDirection("BOTTOM");
					Dom.DimMoveTo(xyMid1.x, xyMid1.y, 0, "");
					Dom.DimLineTo(xyStt.x-xyMid1.x, 0, COMMA(fabs(xyStt.x-xyMid1.x)));
				}
			}
			else
			{
				if(1)//xyStt.x < xyMid1.x)
				{
					Dom.SetDirection("BOTTOM");
					Dom.DimMoveTo(xyStt.x, xyStt.y, 0, "");
					Dom.DimLineTo(xyMid1.x-xyStt.x, 0, COMMA(fabs(xyMid1.x-xyStt.x)));
				}
				else
				{
					if(dRMid > 0)
					{
						Dom.SetDirection("TOP");
						Dom.DimArcMoveTo(xyCen.x, xyCen.y, dRMid, xyMid2.x, xyMid2.y, 0);
						Dom.DimArcLineToExtend(xyMid1.x, xyMid1.y, 0, COMMA(pRC->GetLengthMid()), "R="+COMMA(dRMid));
						Dom.SetDirection("BOTTOM");
						Dom.DimMoveTo(xyMid1.x, xyMid1.y, 0);
						Dom.DimLineTo(xyStt.x-xyMid1.x, 0, COMMA(xyStt.x-xyMid1.x));
					}
					else
					{

					}
				}
			}
		}
	}

	*pDomP << Dom;
}

// 상부 슬래브 슬래브 측면 주철근 조립도 치수
void CDrawVertSection::DimRebarMainAssm_VertSection_UpperSlabSide(CDomyun *pDomP, long nDimDan, long nCycle, long nDrawLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	CHgBaseDrawStd baseDraw;

	if(!pBri->IsOutWall(TRUE) && (nDrawLeft == -1 || nDrawLeft == 0))
	{
		CDoubleArray dArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		CDPoint xyStt(0, 0);
		CString sDir	= _T("LEFT");

		CTwinVector tvSide	= pBri->m_tvArrVertSection.GetTvByInfo("시점슬래브측면");
		xyStt	= tvSide.GetXyBottom();

		dArrDim.Add(pBri->m_dCoverLower_UpperSlab[0]);
		dArrDim.Add(pBri->m_dTS + pBri->m_dHHS - (pBri->m_dCoverLower_UpperSlab[0] + pBri->m_dCoverUpper_UpperSlab[0]));
		dArrDim.Add(pBri->m_dCoverUpper_UpperSlab[0]);

		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, FALSE, FALSE);
	}

	if(!pBri->IsOutWall(FALSE) && (nDrawLeft == 1 || nDrawLeft == 0))
	{
		CDoubleArray dArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		CDPoint xyStt(0, 0);
		CString sDir	= _T("RIGHT");

		CTwinVector tvSide	= pBri->m_tvArrVertSection.GetTvByInfo("종점슬래브측면");
		xyStt	= tvSide.GetXyBottom();
		
		dArrDim.Add(pBri->m_dCoverLower_UpperSlab[0]);
		dArrDim.Add(pBri->m_dTS + pBri->m_dHHE - (pBri->m_dCoverLower_UpperSlab[0] + pBri->m_dCoverUpper_UpperSlab[0]));
		dArrDim.Add(pBri->m_dCoverUpper_UpperSlab[0]);

		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, FALSE, FALSE);
	}

	*pDomP << Dom;
}

// 지점 표시용 삼각형 그리기
// 거더는 항상 사거리로 입력 받으므로 사거리로 그림.
void CDrawVertSection::DrawJijumSign(CDomyun *pDomP, long nJ)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	
	CDomyun Dom(pDomP);
	const double dL	= 10;
	long left = 0; for(left = 0; left < 2; left++)
	{
		if(nJ == 0)
		{
			if(left == iLEFT) continue;
		}
		else if(nJ == pBri->GetCountJijum()-1)
		{
			if(left == iRIGHT) continue;
		}
		else 
		{
			if(!pBri->m_bExpJoint[nJ] && left==iRIGHT) continue;
		}

		CDPoint xy	= pBri->GetXyVertSectionShoe(nJ, left==iLEFT);
		double dH	= pBri->m_dTS;
		if(nJ == 0)
		{
			dH += pBri->m_dHHS;
			if(pBri->IsOutWall(TRUE)) return;
		}
		else if(nJ == pBri->GetCountJijum()-1)
		{
			dH += pBri->m_dHHE;
			if(pBri->IsOutWall(FALSE)) return;
		}
		else
		{
			dH += pBri->GetHeightHunch(nJ-1);
			CWallApp *pWall	= pBri->GetInWall(nJ-1);
			if(pWall)
			{
				if(pWall->m_bIs) return;
			}
		}

		Dom.SetLineColor(2);
		Dom.Triangle(xy.x, xy.y-dH, dL, 0);
	}

	*pDomP << Dom;	
}

// 피복 입력용 
void CDrawVertSection::DimForCoverInput(CDomyun *pDomP, CString sArea)
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
	long nDimDan	= 0;
	CDPoint xyStt	= CDPoint(0, 0);
	
	// 슬래브 상면
	if(sArea == "슬래브 상면" || sArea == "슬래브 하면")
	{
		Dom.SetDimObq(30);
		Dom.SetDimExo(0);
		sDir	= "RIGHT";
		dArr.Add(pBri->m_dCoverLower_UpperSlab[0]);
		sArrText.Add("슬래브 하면");

		dArr.Add(pBri->m_dTS-pBri->m_dCoverLower_UpperSlab[0]-pBri->m_dCoverUpper_UpperSlab[0]);
		sArrText.Add("슬래브 두께");

		dArr.Add(pBri->m_dCoverUpper_UpperSlab[0]);
		sArrText.Add("슬래브 상면");

		double dStaMid	= (pBri->GetStationOnJijum(0) + pBri->GetStationOnJijum(1))/2;
		xyStt	= pBri->GetXyMatchLineAndSlabUpper(CDPoint(dStaMid, 0), CDPoint(0, 1), 0, pBri->m_dTS, FALSE);

		baseDraw.AutoDimCont(&Dom, xyStt, dArr, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
	}
	else if(sArea == "하부슬래브 상면" || sArea == "하부슬래브 하면")
	{
		Dom.SetDimObq(30);
		Dom.SetDimExo(0);
		sDir	= "RIGHT";
		dArr.Add(pBri->m_dCoverLower_LowerSlab[0]);
		sArrText.Add("슬래브 하면");

		dArr.Add(pBri->m_dTS_Lower-pBri->m_dCoverLower_LowerSlab[0]-pBri->m_dCoverUpper_LowerSlab[0]);
		sArrText.Add("슬래브 두께");

		dArr.Add(pBri->m_dCoverUpper_LowerSlab[0]);
		sArrText.Add("슬래브 상면");

		double dStaMid	= (pBri->GetStationOnJijum(0) + pBri->GetStationOnJijum(1))/2;
		xyStt	= pBri->GetXyMatchLineAndLowerSlabLower(CDPoint(dStaMid, 0), CDPoint(0, 1), 0, 0, FALSE);

		baseDraw.AutoDimCont(&Dom, xyStt, dArr, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
	}
	else if(sArea == "슬래브 측면")
	{
		sDir	= "TOP";
		CTwinVector tvStt	= pBri->m_tvArrVertSection.GetTvByInfo(!pBri->IsOutWall(TRUE) ? "시점슬래브측면" : "시점브라켓상부측면");
		CTwinVector tvEnd	= pBri->m_tvArrVertSection.GetTvByInfo(!pBri->IsOutWall(FALSE) ? "종점슬래브측면" : "종점브라켓상부측면");

		dArr.Add(pBri->GetCoverSide_UpperSlab(0));
		sArrText.Add("슬래브 측면");

		dArr.Add((tvEnd.m_v1.x - tvStt.m_v2.x) - pBri->GetCoverSide_UpperSlab(0) * 2);
		sArrText.Add("0");

		dArr.Add(pBri->GetCoverSide_UpperSlab(0));
		sArrText.Add("슬래브 측면");

		xyStt	= tvStt.GetXyTop();
		baseDraw.AutoDimCont(&Dom, xyStt, dArr, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
	}

	else if(sArea == "벽체 외면" || sArea == "벽체 내면")
	{
		if(!pBri->IsOutWall(TRUE)) return;
		sDir	= "BOTTOM";
		CTwinVector tvOutter	= pBri->m_tvArrVertSection.GetTvByInfo("시점벽체좌측면");

		dArr.Add(pBri->m_dCoverWallOut[0]);
		sArrText.Add("벽체 외면");

		dArr.Add(pBri->m_dWS - pBri->m_dCoverWallOut[0] - pBri->m_dCoverWallIn[0]);
		sArrText.Add("0");

		dArr.Add(pBri->m_dCoverWallIn[0]);
		sArrText.Add("벽체 내면");

		xyStt	= tvOutter.GetXyMid();

		baseDraw.AutoDimCont(&Dom, xyStt, dArr, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
	}

	else if(sArea == "슬래브 측면(시종점)")
	{
		CTwinVector tvSide;
		sDir	= "TOP";
		if(pBri->m_tvArrVertSection.GetTvByInfo("시점슬래브측면", tvSide))
		{
			dArr.RemoveAll();
			sArrText.RemoveAll();

			dArr.Add(pBri->m_dCoverWallOut[0]);
			sArrText.Add("슬래브 측면(시종점)");

			xyStt	= tvSide.GetXyTop();
			baseDraw.AutoDimCont(&Dom, xyStt, dArr, sArrText, sArrUnderText, nDimDan, sDir, 0, FALSE, FALSE, FALSE);
		}

		if(pBri->m_tvArrVertSection.GetTvByInfo("종점슬래브측면", tvSide))
		{
			dArr.RemoveAll();
			sArrText.RemoveAll();

			dArr.Add(pBri->m_dCoverWallOut[0]);
			sArrText.Add("슬래브 측면(시종점)");

			xyStt	= tvSide.GetXyTop();
			xyStt.x -= pBri->m_dCoverWallOut[0];
			baseDraw.AutoDimCont(&Dom, xyStt, dArr, sArrText, sArrUnderText, nDimDan, sDir, 0, FALSE, FALSE, FALSE);
		}
	}

	else if(sArea == "내측 벽체")
	{
		sDir	= "BOTTOM";
		CWallApp *pWall	= pBri->GetInWall(0);
		if(pWall)
		{
			if(pWall->m_bIs)
			{
				CTwinVector tvLeft	= pWall->m_tvArrFront.GetTvByInfo("벽체좌측면");

				dArr.Add(pBri->m_dCoverWallIn[0]);
				sArrText.Add("내측 벽체");

				dArr.Add(pWall->m_dW - (pBri->m_dCoverWallIn[0] * 2));
				sArrText.Add("0");

				dArr.Add(pBri->m_dCoverWallIn[0]);
				sArrText.Add("내측 벽체");

				xyStt	= tvLeft.GetXyMid();
				baseDraw.AutoDimCont(&Dom, xyStt, dArr, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
			}
		}
	}

	else if(sArea == "기초 상면")
	{
		sDir	= "LEFT";
		CFootingApp *pFooting	= pBri->GetFooting(0);
		if(pFooting)
		{
			if(pFooting->m_bIs)
			{
				CTwinVector tvLower	= pFooting->m_tvArrFront.GetTvByInfo("기초하면");
				
				dArr.Add(pFooting->m_dCoverLower[0]);
				sArrText.Add("0");

				dArr.Add(pFooting->GetHeight());
				sArrText.Add("0");

				dArr.Add(pFooting->m_dCoverUpper[0]);
				sArrText.Add("기초 상면");

				xyStt	= tvLower.GetXyLeft();
				baseDraw.AutoDimCont(&Dom, xyStt, dArr, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
			}
		}
	}

	else if(sArea == "기초 하면(직접기초)")
	{
		sDir	= "LEFT";
		CFootingApp *pFooting	= NULL;
		long nCountFooting	= pBri->GetCountFooting();
		for(long footing = 0; footing < nCountFooting; footing++)
		{
			CFootingApp *pFootingTmp	= pBri->GetFooting(footing);
			if(!pFootingTmp) continue;
			if(!pFootingTmp->m_bIs) continue;

			if(pFootingTmp->m_exFooting.m_nType == EXFOOTING_TYPE_LEAN)
			{
				pFooting	= pFootingTmp;
				break;
			}
		}

		if(pFooting)
		{
			CTwinVector tvLower		= pFooting->m_tvArrFront.GetTvByInfo("기초하면");
			
			dArr.Add(pBri->m_dCoverLower_Footing_Lean[0]);
			sArrText.Add("기초 하면(직접기초)");

			dArr.Add(pFooting->GetHeight() - pBri->m_dCoverLower_Footing_Lean[0] - pFooting->m_dCoverUpper[0]);
			sArrText.Add("0");

			dArr.Add(pFooting->m_dCoverUpper[0]);
			sArrText.Add("0");

			xyStt	= tvLower.GetXyLeft();
			baseDraw.AutoDimCont(&Dom, xyStt, dArr, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
		}
	}

	else if(sArea == "기초 하면(말뚝기초)")
	{
		sDir	= "LEFT";
		CFootingApp *pFooting	= NULL;
		long nCountFooting	= pBri->GetCountFooting();
		for(long footing = 0; footing < nCountFooting; footing++)
		{
			CFootingApp *pFootingTmp	= pBri->GetFooting(footing);
			if(!pFootingTmp) continue;
			if(!pFootingTmp->m_bIs) continue;

			if(pFootingTmp->m_exFooting.m_nType == EXFOOTING_TYPE_PILE)
			{
				pFooting	= pFootingTmp;
				break;
			}
		}

		if(pFooting)
		{
			CTwinVector tvLower		= pFooting->m_tvArrFront.GetTvByInfo("기초하면");
			
			dArr.Add(pBri->m_dCoverLower_Footing_Pile[0]);
			sArrText.Add("기초 하면(말뚝기초)");

			dArr.Add(pFooting->GetHeight() - pBri->m_dCoverLower_Footing_Pile[0] - pFooting->m_dCoverUpper[0]);
			sArrText.Add("0");

			dArr.Add(pFooting->m_dCoverUpper[0]);
			sArrText.Add("0");

			xyStt	= tvLower.GetXyLeft();
			baseDraw.AutoDimCont(&Dom, xyStt, dArr, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
		}
	}

	*pDomP << Dom;
}

void CDrawVertSection::DimVertSectionUpperOnlyJijum(CDomyun *pDomP, long nDimDan, long nLeft)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDomP, &Dom);
	
	if(nDimDan < 0) nDimDan = 0;
	CTwinVector tv;
	CTwinVectorArray *pTvArr = &pBri->m_tvArrVertSection;
	
	CDoubleArray dArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("TOP");
	CDPoint xyStt(0, 0);

	double dStaCur	= pBri->GetStationOnJijum(0, nLeft),
		   dStaLast	= pBri->GetStationOnJijum(0, nLeft);
	double dStaCurRightAng	= pBri->GetStationOnJijum(0, nLeft, TRUE),	// 직거리 계산용
		   dStaLastRightAng	= pBri->GetStationOnJijum(0, nLeft, TRUE);
	double dLen		= 0,
		   dLenRightAng	= 0;
	CDPoint vAng	= CDPoint(0, 1);

	xyStt.x	= dStaCur;
	xyStt.y	= pTvArr->GetRect().top;

	long nCountJijum	= pBri->GetCountJijum();
	for(long nJ = 0; nJ < nCountJijum; nJ++)
	{
		CJijum *pJ	= pBri->GetJijum(nJ);
		vAng	= pBri->GetAngleJijum(nJ);

		// 지점 0일땐 신축이음만 검사.
		if(nJ == 0)
		{
			if(pBri->m_bExpJoint[nJ])
			{
				dStaCur	+= pJ->m_dLengthUgan[1];
				dStaCurRightAng += fabs(pJ->m_dLengthUgan[1]*vAng.y);

				dLen	= dStaCur - dStaLast;
				dLenRightAng	= dStaCurRightAng - dStaLastRightAng;

				if(!Compare(dLen, 0.0, "="))
				{
					dArrDim.Add(dLen);
					sArrText.Add(COMMA(dLen));
					sArrUnderText.Add(COMMA2(dLenRightAng));
				}
				dStaLast	= dStaCur;
				dStaLastRightAng	= dStaCurRightAng;
			}

			continue;
		}
		else if(nJ == nCountJijum-1)
		{
			dStaCur	= pBri->GetStationOnJijum(nJ, nLeft);
			dStaCurRightAng	= pBri->GetStationOnJijum(nJ, nLeft, TRUE);

			if(pBri->m_bExpJoint[nJ])
			{
				// 신축이음 시작
				dStaCur	-= pJ->m_dLengthUgan[0];
				dStaCurRightAng	-= fabs(pJ->m_dLengthUgan[0]*vAng.y);

				dLen	= dStaCur - dStaLast;
				dLenRightAng	= dStaCurRightAng - dStaLastRightAng;

				if(!Compare(dLen, 0.0, "="))
				{
					dArrDim.Add(dLen);
					sArrText.Add(COMMA(dLen));
					sArrUnderText.Add(COMMA2(dLenRightAng));
				}
				dStaLast	= dStaCur;
				dStaLastRightAng	= dStaCurRightAng;

				// 마지막 지점
				dStaCur += pJ->m_dLengthUgan[0];
				dStaCurRightAng += fabs(pJ->m_dLengthUgan[0]*vAng.y);

				dLen	= dStaCur - dStaLast;
				dLenRightAng	= dStaCurRightAng - dStaLastRightAng;

				if(!Compare(dLen, 0.0, "="))
				{
					dArrDim.Add(dLen);
					sArrText.Add(COMMA(dLen));
					sArrUnderText.Add(COMMA2(dLenRightAng));
				}
				dStaLast	= dStaCur;
				dStaLastRightAng	= dStaCurRightAng;
			}
			else
			{
				dLen	= dStaCur - dStaLast;
				dLenRightAng	= dStaCurRightAng - dStaLastRightAng;

				if(!Compare(dLen, 0.0, "="))
				{
					dArrDim.Add(dLen);
					sArrText.Add(COMMA(dLen));
					sArrUnderText.Add(COMMA2(dLenRightAng));
				}
				dStaLast	= dStaCur;
				dStaLastRightAng	= dStaCurRightAng;
			}
		}
		else
		{
			dStaCur	= pBri->GetStationOnJijum(nJ, nLeft);
			dStaCurRightAng	= pBri->GetStationOnJijum(nJ, nLeft, TRUE);
			
			if(pBri->m_bExpJoint[nJ])
			{
				// 신축이음 시작
				dStaCur -= pJ->m_dLengthUgan[0];
				dStaCurRightAng -= fabs(pJ->m_dLengthUgan[0]*vAng.y);

				dLen	= dStaCur - dStaLast;
				dLenRightAng	= dStaCurRightAng - dStaLastRightAng;

				if(!Compare(dLen, 0.0, "="))
				{
					dArrDim.Add(dLen);
					sArrText.Add(COMMA(dLen));
					sArrUnderText.Add(COMMA2(dLenRightAng));
				}
				dStaLast	= dStaCur;
				dStaLastRightAng	= dStaCurRightAng;

				// 신축이음 끝
				dStaCur	+= (pJ->m_dLengthUgan[0] + pJ->m_dLengthUgan[1]);
				dStaCurRightAng	+= fabs((pJ->m_dLengthUgan[0] + pJ->m_dLengthUgan[1])*vAng.y);

				dLen	= dStaCur - dStaLast;
				dLenRightAng	= dStaCurRightAng - dStaLastRightAng;

				if(!Compare(dLen, 0.0, "="))
				{
					dArrDim.Add(dLen);
					sArrText.Add(COMMA(dLen));
					sArrUnderText.Add(COMMA2(dLenRightAng));
				}
				dStaLast	= dStaCur;
				dStaLastRightAng	= dStaCurRightAng;
			}
			else
			{
				dLen	= dStaCur - dStaLast;
				dLenRightAng	= dStaCurRightAng - dStaLastRightAng;

				if(!Compare(dLen, 0.0, "="))
				{
					dArrDim.Add(dLen);
					sArrText.Add(COMMA(dLen));
					sArrUnderText.Add(COMMA2(dLenRightAng));
				}
				dStaLast	= dStaCur;
				dStaLastRightAng	= dStaCurRightAng;
			}
		}
	}

	m_pStd->ModifyInfoAutoDimConc(sArrText, sArrUnderText, FALSE);

	CHgBaseDrawStd baseDraw;
	baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
	nDimDan++;

	// 전체 치수
	if(dArrDim.GetSize() > 0)
	{
		double dStaStt			= pBri->GetStationOnJijum(0, nLeft, FALSE);
		double dStaEnd			= pBri->GetStationOnJijum(pBri->m_nQtyJigan, nLeft, FALSE);
		double dStaSttRightAng	= pBri->GetStationOnJijum(0, nLeft, TRUE);
		double dStaEndRightAng	= pBri->GetStationOnJijum(pBri->m_nQtyJigan, nLeft, TRUE);

		double dLen				= dStaEnd - dStaStt;
		double dLenRightAng		= dStaEndRightAng - dStaSttRightAng;

		Dom.DimMoveTo(dStaStt, xyStt.y, nDimDan);
		if(Compare(dLen, dLenRightAng, "="))
			Dom.DimLineTo(dLen, nDimDan, COMMA(dLen));
		else
			Dom.DimLineToExtend(dLen, nDimDan, COMMA(dLen), COMMA2(dLenRightAng));
	}

	*pDomP << Dom;
}

// 종단면 상단에 slope 기입
// void CDrawVertSection::DimTextVertSectionSlope(CDomyun *pDomP)
// {
// 	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
// 	if(!pBri) return;
// 	CDomyun Dom(pDomP);
// 	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
// 
// 	// 종단면 중간에 기입
// 
// 	*pDomP << Dom;
// }

// 종단면에 대한 중심선 그림
// 경사교의 경우에만 그린다.
void CDrawVertSection::DrawVertSectionCenterLine(CDomyun *pDomP, BOOL bInput, BOOL bApplyAngle, BOOL bDimAngle, long nJForInput)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;
	
	CDomyun Dom(pDomP);
	CConcSymbol sym(m_pStd->m_pARoadOptionStd);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_CENT);
	CHgBaseDrawStd baseDraw;

	if(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP || pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2)
	{
		// 상부슬래브
		CTwinVectorArray tvArr;
		CTwinVector tvCen;
		tvCen.m_v1		= pBri->GetXySectionSttEnd(TRUE, 0);
		tvCen.m_v2		= pBri->GetXySectionSttEnd(FALSE, 0);
		if(pBri->IsOutWall(TRUE))
		{
			CTwinVector tvLeft	= pBri->m_tvArrVertSection.GetTvByInfo("시점벽체좌측면");
			CTwinVector tvRight	= pBri->m_tvArrVertSection.GetTvByInfo("시점벽체우측면");
			tvCen.m_v1	= pBri->GetXySlabUpperByOffsetFrStation((tvLeft.m_v1.x+tvRight.m_v2.x)/2, 0, 0);
		}

		if(pBri->IsOutWall(FALSE))
		{
			CTwinVector tvLeft	= pBri->m_tvArrVertSection.GetTvByInfo("종점벽체좌측면");
			CTwinVector tvRight	= pBri->m_tvArrVertSection.GetTvByInfo("종점벽체우측면");
			tvCen.m_v2	= pBri->GetXySlabUpperByOffsetFrStation((tvLeft.m_v1.x+tvRight.m_v2.x)/2, 0, 0);
		}

		tvCen.m_v1.y	-= pBri->m_dTS/2;
		tvCen.m_v2.y	-= pBri->m_dTS/2;
		tvCen.m_v2.z	= 2;
		tvArr.Add(tvCen);
		baseDraw.DrawTvArray(&Dom, tvArr, FALSE, FALSE, 0);

		// 외측벽체
		long stt = 0; for(stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			if(!pBri->IsOutWall(bStt)) continue;
			CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;

			CTwinVector tvLower	= pFooting->m_tvArrFront.GetTvByInfo("기초하면");
			CTwinVector tvLeftCen;
			tvLeftCen.m_v1	= tvLeftCen.m_v2 = (bStt ? tvCen.m_v1 : tvCen.m_v2);
			tvLeftCen.m_v1.z	= tvLeftCen.m_v2.z	= 0;
			GetXyMatchLineAndLine(tvLeftCen.m_v2, CDPoint(0, -1), tvLower.m_v1, tvLower.GetXyDir(), tvLeftCen.m_v1);
			baseDraw.DrawTv(&Dom, tvLeftCen);
		}

		tvArr.RemoveAll();
		long nCountWall	= pBri->GetCountInWall();
		for(long wall = 0; wall < nCountWall; wall++)
		{
			CWallApp *pWall	= pBri->GetInWall(wall);
			if(!pWall) continue;
			BOOL bInputCurJijum	= wall+1 == nJForInput || nJForInput == -1 ? bInput : FALSE;

			CTwinVector tv	= pWall->m_tvArrFront.GetTvByInfo("벽체하면좌측");
			tvCen.m_v1	= tv.m_v1;
			tvCen.m_v2	= pBri->GetXyMatchLineAndSlabUpper(tvCen.m_v1, ToDPointFrDegree(pWall->GetAngleWallByHor(bApplyAngle)), 0, pBri->m_dTS/2, FALSE);

			if(pWall->m_footing.m_bIs || pWall->m_footing.IsVirtualFooting())
			{
				CTwinVector tvLower	= pWall->m_footing.m_tvArrFront.GetTvByInfo("기초하면");
				GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), tvCen.m_v1, tvCen.GetXyDir(TRUE), tvCen.m_v1);
			}
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_CENT);
			baseDraw.DrawTv(&Dom, tvCen);

			// 각도 치수도 넣자
			if(bDimAngle)
			{
				m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_DIML);
				// 1. 상부 슬래브와의 각도
				double dAngWall		= pWall->GetAngleWallByHor(bApplyAngle);
				CDPoint vAngWall	= ToDPointFrDegree(dAngWall);
				double dAngSlab		= pWall->GetAngleUpperSlab();
				CDPoint vAngSlab	= ToDPointFrDegree(dAngSlab);
				double dR	= Dom.Always(3);

				// 좌측
				CDPoint xyOrg	= tvCen.m_v2;
				double dAng		= (dAngWall + 180) - (dAngSlab + 180);
				double dAngVert	= pWall->m_dAngle-pWall->m_dAngleUpperSlab_Front;

				if(bInputCurJijum)
					sym.DrawAngleNotation(&Dom, xyOrg, dAngSlab+180, dAngWall+180, dR, 0, !bInput, _T("A"), dAng);
				else
					sym.DrawAngleNotation(&Dom, xyOrg, dAngSlab+180, dAngWall+180, dR, 0, !bInput, _T(""), dAngVert);

				// 우
				dAng	= dAngSlab + 360 - (dAngWall + 180);
				xyOrg	= tvCen.m_v2;
				dAngVert	= 180-pWall->m_dAngle+pWall->m_dAngleUpperSlab_Front;

				if(bInputCurJijum)
					sym.DrawAngleNotation(&Dom, xyOrg, dAngWall+180, dAngSlab, dR, !bInput, 0, _T("A"), dAng);
				else
					sym.DrawAngleNotation(&Dom, xyOrg, dAngWall+180, dAngSlab, dR, !bInput, 0, _T(""), dAngVert);


				// 2. 수평면과의 각도
				double dAngFooting	= 0;
				xyOrg	= tvCen.m_v1;
				dAng	= pWall->GetAngleWallByHor(bApplyAngle);
				dAngVert	= pWall->m_dAngle;

				if(bInputCurJijum)
					sym.DrawAngleNotation(&Dom, xyOrg, dAngFooting, dAng, dR, !bInput, 0, _T("A"), dAng);
				else
					sym.DrawAngleNotation(&Dom, xyOrg, dAngFooting, dAng, dR, !bInput, 0, _T(""), dAngVert);
			}
		}
	}
	else if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		CTwinVectorArray tvArrRibCen;
		baseDraw.DrawTvArray(&Dom, pBri->m_tvArrVertSectionRibCen);
	}

	*pDomP << Dom;
}

// 외측벽체 헌치 그리기
void CDrawVertSection::DrawRebarMain_VertSection_OutWall_Hunch(CDomyun *pDomP, long nCycle, BOOL bMarkRebar, long nDrawLeft, CDRect rectLimit, BOOL bInput, BOOL bForStandardSection, long nVertDir)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(nCycle == -1) nCycle = pBri->GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > pBri->GetCountCycleMainRebar()-1) return;
	
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	// 시점측 벽체
	BOOL bStt = TRUE;
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		bStt	= (stt == iSTT);
		CRebarInfoCycle *pRC	= &pBri->m_rebarInfoCycleOutWallHunch[nCycle][stt];

		// RebarInfoCycle 그리기
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
		m_pStd->DrawRebarInfoCycle(&Dom, pRC, nDrawLeft, rectLimit, TRUE);

		// 철근 마크
		if(bMarkRebar)
		{
			*pDomP << Dom;
			// 마크방향(시점기준)
			BOOL bLeft	= bStt ? FALSE : TRUE;
			double dScalePos	= 0.5;

			m_pStd->MarkRebarInfoCycle(&Dom, pRC, bLeft, TRUE, pBri->m_dCoverWallOut[0], 1, nDrawLeft, rectLimit, dScalePos, bInput, &pBri->m_pArrRebarMain);
		}
	}

	*pDomP << Dom;
}

void CDrawVertSection::DrawRebarSupport_VertSection_OutWallHunch(CDomyun *pDomP, BOOL bStt, BOOL bMarkRebar, BOOL bInput, CDRect rectLimit, long nVertDir)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	// 철근 가져와서 그리기
	CHgBaseDrawStd baseDraw;
	CDPointArray xyArr;
	pBri->GetXyRebarSupport_VertSection_OutWallHunch(xyArr, bStt, nVertDir);
	CTwinVector tvHunch;
	if(!pBri->m_tvArrVertSection.GetTvByInfo(bStt ? "시점벽체하부헌치" : "종점벽체하부헌치", tvHunch)) return;
	CDPoint xyDirScale	= tvHunch.GetXyDir().RotateInv90();

	// rectLimit 적용
	if(rectLimit != CDRect(0, 0, 0, 0))
	{
		CTwinVectorArray tvArrLimit;
		CVectorArray vecArrLimit;
		tvArrLimit.SetFromRect(rectLimit);
		tvArrLimit.GetToVectorArray(vecArrLimit);

		CRebarPlacing rb;
		rb.GetXyByVectorArrayArea(xyArr, vecArrLimit, TRUE);
	}

	baseDraw.DrawPointRebar(&Dom, xyArr, xyDirScale);

	long stt	= bStt ? iSTT : iEND;

	// 마킹
	CString sDescription	= _T("");
	sDescription.Format("%s 헌치부 배력철근", bStt ? "시점측벽체" : "종점측벽체");
	CString sMark			= pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescription);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		CRebar *pRb = pBri->GetRebarSupportByDescription(sDescription);
		CPlaceRebarByBlock *pPlace		= &pBri->m_placeByBlock_SupportRebar_OutWall[stt][iINNER][0];
		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
		mark.SetRebar(xyArr);

		// 설정
		mark.m_sMark					= sMark;
		mark.m_sDia						= pBri->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(ePartUpperSlab));
		mark.m_nTypeMarkPointUpAndDown	= bStt ? TYPE_MARKPOINT_UPPER : TYPE_MARKPOINT_LOWER;
		mark.m_nTypeMarkDir				= bStt ? TYPE_MARKDIR_RIGHT : TYPE_MARKDIR_LEFT;
		mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
		mark.m_dPosScaleMarkStt			= 0.5;
		mark.m_dCtc = pBri->GetCTCRebarSupport_OutWall(bStt, TRUE, 0, TRUE);
		mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;

		mark.MarkRebar();
	}

	*pDomP << Dom;
}

// 종경사 기입
void CDrawVertSection::DimVertSectionJongSlope(CDomyun *pDomP)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	double dStaStt	= pBri->m_dStationBridgeStt;
	double dStaEnd	= pBri->m_dStationBridgeStt + pBri->m_dLengthBridge;
	
	DimJongDanBridge(&Dom, dStaStt, dStaEnd, 1, FALSE, TRUE);
	Dom.Move(CDPoint(0, Dom.GetTextHeight()));

	*pDomP << Dom;
}

void CDrawVertSection::DimJongDanBridge(CDomyun *pDomP, double staBridgeStt, double staBridgeEnd, double scaleVertical, BOOL bDimSttEnd, BOOL bDimSlope) const
{
	BOOL bDimEL	= FALSE;
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	
	CLineInfo *pLine	= pBri->GetLineBase();
	CDomyun Dom(pDomP);
	if(scaleVertical == 0) scaleVertical = 1;
	if(staBridgeStt == 0 && staBridgeEnd == 0)
	{
		staBridgeStt = pLine->m_VIPData[0][0];
		staBridgeEnd = pLine->m_VIPData[pLine->m_VIPsu - 1][0];
	}
	double eleBridgeMax = pLine->GetMaxSectionEL(staBridgeStt, staBridgeEnd);   // 교량 구간내 최대 계획고
	double eleBridgeStt = pLine->GetElevationPlanEl(staBridgeStt); // 교량 시점의 계획고
	double eleBridgeEnd = pLine->GetElevationPlanEl(staBridgeEnd); // 교량 종점의 계획고
	// 교량 시점 표기
	CString strSta = GetStationForStringOutNew(toM(staBridgeStt), TRUE);
	CString strEl = GetStringOutElevationNew(toM(eleBridgeStt), TRUE);
	double lenStrSta  = Dom.GetTextWidth(strSta);
	double lenStrEle  = Dom.GetTextWidth(strEl)*1.2;
	double dTextHeight = Dom.GetTextHeight();
	double lenDimHeight = ((lenStrSta / 2) + (dTextHeight * 4));

	CDomyun pDomArrow(pDomP);
	pDomArrow.SetScaleDim(dTextHeight / scaleVertical);

	if(bDimSttEnd)
	{
		Dom.MoveTo(staBridgeStt, eleBridgeStt * scaleVertical);
		Dom.LineTo(staBridgeStt, (eleBridgeMax * scaleVertical) + lenDimHeight);
		
		Dom.SetTextAngle(0);
		Dom.SetTextAlignHorz(TA_LEFT);
		if(bDimEL)
		{
			Dom.TextOut(staBridgeStt-lenStrEle, (eleBridgeMax * scaleVertical) + ((lenStrSta / 2) + dTextHeight * 1.3), strEl);
		}
		*pDomP << Dom;

		if(bDimEL)
			pDomArrow.GiSiArrowExt(staBridgeStt, (eleBridgeMax * scaleVertical) + ((lenStrSta / 2) + dTextHeight), 0, -lenStrEle, 4, "");
		*pDomP << pDomArrow;

		Dom.MoveTo(staBridgeStt, (eleBridgeMax * scaleVertical) + lenDimHeight);
		Dom.LineTo(staBridgeStt - (lenStrSta + dTextHeight), (eleBridgeMax * scaleVertical) + lenDimHeight);
		Dom.SetTextAngle(0);
		Dom.SetTextAlignHorz(TA_CENTER);
		Dom.TextOut(staBridgeStt - ((lenStrSta / 2) + dTextHeight), (eleBridgeMax * scaleVertical) + lenDimHeight + dTextHeight * 0.3, "교량시점");
		Dom.TextOut(staBridgeStt - ((lenStrSta / 2) + dTextHeight), (eleBridgeMax * scaleVertical) + lenDimHeight - dTextHeight * 1.3,  strSta);
		Dom.Rotate(CDPoint(staBridgeStt, (eleBridgeMax * scaleVertical) + lenDimHeight), CDPoint(2,-1));
		*pDomP << Dom;

		// 교량 길이 표기
		double dLength = staBridgeEnd - staBridgeStt; // 교량 전체 길이(유간 포함)
		CString strLengthBridge;
		strLengthBridge.Format("L = %.3lf", toM(dLength));
		Dom.MoveTo(staBridgeStt, (eleBridgeMax * scaleVertical) + ((lenStrSta / 2) + (dTextHeight * 2)));
		Dom.LineTo(staBridgeEnd, (eleBridgeMax * scaleVertical) + ((lenStrSta / 2) + (dTextHeight * 2)));

		Dom.Circle(staBridgeStt, (eleBridgeMax * scaleVertical) + ((lenStrSta / 2) + (dTextHeight * 2)), dTextHeight/4, TRUE);
		Dom.Circle(staBridgeEnd, (eleBridgeMax * scaleVertical) + ((lenStrSta / 2) + (dTextHeight * 2)), dTextHeight/4, TRUE);

		Dom.SetTextAngle(0);
		Dom.SetTextAlignHorz(TA_CENTER);
		Dom.TextOut(staBridgeStt + dLength / 2, (eleBridgeMax * scaleVertical) + ((lenStrSta / 2) + (dTextHeight * 2)) + (dTextHeight * 0.3),   "교량길이");
		Dom.TextOut(staBridgeStt + dLength / 2, (eleBridgeMax * scaleVertical) + ((lenStrSta / 2) + (dTextHeight * 2)) - (dTextHeight * 1.3), strLengthBridge);

		// 교량 종점 표기
		strSta = GetStationForStringOutNew(toM(staBridgeEnd), TRUE);
		strEl = GetStringOutElevationNew(toM(eleBridgeEnd), TRUE);
		Dom.MoveTo(staBridgeEnd, eleBridgeEnd * scaleVertical);
		Dom.LineTo(staBridgeEnd, (eleBridgeMax * scaleVertical) + lenDimHeight);

		if(bDimEL)
		{
			Dom.SetTextAngle(0);
			Dom.SetTextAlignHorz(TA_RIGHT);
			Dom.TextOut(staBridgeEnd+lenStrEle, (eleBridgeMax * scaleVertical) + ((lenStrSta / 2) + dTextHeight * 1.3), strEl);
			
		}
		*pDomP << Dom;

		if(bDimEL)
			pDomArrow.GiSiArrowExt(staBridgeEnd, (eleBridgeMax * scaleVertical) + ((lenStrSta / 2) + dTextHeight), 0, lenStrEle, 0, "");
		*pDomP << pDomArrow;

		Dom.MoveTo(staBridgeEnd, (eleBridgeMax * scaleVertical) + lenDimHeight);
		Dom.LineTo(staBridgeEnd + (lenStrSta + dTextHeight), (eleBridgeMax * scaleVertical) + lenDimHeight);

		Dom.SetTextAngle(0);
		Dom.SetTextAlignHorz(TA_CENTER);
		Dom.TextOut(staBridgeEnd + (lenStrSta/2 + dTextHeight), (eleBridgeMax * scaleVertical) + lenDimHeight + dTextHeight * 0.3, "교량종점");
		Dom.TextOut(staBridgeEnd + (lenStrSta/2 + dTextHeight), (eleBridgeMax * scaleVertical) + lenDimHeight - dTextHeight * 1.3,  strSta);
		Dom.Rotate(CDPoint(staBridgeEnd, (eleBridgeMax * scaleVertical) + lenDimHeight), CDPoint(2,1));

		*pDomP << Dom;
	}

	if(bDimSlope)
	{
		double giulJong =  pLine->GetJongDanGiul((staBridgeStt + staBridgeEnd)/2);
		strSta.Format("S = %.6lf %%",giulJong*100);
		lenStrSta  = Dom.GetTextWidth(strSta) * 1.25;
		eleBridgeEnd = pLine->GetElevationPlanEl((staBridgeStt + staBridgeEnd)/2);

	//	Dom.GiSiArrowExt((staBridgeStt + staBridgeEnd)/2 + lenStrSta/2, (eleBridgeEnd * scaleVertical) + (dTextHeight * 0.3), 0, -lenStrSta, 4, strSta);
	//	Dom.Rotate(atan(giulJong * scaleVertical), (staBridgeStt + staBridgeEnd)/2 + lenStrSta/2, eleBridgeEnd * scaleVertical);

		Dom.SetTextAlignHorz(TA_CENTER);
		Dom.SetTextAngle(ToDegree(atan(giulJong * scaleVertical)));
		Dom.TextOut((staBridgeStt + staBridgeEnd)/2, (eleBridgeEnd * scaleVertical) + (dTextHeight * 0.3), strSta);
		*pDomP << Dom;

		eleBridgeEnd = pLine->GetElevationPlanEl((staBridgeStt + staBridgeEnd)/2 + lenStrSta/2);
		pDomArrow.GiSiArrowExt((staBridgeStt + staBridgeEnd)/2 + lenStrSta/2, (eleBridgeEnd * scaleVertical) + (dTextHeight * 0.3), 0, -lenStrSta, 4, "");
		pDomArrow.Rotate(atan(giulJong * scaleVertical), (staBridgeStt + staBridgeEnd)/2 + lenStrSta/2, eleBridgeEnd * scaleVertical);

 		*pDomP << pDomArrow;
	}
}

// 상부슬래브 둔각부 철근(횡방향으로 뻗어가는 점철근도 같이 그린다)
void CDrawVertSection::DrawRebarMain_VertSection_SlabObtuse(CDomyun *pDomP,BOOL bUpperSlab, BOOL bStt, BOOL bLeft, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	BOOL bBoxType = pBri->IsBoxType();
	
	long stt	= bStt ? iSTT : iEND;
	long left	= bLeft ? iLEFT : iRIGHT;

	if(!bUpperSlab)
	{
		if(bBoxType && !pBri->m_bIsObtuseAngle_LowerSlab[stt][left]) 
			return;
	}
	else if(!pBri->m_bIsObtuseAngle_UpperSlab[stt][left]) 
	{
		return;
	}

	CTwinVector tvBase	= bUpperSlab? pBri->GetTvOffset_UpperSlab(-1, TRUE, -1, -1, pBri->IsVertDir()) : pBri->GetTvOffset_LowerSlab(-1, FALSE, -1, -1, pBri->IsVertDir());
	CDPoint xyBase	= bStt ? tvBase.m_v1 : tvBase.m_v2;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	for(long upper = 0; upper < 2; upper++)
	{
		BOOL bUpper	= upper == iUPPER;

		CDPointArray xyArrPoint, xyArrPointMid;
		CRebarInfoCycle *pRC	= NULL;
		if(bUpperSlab)
		{
			pRC = &pBri->m_rebarInfoCycle_ObtuseAngle_Upperslab[stt][left][upper];
		}
		else
		{
			pRC = &pBri->m_rebarInfoCycle_ObtuseAngle_Lowerslab[stt][left][upper];//[bUpper? iLOWER : iUPPER];
			CRebarInfoCycle *pRC2 = &pBri->m_rebarInfoCycle_ObtuseAngle_Lowerslab[stt][left][bUpper? iLOWER : iUPPER];

			xyArrPoint.Add(pRC->m_xyStt);
			xyArrPoint.Add(pRC->m_xyEnd);
			xyArrPoint.Add(pRC2->m_xyStt);
			xyArrPoint.Add(pRC2->m_xyEnd);
			xyArrPointMid.Add(pRC->m_xyMid1);
			xyArrPointMid.Add(pRC->m_xyMid2);
			xyArrPointMid.Add(pRC2->m_xyMid1);
			xyArrPointMid.Add(pRC2->m_xyMid2);

		}

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
		m_pStd->DrawRebarInfoCycle(&Dom, pRC);

		if(bDimRebar)
		{
			long nDimDan	= 1;
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

			BOOL bBaseUL = (bUpperSlab && bUpper) || (!bUpperSlab && bUpper)? TRUE : FALSE;
			if(bInput)
			{
				if(bBaseUL)
				{
					Dom.SetDirection(bStt ? "LEFT" : "RIGHT");
					CDPoint xyStt(0,0), xyEnd(0,0);

					if(bUpperSlab)
					{
						xyStt	= pRC->m_xyStt.y < pRC->m_xyEnd.y? pRC->m_xyStt : pRC->m_xyEnd;
						xyEnd = pRC->m_xyMid1.y < pRC->m_xyMid2.y?  pRC->m_xyMid2: pRC->m_xyMid1;
					}
					else
					{
						CDPoint xyPrev, xyCurr;
						for (long ix =0; ix < xyArrPoint.GetSize(); ix++)
						{
							xyCurr = xyArrPoint.GetAt(ix);
							if(ix ==0 )
								xyStt = xyCurr;								
							else
								xyStt = Compare(xyCurr.y, xyPrev.y, _T(">"), 0.1)? xyCurr : xyPrev;								
	
							xyPrev = xyStt;
						}

						CDPoint xyPrevM, xyCurrM;
						for (long ix =0; ix < xyArrPointMid.GetSize(); ix++)
						{
							xyCurrM = xyArrPointMid.GetAt(ix);
							if(ix ==0 )
								xyEnd = xyCurrM;
							else
								xyEnd = Compare(xyCurrM.y, xyPrevM.y, _T(">"), 0.1)? xyCurrM : xyPrevM;								
							xyPrevM = xyEnd;
						}
					}

					double dDist  = xyStt.y- xyEnd.y;
					double dDist2 = xyEnd.y - xyBase.y;

					Dom.DimMoveTo(xyBase.x, xyBase.y, nDimDan, "");
					Dom.DimLineTo(dDist2, nDimDan, COMMA(fabs(dDist2)));
					Dom.DimLineToExtend(dDist, nDimDan, "UL2", COMMA(fabs(dDist)));
					
					Dom.SetDirection("TOP");
					if(bUpperSlab)
					{
						xyStt = bStt ? pRC->m_xyMid2 : pRC->m_xyStt;
						xyEnd = bStt ? pRC->m_xyEnd : pRC->m_xyMid1;
					}
					else
					{
						xyStt	= pRC->m_xyMid2; 
						xyEnd	= pRC->m_xyEnd; 
					}

					if(bStt)
					{
						Dom.DimMoveTo(xyBase.x, xyStt.y, nDimDan);
						Dom.DimLineTo(xyStt.x-xyBase.x, nDimDan, COMMA(xyStt.x-xyBase.x));
					}
					else
						Dom.DimMoveTo(xyStt.x, xyStt.y, nDimDan, "");
					Dom.DimLineToExtend(fabs(xyEnd.x-xyStt.x), nDimDan, "UL1", COMMA(fabs(xyEnd.x-xyStt.x)));
					if(!bStt)
					{
						Dom.DimLineTo(xyBase.x-xyEnd.x, nDimDan, COMMA(xyBase.x-xyEnd.x));
					}

					// 상면 치수 기입시는 피복 치수도 같이 넣어줌
					double dCoverU	= bUpperSlab? pBri->m_dCoverObtuseAngle_UpperSlab[stt][left][iUPPER] : pBri->m_dCoverObtuseAngle_LowerSlab[stt][left][iUPPER];
					double dCoverL	= bUpperSlab? pBri->m_dCoverObtuseAngle_UpperSlab[stt][left][iLOWER] : pBri->m_dCoverObtuseAngle_LowerSlab[stt][left][iLOWER];

					Dom.SetDirection(bStt ? "RIGHT" : "LEFT");
					xyStt	= bStt ? xyEnd : xyStt;
					xyStt.y	-= (pBri->m_dTS - dCoverU);
					double dis	= pBri->m_dTS - dCoverL - dCoverU;
					Dom.DimMoveToExt(xyStt.x, xyStt.y, 0, "D2", COMMA(dCoverL));
					Dom.DimLineTo(dCoverL, 0, "");
					Dom.DimLineTo(dis, 0, COMMA(dis));
					Dom.DimLineToOverExtend(dCoverU, 0, "D1", COMMA(dCoverU));
				}
				else
				{
					Dom.SetDirection("BOTTOM");

					CDPoint xyStt	(0,0), xyEnd(0,0);
					if(bUpperSlab)
					{
						xyStt = bStt ? pRC->m_xyStt : pRC->m_xyMid1;
						xyEnd=  bStt ? pRC->m_xyMid1 : pRC->m_xyStt;
					}
					else
					{
						xyStt	= pRC->m_xyStt ; 
						xyEnd	=pRC->m_xyMid1;
					}

					Dom.DimMoveTo(xyStt.x, xyStt.y, nDimDan, "");
					Dom.DimLineToExtend(fabs(xyEnd.x-xyStt.x), nDimDan, "LL", COMMA(fabs(xyEnd.x-xyStt.x)));
				}
			}
			else
			{
				CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> pArrRC;
				CRebarInfoCycle *pRCTmp	= new CRebarInfoCycle;
				*pRCTmp	= *pRC;
				pArrRC.Add(pRCTmp);

				CDPoint xyStt(0,0), xyEnd(0,0);
				BOOL bLeft = bUpper;
				if(bUpperSlab)
				{
					xyStt		= pRC->m_xyStt;
					xyEnd		= bUpper ? pRC->m_xyEnd : pRC->m_xyMid1;
					if(!bStt && bBoxType && bUpper == FALSE)
						bLeft = TRUE;
				}
				else
				{
					if(bStt)
					{
						xyStt		= pRC->m_xyStt;
						xyEnd		= bUpper ? pRC->m_xyEnd : pRC->m_xyMid2;						
					}
					else
					{
						xyStt		= pRC->m_xyStt;
						xyEnd		= bUpper ? pRC->m_xyMid2 : pRC->m_xyEnd;
						bLeft		= FALSE;
					}					
				}			

				m_pStd->DimRebarInfoCycleArray(&Dom, &pArrRC, xyStt, xyEnd, bLeft, nDimDan, 0, FALSE, TRUE,FALSE, !bUpperSlab);

				AhTPADelete(&pArrRC, (CRebarInfoCycle*)0);
			}
		}

		if(bMarkRebar)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

			double dAddHeight	= bUpperSlab? pBri->m_dCoverObtuseAngle_UpperSlab[stt][left][upper] : pBri->m_dCoverObtuseAngle_LowerSlab[stt][left][upper];

			m_pStd->MarkRebarInfoCycle(&Dom, pRC, bUpper, FALSE, dAddHeight, bStt ? 2 : 0, 0, CDRect(0, 0, 0, 0), 0.5, bInput, &pBri->m_pArrRebarMain);
		}
	}

	// 점철근
	CHgBaseDrawStd baseDraw;

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);
	CDPointArray xyArrUpperSlabUpper, xyArrUpperSlabLower, xyArrUpperSide;
	pBri->GetXyRebarMain_VertSection_SlabObtuse(xyArrUpperSlabUpper, xyArrUpperSide, xyArrUpperSlabLower, bStt, bLeft, !bUpperSlab);

	long nIdxMoveStt_SlabUpper		= -1;
	long nIdxMoveEnd_SlabUper		= -1;
	long nIdxMove_SlabSide			= -1;
	long nIdxMoveStt_SlabLower		= -1;
	long nIdxMoveEnd_SlabLower	= -1;
	if(bBoxType)
	{
		if(bUpperSlab)
		{
			nIdxMoveStt_SlabUpper = bStt ? iOUTTER : -1;
			nIdxMoveEnd_SlabUper = bStt ? -1: iINNER;
			nIdxMove_SlabSide		  = bStt ? -1 : 1;
			nIdxMoveStt_SlabLower  = bStt ? iOUTTER : iOUTTER;
			nIdxMoveEnd_SlabLower = bStt ? iINNER: iINNER;
		}
		else
		{
			nIdxMoveStt_SlabUpper = bStt ? iOUTTER : iOUTTER;
			nIdxMoveEnd_SlabUper = bStt ? -1 : iOUTTER;
			nIdxMove_SlabSide		  = bStt ? -1 : 1;
			nIdxMoveStt_SlabLower  = bStt ? iOUTTER : iOUTTER;
			nIdxMoveEnd_SlabLower = bStt ? -1 : -1;
		}		
	}
	else
	{
		nIdxMoveStt_SlabUpper = bStt ? iINNER : -1;
		nIdxMoveEnd_SlabUper = bStt ? -1 : iINNER;
		nIdxMove_SlabSide		  = bStt ? -1 : 1;
		nIdxMoveStt_SlabLower  = bStt ? iINNER : -1;
		nIdxMoveEnd_SlabLower = bStt ? -1 : iINNER;
	}

	baseDraw.DrawPointRebar(&Dom, xyArrUpperSlabUpper, CDPoint(0, 1), nIdxMoveStt_SlabUpper, nIdxMoveEnd_SlabUper);
	baseDraw.DrawPointRebar(&Dom, xyArrUpperSide, CDPoint(nIdxMove_SlabSide, 0));
 	baseDraw.DrawPointRebar(&Dom, xyArrUpperSlabLower, bBoxType? CDPoint(0, -1) : CDPoint(0, +1),  nIdxMoveStt_SlabLower, nIdxMoveEnd_SlabLower);	

	// 점철근중 입력중인경우 외측벽체에 있는 철근은 치수를 기입한다.
	if(bDimRebar && xyArrUpperSide.GetSize() > 0)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		CPlaceRebarByBlock *pPlace	= bUpperSlab? &pBri->m_placeByBlock_ObtuseAngle_Support_OutWall[stt][left] : &pBri->m_placeByBlock_ObtuseAngle_Support_LowerOutWall[stt][left];
		CDoubleArray dArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		CString sDir	= bStt ? _T("LEFT") : _T("RIGHT");
		CDPoint xyStt =  xyArrUpperSide.GetAt(0); //bBoxType? xyArrUpperSide.GetAt(0) : xyArrUpperSide.GetAt(xyArrUpperSide.GetSize()-1);

		double dDir = bUpperSlab? -1.0 : +1.0;
		double dVal = 0;

		dVal = fabs(xyBase.y - xyStt.y);

		dArrDim.Add(dVal*dDir);
		sArrText.Add(bInput ? " " : _T(" "));
		sArrUnderText.Add(COMMA(dVal)); 

		CDPoint xy(0, 0);
		CString str	= _T("");

		if(pPlace->m_xyArr.GetSize() > 0)
		{
			xy	= pPlace->m_xyArr.GetAt(0);
				
			dVal = fabs(xy.x*xy.y);
			dArrDim.Add(dVal*dDir);
			sArrText.Add(bInput ? "C1'" : COMMA(dVal));
			sArrUnderText.Add(COMMA(dVal)); 

			xy	= pPlace->m_xyArr.GetAt(1);
			if(bInput)
			{
				dVal = fabs(xy.x*xy.y);
				str.Format("B'@C'=%s", COMMA(dVal));
			}
			else
			{
				dVal = fabs(xy.x*xy.y);
				str.Format("%.0f@%s=%s", fabs(xy.x), COMMA(fabs(xy.y)), COMMA(dVal));
			}

			dVal = fabs(xy.x*xy.y);
			dArrDim.Add(dVal*dDir);
			sArrText.Add(str);
			sArrUnderText.Add(COMMA(dVal));

			xy	= pPlace->m_xyArr.GetAt(2);
			dVal = fabs(xy.x*xy.y);
			dArrDim.Add(dVal*dDir);
			sArrText.Add(bInput ? "C2'" : COMMA(dVal));
			sArrUnderText.Add(COMMA(dVal));

// 			xy	= xyArrUpperSide.GetAt(xyArrUpperSide.GetSize() - 1);
// 			dVal = fabs(xy.x*xy.y);
// 			dArrDim.Add(bStt ? tvBase.m_v1.y - xy.y : tvBase.m_v2.y - xy.y);
// 			sArrText.Add("0");
// 			sArrUnderText.Add("0");

			if(!bInput)
				sArrUnderText.RemoveAll();
			
			long nDimDan	= 2;
			baseDraw.AutoDimCont(&Dom, xyBase, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, FALSE, FALSE);
		}
	}

	*pDomP << Dom;
}

// 종단면 둔각부 보강 상세 출력
void CDrawVertSection::DrawRebarVertSectionObtuseStd(CDomyun *pDomP, BOOL bStt, BOOL bLeft, BOOL bDimRebar, BOOL bMarkRebar, BOOL bLowerSlab)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	long stt	= bStt ? iSTT : iEND;
	long left	= bLeft ? iLEFT : iRIGHT;

	if(pBri->IsBoxType() && bLowerSlab)
	{
		if(!pBri->m_bIsObtuseAngle_LowerSlab[stt][left])
			return;
	}
	else if(!pBri->m_bIsObtuseAngle_UpperSlab[stt][left]) 
		return;

	CDomyun Dom(pDomP);

	CDRect rectLimit(0, 0, 0, 0);
	CDRect rectVert	= pBri->m_tvArrVertSection.GetRect();
	rectLimit	= rectVert;

	CRebarInfoCycle *pRCUpper	= bLowerSlab? &pBri->m_rebarInfoCycle_ObtuseAngle_Lowerslab[stt][left][iLOWER] : &pBri->m_rebarInfoCycle_ObtuseAngle_Upperslab[stt][left][iUPPER];
	CRebarInfoCycle *pRCLower	= bLowerSlab? &pBri->m_rebarInfoCycle_ObtuseAngle_Lowerslab[stt][left][iUPPER] : &pBri->m_rebarInfoCycle_ObtuseAngle_Upperslab[stt][left][iLOWER];

	double dExtend = 0;
	if(bStt)
	{
		rectLimit.right		=	max(pRCUpper->m_xyEnd.x, pRCLower->m_xyMid1.x);
		if(pBri->IsBoxType())
		{
			dExtend = bLowerSlab? 0 : 1000;
			rectLimit.top			=  max(max(pRCUpper->m_xyStt.y, pRCUpper->m_xyEnd.y), max(pRCLower->m_xyStt.y, pRCLower->m_xyEnd.y)) + dExtend;
			rectLimit.bottom		=  min(min(pRCUpper->m_xyStt.y, pRCUpper->m_xyEnd.y), min(pRCLower->m_xyStt.y, pRCLower->m_xyEnd.y));
			rectLimit.bottom		-= Dom.Always(30);
		}
		else
		{
			rectLimit.bottom		=  min(pRCUpper->m_xyStt.y, pRCLower->m_xyStt.y);
		}
	}
	else
	{
		rectLimit.left		= min(pRCUpper->m_xyStt.x, pRCLower->m_xyMid1.x);
		if(pBri->IsBoxType())
		{
			dExtend = bLowerSlab? 0 : 1000;
			rectLimit.top			=  max(max(pRCUpper->m_xyStt.y, pRCUpper->m_xyEnd.y), max(pRCLower->m_xyStt.y, pRCLower->m_xyEnd.y)) + dExtend;
			rectLimit.bottom		=  min(min(pRCUpper->m_xyStt.y, pRCUpper->m_xyEnd.y), min(pRCLower->m_xyStt.y, pRCLower->m_xyEnd.y));
			rectLimit.bottom		-= Dom.Always(30);
		}
		else
		{
			rectLimit.bottom	= min(pRCUpper->m_xyEnd.y, pRCLower->m_xyStt.y);
		}		
	}

	double dExt	= Dom.Always(3);
	rectLimit.left -= dExt;
	rectLimit.right += dExt;
	rectLimit.top += dExt;
	rectLimit.bottom -= dExt;

	CTwinVectorArray tvArrVert;
	tvArrVert	= pBri->m_tvArrVertSection;
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	m_pStd->DrawCuttingTvArray(&Dom, tvArrVert, rectLimit, TRUE);
	
	// 철근
	DrawRebarMain_VertSection_SlabObtuse(&Dom, !bLowerSlab , bStt, bLeft, bDimRebar, bMarkRebar, FALSE);

	*pDomP << Dom;
}

void CDrawVertSection::DrawRebarMain_VertSection_OutWallAcute(CDomyun *pDomP, BOOL bStt, BOOL bLeft, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	long stt	= bStt ? iSTT : iEND;
	long left	= bLeft ? iLEFT : iRIGHT;
	if(!pBri->m_bIsAcuteAngle_OutWall[stt][left]) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CRebarInfoCycle *pRC	= &pBri->m_rebarInfoCycle_AcuteAngle_OutWall[stt][left];

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	m_pStd->DrawRebarInfoCycle(&Dom, pRC);

	if(bDimRebar)
	{
		long nDimDan	= 1;
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		if(bInput)
		{
			Dom.SetDirection(bStt ? "RIGHT" : "LEFT");
			CDPoint xyStt	= pRC->m_xyMid2;
			CDPoint xyEnd	= pRC->m_xyEnd;
			Dom.DimMoveTo(xyStt.x, xyStt.y, nDimDan, "");
			Dom.DimLineToExtend(fabs(xyEnd.y-xyStt.y), nDimDan, "OL", COMMA(fabs(xyEnd.y-xyStt.y)));
		}
		else
		{
			CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> pArrRC;
			CRebarInfoCycle *pRCTmp	= new CRebarInfoCycle;
			*pRCTmp	= *pRC;
			pArrRC.Add(pRCTmp);

			CDPoint xyStt	= pRC->m_xyStt;
			CDPoint xyEnd	= pRC->m_xyEnd;
			m_pStd->DimRebarInfoCycleArray(&Dom, &pArrRC, xyStt, xyEnd, TRUE, nDimDan, 0, FALSE, TRUE);

			AhTPADelete(&pArrRC, (CRebarInfoCycle*)0);
		}
	}

	if(bMarkRebar)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		double dAddHeight	= pBri->m_dCoverAcuteAngle_OutWall[stt][left][iUPPER];

		m_pStd->MarkRebarInfoCycle(&Dom, pRC, bStt ? TRUE : FALSE, TRUE, dAddHeight, 2, 0, CDRect(0, 0, 0, 0), 0.5, bInput, &pBri->m_pArrRebarMain);
	}

	*pDomP << Dom;
}

void CDrawVertSection::DrawRebarSupport_VertSection_OutWallAcuteAngle(CDomyun *pDomP, BOOL bStt, BOOL bLeft, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, long nVertDir)
{
	long stt	= bStt ? iSTT : iEND;
	long left	= bLeft ? iLEFT : iRIGHT;
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(!pBri->IsOutWall(bStt)) return;
	if(!pBri->m_bIsAcuteAngle_OutWall[stt][left]) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	BOOL bVertDir	= pBri->IsVertDir();
	if(nVertDir != 0)
		bVertDir	= nVertDir == -1 ? TRUE : FALSE;
	// 철근 가져와서 그리기
	CHgBaseDrawStd baseDraw;
	CDPointArray xyArr;
	pBri->GetXyRebarSupport_VertSection_OutWallAcuteAngle(xyArr, bStt, bLeft, nVertDir);
	CDPoint xyDirScale	= bStt ? CDPoint(-1, 0) : CDPoint(1, 0);
	baseDraw.DrawPointRebar(&Dom, xyArr, xyDirScale);

	// 치수 기입
	if(bDimRebar)
	{
		if(xyArr.GetSize() > 0)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

			CDPointArray xyArrDim;
			CDPoint xyStt	= xyArr.GetAt(0);
			CStringArray sArrText, sArrUnderText;
			CString sDir	= bStt ? _T("LEFT") : _T("RIGHT");
			
			// 치수에 피복과 R정보 추가해야 됨. //////////////////////
			CTwinVector tvLower, tvUpper;
			CRebarInfoCycle *pRCLower	= NULL;
			CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;
			
			// 정보 추가(시작부)
			if(pFooting->m_bIs)
			{
				if(pFooting->m_tvArrFront.GetTvByInfo("기초하면", tvLower))
				{
					xyArrDim.Add(tvLower.m_v1);
				}
			}
			pRCLower	= &pBri->m_rebarInfoCycle_AcuteAngle_OutWall[stt][left];

			xyArrDim.Add(pRCLower->m_xyMid1);
			xyArrDim.Add(pRCLower->m_xyMid2);

			long i = 0; for(i = 0; i < xyArr.GetSize(); i++)
				xyArrDim.Add(xyArr.GetAt(i));

			// 치수 기입
			if(xyArrDim.GetSize() > 1)
			{
				if(bStt)
					xyStt	= CDPoint(tvLower.GetXyLeft().x, xyArrDim.GetAt(0).y);
				else
					xyStt	= CDPoint(tvLower.GetXyRight().x, xyArrDim.GetAt(0).y);

				baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, 0, sDir, -1, FALSE, TRUE, FALSE, TRUE, FALSE, FALSE, FALSE);
			}
		}
	}


	// 마킹
	CString sDescription	= _T("");
	CString sStt	= bStt ? "시점" : "종점";
	CString sLeft	= bLeft ? "좌측" : "우측";
	sDescription.Format("%s%s벽체 예각부 배력철근", sStt, sLeft);
	CString sMark			= pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescription);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		CRebar *pRb = pBri->GetRebarSupportByDescription(sDescription);
		CPlaceRebarByBlock *pPlace		= &pBri->m_placeByBlock_AcuteAngle_SupportRebar_OutWall[stt][left];
		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
		mark.SetRebar(xyArr);

		// 설정
		mark.m_sMark					= sMark;
		mark.m_sDia						= pBri->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(bStt? ePartOutWallStt : ePartOutWallEnd));
		mark.m_nTypeMarkPointUpAndDown	= bStt ? TYPE_MARKPOINT_UPPER : TYPE_MARKPOINT_LOWER;
		mark.m_nTypeMarkDir				= TYPE_MARKDIR_LEFTTAN;
		mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
		mark.m_dPosScaleMarkStt			= 0.75;
		mark.m_dCtc = pPlace->GetCTC();
		mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;

		mark.MarkRebar();
	}

	*pDomP << Dom;
}

// 리브 정보 표시
void CDrawVertSection::DimTextRibInfo(CDomyun *pDomP)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetTextAlignHorz(TA_LEFT);
	
	CRebarPlacing rb;
	CDPoint xyOrg(0, 0);
	CDPointArray xyArrMatch;
	CString str[6];

	for(long upper = 0; upper < 3; upper++)
	{
		// 테스트
		double dSta	= pBri->m_dStationBridgeStt + pBri->m_dLengthBridge/4;

		CTwinVectorArray tvArrRib;
		CParabola *pPb;
		double dRibDistTotal	= 0;
		double dRibHeightTotal	= 0;
		double dRibFactorM		= 0;
		long nDir				= 0;
		CDPoint xyDir			= CDPoint(1, 0);

		switch(upper)
		{
			case iUPPER: 
			{
				tvArrRib		= pBri->m_tvArrVertSectionRib[iUPPER];
				pPb				= &pBri->m_pbRib[iUPPER];
				nDir			= 3;
				xyDir			= ToDPointFrDegree(135);
			}
			break;
			case iLOWER: 
			{
				tvArrRib		= pBri->m_tvArrVertSectionRib[iLOWER];
				pPb				= &pBri->m_pbRib[iLOWER];
				nDir			= 7;
				xyDir			= ToDPointFrDegree(315);
			}
			break;
			case 2: 
			{
				dSta	+= pBri->m_dLengthBridge/2;
				tvArrRib		= pBri->m_tvArrVertSectionRibCen;
				pPb				= &pBri->m_pbRibCen;
				nDir			= 5;
				xyDir			= ToDPointFrDegree(225);
			}
			break;
		}
		dRibDistTotal	= pPb->m_dDistTotal;;
		dRibHeightTotal	= pPb->m_dHeightTotal;
		dRibFactorM		= pPb->m_dFactorM;

		rb.GetXyMatchTvArrAndLine(tvArrRib, CDPoint(dSta, 0), CDPoint(0, 1), xyArrMatch);
		if(xyArrMatch.GetSize() > 0)
			xyOrg	= xyArrMatch.GetAt(0);

		str[0].Format("       f");
		str[1].Format("y = ------ (coshkz-1)");
		str[2].Format("     m - 1");
		str[3].Format("f = %.3f (아치 라이즈)", dRibHeightTotal);
		str[4].Format("l = %.3f (아치 지간)", dRibDistTotal);
		str[5].Format("m = %.3f", dRibFactorM);

		double dHeight	= upper == 2 ? Dom.GetTextHeight() * 5 + 500 : Dom.GetTextHeight() * 5;
		double dWMax	= 0;
		long i = 0; for(i = 0; i < 6; i++)
		{
			if(i == 0)
				dWMax	= Dom.GetTextWidth(str[i]);
			else
				dWMax	= max(dWMax, Dom.GetTextWidth(str[i]));
		}

// 		CDPoint xyText	= xyOrg;
		CDPoint xyText = xyOrg + xyDir * dHeight/ToDPointFrDegree(45).y;
		xyText.x -= ((xyDir*dHeight/ToDPointFrDegree(45).y).x - (xyDir*dHeight).x);
		xyText.y += Dom.GetTextHeight() * 2 * 1.5 + (Dom.GetTextHeight() * 1.5 - Dom.GetTextHeight())/2;
		
		if(nDir == 3 || nDir == 5)
		{
			xyText.x -= dWMax;
		}

		for(i = 0; i < 6; i++)
		{
			Dom.TextOut(xyText.x, xyText.y, str[i]);
			xyText.y -= Dom.GetTextHeight(TRUE) * 1.5;
		}
		
		Dom.GiSiArrowExtend(xyOrg.x, xyOrg.y, dHeight, upper == iLOWER ? dWMax : -dWMax, nDir, "", "");
	}

	*pDomP << Dom;
}

// 이음 간격
void CDrawVertSection::DimRebarMain_VertSection_ArchRib(CDomyun *pDomP, long nCycle, BOOL bUpper, long nDan, long nDimDan, BOOL bInput, long nVertDir)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH) return;
	if(nCycle == -1) nCycle = pBri->GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > pBri->GetCountCycleMainRebar()-1) return;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;

	long upper		= bUpper ? iUPPER : iLOWER;

	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> arrRC;
	pBri->m_Util.AddRebarInfoCycle(&arrRC, &pBri->m_pArrRCMainRib[nCycle][upper][nDan], FALSE);
	if(arrRC.GetSize() == 0) return;
	CRebarInfoCycle *pRC	= arrRC.GetAt(0);
	CDPoint xyFir	= pRC->m_xyMid2;
	CDPoint xyLast	= pRC->m_xyMidExt;
	if(arrRC.GetSize() > 1)
	{
		pRC	= arrRC.GetAt(arrRC.GetSize()-1);
		xyLast	= pRC->m_xyMid1;
	}

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CString sNum	= bUpper ? "RT" : "RB";
	long nNum		= 1;

	// 치수기입용 변수
	CDoubleArray dArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir	= bUpper ? _T("TOP") : _T("BOTTOM");
	CDPoint xyStt	= xyFir;

	CString sText(""), sUnderText("");
	CDoubleArray dArrDist;
	dArrDist	= pBri->m_dArrDistRib[nCycle][upper][nDan];
	long nCountJoint	= nCycle == 0 || nCycle == 2 ? pBri->m_nCountJointMainRebar_ArchRib_Cycle1[upper][nDan] : pBri->m_nCountJointMainRebar_ArchRib_Cycle2[upper][nDan];
	double dTot	= 0;
	long i = 0; for(i = 0; i < nCountJoint; i++)
	{
		if(dArrDist.GetSize() <= i) continue;
		if(bInput)
		{
			sText.Format("%s%d", sNum, nNum++);
			sUnderText	= COMMA(dArrDist.GetAt(i));
		}
		else
		{
			sText	= COMMA(dArrDist.GetAt(i));
			sUnderText	= "0";

		}
		dArrDim.Add(dArrDist.GetAt(i));
		sArrText.Add(sText);
		sArrUnderText.Add(sUnderText);

		dTot += dArrDist.GetAt(i);
	}

	if(dTot == 0) 
	{
		AhTPADelete(&arrRC, (CRebarInfoCycle*)0);
		return;
	}

	CParabola pbRebar;
	double dCover	= bUpper ? -pBri->m_dCoverRib[upper][nDan] : pBri->m_dCoverRib[upper][nDan];
	pBri->m_pbRib[upper].GetOffsetParabola(pbRebar, dCover);
	double dTotalLen	= pbRebar.GetLenByDistToDist(pbRebar.GetDistByXPos(xyFir.x), pbRebar.GetDistByXPos(xyLast.x));
	dArrDim.Add(dTotalLen - dTot);
	sArrText.Add("0");
	sArrUnderText.Add("0");
	
	CHgBaseDrawStd baseDraw;
	
	m_pStd->AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE, TRUE, FALSE, TRUE, TRUE, 90, TRUE, "", &pbRebar);

	*pDomP << Dom;
	AhTPADelete(&arrRC, (CRebarInfoCycle*)0);
}


void CDrawVertSection::DrawRebarSupport_VertSection_ArchRib(CDomyun *pDomP, BOOL bUpper, long nDan, BOOL bMarkRebar, BOOL bDimRebar, long nDimDan, BOOL bInput, CDRect rectLimit, long nVertDir)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH) return;

	long upper	= bUpper ? iUPPER : iLOWER;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	// 철근 가져와서 그리기
	CHgBaseDrawStd baseDraw;
	CDPointArray xyArr[2];
	CDPointArray xyArrDir[2];
	pBri->GetXyRebarSupport_VertSection_ArchRib(xyArr[iLEFT], xyArrDir[iLEFT], xyArr[iRIGHT], xyArrDir[iRIGHT], bUpper, nDan, nVertDir, TRUE);

	long left = 0; for(left = 0; left < 2; left++)
	{
		BOOL bLeft	= left == iLEFT;

		// rectLimit 적용
		if(rectLimit != CDRect(0, 0, 0, 0))
		{
			CTwinVectorArray tvArrLimit;
			CVectorArray vecArrLimit;
			tvArrLimit.SetFromRect(rectLimit);
			tvArrLimit.GetToVectorArray(vecArrLimit);

			CRebarPlacing rb;
			rb.GetXyByVectorArrayArea(xyArr[left], vecArrLimit, TRUE);

			CDPoint xyDirScale	= bUpper ? CDPoint(0, 1) : CDPoint(0, -1);
			baseDraw.DrawPointRebar(&Dom, xyArr[left], xyDirScale);
		}
		else
		{
			CDPoint xyDirScale	= bUpper ? CDPoint(0, 1) : CDPoint(0, -1);
			baseDraw.DrawPointRebar(&Dom, xyArr[left], xyArrDir[left]);
		}

		// 마킹
		CString sDescription	= _T("");
		if(bUpper)
			sDescription.Format("아치리브 배선 %d단 배력철근", nDan+1);
		else
			sDescription.Format("아치리브 복선 %d단 배력철근", nDan+1);
		CString sMark	= pBri->GetMarkRebarSupportArchRib(0, bInput, FALSE, FALSE, sDescription);
		if(bMarkRebar && m_pStd->IsMarkRebar(sMark) && bLeft)
		{
			CDPointArray xyArrTot;
			CRebarPlacing rb;
			xyArrTot	= xyArr[iLEFT];
			rb.ReverseRebar(xyArrTot);
			long i = 0; for(i = 0; i < xyArr[iRIGHT].GetSize(); i++)
				xyArrTot.Add(xyArr[iRIGHT].GetAt(i));

			xyArrTot.Sort(TRUE);

			CRebar *pRb = pBri->GetRebarSupportByDescription(sDescription);
			CPlaceRebarByBlock *pPlace		= &pBri->m_placeByBlock_SupportRebar_ArchRib[upper][nDan][left];
			CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
			mark.SetRebar(xyArrTot);

			// 설정
			mark.m_sMark					= sMark;
			mark.m_sDia						= pBri->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(ePartUpperSlab));
			mark.m_nTypeMarkPointUpAndDown	= bUpper ? TYPE_MARKPOINT_UPPER : TYPE_MARKPOINT_LOWER;
			mark.m_nTypeMarkDir				= TYPE_MARKDIR_LEFTTAN;
			mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
			mark.m_dPosScaleMarkStt			= nDan > 0 ? 0.1 : 0.3;
			mark.m_dCtc = pPlace->GetCTC();
			mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;

			mark.MarkRebar();
		}
	}

	long nCountDan	= pBri->GetDanMainRebarRib(0, bUpper);
	for(left = 0; left < 2; left++)
	{
		BOOL bLeft	= left == iLEFT;
	
		// 치수 기입
		// 치수 기입 할때는 선에 딱 붙도록 점들을 옮겨야 된다.
		if(bDimRebar)
		{
			pBri->GetXyRebarSupport_VertSection_ArchRib(xyArr[iLEFT], xyArrDir[iLEFT], xyArr[iRIGHT], xyArrDir[iRIGHT], bUpper, nDan, nVertDir, FALSE);
			xyArr[iLEFT].Sort(TRUE, FALSE);
			xyArr[iRIGHT].Sort(TRUE, TRUE);

			if(xyArr[left].GetSize() > 0)
			{
				CParabola pbRebar;
				pBri->GetRebarRib(pbRebar, bUpper, 0);
				m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

				long nSize	= pBri->m_pArrRCMainRib[0][upper][0].GetSize();
				CRebarInfoCycle *pRCStt	= pBri->m_pArrRCMainRib[0][upper][0].GetAt(0);
				CRebarInfoCycle *pRCEnd	= pBri->m_pArrRCMainRib[0][upper][0].GetAt(nSize-1);
				double xDisFir	= pbRebar.GetDistByXPos(pRCStt->m_xyMid2.x);
				double xDisLast	= pbRebar.GetDistByXPos(pRCEnd->m_bUseEndExt ? pRCEnd->m_xyEnd.x : pRCEnd->m_xyMid1.x);

				if(!bInput || !bUpper || nDan > 0)
				{
					CRebarPlacing rb;
					CDPointArray xyArrDim;
					xyArrDim	= xyArr[left];
					if(bLeft)
					{
						rb.ReverseRebar(xyArrDim);
						if(nDan == 0)
							xyArrDim.InsertAt(0, pbRebar.GetXyByDist(xDisFir));
					}
					else
					{
						if(nDan == 0)
						{
							xyArrDim.InsertAt(0, pbRebar.GetXyByDist(0));
							xyArrDim.Add(pbRebar.GetXyByDist(xDisLast));
						}
					}

					CStringArray sArrText;
					CStringArray sArrUnderText;
					CString sDir	= bUpper ? _T("TOP") : _T("BOTTOM");

					CDPoint xyStt	= nDan == 0 ? (bLeft ? xyArrDim.GetAt(0) : pbRebar.m_xyCrown) : xyArrDim.GetAt(0);
					m_pStd->AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nCountDan-nDan, sDir, -1, FALSE, TRUE, FALSE, TRUE, FALSE, FALSE, TRUE, TRUE, 90, TRUE, "", &pbRebar, bUpper ? 0 : pBri->m_dAccuracyArchRibLowerSupportDim);

					// 총치수
					if(nDan == 0 && bLeft)
					{
						xyArrDim.RemoveAll();
						sArrText.RemoveAll();
						sArrUnderText.RemoveAll();
						xyStt	= pbRebar.GetXyByDist(xDisFir);

						xyArrDim.Add(pbRebar.GetXyByDist(xDisFir));
						xyArrDim.Add(pbRebar.GetXyByDist(xDisLast));
						m_pStd->AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nCountDan-nDan+1, sDir, -1, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, TRUE, TRUE, 90, TRUE, "", &pbRebar);
					}
				}
				else
				{
				
					CDoubleArray dArrDim;
					CStringArray sArrText;
					CStringArray sArrUnderText;
					CString sDir	= bUpper ? _T("TOP") : _T("BOTTOM");
					long nDimDan	= nCountDan-nDan;
					CDPoint xyStt	= bLeft ? pbRebar.GetXyByDist(xDisFir) : pbRebar.m_xyCrown;

					CString sText	= _T("");
					CString sUnderText	= _T("");
					CDPoint xyR	= CDPoint(0, 0);
					CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_SupportRebar_ArchRib[upper][nDan][left];

					if(bLeft)
					{
						dArrDim.Add(pbRebar.GetLenByDistToDist(xDisFir, pbRebar.GetDistByXPos(xyArr[left].GetAt(xyArr[left].GetSize()-1).x)));
						sArrText.Add("0");
						sArrUnderText.Add("0");
					}
					long i = 0; for(i = 0; i < pPlace->m_nCountBlock; i++)
					{
						long nIdx	= bLeft ? pPlace->m_nCountBlock-1 - i : i;
						xyR	= pPlace->m_xyArr.GetAt(nIdx);
						sText.Format("%s%d", bLeft ? "UL" : "UR", nIdx+1);
						if(xyR.x > 1)
							sUnderText.Format("%0.f@%s=%s", xyR.x, COMMA(xyR.y), COMMA(xyR.x*xyR.y));
						else
							sUnderText.Format("%s", COMMA(xyR.x*xyR.y));

						dArrDim.Add(xyR.x*xyR.y);
						sArrText.Add(sText);
						sArrUnderText.Add(sUnderText);
					}

					if(!bLeft)
					{
						dArrDim.Add(pbRebar.GetLenByDistToDist(pbRebar.GetDistByXPos(xyArr[left].GetAt(xyArr[left].GetSize()-1).x), xDisLast));
						sArrText.Add("0");
						sArrUnderText.Add("0");
					}

					m_pStd->AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE, TRUE, FALSE, TRUE, TRUE, 90, TRUE, "", &pbRebar);

					// 총치수
					if(bLeft)
					{
						dArrDim.RemoveAll();
						sArrText.RemoveAll();
						sArrUnderText.RemoveAll();
						xyStt	= pbRebar.GetXyByDist(xDisFir);

						dArrDim.Add(pbRebar.GetLenByDistToDist(xDisFir, xDisLast));
						m_pStd->AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan+1, sDir, -1, FALSE, FALSE, FALSE, TRUE, FALSE, TRUE, TRUE, 90, TRUE, "", &pbRebar);
					}
				}
			}
		}
	}
	*pDomP << Dom;
}

void CDrawVertSection::DrawRebarShear_VertSection_ArchRib(CDomyun *pDomP, BOOL bMarkRebar, BOOL bDimRebar, BOOL bInput, CDRect rectLimit, long nVertDir)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH) return;

	CDomyun Dom(pDomP);
	
	// 철근 가져와서 그리기
	CTwinVectorArray tvArr[2];
	pBri->GetTvRebarShear_VertSection_ArchRib(tvArr[iLEFT], tvArr[iRIGHT], nVertDir);

	long left = 0; for(left = 0; left < 2; left++)
	{
		BOOL bLeft	= left == iLEFT;

		// rectLimit 적용
		if(rectLimit != CDRect(0, 0, 0, 0))
		{
			CTwinVectorArray tvArrLimit;
			CVectorArray vecArrLimit;
			tvArrLimit.SetFromRect(rectLimit);
			tvArrLimit.GetToVectorArray(vecArrLimit);

			CRebarPlacing rb;
			rb.GetTvArrayByCrossVectorArrayArea(tvArr[left], vecArrLimit, TRUE);
		}
		if(tvArr[left].GetSize() == 0) continue;

		// 전단 철근 그리기
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);
		m_pStd->DrawTvArrayWidthDashDot(&Dom, tvArr[left], FALSE);

		if(bLeft)
		{
// 			CString sDescription	= _T("");
			CString sDescription	= "아치리브 전단철근";
			CString sMark	= pBri->GetMarkRebarShearArchRib(0, bInput, FALSE, FALSE, sDescription);
			
			if(bMarkRebar && m_pStd->IsMarkRebar(sMark) && tvArr[iLEFT].GetSize() > 0 && tvArr[iRIGHT].GetSize() > 0)
			{
				CRebarShearDetail *pDetail	= &pBri->m_RebarShearDetail_ArchRib;
				CString sDia	= pBri->m_pARcBridgeDataStd->GetStringDia(pDetail->m_dDia, pBri->GetValueFy(ePartUpperSlab));

				m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
				CParabola pbRebarC;
				pbRebarC	= pBri->m_pbRibCen;

				CConcSymbol sym(m_pStd->m_pARoadOptionStd);
				CTwinVector tv;
				long i = 0; for(i = tvArr[iLEFT].GetSize()-1; i >= 0; i--)
				{
					tv	= tvArr[iLEFT].GetAt(i);
					sym.RebarCountMarkOne(&Dom, tv.GetXyMid(), ToDPointFrDegree(tv.GetXyDir().GetAngleDegree()-90));
				}
				
				for(i = 0; i < tvArr[iRIGHT].GetSize(); i++)
				{
					tv	= tvArr[iRIGHT].GetAt(i);
					sym.RebarCountMarkOne(&Dom, tv.GetXyMid(), ToDPointFrDegree(tv.GetXyDir().GetAngleDegree()-90));
				}					

				// 선그리기
				double xDisFir	= pbRebarC.GetDistByXPos(tvArr[iLEFT].GetAt(tvArr[iLEFT].GetSize()-1).GetXyMid().x);
				double xDisLast	= pbRebarC.GetDistByXPos(tvArr[iRIGHT].GetAt(tvArr[iRIGHT].GetSize()-1).GetXyMid().x);
				CHgBaseDrawStd baseDraw;
				m_pStd->DrawParabola(&Dom, &pbRebarC, xDisFir, xDisLast, 0);

				// 기호 출력
				CTwinVectorArray tvArrMark;
				tvArrMark	= tvArr[iRIGHT].GetSize() > 1 ? tvArr[iRIGHT] : tvArr[iLEFT];
				if(tvArrMark.GetSize() > 1)
				{
					double xDis1	= pbRebarC.GetDistByXPos(tvArrMark.GetAt(0).GetXyMid().x);
					double xDis2	= pbRebarC.GetDistByXPos(tvArrMark.GetAt(1).GetXyMid().x);
					CDPoint xyMark	= pbRebarC.GetXyByDist((xDis1+xDis2)/2);
					double dAddHeight	= (pBri->m_pbRib[iUPPER].m_xyCrown.y-pBri->m_pbRib[iLOWER].m_xyCrown.y)/2;
					dAddHeight += Dom.Always(MARK_BASE_HEIGHT+MARK_BASE_RADIUS*1.5);

					// sym.RebarMarkLineLower(&Dom, xyMark, sMark, sDia, FALSE, dAddHeight, FALSE);

					SRebarMarkInputData mark_input_data;
					SRebarMarkviewData mark_view_data;
					mark_input_data.mark = sMark;
					mark_input_data.dia = sDia;
					mark_input_data.position = xyMark;
					mark_input_data.ctc = pBri->GetCTCRebarShear_ArchRib();
					mark_input_data.type_rebar = pDetail->GetRebarType();
					mark_input_data.left_dir =  FALSE;
					mark_input_data.height = dAddHeight;

					mark_view_data.count_mark = FALSE;
					mark_view_data.mark_pos = eMarkLower;
					
					sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
				}
			}
		}
		
		if(bDimRebar)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
			if(tvArr[left].GetSize() > 0)
			{
				CParabola pbRebar;
				pBri->GetRebarRib(pbRebar, TRUE, 0);

				CHgBaseDrawStd baseDraw;
				long nSize	= pBri->m_pArrRCMainRib[0][iUPPER][0].GetSize();
				CRebarInfoCycle *pRC	= pBri->m_pArrRCMainRib[0][iUPPER][0].GetAt(0);
				CDPoint xyFir	= pbRebar.GetXyMatchXyAndAngle(pRC->m_xyMid2, CDPoint(1, 0));
				pRC	= pBri->m_pArrRCMainRib[0][iUPPER][0].GetAt(nSize-1);
				CDPoint xyLast	= pbRebar.GetXyMatchXyAndAngle(pRC->m_bUseEndExt ? pRC->m_xyEnd : pRC->m_xyMid1, CDPoint(1, 0));

				CDPointArray xyArr;
				long i = 0; for(i = 0; i < tvArr[left].GetSize(); i++)
					xyArr.Add(tvArr[left].GetAt(i).m_v1);
				if(bLeft)
				{
					CRebarPlacing rb;
					rb.ReverseRebar(xyArr);
				}

				if(!bInput)
				{
					if(bLeft)
						xyArr.InsertAt(0, xyFir);
					else
						xyArr.Add(xyLast);
					CStringArray sArrText;
					CStringArray sArrUnderText;
					CString sDir	= _T("TOP");

					m_pStd->AutoDimCont(&Dom, xyArr.GetAt(0), xyArr, sArrText, sArrUnderText, 0, sDir, -1, FALSE, TRUE, FALSE, TRUE, FALSE, FALSE, TRUE, TRUE, 90, TRUE, "", &pbRebar);

					// 총치수
					if(bLeft)
					{
						xyArr.RemoveAll();
						sArrText.RemoveAll();
						sArrUnderText.RemoveAll();
						
						xyArr.Add(xyFir);
						xyArr.Add(xyLast);

						m_pStd->AutoDimCont(&Dom, xyFir, xyArr, sArrText, sArrUnderText, 1, sDir, -1, FALSE, TRUE, FALSE, TRUE, FALSE, FALSE, TRUE, TRUE, 90, TRUE, "", &pbRebar);
					}
				}
				else
				{
					CDoubleArray dArrDim;
					CStringArray sArrText;
					CStringArray sArrUnderText;
					CString sDir	= _T("TOP");
					long nDimDan	= 0;
					CDPoint xyStt	= bLeft ? xyFir : pbRebar.m_xyCrown;

					CString sText	= _T("");
					CString sUnderText	= _T("");
					CDPoint xyR	= CDPoint(0, 0);
					CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_ShearRebar_ArchRib[left];

					if(bLeft)
					{
						dArrDim.Add(pbRebar.GetLenByDistToDist(pbRebar.GetDistByXPos(xyFir.x), pbRebar.GetDistByXPos(xyArr.GetAt(0).x)));
						sArrText.Add("0");
						sArrUnderText.Add("0");
					}
					long i = 0; for(i = 0; i < pPlace->m_nCountBlock; i++)
					{
						long nIdx	= bLeft ? pPlace->m_nCountBlock - 1 - i : i;
						xyR	= pPlace->m_xyArr.GetAt(nIdx);
						sText.Format("%s%d", bLeft ? "VL" : "VR", nIdx+1);
						if(xyR.x > 1)
							sUnderText.Format("%0.f@%s=%s", xyR.x, COMMA(xyR.y), COMMA(xyR.x*xyR.y));
						else
							sUnderText.Format("%s", COMMA(xyR.x*xyR.y));

						dArrDim.Add(xyR.x*xyR.y);
						sArrText.Add(sText);
						sArrUnderText.Add(sUnderText);
					}

					if(!bLeft)
					{
						dArrDim.Add(pbRebar.GetLenByDistToDist(pbRebar.GetDistByXPos(xyArr.GetAt(xyArr.GetSize()-1).x), pbRebar.GetDistByXPos(xyLast.x)));
					}

					m_pStd->AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE, TRUE, FALSE, TRUE, TRUE, 90, TRUE, "", &pbRebar);

										// 총치수
					if(bLeft)
					{
						dArrDim.RemoveAll();
						sArrText.RemoveAll();
						sArrUnderText.RemoveAll();

						dArrDim.Add(pbRebar.GetLenByDistToDist(pbRebar.GetDistByXPos(xyFir.x), pbRebar.GetDistByXPos(xyLast.x)));
						m_pStd->AutoDimCont(&Dom, xyFir, dArrDim, sArrText, sArrUnderText, nDimDan+1, sDir, -1, FALSE, FALSE, FALSE, TRUE, FALSE, TRUE, TRUE, 90, TRUE, "", &pbRebar);
					}
				}
			}
		}
	}

	*pDomP << Dom;
}

CDRect CDrawVertSection::DrawSupportRebarArchRibStd(CDomyun *pDomP, BOOL bDimRebar, BOOL bMarkRebar, BOOL bMarkMainRebar, long nVertDir)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return CDRect(0, 0, 0, 0);
		
	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	
	CDRect rect(0, 0, 0, 0);
	CDomyun Dom(pDomP);
	long nLeft	= 0;
	long dan(0), upper(0), i(0);
	BOOL bUpper(TRUE);

	BOOL bVertDir	= pBri->IsVertDir();
	if(nVertDir != 0)
	{
		bVertDir	= nVertDir == -1 ? TRUE : FALSE;
	}
	nVertDir	= bVertDir ? -1 : 1;

	// 종단면도 그리기
	// 말뚝 비율을 기본값으로
	pBri->m_footingEnd.m_exFooting.m_Pile.InitScaleCutting();
	// 모든 객체들의 동기화를 해줌.
	//pBri->SyncBridge(nLeft, !bVertDir);
	pBri->GetTvVertSection_Std(nLeft, !bVertDir, FALSE);
	pBri->SyncBridgeRebar(FALSE, nVertDir);

	// 아치리브 종단면 그림
	// 기초는 벽체와 overlap 체크 한거 제거해서 그린다.
	CTwinVectorArray tvArrStt, tvArrEnd;
	tvArrStt	= pBri->m_footingStt.m_tvArrFront;
	tvArrEnd	= pBri->m_footingEnd.m_tvArrFront;
	for(i = 0; i < tvArrStt.GetSize(); i++)
	{
		CTwinVector tv	= tvArrStt.GetAt(i);
		tv.m_vecOverlap1=CVector(0, 0, 0);
		tv.m_vecOverlap2=CVector(0, 0, 0);
		tvArrStt.SetAt(i, tv);
	}
	for(i = 0; i < tvArrEnd.GetSize(); i++)
	{
		CTwinVector tv	= tvArrEnd.GetAt(i);
		tv.m_vecOverlap1=CVector(0, 0, 0);
		tv.m_vecOverlap2=CVector(0, 0, 0);
		tvArrEnd.SetAt(i, tv);
	}

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	CHgBaseDrawStd baseDraw;
	baseDraw.DrawTvArray(&Dom, pBri->m_tvArrVertSectionRib[iUPPER]);
	baseDraw.DrawTvArray(&Dom, pBri->m_tvArrVertSectionRib[iLOWER]);
	DrawVertSectionArchRibHiddenLine(&Dom);
	baseDraw.DrawTvArray(&Dom, tvArrStt);
	baseDraw.DrawTvArray(&Dom, tvArrEnd);


	// 주철근 1cycle 그리기
	for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		DrawRebarMain_VertSection_ArchRib(&Dom, 0, dan, TRUE, FALSE, bMarkMainRebar, 0, CDRect(0, 0, 0, 0), FALSE, nVertDir);
	}
	
	// 배력철근/전단철근 그리기
	for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		for(upper = 0; upper < 2; upper++)
		{
			bUpper = (upper==iUPPER);
			// 리브
			DrawRebarSupport_VertSection_ArchRib(&Dom, bUpper, dan, bMarkRebar, TRUE, dan, FALSE, CDRect(0, 0, 0, 0), nVertDir);
		}
	}
	// 리브
	DrawRebarShear_VertSection_ArchRib(&Dom, bMarkRebar, FALSE, FALSE, CDRect(0, 0, 0, 0), nVertDir);

	// section 마크 2개
	char ch	= 'A';
	CString str	= _T("");
	BOOL bOK	= FALSE;

	// 기초 근처
	double dXDisOfUpper	= m_pStd->m_pDrawCrossSection->GetDistForCrossSection_ArchRib(0, bOK);
	str.Format("%c", ch++);
	DimTextSection_ArchRib(&Dom, dXDisOfUpper, str);

	// 2단 철근 있는 곳
	dXDisOfUpper	= m_pStd->m_pDrawCrossSection->GetDistForCrossSection_ArchRib(1, bOK);
	if(bOK)
	{
		str.Format("%c", ch++);
		DimTextSection_ArchRib(&Dom, dXDisOfUpper, str);
	}

	// 크라운부
	dXDisOfUpper	= m_pStd->m_pDrawCrossSection->GetDistForCrossSection_ArchRib(2, bOK);
	str.Format("%c", ch++);
	DimTextSection_ArchRib(&Dom, dXDisOfUpper, str);

	pBri->GetTvVertSection_Std(nLeft, !bVertDir, FALSE, TRUE);
	*pDomP << Dom;

	Dom.SetCalcExtRect();
	return Dom.GetExtRect();
}

// 리브에 섹션 마크
void CDrawVertSection::DimTextSection_ArchRib(CDomyun *pDomP, double dXDisOfUpper, CString sSection)
{	
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDPoint xyMark	= pBri->m_pbRib[iUPPER].GetXyByDist(dXDisOfUpper);
	CDPoint vAng	= pBri->m_pbRib[iUPPER].GetXyDirTanByDist(dXDisOfUpper);
	double dThick	= pBri->GetThickArchRib(dXDisOfUpper);

	xyMark			= pBri->m_pbRib[iLOWER].GetXyMatchXyAndAngle(xyMark, vAng.RotateInv90());

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_STRS);
	CConcSymbol sym(m_pStd->m_pARoadOptionStd);
	sym.ArrowSection(&Dom, sSection, xyMark, dThick, ToDPointFrDegree(vAng.GetAngleDegree()+180));

	*pDomP << Dom;
}

// 아치리브 지점부 상세
CDRect CDrawVertSection::DrawRebarDetailFooting_Front_ArchJijum(CDomyun *pDomP, BOOL bStt, double dScaleDetail)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	CDRect rect	= CDRect(0, 0, 0, 0);
	if(!pBri) return rect;
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH) return rect;

	CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;
	if(!pFooting->IsRebarSupportFrontSlope(0)) return rect;

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScaleDetail);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	CHgBaseDrawStd baseDraw;
	CDPoint xyOrg(0, 0);
	CDPoint xyDirRot(1, 0);
	CTwinVector tvSlope	= pFooting->m_tvArrFront.GetTvByInfo(pFooting->m_nType == FOOTING_TYPE_BASIC_RIGHTSLOPE ? "기초우측경사" : "기초좌측경사");
	xyDirRot	= ToDPointFrDegree(90-tvSlope.GetXyDir().GetAngleDegree()-90);

	// 기초 상면/주철근1단 그리기
	CTwinVectorArray tvArrDraw, tvArrDrawRebar;

	// 일반도
	CTwinVectorArray tvArrUpper;
	pFooting->GetTvOffset_Front(-1, TRUE, tvArrUpper, TRUE);
	tvArrDraw.AddFromTvArray(tvArrUpper);

	// 주철근
	CRebarInfoCycle *pRC	= &pFooting->m_rebarInfoCycle_Cycle1[iUPPER][0];
	CTwinVector tv;
	tv.m_v1.z	= 0;
	tv.m_v1	= pRC->m_xyStt;
	tv.m_v2	= pRC->m_xyMid1;
	tvArrDrawRebar.Add(tv);
	tv.m_v1	= tv.m_v2;
	tv.m_v2	= pRC->m_xyMid2;
	tvArrDrawRebar.Add(tv);
	tv.m_v1	= tv.m_v2;
	tv.m_v2	= pRC->m_xyEnd;
	tvArrDrawRebar.Add(tv);

	// 필요한 부분만 남기고 잘라냄.
	CRebarPlacing rb;
	rb.RotateTvArr(tvArrDraw, xyOrg, xyDirRot.GetAngleDegree());
	rb.RotateTvArr(tvArrDrawRebar, xyOrg, xyDirRot.GetAngleDegree());
	tvSlope	= rb.RotateTv(tvSlope, xyOrg, xyDirRot.GetAngleDegree());
	rect	= tvArrDraw.GetRect();
	rect.left = tvSlope.GetXyLeft().x - 300;
	rect.right	= tvSlope.GetXyRight().x + 300;

	CTwinVectorArray tvArrRect;
	tvArrRect.SetFromRect(rect);
	rb.GetTvArrayByCrossTvArrayArea(tvArrDraw, tvArrRect, TRUE);
	rb.GetTvArrayByCrossTvArrayArea(tvArrDrawRebar, tvArrRect, TRUE);
	rb.RotateTvArr(tvArrDraw, xyOrg, -xyDirRot.GetAngleDegree());
	rb.RotateTvArr(tvArrDrawRebar, xyOrg, -xyDirRot.GetAngleDegree());
	
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	baseDraw.DrawTvArray(&Dom, tvArrDraw);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	baseDraw.DrawTvArray(&Dom, tvArrDrawRebar);

	
	// 기초 경사면 주철근/배력근
	long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		m_pStd->m_pDrawFooting->DrawRebarMain_FrontSlope(&Dom, pFooting, dan, dan==0, dan==0, FALSE);
		m_pStd->m_pDrawFooting->DrawRebarSupport_FrontSlope(&Dom, pFooting, dan, dan==0, dan==0, FALSE);
	}

	
	Dom.Rotate(xyOrg, xyDirRot);

	Dom.RedrawByScale(pDomP->GetScaleDim()/dScaleDetail);

	Dom.SetCalcExtRect();
	rect	= Dom.GetExtRect();
	*pDomP << Dom;

	return rect;
}

// 아치리브 l과 f 치수 기입
// nUpper == 1 : 상단
// 0 : 축선
// -1 : 하단
void CDrawVertSection::DimVertSectionArchRiblAndf(CDomyun *pDomP, long nUpper, BOOL bInput)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CParabola *pPb	= &pBri->m_pbRibCen;
	if(nUpper == 1) pPb			= &pBri->m_pbRib[iUPPER];
	else if(nUpper == -1) pPb	= &pBri->m_pbRib[iLOWER];

	// l
	CDPoint xyStt	= pPb->m_xyCrown;
	xyStt.x -= pPb->m_dDistTotal/2;
	xyStt.y -= pPb->m_dHeightTotal;

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
	Dom.LineTo(xyStt.x, xyStt.y, xyStt.x + pPb->m_dDistTotal, xyStt.y);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetDirection("BOTTOM");
	Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
	Dom.DimLineToExtend(pPb->m_dDistTotal, 0, bInput ? "l" : COMMA(pPb->m_dDistTotal), bInput ? COMMA(pPb->m_dDistTotal) : "");

	// f
	xyStt	= pPb->m_xyCrown;
	xyStt.y	-= pPb->m_dHeightTotal;

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
	Dom.LineTo(xyStt.x, xyStt.y, xyStt.x, xyStt.y+pPb->m_dHeightTotal);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetDirection("RIGHT");
	Dom.SetDimObq(20);
	Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
	Dom.DimLineToExtend(pPb->m_dHeightTotal, 0, bInput ? "f" : COMMA(pPb->m_dHeightTotal), bInput ? COMMA(pPb->m_dHeightTotal) : "");
	CDPoint xyMatch	= pBri->GetXyMatchLineAndSlabUpper(xyStt, CDPoint(0, 1), 0, pBri->m_dTS);
	if(xyMatch.y > pPb->m_xyCrown.y)
	{
		Dom.DimLineToOver(xyMatch.y-pPb->m_xyCrown.y, 0, COMMA(xyMatch.y-pPb->m_xyCrown.y));
	}

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	CHgBaseDrawStd baseDraw;
	m_pStd->DrawParabola(&Dom, pPb);

	

	// EL
	if(bInput)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		CConcSymbol sym(m_pStd->m_pARoadOptionStd);
		CString sEL1	= m_pStd->GetStringEL(toM(pPb->m_xyCrown.y));
		sym.ArrowElevation(&Dom, sEL1, "", pPb->m_xyCrown, FALSE);

		// sta
		
		sEL1	= "Sta.";
		CString sEL2;
		sEL2.Format("%.0f", pPb->m_xyCrown.x);
		sym.ArrowElevation(&Dom, sEL1, sEL2, pPb->m_xyCrown);
	}

	// o
	if(bInput)
	{
		Dom.SetDimObq(0);
		CTwinVectorArray tvArr;
		switch(nUpper)
		{
		case 1: tvArr	= pBri->m_tvArrVertSectionRib[iUPPER]; break;
		case 0: tvArr	= pBri->m_tvArrVertSectionRibCen; break;
		case -1: tvArr	= pBri->m_tvArrVertSectionRib[iLOWER]; break;
		}
		if(tvArr.GetSize() > 1)
		{
			CTwinVector tv	= tvArr.GetAt(tvArr.GetSize()/8);

			CDoubleArray dArrDim;
			CStringArray sArrText;
			CStringArray sArrUnderText;
			CString sDir	= _T("TOP");
			xyStt	= tv.m_v1;
			xyStt.y	= tv.GetXyTop().y;

			dArrDim.Add(tv.GetHorLength());
			sArrText.Add("O");
			sArrUnderText.Add(COMMA(tv.GetHorLength()));

			baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, 0, sDir, -1, FALSE, FALSE, FALSE);
		}
	}
	
	long nJConnect	= pBri->GetJijumConnectRibAndSlab();
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetDimObq(20);
	Dom.SetDimExo(0);
	if(nJConnect > -1)
	{
		CWallApp *pWall	= pBri->GetInWall(nJConnect-1);
		if(pWall)
		{
			if(pWall->m_bIs)
			{
				double dDiffWidthMin	= pBri->GetXyMatchLineAndSlabUpper(pPb->m_xyCrown, CDPoint(0, 1), 0, 0).y - pBri->GetElMinSlabUpper(pPb->m_xyCrown.x, CDPoint(0, 1));
				double dDiffWidthMax	= pBri->GetXyMatchLineAndSlabUpper(pPb->m_xyCrown, CDPoint(0, 1), 0, 0).y - pBri->GetElMaxSlabUpper(pPb->m_xyCrown.x, CDPoint(0, 1));
				double dMin(0), dMax(0);

				CTwinVector tvUpperSlab;

				CString str	= _T("");
				str.Format("벽체좌측면");
				CTwinVector tv	= pWall->m_tvArrFront.GetTvByInfo(str);

				xyStt	= tv.GetXyBottom();
				tv.m_v1	= xyStt;
				tv.m_v2	= pBri->GetXyMatchLineAndSlabUpper(xyStt, CDPoint(0, 1), 0, pBri->m_dTS);
				tvUpperSlab.m_v1	= tv.m_v2;
				
				Dom.SetDirection("RIGHT");
				Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
				dMin	= tv.GetVerLength()-dDiffWidthMin;
				dMax	= tv.GetVerLength()-dDiffWidthMax;
				str.Format("%.0f~%.0f", dMin, dMax);
				Dom.DimLineTo(tv.GetVerLength(), 0, str);

				str.Format("벽체우측면");
				tv	= pWall->m_tvArrFront.GetTvByInfo(str);
				xyStt	= tv.GetXyBottom();
				tv.m_v1	= xyStt;
				tv.m_v2	= pBri->GetXyMatchLineAndSlabUpper(xyStt, CDPoint(0, 1), 0, pBri->m_dTS);
				tvUpperSlab.m_v2	= tv.m_v2;

				Dom.SetDirection("RIGHT");
				Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
				dMin	= tv.GetVerLength()-dDiffWidthMin;
				dMax	= tv.GetVerLength()-dDiffWidthMax;
				str.Format("%.0f~%.0f", dMin, dMax);
				Dom.DimLineTo(tv.GetVerLength(), 0, str);

				// 내측벽체 연결되면 상부슬래브 연결된 선 그려주기
				m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
				baseDraw.DrawTv(&Dom, tvUpperSlab);
			}

		}
	}

	*pDomP << Dom;
}


void CDrawVertSection::DrawRebarStirrup_VertSection_ConnectSection(CDomyun *pDomP, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH) return;
	if(pBri->GetJijumConnectRibAndSlab() == -1) return;

	CTwinVectorArray tvArrShear, tvArrShearHunch, tvArrShearRib;
	pBri->GetTvRebarShear_VertSection_UpperSlab(tvArrShear, tvArrShearHunch, tvArrShearRib, 0);

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	CHgBaseDrawStd baseDraw;

	CTwinVectorArray tvArr[2];
	pBri->GetTvRebarStirrup_VertSection_ConnectSection(tvArr[iLEFT], tvArr[iRIGHT]);
	long left = 0; for(left = 0; left < 2; left++)
	{
		BOOL bLeft	= left == iLEFT;
		CParabola pbRebar;
		pBri->GetRebarRib(pbRebar, FALSE, 0);

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
		baseDraw.DrawTvArray(&Dom, tvArr[left]);

		CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_StirrupRebar_ConnectSection[left];
		pPlace->m_nCountBlock	= 3;
		pPlace->FitCount();
		if(bDimRebar && tvArr[left].GetSize() > 0)
		{
			*pDomP << Dom;
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
			CDoubleArray dArrDim;
			CStringArray sArrText;
			CStringArray sArrUnderText;
			CDPoint xyStt	= tvArr[left].GetAt(0).m_v1;
			long nDimDan	= 0;
			CString sDir	= bLeft ? "LEFT" : "RIGHT";
			xyStt.y	= pbRebar.GetXyMatchXyAndAngle(xyStt, CDPoint(0, -1)).y;
			
			CDPoint xyR(0, 0);
			CString str	= _T("");

			xyR	= pPlace->m_xyArr.GetAt(0);
			dArrDim.Add(xyR.x*xyR.y);
			sArrText.Add(bInput ? "C1" : COMMA(xyR.y));
			sArrUnderText.Add(COMMA(xyR.y));

			xyR	= pPlace->m_xyArr.GetAt(1);
			str.Format("%s@%s=%s", COMMA(xyR.x), COMMA(xyR.y), COMMA(xyR.x*xyR.y));
			dArrDim.Add(xyR.x*xyR.y);
			sArrText.Add(bInput ? "B@C" : str);
			sArrUnderText.Add(str);

			xyR	= pPlace->m_xyArr.GetAt(2);
			dArrDim.Add(xyR.x*xyR.y);
			sArrText.Add(bInput ? "C2" : COMMA(xyR.y));
			sArrUnderText.Add(COMMA(xyR.y));

			if(!bInput)
			{
				sArrUnderText.RemoveAll();
			}

			baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, FALSE, FALSE);
		}

		CString sDescription	= _T("");
		sDescription.Format("접합부 %s 스터럽 철근", bLeft ? "좌측" : "우측");
		CString sMark	= bInput ? "S" : pBri->GetMarkRebarShear(0, bInput, FALSE, FALSE, sDescription);
		long nSize	= tvArr[left].GetSize();
		if(bMarkRebar && nSize > 0)
		{
			
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

			CRebar *pRb = pBri->GetRebarShearByDescription(sDescription);
			CMarkDomyun mark;
			mark.SetDomyun(&Dom, m_pStd->m_pARoadOptionStd);
			mark.SetRebar(tvArr[left], tvArrShearRib);
			// 설정
			mark.m_sMark	= sMark;
			mark.m_sDia		= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(ePartUpperSlab));
			mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_STT;	// 지름 표시위치 찾는 방법
			mark.m_dPosScaleMarkStt			= 0.5;
			mark.m_nTypeMarkDir				= bLeft ? TYPE_MARKDIR_LEFTTAN : TYPE_MARKDIR_RIGHTTAN;
			mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_LOWER;
			CDPoint xyMark	= mark.GetXyMark();
			CDPoint xyMatch	= pbRebar.GetXyMatchXyAndAngle(xyMark, CDPoint(0, -1));
			mark.m_dAddHeight	= (xyMark.y-xyMatch.y) * 2;
			mark.m_dCtc = pPlace->GetCTC();
			mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;

				
			mark.MarkRebar();
		}
	}

	*pDomP << Dom;	
}

// 아치리브 이음장 치수
void CDrawVertSection::DimRebarMainAssm_VertSection_ArchRib(CDomyun *pDomP, long nDimDan, long nCycle, long nDrawLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	CHgBaseDrawStd baseDraw;

	for(long upper = 0; upper < 2; upper++)
	{
		BOOL bUpper	= upper == iUPPER;
		CParabola pbRebar;
		pBri->GetRebarRib(pbRebar, bUpper, 0);


		CDPointArray xyArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		CDPoint xyStt	= CDPoint(0, 0);
		CString sDir	= bUpper ? "TOP" : "BOTTOM";
		CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &pBri->m_pArrRCMainRib[nCycle][upper][0];
		long nSize	= pArrRC->GetSize();

		if(nSize == 1)
		{
			CRebarInfoCycle *pRC	= pArrRC->GetAt(0);
			xyArrDim.Add(pRC->m_xyMid2);
			xyArrDim.Add(pRC->m_xyEnd);
		}
		else
		{
			for(long rc = 0; rc < nSize; rc++)
			{
				CRebarInfoCycle *pRC	= pArrRC->GetAt(rc);
				if(!pRC->m_bExist) continue;
				if(rc == 0)
				{
					xyArrDim.Add(pRC->m_xyMid2);
					xyArrDim.Add(pRC->m_xyEnd);
				}
				else
				{
					xyArrDim.Add(pRC->m_xyStt);
					xyArrDim.Add(pRC->m_xyMid1);
				}
			}
		}

		

		if(nDrawLeft != 0)
		{
			double dStaCuttingOrg	= m_pStd->GetStaCuttingOrg();
			if(nDrawLeft == -1)
			{
				long i = 0; for(i = 0; i < xyArrDim.GetSize(); i++)
				{
					if(Compare(xyArrDim.GetAt(i).x, dStaCuttingOrg, "=>"))
					{
						xyArrDim.RemoveAt(i);
						i--;
					}
				}
			}
			if(nDrawLeft == 1)
			{
				long i = 0; for(i = 0; i < xyArrDim.GetSize(); i++)
				{
					if(Compare(xyArrDim.GetAt(i).x, dStaCuttingOrg, "=<"))
					{
						xyArrDim.RemoveAt(i);
						i--;
					}
				}
			}
			xyArrDim.Add(CDPoint(dStaCuttingOrg, 0));
		}


		xyArrDim.Sort(TRUE);
		xyStt	= xyArrDim.GetAt(0);
		m_pStd->AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, TRUE, TRUE, 90, TRUE, "", &pbRebar, 0);

		*pDomP << Dom;
	}
}

// 리브 주철근 조립도.
CDRect CDrawVertSection::DrawRebarAssmArchRibStd(CDomyun *pDomP, long nCycleLeft, long nCycleRight, long nVertDir)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return CDRect(0, 0, 0, 0);
		
	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	
	CDRect rect(0, 0, 0, 0);
	CDomyun Dom(pDomP);
	long nLeft	= 0;
	long dan(0), i(0);

	BOOL bVertDir	= pBri->IsVertDir();
	if(nVertDir != 0)
	{
		bVertDir	= nVertDir == -1 ? TRUE : FALSE;
	}
	nVertDir	= bVertDir ? -1 : 1;

	// 종단면도 그리기
	// 말뚝 비율을 기본값으로
	pBri->m_footingEnd.m_exFooting.m_Pile.InitScaleCutting();
	// 모든 객체들의 동기화를 해줌.
	//pBri->SyncBridge(nLeft, !bVertDir);
	pBri->GetTvVertSection_Std(nLeft, !bVertDir, FALSE);
	pBri->SyncBridgeRebar(FALSE, nVertDir);

	// 아치리브 종단면 그림
	// 기초는 벽체와 overlap 체크 한거 제거해서 그린다.
	CTwinVectorArray tvArrStt, tvArrEnd;
	tvArrStt	= pBri->m_footingStt.m_tvArrFront;
	tvArrEnd	= pBri->m_footingEnd.m_tvArrFront;
	for(i = 0; i < tvArrStt.GetSize(); i++)
	{
		CTwinVector tv	= tvArrStt.GetAt(i);
		tv.m_vecOverlap1=CVector(0, 0, 0);
		tv.m_vecOverlap2=CVector(0, 0, 0);
		tvArrStt.SetAt(i, tv);
	}
	for(i = 0; i < tvArrEnd.GetSize(); i++)
	{
		CTwinVector tv	= tvArrEnd.GetAt(i);
		tv.m_vecOverlap1=CVector(0, 0, 0);
		tv.m_vecOverlap2=CVector(0, 0, 0);
		tvArrEnd.SetAt(i, tv);
	}

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	CHgBaseDrawStd baseDraw;
	baseDraw.DrawTvArray(&Dom, pBri->m_tvArrVertSectionRib[iUPPER]);
	baseDraw.DrawTvArray(&Dom, pBri->m_tvArrVertSectionRib[iLOWER]);
	DrawVertSectionArchRibHiddenLine(&Dom);
	baseDraw.DrawTvArray(&Dom, tvArrStt);
	baseDraw.DrawTvArray(&Dom, tvArrEnd);


	if(nCycleLeft == nCycleRight)
	{
		// 주철근 1cycle 그리기
		for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			DrawRebarMain_VertSection_ArchRib(&Dom, nCycleLeft, dan, TRUE, FALSE, TRUE, 0, CDRect(0, 0, 0, 0), FALSE, nVertDir);
		}

		// 주철근 조립도 치수
		DimRebarMainAssm_VertSection_ArchRib(&Dom, 0, nCycleLeft, 0);	
	}
	else
	{
		// 주철근 1cycle 그리기
		for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			DrawRebarMain_VertSection_ArchRib(&Dom, nCycleLeft, dan, TRUE, FALSE, TRUE, -1, CDRect(0, 0, 0, 0), FALSE, nVertDir);
			DrawRebarMain_VertSection_ArchRib(&Dom, nCycleRight, dan, TRUE, FALSE, TRUE, 1, CDRect(0, 0, 0, 0), FALSE, nVertDir);
		}

		// 주철근 조립도 치수
		DimRebarMainAssm_VertSection_ArchRib(&Dom, 0, nCycleLeft, -1);
		DimRebarMainAssm_VertSection_ArchRib(&Dom, 0, nCycleRight, 1);

		// 중심선
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_CENT);
		
		Dom.SetCalcExtRect();
		CDRect rect				= Dom.GetExtRect();
		double dStaCuttingOrg	= m_pStd->GetStaCuttingOrg();

		double dLenExt	= Dom.Always(5);
		double dLenMinusBottom	= Dom.Always(Dom.GetDimVLen()+Dom.GetDimExo()+Dom.GetDimLevelDis());
		CFootingApp *pFooting	= pBri->GetFooting(0);
		if(pFooting)
		{
			dLenMinusBottom += pFooting->GetHeight();
		}

		Dom.LineTo(dStaCuttingOrg, rect.bottom+dLenExt, dStaCuttingOrg, rect.top+dLenMinusBottom);
	}

	pBri->GetTvVertSection_Std(nLeft, !bVertDir, FALSE, TRUE);
	*pDomP << Dom;

	Dom.SetCalcExtRect();
	return Dom.GetExtRect();
}

// 아치리브만 따로 그릴 경우 접합부는 파져서 안 보인다.
// 이 부분은 점선으로 그려줌
void CDrawVertSection::DrawVertSectionArchRibHiddenLine(CDomyun *pDomP)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH) return;
	if(pBri->GetJijumConnectRibAndSlab() == -1) return;
	
	CDomyun Dom(pDomP);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONH);
	CWallApp *pWall	= pBri->GetInWall(pBri->GetJijumConnectRibAndSlab()-1);
	if(pWall)
	{
		CTwinVector tvLeft	= pWall->m_tvArrFront.GetTvByInfo("벽체좌측면");
		CTwinVector tvRight	= pWall->m_tvArrFront.GetTvByInfo("벽체우측면");
		
		CHgBaseDrawStd baseDraw;
		double xDisStt	= pBri->m_pbRib[iUPPER].GetDistByXPos(tvLeft.m_v1.x);
		double xDisEnd	= pBri->m_pbRib[iUPPER].GetDistByXPos(tvRight.m_v1.x);
		
		m_pStd->DrawParabola(&Dom, &pBri->m_pbRib[iUPPER], xDisStt, xDisEnd, 0);
	}



	*pDomP << Dom;
}

// 아치리브 두께 치수
// 시종점 스프링깅부
void CDrawVertSection::DimVertSectionArchRibThick(CDomyun *pDomP)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH) return;
	
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	CHgBaseDrawStd baseDraw;

	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;
		CTwinVector tvSlope	= pFooting->m_tvArrFront.GetTvByInfo(bStt ? "기초우측경사" : "기초좌측경사");

		CDPointArray xyArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		long nDimDan	= 0;
		CString sDir	= "BOTTOM";
		CDPoint xyStt	= CDPoint(0, 0);

		long nSizeU	= pBri->m_tvArrVertSectionRib[iUPPER].GetSize();
		long nSizeL	= pBri->m_tvArrVertSectionRib[iLOWER].GetSize();
		if(nSizeU == 0 || nSizeL == 0) break;

		xyArrDim.Add(tvSlope.GetXyTop());

		CDPoint xyU	= bStt ? pBri->m_tvArrVertSectionRib[iUPPER].GetAt(0).m_v1 : pBri->m_tvArrVertSectionRib[iUPPER].GetAt(nSizeU-1).m_v2;
		CDPoint xyL	= bStt ? pBri->m_tvArrVertSectionRib[iLOWER].GetAt(0).m_v1 : pBri->m_tvArrVertSectionRib[iLOWER].GetAt(nSizeL-1).m_v2;

		if(Compare(xyU.y, tvSlope.GetXyTop().y, "<") && Compare(xyU.y, tvSlope.GetXyBottom().y, ">"))
			xyArrDim.Add(xyU);
		if(Compare(xyL.y, tvSlope.GetXyTop().y, "<") && Compare(xyL.y, tvSlope.GetXyBottom().y, ">"))
			xyArrDim.Add(xyL);

		xyArrDim.Add(tvSlope.GetXyBottom());

		xyArrDim.Sort(TRUE);
		xyStt	= xyArrDim.GetAt(0);
		baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, FALSE, FALSE, TRUE, TRUE);
		*pDomP << Dom;
	}
}

// 내측 라운드 헌치의 직선 가상라인 그림(점선으로)
void CDrawVertSection::DrawVertSectionArchHunchHidden(CDomyun *pDomP)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONH);

	for(long wall = 0; wall < pBri->GetCountInWall(); wall++)
	{
		CWallApp *pWall	= pBri->GetInWall(wall);
		if(pWall)
		{
			CString sHunch[2];
			sHunch[iLEFT].Format("%d번째내측헌치좌측면", wall+1);
			sHunch[iRIGHT].Format("%d번째내측헌치우측면", wall+1);
			CTwinVector tvHunch[2];
			tvHunch[iLEFT]	= pBri->m_tvArrVertSection.GetTvByInfo(sHunch[iLEFT]);
			tvHunch[iRIGHT]	= pBri->m_tvArrVertSection.GetTvByInfo(sHunch[iRIGHT]);

			CTwinVector tvWall[2];
			tvWall[iLEFT]	= pWall->m_tvArrFront.GetTvByInfo("벽체좌측면");
			tvWall[iRIGHT]	= pWall->m_tvArrFront.GetTvByInfo("벽체우측면");
			long left = 0; for(left = 0; left < 2; left++)
			{
				if(tvHunch[left].m_v1.z == -1 || tvHunch[left].m_v1.z == 1)
				{
					CDPoint xyMatch(0, 0);
					if(GetXyMatchSegAndLine(tvHunch[iLEFT].GetXyTop(), tvHunch[iRIGHT].GetXyTop(), tvWall[left].m_v1, tvWall[left].GetXyDir(), xyMatch))
					{
						Dom.LineTo(tvHunch[left].GetXyTop(), xyMatch);
						Dom.LineTo(xyMatch, tvWall[left].GetXyTop());
					}
				}
			}
		}
	}

	*pDomP << Dom;
}

// 신축이음 그리자
void CDrawVertSection::DrawVertSectionFlexibleJoint(CDomyun *pDomP, BOOL bStt)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(!pBri->IsBlockOut(bStt)) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	CRebarPlacing rb;

	CHgBaseDrawStd baseDraw;
	CFlexibleJoint *pFx	= bStt ? &pBri->m_fxJointStt : &pBri->m_fxJointEnd;
	baseDraw.DrawTvArray(&Dom, pFx->m_tvArrSection);

	CTwinVectorArray tvArrArea;
	CTwinVectorArray tvArrHatch;
	tvArrArea	= pFx->m_tvArrSection;
	long i = 0; for(i = 0; i < tvArrArea.GetSize(); i++)
	{
		CTwinVector tv	= tvArrArea.GetAt(i);
		tv.m_v1.z	= 0;
		tvArrArea.SetAt(i, tv);
	}
	rb.GetTvArrayHatch(tvArrArea, tvArrHatch, 100, 45);
	baseDraw.DrawTvArray(&Dom, tvArrHatch);
	
	*pDomP << Dom;
}

// 슬래브에 fck 정보 쓰기
// nStt : -1 시점 , 0 : 중간, 1 : 종점
void CDrawVertSection::DimTextVertSectionUpperSlabFck(CDomyun *pDomP, long nStt)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CTwinVector tvStt, tvEnd;
	if(pBri->IsOutWall(TRUE))
	{
		tvStt	= pBri->m_tvArrVertSection.GetTvByInfo("시점브라켓상부측면");
	}
	else
	{
		tvStt	= pBri->m_tvArrVertSection.GetTvByInfo("시점슬래브측면");
	}

	if(pBri->IsOutWall(FALSE))
	{
		tvEnd	= pBri->m_tvArrVertSection.GetTvByInfo("종점브라켓상부측면");
	}
	else
	{
		tvEnd	= pBri->m_tvArrVertSection.GetTvByInfo("종점슬래브측면");
	}


	CDPoint xyText	= CDPoint(0, 0);
	double dFck = pBri->GetValueFck(ePartUpperSlab, TRUE);		// 단위계적용
	CString sText	= _T("");
	sText.Format("fck=%.f %s", dFck, pBri->m_pARcBridgeDataStd->m_szKgf_CM2);


	if(nStt == -1)
	{
		xyText.x	= tvStt.GetXyLeft().x + Dom.GetTextWidth(sText);
		xyText.y	= tvStt.GetXyTop().y - pBri->m_dTS/2 - Dom.GetTextHeight();
	}
	else if(nStt == 0)
	{
		xyText.x	= (tvStt.GetXyLeft().x + tvEnd.GetXyRight().x)/2 - Dom.GetTextWidth(sText)/2;
		xyText.y	= (tvStt.GetXyTop().y + tvEnd.GetXyTop().y)/2 - pBri->m_dTS/2 - Dom.GetTextHeight();
	}
	else if(nStt == 1)
	{
		xyText.x	= tvEnd.GetXyRight().x - Dom.GetTextWidth(sText);
		xyText.y	= tvEnd.GetXyTop().y - pBri->m_dTS/2 - Dom.GetTextHeight();
	}


	Dom.TextOut(xyText, sText);

	*pDomP << Dom;
}

void CDrawVertSection::DimTextVertSectionLowerSlab(CDomyun *pDomP)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(pBri == NULL || pBri->IsBoxType() == FALSE) return;
	if(pBri->m_nTypeLower != BOXFOOTING_TYPE_PILE) return;

	CExPileApp *pPile = &pBri->m_footingBox.m_exFooting.m_Pile;
	if(pPile == NULL) return;
	if(pPile->m_nCountOrg <= 0)  return;

	CLineInfo *pLine	= pBri->GetLineBase();

	double dDia = pPile->m_dDia;
	double dT1 = pPile->m_dT1;
	double dLS = pPile->m_dLenSteel + 1000;	// 강관의 길이는 입력길이의 +1 (13195)
	double dLP = pPile->m_dLenPHC - 1000;	// phc의 길이는 입력길이의 -1 (13195)
	double dT1S = pPile->m_dT1Steel;
	double dT1P = pPile->m_dT1PHC;

	CDomyun Dom(pDomP);
	CConcSymbol sym(m_pStd->m_pARoadOptionStd);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CString str1, str2, str3;
	str1 = pPile->GetStringPileType();
	if(pPile->m_nType == PILE_TYPE_EHP)
		str1 += pPile->GetStringEHPType(pPile->m_nKind_EHP);
	str2.Format(_T("(%%%%c%.1fX%sT,"), dDia, COMMA(dT1));
	if(pPile->IsDiffPileLen())
		str3.Format(_T("L=VAR."));
	else
		str3.Format(_T("L=%.1fM)"), toM(pPile->m_dLength));

	// 현장타설말뚝은 T를 기입하지 않는다.
	if(dT1==0 || pPile->m_nType == PILE_TYPE_HYUNJANG) 
	{
		str2.Format(_T("(%%%%c%.1f,"), dDia);
	}
	if(pPile->m_nType == PILE_TYPE_SPC)
	{
		str2.Format(_T("(Φ%.fx%sT, L=%.1fM %s)"), dDia, COMMA(dT1S), toM(dLS), hggettext("(강관)"));
		str3.Format(_T("(Φ%.fx%sT, L=%.1fM (PHC))"), dDia, COMMA(dT1P), toM(dLP));
	}

	double dLenStr = MAX(Dom.GetTextWidth(str1, 3), Dom.GetTextWidth(str2, 3));
	CDPoint vAng(0,1);
	double dDrawLen = Dom.Always(30);

	double dStation = pBri->GetStationBridgeStt(FALSE);
	double dStationPile = pLine->GetStationByOffsetWidth(dStation, -pBri->m_dWS, pBri->GetAngleJijum(0));
	
	double dSumDistPile = 0;
	CDPoint xyPileStt(0,0);
	for(long i = 0; i < pPile->m_nCountOrg; i++)
	{
		COrgInfo *pOrg = pPile->m_pArrOrgInfo.GetAt(i);
		dSumDistPile += pOrg->dist;
		Dom.Circle(CDPoint(dStationPile + dSumDistPile, dDrawLen), 50, TRUE);

		if(i==0)
			xyPileStt = CDPoint(dStationPile + pOrg->dist, dDrawLen);
	}
	CDPoint xyText = CDPoint(dStationPile - dLenStr, xyPileStt.y);
	Dom.LineTo(xyPileStt, xyText);
	Dom.LineTo(xyPileStt, CDPoint(dStationPile + dSumDistPile, dDrawLen));

	Dom.TextOut(xyText.x+dLenStr/2, xyText.y+Dom.Always(1), str1);
	Dom.TextOut(xyText.x+dLenStr/2, xyText.y-Dom.GetTextHeight()-Dom.Always(1),str2);
	Dom.TextOut(xyText.x+dLenStr/2, xyText.y-Dom.GetTextHeight()*2-Dom.Always(2),str3);

	*pDomP << Dom;
}

void CDrawVertSection::DimThickVertSection(CDomyun *pDom)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	CDomyun Dom(pDom);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	
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

	CDPoint xyMatch	= pBri->GetXySlabUpperByLenFrSttStation(pBri->GetLengthBridge(FALSE)/3*2, 0, FALSE);
	
	Dom.GiSiArrowExtend(xyMatch.x, xyMatch.y - pBri->m_dTS/2, pBri->m_dTS/2 + Dom.Always(Dom.GetDimExo()+Dom.GetDimExe()+Dom.GetDimVLen()), max(Dom.GetTextWidth(sThickPave), Dom.GetTextWidth(sThickSlab)), 2, sThickPave, sThickSlab);

	if(pBri->GetThickPave() > 0)
	{
		Dom.GiSiArrowExtend(xyMatch.x, xyMatch.y + pBri->GetThickPave()/2, -pBri->GetThickPave()/2 + Dom.Always(Dom.GetDimExo()+Dom.GetDimExe()+Dom.GetDimVLen()), 0, 2, "", "");
	}

	*pDom << Dom;
}

CDRect CDrawVertSection::GetRectCuttingForGirderDetail(long nJ, BOOL bStt, BOOL bLeft, BOOL bGagak, BOOL bJongGirder, BOOL bUpperSlabGirder)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return CDRect(0, 0, 0, 0);

	// 슬래브 상하면에서 좌우로 1000, 상하로 100씩만 띄워서 그림.
	CTwinVector tvUpper;
	CTwinVector tvLower;
	pBri->GetTvVirtualRebarGirder(nJ, tvUpper, tvLower, bStt, bLeft, bGagak, bJongGirder, bUpperSlabGirder);

	double sw = bUpperSlabGirder ? 1.0 : -1.0;

	// 잘라낼 영역
	const double dMarginHor	= 1000;
	const double dMarginVer	= 1000 * sw;
	CDRect rect(tvLower.GetXyLeft().x-dMarginHor, tvUpper.GetXyTop().y+dMarginVer, tvLower.GetXyRight().x+dMarginHor, tvLower.GetXyBottom().y-dMarginVer);

	return rect;
}

void CDrawVertSection::DimVerSectionBoxSideEnd( CDomyun *pDomP, long nDimDan, BOOL bInput, BOOL bForOutBM/*=FALSE*/)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDomP, &Dom);

	if(nDimDan < 0) nDimDan = 0;

	CTwinVector tv;
	CTwinVectorArray *pTvArr = &pBri->m_tvArrVertSection;


	CDoubleArray dArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("RIGHT");
	CDPoint xyStt(0, 0),  xyBase(0, 0);
	CDPoint xySttTot(0, 0), xyEndTot(0, 0);
	double dH = 0;

	pTvArr->GetTvByInfo("하부슬래브하면(종점벽체아래)", tv);
	double dDimAngle = (pBri->m_bSlopeLowerSlab)?  (tv.GetXyDir(TRUE).GetAngleDegree()): 0;
	xyBase = tv.m_v2;
	xyStt = xyBase;
	xySttTot = xyStt;

	// 치수기입 시작위치는 기초가 있을 경우 기초의 확대기초를 기준으로 한다.
	CExFootingApp *pExFooting	= &pBri->m_footingBox.m_exFooting;

	if(pBri->m_nTypeLower == LOWERSLAB_TYPE_MASS && pExFooting->m_bLeftOrgOfMass)
	{
		CString str		= _T("");
		double dH		= pExFooting->GetMaxHeight();
		double dHCur	= 0;
		long i = 0; for(i = 0; i < pExFooting->m_nCountDan; i++)
		{
			str.Format("EH%d", i+1);
			dArrDim.Add(pExFooting->m_dEFH[i]);
			sArrText.Add(str);
			sArrUnderText.Add("");
			dHCur	+= pExFooting->m_dEFH[i];
		}

		dArrDim.Add(dH-dHCur);
		sArrText.Add("0");
		sArrUnderText.Add("");

		xyStt.y -= (dH);
	}
	else
	{
		double dH	= pExFooting->m_dEFHR;
		dArrDim.Add(dH);
		sArrText.Add("EH");
		sArrUnderText.Add(COMMA(dH));

		xyStt.y -= dH;
	}
	
	if(pBri->m_bExistProtectionRiseFooting)
	{
		dArrDim.Add(pBri->m_dPRF_H1);
		sArrText.Add("brH1");
		sArrUnderText.Add(COMMA(pBri->m_dPRF_H1));

		if(pBri->m_dPRF_H2 > 0)
		{
			dArrDim.Add(pBri->m_dPRF_H2);
			sArrText.Add("brH2");
			sArrUnderText.Add(COMMA(pBri->m_dPRF_H2));
		}

		xyStt += pBri->m_dPRF_W * tv.GetXyDir();
		xyBase += pBri->m_dPRF_W * tv.GetXyDir();
	}

	pTvArr->GetTvByInfo("종점벽체우측면", tv);
	dArrDim.Add(tv.GetVerLength());
	sArrText.Add("0");
	sArrUnderText.Add("0");

	pTvArr->GetTvByInfo("종점브라켓헌치", tv);
	dH = tv.GetVerLength();
	if(!bInput)
	{
		dArrDim.Add(dH);
		sArrText.Add("0");
		sArrUnderText.Add(COMMA(dH));
	}
	pTvArr->GetTvByInfo("종점브라켓측면", tv);
	if(bInput)
		dH += tv.GetVerLength();
	else
		dH = tv.GetVerLength();
	dArrDim.Add(dH);
	sArrText.Add("BH");
	sArrUnderText.Add(COMMA(dH));

	pTvArr->GetTvByInfo("종점브라켓상부측면", tv);
	dArrDim.Add(tv.GetVerLength());
	sArrText.Add("BTH");
	sArrUnderText.Add(COMMA(tv.GetVerLength()));

	// 포장 두께
	if(!bForOutBM)
	{
		dArrDim.Add(pBri->GetThickPave(FALSE));
		sArrText.Add("0");
		sArrUnderText.Add("0");
	}
	xyEndTot.y = tv.m_v1.y;

	if(!bInput)
	{
		sArrText.RemoveAll();
		sArrUnderText.RemoveAll();
	}

	Dom.SetDimObq(dDimAngle);
	CHgBaseDrawStd baseDraw;
	baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE);

	// 전체 치수
	{
		// 교량측면 치수
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		m_pStd->ApplyDimSetting(pDomP, &Dom);
		Dom.SetDirection(sDir);
		nDimDan++;
		double dist = xyEndTot.y-xySttTot.y; 
		if(dist > 0)
		{
			Dom.DimMoveTo(xyBase.x, xyBase.y, nDimDan);
			Dom.DimLineTo(dist, nDimDan, COMMA(dist));
		}
	}

	*pDomP << Dom;
}

//bInput ==0 :시종점부 1:중간지점부 2:하부슬래브 3:부상방지저판
void CDrawVertSection::DimVerSectionBoxSideStt( CDomyun *pDomP, long nDimDan, double bInput, BOOL bForOutBM/*=FALSE*/)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDomP, &Dom);

	if(nDimDan < 0) nDimDan = 0;

	CTwinVector tv;
	CTwinVectorArray *pTvArr = &pBri->m_tvArrVertSection;

	CDoubleArray dArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("LEFT");
	CDPoint xyStt(0, 0), xyBase(0,0);
	CDPoint xySttTot(0, 0), xyEndTot(0, 0);
	double dH = 0;

	pTvArr->GetTvByInfo("하부슬래브하면(시점벽체아래)", tv);
	double dDimAngle = (pBri->m_bSlopeLowerSlab)?  -(tv.GetXyDir().GetAngleDegree()): 0;
	xyBase = tv.m_v1;
	xyStt = xyBase;
	xySttTot = xyStt;
	
	// 치수기입 시작위치는 기초가 있을 경우 기초의 확대기초를 기준으로 한다. 
	CExFootingApp *pExFooting	= &pBri->m_footingBox.m_exFooting;
	
	if(pBri->m_nTypeLower == LOWERSLAB_TYPE_MASS && !pExFooting->m_bLeftOrgOfMass) 
	{
		CString str		= _T("");
		double dH		= pExFooting->GetMaxHeight();
		double dHCur	= 0;
		long i = 0; for(i = 0; i < pExFooting->m_nCountDan; i++)
		{
			str.Format("EH%d", i+1);
			dArrDim.Add(pExFooting->m_dEFH[i]);
			sArrText.Add(str);
			sArrUnderText.Add("");
			dHCur	+= pExFooting->m_dEFH[i];
		}

		dArrDim.Add(dH-dHCur);
		sArrText.Add("0");
		sArrUnderText.Add("");

		xyStt.y -= (dH);
	}
	else 
	{
		double dH	= pExFooting->m_dEFHR;
		dArrDim.Add(dH);
		if(bInput == 0)	{sArrText.Add("EH"); sArrUnderText.Add(COMMA(dH));}
		else			{sArrText.Add("0"); sArrUnderText.Add("");}

		xyStt.y -= dH;
	}

	if(pBri->m_bExistProtectionRiseFooting)
	{
		dArrDim.Add(pBri->m_dPRF_H1);
		if(bInput == 3)	{sArrText.Add("brH1");	sArrUnderText.Add(COMMA(pBri->m_dPRF_H1));}
		else			{sArrText.Add("0"); sArrUnderText.Add("");}
		
		if(pBri->m_dPRF_H2 > 0)
		{
			dArrDim.Add(pBri->m_dPRF_H2);
			if(bInput == 3) {sArrText.Add("brH2"); sArrUnderText.Add(COMMA(pBri->m_dPRF_H2));}
			else			{sArrText.Add("0");	sArrUnderText.Add("");}
		}
		
		xyStt += pBri->m_dPRF_W * tv.GetXyDir(TRUE);
		xyBase += pBri->m_dPRF_W * tv.GetXyDir(TRUE);
	}
	else //하부슬래브표시
	{
		dArrDim.Add(pBri->m_dTS_Lower);
		if(bInput == 2)	{sArrText.Add("T"); sArrUnderText.Add(COMMA(pBri->m_dTS_Lower));}
		else{sArrText.Add("0"); sArrUnderText.Add("");}
	}
	
	pTvArr->GetTvByInfo("시점벽체좌측면", tv);
	if(!pBri->m_bExistProtectionRiseFooting)
		dArrDim.Add(tv.GetVerLength() - pBri->m_dTS_Lower);
	else
		dArrDim.Add(tv.GetVerLength());

	sArrText.Add("0");
	sArrUnderText.Add("0");

	pTvArr->GetTvByInfo("시점브라켓헌치", tv);
	dH = tv.GetVerLength();
	if(bInput != 0) 
	{
		dArrDim.Add(dH);
		sArrText.Add("0"); 
		sArrUnderText.Add(COMMA(dH));
	}

	pTvArr->GetTvByInfo("시점브라켓측면", tv);
	if(bInput == 0)
		dH += tv.GetVerLength();
	else
		dH = tv.GetVerLength();

	dArrDim.Add(dH);
	if(bInput == 0){sArrText.Add("BH");	sArrUnderText.Add(COMMA(dH));}
	else{sArrText.Add("0");	sArrUnderText.Add("");}

	pTvArr->GetTvByInfo("시점브라켓상부측면", tv);
	dArrDim.Add(tv.GetVerLength());
	if(bInput == 0)	{sArrText.Add("BTH");sArrUnderText.Add(COMMA(tv.GetVerLength()));}
	else{sArrText.Add("0");	sArrUnderText.Add("");}

	// 포장 두께(수량 산출시는 넣지 않음)
	if(!bForOutBM)
	{
		dArrDim.Add(pBri->GetThickPave(FALSE));
		if(bInput == 0)	{sArrText.Add("0");	sArrUnderText.Add("0");}
		else{sArrText.Add("0");	sArrUnderText.Add("");}
	}

	xyEndTot.y = tv.m_v2.y;

	Dom.SetDimObq(dDimAngle);
	CHgBaseDrawStd baseDraw;
	baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE);

	// 전체 치수	
	{
		// 교량측면 치수
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		m_pStd->ApplyDimSetting(pDomP, &Dom);
		Dom.SetDirection(sDir);
		nDimDan++;
		double dist = xyEndTot.y-xySttTot.y; 
		if(dist > 0)
		{
			Dom.DimMoveTo(xyBase.x, xyBase.y, nDimDan);
			Dom.DimLineTo(dist, nDimDan, COMMA(dist));
		}
	}

	*pDomP << Dom;
}

void CDrawVertSection::DimVertSectionLowerSlab( CDomyun *pDomP, long nLeft, BOOL bDimThick, BOOL bMarkEl, BOOL bOnlyJijumEL )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_BOX) return;

	CDomyun Dom(pDomP);

	m_pStd->ApplyDimSetting(pDomP, &Dom);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	Dom.SetDirection("RIGHT");

	// 일반도는 직거리로 안 그리므로 항상 FALSE
	BOOL bIsVertDir	= FALSE;

	if(bDimThick)
	{
// 		long nJStt	= 0;
// 		long nJEnd	= 0;
// 		double dStaMid	= 0;
		CDPoint xy(0, 0);
		double dObq	= 30;

		Dom.SetDimObq(dObq);
		Dom.SetDimExo(0);
		for(long jigan = 0; jigan < pBri->m_nQtyJigan; jigan++)
		{
			long nJStt	= jigan;
			long nJEnd	= nJStt+1;
			double dStaMid	= (pBri->GetStationOnJijum(nJStt, nLeft, bIsVertDir) + pBri->GetStationOnJijum(nJEnd, nLeft, bIsVertDir))/2;
			xy	= pBri->GetXyMatchLineAndLowerSlab(CDPoint(dStaMid, 0), CDPoint(0, 1), nLeft, 0, bIsVertDir);

			// 슬래브 두께
			Dom.DimMoveTo(xy.x, xy.y, 0);
			Dom.DimLineTo(pBri->m_dTS_Lower, 0, COMMA(pBri->m_dTS_Lower));
		}

		*pDomP << Dom;
	}
	
	if(bMarkEl)
	{
		CConcSymbol sym(m_pStd->m_pARoadOptionStd);

		CString sEL = _T("");
		CDPoint xyStt(0, 0);
		CTwinVector tv;

		// 각 지점별 EL
		BOOL bLeft			= TRUE;
		long nCountJijum	= pBri->GetCountJijum();
		for(long jijum = 0; jijum < nCountJijum; jijum++)
		{
			if(jijum == 0) bLeft = bOnlyJijumEL? TRUE : FALSE;
			else if(jijum == nCountJijum-1) bLeft = bOnlyJijumEL? FALSE : TRUE;
			else bLeft	= FALSE;

			xyStt.x	= pBri->GetStationOnJijum(jijum, 0);
			xyStt.y	= pBri->GetXyMatchLineAndLowerSlab(xyStt, CDPoint(0, 1), 0, 0).y;
			sEL		= m_pStd->GetStringEL(toM(xyStt.y));
			sym.ArrowElevation(&Dom, sEL, "", xyStt, bLeft, TRUE);
		}

		// 시 종점 벽체 끝 상단 EL
		if(!bOnlyJijumEL && pBri->IsOutWall(TRUE))
		{
			xyStt.x	= pBri->GetLineBase()->GetStationByOffsetWidth(pBri->GetStationOnJijum(0, 0), -pBri->m_dWS, pBri->GetAngleJijum(0));
			xyStt.y	= pBri->GetXyMatchLineAndLowerSlab(xyStt, CDPoint(0, 1), 0, 0).y;
			sEL		= m_pStd->GetStringEL(toM(xyStt.y));
			sym.ArrowElevation(&Dom, sEL, "", xyStt, TRUE, TRUE);
		}

		if(!bOnlyJijumEL && pBri->IsOutWall(FALSE))
		{
			xyStt.x	= pBri->GetLineBase()->GetStationByOffsetWidth(pBri->GetStationOnJijum(nCountJijum-1, 0), pBri->m_dWE, pBri->GetAngleJijum(pBri->GetCountJijum()-1));
			xyStt.y	= pBri->GetXyMatchLineAndLowerSlab(xyStt, CDPoint(0, 1), 0, 0).y;
			sEL		= m_pStd->GetStringEL(toM(xyStt.y));
			sym.ArrowElevation(&Dom, sEL, "", xyStt, FALSE, TRUE);
		}

		*pDomP << Dom;
	}
}

void CDrawVertSection::DrawExFootingBoxType(CDomyun *pDomP, BOOL bCutting)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(pBri == NULL || pBri->IsBoxType() == FALSE) return;


	if(pBri->m_nTypeLower == BOXFOOTING_TYPE_STONE)
	{
		DrawStoneBoxType(pDomP, bCutting);
	}
	else if(pBri->m_nTypeLower == BOXFOOTING_TYPE_PILE)
	{
		DrawPileBoxType(pDomP, bCutting);
	}
	else if(pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR)
	{
		DrawEarthAnchorBoxType(pDomP, bCutting);
	}
}

// 함형라멘교 : 잡석기초
void CDrawVertSection::DrawStoneBoxType(CDomyun *pDomP, BOOL bCutting)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(pBri == NULL || pBri->IsBoxType() == FALSE) return;
	if(pBri->m_nTypeLower != BOXFOOTING_TYPE_STONE) return;

	CExFootingApp *pExFoot = &pBri->m_footingBox.m_exFooting;

	CDomyun Dom(pDomP);
	CConcSymbol sym(m_pStd->m_pARoadOptionStd);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	CTwinVector tvR = pExFoot->m_tvArrFront.GetTvByInfo(_T("확대기초우측면"));
	CTwinVector tvL = pExFoot->m_tvArrFront.GetTvByInfo(_T("확대기초좌측면"));

	double dEleBottom = min(tvR.m_v2.y, tvL.m_v1.y) - pBri->m_dThickCobbStone;

	CTwinVector tv;
	CTwinVectorArray tvArr, tvArrHatch;

	CDPoint xy1 = CDPoint(tvL.m_v1.x, dEleBottom);
	CDPoint xy2 = CDPoint(tvR.m_v1.x, dEleBottom);
	tv.m_v1 = xy1;
	tv.m_v2 = xy2;
	tvArr.Add(tv);

	tv.m_v1 = xy1;
	tv.m_v2 = tvL.m_v1;
	tvArr.Add(tv);

	tv.m_v1 = tvR.m_v2;
	tv.m_v2 = xy2;
	tvArr.Add(tv);

	tvArrHatch.AddFromTvArray(tvArr);
	for(long ix = 0; ix < pExFoot->m_tvArrFront.GetSize(); ix++)
	{
		tv = pExFoot->m_tvArrFront.GetAt(ix);
		if(tv.m_sInfo.Find(_T("하면")) != -1)
		{
			tvArr.Add(tv);
		}
	}
	m_pStd->DrawTvArray(&Dom, tvArr);

	// SYMBOL
	if(pBri->m_bSlopeLowerSlab)
	{
		double dCountDraw = 20;
		double dStation = tvL.m_v1.x;
		double dLengthUnit = (tvR.m_v1.x - tvL.m_v1.x) / dCountDraw;

		for(long ix = 0; ix < (long)(dCountDraw); ix++)
		{
			tv.m_v1.x = dStation + dLengthUnit * ix;
			tv.m_v1.y = pBri->GetXyLowerSlabBottomByOffsetFrStation(tv.m_v1.x, 0, 0).y - pExFoot->m_dEFHR;
			tv.m_v2.x = dStation + dLengthUnit * (ix + 1);
			tv.m_v2.y = pBri->GetXyLowerSlabBottomByOffsetFrStation(tv.m_v2.x, 0, 0).y - pExFoot->m_dEFHR;
			tvArrHatch.Add(tv);
		}
	}
	else
	{
		tvArrHatch.RemoveAll();
		tvArrHatch.AddFromTvArray(tvArr);
	}

	CHgBaseDrawStd DrawStd;
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_HATH);
	tvArrHatch.Sort();
	DrawStd.HatchTvArray(&Dom, tvArrHatch, _T("KHAT04"), 1000);

	*pDomP << Dom;
}

void CDrawVertSection::DrawEarthAnchorBoxType(CDomyun *pDomP, BOOL bCutting)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(pBri == NULL || pBri->IsBoxType() == FALSE) return;
	if(pBri->m_nTypeLower != BOXFOOTING_TYPE_ANCHOR) return;
	if(pBri->m_nCountAnchor <= 0)  return;

	CLineInfo *pLine	= pBri->GetLineBase();

	CDomyun Dom(pDomP);
	CConcSymbol sym(m_pStd->m_pARoadOptionStd);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SYMB);

	long nLeft = 0;
	double dStation = pBri->GetStationBridgeStt(FALSE);
	double dStationAnchor = pLine->GetStationByOffsetWidth(dStation, -pBri->m_dWS, pBri->GetAngleJijum(0));

//	double dDia = 0;
	double dOffset = 0;
// 	double dDrawLen = 4000;

	CDPoint xyAnchor(dStationAnchor, 0);
	for(long ix = 0; ix < pBri->m_nCountAnchor; ix++)
	{
		xyAnchor.x += pBri->m_dArrDistJongAnchor.GetAt(ix);
		xyAnchor.y = pBri->GetXyLowerSlabBottomByOffsetFrStation(xyAnchor.x, dOffset, nLeft).y + 100;

		double dDia = pBri->m_dArrDiaAnchor.GetAt(ix);
		double dDrawLen = bCutting ? 3000 : pBri->m_dArrLengthAnchor.GetAt(ix);

		sym.DrawSymbolSteelPile(&Dom, xyAnchor, dDia, dDrawLen, bCutting);
	}

	*pDomP << Dom;
}


void CDrawVertSection::DrawPileBoxType(CDomyun *pDomP, BOOL bCutting)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(pBri == NULL || pBri->IsBoxType() == FALSE) return;
	if(pBri->m_nTypeLower != BOXFOOTING_TYPE_PILE) return;


	CExPileApp *pPile = &pBri->m_footingBox.m_exFooting.m_Pile;
	if(pPile == NULL) return;
	if(pPile->m_nCountOrg <= 0)  return;

	CLineInfo *pLine	= pBri->GetLineBase();

	// 말뚝의 직경이 0보다 작거나 길이가 0보다 작으면 그리지 않고 리턴한다
	if(pPile->m_dDia <= 0 || pPile->m_dLength <= 0) return;

	CDomyun Dom(pDomP);
	CConcSymbol sym(m_pStd->m_pARoadOptionStd);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SYMB);

	double dDia = pPile->m_dDia;
	double dAng = 0;

	long nLeft = 0;
	double dStation = pBri->GetStationBridgeStt(FALSE);

	double dStationPile = pLine->GetStationByOffsetWidth(dStation, -pBri->m_dWS, pBri->GetAngleJijum(0));

	double dOffset = 0;
	double dDrawLen = bCutting ? 3000 : pPile->m_dLength;

	CDPoint xyPile(dStationPile, 0);
	for(long ix = 0; ix < pPile->m_nCountOrg; ix++)
	{
		COrgInfo *pOrg =pPile-> m_pArrOrgInfo.GetAt(ix);
		if(pOrg == NULL) continue;

		xyPile.x += pOrg->dist;
		xyPile.y = pBri->GetXyLowerSlabBottomByOffsetFrStation(xyPile.x, dOffset, nLeft).y + 100;

		sym.DrawSymbolSteelPile(&Dom, xyPile, dDia, dDrawLen, bCutting, dAng);

	}

	*pDomP << Dom;
}

// 하부슬래브 주철근 그리기
void CDrawVertSection::DrawRebarMain_VertSection_LowerSlab( CDomyun *pDomP, long nCycle, long nDan, BOOL bDrawJoint/*=TRUE*/, BOOL bDimRebar/*=TRUE*/, BOOL bMarkRebar/*=TRUE*/, long nDrawLeft/*=0*/, CDRect rectLimit/*=CDRect(0, 0, 0, 0)*/, BOOL bInput/*=TRUE*/, long nVertDir/*=0*/ )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(nCycle == -1) nCycle = pBri->GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > pBri->GetCountCycleMainRebar()-1) return;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;
	if(pBri->IsBoxType() == FALSE) return;

	BOOL bFindOwner	= pBri->m_pARcBridgeDataStd->m_nTypeProcess==TYPE_PROCESS_SLAB_STANDARD_OUTPUT;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	// 하부 슬래브
	// 하면 ///////////////////////////////////////////////////////////////////////
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &pBri->m_pArrRCMainLowerSlab[nCycle][iLOWER][nDan];
	// RebarInfoCycle 그리기
	m_pStd->DrawRebarInfoCycleArray(&Dom, pArrRC, nDrawLeft, rectLimit, !bInput);

	// 이음 그리기
	if(bDrawJoint)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);

		// 이음 정보 가져옴
		CJointArray arrJoint;
		pBri->MakeJointMainRebarLowerSlab(arrJoint, nCycle, nDan, FALSE);

		// 이음 그리기
		m_pStd->DrawJointArray(&Dom, arrJoint, nDrawLeft, rectLimit);
	}

	// 치수기입
	if(bDimRebar)
	{
		DimRebarMain_VertSection_LowerSlab_Lower_Cycle(&Dom, bMarkRebar ? 1 : 0, TRUE, nCycle, nVertDir, nDan);
	}

	// 철근 마크
	if(bMarkRebar)
	{
		double dScalePos	= 0.5;
		switch(nDan)
		{
		case 0: dScalePos	= 0.5; break;
		case 1: dScalePos	= 0.2; break;
		case 2: dScalePos	= 0.8; break;
		}

		m_pStd->MarkRebarInfoCycleArray(&Dom, pArrRC, FALSE, FALSE, pBri->m_dCoverLower_LowerSlab[nDan], 2, 0, nDrawLeft, CDRect(0, 0, 0, 0), dScalePos, bInput, &pBri->m_pArrRebarMain, NULL, bFindOwner);
	}



	// 상면 /////////////////////////////////////////////////////////////////////
	pArrRC = &pBri->m_pArrRCMainLowerSlab[nCycle][iUPPER][nDan];

	// RebarInfoCycle 그리기
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	m_pStd->DrawRebarInfoCycleArray(&Dom, pArrRC, nDrawLeft, rectLimit);

	// 이음 그리기
	if(bDrawJoint)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);

		// 이음 정보 가져옴
		CJointArray arrJoint;
		pBri->MakeJointMainRebarLowerSlab(arrJoint, nCycle, nDan, TRUE);

		// 이음 그리기
		m_pStd->DrawJointArray(&Dom, arrJoint, nDrawLeft, rectLimit);	
	}

	// 치수기입
	if(bDimRebar)
		DimRebarMain_VertSection_LowerSlab_Upper_Cycle1(&Dom, bMarkRebar ? 1 : 0, TRUE, nCycle, nVertDir, nDan);

	// 철근 마크
	if(bMarkRebar)
	{
		double dScalePos	= 0.5;
		switch(nDan)
		{
		case 0: dScalePos	= 0.5; break;
		case 1: dScalePos	= 0.2; break;
		case 2: dScalePos	= 0.8; break;
		}
		m_pStd->MarkRebarInfoCycleArray(&Dom, pArrRC, TRUE, FALSE, pBri->m_dCoverUpper_LowerSlab[nDan], 2, 0, nDrawLeft, CDRect(0, 0, 0, 0), dScalePos, bInput, &pBri->m_pArrRebarMain, NULL, bFindOwner);
	}

	*pDomP << Dom;
}

// 하부슬래브 헌치부 주철근 그리기
void CDrawVertSection::DrawRebarMain_VertSection_LowerSlabHunch( CDomyun *pDomP, long nCycle, BOOL bMarkRebar/*=TRUE*/, long nDrawLeft/*=0*/, CDRect rectLimit/*=CDRect(0, 0, 0, 0)*/, BOOL bInput/*=TRUE*/ )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(!pBri->IsBoxType()) return;

	if(nCycle == -1) nCycle	= pBri->GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > pBri->GetCountCycleMainRebar()-1) return;
	if(pBri->IsBoxType() == FALSE) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	// 상부 슬래브
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &pBri->m_pArrRCMainLowerSlabHunch[nCycle];
	if(!pArrRC) return;

	// RebarInfoCycle 그리기
	m_pStd->DrawRebarInfoCycleArray(&Dom, pArrRC, nDrawLeft, rectLimit);

	if(bMarkRebar)
	{
		m_pStd->MarkRebarInfoCycleArray(&Dom, pArrRC, FALSE, FALSE, pBri->m_dCoverLower_LowerSlab[0], 2, 0, nDrawLeft, CDRect(0, 0, 0, 0), 0.9, bInput, &pBri->m_pArrRebarMain);
	}


	*pDomP << Dom;
}

// 하부슬래브 1단, 1,2사이클, 하면 주철근 치수
void CDrawVertSection::DimRebarMain_VertSection_LowerSlab_Lower_Cycle( CDomyun *pDomP, long nDimDan, BOOL bInput, long nCycle, long nVertDir/*=0*/, long nDan/*=0*/ )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(nCycle == -1) nCycle = pBri->GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > pBri->GetCountCycleMainRebar()-1) return;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;
	if(pBri->IsBoxType() == FALSE) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	BOOL bVertDir	= pBri->IsVertDir();


	// 치수기입용 변수
	CDPointArray xyArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("BOTTOM");
	CDPoint xyStt(0, 0);

	// 치수정보 분석해서 가져옴
	BOOL bApplyEndIs2	= FALSE;
	CDPoint vAngStt			= bVertDir ? CDPoint(0, 1) : pBri->GetAngleJijum(0);
	CDPoint vAngEnd			= bVertDir ? CDPoint(0, 1) : pBri->GetAngleJijum(pBri->m_nQtyJigan);
	double dCoverSideStt	= pBri->GetLengthBynVertDir(pBri->m_dCoverWallOut[0] / vAngStt.y, nVertDir);
	double dCoverSideEnd	= pBri->GetLengthBynVertDir(pBri->m_dCoverWallOut[0] / vAngEnd.y, nVertDir);
	
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &pBri->m_pArrRCMainLowerSlab[nCycle][iLOWER][nDan];

	// 예외사항 처리
	// 1. 시점외측벽체와 상부슬래브가 연결된 형태인 경우 첫번째 철근은 연결 안된 형태로 변형해서 치수 기입한다.
	CRebarInfoCycle rcFirstLowerSlab;
	CRebarInfoCycle *pRCFirstLowerSlab	= NULL;
	if(pArrRC->GetSize() > 0)
	{
		pRCFirstLowerSlab	= pArrRC->GetAt(0);
		rcFirstLowerSlab	= *pRCFirstLowerSlab;
		BOOL bSttRebar	= TRUE;
		if(pBri->IsMainRebarJointLowerSlabAndOutWall(pRCFirstLowerSlab, bSttRebar))
		{
			if(bSttRebar)
			{
				pRCFirstLowerSlab->m_xyStt		= pRCFirstLowerSlab->m_xyMid2;
				pRCFirstLowerSlab->m_xyMid1		= pRCFirstLowerSlab->m_xyEnd;
				pRCFirstLowerSlab->m_xyMid2		= pRCFirstLowerSlab->m_xyMidExt;
				pRCFirstLowerSlab->m_xyEnd		= pRCFirstLowerSlab->m_xyEndExt;
				pRCFirstLowerSlab->m_xyCen		= pRCFirstLowerSlab->m_xyCenExt;
				pRCFirstLowerSlab->m_bUseCenExt	= FALSE;
				pRCFirstLowerSlab->m_bUseCenExt	= FALSE;
				pRCFirstLowerSlab->m_bUseCenExt	= FALSE;
				pRCFirstLowerSlab->m_nPosStt	= REBARINFO_POS_STTWALL_OUTTER;
			}
		}
	}


	if(1)//nCycle == 0 || nCycle == 2)
	{
		bApplyEndIs2	= TRUE;
		m_pStd->GetDimInfoAtRebarInfoCycle(pArrRC, TRUE, xyArrDim, sArrText, FALSE, TRUE, bApplyEndIs2, FALSE, dCoverSideStt, dCoverSideEnd, nVertDir);
	}
	else
	{
		if(pBri->IsJointMainRebarLowerSlab_Lower_Cycle2(-1)) bApplyEndIs2 = TRUE;

		m_pStd->GetDimInfoAtRebarInfoCycle(pArrRC, TRUE, xyArrDim, sArrText, FALSE, FALSE, bApplyEndIs2, FALSE, dCoverSideStt, dCoverSideEnd, nVertDir);
	}

	if(xyArrDim.GetSize() > 0)
	{
		// 가져온 정보를 입력에 맞게 기호로 변경
		if(bInput)
		{
			// 지간 정보 삽입
			long nType	= 0;
			InsertJijumInfo(xyArrDim, sArrText, nType, 0, TRUE, bVertDir);

			// 입력정보로 변경
			long nSize	= sArrText.GetSize();
			CString str	= _T("");
			long nNum	= 1;
			sArrUnderText.Add("");
			long i = 0; for(i = 1; i < nSize-1; i++)
			{
				if(sArrText.GetAt(i) == "1")
				{
					str.Format("LD%d", nNum++);
					sArrText.SetAt(i, str);
					if(bInput)
						sArrUnderText.Add(COMMA2(xyArrDim.GetAt(i).x-xyArrDim.GetAt(i-1).x));
				}
				else
				{
					sArrText.SetAt(i, "0");
					sArrUnderText.Add("");
				}
			}
		}
		else
		{
			sArrText.RemoveAll();
		}

		CDPoint xyStt  = xyArrDim.GetAt(0);
		for(long ix=1; ix<xyArrDim.GetSize(); ix++)
		{
			xyStt.y = min(xyStt.y, xyArrDim.GetAt(ix).y);
		}

		CHgBaseDrawStd baseDraw;
		baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, TRUE);
	}


	// 치수기입을 위해서 변형했던거 복구.
	if(pArrRC->GetSize() > 0 && pRCFirstLowerSlab)
	{
		*pRCFirstLowerSlab = rcFirstLowerSlab;
	}

	*pDomP << Dom;
}

// 하부슬래브 1단, 1사이클, 상면 주철근 치수
void CDrawVertSection::DimRebarMain_VertSection_LowerSlab_Upper_Cycle1( CDomyun *pDomP, long nDimDan, BOOL bInput, long nCycle, long nVertDir/*=0*/, long nDan/*=0*/ )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(nCycle == -1) nCycle = pBri->GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > pBri->GetCountCycleMainRebar()-1) return;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	BOOL bVertDir	= pBri->IsVertDir();


	// 치수기입용 변수
	CDPointArray xyArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("TOP");
	CDPoint xyStt(0, 0);

	// 치수정보 분석해서 가져옴
	BOOL bReverse = FALSE;
	CDPoint vAngStt			= bVertDir ? CDPoint(0, 1) : pBri->GetAngleJijum(0);
	CDPoint vAngEnd			= bVertDir ? CDPoint(0, 1) : pBri->GetAngleJijum(pBri->m_nQtyJigan);
	double dCoverSideStt	= pBri->GetLengthBynVertDir(pBri->m_dCoverWallOut[0] / vAngStt.y, nVertDir);
	double dCoverSideEnd	= pBri->GetLengthBynVertDir(pBri->m_dCoverWallOut[0] / vAngEnd.y, nVertDir);
	m_pStd->GetDimInfoAtRebarInfoCycle(&pBri->m_pArrRCMainLowerSlab[nCycle][iUPPER][nDan], TRUE, xyArrDim, sArrText, FALSE, TRUE, TRUE, bReverse, dCoverSideStt, dCoverSideEnd, nVertDir);

	if(xyArrDim.GetSize() > 0)
	{
		// 2사이클과 4사이클은 시작과 끝에 좌표를 하나 더 넣어 줘야 된다.
		// 왜? 빈공간이라서 치수정보에서 시작과 끝위치가 빠져버리기 때문이지...
		if(0)//nCycle == 1 || nCycle == 3)
		{
			// 피복 정보 제거후 
			xyArrDim.RemoveAt(0);
			xyArrDim.RemoveAt(xyArrDim.GetSize()-1);
			sArrText.RemoveAt(0);
			sArrText.RemoveAt(sArrText.GetSize()-1);


			CTwinVector tvSlab	= pBri->GetTvOffset_LowerSlab(0, TRUE, -1, -1);

			// 시작위치
			xyArrDim.InsertAt(0, tvSlab.m_v1);
			sArrText.InsertAt(0, "0");
			sArrText.SetAt(1, "1");

			// 끝위치
			xyArrDim.Add(tvSlab.m_v2);
			sArrText.Add("1");

			// 피복 정보 추가한다.
			// 처음과 끝부분에 피복 정보 추가한다. ////////////////
			CDPoint xy1	= xyArrDim.GetAt(0);
			CDPoint xy2	= xyArrDim.GetAt(xyArrDim.GetSize()-1);

			xy1 = xy1 + (xy1-xy2).Unit() * dCoverSideStt;
			xyArrDim.InsertAt(0, xy1);
			sArrText.InsertAt(0, "0");

			xy2	= xy2 + (xy2-xy1).Unit() * dCoverSideEnd;
			xyArrDim.Add(xy2);
			sArrText.Add("0");
			////////////////////////////////////////////////////////			
		}

		// 가져온 정보를 입력에 맞게 기호로 변경
		if(bInput)
		{
			// 지간 정보 삽입
			long nType	= 0;
			InsertJijumInfo(xyArrDim, sArrText, nType, nCycle, FALSE, bVertDir);

			// 입력정보로 변경
			long nSize	= sArrText.GetSize();
			CString str	= _T("");
			long nNum	= 1;
			sArrUnderText.Add("");
			long i = 0; for(i = 1; i < nSize-1; i++)
			{
				if(sArrText.GetAt(i) == "1")
				{
					str.Format("UD%d", nNum++);
					sArrText.SetAt(i, str);
					sArrUnderText.Add(COMMA2(xyArrDim[i].x-xyArrDim[i-1].x));
				}
				else
				{
					sArrText.SetAt(i, "0");
					sArrUnderText.Add("");
				}
			}
		}
		else
		{
			sArrText.RemoveAll();
		}

		CDPoint xyStt  = xyArrDim.GetAt(0);
		for(long ix=1; ix<xyArrDim.GetSize(); ix++)
		{
			xyStt.y = max(xyStt.y, xyArrDim.GetAt(ix).y);
		}

		CHgBaseDrawStd baseDraw;
		baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE, TRUE, FALSE, FALSE, TRUE);
	}

	*pDomP << Dom;
}

void CDrawVertSection::DimRebarMainAssm_VertSection_BoxOutSide( CDomyun *pDomP, long nDimDan, long nCycle, long nDrawLeft/*=0*/, long nDan/*=0*/ )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(nCycle == -1) nCycle = pBri->GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > pBri->GetCountCycleMainRebar()-1) return;	
	if(!pBri->m_bDetailInput23DanMainRebar)
		nDan	= 0;

	CDPoint xyStt(0, 0),
		xyEnd(0, 0);
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> arrRC;

	// 모든 외측 rc 결합 ////////////
	pBri->m_Util.AddRebarInfoCycle(&arrRC, &pBri->m_pArrRCMainOutWall[nCycle][iSTT][iOUTTER][nDan], FALSE);
	pBri->m_Util.AddRebarInfoCycle(&arrRC, &pBri->m_pArrRCMainUpperSlab[nCycle][iUPPER][nDan], FALSE);
	pBri->m_Util.AddRebarInfoCycle(&arrRC, &pBri->m_pArrRCMainOutWall[nCycle][iEND][iOUTTER][nDan], TRUE);
	pBri->m_Util.AddRebarInfoCycle(&arrRC, &pBri->m_pArrRCMainLowerSlab[nCycle][iLOWER][nDan], TRUE);

	// 시점측과 종점측 철근의 시작과 끝부분중 라운드부 제거
	long nSize	= arrRC.GetSize();
	if(nSize > 0)
	{
		// 하부슬래브 철근이 일체형일 경우 반을 잘라서 두개로 만들어 준다
		// 그래야 치수가 이쁘게 나옴
		long i = 0; for(i = 0; i < arrRC.GetSize(); i++)
		{
			CRebarInfoCycle *pRC	= arrRC.GetAt(i);
			if(pBri->IsMainRebarJointUpperSlabAndUpperSlab(pRC))
			{
				CRebarInfoCycle *pRCNext	= new CRebarInfoCycle;
				pRCNext->m_xyStt	= pRC->m_xyEnd;
				pRCNext->m_xyMid1	= pRCNext->m_xyStt;
				pRCNext->m_xyMid2	= pRC->m_xyMidExt;
				pRCNext->m_xyCen	= pRC->m_xyCenExt;
				pRCNext->m_xyEnd	= pRC->m_xyEndExt;
				pRCNext->m_R_Mid	= pRC->m_R_Mid;

				pRCNext->m_nPosStt	= REBARINFO_POS_UPPERSLAB_UPPER;
				pRCNext->m_nPosEnd	= REBARINFO_POS_ENDWALL_OUTTER;
				arrRC.InsertAt(i+1, pRCNext);

				pRC->m_nPosEnd	= REBARINFO_POS_UPPERSLAB_UPPER;
			}
			if(pBri->IsMainRebarJointLowerSlabAndLowerSlab(pRC))
			{
				CRebarInfoCycle *pRCNext	= new CRebarInfoCycle;
				pRCNext->m_xyStt	= pRC->m_xyEnd;
				pRCNext->m_xyMid1	= pRCNext->m_xyStt;
				pRCNext->m_xyMid2	= pRC->m_xyMidExt;
				pRCNext->m_xyCen	= pRC->m_xyCenExt;
				pRCNext->m_xyEnd	= pRC->m_xyEndExt;
				pRCNext->m_R_Mid	= pRC->m_R_Mid;

				pRCNext->m_nPosStt	= REBARINFO_POS_LOWERSLAB_LOWER;
				pRCNext->m_nPosEnd	= REBARINFO_POS_STTWALL_OUTTER;
				arrRC.InsertAt(i+1, pRCNext);

				pRC->m_nPosEnd	= REBARINFO_POS_LOWERSLAB_LOWER;
			}
		}

		xyStt	= arrRC.GetAt(0)->m_xyStt;
		CRebarInfoCycle *pRC	= arrRC.GetAt(arrRC.GetSize()-1);
		if(pRC->m_nPosStt == pRC->m_nPosEnd)
			xyEnd	= pRC->m_xyMid1;
		else
			xyEnd	= pRC->m_xyEnd;

		CDomyun Dom(pDomP);
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		m_pStd->DimRebarInfoCycleArray(&Dom, &arrRC, xyStt, xyEnd, TRUE, nDimDan, nDrawLeft, FALSE, TRUE, TRUE);

		*pDomP << Dom;
	}

	AhTPADelete(&arrRC, (CRebarInfoCycle*)0);
}

void CDrawVertSection::DrawRebarSupport_VertSection_LowerSlab( CDomyun *pDomP, BOOL bUpper, long nDan, BOOL bMarkRebar, BOOL bDimRebar, long nDimDan, BOOL bInput, CDRect rectLimit/*=CDRect(0, 0, 0, 0)*/, long nVertDir/*=0*/, BOOL bCalSupRebar/*=TRUE*/ )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(pBri->IsBoxType() == FALSE) return;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	// 철근 가져와서 그리기
	CHgBaseDrawStd baseDraw;
	CDPointArray xyArr;
	pBri->GetXyRebarSupport_VertSection_LowerSlab(xyArr, bUpper, nDan, nVertDir, bCalSupRebar);

	// rectLimit 적용
	if(rectLimit != CDRect(0, 0, 0, 0))
	{
		CTwinVectorArray tvArrLimit;
		CVectorArray vecArrLimit;
		tvArrLimit.SetFromRect(rectLimit);
		tvArrLimit.GetToVectorArray(vecArrLimit);

		CRebarPlacing rb;
		rb.GetXyByVectorArrayArea(xyArr, vecArrLimit, TRUE);
	}

	CDPoint xyDirScale	= bUpper ? CDPoint(0, 1) : CDPoint(0, -1);
	baseDraw.DrawPointRebar(&Dom, xyArr, xyDirScale);

	long upper	= bUpper ? iUPPER : iLOWER;
	BOOL bVertDir	= nVertDir == 0 ? pBri->IsVertDir() : (nVertDir == -1 ? TRUE : FALSE);
	CTwinVector tvLower	= pBri->GetTvOffset_LowerSlab(0, bUpper, -1, -1, bVertDir);
	// 치수 기입
	if(bDimRebar)
	{
		if(xyArr.GetSize() > 0)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

			CDPointArray xyArrDim;
			CDPoint xyStt	= xyArr.GetAt(0);
			CStringArray sArrText, sArrUnderText;
			CString sDir	= bUpper ? "TOP" : "BOTTOM";

			// 치수에 피복과 R정보 추가해야 됨. //////////////////////
			CTwinVector tvUpperStt, tvUpperEnd;
			CRebarInfoCycle *pRCStt	= NULL,
				*pRCEnd	= NULL;

			// 정보 추가(시작부)
			if(pBri->m_tvArrVertSection.GetTvByInfo("시점벽체(부상방지저판)", tvUpperStt))
			{
				xyArrDim.Add(bUpper ? tvUpperStt.m_v2 + CDPoint(0, pBri->m_dTS_Lower) : tvUpperStt.m_v2);	sArrText.Add("0"); sArrUnderText.Add("0");
				xyStt.y	= bUpper ? tvUpperStt.m_v2.y + pBri->m_dTS_Lower : tvUpperStt.m_v2.y;
			}
			else if(pBri->m_tvArrVertSection.GetTvByInfo("시점벽체좌측면", tvUpperStt))
			{
				xyArrDim.Add(bUpper ? tvUpperStt.m_v1 + CDPoint(0, pBri->m_dTS_Lower) : tvUpperStt.m_v1);	sArrText.Add("0"); sArrUnderText.Add("0");
				xyStt.y	= bUpper ? tvUpperStt.m_v1.y + pBri->m_dTS_Lower : tvUpperStt.m_v1.y;
			}

			pRCStt	= pBri->GetRebarInfoCycleLowerSlab(0, 0, FALSE, 0);
			if(pRCStt)
			{
				BOOL bSttRebar	= TRUE;
				if(pBri->IsMainRebarJointLowerSlabAndOutWall(pRCStt, bSttRebar))
				{
					xyArrDim.Add(pRCStt->m_xyEnd); sArrText.Add("0"); sArrUnderText.Add("0");
				}
				else
				{
					xyArrDim.Add(tvLower.m_v1); sArrText.Add("0"); sArrUnderText.Add("0");
				}
			} 

			// 치수 기입 정보 가져옴
			CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_SupportRebar_LowerSlab[upper][nDan];
			m_pStd->GetDimInfoAtPlaceRebarByBlock(xyArr, xyArrDim, sArrText, sArrUnderText, pPlace, bUpper ? "U" : "L", xyStt, CDPoint(1, 0), bInput, TRUE);

			// 정보 추가(끝부)
			pRCEnd	= pBri->GetRebarInfoCycleLowerSlab(0, 0, FALSE, -1);
			if(pRCEnd)
			{
				long nCntJoint =0;
				for (long nJi =0; nJi < pBri->m_nQtyJigan; nJi++)
					nCntJoint += pBri->GetCountJointMainRebarLowerSlab(0,nDan, bUpper, nJi);
				if(bUpper)
				{
					xyArrDim.Add(pRCEnd->m_xyEnd); sArrText.Add("0"); sArrUnderText.Add("0");
				}
				else if(pBri->IsBoxType() && nCntJoint == 0)
				{
					xyArrDim.Add(pRCEnd->m_xyEndExt); sArrText.Add("0"); sArrUnderText.Add("0");
				}
				else
				{
					if(bUpper)
					{
						xyArrDim.Add(pRCEnd->m_xyMid1); sArrText.Add("0"); sArrUnderText.Add("0");
					}

					xyArrDim.Add(pRCEnd->m_xyMid2); sArrText.Add("0"); sArrUnderText.Add("0");
				}
				
			}
			if(pBri->m_tvArrVertSection.GetTvByInfo("종점벽체(부상방지저판)", tvUpperEnd))
			{
				xyArrDim.Add(bUpper ? tvUpperEnd.m_v2 + CDPoint(0, pBri->m_dTS_Lower) : tvUpperEnd.m_v2);	sArrText.Add("0"); sArrUnderText.Add("0");
				xyStt.y	= bUpper ? max(tvUpperEnd.m_v2.y + pBri->m_dTS_Lower, xyStt.y) : min(tvUpperEnd.m_v2.y, xyStt.y);
			}
			else if(pBri->m_tvArrVertSection.GetTvByInfo("종점벽체우측면", tvUpperEnd))
			{
				xyArrDim.Add(bUpper ? tvUpperEnd.m_v2 + CDPoint(0, pBri->m_dTS_Lower) : tvUpperEnd.m_v2);	sArrText.Add("0"); sArrUnderText.Add("0");
				xyStt.y	= bUpper ? max(tvUpperEnd.m_v2.y + pBri->m_dTS_Lower, xyStt.y) : min(tvUpperEnd.m_v2.y, xyStt.y);
			}

			if(!bInput)
			{
				//sArrText.RemoveAll();
				sArrUnderText.RemoveAll();
			}

			// 치수 기입
			if(xyArrDim.GetSize() > 1)
			{
				xyStt.x	= xyArrDim.GetAt(0).x;
				baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, bUpper ? nDimDan : nDimDan+1, sDir, -1, bUpper==FALSE, FALSE, FALSE);
			}
		}
	}


	// 마킹
	CString sDescription	= _T("");
	if(bUpper)
		sDescription.Format("하부슬래브 상면 %d단 배력철근2", nDan+1);
	else
		sDescription.Format("하부슬래브 하면 %d단 배력철근", nDan+1);
	CString sMark	= pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescription);

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
			sMark	= pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescription);

			if(sMark == _T("")) continue;
			else	break;
		}

	}


	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);

		//#0028210
		{
			CRebarPlacing rp;

			CRebar *pRb = pBri->GetRebarSupportByDescription(sDescription);
			//표준 단면 배근 - 배력철근입력에 입력된 좌표의 이격거리에서 최대값을 구함.
			CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_SupportRebar_LowerSlab[upper][0];

			CDPointArray xySupp;

			if(pPlace != NULL) xySupp = pPlace->m_xyArr;

			xySupp.Sort(FALSE, FALSE);

			double dMaxDist = (xySupp.GetSize() == 0) ? 0 : xySupp.GetAt(0).y;

			//배치된 ctc보다 큰 이격거리가 나오게 되면 마킹을 따로 해준다. 마킹이 길어져 깨져보인다.
			CTypedPtrArray<CObArray, CDPointArray*> xyDArr;

			CDPointArray *pXyArr = NULL;

			if(xySupp.GetSize() > 0) pXyArr = new CDPointArray;

			for(long ix = 0; ix < xyArr.GetSize() - 1; ix ++)
			{
				double dLen = rp.GetDistPointToPoint(xyArr[ix], xyArr[ix+1]);

				if(Compare(dLen, dMaxDist + 1, ">", 0.001)) //최대값보다 큰값이 있으면 마킹을 하나더 한다.
				{
					xyDArr.Add(pXyArr);

					pXyArr = new CDPointArray;

					continue;
				}

				pXyArr->Add(xyArr[ix]);
			}

			if(pXyArr && pXyArr->GetSize() > 0) xyDArr.Add(pXyArr);

			for(long ix = 0; ix < xyDArr.GetSize(); ix ++)
			{
				pXyArr = xyDArr.GetAt(ix);

				mark.SetRebar(*pXyArr);

				mark.m_sMark					= sMark;
				mark.m_sDia						= pBri->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(ePartLowerSlab));
				mark.m_nTypeMarkPointUpAndDown	= bUpper ? TYPE_MARKPOINT_UPPER : TYPE_MARKPOINT_LOWER;
				mark.m_nTypeMarkDir				= TYPE_MARKDIR_LEFT;
				mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
				mark.m_dPosScaleMarkStt			= 0.3;
				mark.m_dCtc = pBri->GetCTCRebarSupport_Slab(FALSE, bUpper, nDan, TRUE);
				mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;

				mark.MarkRebar();

				*pDomP << Dom;
			}
			AhTPADelete(&xyDArr, (CDPointArray*)0);
		}

	}

	*pDomP << Dom;
}

void CDrawVertSection::DrawRebarShear_VertSection_LowerSlab( CDomyun *pDomP, BOOL bMarkRebar, BOOL bDimRebar, BOOL bInput, CDRect rectLimit/*=CDRect(0, 0, 0, 0)*/, long nVertDir/*=0*/ )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(pBri->IsBoxType() == FALSE) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);

	// 철근 가져와서 그리기
	CTwinVectorArray tvArr, tvArrHunch;
	pBri->GetTvRebarShear_VertSection_LowerSlab(tvArr, tvArrHunch, nVertDir);

	// rectLimit 적용
	if(rectLimit != CDRect(0, 0, 0, 0))
	{
		CTwinVectorArray tvArrLimit;
		CVectorArray vecArrLimit;
		tvArrLimit.SetFromRect(rectLimit);
		tvArrLimit.GetToVectorArray(vecArrLimit);

		CRebarPlacing rb;
		rb.GetTvArrayByCrossVectorArrayArea(tvArr, vecArrLimit, TRUE);
		rb.GetTvArrayByCrossVectorArrayArea(tvArrHunch, vecArrLimit, TRUE);
	}

	// 전단 철근 그리기
	m_pStd->DrawTvArrayWidthDashDot(&Dom, tvArr, FALSE);
	m_pStd->DrawTvArrayWidthDashDot(&Dom, tvArrHunch, FALSE);

	// 마크시 리브부는 2 헌치부는 1 아닌거는 0
	for(long hunch = 0; hunch < 2; hunch++)
	{
		CTwinVectorArray *pTvArr;//	= bHunch ? &tvArrHunch : &tvArr;
		CString sDescription	= _T("");
		switch(hunch)
		{
		case 0 : 
			pTvArr	= &tvArr; 
			sDescription	= "하부슬래브 전단철근1";
			break;
		case 1 : 
			pTvArr	= &tvArrHunch; 
			sDescription	= "하부슬래브 전단철근2";
			break;
		}


		CString sMark	= pBri->GetMarkRebarShear(0, bInput, FALSE, FALSE, sDescription);
		if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
		{
			double dTS	= pBri->m_dTS_Lower;
			// 철근진행방향이 반대로 되어야 함.
			CRebarPlacing rb;
			CTwinVectorArray tvArrTmp;
			tvArrTmp	= *pTvArr;
			//rb.ReverseRebar(tvArrTmp);

			CRebar *pRb = pBri->GetRebarShearByDescription(sDescription);
			CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_ShearRebar_LowerSlab;
			CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);

			CTwinVectorArray tvTmp;
			CTwinVector tvWall	= pBri->GetTvOffset_LowerSlab(0, FALSE);
			if(hunch == 1)
			{
				tvWall.m_v1.y += dTS/4;
				tvWall.m_v2.y += dTS/4;
			}
			tvTmp.Add(tvWall);
			tvWall	= pBri->GetTvOffset_LowerSlab(0, TRUE);
			if(hunch == 0)
			{
				tvWall.m_v1.y -= dTS/4;
				tvWall.m_v2.y -= dTS/4;
			}
			tvTmp.Add(tvWall);

			mark.SetRebar(tvArrTmp, tvTmp);
			// 설정
			mark.m_sMark	= sMark;
			mark.m_sDia		= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(ePartLowerSlab,TRUE));
			mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_POS;	// 지름 표시위치 찾는 방법
			mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;	// TYPE_MARKPOS_POS일때 옵션
			mark.m_dPosScaleMarkStt			= pBri->m_nQtyJigan == 1 ? 0.8 : 0.6;
			mark.m_nTypeMarkDir				= TYPE_MARKDIR_LEFTTAN;
			mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_LOWER;
			switch(hunch)
			{
			case 0 : mark.m_dAddHeight	= dTS/2 - dTS/4/3; break;
			case 1 : mark.m_dAddHeight	= dTS/2 + dTS/4/4; break;
			}
			mark.m_dCtc = pBri->GetCTCRebarShear_LowerSlab();
			mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;

			mark.MarkRebar();
		}
	}

	if(bDimRebar)
	{
		// 치수 기입할때는 헌치부와 아닌거를 합쳐야 된다.
		// 합쳐서 정렬도 해야 겠지?(정
		CTwinVectorArray tvArrTot;
		tvArrTot.AddFromTvArray(tvArr);
		tvArrTot.AddFromTvArray(tvArrHunch);

		long nSize	= tvArrTot.GetSize();
		CDPointArray xyArr;
		long i = 0; for(i = 0; i < nSize; i++)
			xyArr.Add(tvArrTot.GetAt(i).m_v1);
		xyArr.Sort(TRUE);

		if(nSize > 0)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

			CDPointArray xyArrDim;
			CDPoint xyStt	= tvArrTot.GetAt(0).m_v1;
			CStringArray sArrText, sArrUnderText;
			CString sDir	= _T("BOTTOM");

			xyStt.y = tvArrTot.GetXyBottom().y;

			// 치수에 피복과 R정보 추가해야 됨. //////////////////////
			CTwinVector tvStt, tvEnd;

			// 정보 추가(시작부)
			if(pBri->m_tvArrVertSection.GetTvByInfo("시점벽체(부상방지저판)", tvStt))
			{
				xyArrDim.Add(tvStt.m_v2); sArrText.Add("0"); sArrUnderText.Add("0");
				xyStt	= xyArrDim.GetAt(0);
			}
			else if(pBri->m_tvArrVertSection.GetTvByInfo("시점벽체좌측면", tvStt))
			{
				xyArrDim.Add(tvStt.m_v1); sArrText.Add("0"); sArrUnderText.Add("0");
				xyStt	= xyArrDim.GetAt(0);
			}
			xyStt.y = tvArrTot.GetXyBottom().y;

			// 치수 기입 정보 가져옴
			CRebarPlacing rb;
			CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_ShearRebar_LowerSlab;
			m_pStd->GetDimInfoAtPlaceRebarByBlock(xyArr, xyArrDim, sArrText, sArrUnderText, pPlace, "V", xyStt, CDPoint(1, 0), bInput, TRUE);

			// 정보 추가(끝부)
			if(pBri->m_tvArrVertSection.GetTvByInfo("종점벽체(부상방지저판)", tvEnd))
			{
				xyArrDim.Add(tvEnd.m_v2); sArrText.Add("0"); sArrUnderText.Add("0");
			}
			else if(pBri->m_tvArrVertSection.GetTvByInfo("종점벽체우측면", tvEnd))
			{
				xyArrDim.Add(tvEnd.m_v2); sArrText.Add("0"); sArrUnderText.Add("0");
			}

			// 치수 기입
			if(xyArrDim.GetSize() > 1)
			{
				xyStt	= xyArrDim.GetAt(0);
				xyStt.y = tvArrTot.GetXyBottom().y;
				CHgBaseDrawStd baseDraw;
				baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, 0, sDir, -1, FALSE, FALSE, FALSE);
			}
		}
	}

	*pDomP << Dom;
}

//단면에서 부상방지저판주위만. 
void CDrawVertSection::DrawBFootingSection(CDomyun *pDomP, BOOL bStt)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CHgBaseDrawStd baseDraw;
	baseDraw.m_pLine	= pBri->GetLineBase();
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	CString strStt = bStt ? _T("시점") : _T("종점");
	double sw = bStt ? -1.0 : 1.0;

	CTwinVectorArray tvArrPRF;
	CTwinVector tvTop, tvBottom, tvSide;
	
	pBri->GetTvPRF_VertSection(tvArrPRF, bStt, 0, TRUE);
	tvArrPRF.GetTvByInfo(strStt + _T("부상방지저판상면"), tvTop);
	tvArrPRF.GetTvByInfo(strStt + _T("부상방지저판바깥측면"), tvSide);
	tvArrPRF.GetTvByInfo(strStt + _T("부상방지저판하단"), tvBottom);

	CDRect rect;
	double dSta = pBri->GetStationOnJijum(bStt ? 0 : pBri->m_nQtyJigan);

	//자를 영역 구하기
	if(bStt)
	{
		rect.left = tvSide.m_v1.x + sw * 50;
		rect.right = dSta + sw * -1000;
	}
	else
	{
		rect.right = tvSide.m_v1.x + sw * 50;
		rect.left =  dSta + sw * -1000;
	}
	rect.top = tvTop.m_v2.y + 1000;
	rect.bottom = min(tvBottom.m_v1.y, tvBottom.m_v2.y) - 50;

	pBri->GetTvVertSection_Std(0, TRUE, FALSE);
	m_pStd->DrawCuttingTvArray(&Dom, pBri->m_tvArrVertSection, rect, TRUE);
	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE);
	*pDomP << Dom;
}

CDRect CDrawVertSection::DrawBFootingRebar(CDomyun *pDom, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, BOOL bMainRebar, BOOL bSupportRebar, BOOL bShearRebar, BOOL bInput)
{
	CDRect rectBFooting(0,0,0,0);
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return rectBFooting;
	if(!pBri->m_bExistProtectionRiseFooting) return rectBFooting;

	CDomyun Dom(pDom);
	CHgBaseDrawStd DrawStd;

	// 주철근
	if(bMainRebar)
	{
		DrawRebarMain_VertSection_BFooting(&Dom, bStt, bDimRebar, bMarkRebar, bInput);
	}

	// 수평철근
	if(bSupportRebar)
	{
		DrawRebarSupport_VertSection_BFooting(&Dom, bStt, bDimRebar, bMarkRebar, TRUE);
	}
	
	// 전단철근 그리기
	if(bSupportRebar)
	{
		DrawRebarShear_VertSection_BFooting(&Dom, bStt, bDimRebar, bMarkRebar, TRUE, TRUE, 0);
	}

	Dom.SetCalcExtRect();
	rectBFooting = Dom.GetExtRect();
	*pDom << Dom;
	
	return rectBFooting;
	
}
void CDrawVertSection::DrawRebarSupport_VertSection_LowerSlabHunch( CDomyun *pDomP, long nIdxHunch, BOOL bMarkRebar, BOOL bDimRebar, BOOL bInput, CDRect rectLimit/*=CDRect(0, 0, 0, 0)*/ )
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(pBri->IsBoxType() == FALSE) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	// 철근 가져와서 그리기
	CDPointArray xyArrLeft, xyArrRight;
	pBri->GetXyRebarSupport_VertSection_LowerSlabHunch(xyArrLeft, xyArrRight, nIdxHunch);

	// rectLimit 적용
	if(rectLimit != CDRect(0, 0, 0, 0))
	{
		CTwinVectorArray tvArrLimit;
		CVectorArray vecArrLimit;
		tvArrLimit.SetFromRect(rectLimit);
		tvArrLimit.GetToVectorArray(vecArrLimit);

		CRebarPlacing rb;
		rb.GetXyByVectorArrayArea(xyArrLeft, vecArrLimit, TRUE);
		rb.GetXyByVectorArrayArea(xyArrRight, vecArrLimit, TRUE);
	}

	CHgBaseDrawStd baseDraw;
	baseDraw.DrawPointRebar(&Dom, xyArrLeft);
	baseDraw.DrawPointRebar(&Dom, xyArrRight);

	// 치수 기입
	if(bDimRebar)
	{

	}


	// 마킹
	CDPointArray xyArrTot;
	long i = 0; for(i = 0; i < xyArrLeft.GetSize(); i++)
		xyArrTot.Add(xyArrLeft.GetAt(i));
	for(i = 0; i < xyArrRight.GetSize(); i++)
		xyArrTot.Add(xyArrRight.GetAt(i));
	CString sDescription	= _T("");
	sDescription.Format("하부슬래브 헌치부 배력철근");
	CString sMark	= pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescription);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		CRebar *pRb = pBri->GetRebarSupportByDescription(sDescription);
		CPlaceRebarByBlock *pPlace		= &pBri->m_placeByBlock_SupportRebar_LowerSlab[iUPPER][0];
		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
		mark.SetRebar(xyArrTot);

		// 설정
		mark.m_sMark					= sMark;
		mark.m_sDia						= pBri->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(ePartLowerSlab));
		mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_UPPER;
		mark.m_nTypeMarkDir				= nIdxHunch==0 ? TYPE_MARKDIR_RIGHT : TYPE_MARKDIR_LEFT;
		mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
		mark.m_dPosScaleMarkStt			= 0.5;
		mark.m_dCtc = pBri->GetCTCRebarSupport_Slab(FALSE, TRUE, 0, TRUE);
		mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;
		mark.MarkRebar();
	}


	*pDomP << Dom;
}

void CDrawVertSection::DimVerSectionLower(CDomyun *pDomP, long nDimDan)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_BOX) return;

	CDomyun Dom(pDomP);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	//하부슬래브 치수
	Dom.SetDirection(_T("BOTTOM"));
	CFootingApp *pFooting = (pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)? &pBri->m_footingBox : &pBri->m_footingEnd;

	BOOL bApplyAngle = TRUE;
	CExFootingApp *pExFoot = &pBri->m_footingBox.m_exFooting;

	CTwinVector tv;
	CTwinVectorArray *pTvArr = &pExFoot->m_tvArrFront;

	double dPRFW = pBri->m_bExistProtectionRiseFooting ? pBri->m_dPRF_W : 0;

	double dWS = pBri->IsOutWall(TRUE) ? pBri->m_dWS : 0;
	CDPoint vAngS = pBri->GetAngleJijum(0);
	vAngS = CDPoint(Round(vAngS.x, 5), Round(vAngS.y, 5));
	double dStaStt	= pBri->m_dStationBridgeStt-(dWS/vAngS.y) - dPRFW - pFooting->m_exFooting.m_dEFWL;

	BOOL bPile = pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_PILE;
	double dDrawLen = bPile ? 3000 : Dom.Always(5);

	Dom.DimMoveToExt(dStaStt, pFooting->m_dEL - pFooting->GetHeight() - dDrawLen, 0, COMMA(pFooting->m_exFooting.m_dEFWL));
	Dom.DimLineTo(pFooting->m_exFooting.m_dEFWL, 0, _T(""));
	Dom.DimLineTo(dPRFW, 0, COMMA(dPRFW));

	double dWidth = 0;
	if(pTvArr->GetTvByInfo("확대기초상단", tv))
	{
		dWidth += tv.GetHorLength(bApplyAngle);
		dWidth -= dPRFW*2;
	}
	Dom.DimLineTo(dWidth, 0, COMMA(dWidth));

	Dom.DimLineTo(dPRFW, 0, COMMA(dPRFW));
	Dom.DimLineToOver(pFooting->m_exFooting.m_dEFWR, 0, COMMA(pFooting->m_exFooting.m_dEFWR), 1);

	*pDomP << Dom;
}