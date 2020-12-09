#include "stdafx.h"
#include "ARcBridgeCalculate.h"

CCalcStmMidFoot::CCalcStmMidFoot(CARcBridgeCalcStm* pCalcStm)
{
	m_pCalcStm = pCalcStm;
	m_bEarthQuakeLsd = FALSE;
}

CCalcStmMidFoot::~CCalcStmMidFoot(void)
{
}

void CCalcStmMidFoot::SetPosFooting( HGINT32 nJijum )
{
	m_nJijum = nJijum;
}

void CCalcStmMidFoot::SetTypeLoadCase( eTypeLoadFoot eTypeLoad )
{
	m_eTypeLoad = eTypeLoad;
}

void CCalcStmMidFoot::SetTypeLoadCaseLsd(EFootingCheckLsd eTypeLoad)
{
	m_eTypeLoadLsd = eTypeLoad;
}

void CCalcStmMidFoot::SetEarthQuakeLsd(HGBOOL bEarthQuake)
{
	m_bEarthQuakeLsd = bEarthQuake;
}

void CCalcStmMidFoot::MakeStmModelByTemplete( CStmModel *pStmModel, HGBOOL bStressFlow, ETypeEnvComboLsd eTypeEnv )
{
	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return;
	CARcBridgeDataStd* pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();

	m_pCalcStm->SetConcData(FALSE, FALSE, TRUE);

	CStmPierFooting *pStmMidFoot = new CStmPierFooting(TRUE);
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

			pConfig->m_dFck	= pBri->GetValueFck(ePartInFoot, TRUE);
			pConfig->m_dFy	= pBri->GetValueFy(ePartInFoot, FALSE, TRUE);
			pConfig->m_dFvy	= pBri->GetValueFy(ePartInFoot, TRUE, TRUE);
			pConfig->m_dFt	= GetValueUnitChange(pBri->GetValueStmFctk(eLsdStmFootMid), UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);
			pConfig->m_dEc	= pBri->GetValueEc(ePartInFoot, TRUE);
			pConfig->m_dEs	= GetValueUnitChange(pData->m_Es, UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);
		}
	}

	pStmMidFoot->SetStmModel(pStmModel);
	SetMidFootTemplate(pStmMidFoot);
	SetMidFootSetting(pStmMidFoot, pStmModel);
	SetMidFootFoundation(pStmMidFoot, pStmModel);
	SetMidFootLoad(pStmMidFoot, eTypeEnv);

	if(bStressFlow)
	{
		if( pStmModel->IsLsdDesign() )
			pStmMidFoot->SetMeshSize(TRUE, frM(pBri->m_dMeshSizeBase));
		else
			pStmMidFoot->SetMeshSize(TRUE, pBri->m_dMeshSizeBase);
		pStmMidFoot->MakeStressFlow(TRUE);
	}
	else
	{
		pStmMidFoot->MakeModeling();
		pStmModel->SetStmModifiedFlag(TRUE);
		// MakeModel 하고 나서 철근을 넣어준다.
		SetMidFootRebar(pStmModel);
	}

	delete pStmMidFoot;
	
	if(bLsd)
	{
		CStmObjLoadConditionCombo *pCombo = pStmModel->GetLoadConditionComboIndex(0);
		pCombo->SetName(pBri->m_strStmModelingMidFooting[m_nJijum-1][m_eTypeEnv][m_eTypeLoadLsd]);
	}
}

void CCalcStmMidFoot::ResetLoadAtModel( CStmModel *pStmModel, ETypeEnvComboLsd eTypeEnv )
{
	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return;
	CARcBridgeDataStd* pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();

	CStmPierFooting *pStmMidFoot = new CStmPierFooting(TRUE);
	
	m_eTypeEnv = eTypeEnv;
	pStmMidFoot->SetStmModel(pStmModel);
	SetMidFootTemplate(pStmMidFoot);
	SetMidFootLoad(pStmMidFoot, eTypeEnv);

	pStmMidFoot->MakeLoadOnly();

	delete pStmMidFoot;

	if(bLsd)
	{
		CStmObjLoadConditionCombo *pCombo = pStmModel->GetLoadConditionComboIndex(0);
		pCombo->SetName(pBri->m_strStmModelingMidFooting[m_nJijum-1][m_eTypeEnv][m_eTypeLoadLsd]);
	}
}

