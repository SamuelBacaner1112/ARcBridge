// ARcBridgeCalcColAndGir.cpp: implementation of the CARcBridgeCalcColAndGir class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "ARcBridgeCalculate.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARcBridgeCalcColAndGir::CARcBridgeCalcColAndGir()
{

}

CARcBridgeCalcColAndGir::~CARcBridgeCalcColAndGir()
{
	AhTPADelete(&m_pList,(CGirderAndColumn*)0);
}

void CARcBridgeCalcColAndGir::CalculateAll()
{
	MakeDesignList();

	long nSize = m_pList.GetSize();
	if(nSize==0) return;

	SetConcData(TRUE);
	SetConcData(FALSE);
	SetConcDataJongGir();
}

void CARcBridgeCalcColAndGir::MakeDesignList()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	const HGBOOL bLsd = pData->IsLsdDesign();
	long nJijumConnect = pBri->GetJijumConnectRibAndSlab();

	for(long nJijum=0; nJijum<=pBri->m_nQtyJigan; nJijum++)
	{
		if(nJijum == nJijumConnect) continue;
		CJijum *pJijum = pBri->GetJijum(nJijum);
		
		long nVerPos  = (nJijum==0) ? 1 : 0;
		BOOL bColumn  = (pBri->GetTypeColumnByJijum(nJijum)==-1) ? FALSE : TRUE;
		BOOL bShoePos = (pJijum->m_nQtyShoe[nVerPos]>0) ? TRUE : FALSE;

		if((nJijum==0 || nJijum==pBri->m_nQtyJigan) && bShoePos==FALSE)
		{
			CGirderAndColumn *pGirCol = new CGirderAndColumn;
			pGirCol->m_nJijum	= nJijum;
			pGirCol->m_bGirder	= bColumn || bShoePos;
			pGirCol->m_bColumn	= bColumn;
			pGirCol->m_bWall	= (!bColumn && !bShoePos);

			pGirCol->m_dCtcColumn = bColumn ? pBri->GetCtcColumn(nJijum) : 1.00;

			m_pList.Add(pGirCol);

			continue;
		}

		double DistDanger = bShoePos ? 0 : GetDistDangerFromColumnCenter(nJijum);

		CGirderAndColumn *pGirCol = new CGirderAndColumn;
		pGirCol->m_nJijum	= nJijum;
		pGirCol->m_bGirder	= bColumn || bShoePos;
		pGirCol->m_bColumn	= bColumn;
		pGirCol->m_bWall	= (!bColumn && !bShoePos);

		if(pGirCol->m_bGirder)
		{
			for(long ixTypeEnv=0; ixTypeEnv<eCountEnvTypeLsd; ++ixTypeEnv)
			{
				pGirCol->m_dAxialForceTop[RCBRIDGE_GIR_POS_END][ixTypeEnv]		= GetMaxAxialForceOnGirder(nJijum,TRUE, (ETypeEnvComboLsd)ixTypeEnv);
				pGirCol->m_dAxialForceBot[RCBRIDGE_GIR_POS_END][ixTypeEnv]		= GetMaxAxialForceOnGirder(nJijum,FALSE, (ETypeEnvComboLsd)ixTypeEnv);
				pGirCol->m_dAxialForceTop[RCBRIDGE_GIR_POS_MID][ixTypeEnv]		= pGirCol->m_dAxialForceTop[RCBRIDGE_GIR_POS_END][ixTypeEnv];
				pGirCol->m_dAxialForceBot[RCBRIDGE_GIR_POS_MID][ixTypeEnv]		= pGirCol->m_dAxialForceBot[RCBRIDGE_GIR_POS_END][ixTypeEnv];
			}
			pGirCol->m_dSpanLengthGirder[RCBRIDGE_GIR_POS_END]	= GetLengthSpanGirder(nJijum,FALSE)  - DistDanger;
			pGirCol->m_dSpanLengthGirder[RCBRIDGE_GIR_POS_MID]	= GetLengthSpanGirder(nJijum,TRUE)   - DistDanger*2;
			if(bShoePos)
				pGirCol->m_dDivOutBeamMinus	= (pJijum->m_nQtyShoe[nVerPos]>=2) ? 10.0 : 9.0;
			else
				pGirCol->m_dDivOutBeamMinus	= (pBri->GetInWall(nJijum-1)->m_nCountColumn>=2) ? 10.0 : 9.0;
			pGirCol->m_szCalcLn[RCBRIDGE_GIR_POS_END]	= GetStringCalcLn(nJijum,FALSE);
			pGirCol->m_szCalcLn[RCBRIDGE_GIR_POS_MID]	= GetStringCalcLn(nJijum,TRUE);
		}

		if(bLsd)
		{
			// LSD일때는 한계상태별로 사용하기때문에 미리 집계해놓지 않고 필요할때 집계해서 사용하자.
			if((pGirCol->m_bColumn || pGirCol->m_bWall) && bShoePos==FALSE)
			{
				pGirCol->m_dCtcColumn = bColumn ? pBri->GetCtcColumn(nJijum) : 1.00;
			}
		}
		else
		{
			long nCheckSize = pBri->IsApplyEarthQuakeDesign() ? RCBRIDGE_COL_CHECK : RCBRIDGE_COL_CHECK - 1; // 지진시 미적용
			if((pGirCol->m_bColumn || pGirCol->m_bWall) && bShoePos==FALSE)
			{
				pGirCol->m_dCtcColumn = bColumn ? pBri->GetCtcColumn(nJijum) : 1.00;
				long i=0; for(i=0; i<nCheckSize; i++)
				{
					GetMomentAndAxialOnColumn(nJijum,i,pGirCol->m_dAxialBot[i],pGirCol->m_dMomentBot[i],pGirCol->m_dShearBot[i],pGirCol->m_nTypeDesign[i],pGirCol->m_nCombo[i]);
				}
			}
		}
		
		pGirCol->CalulateForce();
		m_pList.Add(pGirCol);
	}
}

// 내측지간에서 가장 큰 값을 사용한다...
double CARcBridgeCalcColAndGir::GetLengthSpanGirder(long nJijum, BOOL bInnerSpan)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	CJijum *pJijum = pBri->GetJijum(nJijum);

	long nVerPos  = (nJijum==0) ? 1 : 0;
	BOOL bShoePos = (pJijum->m_nQtyShoe[nVerPos]>0) ? TRUE : FALSE;

	double dLengthSpan = 0;

	if(bShoePos==FALSE)
	{
		CWallApp *pWall = m_pStd->m_pBri->GetInWall(nJijum-1);

		if(bInnerSpan)
		{
//			double dDistCol = 0.;
			long i=0; for(i=0; i<pWall->m_nCountColumn-1; i++)
			{
				double dDistCol = m_pStd->m_pBri->GetDistColumnNext(nJijum,i,FALSE);
				if(dDistCol>dLengthSpan)
				{
					dLengthSpan = dDistCol;
				}
			}
		}
		else
		{
			double dDistLeft  = m_pStd->m_pBri->GetDistColumnNext(nJijum,0,TRUE);
			double dDistRight = m_pStd->m_pBri->GetDistColumnNext(nJijum,pWall->m_nCountColumn-1,FALSE);
			dLengthSpan = max(dDistLeft,dDistRight);
		}

		return toM(dLengthSpan);
	}
	else
	{
		long nQtyShoe = pJijum->m_nQtyShoe[nVerPos];
		if(bInnerSpan)
		{
//			double dDistCol = 0.;
			long i=0; for(i=0; i<nQtyShoe-1; i++)
			{
				double dDistCol = pJijum->m_dDistShoe[nVerPos][i+1] - pJijum->m_dDistShoe[nVerPos][i];
				if(dDistCol>dLengthSpan)
				{
					dLengthSpan = dDistCol;
				}
			}
		}
		else
		{
			double dDistLeft  = pJijum->m_dDistShoe[nVerPos][0] - pBri->m_dWidthSlabLeft;
			double dDistRight = pBri->m_dWidthSlabRight - pJijum->m_dDistShoe[nVerPos][nQtyShoe-1];
			dLengthSpan = max(dDistLeft,dDistRight);
		}

		return toM(dLengthSpan);
	}
}

// 거더설계시는 기둥상하단의 최대축력을 가지고 사용함
// 슬래브교,파이형등 교량받침위치의 거더인 경우 절점위치의 전단력의 합으로 한다..
double CARcBridgeCalcColAndGir::GetMaxAxialForceOnGirder(long nJijum, double &Shear1, double &Shear2, BOOL bUpper, ETypeEnvComboLsd eEnvType)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	CJijum *pJijum = pBri->GetJijum(nJijum);

	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();
	long nVerPos  = (nJijum==0) ? 1 : 0;
	BOOL bShoePos = (pJijum->m_nQtyShoe[nVerPos]>0) ? TRUE : FALSE;

	if(bShoePos && bUpper==FALSE) return 0;

	if(bShoePos==FALSE) // 기둥부의 거더
	{
		long nPos = bUpper ? CHECK_POS_TOP : CHECK_POS_BOTTOM;
		BOOL bGirderCalc = TRUE;
		BOOL bOutSideWall= TRUE; // TRUE-배면,FALSE-전면

		double dValue(0);
		if(bLsd)
		{
			// (ARCBRIDGE-2692) 전체 조합에서 최대값을 찾고있다가 각 조합으로 분리.
			if(eEnvType == eEnvExtremeI || eEnvType == eEnvExtremeII)
			{
				double dValueEQ1 = pBri->GetOutValueWall(nJijum, nPos, COMBO_EARTHQUAKE, ELE_AXFORCE, bGirderCalc,bOutSideWall, eEnvType);
				double dValueEQ2 = pBri->GetOutValueWall(nJijum, nPos, COMBO_EARTHQUAKE2, ELE_AXFORCE, bGirderCalc,bOutSideWall, eEnvType);

				dValue = max(dValueEQ1, dValueEQ2);
			}
			else
			{
				dValue = pBri->GetOutValueWall(nJijum, nPos, COMBO_ULTIMATE, ELE_AXFORCE, bGirderCalc,bOutSideWall, eEnvType);
			}
		}
		else
		{
			double dAxialUlt = Round(pBri->GetOutValueWall(nJijum,nPos,COMBO_ULTIMATE   ,ELE_AXFORCE,bGirderCalc,bOutSideWall),3);
			double dAxialEQ1 = Round(pBri->GetOutValueWall(nJijum,nPos,COMBO_EARTHQUAKE ,ELE_AXFORCE,bGirderCalc,bOutSideWall),3);
			double dAxialEQ2 = Round(pBri->GetOutValueWall(nJijum,nPos,COMBO_EARTHQUAKE2,ELE_AXFORCE,bGirderCalc,bOutSideWall),3);

			dValue = max(dAxialUlt,max(dAxialUlt,max(dAxialEQ1,dAxialEQ2)));
		}
		
		// (ARCBRIDGE-2693) 거더 검토에서 부재력은 모델링의 단위폭당 부재력을 쓰기때문에 tonf가 아니라 tonf/m 단위를 쓴다.
		dValue = GetValueUnitChange(dValue,pBri->m_pARcBridgeDataStd->m_szTonf_M,UNIT_CGS_TONF_M);

		return dValue;
	}
	else // 교량받침의 거더 : 지점 양단의 전단력의 합으로 계산 (절대값)
	{
//		long nSpan1 = nJijum - 1;
//		long nSpan2 = nJijum;

		double dValue(0);
		if(bLsd)
		{
			double dShearCur[2]={0,0};
			dShearCur[0] = fabs(pBri->GetOutValueShearSlabOnJijum(nJijum,CHECK_POS_LEFT ,COMBO_ULTIMATE, TRUE, eEnvType));
			dShearCur[1] = fabs(pBri->GetOutValueShearSlabOnJijum(nJijum,CHECK_POS_RIGHT,COMBO_ULTIMATE, TRUE, eEnvType));
			dValue = max(dShearCur[0] + dShearCur[1], dValue);
			if(dValue == (dShearCur[0] + dShearCur[1]))
			{
				Shear1 = dShearCur[0];
				Shear2 = dShearCur[1];
			}
		}
		else
		{
			double dShearUlt[2]={0,0}, dShearEcc[2]={0,0},dShearEQ1[2]={0,0}, dShearEQ2[2]={0,0};

			//		if(nSpan1>=0 && nSpan1<=pBri->m_nQtyJigan)
			//		{
			//			dShearUlt[0] = fabs(Round(pBri->GetOutValueSlab(nSpan1,CHECK_POS_RIGHT,COMBO_ULTIMATE   ,ELE_SHEAR_2,TRUE,TRUE),3));
			//			dShearEcc[0] = fabs(Round(pBri->GetOutValueSlab(nSpan1,CHECK_POS_RIGHT,COMBO_ECCENTRIC  ,ELE_SHEAR_2,TRUE,TRUE),3));
			//			dShearEQ1[0] = fabs(Round(pBri->GetOutValueSlab(nSpan1,CHECK_POS_RIGHT,COMBO_EARTHQUAKE ,ELE_SHEAR_2,TRUE,TRUE),3));
			//			dShearEQ2[0] = fabs(Round(pBri->GetOutValueSlab(nSpan1,CHECK_POS_RIGHT,COMBO_EARTHQUAKE2,ELE_SHEAR_2,TRUE,TRUE),3));
			//		}
			//		if(nSpan2>=0 && nSpan2<pBri->m_nQtyJigan)
			//		{
			//			dShearUlt[1] = fabs(Round(pBri->GetOutValueSlab(nSpan2,CHECK_POS_LEFT,COMBO_ULTIMATE   ,ELE_SHEAR_2,TRUE,TRUE),3));
			//			dShearEcc[1] = fabs(Round(pBri->GetOutValueSlab(nSpan2,CHECK_POS_LEFT,COMBO_ECCENTRIC  ,ELE_SHEAR_2,TRUE,TRUE),3));
			//			dShearEQ1[1] = fabs(Round(pBri->GetOutValueSlab(nSpan2,CHECK_POS_LEFT,COMBO_EARTHQUAKE ,ELE_SHEAR_2,TRUE,TRUE),3));
			//			dShearEQ2[1] = fabs(Round(pBri->GetOutValueSlab(nSpan2,CHECK_POS_LEFT,COMBO_EARTHQUAKE2,ELE_SHEAR_2,TRUE,TRUE),3));
			//		}

			dShearUlt[0] = fabs(Round(pBri->GetOutValueShearSlabOnJijum(nJijum,CHECK_POS_LEFT ,COMBO_ULTIMATE),3));
			dShearEcc[0] = fabs(Round(pBri->GetOutValueShearSlabOnJijum(nJijum,CHECK_POS_LEFT ,COMBO_ECCENTRIC),3));
			dShearEQ1[0] = fabs(Round(pBri->GetOutValueShearSlabOnJijum(nJijum,CHECK_POS_LEFT ,COMBO_EARTHQUAKE),3));
			dShearEQ2[0] = fabs(Round(pBri->GetOutValueShearSlabOnJijum(nJijum,CHECK_POS_LEFT ,COMBO_EARTHQUAKE2),3));
			//
			dShearUlt[1] = fabs(Round(pBri->GetOutValueShearSlabOnJijum(nJijum,CHECK_POS_RIGHT,COMBO_ULTIMATE),3));
			dShearEcc[1] = fabs(Round(pBri->GetOutValueShearSlabOnJijum(nJijum,CHECK_POS_RIGHT,COMBO_ECCENTRIC),3));
			dShearEQ1[1] = fabs(Round(pBri->GetOutValueShearSlabOnJijum(nJijum,CHECK_POS_RIGHT,COMBO_EARTHQUAKE),3));
			dShearEQ2[1] = fabs(Round(pBri->GetOutValueShearSlabOnJijum(nJijum,CHECK_POS_RIGHT,COMBO_EARTHQUAKE2),3));

			double dAxialUlt = dShearUlt[0] + dShearUlt[1];
			double dAxialEcc = dShearEcc[0] + dShearEcc[1];
			double dAxialEQ1 = dShearEQ1[0] + dShearEQ1[1];
			double dAxialEQ2 = dShearEQ2[0] + dShearEQ2[1];

			dValue = max(dAxialUlt,max(dAxialUlt,max(dAxialEQ1,dAxialEQ2)));
			if(dAxialUlt==dValue) { Shear1=dShearUlt[0]; Shear2=dShearUlt[1]; }
			if(dAxialEcc==dValue) { Shear1=dShearEcc[0]; Shear2=dShearEcc[1]; }
			if(dAxialEQ1==dValue) { Shear1=dShearEQ1[0]; Shear2=dShearEQ1[1]; }
			if(dAxialEQ2==dValue) { Shear1=dShearEQ2[0]; Shear2=dShearEQ2[1]; }
		}

		// (ARCBRIDGE-2693) 거더 검토에서 부재력은 모델링의 단위폭당 부재력을 쓰기때문에 tonf가 아니라 tonf/m 단위를 쓴다.
		dValue = GetValueUnitChange(dValue,pBri->m_pARcBridgeDataStd->m_szTonf_M,UNIT_CGS_TONF_M);

		return dValue;
	}
}

