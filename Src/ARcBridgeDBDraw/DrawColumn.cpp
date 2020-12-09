// DrawColumn.cpp: implementation of the CDrawColumn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARcBridgeDBDraw.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawColumn::CDrawColumn()
{

}

CDrawColumn::~CDrawColumn()
{

}

void CDrawColumn::DrawPlan(CDomyun *pDomP, CColumnApp *pColumn, CTwinVectorArray *tvArrReturn, BOOL bDrawCenter, BOOL bApplyAnglePlane, BOOL bHidden, long nFront, CTwinVectorArray &tvArrCen)
{
	if(!pColumn) return;
	if(!pColumn->m_bIs) return;

	CDomyun Dom(pDomP);
	
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, bHidden ? HCAD_CONH : HCAD_CONC);

	
	pColumn->GetTvPlan(bApplyAnglePlane);

	// ����� ����, �ϸ��� �Ǽ����� �׸�
	if(nFront != 0)
	{
		CRebarPlacing rb;
		CTwinVectorArray tvArr;
		tvArr	= pColumn->m_tvArrPlan[iOUTTER][iUPPER];

		rb.TrimTvArrayByTvArray(tvArr, tvArrCen, nFront == -1 ? FALSE : TRUE, TRUE);
		m_pStd->DrawTvArray(&Dom, tvArr, FALSE, FALSE);
	}
	else
	{
		m_pStd->DrawTvArray(&Dom, pColumn->m_tvArrPlan[iOUTTER][iUPPER], FALSE, FALSE);
		tvArrReturn->ChangePos(pColumn->m_tvArrPlan[iOUTTER][iUPPER]);
	}
	

	// �߽ɼ�
	if(bDrawCenter)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
		double dHor = pColumn->GetWidth(FALSE);
		double dVer = pColumn->GetWidth(TRUE);
		double dMarginHor = dHor/5;
		double dMarginVer = dVer/5;
		CDPoint xyCen = pColumn->m_xyCen;
		CDPoint xy1 = xyCen;
		CDPoint xy2 = xyCen;
		CDPointArray xyArr;

		// ����
		xy1.x -= (dHor/2+dMarginHor);
		xy2.x += (dHor/2+dMarginHor);
		xyArr.Add(xy1);
		xyArr.Add(xy2);

		// ����
		xy1 = xyCen; 
		xy2 = xyCen;
		xy1.y -= (dVer/2+dMarginVer);
		xy2.y += (dVer/2+dMarginVer);
		xyArr.Add(xy1);
		xyArr.Add(xy2);

		// ȸ��.
		if(bApplyAnglePlane)
		{
			CRebarPlacing rbPlacing;
			rbPlacing.RotateRebar(xyArr, xyCen, pColumn->m_dAngPlan);
		}

		// �׸���.
		m_pStd->DrawLineArray(&Dom, xyArr);
	}


	*pDomP << Dom;
}

// ��� ġ��
void CDrawColumn::DimPlan(CDomyun *pDomP, CColumnApp *pColumn, long nDimDan, BOOL bInput, BOOL bApplyAnglePlane)
{
	if(!pColumn) return;
	if(!pColumn->m_bIs) return;
	if(nDimDan < 0) nDimDan = 0;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	
	CDoubleArray dArrDim;
	CStringArray sArrText;
	CStringArray sArrUnderText;
// 	CString sDir = _T("BOTTOM");
	CDPoint xyStt(0, 0);
	CTwinVector tv;

	double dHor = pColumn->GetWidth(FALSE);
	double dVer = pColumn->GetWidth(TRUE);


	// ����
	CString sDir = _T("BOTTOM");
	xyStt = pColumn->m_xyCen;
	xyStt.x -= dHor/2;
	xyStt.y -= dVer/2;

	// ��
	if(pColumn->m_nType == COL_TYPE_CIRCLE)
	{
		dArrDim.Add(pColumn->m_dR[iOUTTER][iUPPER]*2);
		sArrText.Add("D");
		sArrUnderText.Add(COMMA(pColumn->m_dR[iOUTTER][iUPPER]*2));
	}
	// �簢��
	else if(pColumn->m_nType == COL_TYPE_RECTANGLE)
	{
		dArrDim.Add(dHor);
		sArrText.Add("B");
		sArrUnderText.Add(COMMA(dHor));
	}
	// �ٰ���
	else if(pColumn->m_nType == COL_TYPE_POLY)
	{
		/*
		// �ٰ����� ���� �������� �ϱ� ������ ġ�����Խ� ���� �׷��ش�.
		m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_HIDDEN_LINE);
		Dom.Circle(pColumn->m_xyCen, pColumn->m_dR);
		m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_SOLID);
		*/
		dArrDim.Add(dHor);
	}
	else if(pColumn->m_nType == COL_TYPE_TRACK)
	{
		dArrDim.Add(pColumn->m_dR[iOUTTER][iUPPER]);
		sArrText.Add("R");
		sArrUnderText.Add(COMMA(pColumn->m_dR[iOUTTER][iUPPER]));

		dArrDim.Add(pColumn->m_dB[iOUTTER][iUPPER]-pColumn->m_dR[iOUTTER][iUPPER]*2);
		sArrText.Add("0");
		sArrUnderText.Add("0");
	
		dArrDim.Add(pColumn->m_dR[iOUTTER][iUPPER]);
		sArrText.Add("R");
		sArrUnderText.Add(COMMA(pColumn->m_dR[iOUTTER][iUPPER]));
	}
	CHgBaseDrawStd baseDraw;

	if(!bInput)
	{
		sArrText.RemoveAll();
		sArrUnderText.RemoveAll();
	}
	baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, !bInput, FALSE);

	if(pColumn->m_nType == COL_TYPE_TRACK)
	{
		dArrDim.RemoveAll();
		sArrText.RemoveAll();
		sArrUnderText.RemoveAll();

		dArrDim.Add(pColumn->m_dB[iOUTTER][iUPPER]);
		sArrText.Add("B");
		sArrUnderText.Add("0");

		if(!bInput)
		{
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();
		}
		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan+1, sDir, -1, FALSE, !bInput, FALSE);
	}

	// ����
	sDir = _T("RIGHT");
	xyStt = pColumn->m_xyCen;
	xyStt.x += dHor/2;
	xyStt.y -= dVer/2;

	dArrDim.RemoveAll();
	sArrText.RemoveAll();
	sArrUnderText.RemoveAll();

	// ��
	if(pColumn->m_nType == COL_TYPE_CIRCLE)
	{
		
	}
	// �簢��
	else if(pColumn->m_nType == COL_TYPE_RECTANGLE)
	{
		dArrDim.Add(dVer);
		sArrText.Add("H");
		sArrUnderText.Add(COMMA(dVer));
	}
	else if(pColumn->m_nType == COL_TYPE_POLY)
	{
	}
	else if(pColumn->m_nType == COL_TYPE_TRACK)
	{
		dArrDim.Add(pColumn->m_dR[iOUTTER][iUPPER]);
		sArrText.Add("R");
		sArrUnderText.Add(COMMA(pColumn->m_dR[iOUTTER][iUPPER]));

		dArrDim.Add(pColumn->m_dH[iOUTTER][iUPPER]-pColumn->m_dR[iOUTTER][iUPPER]*2);
		sArrText.Add("0");
		sArrUnderText.Add("0");
	
		dArrDim.Add(pColumn->m_dR[iOUTTER][iUPPER]);
		sArrText.Add("R");
		sArrUnderText.Add(COMMA(pColumn->m_dR[iOUTTER][iUPPER]));
	}

	if(!bInput)
	{
		sArrText.RemoveAll();
		sArrUnderText.RemoveAll();
	}

	baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, FALSE, !bInput, FALSE);

	if(pColumn->m_nType == COL_TYPE_TRACK)
	{
		dArrDim.RemoveAll();
		sArrText.RemoveAll();
		sArrUnderText.RemoveAll();

		dArrDim.Add(pColumn->m_dH[iOUTTER][iUPPER]);
		sArrText.Add("H");
		sArrUnderText.Add("0");

		if(!bInput)
		{
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();
		}
		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan+1, sDir, -1, FALSE, !bInput, FALSE);
	}

	if(bApplyAnglePlane)
	{
		CDPoint xyRotate = ToDPointFrRadian(ToRadian(pColumn->m_dAngPlan));
		Dom.Rotate(xyRotate.GetAngleRadian(), pColumn->m_xyCen.x, pColumn->m_xyCen.y);
	}


	*pDomP << Dom;
}

// ��� ���� �׸�
// bOnlyPattern : ��� ������ ���̸� �׸���..?
// BOOL bOnlyOutLine : �ܰ����� �׸���?
void CDrawColumn::DrawFront(CDomyun *pDomP, CColumnApp *pColumn, BOOL bSide, BOOL bOnlyPattern, BOOL bOnlyOutLine)
{
	if(!pColumn) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	pColumn->GetTvFront(!bOnlyOutLine, bSide);
	
	CTwinVectorArray tvArr;
	tvArr = pColumn->m_tvArrFront;

	// ��� ���̸� �׸� ��� ��� ���̸� ����.
	if(bOnlyPattern)
	{
		pColumn->m_tvArrFront.GetTvArrByInfoRange("��չ��̽���", "��չ��̳�", tvArr);
	}

	m_pStd->DrawTvArray(&Dom, tvArr, FALSE, FALSE);

	*pDomP << Dom;	
}

