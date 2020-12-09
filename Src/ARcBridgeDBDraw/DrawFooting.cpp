// DrawFooting.cpp: implementation of the CDrawFooting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARcBridgeDBDraw.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawFooting::CDrawFooting()
{

}

CDrawFooting::~CDrawFooting()
{

}

void CDrawFooting::DrawFront(CDomyun *pDomP, CFootingApp *pFooting, CTwinVectorArray *tvArrReturn, BOOL bDrawConstJoint)
{
	if(!pFooting) return;
	if(!pFooting->m_bIs) return;
	long nType	= pFooting->GetTypeByInputValue();

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	m_pStd->DrawTvArray(&Dom, pFooting->m_tvArrFront, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo(), 0);
	tvArrReturn->ChangePos(pFooting->m_tvArrFront);

	if(bDrawConstJoint)	// �ð� ����(�ð� ������������ �ʿ�)
	{
		CTwinVector tvLeft, tvRight, tvHeightL, tvHeightR;
		if(pFooting->m_bIs)
		{
			CTwinVector tvConstJoint;
			if(nType == FOOTING_TYPE_BASIC || nType == FOOTING_TYPE_BASIC_BOTH ||
				nType == FOOTING_TYPE_BASIC_RIGHTSLOPE || nType == FOOTING_TYPE_BASIC_LEFTSLOPE)
			{
				pFooting->m_tvArrFront.GetTvByInfo(_T("���ʻ����������"), tvLeft);
				pFooting->m_tvArrFront.GetTvByInfo(_T("���ʻ���������"), tvRight);
				tvConstJoint.m_v1	= tvLeft.GetXyLeft();
				tvConstJoint.m_v2	= tvRight.GetXyRight();
			}
			else
			{
				pFooting->m_tvArrFront.GetTvByInfo(_T("���ʰ��������"), tvLeft);
				pFooting->m_tvArrFront.GetTvByInfo(_T("���ʰ�������"), tvRight);
				tvConstJoint.m_v1	= tvLeft.GetXyLeft();
				tvConstJoint.m_v2	= tvRight.GetXyRight();
			}
			pFooting->m_tvArrFront.GetTvByInfo(_T("����������"), tvHeightL);
			pFooting->m_tvArrFront.GetTvByInfo(_T("���ʿ�����"), tvHeightR);

			if(tvConstJoint.GetLength() > 0)
			{
				double dW = tvConstJoint.GetLength();
				m_pStd->DrawConstJointHor(&Dom, tvConstJoint.m_v1, dW, tvConstJoint.GetXyDir().GetAngleDegree());

				Dom.SetTextHeight(2);
				double dFH = max(tvHeightL.GetLength(), tvHeightR.GetLength());
				CDPoint xyNum = tvConstJoint.GetXyMid() - CDPoint(0, dFH/2);
				Dom.TextCircle(xyNum.x, xyNum.y, _T("1"), 3, FALSE, TRUE);
			}
		}
	}

	*pDomP << Dom;
}

// bLeftIsFront : ������ �б��� ��������?
// �ٸ��� ���̰� ���� ��ȣ���� ���̸� �ִ�.(�ձ��� FF, �ޱ��� FB) - FOOTING_TYPE_BASIC�϶��� ����
void CDrawFooting::DimFrontUpper(CDomyun *pDomP, CFootingApp *pFooting, long nDimDan, BOOL bInput, BOOL bLeftIsFront)
{
	CRcBridgeRebar *pBri	= m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	long nJ	= 0;
	long i = 0; for(i = 0; i < pBri->GetCountJijum(); i++)
	{
		if(pFooting == pBri->GetFootingByJijumNumber(i))
		{
			nJ	= i;
			break;
		}
	}

	// ��ü �ϸ� ���Ÿ�, ��Ÿ�
	// ���ʴ� ��Ÿ��� �׸���, ��ü�� �����Ÿ��� �׸� �� �ۿ� ����.
	// �׷��� ��ü�� ����̸� ��ü�ϸ�� ������ ���̰� �� ���� �� �ִµ�
	// �̰� ���� �Ұ�
	// ��, ��ü �ϸ��� ġ���� ���������� �׷�����

	if(pFooting == NULL) return;

	BOOL bApplyAngle	= pFooting->IsApplyAngle();	

	double dWLowerVert	= 0;
	double dWLowerAng	= 0;
	if(nJ == 0)
	{
		CTwinVector tv	= pBri->m_tvArrVertSection.GetTvByInfo("������ü�ϸ�");
		dWLowerVert	= tv.GetHorLength();
		dWLowerAng	= tv.GetHorLength(bApplyAngle);
	}
	else if(nJ == pBri->m_nQtyJigan)
	{
		CTwinVector tv	= pBri->m_tvArrVertSection.GetTvByInfo("������ü�ϸ�");
		dWLowerVert	= tv.GetHorLength();
		dWLowerAng	= tv.GetHorLength(bApplyAngle);
	}
	else
	{
		long nIdxInWall	= nJ-1;
		CWallApp *pWall	= pBri->GetInWall(nIdxInWall);
		if(pWall)
		{
			CTwinVector tvLowerL	= pWall->m_tvArrFront.GetTvByInfo("��ü�ϸ�����");
			CTwinVector tvLowerR	= pWall->m_tvArrFront.GetTvByInfo("��ü�ϸ����");
			dWLowerVert	= tvLowerL.GetHorLength() + tvLowerR.GetHorLength();
			dWLowerAng	= tvLowerL.GetHorLength(bApplyAngle) + tvLowerR.GetHorLength(bApplyAngle);
		}
	}

	if(pFooting->IsVirtualFooting()) return;
	if(nDimDan < 0) nDimDan = 0;
	long nType	= pFooting->GetTypeByInputValue();

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDomP, &Dom);

	CHgBaseDrawStd baseDraw;
	
	CDoubleArray dArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("TOP");
	CDPoint xyStt(0, 0);

	

	CTwinVector tv;
	CTwinVectorArray *pTvArr = &pFooting->m_tvArrFront;
	if(pTvArr->GetSize() == 0) return;
//	double dWidth = 0;
//	double dWidth2	= 0;

	if(!bInput && pBri->IsIncludeOnlyLineInBridge())	// ��� �ƴѰ�쿡�� ��밡����
	{
		// �Է¿��� �ƴϸ� ���ʻ���� ���� ��ǥ��� �׸���.
		CTwinVectorArray tvArr;
		tvArr	= *pTvArr;
		CRebarPlacing rb;
		rb.RemoveMeanlessTwinVector(tvArr);
		
		CDPointArray xyArr;
		long i = 0; for(i = 0; i < tvArr.GetSize(); i++)
		{
			if(i == 0)
				xyArr.Add(tvArr.GetAt(i).m_v1);
			xyArr.Add(tvArr.GetAt(i).m_v2);
		}

		// overlap�Ǵ� �κ� ����
		CTwinVector *pTv	= NULL;
		for(i = 0; i < pTvArr->GetSize(); i++)
		{
			pTv	= pTvArr->GetPAt(i);
			if(ComparePt(pTv->m_vecOverlap1, pTv->m_vecOverlap2, "=")) continue;

			double dXLeft	= pTv->m_vecOverlap1.x;
			double dXRight	= pTv->m_vecOverlap2.x;
			if(dXLeft > dXRight)
				GetSwap(dXLeft, dXRight);

			for(long j = 0; j < xyArr.GetSize(); j++)
			{
				if(Compare(xyArr[j].x, dXLeft, ">=") && Compare(xyArr[j].x, dXRight, "<="))
					xyArr.RemoveAt(j--);
			}

			xyArr.Add(pTv->m_vecOverlap1);
			xyArr.Add(pTv->m_vecOverlap2);
		}
		
		xyArr.Sort(TRUE);

		if(xyArr.GetSize() > 0)
		{
			xyStt.x	= xyArr[0].x;
			xyStt.y	= tvArr.GetRect().top;
			
			double dRateAng	= bApplyAngle ? dWLowerVert / dWLowerAng : dWLowerAng / dWLowerVert;
			for(i = 1; i < xyArr.GetSize(); i++)
			{
				double dLen	= xyArr[i].x - xyArr[i-1].x;
				dArrDim.Add(dLen);
			}

			for(i = 0; i < dArrDim.GetSize(); i++)
			{
				if(Compare(dArrDim[i], 0.0, "="))
					dArrDim.RemoveAt(i--);
			}

			for(i = 0; i < dArrDim.GetSize(); i++)
			{
				sArrText.Add(COMMA(dArrDim[i]));
				sArrUnderText.Add(COMMA2(dArrDim[i]*dRateAng));
			}
		}
	}
	else
	{
		if(nType == FOOTING_TYPE_BASIC || nType == FOOTING_TYPE_BASIC_BOTH)
		{
			pTvArr->GetTvByInfo("���ʻ������", tv);
			dArrDim.Add(tv.GetHorLength(bApplyAngle));
			if(bInput)
			{
				if(nType == FOOTING_TYPE_BASIC)
				{
					if(bLeftIsFront)
					{
						sArrText.Add("FF");
					}
					else
					{
						sArrText.Add("FB");
					}
				}
				else
				{
					sArrText.Add("FL");
				}
				sArrUnderText.Add(COMMA3(tv.GetHorLength(TRUE), tv.GetHorLength(FALSE)));
			}
			else
			{
// 				if(nType == FOOTING_TYPE_BASIC)
// 				{
// 					if(bLeftIsFront)
// 					{
// 						sArrText.Add(COMMA(tv.GetHorLength(bApplyAngle)));
// 					}
// 					else
// 					{
// 						sArrText.Add(COMMA(tv.GetHorLength(bApplyAngle)));
// 					}
// 				}
// 				else
// 				{
					sArrText.Add(COMMA(tv.GetHorLength(bApplyAngle)));
// 				}
				sArrUnderText.Add(COMMA2(tv.GetHorLength(FALSE)));
			}

			pTvArr->GetTvByInfo("���ʻ����������", tv);
			double dWidth = tv.GetHorLength(bApplyAngle);
			double dWidth2	= tv.GetHorLength(FALSE);
			pTvArr->GetTvByInfo("���ʻ���������", tv);
			dWidth += tv.GetHorLength(bApplyAngle);
			dWidth2 += tv.GetHorLength(FALSE);

			if(dArrDim.GetSize() > 0)
			{
				double d	= dArrDim.GetAt(dArrDim.GetSize()-1);
				if(bApplyAngle)
				{
					d -= (dWLowerAng - dWidth);
					dArrDim.SetAt(dArrDim.GetSize()-1, d);
				}
			}


			dWidth	= dWLowerAng;
			dWidth2	= dWLowerVert;

			dArrDim.Add(dWidth);
			if(bInput)
			{
				sArrText.Add("W");
				sArrUnderText.Add(COMMA3(dWidth, dWidth2));
			}
			else
			{
				sArrText.Add(COMMA(dWidth));

				pTvArr->GetTvByInfo("���ʻ����������", tv);
				dWidth = tv.GetHorLength(FALSE);
				pTvArr->GetTvByInfo("���ʻ���������", tv);
				dWidth += tv.GetHorLength(FALSE);
				sArrUnderText.Add(COMMA2(dWidth));
			}

			pTvArr->GetTvByInfo("���ʻ�����", tv);
			dArrDim.Add(tv.GetHorLength(bApplyAngle));
			if(bInput)
			{
				if(nType == FOOTING_TYPE_BASIC) 
				{
					if(bLeftIsFront)
					{
						sArrText.Add("FB");
					}
					else
					{
						sArrText.Add("FF");
					}
				}
				else
				{
					sArrText.Add("FL");
				}
				sArrUnderText.Add(COMMA3(tv.GetHorLength(TRUE), tv.GetHorLength(FALSE)));
			}
			else
			{
// 				if(nType == FOOTING_TYPE_BASIC) 
// 				{
// 					if(bLeftIsFront)
// 					{
// 						sArrText.Add(COMMA(tv.GetHorLength(bApplyAngle)));
// 					}
// 					else
// 					{
// 						sArrText.Add(COMMA(tv.GetHorLength(bApplyAngle)));
// 					}
// 				}
// 				else
// 				{
					sArrText.Add(COMMA(tv.GetHorLength(bApplyAngle)));
// 				}
				sArrUnderText.Add(COMMA2(tv.GetHorLength(FALSE)));
			}

			// ġ������ ������ 
			pTvArr->GetTvByInfo("����������", tv);
			xyStt = tv.m_v2;
		}
		else if(nType == FOOTING_TYPE_LEFTSLOPE || nType == FOOTING_TYPE_RIGHTSLOPE)
		{
			CTwinVector tvLower, tvUpper;
			pTvArr->GetTvByInfo("�����ϸ�", tvLower);
			pTvArr->GetTvByInfo("���ʻ��", tvUpper);

			if(nType == FOOTING_TYPE_LEFTSLOPE)
			{
				double dLen		= tvLower.GetHorLength(bApplyAngle) - tvUpper.GetHorLength(bApplyAngle);
				double dLen2	= tvLower.GetHorLength(FALSE) - tvUpper.GetHorLength(FALSE);

				// ���� �ʺ�
				dArrDim.Add(dLen);
				if(bInput)
				{
					sArrText.Add("FW");
					sArrUnderText.Add(COMMA(dLen2));
				}
				else
				{
					sArrText.Add(COMMA(dLen));
					sArrUnderText.Add(COMMA2(dLen2));
				}

				// ����� �ʺ�
				dLen	= tvUpper.GetHorLength(bApplyAngle);
				dLen2	= tvUpper.GetHorLength(FALSE);

				dArrDim.Add(dLen);
				if(bInput)
				{
					sArrText.Add("FSW");
					sArrUnderText.Add(COMMA(dLen));
				}
				else
				{
					sArrText.Add(COMMA(dLen));
					sArrUnderText.Add(COMMA2(dLen2));
				}

				xyStt	= CDPoint(tvLower.m_v2.x, tvUpper.m_v1.y);
			}
			else
			{
				// ����� �ʺ�
				double dLen		= tvUpper.GetHorLength(bApplyAngle);
				double dLen2	= tvUpper.GetHorLength(FALSE);

				dArrDim.Add(dLen);
				if(bInput)
				{
					sArrText.Add("FSW");
					sArrUnderText.Add(COMMA(dLen));
				}
				else
				{
					sArrText.Add(COMMA(dLen));
					sArrUnderText.Add(COMMA2(dLen2));
				}

				dLen	= tvLower.GetHorLength(bApplyAngle) - tvUpper.GetHorLength(bApplyAngle);
				dLen2	= tvLower.GetHorLength(FALSE) - tvUpper.GetHorLength(FALSE);

				// ���� �ʺ�
				dArrDim.Add(dLen);
				if(bInput)
				{
					sArrText.Add("FW");
					sArrUnderText.Add(COMMA(dLen2));
				}
				else
				{
					sArrText.Add(COMMA(dLen));
					sArrUnderText.Add(COMMA2(dLen2));
				}

				xyStt	= CDPoint(tvLower.m_v1.x, tvUpper.m_v1.y);
			}
		}
		else if(nType == FOOTING_TYPE_BASIC_RIGHTSLOPE)
		{
			tv				= pTvArr->GetTvByInfo("���ʻ������");
			double dLen		= tv.GetHorLength(bApplyAngle);
			double dLen2	= tv.GetHorLength(FALSE);
			dArrDim.Add(dLen);
			if(bInput)
			{
				sArrText.Add("FB");
				sArrUnderText.Add(COMMA(dLen2));
			}
			else
			{
				sArrText.Add(COMMA(dLen));
				sArrUnderText.Add(COMMA2(dLen2));
			}

			CTwinVector tvLower;
			pTvArr->GetTvByInfo("�����ϸ�", tvLower);
			xyStt	= CDPoint(tvLower.GetXyLeft().x, tv.m_v1.y);

			pTvArr->GetTvByInfo("���ʻ����������", tv);
			dLen	= tv.GetHorLength(bApplyAngle);
			dLen2	= tv.GetHorLength(FALSE);
			pTvArr->GetTvByInfo("���ʻ���������", tv);
			dLen += tv.GetHorLength(bApplyAngle);
			dLen2 += tv.GetHorLength(FALSE);
			dArrDim.Add(dLen);
			if(bInput)
			{
				sArrText.Add("W");
				sArrUnderText.Add(COMMA(dLen2));
			}
			else
			{
				sArrText.Add(COMMA(dLen));
				sArrUnderText.Add(COMMA2(dLen2));
			}

			tv		= pTvArr->GetTvByInfo("���ʻ�����");
			dLen	= tv.GetHorLength(bApplyAngle);
			dLen2	= tv.GetHorLength(FALSE);
			dArrDim.Add(dLen);
			if(bInput)
			{
				sArrText.Add("FF");
				sArrUnderText.Add(COMMA(dLen2));
			}
			else
			{
				sArrText.Add(COMMA(dLen));
				sArrUnderText.Add(COMMA2(dLen2));
			}

			tv		= pTvArr->GetTvByInfo("���ʿ������");
			dLen	= tv.GetHorLength(bApplyAngle);
			dLen2	= tv.GetHorLength(FALSE);
			dArrDim.Add(dLen);
			if(bInput)
			{
				sArrText.Add("FSW");
				sArrUnderText.Add(COMMA(dLen2));
			}
			else
			{
				sArrText.Add(COMMA(dLen));
				sArrUnderText.Add(COMMA2(dLen2));
			}

			tv	= pTvArr->GetTvByInfo("���ʿ���Ȯ����");
			dLen	= tv.GetHorLength(bApplyAngle);
			dLen2	= tv.GetHorLength(FALSE);
			dArrDim.Add(dLen);
			if(bInput)
			{
				sArrText.Add("FEW");
				sArrUnderText.Add(COMMA(dLen2));
			}
			else
			{
				sArrText.Add(COMMA(dLen));
				sArrUnderText.Add(COMMA2(dLen2));
			}
		}
		else if(nType == FOOTING_TYPE_BASIC_LEFTSLOPE)
		{
			tv	= pTvArr->GetTvByInfo("��������Ȯ����");
			double dLen		= tv.GetHorLength(bApplyAngle);
			double dLen2	= tv.GetHorLength(FALSE);
			dArrDim.Add(dLen);
			if(bInput)
			{
				sArrText.Add("FEW");
				sArrUnderText.Add(COMMA(dLen2));
			}
			else
			{
				sArrText.Add(COMMA(dLen));
				sArrUnderText.Add(COMMA2(dLen2));
			}

			tv		= pTvArr->GetTvByInfo("�����������");
			dLen	= tv.GetHorLength(bApplyAngle);
			dLen2	= tv.GetHorLength(FALSE);
			dArrDim.Add(dLen);
			if(bInput)
			{
				sArrText.Add("FSW");
				sArrUnderText.Add(COMMA(dLen2));
			}
			else
			{
				sArrText.Add(COMMA(dLen));
				sArrUnderText.Add(COMMA2(dLen2));
			}

			tv		= pTvArr->GetTvByInfo("���ʻ������");
			dLen	= tv.GetHorLength(bApplyAngle);
			dLen2	= tv.GetHorLength(FALSE);
			dArrDim.Add(dLen);
			if(bInput)
			{
				sArrText.Add("FF");
				sArrUnderText.Add(COMMA(dLen2));
			}
			else
			{
				sArrText.Add(COMMA(dLen));
				sArrUnderText.Add(COMMA2(dLen2));
			}

			pTvArr->GetTvByInfo("���ʻ����������", tv);
			dLen	= tv.GetHorLength(bApplyAngle);
			dLen2	= tv.GetHorLength(FALSE);
			pTvArr->GetTvByInfo("���ʻ���������", tv);
			dLen += tv.GetHorLength(bApplyAngle);
			dLen2 += tv.GetHorLength(FALSE);
			dArrDim.Add(dLen);
			if(bInput)
			{
				sArrText.Add("W");
				sArrUnderText.Add(COMMA(dLen2));
			}
			else
			{
				sArrText.Add(COMMA(dLen));
				sArrUnderText.Add(COMMA2(dLen2));
			}

			tv		= pTvArr->GetTvByInfo("���ʻ�����");
			dLen	= tv.GetHorLength(bApplyAngle);
			dLen2	= tv.GetHorLength(FALSE);
			dArrDim.Add(dLen);
			if(bInput)
			{
				sArrText.Add("FB");
				sArrUnderText.Add(COMMA(dLen2));
			}
			else
			{
				sArrText.Add(COMMA(dLen));
				sArrUnderText.Add(COMMA2(dLen2));
			}

			CTwinVector tvLower;
			pTvArr->GetTvByInfo("�����ϸ�", tvLower);
			xyStt	= CDPoint(tvLower.GetXyLeft().x, tv.m_v1.y);
		}
	}

	// ��� ������ ��� ��ȣ�� ġ���� �ǹ̰� ����. //////////
	m_pStd->ModifyInfoAutoDimConc(sArrText, sArrUnderText, bInput);
	///////////////////////////////////////////////////////////

	if(pBri)
	{
		if(m_pStd->m_bDisplayAngLenStd && !pBri->IsIncludeOnlyLineInBridge() && m_pStd->IsDimByAngleLen())
		{
			if(sArrText.GetSize() < dArrDim.GetSize())
			{
				long i = 0; for(i = 0; i < dArrDim.GetSize() - sArrText.GetSize(); i++)
				{
					sArrText.Add("0");
				}
			}

			CLineInfo *pLine	= pBri->GetLineBase();
			long nJ	= 0;
			long i = 0; for(i = 0; i < pBri->GetCountJijum(); i++)
			{
				if(pFooting == pBri->GetFootingByJijumNumber(i))
				{
					nJ	= i;
					break;
				}
			}
			if(pLine->GetTypeLine(pBri->GetStationOnJijum(nJ)) != 0)
			{
				dArrDim.Add(0.0001);
				sArrText.Add("(�簢���� ��Ÿ�)");
			}
		}
	}

	baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE);


	*pDomP << Dom;
}

// ���� ������
void CDrawFooting::DimFrontSide(CDomyun *pDomP, CFootingApp *pFooting, long nDimDan, BOOL bDimExFooting, BOOL bInput, BOOL bSide)
{
	if(!pFooting) return;
	if(!pFooting->m_bIs) return;
	if(nDimDan < 0) nDimDan = 0;
	long nType	= pFooting->GetTypeByInputValue();

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	
	CHgBaseDrawStd baseDraw;

	CDoubleArray dArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = nType == FOOTING_TYPE_BASIC_RIGHTSLOPE ? _T("RIGHT") : _T("LEFT");
	CDPoint xyStt(0, 0);
	CDPoint xySttTotal(0, 0);

	CTwinVectorArray tvArrSide, tvArrHidden;
	pFooting->GetTvLeftSide(tvArrSide, tvArrHidden, bInput);
	CTwinVector tv;
	CTwinVectorArray *pTvArr = bSide ? &tvArrSide : &pFooting->m_tvArrFront;

	if(bDimExFooting)
	{
		CExFootingApp *pExFooting	= &pFooting->m_exFooting;

		CDRect rect		= pExFooting->m_tvArrFront.GetRect();
		if((pExFooting->m_bLeftOrgOfMass && nType != FOOTING_TYPE_RIGHTSLOPE) ||
			(!pExFooting->m_bLeftOrgOfMass && nType == FOOTING_TYPE_RIGHTSLOPE) || 
			(pExFooting->m_nType != EXFOOTING_TYPE_MASS) ||
			(bSide && pExFooting->m_bFrontDanMass || !bSide && !pExFooting->m_bFrontDanMass))
		{
			double dH		= rect.top - rect.bottom;
			dArrDim.Add(dH);
			sArrText.Add("EH");
			sArrUnderText.Add("");			
		}
		else
		{
			double dH		= pExFooting->GetMaxHeight();
			double dHCur	= 0;
			CString str		= _T("");
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
		xyStt	= nType == FOOTING_TYPE_RIGHTSLOPE || nType == FOOTING_TYPE_BASIC_RIGHTSLOPE
					? CDPoint(rect.right, rect.bottom) : CDPoint(rect.left, rect.bottom);
	}

	if(nType == FOOTING_TYPE_BASIC || nType == FOOTING_TYPE_BASIC_BOTH)
	{
		pTvArr->GetTvByInfo("����������", tv);
		dArrDim.Add(tv.GetVerLength());
		sArrText.Add("FH");
		sArrUnderText.Add(COMMA(tv.GetVerLength()));
		if(!bDimExFooting) xyStt = tv.m_v1;
		xySttTotal	= tv.m_v1;
	}
	else if(nType == FOOTING_TYPE_LEFTSLOPE)
	{
		CDPoint xy(0, 0);
		pTvArr->GetTvByInfo("����������", tv);
		dArrDim.Add(tv.GetVerLength());
		sArrText.Add("FSH");
		sArrUnderText.Add(COMMA(tv.GetVerLength()));
		if(!bDimExFooting) xyStt = tv.m_v1;
		xySttTotal	= tv.m_v1;
		xy = tv.m_v2;

		pTvArr->GetTvByInfo("���ʻ��", tv);
		dArrDim.Add(tv.m_v1.y - xy.y);
		sArrText.Add("FH");
		sArrUnderText.Add(COMMA(tv.m_v1.y - xy.y));
		
		sDir	= "LEFT";
	}
	else if(nType == FOOTING_TYPE_RIGHTSLOPE)
	{
		CDPoint xy(0, 0);
		pTvArr->GetTvByInfo("���ʿ�����", tv);
		dArrDim.Add(tv.GetVerLength());
		sArrText.Add("FSH");
		sArrUnderText.Add(COMMA(tv.GetVerLength()));
		if(!bDimExFooting) xyStt = tv.m_v1;
		xy = tv.m_v2;
		xySttTotal	= tv.m_v1;

		pTvArr->GetTvByInfo("���ʻ��", tv);
		dArrDim.Add(tv.m_v1.y - xy.y);
		sArrText.Add("FH");
		sArrUnderText.Add(COMMA(tv.m_v1.y - xy.y));

		sDir	= "RIGHT";
	}
	else if(nType == FOOTING_TYPE_BASIC_RIGHTSLOPE)
	{
		if(pTvArr->GetTvByInfo("���ʿ�����", tv))
		{
			dArrDim.Add(tv.GetVerLength());
			sArrText.Add("FH");
			sArrUnderText.Add(COMMA(tv.GetVerLength()));
			if(!bDimExFooting) xyStt	= tv.GetXyBottom();
			xySttTotal	= tv.GetXyBottom();
		}

		if(pTvArr->GetTvByInfo("���ʿ������", tv))
		{
			dArrDim.Add(tv.GetVerLength());
			sArrText.Add("FSH");
			sArrUnderText.Add(COMMA(tv.GetVerLength()));
		}
	}
	else if(nType == FOOTING_TYPE_BASIC_LEFTSLOPE)
	{
		if(pTvArr->GetTvByInfo("����������", tv))
		{
			dArrDim.Add(tv.GetVerLength());
			sArrText.Add("FH");
			sArrUnderText.Add(COMMA(tv.GetVerLength()));
			if(!bDimExFooting) xyStt	= tv.GetXyBottom();
			xySttTotal	= tv.GetXyBottom();
		}

		if(pTvArr->GetTvByInfo("�����������", tv))
		{
			dArrDim.Add(tv.GetVerLength());
			sArrText.Add("FSH");
			sArrUnderText.Add(COMMA(tv.GetVerLength()));
		}
	}
	

	if(!bInput)
	{
		sArrText.RemoveAll();
		sArrUnderText.RemoveAll();
	}
	
	baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE);

	// ��ü ġ���� 
	xySttTotal.x	= xyStt.x;
	Dom.SetDirection(sDir);
	Dom.DimMoveTo(xySttTotal.x, xySttTotal.y, nDimDan+1);
	Dom.DimLineTo(pTvArr->GetVerLength(), nDimDan+1, COMMA(pTvArr->GetVerLength()));


	*pDomP << Dom;
}

