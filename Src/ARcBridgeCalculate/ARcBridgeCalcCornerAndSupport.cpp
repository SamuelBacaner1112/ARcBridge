// ARcBridgeCalcCornerAndSupport.cpp: implementation of the CARcBridgeCalcCornerAndSupport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARcBridgeCalculate.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARcBridgeCalcCornerAndSupport::CARcBridgeCalcCornerAndSupport()
{
	int i=0; for(i=0; i<2; i++)
	{
		m_dTs[i]			= 0;		
		m_dTw[i]			= 0;
		m_dHH[i]			= 0;
		m_dHW[i]			= 0;
		m_dAngle[i]			= 0;
		m_dDeltaL[i]		= 0;
		m_dA[i]				= 0;
		m_dB[i]				= 0;
		m_dR[i]				= 0;
		m_dMomentUltTonM[i]	= 0;
		m_dMomentUltKgfcm[i]= 0;
		m_dMomentTonM[i]	= 0;
		m_dMomentKgfcm[i]	= 0;
		m_dW[i]				= 1000;
		m_dFtmax[i]			= 0;
		m_dFtaUnitApply[i]	= 0;
		m_dFsa[i]			= 0;
		m_dAsReq[i]			= 0;
		m_dAsUsed[i]		= 0;

		int j=0; for(j=0; j<3; j++)
		{
			m_AsDanbu[i][j] = 0;
			m_AsMain[i][j] = 0;
			m_dDanbuCtc[i][j] = 0;
			m_nCntCycleByDan[i][j] = 0;
		}

		m_nCntDanbuRebar[i]	= 0;
		m_nCntMainRebar[i]	= 0;

		m_dMainCtc			= 0;
		m_nCntMainCycle		= 0;

		m_bIsInsideBending[i]	= FALSE;
		m_dMomentInSide[i] = 0;
		m_dTh1[i]	= 0;
		m_dTh2[i]	= 0;
		m_dAs_[i]	= 0;
	}
}

CARcBridgeCalcCornerAndSupport::~CARcBridgeCalcCornerAndSupport()
{

}

void CARcBridgeCalcCornerAndSupport::CalculateAll(BOOL bUpperSlab)
{
	// 우각부 검토에 관련된 사항만 하부슬래브에 설정
	// 수평철근 관련사항은 동일
	m_bUpperSlab = bUpperSlab;

	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(bUpperSlab == FALSE && pBri->IsBoxType() == FALSE) return;

	if(pBri->m_nTypeBridge == BRIDGE_TYPE_PORTAL || pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 || pBri->IsBoxType()
		|| pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		CalculateCorner(TRUE);
		CalculateCorner(FALSE);

		m_bIsInsideBending[0] = CalculateInsideBending(TRUE);
		m_bIsInsideBending[1] = CalculateInsideBending(FALSE);
	}
}

void CARcBridgeCalcCornerAndSupport::CalculateCorner(BOOL bStt)
{
	// CGS일때는 kgf cm 를 기준으로...  SI일때는 Kn  m 를 기준으로
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nIndex = (bStt)? 0:1;
	
	double dTS = pBri->m_dTS;
	double dTW = (bStt) ? pBri->m_dWS : pBri->m_dWE;
	double dHH = (bStt) ? pBri->m_dHHS : pBri->m_dHHE;
	double dHW = (bStt) ? pBri->m_dHWS : pBri->m_dHWE;
	if(m_bUpperSlab == FALSE)
	{
		dTS = pBri->m_dTS_Lower;
		dHH = (bStt) ? pBri->m_dWSHH : pBri->m_dWEHH;
		dHW = (bStt) ? pBri->m_dWSHW : pBri->m_dWEHW;
	}

	m_dTs[nIndex] = (pData->m_nTypeUnit == 0) ? toCm(dTS) : toM(dTS);
	m_dTw[nIndex] = (pData->m_nTypeUnit == 0) ? toCm(dTW) : toM(dTW);
	m_dHH[nIndex] = (pData->m_nTypeUnit == 0) ? toCm(dHH) : toM(dHH);
	m_dHW[nIndex] = (pData->m_nTypeUnit == 0) ? toCm(dHW) : toM(dHW);
	m_dW[nIndex]  = (pData->m_nTypeUnit == 0) ? toCm(1000) : toM(1000); // 단위M당 계산
	
	dTS = m_dTs[nIndex];
	dTW = m_dTw[nIndex];
	dHH = m_dHH[nIndex];
	dHW = m_dHW[nIndex];

	double dAngle = m_dAngle[nIndex]  = ToDegree(atan(dHH/dHW));
	double dDeltaL = (dTW+dHW-dTS)/(tan(dAngle*ConstPi/180.0)+1);
	double dA  = (m_dHH[nIndex]==0 || m_dHW[nIndex]==0)? dTS : dTS + dDeltaL*tan(dAngle*ConstPi/180.0);
	double dB  = (m_dHH[nIndex]==0 || m_dHW[nIndex]==0)? dTW : dTW + dHW - dDeltaL;
	double dR  = sqrt(pow(dA, 2)+pow(dB, 2));
	double Fck = pBri->GetValueFck(ePartUpperSlab, TRUE);	//GetValueUnitChange(pData->m_Fck,UNIT_CGS_KGF_CM2,pData->m_szKgf_CM2);
	double Fy_CGS	= pBri->GetValueFy(ePartUpperSlab);
	
	m_dAngle[nIndex] = dAngle;
	m_dDeltaL[nIndex] = dDeltaL; 
	m_dA[nIndex] = dA;
	m_dB[nIndex] = dB;
	m_dR[nIndex] = dR;

	m_dMomentUltTonM[nIndex]  = pBri->GetMomentCorner(bStt, FALSE, TRUE, m_bUpperSlab);
	m_dMomentUltKgfcm[nIndex] = (pData->m_nTypeUnit == 0)? toCm(frTonM(m_dMomentUltTonM[nIndex])):m_dMomentUltTonM[nIndex]/1000;

	m_dMomentTonM[nIndex]  = pBri->GetMomentCorner(bStt, TRUE, TRUE, m_bUpperSlab);
	m_dMomentKgfcm[nIndex] = (pData->m_nTypeUnit == 0)? toCm(frTonM(m_dMomentTonM[nIndex])):m_dMomentTonM[nIndex]/1000;

	double dMomentSel = (pBri->m_nSelCalcCornerFt == 1)? m_dMomentUltKgfcm[nIndex] : m_dMomentKgfcm[nIndex];
	m_dFtmax[nIndex] = (5*dMomentSel)/(pow(dR, 2)*m_dW[nIndex]);

	if(pData->IsSI())			 // SI 단위계
	{
		switch(pBri->m_nSelCalcCornerFt)
		{
		case 0:	m_dFtaUnitApply[nIndex] = 0.08*sqrt(Fck);	break;
		case 1: m_dFtaUnitApply[nIndex] = 1/3.*sqrt(Fck);	break;
		case 2: m_dFtaUnitApply[nIndex] = 0.13*sqrt(Fck);	break;
		case 3: m_dFtaUnitApply[nIndex] = 0.22*sqrt(Fck);	break;
		}
	}
	else						 // CGS 단위계 
	{
		switch(pBri->m_nSelCalcCornerFt)
		{
		case 0:	m_dFtaUnitApply[nIndex] = 0.25*sqrt(Fck);	break;
		case 1: m_dFtaUnitApply[nIndex] = 1.06*sqrt(Fck);	break;
		case 2: m_dFtaUnitApply[nIndex] = 0.42*sqrt(Fck);	break;
		case 3: m_dFtaUnitApply[nIndex] = 0.7*sqrt(Fck);	break;
		}
	}

	// 보강철근 허용응력
	if(pData->m_Fy < 3500) 		m_dFsa[nIndex] = GetValueUnitChange(1500,UNIT_CGS_KGF_CM2,pData->m_szKgf_CM2);
	else if(pData->m_Fy < 4000) m_dFsa[nIndex] = GetValueUnitChange(1750,UNIT_CGS_KGF_CM2,pData->m_szKgf_CM2);
	else 						m_dFsa[nIndex] = GetValueUnitChange(1800,UNIT_CGS_KGF_CM2,pData->m_szKgf_CM2);

	// 필요철근량
	m_dAsReq[nIndex] = (2*m_dMomentKgfcm[nIndex])/(dR*m_dFsa[nIndex]);
	if(pData->IsSI()) m_dAsReq[nIndex] = frM2(m_dAsReq[nIndex]);

	// 사용철근량
	double dDia = 0;
	double dCtc = pBri->m_dCTC_MainRebar;
	double dUnitLen = (!pData->IsSI())? frCm(m_dW[nIndex]):frM(m_dW[nIndex]);
	double dNum = dUnitLen / dCtc;
	BOOL bBending = pBri->m_bBendingDanBuRebar;

	CHgBaseConcStd HgConcStd; 

	m_dMainCtc = pBri->m_dCTC_MainRebar;
	m_dAsUsed[nIndex] = 0;
	
	long nUD = m_bUpperSlab ? iUPPER : iLOWER;
	int i=0; for(i=0; i<3; i++)
	{
		m_AsDanbu[nIndex][i] = 0;
		if(pBri->m_bIsDanbuRebar_Cycle1[nUD][nIndex])
		{
			dDia = pBri->m_dDia_DanbuRebar_Cycle1[nUD][nIndex][i];
			m_AsDanbu[nIndex][i] += GetValueUnitChange(GetRebarArea_Fix(dDia, dNum),UNIT_CGS_CM2,pData->m_szCM2);
			m_strDanbuDia[nIndex][i][0] = pData->GetStringDia(dDia, Fy_CGS);
		}
		if(pBri->m_bIsDanbuRebar_Cycle2[nUD][nIndex])
		{
			dDia = pBri->m_dDia_DanbuRebar_Cycle2[nUD][nIndex][i];
			m_AsDanbu[nIndex][i] += GetValueUnitChange(GetRebarArea_Fix(dDia, dNum),UNIT_CGS_CM2,pData->m_szCM2);
			m_strDanbuDia[nIndex][i][1] = pData->GetStringDia(dDia, Fy_CGS);
		}

		m_dAsUsed[nIndex] += m_AsDanbu[nIndex][i];
		if(m_AsDanbu[nIndex][i] > 0) m_nCntDanbuRebar[nIndex]++;
	}

	if(bBending)		// 주철근 적용
	{
		m_nCntMainCycle	= MIN(pBri->GetCountCycleMainRebar(), 2);
		m_nCntMainRebar[nIndex] = 0;

		CRebarInfoCycle rc;
		int j=0; for(j=0; j<3; j++)			// 단수
		{
			long nCnt = 0;
			long nSpan = (bStt)? 0:pBri->m_nQtyJigan-1;
			for(int k=1; k<m_nCntMainCycle; k++)		// Cycle수
			{
				CDPoint xyInfo	= CDPoint(0, 0);
				if(m_bUpperSlab)
					xyInfo	= pBri->GetInfoMainRebar_UpperSlab(TRUE, nSpan, k, j, bStt ? -1 : 1);
				else
					xyInfo	= pBri->GetInfoMainRebar_LowerSlab(TRUE, nSpan, k, j, bStt ? -1 : 1);
				dDia = xyInfo.x;
				if(dDia == 0) continue;
				
				m_strMainDia[nIndex][j][k] = pData->GetStringDia(dDia, Fy_CGS);
				m_AsMain[nIndex][j] += GetValueUnitChange(GetRebarArea_Fix(dDia, dNum),UNIT_CGS_CM2,pData->m_szCM2);

				nCnt ++;
				if(j+1>m_nCntMainRebar[nIndex]) m_nCntMainRebar[nIndex] = j+1;
			}
			m_nCntCycleByDan[nIndex][j] = nCnt;
			m_dAsUsed[nIndex] += m_AsMain[nIndex][j];
		}
	}

}

void CARcBridgeCalcCornerAndSupport::WriteCalcCorner(CXLFormatText *XLText, long nIndexTitle, long nIndexMiddle)
{
	long nIndexSmall = 1;
	XLText->GoPosition(NO_MOVE,COL_POS_1);

	CString strTitle = _T("우각부 검토");
	if(m_pStd->m_pBri->IsBoxType())
	{
		strTitle = m_bUpperSlab ? _T("우각부 검토 : 상부슬래브") : _T("우각부 검토 : 하부슬래브");
	}
	XLText->AddFormatText("$n$s+05$e$h$a%d.%d %s$n",nIndexTitle,nIndexMiddle,strTitle);

	WriteCorner(XLText, nIndexMiddle, nIndexSmall++, TRUE);
	WriteCorner(XLText, nIndexMiddle, nIndexSmall++, FALSE);
}

void CARcBridgeCalcCornerAndSupport::WriteCalcSupport(CXLFormatText *XLText, long nIndexTitle, long nIndexMiddle)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	long nIndexSmall = 1;
	long nIndexSub	= 1;
	XLText->GoPosition(NO_MOVE,COL_POS_1);

	BOOL bExistFootOpt = (pBri->m_bCheckSuppoerBarFoot15hs || pBri->m_bCheckSuppoerBarFootMainOver20 || pBri->m_bCheckSuppoerBarFootArea || pBri->m_bCheckSuppoerBarFoot300);
	if(pBri->IsOutsideWall() && pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB && pBri->m_nTypeBridge != BRIDGE_TYPE_PI)
		XLText->AddFormatText("$s+99");
	XLText->AddFormatText("$e$h$a%d.%d 수평철근 검토$n",nIndexTitle,nIndexMiddle);
	XLText->AddFormatText("$n$h");
	XLText->AddFormatText("$e$h %d)$r검토 결과 요약$n", nIndexSmall++);
	// 슬래브
	MakeTableSupportSumSlab(XLText, nIndexSub++);
	if(pBri->IsBoxType())
	{
		if(bExistFootOpt)
			MakeTableSupportSumLowerSlab(XLText, nIndexSub++, FALSE);
	}
	// 시.종점 벽체
	if(pBri->IsOutsideWall()) 
	{
		MakeTableSupportSumWall(XLText, nIndexSub++);	// 중간벽체는 포함되지 않으니 시종점 벽체가 있을때만.
	}
	// 중간벽체
	MakeTableSupportSumInWall(XLText, nIndexSub++);		// 중간 벽체 추가 ㅡㅡ
	// 아치리브
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH) 
	{
		MakeTableSupportSumWall(XLText, nIndexSub++, TRUE);
	}
	if(pBri->m_bExistProtectionRiseFooting)
	{
		if(bExistFootOpt)
			MakeTableSupportSumLowerSlab(XLText, nIndexSub++, TRUE);
	}
	
	// 기초는 기초검토에서..

	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$s+99$e$h %d)$r수평철근 검토$n", nIndexSmall++);
	WriteSupportCheck(XLText, nIndexMiddle, nIndexSmall++);
}

