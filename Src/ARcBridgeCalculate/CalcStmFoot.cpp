#include "stdafx.h"
#include "ARcBridgeCalculate.h"


CCalcStmFoot::CCalcStmFoot(CARcBridgeCalcStm* pCalcStm, BOOL bStt)
{
	m_pCalcStm = pCalcStm;
	m_bStt	= bStt;
	m_bEarthQuakeLsd = FALSE;
}

CCalcStmFoot::~CCalcStmFoot(void)
{
}

void CCalcStmFoot::SetPosFooting( HGBOOL bStt )
{
	m_bStt = bStt;
}

void CCalcStmFoot::SetTypeLoadCase( eTypeLoadFoot eTypeLoad )
{
	m_eTypeLoad = eTypeLoad;
}

void CCalcStmFoot::SetTypeLoadCaseLsd(EFootingCheckLsd eTypeLoad)
{
	m_eTypeLoadLsd = eTypeLoad;
}

void CCalcStmFoot::SetEarthQuakeLsd(HGBOOL bEarthQuake)
{
	m_bEarthQuakeLsd = bEarthQuake;
}

void CCalcStmFoot::MakeStmModelByTemplete( CStmModel *pStmModel, HGBOOL bStressFlow , ETypeEnvComboLsd eTypeEnv )
{
	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return;
	CARcBridgeDataStd* pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	
	m_pCalcStm->SetConcData(FALSE, TRUE, TRUE);

	CStmAbutFooting *pStmFoot = new CStmAbutFooting(TRUE);
	pStmModel->SetStmModifiedFlag(FALSE);
	pStmModel->ClearStm(TRUE);
	pStmModel->SetPlane(eStmPlaneXz);
	
	pStmModel->SetThickModel(bLsd ? frM(UNIT_METER) : UNIT_METER);
	pStmModel->SetLsdDesign(bLsd);

	if(bLsd)
	{
		m_eTypeEnv = eTypeEnv;
		CStmDataConfig *pConfig = pStmModel->GetStmDataConfig();
		if(pConfig)
		{
			CLsdResistCoefMatCtl * pResistCoefCtl = &(pData->m_plsdEnv->GetLsdResistCoefMatCtl());

			ELSDCombKind eComboKind(eLSD_COMBINE_LIMIT_I);
			if(m_eTypeEnv == eEnvExtremeI)
				eComboKind = eLSD_COMBINE_EXTREME_I;
			else if(m_eTypeEnv == eEnvExtremeII)
				eComboKind = eLSD_COMBINE_EXTREME_II;

			pConfig->m_dPi_C = pResistCoefCtl->GetCoefficient(eComboKind, eResistCoefMaterial_Concrete);
			pConfig->m_dPi_S = pResistCoefCtl->GetCoefficient(eComboKind, eResistCoefMaterial_Ferro);

			pConfig->m_dFck	= pBri->GetValueFck(m_bStt? ePartFootStt : ePartFootEnd, TRUE);
			pConfig->m_dFy	= pBri->GetValueFy(m_bStt? ePartFootStt : ePartFootEnd, FALSE, TRUE);
			pConfig->m_dFvy	= pBri->GetValueFy(m_bStt? ePartFootStt : ePartFootEnd, TRUE, TRUE);
			pConfig->m_dFt	= GetValueUnitChange(pBri->GetValueStmFctk(m_bStt? eLsdStmFootStt : eLsdStmFootEnd), UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);
			pConfig->m_dEc	= pBri->GetValueEc(m_bStt? ePartFootStt : ePartFootEnd, TRUE);
			pConfig->m_dEs	= GetValueUnitChange(pData->m_Es, UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);
		}
	}

	pStmFoot->SetStmModel(pStmModel);
	SetFootTemplate(pStmFoot);
	SetFootFoundation(pStmFoot, pStmModel);
	SetFootSetting(pStmFoot, pStmModel);
	SetFootLoad(pStmFoot, eTypeEnv);

	if(bStressFlow)
	{
		if( pStmModel->IsLsdDesign() )
			pStmFoot->SetMeshSize(TRUE, frM(pBri->m_dMeshSizeBase));
		else
			pStmFoot->SetMeshSize(TRUE, pBri->m_dMeshSizeBase);
		pStmFoot->MakeStressFlow(TRUE);
	}
	else
	{
		pStmFoot->MakeModeling();
		pStmModel->SetStmModifiedFlag(TRUE);
		// MakeModel 하고 나서 철근을 넣어준다.
		SetFootRebar(pStmModel);
	}

	delete pStmFoot;

	if(bLsd)
	{
		CStmObjLoadConditionCombo *pCombo = pStmModel->GetLoadConditionComboIndex(0);
		pCombo->SetName(pBri->m_strStmModelingFooting[m_bStt? 0 : 1][eTypeEnv][m_eTypeLoadLsd]);
	}
}

