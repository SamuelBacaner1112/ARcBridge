// ARcBridgeCalcAps.cpp: implementation of the CARcBridgeCalcAps class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARcBridgeCalculate.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define STR_BRACKET_STT		m_bLsd? _T("(") : _T("")
#define STR_BRACKET_END		m_bLsd? _T(")") : _T("")
#define STR_SIMBOL_ETA		m_bLsd? _T(" x ��i") : _T("")

CARcBridgeCalcAps::CARcBridgeCalcAps( CApsApp* pAps, BOOL bStt)
{
	m_pAps		= pAps;
	m_nSelectApsShearCheck	= 0;

	m_bLsd	= FALSE;
	for(long ix=0; ix<APS_CASE_COUNT; ++ix)
	{
		for(long nx=0; nx<3; ++nx)
			m_dLoadFactorDeadLoad[ix][nx]	= 0;
		m_dLoadFactorLiveLoad[ix]	= 0;
		m_dMu[ix]	= 0;
		m_dVu[ix]	= 0;
		m_dEta[ix]	= 0;
		m_szCombo[ix]	= _T("");
	}

	m_bStt		= bStt;
	m_LenAps	= 0;
	m_dSpan		= 0;
	m_dWdp		= 0;
	m_dWdw		= 0;
	m_dCo_i		= 0;
	m_dLe		= 0;
	m_dD		= 0;
	m_dDB		= 0;
	for(long nx=0; nx<3; ++nx)
	{
		m_dMd[nx]		= 0;
		m_dVd[nx]		= 0;
	}
	m_VuCobel	= 0;
	m_dVlDist	= 0;
	m_UW_Ascon	= 0;
	m_UW_Conc	= 0;

	m_pConc		  = new CConcBasic;
	m_pConcCobel  = new CConcBasic;
	m_pConcCobel_ = new CConcData;
	
	m_bConcAps = FALSE;
}

CARcBridgeCalcAps::~CARcBridgeCalcAps()
{
	delete m_pConc;
	delete m_pConcCobel;
	delete m_pConcCobel_;
}

void CARcBridgeCalcAps::WriteCalcAps(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle, BOOL bWriteOne)
{
	 long nIndexSmall = 1;
	WriteDesignAps(XLText, pBri, nIndexTitle, nIndexMiddle, nIndexSmall, bWriteOne);
	WriteRebarCheck(XLText, pBri, nIndexTitle, nIndexMiddle, nIndexSmall, bWriteOne);		// �ܸ�°���, ����ö�� ���� 
	WriteCrackCheck(XLText, pBri, nIndexTitle, nIndexMiddle, nIndexSmall, bWriteOne);

	WriteDesignCobel(XLText, pBri, nIndexTitle, nIndexMiddle, nIndexSmall, bWriteOne);
}


void CARcBridgeCalcAps::CalculateAll()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB) return;
	if(pBri->IsPiRahmen())   return;

	m_nSelectApsShearCheck = pBri->m_nSelectApsShearCheck;

	CalculateAps();
	CalculateCobel();
	SetConcData();
}

