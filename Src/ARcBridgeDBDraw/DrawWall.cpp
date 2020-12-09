// DrawWall.cpp: implementation of the CDrawWall class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARcBridgeDBDraw.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawWall::CDrawWall()
{

}

CDrawWall::~CDrawWall()
{

}

void CDrawWall::DrawFront(CDomyun *pDomP, CWallApp *pWall, BOOL bDrawPattern, CTwinVectorArray *tvArrReturn, BOOL bDrawConstJoint)
{
	if(!pWall) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	// 기둥식일 경우 기둥의 무늬만 살짝 그려준다.
	if(pWall->m_bTopIsColumn)
	{
		m_pStd->m_pDrawColumn->DrawFront(&Dom, &pWall->m_Column, FALSE, FALSE, !bDrawPattern);
	}
	else
	{
		m_baseDraw.DrawTvArray(&Dom, pWall->m_tvArrFront, m_pStd->IsViewVectorInfo(), FALSE, 0);
		if(bDrawConstJoint)	// 시공 이음(시공 순서도에서만 필요)
		{
			if(pWall->m_bIs && !pWall->m_bNotRebar)
			{
				*pDomP << Dom;
				CTwinVector tvLeft, tvRight;
				pWall->m_tvArrFront.GetTvByInfo(_T("벽체좌측면"), tvLeft);
				pWall->m_tvArrFront.GetTvByInfo(_T("벽체우측면"), tvRight);

				CDPoint xyMatch(0, 0);
				GetXyMatchLineAndLine(tvLeft.m_v1, tvLeft.GetXyDir().Rotate90(), tvRight.m_v1, tvRight.GetXyDir(), xyMatch);
				double dW = ~(CDPoint(tvLeft.m_v1) - xyMatch);
				CDPoint xy	= tvLeft.m_v1 + (tvLeft.GetXyDir() * tvLeft.GetLength()/2);
				m_pStd->DrawConstJointHor(&Dom, xy, dW);
				Dom.Rotate(xy, ToDPointFrDegree(pWall->GetAngleWallByHor()-90));

				CDPoint xyNum = tvLeft.m_v1 + (tvLeft.GetXyDir() * tvLeft.GetLength()/4);
				xyNum += tvLeft.GetXyDir().RotateInv90() * dW/2;
				Dom.SetTextHeight(2);
				Dom.TextCircle(xyNum.x, xyNum.y, _T("2"), 3, FALSE, TRUE);
			}
		}

		if(bDrawPattern && pWall->IsRoundRectWall())
		{
			pWall->m_Column.GetTvFront(TRUE, FALSE);
			m_baseDraw.DrawTvArray(&Dom, pWall->m_Column.m_tvArrFront);
		}
	}

	tvArrReturn->ChangePos(pWall->m_tvArrFront);

	*pDomP << Dom;
}

// long nUpper : -1 벽체 하면만 그림, 0 : 상하면 모두 그림, 1 : 상면만 그림
// long nFront : - 1 후면, 1 전면, 0 모두
void CDrawWall::DrawPlan(CDomyun *pDomP, CWallApp *pWall, BOOL bCenterLine, BOOL bHidden, long nUpper, long nFront, CTwinVectorArray &tvArrCen)
{
	if(!pWall) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, bHidden ? HCAD_CONH : HCAD_CONC);

	CTwinVectorArray tvArrPlane;
	
	if(nUpper != 0 && !pWall->m_bTopIsColumn)
	{
		CString str	= nUpper == -1 ? "벽체하면" : "벽체상면";
		long i = 0; for(i = 0; i < pWall->m_tvArrPlan.GetSize(); i++)
		{
			CTwinVector tv	= pWall->m_tvArrPlan.GetAt(i);
			if(tv.m_sInfo.Find(str, 0) != -1)
				tvArrPlane.Add(tv);
		}
	}
	else
	{
		tvArrPlane	= pWall->m_tvArrPlan;
	}

	// 상면은 점선, 하면은 실선으로 그림
	if(nFront != 0)
	{
		CRebarPlacing rb;
		CTwinVectorArray tvArr;
		tvArr	= tvArrPlane;

		rb.TrimTvArrayByTvArray(tvArr, tvArrCen, nFront == -1 ? FALSE : TRUE, FALSE);
		m_pStd->DrawTvArray(&Dom, tvArr, FALSE, FALSE);
	}
	else
		m_baseDraw.DrawTvArray(&Dom, tvArrPlane);

	// 중심선 그림
	if(bCenterLine)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
		double dMargin = 500;
		CTwinVector tv;
		for(long upper = 0; upper < 2; upper++)
		{
			BOOL bUpper	= upper == iUPPER;
			if(nUpper != 0)
			{
				if(bUpper && nUpper == -1) continue;
				if(!bUpper && nUpper == 1) continue;
			}
			else
			{
				if(!bUpper) continue;
			}
				
			tv	= bUpper ? pWall->m_tvPathPlan : pWall->m_tvPathPlanLower;
			CDPoint xyDir = tv.GetXyDir();
			tv.m_v1 -= xyDir*dMargin;
			tv.m_v2 += xyDir*dMargin;

			Dom.LineTo(tv.m_v1, tv.m_v2);
		}
	}

	*pDomP << Dom;
}


void CDrawWall::DimRightPlane(CDomyun *pDomP, CWallApp *pWall, long nDimDan, BOOL bInput, BOOL bDimForWallPlane)
{
	if(!pWall) return;
	if(!pWall->m_bIs) return;
	if(nDimDan < 0) nDimDan = 0;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	
	CDoubleArray dArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("RIGHT");
	CString str = _T("");
	CDPoint xyStt(0, 0);
	CTwinVector tv = pWall->m_tvPathPlan;
	CDPoint xyDir = tv.GetXyDir();
	
	xyStt = tv.m_v1;
	
	dArrDim.Add(pWall->m_dMarginRight);
	sArrText.Add("0");
	sArrUnderText.Add("0");
	
	// 기둥식일때.
	if(pWall->m_bTopIsColumn && pWall->m_nCountColumn > 0)
	{
		if(pWall->m_bRevDirPlacingColumn)
		{
			GetSwap(tv.m_v1, tv.m_v2);
			xyStt	= tv.m_v1;
			xyDir	= xyDir * -1;
		}
		double dLast = 0;
		long i = 0; for(i = 0; i < pWall->m_nCountColumn; i++)
		{
			str.Format("CL%d", i+1);
			dArrDim.Add(pWall->m_dDistColumn[i]);
			sArrText.Add(str);
			sArrUnderText.Add(COMMA(pWall->m_dDistColumn[i]));
			dLast += pWall->m_dDistColumn[i];
		}

		dArrDim.Add(pWall->GetTvWallTopCen().GetLength()-dLast);
	}
	else
	{
		dArrDim.Add(pWall->GetTvWallTopCen().GetLength());
	}
	dArrDim.Add(pWall->m_dMarginRight);

	if(!bInput)
	{
		sArrText.RemoveAll();
		sArrUnderText.RemoveAll();
	}

	CDomyun Dom2(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom2, HCAD_DIML);

	if(bDimForWallPlane)
	{
		Dom2.SetDirection("RIGHT");
		Dom2.DimMoveTo(xyStt.x, xyStt.y, 0, "");
		Dom2.DimLineTo(pWall->m_dW, 0, COMMA(pWall->m_dW));
		Dom2.Rotate(xyStt, CDPoint(0, 1));
	}

	CDPoint xyRotate = ToDPointFrRadian(ToRadian(xyDir.GetAngleDegree()-90));
	m_baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, !bInput, FALSE);
	Dom.Rotate(xyRotate.GetAngleRadian(), xyStt.x, xyStt.y);
	Dom2.Rotate(xyRotate.GetAngleRadian(), xyStt.x, xyStt.y);

	CDPoint xyMove = xyRotate*pWall->m_dW/2;
	Dom.Move(xyMove.x, xyMove.y);
	xyMove += tv.GetXyDir() * tv.GetLength();
	Dom2.Move(xyMove.x, xyMove.y);
	
	*pDomP << Dom;
	*pDomP << Dom2;
}

void CDrawWall::DrawRebarMain_Front(CDomyun *pDomP, CWallApp *pWall, long nCycle, long nDan, BOOL bLeft, BOOL bMarkRebar, BOOL bDimRebar, long nDrawLeft, BOOL bDrawJoint, BOOL bInput, BOOL bForStandardSection)
{
	if(!pWall) return;
	if(!pWall->m_bIs) return;
	if(pWall->m_bNotRebar) return;

	if(nCycle == -1) nCycle = pWall->GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > pWall->GetCountCycleMainRebar()-1) return;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;

	long left	= bLeft ? iLEFT : iRIGHT;
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &pWall->m_pArrRCMain[nCycle][left][nDan];

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	// RebarInfoCycle 그리기
	m_pStd->DrawRebarInfoCycleArray(&Dom, pArrRC, nDrawLeft, CDRect(0, 0, 0, 0), !bInput);

	// 이음 그리기
	if(bDrawJoint)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);

		// 이음 정보 가져옴
		CJointArray arrJoint;
		pWall->MakeJointMainRebar(arrJoint, nCycle, bLeft, nDan);
		
		// 이음 그리기
		m_pStd->DrawJointArray(&Dom, arrJoint, nDrawLeft, CDRect(0, 0, 0, 0));
	}

	// 마킹
	if(bMarkRebar)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		double dCover	= bLeft ? pWall->m_dCoverLeft[nDan] : pWall->m_dCoverRight[nDan];
		double dScalePos	= 0.5;
		switch(nDan)
		{
			case 0: dScalePos	= 0.5; break;
			case 1: dScalePos	= 0.2; break;
			case 2: dScalePos	= 0.8; break;
		}
		m_pStd->MarkRebarInfoCycleArray(&Dom, pArrRC, bLeft, TRUE, dCover, 2, 0, nDrawLeft, CDRect(0, 0, 0, 0), dScalePos, bInput, &pWall->m_pArrRebarMain);

		// 기초에 들어가는 철근중 R이 있으면 R표기 해준다
		if(pArrRC->GetSize() > 0 && !bForStandardSection)
		{
			CRebarInfoCycle *pRC	= pArrRC->GetAt(0);
			m_pStd->MarkRadiusRebarInfoCycle(&Dom, pRC, CDRect(0, 0, 0, 0));
		}
	}

	if(bDimRebar)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		DimRebarMain_Front(&Dom, pWall, 1, TRUE, nCycle, nDan);
	}

	*pDomP << Dom;

}


