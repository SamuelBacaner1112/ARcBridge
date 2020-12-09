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

// ���ܸ鵵 �׸���
// ������� �ɼǿ� ���� �׸�
void CDrawVertSection::DrawVertSection(CDomyun *pDomP, BOOL bDrawPave, CTwinVectorArray *tvArrReturn, CDRect rectLimit, BOOL bDrawConstJoint)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	CHgBaseDrawStd baseDraw;
	baseDraw.m_pLine	= pBri->GetLineBase();

	// ���ܸ� TEST
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
		if(bDrawConstJoint)	// �ð� ����(�ð� ������������ �ʿ�)
		{
			CTwinVector tvLeft, tvRight, tvU, tvL;
			CDPoint xy(0, 0), xyNum(0, 0);
			double dTS = pBri->m_dTS;
			Dom.SetTextHeight(2);

			// ���� ��ü
			if(pBri->m_tvArrVertSection.GetTvByInfo(_T("������ü������"), tvLeft))
			{
				pBri->m_tvArrVertSection.GetTvByInfo(_T("������ü������"), tvRight);
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

			// ���� ��ü
			if(pBri->m_tvArrVertSection.GetTvByInfo(_T("������ü������"), tvLeft))
			{
				pBri->m_tvArrVertSection.GetTvByInfo(_T("������ü������"), tvRight);
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

			// ������
			CString str = _T("");
			if(!pBri->m_tvArrVertSection.GetTvByInfo(_T("��������(������ü����)"), tvU))
				pBri->m_tvArrVertSection.GetTvByInfo("��������(1��°����)", tvU);

			int nSize = pBri->m_tvArrVertSection.GetSize();
			int i=0; for(i=0; i<nSize; i++)
			{
				tvL = pBri->m_tvArrVertSection.GetAt(i);
				str = tvL.m_sInfo;
				if(str.Right(10)==_T("�������ϸ�"))
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
				strLeft.Format(_T("%d��°������ġ������"), i);
				strRight.Format(_T("%d��°������ġ������"), i);
				pBri->m_tvArrVertSection.GetTvByInfo(strLeft, tvLeft);
				pBri->m_tvArrVertSection.GetTvByInfo(strRight, tvRight);

				double dHuH = max(pBri->m_dHHL[i-1][iUPPER], pBri->m_dHHR[i-1][iUPPER]);
				xyNum = (tvLeft.m_v1+tvRight.m_v2)/2 + CDPoint(0, (dHuH+dTS)/2);
				Dom.TextCircle(xyNum.x, xyNum.y, _T("4"), 3, FALSE, TRUE);
			}

			//�ڽ���
			if(pBri->IsBoxType())
			{
				CTwinVector tvStt, tvEnd, tvLowerSlab;

				pBri->m_tvArrVertSection.GetTvByInfo(_T("������ü�Ϻ���ġ"), tvStt);
				pBri->m_tvArrVertSection.GetTvByInfo(_T("������ü�Ϻ���ġ"), tvEnd);
				
				m_pStd->DrawConstJointHor(&Dom, tvStt.m_v1 - CDPoint(1,0)*pBri->m_dWS, pBri->m_dWS);
				m_pStd->DrawConstJointHor(&Dom, tvEnd.m_v2, pBri->m_dWE);

				for(i=1; i<nCountJijum-1; i++)
				{
					CWallApp *pWall = pBri->GetInWall(i-1);
					strLeft.Format(_T("%d��°�����Ϻ���ġ������"), i);
					pBri->m_tvArrVertSection.GetTvByInfo(strLeft, tvLeft);
					m_pStd->DrawConstJointHor(&Dom, tvLeft.m_v2, pWall->m_dW);
				}

				pBri->m_tvArrVertSection.GetTvByInfo(_T("�Ϻν������ϸ�(������ü����)"), tvLowerSlab);
				Dom.TextCircle(tvLowerSlab.GetXyMid().x, tvLowerSlab.GetXyMid().y + pBri->m_dTS_Lower/2, _T("1"), 3, FALSE, TRUE);
			}

			// ��ġ����
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
			{
				// ���긦 �� ������� ������ �¿��� 1/4�� "2"������ ����
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

				// ���� �� �ֱ�
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

		//BOX���϶��� ���Ʒ� ���� Ŀ�� ���� �׷������.
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

// ���ܸ鵵 ��� ġ��
void CDrawVertSection::DimVertSectionUpper(CDomyun *pDomP, long nDimDan, BOOL bStt, BOOL bInput, BOOL bOnlyJijum, BOOL bSttEndMark)
{
	// ����ġ�� ���� �Լ�.
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
	
	pTvArr->GetTvByInfo("��������ϻ��", tv);
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
			
		pTvArr->GetTvByInfo("��������(������ü��)", tv);
		dArrDim.Add(tv.GetHorLength(bApplyAngle));
		sArrText.Add(COMMA(tv.GetHorLength(bApplyAngle)));
		sArrUnderText.Add(bApplyAngle ? COMMA2(tv.GetHorLength(FALSE)) : "0");

		dOldX = tv.m_v2.x;
	}
	else
	{
		pTvArr->GetTvByInfo("��������(������ü����)", tv);
		xyStt.x	= tv.m_v1.x;
		dOldX	= xyStt.x;
	}

	xyStt.y = max(pBri->GetXySectionSttEnd(TRUE).y, pBri->GetXySectionSttEnd(FALSE).y);
	xySttTot.x = tv.m_v1.x;

	// ������ü 
	if(!pBri->IsOutWall(TRUE)) 
	{
		if(pTvArr->GetTvByInfo("��������������", tv))
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
				if(pBri->m_fxJointStt.m_tvArrSection.GetTvByInfo("���������������", tvExp))
					dOldX	= tvExp.GetXyRight().x;
		
			}
			if(pWall->m_tvArrFront.GetTvByInfo("��ü������", tv))
			{
				double dLength	= tv.m_v2.x-dOldX;
				CDPoint vAng	= pBri->GetAngleJijum(i);
 				dArrDim.Add(dLength);
				sArrText.Add(COMMA(dLength));
				sArrUnderText.Add(bApplyAngle && pBri->IsEnableDisplayVertLengthOfJigan() ? COMMA2(dLength*vAng.y) : "0");
				dOldX = tv.m_v2.x;
			}

			if(pWall->m_tvArrFront.GetTvByInfo("��ü������", tv))
			{
				double dLength	= tv.m_v2.x-dOldX;
				CDPoint vAng	= pBri->GetAngleJijum(i+1);
 				dArrDim.Add(dLength);
				sArrText.Add(COMMA(dLength));
				sArrUnderText.Add(bApplyAngle && pBri->IsEnableDisplayVertLengthOfJigan() ? COMMA2(dLength*vAng.y) : "0");
				dOldX = tv.m_v2.x;
			}
		}
		// �������� ��� ��ü �߽��� �����..(�� ������ �����)
		else
		{
			double dSta	= pBri->GetStationOnJijum(i+1, 0, !bApplyAngle);

			if(pBri->m_fxJointStt.m_tvArrSection.GetTvByInfo("���������������", tv) && i == 0)
			{
				if(tv.GetHorLength() > 0)
				{
					dOldX	= tv.GetXyRight().x;
				}
			}

			CString sHunch[2];
			sHunch[iLEFT].Format("%d��°������ġ������", i+1);
			sHunch[iRIGHT].Format("%d��°������ġ������", i+1);
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

	if(pTvArr->GetTvByInfo("��������(������ü����)", tv))
	{
		double dLength = tv.m_v2.x-dOldX;
		CDPoint vAng = pBri->GetAngleJijum(0);
		dArrDim.Add(dLength);
//		sArrText.Add("0");
//		sArrUnderText.Add("0");
		sArrText.Add(COMMA(dLength));
		sArrUnderText.Add(bApplyAngle && pBri->IsEnableDisplayVertLengthOfJigan() ? COMMA2(dLength*vAng.y) : "0");
	}
	else if(pTvArr->GetTvByInfo("��������������", tv))
	{
		double dLength	= tv.m_v2.x - dOldX;
		CDPoint vAng	= pBri->GetAngleJijum(pBri->GetCountJijum()-1);
		dArrDim.Add(dLength);
		sArrText.Add(COMMA(dLength));
		sArrUnderText.Add(bApplyAngle && pBri->IsEnableDisplayVertLengthOfJigan() ? COMMA2(dLength*vAng.y) : "0");
	}
	
	if(!bOnlyJijum)
	{
		if(pTvArr->GetTvByInfo("��������(������ü��)", tv))
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
					str.Format("%d��°������ġ�����������ϸ�", pBri->m_nQtyJigan-1);
				}
				else
				{
					str	= "��������(1��°����)";
				}
				if(pTvArr->GetTvByInfo(str, tv))
				{
					if(pBri->m_nQtyJigan > 1)
					{
						CTwinVector tvTmp;
						str	= "��������������";
						pTvArr->GetTvByInfo(str, tvTmp);
						tv.m_v1	= tvTmp.m_v2;
					}

					CTwinVector tvExp;
					if(0)//pBri->m_fxJointEnd.m_tvArrSection.GetTvByInfo("���������������", tvExp))
					{
						dArrDim.Add(tv.GetHorLength(bApplyAngle) - tvExp.GetHorLength(bApplyAngle));
						sArrText.Add(COMMA(tv.GetHorLength(bApplyAngle) - tvExp.GetHorLength(bApplyAngle)));
						sArrUnderText.Add(bApplyAngle && pBri->IsEnableDisplayVertLengthOfJigan() ? COMMA2(tv.GetHorLength(FALSE) - tvExp.GetHorLength(FALSE)) : "0");
					}
				}
			}
		}

		xyEndTot.x = tv.m_v2.x;

		if(pTvArr->GetTvByInfo("��������ϻ��", tv))
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
			pTvArr->GetTvByInfo("��������������", tv);
			dArrDim.Add(tv.m_v2.x-dOldX);
			sArrText.Add("0");
			sArrUnderText.Add("0");
		}

		xyEndTot.x	= tv.m_v2.x;
	}

	// ������ ���� üũ(��ü�� ���°�� ġ������ ��ġ�� �ٽ� ��������� ��.
	if(!pBri->IsOutWall(FALSE))
	{
		if(pTvArr->GetTvByInfo("��������������", tv))
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
		if(!pTvArr->GetTvByInfo("��������(������ü����)", tv))
			pTvArr->GetTvByInfo("��������(1��°����)", tv);
		
		dArrDim.Add(tv.GetHorLength(bApplyAngle));
		sArrText.Add("0");
		sArrUnderText.Add("0");
	}

	// ���̰� 0�ΰ� �ϴ� ����
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

	// ���������� ���ؼ� ���⼭ ���Ƿ� �־��ش�.
	if(pBri->IsBlockOut(TRUE) && dArrDim.GetSize() > 1)
	{
		pBri->m_fxJointStt.m_tvArrSection.GetTvByInfo("���������������", tv);
		
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
		pBri->m_fxJointEnd.m_tvArrSection.GetTvByInfo("���������������", tv);
		
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

	// ��� ������ ��� ��ȣ�� ġ���� �ǹ̰� ����. //////////
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
		sMsg = sMsg + "(�������� ��Ÿ�)";
		sArrText.SetAt(sArrText.GetSize()-1, sMsg);
		*/
		dArrDim.Add(0.0001);
		sArrText.Add("(�������� ��Ÿ�)");
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
		
		// ������� ġ��(����� ����)
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
		���� ����� ������ ���̸� �������̷� �� ����� ������ ���� ġ�� 
		1. ��ü ġ���� ���� ��ü�߰��� �����ϰ�
		2. ������ ���̴� ������ �ϳ��� �����Ѵ�.
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
	if(!pTvArr->GetTvByInfo("��������ϻ������", tvStt))
	{
		if(!pTvArr->GetTvByInfo("��������������", tvStt))
			return;
	}

	CTwinVector tvEnd;
	if(!pTvArr->GetTvByInfo("��������ϻ������", tvEnd))
	{
		if(!pTvArr->GetTvByInfo("��������������", tvEnd))
			return;
	}


	CDPoint xyStt	= tvStt.GetXyTop();
	CDPoint xyEnd	= tvEnd.GetXyTop();
	double dLenBridge	= pBri->GetLengthSlabReal(0);

	Dom.DimMoveTo(xyStt.x, xyStt.y, 0);
	Dom.DimLineTo(xyEnd.x - xyStt.x, 0, COMMA(dLenBridge));
	

	*pDom << Dom;
}


// ���ܸ鵵 ������ ġ��
// ������ü ������ ġ�� �� ����.. (���������� ���� ��)
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

	// ��ü�� ������ ������ �β� ġ���� ����
	if(!pBri->IsOutWall(TRUE))
	{
		CDPointArray xyArrDim;
		CTwinVector tv;
		CTwinVector tvLower, tvHunch;
		pTvArr->GetTvByInfo("��������������", tv);

		xyArrDim.Add(tv.m_v1);
		xyArrDim.Add(tv.m_v2);
		xyStt	= tv.m_v1;

		if(pTvArr->GetTvByInfo("������ü��ġ", tvHunch))
		{
			xyArrDim.Add(tvHunch.GetXyTop());
		}

		xyArrDim.Sort(FALSE);

		if(pBri->IsBlockOut(TRUE) && pBri->m_fxJointStt.m_tvArrSection.GetTvByInfo("�������������ϸ�", tv))
		{
			xyArrDim.Add(tv.GetXyLeft());
			xyArrDim.Sort(FALSE);

			// �������� �ִ� ��� �� ���� �ִ� ġ���� ���� ������ ġ���� ����
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
				tv	= pBri->m_fxJointStt.m_tvArrSection.GetTvByInfo("������������������");
				sArrUnderText.InsertAt(sArrUnderText.GetSize()-1, COMMA(tv.GetVerLength()));
			}
		}

		CHgBaseDrawStd baseDraw;
		baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE);

		*pDomP << Dom;

		pTvArr->GetTvByInfo("������ġ�ϸ�", tvLower);
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

	// ġ������ ������ġ�� ���ʰ� ���� ��� ������ Ȯ����ʸ� �������� �Ѵ�. 
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
		pBri->m_footingStt.m_tvArrFront.GetTvByInfo("����������", tv);
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

	pTvArr->GetTvByInfo("������ü������", tv);
	dArrDim.Add(tv.GetVerLength());
	sArrText.Add("0");
	sArrUnderText.Add("0");

	pTvArr->GetTvByInfo("�����������ġ", tv);
	dH = tv.GetVerLength();
	if(!bInput)
	{
		dArrDim.Add(dH);
		sArrText.Add("0");
		sArrUnderText.Add(COMMA(dH));
	}
	pTvArr->GetTvByInfo("�������������", tv);
	if(bInput)
		dH += tv.GetVerLength();
	else
		dH = tv.GetVerLength();
	dArrDim.Add(dH);
	sArrText.Add("BH");
	sArrUnderText.Add(COMMA(dH));

	pTvArr->GetTvByInfo("��������ϻ������", tv);
	dArrDim.Add(tv.GetVerLength());
	sArrText.Add("BTH");
	sArrUnderText.Add(COMMA(tv.GetVerLength()));

	// ���� �β�(���� ����ô� ���� ����)
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

	// ��ü ġ��	
	{
		// �������� ġ��
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

// ���ܸ鵵 ������ ġ��
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

	// ��ü�� ������ ������ �β� ġ���� ����
	if(!pBri->IsOutWall(FALSE))
	{
		CDPointArray xyArrDim;
		CTwinVector tv, tvHunch, tvLower;
		pTvArr->GetTvByInfo("��������������", tv);

		xyArrDim.Add(tv.m_v1);
		xyArrDim.Add(tv.m_v2);
		xyStt	= tv.m_v2;

		if(pTvArr->GetTvByInfo("������ü��ġ", tvHunch))
		{
			xyArrDim.Add(tvHunch.GetXyTop());
		}
		xyArrDim.Sort(FALSE);

		if(pBri->IsBlockOut(FALSE) && pBri->m_fxJointEnd.m_tvArrSection.GetTvByInfo("�������������ϸ�", tv))
		{
			xyArrDim.Add(tv.GetXyRight());
			xyArrDim.Sort(FALSE);

			// �������� �ִ� ��� �� ���� �ִ� ġ���� ���� ������ ġ���� ����
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
				tv	= pBri->m_fxJointEnd.m_tvArrSection.GetTvByInfo("������������������");
				sArrUnderText.InsertAt(sArrUnderText.GetSize()-1, COMMA(tv.GetVerLength()));
			}
		}


		CHgBaseDrawStd baseDraw;
		baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE);

		*pDomP << Dom;
		pTvArr->GetTvByInfo("������ġ�ϸ�", tvLower);

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

	// ġ������ ������ġ�� ���ʰ� ���� ��� ������ Ȯ����ʸ� �������� �Ѵ�.
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
		pFooting->m_tvArrFront.GetTvByInfo("���ʿ�����", tv);
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

	pTvArr->GetTvByInfo("������ü������", tv);
	dArrDim.Add(tv.GetVerLength());
	sArrText.Add("0");
	sArrUnderText.Add("0");

	pTvArr->GetTvByInfo("�����������ġ", tv);
	dH = tv.GetVerLength();
	if(!bInput)
	{
		dArrDim.Add(dH);
		sArrText.Add("0");
		sArrUnderText.Add(COMMA(dH));
	}
	pTvArr->GetTvByInfo("�������������", tv);
	if(bInput)
		dH += tv.GetVerLength();
	else
		dH = tv.GetVerLength();
	dArrDim.Add(dH);
	sArrText.Add("BH");
	sArrUnderText.Add(COMMA(dH));

	pTvArr->GetTvByInfo("��������ϻ������", tv);
	dArrDim.Add(tv.GetVerLength());
	sArrText.Add("BTH");
	sArrUnderText.Add(COMMA(tv.GetVerLength()));

	// ���� �β�
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

	// ��ü ġ��
	{
		// �������� ġ��
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

// EL ����
// bOnlyJijum : ������ �� ������ġ�� ����
// ����β� ��� �ؼ� ����β���ŭ ���� �����ؾ� ���������� ����.
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

	// �� ������ EL
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
		// �� ���� ��ü �� ��� EL
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

//�λ��������
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

	//��ü ������
	xyStt.x	= pLine->GetStationByOffsetWidth(dStaJijum, -pBri->m_dWS, vAngJijum);
	xyStt.y	= pBri->m_dELSB;

	// �λ�������� ����
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

// ���ܸ� ���� ���
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
// 	CString str = bStt ? "����" : "����";
// 	pTvArr->GetTvByInfo(str+"�����������", tv);
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
// 	pTvArr->GetTvByInfo(str+"���ʿ������", tv);
// 	dArrDim.Add(tv.GetHorLength());
// 	sArrText.Add("FR");
// 	sArrUnderText.Add(COMMA(tv.GetHorLength()));
// 	
// 	if(bStt && pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)
// 	{
// 		pTvArr->GetTvByInfo("���ʻ��", tv);
// 		dArrDim.Add(tv.GetHorLength());
// 		sArrText.Add("0");
// 		sArrUnderText.Add("0");
// 
// 		dArrDim.Add(pBri->m_dWE);
// 		sArrText.Add("0");
// 		sArrUnderText.Add("0");
// 
// 		pTvArr->GetTvByInfo("�������ʿ������", tv);
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

// ������ü ��ġ
void CDrawVertSection::DimVerSectionHunch(CDomyun *pDomP, long nDimDan, BOOL bStt, BOOL bInput, BOOL bUpper)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;
	if(bUpper == FALSE && pBri->m_nTypeBridge != BRIDGE_TYPE_BOX) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDomP, &Dom);
	
	if(nDimDan < 0) nDimDan = 0;
	CString sStt = bStt ? "����" : "����";
	CString sHunch = bUpper? "��ü��ġ" : "��ü�Ϻ���ġ";
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
		// ��� ������ ��� ��ȣ�� ġ���� �ǹ̰� ����. //////////
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

	// ��米2 �� ��� ��ü�Ϻ���ġ�� �ִ�.
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 || pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		CString sStt = bStt ? "����" : "����";
		CTwinVectorArray *pTvArr = &pBri->m_tvArrVertSection;
		CTwinVector tv;
		CString sText = _T("");
		CString sUnderText = _T("");
			

		if(pTvArr->GetTvByInfo(sStt+"��ü�Ϻ���ġ", tv))
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
			// ��� ������ ��� ��ȣ�� ġ���� �ǹ̰� ����. //////////
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

// ���ܼ��� �׸���
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

	// ��������Ÿ ���
	// ���ݼ� �������� �ִ°͸� ����Ѵ�.
	// ������ ���� ����(���ݼ� �������� �ִ°͸� ����Ѵ�.)�̾�����
	// �ָ� ������ �־ ��꿡�� �������ȴ�.
	// ���� �ָ� ������ �־ ��������Ҷ��� ����� �Ǿ�� �Ѵ�.
	//������ draw(tab��)
	if(bDrawBoring)
	{
		// ������������ ���� �����ֻ󵵸� ���������� ��ø�ؼ� �׷����°� ���´�.
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
				
			//Station�� ���ͷ� �Էµǰ� �и����ͷ� ó���ȴ�. EL�� ���ͷ� �Էµǰ� ó���ǹǷ� ��ȯ���ش�.
			Dom.Move(dSta, frM(pBoringEx->m_EL));				
			*pDomP << Dom;
		}	
	}
}

