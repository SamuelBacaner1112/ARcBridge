// ARcBridgeCalcFatigue.cpp: implementation of the CARcBridgeCalcFatigue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARcBridgeCalculate.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARcBridgeCalcFatigue::CARcBridgeCalcFatigue()
{

}

CARcBridgeCalcFatigue::~CARcBridgeCalcFatigue()
{
 
}

void CARcBridgeCalcFatigue::CalculateAll()
{
	CalculateSluggish();
	CalculateFatigue();
}

void CARcBridgeCalcFatigue::WriteCalcSluggishAndFatigue(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	HGBOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$s+05$e$h$a%d.%d ó�� ����$n", nIndexTitle, nIndexMiddle++);

	if(bLsd)
		WriteCheckSluggish_Lsd(XLText, pBri);
	else
		WriteCheckSluggish(XLText, pBri);
	*XLText += 1;

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$s+04$e$h$a%d.%d �Ƿ� ����$n", nIndexTitle, nIndexMiddle++);
	if(bLsd)
		WriteCheckFatigue_Lsd(XLText, pBri);
	else
		WriteCheckFatigue(XLText, pBri);
}

void CARcBridgeCalcFatigue::CalculateSluggish()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	HGBOOL bLsd = pData->IsLsdDesign();

	if( bLsd )
	{
		long nCntJigan = pBri->GetCountJijum() - 1;

		double dFck = pBri->GetValueFck(ePartUpperSlab, TRUE);
		double dAs_ = 0;

		m_dP0 = sqrt(dFck) / frM(UNIT_METER);
		m_dD = pBri->m_dTS;

		for ( long nJ = 0; nJ < nCntJigan; ++nJ )
		{
			CConcBasic* pConc = pBri->GetConcDataSlab(nJ, CHECK_POS_CENTER, TRUE);
			if(pConc == NULL) continue;

			double dAs = frCm2(pConc->GetRebarAreaTension());
			double dAg = pConc->m_Ho * pConc->m_Bo;

			m_dP[nJ] = dAs / dAg;
			m_dP_[nJ] = dAs_ / dAg;
			m_dK[nJ] = (nCntJigan==1) ? 1.0 : ((nJ==0 || nJ==nCntJigan-1 || nCntJigan==2) ? 1.3 : 1.5);

			m_dI[nJ] = pBri->GetLengthJigan(nJ);
			m_dRatio[nJ] = (m_dI[nJ] / m_dD);

			if( m_dP[nJ] <= m_dP0 )
				m_dResultLsd[nJ] = m_dK[nJ] * ( 11 + 1.5 * sqrt(dFck) * (m_dP0 / m_dP[nJ]) + 3.2 * sqrt(dFck) * pow(( m_dP0 / m_dP[nJ] - 1), 3.0/2) );
			else
				m_dResultLsd[nJ] = m_dK[nJ] * ( 11 + 1.5 * sqrt(dFck) * (m_dP0 / (m_dP[nJ] - m_dP_[nJ])) + 1. / 12 * sqrt(dFck) * sqrt(m_dP0 / m_dP[nJ]) );

			m_bOkSluggish[nJ] = (m_dResultLsd[nJ] > m_dRatio[nJ]) ? TRUE : FALSE;
		}
	}
	else
	{
		double Tmin = 0;
		double dSpanmax = m_pStd->GetLengthModelSpanMax();

		if(pBri->m_nQtyJigan>1)
			Tmin = (dSpanmax +3000)/30;
		else
			Tmin = 1.2 * (dSpanmax +3000)/30;

		m_dTmin = (pData->IsSI())? toM(Tmin):toCm(Tmin);
		m_bOkSluggish[0] = (Tmin < pBri->m_dTS)? TRUE :FALSE;
	}
}