void CCalcStmFoot::ResetLoadAtModel( CStmModel *pStmModel, ETypeEnvComboLsd eTypeEnv )
{
	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return;
	CARcBridgeDataStd* pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();

	CStmAbutFooting *pStmFoot = new CStmAbutFooting(TRUE);

	m_pCalcStm->SetConcData(FALSE, TRUE, TRUE);

	m_eTypeEnv = eTypeEnv;
	pStmFoot->SetStmModel(pStmModel);
	SetFootTemplate(pStmFoot);
	SetFootFoundation(pStmFoot, pStmModel);
	SetFootSetting(pStmFoot, pStmModel);
	SetFootLoad(pStmFoot, eTypeEnv);

	pStmFoot->MakeLoadOnly();

	delete pStmFoot;

	if(bLsd)
	{
		CStmObjLoadConditionCombo *pCombo = pStmModel->GetLoadConditionComboIndex(0);
		pCombo->SetName(pBri->m_strStmModelingFooting[m_bStt? 0 : 1][eTypeEnv][m_eTypeLoadLsd]);
	}
}

void CCalcStmFoot::SetFootTemplate( CStmAbutFooting *pStmFoot )
{
	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return;
	
	CFootingApp *pFoot = pBri->GetFootingByJijumNumber(m_bStt? 0 : pBri->m_nQtyJigan);
	if(pFoot == NULL) return;

	double dThickWall = pBri->GetThickWall(m_bStt? 0 : pBri->m_nQtyJigan, FALSE);
	double dHeight = pFoot->GetHeight();

	pStmFoot->sDim.stt		= m_bStt? 0 : 1;
	pStmFoot->sDim.B		= pFoot->GetLength();
	pStmFoot->sDim.L1		= (m_bStt)? pFoot->m_dFWR : pFoot->m_dFWL;
	pStmFoot->sDim.WW		= dThickWall;
	pStmFoot->sDim.L2		= (m_bStt)? pFoot->m_dFWL : pFoot->m_dFWR;
	pStmFoot->sDim.H1		= dHeight;
	pStmFoot->sDim.H2		= dHeight - ((m_bStt)? pFoot->m_dFSRH : pFoot->m_dFSLH);
	pStmFoot->sDim.H3		= dHeight - ((m_bStt)? pFoot->m_dFSLH : pFoot->m_dFSRH);

	pStmFoot->sDim.W1		= 0;
	pStmFoot->sDim.W2		= dThickWall;
}

