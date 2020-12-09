// ARcBridgeCalcArchRib.cpp: implementation of the CARcBridgeCalcArchRib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARcBridgeCalculate.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARcBridgeCalcArchRib::CARcBridgeCalcArchRib()
{
	m_dWidth	= 0;
	m_dRibJigan	= 0;
	m_dRibRaise	= 0;

	m_dArrDelta.RemoveAll();
	m_dArrRamda.RemoveAll();
	m_dArrAngTan.RemoveAll();
	m_dArrHcr.RemoveAll();
}

CARcBridgeCalcArchRib::~CARcBridgeCalcArchRib()
{
	AhTPADelete(&m_pList,(CGirderAndColumn*)0);
}

void CARcBridgeCalcArchRib::CalculateAll()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	m_dWidth	= toM(pBri->GetWidthArchRib(0));

	pBri->TrimRibByFootingUpper();
	MakeDesignList();
	
	long nSize = m_pList.GetSize();
	if(nSize==0) return;
	
	SetConcDataByCol();
	SetConcDataBySlab();
	CalculateArchRib();
}

void CARcBridgeCalcArchRib::WriteCalcArchRibDesign(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$s+05$e$h$a%d.%d %s$n", nIndexTitle, nIndexMiddle++, "아치 리브 선형의 계산");

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$h$p[ARCH_LINE_Y]$n$n");
	XLText->AddFormatText("$h$m+12$p[ARCH_LINE_GRAPH]");
	XLText->AddFormatText("$h$rm  =  Ws/Wc (1.0~3.0정도)$n");
	XLText->AddFormatText("$h$rWs = 스프리깅에서의 단위 길이당 사하중(t/m)$n");
	XLText->AddFormatText("$h$rWc = 크라운에서의 단위 길이당 사하중(t/m)$n$n");

	XLText->AddFormatText("$h$p[ARCH_LINE_K]$n");
	XLText->AddFormatText("$h$rζ = x / l1$n");
	XLText->AddFormatText("$h$rl1 = l/2 (m)$n");
	XLText->AddFormatText("$h$r$rl  : 지간 길이 (m)$n");
	XLText->AddFormatText("$h$r$rf  : 라이즈 (m)$n");
	*XLText += 4;
	
	XLText->AddFormatText("$h∴ 다음과 같은 모양의 2차 포물선이 된다.$n");
	DrawGraphXY(XLText);

	*XLText += 1;
	XLText->AddFormatText("$h$c*$r아치리브의 면내 및 면외방향의 좌굴에 대한 안전성은 도로교 설계기준(2000)에 소개된$n");
	XLText->AddFormatText("$h$rDIN(1981.4)의 검사방법을 사용하여 콘크리트 아치교의 좌굴을 검토하였음.$n");
	*XLText += 1;
	MakeTableArchRibJewon(XLText);

	XLText->AddFormatText("$s+30$h");
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$s+05$e$h$a%d.%d %s$n", nIndexTitle, nIndexMiddle++, "단면설계");
	
//	XLText->GoPosition(NO_MOVE,COL_POS_2);
	CDPoint vAng = pBri->m_pbRibCen.GetXyDirTanByDist(-pBri->m_pbRibCen.m_dDistTotal/4);
	long nIndexSmall	= 1;
//	long nIndexSub	= 0;
// 	double dHeight	= 0.;
	double dAreaL_4 = m_dWidth * toM(pBri->GetThickArchRib(pBri->m_pbRibCen.m_dDistTotal/4, TRUE));
	double dAngL_4	= fabs(vAng.GetAngleDegree());
	double dLenRib	= toM(GetLenArchRib()) + toM(pBri->GetThickArchRibForCalc(CHECK_POS_LEFT, TRUE) + pBri->GetThickArchRibForCalc(CHECK_POS_RIGHT, TRUE));

	long nSize = m_pList.GetSize();
	long n=0; for(n=0; n<nSize; n++)
	{
		long nIndexSub = 1;
		CGirderAndColumn *pGirCol = m_pList.GetAt(n);
		CConcBasic *pConc = pGirCol->pConcColumn[0];

		if(n>0) XLText->AddFormatText("$s+30");
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$s+14$h$e %d)$r%s$n", nIndexSmall++, pConc->m_sTitle);

		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$h$c%s$r설계단면력$n$h", GetNumberString(nIndexSub++,NUM_CIRCLE));
		MakeTableColumnLoad(XLText, pGirCol);
		*XLText += 1;

		// 평균단면2차 모멘트 옵션일 경우(8966)
		double dHeight = toM(pConc->m_Ho);
		if(pBri->m_nArchRibSectionMomentPos == 0)
			dHeight = toM(pBri->GetThickArchRib(pBri->m_pbRibCen.m_dDistTotal/4, TRUE));

		double dI = (m_dWidth*pow(dHeight,3))/12;
		double dRaiseRatio = m_dRibRaise/m_dRibJigan;
		double dAng = m_dArrAngTan.GetAt(n);
		double dDelta = m_dArrDelta.GetAt(n);
		double dRamda = m_dArrRamda.GetAt(n);
		double dHcr	=  m_dArrHcr.GetAt(n);
		double dLenExc = dDelta * dLenRib;
		double dE = GetValueUnitChange(pBri->m_pARcBridgeDataStd->m_Ec * 10, UNIT_CGS_TONF_M2, pBri->m_pARcBridgeDataStd->m_szTonf_M2);	// kgf/cm2 ==> ton/m2;
		double dAlpha = GetValueAlphaByRaiseRatio(dRaiseRatio);
		
		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$h$c-$r세장비 검토 ( 도.설.기 4.13 )$n$h");
		XLText->AddFormatText("$h$r$p[ARCH_LEN_RATIO]$n$n");
		XLText->AddFormatText("$h$r$cB$r$c=$r$e$me1$i$f3%.3f$m+02m$m+07$cl$r$c=$r$e$me1$i$f3%.3f$m+02m$r(아치리브 지간)$n", m_dWidth, m_dRibJigan);
		XLText->AddFormatText("$h$r$cH$r$c=$r$e$me1$i$f3%.3f$m+02m$m+07$cf$r$c=$r$e$me1$i$f3%.3f$m+02m$r(아치리브 라이즈)$n", dHeight, m_dRibRaise);
		XLText->AddFormatText("$h$r$c$p[RIB_AREA_L4]$r$c=$r$e$me1$i$f3%.3f$m+02m$m+07$cf/l$r$c=$r$e$me1$i$f3%.3f$m+03(라이즈비)$n", dAreaL_4, dRaiseRatio);
		CString str = _T("I");
		if(pBri->m_nArchRibSectionMomentPos == 0)
			str = _T("Im");
		XLText->AddFormatText("$h$r$c$p[RIB_THETA_L4]$r$c=$r$e$me1$i$f3%.3f$m+02°$m+07$c%s$r$c=$r$e$me1$i$f3%.3f$m+02m⁴$n", dAngL_4, str, dI);
		XLText->AddFormatText("$h$r$cL$r$c=$r$e$me1$i$f3%.3f$m+02m$r(L = δ x ι)$m+06$cδ$r$c=$r$e$me1$i$f3%.3f$n", dLenExc, dDelta);
		XLText->AddFormatText("$h$r$cι$r$c=$r$e%.3f + %.3f + %.3f $n", toM(GetLenArchRib()), toM(pBri->GetThickArchRibForCalc(CHECK_POS_LEFT, TRUE)), toM(pBri->GetThickArchRibForCalc(CHECK_POS_RIGHT, TRUE)));
		*XLText += 1;

		XLText->GoPosition(NO_MOVE,COL_POS_3);
		MakeTableDeltaByRaiseRatio(XLText);

		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$h$r$cλ$r$c=$r%.3f x √((%.3f x cos(%.3f) / %.3f)$m+11$c=$r$me2$f3%.3f", dLenExc, dAreaL_4, dAngL_4, dI, dRamda);
		if(dRamda < 20) XLText->AddFormatText("$n$h$m+02--> λ < 20 : 좌굴검사는 필요치 않음$n");
		else if(dRamda >= 20 && dRamda < 70)
		{
			XLText->AddFormatText("$n$h$m+02--> 20 < λ < 70 : 유한변형에 의한 영향을 편심하중에 의한 휨모멘트로 치환하여$n");
			XLText->AddFormatText("$h$m+06발생 모멘트에 더하여 단면의 극한 휨모멘트에 대한 안정성을 검토한다.$n");
		}
		else if(dRamda >= 70 && dRamda < 200)
		{
			XLText->AddFormatText("$n$h$m+02--> 70 < λ < 200 : 유한변형에 의한 영향에 더하여, 철근콘크리트 부재의 재료의$n");
			XLText->AddFormatText("$h$m+06비선형성에 의한 영향을 고려하여 좌굴에 대한 안정성을 검사한다.$n");
		}
		else XLText->AddFormatText("$n$h$m+02--> λ > 200 : 구조물로서 적합하지 않음$n");

		if(dRamda > 70) continue;

		*XLText += 1;

		if(dRamda >= 20 && dRamda < 70)
		{
			XLText->AddFormatText("$h$c-$r좌굴 파라미터(ξ)$n$h");
			XLText->AddFormatText("$h$r$p[ARCH_MD]$n$n");
			XLText->AddFormatText("$h$r$r$cMd$r$c=$r유한변위 이론에 의한 휨모멘트$n");
			XLText->AddFormatText("$h$r$r$cMe$r$c=$r미소변위 이론에 의한 휨모멘트$n");
			XLText->AddFormatText("$h$r$r$cξ$r$c=$r HD / Hcr$n");
			XLText->AddFormatText("$h$m+03$cHD$r$c:$r작용 수평력(축력에 아치리브 지점의 접선각도 cos %.3f˚를 곱해서 구한다.$n", dAng);
			XLText->AddFormatText("$h$m+03$cHcr$r$c:$r아치리브의 좌굴 하중$n");
			XLText->AddFormatText("$h$m+03$p[ARCH_HCR1]$n$n");
			XLText->AddFormatText("$h$m+05$c=$r%.2f x (%.0f x %.3f / %.3f²)$n", dAlpha, dE, dI, m_dRibJigan);
//			식2 사용시
//			XLText->AddFormatText("$h$m+05$c=$r[4 x π²x (1-8(%.3f/%.3f)²)] x (%.0f x %.3f / %.3f²)$n", m_dRibRaise, m_dRibJigan, dE, dI, m_dRibJigan);
			XLText->AddFormatText("$h$m+05$c=$r$me2$f3%.3f$m+03%s$n", dHcr, pBri->m_pARcBridgeDataStd->m_szTonf);

			XLText->GoPosition(NO_MOVE,COL_POS_3);
			MakeTableAlphaByRaiseRatio(XLText);
			
			XLText->GoPosition(NO_MOVE,COL_POS_2);
			XLText->AddFormatText("$h$c-$r면내 좌굴을 고려한 설계 단면력$n$h");
			XLText->GoPosition(NO_MOVE,COL_POS_3);
			MakeTableColumnLoadExc(XLText, pGirCol, n);
			XLText->GoPosition(NO_MOVE,COL_POS_2);
		}
		if(dRamda < 70)
		{
			CString strCombo = _T("");

//			XLText->GoPosition(NO_MOVE,COL_POS_1);
//			XLText->AddFormatText("$s+50$h %d)$r 단면검토$n",nIndexSmall++);

			long j=0; for(j=0; j<RCBRIDGE_COL_CHECK; j++)
			{
				CConcBasic *pConcTemp = pGirCol->pConcColumn[j];
				strCombo = _T("");
				if(pGirCol->m_nTypeDesign[j]==COMBO_ULTIMATE)
					strCombo.Format("UL%02d",pGirCol->m_nCombo[j]+1);
				if(pGirCol->m_nTypeDesign[j]==COMBO_ECCENTRIC)
					strCombo.Format("UE%02d",pGirCol->m_nCombo[j]+1);
				else if (pGirCol->m_nTypeDesign[j]==COMBO_EARTHQUAKE)
					strCombo.Format("EQ%02d",pGirCol->m_nCombo[j]+1);
				else if (pGirCol->m_nTypeDesign[j]==COMBO_EARTHQUAKE2)
					strCombo.Format("EQ%02d",pGirCol->m_nCombo[j]+1);
				
				XLText->GoPosition(NO_MOVE,COL_POS_2);
				XLText->AddFormatText("$s+40$h$c%s$r%s, %s$n",GetNumberString(nIndexSub++,NUM_CIRCLE),pConc->m_sTitle,strCombo);
				
				CColumn ColCalc(pConcTemp, FALSE, 3);
				ColCalc.CalculateAll(FALSE);
				ColCalc.MakeOutputExcel(XLText,COL_POS_3);
			}

			XLText->GoPosition(NO_MOVE,COL_POS_2);
			XLText->AddFormatText("$s+20$h$e%sP-M 상관도$n",GetNumberString(nIndexSub++,NUM_CIRCLE));

			CColumn ColCalc(pConc, FALSE, 3);
			ColCalc.CalculateAll(TRUE);

			double ScaleDim = 50;
			CDomyun *pDom = new CDomyun;
			pDom->SetCWnd(m_pStd->m_pView);
			pDom->SetScaleDim(ScaleDim);
			pDom->SetLTScale(ScaleDim*5);
			
			CDrawPM DrawPM(m_pStd->m_pARoadOptionStd);
			DrawPM.DrawPM(pDom,&ColCalc);
			
			long nSizeRow = 25;
			XLText->InsertDomyun(pDom,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_CENTER|CXLFormatText::INSERT_DOM_MOVEROW);
		}
		
	}
}