void CARcBridgeCalcFatigue::CalculateFatigue()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	HGBOOL bLsd = pData->IsLsdDesign();

	if( bLsd )
	{
		long nCntJijum = pBri->GetCountJijum();		// ����, ���� ��� ����

		for (long nJijum = 0; nJijum < nCntJijum; ++nJijum )
		{
			// ����
			if( nJijum != 0)
			{
				CConcBasic *pConcSpan = pBri->GetConcDataSlab(nJijum-1, CHECK_POS_CENTER, TRUE);
				if( pConcSpan == NULL ) continue;

				for (long nMinMax = 0; nMinMax < 2; ++nMinMax)
				{
					pConcSpan->m_MomentUse = GetValueUnitChange(GetMomentFatigue(pBri, nJijum-1, CHECK_POS_CENTER, nMinMax==0 ? TRUE : FALSE), pData->m_szTonfM, UNIT_CGS_TONFM);

					CLsdBeam pBeamSpan(pConcSpan, LSD_ULS_DESIGN);
					m_dFs_Jigan[nJijum-1][nMinMax] = pBeamSpan.GetFsCrack();
				}

				m_dFF_Jigan[nJijum-1] = 166 - 0.33 * MIN(m_dFs_Jigan[nJijum-1][0], m_dFs_Jigan[nJijum-1][1]);

				double dDeltaF = MAX(m_dFs_Jigan[nJijum-1][0], m_dFs_Jigan[nJijum-1][1]) - MIN(m_dFs_Jigan[nJijum-1][0], m_dFs_Jigan[nJijum-1][1]);
				m_bOkFatigue_Jigan[nJijum-1] = (dDeltaF <= m_dFF[nJijum-1]) ? TRUE : FALSE;
			}

			// ����
			long nJ = nJijum==0 ? 0 : nJijum-1;
			long nPos = nJijum==0 ? CHECK_POS_LEFT : CHECK_POS_RIGHT;
			CConcBasic *pConc = pBri->GetConcDataSlab(nJ, nPos, TRUE);
			if( pConc == NULL ) continue;

			for (long nMinMax = 0; nMinMax<2; ++nMinMax)
			{
				pConc->m_MomentUse = GetValueUnitChange(GetMomentFatigue(pBri, nJ, nPos, nMinMax==0 ? TRUE : FALSE), pData->m_szTonfM, UNIT_CGS_TONFM);

				CLsdBeam pBeamSpan(pConc, LSD_ULS_DESIGN);
				m_dFs[nJijum][nMinMax] = pBeamSpan.GetFsCrack();
			}

			m_dFF[nJijum] = 166 - 0.33 * MIN(m_dFs[nJijum][0], m_dFs[nJijum][1]);

			double dDeltaF = MAX(m_dFs[nJijum][0], m_dFs[nJijum][1]) - MIN(m_dFs[nJijum][0], m_dFs[nJijum][1]);
			m_bOkFatigue[nJijum] = (dDeltaF <= m_dFF[nJijum]) ? TRUE : FALSE;
		}

		return;
	}

	double MomentLmax = 0;
	double MomentLmin = 0;

	

	double dFF = 0;
	if(pData->m_Fy < 3500) dFF = GetValueUnitChange(1300, UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);
	else if(pData->m_Fy < 4000) dFF = GetValueUnitChange(1400, UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);
	else if(pData->m_Fy >= 4000) dFF = GetValueUnitChange(1500, UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);

	m_dFF[0] = dFF;
	
	long nCntJigan = pBri->GetCountJijum()-1;
	for(long nSpan=0; nSpan<nCntJigan; nSpan++)
	{
		long nPos = pBri->GetOutValueSlabMaxForFatigue(nSpan, MomentLmax, MomentLmin);

		CConcBasic *pConc = pBri->GetConcDataSlab(nSpan, nPos);
		if(!pConc) return;	// added by jaeho 20050510
		CRectBeam rcBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		
		double dd = (pData->IsSI())? (pConc->m_Ho-pBri->m_dCoverLower_UpperSlab[0]):toCm(pConc->m_Ho-pBri->m_dCoverLower_UpperSlab[0]);
		double db = (pData->IsSI())? pConc->m_Bo:toCm(pConc->m_Bo);
		double As = rcBeam.GetRebarAreaUse();
		double Ml = MomentLmax - MomentLmin;
		double dn = Round(pData->m_Es/pData->m_Ec, 0);
		double dp = As/(db * dd);
		double dk = -dn * dp + sqrt(pow(dn*dp, 2) + 2*dn*dp);
		double dj = 1 - dk/3;
		
		double dFactor = (pData->IsSI())? pow(10.0, 6.0):pow(10.0, 5.0);
		double deltaF = Ml * dFactor/ (As * dj * dd);
		
		m_bOkFatigue[nSpan] = (dFF > deltaF)? TRUE:FALSE;
	}
}