void CCalcStmFoot::SetFootLoad( CStmAbutFooting *pStmFoot, ETypeEnvComboLsd eTypeEnv )
{
	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return;

	CFootingApp *pFoot = pBri->GetFootingByJijumNumber(m_bStt? 0 : pBri->m_nQtyJigan);
	if(pFoot == NULL) return;

	CARcBridgeCalcFooting *pCalcFooting = m_pCalcStm->m_pStd->m_pFooting;
	pCalcFooting->CalculateAll();

	HGINT32 nJijum = m_bStt? 0 : pBri->m_nQtyJigan;

	HGBOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();
	// 한계상태일때 하중은 단위폭(mm)당으로 구했지만 1m 폭에 재하되는 하중을 준다.
	double dWidth = bLsd? frM(UNIT_METER) : UNIT_METER;

	double dMoment(0), dVert(0), dHori(0);
	long nIdxModel(0);
	if(pBri->GetFemManage(0)->IsOutputLoaded())
	{
		if( bLsd )
		{
			ELSDCombKind eKindCombo = pBri->GetComboKindByEnvType(eTypeEnv, m_bStt ? ePartFootStt : ePartFootEnd, CHECK_POS_CENTER);
			long ixCombo = pBri->GetOutValueForFootingDesign_Lsd(nJijum, eTypeEnv, m_eTypeLoadLsd, dMoment, dHori, dVert, nIdxModel);
			pBri->m_strStmModelingFooting[m_bStt? 0 : 1][eTypeEnv][m_eTypeLoadLsd] = pBri->GetStringComboEta_Lsd(eKindCombo, m_bStt ? ePartFootStt : ePartFootEnd, ixCombo + 1, TRUE, nIdxModel);
		}
		else
			pBri->GetOutValueForFootingDesign(nJijum, m_eTypeLoad, dMoment, dHori, dVert, FALSE);
	}

	pStmFoot->m_strLoadWallName	= _T("W");
	pStmFoot->m_vLoadWall.y		= dVert * dWidth;		// vertical
	pStmFoot->m_vLoadWall.x		= dHori * dWidth * (m_bStt ? 1 : -1);		// horizontal
	pStmFoot->m_vLoadWall.z		= dMoment * dWidth;		// Moment
	pStmFoot->m_strComment		= _T("상부 전달하중");

	pStmFoot->m_icLoad		= 0;
	pStmFoot->m_vStaticLoad.clear();

	CLoadFooting *pDataLoad = NULL;
	if(bLsd)
	{
		if(pCalcFooting->m_pListLoadLsd[nJijum][m_eTypeEnv].GetSize() <= m_eTypeLoadLsd) 
			return;
		pDataLoad = pCalcFooting->m_pListLoadLsd[nJijum][m_eTypeEnv].GetAt(m_eTypeLoadLsd);
	}
	else
	{
		if(pCalcFooting->m_pListLoadUlt[nJijum].GetSize() <= m_eTypeLoad) 
			return;
		pDataLoad = pCalcFooting->m_pListLoadUlt[nJijum].GetAt(m_eTypeLoad);
	}
	
	double wB = (m_bStt)? toM(pFoot->m_dFWL) : toM(pFoot->m_dFWR);
	if(bLsd)
		wB = frM(wB);

	// Load on Heel
	for(HGINT32 ix=0; ix<VER_LOAD_COUNT; ++ix)
	{
		if(Compare(pDataLoad->m_dVerLoad[ix], 0.0, _T("="), 0.001))
			continue;

		// (ARCBRIDGE-3259) 자중은 뒷굽에 태우지 않고 옵션에 따라
		if(ix == VER_LOAD_FOOTW)
		{
			double wFB = toM(pFoot->m_dFWL + pFoot->m_dFWR + pFoot->m_dML + pFoot->m_dMR);
			if(bLsd)
				wFB = frM(wFB);

			pStmFoot->m_dSelfWeight	= pDataLoad->m_dVerLoad[ix] / wFB * dWidth;
		}
		else
		{
			HGDOUBLE dVer = pDataLoad->m_dVerLoad[ix] / wB * dWidth;

			SDistributedLoad sLoad;
			sLoad.name		= pDataLoad->GetStringVertLoad(ix, TRUE);
			sLoad.w1		= dVer;
			sLoad.w2		= dVer;
			sLoad.description	= pDataLoad->GetStringVertLoad(ix, FALSE);

			pStmFoot->m_vStaticLoad.push_back(sLoad);
		}
	}

	pStmFoot->m_icLoad	= pStmFoot->m_vStaticLoad.size();
}

void CCalcStmFoot::SetFootRebar( CStmModel *pStmModel )
{
	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return;

	HGINT32 nJijum = m_bStt? 0 : pBri->m_nQtyJigan;
	m_pCalcStm->SetFootRebar(pStmModel, nJijum);
}

void CCalcStmFoot::SetFootSetting( CStmAbutFooting *pStmFoot, CStmModel *pStmModel )
{
	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return;

	CFootingApp *pFoot = pBri->GetFootingByJijumNumber(m_bStt? 0 : pBri->m_nQtyJigan);
	if(pFoot == NULL) return;

	CStmDataConfig *pDataConfig = pStmModel->GetStmDataConfig();
	if(pDataConfig == NULL) return;

	pStmFoot->m_ixSupportColumn		= pDataConfig->m_ixMinSupportNode;		//	기둥의 지지점 개수(A)
	// 피복 두께
	pStmFoot->m_dDistSupportColumn		= pBri->m_dCoverWallOut[0];			// 기둥의 지지점 위치(B)
	pStmFoot->m_dH1		= GetDistCompressStrut();			// 상단부재 위치(C)
	pStmFoot->m_dH2		= GetDistRebarTie();			// 하단부재 위치(D)
	pStmFoot->m_dDistSupportColumn	= GetDistRebarWall();

	if(pDataConfig->m_ixTypeDivideSpringSpace == 0)
	{
		pStmFoot->m_dRateNodeHeight		= pDataConfig->m_dRateHeightSpringSpace;									// 절점생성:높이비(G)
	}
	else
	{
		pStmFoot->m_dRateNodeHeight		= pDataConfig->m_dDivideSpringSpace / pFoot->GetHeight();
	}

	pStmFoot->m_ixMakeNodeUpper		= 0;									// 절점생성:상단(F)"
	

	pStmFoot->m_ixSpringType		= pDataConfig->m_ixSpringType;									// Spring Type
}