void CARcBridgeCalcCornerAndSupport::WriteCorner(CXLFormatText *XLText, long nIndexMiddle, long nIndexSmall, BOOL bStt)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
//	CString str = _T("");
	CString strsub = _T("");
	CString strTemp = _T("");
	// 길이의 기본 단위는 CGS단위계일때 cm, SI단위계 일때 m 로 한다.
	CString strUnitLen = (pData->m_nTypeUnit == 0)? _T("cm"):_T("m");

	CString str = (bStt)? _T("시점측 우각부"):_T("종점측 우각부");
	XLText->AddFormatText("$n");

	long nIndexSub = 1;
	long nIndex = (bStt)? 0:1;
	if(!bStt) XLText->AddFormatText("$s+99");
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$e$h %d)$r %s$n", nIndexSmall, str);

	CDomyun *pDom = new CDomyun;
	DrawCorner(pDom, bStt);
	XLText->InsertDomyun(pDom,m_pStd->m_pView,10,CXLFormatText::INSERT_DOM_CURPOS | CXLFormatText::INSERT_DOM_MOVEROW);
	
	XLText->AddFormatText("$h$u$u$u$u$u$u$u$u");
	XLText->AddFormatText("$h$t$t$t슬래브 두께$m+02$r$c=$r$e%.03f %s$n$l", m_dTs[nIndex], strUnitLen);
	XLText->AddFormatText("$h$t$t$t벽체 두께$m+02$r$c=$r$e%.03f %s$n$l", m_dTw[nIndex], strUnitLen);
	XLText->AddFormatText("$h$t$t$t헌치 높이$m+02$r$c=$r$e%.03f %s$n$l", m_dHH[nIndex], strUnitLen);
	XLText->AddFormatText("$h$t$t$t헌치 폭$m+02$r$c=$r$e%.03f %s$n$l", m_dHW[nIndex], strUnitLen);
	XLText->AddFormatText("$h$n$n$n$n$n$h");

	// 외측인장 휨모멘트 보강검토
	BOOL bUseMomentUlt	= (pBri->m_nSelCalcCornerFt == 1)? TRUE : FALSE;
	double dMomentSel	= (bUseMomentUlt)? m_dMomentUltKgfcm[nIndex] : m_dMomentKgfcm[nIndex];
	CString szEqFta		= _T("");
	switch(pBri->m_nSelCalcCornerFt)
	{
	case 0:	szEqFta = (pData->IsSI())? _T("0.08√Fck") : _T("0.25√Fck"); break;
	case 1:	szEqFta = (pData->IsSI())? _T("1/3 x √Fck") : _T("1.06√Fck"); break;
	case 2:	szEqFta = (pData->IsSI())? _T("0.13√Fck") : _T("0.42√Fck"); break;
	case 3:	szEqFta = (pData->IsSI())? _T("0.22√Fck") : _T("0.7√Fck"); break;
	}

	if(m_bIsInsideBending[nIndex])
		XLText->AddFormatText("$i$h%s$r외측인장 휨모멘트 보강검토$n$h", GetNumberString(nIndexSub++,NUM_CIRCLE));
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	if(m_dHH[nIndex]==0 || m_dHW[nIndex]==0)
	{
		XLText->AddFormatText("$ha$r$c=$e$r%.3f %s$n", m_dTs[nIndex], strUnitLen);
		XLText->AddFormatText("$hb$r$c=$e$r%.3f %s$n", m_dTw[nIndex], strUnitLen);
	}
	else
	{
		XLText->AddFormatText("$ha$r$c=$e$r%.3f + tan(%.3f) x ΔL$n", m_dTs[nIndex], m_dAngle[nIndex]);
		XLText->AddFormatText("$hb$r$c=$e$r%.3f + %.3f - ΔL$n", m_dTw[nIndex], m_dHW[nIndex]);
		XLText->AddFormatText("$ha = b ,$m+02$rΔL$r$c=$r$e(%.3f+%.3f-%.3f)/(tan(%.3f)+1)$n", m_dTw[nIndex], m_dHW[nIndex], m_dTs[nIndex], m_dAngle[nIndex]);
		XLText->AddFormatText("$hΔL$r$c=$e$r%.3f %s$n", m_dDeltaL[nIndex], strUnitLen);
		XLText->AddFormatText("$ha$r$c=$e$r%.3f + tan(%.3f) x %.3f$m+08$r=$r%.3f %s$n", m_dTs[nIndex], m_dAngle[nIndex], m_dDeltaL[nIndex], m_dA[nIndex], strUnitLen);
		XLText->AddFormatText("$hb$r$c=$e$r%.3f + %.3f - %.3f$m+08$r=$r%.3f %s$n", m_dTw[nIndex], m_dHW[nIndex], m_dDeltaL[nIndex], m_dB[nIndex], strUnitLen);
	}

	XLText->AddFormatText("$hR$r$c=$e$r√(a²+ b²)$n");
	XLText->AddFormatText("$h$r$c=$e$r√(%.3f²+ %.3f²)$m+08$r=$r%.3f %s$n", m_dA[nIndex], m_dB[nIndex], m_dR[nIndex], strUnitLen);
	*XLText += 1;
	XLText->AddFormatText("$hMu$r$c=$e$r%.3f %s$m+04$r(계수하중하에서의 우각부 최대 모멘트)$n", m_dMomentUltTonM[nIndex], pData->m_szTonfM);
	XLText->AddFormatText("$hMo$r$c=$e$r%.3f %s$m+04$r(사용하중하에서의 우각부 최대 모멘트)$n", m_dMomentTonM[nIndex], pData->m_szTonfM);
	XLText->AddFormatText("$hW$r$c=$e$r%.3f %s$m+04$r(절점부의 구조물 폭)$n", m_dW[nIndex], strUnitLen);
	
	if(bUseMomentUlt) XLText->AddFormatText("$h$p[CORNER_FTMAXL]$r$c=$r$e$p[CORNER_FTMAXR_ULT]$n$n");
	else XLText->AddFormatText("$h$p[CORNER_FTMAXL]$r$c=$r$e$p[CORNER_FTMAXR_USE]$n$n");

	if(pData->IsSI()) XLText->AddFormatText("$h$r$c=$e$r(5 x %.1f) / (%.1f²x %.1f)$n", dMomentSel * pow(10.0, 9.0), frM(m_dR[nIndex]), frM(m_dW[nIndex]));
	else XLText->AddFormatText("$h$r$c=$e$r(5 x %.3f) / (%.3f²x %.3f)$n", dMomentSel, m_dR[nIndex], m_dW[nIndex]);
	XLText->AddFormatText("$h$r$c=$e$r%.3f %s$n", m_dFtmax[nIndex], pData->m_szKgf_CM2);

	XLText->AddFormatText("$hfta$r$c=$e$r%s$m+04$r=$r%.3f %s", szEqFta, m_dFtaUnitApply[nIndex], pData->m_szKgf_CM2);

	str = (m_dFtmax[nIndex] > m_dFtaUnitApply[nIndex])? "<":">";
	XLText->AddFormatText("$m+05%s$rftmax", str);
	str = (m_dFtmax[nIndex] > m_dFtaUnitApply[nIndex])? "∴ 우각부 보강철근 필요":"∴ 우각부 보강철근 불필요";
	XLText->AddFormatText("$m+03%s$n", str);

	if(m_dFtmax[nIndex] > m_dFtaUnitApply[nIndex])
	{
		str = (pData->m_nTypeUnit == 0)? _T("㎠"):_T("㎟");
		XLText->AddFormatText("$h필요철근량$m+02$r$c=$r$e$p[CORNER_AS]$n$n");
		if(pData->IsSI()) XLText->AddFormatText("$h$r$r$r$c=$r$e(2 x %.1f) / (%.1f x %.0f)$n", m_dMomentKgfcm[nIndex]*pow(10.0, 9.0), frM(m_dR[nIndex]), m_dFsa[nIndex]);
		else XLText->AddFormatText("$h$r$r$r$c=$r$e(2 x %.3f) / (%.3f x %.0f)$n", m_dMomentKgfcm[nIndex], m_dR[nIndex], m_dFsa[nIndex]);
		XLText->AddFormatText("$h$r$r$r$c=$r$e%.3f %s$n", m_dAsReq[nIndex], str);

		XLText->AddFormatText("$h사용철근량$m+02$r$c=$r$e%.3f %s$n", m_dAsUsed[nIndex], str);

		int i=0; for(i=0; i<m_nCntDanbuRebar[nIndex]; i++)
		{
			if(m_nCntMainRebar[nIndex] == 0) strsub.Format("- %d단", i+1);
			else strsub.Format("- 우각부 %d단", i+1);

			XLText->AddFormatText("$h%s$m+02$r$c:$r$e", strsub);

			if(!pBri->IsDanbuRebar(bStt, i, 0, TRUE) && !pBri->IsDanbuRebar(bStt, i, 1, TRUE)) continue;
			if(m_strDanbuDia[nIndex][i][0] == m_strDanbuDia[nIndex][i][1])
			{
				XLText->AddFormatText("%s$r$c@$r$e%.0f mm$m+01$r", m_strDanbuDia[nIndex][i][0], m_dMainCtc/2);
			}
			else
			{
				if(pBri->IsDanbuRebar(bStt, i, 0, TRUE))
				{
					XLText->AddFormatText("%s$r$c@$r$e%.0f mm$m+01$r", m_strDanbuDia[nIndex][i][0], m_dMainCtc);
				}
				if(pBri->IsDanbuRebar(bStt, i, 0, TRUE) && pBri->IsDanbuRebar(bStt, i, 1, TRUE))
				{
					XLText->AddFormatText(",$r");
					XLText->AddFormatText("%s$r$c@$r$e%.0f mm$m+01$r", m_strDanbuDia[nIndex][i][1], m_dMainCtc);
				}
				if(!pBri->IsDanbuRebar(bStt, i, 0, TRUE) && pBri->IsDanbuRebar(bStt, i, 1, TRUE))
				{
					XLText->AddFormatText("%s$r$c@$r$e%.0f mm$m+01$r", m_strDanbuDia[nIndex][i][1], m_dMainCtc);
				}
			}

			XLText->AddFormatText("$h$t$t$r$r$r$r$c=$m+04$i%.3f %s$e$n", m_AsDanbu[nIndex][i], str);
		}
		
		if(m_nCntMainRebar[nIndex] != 0)
		{
			for(i=0; i<m_nCntMainRebar[nIndex]; i++)
			{
				strsub.Format("- 주철근 %d단", i+1);
				XLText->AddFormatText("$h%s$m+02$r$c:$r$e", strsub);

//				CDPoint xyInfo	= CDPoint(0, 0);
				long nSpan = (bStt)? 0:pBri->m_nQtyJigan-1;
				CDPoint xyInfo	= pBri->GetInfoMainRebar_UpperSlab(TRUE, nSpan, 1, i, (bStt)? -1:1);

				XLText->AddFormatText("%s$r$c@$r$e%.0f mm$m+01$r", m_strMainDia[nIndex][i][1], m_dMainCtc);
				XLText->AddFormatText("$h$t$t$r$r$r$r$c=$m+04$i%.3f %s$e$n", m_AsMain[nIndex][i], str);
			}
		}
		if(m_dAsUsed[nIndex] > m_dAsReq[nIndex])
		{
			strsub = _T(">");
			strTemp = _T("O.K");
		}
		else
		{
			strsub = _T("<");
			strTemp = _T("N.G");
		}
		XLText->AddFormatText("$n$h$m+01사용철근량$m+03$c:$r$e%.3f %s$m+04$c%s$r$e$m+01필요철근량$m+03$c:$r$e%.3f %s$m+06∴$r%s$n", m_dAsUsed[nIndex], str, strsub,m_dAsReq[nIndex], str, strTemp);
	}

	if(m_bIsInsideBending[nIndex] && pBri->m_bCheckCornerInside)
	{
		CConcBasic *pConcWall = pBri->GetConcDataWall(bStt? 0:pBri->m_nQtyJigan, CHECK_POS_TOP, FALSE);
		CConcBasic *pConcSlab = pBri->GetConcDataSlab(bStt? 0:pBri->m_nQtyJigan-1, bStt? CHECK_POS_LEFT:CHECK_POS_RIGHT);

		double dMCornerInSide = pBri->GetMomentCorner(bStt, TRUE, FALSE, m_bUpperSlab);
		dMCornerInSide = (pData->m_nTypeUnit == 0)? toCm(frTonM(dMCornerInSide)):dMCornerInSide/1000*pow(10.0, 9.0);

		double dn = Round(pConcWall->m_Es/pConcWall->m_Ec, 0);
		int nIndex = bStt? 0 : 1;
		// 수평부재
		double dAs = (pData->m_nTypeUnit == 0)? pConcSlab->GetRebarAreaCompression() : frCm2(pConcSlab->GetRebarAreaCompression());
		double dB = (pData->m_nTypeUnit == 0)? toCm(pConcSlab->m_Bo) : pConcSlab->m_Bo;
		double d0 = (pData->m_nTypeUnit == 0)? toCm(pConcSlab->m_Ho - pConcSlab->m_RbC1C_dc[0] + pConcSlab->m_Hunch/3) : (pConcSlab->m_Ho - pConcSlab->m_RbC1C_dc[0] + pConcSlab->m_Hunch/3);
		double dp = dAs/(dB * d0);
		double dK = -1 * (dn * dp) + sqrt(2 * dn * dp + pow(dn * dp, 2));
		double dJ = 1 - dK/3;
		double dFs = dMCornerInSide / (dAs * d0 * dJ);

		str = (pData->m_nTypeUnit == 0)? _T("㎠"):_T("㎟");

		*XLText += 1;
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$s+10$i$h%s$r내측인장 휨모멘트 보강검토$n$h", GetNumberString(nIndexSub++,NUM_CIRCLE));
		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$h$c-$r수평부재 내측에 배근된 주철근량$m+09$c:$r%.3f$me2$m+03%s$n", dAs, str);
		XLText->AddFormatText("$h$m+01$ck$r$c=$r-np + √(2 x np + (np)²)$m+14[∵ n = %d]$n", (long)dn);
		XLText->AddFormatText("$h$m+02$c=$r - %d x %.7f + √(2 x %.4f + (%.4f)²) = %.3f$n", (long)dn, dp, dn*dp, dn*dp, dK);
		XLText->AddFormatText("$h$m+01$cj$r$c=$r1 - k / 3$m+06$c=$r1 - %.3f /3  = %.3f$n", dK, dJ);
		XLText->AddFormatText("$h$m+01$cfs$r$c=$rMo / (As x d x j)$m+06$c=$r%.3f / (%.3f x %.3f x %.3f)$n", dMCornerInSide, dAs, d0, dJ);
		XLText->AddFormatText("$h$m+02$c=$r%.3f %s$n", dFs, pData->m_szKgf_CM2);
		XLText->AddFormatText("$h$m+01인장력 Th1$m+03$c=$rfs x As$m+02$c=$r%.3f x %.3f$m+05$c=$r%.3f %s$n", dFs, dAs, m_dTh1[nIndex], pData->m_szKgf);
		*XLText += 1;

		// 연직부재
		dAs = (pData->m_nTypeUnit == 0)? pConcWall->GetRebarAreaCompression() : frCm2(pConcWall->GetRebarAreaCompression());
		dB = (pData->m_nTypeUnit == 0)? toCm(pConcWall->m_Bo) : pConcWall->m_Bo;
		d0 = (pData->m_nTypeUnit == 0)? toCm(pConcWall->m_Ho - pConcWall->m_RbC1C_dc[0] + pConcWall->m_Hunch/3) : (pConcWall->m_Ho - pConcWall->m_RbC1C_dc[0] + pConcWall->m_Hunch/3);
		dp = dAs/(dB * d0);
		dK = -1 * (dn * dp) + sqrt(2 * dn * dp + pow(dn * dp, 2));
		dJ = 1 - dK/3;
		dFs = dMCornerInSide / (dAs * d0 * dJ);

		XLText->AddFormatText("$s+08$h$c-$r연직부재 내측에 배근된 주철근량$m+09$c:$r%.3f$me2$m+03%s$n", dAs, str);
		XLText->AddFormatText("$h$m+01$ck$r$c=$r-np + √(2 x np + (np)²)$m+14[∵ n = %d]$n", (long)dn);
		XLText->AddFormatText("$h$m+02$c=$r - %d x %.7f + √(2 x %.4f + (%.4f)²) = %.3f$n", (long)dn, dp, dn*dp, dn*dp, dK);
		XLText->AddFormatText("$h$m+01$cj$r$c=$r1 - k / 3$m+06$c=$r1 - %.3f /3  = %.3f$n", dK, dJ);
		XLText->AddFormatText("$h$m+01$cfs$r$c=$rMo / (As x d x j)$m+06$c=$r%.3f / (%.3f x %.3f x %.3f)$n", dMCornerInSide, dAs, d0, dJ);
		XLText->AddFormatText("$h$m+02$c=$r%.3f %s$n", dFs, pData->m_szKgf_CM2);
		XLText->AddFormatText("$h$m+01인장력 Th2$m+03$c=$rfs x As$m+02$c=$r%.3f x %.3f$m+05$c=$r%.3f %s$n", dFs, dAs, m_dTh2[nIndex], pData->m_szKgf);
		*XLText += 1;

		// 보강철근량 산정
		XLText->AddFormatText("$s+06$h$c-$r보강철근량 산정$n");
		XLText->AddFormatText("$h$m+01$cT$r$c=$r√(Th1² + Th2²) = fsa x As' $m+08T$r$c:$r대각선 방향의 인장력$n");
		XLText->AddFormatText("$h$m+07Th$r$c:$r수평부재 또는 연직부재에 작용하는 인장력$n");
		XLText->AddFormatText("$h$m+01$cAs'$r$c=$r√(Th1² + Th2²) / fsa = √(%.3f² + %.3f²) / %.1f$n", m_dTh1[nIndex], m_dTh2[nIndex], m_dFsa[nIndex]);
		XLText->AddFormatText("$h$m+01$cAs'$r$c=$r%.3f %s$n", m_dAs_[nIndex], str);

		// 단부 스트럽철근 사용 철근량
		long nUD = m_bUpperSlab ? iUPPER : iLOWER;
		double dDiaStrup = pBri->m_dDiaStirrupAtDanBuRebar[nUD][nIndex];		// 단부 스터럽 철근 직경 (시종점)
//		double dCtcStrup = pBri->GetCTCRebarShear_UpperSlab();
		double dCtcStrup = pBri->m_dCTC_MainRebar/2 * pBri->m_nCountStirrupCtcMainRebar[nUD][nIndex];
		long nCntStrup = pBri->m_nCountStirrupAtDanBuRebar[nUD][nIndex];		// 단부 스터럽 철근 개수 (시종점)
		double dAsUse = GetRebarArea_Fix(dDiaStrup, nCntStrup * (frM(UNIT_METER)/dCtcStrup));
		dAsUse = (pData->IsSI())? frCm2(dAsUse) : dAsUse;
		strTemp = (dAsUse > m_dAs_[nIndex])? "O.K" : "N.G";

		XLText->AddFormatText("$n$h$m+01사용철근량$m+03$c:$r%s @ %.0f, %d단$m+05$c=$r%.3f %s$m+05$c∴$r%s$n", pData->GetStringDia(dDiaStrup, pBri->GetValueFy(ePartUpperSlab, TRUE)), dCtcStrup, nCntStrup, dAsUse, str, strTemp);
	}
}

void CARcBridgeCalcCornerAndSupport::WriteSupportCheck(CXLFormatText *XLText, long nIndexMiddle, long nIndexSmall)
{
	long nIndexSub = 1;

	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	CString szTitle = _T("슬래브");
	if(pBri->IsBoxType())
	{
		szTitle = _T("상부슬래브");
	}
	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$e$h%s$r%s$n$n$h", GetNumberString(nIndexSub++,NUM_CIRCLE), szTitle);
	WriteSupportCheckSlab(XLText, TRUE);

	if(pBri->IsBoxType())
	{
		// 선택한게 하나도 없다면 출력을 하지 말자.
		if(pBri->m_bCheckSuppoerBarFoot15hs || m_pStd->m_pBri->m_bCheckSuppoerBarFootMainOver20 || pBri->m_bCheckSuppoerBarFootArea ||pBri->m_bCheckSuppoerBarFoot300)
		{
			szTitle = _T("하부슬래브");
			XLText->GoPosition(NO_MOVE,COL_POS_2);
			XLText->AddFormatText("$e$h%s$r%s$n$n$h", GetNumberString(nIndexSub++,NUM_CIRCLE), szTitle);
			WriteSupportCheckLowerSlab(XLText);
		}
	}

	if(m_pStd->m_pBri->IsOutsideWall())
	{
		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$s+99$e$h%s$r벽체$n$n$h", GetNumberString(nIndexSub++,NUM_CIRCLE));
		WriteSupportCheckWall(XLText,FALSE);
	}

	if(m_pStd->m_pBri->GetCountInWall(TRUE) > 0)
	{
		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$s+99$e$h%s$r중간벽체$n$n$h", GetNumberString(nIndexSub++,NUM_CIRCLE));
		WriteSupportCheckInWall(XLText);
	}

	if(m_pStd->m_pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$s+99$e$h%s$r아치 리브$n$n$h", GetNumberString(nIndexSub++,NUM_CIRCLE));
		WriteSupportCheckArchRib(XLText);
	}

	// 접속슬래브는 접속슬래브에서 출력
	// 기초는 기초 설계에서 출력
}

void CARcBridgeCalcCornerAndSupport::WriteSupportCheckSlab(CXLFormatText *XLText, CConcBasic *pConc, double dLenSpan, BOOL bMain, BOOL bMainOver20, BOOL bSlabArea, BOOL bSlab300)
{
	if(pConc == NULL) return;

	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	BOOL bSI = (pData->m_nTypeUnit == UNIT_TYPE_SI)? TRUE:FALSE;

	double dRatio = 0.;
	double dAreaReq = 0.;
// 	double dAreaUseC = 0.;
// 	double dAreaUseT = 0.;
// 	double dAreaUseTot = 0.;
//	double dAreaUseComp = 0.;
	CString szComp = _T("");
	CString szOkNg = _T("");

	double dAreaUseC = m_pStd->GetRebarAreaSuportBar(pConc, FALSE, TRUE, !bSI);
	double dAreaUseT = m_pStd->GetRebarAreaSuportBar(pConc, TRUE, FALSE, !bSI);
	double dAreaUseTot = dAreaUseC + dAreaUseT;

	XLText->AddFormatText("$h$r$c-$r$e사용철근량$n$h");
	XLText->AddFormatText("$h$r$r$r$e압축부$m+02$c:$r$e");
	long j=0; for(j=0; j<MAX_REBAR_DAN; j++)
	{
		if(pConc->m_CSupportBarDia[j] == 0) continue;
		if(j != 0) XLText->AddFormatText("$n$h$r$r$r$m+02$r");
		XLText->AddFormatText("$e%s @ %.0f", pData->GetStringDia(pConc->m_CSupportBarDia[j], pConc->m_Fy), pConc->m_CSupportBarCTC[j]);
	}
	if(bSI) XLText->AddFormatText("$m+03$c=$r$e$f1%.1f$me1$m+02㎟$n$h", dAreaUseC);
	else XLText->AddFormatText("$m+03$c=$r$e$f3%.3f$me1$m+02㎠$n$h", dAreaUseC);

	XLText->AddFormatText("$h$r$r$r$e인장부$m+02$c:$r$e");
	for(j=0; j<MAX_REBAR_DAN; j++)
	{
		if(pConc->m_TSupportBarDia[j] == 0) continue;
		if(j != 0) XLText->AddFormatText("$n$h$r$r$r$m+02$r");
		XLText->AddFormatText("$e%s @ %.0f", pData->GetStringDia(pConc->m_TSupportBarDia[j], pConc->m_Fy), pConc->m_TSupportBarCTC[j]);
	}
	if(bSI) XLText->AddFormatText("$m+03$c=$r$e$f1%.1f$me1$m+02㎟$n$h", dAreaUseT);
	else XLText->AddFormatText("$m+03$c=$r$e$f3%.3f$me1$m+02㎠$n$h", dAreaUseT);

	XLText->AddFormatText("$u$h$m+06$-07$n$h");
	if(bSI) XLText->AddFormatText("$m+08$c$∑$r$c=$r$e$f1%.1f$me1$m+02㎟$n$h", dAreaUseTot);
	else XLText->AddFormatText("$m+08$c$∑$r$c=$r$e$f3%.3f$me1$m+02㎠$n$h", dAreaUseTot);
	XLText->AddFormatText("$n$h");

	if(bMain)
	{
		CRectBeam slab(pConc);
		double dReqMainRebar = slab.GetRebarAreaRequired();
		double dRatio1 = 55/sqrt(toM(dLenSpan));
		dRatio = MIN(dRatio1/100, 0.5);
		dAreaReq = dReqMainRebar * dRatio;

		szComp = (dRatio > 50)? ">":"<";
		XLText->AddFormatText("$h$r$c-$r$e주철근비(주철근이 차량 진행방향에 평행) [도로교설계기준 4.7.5.7]$n$h");
		XLText->AddFormatText("$h$r$r$e55/√(%.3f)$m+04$c=$r$e%.3f %%$me2$m+03$c%s$r$e 최대 50 %% 중 작은값 이상$n$h", toM(dLenSpan), dRatio1, szComp);
		XLText->AddFormatText("$h$r$r$r$e소요주철근$m+03$c:$r$e");
		if(bSI) XLText->AddFormatText("%.1f ㎟$n$h", dReqMainRebar);
		else XLText->AddFormatText("%.3f ㎠$n$h", dReqMainRebar);
		XLText->AddFormatText("$h$r$r$r$e필요 철근량$m+03$c:$r$e");
		if(bSI) XLText->AddFormatText("%.1f x %.3f = %.1f ㎟", dReqMainRebar, dRatio, dAreaReq);
		else XLText->AddFormatText("%.3f x %.3f = %.3f ㎠", dReqMainRebar, dRatio, dAreaReq);

		szComp = (dAreaReq > dAreaUseT)? ">":"<";
		szOkNg = (dAreaReq > dAreaUseT)? "N.G":"O.K";
		if(bSI) XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$m+03$c∴$r$e", szComp, dAreaUseT);
		else XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$m+03$c∴$r$e", szComp, dAreaUseT);

		if(dAreaReq > dAreaUseT) XLText->AddFormatText("$#03");
		XLText->AddFormatText("$a%s$n$h", szOkNg);
		XLText->AddFormatText("$n$h");
	}

	if(bMainOver20)
	{
		CRectBeam slab(pConc);
		double dReqMainRebar = slab.GetRebarAreaRequired();
		dRatio = 0.2;
		dAreaReq = dReqMainRebar * dRatio;

		XLText->AddFormatText("$h$r$c-$r$e필요 주철근량의 20%% 이상$n$h");
		XLText->AddFormatText("$h$r$r$r$e소요주철근$m+03$c:$r$e");
		if(bSI) XLText->AddFormatText("%.1f ㎟$n$h", dReqMainRebar);
		else XLText->AddFormatText("%.3f ㎠$n$h", dReqMainRebar);
		XLText->AddFormatText("$h$r$r$r$e필요 철근량$m+03$c:$r$e");
		XLText->AddFormatText("%.1f x %.3f = %.1f ㎟", dReqMainRebar, dRatio, dAreaReq);
		
		szComp = (dAreaReq > dAreaUseT)? ">":"<";
		szOkNg = (dAreaReq > dAreaUseT)? "N.G":"O.K";
		if(bSI) XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$m+03$c∴$r$e", szComp, dAreaUseT);
		else XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$m+03$c∴$r$e", szComp, dAreaUseT);

		if(dAreaReq > dAreaUseT) XLText->AddFormatText("$#03");
		XLText->AddFormatText("$a%s$n$h", szOkNg);
		XLText->AddFormatText("$n$h");
	}

	if(bSlabArea)
	{
		double dAreaSection = pConc->m_Ho * pConc->m_Bo;
		dRatio = GetSupportBarSlabRatio(pConc->m_Fy);
		dAreaReq = (bSI)? dAreaSection * dRatio:toCm2(dAreaSection * dRatio);

		if (pData->m_nTypeKciCode == DESIGN_CODE_KCI_2003 || pData->m_nTypeKciCode == DESIGN_CODE_KCI_2007)
		{
			XLText->AddFormatText("$h$r$c-$r$e단면적비(온도 및 건조수축 철근) [콘크리트구조설계기준 해설 P.111]$n$h");
		}
		else
		{
			XLText->AddFormatText("$h$r$c-$r$e단면적비(온도 및 건조수축 철근) [콘크리트구조기준 5.7.2]$n$h");
		}

		XLText->AddFormatText("$h$r$r$e단면적대비 %.4f 이상$n$h", dRatio);
		XLText->AddFormatText("$h$r$r$r$e필요철근량$m+03$c:$r$e%.4f x B x T$n$h$m+06$c=$r$e", dRatio);
		if(bSI) XLText->AddFormatText("%.4f x %.0f x %.0f = %.1f ㎟", dRatio, pConc->m_Bo, pConc->m_Ho, dAreaReq);
		else XLText->AddFormatText("%.4f x %.1f x %.1f = %.3f ㎠", dRatio, toCm(pConc->m_Bo), toCm(pConc->m_Ho), dAreaReq);

		if(pData->m_nTypeKciCode != DESIGN_CODE_KCI_2003)
			dAreaReq = MIN(dAreaReq, (bSI ? 1800 : 18));
		szComp = (dAreaReq > dAreaUseTot)? ">":"<";
		szOkNg = (dAreaReq > dAreaUseTot)? "N.G":"O.K";

		if(pData->m_nTypeKciCode != DESIGN_CODE_KCI_2003 && dAreaSection * dRatio >= 1800)	// Kci 2007
		{
			if(bSI)
			{
				XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$n", szComp, dAreaUseTot);
				XLText->AddFormatText("$h$r$r$e필요철근량이 1800㎟보다 클경우 수축,온도 철근 단면적을 단위 m당$n");
				XLText->AddFormatText("$h$r$r$e1800㎟보다 크게 취할 필요가 없다. [콘크리트구조설계기준 5.7.2]$m+19$c∴$r$e%s$n$h", szOkNg);
			}
			else
			{
				XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$n", szComp, dAreaUseTot);
				XLText->AddFormatText("$h$r$r$e필요철근량이 18㎠보다 클경우 수축,온도 철근 단면적을 단위 m당$n");
				XLText->AddFormatText("$h$r$r$e18㎠보다 크게 취할 필요가 없다.  [콘크리트구조설계기준 5.7.2]$m+19$c∴$r$e%s$n$h", szOkNg);
			}
		}
		else
		{
			if(bSI) XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$m+03$c∴$r$e", szComp, dAreaUseTot);
			else XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$m+03$c∴$r$e", szComp, dAreaUseTot);

			if(dAreaReq > dAreaUseTot) XLText->AddFormatText("$#03");
			XLText->AddFormatText("$a%s$n$h", szOkNg);
		}
		XLText->AddFormatText("$n$h");
	}

	if(bSlab300)
	{
		dAreaReq = (bSI)? 300:toCm2(300);

		XLText->AddFormatText("$h$r$c-$r$e노출면 300㎟ 이상 [도로교설계기준 4.3.9, 철도설계기준 P.254]$n$h");
		XLText->AddFormatText("$h$r$r$r$e필요철근량$m+03$c:$r$e");
		if(bSI) XLText->AddFormatText("$f1%.1f ㎟", dAreaReq);
		else XLText->AddFormatText("$f3%.3f ㎠", dAreaReq);

		double dAreaUseComp = MIN(dAreaUseC, dAreaUseT);
		szComp = (dAreaReq > dAreaUseComp)? ">":"<";
		szOkNg = (dAreaReq > dAreaUseComp)? "N.G":"O.K";
		if(bSI) XLText->AddFormatText("$h$m+16$c%s$r$r$e%.1f ㎟$m+03$c∴$r$e", szComp, dAreaUseComp);
		else XLText->AddFormatText("$h$m+16$c%s$r$r$e%.3f ㎠$m+03$c∴$r$e", szComp, dAreaUseComp);

		if(dAreaReq > dAreaUseComp) XLText->AddFormatText("$#03");
		XLText->AddFormatText("$a%s$n$h", szOkNg);
		XLText->AddFormatText("$n$h");
	}
}