void CARcBridgeCalcAps::WriteDesignAps(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long &nIndexMiddle, long &nIndexSmall, BOOL bWriteOne)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(!pBri) return;
	if(!m_pAps) return;

	BOOL bStt = (m_pAps == &pBri->m_apsStt[0]) || (m_pAps == &pBri->m_apsStt[1])? TRUE : FALSE;
	CString str(_T("")), str2(_T(""));
	double dBTH = bStt? pBri->m_dBTHS:pBri->m_dBTHE;

	double dHeightPavement = (pBri->m_nTypeBridge == BRIDGE_TYPE_BOX)? pBri->m_footingBox.m_dHeightPavement : (bStt? pBri->m_footingStt.m_dHeightPavement : pBri->m_footingEnd.m_dHeightPavement);
	double UW_BackFill = GetValueUnitChange(pBri->m_dUnitWeigthBackFill,UNIT_CGS_TONF_M3,pData->m_szTonf_M3);
	double dH_Ascon  = (pBri->m_bUnderGroundRahmen)? (dHeightPavement) : (dBTH - (m_pAps->m_dH1+m_pAps->m_dH2));
	double dH_BackFill = (pBri->m_bUnderGroundRahmen)? (dBTH - (m_pAps->m_dH1+m_pAps->m_dH2) + (pBri->m_dUnderGroundDesignHeight - dHeightPavement)): 0;
	double dH_Aps    = m_pAps->m_dH1;
	double dH_Tp	 = pBri->m_dTP;
	CString szValueEta[APS_CASE_COUNT];
	if(m_bLsd == FALSE)
	{
		dH_Ascon	= toM(dH_Ascon);
		dH_BackFill	= toM(dH_BackFill);
		dH_Aps		= toM(dH_Aps);
		dH_Tp		= toM(dH_Tp);

		szValueEta[WING_FACTOR_USE]	= _T("");
		szValueEta[WING_FACTOR_ULT]	= _T("");
	}
	else
	{
		szValueEta[WING_FACTOR_ULT].Format(_T(" x %.2f"), m_dEta[WING_FACTOR_ULT]);
		szValueEta[WING_FACTOR_USE].Format(_T(" x %.2f"), m_dEta[WING_FACTOR_USE]);
		szValueEta[WING_FACTOR_USE_JUDGE].Format(_T(" x %.2f"), m_dEta[WING_FACTOR_USE_JUDGE]);
	}
	
	if(bWriteOne) nIndexSmall = 1;
	long nIndexTiny  = 1;
	long nIndexSub	 = 1;
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	if(bWriteOne) XLText->AddFormatText("$n$s+05$e$h$a%d.%d ���� �� ���� ����$n",nIndexTitle,nIndexMiddle++);
	else XLText->AddFormatText("$n$e$h %d)$r ���� �� ���� ����$n",nIndexSmall++);

	if(bWriteOne) XLText->AddFormatText("$n$e$h %d)$r��������$n",nIndexSmall++);
	else
	{
		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$n$h%s$r��������$n", GetNumberString(nIndexTiny++,NUM_CIRCLE));
	}

	if(bWriteOne) XLText->GoPosition(NO_MOVE,COL_POS_2);
	else XLText->GoPosition(NO_MOVE,COL_POS_3);

	if(bWriteOne) XLText->AddFormatText("$h%s$r������ ����(�������)�� 70%%�� �������� �ϴ� �ܼ����� ����Ѵ�.$n", GetNumberString(nIndexTiny++,NUM_CIRCLE));
	else XLText->AddFormatText("$h%s$r������ ����(�������)�� 70%%�� �������� �ϴ� �ܼ����� ����Ѵ�.$n", GetNumberString(nIndexSub++,NUM_GANA));

	XLText->AddFormatText("$h$r$cLP$r$c=$r 0.7 x %s$m+04$c=$r %s %s$n", 
		pData->GetStrDouble(m_LenAps, UNIT_CGS_M), pData->GetStrDouble(m_dSpan, UNIT_CGS_M), pData->m_szM);

	if(bWriteOne) XLText->AddFormatText("$h%s$r����� ��������$n", GetNumberString(nIndexTiny++,NUM_CIRCLE));
	else XLText->AddFormatText("$h%s$r����� ��������$n", GetNumberString(nIndexSub++,NUM_GANA));

	if(pBri->m_bUnderGroundRahmen)
	{
		XLText->AddFormatText("$h$r$cW1$r$c=$r %s x %s$m+10$c=$r %s %s ; �������� ����%s$n", 
			pData->GetStrDouble(m_UW_Conc, UNIT_CGS_TONF_M3), pData->GetStrDouble(dH_Aps, UNIT_CGS_M), pData->GetStrDouble(m_dWdw, UNIT_CGS_TONF_M2), pData->GetStringUnitType(UNIT_CGS_TONF_M), 
			(m_bLsd || pBri->m_nSelectApsDeadLoad > 0)? _T("(DC)") : _T(""));

		if(m_bLsd == FALSE || pBri->m_nSelectApsDeadLoad < 2)
		{
			XLText->AddFormatText("$h$r$cW2$r$c=$r %s x %s + %s x %s$m+10$c=$r %s %s ; ���������� ����%s$n", 
				pData->GetStrDouble(m_UW_Ascon, UNIT_CGS_TONF_M3), pData->GetStrDouble(dH_Ascon, UNIT_CGS_M), 
				pData->GetStrDouble(UW_BackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(dH_BackFill, UNIT_CGS_M), pData->GetStrDouble(m_dWdp+m_dWds, UNIT_CGS_TONF_M2), pData->GetStringUnitType(UNIT_CGS_TONF_M),
				(m_bLsd || pBri->m_nSelectApsDeadLoad > 0)? _T("(DW)") : _T(""));
		}
		else
		{
			long nIdx = 2;
			if(m_dWdp > 0)
			{
				XLText->AddFormatText("$h$r$cW%d$r$c=$r %s x %s$m+10$c=$r %s %s ; ���������� ����(DW)$n", 
					nIdx++, pData->GetStrDouble(m_UW_Ascon, UNIT_CGS_TONF_M3), pData->GetStrDouble(dH_Ascon, UNIT_CGS_M), 
					pData->GetStrDouble(m_dWdp, UNIT_CGS_TONF_M2), pData->GetStringUnitType(UNIT_CGS_TONF_M));
			}
			if(m_dWds > 0)
			{
				XLText->AddFormatText("$h$r$cW%d$r$c=$r %s x %s$m+10$c=$r %s %s ; ���������� ����(EV)$n",  
					nIdx++, pData->GetStrDouble(UW_BackFill, UNIT_CGS_TONF_M3), pData->GetStrDouble(dH_BackFill, UNIT_CGS_M), 
					pData->GetStrDouble(m_dWds, UNIT_CGS_TONF_M2), pData->GetStringUnitType(UNIT_CGS_TONF_M));
			}
		}

		if(m_bLsd == FALSE || pBri->m_nSelectApsDeadLoad == 0)
		{
			XLText->AddFormatText("$h$r$cWd$r$c=$r %s + %s$m+10$c=$r %s %s$n", 
				pData->GetStrDouble(m_dWdp, UNIT_CGS_TONF_M2), pData->GetStrDouble(m_dWdw+m_dWds, UNIT_CGS_TONF_M2), pData->GetStrDouble(m_dWdp+m_dWdw+m_dWds, UNIT_CGS_TONF_M2), pData->GetStringUnitType(UNIT_CGS_TONF_M));
		}
	}
	else
	{
		XLText->AddFormatText("$h$r$cW1$r$c=$r %s x %s$m+06$c=$r %s %s ; �������� ����%s$n", 
			pData->GetStrDouble(m_UW_Conc, UNIT_CGS_TONF_M3), pData->GetStrDouble(dH_Aps, UNIT_CGS_M), pData->GetStrDouble(m_dWdw, UNIT_CGS_TONF_M2), pData->GetStringUnitType(UNIT_CGS_TONF_M), 
			(m_bLsd || pBri->m_nSelectApsDeadLoad > 0)? _T("(DC)") : _T(""));

		if(dH_Ascon == 0) 
		{
			XLText->AddFormatText("$h$r$cW2$r$c=$r %s x %s$m+06$c=$r %s %s ; ���������� ����%s$n", 
				pData->GetStrDouble(m_UW_Ascon, UNIT_CGS_TONF_M3), pData->GetStrDouble(dH_Tp, UNIT_CGS_M), pData->GetStrDouble(m_dWdp, UNIT_CGS_TONF_M2), pData->GetStringUnitType(UNIT_CGS_TONF_M),
				(m_bLsd || pBri->m_nSelectApsDeadLoad > 0)? _T("(DW)") : _T(""));
		}
		else
		{
			if(m_bLsd == FALSE || pBri->m_nSelectApsDeadLoad < 2)
			{
				XLText->AddFormatText("$h$r$cW2$r$c=$r %s x (%s+%s)$m+06$c=$r %s %s ; ���������� ����%s$n", 
					pData->GetStrDouble(m_UW_Ascon, UNIT_CGS_TONF_M3), pData->GetStrDouble(dH_Ascon, UNIT_CGS_M), pData->GetStrDouble(dH_Tp, UNIT_CGS_M), pData->GetStrDouble(m_dWdp+m_dWds, UNIT_CGS_TONF_M2), pData->GetStringUnitType(UNIT_CGS_TONF_M),
					(m_bLsd || pBri->m_nSelectApsDeadLoad > 0)? _T("(DW)") : _T(""));
			}
			else
			{
				XLText->AddFormatText("$h$r$cW2$r$c=$r %s x %s$m+06$c=$r %s %s ; ���������� ����(DW)$n", 
					pData->GetStrDouble(m_UW_Ascon, UNIT_CGS_TONF_M3), pData->GetStrDouble(dH_Tp, UNIT_CGS_M), pData->GetStrDouble(m_dWdp, UNIT_CGS_TONF_M2), pData->GetStringUnitType(UNIT_CGS_TONF_M));

				XLText->AddFormatText("$h$r$cW2$r$c=$r %s x %s$m+06$c=$r %s %s ; ���������� ����(EV)$n", 
					pData->GetStrDouble(m_UW_Ascon, UNIT_CGS_TONF_M3), pData->GetStrDouble(dH_Ascon, UNIT_CGS_M), pData->GetStrDouble(m_dWds, UNIT_CGS_TONF_M2), pData->GetStringUnitType(UNIT_CGS_TONF_M));
			}
		}
		
		if(m_bLsd == FALSE || pBri->m_nSelectApsDeadLoad == 0)
		{
			XLText->AddFormatText("$h$r$cWd$r$c=$r %s + %s$m+06$c=$r %s %s$n", 
				pData->GetStrDouble(m_dWdp+m_dWds, UNIT_CGS_TONF_M2), pData->GetStrDouble(m_dWdw, UNIT_CGS_TONF_M2), pData->GetStrDouble(m_dWdp+m_dWds+m_dWdw, UNIT_CGS_TONF_M2), pData->GetStringUnitType(UNIT_CGS_TONF_M));
		}
	}


	if(bWriteOne)
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$n$e$h %d)$r�������߿� ���� �����$n",nIndexSmall++);
	}
	else
	{
		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$n$h%s$r�������߿� ���� �����$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
	}

	CString sOffset(_T(""));

	if(m_bLsd == FALSE || pBri->m_nSelectApsDeadLoad == 0)
	{
		if(m_dVlDist > 0)
		{
			str.Format("%s x %s / 2 - %s x %s",
				pData->GetStrDouble(m_dWdp+m_dWdw+m_dWds, UNIT_CGS_TONF_M2), pData->GetStrDouble(m_dSpan, UNIT_CGS_M), pData->GetStrDouble(m_dWdp+m_dWdw+m_dWds, UNIT_CGS_TONF_M2),  pData->GetStrDouble(m_dVlDist, UNIT_CGS_M));
			sOffset = _T("11");
		}
		else 
		{
			str.Format("%s x %s / 2", pData->GetStrDouble(m_dWdp+m_dWdw+m_dWds, UNIT_CGS_TONF_M2), pData->GetStrDouble(m_dSpan, UNIT_CGS_M)); 
			sOffset = _T("07");
		}

		XLText->AddFormatText("$h$r$cVd$r$c=$r %s$m+%s$c=$r %s %s$n", 
			str, sOffset, pData->GetStrDouble(m_dVd[0], UNIT_CGS_TONF), pData->GetStringUnitType(UNIT_CGS_TONF));

		XLText->AddFormatText("$h$r$cMd$r$c=$r %s x %s��/ 8 $m+%s$c=$r %s %s/%s$n", 
			pData->GetStrDouble(m_dWdp+m_dWds+m_dWdw, UNIT_CGS_TONF_M2), pData->GetStrDouble(m_dSpan, UNIT_CGS_M), sOffset, pData->GetStrDouble(m_dMd[0], UNIT_CGS_TONF), pData->m_szTonfM, m_bLsd? _T("mm") : _T("m"));
	}
	else
	{
		for(long nx=0; nx<3; ++nx)
		{
			double dW(0);
			if(nx == 0)
				dW = m_dWdw;
			else if(nx == 1)
				dW = pBri->m_nSelectApsDeadLoad == 1 ? (m_dWdp + m_dWds) : m_dWdp;
			else
				dW = m_dWds;

			if(nx > pBri->m_nSelectApsDeadLoad)
				break;
			if(m_dVd[nx] == 0)
				break;

			if(m_dVlDist > 0)
			{
				str.Format("%s x %s / 2 - %s x %s",
					pData->GetStrDouble(dW, UNIT_CGS_TONF_M2), pData->GetStrDouble(m_dSpan, UNIT_CGS_M), pData->GetStrDouble(dW, UNIT_CGS_TONF_M2),  pData->GetStrDouble(m_dVlDist, UNIT_CGS_M));
				sOffset = _T("11");
			}
			else 
			{
				str.Format("%s x %s / 2", pData->GetStrDouble(dW, UNIT_CGS_TONF_M2), pData->GetStrDouble(m_dSpan, UNIT_CGS_M)); 
				sOffset = _T("07");
			}

			XLText->AddFormatText("$h$r$cVd%d$r$c=$r %s$m+%s$c=$r %s %s$n", 
				nx+1, str, sOffset, pData->GetStrDouble(m_dVd[nx], UNIT_CGS_TONF), pData->GetStringUnitType(UNIT_CGS_TONF));

			XLText->AddFormatText("$h$r$cMd%d$r$c=$r %s x %s��/ 8 $m+%s$c=$r %s %s/%s$n", 
				nx+1, pData->GetStrDouble(dW, UNIT_CGS_TONF_M2), pData->GetStrDouble(m_dSpan, UNIT_CGS_M), sOffset, pData->GetStrDouble(m_dMd[nx], UNIT_CGS_TONF), pData->m_szTonfM, m_bLsd? _T("mm") : _T("m"));
		}
	}

	if(bWriteOne)
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$n$e$h %d)$rȰ���߿� ���� �����$n",nIndexSmall++);
	}
	else
	{
		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$n$h%s$rȰ���߿� ���� �����$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
	}
	
	str = pData->GetStringLoadDB();
	if(m_bLsd)
	{
		str2.Format("%.2f", m_dCo_i);
	}
	else
	{
		str2.Format("%.1f", m_dCo_i);
	}
	
	XLText->AddFormatText("$h$rȰ������ %s������ �����ϱ�� �Ѵ�. ��ݰ��(i)�� %s���� �Ѵ�.$n", str, str2);
	if(IsApplyApsAxleWeight(pBri))
		XLText->AddFormatText("$h$r$p[APS_PS2]$n");
	else
		XLText->AddFormatText("$h$r$p[APS_PS]$n");
	*XLText += 7;

	XLText->AddFormatText("$h$r$cPs$r$c=$r 2 x %s x (1 + i) /��%s x (%s + 2D)��$n", 
		pData->GetStrDouble(m_dDB, UNIT_CGS_TONF, FALSE, 1), pData->GetStrDouble(m_bLsd? 3000 : toM(3000), UNIT_CGS_M, FALSE, 0), pData->GetStrDouble(m_bLsd? 200 : toM(200), UNIT_CGS_M, FALSE, 1));
	XLText->AddFormatText("$h$r$r$c=$r 2 x %s x (1 + %.3f) /��%s x (%s + 2 x %s)��$n", 
		pData->GetStrDouble(m_dDB, UNIT_CGS_TONF, FALSE, 1), m_dCo_i, pData->GetStrDouble(m_bLsd? 3000 : toM(3000), UNIT_CGS_M, FALSE, 0), pData->GetStrDouble(m_bLsd? 200 : toM(200), UNIT_CGS_M, FALSE, 1), pData->GetStrDouble(m_dD, UNIT_CGS_M));
	XLText->AddFormatText("$h$r$r$c=$r %s %s$n", pData->GetStrDouble(m_dPs, UNIT_CGS_TONF_M2), pData->m_szTonf_M2);

	str = pData->GetStrDouble(m_dPs, UNIT_CGS_TONF_M2);
	str2 = pData->GetStrDouble(m_dLe, UNIT_CGS_M);

	if(IsApplyApsAxleWeight(pBri))
	{
		CString strText(_T(""));
		if(m_dLe > 1200)
		{
			CString szDist1(_T("")), szDist2(_T(""));
			if(pBri->m_nSelectApsShearCheck == 0)
			{
				// ������ġ
			}
			else if(pBri->m_nSelectApsShearCheck == 1)
			{
				// �������� d��ġ
				szDist1 = _T(" - Ps x (Le + 1,200) x d / Lp");
				szDist2.Format(_T(" - %s x (%s + 1,200) x %0.f / %s"), 
					str, str2, m_dVlDist, pData->GetStrDouble(m_dSpan, UNIT_CGS_M));
			}
			else
			{
				// �������� d/2��ġ
				szDist1 = _T(" - Ps x (Le + 1,200) x d / (2 x Lp)");
				szDist2.Format(_T(" - %s x (%s + 1,200) x %.0f / (2 x %s)"), 
					str, str2, m_dVlDist, pData->GetStrDouble(m_dSpan, UNIT_CGS_M));
			}
			// VI
			XLText->AddFormatText(_T("$h$rVl   = 2 x Ps x Le - Ps x (Le +1,200)��/ (2 x Lp)%s$n"), szDist1);
			strText.Format(_T("     = 2 x %s x %s - %s x (%s +1,200)2 / (2 x %s)"), 
				str, str2, str, str2, pData->GetStrDouble(m_dSpan, UNIT_CGS_M));
			XLText->AddFormatText(_T("$h$r%s$n"), strText);
			if(m_dVlDist > 0)
			{
				XLText->AddFormatText(_T("$h$r     =%s$n"), szDist2);
			}
			strText.Format(_T("     = %s %s"), pData->GetStrDouble(m_dVl, UNIT_CGS_TONF), pData->m_szTonf);
			XLText->AddFormatText(_T("$h$r%s$n"), strText);

			// MI
			XLText->AddText(_T("$h$rMl   = Ps x Lp/2 x Le - Ps x (Le/2 + 1,200/2) x (Le/2 + 1,200/2)/2$n"));
			str.Format(_T("     = %s x %s/2 x %s - %s x (%s/2 + 1,200/2) x (%s/2 + 1,200/2)/2"), 
				str, pData->GetStrDouble(m_dSpan, UNIT_CGS_M), str2, str, str2, str2);
			XLText->AddFormatText(_T("$h$r%s$n"), str);
			strText.Format(_T("     = %s %s"), pData->GetStrDouble(m_dMl, UNIT_CGS_TONFM), pData->m_szTonfM);
			XLText->AddFormatText(_T("$h$r%s$n$n"), strText);
		}
		else
		{
			CString szDist1(_T("")), szDist2(_T(""));
			if(pBri->m_nSelectApsShearCheck == 0)
			{
				// ������ġ
			}
			else if(pBri->m_nSelectApsShearCheck == 1)
			{
				// �������� d��ġ
				szDist1 = _T(" + 2 d");
				szDist2.Format(_T(" + 2 x %.0f"), m_dVlDist);
			}
			else
			{
				// �������� d/2��ġ
				szDist1 = _T(" + d");
				szDist2.Format(_T(" + %.0f"), m_dVlDist * 2);
			}
			// VI
			XLText->AddFormatText(_T("$h$rVl   = 2 x Ps x Le - Ps x Le x (Le + 1,200%s) / Lp$n"), szDist1);
			strText.Format(_T("     = 2 x %s x %s - %s x %s x (%s + 1,200%s) / %s"), 
				str, str2, str, str2, str2, szDist2, pData->GetStrDouble(m_dSpan, UNIT_CGS_M));
			XLText->AddFormatText(_T("$h$r%s$n"), strText);
			strText.Format(_T("     = %s %s"), pData->GetStrDouble(m_dVl, UNIT_CGS_TONF), pData->m_szTonf);
			XLText->AddFormatText(_T("$h$r%s$n"), strText);

			// MI
			XLText->AddText(_T("$h$rMl   = Ps x Lp/2 x Le - Ps x Le x 1,200/2$n"));
			strText.Format(_T("     = %s x %s/2 x %s - %s x %s x 1,200/2"), 
				str, pData->GetStrDouble(m_dSpan, UNIT_CGS_M), str2, 
				str, str2);
			XLText->AddFormatText(_T("$h$r%s$n"), strText);
			strText.Format(_T("     = %s %s"), pData->GetStrDouble(m_dMl, UNIT_CGS_TONFM), pData->m_szTonfM);
			XLText->AddFormatText(_T("$h$r%s$n$n"), strText);
		}
	}
	else
	{
		if(m_dVlDist > 0)
		{
			// #0006303 Le->Lp
			XLText->AddFormatText("$h$r$cVl$r$c=$r Ps x Le - (Ps x Le��+ 2 x Ps x Le x d) / (2 x Lp)$n");
			XLText->AddFormatText("$h$r$c$r$c=$r %s x %s - (%s x %s��+ 2 x %s x %s x %s) / (2 x %s)$n", 
				str, str2, str, str2, str, str2, pData->GetStrDouble(m_dVlDist, UNIT_CGS_M), pData->GetStrDouble(m_dSpan, UNIT_CGS_M));
		}
		else
		{
			XLText->AddFormatText("$h$r$cVl$r$c=$r Ps x Le - (Ps x Le��) / (2 x Lp)$n");
			XLText->AddFormatText("$h$r$r$c=$r %s x %s - (%s x %s��) / (2 x %s)$n", 
				str, str2, str, str2, pData->GetStrDouble(m_dSpan, UNIT_CGS_M));
		}
		XLText->AddFormatText("$h$r$r$c=$r %s %s$n", pData->GetStrDouble(m_dVl, UNIT_CGS_TONF), pData->m_szTonf);


		XLText->AddFormatText("$h$r$cMl$r$c=$r Ps x Lp/2 x Le/2 - Ps x Le/2 x Le/4$n");
		XLText->AddFormatText("$h$r$r$c=$r %s x %s/2 x %s/2 - %s x %s/2 x %s/4$n", 
			str, pData->GetStrDouble(m_dSpan, UNIT_CGS_M), str2, str, str2, str2);
		XLText->AddFormatText("$h$r$r$c=$r %s %s$n", pData->GetStrDouble(m_dMl, UNIT_CGS_TONFM), pData->m_szTonfM);
	}

	if(bWriteOne)
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$n$e$h %d)$r�ܸ�� ����$n",nIndexSmall++);
	}
	else
	{
		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$n$h%s$r�ܸ�� ����$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
	}

	for(long ix = APS_CASE_ULT; ix < APS_CASE_COUNT; ++ix)
	{
		if(m_bLsd)
		{
			str.Format(_T("'- %s : %s = "), (ix == APS_CASE_ULT) ? _T("�����Ѱ����") : ((ix == APS_CASE_USE_JUDGE)? _T("����Ѱ����/����") : _T("����Ѱ����/����")), m_szCombo[ix]);
			for(long nx=0; nx<3; ++nx)
			{
				if(nx > pBri->m_nSelectApsDeadLoad)
					break;
				if(m_dVd[nx] == 0)
					continue;

				str2.Format(_T("%.2f x %s + "), m_dLoadFactorDeadLoad[ix][nx], nx==0? _T("DC") : (nx==1? _T("DW") : _T("EV")));
				str += str2;
			}
			str2.Format(_T("%.2f x LL, ��i = %.2f"), m_dLoadFactorLiveLoad[ix], m_dEta[ix] );
			str += str2;
			XLText->AddFormatText(_T("$h$r%s$n"), str);
		}
		else
		{
			if(ix == APS_CASE_USE_JUDGE) break;
		}

		if(m_bLsd == FALSE || pBri->m_nSelectApsDeadLoad == 0)
		{
			XLText->AddFormatText("$h$r$cVu$r$c=$r %s%.2f x Vd + %.2f x Vl%s%s$n", 
				STR_BRACKET_STT, m_dLoadFactorDeadLoad[ix][0], m_dLoadFactorLiveLoad[ix], STR_BRACKET_END, STR_SIMBOL_ETA);
			XLText->AddFormatText("$h$r$r$c=$r %s%.2f x %s + %.2f x %s%s%s$n", 
				STR_BRACKET_STT, m_dLoadFactorDeadLoad[ix][0], pData->GetStrDouble(m_dVd[0], UNIT_CGS_TONF), m_dLoadFactorLiveLoad[ix], pData->GetStrDouble(m_dVl, UNIT_CGS_TONF), STR_BRACKET_END, szValueEta[ix]);
		}
		else if(pBri->m_nSelectApsDeadLoad == 1)
		{
			XLText->AddFormatText("$h$r$cVu$r$c=$r %s%.2f x Vd1 + %.2f x Vd2 + %.2f x Vl%s%s$n", 
				STR_BRACKET_STT, m_dLoadFactorDeadLoad[ix][0], m_dLoadFactorDeadLoad[ix][1], m_dLoadFactorLiveLoad[ix], STR_BRACKET_END, STR_SIMBOL_ETA);
			XLText->AddFormatText("$h$r$r$c=$r %s%.2f x %s + %.2f x %s + %.2f x %s%s%s$n", 
				STR_BRACKET_STT, m_dLoadFactorDeadLoad[ix][0], pData->GetStrDouble(m_dVd[0], UNIT_CGS_TONF), m_dLoadFactorDeadLoad[ix][1], pData->GetStrDouble(m_dVd[1], UNIT_CGS_TONF), 
				m_dLoadFactorLiveLoad[ix], pData->GetStrDouble(m_dVl, UNIT_CGS_TONF), STR_BRACKET_END, szValueEta[ix]);
		}
		else if(pBri->m_nSelectApsDeadLoad == 2)
		{
			XLText->AddFormatText("$h$r$cVu$r$c=$r %s%.2f x Vd1 + %.2f x Vd2 + %.2f x Vd3 + %.2f x Vl%s%s$n", 
				STR_BRACKET_STT, m_dLoadFactorDeadLoad[ix][0], m_dLoadFactorDeadLoad[ix][1], m_dLoadFactorDeadLoad[ix][2], m_dLoadFactorLiveLoad[ix], STR_BRACKET_END, STR_SIMBOL_ETA);
			XLText->AddFormatText("$h$r$r$c=$r %s%.2f x %s + %.2f x %s + %.2f x %s + %.2f x %s%s%s$n", 
				STR_BRACKET_STT, m_dLoadFactorDeadLoad[ix][0], pData->GetStrDouble(m_dVd[0], UNIT_CGS_TONF), m_dLoadFactorDeadLoad[ix][1], pData->GetStrDouble(m_dVd[1], UNIT_CGS_TONF), 
				m_dLoadFactorDeadLoad[ix][2], pData->GetStrDouble(m_dVd[2], UNIT_CGS_TONF), m_dLoadFactorLiveLoad[ix], pData->GetStrDouble(m_dVl, UNIT_CGS_TONF), STR_BRACKET_END, szValueEta[ix]);
		}
		XLText->AddFormatText("$h$r$r$c=$r %s %s$n", pData->GetStrDouble(m_dVu[ix], UNIT_CGS_TONF), pData->m_szTonf);

		if(m_bLsd == FALSE || pBri->m_nSelectApsDeadLoad == 0)
		{
			XLText->AddFormatText("$h$r$cMu$r$c=$r %s%.2f x Md + %.2f x Ml%s%s$n", 
				STR_BRACKET_STT, m_dLoadFactorDeadLoad[ix][0], m_dLoadFactorLiveLoad[ix], STR_BRACKET_END, STR_SIMBOL_ETA);
			XLText->AddFormatText("$h$r$r$c=$r %s%.2f x %s + %.2f x %s%s%s$n", 
				STR_BRACKET_STT, m_dLoadFactorDeadLoad[ix][0], pData->GetStrDouble(m_dMd[0], UNIT_CGS_TONFM), m_dLoadFactorLiveLoad[ix], pData->GetStrDouble(m_dMl, UNIT_CGS_TONFM), STR_BRACKET_END, szValueEta[ix]);
		}
		else if(pBri->m_nSelectApsDeadLoad == 1)
		{
			XLText->AddFormatText("$h$r$cMu$r$c=$r %s%.2f x Md1 + %.2f x Md2 + %.2f x Ml%s%s$n", 
				STR_BRACKET_STT, m_dLoadFactorDeadLoad[ix][0], m_dLoadFactorDeadLoad[ix][1], m_dLoadFactorLiveLoad[ix], STR_BRACKET_END, STR_SIMBOL_ETA);
			XLText->AddFormatText("$h$r$r$c=$r %s%.2f x %s + %.2f x %s + %.2f x %s%s%s$n", 
				STR_BRACKET_STT, m_dLoadFactorDeadLoad[ix][0], pData->GetStrDouble(m_dMd[0], UNIT_CGS_TONFM), m_dLoadFactorDeadLoad[ix][1], pData->GetStrDouble(m_dMd[1], UNIT_CGS_TONFM), 
				m_dLoadFactorLiveLoad[ix], pData->GetStrDouble(m_dMl, UNIT_CGS_TONFM), STR_BRACKET_END, szValueEta[ix]);
		}
		else if(pBri->m_nSelectApsDeadLoad == 2)
		{
			XLText->AddFormatText("$h$r$cMu$r$c=$r %s%.2f x Md1 + %.2f x Md2 + %.2f x Md3 + %.2f x Ml%s%s$n", 
				STR_BRACKET_STT, m_dLoadFactorDeadLoad[ix][0], m_dLoadFactorDeadLoad[ix][1], m_dLoadFactorDeadLoad[ix][2], m_dLoadFactorLiveLoad[ix], STR_BRACKET_END, STR_SIMBOL_ETA);
			XLText->AddFormatText("$h$r$r$c=$r %s%.2f x %s + %.2f x %s + %.2f x %s + %.2f x %s%s%s$n", 
				STR_BRACKET_STT, m_dLoadFactorDeadLoad[ix][0], pData->GetStrDouble(m_dMd[0], UNIT_CGS_TONFM), m_dLoadFactorDeadLoad[ix][1], pData->GetStrDouble(m_dMd[1], UNIT_CGS_TONFM), 
				m_dLoadFactorDeadLoad[ix][2], pData->GetStrDouble(m_dMd[2], UNIT_CGS_TONFM), m_dLoadFactorLiveLoad[ix], pData->GetStrDouble(m_dMl, UNIT_CGS_TONFM), STR_BRACKET_END, szValueEta[ix]);
		}
		XLText->AddFormatText("$h$r$r$c=$r %s %s$n$n", pData->GetStrDouble(m_dMu[ix], UNIT_CGS_TONFM), pData->GetStringUnitType(UNIT_CGS_TONFM));
	}