void CARcBridgeCalcFatigue::WriteCheckSluggish(CXLFormatText *XLText, CRcBridgeRebar *pBri)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	MakeTableTmin(XLText);

	double dTuse = (pData->IsSI())? toM(pBri->m_dTS):toCm(pBri->m_dTS);
	double dTemp = (pData->IsSI())? toM(3000):toCm(3000);
	double dSpanmax = (pData->IsSI())? toM(m_pStd->GetLengthModelSpanMax()):toCm(m_pStd->GetLengthModelSpanMax());
	CString szUnit = (pData->IsSI())? "m":"cm";
	CString szComp = (m_bOkSluggish[0])? "<":">";
	CString szOK = (m_bOkSluggish[0])? "O.K":"N.G";

	if(pData->IsSI())
	{
		if(pBri->m_nQtyJigan>1)
		{
			XLText->AddFormatText("$h$e��α������� �ּ� �β�$m+06$c:$r$etmin$m+02$c=$r$e(S + %.3f)/30$n", dTemp);
			XLText->AddFormatText("$h$m+09$c=$r$e(%.3f + %.3f)/30$n", dSpanmax, dTemp);
		}
		else
		{
			XLText->AddFormatText("$h$e��α������� �ּ� �β�$m+06$c:$r$etmin$m+02$c=$r$e1.2 x (S + %.3f)/30$n", dTemp);
			XLText->AddFormatText("$h$m+09$c=$r$e1.2 x (%.3f + %.3f)/30$n", dSpanmax, dTemp);
		}
		XLText->AddFormatText("$h$m+09$c=$r$e%.3f %s$m+03$c%s$r$etuse$m+02$c=$r$e%.3f %s$n", m_dTmin, szUnit, szComp, dTuse, szUnit);
	}
	else
	{
		if(pBri->m_nQtyJigan>1)
		{
			XLText->AddFormatText("$h$e��α������� �ּ� �β�$m+06$c:$r$etmin$m+02$c=$r$e(S + %.1f)/30$n", dTemp);
			XLText->AddFormatText("$h$m+09$c=$r$e(%.1f + %.1f)/30$n", dSpanmax, dTemp);
		}
		else
		{
			XLText->AddFormatText("$h$e��α������� �ּ� �β�$m+06$c:$r$etmin$m+02$c=$r$e1.2 x (S + %.1f)/30$n", dTemp);
			XLText->AddFormatText("$h$m+09$c=$r$e1.2 x (%.1f + %.1f)/30$n", dSpanmax, dTemp);
		}
		XLText->AddFormatText("$h$m+09$c=$r$e%.1f %s$m+03$c%s$r$etuse$m+02$c=$r$e%.1f %s$n", m_dTmin, szUnit, szComp, dTuse, szUnit);
	}

	XLText->AddFormatText("$h$m+06$c��$r$eó�� ���� %s.$m+06[���α� ������� 4.4.9.5]$n",m_bOkSluggish[0] ? _T("���ʿ�") : _T("�ʿ�"));
}