void CARcBridgeCalcArchRib::CalculateArchRib()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	m_dWidth	= toM(pBri->GetWidthArchRib(0));
	m_dRibJigan	= toM(pBri->m_pbRibCen.m_dDistTotal);
	m_dRibRaise	= toM(pBri->m_pbRibCen.m_dHeightTotal);
	
	m_dArrDelta.RemoveAll();
	m_dArrRamda.RemoveAll();
	m_dArrHcr.RemoveAll();
	
	CDPoint vAng = pBri->m_pbRibCen.GetXyDirTanByDist(-pBri->m_pbRibCen.m_dDistTotal/4);
	double dAreaL_4 = m_dWidth * toM(pBri->GetThickArchRib(pBri->m_pbRibCen.m_dDistTotal/4, TRUE));
	double dAngL_4	= fabs(vAng.GetAngleDegree());
	double dLenRib	= toM(GetLenArchRib()) + toM(pBri->GetThickArchRibForCalc(CHECK_POS_LEFT, TRUE) + pBri->GetThickArchRibForCalc(CHECK_POS_RIGHT, TRUE));

	long nSize = m_pList.GetSize();
	long n=0; for(n=0; n<nSize; n++)
	{
		CGirderAndColumn *pGirCol = m_pList.GetAt(n);
		CConcBasic *pConc = pGirCol->pConcColumn[0];
		
		double dHeight = toM(pConc->m_Ho);

		// 평균단면2차 모멘트 옵션일 경우(8966)
		if(pBri->m_nArchRibSectionMomentPos == 0)
			dHeight = toM(pBri->GetThickArchRib(pBri->m_pbRibCen.m_dDistTotal/4, TRUE));

		double dI = (m_dWidth*pow(dHeight,3))/12;
		double dRaiseRatio = m_dRibRaise/m_dRibJigan;
		double dAng		= m_dArrAngTan.GetAt(n);
		double dAlpha	= GetValueAlphaByRaiseRatio(dRaiseRatio);
		double dDelta	= GetValueDeltaByRaiseRatio(dRaiseRatio);
		double dLenExc	= dDelta * dLenRib;
		 double dE = GetValueUnitChange(pBri->m_pARcBridgeDataStd->m_Ec * 10, UNIT_CGS_TONF_M2, pBri->m_pARcBridgeDataStd->m_szTonf_M2);	// kgf/cm2 ==> ton/m2;
		double dRamda	= dLenExc * sqrt((dAreaL_4)*cos(ToRadian(dAngL_4)) / dI);
//		double dHcr		= (4*pow(PI,2)*(1-8*pow(dRaiseRatio, 2))) * (dE * dI / pow(m_dRibJigan, 2));
		double dHcr		= dAlpha * (dE * dI / pow(m_dRibJigan, 2));

		m_dArrDelta.Add(dDelta);
		m_dArrRamda.Add(dRamda);
		m_dArrHcr.Add(dHcr);
		
		CConcBasic *pConc0 = pGirCol->pConcColumn[0];
		long i=0; for(i=0; i<RCBRIDGE_COL_CHECK; i++)
		{
// 			double dMomentExc = 0.;
// 			double Jeta	=0.;
			CConcBasic *pConc = pGirCol->pConcColumn[i];
			if(dRamda > 20)
			{
				double dAxial = GetValueUnitChange(pGirCol->m_dColAxial[i], UNIT_CGS_TONF, pData->m_szTonf);
				double Jeta = fabs((dAxial * cos(ToRadian(dAng))) / dHcr);
				double dMomentExc = pGirCol->m_dColMoment[i] / (1 - Jeta);
				pConc->m_Moment = dMomentExc;
				pConc->m_Mux    = dMomentExc;
			}
			pConc0->m_xyArrPM.Add(CDPoint(pConc->m_Moment, pConc->m_Axial));
		}
	}
}

void CARcBridgeCalcArchRib::MakeDesignList()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	long nJijumConnect = pBri->GetJijumConnectRibAndSlab();
	long nIdxCrown = (nJijumConnect * 2) + 1;
	long nCnt = 2 + 1 + pBri->GetCountInWall()*2 + 2;  // 스프링깅2 + 크라운 1 + 내측벽체 갯수*2(수직재 있는부분 + 중간) + 리브,슬래브 접합부 좌우
	long nPos = 0;
	long nSpan = 0;
	m_pList.RemoveAll();
	m_dArrAngTan.RemoveAll();

	long k=0; for(k=0; k<nCnt; k++)
	{
		long nIndex = k;
		BOOL bCheckCompare = FALSE;
		BOOL bCrown = FALSE;

		if(nIndex==0 || nIndex==nCnt-1)							// 시종점 SPRINGING
		{
			nSpan = (nIndex==0)? 0 : (nIndex-1)/2-1;
			nPos = (nIndex==0)? CHECK_POS_LEFT : CHECK_POS_RIGHT;
		}
		else if(nIndex==nIdxCrown-1 || nIndex==nIdxCrown+1)		// 리브,슬래브 접합부
		{
			nSpan = (nIndex==nIdxCrown-1)? nJijumConnect-1 : nJijumConnect;
			nPos = CHECK_POS_RIGHT;
			bCheckCompare = TRUE;
		}
		else if(nIndex == nIdxCrown)			// CROWN
		{
			bCrown = TRUE;
			nSpan = nJijumConnect-1;
			nPos = CHECK_POS_RIGHT;
		}
		else if(nIndex%2 == 1)					// 중간부
		{
			nSpan = (nIndex < nIdxCrown)? (nIndex-1)/2 : (nIndex-1)/2-1;
			nPos = CHECK_POS_CENTER;
		}
		else if(nIndex%2 == 0)					// 연직재 교차점
		{
			nSpan = (nIndex < nIdxCrown)? (nIndex-1)/2 : (nIndex-1)/2-1;
			nPos = CHECK_POS_RIGHT;
			bCheckCompare = TRUE;
		}
		
		CGirderAndColumn *pGirCol = new CGirderAndColumn;

		pGirCol->m_dCtcColumn = m_dWidth;
		long i=0; for(i=0; i<RCBRIDGE_COL_CHECK; i++)
		{
			GetMomentAndAxialOnArchRib(nSpan, nPos,i,pGirCol->m_dAxialBot[i],pGirCol->m_dMomentBot[i],pGirCol->m_dShearBot[i],pGirCol->m_nTypeDesign[i],pGirCol->m_nCombo[i], bCrown);
			if(bCheckCompare)
			{
				long nSpan2 = nSpan+1;
				long nTypeDesign, nCombo;
				double dAxialBot, dMomentBot, dShearBot;

				GetMomentAndAxialOnArchRib(nSpan2, CHECK_POS_LEFT,i,dAxialBot,dMomentBot,dShearBot,nTypeDesign,nCombo, bCrown);
				pGirCol->m_dAxialBot[i]	 = (fabs(pGirCol->m_dAxialBot[i]) > fabs(dAxialBot))? pGirCol->m_dAxialBot[i] : dAxialBot;
				pGirCol->m_dMomentBot[i] = (fabs(pGirCol->m_dMomentBot[i]) > fabs(dMomentBot))? pGirCol->m_dMomentBot[i] : dMomentBot;
				pGirCol->m_dShearBot[i] = (fabs(pGirCol->m_dShearBot[i]) > fabs(dShearBot))? pGirCol->m_dShearBot[i] : dShearBot;
			}
		}
		
		double dAngleTan = GetAngleRibTan(nSpan, nPos);
		m_dArrAngTan.Add(dAngleTan);
		pGirCol->CalulateForce();
		m_pList.Add(pGirCol);
	}
}