//////////////////////////////////////////////////////////////////////////
}

void CARcBridgeCalcAps::WriteRebarCheck(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long &nIndexMiddle, long &nIndexSmall, BOOL bWriteOne)
{
	if(!pBri) return;
	if(!m_pAps) return;

	long nT_1 = 1;

	if(bWriteOne) nIndexSmall = 1;
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	if(bWriteOne) 
		XLText->AddFormatText("$n$s+40$e$h$a%d.%d �ܸ�� ����$n$n",nIndexTitle,nIndexMiddle++);
	else 
		XLText->AddFormatText("$n$s+40$e$h %d)$r �ܸ�� ����$n$n",nIndexSmall++);

	if(m_bLsd)
	{
		CString str = _T(""), str2(_T(""));
		str.Format(_T("'- %s : %s = "), _T("�����Ѱ����"), m_szCombo[APS_CASE_ULT]);
		for(long nx=0; nx<3; ++nx)
		{
			if(nx > pBri->m_nSelectApsDeadLoad)
				break;

			str2.Format(_T("%.2f x %s + "), m_dLoadFactorDeadLoad[APS_CASE_ULT][nx], nx==0? _T("DC") : (nx==1? _T("DW") : _T("EV")));
			str += str2;
		}
		str2.Format(_T("%.2f x LL, ��i = %.2f"), m_dLoadFactorLiveLoad[APS_CASE_ULT], m_dEta[APS_CASE_ULT]);
		str += str2;
		
		XLText->AddFormatText(_T("$h$r%s$n$n"), str);

		CLsdBeam pBeam(m_pConc, LSD_ULS_DESIGN);
		pBeam.MakeOutputDesignExcelFile(XLText, nT_1, TRUE, FALSE);
		
	}
	else
	{
		CRectBeam pBeam(m_pConc, pBri->m_bCheckMainSlabAndFootTemper);
		pBeam.MakeOutputDesignExcelFile(XLText,nT_1, TRUE,FALSE);
	}

	if(bWriteOne) 
		XLText->AddFormatText("$n$s+40$e$h$a%d.%d ����ö�� ����$n$n",nIndexTitle,nIndexMiddle++);
	else 
		XLText->AddFormatText("$n$s+40$e$h %d)$r ����ö�� ����$n$n",nIndexSmall++);

	if(m_bLsd)
	{
		long nIdxPrint = 0;
		CLsdBeam pBeam(m_pConc, LSD_ULS_DESIGN);
		pBeam.MakeOutputDryingShrinkageExcelFile(XLText, nT_1, nIdxPrint);
	}
	else
	{
		double dLenSpan = frM(m_dSpan);
		m_pStd->m_pCorner[iUPPER]->WriteSupportCheckSlab(XLText, m_pConc, dLenSpan, pBri->m_bCheckSuppoerBarSlabMain, pBri->m_bCheckSuppoerBarSlabMainOver20, pBri->m_bCheckSuppoerBarSlabArea, pBri->m_bCheckSuppoerBarSlab300);
	}
}