// ������ü ����� ��ġ ġ�� ����
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
	CString str = bStt ? "����" : "����";

	BOOL bApplyAngle	= pBri->IsApplyAngle();
	pTvArr->GetTvByInfo(str+"�������ġ", tv);
	
	// ����
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

	// ��� ������ ��� ��ȣ�� ġ���� �ǹ̰� ����. //////////
	m_pStd->ModifyInfoAutoDimConc(sArrText, sArrUnderText, bInput);
	///////////////////////////////////////////////////////////

	CHgBaseDrawStd baseDraw;
	baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE, bStt);

	// ����
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
	str.Format("%d��°����%s��ġ", nIdxHunch+1, bUpper? _T("") : _T("�Ϻ�"));

	long nIdxInWall		= 0;
	BOOL bLeft			= TRUE;
	pBri->GetInfoHunch(nIdxHunch, nIdxInWall, bLeft);

	// ����ġ��
	if(bUpper)
	{
		pTvArr->GetTvByInfo(str+"�Ʒ�������", tvLeft);
		pTvArr->GetTvByInfo(str+"�Ʒ������", tvRight);
	}
	else
	{
		pTvArr->GetTvByInfo(str+"��������", tvLeft);
		pTvArr->GetTvByInfo(str+"�������", tvRight);
	}

	BOOL bApplyAngle	= tvLeft.m_bApplyAngle;
	double rad			= fabs(sin(ToRadian(pBri->GetAngleJijum(nIdxInWall+1).GetAngleDegree()-180)));
	if(!bApplyAngle || Compare(rad, 0.0, "=")) 
		rad	= 1;

	// �������϶� ���������� ����ؼ� �������� ����� ��ġ�� ���̷� ġ���� �����Ѵ�.
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

		pTvArr->GetTvByInfo(str+"�Ʒ�������", tvLeft);
		pTvArr->GetTvByInfo(str+"�Ʒ������", tvRight);
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
			// �Ʒ��� ���� ġ��
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

			// �Ʒ��� ���� ġ��
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

		// ��� ������ ��� ��ȣ�� ġ���� �ǹ̰� ����. //////////
		m_pStd->ModifyInfoAutoDimConc(sArrText, sArrUnderText, bInput);
		///////////////////////////////////////////////////////////

		CHgBaseDrawStd baseDraw;
		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, TRUE);
		*pDomP << Dom;
	}


	// ������ġ ���ü�
	CString sText = _T("");
	CString sUnderText = _T("");
	CString sTextR = _T("");
	CString sUnderTextR = _T("");
	if(pBri->m_dHLW[nIdxHunch][bUpper? iUPPER:iLOWER] != 0 || pBri->m_dHHL[nIdxHunch][bUpper? iUPPER:iLOWER] != 0)
	{
		pTvArr->GetTvByInfo(str+"������", tv);
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

		// ��� ������ ��� ��ȣ�� ġ���� �ǹ̰� ����. //////////
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


			// ����
			if(pBri->m_nTypeRoundOfHunchInWall[nIdxInWall][iLEFT] != 0)
			{
				Dom.SetDirection("BOTTOM");
				Dom.DimMoveTo(tv.GetXyLeft().x, tv.GetXyBottom().y, 0);
				if(sUnderText == "")
					Dom.DimLineTo(tv.GetHorLength(bApplyAngle), 0, sText);
				else
					Dom.DimLineToExtend(tv.GetHorLength(bApplyAngle), 0, sText, sUnderText);

				// ����
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

	// ������ġ ���ü�
	if(pBri->m_dHRW[nIdxHunch][bUpper? iUPPER:iLOWER] != 0 || pBri->m_dHHR[nIdxHunch][bUpper? iUPPER:iLOWER] != 0)
	{
		pTvArr->GetTvByInfo(str+"������", tv);
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
			// �ϴ� ��ġ�� �������� R�� �Է��Ҽ� ������ �ϴ���ġ ��� ���ص� ��.
			sTextR	= bInput ? "HRR" : "R = " + COMMA(pBri->m_dHRR[nIdxHunch][iUPPER]);
			sUnderTextR	= bInput ? "R = " + COMMA(pBri->m_dHRR[nIdxHunch][iUPPER]) : "0";
		}

		// ��� ������ ��� ��ȣ�� ġ���� �ǹ̰� ����. //////////
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

			// ����
			if(pBri->m_nTypeRoundOfHunchInWall[nIdxInWall][iRIGHT] != 0)
			{
				Dom.SetDirection("BOTTOM");
				Dom.DimMoveTo(tv.GetXyLeft().x, tv.GetXyBottom().y, 0);
				if(sUnderText == "")
					Dom.DimLineTo(tv.GetHorLength(bApplyAngle), 0, sText);
				else
					Dom.DimLineToExtend(tv.GetHorLength(bApplyAngle), 0, sText, sUnderText);

				// ����
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

	// ��ġ�Ʒ������� ġ��
	if(pBri->m_dHBH[nIdxHunch][bUpper? iUPPER:iLOWER] != 0)
	{
		str.Format("%d��°����%s������", nIdxHunch+1, bUpper? _T("��ġ�Ʒ�") : _T("�Ϻ���ġ��"));
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

	// ���������� ���� ġ������ 
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB)
	{
		CString sLeftSide	= _T("");
		CString sRightSide	= _T("");
		CTwinVector tvLeftSide, tvRightSide;

		sLeftSide.Format("%d��°������ġ������������", nIdxHunch+1);
		sRightSide.Format("%d��°������ġ������������", nIdxHunch+1);

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

// ��� ������ ��ö�� �׸���
// long nVertDir : �̹� ö���� ����� ����� �����Ƿ� �Ǻ� ġ�� ���Խÿ��� ��� �ϸ� ��
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

	// ��� ������
	// ��� ///////////////////////////////////////////////////////////////////////
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &pBri->m_pArrRCMainUpperSlab[nCycle][iUPPER][nDan];
	// RebarInfoCycle �׸���
	m_pStd->DrawRebarInfoCycleArray(&Dom, pArrRC, nDrawLeft, rectLimit, !bInput);

	// ���� �׸���
	if(bDrawJoint)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);

		// ���� ���� ������
		CJointArray arrJoint;
		pBri->MakeJointMainRebarUpperSlab(arrJoint, nCycle, nDan, TRUE);
		
		// ���� �׸���
		m_pStd->DrawJointArray(&Dom, arrJoint, nDrawLeft, rectLimit);
	}

	// ġ������
	if(bDimRebar)
	{
		DimRebarMain_VertSection_UpperSlab_Upper_Cycle(&Dom, bMarkRebar ? 1 : 0, TRUE, nCycle, nVertDir, nDan);
	}

	// ö�� ��ũ
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



	// �ϸ� /////////////////////////////////////////////////////////////////////
	pArrRC = &pBri->m_pArrRCMainUpperSlab[nCycle][iLOWER][nDan];

	// RebarInfoCycle �׸���
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	m_pStd->DrawRebarInfoCycleArray(&Dom, pArrRC, nDrawLeft, rectLimit);

	// ���� �׸���
	if(bDrawJoint)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);

		// ���� ���� ������
		CJointArray arrJoint;
		pBri->MakeJointMainRebarUpperSlab(arrJoint, nCycle, nDan, FALSE);
		
		// ���� �׸���
		m_pStd->DrawJointArray(&Dom, arrJoint, nDrawLeft, rectLimit);	
	}

	// ġ������
	if(bDimRebar)
		DimRebarMain_VertSection_UpperSlab_Lower_Cycle1(&Dom, bMarkRebar ? 1 : 0, TRUE, nCycle, nVertDir, nDan);

	// ö�� ��ũ
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

// ���� ��ö�� �׸���
// long nVertDir : �̹� ö���� ����� ����� �����Ƿ� �Ǻ� ġ�� ���Խÿ��� ��� �ϸ� ��
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

	// ����
	for(long upper = 0; upper < 2; upper++)
	{
		BOOL bUpper	= upper == iUPPER;

		CParabola pb;
		double dCover	= bUpper ? -pBri->m_dCoverRib[upper][nDan] : pBri->m_dCoverRib[upper][nDan];
		pBri->m_pbRib[upper].GetOffsetParabola(pb, dCover);

		CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &pBri->m_pArrRCMainRib[nCycle][upper][nDan];
		// RebarInfoCycle �׸���
		m_pStd->DrawRebarInfoCycleArray(&Dom, pArrRC, nDrawLeft, rectLimit, TRUE, &pb);

		// ���� �׸���
		if(bDrawJoint)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);

			// ���� ���� ������
			CJointArray arrJoint;
			pBri->MakeJointMainRebarRib(arrJoint, nCycle, nDan, bUpper);
			
			// ���� �׸���
			m_pStd->DrawJointArray(&Dom, arrJoint, nDrawLeft, rectLimit);
		}

		// ġ������
		if(bDimRebar)
		{
			DimRebarMain_VertSection_ArchRib(&Dom, nCycle, bUpper, nDan, 0, bInput, nVertDir);
		}

		// ö�� ��ũ
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

// BOOL bForStandardSection : ǥ�شܸ鵵������?(���ʿ� ���� ��ü ö�� R ǥ�� ����)
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

	// ������ ��ü
	BOOL	bStt	= TRUE,
			bInner	= TRUE;
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		bStt	= (stt == iSTT);
		for(long inner = 0; inner < 2; inner++)
		{
			bInner	= (inner == iINNER);
			CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &pBri->m_pArrRCMainOutWall[nCycle][stt][inner][nDan];

			// RebarInfoCycle �׸���
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
			m_pStd->DrawRebarInfoCycleArray(&Dom, pArrRC, nDrawLeft, rectLimit, !bInput);

			// ���� �׸���
			if(bDrawJoint)
			{
				m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);

				// ���� ���� ������
				CJointArray arrJoint;
				pBri->MakeJointMainRebarOutWall(arrJoint, nCycle, nDan, bStt, bInner);
				m_pStd->DrawJointArray(&Dom, arrJoint, nDrawLeft, rectLimit);
			}

			// ö�� ��ũ
			if(bMarkRebar)
			{
				*pDomP << Dom;
				// ��ũ����(��������)
				BOOL bLeft	= (inner==iINNER) ? FALSE : TRUE;
				if(stt == iEND) bLeft = !bLeft;	// ���� ����.
				double dScalePos	= 0.5;
				switch(nDan)
				{
					case 0: dScalePos	= 0.5; break;
					case 1: dScalePos	= 0.2; break;
					case 2: dScalePos	= 0.8; break;
				}
				m_pStd->MarkRebarInfoCycleArray(&Dom, pArrRC, bLeft, TRUE, (inner==iINNER) ? pBri->m_dCoverWallOut[nDan] : pBri->m_dCoverWallIn[nDan], 2, 0, nDrawLeft, rectLimit, dScalePos, bInput, &pBri->m_pArrRebarMain);

				// ���ʿ� ���� ö���� R�� ������ Rǥ�� ���ش�
				if(pArrRC->GetSize() > 0 && !bForStandardSection)
				{
					CRebarInfoCycle *pRC	= pArrRC->GetAt(0);
					m_pStd->MarkRadiusRebarInfoCycle(&Dom, pRC, rectLimit);
				}
			}
		}

		// ġ�� ����
		if(bDimRebar)
			DimRebarMain_VertSection_OutWall_Cycle(&Dom, 1, TRUE, stt==iSTT, nCycle, nVertDir, nDan);
	}


	*pDomP << Dom;
}

// ��ν����� 1��, 1,2����Ŭ, ��� ��ö�� ġ��
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


	// ġ�����Կ� ����
	CDPointArray xyArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("TOP");
	CDPoint xyStt(0, 0);

	// ġ������ �м��ؼ� ������
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

	// ���ܻ��� ó��
	// 1. ����������ü�� ��ν����갡 ����� ������ ��� ù��° ö���� ���� �ȵ� ���·� �����ؼ� ġ�� �����Ѵ�.
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
		// ������ ������ �Է¿� �°� ��ȣ�� ����
		if(bInput)
		{
			// ���� ���� ����
			long nType	= 0;
			InsertJijumInfo(xyArrDim, sArrText, nType, 0, TRUE, bVertDir);

			// �Է������� ����
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


	// ġ�������� ���ؼ� �����ߴ��� ����.
	if(pArrRC->GetSize() > 0 && pRCFirstUpperSlab)
	{
		*pRCFirstUpperSlab = rcFirstUpperSlab;
	}

	*pDomP << Dom;
}

