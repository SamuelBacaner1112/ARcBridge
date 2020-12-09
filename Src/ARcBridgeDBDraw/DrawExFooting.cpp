// DrawExFooting.cpp: implementation of the CDrawExFooting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARcBridgeDBDraw.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawExFooting::CDrawExFooting()
{

}

CDrawExFooting::~CDrawExFooting()
{

}

// Path의 기준면을 정면으로 취급
// bBack : 기준면 반대편에서 본 모양 
void CDrawExFooting::DrawFront(CDomyun *pDomP, CExFootingApp *pExFooting, BOOL bBack, BOOL bCuttingPile, CTwinVectorArray *tvArrReturn, BOOL bDrawPile, BOOL bDimPile, BOOL bApplyAngle)
{
	if(!pExFooting) return;
	if(!pExFooting->m_bIs) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	m_pStd->DrawTvArray(&Dom, pExFooting->m_tvArrFront, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());
	tvArrReturn->ChangePos(pExFooting->m_tvArrFront);

	if(pExFooting->m_nType == EXFOOTING_TYPE_PILE && bDrawPile)
	{
		CDrawExPile drawExPile(&pExFooting->m_Pile, m_pStd->m_pARoadOptionStd);
		drawExPile.DrawFront(&Dom, bCuttingPile, bBack, bApplyAngle);

		if(bDimPile)
		{
			drawExPile.DimTextFront(&Dom, bCuttingPile, FALSE, bApplyAngle);
		}
	}

	*pDomP << Dom;
}

// 확대기초 하단 치수
void CDrawExFooting::DimFrontLower(CDomyun *pDomP, CExFootingApp *pExFooting, long nDimDan, BOOL bInput, BOOL bSide)
{
	if(!pExFooting) return;
	if(!pExFooting->m_bIs) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDomP, &Dom);

	BOOL bApplyAngle	= pExFooting->IsApplyAngle();

	CTwinVector tv;
	CTwinVectorArray *pTvArr = bSide ? &pExFooting->m_tvArrSide : &pExFooting->m_tvArrFront;
	BOOL bFrontDanMass	= pExFooting->m_bFrontDanMass;

	if(nDimDan < 0) nDimDan = 0;
	CDoubleArray dArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("BOTTOM");
	CDPoint xyStt(0, 0);
	CDPoint xySttTot(0, 0), xyEndTot(0, 0);
	double dExtendLeft = 0;	// 기초보다 좌측으로 더 나간 양
	double dExtendRight = 0;	// 기초보다 우측으로 더 나간양.
	double dExtendLeftFix = 0;	// 기초보다 좌측으로 더 나간 양(사각적용안함)
	double dExtendRightFix = 0;	// 기초보다 우측으로 더 나간양(사각적용안함)

	pTvArr->GetTvByInfo("확대기초상단좌측면", tv);
	dExtendLeft		= tv.GetHorLength(bApplyAngle);
	dExtendLeftFix	= tv.GetHorLength(FALSE);
	dArrDim.Add(dExtendLeft);
	if(bInput)
	{
		bSide ? sArrText.Add("EFB") : sArrText.Add("EFL");
		sArrUnderText.Add(COMMA3(dExtendLeft, dExtendLeftFix));
	}
	else
	{
		sArrText.Add(COMMA(dExtendLeft));
		sArrUnderText.Add(COMMA2(tv.GetHorLength(FALSE)));
	}
			
	pTvArr->GetTvByInfo("확대기초상단우측면", tv);
	dExtendRight = tv.GetHorLength(bApplyAngle);
	dExtendRightFix	= tv.GetHorLength(FALSE);

	if(pExFooting->m_nType == EXFOOTING_TYPE_MASS && ((bFrontDanMass && !bSide) || (!bFrontDanMass && bSide)))
	{
		if(pExFooting->m_bLeftOrgOfMass)
		{
			CString str = _T("");
// 			double dW	= 0;
// 			double dWFix	= 0;
			long i = 0; for(i = 0; i < pExFooting->m_nCountDan; i++)
			{
				//dX = (i == 0) ? dExtendRight : 0;
				str.Format("%d", i+1);
				pTvArr->GetTvByInfo("확대기초"+str+"번째단하면", tv);
				double dW	= i == 0 ? tv.GetHorLength(bApplyAngle)-dExtendLeft : tv.GetHorLength(bApplyAngle);
				double dWFix	= i == 0 ? tv.GetHorLength(FALSE)-dExtendLeft : tv.GetHorLength(FALSE);
				dArrDim.Add(dW);
				if(bInput)
				{
					sArrText.Add("EW"+str);
					sArrUnderText.Add(COMMA3(dW, dWFix));
				}
				else
				{
					sArrText.Add(COMMA(dW));

					dW	= i == 0 ? tv.GetHorLength(FALSE)-dExtendLeftFix : tv.GetHorLength(FALSE);
					sArrUnderText.Add(COMMA2(dW));
				}

				str.Format("%d", i+1);
				pTvArr->GetTvByInfo("확대기초"+str+"번째단경사", tv);
				dArrDim.Add(tv.GetHorLength(bApplyAngle));
				if(bInput)
				{
					sArrText.Add("ES"+str);
					sArrUnderText.Add(COMMA3(tv.GetHorLength(TRUE), tv.GetHorLength(FALSE)));
				}
				else
				{
					sArrText.Add(COMMA(tv.GetHorLength(bApplyAngle)));
					sArrUnderText.Add(COMMA2(tv.GetHorLength(FALSE)));
				}
			}
			pTvArr->GetTvByInfo("확대기초우측하면", tv);
			if(pExFooting->m_nCountDan > 0)
			{
				dArrDim.Add(tv.GetHorLength(bApplyAngle)-dExtendRight);
				sArrText.Add(COMMA(tv.GetHorLength(bApplyAngle)-dExtendRight));
				sArrUnderText.Add(COMMA2(tv.GetHorLength(FALSE)-dExtendRightFix));
			}
			else
			{
				dArrDim.Add(tv.GetHorLength(bApplyAngle)-dExtendRight-dExtendLeft);
				sArrText.Add(COMMA(tv.GetHorLength(bApplyAngle)-dExtendRight-dExtendLeft));
				sArrUnderText.Add(COMMA2(tv.GetHorLength(FALSE)-dExtendRightFix-dExtendLeftFix));
			}
		}
		else
		{
			pTvArr->GetTvByInfo("확대기초좌측하면", tv);
			if(pExFooting->m_nCountDan > 0)
			{
				dArrDim.Add(tv.GetHorLength(bApplyAngle)-dExtendLeft);
				sArrText.Add(COMMA(tv.GetHorLength(bApplyAngle)-dExtendLeft));
				sArrUnderText.Add(COMMA2(tv.GetHorLength(FALSE)-dExtendLeftFix));
			}
			else
			{
				dArrDim.Add(tv.GetHorLength(bApplyAngle)-dExtendLeft-dExtendRight);
				sArrText.Add(COMMA(tv.GetHorLength(bApplyAngle)-dExtendLeft-dExtendRight));
				sArrUnderText.Add(COMMA2(tv.GetHorLength(FALSE)-dExtendLeftFix-dExtendRightFix));
			}

			CString str = _T("");
// 			double dX	= 0;
// 			double dW	= 0;
// 			double dWFix	= 0;
			long i = 0; for(i = pExFooting->m_nCountDan-1; i >= 0; i--)
			{
				str.Format("%d", i+1);
				pTvArr->GetTvByInfo("확대기초"+str+"번째단경사", tv);
				dArrDim.Add(tv.GetHorLength(bApplyAngle));
				if(bInput)
				{
					sArrText.Add("ES"+str);
					sArrUnderText.Add(COMMA3(tv.GetHorLength(TRUE), tv.GetHorLength(FALSE)));
				}
				else
				{
					sArrText.Add(COMMA(tv.GetHorLength(bApplyAngle)));
					sArrUnderText.Add(COMMA2(tv.GetHorLength(FALSE)));
				}

				double dX = (i == 0) ? dExtendRight : 0;
				pTvArr->GetTvByInfo("확대기초"+str+"번째단하면", tv);
				double dW	= tv.GetHorLength(bApplyAngle);
				double dWFix	= tv.GetHorLength(FALSE);
				dArrDim.Add(dW-dX);
				if(bInput)
				{
					sArrText.Add("EW"+str);
					sArrUnderText.Add(COMMA3(dW-dX, dWFix-dX));
				}
				else
				{
					sArrText.Add(COMMA(dW-dX));
					
					dX	= (i == 0) ? dExtendRightFix : 0;
					dW	= tv.GetHorLength(FALSE);
					sArrUnderText.Add(COMMA2(dW-dX));
				}
			}
		}
	}
	else// if(pExFooting->m_nType == EXFOOTING_TYPE_LEAN || pExFooting->m_nType == EXFOOTING_TYPE_PILE)
	{
		double dWidth = 0;
		pTvArr->GetTvByInfo("확대기초상단좌측마진", tv);
		dWidth += tv.GetHorLength(bApplyAngle);
		pTvArr->GetTvByInfo("확대기초상단우측마진", tv);
		dWidth += tv.GetHorLength(bApplyAngle);
		dArrDim.Add(dWidth);
		sArrText.Add(COMMA(dWidth));

		dWidth = 0;
		pTvArr->GetTvByInfo("확대기초상단좌측마진", tv);
		dWidth += tv.GetHorLength(FALSE);
		pTvArr->GetTvByInfo("확대기초상단우측마진", tv);
		dWidth += tv.GetHorLength(FALSE);
		sArrUnderText.Add(COMMA2(dWidth));
	}
	
	// 확대기초상단우측면
	dArrDim.Add(dExtendRight);
	if(bInput)
	{
		bSide ? sArrText.Add("EFF") : sArrText.Add("EFR");
		sArrUnderText.Add(COMMA3(dExtendRight, dExtendRightFix));
	}
	else
	{
		sArrText.Add(COMMA(dExtendRight));
		sArrUnderText.Add(COMMA2(dExtendRightFix));
	}

	// 시작위치 구함.
	pTvArr->GetTvByInfo("확대기초좌측면", tv);
	xyStt = CDPoint(tv.m_v1.x, tv.GetXyBottom().y);

	pTvArr->GetTvByInfo("확대기초우측면", tv);

	//32691
	double dYStt = xyStt.y;
	double dYTv = tv.GetXyBottom().y;
	//}

	xyStt.y = min(xyStt.y, tv.GetXyBottom().y);
	

	// 32691 기초 측면의 y값이 다르다면 치수선의 단을 -1로 해야 하기 때문에 뭔가 다른 방법을 써줘야 한다.
	if(!Compare(dYStt,  dYTv, _T("="), 0.1))
		nDimDan = 1;


	// 모두 직교일 경우 괄호안 치수는 의미가 없다. //////////
	m_pStd->ModifyInfoAutoDimConc(sArrText, sArrUnderText, bInput);
	///////////////////////////////////////////////////////////

	CHgBaseDrawStd baseDraw;
	baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE);

	// 매스 경사 치수
	DimTextFront_MassSlope(&Dom, pExFooting, bSide);
	*pDomP << Dom;
}