void CARcBridgeCalcAps::WriteDesignCobel(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long &nIndexMiddle, long &nIndexSmall, BOOL bWriteOne)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(!IsCobel()) return;

	CString str = _T("");

	double dWd[3], dVd[3];
	if(m_bLsd == FALSE || pBri->m_nSelectApsDeadLoad == 0)
	{
		dWd[0] = (m_dWdw + m_dWdp + m_dWds);
		dWd[1] = dWd[2] = 0;
	}
	else if(pBri->m_nSelectApsDeadLoad == 1)
	{
		dWd[0] = m_dWdw;
		dWd[1] = (m_dWdp + m_dWds);
		dWd[2] = 0;
	}
	else
	{
		dWd[0] = m_dWdw;
		dWd[1] = m_dWdp;
		dWd[2] = m_dWds;
	}
	for(long ix=0; ix<3; ++ix)
		dVd[ix] = dWd[ix]*m_dSpan/2;

	double dVu = m_dLoadFactorDeadLoad[APS_CASE_ULT][0] * dVd[0] + m_dLoadFactorDeadLoad[APS_CASE_ULT][1] * dVd[1] + m_dLoadFactorDeadLoad[APS_CASE_ULT][2] * dVd[2] + m_dLoadFactorLiveLoad[APS_CASE_ULT] * m_dVl;
 
	if(bWriteOne) nIndexSmall = 1;
	long nIndexTiny  = 1;
	long nIndexSub	 = 1;
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	if(bWriteOne) 
		XLText->AddFormatText("$n$s+40$e$h$a%d.%d ��ħ�� ����$n",nIndexTitle,nIndexMiddle++);
	else 
		XLText->AddFormatText("$n$s+40$e$h %d)$r ��ħ�� ����$n",nIndexSmall++);

	if(bWriteOne)
		XLText->AddFormatText("$n$e$h %d)$r�ۿ����� ����$n",nIndexSmall++);
	else
	{
		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$n$h%s$r�ۿ����� ����$n", GetNumberString(nIndexTiny++,NUM_CIRCLE));
	}

	if(bWriteOne) 
		XLText->GoPosition(NO_MOVE,COL_POS_2);
	else 
		XLText->GoPosition(NO_MOVE,COL_POS_3);
	
	if(bWriteOne) XLText->AddFormatText("$h%s$r���ӽ����꿡�� ���޵Ǵ� ��������$n", GetNumberString(nIndexTiny++,NUM_CIRCLE));
	else XLText->AddFormatText("$h%s$r���ӽ����꿡�� ���޵Ǵ� ��������$n", GetNumberString(nIndexSub++,NUM_GANA));
	
	if(dVd[1] == 0)
	{
		XLText->AddFormatText("$h$r$cVd$r$c=$r %.3f x %.3f/2$m+05$c=$r %.3f %s$n", m_dWdp+m_dWdw+m_dWds, m_dSpan, dVd[0], pData->m_szTonf);
	}
	else
	{
		for(long ix=0; ix<3; ++ix)
		{
			XLText->AddFormatText("$h$r$cVd%d$r$c=$r %.3f x %.3f/2$m+05$c=$r %.3f %s$n", ix+1, dWd[ix], m_dSpan, dVd[ix], pData->m_szTonf);
		}
	}

	str.Format("Ȱ����(%s) : %.02f %s", pData->GetStringLoadDB(), m_dVl, pData->m_szTonf);
	
	if(bWriteOne) XLText->AddFormatText("$h%s$r$e%s$n", GetNumberString(nIndexTiny++,NUM_CIRCLE), str);
	else XLText->AddFormatText("$h%s$r$e%s$n", GetNumberString(nIndexSub++,NUM_GANA), str);

	if(m_bLsd == FALSE || pBri->m_nSelectApsDeadLoad == 0)
	{
		if(bWriteOne) XLText->AddFormatText("$h%s$r$cVu$r$c=$r %.3f x %.3f + %.3f x %.3f$m+09$c=$r %.3f %s$n", GetNumberString(nIndexTiny++,NUM_CIRCLE),
			m_dLoadFactorDeadLoad[APS_CASE_ULT][0], dVd[0], m_dLoadFactorLiveLoad[APS_CASE_ULT], m_dVl, dVu, pData->m_szTonf);
		else XLText->AddFormatText("$h%s$r$cVu$r$c=$r %.3f x %.3f + %.3f x %.3f$m+09$c=$r %.3f %s$n", GetNumberString(nIndexSub++,NUM_GANA),
			m_dLoadFactorDeadLoad[APS_CASE_ULT][0], dVd[0], m_dLoadFactorLiveLoad[APS_CASE_ULT], m_dVl, dVu, pData->m_szTonf);
	}
	else if(pBri->m_nSelectApsDeadLoad == 1)
	{
		if(bWriteOne) XLText->AddFormatText("$h%s$r$cVu$r$c=$r %.3f x %.3f + %.3f x %.3f + %.3f x %.3f$m+09$c=$r %.3f %s$n", GetNumberString(nIndexTiny++,NUM_CIRCLE),
			m_dLoadFactorDeadLoad[APS_CASE_ULT][0], dVd[0], m_dLoadFactorDeadLoad[APS_CASE_ULT][1], dVd[1], m_dLoadFactorLiveLoad[APS_CASE_ULT], m_dVl, dVu, pData->m_szTonf);
		else XLText->AddFormatText("$h%s$r$cVu$r$c=$r %.3f x %.3f + %.3f x %.3f$m+09$c=$r %.3f %s$n", GetNumberString(nIndexSub++,NUM_GANA),
			m_dLoadFactorDeadLoad[APS_CASE_ULT][0], dVd[0], m_dLoadFactorLiveLoad[APS_CASE_ULT], m_dVl, dVu, pData->m_szTonf);
	}
	else
	{
		if(bWriteOne) XLText->AddFormatText("$h%s$r$cVu$r$c=$r %.3f x %.3f + %.3f x %.3f + %.3f x %.3f + %.3f x %.3f$m+09$c=$r %.3f %s$n", GetNumberString(nIndexTiny++,NUM_CIRCLE),
			m_dLoadFactorDeadLoad[APS_CASE_ULT][0], dVd[0], m_dLoadFactorDeadLoad[APS_CASE_ULT][1], dVd[1], m_dLoadFactorDeadLoad[APS_CASE_ULT][2], dVd[2], m_dLoadFactorLiveLoad[APS_CASE_ULT], m_dVl, dVu, pData->m_szTonf);
		else XLText->AddFormatText("$h%s$r$cVu$r$c=$r %.3f x %.3f + %.3f x %.3f + %.3f x %.3f + %.3f x %.3f$m+09$c=$r %.3f %s$n", GetNumberString(nIndexSub++,NUM_GANA),
			m_dLoadFactorDeadLoad[APS_CASE_ULT][0], dVd[0], m_dLoadFactorDeadLoad[APS_CASE_ULT][1], dVd[1], m_dLoadFactorDeadLoad[APS_CASE_ULT][2], dVd[1], m_dLoadFactorLiveLoad[APS_CASE_ULT], m_dVl, dVu, pData->m_szTonf);
	}

	if(pData->IsLsdDesign())
	{
		if(bWriteOne)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$n$e$h %d)$r���ι�ħ�� ����� ��Ʈ��-Ÿ�� ���� �����Ѵ�.",nIndexSmall++);
		}
		else
		{
			XLText->GoPosition(NO_MOVE,COL_POS_2);
			XLText->AddFormatText("$n$h%s$r���ι�ħ�� ����� ��Ʈ��-Ÿ�� ���� �����Ѵ�.", GetNumberString(nIndexTiny++,NUM_CIRCLE));
		}
		return;
	}
	else
	{
		if(bWriteOne)
		{
			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$n$h* ���迡�� ��� �ܸ�¿� ���� �������Ұ���� ���ܰ����� ���� �������Ұ���� ���Ͽ����Ѵ�.");
			if (pData->m_nTypeKciCode == DESIGN_CODE_KCI_2003 || pData->m_nTypeKciCode == DESIGN_CODE_KCI_2007)
			{
				XLText->AddFormatText("$n$h$m+14[��ũ��Ʈ ����������� �ؼ�, P183]");
			}
			else
			{
				XLText->AddFormatText("$n$h$m+14[��ũ��Ʈ ����������� �ؼ�, 7.9.2]");
			}
		}
		else
		{
			XLText->GoPosition(NO_MOVE,COL_POS_2);
			XLText->AddFormatText("$n$h* ���迡�� ��� �ܸ�¿� ���� �������Ұ���� ���ܰ����� ���� �������Ұ���� ���Ͽ����Ѵ�.");
			if (pData->m_nTypeKciCode == DESIGN_CODE_KCI_2003 || pData->m_nTypeKciCode == DESIGN_CODE_KCI_2007)
			{
				XLText->AddFormatText("$n$h$m+13[��ũ��Ʈ ����������� �ؼ�, P183]");
			}
			else
			{
				XLText->AddFormatText("$n$h$m+13[��ũ��Ʈ ����������� �ؼ�, 7.9.2]");
			}
		}

		if(bWriteOne)
		{
			XLText->AddFormatText("$n$e$h %d)$r���ι�ħ�� ���� ���ܼ���",nIndexSmall++);
		}
		else
		{
			XLText->AddFormatText("$n$h%s$r���ι�ħ�� ���� ���ܼ���", GetNumberString(nIndexTiny++,NUM_CIRCLE));
		}
	}
	// �ӽ÷� CConcData�� �����Ͽ� ���.....
	CFactor *pFactor = new CFactor;
	pFactor->m_Pi_C = pData->m_Pi_C;
	pFactor->m_Pi_V = pData->m_Pi_V;
	pFactor->m_Pi_F = pData->m_Pi_F;
	pFactor->m_Pi_Cobel	= pData->m_Pi_F_Cobel;
	pFactor->m_Fck  = pBri->GetValueFck(ePartUpperSlab);
	pFactor->m_Fy   = pBri->GetValueFy(ePartUpperSlab);
	pFactor->m_Ec   = pBri->GetValueEc(ePartUpperSlab, FALSE);
	pFactor->m_Es   = pData->m_Es;

	//////////////////////////////////////////////////////////////////////////
	XLText->OutXL();

	CXLControl *pXL = XLText->GetExcelControl();	
	CCobel Cobel(m_pConcCobel_,pFactor,pData->m_nTypeUnit);

	long nRowExcel = CXLFormatText::GetRowPostion();
	nRowExcel = Cobel.MakeOutputXls(pXL,nRowExcel,COL_POS_2);
	XLText->GoPosition(nRowExcel,COL_POS_2);

	delete pFactor;
}