// ��ν����� 1��, 1����Ŭ, ��� ��ö�� ġ��
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


	// ġ�����Կ� ����
	CDPointArray xyArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("BOTTOM");
	CDPoint xyStt(0, 0);

	// ġ������ �м��ؼ� ������
	BOOL bReverse = FALSE;
	CDPoint vAngStt			= bVertDir ? CDPoint(0, 1) : pBri->GetAngleJijum(0);
	CDPoint vAngEnd			= bVertDir ? CDPoint(0, 1) : pBri->GetAngleJijum(pBri->m_nQtyJigan);
	double dCoverSideStt	= pBri->GetLengthBynVertDir(pBri->m_dCoverWallOut[0] / vAngStt.y, nVertDir);
	double dCoverSideEnd	= pBri->GetLengthBynVertDir(pBri->m_dCoverWallOut[0] / vAngEnd.y, nVertDir);
	m_pStd->GetDimInfoAtRebarInfoCycle(&pBri->m_pArrRCMainUpperSlab[nCycle][iLOWER][nDan], TRUE, xyArrDim, sArrText, FALSE, TRUE, TRUE, bReverse, dCoverSideStt, dCoverSideEnd, nVertDir);

		
	if(xyArrDim.GetSize() > 0)
	{
		// 2����Ŭ�� 4����Ŭ�� ���۰� ���� ��ǥ�� �ϳ� �� �־� ��� �ȴ�.
		// ��? ������̶� ġ���������� ���۰� ����ġ�� ���������� ��������...
		if(0)//nCycle == 1 || nCycle == 3)
		{
			// �Ǻ� ���� ������ 
			xyArrDim.RemoveAt(0);
			xyArrDim.RemoveAt(xyArrDim.GetSize()-1);
			sArrText.RemoveAt(0);
			sArrText.RemoveAt(sArrText.GetSize()-1);


			CTwinVector tvSlab	= pBri->GetTvOffset_UpperSlab(0, FALSE, -1, -1, bVertDir);

			// ������ġ
			xyArrDim.InsertAt(0, tvSlab.m_v1);
			sArrText.InsertAt(0, "0");
			sArrText.SetAt(1, "1");

			// ����ġ
			xyArrDim.Add(tvSlab.m_v2);
			sArrText.Add("1");

			// �Ǻ� ���� �߰��Ѵ�.
			// ó���� ���κп� �Ǻ� ���� �߰��Ѵ�. ////////////////
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


		// ������ ������ �Է¿� �°� ��ȣ�� ����
		if(bInput)
		{
			// ���� ���� ����
			long nType	= 0;
			InsertJijumInfo(xyArrDim, sArrText, nType, nCycle, FALSE, bVertDir);

			// �Է������� ����
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


	// ���۰� �� ö���� ���������� ���� ġ�� ���� ��.
		// ������ �������� ������ ������ �������� mid���� �������� �� ���
	// ������ ������ mid���� �������� �� ��찡 �ִ�. �̰� ������ mid�� ���� 
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

// ��, ���� ������ü ġ�� ����
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

	// ġ�����Կ� ����
	CDPointArray xyArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("");
	CDPoint xyStt(0, 0);

	// ����
	sDir = bStt ? _T("LEFT") : _T("RIGHT");
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &pBri->m_pArrRCMainOutWall[nCycle][stt][iOUTTER][nDan];

	if(pArrRC)
	{
		// ��ü ���� ġ�����Խ� ��ν������� ù��° ö���� �����ͼ� �ӽ÷� �ٿ� �ش�.
		// (����:ġ�����Խ� ������ü�� ��ν����갡 ����Ǳ� ������...)
		CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRCSlab	= &pBri->m_pArrRCMainUpperSlab[nCycle][iUPPER][nDan];

		if(pArrRCSlab)
		{
			long nSizeSlab	= pArrRCSlab->GetSize();
			if(nSizeSlab > 0)
			{
				CRebarInfoCycle	*pRC = new CRebarInfoCycle;
				*pRC = *pArrRCSlab->GetAt(bStt ? 0 : nSizeSlab-1);

				// ������ü�� ��ν����갡 ����� ������ ��� �ΰ��� �ɰ��� ����Ѵ�.
				if(0)//pRC->m_bUseEndExt)
				{
					if(bStt)
					{
						if(pBri->IsBoxType() == FALSE)
						{
							// �����ϸ���� ������ü����.
							CRebarInfoCycle *pRC1	= new CRebarInfoCycle;
							*pRC1	= *pRC;
							pRC1->m_bUseCenExt	= FALSE;
							pRC1->m_bUseEndExt	= FALSE;
							pRC1->m_bUseMidExt	= FALSE;
							pRC1->m_nPosEnd		= REBARINFO_POS_STTWALL_OUTTER;
							pArrRC->Add(pRC1);
						}
					
						// ������ü���� ��ν��������.
						
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
							// �����ϸ���� ������ü����.
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

						// ������ü���� ��ν��������.
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
					
					// ��ν����� R�� ���̴� �����Ǿ�� �Ѵ�.
					// ��Ÿ��� ���Ÿ��� ���̰� ��ȭ�ǰ� ����� ���� ���⼭ ��������
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
						// �Ϻ� �����꿡�� ��ü����
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

		// ġ������ �м��ؼ� ������
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
			// ������ ������ �Է¿� �°� ��ȣ�� ����
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

	// ����
	sDir = bStt ? _T("RIGHT") : _T("LEFT");
	pArrRC = &pBri->m_pArrRCMainOutWall[nCycle][stt][iINNER][nDan];

	if(pArrRC)
	{
		if(pArrRC->GetSize() > 0)
		{
			// ġ������ �м��ؼ� ������
			CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;
			double dCoverStt	= pFooting->m_dCoverLower[0];
			if(pBri->IsBoxType()) dCoverStt	= pBri->m_dCoverLower_LowerSlab[0];
			double dCoverEnd	= pBri->m_dCoverUpper_UpperSlab[0];
			BOOL bApplyEndIs2	= TRUE;//(nCycle == 0 || nCycle == 2) ? TRUE : FALSE;
			BOOL bApplyJoint	= bApplyEndIs2;
			BOOL bReverse		= !bApplyEndIs2;

			m_pStd->GetDimInfoAtRebarInfoCycle(pArrRC, TRUE, xyArrDim, sArrText, FALSE, bApplyJoint, bApplyEndIs2, bReverse, 0, 0);

			// ���� 2, 4 ����Ŭ�� �����ϸ�� ������ ��� ��ǥ�� �߰������ �ȴ�.
			if(0)//nCycle == 1 || nCycle == 3)
			{
				CTwinVector tvWall	= pBri->GetTvOffset_OutWall(0, bStt, TRUE, bVertDir);

				// ������ġ �Ǻ�
				xyArrDim.InsertAt(0, CDPoint(tvWall.m_v1.x, tvWall.m_v1.y - dCoverStt));
				sArrText.InsertAt(0, "0");

				// ������ġ
				xyArrDim.InsertAt(1, tvWall.m_v1);
				sArrText.InsertAt(1, "0");

				sArrText.SetAt(2, "1");

				// ����ġ
				xyArrDim.Add(tvWall.m_v2);
				sArrText.Add("1");

				// ����ġ �Ǻ�
				xyArrDim.Add(CDPoint(tvWall.m_v1.x, tvWall.m_v2.y+dCoverEnd));
				sArrText.Add("0");
			}
			
			if(xyArrDim.GetSize() > 0)
			{
				// ������ ������ �Է¿� �°� ��ȣ�� ����
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

// rebarinfocycle�� �м��� ġ������ ������ ���������� �����Ѵ�.(������ �ɸ��� ġ���� �ѷ� �ɰ�)
// nType : ���Թ�� 
// nType == 0 : �ѷ� �ɰ����� �տ����� ������ "2"�� ����
// nType == 1 : 1. �� ������ ����� ���� ��� �ɰ��� ����(�������Ծ���) - nCycle bUpper ������ ������ ����
//              2. �ɰ����� �տ����� "2"�� ������ ���� "1" �״�� ��. 

// nCycle : nType == 1 �϶� ���� �� ����Ŭ ��ȣ, nType == 0�϶��� �ƹ� �ҿ� ����.
// bUpper : nType == 1 �϶� ���� �� ����Ŭ�� ������� �ϸ�����?
void CDrawVertSection::InsertJijumInfo(CDPointArray &xyArrDim, CStringArray &sArrText, long nType, long nCycle, BOOL bUpper, BOOL bVertDir)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	// ���� ������ �迭�� ��� ���� ////////////
	// �� ó���� ������ ������ ���ߵȴ�.
	CDoubleArray dArrJijum;
	long i = 0; for(i = 1; i < pBri->m_nQtyJigan; i++)
	{
		dArrJijum.Add(pBri->GetStationOnJijum(i, 0, bVertDir));
	}
	// �� ������ �������� ö���� �������� ������ ��.
	for(long jijum = 0; jijum < dArrJijum.GetSize(); jijum++)
	{
		double dJijum = dArrJijum.GetAt(jijum);

		// ������ �������� ö���� �ִ��� �˻�.
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
	// ���� ������ �迭�� ��� ���� ////////////
}


// ��ŷ�� ���� ���� �Ʒ��� ���� �Ǵ��ϴ� �Լ�.
// BOOL CDrawVertSection::IsUpperMarkRebar(CRebarInfoCycle *pRC)
// {
// 	BOOL bUpper		= TRUE;
// 	long nPosStt	= pRC->m_nPosStt;
// 	long nPosEnd	= pRC->m_nPosEnd;
// 	// �����̳� ���� ��� ������ ����̸� ������ ����
// 	if(nPosStt == REBARINFO_POS_UPPERSLAB_UPPER || nPosEnd == REBARINFO_POS_UPPERSLAB_LOWER)
// 	{
// 		bUpper	= TRUE;
// 	}
// 	// �����̳� ���� ��� ������ �ϸ��̸� ������ �Ʒ���
// 	else if(nPosStt == REBARINFO_POS_UPPERSLAB_LOWER || nPosEnd == REBARINFO_POS_UPPERSLAB_LOWER)
// 	{
// 		bUpper	= FALSE;
// 	}
// 	// �����̳� ���� ������ ��ü �������̸� ������ ����
// 	else if(nPosStt == REBARINFO_POS_STTWALL_OUTTER || nPosEnd == REBARINFO_POS_STTWALL_OUTTER)
// 	{
// 		bUpper	= TRUE;
// 	}
// 	// �����̳� ���� ������ ��ü �������̸� ������ �Ʒ���.
// 	else if(nPosStt == REBARINFO_POS_STTWALL_INNER || nPosEnd == REBARINFO_POS_STTWALL_INNER)
// 	{
// 		bUpper	= FALSE;
// 	}
// 	// �����̳� ���� ������ ��ü �������̸� ������ �Ʒ���
// 	else if(nPosStt == REBARINFO_POS_ENDWALL_OUTTER || nPosEnd == REBARINFO_POS_ENDWALL_OUTTER)
// 	{
// 		bUpper	= FALSE;
// 	}
// 	// �����̳� ���� ������ ��ü �������̸� ������ ����.
// 	else if(nPosStt == REBARINFO_POS_ENDWALL_INNER || nPosEnd == REBARINFO_POS_ENDWALL_INNER)
// 	{
// 		bUpper	= TRUE;
// 	}
// 
// 	return bUpper;
// }

// ��ν����� ��ġ�� ��ö�� �׸���
void CDrawVertSection::DrawRebarMain_VertSection_UpperSlabHunch(CDomyun *pDomP, long nCycle, BOOL bMarkRebar, long nDrawLeft, CDRect rectLimit, BOOL bInput)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	if(nCycle == -1) nCycle	= pBri->GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > pBri->GetCountCycleMainRebar()-1) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	// ��� ������
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &pBri->m_pArrRCMainUpperSlabHunch[nCycle];
	if(!pArrRC) return;

	// RebarInfoCycle �׸���
	m_pStd->DrawRebarInfoCycleArray(&Dom, pArrRC, nDrawLeft, rectLimit);

	if(bMarkRebar)
	{
		m_pStd->MarkRebarInfoCycleArray(&Dom, pArrRC, FALSE, FALSE, pBri->m_dCoverLower_UpperSlab[0], 2, 0, nDrawLeft, CDRect(0, 0, 0, 0), 0.9, bInput, &pBri->m_pArrRebarMain);
	}


	*pDomP << Dom;
}

// ���ܸ鵵 ��� �׸���.
// BOOL bPatternRoundRectWall : ���� ��ü ���� �׸���?
// BOOL bApplyAngle : ���� �׸��� ���� �����ϴ���?
CDRect CDrawVertSection::DrawVertSectionAll(CDomyun *pDomP, BOOL bDrawExFooting, BOOL bDrawPile, BOOL bDrawPave, BOOL bDrawConstJoint, BOOL bDrawCuttingPile, BOOL bPatternRoundRectWall, BOOL bApplyAngle, BOOL bFlexibleJoint)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return CDRect(0, 0, 0, 0);

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	// ���ܸ� �׸�
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

// ���ܸ� ��ö�� ��� �׸�'
// CRebar *pRB : ��ȣ�� �ʻ��..
// pRB �� NULL�� ��� �Ϲ������� �׳� �׸���.
// �׷���!!! pRB�� �����ϴ� ���, �̹� ���谡 �� ��Ȳ�̶� �Ǵ��ϸ�..
// pRB�� RebarInfoCycle�� �̹� ��ȣ��  match�Ǿ� �ִ�...
// �׷��Ƿ� pRB�� RebarInfoCycle �� ��ȣ�� ���ؼ� �����Ÿ� �׸���.
// �̰Ŵ� ö�� ���� â���� �ش� ö�ٸ� �������ؼ� �׷��ش�.
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
		
		// ������ ����
		for(long upper = 0; upper < 2; upper++)
		{
			m_pStd->m_pDrawFooting->DrawRebarMain_Front(&Dom, &pBri->m_footingStt, nCycle, dan, upper, bDimRebar, bMarkRebar, nDrawLeft, bInput);
			m_pStd->m_pDrawFooting->DrawRebarMain_Front(&Dom, &pBri->m_footingEnd, nCycle, dan, upper, bDimRebar, bMarkRebar, nDrawLeft, bInput);
		}
		m_pStd->m_pDrawFooting->DrawRebarMain_FrontSlope(&Dom, &pBri->m_footingStt, dan, bDimRebar, bMarkRebar&&dan==0, bInput);
		m_pStd->m_pDrawFooting->DrawRebarMain_FrontSlope(&Dom, &pBri->m_footingEnd, dan, bDimRebar, bMarkRebar&&dan==0, bInput);

		// ��ü�� ö�� �׸���.(��ü�� ���� ����)
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

	// ��ġ�ο� �ߵ��ϰ� �׷��� ����ö���� ���⼭ �׷���..
	for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		DrawRebarShear_VertSection_UpperSlabDanbu(&Dom, nCycle, bStt, !bForStandardSection && bMarkRebar, FALSE, FALSE);
		DrawRebarShear_VertSection_LowerSlabDanbu(&Dom, nCycle, bStt, !bForStandardSection && bMarkRebar, FALSE, FALSE);
	}

	*pDomP << Dom;
}

// ���ܸ鵵 ��� ������ ���ö�� 
// long nVertDir=0 : �ڵ�(��������), -1 : ���Ÿ�, 1 : ��Ÿ�
// BOOL bCalSupRebar : �ٽ� �׸��⸸ ��.. ��ǥ �ٽ� ������ ����
void CDrawVertSection::DrawRebarSupport_VertSection_UpperSlab(CDomyun *pDomP, BOOL bUpper, long nDan, BOOL bMarkRebar, BOOL bDimRebar, long nDimDan, BOOL bInput, CDRect rectLimit, long nVertDir, BOOL bCalSupRebar)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	// ö�� �����ͼ� �׸���
	CHgBaseDrawStd baseDraw;
	CDPointArray xyArr;
	pBri->GetXyRebarSupport_VertSection_UpperSlab(xyArr, bUpper, nDan, nVertDir, bCalSupRebar);

	// rectLimit ����
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
	// ġ�� ����
	if(bDimRebar)
	{
		if(xyArr.GetSize() > 0)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

			CDPointArray xyArrDim;
			CDPoint xyStt	= xyArr.GetAt(0);
			CStringArray sArrText, sArrUnderText;
			CString sDir	= bUpper ? "TOP" : "BOTTOM";

			// ġ���� �Ǻ��� R���� �߰��ؾ� ��. //////////////////////
			CTwinVector tvUpperStt, tvUpperEnd;
			CRebarInfoCycle *pRCStt	= NULL,
							*pRCEnd	= NULL;
			
			// ���� �߰�(���ۺ�)
			if(pBri->m_tvArrVertSection.GetTvByInfo("��������ϻ������", tvUpperStt))
			{
				xyArrDim.Add(bUpper ? tvUpperStt.m_v2 : tvUpperStt.m_v1);	sArrText.Add("0"); sArrUnderText.Add("0");
				xyStt.y	= bUpper ? tvUpperStt.GetXyTop().y : tvUpperStt.GetXyBottom().y;
			}
			else if(pBri->m_tvArrVertSection.GetTvByInfo("��������������", tvUpperStt))
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
						
			// ġ�� ���� ���� ������
			CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_SupportRebar_UpperSlab[upper][nDan];
			m_pStd->GetDimInfoAtPlaceRebarByBlock(xyArr, xyArrDim, sArrText, sArrUnderText, pPlace, bUpper ? "U" : "L", xyStt, CDPoint(1, 0), bInput, TRUE);

			// ���� �߰�(����)
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
			if(pBri->m_tvArrVertSection.GetTvByInfo("��������ϻ������", tvUpperEnd))
			{
				xyArrDim.Add(bUpper ? tvUpperEnd.m_v1 : tvUpperEnd.m_v2); sArrText.Add("0"); sArrUnderText.Add("0");
				xyStt.y	= bUpper ? max(tvUpperEnd.GetXyTop().y, xyStt.y) : min(tvUpperEnd.GetXyBottom().y, xyStt.y);
			}
			else if(pBri->m_tvArrVertSection.GetTvByInfo("��������������", tvUpperEnd))
			{
				xyArrDim.Add(bUpper ? tvUpperEnd.m_v1 : tvUpperEnd.m_v2); sArrText.Add("0"); sArrUnderText.Add("0");
				xyStt.y	= bUpper ? max(tvUpperEnd.GetXyTop().y, xyStt.y) : min(tvUpperEnd.GetXyBottom().y, xyStt.y);
			}

			if(!bInput)
			{
				//sArrText.RemoveAll();
				sArrUnderText.RemoveAll();
			}

			// ġ�� ����
			if(xyArrDim.GetSize() > 1)
			{
				xyStt.x	= xyArrDim.GetAt(0).x;
				baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, bUpper ? nDimDan : nDimDan+1, sDir, -1, bUpper, FALSE, FALSE);
			}
		}
	}


	// ��ŷ
	CString sDescription	= _T("");
	if(bUpper)
		sDescription.Format("��ν����� ��� %d�� ���ö��", nDan+1);	
	else
		sDescription.Format("��ν����� �ϸ� %d�� ���ö��2", nDan+1);
	CString sMark	= pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescription);


	//ARCBRIDGE-2583 ��ν����� ����� ���� ö���� �̰ݰŸ��� ������ �ش� ������ ��ŷ�� ���ֱ� ����
	CString sMarkStt = _T(""), sMarkEnd = _T("");
	CString sDescriptionStt = _T(""), sDescriptionEnd = _T("");

	for (long nStt = 0 ; nStt <= iEND; nStt++)
	{
		CString sUL = bUpper? _T("���") : _T("�ϸ�");
		if(nStt == iSTT)
		{
			if(bUpper)	sDescriptionStt.Format("��ν����� %s %d�� ���ö��%d", sUL, nDan+1, nStt+2);
			else			sDescriptionStt.Format("��ν����� %s %d�� ���ö��2_%d", sUL, nDan+1, nStt+2);
			sMarkStt = pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescriptionStt);
		}
		else
		{
			if(bUpper)	sDescriptionEnd.Format("��ν����� %s %d�� ���ö��%d",sUL, nDan+1, nStt+2);
			else			sDescriptionEnd.Format("��ν����� %s %d�� ���ö��2_%d", sUL, nDan+1, nStt+2);
			sMarkEnd = pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescriptionEnd);
		}
	}
	
	//Ȯ��or������ ��� �ش� ��� ö���� ��ã���� ����
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

		CString sUpdn = bUpper? _T("���") : _T("�ϸ�");

		if(sMark == _T("") )
		{
			for (long ang = 1 ; ang < 5; ang++)
			{
				if(ang == 1 && nSizeStt_And == 0) continue;
				else if(ang == 2 && nSizeStt_Or == 0) continue;
				else if(ang == 3 && nSizeEnd_And == 0) continue;
				else if(ang == 4 && nSizeEnd_Or == 0) continue;
					
				if(bUpper)
					sDescription.Format("��ν����� %s %d�� ���ö��%d", sUpdn, nDan+1, 5+ang);
				else
					sDescription.Format("��ν����� %s %d�� ���ö��2_%d", sUpdn, nDan+1, 5+ang);
				sMark	= pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescription);

				if(sMark == _T("")) continue;
				else break;
			}
		}
		if(sMarkStt == _T(""))
		{
			long nIndexStt = nSizeStt_Or > 0? 2 : 3;
			sDescriptionStt.Format("��ν����� ��� %d�� ���ö��%d", nDan+1, nIndexStt+5);
			sMarkStt = pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescriptionStt);
		}
		if(sMarkEnd == _T(""))
		{
			long nIndexEnd = nSizeEnd_And > 0? 3 : 4;
			sDescriptionEnd.Format("��ν����� ��� %d�� ���ö��%d", nDan+1, nIndexEnd+5);
			sMarkEnd = pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescriptionEnd);
		}

	}


	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		// �Է��� 1�ܸ� �޴´�
		CPlaceRebarByBlock *pPlace		= &pBri->m_placeByBlock_SupportRebar_UpperSlab[upper][nDan];
		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);

		//#0028210
		{
			CRebarPlacing rp;

			//ǥ�� �ܸ� ��� - ���ö���Է¿� �Էµ� ��ǥ�� �̰ݰŸ����� �ִ밪�� ����.
			// ��� ���� �Է��� 1�ܿ����� �ޱ� ������ �ش���� pPlace�� ������� �ʰ� 1���� �Է� ������ ����ؼ� �ִ� �Ÿ��� ���Ѵ�.
			CDPointArray xySupp;
			xySupp = pBri->m_placeByBlock_SupportRebar_UpperSlab[upper][0].m_xyArr;

			xySupp.Sort(FALSE, FALSE);

			double dMaxDist = (xySupp.GetSize() == 0) ? 0 : xySupp.GetAt(0).y;

			//��ġ�� ctc���� ū �̰ݰŸ��� ������ �Ǹ� ��ŷ�� ���� ���ش�. ��ŷ�� ����� �������δ�.
			CTypedPtrArray<CObArray, CDPointArray*> xyDArr;

			BOOL bExistSttAdd = FALSE;
			BOOL bExistEndAdd = FALSE;
			CDPointArray *pXyArr = NULL;

			if(xyArr.GetSize() > 0) pXyArr = new CDPointArray;
			double dLenAll = xyArr.GetSize() > 0 ? rp.GetDistPointToPoint(xyArr[0], xyArr[xyArr.GetSize() - 1]) : 0;

			for(long ix = 0; ix < xyArr.GetSize() - 1; ix ++)
			{
				double dLen = rp.GetDistPointToPoint(xyArr[ix], xyArr[ix+1]);

				if(Compare(dLen, dMaxDist+1 , ">", 0.1) ) //�ִ밪���� ū���� ������ ��ŷ�� �ϳ��� �Ѵ�.
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

// ���ܸ鵵 ������ü ���ö��
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
	// ö�� �����ͼ� �׸���
	CHgBaseDrawStd baseDraw;
	CDPointArray xyArr;
	pBri->GetXyRebarSupport_VertSection_OutWall(xyArr, bStt, bInner, nDan, nVertDir);
	CDPoint xyDirScale	= (bStt && !bInner) || (!bStt && bInner) ? CDPoint(-1, 0) : CDPoint(1, 0);

	// rectLimit ����
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
	// ġ�� ����
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

			// ġ���� �Ǻ��� R���� �߰��ؾ� ��. //////////////////////
			CTwinVector tvLower, tvUpper;
			CRebarInfoCycle *pRCLower	= NULL;
			CRebarInfoCycle *pRCUpper	= NULL;

			if(pBri->IsBoxType())
			{
				CTwinVector tvWall = pBri->GetTvOffset_OutWall(0, bStt, bInner, bVertDir);
				pBri->m_tvArrVertSection.GetTvByInfo(bStt? _T("�Ϻν������ϸ�(������ü�Ʒ�)") : _T("�Ϻν������ϸ�(������ü�Ʒ�)"), tvLower);
				CDPoint xyDimStt  = pBri->GetXyMatchLineAndLowerSlab(tvWall.m_v1, tvWall.GetXyDir(), 0, 0, bVertDir);
				xyArrDim.Add(xyDimStt); sArrText.Add("0"); sArrUnderText.Add("0");

				pRCLower	= pBri->GetRebarInfoCycleLowerSlab(0, 0, FALSE, bStt ? 0 : -1);
				if(!pRCLower || pRCLower->m_nPosStt == pRCLower->m_nPosEnd)
					pRCLower	= pBri->GetRebarInfoCycleOutWall(0, 0, bStt, bInner, 0);
			}
			else
			{
				CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;

				// ���� �߰�(���ۺ�)
				if(pFooting->m_bIs)
				{
					if(pFooting->m_tvArrFront.GetTvByInfo("�����ϸ�", tvLower))
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

			// ġ�� ���� ���� ������
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

			// ���� �߰�(����) - ��� ������ ù��° ö��
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


			if(pBri->m_tvArrVertSection.GetTvByInfo(bStt ? "��������ϻ������" : "��������ϻ������", tvUpper))
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
				sDan.Format("%s(%d��)", bInner ? "����" : "����", nDan+1);
				sArrUnderText.SetAt(nSize-1, sDan);
			}

			// ġ�� ����
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
					// ����ǥ�� �� ������ ����ǥ ����
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


	// ��ŷ
	CString sDescription	= _T("");
	sDescription.Format("%s %s %d�� ���ö��", bStt ? "��������ü" : "��������ü", bInner ? "����" : "����", nDan+1);
	CString sMark			= pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescription);
	if(sMark == "")
	{
		sDescription.Format("%s %s %d�� ���ö��2", bStt ? "��������ü" : "��������ü", bInner ? "����" : "����", nDan+1);
		sMark			= pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescription);
	}
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		CRebar *pRb = pBri->GetRebarSupportByDescription(sDescription);
		CPlaceRebarByBlock *pPlace		= &pBri->m_placeByBlock_SupportRebar_OutWall[stt][inner][nDan];
		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
		mark.SetRebar(xyArr);

		// ����
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

// ���ܸ鵵 ��ν����� ����ö��
void CDrawVertSection::DrawRebarShear_VertSection_UpperSlab(CDomyun *pDomP, BOOL bMarkRebar, BOOL bDimRebar, BOOL bInput, CDRect rectLimit, long nVertDir)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);

	BOOL bVertDir	= pBri->IsVertDir();
	if(nVertDir != 0)
		bVertDir	= nVertDir == -1 ? TRUE : FALSE;

	// ö�� �����ͼ� �׸���
	CTwinVectorArray tvArr, tvArrHunch, tvArrRib;
	pBri->GetTvRebarShear_VertSection_UpperSlab(tvArr, tvArrHunch, tvArrRib, nVertDir);

	// rectLimit ����
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

	// ���� ö�� �׸���
	m_pStd->DrawTvArrayWidthDashDot(&Dom, tvArr, FALSE);
	m_pStd->DrawTvArrayWidthDashDot(&Dom, tvArrHunch, FALSE);
	m_pStd->DrawTvArrayWidthDashDot(&Dom, tvArrRib, FALSE);

	// ��ũ�� ����δ� 2 ��ġ�δ� 1 �ƴѰŴ� 0
	for(long hunch = 0; hunch < 3; hunch++)
	{
		CTwinVectorArray *pTvArr;//	= bHunch ? &tvArrHunch : &tvArr;
		CString sDescription	= _T("");
		switch(hunch)
		{
		case 0 : 
			pTvArr	= &tvArr; 
			sDescription	= "��ν����� ����ö��1";
			break;
		case 1 : 
			pTvArr	= &tvArrHunch; 
			sDescription	= "��ν����� ����ö��2";
			break;
		case 2 : 
			pTvArr	= &tvArrRib; 
			sDescription	= "��ν����� ����ö��3";
			break;
		}

		
		CString sMark	= pBri->GetMarkRebarShear(0, bInput, FALSE, FALSE, sDescription);
		if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
		{
			double dTS	= pBri->m_dTS;
			// ö����������� �ݴ�� �Ǿ�� ��.
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
			// ����
			mark.m_sMark	= sMark;
			mark.m_sDia		= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(ePartUpperSlab, TRUE));
			mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_POS;	// ���� ǥ����ġ ã�� ���
			mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;	// TYPE_MARKPOS_POS�϶� �ɼ�
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
		// ġ�� �����Ҷ��� ��ġ�ο� �ƴѰŸ� ���ľ� �ȴ�.
		// ���ļ� ���ĵ� �ؾ� ����?(��
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

			// ġ���� �Ǻ��� R���� �߰��ؾ� ��. //////////////////////
			CTwinVector tvStt, tvEnd;
						
			xyStt.y = tvArrTot.GetXyTop().y;
			// ���� �߰�(���ۺ�)
			if(pBri->m_tvArrVertSection.GetTvByInfo("��������ϻ������", tvStt))
			{
				xyArrDim.Add(tvStt.m_v2); sArrText.Add("0"); sArrUnderText.Add("0");
				xyStt	= xyArrDim.GetAt(0);
				xyStt.y = tvArrTot.GetXyTop().y;
			}
			else if(pBri->m_tvArrVertSection.GetTvByInfo("��������������", tvStt))
			{
				xyArrDim.Add(tvStt.m_v2); sArrText.Add("0"); sArrUnderText.Add("0");
				xyStt	= xyArrDim.GetAt(0);
				xyStt.y = tvArrTot.GetXyTop().y;
			}
			
			// ġ�� ���� ���� ������
			CRebarPlacing rb;
			CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_ShearRebar_UpperSlab;
			m_pStd->GetDimInfoAtPlaceRebarByBlock(xyArr, xyArrDim, sArrText, sArrUnderText, pPlace, "V", xyStt, CDPoint(1, 0), bInput, TRUE);

			// ���� �߰�(����)
			if(pBri->m_tvArrVertSection.GetTvByInfo("��������ϻ������", tvEnd))
			{
				xyArrDim.Add(tvEnd.m_v1); sArrText.Add("0"); sArrUnderText.Add("0");
			}
			else if(pBri->m_tvArrVertSection.GetTvByInfo("��������������", tvEnd))
			{
				xyArrDim.Add(tvEnd.m_v1); sArrText.Add("0"); sArrUnderText.Add("0");
			}


			// ġ�� ����
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

// long nVertDir : -1 : ���Ÿ�, 0 : �ڵ�(��������), 1 : ��Ÿ�
void CDrawVertSection::DrawRebarShear_VertSection_OutWall(CDomyun *pDomP, BOOL bStt, long nDimDan, BOOL bMarkRebar, BOOL bDimRebar, BOOL bDimTotal, BOOL bInput, long nVertDir)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);

	BOOL bVertDir	= pBri->IsVertDir();

	// ö�� �����ͼ� �׸���
	CTwinVectorArray tvArr;
	pBri->GetTvRebarShear_VertSection_OutWall(tvArr, bStt, nVertDir);

	// ����ö�� �׸���
	m_pStd->DrawTvArrayWidthDashDot(&Dom, tvArr, FALSE);

	CString sDescription	= _T("");
	sDescription.Format("%s ����ö��", bStt ? "��������ü" : "��������ü");
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
		// ����
		mark.m_sMark	= sMark;
		mark.m_sDia		= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(bStt? ePartOutWallStt:ePartOutWallEnd, TRUE));
		mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_POS;
		mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
		mark.m_nTypeMarkDir				= bStt ? TYPE_MARKDIR_RIGHT : TYPE_MARKDIR_LEFT;
		mark.m_dAddHeight				= bStt ? pBri->m_dWS/2 : pBri->m_dWE/2;
		mark.m_dPosScaleMarkStt			= 0.5;
		// ������ü ��ö�ٰ� ���ϱ� ����
		// ��ö�� ù��° �����ִ� ���� ��ũ�Ѵ�.(��ö�� ��ũ�� ������ġ�� �� Ȯ���� ���� 0�� ������ ���� �� 100����Ŭ �Ǹ� ������ ��.. ����)
		CRebarInfoCycle *pRC	= pBri->GetRebarInfoCycleOutWall(0, 0, bStt, TRUE, 0);
		CRebarInfoCycle *pRCNext	= pBri->GetRebarInfoCycleOutWall(0, 0, bStt, TRUE, 1);
		if(pRC)
		{
			double dLenJoint	= pRCNext ? pBri->GetValueJoint(pRC, pRCNext->m_Dia) : 0;
			CDRect rect	= tvArr.GetRect();
			mark.m_dPosScaleMarkStt			= fabs((pRC->m_xyEnd.y - dLenJoint/2) - rect.bottom) / fabs(rect.Height());

			// Ȥ�ó� ������ ��� ��ũ��ġ�� �߰��� �Ѿ�� ������ �׳� ������ ������ ��ũ�Ѵ�.
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

			// ġ���� �Ǻ��� R���� �߰��ؾ� ��. //////////////////////
			CTwinVector tvWall		= pBri->GetTvOffset_OutWall(0, bStt, FALSE, bVertDir);
			CTwinVector tvLower;
			if(pBri->IsBoxType())
			{
				pBri->m_tvArrVertSection.GetTvByInfo(bStt? _T("�Ϻν������ϸ�(������ü�Ʒ�)") : _T("�Ϻν������ϸ�(������ü�Ʒ�)"), tvLower);
				CDPoint xyDimStt  = pBri->GetXyMatchLineAndLowerSlab(tvWall.m_v1, tvWall.GetXyDir(), 0, 0, bVertDir);
				tvLower.m_v1 = tvLower.m_v2 = xyDimStt;
				xyArrDim.Add(xyDimStt); sArrText.Add("0"); sArrUnderText.Add("0");
			}
			else
			{
				CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;

				// ���� �߰�(���ۺ�)
				if(pFooting->m_bIs && pFooting->m_tvArrFront.GetTvByInfo("�����ϸ�", tvLower))
				{
					xyArrDim.Add(CDPoint(tvWall.m_v1.x, tvWall.m_v1.y-pFooting->m_dCoverLower[0]));
					sArrText.Add("0"); sArrUnderText.Add("0");
				}
			}
			
			// ġ�� ���� ���� ������
			CRebarPlacing rb;
			CDPointArray xyArr;
			long i = 0; for(i = 0; i < nSize; i++)
				xyArr.Add(tvArr.GetAt(i).m_v1);
			
			CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_ShearRebar_OutWall[bStt ? iSTT : iEND];
			m_pStd->GetDimInfoAtPlaceRebarByBlock(xyArr, xyArrDim, sArrText, sArrUnderText, pPlace, "V", xyArrDim.GetAt(0), CDPoint(0, 1), bInput, TRUE);

			// ���� �߰�(����)
			xyArrDim.Add(CDPoint(tvWall.m_v2.x, tvWall.m_v2.y+pBri->m_dCoverUpper_UpperSlab[0]));
			sArrText.Add("0"); sArrUnderText.Add("0");
			
			// ġ�� ����
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

// ���ܸ鵵 ��齽���� ��ġ�� ö��
// long nCycle : #0031622�� ���� �߰��� �μ�
void CDrawVertSection::DrawRebarShear_VertSection_UpperSlabDanbu(CDomyun *pDomP, long nCycle, BOOL bStt, BOOL bMarkRebar, 
	BOOL bDimRebar, BOOL bInput)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);

	CString strSlab = _T("");
	if(pBri->IsBoxType()) strSlab = _T("��ν�����");

	// ö�� �����ͼ� �׸���
	CTwinVectorArray tvArr;
	pBri->GetTvRebarShear_VertSection_UpperSlabDanbu(tvArr, bStt, nCycle);

	// ����ö�� �׸���
	m_pStd->DrawTvArrayWidthDashDot(&Dom, tvArr, FALSE);

	long i = 0; for(i = 0; i < tvArr.GetSize(); i++)
	{
		CString sDescription	= _T("");
		sDescription.Format("%s %s �찢�� ���� ����ö��%d", strSlab, bStt ? "����" : "����", i+1);
		CString sMark	= pBri->GetMarkRebarShear(0, bInput, FALSE, FALSE, sDescription);
		if(bMarkRebar && m_pStd->IsMarkRebar(sMark) && tvArr.GetSize() > 0)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
			CConcSymbol sym(m_pStd->m_pARoadOptionStd);
			CTwinVector tv	= tvArr.GetAt(i);
			long nRate	= i+1;//i == 0 ? 1 : 4;
			CDPoint xy	= tv.m_v1 + tv.GetXyDir() * tv.GetLength()/(tvArr.GetSize()+1)*nRate;
			sym.RebarCountMarkOne(&Dom, xy, tv.GetXyDir());

			// ��
			CDPoint xy1	= tvArr.GetAt(0).GetXyMid();
			CDPoint xy2	= tvArr.GetAt(tvArr.GetSize()-1).GetXyMid();
			if(!bStt)
				GetSwap(xy1, xy2);

			double dLen	= ~(xy1-xy2);
			CDPoint xyDir	= (xy2-xy1).Unit();
			xy1	= xy;
			xy2	= xy1 + xyDir * dLen;
			
			CString sWall	= bStt ? "������ü������" : "������ü������";
			
			if(pBri->IsOutWall(bStt))
			{
				CTwinVector tv	= pBri->GetTvOffset_OutWall(0, bStt, FALSE, 0);
				GetXyMatchLineAndLine(tv.m_v1, tv.GetXyDir(), xy1, (xy1-xy2).Unit(), xy2);
			}
			
			Dom.LineTo(xy1, xy2);

			// ��ŷ
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

// ���ܸ鵵 �ϸ齽���� ��ġ�� ö��
void CDrawVertSection::DrawRebarShear_VertSection_LowerSlabDanbu(CDomyun *pDomP, long nCycle, BOOL bStt, BOOL bMarkRebar, 
	BOOL bDimRebar, BOOL bInput)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(!pBri->IsBoxType()) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);

	// ö�� �����ͼ� �׸���
	CTwinVectorArray tvArr;
	pBri->GetTvRebarShear_VertSection_LowerSlabDanbu(tvArr, bStt, nCycle);

	// ����ö�� �׸���
	m_pStd->DrawTvArrayWidthDashDot(&Dom, tvArr, FALSE);

	long i = 0; for(i = 0; i < tvArr.GetSize(); i++)
	{
		CString sDescription	= _T("");
		sDescription.Format("�Ϻν����� %s �찢�� ���� ����ö��%d", bStt ? "����" : "����", i+1);
		CString sMark	= pBri->GetMarkRebarShear(0, bInput, FALSE, FALSE, sDescription);
		if(bMarkRebar && m_pStd->IsMarkRebar(sMark) && tvArr.GetSize() > 0)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
			CConcSymbol sym(m_pStd->m_pARoadOptionStd);
			CTwinVector tv	= tvArr.GetAt(i);
			long nRate	= i+1;//i == 0 ? 1 : 4;
			CDPoint xy	= tv.m_v1 + tv.GetXyDir() * tv.GetLength()/(tvArr.GetSize()+1)*nRate;
			sym.RebarCountMarkOne(&Dom, xy, tv.GetXyDir());

			// ��
			CDPoint xy1	= tvArr.GetAt(0).GetXyMid();
			CDPoint xy2	= tvArr.GetAt(tvArr.GetSize()-1).GetXyMid();
			if(!bStt)
				GetSwap(xy1, xy2);

			double dLen	= ~(xy1-xy2);
			CDPoint xyDir	= (xy2-xy1).Unit();
			xy1	= xy;
			xy2	= xy1 + xyDir * dLen;

			CString sWall	= bStt ? "������ü������" : "������ü������";

			if(pBri->IsOutWall(bStt))
			{
				CTwinVector tv	= pBri->GetTvOffset_OutWall(0, bStt, FALSE, 0);
				GetXyMatchLineAndLine(tv.m_v1, tv.GetXyDir(), xy1, (xy1-xy2).Unit(), xy2);
			}

			Dom.LineTo(xy1, xy2);

			// ��ŷ
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

// ��ν����� �ܺ�ö�ٿ� ���ö�� �׸�
void CDrawVertSection::DrawRebarSupport_VertSection_UpperSlabDanBu(CDomyun *pDomP, BOOL bStt, BOOL bMarkRebar, BOOL bDimRebar, BOOL bInput)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	// ö�� �����ͼ� �׸���
	CHgBaseDrawStd baseDraw;
	CDPointArray xyArr;
	pBri->GetXyRebarSupport_VertSection_UpperSlabDanBu(xyArr, bStt);
	CDPoint xyDirScale	= bStt ? ToDPointFrDegree(135) : ToDPointFrDegree(45);
	baseDraw.DrawPointRebar(&Dom, xyArr, xyDirScale);

	// ġ�� ����
	if(bDimRebar)
	{

	}


	// ��ŷ
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

	// ö�� �����ͼ� �׸���
	CHgBaseDrawStd baseDraw;
	CDPointArray xyArr;
	pBri->GetXyRebarSupport_VertSection_LowerSlabDanBu(xyArr, bStt);
	CDPoint xyDirScale	= bStt ? ToDPointFrDegree(135) : ToDPointFrDegree(45);
	baseDraw.DrawPointRebar(&Dom, xyArr, xyDirScale);

	// ġ�� ����
	if(bDimRebar)
	{

	}

	// ��ŷ
	if(bMarkRebar && m_pStd->IsMarkRebar(""))
	{

	}


	*pDomP << Dom;
}