void CDrawExFooting::DimFrontLowerBox(CDomyun *pDomP, CRcBridgeApp *pBri, long nDimDan, BOOL bInput, BOOL bSide)
{
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_BOX) return;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDomP, &Dom);

	BOOL bApplyAngle = TRUE;
	CExFootingApp *pExFoot = &pBri->m_footingBox.m_exFooting;

	CTwinVector tv;
	CTwinVectorArray *pTvArr = bSide ? &pExFoot->m_tvArrSide : &pExFoot->m_tvArrFront;
	BOOL bFrontDanMass	= pExFoot->m_bFrontDanMass;

	if(nDimDan < 0) nDimDan = 0;
	CDoubleArray dArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("BOTTOM");
	CDPoint xyStt(0, 0);
	CDPoint xySttTot(0, 0), xyEndTot(0, 0);
	double dExtendLeft = 0;	// 기초보다 좌측으로 더 나간 양
	double dExtendRight = 0;	// 기초보다 우측으로 더 나간양.
	double dExtendLeftFix = 0;	// 기초보다 좌측으로 더 나간 양(사각적용안함)
	double dExtendRightFix = 0;	// 기초보다 우측으로 더 나간양(사각적용안함)

	pTvArr->GetTvByInfo("확대기초상단좌측면", tv);
	dExtendLeft		= tv.GetHorLength(bApplyAngle);
	dExtendLeftFix	= tv.GetHorLength(FALSE);
	dArrDim.Add(dExtendLeft);
	if(bInput)
	{
		bSide ? sArrText.Add("EFB") : sArrText.Add("EFL");
		sArrUnderText.Add(COMMA3(dExtendLeft, dExtendLeftFix));
	}
	else
	{
		sArrText.Add(COMMA(dExtendLeft));
		sArrUnderText.Add(COMMA2(tv.GetHorLength(FALSE)));
	}

	pTvArr->GetTvByInfo("확대기초상단우측면", tv);
	dExtendRight = tv.GetHorLength(bApplyAngle);
	dExtendRightFix	= tv.GetHorLength(FALSE);

	if(pBri->m_nTypeLower == LOWERSLAB_TYPE_MASS && ((bFrontDanMass && !bSide) || (!bFrontDanMass && bSide)))
	{
		if(pExFoot->m_bLeftOrgOfMass)
		{
			CString str = _T("");
// 			double dW	= 0;
// 			double dWFix	= 0;
			long i = 0; for(i = 0; i < pExFoot->m_nCountDan; i++)
			{
				//dX = (i == 0) ? dExtendRight : 0;
				str.Format("%d", i+1);
				pTvArr->GetTvByInfo("확대기초"+str+"번째단하면", tv);
				double dW	= i == 0 ? tv.GetHorLength(bApplyAngle)-dExtendLeft : tv.GetHorLength(bApplyAngle);
				double dWFix	= i == 0 ? tv.GetHorLength(FALSE)-dExtendLeft : tv.GetHorLength(FALSE);
				dArrDim.Add(dW);
				if(bInput)
				{
					sArrText.Add("EW"+str);
					sArrUnderText.Add(COMMA3(dW, dWFix));
				}
				else
				{
					sArrText.Add(COMMA(dW));

					dW	= i == 0 ? tv.GetHorLength(FALSE)-dExtendLeftFix : tv.GetHorLength(FALSE);
					sArrUnderText.Add(COMMA2(dW));
				}

				str.Format("%d", i+1);
				pTvArr->GetTvByInfo("확대기초"+str+"번째단경사", tv);
				dArrDim.Add(tv.GetHorLength(bApplyAngle));
				if(bInput)
				{
					sArrText.Add("ES"+str);
					sArrUnderText.Add(COMMA3(tv.GetHorLength(TRUE), tv.GetHorLength(FALSE)));
				}
				else
				{
					sArrText.Add(COMMA(tv.GetHorLength(bApplyAngle)));
					sArrUnderText.Add(COMMA2(tv.GetHorLength(FALSE)));
				}
			}
			pTvArr->GetTvByInfo("확대기초우측하면", tv);
			if(pExFoot->m_nCountDan > 0)
			{
				dArrDim.Add(tv.GetHorLength(bApplyAngle)-dExtendRight);
				sArrText.Add(COMMA(tv.GetHorLength(bApplyAngle)-dExtendRight));
				sArrUnderText.Add(COMMA2(tv.GetHorLength(FALSE)-dExtendRightFix));
			}
			else
			{
				dArrDim.Add(tv.GetHorLength(bApplyAngle)-dExtendRight-dExtendLeft);
				sArrText.Add(COMMA(tv.GetHorLength(bApplyAngle)-dExtendRight-dExtendLeft));
				sArrUnderText.Add(COMMA2(tv.GetHorLength(FALSE)-dExtendRightFix-dExtendLeftFix));
			}
		}
		else
		{
			pTvArr->GetTvByInfo("확대기초좌측하면", tv);
			if(pExFoot->m_nCountDan > 0)
			{
				dArrDim.Add(tv.GetHorLength(bApplyAngle)-dExtendLeft);
				sArrText.Add(COMMA(tv.GetHorLength(bApplyAngle)-dExtendLeft));
				sArrUnderText.Add(COMMA2(tv.GetHorLength(FALSE)-dExtendLeftFix));
			}
			else
			{
				dArrDim.Add(tv.GetHorLength(bApplyAngle)-dExtendLeft-dExtendRight);
				sArrText.Add(COMMA(tv.GetHorLength(bApplyAngle)-dExtendLeft-dExtendRight));
				sArrUnderText.Add(COMMA2(tv.GetHorLength(FALSE)-dExtendLeftFix-dExtendRightFix));
			}

			CString str = _T("");
// 			double dX	= 0;
// 			double dW	= 0;
// 			double dWFix	= 0;
			long i = 0; for(i = pExFoot->m_nCountDan-1; i >= 0; i--)
			{
				str.Format("%d", i+1);
				pTvArr->GetTvByInfo("확대기초"+str+"번째단경사", tv);
				dArrDim.Add(tv.GetHorLength(bApplyAngle));
				if(bInput)
				{
					sArrText.Add("ES"+str);
					sArrUnderText.Add(COMMA3(tv.GetHorLength(TRUE), tv.GetHorLength(FALSE)));
				}
				else
				{
					sArrText.Add(COMMA(tv.GetHorLength(bApplyAngle)));
					sArrUnderText.Add(COMMA2(tv.GetHorLength(FALSE)));
				}

				double dX = (i == 0) ? dExtendRight : 0;
				pTvArr->GetTvByInfo("확대기초"+str+"번째단하면", tv);
				double dW	= tv.GetHorLength(bApplyAngle);
				double dWFix	= tv.GetHorLength(FALSE);
				dArrDim.Add(dW-dX);
				if(bInput)
				{
					sArrText.Add("EW"+str);
					sArrUnderText.Add(COMMA3(dW-dX, dWFix-dX));
				}
				else
				{
					sArrText.Add(COMMA(dW-dX));

					dX	= (i == 0) ? dExtendRightFix : 0;
					dW	= tv.GetHorLength(FALSE);
					sArrUnderText.Add(COMMA2(dW-dX));
				}
			}
		}
	}
	else// if(pExFooting->m_nType == EXFOOTING_TYPE_LEAN || pExFooting->m_nType == EXFOOTING_TYPE_PILE)
	{
		double dWidth = 0;
		pTvArr->GetTvByInfo("확대기초상단", tv);
		dWidth += tv.GetHorLength(bApplyAngle);
		dArrDim.Add(dWidth);
		sArrText.Add(COMMA(dWidth));

		dWidth = 0;
		pTvArr->GetTvByInfo("확대기초상단", tv);
		dWidth += tv.GetHorLength(FALSE);
		sArrUnderText.Add(COMMA2(dWidth));
	}

	// 확대기초상단우측면
	dArrDim.Add(dExtendRight);
	if(bInput)
	{
		bSide ? sArrText.Add("EFF") : sArrText.Add("EFR");
		sArrUnderText.Add(COMMA3(dExtendRight, dExtendRightFix));
	}
	else
	{
		sArrText.Add(COMMA(dExtendRight));
		sArrUnderText.Add(COMMA2(dExtendRightFix));
	}

	// 시작위치 구함.
	pTvArr->GetTvByInfo("확대기초좌측면", tv);
	xyStt = CDPoint(tv.m_v1.x, tv.GetXyBottom().y);

	pTvArr->GetTvByInfo("확대기초우측면", tv);

	//32691
	double dYStt = xyStt.y;
	double dYTv = tv.GetXyBottom().y;
	//}

	xyStt.y = min(xyStt.y, tv.GetXyBottom().y);


	// 32691 기초 측면의 y값이 다르다면 치수선의 단을 -1로 해야 하기 때문에 뭔가 다른 방법을 써줘야 한다.
	if(!Compare(dYStt,  dYTv, _T("="), 0.1))
		nDimDan = 1;


	// 모두 직교일 경우 괄호안 치수는 의미가 없다. //////////
	m_pStd->ModifyInfoAutoDimConc(sArrText, sArrUnderText, bInput);
	///////////////////////////////////////////////////////////

	CHgBaseDrawStd baseDraw;
	baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE);

	// 매스 경사 치수
	DimTextFront_MassSlope(&Dom, pExFoot, bSide);
	*pDomP << Dom;
}