// 종단면도 벽체 배력철근
void CDrawWall::DrawRebarSupport_Front(CDomyun *pDomP, CWallApp *pWall, BOOL bLeft, long nDan, BOOL bMarkRebar, BOOL bDimRebar, BOOL bInput, BOOL bVertDir)
{
	if(!pWall) return;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;
	if(pWall->m_bNotRebar) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	// 철근 가져와서 그리기
	CDPointArray xyArr;
	pWall->GetXyRebarSupport_Front(xyArr, bLeft, nDan, TRUE, bVertDir);
	m_baseDraw.DrawPointRebar(&Dom, xyArr, bLeft ? CDPoint(-1, 0) : CDPoint(1, 0), -1, -1);
	*pDomP << Dom;

	long left	= bLeft ? iLEFT : iRIGHT;
	
	// 치수 기입
	// 치수기입이나 마크할때는 철근들을 모두 수직으로 세워놓고 해야 됨.
	CDPoint xyOrg	= CDPoint(0, 0);
	CRebarPlacing rb;
	double dAng	= 90 - pWall->GetAngleWallByHor(!bVertDir);// ->m_dAngle;
	if(dAng != 0) rb.RotateRebar(xyArr, xyOrg, dAng);

	if(bDimRebar)
	{
		// 라운드식 벽체는 치수 기입할때 긴쪽에만 기입해야 된다.
		BOOL bDimOK	= TRUE;
		if(pWall->IsRoundRectWall())
		{
			
			if(pWall->m_tvArrLowerLimit_FrontRebar.GetSize() == 0 && !bLeft)
				bDimOK	= FALSE;
			else if(pWall->m_tvArrLowerLimit_FrontRebar.GetSize() > 0)
			{
				CTwinVector tvWallL	= pWall->m_tvArrFront.GetTvByInfo("벽체좌측면");
				CTwinVector tvWallR	= pWall->m_tvArrFront.GetTvByInfo("벽체우측면");
				if(tvWallL.GetXyBottom().y < tvWallR.GetXyBottom().y)
					bDimOK	= bLeft ? TRUE : FALSE;
				else
					bDimOK	= bLeft ? FALSE : TRUE;
			}
		}

		if(xyArr.GetSize() > 0 && bDimOK)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

			CTwinVector tvWall	= pWall->GetTvOffset_Front(0, 0, FALSE, FALSE, bLeft, bVertDir);
			if(dAng != 0) tvWall = rb.RotateTv(tvWall, xyOrg, dAng);

			CDPointArray xyArrDim;
			CDPoint xyStt	= xyArr.GetAt(0);
			CStringArray sArrText, sArrUnderText;
			CString sDir	= bLeft ? _T("LEFT") : _T("RIGHT");

			// 치수에 기초하면 피복과 벽체철근 라운드 상단부정보 추가함. //////////////////////
			CTwinVector tvLower;
			CRebarInfoCycle *pRCUpper = NULL;
			CFootingApp *pFooting	= &pWall->m_footing;
			
			CDPoint xyTmp	= xyStt;
			if(pFooting->m_bIs || pFooting->IsVirtualFooting())
			{
				BOOL bVertDir	= pFooting->IsVertDir();
				CTwinVector tvLower	= pFooting->m_tvArrFront.GetTvByInfo("기초하면");
				if(dAng != 0) tvLower = rb.RotateTv(tvLower, xyOrg, dAng);
				GetXyMatchLineAndLine(tvWall.m_v1, tvWall.GetXyDir(), tvLower.m_v1, tvLower.GetXyDir(), tvLower.m_v1);
				xyArrDim.Add(tvLower.m_v1); sArrText.Add("0"); sArrUnderText.Add("0");

				tvLower	= pFooting->GetTvOffset_Front(0, FALSE, bVertDir);
				if(dAng != 0) tvLower = rb.RotateTv(tvLower, xyOrg, dAng);
				GetXyMatchLineAndLine(tvWall.m_v1, tvWall.GetXyDir(), tvLower.m_v1, tvLower.GetXyDir(), tvLower.m_v1);
				xyArrDim.Add(tvLower.m_v1); sArrText.Add("0"); sArrUnderText.Add("0");

				CRebarInfoCycle *pRC	= pWall->GetRebarInfoCycle(0, 0, bLeft, 0);
				if(pRC)
				{
					CDPoint xyMid2	= pRC->m_xyMid2;
					if(dAng != 0) rb.RotateXy(xyMid2, xyOrg, dAng);
					xyArrDim.Add(xyMid2); sArrText.Add("0"); sArrUnderText.Add("0");
				}

				xyTmp	= xyArrDim.GetAt(xyArrDim.GetSize()-1);
			}
			else if(pWall->m_tvArrLowerLimit_FrontRebar.GetSize() > 0)
			{
				tvLower	= pWall->m_tvLowerLimitOfMainRebar;

				CDPointArray xyArrMatch;
				rb.GetXyMatchTvArrAndLine(pWall->m_tvArrLowerLimit_FrontRebar, tvWall.m_v1, tvWall.GetXyDir(), xyArrMatch);
				if(xyArrMatch.GetSize() > 0)
				{
					tvWall.m_v1 = xyArrMatch.GetAt(0);
					xyArrDim.Add(tvWall.m_v1);
					sArrText.Add("0");
					sArrUnderText.Add("0");
				}

				if(pWall->m_bRebarLowerSettle[left])
				{
					CRebarInfoCycle *pRC	= pWall->GetRebarInfoCycle(0, 0, bLeft, 0);
					if(pRC)
					{
						CDPoint xyMid2	= pRC->m_xyMid2;
						if(dAng != 0) rb.RotateXy(xyMid2, xyOrg, dAng);
						xyArrDim.Add(xyMid2); sArrText.Add("0"); sArrUnderText.Add("0");
					}
				}
				else
				{
					xyArrDim.Add(tvWall.m_v1+CDPoint(0, pWall->m_dCoverLower_LowerSlab));
					sArrText.Add("0");
					sArrUnderText.Add("0");
				}

				xyTmp	= xyArrDim.GetAt(xyArrDim.GetSize()-1);
			}
			
			// 치수 기입 정보 가져옴
			CPlaceRebarByBlock *pPlace	= &pWall->m_placeByBlock_SupportRebar[left][nDan];
			m_pStd->GetDimInfoAtPlaceRebarByBlock(xyArr, xyArrDim, sArrText, sArrUnderText, pPlace, bLeft || pWall->IsRoundRectWall() ? "L" : "R", xyTmp, tvWall.GetXyDir(), bInput, TRUE);
			

			// 정보 추가(끝부)
			pRCUpper	= pWall->GetRebarInfoCycle(0, 0, bLeft, -1);
			if(pRCUpper)
			{
				CDPoint xyEnd	= pRCUpper->m_xyEnd;
				if(dAng != 0) rb.RotateXy(xyEnd, xyOrg, dAng);
				xyArrDim.Add(xyEnd); sArrText.Add("0"); sArrUnderText.Add("0");
			}

			// 피복
			CTwinVectorArray tvArrUpperLimit_Front;
			tvArrUpperLimit_Front	= pWall->m_tvArrUpperLimit_Front;
			if(dAng != 0)
				rb.RotateTvArr(tvArrUpperLimit_Front, xyOrg, dAng);
			CDPointArray xyArrMatch;
			rb.GetXyMatchTvArrAndLine(tvArrUpperLimit_Front, tvWall.m_v1, tvWall.GetXyDir(), xyArrMatch);
			if(xyArrMatch.GetSize() > 0)
			{
				xyArrDim.Add(xyArrMatch.GetAt(0)); sArrText.Add("0"); sArrUnderText.Add("0");
			}

			if(sArrUnderText.GetSize() > 0 && pWall->GetDanMainRebar(0, bLeft) > 1)
			{
				long nSize	= sArrUnderText.GetSize();
				CString sDan	= _T("");
				sDan.Format("%s(%d단)", bLeft ? "좌측" : "우측", nDan+1);
				sArrUnderText.SetAt(nSize-1, sDan);
			}


			// 치수 기입
			if(xyArrDim.GetSize() > 1)
			{
				xyStt	= xyArrDim.GetAt(0);
				CTwinVector tvLower	= pWall->m_footing.m_tvArrFront.GetTvByInfo("기초하면");
				CTwinVector tvLeft	= pWall->m_footing.m_tvArrFront.GetTvByInfo("기초좌측면");
				CTwinVector tvRight	= pWall->m_footing.m_tvArrFront.GetTvByInfo("기초우측면");
				if(!pWall->m_footing.m_bIs && !pWall->m_footing.IsVirtualFooting())
				{
					tvLeft = pWall->m_tvArrFront.GetTvByInfo(_T("벽체좌측면"));
					tvRight = pWall->m_tvArrFront.GetTvByInfo(_T("벽체우측면"));
					tvLower = pWall->m_tvLowerLimitOfMainRebar;
					GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), tvLeft.m_v1, tvLeft.GetXyDir(), tvLower.m_v1);
					GetXyMatchLineAndLine(tvLower.m_v2, tvLower.GetXyDir(), tvRight.m_v1, tvRight.GetXyDir(), tvLower.m_v2);
				}
				if(dAng != 0) tvLower = rb.RotateTv(tvLower, xyOrg, dAng);
				if(dAng != 0) tvLeft = rb.RotateTv(tvLeft, xyOrg, dAng);
				if(dAng != 0) tvRight = rb.RotateTv(tvRight, xyOrg, dAng);
				if(pWall->m_nType == WALL_TYPE_SLOPE)
				{
					CDPoint xyMatch(0, 0);
					if(GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), tvWall.m_v1, tvWall.GetXyDir(), xyMatch))
						xyStt.x	= xyMatch.x;
					else
						xyStt.x = bLeft ? tvLeft.GetXyTop().x : tvRight.GetXyBottom().x;
				}
				else
				{
					if(pWall->m_footing.m_bIs || pWall->m_footing.IsVirtualFooting())
					{
						xyStt.x	= bLeft ? tvLower.GetXyLeft().x : tvLower.GetXyRight().x;
					}
					else
						xyStt.x	= tvWall.m_v1.x;
				}
				m_baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDan+1, sDir, -1, FALSE, FALSE, FALSE, TRUE, TRUE);
				if(dAng != 0)
					Dom.Rotate(xyOrg, ToDPointFrDegree(-dAng));
				*pDomP << Dom;
			}
		}
	}


	// 마킹
	CString sDecription	= _T("");
	if(pWall->IsRoundRectWall())
		sDecription.Format("%d단 배력철근", nDan+1);
	else
		sDecription.Format("%s %d단 배력철근", bLeft ? "좌측" : "우측", nDan+1);
	CString sMark	= pWall->GetMarkRebarSupport(0, bInput, FALSE, -1, sDecription);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		CRebar *pRb = pWall->GetRebarSupportByDescription(sDecription);
		CPlaceRebarByBlock *pPlace		= &pWall->m_placeByBlock_SupportRebar[left][nDan];
		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
		mark.SetRebar(xyArr);

		// 설정
		mark.m_sMark					= sMark;
		mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pWall->m_Fy);
		mark.m_nTypeMarkPointUpAndDown	= bLeft ? TYPE_MARKPOINT_UPPER : TYPE_MARKPOINT_LOWER;
		mark.m_nTypeMarkDir				= TYPE_MARKDIR_RIGHTTAN;
		mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
		mark.m_dPosScaleMarkStt			= 0.5;
		mark.m_dCtc = pWall->GetCTCRebarSupport(bLeft, nDan, TRUE);
		mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;

		mark.MarkRebar();
	}

	if(dAng != 0)
		Dom.Rotate(xyOrg, ToDPointFrDegree(-dAng));
	*pDomP << Dom;
}
// long nVertDir : -1 : 직거리, 0 : 자동(원래설정), 1 : 사거리
void CDrawWall::DrawRebarShear_Front(CDomyun *pDomP, CWallApp *pWall, long nDimDan, BOOL bMarkRebar, BOOL bDimRebar, BOOL bDimTotal, BOOL bInput, BOOL bVertDir)
{
	if(!pWall) return;
	if(pWall->m_bNotRebar) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);

	// 철근 가져와서 그리기
	CTwinVectorArray tvArr;
	pWall->GetTvRebarShear_Front(tvArr, bVertDir);

	// 전단철근 그리기
	m_pStd->DrawTvArrayWidthDashDot(&Dom, tvArr, FALSE);
	*pDomP << Dom;

	// 치수기입이나 마크할때는 철근들을 모두 수직으로 세워놓고 해야 됨.
	CDPoint xyOrg	= CDPoint(0, 0);
	CRebarPlacing rb;
	double dAng	= 90 - pWall->GetAngleWallByHor(!bVertDir);
	if(dAng != 0) rb.RotateTvArr(tvArr, xyOrg, dAng);

	CString sDecription	= _T("전단철근");
	CString sMark	= pWall->GetMarkRebarShear(0, bInput, FALSE, -1, sDecription);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		CRebar *pRb = pWall->GetRebarShearByDescription(sDecription);
		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);

		CTwinVectorArray tvTmp;
		CTwinVector tvWall	= pWall->GetTvOffset_Front(0, 0, FALSE, FALSE, TRUE, bVertDir);
		if(dAng != 0) tvWall = rb.RotateTv(tvWall, xyOrg, dAng);
		tvTmp.Add(tvWall);
		tvWall	= pWall->GetTvOffset_Front(0, 0, FALSE, FALSE, FALSE, bVertDir);
		if(dAng != 0) tvWall = rb.RotateTv(tvWall, xyOrg, dAng);
		tvTmp.Add(tvWall);
		
		CPlaceRebarByBlock *pPlace		= &pWall->m_placeByBlock_ShearRebar;
		mark.SetRebar(tvArr, tvTmp);
		// 설정
		mark.m_sMark					= sMark;
		mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pWall->m_FyShear);
		mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_POS;
		mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
		mark.m_nTypeMarkDir				= TYPE_MARKDIR_RIGHT;
		mark.m_dPosScaleMarkStt			= 0.5;

		// 외측벽체 주철근과 피하기 위해
		// 주철근 첫번째 이음있는 곳에 마크한다.(주철근 마크가 이음위치에 올 확률은 거의 0에 가깝기 때문 한 100사이클 되면 올지도 모름.. ㅋㅋ)
		CRebarInfoCycle *pRC	= pWall->GetRebarInfoCycle(0, 0, FALSE, 0);
		CRebarInfoCycle *pRCNext	= pWall->GetRebarInfoCycle(0, 0, FALSE, 1);
		if(pRC)
		{
			double dLenJoint	= pRCNext ? pWall->GetValueJoint(pRC, pRCNext->m_Dia) : 0;
			CDRect rect	= tvArr.GetRect();
			mark.m_dPosScaleMarkStt			= fabs((pRC->m_xyEnd.y - dLenJoint/2) - rect.bottom) / fabs(rect.Height());

			// 혹시나 이음이 없어서 마크위치가 중간을 넘어간다 싶으면 그냥 밑으로 내려서 마크한다.
			if(mark.m_dPosScaleMarkStt >= 0.5 || mark.m_dPosScaleMarkStt < 0)
				mark.m_dPosScaleMarkStt = 0.25;
		}
		mark.m_dAddHeight	= pWall->m_dW/2;
		mark.m_dCtc = pWall->GetCTCRebarShear();
		mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;

		mark.MarkRebar();

		if(dAng != 0)
			Dom.Rotate(xyOrg, ToDPointFrDegree(-dAng));

		*pDomP << Dom;
	}

	if(bDimRebar)
	{
		long nSize	= tvArr.GetSize();
		if(tvArr.GetSize() > 0)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

			CDPointArray xyArrDim;
// 			CDPoint xyStt	= tvArr.GetAt(0).m_v1;
			CStringArray sArrText, sArrUnderText;
			CString sDir	= _T("LEFT");

			// 치수에 피복과 R정보 추가해야 됨. //////////////////////
			CFootingApp *pFooting	= &pWall->m_footing;
			CTwinVector tvWall		= pWall->GetTvOffset_Front(0, 0, FALSE, FALSE, TRUE, bVertDir);
			if(dAng != 0) tvWall = rb.RotateTv(tvWall, xyOrg, dAng);
			CTwinVector tvLower;

			if((pFooting->m_bIs || pFooting->IsVirtualFooting()) && pFooting->m_tvArrFront.GetTvByInfo("기초하면", tvLower))
			{
				if(dAng != 0) tvLower = rb.RotateTv(tvLower, xyOrg, dAng);
				GetXyMatchLineAndLine(tvWall.m_v2, tvWall.GetXyDir(TRUE), tvLower.m_v1, tvLower.GetXyDir(), tvWall.m_v1);
			}
			else if(pWall->m_tvArrLowerLimit_FrontRebar.GetSize() > 0)
			{
				CDPointArray xyArrMatch;
				CRebarPlacing rb;
				rb.GetXyMatchTvArrAndLine(pWall->m_tvArrLowerLimit_FrontRebar, tvWall.m_v1, tvWall.GetXyDir(), xyArrMatch);
				if(xyArrMatch.GetSize() > 0)
				{
					tvWall.m_v1	= xyArrMatch.GetAt(0);
//					tvWall.m_v1.y += pWall->m_dCoverLower_LowerSlab;
				}
			}

			if(pWall->m_tvUpperLimitOfMainRebar.GetLength() > 0)
			{
				CTwinVector tvUpperLimitOfMainRebar	= pWall->m_tvUpperLimitOfMainRebar;
				if(dAng != 0) tvUpperLimitOfMainRebar = rb.RotateTv(tvUpperLimitOfMainRebar, xyOrg, dAng);
				GetXyMatchLineAndLine(tvWall.m_v1, tvWall.GetXyDir(), tvUpperLimitOfMainRebar.m_v1, tvUpperLimitOfMainRebar.GetXyDir(), tvWall.m_v2);
			}
						
			// 정보 추가(시작부)
			if(1)//pFooting->m_bIs)
			{
				xyArrDim.Add(tvWall.m_v1); sArrText.Add("0"); sArrUnderText.Add("0");
			}
			
			// 치수 기입 정보 가져옴
			CRebarPlacing rb;
			CDPointArray xyArr;
			long i = 0; for(i = 0; i < nSize; i++)
				xyArr.Add(tvArr.GetAt(i).m_v1);
			
			CPlaceRebarByBlock *pPlace	= &pWall->m_placeByBlock_ShearRebar;
			m_pStd->GetDimInfoAtPlaceRebarByBlock(xyArr, xyArrDim, sArrText, sArrUnderText, pPlace, "V", xyArrDim.GetAt(0), tvWall.GetXyDir(), bInput, TRUE);

			// 정보 추가(끝부)
			xyArrDim.Add(tvWall.m_v2);
			sArrText.Add("0"); sArrUnderText.Add("0");

				
			// 치수 기입
			if(xyArrDim.GetSize() > 1)
			{
// 				xyStt	= tvWall.m_v1;
				CDPoint xyStt	= xyArrDim.GetAt(0);
				CTwinVector tvLower	= pWall->m_footing.m_tvArrFront.GetTvByInfo("기초하면");
				CTwinVector tvLeft	= pWall->m_footing.m_tvArrFront.GetTvByInfo("기초좌측면");
				if(dAng != 0) tvLower = rb.RotateTv(tvLower, xyOrg, dAng);
				if(dAng != 0) tvLeft = rb.RotateTv(tvLeft, xyOrg, dAng);
				if(pWall->m_nType == WALL_TYPE_SLOPE)
				{
					CDPoint xyMatch(0, 0);
					if(GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), tvWall.m_v1, tvWall.GetXyDir(), xyMatch))
						xyStt.x	= xyMatch.x;
					else
						xyStt.x = tvLeft.GetXyTop().x;
				}
				else
				{
					if(pWall->m_footing.m_bIs || pWall->m_footing.IsVirtualFooting())
					{
						xyStt.x	= tvLower.GetXyLeft().x;
					}
					else
					{
						xyStt.x	= tvWall.m_v1.x;
					}
				}
				m_baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, bDimTotal, FALSE, TRUE, TRUE, TRUE);

				if(dAng != 0)
					Dom.Rotate(xyOrg, ToDPointFrDegree(-dAng));

				*pDomP << Dom;
			}
		}
	}

	*pDomP << Dom;
}