// ���� ��鵵 �׸���
CDRect CDrawFooting::DrawPlan(CDomyun *pDomP, CFootingApp *pFooting, BOOL bDrawCenterLine, BOOL bObtuse, BOOL bFront, BOOL bHidden)
{
	if(!pFooting) return CDRect(0, 0, 0, 0);
	if(pFooting->IsVirtualFooting()) return CDRect(0, 0, 0, 0);

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, bHidden ? HCAD_CONH : HCAD_CONC);
	
	if(bObtuse)
	{
		double dRotate	= -pFooting->m_tvPathPlan.GetXyDir().GetAngleDegree();
		CRebarPlacing rb;

		CTwinVectorArray tvArrPlane;
		tvArrPlane	= pFooting->m_tvArrplan;
		rb.RotateTvArr(tvArrPlane, CDPoint(0, 0), dRotate);
		CDRect rect	= tvArrPlane.GetRect();

		CTwinVector tvPathPlaneOrg		= rb.RotateTv(pFooting->m_tvPathPlan, CDPoint(0, 0), dRotate);
		CTwinVector tvPathPlaneObtuse	= rb.RotateTv(pFooting->GetTvPathPlane_Obtuse(bFront), CDPoint(0, 0), dRotate);
		if(bFront)
		{
			rect.left = tvPathPlaneOrg.m_v1.x;
			rect.right = tvPathPlaneObtuse.m_v1.x;
		}
		else
		{
			rect.left	= tvPathPlaneObtuse.m_v2.x;
			rect.right	= tvPathPlaneOrg.m_v2.x;
		}
		rect.left -= max(pFooting->m_dMarginLeft, pFooting->m_dMarginRight);
		rect.right += max(pFooting->m_dMarginLeft, pFooting->m_dMarginRight);
		if(rect.left > rect.right)
			GetSwap(rect.left, rect.right);
		rect.left -= Dom.Always(5);
		rect.right += Dom.Always(5);
		rect.top += Dom.Always(5);
		rect.bottom -= Dom.Always(5);

		m_pStd->DrawCuttingTvArray(&Dom, tvArrPlane, rect, TRUE);

		Dom.Rotate(CDPoint(0, 0), ToDPointFrDegree(-dRotate));
	}
	else
	{
		m_pStd->DrawTvArray(&Dom, pFooting->m_tvArrplan, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());
	}
	Dom.SetCalcExtRect();
	CDRect rectTot	= Dom.GetExtRect();

	// �߽ɼ� �׸�
	if(bDrawCenterLine && !bObtuse)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
		double dMargin = 500;
		CTwinVector tv = pFooting->GetTvFootingTopCen();
			
		CDPoint xyDir = tv.GetXyDir();
		tv.m_v1 -= xyDir*dMargin;
		tv.m_v2 += xyDir*dMargin;

		Dom.LineTo(tv.m_v1, tv.m_v2);
	}

	*pDomP << Dom;

	return rectTot;
}

// Ȯ����� ��ܿ� ���� ġ�� ����
void CDrawFooting::DimTextFrontEL(CDomyun *pDomP, CFootingApp *pFooting, BOOL bLeft, BOOL bInput)
{
	if(!pFooting) return;
	if(!pFooting->m_bIs) return;
	CConcSymbol sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDomP, &Dom);

	CString sEL		= _T("");
	CString sNum	= _T("");
	CDPoint xyStt(0, 0);
	CTwinVector tv;
	double sw = bLeft ? -1 : 1;

	//  
	double dExt	= bLeft ? pFooting->m_exFooting.m_dEFWL : pFooting->m_exFooting.m_dEFWR;// .m_dMarginLeft : pFooting->m_exFooting.m_dMarginRight;
	if(pFooting->m_tvArrFront.GetTvByInfo(bLeft ? "����������" : "���ʿ�����", tv))
	{
		xyStt = tv.GetXyBottom();
		xyStt.x += ((Dom.Always(5)+dExt)*sw);
		sEL		= m_pStd->GetStringEL(toM(xyStt.y));
		
		if(bInput)
		{
			sNum	= sEL.Right(sEL.GetLength()-3);
			sEL		= sEL.Left(3);
		}
		sym.ArrowElevation(&Dom, sEL, "", xyStt, bLeft, TRUE, sNum);
		Dom.LineTo(xyStt.x-Dom.Always(2), xyStt.y, xyStt.x+Dom.Always(2), xyStt.y);

		// �ž��� ��� �ž� �ϴܿ��� el ǥ��
		if(pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_MASS)
		{
			xyStt.y -= pFooting->m_exFooting.GetMaxHeight();
			sEL		= m_pStd->GetStringEL(toM(xyStt.y));

			sym.ArrowElevation(&Dom, sEL, "", xyStt, bLeft, FALSE, "");
			Dom.LineTo(xyStt.x-Dom.Always(2), xyStt.y, xyStt.x+Dom.Always(2), xyStt.y);
		}
	}

	

	*pDomP << Dom;
}

// ��鵵 �Է� ġ����
void CDrawFooting::DimPlaneInput(CDomyun *pDomP, long nJ, BOOL bInput)
{
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;
	CFootingApp* pFooting = pBri->GetFootingByJijumNumber(nJ);
	if(!pFooting || pFooting->m_bIs == FALSE) return;
	if(pFooting->m_nType == FOOTING_TYPE_BOX) return;

	CTwinVector tv;

	BOOL bStt = nJ==0 ? TRUE : FALSE;
	BOOL bLeft = bStt ? FALSE : TRUE;

	pBri->GetTvWingWallPlane(bStt);
	CDPoint vAngWingL = pBri->GetAngleAzimuthWingWall(bStt, bLeft);
	CDPoint vAngWingR = pBri->GetAngleAzimuthWingWall(bStt, !bLeft);
	if(bStt)
	{
		vAngWingL *= -1;
		vAngWingR *= -1;
	}

	CDPoint vAngLen = pFooting->GetAngleAzimuth();	// ���� ���� ������
	CDPoint vAngLeft = pFooting->m_vAngLeft;		// ���� ������ ������
	CDPoint vAngRight = pFooting->m_vAngRight;		// ���� ������ ������
	CDPoint vAngTanL = pBri->GetAngleAzimuthTanOnJijum(nJ, -1);
	CDPoint vAngTanR = pBri->GetAngleAzimuthTanOnJijum(nJ, 1);
	CDPoint vAngBetL = vAngTanL.BetweenAngle(vAngLeft);
	CDPoint vAngBetR = vAngTanR.BetweenAngle(vAngRight);
	double dObqL = vAngLen.BetweenAngle(vAngLeft).Rotate90().GetAngleDegree();
	double dObqR = vAngRight.BetweenAngle(vAngLen).Rotate90().GetAngleDegree();
	if(pFooting->m_bFootingEndToWing)
	{
		if(pFooting->IsFootingEndToWing(TRUE))
			dObqL = vAngLen.BetweenAngle(vAngWingL).Rotate90().GetAngleDegree();
		if(pFooting->IsFootingEndToWing(FALSE))
			dObqR = vAngWingR.BetweenAngle(vAngLen).Rotate90().GetAngleDegree();
	}

	double dLenFront = pFooting->GetSideLength(TRUE);
	double dLenBack = pFooting->GetSideLength(FALSE);
	double dWidLeft = pFooting->GetSideWidth(TRUE);
	double dWidRight = pFooting->GetSideWidth(FALSE);
	double dDFL = Round(pFooting->m_dMarginLeft, 0);
	double dDFR = Round(pFooting->m_dMarginRight, 0);

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDomP, &Dom);

	// �ĸ� ġ��
	Dom.SetDirection(_T("TOP"));
	pFooting->m_tvArrplan.GetTvByInfo(_T("��������ĸ�"), tv);
	CDPoint xy0 = tv.m_v1;

	Dom.DimMoveTo(xy0.x, xy0.y, 0);
	Dom.DimLineTo(dLenBack, 0, COMMA(dLenBack));

	Dom.Rotate(xy0, vAngLen);
	*pDomP << Dom;

	// ���� ġ��
	if(Round(dLenFront, 0)!=Round(dLenBack, 0))
	{
		Dom.SetDirection(_T("BOTTOM"));
		pFooting->m_tvArrplan.GetTvByInfo(_T("�����������"), tv);
		xy0 = tv.m_v1;

		Dom.DimMoveTo(xy0.x, xy0.y, 0);
		Dom.DimLineTo(dLenFront, 0, COMMA(dLenFront));

		Dom.Rotate(xy0, vAngLen);
		*pDomP << Dom;
	}

	CWingWall* pWingL = pBri->GetWingWall(bStt, bLeft);
	CWingWall* pWingR = pBri->GetWingWall(bStt, !bLeft);
	// ���� ġ��
	if(pFooting->m_bFootingEndToWing && !bStt && pFooting->IsFootingEndToWing(FALSE))
		pFooting->m_tvArrplan.GetTvByInfo(_T("����������1"), tv);
	else
		pFooting->m_tvArrplan.GetTvByInfo(_T("��������ĸ�"), tv);

	Dom.SetDimObq(dObqR);
	Dom.SetDirection(_T("BOTTOM"));
	xy0 = tv.m_v1;

	Dom.DimMoveTo(xy0.x, xy0.y, 0);
	Dom.DimLineTo(dWidRight, 0, COMMA(dWidRight));

	if(pFooting->m_bFootingEndToWing && pWingR->m_bExist && pFooting->IsFootingEndToWing(FALSE))
		Dom.Rotate(xy0, vAngWingR);
	else
		Dom.Rotate(xy0, vAngRight);
	*pDomP << Dom;

	// ���� ġ��
	if(Round(dWidLeft, 0)!=Round(dWidRight, 0))
	{
		if(pFooting->m_bFootingEndToWing && !bStt && pFooting->IsFootingEndToWing(TRUE))
			pFooting->m_tvArrplan.GetTvByInfo(_T("�����������1"), tv);
		else
			pFooting->m_tvArrplan.GetTvByInfo(_T("��������ĸ�"), tv);

		Dom.SetDimObq(dObqL);
		Dom.SetDirection(_T("TOP"));
		xy0 = tv.m_v2;

		Dom.DimMoveTo(xy0.x, xy0.y, 0);
		Dom.DimLineTo(dWidLeft, 0, COMMA(dWidLeft));

		if(pFooting->m_bFootingEndToWing && pWingL->m_bExist && pFooting->IsFootingEndToWing(TRUE))
			Dom.Rotate(xy0, vAngWingL);
		else
			Dom.Rotate(xy0, vAngLeft);
		*pDomP << Dom;
	}

	Dom.SetDimObq(0.0);
	Dom.SetDimExo(4.0);


	// DFL
	CString str = vAngBetL.y>0 ? _T("BOTTOM") : _T("TOP");
	Dom.SetDirection(str);
	xy0 = pFooting->m_tvPathPlan.m_v2;

	if(dDFL!=0.0)
	{
		Dom.DimMoveTo(xy0.x, xy0.y, 0);
		if(bInput)
			Dom.DimLineToExtend(dDFL, 0, _T("DFL"), COMMA(dDFL));
		else
			Dom.DimLineTo(dDFL, 0, COMMA(dDFL));

	}

	
	Dom.Rotate(xy0, vAngLen);
	*pDomP << Dom;

	// DFR
	str = vAngBetR.y>0 ? _T("TOP") : _T("BOTTOM");
	Dom.SetDirection(str);
	xy0 = pFooting->m_tvPathPlan.m_v1 - vAngLen*dDFR;

	if(dDFR!=0.0)
	{
		Dom.DimMoveTo(xy0.x, xy0.y, 0);
		if(bInput)
			Dom.DimLineToExtend(dDFR, 0, _T("DFR"), COMMA(dDFR));
		else
			Dom.DimLineTo(dDFR, 0, COMMA(dDFR));
	}

	Dom.Rotate(xy0, vAngLen);
	*pDomP << Dom;
}

// ���� ���� ����
void CDrawFooting::DimPlaneAngle(CDomyun *pDomP, long nJ, BOOL bInput)
{
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;
	CFootingApp* pFooting = pBri->GetFootingByJijumNumber(nJ);

	if(!pFooting || pFooting->m_bIs == FALSE) return;
	if(pFooting->m_bFootingEndToWing) return;
	if(pFooting->m_nType == FOOTING_TYPE_BOX) return;

	CDPoint vAngLen = pFooting->GetAngleAzimuth();	// ���� ���� ������
	CDPoint vAngLeft = pFooting->m_vAngLeft;		// ���� ������ ������
	CDPoint vAngRight = pFooting->m_vAngRight;		// ���� ������ ������
	CDPoint vAngTanL = pBri->GetAngleAzimuthTanOnJijum(nJ, -1);
	CDPoint vAngTanR = pBri->GetAngleAzimuthTanOnJijum(nJ, 1);
	CDPoint vAngBetL = vAngTanL.BetweenAngle(vAngLeft);
	CDPoint vAngBetR = vAngTanR.BetweenAngle(vAngRight);
	vAngBetL.y = Round(vAngBetL.y, 8);
	vAngBetR.y = Round(vAngBetR.y, 8);

	double dDFL = Round(pFooting->m_dMarginLeft, 0);
	double dDFR = Round(pFooting->m_dMarginRight, 0);
	double se = nJ==0 ? -1.0 : 1.0;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetDimExo(0.0);

	CDPoint vTemp(0, 0);
	double dR = Dom.Always(1.0);
	CString /*str = _T(""), */strAng = _T("");

	// ����
	CDPoint xy0 = pFooting->m_tvPathPlan.m_v2 + dDFL*vAngLen;
	CDPoint xy1 = vAngBetL.y>0 ? xy0 + vAngTanL*dR*se : xy0 - vAngLeft*dR*se;
	CDPoint xy2 = vAngBetL.y>0 ? xy0 + vAngLeft*dR*se : xy0 - vAngTanL*dR*se;
	xy1 = CDPoint(Round(xy1.x, 8), Round(xy1.y, 8));
	xy2 = CDPoint(Round(xy2.x, 8), Round(xy2.y, 8));
	vTemp = CDPoint(vAngBetL.x, fabs(vAngBetL.y));
	strAng.Format(_T("(%.3f��)"), vTemp.GetAngleDegree());
	CString str = bInput ? _T("AFL")+strAng : vTemp.GetDegreeString(1, TRUE);
	Dom.DimArcMoveTo(xy0.x, xy0.y, dR, xy1.x, xy1.y, 1);
	Dom.DimArcLineTo(xy2.x, xy2.y, 1, str);

	// ����
	xy0 = pFooting->m_tvPathPlan.m_v1 - dDFR*vAngLen;
	xy1 = vAngBetR.y>0 ? xy0 + vAngTanR*dR*se : xy0 - vAngRight*dR*se;
	xy2 = vAngBetR.y>0 ? xy0 + vAngRight*dR*se : xy0 - vAngTanR*dR*se;
	xy1 = CDPoint(Round(xy1.x, 8), Round(xy1.y, 8));
	xy2 = CDPoint(Round(xy2.x, 8), Round(xy2.y, 8));
	vTemp = CDPoint(vAngBetR.x, fabs(vAngBetR.y));
	strAng.Format(_T("(%.3f��)"), vTemp.GetAngleDegree());
	str = bInput ? _T("AFR")+strAng : vTemp.GetDegreeString(1, TRUE);
	Dom.DimArcMoveTo(xy0.x, xy0.y, dR, xy1.x, xy1.y, 1);
	Dom.DimArcLineTo(xy2.x, xy2.y, 1, str);

	*pDomP << Dom;
}

void CDrawFooting::DrawRebarMain_Front(CDomyun *pDomP, CFootingApp *pFooting, long nCycle, long nDan, BOOL bUpper, BOOL bDimRebar, BOOL bMarkRebar, long nDrawLeft, BOOL bInput)
{
	if(!pFooting) return;
	if(!pFooting->m_bIs) return;

	if(nCycle == -1) nCycle = pFooting->GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > pFooting->GetCountCycleMainRebar()-1) return;
	if(nDan == -1) nDan = pFooting->GetDanMainRebar(nCycle, bUpper)-1;
	if(nDan < 0 || nDan > pFooting->GetDanMainRebar(nCycle, bUpper)-1) return;
	if(Compare(pFooting->GetDiaMainRebar(nCycle, bUpper, nDan), 0.0, "<=")) return;
	long nType	= pFooting->GetTypeByInputValue();

	long upper	= bUpper ? iUPPER : iLOWER;
	long nIdxEnd	= (bUpper && (nType == FOOTING_TYPE_BASIC_LEFTSLOPE || nType == FOOTING_TYPE_BASIC_RIGHTSLOPE)) ? 2 : 1; 

	long i = 0; for(i = 0; i < nIdxEnd; i++)
	{
		CRebarInfoCycle *pRC = NULL;
		if(i == 0)
		{
			switch(nCycle)
			{
				case 0: pRC	= &pFooting->m_rebarInfoCycle_Cycle1[upper][nDan]; break;
				case 1: pRC	= &pFooting->m_rebarInfoCycle_Cycle2[upper][nDan]; break;
				case 2: pRC	= &pFooting->m_rebarInfoCycle_Cycle3[upper][nDan]; break;
				case 3: pRC	= &pFooting->m_rebarInfoCycle_Cycle4[upper][nDan]; break;
				default: break;
			}
		}
		else
		{
			switch(nCycle)
			{
				case 0: pRC	= &pFooting->m_rebarInfoCycle_Cycle1_Ext[nDan]; break;
				case 1: pRC	= &pFooting->m_rebarInfoCycle_Cycle2_Ext[nDan]; break;
				case 2: pRC	= &pFooting->m_rebarInfoCycle_Cycle3_Ext[nDan]; break;
				case 3: pRC	= &pFooting->m_rebarInfoCycle_Cycle4_Ext[nDan]; break;
				default: break;
			}
		}
		if(!pRC) continue;
		if(Compare(pRC->GetLength(), 0.0, "=")) continue;
			

		CDomyun Dom(pDomP);
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);


		// ġ��
		if(bDimRebar)
		{
			DimRebarMainFront_Cycle1(&Dom, pFooting, 0, TRUE, nCycle, bUpper);
			DimRebarMainFront_Cycle2(&Dom, pFooting, 0, TRUE, nCycle, bUpper);
		}

		// ��ŷ
		if(bMarkRebar)
		{
			double dCover	= bUpper ? pFooting->m_dCoverUpper[nDan] : pFooting->m_dCoverLower[nDan];
			long nTypeMark	= i == 1 ? 0 : 1;
			if(nType == FOOTING_TYPE_BASIC && ((nCycle == 1 || nCycle == 3) && !pFooting->m_b2CycleMainRebarIsSameTypeWidth1Cycle[upper])) nTypeMark	= 2;
			m_pStd->MarkRebarInfoCycle(&Dom, pRC, bUpper, FALSE, dCover, nTypeMark, 0, CDRect(0, 0, 0, 0), 0.5, bInput, &pFooting->m_pArrRebarMain);
		}

		// RebarInfoCycle �׸���
		CRebarInfoCycle rc;
		rc	= *pRC;

		// ���ö���̰� 1, 3����Ŭ �׸����� ö�� �¿����� �ణ �÷��� �ϸ� ö�ٰ� ������ �ǵ��� �� �ش�. ////
		// �������� ������ ö�ٸ� �¿�� �÷��ָ� �ȴ�.
		BOOL bExt2	= nType == FOOTING_TYPE_BASIC_LEFTSLOPE || nType == FOOTING_TYPE_BASIC_RIGHTSLOPE;
		if(bUpper && (nCycle == 0 || nCycle == 2 || bExt2 || pFooting->m_b2CycleMainRebarIsSameTypeWidth1Cycle[upper]))
		{
			const double dGap = 30;
			if(Compare(rc.m_xyStt.x, rc.m_xyMid1.x, "="))
			{
				rc.m_xyStt.x -= dGap;
				rc.m_xyMid1.x -= dGap;
			}
			if(Compare(rc.m_xyMid2.x, rc.m_xyEnd.x, "="))
			{
				if(nType == FOOTING_TYPE_BASIC_LEFTSLOPE)
				{
					if(ComparePt(rc.m_xyMid2, rc.m_xyEnd, "="))
					{
						rc.m_xyMid2.x -= dGap;
						rc.m_xyEnd.x -= dGap;
					}
					else
					{
						rc.m_xyMid2.x += dGap;
						rc.m_xyEnd.x += dGap;
					}
				}
				else
				{
					rc.m_xyMid2.x += dGap;
					rc.m_xyEnd.x += dGap;
				}
			}
			if(!ComparePt(rc.m_xyEndExt, rc.m_xyEnd, "="))
			{
				if(Compare(rc.m_xyEndExt.x, rc.m_xyEnd.x, "="))
				{
					rc.m_xyEndExt.x += dGap;
					rc.m_xyEnd.x += dGap;
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////////////

		m_pStd->DrawRebarInfoCycle(&Dom, &rc, nDrawLeft);

		*pDomP << Dom;	
	}
}

// ���� ��ö�� ġ�� ����
// bInput�� FALSE�� ��� ��ö�� ������ ���̴�.
// �̶��� �ϸ鸸 ġ���� �����Ѵ�.
void CDrawFooting::DimRebarMainFront_Cycle1(CDomyun *pDomP, CFootingApp *pFooting, long nDimDan, BOOL bInput, long nCycle, BOOL bUpper)
{
	if(!pFooting) return;
	if(!(nCycle == 0 || nCycle == 2)) return;
	long nType	= pFooting->GetTypeByInputValue();
	
	if(nType == FOOTING_TYPE_BASIC || nType == FOOTING_TYPE_BASIC_BOTH)
		DimRebarMainFront_Cycle1_Basic(pDomP, pFooting, nDimDan, bInput, nCycle, bUpper);
	else if(nType == FOOTING_TYPE_LEFTSLOPE || nType == FOOTING_TYPE_RIGHTSLOPE)
		DimRebarMainFront_Cycle1_Slope(pDomP, pFooting, nDimDan, bInput, nCycle, bUpper);
	else if(nType == FOOTING_TYPE_BASIC_LEFTSLOPE || nType == FOOTING_TYPE_BASIC_RIGHTSLOPE)
		DimRebarMainFront_Cycle1_BasicSlope(pDomP, pFooting, nDimDan, bInput, nCycle, bUpper);

}

// 2cycle rebar info cycle ġ��
// bInput�� FALSE�� ��� ��ö�� ������ ���̴�.
// �̶��� �ϸ鸸 ġ���� �����Ѵ�.
void CDrawFooting::DimRebarMainFront_Cycle2(CDomyun *pDomP, CFootingApp *pFooting, long nDimDan, BOOL bInput, long nCycle, BOOL bUpper)
{
	if(!pFooting) return;
	if(!(nCycle == 1 || nCycle == 3)) return;
	long nType	= pFooting->GetTypeByInputValue();
	
	long upper	= bUpper ? iUPPER : iLOWER;
	if(nType == FOOTING_TYPE_BASIC || nType == FOOTING_TYPE_BASIC_BOTH)
	{
		if(pFooting->m_b2CycleMainRebarIsSameTypeWidth1Cycle[upper])
			DimRebarMainFront_Cycle1_Basic(pDomP, pFooting, nDimDan, bInput, nCycle, bUpper);
		else
			DimRebarMainFront_Cycle2_Basic(pDomP, pFooting, nDimDan, bInput, nCycle, bUpper);
	}
	else if(nType == FOOTING_TYPE_LEFTSLOPE || nType == FOOTING_TYPE_RIGHTSLOPE)
	{
		if(pFooting->m_b2CycleMainRebarIsSameTypeWidth1Cycle[upper])
			DimRebarMainFront_Cycle1_Slope(pDomP, pFooting, nDimDan, bInput, nCycle, bUpper);
		else
			DimRebarMainFront_Cycle2_Slope(pDomP, pFooting, nDimDan, bInput, nCycle, bUpper);
	}
}

// ���� ������ 
void CDrawFooting::DrawLeftSide(CDomyun *pDomP, CFootingApp *pFooting, BOOL bInput, long nDrawLeft, BOOL bLeft, BOOL bForBM, double dOffsetPath)
{
	if(!pFooting) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	CTwinVectorArray tvArrLeft, tvArrLeftHidden;
	pFooting->GetTvLeftSide(tvArrLeft, tvArrLeftHidden, bInput, nDrawLeft, bLeft, FALSE, FALSE, bForBM, dOffsetPath);

	m_pStd->DrawTvArray(&Dom, tvArrLeft, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());

	if(tvArrLeftHidden.GetSize() > 0)
	{
		m_pStd->DrawTvArray(&Dom, tvArrLeftHidden);
	}

	*pDomP << Dom;
}

// ���鵵 ���ö��
void CDrawFooting::DrawRebarSupport_Front(CDomyun *pDomP, CFootingApp *pFooting, BOOL bUpper, long nDan, BOOL bDimRebar, BOOL bMarkRebar, BOOL bDimTotal, BOOL bInput, long nVertDir)
{
	if(!pFooting) return;
	if(!pFooting->m_bIs) return;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;
	long nType	= pFooting->GetTypeByInputValue();

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	// ö�� �����ͼ� �׸���
	CHgBaseDrawStd baseDraw;
	CDPointArray xyArr;
	pFooting->GetXyRebarSupport_Front(xyArr, bUpper, nDan, nVertDir);
	CDPoint xyDirScale	= bUpper ? CDPoint(0, -1) : CDPoint(0, 1);
	baseDraw.DrawPointRebar(&Dom, xyArr, xyDirScale);

	long upper	= bUpper ? iUPPER : iLOWER;
	CPlaceRebarByBlock *pPlace	= &pFooting->m_placeByBlock_Support_Front[upper][nDan];
	
	// ġ�� ����
	if(bDimRebar)
	{
		if(xyArr.GetSize() > 0)
		{
			BOOL bVertDir		= pFooting->IsVertDir();
			double dCurAngle	= bVertDir ? 90 : pFooting->m_dAngle;
			double dApplyAngle	= nVertDir == -1 ? 90 : (nVertDir == 0 ? dCurAngle : pFooting->m_dAngle);
			CDPoint vAng		= ToDPointFrDegree(dApplyAngle);

			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

			long nCountDanUpper	= pFooting->GetDanMainRebar(0, TRUE);

			CDPointArray xyArrDim;
			CDPoint xyStt	= xyArr.GetAt(0);
			CStringArray sArrText, sArrUnderText;
			CString sDir	= _T("BOTTOM");
			long nDimDan	= bUpper ? nDan : nCountDanUpper+nDan;


			// ġ���� �Ǻ����� �߰��ؾ� ��. //////////////////////
			CTwinVector tvLeft, tvRight;
			CRebarInfoCycle *pRC	= pFooting->GetRebarInfoCycle(0, bUpper, nDan);
			pFooting->m_tvArrFront.GetTvByInfo("����������", tvLeft);
			pFooting->m_tvArrFront.GetTvByInfo("���ʿ�����", tvRight);

			
			// ���� �߰�(���ۺ�)
			xyArrDim.Add(bUpper ? tvLeft.GetXyTop() : tvLeft.GetXyBottom()); sArrText.Add("0"); sArrUnderText.Add("0");
			xyArrDim.Add(CDPoint(tvLeft.m_v1.x + pFooting->m_dCoverSide[0]/vAng.y, pRC->m_xyMid1.y)); sArrText.Add("0"); sArrUnderText.Add("0");
			if(dCurAngle != dApplyAngle && !bInput)
			{
				sArrUnderText.SetAt(sArrUnderText.GetSize()-1, COMMA(fabs(xyArrDim.GetAt(xyArrDim.GetSize()-1).x - xyArrDim.GetAt(xyArrDim.GetSize()-2).x)*vAng.y));
			}

			CPlaceRebarByBlock place23Dan;
			CRebarPlacing rb;

			// ö�� ������ ���Ÿ� �����̾�� �Ѵ�.
			// �׳� convert�ϸ� ��Ÿ��� ���� ���� �ִ�.
			if(nDan > 0)
			{
				CDPointArray xyArrTemp;
				pFooting->GetXyRebarSupport_Front(xyArrTemp, bUpper, nDan, -1);
				rb.ConvertPointArrayToOffsetInfo(xyArrTemp, place23Dan.m_xyArr, 0);
				place23Dan.m_nCountBlock	= place23Dan.m_xyArr.GetSize();
				pPlace	= &place23Dan;
			}

			// ġ�� ���� ���� ������
			if(dCurAngle == dApplyAngle || bInput)
			{
				m_pStd->GetDimInfoAtPlaceRebarByBlock(xyArr, xyArrDim, sArrText, sArrUnderText, pPlace, bUpper ? "U" : "L", xyArr.GetAt(0), CDPoint(1, 0), bInput, TRUE, dCurAngle, dApplyAngle);
			}
			else
			{
				CDPointArray xyArrDimTmp;
				CStringArray sArrTextTmp, sArrUnderTextTmp;
				m_pStd->GetDimInfoAtPlaceRebarByBlock(xyArr, xyArrDim, sArrText, sArrUnderTextTmp, pPlace, bUpper ? "U" : "L", xyArr.GetAt(0), CDPoint(1, 0), bInput, TRUE, dCurAngle, dApplyAngle);
				m_pStd->GetDimInfoAtPlaceRebarByBlock(xyArr, xyArrDimTmp, sArrUnderText, sArrTextTmp, pPlace, bUpper ? "U" : "L", xyArr.GetAt(0), CDPoint(1, 0), bInput, TRUE, dCurAngle, dCurAngle);
			}

			// ���� �߰�(����)
			if(nType == FOOTING_TYPE_LEFTSLOPE || nType == FOOTING_TYPE_RIGHTSLOPE)
			{
				xyArrDim.Add(pRC->m_xyEnd); sArrText.Add("0"); sArrUnderText.Add("0");
			}
			else
			{
				xyArrDim.Add(CDPoint(tvRight.m_v1.x - pFooting->m_dCoverSide[0]/vAng.y, pRC->m_xyMid2.y)); sArrText.Add("0"); sArrUnderText.Add("0");
			}

			if(dCurAngle != dApplyAngle && !bInput)
			{
				sArrUnderText.SetAt(sArrUnderText.GetSize()-1, COMMA(fabs(xyArrDim.GetAt(xyArrDim.GetSize()-1).x - xyArrDim.GetAt(xyArrDim.GetSize()-2).x)*vAng.y));
			}
		
			CString sDan	= _T("");
			sDan.Format("%s(%d��)", bUpper ? "���" : "�ϸ�", nDan+1);
			xyArrDim.Add(bUpper ? tvRight.GetXyTop() : tvRight.GetXyBottom()); sArrText.Add("0"); sArrUnderText.Add(sDan);

			if(dCurAngle != dApplyAngle && !bInput)
			{
				double dLen	= fabs(xyArrDim.GetAt(xyArrDim.GetSize()-1).x - xyArrDim.GetAt(xyArrDim.GetSize()-2).x);
				sArrText.SetAt(sArrText.GetSize()-1, COMMA(dLen)+COMMA2(dLen*vAng.y));

				long i = 0; for(i = 0; i < sArrUnderText.GetSize()-1; i++)
				{
					sArrUnderText.SetAt(i, "(" + sArrUnderText[i] + ")");
				}
			}
					
			// ġ�� ����
			if(xyArrDim.GetSize() > 1)
			{
				xyStt	= xyArrDim.GetAt(0);
				CTwinVector tvLower	= pFooting->m_tvArrFront.GetTvByInfo("�����ϸ�");
				xyStt.y = tvLower.m_v1.y;

				if(dCurAngle != dApplyAngle && !bInput)
				{
					double dTot	= baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
					if(!bUpper && bDimTotal)
					{
						Dom.SetDirection("BOTTOM");
						Dom.DimMoveTo(xyStt.x, xyStt.y, nDimDan+1);
						Dom.DimLineToExtend(dTot, nDimDan+1, COMMA(dTot), COMMA2(dTot*vAng.y));
					}
				}
				else
				{
					baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, !bUpper && bDimTotal, FALSE, FALSE);
				}
			}
		}
	}


	// ġ�� �����ϸ鼭 pPlace�� �ٸ��ɷ� ġȯ�Ѵ�. ���...
	// �׷��ϱ� ��ŷ�� �Ҷ��� �ٽ� �ǵ��� ������ �ǰ���..
	pPlace	= &pFooting->m_placeByBlock_Support_Front[upper][nDan];

	// ��ŷ
	long nTypeRebar	= bUpper ? F_REBAR_S_U1+nDan : F_REBAR_S_L1+nDan;
	// (ARCBRIDGE-1533) m_bFootingEndToWing ���ǿ� ���ؼ� ������ ö���� �ƴѵ� �̷� ������ ���ֳ�...
	// 	if(pFooting->m_bFootingEndToWing)
// 	{
// 		nTypeRebar	= bUpper ? F_REBAR_S_WU1+nDan : F_REBAR_S_WL1+nDan;
// 	}
	CString sMark	= pFooting->GetMarkRebarSupport(nTypeRebar, bInput, FALSE, -1);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		CTwinVector tvLower	= pFooting->m_tvArrFront.GetTvByInfo("�����ϸ�");

		CRebar *pRb = pFooting->GetRebarByType(&pFooting->m_pArrRebarSupport, nTypeRebar);

		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
		mark.SetRebar(xyArr);

		// ����
		mark.m_sMark					= sMark;
		mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pFooting->m_Fy);
		mark.m_nTypeMarkPointUpAndDown	= bUpper ? TYPE_MARKPOINT_UPPER : TYPE_MARKPOINT_LOWER;
		mark.m_nTypeMarkDir				= TYPE_MARKDIR_RIGHTTAN;
		mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
		mark.m_dPosScaleMarkStt			= 0.5+(nDan*0.15);

		// ��ü�� ���ؾ� �ȴ�.
		// FOOTING_TYPE_BASIC �̸� ���� ������ ���ϰ�
		/// FOOTING_TYPE_BASIC_BOTH �̸� ���������� ���Ѵ�.
		if(nType == FOOTING_TYPE_BASIC || nType == FOOTING_TYPE_BASIC_BOTH)
		{
			CTwinVector tvLower				= pFooting->m_tvArrFront.GetTvByInfo("�����ϸ�");
			CTwinVector tvLeft				= pFooting->m_tvArrFront.GetTvByInfo("���ʻ������");
			CTwinVector tvRight				= pFooting->m_tvArrFront.GetTvByInfo("���ʻ�����");

			double dWLeft	= tvLeft.GetHorLength();
			double dWRight	= tvRight.GetHorLength();
			if(nType == FOOTING_TYPE_BASIC)
				mark.m_dPosScaleMarkStt		= fabs((dWLeft<dWRight ? tvRight.GetXyMid().x : tvLeft.GetXyMid().x)-tvLower.GetXyLeft().x) / tvLower.GetHorLength();
			else
				mark.m_dPosScaleMarkStt		= fabs(tvRight.GetXyMid().x-tvLower.GetXyLeft().x) / tvLower.GetHorLength();
			mark.m_dPosScaleMarkStt	+= nDan*0.1;
		}

		CDPoint xyMark					= mark.GetXyMark();
		if(bUpper)
			mark.m_dAddHeight	= tvLower.m_v1.y + pFooting->GetHeight() - xyMark.y + MARK_BASEHEIGHT_UPPER;
		else
			mark.m_dAddHeight	= xyMark.y - tvLower.m_v1.y + MARK_BASEHEIGHT_LOWER;
		mark.m_dAddHeight	+= REBAR_RADIUS;
		mark.m_dCtc						= pFooting->GetCTCRebarSupport(bUpper, nDan);
		mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;

		mark.MarkRebar();
	}

	*pDomP << Dom;
}