void CARcBridgeCalcFatigue::WriteCheckSluggish_Lsd(CXLFormatText *XLText, CRcBridgeRebar *pBri)
{
	long nCntJigan = pBri->GetCountJijum() - 1;

	double dFck = pBri->GetValueFck(ePartUpperSlab, TRUE);
	double dAs_ = 0;				// ����ö�� : �ϴ� 0���� �ΰ� �۾��ϱ�� ��.

	double dCoefficient1 = 11;
	double dCoefficient2 = 1.5;
	double dCoefficient3 = 3.2;

	for ( long nJigan = 0; nJigan < nCntJigan; ++nJigan )
	{
		CConcBasic* pConc = pBri->GetConcDataSlab(nJigan, CHECK_POS_CENTER, TRUE);
		if(pConc == NULL) continue;

		double dAs = frCm2(pConc->GetRebarAreaTension());
//		double dAg = pConc->m_Ho * pConc->m_Bo;

		XLText->GoPosition(NO_MOVE,COL_POS_2);

		CString str(_T(""));
		if(nCntJigan > 1)
		{
			if(nJigan==0 || nJigan==nCntJigan-1 || nCntJigan==2)
				str = _T("���Ӻ��� ���������̹Ƿ�");
			else
				str = _T("���Ӻ��� ���������̹Ƿ�");
		}
		else
			str = _T("�ܼ��������̹Ƿ�");
		CString szComp = (m_bOkSluggish[0])? ">":"<";

		XLText->AddFormatText("$h$c%d)$r$e����%d$n$h", nJigan+1, nJigan+1 );
		XLText->AddFormatText("$h$e��0 = $m+02��fck / 1,000 = ��%.f / 1,000 = %s$n$h", dFck, GetStrDouble(m_dP0, 6, TRUE));
		XLText->AddFormatText("$h$e�� = $m+02As / Ag = %.1f / (%.f x 1,000) = %s$n$h", dAs, pConc->m_Ho, GetStrDouble(m_dP[nJigan], 6, TRUE));
		XLText->AddFormatText("$h$e��' = $m+02As' / Ag = %.1f / (%.f x 1,000) = %s$n$h", dAs_, pConc->m_Ho, GetStrDouble(m_dP_[nJigan], 6, TRUE));
		XLText->AddFormatText("$h$ek = $m+02%s %.1f$n$h", str, m_dK[nJigan]);

		if( m_dP[nJigan] <= m_dP0 )
		{
			XLText->AddFormatText("$h$e�� �� ��0 �̹Ƿ�$n$h");
			XLText->AddFormatText("$h$e�Ѱ� ����/����-�� l/d$n$h");
			XLText->AddFormatText("$h$r$p[EQ_SLUGGISH_1]");

			*XLText += 3;

			XLText->AddFormatText("$h$r$e = %.1f x [%.f + %.1f��%.f x %.6f / %.6f + %.1f��%.f x (%.6f / %.6f - 1)^(3/2)]$n$h", m_dK[nJigan], dCoefficient1, dCoefficient2, dFck, m_dP0, m_dP[nJigan], dCoefficient3, dFck, m_dP0, m_dP[nJigan]);
			XLText->AddFormatText("$h$r$e$me2 = %s$m+04$c%s$rl/d = %s / %s = %s$n$h", GetStrDouble(m_dResultLsd[nJigan], 3, TRUE), szComp, GetStrDouble(m_dI[nJigan], 0, TRUE), GetStrDouble(m_dD, 0, TRUE), GetStrDouble(m_dRatio[nJigan], 3, TRUE));
		}
		else // if( dP > dP0 )
		{
			XLText->AddFormatText("$h$e�� > ��0 �̹Ƿ�$n$h");
			XLText->AddFormatText("$h$e�Ѱ� ����/����-�� l/d$n$h");
			XLText->AddFormatText("$h$r$p[EQ_SLUGGISH_2]");

			*XLText += 3;

			XLText->AddFormatText("$h$r$e = %.1f x [%.f + %.1f��%.f x %.6f / (%.6f - %.f) + 1 / 12 x ��%.f x ��(%.6f / %.6f)]$n$h", m_dK[nJigan], dCoefficient1, dCoefficient2, dFck, m_dP0, m_dP[nJigan], m_dP_[nJigan], dFck, m_dP0, m_dP[nJigan]);
			XLText->AddFormatText("$h$r$e$me2 = %s$m+04$c%s$rl/d = %s / %s = %s$n$h", GetStrDouble(m_dResultLsd[nJigan], 3, TRUE), szComp, GetStrDouble(m_dI[nJigan], 0, TRUE), GetStrDouble(m_dD, 0, TRUE), GetStrDouble(m_dRatio[nJigan], 3, TRUE));
		}
		
		XLText->AddFormatText("$h$r$c��$r$eó�� ���� %s.$m+06%s$n",m_bOkSluggish[nJigan] ? _T("���ʿ�") : _T("�ʿ�"), pBri->m_pARcBridgeDataStd->IsDesignKDS2019() ? _T("[KDS 24 14 21 4.2.4.2]") : _T("[���α� ������� 2015, 5.8.4.2]"));
		XLText->AddFormatText("$n$r$h");
	}
}