// ���ܸ� ��� ������ ��ġ�� ���ö�� �׸���
void CDrawVertSection::DrawRebarSupport_VertSection_UpperSlabHunch(CDomyun *pDomP, long nIdxHunch, BOOL bMarkRebar, BOOL bDimRebar, BOOL bInput, CDRect rectLimit)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	// ö�� �����ͼ� �׸���
	CDPointArray xyArrLeft, xyArrRight;
	pBri->GetXyRebarSupport_VertSection_UpperSlabHunch(xyArrLeft, xyArrRight, nIdxHunch);

	// rectLimit ����
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
	
	// ġ�� ����
	if(bDimRebar)
	{
	
	}


	// ��ŷ
	CDPointArray xyArrTot;
	long i = 0; for(i = 0; i < xyArrLeft.GetSize(); i++)
		xyArrTot.Add(xyArrLeft.GetAt(i));
	for(i = 0; i < xyArrRight.GetSize(); i++)
		xyArrTot.Add(xyArrRight.GetAt(i));
	CString sDescription	= _T("");
	sDescription.Format("��ν����� ��ġ�� ���ö��");
	CString sMark	= pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescription);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		CRebar *pRb = pBri->GetRebarSupportByDescription(sDescription);
		CPlaceRebarByBlock *pPlace		= &pBri->m_placeByBlock_SupportRebar_UpperSlab[iLOWER][0];
		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
		mark.SetRebar(xyArrTot);

		// ����
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

// ���ܸ� ��ν����� ��Ʈ�� ö�� �׸�.(�� ��ü��)
// BOOL bVirtual : ��Ʈ�� ö�� ���� �ܰ��� �׸�.
// long nIdxInputStirrup : �Է����� ���ͷ� ö��
// bDrawDim : Input��ġ������ �����ġ������ �޶� ���� �߰�. 
//  InPut������ B@C �� ǥ���ϰ� ���鿡���� 5@250 
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
		strSlab = bUpperSlabGirder ? _T("���") : _T("�Ϻ�");
	}
	// �Ŵ�ö�� ������ ����
	pBri->SetDataDefaultRebarGirder(nJ, TRUE, TRUE, bStt, bLeft, bGagak, bJongGirder, bApplyVert, bUpperSlabGirder);
	
	// ��Ʈ�� ö�� (��������ö��) /////////////////////////////////
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
		// ��Ʈ�� ö���� ��ö���� ���ε��� �׷��� ��.
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
					if(tv.m_sInfo.Find("��", 0) != -1)
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
			sDescription.Format("%s ���Ŵ� ��������ö��", bLeft ? "����" : "����");
		}
		else
		{
			sDescription.Format("����%d %s�Ŵ��� ��������ö��", nJ+1, strSlab);
		}
		CString sMark	= pBri->GetMarkRebarEtc(G_REBAR_ST, bInput, FALSE, FALSE, sDescription, TRUE);
		if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
		{
			// �������� ������ �и�.
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

			// ������ ö�ٵ��� ��ũ�ϱ� ���� ���� //////////
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
			mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_END;	// ���� ǥ����ġ ã�� ���
			mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;	// TYPE_MARKPOS_POS�϶� �ɼ�
			mark.m_dPosScaleMarkStt			= 0.5;
			mark.m_nTypeMarkDir				= TYPE_MARKDIR_RIGHT;
			mark.m_dCtc = pGirderRB->m_dCTC;
			mark.m_nTypeRebar = pRb? pRb->m_nTypeRebar : -1;
			mark.MarkRebar();
		}
	}

	// ��Ʈ�� ����ö��(����ö����)
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);
	for(upper = 0; upper < 2; upper++)
	{
		BOOL bUpper	= (upper == iUPPER);
		for(dan = 1; dan < MAX_REBAR_DAN; dan++)
		{
			pBri->GetTvRebarMain_SlabGirder(tvArrStrup, nJ, upper, dan, bStt, bLeft, bGagak, bJongGirder, bUpperSlabGirder);

			// ��Ʈ�� ���� ö��(��)
			// 2�ܺ��ʹ� ��±��� �Ʒ����� �̵��ؾ� ��.
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


	// ��ö��(���ܺ�) ///////////////////////////////////
	// ����.
	double dCoverFromt1Dan	= 0;	// 1�ܿ��������� �Ÿ�

	for(upper = 0; upper < 2; upper++)
	{
		BOOL bUpper		= (upper == iUPPER);
		
		//��ν����̺���, �Ϻν����̺��ϸ��ΰ�� bOutter = TRUE:
		BOOL bOutter = TRUE;
		if(bUpperSlabGirder)
			bOutter = bUpper;
		else
			bOutter = !bUpper;

		for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

			// ��ö��
			if(dan == 0)
				pBri->GetxyRebarSupport_SlabGirder(xyArr, xyArrDirApplyScale, upper, nJ, bStt, bLeft, bGagak, bJongGirder, bUpperSlabGirder, TRUE, dScaleDom);
			else
				pBri->GetXyRebarSupport_SlabGirderInner(xyArr, nJ, upper, dan, bStt, bLeft, bGagak, bJongGirder, bUpperSlabGirder);
			baseDraw.DrawPointRebar(&Dom, xyArr, xyArrDirApplyScale);
			
			if(xyArr.GetSize() == 0) continue;

			// 1�ܿ��� ���� �ܱ����� �Ÿ�(ġ�� ���̸� �����ֱ� ����)
			dCoverFromt1Dan	= bUpper ? (pGirderRB->m_dCoverUpper[dan]-pGirderRB->m_dCoverUpper[0]) : (pGirderRB->m_dCoverLower[dan]-pGirderRB->m_dCoverLower[0]);

			// ġ�� ����
			if(bDimRebar && xyArr.GetSize() > 0 && (nDanDim == dan || nDanDim == -1))
			{
				BOOL bTotal	= nDanDim != -1 ? TRUE : FALSE;
				// ��ü �� ġ�� �����Ҷ��� ���� ���� ö���� �ִ��� �˻��ؼ� ������ ��ü ġ���� �ִ´�.
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

				// ���ۺ� �Ǻ�
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

				// �������� �Ǻ�
				CString sDan	= _T("");
				//sDan.Format("%s%d��", bUpper ? "���" : "�ϸ�", dan+1);	// ����� 1�ܸ� �����ϹǷ� �Ʒ��ٰ� ���� ����
				sDan.Format("%s", bUpper ? "���" : "�ϸ�");
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

			// ��ŷ
			CString sDescription	= _T("");
			CString sUpper			= bUpper ? "���" : "�ϸ�";
			long nTypeRebar			= bUpper ? G_REBAR_M_U1+dan : G_REBAR_M_L1+dan;
			if(bJongGirder)
			{
				sDescription.Format("%s ���Ŵ� %s ��ö�� %d��", bLeft ? "����" : "����", sUpper, dan+1);
			}
			else
			{
				sDescription.Format("����%d %s�Ŵ��� %s ��ö�� %d��", nJ+1, strSlab, sUpper, dan+1);
			}
			CString sMark	= pBri->GetMarkRebarEtc(nTypeRebar, bInput, FALSE, FALSE, sDescription, TRUE);
			if(bMarkRebar && m_pStd->IsMarkRebar(sMark) && (nDanDim == dan || nDanDim == -1))
			{
				m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

				CRebar *pRb = pBri->GetRebarEtcByDescription(sDescription, TRUE);
				CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
				
				// ��ö��
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

	// �¿�(��������ö����)
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
			// �Ʒ��� �Ÿ�
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


			// ���� �Ÿ�
			dArrDim.Add(pGirderRB->m_dCoverUpper[0]);
			sArrText.Add("dc");

			xyStt	= rb.GetXyEdgePoint(xyArr, RIGHTDOWN_AREA);
			xyStt.y	-= pGirderRB->m_dF1;
			xyStt.y -= pGirderRB->m_dCoverLower[0];
			sDir	= "RIGHT";
		}
		else //�Ϻν����̺��� ��쿡�� ����������.
		{
			// �Ʒ��� �Ÿ�
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

			// ���� �Ÿ�
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
			sDescription.Format("%s ���Ŵ� ��������ö��", bLeft ? "����" : "����");
		}
		else
		{
			sDescription.Format("����%d %s�Ŵ��� ��������ö��", nJ+1, strSlab);
		}
		CString sMark	= pBri->GetMarkRebarEtc(G_REBAR_HORV, bInput, FALSE, FALSE, sDescription, TRUE);
		if(m_pStd->IsMarkRebar(sMark))
		{
			// ö�� ��ũ�� ���� ������ �ϱ� ���ؼ� ���� ���� �ִ� ö�ٰ� x��ġ�� �����Ÿ� ��󳽴�.
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
			
			// ��ö��
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
			sDescription.Format("%s ���Ŵ� ���� ��������ö��", bLeft ? "����" : "����");
		}
		else
		{
			sDescription.Format("����%d %s�Ŵ��� ���� ��������ö��", nJ+1, strSlab);
		}
		sMark	= pBri->GetMarkRebarEtc(G_REBAR_HORV_I, bInput, FALSE, FALSE, sDescription, TRUE);
		if(m_pStd->IsMarkRebar(sMark))
		{
			// ö�� ��ũ�� ������ �ϱ� ���� ������ ���� ���� �ִ°� ���� �� ���.
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

			// �� ��󳻰� ���� �� �ٸ� ����
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
			
			// ��ö��
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

// �Ŵ� �󼼵� �׸���
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

	// �߶� ����
	CDRect rect	= GetRectCuttingForGirderDetail(nJ, bStt, bLeft, bGagak, bJongGirder, bUpperSlabGirder);

	// ���ܸ鵵�� �߶� ������ŭ �߶󳻼� �׸�.
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
		// ������ skew�� �ִ°�� ���� �������� �����ֱ� ���� ������ ������ �׳� �׷��ش�.
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
			

			// ��� �Ŵ�=>����� �ϸ�, �ϺΰŴ�=>�Ϻ��� �ϸ�
			tvAdd.m_v1 = xyBase;
			tvAdd.m_v1.y += (pBri->m_dHHL[nIdxHunch][nPos] + (bUpperSlabGirder? pBri->m_dTS : pBri->m_dTS_Lower)) * dDirY;
			tvAdd.m_v1.y += dPlusHeight * dDirY;
			tvAdd.m_v2 = tvAdd.m_v1;
			tvAdd.m_v1.x -= 10000;
			tvAdd.m_v2.x += 10000;
			tvArrSect.Add(tvAdd);
						
			// ����
			tvAdd.m_v1 = xyBase;
			tvAdd.m_v2 = xyBase - pBri->m_dWL[nIdxHunch][nPos] * CDPoint(1, 0);
			tvArrSect.Add(tvAdd);
			//����: �߰��� ���̰� �� ��� �߰�
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
			
			// ����
			tvAdd.m_v1 = xyBase;
			tvAdd.m_v2 = xyBase + pBri->m_dWR[nIdxHunch][nPos] * CDPoint(1, 0);
			tvArrSect.Add(tvAdd);
			//����: �߰��� ���̰� �� ��� �߰�
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

	// ��ö�� ������ε� �׸�.
	if(bUpperSlabGirder)
		DrawVirtualRebarMain_VertSection_UpperSlab(&Dom, rect);
	else
		DrawVirtualRebarMain_VertSection_LowerSlab(&Dom, rect);

	Dom.SetCalcExtRect();
	CDRect rectTot	= Dom.GetExtRect();

	*pDomP << Dom;

	return rectTot;
}

// �Ŵ��� ���  �󼼵� �׸�.
void CDrawVertSection::DrawDetail_Column_VertSection(CDomyun *pDomP, long nIdxWall)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	long nCountInWall	= pBri->GetCountInWall();
	if(nIdxWall == -1) nIdxWall	= nCountInWall-1;
	if(nIdxWall < 0 || nIdxWall > nCountInWall-1) return;
	
	CWallApp *pWall	= pBri->GetInWall(nIdxWall);
	if(!pWall) return;

	// ���⼱ �ϴ� ����� ���̴� ����. �Ѥ� 
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

	// ��տ��� �¿�� dMarginHor, ������ ��/���Ͽ��� dMarginVer�� ������׸�.
	// �߶� ����
	const double dMarginHor	= 1000;
	const double dMarginVer	= 1000;
	CDRect rectColumn	= tvArrInWall.GetRect();
	CDRect rectVert		= tvArrVertSection.GetRect();
	CDRect rectFooting	= tvArrFooting.GetRect();
	
	//Box���϶��� Footing�� ������ �ܸ鵵Bottom�� Footing���� ��.
	if(pBri->IsBoxType())
		rectFooting = rectVert;

	CDRect rect(rectColumn.left - dMarginHor, rectVert.top + dMarginVer, rectColumn.right + dMarginHor, rectFooting.bottom - dMarginVer);

	// ���ܸ鵵�� �߶� ������ŭ �߶󳻼� �׸�.
	m_pStd->DrawCuttingTvArray(&Dom, tvTot, rect, FALSE);

	// ��ö�� ������ε� �׷���.
	DrawVirtualRebarMain_VertSection_UpperSlab(&Dom, rect);
	if(pBri->IsBoxType())
		DrawVirtualRebarMain_VertSection_LowerSlab(&Dom, rect);
	*pDomP << Dom;
}


// ���ܸ鵵 ��ö�� ������ ġ�� ����.
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
	
	// �������� �������� rc ���� ////////////
	if(pBri->IsOutWall(TRUE))
	{
		pBri->m_Util.AddRebarInfoCycle(&arrRC, &pBri->m_pArrRCMainOutWall[nCycle][iSTT][iOUTTER][nDan], FALSE);
	}

	pBri->m_Util.AddRebarInfoCycle(&arrRC, &pBri->m_pArrRCMainUpperSlab[nCycle][iUPPER][nDan], FALSE);

	// �������� ����� ��ģ��.
	if(pBri->IsOutWall(FALSE))
	{
		pBri->m_Util.AddRebarInfoCycle(&arrRC, &pBri->m_pArrRCMainOutWall[nCycle][iEND][iOUTTER][nDan], TRUE);
	}
	//////////////////////////////////////////


	// �������� ������ ö���� ���۰� ���κ��� ����� ����
	long nSize	= arrRC.GetSize();
	if(nSize > 0)
	{
		// ��ν����� ö���� ��ü���� ��� ���� �߶� �ΰ��� ����� �ش�
		// �׷��� ġ���� �̻ڰ� ����
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
				// ���� �δ� �������� �и��ؼ� �߰�
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
				// ���� �δ� �������� �и��ؼ� �߰�
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

				// ��ν����� ���� ���� ���� (���� ġ���� ���� ���� �ϱ� ����)
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
				// ���� ���ʿ� ������ ����θ� �������� �߰�
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
				// ���� ���ʿ� ������ ����θ� �������� �߰�
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

				// ��ν����� ���� ���� ���� (���� ġ���� ���� ���� �ϱ� ����)
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
				
		
		// ������ġ�� �Ǻ� ����
		if(pBri->IsOutWall(TRUE))
			xyStt.y -= pBri->m_footingStt.m_dCoverLower[0];
		else
		{
			CTwinVector tv	= pBri->m_tvArrVertSection.GetTvByInfo("��������������");
			xyStt.x	= tv.m_v1.x;
		}

		// ��������ġ�� �Ǻ�����
		if(pBri->IsOutWall(FALSE))
			xyEnd.y -= pBri->m_footingEnd.m_dCoverLower[0];
		else
		{
			CTwinVector tv	= pBri->m_tvArrVertSection.GetTvByInfo("��������������");
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

	// Box���϶� �Ϻ� ������� ��� ������� �ݴ��̹Ƿ� ��ν������� �ϸ��� �Ϻν������� ����̴�.
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

		// ������ǥ �� ��ǥ�� �¿��� ��ü ���� �ݰ� �Ѵ�.
		if(pBri->IsOutWall(TRUE))
		{
			CTwinVector tvWall	= pBri->m_tvArrVertSection.GetTvByInfo("������ü������");
			GetXyMatchLineAndLine(tvWall.m_v1, tvWall.GetXyDir(), xyStt, CDPoint(-1, 0), xyStt);
		}
		else 
		{
			CTwinVector tvSide	= pBri->m_tvArrVertSection.GetTvByInfo("��������������");
			GetXyMatchLineAndLine(tvSide.m_v1, tvSide.GetXyDir(), xyStt, CDPoint(-1, 0), xyStt);
		}

		if(pBri->IsOutWall(FALSE))
		{
			CTwinVector tvWall	= pBri->m_tvArrVertSection.GetTvByInfo("������ü������");
			GetXyMatchLineAndLine(tvWall.m_v1, tvWall.GetXyDir(), xyEnd, CDPoint(1, 0), xyEnd);
		}
		else
		{
			CTwinVector tvSide	= pBri->m_tvArrVertSection.GetTvByInfo("��������������");
			GetXyMatchLineAndLine(tvSide.m_v1, tvSide.GetXyDir(), xyEnd, CDPoint(1, 0), xyEnd);
		}
		
		m_pStd->DimRebarInfoCycleArray(&Dom, &arrRC, xyStt, xyEnd, bUpperSlab? FALSE:TRUE, nDimDan, nDrawLeft);

		*pDomP << Dom;
	}

	AhTPADelete(&arrRC, (CRebarInfoCycle*)0);
}

// ������������ ��ü 
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
		// ù��°�� ����� ����
		// ����δ� ���� ������
		CRebarInfoCycle *pRC	= arrRC.GetAt(0);
		if(pRC->m_R_Mid > 0)
		{
			// ���� �δ� �������� �и��ؼ� �߰�
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
			//#ARCBRIDGE-3725 ������ü ����ö�� ġ���� �� �ϴ� �Ǻ� ��ġ ����
			if(pRC->m_nPosStt != pRC->m_nPosEnd)
			{
				pRC->m_xyStt		= pRC->m_xyMid1;
			}
		}

		xyStt	= arrRC.GetAt(0)->m_xyStt;
		
		pRC	= arrRC.GetAt(arrRC.GetSize()-1);
		xyEnd	= pRC->m_xyEnd;

		// ������ġ�� �Ǻ� ����
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
			CTwinVector tvLower	= pFooting->m_tvArrFront.GetTvByInfo("�����ϸ�");
			xyStt.y = tvLower.m_v1.y;
			if(pBri->IsBoxType())
			{
				tvLower = pBri->m_tvArrVertSection.GetTvByInfo(bStt? _T("�Ϻν������ϸ�(������ü�Ʒ�)") : _T("�Ϻν������ϸ�(������ü�Ʒ�)"));
				GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), xyStt, CDPoint(0, 1), xyStt);
			}
			//GetXyMatchLineAndLine(pRC->m_xyStt, (pRC->m_xyStt-pRC->m_xyMid1).Unit(), tvLower.m_v1, tvLower.GetXyDir(), xyMatch);
			
		}

		// ������ ��ġ�� �Ǻ� ����
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

// ��ö�� ������ �׸�.
// nCycle : �׸� cycle
// nDrawLeft : -1 -> ���ʸ� �׸�, 0 : �¿� ��� �׸�, 1 : �����ʸ� �׸�.
void CDrawVertSection::DrawMainRebarAssm(CDomyun *pDomP, long nCycle, long nDrawLeft, BOOL bDrawVertSection, BOOL bVertDir, BOOL bInput)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(nCycle == -1) nCycle = pBri->GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > pBri->GetCountCycleMainRebar()-1) return;		

	CDomyun Dom(pDomP);
	long nDimDan	= 2;

	// ���� ������ �⺻������
	pBri->m_footingEnd.m_exFooting.m_Pile.InitScaleCutting();

	// ��� ��ü���� ����ȭ�� ����.
	//pBri->SyncBridge(0, !bVertDir);
	pBri->GetTvVertSection_Std(0, !bVertDir, FALSE);
	
	// ���ܸ� �׸�
	BOOL bPatternRoundRectWall	= FALSE;
	CWallApp *pWall	= pBri->GetInWall(0);
	if(pWall)
	{
		bPatternRoundRectWall	= pWall->m_bTopIsColumn;

	}
	if(bDrawVertSection)
		DrawVertSectionAll(&Dom, FALSE, FALSE, FALSE, FALSE, FALSE, bPatternRoundRectWall);

	// ö�� �׸���(ġ��, ���� ����)
	long nCountWall	= pBri->GetCountInWall();

	pBri->SyncBridgeRebar(FALSE, bVertDir?-1:1, TRUE);
	
	// ��ö�� �׸�.
	DrawRebarMain_VertSectionAll(&Dom, nCycle, TRUE, FALSE, TRUE, nDrawLeft, bInput);

	// ������ ġ�� ����
	if(pBri->IsBoxType())
	{
		DimRebarMainAssm_VertSection_BoxOutSide(&Dom, nDimDan, nCycle, nDrawLeft);
		// �Ϻν������� ���
		DimRebarMainAssm_VertSection_Lower(&Dom, nDimDan, nCycle, nDrawLeft, 0, FALSE);
	}
	else
	{
		DimRebarMainAssm_VertSection_Upper(&Dom, nDimDan, nCycle, nDrawLeft);
		DimRebarMainAssm_VertSection_FootingLower(&Dom, nDimDan, nCycle, nDrawLeft);
		DimRebarMainAssm_VertSection_UpperSlabSide(&Dom, nDimDan, nCycle, nDrawLeft);
	}
	// ��ν������� �ϸ�
	DimRebarMainAssm_VertSection_Lower(&Dom, nDimDan, nCycle, nDrawLeft);
	DimRebarMainAssm_VertSection_OutWall(&Dom, nDimDan, nCycle, TRUE, nDrawLeft);
	DimRebarMainAssm_VertSection_OutWall(&Dom, nDimDan, nCycle, FALSE, nDrawLeft);
	
	// ��ü��
	for(long wall = 0; wall < nCountWall; wall++)
	{
		CWallApp *pWall	= pBri->GetInWall(wall);
		if(pWall)
		{
			m_pStd->m_pDrawWall->DimRebarMainAssm_Front(&Dom, pWall, nDimDan, nCycle, TRUE, nDrawLeft);
			m_pStd->m_pDrawWall->DimRebarMainAssm_Front(&Dom, pWall, nDimDan, nCycle, FALSE, nDrawLeft);
		}
	}

	// ���ʵ�
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
	
	// ����Ŭ ���� ǥ��
	CString sCycle	= _T("");
	CString sCTC	= _T("");
	sCycle.Format("%s CYCLE", m_pStd->m_pARcBridgeDataStd->GetStringCircleNum(nCycle+1));
	sCTC.Format("C.T.C %.0f", pBri->m_dCTC_MainRebar);

	// ���� �� ��ġ
	Dom.SetTextAlignHorz(TA_CENTER);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SMALL_TITLE);
	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();
	double dStaCuttingOrg	= m_pStd->GetStaCuttingOrg();
	CDPoint xyText	= CDPoint(dStaCuttingOrg, rect.bottom+Dom.GetTextHeight()*2);
	if(nDrawLeft == -1) xyText.x -= Dom.GetTextWidth(sCTC);
	else if(nDrawLeft == 1) xyText.x += Dom.GetTextWidth(sCTC);

	// ���� ��
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

		// �߽ɼ�
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