void CARcBridgeCalcCornerAndSupport::WriteSupportCheckSlab(CXLFormatText *XLText, BOOL bUpperSlab)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
//	BOOL bSI = (pData->m_nTypeUnit == UNIT_TYPE_SI) ? TRUE:FALSE;

	CConcBasic *pConcData(NULL);

	//double dRatio = 0;
	//double dAreaReq = 0;
	//double dAreaUseC = 0;
	//double dAreaUseT = 0;
	//double dAreaUseTot = 0;
	//double dAreaUseComp = 0;
	CString szComp = _T("");
	CString szOkNg = _T("");

	long nSizeConc = pBri->m_pArrConcSlab[eEnvLimit].GetSize();

	long nIndexConcStt = !bUpperSlab ? nSizeConc/2 : 0;
	long nIndexConcEnd = (pBri->IsBoxType() && bUpperSlab) ? nSizeConc/2 : nSizeConc;

	for(long i=nIndexConcStt; i<nIndexConcEnd; i++)
	{
		pConcData = pBri->m_pArrConcSlab[eEnvLimit].GetAt(i);
		if(pConcData == NULL) continue;

		CString strTitle = pBri->IsBoxType() ? pConcData->m_sTitle.Right(pConcData->m_sTitle.GetLength() - 11) : pConcData->m_sTitle;

		XLText->AddFormatText("$s+20$i$h%s$r%s$n$n$h", GetNumberString(i - nIndexConcStt + 1,NUM_GANA), strTitle);
		XLText->GoPosition(NO_MOVE,COL_POS_2);

		double dLenSpan = 0;
		if(i < 2) dLenSpan = pBri->GetLengthModelSpan(0);
		else if(i > (nIndexConcEnd - nIndexConcStt) - 3) dLenSpan = pBri->GetLengthModelSpan(pBri->GetCountJijum()-2);
		else if(i%2 != 0) dLenSpan = pBri->GetLengthModelSpan((i-1)/2);
		else if(i%2 == 0) dLenSpan = MIN(pBri->GetLengthModelSpan(i/2 - 1), pBri->GetLengthModelSpan(i/2));

		WriteSupportCheckSlab(XLText, pConcData, dLenSpan, bUpperSlab && pBri->m_bCheckSuppoerBarSlabMain, pBri->m_bCheckSuppoerBarSlabMainOver20, pBri->m_bCheckSuppoerBarSlabArea, pBri->m_bCheckSuppoerBarSlab300);
	}
}

void CARcBridgeCalcCornerAndSupport::WriteSupportCheckWall(CXLFormatText *XLText, BOOL bWingWall)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	BOOL bSI = (pData->m_nTypeUnit == UNIT_TYPE_SI)? TRUE:FALSE;
	
	CConcBasic *pConcData;
	long nCnt = MIN((pBri->IsOutsideWall())? 6:0, pBri->m_pArrConcWall[eEnvLimit].GetSize());

	double dRatio = 0;
	double dAreaReq = 0;
	double dAreaReqSect = 0;
	double dAreaUseC = 0;
	double dAreaUseT = 0;
	double dAreaUseTot = 0;
	double dAreaUseComp = 0;
	CString szComp = _T("");
	CString szOkNg = _T("");
	CString szTitle = _T("");

	long i=0; for(i=0; i<nCnt; i++)
	{
		pConcData = pBri->GetConcDataWall(i, FALSE, eEnvLimit, TRUE);
		if(pConcData == NULL) continue;

		dAreaUseC = m_pStd->GetRebarAreaSuportBar(pConcData, FALSE, TRUE, !bSI);
		dAreaUseT = m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, FALSE, !bSI);
		dAreaUseTot = dAreaUseC + dAreaUseT;
		
		XLText->AddFormatText("$s+20$i$h%s$r%s$n$h", GetNumberString(i+1,NUM_GANA), pConcData->m_sTitle);
		XLText->GoPosition(NO_MOVE,COL_POS_2);

		XLText->AddFormatText("$h$r$c-$r$e사용철근량$n$h");
		XLText->AddFormatText("$h$r$r$r$e압축부$m+02$c:$r$e");
		long j=0; for(j=0; j<MAX_REBAR_DAN; j++)
		{
			if(pConcData->m_CSupportBarDia[j] == 0) continue;
			if(j != 0) XLText->AddFormatText("$n$h$r$r$r$m+02$r");
			XLText->AddFormatText("$e%s @ %.0f", pData->GetStringDia(pConcData->m_CSupportBarDia[j], pConcData->m_Fy), pConcData->m_CSupportBarCTC[j]);
		}
		if(bSI) XLText->AddFormatText("$m+03$c=$r$e$f1%.1f$me1$m+02㎟$n$h", dAreaUseC);
		else XLText->AddFormatText("$m+03$c=$r$e$f3%.3f$me1$m+02㎠$n$h", dAreaUseC);
		
		XLText->AddFormatText("$h$r$r$r$e인장부$m+02$c:$r$e");
		for(j=0; j<MAX_REBAR_DAN; j++)
		{
			if(pConcData->m_TSupportBarDia[j] == 0) continue;
			if(j != 0) XLText->AddFormatText("$n$h$r$r$r$m+02$r");
			XLText->AddFormatText("$e%s @ %.0f", pData->GetStringDia(pConcData->m_TSupportBarDia[j], pConcData->m_Fy), pConcData->m_TSupportBarCTC[j]);
		}
		if(bSI) XLText->AddFormatText("$m+03$c=$r$e$f1%.1f$me1$m+02㎟$n$h", dAreaUseT);
		else XLText->AddFormatText("$m+03$c=$r$e$f3%.3f$me1$m+02㎠$n$h", dAreaUseT);
		
		XLText->AddFormatText("$u$h$m+06$-07$n$h");
		if(bSI) XLText->AddFormatText("$m+08$c$∑$r$c=$r$e$f1%.1f$me1$m+02㎟$n$h", dAreaUseTot);
		else XLText->AddFormatText("$m+08$c$∑$r$c=$r$e$f3%.3f$me1$m+02㎠$n$h", dAreaUseTot);
		XLText->AddFormatText("$n$h");
		
		if(pBri->m_bCheckSuppoerBarWall15hs)
		{
			double dMaxH = pData->IsDoro2010() ? 1800 : 1200;
			double dThick = MIN(dMaxH, pConcData->m_Ho);
			double dB = pConcData->m_Bo;
			double dReqOrg = (bSI) ? 0.0015 * dThick * dB: toCm2(0.0015 * dThick * dB);

			XLText->AddFormatText("$h$r$c-$r$e0.0015hb  [도로교설계기준 4.3.9]$n$h");
			if(pData->IsDoro2010())
				XLText->AddFormatText("$h$r$r$eh : 부재두께$m+04b : 부재 폭$n$h");
			else
				XLText->AddFormatText("$h$r$r$eh : 부재두께 최대 %.0f$m+07b : 부재 폭$n$h", dMaxH);


			XLText->AddFormatText("$h$r$r$r$e필요 철근량$m+03$c:$r$e");

			if(pData->m_nTypeKciCode != DESIGN_CODE_KCI_2003)	// Kci 2007
			{
				if(bSI) XLText->AddFormatText("0.0015 x %.0f x %.0f$m+07$c=$r$e %.1f ㎟  (≤1800)$n$h", dThick, dB, dReqOrg);
				else XLText->AddFormatText("0.0015 x %.1f x %.1f$m+07$c=$e %.3f ㎠  (≤1800)$n$h", toCm(dThick), toCm(dB), dReqOrg);
				
				dAreaReq = MIN(dReqOrg, (bSI ? 1800 : 18));
				szComp = (dAreaReq > dAreaUseTot)? ">":"<";
				szOkNg = (dAreaReq > dAreaUseTot)? "N.G":"O.K";

				if(0.0015*dThick*1000>1800 && pData->IsDoro2010())
				{
					XLText->AddFormatText(_T("$h$r$r$e필요철근량이 1800㎟보다 클경우 수축,온도 철근 단면적을 단위 m당$n"));//zzzz줄맞추기
					XLText->AddFormatText(_T("$h$r$r$e1800㎟보다 크게 취할 필요가 없다. [콘크리트구조설계기준 5.7.2]$n$h"));
				}

				if(bSI) 
					XLText->AddFormatText("$h$m+12$c∴$r %.1f ㎟$m+03$c%s$r$r$e$f1%.1f ㎟$m+03$c∴$r$e", dAreaReq, szComp, dAreaUseTot);
				else 
					XLText->AddFormatText("$h$m+12$c∴$r %.3f ㎠$m+03$c%s$r$r$e$f3%.3f ㎠$m+03$c∴$r$e", dAreaReq, szComp, dAreaUseTot);
			}
			else
			{
				if(bSI) XLText->AddFormatText("0.0015 x %.0f x %.0f = %.1f ㎟", dThick, dB, dAreaReq);
				else XLText->AddFormatText("0.0015 x %.1f x %.1f = %.3f ㎠", toCm(dThick), toCm(dB), dAreaReq);
				
				szComp = (dAreaReq > dAreaUseTot)? ">":"<";
				szOkNg = (dAreaReq > dAreaUseTot)? "N.G":"O.K";
				if(bSI) 
					XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$m+03$c∴$r$e", szComp, dAreaUseTot);
				else 
					XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$m+03$c∴$r$e", szComp, dAreaUseTot);
			}
			
			if(dAreaReq > dAreaUseTot) XLText->AddFormatText("$#03");
			XLText->AddFormatText("$a%s$n$h", szOkNg);
			XLText->AddFormatText("$n$h");
		}

		if(pBri->m_bCheckSuppoerBarWallMainOver20)
		{
			CRectBeam slab(pConcData);
			double dReqMainRebar = slab.GetRebarAreaRequired();
			dRatio = 0.2;
			dAreaReq = dReqMainRebar * dRatio;

			XLText->AddFormatText("$h$r$c-$r$e필요 주철근량의 20%% 이상$n$h");
			XLText->AddFormatText("$h$r$r$r$e소요주철근$m+03$c:$r$e");
			if(bSI) XLText->AddFormatText("%.1f ㎟$n$h", dReqMainRebar);
			else XLText->AddFormatText("%.3f ㎠$n$h", dReqMainRebar);
			XLText->AddFormatText("$h$r$r$r$e필요 철근량$m+03$c:$r$e");
			XLText->AddFormatText("%.1f x %.3f = %.1f ㎟", dReqMainRebar, dRatio, dAreaReq);

			szComp = (dAreaReq > dAreaUseT)? ">":"<";
			szOkNg = (dAreaReq > dAreaUseT)? "N.G":"O.K";
			if(bSI) XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$m+03$c∴$r$e", szComp, dAreaUseT);
			else XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$m+03$c∴$r$e", szComp, dAreaUseT);

			if(dAreaReq > dAreaUseT) XLText->AddFormatText("$#03");
			XLText->AddFormatText("$a%s$n$h", szOkNg);
			XLText->AddFormatText("$n$h");
		}
		
		if(pBri->m_bCheckSuppoerBarWallArea)
		{
			double dAreaSection = pConcData->m_Ho * pConcData->m_Bo;
			dRatio = bWingWall ? pBri->m_dCheckSuppoerBarWingArea : pBri->m_dCheckSuppoerBarWallArea;
			if(!bWingWall && pConcData->m_Fy >= 4000 && pConcData->m_TSupportBarDia[0] <= 16.0)
			{
				dRatio = pBri->m_dCheckSuppoerBarWallAreaForOver400;
			}

			dAreaReq = (bSI)? dAreaSection * dRatio:toCm2(dAreaSection * dRatio);
			dAreaReqSect = dAreaReq;

			if (pData->m_nTypeKciCode == DESIGN_CODE_KCI_2003 || pData->m_nTypeKciCode == DESIGN_CODE_KCI_2007)
			{
				XLText->AddFormatText("$h$r$c-$r$e단면적비(온도 및 건조수축 철근) [콘크리트구조설계기준 P.287]$n$h");
			}
			else
			{
				XLText->AddFormatText("$h$r$c-$r$e단면적비(온도 및 건조수축 철근) [콘크리트구조기준 11.3(2)]$n$h");
			}
			XLText->AddFormatText("$h$r$r$e단면적대비 %.4f 이상$n$h", dRatio);
			XLText->AddFormatText("$h$r$r$r$e필요철근량$m+03$c:$r$e%.4f x B x T$n$h$m+06$c=$r$e", dRatio);
			if(bSI) XLText->AddFormatText("%.4f x %.0f x %.0f = %.1f ㎟", dRatio, pConcData->m_Bo, pConcData->m_Ho, dAreaReq);
			else XLText->AddFormatText("%.4f x %.1f x %.1f = %.3f ㎠", dRatio, toCm(pConcData->m_Bo), toCm(pConcData->m_Ho), dAreaReq);

			if(pData->m_nTypeKciCode != DESIGN_CODE_KCI_2003)
				dAreaReq = MIN(dAreaReq, (bSI ? 1800 : 18));
			szComp = (dAreaReq > dAreaUseTot)? ">":"<";
			szOkNg = (dAreaReq > dAreaUseTot)? "N.G":"O.K";
			if(pData->m_nTypeKciCode != DESIGN_CODE_KCI_2003 && dAreaSection * dRatio >= 1800)	// Kci 2007
			{
				if(bSI)
				{
					XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$n", szComp, dAreaUseTot);
					XLText->AddFormatText("$h$r$r$e필요철근량이 1800㎟보다 클경우 수축,온도 철근 단면적을 단위 m당$n");
					XLText->AddFormatText("$h$r$r$e1800㎟보다 크게 취할 필요가 없다. [콘크리트구조설계기준 5.7.2]$m+19$c∴$r$e%s$n$h", szOkNg);
				}
				else
				{
					XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$n", szComp, dAreaUseTot);
					XLText->AddFormatText("$h$r$r$e필요철근량이 18㎠보다 클경우 수축,온도 철근 단면적을 단위 m당$n");
					XLText->AddFormatText("$h$r$r$e18㎠보다 크게 취할 필요가 없다. [콘크리트구조설계기준 5.7.2]$m+19$c∴$r$e%s$n$h", szOkNg);
				}
			}
			else
			{
				if(bSI) XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$m+03$c∴$r$e", szComp, dAreaUseTot);
				else XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$m+03$c∴$r$e", szComp, dAreaUseTot);
				
				if(dAreaReq > dAreaUseTot) XLText->AddFormatText("$#03");
				XLText->AddFormatText("$a%s$n$h", szOkNg);
			}
			XLText->AddFormatText("$n$h");
		}
		
		if(pBri->m_bCheckSuppoerBarWall300)
		{
			dAreaReq = (bSI)? 300:toCm2(300);
			
			XLText->AddFormatText("$h$r$c-$r$e노출면 300㎟ 이상 [도로교설계기준 4.3.9, 철도설계기준 P.254]$n$h");
			XLText->AddFormatText("$h$r$r$r$e필요철근량$m+03$c:$r$e");
			if(bSI) XLText->AddFormatText("$f1%.1f ㎟", dAreaReq);
			else XLText->AddFormatText("$f3%.3f ㎠", dAreaReq);

			dAreaUseComp = (pConcData->m_sTitle.Find("중앙") != -1 || pConcData->m_sTitle.Find("중간") != -1)? dAreaUseT:dAreaUseC;
			szComp = (dAreaReq > dAreaUseComp)? ">":"<";
			szOkNg = (dAreaReq > dAreaUseComp)? "N.G":"O.K";
			if(bSI) XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$m+03$c∴$r$e", szComp, dAreaUseComp);
			else XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$m+03$c∴$r$e", szComp, dAreaUseComp);
			
			if(dAreaReq > dAreaUseComp) XLText->AddFormatText("$#03");
			XLText->AddFormatText("$a%s$n$h", szOkNg);
			XLText->AddFormatText("$n$h");
		}

		if(pBri->m_bCheckSuppoerBarWallArea && pBri->m_bSelectSupportBarWall)
		{
			dAreaUseComp = (pConcData->m_sTitle.Find("중앙") != -1 || pConcData->m_sTitle.Find("중간") != -1)? dAreaUseT:dAreaUseC;

			XLText->AddFormatText("$h$r$c-$r$e노출면 배치 철근량 검토$n$h");
			XLText->AddFormatText("$h$r$r$e노출면에 1/2이상, 2/3이하 배치$n$h");

			if(bSI) XLText->AddFormatText("$h$m+02$c:$r$e%.1f / 2 = %.1f ㎟$m+06", dAreaReqSect, dAreaReqSect/2);
			else XLText->AddFormatText("$h$m+02$c:$r$e%.3f / 2 = %.3f ㎠$m+06", dAreaReqSect, dAreaReqSect/2);
			szComp = (dAreaReqSect/2 < dAreaUseComp)? "<":">";
			if(bSI) XLText->AddFormatText("$c%s$r$e%.1f ㎟$m+03", szComp, dAreaUseComp);
			else XLText->AddFormatText("$c%s$r$e%.3f ㎠$m+03", szComp, dAreaUseComp);
			szComp = (dAreaReqSect*2/3 > dAreaUseComp)? "<":">";
			if(bSI) XLText->AddFormatText("$c%s$r$e%.1f x 2/3 = %.1f ㎟$m+07$c∴$r$e", szComp, dAreaReqSect, dAreaReqSect*2/3);
			else XLText->AddFormatText("$c%s$r$e%.3f x 2/3 = %.3f ㎠$m+07$c∴$r$e", szComp, dAreaReqSect, dAreaReqSect*2/3);

			szOkNg = (dAreaReqSect/2 < dAreaUseComp && dAreaReqSect*2/3 > dAreaUseComp)? "O.K":"N.G";
			if(szOkNg == "N.G") XLText->AddFormatText("$#03");
			XLText->AddFormatText("$a%s$n$h", szOkNg);
			XLText->AddFormatText("$n$h");
		}
	}
	XLText->AddFormatText("$n$h");
}