double CARcBridgeCalcColAndGir::GetMaxAxialForceOnGirder(long nJijum, BOOL bUpper, ETypeEnvComboLsd eEnvType)
{
	double Shear1=0,Shear2=0;
	return GetMaxAxialForceOnGirder(nJijum, Shear1, Shear2, bUpper, eEnvType);
}

// nTypeCheck 0-축력최대,1-모멘트최대,2-최대편심,3-지진시
void CARcBridgeCalcColAndGir::GetMomentAndAxialOnColumn(long nJijum, long nTypeCheck, double &dAxialReturn, double &dMomentReturn, double &dShearReturn, long &nTypeDesignReturn, long &nComboReturn)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	CElement *pEle = NULL;
	CResultForceElement *pResultMax = NULL;
	CResultForceElement *pResultMin = NULL;

//	long nSizeUlt = pBri->m_MatComboUlt.GetRow();
	long nSizeUlt = pBri->GetSizeCombo(COMBO_ULTIMATE, TRUE);

	BOOL bArch = (pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)? TRUE : FALSE;
	CString szCombo = _T("");
	long nLoadCase  = 0;
	long nPoint = 0;
	double dAxialMax,dAxialMin,dMomentMax,dMomentMin,dShearMax,dShearMin;
	double dAxialCur,dMomentCur,dShearCur,eCur;
	dAxialReturn  = 0; 
	dMomentReturn = 0;
	dShearReturn  = 0;
	nTypeDesignReturn = -1;
	nComboReturn  = -1;

	// (ARCBRIDGE-2034) 기둥하단위치 부재력 -> 기둥부재 전체 부재력 적용
	long nFindEleType(0);
	if(nJijum == 0)
		nFindEleType = ELEMENT_STTWALL;
	else if(nJijum == pBri->m_nQtyJigan)
		nFindEleType = ELEMENT_ENDWALL;
	else
		nFindEleType = ELEMENT_MIDWALL;
	BOOL bMaxAxial	= (nTypeCheck == 0)? TRUE : FALSE;
	BOOL bMaxShear	= TRUE;

	long nSizeEle = pBri->m_FemNormal.GetElementSize();
	long nIdxEleEnd = pBri->GetNumberElementWallEndCheck(nJijum, (nTypeCheck==3)? COMBO_EARTHQUAKE:COMBO_SERVICE, FALSE, FALSE);
	// 계수하중 조합
	long i=0; for(i=0; i<nSizeUlt; i++)
	{
		if(nTypeCheck==3) break;

		BOOL bNotMaxMin = FALSE;
		szCombo.Format("UL%02d MAX",i+1);
		nLoadCase = pBri->m_FemNormal.GetLoadCaseIndex(szCombo);	
		if(nLoadCase==-1) 
		{
			szCombo.Format("UL%02d",i+1);
			nLoadCase = pBri->m_FemNormal.GetLoadCaseIndex(szCombo);	
			if(nLoadCase==-1) 
				continue;

			bNotMaxMin = TRUE;
		}

// 		if(bArch)
// 		{
// 			// ARCH 일경우 중간벽체에 FIX가 없으므로.
// 			pEle = m_pStd->m_pFrame->GetElementWallMatchArchRib(&pBri->m_FemNormal,nJijum);
// 		}
// 		else
// 		{
// 			// 뒤 두 인자는 필요없음..
// //			pEle = pBri->m_FemNormal.GetElement((USHORT)nIdxEleEnd);
// 			
// 			pEle = m_pStd->m_pFrame->GetElementWallFix(&pBri->m_FemNormal,nJijum);
// 		}
// 		if(pEle == NULL) return;

		BOOL bFirst(TRUE);
		for(long ixEle=0; ixEle<nSizeEle; ++ixEle)
		{
			pEle = pBri->m_FemNormal.GetElement((USHORT)ixEle);

			if(pEle->m_nFlag != nFindEleType)
				continue;

			if(pEle->m_LongAttrib[LONG_1_ELEMENT_JIJUMNO] != nJijum)
				continue;

			pResultMax  = pEle->GetResultForce(nLoadCase);
			if(bNotMaxMin == FALSE)
				pResultMin  = pEle->GetResultForce(nLoadCase+1);

			long icPoint = pResultMax->m_ResultForceElement.GetSize();
			for(long ixPoint=0; ixPoint<icPoint; ++ixPoint)
			{
				dAxialMax  = fabs(pResultMax->m_ResultForceElement[ixPoint]->m_dForceOnElement[ELE_AXFORCE]);
				dMomentMax = fabs(pResultMax->m_ResultForceElement[ixPoint]->m_dForceOnElement[ELE_MOMENT2]);
				dShearMax = fabs(pResultMax->m_ResultForceElement[ixPoint]->m_dForceOnElement[ELE_SHEAR_2]);
				
				if(bNotMaxMin == FALSE)
				{
					dAxialMin  = fabs(pResultMin->m_ResultForceElement[ixPoint]->m_dForceOnElement[ELE_AXFORCE]);
					dMomentMin = fabs(pResultMin->m_ResultForceElement[ixPoint]->m_dForceOnElement[ELE_MOMENT2]);
					dShearMin = fabs(pResultMin->m_ResultForceElement[ixPoint]->m_dForceOnElement[ELE_SHEAR_2]);
				}

				// (ARCBRIDGE-2369) Max case에는 부재력이 0이 나올수 있다. 축력은 Min만 가지고 구하도록.
				double dAxialMaxMin = bNotMaxMin? dAxialMax : dAxialMin; //((bMaxAxial)? MAX(dAxialMax, dAxialMin) : MIN(dAxialMax, dAxialMin));
				double dMomentMaxMin = bNotMaxMin? dMomentMax : MAX(dMomentMax, dMomentMin);
				double dShearMaxMin = bNotMaxMin? dShearMax : (bMaxShear? MAX(dShearMax, dShearMin) : MIN(dShearMax, dShearMin));

				if(bFirst)
				{
					bFirst = FALSE;
					dAxialCur	= dAxialMaxMin;
					dMomentCur	= dMomentMaxMin;
					dShearCur	= dShearMaxMin;
				}
				else
				{
					if(bMaxAxial == (dAxialMaxMin > dAxialCur))
						dAxialCur = dAxialMaxMin;
					if(dMomentMaxMin > dMomentCur)
						dMomentCur = dMomentMaxMin;
					if(bMaxShear == (dShearMaxMin > dShearCur))
						dShearCur = dShearMaxMin;
				}
			}
		}

		BOOL bApplyCase = FALSE;
		if(nTypeCheck==0)
		{
			if(dAxialCur>dAxialReturn)
			{
				bApplyCase = TRUE;
			}
		}
		else if(nTypeCheck==1)
		{
			if(dMomentCur>dMomentReturn)
			{
				bApplyCase = TRUE;
			}
		}
		else if (nTypeCheck==2)
		{
			eCur    = dMomentCur/dAxialCur;
		
			if(eCur>(dMomentReturn/dAxialReturn))
			{
				bApplyCase = TRUE;
			}
		}
		if(bApplyCase || nComboReturn==-1)
		{
			dAxialReturn  = dAxialCur;
			dMomentReturn = dMomentCur;
			dShearReturn  = dShearCur;
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

		BOOL bNotMaxMin = FALSE;
		szCombo.Format("UE%02d MAX",i+1);
		nLoadCase = pBri->m_FemNormal.GetLoadCaseIndex(szCombo);	
		if(nLoadCase==-1) 
		{
			szCombo.Format("UE%02d",i+1);
			nLoadCase = pBri->m_FemNormal.GetLoadCaseIndex(szCombo);	
			if(nLoadCase==-1) 
				continue;

			bNotMaxMin = TRUE;
		}

		BOOL bFirst(TRUE);
		for(long ixEle=0; ixEle<nSizeEle; ++ixEle)
		{
			pEle = pBri->m_FemNormal.GetElement((USHORT)ixEle);

			if(pEle->m_nFlag != nFindEleType)
				continue;

			if(pEle->m_LongAttrib[LONG_1_ELEMENT_JIJUMNO] != nJijum)
				continue;

			pResultMax  = pEle->GetResultForce(nLoadCase);
			if(bNotMaxMin == FALSE)
				pResultMin  = pEle->GetResultForce(nLoadCase+1);

			long icPoint = pResultMax->m_ResultForceElement.GetSize();
			for(long ixPoint=0; ixPoint<icPoint; ++ixPoint)
			{
				dAxialMax  = fabs(pResultMax->m_ResultForceElement[ixPoint]->m_dForceOnElement[ELE_AXFORCE]);
				dMomentMax = fabs(pResultMax->m_ResultForceElement[ixPoint]->m_dForceOnElement[ELE_MOMENT2]);
				dShearMax = fabs(pResultMax->m_ResultForceElement[ixPoint]->m_dForceOnElement[ELE_SHEAR_2]);
				
				if(bNotMaxMin == FALSE)
				{
					dAxialMin  = fabs(pResultMin->m_ResultForceElement[ixPoint]->m_dForceOnElement[ELE_AXFORCE]);
					dMomentMin = fabs(pResultMin->m_ResultForceElement[ixPoint]->m_dForceOnElement[ELE_MOMENT2]);
					dShearMin = fabs(pResultMin->m_ResultForceElement[ixPoint]->m_dForceOnElement[ELE_SHEAR_2]);
				}

				// (ARCBRIDGE-2369) Max case에는 부재력이 0이 나올수 있다. 축력은 Min만 가지고 구하도록.
				double dAxialMaxMin = bNotMaxMin? dAxialMax : dAxialMin; // (bMaxAxial? MAX(dAxialMax, dAxialMin) : MIN(dAxialMax, dAxialMin));
				double dMomentMaxMin = bNotMaxMin? dMomentMax : MAX(dMomentMax, dMomentMin);
				double dShearMaxMin = bNotMaxMin? dShearMax : (bMaxShear? MAX(dShearMax, dShearMin) : MIN(dShearMax, dShearMin));

				if(bFirst)
				{
					bFirst = FALSE;
					dAxialCur	= dAxialMaxMin;
					dMomentCur	= dMomentMaxMin;
					dShearCur	= dShearMaxMin;
				}
				else
				{
					if(bMaxAxial == (dAxialMaxMin > dAxialCur))
						dAxialCur = dAxialMaxMin;
					if(dMomentMaxMin > dMomentCur)
						dMomentCur = dMomentMaxMin;
					if(bMaxShear == (dShearMaxMin > dShearCur))
						dShearCur = dShearMaxMin;
				}
			}
		}
		eCur = dMomentCur/dAxialCur;
		if(eCur>(dMomentReturn/dAxialReturn))
		{
			dAxialReturn  = dAxialCur;
			dMomentReturn = dMomentCur;
			dShearReturn = dShearCur;
			nTypeDesignReturn = COMBO_ECCENTRIC;
			nComboReturn = i;
		}
	}

	// 지진시 조합 -> 편심이 가장 크게 발생하는 조합으로 검토
	
	nIdxEleEnd = pBri->GetNumberElementWallEndCheck(nJijum, COMBO_EARTHQUAKE, FALSE, FALSE);
	long nSizeEQ = pBri->m_MatComboEQ.GetRow();
	long nSizeModel = pBri->GetCountEarthQuakeModel();
	long nIdxFemEQ = 0;
	for(long nEQ=0; nEQ < nSizeModel; nEQ++)
	{
		nIdxFemEQ = pBri->IsBoxType() ? nEQ : nEQ + 1;
		long nSizeEle = pBri->m_FemEQ[nIdxFemEQ].GetElementSize();
		if(nTypeCheck!=3) break;

		// if(pBri->IsBoxType() == FALSE && nEQ == 1 && pBri->m_nSelectModelingCountEQ!=2) break;

		for(i=0; i<nSizeEQ; i++)
		{
			if(nTypeCheck!=3) break;

			BOOL bNotMaxMin = FALSE;
			szCombo.Format("EQ%02d MAX",i+1);
			nLoadCase = pBri->m_FemEQ[nIdxFemEQ].GetLoadCaseIndex(szCombo);	
			if(nLoadCase==-1) 
			{
				szCombo.Format("EQ%02d",i+1);
				nLoadCase = pBri->m_FemEQ[nIdxFemEQ].GetLoadCaseIndex(szCombo);	
				if(nLoadCase==-1) 
					continue;

				bNotMaxMin = TRUE;
			}

			BOOL bFirst(TRUE);
			for(long ixEle=0; ixEle<nSizeEle; ++ixEle)
			{
				pEle = pBri->m_FemEQ[nIdxFemEQ].GetElement((USHORT)ixEle);

				if(pEle->m_nFlag != nFindEleType)
					continue;

				if(pEle->m_LongAttrib[LONG_1_ELEMENT_JIJUMNO] != nJijum)
					continue;

				pResultMax  = pEle->GetResultForce(nLoadCase);
				if(bNotMaxMin == FALSE)
					pResultMin  = pEle->GetResultForce(nLoadCase+1);

				long icPoint = pResultMax->m_ResultForceElement.GetSize();
				for(long ixPoint=0; ixPoint<icPoint; ++ixPoint)
				{
					dAxialMax  = fabs(pResultMax->m_ResultForceElement[ixPoint]->m_dForceOnElement[ELE_AXFORCE]);
					dMomentMax = fabs(pResultMax->m_ResultForceElement[ixPoint]->m_dForceOnElement[ELE_MOMENT2]);
					dShearMax = fabs(pResultMax->m_ResultForceElement[ixPoint]->m_dForceOnElement[ELE_SHEAR_2]);

					if(bNotMaxMin == FALSE)
					{
						dAxialMin  = fabs(pResultMin->m_ResultForceElement[ixPoint]->m_dForceOnElement[ELE_AXFORCE]);
						dMomentMin = fabs(pResultMin->m_ResultForceElement[ixPoint]->m_dForceOnElement[ELE_MOMENT2]);
						dShearMin = fabs(pResultMin->m_ResultForceElement[ixPoint]->m_dForceOnElement[ELE_SHEAR_2]);
					}

					// (ARCBRIDGE-2369) Max case에는 부재력이 0이 나올수 있다. 축력은 Min만 가지고 구하도록.
					double dAxialMaxMin = bNotMaxMin? dAxialMax : dAxialMin; // (bMaxAxial? MAX(dAxialMax, dAxialMin) : MIN(dAxialMax, dAxialMin));
					double dMomentMaxMin = bNotMaxMin? dMomentMax : MAX(dMomentMax, dMomentMin);
					double dShearMaxMin = bNotMaxMin? dShearMax : (bMaxShear? MAX(dShearMax, dShearMin) : MIN(dShearMax, dShearMin));

					if(bFirst)
					{
						bFirst = FALSE;
						dAxialCur	= dAxialMaxMin;
						dMomentCur	= dMomentMaxMin;
						dShearCur	= dShearMaxMin;
					}
					else
					{
						if(bMaxAxial == (dAxialMaxMin > dAxialCur))
							dAxialCur = dAxialMaxMin;
						if(dMomentMaxMin > dMomentCur)
							dMomentCur = dMomentMaxMin;
						if(bMaxShear == (dShearMaxMin > dShearCur))
							dShearCur = dShearMaxMin;
					}
				}
			}
			eCur = dMomentCur/dAxialCur;
			if(eCur>(dMomentReturn/dAxialReturn) || (Compare(eCur, 0.0, ">") && Compare(dAxialReturn, 0.0, "=")))
			{
				dAxialReturn  = dAxialCur;
				dMomentReturn = dMomentCur;
				dShearReturn = dShearCur;
				nTypeDesignReturn = (i==0) ? COMBO_EARTHQUAKE : COMBO_EARTHQUAKE2;
				nComboReturn = i;
			}
		}
	}

	double SlabWidth  = m_pStd->GetSlabWidth();
	double dDivide = (nTypeCheck==3) ? SlabWidth : 1.00;

	dAxialReturn  = GetValueUnitChange(Round(dAxialReturn,3) ,pBri->m_pARcBridgeDataStd->m_szTonf,UNIT_CGS_TONF)/dDivide;
	dMomentReturn = GetValueUnitChange(Round(dMomentReturn,3),pBri->m_pARcBridgeDataStd->m_szTonf,UNIT_CGS_TONF)/dDivide;
	dShearReturn = GetValueUnitChange(Round(dShearReturn,3),pBri->m_pARcBridgeDataStd->m_szTonf,UNIT_CGS_TONF)/dDivide;
}