// ǥ�� �ܸ鵵 �׸���(���� ��¿�)
// long nVertDir : -1 : ���Ÿ�, 0 : �ڵ�(��������), 1 : ��Ÿ�
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

	// ���ܸ鵵 �׸���
	// ���� ������ �⺻������
	pBri->m_footingEnd.m_exFooting.m_Pile.InitScaleCutting();
	// ��� ��ü���� ����ȭ�� ����.
	//pBri->SyncBridge(nLeft, !bVertDir);
	pBri->GetTvVertSection_Std(nLeft, !bVertDir, FALSE);
	pBri->SyncBridgeRebar(FALSE, nVertDir);

	// ���ܸ� �׸�
	DrawVertSectionAll(&Dom, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE);

	// ��ö�� 1cycle �׸���
	DrawRebarMain_VertSectionAll(&Dom, 0, TRUE, FALSE, bMarkMainRebar, 0, FALSE, TRUE);

	// ���ö��/����ö�� �׸���
	long dan(0), upper(0), stt(0), wall(0), left(0), i(0);
	BOOL bUpper(TRUE), bStt(TRUE), bLeft(TRUE);
	
	// ��ν�����
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

			// ����
			DrawRebarSupport_VertSection_ArchRib(&Dom, bUpper, dan, bMarkRebar, FALSE, dan, FALSE, CDRect(0, 0, 0, 0), nVertDir);
		}
	}
	DrawRebarShear_VertSection_UpperSlab(&Dom, bMarkRebar, FALSE, FALSE);

	if(pBri->IsBoxType())
		DrawRebarShear_VertSection_LowerSlab(&Dom, bMarkRebar, FALSE, FALSE);

	// ����
	DrawRebarShear_VertSection_ArchRib(&Dom, bMarkRebar, FALSE, FALSE, CDRect(0, 0, 0, 0), nVertDir);
	DrawRebarStirrup_VertSection_ConnectSection(&Dom, FALSE, bMarkRebar, FALSE);

	for(stt = 0; stt < 2; stt++)
	{
		bStt	= (stt==iSTT);
		DrawRebarSupport_VertSection_UpperSlabDanBu(&Dom, bStt, bMarkRebar, bDimRebar, FALSE);
		if(pBri->IsBoxType())
			DBStd.m_pDrawVertSection->DrawRebarSupport_VertSection_LowerSlabDanBu(&Dom, bStt, bMarkRebar, bDimRebar, FALSE);
	}

	// ������ü
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
	
	// ����� & �λ��������
	for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		DrawRebarSupport_VertSection_Bracket(&Dom, bStt, bDimRebar, bMarkRebar, FALSE);
		DrawRebarShear_VertSection_Bracket(&Dom, bStt, bDimRebar, bMarkRebar, FALSE);	
		DrawBFootingRebar(&Dom, bStt, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE);
	}

	// ������ü
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

	// ��ġ�� ���ö��
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

	// ����
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

			// ����ö��
			// ������� ���� ġ�� ����
			BOOL bDimSideRebar	= pFooting->m_nType != FOOTING_TYPE_BASIC && pFooting->m_nType != FOOTING_TYPE_BASIC_BOTH;
			BOOL bLeftSlope	= pFooting->m_nType != FOOTING_TYPE_RIGHTSLOPE && pFooting->m_nType != FOOTING_TYPE_BASIC_RIGHTSLOPE;
			DBStd.m_pDrawFooting->DrawRebarSideOut_Front(&Dom, pFooting, TRUE, TRUE, bDimSideRebar, TRUE, FALSE, bLeftSlope ? 2 : 3);
			DBStd.m_pDrawFooting->DrawRebarSideOut_Front(&Dom, pFooting, FALSE, TRUE, bDimSideRebar, TRUE, FALSE, bLeftSlope ? 3 : 2);
		}
	}


	// �Ŵ� ���� ö�� ǥ��
	for(long nJ = 0; nJ < pBri->GetCountJijum(); nJ++)
	{
		DrawRebarAll_SlabGirder(&Dom, nJ, FALSE, FALSE, 0, FALSE, TRUE,TRUE,TRUE,TRUE,FALSE,-1,TRUE);
	}

	// ������ ����ö��
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


	// ��� ���ʰ� ���Ÿ��� �׷����� �ƴ϶�� ()���� ġ���� ���Ÿ��� ǥ�� ����
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
			Dom.TextOut(rect.right, rect.top-Dom.GetTextHeight()*2, _T("* () ���� ġ���� ���Ÿ� ġ����."));
			*pDomP << Dom;
		}
	}

	pBri->GetTvVertSection_Std(nLeft, !bVertDir, FALSE, TRUE);
	return rectTot;
}

// ���ܸ鵵 ġ�� ��� �ֱ�
// BOOL bForOutBM : �������⼭ ��¿�����?
void CDrawVertSection::DimVertSectionAll(CDomyun *pDomP, BOOL bForOutBM)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	
	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	DBStd.m_bApplyDimSettingOfBaseDom	= m_pStd->m_bApplyDimSettingOfBaseDom;

	CDomyun Dom(pDomP);
	m_pStd->ApplyDimSetting(pDomP, &Dom);

	// ����
	if(!bForOutBM)
		DimTextVerSectionEL(&Dom);
	if(bForOutBM)
	{
		if(pBri->m_nTypeSlabLength == 0)
			DimVertSectionUpper(&Dom, bForOutBM ? 0 : 1, TRUE, FALSE);
		else
			DimVertSectionUpperForBMSlabLengthTyp1(&Dom);	// �������̷� ������ ���� ������ ��� ��
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
	
	// ����
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
			// Ȯ�����
			DBStd.m_pDrawExFooting->DimFrontLower(&Dom, &pFooting->m_exFooting, 0, FALSE);
			DBStd.m_pDrawExFooting->DimFrontSide(&Dom, &pFooting->m_exFooting, nJ==pBri->GetCountJijum()-1, 0, FALSE);
		}
	}

	// ������ġ
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

// �߰� ��ü �� �ܸ�(A-A) ������..
// �߰���ü ������ ���� A-A �ܸ����� ���.
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


	// �ܸ鵵 A-A�� �׷��� ���� ���.
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
	
	// ��ν������ �� �ܸ鵵�� ������ �׸�.
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
	
	// ������ü �ܸ鵵
	DBStd.m_pDrawWall->DrawRebar_FrontSectionStd(&Dom, pWall, bInput, TRUE, FALSE, bVertDir);
	DrawRebarAll_SlabGirder(&Dom, nJ, FALSE, FALSE, 0, FALSE, TRUE,TRUE,TRUE,TRUE,FALSE,-1,TRUE);
	if(pBri->IsBoxType())
		DrawRebarAll_SlabGirder(&Dom, nJ, FALSE, FALSE, 0, FALSE, TRUE,TRUE,TRUE,TRUE,FALSE,-1,TRUE, FALSE);
	// ���� �ܸ鵵
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
	// �ܸ鵵 A-A�� �׷��� ���� ���.
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
	
	// ��ν������ �� �ܸ鵵�� ������ �׸�.
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

	// ������ü ..
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

			// ��米2Ÿ���� ���.. ������ü�� ���� �׷���
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

	// �����
	DrawRebarMain_VertSection_Bracket(&Dom, bStt, FALSE, FALSE, FALSE, 0, rect);
	if(!bDrawOnlyMainRebar)
	{
		DrawRebarSupport_VertSection_Bracket(&Dom, bStt, FALSE, bMarkRebar, FALSE, rect);
		DrawRebarShear_VertSection_Bracket(&Dom, bStt, FALSE, bMarkRebar, FALSE, rect);
	}
	
	// �� ������
	DrawRebarMain_VertSection_Danbu(&Dom, FALSE, FALSE, FALSE, 0, bStt ? -1 : 1);
	if(!bDrawOnlyMainRebar)
	{
		DrawRebarSupport_VertSection_UpperSlabDanBu(&Dom, bStt, bMarkRebar, FALSE, FALSE);
		if(pBri->IsBoxType())
			DBStd.m_pDrawVertSection->DrawRebarSupport_VertSection_LowerSlabDanBu(&Dom, bStt, bMarkRebar, FALSE, FALSE);
	}

	// ��ġ�� ����ö��
	if(!bDrawOnlyMainRebar)
	{
		DrawRebarShear_VertSection_UpperSlabDanbu(&Dom, 0, bStt, FALSE, FALSE, FALSE);
		DrawRebarShear_VertSection_LowerSlabDanbu(&Dom, 0, bStt, FALSE, FALSE, FALSE);
	}
	

	// ������ ����ö��
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

	// ���� �ܸ鵵
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

// ��ν����� ��ö�� 1�ܿ� ���� ���� ������ �׸���.
// �̴� ��� ��� �Ŵ� �ܸ�ö���� ����Ҷ� �����ϱ� ���� �׷��ִ� ����.
// CONC���� �߽ɼ����� �׷���.
// ��������� �׸����� ��ö���� ��� �����ؼ� �׷��� ��.
// �׷��� ������� �߽ɼ��� �������� ���̹Ƿ� �������� �� �� ����.
void CDrawVertSection::DrawVirtualRebarMain_VertSection_UpperSlab(CDomyun *pDomP, CDRect rectLimit)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARR, LT_DASHDOT_LINE);

	// ��� 
	CRebarInfoCycle *pRCUpperStt	= pBri->GetRebarInfoCycleUpperSlab(0, 0, TRUE, 0),
					*pRCUpperEnd	= pBri->GetRebarInfoCycleUpperSlab(0, 0, TRUE, -1);
	CRebarInfoCycle rcUpperTot;

	if(pRCUpperStt && pRCUpperEnd)
	{
		rcUpperTot.m_xyStt	= pRCUpperStt->m_xyMid2;
		rcUpperTot.m_xyMid1	= pRCUpperEnd->m_xyMid1;
		m_pStd->DrawRebarInfoCycle(&Dom, &rcUpperTot, 0, rectLimit);
	}

	// �ϸ� 
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

// �Ϻν����� ��ö�� 1�ܿ� ���� ���� ������ �׸���.
// �̴� ��� �ϴ� �Ŵ� �ܸ�ö���� ����Ҷ� �����ϱ� ���� �׷��ִ� ����.
// CONC���� �߽ɼ����� �׷���.
// ��������� �׸����� ��ö���� ��� �����ؼ� �׷��� ��.
// �׷��� ������� �߽ɼ��� �������� ���̹Ƿ� �������� �� �� ����.
void CDrawVertSection::DrawVirtualRebarMain_VertSection_LowerSlab(CDomyun *pDomP, CDRect rectLimit)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARR, LT_DASHDOT_LINE);
	// ��� 
	CRebarInfoCycle *pRCUpperStt	= pBri->GetRebarInfoCycleLowerSlab(0, 0, TRUE, 0),
		*pRCUpperEnd	= pBri->GetRebarInfoCycleLowerSlab(0, 0, TRUE, -1);
	CRebarInfoCycle rcUpperTot;

	if(pRCUpperStt && pRCUpperEnd)
	{
		rcUpperTot.m_xyStt	= pRCUpperStt->m_xyMid2;
		rcUpperTot.m_xyMid1	= pRCUpperEnd->m_xyMid1;
		m_pStd->DrawRebarInfoCycle(&Dom, &rcUpperTot, 0, rectLimit);
	}

	// �ϸ� 
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

//BOOL bForUseBM : ������(��ä���� ������ ����ä �׷�����.)
//long iIndex : ����
//pBriBox : Box���� ������ܸ��� ����ؾ��ϹǷ� ���ڷ� �Ѱܹ���.
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

	// ���ı� EL 
	CDRect rect = tvArr.GetRect();
	GetSwap(rect.top, rect.bottom);
	CString strEl	= _T("");
	CConcSymbol SymDom(m_pStd->m_pARoadOptionStd);

	if(bTyopagi)
	{
		double dExt	= 1000;

		if(pFooting->m_dEL_DiggingFirst > pFooting->m_dEL-pFooting->GetHeight())
			m_pStd->DrawTvArray(&Dom, tvArr, FALSE, FALSE, 0);

		// ���ݼ�
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

		// ���ϼ��� ǥ��
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
			
			CString sTupagi = _T("�������ı�");
			CString sWaterTupagi =pBri->m_bApplyYongSu? _T("������ı�") : _T("�������ı�");

			Dom.TextOut(rect.right-dExt, pFooting->m_dEL_DiggingWater+Dom.Always(1), sTupagi);
			Dom.TextOut(rect.right-dExt, pFooting->m_dEL_DiggingWater-Dom.Always(1)-Dom.GetTextHeight(), sWaterTupagi);

			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
			m_pStd->ApplyDimSetting(pDomP, &Dom);

			strEl.Format("EL. %0.3f", toM(pFooting->m_dEL_DiggingWater));
			SymDom.ArrowElevation(&Dom, bInput ? "EL2" : strEl, bInput ? strEl : "", CDPoint(rect.left+Dom.Always(3), pFooting->m_dEL_DiggingWater),FALSE,FALSE);
		}
	}

	// �ֻ�
	if(bJusang)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CUTL);
		m_pStd->ApplyDimSetting(pDomP, &Dom);

		CBoringDataEx *pBoring;
		
		pBoring = pBri->GetBoringDataExByJijumNumber(iIndex);
		m_pStd->m_pDrawLine->DrawBroing(&Dom, pBoring, TRUE);
	}

	// ��ä��
	if(bBackFill)
	{
		//Box���϶��� ������ Bridge�� ���� ����.
		if(pBri->IsBoxType())
		{
			if(!pBriBox) return;
			pBri = pBriBox;
		}
		pBri->m_pLineApp;

		CTwinVector tv;
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DASH);
		m_pStd->ApplyDimSetting(pDomP, &Dom);

		//BOX���� ��� ���ʴ� �׷��ش�.
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_BOX && !bForUseBM)
		{
			if(pBri->m_nDeductNearTupagiBM != 1)
			{
				tv	= pBri->GetTvBackFill(TRUE, bForUseBM ? 4 : 0, TRUE); //��
				Dom.LineTo(tv.m_v1, tv.m_v2);
			}
			if(pBri->m_nDeductNearTupagiBM != 2)
			{
				tv	= pBri->GetTvBackFill(FALSE, bForUseBM ? 4 : 0, TRUE); //��
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

// BOOL bOnlyFooting : ���ʴ� ���� �׸��� ��ü�� �߶� ���ݸ� �׸� (��� ������ �ȳ���)
// ��米2, ��ġ���� ���ʿ� ��ü�� �پ������� ���� �׷�����Ѵ�.
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
		else	// �������� �ڸ��� �κп��� ������ �־ ��� ���� �ʴ´�.
		{
			if(tv.m_v1.z == 0)
				tvArrTot.Add(tv);				
		}
	}
	
	CTwinVector tvWallLowerR, tvWallLowerL;
	// ��ü
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
			// �ߺ��� �� ����
			long nSize = pWall->m_tvArrFront.GetSize();
			for(i=0; i<nSize; i++)
			{
				tv = pWall->m_tvArrFront.GetAt(i);
				if(tv.m_sInfo == _T("��ü�ϸ����"))
				{
					tvWallLowerR = tv;
					continue;
				}
				if(tv.m_sInfo == _T("��ü�ϸ�����"))
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
			// �ߺ��� �� ����
			long nSize = pWall->m_tvArrFront.GetSize();
			for(i=0; i<nSize; i++)
			{
				tv = pWall->m_tvArrFront.GetAt(i);
				if(tv.m_sInfo == _T("��ü�ϸ����"))
				{
					tvWallLowerR = tv;
					continue;
				}
				if(tv.m_sInfo == _T("��ü�ϸ�����"))
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
	else	//  ��米 ��ü
	{
		CWallApp *pWall;
		pWall = pBri->GetInWall(iIndex-1);
		tvArrOld = pWall->m_tvArrFront;

		//31720
		CTwinVectorArray tvArrVertSection;
		tvArrVertSection.AddFromTvArray(pBri->m_tvArrVertSection); 

		pWall->GetTvFront(tvArrVertSection, FALSE);
		// �ߺ��� �� ����
		long nSize = pWall->m_tvArrFront.GetSize();
		for(i=0; i<nSize; i++)
		{
			tv = pWall->m_tvArrFront.GetAt(i);
			if(tv.m_sInfo == _T("��ü�ϸ����") || tv.m_sInfo == _T("��ü�ϸ�����"))
				continue;
			tvArrTot.Add(tv);
		}
		//tvArrTot.AddFromTvArray(pWall->m_tvArrFront);
		
		pWall->m_tvArrFront = tvArrOld;
		pFooting = &(pWall->m_footing);
	}
	
	// ����
	CTwinVector tvFootingUp, tvFootingExt;
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 || pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		if(iIndex==0)
		{
			pFooting->m_tvArrFront.GetTvByInfo(_T("���ʻ�����"), tvFootingUp);
			pFooting->m_tvArrFront.GetTvByInfo(_T("���ʿ���Ȯ����"), tvFootingExt);
		}
		else
		{
			pFooting->m_tvArrFront.GetTvByInfo(_T("���ʻ������"), tvFootingUp);
			pFooting->m_tvArrFront.GetTvByInfo(_T("��������Ȯ����"), tvFootingExt);
		}
	}

	// ���ʿ� ��ü�� �̾����� �� ����� �ش�.
	pFooting->GetTvFront(FALSE);
	tvArrTot.AddFromTvArray(pFooting->m_tvArrFront);
	
	tvArrOld = pFooting->m_exFooting.m_tvArrFront;
	pFooting->m_exFooting.GetTvFront(FALSE);
	tvArrTot.AddFromTvArray(pFooting->m_exFooting.m_tvArrFront);
	pFooting->m_exFooting.m_tvArrFront = tvArrOld;

	rtemp = tvArrTot.GetRect();
	cRect.top = rtemp.top+500;
	cRect.bottom = rtemp.bottom-500;

	// ���� �ʺ� ���缭 �ڸ�
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
	// ���ı� �ʺ� ���� ��ν����긦 �аų� ���� �ڸ��� ���� 
	// ���ı� ���̸� �¿�� �����ش�
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

//BOOL bForUseBM : ������(��ä���� ������ ����ä �׷�����.)
//pBriBox : Box���� ������ܸ��� ����ؾ��ϹǷ� ���ڷ� �Ѱܹ���.
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

		pTvArr.GetTvByInfo("���� �̰�", tv);
		dArrDim.Add(tv.GetHorLength());
		sArrText.Add("Pf");
		sArrUnderText.Add(COMMA(tv.GetHorLength()));
		xyStt = tv.m_v2;

		pTvArr.GetTvByInfo("������", tv);
		dArrDim.Add(tv.GetHorLength());
		sArrText.Add("������");
		sArrUnderText.Add(COMMA(tv.GetHorLength()));
		
		pTvArr.GetTvByInfo("������ �̰�", tv);
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
		
		// ���ݼ�
		CTwinVectorArray tvArrTypagi;
		pBri->GetTvTogongLine(tvArrTypagi, iIndex, FALSE, TRUE, TRUE, FALSE, pBri->m_nDeductNearTupagiBM);
		CTwinVector tvEL;
		tvArrTypagi.GetTvByInfo("���", tvEL);
		
		double dDist = 0;
		// ��ä��
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
			
			//BOX�ΰ�� ���ʿ��� DistOffset��ŭ�� ���ָ��.
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
			sArrUnderText.Add("������");
			
			dArrDim.Add(-(pFooting->m_dHeightNosang));
			sArrText.Add(bInput ? "H2" : COMMA(fabs(pFooting->m_dHeightNosang)));
			sArrUnderText.Add("�����");
			
			dh = dy - pFooting->m_dHeightPavement - pFooting->m_dHeightNosang - pFooting->m_dEL;
			dArrDim.Add(-(dh));
			sArrText.Add(COMMA(dh));
			sArrUnderText.Add("��ü��");
			
			baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, 0, sDir, bForUseBM ? 0 : -1, FALSE, FALSE, TRUE);

			// ��
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
			sArrUnderText.Add("������");

			dArrDim.Add(-(pBri->m_footingEnd.m_dHeightNosang));
			sArrText.Add(bInput ? "H2" : COMMA(fabs(pBri->m_footingEnd.m_dHeightNosang)));
			sArrUnderText.Add("�����");
			
			dh = dy - pBri->m_footingEnd.m_dHeightPavement - pBri->m_footingEnd.m_dHeightNosang - pBri->m_footingEnd.m_dEL;
			dArrDim.Add(-(dh));
			sArrText.Add(COMMA(dh));
			sArrUnderText.Add("��ü��");

			baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, 0, sDir, bForUseBM ? 0 : -1, FALSE, FALSE, TRUE);

			// ��
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
	
	// ��� Slope ġ��
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
			
			long nIndex = pTvArr.GetIdxByInfo(_T("������ �̰�"));
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
// ������ü ���� ��� �������� ���� ����.
void CDrawVertSection::DrawVertSectionAbut(CDomyun *pDomP)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	CHgBaseDrawStd baseDraw;

	CTwinVectorArray tvArr;

	// ������ ////////////////////
	if(!pBri->IsOutWall(TRUE))
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_CONC);
		pBri->GetTvVertSectionAbut(tvArr, TRUE);
		baseDraw.DrawTvArray(&Dom, tvArr, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());

		// ���ö���
		CDRect rect	= tvArr.GetRect();
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_CUTL);
		Dom.CutLightningAnyDir(CDPoint(rect.left, rect.bottom), CDPoint(rect.right, rect.bottom), 500);

		// ��
		CTwinVector tv	= tvArr.GetTvByInfo("����ħ��");
		CJijum *pJ	= pBri->GetJijum(0);
		double dX	= tv.GetXyLeft().x + pJ->m_dLengthSlabToShoe[1];
		DrawSimpleShoe(&Dom, CDPoint(dX, tv.GetXyMid().y+400));
	}

	// ������ //////////////////////
	if(!pBri->IsOutWall(FALSE))
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_CONC);
		pBri->GetTvVertSectionAbut(tvArr, FALSE);
		baseDraw.DrawTvArray(&Dom, tvArr, m_pStd->IsViewVectorInfo(), m_pStd->IsViewVectorInfo());

		// ���ö���
		CDRect rect	= tvArr.GetRect();
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_CUTL);
		Dom.CutLightningAnyDir(CDPoint(rect.left, rect.bottom), CDPoint(rect.right, rect.bottom), 500);

		// ��
		CTwinVector tv	= tvArr.GetTvByInfo("����ħ��");
		CJijum *pJ	= pBri->GetJijum(pBri->m_nQtyJigan);
		double dX	= tv.GetXyLeft().x + pJ->m_dLengthSlabToShoe[0];
		DrawSimpleShoe(&Dom, CDPoint(dX, tv.GetXyMid().y+400));
	}

	*pDomP << Dom;
}

// �λ������ ��ö�� 
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
	CString strStt = bStt ? _T("����") : _T("����");
	
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
		//���� ��ġ ���ϱ�
		pRC->m_tvArrRebar.GetTvByInfo(strStt + _T("�λ�������ǻ��"), tvTop);
		pRC->m_tvArrRebar.GetTvByInfo(strStt + _T("�λ�������ǹٱ�����"), tvSide);
		pRC->m_tvArrRebar.GetTvByInfo(strStt + _T("�λ���������ϴ�"), tvBottom);
		CDPoint xyStt = tvBottom.m_v2 ;
		xyStt.x -= sw * pBri->m_dCoverWallIn[0];
		xyStt.y -= pBri->m_dCoverWallIn[0];

		//����ö�� ���������� ��ö�� ������ �Ÿ�
		double dSupportEnd = tvTop.m_v1.x + sw * ((nCountHori-1) * pBri->m_dPRF_CTC_Support);
		double dMainEnd = pBri->GetStationOnJijum(bStt ? 0 : pBri->m_nQtyJigan) - pBri->m_dCoverWallIn[0];
		double dDist = fabs(dMainEnd - dSupportEnd);
		CString strDimDir = bStt ? _T("LEFT") : _T("RIGHT");
		//��ö�� - �����ġ
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

		//��ö�� - ������ġ
		CDPointArray xyArrVert;
		dArrDim.RemoveAll();

		//�������� �� ö�ٵ鸸 ��󳽴�.
		for(long ix = 0; ix < xyArrSupport.GetSize(); ix++)
		{
			if(xyArrSupport.GetAt(ix).y < tvSide.m_v2.y - REBAR_SYMBOL_RADIUS*dScale/50.0&& xyArrSupport.GetAt(ix).y > tvSide.m_v1.y + REBAR_SYMBOL_RADIUS*dScale/50.0)
			{
				//��簡 �������� ������ x��ǥ�� �ѹ� Ȯ��..
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
		sDescription.Format("�λ�������� ��ö��%s", pBri->m_pARcBridgeDataStd->GetStringCircleNum(nCycle+1));
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
			if(pRC->m_tvArrRebar.GetTvByInfo(strStt+"�λ�������ǻ��", tvUpper))
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
// �λ������ ����ö�� 
void CDrawVertSection::DrawRebarSupport_VertSection_BFooting(CDomyun *pDomP, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, CDRect rectLimit)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	// ö�� �����ͼ� �׸���
	CHgBaseDrawStd baseDraw;
	CDPointArray xyArr;
	double dDomSacle = Dom.GetScaleDim();
	pBri->GetXyRebarSupport_VertSection_BFooting(xyArr, bStt, dDomSacle);
	long stt = bStt ? iSTT : iEND;
	CString strStt = bStt ? _T("����") : _T("����");
	double sw = bStt ? -1.0 : 1.0;
	double dScale = Dom.GetScaleDim();
	// rectLimit ����
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

	// ġ�� ����
	if(bDimRebar)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC =&pBri->m_pArrRCMainBoxLowerPRF_Cycle1[stt];

		if(pArrRC->GetSize() == 0) return;
		CRebarInfoCycle *pRC	= pArrRC->GetAt(0);
		CDPointArray xyArrSupport;
		CTwinVector tvSide;
		pRC->m_tvArrRebar.GetTvByInfo(strStt + _T("�λ�������ǹٱ�����"), tvSide);
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

			//�������ε� ����ö�� ����
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

//�λ������ ����ö�� 
void CDrawVertSection::DrawRebarShear_VertSection_BFooting(CDomyun *pDomP, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, long nCycle, CDRect rectLimit)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	CHgBaseDrawStd baseDraw;
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);
	// ö�� �����ͼ� �׸���
	CTwinVectorArray tvArrShear;
	long nCountShear = pBri->GetTvRebarShear_VertSection_BFooting(tvArrShear, bStt);
	baseDraw.DrawTvArray(&Dom, tvArrShear);

	CString sDescription	= _T("");
	sDescription.Format("%s �λ�������� ����ö��%s", bStt ? "����" : "����", pBri->m_pARcBridgeDataStd->GetStringCircleNum(nCycle+1));
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

// ����� ��ö�� 
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
	
	// RebarInfoCycle �׸���
	m_pStd->DrawRebarInfoCycle(&Dom, pRC, nDrawLeft, rectLimit);

	CString sDescription	= _T("");
	sDescription.Format("%s ����� ��ö��%s", bStt ? "����" : "����", pBri->m_pARcBridgeDataStd->GetStringCircleNum(nCycle+1));
	CString sMark			= pBri->GetMarkRebarEtc(BR_REBAR_M, bInput, FALSE, FALSE, sDescription);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		CString sStt		= bStt ? "����" : "����";
		CString sDia		= m_pStd->m_pARcBridgeDataStd->GetStringDia(nCycle == 0 ? pBri->m_dDiaRebarMain_Bracket_Cycle1[stt] : pBri->m_dDiaRebarMain_Bracket_Cycle2[stt], pBri->GetValueFy(bStt? ePartOutWallStt : ePartOutWallEnd));
		CDPoint vAngGisi	= bStt ? ToDPointFrDegree(45) : ToDPointFrDegree(135);
		CTwinVector tvHunch	;
		
		if(pRC->m_tvArrRebar.GetTvByInfo(sStt+"�������ġ", tvHunch))
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

// ����� ��±�
void CDrawVertSection::DrawRebarSupport_VertSection_Bracket(CDomyun *pDomP, BOOL bStt, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, CDRect rectLimit)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	// ö�� �����ͼ� �׸���
	CHgBaseDrawStd baseDraw;
	CDPointArray xyArr;
	pBri->GetXyRebarSupport_VertSection_Bracket(xyArr, bStt);
	
	// rectLimit ����
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
	
	// ġ�� ����
	if(bDimRebar)
	{

	}

	// ��ŷ
	CString sDescription	= _T("");
	sDescription.Format("%s ����� ���ö��", bStt ? "����" : "����");
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

	// ö�� �����ͼ� �׸���
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

			CTwinVector tv	= pBri->m_tvArrVertSection.GetTvByInfo(bStt ? "��������ϻ��" : "��������ϻ��");

			xyArrDim.Add(CDPoint(xyStt.x, rect.top));
			sArrText.Add("0");
			sArrUnderText.Add("0");

			xyArrDim.Add(CDPoint(xyStt.x, tv.m_v1.y));
			sArrText.Add("0");
			sArrUnderText.Add("0");

			baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, TRUE, FALSE);
		}
	}

	// ����ö�� �׸���
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);
	m_pStd->DrawTvArrayWidthDashDot(&Dom, tvArr, FALSE);

	CString sDescription	= _T("");
	sDescription.Format("%s ����� ����ö��", bStt ? "����" : "����");
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