void CCalcStmMidFoot::SetMidFootTemplate( CStmPierFooting *pStmMidFoot )
{
	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return;

	CFootingApp *pFoot = pBri->GetFootingByJijumNumber(m_nJijum);
	if(pFoot == NULL) return;

	double dThickWall = pBri->GetThickWall(m_nJijum);
	double dHeight = pFoot->GetHeight();

	pStmMidFoot->sDim.B		= pFoot->GetLength();
	pStmMidFoot->sDim.LL		= pFoot->m_dFWL;
	pStmMidFoot->sDim.LR		= pFoot->m_dFWR;
	pStmMidFoot->sDim.FH		= dHeight;
	pStmMidFoot->sDim.EA		= 1;
	pStmMidFoot->sDim.CW		= dThickWall;
}

void CCalcStmMidFoot::SetMidFootLoad( CStmPierFooting *pStmMidFoot, ETypeEnvComboLsd eTypeEnv )
{
	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return;

	HGBOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

 	CARcBridgeCalcFooting *pCalcFooting = m_pCalcStm->m_pStd->m_pFooting;
	double dMoment(0), dVert(0), dHori(0);
	long nIdxModel(0);

	if(pBri->GetFemManage(0)->IsOutputLoaded())
	{
		if( bLsd )
		{
			ELSDCombKind eKindCombo = pBri->GetComboKindByEnvType(eTypeEnv, ePartInFoot, CHECK_POS_CENTER);
			long ixCombo = pBri->GetOutValueForFootingDesign_Lsd(m_nJijum, eTypeEnv, m_eTypeLoadLsd, dMoment, dHori, dVert, nIdxModel);
			pBri->m_strStmModelingMidFooting[m_nJijum-1][eTypeEnv][m_eTypeLoadLsd] = pBri->GetStringComboEta_Lsd(eKindCombo, ePartInFoot, ixCombo + 1, TRUE, nIdxModel);
		}
		else
			pBri->GetOutValueForFootingDesign(m_nJijum, bLsd ? m_eTypeLoadLsd : m_eTypeLoad, dMoment, dHori, dVert, FALSE);
	}

	pStmMidFoot->m_vLoad.clear();

	// 한계상태일때 하중은 단위폭(mm)당으로 구했지만 1m 폭에 재하되는 하중을 준다.
	double dWidth = bLsd? frM(UNIT_METER) : UNIT_METER;

	SPierFootingLoad sLoad;
	sLoad.name		= _T("W");				// (ARCBRIDGE-3257) 시종점 기초와 중간기초 모두 상부하중은 W로 정의했다. 하중이름으로 계산서 출력시 상부하중과 기타하중을 구분하므로 바꾸면안된다.
	sLoad.vert		= dVert * dWidth;
	sLoad.hori		= dHori * dWidth;
	sLoad.moment	= dMoment * dWidth;
	sLoad.comment   = _T("상부 전달하중");
	
	pStmMidFoot->m_vLoad.push_back(sLoad);

	// 자중
	CLoadFooting *pDataLoad = NULL;
	pCalcFooting->CalculateAll();
	if(bLsd)
	{
		if(pCalcFooting->m_pListLoadLsd[m_nJijum][m_eTypeEnv].GetSize() <= m_eTypeLoadLsd) 
			pDataLoad = NULL;
		else
			pDataLoad = pCalcFooting->m_pListLoadLsd[m_nJijum][m_eTypeEnv].GetAt(m_eTypeLoadLsd);
	}
	else
	{
		if(pCalcFooting->m_pListLoadUlt[m_nJijum].GetSize() <= m_eTypeLoad) 
			pDataLoad = NULL;
		else
			pDataLoad = pCalcFooting->m_pListLoadUlt[m_nJijum].GetAt(m_eTypeLoad);
	}
	if(pDataLoad)
	{
		CFootingApp *pFoot = pBri->GetFootingByJijumNumber(m_nJijum);
		if(pFoot == NULL) return;

		double wFB = toM(pFoot->GetWidth());
		if(bLsd)
			wFB = frM(wFB);

		pStmMidFoot->m_dSelfWeight = pDataLoad->m_dVerLoad[VER_LOAD_FOOTW] / wFB * dWidth;
	}
}

void CCalcStmMidFoot::SetMidFootRebar( CStmModel *pStmModel )
{
	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return;

	m_pCalcStm->SetFootRebar(pStmModel, m_nJijum);
}

void CCalcStmMidFoot::SetMidFootSetting( CStmPierFooting *pStmFoot, CStmModel *pStmModel )
{
	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return;

	CFootingApp *pFoot = pBri->GetFootingByJijumNumber(m_nJijum);
	if(pFoot == NULL) return;

	CStmDataConfig *pDataConfig = pStmModel->GetStmDataConfig();
	if(pDataConfig == NULL) return;

	pStmFoot->m_ixSupportColumn		= 2;		//	기둥의 지지점 개수(A)
	// 피복 두께
	pStmFoot->m_dDistSupportColumn		= pBri->m_dCoverWallOut[0];			// 기둥의 지지점 위치(B)
	pStmFoot->m_dH1		= GetDistCompressStrut();			// 상단부재 위치(C)
	pStmFoot->m_dH2		= GetDistRebarTie();			// 하단부재 위치(D)
	pStmFoot->m_ixMakeNodeUpper		= 0;									// 절점생성:상단(F)"
	pStmFoot->m_dRateNodeHeight		= 0.8;									// 절점생성:높이비(G)
	pStmFoot->m_ixSpringType		= pDataConfig->m_ixSpringType;									// Spring Type
}