void CARcBridgeCalcArchRib::SetConcDataByCol()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	CString szTitle = _T("");
	long nPos = 0;
	long nSpan = 0;
	long nJijumConnect = pBri->GetJijumConnectRibAndSlab();
	long nIdxCrown = (nJijumConnect * 2) + 1;

	long nSize = m_pList.GetSize();
	long i=0; for(i=0; i<nSize; i++)
	{
		CGirderAndColumn    *pGirCol = m_pList.GetAt(i);
		BOOL bCrown = FALSE;

		long nIndex = i;
		if(nIndex==0 || nIndex==nSize-1)							// 시종점 SPRINGING
		{
			nSpan = (nIndex==0)? 0 : (nIndex-1)/2-1;
			szTitle = (nIndex == 0)? _T("시점 SPRINGING") : _T("종점 SPRINGING");
			nPos = (nIndex==0)? CHECK_POS_LEFT : CHECK_POS_RIGHT;
		}
		else if(nIndex==nIdxCrown-1 || nIndex==nIdxCrown+1)		// 리브,슬래브 접합부
		{
			nSpan = (nIndex==nIdxCrown-1)? nJijumConnect-1 : nJijumConnect;
			szTitle.Format("슬래브접합부%s",(nIndex==nIdxCrown-1)? "좌측":"우측");
			nPos = (nIndex==nIdxCrown-1) ? CHECK_POS_RIGHT : CHECK_POS_LEFT;
		}
		else if(nIndex == nIdxCrown)			// CROWN
		{
			bCrown = TRUE;
			nSpan = nJijumConnect - 1;
			szTitle = _T("CROWN부");
			nPos = CHECK_POS_RIGHT;
		}
		else if(nIndex%2 == 1)					// 중간부
		{
			nSpan = (nIndex < nIdxCrown)? (nIndex-1)/2 : (nIndex-1)/2-1;
			szTitle.Format("중간부%d", nIndex/2+1);
			nPos = CHECK_POS_CENTER;
		}
		else if(nIndex%2 == 0)					// 연직재 교차점
		{
			nSpan = (nIndex < nIdxCrown)? (nIndex-1)/2 : (nIndex-1)/2-1;
			szTitle.Format("연직재 교차점%d", nIndex/2);
			nPos = CHECK_POS_RIGHT;
		}

		for(long nCheck=0; nCheck<RCBRIDGE_COL_CHECK; nCheck++)
		{
			CConcBasic *pConc = pGirCol->pConcColumn[nCheck];

			pConc->m_sTitle = szTitle;
			pConc->m_nTypeSection = SECTION_WALL;
//			pConc->m_Bo = (nCheck==3)? frM(UNIT_METER) : frM(m_dWidth);
			pConc->m_Bo = frM(m_dWidth);
			pConc->m_Ho = GetThickArchRib(nSpan, nPos, bCrown);
			pConc->m_Ro = 0;

			pConc->m_Mux    = pGirCol->m_dColMoment[nCheck];
			pConc->m_Moment = pGirCol->m_dColMoment[nCheck];
			pConc->m_Axial  = pGirCol->m_dColAxial[nCheck];
			pConc->m_Shear	= pGirCol->m_dColShear[nCheck];

			SetConcData_BaseInfo(pConc, TRUE);

			// 철근집계
			long nCntMainCycle	= 2;
			BOOL bUpper = TRUE;
			double dDia = 0;
			double dDc =0;
			double dCtc = pBri->m_dCTC_MainRebar;
			double dB = pConc->m_Bo/dCtc; 
			
			CDPoint xyInfo	= CDPoint(0, 0);
			int i=0; for(i=0; i<nCntMainCycle; i++)
			{
				int j=0; for(j=0; j<MAX_REBAR_DAN; j++)
				{
					// 아치리브는 상하면이 대칭으로 배근되므로 상면을 기준으로 인장압축면을 같이 넣어준다.
					// 따로 입력 되는데 왜 같이? 따로 넣자. 12491
					
					// 상면 인장.
					dDc = pBri->m_dCoverRib[0][j];
					xyInfo	= pBri->GetInfoMainRebar_ArchRib(bUpper, nSpan, i, j, nPos-1, bCrown); // 0,1,2 => -1,0,1
					dDia = xyInfo.x;
					if(i == 0)
					{
						pConc->m_RbT1C_DIA[j] = dDia; // 철근의 직경
						if(dDia>0)
						{
							pConc->m_RbT1C_dc[j] = dDc;  // 철근의 중심거리 피복 + 100*단수(-1)
							pConc->m_RbT1C_B [j] = dB;  // 철근의 개수 (1000/CTC)
						}
					}
					else
					{
						pConc->m_RbT2C_DIA[j] = dDia; // 철근의 직경
						if(dDia>0)
						{
							pConc->m_RbT2C_dc[j] = dDc;  // 철근의 중심거리 피복 + 100*단수(-1)
							pConc->m_RbT2C_B [j] = dB;  // 철근의 개수 (1000/CTC)
						}
					}
					
					// 하면 압축
					xyInfo	= pBri->GetInfoMainRebar_ArchRib(!bUpper, nSpan, i, j, nPos-1, bCrown); // 0,1,2 => -1,0,1
					dDia = xyInfo.x;
					if(i == 0)
					{
						pConc->m_RbC1C_DIA[j] = dDia; // 철근의 직경
						if(dDia>0)
						{	
							pConc->m_RbC1C_dc[j] = dDc;  // 철근의 중심거리 피복 + 100*단수(-1)
							pConc->m_RbC1C_B [j] = dB;  // 철근의 개수 (1000/CTC)
						}
					}
					else
					{
						pConc->m_RbC2C_DIA[j] = dDia; // 철근의 직경
						if(dDia>0)
						{		
							pConc->m_RbC2C_dc[j] = dDc;  // 철근의 중심거리 피복 + 100*단수(-1)
							pConc->m_RbC2C_B [j] = dB;  // 철근의 개수 (1000/CTC)
						}
					}
				}
			}
			
			for(i=0; i<MAX_REBAR_DAN; i++)
			{
				xyInfo = pBri->GetInfoSupportRebar_ArchRib(bUpper, nSpan, i, nPos-1, bCrown);
				pConc->m_CSupportBarDia[i] = xyInfo.x;
				pConc->m_CSupportBarCTC[i] = xyInfo.y;
				pConc->m_TSupportBarDia[i] = xyInfo.x;
				pConc->m_TSupportBarCTC[i] = xyInfo.y;
			}
			xyInfo = pBri->GetInfoShearRebar_ArchRib(nSpan, nPos-1, bCrown);
			pConc->m_SvDia = xyInfo.x;
			pConc->m_SvCtc = xyInfo.y;
			double dSvEa  = frM(UNIT_METER)/(pBri->m_nCountWrapMainRebarForArchRib * pBri->GetCtcRebarMain()/2);
			dSvEa *= m_dWidth;
			pConc->m_SvEa = (pBri->m_RebarShearDetail_ArchRib.m_nType == TYPE_SHEAR_DIGUT || pBri->m_RebarShearDetail_ArchRib.m_nType == TYPE_SHEAR_SINGLE_HOOK)? dSvEa/2 : dSvEa;
		}
	}
}

