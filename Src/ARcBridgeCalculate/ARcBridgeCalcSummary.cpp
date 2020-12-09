// ARcBridgeCalcSummary.cpp: implementation of the CARcBridgeCalcSummary class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "ARcBridgeCalculate.h"

#define LSD_CHECK_AXIAL_COEF		0.66
#define LSD_CHECK_AXIALMOMENT_COEF	1.00

#define AREA_A		0
#define AREA_B		1
#define AREA_C		2
#define AREA_D		3
#define AREA_A_		4
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARcBridgeCalcSummary::CARcBridgeCalcSummary()
{

}

CARcBridgeCalcSummary::~CARcBridgeCalcSummary()
{

}

void CARcBridgeCalcSummary::WriteCalcResultSummary(CXLFormatText *XLText, CRcBridgeRebar *pBri)
{
	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	XLText->GoPosition(NO_MOVE, 0);
	XLText->AddFormatText("$h$tb13¢Ã ÇØ¼®°á°ú ¿ä¾à$n");
	XLText->GoPosition(NO_MOVE, COL_POS_1);
	*XLText += 1;

	BOOL bCheckMidWall = (pBri->m_nSelectInWallSectionDesign == 1 && pBri->GetCountInWall() > 0) ? TRUE : FALSE;

	MakeTableCheckFootAndPile(XLText);
	
	if(bLsd)
	{
		MakeTableCheckBeding_Lsd(XLText);
		MakeTableCheckCrack_Lsd(XLText);
		MakeTableCheckSupport_Lsd(XLText);
		MakeTableCheckShear_Lsd(XLText);
		
		if( bCheckMidWall )
			MakeTableCheckInSideWall_Lsd(XLText);
	}
	else
	{
		MakeTableCheckBeding(XLText);
		MakeTableCheckCrack(XLText);
		MakeTableCheckSupport(XLText);
		MakeTableCheckShear(XLText);

		if( bCheckMidWall )
			MakeTableCheckInSideWall(XLText);
	}

// 	if(pBri->m_nSelectInWallSectionDesign == 1 && pBri->GetCountInWall() > 0)
// 		MakeTableCheckInSideWall(XLText);
	XLText->GoPosition(NO_MOVE, 0);
	XLText->AddFormatText("$s+99$h$tb13¢Ã ÁÖÃ¶±Ù Á¶¸³µµ$n");
	XLText->GoPosition(NO_MOVE, COL_POS_1);
	DrawSectionMainRebar(XLText, 0);
	DrawSectionMainRebar(XLText, 1);
	*XLText += 1;
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB && (pBri->IsWingWall(TRUE, TRUE)||pBri->IsWingWall(TRUE, FALSE)||pBri->IsWingWall(FALSE, TRUE)||pBri->IsWingWall(FALSE, FALSE)))
	{
		long nTypeJongdanForRebarOrg = pBri->m_nTypeJongdanForRebar;
		CTwinVectorArray tvArrVertSection;
		tvArrVertSection	= pBri->m_tvArrVertSection;
		pBri->m_nTypeJongdanForRebar = 2;

		XLText->GoPosition(NO_MOVE, 0);
		XLText->AddFormatText("$s+99$h$tb13¢Ã ÁÖÃ¶±Ù Á¶¸³µµ$n");
		*XLText += 1;
		XLText->GoPosition(NO_MOVE, COL_POS_1);
		if(pBri->IsWingWall(TRUE, TRUE))
		{
			XLText->AddFormatText("$s+23$h$c-$r$e½ÃÁ¡ÁÂÃø ³¯°³º®$n");
			DrawWingWallMainRebar(XLText, TRUE, TRUE);
		}
		if(pBri->IsWingWall(TRUE, FALSE))
		{
			XLText->AddFormatText("$s+23$h$c-$r$e½ÃÁ¡¿ìÃø ³¯°³º®$n");
			DrawWingWallMainRebar(XLText, TRUE, FALSE);
		}
		if(pBri->IsWingWall(FALSE, TRUE))
		{
			XLText->AddFormatText("$s+23$h$c-$r$eÁ¾Á¡ÁÂÃø ³¯°³º®$n");
			DrawWingWallMainRebar(XLText, FALSE, TRUE);
		}
		if(pBri->IsWingWall(FALSE, FALSE))
		{
			XLText->AddFormatText("$s+23$h$c-$r$eÁ¾Á¡¿ìÃø ³¯°³º®$n");
			DrawWingWallMainRebar(XLText, FALSE, FALSE);
		}

		pBri->m_nTypeJongdanForRebar	= nTypeJongdanForRebarOrg;
		pBri->m_tvArrVertSection		= tvArrVertSection;
	}
}

void CARcBridgeCalcSummary::MakeTableCheckFootAndPileBoxType_Usd(CXLFormatText *XLText)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(pBri->IsBoxType() == FALSE) 
	{
		return;
	}

	BOOL bLsd = pData->IsLsdDesign();
	long nJijum = 0;
	BOOL bApplyEarthQauke = pBri->IsApplyEarthQuakeDesign();
	BOOL bPileBase = pBri->IsPileBase(nJijum);
	BOOL bPilePHC	= bPileBase? pBri->m_footingBox.m_exFooting.m_Pile.IsPHC() : FALSE;
	BOOL bPileHyunta = bPileBase? (pBri->m_footingBox.m_exFooting.m_Pile.m_nType == PILE_TYPE_HYUNJANG) : FALSE;
	CARcBridgeCalcFooting *pFoot = m_pStd->m_pFooting;

	long nCntRow = bPileBase ? (bPilePHC? 6 : (bPileHyunta? 4 : 5)) : 1;
	if(bApplyEarthQauke && bPileBase)
	{
		nCntRow *= 2;
	}

	XLText->AddFormatText(bPileBase ? "$h$e¡á ¸»¶Ò °ËÅä$n$h" : "$h$e¡á ±âÃÊ °ËÅä$n$h");
	XLText->AddTable(nCntRow, 25-1, 1, 9);

	long nWidthTitle[] = { 11, 3, 3, 3, 3, 2 };
	char *szTitle[] = { "±¸  ºÐ", "Çã¿ë°ª", "¹ß»ý°ª", "¾ÈÀüÀ²", "Çã¿ë¾ÈÀüÀ²", "ºñ °í"};
	for(long i=0; i<6; i++)
	{
		XLText->AddMergeFormatText(0, nWidthTitle[i]-1, "$c%s", szTitle[i]);
		XLText->AddFormatText("$m+%02d", nWidthTitle[i]);
	}
	XLText->AddFormatText("$n$h");

	CStringArray strArr;
	CString str = _T("");
	if(bPileBase)
	{
		str.Format("  Çã¿ëÁöÁö·Â(%s/EA)", pData->m_szTonf);
		strArr.Add(str);
		str.Format("  Çã¿ëÀÎ¹ß·Â(%s/EA)", pData->m_szTonf);
		strArr.Add(str);
		if(bPileHyunta)
		{
			str.Format("  Ãà¹æÇâ°­µµ(%s)", pData->m_szTonf);
			strArr.Add(str);
			strArr.Add(_T(""));
			strArr.Add(_T(""));
		}
		else
		{
			if(bPilePHC)
				str.Format("  ÈÚ¾ÐÃàÀÀ·Â(%s)", pData->m_szKgf_CM2);
			else
				str.Format("  ÈÚÀÀ·Â(%s)", pData->m_szKgf_CM2);
			strArr.Add(str);
			str.Format("  ÈÚÀÎÀåÀÀ·Â(%s)", pData->m_szKgf_CM2);
			strArr.Add(str);
			str.Format("  Àü´ÜÀÀ·Â(%s)", pData->m_szKgf_CM2);
			strArr.Add(str);
		}
		
		str.Format("  ¼öÆòº¯À§(%s)", (pData->IsSI())? "mm":"cm");
		strArr.Add(str);
	}
	else
	{
		str.Format("ÁöÁö·Â¿¡´ëÇÑ ¾ÈÁ¤(%s)", pData->m_szTonf_M2);
		strArr.Add(str);
	}

	double dApply = 0, dUse = 0, dSF = 0, dSFa = 0;
	BOOL bOK = FALSE;
	CString szOkNg = _T("");
	if(bPileBase) 
	{
		long nWidthData[] = {4, 7, 3, 3, 3, 3, 2 };
		long nCntItem = bApplyEarthQauke ? 12 : 6;
		long j=0; for(j=0; j<nCntItem; j++)
		{
			BOOL bNormal = (j<6) ? TRUE:FALSE; 
			if(bNormal==FALSE && bApplyEarthQauke==FALSE)
			{
				break;
			}
			long nId = (j > 5)? j-6 : j;

			if(bPilePHC == FALSE && nId == 3)
				continue;
			if(bPileHyunta && nId == 4)
				continue;

			if(nId == 0)
			{
				pFoot->GetCheckValuePileForce(nJijum, &dApply, &dUse, bNormal);
				dSF		= fabs(dApply/dUse);
				dSFa	= 1.0;
				szOkNg	= (dSF > dSFa)? "O.K":"$#03N.G";
			}
			else if(nId == 1)
			{
				pFoot->GetCheckValuePileForceMinus(nJijum, &dApply, &dUse, bNormal);
				dSF		= fabs(dApply/dUse);
				dSFa	= 1.0;
				BOOL bOK = (fabs(dUse) < dApply || dUse >= 0) ? TRUE : FALSE;
				szOkNg	= (bOK)? "O.K":"$#03N.G";
			}
			else if(nId == 2 || nId == 3)
			{
				if(bPileHyunta)
				{
					pFoot->GetCheckValueHyuntaPileAxialForce(nJijum, &dApply, &dUse, bNormal);
				}
				else
				{
					pFoot->GetCheckValuePileBending(nJijum, &dApply, &dUse, bNormal, nId == 3);
				}
				dSF		= fabs(dApply/dUse);
				dSFa	= 1.0;
				szOkNg	= (dUse == 0 || dSF > dSFa)? "O.K":"$#03N.G";
			}
			else if(nId == 4)
			{
				pFoot->GetCheckValuePileShear(nJijum, &dApply, &dUse, bNormal);
				dSF		= fabs(dApply/dUse);
				dSFa	= 1.0;
				szOkNg	= (dSF > dSFa)? "O.K":"$#03N.G";
			}
			else if(nId == 5)
			{
				pFoot->GetCheckValuePileDisplacement(nJijum, &dApply, &dUse, bNormal);
				dSF		= fabs(dApply/dUse);
				dSFa	= 1.0;
				szOkNg	= (dSF > dSFa)? "O.K":"$#03N.G";
			}

			for(long k=0; k<7; k++)
			{
				if(k==0)
				{
					if(j==0 || j==6) XLText->AddFormatText("$md%d", bPilePHC? 5 : (bPileHyunta? 3 : 4));
					if(bLsd)
						str = !bNormal ? "±Ø´Ü»óÈ²ÇÑ°è»óÅÂ":"»ç¿ëÇÑ°è»óÅÂ";
					else
						str = !bNormal ? "ÁöÁø½Ã":"Æò»ó½Ã";
				}
				else if(k==1) str = strArr.GetAt(nId);
				else if(k==2)
				{
					if(bLsd && nId==0)
						str = GetStrDouble(dApply, 0, TRUE);
					else
						str.Format("$f3%.3f", dApply);
				}
				else if(k==3)
				{
					if((nId==1 && dUse>=0)) str = _T("-");
					else if(nId==2 && dUse<=0)  str = _T("-");
					else if(nId==3 && dUse>=0)  str = _T("-");
					else if(bLsd && nId==0)
						str = GetStrDouble(dUse, 0, TRUE);
					else str.Format("$f3%.3f", dUse);
				}
				else if(k==4)
				{
					if((nId==1 && dUse>=0) || (nId==4 && dUse==0)) str = _T("-");
					else if(nId==2 && dUse<=0)  str = _T("-");
					else if(nId==3 && dUse>=0)  str = _T("-");
					else str.Format("$f3%.3f", dSF);
				}
				else if(k==5)
				{
					if((nId==1 && dUse>=0) || (nId==4 && dUse==0)) str = _T("-");
					else if(nId==2 && dUse<=0)  str = _T("-");
					else if(nId==3 && dUse>=0)  str = _T("-");
					else str.Format("$f3%.3f", dSFa);
				}
				else if(k==6) 
				{
					str = szOkNg;
				}
				else
				{
					str = _T("");
				}

				XLText->AddMergeFormatText(0, nWidthData[k]-1, k==1 ? "$e%s" : "$c%s", str);
				XLText->AddFormatText("$m+%02d", nWidthData[k]);
			}
			XLText->AddFormatText("$n$h");
		}
	}
	else 
	{
		long nWidthData[] = {11, 3, 3, 3, 3, 2 };
		bOK		= pFoot->GetCheckValueSupport(nJijum, &dApply, &dUse, TRUE);
		dSF		= dApply/dUse;
		dSFa	= 1.0;
		szOkNg	= (bOK) ? "O.K":"$#03N.G";

		long nId = 0;
		for(long k=0; k<6; k++)
		{
			if(k==0) str = strArr.GetAt(nId);
			else if(k==1) str.Format("$f3%.3f", dApply);
			else if(k==2) str.Format("$f3%.3f", dUse);
			else if(k==3)
			{
				if(dUse > 0) str.Format("$f3%.3f", dSF);
				else str = _T("¡Ä");
			}
			else if(k==4) str.Format("$f3%.3f", dSFa);
			else if(k==5) str = szOkNg;
			else str = _T("");

			XLText->AddMergeFormatText(0, nWidthData[k]-1, "$c%s", str);
			XLText->AddFormatText("$m+%02d", nWidthData[k]);
		}
		XLText->AddFormatText("$n$h"); 
	}

	*XLText += 1;
}

void CARcBridgeCalcSummary::MakeTableCheckFootAndPileBoxType_Lsd( CXLFormatText *XLText )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(pBri->IsBoxType() == FALSE) 
	{
		return;
	}

	long nJijum = 0;
	BOOL bPileBase = pBri->IsPileBase(nJijum);
	CARcBridgeCalcFooting *pFoot = m_pStd->m_pFooting;

	CLsdFootingDesign* pLsdFooting = m_pStd->m_pFooting->GetLsdFootingDesign(nJijum);
	if(pLsdFooting == NULL) return;

	BOOL bOK = FALSE;
	CString szOkNg = _T("");
	CString str = _T("");


	if( bPileBase )
	{
		XLText->AddFormatText("$h$e¡á ¸»¶Ò °ËÅä$n$h");

		CString str = _T(""), str1 = _T(""), str2 = _T(""), str3 = _T(""), strOk  = _T("");

		CDesignPile *pCalcPile = m_pStd->m_pArrPileCalc.GetAt(nJijum);
		if( pCalcPile == NULL ) return;

		long nSizeCombo = 0;

		HGBOOL bBoredPileASD = (pCalcPile->m_bApplyBoredPileASD && pCalcPile->m_pPile->IsBoredPile());

		//////////////////////¸»¶Ò ÁöÁö·Â///////////////////
		nSizeCombo += pCalcPile->GetSizePileLoad(PILE_CASE_USE, TRUE);
		nSizeCombo += bBoredPileASD? 0 : pCalcPile->GetSizePileLoad(PILE_CASE_ULTIMATE, TRUE);
		nSizeCombo += pCalcPile->GetSizePileLoad(PILE_CASE_EXTREME, TRUE);

		if(nSizeCombo == 0) return;

		XLText->AddTable(nSizeCombo, 13, 1, 5);

		XLText->AddFormatText(_T("$h$c$me5%s$m+06$c$me2%s$m+03$c$me2%s$m+03$c$me1%s$n$h"),
			hggettext("±¸ ºÐ"),hggettext("Vmax(N)"),hggettext("Qr(N)"),hggettext("ºñ°í"));		
		XLText->AddMergeText(nSizeCombo-1, 0, hggettext("$h$cÁöÁö·Â"));

		for(long nTypePileCase = 0; nTypePileCase < 3; nTypePileCase++)
		{
			// (ARCBRIDGE-2604) ¸ÅÀÔ¸»¶Ò Çã¿ëÀ»·ÂÀ¸·Î °ËÅä½Ã ±ØÇÑÇÑ°è»óÅÂ Ãâ·Â ¾ÈÇÔ.
			if(bBoredPileASD && nTypePileCase == PILE_CASE_ULTIMATE)
				continue;

			CString strEnv(_T(""));
			if( nTypePileCase == PILE_CASE_ULTIMATE ) strEnv = "±ØÇÑ";
			else if( nTypePileCase == PILE_CASE_EXTREME ) strEnv = "±Ø´Ü";
			else strEnv = "»ç¿ë";

			long nCntCombo = pCalcPile->GetSizePileLoad(nTypePileCase, TRUE);
			for (long nCombo =0; nCombo< nCntCombo; nCombo++)
			{
				CPileLoad *pLoad = pCalcPile->GetPileLoad(nTypePileCase, nCombo, TRUE);

				long nIdxOrg = nCombo % pCalcPile->m_pPile->m_nCountOrg;
				double dDistPile = pFoot->GetDistMaxPile(nIdxOrg);
				double dVmaxFix =  GetValueUnitChange(pLoad->m_dForceVert * dDistPile, UNIT_CGS_TONF, pData->m_szTonf);
				double dVmaxHin =  0;
				double dVmax = max(dVmaxFix, dVmaxHin);
				double dRa = pCalcPile->m_Ra[nTypePileCase];

				strOk	= (dVmax > dRa)? _T("$#03N.G") : _T("O.K");
				str		= GetStrDouble(dVmax, 0, TRUE);
				str1	= GetStrDouble(dRa, 0, TRUE);

				if(nCombo == 0)
				{
					XLText->AddFormatText(_T("$h$c$me1%s$m+02"), hggettext("ÁöÁö·Â"));
					XLText->AddMergeFormatText(nCntCombo-1, 1, _T("$c$me1%s$m+02"), strEnv);
					XLText->AddFormatText(_T("$c$me1%s$m+02$c$me2%s$m+03$c$me2%s$m+03$c$me1%s$n$h"), pLoad->m_sName, str, str1, strOk);
				}
				else
				{
					XLText->AddFormatText(_T("$h$c$me1%s$m+04$c$me1%s$m+02$c$me2%s$m+03$c$me2%s$m+03$c$me1%s$n$h"),
						hggettext("ÁöÁö·Â"),  pLoad->m_sName, str, str1, strOk);		
				}		
			}
		}
		XLText->AddFormatText(_T("$n$h"));

		//////////////////////¸»¶Ò ÀÎ¹ß·Â///////////////////
		XLText->AddFormatText(_T("$s+%02d"), nSizeCombo+5);

		XLText->AddTable(nSizeCombo, 13, 1, 5);				

		XLText->AddFormatText(_T("$h$c$me5%s$m+06$c$me2%s$m+03$c$me2%s$m+03$c$me1%s$n$h"),
			hggettext("±¸ ºÐ"),hggettext("Vmin(N)"),hggettext("Qu(N)"),hggettext("ºñ°í"));		
		XLText->AddMergeText(nSizeCombo-1, 0, hggettext("$h$cÀÎ¹ß·Â"));

		for(long nTypePileCase = 0; nTypePileCase < 3; nTypePileCase++)
		{
			if(bBoredPileASD && nTypePileCase == PILE_CASE_ULTIMATE)
				continue;

			CString strEnv(_T(""));
			if( nTypePileCase == PILE_CASE_ULTIMATE ) strEnv = "±ØÇÑ";
			else if( nTypePileCase == PILE_CASE_EXTREME ) strEnv = "±Ø´Ü";
			else strEnv = "»ç¿ë";

			long nCntCombo = pCalcPile->GetSizePileLoad(nTypePileCase, TRUE);
			for (long nCombo =0; nCombo< nCntCombo; nCombo++)
			{
				CPileLoad *pLoad = pCalcPile->GetPileLoad(nTypePileCase, nCombo, TRUE);

				long nIdxOrg = nCombo % pCalcPile->m_pPile->m_nCountOrg;
				double dDistPile = pFoot->GetDistMaxPile(nIdxOrg);
				double dVmaxFix =  GetValueUnitChange(pLoad->m_dForceVert * dDistPile, UNIT_CGS_TONF, pData->m_szTonf);
				double dVmaxHin =  0;
				double dVmin = min(dVmaxFix, dVmaxHin);

				double dRa = pCalcPile->m_Ra[nTypePileCase];
				double dPa = pCalcPile->m_Pa[nTypePileCase];

				strOk	= (dVmin > dRa)? _T("$#03N.G") : _T("O.K");
				str		= GetStrDouble(dVmin, 0, TRUE);
				str1	= GetStrDouble(dPa, 0, TRUE);


				if(nCombo == 0)
				{
					XLText->AddFormatText(_T("$h$c$me1%s$m+02"), hggettext("ÀÎ¹ß·Â"));
					XLText->AddMergeFormatText(nCntCombo-1, 1, _T("$c$me1%s$m+02"), strEnv);
					XLText->AddFormatText(_T("$c$me1%s$m+02$c$me2%s$m+03$c$me2%s$m+03$c$me1%s$n$h"), pLoad->m_sName, str, str1, strOk);
				}
				else
				{
					XLText->AddFormatText(_T("$h$c$me1%s$m+04$c$me1%s$m+02$c$me2%s$m+03$c$me2%s$m+03$c$me1%s$n$h"),
						hggettext("ÀÎ¹ß·Â"),  pLoad->m_sName, str, str1, strOk);		
				}	

			}
		}
		XLText->AddFormatText(_T("$n$h"));

		//////////////////////¼öÆò º¯À§///////////////////	
		nSizeCombo = pCalcPile->GetSizePileLoad(PILE_CASE_USE, TRUE);		//»ç¿ëÇÑ°è»óÅÂ

		XLText->AddFormatText(_T("$s+%02d"), nSizeCombo+5);

		XLText->AddTable(nSizeCombo, 13, 1, 5);

		XLText->AddFormatText(_T("$h$c$me5%s $m+06$c$me2%s $m+03$c$me2%s $m+03$c$me1%s $n$h"),
			hggettext("±¸ ºÐ"),hggettext("¡â(mm)"),hggettext("¡âa(mm)"),hggettext("ºñ°í"));		
		XLText->AddMergeText(nSizeCombo-1, 0, hggettext("$h$c¼öÆòº¯À§"));

		CString sDelta = _T(""), sDisa = _T("");

		long nCntCombo = pCalcPile->GetSizePileLoad(PILE_CASE_USE, TRUE);
		for (long nCombo =0; nCombo< nCntCombo; nCombo++)
		{
			long nIdxOrg(0);
			CPileLoad *pLoad = pCalcPile->GetPileLoad(PILE_CASE_USE, nCombo, TRUE, nIdxOrg);
			if(pLoad == NULL) continue;

			//double deltaX	= max(fabs(pCalcPile->m_EqFix_deltaX[PILE_CASE_USE].GetAt(nCombo)),fabs(pCalcPile->m_EqHin_deltaX[PILE_CASE_USE].GetAt(nCombo)));
			double deltaX	= pFoot->m_dArrDispX[PILE_CASE_USE].GetAt(nIdxOrg);
			double dDisa	= frCm(pCalcPile->m_Disa[0]*100);

			sDelta	= GetStrDouble(deltaX, 3, TRUE, TRUE);
			sDisa	= GetStrDouble(dDisa, 3, TRUE, TRUE);
			strOk	= deltaX > dDisa? _T("$#03N.G") : _T("O.K");

			if(nCombo == 0)
			{
				XLText->AddFormatText(_T("$h$c$me1%s$m+02"), hggettext("¼öÆò\nº¯À§"));
				XLText->AddMergeFormatText(nCntCombo-1, 1, _T("$c$me1%s$m+02"), hggettext("»ç¿ë"));
				XLText->AddFormatText(_T("$c$me1%s$m+02$c$me2%s$m+03$c$me2%s$m+03$c$me1%s$n$h"), pLoad->m_sName, str, str1, strOk);
			}
			else
			{
				XLText->AddFormatText(_T("$h$c$me1%s$m+04$c$me1%s$m+02$c$me2%s$m+03$c$me2%s$m+03$c$me1%s$n$h"),
					hggettext("¼öÆò\nº¯À§"),  pLoad->m_sName, str, str1, strOk);		
			}	
		}
		XLText->AddFormatText(_T("$n$h"));		

		//////////////////////Ä§ÇÏ·®///////////////////
		nSizeCombo = pCalcPile->GetSizePileLoad(PILE_CASE_USE, TRUE);		//»ç¿ëÇÑ°è»óÅÂ

		XLText->AddFormatText(_T("$s+%02d"), nSizeCombo+5);

		XLText->AddTable(nSizeCombo, 13, 1, 5);

		XLText->AddFormatText(_T("$h$c$me5%s $m+06$c$me2%s $m+03$c$me2%s $m+03$c$me1%s $n$h"),
			hggettext("±¸ ºÐ"),hggettext("¡â(mm)"),hggettext("¡âa(mm)"),hggettext("ºñ°í"));		
		XLText->AddMergeText(nSizeCombo-1, 0, hggettext("$h$cÄ§ÇÏ·®"));

		for (long nCombo =0; nCombo< nSizeCombo; nCombo++)
		{
			CPileLoad *pLoad = pCalcPile->GetPileLoad(PILE_CASE_USE, nCombo, TRUE);
			if(pLoad == NULL) continue;

			double dSubsidence	= pFoot->m_dArrDispZ[PILE_CASE_USE].GetAt(nCombo);
			//double dSubsidence = pCalcPile->GetLsdSubsidenceByCombo(nCombo);
			double dSa		   = 15; //¸Â´Â º¯¼ö¸¦ Ã£À» °Í

			str		= GetStrDouble(dSubsidence, 3, TRUE, TRUE);
			str1	= GetStrDouble(dSa, 3, TRUE, TRUE);
			strOk	= dSubsidence > dSa? _T("$#03N.G") : _T("O.K");


			if(nCombo == 0)
			{
				XLText->AddFormatText(_T("$h$c$me1%s$m+02"), hggettext("Ä§ÇÏ·®"));
				XLText->AddMergeFormatText(nCntCombo-1, 1, _T("$c$me1%s$m+02"), hggettext("»ç¿ë"));
				XLText->AddFormatText(_T("$c$me1%s$m+02$c$me2%s$m+03$c$me2%s$m+03$c$me1%s$n$h"), pLoad->m_sName, str, str1, strOk);
			}
			else
			{
				XLText->AddFormatText(_T("$h$c$me1%s$m+04$c$me1%s$m+02$c$me2%s$m+03$c$me2%s$m+03$c$me1%s$n$h"),
					hggettext("Ä§ÇÏ·®"),  pLoad->m_sName, str, str1, strOk);		
			}	
		}
		XLText->AddFormatText(_T("$n$h"));

//		if(pCalcPile->m_pPile->m_nType != PILE_TYPE_HYUNJANG && bBoredPileASD)
		if(pCalcPile->m_pPile->m_nType == PILE_TYPE_STEEL || pCalcPile->m_pPile->m_nType == PILE_TYPE_STEEL_DIGGING)
		{
			//////////////////////Ãà·Â, ®‚¾ÐÃà°ú ÈÚ, Àü´ÜÀÀ·Â///////////////////
			nSizeCombo = 0;
			nSizeCombo += pCalcPile->GetSizePileLoad(PILE_CASE_ULTIMATE, TRUE);	    //±ØÇÑÇÑ°è
			nSizeCombo += pCalcPile->GetSizePileLoad(PILE_CASE_EXTREME, TRUE);	//±Ø´Ü»óÈ²ÇÑ°è	
			nSizeCombo += 1;
			XLText->AddFormatText(_T("$s+%02d"), nSizeCombo+5);

			XLText->AddTable(nSizeCombo, 24, 2, 4);

			XLText->AddFormatText(_T("$h$c$me4$md1%s $m+05$c$me5%s $m+06$c$me5%s $m+06$c$me5%s $m+06$c$me1$md1%s$n$h"),
				hggettext("±¸ ºÐ"),hggettext("Ãà·Â"),hggettext("Ãà¾ÐÃà°ú ÈÚ"),hggettext("Àü´ÜÀÀ·Â"), hggettext("ºñ°í"));
			XLText->AddFormatText(_T("$h$c$me4%s $m+05$c$me2%s $m+03$c$me2%s $m+03$c$me2%s $m+03$c$me2%s $m+03$c$me2%s $m+03$c$me2%s $m+03$c$me1%s$n$h"),
				hggettext("±¸ ºÐ"),hggettext("Pu(N)"),hggettext("Pr(N)"),hggettext("Pu/Pr"),hggettext("°á°ú"),hggettext("H(N)"),hggettext("Vr"), hggettext("ºñ°í"));

			XLText->AddMergeText(nSizeCombo-2, 0, hggettext("$h$cº»Ã¼°ËÅä"));

			XLText->OutXL();

			for(long nTypePileCase = PILE_CASE_ULTIMATE; nTypePileCase < PILE_CASE_COUNT; nTypePileCase++)
			{
				CString strEnv(_T(""));
				if( nTypePileCase == PILE_CASE_ULTIMATE ) strEnv = "±Ø\nÇÑ";
				else if( nTypePileCase == PILE_CASE_EXTREME ) strEnv = "±Ø\n´Ü";
				else strEnv = "»ç\n¿ë";

				long nSize = pCalcPile->GetSizePileLoad(nTypePileCase, TRUE);
				for(long row=0; row<nSize; row++) //row
				{
					long nCombo =  row; //(long)(row/2.0);

					CPileLoad *pLoad = pCalcPile->GetPileLoad(nTypePileCase, nCombo, TRUE);
					if(pLoad == NULL) continue;

					double dDistPile = pFoot->GetDistMaxPile(nCombo);
					////////////////////////////// (Ãà·Â) ////////////////////////
					//Pu 
					double  dPu		= GetValueUnitChange(fabs(pLoad->m_dForceVert * dDistPile), UNIT_CGS_TONF, pData->m_szTonf);
					CString sDisPuMax	= GetStrDouble(dPu, 0 ,TRUE);

					//Pr
					double  dDisPr  = pCalcPile->GetLsdAxialPr(nTypePileCase);
					CString sDisPrMax = GetStrDouble(dDisPr, 0 ,TRUE);

					/////////////////////// Ãà ¾ÐÃà°ú Èû /////////////////////////
					// Pu/Pr
					double dPuPr	=  (dPu == 0 || dDisPr == 0) ? 0 : dPu / dDisPr;
					double dMu = GetValueUnitChange(pLoad->m_dMoment * dDistPile, UNIT_CGS_TONFM, pData->m_szTonfM);
					double dMr = pCalcPile->GetLsdAxialMomentMr(nTypePileCase);
					double dResult = 0;
					if(dPu / dDisPr < 0.2)
					{
						dResult = dPu / (2 * dDisPr) + dMu / dMr;
					}
					else
					{
						dResult = dPu / dDisPr + (8./9.) * dMu / dMr;
					}
					CString sPuPr, sResult;
					sPuPr.Format(_T("%.3f"), dPuPr);
					sResult.Format(_T("%.3f"), dResult);

					//////////////////// Àü´ÜÀÀ·Â ////////////////////
					double dH	= GetValueUnitChange(fabs(pLoad->m_dForceHori * dDistPile), UNIT_CGS_TONF, pData->m_szTonf);
					double dVr	= max(pCalcPile->GetLsdShearVr(nTypePileCase, nCombo, TRUE),  pCalcPile->GetLsdShearVr(nTypePileCase, nCombo, FALSE));

					CString sH = GetStrDouble(dH, 0, TRUE);
					CString sVr = GetStrDouble(dVr, 0, TRUE);

					BOOL bOkCase = TRUE;
					if(dPu >= dDisPr) bOkCase = FALSE;
					if(dH >= dVr) bOkCase = FALSE;
					if(dResult >= LSD_CHECK_AXIALMOMENT_COEF) bOkCase = FALSE;
					
					CString strOk = bOkCase ? _T("O.K") : _T("$#03N.G");


					if(nCombo == 0)
					{
						XLText->AddFormatText(_T("$h$c$me1%s$m+02"), hggettext("º»Ã¼\n°ËÅä"));
						XLText->AddMergeFormatText(nCntCombo-1, 0, _T("$c%s$m+01"), strEnv);
						XLText->AddFormatText(_T("$c$me1%s$m+02$c$me2%s$m+03$c$me2%s$m+03$c$f3$me2%s$m+03$c$f3$me2%s$m+03$c$me2%s$m+03$c$me2%s$m+03$c$me1%s$n$h"), pLoad->m_sName, sDisPuMax, sDisPrMax, sPuPr, sResult, sH, sVr,strOk);
					}
					else
					{
						XLText->AddFormatText(_T("$h$c$me1%s$m+03$c$me1%s$m+02$c$me2%s$m+03$c$me2%s$m+03$c$f3$me2%s$m+03$c$f3$me2%s$m+03$c$me2%s$m+03$c$me2%s$m+03$c$me1%s$n$h"),
							hggettext("º»Ã¼\n°ËÅä"), pLoad->m_sName, sDisPuMax, sDisPrMax, sPuPr, sResult, sH, sVr,strOk);
					}		
				}
			}

			XLText->AddFormatText(_T("$n$h"));
		}
		else if(pCalcPile->m_pPile->m_nType == PILE_TYPE_HYUNJANG)
		{
			long nSize = pCalcPile->m_sMatHyunta.GetRow();
			long nCols = 9;
			XLText->AddTable(nSize, 24, 1, 6);
			XLText->AddFormatText(_T("$h$c$me5%s$m+06$c$me2%s$m+03$c$me2%s$m+03$c$me2%s$m+03$c$me2%s$m+03$c$me2%s$m+03$c$me1%s$m+02$c$me1%s$m+02$n$h"),
				hggettext("±¸ ºÐ"),hggettext("As,min"),hggettext("As,use"),hggettext("As,Max"), hggettext("Pu"), hggettext("¥õPn"), hggettext("¾ÈÀüµµ"), hggettext("ºñ°í"));

			for(long ix=0; ix<nSize; ++ix)
			{
				BOOL bExtreme = (ix > 2) ? TRUE : FALSE;
				double dMoment = GetValueUnitChange(::hgatof(pCalcPile->m_sMatHyunta(ix,DESIGNPILE_HYUNTA_M)),pData->m_szTonfM,UNIT_CGS_TONFM);
				double dAxial  = GetValueUnitChange(::hgatof(pCalcPile->m_sMatHyunta(ix,DESIGNPILE_HYUNTA_P)),pData->m_szTonf,UNIT_CGS_TONF);
				double dDepth = pCalcPile->m_pPile->m_dH0 + hgatof(pCalcPile->m_sMatHyunta(ix, 6));
				CConcBasic *pConc = new CConcBasic;
				pCalcPile->SetConcDataHyunta(pConc, dDepth, bExtreme);

				// BoxÇü ÀÌ¶ó¸é ¸»¶Ò ¹Ý·ÂÀÌ ¾Æ´Ï¶ó PileLoadÀÇ Ãà·ÂÀ» ½á¾ßÁö
				if(pBri->IsBoxType())
				{
					long nIdxPile = bExtreme? ix-3 : ix;
					double dDistPile = pFoot->GetDistMaxPile(nIdxPile);

					CPileLoad *pPileLoad = pCalcPile->GetPileLoad(bExtreme? PILE_CASE_EXTREME : PILE_CASE_ULTIMATE, nIdxPile);
					dMoment = pPileLoad->m_dMoment * dDistPile;
					dAxial  = pPileLoad->m_dForceVert * dDistPile;
				}

				pConc->m_sTitle = pCalcPile->m_sMatHyunta(ix,5);
				pConc->m_Axial	= fabs(dAxial);
				pConc->m_Mux	= fabs(dMoment);

				CColumn Column(pConc, FALSE, pCalcPile->m_nSeclectPmin);	// ÇöÀåÅ¸¼³¸»¶Ò
				Column.SetValuePmax(pData->GetPmaxColumn());
				Column.CalculateAll(FALSE);

				for(long jx=0; jx<9; ++jx)
				{
					long nMerge(0);
					if(jx == 0)
					{
						str = ix < 3? _T("±ØÇÑÇÑ°è") : _T("±Ø´Ü»óÈ²ÇÑ°è");
						nMerge = (ix==0 || ix==3)? 2 : 0;
					}
					else if(jx == 1)
						str = pConc->m_sTitle;
					else if(jx == 2)
						str.Format(_T("%.5f"), Column.GetValuePmin());
					else if(jx == 3)
						str.Format(_T("%.5f"), Column.GetRebarUsedRate());
					else if(jx == 4)
						str.Format(_T("%.5f"), Column.GetValuePmax());
					else if(jx == 5)
						str = pData->GetStrDoubleUnitChange(Column.m_pData->m_Axial, UNIT_CGS_TONF, pData->m_szTonf);
					else if(jx == 6)
						str = pData->GetStrDoubleUnitChange(Column.GetPiPn(), UNIT_CGS_TONF, pData->m_szTonf);
					else if(jx == 7)
						str.Format(_T("%.3f"), Column.GetPiPn()/Column.m_pData->m_Axial);
					else if(jx == 8)
					{
						BOOL bOK	= (Column.IsOkAxial() && Column.IsOK_RebarRate());
						str = bOK? _T("O.K") : _T("$#03N.G");
					}

					XLText->AddMergeFormatText(nMerge, (jx < 7)? 2 : 1, "$c%s", str);
					XLText->AddFormatText("$m+%02d", (jx < 7)? 3 : 2);
				}
				XLText->AddFormatText("$n$h");
				delete pConc;
			}
		}
	}
	else
	{
		double dApply = 0, dUse = 0, dSF = 0, dSFa = 0;
		// (ARCBRIDGE-3360) »ó½Ã ÇÏ³ª¸¸ Ãâ·Â¿¡¼­ ÇÑ°è»óÅÂº° Ãâ·ÂÀ¸·Î º¯°æ.
		XLText->AddFormatText("$h$e¡á ±âÃÊ °ËÅä$n$h");
		XLText->AddTable(4-1, 16-1, 1, 8);

		// Title
		long nWidthTitle[] = { 8, 3, 3, 2 };
		CString str;
		CStringArray sArrTitle;
		sArrTitle.Add(_T("±¸  ºÐ"));
		str.Format(_T("Qmax(%s)"), pData->m_szTonf_M2);;
		sArrTitle.Add(str);
		str.Format(_T("Qa(%s)"), pData->m_szTonf_M2);;
		sArrTitle.Add(str);
		sArrTitle.Add(_T("ºñ °í"));

		for(long i=0; i<4; i++)
		{
			XLText->AddMergeFormatText(0, nWidthTitle[i]-1, "$c%s", sArrTitle.GetAt(i));
			XLText->AddFormatText("$m+%02d", nWidthTitle[i]);
		}
		XLText->AddFormatText("$n$h");

		// Data
		long nWidthData[] = { 3, 5, 3, 3, 2 };
		for(long nCombo=0; nCombo<3; nCombo++)
		{
			ELSDLoadGroupKind eKind = static_cast<ELSDLoadGroupKind>(nCombo);

			double dQmax(0);//	= max(pLsdLoad->sSafeResult.Qmax, pLsdLoad->sSafeResult.Qmin) + pLsdLoad->sSafeResult.QMax_Mass;
			double dQa(0);//		= pLsdLoad->sSafeResult.GetQa(bRock);
			bOK		= pFoot->GetCheckValueSupport(nJijum, &dQa, &dQmax, TRUE, eKind);
			szOkNg	= (bOK) ? "O.K":"$#03N.G";

			CString strEnv(_T(""));
			if( eKind == eLSD_COMBINATION_LIMIT ) strEnv = "±ØÇÑÇÑ°è»óÅÂ";
			else if( eKind == eLSD_COMBINATION_EXTREME ) strEnv = "±Ø´Ü»óÈ²ÇÑ°è»óÅÂ";
			else strEnv = "»ç¿ëÇÑ°è»óÅÂ";

			for(long k=0; k<5; k++)
			{
				if(k==0) str = _T("ÁöÁö·Â");
				else if(k==1) str = strEnv;
				else if(k==2) str.Format("$f3%.3f", dQmax);
				else if(k==3) str.Format("$f3%.3f", dQa);
				else if(k==4) str = szOkNg;
				else str = _T("");

				XLText->AddMergeFormatText(nCombo==0 && k == 0 ? 2 : 0, nWidthData[k]-1, "$c%s", str);
				XLText->AddFormatText("$m+%02d", nWidthData[k]);
			}
			XLText->AddFormatText("$n$h"); 
		}
	}

	*XLText += 1;
}

void CARcBridgeCalcSummary::MakeTableCheckFootAndPile(CXLFormatText *XLText)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nCntJijum =  pBri->GetCountJijum();
	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	if(pBri->IsRcSlab()) return;
	if(pBri->IsBoxType()) 
	{
		HGBOOL bPileASD = (pBri->IsBoredPileASD(0) || pBri->IsPhcPileASD(0))? TRUE : FALSE;
		if(bLsd && bPileASD == FALSE)
			MakeTableCheckFootAndPileBoxType_Lsd(XLText);
		else
			MakeTableCheckFootAndPileBoxType_Usd(XLText);
		return;
	}

	
	long nIdxPileCalc(0);

	BOOL bExistDF = FALSE;
	for(long nJijum=0; nJijum<nCntJijum; nJijum++)
	{
		if((nJijum==0 || nJijum==nCntJijum-1) && pBri->IsFooting(nJijum==0) == FALSE)
			continue;
		CFootingApp* pFoot = pBri->GetFootingByJijumNumber(nJijum);
		if(pFoot == NULL || pFoot->IsVirtualFooting()) continue;

		CString str = "";
		if( nJijum==0 )
			str.Format("½ÃÁ¡");
		else if( nJijum == nCntJijum -1 )
			str.Format("Á¾Á¡");
		else
			str.Format("Áß°£ÁöÁ¡%d", nJijum);

		CString strTitle = "¡á ±âÃÊ ¹× ¸»¶ÒÀÇ °ËÅä : " + str;

		XLText->AddFormatText("$h$e%s$n$h", strTitle);

		BOOL bPileBase = pBri->IsPileBase(nJijum);
		if(bPileBase)
		{
			HGBOOL bPileASD = (pBri->IsBoredPileASD(nJijum) || pBri->IsPhcPileASD(nJijum))? TRUE : FALSE;
			if(bLsd && bPileASD == FALSE)
			{
				MakeTableCheckPile_Lsd(XLText, nIdxPileCalc);
			}
			else
			{
				MakeTableCheckPile_Usd(XLText, nJijum);
			}
			nIdxPileCalc++;
		}
		else
		{
			if(bLsd)
			{
				MakeTableCheckFoot_Lsd(XLText, nJijum);
			}
			else
			{
				bExistDF = TRUE;
				MakeTableCheckFoot_Usd(XLText, nJijum);
			}
		}

		XLText->AddFormatText(_T("$n$h"));
	}
	if(bExistDF)
	{
		XLText->AddFormatText("$h$c*$rÀüµµ¿¡´ëÇÑ ¾ÈÁ¤ÀÇ °æ¿ì ¾ÈÀüÀ²ÀÌ Çã¿ë¾ÈÀüÀ²º¸´Ù Ä¿µµ Æí½É°ËÅä°¡ N.GÀÏ°æ¿ì °á°ú°¡ N.G·Î ³ª¿Â´Ù.$n");
	}
	XLText->AddFormatText(_T("$n$h"));
}

void CARcBridgeCalcSummary::MakeTableCheckFoot_Usd(CXLFormatText *XLText, long nJijum)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CARcBridgeCalcFooting *pFoot = m_pStd->m_pFooting;

	BOOL bApplyEarthQauke = pBri->IsApplyEarthQuakeDesign();
	long nCntRow = bApplyEarthQauke ? 6 : 3;

	XLText->AddTable(nCntRow, 25-1, 1, 11);

	long nWidthTitle[] = { 11, 3, 3, 3, 3, 2 };
	char *szTitle[] = { "±¸  ºÐ", "Çã¿ë°ª", "¹ß»ý°ª", "¾ÈÀüÀ²", "Çã¿ë¾ÈÀüÀ²", "ºñ °í"};
	for(long i=0; i<6; i++)
	{
		XLText->AddMergeFormatText(0, nWidthTitle[i]-1, "$c%s", szTitle[i]);
		XLText->AddFormatText("$m+%02d", nWidthTitle[i]);
	}
	XLText->AddFormatText("$n$h");

	CStringArray strArrFoot;
	CString str = _T(""),szFirst = _T("");

	if( nJijum==0 )
		szFirst.Format("½ÃÁ¡");
	else if( nJijum == pBri->m_nQtyJigan )
		szFirst.Format("Á¾Á¡");
	else
		szFirst.Format("Áß°£\nÁöÁ¡%d", nJijum);

	str.Format("  Àüµµ¿¡´ëÇÑ ¾ÈÁ¤(%s)", pData->m_szTonfM);
	strArrFoot.Add(str);
	str.Format("  È°µ¿¿¡´ëÇÑ ¾ÈÁ¤(%s)", pData->m_szTonf);
	strArrFoot.Add(str);
	str.Format("  ÁöÁö·Â¿¡´ëÇÑ ¾ÈÁ¤(%s)", pData->m_szTonf_M2);
	strArrFoot.Add(str);

	long nWidthData[] = { 2, 2, 7, 3, 3, 3, 3, 2 };

	long nCntItem = bApplyEarthQauke ? 6 : 3;
	XLText->AddFormatText("$md%d", nCntItem-1);
	for(long j=0; j<nCntItem; j++)
	{
		BOOL bOK	= FALSE;
		BOOL bNormal = (j<3)? TRUE : FALSE;
		if(bNormal==FALSE && bApplyEarthQauke==FALSE)
		{
			break;
		}

		long nId = (j > 2)? j-3:j;
		double dApply = 0;
		double dUse = 0;
		double dSF	= 0;
		double dSFa = 0;
		CString szOkNg = _T("");

		if(nId == 0)
		{
			bOK		= pFoot->GetCheckValueOverturning(nJijum, &dApply, &dUse, bNormal);
			dSF		= dApply/dUse;
			dSFa	= (bNormal)? pBri->m_SafeFactorOverturning[0] : pBri->m_SafeFactorOverturning[1];
			szOkNg	= (bOK)? "O.K":"$#03N.G";
		}
		else if(nId == 1)
		{
			bOK		= pFoot->GetCheckValueSliding(nJijum, &dApply, &dUse, bNormal);
			dSF		= dApply/dUse;
			dSFa	= (bNormal)? pBri->m_SafeFactorSliding[0] : pBri->m_SafeFactorSliding[1];
			szOkNg	= (bOK)? "O.K":"$#03N.G";
		}
		else if(nId == 2)
		{
			bOK		= pFoot->GetCheckValueSupport(nJijum, &dApply, &dUse, bNormal);
			dSF		= dApply/dUse;
			dSFa	= 1.0;
			szOkNg	= (bOK) ? "O.K":"$#03N.G";
		}

		long k=0; for(k=0; k<8; k++)
		{
			if(k == 0) str = szFirst;
			else if(k==1)
			{
				if(j==0 || j==3) XLText->AddFormatText("$md2");
				str = !bNormal ? "ÁöÁø½Ã":"Æò»ó½Ã";
			}
			else if(k==2) str = strArrFoot.GetAt(nId);
			else if(k==3) str.Format("$f3%.3f", dApply);
			else if(k==4) str.Format("$f3%.3f", dUse);
			else if(k==5) str.Format("$f3%.3f", fabs(dSF));
			else if(k==6) str.Format("$f3%.3f", dSFa);
			else if(k==7) str = szOkNg;
			else str = _T("");

			XLText->AddMergeFormatText(0, nWidthData[k]-1, k==2 ? "$e%s" : "$c%s", str);
			XLText->AddFormatText("$m+%02d", nWidthData[k]);
		}
		XLText->AddFormatText(_T("$n$h"));
	}
}

void CARcBridgeCalcSummary::MakeTableCheckFoot_Lsd(CXLFormatText *XLText, long nJijum)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CLsdFootingDesign* pLsdFooting = m_pStd->m_pFooting->GetLsdFootingDesign(nJijum);
	if(pLsdFooting == NULL) return;

	long nCntCombo = 0;
	const long nSizeCombo = pLsdFooting->GetSizeLoad();

	for(long nCombo=0; nCombo<nSizeCombo; nCombo++)
	{
		sLsdFootingLoad *pLsdLoad = pLsdFooting->GetLoad(nCombo);
		if(pLsdLoad == NULL || pLsdLoad->eStatus == eLSD_COMBINATION_SERVICE) continue;

		nCntCombo++;
	}

	if(nCntCombo == 0) return;

	////////////////////////// Àüµµ//////////////////////////
	XLText->AddTable(nCntCombo, 15, 1, 8);

	XLText->AddFormatText(_T("$h$c$me7%s $m+08$c$me2%s $m+03$c$me2%s $m+03$c$me1%s $n$h"),
		hggettext("±¸ ºÐ"),hggettext("e(mm)"),hggettext("Çã¿ëÆø(mm)"),hggettext("ºñ°í"));

	CString sName = _T(""), sTitle = _T(""), sE =  _T(""), sWidth = _T(""), sOK = _T("");

	long nCntRow = 1;

	for(long nCombo=0; nCombo<nSizeCombo; nCombo++)
	{
		sLsdFootingLoad *pLsdLoad = pLsdFooting->GetLoad(nCombo);
		if(pLsdLoad == NULL || pLsdLoad->eStatus == eLSD_COMBINATION_SERVICE) continue;

		CString strEnv(_T(""));
		if( pLsdLoad->eStatus == eLSD_COMBINATION_LIMIT ) strEnv = "±ØÇÑ";
		else if( pLsdLoad->eStatus == eLSD_COMBINATION_EXTREME ) strEnv = "±Ø´Ü";
		else strEnv = "»ç¿ë";

		sName	= _T("Àüµµ");
		sTitle	= pLsdLoad->sTitle;
		sE		= GetStrDouble(pLsdLoad->eLoad, 0, TRUE);
		sWidth	= GetStrDouble(pLsdLoad->sSafeResult.eAllow, 0, TRUE);
		sOK		= pLsdLoad->IsOkOverturning() ? _T("O.K") : _T("N.G"); 

		if( nCntRow == nCntCombo )
		{
			XLText->AddFormatText(_T("$h$c$me2%s $m+03$md%d$c$me1%s$m+02$c$me2%s $m+03$c$me2%s $m+03$c$me2%s $m+03$c$me1%s $n$h"),
				sName, (nCombo%eCountFootCheck == 0)? eCountFootCheck-1:0, strEnv, sTitle, sE, sWidth, sOK);
		}
		else
		{
			XLText->AddFormatText(_T("$h$c$me2$md1%s $m+03$md%d$c$me1%s$m+02$c$me2%s $m+03$c$me2%s $m+03$c$me2%s $m+03$c$me1%s $n$h"),
				sName, (nCombo%eCountFootCheck == 0)? eCountFootCheck-1:0, strEnv, sTitle, sE, sWidth, sOK);
		}
		nCntRow++;						
	}

	//			XLText->OutXL();

	///////////////////////////////////È° µ¿//////////////////////////////
	HGBOOL bSkipCheck = FALSE;
	if(pLsdFooting->m_ixTypeGround == LSD_GROUND_TYPE_ROCK)
	{
		bSkipCheck = TRUE;
		for(HGINT32 ix = 0; ix < eLSD_COMBINATION_COUNT - 1; ix++) // ÇÇ·Î´Â Á¦¿Ü
		{
			if(pLsdFooting->sCoffSF.slidingResitCoff[LSD_GROUND_TYPE_ROCK][ix] > 0)
			{
				bSkipCheck = FALSE;
				break;
			}
		}
	}

	if(bSkipCheck == FALSE)
	{
		XLText->AddFormatText(_T("$n$h"));

		XLText->AddFormatText(_T("$s+%02d"), nCntCombo + 3);
		XLText->AddTable(nCntCombo, 17, 1, 8);

		XLText->AddFormatText(_T("$h$c$me7%s $m+08$c$me2%s $m+03$c$me2%s $m+03$c$me1%s $m+02$c$me1%s $n$h"),
			hggettext("±¸ ºÐ"),hggettext("QR"),hggettext("H(N)"),hggettext("S.F"), hggettext("ºñ°í"));


		CString sQR =  _T(""), sH = _T(""), sSF = _T(""), strOk = _T("");
		sName = _T(""), sTitle = _T("");

		nCntRow = 1;

		for(long nCombo=0; nCombo<nSizeCombo; nCombo++)
		{
			sLsdFootingLoad *pLsdLoad = pLsdFooting->GetLoad(nCombo);
			if(pLsdLoad == NULL || pLsdLoad->eStatus == eLSD_COMBINATION_SERVICE) continue;

			double dQR	= pLsdLoad->sSafeResult.QR;
			double dH	= pLsdLoad->H;
			double dSF	= dQR / dH;

			CString strEnv(_T(""));
			if( pLsdLoad->eStatus == eLSD_COMBINATION_LIMIT ) strEnv = "±ØÇÑ";
			else if( pLsdLoad->eStatus == eLSD_COMBINATION_EXTREME ) strEnv = "±Ø´Ü";
			else strEnv = "»ç¿ë";

			sName	= _T("È°µ¿");
			sTitle	= pLsdLoad->sTitle;
			sQR		= GetStrDouble(dQR, 0, TRUE);
			sH		= GetStrDouble(dH,0,TRUE);
			if(Compare(dH, 0., _T("=")))	sSF = _T("¡Ä");
			else							sSF = GetStrDouble(fabs(dSF),3,TRUE);	

			strOk = pLsdLoad->IsOkSliding()? _T("O.K") : _T("N.G");

			if(nCntRow == nCntCombo )
			{
				XLText->AddFormatText(_T("$h$c$me2%s $m+03$md%d$c$me1%s$m+02$c$me2%s $m+03$c$me2%s $m+03$c$me2%s $m+03$c$me1%s $m+02$c$me1%s $n$h"),
					sName, (nCombo%eCountFootCheck == 0)? eCountFootCheck-1:0, strEnv, sTitle, sQR, sH, sSF, strOk);
			}
			else
			{
				XLText->AddFormatText(_T("$h$c$me2$md1%s $m+03$md%d$c$me1%s$m+02$c$me2%s $m+03$c$me2%s $m+03$c$me2%s $m+03$c$me1%s $m+02$c$me1%s $n$h"),
					sName, (nCombo%eCountFootCheck == 0)? eCountFootCheck-1:0, strEnv, sTitle, sQR, sH, sSF, strOk);
			}
			nCntRow++;						
		}
	}

	///////////////////////////////////Áö Áö ·Â//////////////////////////////
	for(long nCombo=0; nCombo<nSizeCombo; nCombo++)
	{
		sLsdFootingLoad *pLsdLoad = pLsdFooting->GetLoad(nCombo);
		if(pLsdLoad == NULL || pLsdLoad->eStatus) continue;

		nCntCombo++;
	}

	XLText->AddFormatText(_T("$n$h"));

	XLText->AddFormatText(_T("$s+%02d"), nCntCombo +15);
	XLText->AddTable(nCntCombo, 15, 1, 8);

	CString str = _T("Qmax(N/§±)");
	CString str1 = _T("Qa(N/§±)");

	XLText->AddFormatText(_T("$h$c$me7%s $m+08$c$me2%s $m+03$c$me2%s $m+03$c$me1%s $n$h"),
		hggettext("±¸ ºÐ"), str, str1, hggettext("ºñ°í"));


	CString sQmax =  _T(""), sQa = _T(""), sOk = _T("");
	sName = _T(""), sTitle = _T("");

	nCntRow = 1;

	for(long nCombo=0; nCombo<nSizeCombo; nCombo++)
	{
		sLsdFootingLoad *pLsdLoad = pLsdFooting->GetLoad(nCombo);
		if(pLsdLoad == NULL) continue;

		BOOL bRock = pLsdFooting->IsRock();

		double dQmax	= max(pLsdLoad->sSafeResult.Qmax, pLsdLoad->sSafeResult.Qmin) + pLsdLoad->sSafeResult.QMax_Mass;
		double dQa		= pLsdLoad->sSafeResult.GetQa(bRock);

		CString strEnv(_T(""));
		if( pLsdLoad->eStatus == eLSD_COMBINATION_LIMIT ) strEnv = "±ØÇÑ";
		else if( pLsdLoad->eStatus == eLSD_COMBINATION_EXTREME ) strEnv = "±Ø´Ü";
		else strEnv = "»ç¿ë";

		sName	= _T("ÁöÁö·Â");
		sTitle	= pLsdLoad->sTitle;
		sQmax	= GetStrDouble(dQmax, 3, TRUE,TRUE);
		sQa		= GetStrDouble(dQa,3,TRUE,TRUE);
		sOk		= pLsdLoad->IsOkSupport(bRock)? _T("O.K") : _T("N.G");

		if(nCntRow == nCntCombo )
		{
			XLText->AddFormatText(_T("$h$c$me2%s $m+03$md%d$c$me1%s$m+02$c$me2%s $m+03$c$me2%s $m+03$c$me2%s $m+03$c$me1%s $n$h"),
				sName, (nCombo%eCountFootCheck == 0)? eCountFootCheck-1:0, strEnv, sTitle, sQmax, sQa, sOk);
		}
		else
		{
			XLText->AddFormatText(_T("$h$c$me2$md1%s $m+03$md%d$c$me1%s$m+02$c$me2%s $m+03$c$me2%s $m+03$c$me2%s $m+03$c$me1%s $n$h"),
				sName, (nCombo%eCountFootCheck == 0)? eCountFootCheck-1:0, strEnv, sTitle, sQmax, sQa, sOk);
		}
		nCntRow++;						
	}

	//////////////////////////////////// Ä§  ÇÏ ////////////////
	nCntCombo = 0;
	for(long nCombo=0; nCombo<nSizeCombo; nCombo++)
	{
		sLsdFootingLoad *pLsdLoad = pLsdFooting->GetLoad(nCombo);
		if(pLsdLoad == NULL || pLsdLoad->eStatus != eLSD_COMBINATION_SERVICE) continue;

		nCntCombo++;
	}

	if(nCntCombo == 0) return;

	XLText->AddFormatText(_T("$n$h"));

	XLText->AddFormatText(_T("$s+%02d"), nCntCombo + 3);
	XLText->AddTable(nCntCombo, 15, 1, 8);

	XLText->AddFormatText(_T("$h$c$me7%s $m+08$c$me2%s $m+03$c$me2%s $m+03$c$me1%s $n$h"),
		hggettext("±¸ ºÐ"), hggettext("Se(mm)"), hggettext("Sa(mm)"), hggettext("ºñ°í"));


	CString sSe =  _T(""), sSa = _T("");
	sName = _T(""), sTitle = _T(""),  sOk = _T("");;

	nCntRow = 1;

	for(long nCombo=0; nCombo<nSizeCombo; nCombo++)
	{
		sLsdFootingLoad *pLsdLoad = pLsdFooting->GetLoad(nCombo);
		if(pLsdLoad == NULL || pLsdLoad->eStatus != eLSD_COMBINATION_SERVICE) continue;

		double dSe	= pLsdLoad->sSafeResult.Se;
		double dSa = pLsdFooting->m_pFooting->m_dAllowableSubsidence;

		CString strEnv(_T(""));
		if( pLsdLoad->eStatus == eLSD_COMBINATION_LIMIT ) strEnv = "±ØÇÑ";
		else if( pLsdLoad->eStatus == eLSD_COMBINATION_EXTREME ) strEnv = "±Ø´Ü";
		else strEnv = "»ç¿ë";

		sName	= _T("Ä§ÇÏ");
		sTitle	= pLsdLoad->sTitle;
		sSe		= GetStrDouble(dSe, 3, TRUE,TRUE);
		sSa		= GetStrDouble(dSa, 3, TRUE,TRUE);
		sOk	= pLsdLoad->IsOkSubsidence()? _T("O.K") : _T("N.G");

		if(nCntRow == nCntCombo )
		{
			XLText->AddFormatText(_T("$h$c$me2%s $m+03$md%d$c$me1%s$m+02$c$me2%s $m+03$c$me2%s $m+03$c$me2%s $m+03$c$me1%s $n$h"),
				sName, (nCombo%eCountFootCheck == 0)? eCountFootCheck-1:0, strEnv, sTitle, sSe, sSa, sOk);
		}
		else
		{
			XLText->AddFormatText(_T("$h$c$me2$md1%s $m+03$md%d$c$me1%s$m+02$c$me2%s $m+03$c$me2%s $m+03$c$me2%s $m+03$c$me1%s $n$h"),
				sName, (nCombo%eCountFootCheck == 0)? eCountFootCheck-1:0, strEnv, sTitle, sSe, sSa, sOk);
		}
		nCntRow++;						
	}
}

void CARcBridgeCalcSummary::MakeTableCheckPile_Usd(CXLFormatText *XLText, long nJijum)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CARcBridgeCalcFooting *pFoot = m_pStd->m_pFooting;

	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	BOOL bPileHyunta = pBri->GetFootingByJijumNumber(nJijum)->m_exFooting.m_Pile.m_nType == PILE_TYPE_HYUNJANG;
	BOOL bApplyEarthQauke = pBri->IsApplyEarthQuakeDesign();
	long nCntRow = (bPileHyunta? 4 : 5) * (bApplyEarthQauke ? 2 : 1);
	long nDec03 = bLsd ? 0 : 3;

	XLText->AddTable(nCntRow, 25-1, 1, 11);

	long nWidthTitle[] = { 11, 3, 3, 3, 3, 2 };
	char *szTitle[] = { "±¸  ºÐ", "Çã¿ë°ª", "¹ß»ý°ª", "¾ÈÀüÀ²", "Çã¿ë¾ÈÀüÀ²", "ºñ °í"};
	for(long i=0; i<6; i++)
	{
		XLText->AddMergeFormatText(0, nWidthTitle[i]-1, "$c%s", szTitle[i]);
		XLText->AddFormatText("$m+%02d", nWidthTitle[i]);
	}
	XLText->AddFormatText("$n$h");

	CStringArray strArr;
	CString str = _T("");

	str.Format("  Çã¿ëÁöÁö·Â(%s/EA)", pData->m_szTonf);
	strArr.Add(str);
	str.Format("  Çã¿ëÀÎ¹ß·Â(%s/EA)", pData->m_szTonf);
	strArr.Add(str);
	if(bPileHyunta)
	{
		str.Format(" Ãà¹æÇâ°­µµ(%s)", pData->m_szTonf);
		strArr.Add(str);
		strArr.Add(_T(""));
	}
	else
	{
		str.Format("  ÈÚÀÀ·Â(%s)", pData->m_szKgf_CM2);
		strArr.Add(str);
		str.Format("  Àü´ÜÀÀ·Â(%s)", pData->m_szKgf_CM2);
		strArr.Add(str);
	}
	str.Format("  ¼öÆòº¯À§(%s)", (pData->IsSI())? "mm":"cm");
	strArr.Add(str);

	long nWidthData[] = { 4, 7, 3, 3, 3, 3, 2 };
	long nCntItem = bApplyEarthQauke ? 10 : 5;
	for(long j=0; j<nCntItem; j++)
	{
		BOOL bNormal = (j<5) ? TRUE:FALSE; 
		if(bNormal==FALSE && bApplyEarthQauke==FALSE)
		{
			break;
		}
		long nId = (j > 4)? j-5:j;
		double dApply = 0;
		double dUse = 0;
		double dSF	= 0;
		double dSFa = 0;
		CString szOkNg = _T("");

		if(bPileHyunta && nId == 3) continue;

		if(nId == 0)
		{
			pFoot->GetCheckValuePileForce(nJijum, &dApply, &dUse, bNormal);
			dSF		= fabs(dApply/dUse);
			dSFa	= 1.0;
			szOkNg	= (dSF > dSFa)? "O.K":"$#03N.G";
		}
		else if(nId == 1)
		{
			pFoot->GetCheckValuePileForceMinus(nJijum, &dApply, &dUse, bNormal);
			dSF		= fabs(dApply/dUse);
			dSFa	= 1.0;
			BOOL bOK = (fabs(dUse) < dApply || dUse >= 0) ? TRUE : FALSE;
			szOkNg	= (bOK)? "O.K":"$#03N.G";
		}
		else if(nId == 2)
		{
			if(bPileHyunta)
				pFoot->GetCheckValueHyuntaPileAxialForce(nJijum, &dApply, &dUse, bNormal);
			else
				pFoot->GetCheckValuePileBending(nJijum, &dApply, &dUse, bNormal);
			dSF		= fabs(dApply/dUse);
			dSFa	= 1.0;
			szOkNg	= (dUse == 0 || dSF > dSFa)? "O.K":"$#03N.G";
		}
		else if(nId == 3)
		{
			pFoot->GetCheckValuePileShear(nJijum, &dApply, &dUse, bNormal);
			dSF		= fabs(dApply/dUse);
			dSFa	= 1.0;
			szOkNg	= (dSF > dSFa)? "O.K":"$#03N.G";
		}
		else if(nId == 4)
		{
			pFoot->GetCheckValuePileDisplacement(nJijum, &dApply, &dUse, bNormal);
			dSF		= fabs(dApply/dUse);
			dSFa	= 1.0;
			szOkNg	= (dSF > dSFa)? "O.K":"$#03N.G";
		}

		for(long k=0; k<7; k++)
		{
			if(k==0)
			{
 				if(j==0 || j==5) XLText->AddFormatText("$md%d", bPileHyunta ? 3 : 4);
				if(bLsd)
				{
					str = !bNormal ? "±Ø´Ü»óÈ²":"»ç¿ëÇÑ°è";
				}
				else
				{
					str = !bNormal ? "ÁöÁø½Ã":"Æò»ó½Ã";
				}
			}
			else if(k==1) str = strArr.GetAt(nId);
			else if(k==2) 
			{
				if(bLsd && nId < 2)
					str = CommaC(dApply);
				else
					str.Format("$f3%.3f", dApply);
			}
			else if(k==3)
			{
				if((nId==1 && dUse>=0)) 
					str = _T("-");
				else 
				{
					if(bLsd && nId < 2)
						str = CommaC(dUse);
					else
						str.Format("$f3%.3f", dUse);
				}
			}
			else if(k==4)
			{
				if((nId==1 && dUse>=0) || (nId==3 && dUse==0) || (bPileHyunta && nId==2 && dUse==0)) str = _T("-");
				else str.Format("$f3%.3f", dSF);
			}
			else if(k==5)
			{
				if((nId==1 && dUse>=0) || (nId==3 && dUse==0)) str = _T("-");
				else str.Format("$f3%.3f", dSFa);
			}
			else if(k==6) 
			{
				str = szOkNg;
			}
			else
			{
				str = _T("");
			}

			XLText->AddMergeFormatText(0, nWidthData[k]-1, k==1 ? "$e%s" : "$c%s", str);
			XLText->AddFormatText("$m+%02d", nWidthData[k]);
		}
		XLText->AddFormatText("$n$h");
	}
}

void CARcBridgeCalcSummary::MakeTableCheckPile_Lsd(CXLFormatText *XLText, long nIdxPileCalc)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	if( m_pStd->m_pArrPileCalc.IsEmpty() || m_pStd->m_pArrPileCalc.GetSize() <= nIdxPileCalc )
		return;

	CString str = _T(""), str1 = _T(""), str2 = _T(""), str3 = _T(""), strOk  = _T("");

	CDesignPile *pCalcPile = m_pStd->m_pArrPileCalc.GetAt(nIdxPileCalc);
	if( pCalcPile == NULL ) return;

	long nSizeCombo = 0;
	//////////////////////¸»¶Ò ÁöÁö·Â///////////////////
	nSizeCombo += pCalcPile->GetSizePileLoad(PILE_CASE_USE, TRUE);
	nSizeCombo += pCalcPile->GetSizePileLoad(PILE_CASE_ULTIMATE);
	nSizeCombo += pCalcPile->GetSizePileLoad(PILE_CASE_EXTREME);

	if(nSizeCombo == 0) return;

	XLText->AddTable(nSizeCombo, 15, 1, 8);

	XLText->AddFormatText(_T("$h$c$me7%s$m+08$c$me2%s$m+03$c$me2%s$m+03$c$me1%s$n$h"),
		hggettext("±¸ ºÐ"),hggettext("Vmax(N)"),hggettext("Qr(N)"),hggettext("ºñ°í"));		
	XLText->AddMergeText(nSizeCombo-1, 0, hggettext("$h$cÁöÁö·Â"));

	for(long nTypePileCase = 0; nTypePileCase < 3; nTypePileCase++)
	{
		CString strEnv(_T(""));
		if( nTypePileCase == PILE_CASE_ULTIMATE ) strEnv = "±ØÇÑ";
		else if( nTypePileCase == PILE_CASE_EXTREME ) strEnv = "±Ø´Ü";
		else strEnv = "»ç¿ë";

		long nCntCombo = pCalcPile->GetSizePileLoad(nTypePileCase, TRUE);
		for (long nCombo =0; nCombo< nCntCombo; nCombo++)
		{
			CPileLoad *pLoad = pCalcPile->GetPileLoad(nTypePileCase, nCombo, TRUE);

			double dVmaxFix =  GetValueUnitChange(pCalcPile->GetValueMaxMinPileHead(nCombo, PILE_FORCE_AXIAL, -1, TRUE , TRUE, nTypePileCase, TRUE), UNIT_CGS_TONF, pData->m_szTonf);
			double dVmaxHin =  GetValueUnitChange(pCalcPile->GetValueMaxMinPileHead(nCombo, PILE_FORCE_AXIAL, -1, FALSE , TRUE, nTypePileCase, TRUE), UNIT_CGS_TONF, pData->m_szTonf);
			double dVmax = max(dVmaxFix, dVmaxHin);
			double dRa = pCalcPile->m_Ra[nTypePileCase];

			strOk	= (dVmax > dRa)? _T("N.G") : _T("O.K");
			str		= GetStrDouble(dVmax, 0, TRUE);
			str1	= GetStrDouble(dRa, 0, TRUE);

			XLText->AddFormatText(_T("$h$c$me1%s$m+02$md%d$c$me1%s$m+02$c$me3%s$m+04 $c$me2%s$m+03 $c$me2%s$m+03 $c$me1%s$n$h"),
				hggettext("ÁöÁö·Â"), nCombo==0? nCntCombo-1:0, strEnv, pLoad->m_sName, str, str1, strOk);		

		}
	}
	XLText->AddFormatText(_T("$n$h"));

	//////////////////////¸»¶Ò ÀÎ¹ß·Â///////////////////
	XLText->AddFormatText(_T("$s+%02d"), nSizeCombo+5);

	XLText->AddTable(nSizeCombo, 15, 1, 8);				

	XLText->AddFormatText(_T("$h$c$me7%s$m+08$c$me2%s$m+03$c$me2%s$m+03$c$me1%s$n$h"),
		hggettext("±¸ ºÐ"),hggettext("Vmin(N)"),hggettext("Qu(N)"),hggettext("ºñ°í"));		
	XLText->AddMergeText(nSizeCombo-1, 0, hggettext("$h$cÀÎ¹ß·Â"));

	for(long nTypePileCase = 0; nTypePileCase < 3; nTypePileCase++)
	{
		CString strEnv(_T(""));
		if( nTypePileCase == PILE_CASE_ULTIMATE ) strEnv = "±ØÇÑ";
		else if( nTypePileCase == PILE_CASE_EXTREME ) strEnv = "±Ø´Ü";
		else strEnv = "»ç¿ë";

		long nCntCombo = pCalcPile->GetSizePileLoad(nTypePileCase, TRUE);
		for (long nCombo =0; nCombo< nCntCombo; nCombo++)
		{
			CPileLoad *pLoad = pCalcPile->GetPileLoad(nTypePileCase, nCombo, TRUE);

			double dVmaxFix =  GetValueUnitChange(pCalcPile->GetValueMaxMinPileHead(nCombo, PILE_FORCE_AXIAL, -1, TRUE , TRUE, nTypePileCase, TRUE), UNIT_CGS_TONF, pData->m_szTonf);
			double dVmaxHin =  GetValueUnitChange(pCalcPile->GetValueMaxMinPileHead(nCombo, PILE_FORCE_AXIAL, -1, FALSE , TRUE, nTypePileCase, TRUE), UNIT_CGS_TONF, pData->m_szTonf);
			double dVmax = MAX(dVmaxFix, dVmaxHin);

			double dVminFix =  GetValueUnitChange(pCalcPile->GetValueMaxMinPileHead(nCombo, PILE_FORCE_AXIAL, -1,TRUE, FALSE, nTypePileCase,TRUE), UNIT_CGS_TONF, pData->m_szTonf);
			double dVminHin =  GetValueUnitChange(pCalcPile->GetValueMaxMinPileHead(nCombo, PILE_FORCE_AXIAL, -1,FALSE % 2, FALSE, nTypePileCase,TRUE), UNIT_CGS_TONF, pData->m_szTonf);
			double dVmin = MIN(0, MIN(dVminFix, dVminHin));

			double dRa = pCalcPile->m_Ra[nTypePileCase];
			double dPa = pCalcPile->m_Pa[nTypePileCase];

			strOk	= (fabs(dVmin) > dPa)? _T("N.G") : _T("O.K");
			str		= GetStrDouble(fabs(dVmin), 0, TRUE);
			str1	= GetStrDouble(dPa, 0, TRUE);


			XLText->AddFormatText(_T("$h$c$me1%s$m+02$md%d$c$me1%s$m+02$c$me3%s$m+04 $c$me2%s$m+03 $c$me2%s$m+03 $c$me1%s$n$h"),
				hggettext("ÀÎ¹ß·Â"), nCombo==0? nCntCombo-1:0, strEnv,pLoad->m_sName, str, str1, strOk);		

		}
	}
	XLText->AddFormatText(_T("$n$h"));

	//////////////////////¼öÆò º¯À§///////////////////	
	nSizeCombo = pCalcPile->GetSizePileLoad(PILE_CASE_USE, TRUE);	//»ç¿ëÇÑ°è»óÅÂ

	XLText->AddFormatText(_T("$s+%02d"), nSizeCombo+5);

	XLText->AddTable(nSizeCombo, 15, 1, 8);

	XLText->AddFormatText(_T("$h$c$me7%s $m+08$c$me2%s $m+03$c$me2%s $m+03$c$me1%s $n$h"),
		hggettext("±¸ ºÐ"),hggettext("¡â(mm)"),hggettext("¡âa(mm)"),hggettext("ºñ°í"));		
	XLText->AddMergeText(nSizeCombo-1, 0, hggettext("$h$c¼öÆòº¯À§"));

	CString sDelta = _T(""), sDisa = _T("");

	long nCntCombo = pCalcPile->GetSizePileLoad(PILE_CASE_USE, TRUE);
	for (long nCombo =0; nCombo< nCntCombo; nCombo++)
	{
		long nIdxOrg(0);
		CPileLoad *pLoad = pCalcPile->GetPileLoad(PILE_CASE_USE, nCombo, TRUE, nIdxOrg);
		if(pLoad == NULL) continue;

		double deltaX	= max(fabs(pCalcPile->m_EqFix_deltaX[PILE_CASE_USE].GetAt(nIdxOrg)),fabs(pCalcPile->m_EqHin_deltaX[PILE_CASE_USE].GetAt(nIdxOrg)));
		double dDisa	= frCm(pCalcPile->m_Disa[0]*100);

		deltaX			= frCm(deltaX*100);

		sDelta	= GetStrDouble(deltaX, 0, TRUE, TRUE);
		sDisa	= GetStrDouble(dDisa, 0, TRUE, TRUE);
		strOk	= deltaX > dDisa? _T("N.G") : _T("O.K");


		XLText->AddFormatText(_T("$h$c$me1%s $m+02$md%d$c$me1%s$m+02$c$me3%s $m+04$c$me2%s $m+03$c$me2%s $m+03$c$me1%s $n$h"),
			hggettext("¼öÆò\nº¯À§"), nCombo==0? nSizeCombo-1:0, hggettext("»ç¿ë"), pLoad->m_sName, sDelta, sDisa,  strOk);	
	}
	XLText->AddFormatText(_T("$n$h"));		

	//////////////////////Ä§ÇÏ·®///////////////////
	nSizeCombo = pCalcPile->GetSizePileLoad(PILE_CASE_USE, TRUE);	//»ç¿ëÇÑ°è»óÅÂ

	XLText->AddFormatText(_T("$s+%02d"), nSizeCombo+5);

	XLText->AddTable(nSizeCombo, 15, 1, 8);

	XLText->AddFormatText(_T("$h$c$me7%s $m+08$c$me2%s $m+03$c$me2%s $m+03$c$me1%s $n$h"),
		hggettext("±¸ ºÐ"),hggettext("¡â(mm)"),hggettext("¡âa(mm)"),hggettext("ºñ°í"));		
	XLText->AddMergeText(nSizeCombo-1, 0, hggettext("$h$cÄ§ÇÏ·®"));

	for (long nCombo =0; nCombo< nSizeCombo; nCombo++)
	{
		CPileLoad *pLoad = pCalcPile->GetPileLoad(PILE_CASE_USE, nCombo, TRUE);
		if(pLoad == NULL) continue;

		double dSubsidence = pCalcPile->GetLsdSubsidenceByCombo(nCombo);
		double dSa		   = 15; //¸Â´Â º¯¼ö¸¦ Ã£À» °Í

		str		= GetStrDouble(dSubsidence, 0, TRUE);
		str1	= GetStrDouble(dSa, 0, TRUE);
		strOk	= dSubsidence > dSa? _T("N.G") : _T("O.K");


		XLText->AddFormatText(_T("$h$c$me1%s $m+02$md%d$c$me1%s$m+02$c$me3%s $m+04$c$me2%s $m+03$c$me2%s $m+03$c$me1%s $n$h"),
			hggettext("Ä§ÇÏ·®"), nCombo==0? nSizeCombo-1:0, hggettext("»ç¿ë"),pLoad->m_sName, str, str1, strOk);	
	}
	XLText->AddFormatText(_T("$n$h"));

	//////////////////////Ãà·Â, ®‚¾ÐÃà°ú ÈÚ, Àü´ÜÀÀ·Â///////////////////
	// (ARCBRIDGE-2689) °­°ü ¸»¶Ò ÀÏ¶§¸¸ º»Ã¼ °ËÅä
	if(pCalcPile->m_pPile->m_nType == PILE_TYPE_STEEL || pCalcPile->m_pPile->m_nType == PILE_TYPE_STEEL_DIGGING)
	{
		nSizeCombo = 0;
		nSizeCombo += pCalcPile->GetSizePileLoad(PILE_CASE_ULTIMATE);
		nSizeCombo += pCalcPile->GetSizePileLoad(PILE_CASE_EXTREME);
		nSizeCombo += 1;
		XLText->AddFormatText(_T("$s+%02d"), nSizeCombo+5);

		XLText->AddTable(nSizeCombo, 23, 2, 6);

		XLText->AddFormatText(_T("$h$c$me5$md1%s $m+06$c$me5%s $m+06$c$me3%s $m+04$c$me5%s $m+06$c$me1$md1%s$n$h"),
			hggettext("±¸ ºÐ"),hggettext("Ãà·Â"),hggettext("Ãà¾ÐÃà°ú ÈÚ"),hggettext("Àü´ÜÀÀ·Â"), hggettext("ºñ°í"));
		XLText->AddFormatText(_T("$h$c$me5%s $m+06$c$me2%s $m+03$c$me2%s $m+03$c$me1%s $m+02$c$me1%s $m+02$c$me2%s $m+03$c$me2%s $m+03$c$me1%s$n$h"),
			hggettext("±¸ ºÐ"),hggettext("Pu(N)"),hggettext("Pr(N)"),hggettext("Pu/Pr"),hggettext("°á°ú"),hggettext("H(N)"),hggettext("Vr"), hggettext("ºñ°í"));

		XLText->OutXL();

		for(long nTypePileCase = PILE_CASE_USE; nTypePileCase < PILE_CASE_COUNT; nTypePileCase++)
		{
			// (ARCBRIDGE-2689) º»Ã¼ °ËÅä´Â ±ØÇÑÇÑ°è»óÅÂ¿Í ±Ø´Ü»óÈ²ÇÑ°è»óÅÂ Ãâ·Â
			if(nTypePileCase == PILE_CASE_USE) continue;

			CString strEnv(_T(""));
			if( nTypePileCase == PILE_CASE_ULTIMATE ) strEnv = "±Ø\nÇÑ";
			else if( nTypePileCase == PILE_CASE_EXTREME ) strEnv = "±Ø\n´Ü";
			else strEnv = "»ç¿ë";

			long nSize = pCalcPile->GetSizePileLoad(nTypePileCase);
			XLText->AddMergeText(nSize-1, 0, strEnv);

			for(long row=0; row<nSize; row++) //row
			{
				long nCombo =  row; //(long)(row/2.0);

				CPileLoad *pLoad = pCalcPile->GetPileLoad(nTypePileCase, nCombo);
				if(pLoad == NULL) continue;
				////////////////////////////// (Ãà·Â) ////////////////////////
				//Pu 
				double  dDisPu1		= GetValueUnitChange(fabs(pCalcPile->GetValueMaxMinPileHead(nCombo, PILE_FORCE_AXIAL, -1, TRUE, TRUE, nTypePileCase, TRUE)), UNIT_CGS_TONF, pData->m_szTonf);
				double  dDisPu2		= GetValueUnitChange(fabs(pCalcPile->GetValueMaxMinPileHead(nCombo, PILE_FORCE_AXIAL, -1, FALSE, TRUE, nTypePileCase, TRUE)), UNIT_CGS_TONF, pData->m_szTonf);
				CString sDisPuMax	= GetStrDouble(max(dDisPu1, dDisPu2), 0 ,TRUE);

				//Pr
				double  dDisPr  = pCalcPile->GetLsdAxialPr(nTypePileCase);
				CString sDisPrMax = GetStrDouble(dDisPr, 0 ,TRUE);

				/////////////////////// Ãà ¾ÐÃà°ú Èû /////////////////////////
				// Pu/Pr
				double dPuPr	=  (max(dDisPu1, dDisPu2) == 0 || dDisPr == 0) ? 0 : max(dDisPu1, dDisPu2) / dDisPr;
				double dResult = max(pCalcPile->GetLsdAxialMomentResult(nTypePileCase, nCombo, TRUE), pCalcPile->GetLsdAxialMomentResult(nTypePileCase, nCombo, FALSE));
				CString sPuPr, sResult;
				sPuPr.Format(_T("%.3f"), dPuPr);
				sResult.Format(_T("%.3f"), dResult);

				//////////////////// Àü´ÜÀÀ·Â ////////////////////
				double dH1	= GetValueUnitChange(fabs(pCalcPile->GetValueMaxMinPileHead(nCombo, PILE_FORCE_HORI, -1, TRUE, TRUE, nTypePileCase, TRUE)), UNIT_CGS_TONF, pData->m_szTonf);
				double dH2	= GetValueUnitChange(fabs(pCalcPile->GetValueMaxMinPileHead(nCombo, PILE_FORCE_HORI, -1, FALSE, TRUE, nTypePileCase, TRUE)), UNIT_CGS_TONF, pData->m_szTonf);
				double dH	= max(dH1, dH2);
				double dVr	= max(pCalcPile->GetLsdShearVr(nTypePileCase, nCombo, TRUE),  pCalcPile->GetLsdShearVr(nTypePileCase, nCombo, FALSE));

				CString sH = GetStrDouble(dH, 0, TRUE);
				CString sVr = GetStrDouble(dVr, 0, TRUE);

				BOOL bOkCase = TRUE;
				if(pCalcPile->IsOkLsdShearStress(nTypePileCase, nCombo, TRUE) == FALSE) bOkCase = FALSE;
				if(pCalcPile->IsOkLsdShearStress(nTypePileCase, nCombo, FALSE) == FALSE) bOkCase = FALSE;
				if(pCalcPile->IsOkLsdAxialForce(nTypePileCase, nCombo, TRUE) == FALSE) bOkCase = FALSE;
				if(pCalcPile->IsOkLsdAxialForce(nTypePileCase, nCombo, FALSE) == FALSE) bOkCase = FALSE;
				if(pCalcPile->IsOkLsdAxialMoment(nTypePileCase, nCombo, TRUE) == FALSE) bOkCase = FALSE;
				if(pCalcPile->IsOkLsdAxialMoment(nTypePileCase, nCombo, FALSE) == FALSE) bOkCase = FALSE;

				CString strOk = bOkCase ? _T("O.K") : _T("N.G");

				XLText->AddFormatText(_T("$h$c$me1%s $m+02$c$me3%s $m+04$c$me2%s $m+03$c$me2%s $m+03$c$f3$me1%s $m+02$c$f3$me1%s $m+02$c$me2%s $m+03$c$me2%s $m+03$c$me1%s$n$h"),
					strEnv, pLoad->m_sName, sDisPuMax, sDisPrMax, sPuPr, sResult, sH, sVr, strOk);
			}
		}
	}
	// (AAB-5236) ÇöÀåÅ¸¼³¸»¶ÒÀÏ¶§´Â ÈÚºÎÀç °ËÅä·Î..
	else if(pCalcPile->m_pPile->m_nType == PILE_TYPE_HYUNJANG)
	{
		long nSize = pCalcPile->m_sMatHyunta.GetRow();

		XLText->AddTable(nSize, 22, 1, 7);

		XLText->AddFormatText(_T("$h$c$me6%s $m+07$c$me1%s $m+02$c$me1%s $m+02$c$me1%s $m+02$c$me2%s $m+03$c$me2%s $m+03$c$me1%s $m+02$c$me1%s$n$h"),
			hggettext("±¸ ºÐ"),hggettext("As,min"),hggettext("As,use"),hggettext("As,max"),hggettext("Pu(N)"),hggettext("Pn(N)"),hggettext("¾ÈÀüµµ"), hggettext("ºñ°í"));

		XLText->OutXL();

		for(long nIndex=0; nIndex<nSize; ++nIndex) //row
		{
			double dMoment = GetValueUnitChange(::hgatof(pCalcPile->m_sMatHyunta(nIndex,DESIGNPILE_HYUNTA_M)),pData->m_szTonfM,UNIT_CGS_TONFM);
			double dAxial  = GetValueUnitChange(::hgatof(pCalcPile->m_sMatHyunta(nIndex,DESIGNPILE_HYUNTA_P)),pData->m_szTonf,UNIT_CGS_TONF);
			double MMaxDepth = pCalcPile->m_pPile->m_dH0 + hgatof(pCalcPile->m_sMatHyunta(nIndex,6));

			CConcBasic *pConc = new CConcBasic;
			pConc->m_nLsdComboKind = (nIndex > 2)? 5 : -1;
			pConc->m_nTypeConcDesign = pData->GetTypeConcDesign();
			pConc->m_Fck = pCalcPile->m_pPile->m_pHyunTa->m_dHP_Fck;
			pConc->m_Fy  = pCalcPile->m_pPile->m_pHyunTa->m_dHP_Fy;
			pConc->m_nTypeUnit = pData->m_nTypeUnit;
			pConc->m_nTypeSection = SECTION_COLUMN;
			pConc->m_Ro = pCalcPile->m_pPile->m_dDia/2;
			long nDan=0; for(nDan=0; nDan<REBAR_MAX_LAYER; nDan++)
			{
				pConc->m_RbT1C_dc[nDan] = pCalcPile->m_pPile->m_pHyunTa->m_dHP_MainRebarDist[nDan];
				pConc->m_RbT1C_DIA[nDan]  = pCalcPile->m_pPile->m_pHyunTa->m_dHP_MainRebarDia[nDan];
				pConc->m_RbT1C_DIA[nDan]= pCalcPile->m_pPile->GetDiaRebarMain(MMaxDepth,nDan); //
				pConc->m_RbT1C_RE[nDan] = pCalcPile->m_pPile->m_pHyunTa->m_dHP_MainRebarEa[nDan];
			}
			pConc->m_sTitle = pCalcPile->m_sMatHyunta(nIndex,5);
			pConc->m_Axial	= fabs(dAxial);
			pConc->m_Mux	= fabs(dMoment);

			//´©¶ôÇ×¸ñÃß°¡
			BOOL bExtreme = nIndex < 3? FALSE : TRUE;
			pConc->m_Pi_C = bExtreme ? pCalcPile->m_dPI_C_Extreme : pCalcPile->m_dPI_C;
			pConc->m_Pi_S = bExtreme ? pCalcPile->m_dPI_S_Extreme : pCalcPile->m_dPI_S;
			pConc->m_Pi_V = bExtreme ? pCalcPile->m_dPI_V_Extreme : pCalcPile->m_dPI_V;

			CColumn Column(pConc, FALSE, pCalcPile->m_nSeclectPmin );	// ÇöÀåÅ¸¼³ ¸»¶Ò
			Column.CalculateAll(FALSE);

			double dAxialUnit  = GetValueUnitChange(Column.m_pData->m_Axial, UNIT_CGS_TONF, pData->m_szTonf); // GetValueTonf(m_pData->m_Axial); //pu
			double dpiPn	   = GetValueUnitChange(Column.GetPiPn(),UNIT_CGS_TONF, pData->m_szTonf);         //GetValueTonf(Column.GetPiPn()); //pn
			CString sRate = (Column.m_pData->m_Axial == 0)? _T("-") : GetStrDouble(Column.GetPiPn()/Column.m_pData->m_Axial, 3, TRUE);
			strOk = (Column.IsOkAxial() && Column.IsOK_RebarRate())? _T("O.K") : _T("N.G");

			CString sVal(_T(""));
			if(nIndex == 0 || nIndex == 3)
			{
				sVal = (nIndex == 0)? _T("±ØÇÑÇÑ°è") : _T("±Ø´Ü»óÈ²ÇÑ°è");
				XLText->AddMergeText(2, 0, sVal);
			}

			XLText->AddFormatText(_T("$h$c$me3%s $m+04$c$me2%s $m+03$c$me1%s $m+02$c$me1%s $m+02$c$me1%s $m+02$c$me2%s $m+03$c$me2%s $m+03$c$me1%s $m+02$c$me1%s$n$h"),
				sVal, 
				pConc->m_sTitle, 
				GetStrDouble(Column.GetValuePmin(), 4, TRUE),
				GetStrDouble(Column.GetRebarUsedRate(), 4, TRUE),
				GetStrDouble(Column.GetValuePmax(), 4, TRUE),
				GetStrDouble(dAxialUnit, 0, TRUE),
				GetStrDouble(dpiPn, 0, TRUE),
				sRate,
				strOk);
		}
	}
}

void CARcBridgeCalcSummary::MakeTableCheckBeding(CXLFormatText *XLText)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bSkipFirstPart = (pBri->IsBoxType() || pBri->IsRcSlab()) ? TRUE : FALSE;
	BOOL bSI = pData->IsSI();
	CString str = _T("");
	long nCntItemSlab = pBri->m_pArrConcSlab[eEnvLimit].GetSize();
	long nCntItemWall = (pBri->IsOutsideWall())? MIN(10, pBri->m_pArrConcWall[eEnvLimit].GetSize()):0;
	long nCntItemInWall = (pBri->m_nSelectInWallSectionDesign == 0)? pBri->m_pArrConcInWall[eEnvLimit].GetSize() : 0;
	long nCntItemFoot = pBri->m_pArrConcFoot[eEnvLimit].GetSize() + pBri->m_pArrConcFootRev[eEnvLimit].GetSize();
	long nCntItemArchRib = pBri->m_pArrConcArchRib.GetSize();
	long nCntItemPRF = pBri->m_pArrConcPRF[eEnvLimit].GetSize();

	long nCntRow = GetRowCountAll(0);

	str.Format("$s+%02d", nCntRow+3);
	XLText->AddFormatText("%s$h$e¡á ´Ü¸é ÈÚ ¼³°è$n$h", str);
	XLText->AddTable(nCntRow+1, 24, 2, 6);
	
	long nWidthTitle[] = { 6, 2, 2, 2, 3, 3, 3, 2, 2 };
	char *szTitleCGS[] = { "´Ü¸éÀ§Ä¡", "±íÀÌ\nH(cm)", "dc\n(cm)", "Req. As\n(§²)", "»ç¿ëÃ¶±Ù·®\n(§²)", "$tx08Mu\n(tonf.m)", "$tx08¥õMn\n(tonf.m)", "¾ÈÀüµµ", "ºñ °í" };
	char *szTitleSI[] = { "´Ü¸éÀ§Ä¡", "±íÀÌ\nH(mm)", "dc\n(mm)", "Req. As\n(§±)", "»ç¿ëÃ¶±Ù·®\n(§±)", "Mu\n(kN.m)", "¥õMn\n(kN.m)", "¾ÈÀüµµ", "ºñ °í" };
	long i=0; for(i=0; i<9; i++)
	{
		if(pData->IsSI()) XLText->AddMergeFormatText(1, nWidthTitle[i]-1, "$c%s", szTitleSI[i]);
		else XLText->AddMergeFormatText(1, nWidthTitle[i]-1, "$c%s", szTitleCGS[i]);
		XLText->AddFormatText("$m+%02d", nWidthTitle[i]);
	}
	XLText->AddFormatText("$n$n$h");

	//////////////////////////////////////////////////////////////////////////
	BOOL bBgColr = FALSE;
	CString szPre = _T("");
	CString szCom = _T("");
	CString szBkColr = _T("");

	CConcBasic *pConc;
	long nWidthData[] = { 1, 5, 2, 2, 2, 3, 3, 3, 2, 2 };
	if(bSkipFirstPart)
	{
		nWidthData[0] = 0;
		nWidthData[1] = 6;
	}

	long nLine = 0;
	long nMerge = 0;

	// ½½·¡ºê
	for(i=0; i<nCntItemSlab; i++)
	{
		pConc = pBri->m_pArrConcSlab[eEnvLimit].GetAt(i);
		if(pConc == NULL) continue;

		str = GetStringMainRebarUse(pConc, nLine);
		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		BOOL bValid = (pConc->m_Moment == 0)? FALSE:TRUE;
		BOOL bRebarRate = (pBeam.GetCheckRebarRate() == 0 || pBeam.GetCheckRebarRate() == 1);
		
		long j=0; for(j=0; j<10; j++)
		{
			nMerge = (i==0 && j==0)? GetRowCountAll(1)-1:MAX(nLine-1, 0);
			szBkColr = (pBri->IsBoxType() && j > 1 &&  i < nCntItemSlab / 2.) ? _T("$b040"): _T("");

			if(bSkipFirstPart && j==0) continue;
			else if(j==0) str = "º»\nÃ¼";
			else if(j==1) str = pConc->m_sTitle;
			else if(j==2 && bSI) str.Format("$f0%.0f", pConc->m_Ho + pConc->m_Hunch/3);
			else if(j==2 && !bSI) str.Format("$f1%.1f", toCm(pConc->m_Ho + pConc->m_Hunch/3));
			else if(j==3 && bSI) str.Format("$f0%.0f", pConc->m_RbT1C_dc[0]);
			else if(j==3 && !bSI) str.Format("$f1%.1f", toCm(pConc->m_RbT1C_dc[0]));
			else if(j==4)
			{
				if(bValid && bSI) str.Format("$f1%.1f", pBeam.GetRebarAreaRequired());
				else if(bValid && !bSI) str.Format("$f3%.3f", pBeam.GetRebarAreaRequired());
				else str = "-";
			}
			else if(j==5) str = GetStringMainRebarUse(pConc, nLine);
			else if(j==6) str.Format("$f3%.3f", pBeam.GetMu());
			else if(j==7) str.Format("$f3%.3f", pBeam.GetPiMn());
			else if(j==8)
			{
				if(bValid) str.Format("$f3%.3f", pBeam.GetSafeFactorMoment());
				else str = "-";
			}
			else if(j==9)
			{
				if(bValid) str = (pBeam.IsOkMoment() && bRebarRate)? "O.K":"$#03N.G";
				else str = "-";
			}
			else str = _T("");

			if(j>7 && Compare(pBeam.GetMu(), 0.0, _T("<="), 0.01)) str = _T("-");
			XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
		}
		XLText->AddFormatText("$n$h");
		*XLText += MAX(nLine-1, 0);
	}

	if(pBri->IsRcSlab() == FALSE)
	{
		// º®Ã¼
		for(i=0; i<nCntItemWall; i++)
		{
			pConc = pBri->GetConcDataWall(i, TRUE);
			if(pConc == NULL) continue;
			BOOL bCalcWall = (pConc->m_nTypeSection==SECTION_WALL)? TRUE : FALSE;
			
			str = GetStringMainRebarUse(pConc, nLine);
			CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
			BOOL bValid = (pConc->m_Moment == 0)? FALSE:TRUE;
			BOOL bRebarRate = (pBeam.GetCheckRebarRate() == 0 || pBeam.GetCheckRebarRate() == 1);
			szCom = pConc->m_sTitle.Left(6);
			if(szPre != szCom) bBgColr = !bBgColr;
			szPre = szCom;

			CColumn pWall(pConc);
			if(bCalcWall) pWall.CalculateAll(FALSE);
			
			long j=0; for(j=0; j<10; j++)
			{
				nMerge = (i==0 && j==0)? 0:MAX(nLine-1, 0);
				szBkColr = (j > 1 && bBgColr) ? _T("$b040"): _T("");
				
				if(bSkipFirstPart && j==0) continue;
				else if(j==0) str = "º»\nÃ¼";
				else if(j==1) str = pConc->m_sTitle;
				else if(j==2 && bSI) str.Format("$f0%.0f", pConc->m_Ho + pConc->m_Hunch/3);
				else if(j==2 && !bSI) str.Format("$f1%.1f", toCm(pConc->m_Ho + pConc->m_Hunch/3));
				else if(j==3 && bSI) str.Format("$f0%.0f", pConc->m_RbT1C_dc[0]);
				else if(j==3 && !bSI) str.Format("$f1%.1f", toCm(pConc->m_RbT1C_dc[0]));
				else if(j==4)
				{
					if(bCalcWall) str = "-";
					else if(bValid && bSI) str.Format("$f1%.1f", pBeam.GetRebarAreaRequired());
					else if(bValid && !bSI) str.Format("$f3%.3f", pBeam.GetRebarAreaRequired());
					else str = "-";
				}
				else if(j==5) str = GetStringMainRebarUse(pConc, nLine);
				else if(j==6)
				{
					if(bCalcWall) str.Format("$f2%.3f", GetValueUnitChange(pConc->m_Axial,UNIT_CGS_TONF, pData->m_szTonf));
					else str.Format("$f3%.3f", pBeam.GetMu());
				}
				else if(j==7)
				{
					if(bCalcWall) str.Format("$f2%.3f", GetValueUnitChange(pWall.GetPiPn(),UNIT_CGS_TONF, pData->m_szTonf));
					else str.Format("$f3%.3f", pBeam.GetPiMn());
				}
				else if(j==8)
				{
					if(bCalcWall) str.Format("$f3%.3f", pWall.GetSafeFactorAxial());
					else if(bValid) str.Format("$f3%.3f", pBeam.GetSafeFactorMoment());
					else str = "-";
				}
				else if(j==9)
				{
					if(bCalcWall) str = (pWall.IsOkAxial())? "O.K":"$#03N.G";
					else if(bValid) str = (pBeam.IsOkMoment() && bRebarRate)? "O.K":"$#03N.G";
					else str = "-";
				}
				else str = _T("");
				
				if(j>7 &&  Compare(pBeam.GetMu(), 0.0, _T("<="), 0.01)) str = _T("-");
				XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
				XLText->AddFormatText("$m+%02d", nWidthData[j]);
			}
			XLText->AddFormatText("$n$h");
			*XLText += MAX(nLine-1, 0);
		}

		// ³»Ãøº®Ã¼
		for(i=0; i<nCntItemInWall; i++)
		{
			pConc = pBri->m_pArrConcInWall[eEnvLimit].GetAt(i);
			if(pConc == NULL) continue;
	
			str = GetStringMainRebarUse(pConc, nLine);
			CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
			BOOL bValid = (pConc->m_Moment == 0)? FALSE:TRUE;
			BOOL bRebarRate = (pBeam.GetCheckRebarRate() == 0 || pBeam.GetCheckRebarRate() == 1);
			szCom = pConc->m_sTitle.Left(6);
			if(szPre != szCom) bBgColr = !bBgColr;
			szPre = szCom;
		
			long j=0; for(j=0; j<10; j++)
			{
				nMerge = (i==0 && j==0)? 0:MAX(nLine-1, 0);
				szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");
				
				if(bSkipFirstPart && j==0) continue;
				else if(j==0) str = "º»\nÃ¼";
				else if(j==1) str = pConc->m_sTitle;
				else if(j==2 && bSI) str.Format("$f0%.0f", pConc->m_Ho + pConc->m_Hunch/3);
				else if(j==2 && !bSI) str.Format("$f1%.1f", toCm(pConc->m_Ho + pConc->m_Hunch/3));
				else if(j==3 && bSI) str.Format("$f0%.0f", pConc->m_RbT1C_dc[0]);
				else if(j==3 && !bSI) str.Format("$f1%.1f", toCm(pConc->m_RbT1C_dc[0]));
				else if(j==4)
				{
					if(bValid && bSI) str.Format("$f1%.1f", pBeam.GetRebarAreaRequired());
					else if(bValid && !bSI) str.Format("$f3%.3f", pBeam.GetRebarAreaRequired());
					else str = "-";
				}
				else if(j==5) str = GetStringMainRebarUse(pConc, nLine);
				else if(j==6)
				{
					str.Format("$f3%.3f", pBeam.GetMu());
				}
				else if(j==7)
				{
					str.Format("$f3%.3f", pBeam.GetPiMn());
				}
				else if(j==8)
				{
					if(bValid) str.Format("$f3%.3f", pBeam.GetSafeFactorMoment());
					else str = "-";
				}
				else if(j==9)
				{
					if(bValid) str = (pBeam.IsOkMoment() && bRebarRate)? "O.K":"$#03N.G";
					else str = "-";
				}
				else str = _T("");
				
				if(j>7 && Compare(pBeam.GetMu(), 0.0, _T("<="), 0.01)) str = _T("-");
				XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
				XLText->AddFormatText("$m+%02d", nWidthData[j]);
			}
			XLText->AddFormatText("$n$h");
			*XLText += MAX(nLine-1, 0);
		}
		//ºÎ»ó¹æÁöÀúÆÇ
		for(i = 0; i < nCntItemPRF; ++i)
		{
			pConc = pBri->m_pArrConcPRF[eEnvLimit].GetAt(i);
			if(pConc == NULL) continue;

			str = GetStringMainRebarUse(pConc, nLine);
			CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
			BOOL bValid = (pConc->m_Moment == 0)? FALSE:TRUE;
			BOOL bRebarRate = (pBeam.GetCheckRebarRate() == 0 || pBeam.GetCheckRebarRate() == 1);

			if(i == 0)
				bBgColr = !bBgColr;

			for(long j=0; j<10; j++)
			{
				nMerge = (i==0 && j==0)?0:MAX(nLine-1, 0);
				
				if(bSkipFirstPart && j==0) continue;
				else if(j==0) str = "º»\nÃ¼";
				else if(j==1) str = pConc->m_sTitle;
				else if(j==2 && bSI) str.Format("$f0%.0f", pConc->m_Ho + pConc->m_Hunch/3);
				else if(j==2 && !bSI) str.Format("$f1%.1f", toCm(pConc->m_Ho + pConc->m_Hunch/3));
				else if(j==3 && bSI) str.Format("$f0%.0f", pConc->m_RbT1C_dc[0]);
				else if(j==3 && !bSI) str.Format("$f1%.1f", toCm(pConc->m_RbT1C_dc[0]));
				else if(j==4)
				{
					if(bValid && bSI) str.Format("$f1%.1f", pBeam.GetRebarAreaRequired());
					else if(bValid && !bSI) str.Format("$f3%.3f", pBeam.GetRebarAreaRequired());
					else str = "-";
				}
				else if(j==5) str = GetStringMainRebarUse(pConc, nLine);
				else if(j==6) str.Format("$f3%.3f", pBeam.GetMu());
				else if(j==7) str.Format("$f3%.3f", pBeam.GetPiMn());
				else if(j==8)
				{
					if(bValid) str.Format("$f3%.3f", pBeam.GetSafeFactorMoment());
					else str = "-";
				}
				else if(j==9)
				{
					if(bValid) str = (pBeam.IsOkMoment() && bRebarRate)? "O.K":"$#03N.G";
					else str = "-";
				}
				else str = _T("");

				szBkColr = (j > 1 && bBgColr) ? _T("$b040"): _T("");

				if(j>7 && Compare(pBeam.GetMu(), 0.0, _T("<="), 0.01)) str = _T("-");
				XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
				XLText->AddFormatText("$m+%02d", nWidthData[j]);

				XLText->OutXL();
			}
			XLText->AddFormatText("$n$h");
			*XLText += MAX(nLine-1, 0);
		}
		// ¾ÆÄ¡¸®ºê
		for(i=0; i<nCntItemArchRib; i++)
		{
			pConc = pBri->m_pArrConcArchRib.GetAt(i);
			if(pConc == NULL) continue;
			
			if(i==0) bBgColr = !bBgColr;
			str = GetStringMainRebarUse(pConc, nLine);
			CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
			BOOL bValid = (pConc->m_Moment == 0)? FALSE:TRUE;
			BOOL bRebarRate = (pBeam.GetCheckRebarRate() == 0 || pBeam.GetCheckRebarRate() == 1);
			
			long j=0; for(j=0; j<10; j++)
			{
				nMerge = (i==0 && j==0)? GetRowCountAll(4)-1:MAX(nLine-1, 0);
				
				if(bSkipFirstPart && j==0) continue;
				else if(j==0) str = "¾Æ\nÄ¡\n¸®\nºê";
				else if(j==1) str = pConc->m_sTitle;
				else if(j==2 && bSI) str.Format("$f0%.0f", pConc->m_Ho + pConc->m_Hunch/3);
				else if(j==2 && !bSI) str.Format("$f1%.1f", toCm(pConc->m_Ho));
				else if(j==3 && bSI) str.Format("$f0%.0f", pConc->m_RbT1C_dc[0]);
				else if(j==3 && !bSI) str.Format("$f1%.1f", toCm(pConc->m_RbT1C_dc[0]));
				else if(j==4)
				{
					if(bValid && bSI) str.Format("$f1%.1f", pBeam.GetRebarAreaRequired());
					else if(bValid && !bSI) str.Format("$f3%.3f", pBeam.GetRebarAreaRequired());
					else str = "-";
				}
				else if(j==5) str = GetStringMainRebarUse(pConc, nLine);
				else if(j==6) str.Format("$f3%.3f", pBeam.GetMu());
				else if(j==7) str.Format("$f3%.3f", pBeam.GetPiMn());
				else if(j==8)
				{
					if(bValid) str.Format("$f3%.3f", pBeam.GetSafeFactorMoment());
					else str = "-";
				}
				else if(j==9)
				{
					if(bValid) str = (pBeam.IsOkMoment() && bRebarRate)? "O.K":"$#03N.G";
					else str = "-";
				}
				else str = _T("");
				
				if(j>7 && Compare(pBeam.GetMu(), 0.0, _T("<="), 0.01)) str = _T("-");
				szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");
				XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
				XLText->AddFormatText("$m+%02d", nWidthData[j]);
			}
			XLText->AddFormatText("$n$h");
			*XLText += MAX(nLine-1, 0);
		}
		// ±âÃÊ
		BOOL bRev = FALSE;
		for(i=0; i<nCntItemFoot; i++)
		{	

			long nInd = i/2;
			if(!bRev) pConc = pBri->m_pArrConcFoot[eEnvLimit].GetAt(nInd);
			else pConc = pBri->m_pArrConcFootRev[eEnvLimit].GetAt(nInd);

			BOOL bUpper = (pConc->m_sTitle.Right(6) == _T("(µÞ±Á)"))? !bRev : bRev;
			CString strSub = (!bUpper)? _T("-ÇÏ¸é") : _T("-»ó¸é");


			bRev = !bRev;
			if(!bRev && pConc->m_sTitle.Left(4) == "ÁöÁ¡") continue;

			if(pConc == NULL) continue;
			
			str = GetStringMainRebarUse(pConc, nLine);
			CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper, pBri->m_bCheckRebarRateFooting);
			BOOL bValid = (pConc->m_Moment == 0) ? FALSE : TRUE;
			BOOL bRebarRate = (pBeam.GetCheckRebarRate() == 0 || pBeam.GetCheckRebarRate() == 1);

			szCom = pConc->m_sTitle.Left(6);
			if(szPre != szCom) bBgColr = !bBgColr;
			szPre = szCom;
			
			long j=0; for(j=0; j<10; j++)
			{
				nMerge = (i==0 && j==0)? GetRowCountAll(2)-1:MAX(nLine-1, 0);
				szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");
				
				if(j==0) str = "±â\nÃÊ";
				else if(j==1) str = pConc->m_sTitle+strSub;
				else if(j==2 && bSI) str.Format("$f0%.0f", pConc->m_Ho);
				else if(j==2 && !bSI) str.Format("$f1%.1f", toCm(pConc->m_Ho));
				else if(j==3 && bSI) str.Format("$f0%.0f", pConc->m_RbT1C_dc[0]);
				else if(j==3 && !bSI) str.Format("$f1%.1f", toCm(pConc->m_RbT1C_dc[0]));
				else if(j==4)
				{
					if(bValid && bSI) str.Format("$f1%.1f", pBeam.GetRebarAreaRequired());
					else if(bValid && !bSI) str.Format("$f3%.3f", pBeam.GetRebarAreaRequired());
					else str = "-";
				}
				else if(j==5) str = GetStringMainRebarUse(pConc, nLine);
				else if(j==6) str.Format("$f3%.3f", pBeam.GetMu());
				else if(j==7) str.Format("$f3%.3f", pBeam.GetPiMn());
				else if(j==8)
				{
					if(bValid) str.Format("$f3%.3f", pBeam.GetSafeFactorMoment());
					else str = "-";
				}
				else if(j==9)
				{
					if(bValid) str = (pBeam.IsOkMoment() && bRebarRate)? "O.K":"$#03N.G";
					else str = "-";
				}
				else str = _T("");
				
				if(j>7 && Compare(pBeam.GetMu(), 0.0, _T("<="), 0.01)) str = _T("-");

				XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
				XLText->AddFormatText("$m+%02d", nWidthData[j]);
			}
			XLText->AddFormatText("$n$h");
			*XLText += MAX(nLine-1, 0);
		}
		// ³¯°³º®
		BOOL bFirst = TRUE;
		long k=0; for(k=0; k<4; k++)
		{
			if(!pBri->IsWingWall(k==0||k==1, k==0||k==2)) continue;
			
			for(i=0; i<5; i++)
			{
				if(k==0) pConc = pBri->m_pArrConcWingSttLeft.GetAt(i);
				else if(k==1) pConc = pBri->m_pArrConcWingSttRight.GetAt(i);
				else if(k==2) pConc = pBri->m_pArrConcWingEndLeft.GetAt(i);
				else if(k==3) pConc = pBri->m_pArrConcWingEndRight.GetAt(i);
				
				if(pConc == NULL || pConc->m_Moment == 0) continue;
				
				str = GetStringMainRebarUse(pConc, nLine);
				CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
				BOOL bValid = (pConc->m_Moment == 0)? FALSE:TRUE;
				BOOL bRebarRate = (pBeam.GetCheckRebarRate() == 0 || pBeam.GetCheckRebarRate() == 1);

				szCom = pConc->m_sTitle.Left(6);
				if(szPre != szCom) bBgColr = !bBgColr;
				szPre = szCom;
				
				long j=0; for(j=0; j<10; j++)
				{
					nMerge = (bFirst) ? GetRowCountAll(3)-1 : MAX(nLine-1, 0);
					szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");
					bFirst = FALSE;
					
					if(bSkipFirstPart && j==0) continue;
					else if(j==0) str = "³¯\n°³\nº®";
					else if(j==1) str = pConc->m_sTitle;
					else if(j==2 && bSI) str.Format("$f0%.0f", pConc->m_Ho);
					else if(j==2 && !bSI) str.Format("$f1%.1f", toCm(pConc->m_Ho));
					else if(j==3 && bSI) str.Format("$f0%.0f", pConc->m_RbT1C_dc[0]);
					else if(j==3 && !bSI) str.Format("$f1%.1f", toCm(pConc->m_RbT1C_dc[0]));
					else if(j==4)
					{
						if(bValid && bSI) str.Format("$f1%.1f", pBeam.GetRebarAreaRequired());
						else if(bValid && !bSI) str.Format("$f3%.3f", pBeam.GetRebarAreaRequired());
						else str = "-";
					}
					else if(j==5) str = GetStringMainRebarUse(pConc, nLine);
					else if(j==6) str.Format("$f3%.3f", pBeam.GetMu());
					else if(j==7) str.Format("$f3%.3f", pBeam.GetPiMn());
					else if(j==8)
					{
						if(bValid) str.Format("$f3%.3f", pBeam.GetSafeFactorMoment());
						else str = "-";
					}
					else if(j==9)
					{
						if(bValid) str = (pBeam.IsOkMoment() && bRebarRate)? "O.K":"$#03N.G";
						else str = "-";
					}
					else str = _T("");
					
					if(j>7 && Compare(pBeam.GetMu(), 0.0, _T("<="), 0.01)) str = _T("-");
					XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e %s%s" : "$c%s%s", str, szBkColr); 
					XLText->AddFormatText("$m+%02d", nWidthData[j]);
				}
				XLText->AddFormatText("$n$h");
				*XLText += MAX(nLine-1, 0);
			}
		}
	}
	*XLText += 1;
}

void CARcBridgeCalcSummary::MakeTableCheckBeding_Lsd(CXLFormatText *XLText)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CString str = _T("");
	long nRowSlab = 0;
	long nRowLoSlab = 0;
	long nRowWallStt = 0;
	long nRowWallEnd = 0;
	long nRowInWall = 0;
	long nRowArchRib = 0;
	long nRowPRF = 0;
	long nRowWing = 0;
	long nCntRow = 0;
	long nCntCol = 0;

	BOOL bSkipFirstPart = (pBri->IsRcSlab()) ? TRUE : FALSE;

	CDoubleArray dItemFootingTitleMerge;		// ±âÃÊ ¸ÓÁö °³¼ö
	long nCntItemSlab		= pBri->GetCountConcUpperAndLowerSlab(TRUE);
	long nCntItemLoSlab	= pBri->IsBoxType()? pBri->GetCountConcUpperAndLowerSlab(FALSE) : 0;

	long nCntItemWall = (pBri->IsOutsideWall())? pBri->GetCountConcOutWall():0;
	long nCntItemInWall = pBri->GetCountConcInWall();
	long nCntItemArchRib = pBri->m_pArrConcArchRib.GetSize();
	long nCntItemPRF = pBri->GetCountConcPRF();
	long nCntItemFoot = pBri->GetCountFooting();

	long nCntItemRow = 1;
	const HGINT32 icValidEnvType = pBri->GetCountValidComboEnvType();

	CString strEnv;

	long nIdxSttArchRib = nCntItemSlab+nCntItemLoSlab+nCntItemWall+nCntItemInWall;
	long nIdxSttPRF = nCntItemSlab+nCntItemLoSlab+nCntItemWall+nCntItemInWall+nCntItemArchRib;

	// ½½·¡ºê, ¿Üº®, ³»º®, ¾ÆÄ¡¸®ºê, ºÎ»ó¹æÁöÀúÆÇ
	long i=0; for(i=0; i<nCntItemSlab+nCntItemLoSlab+nCntItemWall+nCntItemInWall+nCntItemArchRib+nCntItemPRF; i++)
	{
		BOOL bSlab = (i<nCntItemSlab+nCntItemLoSlab)? TRUE:FALSE;
		
		nCntRow += nCntItemRow;
		if(bSlab) 
		{
			if(pBri->IsBoxType())
			{
				BOOL bLoSlab = (i<nCntItemSlab+nCntItemLoSlab && nCntItemSlab <= i)? TRUE:FALSE;
				if(bLoSlab)
					nRowLoSlab += nCntItemRow;
				else
					nRowSlab += nCntItemRow;
			}
			else
			{
				nRowSlab += nCntItemRow;
			}			
		}
		else if(i<nCntItemSlab+nCntItemLoSlab+nCntItemWall/2)
			nRowWallStt += nCntItemRow;
		else if(i<nCntItemSlab+nCntItemLoSlab+nCntItemWall)
			nRowWallEnd += nCntItemRow;
		else if(i < nIdxSttArchRib)
			nRowInWall += nCntItemRow;
		else if(i < nIdxSttPRF)
			nRowArchRib += nCntItemRow;
		else
			nRowPRF += nCntItemRow;
	}

	// ±âÃÊ - ±âÃÊ´Â ¾Õ¸é, µÞ¸é, »ó¸é, ÇÏ¸éÀ» °í·ÁÇÏ¿© µû·Î Ã³¸®ÇÑ´Ù.
	long nCntJijum = pBri->GetCountJijum();
	for (i=0; i<nCntJijum; ++i)
	{
		if( pBri->IsFootingJijum(i) == FALSE ) continue;

		long nCntRowFoot(0);
		long nMergeCnt(0);
		for (long nFront=0; nFront < 2; ++nFront)
		{
			for (long nUpper=0; nUpper<2; ++nUpper)
			{
				for (HGINT32 ixEnvType = 0; ixEnvType < eEnvService; ++ixEnvType )
				{
					if( pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(ixEnvType)) == FALSE ) continue;

 					CConcBasic *pConc = pBri->GetConcDataFooting(i, nFront==0, nUpper==0, -1, static_cast<ETypeEnvComboLsd>(ixEnvType));
 					if( pConc == NULL ) continue;

					nCntRow += nCntItemRow;
					nCntRowFoot += nCntItemRow;
					nMergeCnt++;
				}
			}
		}

		dItemFootingTitleMerge.Add(nCntRowFoot);
	}

	// ³¯°³º®
	long nArrArea[5] = { AREA_D, AREA_A, AREA_B, AREA_C, AREA_A_ };

	for (long ixStt = 0; ixStt < 2; ++ixStt)
	{
		for (long ixLeft = iLEFT; ixLeft <= iRIGHT; ++ixLeft)
		{
			long nIndex(0);
			if ( ixStt == 0 && ixLeft==iLEFT ) 
				nIndex = 0;
			else if( ixStt == 0 && ixLeft == iRIGHT)
				nIndex = 1;
			else if( ixStt == 1 && ixLeft == iLEFT)
				nIndex = 2;
			else
				nIndex = 3;

			if(pBri->IsWingWall(ixStt==0, ixLeft==0) == FALSE) continue;
			m_pStd->m_pWing[nIndex]->CalculateAll(ixStt==0, ixLeft==iLEFT);

			for(long ix=0; ix<5; ++ix)
			{
				long nArea = nArrArea[ix];
				if(nArea == AREA_D && !m_pStd->m_pWing[nIndex]->m_bIsD) continue;
				else if(nArea == AREA_A_ && !m_pStd->m_pWing[nIndex]->m_bIsA_) continue;
				else if(nArea < AREA_D && !m_pStd->m_pWing[nIndex]->m_bIsA) continue;
				else if(nArea == AREA_A && !m_pStd->m_pWing[nIndex]->m_bIsA_Calc) continue;

				nCntRow += nCntItemRow;
				nRowWing += nCntItemRow;
			}
		}
	}

	str.Format("$s+%02d", nCntRow+3);
	XLText->AddFormatText("%s$h$e¡á ´Ü¸é ÈÚ ¼³°è$n$h", str);
	XLText->AddTable(nCntRow+1, 25-1, 2, 6);

	long nWidthTitle[] = { 6, 2, 2, 2, 3, 4, 4, 2 };
	char *szTitle[] = { "´Ü¸éÀ§Ä¡", "±íÀÌ\nH(mm)", "dc\n(mm)", "Req. As\n(§±)", "»ç¿ëÃ¶±Ù·®\n(§±)", "Mu\n(N.mm)", "Mr\n(N.mm)", "¾ÈÀüµµ" };
	for(i=0; i<8; i++)
	{
		XLText->AddMergeFormatText(1, nWidthTitle[i]-1, "$c%s", szTitle[i]);
		XLText->AddFormatText("$m+%02d", nWidthTitle[i]);
	}
	XLText->AddFormatText("$n$n$h");
	XLText->OutXL();
	
	//////////////////////////////////////////////////////////////////////////
	BOOL bBgColr = FALSE;
	CString szPre = _T("");
	CString szCom = _T("");
	CString szBkColr = _T("");

	long nWidthData[] = { 1, 3, 2, 2, 2, 2, 3, 4, 4, 2 };
	if(bSkipFirstPart)
	{
		nWidthData[0] = 0;
		nWidthData[1] = 4;
	}

	long nLine = 0;
	nCntCol = 10;
	BOOL bIsOutWall = (pBri->IsOutsideWall());
	////////////////////////////////////////////////////////////////////////// Data
	// ½½·¡ºê, º®Ã¼, ¾ÆÄ¡¸®ºê, ºÎ»ó¹æÁöÀúÆÇ
// 	BOOL bExistCalcWall = FALSE;
	for(i=0; i<nCntItemSlab+nCntItemLoSlab+nCntItemWall+nCntItemInWall+nCntItemArchRib+nCntItemPRF; i++)
	{
		BOOL bSlab = (i<nCntItemSlab+nCntItemLoSlab) ? TRUE : FALSE;
		BOOL bInWall = (nCntItemInWall > 0 &&  i >= (nCntItemSlab+nCntItemLoSlab+nCntItemWall) && i < nIdxSttArchRib) ? TRUE : FALSE;

		if(pBri->IsRcSlab() == TRUE && bSlab == FALSE )
			continue;

		CConcBasic *pConc = NULL;
		if(bSlab)
		{
			pConc = pBri->GetConcDataSlab_Lsd(i, &strEnv);
		}
		else if(i<(nCntItemSlab + nCntItemLoSlab+nCntItemWall))
		{
			pConc = pBri->GetConcDataWall_Lsd(i-nCntItemSlab-nCntItemLoSlab, &strEnv);
		}
		else if(i < nIdxSttArchRib)
			pConc = pBri->GetConcDataInWall_Lsd(i-nCntItemSlab-nCntItemLoSlab-nCntItemWall, &strEnv);
		else if(i < nIdxSttPRF)
			pConc = pBri->m_pArrConcArchRib.GetAt(i-nIdxSttArchRib);
		else
			pConc = pBri->GetConcDataPRF_Lsd(i-nIdxSttPRF, &strEnv);
		if(pConc == NULL) continue;

		CLsdBeam pBeamLsd(pConc, strEnv == "±ØÇÑ" ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN);

		BOOL bValid = (pConc->m_Moment == 0)? FALSE:TRUE;

		long j=0; for(j=0; j<nCntCol; j++)
		{
			long nMerge = 0;
			if(j == 0)
			{
				if(bSkipFirstPart) continue;

				else if(bSlab) 
				{
					if(pBri->IsBoxType())
					{
						BOOL bLoSlab = (i<nCntItemSlab+nCntItemLoSlab && nCntItemSlab <=  i)? TRUE:FALSE;
						str = bLoSlab? "ÇÏºÎ½½·¡ºê" : "»óºÎ½½·¡ºê";
					}
					else
					{
						str = "½½·¡ºê";
					}					
				}
				else if(i-nCntItemSlab-nCntItemLoSlab<nCntItemWall / 2 && bIsOutWall) str = "½ÃÁ¡º®Ã¼";
				else if(i-nCntItemSlab-nCntItemLoSlab<nCntItemWall && bIsOutWall) str = "Á¾Á¡º®Ã¼";
				else if(i-nCntItemSlab-nCntItemLoSlab-nCntItemWall<nCntItemInWall && bInWall) str = "³»Ãøº®Ã¼";
				else if(i-nCntItemSlab-nCntItemLoSlab-nCntItemWall-nCntItemInWall<nCntItemArchRib) str = "¾ÆÄ¡¸®ºê";
				else 
					str = _T("ºÎ»ó¹æÁöÀúÆÇ");

				if(i==0)																	nMerge = nRowSlab - 1;								
				else if(pBri->IsBoxType() && i==nCntItemSlab &&  bSlab)						nMerge = nRowLoSlab-1;
				else if(nCntItemWall > 0 && i==nCntItemSlab+nCntItemLoSlab && bIsOutWall)	nMerge = nRowWallStt-1;
				else if(nCntItemWall > 0 && i==nCntItemSlab+nCntItemLoSlab+nCntItemWall/2 && bIsOutWall) nMerge = nRowWallEnd-1;
				else if(nCntItemInWall > 0 && ((i==nCntItemSlab+nCntItemLoSlab+nCntItemWall && bInWall) || (i==nCntItemSlab+nCntItemLoSlab && !bInWall))) nMerge = nRowInWall-1; //+nCntItemInWall
				else if(nRowArchRib > 0 && (i==nCntItemSlab+nCntItemLoSlab+nCntItemWall && bIsOutWall) || (i==nCntItemSlab+nCntItemLoSlab && !bIsOutWall)) nMerge = nRowArchRib-1;
				else if(nRowPRF > 0 && i == nIdxSttPRF) nMerge = nRowPRF-1;
				else nMerge = 0;
			}
			else if(j == 1)
			{
				HGINT32 ix = pConc->m_sTitle.Find(" ");
				if(ix > 0)
				{
					str = pConc->m_sTitle.Mid(ix);
				}
				else
				{
					str = pConc->m_sTitle;
				}
		
				ix = str.Find("(");
				if (ix > 0)
				{
					str.Insert(ix, "\n");
				}
			}
			else if (j == 2)
			{
				str = strEnv;
			}
			else if(j == 3)
			{
				// ±æÀÌ
				str.Format("$f1%.1f", pConc->m_Ho + pConc->m_Hunch/3);
			}
			else if(j == 4)
			{
				str.Format("$f1%.1f", pConc->m_RbT1C_dc[0]);		// dc
			}
			else if(j==5)
			{
				if(bValid) str.Format("$f1%.1f", pBeamLsd.GetAsReq());
				else str = "-";
			}
			else if(j==6) str.Format("$f1%.1f", pBeamLsd.GetAsUse());
			else if(j==7) str = pData->GetStrDouble(pBeamLsd.GetMu(), UNIT_CGS_TONFM);
			else if(j==8) str = pData->GetStrDouble(pBeamLsd.GetMn(), UNIT_CGS_TONFM);
			else if(j==9)
			{
				if(bValid) str.Format("$f3%.3f", pBeamLsd.GetMn() / pBeamLsd.GetMu());
				else str = "-";
			}

			// Ãâ·Â
			if(j > 2 && pBeamLsd.GetMu() <= 0.0) str = _T("-");
			if (j == 1 && icValidEnvType > 1 && i % icValidEnvType == 0)
			{
				HGINT32 icMerge = 0;
				for (HGINT32 ix = 0; ix < icValidEnvType; ++ix)
				{
					// (ARCBRIDGE-2310) ¹«Á¶°Ç ÇÑÁÙ Ãâ·ÂÀÌ´Ù.
					icMerge += 1;// static_cast<HGINT32>(dItemRowArr.GetAt(i+ix));
				}

				XLText->AddMergeFormatText(icMerge - 1, nWidthData[j]-1, "$j$c%s", str);
				XLText->OutXL();
			}
			else
			{
				XLText->AddMergeFormatText(0, nWidthData[j]-1, "$j$c%s", str);
				XLText->OutXL();
			}

			XLText->Merge(nMerge, 0);
			XLText->AddFormatText("$m+0%d", nWidthData[j]);
		}

		XLText->AddText("$n$h");
		XLText->OutXL();
	}

	//////////////////////////////////////////////////////////////////////////
	// ±âÃÊ
	long nIndex(0);
	for(i=0; i<nCntJijum; i++)
	{
		if( pBri->IsFootingJijum(i) == FALSE ) continue;

		HGBOOL bTitle = FALSE;

		for (long nFront=0; nFront<2; ++nFront)
		{
			for (long nUpper=0; nUpper<2; ++nUpper)
			{
				HGBOOL bSubTitle = FALSE;

				for (HGINT32 ixEnvType = 0; ixEnvType < eEnvService; ++ixEnvType )
				{
					if( pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(ixEnvType)) == FALSE ) continue;

					if( ixEnvType == eEnvLimit ) strEnv = "±ØÇÑ";
					else if( ixEnvType == eEnvExtremeI ) strEnv = "±Ø´ÜI";
					else strEnv = "±Ø´Ü¥±";

					CConcBasic *pConc = pBri->GetConcDataFooting(i, nFront==0, nUpper==0, -1, static_cast<ETypeEnvComboLsd>(ixEnvType));
					if( pConc == NULL ) continue;

					BOOL bValid = (pConc->m_Moment == 0)? FALSE:TRUE;

					CLsdBeam pBeamLsd(pConc, strEnv == "±ØÇÑ" ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN);

					long j=0; for(j=0; j<nCntCol; j++)
					{
						long nMerge = 0;
						if(j == 0)
						{
							if( i==0 ) str = "½Ã\nÁ¡\nÃø\n±â\nÃÊ";
							else if( i==nCntItemFoot-1 ) str = "Á¾\nÁ¡\nÃø\n±â\nÃÊ";
							else str.Format("Áß\n°£\nÁö\nÁ¡\n±â\nÃÊ (%d)", i);

							if( bTitle == FALSE )
								nMerge = static_cast<long>(dItemFootingTitleMerge.GetAt(nIndex)) - 1;
						}
						else if(j == 1)
						{
							CString strTemp1 = nFront==0 ? "¾Õ±Á" : "µÞ±Á";
							CString strTemp2 = ((nFront==0) == (nUpper==0)) ? "(»ó¸é)" : "(ÇÏ¸é)";

							str = strTemp1 + strTemp2;

							if( bSubTitle == FALSE )
							{
								nMerge = -1;
								for (HGINT32 ixT = 0; ixT < eEnvService; ++ixT )
								{
									if( pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(ixT)) == FALSE ) continue;
									nMerge += 1;
								}
							}
							else
								nMerge = 0;

							bSubTitle = TRUE;
						}
						else if (j == 2)
						{
							str = strEnv;
						}
						else if(j == 3)
						{
							// ±æÀÌ
							str.Format("$f1%.1f", pConc->m_Ho + pConc->m_Hunch/3);
						}
						else if(j == 4)
						{
							str.Format("$f1%.1f", pConc->m_RbT1C_dc[0]);		// dc
						}
						else if(j==5)
						{
							if(bValid) str.Format("$f1%.1f", pBeamLsd.GetAsReq());
							else str = "-";
						}
						else if(j==6) str.Format("$f1%.1f", pBeamLsd.GetAsUse());
						else if(j==7) str = pData->GetStrDouble(pBeamLsd.GetMu(), UNIT_CGS_TONFM);
						else if(j==8) str = pData->GetStrDouble(pBeamLsd.GetMn(), UNIT_CGS_TONFM);
						else if(j==9)
						{
							if(bValid) str.Format("$f3%.3f", pBeamLsd.GetMn() / pBeamLsd.GetMu());
							else str = "-";
						}

						// Ãâ·Â
						if(j > 2 && pBeamLsd.GetMu() <= 0.0) str = _T("-");
						XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, "$j$c%s", str);
						XLText->AddFormatText("$m+%02d", nWidthData[j]);
						bTitle = TRUE;
					}

					XLText->AddText("$n$h");
				}
			}
		}

		nIndex++;
	}
	//////////////////////////////////////////////////////////////////////////
	// ³¯°³º®
	HGBOOL bTitle = FALSE;
	strEnv = "±ØÇÑ";
	for (long ixStt = 0; ixStt < 2; ++ixStt )
	{
		for (long ixLeft = iLEFT; ixLeft <= iRIGHT; ++ixLeft )
		{
			long nIndex(0);
			if ( ixStt == 0 && ixLeft==iLEFT ) 
				nIndex = 0;
			else if( ixStt == 0 && ixLeft == iRIGHT)
				nIndex = 1;
			else if( ixStt == 1 && ixLeft == iLEFT)
				nIndex = 2;
			else
				nIndex = 3;

			if(pBri->IsWingWall(ixStt==0, ixLeft==0) == FALSE) continue;
			m_pStd->m_pWing[nIndex]->CalculateAll(ixStt==0, ixLeft==iLEFT);

			for(long ix=0; ix<5; ++ix)
			{
				long nArea = nArrArea[ix];
				if(nArea == AREA_D && !m_pStd->m_pWing[nIndex]->m_bIsD) continue;
				else if(nArea == AREA_A_ && !m_pStd->m_pWing[nIndex]->m_bIsA_) continue;
				else if(nArea < AREA_D && !m_pStd->m_pWing[nIndex]->m_bIsA) continue;
				else if(nArea == AREA_A && !m_pStd->m_pWing[nIndex]->m_bIsA_Calc) continue;

				CConcBasic *pConc = pBri->GetConcDataWingWall(ixStt==0, ixLeft==iLEFT, nArea);
				if( pConc == NULL ) continue;

				str = GetStringMainRebarUse(pConc, nLine);
//				CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
				CLsdBeam pBeamLsd(pConc, strEnv == "±ØÇÑ" ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN);
				BOOL bValid = (pConc->m_Moment == 0)? FALSE:TRUE;
				szCom = pConc->m_sTitle.Left(6);
				if(szPre != szCom) bBgColr = !bBgColr;
				szPre = szCom;

				long j=0; for(j=0; j<nCntCol; j++)
				{
					long nMerge(0);
					if( bTitle == FALSE )
						nMerge = nRowWing-1;
					else 
						nMerge = 0;

					szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");

					if(bSkipFirstPart && j==0) continue;
					else if(j==0) str = "³¯\n°³\nº®";
					else if(j==1)
					{
						CString strTemp1 = ixStt==0 ? "½ÃÁ¡" : "Á¾Á¡";
						CString strTemp2 = ixLeft==0 ? "ÁÂÃø" : "¿ìÃø";
						pConc->m_sTitle;

						str = pConc->m_sTitle;

					}
					else if (j == 2)
					{
						str = strEnv;
					}
					else if(j==3) str.Format("$f1%.1f", pConc->m_Ho + pConc->m_Hunch/3);
//					else if(j==3) str.Format("$f1%.1f", toCm(pConc->m_RbT1C_dc[0]));
					else if(j==4)
					{
						str.Format("$f1%.1f", pConc->m_RbT1C_dc[0]);		// dc
					}
					else if(j==5)
					{
						if(bValid ) str.Format("$f1%.1f", pBeamLsd.GetAsReq());
						//				else if(bValid && !bSI) str.Format("$f3%.3f", pBeam.GetRebarAreaRequired());
						else str = "-";
					}
					else if(j==6)
					{
						str.Format("$f1%.1f", pBeamLsd.GetAsUse());
					}
					else if(j==7) str = pData->GetStrDouble(pBeamLsd.GetMu(), UNIT_CGS_TONFM);
					else if(j==8) str = pData->GetStrDouble(pBeamLsd.GetMn(), UNIT_CGS_TONFM);
					else if(j==9)
					{
						if(bValid) str.Format("$f3%.3f", pBeamLsd.GetMn() / pBeamLsd.GetMu());
						else str = "-";
					}
					else str = _T("");

					// Ãâ·Â
					if(j > 2 && pBeamLsd.GetMu() <= 0.0) str = _T("-");

					XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, "$j$c%s", str);
					XLText->AddFormatText("$m+0%d", nWidthData[j]);
					bTitle = TRUE;
				}

				XLText->AddText("$n$h");

			}
		}
	}
}

void CARcBridgeCalcSummary::MakeTableCheckCrack(CXLFormatText *XLText)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bSkipFirstPart = (pBri->IsBoxType() || pBri->IsRcSlab()) ? TRUE : FALSE;
	CString str = _T("");
	long nCntItemSlab = pBri->m_pArrConcSlab[eEnvLimit].GetSize();
	// 3°³¾¿¸¸ Ãâ·Â¿¡¼­ ÀüÃ¼ Ãâ·ÂÀ¸·Î º¯°æ(18901)
	long nCntItemWall = (pBri->IsOutsideWall() && (pBri->m_nSelectWallSectionDesign==0 || pBri->m_bOutWallUsebilityCheck))? MIN(10, pBri->m_pArrConcWall[eEnvLimit].GetSize()):0;
	long nCntItemInWall = (pBri->m_nSelectInWallSectionDesign == 0)? pBri->m_pArrConcInWall[eEnvLimit].GetSize() : 0;
	long nCntItemFoot = pBri->m_pArrConcFoot[eEnvLimit].GetSize() + pBri->m_pArrConcFootRev[eEnvLimit].GetSize();
	long nCntItemFootReal = nCntItemFoot;
	long nCntItemArchRib = pBri->m_pArrConcArchRib.GetSize();
	long nCntItemPRF = pBri->m_pArrConcPRF[eEnvLimit].GetSize();
	long nCntItemWing = 0;
	long i=0; for(i=0; i<4; i++)
	{
		CARcBridgeCalcWing *pCalcWing = m_pStd->m_pWing[i];
		if(!pBri->IsWingWall(i==0||i==1, i==0||i==2)) continue;

		if(pCalcWing->m_bIsA) nCntItemWing += 3;
		if(pCalcWing->m_bIsD) nCntItemWing += 1;
		if(pCalcWing->m_bIsA_) nCntItemWing += 1;
	}

	BOOL bRev = FALSE;
	CConcBasic *pConc;
	for(i=0; i<nCntItemFoot; i++)
	{	
		long nInd = i/2;
		if(!bRev) pConc = pBri->m_pArrConcFoot[eEnvLimit].GetAt(nInd);
		else pConc = pBri->m_pArrConcFootRev[eEnvLimit].GetAt(nInd);

		bRev = !bRev;
		if(!bRev && pConc->m_sTitle.Left(8) == "Áß°£ÁöÁ¡") 
		{
			nCntItemFootReal--;
		}
	}

	long nCntRow = nCntItemSlab + nCntItemWall + nCntItemInWall + nCntItemFootReal + nCntItemWing + nCntItemArchRib + nCntItemPRF;

	str.Format("$s+%02d", nCntRow+3);
	XLText->AddFormatText("%s$h$e¡á »ç¿ë¼º °ËÅä$n$h", str);
	XLText->AddTable(nCntRow+1, 25-1, 2, 6);
	
	long nWidthTitle[] = { 6, 4, 4, 4, 4, 3 };
	char *szTitleCGS[] = { "±¸  ºÐ", "Ms(tonf.m)", "fs(kgf/§²)", "±Õ¿­Æø (W)", "Çã¿ë±Õ¿­Æø(Wa)", "ºñ °í" };
	char *szTitleSI[] = { "±¸  ºÐ", "Ms(kN.m)", "fs(MPa)", "±Õ¿­Æø (W)", "Çã¿ë±Õ¿­Æø(Wa)", "ºñ °í" };
	if(pData->m_nTypeKciCode!=0)
	{
		szTitleCGS[3] = szTitleSI[3] = _T("Ã¶±Ù°£°Ý(S)");
		szTitleCGS[4] = szTitleSI[4] = _T("ÃÖ´ë°£°Ý(Sa)");
	}
	for(i=0; i<6; i++)
	{
		if(pData->IsSI()) XLText->AddMergeFormatText(1, nWidthTitle[i]-1, "$c%s", szTitleSI[i]);
		else XLText->AddMergeFormatText(1, nWidthTitle[i]-1, "$c%s", szTitleCGS[i]);
		XLText->AddFormatText("$m+%02d", nWidthTitle[i]);
	}
	XLText->AddFormatText("$n$n$h");

	//////////////////////////////////////////////////////////////////////////
	BOOL bBgColr = FALSE;
	CString szPre = _T("");
	CString szCom = _T("");

	long nWidthData[] = { 1, 5, 4, 4, 4, 4, 3 };
	if(bSkipFirstPart)
	{
		nWidthData[0] = 0;
		nWidthData[1] = 6;
	}

	long nMerge = 0;
	CString szBkColr = _T("");

	// ½½·¡ºê
	for(i=0; i<nCntItemSlab; i++)
	{
		pConc = pBri->m_pArrConcSlab[eEnvLimit].GetAt(i);
		if(pConc == NULL) continue;

		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		long j=0; for(j=0; j<7; j++)
		{
			nMerge = (i==0 && j==0)? nCntItemSlab+nCntItemWall+nCntItemInWall-1:0;
			szBkColr = (pBri->IsBoxType() && j > 1 &&  i < nCntItemSlab / 2.) ? _T("$b040"): _T("");

			if(bSkipFirstPart && j==0) continue;
			else if(j==0) str = "º»\nÃ¼";
			else if(j==1) str = pConc->m_sTitle;
			else if(j==2) str.Format("$f3%.3f", GetValueUnitChange(pConc->m_MomentUse, UNIT_CGS_TONFM, pData->m_szTonfM));
			else if(j==3) str.Format("$f3%.3f", pBeam.GetFs());
			else if(j==4) str.Format("$f3%.3f", pBeam.GetW());
			else if(j==5) str.Format("$f3%.3f", pBeam.GetWa());
			else if(j==6) str = (pBeam.GetWa()>pBeam.GetW())? "O.K":"$#03N.G";
			else str = _T("");

			if(j>2 && (pBeam.GetWa() <= 0 || Compare(pConc->m_MomentUse, 0.0, _T("="), 0.01))) str = _T("-");
			XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
		}
		XLText->AddFormatText("$n$h");
	}
	// º®Ã¼
	for(i=0; i<nCntItemWall; i++)
	{
		pConc = pBri->GetConcDataWall(i, TRUE);
		if(pConc == NULL) continue;

		szCom = pConc->m_sTitle.Left(6);
		if(szPre != szCom) bBgColr = !bBgColr;
		szPre = szCom;

		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		long j=0; for(j=0; j<7; j++)
		{
			nMerge = 0;
			szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");
			
			if(bSkipFirstPart && j==0) continue;
			else if(j==0) str = "º»\nÃ¼";
			else if(j==1)
			{
//				CString strTemp = _T("");
//				strTemp = pConc->m_sTitle;
//				if(strTemp.Right(1) == ")") str = strTemp.Left(13)+"ºÎ";
//				else str = strTemp;
				str = pConc->m_sTitle;
			}
			else if(j==2) str.Format("$f3%.3f", GetValueUnitChange(pConc->m_MomentUse, UNIT_CGS_TONFM, pData->m_szTonfM));
			else if(j==3) str.Format("$f3%.3f", pBeam.GetFs());
			else if(j==4) str.Format("$f3%.3f", pBeam.GetW());
			else if(j==5) str.Format("$f3%.3f", pBeam.GetWa());
			else if(j==6) str = (pBeam.GetWa()>pBeam.GetW())? "O.K":"$#03N.G";
			else str = _T("");
			
			if(j>2 && (pBeam.GetWa() <= 0 || Compare(pConc->m_MomentUse, 0.0, _T("="), 0.01))) str = _T("-");
			XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
		}
		XLText->AddFormatText("$n$h");
	}

	// ³»Ãøº®Ã¼
	for(i=0; i<nCntItemInWall; i++)
	{
		pConc = pBri->m_pArrConcInWall[eEnvLimit].GetAt(i);
		if(pConc == NULL) continue;

		szCom = pConc->m_sTitle.Left(6);
		if(szPre != szCom) bBgColr = !bBgColr;
		szPre = szCom;

		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		long j=0; for(j=0; j<7; j++)
		{
			nMerge = 0;
			szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");
			
			if(bSkipFirstPart && j==0) continue;
			else if(j==0) str = "º»\nÃ¼";
			else if(j==1)
			{
				str = pConc->m_sTitle;
			}
			else if(j==2) str.Format("$f3%.3f", GetValueUnitChange(pConc->m_MomentUse, UNIT_CGS_TONFM, pData->m_szTonfM));
			else if(j==3) str.Format("$f3%.3f", pBeam.GetFs());
			else if(j==4) str.Format("$f3%.3f", pBeam.GetW());
			else if(j==5) str.Format("$f3%.3f", pBeam.GetWa());
			else if(j==6) str = (pBeam.GetWa()>pBeam.GetW())? "O.K":"$#03N.G";
			else str = _T("");
			
			if(j>2 && (pBeam.GetWa() <= 0 || Compare(pConc->m_MomentUse, 0.0, _T("="), 0.01))) str = _T("-");
			XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
		}
		XLText->AddFormatText("$n$h");
	}
	// ¾ÆÄ¡¸®ºê
	for(i=0; i<nCntItemArchRib; i++)
	{
		pConc = pBri->m_pArrConcArchRib.GetAt(i);
		if(pConc == NULL) continue;
		
		if(i==0) bBgColr = !bBgColr;
		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		long j=0; for(j=0; j<7; j++)
		{
			nMerge = (i==0 && j==0)? nCntItemArchRib-1:0;
			
			if(bSkipFirstPart && j==0) continue;
			else if(j==0) str = "¾Æ\nÄ¡\n¸®\nºê";
			else if(j==1) str = pConc->m_sTitle;
			else if(j==2) str.Format("$f3%.3f", GetValueUnitChange(pConc->m_MomentUse, UNIT_CGS_TONFM, pData->m_szTonfM));
			else if(j==3) str.Format("$f3%.3f", pBeam.GetFs());
			else if(j==4) str.Format("$f3%.3f", pBeam.GetW());
			else if(j==5) str.Format("$f3%.3f", pBeam.GetWa());
			else if(j==6) str = (pBeam.GetWa()>pBeam.GetW())? "O.K":"$#03N.G";
			else str = _T("");
			
			if(j>2 && (pBeam.GetWa() <= 0 || Compare(pConc->m_MomentUse, 0.0, _T("="), 0.01))) str = _T("-");
			szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");
			XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
		}
		XLText->AddFormatText("$n$h");
	}
	//ºÎ»ó¹æÁöÀúÆÇ
	for(i=0; i<nCntItemPRF; i++)
	{
		pConc = pBri->GetConcDataPRF(i==0);
		if(pConc == NULL) continue;

		if(i==0)
			bBgColr = !bBgColr;

		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		long j=0; for(j=0; j<7; j++)
		{
			nMerge = 0;
			szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");

			if(bSkipFirstPart && j==0) continue;
			else if(j==0) str = "º»\nÃ¼";
			else if(j==1) str = pConc->m_sTitle;
			else if(j==2) str.Format("$f3%.3f", GetValueUnitChange(pConc->m_MomentUse, UNIT_CGS_TONFM, pData->m_szTonfM));
			else if(j==3) str.Format("$f3%.3f", pBeam.GetFs());
			else if(j==4) str.Format("$f3%.3f", pBeam.GetW());
			else if(j==5) str.Format("$f3%.3f", pBeam.GetWa());
			else if(j==6) str = (pBeam.GetWa()>pBeam.GetW())? "O.K":"$#03N.G";
			else str = _T("");

			if(j>2 && (pBeam.GetWa() <= 0 || Compare(pConc->m_MomentUse, 0.0, _T("="), 0.01))) str = _T("-");
			XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
		}
		XLText->AddFormatText("$n$h");
	}

	// ±âÃÊ
	bRev = FALSE;
	for(i=0; i<nCntItemFoot; i++)
	{	
		long nInd = i/2;
		if(!bRev) pConc = pBri->m_pArrConcFoot[eEnvLimit].GetAt(nInd);
		else pConc = pBri->m_pArrConcFootRev[eEnvLimit].GetAt(nInd);
		
		BOOL bUpper = (pConc->m_sTitle.Right(6) == _T("(µÞ±Á)"))? !bRev : bRev;
		CString strSub = (!bUpper)? _T("-ÇÏ¸é") : _T("-»ó¸é");
		
		bRev = !bRev;
		if(!bRev && pConc->m_sTitle.Left(8) == "Áß°£ÁöÁ¡") 
		{
			continue;
		}

		szCom = pConc->m_sTitle.Left(6);
		if(szPre != szCom) bBgColr = !bBgColr;
		szPre = szCom;

		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper, pBri->m_bCheckRebarRateFooting);
		long j=0; for(j=0; j<7; j++)
		{
			nMerge = (i==0 && j==0)? nCntItemFootReal-1:0;
			szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");
			
			if(bSkipFirstPart && j==0) continue;
			else if(j==0) str = "±â\nÃÊ";
			else if(j==1) str = pConc->m_sTitle+strSub;
			else if(j==2) str.Format("$f3%.3f", GetValueUnitChange(pConc->m_MomentUse, UNIT_CGS_TONFM, pData->m_szTonfM));
			else if(j==3) str.Format("$f3%.3f", pBeam.GetFs());
			else if(j==4) str.Format("$f3%.3f", pBeam.GetW());
			else if(j==5) str.Format("$f3%.3f", pBeam.GetWa());
			else if(j==6) str = (pBeam.GetWa()>pBeam.GetW())? "O.K":"$#03N.G";
			else str = _T("");
			
			if(j>2 && (pBeam.GetWa() <= 0 || Compare(pConc->m_MomentUse, 0.0, _T("="), 0.01))) str = _T("-");
			XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
		}
		XLText->AddFormatText("$n$h");
	}
	// ³¯°³º®
	BOOL bFirst = TRUE;
	long k=0; for(k=0; k<4; k++)
	{
		if(!pBri->IsWingWall(k==0||k==1, k==0||k==2)) continue;
		CARcBridgeCalcWing *pCalcWing = m_pStd->m_pWing[k];
		
		for(i=0; i<5; i++)
		{
			if(k==0) pConc = pBri->m_pArrConcWingSttLeft.GetAt(i);
			else if(k==1) pConc = pBri->m_pArrConcWingSttRight.GetAt(i);
			else if(k==2) pConc = pBri->m_pArrConcWingEndLeft.GetAt(i);
			else if(k==3) pConc = pBri->m_pArrConcWingEndRight.GetAt(i);

			if(pConc == NULL) continue;
			if(pCalcWing->m_bIsA == FALSE && i < 3) continue;
			if(pCalcWing->m_bIsD == FALSE && i == 3) continue;
			if(pCalcWing->m_bIsA_ == FALSE && i == 4) continue;

			szCom = pConc->m_sTitle.Left(6);
			if(szPre != szCom) bBgColr = !bBgColr;
			szPre = szCom;

			CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
			long j=0; for(j=0; j<7; j++)
			{
				nMerge = (bFirst) ? nCntItemWing-1 : 0;

				szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");
				bFirst = FALSE;

				if(bSkipFirstPart && j==0) continue;
				else if(j==0) str = "³¯\n°³\nº®";
				else if(j==1) str = pConc->m_sTitle;
				else if(pConc->m_MomentUse == 0) str = _T("-");
				else if(j==2) str.Format("$f3%.3f", GetValueUnitChange(pConc->m_MomentUse, UNIT_CGS_TONFM, pData->m_szTonfM));
				else if(j==3) str.Format("$f3%.3f", pBeam.GetFs());
				else if(j==4) str.Format("$f3%.3f", pBeam.GetW());
				else if(j==5) str.Format("$f3%.3f", pBeam.GetWa());
				else if(j==6) str = (pBeam.GetWa()>pBeam.GetW())? "O.K":"$#03N.G";
				else str = _T("");
				
				if(j>2 && (pBeam.GetWa() <= 0 || Compare(pConc->m_MomentUse, 0.0, _T("="), 0.01))) str = _T("-");
				XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e %s%s" : "$c%s%s", str, szBkColr); 
				XLText->AddFormatText("$m+%02d", nWidthData[j]);
			}
			XLText->AddFormatText("$n$h");
		}
	}
	*XLText += 1;
}

void CARcBridgeCalcSummary::MakeTableCheckCrack_Lsd(CXLFormatText *XLText)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bSkipFirstPart = (pBri->IsBoxType() || pBri->IsRcSlab()) ? TRUE : FALSE;
	BOOL bCheckCrack = pBri->m_nSelectUsebilityCheck == 1 ? TRUE : FALSE;				// »ç¿ë¼º °ËÅä½Ã ±Õ¿­°ËÅä
	
	const HGINT32 icValidEnvType = pBri->GetCountValidComboEnvType();

	CString str = _T("");
	long nCntItemSlab = pBri->m_pArrConcSlab[eEnvLimit].GetSize();
	long nCntItemWall = (pBri->IsOutsideWall() && (pBri->m_nSelectWallSectionDesign==0))? MIN(10, pBri->GetCountConcOutWall()):0;
	// (ARCBRIDGE-3618) LSD¿¡¼­ ÈÚ¾ÐÃàºÎÀç·Î °ËÅä½Ã »ç¿ë¼º°ËÅä´Â Baem°ËÅä µ¥ÀÌÅÍ¸¦ ¾²Áö¾Ê°í ±âµÕ°ËÅä µ¥ÀÌÅÍ¸¦ ½á´Þ¶ó´Â ¿äÃ»À¸·Î µû·Î Ãâ·ÂÇÑ´Ù.
	if(pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign==1 && pBri->m_bOutWallUsebilityCheck)
		nCntItemWall = 2;

	long nCntItemInWall = (pBri->m_nSelectInWallSectionDesign == 0)? pBri->m_pArrConcInWall[eEnvService].GetSize() : 0; //pBri->GetCountConcInWall() : 0;
	long nCntItemFoot = pBri->GetCountFooting();
	long nCntItemFootReal = nCntItemFoot;
	long nCntItemArchRib = pBri->m_pArrConcArchRib.GetSize();
	long nCntItemPRF = pBri->m_pArrConcPRF[eEnvLimit].GetSize();

	long nCntItemWing = 0;
	long i=0; for(i=0; i<4; i++)
	{
		CARcBridgeCalcWing *pCalcWing = m_pStd->m_pWing[i];
		if(!pBri->IsWingWall(i==0||i==1, i==0||i==2)) continue;

		if(pCalcWing->m_bIsA) nCntItemWing += 3;
		if(pCalcWing->m_bIsD) nCntItemWing += 1;
		if(pCalcWing->m_bIsA_) nCntItemWing += 1;
	}

	long nCntItemRow = 1;
	long nRowFooting(0);
	CConcBasic *pConc;

	long nCntJijum = pBri->GetCountJijum();
	for(i=0; i<nCntJijum; i++)
	{	
		if( pBri->IsFootingJijum(i) == FALSE ) continue;

		for (long ixFront=0; ixFront < 2; ++ixFront)
		{
			for (long ixUpper=1; ixUpper >= 0; --ixUpper)
			{
				CConcBasic *pConc = pBri->GetConcDataFooting(i, ixFront==0, ixUpper==0);
				if( pConc == NULL ) continue;

				if(ixFront==0 && pConc->m_sTitle.Left(8) == "Áß°£ÁöÁ¡") 
				{
					nCntItemFootReal--;
					continue;
				}

				nRowFooting += nCntItemRow;
			}
		}
	}

	// ¾Õ, µÚ, »ó, ÇÏ
	long nCntRow = nCntItemSlab + nCntItemWall + nCntItemInWall + nRowFooting + nCntItemWing + nCntItemArchRib + nCntItemPRF;

	str.Format("$s+%02d", nCntRow+3);
	XLText->AddFormatText("%s$h$e¡á »ç¿ë¼º °ËÅä$n$h", str);
	XLText->AddTable(nCntRow+1, 25-1, 2, 6);

	long nWidthTitle[] = { 6, 3, 8, 8 };
	long nWidthSubTitle[] = { 6, 3, 3, 3, 2, 3, 3, 2 };
	char *szTitle[] = { "±¸  ºÐ", "±Õ¿­", "ÃÖ¼ÒÃ¶±Ù·®", "°£Á¢±Õ¿­°ËÅä" };
	char *szTitleSub[] = { "±¸  ºÐ", "±Õ¿­", "As,min", "As,use", "ºñ °í", "fs,max", "fs", "ºñ °í" };

	if( bCheckCrack == FALSE )
	{
		szTitle[3] = "±Õ¿­Æø°ËÅä";
		szTitleSub[5] = "Wk";
		szTitleSub[6] = "Wa";
	}

	for(i=0; i<4; i++)
	{
		XLText->AddMergeFormatText((i==0 || i==1) ? 1 : 0, nWidthTitle[i]-1, "$c%s", szTitle[i]);
		XLText->AddFormatText("$m+%02d", nWidthTitle[i]);
	}
	XLText->AddFormatText("$n$h");

	for(i=0; i<8; i++)
	{
		if( 1 < i )
			XLText->AddMergeFormatText(0, nWidthSubTitle[i]-1, "$c%s", szTitleSub[i]);
		XLText->AddFormatText("$m+%02d", nWidthSubTitle[i]);
	}
	XLText->AddFormatText("$n$h");

	//////////////////////////////////////////////////////////////////////////
	BOOL bBgColr = FALSE;
	CString szPre = _T("");
	CString szCom = _T("");

	long nWidthData[] = { 1, 5, 3, 3, 3, 2, 3, 3, 2 };
 	if(bSkipFirstPart)
 	{
 		nWidthData[0] = 0;
 		nWidthData[1] = 6;
 	}

	long nMerge = 0;
	CString szBkColr = _T("");

	// ½½·¡ºê
	for(i=0; i<nCntItemSlab; i++)
	{
		pConc = pBri->m_pArrConcSlab[eEnvService].GetAt(i);
		if(pConc == NULL) continue;

		CLsdBeam pBeamLsd(pConc, LSD_ULS_DESIGN);

		const HGBOOL bCrack = pBeamLsd.IsCrackSection();
		BOOL bCheckCrackAsMin = pBeamLsd.IsOkCrackAsMin();
		BOOL bCheckCrackCtc = pBeamLsd.IsOkCrackCtc();

		long j=0; for(j=0; j<9; j++)
		{
			nMerge = (i==0 && j==0)? nCntItemSlab+nCntItemWall+nCntItemInWall-1:0;
			szBkColr = (pBri->IsBoxType() && j > 1 &&  i < nCntItemSlab / 2.) ? _T("$b040"): _T("");

			if(bSkipFirstPart && j==0) continue;
			else if(j==0) str = "º»\nÃ¼";
			else if(j==1) str = pConc->m_sTitle;
			else if(j==2) str = bCrack ? "±Õ¿­" : "ºñ±Õ¿­";
			else if(j==3) str.Format("$f1%.3f", pBeamLsd.GetAsMinCrack());
			else if(j==4) str.Format("$f1%.3f", pBeamLsd.GetAsUse());
			else if(j==5) str = (bCheckCrackAsMin)? "O.K":"$#03N.G";
			else if(j==6) str.Format("$f1%.3f", bCheckCrack ? pBeamLsd.GetMaxCrackMpa() : pBeamLsd.GetWk());
			else if(j==7) str.Format("$f1%.3f", bCheckCrack ? pBeamLsd.GetFsCrack() : pBeamLsd.GetWa());
			else if(j==8) str = (bCheckCrackCtc)? "O.K":"$#03N.G";
			else str = _T("");

			if(j>2 && bCrack == FALSE ) str = _T("-");

			XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
			XLText->OutXL();
		}
		XLText->AddFormatText("$n$h");
	}
	// º®Ã¼
	for(i=0; i<nCntItemWall; i++)
	{
		//pConc = pBri->GetConcDataWall_Lsd(i);
		if(pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign==1 && pBri->m_bOutWallUsebilityCheck)
		{
			pConc = m_pStd->m_pColGir->GetConcWallForOutWallUsebilityCheck(i==0);
		}
		else
		{
			if(pBri->m_pArrConcWall[eEnvService].GetSize() > i)
				pConc = pBri->m_pArrConcWall[eEnvService].GetAt(i);
			else
				pConc = NULL;
		}
		if(pConc == NULL) continue;

		szCom = pConc->m_sTitle.Left(6);
		if(szPre != szCom) bBgColr = !bBgColr;
		szPre = szCom;

		CLsdBeam pBeamLsd(pConc, LSD_ULS_DESIGN);

		const HGBOOL bCrack = pBeamLsd.IsCrackSection();
		BOOL bCheckCrackAsMin = pBeamLsd.IsOkCrackAsMin();
		BOOL bCheckCrackCtc = pBeamLsd.IsOkCrackCtc();

		long j=0; for(j=0; j<9; j++)
		{
			nMerge = 0;
			szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");

			if(bSkipFirstPart && j==0) continue;
			else if(j==0) str = "º»\nÃ¼";
			else if(j==1) str = pConc->m_sTitle;
			else if(j==2) str = bCrack ? "±Õ¿­" : "ºñ±Õ¿­";
			else if(j==3) str.Format("$f1%.3f", pBeamLsd.GetAsMinCrack());
			else if(j==4) str.Format("$f1%.3f", pBeamLsd.GetAsUse());
			else if(j==5) str = (bCheckCrackAsMin)? "O.K":"$#03N.G";
			else if(j==6) str.Format("$f1%.3f", bCheckCrack ? pBeamLsd.GetMaxCrackMpa() : pBeamLsd.GetWk());
			else if(j==7) str.Format("$f1%.3f", bCheckCrack ? pBeamLsd.GetFsCrack() : pBeamLsd.GetWa());
			else if(j==8) str = (bCheckCrackCtc)? "O.K":"$#03N.G";
			else str = _T("");

//			if(j>1 && pBeam.GetWa() <= 0) str = _T("-");

			if(j>2 && bCrack == FALSE ) str = _T("-");
			XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
			XLText->OutXL();
		}
		XLText->AddFormatText("$n$h");
	}

	// ³»Ãøº®Ã¼
	for(i=0; i<nCntItemInWall; i++)
	{
		//pConc = pBri->GetConcDataInWall_Lsd(i);
		if(pBri->m_pArrConcInWall[eEnvService].GetSize() > i)
			pConc = pBri->m_pArrConcInWall[eEnvService].GetAt(i);
		else
			pConc = NULL;
		if(pConc == NULL) continue;

		szCom = pConc->m_sTitle.Left(6);
		if(szPre != szCom) bBgColr = !bBgColr;
		szPre = szCom;

		CLsdBeam pBeamLsd(pConc, LSD_ULS_DESIGN);

		const HGBOOL bCrack = pBeamLsd.IsCrackSection();
		BOOL bCheckCrackAsMin = pBeamLsd.IsOkCrackAsMin();
		BOOL bCheckCrackCtc = pBeamLsd.IsOkCrackCtc();

		long j=0; for(j=0; j<9; j++)
		{
			nMerge = 0;
			szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");

			if(bSkipFirstPart && j==0) continue;
			else if(j==0) str = "º»\nÃ¼";
			else if(j==1) str = pConc->m_sTitle;
			else if(j==2) str = bCrack ? "±Õ¿­" : "ºñ±Õ¿­";
			else if(j==3) str.Format("$f1%.3f", pBeamLsd.GetAsMinCrack());
			else if(j==4) str.Format("$f1%.3f", pBeamLsd.GetAsUse());
			else if(j==5) str = (bCheckCrackAsMin)? "O.K":"$#03N.G";
			else if(j==6) str.Format("$f1%.3f", bCheckCrack ? pBeamLsd.GetMaxCrackMpa() : pBeamLsd.GetWk());
			else if(j==7) str.Format("$f1%.3f", bCheckCrack ? pBeamLsd.GetFsCrack() : pBeamLsd.GetWa());
			else if(j==8) str = (bCheckCrackCtc)? "O.K":"$#03N.G";
			else str = _T("");

			if(j>2 && bCrack == FALSE) str = _T("-");
			XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
			XLText->OutXL();
		}
		XLText->AddFormatText("$n$h");
	}
	// ¾ÆÄ¡¸®ºê
	for(i=0; i<nCntItemArchRib; i++)
	{
		pConc = pBri->m_pArrConcArchRib.GetAt(i);
		if(pConc == NULL) continue;

		if(i==0) bBgColr = !bBgColr;
		CLsdBeam pBeamLsd(pConc, LSD_ULS_DESIGN);

		const HGBOOL bCrack = pBeamLsd.IsCrackSection();
		BOOL bCheckCrackAsMin = pBeamLsd.IsOkCrackAsMin();
		BOOL bCheckCrackCtc = pBeamLsd.IsOkCrackCtc();

		long j=0; for(j=0; j<9; j++)
		{
			nMerge = (i==0 && j==0)? nCntItemArchRib-1:0;

			if(bSkipFirstPart && j==0) continue;
			else if(j==0) str = "¾Æ\nÄ¡\n¸®\nºê";
			else if(j==1) str = pConc->m_sTitle;
			else if(j==2) str = bCrack ? "±Õ¿­" : "ºñ±Õ¿­";
			else if(j==3) str.Format("$f1%.3f", pBeamLsd.GetAsMinCrack());
			else if(j==4) str.Format("$f1%.3f", pBeamLsd.GetAsUse());
			else if(j==5) str = (bCheckCrackAsMin)? "O.K":"$#03N.G";
			else if(j==6) str.Format("$f1%.3f", bCheckCrack ? pBeamLsd.GetMaxCrackMpa() : pBeamLsd.GetWk());
			else if(j==7) str.Format("$f1%.3f", bCheckCrack ? pBeamLsd.GetFsCrack() : pBeamLsd.GetWa());
			else if(j==8) str = (bCheckCrackCtc)? "O.K":"$#03N.G";
			else str = _T("");

//			if(j>1 && pBeam.GetWa() <= 0) str = _T("-");
			if(j>1 && bCrack == FALSE) str = _T("-");
			szBkColr = (j > 2 && bBgColr)? _T("$b040"): _T("");
			XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
		}
		XLText->AddFormatText("$n$h");
	}

	//ºÎ»ó¹æÁöÀúÆÇ
	for(i=0; i<nCntItemPRF; i++)
	{
		if(pBri->m_pArrConcPRF[eEnvService].GetSize() > i)
			pConc = pBri->m_pArrConcPRF[eEnvService].GetAt(i);
		else
			pConc = NULL;
		if(pConc == NULL) continue;

		if(i == 0)
			bBgColr = !bBgColr;
		
		CLsdBeam pBeamLsd(pConc, LSD_ULS_DESIGN);

		const HGBOOL bCrack = pBeamLsd.IsCrackSection();
		BOOL bCheckCrackAsMin = pBeamLsd.IsOkCrackAsMin();
		BOOL bCheckCrackCtc = pBeamLsd.IsOkCrackCtc();

		long j=0; for(j=0; j<9; j++)
		{
			nMerge = 0;
			szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");

			if(bSkipFirstPart && j==0) continue;
			else if(j==0) str = "º»\nÃ¼";
			else if(j==1) str = pConc->m_sTitle;
			else if(j==2) str = bCrack ? "±Õ¿­" : "ºñ±Õ¿­";
			else if(j==3) str.Format("$f1%.3f", pBeamLsd.GetAsMinCrack());
			else if(j==4) str.Format("$f1%.3f", pBeamLsd.GetAsUse());
			else if(j==5) str = (bCheckCrackAsMin)? "O.K":"$#03N.G";
			else if(j==6) str.Format("$f1%.3f", bCheckCrack ? pBeamLsd.GetMaxCrackMpa() : pBeamLsd.GetWk());
			else if(j==7) str.Format("$f1%.3f", bCheckCrack ? pBeamLsd.GetFsCrack() : pBeamLsd.GetWa());
			else if(j==8) str = (bCheckCrackCtc)? "O.K":"$#03N.G";
			else str = _T("");

			if(j>2 && bCrack == FALSE ) str = _T("-");
			XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
			XLText->OutXL();
		}
		XLText->AddFormatText("$n$h");
	}
	// ±âÃÊ
//	bRev = FALSE;
	for(i=0; i<nCntJijum; i++)
	{	
		if( pBri->IsFootingJijum(i) == FALSE )
			continue;

		for (long ixFront=0; ixFront < 2; ++ixFront)
		{
			for (long ixUpper=1; ixUpper >= 0; --ixUpper)
			{
				pConc = pBri->GetConcDataFooting(i, ixFront==0, ixUpper==0, -1, eEnvService);		// ÇÏ¸éºÎÅÍ Ã¼Å©
				if( pConc == NULL ) continue;
				
				BOOL bUpper = ixUpper==0;
				CString strSub = (!bUpper)? _T("-ÇÏ¸é") : _T("-»ó¸é");

				if(ixFront==0 && pConc->m_sTitle.Left(8) == "Áß°£ÁöÁ¡") 
				{
					continue;
				}

				szCom = pConc->m_sTitle.Left(6);
				if(szPre != szCom) bBgColr = !bBgColr;
				szPre = szCom;

				CLsdBeam pBeamLsd(pConc, LSD_ULS_DESIGN);

				const HGBOOL bCrack = pBeamLsd.IsCrackSection();
				BOOL bCheckCrackAsMin = pBeamLsd.IsOkCrackAsMin();
				BOOL bCheckCrackCtc = pBeamLsd.IsOkCrackCtc();

				long j=0; for(j=0; j<9; j++)
				{
					nMerge = (i==0 && j==0 && ixFront==0 && ixUpper==1)? nRowFooting-1 : 0;
					szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");

					if(bSkipFirstPart && j==0) continue;
					else if(j==0) str = "±â\nÃÊ";
					else if(j==1) str = pConc->m_sTitle+strSub;
					else if(j==2) str = bCrack ? "±Õ¿­" : "ºñ±Õ¿­";
					else if(j==3) str.Format("$f1%.3f", pBeamLsd.GetAsMinCrack());
					else if(j==4) str.Format("$f1%.3f", pBeamLsd.GetAsUse());
					else if(j==5) str = (bCheckCrackAsMin)? "O.K":"$#03N.G";
					else if(j==6) str.Format("$f1%.3f", bCheckCrack ? pBeamLsd.GetMaxCrackMpa() : pBeamLsd.GetWk());
					else if(j==7) str.Format("$f1%.3f", bCheckCrack ? pBeamLsd.GetFsCrack() : pBeamLsd.GetWa());
					else if(j==8) str = (bCheckCrackCtc)? "O.K":"$#03N.G";
					else str = _T("");

					if(j>2 && bCrack == FALSE) str = _T("-");
					XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
					XLText->AddFormatText("$m+%02d", nWidthData[j]);
				}
				XLText->AddFormatText("$n$h");

			}
		}
	}
	XLText->OutXL();
	// ³¯°³º®
	BOOL bFirst = TRUE;
	long k=0; for(k=0; k<4; k++)
	{
		if(!pBri->IsWingWall(k==0||k==1, k==0||k==2)) continue;
		CARcBridgeCalcWing *pCalcWing = m_pStd->m_pWing[k];
		
		for(i=0; i<5; i++)
		{
			if(k==0) pConc = pBri->m_pArrConcWingSttLeft.GetAt(i);
			else if(k==1) pConc = pBri->m_pArrConcWingSttRight.GetAt(i);
			else if(k==2) pConc = pBri->m_pArrConcWingEndLeft.GetAt(i);
			else if(k==3) pConc = pBri->m_pArrConcWingEndRight.GetAt(i);

			if(pConc == NULL) continue;
			if(pCalcWing->m_bIsA == FALSE && i < 3) continue;
			if(pCalcWing->m_bIsD == FALSE && i == 3) continue;
			if(pCalcWing->m_bIsA_ == FALSE && i == 4) continue;

			szCom = pConc->m_sTitle.Left(6);
			if(szPre != szCom) bBgColr = !bBgColr;
			szPre = szCom;

			CLsdBeam pBeamLsd(pConc, LSD_ULS_DESIGN );

			const HGBOOL bCrack = pBeamLsd.IsCrackSection();
			BOOL bCheckCrackAsMin = pBeamLsd.IsOkCrackAsMin();
			BOOL bCheckCrackCtc = pBeamLsd.IsOkCrackCtc();

			BOOL bNoneCheck(FALSE);
			BOOL bStt = k==0 || k==1;
			BOOL bUseWingCrack = pBri->IsUseSelectLoadCaseWingOrApsMatchDesignRating(pConc->m_strDesignDegree , bStt? ePartWingWallStt : ePartWingWallEnd);
			if(pConc->m_Moment == 0 || pBri->IsCheckPosEtaUseLoad(bStt? ePartWingWallStt : ePartWingWallEnd) == FALSE || bUseWingCrack == FALSE)
			{
				bNoneCheck = TRUE;
			}

			long j=0; for(j=0; j<9; j++)
			{
				nMerge = (bFirst) ? nCntItemWing-1 : 0;

				szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");
				bFirst = FALSE;

				if(bSkipFirstPart && j==0) continue;
				else if(j==0) str = "³¯\n°³\nº®";
				else if(j==1) str = pConc->m_sTitle;
				else if(bNoneCheck == TRUE) str = _T("-");
				else if(j==2) str = bCrack ? "±Õ¿­" : "ºñ±Õ¿­";
				else if(j==3) str.Format("$f1%.3f", pBeamLsd.GetAsMinCrack());
				else if(j==4) str.Format("$f1%.3f", pBeamLsd.GetAsUse());
				else if(j==5) str = (bCheckCrackAsMin)? "O.K":"$#03N.G";
				else if(j==6) str.Format("$f1%.3f", bCheckCrack ? pBeamLsd.GetMaxCrackMpa() : pBeamLsd.GetWk());
				else if(j==7) str.Format("$f1%.3f", bCheckCrack ? pBeamLsd.GetFsCrack() : pBeamLsd.GetWa());
				else if(j==8) str = (bCheckCrackCtc)? "O.K":"$#03N.G";
				else str = _T("");

//				if(j>1 && pBeam.GetWa() <= 0) str = _T("-");
				if(j>2 && bCrack == FALSE) str = _T("-");
				XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e %s%s" : "$c%s%s", str, szBkColr); 
				XLText->AddFormatText("$m+%02d", nWidthData[j]);
			}
			XLText->AddFormatText("$n$h");
		}
	}
	*XLText += 1;
}

void CARcBridgeCalcSummary::MakeTableCheckSupport(CXLFormatText *XLText)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nCntItemInWall = 0;
	long nSizeList = m_pStd->m_pColGir->m_pList.GetSize();
	long i=0; for(i=0; i<nSizeList; i++)
	{
		CGirderAndColumn *pGirCol = m_pStd->m_pColGir->m_pList.GetAt(i);
		if(pGirCol->m_nJijum == 0 || pGirCol->m_nJijum == nSizeList - 1) continue;	// #ARCBRIDGE-3568 ½ÃÁ¡°ú Á¾Á¡ÀÇ ¼öÆòÃ¶±ÙÀº µû·Î °è»êµÇ¹Ç·Î
		if(pGirCol->m_bWall) nCntItemInWall++;
	}
	
	BOOL bSkipFirstPart = (pBri->IsBoxType() || pBri->IsRcSlab()) ? TRUE : FALSE;
	CString str = _T("");
	long nCntItemSlab = pBri->m_pArrConcSlab[eEnvLimit].GetSize();
	long nCntItemWall = (pBri->IsOutsideWall())? MIN(6, pBri->m_pArrConcWall[eEnvLimit].GetSize()):0;
	if(pBri->m_nSelectInWallSectionDesign == 0)
	{
		nCntItemInWall = pBri->m_pArrConcInWall[eEnvLimit].GetSize()/2;
	}
	long nCntItemFoot = pBri->m_pArrConcFoot[eEnvLimit].GetSize();
	long nCntItemArchRib = pBri->m_pArrConcArchRib.GetSize();
	long nCntItemPRF = pBri->m_pArrConcPRF[eEnvLimit].GetSize();
	long nCntItemWing = 0;
	for(i=0; i<4; i++)
	{
		if(!pBri->IsWingWall(i==0||i==1, i==0||i==2)) continue;

		CARcBridgeCalcWing *pCalcWing = m_pStd->m_pWing[i];
		if(pCalcWing->m_bIsA) nCntItemWing += 3;
		if(pCalcWing->m_bIsD) nCntItemWing += 1;
		if(pCalcWing->m_bIsA_) nCntItemWing += 1;
	}

	long nCntRow = nCntItemSlab+nCntItemWall+nCntItemInWall+nCntItemFoot+nCntItemWing+nCntItemArchRib+nCntItemPRF;

	BOOL bBgColr = FALSE;
	CString szPre = _T("");
	CString szCom = _T("");
	CString szBkColr = _T("");

	str.Format("$s+%02d", nCntRow+3);
// 	CString szTitle = _T("");
	CString szTitle = (pData->m_nTypeUnit == UNIT_TYPE_SI)? "§±":"§²";
	XLText->AddFormatText("%s$h$e¡á ¼öÆòÃ¶±Ù °ËÅä$m+19$i$me1´ÜÀ§$m+02$c:$r$e %s $n$h", str, szTitle);
	XLText->AddTable(nCntRow+1, 25-1, 2, 5);
	
	long nWidthTitle1[] = { 6, 4, 4, 4, 4, 3 };
	long nWidthTitle2[] = { 6, 2, 2, 2, 2, 2, 2, 2, 2, 3 };
	char *szTitle1[] = { "±¸  ºÐ", "ÁÖÃ¶±Ù/0.0015hb", "ÁÖÃ¶±Ù/20%", "´Ü¸éÀûºñ", "300§±ÀÌ»ó", "ºñ °í" };
	char *szTitle2[] = { "±¸  ºÐ", "ÇÊ¿ä", "»ç¿ë", "ÇÊ¿ä", "»ç¿ë", "ÇÊ¿ä", "»ç¿ë", "ÇÊ¿ä", "»ç¿ë", "ºñ °í" };
	for(i=0; i<6; i++)
	{
		XLText->AddMergeFormatText((i==0 || i==5)? 1:0, nWidthTitle1[i]-1, "$c%s", szTitle1[i]);
		XLText->AddFormatText("$m+%02d", nWidthTitle1[i]);
	}
	XLText->AddFormatText("$n$h");
	for(i=0; i<10; i++)
	{
		XLText->AddMergeFormatText(0, nWidthTitle2[i]-1, "$c%s", szTitle2[i]);
		XLText->AddFormatText("$m+%02d", nWidthTitle2[i]);
	}
	XLText->AddFormatText("$n$h");

	//////////////////////////////////////////////////////////////////////////
	CARcBridgeCalcCornerAndSupport *pSupport = m_pStd->m_pCorner[iUPPER];
	CConcBasic *pConc;
	long nWidthData[] = { 1, 5, 2, 2, 2, 2, 2, 2, 2, 2, 3 };
	if(bSkipFirstPart)
	{
		nWidthData[0] = 0;
		nWidthData[1] = 6;
	}

	long nMerge = 0;

	// ½½·¡ºê
	for(i=0; i<nCntItemSlab; i++)
	{
		double dLenSpan = 0;
		
		if(i < 2) dLenSpan = pBri->GetLengthModelSpan(0);
		else if(i > nCntRow-5) dLenSpan = pBri->GetLengthModelSpan(pBri->GetCountJijum()-2);
		else if(i%2 != 0) dLenSpan = pBri->GetLengthModelSpan((i-1)/2);
		else if(i%2 == 0) dLenSpan = MAX(pBri->GetLengthModelSpan(i/2 - 1), pBri->GetLengthModelSpan(i/2));
		
		pConc = pBri->m_pArrConcSlab[eEnvLimit].GetAt(i);
		if(pConc == NULL) continue;

		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		long j=0; for(j=0; j<11; j++)
		{
			nMerge = (i==0 && j==0)? nCntItemSlab+nCntItemWall+nCntItemInWall-1:0;
			szBkColr = (pBri->IsBoxType() && j > 1 &&  i < nCntItemSlab / 2.) ? _T("$b040"): _T("");

			// ÇÏºÎ½½·¡ºê´Â ±âÃÊ°ËÅäÁ¶°Ç¿¡ ¸ÂÃç¼­ Ãâ·ÂÇÑ´Ù.
			if(pBri->IsBoxType() && i >= nCntItemSlab / 2.)
			{
				if(bSkipFirstPart && j==0) continue;
				else if(j==0) str = "º»\nÃ¼";
				else if(j==1) str = pConc->m_sTitle;
				else if(j==2) str = (pBri->m_bCheckSuppoerBarFoot15hs) ? pSupport->GetStringCheckConcFooting(pConc, TRUE, 0):_T("");
				else if(j==3) str = (pBri->m_bCheckSuppoerBarFoot15hs)? pSupport->GetStringCheckConcFooting(pConc, FALSE, 0):_T("");
				else if(j==4) str = (pBri->m_bCheckSuppoerBarFootMainOver20)? pSupport->GetStringCheckConcFooting(pConc, TRUE, 1):_T("");
				else if(j==5) str = (pBri->m_bCheckSuppoerBarFootMainOver20)? pSupport->GetStringCheckConcFooting(pConc, FALSE, 1):_T("");
				else if(j==6) str = (pBri->m_bCheckSuppoerBarFootArea)? pSupport->GetStringCheckConcFooting(pConc, TRUE, 2):_T("");
				else if(j==7) str = (pBri->m_bCheckSuppoerBarFootArea)? pSupport->GetStringCheckConcFooting(pConc, FALSE, 2):_T("");
				else if(j==8) str = (pBri->m_bCheckSuppoerBarFoot300)? pSupport->GetStringCheckConcFooting(pConc, TRUE, 3):_T("");
				else if(j==9) str = (pBri->m_bCheckSuppoerBarFoot300)? pSupport->GetStringCheckConcFooting(pConc, FALSE, 3):_T("");
				else if(j==10) str = pSupport->GetStringCheckConcFooting(pConc, TRUE, 4);
				else str = _T("");
			}
			else
			{
				if(bSkipFirstPart && j==0) continue;
				else if(j==0) str = "º»\nÃ¼";
				else if(j==1) str = pConc->m_sTitle;
				else if(j==2) str = (pBri->m_bCheckSuppoerBarSlabMain)? pSupport->GetStringCheckConcSlab(pConc, TRUE, 0, dLenSpan):_T("");
				else if(j==3) str = (pBri->m_bCheckSuppoerBarSlabMain)? pSupport->GetStringCheckConcSlab(pConc, FALSE, 0, dLenSpan):_T("");
				else if(j==4) str = (pBri->m_bCheckSuppoerBarSlabMainOver20)? pSupport->GetStringCheckConcSlab(pConc, TRUE, 1, dLenSpan):_T("");
				else if(j==5) str = (pBri->m_bCheckSuppoerBarSlabMainOver20)? pSupport->GetStringCheckConcSlab(pConc, FALSE, 1, dLenSpan):_T("");
				else if(j==6) str = (pBri->m_bCheckSuppoerBarSlabArea)? pSupport->GetStringCheckConcSlab(pConc, TRUE, 2, dLenSpan):_T("");
				else if(j==7) str = (pBri->m_bCheckSuppoerBarSlabArea)? pSupport->GetStringCheckConcSlab(pConc, FALSE, 2, dLenSpan):_T("");
				else if(j==8) str = (pBri->m_bCheckSuppoerBarSlab300)? pSupport->GetStringCheckConcSlab(pConc, TRUE, 3, dLenSpan):_T("");
				else if(j==9) str = (pBri->m_bCheckSuppoerBarSlab300)? pSupport->GetStringCheckConcSlab(pConc, FALSE, 3, dLenSpan):_T("");
				else if(j==10) str = pSupport->GetStringCheckConcSlab(pConc, TRUE, 4, dLenSpan);
				else str = _T("");
			}

			XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
		}
		XLText->AddFormatText("$n$h");
	}
	// º®Ã¼
	for(i=0; i<nCntItemWall; i++)
	{
		pConc = pBri->GetConcDataWall(i, FALSE, eEnvLimit, TRUE);
		if(pConc == NULL) continue;

		szCom = pConc->m_sTitle.Left(6);
		if(szPre != szCom) bBgColr = !bBgColr;
		szPre = szCom;

		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		long j=0; for(j=0; j<11; j++)
		{
			nMerge = 0;
			szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");
			
			if(bSkipFirstPart && j==0) continue;
			else if(j==0) str = "º»\nÃ¼";
			else if(j==1)
			{
// 				CString strTemp = _T("");
				CString strTemp = pConc->m_sTitle;
				if(strTemp.Right(1) == ")") str = strTemp.Left(13)+"ºÎ";
				else str = strTemp;
			}
			else if(j==2) str = (pBri->m_bCheckSuppoerBarWall15hs)? pSupport->GetStringCheckConcWall(pConc, TRUE, 0):_T("");
			else if(j==3) str = (pBri->m_bCheckSuppoerBarWall15hs)? pSupport->GetStringCheckConcWall(pConc, FALSE, 0):_T("");
			else if(j==4) str = (pBri->m_bCheckSuppoerBarWallMainOver20)? pSupport->GetStringCheckConcWall(pConc, TRUE, 1):_T("");
			else if(j==5) str = (pBri->m_bCheckSuppoerBarWallMainOver20)? pSupport->GetStringCheckConcWall(pConc, FALSE, 1):_T("");
			else if(j==6) str = (pBri->m_bCheckSuppoerBarWallArea)? pSupport->GetStringCheckConcWall(pConc, TRUE, 2):_T("");
			else if(j==7) str = (pBri->m_bCheckSuppoerBarWallArea)? pSupport->GetStringCheckConcWall(pConc, FALSE, 2):_T("");
			else if(j==8) str = (pBri->m_bCheckSuppoerBarWall300)? pSupport->GetStringCheckConcWall(pConc, TRUE, 3):_T("");
			else if(j==9) str = (pBri->m_bCheckSuppoerBarWall300)? pSupport->GetStringCheckConcWall(pConc, FALSE, 3):_T("");
			else if(j==10) str = pSupport->GetStringCheckConcWall(pConc, TRUE, 4);
			else str = _T("");
			
			XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
		}
		XLText->AddFormatText("$n$h");
	}
	if(pBri->m_nSelectInWallSectionDesign == 0)
	{
		// ³»Ãøº®Ã¼ - ½½·¡ºê·Î °ËÅä
		for(i=0; i<nCntItemInWall; i++)
		{
			pConc = pBri->m_pArrConcInWall[eEnvLimit].GetAt(i*2);
			if(pConc == NULL) continue;
			
			szCom = pConc->m_sTitle.Left(6);
			if(szPre != szCom) bBgColr = !bBgColr;
			szPre = szCom;
			
			CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
			long j=0; for(j=0; j<11; j++)
			{
				nMerge = 0;
				szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");
				
				if(bSkipFirstPart && j==0) continue;
				else if(j==0) str = "º»\nÃ¼";
				else if(j==1)
				{
// 					CString strTemp = _T("");
					CString strTemp = pConc->m_sTitle;
					if(strTemp.Right(1) == ")") str = strTemp.Left(14)+"ºÎ";
					else str = strTemp;
				}
				else if(j==2) str = (pBri->m_bCheckSuppoerBarWall15hs)? pSupport->GetStringCheckConcWall(pConc, TRUE, 0):_T("");
				else if(j==3) str = (pBri->m_bCheckSuppoerBarWall15hs)? pSupport->GetStringCheckConcWall(pConc, FALSE, 0):_T("");
				else if(j==4) str = (pBri->m_bCheckSuppoerBarWallMainOver20)? pSupport->GetStringCheckConcWall(pConc, TRUE, 1):_T("");
				else if(j==5) str = (pBri->m_bCheckSuppoerBarWallMainOver20)? pSupport->GetStringCheckConcWall(pConc, FALSE, 1):_T("");
				else if(j==6) str = (pBri->m_bCheckSuppoerBarWallArea)? pSupport->GetStringCheckConcWall(pConc, TRUE, 2):_T("");
				else if(j==7) str = (pBri->m_bCheckSuppoerBarWallArea)? pSupport->GetStringCheckConcWall(pConc, FALSE, 2):_T("");
				else if(j==8) str = (pBri->m_bCheckSuppoerBarWall300)? pSupport->GetStringCheckConcWall(pConc, TRUE, 3):_T("");
				else if(j==9) str = (pBri->m_bCheckSuppoerBarWall300)? pSupport->GetStringCheckConcWall(pConc, FALSE, 3):_T("");
				else if(j==10) str = pSupport->GetStringCheckConcWall(pConc, TRUE, 4);
				else str = _T("");
				
				XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
				XLText->AddFormatText("$m+%02d", nWidthData[j]);
			}
			XLText->AddFormatText("$n$h");
		}
	}
	else
	{
		// ³»Ãø º®Ã¼
		for(i = 0; i<nSizeList; i++)
		{
			CGirderAndColumn *pGirCol = m_pStd->m_pColGir->m_pList.GetAt(i);
			if(pGirCol->m_nJijum == 0 || pGirCol->m_nJijum == nSizeList - 1) continue; //#ARCBRIDGE-3568
			if(!pGirCol->m_bWall) continue;
			
			pConc = pGirCol->pConcColumn[0];
			if(pConc == NULL) continue;
			
			if(i==0) bBgColr = !bBgColr;
			CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
			long j=0; for(j=0; j<11; j++)
			{
				nMerge = 0;
				szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");
				
				if(bSkipFirstPart && j==0) continue;
				else if(j==0) str = "º»\nÃ¼";
				else if(j==1)
				{
					CString strTemp = _T("");
					str = pConc->m_sTitle;
				}
				else if(j==2) str = (pBri->m_bCheckSuppoerBarWall15hs)? pSupport->GetStringCheckConcWall(pConc, TRUE, 0):_T("");
				else if(j==3) str = (pBri->m_bCheckSuppoerBarWall15hs)? pSupport->GetStringCheckConcWall(pConc, FALSE, 0):_T("");
				else if(j==4) str = (pBri->m_bCheckSuppoerBarWallMainOver20)? pSupport->GetStringCheckConcWall(pConc, TRUE, 1):_T("");
				else if(j==5) str = (pBri->m_bCheckSuppoerBarWallMainOver20)? pSupport->GetStringCheckConcWall(pConc, FALSE, 1):_T("");
				else if(j==6) str = (pBri->m_bCheckSuppoerBarWallArea)? pSupport->GetStringCheckConcWall(pConc, TRUE, 2):_T("");
				else if(j==7) str = (pBri->m_bCheckSuppoerBarWallArea)? pSupport->GetStringCheckConcWall(pConc, FALSE, 2):_T("");
				else if(j==8) str = (pBri->m_bCheckSuppoerBarWall300)? pSupport->GetStringCheckConcWall(pConc, TRUE, 3):_T("");
				else if(j==9) str = (pBri->m_bCheckSuppoerBarWall300)? pSupport->GetStringCheckConcWall(pConc, FALSE, 3):_T("");
				else if(j==10) str = pSupport->GetStringCheckConcWall(pConc, TRUE, 4);
				else str = _T("");
				
				XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
				XLText->AddFormatText("$m+%02d", nWidthData[j]);
			}
			XLText->AddFormatText("$n$h");
			
		}
	}
	// ¾ÆÄ¡¸®ºê
	for(i=0; i<nCntItemArchRib; i++)
	{
		pConc = pBri->m_pArrConcArchRib.GetAt(i);
		if(pConc == NULL) continue;
		
		if(i==0) bBgColr = !bBgColr;
		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		long j=0; for(j=0; j<11; j++)
		{
			nMerge = (i==0 && j==0)? nCntItemArchRib-1:0;
			
			if(bSkipFirstPart && j==0) continue;
			else if(j==0) str = "¾Æ\nÄ¡\n¸®\nºê";
			else if(j==1) str = pConc->m_sTitle;
			else if(j==2) str = (pBri->m_bCheckSuppoerBarSlabMain)? pSupport->GetStringCheckConcWall(pConc, TRUE, 0):_T("");
			else if(j==3) str = (pBri->m_bCheckSuppoerBarSlabMain)? pSupport->GetStringCheckConcWall(pConc, FALSE, 0):_T("");
			else if(j==4) str = (pBri->m_bCheckSuppoerBarSlabMainOver20)? pSupport->GetStringCheckConcWall(pConc, TRUE, 1):_T("");
			else if(j==5) str = (pBri->m_bCheckSuppoerBarSlabMainOver20)? pSupport->GetStringCheckConcWall(pConc, FALSE, 1):_T("");
			else if(j==6) str = (pBri->m_bCheckSuppoerBarSlabArea)? pSupport->GetStringCheckConcWall(pConc, TRUE, 2):_T("");
			else if(j==7) str = (pBri->m_bCheckSuppoerBarSlabArea)? pSupport->GetStringCheckConcWall(pConc, FALSE, 2):_T("");
			else if(j==8) str = (pBri->m_bCheckSuppoerBarSlab300)? pSupport->GetStringCheckConcWall(pConc, TRUE, 3):_T("");
			else if(j==9) str = (pBri->m_bCheckSuppoerBarSlab300)? pSupport->GetStringCheckConcWall(pConc, FALSE, 3):_T("");
			else if(j==10) str = pSupport->GetStringCheckConcWall(pConc, TRUE, 4);
			else str = _T("");
			
			szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");
			XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
		}
		XLText->AddFormatText("$n$h");
	}
	// ºÎ»ó¹æÁöÀúÆÇ
	for(i=0; i<nCntItemPRF; i++)
	{
		pConc = pBri->GetConcDataPRF(i==0);
		if(pConc == NULL) continue;

		if(i == 0) 
			bBgColr = !bBgColr;
		
		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		long j=0; for(j=0; j<11; j++)
		{
			nMerge = 0;
			szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");

			if(bSkipFirstPart && j==0) continue;
			else if(j==0) str = "º»\nÃ¼";
			else if(j==1) str = pConc->m_sTitle;
			else if(j==2) str = (pBri->m_bCheckSuppoerBarFoot15hs)? pSupport->GetStringCheckConcFooting(pConc, TRUE, 0):_T("");
			else if(j==3) str = (pBri->m_bCheckSuppoerBarFoot15hs)? pSupport->GetStringCheckConcFooting(pConc, FALSE, 0):_T("");
			else if(j==4) str = (pBri->m_bCheckSuppoerBarFootMainOver20)? pSupport->GetStringCheckConcFooting(pConc, TRUE, 1):_T("");
			else if(j==5) str = (pBri->m_bCheckSuppoerBarFootMainOver20)? pSupport->GetStringCheckConcFooting(pConc, FALSE, 1):_T("");
			else if(j==6) str = (pBri->m_bCheckSuppoerBarFootArea)? pSupport->GetStringCheckConcFooting(pConc, TRUE, 2):_T("");
			else if(j==7) str = (pBri->m_bCheckSuppoerBarFootArea)? pSupport->GetStringCheckConcFooting(pConc, FALSE, 2):_T("");
			else if(j==8) str = (pBri->m_bCheckSuppoerBarFoot300)? pSupport->GetStringCheckConcFooting(pConc, TRUE, 3):_T("");
			else if(j==9) str = (pBri->m_bCheckSuppoerBarFoot300)? pSupport->GetStringCheckConcFooting(pConc, FALSE, 3):_T("");
			else if(j==10) str = pSupport->GetStringCheckConcFooting(pConc, TRUE, 4);
			else str = _T("");

			XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
		}
		XLText->AddFormatText("$n$h");
	}
	// ±âÃÊ
	for(i=0; i<nCntItemFoot; i++)
	{	
		pConc = pBri->m_pArrConcFoot[eEnvLimit].GetAt(i);
		if(pConc == NULL) continue;

		szCom = pConc->m_sTitle.Left(6);
		if(szPre != szCom) bBgColr = !bBgColr;
		szPre = szCom;
		
		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper, pBri->m_bCheckRebarRateFooting);
		long j=0; for(j=0; j<11; j++)
		{
			nMerge = (i==0 && j==0)? nCntItemFoot-1:0;
			szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");
			
			if(bSkipFirstPart && j==0) continue;
			else if(j==0) str = "±â\nÃÊ";
			else if(j==1) str = pConc->m_sTitle;
			else if(j==2) str = (pBri->m_bCheckSuppoerBarFoot15hs)? pSupport->GetStringCheckConcFooting(pConc, TRUE, 0):_T("");
			else if(j==3) str = (pBri->m_bCheckSuppoerBarFoot15hs)? pSupport->GetStringCheckConcFooting(pConc, FALSE, 0):_T("");
			else if(j==4) str = (pBri->m_bCheckSuppoerBarFootMainOver20)? pSupport->GetStringCheckConcFooting(pConc, TRUE, 1):_T("");
			else if(j==5) str = (pBri->m_bCheckSuppoerBarFootMainOver20)? pSupport->GetStringCheckConcFooting(pConc, FALSE, 1):_T("");
			else if(j==6) str = (pBri->m_bCheckSuppoerBarFootArea)? pSupport->GetStringCheckConcFooting(pConc, TRUE, 2):_T("");
			else if(j==7) str = (pBri->m_bCheckSuppoerBarFootArea)? pSupport->GetStringCheckConcFooting(pConc, FALSE, 2):_T("");
			else if(j==8) str = (pBri->m_bCheckSuppoerBarFoot300)? pSupport->GetStringCheckConcFooting(pConc, TRUE, 3):_T("");
			else if(j==9) str = (pBri->m_bCheckSuppoerBarFoot300)? pSupport->GetStringCheckConcFooting(pConc, FALSE, 3):_T("");
			else if(j==10) str = pSupport->GetStringCheckConcFooting(pConc, TRUE, 4);
			else str = _T("");
			
			XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
		}
		XLText->AddFormatText("$n$h");
	}
	// ³¯°³º®
	BOOL bFirst = TRUE;
	long k=0; for(k=0; k<4; k++)
	{
		if(!pBri->IsWingWall(k==0||k==1, k==0||k==2)) continue;
		CARcBridgeCalcWing *pCalcWing = m_pStd->m_pWing[k];
		
		for(i=0; i<5; i++)
		{
			if(k==0) pConc = pBri->m_pArrConcWingSttLeft.GetAt(i);
			else if(k==1) pConc = pBri->m_pArrConcWingSttRight.GetAt(i);
			else if(k==2) pConc = pBri->m_pArrConcWingEndLeft.GetAt(i);
			else if(k==3) pConc = pBri->m_pArrConcWingEndRight.GetAt(i);

			if(pConc == NULL) continue;
			if(pCalcWing->m_bIsA == FALSE && i < 3) continue;
			if(pCalcWing->m_bIsD == FALSE && i == 3) continue;
			if(pCalcWing->m_bIsA_ == FALSE && i == 4) continue;
			
			szCom = pConc->m_sTitle.Left(6);
			if(szPre != szCom) bBgColr = !bBgColr;
			szPre = szCom;

			CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
			long j=0; for(j=0; j<11; j++)
			{
				nMerge = (bFirst)? nCntItemWing-1:0;
				bFirst = FALSE;
				szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");

				if(bSkipFirstPart && j==0) continue;
				else if(j==0) str = "³¯\n°³\nº®";
				else if(j==1) str = pConc->m_sTitle;
				else if(j==2) str = (pBri->m_bCheckSuppoerBarWing15hs)? pSupport->GetStringCheckConcWingWall(pConc, TRUE, 0):_T("");
				else if(j==3) str = (pBri->m_bCheckSuppoerBarWing15hs)? pSupport->GetStringCheckConcWingWall(pConc, FALSE, 0):_T("");
				else if(j==4) str = _T("");
				else if(j==5) str = _T("");
				else if(j==6) str = (pBri->m_bCheckSuppoerBarWingArea)? pSupport->GetStringCheckConcWingWall(pConc, TRUE, 1):_T("");
				else if(j==7) str = (pBri->m_bCheckSuppoerBarWingArea)? pSupport->GetStringCheckConcWingWall(pConc, FALSE, 1):_T("");
				else if(j==8) str = (pBri->m_bCheckSuppoerBarWing300)? pSupport->GetStringCheckConcWingWall(pConc, TRUE, 2):_T("");
				else if(j==9) str = (pBri->m_bCheckSuppoerBarWing300)? pSupport->GetStringCheckConcWingWall(pConc, FALSE, 2):_T("");
				else if(j==10) str = pSupport->GetStringCheckConcWingWall(pConc, TRUE, 3);
				else str = _T("");
	
				XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e %s%s" : "$c%s%s", str, szBkColr); 
				XLText->AddFormatText("$m+%02d", nWidthData[j]);
			}
			XLText->AddFormatText("$n$h");
		}
	}
	*XLText += 1;
}

void CARcBridgeCalcSummary::MakeTableCheckSupport_Lsd(CXLFormatText *XLText)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nCntItemInWall = 0;
 	long nSizeList = m_pStd->m_pColGir->m_pList.GetSize();

	BOOL bSkipFirstPart = (pBri->IsBoxType() || pBri->IsRcSlab()) ? TRUE : FALSE;
	CString str = _T("");
	long nCntItemSlab = pBri->m_pArrConcSlab[eEnvLimit].GetSize();
	long nCntItemWall = (pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign==0)? MIN(10, pBri->GetCountConcOutWall()):0;
	long nCntItemArchRib = pBri->m_pArrConcArchRib.GetSize();
	long nCntItemPRF = pBri->m_pArrConcPRF[eEnvLimit].GetSize();

	long i=0; for(i=0; i<nSizeList; i++)
	{
		CGirderAndColumn *pGirCol = m_pStd->m_pColGir->m_pList.GetAt(i);

		CConcBasic *pConc = pGirCol->pConcColumn[0];
		if(pConc == NULL) continue;

		if(pGirCol->m_nJijum == 0 || pGirCol->m_nJijum == pBri->m_nQtyJigan)
		{
			if(pBri->m_nSelectWallSectionDesign==1)
				nCntItemWall++;
		}
		else if(pGirCol->m_bWall) nCntItemInWall++;
	}

	if(pBri->m_nSelectInWallSectionDesign == 0)
	{
		nCntItemInWall = pBri->GetCountConcInWall()/2;
 	}

	long nCntItemWing = 0;
	for(i=0; i<4; i++)
	{
		if(!pBri->IsWingWall(i==0||i==1, i==0||i==2)) continue;

		CARcBridgeCalcWing *pCalcWing = m_pStd->m_pWing[i];
		if(pCalcWing->m_bIsA) nCntItemWing += 3;
		if(pCalcWing->m_bIsD) nCntItemWing += 1;
		if(pCalcWing->m_bIsA_) nCntItemWing += 1;
	}

	long nCntItemRow = 1;
	long nRowFooting(0);
	CConcBasic *pConc;

	long nCntJijum = pBri->GetCountJijum();
	for(i=0; i<nCntJijum; i++)
	{	
		if( pBri->IsFootingJijum(i) == FALSE )
			continue;

		for (long ixFront=0; ixFront < 2; ++ixFront)
		{
			for (long ixUpper=1; ixUpper >= 0; --ixUpper)
			{
				CConcBasic *pConc = pBri->GetConcDataFooting(i, ixFront==0, ixUpper==0);
				if( pConc == NULL ) continue;

//				CString strTemp = m_pStd->m_pSummary->GetStringMainRebarUse(pConc, nCntItemRow);

				if(ixFront==0 && pConc->m_sTitle.Left(8) == "Áß°£ÁöÁ¡") 
				{
					continue;
				}

				nRowFooting += nCntItemRow;
			}
		}
	}

	long nCntRow = nCntItemSlab + nCntItemWall + nCntItemInWall + nRowFooting + nCntItemWing + nCntItemArchRib + nCntItemPRF;

	BOOL bBgColr = FALSE;
	CString szPre = _T("");
	CString szCom = _T("");
	CString szBkColr = _T("");

	str.Format("$s+%02d", nCntRow+3);
	// 	CString szTitle = _T("");
	CString szTitle = (pData->m_nTypeUnit == UNIT_TYPE_SI)? "§±":"§²";
	XLText->AddFormatText("%s$h$e¡á ¼öÆòÃ¶±Ù °ËÅä$m+19$i$me1´ÜÀ§$m+02$c:$r$e %s $n$h", str, szTitle);
	XLText->AddTable(nCntRow+1, 23, 2, 5);

	long nWidthTitle1[] = { 6, 9, 9 };
	long nWidthTitle2[] = { 6, 3, 3, 3, 3, 3, 3 };
	char *szTitle1[] = { "±¸  ºÐ", "¼öÆò¹æÇâ", "¼öÁ÷¹æÇâ" };
	char *szTitle2[] = { "±¸  ºÐ", "ÇÊ¿äÃ¶±Ù·®", "»ç¿ëÃ¶±Ù·®", "ºñ°í", "ÇÊ¿äÃ¶±Ù·®", "»ç¿ëÃ¶±Ù·®", "ºñ °í" };
	for(i=0; i<3; i++)
	{
		XLText->AddMergeFormatText((i==0)? 1:0, nWidthTitle1[i]-1, "$c%s", szTitle1[i]);
		XLText->AddFormatText("$m+%02d", nWidthTitle1[i]);
	}
	XLText->AddFormatText("$n$h");
	for(i=0; i<7; i++)
	{
		XLText->AddMergeFormatText(0, nWidthTitle2[i]-1, "$c%s", szTitle2[i]);
		XLText->AddFormatText("$m+%02d", nWidthTitle2[i]);
	}
	XLText->AddFormatText("$n$h");

	//////////////////////////////////////////////////////////////////////////
	long nWidthData[] = { 1, 5, 3, 3, 3, 3, 3, 3, 2 };
	if(bSkipFirstPart)
	{
		nWidthData[0] = 0;
		nWidthData[1] = 6;
	}

	long nMerge = 0;
	CString strEnv;
	// ½½·¡ºê
	for(i=0; i<nCntItemSlab; i++)
	{
// 		double dLenSpan = 0;
// 
// 		if(i < 2) dLenSpan = pBri->GetLengthModelSpan(0);
// 		else if(i > nCntRow-5) dLenSpan = pBri->GetLengthModelSpan(pBri->GetCountJijum()-2);
// 		else if(i%2 != 0) dLenSpan = pBri->GetLengthModelSpan((i-1)/2);
// 		else if(i%2 == 0) dLenSpan = MAX(pBri->GetLengthModelSpan(i/2 - 1), pBri->GetLengthModelSpan(i/2));

//		pConc = pBri->GetConcDataSlab_Lsd(i, &strEnv);

		pConc = pBri->m_pArrConcSlab[eEnvLimit].GetAt(i);
		if(pConc == NULL) continue;

		CLsdBeam pBeamLsd(pConc, LSD_ULS_DESIGN);
		
		long j=0; for(j=0; j<8; j++)
		{
			nMerge = (i==0 && j==0)? nCntItemSlab+nCntItemWall+nCntItemInWall-1:0;
			szBkColr = (pBri->IsBoxType() && j > 1 &&  i < nCntItemSlab / 2.) ? _T("$b040"): _T("");

			double dAsReqHori = pBeamLsd.GetAsReqDryingShrinkage(TRUE);		// ÇÊ¿äÃ¶±Ù·®
			double dAsUseHori = pBeamLsd.GetAsUseDryingShrinkage(TRUE);		// »ç¿ëÃ¶±Ù·®
			double dAsReqVert = pBeamLsd.GetAsReqDryingShrinkage(FALSE);		// ÇÊ¿äÃ¶±Ù·®
			double dAsUseVert = pBeamLsd.GetAsUseDryingShrinkage(FALSE);		// »ç¿ëÃ¶±Ù·®

			BOOL bNotCheck = (pBri->m_bCheckSupportSlabDryingShrinkage == FALSE);
			
			if(bSkipFirstPart && j==0) continue;
			else if(j==0) str = "º»\nÃ¼";
			else if(j==1) str = pConc->m_sTitle;
			else if(j==2) str.Format(_T("%.1f"), dAsReqHori);
			else if(j==3) str.Format(_T("%.f"), dAsUseHori);
			else if(j==4) str = dAsReqHori < dAsUseHori ? _T("O.K"):_T("$#03N.G");
			else if(j==5) str.Format(_T("%.1f"), dAsReqVert);
			else if(j==6) str.Format(_T("%.f"), dAsUseVert);
			else if(j==7) str = dAsReqVert < dAsUseVert ? _T("O.K"):_T("$#03N.G");	
			else str = _T("");

			if(j > 1 && bNotCheck)
				str = _T("-");
			
			XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
		}
		XLText->AddFormatText("$n$h");
	}


	// º®Ã¼
	if(pBri->m_nSelectWallSectionDesign==0)
	{
		for(i=0; i<nCntItemWall; i++)
		{
			// pConc = pBri->GetConcDataWall_Lsd(i, &strEnv);
			if(pBri->m_pArrConcWall[eEnvLimit].GetSize() > i)
				pConc = pBri->m_pArrConcWall[eEnvLimit].GetAt(i);
			else
				pConc = NULL;
			if(pConc == NULL) continue;

			szCom = pConc->m_sTitle.Left(6);
			if(szPre != szCom) bBgColr = !bBgColr;
			szPre = szCom;

			CLsdBeam pBeamLsd(pConc, LSD_ULS_DESIGN);

			double dAsReqHori = pBeamLsd.GetAsReqDryingShrinkage(TRUE);		// ÇÊ¿äÃ¶±Ù·®
			double dAsUseHori = pBeamLsd.GetAsUseDryingShrinkage(TRUE);		// »ç¿ëÃ¶±Ù·®
			double dAsReqVert = pBeamLsd.GetAsReqDryingShrinkage(FALSE);		// ÇÊ¿äÃ¶±Ù·®
			double dAsUseVert = pBeamLsd.GetAsUseDryingShrinkage(FALSE);		// »ç¿ëÃ¶±Ù·®

			BOOL bNotCheck = (pBri->m_bCheckSupportWallDryingShrinkage == FALSE);

			long j=0; for(j=0; j<8; j++)
			{
				nMerge = 0;
				szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");

				if(bSkipFirstPart && j==0) continue;
				else if(j==0) str = "º»\nÃ¼";
				else if(j==1)
				{
					// 				CString strTemp = _T("");
					CString strTemp = pConc->m_sTitle;
					if(strTemp.Right(1) == ")") str = strTemp.Left(13)+"ºÎ";
					else str = strTemp;
				}
				else if(j==2) str.Format(_T("%.1f"), dAsReqHori);
				else if(j==3) str.Format(_T("%.f"), dAsUseHori);
				else if(j==4) str = dAsReqHori < dAsUseHori ? _T("O.K"):_T("$#03N.G");
				else if(j==5) str.Format(_T("%.1f"), dAsReqVert);
				else if(j==6) str.Format(_T("%.f"), dAsUseVert);
				else if(j==7) str = dAsReqVert < dAsUseVert ? _T("O.K"):_T("$#03N.G");	
				else str = _T("");

				if(j > 1 && bNotCheck)
					str = _T("-");

				XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
				XLText->AddFormatText("$m+%02d", nWidthData[j]);
			}
			XLText->AddFormatText("$n$h");
		}
	}
	if(pBri->m_nSelectInWallSectionDesign == 0)
	{
		// ³»Ãøº®Ã¼ - ½½·¡ºê·Î °ËÅä
		for(i=0; i<nCntItemInWall; i++)
		{
			//pConc = pBri->GetConcDataInWall_Lsd(i, &strEnv);
			if(pBri->m_pArrConcInWall[eEnvLimit].GetSize() > i)
				pConc = pBri->m_pArrConcInWall[eEnvLimit].GetAt(i);
			else
				pConc = NULL;
			if(pConc == NULL) continue;

			szCom = pConc->m_sTitle.Left(6);
			if(szPre != szCom) bBgColr = !bBgColr;
			szPre = szCom;

			CLsdBeam pBeamLsd(pConc, LSD_ULS_DESIGN);

			double dAsReqHori = pBeamLsd.GetAsReqDryingShrinkage(TRUE);		// ÇÊ¿äÃ¶±Ù·®
			double dAsUseHori = pBeamLsd.GetAsUseDryingShrinkage(TRUE);		// »ç¿ëÃ¶±Ù·®
			double dAsReqVert = pBeamLsd.GetAsReqDryingShrinkage(FALSE);		// ÇÊ¿äÃ¶±Ù·®
			double dAsUseVert = pBeamLsd.GetAsUseDryingShrinkage(FALSE);		// »ç¿ëÃ¶±Ù·®

			BOOL bNotCheck = (pBri->m_bCheckSupportWallDryingShrinkage == FALSE);

			long j=0; for(j=0; j<8; j++)
			{
				nMerge = 0;
				szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");

				if(bSkipFirstPart && j==0) continue;
				else if(j==0) str = "º»\nÃ¼";
				else if(j==1)
				{
					// 					CString strTemp = _T("");
					CString strTemp = pConc->m_sTitle;
					if(strTemp.Right(1) == ")") str = strTemp.Left(14)+"ºÎ";
					else str = strTemp;
				}
				else if(j==2) str.Format(_T("%.1f"), dAsReqHori);
				else if(j==3) str.Format(_T("%.f"), dAsUseHori);
				else if(j==4) str = dAsReqHori < dAsUseHori ? _T("O.K"):_T("$#03N.G");
				else if(j==5) str.Format(_T("%.1f"), dAsReqVert);
				else if(j==6) str.Format(_T("%.f"), dAsUseVert);
				else if(j==7) str = dAsReqVert < dAsUseVert ? _T("O.K"):_T("$#03N.G");	
				else str = _T("");

				if(j > 1 && bNotCheck)
					str = _T("-");

				XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
				XLText->AddFormatText("$m+%02d", nWidthData[j]);
			}
			XLText->AddFormatText("$n$h");
		}
	}
	if(pBri->m_nSelectWallSectionDesign == 1 || pBri->m_nSelectInWallSectionDesign == 1)
	{
		// ³»Ãø º®Ã¼
		for(i = 0; i<nSizeList; i++)
		{
			CGirderAndColumn *pGirCol = m_pStd->m_pColGir->m_pList.GetAt(i);
			if(!pGirCol->m_bWall) continue;

			pConc = pGirCol->pConcColumnLsd[eEnvLimit][1];
			if(pConc == NULL) continue;

			if(pGirCol->m_nJijum == 0 || pGirCol->m_nJijum == pBri->m_nQtyJigan)
			{
				if(pBri->m_nSelectWallSectionDesign == 0)
					continue;
			}
			else
			{
				if(pBri->m_nSelectInWallSectionDesign == 0)
					continue;
			}

			if(i==0) bBgColr = !bBgColr;
			CLsdBeam pBeamLsd(pConc, LSD_ULS_DESIGN);

			double dAsReqHori = pBeamLsd.GetAsReqDryingShrinkage(TRUE);		// ÇÊ¿äÃ¶±Ù·®
			double dAsUseHori = pBeamLsd.GetAsUseDryingShrinkage(TRUE);		// »ç¿ëÃ¶±Ù·®
			double dAsReqVert = pBeamLsd.GetAsReqDryingShrinkage(FALSE);		// ÇÊ¿äÃ¶±Ù·®
			double dAsUseVert = pBeamLsd.GetAsUseDryingShrinkage(FALSE);		// »ç¿ëÃ¶±Ù·®

			BOOL bNotCheck = (pBri->m_bCheckSupportWallDryingShrinkage == FALSE);

			long j=0; for(j=0; j<8; j++)
			{
				nMerge = 0;
				szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");

				if(bSkipFirstPart && j==0) continue;
				else if(j==0) str = "º»\nÃ¼";
				else if(j==1)
				{
					CString strTemp = _T("");
					if(pGirCol->m_nJijum == 0)
						str = "½ÃÁ¡º®Ã¼";
					else if(pGirCol->m_nJijum == pBri->m_nQtyJigan)
						str = "Á¾Á¡º®Ã¼";
					else
						str = "³»Ãøº®Ã¼";
//					str = pConc->m_sTitle;
				}
				else if(j==2) str.Format(_T("%.1f"), dAsReqHori);
				else if(j==3) str.Format(_T("%.f"), dAsUseHori);
				else if(j==4) str = dAsReqHori < dAsUseHori ? _T("O.K"):_T("$#03N.G");
				else if(j==5) str.Format(_T("%.1f"), dAsReqVert);
				else if(j==6) str.Format(_T("%.f"), dAsUseVert);
				else if(j==7) str = dAsReqVert < dAsUseVert ? _T("O.K"):_T("$#03N.G");	
				else str = _T("");

				if(j > 1 && bNotCheck)
					str = _T("-");

				XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
				XLText->AddFormatText("$m+%02d", nWidthData[j]);
			}
			XLText->AddFormatText("$n$h");

		}
	}
	// ¾ÆÄ¡¸®ºê
	for(i=0; i<nCntItemArchRib; i++)
	{
		pConc = pBri->m_pArrConcArchRib.GetAt(i);
		if(pConc == NULL) continue;

		if(i==0) bBgColr = !bBgColr;
		CLsdBeam pBeamLsd(pConc, /*strEnv == "±ØÇÑ" ?*/ LSD_ULS_DESIGN /*: LSD_ESLS_DESIGN*/);

		double dAsReqHori = pBeamLsd.GetAsReqDryingShrinkage(TRUE);		// ÇÊ¿äÃ¶±Ù·®
		double dAsUseHori = pBeamLsd.GetAsUseDryingShrinkage(TRUE);		// »ç¿ëÃ¶±Ù·®
		double dAsReqVert = pBeamLsd.GetAsReqDryingShrinkage(FALSE);		// ÇÊ¿äÃ¶±Ù·®
		double dAsUseVert = pBeamLsd.GetAsUseDryingShrinkage(FALSE);		// »ç¿ëÃ¶±Ù·®

		BOOL bNotCheck = (pBri->m_bCheckSupportWallDryingShrinkage == FALSE);

		long j=0; for(j=0; j<8; j++)
		{
			nMerge = (i==0 && j==0)? nCntItemArchRib-1:0;

			if(bSkipFirstPart && j==0) continue;
			else if(j==0) str = "¾Æ\nÄ¡\n¸®\nºê";
			else if(j==1) str = pConc->m_sTitle;
			else if(j==2) str.Format(_T("%.1f"), dAsReqHori);
			else if(j==3) str.Format(_T("%.f"), dAsUseHori);
			else if(j==4) str = dAsReqHori < dAsUseHori ? _T("O.K"):_T("$#03N.G");
			else if(j==5) str.Format(_T("%.1f"), dAsReqVert);
			else if(j==6) str.Format(_T("%.f"), dAsUseVert);
			else if(j==7) str = dAsReqVert < dAsUseVert ? _T("O.K"):_T("$#03N.G");	
			else str = _T("");

			if(j > 1 && bNotCheck)
				str = _T("-");

			szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");
			XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
		}
		XLText->AddFormatText("$n$h");
	}
	// ºÎ»ó¹æÁöÀúÆÇ
	for(i=0; i<nCntItemPRF; i++)
	{
		if(pBri->m_pArrConcPRF[eEnvLimit].GetSize() > i)
			pConc = pBri->m_pArrConcPRF[eEnvLimit].GetAt(i);
		else
			pConc = NULL;
		if(pConc == NULL) continue;

		if(i == 0)
			bBgColr = !bBgColr;
		
		CLsdBeam pBeamLsd(pConc, LSD_ULS_DESIGN);

		double dAsReqHori = pBeamLsd.GetAsReqDryingShrinkage(TRUE);		// ÇÊ¿äÃ¶±Ù·®
		double dAsUseHori = pBeamLsd.GetAsUseDryingShrinkage(TRUE);		// »ç¿ëÃ¶±Ù·®
		double dAsReqVert = pBeamLsd.GetAsReqDryingShrinkage(FALSE);		// ÇÊ¿äÃ¶±Ù·®
		double dAsUseVert = pBeamLsd.GetAsUseDryingShrinkage(FALSE);		// »ç¿ëÃ¶±Ù·®

		BOOL bNotCheck = (pBri->m_bCheckSupportWallDryingShrinkage == FALSE);

		long j=0; for(j=0; j<8; j++)
		{
			nMerge = 0;
			szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");

			if(bSkipFirstPart && j==0) continue;
			else if(j==0) str = "º»\nÃ¼";
			else if(j==1)
			{
				CString strTemp = pConc->m_sTitle;
				if(strTemp.Right(1) == ")") str = strTemp.Left(13)+"ºÎ";
				else str = strTemp;
			}
			else if(j==2) str.Format(_T("%.1f"), dAsReqHori);
			else if(j==3) str.Format(_T("%.f"), dAsUseHori);
			else if(j==4) str = dAsReqHori < dAsUseHori ? _T("O.K"):_T("$#03N.G");
			else if(j==5) str.Format(_T("%.1f"), dAsReqVert);
			else if(j==6) str.Format(_T("%.f"), dAsUseVert);
			else if(j==7) str = dAsReqVert < dAsUseVert ? _T("O.K"):_T("$#03N.G");	
			else str = _T("");

			if(j > 1 && bNotCheck)
				str = _T("-");

			XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
		}
		XLText->AddFormatText("$n$h");
	}
	// ±âÃÊ
	for(i=0; i<nCntJijum; i++)
	{	
		if( pBri->IsFootingJijum(i) == FALSE )
			continue;

		for (long ixFront=0; ixFront < 2; ++ixFront)
		{
			for (long ixUpper=1; ixUpper >= 0; --ixUpper)
			{
				pConc = pBri->GetConcDataFooting(i, ixFront==0, ixUpper==0);
				if(pConc == NULL) continue;

				szCom = pConc->m_sTitle.Left(6);
				if(szPre != szCom) bBgColr = !bBgColr;
				szPre = szCom;

				CLsdBeam pBeamLsd(pConc, /*strEnv == "±ØÇÑ" ?*/ LSD_ULS_DESIGN /*: LSD_ESLS_DESIGN*/);

				double dAsReqHori = pBeamLsd.GetAsReqDryingShrinkage(TRUE);		// ÇÊ¿äÃ¶±Ù·®
				double dAsUseHori = pBeamLsd.GetAsUseDryingShrinkage(TRUE);		// »ç¿ëÃ¶±Ù·®
				double dAsReqVert = pBeamLsd.GetAsReqDryingShrinkage(FALSE);		// ÇÊ¿äÃ¶±Ù·®
				double dAsUseVert = pBeamLsd.GetAsUseDryingShrinkage(FALSE);		// »ç¿ëÃ¶±Ù·®

				BOOL bNotCheck = (pBri->m_bCheckSupportFootDryingShrinkage == FALSE);

				long j=0; for(j=0; j<8; j++)
				{
					nMerge = (i==0 && j==0 && ixFront==0 && ixUpper == 1) ? nRowFooting-1 : 0;
					szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");

					if(bSkipFirstPart && j==0) continue;
					else if(j==0) str = "±â\nÃÊ";
					else if(j==1) str = pConc->m_sTitle;
					else if(j==2) str.Format(_T("%.1f"), dAsReqHori);
					else if(j==3) str.Format(_T("%.f"), dAsUseHori);
					else if(j==4) str = dAsReqHori < dAsUseHori ? _T("O.K"):_T("$#03N.G");
					else if(j==5) str.Format(_T("%.1f"), dAsReqVert);
					else if(j==6) str.Format(_T("%.f"), dAsUseVert);
					else if(j==7) str = dAsReqVert < dAsUseVert ? _T("O.K"):_T("$#03N.G");	
					else str = _T("");

					if(j > 1 && bNotCheck)
						str = _T("-");

					XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
					XLText->AddFormatText("$m+%02d", nWidthData[j]);
				}
				XLText->AddFormatText("$n$h");
			}
			}
	}
	// ³¯°³º®
	BOOL bFirst = TRUE;
	long k=0; for(k=0; k<4; k++)
	{
		if(!pBri->IsWingWall(k==0||k==1, k==0||k==2)) continue;
		CARcBridgeCalcWing *pCalcWing = m_pStd->m_pWing[k];

		for(i=0; i<5; i++)
		{
			if(k==0) pConc = pBri->m_pArrConcWingSttLeft.GetAt(i);
			else if(k==1) pConc = pBri->m_pArrConcWingSttRight.GetAt(i);
			else if(k==2) pConc = pBri->m_pArrConcWingEndLeft.GetAt(i);
			else if(k==3) pConc = pBri->m_pArrConcWingEndRight.GetAt(i);

			if(pConc == NULL) continue;
			if(pCalcWing->m_bIsA == FALSE && i < 3) continue;
			if(pCalcWing->m_bIsD == FALSE && i == 3) continue;
			if(pCalcWing->m_bIsA_ == FALSE && i == 4) continue;

			szCom = pConc->m_sTitle.Left(6);
			if(szPre != szCom) bBgColr = !bBgColr;
			szPre = szCom;

			CLsdBeam pBeamLsd(pConc, LSD_ULS_DESIGN);

			double dAsReqHori = pBeamLsd.GetAsReqDryingShrinkage(TRUE);		// ÇÊ¿äÃ¶±Ù·®
			double dAsUseHori = pBeamLsd.GetAsUseDryingShrinkage(TRUE);		// »ç¿ëÃ¶±Ù·®
			double dAsReqVert = pBeamLsd.GetAsReqDryingShrinkage(FALSE);		// ÇÊ¿äÃ¶±Ù·®
			double dAsUseVert = pBeamLsd.GetAsUseDryingShrinkage(FALSE);		// »ç¿ëÃ¶±Ù·®

			BOOL bNotCheck = FALSE;

			long j=0; for(j=0; j<8; j++)
			{
				nMerge = (bFirst)? nCntItemWing-1:0;
				bFirst = FALSE;
				szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");

				if(bSkipFirstPart && j==0) continue;
				else if(j==0) str = "³¯\n°³\nº®";
				else if(j==1) str = pConc->m_sTitle;
				else if(pConc->m_Moment == 0) str = _T("-");
				else if(j==2) str.Format(_T("%.1f"), dAsReqHori);
				else if(j==3) str.Format(_T("%.f"), dAsUseHori);
				else if(j==4) str = dAsReqHori < dAsUseHori ? _T("O.K"):_T("$#03N.G");
				else if(j==5) str.Format(_T("%.1f"), dAsReqVert);
				else if(j==6) str.Format(_T("%.f"), dAsUseVert);
				else if(j==7) str = dAsReqVert < dAsUseVert ? _T("O.K"):_T("$#03N.G");	
				else str = _T("");

				if(j > 1 && bNotCheck)
					str = _T("-");

				XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e %s%s" : "$c%s%s", str, szBkColr); 
				XLText->AddFormatText("$m+%02d", nWidthData[j]);
			}
			XLText->AddFormatText("$n$h");
		}
	}
	*XLText += 1;
}

void CARcBridgeCalcSummary::MakeTableCheckShear(CXLFormatText *XLText)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bSkipFirstPart = (pBri->IsBoxType() || pBri->IsRcSlab()) ? TRUE : FALSE;
	BOOL bSI = pData->IsSI();
	CString str = _T("");
	long nCntItemSlab = pBri->m_pArrConcSlab[eEnvLimit].GetSize();
	long nCntItemWall = (pBri->IsOutsideWall())? MIN(6, pBri->m_pArrConcWall[eEnvLimit].GetSize()):0;
	long nCntItemInWall = (pBri->m_nSelectInWallSectionDesign == 0)? pBri->m_pArrConcInWall[eEnvLimit].GetSize()/2 : 0;
	long nCntItemFoot = pBri->m_pArrConcFoot[eEnvLimit].GetSize() + pBri->m_pArrConcFootRev[eEnvLimit].GetSize();
	long nCntItemFootReal = nCntItemFoot;
	long nCntItemArchRib = pBri->m_pArrConcArchRib.GetSize();
	long nCntItemPRF = pBri->m_pArrConcPRF[eEnvLimit].GetSize();
	long nCntItemWing = 0;
	long i=0; for(i=0; i<4; i++)
	{
		if(!pBri->IsWingWall(i==0||i==1, i==0||i==2)) continue;
		
		CARcBridgeCalcWing *pCalcWing = m_pStd->m_pWing[i];
		if(pCalcWing->m_bIsA) nCntItemWing += 3;
		if(pCalcWing->m_bIsD) nCntItemWing += 1;
		if(pCalcWing->m_bIsA_) nCntItemWing += 1;
	}

	BOOL bRev = FALSE;
	CConcBasic *pConc;
	for(i=0; i<nCntItemFoot; i++)
	{	
		long nInd = i/2;
		if(!bRev) pConc = pBri->m_pArrConcFoot[eEnvLimit].GetAt(nInd);
		else pConc = pBri->m_pArrConcFootRev[eEnvLimit].GetAt(nInd);

		bRev = !bRev;
		if(!bRev && pConc->m_sTitle.Left(8) == "Áß°£ÁöÁ¡") 
		{
			nCntItemFootReal--;
		}
	}

	long nCntRow = nCntItemSlab+nCntItemWall+nCntItemInWall+nCntItemFootReal+nCntItemWing+nCntItemArchRib+nCntItemPRF;

	BOOL bBgColr = FALSE;
	CString szPre = _T("");
	CString szCom = _T("");
	CString szBkColr = _T("");

	str.Format("$s+%02d", nCntRow+3);
	XLText->AddFormatText("%s$h$e¡á Àü´ÜÃ¶±Ù °ËÅä$n$h", str);
	XLText->AddTable(nCntRow+2, 25-1, 3, 6);
	
	long nWidthTitle1[] = { 6, 2, 2, 2, 2, 3, 6, 2 };
	long nWidthTitle2[] = { 6, 2, 2, 2, 2, 3, 2, 2, 2, 2 };
	char *szTitle1[] = { "±¸  ºÐ", "¥õVc", "Vu", "¥õVn", "´Ù¸®¼ö", "ÇÊ¿ä\nÃ¶±Ù·®", "»ç¿ëÃ¶±Ù·®", "ºñ °í" };
	char *szTitleCGS[] = { "±¸  ºÐ", "tonf", "tonf", "tonf", "°³", "§²", "Á÷°æ", "°£°Ý", "§²", "ºñ °í" };
	char *szTitleSI[] = { "±¸  ºÐ", "kN", "kN", "kN", "°³", "§±", "Á÷°æ", "°£°Ý", "§±", "ºñ °í" };
	for(i=0; i<8; i++)
	{
		long nMerge = (i==0 || i==7)? 2:1;
		XLText->AddMergeFormatText(nMerge, nWidthTitle1[i]-1, "$c%s", szTitle1[i]);
		XLText->AddFormatText("$m+%02d", nWidthTitle1[i]);
	}
	XLText->AddFormatText("$n$n$h");
	for(i=0; i<10; i++)
	{
		if(pData->IsSI()) XLText->AddMergeFormatText(0, nWidthTitle2[i]-1, "$c%s", szTitleSI[i]);
		else XLText->AddMergeFormatText(0, nWidthTitle2[i]-1, "$c%s", szTitleCGS[i]);
		XLText->AddFormatText("$m+%02d", nWidthTitle2[i]);
	}
	XLText->AddFormatText("$n$h");

	//////////////////////////////////////////////////////////////////////////
	long nWidthData[] = { 1, 5, 2, 2, 2, 2, 3, 2, 2, 2, 2 };
	if(bSkipFirstPart)
	{
		nWidthData[0] = 0;
		nWidthData[1] = 6;
	}

	long nMerge = 0;

	// ½½·¡ºê
//	BOOL bCalcShear = (pBri->m_RebarShearDetail_UpperSlab.m_nType==TYPE_SHEAR_SPACER)? FALSE : TRUE;
	for(i=0; i<nCntItemSlab; i++)
	{
		pConc = pBri->m_pArrConcSlab[eEnvLimit].GetAt(i);
		if(pConc == NULL) continue;

		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		long nCheckShearForce =  pBeam.GetCheckShearForce();
		
		long j=0; for(j=0; j<11; j++)
		{
			nMerge = (i==0 && j==0)? nCntItemSlab+nCntItemWall+nCntItemInWall-1:0;
			szBkColr = (pBri->IsBoxType() && j > 1 &&  i < nCntItemSlab / 2.) ? _T("$b040"): _T("");

			if(bSkipFirstPart && j==0) continue;
			else if(j==0) str = "º»\nÃ¼";
			else if(j==1) str = pConc->m_sTitle;
			else if(j==2) str.Format("$f2%.2f", pBeam.GetVc()*pConc->m_Pi_V);
			else if(j==3) str.Format("$f2%.2f", pBeam.GetVu());
			else if(j==4) str.Format("$f2%.2f", pBeam.GetPiVn());
			else if(j==5) str.Format("$f2%.2f", pConc->m_SvEa);
			else if(j==6)
			{
				double AvReq = 0;
				long n = (bSI)? 1:3;
				
				if(nCheckShearForce == 6) 
					AvReq = -1;
				else if(nCheckShearForce == 1 || nCheckShearForce == 2)
					AvReq = pBeam.GetMinAv();
				else
					AvReq = pBeam.GetAvReq();
				
				if(AvReq > 0) str.Format("$f%d%.3f",n, AvReq);
				else str = _T("-");
			}
			else if(j==7) str = pData->GetStringDia(pConc->m_SvDia, pConc->m_FyShear);
			else if(j==8) str.Format("$f0%.3f", pConc->m_SvCtc);
			else if(j==9)
			{
				double dArea = pBeam.GetAvUse();
				long n = (bSI)? 1 : 3;
				str.Format("$f%d%.3f", n, dArea);
			}
			else if(j==10) str = (nCheckShearForce == 0 || nCheckShearForce == 2 || nCheckShearForce == 6)? _T("O.K"):_T("$#03N.G");
			else str = _T("");

//			if(j > 5 && !bCalcShear && j < 11) str = _T("-");
			XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
		}
		XLText->AddFormatText("$n$h");
	}
	// º®Ã¼
	for(i=0; i<nCntItemWall; i++)
	{
		pConc = pBri->GetConcDataWall(i, FALSE);
		if(pConc == NULL) continue;

//		bCalcShear = (pBri->m_RebarShearDetail_OutWall[(i<nCntItemWall/2)? iSTT:iEND].m_nType==TYPE_SHEAR_SPACER)? FALSE : TRUE;
		szCom = pConc->m_sTitle.Left(6);
		if(szPre != szCom) bBgColr = !bBgColr;
		szPre = szCom;

		double PiVn = 0;
		double PiVc = 0;		// Vc´Â ÀÖ´Âµ¥ PiVc´Â ¾ø³×...
		double Vu = 0;
		long nCheckShearForce =  0;
		double ReqAv = 0;
		double UseAv = 0;

		if(pBri->m_nSelectWallSectionDesign==1)
		{
			CColumn pWall(pConc);
			pWall.CalculateAll(FALSE);

			PiVn = pWall.GetPiVn();
			PiVc = pConc->m_Pi_V * pWall.GetVc();		// Vc´Â ÀÖ´Âµ¥ PiVc´Â ¾ø³×...
			Vu = pWall.GetVu();

			nCheckShearForce =  pWall.GetCheckShearForce();
			ReqAv = (nCheckShearForce == 7)? -1 : pWall.GetMinAv();
			UseAv = pWall.GetAvUse();
		}
		else
		{
			CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
			PiVc = pBeam.GetVc()*pConc->m_Pi_V;
			Vu = pBeam.GetVu();
			PiVn = pBeam.GetPiVn();

			nCheckShearForce =  pBeam.GetCheckShearForce();
			ReqAv = (nCheckShearForce == 6)? -1 : ((nCheckShearForce == 1 || nCheckShearForce == 2)? pBeam.GetMinAv() : pBeam.GetAvReq());
			UseAv = pBeam.GetAvUse();
		}

		long j=0; for(j=0; j<11; j++)
		{
			nMerge = 0;
			szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");
			
			if(bSkipFirstPart && j==0) continue;
			else if(j==0) str = "º»\nÃ¼";
			else if(j==1)
			{
// 				CString strTemp = _T("");
				CString strTemp = pConc->m_sTitle;
				if(strTemp.Right(1) == ")") str = strTemp.Left(13)+"ºÎ";
				else str = strTemp;
			}
			else if(j==2) str.Format("$f2%.2f", PiVc);
			else if(j==3) str.Format("$f2%.2f", Vu);
			else if(j==4) str.Format("$f2%.2f", PiVn);
			else if(j==5) str.Format("$f2%.2f", pConc->m_SvEa);
			else if(j==6)
			{
				long n = (bSI)? 1:3;
				if(ReqAv > 0) str.Format("$f%d%.3f",n, ReqAv);
				else str = _T("-");
			}
			else if(j==7) str = pData->GetStringDia(pConc->m_SvDia, pConc->m_FyShear);
			else if(j==8) str.Format("$f0%.3f", pConc->m_SvCtc);
			else if(j==9)
			{
				long n = (bSI)? 1 : 3;
				str.Format("$f%d%.3f", n, UseAv);
			}
			else if(j==10)
			{	
				if(pBri->m_nSelectWallSectionDesign == 1)
				{
					str = (nCheckShearForce == 0 || nCheckShearForce == 4 || nCheckShearForce == 6)? _T("O.K"):_T("N.G");
					if(nCheckShearForce == 7) str = "-";
				}
				else
				{
					str = (nCheckShearForce == 0 || nCheckShearForce == 2 || nCheckShearForce == 6)? _T("O.K"):_T("N.G");
				}
			}
			else str = _T("");
			
//			if(j > 1 && !bCalcShear) str = _T("-");
			XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
		}
		XLText->AddFormatText("$n$h");
	}

	// ³»Ãø º®Ã¼ - ½½·¡ºê·Î °ËÅä
	for(i=0; i<nCntItemInWall; i++)
	{
		pConc = pBri->m_pArrConcInWall[eEnvLimit].GetAt(i*2);
		if(pConc == NULL) continue;

//		bCalcShear = (pBri->m_RebarShearDetail_InWall.m_nType==TYPE_SHEAR_SPACER)? FALSE : TRUE;
		szCom = pConc->m_sTitle.Left(6);
		if(szPre != szCom) bBgColr = !bBgColr;
		szPre = szCom;

		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		long nCheckShearForce =  pBeam.GetCheckShearForce();

		long j=0; for(j=0; j<11; j++)
		{
			nMerge = 0;
			szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");
			
			if(bSkipFirstPart && j==0) continue;
			else if(j==0) str = "º»\nÃ¼";
			else if(j==1)
			{
// 				CString strTemp = _T("");
				CString strTemp = pConc->m_sTitle;
				if(strTemp.Right(1) == ")") str = strTemp.Left(14)+"ºÎ";
				else str = strTemp;
			}
			else if(j==2) str.Format("$f2%.2f", pBeam.GetVc()*pConc->m_Pi_V);
			else if(j==3) str.Format("$f2%.2f", pBeam.GetVu());
			else if(j==4) str.Format("$f2%.2f", pBeam.GetPiVn());
			else if(j==5) str.Format("$f2%.2f", pConc->m_SvEa);
			else if(j==6)
			{
				double AvReq = 0;
				long n = (bSI)? 1:3;
				
				if(nCheckShearForce == 6) 
					AvReq = -1;
				else if(nCheckShearForce == 1 || nCheckShearForce == 2)
					AvReq = pBeam.GetMinAv();
				else
					AvReq = pBeam.GetAvReq();
				
				if(AvReq > 0) str.Format("$f%d%.3f",n, AvReq);
				else str = _T("-");
			}
			else if(j==7) str = pData->GetStringDia(pConc->m_SvDia, pConc->m_FyShear);
			else if(j==8) str.Format("$f0%.3f", pConc->m_SvCtc);
			else if(j==9)
			{
				double dArea = pBeam.GetAvUse();
				long n = (bSI)? 1 : 3;
				str.Format("$f%d%.3f", n, dArea);
			}
			else if(j==10) str = (nCheckShearForce == 0 || nCheckShearForce == 2 || nCheckShearForce == 6)? _T("O.K"):_T("$#03N.G");
			else str = _T("");
			
//			if(j > 1 && !bCalcShear) str = _T("-");
			XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
		}
		XLText->AddFormatText("$n$h");
	}

	// ¾ÆÄ¡¸®ºê
//	bCalcShear = (pBri->m_RebarShearDetail_ArchRib.m_nType==TYPE_SHEAR_SPACER)? FALSE : TRUE;
	for(i=0; i<nCntItemArchRib; i++)
	{
		pConc = pBri->m_pArrConcArchRib.GetAt(i);
		if(pConc == NULL) continue;
		
		if(i==0) bBgColr = !bBgColr;
		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		long nCheckShearForce =  pBeam.GetCheckShearForce();
		
		long j=0; for(j=0; j<11; j++)
		{
			nMerge = (i==0 && j==0)? nCntItemArchRib-1:0;
			
			if(bSkipFirstPart && j==0) continue;
			else if(j==0) str = "¾Æ\nÄ¡\n¸®\nºê";
			else if(j==1) str = pConc->m_sTitle;
			else if(j==2) str.Format("$f2%.2f", pBeam.GetVc()*pConc->m_Pi_V);
			else if(j==3) str.Format("$f2%.2f", pBeam.GetVu());
			else if(j==4) str.Format("$f2%.2f", pBeam.GetPiVn());
			else if(j==5) str.Format("$f2%.2f", pConc->m_SvEa);
			else if(j==6)
			{
				double AvReq = 0;
				long n = (bSI)? 1:3;
				
				if(nCheckShearForce == 6) 
					AvReq = -1;
				else if(nCheckShearForce == 1 || nCheckShearForce == 2)
					AvReq = pBeam.GetMinAv();
				else
					AvReq = pBeam.GetAvReq();
				
				if(AvReq > 0) str.Format("$f%d%.3f",n, AvReq);
				else str = _T("-");
			}
			else if(j==7) str = pData->GetStringDia(pConc->m_SvDia, pConc->m_FyShear);
			else if(j==8) str.Format("$f0%.3f", pConc->m_SvCtc);
			else if(j==9)
			{
				double dArea = pBeam.GetAvUse();
				long n = (bSI)? 1 : 3;
				str.Format("$f%d%.3f", n, dArea);
			}
			else if(j==10) str = (nCheckShearForce == 0 || nCheckShearForce == 2 || nCheckShearForce == 6)? _T("O.K"):_T("$#03N.G");
			else str = _T("");
			
//			if(j > 1 && !bCalcShear) str = _T("-");
			szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");
			XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
		}
		XLText->AddFormatText("$n$h");
	}
	
	// ºÎ»ó¹æÁöÀúÆÇ	
	for(i=0; i<nCntItemPRF; i++)
	{
		pConc = pBri->m_pArrConcPRF[eEnvLimit].GetAt(i);
		if(pConc == NULL) continue;

		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
		long nCheckShearForce =  pBeam.GetCheckShearForce();

		if(i == 0)
			bBgColr = !bBgColr;
		for(long j=0; j<11; j++)
		{
			if(bSkipFirstPart && j==0) continue;
			if(j==0) str = "º»\nÃ¼";
			else if(j==1) str = pConc->m_sTitle;
			else if(j==2) str.Format("$f2%.2f", pBeam.GetVc()*pConc->m_Pi_V);
			else if(j==3) str.Format("$f2%.2f", pBeam.GetVu());
			else if(j==4) str.Format("$f2%.2f", pBeam.GetPiVn());
			else if(j==5) str.Format("$f2%.2f", pConc->m_SvEa);
			else if(j==6)
			{
				double AvReq = 0;
				long n = (bSI)? 1:3;

				if(nCheckShearForce == 6) 
					AvReq = -1;
				else if(nCheckShearForce == 1 || nCheckShearForce == 2)
					AvReq = pBeam.GetMinAv();
				else
					AvReq = pBeam.GetAvReq();

				if(AvReq > 0) str.Format("$f%d%.3f",n, AvReq);
				else str = _T("-");
			}
			else if(j==7) str = pData->GetStringDia(pConc->m_SvDia, pConc->m_FyShear);
			else if(j==8) str.Format("$f0%.3f", pConc->m_SvCtc);
			else if(j==9)
			{
				double dArea = pBeam.GetAvUse();
				long n = (bSI)? 1 : 3;
				str.Format("$f%d%.3f", n, dArea);
			}
			else if(j==10) str = (nCheckShearForce == 0 || nCheckShearForce == 2 || nCheckShearForce == 6)? _T("O.K"):_T("$#03N.G");
			else str = _T("");

			XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
		}
		XLText->AddFormatText("$n$h");
	}
	
	// ±âÃÊ
	bRev = FALSE;
	for(i=0; i<nCntItemFoot; i++)
	{	
		long nInd = i/2;
		if(!bRev) pConc = pBri->m_pArrConcFoot[eEnvLimit].GetAt(nInd);
		else pConc = pBri->m_pArrConcFootRev[eEnvLimit].GetAt(nInd);

		if(pConc == NULL) continue;

		BOOL bUpper = (pConc->m_sTitle.Right(6) == _T("(µÞ±Á)"))? !bRev : bRev;
		CString strSub = (!bUpper)? _T("-ÇÏ¸é") : _T("-»ó¸é");

		bRev = !bRev;
		if(!bRev && pConc->m_sTitle.Left(8) == "Áß°£ÁöÁ¡") 
		{
			continue;
		}

		szCom = pConc->m_sTitle.Left(6);
		if(szPre != szCom) bBgColr = !bBgColr;
		szPre = szCom;

		
//		bCalcShear = (pConc->m_SvDia==0 && pConc->m_SvCtc==0 && pConc->m_SvEa==0)? FALSE : TRUE;

		CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper, pBri->m_bCheckRebarRateFooting);
		long nCheckShearForce =  pBeam.GetCheckShearForce();

		long j=0; for(j=0; j<11; j++)
		{
			nMerge = (i==0 && j==0)? nCntItemFootReal-1:0;
			szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");
			
			if(bSkipFirstPart && j==0) continue;
			else if(j==0) str = "±â\nÃÊ";
			else if(j==1) str = pConc->m_sTitle+strSub;
			else if(j==2) str.Format("$f2%.2f", pBeam.GetVc()*pConc->m_Pi_V);
			else if(j==3) str.Format("$f2%.2f", pBeam.GetVu());
			else if(j==4) str.Format("$f2%.2f", pBeam.GetPiVn());
			else if(j==5) str.Format("$f2%.2f", pConc->m_SvEa);
			else if(j==6)
			{
				double AvReq = 0;
				long n = (bSI)? 1:3;
				
				if(nCheckShearForce == 6) 
					AvReq = -1;
				else if(nCheckShearForce == 1 || nCheckShearForce == 2)
					AvReq = pBeam.GetMinAv();
				else
					AvReq = pBeam.GetAvReq();
				
				if(AvReq > 0) str.Format("$f%d%.3f",n, AvReq);
				else str = _T("-");
			}
			else if(j==7) str = pData->GetStringDia(pConc->m_SvDia, pConc->m_FyShear);
			else if(j==8) str.Format("$f0%.3f", pConc->m_SvCtc);
			else if(j==9)
			{
				double dArea = pBeam.GetAvUse();
				long n = (bSI)? 1 : 3;
				str.Format("$f%d%.3f", n, dArea);
			}
			else if(j==10) str = (nCheckShearForce == 0 || nCheckShearForce == 2 || nCheckShearForce == 6)? _T("O.K"):_T("$#03N.G");
			else str = _T("");
			
//			if(j > 1 && !bCalcShear) str = _T("-");
			XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
		}
		XLText->AddFormatText("$n$h");
	}
	// ³¯°³º®
	BOOL bFirst = TRUE;
	long k=0; for(k=0; k<4; k++)
	{
		if(!pBri->IsWingWall(k==0||k==1, k==0||k==2)) continue;
 		CARcBridgeCalcWing *pCalcWing = m_pStd->m_pWing[k];
		
		for(i=0; i<5; i++)
		{
			if(k==0) pConc = pBri->m_pArrConcWingSttLeft.GetAt(i);
			else if(k==1) pConc = pBri->m_pArrConcWingSttRight.GetAt(i);
			else if(k==2) pConc = pBri->m_pArrConcWingEndLeft.GetAt(i);
			else if(k==3) pConc = pBri->m_pArrConcWingEndRight.GetAt(i);

// 			if(i==0) bCalcShear = (pCalcWing->m_pWing->m_RebarShearDetailA.m_nType==TYPE_SHEAR_SPACER)? FALSE : TRUE;
// 			else if(i==1) bCalcShear = ((pCalcWing->m_pWing->m_nTypeShearBC==1)? pCalcWing->m_pWing->m_RebarShearDetailC.m_nType==TYPE_SHEAR_SPACER : pCalcWing->m_pWing->m_RebarShearDetailB.m_nType==TYPE_SHEAR_SPACER)? FALSE : TRUE;
// 			else if(i==2) bCalcShear = ((pCalcWing->m_pWing->m_nTypeShearBC==0)? pCalcWing->m_pWing->m_RebarShearDetailB.m_nType==TYPE_SHEAR_SPACER : pCalcWing->m_pWing->m_RebarShearDetailC.m_nType==TYPE_SHEAR_SPACER)? FALSE : TRUE;
// 			else if(i==3) bCalcShear = (pCalcWing->m_pWing->m_RebarShearDetailD.m_nType==TYPE_SHEAR_SPACER)? FALSE : TRUE;
// 			else if(i==4) bCalcShear = (pCalcWing->m_pWing->m_RebarShearDetailA.m_nType==TYPE_SHEAR_SPACER)? FALSE : TRUE;
// 			else bCalcShear = FALSE;

			if(pConc == NULL) continue;
			if(pCalcWing->m_bIsA == FALSE && i < 3) continue;
			if(pCalcWing->m_bIsD == FALSE && i == 3) continue;
			if(pCalcWing->m_bIsA_ == FALSE && i == 4) continue;

			szCom = pConc->m_sTitle.Left(6);
			if(szPre != szCom) bBgColr = !bBgColr;
			szPre = szCom;

			CRectBeam pBeam(pConc, pBri->m_bCheckMainSlabAndFootTemper);
			long nCheckShearForce =  pBeam.GetCheckShearForce();
			
			long j=0; for(j=0; j<11; j++)
			{
				nMerge = (bFirst)? nCntItemWing-1:0;
				bFirst = FALSE;
				szBkColr = (j > 1 && bBgColr)? _T("$b040"): _T("");

				if(bSkipFirstPart && j==0) continue;
				else if(j==0) str = "³¯\n°³\nº®";
 				else if(j==1) str = pConc->m_sTitle;
				else if(j==2) str.Format("$f2%.2f", pBeam.GetVc()*pConc->m_Pi_V);
				else if(j==3) str.Format("$f2%.2f", pBeam.GetVu());
				else if(j==4) str.Format("$f2%.2f", pBeam.GetPiVn());
				else if(j==5) str.Format("$f2%.2f", pConc->m_SvEa);
				else if(j==6)
				{
					double AvReq = 0;
					long n = (bSI)? 1:3;
					
					if(nCheckShearForce == 6) 
						AvReq = -1;
					else if(nCheckShearForce == 1 || nCheckShearForce == 2)
						AvReq = pBeam.GetMinAv();
					else
						AvReq = pBeam.GetAvReq();
					
					if(AvReq > 0) str.Format("$f%d%.3f",n, AvReq);
					else str = _T("-");
				}
				else if(j==7) str = pData->GetStringDia(pConc->m_SvDia, pConc->m_FyShear);
				else if(j==8) str.Format("$f0%.3f", pConc->m_SvCtc);
				else if(j==9)
				{
					double dArea = (pData->IsSI()) ? frCm2(pConc->GetAreaRebar(pConc->m_SvDia, pConc->m_SvEa)) 
						: pConc->GetAreaRebar(pConc->m_SvDia, pConc->m_SvEa);

					long n = (bSI)? 1 : 3;
					str.Format("$f%d%.3f", n, dArea);
				}
				else if(j==10) str = (nCheckShearForce == 0 || nCheckShearForce == 2 || nCheckShearForce == 6)? _T("O.K"):_T("$#03N.G");
				else str = _T("");
				
//				if(j > 1 && !bCalcShear) str = _T("-");
				XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e %s%s" : "$c%s%s", str, szBkColr); 
				XLText->AddFormatText("$m+%02d", nWidthData[j]);
			}
			XLText->AddFormatText("$n$h");
		}
	}
	*XLText += 1;
}

void CARcBridgeCalcSummary::MakeTableCheckShear_Lsd(CXLFormatText *XLText)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bSkipFirstPart = (pBri->IsRcSlab()) ? TRUE : FALSE;
	CString str = _T("");
	long nCntItemSlab = pBri->GetCountConcUpperAndLowerSlab(TRUE);
	long nCntItemLoSlab = pBri->IsBoxType()? pBri->GetCountConcUpperAndLowerSlab(FALSE) : 0;
	long nCntItemWall = (pBri->IsOutsideWall())? pBri->GetCountConcOutWall()/10 * 6:0;
	long nCntItemInWall = (pBri->m_nSelectInWallSectionDesign == 0)? pBri->GetCountConcInWall() : 0;
	long nCntItemFoot = pBri->GetCountFooting();
	long nCntItemArchRib = pBri->m_pArrConcArchRib.GetSize();
	long nCntItemPRF = pBri->GetCountConcPRF();
	long nCntItemWing = 0;
	long nRowFooting = 0;
	const HGINT32 icValidEnvType = pBri->GetCountValidComboEnvType();

	long i=0; for(i=0; i<4; i++)
	{
		if(!pBri->IsWingWall(i==0||i==1, i==0||i==2)) continue;

		CARcBridgeCalcWing *pCalcWing = m_pStd->m_pWing[i];
		if(pCalcWing->m_bIsA) nCntItemWing += 3;
		if(pCalcWing->m_bIsD) nCntItemWing += 1;
		if(pCalcWing->m_bIsA_) nCntItemWing += 1;
	}

	long nRowSlab = 0;
	long nRowLoSlab = 0;
	CConcBasic *pConc = NULL;
	CString strTemp = _T("");
	CDoubleArray dItemRowArr;
	CDoubleArray dItemFootingTitleMerge;		// ±âÃÊ ¸ÓÁö °³¼ö
	long nCntItemRow = 1;
	// ¾ÆÄ¡¸®ºê±îÁö µ¹·Á¼­ ÀúÀå
	for(i=0; i<nCntItemSlab+nCntItemLoSlab+nCntItemWall+nCntItemInWall+nCntItemArchRib+nCntItemPRF; i++)			
	{
		BOOL bSlab = (i<nCntItemSlab+nCntItemLoSlab)? TRUE:FALSE;

		dItemRowArr.Add(nCntItemRow);

		if( bSlab )
		{
			if(pBri->IsBoxType())
			{
				if(i<nCntItemSlab+nCntItemLoSlab && nCntItemSlab <= i)
					nRowLoSlab += nCntItemRow;
				else
					nRowSlab += nCntItemRow;
			}
			else
			{
				nRowSlab += nCntItemRow;
			}		
		}
	}

	// ±âÃÊ
	long nCntJijum = pBri->GetCountJijum();

	for(i=0; i<nCntJijum; ++i)
	{
		if(pBri->IsFootingJijum(i) == FALSE)
			continue;

		long nMergeCntFoot(0);
		for(long nFront=0; nFront < 2; ++nFront)
		{
			for(long nUpper=0; nUpper<2; ++nUpper)
			{
				for(HGINT32 ixEnvType = 0; ixEnvType < eEnvService; ++ixEnvType)
				{
					if(pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(ixEnvType)) == FALSE) continue;

					pConc = pBri->GetConcDataFooting(i, nFront==0, nUpper==0, -1, static_cast<ETypeEnvComboLsd>(ixEnvType));
					if( pConc == NULL ) continue;

//					strTemp = m_pStd->m_pSummary->GetStringMainRebarUse(pConc, nCntItemRow);
					dItemRowArr.Add(nCntItemRow);
					nRowFooting += nCntItemRow;
					nMergeCntFoot++;
				}
			}
		}

		dItemFootingTitleMerge.Add(nMergeCntFoot);
	}

	// ³¯°³º®
	long nArrArea[5] = { AREA_D, AREA_A, AREA_B, AREA_C, AREA_A_ };

	for (long ixStt = 0; ixStt < 2; ++ixStt)
	{
		for (long ixLeft = iLEFT; ixLeft <= iRIGHT; ++ixLeft)
		{
			for(long ix=0; ix<5; ++ix)
			{
				long nIndex(0);
				if ( ixStt == 0 && ixLeft==iLEFT ) 
					nIndex = 0;
				else if( ixStt == 0 && ixLeft == iRIGHT)
					nIndex = 1;
				else if( ixStt == 1 && ixLeft == iLEFT)
					nIndex = 2;
				else
					nIndex = 3;

				m_pStd->m_pWing[nIndex]->CalculateAll(ixStt==0, ixLeft==iLEFT);

				long nArea = nArrArea[ix];
				if(nArea == AREA_D && !m_pStd->m_pWing[nIndex]->m_bIsD) continue;
				else if(nArea == AREA_A_ && !m_pStd->m_pWing[nIndex]->m_bIsA_) continue;
				else if(nArea < AREA_D && !m_pStd->m_pWing[nIndex]->m_bIsA) continue;
				else if(nArea == AREA_A && !m_pStd->m_pWing[nIndex]->m_bIsA_Calc) continue;

				pConc = pBri->GetConcDataWingWall(ixStt==0, ixLeft==iLEFT, nArea);
				if( pConc == NULL ) continue;

//				strTemp = m_pStd->m_pSummary->GetStringMainRebarUse(pConc, nCntItemRow);

				dItemRowArr.Add(nCntItemRow);
			}
		}
	}

	BOOL bBgColr = FALSE;
	CString szPre = _T("");
	CString szCom = _T("");
	CString szBkColr = _T("");

	long nCntRow = nCntItemSlab+nCntItemLoSlab+nCntItemWall+nCntItemInWall+nCntItemWing+nCntItemArchRib+nCntItemPRF+nRowFooting - 1;

	str.Format("$s+%02d", nCntRow+3);
	XLText->AddFormatText("%s$h$e¡á Àü´ÜÃ¶±Ù °ËÅä$n$h", str);
	XLText->AddTable(nCntRow+2, 25-1, 2, 6);

	long nWidthTitle[] = { 6, 3, 3, 3, 2, 2, 2, 2, 2 };
	char *szTitle[] = { "±¸  ºÐ", "Vcd\n(N)", "Vsd\n(N)", "Vu\n(N)", "pv.min", "pv.use", "S_max\n(mm)", "S\n(mm)", "ºñ°í" };

	for(i=0; i<9; i++)
	{
		XLText->AddMergeFormatText(1, nWidthTitle[i]-1, "$c%s", szTitle[i]);
		XLText->AddFormatText("$m+%02d", nWidthTitle[i]);
	}
	XLText->AddFormatText("$n$n$h");
	
	//////////////////////////////////////////////////////////////////////////
	long nWidthData[] = { 1, 3, 2, 3, 3, 3, 2, 2, 2, 2, 2 };
	if(bSkipFirstPart)
	{
		nWidthData[0] = 0;
		nWidthData[1] = 4;
	}

	long nMerge = 0;

	// ½½·¡ºê
	CString strEnv;
	for(i=0; i<nCntItemSlab+nCntItemLoSlab; i++)
	{
		pConc = pBri->GetConcDataSlab_Lsd(i, &strEnv);
		if(pConc == NULL) continue;

		CLsdBeam pBeamLsd(pConc, strEnv == "±ØÇÑ" ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN);

		const HGBOOL bNeedShear = pBeamLsd.IsNeedShearRebar();
		const HGBOOL bShearOk = (bNeedShear && (!pBeamLsd.IsOk_SvCtc() || !pBeamLsd.IsOk_Vsd() || !pBeamLsd.IsOk_pvMin()))? FALSE : TRUE;
		const HGBOOL bMinAv = (bNeedShear || pBeamLsd.m_pData->m_bMinAvCheck);

		for(long j=0; j<11; j++)
		{
			if(pBri->IsBoxType())
			{
				if(i==0 && j== 0)
					nMerge = (nRowSlab - 1);
				else if(i == nCntItemSlab && j ==0 )
					nMerge = nRowLoSlab -1;
				else
					nMerge = 0;
			}
			else
			{
				nMerge = (i==0 && j==0) ? (nRowSlab - 1) : 0;
			}
			szBkColr = (pBri->IsBoxType() && j > 2 &&  i < nCntItemSlab+nCntItemLoSlab / 2.) ? _T("$b040"): _T("");

			if(bSkipFirstPart && j==0) continue;
			else if(j==0)
			{
				if(pBri->IsBoxType())
				{
					if(i < nCntItemSlab)
						str = "»óºÎ½½·¡ºê";
					else 
						str = "ÇÏºÎ½½·¡ºê";
				}
				else
				{
					str = "½½·¡ºê";
				}				
			}
			else if(j==1)
			{
				HGINT32 ix = pConc->m_sTitle.Find(" ");
				if(ix > 0)
				{
					str = pConc->m_sTitle.Mid(ix);
				}
				else
				{
					str = pConc->m_sTitle;
				}

				ix = str.Find("(");
				if (ix > 0)
				{
					str.Insert(ix, "\n");
				}
			}
			else if(j==2) str = strEnv;
			else if(j==3) str.Format("$f2%.2f", pBeamLsd.GetVcd());
			else if(j==4)
			{
				if(bNeedShear)
					str.Format("$f2%.2f", pBeamLsd.GetVd());
				else
					str = _T("-");
			}
			else if(j==5) str.Format("$f2%.2f", pBeamLsd.GetVu());
			else if(j==6) str.Format("$f5%.5f", pBeamLsd.GetpvMin());
			else if(j==7) str.Format("$f5%.5f", pBeamLsd.GetpvUse());
			else if(j==8) str.Format("$f2%.2f", pBeamLsd.GetSMax());		
			else if(j==9) str.Format("$f2%.2f", pBeamLsd.GetSvCtc());
			else if(j==10) str = bShearOk ? _T("O.K"):_T("$#03N.G");
			else str = _T("");


			if(bMinAv == FALSE && (j>=6 && j<=9)) //ÃÖ¼ÒÀü´ÜÃ¶±Ù,Àü´ÜÃ¶±Ù°£°Ý
				str = _T("-");

			if(j == 1 && icValidEnvType > 1 && i % icValidEnvType == 0)
			{
				HGINT32 icMerge = 0;
				for (HGINT32 ix = 0; ix < icValidEnvType; ++ix)
				{
					icMerge += static_cast<HGINT32>(dItemRowArr.GetAt(i+ix));
				}

				XLText->AddMergeFormatText(icMerge - 1, nWidthData[j]-1, "$j$c%s", str);
			}
			else
				XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, "$j$c%s", str);
				
			XLText->Merge(nMerge, 0);
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
			XLText->OutXL();
		}
		XLText->AddFormatText("$n$h");
	}

	// º®Ã¼
	BOOL bIsOutWall = (pBri->IsOutsideWall());
	for(i=0; i<nCntItemWall; i++)
	{
		pConc = pBri->GetConcDataWall_Lsd(i, &strEnv, FALSE);
		if(pConc == NULL) continue;

		szCom = pConc->m_sTitle.Left(6);
		if(szPre != szCom) bBgColr = !bBgColr;
		szPre = szCom;

		CLsdBeam pBeamLsd(pConc, strEnv == "±ØÇÑ" ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN);
		
		const HGBOOL bNeedShear = pBeamLsd.IsNeedShearRebar();
		const HGBOOL bShearOk = (bNeedShear && (!pBeamLsd.IsOk_SvCtc() || !pBeamLsd.IsOk_Vsd() || !pBeamLsd.IsOk_pvMin()))? FALSE : TRUE;
		const HGBOOL bMinAv = (bNeedShear || pBeamLsd.m_pData->m_bMinAvCheck);

		for(long j=0; j<11; j++)
		{
			nMerge = ((i==0 || i==nCntItemWall/2) && j==0)? nCntItemWall/2-1:0;
			szBkColr = (j > 2 && bBgColr)? _T("$b040"): _T("");

			if(bSkipFirstPart && j==0) continue;
			else if(j==0)
			{
				if(bIsOutWall && i < nCntItemWall / 2) str = "½Ã\nÁ¡\nº®\nÃ¼";
				else if(i<nCntItemWall && bIsOutWall) str = "Á¾\nÁ¡\nº®\nÃ¼";
				else str = "";
			}
			else if(j==1)
			{
				HGINT32 ix = pConc->m_sTitle.Find(" ");
				if(ix > 0)
				{
					str = pConc->m_sTitle.Mid(ix);
				}
				else
				{
					CString strTemp = pConc->m_sTitle;
					if(strTemp.Right(1) == ")") str = strTemp.Left(13)+"ºÎ";
					else str = strTemp;
				}

				ix = str.Find("(");
				if (ix > 0)
				{
					str.Insert(ix, "\n");
				}
			}
			else if(j==2) str = strEnv;
			else if(j==3) str.Format("$f2%.2f", pBeamLsd.GetVcd());
			else if(j==4)
			{
				if(bNeedShear)
					str.Format("$f2%.2f", pBeamLsd.GetVd());
				else
					str = _T("-");
			}
			else if(j==5) str.Format("$f2%.2f", pBeamLsd.GetVu());
			else if(j==6) str.Format("$f5%.5f", pBeamLsd.GetpvMin());
			else if(j==7) str.Format("$f5%.5f", pBeamLsd.GetpvUse());
			else if(j==8) str.Format("$f2%.2f", pBeamLsd.GetSMax());
			else if(j==9) str.Format("$f2%.2f", pBeamLsd.GetSvCtc());
			else if(j==10) str = bShearOk ? _T("O.K"):_T("$#03N.G");
			else str = _T("");

			if(bMinAv == FALSE && (j>=6 && j<=9)) //ÃÖ¼ÒÀü´ÜÃ¶±Ù,Àü´ÜÃ¶±Ù°£°Ý
				str = _T("-");

			if (j == 1 && icValidEnvType > 1 && i % icValidEnvType == 0)
			{
				HGINT32 icMerge = 0;
				for (HGINT32 ix = 0; ix < icValidEnvType; ++ix)
				{
					icMerge += static_cast<HGINT32>(dItemRowArr.GetAt(i+ix+nCntItemSlab+nCntItemLoSlab));
				}

				XLText->AddMergeFormatText(icMerge - 1, nWidthData[j]-1, "$j$c%s", str);
			}
			else
				XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
		}
		XLText->AddFormatText("$n$h");
	}
	
	// ³»Ãø º®Ã¼ - ½½·¡ºê·Î °ËÅä
	for(i=0; i<nCntItemInWall; i++)
	{
		pConc = pBri->GetConcDataInWall_Lsd(i, &strEnv);
		if(pConc == NULL) continue;

		szCom = pConc->m_sTitle.Left(6);
		if(szPre != szCom) bBgColr = !bBgColr;
		szPre = szCom;

		CLsdBeam pBeamLsd(pConc, strEnv == "±ØÇÑ" ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN);
		
		const HGBOOL bNeedShear = pBeamLsd.IsNeedShearRebar();
		const HGBOOL bShearOk = (bNeedShear && (!pBeamLsd.IsOk_SvCtc() || !pBeamLsd.IsOk_Vsd() || !pBeamLsd.IsOk_pvMin()))? FALSE : TRUE;
		const HGBOOL bMinAv = (bNeedShear || pBeamLsd.m_pData->m_bMinAvCheck);

		for(long j=0; j<11; j++)
		{
			nMerge = (i==0 && j==0)? nCntItemInWall-1:0;
			szBkColr = (j > 2 && bBgColr)? _T("$b040"): _T("");

			if(bSkipFirstPart && j==0) continue;
			else if(j==0) str = "³»\nÃø\nº®\nÃ¼";
			else if(j==1)
			{
				HGINT32 ix = pConc->m_sTitle.Find(" ");
				if(ix > 0)
				{
					str = pConc->m_sTitle.Mid(ix);
				}
				else
				{
					CString strTemp = pConc->m_sTitle;
					if(strTemp.Right(1) == ")") str = strTemp.Left(14)+"ºÎ";
					else str = strTemp;
				}

				ix = str.Find("(");
				if (ix > 0)
				{
					str.Insert(ix, "\n");
				}				
			}
			else if(j==2) str = strEnv;
			else if(j==3) str.Format("$f2%.2f", pBeamLsd.GetVcd());
			else if(j==4)
			{
				if(bNeedShear)
					str.Format("$f2%.2f", pBeamLsd.GetVd());
				else
					str = _T("-");
			}
			else if(j==5) str.Format("$f2%.2f", pBeamLsd.GetVu());
			else if(j==6) str.Format("$f5%.5f", pBeamLsd.GetpvMin());
			else if(j==7) str.Format("$f5%.5f", pBeamLsd.GetpvUse());
			else if(j==8) str.Format("$f2%.2f", pBeamLsd.GetSMax());
			else if(j==9) str.Format("$f2%.2f", pBeamLsd.GetSvCtc());
			else if(j==10) str = bShearOk ? _T("O.K"):_T("$#03N.G");
			else str = _T("");

			if(bMinAv == FALSE && (j>=6 && j<=9)) //ÃÖ¼ÒÀü´ÜÃ¶±Ù,Àü´ÜÃ¶±Ù°£°Ý
				str = _T("-");

			if(j == 1 && icValidEnvType > 1 && i % icValidEnvType == 0)
			{
				HGINT32 icMerge = 0;
				for (HGINT32 ix = 0; ix < icValidEnvType; ++ix)
				{
					icMerge += static_cast<HGINT32>(dItemRowArr.GetAt(i+ix+nCntItemSlab+nCntItemLoSlab+nCntItemWall));
				}

				XLText->AddMergeFormatText(icMerge - 1, nWidthData[j]-1, "$j$c%s", str);
			}
			else
				XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
		}
		XLText->AddFormatText("$n$h");
	}
	
	// ¾ÆÄ¡¸®ºê
	for(i=0; i<nCntItemArchRib; i++)
	{
		pConc = pBri->m_pArrConcArchRib.GetAt(i);
		if(pConc == NULL) continue;

		if(i==0) bBgColr = !bBgColr;
		CLsdBeam pBeamLsd(pConc, LSD_ULS_DESIGN);
		
		const HGBOOL bNeedShear = pBeamLsd.IsNeedShearRebar();
		const HGBOOL bShearOk = (bNeedShear && (!pBeamLsd.IsOk_SvCtc() || !pBeamLsd.IsOk_Vsd() || !pBeamLsd.IsOk_pvMin()))? FALSE : TRUE;
		const HGBOOL bMinAv = (bNeedShear || pBeamLsd.m_pData->m_bMinAvCheck);

		for(long j=0; j<11; j++)
		{
			nMerge = (i==0 && j==0)? nCntItemArchRib-1:0;

			if(bSkipFirstPart && j==0) continue;
			else if(j==0) str = "¾Æ\nÄ¡\n¸®\nºê";
			else if(j==1)
			{
				HGINT32 ix = pConc->m_sTitle.Find(" ");
				if(ix > 0)
				{
					str = pConc->m_sTitle.Mid(ix);
				}
				else
				{
					str = pConc->m_sTitle;
				}

				ix = str.Find("(");
				if (ix > 0)
				{
					str.Insert(ix, "\n");
				}
			}	
			else if(j==2) str = strEnv;
			else if(j==3) str.Format("$f2%.2f", pBeamLsd.GetVcd());
			else if(j==4)
			{
				if(bNeedShear)
					str.Format("$f2%.2f", pBeamLsd.GetVd());
				else
					str = _T("-");
			}
			else if(j==5) str.Format("$f2%.2f", pBeamLsd.GetVu());
			else if(j==6) str.Format("$f5%.5f", pBeamLsd.GetpvMin());
			else if(j==7) str.Format("$f5%.5f", pBeamLsd.GetpvUse());
			else if(j==8) str.Format("$f2%.2f", pBeamLsd.GetSMax());
			else if(j==9) str.Format("$f2%.2f", pBeamLsd.GetSvCtc());
			else if(j==10) str = bShearOk ? _T("O.K"):_T("$#03N.G");
			else str = _T("");

			if(bMinAv == FALSE && (j>=6 && j<=9)) //ÃÖ¼ÒÀü´ÜÃ¶±Ù,Àü´ÜÃ¶±Ù°£°Ý
				str = _T("-");

			szBkColr = (j > 2 && bBgColr)? _T("$b040"): _T("");
			if(j == 1 && icValidEnvType > 1 && i % icValidEnvType == 0)
			{
				HGINT32 icMerge = 0;
				for (HGINT32 ix = 0; ix < icValidEnvType; ++ix)
				{
					icMerge += static_cast<HGINT32>(dItemRowArr.GetAt(i+ix+nCntItemSlab+nCntItemLoSlab+nCntItemWall+nCntItemInWall));
				}

				XLText->AddMergeFormatText(icMerge - 1, nWidthData[j]-1, "$j$c%s", str);
			}
			else
				XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
		}
		XLText->AddFormatText("$n$h");
	}
	// ºÎ»ó¹æÁöÀúÆÇ
	for(i=0; i<nCntItemPRF; i++)
	{
		pConc = pBri->GetConcDataPRF_Lsd(i, &strEnv);
		if(pConc == NULL) continue;

		if(i == 0)
			bBgColr = !bBgColr;

		CLsdBeam pBeamLsd(pConc, strEnv == "±ØÇÑ" ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN);

		const HGBOOL bNeedShear = pBeamLsd.IsNeedShearRebar();
		const HGBOOL bShearOk = (bNeedShear && (!pBeamLsd.IsOk_SvCtc() || !pBeamLsd.IsOk_Vsd() || !pBeamLsd.IsOk_pvMin()))? FALSE : TRUE;
		const HGBOOL bMinAv = (bNeedShear || pBeamLsd.m_pData->m_bMinAvCheck);

		for(long j=0; j<11; j++)
		{
			nMerge = (i == 0 && j == 0)? nCntItemPRF - 1 : 0;
			szBkColr = (j > 2 && bBgColr)? _T("$b040"): _T("");

			if(bSkipFirstPart && j==0) continue;
			else if(j==0)
			{
				str = _T("ºÎ»ó¹æÁöÀúÆÇ");
			}
			else if(j==1)
			{
				HGINT32 ix = pConc->m_sTitle.Find(" ");
				if(ix > 0)
				{
					str = pConc->m_sTitle.Mid(ix);
				}

				ix = str.Find("(");
				if (ix > 0)
				{
					str.Insert(ix, "\n");
				}
			}
			else if(j==2) str = strEnv;
			else if(j==3) str.Format("$f2%.2f", pBeamLsd.GetVcd());
			else if(j==4)
			{
				if(bNeedShear)
					str.Format("$f2%.2f", pBeamLsd.GetVd());
				else
					str = _T("-");
			}
			else if(j==5) str.Format("$f2%.2f", pBeamLsd.GetVu());
			else if(j==6) str.Format("$f5%.5f", pBeamLsd.GetpvMin());
			else if(j==7) str.Format("$f5%.5f", pBeamLsd.GetpvUse());
			else if(j==8) str.Format("$f2%.2f", pBeamLsd.GetSMax());
			else if(j==9) str.Format("$f2%.2f", pBeamLsd.GetSvCtc());
			else if(j==10) str = bShearOk ? _T("O.K"):_T("$#03N.G");
			else str = _T("");

			if(bMinAv == FALSE && (j>=6 && j<=9)) //ÃÖ¼ÒÀü´ÜÃ¶±Ù,Àü´ÜÃ¶±Ù°£°Ý
				str = _T("-");

			if (j == 1 && icValidEnvType > 1 && i % icValidEnvType == 0)
			{
				HGINT32 icMerge = 0;
				for (HGINT32 ix = 0; ix < icValidEnvType; ++ix)
				{
					icMerge += static_cast<HGINT32>(dItemRowArr.GetAt(i+ix+nCntItemSlab+nCntItemLoSlab));
				}

				XLText->AddMergeFormatText(icMerge - 1, nWidthData[j]-1, "$j$c%s", str);
			}
			else
				XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$j$e%s%s" : "$j$c%s%s", str, szBkColr); 
			XLText->AddFormatText("$m+%02d", nWidthData[j]);
		}
		XLText->AddFormatText("$n$h");
	}
	// ±âÃÊ
	long nIndex(0);
	for(i=0; i<nCntJijum; i++)
	{	
		if(pBri->IsFootingJijum(i) == FALSE)
			continue;

		HGBOOL bTitle = FALSE;

		for(long ixFront=0; ixFront<2; ++ixFront)
		{
			for(long ixUpper=1; ixUpper >= 0; --ixUpper)
			{
				HGBOOL bSubTitle = FALSE;

				for(HGINT32 ixEnvType = 0; ixEnvType < eEnvService; ++ixEnvType)
				{
					if(pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(ixEnvType)) == FALSE) continue;

					if(ixEnvType == eEnvLimit) strEnv = "±ØÇÑ";
					else if( ixEnvType == eEnvExtremeI ) strEnv = "±Ø´ÜI";
					else strEnv = "±Ø´Ü¥±";

					pConc = pBri->GetConcDataFooting(i, ixFront==0, ixUpper==0, -1, static_cast<ETypeEnvComboLsd>(ixEnvType));
					if( pConc == NULL ) continue;
// 
// 					if(!ixFront==0 && pConc->m_sTitle.Left(8) == "Áß°£ÁöÁ¡") 
// 					{
// 						continue;
// 					}

					CString strSub = (ixUpper==1)? _T("-ÇÏ¸é") : _T("-»ó¸é");

					szCom = pConc->m_sTitle.Left(6);
					if(szPre != szCom) bBgColr = !bBgColr;
					szPre = szCom;

					CLsdBeam pBeamLsd(pConc, strEnv == "±ØÇÑ" ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN);
					
					const HGBOOL bNeedShear = pBeamLsd.IsNeedShearRebar();
					const HGBOOL bShearOk = (bNeedShear && (!pBeamLsd.IsOk_SvCtc() || !pBeamLsd.IsOk_Vsd() || !pBeamLsd.IsOk_pvMin()))? FALSE : TRUE;
					const HGBOOL bMinAv = (bNeedShear || pBeamLsd.m_pData->m_bMinAvCheck);

					for(long j=0; j<11; j++)
					{
						nMerge = 0;
						szBkColr = (j > 2 && bBgColr)? _T("$b040"): _T("");

						if(bSkipFirstPart && j==0) continue;
						else if(j==0)
						{
							if( i==0 ) str = "½Ã\nÁ¡\nÃø\n±â\nÃÊ";
							else if( i==nCntItemFoot-1 ) str = "Á¾\nÁ¡\nÃø\n±â\nÃÊ";
							else str.Format("Áß\n°£\nÁö\nÁ¡\n±â\nÃÊ (%d)", i);

							if( bTitle == FALSE )
								nMerge = static_cast<long>(dItemFootingTitleMerge.GetAt(nIndex)) - 1;
						}
						else if(j==1) 
						{
							CString strTemp1 = ixFront==0 ? "¾Õ±Á" : "µÞ±Á";
							CString strTemp2 = ixUpper==0 ? "(»ó¸é)" : "(ÇÏ¸é)";

							str = strTemp1 + strTemp2;

							if( bSubTitle == FALSE )
								nMerge = static_cast<long>(dItemRowArr.GetAt(nCntItemSlab+nCntItemLoSlab+nCntItemWall+nCntItemArchRib+nIndex+ixFront+ixUpper+ixEnvType));
							bSubTitle = TRUE;
						}
						else if(j==2) str = strEnv;
						else if(j==3) str.Format("$f2%.2f", pBeamLsd.GetVcd());
						else if(j==4)
						{
							if(bNeedShear)
								str.Format("$f2%.2f", pBeamLsd.GetVd());
							else
								str = _T("-");
						}
						else if(j==5) str.Format("$f2%.2f", pBeamLsd.GetVu());
						else if(j==6) str.Format("$f5%.5f", pBeamLsd.GetpvMin());
						else if(j==7) str.Format("$f5%.5f", pBeamLsd.GetpvUse());
						else if(j==8) str.Format("$f2%.2f", pBeamLsd.GetSMax());
						else if(j==9) str.Format("$f2%.2f", pBeamLsd.GetSvCtc());
						else if(j==10) str = bShearOk ? _T("O.K"):_T("$#03N.G");
						else str = _T("");

						if(bMinAv == FALSE && (j>=6 && j<=9)) //ÃÖ¼ÒÀü´ÜÃ¶±Ù,Àü´ÜÃ¶±Ù°£°Ý
							str = _T("-");

						XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e%s%s" : "$c%s%s", str, szBkColr); 
						XLText->AddFormatText("$m+%02d", nWidthData[j]);
						bTitle = TRUE;
					}
					XLText->AddFormatText("$n$h");
				}
			}
		}
		nIndex++;
	}

	// ³¯°³º®
	BOOL bFirst = TRUE;
	strEnv = "±ØÇÑ";
	long k=0; for(k=0; k<4; k++)
	{
		if(!pBri->IsWingWall(k==0||k==1, k==0||k==2)) continue;
		CARcBridgeCalcWing *pCalcWing = m_pStd->m_pWing[k];

		for(i=0; i<5; i++)
		{
			if(k==0) pConc = pBri->m_pArrConcWingSttLeft.GetAt(i);
			else if(k==1) pConc = pBri->m_pArrConcWingSttRight.GetAt(i);
			else if(k==2) pConc = pBri->m_pArrConcWingEndLeft.GetAt(i);
			else if(k==3) pConc = pBri->m_pArrConcWingEndRight.GetAt(i);

			if(pConc == NULL) continue;
			if(pCalcWing->m_bIsA == FALSE && i < 3) continue;
			if(pCalcWing->m_bIsD == FALSE && i == 3) continue;
			if(pCalcWing->m_bIsA_ == FALSE && i == 4) continue;

			szCom = pConc->m_sTitle.Left(6);
			if(szPre != szCom) bBgColr = !bBgColr;
			szPre = szCom;

			CLsdBeam pBeamLsd(pConc, LSD_ULS_DESIGN);
			
			const HGBOOL bNeedShear = pBeamLsd.IsNeedShearRebar();
			const HGBOOL bShearOk = (bNeedShear && (!pBeamLsd.IsOk_SvCtc() || !pBeamLsd.IsOk_Vsd() || !pBeamLsd.IsOk_pvMin()))? FALSE : TRUE;
			const HGBOOL bMinAv = (bNeedShear || pBeamLsd.m_pData->m_bMinAvCheck);

			for(long j=0; j<11; j++)
			{
				nMerge = (bFirst)? nCntItemWing-1:0;
				bFirst = FALSE;
				szBkColr = (j > 2 && bBgColr)? _T("$b040"): _T("");

				if(bSkipFirstPart && j==0) continue;
				else if(j==0) str = "³¯\n°³\nº®";
				else if(j==1) str = pConc->m_sTitle;
				else if(j==2) str = strEnv;
				else if(j==3) str.Format("$f2%.2f", pBeamLsd.GetVcd());
				else if(j==4)
				{
					if(bNeedShear)
						str.Format("$f2%.2f", pBeamLsd.GetVd());
					else
						str = _T("-");
				}
				else if(j==5) str.Format("$f2%.2f", pBeamLsd.GetVu());
				else if(j==6) str.Format("$f5%.5f", pBeamLsd.GetpvMin());
				else if(j==7) str.Format("$f5%.5f", pBeamLsd.GetpvUse());
				else if(j==8) str.Format("$f2%.2f", pBeamLsd.GetSMax());
				else if(j==9) str.Format("$f2%.2f", pBeamLsd.GetSvCtc());
				else if(j==10) str = bShearOk ? _T("O.K"):_T("$#03N.G");
				else str = _T("");
				
				if(bMinAv == FALSE && (j>=6 && j<=9)) //ÃÖ¼ÒÀü´ÜÃ¶±Ù,Àü´ÜÃ¶±Ù°£°Ý
					str = _T("-");

				XLText->AddMergeFormatText(nMerge, nWidthData[j]-1, j == 1 ? "$e %s%s" : "$c%s%s", str, szBkColr); 
				XLText->AddFormatText("$m+%02d", nWidthData[j]);
			}
			XLText->AddFormatText("$n$h");
		}
	}

	*XLText += 1;
}

void CARcBridgeCalcSummary::MakeTableCheckInSideWall(CXLFormatText *XLText)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	if(pBri->m_nSelectInWallSectionDesign == 0) return;
	long nCountInWall = pBri->GetCountInWallReal();
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		long nJijumConnect = pBri->GetJijumConnectRibAndSlab();
		if(nJijumConnect != -1) nCountInWall--;
	}
	if(nCountInWall == 0) return;

	CString str = _T("");
	CString szPre = _T("");
	CString szCom = _T("");
	CString szBkColr = _T("");

	long nCountCase = (pBri->GetTypeDesingEQ() == 1)? 4 : 3;
	long nCntRow = nCountInWall*nCountCase + 2;
	str.Format("$s+%02d", nCntRow);
	XLText->AddFormatText("%s$h$e¡á Áß°£º®Ã¼ °ËÅä$n$h", str);
	XLText->AddTable(nCntRow-1, 25-1, 2, 4);

	long nWidthTitle1[] = { 4, 8, 13 };
	long nWidthTitle2[] = { 2, 2, 2, 2, 2, 2, 3, 3, 3, 2, 2 };
	char *szTitle1[] = { "±¸  ºÐ", "Ã¶ ±Ù ºñ", "Ãà¹æÇâ·Â, ÈÚ" };
	char *szTitle2[] = { "Áö Á¡", "ÇÏ Áß", "Pmin", "Puse", "Pmax", "ºñ °í", "¥õPn", "Pu", "Mu", "¾ÈÀüµµ", "ºñ °í" };
	long i=0; for(i=0; i<3; i++)
	{
		XLText->AddMergeFormatText(0, nWidthTitle1[i]-1, "$c%s", szTitle1[i]);
		XLText->AddFormatText("$m+%02d", nWidthTitle1[i]);
	}
	XLText->AddFormatText("$n$h");
	for(i=0; i<11; i++)
	{
		XLText->AddMergeFormatText(0, nWidthTitle2[i]-1, "$c%s", szTitle2[i]);
		XLText->AddFormatText("$m+%02d", nWidthTitle2[i]);
	}
	XLText->AddFormatText("$n$h");

	long nSizeJijum = pBri->GetCountJijum(); 
	for(i=0; i<nSizeJijum; i++)
	{
		if(i==0 || i==nSizeJijum-1) continue;
		CGirderAndColumn *pCalCol = m_pStd->m_pColGir->m_pList.GetAt(i);

		CString szJ = _T("");
		szJ.Format("ÁöÁ¡%d\n%s", i, pCalCol->m_bWall? "(º®Ã¼)":"(±âµÕ)");

		for(long nC=0; nC<nCountCase; nC++)
		{
			if(pCalCol->m_nTypeDesign[nC] < 0)
				continue;

			CConcBasic *pConc = pCalCol->pConcColumn[nC];
			CColumn ColCalc(pConc);
			ColCalc.CalculateAll(FALSE);
			BOOL bWall = pConc->IsWall();

			long j=0; for(j=0; j<11; j++)
			{
				long nMerge = (nC==0 && j==0)? nCountCase-1:0;
				
				if(j==0) str = szJ;
				else if(j==1)
				{
					if(pCalCol->m_nTypeDesign[nC]==COMBO_ULTIMATE)	 str.Format("UL%02d",pCalCol->m_nCombo[nC]+1);
					else if(pCalCol->m_nTypeDesign[nC]==COMBO_ECCENTRIC)	 str.Format("UE%02d",pCalCol->m_nCombo[nC]+1);
					else if(pCalCol->m_nTypeDesign[nC]==COMBO_EARTHQUAKE)	 str.Format("EQ%02d",pCalCol->m_nCombo[nC]+1);
					else if(pCalCol->m_nTypeDesign[nC]==COMBO_EARTHQUAKE2) str.Format("EQ%02d",pCalCol->m_nCombo[nC]+1);
				}
				else if(j==2) str.Format(bWall ? "%.4f" : "%.3f",ColCalc.GetValuePmin());
				else if(j==3) str.Format("%.3f",ColCalc.GetRebarUsedRate());
				else if(j==4) str.Format("%.3f",MaxRebar_Column);
				else if(j==5) str.Format("%s",ColCalc.GetCheckRebarRate()==1 ? "O.K" : "$#03N.G");
				else if(j==6) str.Format("%.3f",GetValueUnitChange(ColCalc.GetPiPn(),UNIT_CGS_TONF,pConc->m_szTonf));
				else if(j==7) str.Format("%.3f",GetValueUnitChange(pConc->m_Axial,UNIT_CGS_TONF,pConc->m_szTonf));
				else if(j==8) str.Format("%.3f",GetValueUnitChange(pConc->m_Moment,UNIT_CGS_TONFM,pConc->m_szTonfM));
				else if(j==9) str.Format("%.3f",ColCalc.GetSafeFactorAxial());
				else if(j==10) str.Format("%s",ColCalc.IsOkAxial() ? "O.K" : "$#03N.G");
				else str = _T("");
				
				XLText->AddMergeFormatText(nMerge, nWidthTitle2[j]-1, "$c%s%s", str, szBkColr); 
				XLText->AddFormatText("$m+%02d", nWidthTitle2[j]);
			}
			XLText->AddFormatText("$n$h");
		}
	}
}

void CARcBridgeCalcSummary::MakeTableCheckInSideWall_Lsd(CXLFormatText *XLText)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	if(pBri->m_nSelectInWallSectionDesign == 0) return;
	long nCountInWall = pBri->GetCountInWallReal();
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		long nJijumConnect = pBri->GetJijumConnectRibAndSlab();
		if(nJijumConnect != -1) nCountInWall--;
	}
	if(nCountInWall == 0) return;

	CString str = _T("");
	CString szPre = _T("");
	CString szCom = _T("");
	CString szBkColr = _T("");

	long nSizeJijum = pBri->GetCountJijum(); 
	long nCntRow = 2;
	
	CDoubleArray dArrItemMerge;
	long i=0; for(i=0; i<nSizeJijum; i++)		// ÁöÁ¡
	{
		long nCntItem(0);
		if(i == 0 || i == nSizeJijum-1)
		{
			if(pBri->m_nSelectWallSectionDesign == 0) continue;
		}
		else
		{
			if(pBri->m_nSelectInWallSectionDesign == 0) continue;
		}

		for(long ix=0; ix<eEnvService; ++ix)		// ±ØÇÑ, ±Ø´Ü...
		{
			ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
			if(pBri->IsValidComboEnvType(eEnvType) == FALSE)
				continue;

			for(long iCheck=0; iCheck<eCountFootCheck; ++iCheck)		// ÇÏÁßº°
			{
				// CLoadFooting *pLoadFooting = m_pStd->m_pFooting->GetLoadFootingLsd(i, eEnvType, iCheck);
				// if (pLoadFooting == NULL) continue;

				nCntItem++;
				nCntRow++;
			}
		}

		dArrItemMerge.Add(nCntItem);
	}

	//////////////////////////////////////////////////////////////////////////

	str.Format("$s+%02d", nCntRow);
	XLText->AddFormatText("%s$h$e¡á º®Ã¼ °ËÅä$n$h", str);
	XLText->AddTable(nCntRow-1, 25-1, 2, 4);

	long nWidthTitle1[] = { 5, 6, 14 };
	long nWidthTitle2[] = { 5, 2, 2, 2, 3, 3, 4, 2, 2 };
	long nWidthTitle3[] = { 2, 1, 2, 2, 2, 2, 3, 3, 4, 2, 2 };

	char *szTitle1[] = { "±¸  ºÐ", "Ã¶ ±Ù ºñ", "Ãà¹æÇâ·Â, ÈÚ" };
	char *szTitle2[] = { "±¸  ºÐ", "As,min", "As,use", "As,max", "¥õPn", "Pu", "Mu", "¾ÈÀüµµ", "ºñ °í" };
	for(i=0; i<3; i++)
	{
		XLText->AddMergeFormatText(i==0 ? 1 : 0, nWidthTitle1[i]-1, "$c%s", szTitle1[i]);
		XLText->AddFormatText("$m+%02d", nWidthTitle1[i]);
	}
	XLText->AddFormatText("$n$h");
	for(i=0; i<9; i++)
	{
		XLText->AddMergeFormatText(0, nWidthTitle2[i]-1, "$c%s", szTitle2[i]);
		XLText->AddFormatText("$m+%02d", nWidthTitle2[i]);
	}
	XLText->AddFormatText("$n$h");

	//////////////////////////////////////////////////////////////////////////

	CConcBasic *pConc = NULL;
	EPartOfBridge ePart;
	long iIndex = 0;
	for(i=0; i<nSizeJijum; i++)		// ÁöÁ¡
	{
		CGirderAndColumn *pCalCol = m_pStd->m_pColGir->m_pList.GetAt(i);

		CString szJ = _T("");
		if( i==0 || i == nSizeJijum-1 )
		{
			szJ = i==0 ? "½ÃÁ¡" : "Á¾Á¡";
			ePart = (i == 0) ? ePartFootStt : ePartFootEnd;

			if(pBri->m_nSelectWallSectionDesign == 0) 
				continue;
		}
		else
		{
			szJ.Format("ÁöÁ¡%d\n%s", i, pCalCol->m_bWall? "(º®Ã¼)":"(±âµÕ)");
			ePart = ePartInFoot;

			if(pBri->m_nSelectInWallSectionDesign == 0) 
				continue;
		}

		for(long ix=0; ix<eEnvService; ++ix)		// ±ØÇÑ, ±Ø´Ü...
		{
			ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
			if(pBri->IsValidComboEnvType(eEnvType) == FALSE)
				continue;

			ELSDCombKind eKindCombo = pBri->GetComboKindByEnvType(eEnvType, ePart, CHECK_POS_CENTER);

			for(long iCheck=0; iCheck<eCountFootCheck; ++iCheck)		// ÇÏÁßº°
			{
				CString szCombo(_T(""));
				if(pBri->IsBoxType() == FALSE)
				{
					CLoadFooting *pLoadFooting = m_pStd->m_pFooting->GetLoadFootingLsd(i, eEnvType, iCheck);
					if (pLoadFooting)
						szCombo = pLoadFooting->m_sCombo;
				}
				else
				{
					ELSDCombKind eKindCombo = pBri->GetComboKindByEnvType(eEnvType, ePart, CHECK_POS_CENTER);
					double Moment(0), Shear(0), Axial(0);
					long nIdxModel(0);
					long nCombo = pBri->GetOutValueForWallDesing_Lsd(i,eEnvType, (EWallCheckLsd)iCheck,Moment,Shear,Axial, nIdxModel, FALSE);
					szCombo = pBri->GetStringComboEta_Lsd(eKindCombo, ePart, nCombo + 1, TRUE, nIdxModel);
				}

				pConc = pCalCol->pConcColumnLsd[ix][iCheck];

				CColumn ColCalc(pConc);
				ColCalc.CalculateAll(FALSE);

				BOOL bWall = pConc->IsWall();

				// Data
				long j=0; for (j=0; j<11; ++j)
				{
					long nMerge = 0;

					if( j==0 )
					{
						str = szJ;
						nMerge = (ix==0 && iCheck==0) ? static_cast<long>(dArrItemMerge.GetAt(iIndex)) - 1 : 0;
					}
					else if( j== 1 ) 
					{
						if( ix == eEnvLimit) str = "±Ø\nÇÑ";
						else if( ix == eEnvExtremeI ) str = "±Ø\n´Ü\nI";
						else str = "±Ø\n´Ü\n¥±";

						nMerge = iCheck==0 ? 4 : 0;
					}
					else if( j== 2 ) str = szCombo;
					else if( j== 3 ) str.Format(bWall ? "$f4%.4f" : "$f3%.3f",ColCalc.GetValuePmin());
					else if( j== 4 ) str.Format("$f3%.3f",ColCalc.GetRebarUsedRate());
					else if( j== 5 ) str.Format("$f3%.3f",MaxRebar_Column);
					else if( j== 6 ) str.Format("%s", pData->GetStrDoubleUnitChange(ColCalc.GetPiPn(),UNIT_CGS_TONF,pConc->m_szTonf));
					else if( j== 7 ) str.Format("%s", pData->GetStrDoubleUnitChange(pConc->m_Axial,UNIT_CGS_TONF,pConc->m_szTonf));
					else if( j== 8 ) str.Format("%s", pData->GetStrDoubleUnitChange(pConc->m_Moment,UNIT_CGS_TONFM,pConc->m_szTonfM));
					else if( j== 9 ) str.Format("$f3%.3f",ColCalc.GetSafeFactorAxial());
					else if( j== 10 ) str.Format("%s",ColCalc.IsOkAxial() ? "O.K" : "$#03N.G");
					else str = _T("");

					XLText->AddMergeFormatText(nMerge, nWidthTitle3[j]-1, "$c%s%s", str, szBkColr); 
					XLText->AddFormatText("$m+%02d", nWidthTitle3[j]);
				}
				XLText->AddFormatText("$n$h");
			}
		}
		++iIndex;
	}
}

void CARcBridgeCalcSummary::DrawSectionMainRebar(CXLFormatText *XLText, long nCycle)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CDomyun *pDom = new CDomyun;

	pDom->SetScaleDim(150);
	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, pData);
	
	BOOL bVertDir	= pBri->IsVertDir();
	
	//pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, bVertDir?-1:1);
	pBri->SyncBridge(0, !bVertDir, FALSE);
	pBri->SyncBridgeRebar(FALSE, 0, TRUE);
	
	// Á¾´Ü¸é ±×¸²
	DBStd.m_pDrawVertSection->DrawVertSectionAll(pDom, FALSE, FALSE, FALSE, FALSE);

	m_pStd->SetDomyunScale(pDom, pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB? 35 : 38);

	BOOL bMarkUpperSlab	= TRUE;
	BOOL bMarkOutWall	= TRUE;
	BOOL bMarkInWall		= TRUE;
	BOOL bMarkFooting	= TRUE;
	BOOL bPageAll = (pBri->m_nQtyJigan > 1)? TRUE:FALSE;

	pDom->SetDimExo(1);
	// Ã¶±Ù ±×¸®±â(Ä¡¼ö, ÀÌÀ½ Æ÷ÇÔ)
	long nCountWall		= pBri->GetCountInWall();
	long nCountFooting	= pBri->GetCountFooting();

	for(long dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		
		DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_UpperSlab(pDom, nCycle, dan, TRUE, FALSE, bMarkUpperSlab);
		if(pBri->IsBoxType())
			DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_LowerSlab(pDom, nCycle, dan, TRUE, FALSE, bMarkUpperSlab);
		DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_OutWall(pDom, nCycle, dan, TRUE, FALSE, bMarkOutWall);
		
		// ¿ÜÃøº®Ã¼ ÀÔ·Â½Ã »óºÎ½½·¡ºê¿Í ¿ÜÃøº®Ã¼ ¿¬°áµÈ Ã¶±Ù ¸¶Å©ÇØÁà¾ß ÇÔ
		if(!bMarkUpperSlab && bMarkOutWall)
		{
			CRebarInfoCycle *pRC	= pBri->GetRebarInfoCycleUpperSlab(nCycle, 0, TRUE, 0);
			if(pRC)
				DBStd.MarkRebarInfoCycle(pDom, pRC, TRUE, FALSE, TRUE, 2, 0, CDRect(0, 0, 0, 0), 0.5, FALSE, &pBri->m_pArrRebarMain);
			
			pRC	= pBri->GetRebarInfoCycleUpperSlab(nCycle, 0, TRUE, -1);
			if(pRC)
				DBStd.MarkRebarInfoCycle(pDom, pRC, TRUE, FALSE, TRUE, 0, 0, CDRect(0, 0, 0, 0), 0.5, FALSE, &pBri->m_pArrRebarMain);
			
		}
		
		// º®Ã¼µé Ã¶±Ù ±×¸®±â
		for(long wall = 0; wall < nCountWall; wall++)
		{
			CWallApp *pWall	= pBri->GetInWall(wall);
			if(pWall)
			{
				for(long left = 0; left < 2; left++)
				{
					DBStd.m_pDrawWall->DrawRebarMain_Front(pDom, pWall, nCycle, dan, left, bMarkInWall, FALSE);
				}
			}
		}
		
		
		// °Å´õ Ã¶±Ù
		for(long nJ = 0; nJ < pBri->GetCountJijum(); nJ++)
		{
			DBStd.m_pDrawVertSection->DrawRebarAll_SlabGirder(pDom, nJ, FALSE, TRUE, 0, FALSE, TRUE);
		}
		
		// ±âÃÊµé ±×¸®±â
		
		for(long footing = 0; footing < nCountFooting; footing++)
		{
			CFootingApp *pFooting	= pBri->GetFooting(footing);
			if(!pFooting) continue;
			
			for(long upper = 0; upper < 2; upper++)
			{
				DBStd.m_pDrawFooting->DrawRebarMain_Front(pDom, pFooting, nCycle, dan, upper, FALSE, bMarkFooting);	
			}
		}

	}

	*XLText += 1;
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB)
	{
		pDom->Rotate(ConstPi/2*3,FALSE);
		if(nCycle==0) XLText->AddFormatText("$h$m+04$n");
		else XLText->AddFormatText("$u$u$u$u$h$m+15$n");
		XLText->AddFormatText("$tx11$e%sCTC%.0f$n$n$h", GetNumberString(nCycle+1,NUM_CIRCLE), pBri->m_dCTC_MainRebar);
		CDRect rc = pDom->GetExtRect();
		long nRowDom = 35;
		if(nCycle==0) XLText->AddFormatText("$h$m+04");
		else XLText->AddFormatText("$h$m+15");		

		XLText->InsertDomyun(pDom,m_pStd->m_pView,nRowDom,CXLFormatText:: INSERT_DOM_CURPOS&~CXLFormatText::INSERT_DOM_MOVEROW);
	}
	else
	{
		XLText->AddFormatText("$h$m+10$tx11$e%sCTC%.0f$n$n$h", GetNumberString(nCycle+1,NUM_CIRCLE), pBri->m_dCTC_MainRebar);
		CDRect rc = pDom->GetExtRect();
		long nRowDom = (long)MIN(22, 32*(rc.Height()/rc.Width()));

		if(bPageAll) 
		{
			pDom->Rotate(rc.BottomLeft(), CDPoint(0,-1));
			nRowDom = 38;
		}

		XLText->InsertDomyun(pDom,m_pStd->m_pView,nRowDom,CXLFormatText::INSERT_DOM_CENTER | CXLFormatText::INSERT_DOM_MOVEROW);
		*XLText += 1;

		if(bPageAll && nCycle == 0) XLText->AddFormatText("$s+40");
	}

	// Ã³À½¿¡  GetTvVertSection_StdÇß´Ù°¡ ´Ù½Ã ¸·¾ÒÀ¸¹Ç·Î º¹±¸µµ ÇÏÁö ¸»ÀÚ.
	//pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, bVertDir?-1:1);
	pBri->SyncBridge();
}

void CARcBridgeCalcSummary::DrawWingWallMainRebar(CXLFormatText *XLText, BOOL bStt, BOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	CDomyun *pDom = new CDomyun;
	
	pDom->SetScaleDim(100);
	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, pData);

	long nId = 0;
	if(bStt && bLeft) nId = 0;
	else if(bStt && !bLeft) nId = 1;
	else if(!bStt && bLeft) nId = 2;
	else if(!bStt && !bLeft) nId = 3;

	CARcBridgeCalcWing *pCalcWing = m_pStd->m_pWing[nId];
	CWingWall *pWing = pBri->GetWingWall(bStt, bLeft);
	CFootingApp *pFoot = (bStt)? pBri->GetFootingByJijumNumber(0):pBri->GetFootingByJijumNumber(pBri->GetCountJijum()-1);
	CTwinVectorArray tvArr;
	CTwinVector tv;
	
	long iDir = (bStt)? 1:-1;
	CDPoint xyTemp(0, 0);
	int se = bStt ? -1 : 1;
	int nLeft = bLeft ? -se : se;	// ½ÃÁ¡ÀÏ °æ¿ì ±³·® ÁøÇà ¹æÇâÀÇ ÁÂ¿ì¿Í ³¯°³º®ÀÇ ÁÂ¿ì°¡ ¹Ý´ëÀÓ.

	pBri->SyncBridge(nLeft, TRUE);

	m_pStd->m_pARoadOptionStd->SetEnvType(pDom, HCAD_CONC);
	DBStd.m_pDrawWingWall->DrawGenFront(pDom, bStt, bLeft, 1, FALSE, FALSE, FALSE);
	DBStd.m_pDrawWingWall->DrawGenCutBriSect(pDom, bStt, bLeft);

	m_pStd->m_pARoadOptionStd->SetEnvType(pDom, HCAD_DIML);

	pBri->GetTvWingWallFront(bStt, bLeft, 1, tvArr, TRUE, FALSE, TRUE, FALSE);
	
	long i = 0;
	CString str = _T("");
	CDPoint xyP1(0, 0);
	CDPoint xyP2(0, 0);
	CDPoint xyP3(0, 0);
	CDPoint xyP4(0, 0);
	CDPoint xyP5(0, 0);
	//double dWidthOutWall = (bStt)? pBri->m_dWS:pBri->m_dWE;
	double dHightFoot = (pFoot)? pFoot->GetHeight():0;
	if(pBri->IsBoxType()) dHightFoot = 0;
	double dCtc = 0;
	double dY = 0;
	double dX = 0;
	
	dCtc = pWing->m_dMainCTCA/2;
	double dBCLength = pBri->GetLengthWingWallBlockBC(bStt, bLeft);

	tv = tvArr.GetTvByInfo("³¯°³º®Á¤¸é»ó´Ü");
	//xyP1 = tv.m_v1+CDPoint(dWidthOutWall*iDir, 0);
	xyP1 = tv.m_v1;
	xyP2 = tv.m_v2;
	tv = tvArr.GetTvByInfo("³¯°³º®Á¤¸é³¡´ÜÇÏºÎ");
	//xyP3 = tv.m_v2 + CDPoint(0, pBri->GetLengthWingWallBlockBC(bStt, bLeft));
	//xyP4 = tv.m_v2-CDPoint(0, dHightFoot);
	xyP3 = tv.m_v1;
	xyP4 = tv.m_v2-CDPoint(0, dHightFoot);
	dY = (xyP3.y+xyP4.y)/2+pWing->m_dMainCTCB/2;
	tv = tvArr.GetTvByInfo("³¯°³º®Á¤¸éÇÏ´Ü");
	xyP5 = tv.m_v2;
	dX = fabs(tv.m_v2.x+tv.m_v1.x)/2;/*-pWing->m_dStdVerCTC/2*iDir*/;

	//////////////////////////////////////////////////////////////////////////
	double dCtcAreaA=0,dCtcAreaB=0,dCtcAreaC=0,dCtcAreaD=0;
	double nEa1Cycle=0,nEa2Cycle=0;
	long nDan=0;
	CConcBasic *pBasic;
	pBasic = pBri->GetConcDataWingWall(bStt,bLeft,AREA_A);
	if(pBasic->IsValidData()) 
	{
		// ¼Ò¼öÁ¡À» Àß¶ó³»¸é ½ÇÁ¦ ¹è±ÙµÈ °£°Ý°ú ´Ù¸£°Ô ³ª¿Â´Ù.
//		nEa1Cycle = (long)pBasic->m_RbT1C_B[nDan];
//		nEa2Cycle = (long)pBasic->m_RbT2C_B[nDan];
		nEa1Cycle = pBasic->m_RbT1C_B[nDan];
		nEa2Cycle = pBasic->m_RbT2C_B[nDan];
		dCtcAreaA = (1000.0)/(double)(nEa1Cycle+nEa2Cycle);
	}
	pBasic = pBri->GetConcDataWingWall(bStt,bLeft,AREA_B);
	if(pBasic->IsValidData()) 
	{
//		nEa1Cycle = (long)pBasic->m_RbT1C_B[nDan];
//		nEa2Cycle = (long)pBasic->m_RbT2C_B[nDan];
		nEa1Cycle = pBasic->m_RbT1C_B[nDan];
		nEa2Cycle = pBasic->m_RbT2C_B[nDan];
		dCtcAreaB = (1000.0)/(double)(nEa1Cycle+nEa2Cycle);
	}
	pBasic = pBri->GetConcDataWingWall(bStt,bLeft,AREA_C);
	if(pBasic->IsValidData()) 
	{
//		nEa1Cycle = (long)pBasic->m_RbT1C_B[nDan];
//		nEa2Cycle = (long)pBasic->m_RbT2C_B[nDan];
		nEa1Cycle = pBasic->m_RbT1C_B[nDan];
		nEa2Cycle = pBasic->m_RbT2C_B[nDan];
		dCtcAreaC = (1000.0)/(double)(nEa1Cycle+nEa2Cycle);
	}
	pBasic = pBri->GetConcDataWingWall(bStt,bLeft,AREA_D);
	if(pBasic->IsValidData()) 
	{
//		nEa1Cycle = (long)pBasic->m_RbT1C_B[nDan];
//		nEa2Cycle = (long)pBasic->m_RbT2C_B[nDan];
		nEa1Cycle = pBasic->m_RbT1C_B[nDan];
		nEa2Cycle = pBasic->m_RbT2C_B[nDan];
		dCtcAreaD = (1000.0)/(double)(nEa1Cycle+nEa2Cycle);
	}
	//////////////////////////////////////////////////////////////////////////

	double dFy = pBri->GetValueFy(bStt? ePartWingWallStt : ePartWingWallEnd);
	
	if(pCalcWing->m_bIsA)
	{
		if(pWing->m_bSelMainCTCD || pWing->m_dDiaMainA[0][nDan] != pWing->m_dDiaMainD[0][nDan])
		{
			for(i=0; i<3; i++)
			{
				pDom->MoveTo(xyP1.x-150*iDir, xyP1.y-(500+dCtc*i));
				pDom->LineTo(xyP3.x, xyP1.y-(500+dCtc*i));
				
				pDom->MoveTo(xyP3.x, xyP1.y-(500+dCtc*2*i));
				pDom->LineTo(xyP2.x+150*iDir, xyP1.y-(500+dCtc*2*i));
			}
			
			if(dCtcAreaD>0)
			{
				if(bStt) pDom->SetTextAlignHorz(TA_RIGHT);
				else pDom->SetTextAlignHorz(TA_LEFT);
				str.Format("%s-CTC %.0f", pData->GetStringDia(pWing->m_dDiaMainD[0][nDan], dFy), dCtcAreaD);
				pDom->TextOut(xyP3.x-200*iDir, xyP1.y-(800+dCtc*4), str);
			}
			if(bStt) pDom->SetTextAlignHorz(TA_LEFT);
			else pDom->SetTextAlignHorz(TA_RIGHT);
			str.Format("%s-CTC %.0f", pData->GetStringDia(pWing->m_dDiaMainA[0][nDan], dFy), dCtcAreaA);
			pDom->TextOut(xyP3.x+200*iDir, xyP1.y-(800+dCtc*2), str);
		}
		else
		{
			long i=0; for(i=0; i<3; i++)
			{
				pDom->MoveTo(xyP1.x-150*iDir, xyP1.y-(500+dCtc*i));
				pDom->LineTo(xyP2.x+150*iDir, xyP1.y-(500+dCtc*i));
			}
			if(bStt) pDom->SetTextAlignHorz(TA_CENTER);
			str.Format("%s-CTC %.0f", pData->GetStringDia(pWing->m_dDiaMainA[0][nDan], dFy), dCtcAreaA);
			pDom->TextOut((xyP1.x+xyP2.x)/2, xyP1.y-(600+dCtc*3), str);
		}
		
		//¼öÆò¼±
		dCtc = pWing->m_dMainCTCB/2;
		for(i=0; i<3; i++)
		{
			if(xyP4.y+dHightFoot > dY-dCtc*i) break;
			pDom->MoveTo(xyP3.x+dBCLength*iDir, dY-dCtc*i);
			pDom->LineTo(xyP3.x, dY-dCtc*i);
		}
		if(bStt) pDom->SetTextAlignHorz(TA_RIGHT);
		else pDom->SetTextAlignHorz(TA_LEFT);
		str.Format("%s-CTC %.0f", pData->GetStringDia(pWing->m_dDiaMainB[0][nDan], dFy), dCtcAreaB);
		pDom->TextOut(dX, dY+100, str);
		

		//¼öÁ÷¼±
		dCtc = pWing->m_dStdVerCTC/2;
		for(i=0; i<3; i++)
		{
			pDom->MoveTo(xyP4.x + (dBCLength/2 + dCtc*i)*iDir, xyP3.y);
			pDom->LineTo(xyP4.x + (dBCLength/2 + dCtc*i)*iDir, xyP4.y);
		}
		if(bStt) pDom->SetTextAlignHorz(TA_LEFT);
		else pDom->SetTextAlignHorz(TA_RIGHT);
		str.Format("%s-CTC %.0f", pData->GetStringDia(pWing->m_dDiaSupportB[0][nDan], dFy), dCtcAreaC);
		pDom->TextOut(dX+dCtc*3*iDir, xyP4.y+400, str);
	}
	else
	{
		for(i=0; i<3; i++)
		{
			pDom->MoveTo(xyP3.x, xyP1.y-(500+dCtc*2*i));
			pDom->LineTo(xyP2.x+100*iDir, xyP1.y-(500+dCtc*2*i));
		}
		
		if(dCtcAreaD>0)
		{
			if(bStt) pDom->SetTextAlignHorz(TA_RIGHT);
			else pDom->SetTextAlignHorz(TA_LEFT);
			str.Format("%s-CTC %.0f", pData->GetStringDia(pWing->m_dDiaMainD[0][nDan], dFy), dCtcAreaD);
			pDom->TextOut(xyP3.x-200*iDir, xyP1.y-(800+dCtc*4), str);
		}
	}

	CDRect rc = pDom->GetExtRect();
	long nRowDom = (long)MIN(17, 32*(rc.Height()/rc.Width()));
	
	*XLText += 1;
	XLText->InsertDomyun(pDom,m_pStd->m_pView,nRowDom,CXLFormatText::INSERT_DOM_CENTER | CXLFormatText::INSERT_DOM_MOVEROW);
	*XLText += 1;
}

CString CARcBridgeCalcSummary::GetStringMainRebarUse(CConcBasic *pConc, long &nLine)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nCntItemRow = 0;
	CString str = _T("");
	CString strTemp = _T("");

// 	double dMainRebarCtc = 0;

	double dFy = pConc->m_Fy;

	int j=0; for(j=0; j<REBAR_MAX_LAYER; j++)
	{
		double dMainRebarCtc = frM(UNIT_METER)/MAX(pConc->m_RbT1C_B[j], pConc->m_RbT2C_B[j]);
		if(pConc->m_RbT1C_DIA[j] == 0 && pConc->m_RbT2C_DIA[j] == 0)
		{
			if(j == REBAR_MAX_LAYER-1 && nCntItemRow == 0)
			{
				strTemp = "";
			}
			continue;
		}
		else if(pConc->m_RbT1C_DIA[j] == pConc->m_RbT2C_DIA[j])
		{
			if(strTemp!=_T("")) strTemp += "\n";
			str.Format("%s@%.0f", pData->GetStringDia(pConc->m_RbT2C_DIA[j], dFy), dMainRebarCtc/2);
			strTemp += str;
			nCntItemRow++;
		}
		else
		{
			if(pConc->m_RbT1C_DIA[j] != 0 && pConc->m_RbT2C_DIA[j] != 0)
			{
				if(strTemp!=_T("")) strTemp += "\n";
				str.Format("%s,%s@%.0f", pData->GetStringDia(pConc->m_RbT1C_DIA[j], dFy), pData->GetStringDia(pConc->m_RbT2C_DIA[j], dFy), dMainRebarCtc);
				strTemp += str;
				nCntItemRow++;
			}
			else if(pConc->m_RbT1C_DIA[j] != 0)
			{
				if(strTemp!=_T("")) strTemp += "\n";
				str.Format("%s@%.0f", pData->GetStringDia(pConc->m_RbT1C_DIA[j], dFy), dMainRebarCtc);
				strTemp += str;
				nCntItemRow++;
			}
			else if(pConc->m_RbT2C_DIA[j] != 0)
			{
				if(strTemp!=_T("")) strTemp += "\n";
				str.Format("%s@%.0f", pData->GetStringDia(pConc->m_RbT2C_DIA[j], dFy), dMainRebarCtc);
				strTemp += str;
				nCntItemRow++;
			}
		}
	}
	
	nLine = MAX(nCntItemRow, 1);	
	return strTemp;
}

long CARcBridgeCalcSummary::GetRowCountAll(long nType)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CString str = _T("");
	long nCntItemSlab = pBri->m_pArrConcSlab[eEnvLimit].GetSize();
	long nCntItemWall = (pBri->IsOutsideWall())? MIN(10, pBri->m_pArrConcWall[eEnvLimit].GetSize()):0;
	long nCntItemInWall = (pBri->m_nSelectInWallSectionDesign == 0)? pBri->m_pArrConcInWall[eEnvLimit].GetSize() : 0;
//	long nCntFootRev = ((pBri->IsOutWall(TRUE))? 2:0) + ((pBri->IsOutWall(FALSE))? 2:0);
	long nCntItemFoot = pBri->m_pArrConcFoot[eEnvLimit].GetSize() + pBri->m_pArrConcFootRev[eEnvLimit].GetSize();
	long nCntItemArchRib = pBri->m_pArrConcArchRib.GetSize();
	long nCntItemPRF = pBri->m_pArrConcPRF[eEnvLimit].GetSize();
	long i=0; 
	long nCntRow = 0;
	//////////////////////////////////////////////////////////////////////////
	CConcBasic *pConc = NULL;

	long nLine = 0;

	// ½½·¡ºê
	if(nType==0 || nType ==1)
	{
		for(i=0; i<nCntItemSlab; i++)
		{
			pConc = pBri->m_pArrConcSlab[eEnvLimit].GetAt(i);
			if(pConc == NULL) continue;

			str = GetStringMainRebarUse(pConc, nLine);
			nCntRow += nLine;
		}
	}
	if(pBri->IsRcSlab() == FALSE)
	{
		// º®Ã¼
		if(nType==0 || nType ==1)
		{
			for(i=0; i<nCntItemWall; i++)
			{
				pConc = pBri->GetConcDataWall(i, TRUE);
				if(pConc == NULL) continue;
				
				str = GetStringMainRebarUse(pConc, nLine);
				nCntRow += nLine;
			}
			for(i=0; i<nCntItemInWall; i++)
			{
				pConc = pBri->m_pArrConcInWall[eEnvLimit].GetAt(i);
				if(pConc == NULL) continue;
				
				str = GetStringMainRebarUse(pConc, nLine);
				nCntRow += nLine;
			}
		}
		// ¾ÆÄ¡¸®ºê
		if(nType==0 || nType ==4)
		{
			for(i=0; i<nCntItemArchRib; i++)
			{	
				pConc = pBri->m_pArrConcArchRib.GetAt(i);
				
				str = GetStringMainRebarUse(pConc, nLine);
				nCntRow += nLine;
			}
		}
		// ±âÃÊ
		if(nType==0 || nType ==2)
		{
			BOOL bRev = FALSE;
			for(i=0; i<nCntItemFoot; i++)
			{	
				long nInd = i/2;
				if(!bRev) pConc = pBri->m_pArrConcFoot[eEnvLimit].GetAt(nInd);
				else pConc = pBri->m_pArrConcFootRev[eEnvLimit].GetAt(nInd);
				bRev = !bRev;
				if(pConc == NULL) continue;
				if(!bRev && pConc->m_sTitle.Left(4) == "ÁöÁ¡") continue;
				
				str = GetStringMainRebarUse(pConc, nLine);
				nCntRow += nLine;
			}
		}
		// ³¯°³º®
		if(nType==0 || nType ==3)
		{
			long k=0; for(k=0; k<4; k++)
			{
				if(!pBri->IsWingWall(k==0||k==1, k==0||k==2)) continue;
				
				for(i=0; i<5; i++)
				{
					if(k==0) pConc = pBri->m_pArrConcWingSttLeft.GetAt(i);
					else if(k==1) pConc = pBri->m_pArrConcWingSttRight.GetAt(i);
					else if(k==2) pConc = pBri->m_pArrConcWingEndLeft.GetAt(i);
					else if(k==3) pConc = pBri->m_pArrConcWingEndRight.GetAt(i);
					
					if(pConc == NULL) continue;
					else if(pConc->m_Moment == 0)
					{
						str = _T("");
					}
					else
					{
						str = GetStringMainRebarUse(pConc, nLine);
						nCntRow += nLine;
					}
				}
			}
		}

		if(nType==0 || nType==5)
		{
			for(i=0; i<nCntItemPRF; i++)
			{
				BOOL bStt = (i==0);
				pConc = pBri->GetConcDataPRF(bStt);
				if(pConc == NULL) continue;

				str = GetStringMainRebarUse(pConc, nLine);
				nCntRow += nLine;
			}
		}
	}

	return nCntRow;
}