// 벽체 좌측면도 그리기
// long nDrawLeft : -1 : 좌측면 그림, 1 : 우측면 그림, 0 : 모두 그림
// 경사형 벽체일때를 제외하고는 nDrawLeft는 사실 의미가 없음
// BOOL bHiddenSide : 측면은 가각부 적용을 위해 점선으로 그릴지?
// BOOL bCorrectionEL : 헌치가 없이 슬래브가 경사일때 벽체높이를 경사에의한 높이를 보정해준다.
void CDrawWall::DrawLeftSide(CDomyun *pDomP, CWallApp *pWall, BOOL bIncludeFooting, BOOL bIncludeUpperSlab, BOOL bLeft, long nDrawLeft, BOOL bDrawCutting, BOOL bHiddenSide, BOOL bDrawPattern, BOOL bOnlyViewFromLeft, BOOL bCorrectionEL)
{
	if(!pWall) return;
	
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	CTwinVectorArray tvArrLeft;
	pWall->GetTvLeftSide(tvArrLeft, FALSE, bIncludeFooting, bIncludeUpperSlab, bLeft, nDrawLeft, FALSE, TRUE, bOnlyViewFromLeft, bDrawPattern, bCorrectionEL);

	CTwinVectorArray tvArrHidden;
	if(bHiddenSide)
	{
		long nIdx	= tvArrLeft.GetIdxByInfo("벽체좌측");
		if(nIdx > -1 && pWall->m_dWidthExp[iLEFT] > 0)
		{
			tvArrHidden.Add(tvArrLeft.GetAt(nIdx));
			tvArrLeft.RemoveAt(nIdx);
		}
		nIdx	= tvArrLeft.GetIdxByInfo("벽체우측");
		if(nIdx > -1 && pWall->m_dWidthExp[iRIGHT] > 0)
		{
			tvArrHidden.Add(tvArrLeft.GetAt(nIdx));
			tvArrLeft.RemoveAt(nIdx);
		}

		// 가각부 적용을 하면 상면도 그릴 필요가 없다.
		long i = 0; for(i = 0; i < tvArrLeft.GetSize(); i++)
		{
			if(tvArrLeft.GetAt(i).m_sInfo == "벽체상면" || tvArrLeft.GetAt(i).m_sInfo == "벽체상면끝")
			{
				tvArrLeft.RemoveAt(i);
				i--;
			}
		}
	}


	for(long hid = 0; hid < 2; hid++)
	{
		CTwinVectorArray tvArr;
		tvArr	= hid == 0 ? tvArrLeft : tvArrHidden;
		if(tvArr.GetSize() == 0) continue;

		if(hid == 1)
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONH);

		if(bDrawCutting)
		{
			double dHeight	= Dom.Always(Dom.GetDimLevelDis()*3+Dom.GetDimExo()+Dom.GetDimVLen());
			CDRect rect	= tvArr.GetRect();
			CDRect rectCut	= rect;
			rectCut.top = rectCut.bottom + dHeight;
			rectCut.bottom -= 100;
			rectCut.left -= 100;
			rectCut.right += 100;

			m_pStd->DrawCuttingTvArray(&Dom, tvArr, rectCut, TRUE);

			if(bDrawPattern && pWall->IsRoundRectWall())
			{
				pWall->m_Column.GetTvLeftSide(tvArr, bDrawPattern);
				m_pStd->DrawCuttingTvArray(&Dom, tvArr, rectCut, TRUE);
			}
		}
		else
		{
			m_baseDraw.DrawTvArray(&Dom, tvArr, m_pStd->IsViewVectorInfo());

			if(bDrawPattern && pWall->IsRoundRectWall())
			{
				// 기둥은 무조건 중심을 기준으로 그리는데 반해 라운드벽체는 일반벽체 처럼 좌우측 모두를 그리므로
				// 아치교 처럼 하면이 수평이 아닌 벽체는 기둥에서 가져온 패턴을 그대로 사용할 수 없다.
				// 따라서 벽체의 하면까지 뻗줌.
				pWall->m_Column.GetTvLeftSide(tvArr, TRUE);
				if(pWall->m_tvArrLowerLimit_Front.GetSize() > 0)
				{
					CTwinVector tvLower	= tvArrLeft.GetTvByInfo("벽체하면");
					long i = 0; for(i = 0; i < tvArr.GetSize(); i++)
					{
						CTwinVector tv	= tvArr.GetAt(i);
						if(tv.GetVerLength() > 0)
						{
							GetXyMatchSegAndLine(tvLower.m_v1, tvLower.m_v2, tv.m_v1, tv.GetXyDir(), tv.m_v1);
							tvArr.SetAt(i, tv);
						}
					}
					CTwinVector tvLowerL	= tvArr.GetTvByInfo("기둥하면좌측"),
								tvLowerR	= tvArr.GetTvByInfo("기둥하면우측");

					tvLowerL.m_v1.y	= tvLower.m_v1.y;
					tvLowerL.m_v2.y	= tvLower.m_v1.y;
					tvLowerR.m_v1.y	= tvLower.m_v1.y;
					tvLowerR.m_v2.y	= tvLower.m_v1.y;;
					tvArr.SetTvByInfo("기둥하면좌측", tvLowerL);
					tvArr.SetTvByInfo("기둥하면우측", tvLowerR);
				}
				m_pStd->DrawTvArray(&Dom, tvArr);
			}
		}
	}



	*pDomP << Dom;
}

// 벽체 좌측면 치수
void CDrawWall::DimLeftPlane(CDomyun *pDomP, CWallApp *pWall, BOOL bInput)
{
	if(!pWall) return;
	if(!pWall->m_bIs) return;
	
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	
	CDoubleArray dArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir	= _T("BOTTOM");
	CString str		= _T("");
	CDPoint xyStt	= CDPoint(0, 0);
	long nDimDan	= 0;


	// 기둥식일땐 기둥간격 치수
	CDPoint xyLeft	= CDPoint(0, 0);
	CDPoint xyRight	= CDPoint(0, 0);
	long nSize	= pWall->m_tvArrLeftUpper.GetSize();
	if(nSize > 0)
	{
		xyLeft		= pWall->m_tvArrLeftUpper.GetAt(0).m_v1;
		xyRight		= pWall->m_tvArrLeftUpper.GetAt(nSize-1).m_v2;
	}
	else
	{
		xyLeft		= CDPoint(0, pWall->m_xyOrg.y);
		xyRight		= CDPoint(pWall->m_tvPathPlan.GetLength(), xyLeft.y);
	}
	xyStt	= CDPoint(xyLeft.x, min(xyLeft.y-pWall->m_dThickUppeSlab, xyRight.y-pWall->m_dThickUppeSlab));

	if(pWall->m_bTopIsColumn)
	{
		double dTot			= 0;
		long nCountColumn	= pWall->m_nCountColumn;
		for(long col = 0; col < nCountColumn; col++)
		{
			str.Format("CL%d", col+1);
			dArrDim.Add(pWall->m_dDistColumn[col]);
			sArrText.Add(str);
			dTot	+= pWall->m_dDistColumn[col];
		}

		dArrDim.Add(pWall->m_tvPathPlan.GetLength() - dTot);
		sArrText.Add("0");

		if(!bInput)
			sArrText.RemoveAll();

		m_baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, FALSE, TRUE, TRUE, TRUE);
	}
	else
	{
		double dLen	= xyRight.x - xyLeft.x;
		Dom.SetDirection(sDir);
		Dom.DimMoveTo(xyLeft.x, xyLeft.y, nDimDan);
		Dom.DimLineTo(dLen, nDimDan, COMMA(dLen));
	}


	*pDomP << Dom;

}

// 벽체 정면도 좌측치수
void CDrawWall::DimLeftFront(CDomyun *pDomP, CWallApp *pWall)
{
	if(!pWall) return;
	if(!pWall->m_bIs) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	if(pWall->m_nType == WALL_TYPE_SLOPE)
	{
		long left = 0; for(left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT;
			CString str	= bLeft ? "벽체좌측면" : "벽체우측면";
			CTwinVector tv	= pWall->m_tvArrFront.GetTvByInfo(str);

			double dLen	= tv.GetLength();
			Dom.SetDirection(bLeft ? "LEFT" : "RIGHT");
			Dom.DimMoveTo(tv.m_v1.x, tv.m_v1.y, 0);
			Dom.DimLineTo(dLen, 0, COMMA(dLen));
			Dom.Rotate(tv.m_v1, ToDPointFrDegree(tv.GetXyDir().GetAngleDegree()-90));
			*pDomP << Dom;
		}
		
	}
	else
	{
		Dom.SetDirection("LEFT");
		
		CDRect rect			= pWall->m_tvArrFront.GetRect();
		double dH			= rect.top - rect.bottom;
		Dom.DimMoveTo(rect.left, rect.bottom, 0);
		Dom.DimLineTo(dH, 0, COMMA(dH));
	}
	
	*pDomP << Dom;
}

// 내측 벽체 
// 현재 기초하면부터 치수 기입하게 되어 있으나, 정차장님으로 부터 기초상면부터 기입할 것을 주문받음
// 그런데 주철근 조립도는 철근의 길이에 대한 치수가 있어야 될 꺼 같아서 일단 놔둠
// 
void CDrawWall::DimRebarMainAssm_Front(CDomyun *pDomP, CWallApp *pWall, long nDimDan, long nCycle, BOOL bLeft, long nDrawLeft, long nDan)
{
	if(!pWall) return;
	if(!pWall->m_bIs) return;
	if(nCycle == -1) nCycle = pWall->GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > pWall->GetCountCycleMainRebar()-1) return;	
	if(pWall->m_bNotRebar) return;
	if(!pWall->m_bDetailInput23DanMainRebar)
		nDan	= 0;

	CDPoint xyStt(0, 0),
			xyEnd(0, 0);
	long left	= bLeft ? iLEFT : iRIGHT;
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC	= &pWall->m_pArrRCMain[nCycle][left][nDan];

	long nSize	= pArrRC->GetSize();
	if(nSize > 0)
	{
		BOOL bAllNotExist	= TRUE;
		long i = 0; for(i = 0; i < nSize; i++)
		{
			if(pArrRC->GetAt(i)->m_bExist)
			{
				bAllNotExist	= FALSE;
				break;
			}
		}

		if(bAllNotExist) return;

		// 첫번째꺼 라운드부 편집
		CRebarInfoCycle *pRC	= pArrRC->GetAt(0);
		if(pRC->m_R_Mid > 0)
		{
			// 라운드 부는 직선으로 분리해서 추가
			CRebarInfoCycle *pRCPrev	= new CRebarInfoCycle;;
			pRCPrev->m_nPosStt		= pRC->m_nPosEnd;
			pRCPrev->m_nPosEnd		= pRC->m_nPosEnd;
			GetXyMatchLineAndLine(pRC->m_xyStt, (pRC->m_xyMid1-pRC->m_xyStt).Unit(), pRC->m_xyEnd, (pRC->m_xyEnd-pRC->m_xyMid2).Unit(), pRCPrev->m_xyStt);
			pRCPrev->m_xyMid1		= pRC->m_xyMid2;
			pRCPrev->m_xyMid2		= pRCPrev->m_xyMid1;
			pRCPrev->m_xyEnd		= pRCPrev->m_xyMid1;
			pArrRC->InsertAt(0, pRCPrev);

			pRC->m_nPosStt			= pRC->m_nPosEnd;
			pRC->m_xyStt			= pRC->m_xyMid2;
			pRC->m_xyMid1			= pRC->m_xyEnd;

			pArrRC->SetAt(1, pRC);
		}
		else if(pWall->m_footing.m_bIs || pWall->m_footing.IsVirtualFooting())
		{
			// Box형과 같이 기초가 없는경우는 옮겨줄 필요가 없지.
			pRC->m_nPosStt	= pRC->m_nPosEnd;
			pRC->m_xyStt	= pRC->m_xyMid1;
			pRC->m_xyMid1	= pRC->m_xyEnd;
			pRC->m_xyMid2	= pRC->m_xyEnd;
		}

		xyStt	= pArrRC->GetAt(0)->m_xyStt;

		pRC		= pArrRC->GetAt(pArrRC->GetSize()-1);
		xyEnd	= pRC->m_xyMid1;

		CRebarPlacing rb;
		CDPointArray xyArrMatch;
		rb.GetXyMatchTvArrAndLine(pWall->m_tvArrUpperLimit_Front, xyEnd, (pRC->m_xyMid1-pRC->m_xyStt).Unit(), xyArrMatch);
		if(xyArrMatch.GetSize() > 0)
			xyEnd	= xyArrMatch.GetAt(0);
		
		
		// 시작위치를 기초하면으로 설정
		CTwinVector tv;
		if(pWall->m_footing.m_bIs || pWall->m_footing.IsVirtualFooting())
		{
			pWall->m_footing.m_tvArrFront.GetTvByInfo("기초하면", tv);
		}
		else if(pWall->m_tvArrLowerLimit_FrontRebar.GetSize() > 0)
		{
			pWall->m_tvArrLowerLimit_FrontRebar.GetAt(0);
		}
		GetXyMatchLineAndLine(pRC->m_xyEnd,  (pRC->m_xyMid1-pRC->m_xyStt).Unit(), tv.m_v1, tv.GetXyDir(), xyStt);

		CDomyun Dom(pDomP);
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		m_pStd->DimRebarInfoCycleArray(&Dom, pArrRC, xyStt, xyEnd, bLeft, nDimDan, nDrawLeft, TRUE, FALSE);

		*pDomP << Dom;
	}
}

// 벽체 정면도 중간부분 치수
// 벽체 두께 기입(경사교각일때만..)
void CDrawWall::DimMidFront(CDomyun *pDomP, CWallApp *pWall, BOOL bInput)
{
	if(!pWall) return;
	if(!pWall->m_bIs) return;
	if(pWall->m_nType != WALL_TYPE_SLOPE) return;	

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetDirection("BOTTOM");

	CTwinVector tvLeft, tvRight;
	pWall->m_tvArrFront.GetTvByInfo("벽체좌측면", tvLeft);
	pWall->m_tvArrFront.GetTvByInfo("벽체우측면", tvRight);

	CDPoint vAngJijum	= ToDPointFrDegree(pWall->m_dAnglePath);
	double dW	= pWall->m_dW/vAngJijum.y;
	CDPoint xyStt	= tvLeft.GetXyMid();
	Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
	Dom.DimLineTo(dW, 0, bInput ? "W" : COMMA(dW));

	Dom.Rotate(ToRadian(pWall->GetAngleWallByHor(TRUE)-90), xyStt.x, xyStt.y);
	
	*pDomP << Dom;
}