void CARcBridgeCalcCornerAndSupport::WriteSupportCheckInWall(CXLFormatText *XLText)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	BOOL bSI = (pData->m_nTypeUnit == UNIT_TYPE_SI)? TRUE:FALSE;

	CConcBasic *pConcData;
	double dRatio = 0;
	double dAreaReq = 0;
	double dAreaReqSect = 0;
	double dAreaUseC = 0;
	double dAreaUseT = 0;
	double dAreaUseTot = 0;
	double dAreaUseComp = 0;
	CString szComp = _T("");
	CString szOkNg = _T("");
	CString szTitle = _T("");

	long nCntOut = 0;
	long nSizeList = m_pStd->m_pColGir->m_pList.GetSize();
	long i=0; 
	for(i=0; i<nSizeList; i++)
	{
		CGirderAndColumn *pGirCol = m_pStd->m_pColGir->m_pList.GetAt(i);
		if(pGirCol->m_nJijum == 0 || pGirCol->m_nJijum == nSizeList - 1) continue; //#ARCBRIDGE-3568
		if(pGirCol->m_bWall) nCntOut++;
	}
	if(pBri->m_nSelectInWallSectionDesign == 0)
	{
		nCntOut = pBri->m_pArrConcInWall[eEnvLimit].GetSize();
	}
	if(nCntOut == 0) return;

	long nIndexOut = 0;
	for(i = 0; i<nCntOut; i++)
	{
		CGirderAndColumn *pGirCol = NULL;
		if(pBri->m_nSelectInWallSectionDesign == 0)
		{
			pConcData = pBri->m_pArrConcInWall[eEnvLimit].GetAt(i);
		}
		else
		{
			long nIndex = 0;
			for(long ix = 0; ix<nSizeList; ix++)
			{
				pGirCol = m_pStd->m_pColGir->m_pList.GetAt(ix);
				if(pGirCol->m_nJijum == 0 || pGirCol->m_nJijum == nSizeList - 1) continue; //#ARCBRIDGE-3568
				if(!pGirCol->m_bWall) continue;
				if(nIndex == i)
				{
					pConcData = pGirCol->pConcColumn[0];
					break;
				}
				nIndex++;
			}
		}
		if(pConcData == NULL) continue;


		dAreaUseC = m_pStd->GetRebarAreaSuportBar(pConcData, FALSE, TRUE, !bSI);
		dAreaUseT = m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, FALSE, !bSI);
		dAreaUseTot = dAreaUseC + dAreaUseT;
		
		XLText->AddFormatText("$s+20$i$h%s$r%s$n$h", GetNumberString(nIndexOut+1,NUM_GANA), pConcData->m_sTitle);
		XLText->GoPosition(NO_MOVE,COL_POS_2);

		XLText->AddFormatText("$h$r$c-$r$e사용철근량$n$h");
		XLText->AddFormatText("$h$r$r$r$e압축부$m+02$c:$r$e");
		long j=0; for(j=0; j<MAX_REBAR_DAN; j++)
		{
			if(pConcData->m_CSupportBarDia[j] == 0) continue;
			if(j != 0) XLText->AddFormatText("$n$h$r$r$r$m+02$r");
			XLText->AddFormatText("$e%s @ %.0f", pData->GetStringDia(pConcData->m_CSupportBarDia[j], pConcData->m_Fy), pConcData->m_CSupportBarCTC[j]);
		}
		if(bSI) XLText->AddFormatText("$m+03$c=$r$e$f1%.1f$me1$m+02㎟$n$h", dAreaUseC);
		else XLText->AddFormatText("$m+03$c=$r$e$f3%.3f$me1$m+02㎠$n$h", dAreaUseC);
		
		XLText->AddFormatText("$h$r$r$r$e인장부$m+02$c:$r$e");
		for(j=0; j<MAX_REBAR_DAN; j++)
		{
			if(pConcData->m_TSupportBarDia[j] == 0) continue;
			if(j != 0) XLText->AddFormatText("$n$h$r$r$r$m+02$r");
			XLText->AddFormatText("$e%s @ %.0f", pData->GetStringDia(pConcData->m_TSupportBarDia[j], pConcData->m_Fy), pConcData->m_TSupportBarCTC[j]);
		}
		if(bSI) XLText->AddFormatText("$m+03$c=$r$e$f1%.1f$me1$m+02㎟$n$h", dAreaUseT);
		else XLText->AddFormatText("$m+03$c=$r$e$f3%.3f$me1$m+02㎠$n$h", dAreaUseT);
		
		XLText->AddFormatText("$u$h$m+06$-07$n$h");
		if(bSI) XLText->AddFormatText("$m+08$c$∑$r$c=$r$e$f1%.1f$me1$m+02㎟$n$h", dAreaUseTot);
		else XLText->AddFormatText("$m+08$c$∑$r$c=$r$e$f3%.3f$me1$m+02㎠$n$h", dAreaUseTot);
		XLText->AddFormatText("$n$h");
		
		if(pBri->m_bCheckSuppoerBarWall15hs)
		{
			double dMaxH = pData->IsDoro2010() ? 1800 : 1200;
			double dThick = MIN(dMaxH, pConcData->m_Ho);
			double dB = pConcData->m_Bo;
			dAreaReq = (bSI)? 0.0015 * dThick * dB:toCm2(0.0015 * dThick * dB);

			XLText->AddFormatText("$h$r$c-$r$e0.0015hb  [도로교설계기준 4.3.9]$n$h");
			if(pData->IsDoro2010())
				XLText->AddFormatText("$h$r$r$eh : 부재두께$m+04b : 부재 폭$n$h");
			else
				XLText->AddFormatText("$h$r$r$eh : 부재두께 최대 %.0f$m+07b : 부재 폭$n$h", dMaxH);

			XLText->AddFormatText("$h$r$r$r$e필요 철근량$m+03$c:$r$e");

			if(pData->m_nTypeKciCode != DESIGN_CODE_KCI_2003)	// Kci 2007
			{
				if(bSI) XLText->AddFormatText("0.0015 x %.0f x %.0f$m+07$c=$r$e %.1f ㎟  (≤1800)$n$h", dThick, dB, dAreaReq);
				else XLText->AddFormatText("0.0015 x %.1f x %.1f$m+07$c=$e %.3f ㎠  (≤1800)$n$h", toCm(dThick), toCm(dB), dAreaReq);
				
				if(0.0015*dThick*1000>1800 && pData->IsDoro2010())
				{
					XLText->AddFormatText(_T("$h$r$r$e필요철근량이 1800㎟보다 클경우 수축,온도 철근 단면적을 단위 m당$n"));//zzzz줄맞추기
					XLText->AddFormatText(_T("$h$r$r$e1800㎟보다 크게 취할 필요가 없다. [콘크리트구조설계기준 5.7.2]$n$h"));
				}

				dAreaReq = MIN(dAreaReq, (bSI? 1800 : 18));
				szComp = (dAreaReq > dAreaUseTot)? ">":"<";
				szOkNg = (dAreaReq > dAreaUseTot)? "N.G":"O.K";
				if(bSI) XLText->AddFormatText("$h$m+12$c∴$r %.1f ㎟$m+03$c%s$r$r$e$f1%.1f ㎟$m+03$c∴$r$e", dAreaReq, szComp, dAreaUseTot);
				else XLText->AddFormatText("$h$m+12$c∴$r %.3f ㎠$m+03$c%s$r$r$e$f3%.3f ㎠$m+03$c∴$r$e", dAreaReq, szComp, dAreaUseTot);
			}
			else
			{
				if(bSI) XLText->AddFormatText("0.0015 x %.0f x %.0f = %.1f ㎟", dThick, dB, dAreaReq);
				else XLText->AddFormatText("0.0015 x %.1f x %.1f = %.3f ㎠", toCm(dThick), toCm(dB), dAreaReq);
				
				szComp = (dAreaReq > dAreaUseTot)? ">":"<";
				szOkNg = (dAreaReq > dAreaUseTot)? "N.G":"O.K";
				if(bSI) XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$m+03$c∴$r$e", szComp, dAreaUseTot);
				else XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$m+03$c∴$r$e", szComp, dAreaUseTot);
			}
			
			if(dAreaReq > dAreaUseTot) XLText->AddFormatText("$#03");
			XLText->AddFormatText("$a%s$n$h", szOkNg);
			XLText->AddFormatText("$n$h");
		}

		if(pBri->m_bCheckSuppoerBarWallMainOver20)
		{
			CRectBeam slab(pConcData);
			double dReqMainRebar = slab.GetRebarAreaRequired();
			dRatio = 0.2;
			dAreaReq = dReqMainRebar * dRatio;

			XLText->AddFormatText("$h$r$c-$r$e필요 주철근량의 20%% 이상$n$h");
			XLText->AddFormatText("$h$r$r$r$e소요주철근$m+03$c:$r$e");
			if(bSI) XLText->AddFormatText("%.1f ㎟$n$h", dReqMainRebar);
			else XLText->AddFormatText("%.3f ㎠$n$h", dReqMainRebar);
			XLText->AddFormatText("$h$r$r$r$e필요 철근량$m+03$c:$r$e");
			XLText->AddFormatText("%.1f x %.3f = %.1f ㎟", dReqMainRebar, dRatio, dAreaReq);

			szComp = (dAreaReq > dAreaUseT)? ">":"<";
			szOkNg = (dAreaReq > dAreaUseT)? "N.G":"O.K";
			if(bSI) XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$m+03$c∴$r$e", szComp, dAreaUseT);
			else XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$m+03$c∴$r$e", szComp, dAreaUseT);

			if(dAreaReq > dAreaUseT) XLText->AddFormatText("$#03");
			XLText->AddFormatText("$a%s$n$h", szOkNg);
			XLText->AddFormatText("$n$h");
		}
		
		if(pBri->m_bCheckSuppoerBarWallArea)
		{
			double dAreaSection = pConcData->m_Ho * pConcData->m_Bo;
			dRatio = pBri->m_dCheckSuppoerBarWallArea;
			if(pConcData->m_Fy >= 4000 && pConcData->m_TSupportBarDia[0] <= 16.0)
			{
				dRatio = pBri->m_dCheckSuppoerBarWallAreaForOver400;
			}

			dAreaReq = (bSI)? dAreaSection * dRatio:toCm2(dAreaSection * dRatio);
			dAreaReqSect = dAreaReq;

			if (pData->m_nTypeKciCode == DESIGN_CODE_KCI_2003 || pData->m_nTypeKciCode == DESIGN_CODE_KCI_2007)
			{
				XLText->AddFormatText("$h$r$c-$r$e단면적비(온도 및 건조수축 철근) [콘크리트구조설계기준 P.287]$n$h");
			}
			else
			{
				XLText->AddFormatText("$h$r$c-$r$e단면적비(온도 및 건조수축 철근) [콘크리트구조기준 11.3(2)]$n$h");
			}
			XLText->AddFormatText("$h$r$r$e단면적대비 %.4f 이상$n$h", dRatio);
			XLText->AddFormatText("$h$r$r$r$e필요철근량$m+03$c:$r$e%.4f x B x T$n$h$m+06$c=$r$e", dRatio);
			if(bSI) XLText->AddFormatText("%.4f x %.0f x %.0f = %.1f ㎟", dRatio, pConcData->m_Bo, pConcData->m_Ho, dAreaReq);
			else XLText->AddFormatText("%.4f x %.1f x %.1f = %.3f ㎠", dRatio, toCm(pConcData->m_Bo), toCm(pConcData->m_Ho), dAreaReq);

			if(pData->m_nTypeKciCode != DESIGN_CODE_KCI_2003)
				dAreaReq = MIN(dAreaReq, (bSI ? 1800 : 18));
			szComp = (dAreaReq > dAreaUseTot)? ">":"<";
			szOkNg = (dAreaReq > dAreaUseTot)? "N.G":"O.K";
			if(pData->m_nTypeKciCode != DESIGN_CODE_KCI_2003 && dAreaSection * dRatio >= 1800)	// Kci 2007
			{
				if(bSI)
				{
					XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$n", szComp, dAreaUseTot);
					XLText->AddFormatText("$h$r$r$e필요철근량이 1800㎟보다 클경우 수축,온도 철근 단면적을 단위 m당$n");
					XLText->AddFormatText("$h$r$r$e1800㎟보다 크게 취할 필요가 없다. [콘크리트구조설계기준 5.7.2]$m+19$c∴$r$eO.K$n$h");
				}
				else
				{
					XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$n", szComp, dAreaUseTot);
					XLText->AddFormatText("$h$r$r$e필요철근량이 18㎠보다 클경우 수축,온도 철근 단면적을 단위 m당$n");
					XLText->AddFormatText("$h$r$r$e18㎠보다 크게 취할 필요가 없다. [콘크리트구조설계기준 5.7.2]$m+19$c∴$r$eO.K$n$h");
				}
			}
			else
			{
				if(bSI) XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$m+03$c∴$r$e", szComp, dAreaUseTot);
				else XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$m+03$c∴$r$e", szComp, dAreaUseTot);
				
				if(dAreaReq > dAreaUseTot) XLText->AddFormatText("$#03");
				XLText->AddFormatText("$a%s$n$h", szOkNg);
			}
			XLText->AddFormatText("$n$h");
		}
		
		if(pBri->m_bCheckSuppoerBarWall300)
		{
			dAreaReq = (bSI)? 300:toCm2(300);
			
			XLText->AddFormatText("$h$r$c-$r$e노출면 300㎟ 이상 [도로교설계기준 4.3.9, 철도설계기준 P.254]$n$h");
			XLText->AddFormatText("$h$r$r$r$e필요철근량$m+03$c:$r$e");
			if(bSI) XLText->AddFormatText("$f1%.1f ㎟", dAreaReq);
			else XLText->AddFormatText("$f3%.3f ㎠", dAreaReq);

			dAreaUseComp = (pConcData->m_sTitle.Find("중앙") != -1 || pConcData->m_sTitle.Find("중간") != -1)? dAreaUseT:dAreaUseC;
			szComp = (dAreaReq > dAreaUseComp)? ">":"<";
			szOkNg = (dAreaReq > dAreaUseComp)? "N.G":"O.K";
			if(bSI) XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$m+03$c∴$r$e", szComp, dAreaUseComp);
			else XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$m+03$c∴$r$e", szComp, dAreaUseComp);
			
			if(dAreaReq > dAreaUseComp) XLText->AddFormatText("$#03");
			XLText->AddFormatText("$a%s$n$h", szOkNg);
			XLText->AddFormatText("$n$h");
		}

		if(pBri->m_bCheckSuppoerBarWallArea && pBri->m_bSelectSupportBarWall)
		{
			dAreaUseComp = (pConcData->m_sTitle.Find("중앙") != -1 || pConcData->m_sTitle.Find("중간") != -1)? dAreaUseT:dAreaUseC;

			XLText->AddFormatText("$h$r$c-$r$e노출면 배치 철근량 검토$n$h");
			XLText->AddFormatText("$h$r$r$e노출면에 1/2이상, 2/3이하 배치$n$h");

			if(bSI) XLText->AddFormatText("$h$m+02$c:$r$e%.1f / 2 = %.1f ㎟$m+06", dAreaReqSect, dAreaReqSect/2);
			else XLText->AddFormatText("$h$m+02$c:$r$e%.3f / 2 = %.3f ㎠$m+06", dAreaReqSect, dAreaReqSect/2);
			szComp = (dAreaReqSect/2 < dAreaUseComp)? "<":">";
			if(bSI) XLText->AddFormatText("$c%s$r$e%.1f ㎟$m+03", szComp, dAreaUseComp);
			else XLText->AddFormatText("$c%s$r$e%.3f ㎠$m+03", szComp, dAreaUseComp);
			szComp = (dAreaReqSect*2/3 > dAreaUseComp)? "<":">";
			if(bSI) XLText->AddFormatText("$c%s$r$e%.1f x 2/3 = %.1f ㎟$m+07$c∴$r$e", szComp, dAreaReqSect, dAreaReqSect*2/3);
			else XLText->AddFormatText("$c%s$r$e%.3f x 2/3 = %.3f ㎠$m+07$c∴$r$e", szComp, dAreaReqSect, dAreaReqSect*2/3);

			szOkNg = (dAreaReqSect/2 < dAreaUseComp && dAreaReqSect*2/3 > dAreaUseComp)? "O.K":"N.G";
			if(szOkNg == "N.G") XLText->AddFormatText("$#03");
			XLText->AddFormatText("$a%s$n$h", szOkNg);
			XLText->AddFormatText("$n$h");
			XLText->AddFormatText("$n$h");
		}
		nIndexOut++;
	}
	XLText->AddFormatText("$n$h");
}

void CARcBridgeCalcCornerAndSupport::WriteSupportCheckLowerSlab(CXLFormatText *XLText)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	BOOL bSI = (pData->m_nTypeUnit == UNIT_TYPE_SI)? TRUE:FALSE;
	
	CConcBasic *pConcData;
	long nSizeConc = pBri->m_pArrConcSlab[eEnvLimit].GetSize();

	long nIndexConcStt = nSizeConc/2;
	long nIndexConcEnd = nSizeConc;

	double dRatio = 0;
	double dAreaReq = 0;
	double dAreaUseC = 0;
	double dAreaUseT = 0;
	double dAreaUseTot = 0;
	double dAreaUseComp = 0;
	CString szComp = _T("");
	CString szOkNg = _T("");

	long nIndex = 0;
	for(long i=nIndexConcStt; i<nIndexConcEnd; i++)
	{
		pConcData = pBri->m_pArrConcSlab[eEnvLimit].GetAt(i);
		if(pConcData == NULL) continue;

		nIndex++;
		dAreaUseC = m_pStd->GetRebarAreaSuportBar(pConcData, FALSE, TRUE, !bSI);
		dAreaUseT = m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, FALSE, !bSI);
		dAreaUseTot = dAreaUseC + dAreaUseT;
		
		XLText->AddFormatText("$s+20$i$h%s$r%s$n$h", GetNumberString(nIndex,NUM_GANA), pConcData->m_sTitle);
		XLText->GoPosition(NO_MOVE,COL_POS_2);

		XLText->AddFormatText("$h$r$c-$r$e사용철근량$n$h");
		XLText->AddFormatText("$h$r$r$r$e압축부$m+02$c:$r$e");
		long j=0; for(j=0; j<MAX_REBAR_DAN; j++)
		{
			if(pConcData->m_CSupportBarDia[j] == 0) continue;
			if(j != 0) XLText->AddFormatText("$n$h$r$r$r$m+02$r");
			XLText->AddFormatText("$e%s @ %.0f", pData->GetStringDia(pConcData->m_CSupportBarDia[j], pConcData->m_Fy), pConcData->m_CSupportBarCTC[j]);
		}
		if(bSI) XLText->AddFormatText("$m+03$c=$r$e$f1%.1f$me1$m+02㎟$n$h", dAreaUseC);
		else XLText->AddFormatText("$m+03$c=$r$e$f3%.3f$me1$m+02㎠$n$h", dAreaUseC);
		
		XLText->AddFormatText("$h$r$r$r$e인장부$m+02$c:$r$e");
		for(j=0; j<MAX_REBAR_DAN; j++)
		{
			if(pConcData->m_TSupportBarDia[j] == 0) continue;
			if(j != 0) XLText->AddFormatText("$n$h$r$r$r$m+02$r");
			XLText->AddFormatText("$e%s @ %.0f", pData->GetStringDia(pConcData->m_TSupportBarDia[j], pConcData->m_Fy), pConcData->m_TSupportBarCTC[j]);
		}
		if(bSI) XLText->AddFormatText("$m+03$c=$r$e$f1%.1f$me1$m+02㎟$n$h", dAreaUseT);
		else XLText->AddFormatText("$m+03$c=$r$e$f3%.3f$me1$m+02㎠$n$h", dAreaUseT);
		
		XLText->AddFormatText("$u$h$m+06$-07$n$h");
		if(bSI) XLText->AddFormatText("$m+08$c$∑$r$c=$r$e$f1%.1f$me1$m+02㎟$n$h", dAreaUseTot);
		else XLText->AddFormatText("$m+08$c$∑$r$c=$r$e$f3%.3f$me1$m+02㎠$n$h", dAreaUseTot);
		XLText->AddFormatText("$n$h");
		
		if(pBri->m_bCheckSuppoerBarFoot15hs)
		{
			double dMaxH = pData->IsDoro2010() ? 1800 : 1200;
			double dThick = MIN(dMaxH, pConcData->m_Ho);
			double dB = pConcData->m_Bo;
			dAreaReq = (bSI) ? 0.0015 * dThick * dB:toCm2(0.0015 * dThick * dB);

			XLText->AddFormatText("$h$r$c-$r$e0.0015hb  [도로교설계기준 4.3.9]$n$h");
			if(pData->IsDoro2010())
				XLText->AddFormatText("$h$r$r$eh : 부재두께$m+04b : 부재 폭$n$h");
			else
				XLText->AddFormatText("$h$r$r$eh : 부재두께 최대 %.0f$m+07b : 부재 폭$n$h", dMaxH);

			XLText->AddFormatText("$h$r$r$r$e필요 철근량$m+03$c:$r$e");

			if(pData->m_nTypeKciCode != 0)	// Kci 2007
			{
				if(bSI) XLText->AddFormatText("0.0015 x %.0f x %.0f$m+07$c=$r$e %.1f ㎟  (≤1800)$n$h", dThick, dB, dAreaReq);
				else XLText->AddFormatText("0.0015 x %.1f x %.1f$m+07$c=$e %.3f ㎠  (≤1800)$n$h", toCm(dThick), toCm(dB), dAreaReq);
				
				dAreaReq = MIN(dAreaReq, (bSI? 1800 : 18));
				szComp = (dAreaReq > dAreaUseTot)? ">":"<";
				szOkNg = (dAreaReq > dAreaUseTot)? "N.G":"O.K";
				if(bSI) XLText->AddFormatText("$h$m+12$c∴$r %.1f ㎟$m+03$c%s$r$r$e$f1%.1f ㎟$m+03$c∴$r$e", dAreaReq, szComp, dAreaUseTot);
				else XLText->AddFormatText("$h$m+12$c∴$r %.3f ㎠$m+03$c%s$r$r$e$f3%.3f ㎠$m+03$c∴$r$e", dAreaReq, szComp, dAreaUseTot);
			}
			else
			{
				if(bSI) XLText->AddFormatText("0.0015 x %.0f x %.0f = %.1f ㎟", dThick, dB, dAreaReq);
				else XLText->AddFormatText("0.0015 x %.1f x %.1f = %.3f ㎠", toCm(dThick), toCm(dB), dAreaReq);
				
				szComp = (dAreaReq > dAreaUseTot)? ">":"<";
				szOkNg = (dAreaReq > dAreaUseTot)? "N.G":"O.K";
				if(bSI) XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$m+03$c∴$r$e", szComp, dAreaUseTot);
				else XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$m+03$c∴$r$e", szComp, dAreaUseTot);
			}
			
			if(dAreaReq > dAreaUseTot) XLText->AddFormatText("$#03");
			XLText->AddFormatText("$a%s$n$h", szOkNg);
			XLText->AddFormatText("$n$h");
		}

		if(m_pStd->m_pBri->m_bCheckSuppoerBarFootMainOver20)
		{
			CRectBeam slab(pConcData);
			double dReqMainRebar = slab.GetRebarAreaRequired();
			dRatio = 0.2;
			dAreaReq = dReqMainRebar * dRatio;

			XLText->AddFormatText("$h$r$c-$r$e필요 주철근량의 20%% 이상$n$h");
			XLText->AddFormatText("$h$r$r$r$e소요주철근$m+03$c:$r$e");
			if(bSI) XLText->AddFormatText("%.1f ㎟$n$h", dReqMainRebar);
			else XLText->AddFormatText("%.3f ㎠$n$h", dReqMainRebar);
			XLText->AddFormatText("$h$r$r$r$e필요 철근량$m+03$c:$r$e");
			XLText->AddFormatText("%.1f x %.3f = %.1f ㎟", dReqMainRebar, dRatio, dAreaReq);

			szComp = (dAreaReq > dAreaUseT)? ">":"<";
			szOkNg = (dAreaReq > dAreaUseT)? "N.G":"O.K";
			if(bSI) XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$m+03$c∴$r$e", szComp, dAreaUseT);
			else XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$m+03$c∴$r$e", szComp, dAreaUseT);

			if(dAreaReq > dAreaUseT) XLText->AddFormatText("$#03");
			XLText->AddFormatText("$a%s$n$h", szOkNg);
			XLText->AddFormatText("$n$h");
		}
		
		if(pBri->m_bCheckSuppoerBarFootArea)
		{
			double dAreaSection = pConcData->m_Ho * pConcData->m_Bo;
			dRatio = GetSupportBarFootRatio(pConcData->m_Fy);
			dAreaReq = (bSI)? dAreaSection * dRatio:toCm2(dAreaSection * dRatio);

			if (pData->m_nTypeKciCode == 0 || pData->m_nTypeKciCode == 1)
			{
				XLText->AddFormatText("$h$r$c-$r$e단면적비(온도 및 건조수축 철근) [콘크리트구조설계기준 해설 P.111]$n$h");
			}
			else
			{
				XLText->AddFormatText("$h$r$c-$r$e단면적비(온도 및 건조수축 철근) [콘크리트구조기준 5.7.2]$n$h");
			}

			XLText->AddFormatText("$h$r$r$e단면적대비 %.4f 이상$n$h", dRatio);
			XLText->AddFormatText("$h$r$r$r$e필요철근량$m+03$c:$r$e%.4f x B x T$n$h$m+06$c=$r$e", dRatio);
			if(bSI) XLText->AddFormatText("%.4f x %.0f x %.0f = %.1f ㎟", dRatio, pConcData->m_Bo, pConcData->m_Ho, dAreaReq);
			else XLText->AddFormatText("%.4f x %.1f x %.1f = %.3f ㎠", dRatio, toCm(pConcData->m_Bo), toCm(pConcData->m_Ho), dAreaReq);

			if(pData->m_nTypeKciCode != 0)
				dAreaReq = MIN(dAreaReq, (bSI ? 1800 : 18));
			szComp = (dAreaReq > dAreaUseTot)? ">":"<";
			szOkNg = (dAreaReq > dAreaUseTot)? "N.G":"O.K";
			if(pData->m_nTypeKciCode != 0 && dAreaSection * dRatio >= 1800)	// Kci 2007
			{
				if(bSI)
				{
					XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$n", szComp, dAreaUseTot);
					XLText->AddFormatText("$h$r$r$e필요철근량이 1800㎟보다 클경우 수축,온도 철근 단면적을 단위 m당$n");
					XLText->AddFormatText("$h$r$r$e1800㎟보다 크게 취할 필요가 없다. [콘크리트구조설계기준 5.7.2]$m+19$c∴$r$e%s$n$h", szOkNg);
				}
				else
				{
					XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$n", szComp, dAreaUseTot);
					XLText->AddFormatText("$h$r$r$e필요철근량이 18㎠보다 클경우 수축,온도 철근 단면적을 단위 m당$n");
					XLText->AddFormatText("$h$r$r$e18㎠보다 크게 취할 필요가 없다. [콘크리트구조설계기준 5.7.2]$m+19$c∴$r$e%s$n$h", szOkNg);
				}
			}
			else
			{
				if(bSI) XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$m+03$c∴$r$e", szComp, dAreaUseTot);
				else XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$m+03$c∴$r$e", szComp, dAreaUseTot);
				
				if(dAreaReq > dAreaUseTot) XLText->AddFormatText("$#03");
				XLText->AddFormatText("$a%s$n$h", szOkNg);
			}
			XLText->AddFormatText("$n$h");
		}
		
		if(pBri->m_bCheckSuppoerBarFoot300)
		{
			dAreaReq = (bSI)? 300:toCm2(300);
			
			XLText->AddFormatText("$h$r$c-$r$e노출면 300㎟ 이상 [도로교설계기준 4.3.9, 철도설계기준 P.254]$n$h");
			XLText->AddFormatText("$h$r$r$r$e필요철근량$m+03$c:$r$e");
			if(bSI) XLText->AddFormatText("$f1%.1f ㎟", dAreaReq);
			else XLText->AddFormatText("$f3%.3f ㎠", dAreaReq);

			dAreaUseComp = MIN(dAreaUseC, dAreaUseT);
			szComp = (dAreaReq > dAreaUseComp)? ">":"<";
			szOkNg = (dAreaReq > dAreaUseComp)? "N.G":"O.K";
			if(bSI) XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$m+03$c∴$r$e", szComp, dAreaUseComp);
			else XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$m+03$c∴$r$e", szComp, dAreaUseComp);
			
			if(dAreaReq > dAreaUseComp) XLText->AddFormatText("$#03");
			XLText->AddFormatText("$a%s$n$h", szOkNg);
			XLText->AddFormatText("$n$h");
		}
	}
	XLText->AddFormatText("$n$h");
}