void CCalcStmFoot::SetFootFoundation( CStmAbutFooting *pStmFoot, CStmModel *pStmModel )
{
	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CFootingApp *pFoot = pBri->GetFootingByJijumNumber(m_bStt? 0 : pBri->m_nQtyJigan);
	if(pFoot == NULL) return;

	CStmDataConfig *pStmData = pStmModel->GetStmDataConfig();
	if(pStmData == NULL) return;

	HGBOOL bPileBase = pBri->IsPileBase(m_bStt? 0 : pBri->m_nQtyJigan);

	pStmFoot->sFoot.FootingType	= bPileBase ? 1 : 0;

	if(bPileBase)
	{
		m_pCalcStm->m_pStd->m_pFooting->CalculateAll();
		CDesignPile *pDesignPile = m_pCalcStm->m_pStd->GetDesignPile(m_bStt? 0 : pBri->m_nQtyJigan);
		CExPileApp *pPile = &(pFoot->m_exFooting.m_Pile);
		pDesignPile->CalculateAll();

		pStmFoot->sFoot.EaPile		= pPile->m_nCountOrg;
		pStmFoot->sFoot.DiaPile		= pPile->m_dDia;

		CString strIdx;
		pStmFoot->sFoot.vPile_Dist.clear();
		pStmFoot->sFoot.vPile_Ea.clear();
		HGINT32 icPile = pStmFoot->sFoot.EaPile;
		for(HGINT32 ix = 0; ix < icPile; ++ix)
		{
			pStmFoot->sFoot.vPile_Dist.push_back(pPile->GetPileDist(ix, TRUE));
			pStmFoot->sFoot.vPile_Ea.push_back(pPile->GetPileSu(ix, TRUE));
		}
		
		pStmFoot->sFoot.Kv		= GetValueUnitChange(pDesignPile->m_Kv, UNIT_CGS_TONF_M, pData->m_szTonf_M);
		if( pBri->m_pARcBridgeDataStd->IsLsdDesign() )
		{
			pStmFoot->sFoot.Kh		= GetValueUnitChange(pDesignPile->m_K1_Fix[m_bEarthQuakeLsd ? PILE_CASE_EXTREME : PILE_CASE_ULTIMATE], UNIT_CGS_TONF_M, pData->m_szTonf_M);
			pStmFoot->sFoot.Kr		= GetValueUnitChange(pDesignPile->m_K4_Fix[m_bEarthQuakeLsd ? PILE_CASE_EXTREME : PILE_CASE_ULTIMATE], UNIT_CGS_TONF_M, pData->m_szTonf_M);
		}
		else
		{
			pStmFoot->sFoot.Kh		= GetValueUnitChange(pDesignPile->m_K1_Fix[m_eTypeLoad == eEarthQuake? PILE_CASE_EARTH : PILE_CASE_NORMAL], UNIT_CGS_TONF_M, pData->m_szTonf_M);
			pStmFoot->sFoot.Kr		= GetValueUnitChange(pDesignPile->m_K4_Fix[m_eTypeLoad == eEarthQuake? PILE_CASE_EARTH : PILE_CASE_NORMAL], UNIT_CGS_TONF_M, pData->m_szTonf_M);
		}

		if(pStmData->m_ixTypeFootingLoad == 1) //기초하단 반력 작용
		{
			pStmFoot->sFoot.vReaction.clear();
			double dUC = GetValueUnitChange(1.0, UNIT_CGS_TONF, pData->m_szTonf);

			for(HGINT32 nPile = 0; nPile < icPile; ++nPile)
			{
				double dPileFix_Axial  = pDesignPile->GetValueMaxMinPileHead(m_eTypeLoadLsd, PILE_FORCE_AXIAL, nPile, TRUE ,TRUE, PILE_CASE_ULTIMATE, FALSE);
				double dPileHinge_Axial = pDesignPile->GetValueMaxMinPileHead(m_eTypeLoadLsd, PILE_FORCE_AXIAL, nPile, FALSE ,TRUE, PILE_CASE_ULTIMATE, FALSE);
				double dPileFix_Hori = pDesignPile->GetValueMaxMinPileHead(m_eTypeLoadLsd, PILE_FORCE_HORI, nPile, TRUE ,TRUE, PILE_CASE_ULTIMATE, FALSE);
				double dPileHinge_Hori = pDesignPile->GetValueMaxMinPileHead(m_eTypeLoadLsd, PILE_FORCE_HORI, nPile, FALSE ,TRUE, PILE_CASE_ULTIMATE, FALSE);

				double dV = MAX(dPileFix_Axial, dPileHinge_Axial) * dUC;
				double dH = MAX(dPileFix_Hori, dPileHinge_Hori) * dUC;

				//폭당 계산된 값을 적용한다
				long nPileSide = pPile->GetPileSu(nPile, TRUE);
				dV = dV * nPileSide / pFoot->GetLength() * 1000;
				dH = dH * nPileSide / pFoot->GetLength() * 1000 * (m_bStt ? 1 : -1);

				pStmFoot->sFoot.vReaction.push_back(CVector(-dH, 0, dV));
			}
		}
	}
	else
	{
		CARcBridgeCalcEarthQuake *pLoadEQ = m_pCalcStm->m_pStd->m_pLoadEQ;
		pLoadEQ->CalculateAll();

		CRigidityFooting *pRigid = pLoadEQ->GetRigidityFooting(m_bStt? 0 : pBri->m_nQtyJigan);

		// 지반반력 계수
		pStmFoot->sFoot.Kv	= GetValueUnitChange(pRigid->m_ValueK[RIGIDITY_DISPY], UNIT_CGS_TONF_M, pData->m_szTonf_M);
		pStmFoot->sFoot.Kh	= GetValueUnitChange(pRigid->m_ValueK[RIGIDITY_DISPX], UNIT_CGS_TONF_M, pData->m_szTonf_M);
	}
}