// 확대기초 좌측
// bLeft 는 직접기초일때 적용 : 왼쪽에 기입할지 오른쪽에 기입할지.
// mass일때는 어짜피 양쪽 모두 치수가 들어가므로 의미없음.
void CDrawExFooting::DimFrontSide(CDomyun *pDomP, CExFootingApp *pExFooting, BOOL bLeft, long nDimDan, BOOL bInput, BOOL bSide)
{
	if(!pExFooting) return;
	if(!pExFooting->m_bIs) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDomP, &Dom);
	
	CTwinVector tv;
	CTwinVectorArray *pTvArr = bSide ? &pExFooting->m_tvArrSide : &pExFooting->m_tvArrFront;
	BOOL bFrontDanMass	= pExFooting->m_bFrontDanMass;

	if(nDimDan < 0) nDimDan = 0;
	CDoubleArray dArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = bLeft ? _T("LEFT") : _T("RIGHT");
	CDPoint xyStt(0, 0);

	if(pExFooting->m_nType == EXFOOTING_TYPE_MASS && ((bFrontDanMass && !bSide) || (!bFrontDanMass && bSide)))
	{
		CDRect rect	= pTvArr->GetRect();
// 		double dH	= 0;
		CString str	= _T("");
		if(pExFooting->m_bLeftOrgOfMass == bLeft)
		{
			double dH		= rect.top-rect.bottom;
			dArrDim.Add(dH);
			sArrText.Add("EH");
			sArrUnderText.Add("0");
		}
		else
		{
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

		xyStt = bLeft ? CDPoint(rect.left, rect.bottom) : CDPoint(rect.right, rect.bottom);

		if(!bInput)
		{
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();
		}
		CHgBaseDrawStd baseDraw;
		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE);
	}
	else //if(pExFooting->m_nType != EXFOOTING_TYPE_PILE)
	{
		pTvArr->GetTvByInfo(bLeft ? "확대기초좌측면" : "확대기초우측면", tv);
		dArrDim.Add(tv.GetVerLength());
		sArrText.Add("EH");
		sArrUnderText.Add(COMMA(tv.GetVerLength()));
		xyStt = bLeft ? tv.m_v1 : tv.m_v2;

		if(!bInput)
		{
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();
		}
		CHgBaseDrawStd baseDraw;
		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE);
	}

	*pDomP << Dom;
}

// 확대기초 상단에 지반 치수 기입
void CDrawExFooting::DimTextFrontEL(CDomyun *pDomP, CExFootingApp *pExFooting, BOOL bLeft)
{
	if(!pExFooting) return;
	if(!pExFooting->m_bIs) return;

	CConcSymbol sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CString sEL = _T("");
	CDPoint xyStt(0, 0);
	CTwinVector tv;
	double sw = bLeft ? -1 : 1;

	//  
	if(pExFooting->m_tvArrFront.GetTvByInfo(bLeft ? "확대기초좌측면" : "확대기초우측면", tv))
	{
		const double dGap	= 50;

		xyStt = tv.GetXyTop();
		xyStt.x += (dGap*sw);
		sEL		= m_pStd->GetStringEL(toM(xyStt.y));
		sym.ArrowElevation(&Dom, sEL, "", xyStt, bLeft, TRUE);

		Dom.LineTo(xyStt.x-Dom.Always(2), xyStt.y, xyStt.x+Dom.Always(2), xyStt.y);
	}

	*pDomP << Dom;
}

void CDrawExFooting::DrawPlanEarthAnchor(CDomyun *pDomP)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(pBri == NULL) return;
	if(pBri->IsBoxType() == FALSE || pBri->m_nTypeLower != BOXFOOTING_TYPE_ANCHOR) return;

	CDomyun Dom(pDomP);

	for(long ix = 0; ix < pBri->m_nCountAnchor; ix++)
	{
		long nCountSide = (long)(pBri->m_dArrEaAnchor.GetAt(ix));
		double dDia = pBri->m_dArrDiaAnchor.GetAt(ix); 

		for(long kk = 0; kk < nCountSide; kk++)
		{
			CDPoint xyAnchor = pBri->GetXyPilePos(&pBri->m_footingBox, ix, kk);

			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SYMB);
			Dom.Circle(xyAnchor, dDia / 2.);

			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
			Dom.LineTo(xyAnchor.x - dDia, xyAnchor.y, xyAnchor.x + dDia, xyAnchor.y);
			Dom.LineTo(xyAnchor.x, xyAnchor.y - dDia, xyAnchor.x, xyAnchor.y + dDia);
		}
	}

	*pDomP << Dom;
}


void CDrawExFooting::DrawPlan(CDomyun *pDomP, CExFootingApp *pExFooting, BOOL bDrawCenter, BOOL bHidden, BOOL bDrawPile, BOOL bDistCheck)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri || !pExFooting) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, bHidden ? HCAD_CONH : HCAD_CONC);
	
	m_pStd->DrawTvArray(&Dom, pExFooting->m_tvArrPlan, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());

	if(bDrawPile)
	{
		BOOL bEarthAnchor = (pBri->IsBoxType() && pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR) ? TRUE : FALSE;
		BOOL bPileBase = pBri->IsBoxType() ? pBri->IsPileBase(0) : pExFooting->m_nType == EXFOOTING_TYPE_PILE;
		if(bPileBase)
		{
			CDrawExPile drawExPile(&pExFooting->m_Pile, m_pStd->m_pARoadOptionStd);
			drawExPile.DrawPlan(&Dom, bDistCheck);
		}
		else if(bEarthAnchor)
		{
			DrawPlanEarthAnchor(&Dom);
		}

	}

	// 중심선 그림
	if(bDrawCenter)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
		double dMargin = 500;
		CTwinVector tv = pExFooting->GetTvExFootingTopCen();
			
		CDPoint xyDir = tv.GetXyDir();
		tv.m_v1 -= xyDir*dMargin;
		tv.m_v2 += xyDir*dMargin;
	
		Dom.LineTo(tv.m_v1, tv.m_v2);
	}

	*pDomP << Dom;
}

// 매스 경사 치수 기입
void CDrawExFooting::DimTextFront_MassSlope(CDomyun *pDomP, CExFootingApp *pExFooting, BOOL bSide)
{
	if(!pExFooting) return;
	if(!pExFooting->m_bIs) return;
	if(pExFooting->m_nType != EXFOOTING_TYPE_MASS) return;
	
	BOOL bFrontDanMass	= pExFooting->m_bFrontDanMass;
	if(bFrontDanMass && bSide) return;
	if(!bFrontDanMass && !bSide) return;
	CTwinVectorArray *pTvArr	= bSide ? &pExFooting->m_tvArrSide : &pExFooting->m_tvArrFront;

	CConcSymbol sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDomP, &Dom);

	CString sSlope	= _T("");
	CTwinVector tv;
	
	CHgBaseDrawStd baseDraw;
	long i = 0; for(i = 0; i < pExFooting->m_nCountDan; i++)
	{
		sSlope.Format("확대기초%d번째단경사", i+1);
		if(pTvArr->GetTvByInfo(sSlope, tv))
		{
			baseDraw.DrawSlopeTriangle(&Dom, tv.m_v1, tv.m_v2, 500);
		}
	}


	*pDomP << Dom;
}