// nCycle : 현재는 1cycle만 지원됨
// 배력근 bLeft는 치수 넣을때만 주의 하면 됨.
void CDrawWall::DrawRebarSupport_Side(CDomyun *pDomP, CWallApp *pWall, BOOL bLeftRebar, long nCycle, long nDan, BOOL bMarkRebar, BOOL bDimRebar, long nDrawLeft, BOOL bLeft, BOOL bInput )
{
	if(!pWall) return;
	if(!pWall->m_bIs) return;
	if(pWall->m_bNotRebar) return;

	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	CHgBaseDrawStd baseDraw;

	//	
	BOOL bCoverLocation = FALSE;
	if(pBri->IsBoxType())
	{
		if(!bLeft && bLeftRebar) //시점
			bCoverLocation = TRUE;
		else if(bLeft && !bLeftRebar)
			bCoverLocation = TRUE;
	}

	CTwinVectorArray tvArr, tvArrAngle, tvArrAngleAdd;
	pWall->GetTvRebarSupport_Side(tvArr, tvArrAngle, tvArrAngleAdd, bLeftRebar, nDan, bLeft,TRUE,bCoverLocation);
	if(tvArr.GetSize() == 0 && tvArrAngle.GetSize() == 0) return;

	// 수직벽체는 오른쪽 철근이든 왼쪽 철근이든 벽체 좌측면 기준으로 그리면 되지만
	// 경사벽체는 철근이 있는 방향을 기준으로 벽체를 그려야 됨.
	BOOL bLeftWall	= pWall->IsVertWall() && pWall->m_tvArrLowerLimit_FrontRebar.GetSize() == 0 ? bLeft : bLeftRebar;

	// nDrawLeft에 따라서 걸러냄.
	if(nDrawLeft != 0)
	{
		CTwinVector tvCenter	= pWall->GetTvCenter_Side(bLeft, 100);
		CRebarPlacing rb;
		rb.TrimTvArrayByLine(tvArr, tvCenter.m_v1, tvCenter.m_v2, nDrawLeft==1, TRUE, FALSE);
		rb.TrimTvArrayByLine(tvArrAngle, tvCenter.m_v1, tvCenter.m_v2, nDrawLeft==1, TRUE, FALSE);
	}

	// 치수기입시는 cutting Area적용 안된 좌표로 치수를 기입해야 된다.
	// 그래서 가장 먼저 치수 기입먼저 한다.
	if(bDimRebar)
	{
		// 치수 기입시는 실제 배력근 말고 다른 좌표를 추가하기 때문에 
		// 다른 변수에 저장 후 사용한다.
		CTwinVectorArray tvArrSup, tvArrSupAngle;
		tvArrSup	= tvArr;
		tvArrSupAngle	= tvArrAngle;

		// m_sInfo 가 "" 인거 제거
		long i = 0; for(i = 0; i < tvArrSupAngle.GetSize(); i++)
		{
			if(tvArrSupAngle.GetAt(i).m_sInfo == "")
			{
				tvArrSupAngle.RemoveAt(i);
				i--;
			}
		}

		// 좌우측...
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		BOOL bLeftDim	= nDrawLeft == 0 || nDrawLeft == -1 ? TRUE : FALSE;
		CDoubleArray dArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		CString sDir	= bLeftDim ? "LEFT" : "RIGHT";
		CDPoint xyStt	= CDPoint(0, 0);
		long nDimDan	= 1;
		
		// 벽체 측면, 하면, 좌우측, 상면 구함. 
		BOOL bIncludeFooting	= TRUE;
		BOOL bIncludeUpperSlab	= pWall->m_nType == WALL_TYPE_SLOPE || pWall->m_tvArrLowerLimit_FrontRebar.GetSize() > 0 ? TRUE : FALSE;
		CTwinVectorArray tvArrSide, tvArrRebarSide;
		pWall->GetTvLeftSide(tvArrSide, FALSE, bIncludeFooting, bIncludeUpperSlab, bLeftWall, 0, FALSE, FALSE, TRUE);
		pWall->GetTvOffset_Side(tvArrRebarSide, bLeftWall, 0);
		CTwinVector tvLeft	= tvArrSide.GetTvByInfo("벽체좌측");
		CTwinVector tvRight	= tvArrSide.GetTvByInfo("벽체우측");
		CTwinVector tvLower	= tvArrSide.GetTvByInfo("벽체하면");
		CTwinVector tvRebarLower	= tvArrRebarSide.GetTvByInfo("벽체하면");

		CTwinVectorArray tvArrUpper, tvArrRebarUpper;
		tvArrSide.GetTvArrByInfoRange("벽체상면", "벽체상면끝", tvArrUpper);
		tvArrSup.InsertAt(0, tvLower);
		tvArrSup.InsertAt(1, tvRebarLower);

		// 치수 기입할 좌표들을 수집한다. ////////////////////////////////////////////////////////
		// 좌측에 기입시는 좌측벽체로 뻗은 교차점, 우측에 기입시는 우측벽체로 뻗은 교차점을 사용.
		CDPoint xySlopeAngle	= CDPoint(1, 0);
		CDPointArray xyArr, xyArrAngle;
		CDPoint xyMatch(0, 0);
		CTwinVector tv;
		CTwinVector tvOrg	= bLeftDim ? tvLeft : tvRight;
		long nSize	= tvArrSup.GetSize();
		for(i = 0; i < nSize; i++)
		{
			tv	= tvArrSup.GetAt(i);
			if(GetXyMatchLineAndLine(tvOrg.m_v1, tvOrg.GetXyDir(), tv.m_v1, tv.GetXyDir(), xyMatch))
				xyArr.Add(xyMatch);
		}
		
		CRebarPlacing rb;
		CTwinVectorArray tvArrRebar;
		nSize	= tvArrSupAngle.GetSize();
		if(nSize > 0)
		{
			CString sInfo	= tvArrSupAngle.GetAt(0).m_sInfo;
			for(i = 0; i < nSize; i++)
			{
				tv	= tvArrSupAngle.GetAt(i);
				if(i == 0)
				{
					tvArrRebar.Add(tv);
					continue;
				}
				else
				{
					if(i < nSize - 1)
					{
						if(tv.m_sInfo == sInfo)
						{
							tvArrRebar.Add(tv);
							continue;
						}
						sInfo	= tv.m_sInfo;
						i--;
					}
					else
						tvArrRebar.Add(tv);
				}


				long nSizeRebar	= tvArrRebar.GetSize();
				if(nSizeRebar > 0)
				{
					CTwinVector tv1	= bLeftDim ? tvArrRebar.GetAt(0) : tvArrRebar.GetAt(nSizeRebar-1);
					CDPoint xyStd = bLeftDim ? tvLeft.m_v1 : tvRight.m_v1;

					// 가각부에 대한 치수선 기준 이동
					if(pWall->m_dWidthExp[iLEFT] > 0 && bLeftDim)
						xyStd.x -= pWall->m_dWidthExp[iLEFT];
					
					if(pWall->m_dWidthExp[iRIGHT] > 0 && !bLeftDim)
						xyStd.x += pWall->m_dWidthExp[iRIGHT];

					xySlopeAngle	= tv1.GetXyDir();
					BOOL bMatch	= FALSE;
					if(bLeftDim)
						bMatch	= GetXyMatchLineAndLine(xyStd, tvLeft.GetXyDir(), tv1.m_v1, tv1.GetXyDir(), xyMatch);
					else
						bMatch	= GetXyMatchLineAndLine(xyStd, tvRight.GetXyDir(), tv1.m_v1, tv1.GetXyDir(), xyMatch);

					if(bMatch)
						xyArrAngle.Add(xyMatch);
					tvArrRebar.RemoveAll();
				}
			}
		}

		// 마지막으로 벽체 상면 피복과 상면 추가.
		// 상부슬래브 좌측이나 우측 위치 추가해줌(벽체를 해주면 안됨:중간지점은 상부슬래브 헌치 밑에서 벽체가 끝나므로)
		CTwinVectorArray tvArrLeftUpper;
		tvArrLeftUpper	= pWall->m_tvArrLeftUpperSlab;
		double dHMove	= 0;
		if(!bLeft)
			rb.Mirror(tvArrLeftUpper, CDPoint(pWall->m_dCenterOfWall, 0), TRUE);

		// tvArrLeftUpper 이게 좌측 기준이니까 m_dDifftop[0]을 사용하고 좌우측차에 대한건 따로 빼주자.
		if(pWall->m_dDiffTop[0] != 0)
			dHMove	= -pWall->m_dDiffTop[0];
		if(!Compare(pWall->m_dDiffTop[0], pWall->m_dDiffTop[1], _T("="), 0.1))
		{
			// BoxType만 dDiffTop을 다르게 줬으니깐 .
			// 상단이 경사인 경우에만.
			if(!bLeftRebar && pWall->m_tvUpperLimitOfMainRebar.GetVerLength() > 10.)
			{
				dHMove -= (pWall->m_dDiffTop[1] - pWall->m_dDiffTop[0]);
			}
		}
		else if(pWall->m_nTypeRebarFront == 0)
		{
			dHMove += bLeftRebar ? pWall->m_dDiffHLeftByCenter : pWall->m_dDiffHRightByCenter;
		}
		rb.MoveRebar(tvArrLeftUpper, CDPoint(0, dHMove));

		CDPoint xyTop	= bLeftDim ? tvArrLeftUpper.GetXyLeft() : tvArrLeftUpper.GetXyRight();
		xyArr.Add(xyTop);
		double dCover	= pWall->m_dCoverUpper_UpperSlab;
		xyArr.InsertAt(xyArr.GetSize()-1, CDPoint(xyTop.x, xyTop.y - dCover));
		/////////////////////////////////////////////////////////////////////////////////////////
				
		
		// 간격구함.
		CDoubleArray dArr;
		rb.ConvertPointArrayToOffsetArray(xyArr, dArr, FALSE, FALSE);

		// 치수기입 정보 생성
		CDoubleArray dArrTmp;
		m_pStd->GetDimInfoAtDistArray(dArrDim, sArrText, sArrUnderText, dArr, "", FALSE, TRUE, TRUE, &Dom, 0.0, TRUE, TRUE, dArrTmp);

		// 경사벽체는 수직거리도 밑에다 써 준다.
		CDPoint vAngWall	= ToDPointFrDegree(pWall->GetAngleWallByHor(TRUE));
		if(!pWall->IsVertWall())
		{
			long i = 0; for(i = 0; i < dArr.GetSize(); i++)
			{
				dArr.SetAt(i, dArr.GetAt(i)*vAngWall.y);
			}

			CDoubleArray dArrDimTmp;
			CStringArray sArrTextTmp;
			CStringArray sArrUnderTextTmp;
			CDoubleArray dArrTmp;
			m_pStd->GetDimInfoAtDistArray(dArrDimTmp, sArrTextTmp, sArrUnderTextTmp, dArr, "", FALSE, TRUE, TRUE, &Dom, 0.0, TRUE, TRUE, dArrTmp);

			sArrUnderText.RemoveAll();
			CString str	= _T("");
			for(i = 0; i < sArrTextTmp.GetSize(); i++)
			{
				str	= sArrTextTmp.GetAt(i);
				str	= "(" + str + ")";
				sArrUnderText.Add(str);
			}
		}
		


		if(pWall->m_dWidthExp[iLEFT] > 0 && bLeftDim)
		{
			tvLower.m_v1.x -= pWall->m_dWidthExp[iLEFT];
			tvLower.m_v2.x -= pWall->m_dWidthExp[iLEFT];
		}
		
		if(pWall->m_dWidthExp[iRIGHT] > 0 && !bLeftDim)
		{
			tvLower.m_v1.x += pWall->m_dWidthExp[iRIGHT];
			tvLower.m_v2.x += pWall->m_dWidthExp[iRIGHT];
		}

		if(pWall->m_footing.m_bIs)
		{
			CTwinVectorArray tvArr, tvArrHidden;
			CFootingApp *pFootingVert	= pWall->IsVertWall() ? &pWall->m_footing : pWall->GetFootingByRotateToVert(bLeftWall);
			pFootingVert->GetTvLeftSide(tvArr, tvArrHidden, FALSE, 0, bLeftWall);
			CTwinVector tvTmp	= tvArr.GetTvByInfo("기초하면");
			if(bLeftDim)
			{
				if(tvTmp.GetXyLeft().x < tvLower.GetXyLeft().x)
					tvLower	= tvTmp;
			}	
			else
			{
				if(tvTmp.GetXyRight().x > tvLower.GetXyRight().x)
					tvLower	= tvTmp;
			}
		}
		else if(pWall->m_tvArrLowerLimit_FrontRebar.GetSize() > 0)
		{
			CTwinVector tvWall	= pWall->m_tvArrFront.GetTvByInfo(bLeftWall ? "벽체좌측면" : "벽체우측면");

			CDPointArray xyArrMatch;
			rb.GetXyMatchTvArrAndLine(pWall->m_tvArrLowerLimit_FrontRebar, tvWall.m_v1, CDPoint(0, -1), xyArrMatch);
			if(xyArrMatch.GetSize() > 0)
				tvLower.m_v1.y	= xyArrMatch.GetAt(0).y;
			tvLower.m_v2.y	= tvLower.m_v1.y;
		}

		xyStt	= bLeftDim ? tvLower.GetXyLeft() : tvLower.GetXyRight();

		long nDan	= xyArrAngle.GetSize() > 0 ? nDimDan + 1 : nDimDan;
		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDan, sDir, -1, pWall->IsVertWall(), FALSE, FALSE, TRUE, FALSE);	

		// 전체 치수
		Dom.SetDirection(sDir);
		double dTot	= 0;
		for(i = 0; i < dArrDim.GetSize(); i++)
			dTot += dArrDim.GetAt(i);

		if(!pWall->IsVertWall())
		{
			Dom.DimMoveTo(xyStt.x, xyStt.y, nDan+1, "");
			Dom.DimLineToExtend(dTot, nDan+1, COMMA(dTot), COMMA(dTot*vAngWall.y));
		}
		

		// 사방향 철근 치수 기입
		if(xyArrAngle.GetSize() > 0)
		{
			xyArrAngle.Add(xyTop);
			xyArrAngle.InsertAt(xyArrAngle.GetSize()-1, CDPoint(xyTop.x, xyTop.y - dCover));

			if(tvArrAngleAdd.GetSize() > 0)
			{
				CPlaceRebarByBlock *pPlace	= &pWall->m_placeByBlock_SupportRebarAddSameUpperSlab;
				if(pPlace->m_xyArr.GetSize() > 0)
				{
					CDPoint xyR	= pPlace->m_xyArr.GetAt(0);
					CDPoint xy	= xyArrAngle.GetAt(0);
					long i = 0; for(i = 0; i < xyR.x; i++)
					{
						xy.y -= xyR.y;
						xyArrAngle.InsertAt(0, xy);
					}
				}
			}

			xyStt	= xyArrAngle.GetAt(0);
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();

			Dom.SetDimObq(bLeftDim ? -xySlopeAngle.GetAngleDegree() : xySlopeAngle.GetAngleDegree());
			baseDraw.AutoDimCont(&Dom, xyStt, xyArrAngle, sArrText, sArrUnderText, nDimDan, sDir, dCover, FALSE, TRUE, FALSE);
		}

		// 꺽임에 대한 치수
		if(bInput)
		{
			if(tvArrAngleAdd.GetSize() > 0)
			{
				CDRect rect	= tvArrAngle.GetRect();
				CTwinVectorArray tvArrSide;
				pWall->GetTvLeftSide(tvArrSide, FALSE, TRUE, FALSE, bLeft, 0, FALSE, FALSE);
				
				// 좌측
				Dom.SetDimObq(0);
				Dom.SetDirection("BOTTOM");
				xyStt	= CDPoint(rect.left, tvArrSide.GetRect().bottom);
				Dom.DimMoveTo(xyStt.x, xyStt.y, 0, "");
				Dom.DimLineToExtend(pWall->m_dDistAddSupportRebarForSameUpperSlab[iLEFT], 0, "좌측범위", COMMA(pWall->m_dDistAddSupportRebarForSameUpperSlab[iLEFT]));

				// 우측
				xyStt	= CDPoint(rect.right, tvArrSide.GetRect().bottom);
				Dom.DimMoveTo(xyStt.x - pWall->m_dDistAddSupportRebarForSameUpperSlab[iRIGHT], xyStt.y, 0);
				Dom.DimLineToExtend(pWall->m_dDistAddSupportRebarForSameUpperSlab[iRIGHT], 0, "우측범위", COMMA(pWall->m_dDistAddSupportRebarForSameUpperSlab[iRIGHT]));
			}
		}
	}

	// 구해진 철근에 cutting Area 적용