HGDOUBLE CCalcStmFoot::GetDistRebarTie()
{
	HGDOUBLE dDist(0);

	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return dDist;

	HGBOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	CStmModelForArcbridge *pStmModel = dynamic_cast<CStmModelForArcbridge*>(pBri->GetStmModelFoot(m_bStt, bLsd ? m_eTypeLoadLsd : m_eTypeLoad, bLsd ? m_eTypeEnv : 0));
	if(!pStmModel) return dDist;

	HGINT32 nJjum = m_bStt? 0 : pBri->m_nQtyJigan;
	CFootingApp *pFoot = pBri->GetFootingByJijumNumber(m_bStt? 0 : pBri->m_nQtyJigan);
	if(pFoot == NULL) return dDist;

	if(pStmModel->m_nTypePosRebarTie == 0)	// 1단철근위치
	{
		dDist = pFoot->m_dCoverLower[0];
	}
	else if(pStmModel->m_nTypePosRebarTie == 1)	// 철근의도심
	{
		
		CConcBasic *pConc = pBri->GetConcDataFooting(nJjum, TRUE);
		if(pConc)
			dDist = pConc->GetRebarDistTension();
	}	
	else if(pStmModel->m_nTypePosRebarTie == 2) // 사용자입력
	{
		dDist = pStmModel->m_dUserPosRebarTie;
	}

	return dDist;
}