void CARcBridgeCalcCornerAndSupport::DrawCorner(CDomyun *pDomP, BOOL bStt)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CDomyun Dom(pDomP);
	
	Dom.SetScaleDim(50);
	Dom.SetTextHeight(10);

	CString strMark = _T("");
	CDPoint xyStt(0, 0);
	CDPoint xyTemp(0, 0);

	long iDir = (bStt) ? 1 : -1;
	long nIndex = (bStt) ? 0 : 1;

	double dTS = (pData->m_nTypeUnit == 0) ? frCm(m_dTs[nIndex]) : frM(m_dTs[nIndex]);
	double dTW = (pData->m_nTypeUnit == 0) ? frCm(m_dTw[nIndex])*iDir : frM(m_dTw[nIndex])*iDir;
	double dHH = (pData->m_nTypeUnit == 0) ? frCm(m_dHH[nIndex]) : frM(m_dHH[nIndex]);
	double dHW = (pData->m_nTypeUnit == 0) ? frCm(m_dHW[nIndex])*iDir : frM(m_dHW[nIndex])*iDir;
	double dA  = (pData->m_nTypeUnit == 0) ? frCm(m_dA[nIndex]) : frM(m_dA[nIndex]);
	double dB  = (pData->m_nTypeUnit == 0) ? frCm(m_dB[nIndex])*iDir : frM(m_dB[nIndex])*iDir;
	double dR  = (pData->m_nTypeUnit == 0) ? frCm(m_dR[nIndex]) : frM(m_dR[nIndex]);
	
	double dLenX = dTW + dHW + 500*iDir;
	double dLenY = dTS + dHH + 500;
	double sw = m_bUpperSlab ? 1.0 : -1.0;

	// 외곽선
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	
	xyTemp.x = xyStt.x + dLenX;
	xyTemp.y = xyStt.y;
	Dom.MoveTo(xyTemp.x, xyTemp.y);
	xyTemp.x -= dLenX;
	Dom.LineTo(xyTemp.x, xyTemp.y);
	xyTemp.y -= dLenY*sw; 
	Dom.LineTo(xyTemp.x, xyTemp.y);
	xyTemp.x += dTW;
	Dom.MoveTo(xyTemp.x, xyTemp.y);
	xyTemp.y += (dLenY-dTS-dHH)*sw;
	Dom.LineTo(xyTemp.x, xyTemp.y);
	xyTemp.x += dHW;
	xyTemp.y += dHH*sw;
	Dom.LineTo(xyTemp.x, xyTemp.y);
	xyTemp.x += (dLenX - dTW - dHW);
	Dom.LineTo(xyTemp.x, xyTemp.y);

	// 절단선
	m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_CENTER_LINE);
	xyTemp.x = xyStt.x + dLenX;
	xyTemp.y = xyStt.y + 200*sw;
	Dom.MoveTo(xyTemp.x, xyTemp.y);
	xyTemp.y -= (dTS+400)*sw;
	Dom.LineTo(xyTemp.x, xyTemp.y);
	xyTemp.x = xyStt.x - 200*iDir;
	xyTemp.y = xyStt.y - dLenY*sw;
	Dom.MoveTo(xyTemp.x, xyTemp.y);
	xyTemp.x += (dTW+400*iDir);
	Dom.LineTo(xyTemp.x, xyTemp.y);

	// 치수
	m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	Dom.SetTextHeight(3);
	Dom.SetDimVLen(6);
	Dom.SetDimExo(4);

	xyTemp = xyStt;
	xyTemp.y -= dLenY*sw;
	Dom.SetDirection(m_bUpperSlab ? "BOTTOM" : "TOP");
	Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, "");
	Dom.DimLineTo(dTW, 0, COMMA(fabs(dTW)));
	Dom.DimLineTo(dHW, 0, COMMA(fabs(dHW)));

	strMark = (bStt)? _T("RIGHT"):_T("LEFT");
	xyTemp = xyStt;
	xyTemp.x += dLenX;
	Dom.SetDirection(strMark);
	Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, "");
	Dom.DimLineTo(-dTS*sw, 0, COMMA(fabs(dTS)));
	Dom.DimLineTo(-dHH*sw, 0, COMMA(fabs(dHH)));

	Dom.SetDimVLen(5);
	Dom.SetDimExo(1);
	strMark = (bStt)? "LEFT":"RIGHT";
	Dom.SetDirection(strMark);
	xyTemp = xyStt;
	Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, "");
	xyTemp.y -= dA*sw;
	Dom.DimLineToExtend(-dA*sw, 0, "a", COMMA(dA));

	Dom.SetDirection(!m_bUpperSlab ? "BOTTOM" : "TOP");
	xyTemp = xyStt;
	Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, "");
	xyTemp.x += dB;
	Dom.DimLineToExtend(dB, 0, "b", COMMA(fabs(dB)));
	*pDomP << Dom;

	Dom.SetDirection("BOTTOM");
	Dom.SetDimVLen(3);
	xyTemp = xyStt;
	Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, "");
	xyTemp.x += dR;
	Dom.DimLineToExtend(dR*iDir, 0, "R", COMMA(fabs(dR)));
	Dom.Rotate(xyStt, CDPoint(dB*iDir, -dA*sw*iDir));

	// 중심선 R  점선 a,b
	m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_CENTER_LINE);
	pDomP->SetLTScale(200);
	xyTemp = xyStt;
	Dom.MoveTo(xyTemp.x, xyTemp.y);
	xyTemp.x += dB;
	xyTemp.y -= dA*sw;
	Dom.LineTo(xyTemp.x, xyTemp.y);
	m_pStd->m_pARoadOptionStd->SetEnvLType(&Dom, LT_DASH_LINE);
	xyTemp = xyStt;
	xyTemp.x += dB;
	Dom.MoveTo(xyTemp.x, xyTemp.y);
	xyTemp.y -= dA*sw;
	Dom.LineTo(xyTemp.x, xyTemp.y);
	xyTemp.x -= dB;
	Dom.LineTo(xyTemp.x, xyTemp.y);

	*pDomP << Dom;
}

void CARcBridgeCalcCornerAndSupport::MakeTableSupportSumSlab(CXLFormatText *XLText, long nIndexSub)
{
	CHgBaseConcStd HgConcStd;
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pDataStd = pBri->m_pARcBridgeDataStd;

//	CConcBasic *pConcData;
	CStringArray szArrTitle1;
	CStringArray szArrTitle2;
	CDoubleArray dArrWidth1;
	CDoubleArray dArrWidth2;

	XLText->GoPosition(NO_MOVE,COL_POS_2);

	szArrTitle1.RemoveAll();
	szArrTitle2.RemoveAll();
	dArrWidth1.RemoveAll();
	dArrWidth2.RemoveAll();

	szArrTitle1.Add("구 분");
	szArrTitle2.Add("구 분");
	int nColCnt = 0;
	if(pBri->m_bCheckSuppoerBarSlabMain)	nColCnt++;
	if(pBri->m_bCheckSuppoerBarSlabMainOver20)	nColCnt++;
	if(pBri->m_bCheckSuppoerBarSlabArea)	nColCnt++;
	if(pBri->m_bCheckSuppoerBarSlab300)		nColCnt++;

	if(pBri->m_bCheckSuppoerBarSlabMain)
	{
		szArrTitle1.Add("주철근비");
		szArrTitle2.Add(nColCnt > 3 ? "필요" : "필요철근량");
		szArrTitle2.Add(nColCnt > 3 ? "사용" : "사용철근량");
	}
	if(pBri->m_bCheckSuppoerBarSlabMainOver20)
	{
		szArrTitle1.Add("주철근의 20%");
		szArrTitle2.Add(nColCnt > 3 ? "필요" : "필요철근량");
		szArrTitle2.Add(nColCnt > 3 ? "사용" : "사용철근량");
	}
	if(pBri->m_bCheckSuppoerBarSlabArea)
	{
		szArrTitle1.Add("단면적비");
		szArrTitle2.Add(nColCnt > 3 ? "필요" : "필요철근량");
		szArrTitle2.Add(nColCnt > 3 ? "사용" : "사용철근량");
	}
	if(pBri->m_bCheckSuppoerBarSlab300)
	{
		szArrTitle1.Add("300㎟이상");
		szArrTitle2.Add(nColCnt > 3 ? "필요" : "필요철근량");
		szArrTitle2.Add(nColCnt > 3 ? "사용" : "사용철근량");
	}
	szArrTitle1.Add("비 고");
	szArrTitle2.Add("비 고");

	if(nColCnt == 0) return;
	else if(nColCnt == 1)
	{
		dArrWidth1.Add(6);
		dArrWidth1.Add(12);
		dArrWidth1.Add(6);

		dArrWidth2.Add(6);
		dArrWidth2.Add(6);
		dArrWidth2.Add(6);
		dArrWidth2.Add(6);
	}
	else if(nColCnt == 2)
	{
		dArrWidth1.Add(4);
		dArrWidth1.Add(8);
		dArrWidth1.Add(8);
		dArrWidth1.Add(4);
		
		dArrWidth2.Add(4);
		dArrWidth2.Add(4);
		dArrWidth2.Add(4);
		dArrWidth2.Add(4);
		dArrWidth2.Add(4);
		dArrWidth2.Add(4);
	}
	else if(nColCnt == 3)
	{
		dArrWidth1.Add(4);
		dArrWidth1.Add(6);
		dArrWidth1.Add(6);
		dArrWidth1.Add(6);
		dArrWidth1.Add(2);
		
		dArrWidth2.Add(4);
		dArrWidth2.Add(3);
		dArrWidth2.Add(3);
		dArrWidth2.Add(3);
		dArrWidth2.Add(3);
		dArrWidth2.Add(3);
		dArrWidth2.Add(3);
		dArrWidth2.Add(2);
	}
	else if(nColCnt == 4)
	{
		dArrWidth1.Add(4);
		dArrWidth1.Add(4);
		dArrWidth1.Add(4);
		dArrWidth1.Add(4);
		dArrWidth1.Add(4);
		dArrWidth1.Add(4);

		dArrWidth2.Add(4);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(4);
	}

	
	pBri->SetConcDataSlab();
	long nSizeConc = pBri->m_pArrConcSlab[eEnvLimit].GetSize();
	int nCntRow = pBri->IsBoxType() ? 2 + nSizeConc/2 : 2 + nSizeConc;
	long ColWidth = 0;

	// (ARCBRIDGE-1455) 하부슬래브는 검토옵션을 기초부 옵션으로 적용해줘야 하기 때문에 따로 출력한다.
	// for(long iUD = iUPPER; iUD <= iLOWER; iUD++)

	long nIndex[4] = { 0, 0, 0, 0 };
//	CString szTitle = _T("");
	CString szTitle = _T("슬래브");
	if(pBri->IsBoxType())
	{
		szTitle = _T("상부슬래브");
	}
	XLText->AddFormatText("$s+03$h$c%s$r%s$m+18$i$me1단위$m+02$c:$r$e%s$n$h", GetNumberString(nIndexSub, NUM_CIRCLE), szTitle, pDataStd->m_nTypeUnit == UNIT_TYPE_SI ? "㎟" : "㎠");
	XLText->AddTable(nCntRow-1, 24-1, 2, 1);

	long nIdxBase = 0;
	for(long ix=0; ix<4; ++ix)
	{
		BOOL bCheck = FALSE;
		switch (ix)
		{
		case 0:
			bCheck = pBri->m_bCheckSuppoerBarSlabMain;	break;
		case 1:
			bCheck = pBri->m_bCheckSuppoerBarSlabMainOver20;	break;
		case 2:
			bCheck = pBri->m_bCheckSuppoerBarSlabArea;	break;
		case 3:
			bCheck = TRUE;	break;
		}
		if(bCheck)
		{
			nIndex[nIdxBase] = ix;
			++nIdxBase;
		}
	}

	// Title
	long nCntMerge = 0;
	long i=0; for(i=0; i<nColCnt+2; i++)
	{
		ColWidth = (long)dArrWidth1.GetAt(i);
		szTitle = szArrTitle1.GetAt(i);
		XLText->AddMergeFormatText(nCntMerge, ColWidth-1,"$c%s", szTitle);
		if(i==0 || i==nColCnt+1) XLText->AddFormatText("$md1");
		XLText->AddFormatText("$m+%02d", ColWidth);
	}
	XLText->AddText("$n$h");
	for(i=0; i<nColCnt*2+2; i++)
	{
		ColWidth = (long)dArrWidth2.GetAt(i);
		szTitle = szArrTitle2.GetAt(i);
		XLText->AddMergeFormatText(nCntMerge, ColWidth-1,"$c%s", szTitle);
		XLText->AddFormatText("$m+%02d", ColWidth);
	}
	XLText->AddText("$n$h");

	// Data
	CString str = _T("");
	long nIndexConcStt = 0;
	long nIndexConcEnd = pBri->IsBoxType()? nSizeConc/2 : nSizeConc;
	for(i = nIndexConcStt; i < nIndexConcEnd; i++)
	{
		double dLenSpan = 0;

		if(i < 2) dLenSpan = pBri->GetLengthModelSpan(0);
		else if(i > nCntRow-5) dLenSpan = pBri->GetLengthModelSpan(pBri->GetCountJijum()-2);
		else if(i%2 != 0) dLenSpan = pBri->GetLengthModelSpan((i-1)/2);
		else if(i%2 == 0) dLenSpan = MAX(pBri->GetLengthModelSpan(i/2 - 1), pBri->GetLengthModelSpan(i/2));

		CConcBasic *pConcData = pBri->m_pArrConcSlab[eEnvLimit].GetAt(i);
		if(pConcData == NULL) continue;

		long j=0; for(j=0; j<nColCnt*2+2; j++)
		{
			ColWidth = (long)dArrWidth2.GetAt(j);

			if(j == 0) 
			{
				str = pBri->IsBoxType() ? pConcData->m_sTitle.Right(pConcData->m_sTitle.GetLength() - 11) : pConcData->m_sTitle;
			}
			else if(j == nColCnt*2+1) str = GetStringCheckConcSlab(pConcData, TRUE, 4, dLenSpan);
			else if(j == 1) str = GetStringCheckConcSlab(pConcData, TRUE, nIndex[0], dLenSpan);
			else if(j == 2) str = GetStringCheckConcSlab(pConcData, FALSE, nIndex[0], dLenSpan);
			else if(j == 3) str = GetStringCheckConcSlab(pConcData, TRUE, nIndex[1], dLenSpan);
			else if(j == 4) str = GetStringCheckConcSlab(pConcData, FALSE, nIndex[1], dLenSpan);
			else if(j == 5) str = GetStringCheckConcSlab(pConcData, TRUE, nIndex[2], dLenSpan);
			else if(j == 6) str = GetStringCheckConcSlab(pConcData, FALSE, nIndex[2], dLenSpan);
			else if(j == 7) str = GetStringCheckConcSlab(pConcData, TRUE, nIndex[3], dLenSpan);
			else if(j == 8) str = GetStringCheckConcSlab(pConcData, FALSE, nIndex[3], dLenSpan);
			else str = _T("");

			XLText->AddMergeFormatText(nCntMerge, ColWidth-1,"$c%s", str);
			XLText->AddFormatText("$m+%02d", ColWidth);
		}
		XLText->AddText("$n$h");
	}
	XLText->AddText("$n$h");
}

void CARcBridgeCalcCornerAndSupport::MakeTableSupportSumWall(CXLFormatText *XLText, long nIndexSub, BOOL bArchRib)
{
	CHgBaseConcStd HgConcStd;
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pDataStd = pBri->m_pARcBridgeDataStd;

	CConcBasic *pConcData;
	CStringArray szArrTitle1;
	CStringArray szArrTitle2;
	CDoubleArray dArrWidth1;
	CDoubleArray dArrWidth2;

	XLText->GoPosition(NO_MOVE,COL_POS_2);

	szArrTitle1.RemoveAll();
	szArrTitle2.RemoveAll();
	dArrWidth1.RemoveAll();
	dArrWidth2.RemoveAll();

	szArrTitle1.Add("구 분");
	szArrTitle2.Add("구 분");
	szArrTitle2.Add("");
	int nColCnt = 0;
	if(pBri->m_bCheckSuppoerBarWall15hs)	nColCnt++;
	if(pBri->m_bCheckSuppoerBarWallMainOver20)	nColCnt++;
	if(pBri->m_bCheckSuppoerBarWallArea)	nColCnt++;
	if(pBri->m_bCheckSuppoerBarWall300)		nColCnt++;

	if(pBri->m_bCheckSuppoerBarWall15hs)
	{
		szArrTitle1.Add("0.0015hb");
		szArrTitle2.Add(nColCnt > 3 ? "필요" : "필요철근량");
		szArrTitle2.Add(nColCnt > 3 ? "사용" : "사용철근량");
	}
	if(pBri->m_bCheckSuppoerBarWallMainOver20)
	{
		szArrTitle1.Add("주철근의 20%");
		szArrTitle2.Add(nColCnt > 3 ? "필요" : "필요철근량");
		szArrTitle2.Add(nColCnt > 3 ? "사용" : "사용철근량");
	}
	if(pBri->m_bCheckSuppoerBarWallArea)
	{
		szArrTitle1.Add("단면적비");
		szArrTitle2.Add(nColCnt > 3 ? "필요" : "필요철근량");
		szArrTitle2.Add(nColCnt > 3 ? "사용" : "사용철근량");
	}
	if(pBri->m_bCheckSuppoerBarWall300)
	{
		szArrTitle1.Add("300㎟이상");
		szArrTitle2.Add(nColCnt > 3 ? "필요" : "필요철근량");
		szArrTitle2.Add(nColCnt > 3 ? "사용" : "사용철근량");
	}
	szArrTitle1.Add("비 고");
	szArrTitle2.Add("비 고");

	if(nColCnt == 0) return;
	else if(nColCnt == 1)
	{
		dArrWidth1.Add(6);
		dArrWidth1.Add(12);
		dArrWidth1.Add(6);

		dArrWidth2.Add(3);
		dArrWidth2.Add(3);
		dArrWidth2.Add(6);
		dArrWidth2.Add(6);
		dArrWidth2.Add(6);
	}
	else if(nColCnt == 2)
	{
		dArrWidth1.Add(4);
		dArrWidth1.Add(8);
		dArrWidth1.Add(8);
		dArrWidth1.Add(4);
		
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(4);
		dArrWidth2.Add(4);
		dArrWidth2.Add(4);
		dArrWidth2.Add(4);
		dArrWidth2.Add(4);
	}
	else if(nColCnt == 3)
	{
		dArrWidth1.Add(4);
		dArrWidth1.Add(6);
		dArrWidth1.Add(6);
		dArrWidth1.Add(6);
		dArrWidth1.Add(2);
		
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(3);
		dArrWidth2.Add(3);
		dArrWidth2.Add(3);
		dArrWidth2.Add(3);
		dArrWidth2.Add(3);
		dArrWidth2.Add(3);
		dArrWidth2.Add(2);
	}
	else if(nColCnt == 4)
	{
		dArrWidth1.Add(4);
		dArrWidth1.Add(4);
		dArrWidth1.Add(4);
		dArrWidth1.Add(4);
		dArrWidth1.Add(4);
		dArrWidth1.Add(4);

		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(4);
	}

//	pBri->SetConcDataWall();
	long nCntConcWall = bArchRib? pBri->m_pArrConcArchRib.GetSize() : MIN((pBri->IsOutsideWall())? 6:0, pBri->m_pArrConcWall[eEnvLimit].GetSize());
	int nCntRow = 2 + nCntConcWall;
	long ColWidth = 0;

	long nIndex[4] = { 0, 0, 0, 0 };
//	CString szTitle = _T("");
	CString szTitle = (pDataStd->m_nTypeUnit == UNIT_TYPE_SI)? "㎟":"㎠";
	XLText->AddFormatText("$s+03$h$c%s$r%s$m+18$i$me1단위$m+02$c:$r$e%s$n$h", GetNumberString(nIndexSub,NUM_CIRCLE), bArchRib? "아치리브" : "벽체", szTitle);
	XLText->AddTable(nCntRow-1, 24-1, 2, 4);
	
	long nIdxBase = 0;
	for(long ix=0; ix<4; ++ix)
	{
		BOOL bCheck = FALSE;
		switch (ix)
		{
		case 0:
			bCheck = pBri->m_bCheckSuppoerBarWall15hs;	break;
		case 1:
			bCheck = pBri->m_bCheckSuppoerBarWallMainOver20;	break;
		case 2:
			bCheck = pBri->m_bCheckSuppoerBarWallArea;	break;
		case 3:
			bCheck = TRUE;	break;
		}
		if(bCheck)
		{
			nIndex[nIdxBase] = ix;
			++nIdxBase;
		}
	}
	
	// Title
	long nCntMerge = 0;
	long i=0; for(i=0; i<nColCnt+2; i++)
	{
		ColWidth = (long)dArrWidth1.GetAt(i);
		szTitle = szArrTitle1.GetAt(i);
		XLText->AddMergeFormatText(nCntMerge, ColWidth-1,"$c%s", szTitle);
		if(i==0 || i==nColCnt+1) XLText->AddFormatText("$md1");
		XLText->AddFormatText("$m+%02d", ColWidth);
	}
	XLText->AddText("$n$h");
	for(i=0; i<nColCnt*2+2; i++)
	{
		ColWidth = (long)dArrWidth2.GetAt(i);
		szTitle = szArrTitle2.GetAt(i);
		XLText->AddMergeFormatText(nCntMerge, ColWidth-1,"$c%s", szTitle);
		XLText->AddFormatText("$m+%02d", ColWidth);
	}
	XLText->AddText("$n$h");

	// Data
	BOOL bIsOutWall = (pBri->IsOutsideWall());
	CString str = _T("");
	for(i = 0; i<nCntRow-2; i++)
	{
		pConcData = bArchRib? pBri->m_pArrConcArchRib.GetAt(i) : pBri->GetConcDataWall(i, FALSE, eEnvLimit, TRUE);
		if(pConcData == NULL) continue;

		long j=0; for(j=0; j<nColCnt*2+3; j++)
		{
			ColWidth = (long)dArrWidth2.GetAt(j);
			
			if(j == 0)
			{
				if(bArchRib)
				{
					continue;
				}
				else
				{
					if(i<3 && bIsOutWall)
					{
						str = "시점벽체";
						if(i == 0) XLText->AddFormatText("$md2");
					}
					else if(i<6 && bIsOutWall)
					{
						str = "종점벽체";
						if(i == 3) XLText->AddFormatText("$md2");
					}
					else
					{
						str = "중간벽체";
						if((i == 6 && bIsOutWall) || (i == 0 && !bIsOutWall))XLText->AddFormatText("$md%d", pBri->GetCountInWall()-1);
					}
				}
			}
			else if(j == 1)
			{
				CString strTemp = _T("");
				if(!bArchRib)
				{
					strTemp = pConcData->m_sTitle.Mid(9);
				}
				else
				{
					ColWidth += (long)dArrWidth2.GetAt(0);
					strTemp = pConcData->m_sTitle;
				}
				
				if(strTemp.Right(1) == ")") str = strTemp.Left(4) + "부";
				else str = strTemp;
			}
			else if(j == nColCnt*2+2) str = GetStringCheckConcWall(pConcData, TRUE, 4);
			else if(j == 2) str = GetStringCheckConcWall(pConcData, TRUE, nIndex[0]);
			else if(j == 3) str = GetStringCheckConcWall(pConcData, FALSE, nIndex[0]);
			else if(j == 4) str = GetStringCheckConcWall(pConcData, TRUE, nIndex[1]);
			else if(j == 5) str = GetStringCheckConcWall(pConcData, FALSE, nIndex[1]);
			else if(j == 6) str = GetStringCheckConcWall(pConcData, TRUE, nIndex[2]);
			else if(j == 7) str = GetStringCheckConcWall(pConcData, FALSE, nIndex[2]);
			else if(j == 8) str = GetStringCheckConcWall(pConcData, TRUE, nIndex[3]);
			else if(j == 9) str = GetStringCheckConcWall(pConcData, FALSE, nIndex[3]);
			else str = _T("");
			XLText->AddMergeFormatText(nCntMerge, ColWidth-1,"$c%s", str);
			XLText->AddFormatText("$m+%02d", ColWidth);
		}
		XLText->AddText("$n$h");
	}
	XLText->AddText("$n$h");
}