// ��� ��� ��ö��(��) �׸���
void CDrawColumn::DrawRebarMain_Plan(CDomyun *pDomP, CWallApp *pWall, long nDan, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, BOOL b2Cycle)
{
	if(pWall == NULL)
		return;

	if(pWall->IsRoundRectWall() == FALSE && b2Cycle)
		return;

	CColumnApp *pColumn = &pWall->m_Column;
	if(pColumn == NULL)
	{
		return;
	}
	if(pColumn->m_bIs == FALSE)
	{
		return;
	}

	if(nDan == -1) nDan = MAX_REBAR_DAN - 1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN - 1) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARX);

	// ��ö��(��) �׸���
	CHgBaseDrawStd baseDraw;
	CDPointArray xyArr, xyArrDirApplyScale;

	if(pWall->IsRoundRectWall())
		pWall->GetXyRebarMain_Plan_For_Round_Wall(xyArr, xyArrDirApplyScale, nDan, b2Cycle);
	else
		pColumn->GetXyRebarMain_Plan(xyArr, xyArrDirApplyScale, nDan);

	baseDraw.DrawPointRebar(&Dom, xyArr, xyArrDirApplyScale, -1, -1, FALSE);

	if(xyArr.GetSize() == 0)
	{
		return;
	}

	// ġ���� �׸���	
	if(bDimRebar == TRUE && b2Cycle == FALSE)
	{
		if(bInput == FALSE)
		{
			BOOL bExist2Cycle = (pWall->IsRoundRectWall() && pWall->IsExistMainRebar(1, 0, TRUE, 0))? TRUE : FALSE;
			if(pColumn->m_nType != COL_TYPE_POLY)
			{
				DimRebarMain_Plane_Track(&Dom, pColumn, 1, bExist2Cycle);	// 0,0�� �߽����� �ؼ� �׸�
			}
		}
		else
		{
			DimRebarMain_Plane_Track_InputMode(&Dom, pColumn, nDan, 1);
		}
	}
	
	// ��ũ �׸���
	if(bMarkRebar == TRUE)
	{
		DrawRebarMainMark_Plan(&Dom, pWall, nDan, FALSE, bInput, b2Cycle);

		// ��ü���϶��� ��,���� ����� ǥ���Ѵ� (#0031745)
		if(pWall->m_bTopIsColumn == FALSE)
		{
			DrawRebarMainMark_Plan(&Dom, pWall, nDan, TRUE, bInput, b2Cycle);
		}
	}

	*pDomP << Dom;
}

// ��� ��ö��(��) ��� �׸���
void CDrawColumn::DrawRebarRound_Plan(CDomyun *pDomP, CColumnApp *pColumn, long nDan, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput)
{
	if(!pColumn) return;
	if(!pColumn->m_bIs) return;
	if(nDan == -1) nDan = MAX_REBAR_DAN - 1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN - 1) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	// ��ö�� �׸���
	CRebarPlacing rb;
	CTwinVectorArray tvArrPlan;
	pColumn->GetTvRebarRound_Plan(tvArrPlan, nDan);
	rb.GetOffsetTvArray(tvArrPlan, REBAR_RADIUS);

	m_pStd->DrawTvArray(&Dom, tvArrPlan);

	if(bDimRebar)
	{
	}

	if(bMarkRebar)
	{
	}

	*pDomP << Dom;
}

// ��� ���鵵 ��ö�� �׸���
void CDrawColumn::DrawRebarMain_Front(CDomyun *pDomP, CColumnApp *pColumn, BOOL bSide, long nDan, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, BOOL bOnlyOutSideRebar, BOOL bApplyCuttingCircle)
{
	if(!pColumn) return;
	if(nDan == -1) nDan = MAX_REBAR_DAN - 1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN - 1) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CTwinVectorArray tvArr;
	// �Ϻ� ����� ö�� �׸���
	// ����� ö���� �ܰ��� �⺻ö���� ���� ���ؾ� �ȴ�.
	// �ܰ��� �Ǽ��̰�, �⺻ö���� �����̱� ������..
	// �⺻ö��(����, �����)
	if(!bOnlyOutSideRebar)
	{
		pColumn->GetTvRebarMain_Front(tvArr, bSide, nDan, 1, TRUE);
		if(tvArr.GetSize() == 0) return;
		m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_HIDDEN2_LINE);
		if(bApplyCuttingCircle)
			ApplyCuttingArea_Front(&Dom, pColumn, tvArr, nDan, FALSE, bSide);
		m_pStd->DrawTvArray(&Dom, tvArr);
	}

	
	m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_SOLID);

	// �⺻ö��(��������)
	if(!bOnlyOutSideRebar)
	{
		pColumn->GetTvRebarMain_Front(tvArr, bSide, nDan, 1, FALSE);
		if(bApplyCuttingCircle)
			ApplyCuttingArea_Front(&Dom, pColumn, tvArr, nDan, FALSE, bSide);
		m_pStd->DrawTvArray(&Dom, tvArr);
	}

	// �ܰ�(����)
	pColumn->GetTvRebarMain_Front(tvArr, bSide, nDan, 0, TRUE);
	// �ܰ����� ö���� ������ ǥ�ø� ���ؼ� �ణ ���� ����� �׷��ش�.
	const double dGap	= 50;
	CRebarPlacing rb;
	rb.MoveRebar(tvArr, CDPoint(0, dGap));
	if(bApplyCuttingCircle)
		ApplyCuttingArea_Front(&Dom, pColumn, tvArr, nDan, FALSE, bSide);
	m_pStd->DrawTvArray(&Dom, tvArr);

	// �ܰ�(��������ö��)
	pColumn->GetTvRebarMain_Front(tvArr, bSide, nDan, 0, FALSE);
	if(bApplyCuttingCircle)
		ApplyCuttingArea_Front(&Dom, pColumn, tvArr, nDan, FALSE, bSide);
	m_pStd->DrawTvArray(&Dom, tvArr);

	
	// 
	CTwinVectorArray tvArrRound;
	pColumn->GetTvRebarRound_Front(tvArrRound, bSide, nDan);
	
	if(bDimRebar)
	{
	}

	CString sDescription	= _T("");
	sDescription.Format("��� ��ö�� ���� %d��", nDan+1);
	CString sMark			= pColumn->GetMarkRebar(COL_REBAR_MO1+nDan, bInput, FALSE, -1, sDescription);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		if(bOnlyOutSideRebar)
			pColumn->GetTvRebarMain_Front(tvArr, bSide, nDan, 0, FALSE);
		else
			pColumn->GetTvRebarMain_Front(tvArr, bSide, nDan, -1, FALSE);
		if(bApplyCuttingCircle)
			ApplyCuttingArea_Front(&Dom, pColumn, tvArr, nDan, nDan==0, bSide);

		if(tvArrRound.GetSize() == 0)
		{
			CDPoint xyCen(0, 0);
			double dR(0);
			pColumn->GetInfoCuttingCircleFront(xyCen, dR, nDan, bSide);

			CTwinVector tv;
			tv.m_v1	= CVector(xyCen.x - dR, xyCen.y - dR, 0);
			tv.m_v2	= CVector(xyCen.x + dR, xyCen.y - dR, 0);
			tvArrRound.Add(tv);

			tv.m_v1	= CVector(xyCen.x - dR, xyCen.y + dR, 0);
			tv.m_v2	= CVector(xyCen.x + dR, xyCen.y + dR, 0);
			tvArrRound.Add(tv);
		}

		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd); 
		CRebar *pRb = pColumn->GetRebarByDescription(sDescription);

		mark.SetRebar(tvArr, tvArrRound);
		mark.m_sMark					= sMark;
		mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pColumn->m_dDiaMain[iOUTTER][nDan], pColumn->m_Fy);
		mark.m_dPosScale				= 0.3;
		mark.m_nTypeMarkDir				= TYPE_MARKDIR_LEFT;
		mark.m_dCtc						= pColumn->GetCTCMainRebar(FALSE, nDan);
		mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;
		mark.MarkRebar();
	}
	

	*pDomP << Dom;
}