void CARcBridgeCalcFatigue::WriteCheckFatigue(CXLFormatText *XLText, CRcBridgeRebar *pBri)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	long nIndexSmall = 1;
	long nCntJigan = pBri->GetCountJijum()-1;

	double MomentLmax = 0;
	double MomentLmin = 0;

	double dCtcMainRebar = pBri->GetCtcRebarMain();
	CString szUnit = (pData->IsSI())? "��":"��";

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$h$c-$r$e����� ������ ���Ȱ���߿� ���� ö���� ���� ���� �� ������Ʈ���� �������� ��������$n");
	XLText->AddFormatText("$h$r$e���� ������ ǥ 4.4.5�� ���� �̳��� ��� �Ƿο� ���Ͽ� ������ �ʿ䰡 ����.$n");
	*XLText += 1;
	MakeTableFf(XLText);
	*XLText += 1;

	for(long nSpan=0; nSpan<nCntJigan; nSpan++)
	{
		long nPos = pBri->GetOutValueSlabMaxForFatigue(nSpan, MomentLmax, MomentLmin);

		CConcBasic *pConc = pBri->GetConcDataSlab(nSpan, nPos);

		if(pConc == NULL) continue;
		CRectBeam rcBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);

		double dd = (pData->IsSI())? (pConc->m_Ho-pBri->m_dCoverLower_UpperSlab[0]):toCm(pConc->m_Ho-pBri->m_dCoverLower_UpperSlab[0]);
		double db = (pData->IsSI())? pConc->m_Bo:toCm(pConc->m_Bo);
		double As = rcBeam.GetRebarAreaUse();
		double Ml = MomentLmax - MomentLmin;
		double dn = Round(pData->m_Es/pData->m_Ec, 0);
		double dp = As/(db * dd);
		double dk = -dn * dp + sqrt(pow(dn*dp, 2) + 2*dn*dp);
		double dj = 1 - dk/3;

		double dFactor = (pData->IsSI())? pow(10.0, 6.0):pow(10.0, 5.0);
		double deltaF = Ml * dFactor/ (As * dj * dd);
		double Ff = m_dFF[0];



		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$s+25$e$h %d)$r$e��� ������ %d������$n", nIndexSmall++, nSpan+1);

		XLText->GoPosition(NO_MOVE,COL_POS_3);
		XLText->AddFormatText("$h$p[FATIGUE_MLMAX]$m+02$c=$r$e%.3f$me1$m+02%s$m+03$e(Ȱ���߰� ��ݿ� ���� �߱�Ǵ� �ִ� ���Ʈ)$n", MomentLmax, pData->m_szTonfM);
		XLText->AddFormatText("$h$p[FATIGUE_MLMIN]$m+02$c=$r$e%.3f$me1$m+02%s$m+03$e(Ȱ���߰� ��ݿ� ���� �߱�Ǵ� �ּ� ���Ʈ)$n", MomentLmin, pData->m_szTonfM);

		XLText->AddFormatText("$h$me2$e���ö�� :$m+03$eAs$r$c=$r");

		int j=0; for(j=0; j<REBAR_MAX_LAYER; j++)
		{
			if(pConc->m_RbT1C_DIA[j] == pConc->m_RbT2C_DIA[j] && pConc->m_RbT1C_DIA[j] != 0) XLText->AddFormatText("$h$m+05$c%s$me1$m+02$c@$r$me1$c%.0f$n", pData->GetStringDia(pConc->m_RbT1C_DIA[j], pConc->m_Fy), dCtcMainRebar/2);
			else
			{
				if(pConc->m_RbT1C_DIA[j] !=0) XLText->AddFormatText("$h$m+05$c%s$me1$m+02$c@$r$me1$c%.0f$n", pData->GetStringDia(pConc->m_RbT1C_DIA[j], pConc->m_Fy), dCtcMainRebar);
				if(pConc->m_RbT2C_DIA[j] !=0) XLText->AddFormatText("$h$m+05$c%s$me1$m+02$c@$r$me1$c%.0f$n", pData->GetStringDia(pConc->m_RbT2C_DIA[j], pConc->m_Fy), dCtcMainRebar);
			}
		}
		if(pData->IsSI()) XLText->AddFormatText("$h$m+04$c=$r$e$me1%.1f$m+02%s$n", As, szUnit);
		else XLText->AddFormatText("$h$m+04$c=$r$e$me1%.3f$m+02%s$n", As, szUnit);
		*XLText += 1;

		XLText->AddFormatText("$h$c$p[FATIGUE_ML]$r$c=$r$c$me1$p[FATIGUE_MLMAX]$m+02$c-$r$me1$c$p[FATIGUE_MLMIN]$m+02$c=$r$me1$c%.3f$m+02$c-$r$me1$c%.3f$m+02$c=$r$me1$c%.3f$m+02$e%s$n", MomentLmax, MomentLmin, Ml, pData->m_szTonfM);
		*XLText += 1;

		XLText->AddFormatText("$h$c$md1$cn$r$md1$c=$r$-01$cEs$r$md1$c=$r$-03$c%.0f$me2$m+03$md1$c=$r$md1$c%0.f$n$h$m+02$cEc$m+02$me2$c%.0f$n", pData->m_Es, dn, pData->m_Ec);
		*XLText += 1;
		if(pData->IsSI()) XLText->AddFormatText("$h$c$md1$cp$r$md1$c=$r$-02$cAs$me1$m+02$md1$c=$r$-03$c%.1f$me2$m+03$md1$c=$r$md1$c%.7f$me2$n$h$m+02$cb x d$me1$m+03$me2$c%.0f x %.0f$n", As, dp, db, dd);
		else XLText->AddFormatText("$h$c$md1$cp$r$md1$c=$r$-02$cAs$me1$m+02$md1$c=$r$-03$c%.3f$me2$m+03$md1$c=$r$md1$c%.7f$me2$n$h$m+02$cb x d$me1$m+03$me2$c%.0f x %.0f$n", As, dp, db, dd);
		*XLText += 1;

		XLText->AddFormatText("$h$ck$r$c=$r$me1$c-np$m+02$c+$r$e��((np)��+2np)$n");
		XLText->AddFormatText("$h$r$c=$r$e-%.0f x %.7f + ��((%.0f x %.7f)��+ 2 x %.0f x %.7f)$n", dn, dp, dn, dp, dn, dp);
		XLText->AddFormatText("$h$r$c=$e$r%.3f$me2$n", dk);
		*XLText += 1;
		XLText->AddFormatText("$h$cj$r$c=$r$c1 - k/3$me2$m+03$c=$r$e%.3f$me2$n", dj);
		*XLText += 1;

		if(pData->IsSI())
		{
			XLText->AddFormatText("$h$c$md1$c$p[FATIGUE_DELTAFEQ]$m+05$md1$c=$r$-07$c%.3f x 10^6$me6$m+07$md1$c=$r$md1$c%.3f %s$me3$n$h$m+06$me6$c%.1f x %.3f x %.0f$n", Ml, deltaF, pData->m_szKgf_CM2, As, dj, dd);
		}
		else
		{
			XLText->AddFormatText("$h$c$md1$c$p[FATIGUE_DELTAFEQ]$m+05$md1$c=$r$-07$c%.3f x 10^5$me6$m+07$md1$c=$r$md1$c%.3f %s$me3$n$h$m+06$me6$c%.3f x %.3f x %.1f$n", Ml, deltaF, pData->m_szKgf_CM2, As, dj, dd);
		}
		*XLText += 1;

		CString szComp = (m_bOkFatigue[nSpan])? ">":"<";
		CString szOk = (m_bOkFatigue[nSpan])? "O.K":"N.G";
		XLText->AddFormatText("$h$c$p[FATIGUE_FF]$r$c=$r$c%.3f$me2$m+03$e%s$m+02$c%s$m+02$me1$c$p[FATIGUE_DELTAF]$m+02$c=$r%.3f %s", Ff, pData->m_szKgf_CM2, szComp, deltaF, pData->m_szKgf_CM2);

		if(m_bOkFatigue[nSpan]) XLText->AddFormatText("$h$m+19$c��$r$e %s$n", szOk);
		else XLText->AddFormatText("$h$m+19$c��$r$e$# %s$n", szOk);
		*XLText += 1;
	}
}