// 	long nUpper	= -1;
	if(pWall->GetRebarInfoCycle(nCycle, 1, bLeftRebar, 0))
	{

		//#0028185 1단철근만 있는경우 cutting를 하지 않는다.
		BOOL bExistUpper = FALSE;
		BOOL bExistLower = FALSE;

		//2단, 3단이 배치가 되는지 판별...
		for(long ix = 1; ix < MAX_REBAR_DAN; ix ++)
		{
			CTwinVectorArray tvArrCuttingArea;
			pWall->GetTvCuttingArea_Side(tvArrCuttingArea, bLeftRebar, ix, TRUE);

			CDPoint xy1 = tvArrCuttingArea.GetXyTop();
			CDPoint xy2 = tvArrCuttingArea.GetXyBottom();

			double dMidUpper = min(xy1.y, xy2.y) + fabs(xy1.y - xy2.y) / 2;

			pWall->GetTvCuttingArea_Side(tvArrCuttingArea, bLeftRebar, ix, FALSE);

			xy1 = tvArrCuttingArea.GetXyTop();
			xy2 = tvArrCuttingArea.GetXyBottom();

			double dMidLower = min(xy1.y, xy2.y) + fabs(xy1.y - xy2.y) / 2;


			long left	= bLeftRebar ? iLEFT : iRIGHT;
			CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &pWall->m_pArrRCMain[nCycle][left][ix];
			long nSize	= pArrRC->GetSize();

			for(long rc = 0; rc < nSize; rc++)
			{
				CRebarInfoCycle *pRC	= pWall->GetRebarInfoCycle(nCycle, ix, bLeftRebar, rc);
				if(!pRC) continue;

				if(pRC->m_bExist == FALSE) continue;

				double dHiht = max(pRC->m_xyStt.y, pRC->m_xyEnd.y);
				double dLow = min(pRC->m_xyStt.y, pRC->m_xyEnd.y);

				if(dLow <= dMidUpper && dHiht >= dMidUpper)
				{
					bExistUpper = TRUE;
				}

				if(dLow <= dMidLower && dHiht >= dMidLower)
				{
					bExistLower = TRUE;
				}
			}
		}

		long nUpper	= -1;
		if(bExistUpper && bExistLower == FALSE)
			nUpper = iUPPER;

		if(bExistUpper == FALSE && bExistLower)
			nUpper = iLOWER;
		
// 		ApplyCuttingArea_Side(&Dom, pWall, tvArr, nDrawLeft, nDan, nUpper, nDan == 0, bLeftRebar);
// 		ApplyCuttingArea_Side(&Dom, pWall, tvArrAngle, nDrawLeft, nDan, nUpper, nDan == 0, bLeftRebar);
// 		ApplyCuttingArea_Side(&Dom, pWall, tvArrAngleAdd, nDrawLeft, nDan, nUpper, nDan == 0, bLeftRebar);	

		BOOL bDrawCutArc = (bExistUpper || bExistLower)? TRUE : FALSE;
		ApplyCuttingArea_Side(&Dom, pWall, tvArr, nDrawLeft, nDan, nUpper, nDan == 0 && bDrawCutArc, bLeftRebar);
		ApplyCuttingArea_Side(&Dom, pWall, tvArrAngle, nDrawLeft, nDan, nUpper, nDan == 0 && bDrawCutArc, bLeftRebar);
		ApplyCuttingArea_Side(&Dom, pWall, tvArrAngleAdd, nDrawLeft, nDan, nUpper, nDan == 0 && bDrawCutArc, bLeftRebar);	
	}
	if(pWall->m_nTypeOutputMainRebar_Side == 1)
	{
		ApplyCuttingArea_Side_ForGagak(&Dom, pWall, tvArr, nDrawLeft, nDan == 0);
		ApplyCuttingArea_Side_ForGagak(&Dom, pWall, tvArrAngle, nDrawLeft, nDan == 0);
		ApplyCuttingArea_Side_ForGagak(&Dom, pWall, tvArrAngleAdd, nDrawLeft, nDan == 0);
	}

	
	// 철근 그림
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	baseDraw.DrawTvArray(&Dom, tvArr);
	baseDraw.DrawTvArray(&Dom, tvArrAngle);
	baseDraw.DrawTvArray(&Dom, tvArrAngleAdd);

	CString sDecription	= _T("");
	if(pWall->IsRoundRectWall())
		sDecription.Format("%d단 배력철근", nDan+1);
	else
		sDecription.Format("%s %d단 배력철근", bLeftRebar ? pWall->m_sNameLeft : pWall->m_sNameRight, nDan+1);
	CString sMark	= pWall->GetMarkRebarSupport(0, FALSE, FALSE, -1, sDecription);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		// (ARCBRIDGE-2828) Trime이 된상태에서 다시 Trime을 시도하면 잘못된 결과를 가져올수있다. GetTvByCircleArea() 함수에 문제가 있는데 해결할수가 없다.
		// 2단 이상의 철근 마크할때는 구해진 배력철근중 상부의 원에 대해서만 철근 마크를 하자...
		if(nDan > 0)
		{
			ApplyCuttingArea_Side(&Dom, pWall, tvArr, nDrawLeft, nDan, iUPPER, FALSE, bLeftRebar);
		}
		if(pWall->m_nTypeOutputMainRebar_Side == 1)
		{
			// ApplyCuttingArea_Side_ForGagak(&Dom, pWall, tvArr, bLeftRebar? -1 : 1, FALSE);
		}

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		// ?????? bLeftRebar를 써야되는거 아님?
		// long left	= bLeft ? iLEFT : iRIGHT;
		long left	= bLeftRebar ? iLEFT : iRIGHT;
		CPlaceRebarByBlock *pPlace	= &pWall->m_placeByBlock_SupportRebar[left][nDan];
		CRebar *pRb = pWall->GetRebarSupportByDescription(sDecription);

		// 배력철근 마크를 위한 주철근 좌표(주철근의 대략 1/6 지점에 마크함) /////////
		CTwinVectorArray tvArrMain1;
		CTwinVectorArray tvArrMain2;
		CTwinVectorArray tvArrJoint;
		pWall->GetTvRebarMain_Side(tvArrMain1, bLeftRebar, 0, nDan, 999, tvArrJoint, TRUE, bLeft);
		pWall->GetTvRebarMain_Side(tvArrMain2, bLeftRebar, 1, nDan, 999, tvArrJoint, TRUE, bLeft);

		if(nDan > 0)
		{
			ApplyCuttingArea_Side(&Dom, pWall, tvArrMain1, nDrawLeft, nDan, iUPPER, FALSE, bLeftRebar);
			ApplyCuttingArea_Side(&Dom, pWall, tvArrMain2, nDrawLeft, nDan, iUPPER, FALSE, bLeftRebar);
		}
		if(pWall->m_nTypeOutputMainRebar_Side == 1)
		{
			ApplyCuttingArea_Side_ForGagak(&Dom, pWall, tvArrMain1, nDrawLeft, FALSE);
			ApplyCuttingArea_Side_ForGagak(&Dom, pWall, tvArrMain2, nDrawLeft, FALSE);
		}

		CTwinVectorArray tvArrTmp;
		long nSize	= tvArrMain1.GetSize();
		long nSize2	= tvArrMain2.GetSize();
		if(nSize%2==0) nSize--;
		if(nSize2%2==0) nSize2--;
		if(nSize > 0)
		{
			tvArrTmp.Add(tvArrMain1.GetAt(nDan == 0 ? nSize/4 : nSize/2));
			if(nSize2 > 0)
				tvArrTmp.Add(tvArrMain2.GetAt(nDan == 0 ? nSize2/4 : nSize2/2));
			else
				tvArrTmp.Add(tvArrMain1.GetAt(nSize > 1 ? (nDan == 0 ? nSize/4 : nSize/2)+1 : 0));
		}
		//////////////////////////////////////////////////////////////////////////////

		CDRect rect	= tvArr.GetRect();
// 		CTwinVector *pTv	= NULL;
		long i = 0; for(i = 0; i < tvArrTmp.GetSize(); i++)
		{
			CTwinVector *pTv	= tvArrTmp.GetPAt(i);
			pTv->m_v1.y	= rect.bottom;
			pTv->m_v2.y	= rect.top;
			if(Compare(pTv->GetLength(), 0.0, "="))
			{
				tvArrTmp.RemoveAt(i--);
				continue;
			}
		}

		if(tvArr.GetSize() > 0 && tvArrTmp.GetSize() > 0)
		{
			CMarkDomyun mark;//(&Dom, m_pStd->m_pARoadOptionStd);
			mark.SetDomyun(&Dom, m_pStd->m_pARoadOptionStd);

			//#0028185수평철근이 sort가 되어 있지 않아 마킹의 순서가 잘못 출력되어 마킹이 잘못 표현 됨
			//sorting
			{
				CDPoint xy;
				CDPointArray xyArrData;
				CTwinVectorArray tvArrSort;

				tvArrSort.AddFromTvArray(tvArr);

				for(long ix = 0; ix < tvArrSort.GetSize(); ix ++)
				{
					xy.x = tvArrSort.GetAt(ix).GetXyLeft().y;
					xy.y = ix;
					xyArrData.Add(xy);
				}

				xyArrData.Sort(TRUE, TRUE);

				tvArr.RemoveAll();

				for(long ix = 0; ix < xyArrData.GetSize(); ix ++)
				{
					CTwinVector tv = tvArrSort.GetAt((int)(xyArrData.GetAt(ix).y));

					tvArr.Add(tv);
				}
			}

			mark.SetRebar(tvArr, tvArrTmp);

			// 설정
			mark.m_sMark					= sMark;
			mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pWall->m_Fy);
			mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_END;
			mark.m_nTypeMarkDir				= TYPE_MARKDIR_RIGHT;
			mark.m_dPosScaleMarkStt			= 0.5;
			mark.m_dAddHeight				= Dom.Always(MARK_BASE_HEIGHT);
						
			// 마지막 철근마크위치 부터 벽체상면까지 거리
			BOOL bIncludeFooting	= TRUE;
			BOOL bIncludeUpperSlab	= pWall->m_nType == WALL_TYPE_SLOPE ? TRUE : FALSE;
			CTwinVectorArray tvArrSide, tvArrUpper;
			pWall->GetTvLeftSide(tvArrSide, FALSE, bIncludeFooting, bIncludeUpperSlab, bLeft, 0, FALSE, FALSE);
			tvArrSide.GetTvArrByInfoRange("벽체상면", "벽체상면끝", tvArrUpper);
			if(pWall->m_tvArrLeftUpper.GetSize() > 0)
			{
				tvArrUpper.RemoveAll();
				tvArrUpper.AddFromTvArray(pWall->m_tvArrLeftUpper);

				if(pWall->m_dDiffTop[bLeft? 0 : 1] != 0)
				{
					CRebarPlacing rb;
					rb.MoveRebar(tvArrUpper, CDPoint(0, -pWall->m_dDiffTop[bLeft? 0 : 1]));
				}
					
			}

			rect	= tvArrUpper.GetRect();

			CDPoint xyMark					= mark.GetXyMark();
			CDPoint xyMatch(0, 0);

			//#0028185 다른 D6, D8철근처럼 철근체크를 연결하여 구조물 위쪽에 표기되어야 합니다		
			if(GetXyMatchLineAndLine(CDPoint(rect.left, rect.top), CDPoint(1, 0), xyMark, CDPoint(0, 1), xyMatch))
				mark.m_dAddHeight	+= (xyMatch.y - xyMark.y);
			mark.m_dCtc = pWall->GetCTCRebarSupport(bLeft, nDan, TRUE);
			mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;
			
			mark.MarkRebar();

			// 꺽이는 배력 철근
			for(long add = 0; add < 2; add++)
			{
				CTwinVectorArray *pTvArr = add == 0 ? &tvArrAngle : &tvArrAngleAdd;
				sDecription.Format("%s %d단 배력철근%d", bLeftRebar ? pWall->m_sNameLeft : pWall->m_sNameRight, nDan+1, 2+add);
				CString sMark	= pWall->GetMarkRebarSupport(0, FALSE, FALSE, -1, sDecription);
				if(pTvArr->GetSize() > 0)
				{
					CRebarPlacing rb;
					CDRect rectRebar	= tvArrAngle.GetRect();
					if(add == 1)
						rectRebar	= tvArrAngleAdd.GetRect();
					double dXMark	= add == 0 ? rectRebar.left + rectRebar.Width() / 5 * 4 : rectRebar.left + rectRebar.Width() / 5 * 3.5;

					CDPointArray xyArrMark;

					CDPointArray xyArrMatch;
					CTwinVectorArray tvArrRebar;
					CString sInfo	= _T("");
					CTwinVector tv;
					
					long i = 0; for(i = 0; i < pTvArr->GetSize(); i++)
					{
						tv	= pTvArr->GetAt(i);

						tvArrRebar.RemoveAll();
						tvArrRebar.Add(tv);
						sInfo	= tv.m_sInfo;
					
						for(long j = i + 1; j < pTvArr->GetSize(); j++)
						{
							tv	= pTvArr->GetAt(j);
							if(tv.m_sInfo == sInfo)
							{
								sInfo	= tv.m_sInfo;
								i	= j - 1;
								tvArrRebar.Add(tv);
							}
							else
							{
								sInfo	= tv.m_sInfo;
								i	= j - 1;
								break;
							}
						}
						

						if(tvArrRebar.GetSize() > 0)
						{
							rb.GetXyMatchTvArrAndLine(tvArrRebar, CDPoint(dXMark, 0), CDPoint(0, 1), xyArrMatch);
							if(xyArrMatch.GetSize() > 0)
							{
								xyArrMark.Add(xyArrMatch.GetAt(0));
							}
						}
					}

					// 마크할 위치 다 찾은 뒤 마크함.
					if(xyArrMark.GetSize() > 0)
					{
						CConcSymbol sym(m_pStd->m_pARoadOptionStd);
						for(i = 0; i < xyArrMark.GetSize(); i++)
						{
							sym.RebarCountMarkOne(&Dom, xyArrMark.GetAt(i));
						}

						xyMark	= xyArrMark.GetAt(xyArrMark.GetSize()-1);
						xyMark.y	= rect.top;

						Dom.LineTo(xyArrMark.GetAt(0), xyMark);
						// sym.RebarMarkLineUpper(&Dom, xyMark, sMark, m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia), FALSE, 0, FALSE);

						pRb = pWall->GetRebarSupportByDescription(sDecription);

						SRebarMarkInputData mark_input_data;
						SRebarMarkviewData mark_view_data;
						mark_input_data.mark = sMark;
						mark_input_data.dia = m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pWall->m_Fy);
						mark_input_data.position = xyMark;
						mark_input_data.ctc = pWall->GetCTCRebarSupport(bLeft, nDan, TRUE);
						mark_input_data.type_rebar = pRb? pRb->m_nTypeRebar : -1;
						mark_input_data.left_dir =  FALSE;
						mark_input_data.height = 0;

						mark_view_data.count_mark = FALSE;
						mark_view_data.mark_pos = eMarkUpper;

						sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
					}
				}
			}
		}
	}

	*pDomP << Dom;
}