// �������� ��ġ�� ǥ�� �׸���
// ����� �׳� ���׶�̷� �׸�
// BOOL bLeft : V�� ��ü�� ���� �̸� ������
void CDrawVertSection::DrawSignHunchByJijum(CDomyun *pDomP, long nJijum, BOOL bLeft, BOOL bUpper)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(nJijum < 0 || nJijum > pBri->m_nQtyJigan) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	// �� ����
	CDPoint xyCen	= CDPoint(0, 0);
	double dR		= 0;

	// �߽���ǥ ���ϱ� /////////////////////////////////
	if(nJijum == 0 || nJijum == pBri->m_nQtyJigan)
	{
		BOOL bStt	= nJijum == 0;
		CString sStt = _T("");

		if(!pBri->IsOutWall(bStt))
		{
			sStt	= bStt ? "��������������" : "��������������";
			CTwinVector tv	= pBri->m_tvArrVertSection.GetTvByInfo(sStt);
			xyCen	= tv.GetXyMid();
		}
		else
		{
			double ul = bUpper ? -1.0 : 1.0;
			double dThickSlab = bUpper ? pBri->m_dTS : pBri->m_dTS_Lower;

			if(bUpper) 	
				sStt	= bStt ? "��������(������ü��)" : "��������(������ü��)";
			else	
				sStt	= bStt ? "�Ϻν������ϸ�(������ü�Ʒ�)" : "�Ϻν������ϸ�(������ü�Ʒ�)";

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
			strL.Format("%d��°������ġ������", nIdxHunch+1);
			strR.Format("%d��°������ġ������", nIdxHunch+1);
		}
		else
		{
			strL.Format("%d��°�����Ϻ���ġ������", nIdxHunch+1);
			strR.Format("%d��°�����Ϻ���ġ������", nIdxHunch+1);
		}

		CTwinVector tvL	= pBri->m_tvArrVertSection.GetTvByInfo(strL),
					tvR	= pBri->m_tvArrVertSection.GetTvByInfo(strR);
		xyCen	= (tvL.GetXyLeft() + tvR.GetXyRight())/2;
	}

	// �������� �� ������ ���� ū ���� ����� ///////////
	// ����
	CTwinVector tv	= pBri->m_tvArrVertSection.GetTvByInfo("������ü��ġ");
	dR				= max(tv.GetVerLength(), tv.GetHorLength());

	// ����
	tv	= pBri->m_tvArrVertSection.GetTvByInfo("������ü��ġ");
	dR				= max(dR, max(tv.GetVerLength(), tv.GetHorLength()));

	// �߰�����
	long nCountInWall	= pBri->GetCountInWall();
	for(long wall = 0; wall < nCountInWall; wall++)
	{
		long nIdxHunch	= pBri->GetIdxHunchInWall(wall, bLeft);
		CString strL	= _T(""),
				strR	= _T("");
		strL.Format("%d��°������ġ������", nIdxHunch+1);
		strR.Format("%d��°������ġ������", nIdxHunch+1);

		CTwinVector tvL	= pBri->m_tvArrVertSection.GetTvByInfo(strL),
					tvR	= pBri->m_tvArrVertSection.GetTvByInfo(strR);
		double dLenH	= fabs(tvL.GetXyLeft().x - tvR.GetXyRight().x) / 2;
		double dLenV	= tvL.GetVerLength();
		dR		= max(dR, max(dLenH, dLenV));
	}

	// �׸�
	Dom.Circle(xyCen, dR, FALSE);
	
	*pDomP << Dom;
}

// ���ٸ� ġ�� ����(������¿�)
void CDrawVertSection::DimPostForBM(CDomyun *pDomP)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	CARcBridgeDataStd *pDataStd	= m_pStd->m_pARcBridgeDataStd;
	if(!pDataStd) return;
	
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->ApplyDimSetting(pDomP, &Dom);

	// ������ ���ٸ� ũ��
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

	// ��, ��
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		CString sStt	= bStt ? "����" : "����";
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
			if(nTypePost==0)	// ������
			{
				if(pBri->m_nTypePostBaseLine == 2)
					dArrY.Add(frM(pBri->m_dPostLineEL)+dWidthHeight);
			}
			if(tvArrPost.GetSize()>0)
				dArrY.Add(tvArrPost.GetXyTop().y);
			if(pBri->m_tvArrVertSection.GetTvByInfo(sStt+"����ϻ��", tv))
				dArrY.Add(tv.GetXyLeft().y);
			if(pBri->m_tvArrVertSection.GetTvByInfo(sStt+"����ϻ������", tv))
				dArrY.Add(tv.GetXyTop().y);

			bTotal	= TRUE;
			if(bBoxType)
			{
				CString sEnd = _T("");
				if(pBri->m_bExistProtectionRiseFooting)
					sEnd.Format(_T("%s�λ�������ǹٱ�����"), sStt);
				else
					sEnd.Format(_T("�Ϻν������ϸ�(%s��ü�Ʒ�)"), sStt);
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
			// �ϴ� ��, ���������� ���ݰ� ���ϱ�
			CTwinVector tvHunchL, tvHunchR;
			CLineInfo *pLine = pBri->GetLineBase();
			pBri->m_tvArrVertSection.GetTvByInfo("��������������", tvHunchL);
			tvHunchL.m_v1	= tvHunchL.GetXyBottom();
			tvHunchL.m_v2	= tvHunchL.GetXyBottom();
			pBri->m_tvArrVertSection.GetTvByInfo("��������������", tvHunchR);
			tvHunchR.m_v1	= tvHunchR.GetXyBottom();
			tvHunchR.m_v2	= tvHunchR.GetXyBottom();
			// ����
			CFootingApp *pFooting = pBri->GetFooting( bStt ? 0 : nCountFooting-1);
			if(pFooting)
			{
				dArrY.Add(pFooting->m_exFooting.m_tvArrFront.GetXyBottom().y);
				dArrY.Add(pFooting->m_tvArrFront.GetXyBottom().y);
				
				// ���� ����� ū�� ���ݰ� ū��?
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
				pBri->m_tvArrVertSection.GetTvByInfo(sStt+"����������", tv);
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
	
	// ��
	CDoubleArray dArrX;
	dArrDim.RemoveAll();
	if(pBri->IsOutWall(TRUE))
	{
		if(pBri->m_tvArrVertSection.GetTvByInfo("��������ϻ��", tv))
		{
			dArrX.Add(tv.GetXyLeft().x);
			dArrX.Add(tv.GetXyRight().x);
		}
		if(pBri->m_tvArrVertSection.GetTvByInfo("������ü������", tv))
			dArrX.Add(tv.m_v2.x);
	}
	else
	{
		if(pBri->m_tvArrVertSection.GetTvByInfo("��������������", tv))
			dArrX.Add(tv.m_v2.x);
		CFootingApp *pFooting = pBri->GetFooting(0);
		if(pFooting && pFooting->m_tvArrFront.GetTvByInfo("����������", tv))
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
		if(pWall->m_tvArrFront.GetTvByInfo("��ü������", tv))
			dArrX.Add(tv.m_v2.x);
		if(!bOK)
		{
			long nSize = tvArrUpper.GetSize();
			tv = tvArrUpper.GetAt(nSize-1);
			dArrX.Add(tv.GetXyRight().x);
		}		
		if(pWall->m_tvArrFront.GetTvByInfo("��ü������", tv))
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
			if(pBri->m_tvArrVertSection.GetTvByInfo("������ü������", tv))
				dArrX.Add(tv.m_v1.x);

			if(pBri->m_tvArrVertSection.GetTvByInfo("��������ϻ��", tv))
			{
				dArrX.Add(tv.GetXyLeft().x);
				dArrX.Add(tv.GetXyRight().x);
			}
		}
	}
	else
	{
		CFootingApp *pFooting = pBri->GetFooting(nCountFooting-1);
		if(pFooting && pFooting->m_tvArrFront.GetTvByInfo("���ʿ�����", tv))
			dArrX.Add(tv.m_v1.x);
		if(pBri->m_tvArrVertSection.GetTvByInfo("��������������", tv))
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
// 		// �ϸ� ġ��
// 		CString sEnd = _T("");
// 		for(long nstt=0; nstt<2; nstt++)
// 		{
// 			if(pBri->m_bExistProtectionRiseFooting)
// 				sEnd.Format(_T("%s�λ���������ϴ�"), sStt);
// 			else
// 				sEnd.Format(_T("�Ϻν�����(%s��ü�Ʒ�)"), sStt);
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

		// ��
		dArrDim.RemoveAll();
		dArrDim.Add(tvArr.GetHorLength());
		sDir	= _T("TOP");
		xyStt.x	= tvArr.GetXyLeft().x;
		xyStt.y	= tvArr.GetXyTop().y;

		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, 500, FALSE, FALSE, FALSE);

		// ���� 
		dArrDim.RemoveAll();
		dArrDim.Add(tvArr.GetVerLength());
		sDir	= bStt ? _T("LEFT") : _T("RIGHT");
		xyStt.x	= bStt ? tvArr.GetXyLeft().x : tvArr.GetXyRight().x;
		xyStt.y	= tvArr.GetXyBottom().y;

		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, 500, FALSE, FALSE, FALSE);
	}

	*pDomP << Dom;
}

// ���� �׸���
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
			// ���ö���
			CDRect rect	= tvArr.GetRect();
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_CUTL);
			Dom.CutLightningAnyDir(CDPoint(rect.left, rect.bottom), CDPoint(rect.right, rect.bottom), 500);

			// ��
			DrawSimpleShoe(&Dom, CDPoint((rect.left+rect.right)/2, rect.top+400));
		}
	}

	*pDomP << Dom;
}

// xy : �� ��� �߽�
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

// ���Ʈ��/���ܷµ� �׸���
// ���̾�׷��� ������ : x : ��ü�� �ϳ��� �ִٸ� ��ü�� �߰�, ������  ������ ������
//                       y : ��� ������ ���� ���� ���� �ִ� ������ ���
// BOOL bWhenDiagramIsNoneDrawRect : ���̾�׷��� �ȱ׷� ������ �׳� �簢�� �׸���?
CDRect CDrawVertSection::DrawDiagramBMD_SFD(CDomyun *pDomP, BOOL bBMD, BOOL bSFD, BOOL bAXIAL, BOOL bWhenDiagramIsNoneDrawRect, BOOL bInput)
{
	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
	if(!pBri) return CDRect(0, 0, 0, 0);
	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	CDRect rectOrg(0, 0, 0, 0);
	if(bBMD == FALSE && bSFD == FALSE && bAXIAL == FALSE) return rectOrg;

	pDomP->SetCalcExtRect();
	rectOrg	= pDomP->GetExtRect();

	// ������ ã�� ////////////////////////////
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


// ��� ������ �β� ġ��
// ���� �߾Ӹ��� �ϳ��� ����
void CDrawVertSection::DimVertSectionUpperSlabThick(CDomyun *pDomP, BOOL bDimPave, long nLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	
	CDomyun Dom(pDomP);
	
	m_pStd->ApplyDimSetting(pDomP, &Dom);
	
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	Dom.SetDirection("RIGHT");
	
	CTwinVector tv;

	// �Ϲݵ��� ���Ÿ��� �� �׸��Ƿ� �׻� FALSE
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

		// ������ �β�
		Dom.DimMoveTo(xy.x, xy.y - pBri->m_dTS, 0);
		Dom.DimLineTo(pBri->m_dTS, 0, COMMA(pBri->m_dTS));

		// ��ũ��Ʈ �β�
		if(bDimPave)
		{
			if(pBri->GetThickPave(FALSE) > 0)
				Dom.DimLineToOver(pBri->GetThickPave(FALSE), 0, COMMA(pBri->GetThickPave(FALSE)));
		}
	}
	
	*pDomP << Dom;
}

// �⺻���� ġ�� �����ѵ� ���ܻ��׿� ���� ġ�� ��������
// 1, �����ϸ� ����/������ ġ�� ����
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

// ��� ������ ������ ���� ��ö�� ������ ġ��
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

		CTwinVector tvSide	= pBri->m_tvArrVertSection.GetTvByInfo("��������������");
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

		CTwinVector tvSide	= pBri->m_tvArrVertSection.GetTvByInfo("��������������");
		xyStt	= tvSide.GetXyBottom();
		
		dArrDim.Add(pBri->m_dCoverLower_UpperSlab[0]);
		dArrDim.Add(pBri->m_dTS + pBri->m_dHHE - (pBri->m_dCoverLower_UpperSlab[0] + pBri->m_dCoverUpper_UpperSlab[0]));
		dArrDim.Add(pBri->m_dCoverUpper_UpperSlab[0]);

		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, FALSE, FALSE);
	}

	*pDomP << Dom;
}

// ���� ǥ�ÿ� �ﰢ�� �׸���
// �Ŵ��� �׻� ��Ÿ��� �Է� �����Ƿ� ��Ÿ��� �׸�.
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