void CARcBridgeCalcArchRib::SetConcDataBySlab()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	AhTPADelete(&pBri->m_pArrConcArchRib,   (CConcBasic*)0);

	CString szTitle = _T("");
	long nPosCheck = 0;
	long nSpan = 0;
	long nJijumConnect = pBri->GetJijumConnectRibAndSlab();
	long nIdxCrown = (nJijumConnect * 2) + 1;

	long nSize = m_pList.GetSize();
	long i=0; for(i=0; i<nSize; i++)
	{
		BOOL bCheckCompare = FALSE;
		BOOL bCrown	= FALSE;

		long nIndex = i;
		if(nIndex==0 || nIndex==nSize-1)							// 시종점 SPRINGING
		{
			nSpan = (nIndex==0)? 0 : (nIndex-1)/2-1;
			szTitle = (nIndex == 0)? _T("시점 SPRINGING") : _T("종점 SPRINGING");
			nPosCheck = (nIndex==0)? CHECK_POS_LEFT : CHECK_POS_RIGHT;
		}
		else if(nIndex==nIdxCrown-1 || nIndex==nIdxCrown+1)		// 리브,슬래브 접합부
		{
			nSpan = (nIndex==nIdxCrown-1)? nJijumConnect-1 : nJijumConnect;
			szTitle.Format("슬래브접합부%s",(nIndex==nIdxCrown-1)? "좌측":"우측");
			nPosCheck = (nIndex==nIdxCrown-1)? CHECK_POS_RIGHT : CHECK_POS_LEFT;
			bCheckCompare = TRUE;
		}
		else if(nIndex == nIdxCrown)			// CROWN
		{
			bCrown	= TRUE;
			nSpan = nJijumConnect-1;
			szTitle = _T("CROWN부");
			nPosCheck = CHECK_POS_RIGHT;
		}
		else if(nIndex%2 == 1)					// 중간부
		{
			nSpan = (nIndex < nIdxCrown)? (nIndex-1)/2 : (nIndex-1)/2-1;
			szTitle.Format("중간부%d", nIndex/2+1);
			nPosCheck = CHECK_POS_CENTER;
		}
		else if(nIndex%2 == 0)					// 연직재 교차점
		{
			nSpan = (nIndex < nIdxCrown)? (nIndex-1)/2 : (nIndex-1)/2-1;
			szTitle.Format("연직재 교차점%d", nIndex/2);
			nPosCheck = CHECK_POS_RIGHT;
			bCheckCompare = TRUE;
		}
		
		CConcBasic *pConc= new CConcBasic;
		
		pConc->m_sTitle = szTitle;
		pConc->m_nTypeSection = SECTION_SINGLE;
		pConc->m_Bo = frM(UNIT_METER);
		pConc->m_Ho = GetThickArchRib(nSpan, nPosCheck, bCrown);
		pConc->m_Ro = 0;
	////////////////////////////////////////////////////
		BOOL bCalc3DUlt = pBri->IsCalc3D(COMBO_ULTIMATE);
		BOOL bCalc3DUse = pBri->IsCalc3D(COMBO_SERVICE);
		double Moment=0,MomentUse=0, MomentEq=0, Shear=0, ShearEq = 0, MomentEq_2=0, ShearEq_2 = 0;
		
		// 값이 CGS단위가 아닌 것은 변환을 하여 저장하여 줌.....
		Moment	  = fabs(GetOutValueArchRibBySlab(nSpan, nPosCheck, COMBO_ULTIMATE, ELE_MOMENT2, bCrown));
		MomentEq  = fabs(GetOutValueArchRibBySlab(nSpan, nPosCheck, COMBO_EARTHQUAKE, ELE_MOMENT2, bCrown));
		MomentUse = fabs(GetOutValueArchRibBySlab(nSpan, nPosCheck, COMBO_SERVICE, ELE_MOMENT2, bCrown));
		Shear	  = fabs(GetOutValueArchRibBySlab(nSpan, nPosCheck, COMBO_ULTIMATE, ELE_SHEAR_2, bCrown));
		ShearEq	  = fabs(GetOutValueArchRibBySlab(nSpan, nPosCheck, COMBO_EARTHQUAKE, ELE_SHEAR_2, bCrown));
		if(pBri->m_nSelectModelingCountEQ==2)
		{
			MomentEq_2	= fabs(GetOutValueArchRibBySlab(nSpan, nPosCheck, COMBO_EARTHQUAKE2, ELE_MOMENT2, bCrown));
			ShearEq_2	= fabs(GetOutValueArchRibBySlab(nSpan, nPosCheck, COMBO_EARTHQUAKE2, ELE_SHEAR_2, bCrown));
			MomentEq	= (fabs(MomentEq) > fabs(MomentEq_2))? MomentEq : MomentEq_2;
			ShearEq		= (fabs(ShearEq) > fabs(ShearEq_2))? ShearEq : ShearEq_2;
		}

		if(bCheckCompare)
		{
			long nSpan2 = nSpan+1;
			long nPosCheck2 = CHECK_POS_LEFT;
			
			double Moment2	  = fabs(GetOutValueArchRibBySlab(nSpan2, nPosCheck2, COMBO_ULTIMATE, ELE_MOMENT2, bCrown));
			double MomentEq2  = fabs(GetOutValueArchRibBySlab(nSpan2, nPosCheck2, COMBO_EARTHQUAKE, ELE_MOMENT2, bCrown));
			double MomentUse2 = fabs(GetOutValueArchRibBySlab(nSpan2, nPosCheck2, COMBO_SERVICE, ELE_MOMENT2, bCrown));
			double Shear2	  = fabs(GetOutValueArchRibBySlab(nSpan2, nPosCheck2, COMBO_ULTIMATE, ELE_SHEAR_2, bCrown));
			double ShearEq2	  = fabs(GetOutValueArchRibBySlab(nSpan2, nPosCheck2, COMBO_EARTHQUAKE, ELE_SHEAR_2, bCrown));
			if(pBri->m_nSelectModelingCountEQ==2)
			{
				MomentEq_2	= fabs(GetOutValueArchRibBySlab(nSpan2, nPosCheck2, COMBO_EARTHQUAKE2, ELE_MOMENT2, bCrown));
				ShearEq_2	= fabs(GetOutValueArchRibBySlab(nSpan2, nPosCheck2, COMBO_EARTHQUAKE2, ELE_SHEAR_2, bCrown));
				MomentEq2	= (fabs(MomentEq2) > fabs(MomentEq_2))? MomentEq2 : MomentEq_2;
				ShearEq2	= (fabs(ShearEq2) > fabs(ShearEq_2))? ShearEq2 : ShearEq_2;
			}

			Moment	  = max(Moment, Moment2);
			MomentEq  = max(MomentEq, MomentEq2);
			MomentUse = max(MomentUse, MomentUse2);
			Shear	  = max(Shear, Shear2);
			ShearEq	  = max(ShearEq, ShearEq2);
		}
		Moment	  = max(Moment, MomentEq);
		Shear	  = max(Shear, ShearEq);

		if(bCalc3DUlt)
		{
			if(i != 0 && i%2 == 0)
			{
				double Moment3D = max(fabs(pBri->GetOutValueArchRibShell(nSpan,CHECK_POS_RIGHT,COMBO_ULTIMATE,ELE_MOMENT2)), fabs(pBri->GetOutValueArchRibShell(nSpan+1,CHECK_POS_LEFT,COMBO_ULTIMATE,ELE_MOMENT2)));
				double Shear3D = max(fabs(pBri->GetOutValueArchRibShell(nSpan,CHECK_POS_RIGHT,COMBO_ULTIMATE,ELE_SHEAR_2)), fabs(pBri->GetOutValueArchRibShell(nSpan+1,CHECK_POS_LEFT,COMBO_ULTIMATE,ELE_SHEAR_2)));
				Moment	= max(Moment, Moment3D);
				Shear	= max(Shear, Shear3D);
			}
			else
			{
				Moment	= max(Moment, fabs(pBri->GetOutValueArchRibShell(nSpan,nPosCheck,COMBO_ULTIMATE,ELE_MOMENT2)));
				Shear	= max(Shear, fabs(pBri->GetOutValueArchRibShell(nSpan,nPosCheck,COMBO_ULTIMATE,ELE_SHEAR_2)));
			}
		}
		if(bCalc3DUse)
		{
			if(i != 0 && i%2 == 0)
			{
				double MomentUse3D = max(fabs(pBri->GetOutValueArchRibShell(nSpan,CHECK_POS_RIGHT,COMBO_SERVICE ,ELE_MOMENT2)), fabs(pBri->GetOutValueArchRibShell(nSpan+1,CHECK_POS_LEFT,COMBO_SERVICE ,ELE_MOMENT2)));
				MomentUse = max(MomentUse, MomentUse3D);
			}
			else MomentUse = max(MomentUse, fabs(pBri->GetOutValueArchRibShell(nSpan,nPosCheck,COMBO_SERVICE ,ELE_MOMENT2)));
		}
		
		pConc->m_Moment		= GetValueUnitChange(Moment,pData->m_szTonfM,UNIT_CGS_TONFM);
		pConc->m_Shear		= GetValueUnitChange(Shear,pData->m_szTonf,UNIT_CGS_TONF);
		pConc->m_MomentUse	= GetValueUnitChange(MomentUse,pData->m_szTonfM,UNIT_CGS_TONFM);
		pConc->m_MomentCrack	= pConc->m_MomentUse;
	////////////////////////////////////////////////////
		SetConcData_BaseInfo(pConc, FALSE);
		
		// 철근집계
		long nCntMainCycle	= 2;
		BOOL bUpper = TRUE;
		double dDia = 0;
		double dDc =0;
		double dCtc = pBri->m_dCTC_MainRebar;
		double dB = pConc->m_Bo/dCtc; 
		
		CDPoint xyInfo	= CDPoint(0, 0);
		int i=0; for(i=0; i<nCntMainCycle; i++)
		{
			int j=0; for(j=0; j<MAX_REBAR_DAN; j++)
			{
				// 아치리브는 상하면이 대칭으로 배근되므로 상면을 기준으로 인장압축면을 같이 넣어준다.
				// 따로 입력 되는데 왜 같이? 따로 넣자. 12491

				// 상면 인장.
				dDc = pBri->m_dCoverRib[0][j];
				xyInfo	= pBri->GetInfoMainRebar_ArchRib(bUpper, nSpan, i, j, nPosCheck-1, bCrown); // 0,1,2 => -1,0,1
				dDia = xyInfo.x;
				if(i == 0)
				{
					pConc->m_RbT1C_DIA[j] = dDia; // 철근의 직경
					if(dDia>0)
					{
						pConc->m_RbT1C_dc[j] = dDc;  // 철근의 중심거리 피복 + 100*단수(-1)
						pConc->m_RbT1C_B [j] = dB;  // 철근의 개수 (1000/CTC)
					}
				}
				else
				{
					pConc->m_RbT2C_DIA[j] = dDia; // 철근의 직경
					if(dDia>0)
					{
						pConc->m_RbT2C_dc[j] = dDc;  // 철근의 중심거리 피복 + 100*단수(-1)
						pConc->m_RbT2C_B [j] = dB;  // 철근의 개수 (1000/CTC)
					}
				}

				// 하면 압축
				xyInfo	= pBri->GetInfoMainRebar_ArchRib(!bUpper, nSpan, i, j, nPosCheck-1, bCrown); // 0,1,2 => -1,0,1
				dDia = xyInfo.x;
				if(i == 0)
				{
					pConc->m_RbC1C_DIA[j] = dDia; // 철근의 직경
					if(dDia>0)
					{	
						pConc->m_RbC1C_dc[j] = dDc;  // 철근의 중심거리 피복 + 100*단수(-1)
						pConc->m_RbC1C_B [j] = dB;  // 철근의 개수 (1000/CTC)
					}
				}
				else
				{
					pConc->m_RbC2C_DIA[j] = dDia; // 철근의 직경
					if(dDia>0)
					{		
						pConc->m_RbC2C_dc[j] = dDc;  // 철근의 중심거리 피복 + 100*단수(-1)
						pConc->m_RbC2C_B [j] = dB;  // 철근의 개수 (1000/CTC)
					}
				}
			}
		}
		
		for(i=0; i<MAX_REBAR_DAN; i++)
		{
			// 따로 입력 되는데 왜 같이? 따로 넣자. 12758
			xyInfo = pBri->GetInfoSupportRebar_ArchRib(bUpper, nSpan, i, nPosCheck-1, bCrown);
			pConc->m_TSupportBarDia[i] = xyInfo.x;
			pConc->m_TSupportBarCTC[i] = xyInfo.y;

			xyInfo = pBri->GetInfoSupportRebar_ArchRib(!bUpper, nSpan, i, nPosCheck-1, bCrown);
			pConc->m_CSupportBarDia[i] = xyInfo.x;
			pConc->m_CSupportBarCTC[i] = xyInfo.y;
		}
		xyInfo = pBri->GetInfoShearRebar_ArchRib(nSpan, nPosCheck-1, bCrown);
		pConc->m_SvDia = xyInfo.x;
		pConc->m_SvCtc = xyInfo.y;
		double dSvEa  = frM(UNIT_METER)/(pBri->m_nCountWrapMainRebarUpperSlab * pBri->GetCtcRebarMain()/2);
		pConc->m_SvEa = (pBri->m_RebarShearDetail_ArchRib.m_nType == TYPE_SHEAR_DIGUT || pBri->m_RebarShearDetail_ArchRib.m_nType == TYPE_SHEAR_SINGLE_HOOK)? dSvEa/2 : dSvEa;

		pBri->m_pArrConcArchRib.Add(pConc);
	}
		
}