void CARcBridgeCalcFatigue::WriteCheckFatigue_Lsd(CXLFormatText *XLText, CRcBridgeRebar *pBri)
{
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	if(pBri->m_bUnderGroundRahmen)//���߶��
	{
		XLText->AddFormatText("$h$c-$r���߶������ ������ ���� Ȱ������ ������������� ����ϹǷ� �������߿� ���� �Ƿ��Ѱ����$n");
		XLText->AddFormatText("$h$r������ �����Ѵ�.$n");
		return;
	}

	long nCntJijum = pBri->GetCountJijum();		// ����, ���� ��� ����

	XLText->AddFormatText("$h$c-$r$e��Ģ���� ���� ������ �ۿ��ϴ� ���� ��ҿ� ���翡 ���Ͽ� �Ƿ��Ѱ���¸� �����Ͽ��� �ϸ�,$n");
	XLText->AddFormatText("$h$r$e�� ������ �ش� ���縦 �����ϰ� �ִ� ö�ٿ� ���ؼ��� �����Ͽ��� �Ѵ�.$n");
	XLText->AddFormatText("$h$r$e%s$n", pBri->m_pARcBridgeDataStd->IsDesignKDS2019() ? _T("[KDS 24 14 21 4.3]") : _T("[���α� ������� 2015, 5.9]"));
	*XLText += 1;

	long nTitleNum = 1;
	long nMidJijum = 1, nMidJigan = 1;
	CStringArray strArr;
	for ( long nJ=0; nJ < nCntJijum; ++nJ )
	{
		CString strTitle;
		XLText->GoPosition(NO_MOVE,COL_POS_2);

		// ���������� �ƴϸ� �������� ����� ������ ����Ѵ�.
		if( nCntJijum > 1 && 0 < nJ )	
		{
			strTitle.Format("��� ������ �߾Ӻ�(����%d)", nMidJigan++);
			XLText->AddFormatText("$h$e%d)$r%s $n", nTitleNum++, strTitle);

			WriteCheckFatigueStress(pBri, XLText, nJ-1, CHECK_POS_CENTER, TRUE, TRUE);
			WriteCheckFatigueStress(pBri, XLText, nJ-1, CHECK_POS_CENTER, FALSE, TRUE);
		}

		if( nJ == 0 )
			strTitle = "�����ܺ�";
		else if( nJ == nCntJijum-1 ) 
			strTitle = "�����ܺ�";
		else
		{
			strTitle.Format("�߰�����%d", nMidJijum++);
		}

		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$h$e%d)$r��� ������ %s $n", nTitleNum++, strTitle);

		long nPos = nJ==0 ? CHECK_POS_LEFT : CHECK_POS_RIGHT;
		WriteCheckFatigueStress(pBri, XLText, nJ==0 ? 0 : nJ-1, nPos, TRUE);
		WriteCheckFatigueStress(pBri, XLText, nJ==0 ? 0 : nJ-1, nPos, FALSE);
	}
}