void CARcBridgeCalcColAndGir::WriteCalcGirder(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle, BOOL bUpperGirder)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pData->IsLsdDesign();

	long nIndexSmall = 1;
	long nIndexTiny  = 1;
	long nIndexTiny2 = 1;
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$s+40$h$a%d.%d %s$n$n",nIndexTitle,nIndexMiddle, bUpperGirder ? _T("상 부 거 더") :  _T("하 부 거 더"));

	long nDec03 = bLsd ? 0 : 3;

	CString str = _T("");
	long nSize = m_pList.GetSize();
	long nUD = bUpperGirder ? iUPPER : iLOWER;
	CString strGirder = bUpperGirder ? _T("상부거더") : _T("하부거더");
	for(long i=0; i<nSize; i++)
	{
		CGirderAndColumn *pGirCol = m_pList.GetAt(i);
		if(pGirCol->m_bGirder==FALSE) continue;

		double Ln1   = pGirCol->m_dSpanLengthGirder[RCBRIDGE_GIR_POS_END];
		double Ln2   = pGirCol->m_dSpanLengthGirder[RCBRIDGE_GIR_POS_MID];
		double H     = toM(pGirCol->pConcGirEnd[nUD][0]->m_Ho);
		double dc    = toM(pGirCol->pConcGirEnd[nUD][0]->GetRebarDistTension());
		double dDist = H - dc;

		nIndexTiny  = 1;
		if(pGirCol->m_nJijum == 0 || pGirCol->m_nJijum == pBri->m_nQtyJigan)
		{
			XLText->AddFormatText("$s+40$h %d)$r %s : %s$n", nIndexSmall++, strGirder,(pGirCol->m_nJijum == 0)? "시점" : "종점");
		}
		else
		{
			XLText->AddFormatText("$s+40$h %d)$r %s : 중간지점 %d$n",nIndexSmall++,strGirder,pGirCol->m_nJijum);
		}

		XLText->GoPosition(NO_MOVE ,COL_POS_2);
		XLText->AddFormatText("$h$c%s$r설계단면 및 조건$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));	
		XLText->AddFormatText("$h$r'- 외측지간(ln)$m+04$c=$r%s$m+08$c=$r%s %s$n",
			pGirCol->m_szCalcLn[RCBRIDGE_GIR_POS_END], pData->GetStrDoubleUnitChange(Ln1, UNIT_CGS_M, pData->m_szM), pData->m_szM);
		if(Ln2 > 0) 
		{
			XLText->AddFormatText("$h$r'- 내측지간(ln)$m+04$c=$r%s$m+08$c=$r%s %s$n",
				pGirCol->m_szCalcLn[RCBRIDGE_GIR_POS_MID],pData->GetStrDoubleUnitChange(Ln2, UNIT_CGS_M, pData->m_szM), pData->m_szM);
		}
		XLText->AddFormatText("$h$r'- 단면폭  (B) $m+04$c=$r%s %s$n",pData->GetStrDoubleUnitChange(toM(pGirCol->pConcGirEnd[nUD][0]->m_Bo), UNIT_CGS_M, pData->m_szM), pData->m_szM);
		XLText->AddFormatText("$h$r'- 단면높이(H) $m+04$c=$r%s %s$n",pData->GetStrDoubleUnitChange(toM(pGirCol->pConcGirEnd[nUD][0]->m_Ho), UNIT_CGS_M, pData->m_szM), pData->m_szM);
		XLText->AddFormatText("$h$r'- 유효높이(d) $m+04$c=$r%s %s$n",pData->GetStrDoubleUnitChange(dDist, UNIT_CGS_M, pData->m_szM), pData->m_szM);

		for(long ixEnvType=0; ixEnvType < eCountEnvTypeLsd; ++ixEnvType)
		{
			if(bLsd == FALSE && ixEnvType > 0)
				break;

			CString szEnvType(_T(""));
			if(bLsd)
			{
				ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ixEnvType;
				if(pBri->IsValidComboEnvType(eEnvType) == FALSE)
					continue;

				szEnvType.Format(_T("(%s)"), pBri->GetStringComboEnvType_Lsd(eEnvType, TRUE));
			}

			// (ARCBRIDGE-2693) 거더 검토에서 부재력은 모델링의 단위폭당 부재력을 쓰기때문에 tonf가 아니라 tonf/m 단위를 쓴다.
			double w     = GetValueUnitChange(bUpperGirder ? pGirCol->m_dAxialForceTop[RCBRIDGE_GIR_POS_END][ixEnvType] : pGirCol->m_dAxialForceBot[RCBRIDGE_GIR_POS_END][ixEnvType],UNIT_CGS_TONF_M,pData->m_szTonf_M);

			if(pGirCol->m_bColumn==FALSE)
			{
				XLText->AddFormatText("$h$r'- 작용하중(w) $m+04$c=$r'%.3f %s %s$n",w,pData->m_szTonf, szEnvType);
			}
			else
			{
				CString strPos = bUpperGirder ? _T("상단부") : _T("하단부");
				XLText->AddFormatText("$h$r'- 작용하중(w) $m+04$c=$r'%.3f %s, 기둥%s의 축력 %s$n",w,pData->m_szTonf,strPos, szEnvType);
			}
		}

		double dCheckDeepBeam = (pData->m_nTypeKciCode == DESIGN_CODE_KCI_2003)? 5.0 : 4.0;
		double dLenCheck = toM(pGirCol->pConcGirEnd[nUD][0]->m_Ln);			// #33109 깊은보 검토 오류
		if(pGirCol->pConcGirEnd[nUD][0]->IsDeepBeam())						// #33109 깊은보 검토 오류
		{
			XLText->AddFormatText("$h$r'→ ln/d(= %.3f)이 %.0f 보다 작으므로 깊은 보로 설계한다.$n",dLenCheck/dDist, dCheckDeepBeam);
		}
		else
		{
			XLText->AddFormatText("$h$r'→ ln/d(= %.3f)이 %.0f 이상이므로 일반단면으로 설계한다.$n",dLenCheck/dDist, dCheckDeepBeam);
		}
		DrawGirderSection(XLText,pGirCol->m_nJijum,bUpperGirder);
		//
		CString strAdd = (Ln1!=Ln2) ? " : 최외측 경간" : _T("");
		XLText->AddFormatText("$h$c%s$r부재력 산정%s$n",GetNumberString(nIndexTiny++,NUM_CIRCLE),strAdd);

		for(long ixEnvType=0; ixEnvType < eCountEnvTypeLsd; ++ixEnvType)
		{
			if(bLsd == FALSE && ixEnvType > 0)
				break;

			CString szEnvType(_T(""));
			if(bLsd)
			{
				ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ixEnvType;
				if(pBri->IsValidComboEnvType(eEnvType) == FALSE)
					continue;

				szEnvType.Format(_T(" : %s"), pBri->GetStringComboEnvType_Lsd(eEnvType, TRUE));
			}
			double w     = GetValueUnitChange(bUpperGirder ? pGirCol->m_dAxialForceTop[RCBRIDGE_GIR_POS_END][ixEnvType] : pGirCol->m_dAxialForceBot[RCBRIDGE_GIR_POS_END][ixEnvType],UNIT_CGS_TONF_M,pData->m_szTonf_M);
			double Mu1   = GetValueUnitChange(pGirCol->m_GirMomentPlus[nUD][RCBRIDGE_GIR_POS_END][ixEnvType] ,UNIT_CGS_TONFM,pData->m_szTonfM);
			double Mu2   = GetValueUnitChange(pGirCol->m_GirMomentMinus[nUD][RCBRIDGE_GIR_POS_END][ixEnvType],UNIT_CGS_TONFM,pData->m_szTonfM);
			double Vu    = GetValueUnitChange(pGirCol->m_GirShear[nUD][RCBRIDGE_GIR_POS_END][ixEnvType],UNIT_CGS_TONF,pData->m_szTonf);

			XLText->AddFormatText("$h$r$c%s$r정휨모멘트%s$n",GetNumberString(nIndexTiny2++,NUM_GANA), szEnvType);	
			// (ARCBRIDGE-2693) pGirCol->m_dDivOutBeamPlus는 상수로 단위가 없다.
			XLText->AddFormatText("$h$r$r$cMu$r$c=$rw x ln²/ %.0f = %.3f x %s²/ %.0f$n",
				pGirCol->m_dDivOutBeamPlus,w,
				pData->GetStrDoubleUnitChange(Ln1, UNIT_CGS_M, pData->m_szM),pGirCol->m_dDivOutBeamPlus);	
			XLText->AddFormatText("$h$m+03$c=$r%s %s$n",GetStrDouble(Mu1,nDec03,bLsd),pData->m_szTonfM);	

			XLText->AddFormatText("$h$r$c%s$r부휨모멘트%s$n",GetNumberString(nIndexTiny2++,NUM_GANA), szEnvType);	
			XLText->AddFormatText("$h$r$r$cMu$r$c=$rw x ln²/ %.0f = %.3f x %s²/ %.0f$n",
				pGirCol->m_dDivOutBeamMinus, w,
				pData->GetStrDoubleUnitChange(Ln1, UNIT_CGS_M, pData->m_szM),pGirCol->m_dDivOutBeamMinus);	
			XLText->AddFormatText("$h$m+03$c=$r%s %s$n",GetStrDouble(Mu2,nDec03,bLsd),pData->m_szTonfM);	

			XLText->AddFormatText("$h$r$c%s$r전단력 : 등분포하중을 받는 보%s$n",GetNumberString(nIndexTiny2++,NUM_GANA), szEnvType);	
			XLText->AddFormatText("$h$r$r$cVu$r$c=$r%.2f x w x ln / 2 - 0.15 x w x ln$n",pGirCol->m_dFactorShear);
			XLText->AddFormatText("$h$m+03$c=$r%.2f x %.3f x %s / 2 - 0.15 x %.3f x %s$n",
				pGirCol->m_dFactorShear,w,pData->GetStrDoubleUnitChange(Ln1, UNIT_CGS_M, pData->m_szM),w,pData->GetStrDoubleUnitChange(Ln1, UNIT_CGS_M, pData->m_szM));
			XLText->AddFormatText("$h$m+03$c=$r%s %s$n",GetStrDouble(Vu,nDec03,bLsd),pData->m_szTonf);
		}
		if(Ln1!=Ln2 && Ln2>0)
		{
			nIndexTiny2 = 1;
			XLText->AddFormatText("$s+40$h$c%s$r부재력 산정 : 내부 경간$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));

			for(long ixEnvType=0; ixEnvType < eCountEnvTypeLsd; ++ixEnvType)
			{
				if(bLsd == FALSE && ixEnvType > 0)
					break;

				CString szEnvType(_T(""));
				if(bLsd)
				{
					ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ixEnvType;
					if(pBri->IsValidComboEnvType(eEnvType) == FALSE)
						continue;

					szEnvType.Format(_T(" : %s"), pBri->GetStringComboEnvType_Lsd(eEnvType, TRUE));
				}
				double w     = GetValueUnitChange(bUpperGirder ? pGirCol->m_dAxialForceTop[RCBRIDGE_GIR_POS_MID][ixEnvType] : pGirCol->m_dAxialForceBot[RCBRIDGE_GIR_POS_MID][ixEnvType],UNIT_CGS_TONF_M,pData->m_szTonf_M);
				double Mu1   = GetValueUnitChange(pGirCol->m_GirMomentPlus[nUD][RCBRIDGE_GIR_POS_MID][ixEnvType] ,UNIT_CGS_TONFM,pData->m_szTonfM);
				double Mu2   = GetValueUnitChange(pGirCol->m_GirMomentMinus[nUD][RCBRIDGE_GIR_POS_MID][ixEnvType],UNIT_CGS_TONFM,pData->m_szTonfM);
				double Vu    = GetValueUnitChange(pGirCol->m_GirShear[nUD][RCBRIDGE_GIR_POS_MID][ixEnvType],UNIT_CGS_TONF,pData->m_szTonf);
				// (ARCBRIDGE-2693) pGirCol->m_dDivOutBeamPlus는 상수로 단위가 없다. (ARCBRIDGE-2732) m_dDivInBeamPlus도 
				XLText->AddFormatText("$h$r$c%s$r정휨모멘트%s$n",GetNumberString(nIndexTiny2++,NUM_GANA), szEnvType);	
				XLText->AddFormatText("$h$r$r$cMu$r$c=$rw x ln²/ %.0f = %.3f x %s²/ %.0f$n",
					pGirCol->m_dDivInBeamPlus,w,pData->GetStrDoubleUnitChange(Ln2, UNIT_CGS_M, pData->m_szM),pGirCol->m_dDivInBeamPlus);	
				XLText->AddFormatText("$h$m+03$c=$r%s %s$n",GetStrDouble(Mu1,nDec03,bLsd),pData->m_szTonfM);	
				XLText->AddFormatText("$h$r$c%s$r부휨모멘트%s$n",GetNumberString(nIndexTiny2++,NUM_GANA), szEnvType);	
				XLText->AddFormatText("$h$r$r$cMu$r$c=$rw x ln²/ %.0f = %.3f x %s²/ %.0f$n",
					pGirCol->m_dDivInBeamMinus,w,pData->GetStrDoubleUnitChange(Ln2, UNIT_CGS_M, pData->m_szM),pGirCol->m_dDivInBeamMinus);	
				XLText->AddFormatText("$h$m+03$c=$r%s %s$n",GetStrDouble(Mu2,nDec03,bLsd),pData->m_szTonfM);	
				XLText->AddFormatText("$h$r$c%s$r전단력 : 등분포하중을 받는 보%s$n",GetNumberString(nIndexTiny2++,NUM_GANA), szEnvType);	
				XLText->AddFormatText("$h$r$r$cVu$r$c=$r%.2f x w x ln / 2 - 0.15 x w x ln$n",pGirCol->m_dFactorShear);
				XLText->AddFormatText("$h$m+03$c=$r%.2f x %.3f x %s / 2 - 0.15 x %.3f x %s$n",
					pGirCol->m_dFactorShear,w,pData->GetStrDoubleUnitChange(Ln2, UNIT_CGS_M, pData->m_szM),w,pData->GetStrDoubleUnitChange(Ln2, UNIT_CGS_M, pData->m_szM));
				XLText->AddFormatText("$h$m+03$c=$r%s %s$n",GetStrDouble(Vu,nDec03,bLsd),pData->m_szTonf);	
			}
		}
		//
		if (bLsd)
		{
			for(long ixEnvType=0; ixEnvType < eEnvService; ++ixEnvType)
			{
				ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ixEnvType;
				CString szEnvType(_T(""));
				szEnvType.Format(_T(" : %s"), pBri->GetStringComboEnvType_Lsd(eEnvType, FALSE));

				if(pBri->IsValidComboEnvType(eEnvType) == FALSE)
					continue;

				XLText->AddFormatText("$s+40$h$c%s$r단면검토 : 지점부%s$n",GetNumberString(nIndexTiny++,NUM_CIRCLE), szEnvType);

				CLsdBeam beamEnd(pGirCol->pConcGirEnd[nUD][ixEnvType], LSD_ULS_DESIGN);
				beamEnd.MakeOutputDesignExcelFile (XLText,COL_POS_2,TRUE, FALSE, FALSE);

				if(pGirCol->pConcGirEnd[nUD][ixEnvType]->IsDeepBeam())
				{
					CRectBeam rectEnd(pGirCol->pConcGirEnd[nUD][ixEnvType], pBri->m_bCheckMainSlabAndFootTemper);
					XLText->AddFormatText("$s+40$h$c%s$r깊은 보 검토 : 지점부%s$n",GetNumberString(nIndexTiny++,NUM_CIRCLE), szEnvType);
					rectEnd.MakeOutputDeepBeamExcelFile(XLText,COL_POS_2);
				}

				if(Ln2 > 0)
				{
					XLText->AddFormatText("$s+40$h$c%s$r단면검토 : 중앙부%s$n",GetNumberString(nIndexTiny++,NUM_CIRCLE), szEnvType);

					CLsdBeam beamMid(pGirCol->pConcGirMid[nUD][ixEnvType], LSD_ULS_DESIGN);
					beamMid.MakeOutputDesignExcelFile (XLText,COL_POS_2,FALSE, FALSE, FALSE);
				}
			}

			// 사용성검토
			XLText->AddFormatText("$s+40$h$c%s$r균열검토 : 지점부$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
			CLsdBeam BeamLsd(pGirCol->pConcGirEnd[nUD][eEnvLimit], LSD_ULS_DESIGN);
			long nMatType = pBri->GetLsdMatrialTypePos(bUpperGirder? ePartUpperSlab : ePartLowerSlab, FALSE);
			BeamLsd.SetMatrialTypePos(nMatType);
			BeamLsd.MakeOutputUsabilityExcelFile(XLText, COL_POS_2, FALSE);	

			if(Ln2 > 0)
			{
				XLText->AddFormatText("$s+40$h$c%s$r균열검토 : 중앙부$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
				CLsdBeam BeamLsd(pGirCol->pConcGirMid[nUD][eEnvLimit], LSD_ULS_DESIGN);
				long nMatType = pBri->GetLsdMatrialTypePos(bUpperGirder? ePartUpperSlab : ePartLowerSlab, FALSE);
				BeamLsd.SetMatrialTypePos(nMatType);
				BeamLsd.MakeOutputUsabilityExcelFile(XLText, COL_POS_2, FALSE);	
			}
		}
		else
		{
			XLText->AddFormatText("$s+40$h$c%s$r단면검토 : 지점부$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));

			CRectBeam rectEnd(pGirCol->pConcGirEnd[nUD][0], pBri->m_bCheckMainSlabAndFootTemper);		
			rectEnd.MakeOutputDesignExcelFile (XLText,COL_POS_2);

			if(pGirCol->pConcGirEnd[nUD][0]->IsDeepBeam())
			{
				XLText->AddFormatText("$s+40$h$c%s$r깊은 보 검토 : 지점부$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
				rectEnd.MakeOutputDeepBeamExcelFile(XLText,COL_POS_2);
			}

			if(Ln2 > 0)
			{
				XLText->AddFormatText("$s+40$h$c%s$r단면검토 : 중앙부$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
				CRectBeam rectMid(pGirCol->pConcGirMid[nUD][0], pBri->m_bCheckMainSlabAndFootTemper);
				rectMid.MakeOutputDesignExcelFile (XLText,COL_POS_2, FALSE);
			}
		}
	}
}


void CARcBridgeCalcColAndGir::WriteCalcJongGirder(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pData->IsLsdDesign();
	
	long nIndexSmall = 1;
	long nIndexTiny  = 1;
	long nIndexTiny2 = 1;
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$s+40$h$a%d.%d 종 방 향 거 더$n$n",nIndexTitle,nIndexMiddle);
	
	long nCombo = 0;
	double FactorL1(0);
	if(bLsd)
	{
		CLsdLoadCase *pLoadCase = pBri->m_plsdManager->GetLsdLoadCase(eLSD_COMBINE_LIMIT_I, eLSD_LOADCASE_DIR_BRI, 0);
		if(pLoadCase)
		{
			FactorL1 = pLoadCase->GetLoadFactor(eLSD_LOADCASE_LL1);
		}
	}
	else
	{
		FactorL1 = pBri->GetLoadFactor(LOAD_COMBINE_ULT, nCombo, BRIDGE_LOAD_LIVE_FULL);
	}

	CString str = _T("");
	long nSize = m_pArrConcJongGir.GetSize();
	long i=0; for(i=0; i<nSize; i++)
	{
		CConcBasic *pConc = m_pArrConcJongGir.GetAt(i);
		if(!pConc) continue;
		if(!pConc->IsValidData()) continue;
		
		double Ln1   = toM(pConc->m_Ln);
		double H     = toM(pConc->m_Ho);
		double dc    = toM(pConc->GetRebarDistTension());
		double dDist = H - dc;
		double P     = GetValueUnitChange(m_pStd->GetWheelLoadRear(),UNIT_CGS_TONF,pData->m_szTonf);
		double Mu1   = GetValueUnitChange(pConc->m_Moment ,UNIT_CGS_TONFM,pData->m_szTonfM);
		
		BOOL bStt		= FALSE;
		BOOL bLeft		= FALSE;
		BOOL bGagak	= FALSE;
		if(pConc->m_sTitle.Right(4) == "가각")
		{
			bStt = (pConc->m_sTitle.Left(4) == "시점")? TRUE : FALSE;
			bLeft = (pConc->m_sTitle.Mid(4).Left(4) == "좌측")? TRUE : FALSE;
			bGagak = TRUE;
		}
		else
		{
			bLeft = (pConc->m_sTitle.Left(4) == "좌측")? TRUE : FALSE;
			bStt = bGagak = FALSE;
		}

		nIndexTiny  = 1;
		
		XLText->AddFormatText("$s+40$h %d)$r 종방향 거더 : %s$n", nIndexSmall++, pConc->m_sTitle);
		
		XLText->GoPosition(NO_MOVE ,COL_POS_2);
		XLText->AddFormatText("$h$c%s$r설계단면 및 조건$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));	
		XLText->AddFormatText("$h$r'- 내측지간(ln)$m+04$c=$r'%s %s$n", pData->GetStrDoubleUnitChange(Ln1, UNIT_CGS_M, pData->m_szM), pData->m_szM);
		XLText->AddFormatText("$h$r'- 단면폭  (B) $m+04$c=$r'%s %s$n", pData->GetStrDoubleUnitChange(toM(pConc->m_Bo), UNIT_CGS_M, pData->m_szM), pData->m_szM);
		XLText->AddFormatText("$h$r'- 단면높이(H) $m+04$c=$r'%s %s$n", pData->GetStrDoubleUnitChange(toM(pConc->m_Ho), UNIT_CGS_M, pData->m_szM), pData->m_szM);
		XLText->AddFormatText("$h$r'- 유효높이(d) $m+04$c=$r'%s %s$n", pData->GetStrDoubleUnitChange(dDist, UNIT_CGS_M, pData->m_szM), pData->m_szM);
		XLText->AddFormatText("$h$r'- 작용하중(P) $m+04$c=$r'%s %s$n", pData->GetStrDouble(P, UNIT_CGS_TONF),pData->m_szTonf);

		DrawJongGirderSection(XLText, bStt, bLeft, bGagak);
		//
		XLText->AddFormatText("$h$c%s$r부재력 산정%s$n",GetNumberString(nIndexTiny++,NUM_CIRCLE), "내부 경간");
		XLText->AddFormatText("$h$r$c%s$r정휨모멘트$n",GetNumberString(nIndexTiny2++,NUM_GANA));	
		XLText->AddFormatText("$h$r$r$cMu$r$c=$r(%.2f x PL) x %.2f = (%.2f x (%.3f x %.3f)) x %.2f$n", (pBri->m_nQtyJigan>1)? 0.08 : 0.10,
			FactorL1, (pBri->m_nQtyJigan>1)? 0.08 : 0.10, P,Ln1, FactorL1);
		XLText->AddFormatText("$h$m+03$c=$r%s %s$n", pData->GetStrDouble(Mu1, UNIT_CGS_TONFM),pData->m_szTonfM);	

		XLText->AddFormatText("$s+40$h$c%s$r단면검토 : 중앙부$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
		if (bLsd)
		{
			CLsdBeam rectMid(pConc, LSD_ULS_DESIGN);
			rectMid.MakeOutputDesignExcelFile (XLText,COL_POS_2,FALSE,FALSE,FALSE);
		}
		else
		{
			CRectBeam rectMid(pConc, pBri->m_bCheckMainSlabAndFootTemper);
			rectMid.MakeOutputDesignExcelFile (XLText,COL_POS_2);
		}
	}
}

void CARcBridgeCalcColAndGir::WriteCalcColumn(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pData->IsLsdDesign();

	long nIndexSmall = 1;
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$s+40$h$a%d.%d 기둥,중간벽체 검토$n$n",nIndexTitle,nIndexMiddle);

	XLText->AddFormatText("$h %d)$r 단면력 산정$n",nIndexSmall++);
	XLText->GoPosition(NO_MOVE ,COL_POS_2);

	if (bLsd)
	{
		MakeTableColumnLoadLsd(XLText);
	}
	else
	{
		MakeTableColumnLoad(XLText);
	}
	

	if(IsExistColumn())
	{
		*XLText += 2;
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$h %d)$r 설계단면$n",nIndexSmall++);
		DrawColumnSection(XLText);
	}
	
	if (bLsd)
	{
		WriteCheckSection_Lsd(XLText, pBri, nIndexSmall);
	}
	else
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$s+50$h %d)$r 단면검토$n",nIndexSmall);

		WriteCheckSection(XLText, pBri);
	}	

	if (bLsd == FALSE || (bLsd && pBri->m_nSelectInWallSectionDesign == 1))
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$s+50$h %d)$r P-M 상관도$n",nIndexSmall++);

		if(bLsd)
			WritePMDiagram_Lsd(XLText, pBri);
		else
			WritePMDiagram(XLText, pBri);
	}
}

double CARcBridgeCalcColAndGir::GetDistDangerFromColumnCenter(long nJijum)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	CColumnApp *pCol = &pBri->GetInWall(nJijum-1)->m_Column;

	double Dist = 0;
//	double B = toM(pCol->m_dB[iOUTTER][iUPPER]);
//	double H = toM(pCol->m_dH[iOUTTER][iUPPER]);
	double Area = pCol->GetColumnArea();

// 	if(pCol->m_nType==COL_TYPE_CIRCLE) 
// 		Dist = sqrt(Area)/2;
// 	else if(pCol->m_nType==COL_TYPE_RECTANGLE)
// 		Dist = H/2;
// 	else if(pCol->m_nType==COL_TYPE_TRACK)
// 		Dist = Area/B/2;
// 	else 
// 		Dist = B/2;

	Dist = sqrt(Area)/2;
//	(ARCBRIDGE-1462) 기둥 환산단면 폭이 거더폭보다 클때 다시 환산단면을 구해준다.
	double dGirderW = toM(pBri->GetGirderWidth(nJijum, TRUE));
	if(Dist * 2 > dGirderW)
	{
		Dist = Area/dGirderW/2.;
	}

	return Dist;
}

CString CARcBridgeCalcColAndGir::GetStringCalcLn(long nJijum, BOOL bInnerSpan)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CJijum *pJijum = pBri->GetJijum(nJijum);

	long nVerPos  = (nJijum==0) ? 1 : 0;
	BOOL bShoePos = (pJijum->m_nQtyShoe[nVerPos]>0) ? TRUE : FALSE;

	double LenCen = GetLengthSpanGirder(nJijum,bInnerSpan);
	CString strLenCen = pData->GetStrDoubleUnitChange(LenCen, UNIT_CGS_M, pData->m_szM);

	CString strCalc = _T("");
	if(bShoePos)
	{
		if(bInnerSpan)
			strCalc.Format("%s (받침중심간 거리)",strLenCen);
		else
			strCalc.Format("%s (받침중심 ~ 슬래브연단)",strLenCen);
		return strCalc;
	}
	else
	{
		CColumnApp *pCol = &pBri->GetInWall(nJijum-1)->m_Column;
		
		double B = GetDistDangerFromColumnCenter(nJijum) * 2; // toM(pCol->m_dB[iOUTTER][iUPPER]);
		if (pCol->m_nType==COL_TYPE_CIRCLE)
			B /= 2;
		
		CString strB = pData->GetStrDoubleUnitChange(B, UNIT_CGS_M, pData->m_szM);
		
		if(pCol->m_nType==COL_TYPE_CIRCLE) 
		{
			if(bInnerSpan)
				strCalc.Format("%s - √(π x %s²)",strLenCen, strB);
			else
				strCalc.Format("%s - √(π x %s²) / 2",strLenCen, strB);
		}
		else if(pCol->m_nType==COL_TYPE_RECTANGLE)
		{
			if(bInnerSpan)
				strCalc.Format("%s - %s",strLenCen, strB);
			else
				strCalc.Format("%s - %s / 2",strLenCen, strB);
		}
		else if(pCol->m_nType==COL_TYPE_TRACK)
		{
			double Area = pCol->GetColumnArea();
			CString strArea = pData->GetStrDoubleUnitChange(Area, UNIT_CGS_M, pData->m_szM);

			if(bInnerSpan)
				strCalc.Format("%s - %s / %s",strLenCen,strArea, strB);
			else
				strCalc.Format("%s - %s / %s / 2",strLenCen,strArea, strB);
		}
		else 
		{
			if(bInnerSpan)
				strCalc.Format("%s - %s",strLenCen, strB);
			else
				strCalc.Format("%s - %s / 2",strLenCen, strB);
		}

		return strCalc;
	}
}

void CARcBridgeCalcColAndGir::DrawGirderSection(CXLFormatText *XLText, long nJijum, BOOL bUpperSlabGirder)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM = &pBri->m_FemNormal;

	double ScaleDim = 50;

	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(ScaleDim);
	pDom->SetLTScale(ScaleDim*5);

	long nSizeRow = 15;
	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, pData);
	DBStd.m_pDrawVertSection->DrawDetail_Girder(pDom, nJijum, TRUE, TRUE, TRUE, FALSE, bUpperSlabGirder);
	DBStd.m_pDrawVertSection->DrawRebarAll_SlabGirder(pDom, nJijum, TRUE, TRUE, 0, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, -1, FALSE, bUpperSlabGirder);
	DBStd.m_pDrawVertSection->DrawJijumSign(pDom, nJijum);

	XLText->InsertDomyun(pDom,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);
}