void CDrawFooting::DrawRebarShear_Front(CDomyun *pDomP, CFootingApp *pFooting, BOOL bFront, BOOL bMarkRebar, BOOL bDimRebar, BOOL bDimTotal, long nDimDan, BOOL bInput, long nVertDir)
{
	if(!pFooting) return;
	long nType	= pFooting->GetTypeByInputValue();

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);

	CHgBaseDrawStd baseDraw;

	// ö�� �����ͼ� �׸���
	CTwinVectorArray tvArr;
	pFooting->GetTvRebarShear_Front(tvArr, bFront, nVertDir, bInput);
	if(tvArr.GetSize() == 0) return;

	// ����ö�� �׸���
	m_pStd->DrawTvArrayWidthDashDot(&Dom, tvArr, FALSE);

	CPlaceRebarByBlock *pPlace		= bFront ? &pFooting->m_placeByBlock_Shear_Front : &pFooting->m_placeByBlock_Shear_Side;

	CString sMark	= pFooting->GetMarkRebarShear(F_REBAR_V, bInput, FALSE);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);

		BOOL bVertDir	= pFooting->IsVertDir();
		CTwinVectorArray tvTmp;
		CTwinVector tvRebar	= bFront ? pFooting->GetTvOffset_Front(0, FALSE, bVertDir) : pFooting->GetTvOffset_Side_Upper(0, FALSE, bInput);
		if(tvRebar.m_v1.x > tvRebar.m_v2.x) 
			GetSwap(tvRebar.m_v1, tvRebar.m_v2);
		tvTmp.Add(tvRebar);
		tvRebar = bFront ? pFooting->GetTvOffset_Front(0, TRUE, bVertDir) : pFooting->GetTvOffset_Side_Upper(0, TRUE, bInput);
		if(tvRebar.m_v1.x > tvRebar.m_v2.x) 
			GetSwap(tvRebar.m_v1, tvRebar.m_v2);
		tvTmp.Add(tvRebar);
				
		mark.SetRebar(tvArr, tvTmp);
		// ����
		mark.m_sMark					= sMark;
		mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pFooting->m_FyShear);
		mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_POS;
		mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
		mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_UPPER;
		mark.m_nTypeMarkDir				= TYPE_MARKDIR_RIGHTTAN;
		mark.m_dPosScale				= 0.5;
		mark.m_dPosScaleMarkStt			= 0.2;
		mark.m_dAddHeight				= pFooting->GetHeight()/2;

		// ��ü�� ���ö�ٸ�ũ�� ���ϱ� ����
		// ������ 0.2�� �׸��� ������ ���ʻ���� ���� ��쿡�� �������� ��ũ�ϰ� 0.8�� �ش�
		if(nType == FOOTING_TYPE_BASIC)
		{
			CTwinVector tvLeft				= pFooting->m_tvArrFront.GetTvByInfo("���ʻ������");
			CTwinVector tvRight				= pFooting->m_tvArrFront.GetTvByInfo("���ʻ�����");

			if(tvLeft.GetHorLength() < tvRight.GetHorLength())
			{
				mark.m_dPosScaleMarkStt			= 0.8;
				mark.m_nTypeMarkDir				= TYPE_MARKDIR_LEFTTAN;
			}
		}
		mark.m_dCtc						= pFooting->GetCTCRebarShear();
		mark.m_nTypeRebar				= pFooting->m_rebarShearDetail.GetRebarType();

		mark.MarkRebar();
	} 

	if(bDimRebar)
	{
		long nSize	= tvArr.GetSize();
		if(tvArr.GetSize() > 0)
		{
			BOOL bVertDir		= pFooting->IsVertDir();
			double dCurAngle	= bVertDir ? 90 : pFooting->m_dAngle;
			double dApplyAngle	= nVertDir == -1 ? 90 : (nVertDir == 0 ? dCurAngle : pFooting->m_dAngle);

			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

			CDPointArray xyArrDim;
			CDPoint xyStt	= bInput ? tvArr.GetAt(0).m_v1 : tvArr.GetAt(0).m_v2;
			CStringArray sArrText, sArrUnderText;
			CString sDir	= bInput ? _T("BOTTOM") : _T("TOP");

			// ġ���� �Ǻ����� �߰��ؾ� ��. //////////////////////
			CTwinVector tvLeft, tvRight;
			CTwinVector tvRebarUpper;

			if(bFront)
			{
				pFooting->m_tvArrFront.GetTvByInfo("����������", tvLeft);
				pFooting->m_tvArrFront.GetTvByInfo("���ʿ�����", tvRight);

				CRebarInfoCycle *pRC	= pFooting->GetRebarInfoCycle(0, TRUE, 0);
				if(!pRC) return;

				tvRebarUpper.m_v1	= pRC->m_xyMid1;
				tvRebarUpper.m_v2	= pRC->m_xyMid2;
				tvRebarUpper.m_v1.x	= tvLeft.m_v1.x + pFooting->GetCoverSide(TRUE, 0, FALSE);
				tvRebarUpper.m_v2.x	= tvRight.m_v1.x - pFooting->GetCoverSide(FALSE, 0, FALSE);
			}
			else
			{
				CTwinVectorArray tvArrSide, tvArrHidden;
				pFooting->GetTvLeftSide(tvArrSide, tvArrHidden, bInput);
				tvArrSide.GetTvByInfo("����������", tvLeft);
				tvArrSide.GetTvByInfo("���ʿ�����", tvRight);

				tvRebarUpper		= pFooting->GetTvOffset_Side_Upper(0, TRUE, bInput);
			}

			// ���� �߰�(���ۺ�)
			xyArrDim.Add(tvLeft.GetXyTop()); sArrText.Add("0"); sArrUnderText.Add("0");
			xyArrDim.Add(tvRebarUpper.m_v1); sArrText.Add("0"); sArrUnderText.Add("0");

			
			// ġ�� ���� ���� ������
			CRebarPlacing rb;
			CDPointArray xyArr;
			long i = 0; for(i = 0; i < nSize; i++)
			{
				xyArr.Add(tvArr.GetAt(i).m_v2);
			}
			
			m_pStd->GetDimInfoAtPlaceRebarByBlock(xyArr, xyArrDim, sArrText, sArrUnderText, pPlace, "V", xyArrDim.GetAt(xyArrDim.GetSize()-1), CDPoint(1, 0), bInput, TRUE, dCurAngle, dApplyAngle);

			// ���� �߰�(����)
			xyArrDim.Add(tvRebarUpper.m_v2); sArrText.Add("0"); sArrUnderText.Add("0");
			xyArrDim.Add(tvRight.GetXyTop()); sArrText.Add("0"); sArrUnderText.Add("0");

			// ġ�� ����
			if(xyArrDim.GetSize() > 1)
			{
				CTwinVector tvLower;
				if(bFront)
				{
					tvLower	= pFooting->m_tvArrFront.GetTvByInfo("�����ϸ�");
				}
				else
				{
					CTwinVectorArray tvArrSide, tvArrHidden;
					pFooting->GetTvLeftSide(tvArrSide, tvArrHidden, bInput);
					tvLower	= tvArrSide.GetTvByInfo("�����ϸ�");
				}

				xyStt	= CDPoint(xyArrDim.GetAt(0).x, bInput ? tvLower.m_v1.y : tvLower.m_v1.y + pFooting->GetHeight());
				baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, bDimTotal, FALSE, FALSE);
			}
		}
	}

	*pDomP << Dom;
}

void CDrawFooting::DimRebarMainAssm_Front(CDomyun *pDomP, CFootingApp *pFooting, long nDimDan, long nCycle, long nDrawLeft)
{
	if(!pFooting) return;

	// DrawLeft ��� ���� ��
	// ������ �߽��� ���ؼ� �������� ������������ �Ǵ��ؼ� ġ���� ������.
	CTwinVector tvLower	= pFooting->m_tvArrFront.GetTvByInfo("�����ϸ�");
	double dStaCuttingOrg	= m_pStd->GetStaCuttingOrg();
	if(nDrawLeft == 0 || (nDrawLeft == -1 && tvLower.GetXyMid().x < dStaCuttingOrg) || (nDrawLeft == 1 && tvLower.GetXyMid().x > dStaCuttingOrg))
	{
		for(long upper = 0; upper < 2; upper++)
		{
			BOOL bUpper	= upper == iUPPER;

			if(nCycle == 0 || nCycle == 2)
				DimRebarMainFront_Cycle1(pDomP, pFooting, nDimDan, FALSE, nCycle, bUpper);
			else if(nCycle == 1 || nCycle == 3)
				DimRebarMainFront_Cycle2(pDomP, pFooting, nDimDan, FALSE, nCycle, bUpper);
		}
	}
}

// �Է¿�
// bInput�� FALSE�� ��� ��ö�� ������ ���̴�.
// �̶��� �ϸ鸸 ġ���� �����Ѵ�.
void CDrawFooting::DimRebarMainFront_Cycle1_BasicSlope(CDomyun *pDomP, CFootingApp *pFooting, long nDimDan, BOOL bInput, long nCycle, BOOL bUpper)
{
	if(!(nCycle == 0 || nCycle == 2)) return;
	if(nDimDan < 0) nDimDan	= 0;

	if(!pFooting) return;
	long nType	= pFooting->GetTypeByInputValue();

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CHgBaseDrawStd baseDraw;

	CRebarInfoCycle *pRC	= NULL;
	long nDan				= 0;
	long upper				= bUpper ? iUPPER : iLOWER;
	double dDist		= nDan == 0 ? pFooting->m_dDistMainRebarVert[nCycle][upper] : 100;
	double dDistExt		= nDan == 0 ? pFooting->m_dDistMainRebarVertExt[nCycle][upper] : 100;

	// ġ�����Կ� ����
	CDPointArray xyArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("");
	CDPoint xyStt(0, 0);


	CDRect rect	= pFooting->m_tvArrFront.GetRect();
	double dHeight	= pFooting->GetHeight();
	CTwinVector tvLower	= pFooting->m_tvArrFront.GetTvByInfo("�����ϸ�"),
				tvUpper	= pFooting->m_tvArrFront.GetTvByInfo("���ʻ������"),
				tvLeft	= pFooting->m_tvArrFront.GetTvByInfo("����������"),
				tvRight	= pFooting->m_tvArrFront.GetTvByInfo("���ʿ�����");
	if(nType == FOOTING_TYPE_LEFTSLOPE || nType == FOOTING_TYPE_RIGHTSLOPE)
				tvUpper	= pFooting->m_tvArrFront.GetTvByInfo("���ʻ��");
	CDPoint xyMatch(0, 0);
	double dCoverL	= pFooting->m_dCoverLower[0];
	double dCoverU	= pFooting->m_dCoverUpper[0];
	double dHeightExt	= pFooting->m_bLeftIsFrontGup ? tvRight.GetVerLength() : tvLeft.GetVerLength();

	// ���ö�� 
	switch(nCycle)
	{
		case 0: pRC	= &pFooting->m_rebarInfoCycle_Cycle1[iUPPER][nDan]; break;
		case 2: pRC	= &pFooting->m_rebarInfoCycle_Cycle3[iUPPER][nDan]; break;
		default: break;
	}

	if(pRC && bUpper)
	{
		if(bInput)
		{
			// (L1)
			xyArrDim.RemoveAll();
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();

			double dX	= pFooting->m_bLeftIsFrontGup ? rect.left : rect.right;
			xyArrDim.Add(CDPoint(dX, rect.bottom));
			sArrText.Add("0");
			xyArrDim.Add(CDPoint(dX, rect.bottom + dCoverL));
			sArrText.Add("0");
			xyArrDim.Add(CDPoint(dX, rect.bottom + dHeight - dCoverU - dDist));
			sArrText.Add("0");
			xyArrDim.Add(CDPoint(dX, rect.top - dCoverU));
			sArrText.Add("L1");
			xyArrDim.Add(CDPoint(dX, rect.top));
			sArrText.Add("0");

			sDir	= pFooting->m_bLeftIsFrontGup ? "LEFT" : "RIGHT";
			xyStt	= xyArrDim.GetAt(0);

			baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);

			// (L2)
			xyArrDim.RemoveAll();
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();

			dX	= pFooting->m_bLeftIsFrontGup ? rect.right : rect.left;
			xyArrDim.Add(CDPoint(dX, rect.bottom));
			sArrText.Add("0");
			xyArrDim.Add(CDPoint(dX, rect.bottom + dCoverL));
			sArrText.Add("0");
			xyArrDim.Add(CDPoint(dX, rect.bottom + dHeightExt - dCoverU - dDistExt));
			sArrText.Add("0");
			xyArrDim.Add(CDPoint(dX, rect.bottom + dHeightExt - dCoverU));
			sArrText.Add("L2");
			xyArrDim.Add(CDPoint(dX, rect.bottom + dHeightExt));
			sArrText.Add("0");

			sDir	= pFooting->m_bLeftIsFrontGup ? "RIGHT" : "LEFT";
			xyStt	= xyArrDim.GetAt(0);

			baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
		}
		else
		{
			// ��
			xyArrDim.RemoveAll();
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();
			xyArrDim.Add(CDPoint(rect.left, rect.bottom));
			xyArrDim.Add(CDPoint(rect.left, rect.bottom + dCoverL));
			xyArrDim.Add(CDPoint(rect.left, rect.top - dCoverU));
			xyArrDim.Add(CDPoint(rect.left, rect.top));

			sDir	= _T("LEFT");
			xyStt	= xyArrDim.GetAt(0);
			baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);

			// ��
			xyArrDim.RemoveAll();
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();
			xyArrDim.Add(CDPoint(rect.right, rect.bottom));
			xyArrDim.Add(CDPoint(rect.right, rect.bottom + dCoverL));
			xyArrDim.Add(CDPoint(rect.right, rect.top - dCoverU));
			xyArrDim.Add(CDPoint(rect.right, rect.top));

			sDir	= _T("RIGHT");
			xyStt	= xyArrDim.GetAt(0);
			baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
		}
	}


	// �ϸ�ö�� 
	switch(nCycle)
	{
		case 0: pRC	= &pFooting->m_rebarInfoCycle_Cycle1[iLOWER][nDan]; break;
		case 2: pRC	= &pFooting->m_rebarInfoCycle_Cycle3[iLOWER][nDan]; break;
		default: break;
	}
	if(pRC && !bUpper)
	{
		if(bInput)
		{
			// (L3)
			xyArrDim.RemoveAll();
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();

			double dX	= pFooting->m_bLeftIsFrontGup ? rect.left : rect.right;
			xyArrDim.Add(CDPoint(dX, rect.bottom));
			sArrText.Add("0");
			xyArrDim.Add(CDPoint(dX, rect.bottom + dCoverL));
			sArrText.Add("0");
			xyArrDim.Add(CDPoint(dX, rect.bottom + dCoverL + dDist));
			sArrText.Add("L3");
			xyArrDim.Add(CDPoint(dX, rect.top - dCoverU));
			sArrText.Add("0");
			xyArrDim.Add(CDPoint(dX, rect.top));
			sArrText.Add("0");

			sDir	= pFooting->m_bLeftIsFrontGup ? "RIGHT" : "LEFT";
			xyStt	= xyArrDim.GetAt(0);

			baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);

			// (L2)
			xyArrDim.RemoveAll();
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();

			dX	= pFooting->m_bLeftIsFrontGup ? rect.right : rect.left;
			xyArrDim.Add(CDPoint(dX, rect.bottom));
			sArrText.Add("0");
			xyArrDim.Add(CDPoint(dX, rect.bottom + dCoverL));
			sArrText.Add("0");
			xyArrDim.Add(CDPoint(dX, rect.bottom + dCoverL + dDistExt));
			sArrText.Add("L4");
			xyArrDim.Add(CDPoint(dX, rect.bottom + dHeightExt - dCoverU));
			sArrText.Add("0");
			xyArrDim.Add(CDPoint(dX, rect.bottom + dHeightExt));
			sArrText.Add("0");

			sDir	= pFooting->m_bLeftIsFrontGup ? "LEFT" : "RIGHT";
			xyStt	= xyArrDim.GetAt(0);

			baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
		}

		xyArrDim.RemoveAll();
		sArrText.RemoveAll();
		sArrUnderText.RemoveAll();

		xyArrDim.Add(CDPoint(rect.left, rect.bottom));
		xyArrDim.Add(CDPoint(pRC->m_xyMid1.x, rect.bottom));
		xyArrDim.Add(CDPoint(pRC->m_xyMid2.x, rect.bottom));
		xyArrDim.Add(CDPoint(rect.right, rect.bottom));

		sDir	= _T("BOTTOM");
		xyStt	= xyArrDim.GetAt(0);
		baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
	}

	*pDomP << Dom;
}