// 단면, 축력, 수직철근량 등이 입력 된 후에 호출해야함...
void CARcBridgeCalcArchRib::SetConcData_BaseInfo(CConcBasic *pConc, BOOL bSectWall)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	pConc->m_nTypeUnit		= pData->m_nTypeUnit; //0-CGS,1-SI
	pConc->m_nTypeKciCode	= pData->m_nTypeKciCode;
	pConc->m_nTypeSection	= bSectWall? SECTION_WALL : SECTION_SINGLE; // 단면의 타입,SECTION_SINGLE
	pConc->m_Fck	= pBri->GetValueFck(ePartInWall, FALSE);
	pConc->m_Fy		= pBri->GetValueFy(ePartInWall, FALSE, FALSE);
	pConc->m_Ec		= pBri->GetValueEc(ePartInWall, FALSE);
	pConc->m_Es		= pData->m_Es;
	pConc->m_Pi_C	= pData->m_Pi_C;
	pConc->m_Pi_F	= pData->m_Pi_F;
	pConc->m_Pi_V	= pData->m_Pi_V;
	pConc->m_nEnvTypeCrack = pBri->m_nTypeCrackDesignSlab;
	if(!bSectWall) pConc->m_bMinAvCheck   = pBri->m_bApplyMinAvSlab;

	if(bSectWall)
	{
		BOOL bIsWall = FALSE;
		double dAg = pConc->m_Ho * pConc->m_Bo;
		double dAs = pConc->GetRebarAreaCompression() + pConc->GetRebarAreaTension();
		if((pConc->m_Axial < 0.4*dAg* pBri->GetValueFck(ePartInWall, FALSE)) && (dAs < 0.01 * dAg)) bIsWall = TRUE;
		
		pConc->m_nTypeSection = (bIsWall)? SECTION_WALL : SECTION_COLUMN;
	}
}

// nTypeCheck 0-축력최대,1-모멘트최대,2-최대편심,3-지진시
void CARcBridgeCalcArchRib::GetMomentAndAxialOnArchRib(long nSpan, long nPos, long nTypeCheck, double &dAxialReturn, double &dMomentReturn, double &dShearReturn, long &nTypeDesignReturn, long &nComboReturn, BOOL bCrown)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	CElement *pEle = NULL;
	CResultForceElement *pResultMax = NULL;
	CResultForceElement *pResultMin = NULL;

//	long nSizeUlt = pBri->m_MatComboUlt.GetRow();
	long nSizeUlt = pBri->GetSizeCombo(COMBO_ULTIMATE, TRUE);

	CString szCombo = _T("");
	long nLoadCase  = 0;
	long nPoint = 0;
	double dAxialMax,dAxialMin,dMomentMax,dMomentMin,eMax,eMin;
	double dAxialCur,dMomentCur,dShearCur,eCur;
	dAxialReturn  = 0; 
	dMomentReturn = 0;
	nTypeDesignReturn = -1;
	nComboReturn  = -1;

	long nIdxEleStt = pBri->GetNumberElementArchRibSpanSttCheck(nSpan, nTypeCheck, bCrown);
	long nIdxEleEnd = pBri->GetNumberElementArchRibSpanEndCheck(nSpan, nTypeCheck, bCrown);
	if(nIdxEleStt==-1 || nIdxEleEnd==-1) return ;
	
	long nIdxEle = 0;
	if(nPos==CHECK_POS_LEFT || nPos==CHECK_POS_RIGHT) nIdxEle = (nPos==CHECK_POS_LEFT) ? nIdxEleStt : nIdxEleEnd;
	else nIdxEle = (nIdxEleStt + nIdxEleEnd)/2; 
	
	// 계수하중 조합
	long i=0; for(i=0; i<nSizeUlt; i++)
	{
		if(nTypeCheck==3) break;
		szCombo.Format("UL%02d MAX",i+1);
		nLoadCase = pBri->m_FemNormal.GetLoadCaseIndex(szCombo);	
		if(nLoadCase==-1) continue;

		pEle = pBri->m_FemNormal.GetElement((USHORT)nIdxEle);

		pResultMax  = pEle->GetResultForce(nLoadCase);
		pResultMin  = pEle->GetResultForce(nLoadCase+1);

		nPoint = (nPos==CHECK_POS_LEFT)? 0 : pResultMax->m_ResultForceElement.GetSize()-1;
		dAxialMax  = fabs(pResultMax->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_AXFORCE]);
		dAxialMin  = fabs(pResultMin->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_AXFORCE]);
		dMomentMax = fabs(pResultMax->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_MOMENT2]);
		dMomentMin = fabs(pResultMin->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_MOMENT2]);

		BOOL bApplyCase = FALSE;
		CResultForceElement *pResult = NULL;
		if(nTypeCheck==0)
		{
			pResult   = (dAxialMax>=dAxialMin) ? pResultMax : pResultMin;
			dAxialCur  = fabs(pResult->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_AXFORCE]);
			dMomentCur = fabs(pResult->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_MOMENT2]);
			dShearCur = fabs(pResult->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_SHEAR_2]);
			if(dAxialCur>dAxialReturn)
			{
				bApplyCase = TRUE;
			}
		}
		else if(nTypeCheck==1)
		{
			pResult    = (dMomentMax>=dMomentMin) ? pResultMax : pResultMin;
			dAxialCur  = fabs(pResult->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_AXFORCE]);
			dMomentCur = fabs(pResult->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_MOMENT2]);
			dShearCur = fabs(pResult->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_SHEAR_2]);
			if(dMomentCur>dMomentReturn)
			{
				bApplyCase = TRUE;
			}
		}
		else if (nTypeCheck==2)
		{
			eMax    = dMomentMax/dAxialMax;
			eMin    = dMomentMin/dAxialMin;
			pResult = (eMax>=eMin) ? pResultMax : pResultMin;
			dAxialCur  = fabs(pResult->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_AXFORCE]);
			dMomentCur = fabs(pResult->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_MOMENT2]);
			dShearCur = fabs(pResult->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_SHEAR_2]);
			eCur = dMomentCur/dAxialCur;
			if(eCur>(dMomentReturn/dAxialReturn))
			{
				bApplyCase = TRUE;
			}
		}
		if(bApplyCase || nComboReturn==-1)
		{
			dAxialReturn  = dAxialCur;
			dMomentReturn = dMomentCur;
			dShearReturn = (nPos != CHECK_POS_CENTER)? dShearCur : 0;
			nTypeDesignReturn = COMBO_ULTIMATE;
			nComboReturn = i;
		}
	}
	// 최대편심 조합