void CDrawExFooting::DimFootPlane(CDomyun* pDomP, int nJijum, BOOL bDimTablePile)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	CFootingApp* pFoot = pBri->GetFootingByJijumNumber(nJijum);
	if(!pFoot) return;

	// 말뚝 치수 넣을때 1열만 넣을지?
	BOOL bDimOnlyOneRowPile	= TRUE;
	long nDimDanBottom	= 0;	// 하면 치수 기입 단 위치.

	double se = nJijum==0 ? -1.0 : 1.0;
	CDPoint xyJijum = pBri->GetXyOnJijum(nJijum);
	CDPoint vAngAzi = pBri->GetAngleAzimuthJijum(nJijum);
	CDPoint vAng = pBri->GetAngleJijum(nJijum);

	CTwinVector tv, tvFront, tvBack, tvLeft, tvRight;
	CTwinVectorArray tvArr;
	tvArr = pFoot->m_tvArrplan;
	tvArr.Rotate(xyJijum, vAngAzi.GetMirrorVert()*se);

	tvArr.GetTvByInfo(_T("기초평면후면"), tvBack);
	tvArr.GetTvByInfo(_T("기초평면전면"), tvFront);
	tvArr.GetTvByInfo(_T("기초평면좌측1"), tvLeft);
	tvArr.GetTvByInfo(_T("기초평면우측1"), tvRight);

	CString str("");
	CDPoint xy(0, 0);
	int nCountJijum = pBri->GetCountJijum();

	CDomyun Dom(pDomP);
	CDomyun domPile(pDomP);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->m_pARoadOptionStd->SetEnvType(&domPile, HCAD_DIML);

	double dLenU = nJijum==0 ? tvBack.GetLength() : tvFront.GetLength();
	double dLenL = nJijum==0 ? tvFront.GetLength() : tvBack.GetLength();
	double dDFL = Round(pFoot->m_dMarginLeft, 0);
	double dDFR = Round(pFoot->m_dMarginRight, 0);
	double dEF = pFoot->m_exFooting.m_dEFWL;
	double dEFB = pFoot->m_exFooting.m_dEFWB;
	double dEFF = pFoot->m_exFooting.m_dEFWF;
	CDPoint vAngB	= ToDPointFrDegree(pFoot->m_exFooting.GetAngleFBPlane(FALSE));
	CDPoint vAngF	= ToDPointFrDegree(pFoot->m_exFooting.GetAngleFBPlane(TRUE));
	double dEFL = pFoot->m_bRAngLeft ? dEFB : dEFB/fabs(vAngB.y);
	double dEFR = pFoot->m_bRAngRight ? dEFF : dEFF/fabs(vAngF.y);

	CExFootingApp *pExFooting	= &pFoot->m_exFooting;
	if(pExFooting->m_nType == EXFOOTING_TYPE_PILE)
	{
		CExPileApp *pPile	= &pExFooting->m_Pile;
		CDrawExPile drawExPile(pPile, m_pStd->m_pARoadOptionStd);
		CTwinVectorArray tvArrPath;
		pPile->GetTvPlanPath(tvArrPath);
		
		
		long nCountOrg	= bDimOnlyOneRowPile ? 2 : pPile->m_nCountOrg;
		CTwinVector tvPathOrg	= tvArrPath.GetAt(pPile->m_nCountOrg-1);
		long i = 0; for(i = 0; i < nCountOrg; i++)
		{
			long nIdxOrg	= nCountOrg - i - 1;

			// 한열만 치수 기입시 시작과 끝에만 넣는다.
			if(bDimOnlyOneRowPile)
			{
				nIdxOrg	= i == 0 ? pPile->m_nCountOrg - 1 : 0;
			}
			COrgInfo *pOrg	= pPile->m_pArrOrgInfo.GetAt(nIdxOrg);
			CDPointArray xyArrDim;
			CStringArray sArrText;
			CStringArray sArrUnderText;
			long nDimDan	= i;
			CString sDir	= _T("TOP");
			CDPoint xyStt	= CDPoint(0, 0);
			CString sRow	= _T("");

			CTwinVector tvPath	= tvArrPath.GetAt(nIdxOrg);
			
			xyArrDim.Add(tvPath.m_v1);
			sArrUnderText.Add("0");
			for(long j = 0; j < pOrg->countSide; j++)
			{
				CPileAttr *pAttr	= pPile->GetPileAttr(nIdxOrg, j);
				if(pAttr)
				{
					xyArrDim.Add(pAttr->xy);
					sArrUnderText.Add("0");
				}
			}

						
			xyArrDim.Add(tvPath.m_v2);
			sRow.Format("%d열", nIdxOrg+1);
			sArrUnderText.Add(sRow);

			if(nJijum != 0)
			{
				CRebarPlacing rb;
				rb.ReverseRebar(xyArrDim);
			}

			GetXyMatchLineAndLine(tvPathOrg.m_v1, tvPathOrg.GetXyDir(), xyArrDim.GetAt(0), tvPathOrg.GetXyDir().Rotate90(), xyStt);

			// 한열만 기입 할 경우.
			if(bDimOnlyOneRowPile)
			{
				sDir	= i == 0 ? _T("TOP") : _T("BOTTOM");
				if(i == 1)
				{
					nDimDan = nDimDanBottom;
					CTwinVector tvSide	= pFoot->m_tvArrplan.GetTvByInfo(nJijum == 0 ? "기초평면전면" : "기초평면후면");
					GetXyMatchLineAndLine(tvSide.m_v1, tvSide.GetXyDir(), xyArrDim.GetAt(0), tvPathOrg.GetXyDir().Rotate90(), xyStt);
					nDimDanBottom++;
				}
			}

			CHgBaseDrawStd baseDraw;
			baseDraw.AutoDimCont(&domPile, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, TRUE, FALSE, TRUE, TRUE);
			domPile.Rotate(xyJijum, vAngAzi.GetMirrorVert()*se);
			*pDomP << domPile;
		}
	}
	else
	{
		// 상단
		if(dLenU!=dLenL)
		{
			Dom.SetDirection(_T("TOP"));
			xy = nJijum==0 ? tvBack.m_v1 : tvFront.m_v2;
			if(nJijum==0)
			{
				Dom.DimMoveTo(xy.x-dEFR, xy.y, 0, COMMA(dEFR));
				Dom.DimLineTo(dEFR, 0, _T(""));
				Dom.DimLineTo(dDFR, 0, COMMA(dDFR));
				Dom.DimLineTo(dLenU-dDFL-dDFR, 0, COMMA(dLenU-dDFL-dDFR));
				Dom.DimLineTo(dDFL, 0, COMMA(dDFL));
				Dom.DimLineToOver(dEFL, 0, COMMA(dEFL));
			}
			else
			{
				Dom.DimMoveTo(xy.x-dEFL, xy.y, 0, COMMA(dEFL));
				Dom.DimLineTo(dEFL, 0, _T(""));
				Dom.DimLineTo(dDFL, 0, COMMA(dDFL));
				Dom.DimLineTo(dLenU-dDFL-dDFR, 0, COMMA(dLenU-dDFL-dDFR));
				Dom.DimLineTo(dDFR, 0, COMMA(dDFR));
				Dom.DimLineToOver(dEFR, 0, COMMA(dEFR));
			}

			if(dDFL>0 || dDFR>0)
			{
				Dom.DimMoveTo(xy.x, xy.y, 1);
				Dom.DimLineTo(dLenU, 1, COMMA(dLenU));
			}
		}
	}

	// 하단
	Dom.SetDirection(_T("BOTTOM"));
	xy = nJijum==0 ? tvFront.m_v1 : tvBack.m_v2;
	if(nJijum==0)
	{
		Dom.DimMoveTo(xy.x-dEFR, xy.y, nDimDanBottom, COMMA(dEFR));
		Dom.DimLineTo(dEFR, nDimDanBottom, _T(""));
		Dom.DimLineTo(dDFR, nDimDanBottom, COMMA(dDFR));
		Dom.DimLineTo(dLenL-dDFL-dDFR, nDimDanBottom, COMMA(dLenL-dDFL-dDFR));
		Dom.DimLineTo(dDFL, nDimDanBottom, COMMA(dDFL));
		Dom.DimLineToOver(dEFL, nDimDanBottom, COMMA(dEFL));
	}
	else
	{
		Dom.DimMoveTo(xy.x-dEFL, xy.y, nDimDanBottom, COMMA(dEFL));
		Dom.DimLineTo(dEFL, nDimDanBottom, _T(""));
		Dom.DimLineTo(dDFL, nDimDanBottom, COMMA(dDFL));
		Dom.DimLineTo(dLenL-dDFL-dDFR, nDimDanBottom, COMMA(dLenL-dDFL-dDFR));
		Dom.DimLineTo(dDFR, nDimDanBottom, COMMA(dDFR));
		Dom.DimLineToOver(dEFR, nDimDanBottom, COMMA(dEFR));
	}

	if(dDFL>0 || dDFR>0)
	{
		Dom.DimMoveTo(xy.x, xy.y, nDimDanBottom+1);
		Dom.DimLineTo(dLenL, nDimDanBottom+1, COMMA(dLenL));
	}

	////
	double dWitL = Round(tvLeft.GetLength(), 0);
	double dWitR = Round(tvRight.GetLength(), 0);
	double dWit(0), dW(0), dFF(0), dFB(0);
	BOOL bRAng = FALSE;
	CDoubleArray dArrWidth;
	if(nJijum==0)					// 시점
	{
		if(pFoot->m_nType == FOOTING_TYPE_BASIC_RIGHTSLOPE)
		{
			dArrWidth.Add(pFoot->m_dFERW);
			dArrWidth.Add(pFoot->m_dFSRW);
			dArrWidth.Add(pFoot->m_dFWR);
			dArrWidth.Add(pBri->m_dWS);
			dArrWidth.Add(pFoot->m_dFWL);
		}
		else
		{
			dW = pBri->m_dWS;
			dFF = pFoot->m_dFWR;
			dFB = pFoot->m_dFWL;
			dArrWidth.Add(dFF);
			dArrWidth.Add(dW);
			dArrWidth.Add(dFB);
		}
	}
	else if(nJijum==nCountJijum-1)	// 종점
	{
		if(pFoot->m_nType == FOOTING_TYPE_BASIC_LEFTSLOPE)
		{
			dArrWidth.Add(pFoot->m_dFELW);
			dArrWidth.Add(pFoot->m_dFSLW);
			dArrWidth.Add(pFoot->m_dFWL);
			dArrWidth.Add(pBri->m_dWE);
			dArrWidth.Add(pFoot->m_dFWR);
		}
		else
		{
			dW = pBri->m_dWE;
			dFF = pFoot->m_dFWL;
			dFB = pFoot->m_dFWR;
			dArrWidth.Add(dFF);
			dArrWidth.Add(dW);
			dArrWidth.Add(dFB);
		}
	}
	else							// 중간 지점
	{
		CWallApp* pWall = pBri->GetInWall(nJijum-1);
		dW = pWall->m_dW;
		if(pFoot->m_nType == FOOTING_TYPE_LEFTSLOPE)
		{
			dFF	= pFoot->m_dFW/2;
			dFB	= pFoot->m_dFSLW + dFF;

			dFF	-= dW/2;
			dFB	-= dW/2;
		}
		else if(pFoot->m_nType == FOOTING_TYPE_RIGHTSLOPE)
		{
			dFB	= pFoot->m_dFW/2;
			dFF	= pFoot->m_dFSRW + dFB;

			dFB	-= dW/2;
			dFF -= dW/2;
		}
		else
		{
			dFF = pFoot->m_dFWL;
			dFB = pFoot->m_dFWL;
		}
		dArrWidth.Add(dFF);
		dArrWidth.Add(dW);
		dArrWidth.Add(dFB);
	}


	// 좌측
	long nDimDan	= 0;
	Dom.SetDirection(_T("LEFT"));
	tv = nJijum==0 ? tvRight : tvLeft;
	dWit = Round(fabs(tv.m_v1.y-tv.m_v2.y), 0);
	bRAng = nJijum==0 ? dWitR==dWit : dWitL==dWit;
	xy = CDPoint(min(tv.m_v1.x, tv.m_v2.x), tv.m_v2.y);

	// 말뚝인 경우 말뚝 치수 먼저 넣어준다.
	if(pExFooting->m_nType == EXFOOTING_TYPE_PILE)
	{
		CExPileApp *pPile	= &pExFooting->m_Pile;
		CDrawExPile drawExPile(pPile, m_pStd->m_pARoadOptionStd);

		CTwinVector tvOrg	= pPile->GetTvOrg();
		long nCountOrg	= pPile->m_nCountOrg;
		CTwinVectorArray tvArrPath;
		pPile->GetTvPlanPath(tvArrPath);
		if(tvArrPath.GetSize() > 0)
		{
			CTwinVector tvPath	= tvArrPath.GetAt(0);
			
			CDPointArray xyArrDim;
			CStringArray sArrText;
			CStringArray sArrUnderText;
			CString sDir	= _T("LEFT");
			CDPoint xyStt	= CDPoint(0, 0);

			if(nJijum == 0)
				GetSwap(tvOrg.m_v1, tvOrg.m_v2);

			xyArrDim.Add(tvOrg.m_v1);
 
			CDPoint xyMatch(0, 0);
			long i = 0; for(i = 0; i < nCountOrg; i++)
			{
				tvPath	= tvArrPath.GetAt(i);
				GetXyMatchLineAndLine(tvOrg.m_v1, tvPath.GetXyDir().Rotate90(), tvPath.m_v1, tvPath.GetXyDir(), xyMatch);
				xyArrDim.Add(xyMatch);
			}

			GetXyMatchLineAndLine(tvOrg.m_v1, tvPath.GetXyDir().Rotate90(), tvOrg.m_v2, tvPath.GetXyDir(), xyMatch);
			xyArrDim.Add(xyMatch);

			CDPoint xyDirDim	= (xyArrDim.GetAt(xyArrDim.GetSize()-1)-xyArrDim.GetAt(0)).Unit();
			xyArrDim.InsertAt(0, xyArrDim.GetAt(0)+(xyDirDim*-1*dEF));
			xyArrDim.Add(xyArrDim.GetAt(xyArrDim.GetSize()-1) + (xyDirDim*dEF));

			xyStt	= xyArrDim.GetAt(0);
			CHgBaseDrawStd baseDraw;
			baseDraw.AutoDimCont(&domPile, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, (dWitL==dWitR), TRUE, FALSE, TRUE, TRUE);
			domPile.Move(xy-tvOrg.m_v1);
			domPile.Rotate(xy, vAngAzi.GetMirrorVert()*se);
			*pDomP << domPile;
			nDimDan++;
		}
	}

	if(dWitL!=dWitR)
	{
		str = bRAng ? _T("") : _T("(")+COMMA(dEF/vAng.y)+_T(")");
		Dom.DimMoveToExt(xy.x, xy.y-dEF, nDimDan, COMMA(dEF), str);
		Dom.DimLineTo(dEF, nDimDan, "");
		//Dom.DimLineToExtend(dEF, 0, _T(""), str);

		str = bRAng ? _T("") : _T("(")+COMMA(dFF/vAng.y)+_T(")");
		Dom.DimLineToExtend(dFF, nDimDan, COMMA(dFF), str);
		str = bRAng ? _T("") : _T("(")+COMMA(dW/vAng.y)+_T(")");
		Dom.DimLineToExtend(dW, nDimDan, COMMA(dW), str);
		str = bRAng ? _T("") : _T("(")+COMMA(dFB/vAng.y)+_T(")");
		Dom.DimLineToExtend(dFB, nDimDan, COMMA(dFB), str);


		str = bRAng ? _T("") : _T("(")+COMMA(dEF/vAng.y)+_T(")");
		Dom.DimLineToOverExtend(dEF, nDimDan, COMMA(dEF), str);

		Dom.DimMoveTo(xy.x, xy.y, nDimDan+1);
		str = bRAng ? _T("") : _T("(")+COMMA(dWit/vAng.y)+_T(")");
		Dom.DimLineToExtend(dWit, nDimDan+1, COMMA(dWit), str);
	}

	// 우측
	Dom.SetDirection(_T("RIGHT"));
	tv = nJijum==0 ? tvLeft : tvRight;
	dWit = Round(fabs(tv.m_v1.y-tv.m_v2.y), 0);
	bRAng = nJijum==0 ? dWitL==dWit : dWitR==dWit;
	xy = CDPoint(max(tv.m_v1.x, tv.m_v2.x), tv.m_v1.y);

	
	str = bRAng ? _T("") : _T("(")+COMMA(dEF/vAng.y)+_T(")");
	Dom.DimMoveToExt(xy.x, xy.y-dEF, 0, COMMA(dEF), str);
	Dom.DimLineTo(dEF, 0, "");
	//Dom.DimLineToExtend(dEF, 0, _T(""), str);
	for(long w = 0; w < dArrWidth.GetSize(); w++)
	{
		double dW	= dArrWidth.GetAt(w);

		str = bRAng ? _T("") : _T("(")+COMMA(dW/vAng.y)+_T(")");
		Dom.DimLineToExtend(dW, 0, COMMA(dW), str);
	}

	str = bRAng ? _T("") : _T("(")+COMMA(dEF/vAng.y)+_T(")");
	Dom.DimLineToOverExtend(dEF, 0, COMMA(dEF), str);

	Dom.DimMoveTo(xy.x, xy.y, 1);
	str = bRAng ? _T("") : _T("(")+COMMA(dWit/vAng.y)+_T(")");
	Dom.DimLineToExtend(dWit, 1, COMMA(dWit), str);


	// 말뚝 제원 표현
	// 마지막열 두번째꺼에 표현
	if(pExFooting->m_nType == EXFOOTING_TYPE_PILE)
	{
		CExPileApp *pPile	= &pExFooting->m_Pile;
		CPileAttr *pAttr	= pPile->GetPileAttr(pPile->m_nCountOrg-1, 1);
		if(!pAttr)	// 두번째가 없다면 부득이 하게 첫번째꺼에 표기
			pAttr	= pPile->GetPileAttr(pPile->m_nCountOrg-1, 0);

		if(pAttr)
		{
			CDPoint xyMark	= pAttr->xy;
			CRebarPlacing rb;
			rb.RotateXy(xyMark, xyJijum, (vAngAzi.GetMirrorVert()*se).GetAngleDegree());
			double dDia		= pPile->m_dDia;
			double dLength	= pPile->m_dLength;
			double dDT		= pPile->m_dT1;
			CString str1 = pPile->GetStringPileType();
			if(pPile->m_nType == PILE_TYPE_EHP)
				str1 += pPile->GetStringEHPType(pPile->m_nKind_EHP);
			CString str2;	
			str2.Format("%%%%c%.1fx%.fT, L=%.1fM", dDia, dDT, toM(dLength));
			if(dDT==0)
				str2.Format("%%%%c%.1f, L=%.1fM", dDia, toM(dLength));

			
			double Dis1 = Dom.Always(Dom.GetDimVLen()+Dom.GetDimLevelDis());
			double Dis2 = max(Dom.GetTextWidth(str1), Dom.GetTextWidth(str2));
// 			double dHYunDan	= 650;

			CTwinVector tvOrg	= pPile->GetTvOrg();
			double dHYunDan	= tvOrg.GetLength();
			long i = 0; for(i = 0; i < pPile->m_nCountOrg; i++)
			{
				COrgInfo *pOrg	= pPile->m_pArrOrgInfo.GetAt(i);
				dHYunDan -= pOrg->dist;
			}
			

			domPile.GiSiArrowExtend(xyMark.x, xyMark.y, Dis1+dHYunDan, nJijum==0?Dis2:-Dis2, 4, str1, str2);
			domPile.Rotate(xyMark, CDPoint(0, -1));
		}
	}

	*pDomP << Dom;
	*pDomP << domPile;

	if(pExFooting->m_nType == EXFOOTING_TYPE_PILE && bDimTablePile)
	{
		CDRect rectPlane	= tvArr.GetRect();
		DimTablePilePlane(&Dom, pExFooting, CDPoint(rectPlane.left, rectPlane.bottom-Dom.Always(50)));
	}

	*pDomP << Dom;
}