void CARcBridgeCalcFatigue::MakeTableTmin(CXLFormatText *XLText)
{
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$h$c-$r$e���̰� ������ ������ �ּҵβ�$n$h");

	int nCntRow = 4;
	XLText->AddTable(nCntRow-1, 24-1, 2, 1);

	XLText->AddMergeFormatText(1, 10-1,"$c��α����� ����");
	XLText->AddFormatText("$m+10");

	XLText->AddMergeFormatText(0, 14-1,"$c�ּҵβ�(m)");
	XLText->AddFormatText("$m+14");

	XLText->AddFormatText("$n$h$m+10");
	XLText->AddMergeFormatText(0, 7-1,"$c�ܼ��氣");
	XLText->AddFormatText("$m+%02d",7);
	XLText->AddMergeFormatText(0, 7-1,"$c���Ӱ氣");
	XLText->AddFormatText("$m+%02d",7);
	XLText->AddFormatText("$n$h");

	XLText->AddMergeFormatText(1, 10-1,"$e��ö���� ���� ������⿡ �����ϰų�\n������ ���� ������");
	XLText->AddFormatText("$m+%02d",10);
	XLText->AddMergeFormatText(1, 7-1,"$c1.2(S+3)/30");
	XLText->AddFormatText("$m+%02d",7);
	XLText->AddMergeFormatText(1, 7-1,"$c(S+3)/30");
	XLText->AddFormatText("$m+%02d",7);

	*XLText += 2;
	XLText->AddFormatText("$h$c*$r$e���ܸ� ���簡 ���Ǵ� ���, ���� ���� �����Ʈ �ܸ�� �θ��Ʈ�ܸ鿡���� ��밭����$n");
	XLText->AddFormatText("$h$r$e��ȭ�� ����ϱ� ���� ������ �� �ִ�.$n");
	XLText->AddFormatText("$h$c*$r$eS�� �氣 ���̸� ��Ÿ����(m).$n");
	*XLText += 1;
}

void CARcBridgeCalcFatigue::MakeTableFf(CXLFormatText *XLText)
{
	CARcBridgeDataStd *pData = m_pStd->m_pBri->m_pARcBridgeDataStd;

	XLText->AddFormatText("$h$c-$r$e�Ƿθ� ������� �ʾƵ� �Ǵ� ö���� ���¹���$n$h");

	int nCntRow = 4;
	XLText->AddTable(nCntRow-1, 20-1, 1, 1);

	XLText->AddMergeFormatText(0, 7-1,"$cö���� ����");
	XLText->AddFormatText("$m+07");
	XLText->AddMergeFormatText(0, 13-1,"$cö���� ���� �� �������� ���� (%s)", pData->m_szKgf_CM2);
	XLText->AddFormatText("$m+13");
	XLText->AddFormatText("$n$h");
	
	BOOL bSI = (pData->m_nTypeUnit == UNIT_TYPE_SI)? TRUE:FALSE;
	char *szItemCGS[] = { "SD 30", "SD 35", "SD 40" };
	char *szItem[] = { "SD 300", "SD 350", "SD 400" };
	double dArrFf[] = { 1300, 1400, 1500 };

	long i=0; for(i=0; i<3; i++)
	{
		XLText->AddMergeFormatText(0, 7-1,"$c%s", bSI? szItem[i] : szItemCGS[i]);
		XLText->AddFormatText("$m+07");
		XLText->AddMergeFormatText(0, 13-1,"$c$f[#,###]%.0f", GetValueUnitChange(dArrFf[i], UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2));
		XLText->AddFormatText("$m+13");
		XLText->AddFormatText("$n$h");
	}
}

double CARcBridgeCalcFatigue::GetMomentFatigue(CRcBridgeRebar *pBri, long nJigan, long nPos, HGBOOL bMax)
{
	double MomentLmax(0), MomentLmin(0);
	pBri->GetOutValueSlabMaxForFatigue_Lsd(nJigan, nPos, MomentLmax, MomentLmin);

	if( nPos != CHECK_POS_CENTER )
	{
		// �θ��Ʈ�� ��쿡�� -1�� �����ش�.
		MomentLmax *= -1; MomentLmin *= -1;
		double dTemp1 = MAX(MomentLmax, MomentLmin);
		double dTemp2 = MIN(MomentLmax, MomentLmin);

		MomentLmax = dTemp1;
		MomentLmin = dTemp2;
	}

	if( bMax )
		return MomentLmax;
	else
		return MomentLmin;
}