//	long nSizeEcc = pBri->m_MatComboEcc.GetRow();
	long nSizeEcc = pBri->GetSizeCombo(COMBO_ECCENTRIC, TRUE);
	for(i=0; i<nSizeEcc; i++)
	{
		if(nTypeCheck!=2) break;
		szCombo.Format("UE%02d MAX",i+1);
		nLoadCase = pBri->m_FemNormal.GetLoadCaseIndex(szCombo);	
		if(nLoadCase==-1) continue;
	
		pResultMax  = pEle->GetResultForce(nLoadCase);
		pResultMin  = pEle->GetResultForce(nLoadCase+1);

		nPoint = pResultMax->m_ResultForceElement.GetSize()-1;
		dAxialMax  = fabs(pResultMax->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_AXFORCE]);
		dAxialMin  = fabs(pResultMin->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_AXFORCE]);
		dMomentMax = fabs(pResultMax->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_MOMENT2]);
		dMomentMin = fabs(pResultMin->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_MOMENT2]);

		CResultForceElement *pResult = NULL;
		eMax    = dMomentMax/dAxialMax;
		eMin    = dMomentMin/dAxialMin;
		pResult = (eMax>=eMin) ? pResultMax : pResultMin;
		dAxialCur  = fabs(pResult->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_AXFORCE]);
		dMomentCur = fabs(pResult->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_MOMENT2]);
		dShearCur = fabs(pResult->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_SHEAR_2]);
		eCur = dMomentCur/dAxialCur;
		if(eCur>(dMomentReturn/dAxialReturn))
		{
			dAxialReturn  = dAxialCur;
			dMomentReturn = dMomentCur;
			dShearReturn = (nPos != CHECK_POS_CENTER)? dShearCur : 0;
			nTypeDesignReturn = COMBO_ECCENTRIC;
			nComboReturn = i;
		}
	}

	// 지진시 조합 -> 편심이 가장 크게 발생하는 조합으로 검토
	long nSizeEQ = pBri->m_MatComboEQ.GetRow();
	for(long nEQ=0; nEQ<2; nEQ++)
	{
		if(nTypeCheck!=3) break;
		if(nEQ==1 && pBri->m_nSelectModelingCountEQ!=2) break;
		for(i=0; i<nSizeEQ; i++)
		{
			if(nTypeCheck!=3) break;
			szCombo.Format("EQ%02d MAX",i+1);
			nLoadCase = pBri->m_FemEQ[nEQ+1].GetLoadCaseIndex(szCombo);	
			if(nLoadCase==-1) continue;

			pEle	= pBri->m_FemEQ[nEQ+1].GetElement((USHORT)nIdxEle);

			pResultMax  = pEle->GetResultForce(nLoadCase);
			pResultMin  = pEle->GetResultForce(nLoadCase+1);

			nPoint = pResultMax->m_ResultForceElement.GetSize()-1;
			dAxialMax  = fabs(pResultMax->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_AXFORCE]);
			dAxialMin  = fabs(pResultMin->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_AXFORCE]);
			dMomentMax = fabs(pResultMax->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_MOMENT2]);
			dMomentMin = fabs(pResultMin->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_MOMENT2]);

			CResultForceElement *pResult = NULL;
			eMax    = dMomentMax/dAxialMax;
			eMin    = dMomentMin/dAxialMin;
			pResult = (eMax>=eMin) ? pResultMax : pResultMin;
			dAxialCur  = fabs(pResult->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_AXFORCE]);
			dMomentCur = fabs(pResult->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_MOMENT2]);
			dShearCur = fabs(pResult->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_SHEAR_2]);
			eCur = dMomentCur/dAxialCur;

			if(eCur>(dMomentReturn/dAxialReturn) || (Compare(eCur, 0.0, ">") && Compare(dAxialReturn, 0.0, "=")))
			{
				dAxialReturn  = dAxialCur;
				dMomentReturn = dMomentCur;
				dShearReturn = (nPos != CHECK_POS_CENTER)? dShearCur : 0;
				nTypeDesignReturn = (i==0) ? COMBO_EARTHQUAKE : COMBO_EARTHQUAKE2;
				nComboReturn = i;
			}
		}
	}

	double dArchRibWidth  = m_dWidth;
	double dDivide = (nTypeCheck==3) ? dArchRibWidth : 1.00;
	// 지진시는 전폭으로 모델링 되어있기때문에 폭으로 나눠 단위M당 부재력을 구한다.

	dAxialReturn  = GetValueUnitChange(Round(dAxialReturn,3) ,pBri->m_pARcBridgeDataStd->m_szTonf,UNIT_CGS_TONF)/dDivide;
	dMomentReturn = GetValueUnitChange(Round(dMomentReturn,3),pBri->m_pARcBridgeDataStd->m_szTonf,UNIT_CGS_TONF)/dDivide;
	dShearReturn = GetValueUnitChange(Round(dShearReturn,3),pBri->m_pARcBridgeDataStd->m_szTonf,UNIT_CGS_TONF)/dDivide;
}

double CARcBridgeCalcArchRib::GetThickArchRib(long nSpan, long nPos, BOOL bCrown)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	double dThick = 0;
	double dSta = pBri->GetStationRibBySpan(nSpan, nPos, bCrown);

	dThick = m_pStd->GetThickArchRib(dSta);

	return dThick;
}

void CARcBridgeCalcArchRib::MakeTableColumnLoad(CXLFormatText *XLText, CGirderAndColumn *pGirCol)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	long nCntRow  = RCBRIDGE_COL_CHECK + 1;
	long nCntCol  = 7;
	
	XLText->AddTable(nCntRow-1, 24-1, 1, 2);
	//////////////////////////////////////////////////////////////////////////
	// Title
	int nWidthTitle[] = {  4, 3, 3, 3, 4, 4, 3};
	int nWidthColumn[] = { 4, 3, 3, 3, 4, 4, 3};
	
	char* Title[]  = { "구 분", "축력/m", "모멘트/m", "C.T.C", "축력", "모멘트", "비 고"};
	char* szCheck[] = { "축력최대","모멘트최대", "편심최대","지진시"};
	
	int i = 0; for(i = 0; i<nCntCol; i++)
	{
		CString szUnit = _T("");
		if(i==4) szUnit = "(" + pData->m_szTonf +")";
		else if(i==5) szUnit = "(" + pData->m_szTonfM +")";
		XLText->AddMergeFormatText(0,nWidthTitle[i]-1,"$c%s%s", Title[i], szUnit);
		XLText->AddFormatText("$m+0%d",nWidthTitle[i]);
	}
	
// 	double Pu_m(0.), Mu_m(0.), Pu(0.), Mu(0.), CTC(0.);
	CString strCombo=_T(""),str=_T("");

	long j=0; for(j=0; j<RCBRIDGE_COL_CHECK; j++)
	{
		double Pu_m = pGirCol ? GetValueUnitChange(pGirCol->m_dAxialBot[j],UNIT_CGS_TONF,pData->m_szTonf)  : 0;
		double Mu_m = pGirCol ? GetValueUnitChange(pGirCol->m_dMomentBot[j],UNIT_CGS_TONFM,pData->m_szTonfM) : 0;
		double CTC  = pGirCol ? pGirCol->m_dCtcColumn    : 0;
		double Pu   = pGirCol ? GetValueUnitChange(pGirCol->m_dColAxial[j],UNIT_CGS_TONF,pData->m_szTonf)  : 0;
		double Mu   = pGirCol ? GetValueUnitChange(pGirCol->m_dColMoment[j],UNIT_CGS_TONFM,pData->m_szTonfM) : 0;
		strCombo = _T("");
		if(pGirCol)
		{
			if(pGirCol->m_nTypeDesign[j]==COMBO_ULTIMATE)
				strCombo.Format("UL%02d",pGirCol->m_nCombo[j]+1);
			if(pGirCol->m_nTypeDesign[j]==COMBO_ECCENTRIC)
				strCombo.Format("UE%02d",pGirCol->m_nCombo[j]+1);
			else if (pGirCol->m_nTypeDesign[j]==COMBO_EARTHQUAKE)
				strCombo.Format("EQ%02d",pGirCol->m_nCombo[j]+1);
			else if (pGirCol->m_nTypeDesign[j]==COMBO_EARTHQUAKE2)
				strCombo.Format("EQ%02d",pGirCol->m_nCombo[j]+1);
		}
		for(int k=0; k<nCntCol; k++)
		{
			str = _T("");
			if(k==0) str.Format("%s",szCheck[j]);
			if(k==1) str.Format("$f3%.3f", Pu_m);
			if(k==2) str.Format("$f3%.3f", Mu_m);
			if(k==3) str.Format("$f3%.3f", CTC);
			if(k==4) str.Format("$f3%.3f", Pu);
			if(k==5) str.Format("$f3%.3f", Mu);
			if(k==6) str = strCombo;
			
			if(k==0)	XLText->AddText("$n$h");
			XLText->AddMergeFormatText(0,nWidthColumn[k]-1,"$c%s",str);
			XLText->AddFormatText("$m+0%d",nWidthColumn[k]);
		}
	}
}

double CARcBridgeCalcArchRib::GetValueDeltaByRaiseRatio(double dRiseRatio)
{
	double dRetDelta	= 0;
	double dDelta1		= 0;
	double dDelta2		= 0;

	long nIdx = 0;
	double dArDelta[] = { 0.36, 0.375, 0.396, 0.422, 0.453, 0.49, 0.544, 0.596, 0.648 };
	double dRatio = 0.1;

	for(nIdx=0; nIdx<8; nIdx++)
	{
		dRatio += 0.05;
		if(dRiseRatio < dRatio) break;
	}
	
	dDelta1 = dArDelta[nIdx];
	dDelta2 = dArDelta[nIdx+1];

	dRetDelta = dDelta1 + (dDelta2-dDelta1)*((dRiseRatio-(dRatio-0.05))/0.05);

	return dRetDelta;
}