// ��� ���鵵 ��ö��
void CDrawColumn::DrawRebarRound_Front(CDomyun *pDomP, CColumnApp *pColumn, long nJijum, BOOL bSide, long nDan, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, BOOL bIsTotalDim, BOOL bApplyCuttingCircle)
{
	if(!pColumn) return;
	if(nDan == -1) nDan = MAX_REBAR_DAN - 1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN - 1) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARA);
	CHgBaseDrawStd baseDraw;

	CTwinVectorArray tvArr;
	pColumn->GetTvRebarRound_Front(tvArr, bSide, nDan);
	if(bApplyCuttingCircle)
		ApplyCuttingArea_Front(&Dom, pColumn, tvArr, nDan, FALSE, bSide);
	baseDraw.DrawTvArray(&Dom, tvArr);

	if(bDimRebar && tvArr.GetSize() > 0)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		// �����ʿ� ġ���� ���� �ϱ� ������ ��ö���� ���� ������ ���� �������� �����Ѵ�.
		CTwinVectorArray tvArrMainOutUpper;
		pColumn->GetTvRebarMain_Front(tvArrMainOutUpper, bSide, nDan, 0, FALSE);
		if(tvArrMainOutUpper.GetSize() == 0)
		{
			AfxMessageBox("��ö���� ���������� �������� �ʾƼ� ��ö�� ġ�� ������ �Ұ��� �մϴ�.", MB_ICONSTOP);
			return;
		}

		// ġ���� ���� ��
		// �����ϸ���� ... ö�ٳ�����...
		CTwinVector tvRebarMain, tvLower;
		tvRebarMain.m_v1	= tvArrMainOutUpper.GetAt(1).GetXyBottom();
		tvRebarMain.m_v2	= tvArrMainOutUpper.GetAt(1).GetXyTop();
		tvLower	= pColumn->GetTvLowerLimitOfMainRebar();//pColumn->m_footing.m_tvArrFront.GetTvByInfo("�����ϸ�");
		tvRebarMain.m_v1.y	= tvLower.m_v1.y;
			
		CDoubleArray dArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		long nDimDan	= nDan+1;
		CString sDir	= _T("RIGHT");
		CDPoint xyStt	= tvRebarMain.m_v1;
		CString sRound	= nDan == 0 ? "��ö�� ����" : "��ö�� ����";

		m_pStd->GetDimInfoAtPlaceRebarByBlock(dArrDim, sArrText, sArrUnderText, &pColumn->m_placeRebarRound[iOUTTER][nDan], nDan == 0 ? "H" : "h", TRUE, TRUE);

		CTwinVectorArray tvArrTmp;
		pColumn->GetTvRebarRound_Front(tvArrTmp, bSide, nDan);

		dArrDim.Add(tvRebarMain.m_v2.y - tvArrTmp.GetAt(tvArrTmp.GetSize()-1).m_v1.y);
		sArrText.Add("0");
		sArrUnderText.Add("0");

		if(!bInput)
		{
			
			sArrText.RemoveAll();
			long i = 0; for(i = 0; i < sArrUnderText.GetSize(); i++)
			{
				sArrText.Add(sArrUnderText.GetAt(i));
				sArrUnderText.SetAt(i, "0");
			}
		

			if(!bSide)
			{
				if(pColumn->m_footing.m_bIs)
				{
					sDir	= _T("LEFT");
					CTwinVector tvLower	= pColumn->m_footing.m_tvArrFront.GetTvByInfo("�����ϸ�");
					xyStt.x	= tvLower.GetXyLeft().x;
				}
				else if(m_pStd->m_pRcBridge->IsBoxType())
				{
					xyStt.x = m_pStd->m_pRcBridge->GetStationOnJijum(nJijum) + 3000;
				}
			}
			else
			{
				if(pColumn->m_footing.m_bIs)
				{
					sDir	= _T("RIGHT");
					CTwinVector tvLower;
					CTwinVectorArray tvArrSide, tvArrHidden;
					pColumn->m_footing.GetTvLeftSide(tvArrSide, tvArrHidden, FALSE);
					tvLower	= tvArrSide.GetTvByInfo("�����ϸ�");
					xyStt.x	= tvLower.GetXyRight().x;
				}
				else if(m_pStd->m_pRcBridge->IsBoxType())
				{
					double dStation = m_pStd->m_pRcBridge->GetStationOnJijum(nJijum);
					CDPoint vAngJijum = m_pStd->m_pRcBridge->GetAngleJijum(nJijum);
					double dDistRight = m_pStd->m_pRcBridge->GetDisSlabRightAct(dStation, vAngJijum);
					xyStt.x = dDistRight;
				}
			}
		}

		sArrUnderText.SetAt(sArrUnderText.GetSize()-1, sRound);
		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, bIsTotalDim, FALSE, FALSE);
	}

	CString sDescription	= _T("");
	sDescription.Format("��� ��ö�� ���� %d��", nDan+1);
	CString sMark			= pColumn->GetMarkRebar(COL_REBAR_RO1+nDan, bInput, FALSE, -1, sDescription);
	if(bMarkRebar && tvArr.GetSize() > 0 && m_pStd->IsMarkRebar(sMark))
	{
		CTwinVectorArray tvArrMain;
		pColumn->GetTvRebarMain_Front(tvArrMain, bSide, nDan, -1, FALSE);

		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd); 
		CRebar *pRb = pColumn->GetRebarByDescription(sDescription);

		mark.SetRebar(tvArr, tvArrMain);
		mark.m_sMark					= sMark;
		mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pColumn->m_placeRebarRound[iOUTTER][nDan].m_dDia, pColumn->m_FyShear);
		mark.m_nTypeMarkDir				= TYPE_MARKDIR_RIGHT;
		mark.m_nTypeMarkPosLineRebar	= TYPE_MARKPOS_POS;
		mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POSSCALE;
		mark.m_dPosScaleMarkStt			= 0.7;
		mark.m_dPosScale				= 0.5;
		mark.m_dPosScaleSpace			= nDan == 0 ? 0.5 : 0.25;
		mark.m_dAddHeight				= pColumn->GetWidth(bSide)/2;
		mark.m_dCtc						= pColumn->GetCTCRoundRebar(FALSE, nDan);
		mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;
		mark.MarkRebar();
	}

	*pDomP << Dom;

}

// ��� ���鵵 ũ�ν�Ÿ��ö�� ġ��ǥ��
void CDrawColumn::DimRebarCrossTie_Front(CDomyun *pDomP, CColumnApp *pColumn, long nJijum, long nDimDan, BOOL bSide, BOOL bInput, BOOL bIsTotalDim)
{
	if(!pColumn) return;

	CDomyun Dom(pDomP);
	CHgBaseDrawStd baseDraw;

	CTwinVectorArray tvArr;
	pColumn->GetTvRebarCrossTie_Front(tvArr, bSide);

	if(tvArr.GetSize() > 0)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		// �����ʿ� ġ���� ���� �ϱ� ������ ��ö���� ���� ������ ���� �������� �����Ѵ�.
		CTwinVectorArray tvArrMainOutUpper;
		pColumn->GetTvRebarMain_Front(tvArrMainOutUpper, bSide, 0, 0, FALSE);
		if(tvArrMainOutUpper.GetSize() == 0)
		{
			AfxMessageBox("��ö���� ���������� �������� �ʾƼ� ũ�ν�Ÿ�� ö��ö�� ġ�� ������ �Ұ��� �մϴ�.", MB_ICONSTOP);
			return;
		}

		// ġ���� ���� ��
		// �����ϸ���� ... ö�ٳ�����...
		CPlaceRebarByBlock *pPlace	= &pColumn->m_placeRebarCrossTie;
		CTwinVector tvRebarMain, tvLower;
		tvRebarMain.m_v1	= tvArrMainOutUpper.GetAt(1).GetXyBottom();
		tvRebarMain.m_v2	= tvArrMainOutUpper.GetAt(1).GetXyTop();
		tvLower	= pColumn->GetTvLowerLimitOfMainRebar();//pColumn->m_footing.m_tvArrFront.GetTvByInfo("�����ϸ�");
		tvRebarMain.m_v1.y	= tvLower.m_v1.y;
			
		CDoubleArray dArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		CString sDir	= _T("RIGHT");
		CDPoint xyStt	= tvRebarMain.m_v1;
		CString sCrossTie	= "ũ�ν�Ÿ��";

		m_pStd->GetDimInfoAtPlaceRebarByBlock(dArrDim, sArrText, sArrUnderText, pPlace, "H", TRUE, TRUE);

		CTwinVectorArray tvArrTmp;
		pColumn->GetTvRebarCrossTie_Front(tvArrTmp, bSide);

		dArrDim.Add(tvRebarMain.m_v2.y - tvArrTmp.GetAt(tvArrTmp.GetSize()-1).m_v1.y);
		sArrText.Add("0");
		sArrUnderText.Add("0");

		if(!bInput)
		{
			sArrText.RemoveAll();
			long i = 0; for(i = 0; i < sArrUnderText.GetSize(); i++)
			{
				sArrText.Add(sArrUnderText.GetAt(i));
				sArrUnderText.SetAt(i, "0");
			}
		

			if(!bSide)
			{
				if(pColumn->m_footing.m_bIs)
				{
					sDir	= _T("LEFT");
					CTwinVector tvLower	= pColumn->m_footing.m_tvArrFront.GetTvByInfo("�����ϸ�");
					xyStt.x	= tvLower.GetXyLeft().x;
				}
				else if(m_pStd->m_pRcBridge->IsBoxType())
				{
					xyStt.x = m_pStd->m_pRcBridge->GetStationOnJijum(nJijum) + 3000;
				}
			}
			else
			{
				if(pColumn->m_footing.m_bIs)
				{
					sDir	= _T("RIGHT");
					CTwinVector tvLower;
					CTwinVectorArray tvArrSide, tvArrHidden;
					pColumn->m_footing.GetTvLeftSide(tvArrSide, tvArrHidden, FALSE);
					tvLower	= tvArrSide.GetTvByInfo("�����ϸ�");
					xyStt.x	= tvLower.GetXyRight().x;
				}
				else if(m_pStd->m_pRcBridge->IsBoxType())
				{
					double dStation = m_pStd->m_pRcBridge->GetStationOnJijum(nJijum);
					CDPoint vAngJijum = m_pStd->m_pRcBridge->GetAngleJijum(nJijum);
					double dDistRight = m_pStd->m_pRcBridge->GetDisSlabRightAct(dStation, vAngJijum);
					xyStt.x = dDistRight;
				}
			}
		}

		sArrUnderText.SetAt(sArrUnderText.GetSize()-1, sCrossTie);
		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, bIsTotalDim, FALSE, FALSE);
	}

	*pDomP << Dom;
}