//하부슬래브 평면도 치수표기
void CDrawExFooting::DimFootPlaneBoxType(CDomyun* pDomP)
{
	CRcBridgeApp	*pBri	= m_pStd->m_pRcBridge;
	CExPileApp		*pPile	= &pBri->m_footingBox.m_exFooting.m_Pile;

	CLineInfoApp* pLineApp = static_cast<CLineInfoApp*>(pBri->GetLineBase());
	CDimDomyun Dom(pDomP, pLineApp);
	
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	double dWS = pBri->GetThickOutWallByLine(TRUE);
	double dWE = pBri->GetThickOutWallByLine(FALSE);
	CDPoint vAngS = pBri->GetAngleJijum(0);
	CDPoint vAngE = pBri->GetAngleJijum(pBri->m_nQtyJigan);
	double dStaStt	= pBri->m_dStationBridgeStt - dWS;
	double dLenBridge = pBri->GetLengthBridge(FALSE);
	double dLenAll =  dWS + dLenBridge + dWE;
	
	double dDistSttLeft = pBri->GetDisSlabLeftActByGagak(pBri->m_dStationBridgeStt, vAngS);
	double dDistSttRight = pBri->GetDisSlabRighActByGagak(pBri->m_dStationBridgeStt, vAngS);
	double dDistEndLeft = pBri->GetDisSlabLeftActByGagak(pBri->m_dStationBridgeStt + dLenBridge, vAngE);
	double dDistEndRight = pBri->GetDisSlabRighActByGagak(pBri->m_dStationBridgeStt + dLenBridge, vAngE);

	double dDistLeft		= MIN(dDistSttLeft, dDistEndLeft);
	
	long nDimDan = 1;
	double dLevel   = pDomP->GetDimLevelDis();
	double dExo	    = pDomP->GetDimExo();
	double dVLen    = pDomP->GetDimVLen();
	double Dist		= pDomP->Always(dExo) + pDomP->Always(dVLen) + pDomP->Always(dLevel)*nDimDan;
	double dDistDim = dDistLeft - Dist;

	Dom.SetDirection(_T("TOP"));
	Dom.DimBaseCurvePoint(dStaStt, dDistDim, nDimDan);
	
	Dom.DimCurveMoveTo(dStaStt, dDistDim, nDimDan, FALSE);
	Dom.DimCurveLineTo(dWS, dDistDim, nDimDan, COMMA(dWS), _T(""));
	Dom.DimCurveLineTo(dLenBridge, dDistDim, nDimDan, COMMA(dLenBridge), _T(""));
	Dom.DimCurveLineTo(dWE, dDistDim, nDimDan, COMMA(dWE), _T(""));

	nDimDan--;
	Dist = pDomP->Always(dExo) + pDomP->Always(dVLen) + pDomP->Always(dLevel)*nDimDan;
	dDistDim = dDistLeft - Dist;
	Dom.DimBaseCurvePoint(dStaStt, dDistDim, nDimDan);
	Dom.DimCurveMoveTo(dStaStt, dDistDim, nDimDan, FALSE);
	double sumPileDist = 0;
	long nPileCount = 1;
	double prevDist = 0;
	CString str;
	for(long i=0; i<pPile->m_nCountOrg; i++)
	{
		COrgInfo *pOrg = pPile->m_pArrOrgInfo.GetAt(i);
		if(pOrg)
		{
			if(i == 0)
				Dom.DimCurveLineTo(pOrg->dist, dDistDim, nDimDan, COMMA(pOrg->dist), _T(""));
			else if(i == pPile->m_nCountOrg - 1)
			{
				if(prevDist == pOrg->dist)
				{
					nPileCount++;
					str.Format("%s@%d = %s", COMMA(prevDist), nPileCount, COMMA(prevDist*nPileCount));
					Dom.DimCurveLineTo(prevDist*nPileCount, dDistDim, nDimDan, str, _T(""));
				}
				else
				{
					Dom.DimCurveLineTo(prevDist, dDistDim, nDimDan, COMMA(prevDist), _T(""));
					Dom.DimCurveLineTo(pOrg->dist, dDistDim, nDimDan, COMMA(pOrg->dist), _T(""));
				}
				sumPileDist += pOrg->dist;
				Dom.DimCurveLineTo(dLenAll-sumPileDist, dDistDim, nDimDan, COMMA(dLenAll-sumPileDist), _T(""));
			}
			else
			{
				if(i > 1)	//두번째 말뚝거리부터 @연산 적용
				{
					if(prevDist == pOrg->dist)
						nPileCount++;
					else
					{
						if(nPileCount == 1)
							Dom.DimCurveLineTo(prevDist, dDistDim, nDimDan, COMMA(prevDist), _T(""));
						else
						{
							str.Format("%s@%d = %s", COMMA(prevDist), nPileCount, COMMA(prevDist*nPileCount));
							Dom.DimCurveLineTo(prevDist*nPileCount, dDistDim, nDimDan, str, _T(""));
							nPileCount = 1;
						}
					}
				}
				prevDist = pOrg->dist;
			}
			sumPileDist += pOrg->dist;
		}
	}
	*pDomP << Dom;

	//side 치수
	double  dStaDimStt = MIN(pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, TRUE, TRUE), pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, FALSE, TRUE));
	double  dStaDimEnd = MAX(pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB, TRUE, TRUE), pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB, FALSE, TRUE));

	double sumDistTotalStt = dDistSttRight - dDistSttLeft;
	CDPoint xyBase = pLineApp->GetXyLineDis(dStaDimStt, dDistSttRight);
	COrgInfo *pOrg = pPile->m_pArrOrgInfo.GetAt(0);
	double dDistSum(0);

	Dom.SetDirection(_T("LEFT"));
	Dom.DimMoveTo(xyBase.x, xyBase.y, 1);
	Dom.DimLineTo(sumDistTotalStt, 1, COMMA(sumDistTotalStt));

	Dom.DimMoveTo(xyBase.x, xyBase.y, 0);
	if(pBri->IsGagakForLine(TRUE, FALSE))
	{
		//Dom.DimLineTo(pBri->m_dExpSttH[iRIGHT], 0, COMMA(pBri->m_dExpSttH[iRIGHT]));
		//모따기 옵션때문에
		double dLenGagak = fabs(dDistSttRight - pBri->m_dWidthSlabRight);
		Dom.DimLineTo(dLenGagak, 0, COMMA(dLenGagak));
		dDistSum += dLenGagak;
	}

	for(long ix=0; ix<pOrg->countSide; ++ix)
	{
		CPileAttr *pPileAttr = pPile->GetPileAttr(0, ix);
		
		Dom.DimLineTo(pPileAttr->dist, 0, COMMA(pPileAttr->dist));
		dDistSum += pPileAttr->dist;
	}

	Dom.DimLineTo(sumDistTotalStt-dDistSum, 0, COMMA(sumDistTotalStt-dDistSum));

	// 선형에 직각방향으로 돌려준다.
	Dom.Rotate(xyBase, pLineApp->GetAngleAzimuth(dStaStt));
	*pDomP << Dom;

	long nIndexOrg = pPile->m_pArrOrgInfo.GetSize() - 1;
	if(nIndexOrg > 0)
	{
		double sumDistTotalEnd = dDistEndRight - dDistEndLeft;

		xyBase = pLineApp->GetXyLineDis(dStaDimEnd, dDistEndRight);
		pOrg = pPile->m_pArrOrgInfo.GetAt(nIndexOrg);
		dDistSum = 0;

		Dom.SetDirection(_T("RIGHT"));
		Dom.DimMoveTo(xyBase.x, xyBase.y, 1);
		Dom.DimLineTo(sumDistTotalEnd, 1, COMMA(sumDistTotalEnd));

		Dom.DimMoveTo(xyBase.x, xyBase.y, 0);
		if(pBri->IsGagakForLine(FALSE, FALSE))
		{
			//Dom.DimLineTo(pBri->m_dExpEndH[iRIGHT], 0, COMMA(pBri->m_dExpEndH[iRIGHT]));
			//모따기 옵션때문에
			double dLenGagak = fabs(dDistEndRight - pBri->m_dWidthSlabRight);
			Dom.DimLineTo(dLenGagak, 0, COMMA(dLenGagak));
			dDistSum += dLenGagak;
		}

		for(long ix=0; ix<pOrg->countSide; ++ix)
		{
			CPileAttr *pPileAttr = pPile->GetPileAttr(nIndexOrg, ix);

			Dom.DimLineTo(pPileAttr->dist, 0, COMMA(pPileAttr->dist));
			dDistSum += pPileAttr->dist;
		}

		Dom.DimLineTo(sumDistTotalEnd-dDistSum, 0, COMMA(sumDistTotalEnd-dDistSum));

		// 선형에 직각방향으로 돌려준다.
		Dom.Rotate(xyBase, pLineApp->GetAngleAzimuth(dStaStt + dLenAll));
		*pDomP << Dom;
	}
}