void CARcBridgeCalcArchRib::MakeTableDeltaByRaiseRatio(CXLFormatText *XLText)
{
	long nCntRow  = 2;
	long nCntCol  = 11;
	
	XLText->AddFormatText("$h▣$r라이스비에 따른 δ$n$h");
	XLText->AddTable(nCntRow-1, 23-1, 1, 2);
	//////////////////////////////////////////////////////////////////////////
	// Title
	int nWidthTitle[] = {  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3 };

	char* Title[]  = { "f/l", "0.1", "0.15", "0.2", "0.25", "0.3", "0.35", "0.4", "0.45", "0.5", "비 고"};
	char* szDelta[] = { "δ", "0.36", "0.375", "0.396", "0.422", "0.453", "0.49", "0.544", "0.596", "0.648", "양단고정아치" };
	
	int i = 0; for(i = 0; i<nCntCol; i++)
	{
		XLText->AddMergeFormatText(0,nWidthTitle[i]-1,"$c%s", Title[i]);
		XLText->AddFormatText("$m+0%d",nWidthTitle[i]);
	}
	XLText->AddFormatText("$n$h");
	for(i = 0; i<nCntCol; i++)
	{
		XLText->AddMergeFormatText(0,nWidthTitle[i]-1,"$c%s", szDelta[i]);
		XLText->AddFormatText("$m+0%d",nWidthTitle[i]);
	}
	XLText->AddFormatText("$n$h");
	*XLText += 1;

}

double CARcBridgeCalcArchRib::GetAngleRibTan(long nSpan, long nPos)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	double dAngle = 0;
	double dSta = pBri->GetStationRibBySpan(nSpan, nPos);
	
	CDPoint vAngRib = m_pStd->GetAngleArchRibBySta(dSta);
	dAngle = fabs(vAngRib.GetAngleDegree());
	
	return dAngle;
}

void CARcBridgeCalcArchRib::MakeTableColumnLoadExc(CXLFormatText *XLText, CGirderAndColumn *pGirCol, long nIndex)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	long nCntRow  = RCBRIDGE_COL_CHECK + 1;
	long nCntCol  = 6;
	
	XLText->AddTable(nCntRow-1, 22-1, 1, 2);
	//////////////////////////////////////////////////////////////////////////
	// Title
	int nWidthTitle[] = {  4, 3, 4, 4, 4, 3};
	int nWidthColumn[] = { 4, 3, 4, 4, 4, 3};
	
	char* Title[]  = { "구 분", "ζ", "축력(Hd)", "모멘트", "전단력", "비 고"};
	char* szCheck[] = { "축력최대","모멘트최대", "편심최대","지진시"};
	
	int i = 0; for(i = 0; i<nCntCol; i++)
	{
		XLText->AddMergeFormatText(0,nWidthTitle[i]-1,"$c%s", Title[i]);
		XLText->AddFormatText("$m+0%d",nWidthTitle[i]);
	}
	
// 	double Pu=0., Md=0., Sh=0., dJeta=0.;
	double dAngle = m_dArrAngTan.GetAt(nIndex);
	double dHcr = m_dArrHcr.GetAt(nIndex);
	
	CString strCombo=_T(""),str=_T("");
	
	long j=0; for(j=0; j<RCBRIDGE_COL_CHECK; j++)
	{
		CConcBasic *pConc = pGirCol->pConcColumn[j];

		double Pu   = pGirCol ? GetValueUnitChange(pGirCol->m_dColAxial[j],UNIT_CGS_TONF,pData->m_szTonf) : 0;
		double Md   = pGirCol ? GetValueUnitChange(pConc->m_Moment,UNIT_CGS_TONFM,pData->m_szTonfM) : 0;
		double Sh	 = pGirCol ? GetValueUnitChange(pConc->m_Shear,UNIT_CGS_TONF,pData->m_szTonf) : 0;
		double dJeta = fabs((Pu * cos(ToRadian(dAngle))) / dHcr);
		strCombo = _T("");
		if(pGirCol)
		{
			if(pGirCol->m_nTypeDesign[j]==COMBO_ULTIMATE)
				strCombo.Format("UL%02d",pGirCol->m_nCombo[j]+1);
			if(pGirCol->m_nTypeDesign[j]==COMBO_ECCENTRIC)
				strCombo.Format("UE%02d",pGirCol->m_nCombo[j]+1);
			else if (pGirCol->m_nTypeDesign[j]==COMBO_EARTHQUAKE)
				strCombo.Format("EQ%02d",pGirCol->m_nCombo[j]+1);
			else if (pGirCol->m_nTypeDesign[j]==COMBO_EARTHQUAKE2)
				strCombo.Format("EQ%02d",pGirCol->m_nCombo[j]+1);
		}
		for(int k=0; k<nCntCol; k++)
		{
			str = _T("");
			if(k==0) str.Format("%s",szCheck[j]);
			if(k==1) str.Format("$f3%.3f", dJeta);
			if(k==2) str.Format("$f3%.3f", Pu);
			if(k==3) str.Format("$f3%.3f", Md);
			if(k==4) str.Format("$f3%.3f", Sh);
			if(k==5) str = strCombo;
			
			if(k==0)	XLText->AddText("$n$h");
			XLText->AddMergeFormatText(0,nWidthColumn[k]-1,"$c%s",str);
			XLText->AddFormatText("$m+0%d",nWidthColumn[k]);
		}
	}
	XLText->AddText("$n$h");
}

void CARcBridgeCalcArchRib::DrawGraphXY(CXLFormatText *XLText)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CString str = _T("");
	double ScaleDim = 50;
	CDomyun *pDom = new CDomyun;
	CDomyun Dom(pDom);

	Dom.SetCWnd(m_pStd->m_pView);
	Dom.SetScaleDim(ScaleDim);
	Dom.SetLTScale(ScaleDim*5);

	Dom.Rectangle(0, 0, 18000, -7000);

	double dY = 0;
	Dom.SetTextAlignHorz(TA_RIGHT);
	Dom.SetTextHeight(5);
	long n=0; for(n=1; n<8; n++)
	{
		dY = -1000 * n;
		Dom.LineTo(0, dY, 18000, dY);
		str.Format("%d", 1000*n);
		Dom.TextOut(0, dY, str);
	}
	*pDom << Dom;

	Dom.SetTextAlignHorz(TA_LEFT);
	for(n=1; n<19; n++)
	{
		dY = -1000 * n;
		Dom.LineTo(0, dY, -200, dY);
		str.Format("%d", 1000*n);
		Dom.TextOut(0, dY, str);
	}
	Dom.Rotate(CDPoint(0, 0), CDPoint(0, 1));
	*pDom << Dom;

// 	double dDist1 = 0.;
// 	double dDist2 = 0.;
	CDPoint xyCenUp = pBri->m_pbRib[0].GetXyByDist(0);
	CDPoint xyCenLo = pBri->m_pbRib[1].GetXyByDist(0);
	CDPoint xyCenMi = pBri->m_pbRibCen.GetXyByDist(0);
	CDPoint xy1, xy2;
	for(n=0; n<100; n++)
	{
		double dDist1 = 200 * n;
		double dDist2 = 200 * (n+1);

		xy1 = pBri->m_pbRib[0].GetXyByDist(dDist1) - xyCenUp;
		xy2 = pBri->m_pbRib[0].GetXyByDist(dDist2) - xyCenUp;
		if(xy2.y >= -7000) Dom.LineTo(xy1, xy2);
		else break;

		xy1 = pBri->m_pbRib[1].GetXyByDist(dDist1) - xyCenLo;
		xy2 = pBri->m_pbRib[1].GetXyByDist(dDist2) - xyCenLo;
		if(xy2.y >= -7000) Dom.LineTo(xy1, xy2);

		xy1 = pBri->m_pbRibCen.GetXyByDist(dDist1) - xyCenMi;
		xy2 = pBri->m_pbRibCen.GetXyByDist(dDist2) - xyCenMi;
		if(xy2.y >= -7000) Dom.LineTo(xy1, xy2);
	}
	*pDom << Dom;

	long nSizeRow = 11;
	XLText->InsertDomyun(pDom,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);
}

double CARcBridgeCalcArchRib::GetOutValueArchRibBySlab(long nSpan, long nPos, long nTypeDesign, long nTypeForce, BOOL bCrown)
{
	if(nTypeForce!=ELE_MOMENT2 && nTypeForce!=ELE_SHEAR_2) return 0;
	if(nPos==CHECK_POS_CENTER && nTypeForce==ELE_SHEAR_2) return 0;

	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CFEMManage *pFEM = (nTypeDesign==COMBO_EARTHQUAKE) ? &pBri->m_FemEQ[1] : &pBri->m_FemNormal;
	if(nTypeDesign==COMBO_EARTHQUAKE2) pFEM = &pBri->m_FemEQ[2];
	if(pFEM->IsOutputLoaded()==FALSE) return 0;
	
	double dOutputMax = 0;
	double dOutputMin = 0;
	double dValueMax  = 0;
	double dValueMin  = 0;
	
	long nIdxEleStt = pBri->GetNumberElementArchRibSpanSttCheck(nSpan,nTypeDesign, bCrown);
	long nIdxEleEnd = pBri->GetNumberElementArchRibSpanEndCheck(nSpan,nTypeDesign, bCrown);
	if(nIdxEleStt==-1 || nIdxEleEnd==-1) return dOutputMax;
	
	CString szCombo = "ENVUSE MAX";
	if(nTypeDesign==COMBO_ULTIMATE)    szCombo = "ENVULT MAX";
	if(nTypeDesign==COMBO_ECCENTRIC)   szCombo = "ENVECC MAX";
	if(nTypeDesign==COMBO_EARTHQUAKE)  szCombo = "ENV MAX";
	if(nTypeDesign==COMBO_EARTHQUAKE2) szCombo = "ENV MAX";
	
	long nLoadCase  = pFEM->GetLoadCaseIndex(szCombo);	
	if(nLoadCase<0) return dOutputMax;
	
	CElement *pEle = NULL;
	CResultForceElement *pResultMax = NULL;
	CResultForceElement *pResultMin = NULL;
	// 하나의 부재에 대하여 부재력을 산출한다...
	BOOL bFirst = TRUE;
	if(nPos==CHECK_POS_LEFT || nPos==CHECK_POS_RIGHT)
	{
		long nIdxEle = (nPos==0) ? nIdxEleStt : nIdxEleEnd;
		BOOL bSttPos = (nPos==0) ? TRUE : FALSE;
		pEle = pFEM->GetElement((USHORT)nIdxEle);
		pResultMax = pEle->GetResultForce(nLoadCase);
		pResultMin = pEle->GetResultForce(nLoadCase+1);
		
		long nPoint  = bSttPos ? 0 : pResultMax->m_ResultForceElement.GetSize()-1;
		dValueMax  = pResultMax->m_ResultForceElement[nPoint]->m_dForceOnElement[nTypeForce];
		dValueMin  = pResultMin->m_ResultForceElement[nPoint]->m_dForceOnElement[nTypeForce];
		dOutputMax = bFirst ? dValueMax : max(dValueMax,dOutputMax);
		dOutputMin = bFirst ? dValueMin : min(dValueMin,dOutputMin);
		bFirst = FALSE;
	}
	else
	{
		long nIdxEle = (nIdxEleStt+nIdxEleEnd)/2;
		
		pEle = pFEM->GetElement((USHORT)nIdxEle);
		pResultMax = pEle->GetResultForce(nLoadCase);
		pResultMin = pEle->GetResultForce(nLoadCase+1);
			
		long nSizePoint = pResultMax->m_ResultForceElement.GetSize();
		long i=0; for(i=0; i<nSizePoint; i++)
		{
			dValueMax  = pResultMax->m_ResultForceElement[i]->m_dForceOnElement[nTypeForce];
			dValueMin  = pResultMin->m_ResultForceElement[i]->m_dForceOnElement[nTypeForce];
			dOutputMax = bFirst ? dValueMax : max(dValueMax,dOutputMax);
			dOutputMin = bFirst ? dValueMin : min(dValueMin,dOutputMin);
			bFirst = FALSE;
		}
	}
	
	double dValueReturn = (fabs(dOutputMax) > fabs(dOutputMin))? dOutputMax : dOutputMin;
	if(nTypeForce==ELE_SHEAR_2)
	{
		dValueReturn = max(fabs(dOutputMax),fabs(dOutputMin));
	}
	
	double dArchRibWidth = m_dWidth;	// 아치리브 폭원
	double dDivide = (nTypeDesign>=COMBO_EARTHQUAKE) ? dArchRibWidth : 1.00;
	return dValueReturn/dDivide;
}