// ��� ��� ũ�ν� Ÿ�� ö�� �׸���.
void CDrawColumn::DrawRebarCrossTie_Plane(CDomyun *pDomP, CColumnApp *pColumn, BOOL bDimRebar, BOOL bMarkRebar, BOOL bInput, long nDimDan, BOOL bRotateVertDim)
{
	if(!pColumn) return;

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CHgBaseDrawStd baseDraw;
	CTwinVectorArray tvArrHor, tvArrVer;
	CString str	= _T("");

	pColumn->GetTvRebarCrossTie_Plan(tvArrHor, TRUE);
	pColumn->GetTvRebarCrossTie_Plan(tvArrVer, FALSE);

	// ���� ���� ��ġ ö��
	baseDraw.DrawTvArray(&Dom, tvArrHor);

	// ����� ũ�ν�Ÿ���� ��� ġ���� ���� �� �ǰ���� ���ְ� �ִ´�. 
	// ġ���� ��� �߰��� �ϳ��� �ֱ�����.
	if(bDimRebar && pColumn->m_nTypeRebarCrossTie == 1)
	{
		double dDia	= pColumn->m_dDiaCrossTie;
		pColumn->SetDataDefaultRebarCrossTie_Plane();
		pColumn->m_dDiaCrossTie	= dDia;
	}

	if(bDimRebar)
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		pColumn->GetTvPlan(FALSE);

		CDoubleArray dArrDim;
		CStringArray sArrText;
		CStringArray sArrUnderText;
		CString sDir	= _T("TOP");
// 		CDPoint xyStt	= CDPoint(0, 0);

		// ��� �ܰ��� ����.
		double dLen	= 0;
		double dTot	= 0;
		CTwinVectorArray tvArrPlane;
		CTwinVectorArray tvArrRebarPlane;
		tvArrPlane	= pColumn->m_tvArrPlan[iOUTTER][iUPPER];
		pColumn->GetTvRebarRound_Plan(tvArrRebarPlane, 0);

		CDRect rect			= tvArrPlane.GetRect();
		CDRect rectRebar	= tvArrRebarPlane.GetRect();
		CDPoint xyStt	= CDPoint(rect.left, rect.top);

		dArrDim.Add((rect.Width()-rectRebar.Width())/2);
		sArrText.Add("0");

		dLen	= pColumn->m_dCTH1;
		dArrDim.Add(dLen);
		sArrText.Add(bInput ? "CTH1" : "0");
		dTot	+= dLen;

		dLen	= pColumn->m_nCTH*pColumn->m_dCTH;
		dArrDim.Add(dLen);
		if(bInput) str	= "NH@CTH";
		else str.Format("%s@%s=%s", COMMA(pColumn->m_nCTH), COMMA(pColumn->m_dCTH), COMMA(dLen)); 
		sArrText.Add(str);
		dTot	+= dLen;

		dLen	= pColumn->m_dCTH2;
		dArrDim.Add(dLen);
		sArrText.Add(bInput ? "CTH2" : "0");
		dTot	+= dLen;

		if(rectRebar.Width() > dTot)
		{
			dLen	= rectRebar.Width() - dTot;
			dArrDim.Add(dLen);
			sArrText.Add("0");
		}

		dArrDim.Add((rect.Width()-rectRebar.Width())/2);
		sArrText.Add("0");

		// ������̸� ö�ٰ����� �Է¹��� �����Ƿ� ��ȣ����
		if(pColumn->m_nTypeRebarCrossTie == 1)
		{
			sArrText.RemoveAll();
			sArrUnderText.RemoveAll();
		}

		baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, FALSE, TRUE);
	}


	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	// ���� ���� ��ġ ö��
	baseDraw.DrawTvArray(&Dom, tvArrVer);
	CDRect rtDom = tvArrVer.GetRect();
	*pDomP << Dom;

	if(bDimRebar)
	{	
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		pColumn->GetTvPlan(FALSE);

		if(bRotateVertDim)
		{
			CDPointArray	xyArrDim;
			CDPoint xyDim(0, 0);

			CStringArray sArrText;
			CStringArray sArrUnderText;
			CString sDir	= _T("BOTTOM");
// 			CDPoint xyStt	= CDPoint(0, 0);
	
			// ��� �ܰ��� ����.
			double dLen	= 0;
			double dTot	= 0;
			CTwinVectorArray tvArrPlane;
			CTwinVectorArray tvArrRebarPlane;
			tvArrPlane	= pColumn->m_tvArrPlan[iOUTTER][iUPPER];
			pColumn->GetTvRebarRound_Plan(tvArrRebarPlane, 0);

			CDRect rect			= tvArrPlane.GetRect();
			CDRect rectRebar	= tvArrRebarPlane.GetRect();
			CDPoint xyStt	= CDPoint(rect.left, rect.top);
			xyDim = xyStt;

			xyArrDim.Add(xyDim);
			sArrText.Add("0");

			dLen = (fabs(rect.Height())-fabs(rectRebar.Height()))/2;
			xyDim.y -= dLen;
			xyArrDim.Add(xyDim);
			sArrText.Add("0");

			dLen	= pColumn->m_dCTV2;
			xyDim.y -= dLen;
			xyArrDim.Add(xyDim);
			sArrText.Add(bInput ? "CTV2" : "0");
			dTot	+= dLen;

			dLen	= pColumn->m_nCTV*pColumn->m_dCTV;
			xyDim.y -= dLen;
			xyArrDim.Add(xyDim);
			if(bInput) str	= "NV@CTV";
			else str.Format("%s@%s=%s", COMMA(pColumn->m_nCTV), COMMA(pColumn->m_dCTV), COMMA(dLen)); 
			sArrText.Add(str);
			dTot	+= dLen;

			dLen	= pColumn->m_dCTV1;
			xyDim.y -= dLen;
			xyArrDim.Add(xyDim);
			sArrText.Add(bInput ? "CTV1" : "0");
			dTot	+= dLen;

			if(fabs(rectRebar.Height()) > dTot)
			{
				dLen	= fabs(rectRebar.Height()) - dTot;
				xyDim.y -= dLen;
				xyArrDim.Add(xyDim);
				sArrText.Add("0");
			}

			dLen = (fabs(rect.Height())-fabs(rectRebar.Height()))/2;
			xyDim.y -= dLen;
			xyArrDim.Add(xyDim);
			sArrText.Add("0");


			baseDraw.AutoDimCont(&Dom, xyStt, xyArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, FALSE, TRUE, TRUE, TRUE);
			Dom.Mirror(rtDom.CenterPoint().y, FALSE);
			*pDomP << Dom;
		}
		else
		{
			CDoubleArray dArrDim;
			CStringArray sArrText;
			CStringArray sArrUnderText;
			CString sDir	= _T("LEFT");
// 			CDPoint xyStt	= CDPoint(0, 0);

			// ��� �ܰ��� ����.
// 			double dLen	= 0;
			double dTot	= 0;
			CTwinVectorArray tvArrPlane;
			CTwinVectorArray tvArrRebarPlane;
			tvArrPlane	= pColumn->m_tvArrPlan[iOUTTER][iUPPER];
			pColumn->GetTvRebarRound_Plan(tvArrRebarPlane, 0);

			CDRect rect			= tvArrPlane.GetRect();
			CDRect rectRebar	= tvArrRebarPlane.GetRect();
			CDPoint xyStt	= CDPoint(rect.left, rect.bottom);

// 			dLen = (fabs(rect.Height())-fabs(rectRebar.Height()))/2;
			dArrDim.Add((fabs(rect.Height())-fabs(rectRebar.Height()))/2);
			sArrText.Add("0");

			double dLen	= pColumn->m_dCTV1;
			dArrDim.Add(dLen);
			sArrText.Add(bInput ? "CTV1" : "0");
			dTot	+= dLen;

			dLen	= pColumn->m_nCTV*pColumn->m_dCTV;
			dArrDim.Add(dLen);
			if(bInput) str	= "NV@CTV";
			else str.Format("%s@%s=%s", COMMA(pColumn->m_nCTV), COMMA(pColumn->m_dCTV), COMMA(dLen)); 
			sArrText.Add(str);
			dTot	+= dLen;

			dLen	= pColumn->m_dCTV2;
			dArrDim.Add(dLen);
			sArrText.Add(bInput ? "CTV2" : "0");
			dTot	+= dLen;

			if(fabs(rectRebar.Height()) > dTot)
			{
				dLen	= fabs(rectRebar.Height()) - dTot;
				dArrDim.Add(dLen);
				sArrText.Add("0");
			}

			dLen = (fabs(rect.Height())-fabs(rectRebar.Height()))/2;
			dArrDim.Add(dLen);
			sArrText.Add("0");

			// ������̸� ö�ٰ����� �Է¹��� �����Ƿ� ��ȣ����
			if(pColumn->m_nTypeRebarCrossTie == 1)
			{
				sArrText.RemoveAll();
				sArrUnderText.RemoveAll();
			}

			baseDraw.AutoDimCont(&Dom, xyStt, dArrDim, sArrText, sArrUnderText, nDimDan, sDir, -1, TRUE, FALSE, TRUE);
			*pDomP << Dom;
		}
	}

	// �����ġ�� ���� ��ġ�� ���� ö���̹Ƿ� ��ũ�� �Ѱ����� �Ѵ�.
	CString sDescription	= _T("");
	if(!bInput)
	{
		long i = 0; for(i = 0; i < pColumn->m_pArrRebar.GetSize(); i++)
		{
			CRebar *pRB	= pColumn->m_pArrRebar.GetAt(i);
			if(pRB->m_sDescription.Find("ũ�ν�Ÿ��", 0) != -1)
			{
				sDescription	= pRB->m_sDescription;
				break;
			}
		}
	}
	CString sMark			= pColumn->GetMarkRebar(COL_REBAR_CT1, bInput, FALSE, -1, sDescription);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		if(pColumn->m_nTypeRebarCrossTie == 1)
		{
			// ���� ���θ� ������
			CTwinVectorArray tvArrTmp;
			tvArrTmp	= tvArrHor;
			tvArrHor.RemoveAll();
			long i = 0; for(i = 0; i < tvArrTmp.GetSize()-2; i+=4)
			{
				tvArrVer.Add(tvArrTmp.GetAt(i));
				tvArrHor.Add(tvArrTmp.GetAt(i+2));
			}
		}

		CTwinVectorArray tvArrHorOther;
		tvArrHorOther	= tvArrVer;
		if(tvArrHorOther.GetSize() == 0)
		{
			CDRect rect	= tvArrHor.GetRect();
			CTwinVector tv;
			tv.m_v1	= CVector(rect.left, rect.top, 0);
			tv.m_v2	= CVector(rect.right, rect.top, 0);
			tvArrHorOther.Add(tv);
			
			tv.m_v1.y	= rect.bottom;
			tv.m_v2.y	= rect.bottom;
			tvArrHorOther.Add(tv);
		}

		if(tvArrVer.GetSize() > 0 && tvArrHor.GetSize() > 0)
		{

			CRebar *pRb = pColumn->GetRebarByDescription(sDescription);
			CDRect rect	= pColumn->m_tvArrPlan[iOUTTER][iUPPER].GetRect();
			CRebarPlacing rb;
			CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
			mark.SetRebar(tvArrHor, tvArrVer);
			mark.m_sMark					= sMark;
			mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pColumn->m_dDiaCrossTie, pColumn->m_FyShear);
			mark.m_nTypeCountPos			= TYPE_COUNTPOS_POSSCALE;
			mark.m_dPosScale				= 0.2;
			mark.m_nTypeMarkDir				= TYPE_MARKDIR_LEFTTAN;
			mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_LOWER;
			CDPoint xyMark					= mark.GetXyMark();
			mark.m_dAddHeight				= xyMark.x - rect.left;
			mark.m_xyDirCountMark			= CDPoint(1, 0);
			mark.m_dCtc						= pColumn->GetCTCRoundRebar(FALSE, 0);
			mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;
			mark.MarkRebar();

			mark.SetRebar(tvArrVer, tvArrHor);
			mark.m_sMark					= sMark;
			mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(pColumn->m_dDiaCrossTie, pColumn->m_FyShear);
			mark.m_nTypeCountPos			= TYPE_COUNTPOS_POSSCALE;
			mark.m_dPosScale				= 0.2;
			mark.m_nTypeMarkDir				= TYPE_MARKDIR_LEFTTAN;
			mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_LOWER;
			xyMark					= mark.GetXyMark();
			mark.m_dAddHeight				= xyMark.y - rect.bottom;
			mark.m_xyDirCountMark			= CDPoint(0, 1);
			mark.m_dCtc						= pColumn->GetCTCRoundRebar(FALSE, 0);
			mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;
			mark.MarkRebar();
		}

	}
	

	*pDomP << Dom;
}