// �Ǻ� �Է¿� 
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
	
	// ������ ���
	if(sArea == "������ ���" || sArea == "������ �ϸ�")
	{
		Dom.SetDimObq(30);
		Dom.SetDimExo(0);
		sDir	= "RIGHT";
		dArr.Add(pBri->m_dCoverLower_UpperSlab[0]);
		sArrText.Add("������ �ϸ�");

		dArr.Add(pBri->m_dTS-pBri->m_dCoverLower_UpperSlab[0]-pBri->m_dCoverUpper_UpperSlab[0]);
		sArrText.Add("������ �β�");

		dArr.Add(pBri->m_dCoverUpper_UpperSlab[0]);
		sArrText.Add("������ ���");

		double dStaMid	= (pBri->GetStationOnJijum(0) + pBri->GetStationOnJijum(1))/2;
		xyStt	= pBri->GetXyMatchLineAndSlabUpper(CDPoint(dStaMid, 0), CDPoint(0, 1), 0, pBri->m_dTS, FALSE);

		baseDraw.AutoDimCont(&Dom, xyStt, dArr, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
	}
	else if(sArea == "�Ϻν����� ���" || sArea == "�Ϻν����� �ϸ�")
	{
		Dom.SetDimObq(30);
		Dom.SetDimExo(0);
		sDir	= "RIGHT";
		dArr.Add(pBri->m_dCoverLower_LowerSlab[0]);
		sArrText.Add("������ �ϸ�");

		dArr.Add(pBri->m_dTS_Lower-pBri->m_dCoverLower_LowerSlab[0]-pBri->m_dCoverUpper_LowerSlab[0]);
		sArrText.Add("������ �β�");

		dArr.Add(pBri->m_dCoverUpper_LowerSlab[0]);
		sArrText.Add("������ ���");

		double dStaMid	= (pBri->GetStationOnJijum(0) + pBri->GetStationOnJijum(1))/2;
		xyStt	= pBri->GetXyMatchLineAndLowerSlabLower(CDPoint(dStaMid, 0), CDPoint(0, 1), 0, 0, FALSE);

		baseDraw.AutoDimCont(&Dom, xyStt, dArr, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
	}
	else if(sArea == "������ ����")
	{
		sDir	= "TOP";
		CTwinVector tvStt	= pBri->m_tvArrVertSection.GetTvByInfo(!pBri->IsOutWall(TRUE) ? "��������������" : "��������ϻ������");
		CTwinVector tvEnd	= pBri->m_tvArrVertSection.GetTvByInfo(!pBri->IsOutWall(FALSE) ? "��������������" : "��������ϻ������");

		dArr.Add(pBri->GetCoverSide_UpperSlab(0));
		sArrText.Add("������ ����");

		dArr.Add((tvEnd.m_v1.x - tvStt.m_v2.x) - pBri->GetCoverSide_UpperSlab(0) * 2);
		sArrText.Add("0");

		dArr.Add(pBri->GetCoverSide_UpperSlab(0));
		sArrText.Add("������ ����");

		xyStt	= tvStt.GetXyTop();
		baseDraw.AutoDimCont(&Dom, xyStt, dArr, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
	}

	else if(sArea == "��ü �ܸ�" || sArea == "��ü ����")
	{
		if(!pBri->IsOutWall(TRUE)) return;
		sDir	= "BOTTOM";
		CTwinVector tvOutter	= pBri->m_tvArrVertSection.GetTvByInfo("������ü������");

		dArr.Add(pBri->m_dCoverWallOut[0]);
		sArrText.Add("��ü �ܸ�");

		dArr.Add(pBri->m_dWS - pBri->m_dCoverWallOut[0] - pBri->m_dCoverWallIn[0]);
		sArrText.Add("0");

		dArr.Add(pBri->m_dCoverWallIn[0]);
		sArrText.Add("��ü ����");

		xyStt	= tvOutter.GetXyMid();

		baseDraw.AutoDimCont(&Dom, xyStt, dArr, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
	}

	else if(sArea == "������ ����(������)")
	{
		CTwinVector tvSide;
		sDir	= "TOP";
		if(pBri->m_tvArrVertSection.GetTvByInfo("��������������", tvSide))
		{
			dArr.RemoveAll();
			sArrText.RemoveAll();

			dArr.Add(pBri->m_dCoverWallOut[0]);
			sArrText.Add("������ ����(������)");

			xyStt	= tvSide.GetXyTop();
			baseDraw.AutoDimCont(&Dom, xyStt, dArr, sArrText, sArrUnderText, nDimDan, sDir, 0, FALSE, FALSE, FALSE);
		}

		if(pBri->m_tvArrVertSection.GetTvByInfo("��������������", tvSide))
		{
			dArr.RemoveAll();
			sArrText.RemoveAll();

			dArr.Add(pBri->m_dCoverWallOut[0]);
			sArrText.Add("������ ����(������)");

			xyStt	= tvSide.GetXyTop();
			xyStt.x -= pBri->m_dCoverWallOut[0];
			baseDraw.AutoDimCont(&Dom, xyStt, dArr, sArrText, sArrUnderText, nDimDan, sDir, 0, FALSE, FALSE, FALSE);
		}
	}

	else if(sArea == "���� ��ü")
	{
		sDir	= "BOTTOM";
		CWallApp *pWall	= pBri->GetInWall(0);
		if(pWall)
		{
			if(pWall->m_bIs)
			{
				CTwinVector tvLeft	= pWall->m_tvArrFront.GetTvByInfo("��ü������");

				dArr.Add(pBri->m_dCoverWallIn[0]);
				sArrText.Add("���� ��ü");

				dArr.Add(pWall->m_dW - (pBri->m_dCoverWallIn[0] * 2));
				sArrText.Add("0");

				dArr.Add(pBri->m_dCoverWallIn[0]);
				sArrText.Add("���� ��ü");

				xyStt	= tvLeft.GetXyMid();
				baseDraw.AutoDimCont(&Dom, xyStt, dArr, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
			}
		}
	}

	else if(sArea == "���� ���")
	{
		sDir	= "LEFT";
		CFootingApp *pFooting	= pBri->GetFooting(0);
		if(pFooting)
		{
			if(pFooting->m_bIs)
			{
				CTwinVector tvLower	= pFooting->m_tvArrFront.GetTvByInfo("�����ϸ�");
				
				dArr.Add(pFooting->m_dCoverLower[0]);
				sArrText.Add("0");

				dArr.Add(pFooting->GetHeight());
				sArrText.Add("0");

				dArr.Add(pFooting->m_dCoverUpper[0]);
				sArrText.Add("���� ���");

				xyStt	= tvLower.GetXyLeft();
				baseDraw.AutoDimCont(&Dom, xyStt, dArr, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
			}
		}
	}

	else if(sArea == "���� �ϸ�(��������)")
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
			CTwinVector tvLower		= pFooting->m_tvArrFront.GetTvByInfo("�����ϸ�");
			
			dArr.Add(pBri->m_dCoverLower_Footing_Lean[0]);
			sArrText.Add("���� �ϸ�(��������)");

			dArr.Add(pFooting->GetHeight() - pBri->m_dCoverLower_Footing_Lean[0] - pFooting->m_dCoverUpper[0]);
			sArrText.Add("0");

			dArr.Add(pFooting->m_dCoverUpper[0]);
			sArrText.Add("0");

			xyStt	= tvLower.GetXyLeft();
			baseDraw.AutoDimCont(&Dom, xyStt, dArr, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, FALSE, FALSE);
		}
	}

	else if(sArea == "���� �ϸ�(���ұ���)")
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
			CTwinVector tvLower		= pFooting->m_tvArrFront.GetTvByInfo("�����ϸ�");
			
			dArr.Add(pBri->m_dCoverLower_Footing_Pile[0]);
			sArrText.Add("���� �ϸ�(���ұ���)");

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
	double dStaCurRightAng	= pBri->GetStationOnJijum(0, nLeft, TRUE),	// ���Ÿ� ����
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

		// ���� 0�϶� ���������� �˻�.
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
				// �������� ����
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

				// ������ ����
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
				// �������� ����
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

				// �������� ��
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

	// ��ü ġ��
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

// ���ܸ� ��ܿ� slope ����
// void CDrawVertSection::DimTextVertSectionSlope(CDomyun *pDomP)
// {
// 	CRcBridgeApp *pBri = m_pStd->m_pRcBridge;
// 	if(!pBri) return;
// 	CDomyun Dom(pDomP);
// 	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
// 
// 	// ���ܸ� �߰��� ����
// 
// 	*pDomP << Dom;
// }

// ���ܸ鿡 ���� �߽ɼ� �׸�
// ��米�� ��쿡�� �׸���.
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
		// ��ν�����
		CTwinVectorArray tvArr;
		CTwinVector tvCen;
		tvCen.m_v1		= pBri->GetXySectionSttEnd(TRUE, 0);
		tvCen.m_v2		= pBri->GetXySectionSttEnd(FALSE, 0);
		if(pBri->IsOutWall(TRUE))
		{
			CTwinVector tvLeft	= pBri->m_tvArrVertSection.GetTvByInfo("������ü������");
			CTwinVector tvRight	= pBri->m_tvArrVertSection.GetTvByInfo("������ü������");
			tvCen.m_v1	= pBri->GetXySlabUpperByOffsetFrStation((tvLeft.m_v1.x+tvRight.m_v2.x)/2, 0, 0);
		}

		if(pBri->IsOutWall(FALSE))
		{
			CTwinVector tvLeft	= pBri->m_tvArrVertSection.GetTvByInfo("������ü������");
			CTwinVector tvRight	= pBri->m_tvArrVertSection.GetTvByInfo("������ü������");
			tvCen.m_v2	= pBri->GetXySlabUpperByOffsetFrStation((tvLeft.m_v1.x+tvRight.m_v2.x)/2, 0, 0);
		}

		tvCen.m_v1.y	-= pBri->m_dTS/2;
		tvCen.m_v2.y	-= pBri->m_dTS/2;
		tvCen.m_v2.z	= 2;
		tvArr.Add(tvCen);
		baseDraw.DrawTvArray(&Dom, tvArr, FALSE, FALSE, 0);

		// ������ü
		long stt = 0; for(stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			if(!pBri->IsOutWall(bStt)) continue;
			CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;

			CTwinVector tvLower	= pFooting->m_tvArrFront.GetTvByInfo("�����ϸ�");
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

			CTwinVector tv	= pWall->m_tvArrFront.GetTvByInfo("��ü�ϸ�����");
			tvCen.m_v1	= tv.m_v1;
			tvCen.m_v2	= pBri->GetXyMatchLineAndSlabUpper(tvCen.m_v1, ToDPointFrDegree(pWall->GetAngleWallByHor(bApplyAngle)), 0, pBri->m_dTS/2, FALSE);

			if(pWall->m_footing.m_bIs || pWall->m_footing.IsVirtualFooting())
			{
				CTwinVector tvLower	= pWall->m_footing.m_tvArrFront.GetTvByInfo("�����ϸ�");
				GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), tvCen.m_v1, tvCen.GetXyDir(TRUE), tvCen.m_v1);
			}
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_CENT);
			baseDraw.DrawTv(&Dom, tvCen);

			// ���� ġ���� ����
			if(bDimAngle)
			{
				m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_DIML);
				// 1. ��� ��������� ����
				double dAngWall		= pWall->GetAngleWallByHor(bApplyAngle);
				CDPoint vAngWall	= ToDPointFrDegree(dAngWall);
				double dAngSlab		= pWall->GetAngleUpperSlab();
				CDPoint vAngSlab	= ToDPointFrDegree(dAngSlab);
				double dR	= Dom.Always(3);

				// ����
				CDPoint xyOrg	= tvCen.m_v2;
				double dAng		= (dAngWall + 180) - (dAngSlab + 180);
				double dAngVert	= pWall->m_dAngle-pWall->m_dAngleUpperSlab_Front;

				if(bInputCurJijum)
					sym.DrawAngleNotation(&Dom, xyOrg, dAngSlab+180, dAngWall+180, dR, 0, !bInput, _T("A"), dAng);
				else
					sym.DrawAngleNotation(&Dom, xyOrg, dAngSlab+180, dAngWall+180, dR, 0, !bInput, _T(""), dAngVert);

				// ��
				dAng	= dAngSlab + 360 - (dAngWall + 180);
				xyOrg	= tvCen.m_v2;
				dAngVert	= 180-pWall->m_dAngle+pWall->m_dAngleUpperSlab_Front;

				if(bInputCurJijum)
					sym.DrawAngleNotation(&Dom, xyOrg, dAngWall+180, dAngSlab, dR, !bInput, 0, _T("A"), dAng);
				else
					sym.DrawAngleNotation(&Dom, xyOrg, dAngWall+180, dAngSlab, dR, !bInput, 0, _T(""), dAngVert);


				// 2. �������� ����
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

// ������ü ��ġ �׸���
void CDrawVertSection::DrawRebarMain_VertSection_OutWall_Hunch(CDomyun *pDomP, long nCycle, BOOL bMarkRebar, long nDrawLeft, CDRect rectLimit, BOOL bInput, BOOL bForStandardSection, long nVertDir)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	if(nCycle == -1) nCycle = pBri->GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > pBri->GetCountCycleMainRebar()-1) return;
	
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	// ������ ��ü
	BOOL bStt = TRUE;
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		bStt	= (stt == iSTT);
		CRebarInfoCycle *pRC	= &pBri->m_rebarInfoCycleOutWallHunch[nCycle][stt];

		// RebarInfoCycle �׸���
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
		m_pStd->DrawRebarInfoCycle(&Dom, pRC, nDrawLeft, rectLimit, TRUE);

		// ö�� ��ũ
		if(bMarkRebar)
		{
			*pDomP << Dom;
			// ��ũ����(��������)
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

	// ö�� �����ͼ� �׸���
	CHgBaseDrawStd baseDraw;
	CDPointArray xyArr;
	pBri->GetXyRebarSupport_VertSection_OutWallHunch(xyArr, bStt, nVertDir);
	CTwinVector tvHunch;
	if(!pBri->m_tvArrVertSection.GetTvByInfo(bStt ? "������ü�Ϻ���ġ" : "������ü�Ϻ���ġ", tvHunch)) return;
	CDPoint xyDirScale	= tvHunch.GetXyDir().RotateInv90();

	// rectLimit ����
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

	// ��ŷ
	CString sDescription	= _T("");
	sDescription.Format("%s ��ġ�� ���ö��", bStt ? "��������ü" : "��������ü");
	CString sMark			= pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescription);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		CRebar *pRb = pBri->GetRebarSupportByDescription(sDescription);
		CPlaceRebarByBlock *pPlace		= &pBri->m_placeByBlock_SupportRebar_OutWall[stt][iINNER][0];
		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
		mark.SetRebar(xyArr);

		// ����
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

// ����� ����
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
	double eleBridgeMax = pLine->GetMaxSectionEL(staBridgeStt, staBridgeEnd);   // ���� ������ �ִ� ��ȹ��
	double eleBridgeStt = pLine->GetElevationPlanEl(staBridgeStt); // ���� ������ ��ȹ��
	double eleBridgeEnd = pLine->GetElevationPlanEl(staBridgeEnd); // ���� ������ ��ȹ��
	// ���� ���� ǥ��
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
		Dom.TextOut(staBridgeStt - ((lenStrSta / 2) + dTextHeight), (eleBridgeMax * scaleVertical) + lenDimHeight + dTextHeight * 0.3, "��������");
		Dom.TextOut(staBridgeStt - ((lenStrSta / 2) + dTextHeight), (eleBridgeMax * scaleVertical) + lenDimHeight - dTextHeight * 1.3,  strSta);
		Dom.Rotate(CDPoint(staBridgeStt, (eleBridgeMax * scaleVertical) + lenDimHeight), CDPoint(2,-1));
		*pDomP << Dom;

		// ���� ���� ǥ��
		double dLength = staBridgeEnd - staBridgeStt; // ���� ��ü ����(���� ����)
		CString strLengthBridge;
		strLengthBridge.Format("L = %.3lf", toM(dLength));
		Dom.MoveTo(staBridgeStt, (eleBridgeMax * scaleVertical) + ((lenStrSta / 2) + (dTextHeight * 2)));
		Dom.LineTo(staBridgeEnd, (eleBridgeMax * scaleVertical) + ((lenStrSta / 2) + (dTextHeight * 2)));

		Dom.Circle(staBridgeStt, (eleBridgeMax * scaleVertical) + ((lenStrSta / 2) + (dTextHeight * 2)), dTextHeight/4, TRUE);
		Dom.Circle(staBridgeEnd, (eleBridgeMax * scaleVertical) + ((lenStrSta / 2) + (dTextHeight * 2)), dTextHeight/4, TRUE);

		Dom.SetTextAngle(0);
		Dom.SetTextAlignHorz(TA_CENTER);
		Dom.TextOut(staBridgeStt + dLength / 2, (eleBridgeMax * scaleVertical) + ((lenStrSta / 2) + (dTextHeight * 2)) + (dTextHeight * 0.3),   "��������");
		Dom.TextOut(staBridgeStt + dLength / 2, (eleBridgeMax * scaleVertical) + ((lenStrSta / 2) + (dTextHeight * 2)) - (dTextHeight * 1.3), strLengthBridge);

		// ���� ���� ǥ��
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
		Dom.TextOut(staBridgeEnd + (lenStrSta/2 + dTextHeight), (eleBridgeMax * scaleVertical) + lenDimHeight + dTextHeight * 0.3, "��������");
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

// ��ν����� �а��� ö��(Ⱦ�������� ����� ��ö�ٵ� ���� �׸���)
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

					// ��� ġ�� ���Խô� �Ǻ� ġ���� ���� �־���
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

	// ��ö��
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

	// ��ö���� �Է����ΰ�� ������ü�� �ִ� ö���� ġ���� �����Ѵ�.
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

// ���ܸ� �а��� ���� �� ���
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
	
	// ö��
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
	// ö�� �����ͼ� �׸���
	CHgBaseDrawStd baseDraw;
	CDPointArray xyArr;
	pBri->GetXyRebarSupport_VertSection_OutWallAcuteAngle(xyArr, bStt, bLeft, nVertDir);
	CDPoint xyDirScale	= bStt ? CDPoint(-1, 0) : CDPoint(1, 0);
	baseDraw.DrawPointRebar(&Dom, xyArr, xyDirScale);

	// ġ�� ����
	if(bDimRebar)
	{
		if(xyArr.GetSize() > 0)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

			CDPointArray xyArrDim;
			CDPoint xyStt	= xyArr.GetAt(0);
			CStringArray sArrText, sArrUnderText;
			CString sDir	= bStt ? _T("LEFT") : _T("RIGHT");
			
			// ġ���� �Ǻ��� R���� �߰��ؾ� ��. //////////////////////
			CTwinVector tvLower, tvUpper;
			CRebarInfoCycle *pRCLower	= NULL;
			CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;
			
			// ���� �߰�(���ۺ�)
			if(pFooting->m_bIs)
			{
				if(pFooting->m_tvArrFront.GetTvByInfo("�����ϸ�", tvLower))
				{
					xyArrDim.Add(tvLower.m_v1);
				}
			}
			pRCLower	= &pBri->m_rebarInfoCycle_AcuteAngle_OutWall[stt][left];

			xyArrDim.Add(pRCLower->m_xyMid1);
			xyArrDim.Add(pRCLower->m_xyMid2);

			long i = 0; for(i = 0; i < xyArr.GetSize(); i++)
				xyArrDim.Add(xyArr.GetAt(i));

			// ġ�� ����
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


	// ��ŷ
	CString sDescription	= _T("");
	CString sStt	= bStt ? "����" : "����";
	CString sLeft	= bLeft ? "����" : "����";
	sDescription.Format("%s%s��ü ������ ���ö��", sStt, sLeft);
	CString sMark			= pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescription);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		CRebar *pRb = pBri->GetRebarSupportByDescription(sDescription);
		CPlaceRebarByBlock *pPlace		= &pBri->m_placeByBlock_AcuteAngle_SupportRebar_OutWall[stt][left];
		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
		mark.SetRebar(xyArr);

		// ����
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

// ���� ���� ǥ��
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
		// �׽�Ʈ
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
		str[3].Format("f = %.3f (��ġ ������)", dRibHeightTotal);
		str[4].Format("l = %.3f (��ġ ����)", dRibDistTotal);
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

// ���� ����
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

	// ġ�����Կ� ����
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

	// ö�� �����ͼ� �׸���
	CHgBaseDrawStd baseDraw;
	CDPointArray xyArr[2];
	CDPointArray xyArrDir[2];
	pBri->GetXyRebarSupport_VertSection_ArchRib(xyArr[iLEFT], xyArrDir[iLEFT], xyArr[iRIGHT], xyArrDir[iRIGHT], bUpper, nDan, nVertDir, TRUE);

	long left = 0; for(left = 0; left < 2; left++)
	{
		BOOL bLeft	= left == iLEFT;

		// rectLimit ����
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

		// ��ŷ
		CString sDescription	= _T("");
		if(bUpper)
			sDescription.Format("��ġ���� �輱 %d�� ���ö��", nDan+1);
		else
			sDescription.Format("��ġ���� ���� %d�� ���ö��", nDan+1);
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

			// ����
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
	
		// ġ�� ����
		// ġ�� ���� �Ҷ��� ���� �� �ٵ��� ������ �Űܾ� �ȴ�.
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

					// ��ġ��
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

					// ��ġ��
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
	
	// ö�� �����ͼ� �׸���
	CTwinVectorArray tvArr[2];
	pBri->GetTvRebarShear_VertSection_ArchRib(tvArr[iLEFT], tvArr[iRIGHT], nVertDir);

	long left = 0; for(left = 0; left < 2; left++)
	{
		BOOL bLeft	= left == iLEFT;

		// rectLimit ����
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

		// ���� ö�� �׸���
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);
		m_pStd->DrawTvArrayWidthDashDot(&Dom, tvArr[left], FALSE);

		if(bLeft)
		{
// 			CString sDescription	= _T("");
			CString sDescription	= "��ġ���� ����ö��";
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

				// ���׸���
				double xDisFir	= pbRebarC.GetDistByXPos(tvArr[iLEFT].GetAt(tvArr[iLEFT].GetSize()-1).GetXyMid().x);
				double xDisLast	= pbRebarC.GetDistByXPos(tvArr[iRIGHT].GetAt(tvArr[iRIGHT].GetSize()-1).GetXyMid().x);
				CHgBaseDrawStd baseDraw;
				m_pStd->DrawParabola(&Dom, &pbRebarC, xDisFir, xDisLast, 0);

				// ��ȣ ���
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

					// ��ġ��
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

										// ��ġ��
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

	// ���ܸ鵵 �׸���
	// ���� ������ �⺻������
	pBri->m_footingEnd.m_exFooting.m_Pile.InitScaleCutting();
	// ��� ��ü���� ����ȭ�� ����.
	//pBri->SyncBridge(nLeft, !bVertDir);
	pBri->GetTvVertSection_Std(nLeft, !bVertDir, FALSE);
	pBri->SyncBridgeRebar(FALSE, nVertDir);

	// ��ġ���� ���ܸ� �׸�
	// ���ʴ� ��ü�� overlap üũ �Ѱ� �����ؼ� �׸���.
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


	// ��ö�� 1cycle �׸���
	for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		DrawRebarMain_VertSection_ArchRib(&Dom, 0, dan, TRUE, FALSE, bMarkMainRebar, 0, CDRect(0, 0, 0, 0), FALSE, nVertDir);
	}
	
	// ���ö��/����ö�� �׸���
	for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		for(upper = 0; upper < 2; upper++)
		{
			bUpper = (upper==iUPPER);
			// ����
			DrawRebarSupport_VertSection_ArchRib(&Dom, bUpper, dan, bMarkRebar, TRUE, dan, FALSE, CDRect(0, 0, 0, 0), nVertDir);
		}
	}
	// ����
	DrawRebarShear_VertSection_ArchRib(&Dom, bMarkRebar, FALSE, FALSE, CDRect(0, 0, 0, 0), nVertDir);

	// section ��ũ 2��
	char ch	= 'A';
	CString str	= _T("");
	BOOL bOK	= FALSE;

	// ���� ��ó
	double dXDisOfUpper	= m_pStd->m_pDrawCrossSection->GetDistForCrossSection_ArchRib(0, bOK);
	str.Format("%c", ch++);
	DimTextSection_ArchRib(&Dom, dXDisOfUpper, str);

	// 2�� ö�� �ִ� ��
	dXDisOfUpper	= m_pStd->m_pDrawCrossSection->GetDistForCrossSection_ArchRib(1, bOK);
	if(bOK)
	{
		str.Format("%c", ch++);
		DimTextSection_ArchRib(&Dom, dXDisOfUpper, str);
	}

	// ũ����
	dXDisOfUpper	= m_pStd->m_pDrawCrossSection->GetDistForCrossSection_ArchRib(2, bOK);
	str.Format("%c", ch++);
	DimTextSection_ArchRib(&Dom, dXDisOfUpper, str);

	pBri->GetTvVertSection_Std(nLeft, !bVertDir, FALSE, TRUE);
	*pDomP << Dom;

	Dom.SetCalcExtRect();
	return Dom.GetExtRect();
}

// ���꿡 ���� ��ũ
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

// ��ġ���� ������ ��
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
	CTwinVector tvSlope	= pFooting->m_tvArrFront.GetTvByInfo(pFooting->m_nType == FOOTING_TYPE_BASIC_RIGHTSLOPE ? "���ʿ������" : "�����������");
	xyDirRot	= ToDPointFrDegree(90-tvSlope.GetXyDir().GetAngleDegree()-90);

	// ���� ���/��ö��1�� �׸���
	CTwinVectorArray tvArrDraw, tvArrDrawRebar;

	// �Ϲݵ�
	CTwinVectorArray tvArrUpper;
	pFooting->GetTvOffset_Front(-1, TRUE, tvArrUpper, TRUE);
	tvArrDraw.AddFromTvArray(tvArrUpper);

	// ��ö��
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

	// �ʿ��� �κи� ����� �߶�.
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

	
	// ���� ���� ��ö��/��±�
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

// ��ġ���� l�� f ġ�� ����
// nUpper == 1 : ���
// 0 : �༱
// -1 : �ϴ�
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
				str.Format("��ü������");
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

				str.Format("��ü������");
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

				// ������ü ����Ǹ� ��ν����� ����� �� �׷��ֱ�
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
		sDescription.Format("���պ� %s ���ͷ� ö��", bLeft ? "����" : "����");
		CString sMark	= bInput ? "S" : pBri->GetMarkRebarShear(0, bInput, FALSE, FALSE, sDescription);
		long nSize	= tvArr[left].GetSize();
		if(bMarkRebar && nSize > 0)
		{
			
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

			CRebar *pRb = pBri->GetRebarShearByDescription(sDescription);
			CMarkDomyun mark;
			mark.SetDomyun(&Dom, m_pStd->m_pARoadOptionStd);
			mark.SetRebar(tvArr[left], tvArrShearRib);
			// ����
			mark.m_sMark	= sMark;
			mark.m_sDia		= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(ePartUpperSlab));
			mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_STT;	// ���� ǥ����ġ ã�� ���
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

// ��ġ���� ������ ġ��
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

// ���� ��ö�� ������.
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

	// ���ܸ鵵 �׸���
	// ���� ������ �⺻������
	pBri->m_footingEnd.m_exFooting.m_Pile.InitScaleCutting();
	// ��� ��ü���� ����ȭ�� ����.
	//pBri->SyncBridge(nLeft, !bVertDir);
	pBri->GetTvVertSection_Std(nLeft, !bVertDir, FALSE);
	pBri->SyncBridgeRebar(FALSE, nVertDir);

	// ��ġ���� ���ܸ� �׸�
	// ���ʴ� ��ü�� overlap üũ �Ѱ� �����ؼ� �׸���.
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
		// ��ö�� 1cycle �׸���
		for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			DrawRebarMain_VertSection_ArchRib(&Dom, nCycleLeft, dan, TRUE, FALSE, TRUE, 0, CDRect(0, 0, 0, 0), FALSE, nVertDir);
		}

		// ��ö�� ������ ġ��
		DimRebarMainAssm_VertSection_ArchRib(&Dom, 0, nCycleLeft, 0);	
	}
	else
	{
		// ��ö�� 1cycle �׸���
		for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			DrawRebarMain_VertSection_ArchRib(&Dom, nCycleLeft, dan, TRUE, FALSE, TRUE, -1, CDRect(0, 0, 0, 0), FALSE, nVertDir);
			DrawRebarMain_VertSection_ArchRib(&Dom, nCycleRight, dan, TRUE, FALSE, TRUE, 1, CDRect(0, 0, 0, 0), FALSE, nVertDir);
		}

		// ��ö�� ������ ġ��
		DimRebarMainAssm_VertSection_ArchRib(&Dom, 0, nCycleLeft, -1);
		DimRebarMainAssm_VertSection_ArchRib(&Dom, 0, nCycleRight, 1);

		// �߽ɼ�
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

// ��ġ���길 ���� �׸� ��� ���պδ� ������ �� ���δ�.
// �� �κ��� �������� �׷���
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
		CTwinVector tvLeft	= pWall->m_tvArrFront.GetTvByInfo("��ü������");
		CTwinVector tvRight	= pWall->m_tvArrFront.GetTvByInfo("��ü������");
		
		CHgBaseDrawStd baseDraw;
		double xDisStt	= pBri->m_pbRib[iUPPER].GetDistByXPos(tvLeft.m_v1.x);
		double xDisEnd	= pBri->m_pbRib[iUPPER].GetDistByXPos(tvRight.m_v1.x);
		
		m_pStd->DrawParabola(&Dom, &pBri->m_pbRib[iUPPER], xDisStt, xDisEnd, 0);
	}



	*pDomP << Dom;
}

// ��ġ���� �β� ġ��
// ������ ���������
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
		CTwinVector tvSlope	= pFooting->m_tvArrFront.GetTvByInfo(bStt ? "���ʿ������" : "�����������");

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

// ���� ���� ��ġ�� ���� ������� �׸�(��������)
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
			sHunch[iLEFT].Format("%d��°������ġ������", wall+1);
			sHunch[iRIGHT].Format("%d��°������ġ������", wall+1);
			CTwinVector tvHunch[2];
			tvHunch[iLEFT]	= pBri->m_tvArrVertSection.GetTvByInfo(sHunch[iLEFT]);
			tvHunch[iRIGHT]	= pBri->m_tvArrVertSection.GetTvByInfo(sHunch[iRIGHT]);

			CTwinVector tvWall[2];
			tvWall[iLEFT]	= pWall->m_tvArrFront.GetTvByInfo("��ü������");
			tvWall[iRIGHT]	= pWall->m_tvArrFront.GetTvByInfo("��ü������");
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

// �������� �׸���
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