void CARcBridgeCalcAps::WriteCrackCheck(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long &nIndexMiddle, long &nIndexSmall, BOOL bWriteOne)
{
	if(bWriteOne) nIndexSmall = 1;

	long nT_1 = 1;

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	if(bWriteOne) 
		XLText->AddFormatText("$n$s+40$e$h$a%d.%d ��뼺 ����$n$n",nIndexTitle,nIndexMiddle++);
	else 
		XLText->AddFormatText("$n$s+40$e$h %d)$r ��뼺 ����$n$n",nIndexSmall++);

	if(m_bLsd)
	{
		if(m_pConc->m_strDesignDegree == _T("A"))
		{
			XLText->AddFormatText(_T("$h$r������ A�� ��տ� ���� �� �տ��� �Ѱ���� �������� �ƴϹǷ� �տ� ���並 �������� �ʽ��ϴ�.$n$h"));
			return;
		}

		CString sDesign= _T(""), sKind=_T(""), sExtra = _T("");
		double dCrackWitdh = pBri->GetCrackLimitWidth_LSD(m_bStt? ePartApsStt : ePartApsEnd, FALSE, sKind, sDesign); //�ձ� ��� �ޱ� �ϸ�

		BOOL bUseApsCrack = pBri->IsUseSelectLoadCaseWingOrApsMatchDesignRating(m_pConc->m_strDesignDegree , m_bStt? ePartApsStt : ePartApsEnd);
		if(pBri->IsCheckPosEtaUseLoad(m_bStt? ePartApsStt : ePartApsEnd) == FALSE || bUseApsCrack == FALSE)
		{
			XLText->AddFormatText(_T("$h �� ���� ȯ�濡 ���� ���� ���� ��� : %s %s$n$h"), sDesign, sExtra);
			XLText->AddFormatText(_T("$h������� ���տ��� �Ѱ���� ������ ���� ���������� �̰����Ͽ����Ƿ� ��뼺 ����� �������� �ʴ´�.$n$n$h"));
			return;
		}

		if(sDesign != _T("A"))
			XLText->AddFormatText(_T("$h$r ������ %s �̹Ƿ� �տ��� �Ѱ���´� %s, ǥ�� �Ѱ�տ����� %.1fmm�� ����.$n$h"), sDesign,sKind,dCrackWitdh );	


		long nMatType = pBri->GetLsdMatrialTypePos(m_bStt? ePartApsStt : ePartApsEnd, FALSE);

		CLsdBeam Beam(m_pConc, LSD_ULS_DESIGN);
		Beam.SetMatrialTypePos(nMatType);
		Beam.MakeOutputUsabilityExcelFile(XLText, nT_1, FALSE);	
	}
	else
	{
		CRectBeam pBeam(m_pConc, pBri->m_bCheckMainSlabAndFootTemper);
		pBeam.MakeOutputCrackExcelFile(XLText,nT_1,FALSE);
	}
}