void CARcBridgeCalcCornerAndSupport::MakeTableSupportSumInWall(CXLFormatText *XLText, long nIndexSub)
{
	CHgBaseConcStd HgConcStd;
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pDataStd = pBri->m_pARcBridgeDataStd;

	long nCntOut = 0;
	long nSizeList = m_pStd->m_pColGir->m_pList.GetSize();
	long i=0; 
	for(i=0; i<nSizeList; i++)
	{
		CGirderAndColumn *pGirCol = m_pStd->m_pColGir->m_pList.GetAt(i);
		if(pGirCol->m_nJijum == 0 || pGirCol->m_nJijum == nSizeList - 1) continue;	// #ARCBRIDGE-3568 시점과 종점은 따로 계산되므로
		if(pGirCol->m_bWall) nCntOut++;
	}
	if(pBri->m_nSelectInWallSectionDesign == 0)
	{
		nCntOut = pBri->m_pArrConcInWall[eEnvLimit].GetSize();
	}
	if(nCntOut == 0) return;

	CConcBasic *pConcData;
	CStringArray szArrTitle1;
	CStringArray szArrTitle2;
	CDoubleArray dArrWidth1;
	CDoubleArray dArrWidth2;

	XLText->GoPosition(NO_MOVE,COL_POS_2);

	szArrTitle1.RemoveAll();
	szArrTitle2.RemoveAll();
	dArrWidth1.RemoveAll();
	dArrWidth2.RemoveAll();

	szArrTitle1.Add("구 분");
	szArrTitle2.Add("구 분");
	szArrTitle2.Add("");
	int nColCnt = 0;
	if(pBri->m_bCheckSuppoerBarWall15hs)	nColCnt++;
	if(pBri->m_bCheckSuppoerBarWallMainOver20)	nColCnt++;
	if(pBri->m_bCheckSuppoerBarWallArea)	nColCnt++;
	if(pBri->m_bCheckSuppoerBarWall300)		nColCnt++;

	if(pBri->m_bCheckSuppoerBarWall15hs)
	{
		szArrTitle1.Add("0.0015hb");
		szArrTitle2.Add(nColCnt > 3 ? "필요" : "필요철근량");
		szArrTitle2.Add(nColCnt > 3 ? "사용" : "사용철근량");
	}
	if(pBri->m_bCheckSuppoerBarWallMainOver20)
	{
		szArrTitle1.Add("주철근의 20%");
		szArrTitle2.Add(nColCnt > 3 ? "필요" : "필요철근량");
		szArrTitle2.Add(nColCnt > 3 ? "사용" : "사용철근량");
	}
	if(pBri->m_bCheckSuppoerBarWallArea)
	{
		szArrTitle1.Add("단면적비");
		szArrTitle2.Add(nColCnt > 3 ? "필요" : "필요철근량");
		szArrTitle2.Add(nColCnt > 3 ? "사용" : "사용철근량");
	}
	if(pBri->m_bCheckSuppoerBarWall300)
	{
		szArrTitle1.Add("300㎟이상");
		szArrTitle2.Add(nColCnt > 3 ? "필요" : "필요철근량");
		szArrTitle2.Add(nColCnt > 3 ? "사용" : "사용철근량");
	}
	szArrTitle1.Add("비 고");
	szArrTitle2.Add("비 고");

	if(nColCnt == 0) return;
	else if(nColCnt == 1)
	{
		dArrWidth1.Add(6);
		dArrWidth1.Add(12);
		dArrWidth1.Add(6);

		dArrWidth2.Add(3);
		dArrWidth2.Add(3);
		dArrWidth2.Add(6);
		dArrWidth2.Add(6);
		dArrWidth2.Add(6);
	}
	else if(nColCnt == 2)
	{
		dArrWidth1.Add(6);
		dArrWidth1.Add(8);
		dArrWidth1.Add(8);
		dArrWidth1.Add(2);
		
		dArrWidth2.Add(3);
		dArrWidth2.Add(3);
		dArrWidth2.Add(4);
		dArrWidth2.Add(4);
		dArrWidth2.Add(4);
		dArrWidth2.Add(4);
		dArrWidth2.Add(2);
	}
	else if(nColCnt == 3)
	{
		dArrWidth1.Add(4);
		dArrWidth1.Add(6);
		dArrWidth1.Add(6);
		dArrWidth1.Add(6);
		dArrWidth1.Add(2);
		
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(3);
		dArrWidth2.Add(3);
		dArrWidth2.Add(3);
		dArrWidth2.Add(3);
		dArrWidth2.Add(3);
		dArrWidth2.Add(3);
		dArrWidth2.Add(2);
	}
	else if(nColCnt == 4)
	{
		dArrWidth1.Add(4);
		dArrWidth1.Add(4);
		dArrWidth1.Add(4);
		dArrWidth1.Add(4);
		dArrWidth1.Add(4);
		dArrWidth1.Add(4);

		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(4);
	}

	int nCntRow = 2 + nCntOut;
	long ColWidth = 0;

	long nIndex[4] = { 0, 0, 0, 0 };
//	CString szTitle = _T("");
	CString szTitle = (pDataStd->m_nTypeUnit == UNIT_TYPE_SI)? "㎟":"㎠";
	XLText->AddFormatText("$s+03$h$c%s$r%s$m+18$i$me1단위$m+02$c:$r$e%s$n$h", GetNumberString(nIndexSub,NUM_CIRCLE), "중간벽체", szTitle);
	XLText->AddTable(nCntRow-1, 24-1, 2, 4);
	
	long nIdxBase = 0;
	for(long ix=0; ix<4; ++ix)
	{
		BOOL bCheck = FALSE;
		switch (ix)
		{
		case 0:
			bCheck = pBri->m_bCheckSuppoerBarWall15hs;	break;
		case 1:
			bCheck = pBri->m_bCheckSuppoerBarWallMainOver20;	break;
		case 2:
			bCheck = pBri->m_bCheckSuppoerBarWallArea;	break;
		case 3:
			bCheck = TRUE;	break;
		}
		if(bCheck)
		{
			nIndex[nIdxBase] = ix;
			++nIdxBase;
		}
	}
	// Title
	long nCntMerge = 0;
	for(i=0; i<nColCnt+2; i++)
	{
		ColWidth = (long)dArrWidth1.GetAt(i);
		szTitle = szArrTitle1.GetAt(i);
		XLText->AddMergeFormatText(nCntMerge, ColWidth-1,"$c%s", szTitle);
		if(i==0 || i==nColCnt+1) XLText->AddFormatText("$md1");
		XLText->AddFormatText("$m+%02d", ColWidth);
	}
	XLText->AddText("$n$h");
	for(i=0; i<nColCnt*2+2; i++)
	{
		ColWidth = (long)dArrWidth2.GetAt(i);
		szTitle = szArrTitle2.GetAt(i);
		XLText->AddMergeFormatText(nCntMerge, ColWidth-1,"$c%s", szTitle);
		XLText->AddFormatText("$m+%02d", ColWidth);
	}
	XLText->AddText("$n$h");

	// Data
	CString str = _T("");
	for(i = 0; i<nCntOut; i++)
	{
		CGirderAndColumn *pGirCol = NULL;
		if(pBri->m_nSelectInWallSectionDesign == 0)
		{
			pConcData = pBri->m_pArrConcInWall[eEnvLimit].GetAt(i);
		}
		else
		{
			long nIndex = 0;
			for(long ix = 0; ix<nSizeList; ix++)
			{
				pGirCol = m_pStd->m_pColGir->m_pList.GetAt(ix);
				if(pGirCol->m_nJijum == 0 || pGirCol->m_nJijum == nSizeList - 1) continue; //#ARCBRIDGE-3568
				if(!pGirCol->m_bWall) continue;
				if(nIndex == i)
				{
					pConcData = pGirCol->pConcColumn[0];
					break;
				}
				nIndex++;
			}
		}
		if(pConcData == NULL) continue;

		long j=0; for(j=0; j<nColCnt*2+3; j++)
		{
			ColWidth = (long)dArrWidth2.GetAt(j);
			
			if(j == 0)
			{
				if(pBri->m_nSelectInWallSectionDesign == 0)
				{
					ColWidth += (long)dArrWidth2.GetAt(j+1);
					str = pConcData->m_sTitle;
					XLText->AddMergeFormatText(nCntMerge, ColWidth-1,"$c%s", str);
					XLText->AddFormatText("$m+%02d", ColWidth);
					continue;
				}
				else
				{
					str.Format("중간벽체");
					XLText->AddFormatText("$md%d", (i==0)? nCntOut-1 : 0);
				}
			}
			else if(j == 1)
			{
				if(pBri->m_nSelectInWallSectionDesign == 0)
					continue;
				if(pGirCol)
					str.Format("지점%d", pGirCol->m_nJijum+1);
				else
					str = _T("");
			}
			else if(j == nColCnt*2+2) str = GetStringCheckConcWall(pConcData, TRUE, 4);
			else if(j == 2) str = GetStringCheckConcWall(pConcData, TRUE, nIndex[0]);
			else if(j == 3) str = GetStringCheckConcWall(pConcData, FALSE, nIndex[0]);
			else if(j == 4) str = GetStringCheckConcWall(pConcData, TRUE, nIndex[1]);
			else if(j == 5) str = GetStringCheckConcWall(pConcData, FALSE, nIndex[1]);
			else if(j == 6) str = GetStringCheckConcWall(pConcData, TRUE, nIndex[2]);
			else if(j == 7) str = GetStringCheckConcWall(pConcData, FALSE, nIndex[2]);
			else if(j == 8) str = GetStringCheckConcWall(pConcData, TRUE, nIndex[3]);
			else if(j == 9) str = GetStringCheckConcWall(pConcData, FALSE, nIndex[3]);
			else str = _T("");
			XLText->AddMergeFormatText(nCntMerge, ColWidth-1,"$c%s", str);
			XLText->AddFormatText("$m+%02d", ColWidth);
		}
		XLText->AddText("$n$h");
	}
	XLText->AddText("$n$h");
}

void CARcBridgeCalcCornerAndSupport::MakeTableSupportSumLowerSlab(CXLFormatText *XLText, long nIndexSub, BOOL bPRF)
{
	CHgBaseConcStd HgConcStd;
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pDataStd = pBri->m_pARcBridgeDataStd;

	if(!pBri->IsBoxType())
		return;

	CConcBasic *pConcData = NULL;
	CStringArray szArrTitle1;
	CStringArray szArrTitle2;
	CDoubleArray dArrWidth1;
	CDoubleArray dArrWidth2;

	XLText->GoPosition(NO_MOVE,COL_POS_2);

	szArrTitle1.RemoveAll();
	szArrTitle2.RemoveAll();
	dArrWidth1.RemoveAll();
	dArrWidth2.RemoveAll();

	szArrTitle1.Add("구 분");
	szArrTitle2.Add("구 분");
	int nColCnt = 0;
	if(pBri->m_bCheckSuppoerBarFoot15hs)	nColCnt++;
	if(pBri->m_bCheckSuppoerBarFootMainOver20)	nColCnt++;
	if(pBri->m_bCheckSuppoerBarFootArea)	nColCnt++;
	if(pBri->m_bCheckSuppoerBarFoot300)		nColCnt++;

	if(pBri->m_bCheckSuppoerBarFoot15hs)
	{
		szArrTitle1.Add("0.0015hb");
		szArrTitle2.Add("필요철근량");
		szArrTitle2.Add("사용철근량");
	}
	if(pBri->m_bCheckSuppoerBarFootMainOver20)
	{
		szArrTitle1.Add("주철근의 20%");
		szArrTitle2.Add("필요철근량");
		szArrTitle2.Add("사용철근량");
	}
	if(pBri->m_bCheckSuppoerBarFootArea)
	{
		szArrTitle1.Add("단면적비");
		szArrTitle2.Add("필요철근량");
		szArrTitle2.Add("사용철근량");
	}
	if(pBri->m_bCheckSuppoerBarFoot300)
	{
		szArrTitle1.Add("300㎟이상");
		szArrTitle2.Add("필요철근량");
		szArrTitle2.Add("사용철근량");
	}
	szArrTitle1.Add("비 고");
	szArrTitle2.Add("비 고");

	if(nColCnt == 0) return;
	else if(nColCnt == 1)
	{
		dArrWidth1.Add(6);
		dArrWidth1.Add(12);
		dArrWidth1.Add(6);

		dArrWidth2.Add(6);
		dArrWidth2.Add(6);
		dArrWidth2.Add(6);
		dArrWidth2.Add(6);
	}
	else if(nColCnt == 2)
	{
		dArrWidth1.Add(4);
		dArrWidth1.Add(8);
		dArrWidth1.Add(8);
		dArrWidth1.Add(4);
		
		dArrWidth2.Add(4);
		dArrWidth2.Add(4);
		dArrWidth2.Add(4);
		dArrWidth2.Add(4);
		dArrWidth2.Add(4);
		dArrWidth2.Add(4);
	}
	else if(nColCnt == 3)
	{
		dArrWidth1.Add(4);
		dArrWidth1.Add(6);
		dArrWidth1.Add(6);
		dArrWidth1.Add(6);
		dArrWidth1.Add(2);
		
		dArrWidth2.Add(4);
		dArrWidth2.Add(3);
		dArrWidth2.Add(3);
		dArrWidth2.Add(3);
		dArrWidth2.Add(3);
		dArrWidth2.Add(3);
		dArrWidth2.Add(3);
		dArrWidth2.Add(2);
	}
	else if(nColCnt == 4)
	{
		dArrWidth1.Add(4);
		dArrWidth1.Add(4);
		dArrWidth1.Add(4);
		dArrWidth1.Add(4);
		dArrWidth1.Add(4);
		dArrWidth1.Add(4);

		dArrWidth2.Add(4);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(2);
		dArrWidth2.Add(4);
	}

	long nSizeConc = bPRF ? pBri->m_pArrConcPRF[eEnvLimit].GetSize() : pBri->m_pArrConcSlab[eEnvLimit].GetSize();
	long nCntRow = nSizeConc;
	if(bPRF == FALSE)
		nCntRow = nSizeConc/2;
	
	long ColWidth = 0;
	long nIndex[4] = { 0, 0, 0, 0 };
	CString szUnit = (pDataStd->m_nTypeUnit == UNIT_TYPE_SI)? "㎟":"㎠";
	CString szTitle = bPRF ? _T("부상방지저판") : _T("하부슬래브");
	XLText->AddFormatText("$s+03$h$c%s$r%s$m+18$i$me1단위$m+02$c:$r$e%s$n$h", GetNumberString(nIndexSub,NUM_CIRCLE), szTitle, szUnit);
	XLText->AddTable(nCntRow+1, 24-1, 2, 1);

	long nIdxBase = 0;
	for(long ix=0; ix<4; ++ix)
	{
		BOOL bCheck = FALSE;
		switch (ix)
		{
		case 0:
			bCheck = pBri->m_bCheckSuppoerBarFoot15hs;	break;
		case 1:
			bCheck = pBri->m_bCheckSuppoerBarFootMainOver20;	break;
		case 2:
			bCheck = pBri->m_bCheckSuppoerBarFootArea;	break;
		case 3:
			bCheck = TRUE;	break;
		}
		if(bCheck)
		{
			nIndex[nIdxBase] = ix;
			++nIdxBase;
		}
	}
	
	// Title
	long nCntMerge = 0;
	long i=0; for(i=0; i<nColCnt+2; i++)
	{
		ColWidth = (long)dArrWidth1.GetAt(i);
		szTitle = szArrTitle1.GetAt(i);
		XLText->AddMergeFormatText(nCntMerge, ColWidth-1,"$c%s", szTitle);
		if(i==0 || i==nColCnt+1) XLText->AddFormatText("$md1");
		XLText->AddFormatText("$m+%02d", ColWidth);
	}
	XLText->AddText("$n$h");
	for(i=0; i<nColCnt*2+2; i++)
	{
		ColWidth = (long)dArrWidth2.GetAt(i);
		szTitle = szArrTitle2.GetAt(i);
		XLText->AddMergeFormatText(nCntMerge, ColWidth-1,"$c%s", szTitle);
		XLText->AddFormatText("$m+%02d", ColWidth);
	}
	XLText->AddText("$n$h");

	// Data
	CString str = _T("");
	long nIndexConcStt = bPRF ? 0 : nSizeConc/2;
	long nIndexConcEnd = nSizeConc;
	for(i = nIndexConcStt; i < nIndexConcEnd; i++)
	{
		pConcData = bPRF ? pBri->m_pArrConcPRF[eEnvLimit].GetAt(i) : pBri->m_pArrConcSlab[eEnvLimit].GetAt(i);
		if(pConcData == NULL) continue;

		for(long j=0; j<nColCnt*2+2; j++)
		{
			ColWidth = (long)dArrWidth2.GetAt(j);

			if(j == 0) str = pConcData->m_sTitle;
			else if(j == nColCnt*2+1) str = GetStringCheckConcFooting(pConcData, TRUE, 4);
			else if(j == 1) str = GetStringCheckConcFooting(pConcData, TRUE, nIndex[0]);
			else if(j == 2) str = GetStringCheckConcFooting(pConcData, FALSE, nIndex[0]);
			else if(j == 3) str = GetStringCheckConcFooting(pConcData, TRUE, nIndex[1]);
			else if(j == 4) str = GetStringCheckConcFooting(pConcData, FALSE, nIndex[1]);
			else if(j == 5) str = GetStringCheckConcFooting(pConcData, TRUE, nIndex[2]);
			else if(j == 6) str = GetStringCheckConcFooting(pConcData, FALSE, nIndex[2]);
			else if(j == 7) str = GetStringCheckConcFooting(pConcData, TRUE, nIndex[3]);
			else if(j == 8) str = GetStringCheckConcFooting(pConcData, FALSE, nIndex[3]);
			else str = _T("");
			XLText->AddMergeFormatText(nCntMerge, ColWidth-1,"$c%s", str);
			XLText->AddFormatText("$m+%02d", ColWidth);
		}
		XLText->AddText("$n$h");
	}
	XLText->AddText("$n$h");
}

// nIndex => 0: 주철근비 1: 주철근의 20%  2: 단면적비 3: 300mm2이상 4: OK/NG
// nKind => 0: Slab, 1: Wall, 3:Footing
CString CARcBridgeCalcCornerAndSupport::GetStringCheckConcSlab(CConcBasic *pConcData, BOOL bReq, long nIndex, double dLenSpan, BOOL bOption)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	BOOL bSI = (pData->m_nTypeUnit == UNIT_TYPE_SI)? TRUE:FALSE;
	BOOL bOK = TRUE;
	CString szRet = _T("");
	double dAreaUse[4];
	double dAreaReq[4];
	double dRatio = 0;
	
	// 주철근비 검토
	CRectBeam slab(pConcData);
	double dAreaMainRebar = slab.GetRebarAreaRequired();
	double dLenJigan = dLenSpan;
	dRatio = MIN(55/sqrt(toM(dLenJigan))/100, 0.5);
	dAreaReq[0] = dAreaMainRebar * dRatio;
	dAreaUse[0] = m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, FALSE, !bSI);
	if(pData->m_nTypeKciCode != 0)	// Kci 2007
	{
		dAreaReq[0] = MIN(dAreaReq[0], (bSI? 1800:18));
	}

	// 주철근의 20%
	dRatio = 0.2;
	dAreaReq[1] = dAreaMainRebar * dRatio;
	dAreaUse[1] = dAreaUse[0];
	if(pData->m_nTypeKciCode != 0)	// Kci 2007
	{
		dAreaReq[1] = MIN(dAreaReq[1], (bSI? 1800:18));
	}
		
	// 단면적비 검토
	double dAreaSection = pConcData->m_Ho * pConcData->m_Bo;
	dRatio = GetSupportBarSlabRatio(pConcData->m_Fy);
	dAreaReq[2] = (bSI)? dAreaSection * dRatio:toCm2(dAreaSection * dRatio);
	dAreaUse[2] = m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, TRUE, !bSI);
	if(pData->m_nTypeKciCode != 0)	// Kci 2007
	{
		dAreaReq[2] = MIN(dAreaReq[2], (bSI? 1800:18));
	}
	
	// 300mm2 이상 검토
	dAreaReq[3] = (bSI)? 300:toCm2(300);
	dAreaUse[3] = MIN(m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, FALSE, !bSI), m_pStd->GetRebarAreaSuportBar(pConcData, FALSE, TRUE, !bSI));
	
	if(pBri->m_bCheckSuppoerBarSlabMain && (dAreaReq[0] > dAreaUse[0])) bOK = FALSE;
	if(pBri->m_bCheckSuppoerBarSlabMainOver20 && (dAreaReq[1] > dAreaUse[1])) bOK = FALSE;
	if(pBri->m_bCheckSuppoerBarSlabArea && (dAreaReq[2] > dAreaUse[2]))	bOK = FALSE;
	if(pBri->m_bCheckSuppoerBarSlab300 && (dAreaReq[3] > dAreaUse[3])) bOK = FALSE;
	
	if(nIndex == 4) szRet = (bOK)? "O.K":(bOption)? "$#03N.G":"N.G";
	else if(bReq && bSI) szRet.Format("$f1%.1f", dAreaReq[nIndex]);
	else if(bReq && !bSI) szRet.Format("$f3%.3f", dAreaReq[nIndex]);
	else if(bSI) szRet.Format("$f1%.1f", dAreaUse[nIndex]);
	else szRet.Format("$f3%.3f", dAreaUse[nIndex]);
	
	return szRet;
}