void CDrawExFooting::MarkFootPlaneCenterOfRoad(CDomyun* pDomP, int nJijum)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	CFootingApp* pFoot = pBri->GetFootingByJijumNumber(nJijum);
	if(!pFoot) return;

	double dLenExt = 1000.0;
	double dStaFront = 0, dStaBack = 0;
	CTwinVector tvFront, tvBack;
	CTwinVectorArray tvArr;
	tvArr = pFoot->m_tvArrplan;
	tvArr.GetTvByInfo(_T("기초평면후면"), tvBack);
	tvArr.GetTvByInfo(_T("기초평면전면"), tvFront);

	CLineInfo* pLine = pBri->GetLineBase();
	pLine->GetStationMatchSegment(tvBack.m_v1, tvBack.m_v2, dStaBack);
	pLine->GetStationMatchSegment(tvFront.m_v1, tvFront.m_v2, dStaFront);

	// (ARCBRIDGE-2735) GetStationMatchSegment() 함수를 썼을때 교량이 선형을 벗어나 있다면 Station을 구하지 못한다.
	if(dStaBack == 0)
	{
		pLine->GetStationMatchByXyAndAngle(tvBack.m_v1, tvBack.GetXyDir(), dStaBack, 0);
	}
	if(dStaFront == 0)
	{
		pLine->GetStationMatchByXyAndAngle(tvFront.m_v1, tvFront.GetXyDir(), dStaFront, 0);
	}

	dStaBack -= dLenExt;
	dStaFront += dLenExt;

	CDomyun Dom(pDomP);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
	pLine->DrawLS(&Dom, dStaBack, dStaFront, 0);

	CDPoint xyJijum = pBri->GetXyOnJijum(nJijum);
	CDPoint vAng = pBri->GetAngleAzimuthJijum(nJijum);
	if(nJijum==0) vAng *= -1;
	Dom.Rotate(xyJijum, vAng.GetMirrorVert());

	double dStaCenter = nJijum==0 ? dStaBack : dStaFront;
	CDPoint xyCenter = pLine->GetXyLine(dStaCenter);
	xyCenter = xyCenter.GetXyRotateByBase(xyJijum, vAng.GetMirrorVert());
	m_pStd->MarkCenterOfRoad(&Dom, xyCenter);

	*pDomP << Dom;
}

// 
//BOOL bApplyAnglePile : 말뚝 그릴때 사거리적용할지?
void CDrawExFooting::DrawLeftSide(CDomyun *pDomP, CExFootingApp *pExFooting, long nDrawLeft, BOOL bDrawPile, BOOL bDimTextPile, BOOL bApplyAnglePile, BOOL bLeft)
{
	if(!pExFooting) return;
	if(!pExFooting->m_bIs) return;

	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	CTwinVectorArray tvArr;
	tvArr = pExFooting->m_tvArrSide;

	if(nDrawLeft != 0)
	{
		CRebarPlacing rb;
		CDRect rect	= tvArr.GetRect();
		double dMid	= pExFooting->m_dCenterOfWall;
		BOOL bLeft	= nDrawLeft == -1 ? FALSE : TRUE;
		CDPoint xy1	= CDPoint(dMid, rect.bottom-100);
		CDPoint xy2	= CDPoint(dMid, rect.top+100);
		rb.TrimTvArrayByLine(tvArr, xy1, xy2, bLeft, TRUE, FALSE);
	}
	baseDraw.DrawTvArray(&Dom, tvArr, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());

	if(pExFooting->m_nType == EXFOOTING_TYPE_PILE && bDrawPile)
	{
		// 말뚝은 가장 path와 가장 가까운 곳에 있는 열을 그린다.
		long nIdx	= pExFooting->GetIdxOrgPileWidthPath();
		if(nIdx > -1)
		{
			CDrawExPile drawExPile(&pExFooting->m_Pile, m_pStd->m_pARoadOptionStd);
			drawExPile.DrawSide(&Dom, TRUE, bLeft, nIdx, FALSE, nDrawLeft, bApplyAnglePile);
			if(bDimTextPile)
				drawExPile.DimTextSide(&Dom, TRUE, bLeft, nIdx, bApplyAnglePile, nDrawLeft); 
		}
	}

	*pDomP << Dom;
}