void CARcBridgeCalcColAndGir::SetConcData_BaseInfo(CConcBasic *pConc, BOOL bGirder, BOOL bUpperGirder, BOOL UpAndFront, ELSDLoadGroupKind eGroupKind)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	pConc->m_nTypeSection	= SECTION_GIRDER; // 단면의 타입,SECTION_SINGLE
	
	EPartOfBridge ePart = bGirder? (bUpperGirder? ePartUpperSlab : ePartLowerSlab) : ePartInWall;
	pBri->SetConcDataBasic(pConc, ePart, UpAndFront, eGroupKind);

	pConc->m_bMinAvCheck = TRUE;
	pConc->m_nEnvTypeCrack = pBri->m_nTypeCrackDesignSlab;
}

void CARcBridgeCalcColAndGir::SetConcData(BOOL bUpperSlabGirder)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pData->IsLsdDesign();

	if(bUpperSlabGirder == FALSE && pBri->IsBoxType() == FALSE) return;

	long nSize = m_pList.GetSize();

	long nUD = bUpperSlabGirder ? iUPPER : iLOWER;
	CString strGirder = bUpperSlabGirder ? _T("상부거더") : _T("하부거더");
	long i=0; for(i=0; i<nSize; i++)
	{
		CGirderAndColumn    *pGirCol = m_pList.GetAt(i);
		CGirderSectionRebar *pRebar  = &pBri->m_SlabGirderRebar[nUD][pGirCol->m_nJijum];

		// 거더 검토 단면
		// 극한, 극단
		for(long ixEnvType=0; ixEnvType<eEnvService; ++ixEnvType)
		{
			if(bLsd == FALSE && ixEnvType > 0)
				break;

			ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ixEnvType;
			
			// 거더 지점부
			CConcBasic *pConc = pGirCol->pConcGirEnd[nUD][ixEnvType];
			SetConcData_BaseInfo(pConc, TRUE, bUpperSlabGirder, TRUE, (ixEnvType == 0)? eLSD_COMBINATION_LIMIT : eLSD_COMBINATION_EXTREME);
			if(pGirCol->m_nJijum == 0 || pGirCol->m_nJijum == pBri->m_nQtyJigan)
			{
				pConc->m_sTitle.Format("%s 지점부 : %s", strGirder, (pGirCol->m_nJijum == 0)? "시점" : "종점");
			}
			else pConc->m_sTitle.Format("%s 지점부 : 중간지점 %d",strGirder,pGirCol->m_nJijum);
			pConc->m_Bo = pBri->GetGirderWidth(pGirCol->m_nJijum, bUpperSlabGirder);
			pConc->m_Ho = pBri->GetGirderHeight(pGirCol->m_nJijum, bUpperSlabGirder);

			pConc->m_Moment = max(pGirCol->m_GirMomentMinus[nUD][RCBRIDGE_GIR_POS_END][eEnvType],pGirCol->m_GirMomentMinus[nUD][RCBRIDGE_GIR_POS_MID][eEnvType]);
			pConc->m_Shear  = max(pGirCol->m_GirShear[nUD][RCBRIDGE_GIR_POS_END][eEnvType],pGirCol->m_GirShear[nUD][RCBRIDGE_GIR_POS_MID][eEnvType]);

			if(bLsd && eEnvType == eEnvLimit)
			{
				// 극한한계상태 - 상시
				pConc->m_ShearUse	= pConc->m_Shear;
				pConc->m_MomentUse	= max(pGirCol->m_GirMomentMinus[nUD][RCBRIDGE_GIR_POS_END][eEnvService],pGirCol->m_GirMomentMinus[nUD][RCBRIDGE_GIR_POS_MID][eEnvService]);
				pConc->m_MomentCrack	= max(pGirCol->m_GirMomentMinus[nUD][RCBRIDGE_GIR_POS_END][eEnvService_Crack],pGirCol->m_GirMomentMinus[nUD][RCBRIDGE_GIR_POS_MID][eEnvService_Crack]);
			}
			else
			{
				pConc->m_ShearUse	= max(pGirCol->m_GirShear[nUD][RCBRIDGE_GIR_POS_END][eEnvService],pGirCol->m_GirShear[nUD][RCBRIDGE_GIR_POS_MID][eEnvService]);
				pConc->m_MomentUse	= pConc->m_Moment;
				pConc->m_MomentCrack	= pConc->m_Moment;
			}
		
			pConc->m_Ln     = frM(min(pGirCol->m_dSpanLengthGirder[RCBRIDGE_GIR_POS_END],pGirCol->m_dSpanLengthGirder[RCBRIDGE_GIR_POS_MID]));
			if(pConc->m_Ln <= 0) pConc->m_Ln = frM(max(pGirCol->m_dSpanLengthGirder[RCBRIDGE_GIR_POS_END],pGirCol->m_dSpanLengthGirder[RCBRIDGE_GIR_POS_MID]));

			long nRebarDan=0; for(nRebarDan=0; nRebarDan<MAX_REBAR_DAN; nRebarDan++)
			{
				pConc->m_RbT1C_DIA[nRebarDan] = pRebar->m_dDiaUL[iUPPER][nRebarDan];
				pConc->m_RbT1C_dc[nRebarDan]  = pRebar->m_dCoverUpper[nRebarDan];
				pConc->m_RbT1C_B [nRebarDan]  = pRebar->m_nB[iUPPER][nRebarDan] + 1;
				if(!Compare(pRebar->m_dC1[iUPPER][nRebarDan], 0.0, "=", 0)) pConc->m_RbT1C_B [nRebarDan]++;
				if(!Compare(pRebar->m_dC2[iUPPER][nRebarDan], 0.0, "=", 0)) pConc->m_RbT1C_B [nRebarDan]++;
				pConc->m_SvDia = pRebar->m_dDiaStirrup;
				pConc->m_SvCtc = pRebar->m_dCTC;
				pConc->m_SvEa  = pRebar->m_nCountStirrup*2;
				pConc->m_ShDia = pRebar->m_dDiaSide;
				pConc->m_ShCtc = (pRebar->m_dF==0) ? max(pRebar->m_dF1,pRebar->m_dF2) : pRebar->m_dF;
				pConc->m_ShEa  = pRebar->m_nCountStirrup*2;
			}
			// 거더 중앙부
			pConc = pGirCol->pConcGirMid[nUD][ixEnvType];
			SetConcData_BaseInfo(pConc, TRUE, bUpperSlabGirder, FALSE);
			if(pGirCol->m_nJijum == 0 || pGirCol->m_nJijum == pBri->m_nQtyJigan)
			{
				pConc->m_sTitle.Format("%s 중앙부 : %s", strGirder, (pGirCol->m_nJijum)? "시점" : "종점");
			}
			else pConc->m_sTitle.Format("%s 중앙부 : 중간지점 %d",strGirder, pGirCol->m_nJijum);
			pConc->m_Bo = pBri->GetGirderWidth(pGirCol->m_nJijum, bUpperSlabGirder);
			pConc->m_Ho = pBri->GetGirderHeight(pGirCol->m_nJijum, bUpperSlabGirder);
			pConc->m_Moment = max(pGirCol->m_GirMomentPlus[nUD][RCBRIDGE_GIR_POS_END][ixEnvType],pGirCol->m_GirMomentPlus[nUD][RCBRIDGE_GIR_POS_MID][ixEnvType]);
			pConc->m_Shear  = 0;
			if(bLsd && eEnvType == eEnvLimit)
			{
				// 극한한계상태 - 상시
				pConc->m_ShearUse	= 0;
				pConc->m_MomentUse	= max(pGirCol->m_GirMomentPlus[nUD][RCBRIDGE_GIR_POS_END][eEnvService],pGirCol->m_GirMomentPlus[nUD][RCBRIDGE_GIR_POS_MID][eEnvService]);
				pConc->m_MomentCrack	= max(pGirCol->m_GirMomentPlus[nUD][RCBRIDGE_GIR_POS_END][eEnvService_Crack],pGirCol->m_GirMomentPlus[nUD][RCBRIDGE_GIR_POS_MID][eEnvService_Crack]);
			}
			else
			{
				pConc->m_ShearUse	= 0;
				pConc->m_MomentUse	= pConc->m_Moment;
				pConc->m_MomentCrack	= pConc->m_Moment;
			}

			pConc->m_Ln     = frM(min(pGirCol->m_dSpanLengthGirder[RCBRIDGE_GIR_POS_END],pGirCol->m_dSpanLengthGirder[RCBRIDGE_GIR_POS_MID]));
			for(nRebarDan=0; nRebarDan<MAX_REBAR_DAN; nRebarDan++)
			{
				pConc->m_RbT1C_DIA[nRebarDan] = pRebar->m_dDiaUL[iLOWER][nRebarDan];
				pConc->m_RbT1C_dc[nRebarDan]  = pRebar->m_dCoverLower[nRebarDan];
				pConc->m_RbT1C_B [nRebarDan]  = pRebar->m_nB[iLOWER][nRebarDan] + 1;
				if(!Compare(pRebar->m_dC1[iLOWER][nRebarDan], 0.0, "=", 0)) pConc->m_RbT1C_B [nRebarDan]++;
				if(!Compare(pRebar->m_dC2[iLOWER][nRebarDan], 0.0, "=", 0)) pConc->m_RbT1C_B [nRebarDan]++;
			}
		}

		// 기둥검토 단면
		if (bLsd)
		{
			if(pBri->IsWall(pGirCol->m_nJijum) == FALSE)
				continue;

			const HGBOOL bStt = pGirCol->m_nJijum==0;
			for (HGINT32 ix = 0; ix < eCountEnvTypeLsd; ++ix)
			{
				if(pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(ix)) == FALSE)
					continue;

				long nIdxConc = (ix == eEnvService_Crack)? eEnvService : ix;

				CConcBasic *pFirstConc = NULL;
				for (HGINT32 jx = 0; jx < RCBRIDGE_COL_CHECK_LSD; ++jx)
				{
					CConcBasic *pConc = pGirCol->pConcColumnLsd[nIdxConc][jx];
					pConc->m_nLsdComboKind = (ix == eEnvExtremeI) ? eLSD_COMBINE_EXTREME_I : -1;
					if (pGirCol->m_nJijum==0 || pGirCol->m_nJijum==pBri->m_nQtyJigan)
					{
						pBri->SetConcDataOutWall(pConc, static_cast<ETypeEnvComboLsd>(ix), bStt ? 0 : 5);
					}
					else
					{
						CWallApp   *pWall   = pBri->GetInWall(pGirCol->m_nJijum-1);
						if (pWall == NULL)
							continue;

						SetConcDataInWall(pConc, pGirCol, pWall, static_cast<ELSDLoadGroupKind>(ix));
					}

					double Moment = 0;
					double Shear = 0;
					double Axial = 0;
					long nIdxModel = 0;

					pBri->GetOutValueForWallDesing_Lsd(pGirCol->m_nJijum, static_cast<ETypeEnvComboLsd>(ix), static_cast<EWallCheckLsd>(jx), Moment, Shear, Axial, nIdxModel, TRUE);

					pConc->m_Mux = GetValueUnitChange(Moment, pData->m_szTonfM, UNIT_CGS_TONFM);
					pConc->m_Shear = GetValueUnitChange(Shear, pData->m_szTonf, UNIT_CGS_TONF);
					pConc->m_Axial = GetValueUnitChange(Axial, pData->m_szTonf, UNIT_CGS_TONF);

					if (jx == 0)
					{
						pFirstConc = pConc;
						pFirstConc->m_xyArrPM.RemoveAll();
					}

					if (pFirstConc)
					{
						CDPoint xyPM(0,0);
						
						xyPM.x = pConc->m_Mux;
						xyPM.y = pConc->m_Axial;
						pFirstConc->m_xyArrPM.Add(xyPM);
					}
				}
			}
		}
		else
		{
			if(pGirCol->m_nJijum==0 || pGirCol->m_nJijum==pBri->m_nQtyJigan) continue;

			//////////////////////////////////////////////////////////////////////////
			// 기둥,중간벽체
			CWallApp   *pWall   = pBri->GetInWall(pGirCol->m_nJijum-1);
			if(pWall==NULL || (pGirCol->m_nJijum ==0 || pGirCol->m_nJijum == pBri->m_nQtyJigan)) continue;

			long nCheckSize = pBri->IsApplyEarthQuakeDesign() ? RCBRIDGE_COL_CHECK : RCBRIDGE_COL_CHECK - 1; // 지진시 미적용
			for(long nCheck=0; nCheck<nCheckSize; nCheck++)
			{
				CConcBasic *pConc = pGirCol->pConcColumn[nCheck];
				SetConcDataInWall(pConc, pGirCol, pWall);

				pConc->m_Mux    = pGirCol->m_dColMoment[nCheck];
				//			pConc->m_Moment = pGirCol->m_dColMoment[nCheck];
				pConc->m_Axial  = pGirCol->m_dColAxial[nCheck];
				pConc->m_Shear	= pGirCol->m_dColShear[nCheck];
				if(nCheck==0)
				{
					long nCheckSize = pBri->IsApplyEarthQuakeDesign() ? RCBRIDGE_COL_CHECK : RCBRIDGE_COL_CHECK - 1; // 지진시 미적용

					CDPoint xyPM(0,0);
					long k=0; for(k=0; k<nCheckSize; k++)
					{
						xyPM.x = pGirCol->m_dColMoment[k];
						xyPM.y = pGirCol->m_dColAxial[k];
						pConc->m_xyArrPM.Add(xyPM);
					}
				}
			}
		}
		