// �Է¿�
// bInput�� FALSE�� ��� ��ö�� ������ ���̴�.
// �̶��� �ϸ鸸 ġ���� �����Ѵ�.
void CDrawFooting::DimRebarMainFront_Cycle1_Slope(CDomyun *pDomP, CFootingApp *pFooting, long nDimDan, BOOL bInput, long nCycle, BOOL bUpper)
{
	if(!pFooting) return;
	long nType	= pFooting->GetTypeByInputValue();

	long upper	= bUpper ? iUPPER : iLOWER;
	if(!(nCycle == 0 || nCycle == 2) && !pFooting->m_b2CycleMainRebarIsSameTypeWidth1Cycle[upper]) return;
	if(nDimDan < 0) nDimDan	= 0;


	

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CHgBaseDrawStd baseDraw;


	CRebarInfoCycle *pRC	= NULL;
	long nDan				= 0;
	BOOL bLeftSlop			= nType == FOOTING_TYPE_LEFTSLOPE;

	// ġ�����Կ� ����
	CDPointArray xyArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("");
	CDPoint xyStt(0, 0);

	CTwinVector tvLower	= pFooting->m_tvArrFront.GetTvByInfo("�����ϸ�"),
				tvUpper	= pFooting->m_tvArrFront.GetTvByInfo("���ʻ������"),
				tvLeft	= pFooting->m_tvArrFront.GetTvByInfo("����������"),
				tvRight	= pFooting->m_tvArrFront.GetTvByInfo("���ʿ�����");
	if(nType == FOOTING_TYPE_LEFTSLOPE || nType == FOOTING_TYPE_RIGHTSLOPE)
				tvUpper	= pFooting->m_tvArrFront.GetTvByInfo("���ʻ��");
	CDPoint xyMatch(0, 0);
	double dCoverL	= pFooting->m_dCoverLower[0];
	double dCoverU	= pFooting->m_dCoverUpper[0];

	
	if(bUpper)
	{
		// ���ö�� 
		switch(nCycle)
		{
			case 0: pRC	= &pFooting->m_rebarInfoCycle_Cycle1[iUPPER][nDan]; break;
			case 1: pRC	= &pFooting->m_rebarInfoCycle_Cycle2[iUPPER][nDan]; break;
			case 2: pRC	= &pFooting->m_rebarInfoCycle_Cycle3[iUPPER][nDan]; break;
			case 3: pRC	= &pFooting->m_rebarInfoCycle_Cycle4[iUPPER][nDan]; break;
			default: break;
		}


		if(pRC/* && bInput*/)
		{
			// ���
			if(bInput)
			{
				sDir	= "TOP";
				xyArrDim.RemoveAll();
				sArrText.RemoveAll();
				sArrUnderText.RemoveAll();

				GetXyMatchLineAndLine(tvLeft.m_v1, tvLeft.GetXyDir(), pRC->m_xyMid1, (pRC->m_xyMid1-pRC->m_xyMid2).Unit(), xyMatch);
				xyArrDim.Add(xyMatch);
				xyArrDim.Add(pRC->m_xyMid1);
				xyArrDim.Add(pRC->m_xyMid2);
				xyArrDim.Add(pRC->m_xyEnd);
				GetXyMatchLineAndLine(tvRight.m_v1, tvRight.GetXyDir(), pRC->m_xyMid2, (pRC->m_xyMid2-pRC->m_xyMid1).Unit(), xyMatch);
				xyArrDim.Add(xyMatch);

				xyStt	= xyArrDim.GetAt(0);
				CTwinVector tvLower	= pFooting->m_tvArrFront.GetTvByInfo("�����ϸ�");
				xyStt.y	= tvLower.m_v1.y + pFooting->GetHeight();

				baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
				*pDomP << Dom;
			}

			// ������ ��������
			sDir	= "RIGHT";
			xyArrDim.RemoveAll();
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();

			GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), pRC->m_xyEndExt, (pRC->m_xyEndExt-pRC->m_xyEnd).Unit(), xyMatch);
			xyArrDim.Add(xyMatch);
			sArrText.Add("0");
			sArrUnderText.Add("");

			// �Է����� �ƴ϶�� �Ǻ�ġ�� ���� ��Ŵ
			if(!bInput)
			{
				xyArrDim.Add(xyMatch+CDPoint(0, dCoverL));
				sArrText.Add("0");
				sArrUnderText.Add("");
			}

			xyArrDim.Add(pRC->m_xyEndExt);
			sArrText.Add("0");
			sArrUnderText.Add("");
			xyArrDim.Add(pRC->m_xyEnd);
			sArrText.Add(bLeftSlop ? "L1" : "L2");
			sArrUnderText.Add(COMMA2(~(pRC->m_xyEnd-pRC->m_xyEndExt)));

			// �Է����� �ƴ϶�� �Ǻ�ġ�� ���� ��Ŵ
			if(!bInput)
			{
				xyArrDim.Add(tvUpper.m_v1+CDPoint(0, -dCoverU));
				sArrText.Add("0");
				sArrUnderText.Add("");
			}

			GetXyMatchLineAndLine(tvUpper.m_v1, tvUpper.GetXyDir(), pRC->m_xyEnd, (pRC->m_xyEnd-pRC->m_xyEndExt).Unit(), xyMatch);
			xyArrDim.Add(xyMatch);
			sArrText.Add("0");
			sArrUnderText.Add("(���)");

			xyStt	= xyArrDim.GetAt(0);

			if(!bInput)
			{
				sArrText.RemoveAll();
				long i = 0; for(i = 0; i < sArrUnderText.GetSize()-1; i++)
				{
					sArrUnderText.SetAt(i, "0");
				}
			}
			baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
			*pDomP << Dom;

			// ������ ��������
			sDir	= "LEFT";
			xyArrDim.RemoveAll();
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();
			
			GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), pRC->m_xyStt, (pRC->m_xyStt-pRC->m_xyMid1).Unit(), xyMatch);
			xyArrDim.Add(xyMatch);
			sArrText.Add("0");
			sArrUnderText.Add("");
			
			// �Է����� �ƴ϶�� �Ǻ�ġ�� ���� ��Ŵ
			if(!bInput)
			{
				xyArrDim.Add(xyMatch+CDPoint(0, dCoverL));
				sArrText.Add("0");
				sArrUnderText.Add("");
			}

			xyArrDim.Add(pRC->m_xyStt);
			sArrText.Add("0");
			sArrUnderText.Add("");
			xyArrDim.Add(pRC->m_xyMid1);
			sArrText.Add(bLeftSlop ? "L2" : "L1");
			sArrUnderText.Add(COMMA2(~(pRC->m_xyMid1-pRC->m_xyStt)));
			GetXyMatchLineAndLine(tvUpper.m_v1, tvUpper.GetXyDir(), pRC->m_xyMid1, (pRC->m_xyMid1-pRC->m_xyStt).Unit(), xyMatch);

			// �Է����� �ƴ϶�� �Ǻ�ġ�� ���� ��Ŵ
			if(!bInput)
			{
				xyArrDim.Add(tvUpper.m_v1+CDPoint(0, -dCoverU));
				sArrText.Add("0");
				sArrUnderText.Add("");
			}

			xyArrDim.Add(xyMatch);
			sArrText.Add("0");
			sArrUnderText.Add("(���)");

			xyStt	= xyArrDim.GetAt(0);

			if(!bInput)
			{
				sArrText.RemoveAll();
				long i = 0; for(i = 0; i < sArrUnderText.GetSize()-1; i++)
				{
					sArrUnderText.SetAt(i, "0");
				}
			}
			baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
			*pDomP << Dom;
		}
	}



	// �ϸ�ö�� 
	if(!bUpper)
	{
		switch(nCycle)
		{
			case 0: pRC	= &pFooting->m_rebarInfoCycle_Cycle1[iLOWER][nDan]; break;
			case 1: pRC	= &pFooting->m_rebarInfoCycle_Cycle2[iLOWER][nDan]; break;
			case 2: pRC	= &pFooting->m_rebarInfoCycle_Cycle3[iLOWER][nDan]; break;
			case 3: pRC	= &pFooting->m_rebarInfoCycle_Cycle4[iLOWER][nDan]; break;
			default: break;
		}
		if(pRC)
		{
			if(!bInput)
			{
				// �ϸ�
				sDir	= "BOTTOM";
				xyArrDim.RemoveAll();
				sArrText.RemoveAll();
				sArrUnderText.RemoveAll();

				xyArrDim.Add(tvLower.GetXyLeft());
				sArrText.Add("0");
				sArrUnderText.Add("");

				xyArrDim.Add(pRC->m_xyMid1);
				sArrText.Add("0");
				sArrUnderText.Add("");

				xyArrDim.Add(pRC->m_xyMid2);
				sArrText.Add("0");
				sArrUnderText.Add("");

				xyArrDim.Add(tvLower.GetXyRight());
				sArrText.Add("0");
				sArrUnderText.Add("");

				xyStt	= xyArrDim.GetAt(0);

				baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
				*pDomP << Dom;
			}

			if(1)//bInput)
			{
				// ������ ��������
				sDir	= "RIGHT";
				xyArrDim.RemoveAll();
				sArrText.RemoveAll();
				sArrUnderText.RemoveAll();
				
				GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), pRC->m_xyMid2, (pRC->m_xyMid2-pRC->m_xyEnd).Unit(), xyMatch);
				xyArrDim.Add(xyMatch);
				sArrText.Add("0");
				sArrUnderText.Add("");

				// �Է����� �ƴ϶�� �Ǻ�ġ�� ���� ��Ŵ
				if(!bInput)
				{
					xyArrDim.Add(xyMatch+CDPoint(0, dCoverL));
					sArrText.Add("0");
					sArrUnderText.Add("");
				}

				xyArrDim.Add(pRC->m_xyMid2);
				sArrText.Add("0");
				sArrUnderText.Add("");
				xyArrDim.Add(pRC->m_xyEnd);
				sArrText.Add(bLeftSlop ? "L3" : "L4");
				sArrUnderText.Add(COMMA2(~(pRC->m_xyEnd-pRC->m_xyMid2)));

				// �Է����� �ƴ϶�� �Ǻ�ġ�� ���� ��Ŵ
				if(!bInput)
				{
					xyArrDim.Add(tvUpper.m_v1+CDPoint(0, -dCoverU));
					sArrText.Add("0");
					sArrUnderText.Add("");
				}

				GetXyMatchLineAndLine(tvUpper.m_v1, tvUpper.GetXyDir(), pRC->m_xyEnd, (pRC->m_xyEnd-pRC->m_xyMid2).Unit(), xyMatch);
				xyArrDim.Add(xyMatch);
				sArrText.Add("0");
				sArrUnderText.Add("(�ϸ�)");

				xyStt	= xyArrDim.GetAt(0);

				if(!bInput)
				{
					sArrText.RemoveAll();
					long i = 0; for(i = 0; i < sArrUnderText.GetSize()-1; i++)
					{
						sArrUnderText.SetAt(i, "0");
					}
				}
				baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan+1, sDir, -1, FALSE, FALSE, FALSE);
				*pDomP << Dom;

				// ������ ��������
				sDir	= "LEFT";
				xyArrDim.RemoveAll();
				sArrText.RemoveAll();
				sArrUnderText.RemoveAll();
				
				GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), pRC->m_xyMid1, (pRC->m_xyMid1-pRC->m_xyStt).Unit(), xyMatch);
				xyArrDim.Add(xyMatch);
				sArrText.Add("0");
				sArrUnderText.Add("");

				// �Է����� �ƴ϶�� �Ǻ�ġ�� ���� ��Ŵ
				if(!bInput)
				{
					xyArrDim.Add(xyMatch+CDPoint(0, dCoverL));
					sArrText.Add("0");
					sArrUnderText.Add("");
				}

				xyArrDim.Add(pRC->m_xyMid1);
				sArrText.Add("0");
				sArrUnderText.Add("");
				xyArrDim.Add(pRC->m_xyStt);
				sArrText.Add(bLeftSlop ? "L4" : "L3");
				sArrUnderText.Add(COMMA2(~(pRC->m_xyStt-pRC->m_xyMid1)));
				
				// �Է����� �ƴ϶�� �Ǻ�ġ�� ���� ��Ŵ
				if(!bInput)
				{
					xyArrDim.Add(tvUpper.m_v1+CDPoint(0, -dCoverU));
					sArrText.Add("0");
					sArrUnderText.Add("");
				}

				GetXyMatchLineAndLine(tvUpper.m_v1, tvUpper.GetXyDir(), pRC->m_xyStt, (pRC->m_xyStt-pRC->m_xyMid1).Unit(), xyMatch);
				xyArrDim.Add(xyMatch);
				sArrText.Add("0");
				sArrUnderText.Add("(�ϸ�)");

				xyStt	= xyArrDim.GetAt(0);

				if(!bInput)
				{
					sArrText.RemoveAll();
					long i = 0; for(i = 0; i < sArrUnderText.GetSize()-1; i++)
					{
						sArrUnderText.SetAt(i, "0");
					}
				}
				baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan+1, sDir, -1, FALSE, FALSE, FALSE);
				*pDomP << Dom;
			}
		}
	}
}

// bInput�� FALSE�� ��� ��ö�� ������ ���̴�.
// �̶��� �ϸ鸸 ġ���� �����Ѵ�.
void CDrawFooting::DimRebarMainFront_Cycle1_Basic(CDomyun *pDomP, CFootingApp *pFooting, long nDimDan, BOOL bInput, long nCycle, BOOL bUpper)
{
	if(!pFooting) return;
	long nType	= pFooting->GetTypeByInputValue();

	long upper	= bUpper ? iUPPER : iLOWER;
	if(!(nCycle == 0 || nCycle == 2) && !pFooting->m_b2CycleMainRebarIsSameTypeWidth1Cycle[upper]) return;
	if(nDimDan < 0) nDimDan	= 0;


	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CHgBaseDrawStd baseDraw;

	CRebarInfoCycle *pRC	= NULL;
	long nDan				= 0;

	// ġ�����Կ� ����
	CDPointArray xyArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("");
	CDPoint xyStt(0, 0);

	CTwinVector tvLower	= pFooting->m_tvArrFront.GetTvByInfo("�����ϸ�"),
				tvUpper	= pFooting->m_tvArrFront.GetTvByInfo("���ʻ������"),
				tvLeft	= pFooting->m_tvArrFront.GetTvByInfo("����������"),
				tvRight	= pFooting->m_tvArrFront.GetTvByInfo("���ʿ�����");
	if(nType == FOOTING_TYPE_LEFTSLOPE || nType == FOOTING_TYPE_RIGHTSLOPE)
				tvUpper	= pFooting->m_tvArrFront.GetTvByInfo("���ʻ��");
	CDPoint xyMatch(0, 0);

	
	if(bUpper)
	{
		// ���ö�� 
		switch(nCycle)
		{
			case 0: pRC	= &pFooting->m_rebarInfoCycle_Cycle1[iUPPER][nDan]; break;
			case 1: pRC	= &pFooting->m_rebarInfoCycle_Cycle2[iUPPER][nDan]; break;
			case 2: pRC	= &pFooting->m_rebarInfoCycle_Cycle3[iUPPER][nDan]; break;
			case 3: pRC	= &pFooting->m_rebarInfoCycle_Cycle4[iUPPER][nDan]; break;
			default: break;
		}

		
		if(pRC/* && bInput*/)
		{
			// ���
			if(bInput)
			{
				sDir	= "TOP";
				xyArrDim.RemoveAll();
				sArrText.RemoveAll();
				sArrUnderText.RemoveAll();

				GetXyMatchLineAndLine(tvLeft.m_v1, tvLeft.GetXyDir(), pRC->m_xyMid1, (pRC->m_xyMid1-pRC->m_xyMid2).Unit(), xyMatch);
				xyArrDim.Add(xyMatch);
				xyArrDim.Add(pRC->m_xyMid1);
				xyArrDim.Add(pRC->m_xyMid2);
				GetXyMatchLineAndLine(tvRight.m_v1, tvRight.GetXyDir(), pRC->m_xyMid2, (pRC->m_xyMid2-pRC->m_xyMid1).Unit(), xyMatch);
				xyArrDim.Add(xyMatch);

				xyStt	= xyArrDim.GetAt(0);

				baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
				*pDomP << Dom;
			}

			// ������
			sDir	= "RIGHT";
			xyArrDim.RemoveAll();
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();

			if(!ComparePt(pRC->m_xyEnd, pRC->m_xyMid2, "="))
			{
				GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), pRC->m_xyEnd, (pRC->m_xyEnd-pRC->m_xyMid2).Unit(), xyMatch);
				xyArrDim.Add(xyMatch);
				sArrText.Add("0");
				sArrUnderText.Add("");
				xyArrDim.Add(pRC->m_xyEnd);
				sArrText.Add("0");
				sArrUnderText.Add("");
				xyArrDim.Add(pRC->m_xyMid2);
				sArrText.Add("L1");
				sArrUnderText.Add(COMMA2(~(pRC->m_xyMid2-pRC->m_xyEnd)));
				GetXyMatchLineAndLine(tvUpper.m_v1, tvUpper.GetXyDir(), pRC->m_xyMid2, (pRC->m_xyMid2-pRC->m_xyEnd).Unit(), xyMatch);
				xyArrDim.Add(xyMatch);
				sArrText.Add("0");
				sArrUnderText.Add("");

				xyStt	= xyArrDim.GetAt(0);

				if(!bInput)
				{
					sArrText.RemoveAll();
					sArrUnderText.RemoveAll();
				}
				baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
				*pDomP << Dom;
			}
		}
	}


	if(!bUpper)
	{
		// �ϸ�ö�� 
		switch(nCycle)
		{
			case 0: pRC	= &pFooting->m_rebarInfoCycle_Cycle1[iLOWER][nDan]; break;
			case 1: pRC	= &pFooting->m_rebarInfoCycle_Cycle2[iLOWER][nDan]; break;
			case 2: pRC	= &pFooting->m_rebarInfoCycle_Cycle3[iLOWER][nDan]; break;
			case 3: pRC	= &pFooting->m_rebarInfoCycle_Cycle4[iLOWER][nDan]; break;
			default: break;
		}
		if(pRC)
		{
			// �ϸ�
			sDir	= "BOTTOM";
			xyArrDim.RemoveAll();
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();

			GetXyMatchLineAndLine(tvLeft.m_v1, tvLeft.GetXyDir(), pRC->m_xyMid1, (pRC->m_xyMid1-pRC->m_xyMid2).Unit(), xyMatch);
			xyArrDim.Add(xyMatch);
			xyArrDim.Add(pRC->m_xyMid1);
			xyArrDim.Add(pRC->m_xyMid2);
			GetXyMatchLineAndLine(tvRight.m_v1, tvRight.GetXyDir(), pRC->m_xyMid2, (pRC->m_xyMid2-pRC->m_xyMid1).Unit(), xyMatch);
			xyArrDim.Add(xyMatch);

			xyStt	= xyArrDim.GetAt(0);

			baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
			*pDomP << Dom;

			// ������
			if(1/*bInput*/)
			{
				sDir	= "LEFT";
				xyArrDim.RemoveAll();
				sArrText.RemoveAll();
				sArrUnderText.RemoveAll();

				if(!ComparePt(pRC->m_xyMid1, pRC->m_xyStt, "="))
				{
					GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), pRC->m_xyMid1, (pRC->m_xyMid1-pRC->m_xyStt).Unit(), xyMatch);
					xyArrDim.Add(xyMatch);
					sArrText.Add("0");
					sArrUnderText.Add("");
					xyArrDim.Add(pRC->m_xyMid1);
					sArrText.Add("0");
					sArrUnderText.Add("");
					xyArrDim.Add(pRC->m_xyStt);
					sArrText.Add("L3");
					sArrUnderText.Add(COMMA2(~(pRC->m_xyStt-pRC->m_xyMid1)));
					GetXyMatchLineAndLine(tvUpper.m_v1, tvUpper.GetXyDir(), pRC->m_xyStt, (pRC->m_xyStt-pRC->m_xyMid1).Unit(), xyMatch);
					xyArrDim.Add(xyMatch);
					sArrText.Add("0");
					sArrUnderText.Add("");
					
					xyStt	= xyArrDim.GetAt(0);

					if(!bInput)
					{
						sArrText.RemoveAll();
						sArrUnderText.RemoveAll();
					}
					baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
					*pDomP << Dom;
				}
			}
		}
	}
}

// bInput�� FALSE�� ��� ��ö�� ������ ���̴�.
// �̶��� �ϸ鸸 ġ���� �����Ѵ�.
void CDrawFooting::DimRebarMainFront_Cycle2_Basic(CDomyun *pDomP, CFootingApp *pFooting, long nDimDan, BOOL bInput, long nCycle, BOOL bUpper)
{
	if(!pFooting) return;
	long nType	= pFooting->GetTypeByInputValue();
	long upper	= bUpper ? iUPPER : iLOWER;
	if(pFooting->m_b2CycleMainRebarIsSameTypeWidth1Cycle[upper])
	{
		DimRebarMainFront_Cycle1_Basic(pDomP, pFooting, nDimDan, bInput, nCycle, bUpper);
		return;
	}
	if(!(nCycle == 1 || nCycle == 3)) return;
	if(nDimDan < 0) nDimDan	= 0;

	

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CHgBaseDrawStd baseDraw;
	CRebarInfoCycle *pRC	= NULL;
	long nDan				= 0;
	BOOL bBoth				= (nType == FOOTING_TYPE_BASIC_BOTH);
	long nDimDanLower		= nDimDan+1;	// �ϸ��� ö�� ��ũ ������ ���� �ܼ����� �Ѵ� ����

	// ġ�����Կ� ����
	CDPointArray xyArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("");
	CDPoint xyStt(0, 0);

	CTwinVector tvLower	= pFooting->m_tvArrFront.GetTvByInfo("�����ϸ�"),
				tvUpper	= pFooting->m_tvArrFront.GetTvByInfo("���ʻ������"),
				tvLeft	= pFooting->m_tvArrFront.GetTvByInfo("����������"),
				tvRight	= pFooting->m_tvArrFront.GetTvByInfo("���ʿ�����");
	if(nType == FOOTING_TYPE_LEFTSLOPE || nType == FOOTING_TYPE_RIGHTSLOPE)
				tvUpper	= pFooting->m_tvArrFront.GetTvByInfo("���ʻ��");
	CDPoint xyMatch(0, 0);
	

	if(bUpper)
	{
		// ���ö�� 
		switch(nCycle)
		{
			case 1: pRC	= &pFooting->m_rebarInfoCycle_Cycle2[iUPPER][nDan]; break;
			case 3: pRC	= &pFooting->m_rebarInfoCycle_Cycle4[iUPPER][nDan]; break;
			default: break;
		}

		if(pRC && !bBoth/* && bInput*/)
		{
			// ���
			if(bInput)
			{
				sDir	= "TOP";
				xyArrDim.RemoveAll();
				sArrText.RemoveAll();
				sArrUnderText.RemoveAll();
				
				if(pFooting->m_bLeftIsFrontGup)
				{
					GetXyMatchLineAndLine(tvLeft.m_v1, tvLeft.GetXyDir(), pRC->m_xyMid2, (pRC->m_xyMid2-pRC->m_xyEnd).Unit(), xyMatch);
					xyArrDim.Add(xyMatch);
					sArrText.Add("0");
					sArrUnderText.Add("");

					xyArrDim.Add(pRC->m_xyMid2);
					sArrText.Add("0");
					sArrUnderText.Add("");

					xyArrDim.Add(pRC->m_xyEnd);
					sArrText.Add("L2");
					sArrUnderText.Add(COMMA2(~(pRC->m_xyEnd-pRC->m_xyMid2)));

					GetXyMatchLineAndLine(tvRight.m_v1, tvRight.GetXyDir(), pRC->m_xyEnd, (pRC->m_xyEnd-pRC->m_xyMid2).Unit(), xyMatch);
					xyArrDim.Add(xyMatch);
				}
				else
				{
					GetXyMatchLineAndLine(tvLeft.m_v1, tvLeft.GetXyDir(), pRC->m_xyMid2, (pRC->m_xyMid2-pRC->m_xyEnd).Unit(), xyMatch);

					xyArrDim.Add(xyMatch);
					sArrText.Add("0");
					sArrUnderText.Add("");

					xyArrDim.Add(pRC->m_xyEnd);
					sArrText.Add("0");
					sArrUnderText.Add("0");

					xyArrDim.Add(pRC->m_xyMid2);
					sArrText.Add("L2");
					sArrUnderText.Add(COMMA2(~(pRC->m_xyMid2-pRC->m_xyEnd)));

					GetXyMatchLineAndLine(tvRight.m_v1, tvRight.GetXyDir(), pRC->m_xyEnd, (pRC->m_xyEnd-pRC->m_xyMid2).Unit(), xyMatch);
					xyArrDim.Add(xyMatch);
				}

				xyStt	= xyArrDim.GetAt(0);

				if(!bInput)
				{
					sArrText.RemoveAll();
					sArrUnderText.RemoveAll();
				}
				baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
				*pDomP << Dom;
			}

			// ������
			sDir	= pFooting->m_bLeftIsFrontGup ? "LEFT" : "RIGHT";
			xyArrDim.RemoveAll();
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();

			if(!ComparePt(pRC->m_xyStt, pRC->m_xyMid1, "="))
			{
				GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), pRC->m_xyStt, (pRC->m_xyStt-pRC->m_xyMid1).Unit(), xyMatch);
				xyArrDim.Add(xyMatch);
				sArrText.Add("0");
				sArrUnderText.Add("");
				xyArrDim.Add(pRC->m_xyStt);
				sArrText.Add("0");
				sArrUnderText.Add("");
				xyArrDim.Add(pRC->m_xyMid1);
				sArrText.Add("L1");
				sArrUnderText.Add(COMMA2(~(pRC->m_xyMid1-pRC->m_xyStt)));
				GetXyMatchLineAndLine(tvUpper.m_v1, tvUpper.GetXyDir(), pRC->m_xyMid1, (pRC->m_xyMid1-pRC->m_xyStt).Unit(), xyMatch);
				xyArrDim.Add(xyMatch);
				sArrText.Add("0");
				
				xyStt	= xyArrDim.GetAt(0);

				if(!bInput)
				{
					sArrText.RemoveAll();
					sArrUnderText.RemoveAll();
				}
				baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
				*pDomP << Dom;
			}
		}
	}


	if(!bUpper)
	{
		// �ϸ�ö�� 
		switch(nCycle)
		{
			case 1: pRC	= &pFooting->m_rebarInfoCycle_Cycle2[iLOWER][nDan]; break;
			case 3: pRC	= &pFooting->m_rebarInfoCycle_Cycle4[iLOWER][nDan]; break;
			default: break;
		}
		if(pRC)
		{
			if(bBoth)
			{
				// �ϸ�
				sDir	= "BOTTOM";
				xyArrDim.RemoveAll();
				sArrText.RemoveAll();
				sArrUnderText.RemoveAll();

				GetXyMatchLineAndLine(tvLeft.m_v1, tvLeft.GetXyDir(), pRC->m_xyMid1, (pRC->m_xyMid1-pRC->m_xyMid2).Unit(), xyMatch);
				xyArrDim.Add(xyMatch);
				xyArrDim.Add(pRC->m_xyMid1);
				xyArrDim.Add(pRC->m_xyMid2);
				GetXyMatchLineAndLine(tvRight.m_v1, tvRight.GetXyDir(), pRC->m_xyMid2, (pRC->m_xyMid2-pRC->m_xyMid1).Unit(), xyMatch);
				xyArrDim.Add(xyMatch);

				xyStt	= xyArrDim.GetAt(0);

				baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDanLower, sDir, -1, FALSE, FALSE, FALSE);
				*pDomP << Dom;

				// ������
				if(1)//bInput)
				{
					sDir	= "RIGHT";
					xyArrDim.RemoveAll();
					sArrText.RemoveAll();
					sArrUnderText.RemoveAll();

					if(!ComparePt(pRC->m_xyMid2, pRC->m_xyEnd, "="))
					{
						GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), pRC->m_xyMid2, (pRC->m_xyMid2-pRC->m_xyEnd).Unit(), xyMatch);
						xyArrDim.Add(xyMatch);
						sArrText.Add("0");
						sArrUnderText.Add("");
						xyArrDim.Add(pRC->m_xyMid2);
						sArrText.Add("0");
						sArrUnderText.Add("");
						xyArrDim.Add(pRC->m_xyEnd);
						sArrText.Add("L3");
						sArrUnderText.Add(COMMA2(~(pRC->m_xyEnd-pRC->m_xyMid2)));
						GetXyMatchLineAndLine(tvUpper.m_v1, tvUpper.GetXyDir(), pRC->m_xyEnd, (pRC->m_xyEnd-pRC->m_xyMid2).Unit(), xyMatch);
						xyArrDim.Add(xyMatch);
						sArrText.Add("0");
						
						xyStt	= xyArrDim.GetAt(0);

						if(!bInput)
						{
							sArrText.RemoveAll();
							sArrUnderText.RemoveAll();
						}
						baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
						*pDomP << Dom;
					}
				}
			}
			else
			{
				// �ϸ�
				sDir	= "BOTTOM";
				xyArrDim.RemoveAll();
				sArrText.RemoveAll();
				sArrUnderText.RemoveAll();

				if(pFooting->m_bLeftIsFrontGup)
				{
					GetXyMatchLineAndLine(tvLeft.m_v1, tvLeft.GetXyDir(), pRC->m_xyEnd, (pRC->m_xyEnd-pRC->m_xyMid2).Unit(), xyMatch);
					xyArrDim.Add(xyMatch);
					sArrText.Add("0");
					sArrUnderText.Add("");
					xyArrDim.Add(pRC->m_xyEnd);
					sArrText.Add("0");
					sArrUnderText.Add("");
					xyArrDim.Add(pRC->m_xyMid2);
					sArrText.Add("L4");
					sArrUnderText.Add(COMMA2(~(pRC->m_xyMid2-pRC->m_xyEnd)));
					GetXyMatchLineAndLine(tvRight.m_v1, tvRight.GetXyDir(), pRC->m_xyMid2, (pRC->m_xyMid2-pRC->m_xyEnd).Unit(), xyMatch);
					xyArrDim.Add(xyMatch);
				}
				else
				{
					GetXyMatchLineAndLine(tvLeft.m_v1, tvLeft.GetXyDir(), pRC->m_xyEnd, (pRC->m_xyEnd-pRC->m_xyMid2).Unit(), xyMatch);
					xyArrDim.Add(xyMatch);
					sArrText.Add("0");
					sArrUnderText.Add("");
					xyArrDim.Add(pRC->m_xyMid2);
					sArrText.Add("0");
					sArrUnderText.Add("");
					xyArrDim.Add(pRC->m_xyEnd);
					sArrText.Add("L4");
					sArrUnderText.Add(COMMA2(~(pRC->m_xyEnd-pRC->m_xyMid2)));
					GetXyMatchLineAndLine(tvRight.m_v1, tvRight.GetXyDir(), pRC->m_xyMid2, (pRC->m_xyMid2-pRC->m_xyEnd).Unit(), xyMatch);
					xyArrDim.Add(xyMatch);
				}

				xyStt	= xyArrDim.GetAt(0);

				if(!bInput)
				{
					sArrText.RemoveAll();
					sArrUnderText.RemoveAll();
				}
				baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDanLower, sDir, -1, FALSE, FALSE, FALSE);
				*pDomP << Dom;

				// ������
				if(1)//bInput)
				{
					sDir	= pFooting->m_bLeftIsFrontGup ? "RIGHT" : "LEFT";
					xyArrDim.RemoveAll();
					sArrText.RemoveAll();
					sArrUnderText.RemoveAll();
			
					if(!ComparePt(pRC->m_xyMid1, pRC->m_xyStt, "="))
					{
						GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), pRC->m_xyMid1, (pRC->m_xyMid1-pRC->m_xyStt).Unit(), xyMatch);
						xyArrDim.Add(xyMatch);
						sArrText.Add("0");
						sArrUnderText.Add("");
						xyArrDim.Add(pRC->m_xyMid1);
						sArrText.Add("0");
						sArrUnderText.Add("");
						xyArrDim.Add(pRC->m_xyStt);
						sArrText.Add("L3");
						sArrUnderText.Add(COMMA2(~(pRC->m_xyStt-pRC->m_xyMid1)));
						GetXyMatchLineAndLine(tvUpper.m_v1, tvUpper.GetXyDir(), pRC->m_xyStt, (pRC->m_xyStt-pRC->m_xyMid1).Unit(), xyMatch);
						xyArrDim.Add(xyMatch);
						sArrText.Add("0");
						
						xyStt	= xyArrDim.GetAt(0);

						if(!bInput)
						{
							sArrText.RemoveAll();
							sArrUnderText.RemoveAll();
						}
						baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
						*pDomP << Dom;
					}
				}
			}
		}
	}
}