// 말뚝 간격 테이블 처리
// CDPoint xyPos : 좌측 상단 
void CDrawExFooting::DimTablePilePlane(CDomyun *pDomP, CExFootingApp *pExFooting, CDPoint xyPos)
{
	if(!pExFooting) return;
	if(pExFooting->m_nType != EXFOOTING_TYPE_PILE) return;

	CExPileApp *pPile	= &pExFooting->m_Pile;


	CGridDomyunEx Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_TBL1);
	Dom.SetScaleDim(pDomP->GetScaleDim());
	

	long nRowSu = pPile->m_nCountOrg + 1;
	long nColSu	= 0;
	long i = 0; for(i = 0; i < pPile->m_nCountOrg; i++)
	{
		COrgInfo *pOrg	= pPile->m_pArrOrgInfo.GetAt(i);
		if(i == 0)
			nColSu	= pOrg->countSide+1;
		else
			nColSu	= max(pOrg->countSide+1, nColSu);
	}

	/////////////////////////////////////////////
	//테이블 환경 설정
	Dom.SetBasePoint(xyPos);
	Dom.SetColumnCount(nColSu);
	Dom.SetRowCount(nRowSu);
	Dom.SetRowHeightAll(Dom.Always(8));
	Dom.SetColumnWidthAll(Dom.Always(30));

	long nRow = 0;
	long nCol = 0;
	Dom.SetColumnWidth(nCol++, Dom.GetTextWidth("99열")+Dom.Always(10));
	for(i = 1; i < nColSu; i++)
	{
		Dom.SetColumnWidth(nCol++, Dom.Always(40));
	}

	nRow	= 0;
	for(i = 1; i < nColSu; i++)
		Dom.SetTextMatrixCircle(nRow, i, COMMA(i));

	nCol = 0;
	for(i = 1; i < nRowSu; i++)
	{
		nCol	= 0;
		Dom.SetTextMatrix(i, nCol++, COMMA(i)+"열");

		long nIdxOrg	= i-1;
		COrgInfo *pOrg	= pPile->m_pArrOrgInfo.GetAt(nIdxOrg);
		for(long j = 0; j < pOrg->countSide; j++)
		{
			CPileAttr *pAttr	= pPile->GetPileAttr(nIdxOrg, j);
			if(pAttr)
				Dom.SetTextMatrix(i, nCol++, COMMA(pAttr->dist));
		}
	}

	Dom.Draw();
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_TBL2);
	Dom.DrawOutLine();

	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_TXTG);
	Dom.TextOut(CDPoint(rect.left+rect.Width()/2, rect.bottom+Dom.Always(3)), "말뚝 배치 간격 테이블");
	*pDomP << Dom;
}

void CDrawExFooting::DrawLeftSideLowerBox( CDomyun *pDomP, CRcBridgeApp *pBri, double dStation, CDPoint vAng, long nDrawLeft, BOOL bDrawPile/*=TRUE*/, BOOL bDimTextPile/*=TRUE*/, BOOL bApplyAnglePile/*=TRUE*/, BOOL bLeft/*=TRUE*/, BOOL bDrawExFooting/*TRUE*/, BOOL bDrawHunch , BOOL bDrawRubble /* FALSE*/)
{
	if(!pBri) return;
	if(!pBri->IsBoxType()) return;

	CLineInfoApp *pLine = pBri->m_pLineApp;
	if(!pLine) return;

	CExFootingApp *pExFooting = &pBri->m_footingBox.m_exFooting;
	pBri->GetTvBoxLowerExFootSide(dStation, vAng, nDrawLeft);

	CRebarPlacing rp;
	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	// 기초상단 
	long nJijum = pBri->GetJijumNumberByStation(dStation);

	double dDistLeft = pBri->GetDisSlabLeftActByGagak(dStation, vAng);
	double dDistRight = pBri->GetDisSlabRighActByGagak(dStation, vAng);
//	double dEl_Plan = pLine->GetElevationPlanEl(dStation);
//	double dEL_Foot = dEl_Plan - pBri->GetThickPave() - pBri->GetHeightBridgeBox(dStation) + pBri->m_dTS_Lower;
	// 선형에서 바로 구하면 종단 타입 선택시에 위치를 잘못구한다.
	double dEL_Foot = pBri->GetXyLowerSlabBottomByOffsetFrStation(dStation, 0, 0).y + pBri->m_dTS_Lower;
	double dHeightHunch = bDrawHunch? pBri->GetHeightHunchByJijum(nJijum, FALSE) : 0;
	// 헌치가 없는경우에도 경사에의한 차이가 생기지만 그릴 필요가 없다.
	if(pBri->m_bSlopeLowerSlab && bDrawHunch && dHeightHunch > 0)
	{
		double dHeightHunchDiff = pBri->GetHeightLowerSlab(nJijum, 0, CDPoint(0,1)) - pBri->m_dTS_Lower - pBri->GetHeightHunchByJijum(nJijum, FALSE);
		dHeightHunch += dHeightHunchDiff;
	}

	CDPoint vY(0,1);
	CDPoint xy1(dDistLeft, dEL_Foot + dHeightHunch), xy2(dDistRight, dEL_Foot + dHeightHunch);
	if(nDrawLeft == -1)
	{
		xy2.x = (dDistLeft + dDistRight) / 2.;
	}
	if(nDrawLeft == 1)
	{
		xy1.x = (dDistLeft + dDistRight) / 2.;
	}

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	Dom.LineTo(xy1, xy2);

//	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, bColumn ? HCAD_CONC : HCAD_CONH);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONH);
	if(dHeightHunch > 0)
	{
		Dom.LineTo(xy1 - dHeightHunch * vY, xy2 - dHeightHunch * vY);
	}

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	Dom.LineTo(xy1, xy1 - (pBri->m_dTS_Lower + dHeightHunch) * vY); // 기초측면
	Dom.LineTo(xy2, xy2 - (pBri->m_dTS_Lower + dHeightHunch) * vY); // 기초측면
	Dom.LineTo(xy1 - (pBri->m_dTS_Lower + dHeightHunch) * vY, xy2 - (pBri->m_dTS_Lower + dHeightHunch) * vY); // 기초하면

	// 버림콘크리트 구조물
	if(bDrawExFooting)
	{
		CTwinVectorArray tvArr;
		tvArr = pExFooting->m_tvArrSide;
		baseDraw.DrawTvArray(&Dom, tvArr, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());
	}

	// 말뚝, 앵커
	BOOL bEarthAnchor = pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR;
	if((pBri->IsPileBase(nJijum) || bEarthAnchor) && bDrawPile)
	{
		CExPileApp *pPile = &pBri->m_footingBox.m_exFooting.m_Pile;
		double dStationJijum = pBri->GetStationOnJijum(nJijum);

		long nCount = bEarthAnchor ? pBri->m_nCountAnchor : pPile->m_nCountOrg;
		long nIdxOrg = 0;
		if(nJijum == pBri->m_nQtyJigan)
			nIdxOrg = nCount - 1;
		else if(nJijum > 0)
		{
// 			double dStationPile = 0;
			for(long ix = 0; ix < nCount; ix++)
			{
				double dStationPile = pBri->GetStationPileOrAnchor(&pBri->m_footingBox, ix, pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR);
				if(dStationPile >= dStationJijum)
				{
					nIdxOrg = ix;
					break;
				}
			}
		}
		if(nIdxOrg != -1)
		{
			if(bEarthAnchor)
			{
				DrawEarthAnchor(&Dom, nJijum, nIdxOrg, TRUE);
				if(bDimTextPile)
				{
					DimTextEarthAnchor(&Dom, nJijum, nIdxOrg);
				}
			}
			else
			{
				DrawPileBoxType(&Dom, nJijum, nIdxOrg, TRUE);
				if(bDimTextPile)
				{
					DimTextPileBoxType(&Dom, nJijum, nIdxOrg);
				}
			}
		}
	}

	if(bDrawRubble && pBri->m_dThickCobbStone > 0)
	{
		CTwinVectorArray tvArrRubble, tvArr;

		CDPoint xySttPoint;
		BOOL bFind = FALSE;
		for (long ix =0; ix < pExFooting->m_tvArrSide.GetSize(); ix++)
		{
			CTwinVector tvL = pExFooting->m_tvArrSide.GetAt(ix);
			if(tvL.m_sInfo == _T("확대기초우측면"))
			{
				xySttPoint = tvL.GetXyBottom();
				bFind = TRUE;
				break;
			}
		}

		if(bFind)
		{
			for (long ix =0; ix < pExFooting->m_tvArrSide.GetSize(); ix++)
			{
				CTwinVector tv = pExFooting->m_tvArrSide.GetAt(ix);

				BOOL bLenHori = tv.GetHorLength() > 0? TRUE : FALSE;
				BOOL bLenVert = tv.GetVerLength() > 0? TRUE : FALSE;
				if(bLenHori == FALSE && bLenVert ) 
					continue;

				if(ComparePt(tv.m_v1, xySttPoint, _T("="), 0.1))
				{
					tvArrRubble.Add(tv);
					xySttPoint = tv.m_v2;
				}
			}

			if(tvArrRubble.GetSize() > 0)
			{
				CDPoint xyLeft		= tvArrRubble.GetXyLeft();
				CDPoint xyRight		= tvArrRubble.GetXyRight();

				tvArr.AddFromTvArray(tvArrRubble);
				rp.GetOffsetTvArray(tvArr, -pBri->m_dThickCobbStone, FALSE);
				
				if(tvArr.GetXyLeft().y > xyLeft.y)
					rp.GetOffsetTvArray(tvArr, -2*pBri->m_dThickCobbStone, FALSE);

				rp.ReverseRebar(tvArr);

				CTwinVector tvConLeft, tvConRight;
				tvConLeft.m_v1 = xyLeft;
				tvConLeft.m_v2 = tvArr.GetAt(0).m_v1;
				tvConRight.m_v1 = tvArr.GetAt(tvArr.GetSize()-1).m_v2;
				tvConRight.m_v2 = xyRight;

				tvArrRubble.Add(tvConLeft);
				tvArrRubble.AddFromTvArray(tvArr);
				tvArrRubble.Add(tvConRight);
				
				baseDraw.DrawTvArray(&Dom, tvArrRubble, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());

				CHgBaseDrawStd DrawStd2;
				m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_HATH);
				tvArrRubble.Sort();
				DrawStd2.HatchTvArray(&Dom, tvArrRubble, _T("KHAT04"), 1000);
			}			
		}
	}

	*pDomP << Dom;
}