void CDrawWall::DrawRebarMain_Side(CDomyun *pDomP, CWallApp *pWall, BOOL bLeftRebar, long nCycle, long nDan, BOOL bMarkRebar, BOOL bDimRebar, long nDrawLeft, CString sDescription, BOOL bLeft)
{
	if(!pWall) return;
	if(!pWall->m_bIs) return;
	if(pWall->m_bNotRebar) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	CHgBaseDrawStd baseDraw;
	CHgBaseConcStd baseConc;
	CRebarPlacing rb;
	
	long left	= bLeftRebar ? iLEFT : iRIGHT;
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &pWall->m_pArrRCMain[nCycle][left][nDan];
	long nSize	= pArrRC->GetSize();
	CTwinVector tvLower	= pWall->m_tvArrFront.GetTvByInfo("벽체하면좌측");

	BOOL bBoxType = m_pStd->m_pRcBridge->IsBoxType();
	
	for(long rc = 0; rc < nSize; rc++)
	{
		CRebarInfoCycle *pRC	= pWall->GetRebarInfoCycle(nCycle, nDan, bLeftRebar, rc);
		if(!pRC) continue;

		BOOL bBoxTypeNotSlabJoint = bBoxType && pRC->m_nPosStt == REBARINFO_POS_STTWALL_OUTTER && pRC->m_nPosEnd == REBARINFO_POS_ENDWALL_OUTTER;

		CTwinVectorArray tvArr, tvArrJoint;
		pWall->GetTvRebarMain_Side(tvArr, bLeftRebar, nCycle, nDan, rc, tvArrJoint, TRUE, bLeft);
		if(rc == 0)
			pWall->m_tvArrRebarMainSideForDim.AddFromTvArray(tvArr);


		if(bDimRebar && rc==0)
		{
			CTwinVectorArray tvArrDim, tvArrJointDim;
			pWall->GetTvRebarMain_Side(tvArrDim, bLeftRebar, -1, nDan, rc, tvArrJointDim, TRUE, bLeft, TRUE);

			// 위쪽에 치수기입
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
			CDoubleArray dArrDim;
			CStringArray sArrText;
			CStringArray sArrUnderText;
			CString sDir	= "TOP";
			CDPoint xyStt	= CDPoint(0, 0);
			long nDimDan	= 1;
			
			// 벽체 측면 좌우측 구함.
			CTwinVectorArray tvArrSide;
			BOOL bIncludeFooting	= TRUE;
			BOOL bIncludeUpperSlab	= TRUE;
			pWall->GetTvLeftSide(tvArrSide, FALSE, bIncludeFooting, bIncludeUpperSlab, bLeft);
			CTwinVector tvLeft	= tvArrSide.GetTvByInfo("벽체좌측");
			CTwinVector tvRight	= tvArrSide.GetTvByInfo("벽체우측");

			if(pWall->m_dWidthExp[iLEFT] > 0)
			{
				tvLeft.m_v1.x -= pWall->m_dWidthExp[iLEFT];
				tvLeft.m_v2.x -= pWall->m_dWidthExp[iLEFT];
			}
			if(pWall->m_dWidthExp[iRIGHT] > 0)
			{
				tvRight.m_v1.x += pWall->m_dWidthExp[iRIGHT];
				tvRight.m_v2.x += pWall->m_dWidthExp[iRIGHT];
			}
			
			// 간격구함.
			CRebarPlacing rb;
			CDPointArray xyArr;
			CDoubleArray dArr;
			long nSize	= tvArrDim.GetSize();
			long i = 0; for(i = 0; i < nSize; i++)
				xyArr.Add(tvArrDim[i].m_v2);
			rb.ConvertPointArrayToOffsetArray(xyArr, dArr, TRUE, FALSE);

			// 치수기입 정보 생성
			if(tvArrDim.GetSize() > 0)
			{
				dArrDim.Add(tvArrDim[0].m_v2.x-tvLeft.m_v2.x);
				sArrText.Add("0");
				sArrUnderText.Add("0");

				CDoubleArray dArrTmp;
				m_pStd->GetDimInfoAtDistArray(dArrDim, sArrText, sArrUnderText, dArr, "", FALSE, TRUE, TRUE, &Dom, 0.0, TRUE, TRUE, dArrTmp);

				dArrDim.Add(tvRight.m_v2.x-tvArrDim[nSize-1].m_v2.x);
				sArrText.Add("0");
				sArrUnderText.Add("0");
			}
			else
			{
				dArrDim.Add(tvRight.m_v2.x-tvLeft.m_v2.x);
				sArrText.Add("0");
				sArrUnderText.Add("0");
			}
			
			xyStt	= tvLeft.GetXyTop();

			// 치수기입은 벽체 좌우측면 중 높은 곳을 기준으로 한다.
			xyStt.y	= pWall->m_tvArrLeftUpperSlab.GetRect().top;
			xyStt.y -= pWall->m_dDiffTop[bLeft? 0 : 1];

			baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, FALSE, FALSE, TRUE, FALSE);
		}

		// nDrawLeft에 따라서 걸러냄.
		if(nDrawLeft != 0)
		{
			CTwinVector tvCenter	= pWall->GetTvCenter_Side(bLeft, 100);
			CRebarPlacing rb;
			double dDiff	= nDrawLeft == -1 ? -1 : 1;
			tvCenter.m_v1.x += dDiff;
			tvCenter.m_v2.x += dDiff;

			rb.TrimTvArrayByLine(tvArr, tvCenter.m_v1, tvCenter.m_v2, nDrawLeft==1, TRUE, FALSE);
			rb.TrimTvArrayByLine(tvArrJoint, tvCenter.m_v1, tvCenter.m_v2, nDrawLeft==1, TRUE, FALSE);
		}

		// 라운드부는 점선으로 그리기 위해서 점선부분을 분리 해 낸다.
		CTwinVectorArray tvArrRound1, tvArrRound2;
		if(pRC->m_nPosStt != pRC->m_nPosEnd)
		{
			// Box형은 바닥보다 철근이 위에 있으니가 150정도만 올려서 판단해 보자.
			double dSttY = bBoxTypeNotSlabJoint && !bLeftRebar? pRC->m_xyEnd.y : pRC->m_xyStt.y;

			BOOL bInFooting	= tvLower.GetXyTop().y + 150 > dSttY;
			pWall->DivideTvArrayByRound(tvArr, tvArrRound1, pWall->GetLenRebarAtEdge(pRC->m_Dia, bInFooting), bInFooting);
			if(pRC->m_bUseEndExt && !bBoxTypeNotSlabJoint)
				pWall->DivideTvArrayByRound(tvArr, tvArrRound2, pWall->GetLenRebarAtEdge(pRC->m_Dia, TRUE), FALSE);
		}
		
		long nUpper	= -1;
		//ARCBRIDGE-2829 상부 슬래브의 2단 이상의 배근이 있을 경우 최상단, 최하단의 배근은 자동으로 성립
		//if(pWall->GetDanMainRebar(0, bLeftRebar) > 1 || pWall->GetDanMainRebar(1, bLeftRebar) > 1)
		{
			//#0028185 1단철근만 있는경우 cutting를 하지 않는다.
			BOOL bExistUpper = FALSE;
			BOOL bExistLower = FALSE;

			//2단, 3단이 배치가 되는지 판별...
			for(long ix = 1; ix < MAX_REBAR_DAN; ix ++)
			{
				CTwinVectorArray tvArrCuttingArea;
				pWall->GetTvCuttingArea_Side(tvArrCuttingArea, bLeftRebar, ix, TRUE);

				CDPoint xy1 = tvArrCuttingArea.GetXyTop();
				CDPoint xy2 = tvArrCuttingArea.GetXyBottom();

				double dMidUpper = min(xy1.y, xy2.y) + fabs(xy1.y - xy2.y) / 2;

				pWall->GetTvCuttingArea_Side(tvArrCuttingArea, bLeftRebar, ix, FALSE);

				xy1 = tvArrCuttingArea.GetXyTop();
				xy2 = tvArrCuttingArea.GetXyBottom();

				double dMidLower = min(xy1.y, xy2.y) + fabs(xy1.y - xy2.y) / 2;


				long left	= bLeftRebar ? iLEFT : iRIGHT;
				CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &pWall->m_pArrRCMain[nCycle][left][ix];
				long nSize	= pArrRC->GetSize();

				for(long rc = 0; rc < nSize; rc++)
				{
					CRebarInfoCycle *pRC	= pWall->GetRebarInfoCycle(nCycle, ix, bLeftRebar, rc);
					if(!pRC) continue;

					if(pRC->m_bExist == FALSE) continue;

					double dHiht = max(pRC->m_xyStt.y, pRC->m_xyEnd.y);
					double dLow = min(pRC->m_xyStt.y, pRC->m_xyEnd.y);

					if(dLow <= dMidUpper && dHiht >= dMidUpper)
					{
						bExistUpper = TRUE;
					}

					if(dLow <= dMidLower && dHiht >= dMidLower)
					{
						bExistLower = TRUE;
					}
				}
			}

			if(bExistUpper && bExistLower == FALSE)
				nUpper = iUPPER;

			if(bExistUpper == FALSE && bExistLower)
				nUpper = iLOWER;

// 			ApplyCuttingArea_Side(&Dom, pWall, tvArr, nDrawLeft, nDan, nUpper, nDan == 0 && nCycle == 0, bLeftRebar);
// 			ApplyCuttingArea_Side(&Dom, pWall, tvArrRound1, nDrawLeft, nDan, nUpper, nDan == 0 && nCycle == 0, bLeftRebar);
// 			ApplyCuttingArea_Side(&Dom, pWall, tvArrRound2, nDrawLeft, nDan, nUpper, nDan == 0 && nCycle == 0, bLeftRebar);
// 			ApplyCuttingArea_Side(&Dom, pWall, tvArrJoint, nDrawLeft, nDan, nUpper, nDan == 0 && nCycle == 0, bLeftRebar);

			BOOL bDrawCutArc = (bExistUpper || bExistLower)? TRUE : FALSE;
			if(bDrawCutArc)
			{
				ApplyCuttingArea_Side(&Dom, pWall, tvArr, nDrawLeft, nDan, nUpper, nDan == 0 && bDrawCutArc, bLeftRebar);
				ApplyCuttingArea_Side(&Dom, pWall, tvArrRound1, nDrawLeft, nDan, nUpper, nDan == 0 && bDrawCutArc, bLeftRebar);
				ApplyCuttingArea_Side(&Dom, pWall, tvArrRound2, nDrawLeft, nDan, nUpper, nDan == 0 && bDrawCutArc, bLeftRebar);
				ApplyCuttingArea_Side(&Dom, pWall, tvArrJoint, nDrawLeft, nDan, nUpper, nDan == 0 && bDrawCutArc, bLeftRebar);
			}
		}
		if(pWall->m_nTypeOutputMainRebar_Side == 1)
		{
			ApplyCuttingArea_Side_ForGagak(&Dom, pWall, tvArr, nDrawLeft, nDan == 0 && nCycle == 0);
			ApplyCuttingArea_Side_ForGagak(&Dom, pWall, tvArrRound1, nDrawLeft, nDan == 0 && nCycle == 0);
			ApplyCuttingArea_Side_ForGagak(&Dom, pWall, tvArrRound2, nDrawLeft, nDan == 0 && nCycle == 0);
			ApplyCuttingArea_Side_ForGagak(&Dom, pWall, tvArrJoint, nDrawLeft, nDan == 0 && nCycle == 0);
		}
		
		// 철근과 라운드부 점선 그림
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
		baseDraw.DrawTvArray(&Dom, tvArr);
		m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_HIDDEN_LINE);
		baseDraw.DrawTvArray(&Dom, tvArrRound1);
		baseDraw.DrawTvArray(&Dom, tvArrRound2);
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);
		baseDraw.DrawTvArray(&Dom, tvArrJoint);

		
		if(pWall->GetRebarInfoCycle(nCycle, 1, bLeftRebar, 0))
		{
			if(nDrawLeft == 0 && pWall->m_nTypeOutputMainRebar_Side == 1)
			{
				ApplyCuttingArea_Side(&Dom, pWall, tvArr, -1, nDan, nUpper, nDan == 0 && nCycle == 0, bLeftRebar);
			}
		}

		// 벽체 가운데를 파는 옵션이면서 좌우측 모두 출력인 경우 우측은 없애 버린다.
		if(nDrawLeft == 0 && pWall->m_nTypeOutputMainRebar_Side == 1)
		{
			ApplyCuttingArea_Side_ForGagak(&Dom, pWall, tvArr, -1, nDan == 0 && nCycle == 0);
		}

		//CString sTmp	= _T("");
		//sTmp.Format("%s %d", sDescription, rc+1);
		CString sMark	= pWall->GetMarkRebarMain(0, FALSE, nCycle, FALSE, FALSE, pRC->m_sDescription, nDan);//pRC->m_szMark;
		if(bMarkRebar && m_pStd->IsMarkRebar(sMark) && tvArr.GetSize() > 0)
		{
			
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

			// 1cycle은 0.55
			// 2cycle은 0.44 위치에 기입
			double dCountCycle	= (pWall->GetCountCycleMainRebar()+1);
			double dPosScale	= 1./dCountCycle * (nCycle+1);//nCycle == 0 ? 0.53+(nDan*0.3) : 0.47-(nDan*0.3);

			BOOL bLeftDim	= nDrawLeft == 0 || nDrawLeft == -1 ? TRUE : FALSE;
			// 벽체는 파는 옵션이면 마크방향을 안쪽으로 바꿈.
			if(pWall->m_nTypeOutputMainRebar_Side==1)
			{
				bLeftDim	= !bLeftDim;
			}						
			// 주철근 마크를 위한 상대적인 배력철근 좌표
			CTwinVectorArray tvArrSup, tvArrSupAngle, tvArrSupAngleAdd;
			pWall->GetTvRebarSupport_Side(tvArrSup, tvArrSupAngle, tvArrSupAngleAdd, bLeftRebar, nDan, bLeft);

			// 배력철근을 주철근 범위 내에꺼만 사용한다. 상하만 체크함. /////
			CDRect rect	= tvArr.GetRect();
			CTwinVector tvRange;
			tvRange.m_v1	= CVector(rect.left, rect.bottom, 0);
			tvRange.m_v2	= CVector(rect.left, rect.top, 0);
			rb.GetTvArrInRange(tvArrSup, tvRange, FALSE);
			if(tvArrSup.GetSize() == 0)
			{
				CTwinVector tv;
				tv.m_v1	= CVector(rect.left, rect.bottom, 0);
				tv.m_v2	= CVector(rect.right, rect.bottom, 0);
				tvArrSup.Add(tv);
				tv.m_v1	= CVector(rect.left, rect.top, 0);
				tv.m_v2	= CVector(rect.right, rect.top, 0);
				tvArrSup.Add(tv);
			}
			//////////////////////////////////////////////////////////////////

			CRebar *pRb = pWall->GetRebarMainByDescription(pRC->m_sDescription);
			CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
			mark.SetRebar(tvArr, tvArrSup);

			// 설정
			mark.m_sMark					= sMark;
			mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pWall->m_Fy);
			mark.m_nTypeMarkPosLineRebar	= bLeftDim ? TYPE_MARKPOS_STT : TYPE_MARKPOS_END;
			mark.m_nTypeMarkDir				= bLeftDim ? TYPE_MARKDIR_LEFT : TYPE_MARKDIR_RIGHT;
			mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
			mark.m_dPosScale				= dPosScale;
			CDPoint xyMark					= mark.GetXyMark();
			if(pWall->m_nTypeOutputMainRebar_Side == 0)
			{
				// 벽체 좌우측까지의 간격 구함.
				CDPoint xyMatch(0, 0);
				CTwinVectorArray tvArrSide;
				pWall->GetTvLeftSide(tvArrSide, FALSE, FALSE, FALSE, bLeft, 0, FALSE, FALSE, TRUE);

				CTwinVector tvWall	= tvArrSide.GetTvByInfo(bLeftDim ? "벽체좌측" : "벽체우측");
				GetXyMatchLineAndLine(tvWall.m_v1, tvWall.GetXyDir(), xyMark, CDPoint(1, 0), xyMatch);
				mark.m_dAddHeight	= fabs(xyMatch.x-xyMark.x) + pWall->m_dWidthExp[bLeftDim ? iLEFT : iRIGHT];
			}
			else
			{
				CDPoint xyCen(0, 0);
				double dR, dAngStt, dAngEnd;
				pWall->GetInfoCuttingCircle_Side(nDrawLeft == 0 || nDrawLeft == -1, xyCen, dR, dAngStt, dAngEnd);
				mark.m_dAddHeight	= dR-fabs((xyCen.x-xyMark.x));
			}
			mark.m_dCtc = pWall->m_dCTC_MainRebar;
			mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;
			
			mark.MarkRebar();
		}
	}

	
	*pDomP << Dom;	
}

// 측면 구조도 그리기
// 벽체 측면도 그릴때 주의할 사항 
// 경사형 벽체일경우 기초도 안그리고 상부슬래브도 그리지 않음
// 경사형 벽체를 똑 바로 세운 상태에서 그리기 때문에 다른 구조물과 위치가 맞지 않음.
// 따라서 벽체만 딸랑 그림.
// BOOL bLeft : 왼쪽에서 본 건지?
CDRect CDrawWall::DrawRebar_SideStd(CDomyun *pDomP, CWallApp *pWall, BOOL bLeftAtLeft, BOOL bLeftAtRight, BOOL bMarkRebar, BOOL bDimRebar, CString sLeft, CString sRight, BOOL bDrawCrossSection, BOOL bLeft, BOOL bDrawMainRebar, BOOL bDrawSupportRebar, BOOL bDrawShearRebar, BOOL bInput, double dDistFromCenter)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return CDRect(0, 0, 0, 0); 
	if(!pWall) return CDRect(0, 0, 0, 0); 
	if(!pWall->m_bIs) return CDRect(0, 0, 0, 0); 
	if(pWall->m_bNotRebar) return CDRect(0, 0, 0, 0);
	if(pWall->m_bTopIsColumn)
	{
		return DrawRebar_Column_SideStd(pDomP, pWall, bMarkRebar, bDimRebar);
	}

	// 치수넣는 타입 동기화 /////////////////////////
	// 가각 지원인 경우 가운데를 판다
	if(pWall->m_dWidthExp[iLEFT] > 0 || pWall->m_dWidthExp[iRIGHT] > 0)
		pWall->m_nTypeOutputMainRebar_Side	= 1;
	else
		pWall->m_nTypeOutputMainRebar_Side	= 0;
	//////////////////////////////////////////////////


	double dSta	= pWall->m_xyOrg.x;
	BOOL bSameLR	= bLeftAtLeft==bLeftAtRight;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	// 외곽선 그리기
	BOOL bIncludeFooting	= FALSE;
	BOOL bIncludeUpperSlab	= FALSE;
	if(pWall->m_nType == WALL_TYPE_SLOPE)
		bIncludeUpperSlab	= TRUE;

	// 일반도는 따로 그리자.
	// (39915) 상부슬래브는 따로 그려도 벽체는 무조건 그려줘야댐.