// �����꿡 fck ���� ����
// nStt : -1 ���� , 0 : �߰�, 1 : ����
void CDrawVertSection::DimTextVertSectionUpperSlabFck(CDomyun *pDomP, long nStt)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	CTwinVector tvStt, tvEnd;
	if(pBri->IsOutWall(TRUE))
	{
		tvStt	= pBri->m_tvArrVertSection.GetTvByInfo("��������ϻ������");
	}
	else
	{
		tvStt	= pBri->m_tvArrVertSection.GetTvByInfo("��������������");
	}

	if(pBri->IsOutWall(FALSE))
	{
		tvEnd	= pBri->m_tvArrVertSection.GetTvByInfo("��������ϻ������");
	}
	else
	{
		tvEnd	= pBri->m_tvArrVertSection.GetTvByInfo("��������������");
	}


	CDPoint xyText	= CDPoint(0, 0);
	double dFck = pBri->GetValueFck(ePartUpperSlab, TRUE);		// ����������
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
	double dLS = pPile->m_dLenSteel + 1000;	// ������ ���̴� �Է±����� +1 (13195)
	double dLP = pPile->m_dLenPHC - 1000;	// phc�� ���̴� �Է±����� -1 (13195)
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

	// ����Ÿ�������� T�� �������� �ʴ´�.
	if(dT1==0 || pPile->m_nType == PILE_TYPE_HYUNJANG) 
	{
		str2.Format(_T("(%%%%c%.1f,"), dDia);
	}
	if(pPile->m_nType == PILE_TYPE_SPC)
	{
		str2.Format(_T("(��%.fx%sT, L=%.1fM %s)"), dDia, COMMA(dT1S), toM(dLS), hggettext("(����)"));
		str3.Format(_T("(��%.fx%sT, L=%.1fM (PHC))"), dDia, COMMA(dT1P), toM(dLP));
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
	sThickSlab.Format("������:t=%smm", COMMA(pBri->m_dTS));
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

	// ������ ���ϸ鿡�� �¿�� 1000, ���Ϸ� 100���� ����� �׸�.
	CTwinVector tvUpper;
	CTwinVector tvLower;
	pBri->GetTvVirtualRebarGirder(nJ, tvUpper, tvLower, bStt, bLeft, bGagak, bJongGirder, bUpperSlabGirder);

	double sw = bUpperSlabGirder ? 1.0 : -1.0;

	// �߶� ����
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

	pTvArr->GetTvByInfo("�Ϻν������ϸ�(������ü�Ʒ�)", tv);
	double dDimAngle = (pBri->m_bSlopeLowerSlab)?  (tv.GetXyDir(TRUE).GetAngleDegree()): 0;
	xyBase = tv.m_v2;
	xyStt = xyBase;
	xySttTot = xyStt;

	// ġ������ ������ġ�� ���ʰ� ���� ��� ������ Ȯ����ʸ� �������� �Ѵ�.
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

	pTvArr->GetTvByInfo("������ü������", tv);
	dArrDim.Add(tv.GetVerLength());
	sArrText.Add("0");
	sArrUnderText.Add("0");

	pTvArr->GetTvByInfo("�����������ġ", tv);
	dH = tv.GetVerLength();
	if(!bInput)
	{
		dArrDim.Add(dH);
		sArrText.Add("0");
		sArrUnderText.Add(COMMA(dH));
	}
	pTvArr->GetTvByInfo("�������������", tv);
	if(bInput)
		dH += tv.GetVerLength();
	else
		dH = tv.GetVerLength();
	dArrDim.Add(dH);
	sArrText.Add("BH");
	sArrUnderText.Add(COMMA(dH));

	pTvArr->GetTvByInfo("��������ϻ������", tv);
	dArrDim.Add(tv.GetVerLength());
	sArrText.Add("BTH");
	sArrUnderText.Add(COMMA(tv.GetVerLength()));

	// ���� �β�
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

	// ��ü ġ��
	{
		// �������� ġ��
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

//bInput ==0 :�������� 1:�߰������� 2:�Ϻν����� 3:�λ��������
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

	pTvArr->GetTvByInfo("�Ϻν������ϸ�(������ü�Ʒ�)", tv);
	double dDimAngle = (pBri->m_bSlopeLowerSlab)?  -(tv.GetXyDir().GetAngleDegree()): 0;
	xyBase = tv.m_v1;
	xyStt = xyBase;
	xySttTot = xyStt;
	
	// ġ������ ������ġ�� ���ʰ� ���� ��� ������ Ȯ����ʸ� �������� �Ѵ�. 
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
	else //�Ϻν�����ǥ��
	{
		dArrDim.Add(pBri->m_dTS_Lower);
		if(bInput == 2)	{sArrText.Add("T"); sArrUnderText.Add(COMMA(pBri->m_dTS_Lower));}
		else{sArrText.Add("0"); sArrUnderText.Add("");}
	}
	
	pTvArr->GetTvByInfo("������ü������", tv);
	if(!pBri->m_bExistProtectionRiseFooting)
		dArrDim.Add(tv.GetVerLength() - pBri->m_dTS_Lower);
	else
		dArrDim.Add(tv.GetVerLength());

	sArrText.Add("0");
	sArrUnderText.Add("0");

	pTvArr->GetTvByInfo("�����������ġ", tv);
	dH = tv.GetVerLength();
	if(bInput != 0) 
	{
		dArrDim.Add(dH);
		sArrText.Add("0"); 
		sArrUnderText.Add(COMMA(dH));
	}

	pTvArr->GetTvByInfo("�������������", tv);
	if(bInput == 0)
		dH += tv.GetVerLength();
	else
		dH = tv.GetVerLength();

	dArrDim.Add(dH);
	if(bInput == 0){sArrText.Add("BH");	sArrUnderText.Add(COMMA(dH));}
	else{sArrText.Add("0");	sArrUnderText.Add("");}

	pTvArr->GetTvByInfo("��������ϻ������", tv);
	dArrDim.Add(tv.GetVerLength());
	if(bInput == 0)	{sArrText.Add("BTH");sArrUnderText.Add(COMMA(tv.GetVerLength()));}
	else{sArrText.Add("0");	sArrUnderText.Add("");}

	// ���� �β�(���� ����ô� ���� ����)
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

	// ��ü ġ��	
	{
		// �������� ġ��
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

	// �Ϲݵ��� ���Ÿ��� �� �׸��Ƿ� �׻� FALSE
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

			// ������ �β�
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

		// �� ������ EL
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

		// �� ���� ��ü �� ��� EL
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

// ������౳ : �⼮����
void CDrawVertSection::DrawStoneBoxType(CDomyun *pDomP, BOOL bCutting)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(pBri == NULL || pBri->IsBoxType() == FALSE) return;
	if(pBri->m_nTypeLower != BOXFOOTING_TYPE_STONE) return;

	CExFootingApp *pExFoot = &pBri->m_footingBox.m_exFooting;

	CDomyun Dom(pDomP);
	CConcSymbol sym(m_pStd->m_pARoadOptionStd);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	CTwinVector tvR = pExFoot->m_tvArrFront.GetTvByInfo(_T("Ȯ����ʿ�����"));
	CTwinVector tvL = pExFoot->m_tvArrFront.GetTvByInfo(_T("Ȯ�����������"));

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
		if(tv.m_sInfo.Find(_T("�ϸ�")) != -1)
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

	// ������ ������ 0���� �۰ų� ���̰� 0���� ������ �׸��� �ʰ� �����Ѵ�
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

// �Ϻν����� ��ö�� �׸���
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

	// �Ϻ� ������
	// �ϸ� ///////////////////////////////////////////////////////////////////////
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &pBri->m_pArrRCMainLowerSlab[nCycle][iLOWER][nDan];
	// RebarInfoCycle �׸���
	m_pStd->DrawRebarInfoCycleArray(&Dom, pArrRC, nDrawLeft, rectLimit, !bInput);

	// ���� �׸���
	if(bDrawJoint)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);

		// ���� ���� ������
		CJointArray arrJoint;
		pBri->MakeJointMainRebarLowerSlab(arrJoint, nCycle, nDan, FALSE);

		// ���� �׸���
		m_pStd->DrawJointArray(&Dom, arrJoint, nDrawLeft, rectLimit);
	}

	// ġ������
	if(bDimRebar)
	{
		DimRebarMain_VertSection_LowerSlab_Lower_Cycle(&Dom, bMarkRebar ? 1 : 0, TRUE, nCycle, nVertDir, nDan);
	}

	// ö�� ��ũ
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



	// ��� /////////////////////////////////////////////////////////////////////
	pArrRC = &pBri->m_pArrRCMainLowerSlab[nCycle][iUPPER][nDan];

	// RebarInfoCycle �׸���
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);
	m_pStd->DrawRebarInfoCycleArray(&Dom, pArrRC, nDrawLeft, rectLimit);

	// ���� �׸���
	if(bDrawJoint)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);

		// ���� ���� ������
		CJointArray arrJoint;
		pBri->MakeJointMainRebarLowerSlab(arrJoint, nCycle, nDan, TRUE);

		// ���� �׸���
		m_pStd->DrawJointArray(&Dom, arrJoint, nDrawLeft, rectLimit);	
	}

	// ġ������
	if(bDimRebar)
		DimRebarMain_VertSection_LowerSlab_Upper_Cycle1(&Dom, bMarkRebar ? 1 : 0, TRUE, nCycle, nVertDir, nDan);

	// ö�� ��ũ
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

// �Ϻν����� ��ġ�� ��ö�� �׸���
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

	// ��� ������
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &pBri->m_pArrRCMainLowerSlabHunch[nCycle];
	if(!pArrRC) return;

	// RebarInfoCycle �׸���
	m_pStd->DrawRebarInfoCycleArray(&Dom, pArrRC, nDrawLeft, rectLimit);

	if(bMarkRebar)
	{
		m_pStd->MarkRebarInfoCycleArray(&Dom, pArrRC, FALSE, FALSE, pBri->m_dCoverLower_LowerSlab[0], 2, 0, nDrawLeft, CDRect(0, 0, 0, 0), 0.9, bInput, &pBri->m_pArrRebarMain);
	}


	*pDomP << Dom;
}

// �Ϻν����� 1��, 1,2����Ŭ, �ϸ� ��ö�� ġ��
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


	// ġ�����Կ� ����
	CDPointArray xyArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("BOTTOM");
	CDPoint xyStt(0, 0);

	// ġ������ �м��ؼ� ������
	BOOL bApplyEndIs2	= FALSE;
	CDPoint vAngStt			= bVertDir ? CDPoint(0, 1) : pBri->GetAngleJijum(0);
	CDPoint vAngEnd			= bVertDir ? CDPoint(0, 1) : pBri->GetAngleJijum(pBri->m_nQtyJigan);
	double dCoverSideStt	= pBri->GetLengthBynVertDir(pBri->m_dCoverWallOut[0] / vAngStt.y, nVertDir);
	double dCoverSideEnd	= pBri->GetLengthBynVertDir(pBri->m_dCoverWallOut[0] / vAngEnd.y, nVertDir);
	
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &pBri->m_pArrRCMainLowerSlab[nCycle][iLOWER][nDan];

	// ���ܻ��� ó��
	// 1. ����������ü�� ��ν����갡 ����� ������ ��� ù��° ö���� ���� �ȵ� ���·� �����ؼ� ġ�� �����Ѵ�.
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
		// ������ ������ �Է¿� �°� ��ȣ�� ����
		if(bInput)
		{
			// ���� ���� ����
			long nType	= 0;
			InsertJijumInfo(xyArrDim, sArrText, nType, 0, TRUE, bVertDir);

			// �Է������� ����
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


	// ġ�������� ���ؼ� �����ߴ��� ����.
	if(pArrRC->GetSize() > 0 && pRCFirstLowerSlab)
	{
		*pRCFirstLowerSlab = rcFirstLowerSlab;
	}

	*pDomP << Dom;
}

// �Ϻν����� 1��, 1����Ŭ, ��� ��ö�� ġ��
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


	// ġ�����Կ� ����
	CDPointArray xyArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
	CString sDir = _T("TOP");
	CDPoint xyStt(0, 0);

	// ġ������ �м��ؼ� ������
	BOOL bReverse = FALSE;
	CDPoint vAngStt			= bVertDir ? CDPoint(0, 1) : pBri->GetAngleJijum(0);
	CDPoint vAngEnd			= bVertDir ? CDPoint(0, 1) : pBri->GetAngleJijum(pBri->m_nQtyJigan);
	double dCoverSideStt	= pBri->GetLengthBynVertDir(pBri->m_dCoverWallOut[0] / vAngStt.y, nVertDir);
	double dCoverSideEnd	= pBri->GetLengthBynVertDir(pBri->m_dCoverWallOut[0] / vAngEnd.y, nVertDir);
	m_pStd->GetDimInfoAtRebarInfoCycle(&pBri->m_pArrRCMainLowerSlab[nCycle][iUPPER][nDan], TRUE, xyArrDim, sArrText, FALSE, TRUE, TRUE, bReverse, dCoverSideStt, dCoverSideEnd, nVertDir);

	if(xyArrDim.GetSize() > 0)
	{
		// 2����Ŭ�� 4����Ŭ�� ���۰� ���� ��ǥ�� �ϳ� �� �־� ��� �ȴ�.
		// ��? ������̶� ġ���������� ���۰� ����ġ�� ���������� ��������...
		if(0)//nCycle == 1 || nCycle == 3)
		{
			// �Ǻ� ���� ������ 
			xyArrDim.RemoveAt(0);
			xyArrDim.RemoveAt(xyArrDim.GetSize()-1);
			sArrText.RemoveAt(0);
			sArrText.RemoveAt(sArrText.GetSize()-1);


			CTwinVector tvSlab	= pBri->GetTvOffset_LowerSlab(0, TRUE, -1, -1);

			// ������ġ
			xyArrDim.InsertAt(0, tvSlab.m_v1);
			sArrText.InsertAt(0, "0");
			sArrText.SetAt(1, "1");

			// ����ġ
			xyArrDim.Add(tvSlab.m_v2);
			sArrText.Add("1");

			// �Ǻ� ���� �߰��Ѵ�.
			// ó���� ���κп� �Ǻ� ���� �߰��Ѵ�. ////////////////
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

		// ������ ������ �Է¿� �°� ��ȣ�� ����
		if(bInput)
		{
			// ���� ���� ����
			long nType	= 0;
			InsertJijumInfo(xyArrDim, sArrText, nType, nCycle, FALSE, bVertDir);

			// �Է������� ����
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

	// ��� ���� rc ���� ////////////
	pBri->m_Util.AddRebarInfoCycle(&arrRC, &pBri->m_pArrRCMainOutWall[nCycle][iSTT][iOUTTER][nDan], FALSE);
	pBri->m_Util.AddRebarInfoCycle(&arrRC, &pBri->m_pArrRCMainUpperSlab[nCycle][iUPPER][nDan], FALSE);
	pBri->m_Util.AddRebarInfoCycle(&arrRC, &pBri->m_pArrRCMainOutWall[nCycle][iEND][iOUTTER][nDan], TRUE);
	pBri->m_Util.AddRebarInfoCycle(&arrRC, &pBri->m_pArrRCMainLowerSlab[nCycle][iLOWER][nDan], TRUE);

	// �������� ������ ö���� ���۰� ���κ��� ����� ����
	long nSize	= arrRC.GetSize();
	if(nSize > 0)
	{
		// �Ϻν����� ö���� ��ü���� ��� ���� �߶� �ΰ��� ����� �ش�
		// �׷��� ġ���� �̻ڰ� ����
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

	// ö�� �����ͼ� �׸���
	CHgBaseDrawStd baseDraw;
	CDPointArray xyArr;
	pBri->GetXyRebarSupport_VertSection_LowerSlab(xyArr, bUpper, nDan, nVertDir, bCalSupRebar);

	// rectLimit ����
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
	// ġ�� ����
	if(bDimRebar)
	{
		if(xyArr.GetSize() > 0)
		{
			m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

			CDPointArray xyArrDim;
			CDPoint xyStt	= xyArr.GetAt(0);
			CStringArray sArrText, sArrUnderText;
			CString sDir	= bUpper ? "TOP" : "BOTTOM";

			// ġ���� �Ǻ��� R���� �߰��ؾ� ��. //////////////////////
			CTwinVector tvUpperStt, tvUpperEnd;
			CRebarInfoCycle *pRCStt	= NULL,
				*pRCEnd	= NULL;

			// ���� �߰�(���ۺ�)
			if(pBri->m_tvArrVertSection.GetTvByInfo("������ü(�λ��������)", tvUpperStt))
			{
				xyArrDim.Add(bUpper ? tvUpperStt.m_v2 + CDPoint(0, pBri->m_dTS_Lower) : tvUpperStt.m_v2);	sArrText.Add("0"); sArrUnderText.Add("0");
				xyStt.y	= bUpper ? tvUpperStt.m_v2.y + pBri->m_dTS_Lower : tvUpperStt.m_v2.y;
			}
			else if(pBri->m_tvArrVertSection.GetTvByInfo("������ü������", tvUpperStt))
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

			// ġ�� ���� ���� ������
			CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_SupportRebar_LowerSlab[upper][nDan];
			m_pStd->GetDimInfoAtPlaceRebarByBlock(xyArr, xyArrDim, sArrText, sArrUnderText, pPlace, bUpper ? "U" : "L", xyStt, CDPoint(1, 0), bInput, TRUE);

			// ���� �߰�(����)
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
			if(pBri->m_tvArrVertSection.GetTvByInfo("������ü(�λ��������)", tvUpperEnd))
			{
				xyArrDim.Add(bUpper ? tvUpperEnd.m_v2 + CDPoint(0, pBri->m_dTS_Lower) : tvUpperEnd.m_v2);	sArrText.Add("0"); sArrUnderText.Add("0");
				xyStt.y	= bUpper ? max(tvUpperEnd.m_v2.y + pBri->m_dTS_Lower, xyStt.y) : min(tvUpperEnd.m_v2.y, xyStt.y);
			}
			else if(pBri->m_tvArrVertSection.GetTvByInfo("������ü������", tvUpperEnd))
			{
				xyArrDim.Add(bUpper ? tvUpperEnd.m_v2 + CDPoint(0, pBri->m_dTS_Lower) : tvUpperEnd.m_v2);	sArrText.Add("0"); sArrUnderText.Add("0");
				xyStt.y	= bUpper ? max(tvUpperEnd.m_v2.y + pBri->m_dTS_Lower, xyStt.y) : min(tvUpperEnd.m_v2.y, xyStt.y);
			}

			if(!bInput)
			{
				//sArrText.RemoveAll();
				sArrUnderText.RemoveAll();
			}

			// ġ�� ����
			if(xyArrDim.GetSize() > 1)
			{
				xyStt.x	= xyArrDim.GetAt(0).x;
				baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, bUpper ? nDimDan : nDimDan+1, sDir, -1, bUpper==FALSE, FALSE, FALSE);
			}
		}
	}


	// ��ŷ
	CString sDescription	= _T("");
	if(bUpper)
		sDescription.Format("�Ϻν����� ��� %d�� ���ö��2", nDan+1);
	else
		sDescription.Format("�Ϻν����� �ϸ� %d�� ���ö��", nDan+1);
	CString sMark	= pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescription);

	//Ȯ��or������ ��� �ش� ��� ö���� ��ã���� ����
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

		CString sUpdn = bUpper? _T("���") : _T("�ϸ�");

		for (long ang = 1 ; ang < 5; ang++)
		{
			if(ang == 1 && nSizeStt_And == 0) continue;
			else if(ang == 2 && nSizeStt_Or == 0) continue;
			else if(ang == 3 && nSizeEnd_And == 0) continue;
			else if(ang == 4 && nSizeEnd_Or == 0) continue;

			if(bUpper)
				sDescription.Format("�Ϻν����� %s %d�� ���ö��2_%d", sUpdn, nDan+1, 5+ang);
			else
				sDescription.Format("�Ϻν����� %s %d�� ���ö��%d", sUpdn, nDan+1, 5+ang);
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
			//ǥ�� �ܸ� ��� - ���ö���Է¿� �Էµ� ��ǥ�� �̰ݰŸ����� �ִ밪�� ����.
			CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_SupportRebar_LowerSlab[upper][0];

			CDPointArray xySupp;

			if(pPlace != NULL) xySupp = pPlace->m_xyArr;

			xySupp.Sort(FALSE, FALSE);

			double dMaxDist = (xySupp.GetSize() == 0) ? 0 : xySupp.GetAt(0).y;

			//��ġ�� ctc���� ū �̰ݰŸ��� ������ �Ǹ� ��ŷ�� ���� ���ش�. ��ŷ�� ����� �������δ�.
			CTypedPtrArray<CObArray, CDPointArray*> xyDArr;

			CDPointArray *pXyArr = NULL;

			if(xySupp.GetSize() > 0) pXyArr = new CDPointArray;

			for(long ix = 0; ix < xyArr.GetSize() - 1; ix ++)
			{
				double dLen = rp.GetDistPointToPoint(xyArr[ix], xyArr[ix+1]);

				if(Compare(dLen, dMaxDist + 1, ">", 0.001)) //�ִ밪���� ū���� ������ ��ŷ�� �ϳ��� �Ѵ�.
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

	// ö�� �����ͼ� �׸���
	CTwinVectorArray tvArr, tvArrHunch;
	pBri->GetTvRebarShear_VertSection_LowerSlab(tvArr, tvArrHunch, nVertDir);

	// rectLimit ����
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

	// ���� ö�� �׸���
	m_pStd->DrawTvArrayWidthDashDot(&Dom, tvArr, FALSE);
	m_pStd->DrawTvArrayWidthDashDot(&Dom, tvArrHunch, FALSE);

	// ��ũ�� ����δ� 2 ��ġ�δ� 1 �ƴѰŴ� 0
	for(long hunch = 0; hunch < 2; hunch++)
	{
		CTwinVectorArray *pTvArr;//	= bHunch ? &tvArrHunch : &tvArr;
		CString sDescription	= _T("");
		switch(hunch)
		{
		case 0 : 
			pTvArr	= &tvArr; 
			sDescription	= "�Ϻν����� ����ö��1";
			break;
		case 1 : 
			pTvArr	= &tvArrHunch; 
			sDescription	= "�Ϻν����� ����ö��2";
			break;
		}


		CString sMark	= pBri->GetMarkRebarShear(0, bInput, FALSE, FALSE, sDescription);
		if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
		{
			double dTS	= pBri->m_dTS_Lower;
			// ö����������� �ݴ�� �Ǿ�� ��.
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
			// ����
			mark.m_sMark	= sMark;
			mark.m_sDia		= m_pStd->m_pARcBridgeDataStd->GetStringDia(pPlace->m_dDia, pBri->GetValueFy(ePartLowerSlab,TRUE));
			mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_POS;	// ���� ǥ����ġ ã�� ���
			mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;	// TYPE_MARKPOS_POS�϶� �ɼ�
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
		// ġ�� �����Ҷ��� ��ġ�ο� �ƴѰŸ� ���ľ� �ȴ�.
		// ���ļ� ���ĵ� �ؾ� ����?(��
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

			// ġ���� �Ǻ��� R���� �߰��ؾ� ��. //////////////////////
			CTwinVector tvStt, tvEnd;

			// ���� �߰�(���ۺ�)
			if(pBri->m_tvArrVertSection.GetTvByInfo("������ü(�λ��������)", tvStt))
			{
				xyArrDim.Add(tvStt.m_v2); sArrText.Add("0"); sArrUnderText.Add("0");
				xyStt	= xyArrDim.GetAt(0);
			}
			else if(pBri->m_tvArrVertSection.GetTvByInfo("������ü������", tvStt))
			{
				xyArrDim.Add(tvStt.m_v1); sArrText.Add("0"); sArrUnderText.Add("0");
				xyStt	= xyArrDim.GetAt(0);
			}
			xyStt.y = tvArrTot.GetXyBottom().y;

			// ġ�� ���� ���� ������
			CRebarPlacing rb;
			CPlaceRebarByBlock *pPlace	= &pBri->m_placeByBlock_ShearRebar_LowerSlab;
			m_pStd->GetDimInfoAtPlaceRebarByBlock(xyArr, xyArrDim, sArrText, sArrUnderText, pPlace, "V", xyStt, CDPoint(1, 0), bInput, TRUE);

			// ���� �߰�(����)
			if(pBri->m_tvArrVertSection.GetTvByInfo("������ü(�λ��������)", tvEnd))
			{
				xyArrDim.Add(tvEnd.m_v2); sArrText.Add("0"); sArrUnderText.Add("0");
			}
			else if(pBri->m_tvArrVertSection.GetTvByInfo("������ü������", tvEnd))
			{
				xyArrDim.Add(tvEnd.m_v2); sArrText.Add("0"); sArrUnderText.Add("0");
			}

			// ġ�� ����
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

//�ܸ鿡�� �λ��������������. 
void CDrawVertSection::DrawBFootingSection(CDomyun *pDomP, BOOL bStt)
{
	CRcBridgeRebar *pBri = m_pStd->m_pRcBridgeRebar;
	if(!pBri) return;

	CHgBaseDrawStd baseDraw;
	baseDraw.m_pLine	= pBri->GetLineBase();
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	CString strStt = bStt ? _T("����") : _T("����");
	double sw = bStt ? -1.0 : 1.0;

	CTwinVectorArray tvArrPRF;
	CTwinVector tvTop, tvBottom, tvSide;
	
	pBri->GetTvPRF_VertSection(tvArrPRF, bStt, 0, TRUE);
	tvArrPRF.GetTvByInfo(strStt + _T("�λ�������ǻ��"), tvTop);
	tvArrPRF.GetTvByInfo(strStt + _T("�λ�������ǹٱ�����"), tvSide);
	tvArrPRF.GetTvByInfo(strStt + _T("�λ���������ϴ�"), tvBottom);

	CDRect rect;
	double dSta = pBri->GetStationOnJijum(bStt ? 0 : pBri->m_nQtyJigan);

	//�ڸ� ���� ���ϱ�
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

	// ��ö��
	if(bMainRebar)
	{
		DrawRebarMain_VertSection_BFooting(&Dom, bStt, bDimRebar, bMarkRebar, bInput);
	}

	// ����ö��
	if(bSupportRebar)
	{
		DrawRebarSupport_VertSection_BFooting(&Dom, bStt, bDimRebar, bMarkRebar, TRUE);
	}
	
	// ����ö�� �׸���
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

	// ö�� �����ͼ� �׸���
	CDPointArray xyArrLeft, xyArrRight;
	pBri->GetXyRebarSupport_VertSection_LowerSlabHunch(xyArrLeft, xyArrRight, nIdxHunch);

	// rectLimit ����
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

	// ġ�� ����
	if(bDimRebar)
	{

	}


	// ��ŷ
	CDPointArray xyArrTot;
	long i = 0; for(i = 0; i < xyArrLeft.GetSize(); i++)
		xyArrTot.Add(xyArrLeft.GetAt(i));
	for(i = 0; i < xyArrRight.GetSize(); i++)
		xyArrTot.Add(xyArrRight.GetAt(i));
	CString sDescription	= _T("");
	sDescription.Format("�Ϻν����� ��ġ�� ���ö��");
	CString sMark	= pBri->GetMarkRebarSupport(0, bInput, FALSE, FALSE, sDescription);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		CRebar *pRb = pBri->GetRebarSupportByDescription(sDescription);
		CPlaceRebarByBlock *pPlace		= &pBri->m_placeByBlock_SupportRebar_LowerSlab[iUPPER][0];
		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
		mark.SetRebar(xyArrTot);

		// ����
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
	//�Ϻν����� ġ��
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
	if(pTvArr->GetTvByInfo("Ȯ����ʻ��", tv))
	{
		dWidth += tv.GetHorLength(bApplyAngle);
		dWidth -= dPRFW*2;
	}
	Dom.DimLineTo(dWidth, 0, COMMA(dWidth));

	Dom.DimLineTo(dPRFW, 0, COMMA(dPRFW));
	Dom.DimLineToOver(pFooting->m_exFooting.m_dEFWR, 0, COMMA(pFooting->m_exFooting.m_dEFWR), 1);

	*pDomP << Dom;
}