// bInput�� FALSE�� ��� ��ö�� ������ ���̴�.
// �̶��� �ϸ鸸 ġ���� �����Ѵ�.
void CDrawFooting::DimRebarMainFront_Cycle2_Slope(CDomyun *pDomP, CFootingApp *pFooting, long nDimDan, BOOL bInput, long nCycle, BOOL bUpper)
{
	if(!pFooting) return;
	long nType	= pFooting->GetTypeByInputValue();
	long upper	= bUpper ? iUPPER : iLOWER;
	if(pFooting->m_b2CycleMainRebarIsSameTypeWidth1Cycle[upper]) 
	{
		DimRebarMainFront_Cycle1_Slope(pDomP, pFooting, nDimDan, bInput, nCycle, bUpper);
		return;
	}
	if(!(nCycle == 1 || nCycle == 3)) return;
	if(nDimDan < 0) nDimDan	= 0;

	

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CHgBaseDrawStd baseDraw;

	CRebarInfoCycle *pRC	= NULL;
	long nDan				= 0;
	BOOL bLeftSlope			= (nType == FOOTING_TYPE_LEFTSLOPE);

	// ġ�����Կ� ����
	CDPointArray xyArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("");
	CDPoint xyStt(0, 0);

	// ���ö�� 
	switch(nCycle)
	{
		case 1: pRC	= &pFooting->m_rebarInfoCycle_Cycle2[iUPPER][nDan]; break;
		case 3: pRC	= &pFooting->m_rebarInfoCycle_Cycle4[iUPPER][nDan]; break;
		default: break;
	}

	CTwinVector tvLower	= pFooting->m_tvArrFront.GetTvByInfo("�����ϸ�"),
				tvUpper	= pFooting->m_tvArrFront.GetTvByInfo("���ʻ������"),
				tvLeft	= pFooting->m_tvArrFront.GetTvByInfo("����������"),
				tvRight	= pFooting->m_tvArrFront.GetTvByInfo("���ʿ�����");
	if(nType == FOOTING_TYPE_LEFTSLOPE || nType == FOOTING_TYPE_RIGHTSLOPE)
				tvUpper	= pFooting->m_tvArrFront.GetTvByInfo("���ʻ��");
	CDPoint xyMatch(0, 0);
	
	if(pRC/* && bInput*/)
	{
		CDPoint xyRcStt(0, 0);
		CDPoint xyRcEnd(0, 0);
		// ���
		if(bInput)
		{
			sDir	= "TOP";
			xyArrDim.RemoveAll();
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();

			xyRcStt	= bLeftSlope ? pRC->m_xyMid2 : pRC->m_xyStt;
			xyRcEnd	= bLeftSlope ? pRC->m_xyEnd : pRC->m_xyMid1;
			GetXyMatchLineAndLine(tvLeft.m_v1, tvLeft.GetXyDir(), xyRcStt, (xyRcStt-xyRcEnd).Unit(), xyMatch);
			xyArrDim.Add(xyMatch);
			sArrText.Add("0");
			sArrUnderText.Add("");
			xyArrDim.Add(xyRcStt);
			sArrText.Add("0");
			sArrUnderText.Add("");
			xyArrDim.Add(xyRcEnd);
			sArrText.Add("L2");
			sArrUnderText.Add(COMMA2(~(xyRcEnd-xyRcStt)));
			GetXyMatchLineAndLine(tvRight.m_v1, tvRight.GetXyDir(), xyRcEnd, (xyRcEnd-xyRcStt).Unit(), xyMatch);
			xyArrDim.Add(xyMatch);
			sArrText.Add("0");

			if(0)//!bLeftSlope)
			{
				xyRcStt	= xyArrDim.GetAt(0);
				xyRcEnd	= xyArrDim.GetAt(xyArrDim.GetSize()-1);
				GetSwap(xyRcStt, xyRcEnd);
				xyArrDim.SetAt(0, xyRcStt);
				xyArrDim.SetAt(xyArrDim.GetSize()-1, xyRcEnd);
			}


			xyStt	= xyArrDim.GetAt(0);

			if(!bInput)
			{
				sArrText.RemoveAll();
				sArrUnderText.RemoveAll();
			}
			baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
			*pDomP << Dom;
		}

		// ������Ǵ� ������
		sDir	= bLeftSlope ? "LEFT" : "RIGHT";
		xyArrDim.RemoveAll();
		sArrText.RemoveAll();
		sArrUnderText.RemoveAll();
		
		xyRcStt	= bLeftSlope ? pRC->m_xyStt : pRC->m_xyMid2;
		xyRcEnd	= bLeftSlope ? pRC->m_xyMid1 : pRC->m_xyEnd;
		GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), xyRcStt, (xyRcStt-xyRcEnd).Unit(), xyMatch);
		xyArrDim.Add(xyMatch);
		sArrText.Add("0");
		sArrUnderText.Add("");
		xyArrDim.Add(xyRcStt);
		sArrText.Add("0");
		sArrUnderText.Add("");
		xyArrDim.Add(xyRcEnd);
		sArrText.Add("L1");
		sArrUnderText.Add(COMMA2(~(xyRcEnd-xyRcStt)));
		GetXyMatchLineAndLine(tvUpper.m_v1, tvUpper.GetXyDir(), xyRcEnd, (xyRcEnd-xyRcStt).Unit(), xyMatch);
		xyArrDim.Add(xyMatch);
		sArrText.Add("0");
		
		if(!bLeftSlope)
		{
			xyRcStt	= xyArrDim.GetAt(0);
			xyRcEnd	= xyArrDim.GetAt(xyArrDim.GetSize()-1);
			GetSwap(xyRcStt, xyRcEnd);
			xyArrDim.SetAt(0, xyRcStt);
			xyArrDim.SetAt(xyArrDim.GetSize()-1, xyRcEnd);
		}
			
		xyStt	= xyArrDim.GetAt(0);

		if(!bInput)
		{
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();
		}
		baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
		*pDomP << Dom;
	}


	// �ϸ�ö�� 
	switch(nCycle)
	{
		case 1: pRC	= &pFooting->m_rebarInfoCycle_Cycle2[iLOWER][nDan]; break;
		case 3: pRC	= &pFooting->m_rebarInfoCycle_Cycle4[iLOWER][nDan]; break;
		default: break;
	}

	if(pRC)
	{
		// �ϸ�
		sDir	= "BOTTOM";
		xyArrDim.RemoveAll();
		sArrText.RemoveAll();
		sArrUnderText.RemoveAll();

		CDPoint xyRcStt	= bLeftSlope ? pRC->m_xyStt : pRC->m_xyMid2;
		CDPoint xyRcEnd	= bLeftSlope ? pRC->m_xyMid1 : pRC->m_xyEnd;
		GetXyMatchLineAndLine(tvLeft.m_v1, tvLeft.GetXyDir(), xyRcStt, (xyRcStt-xyRcEnd).Unit(), xyMatch);
		xyArrDim.Add(xyMatch);
		sArrText.Add("0");
		sArrUnderText.Add("");
		xyArrDim.Add(xyRcStt);
		sArrText.Add("0");
		sArrUnderText.Add("");
		xyArrDim.Add(xyRcEnd);
		sArrText.Add("L4");
		sArrUnderText.Add(COMMA2(~(xyRcEnd-xyRcStt)));
		GetXyMatchLineAndLine(tvRight.m_v1, tvRight.GetXyDir(), xyRcEnd, (xyRcEnd-xyRcStt).Unit(), xyMatch);
		xyArrDim.Add(xyMatch);
		sArrText.Add("0");

		xyStt	= xyArrDim.GetAt(0);
		if(!bInput)
		{
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();
		}
		baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
		*pDomP << Dom;

		// ������ �Ǵ� ������
		if(1)//bInput)
		{
			sDir	= bLeftSlope ? "RIGHT" : "LEFT";
			xyArrDim.RemoveAll();
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();

			xyRcStt	= bLeftSlope ? pRC->m_xyMid2 : pRC->m_xyMid1;
			xyRcEnd	= bLeftSlope ? pRC->m_xyEnd : pRC->m_xyStt;
			GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), xyRcStt, (xyRcStt-xyRcEnd).Unit(), xyMatch);
			xyArrDim.Add(xyMatch);
			sArrText.Add("0");
			sArrUnderText.Add("");
			xyArrDim.Add(xyRcStt);
			sArrText.Add("0");
			sArrUnderText.Add("");
			xyArrDim.Add(xyRcEnd);
			sArrText.Add("L3");
			sArrUnderText.Add(COMMA2(~(xyRcEnd-xyRcStt)));
			GetXyMatchLineAndLine(tvUpper.m_v1, tvUpper.GetXyDir(), xyRcEnd, (xyRcEnd-xyRcStt).Unit(), xyMatch);
			xyArrDim.Add(xyMatch);
			sArrText.Add("0");
			
			xyStt	= xyArrDim.GetAt(0);

			if(!bInput)
			{
				sArrText.RemoveAll();
				sArrUnderText.RemoveAll();
			}
			baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
			*pDomP << Dom;
		}
	}
}

// ���� ���鵵 ������ ����
// BOOL bDimSide : ��ü��� �Բ� �׷����� ���� ġ���� ���� �ʿ䰡 ��� �μ��� ��.
CDRect CDrawFooting::DrawRebar_FrontSectionStd(CDomyun *pDomP, CFootingApp *pFooting, BOOL bDimSide, BOOL bDimRebar, BOOL bMarkRebar, CString sMark, BOOL bInput, long nVertDir, BOOL bDrawOnlyMainRebar)
{
	if(!pFooting) return CDRect(0, 0, 0, 0);
	if(!pFooting->m_bIs) return CDRect(0, 0, 0, 0);


	CDomyun Dom(pDomP);
	long nDimDan	= 0;


	// �ܸ�
	DrawFront(&Dom, pFooting);
	Dom.SetCalcExtRect();
	CDRect rectTot	= Dom.GetExtRect();
	
	long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		for(long upper = 0; upper < 2; upper++)
		{
			BOOL bUpper		= upper == iUPPER ? TRUE : FALSE;

			// ��ö��
			DrawRebarMain_Front(&Dom, pFooting, 0, dan, bUpper, FALSE, FALSE, 0, bInput);
			DrawRebarMain_FrontSlope(&Dom, pFooting, dan, FALSE, dan == 0, bInput);
					
			// ��±�
			if(!bDrawOnlyMainRebar)
			{
				BOOL bDimTotal	= !bUpper && dan == pFooting->GetDanMainRebar(0, bUpper)-1;
				DrawRebarSupport_Front(&Dom, pFooting, bUpper, dan, bDimRebar, bMarkRebar, bDimTotal, FALSE, nVertDir);
				DrawRebarSupport_FrontSlope(&Dom, pFooting, dan, bDimRebar&&&dan==0, bMarkRebar&&dan==0, bInput);
			}
		}
	}

	nDimDan	= pFooting->GetDanMainRebar(0, TRUE);

	// ����ܰ� ö��
	if(!bDrawOnlyMainRebar)
	{
		DrawRebarSideOut_Front(&Dom, pFooting, TRUE, TRUE, FALSE, bMarkRebar, FALSE);
		DrawRebarSideOut_Front(&Dom, pFooting, FALSE, TRUE, bDimSide, FALSE, FALSE);

		// ����ö��
		DrawRebarShear_Front(&Dom, pFooting, TRUE, bMarkRebar, FALSE, FALSE, nDimDan, FALSE, nVertDir);
	}
		

	*pDomP << Dom;

	return rectTot;
}