//		// 하부거더 지점부 (기초)
//		pConc = pGirCol->pConcGirLowerEnd;
//		SetConcData_BaseInfo(pConc);
//		pConc->m_sTitle.Format("하부거더 지점부 : 중간기초 %d",pGirCol->m_nJijum);
//		// 하부거더 중앙부 (기초)
//		pConc = pGirCol->pConcGirLowerMid;
//		SetConcData_BaseInfo(pConc);
//		pConc->m_sTitle.Format("하부거더 중앙부 : 중간기초 %d",pGirCol->m_nJijum);
	}
}

void CARcBridgeCalcColAndGir::SetConcDataInWall(CConcBasic *pConc, CGirderAndColumn *pGirCol, CWallApp *pWall, ELSDLoadGroupKind eGroupKind)
{
	if (pConc == NULL)
		return;
	
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CColumnApp *pColApp = &pWall->m_Column;

	SetConcData_BaseInfo(pConc, FALSE, FALSE, FALSE, eGroupKind);
	pConc->m_nTypeSection = SECTION_PILLAR;
	if(pBri->GetTypeColumnByJijum(pGirCol->m_nJijum)==0) pConc->m_nTypeSection = SECTION_COLUMN;
	if(pBri->GetTypeColumnByJijum(pGirCol->m_nJijum)==1) pConc->m_nTypeSection = SECTION_POST;

	if(pGirCol->m_bColumn)
	{
		pConc->m_sTitle.Format("기둥 : 중간지점 %d",pGirCol->m_nJijum);
		// 기둥과 부재력의 방향이 다르므로 Bo, Ho가 바뀐다.
		pConc->m_Bo = pColApp->m_dH[iOUTTER][iUPPER];
		pConc->m_Ho = pColApp->m_dB[iOUTTER][iUPPER];
		pConc->m_Ro = pColApp->m_dR[iOUTTER][iUPPER];
		if(pColApp->m_nType==COL_TYPE_CIRCLE)
		{
			pConc->m_Bo = 0;
			pConc->m_Ho = 0;
		}
		else if(pColApp->m_nType==COL_TYPE_RECTANGLE)
		{
			pConc->m_Ro = 0;
		}
		else if(pColApp->m_nType==COL_TYPE_TRACK)
		{
			pConc->m_Bo = pColApp->m_dH[iOUTTER][iUPPER] - pColApp->m_dR[iOUTTER][iUPPER]*2;
			pConc->m_Ho = pColApp->m_dB[iOUTTER][iUPPER] - pColApp->m_dR[iOUTTER][iUPPER]*2;
			pConc->m_Ro = pColApp->m_dR[iOUTTER][iUPPER];
		}
	}
	else
	{
		// [내측벽체 단면 검토] 옵션이 슬래브인 경우 SetConcDataWall() 에서 만든다(8104)
		if(pBri->m_nSelectInWallSectionDesign == 0)
			return;

		pConc->m_nTypeSection = SECTION_WALL;
		pConc->m_sTitle.Format("중간벽체 : 중간지점 %d",pGirCol->m_nJijum);
		pConc->m_Bo = frM(UNIT_METER);
		pConc->m_Ho = pWall->m_dW;
	}
	
	if(pGirCol->m_bColumn)
	{
		long nDanMain = 0;

		for(HGINT32 nRebarDan=0; nRebarDan<MAX_REBAR_DAN; nRebarDan++)
		{
			pConc->m_RbT1C_DIA[nRebarDan] = pColApp->m_dDiaMain[iOUTTER][nRebarDan];
			pConc->m_RbT1C_dc[nRebarDan]  = pColApp->m_dCover[iOUTTER][nRebarDan];
			pConc->m_RbT1C_RE[nRebarDan]  = pColApp->m_nR[iOUTTER][nRebarDan];

			if(pConc->m_RbT1C_DIA[nRebarDan] > 0)
			{
				pConc->m_RbT1C_E [nRebarDan]  = pColApp->m_nB[iOUTTER][nRebarDan]+1;	// 왜 +2를 했었삼~? 갯수가 하나 더 나오잖아,,
				if(pColApp->m_dC1[iOUTTER][nRebarDan]>0) pConc->m_RbT1C_E [nRebarDan]++;
				if(pColApp->m_dC2[iOUTTER][nRebarDan]>0) pConc->m_RbT1C_E [nRebarDan]++;

				pConc->m_RbT1C_B [nRebarDan]  = pColApp->m_nE[iOUTTER][nRebarDan]+1;	// 왜 +2를 했었삼~? 갯수가 하나 더 나오잖아,,
				if(pColApp->m_dF1[iOUTTER][nRebarDan]>0) pConc->m_RbT1C_B[nRebarDan]++;
				if(pColApp->m_dF2[iOUTTER][nRebarDan]>0) pConc->m_RbT1C_B[nRebarDan]++;

				++nDanMain;
			}
		}
		pConc->m_SvDia = pColApp->GetDiaRebarRound(FALSE, 0, FALSE);
		pConc->m_SvCtc = pColApp->GetCTCRoundRebar(FALSE, 0);		// (ARCBRIDGE-2864) 간격은 제일 큰간격으로 적용
		pConc->m_SvEa  = nDanMain * 2;
	}
	else
	{
		CDPoint vAngleJijum = (pBri->m_nTypePlacingMainRebar_UpperSlab==0) ? pBri->GetAngleJijum(pGirCol->m_nJijum) : CDPoint(0,1);
		double dCtc = pBri->GetCtcRebarMain(TRUE);

		long nCycle1=0,nCycle2=1;
		for(HGINT32 nRebarDan=0; nRebarDan<MAX_REBAR_DAN; nRebarDan++)
		{
			BOOL bExistRebar = (pWall->IsExistMainRebar(nCycle1, nRebarDan, TRUE, 0));
			pConc->m_RbT1C_DIA[nRebarDan] = (bExistRebar)? pWall->GetDiaMainRebar(nCycle1, nRebarDan, TRUE, 0) : 0;
			pConc->m_RbT1C_dc[nRebarDan]  = (bExistRebar)? pBri->m_dCoverWallMid[nRebarDan] : 0;
			pConc->m_RbT1C_B [nRebarDan]  = (bExistRebar)? Round(pConc->m_Bo/(dCtc/vAngleJijum.y),3) : 0;

			bExistRebar = (pWall->IsExistMainRebar(nCycle2, nRebarDan, TRUE, 0));
			pConc->m_RbT2C_DIA[nRebarDan] = (bExistRebar)? pWall->GetDiaMainRebar(nCycle2, nRebarDan, TRUE, 0) : 0;
			pConc->m_RbT2C_dc[nRebarDan]  = (bExistRebar)? pConc->m_RbT1C_dc[nRebarDan]	: 0;
			pConc->m_RbT2C_B [nRebarDan]  = (bExistRebar)? Round(pConc->m_Bo/(dCtc/vAngleJijum.y),3) : 0;
			if(pConc->m_RbT1C_DIA[nRebarDan]==pConc->m_RbT2C_DIA[nRebarDan] && bExistRebar)
			{
				pConc->m_RbT1C_B [nRebarDan] += pConc->m_RbT2C_B [nRebarDan];
				pConc->m_RbT2C_B [nRebarDan]  = 0;
			}
			//
			bExistRebar = (pWall->IsExistMainRebar(nCycle1, nRebarDan, FALSE, 0));
			pConc->m_RbC1C_DIA[nRebarDan] = (bExistRebar)? pWall->GetDiaMainRebar(nCycle1, nRebarDan, FALSE, 0) : 0;
			pConc->m_RbC1C_dc[nRebarDan]  = (bExistRebar)? pBri->m_dCoverWallMid[nRebarDan] : 0;
			pConc->m_RbC1C_B [nRebarDan]  = (bExistRebar)? Round(pConc->m_Bo/(dCtc/vAngleJijum.y),3) : 0;

			bExistRebar = (pWall->IsExistMainRebar(nCycle2, nRebarDan, FALSE, 0));
			pConc->m_RbC2C_DIA[nRebarDan] = (bExistRebar)? pWall->GetDiaMainRebar(nCycle2, nRebarDan, FALSE, 0) : 0;
			pConc->m_RbC2C_dc[nRebarDan]  = (bExistRebar)? pConc->m_RbC1C_dc[nRebarDan] : 0;
			pConc->m_RbC2C_B [nRebarDan]  = (bExistRebar)? Round(pConc->m_Bo/(dCtc/vAngleJijum.y),3) : 0;
			if(pConc->m_RbC1C_DIA[nRebarDan]==pConc->m_RbC2C_DIA[nRebarDan] && bExistRebar)
			{
				pConc->m_RbC1C_B [nRebarDan] += pConc->m_RbC2C_B [nRebarDan];
				pConc->m_RbC2C_B [nRebarDan]  = 0;
			}
		}

		// 배력철근
		CDPoint xyInfo;
		for(long dan=0; dan<MAX_REBAR_DAN; dan++)
		{
			xyInfo = pBri->GetInfoSupportRebar_Wall(TRUE, pGirCol->m_nJijum, dan, -1);
			pConc->m_CSupportBarDia[dan] = xyInfo.x;
			pConc->m_CSupportBarCTC[dan] = xyInfo.y;
			xyInfo = pBri->GetInfoSupportRebar_Wall(TRUE, pGirCol->m_nJijum, dan, -1);
			pConc->m_TSupportBarDia[dan] = xyInfo.x;
			pConc->m_TSupportBarCTC[dan] = xyInfo.y;
		}

		// 전단철근
		pConc->m_SvDia = pWall->m_rebarShearDetail.m_dDia;
		pConc->m_SvCtc = pWall->GetCTCRebarShear();// m_dCTC_ShearRebar;
		pConc->m_SvEa  = 1000 / (pWall->m_nCountWrapMainRebar * pWall->m_dCTC_MainRebar / 2);

		pConc->m_nCheckSupportBarRate = SUPPORTBAR_CHECK_WALL;
		pConc->m_bCheckSuppoerBarAreaRate = pBri->m_bCheckSupportWallDryingShrinkage;
		pConc->m_bCheckSupportBarMainRate20 = pBri->m_bCheckSupportWallMainRate;

		pConc->m_bMinAvCheck   = pBri->m_bApplyMinAvWall;
		pConc->m_nEnvTypeCrack = pBri->m_nTypeCrackDesignWall;
	}
}