void CCalcStmMidFoot::SetMidFootFoundation( CStmPierFooting *pStmFoot, CStmModel *pStmModel )
{
	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CFootingApp *pFoot = pBri->GetFootingByJijumNumber(m_nJijum);
	if(pFoot == NULL) return;

	CStmDataConfig *pStmData = pStmModel->GetStmDataConfig();
	if(pStmData == NULL) return;

	HGBOOL bPileBase = pBri->IsPileBase(m_nJijum);

	pStmFoot->sFoot.FootingType	= bPileBase ? 1 : 0;

	if(bPileBase)
	{
		m_pCalcStm->m_pStd->m_pFooting->CalculateAll();
		CDesignPile *pDesignPile = m_pCalcStm->m_pStd->GetDesignPile(m_nJijum);
		CExPileApp *pPile = &(pFoot->m_exFooting.m_Pile);
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

		pStmFoot->sFoot.Kv		= pDesignPile->m_Kv;
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
				dH = dH * nPileSide / pFoot->GetLength() * 1000;

				pStmFoot->sFoot.vReaction.push_back(CVector(-dH, 0, dV));
			}
		}
	}
	else
	{
		CARcBridgeCalcEarthQuake *pLoadEQ = m_pCalcStm->m_pStd->m_pLoadEQ;
		pLoadEQ->CalculateAll();

		CRigidityFooting *pRigid = pLoadEQ->GetRigidityFooting(m_nJijum);

		// 지반반력 계수
		pStmFoot->sFoot.Kv	= GetValueUnitChange(pRigid->m_ValueK[RIGIDITY_DISPY], UNIT_CGS_TONF_M, pData->m_szTonf_M);
		pStmFoot->sFoot.Kh	= GetValueUnitChange(pRigid->m_ValueK[RIGIDITY_DISPX], UNIT_CGS_TONF_M, pData->m_szTonf_M);
	}
}

HGDOUBLE CCalcStmMidFoot::GetDistRebarTie()
{
	HGDOUBLE dDist(0);

	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return dDist;

	HGBOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	CStmModelForArcbridge *pStmModel = dynamic_cast<CStmModelForArcbridge*>(pBri->GetStmModelMidFoot(m_nJijum, bLsd ? m_eTypeLoadLsd : m_eTypeLoad));
	if(!pStmModel) return dDist;

	CFootingApp *pFoot = pBri->GetFootingByJijumNumber(m_nJijum);
	if(pFoot == NULL) return dDist;

	if(pStmModel->m_nTypePosRebarTie == 0)	// 1단철근위치
	{
		dDist = pFoot->m_dCoverLower[0];
	}
	else if(pStmModel->m_nTypePosRebarTie == 1)	// 철근의도심
	{
		CConcBasic *pConc = pBri->GetConcDataFooting(m_nJijum, FALSE);
		if(pConc)
			dDist = pConc->GetRebarDistTension();
	}	
	else if(pStmModel->m_nTypePosRebarTie == 2) // 사용자입력
	{
		dDist = pStmModel->m_dUserPosRebarTie;
	}

	return dDist;
}

HGDOUBLE CCalcStmMidFoot::GetDistCompressStrut()
{
	HGDOUBLE dDist(0);

	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return dDist;

	HGBOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	CStmModelForArcbridge *pStmModel = dynamic_cast<CStmModelForArcbridge*>(pBri->GetStmModelMidFoot(m_nJijum, bLsd ? m_eTypeLoadLsd : m_eTypeLoad));
	if(!pStmModel) return dDist;

	CFootingApp *pFoot = pBri->GetFootingByJijumNumber(m_nJijum);
	if(pFoot == NULL) return dDist;

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
		CConcBasic *pConc = pBri->GetConcDataFooting(m_nJijum, FALSE);
		if(pConc)
		{
			double Fck = pBri->GetValueFck(ePartInFoot);		// CGS 단위 그대로 사용
			double Fy = pBri->GetValueFy(ePartInFoot);

			double dc = pConc->GetRebarDistCompression();
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
		CConcBasic *pConc = pBri->GetConcDataFooting(m_nJijum, FALSE);
		if(pConc)
			dDist = pConc->GetRebarDistCompression();
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