// ��� ��鵵 ���
CDRect CDrawColumn::DrawRebar_PlaneStd(CDomyun *pDomP, CWallApp *pWall, BOOL bDimMainRebar, BOOL bDimCrossRebar, 
	BOOL bMarkRebar, BOOL bInput, BOOL bDrawCrossTie, BOOL bRotateVertDim)
{
	if(pWall == NULL)
	{
		return CDRect(0, 0, 0, 0);
	}

	CColumnApp *pColumn = &pWall->m_Column;
	if(pColumn == NULL)
	{
		return CDRect(0, 0, 0, 0);
	}
	if(pColumn->m_bIs == FALSE)
	{
		return CDRect(0, 0, 0, 0);
	}

	CDomyun Dom(pDomP);

	// ��� �Ϲݵ�
	CTwinVectorArray tvArrReturnColumn;
	DrawPlan(&Dom, pColumn, &tvArrReturnColumn, FALSE, FALSE);

	Dom.SetCalcExtRect();
	CDRect rectTot	= Dom.GetExtRect();

	long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		// ��ö��
		DrawRebarMain_Plan(&Dom, pWall, dan, bDimMainRebar, bMarkRebar, bInput, FALSE);
		if(pWall->IsRoundRectWall())
		{
			DrawRebarMain_Plan(&Dom, pWall, dan, bDimMainRebar, bMarkRebar, bInput, TRUE);
		}
	
		// ��ö��
		DrawRebarRound_Plan(&Dom, pColumn, dan, bDimMainRebar, bMarkRebar, bInput);
	}
	
	if(pWall->IsRoundRectWall())
	{
		// ����ö��
		if(bDrawCrossTie && bInput == FALSE)
			DrawRebarWallShear_Plane(&Dom, pWall, bMarkRebar, bRotateVertDim);
	}
	else
	{
		// ũ�ν� Ÿ��
		if(bDrawCrossTie)
			DrawRebarCrossTie_Plane(&Dom, pColumn, bDimCrossRebar, bMarkRebar, bInput, pColumn->GetCountDan_RebarMain()+1, bRotateVertDim);
	}

	*pDomP << Dom;

	return rectTot;
}