//	if(bDrawCrossSection)
	{
		DrawLeftSide(&Dom, pWall, bIncludeFooting, bIncludeUpperSlab, bLeft, -1, FALSE, TRUE, FALSE);
		DrawLeftSide(&Dom, pWall, bIncludeFooting, bIncludeUpperSlab, bLeft, 1, FALSE, TRUE, FALSE);
	}
	Dom.SetCalcExtRect();
	CDRect rectTot	= Dom.GetExtRect();
	if(pWall->m_nType != WALL_TYPE_SLOPE)
	{
		long nJ = pBri->GetJijumNumberByStation(dSta);
		double dOffsetDist = 0;
		if(nJ == 0 || nJ == pBri->m_nQtyJigan)
		{
			// (ARCBRIDGE-3237) 벽체 배면위치의 기초를 표현 하기 위해서 벽체 전면에서의 Offset을 구해서 처리한다.
			if(pBri->m_nTypeWallForRebarGen == 0)
			{
				dOffsetDist = pBri->m_nTypeWallForRebarGen == 1 ? 0 : pWall->m_dW * (nJ == 0 ? 1 : -1);
			}
		}
		m_pStd->m_pDrawFooting->DrawLeftSide(&Dom, &pWall->m_footing, FALSE, 0, bLeft, FALSE, dOffsetDist);
		if(bDrawCrossSection)
		{
			*pDomP << Dom;
			m_pStd->m_pDrawCrossSection->DrawCrossSection(&Dom, dSta, ToDPointFrDegree(pWall->m_dAnglePath), FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE);
			m_pStd->m_pDrawCrossSection->DrawCrossSlabHunch(&Dom, dSta, ToDPointFrDegree(pWall->m_dAnglePath), FALSE, 0, FALSE, TRUE);

			if(pWall->m_dDiffTop[bLeft? 0 : 1] != 0)
				Dom.Move(CDPoint(0, -pWall->m_dDiffTop[bLeft? 0 : 1]));
			if(!bLeft)
				Dom.Mirror(pWall->m_dCenterOfWall, TRUE);
			*pDomP << Dom;
		}
	}


	
	pWall->m_tvArrRebarMainSideForDim.RemoveAll();
	long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		// 배력철근(좌우측 구분 있어야 됨)
		if(bDrawSupportRebar)
		{
			// 좌측 그리기
 			DrawRebarSupport_Side(&Dom, pWall, bLeftAtLeft, 0, dan, bMarkRebar, dan==0, bSameLR ? 0 : -1, bLeft, bInput);

			if(!bSameLR)
			{
 				DrawRebarSupport_Side(&Dom, pWall, bLeftAtRight, 0, dan, bMarkRebar, dan==0, 1, bLeft, bInput);
			}
		}

		// 주철근(좌우측 구분 있어야 됨)
		// 가각이있다면 주철근은 치수 기입 안함(가각부에서 한꺼번에 하기 때문);
		if(bDrawMainRebar)
		{
			BOOL bIsGagak	= pWall->m_dWidthExp[iLEFT] > 0 || pWall->m_dWidthExp[iRIGHT];
			for(long cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
			{
				// (ARCBRIDGE-2819) 벽체 주철근과 치수를 같이 넣었었는데 따로 넣는걸로 변경
				// DrawRebarMain_Side(&Dom, pWall, bLeftAtLeft, cycle, dan, bMarkRebar, !bIsGagak && dan==0&&cycle==0, bSameLR ? 0 : -1, "", bLeft);
				DrawRebarMain_Side(&Dom, pWall, bLeftAtLeft, cycle, dan, bMarkRebar, dan==0&&cycle==0, bSameLR ? 0 : -1, "", bLeft);
				if(!bSameLR)
				{
					DrawRebarMain_Side(&Dom, pWall, bLeftAtRight, cycle, dan, bMarkRebar, dan==0&&cycle==0, bSameLR ? 0 : 1, "", bLeft);
				}
			}
		}
	}
	// 전단철근(좌우측 구분 없어도 됨)
	if(bDrawShearRebar)
	{
		DrawRebarShear_Side(&Dom, pWall, -1, bLeftAtLeft);
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

	// 단면 마크 "A"
	DrawSectionMark_Side(&Dom, pWall, dDistFromCenter);

	*pDomP << Dom;


	return rectTot;
}

// 측면도에 단면 마크 "A"
void CDrawWall::DrawSectionMark_Side(CDomyun *pDomP, CWallApp *pWall, double dDistFromCenter)
{
	if(!pWall) return;
	if(!pWall->m_bIs) return;

	CDomyun Dom(pDomP);
	CConcSymbol sym(m_pStd->m_pARoadOptionStd);

	// A-A 단면
	// 도로 중심에 해야 됨.(중심선형)
	CTwinVector tvCenLeft	= pWall->GetTvCenter_Side(TRUE, 0);
	CTwinVector tvCenRight	= pWall->GetTvCenter_Side(FALSE, 0);
	CTwinVector tvCen		= tvCenLeft;
	tvCen.m_v1.y			= min(tvCenLeft.GetXyBottom().y, tvCenRight.GetXyBottom().y);
	tvCen.m_v2.y			= max(tvCenLeft.GetXyTop().y, tvCenRight.GetXyTop().y);
	tvCen.m_v1.x += dDistFromCenter;
	tvCen.m_v2.x += dDistFromCenter;
	
	double m_dThickLowerHunch = m_pStd->m_pRcBridge->m_dHHR[0][iLOWER];
	//기둥식일경우 상하부 슬래브 두께가 포함 안되있음. 
	if(pWall->m_bTopIsColumn)
	{
		tvCen.m_v2.y += pWall->m_dThickUppeSlab + pWall->m_dThickHunch;
		tvCen.m_v1.y -= pWall->m_dThickLowerSlab + m_dThickLowerHunch;
	}

	// 위
	sym.ArrowSection(&Dom, "A", tvCen.m_v2, 100, CDPoint(-1, 0), 2);
	// 아래
	sym.ArrowSection(&Dom, "A", tvCen.m_v1, 100, CDPoint(-1, 0), 1);

	// 기둥식인 경우 가운데 기둥의 중간쯤에 마크 "B"
	if(pWall->m_bTopIsColumn || pWall->IsRoundRectWall())
	{
		CColumnApp *pColumn	= pWall->GetColumn(pWall->m_nCountColumn/2);
		if(pColumn)
		{
			pColumn->GetTvFront(FALSE, TRUE);
			CDRect rect	= pColumn->m_tvArrFront.GetRect();
			tvCen.m_v1	= CVector(rect.left, rect.top-fabs(rect.Height())/2, 0);
			tvCen.m_v2	= tvCen.m_v1;
			tvCen.m_v2.x	= rect.right;
		
			// 좌
			sym.ArrowSection(&Dom, "B", tvCen.m_v1, 100, CDPoint(0, 1), 1);
			// 우
			sym.ArrowSection(&Dom, "B", tvCen.m_v2, 100, CDPoint(0, 1), 2);

			
			if(pWall->IsRoundRectWall() == FALSE)
			{
				//다음기둥과의 거리 중간에 표기
				double dGapColumn = pWall->m_dDistColumn[pWall->m_nCountColumn/2] / 2;

				//상부거더 단면 "C"
				tvCen.m_v1.x = tvCen.m_v2.x = rect.CenterPoint().x - dGapColumn;
				tvCen.m_v1.y = rect.top + pWall->m_dThickUppeSlab + pWall->m_dThickHunch;
				tvCen.m_v2.y = rect.top;

				sym.ArrowSection(&Dom, "C", tvCen.m_v1, 100, CDPoint(-1, 0), 2);
				sym.ArrowSection(&Dom, "C", tvCen.m_v2, 100, CDPoint(-1, 0), 1);

				//하부거더 단면 "D"
				if(m_pStd->m_pRcBridge->IsBoxType())
				{
					tvCen.m_v1.y = rect.bottom;
					tvCen.m_v2.y = rect.bottom - pWall->m_dThickLowerSlab - m_dThickLowerHunch;

					sym.ArrowSection(&Dom, "D", tvCen.m_v1, 100, CDPoint(-1, 0), 2);
					sym.ArrowSection(&Dom, "D", tvCen.m_v2, 100, CDPoint(-1, 0), 1);
				}
			}
		}
	}

	*pDomP << Dom;
}

// 벽체 구조도 정면 단면도 통합.
// BOOL bCut : 경사교출력을 위해서 가운데를 잘라버림.
CDRect CDrawWall::DrawRebar_FrontSectionStd(CDomyun *pDomP, CWallApp *pWall, BOOL bInput, BOOL bDimRebar, BOOL bCut, BOOL bVertDir)
{
	if(!pWall) return CDRect(0, 0, 0, 0);
	if(!pWall->m_bIs) return CDRect(0, 0, 0, 0);
	
	CDomyun Dom(pDomP);
	
	// 단면
	CTwinVectorArray tvArrReturn;
	DrawFront(&Dom, pWall, FALSE, &tvArrReturn, FALSE);
	
	Dom.SetCalcExtRect();
	CDRect rectTot	= Dom.GetExtRect();

	if(pWall->m_bTopIsColumn)
	{
		long nJijum = m_pStd->m_pRcBridge->GetJijumWallByStation(pWall->m_xyOrg.x);
		CColumnApp *pColumn	= &pWall->m_Column;
		long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			BOOL bDim	= dan < 2 && bDimRebar; 
			m_pStd->m_pDrawColumn->DrawRebarMain_Front(&Dom, pColumn, FALSE, dan, FALSE, TRUE, FALSE);
			m_pStd->m_pDrawColumn->DrawRebarRound_Front(&Dom, pColumn, nJijum, FALSE, dan, bDim, TRUE, FALSE, FALSE);
		}
		m_pStd->m_pDrawColumn->DimRebarCrossTie_Front(&Dom, pColumn, nJijum, pColumn->GetCountDan_RebarMain()+1, FALSE, FALSE, TRUE);
	}
	else
	{
		long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			long left = 0; for(left = 0; left < 2; left++)
			{
				BOOL bLeft	= left == iLEFT ? TRUE : FALSE;
				// 주철근
				for(long cycle = 0; cycle < pWall->GetCountCycleMainRebar(); cycle++)
				{
					DrawRebarMain_Front(&Dom, pWall, cycle, dan, bLeft, FALSE, FALSE, 0, TRUE, bInput);
				}
							
				// 배력근
				DrawRebarSupport_Front(&Dom, pWall, bLeft, dan, bDimRebar, bDimRebar &&& dan == 0, FALSE, bVertDir);
			}
		}

		// 전단철근
		long nDimDan	= pWall->GetDanMainRebar(0, TRUE);
		//#0032085 전단철근이 직거리의 길이로 표현이 되어 정상적으로 전단철근이 표현되지 않음
		DrawRebarShear_Front(&Dom, pWall, nDimDan, bDimRebar, bDimRebar, TRUE, FALSE, bVertDir);
	}


	*pDomP << Dom;

	return rectTot;
}

// 벽체 측면에 그려지는 철근에 대해서 cuttngArea적용을 한다.
// 결과는 tvArr에 되돌려짐.
// nUpper == -1이면 상하 모두 처리
// nUpper = iUPPER이면 상면만 처리, nUpper == iLOWER이면 하면만 처리
void CDrawWall::ApplyCuttingArea_Side(CDomyun *pDomP, CWallApp *pWall, CTwinVectorArray &tvArr, long nLeft, long nDan, long nUpper, BOOL bDrawCuttingArea, BOOL bLeftRebar)
{

	CDomyun Dom(pDomP);
	long envType	= m_pStd->m_pARoadOptionStd->GetEnvType(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);


	// 1단일 경우 2,3단의 모든 cuttingArea를 잘라낸다. ////////////////////////////
	// 2,3단일 경우 해당 cuttingArea만 그린다.
	CRebarPlacing rb;
	CHgBaseDrawStd baseDraw;
	if(nDan == 0)
	{
		long dan = 0; for(dan = 1; dan < MAX_REBAR_DAN; dan++)
		{
			for(long upper = 0; upper < 2; upper++)
			{
				BOOL bUpper	= upper == iUPPER ? TRUE : FALSE;

				CRebarInfoCycle *pRC1L	= pWall->GetRebarInfoCycle(0, dan, bLeftRebar, 0);
				CRebarInfoCycle *pRC2L	= pWall->GetRebarInfoCycle(1, dan, bLeftRebar, 0);
				BOOL bLowerExist	= pRC1L && pRC1L->m_bExist || pRC2L && pRC2L->m_bExist ? TRUE : FALSE;

				CRebarInfoCycle *pRC1U	= pWall->GetRebarInfoCycle(0, dan, bLeftRebar, pWall->GetCountRebarInfoCycle(0, dan, bLeftRebar)-1);
				CRebarInfoCycle *pRC2U	= pWall->GetRebarInfoCycle(1, dan, bLeftRebar, pWall->GetCountRebarInfoCycle(1, dan, bLeftRebar)-1);
				BOOL bUpperExist	= pRC1U && pRC1U->m_bExist || pRC2U && pRC2U->m_bExist ? TRUE : FALSE;
				
				// 상하면이 같은 철근이면 하나의 철근으로 이루어 진것이다(하면만 파자)
				if(pRC1L == pRC1U && pRC2L == pRC2U)
				{
					//상면만 있는 형태인지 하면만 있는 형태인지 알수 없다 (1사이클만체크)
					BOOL bLower_KindUpper_1Cycle = pRC1L &&pRC1L->m_sDescription.Find(_T("상부")) > -1? TRUE : FALSE;		//상부슬래브 철근인데 하부슬래브로 표기됨
										
					if(bLower_KindUpper_1Cycle == FALSE)
						bUpperExist	= FALSE;
				}
				if(bUpper && !bUpperExist) continue;
				if(!bUpper && !bLowerExist) continue;


				if(nUpper == -1 || nUpper == upper) 
				{
					CTwinVectorArray tvArrCuttingArea;
					pWall->GetTvCuttingArea_Side(tvArrCuttingArea, bLeftRebar, dan, bUpper);
					if(tvArrCuttingArea.GetSize() > 0)
					{
						rb.GetTvArrayByCrossTvArrayArea(tvArr, tvArrCuttingArea, FALSE);
						if(bDrawCuttingArea)
						{
							m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);

							tvArrCuttingArea.RemoveAt(tvArrCuttingArea.GetIdxByInfo("그릴때제거"));
							baseDraw.DrawTvArray(&Dom, tvArrCuttingArea);
						}
					}
				}
			}
		}

		// 임의로 파는 영역
		for(long cut = 0; cut < 1; cut++)
		{
			CTwinVectorArray tvArrCuttingArea;
			tvArrCuttingArea	= pWall->m_tvArrCuttingArea_Side_For1Dan;
			if(tvArrCuttingArea.GetSize() > 0)
			{
				rb.GetTvArrayByCrossTvArrayArea(tvArr, tvArrCuttingArea, FALSE);
				if(bDrawCuttingArea)
				{
					m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);

					tvArrCuttingArea.RemoveAt(tvArrCuttingArea.GetIdxByInfo("그릴때제거"));
					baseDraw.DrawTvArray(&Dom, tvArrCuttingArea);
				}
			}
		}
	}
	else
	{
		// 2, 3단일 경우는 cuttingArea안만 골라 내야 되는데.
		// upper이 -1일 경우 상하면 모두 안만 골라내게 된다.
		// 그러면 상면골라낸후 하면 골라내면 모두 사라지므로 array를 2개 만들어서 하나씩 골라낸뒤 합쳐야 된다.
		CTwinVectorArray tvArrUpper, tvArrLower;
		tvArrUpper	= tvArr;
		tvArrLower	= tvArr;
		tvArr.RemoveAll();

		for(long upper = 0; upper < 2; upper++)
		{
			BOOL bUpper	= upper == iUPPER ? TRUE : FALSE;
			if(nUpper == -1 || nUpper == upper) 
			{
				CTwinVectorArray tvArrCuttingArea;
				pWall->GetTvCuttingArea_Side(tvArrCuttingArea, bLeftRebar, nDan, bUpper);
				if(tvArrCuttingArea.GetSize() > 0)
				{
					rb.GetTvArrayByCrossTvArrayArea(bUpper ? tvArrUpper : tvArrLower, tvArrCuttingArea, TRUE);
					if(bDrawCuttingArea)
					{
						m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);

						tvArrCuttingArea.RemoveAt(tvArrCuttingArea.GetIdxByInfo("그릴때제거"));
						baseDraw.DrawTvArray(&Dom, tvArrCuttingArea);
					}
				}
			}
		}

		//#0028185 Trim구역이 잘못 설정이 되어있어서 OW26, OW22의 철근 마킹이 잘못 표현이 되고있었음
		if(nUpper == iUPPER) tvArrLower.RemoveAll();
		if(nUpper == iLOWER) tvArrUpper.RemoveAll();
		tvArr	= tvArrLower;
		
		tvArr.AddFromTvArray(tvArrUpper);
	}
	//////////////////////////////////////////////////////////////////////////////



	*pDomP << Dom;
	m_pStd->m_pARoadOptionStd->SetEnvType(pDomP, envType);
}