void CDrawExFooting::DrawPileBoxType(CDomyun *pDomP, long nJijum, long nIdxOrg, BOOL bCutting)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(pBri == NULL || pBri->IsBoxType() == FALSE) return;
	if(pBri->m_nTypeLower != BOXFOOTING_TYPE_PILE) return;


	CExPileApp *pPile = &pBri->m_footingBox.m_exFooting.m_Pile;
	if(pPile == NULL) return;
	if(pPile->m_nCountOrg <= 0)  return;
	if(nIdxOrg < 0 || nIdxOrg >= pPile->m_nCountOrg) return;

	// 말뚝의 직경이 0보다 작거나 길이가 0보다 작으면 그리지 않고 리턴한다
	if(pPile->m_dDia <= 0 || pPile->m_dLength <= 0) return;

	CDomyun Dom(pDomP);
	CConcSymbol sym(m_pStd->m_pARoadOptionStd);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SYMB);

	double dDia = pPile->m_dDia;
	double dAng = 0;
	CDPoint vAng(0,1);

 	double dStationJijum = pBri->GetStationOnJijum(nJijum);
	// double dDistPile = nJijum == 0 ? pBri->GetDisSlabRightAct(dStationJijum, vAng) : pBri->GetDisSlabLeftAct(dStationJijum, vAng);
	// double dDistPile = nJijum == 0 ? pBri->GetDisSlabRighActByGagak(dStationJijum, vAng) : pBri->GetDisSlabLeftActByGagak(dStationJijum, vAng);
	// 말뚝 거리 기준위치는 지점에 관계없이 오른쪽부터다. 
	// (ARCBRIDGE-3456) 말뚝배치는 가각에 적용되지 않고 교량폭에대해서만 배치 되므로 시작위치는 가각이 적용안된 오른쪽 슬래브 위치가 맞다.
	// double dDistPile = pBri->GetDisSlabRighActByGagak(dStationJijum, vAng);
	double dDistPile = pBri->GetDisSlabRightAct(dStationJijum, vAng);
	double dElevation = pBri->GetElevationBoxBottom(nJijum);
	double dDrawLen = bCutting ? 3000 : pPile->m_dLength;

	COrgInfo *pOrg = pPile->m_pArrOrgInfo.GetAt(nIdxOrg);
	for(long ix = 0; ix < pOrg->countSide; ix++)
	{
		CPileAttr *pPileAttr = pPile->GetPileAttr(nIdxOrg, ix);

		// dDistPile += nJijum == 0 ? -pPileAttr->dist : pPileAttr->dist;
		dDistPile -= pPileAttr->dist;
 		sym.DrawSymbolSteelPile(&Dom, CDPoint(dDistPile, dElevation + 100), dDia, dDrawLen, bCutting, dAng);
	}

	*pDomP << Dom;
}

void CDrawExFooting::DimTextPileBoxType(CDomyun *pDomP, long nJijum, long nIdxOrg)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(pBri == NULL || pBri->IsBoxType() == FALSE) return;
	if(pBri->m_nTypeLower != BOXFOOTING_TYPE_PILE) return;


	CExPileApp *pPile = &pBri->m_footingBox.m_exFooting.m_Pile;
	if(pPile == NULL) return;
	if(pPile->m_nCountOrg <= 0)  return;
	if(nIdxOrg < 0 || nIdxOrg >= pPile->m_nCountOrg) return;

	// 말뚝의 직경이 0보다 작거나 길이가 0보다 작으면 그리지 않고 리턴한다
	if(pPile->m_dDia <= 0 || pPile->m_dLength <= 0) return;

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
	double dStationJijum = pBri->GetStationOnJijum(nJijum);
	// double dDistPile = nJijum == 0 ? pBri->GetDisSlabRightAct(dStationJijum, vAng) : pBri->GetDisSlabLeftAct(dStationJijum, vAng);
	// double dDistPile = nJijum == 0 ? pBri->GetDisSlabRighActByGagak(dStationJijum, vAng) : pBri->GetDisSlabLeftActByGagak(dStationJijum, vAng);
	// 말뚝 거리 기준위치는 지점에 관계없이 오른쪽부터다. 
	// (ARCBRIDGE-3456) 말뚝배치는 가각에 적용되지 않고 교량폭에대해서만 배치 되므로 시작위치는 가각이 적용안된 오른쪽 슬래브 위치가 맞다.
	// double dDistPile = pBri->GetDisSlabRighActByGagak(dStationJijum, vAng);
	double dDistPile = pBri->GetDisSlabRightAct(dStationJijum, vAng);
	double dElevation = pBri->GetElevationBoxBottom(nJijum);
	double dDrawLen = 3000;

	COrgInfo *pOrg = pPile->m_pArrOrgInfo.GetAt(nIdxOrg);
	CDPoint xyPileStt(0,0);
	for(long ix = 0; ix < pOrg->countSide; ix++)
	{
		CPileAttr *pPileAttr = pPile->GetPileAttr(nIdxOrg, ix);

		// dDistPile += nJijum == 0 ? -pPileAttr->dist : pPileAttr->dist;
		dDistPile -= pPileAttr->dist;

		Dom.Circle(CDPoint(dDistPile, dElevation - dDrawLen + 1000), 50, TRUE);

		// 맨마지막 파일 위치..
		// if(nJijum == 0 || xyPileStt.IsNull())
		{
			xyPileStt = CDPoint(dDistPile, dElevation - dDrawLen + 1000);
		}

	}

	CDPoint xyText = CDPoint(pBri->GetDisSlabRightAct(dStationJijum, vAng) + dLenStr, xyPileStt.y);
	Dom.LineTo(xyPileStt, xyText);

	Dom.TextOut(xyText.x-dLenStr/2, xyText.y+Dom.Always(1), str1);
	Dom.TextOut(xyText.x-dLenStr/2, xyText.y-Dom.GetTextHeight()-Dom.Always(1),str2);
	Dom.TextOut(xyText.x-dLenStr/2, xyText.y-Dom.GetTextHeight()*2-Dom.Always(2),str3);


	*pDomP << Dom;
}

void CDrawExFooting::DrawEarthAnchor(CDomyun *pDomP, long nJijum, long nIdxOrg, BOOL bCutting)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(pBri == NULL || pBri->IsBoxType() == FALSE) return;
	if(pBri->m_nTypeLower != BOXFOOTING_TYPE_ANCHOR) return;

	if(nIdxOrg < 0 || nIdxOrg >= pBri->m_nCountAnchor) return;

	// 말뚝의 직경이 0보다 작거나 길이가 0보다 작으면 그리지 않고 리턴한다
	double dDia = pBri->m_dArrDiaAnchor.GetAt(nIdxOrg);
	double dLen = pBri->m_dArrLengthAnchor.GetAt(nIdxOrg);
	if(dDia <= 0 || dLen <= 0) return;

	CDomyun Dom(pDomP);
	CConcSymbol sym(m_pStd->m_pARoadOptionStd);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SYMB);

	double dAng = 0;
	CDPoint vAng(0,1);

	double dStationJijum = pBri->GetStationOnJijum(nJijum);
	double dDistAnchor = nJijum == 0 ? pBri->GetDisSlabRightAct(dStationJijum, vAng) : pBri->GetDisSlabLeftAct(dStationJijum, vAng);
	double dElevation = pBri->GetElevationBoxBottom(nJijum);
	double dDrawLen = bCutting ? 3000 : dLen;

	long nCount = (long)pBri->m_dArrEaAnchor.GetAt(nIdxOrg);
	for(long ix = 0; ix < nCount; ix++)
	{
		dDistAnchor += nJijum == 0 ? -pBri->m_dArrDistHDanAnchor[nIdxOrg].GetAt(ix) : pBri->m_dArrDistHDanAnchor[nIdxOrg].GetAt(ix);
		sym.DrawSymbolSteelPile(&Dom, CDPoint(dDistAnchor, dElevation + 100), dDia, dDrawLen, bCutting, dAng);
	}

	*pDomP << Dom;
}

void CDrawExFooting::DimTextEarthAnchor(CDomyun *pDomP, long nIdxOrg, long nJijum)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(pBri == NULL || pBri->IsBoxType() == FALSE) return;
	if(pBri->m_nTypeLower != BOXFOOTING_TYPE_ANCHOR) return;

	if(nIdxOrg < 0 || nIdxOrg >= pBri->m_nCountAnchor) return;

	// 말뚝의 직경이 0보다 작거나 길이가 0보다 작으면 그리지 않고 리턴한다
	double dDia = pBri->m_dArrDiaAnchor.GetAt(nIdxOrg);
	double dLen = pBri->m_dArrLengthAnchor.GetAt(nIdxOrg);
	if(dDia <= 0 || dLen <= 0) return;

	CDomyun Dom(pDomP);
	CConcSymbol sym(m_pStd->m_pARoadOptionStd);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CString str1, str2;
	str1 = _T("Earth Anchor");
	str2.Format(_T("%%%%c%.1f, L = %.3f m"), dDia, toM(dLen));
	for(long ix=1; ix<pBri->m_nCountAnchor; ix++)
	{
		if(Compare(pBri->m_dArrLengthAnchor.GetAt(ix), pBri->m_dArrLengthAnchor.GetAt(0), _T("=")) == FALSE)
		{
			str2.Format(_T("%%%%c%.1f, L = VAR"), dDia);
		}
	}

	double dLenStr = MAX(Dom.GetTextWidth(str1, 3), Dom.GetTextWidth(str2, 3));

	CDPoint vAng(0,1);
	double dStationJijum = pBri->GetStationOnJijum(nJijum);
	double dDistAnchor = nJijum == 0 ? pBri->GetDisSlabRightAct(dStationJijum, vAng) : pBri->GetDisSlabLeftAct(dStationJijum, vAng);
	double dElevation = pBri->GetElevationBoxBottom(nJijum);
	double dDrawLen = 3000;

	CDPoint xyAnchorStt(0,0);
	long nCount = (long)pBri->m_dArrEaAnchor.GetAt(nIdxOrg);
	for(long ix = 0; ix < nCount; ix++)
	{
		dDistAnchor += nJijum == 0 ? -pBri->m_dArrDistHDanAnchor[nIdxOrg].GetAt(ix) : pBri->m_dArrDistHDanAnchor[nIdxOrg].GetAt(ix);

		Dom.Circle(CDPoint(dDistAnchor, dElevation - dDrawLen + 1000), 50, TRUE);

		if(nJijum == 0 || xyAnchorStt.IsNull())
		{
			xyAnchorStt = CDPoint(dDistAnchor, dElevation - dDrawLen + 1000);
		}

	}

	CDPoint xyText = CDPoint(pBri->GetDisSlabRightAct(dStationJijum, vAng) + dLenStr, xyAnchorStt.y);
	Dom.LineTo(xyAnchorStt, xyText);

	Dom.TextOut(xyText.x-dLenStr/2, xyText.y+Dom.Always(1), str1);
	Dom.TextOut(xyText.x-dLenStr/2, xyText.y-Dom.GetTextHeight()-Dom.Always(1),str2);

	*pDomP << Dom;
}