// ��鵵 ��ö�� �׸�.
// nDrawUpper = -1 �̸� �Ʒ��ʿ�, 1�̸� ���ʿ� �׸�, 0�̸� ��� �׸�.
// BOOL bDimTotal : �Ѱ�ġ���� ��������?
// BOOL bDimForTopIsRight : ���ʿ����� ���� ������ �ϴ� ġ�� ����?
// BOOL bSlope : ��������� ��������?
void CDrawFooting::DrawRebarMain_Plane(CDomyun *pDomP, CFootingApp *pFooting, long nCycle, long nDan, BOOL bUpper, BOOL bDimRebar, BOOL bMarkRebar, BOOL bDimTotal, long nDrawUpper, BOOL bDimForTopIsRight, BOOL bExt, BOOL bObtuse, BOOL bFront, BOOL bSlope)
{
	long front	= bFront ? iFRONT : iBACK;
	if(!pFooting) return;
	if(bObtuse)
	{
		if(!pFooting->m_bIsObtuse[front]) return;
		bUpper	= TRUE;
		nCycle	= 0;
		nDan	= 0;
	}
	else if(bSlope)
	{
		if(!bUpper) return;
		if(!pFooting->IsRebarSupportFrontSlope(0)) return;
		nCycle	= -1;
		nDan	= 0;
	}
	
	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CTwinVectorArray tvArr, tvArrAngleFront, tvArrAngleBack;
	if(bObtuse)
		pFooting->GetTvRebarMain_Plane_Obtuse(tvArr, tvArrAngleFront, tvArrAngleBack, bFront);
	else if(bSlope)
		pFooting->GetTvRebarMain_Plane_Slope(tvArr, tvArrAngleFront, tvArrAngleBack, nDan);
	else
		pFooting->GetTvRebarMain_Plane(tvArr, tvArrAngleFront, tvArrAngleBack, bUpper, nCycle, nDan, 0, bExt);
	if(tvArr.GetSize() == 0 && tvArrAngleFront.GetSize() == 0 && tvArrAngleBack.GetSize() == 0) return;

	CRebarPlacing rb;
	double dRotate	= bDimForTopIsRight ? 180-pFooting->m_tvPathPlan.GetXyDir().GetAngleDegree() : -pFooting->m_tvPathPlan.GetXyDir().GetAngleDegree();

	CTwinVector tvUpper, tvLower;
	if(bDimRebar)
	{
		CTwinVectorArray tvArrDim, tvArrAngleFrontDim, tvArrAngleBackDim;
		if(bObtuse)
			pFooting->GetTvRebarMain_Plane_Obtuse(tvArrDim, tvArrAngleFrontDim, tvArrAngleBackDim, bFront);
		else
			pFooting->GetTvRebarMain_Plane(tvArrDim, tvArrAngleFrontDim, tvArrAngleBackDim, bUpper, -1, nDan, 0);

		// ġ�������Ҵ�� ��ǥ�� �������� �����Ѵ�.
		if(dRotate != 0)
		{
			rb.RotateTvArr(tvArrDim, CDPoint(0, 0), dRotate);
			rb.RotateTvArr(tvArrAngleFrontDim, CDPoint(0, 0), dRotate);
			rb.RotateTvArr(tvArrAngleBackDim, CDPoint(0, 0), dRotate);
			rb.RotateTvArr(pFooting->m_tvArrplan, CDPoint(0, 0), dRotate);
		}

		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		CDoubleArray dArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		CString sDir	= bUpper ? "TOP" : "BOTTOM";
		CDPoint xyStt	= CDPoint(0, 0);
		long nDimDan	= nDan+1;
		CTwinVector tvLeft	= pFooting->m_tvArrplan.GetTvByInfo("��������ĸ�");
		CTwinVector tvRight	= pFooting->m_tvArrplan.GetTvByInfo("�����������");

		// ���ʸ� �������� ���� �ϸ�
		tvRight.Sort(TRUE);
		tvLeft.Sort(TRUE);
		tvUpper	= bDimForTopIsRight ? tvRight : tvLeft;
		tvLower	= !bDimForTopIsRight ? tvRight : tvLeft;

		CRebarPlacing rb;

		// �����ö�ٿ� ���� ġ�� ���� /////////////////////////////////////////////////////
		if(tvArrAngleFrontDim.GetSize() > 0 || tvArrAngleBackDim.GetSize() > 0)
		{
			CTwinVectorArray tvArrTmp;
			long i = 0; for(i = 0; i < 2; i++)
			{
				tvArrTmp	= i == 0 ? tvArrAngleFrontDim : tvArrAngleBackDim;
				if(tvArrTmp.GetSize() == 0) continue;
				

				// ö���� �¿��� �ܰ��� �ٵ��� �����Ѵ�.(�׷��� ġ���� ��Ȯ�ϰ� ���Ե�)
				CTwinVector tmp;
				for(long j = 0; j < tvArrTmp.GetSize(); j++)
				{
					tmp	= tvArrTmp.GetAt(j);
					tmp.Sort(FALSE);
					GetXyMatchLineAndLine(tvUpper.m_v1, tvUpper.GetXyDir(), tmp.m_v2, tmp.GetXyDir(), tmp.m_v2);
					GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), tmp.m_v1, tmp.GetXyDir(), tmp.m_v1);
					tvArrTmp.SetAt(j, tmp);
				}
				// ����������� ö�ٰ� ���� �¿��� ��ǥ�� �¿��� ���, �ؿ��� ���� ������ �ȴ�. <----

				double dDimObq	= tvArrTmp.GetAt(0).GetXyDir().GetAngleDegree() - 90;
				if(bUpper) dDimObq *= -1;
				Dom.SetDimObq(dDimObq);


				// ���ݱ���.
				CDPointArray xyArr;
				CDoubleArray dArr;
				long nSize	= tvArrTmp.GetSize();
				for(long k = 0; k < nSize; k++)
					xyArr.Add(bUpper ? tvArrTmp.GetAt(k).m_v2 : tvArrTmp.GetAt(k).m_v1);

				if(i == 0)
				{
					if(bDimForTopIsRight)
						xyArr.Add(bUpper ? tvUpper.m_v2 : tvLower.m_v2);
					else
						xyArr.InsertAt(0, bUpper ? tvUpper.m_v1 : tvLower.m_v1);
				}
				else
				{
					if(bDimForTopIsRight)
						xyArr.InsertAt(0, bUpper ? tvUpper.m_v1 : tvLower.m_v1);
					else
						xyArr.Add(bUpper ? tvUpper.m_v2 : tvLower.m_v2);
				}
				

				xyArr.Sort(TRUE, TRUE);
				rb.ConvertPointArrayToOffsetArray(xyArr, dArr, TRUE, FALSE);

				// ġ������ ���� ����
				dArrDim.RemoveAll();
				sArrText.RemoveAll();
				sArrUnderText.RemoveAll();

				CDoubleArray dArrTmp;
				m_pStd->GetDimInfoAtDistArray(dArrDim, sArrText, sArrUnderText, dArr, "", FALSE, TRUE, TRUE, &Dom, 0.001, TRUE, TRUE, dArrTmp);
							
				
				if((!bDimForTopIsRight && i == 0) || (bDimForTopIsRight && i == 1))
				{
					xyStt	= bUpper ? tvUpper.m_v1 : tvLower.m_v1;
				}
				else
				{
					xyStt	= xyArr.GetAt(0);
				}

				baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE, TRUE, FALSE);
			}

			nDimDan++;
			if(bUpper) nDimDan++;
		}
		////////////////////////////////////////////////////////////////////////////////

				
		// ��ü ���� �¿��� ����.
		if(tvArrDim.GetSize() > 0)
		{
			CTwinVector tvUpper1Dan	= pFooting->GetTvOffset_Plane_Side(!bDimForTopIsRight);
			CTwinVector tvLower1Dan	= pFooting->GetTvOffset_Plane_Side(bDimForTopIsRight);
			if(dRotate != 0)
			{
				rb.RotateTv(tvUpper1Dan, CDPoint(0, 0), dRotate);
				rb.RotateTv(tvLower1Dan, CDPoint(0, 0), dRotate);
			}
			tvUpper1Dan.Sort(TRUE);
			tvLower1Dan.Sort(TRUE);
			
			Dom.SetDimObq(0);
			dArrDim.RemoveAll();
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();

			// ���ݱ���.
			CRebarPlacing rb;
			CDPointArray xyArr;
			CDoubleArray dArr;
			long nSize	= tvArrDim.GetSize();
			long i = 0; for(i = 0; i < nSize; i++)
			{
				// ���ϸ� �¿����� ����°��� ���� ���� ����
				// �ٸ��ʿ��� �����ϱ� ������.
				CDPoint xy	= CDPoint(0, 0);
				// (ARCBRIDGE-3668) ��ö���� ������ ȸ������
				// if(pFooting->m_bLeftIsFrontGup)
				if(bDimForTopIsRight == FALSE)
					xy	= bUpper ? tvArrDim[i].m_v1 : tvArrDim[i].m_v2;
				else
					xy	= bUpper ? tvArrDim[i].m_v2 : tvArrDim[i].m_v1;

				BOOL bOK	= TRUE;
				if(bUpper)
				{
					if(tvUpper1Dan.m_v1.x >= xy.x) bOK	= FALSE;
					else if(tvUpper1Dan.m_v2.x <= xy.x) bOK	= FALSE;
				}
				else
				{
					if(tvLower1Dan.m_v1.x >= xy.x) bOK	= FALSE;
					else if(tvLower1Dan.m_v2.x <= xy.x) bOK	= FALSE;
				}

				if(bOK)
				{
					// ö���� �¿��� �ܰ��� �ٵ��� �����Ѵ�.(�׷��� ġ���� ��Ȯ�ϰ� ���Ե�)
					if(bUpper)
						GetXyMatchLineAndLine(tvUpper.m_v1, tvUpper.GetXyDir(), xy, tvArrDim[i].GetXyDir(), xy);
					else
						GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), xy, tvArrDim[i].GetXyDir(), xy);
					xyArr.Add(xy);
				}
			}


			// ������ ���������� ���󰡴� ���
			if(pFooting->m_bFootingEndToWing)
			{
				CDPoint xyMatch(0, 0);
				CTwinVector tvTmp	= bUpper ? tvUpper : tvLower;
				CTwinVector tvTmp1Dan	= bUpper ? tvUpper1Dan : tvLower1Dan;

				if(!bObtuse || (!bDimForTopIsRight && bFront) || (bDimForTopIsRight && !bFront))
				{
					if(GetXyMatchSegAndLine(tvTmp.m_v1, tvTmp.m_v2, tvTmp1Dan.m_v1, tvTmp1Dan.GetXyDir().Rotate90(), xyMatch))
						xyArr.InsertAt(0, xyMatch);
					xyArr.InsertAt(0, tvTmp.m_v1);
				}

				if(!bObtuse || (!bDimForTopIsRight && !bFront) || (bDimForTopIsRight && bFront))
				{
					if(GetXyMatchSegAndLine(tvTmp.m_v1, tvTmp.m_v2, tvTmp1Dan.m_v2, tvTmp1Dan.GetXyDir().Rotate90(), xyMatch))
						xyArr.Add(xyMatch);
					xyArr.Add(tvTmp.m_v2);
				}
			}
			else
			{
				double dCover1 = pFooting->GetCoverSide(bDimForTopIsRight, 0, TRUE);
				double dCover2 = pFooting->GetCoverSide(!bDimForTopIsRight, 0, TRUE);

				//xyArr.InsertAt(0, bUpper ? tvUpper1Dan.m_v1 : tvLower1Dan.m_v1);
				if(!bObtuse || (!bDimForTopIsRight && bFront) || (bDimForTopIsRight && !bFront))
				{
					xyArr.InsertAt(0, CDPoint(bUpper ? tvUpper.m_v1.x+dCover1 : tvLower.m_v1.x+dCover1, bUpper ? tvUpper.m_v1.y : tvLower.m_v1.y));
					xyArr.InsertAt(0, bUpper ? tvUpper.m_v1 : tvLower.m_v1);
				}

				if(!bObtuse || (!bDimForTopIsRight && !bFront) || (bDimForTopIsRight && bFront))
				{
					xyArr.Add(CDPoint(bUpper ? tvUpper.m_v2.x - dCover2 : tvLower.m_v2.x - dCover2, bUpper ? tvUpper.m_v2.y : tvLower.m_v2.y));
					xyArr.Add(bUpper ? tvUpper.m_v2 : tvLower.m_v2);
				}
			}

			xyArr.Sort(TRUE);
			rb.ConvertPointArrayToOffsetArray(xyArr, dArr, TRUE, FALSE);

			CDoubleArray dArrTmp;
			m_pStd->GetDimInfoAtDistArray(dArrDim, sArrText, sArrUnderText, dArr, "", FALSE, TRUE, TRUE, &Dom, 0.001, FALSE, FALSE, dArrTmp);

			if(sArrUnderText.GetSize() > 0)
			{
				long nSize		= sArrUnderText.GetSize();
				CString sDan	= _T("");
				sDan.Format("%s(%d��)", bUpper ? "���" : "�ϸ�", nDan+1);
				sArrUnderText.SetAt(nSize-1, sDan);
			}

			xyStt	= bUpper ? tvUpper.m_v1 : tvLower.m_v1;
			xyStt.x	= xyArr.GetAt(0).x;
			baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, bDimTotal, FALSE, FALSE, TRUE, FALSE);
		}


		// ġ�������Ҷ� �������� ��ȯ�� ��ǥ ����
		if(dRotate != 0)
		{
			rb.RotateTvArr(tvArrDim, CDPoint(0, 0), -dRotate);
			rb.RotateTvArr(tvArrAngleFrontDim, CDPoint(0, 0), -dRotate);
			rb.RotateTvArr(tvArrAngleBackDim, CDPoint(0, 0), -dRotate);
			rb.RotateTvArr(pFooting->m_tvArrplan, CDPoint(0, 0), -dRotate);
			Dom.Rotate(CDPoint(0, 0), ToDPointFrDegree(-dRotate));
		}
		*pDomP << Dom;
	}

	// nDrawUpper�� �°� �ɷ���
	if(nDrawUpper != 0)
	{
		CRebarPlacing rb;
		CTwinVector tvCenter	= pFooting->GetTvCenter_Plane(1000);
		if(!bDimForTopIsRight)
			GetSwap(tvCenter.m_v1, tvCenter.m_v2);
		BOOL bLeft				= nDrawUpper == -1 ? TRUE : FALSE;
		rb.TrimTvArrayByLine(tvArr, tvCenter.m_v1, tvCenter.m_v2, bLeft, TRUE, FALSE);
		rb.TrimTvArrayByLine(tvArrAngleFront, tvCenter.m_v1, tvCenter.m_v2, bLeft, TRUE, FALSE);
		rb.TrimTvArrayByLine(tvArrAngleBack, tvCenter.m_v1, tvCenter.m_v2, bLeft, TRUE, FALSE);
	}

	// ������ ö�ٿ� cutting Area ����
	BOOL bAtLeft = nDrawUpper==-1 ? TRUE : FALSE;
	if(bDimForTopIsRight)
		bAtLeft	= !bAtLeft;
	ApplyCuttingArea_Plane(&Dom, pFooting, tvArr, bUpper, bAtLeft, nDan, TRUE, bExt, bSlope);
	ApplyCuttingArea_Plane(&Dom, pFooting, tvArrAngleFront, bUpper, bAtLeft, nDan, FALSE, bExt,  bSlope);
	ApplyCuttingArea_Plane(&Dom, pFooting, tvArrAngleBack, bUpper, bAtLeft, nDan, FALSE, bExt, bSlope);


	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	baseDraw.DrawTvArray(&Dom, tvArr);
	baseDraw.DrawTvArray(&Dom, tvArrAngleFront, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());
	baseDraw.DrawTvArray(&Dom, tvArrAngleBack, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());

	if(bMarkRebar)
	{
		CTwinVectorArray tvArrHalf;
		long i = 0; for(i = 0; i < tvArr.GetSize(); i++)
		{
			if(tvArr.GetAt(i).m_sInfo == "����")
			{
				tvArrHalf.Add(tvArr.GetAt(i));
				tvArr.RemoveAt(i);
				i--;
			}
		}



		CTwinVector tvLeft	= pFooting->m_tvArrplan.GetTvByInfo("��������ĸ�");
		CTwinVector tvRight	= pFooting->m_tvArrplan.GetTvByInfo("�����������");

		// ���ʸ� �������� ���� �ϸ�
		tvUpper	= bDimForTopIsRight ? tvRight : tvLeft;
		tvLower	= !bDimForTopIsRight ? tvRight : tvLeft;

		
		CTwinVectorArray tvArrSup, tvArrSupWall;
		pFooting->GetTvRebarSupport_Plane(tvArrSup, tvArrSupWall, bUpper, 0, 0, FALSE, FALSE, FALSE, bSlope);
		if(!bDimForTopIsRight)
		{
			rb.ReverseRebar(tvArr);
			rb.ReverseRebar(tvArrHalf);
			rb.ReverseRebar(tvArrSup);
		}

		CRebarInfoCycle *pRC	= pFooting->GetRebarInfoCycle(nCycle, bUpper, nDan, bExt);
		if(bObtuse)
			pRC	= &pFooting->m_rebarInfoCycle_Obtuse[front];
		else if(bSlope)
			pRC	= &pFooting->m_rebarInfoCycle_Slope[nDan];

		double dCountCycle	= (pFooting->GetCountCycleMainRebar()+1);
		
		if(pRC)
		{
			// ��������..
			CString sDescription	= _T("");
			CString sMark			= _T("");
			long s = 0; for(s = 0; s < 2; s++)
			{
				CTwinVectorArray tvArrTmp;
				if(s == 0)
				{
					sDescription.Format("%s", pRC->m_sDescription);
					tvArrTmp	= tvArr;
				}
				else
				{
					sDescription.Format("%s(����)", pRC->m_sDescription);
					tvArrTmp	= tvArrHalf;
				}

				sMark	= pFooting->GetMarkRebarMain(MARK_UPPER, FALSE, 0, FALSE, FALSE, sDescription, nDan);
				if(tvArr.GetSize() > 0 && m_pStd->IsMarkRebar(sMark))
				{
					CRebar *pRb = pFooting->GetRebarByDescription(&pFooting->m_pArrRebarMain, sDescription);
					CMarkDomyun mark;//(&Dom, m_pStd->m_pARoadOptionStd);
					mark.SetDomyun(&Dom, m_pStd->m_pARoadOptionStd);
					mark.SetRebar(tvArrTmp, tvArrSup);
					// ����
					mark.m_sMark					= sMark;
					mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pFooting->m_Fy);
					mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_POS;
					mark.m_nTypeMarkDir				= TYPE_MARKDIR_RIGHTTAN;
					mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
					mark.m_dPosScaleMarkStt			= 1./dCountCycle * (nCycle+1);
					mark.m_dPosScale				= 1./dCountCycle * (nCycle+1);
					mark.m_dPosScaleSpace			= 0.5 - (1./(dCountCycle-1))/2;
					if(bSlope)
					{
						mark.m_dPosScaleSpace		= 0.5;
						mark.m_dPosScaleMarkStt		= 0.8;
					}
				
					// ��ũ�� ����
					CDPoint xyMark					= mark.GetXyMark();
					CDPoint xyMatch(0, 0);
					GetXyMatchLineAndLine(tvUpper.m_v1, tvUpper.GetXyDir(), xyMark, tvArr.GetAt(0).GetXyDir(), xyMatch);
					mark.m_dAddHeight				= ~(xyMark - xyMatch) + Dom.Always(MARK_BASE_HEIGHT);
					if(nDan > 0)
					{
						mark.m_nTypeMarkDir	= nCycle < 2 ? TYPE_MARKDIR_LEFTTAN : TYPE_MARKDIR_RIGHTTAN;
						mark.m_dAddHeight	+= Dom.Always(MARK_BASE_HEIGHT);
					}
					mark.m_dCtc						= pFooting->m_dCTC_MainRebar;
					mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;

					mark.MarkRebar();	
				}
			}

			// �纸�� (�����)
			CTwinVector tvLeft	= pFooting->GetTvOffset_Plane_Side(TRUE);
			CTwinVector tvRight	= pFooting->GetTvOffset_Plane_Side(FALSE);
			tvLeft	= rb.RotateTv(tvLeft, CDPoint(0, 0), dRotate);
			tvRight	= rb.RotateTv(tvRight, CDPoint(0, 0), dRotate);

			if(bObtuse)
			{
				sDescription.Format("%s %d", pRC->m_sDescription, 2);
			}
			else
				sDescription.Format("%s %d", pRC->m_sDescription.Left(pRC->m_sDescription.GetLength()-2), 3);
			sMark	= pFooting->GetMarkRebarMain(MARK_UPPER, FALSE, 0, FALSE, FALSE, sDescription, nDan);
			if(tvArrAngleFront.GetSize() > 0 && m_pStd->IsMarkRebar(sMark) && nDan == 0)	// 1�ܸ� ��ũ�ص� �����ѵ�, 2,3�ܱ��� ���� ��ũ�ϸ� �ʹ� ����������
			{
				CRebar *pRb = pFooting->GetRebarByDescription(&pFooting->m_pArrRebarMain, sDescription);
				CMarkDomyun mark;
				mark.SetDomyun(&Dom, m_pStd->m_pARoadOptionStd);
				mark.SetRebar(tvArrAngleFront, tvArrSup);

				// ����
				mark.m_sMark					= sMark;
				mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pFooting->m_Fy);
				mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_END;
				mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
				mark.m_dPosScaleMarkStt			= 1./dCountCycle * (nCycle+1)+0.1;
				mark.m_dPosScale				= 1./dCountCycle * (nCycle+1)+0.1;
				mark.m_dPosScaleSpace			= 0.5 - (1./(dCountCycle-1))/2;
				mark.m_xyDirCountMark			= tvArrAngleFront.GetAt(0).GetXyDir().RotateInv90();
				mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_LOWER;
				mark.m_nTypeMarkDir				= bDimForTopIsRight ? TYPE_MARKDIR_RIGHTTAN : TYPE_MARKDIR_LEFTTAN;
				//#0028185 ��ŷ����
				//CDPoint xyMark					= mark.GetXyMark();
				//rb.RotateXy(xyMark, CDPoint(0, 0), dRotate);
				mark.m_dAddHeight				= pFooting->GetCoverSide(0, FALSE, TRUE);
				mark.m_dAngRotCount	= 77.5;
				mark.m_dCtc						= pFooting->m_dCTC_MainRebar;
				mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;

				mark.MarkRebar();	
			}

			// �纸�� (�ĸ��)
			if(bObtuse)
				sDescription.Format("%s %d", pRC->m_sDescription, 3);
			else
				sDescription.Format("%s %d", pRC->m_sDescription.Left(pRC->m_sDescription.GetLength()-2), 2);
			sMark	= pFooting->GetMarkRebarMain(MARK_UPPER, FALSE, 0, FALSE, FALSE, sDescription, nDan);
			
			if(tvArrAngleBack.GetSize() > 0 && m_pStd->IsMarkRebar(sMark) && nDan == 0)	// 1�ܸ� ��ũ�ص� �����ѵ�, 2,3�ܱ��� ���� ��ũ�ϸ� �ʹ� ����������
			{
				CRebar *pRb = pFooting->GetRebarByDescription(&pFooting->m_pArrRebarMain, sDescription);
				CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
				mark.SetRebar(tvArrAngleBack, tvArrSup);

				// ����
				mark.m_sMark					= sMark;
				mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pFooting->m_Fy);
				mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_STT;
				mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
				mark.m_dPosScaleMarkStt			= 1./dCountCycle * (nCycle+1)+0.1;
				mark.m_dPosScale				= 1./dCountCycle * (nCycle+1)+0.1;
				mark.m_dPosScaleSpace			= 0.5 - (1./(dCountCycle-1))/2;
				mark.m_xyDirCountMark			= tvArrAngleBack.GetAt(0).GetXyDir().RotateInv90();
				mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_LOWER;
				mark.m_nTypeMarkDir				= TYPE_MARKDIR_LEFTTAN;
				mark.m_dAddHeight				= pFooting->GetCoverSide(0, TRUE, TRUE);
				mark.m_dAngRotCount	= 22.5;
				mark.m_dCtc						= pFooting->m_dCTC_MainRebar;
				mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;
				mark.MarkRebar();	
			}
		}
	}


	*pDomP << Dom;
}


// ��� ������ ����.
// BOOL bRotate : �������� ȸ����ų��??
// BOOL bDrawPathPlane : ���� �� �׸�
// long nRotate : -1 = 90�� ȸ��, 0 = ȸ������, 1 = -90�� ȸ��
CDRect CDrawFooting::DrawRebar_PlaneStd(CDomyun *pDomP, CFootingApp *pFooting, BOOL bUpperAtUpper, BOOL bUpperAtLower, BOOL bMarkRebar, BOOL bDimRebar, long nRotate, BOOL bDrawPathPlane, BOOL bInput)
{
	if(!pFooting) return CDRect(0, 0, 0, 0); 
	if(!pFooting->m_bIs) return CDRect(0, 0, 0, 0); 
	BOOL bSameUL	= bUpperAtUpper==bUpperAtLower;

	CDomyun domTot(pDomP);
	CDomyun Dom(&domTot);
	
	// �ܰ��� �׸���
	CDRect rectTot	= DrawPlan(&Dom, pFooting, FALSE);;
	if(nRotate != 0)
	{
		// �����̷� ���̵��� ȸ��
		double dRotate	= nRotate == -1 ? 180-pFooting->m_tvPathPlan.GetXyDir().GetAngleDegree() : -pFooting->m_tvPathPlan.GetXyDir().GetAngleDegree();
		Dom.Rotate(CDPoint(0, 0), ToDPointFrDegree(dRotate));
		Dom.SetCalcExtRect();
		rectTot	= Dom.GetExtRect();
		*pDomP << Dom;
	}
	
	long cycle = 0, dan = 0, ext = 0;


	// ��ö��
	// ��ö�� �纸�� ��ġ ���� ����� ö���� ���� ������ �Ÿ��� �̸� ����� �ش�.
	
	long nMaxDanUpper	= pFooting->GetDanMainRebar(0, TRUE);
	long nMaxDanLower	= pFooting->GetDanMainRebar(0, FALSE);
	for(ext = 0; ext < 3; ext++)
	{
		BOOL bExt	= ext == 1;
		BOOL bSlope	= ext == 2;
		long nExt	= bExt ? 1 : -1;

		for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			if(dan > 0 && bExt) continue;
			if(dan > 0 && bSlope) continue;

			for(cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
			{
				if(cycle > 0 && bSlope) continue;

				DrawRebarMain_Plane(&Dom, pFooting, cycle, dan, bUpperAtUpper, !bExt && !bSlope && cycle==0, bMarkRebar, bUpperAtUpper ? (nMaxDanUpper==dan+1) : (nMaxDanLower==dan+1), bSameUL ? 0 : 1, nRotate==-1 ? TRUE : FALSE, bExt, FALSE, FALSE, bSlope);
				if(!bSameUL)
					DrawRebarMain_Plane(&Dom, pFooting, cycle, dan, bUpperAtLower, !bExt && !bSlope && cycle==0, bMarkRebar, bUpperAtLower ? (nMaxDanUpper==dan+1) : (nMaxDanLower==dan+1), -1, nRotate==-1 ? TRUE : FALSE, bExt, FALSE, FALSE, bSlope);
				else
					DrawRebarMain_Plane(&Dom, pFooting, cycle, dan, !bUpperAtUpper, !bExt && !bSlope && cycle==0, bMarkRebar, bUpperAtUpper ? (nMaxDanUpper==dan+1) : (nMaxDanLower==dan+1), bSameUL ? 0 : 1, nRotate==-1 ? TRUE : FALSE, bExt, FALSE, FALSE, bSlope);
			}
		}
		
		// ��±�
		for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			if(dan > 0 && bExt) continue;
			if(dan > 0 && bSlope) continue;

			DrawRebarSupport_Plane(&Dom, pFooting, dan, bUpperAtUpper, !bExt && !bSlope, bMarkRebar, bSameUL ? 0 : 1, bInput, nRotate==-1 ? TRUE : FALSE, nExt, FALSE, FALSE, bSlope);
			if(!bSameUL)
				DrawRebarSupport_Plane(&Dom, pFooting, dan, bUpperAtLower, !bExt && !bSlope, bMarkRebar, -1, bInput, nRotate==-1 ? TRUE : FALSE, nExt, FALSE, FALSE, bSlope);
		}
	}


	// ����ö��
	DrawRebarShear_Plane(&Dom, pFooting, pFooting->m_bLeftIsFrontGup ? 1 : -1);

	if(!bSameUL)
	{
		// �߽ɼ� �׸�
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
		CTwinVector tv = pFooting->GetTvCenter_Plane(500);
		Dom.LineTo(tv.m_v1, tv.m_v2);

		// �¿� ��ġ ǥ��
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SMALL_TITLE);
		CString sUpper	= bUpperAtUpper ? "�ϸ�" : "���";
		CString sLower	= bUpperAtLower ? "�ϸ�" : "���";

		// ��
		domTot << Dom;
		double dRotate	= -(90+pFooting->m_tvPathPlan.GetXyDir().GetAngleDegree());
		if(nRotate == 1)
		{
			dRotate	+= 180;
			GetSwap(tv.m_v1, tv.m_v2);
		}
		CDPoint xyText	= CDPoint(tv.m_v2.x, tv.m_v2.y + Dom.Always(10) + Dom.GetTextWidth(sUpper)/2);
		CObInsert *pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, "HGPOSMARK", xyText, Dom.GetScaleDim());	
		CHgBaseDrawStd::SetAttrib(&Dom, pIns, "DIR", sUpper);
		Dom.Rotate(xyText, ToDPointFrDegree(90));
		Dom.Rotate(tv.m_v2, ToDPointFrDegree(-dRotate));

		// ��
		domTot << Dom;
		xyText	= CDPoint(tv.m_v2.x, tv.m_v2.y - Dom.Always(10) - Dom.GetTextWidth(sLower)/2);
		pIns = CHgBaseDrawStd::InsertHCadBlock(&Dom, "HGPOSMARK", xyText, Dom.GetScaleDim());	
		CHgBaseDrawStd::SetAttrib(&Dom, pIns, "DIR", sLower);
		Dom.Rotate(xyText, ToDPointFrDegree(90));
		Dom.Rotate(tv.m_v2, ToDPointFrDegree(-dRotate));
	}

	domTot << Dom;

	// ���ؼ�
	if(bDrawPathPlane)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
		domTot.LineTo(pFooting->m_tvPathPlan.m_v1, pFooting->m_tvPathPlan.m_v2);
	}

	if(nRotate != 0)
	{
		// �����̷� ���̵��� ȸ��
		double dRotate	= nRotate == -1 ? 180-pFooting->m_tvPathPlan.GetXyDir().GetAngleDegree() : -pFooting->m_tvPathPlan.GetXyDir().GetAngleDegree();
		domTot.Rotate(CDPoint(0, 0), ToDPointFrDegree(dRotate));
	}

	*pDomP << domTot;

	return rectTot;
}

// ���� ��ö�� (��ö��)
void CDrawFooting::DrawRebarMain_Side(CDomyun *pDomP, CFootingApp *pFooting, long nCycle, BOOL bUpper, long nDan, BOOL bDimRebar, BOOL bDimTotal, BOOL bMarkRebar, BOOL bInput)
{
	if(!pFooting) return;
	if(!pFooting->m_bIs) return;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	// ö�� �����ͼ� �׸���
	CHgBaseDrawStd baseDraw;
	CDPointArray xyArr;
	pFooting->GetXyRebarMain_Side(xyArr, nCycle, bUpper, nDan, FALSE, FALSE, bInput);
	if(xyArr.GetSize() == 0) return;
	baseDraw.DrawPointRebar(&Dom, xyArr);

	long upper	= bUpper ? iUPPER : iLOWER;
	CPlaceRebarByBlock *pPlace	= &pFooting->m_placeByBlock_Main_Side[upper][nDan];
	
	// ġ�� ����
	if(bDimRebar)
	{
		CDPointArray xyArrForDim;
		pFooting->GetXyRebarMain_Side(xyArrForDim, -1, bUpper, nDan, FALSE, FALSE, bInput);
		if(xyArrForDim.GetSize() > 0)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

			CDPointArray xyArrDim;
			CDPoint xyStt	= xyArrForDim.GetAt(0);
			CStringArray sArrText, sArrUnderText;
			CString sDir	= bUpper ? _T("TOP") : _T("BOTTOM");

			// ġ���� �Ǻ����� �߰��ؾ� ��. //////////////////////
			CTwinVector tvLeft, tvRight;
			CTwinVector tvRebarLeft, tvRebarRight;
			CTwinVectorArray tvArrSide, tvArrHidden;
			pFooting->GetTvLeftSide(tvArrSide, tvArrHidden, bInput);
			tvLeft			= tvArrSide.GetTvByInfo("����������");
			tvRight			= tvArrSide.GetTvByInfo("���ʿ�����");
			tvRebarLeft		= pFooting->GetTvOffset_Side_Side(0, TRUE, bInput);
			tvRebarRight	= pFooting->GetTvOffset_Side_Side(0, FALSE, bInput);

			// ���� �߰�(���ۺ�)
			xyArrDim.Add(bUpper ? tvLeft.GetXyTop() : tvLeft.GetXyBottom()); sArrText.Add("0"); sArrUnderText.Add("0");
			xyArrDim.Add(tvRebarLeft.m_v1); sArrText.Add("0"); sArrUnderText.Add("0");
		

			// ġ�� ���� ���� ������
			if(bInput)
			{
				m_pStd->GetDimInfoAtPlaceRebarByBlock(xyArrForDim, xyArrDim, sArrText, sArrUnderText, pPlace, bUpper ? "U" : "L", xyArrForDim.GetAt(0), CDPoint(1, 0), bInput, TRUE);
			}
			else
			{
				long i = 0; for(i = 0; i < xyArrForDim.GetSize(); i++)
					xyArrDim.Add(xyArrForDim.GetAt(i));
			}

			// ���� �߰�(����)
			xyArrDim.Add(tvRebarRight.m_v1); sArrText.Add("0"); sArrUnderText.Add("0");
			xyArrDim.Add(bUpper ? tvRight.GetXyTop() : tvRight.GetXyBottom()); sArrText.Add("0"); sArrUnderText.Add("0");
			
			// ġ�� ����
			if(xyArrDim.GetSize() > 1)
			{
				// �ϸ� ö���̸鼭 �Է¿��� �ƴϸ� ��ü ġ���� ������ �� �ִ�.
				BOOL bTotal	= !bUpper && !bInput && bDimTotal;

				xyStt	= xyArrDim.GetAt(0);
				if(bInput)
					baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDan, sDir, -1, bTotal, FALSE, FALSE);
				else
					baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDan, sDir, -1, bTotal, TRUE, FALSE, TRUE, FALSE, FALSE, FALSE, FALSE); 

			}
		}
	}


	// ��ŷ
	CString sDescription = _T("");
	CString sMark	= _T("");
	if(bInput)
	{
		sMark.Format("B-%d", nCycle+1);
	}
	else
	{
		CRebarInfoCycle *pRC	= pFooting->GetRebarInfoCycle(nCycle, bUpper, nDan, FALSE);
		if(pRC)
		{
			sDescription = pRC->m_sDescription;
			sMark	= pFooting->GetMarkRebarMain(MARK_UPPER, FALSE, 0, FALSE, FALSE, sDescription, nDan);
		}
	}
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		CTwinVectorArray tvArrSide, tvArrHidden;
		pFooting->GetTvLeftSide(tvArrSide, tvArrHidden, bInput);
		CTwinVector tvUpper	= tvArrSide.GetTvByInfo("���ʻ��");
		CTwinVector tvLower	= tvArrSide.GetTvByInfo("�����ϸ�");

		CRebar *pRb = pFooting->GetRebarByDescription(&pFooting->m_pArrRebarMain, sDescription);
		CMarkDomyun mark;
		mark.SetDomyun(&Dom, m_pStd->m_pARoadOptionStd);
		mark.SetRebar(xyArr);
		
		// ����
		mark.m_sMark					= sMark;
		mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pFooting->GetDiaMainRebar(nCycle, bUpper, nDan), pFooting->m_Fy);
		mark.m_nTypeMarkPointUpAndDown	= bUpper ? TYPE_MARKPOINT_UPPER : TYPE_MARKPOINT_LOWER;
		mark.m_nTypeMarkDir				= TYPE_MARKDIR_RIGHTTAN;
		mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
		mark.m_dPosScaleMarkStt			= 0.5+(nDan*0.1);
		CDPoint xyMark	= mark.GetXyMark();
		if(bUpper)
			mark.m_dAddHeight	= tvUpper.m_v1.y - xyMark.y + MARK_BASEHEIGHT_UPPER;
		else
			mark.m_dAddHeight	= xyMark.y - tvLower.m_v1.y + MARK_BASEHEIGHT_LOWER;
		mark.m_dCtc						= pFooting->m_dCTC_MainRebar;
		mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;

		mark.MarkRebar();
	}

	*pDomP << Dom;
}