// 벽체 전단철근 측면도
void CDrawWall::DrawRebarShear_Side(CDomyun *pDomP, CWallApp *pWall, long nDrawLeft, BOOL bLeftWall)
{
	if(!pWall) return;
	if(pWall->m_bNotRebar) return;

	CDomyun Dom(pDomP);
	CHgBaseDrawStd baseDraw;
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);
	CConcSymbol sym(m_pStd->m_pARoadOptionStd);

	CTwinVectorArray tvArr;	// 철근 좌표
	pWall->GetTvRebarShear_Side(tvArr, bLeftWall);

	// 전단철근 모양으로 수정
	CRebarPlacing rb;
	rb.GetTvArrShearRebar(tvArr, FALSE, pWall->m_rebarShearDetail.m_nType == TYPE_SHEAR_DIGUT || pWall->m_rebarShearDetail.m_nType == TYPE_SHEAR_SPACER || pWall->m_rebarShearDetail.m_nType == TYPE_SHEAR_SINGLE_HOOK, FALSE);

	// nDrawLeft에 따라서 필요한 것만 골라냄
	if(nDrawLeft != 0)
	{
		CTwinVectorArray tvArrArea;
		pWall->GetTvCuttingArea_Side_Shear(tvArrArea, nDrawLeft);
		rb.GetTvArrayByCrossTvArrayArea(tvArr, tvArrArea, TRUE);

		// 자르는 타입이 원인 경우 자르는 영역을 그려준다.
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
		baseDraw.DrawTvArray(&Dom, tvArrArea);
	}

	// 그린다.
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);	
	baseDraw.DrawTvArray(&Dom, tvArr);

	*pDomP << Dom;
}

// 벽체 측면도를 그린다.
// 단, 기둥식에 대해서 그리는 함수임.
// 기둥식은 좌측면 우측면 나눌 필요가 없다. 흐흐흐
CDRect CDrawWall::DrawRebar_Column_SideStd(CDomyun *pDomP, CWallApp *pWall, BOOL bMarkRebar, BOOL bDimRebar)
{
	if(!pWall) return CDRect(0, 0, 0, 0); 
	if(!pWall->m_bIs) return CDRect(0, 0, 0, 0); 
	if(!pWall->m_bTopIsColumn) return CDRect(0, 0, 0, 0);
	double dSta	= pWall->m_xyOrg.x;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	// 외곽선 그리기
	DrawLeftSide(&Dom, pWall);
	
	m_pStd->m_pDrawFooting->DrawLeftSide(&Dom, &pWall->m_footing, FALSE);

	Dom.SetCalcExtRect();
	CDRect rectTot	= Dom.GetExtRect();
	rectTot.left	= pWall->m_footing.m_tvPathLeft.GetXyLeft().x;

	for(long col = 0; col < pWall->m_nCountColumn; col++)
	{
		CColumnApp *pColumn	= pWall->GetColumn(col);

		long nJijum = m_pStd->m_pRcBridge->GetJijumWallByStation(dSta);
		long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			if(col!=0 && dan > 0) continue;

			BOOL bDim	= col==0 && bDimRebar; 
			BOOL bMark	= col==0 && bMarkRebar;
			BOOL bIsTotalDim	= pColumn->GetCountDan_RebarMain()-1 == dan;
			BOOL bApplyCuttingCircle	= col==0;

			m_pStd->m_pDrawColumn->DrawRebarMain_Front(&Dom, pColumn, TRUE, dan, bDim, bMark, FALSE, FALSE, bApplyCuttingCircle);
			m_pStd->m_pDrawColumn->DrawRebarRound_Front(&Dom, pColumn, nJijum, TRUE, dan, bDim, bMark, FALSE, bIsTotalDim, bApplyCuttingCircle);
		}
	}

	// 기초 횡단면도
	// 기초 주철근
	m_pStd->m_pDrawFooting->DrawRebar_SideStd(&Dom, &pWall->m_footing, bDimRebar, bMarkRebar, FALSE);

	// 상부슬래브 횡단면도
	long nJ	= m_pStd->m_pRcBridgeRebar->GetJijumNumberByStation(dSta);
	m_pStd->m_pDrawCrossSection->DrawCrossSection(&Dom, dSta, ToDPointFrDegree(pWall->m_dAnglePath), FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE, TRUE);
	m_pStd->m_pDrawCrossSection->DrawCrossSlabHunch(&Dom, dSta, ToDPointFrDegree(pWall->m_dAnglePath), FALSE, 0, FALSE, TRUE);
	m_pStd->m_pDrawCrossSection->DrawRebarShear_CrossSection_SlabGirder(&Dom, nJ, TRUE, TRUE, 0);
	m_pStd->m_pDrawCrossSection->DrawRebarSupport_CrossSection_UpperSlabGirder(&Dom, nJ, TRUE, TRUE, 0);

	//m_pStd->m_pDrawCrossSection->DrawRebar_CrossSectionStd(&Dom, dSta, dSta, bDimRebar, FALSE, bMarkRebar, FALSE, FALSE);


	// 단면 마크 "A"
	DrawSectionMark_Side(&Dom, pWall);

	*pDomP << Dom;


	return rectTot;
}

void CDrawWall::DimRebarMain_Front(CDomyun *pDomP, CWallApp *pWall, long nDimDan, BOOL bInput, long nCycle, long nDan)
{
	if(!pWall) return;

	if(nCycle == -1) nCycle = pWall->GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > pWall->GetCountCycleMainRebar()-1) return;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;
	if(pWall->m_bNotRebar) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CString sNum	= "WD";
	long nNum		= 1;

	// 좌측
	long left = 0; for(left = 0; left < 2; left++)
	{
		BOOL bLeft	= left == iLEFT;
		// 치수기입용 변수
		CDPointArray xyArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		CString sDir = _T("");
		CDPoint xyStt(0, 0);

		sDir = bLeft ? _T("LEFT") : _T("RIGHT");
		CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &pWall->m_pArrRCMain[nCycle][left][nDan];

		if(pArrRC)
		{
			BOOL bAllNotExist	= TRUE;
			for(long e = 0; e < pArrRC->GetSize(); e++)
			{
				if(pArrRC->GetAt(e)->m_bExist)
				{
					bAllNotExist	= FALSE;
					break;
				}
			}
			if(bAllNotExist) continue;

			// 치수정보 분석해서 가져옴
			double rad	= pWall->m_nType == WALL_TYPE_SLOPE ? ToRadian(90-fabs(90-pWall->m_dAngle)) : 1;
			CFootingApp *pFooting	= &pWall->m_footing;
 			double dCoverStt		= pFooting->m_dCoverLower[0]/rad;
			double dCoverEnd		= pWall->m_dCoverUpper_UpperSlab/rad;
			BOOL bApplyEndIs2		= TRUE;
			BOOL bApplyJoint		= bApplyEndIs2;
			m_pStd->GetDimInfoAtRebarInfoCycle(pArrRC, TRUE, xyArrDim, sArrText, FALSE, bApplyJoint, bApplyEndIs2, FALSE, dCoverStt, dCoverEnd);

			if(xyArrDim.GetSize() > 0)
			{
				// 가져온 정보를 입력에 맞게 기호로 변경
				if(bInput)
				{
					long nSize	= sArrText.GetSize();
					CString str	= _T("");
					long i = 0; for(i = 1; i < nSize-1; i++)
					{
						if(sArrText.GetAt(i) == "1")
						{
							str.Format("%s%d", sNum, nNum++);
							sArrText.SetAt(i, str);
						}
						else
							sArrText.SetAt(i, "0");
					}
				}
				else
				{
					sArrText.RemoveAll();
				}
				m_baseDraw.AutoDimCont(&Dom, xyArrDim.GetAt(0), xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE, TRUE, TRUE);

				*pDomP << Dom;
			}
		}
	}
	*pDomP << Dom;
}

// 기초 배력근과 전단철근 배근을 위해 참조할 기초/벽체 구조물 치수(피복 포함) - 입력용
void CDrawWall::DimFootingUpperReferForSupAndShearRebar(CDomyun *pDomP, CWallApp *pWall)
{
	if(!pWall) return;
	CFootingApp *pFooting	= &pWall->m_footing;
	if(!pFooting->m_bIs) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CDPointArray xyArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	long nDimDan	= 0;
	CString sDir	= _T("TOP");
	CDPoint xyStt	= CDPoint(0, 0);

	CTwinVector tv;
	CTwinVector tvLeft, tvRight;
	CTwinVector tvWallL, tvWallR;
	tvLeft	= pFooting->m_tvArrFront.GetTvByInfo("기초좌측면");
	tvRight	= pFooting->m_tvArrFront.GetTvByInfo("기초우측면");
	tvWallL	= pWall->m_tvArrFront.GetTvByInfo("벽체좌측면");
	tvWallR	= pWall->m_tvArrFront.GetTvByInfo("벽체우측면");
	xyStt.x	= tvLeft.GetXyTop().x;
	xyStt.y	= max(tvLeft.GetXyTop().y, tvRight.GetXyTop().y);


	CRebarInfoCycle *pRCFootingUpper	= pFooting->GetRebarInfoCycle(0, TRUE, 0);
	CRebarInfoCycle *pRCWallLeft		= pWall->GetRebarInfoCycle(0, 0, TRUE, 0);
	CRebarInfoCycle *pRCWallRight		= pWall->GetRebarInfoCycle(0, 0, FALSE, 0);

	if(pRCWallLeft && pRCWallRight && pWall->IsVertWall())
	{
		tvWallL.m_v1.x	= pRCWallLeft->m_xyEnd.x - pWall->m_dCoverLeft[0];
		tvWallL.m_v2.x	= tvWallL.m_v1.x;
		tvWallR.m_v1.x	= pRCWallRight->m_xyEnd.x + pWall->m_dCoverRight[0];
		tvWallR.m_v2.x	= tvWallR.m_v1.x;
	}
	else
	{
		tvWallL.m_v1	= tvWallL.m_v2;
		tvWallR.m_v1	= tvWallR.m_v2;
	}

	long i = 0; for(i = 0; i < pFooting->m_tvArrFront.GetSize(); i++)
	{
		tv	= pFooting->m_tvArrFront.GetAt(i);
		xyArrDim.Add(tv.m_v1);
		xyArrDim.Add(tv.m_v2);
	}

	if(pFooting->m_nType == FOOTING_TYPE_BASIC_LEFTSLOPE)
	{
		if(pRCFootingUpper)
		{
			xyArrDim.Add(pRCFootingUpper->m_xyStt);
		}
	}
	else
	{
		
		if(pRCFootingUpper)
			xyArrDim.Add(pRCFootingUpper->m_xyMid1);
	}

	double dXLeftWall	= tvWallL.GetXyBottom().x;
	double dXRightWall	= tvWallR.GetXyBottom().x;

	if(tvWallL.GetLength() > 0 && tvWallR.GetLength() > 0)
	{
		xyArrDim.Add(tvWallL.GetXyBottom());
		xyArrDim.Add(tvWallR.GetXyBottom());
	}
	else
		dXLeftWall	= dXRightWall;

	if(pFooting->m_nType == FOOTING_TYPE_BASIC || pFooting->m_nType == FOOTING_TYPE_BASIC_BOTH)
	{
		if(pRCWallLeft)
			xyArrDim.Add(pRCWallLeft->m_xyMid2);

		if(pRCWallRight)
			xyArrDim.Add(pRCWallRight->m_xyMid2);

		if(pRCWallLeft && pRCWallRight)
		{
			dXLeftWall	= pRCWallLeft->m_xyMid2.x;
			dXRightWall	= pRCWallRight->m_xyMid2.x;
		}
	}

	if(pFooting->m_nType == FOOTING_TYPE_BASIC_RIGHTSLOPE)
	{
		if(pRCFootingUpper)
		{
			xyArrDim.Add(pRCFootingUpper->m_xyEnd);
		}
	}
	else
	{
		if(pRCFootingUpper)
			xyArrDim.Add(pRCFootingUpper->m_xyMid2);
	}

	CDRect rect	= pFooting->m_tvArrFront.GetRect();
	for(i = 0; i < xyArrDim.GetSize(); i++)
	{
		CDPoint xy	= xyArrDim.GetAt(i);
		// 벽체 철근 사이에 있는 치수 제거
		if(pRCWallLeft && pRCWallRight)
		{
			if(xy.x > dXLeftWall && xy.x < dXRightWall)
			{
				xyArrDim.RemoveAt(i);
				i--;
				continue;
			}
		}

		xy.y	= rect.top;
		xyArrDim.SetAt(i, xy);
	}


	CRebarPlacing rb;
	rb.RemoveSamePoint(xyArrDim, TRUE);
	xyArrDim.Sort(TRUE);

	CHgBaseDrawStd baseDraw;
	baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, FALSE, FALSE);

	*pDomP << Dom;
}

void CDrawWall::ApplyCuttingArea_Side_ForGagak(CDomyun *pDom, CWallApp *pWall, CTwinVectorArray &tvArr, long nLeft, BOOL bDrawCuttingArea)
{
	if(pWall->m_nTypeOutputMainRebar_Side != 1) return;

	BOOL bLeft	= nLeft == -1 || nLeft == 0;
	CDomyun Dom(pDom);
	long envType	= m_pStd->m_pARoadOptionStd->GetEnvType(pDom);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	
	// 구해진 철근에 cutting Area 적용
	// 주철근을 파서 표현하는 옵션인 경우..
	CRebarPlacing rb;
	long nIdxStt	= 0;
	long nIdxEnd	= nLeft == 0 ? 2 : 1;
	CTwinVectorArray tvArr1, tvArr2;
	tvArr1	= tvArr;
	tvArr2	= tvArr;
	long i = 0; for(i = nIdxStt; i < nIdxEnd; i++)
	{
		if(nLeft == 0)
		{
			bLeft = i == 0;
		}

		CDPoint xyCen(0, 0);
		double dR(0), dAngStt(0), dAngEnd(0);
		pWall->GetInfoCuttingCircle_Side(bLeft, xyCen, dR, dAngStt, dAngEnd);

		rb.GetTvArrayByCircleArea(i == 0 ? tvArr1 : tvArr2, xyCen, dR, TRUE);				

		if(bDrawCuttingArea)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
			Dom.Arc(xyCen, dR, dAngStt, dAngEnd-dAngStt);
		}
	}

	tvArr.RemoveAll();
	tvArr	= tvArr1;
	if(nLeft == 0)
		tvArr.AddFromTvArray(tvArr2);

	*pDom << Dom;
	m_pStd->m_pARoadOptionStd->SetEnvType(pDom, envType);
}