void CARcBridgeCalcColAndGir::MakeTableColumnLoad(CXLFormatText *XLText)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nJijumConnect = pBri->GetJijumConnectRibAndSlab();
	long nSizeJijum = pBri->GetCountJijum(); 
	long nCntCol  = 8;
	long nCheckSize = pBri->IsApplyEarthQuakeDesign() ? RCBRIDGE_COL_CHECK : RCBRIDGE_COL_CHECK - 1; // 지진시 미적용
	long nCntRow  = (nSizeJijum - 2) * nCheckSize + 1;
	if(nJijumConnect > -1) 
	{
		nCntRow -= nCheckSize;
	}

	XLText->AddTable(nCntRow-1, 24-1, 1, 2);
	//////////////////////////////////////////////////////////////////////////
	// Title
	int nWidthTitle[] = {  6, 3, 3, 3, 3, 3, 3};
	int nWidthColumn[] = { 3, 3, 3, 3, 3, 3, 3, 3};
	
	char* Title[]  = { "구 분", "축력/m", "모멘트/m", "C.T.C", "축력", "모멘트", "비 고"};
	char* szCheck[] = { "축력최대","모멘트최대", "편심최대","지진시"};
	
	int i = 0; for(i = 0; i<nCntCol-1; i++)
	{
		XLText->AddMergeFormatText(0,nWidthTitle[i]-1,"$c%s", Title[i]);
		XLText->AddFormatText("$m+0%d",nWidthTitle[i]);
	}

	double Pu_m=0,Mu_m=0,Pu=0,Mu=0,CTC;

	long nIdxDesign = 0;
	CString strCombo=_T(""),str=_T("");
	long nJijum=0; 
	for(nJijum=0; nJijum<nSizeJijum; nJijum++)
	{
		CJijum *pJijum = pBri->GetJijum(nJijum);
		//long nVerPos  = (nJijum==0) ? 1 : 0;
		//BOOL bShoePos = (pJijum->m_nQtyShoe[nVerPos]>0) ? TRUE : FALSE;
		if((nJijum==0 || nJijum==pBri->m_nQtyJigan) && TRUE /*bShoePos==FALSE*/)
		{
			nIdxDesign++;
			continue;
		}
		if(nJijum == nJijumConnect) continue;

		CGirderAndColumn *pGirCol = m_pList.GetAt(nIdxDesign);
		if(!(pGirCol->m_bColumn || pGirCol->m_bWall))
		{
			nIdxDesign++;
			continue;
		}
		long j=0; 
		for(j=0; j<nCheckSize; j++)
		{
			Pu_m = pGirCol ? GetValueUnitChange(pGirCol->m_dAxialBot[j],UNIT_CGS_TONF,pData->m_szTonf)  : 0;
			Mu_m = pGirCol ? GetValueUnitChange(pGirCol->m_dMomentBot[j],UNIT_CGS_TONF,pData->m_szTonf) : 0;
			CTC  = pGirCol ? pGirCol->m_dCtcColumn    : 0;
			Pu   = pGirCol ? GetValueUnitChange(pGirCol->m_dColAxial[j],UNIT_CGS_TONF,pData->m_szTonf)  : 0;
			Mu   = pGirCol ? GetValueUnitChange(pGirCol->m_dColMoment[j],UNIT_CGS_TONF,pData->m_szTonf) : 0;
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
				if(k==0 && j==0) str.Format("중간지점 %d",nJijum);
				if(k==1) str.Format("%s",szCheck[j]);
				if(k==2) str.Format("$f3%.3f", Pu_m);
				if(k==3) str.Format("$f3%.3f", Mu_m);
				if(k==4) str.Format("$f3%.3f", CTC);
				if(k==5) str.Format("$f3%.3f", Pu);
				if(k==6) str.Format("$f3%.3f", Mu);
				if(k==7) str = strCombo;

				if(k==0)	XLText->AddText("$n$h");
				if(k==0 && j==0) 
				{
					if(pBri->IsApplyEarthQuakeDesign())
						XLText->AddText("$md3");
					else
						XLText->AddText("$md2");
				}
				XLText->AddMergeFormatText(0, nWidthColumn[k]-1,"$c%s",str);
				XLText->AddFormatText("$m+0%d", nWidthColumn[k]);
			}
		}
		nIdxDesign++;
	}
}