void CARcBridgeCalcArchRib::MakeTableArchRibJewon(CXLFormatText *XLText)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	long nCntRow  = 4;
	long nCntCol  = 7;
	
	XLText->AddFormatText("$h▣$r아치리브 제원표$n$h");
	XLText->AddTable(nCntRow-1, 22-1, 1, 2);
	//////////////////////////////////////////////////////////////////////////
	// Title
	int nWidthTitle[] = {  4,3,3,3,3,3,3 };
	
	char* Title[]  = { "구  분", "Sta.", "EL.", "l₁", "f", "m", "라이즈비"};
	char* szItem[] = { "리브상단", "리브축선", "리브하단" };
	
	int i = 0; for(i = 0; i<nCntCol; i++)
	{
		XLText->AddMergeFormatText(0,nWidthTitle[i]-1,"$c%s", Title[i]);
		XLText->AddFormatText("$m+0%d",nWidthTitle[i]);
	}
	XLText->AddFormatText("$n$h");
	////////////////////////////////////////////////////////////////////////
	CString str = _T("");
	for(i = 0; i<nCntRow-1; i++)
	{
		CParabola pParabRib;
		switch(i)
		{
		case 0:
			pParabRib = pBri->m_pbRib[iUPPER]; break;
		case 1:
			pParabRib = pBri->m_pbRibCen; break;
		case 2:
			pParabRib = pBri->m_pbRib[iLOWER]; break;
		}

		long j=0; for(j=0; j<nCntCol; j++)
		{
			if(j==0) str = szItem[i];
			else if(j==1) str.Format("$f3%.3f", toM(pParabRib.m_xyCrown.x));
			else if(j==2) str.Format("$f3%.3f", toM(pParabRib.m_xyCrown.y));
			else if(j==3) str.Format("$f3%.3f", toM(pParabRib.m_dDistTotal));
			else if(j==4) str.Format("$f3%.3f", toM(pParabRib.m_dHeightTotal));
			else if(j==5) str.Format("$f2%.3f", pParabRib.m_dFactorM);
			else if(j==6)
			{
				pParabRib.SetCalcRate();
				str.Format("$f2%.3f", pParabRib.m_dRatefl);
			}

			XLText->AddMergeFormatText(0,nWidthTitle[j]-1,"$c%s", str);
			XLText->AddFormatText("$m+0%d",nWidthTitle[j]);
		}
	
		XLText->AddFormatText("$n$h");
	}
	*XLText += 1;
}

double CARcBridgeCalcArchRib::GetValueAlphaByRaiseRatio(double dRiseRatio)
{
	double dRetDelta	= 0;
	double dDelta1		= 0;
	double dDelta2		= 0;
	
	long nIdx = 0;
	double dArDelta[] = { 76.0, 63.0, 48.0, 35.0 };
	double dRatio = 0.1;
	
	for(nIdx=0; nIdx<3; nIdx++)
	{
		dRatio += 0.1;
		if(dRiseRatio < dRatio) break;
	}
	
	dDelta1 = dArDelta[nIdx];
	dDelta2 = dArDelta[nIdx+1];
	
	dRetDelta = dDelta1 + (dDelta2-dDelta1)*((dRiseRatio-(dRatio-0.1))/0.1);
	
	return dRetDelta;
}

void CARcBridgeCalcArchRib::MakeTableAlphaByRaiseRatio(CXLFormatText *XLText)
{
	long nCntRow  = 2;
	long nCntCol  = 5;
	
	XLText->AddFormatText("$h▣$r라이스비에 따른 좌굴파라미터α(강도로교 설계편람 P387)$n$h");
	XLText->AddTable(nCntRow-1, 14-1, 1, 2);
	//////////////////////////////////////////////////////////////////////////
	// Title
	int nWidthTitle[] = {  2, 3, 3, 3, 3 };
	
	char* Title[]  = { "f/l", "0.1", "0.2", "0.3", "0.4"};
	char* szDelta[] = { "α", "76.0", "63.0", "48.0", "35.0" };
	
	int i = 0; for(i = 0; i<nCntCol; i++)
	{
		XLText->AddMergeFormatText(0,nWidthTitle[i]-1,"$c%s", Title[i]);
		XLText->AddFormatText("$m+0%d",nWidthTitle[i]);
	}
	XLText->AddFormatText("$n$h");
	for(i = 0; i<nCntCol; i++)
	{
		XLText->AddMergeFormatText(0,nWidthTitle[i]-1,"$c%s", szDelta[i]);
		XLText->AddFormatText("$m+0%d",nWidthTitle[i]);
	}
	XLText->AddFormatText("$n$h");
	*XLText += 1;
	
}

double CARcBridgeCalcArchRib::GetLenArchRib()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	double dLenArchRib = 0;
	double dDistStt = pBri->m_pbRibCen.GetDistByXPos(pBri->m_tvArrVertSectionRibCen.GetAt(0).m_v1.x);
	double dDistEnd = pBri->m_pbRibCen.GetDistByXPos(pBri->m_tvArrVertSectionRibCen.GetAt(pBri->m_tvArrVertSectionRibCen.GetSize()-1).m_v2.x);

	dLenArchRib = pBri->m_pbRibCen.GetLenByDistToDist(dDistStt, dDistEnd);

	return dLenArchRib;
}

double CARcBridgeCalcArchRib::GetOutValueArchRibBySlabEQ(long nSpan, long nPos, long nTypeForce, BOOL bCrown)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	double dEQ1 = GetOutValueArchRibBySlab(nSpan,nPos,COMBO_EARTHQUAKE,nTypeForce, bCrown);
	double dEQ2 = (pBri->m_nSelectModelingCountEQ!=2) ? dEQ1 : GetOutValueArchRibBySlab(nSpan,nPos,COMBO_EARTHQUAKE2,nTypeForce, bCrown);
	
	if(nTypeForce==ELE_MOMENT2 && nPos!=CHECK_POS_CENTER)
		return min(dEQ1,dEQ2);
	else
		return max(dEQ1,dEQ2);
}

void CARcBridgeCalcArchRib::GetInxSpanAndCheckPosByIndex(long nIndex, long &nSpan, long &nPosCheck, BOOL &bCompare, BOOL &bCrown)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	long nSize = pBri->m_pArrConcArchRib.GetSize();
	long nJijumConnect = pBri->GetJijumConnectRibAndSlab();
	long nIdxCrown = (nJijumConnect * 2) + 1;

	bCrown	= FALSE;
	bCompare = FALSE;
	if(nIndex==0 || nIndex==nSize-1)							// 시종점 SPRINGING
	{
		nSpan = (nIndex==0)? 0 : (nIndex-1)/2-1;
		nPosCheck = (nIndex==0)? CHECK_POS_LEFT : CHECK_POS_RIGHT;
	}
	else if(nIndex==nIdxCrown-1 || nIndex==nIdxCrown+1)		// 리브,슬래브 접합부
	{
		nSpan = (nIndex==nIdxCrown-1)? nJijumConnect-1 : nJijumConnect;
		nPosCheck = CHECK_POS_RIGHT;
		bCompare = TRUE;
	}
	else if(nIndex == nIdxCrown)			// CROWN
	{
		bCrown = TRUE;
		nSpan = nJijumConnect-1;
		nPosCheck = CHECK_POS_RIGHT;
	}
	else if(nIndex%2 == 1)					// 중간부
	{
		nSpan = (nIndex < nIdxCrown)? (nIndex-1)/2 : (nIndex-1)/2-1;
		nPosCheck = CHECK_POS_CENTER;
	}
	else if(nIndex%2 == 0)					// 연직재 교차점
	{
		nSpan = (nIndex < nIdxCrown)? (nIndex-1)/2 : (nIndex-1)/2-1;
		nPosCheck = CHECK_POS_RIGHT;
		bCompare = TRUE;
	}
}