CString CARcBridgeCalcCornerAndSupport::GetStringCheckConcAps(CConcBasic *pConcData, BOOL bReq, long nIndex, double dLenSpan, BOOL bOption)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	BOOL bSI = (pData->m_nTypeUnit == UNIT_TYPE_SI)? TRUE:FALSE;
	BOOL bOK = TRUE;
	CString szRet = _T("");
	double dAreaUse[3];
	double dAreaReq[3];
	double dRatio = 0;
	// 주철근비 검토
	CRectBeam slab(pConcData);
	double dAreaMainRebar = slab.GetRebarAreaRequired();
	double dLenJigan = dLenSpan * 0.7;
	dRatio = MIN(55/sqrt(toM(dLenJigan))/100, 0.5);
	dAreaReq[0] = dAreaMainRebar * dRatio;
	dAreaUse[0] = m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, FALSE, !bSI);
	if(pData->m_nTypeKciCode != 0)	// Kci 2007
	{
		dAreaReq[0] = MIN(dAreaReq[0], (bSI? 1800:18));
	}
	
	// 단면적비 검토
	double dAreaSection = pConcData->m_Ho * pConcData->m_Bo;
	dRatio = GetSupportBarSlabRatio(pConcData->m_Fy);
	dAreaReq[1] = (bSI)? dAreaSection * dRatio:toCm2(dAreaSection * dRatio);
	dAreaUse[1] = m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, TRUE, !bSI);
	if(pData->m_nTypeKciCode != 0)	// Kci 2007
	{
		dAreaReq[1] = MIN(dAreaReq[1], (bSI? 1800:18));
	}
	
	// 300mm2 이상 검토
	dAreaReq[2] = (bSI)? 300:toCm2(300);
	dAreaUse[2] = MIN(m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, FALSE, !bSI), m_pStd->GetRebarAreaSuportBar(pConcData, FALSE, TRUE, !bSI));
	
	if(pBri->m_bCheckSuppoerBarSlabMain && (dAreaReq[0] > dAreaUse[0])) bOK = FALSE;
	if(pBri->m_bCheckSuppoerBarSlabArea && (dAreaReq[1] > dAreaUse[1]))	bOK = FALSE;
	if(pBri->m_bCheckSuppoerBarSlab300 && (dAreaReq[2] > dAreaUse[2])) bOK = FALSE;
	
	if(nIndex > 0)
		nIndex -= 1;

	if(nIndex == 3) szRet = (bOK)? "O.K":(bOption)? "$#03N.G":"N.G";
	else if(bReq && bSI) szRet.Format("$f1%.1f", dAreaReq[nIndex]);
	else if(bReq && !bSI) szRet.Format("$f3%.3f", dAreaReq[nIndex]);
	else if(bSI) szRet.Format("$f1%.1f", dAreaUse[nIndex]);
	else szRet.Format("$f3%.3f", dAreaUse[nIndex]);
	
	return szRet;
}

CString CARcBridgeCalcCornerAndSupport::GetStringCheckConcWall(CConcBasic *pConcData, BOOL bReq, long nIndex, BOOL bOption)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	BOOL bSI = (pData->m_nTypeUnit == UNIT_TYPE_SI)? TRUE:FALSE;
	BOOL bOK = TRUE;
	CString szRet = _T("");
	double dAreaUse[4];
	double dAreaReq[4];
	double dRatio = 0;
	
	// 0.0015hs
	double dThick = MIN(1200, pConcData->m_Ho);
	double dB = pConcData->m_Bo;
	dAreaReq[0] = (bSI)? 0.0015 * dThick * dB:toCm2(0.0015 * dThick * dB);
	dAreaUse[0] = m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, TRUE, !bSI);
	if(pData->m_nTypeKciCode != 0)	// Kci 2007
	{
		dAreaReq[0] = MIN(dAreaReq[0], (bSI? 1800:18));
	}

	// 주철근의 20%
	CRectBeam slab(pConcData);
	double dAreaMainRebar = slab.GetRebarAreaRequired();
	dRatio = 0.2;
	dAreaReq[1] = dAreaMainRebar * dRatio;
	dAreaUse[1] = dAreaUse[0];
	if(pData->m_nTypeKciCode != 0)	// Kci 2007
	{
		dAreaReq[1] = MIN(dAreaReq[1], (bSI? 1800:18));
	}
	
	// 단면적비 검토
	double dAreaSection = pConcData->m_Ho * pConcData->m_Bo;
	dRatio = pBri->m_dCheckSuppoerBarWallArea;
	if(pConcData->m_Fy >= 4000 && pConcData->m_TSupportBarDia[0] <= 16.0)
	{
		dRatio = pBri->m_dCheckSuppoerBarWallAreaForOver400;
	}
	dAreaReq[2] = (bSI)? dAreaSection * dRatio:toCm2(dAreaSection * dRatio);
	dAreaUse[2] = m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, TRUE, !bSI);
	if(pData->m_nTypeKciCode != 0)	// Kci 2007
	{
		dAreaReq[2] = MIN(dAreaReq[2], (bSI? 1800:18));
	}
	
	// 300mm2 이상 검토
	dAreaReq[3] = (bSI)? 300:toCm2(300);
	dAreaUse[3] = (pConcData->m_sTitle.Find("중앙") != -1 || pConcData->m_sTitle.Find("중간") != -1)? m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, FALSE, !bSI) : m_pStd->GetRebarAreaSuportBar(pConcData, FALSE, TRUE, !bSI);
	
	if(pBri->m_bCheckSuppoerBarWall15hs && (dAreaReq[0] > dAreaUse[0])) bOK = FALSE;
	if(pBri->m_bCheckSuppoerBarWallMainOver20 && (dAreaReq[1] > dAreaUse[1])) bOK = FALSE;
	if(pBri->m_bCheckSuppoerBarWallArea && (dAreaReq[2] > dAreaUse[2])) bOK = FALSE;
	if(pBri->m_bCheckSuppoerBarWall300 && (dAreaReq[3] > dAreaUse[3])) bOK = FALSE;
	
	if(nIndex == 4) szRet = (bOK)? "O.K":(bOption)? "$#03N.G":"N.G";
	else if(bReq && bSI) szRet.Format("$f1%.1f", dAreaReq[nIndex]);
	else if(bReq && !bSI) szRet.Format("$f3%.3f", dAreaReq[nIndex]);
	else if(bSI) szRet.Format("$f1%.1f", dAreaUse[nIndex]);
	else szRet.Format("$f3%.3f", dAreaUse[nIndex]);
	
	return szRet;
}

CString CARcBridgeCalcCornerAndSupport::GetStringCheckConcFooting(CConcBasic *pConcData, BOOL bReq, long nIndex, BOOL bOption)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	BOOL bSI = (pData->m_nTypeUnit == UNIT_TYPE_SI)? TRUE:FALSE;
	BOOL bOK = TRUE;
	CString szRet = _T("");
	double dAreaUse[4];
	double dAreaReq[4];
	double dRatio = 0;
	
	// 0.0015hs
	double dThick = MIN(1200, pConcData->m_Ho);
	double dB = pConcData->m_Bo;
	dAreaReq[0] = (bSI)? 0.0015 * dThick * dB:toCm2(0.0015 * dThick * dB);
	dAreaUse[0] = m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, TRUE, !bSI);
	if(pData->m_nTypeKciCode != 0)	// Kci 2007
	{
		dAreaReq[0] = MIN(dAreaReq[0], (bSI? 1800:18));
	}

	// 주철근의 20%
	CRectBeam slab(pConcData);
	double dAreaMainRebar = slab.GetRebarAreaRequired();
	dRatio = 0.2;
	dAreaReq[1] = dAreaMainRebar * dRatio;
	dAreaUse[1] = dAreaUse[0];
	if(pData->m_nTypeKciCode != 0)	// Kci 2007
	{
		dAreaReq[1] = MIN(dAreaReq[1], (bSI? 1800:18));
	}
		
	// 단면적비 검토
	double dAreaSection = pConcData->m_Ho * pConcData->m_Bo;
	dRatio = GetSupportBarFootRatio(pConcData->m_Fy);
	dAreaReq[2] = (bSI)? dAreaSection * dRatio:toCm2(dAreaSection * dRatio);
	dAreaUse[2] = m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, TRUE, !bSI);
	if(pData->m_nTypeKciCode != 0)	// Kci 2007
	{
		dAreaReq[2] = MIN(dAreaReq[2], (bSI? 1800:18));
	}
	
	// 300mm2 이상 검토
	dAreaReq[3] = (bSI)? 300:toCm2(300);
	dAreaUse[3] = MIN(m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, FALSE, !bSI), m_pStd->GetRebarAreaSuportBar(pConcData, FALSE, TRUE, !bSI));
	
	if(pBri->m_bCheckSuppoerBarFoot15hs && (dAreaReq[0] > dAreaUse[0])) bOK = FALSE;
	if(pBri->m_bCheckSuppoerBarFootMainOver20 && (dAreaReq[1] > dAreaUse[1])) bOK = FALSE;
	if(pBri->m_bCheckSuppoerBarFootArea && (dAreaReq[2] > dAreaUse[2])) bOK = FALSE;
	if(pBri->m_bCheckSuppoerBarFoot300 && (dAreaReq[3] > dAreaUse[3])) bOK = FALSE;
	
	if(nIndex == 4) szRet = (bOK)? "O.K":(bOption)? "$#03N.G":"N.G";
	else if(bReq && bSI) szRet.Format("$f1%.1f", dAreaReq[nIndex]);
	else if(bReq && !bSI) szRet.Format("$f3%.3f", dAreaReq[nIndex]);
	else if(bSI) szRet.Format("$f1%.1f", dAreaUse[nIndex]);
	else szRet.Format("$f3%.3f", dAreaUse[nIndex]);
	
	return szRet;
}

CString CARcBridgeCalcCornerAndSupport::GetStringCheckConcWingWall(CConcBasic *pConcData, BOOL bReq, long nIndex, BOOL bOption)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	BOOL bSI = (pData->m_nTypeUnit == UNIT_TYPE_SI)? TRUE:FALSE;
	BOOL bOK = TRUE;
	CString szRet = _T("");
	double dAreaUse[3];
	double dAreaReq[3];
	double dRatio = 0;
	
	// 0.0015hs
	double dThick = MIN(1200, pConcData->m_Ho);
	double dB = frM(UNIT_METER);
	dAreaReq[0] = (bSI)? 0.0015 * dThick * dB:toCm2(0.0015 * dThick * dB);
	dAreaUse[0] = m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, TRUE, !bSI);
	if(pData->m_nTypeKciCode != 0)	// Kci 2007
	{
		dAreaReq[0] = MIN(dAreaReq[0], (bSI? 1800:18));
	}
	
	// 단면적비 검토
	double dAreaSection = pConcData->m_Ho * pConcData->m_Bo;
	dRatio = pBri->m_dCheckSuppoerBarWingArea;
	dAreaReq[1] = (bSI)? dAreaSection * dRatio:toCm2(dAreaSection * dRatio);
	dAreaUse[1] = m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, TRUE, !bSI);
	if(pData->m_nTypeKciCode != 0)	// Kci 2007
	{
		dAreaReq[1] = MIN(dAreaReq[1], (bSI? 1800:18));
	}
	
	// 300mm2 이상 검토
	dAreaReq[2] = (bSI)? 300:toCm2(300);
	dAreaUse[2] = MIN(m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, FALSE, !bSI), m_pStd->GetRebarAreaSuportBar(pConcData, FALSE, TRUE, !bSI));
	
	if(pBri->m_bCheckSuppoerBarWing15hs && (dAreaReq[0] > dAreaUse[0])) bOK = FALSE;
	if(pBri->m_bCheckSuppoerBarWingArea && (dAreaReq[1] > dAreaUse[1])) bOK = FALSE;
	if(pBri->m_bCheckSuppoerBarWing300 && (dAreaReq[2] > dAreaUse[2])) bOK = FALSE;
	
	if(nIndex == 3) szRet = (bOK)? "O.K":(bOption)? "$#03N.G":"N.G";
	else if(bReq && bSI) szRet.Format("$f1%.1f", dAreaReq[nIndex]);
	else if(bReq && !bSI) szRet.Format("$f3%.3f", dAreaReq[nIndex]);
	else if(bSI) szRet.Format("$f1%.1f", dAreaUse[nIndex]);
	else szRet.Format("$f3%.3f", dAreaUse[nIndex]);
	
	return szRet;
}

double CARcBridgeCalcCornerAndSupport::GetDoubleCheckConcSlab(CConcBasic *pConcData, BOOL bReq, long nIndex, double dLenSpan)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	BOOL bSI = (pData->m_nTypeUnit == UNIT_TYPE_SI)? TRUE:FALSE;
	
	double dRet = 0;
	double dAreaUse[4];
	double dAreaReq[4];
	double dRatio = 0;
	// 주철근비 검토
	CRectBeam slab(pConcData);
	double dAreaMainRebar = slab.GetRebarAreaRequired();
	double dLenJigan = dLenSpan;
	dRatio = MIN(55/sqrt(toM(dLenJigan))/100, 0.5);
	dAreaReq[0] = dAreaMainRebar * dRatio;
	dAreaUse[0] = m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, FALSE, !bSI);
	if(pData->m_nTypeKciCode != 0)	// Kci 2007
	{
		dAreaReq[0] = MIN(dAreaReq[0], (bSI? 1800:18));
	}

	// 주철근의 20%
	dRatio = 0.2;
	dAreaReq[1] = dAreaMainRebar * dRatio;
	dAreaUse[1] = dAreaUse[0];
	if(pData->m_nTypeKciCode != 0)	// Kci 2007
	{
		dAreaReq[1] = MIN(dAreaReq[1], (bSI? 1800:18));
	}
		
	// 단면적비 검토
	double dAreaSection = pConcData->m_Ho * pConcData->m_Bo;
	dRatio = GetSupportBarSlabRatio(pConcData->m_Fy);
	dAreaReq[2] = (bSI)? dAreaSection * dRatio:toCm2(dAreaSection * dRatio);
	dAreaUse[2] = m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, TRUE, !bSI);
	if(pData->m_nTypeKciCode != 0)	// Kci 2007
	{
		dAreaReq[2] = MIN(dAreaReq[2], (bSI? 1800:18));
	}
	
	// 300mm2 이상 검토
	dAreaReq[3] = (bSI)? 300:toCm2(300);
	dAreaUse[3] = MIN(m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, FALSE, !bSI), m_pStd->GetRebarAreaSuportBar(pConcData, FALSE, TRUE, !bSI));

	dRet = (bReq)? dAreaReq[nIndex]:dAreaUse[nIndex];
	
	return dRet;
}

double CARcBridgeCalcCornerAndSupport::GetDoubleCheckConcWall(CConcBasic *pConcData, BOOL bReq, long nIndex, BOOL bWingWall)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bSI = (pData->m_nTypeUnit == UNIT_TYPE_SI)? TRUE:FALSE;
	double dRet = 0;
	double dAreaUse[4];
	double dAreaReq[4];
	double dRatio = 0;
	
	// 0.0015hs
	double dMaxH = pData->IsDoro2010() ? 1800 : 1200;
	double dThick = MIN(dMaxH, pConcData->m_Ho);
	double dB = pConcData->m_Bo;
	dAreaReq[0] = (bSI)? 0.0015 * dThick * dB:toCm2(0.0015 * dThick * dB);
	dAreaUse[0] = m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, TRUE, !bSI);
	if(pData->m_nTypeKciCode != 0)	// Kci 2007
	{
		dAreaReq[0] = MIN(dAreaReq[0], (bSI ? 1800 : 18));
	}

	// 주철근의 20%
	CRectBeam slab(pConcData);
	double dAreaMainRebar = slab.GetRebarAreaRequired();
	dRatio = 0.2;
	dAreaReq[1] = dAreaMainRebar * dRatio;
	dAreaUse[1] = dAreaUse[0];
	if(pData->m_nTypeKciCode != 0)	// Kci 2007
	{
		dAreaReq[1] = MIN(dAreaReq[1], (bSI? 1800:18));
	}
	
	// 단면적비 검토
	double dAreaSection = pConcData->m_Ho * pConcData->m_Bo;
	dRatio = bWingWall ? pBri->m_dCheckSuppoerBarWingArea : pBri->m_dCheckSuppoerBarWallArea;
	if(!bWingWall && pConcData->m_Fy >= 4000 && pConcData->m_TSupportBarDia[0] <= 16.0)
	{
		dRatio = pBri->m_dCheckSuppoerBarWallAreaForOver400;
	}

	dAreaReq[2] = (bSI)? dAreaSection * dRatio:toCm2(dAreaSection * dRatio);
	dAreaUse[2] = m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, TRUE, !bSI);
	if(pData->m_nTypeKciCode != 0)	// Kci 2007
	{
		dAreaReq[2] = MIN(dAreaReq[2], (bSI? 1800:18));
	}
	
	// 300mm2 이상 검토
	dAreaReq[3] = (bSI)? 300:toCm2(300);
	dAreaUse[3] = (pConcData->m_sTitle.Find("중앙") != -1 || pConcData->m_sTitle.Find("중간") != -1)? m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, FALSE, !bSI) : m_pStd->GetRebarAreaSuportBar(pConcData, FALSE, TRUE, !bSI);

	dRet = (bReq)? dAreaReq[nIndex]:dAreaUse[nIndex];
	
	return dRet;
}

double CARcBridgeCalcCornerAndSupport::GetDoubleCheckConcFooting(CConcBasic *pConcData, BOOL bReq, long nIndex)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	BOOL bSI = (pData->m_nTypeUnit == UNIT_TYPE_SI)? TRUE:FALSE;
	double dRet = 0;
	double dAreaUse[4];
	double dAreaReq[4];
	double dRatio = 0;
	
	// 0.0015hs
	double dMaxH = pData->IsDoro2010() ? 1800 : 1200;
	double dThick = MIN(dMaxH, pConcData->m_Ho);
	double dB = pConcData->m_Bo;
	dAreaReq[0] = (bSI)? 0.0015 * dThick * dB:toCm2(0.0015 * dThick * dB);
	dAreaUse[0] = m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, TRUE, !bSI);
	if(pData->m_nTypeKciCode != 0)	// Kci 2007
	{
		dAreaReq[0] = MIN(dAreaReq[0], (bSI? 1800:18));
	}

	// 주철근의 20%
	CRectBeam slab(pConcData);
	double dAreaMainRebar = slab.GetRebarAreaRequired();
	dRatio = 0.2;
	dAreaReq[1] = dAreaMainRebar * dRatio;
	dAreaUse[1] = dAreaUse[0];
	if(pData->m_nTypeKciCode != 0)	// Kci 2007
	{
		dAreaReq[1] = MIN(dAreaReq[1], (bSI? 1800:18));
	}
	
	// 단면적비 검토
	double dAreaSection = pConcData->m_Ho * pConcData->m_Bo;
	dRatio = GetSupportBarFootRatio(pConcData->m_Fy);
	dAreaReq[2] = (bSI)? dAreaSection * dRatio:toCm2(dAreaSection * dRatio);
	dAreaUse[2] = m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, TRUE, !bSI);
	if(pData->m_nTypeKciCode != 0)	// Kci 2007
	{
		dAreaReq[2] = MIN(dAreaReq[2], (bSI? 1800:18));
	}
	
	// 300mm2 이상 검토
	dAreaReq[3] = (bSI)? 300:toCm2(300);
	dAreaUse[3] = MIN(m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, FALSE, !bSI), m_pStd->GetRebarAreaSuportBar(pConcData, FALSE, TRUE, !bSI));

	dRet = (bReq)? dAreaReq[nIndex]:dAreaUse[nIndex];
	
	return dRet;
}

double CARcBridgeCalcCornerAndSupport::GetDoubleCheckConcAps(CConcBasic *pConcData, BOOL bReq, long nIndex, double dLenSpan)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	BOOL bSI = (pData->m_nTypeUnit == UNIT_TYPE_SI)? TRUE:FALSE;
	
	double dRet = 0;
	double dAreaUse[4];
	double dAreaReq[4];
	double dRatio = 0;
	// 주철근비 검토
	CRectBeam slab(pConcData);
	double dAreaMainRebar = slab.GetRebarAreaRequired();
	double dLenJigan = dLenSpan * 0.7;
	dRatio = MIN(55/sqrt(toM(dLenJigan))/100, 0.5);
	dAreaReq[0] = dAreaMainRebar * dRatio;
	dAreaUse[0] = m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, FALSE, !bSI);
	if(pData->m_nTypeKciCode != 0)	// Kci 2007
	{
		dAreaReq[0] = MIN(dAreaReq[0], (bSI? 1800:18));
	}

	// 주철근의 20%
	dAreaReq[1] = 0;
	dAreaUse[1] = 0;
	
	// 단면적비 검토
	double dAreaSection = pConcData->m_Ho * pConcData->m_Bo;
	dRatio = GetSupportBarSlabRatio(pConcData->m_Fy);
	dAreaReq[2] = (bSI)? dAreaSection * dRatio:toCm2(dAreaSection * dRatio);
	dAreaUse[2] = m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, TRUE, !bSI);
	if(pData->m_nTypeKciCode != 0)	// Kci 2007
	{
		dAreaReq[2] = MIN(dAreaReq[2], (bSI? 1800:18));
	}
	
	// 300mm2 이상 검토
	dAreaReq[3] = (bSI)? 300:toCm2(300);
	dAreaUse[3] = MIN(m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, FALSE, !bSI), m_pStd->GetRebarAreaSuportBar(pConcData, FALSE, TRUE, !bSI));
	
	dRet = (bReq)? dAreaReq[nIndex]:dAreaUse[nIndex];
	
	return dRet;
}