void CARcBridgeCalcFatigue::WriteStringArray(CXLFormatText *XLText, CStringArray &strArr)
{
	// Write
	XLText->GoPosition(NO_MOVE,COL_POS_3);
	long nSize = strArr.GetSize();
	// Write Output
	for(long n=0; n<nSize; n++)
	{
		CString str = strArr.GetAt(n);
		str.Replace(_T("\n"),_T("$n"));
		if(str.Find(hggettext("���� �տ� ����"))!=-1)
		{
			CString strPrint;
			strPrint.Format(_T("$s+%02d%s"),(long)((nSize-n)/2.0),str);
			XLText->AddFormatText(_T("$h%s"),strPrint);
		}
		else
		{
			XLText->AddFormatText(_T("$h%s"),str);
		}
	}
}

void CARcBridgeCalcFatigue::WriteCheckFatigueStress(CRcBridgeRebar *pBri, CXLFormatText *XLText, long nJigan, long nPos, HGBOOL bMax, HGBOOL bJigan)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	XLText->GoPosition(NO_MOVE,COL_POS_3);
	if( bMax == TRUE )
	{
		double dMomentMax = GetMomentFatigue(pBri, nJigan, nPos, TRUE);
		double dMomentMin = GetMomentFatigue(pBri, nJigan, nPos, FALSE);

		XLText->AddFormatText(_T("$h$eMmax =$m+03$me4$i%s$m+05N.mm$m+02(�Ƿ��Ѱ�������տ� ���� �ִ� ���Ʈ)$n"), GetStrDouble(dMomentMax, 0, TRUE));
		XLText->AddFormatText(_T("$h$eMmin =$m+03$me4$i%s$m+05N.mm$m+02(�Ƿ��Ѱ�������տ� ���� �ּ� ���Ʈ)$n$n"), GetStrDouble(dMomentMin, 0, TRUE));
	}

	CStringArray strArr;
	CConcBasic *pConcSpan = pBri->GetConcDataSlab(nJigan, nPos, TRUE);
	if( pConcSpan == NULL ) return;

	pConcSpan->m_MomentCrack = GetValueUnitChange(GetMomentFatigue(pBri, nJigan, nPos, bMax), pData->m_szTonfM, UNIT_CGS_TONFM);
	pConcSpan->m_MomentUse = pConcSpan->m_MomentCrack;

	long nPrintNum = 1;
	CLsdBeam pBeamSpan(pConcSpan, LSD_ULS_DESIGN);
	pBeamSpan.SetIndexPrintNum(nPrintNum);
	// pBeamSpan.MakeOutputCrackSectionCheck();			(ARCBRIDGE-3274) �Ƿ� ����� �տ����� ǥ�� ����
	pBeamSpan.MakeOutputCrackStressCheck(FALSE, TRUE);
	pBeamSpan.GetStringArray(strArr);

	CString str = bMax ? "fmax(�ִ� Ȱ���� ����)" : "fmin(�ּ� Ȱ���� ����)";
	XLText->AddFormatText(_T("$h$e%s$r%s$n$n"), GetNumberString(bMax ? 1 : 2, NUM_CIRCLE), str);

	WriteStringArray(XLText, strArr);

	if( bMax == FALSE )
	{
		long nIndex(nJigan);
		if(bJigan == FALSE && nPos == 2)
			nIndex = nJigan + 1;			// (ARCBRIDGE-3229) m_dFs �迭�� �ε����� ������ �ƴ϶� �����̴�.

		double dFF = bJigan ? m_dFF_Jigan[nJigan] : m_dFF[nIndex];
		double dMax = bJigan ? m_dFs_Jigan[nJigan][0] : m_dFs[nIndex][0];
		double dMin = bJigan ? m_dFs_Jigan[nJigan][1] : m_dFs[nIndex][1];
 		double dDeltaF = dMax - dMin;
		HGBOOL bOk = bJigan ? m_bOkFatigue_Jigan[nJigan] : m_bOkFatigue[nIndex];

		CString szOk = (bOk)? "O.K":"N.G";

		XLText->AddFormatText(_T("$h$e%s ffat(�Ƿ����¹���)$n"), GetNumberString(3, NUM_CIRCLE));
 		XLText->AddFormatText("$h$r$p[EQ_FATIGUE]$n");
		XLText->AddFormatText("$h$r$r = 166 - 0.33 x %.3f$m+08 =$r$me2%.3f$m+03N/mm��$n", MIN(dMax, dMin), dFF);
		XLText->AddFormatText("$h$r��f = fmax - fmin = $m+05%.3f - %.3f =$m+05%.3f �� ffat (= %.3fN/mm��)  �� %s$n$n", dMax, dMin, dDeltaF, dFF, szOk);
	}
}