// ��� ��� ��ö�� ġ��
// division�������� ����
// ����, �簢��, Ʈ������ pier���� ������ �ҽ��� ����
// �ٰ����� ���� �ۼ� 
void CDrawColumn::DimRebarMain_Plane_Track(CDomyun *pDomP, CColumnApp *pColumn, long nDimDan, BOOL bExist2Cycle)
{
	BOOL bHole	= FALSE;	// �߰�������?
	double dColH1	= pColumn->m_dH[iOUTTER][iUPPER] - pColumn->m_dR[iOUTTER][iUPPER] * 2;
	double dColB1	= pColumn->m_dB[iOUTTER][iUPPER] - pColumn->m_dR[iOUTTER][iUPPER] * 2;
	double dColR1	= pColumn->m_dR[iOUTTER][iUPPER];
	// �߰��� ���� �� ��� ������ ���� ����
	double dColB1_	= pColumn->m_dB[iINNER][iUPPER];

	double dC1[2][MAX_REBAR_DAN], dC2[2][MAX_REBAR_DAN];
	double dF1[2][MAX_REBAR_DAN], dF2[2][MAX_REBAR_DAN];
	long nR[2][MAX_REBAR_DAN], nB[2][MAX_REBAR_DAN], nE[2][MAX_REBAR_DAN];
	long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		for(long inner = 0; inner < 2; inner++)
		{
			nR[inner][dan]	= pColumn->m_nR[inner][dan];

			dC1[inner][dan]	= pColumn->m_dC1[inner][dan];
			dC2[inner][dan]	= pColumn->m_dC2[inner][dan];
			nB[inner][dan]	= pColumn->m_nB[inner][dan];

			dF1[inner][dan]	= pColumn->m_dF1[inner][dan];
			dF2[inner][dan]	= pColumn->m_dF2[inner][dan];
			nE[inner][dan]	= pColumn->m_nE[inner][dan];
		}
	}

	if(pColumn->m_nType == COL_TYPE_CIRCLE)
	{
		dColB1	= 0;
		dColB1_	= 0;
		dColH1	= 0;
	}
	if(pColumn->m_nType == COL_TYPE_RECTANGLE)
	{
		dColR1	= 0;
	}

	CDomyun Dom(pDomP);
	CDomyun DomArc(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_DIML);
	m_pStd->m_pARoadOptionStd->SetEnvType(&DomArc,HCAD_DIML);
	Dom.SetDimVLen(10);
	Dom.SetDimExo(5);
	DomArc.SetDimVLen(10);
	DomArc.SetDimExo(5);
	DomArc.SetDirection("TOP");
	
	CString sText = _T("");
	CString sUnderText = _T("");
	const double dGap = Dom.Always(1);
	long nNum = 0;
	double dH = pColumn->GetWidth(TRUE);
	double dW = pColumn->GetWidth(FALSE);
	CDPoint xyCen	= pColumn->m_xyCen;
		
	long nDanSu = pColumn->GetCountDan_RebarMain();

	for(dan = 0; dan < nDanSu; dan++)
	{
		double dGapDim = (Dom.GetDimVLen() + Dom.GetDimExo() + (Dom.GetDimLevelDis()*(nDimDan+dan))) * Dom.GetScaleDim() + dColR1;
		double dCoverBOut = pColumn->m_dCover[iOUTTER][dan];
		double dCoverBIn = pColumn->m_dCover[iINNER][dan];
		if(Compare(dCoverBOut, 0.0, "<=") && Compare(dCoverBIn, 0.0, "<=")) continue;

		// �������� B�� H
		if(dColB1 > 0 || dColH1 > 0)
		{
			// ���� ��, �Ʒ�
			if(dColB1 > 0)
			{
				// ����
				nNum = nB[iOUTTER][dan];
				nNum += (dC1[iOUTTER][dan] > 0 ? 1 : 0);
				nNum += (dC2[iOUTTER][dan] > 0 ? 1 : 0);
				sText.Format("DIVISION %d",  nNum);
				if(bHole) sText += " (����)";
				// ����
				nNum = nB[iINNER][dan];
				nNum += (dC1[iINNER][dan] > 0 ? 1 : 0);
				nNum += (dC2[iINNER][dan] > 0 ? 1 : 0);
				sUnderText.Format("DIVISION %d",  nNum);
				if(bHole) sUnderText += " (����)";
				if(!bHole) sUnderText = _T("");

				// ��
				Dom.SetDirection("TOP");
				Dom.DimMoveTo(xyCen.x-dColB1/2, xyCen.y+dH/2, nDimDan+dan);
				Dom.DimLineToExtend(dColB1, nDimDan+dan, sText, sUnderText);

				// �Ʒ�
				Dom.SetDirection("BOTTOM");
				Dom.DimMoveTo(xyCen.x-dColB1/2, xyCen.y-dH/2, nDimDan+dan);
				if(dColB1_==0 && dColR1==0)
					Dom.DimLineToExtend(dColB1, nDimDan+dan, sText , sUnderText);
				else
					Dom.DimLineToExtend(dColB1, nDimDan+dan, sUnderText, sText);
			}
			// ���� ��,��
			if(dColH1 > 0)
			{
				// ����
				nNum = nE[iOUTTER][dan];
				nNum += (dF1[iOUTTER][dan] > 0 ? 1 : 0);
				nNum += (dF2[iOUTTER][dan] > 0 ? 1 : 0);
				if(bExist2Cycle)
					nNum = nNum * 2;
				sText.Format("DIVISION %d",  nNum);
				if(bHole) sText += " (����)";

				// ����
				nNum = nE[iINNER][dan];
				nNum += (dF1[iINNER][dan] > 0 ? 1 : 0);
				nNum += (dF2[iINNER][dan] > 0 ? 1 : 0);
				if(bExist2Cycle)
					nNum = nNum * 2;
				sUnderText.Format("DIVISION %d",  nNum);
				if(bHole) sUnderText += " (����)";
				if(!bHole) sUnderText = _T("");

				// ��
				Dom.SetDirection("LEFT");
				Dom.DimMoveTo(xyCen.x-dW/2, xyCen.y-dColH1/2, nDimDan+dan);
				Dom.DimLineToExtend(dColH1, nDimDan+dan, sText, sUnderText);

				// ��
				Dom.SetDirection("RIGHT");
				Dom.DimMoveTo(xyCen.x+dW/2, xyCen.y-dColH1/2, nDimDan+dan);
				if(dColB1_ == 0 && dColR1 == 0)
					Dom.DimLineToExtend(dColH1, nDimDan+dan, sText, sUnderText);
				else
					Dom.DimLineToExtend(dColH1, nDimDan+dan, sUnderText, sText);
			}
		}

		// ���� ġ��
		if(dColR1 > 0)
		{
			nNum = nR[iOUTTER][dan];
			if(bExist2Cycle)
				nNum = nNum * 2 + 1;
			// �̻��ϰԵ� �����϶��� ö�� ������ �ٸ��� ����. �Ѥ�;
			sText.Format("DIVISION %d", nNum);
			if(bHole) sText += " (����)";
			if(nNum == 0) sText = _T("");

			nNum = nR[iINNER][dan];
			if(bExist2Cycle)
				nNum = nNum * 2 + 1;
			sUnderText.Format("DIVISION %d", nNum);
			if(bHole) sUnderText += " (����)";
			if(!bHole) sUnderText = _T("");
			if(nNum == 0) sUnderText = _T("");

			// �� �ϳ��� ���
			if(dColH1 <= 0 && dColB1 <= 0)
			{
				*pDomP << Dom;
				CDPoint xyStt	= ToDPointFrDegree(0);
				CDPoint xyEnd	= ToDPointFrRadian(ToRadian(359.999));
				double dDist	= DomArc.Always(nDimDan * DomArc.GetDimLevelDis());
				xyStt	= xyCen + xyStt * (dColR1 + dDist);
				xyEnd	= xyCen + xyEnd * (dColR1 + dDist);
				DomArc.DimArcMoveTo(xyCen.x, xyCen.y, dDist+dColR1, xyStt.x, xyStt.y, dan);
				DomArc.DimArcLineTo(xyEnd.x, xyEnd.y, dan);
				*pDomP << DomArc;
				
				//���� �ٱ��� ���̿��� ���̸�ŭ���ش�. ������ 1���̹Ƿ�
				double dOutter = (Dom.GetDimVLen() + Dom.GetDimExo() + (Dom.GetDimLevelDis()*(nDimDan+(nDanSu-1)))) * Dom.GetScaleDim() + dColR1;
				double dDiff = (Dom.GetDimLevelDis()*nDimDan) * Dom.GetScaleDim();
				dGapDim = dOutter - (dDiff * dan);
				Dom.TextOut(xyCen.x, xyCen.y+dGap/2+dGapDim, sText);
				Dom.TextOut(xyCen.x, xyCen.y+dGap/2+dGapDim-Dom.GetTextHeight(), sUnderText);
			}

			// �¿쿡 ���� ���� ���
			else if(dColB1 > 0 && dColH1 <= 0)
			{
				*pDomP << Dom;
				// ��
				CDPoint xyStt	= ToDPointFrDegree(90);
				CDPoint xyEnd	= ToDPointFrRadian(ToRadian(270));
				double dDist	= DomArc.Always(nDimDan * DomArc.GetDimLevelDis());
				xyStt	= CDPoint(xyCen.x-dColB1/2, xyCen.y) + xyStt * (dColR1 + dDist);
				xyEnd	= CDPoint(xyCen.x-dColB1/2, xyCen.y) + xyEnd * (dColR1 + dDist);

				DomArc.DimArcMoveTo(xyCen.x-dColB1/2, xyCen.y, dDist+dColR1, xyStt.x, xyStt.y, dan);
				DomArc.DimArcLineTo(xyEnd.x, xyEnd.y, nDimDan+dan);
				*pDomP << DomArc;

				Dom.TextOut(xyCen.x-dColB1/2-dGapDim, xyCen.y+dGap/2, sText);
				Dom.TextOut(xyCen.x-dColB1/2-dGapDim, xyCen.y-Dom.GetTextHeight()-dGap/2, sUnderText);
				Dom.Rotate(CDPoint(xyCen.x-dColB1/2-dGapDim, xyCen.y), CDPoint(0, 1));
				*pDomP << Dom;
				
				// ��
				xyStt	= ToDPointFrDegree(270);
				xyEnd	= ToDPointFrDegree(90);
				xyStt	= CDPoint(xyCen.x+dColB1/2, xyCen.y) + xyStt * (dColR1 + dDist);
				xyEnd	= CDPoint(xyCen.x+dColB1/2, xyCen.y) + xyEnd * (dColR1 + dDist);

				DomArc.DimArcMoveTo(xyCen.x+dColB1/2, xyCen.y, dDist+dColR1, xyStt.x, xyStt.y, dan);
				DomArc.DimArcLineTo(xyEnd.x, xyEnd.y, dan);
				*pDomP << DomArc;

				Dom.TextOut(xyCen.x+dColB1/2+dGapDim, xyCen.y+dGap/2, sUnderText);
				Dom.TextOut(xyCen.x+dColB1/2+dGapDim, xyCen.y-Dom.GetTextHeight()-dGap/2, sText);
				Dom.Rotate(CDPoint(xyCen.x+dColB1/2+dGapDim, xyCen.y), CDPoint(0, 1));
				*pDomP << Dom;	
			}
			// ���Ͽ� ���� ���� ���
			else if(dColB1 <= 0 && dColH1 > 0)
			{
				*pDomP << Dom;
				// ��
				CDPoint xyStt	= ToDPointFrDegree(0);
				CDPoint xyEnd	= ToDPointFrRadian(ToRadian(180));
				double dDist	= DomArc.Always(nDimDan * DomArc.GetDimLevelDis());
				xyStt	= CDPoint(xyCen.x, xyCen.y+dColH1/2) + xyStt * (dColR1 + dDist);
				xyEnd	= CDPoint(xyCen.x, xyCen.y+dColH1/2) + xyEnd * (dColR1 + dDist);

				DomArc.DimArcMoveTo(xyCen.x, xyCen.y+dColH1/2, dDist+dColR1, xyStt.x, xyStt.y, dan);
				DomArc.DimArcLineTo(xyEnd.x, xyEnd.y, dan);
				*pDomP << DomArc;

				Dom.TextOut(xyCen.x, xyCen.y+dColH1/2+dGapDim+dGap/2, sText);
				Dom.TextOut(xyCen.x, xyCen.y+dColH1/2+dGapDim-Dom.GetTextHeight()-dGap/2, sUnderText);
								
				// ��
				xyStt	= ToDPointFrDegree(180);
				xyEnd	= ToDPointFrRadian(ToRadian(0));
				xyStt	= CDPoint(xyCen.x, xyCen.y-dColH1/2) + xyStt * (dColR1 + dDist);
				xyEnd	= CDPoint(xyCen.x, xyCen.y-dColH1/2) + xyEnd * (dColR1 + dDist);

				DomArc.DimArcMoveTo(xyCen.x, xyCen.y-dColH1/2, dDist+dColR1, xyStt.x, xyStt.y, dan);
				DomArc.DimArcLineTo(xyEnd.x, xyEnd.y, dan);
				*pDomP << DomArc;

				Dom.TextOut(xyCen.x, xyCen.y-dColH1/2-dGapDim+dGap/2, sUnderText);
				Dom.TextOut(xyCen.x, xyCen.y-dColH1/2-dGapDim-Dom.GetTextHeight()-dGap/2, sText);
			}
			// �����¿쿡 ��� ���� �ִ� ���
			else if(dColB1 > 0 && dColH1 > 0)
			{
				*pDomP << Dom;
				// �»�
				CDPoint xyStt	= ToDPointFrDegree(90);
				CDPoint xyEnd	= ToDPointFrRadian(ToRadian(180));
				double dDist	= DomArc.Always(nDimDan * DomArc.GetDimLevelDis());
				xyStt	= CDPoint(xyCen.x-dColB1/2, xyCen.y+dColH1/2) + xyStt * (dColR1 + dDist);
				xyEnd	= CDPoint(xyCen.x-dColB1/2, xyCen.y+dColH1/2) + xyEnd * (dColR1 + dDist);

				DomArc.DimArcMoveTo(xyCen.x-dColB1/2, xyCen.y+dColH1/2, dDist+dColR1, xyStt.x, xyStt.y, dan);
				DomArc.DimArcLineTo(xyEnd.x, xyEnd.y, dan);
				*pDomP << DomArc;

				
				CDPoint xyText = CDPoint(xyCen.x-dColB1/2, xyCen.y+dColH1/2);
				xyText += (ToDPointFrRadian(ToRadian(135)) * (dGapDim));
				Dom.TextOut(xyText + (CDPoint(0, 1)*dGap/2), sText);
				Dom.TextOut(xyText + (CDPoint(0, -1) *(Dom.GetTextHeight()+ dGap/2)), sUnderText);
				Dom.Rotate(xyText, ToDPointFrRadian(ToRadian(45)));			
				*pDomP << Dom;

				// ����
				xyStt	= ToDPointFrDegree(180);
				xyEnd	= ToDPointFrRadian(ToRadian(270));
				xyStt	= CDPoint(xyCen.x-dColB1/2, xyCen.y-dColH1/2) + xyStt * (dColR1 + dDist);
				xyEnd	= CDPoint(xyCen.x-dColB1/2, xyCen.y-dColH1/2) + xyEnd * (dColR1 + dDist);

				DomArc.DimArcMoveTo(xyCen.x-dColB1/2, xyCen.y-dColH1/2, dDist+dColR1, xyStt.x, xyStt.y, dan);
				DomArc.DimArcLineTo(xyEnd.x, xyEnd.y, dan);
				*pDomP << DomArc;

				xyText = CDPoint(xyCen.x-dColB1/2, xyCen.y-dColH1/2);
				xyText += (ToDPointFrRadian(ToRadian(225)) * (dGapDim));
				Dom.TextOut(xyText + (CDPoint(0, 1)*dGap/2), sUnderText);
				Dom.TextOut(xyText + (CDPoint(0, -1) *(Dom.GetTextHeight()+ dGap/2)), sText);
				Dom.Rotate(xyText, ToDPointFrRadian(ToRadian(-45)));			
				*pDomP << Dom;

				// ����
				xyStt	= ToDPointFrDegree(270);
				xyEnd	= ToDPointFrRadian(ToRadian(0));
				xyStt	= CDPoint(xyCen.x+dColB1/2, xyCen.y-dColH1/2) + xyStt * (dColR1 + dDist);
				xyEnd	= CDPoint(xyCen.x+dColB1/2, xyCen.y-dColH1/2) + xyEnd * (dColR1 + dDist);

				DomArc.DimArcMoveTo(xyCen.x+dColB1/2, xyCen.y-dColH1/2, dDist+dColR1, xyStt.x, xyStt.y, dan);
				DomArc.DimArcLineTo(xyEnd.x, xyEnd.y, dan);
				*pDomP << DomArc;

				xyText = CDPoint(xyCen.x+dColB1/2, xyCen.y-dColH1/2);
				xyText += (ToDPointFrRadian(ToRadian(315)) * (dGapDim));
				Dom.TextOut(xyText + (CDPoint(0, 1)*dGap/2), sUnderText);
				Dom.TextOut(xyText + (CDPoint(0, -1) *(Dom.GetTextHeight()+ dGap/2)), sText);
				Dom.Rotate(xyText, ToDPointFrRadian(ToRadian(45)));			
				*pDomP << Dom;

				// ���
				xyStt	= ToDPointFrDegree(0);
				xyEnd	= ToDPointFrRadian(ToRadian(90));
				xyStt	= CDPoint(xyCen.x+dColB1/2, xyCen.y+dColH1/2) + xyStt * (dColR1 + dDist);
				xyEnd	= CDPoint(xyCen.x+dColB1/2, xyCen.y+dColH1/2) + xyEnd * (dColR1 + dDist);

				DomArc.DimArcMoveTo(xyCen.x+dColB1/2, xyCen.y+dColH1/2, dDist+dColR1, xyStt.x, xyStt.y, dan);
				DomArc.DimArcLineTo(xyEnd.x, xyEnd.y, dan);
				*pDomP << DomArc;

				xyText = CDPoint(xyCen.x+dColB1/2, xyCen.y+dColH1/2);
				xyText += (ToDPointFrRadian(ToRadian(45)) * (dGapDim));
				Dom.TextOut(xyText + (CDPoint(0, 1)*dGap/2), sText);
				Dom.TextOut(xyText + (CDPoint(0, -1) *(Dom.GetTextHeight()+ dGap/2)), sUnderText);
				Dom.Rotate(xyText, ToDPointFrRadian(ToRadian(-45)));			
				*pDomP << Dom;	
				
			}
		}
	}

	*pDomP << Dom;
}