void CARcBridgeCalcAps::CalculateAps()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	m_bLsd = pData->IsLsdDesign();

	CString str = _T("");
	double dBTH = ((m_pAps == &pBri->m_apsStt[0]) || (m_pAps == &pBri->m_apsStt[1]))? pBri->m_dBTHS:pBri->m_dBTHE;

	double dRateSpan = 0.70;
	double dH_Ascon  = (pBri->m_bUnderGroundRahmen)? (pBri->m_footingStt.m_dHeightPavement) : (dBTH - (m_pAps->m_dH1+m_pAps->m_dH2));
	double dH_BackFill = (pBri->m_bUnderGroundRahmen)? (dBTH - (m_pAps->m_dH1+m_pAps->m_dH2) + (pBri->m_dUnderGroundDesignHeight - pBri->m_footingStt.m_dHeightPavement)) : 0;
	double dH_Aps    = m_pAps->m_dH1;
	double dH_Tp	 = pBri->m_dTP;
	if(m_bLsd == FALSE)
	{
		dH_Ascon	= toM(dH_Ascon);
		dH_BackFill	= toM(dH_BackFill);
		dH_Aps		= toM(dH_Aps);
		dH_Tp		= toM(dH_Tp);
	}

	m_UW_Ascon = toTon(pData->m_pBasicConcInfo->m_UWeightAsphalt);
	m_UW_Conc  = toTon(pData->m_pBasicConcInfo->m_UWeightSteelConcrete);
	if(pBri->m_bUnderGroundRahmen && pBri->m_nTypePavement==1)
	{
		m_UW_Ascon = toTon(pData->m_pBasicConcInfo->m_UWeightConcrete);
	}

	m_UW_Ascon = GetValueUnitChange(m_UW_Ascon,UNIT_CGS_TONF_M3,pData->m_szTonf_M3);
	m_UW_Conc  = GetValueUnitChange(m_UW_Conc,UNIT_CGS_TONF_M3,pData->m_szTonf_M3);
	double UW_BackFill = GetValueUnitChange(pBri->m_dUnitWeigthBackFill,UNIT_CGS_TONF_M3,pData->m_szTonf_M3);

	if(m_bLsd)
	{
		CLsdManagerBridge *pLsdManager = pBri->m_plsdManager;
		for(long i = 0; i < APS_CASE_COUNT; i++)
		{
			CLsdLoadCase *pLoadCase = pLsdManager->GetLsdLoadCaseWingWallOrAps(m_dEta[i], m_szCombo[i], i != APS_CASE_ULT, i == APS_CASE_USE_JUDGE, FALSE, m_bStt);
			if(!pLoadCase) continue;

			m_dLoadFactorDeadLoad[i][0] = pLoadCase->GetLoadFactor(eLSD_LOADCASE_DC1);
			m_dLoadFactorDeadLoad[i][1] = pLoadCase->GetLoadFactor(eLSD_LOADCASE_DW1);
			m_dLoadFactorDeadLoad[i][2] = pLoadCase->GetLoadFactor(eLSD_LOADCASE_EV);
			m_dLoadFactorLiveLoad[i] =	pLoadCase->GetLoadFactor(eLSD_LOADCASE_LL1);
		}
	}
	else
	{
		m_dLoadFactorDeadLoad[APS_CASE_USE][0]	= 1.0;
		m_dLoadFactorLiveLoad[APS_CASE_USE]	= 1.0;
		m_dLoadFactorDeadLoad[APS_CASE_ULT][0]	= 1.30;
		m_dLoadFactorLiveLoad[APS_CASE_ULT]	= 2.15;

		m_dEta[APS_CASE_USE] = m_dEta[APS_CASE_ULT] = 1.0;
	}

	//////////////////////////////////////////////////////////////////////////
	// ���� �� ���� ����
	// �߰� �ؾߵ�..
	m_LenAps = max(m_pAps->GetWidth(iLEFT), m_pAps->GetWidth(iRIGHT));
	if(m_bLsd == FALSE)
	{
		m_LenAps	= toM(m_LenAps);
	}
	//////////////////////////////////////////////////////////////////////////

	m_dVlDist = 0;
	if(m_nSelectApsShearCheck==0) m_dVlDist = 0;
	if(m_nSelectApsShearCheck==1) m_dVlDist = m_bLsd? (m_pAps->m_dH1 - m_pAps->m_dCoverLower) : toM(m_pAps->m_dH1 - m_pAps->m_dCoverLower);
	if(m_nSelectApsShearCheck==2) m_dVlDist = m_bLsd? ((m_pAps->m_dH1 - m_pAps->m_dCoverLower)/2) : toM((m_pAps->m_dH1 - m_pAps->m_dCoverLower)/2);

	m_dSpan  = m_LenAps*dRateSpan;
	if(pBri->m_bUnderGroundRahmen)
	{
		m_dWdp	 = m_UW_Ascon * dH_Ascon;
		m_dWds	 = UW_BackFill * dH_BackFill;
	}
	else
	{
		m_dWdp	 = m_UW_Ascon * dH_Tp;
		m_dWds	 = m_UW_Ascon * dH_Ascon;
	}
	m_dWdw	 = m_UW_Conc*dH_Aps;

	for(long ix=0; ix<3; ++ix)
	{
		m_dVd[ix]	 = 0;
		m_dMd[ix]	 = 0;
	}
	if(m_bLsd == FALSE || pBri->m_nSelectApsDeadLoad == 0)
	{
		m_dVd[0]	 = (m_dWdp+m_dWdw+m_dWds)*m_dSpan/2.0 - (m_dWdp+m_dWdw+m_dWds)*m_dVlDist;
		m_dMd[0]	 = (m_dWdp+m_dWdw+m_dWds)*pow(m_dSpan,2)/8;
	}
	else if(pBri->m_nSelectApsDeadLoad == 1)
	{
		m_dVd[0]	 = (m_dWdw)*m_dSpan/2.0 - (m_dWdw)*m_dVlDist;
		m_dMd[0]	 = (m_dWdw)*pow(m_dSpan,2)/8;
		m_dVd[1]	 = (m_dWdp+m_dWds)*m_dSpan/2.0 - (m_dWdp+m_dWds)*m_dVlDist;
		m_dMd[1]	 = (m_dWdp+m_dWds)*pow(m_dSpan,2)/8;
	}
	else if(pBri->m_nSelectApsDeadLoad == 2)
	{
		m_dVd[0]	 = (m_dWdw)*m_dSpan/2.0 - (m_dWdw)*m_dVlDist;
		m_dMd[0]	 = (m_dWdw)*pow(m_dSpan,2)/8;
		m_dVd[1]	 = (m_dWdp)*m_dSpan/2.0 - (m_dWdp)*m_dVlDist;
		m_dMd[1]	 = (m_dWdp)*pow(m_dSpan,2)/8;
		m_dVd[2]	 = (m_dWds)*m_dSpan/2.0 - (m_dWds)*m_dVlDist;
		m_dMd[2]	 = (m_dWds)*pow(m_dSpan,2)/8;
	}
	
	// Ȱ���߸��Ʈ (���μ�����, �ѱ����ΰ���)
	m_dCo_i	= m_bLsd? 0.25 : 0.30;
	if(pBri->m_bUnderGroundRahmen)
	{
		m_dD	= m_bLsd? (dBTH - (m_pAps->m_dH1+m_pAps->m_dH2) + pBri->m_dUnderGroundDesignHeight) : toM(dBTH - (m_pAps->m_dH1+m_pAps->m_dH2) + pBri->m_dUnderGroundDesignHeight);

	}
	else
	{
		m_dD	= m_bLsd? (dBTH - m_pAps->m_dH1 - m_pAps->m_dH2 + pBri->m_dTP) : toM(dBTH - m_pAps->m_dH1 - m_pAps->m_dH2 + pBri->m_dTP);
	}
	m_dLe    = (m_bLsd? 200 : toM(200)) + 2*m_dD;
	if(m_dLe>m_dSpan)		// ACUL-6536 : ������ �������� ���ӽ����� ������ Ŭ������ ������ ������ ����
		m_dLe = m_dSpan;

	// DB ���߰����� LE�� ���� ���Ŀ�.. LE���� ���� ���� ������ �����Ǵϱ�.
	if(IsApplyApsAxleWeight(pBri))
		m_dDB      = GetValueUnitChange(m_pStd->GetWheelLoadMiddle(),UNIT_CGS_TONF,pData->m_szTonf);
	else
		m_dDB      = GetValueUnitChange(m_pStd->GetWheelLoadRear(),UNIT_CGS_TONF,pData->m_szTonf);

	m_dPs	= 2*m_dDB*(1+m_dCo_i) / ((m_bLsd? 3000 : toM(3000))*(m_dLe));

	if(IsApplyApsAxleWeight(pBri))
	{
		if(m_dLe > 1200)
		{
			m_dMl	= fabs(m_dPs * (m_dSpan/2.) * m_dLe - m_dPs * (m_dLe/2. + 1200/2.) * (m_dLe/2. + 1200/2.)/2.);
			m_dVl   = fabs(2 * m_dPs * m_dLe - (m_dPs * pow(m_dLe + 1200, 2) / (2 * m_dSpan)) - (m_dPs * (m_dLe + 1200) * m_dVlDist / m_dSpan));
		}
		else
		{
			m_dMl	= fabs(m_dPs * (m_dSpan/2.) * m_dLe - m_dPs * m_dLe * (1200/2.));
			m_dVl   = fabs(2 * m_dPs * m_dLe - (m_dPs * m_dLe * (m_dLe + 1200 + m_dVlDist * 2) / m_dSpan));
		}
	}
	else
	{
		m_dMl	= fabs(m_dPs * (m_dSpan/2.0) * (m_dLe/2.0) - m_dPs * (m_dLe/2.0) * (m_dLe/4.0));
		// #0006303 Le->Lp
		if(Compare(m_LenAps, 0.0, _T("=")))
			m_dVl = 0;
		else
			m_dVl   = fabs(m_dPs * m_dLe - (m_dPs*m_dLe*m_dLe+2*m_dPs*m_dLe*m_dVlDist)/(2*m_dSpan));
	}
	
	//
	for(long ix=APS_CASE_ULT; ix < APS_CASE_COUNT; ++ix)
	{
		m_dMu[ix] = m_dLoadFactorLiveLoad[ix]*m_dMl * m_dEta[ix];
		m_dVu[ix] = m_dLoadFactorLiveLoad[ix]*m_dVl * m_dEta[ix];
		for(long nx=0; nx<3; ++nx)
		{
			m_dMu[ix] += m_dLoadFactorDeadLoad[ix][nx]*m_dMd[nx] * m_dEta[ix];
			m_dVu[ix] += m_dLoadFactorDeadLoad[ix][nx]*m_dVd[nx] * m_dEta[ix];
		}
	}
}