BOOL CARcBridgeCalcCornerAndSupport::CalculateInsideBending(BOOL bStt)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	double dMCornerInSide = pBri->GetMomentCorner(bStt, TRUE, FALSE, m_bUpperSlab);
	dMCornerInSide = (pData->m_nTypeUnit == 0)? toCm(frTonM(dMCornerInSide)):dMCornerInSide/1000*pow(10.0, 9.0);

	if(dMCornerInSide > 0)
	{
		CConcBasic *pConcWall = pBri->GetConcDataWall(bStt? 0:pBri->m_nQtyJigan, CHECK_POS_TOP, FALSE);
		CConcBasic *pConcSlab = pBri->GetConcDataSlab(bStt? 0:pBri->m_nQtyJigan-1, bStt? CHECK_POS_LEFT:CHECK_POS_RIGHT);

		if(pConcWall == NULL || pConcSlab == NULL) return FALSE;

		double dn = Round(pConcWall->m_Es/pConcWall->m_Ec, 0);
		int nIndex = bStt? 0 : 1;
		// 수평부재
		double dAs = (pData->m_nTypeUnit == 0)? pConcSlab->GetRebarAreaCompression() : frCm2(pConcSlab->GetRebarAreaCompression());
		double dB = (pData->m_nTypeUnit == 0)? toCm(pConcSlab->m_Bo) : pConcSlab->m_Bo;
		double d0 = (pData->m_nTypeUnit == 0)? toCm(pConcSlab->m_Ho - pConcSlab->m_RbC1C_dc[0] + pConcSlab->m_Hunch/3) : (pConcSlab->m_Ho - pConcSlab->m_RbC1C_dc[0] + pConcSlab->m_Hunch/3);

		double dp = dAs/(dB * d0);
		double dK = -1 * (dn * dp) + sqrt(2 * dn * dp + pow(dn * dp, 2));
		double dJ = 1 - dK/3;
		double dFs = dMCornerInSide / (dAs * d0 * dJ);
		double Th1 = dFs * dAs;
		
		// 수직부재
		dAs = (pData->m_nTypeUnit == 0)? pConcWall->GetRebarAreaCompression() : frCm2(pConcWall->GetRebarAreaCompression());
		dB = (pData->m_nTypeUnit == 0)? toCm(pConcWall->m_Bo) : pConcWall->m_Bo;
		d0 = (pData->m_nTypeUnit == 0)? toCm(pConcWall->m_Ho - pConcWall->m_RbC1C_dc[0] + pConcWall->m_Hunch/3) : (pConcWall->m_Ho - pConcWall->m_RbC1C_dc[0] + pConcWall->m_Hunch/3);
		dp = dAs/(dB * d0);
		dK = -1 * (dn * dp) + sqrt(2 * dn * dp + pow(dn * dp, 2));
		dJ = 1 - dK/3;
		dFs = dMCornerInSide / (dAs * d0 * dJ);

		double Th2 = dFs * dAs;

		// 보강철근량 산정
		double dAs_ = sqrt(pow(Th1,2) + pow(Th2,2)) / m_dFsa[nIndex];

		m_dMomentInSide[nIndex] = dMCornerInSide;
		m_dTh1[nIndex]	= Th1;
		m_dTh2[nIndex]	= Th2;
		m_dAs_[nIndex]	= dAs_;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

void CARcBridgeCalcCornerAndSupport::WriteSupportCheckArchRib(CXLFormatText *XLText)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	BOOL bSI = (pData->m_nTypeUnit == UNIT_TYPE_SI)? TRUE:FALSE;
	
	CConcBasic *pConcData;
	long nCnt = pBri->m_pArrConcArchRib.GetSize();

	double dRatio = 0;
	double dAreaReq = 0;
	double dAreaReqSect = 0;
	double dAreaUseC = 0;
	double dAreaUseT = 0;
	double dAreaUseTot = 0;
	double dAreaUseComp = 0;
	CString szComp = _T("");
	CString szOkNg = _T("");
	CString szTitle = _T("");

	long i=0; for(i=0; i<nCnt; i++)
	{

		pConcData = pBri->m_pArrConcArchRib.GetAt(i);
		if(pConcData == NULL) continue;

		dAreaUseC = m_pStd->GetRebarAreaSuportBar(pConcData, FALSE, TRUE, !bSI);
		dAreaUseT = m_pStd->GetRebarAreaSuportBar(pConcData, TRUE, FALSE, !bSI);
		dAreaUseTot = dAreaUseC + dAreaUseT;
		
		XLText->AddFormatText("$s+20$i$h%s$r%s$n$h", GetNumberString(i+1,NUM_GANA), pConcData->m_sTitle);
		XLText->GoPosition(NO_MOVE,COL_POS_2);

		XLText->AddFormatText("$h$r$c-$r$e사용철근량$n$h");
		XLText->AddFormatText("$h$r$r$r$e압축부$m+02$c:$r$e");
		long j=0; for(j=0; j<MAX_REBAR_DAN; j++)
		{
			if(pConcData->m_CSupportBarDia[j] == 0) continue;
			if(j != 0) XLText->AddFormatText("$n$h$r$r$r$m+02$r");
			XLText->AddFormatText("$e%s @ %.0f", pData->GetStringDia(pConcData->m_CSupportBarDia[j], pConcData->m_Fy), pConcData->m_CSupportBarCTC[j]);
		}
		if(bSI) XLText->AddFormatText("$m+03$c=$r$e$f1%.1f$me1$m+02㎟$n$h", dAreaUseC);
		else XLText->AddFormatText("$m+03$c=$r$e$f3%.3f$me1$m+02㎠$n$h", dAreaUseC);
		
		XLText->AddFormatText("$h$r$r$r$e인장부$m+02$c:$r$e");
		for(j=0; j<MAX_REBAR_DAN; j++)
		{
			if(pConcData->m_TSupportBarDia[j] == 0) continue;
			if(j != 0) XLText->AddFormatText("$n$h$r$r$r$m+02$r");
			XLText->AddFormatText("$e%s @ %.0f", pData->GetStringDia(pConcData->m_TSupportBarDia[j], pConcData->m_Fy), pConcData->m_TSupportBarCTC[j]);
		}
		if(bSI) XLText->AddFormatText("$m+03$c=$r$e$f1%.1f$me1$m+02㎟$n$h", dAreaUseT);
		else XLText->AddFormatText("$m+03$c=$r$e$f3%.3f$me1$m+02㎠$n$h", dAreaUseT);
		
		XLText->AddFormatText("$u$h$m+06$-07$n$h");
		if(bSI) XLText->AddFormatText("$m+08$c$∑$r$c=$r$e$f1%.1f$me1$m+02㎟$n$h", dAreaUseTot);
		else XLText->AddFormatText("$m+08$c$∑$r$c=$r$e$f3%.3f$me1$m+02㎠$n$h", dAreaUseTot);
		XLText->AddFormatText("$n$h");
		
		if(pBri->m_bCheckSuppoerBarWall15hs)
		{
			double dMaxH = pData->IsDoro2010() ? 1800 : 1200;
			double dThick = MIN(dMaxH, pConcData->m_Ho);
			double dB = pConcData->m_Bo;
			dAreaReq = (bSI)? 0.0015 * dThick * dB:toCm2(0.0015 * dThick * dB);

			XLText->AddFormatText("$h$r$c-$r$e0.0015hb  [도로교설계기준 4.3.9]$n$h");
			if(pData->IsDoro2010())
				XLText->AddFormatText("$h$r$r$eh : 부재두께$m+04b : 부재 폭$n$h");
			else
				XLText->AddFormatText("$h$r$r$eh : 부재두께 최대 %.0f$m+07b : 부재 폭$n$h", dMaxH);

			XLText->AddFormatText("$h$r$r$r$e필요 철근량$m+03$c:$r$e");

			if(pData->m_nTypeKciCode != 0)	// Kci 2007
			{
				if(bSI) XLText->AddFormatText("0.0015 x %.0f x %.0f$m+07$c=$r$e %.1f ㎟  (≤1800)$n$h", dThick, dB, dAreaReq);
				else XLText->AddFormatText("0.0015 x %.1f x %.1f$m+07$c=$e %.3f ㎠  (≤1800)$n$h", toCm(dThick), toCm(dB), dAreaReq);
				
				dAreaReq = MIN(dAreaReq, (bSI? 1800 : 18));
				szComp = (dAreaReq > dAreaUseTot)? ">":"<";
				szOkNg = (dAreaReq > dAreaUseTot)? "N.G":"O.K";
				if(bSI) XLText->AddFormatText("$h$m+12$c∴$r %.1f ㎟$m+03$c%s$r$r$e$f1%.1f ㎟$m+03$c∴$r$e", dAreaReq, szComp, dAreaUseTot);
				else XLText->AddFormatText("$h$m+12$c∴$r %.3f ㎠$m+03$c%s$r$r$e$f3%.3f ㎠$m+03$c∴$r$e", dAreaReq, szComp, dAreaUseTot);
			}
			else
			{
				if(bSI) XLText->AddFormatText("0.0015 x %.0f x %.0f = %.1f ㎟", dThick, dB, dAreaReq);
				else XLText->AddFormatText("0.0015 x %.1f x %.1f = %.3f ㎠", toCm(dThick), toCm(dB), dAreaReq);
				
				szComp = (dAreaReq > dAreaUseTot)? "＞":"≤";
				szOkNg = (dAreaReq > dAreaUseTot)? "N.G":"O.K";
				if(bSI) XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$m+03$c∴$r$e", szComp, dAreaUseTot);
				else XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$m+03$c∴$r$e", szComp, dAreaUseTot);
			}
			
			if(dAreaReq > dAreaUseTot) XLText->AddFormatText("$#03");
			XLText->AddFormatText("$a%s$n$h", szOkNg);
		}
		
		if(pBri->m_bCheckSuppoerBarWallArea)
		{
			double dAreaSection = pConcData->m_Ho * pConcData->m_Bo;
			dRatio = pBri->m_dCheckSuppoerBarWallArea;
			if(pConcData->m_Fy >= 4000 && pConcData->m_TSupportBarDia[0] <= 16.0)
			{
				dRatio = pBri->m_dCheckSuppoerBarWallAreaForOver400;
			}

			dAreaReq = (bSI)? dAreaSection * dRatio:toCm2(dAreaSection * dRatio);
			dAreaReqSect = dAreaReq;

			if (pData->m_nTypeKciCode == 0 || pData->m_nTypeKciCode == 1)
			{
				XLText->AddFormatText("$h$r$c-$r$e단면적비(온도 및 건조수축 철근) [콘크리트구조설계기준 P.287]$n$h");
			}
			else
			{
				XLText->AddFormatText("$h$r$c-$r$e단면적비(온도 및 건조수축 철근) [콘크리트구조기준 11.3(2)]$n$h");
			}
			XLText->AddFormatText("$h$r$r$e단면적대비 %.4f 이상$n$h", dRatio);
			XLText->AddFormatText("$h$r$r$r$e필요철근량$m+03$c:$r$e%.4f x B x T$n$h$m+06$c=$r$e", dRatio);
			if(bSI) XLText->AddFormatText("%.4f x %.0f x %.0f = %.1f ㎟", dRatio, pConcData->m_Bo, pConcData->m_Ho, dAreaReq);
			else XLText->AddFormatText("%.4f x %.1f x %.1f = %.3f ㎠", dRatio, toCm(pConcData->m_Bo), toCm(pConcData->m_Ho), dAreaReq);

			if(pData->m_nTypeKciCode != 0)
				dAreaReq = MIN(dAreaReq, (bSI ? 1800 : 18));
			szComp = (dAreaReq > dAreaUseTot)? "＞":"≤";
			szOkNg = (dAreaReq > dAreaUseTot)? "N.G":"O.K";
			if(pData->m_nTypeKciCode != 0 && dAreaSection * dRatio >= 1800)	// Kci 2007
			{
				if(bSI)
				{
					XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$n", szComp, dAreaUseTot);
					XLText->AddFormatText("$h$r$r$e필요철근량이 1800㎟보다 클경우 수축,온도 철근 단면적을 단위 m당$n");
					XLText->AddFormatText("$h$r$r$e1800㎟보다 크게 취할 필요가 없다. [콘크리트구조설계기준 5.7.2]$m+19$c∴$r$e%s$n$h", szOkNg);
				}
				else
				{
					XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$n", szComp, dAreaUseTot);
					XLText->AddFormatText("$h$r$r$e필요철근량이 18㎠보다 클경우 수축,온도 철근 단면적을 단위 m당$n");
					XLText->AddFormatText("$h$r$r$e18㎠보다 크게 취할 필요가 없다. [콘크리트구조설계기준 5.7.2]$m+19$c∴$r$e%s$n$h", szOkNg);
				}
			}
			else
			{
				if(bSI) XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$m+03$c∴$r$e", szComp, dAreaUseTot);
				else XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$m+03$c∴$r$e", szComp, dAreaUseTot);
				
				if(dAreaReq > dAreaUseTot) XLText->AddFormatText("$#03");
				XLText->AddFormatText("$a%s$n$h", szOkNg);
			}
		}
		
		if(pBri->m_bCheckSuppoerBarWall300)
		{
			dAreaReq = (bSI)? 300:toCm2(300);
			
			XLText->AddFormatText("$h$r$c-$r$e노출면 300㎟ 이상 [도로교설계기준 4.3.9, 철도설계기준 P.254]$n$h");
			XLText->AddFormatText("$h$r$r$r$e필요철근량$m+03$c:$r$e");
			if(bSI) XLText->AddFormatText("$f1%.1f ㎟", dAreaReq);
			else XLText->AddFormatText("$f3%.3f ㎠", dAreaReq);

			dAreaUseComp = (pConcData->m_sTitle.Find("중앙") != -1 || pConcData->m_sTitle.Find("중간") != -1)? dAreaUseT:dAreaUseC;
			szComp = (dAreaReq > dAreaUseComp)? "＞":"≤";
			szOkNg = (dAreaReq > dAreaUseComp)? "N.G":"O.K";
			if(bSI) XLText->AddFormatText("$h$m+16$c%s$r$r$e$f1%.1f ㎟$m+03$c∴$r$e", szComp, dAreaUseComp);
			else XLText->AddFormatText("$h$m+16$c%s$r$r$e$f3%.3f ㎠$m+03$c∴$r$e", szComp, dAreaUseComp);
			
			if(dAreaReq > dAreaUseComp) XLText->AddFormatText("$#03");
			XLText->AddFormatText("$a%s$n$h", szOkNg);
		}

		if(pBri->m_bCheckSuppoerBarWallArea && pBri->m_bSelectSupportBarWall)
		{
			dAreaUseComp = (pConcData->m_sTitle.Find("중앙") != -1 || pConcData->m_sTitle.Find("중간") != -1)? dAreaUseT:dAreaUseC;

			XLText->AddFormatText("$h$r$c-$r$e노출면 배치 철근량 검토$n$h");
			XLText->AddFormatText("$h$r$r$e노출면에 1/2이상, 2/3이하 배치$n$h");

			if(bSI) XLText->AddFormatText("$h$m+02$c:$r$e%.1f / 2 = %.1f ㎟$m+06", dAreaReqSect, dAreaReqSect/2);
			else XLText->AddFormatText("$h$m+02$c:$r$e%.3f / 2 = %.3f ㎠$m+06", dAreaReqSect, dAreaReqSect/2);
			szComp = (dAreaReqSect/2 < dAreaUseComp)? "<":">";
			if(bSI) XLText->AddFormatText("$c%s$r$e%.1f ㎟$m+03", szComp, dAreaUseComp);
			else XLText->AddFormatText("$c%s$r$e%.3f ㎠$m+03", szComp, dAreaUseComp);
			szComp = (dAreaReqSect*2/3 > dAreaUseComp)? "<":">";
			if(bSI) XLText->AddFormatText("$c%s$r$e%.1f x 2/3 = %.1f ㎟$m+07$c∴$r$e", szComp, dAreaReqSect, dAreaReqSect*2/3);
			else XLText->AddFormatText("$c%s$r$e%.3f x 2/3 = %.3f ㎠$m+07$c∴$r$e", szComp, dAreaReqSect, dAreaReqSect*2/3);

			szOkNg = (dAreaReqSect/2 < dAreaUseComp && dAreaReqSect*2/3 > dAreaUseComp)? "O.K":"N.G";
			if(szOkNg == "N.G") XLText->AddFormatText("$#03");
			XLText->AddFormatText("$a%s$n$h", szOkNg);
		}

		if(pBri->m_bCheckSuppoerBarArchRibMinDia1 || pBri->m_bCheckSuppoerBarArchRibMinDia2)
		{
			XLText->AddFormatText("$h$r$c-$r$e최소 철근직경 검토$n$h");
			
			double dDiamin = (pData->m_nTypeConditionApply == DESIGN_CONDITION_RAIL)? 6 : 13;
			double dRateFactor = (pData->m_nTypeConditionApply == DESIGN_CONDITION_RAIL)? 4 : 3;
			if(pBri->m_bCheckSuppoerBarArchRibMinDia1)
			{
				XLText->AddFormatText("$h$r$r$r$e최소 철근직경 %.0fmm이상$n$h", dDiamin);
				
				szComp = (pConcData->m_CSupportBarDia[0] >= dDiamin)? "≥":"<";
				szOkNg = (pConcData->m_CSupportBarDia[0] >= dDiamin)? "O.K":"N.G";

				XLText->AddFormatText("$h$r$r$r$c$f0%.0fmm$me1$m+02$c%s$r$c$me1%.0fmm$m+15$c∴$r$e", pConcData->m_CSupportBarDia[0], szComp, dDiamin);
				if(szOkNg == "N.G") XLText->AddFormatText("$#03");
				XLText->AddFormatText("$a%s$n$h", szOkNg);
			}
			if(pBri->m_bCheckSuppoerBarArchRibMinDia2)
			{
				XLText->AddFormatText("$h$r$r$r$e주철근 직경의 1/%.0f이상$n$h", dRateFactor);
				
				szComp = (pConcData->m_CSupportBarDia[0] >= pConcData->m_RbC1C_DIA[0]/dRateFactor)? "≥":"<";
				szOkNg = (pConcData->m_CSupportBarDia[0] >= pConcData->m_RbC1C_DIA[0]/dRateFactor)? "O.K":"N.G";
				
				XLText->AddFormatText("$h$r$r$r$c$f0%.0fmm$me1$m+02$c%s$r$c$me2%.0f / %.0f$m+03$c=$r$me1%.1fmm$m+11$c∴$r$e", pConcData->m_CSupportBarDia[0], szComp, pConcData->m_RbC1C_DIA[0], dRateFactor, pConcData->m_RbC1C_DIA[0]/dRateFactor);
				if(szOkNg == "N.G") XLText->AddFormatText("$#03");
				XLText->AddFormatText("$a%s$n$h", szOkNg);
			}

			
			double dCtc = MIN(pConcData->m_RbT1C_DIA[0]*15, pBri->GetThickArchRib(0, TRUE));
			XLText->AddFormatText("$h$r$c-$r$e최대 철근간격 검토$n$h");
			XLText->AddFormatText("$h$r$r$r$e - 축방향주철근지름의 15배  = %.0f * 15 = %.0fmm$n$h", pConcData->m_RbT1C_DIA[0], pConcData->m_RbT1C_DIA[0]*15);
			XLText->AddFormatText("$h$r$r$r$e - 아치리브 단면의 최소치수 = %.0fmm$n$h", pBri->GetThickArchRib(0, TRUE));
			XLText->AddFormatText("$h$r$r$r$e최대 철근간격 %.0fmm이하$n$h", dCtc);
			
			szComp = (pConcData->m_CSupportBarCTC[0] >= dCtc) ? "≥":"<";
			szOkNg = (pConcData->m_CSupportBarCTC[0] <= dCtc) ? "O.K":"N.G";
			
			XLText->AddFormatText("$h$r$r$r$c$f0%.0fmm$me1$m+02$c%s$r$c$me1%.0fmm$m+15$c∴$r$e", pConcData->m_CSupportBarCTC[0], szComp, dCtc);
			if(szOkNg == "N.G") XLText->AddFormatText("$#03");
			XLText->AddFormatText("$a%s$n$h", szOkNg);
		}
		XLText->AddFormatText("$n$h");
	}
	XLText->AddFormatText("$n$h");
}

double CARcBridgeCalcCornerAndSupport::GetSupportBarSlabRatio(double dFy)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	return dFy > 4000 ? 0.002*4000./dFy : pBri->m_dCheckSuppoerBarSlabArea;
}

double CARcBridgeCalcCornerAndSupport::GetSupportBarFootRatio(double dFy)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	return dFy > 4000 ? 0.002*4000./dFy : pBri->m_dCheckSuppoerBarFootArea;
}

// void CARcBridgeCalcCornerAndSupport::MakeTableSupportSumArchrib(CXLFormatText *XLText, long nIndexSub)
// {
// 	
// }

// double CARcBridgeCalcCornerAndSupport::GetDoubleCheckConcArchRib(CConcBasic *pConcData, BOOL bReq, long nIndex)
// {
// 	double dRet = 0;
// 	double dAreaUse[2];
// 	double dAreaReq[2];
// 
// 	// 최소 철근직경, 13mm
// 	dAreaReq[0] = 13.0;
// 	dAreaUse[0] = pConcData->m_CSupportBarDia[0];
// 	
// 	// 최소 철근직경, 주철근 1/3
// 	dAreaReq[1] = pConcData->m_RbC1C_DIA[0]/3;
// 	dAreaUse[1] = pConcData->m_CSupportBarDia[0];
// 	
// 	dRet = (bReq)? dAreaReq[nIndex]:dAreaUse[nIndex];
// 	
// 	return dRet;
// }

// CString CARcBridgeCalcCornerAndSupport::GetStringCheckConcArchRib(CConcBasic *pConcData, BOOL bReq, long nIndex, BOOL bOption)
// {
// 	CRcBridgeRebar *pBri = m_pStd->m_pBri;
// 	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
// 	
// 	BOOL bOK = TRUE;
// 	CString szRet = _T("");
// 	double dAreaUse[3];
// 	double dAreaReq[3];
// 	
// 	// 최소 철근직경, 13mm
// 	dAreaReq[0] = 13.0;
// 	dAreaUse[0] = pConcData->m_CSupportBarDia[0];
// 
// 	// 최소 철근직경, 주철근 1/3
// 	dAreaReq[1] = pConcData->m_RbC1C_DIA[0]/3;
// 	dAreaUse[1] = pConcData->m_CSupportBarDia[0];
// 	
// 	if(pBri->m_bCheckSuppoerBarArchRibMinDia1 && (dAreaReq[0] > dAreaUse[0])) bOK = FALSE;
// 	if(pBri->m_bCheckSuppoerBarArchRibMinDia2 && (dAreaReq[1] > dAreaUse[1])) bOK = FALSE;
// 	
// 	if(nIndex == 2) szRet = (bOK)? "O.K":(bOption)? "$#03N.G":"N.G";
// 	else szRet.Format("$f0%.0f", bReq? dAreaReq[nIndex] : dAreaUse[nIndex]);
// 	
// 	return szRet;
// }