// nDrawUpper == -1 : �Ʒ��ʿ� �׸���, 1�̸� ���ʿ� �׸��� 0�̸� ��� �׸�
void CDrawFooting::DrawRebarSupport_Plane(CDomyun *pDomP, CFootingApp *pFooting, long nDan, BOOL bUpper, BOOL bDimRebar, BOOL bMarkRebar, long nDrawUpper, BOOL bInput, BOOL bDimForTopIsRight, long nExt, BOOL bObtuse, BOOL bFront, BOOL bSlope)
{
	if(!pFooting) return;
	long nType	= pFooting->GetTypeByInputValue();
	BOOL bExt	= nExt == 1;
	long front	= bFront ? iFRONT : iBACK;

	CHgBaseDrawStd baseDraw;
	CDomyun Dom(pDomP);

	CTwinVectorArray tvArr, tvArrWall, tvArrTot;
	if(bObtuse)
	{
		pFooting->GetTvRebarSupport_Plane_Obtuse(tvArr, bFront);
		tvArrTot	= tvArr;
	}
	else if(bSlope)
	{
		if(!bUpper) return;
		pFooting->GetTvRebarSupport_Plane_Slope(tvArr, nDan);
		tvArrTot	= tvArr;
	}
	else
	{
		pFooting->GetTvRebarSupport_Plane(tvArrTot, tvArrWall, bUpper, nDan, nExt, FALSE, FALSE, FALSE);
		pFooting->GetTvRebarSupport_Plane(tvArr, tvArrWall, bUpper, nDan, nExt, FALSE, FALSE, TRUE);
	}

	if(tvArrTot.GetSize() == 0) return;

	CRebarPlacing rb;
	CTwinVector tvUpper, tvLower;

	if(bDimRebar)
	{
		// ġ�������� ���Ǹ� ���ؼ� ö�ٹ�ġ�� ��ǥ�� "����"���� �����Ѵ�.
		double dRotate	= bDimForTopIsRight ? 180-pFooting->m_tvPathPlan.GetXyDir().GetAngleDegree() : -pFooting->m_tvPathPlan.GetXyDir().GetAngleDegree();
		
		if(dRotate != 0)
		{
			rb.RotateTvArr(tvArr, CDPoint(0, 0), dRotate);
			rb.RotateTvArr(tvArrWall, CDPoint(0, 0), dRotate);
			rb.RotateTvArr(pFooting->m_tvArrplan, CDPoint(0, 0), dRotate);
		}

		CTwinVectorArray tvArrTmp;
		tvArrTmp.AddFromTvArray(tvArr);

		if(!bDimForTopIsRight)
			rb.ReverseRebar(tvArrTmp);

		// ������ ġ�� ����.(����� 1�ܰ� ��ü ġ��, �ϸ��� 0�ܰ� ��ü ġ�� ��� ����)
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		CDoubleArray dArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		CString sDir	= !bDimForTopIsRight ? "LEFT" : "RIGHT";
		CDPoint xyStt	= CDPoint(0, 0);
		long nDimDan	= bUpper ? 1 : 0;
		CTwinVector tvLeft	= pFooting->m_tvArrplan.GetTvByInfo("��������ĸ�");
		CTwinVector tvRight = pFooting->m_tvArrplan.GetTvByInfo("�����������");

		// ���ʸ� �������� ���� �ϸ�
		tvRight.Sort(TRUE);
		tvLeft.Sort(TRUE);
		tvUpper	= bDimForTopIsRight ? tvRight : tvLeft;
		tvLower	= !bDimForTopIsRight ? tvRight : tvLeft;
		CRebarPlacing rb;

		if(tvArrTmp.GetSize() > 0)
		{
			// ���ݱ���.
			CDPointArray xyArr;
			CDoubleArray dArr;
			long nSize	= tvArrTmp.GetSize();
			for(long k = 0; k < nSize; k++)
				xyArr.Add(tvArrTmp.GetAt(k).m_v1);
			rb.ConvertPointArrayToOffsetArray(xyArr, dArr, FALSE, FALSE);

			// ġ������ ���� ����
			dArrDim.Add(xyArr[0].y-tvLower.m_v1.y);
			sArrText.Add(COMMA(dArrDim[0]));
			sArrUnderText.Add("0");

			CDoubleArray dArrTmp;
			m_pStd->GetDimInfoAtDistArray(dArrDim, sArrText, sArrUnderText, dArr, "", FALSE, TRUE, FALSE, &Dom, 0.001, TRUE, TRUE, dArrTmp);

			dArrDim.Add(tvUpper.m_v1.y-xyArr[xyArr.GetSize()-1].y);
			sArrText.Add(COMMA(tvUpper.GetXyLeft().y-xyArr[xyArr.GetSize()-1].y) + (bUpper ? "(���)" : "(�ϸ�)"));
			sArrUnderText.Add("0");

			if(bDimForTopIsRight)
			{
				xyStt.x	= max(tvUpper.m_v2.x, tvLower.m_v2.x);
				xyStt.y	= tvLower.m_v2.y;
			}
			else
			{
				xyStt.x	= min(tvUpper.m_v1.x, tvLower.m_v1.x);
				xyStt.y	= tvLower.m_v1.y;
			}

			if(bObtuse)
			{
				if(tvLeft.m_v1.y > tvRight.m_v1.y)
				{
					sDir = bFront ? "LEFT" : "RIGHT";
				}
				else
				{
					sDir = !bFront ? "LEFT" : "RIGHT";
				}
				xyStt.x = sDir == "LEFT" ? min(tvUpper.m_v1.x, tvLower.m_v1.x) : max(tvUpper.m_v2.x, tvLower.m_v2.x);
			}

			baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, bUpper || nDrawUpper==0, FALSE, FALSE, TRUE, FALSE);
		}

		// ġ�������� ���Ǹ� ���ؼ� ö�ٹ�ġ�� ��ǥ�� "����"���� ������ ��ǥ�� �����Ѵ�.
		if(dRotate != 0)
		{
			rb.RotateTvArr(tvArr, CDPoint(0, 0), -dRotate);
			rb.RotateTvArr(tvArrWall, CDPoint(0, 0), -dRotate);
			rb.RotateTvArr(pFooting->m_tvArrplan, CDPoint(0, 0), -dRotate);
			Dom.Rotate(CDPoint(0, 0), ToDPointFrDegree(-dRotate));

			*pDomP << Dom;
		}
	}



	// nDrawUpper�� �°� �ɷ���
	if(nDrawUpper != 0)
	{
		CRebarPlacing rb;
		CTwinVector tvCenter	= pFooting->GetTvCenter_Plane(1000);
		if(!bDimForTopIsRight)
			GetSwap(tvCenter.m_v1, tvCenter.m_v2);
		BOOL bLeft				= nDrawUpper == -1 ? TRUE : FALSE;
		rb.TrimTvArrayByLine(tvArr, tvCenter.m_v1, tvCenter.m_v2, bLeft, TRUE, FALSE);
		rb.TrimTvArrayByLine(tvArrWall, tvCenter.m_v1, tvCenter.m_v2, bLeft, TRUE, FALSE);
	}

	// ������ ö�ٿ� cutting Area ����
	BOOL bAtLeft = nDrawUpper==-1 ? TRUE : FALSE;
	if(bDimForTopIsRight)
		bAtLeft	= !bAtLeft;
	ApplyCuttingArea_Plane(&Dom, pFooting, tvArr, bUpper, bAtLeft, nDan, nDan==0, bExt, bSlope);
	ApplyCuttingArea_Plane(&Dom, pFooting, tvArrWall, bUpper, bAtLeft, nDan, nDan==0, bExt, bSlope);

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	baseDraw.DrawTvArray(&Dom, tvArr);
	baseDraw.DrawTvArray(&Dom, tvArrWall);

	long nTypeRebar = 0;
	CString sMark	= bUpper ? "BU" : "BL";
	if(bObtuse)
	{
		nTypeRebar	= bFront ? F_REBAR_S_OF : F_REBAR_S_OB;
		sMark	= pFooting->GetMarkRebarSupport(nTypeRebar, bInput, FALSE, -1);
	}
	else if(bSlope)
	{
		nTypeRebar	= F_REBAR_S_SL;
		sMark	= pFooting->GetMarkRebarSupport(nTypeRebar, bInput, FALSE, -1);
	}
	else
	{
		nTypeRebar	= bUpper ? F_REBAR_S_U1+nDan : F_REBAR_S_L1+nDan;
		sMark	= pFooting->GetMarkRebarSupport(nTypeRebar, bInput, FALSE, -1);
	}
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark) && tvArr.GetSize() > 0)
	{
		CTwinVectorArray tvArrMain1, tvArrMain2, tvArrMain, tvArrAngleFront, tvArrAngleBack;	// ��ö�� ��ǥ
		if(bObtuse)
		{
			pFooting->GetTvRebarMain_Plane_Obtuse(tvArrMain1, tvArrAngleFront, tvArrAngleBack, bFront);
			pFooting->GetTvRebarMain_Plane_Obtuse(tvArrMain2, tvArrAngleFront, tvArrAngleBack, bFront);
		}
		else if(bSlope)
		{
			pFooting->GetTvRebarMain_Plane_Slope(tvArrMain1, tvArrAngleFront, tvArrAngleBack, nDan);
		}
		else
		{
			pFooting->GetTvRebarMain_Plane(tvArrMain1, tvArrAngleFront, tvArrAngleBack, bUpper, 0, 0, 0);
			pFooting->GetTvRebarMain_Plane(tvArrMain2, tvArrAngleFront, tvArrAngleBack, bUpper, 1, 0, 0);
		}

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


		if(!bDimForTopIsRight)
		{
			rb.ReverseRebar(tvArrMain);
			rb.ReverseRebar(tvArr);
			rb.ReverseRebar(tvArrWall);
		}

		CTwinVector tvLeft	= pFooting->m_tvArrplan.GetTvByInfo("��������ĸ�");
		CTwinVector tvRight = pFooting->m_tvArrplan.GetTvByInfo("�����������");

		// ���ʸ� �������� ���� �ϸ�
		tvRight.Sort(TRUE);
		tvLeft.Sort(TRUE);
		tvUpper	= bDimForTopIsRight ? tvRight : tvLeft;
		tvLower	= !bDimForTopIsRight ? tvRight : tvLeft;

		long upper	= bUpper ? iUPPER : iLOWER;
		CPlaceRebarByBlock *pPlace		= &pFooting->m_placeByBlock_Support_Front[upper][nDan];
		if(bObtuse)
			pPlace	= &pFooting->m_placeByBlock_Obtuse_SupportRebar[front];
		if(bSlope && nType == FOOTING_TYPE_BASIC_LEFTSLOPE)
		{
			rb.ReverseRebar(tvArr);
		}
		
		CMarkDomyun mark;//(&Dom, m_pStd->m_pARoadOptionStd);
		mark.SetDomyun(&Dom, m_pStd->m_pARoadOptionStd);
		for(long wall = 0; wall < 2; wall++)
		{
			BOOL bWall	= wall == 1;
			if(bWall)
			{
				nTypeRebar	= bUpper ? F_REBAR_S_WU1+nDan : F_REBAR_S_WL1+nDan;
				sMark	= pFooting->GetMarkRebarSupport(nTypeRebar, bInput, FALSE, -1);
			}

			CRebar *pRb = pFooting->GetRebarByType(&pFooting->m_pArrRebarMain, nTypeRebar);

			if(tvArrMain.GetSize() == 0)
			{
				CTwinVector tv;
				CTwinVectorArray *pTvArr	= bWall ? &tvArrWall : &tvArr;
				if(pTvArr->GetSize() > 0)
				{
					tv.m_v1	= pTvArr->GetAt(0).m_v1;
					tv.m_v2	= pTvArr->GetAt(pTvArr->GetSize()-1).m_v1;
					tvArrMain.Add(tv);

					tv.m_v1	= pTvArr->GetAt(0).m_v2;
					tv.m_v2	= pTvArr->GetAt(pTvArr->GetSize()-1).m_v2;
					tvArrMain.Add(tv);
				}
			}
				
			mark.SetRebar(bWall ? tvArrWall : tvArr, tvArrMain);
			// ����
			mark.m_sMark					= sMark;
			mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pFooting->m_Fy);
			mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_STT;
			mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_LOWER;
			mark.m_nTypeMarkDir				= TYPE_MARKDIR_RIGHTTAN;
			mark.m_dPosScale				= bWall ? 0.7 : 0.5;
			if(bExt)
			{
				mark.m_dPosScale			= 0.3;
				mark.m_nTypeMarkDir			= TYPE_MARKDIR_LEFTTAN;
			}
			else if(bSlope)
			{
				mark.m_dPosScale			= 0.7;
			}
			if(tvArrMain.GetSize() > 0)
			{
				CDPoint xyMark				= mark.GetXyMark();
				CDPoint xyMatch(0, 0);
				GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), xyMark, tvArrMain.GetAt(0).GetXyDir(), xyMatch);
				mark.m_dAddHeight			= ~(xyMark - xyMatch);
				mark.m_dAddHeight			+= Dom.Always(MARK_BASE_HEIGHT);
			}
			mark.m_dCtc						= pFooting->GetCTCRebarSupport(bUpper, nDan);
			mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;

			mark.MarkRebar();
		}
	}


	*pDomP << Dom;
}

// ���鵵/���鵵���� ����ܰ�ö�� �׸���
void CDrawFooting::DrawRebarSideOut_Front(CDomyun *pDomP, CFootingApp *pFooting, BOOL bLeft, BOOL bFront, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, long nDimDan)
{
	if(!pFooting) return;
	if(!pFooting->m_bIs) return;
	long nType	= pFooting->GetTypeByInputValue();

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	// ö�� �����ͼ� �׸���
	CHgBaseDrawStd baseDraw;
	CDPointArray xyArr, xyArr2;
	pFooting->GetXyRebarSideOut_Front(xyArr, xyArr2, bLeft, bFront, bInput);
	if(xyArr.GetSize() == 0) return;
	CDPoint xyDirScale	= bLeft ? CDPoint(1, 0) : CDPoint(-1, 0);
	baseDraw.DrawPointRebar(&Dom, xyArr, xyDirScale);
	baseDraw.DrawPointRebar(&Dom, xyArr2, xyDirScale);

	CPlaceRebarByBlock *pPlace	= &pFooting->m_placeByBlock_Side_Out;
	
	// ġ�� ����
	if(bDimRebar)
	{
		double lC	= pFooting->m_dCoverLower[0];
		if(xyArr.GetSize() > 0)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

			CDPointArray xyArrDim;
			CDPoint xyStt	= xyArr.GetAt(0);
			CStringArray sArrText, sArrUnderText;
			CString sDir	= bLeft ? _T("LEFT") : _T("RIGHT");

			// ġ���� �Ǻ����� �߰��ؾ� ��. //////////////////////
			CTwinVector tvUpper, tvLower;
			if(bFront)
			{
				if(nType == FOOTING_TYPE_LEFTSLOPE || nType == FOOTING_TYPE_RIGHTSLOPE)
					tvUpper	= pFooting->m_tvArrFront.GetTvByInfo("���ʻ��");
				else
					tvUpper	= pFooting->m_tvArrFront.GetTvByInfo("���ʻ������");
				tvLower	= pFooting->m_tvArrFront.GetTvByInfo("�����ϸ�");
			}
			else
			{
				CTwinVectorArray tvArrSide, tvArrHidden;
				
				pFooting->GetTvLeftSide(tvArrSide, tvArrHidden, bInput);
				tvUpper	= tvArrSide.GetTvByInfo("���ʻ��");
				tvLower	= tvArrSide.GetTvByInfo("�����ϸ�");
			}
			
			// ���� �߰�(���ۺ�)
			xyArrDim.Add(bLeft ? tvLower.GetXyLeft() : tvLower.GetXyRight()); sArrText.Add("0"); sArrUnderText.Add("0");
			xyArrDim.Add(bLeft ? tvLower.GetXyLeft()+CDPoint(0, lC) : tvLower.GetXyRight()+CDPoint(0, lC)); sArrText.Add("0"); sArrUnderText.Add("0");

			// ġ�� ���� ���� ������
			CDPointArray xyArrTot;
			xyArrTot	= xyArr;
			long i = 0; for(i = 0; i < xyArr2.GetSize(); i++)
				xyArrTot.Add(xyArr2.GetAt(i));
			
			if(bInput)
			{
				m_pStd->GetDimInfoAtPlaceRebarByBlock(xyArrTot, xyArrDim, sArrText, sArrUnderText, pPlace, "S", xyArrDim.GetAt(1), CDPoint(0, 1), bInput, TRUE);
			}
			else
			{
				long i = 0; for(i = 0; i < xyArrTot.GetSize(); i++)
					xyArrDim.Add(xyArrTot.GetAt(i));
				sArrText.RemoveAll();
				sArrUnderText.RemoveAll();
			}

			// ���� �߰�(����)
			double dCoverUpper	= pFooting->m_dCoverUpper[0];
			xyArrDim.Add(tvUpper.GetXyLeft()+CDPoint(0, -dCoverUpper)); sArrText.Add("0"); sArrUnderText.Add("0");
			xyArrDim.Add(tvUpper.GetXyLeft()); sArrText.Add("0"); sArrUnderText.Add("0");
			
			// ġ�� ����
			if(xyArrDim.GetSize() > 1)
			{
				xyStt	= xyArrDim.GetAt(0);
				baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, !bInput, FALSE);
			}
		}
	}


	// ��ŷ
	CString sMark	= bInput ? _T("S") : pFooting->GetMarkRebarEtc(F_REBAR_V_SI, FALSE, FALSE);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		CRebar *pRb = pFooting->GetRebarByType(&pFooting->m_pArrRebarEtc, F_REBAR_V_SI);
		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
		mark.SetRebar(xyArr);

		// ����
		mark.m_sMark					= sMark;
		mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pFooting->m_Fy);
		mark.m_nTypeMarkPointUpAndDown	= bLeft ? TYPE_MARKPOINT_UPPER : TYPE_MARKPOINT_LOWER;
		mark.m_nTypeMarkDir				= TYPE_MARKDIR_RIGHTTAN;
		mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
		mark.m_dPosScaleMarkStt			= 0.5;
		mark.m_xyDirCountMark			= CDPoint(0, 1);
		mark.m_dCtc						= pPlace->GetCTC();
		mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;

		mark.MarkRebar();
	}

	// '��'�� ����ö��
	sMark	= bInput ? _T("S") : pFooting->GetMarkRebarEtc(F_REBAR_V_SI2, FALSE, FALSE);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark) && xyArr2.GetSize() > 0)
	{
		CRebar *pRb = pFooting->GetRebarByType(&pFooting->m_pArrRebarEtc, F_REBAR_V_SI2);
		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
		mark.SetRebar(xyArr2);

		// ����
		mark.m_sMark					= sMark;
		mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pFooting->m_Fy);
		mark.m_nTypeMarkPointUpAndDown	= bLeft ? TYPE_MARKPOINT_UPPER : TYPE_MARKPOINT_LOWER;
		mark.m_nTypeMarkDir				= TYPE_MARKDIR_RIGHTTAN;
		mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
		mark.m_dPosScaleMarkStt			= 0.5;
		mark.m_xyDirCountMark			= CDPoint(0, 1);
		mark.m_dCtc						= pPlace->GetCTC();
		mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;

		mark.MarkRebar();
	}

	*pDomP << Dom;
}

// ���鵵 ���ö��
void CDrawFooting::DrawRebarSupport_Side(CDomyun *pDomP, CFootingApp *pFooting, BOOL bUpper, long nDan, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput)
{
	if(!pFooting) return;
	if(!pFooting->m_bIs) return;

	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;

	CTwinVector tvSup	= pFooting->GetTvRebarSupport_Side(bUpper, nDan, bInput);
	if(Compare(tvSup.GetLength(), 0.0, "=")) return;
		
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	// ��±��� ��ö�ٿ� ���ؼ� dnl�� �׸���.
	double dRate = Dom.GetScaleDim()/50;
	// (ARCBRIDGE-2913) ���ö���� �̵���Ű�� �ʰ� ��ö���� ������ �׸���. AAbutPier�� ���߱� ���ؼ�.
	// double dRad	= bUpper ? -REBAR_RADIUS*dRate : REBAR_RADIUS*dRate;
	// tvSup.m_v1.y += dRad;
	// tvSup.m_v2.y += dRad;
	Dom.LineTo(tvSup.m_v1, tvSup.m_v2);

	double dCoverTot	= pFooting->m_dCoverLower[nDan] + pFooting->m_dCoverUpper[nDan];
	if(bUpper)
	{
		Dom.LineTo(tvSup.m_v1, tvSup.m_v1 - CDPoint(0, pFooting->GetHeight()/2-dCoverTot));
		Dom.LineTo(tvSup.m_v2, tvSup.m_v2 - CDPoint(0, pFooting->GetHeight()/2-dCoverTot));
	}
	else
	{
		Dom.LineTo(tvSup.m_v1, tvSup.m_v1 + CDPoint(0, pFooting->GetHeight()-dCoverTot));
		Dom.LineTo(tvSup.m_v2, tvSup.m_v2 + CDPoint(0, pFooting->GetHeight()-dCoverTot));
	}

	// ġ��
	if(bDimRebar)
	{
	}

	// ��ŷ
	if(bMarkRebar)
	{
	}

	*pDomP << Dom;
}

// ���� ���� ö��/�ܰ� std
// bInput : �Է¿��ϰ�� ��� ġ���� �� ��Ÿ������
// ��¿��� ���� �����ϸ� ��ö�� ġ���� ��Ÿ����, ���ö���̳� ����ö���� ġ������ ���Ѵ�.
CDRect CDrawFooting::DrawRebar_SideStd(CDomyun *pDomP, CFootingApp *pFooting, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput)
{
	if(!pFooting) return CDRect(0, 0, 0, 0);
	if(!pFooting->m_bIs) return CDRect(0, 0, 0, 0);

	CDomyun Dom(pDomP);

	long dan	= 0;
	long nDimDan	= 0;

	// �Ϲݵ�(����)
	DrawLeftSide(&Dom, pFooting, bInput);
	Dom.SetCalcExtRect();
	CDRect rectTot	= Dom.GetExtRect();

	nDimDan	= pFooting->GetDanMainRebar(0, FALSE);

	// ������
	for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		// ���ö��
		DrawRebarSupport_Side(&Dom, pFooting, TRUE, dan, bInput && bDimRebar, bMarkRebar, bInput);
		DrawRebarSupport_Side(&Dom, pFooting, FALSE, dan, bInput && bDimRebar, bMarkRebar, bInput);

		for(long cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
		{
			BOOL bDim	= cycle==0;
			// ��ö�� 
			DrawRebarMain_Side(&Dom, pFooting, cycle, TRUE, dan, bDim && bDimRebar && bInput, FALSE, bDim && bMarkRebar, bInput);
			DrawRebarMain_Side(&Dom, pFooting, cycle, FALSE, dan, bDim && bDimRebar, !bInput && nDimDan==dan+1 && bDim, bDim && bMarkRebar, bInput);
		}
	}
	
	// ����ܰ� ö��
	DrawRebarSideOut_Front(&Dom, pFooting, TRUE, FALSE, bInput && bDimRebar, bMarkRebar, bInput);
	DrawRebarSideOut_Front(&Dom, pFooting, FALSE, FALSE, FALSE, FALSE, bInput);

	// ����ö��
	DrawRebarShear_Front(&Dom, pFooting, FALSE, bMarkRebar, bInput && bDimRebar, TRUE, nDimDan, bInput);
	


	*pDomP << Dom;
	return rectTot;
}

// ��ü ���鿡 �׷����� ö�ٿ� ���ؼ� cuttngArea������ �Ѵ�.
// ����� tvArr�� �ǵ�����.
// nUpdn == -1�̸� ���� ��� ó��
// nUpdn = iUPPER�̸� ��鸸 ó��, nUpdn == iLOWER�̸� �ϸ鸸 ó��
// BOOL bAtLeft : �������� ���� ������ �ľ� �Ǵ���?
void CDrawFooting::ApplyCuttingArea_Plane(CDomyun *pDomP, CFootingApp *pFooting, CTwinVectorArray &tvArr, BOOL bUpper, BOOL bAtLeft, long nDan, BOOL bDrawCuttingArea, BOOL bExt, BOOL bSlope)
{
	CDomyun Dom(pDomP);
	long envType	= m_pStd->m_pARoadOptionStd->GetEnvType(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);


	// ������ �׸����� �������� ������ �۾��ϹǷ�.. ���⼭ ������.
	double dAngRot	= 0;//pFooting->m_tvPathPlan.GetXyDir().GetAngleDegree();

	// 1���� ��� 2,3���� ��� cuttingArea�� �߶󳽴�. ////////////////////////////
	// 2,3���� ��� �ش� cuttingArea�� �׸���.
	CRebarPlacing rb;
	CHgBaseDrawStd baseDraw;
	if(nDan == 0 && !bExt && !bSlope)
	{
		long dan = 0; for(dan = 1; dan < MAX_REBAR_DAN+2; dan++)
		{
			CTwinVectorArray tvArrCuttingArea;
			if(dan == MAX_REBAR_DAN)
			{
				if(bUpper)
					pFooting->GetTvCuttingArea_Plane_ExtOrSlope(tvArrCuttingArea, bAtLeft, 0);
			}
			else if(dan == MAX_REBAR_DAN+1)
			{
				if(0)//bUpper)
					pFooting->GetTvCuttingArea_Plane_ExtOrSlope(tvArrCuttingArea, bAtLeft, 0);
			}
			else
			{
				pFooting->GetTvCuttingArea_Plane(tvArrCuttingArea, bUpper, bAtLeft, dan);
			}
			rb.RotateTvArr(tvArrCuttingArea, CDPoint(0, 0), -dAngRot);
			if(tvArrCuttingArea.GetSize() > 0)
			{
				rb.GetTvArrayByCrossTvArrayArea(tvArr, tvArrCuttingArea, FALSE);
				if(bDrawCuttingArea)
				{
					m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);

					tvArrCuttingArea.RemoveAt(tvArrCuttingArea.GetIdxByInfo("�׸�������"));
					baseDraw.DrawTvArray(&Dom, tvArrCuttingArea, FALSE);
				}
			}
		}
	}
	else
	{
		CTwinVectorArray tvArrCuttingArea;
		if(bExt)
		{
			if(bUpper)
				pFooting->GetTvCuttingArea_Plane_ExtOrSlope(tvArrCuttingArea, bAtLeft, 0);
		}
		else if(bSlope)
		{
			if(0)//bUpper)
				pFooting->GetTvCuttingArea_Plane_ExtOrSlope(tvArrCuttingArea, bAtLeft, 0);
		}
		else
		{
			// 2, 3���� ���� cuttingArea�ȸ� ��� ���� �Ǵµ�.
			pFooting->GetTvCuttingArea_Plane(tvArrCuttingArea, bUpper, bAtLeft, nDan);
		}
		rb.RotateTvArr(tvArrCuttingArea, CDPoint(0, 0), -dAngRot);
		if(tvArrCuttingArea.GetSize() > 0)
		{
			rb.GetTvArrayByCrossTvArrayArea(tvArr, tvArrCuttingArea, TRUE);
			if(bDrawCuttingArea)
			{
				m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);

				tvArrCuttingArea.RemoveAt(tvArrCuttingArea.GetIdxByInfo("�׸�������"));
				baseDraw.DrawTvArray(&Dom, tvArrCuttingArea, FALSE);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////////

	*pDomP << Dom;
	m_pStd->m_pARoadOptionStd->SetEnvType(pDomP, envType);
}

// ����ö�� �׸���
void CDrawFooting::DrawRebarShear_Plane(CDomyun *pDomP, CFootingApp *pFooting, long nDrawLeft)
{
	CDomyun Dom(pDomP);
	CHgBaseDrawStd baseDraw;
	CConcSymbol sym(m_pStd->m_pARoadOptionStd);

	CTwinVectorArray tvArr;	// ö�� ��ǥ
	pFooting->GetTvRebarShear_Plane(tvArr);

	// ����ö�� ������� ����
	CRebarPlacing rb;
	rb.GetTvArrShearRebar(tvArr, FALSE, pFooting->m_rebarShearDetail.m_nType == TYPE_SHEAR_DIGUT || pFooting->m_rebarShearDetail.m_nType == TYPE_SHEAR_SPACER || pFooting->m_rebarShearDetail.m_nType == TYPE_SHEAR_SINGLE_HOOK, FALSE);

	// nDrawLeft�� ���� �ʿ��� �͸� ���
	if(nDrawLeft != 0)
	{
		CTwinVectorArray tvArrArea;
		pFooting->GetTvCuttingArea_Plane_Shear(tvArrArea, nDrawLeft);
		rb.GetTvArrayByCrossTvArrayArea(tvArr, tvArrArea, TRUE);

		// �ڸ��� Ÿ���� ���� ��� �ڸ��� ������ �׷��ش�.
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
		baseDraw.DrawTvArray(&Dom, tvArrArea);
	}

	// �׸���.
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);	
	baseDraw.DrawTvArray(&Dom, tvArr);
	
	*pDomP << Dom;
}