double CARcBridgeCalcAps::GetVForceCorbel( BOOL bDeadLoad, long nLoadFactor )
{
	if(bDeadLoad)
	{
		CRcBridgeRebar *pBri = m_pStd->m_pBri;

		if(nLoadFactor < 0)
			return (m_dWdp + m_dWdw + m_dWds) * m_dSpan / 2;
		else if(m_bLsd == FALSE || pBri->m_nSelectApsDeadLoad == 0)
		{
			return (m_dWdp + m_dWdw + m_dWds) * m_dLoadFactorDeadLoad[nLoadFactor][0] * m_dSpan / 2;
		}
		else if(pBri->m_nSelectApsDeadLoad == 0)
		{
			return (m_dWdw * m_dLoadFactorDeadLoad[nLoadFactor][0] + (m_dWdp + m_dWds) * m_dLoadFactorDeadLoad[nLoadFactor][1]) * m_dSpan / 2;
		}
		else if(pBri->m_nSelectApsDeadLoad == 0)
		{
			return (m_dWdw * m_dLoadFactorDeadLoad[nLoadFactor][0] + m_dWdp * m_dLoadFactorDeadLoad[nLoadFactor][1] + m_dWds * m_dLoadFactorDeadLoad[nLoadFactor][2]) * m_dSpan / 2;
		}
	}
	else
	{
		if(nLoadFactor < 0)
			return m_dVl;
		else
			return m_dVl * m_dLoadFactorLiveLoad[nLoadFactor];
	}

	return 0;
}

void CARcBridgeCalcAps::CalculateCobel()
{
	double Vd = GetVForceCorbel(TRUE, APS_CASE_ULT);
	double Vl = GetVForceCorbel(FALSE, APS_CASE_ULT);

	m_VuCobel = (Vd + Vl) * m_dEta[APS_CASE_ULT];
}

BOOL CARcBridgeCalcAps::IsCobel()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	double dHH2 = ((m_pAps == &pBri->m_apsStt[0]) || (m_pAps == &pBri->m_apsStt[1]))? pBri->m_dBHHS :pBri->m_dBHHE;
	double dHH3 = 0;
	if((m_pAps == &pBri->m_apsStt[0]) || (m_pAps == &pBri->m_apsStt[1])) dHH3 = pBri->m_dBHS - dHH2;
	else dHH3 = pBri->m_dBHE - dHH2;

	if(	dHH2>0 && dHH3>0) return TRUE;

	return FALSE;
}