HGDOUBLE CCalcStmFoot::GetDistCompressStrut()
{
	HGDOUBLE dDist(0);

	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return dDist;

	HGBOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	CStmModelForArcbridge *pStmModel = dynamic_cast<CStmModelForArcbridge*>(pBri->GetStmModelFoot(m_bStt, bLsd ? m_eTypeLoadLsd : m_eTypeLoad, bLsd ? m_eTypeEnv : 0));
	if(!pStmModel) return dDist;

	CFootingApp *pFoot = pBri->GetFootingByJijumNumber(m_bStt? 0 : pBri->m_nQtyJigan);
	if(pFoot == NULL) return dDist;

	HGINT32 nJjum = m_bStt? 0 : pBri->m_nQtyJigan;
	CStmDataConfig *pStmData = pStmModel->GetStmDataConfig();
	HGDOUBLE dRatePiT_PiS = pStmData->m_dUserRatioPiT_PiS;
	if(pStmData->m_ixRatioPiT_PiS == 0)
	{
		if(bLsd)
			dRatePiT_PiS = pStmData->m_dPi_S / pStmData->m_dPi_C;
		else
			dRatePiT_PiS = pStmData->m_dPiTie / pStmData->m_dPiStrut;
	}

	if(pStmModel->m_nTypePosCompressStrut == 0)	// Ac,min/2
	{
		CConcBasic *pConc = pBri->GetConcDataFooting(nJjum, TRUE, TRUE);
		if(pConc)
		{
			double Fck = pBri->GetValueFck(m_bStt? ePartFootStt : ePartFootEnd);		// CGS 단위 그대로 사용
			double Fy = pBri->GetValueFy(m_bStt? ePartFootStt : ePartFootEnd, FALSE);

			double dc = pConc->GetRebarDistTension();
			double dEs = pBri->m_pARcBridgeDataStd->m_Es;
			double dHeight = pConc->m_Ho;
			double dDist1 = dHeight - dc;
			double cDist = UltimateEs / (UltimateEs + Fy / dEs) * dDist1;
			double aDist = GetK1Value(Fck) * cDist;

			double dHeightBlock = aDist;

			dDist = dHeightBlock * dRatePiT_PiS / 2;
		}
	}
	else if(pStmModel->m_nTypePosCompressStrut == 1 || pStmModel->m_nTypePosCompressStrut == 3)	// 1단철근위치(피복) | 1단철근위치(피복)x강도감소계수비
	{
		dDist = pFoot->m_dCoverUpper[0];

		if(pStmModel->m_nTypePosCompressStrut == 3)
		{
			dDist *= dRatePiT_PiS;
		}
	}
	else if(pStmModel->m_nTypePosCompressStrut == 2 || pStmModel->m_nTypePosCompressStrut == 4)	// 철근의도심 | 철근의도심x 강도감소계수비
	{
		CConcBasic *pConc = pBri->GetConcDataFooting(nJjum, TRUE, TRUE);
		if(pConc)
			dDist = pConc->GetRebarDistTension();
		if(pStmModel->m_nTypePosCompressStrut == 4)
		{
			dDist *= dRatePiT_PiS;
		}
	}
	else if(pStmModel->m_nTypePosCompressStrut == 5) // 사용자입력
	{
		dDist = pStmModel->m_dUserPosCompressStrut;
	}

	return dDist;
}

HGDOUBLE CCalcStmFoot::GetDistRebarWall()
{
	HGDOUBLE dDist(0);

	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return dDist;

	HGBOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	CStmModelForArcbridge *pStmModel = dynamic_cast<CStmModelForArcbridge*>(pBri->GetStmModelFoot(m_bStt, bLsd ? m_eTypeLoadLsd : m_eTypeLoad, bLsd ? m_eTypeEnv : 0));
	if(!pStmModel) return dDist;

	CStmDataConfig *pDataConfig = pStmModel->GetStmDataConfig();
	if(pDataConfig == NULL) return dDist;

	HGINT32 nJjum = m_bStt? 0 : pBri->m_nQtyJigan;
	double dThickWall = pBri->GetThickWall(nJjum, FALSE);
	double Fck = pBri->GetValueFck(m_bStt? ePartFootStt : ePartFootEnd, TRUE);		// 단위계적용

	if(pDataConfig->m_ixEffectiveWidthColumn == 0)		// 기둥철근도심 x 2
	{
		CConcBasic *pConc = pBri->GetConcDataWall(nJjum,CHECK_POS_BOTTOM, TRUE);
		if(pConc)
			dDist = pConc->GetRebarDistTension();
	}
	else if(pDataConfig->m_ixEffectiveWidthColumn == 1)		// 기둥두께등분
	{
		double dCountSupport = pDataConfig->m_ixMinSupportNode + 1;
		dDist = dThickWall / dCountSupport / 2.;
	}
	else if(pDataConfig->m_ixEffectiveWidthColumn == 2)		// (분담하중/작용하중) x 기둥두께
	{
		double dVForce = 100;
		double dAForce = dVForce / 2;	// 지지점의 하중 (좌우 양측이므로 / 2로 적용)

		dDist = dThickWall * dAForce / dVForce / 2.;

		// ????? 이거 그냥1/4 인데??
	}
	else if(pDataConfig->m_ixEffectiveWidthColumn == 3)		// 분담하중/fce
	{
		double dAForce = 100;
		dDist = dAForce / (0.85 * 1.0 * Fck);
	}
	else
	{
		dDist = pDataConfig->m_dUserEffectiveWidthColumn;
	}

	return dDist;
}