void CARcBridgeCalcColAndGir::MakeTableColumnLoadLsd(CXLFormatText *XLText)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	const HGBOOL bLsd = pData->IsLsdDesign();

	CString str1;
	HGINT32 nIdxCircle = 0;
	//////////////////////////////////////////////////////////////////////////
	// 3. 시점측벽체 기초검토용 하중	4. 종점측 벽체 기초검토용 하중
	if(pBri->IsOutsideWall() && pBri->m_nSelectWallSectionDesign == 1)
	{
		for(long ix=0; ix<2; ix++)
		{
			str1 = (ix==0)? "시점측":"종점측";
			XLText->GoPosition(NO_MOVE, COL_POS_1);
			XLText->AddFormatText(_T("$h%s$r%s 벽체$n$h"), GetNumberString(++nIdxCircle, NUM_CIRCLE), str1);

			MakeTableLoadForWallCheck_Lsd(XLText, pBri, (ix==0)? 0 : pBri->m_nQtyJigan);
			*XLText += 2;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 5. 교각(n)기초검토용 하중 ~
	if (bLsd == FALSE || (bLsd && pBri->m_nSelectInWallSectionDesign == 1))
	{
		long nCntInWall = pBri->GetCountInWallReal();
		for(long ix=0; ix<nCntInWall; ix++)
		{
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH) break;
			str1.Format("중간벽체(중간지점%d)", ix+1);

			XLText->GoPosition(NO_MOVE, COL_POS_1);
			XLText->AddFormatText(_T("$h%s$r%s$n$h"), GetNumberString(++nIdxCircle, NUM_CIRCLE), str1);

			MakeTableLoadForWallCheck_Lsd(XLText, pBri, ix+1);
			*XLText += 2;
		}
	}
}

void CARcBridgeCalcColAndGir::DrawColumnSection(CXLFormatText *XLText)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM = &pBri->m_FemNormal;

	double ScaleDim = 50;

	long nIdxSmall = 1;
	long nJijum=0; for(nJijum=0; nJijum<pBri->GetCountJijum(); nJijum++)
	{
		if(pBri->GetTypeColumnByJijum(nJijum)==-1) continue;

		CWallApp *pWall = pBri->GetInWall(nJijum-1);
		CColumnApp *pColumn = &pWall->m_Column;

		CDomyun *pDom = new CDomyun;
		pDom->SetCWnd(m_pStd->m_pView);
		pDom->SetScaleDim(ScaleDim);
		pDom->SetLTScale(ScaleDim*5);

		XLText->GoPosition(NO_MOVE ,COL_POS_2);
		XLText->AddFormatText("$h$c%s$r중간지점 %d$n",GetNumberString(nIdxSmall++,NUM_CIRCLE),nJijum);

		long nSizeRow = 13;
		CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, pData);
		DBStd.m_pDrawColumn->DrawRebar_PlaneStd(pDom, pWall, TRUE, TRUE, TRUE, FALSE);
		XLText->InsertDomyun(pDom,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);
	}
}

BOOL CARcBridgeCalcColAndGir::IsExistColumn()
{
	long nSize = m_pList.GetSize();
	long i=0; for(i=0; i<nSize; i++)
	{
		CGirderAndColumn *pGirCol = m_pList.GetAt(i);
		if(pGirCol->m_bColumn) return TRUE;
	}

	return FALSE;
}

void CARcBridgeCalcColAndGir::SetConcDataJongGir()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long n=0; for(n=0; n<6; n++)
	{
		BOOL bStt	= (n>3)? FALSE : TRUE;
		BOOL bLeft	= (n%2==0)? TRUE : FALSE;
		BOOL bGagak	= (n<2)? FALSE : TRUE;		//pBri->IsGagakForLine(bStt, bLeft);

		CGirderSectionRebar *pGirderRB	= pBri->GetGirderRebarJong(bStt, bLeft, bGagak);
		if(!pGirderRB->m_bIs || (bGagak && !pBri->IsGagakForLine(bStt, bLeft)))
		{
			CConcBasic *pConc = new CConcBasic;
			m_pArrConcJongGir.Add(pConc);
			continue;
		}

		CString szStt = bGagak? (bStt? _T("시점") : _T("종점")) : _T("");
		CString szDir = bLeft? _T("좌측") : _T("우측");
		CString szGagak = bGagak? _T("가각") : _T("");
		CConcBasic *pConc = new CConcBasic;
		SetConcData_BaseInfo(pConc, TRUE, TRUE, FALSE);

		pConc->m_sTitle.Format("%s", szStt+szDir+szGagak);
		pConc->m_Bo = pGirderRB->m_dC1[iLOWER][0] + pGirderRB->m_nB[iLOWER][0]*pGirderRB->m_dC[iLOWER][0] + pGirderRB->m_dC2[iLOWER][0] + pGirderRB->m_dDist*2;
		pConc->m_Ho = pBri->m_dTS;

		pConc->m_Moment = GetMomentJongGirder(bStt, bLeft, bGagak);
		pConc->m_Shear  = 0;
		pConc->m_Ln     = GetLenSpanJongGirder(bStt, bLeft, bGagak);

		long nRebarDan=0; for(nRebarDan=0; nRebarDan<MAX_REBAR_DAN; nRebarDan++)
		{
			pConc->m_RbT1C_DIA[nRebarDan] = pGirderRB->m_dDiaUL[iLOWER][nRebarDan];
			pConc->m_RbT1C_dc[nRebarDan]  = pGirderRB->m_dCoverLower[nRebarDan];
			pConc->m_RbT1C_B [nRebarDan]  = pGirderRB->m_nB[iLOWER][nRebarDan] + 1;
			if(pGirderRB->m_dC1[iLOWER][nRebarDan]>0) pConc->m_RbT1C_B [nRebarDan]++;
			if(pGirderRB->m_dC2[iLOWER][nRebarDan]>0) pConc->m_RbT1C_B [nRebarDan]++;
			pConc->m_SvDia = pGirderRB->m_dDiaStirrup;
			pConc->m_SvCtc = pGirderRB->m_dCTC;
			pConc->m_SvEa  = pGirderRB->m_nCountStirrup*2;
			pConc->m_ShDia = pGirderRB->m_dDiaSide;
			pConc->m_ShCtc = (pGirderRB->m_dF==0) ? max(pGirderRB->m_dF1,pGirderRB->m_dF2) : pGirderRB->m_dF;
			pConc->m_ShEa  = pGirderRB->m_nCountStirrup*2;
		}

		m_pArrConcJongGir.Add(pConc);
	}
}

double CARcBridgeCalcColAndGir::GetLenSpanJongGirder(BOOL bStt, BOOL bLeft, BOOL bGagak)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	double dLenGirder = 0;

	if(bGagak)
	{
		CString szInfo = _T("");
		CString szStt = bStt? _T("시점") : _T("종점");
		CString szDir = bLeft? _T("좌측") : _T("우측");
		szInfo.Format("%s(%s%s가각부)", szDir, szStt, szDir);
		CTwinVectorArray tvArr;
		pBri->GetTvRebarOut_Plane_UpperSlab(tvArr, 0, TRUE, bLeft? iLEFT:iRIGHT);
		CTwinVector tv =  tvArr.GetTvByInfo(szInfo);
		dLenGirder = tv.GetLength();
	}
	else
	{
		long n=0; for(n=0; n<pBri->m_nQtyJigan; n++)
		{
			double dLen = pBri->GetLengthJigan(n, bLeft);
			dLenGirder = max(dLen, dLenGirder);
		}
	}
	
	return dLenGirder;
}

double CARcBridgeCalcColAndGir::GetMomentJongGirder(BOOL bStt, BOOL bLeft, BOOL bGagak)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	double dMoment = 0;
	double dFeelLoad = m_pStd->GetWheelLoadRear();
	double dLenGirder = toM(GetLenSpanJongGirder(bStt, bLeft, bGagak));

	long nCombo = 0;
	double dFactorLive = 0;
	if(pData->IsLsdDesign())
	{
		CLsdLoadCase *pLoadCase = pBri->m_plsdManager->GetLsdLoadCase(eLSD_COMBINE_LIMIT_I, eLSD_LOADCASE_DIR_BRI, 0);
		if(pLoadCase)
		{
			dFactorLive = pLoadCase->GetLoadFactor(eLSD_LOADCASE_LL1);
		}
	}
	else
	{
		dFactorLive = pBri->GetLoadFactor(LOAD_COMBINE_ULT, nCombo, BRIDGE_LOAD_LIVE_FULL);
	}

	if(pBri->m_nQtyJigan == 1 || bGagak)
	{
		dMoment = 0.10 * dFeelLoad * dLenGirder;
	}
	else
	{
		dMoment = 0.08 * dFeelLoad * dLenGirder;
	}

	return dMoment * dFactorLive;
}

void CARcBridgeCalcColAndGir::DrawJongGirderSection(CXLFormatText *XLText, BOOL bStt, BOOL bLeft, BOOL bGagak)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM = &pBri->m_FemNormal;
	
	double ScaleDim = 50;
	
	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(ScaleDim);
	pDom->SetLTScale(ScaleDim*5);
	
	long nSizeRow = 15;
	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, pData);

	// 종단면 거더부 상세 그리기
	DBStd.m_pDrawVertSection->DrawDetail_Girder(pDom, 0, bStt, bLeft, bGagak, TRUE);
	
	// 스트럽 철근 관련 그리기
	DBStd.m_pDrawVertSection->DrawRebarAll_SlabGirder(pDom, 0, TRUE, TRUE, 0, FALSE, FALSE, bStt, bLeft, bGagak, TRUE);

	XLText->InsertDomyun(pDom,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_CURPOS|CXLFormatText::INSERT_DOM_MOVEROW);
}

BOOL CARcBridgeCalcColAndGir::IsExistJongGirder()
{
	BOOL bExist = FALSE;
	long nSize = m_pArrConcJongGir.GetSize();
	long i=0; for(i=0; i<nSize; i++)
	{
		CConcBasic *pConc = m_pArrConcJongGir.GetAt(i);
		if(pConc->IsValidData())
		{
			bExist = TRUE;
			break;
		}
	}
	
	return bExist;
}