void CDrawFooting::DrawRebarMain_Front_Obtuse(CDomyun *pDomP, CFootingApp *pFooting, BOOL bFront, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput)
{
	if(!pFooting) return;
	if(!pFooting->m_bIs) return;

	long front	= bFront ? iFRONT : iBACK;
	if(!pFooting->m_bIsObtuse[front]) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CRebarInfoCycle *pRC	= &pFooting->m_rebarInfoCycle_Obtuse[front];

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	m_pStd->DrawRebarInfoCycle(&Dom, pRC);

	if(bDimRebar)
	{
		long nDimDan	= 1;
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		if(bInput)
		{
			Dom.SetDirection("TOP");
			CDPoint xyStt	= pRC->m_xyStt;
			CDPoint xyEnd	= pRC->m_xyMid1;
			Dom.DimMoveTo(xyStt.x, xyStt.y, nDimDan, "");
			Dom.DimLineToExtend(fabs(xyEnd.y-xyStt.y), nDimDan, "FL", COMMA(fabs(xyEnd.y-xyStt.y)));
		}
		else
		{
			CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> pArrRC;
			CRebarInfoCycle *pRCTmp	= new CRebarInfoCycle;
			*pRCTmp	= *pRC;
			pArrRC.Add(pRCTmp);

			CDPoint xyStt	= pRC->m_xyStt;
			CDPoint xyEnd	= pRC->m_xyMid1;
			m_pStd->DimRebarInfoCycleArray(&Dom, &pArrRC, xyStt, xyEnd, TRUE, nDimDan, 0, FALSE, TRUE);

			AhTPADelete(&pArrRC, (CRebarInfoCycle*)0);
		}
	}

	if(bMarkRebar)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		double dAddHeight	= pFooting->m_dCoverObtuseUpper[front];

		m_pStd->MarkRebarInfoCycle(&Dom, pRC, TRUE, FALSE, dAddHeight, 0, 0, CDRect(0, 0, 0, 0), 0.5, bInput, &pFooting->m_pArrRebarMain);
	}

	*pDomP << Dom;
}

void CDrawFooting::DrawRebarSupport_FrontObtuse(CDomyun *pDomP, CFootingApp *pFooting, BOOL bFront, BOOL bDimRebar, BOOL bMarkRebar, long nVertDir, BOOL bInput)
{
	long front = bFront ? iFRONT : iBACK;
	if(!pFooting) return;
	if(!pFooting->m_bIs) return;
	if(!pFooting->m_bIsObtuse[front]) return;
	long nType	= pFooting->GetTypeByInputValue();
	
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	// ö�� �����ͼ� �׸���
	CHgBaseDrawStd baseDraw;
	CDPointArray xyArr;
	pFooting->GetXyRebarSupport_FrontObtuse(xyArr, bFront, nVertDir);
	CDPoint xyDirScale	= CDPoint(0, -1);
	baseDraw.DrawPointRebar(&Dom, xyArr, xyDirScale);

	CPlaceRebarByBlock *pPlace	= &pFooting->m_placeByBlock_Obtuse_SupportRebar[front];
	
	// ġ�� ����
	if(bDimRebar)
	{
		if(xyArr.GetSize() > 0)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

			CDPointArray xyArrDim;
			CDPoint xyStt	= xyArr.GetAt(0);
			CStringArray sArrText, sArrUnderText;
			CString sDir	= _T("BOTTOM");
//			long nDimDan	= 0;

			// ġ���� �Ǻ����� �߰��ؾ� ��. //////////////////////
			CTwinVector tvLeft, tvRight;
			CRebarInfoCycle *pRC	= &pFooting->m_rebarInfoCycle_Obtuse[front];
			pFooting->m_tvArrFront.GetTvByInfo("����������", tvLeft);
			pFooting->m_tvArrFront.GetTvByInfo("���ʿ�����", tvRight);


			CDPoint xySttTmp	= pRC->m_xyStt;
			CDPoint xyMid1Tmp	= pRC->m_xyMid1;
			if(pFooting->m_bLeftIsFrontGup)
			{
				GetSwap(xySttTmp, xyMid1Tmp);
			}


			// ���� �߰�(���ۺ�)
			xyArrDim.Add(tvLeft.GetXyTop());
			xyArrDim.Add(xySttTmp);
			long i = 0; for(i = 0; i < xyArr.GetSize(); i++)
				xyArrDim.Add(xyArr.GetAt(i));
			xyArrDim.Add(xyMid1Tmp);
			xyArrDim.Add(tvRight.GetXyTop());

			xyArrDim.Sort(TRUE);
		
			// ġ�� ����
			if(xyArrDim.GetSize() > 1)
			{
				xyStt	= xyArrDim.GetAt(0);
				xyStt.y = min(tvLeft.GetXyBottom().y, tvRight.GetXyBottom().y);

				long nDimDan	= 0;
				baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, TRUE, FALSE, TRUE, FALSE, FALSE, FALSE);
			}
		}
	}


	// ��ŷ
	long nTypeRebar	= bFront ? F_REBAR_S_OF : F_REBAR_S_OB;;
	CString sMark	= pFooting->GetMarkRebarSupport(nTypeRebar, bInput, FALSE, -1);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		CTwinVector tvLower	= pFooting->m_tvArrFront.GetTvByInfo("�����ϸ�");

		CRebar *pRb = pFooting->GetRebarByType(&pFooting->m_pArrRebarSupport, nTypeRebar);
		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
		mark.SetRebar(xyArr);

		// ����
		mark.m_sMark					= sMark;
		mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pFooting->m_Fy);
		mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_UPPER;
		mark.m_nTypeMarkDir				= TYPE_MARKDIR_RIGHTTAN;
		mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
		mark.m_dPosScaleMarkStt			= 0.5;

		// ��ü�� ���ؾ� �ȴ�.
		// FOOTING_TYPE_BASIC �̸� ���� ������ ���ϰ�
		/// FOOTING_TYPE_BASIC_BOTH �̸� ���������� ���Ѵ�.
		if(nType == FOOTING_TYPE_BASIC || nType == FOOTING_TYPE_BASIC_BOTH)
		{
			CTwinVector tvLower				= pFooting->m_tvArrFront.GetTvByInfo("�����ϸ�");
			CTwinVector tvLeft				= pFooting->m_tvArrFront.GetTvByInfo("���ʻ������");
			CTwinVector tvRight				= pFooting->m_tvArrFront.GetTvByInfo("���ʻ�����");

			double dWLeft	= tvLeft.GetHorLength();
			double dWRight	= tvRight.GetHorLength();
			if(nType == FOOTING_TYPE_BASIC)
				mark.m_dPosScaleMarkStt		= fabs((dWLeft<dWRight ? tvRight.GetXyMid().x : tvLeft.GetXyMid().x)-tvLower.GetXyLeft().x) / tvLower.GetHorLength();
			else
				mark.m_dPosScaleMarkStt		= fabs(tvRight.GetXyMid().x-tvLower.GetXyLeft().x) / tvLower.GetHorLength();
		}

		CDPoint xyMark		= mark.GetXyMark();
		mark.m_dAddHeight	= tvLower.m_v1.y + pFooting->GetHeight() - xyMark.y + MARK_BASEHEIGHT_UPPER;
		mark.m_dAddHeight	+= REBAR_RADIUS;
		mark.m_dCtc						= pPlace->GetCTC();
		mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;

		mark.MarkRebar();
	}

	*pDomP << Dom;
}

void CDrawFooting::DrawRebarMain_Plane_Obtuse(CDomyun *pDomP, CFootingApp *pFooting, BOOL bFront, BOOL bDimRebar, BOOL bMarkRebar, BOOL bDimTotal, long nDrawUpper, BOOL bDimForTopIsRight)
{
	long front	= bFront ? iFRONT : iBACK;
	if(!pFooting) return;
	if(!pFooting->m_bIsObtuse[front]) return;

	DrawRebarMain_Plane(pDomP, pFooting, 0, 0, TRUE, bDimRebar, bMarkRebar, bDimTotal, nDrawUpper, bDimForTopIsRight, FALSE, TRUE, bFront);
}


void CDrawFooting::DrawRebarSupport_Plane_Obtuse(CDomyun *pDomP, CFootingApp *pFooting, BOOL bFront, BOOL bDimRebar, BOOL bMarkRebar, long nDrawUpper, BOOL bInput, BOOL bDimForTopIsRight)
{
	long front	= bFront ? iFRONT : iBACK;
	if(!pFooting) return;
	if(!pFooting->m_bIsObtuse[front]) return;

	DrawRebarSupport_Plane(pDomP, pFooting, 0, 0, bInput ? FALSE : bDimRebar, bMarkRebar, nDrawUpper, bInput, bDimForTopIsRight, -1, TRUE, bFront);
}

void CDrawFooting::DimRebarRange_Plane_Obtuse(CDomyun *pDomP, CFootingApp *pFooting, BOOL bFront)
{
	long front	= bFront ? iFRONT : iBACK;
	if(!pFooting) return;
	if(!pFooting->m_bIsObtuse[front]) return;
	
	CTwinVector tvPathPlane	= pFooting->GetTvPathPlane_Obtuse(bFront);
	CTwinVector tvPathOrg	= tvPathPlane;
	double dRange	= pFooting->m_dRange_Obtuse_MainRebar[front];
	if(bFront)
	{
		tvPathOrg.m_v1	= tvPathOrg.m_v1 + tvPathOrg.GetXyDir(TRUE) * dRange;
	}
	else
	{
		tvPathOrg.m_v2	= tvPathOrg.m_v2 + tvPathOrg.GetXyDir(FALSE) * dRange;
	}

	
	
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	CDPointArray xyArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CDPoint xyStt	= CDPoint(0, 0);
	CString sDir	= _T("TOP");
	long nDimDan	= 0;
	

	if(bFront)
	{
		xyStt	= tvPathOrg.m_v1;
		xyArrDim.Add(tvPathOrg.m_v1);
		xyArrDim.Add(tvPathPlane.m_v1);
	}
	else
	{
		xyStt	= tvPathPlane.m_v2;
		xyArrDim.Add(tvPathPlane.m_v2);
		xyArrDim.Add(tvPathOrg.m_v2);
	}

	sArrText.Add("FMR");
	sArrText.Add("FMR");
	sArrUnderText.Add(COMMA(dRange));
	sArrUnderText.Add(COMMA(dRange));

	CHgBaseDrawStd baseDraw;
	baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE, TRUE, TRUE);

	*pDomP << Dom;
}

CDRect CDrawFooting::DrawRebar_Plane_ObtuseStd(CDomyun *pDomP, CFootingApp *pFooting, BOOL bFront, BOOL bDimRebar, BOOL bMarkRebar, BOOL bDimTotal, long nDrawUpper, BOOL bDimForTopIsRight, long nRotate, BOOL bInput)
{
	long front	= bFront ? iFRONT : iBACK;
	if(!pFooting) return CDRect(0, 0, 0, 0);
	if(!pFooting->m_bIsObtuse[front]) return CDRect(0, 0, 0, 0);

	CDomyun Dom(pDomP);
	DrawPlan(&Dom, pFooting, TRUE, TRUE, bFront);

	DrawRebarMain_Plane_Obtuse(&Dom, pFooting, bFront, TRUE, TRUE, TRUE, nDrawUpper, bDimForTopIsRight);

	BOOL bChgDimForTopisRight = (nRotate > 0 && !bInput)? !bDimForTopIsRight : bDimForTopIsRight;
	DrawRebarSupport_Plane_Obtuse(&Dom, pFooting, bFront, TRUE, TRUE, nDrawUpper, bInput, bChgDimForTopisRight);
	DrawRebarSupport_FrontSlope(&Dom, pFooting, 0, FALSE, TRUE, bInput);

	if(bInput)
		DimRebarRange_Plane_Obtuse(&Dom, pFooting, bFront);

	// �������� ���̵��� ȸ��
	double dRotate	= nRotate == -1 ? 180-pFooting->m_tvPathPlan.GetXyDir().GetAngleDegree()-90 : -pFooting->m_tvPathPlan.GetXyDir().GetAngleDegree()+90;
	Dom.Rotate(CDPoint(0, 0), ToDPointFrDegree(dRotate));
	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();
	
	*pDomP << Dom;



	// ���� �¿����� ġ���� ������ ���������� �¿������� ���� �ش�.
	CTwinVectorArray tvArrPlane;
	tvArrPlane	= pFooting->m_tvArrplan;
	
	CRebarPlacing rb;
	rb.RotateTvArr(tvArrPlane, CDPoint(0, 0), dRotate);
	CDRect rectPl	= tvArrPlane.GetRect();
	rect.left		= rectPl.left;
	rect.right		= rectPl.right;

	return rect;
}

// ��������� ��ö��
void CDrawFooting::DrawRebarMain_FrontSlope(CDomyun *pDomP, CFootingApp *pFooting, long nDan, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput)
{
	if(!pFooting) return;
	if(!pFooting->m_bIs) return;
	if(!pFooting->IsRebarSupportFrontSlope(nDan)) return;
	long nType	= pFooting->GetTypeByInputValue();

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	
	CRebarInfoCycle *pRC	= &pFooting->m_rebarInfoCycle_Slope[nDan];
	CHgBaseDrawStd baseDraw;
	m_pStd->DrawRebarInfoCycle(&Dom, pRC, 0, CDRect(0, 0, 0, 0));
	
	BOOL bLeftSlope	= nType != FOOTING_TYPE_BASIC_RIGHTSLOPE;
	CTwinVector tvSLope	= pFooting->m_tvArrFront.GetTvByInfo(!bLeftSlope ? "���ʿ������" : "�����������");
	CDPoint xyRot	= !bLeftSlope ? tvSLope.GetXyDir() : tvSLope.GetXyDir(FALSE);
	if(bDimRebar)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		CDPoint xyStt	= !bLeftSlope ? tvSLope.m_v2 : tvSLope.m_v1;
		CString sDir	= !bLeftSlope ? _T("RIGHT") : _T("LEFT");

		// �Ǻ� ġ�� ���� �� ����
		if(!bInput && nDan == 0)
		{
			*pDomP << Dom;
			CDoubleArray dArrDim;
			CStringArray sArrText;
			CStringArray sArrUnderText;
			
			long nDimDan	= 0;
		
			dArrDim.Add(pFooting->m_dCoverUpper[0]);
			if(pFooting->m_dCoverSlope[0] > 0)
				dArrDim.Add(pFooting->m_dCoverSlope[0]-pFooting->m_dCoverUpper[0]);
			if(pFooting->m_dCoverSlope[1] > 0)
				dArrDim.Add(pFooting->m_dCoverSlope[1]-pFooting->m_dCoverSlope[0]);
			if(pFooting->m_dCoverSlope[2] > 0)
				dArrDim.Add(pFooting->m_dCoverSlope[2]-pFooting->m_dCoverSlope[1]);
			dArrDim.Add(pFooting->m_dLenLeg_Support_FrontSlope);

			CDoubleArray dArrTmp;
			long i = 0; for(i = dArrDim.GetSize()-1; i >= 0; i--)
				dArrTmp.Add(dArrDim.GetAt(i));

			dArrDim	= dArrTmp;
			
			long nCountDan	= pFooting->IsRebarSupportFrontSlope(0) ? 1 : 0;
			nCountDan	+= pFooting->IsRebarSupportFrontSlope(1) ? 1 : 0;
			nCountDan	+= pFooting->IsRebarSupportFrontSlope(2) ? 1 : 0;

			xyStt	= pFooting->m_rebarInfoCycle_Slope[nCountDan-1].m_xyMid2;
			xyStt	+= tvSLope.GetXyDir().RotateInv90() * pFooting->m_dLenLeg_Support_FrontSlope;
			baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, FALSE, FALSE);
			Dom.Rotate(xyStt, xyRot);
		}
		else
		{
			*pDomP << Dom;
			
			long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				if(pFooting->IsRebarSupportFrontSlope(dan))
				{
					xyStt	= pFooting->m_rebarInfoCycle_Slope[dan].m_xyMid2;
					xyStt	+= tvSLope.GetXyDir().RotateInv90() * pFooting->m_dLenLeg_Support_FrontSlope;
					CString str	= _T("");
					str.Format("D%d", dan+1);
					Dom.SetDirection(sDir);
					Dom.DimMoveTo(xyStt.x, xyStt.y, dan);
					Dom.DimLineToExtend(pFooting->m_dLenLeg_Support_FrontSlope, dan, "L", COMMA(pFooting->m_dLenLeg_Support_FrontSlope));
					Dom.DimLineToExtend(pFooting->m_dCoverSlope[dan], dan, str, COMMA(pFooting->m_dCoverSlope[dan]));

					Dom.Rotate(xyStt, xyRot);
					*pDomP << Dom;
				}
			}
		}
	}


	CString sDescription	= _T("");
	sDescription.Format("%s", pRC->m_sDescription);
	CString sMark	= pFooting->GetMarkRebarMain(MARK_SLOPE, bInput, 0, FALSE, FALSE, sDescription, 1);
	if(bMarkRebar)
	{
		*pDomP << Dom;
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		CString sDia	= m_pStd->m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pFooting->m_Fy);
		CPlaceRebarByBlock *pPlace	= &pFooting->m_placeByBlock_Support_FrontSlope;
		pPlace->m_nCountBlock	= 3;
		pPlace->FitCount();
		CDPoint xyR	= pPlace->m_xyArr.GetAt(0);
		double dDist	= xyR.y/2;
		if(dDist == 0)
		{
			xyR	= pPlace->m_xyArr.GetAt(1);
			dDist	= xyR.y/2;
		}
		

		CConcSymbol sym(m_pStd->m_pARoadOptionStd);
		CDPoint xy;
		CTwinVector tvMain;
		tvMain.m_v1	= pRC->m_xyMid1;
		tvMain.m_v2	= pRC->m_xyMid2;
		xy	= tvMain.m_v1 + dDist * tvMain.GetXyDir();;

		// sym.RebarMarkLineUpper(&Dom, xy, sMark, sDia, nType == FOOTING_TYPE_BASIC_LEFTSLOPE ? FALSE : TRUE, 0);

		CRebar *pRb = pFooting->GetRebarByDescription(&pFooting->m_pArrRebarMain, sDescription);
		SRebarMarkInputData mark_input_data;
		SRebarMarkviewData mark_view_data;
		mark_input_data.mark = sMark;
		mark_input_data.dia = sDia;
		mark_input_data.position = xy;
		mark_input_data.ctc = pFooting->m_dCTC_MainRebar;
		mark_input_data.type_rebar = pRb? pRb->m_nTypeRebar : -1;
		mark_input_data.left_dir = nType == FOOTING_TYPE_BASIC_LEFTSLOPE ? FALSE : TRUE;
		mark_input_data.height = 0;

		mark_view_data.count_mark = TRUE;
		mark_view_data.line_rebar = TRUE;
		mark_view_data.mark_pos = eMarkUpper;

		sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

		Dom.Rotate(xy, xyRot);
		*pDomP << Dom;
	}

	*pDomP << Dom;
}

// ��������� ���ö��
void CDrawFooting::DrawRebarSupport_FrontSlope(CDomyun *pDomP, CFootingApp *pFooting, long nDan, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput)
{
	if(!pFooting) return;
	if(!pFooting->m_bIs) return;
	if(!pFooting->IsRebarSupportFrontSlope(nDan)) return;
	long nType	= pFooting->GetTypeByInputValue();

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CDPointArray xyArr;
	CDPoint xyDirApplyScale	= pFooting->GetXyRebarSupport_Front_Slope(xyArr, nDan);
	
	CHgBaseDrawStd baseDraw;
	baseDraw.DrawPointRebar(&Dom, xyArr, xyDirApplyScale);


	CPlaceRebarByBlock *pPlace	= &pFooting->m_placeByBlock_Support_FrontSlope;
	pPlace->m_nCountBlock	= 3;
	pPlace->FitCount();
	if(bDimRebar && xyArr.GetSize() > 0)
	{
		*pDomP << Dom;
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		CDoubleArray dArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		CDPoint xyStt	= nType == FOOTING_TYPE_BASIC_RIGHTSLOPE ? xyArr.GetAt(0) : xyArr.GetAt(xyArr.GetSize()-1);
		long nDimDan	= 0;
		CString sDir	= "TOP";
		
		CDPoint xyR(0, 0);
		if(nType == FOOTING_TYPE_BASIC_RIGHTSLOPE)
		{
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
		}
		else if(nType == FOOTING_TYPE_BASIC_LEFTSLOPE)
		{
			CString str	= _T("");

			xyR	= pPlace->m_xyArr.GetAt(2);
			dArrDim.Add(xyR.x*xyR.y);
			sArrText.Add(bInput ? "C2" : COMMA(xyR.y));
			sArrUnderText.Add(COMMA(xyR.y));

			xyR	= pPlace->m_xyArr.GetAt(1);
			str.Format("%s@%s=%s", COMMA(xyR.x), COMMA(xyR.y), COMMA(xyR.x*xyR.y));
			dArrDim.Add(xyR.x*xyR.y);
			sArrText.Add(bInput ? "B@C" : str);
			sArrUnderText.Add(str);

			xyR	= pPlace->m_xyArr.GetAt(0);
			dArrDim.Add(xyR.x*xyR.y);
			sArrText.Add(bInput ? "C1" : COMMA(xyR.y));
			sArrUnderText.Add(COMMA(xyR.y));
		}


		if(!bInput)
		{
			sArrUnderText.RemoveAll();
		}

		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, FALSE, FALSE);

		Dom.Rotate(xyStt, nType == FOOTING_TYPE_BASIC_RIGHTSLOPE ? xyDirApplyScale.Rotate90() : xyDirApplyScale.RotateInv90());
	}

	long nTypeRebar	= F_REBAR_S_SL;
	CString sMark	= bInput ? "S" : pFooting->GetMarkRebarSupport(nTypeRebar, bInput, FALSE, -1);
	if(bMarkRebar)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		if(nType == FOOTING_TYPE_BASIC_LEFTSLOPE)
		{
			CRebarPlacing rb;
			rb.ReverseRebar(xyArr);
		}
			
		CRebar *pRb = pFooting->GetRebarByType(&pFooting->m_pArrRebarSupport, nTypeRebar);
		CMarkDomyun mark;
		mark.SetDomyun(&Dom, m_pStd->m_pARoadOptionStd);
		mark.SetRebar(xyArr);
		mark.m_sMark	= sMark;
		mark.m_sDia		= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pFooting->m_Fy);
		mark.m_nTypeMarkDir	= TYPE_MARKDIR_RIGHTTAN;
		mark.m_dCtc						= pPlace->GetCTC();
		mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;

		mark.MarkRebar();
	}

	*pDomP << Dom;
}

// void CDrawFooting::DrawRebarMain_Plane_Slope(CDomyun *pDomP, CFootingApp *pFooting, BOOL bMarkRebar, long nDrawUpper, BOOL bDimForTopIsRight)
// {
// 	if(!pFooting) return;
// 	if(!pFooting->IsRebarSupportFrontSlope(0)) return;
// 
// 	DrawRebarMain_Plane(pDomP, pFooting, 0, 0, TRUE, FALSE, bMarkRebar, FALSE, nDrawUpper, bDimForTopIsRight, FALSE, FALSE, TRUE, TRUE);
// }

// void CDrawFooting::DrawRebarSupport_Plane_Slope(CDomyun *pDomP, CFootingApp *pFooting, BOOL bMarkRebar, long nDrawUpper, BOOL bInput, BOOL bDimForTopIsRight)
// {
// 	if(!pFooting) return;
// 	if(!pFooting->IsRebarSupportFrontSlope(0)) return;
// 
// 	DrawRebarSupport_Plane(pDomP, pFooting, 0, TRUE, FALSE, bMarkRebar, nDrawUpper, bInput, bDimForTopIsRight, -1, FALSE, TRUE, TRUE);
// }

// ���� ��ũ��Ʈ ���� ǥ��
void CDrawFooting::MakrPlaneConcFck(CDomyun *pDom, CFootingApp *pFooting)
{
	if(!pFooting) return;
	if(!pFooting->m_bIs) return;
	CRcBridgeApp* pBri = m_pStd->m_pRcBridge;
	
	CDPoint xyMark(0, 0);
	CTwinVector tvFront	= pFooting->m_tvArrplan.GetTvByInfo("�����������");
	CTwinVector tvLeft	= pFooting->m_tvArrplan.GetTvByInfo("�����������1");
	GetXyMatchLineAndLine(tvFront.m_v1 + tvFront.GetXyDir() * tvFront.GetLength()/4*1, tvLeft.GetXyDir(), tvLeft.GetXyMid(), tvFront.GetXyDir(), xyMark);

	CDomyun Dom(pDom);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SYMB);
	
	CFootingApp *pFootingStt = pBri->GetFootingByJijumNumber(0);
	CFootingApp *pFootingEnd = pBri->GetFootingByJijumNumber(pBri->GetCountJijum()-1);
	const EPartOfBridge ePart = pFooting == pFootingStt ? ePartFootStt : pFooting == pFootingEnd ? ePartFootEnd : ePartInFoot;
	CString	str = m_pStd->GetStringTextDimFck(pFooting->m_Fck, pBri->GetValueAggregate(ePart), pBri->GetValueSlump(ePart));
	CStringArray strArr;
	strArr.Add("���� ��ũ��Ʈ");
	strArr.Add(str);

	CConcSymbol Sym;
	Sym.DrawSymbolConCArrow(&Dom, xyMark, strArr);
	CDPoint vAngLine = tvFront.GetXyDir(TRUE);
	Dom.Rotate(xyMark, ToDPointFrDegree(vAngLine.GetAngleDegree() + 30));
	*pDom << Dom;

	//double dWidth = Dom.GetExtWidth();
	// �ɺ��� �����ؼ� ��.
	//CHgBaseDrawStd::InsertHCadBlock(&Dom, "CGRECON", xyMark-dWidth/2*vAngLine, Dom.GetScaleDim()*2/3);
	//CHgBaseDrawStd::InsertHCadBlock(&Dom, "CGRECON", xyMark+dWidth/2*vAngLine, Dom.GetScaleDim()*2/3);	
	Dom.Rotate(xyMark, ToDPointFrDegree(vAngLine.GetAngleDegree() - 60));
	*pDom << Dom;
}