void CARcBridgeCalcAps::SetConcData()
{
	CHgBaseConcStd HgConcStd; 
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	double Moment=0,MomentUse=0,Shear=0,MomnentUseJudge=0;
	BOOL bCalcShear = (m_pAps->m_rebarShearDetail.m_nType==TYPE_SHEAR_SPACER)? FALSE : TRUE;
	
	m_pConc->m_nTypeUnit	= pData->m_nTypeUnit; //0-CGS,1-SI
	m_pConc->m_nTypeKciCode = pData->m_nTypeKciCode;
	m_pConc->m_nTypeSection	= SECTION_SINGLE; // �ܸ��� Ÿ��,SECTION_SINGLE
	m_pConc->m_bMinAvCheck	= pBri->m_bApplyMinAvAps;
	m_pConc->m_bCheckMinCtcShear = pBri->m_bCheckMinCtcShearAps;
	m_pConc->m_nEnvTypeCrack	 = pBri->m_nTypeCrackDesignAps;

	m_pConc->m_sTitle	= _T("");
// 	m_pConc->m_Fck	= m_pAps->m_dFck;
// 	m_pConc->m_Fy	= m_pAps->m_dFy;
// 	m_pConc->m_Ec	= pBri->GetValueEc(ePartAps, FALSE);
// 	m_pConc->m_Es	= pData->m_Es;
// 	m_pConc->m_Pi_C = pData->m_Pi_C;
// 	m_pConc->m_Pi_F	= pData->m_Pi_F;
// 	m_pConc->m_Pi_V	= pData->m_Pi_V;
	pBri->SetConcDataBasic(m_pConc, m_bStt? ePartApsStt : ePartApsEnd, FALSE, eLSD_COMBINATION_LIMIT);
	
	m_pConc->m_nCheckSupportBarRate = SUPPORTBAR_CHECK_SLAB;
	m_pConc->m_bCheckSuppoerBarAreaRate = pBri->m_bCheckSupportSlabDryingShrinkage;
	m_pConc->m_bHoriRebarCheckByReqRebar = pBri->m_bCheckSupportSlabByReqRebar;
	m_pConc->m_bCheckSupportBarMainRate20 = pBri->m_bCheckSupportSlabMainRate;
	m_pConc->m_bCheckSupportBarMainRateSpan = pBri->m_bCheckSupportSlabMainRateSpan;
	m_pConc->m_bParallelMainRebar = TRUE;
	m_pConc->m_LnSpan = m_dSpan;

//	if(bLsd == FALSE)
	{
		m_pConc->m_Fck	= m_pAps->m_dFck;
		m_pConc->m_Fy	= m_pAps->m_dFy;
		if(pData->IsLsdDesign() == FALSE) m_pConc->m_FyShear = m_pAps->m_dFy;
	}
	m_pConc->m_Bo	= frM(UNIT_METER);
	m_pConc->m_Ho	= m_pAps->m_dH1; // ������β�

	Moment			= m_dMu[APS_CASE_ULT];
	Shear			= m_dVu[APS_CASE_ULT];
	MomentUse		= m_dMu[APS_CASE_USE];
	MomnentUseJudge	= m_dMu[APS_CASE_USE_JUDGE];

	if(m_bLsd)
	{
		// �Ѱ���¿��� ������� mm ������ ��������� ����� ������(1m)�� ���ǹǷ� ����¿� x1000�� ���ش�.
		Moment		= Moment * 1000;
		Shear		= Shear * 1000;
		MomentUse	= MomentUse * 1000;
		MomnentUseJudge	= MomnentUseJudge * 1000;
	}

	// ���� CGS������ �ƴ� ���� ��ȯ�� �Ͽ� �����Ͽ� ��.....
	m_pConc->m_Moment		= GetValueUnitChange(Moment,pData->m_szTonfM,UNIT_CGS_TONFM);
	m_pConc->m_Shear		= GetValueUnitChange(Shear,pData->m_szTonf,UNIT_CGS_TONF);
	m_pConc->m_MomentUse	= GetValueUnitChange(MomentUse,pData->m_szTonfM,UNIT_CGS_TONFM);
	m_pConc->m_MomentCrack	= GetValueUnitChange(MomnentUseJudge,pData->m_szTonfM,UNIT_CGS_TONFM);

	// ����� ��ö��
	double dCtc = m_pAps->m_dCtcMain[iLOWER];
	double dB = frM(UNIT_METER)/dCtc;
	m_pConc->m_RbT1C_DIA[0]	= m_pAps->m_dDiaMain1[iLOWER];
	m_pConc->m_RbT1C_dc[0]	= m_pAps->m_dCoverLower;
	m_pConc->m_RbT1C_B[0]	= dB;

	// ����� ��ö��
	double dCtc2 = m_pAps->m_dCtcMain[iUPPER];
	double dB2 = frM(UNIT_METER)/dCtc2;
	m_pConc->m_RbC1C_DIA[0]	= m_pAps->m_dDiaMain1[iUPPER];
	m_pConc->m_RbC1C_dc[0]	= m_pAps->m_dCoverUpper;
	m_pConc->m_RbC1C_B[0]	= dB2;

	m_pConc->m_CSupportBarDia[0] = m_pAps->m_dDiaSup1[iUPPER];
	m_pConc->m_CSupportBarCTC[0] = m_pAps->m_dCtcSup[iUPPER];
	m_pConc->m_TSupportBarDia[0] = m_pAps->m_dDiaSup1[iLOWER];
	m_pConc->m_TSupportBarCTC[0] = m_pAps->m_dCtcSup[iLOWER];

	if(bCalcShear)
	{
		m_pConc->m_SvDia = m_pAps->m_rebarShearDetail.m_dDia;
		m_pConc->m_SvCtc = m_pAps->m_dCtcShear;
		m_pConc->m_SvEa  = frM(UNIT_METER)/(m_pAps->m_nCountWrapMainRebar *dCtc);
	}
	else
	{
		m_pConc->m_SvDia	= 0;
		m_pConc->m_SvCtc	= 0;
		m_pConc->m_SvEa	= 0;
	}
	//////////////////////////////////////////////////////////////////////////+

	// �Ѱ���� ���信���� ��ħ�� ���� ����.
	if(m_bLsd)
		return;
	
	// COBEL CHECK.........
	m_pConcCobel_->m_Title   = "��ħ�� ����";
	m_pConcCobel_->m_Type	 = SECT_COBEL;
	m_pConcCobel_->m_H		 = toCm(m_bStt ? pBri->m_dBHS : pBri->m_dBHE);
	m_pConcCobel_->m_H2		 = toCm(m_bStt ? pBri->m_dBHS-pBri->m_dBHHS : pBri->m_dBHE-pBri->m_dBHHE);
	m_pConcCobel_->m_B		 = 100;	// ����M
	m_pConcCobel_->m_Shear	 = GetValueUnitChange(m_VuCobel,pData->m_szTonf,UNIT_CGS_TONF);
	m_pConcCobel_->m_Ln		 = toCm((m_bStt ? pBri->m_dBWS : pBri->m_dBWE)/2);
	m_pConcCobel_->m_nUseOneBoxFactorInShear = 0;
	m_pConcCobel_->m_nTypeUnit = pData->m_nTypeUnit;
	//
	BOOL bIs1Cycle = m_bStt ? pBri->m_bIsBracketRebar[0][iSTT] : pBri->m_bIsBracketRebar[0][iEND];
	BOOL bIs2Cycle = m_bStt ? pBri->m_bIsBracketRebar[1][iSTT] : pBri->m_bIsBracketRebar[1][iEND];
	if(bIs1Cycle)
	{
		m_pConcCobel_->m_TDiaArr[0]	= pBri->m_dDiaRebarMain_Bracket_Cycle1[m_bStt ? iSTT : iEND];
		m_pConcCobel_->m_TDisArr[0]	= toCm(pBri->m_dCoverWallOut[0]);
		m_pConcCobel_->m_TEaArr[0]	= 1000/pBri->m_dCTC_MainRebar;
	}
	if(bIs2Cycle)
	{
		m_pConcCobel_->m_TDiaArr[1]	= pBri->m_dDiaRebarMain_Bracket_Cycle2[m_bStt ? iSTT : iEND];
		m_pConcCobel_->m_TDisArr[1]	= toCm(pBri->m_dCoverWallOut[0]);
		m_pConcCobel_->m_TEaArr[1]	= 1000/pBri->m_dCTC_MainRebar;
		if(m_pConcCobel_->m_TDiaArr[0]==m_pConcCobel_->m_TDiaArr[1])
		{
			m_pConcCobel_->m_TDiaArr[1] = 0;
			m_pConcCobel_->m_TEaArr[0] += m_pConcCobel_->m_TEaArr[1];
			m_pConcCobel_->m_TEaArr[1]  = 0;
		}
	}
	// ���ս��ͷ� ����... 2/3d�� ������
	CRebarShearDetail *pShearDetail = &pBri->m_RebarShearDetail_Bracket[m_bStt ? iSTT : iEND];
	CTwinVectorArray tvArr;
	pBri->GetTvRebarShear_VertSection_Bracket(tvArr, m_bStt);
	long nDanShear = tvArr.GetSize();
	m_pConcCobel_->m_StirDia    = pShearDetail->m_dDia;
	double dSvEa = 1000.0/m_pConcCobel_->GetRebarCTC(0,TRUE)/pBri->m_nCountWrapMainRebarForBracket[m_bStt ? iSTT : iEND];
	m_pConcCobel_->m_StirEa     = (pShearDetail->m_nType == TYPE_SHEAR_DIGUT || pShearDetail->m_nType == TYPE_SHEAR_SINGLE_HOOK)? dSvEa/2 : dSvEa;

	m_pConcCobel_->m_StirDan    = nDanShear;	
}

BOOL CARcBridgeCalcAps::IsApplyApsAxleWeight(CRcBridgeRebar *pBri)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	BOOL bLsd = pData->IsLsdDesign();

	// LP > LE �϶��� �ٴ��� ����
	// LE > LP �϶��� LE = LP �� ���ְ� ������ �������� ������ �ȵǴϱ� �������.
	return bLsd && pBri->m_nSelectApsAxleWeight == 0 && m_dLe < m_dSpan;
}