// ������ ���鵵�� ���� ����
void CDrawColumn::ApplyCuttingArea_Front(CDomyun *pDomP, CColumnApp *pColumn, CTwinVectorArray &tvArr, long nDan, BOOL bDrawCuttingArea, BOOL bSide)
{
	CDomyun Dom(pDomP);
	long envType	= m_pStd->m_pARoadOptionStd->GetEnvType(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);


	// 1���� ��� 2,3���� ��� cuttingArea�� �߶󳽴�. ////////////////////////////
	// 2,3���� ��� �ش� cuttingArea�� �׸���.
	CRebarPlacing rb;
	CHgBaseDrawStd baseDraw;
	if(nDan == 0)
	{
		long dan = 0; for(dan = 1; dan < MAX_REBAR_DAN; dan++)
		{
			CDPoint xyCen(0, 0);
			double dR	= 0;
			pColumn->GetInfoCuttingCircleFront(xyCen, dR, dan, bSide);
			
			if(Compare(dR, 0.0, ">"))
			{
				rb.GetTvArrayByCircleArea(tvArr, xyCen, dR, FALSE);
				if(bDrawCuttingArea)
				{
					m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CUTL);
					Dom.Arc(xyCen, dR, -100, 200);
				}
			}
		}
	}
	else
	{
		CDPoint xyCen(0, 0);
		double dR	= 0;
		pColumn->GetInfoCuttingCircleFront(xyCen, dR, nDan, bSide);
		
		if(Compare(dR, 0.0, ">"))
		{
			rb.GetTvArrayByCircleArea(tvArr, xyCen, dR, TRUE);
			
			if(bDrawCuttingArea)
			{
				m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CUTL);
				Dom.Arc(xyCen, dR, -100, 200);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////////

	*pDomP << Dom;
	m_pStd->m_pARoadOptionStd->SetEnvType(pDomP, envType);
}

/**
@brief
	- ��� ��� ��ö�� ġ�� �Է¸��

@return
	- 

@param
	- 
*/
void CDrawColumn::DimRebarMain_Plane_Track_InputMode(CDomyun *pDom, CColumnApp *pColumn, long nDan, long nDimDan)
{
	if(pColumn == NULL)
	{
		return;
	}

	CARcBridgeDataStd *pDataStd = m_pStd->m_pARcBridgeDataStd;
	if(pDataStd == NULL)
	{
		return;
	}

	double dHorzC1 = pColumn->m_dC1[iOUTTER][nDan];
	double dHorzB = pColumn->m_nB[iOUTTER][nDan];
	double dHorzC = pColumn->m_dC[iOUTTER][nDan];
	double dHorzC2 = pColumn->m_dC2[iOUTTER][nDan];

	double dVertF1 = pColumn->m_dF1[iOUTTER][nDan];
	double dVertE = pColumn->m_nE[iOUTTER][nDan];
	double dVertF = pColumn->m_dF[iOUTTER][nDan];
	double dVertF2 = pColumn->m_dF2[iOUTTER][nDan];

	double dJewonH = pColumn->m_dH[iOUTTER][iUPPER] - pColumn->m_dR[iOUTTER][iUPPER] * 2;
	double dJewonB = pColumn->m_dB[iOUTTER][iUPPER] - pColumn->m_dR[iOUTTER][iUPPER] * 2;
//	double dJewonR = pColumn->m_dR[iOUTTER][iUPPER] / 2;
	double dCover = pColumn->m_dCover[iOUTTER][nDan];
	
	if(pColumn->m_nType == COL_TYPE_CIRCLE)
	{
		dJewonB	= 0;
		dJewonH	= 0;
		dCover = 0;
	}
	else if(pColumn->m_nType == COL_TYPE_RECTANGLE)
	{
//		dJewonR	= 0;		
	}
	else if(pColumn->m_nType == COL_TYPE_POLY)
	{
		dCover = 0;
	}
	else if(pColumn->m_nType == COL_TYPE_TRACK)
	{
		dCover = 0;
	}

	CDomyun Dom(pDom);
	CDomyun DomDim(pDom);;
	CHgBaseDrawStd baseDraw;

	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	m_pStd->m_pARoadOptionStd->SetEnvType(&DomDim, HCAD_DIML);
	
	double dHeight = pColumn->GetWidth(TRUE);
	double dWidth = pColumn->GetWidth(FALSE);
	
	CString strText = _T("");

	CDPoint xyCen = pColumn->m_xyCen;
	CDPoint xyDimStt(0,0);

	CDoubleArray dArrDistDim;

	CStringArray strArrText, strArrUnderText;
		
	// �Ʒ�
	if(pDataStd->IsExistMainRebarStraightHorz(pColumn, nDan) == TRUE)
	{
		dArrDistDim.RemoveAll();
		strArrText.RemoveAll();
		strArrUnderText.RemoveAll();

		double dRotateAng = 0;
		if(pColumn->m_nType == COL_TYPE_POLY)
		{
			CTwinVectorArray tvArrRebarPlan;
			pColumn->GetTvRebarRound_Plan(tvArrRebarPlan, nDan);

			CTwinVector tv;
			tvArrRebarPlan.GetTvByInfo(NOTRANS("1��°��"), tv);

			CDPoint xyStt = tv.m_v1;
			CDPoint xyEnd = tv.m_v2;
			CDPoint xyDir = (xyStt - xyEnd).Unit();

			xyDimStt = xyEnd;
			dRotateAng = CDPoint(1,0).BetweenAngle(xyDir).GetAngleDegree();
		}
		else
		{
			xyDimStt.x = xyCen.x - dJewonB/2 + dCover;
			xyDimStt.y = xyCen.y - dHeight/2;			
		}

		dArrDistDim.Add(dHorzC1);
		strArrText.Add(_T("C1"));
		strArrUnderText.Add(_T(""));

		for(long ix = 0; ix < dHorzB; ++ix)
		{
			dArrDistDim.Add(dHorzC);
			strArrText.Add(_T("B@C"));

			strText.Format(_T("(%.0f@%s=%s)"), dHorzB, COMMA(dHorzC), COMMA(dHorzB*dHorzC));
			strArrUnderText.Add(strText);
		}

		dArrDistDim.Add(dHorzC2);
		strArrText.Add(_T("C2"));
		strArrUnderText.Add(_T(""));

		baseDraw.AutoDimCont(&DomDim, xyDimStt, dArrDistDim, strArrText, strArrUnderText, nDimDan + nDan, _T("BOTTOM"), 
			-1, TRUE, TRUE, FALSE);

		DomDim.Rotate(xyDimStt, ToDPointFrDegree(dRotateAng));

		Dom << DomDim;
	}

	// ��
	if(pDataStd->IsExistMainRebarStraightVert(pColumn, nDan) == TRUE)
	{
		dArrDistDim.RemoveAll();
		strArrText.RemoveAll();
		strArrUnderText.RemoveAll();

		xyDimStt.x = xyCen.x - dWidth/2;
		xyDimStt.y = xyCen.y - dJewonH/2 + dCover;

		dArrDistDim.Add(dVertF1);
		strArrText.Add(_T("F1"));
		strArrUnderText.Add(_T(""));

		for(long ix = 0; ix < dVertE; ++ix)
		{
			dArrDistDim.Add(dVertF);
			strArrText.Add(_T("E@F"));

			strText.Format(_T("(%.0f@%s=%s)"), dVertE, COMMA(dVertF), COMMA(dVertE*dVertF));
			strArrUnderText.Add(strText);
		}

		dArrDistDim.Add(dVertF2);
		strArrText.Add(_T("F2"));
		strArrUnderText.Add(_T(""));

		baseDraw.AutoDimCont(&DomDim, xyDimStt, dArrDistDim, strArrText, strArrUnderText, nDimDan + nDan, _T("LEFT"), 
			-1, TRUE, TRUE, FALSE);

		Dom << DomDim;
	}

	*pDom << Dom;
}

/**
@brief
	- ��鵵�� ��,��� ��ö�� Mark�� �׸���

@param
	- 
*/
void CDrawColumn::DrawRebarMainMark_Plan(CDomyun *pDomP, CWallApp *pWall, long nDan, BOOL bLeft, BOOL bInput, BOOL b2Cycle)
{
	if(pWall == NULL)
	{
		return;
	}
	if(pWall->IsRoundRectWall() == FALSE && b2Cycle)
		return;

	CColumnApp *pColumn = &pWall->m_Column;
	if(pColumn == NULL)
	{
		return;
	}
	if(pColumn->m_bIs == FALSE)
	{
		return;
	}
	if(nDan == -1)
	{
		nDan = MAX_REBAR_DAN - 1;
	}
	if(nDan < 0 || nDan > MAX_REBAR_DAN - 1) 
	{
		return;
	}

	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
		
	CDPointArray xyArr, xyArrDirApplyScale;
	if(pWall->IsRoundRectWall())
		pWall->GetXyRebarMain_Plan_For_Round_Wall(xyArr, xyArrDirApplyScale, nDan, b2Cycle);
	else
		pColumn->GetXyRebarMain_Plan(xyArr, xyArrDirApplyScale, nDan);

	if(xyArr.GetSize() <= 0)
	{
		return;
	}

	CString sMark = _T("");
	CString sDescription = _T("");
	if(pWall->IsRoundRectWall())
	{
		sDescription.Format("�߰�����%d %s %d�� %s 1", pWall->m_footing.m_nIndexFooting, bLeft? _T("������") : _T("������"), nDan+1, b2Cycle? _T("��") : _T("��"));
		sMark = pWall->GetMarkRebarMain(0, FALSE, 0, FALSE, FALSE, sDescription, nDan);
	}
	else
	{
		sDescription.Format("��� ��ö�� ���� %d��", nDan+1);
		sMark = pColumn->GetMarkRebar(COL_REBAR_MO1 + nDan, bInput, FALSE, -1, sDescription);
	}

	if(m_pStd->IsMarkRebar(sMark))
	{
		if(bLeft == FALSE)
		{
			CRebarPlacing rb;
			rb.ReverseRebar(xyArr);
		}

		CDRect rect	= pColumn->m_tvArrPlan[iOUTTER][iUPPER].GetRect();

		// ö�� ��ũ�� ��Ȯ�� ��ġ�� ã�� ���� �ʿ��� ö��3���� �̾Ƴ��� ��ũ�Ѵ�
		double dPosX = bLeft == TRUE ? rect.left : rect.right;

		CDPoint xyPos = CDPoint(dPosX, (rect.top+rect.bottom)/2 - (b2Cycle? rect.Height()/4 : 0));

		if(nDan == 1)
		{
			xyPos = CDPoint(dPosX, rect.bottom - rect.Height()/(b2Cycle?4 : 3));
		}
		else if(nDan == 2)
		{
			xyPos = CDPoint(dPosX, rect.bottom - rect.Height()/(b2Cycle?7 : 6));
		}
		///////////////////////////////////////////////////////////////////////////////

		// ö���� ������ ���Ѵ�
		long nCycle = b2Cycle? 1 : 0;

		double dRebarDia = pWall->GetColumnMainRebarDia(nCycle, nDan, bLeft);

		// ����
		CRebar *pRb = pColumn->GetRebarByDescription(sDescription);
		CMarkDomyun mark(&Dom, m_pStd->m_pARoadOptionStd);
		mark.SetRebar(xyArr);
		
		mark.m_sMark					= sMark;
		mark.m_sDia						= m_pStd->m_pARcBridgeDataStd->GetStringDia(dRebarDia, pColumn->m_Fy);
		mark.m_nTypeMark				= TYPE_MARK_POINT;
		mark.m_nTypeMarkPointUpAndDown	= TYPE_MARKPOINT_UPPER;
		mark.m_nTypeMarkSttPos			= TYPE_MARKSTTPOS_POS;
		mark.m_nTypeMarkDir				= TYPE_MARKDIR_POS;
		mark.m_xyPosMarkStt				= xyPos;
		mark.m_dCtc						= pColumn->GetCTCMainRebar(FALSE, nDan);
		mark.m_nTypeRebar				= pRb? pRb->m_nTypeRebar : -1;
		mark.MarkRebar(); 
	}

	*pDomP << Dom;
}

void CDrawColumn::DrawRebarWallShear_Plane( CDomyun *pDomP, CWallApp *pWall, BOOL bMarkRebar, BOOL bRotateVertDim/*=FALSE*/ )
{
	if(!pWall) return;

	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_BARC);

	CHgBaseDrawStd baseDraw;
	CTwinVectorArray tvArrShear;
	
	pWall->GetTvRebarShear_Plane(tvArrShear);
	baseDraw.DrawTvArray(&Dom, tvArrShear);

	CString sDecription	= _T("����ö��");
	CString sMark = pWall->GetMarkRebarShear(0, FALSE, FALSE, -1, sDecription);
	if(bMarkRebar && m_pStd->IsMarkRebar(sMark))
	{
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		int nSize = tvArrShear.GetSize();
		
		CDPoint xyMark(0, 0);
		vector<CDPoint> xy_shear;
		for(long ix=0; ix<nSize; ++ix)
		{
			CTwinVector tv = tvArrShear.GetAt(ix);
			xyMark = tv.GetXyMid();
			
			Sym.RebarCountMarkOne(&Dom, xyMark);
			xy_shear.push_back(xyMark);
		}
		if(xy_shear.size() > 0)
		{
			CDPoint xy1 = xy_shear.front();
			CDPoint xy2 = xy_shear.back();
			Dom.LineTo(xy1, xy2);

			if(xy_shear.size() > 1)
			{
				long idx = xy_shear.size() / 3;
				xy1 = xy_shear[idx];
				xy2 = xy_shear[idx+1];
				xyMark = xy1 + (xy2 - xy1).Unit() * ~(xy1 - xy2) / 3;
			}
			else
			{
				xyMark = xy1;
			}

			double dRebarDia = pWall->m_placeByBlock_ShearRebar.m_dDia;
			CString sDia = m_pStd->m_pARcBridgeDataStd->GetStringDia(dRebarDia, pWall->m_FyShear);
			// Sym.RebarMarkLineSide(&Dom, xyMark, sMark, sDia, FALSE, pWall->m_dW/2, FALSE);

			SRebarMarkInputData mark_input_data;
			SRebarMarkviewData mark_view_data;

			mark_input_data.mark = sMark;
			mark_input_data.dia = sDia;
			mark_input_data.position = xyMark;
			mark_input_data.ctc = pWall->GetCTCRebarShear();
			mark_input_data.type_rebar = pWall->m_rebarShearDetail.GetRebarType();
			mark_input_data.height	= FALSE, pWall->m_dW/2;
			mark_input_data.left_dir = FALSE;
			
			mark_view_data.apply_basic_height = TRUE;
			mark_view_data.count_mark = TRUE;
			mark_view_data.line_rebar = TRUE;
			mark_view_data.mark_pos = eMarkSide;

			Sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);
		}
	}

	*pDomP << Dom;
}