void CARcBridgeCalcColAndGir::MakeTableLoadForWallCheck_Lsd( CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum )
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	//	CString szUnit_Moment = _T("");
	//	CString szUnit_Force = _T("");
	CString str1, str2;

	CString szUnit_Moment = pData->GetStringUnitType(UNIT_CGS_TONFM);
	CString szUnit_Force  = pData->GetStringUnitType(UNIT_CGS_TONF);
	EPartOfBridge ePart = (nJijum == 0)? ePartOutWallStt : ((nJijum == pBri->m_nQtyJigan)? ePartOutWallEnd : ePartInWall);

	int nWidthJoint1[] = { 5, 2, 4, 4, 4, 6 };
	int nWidthJoint2[] = { 7, 4, 4, 4, 6 };
	CString strTemp2 = "수평력("+szUnit_Force+")";
	CString strTemp  = "수직력("+szUnit_Force+")";
	CString strTemp1 = "모멘트("+szUnit_Moment+")";
	char *Title2[] = { "구 분", strTemp.GetBuffer(strTemp.GetLength()), strTemp1.GetBuffer(strTemp1.GetLength()), strTemp2.GetBuffer(strTemp2.GetLength()), "비 고" };

	long nCountEnvType(0);
	for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
	{
		ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
		if(pBri->IsValidComboEnvType(eEnvType) == FALSE)
			continue;
		// 사용한계상태는 적용안함.
		// (ARCBRIDGE-3618) 사용성검토 할때는 사용한계상태 집계필요. - Baem검토를 집계하지 않고 기둥집계로 사용성검토를 하도록 요청.
		if((eEnvType == eEnvService || eEnvType == eEnvService_Crack) && pBri->m_bOutWallUsebilityCheck == FALSE)
			continue;
		
		++nCountEnvType;
	}

	// table의 전체 row수

	long nCntRow = nCountEnvType * 5 * 2 + 1;
	XLText->AddTable(nCntRow-1, 25-1, 1, 6);

	// Title
	long j=0; for(j=0; j<5; j++)
	{
		XLText->AddMergeFormatText(0,nWidthJoint2[j]-1,"$c%s",Title2[j]);
		XLText->AddFormatText("$m+0%d",nWidthJoint2[j]);
	}
	XLText->AddText("$n$h");

	CString str = _T("");
	CString strOpt = _T("");
	double Moment = 0;
	double Shear = 0;
	double Axial = 0;
	long nIdxModel = 0;

	// data
	double nTypeJongOld = pBri->m_nTypeJongdanForRebar;
	BOOL bArch  = (pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH);
	pBri->GetTvVertSection_Std(0, FALSE, FALSE, FALSE, bArch? 1 : 0);

	double dCTC(1000);
	if(nJijum > 0 && nJijum < pBri->m_nQtyJigan)
	{
		dCTC = frM(pBri->GetCtcColumn(nJijum));
	}

	for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
	{
		ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
		if(pBri->IsValidComboEnvType(eEnvType) == FALSE)
			continue;

		if((eEnvType == eEnvService || eEnvType == eEnvService_Crack) && pBri->m_bOutWallUsebilityCheck == FALSE)
			continue;

		ELSDCombKind eKindCombo = pBri->GetComboKindByEnvType(eEnvType, ePart, CHECK_POS_CENTER);

		for(long iW=0; iW<2; ++iW)
		{
			for(long iCheck=0; iCheck<eCountWallCheck; ++iCheck)
			{
				long nCombo = pBri->GetOutValueForWallDesing_Lsd(nJijum,eEnvType, (EWallCheckLsd)iCheck,Moment,Shear,Axial, nIdxModel, (iW == 1));
				CString szCombo = pBri->GetStringComboEta_Lsd(eKindCombo, ePart, nCombo + 1, TRUE, nIdxModel);

				for(j=0; j<6; j++)
				{
					if(j == 0) str = pBri->GetStringComboEnvType_Lsd(eEnvType, TRUE, TRUE);
					if(j == 1) str = (iW == 0)? _T("단면력") : (_T("$jCTC\n") + CommaC(dCTC));
					if(iW == 0)
					{
						if(j == 2) str.Format("$f3%.3f",Axial);
						if(j == 3) str.Format("$f3%.3f",Moment);
						if(j == 4) str.Format("$f3%.3f",Shear);
					}
					else
					{
						if(j == 2) str.Format("%s", pData->GetStrDouble(Axial, UNIT_CGS_TONF));
						if(j == 3) str.Format("%s", pData->GetStrDouble(Moment, UNIT_CGS_TONF));
						if(j == 4) str.Format("%s", pData->GetStrDouble(Shear, UNIT_CGS_TONF));
					}
					if(j == 5) 
					{
						str = pBri->GetStringWallCheckCase((EWallCheckLsd)iCheck) + _T(" (") + szCombo + _T(")");
					}
					if(j==0 && iCheck == 0 && iW == 0) 
						strOpt = " $md9";
					else if(j==1 && iCheck == 0)
						strOpt = " $md4";
					else 
						strOpt = "";
					XLText->AddMergeFormatText(0, nWidthJoint1[j]-1, "$c%s%s", str, strOpt);
					XLText->AddFormatText("$m+0%d",nWidthJoint1[j]);
				}
				XLText->AddText("$n$h");
			}
		}
	}
	*XLText += 2;

	pBri->m_nTypeJongdanForRebar = (long)nTypeJongOld;
	pBri->m_nTypeJongdanForRebarOld = (long)nTypeJongOld;
	pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, (long)nTypeJongOld);
	//	pBri->GetTvVertSection_Std(0, FALSE, FALSE, TRUE);
}

void CARcBridgeCalcColAndGir::WriteCheckSection(CXLFormatText *XLText, CRcBridgeRebar *pBri)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CString strCombo=_T(""),str=_T("");
	long nSizeJijum = pBri->GetCountJijum(); 
	long nIdxDesign = 0;
	long nJijumConnect = pBri->GetJijumConnectRibAndSlab();
	long nCheckSize = pBri->IsApplyEarthQuakeDesign() ? RCBRIDGE_COL_CHECK : RCBRIDGE_COL_CHECK - 1; // 지진시 미적용

	long nIndexSmall = 1;

	long nJijum=0; 
	for(nJijum=0; nJijum<nSizeJijum; nJijum++)
	{
		//CJijum *pJijum = pBri->GetJijum(nJijum);

		//long nVerPos  = (nJijum==0) ? 1 : 0;
		//BOOL bShoePos = (pJijum->m_nQtyShoe[nVerPos]>0) ? TRUE : FALSE;
		if((nJijum==0 || nJijum==pBri->m_nQtyJigan) && TRUE /*bShoePos==FALSE*/)
		{
			nIdxDesign++;
			continue;
		}
		if(nJijum == nJijumConnect) continue;

		CGirderAndColumn *pGirCol = m_pList.GetAt(nIdxDesign);
		if(pGirCol->m_bColumn || pGirCol->m_bWall)
		{
			long j=0; for(j=0; j<nCheckSize; j++)
			{
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
				XLText->AddFormatText("$s+40$h$c%s$r중간지점 %d, %s$n",GetNumberString(nIndexSmall++,NUM_CIRCLE),nJijum,strCombo);

				CColumn ColCalc(pGirCol->pConcColumn[j], FALSE, pBri->m_nSelectPminColumn);
				ColCalc.SetValuePmax(pData->GetPmaxColumn());
				ColCalc.MakeOutputExcel(XLText,COL_POS_3, FALSE, 1, TRUE, TRUE);
			}
		}

		nIdxDesign++;
	}
}

void CARcBridgeCalcColAndGir::WriteCheckSection_Lsd(CXLFormatText *XLText, CRcBridgeRebar *pBri,long &nIndexSmall)
{
	long nSizeJijum = pBri->GetCountJijum(); 

	if (pBri->m_nSelectWallSectionDesign == 1)
	{
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$s+50$h%d) 단면검토 - 시점측 벽체$n",nIndexSmall++);

		WriteCheckSectionOfJijum_Lsd(XLText, pBri, 0);
		
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$s+50$h%d) 단면검토 - 종점측 벽체$n",nIndexSmall++);
		WriteCheckSectionOfJijum_Lsd(XLText, pBri, nSizeJijum - 1);
	}

	if (pBri->m_nSelectInWallSectionDesign == 1)
	{
		long nJijum=0; 
		for(nJijum=1; nJijum<nSizeJijum-1; ++nJijum)
		{
			CString strJijum;
			strJijum.Format("중간벽체 %d", nJijum);

			XLText->GoPosition(NO_MOVE,COL_POS_1);
			XLText->AddFormatText("$s+50$h%d) 단면검토 - %s$n",nIndexSmall++, strJijum);

			WriteCheckSectionOfJijum_Lsd(XLText, pBri, nJijum);
		}
	}
}

void CARcBridgeCalcColAndGir::WriteCheckSectionOfJijum_Lsd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nJijum)
{
	long nJijumConnect = pBri->GetJijumConnectRibAndSlab();
	if(nJijum == nJijumConnect) 
		return;
	
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	double Moment = 0;
	double Shear = 0;
	double Axial = 0;
	long nIdxModel = 0;

	EPartOfBridge ePart = (nJijum == 0)? ePartOutWallStt : ((nJijum == pBri->m_nQtyJigan)? ePartOutWallEnd : ePartInWall);

	long nIndexSmall = 1;
	long nCheckSize = RCBRIDGE_COL_CHECK_LSD;
	CGirderAndColumn *pGirCol = m_pList.GetAt(nJijum);
	for (HGINT32 ix = 0; ix < eCountEnvTypeLsd; ++ix)
	{
		ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
		if(pBri->IsValidComboEnvType(eEnvType) == FALSE)
			continue;

		// 사용한계상태는 단면 검토 하지 않는다.
		if(eEnvType >= eEnvService)
			break;

		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$s+40$h  (%d) %s$n",nIndexSmall++, pBri->GetStringComboEnvType_Lsd(eEnvType));

		ELSDCombKind eKindCombo = pBri->GetComboKindByEnvType(eEnvType, ePart, CHECK_POS_CENTER);
		long nIndexCircle = 1;
		long j=0; for(j=0; j<nCheckSize; j++)
		{
			long nCombo = pBri->GetOutValueForWallDesing_Lsd(nJijum,eEnvType, (EWallCheckLsd)j,Moment,Shear,Axial, nIdxModel);
			CString szCombo = pBri->GetStringComboEta_Lsd(eKindCombo, ePart, nCombo + 1, TRUE, nIdxModel);

			XLText->GoPosition(NO_MOVE,COL_POS_2);
			XLText->AddFormatText("$s+40$h$c%s$r%s$n",GetNumberString(nIndexCircle++,NUM_CIRCLE), szCombo);

			CColumn ColCalc(pGirCol->pConcColumnLsd[ix][j], FALSE, pBri->m_nSelectPminColumn);
			ColCalc.SetValuePmax(pData->GetPmaxColumn());

			ColCalc.SetCheckShearHoriRebar(pGirCol->m_bColumn);

			ColCalc.MakeOutputExcel(XLText,COL_POS_3, FALSE, TRUE, TRUE, TRUE);
		}
	}

	if(pGirCol->m_bWall)
	{
		// (ARCBRIDGE-2752) 수평철근 검토 출력
		CLsdBeam Beam(pGirCol->pConcColumnLsd[0][0], LSD_ULS_DESIGN);
		Beam.CalculateDryingShrinkage();
		Beam.MakeOutputDryingShrinkageExcelFile(XLText, COL_POS_2, 0);
	}
}

void CARcBridgeCalcColAndGir::WritePMDiagram( CXLFormatText *XLText, CRcBridgeRebar *pBri )
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nIndexSub(1);
	long nIdxDesign = 0;
	long nSizeJijum = pBri->GetCountJijum();
	long nJijumConnect = pBri->GetJijumConnectRibAndSlab();

	for(long nJijum=0; nJijum<nSizeJijum; nJijum++)
	{
		//CJijum *pJijum = pBri->GetJijum(nJijum);

		//long nVerPos  = (nJijum==0) ? 1 : 0;
		//BOOL bShoePos = (pJijum->m_nQtyShoe[nVerPos]>0) ? TRUE : FALSE;
		if((nJijum==0 || nJijum==pBri->m_nQtyJigan) && TRUE /*bShoePos==FALSE*/)
		{
			nIdxDesign++;
			continue;
		}
		if(nJijum == nJijumConnect) continue;

		CGirderAndColumn *pGirCol = m_pList.GetAt(nIdxDesign);
		XLText->GoPosition(NO_MOVE,COL_POS_2);
		XLText->AddFormatText("$s+40$h$c%s$r중간지점 %d$n",GetNumberString(nIndexSub++,NUM_CIRCLE),nJijum);
		CColumn ColCalc(pGirCol->pConcColumn[0], FALSE, pBri->m_nSelectPminColumn);
		ColCalc.SetValuePmax(pData->GetPmaxColumn());
		ColCalc.CalculateAll(TRUE);

		double ScaleDim = 50;
		CDomyun *pDom = new CDomyun;
		pDom->SetCWnd(m_pStd->m_pView);
		pDom->SetScaleDim(ScaleDim);
		pDom->SetLTScale(ScaleDim*5);

 		CDrawPM DrawPM(m_pStd->m_pARoadOptionStd);
 		DrawPM.DrawPM(pDom,&ColCalc);
 
 		long nSizeRow = 30;
 		XLText->InsertDomyun(pDom,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_CENTER|CXLFormatText::INSERT_DOM_MOVEROW);			

		nIdxDesign++;
	}
}

void CARcBridgeCalcColAndGir::WritePMDiagram_Lsd( CXLFormatText *XLText, CRcBridgeRebar *pBri )
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nIndexSub(1);
	long nSizeJijum = pBri->GetCountJijum();
	long nJijumConnect = pBri->GetJijumConnectRibAndSlab();

	for(long nJijum=0; nJijum<nSizeJijum; nJijum++)
	{
		if(nJijum == nJijumConnect) continue;

		CString szTitle(_T(""));

		if(nJijum==0 || nJijum==pBri->m_nQtyJigan)
		{
			if(pBri->m_nSelectWallSectionDesign == 0)
			{
				continue;
			}

			szTitle = (nJijum == 0)? _T("시점벽체") : _T("종점벽체");
		}
		else
		{
			if(pBri->m_nSelectInWallSectionDesign == 0)
			{
				continue;
			}

			szTitle.Format(_T("중간지점 %d"), nJijum);
		}

		for (HGINT32 ix = 0; ix < eCountEnvTypeLsd; ++ix)
		{
			if(pBri->IsValidComboEnvType((ETypeEnvComboLsd)ix) == FALSE)
				continue;

			// 사용한계상태는 단면 검토 하지 않는다.
			if(ix >= eEnvService)
				break;

			CString szTypeCombo = pBri->GetStringComboEnvType_Lsd((ETypeEnvComboLsd)ix);
			XLText->GoPosition(NO_MOVE,COL_POS_2);
			XLText->AddFormatText("$s+40$h$c%s$r%s$n",GetNumberString(nIndexSub++,NUM_CIRCLE), szTitle + _T(" : ") + szTypeCombo);

			CGirderAndColumn *pGirCol = m_pList.GetAt(nJijum);
			
			CColumn ColCalc(pGirCol->pConcColumnLsd[ix][0], FALSE, pBri->m_nSelectPminColumn);
			ColCalc.SetValuePmax(pData->GetPmaxColumn());
			ColCalc.CalculateAll(TRUE);

			double ScaleDim = 50;
			CDomyun *pDom = new CDomyun;
			pDom->SetCWnd(m_pStd->m_pView);
			pDom->SetScaleDim(ScaleDim);
			pDom->SetLTScale(ScaleDim*5);

			CDrawPM DrawPM(m_pStd->m_pARoadOptionStd);
			DrawPM.DrawPM(pDom,&ColCalc);

			long nSizeRow = 30;
			XLText->InsertDomyun(pDom,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_CENTER|CXLFormatText::INSERT_DOM_MOVEROW);	
		}
	}
}

CConcBasic* CARcBridgeCalcColAndGir::GetConcWallForOutWallUsebilityCheck( BOOL bSttWall )
{
	// (ARCBRIDGE-3618) 시종점 벽체를 휨압축 부재로 검토하고 사용성검토를 할때 사용성검토를 Beam부재 집계데이터를 사용하지않고
	// 기둥집계 데이터에서 모멘트 최대 조합으로 검토를 해달라는...
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CConcBasic *pConc(NULL);

	long nJ = bSttWall ? 0 : pBri->m_nQtyJigan;
	for(long ix=0; ix<m_pList.GetSize(); ++ix)
	{
		CGirderAndColumn *pGirCol = m_pList.GetAt(ix);
		if(pGirCol->m_nJijum == nJ && pGirCol->m_bWall)
		{
			pConc = pGirCol->pConcColumnLsd[eEnvService][1];
			break;
		}
	}

	return pConc;
}
