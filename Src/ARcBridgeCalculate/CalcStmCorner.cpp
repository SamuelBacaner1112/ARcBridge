#include "stdafx.h"
#include "ARcBridgeCalculate.h"


CCalcStmCorner::CCalcStmCorner(CARcBridgeCalcStm* pCalcStm)
{
	m_pCalcStm = pCalcStm;
}


CCalcStmCorner::~CCalcStmCorner(void)
{
}

void CCalcStmCorner::SetPosCorner( HGINT32 ixStmKey )
{
	m_ixStmKey = ixStmKey;
	m_bStt = (ixStmKey == STM_MODEL_CORNER_STT_UPPER_PLUS || ixStmKey == STM_MODEL_CORNER_STT_LOWER_PLUS || ixStmKey == STM_MODEL_CORNER_STT_UPPER_MINUS || ixStmKey == STM_MODEL_CORNER_STT_LOWER_MINUS
		|| ixStmKey == STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1 || ixStmKey == STM_MODEL_CORNER_STT_LOWER_MINUS_EXT_1 || ixStmKey == STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_1 || ixStmKey == STM_MODEL_CORNER_STT_LOWER_PLUS_EXT_1 
		|| ixStmKey == STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_2 || ixStmKey == STM_MODEL_CORNER_STT_LOWER_MINUS_EXT_2 || ixStmKey == STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_2 || ixStmKey == STM_MODEL_CORNER_STT_LOWER_PLUS_EXT_2)? TRUE : FALSE;

	m_bUpper = (ixStmKey == STM_MODEL_CORNER_STT_UPPER_PLUS || ixStmKey == STM_MODEL_CORNER_END_UPPER_PLUS || ixStmKey == STM_MODEL_CORNER_STT_UPPER_MINUS || ixStmKey == STM_MODEL_CORNER_END_UPPER_MINUS
		|| ixStmKey == STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1 || ixStmKey == STM_MODEL_CORNER_END_UPPER_MINUS_EXT_1 || ixStmKey == STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_1 || ixStmKey == STM_MODEL_CORNER_END_UPPER_PLUS_EXT_1
		|| ixStmKey == STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_2 || ixStmKey == STM_MODEL_CORNER_END_UPPER_MINUS_EXT_2 || ixStmKey == STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_2 || ixStmKey == STM_MODEL_CORNER_END_UPPER_PLUS_EXT_2)? TRUE : FALSE;

	m_bPlus = ((ixStmKey > STM_MODEL_CORNER_END_LOWER_MINUS && STM_MODEL_FOOT_STT_MAXP > ixStmKey) 
		|| (ixStmKey > STM_MODEL_CORNER_END_LOWER_MINUS_EXT_1 && STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_2 > ixStmKey)
		|| (ixStmKey > STM_MODEL_CORNER_END_LOWER_MINUS_EXT_2 && STM_MODEL_FOOT_STT_MAX_AXIAL > ixStmKey) ) ? TRUE : FALSE;

	if(ixStmKey < STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1)
		m_eTypeEnv = eEnvLimit;
	else if(ixStmKey < STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_2)
		m_eTypeEnv = eEnvExtremeI;
	else
		m_eTypeEnv = eEnvExtremeII;
}

void CCalcStmCorner::MakeStmModelByTemplete( CStmModelForArcbridge *pStmModel, HGBOOL bStressFlow )
{
	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return;
	CARcBridgeDataStd* pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	m_pCalcStm->SetConcData(TRUE, TRUE, FALSE);

	CStmCorner *pStmCorner = new CStmCorner;
	pStmModel->SetStmModifiedFlag(FALSE);
	pStmModel->ClearStm(TRUE);
	pStmModel->SetPlane(eStmPlaneXz);

	pStmModel->SetThickModel(bLsd ? frM(UNIT_METER) : UNIT_METER);
	pStmModel->SetLsdDesign(bLsd);

	BOOL bVertDir	= (pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH && pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)? TRUE : pBri->IsVertDir();
	pBri->SyncBridgeRebar(FALSE, bVertDir?-1:1, TRUE);

	if(bLsd)
	{
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

			pConfig->m_dFck	= pBri->GetValueFck(m_bUpper? ePartUpperSlab : ePartLowerSlab, TRUE);
			pConfig->m_dFy	= pBri->GetValueFy(m_bUpper? ePartUpperSlab : ePartLowerSlab, FALSE, TRUE);
			pConfig->m_dFvy	= pBri->GetValueFy(m_bUpper? ePartUpperSlab : ePartLowerSlab, TRUE, TRUE);
			pConfig->m_dFt	= GetValueUnitChange(pBri->GetValueStmFctk(m_bUpper? eLsdStmCornerUpper : eLsdStmCornerLower), UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);
			pConfig->m_dEc	= pBri->GetValueEc(m_bUpper? ePartUpperSlab : ePartLowerSlab, TRUE);
			pConfig->m_dEs	= GetValueUnitChange(pData->m_Es, UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);
		}
	}

	pStmCorner->SetStmModel(pStmModel);
	SetCornerTemplate(pStmCorner);
	SetCornerLoad(pStmCorner);

	pStmCorner->MakeStableElement(TRUE);
	pStmCorner->m_eTypeLoadStressFlow = (EStmStressFlowType)pStmModel->m_nTypeLoadStressFlow;
	pStmCorner->m_ixTypeInnerTensionStirrup = pStmModel->m_nTypeStirrupInnerTension;
	pStmCorner->m_dOffsetRate = pStmModel->m_dUserStirrupInnerTension;
	pStmCorner->m_ixInnerAngle			= pStmModel->m_ixInnerAngle;


	switch (pStmModel->m_nTypeDistCornerRebar)
	{
	case 0:		// R기준 사용자 입력
		pStmCorner->m_dRo	= pStmModel->m_dUserDistCornerRebar;
		break;
	case 1:		// 보강철근 도심
		pStmCorner->m_dRo	= GetDistDanbuRebar();
		break;
	case 2:		// 사용자 입력
		{
			CTwinVector tvR = pBri->GetTvCornerBaseLine(m_bStt, m_bUpper);
			pStmCorner->m_dRo	= pStmModel->m_dUserDistCornerRebar / tvR.GetLength();
		}
		break;	
	default: break;
	}
	
	switch (pStmModel->m_nTypeDistHunchRebar)
	{
	case 0:		// R기준 사용자 입력
		pStmCorner->m_dRi	= pStmModel->m_dUserDistHunchRebar;
		break;
	case 1:		// 헌치철근 위치
		{
			CTwinVector tvR = pBri->GetTvCornerBaseLine(m_bStt, m_bUpper);
			pStmCorner->m_dRi	= 1- ((m_bUpper? pBri->m_dCoverLower_UpperSlab[0] : pBri->m_dCoverUpper_LowerSlab[0]) / tvR.GetLength());
		}
		break;
	case 2:		// 사용자 입력
		{
			CTwinVector tvR = pBri->GetTvCornerBaseLine(m_bStt, m_bUpper);
			pStmCorner->m_dRi	= 1- (pStmModel->m_dUserDistHunchRebar / tvR.GetLength());
		}
		break;	
	default: break;
	}

	if(bStressFlow)
	{
		if( pStmModel->IsLsdDesign() )
			pStmCorner->SetMeshSize(TRUE, frM(pBri->m_dMeshSizeArcCorner));
		else
			pStmCorner->SetMeshSize(TRUE, pBri->m_dMeshSizeArcCorner);
		pStmCorner->MakeStressFlow();
	}
	else
	{
		pStmCorner->MakeModeling(TRUE);
		pStmModel->SetStmModifiedFlag(TRUE);
		pStmModel->ReSetIdNodeAndElement(TRUE);

		// MakeModel 하고 나서 철근을 넣어준다.
		SetCornerRebar(pStmModel, pStmCorner->m_eMomentType);
	}
	
	delete pStmCorner;

	if(bLsd)
	{
		CStmObjLoadConditionCombo *pCombo = pStmModel->GetLoadConditionComboIndex(0);
		pCombo->SetName(pBri->m_strStmModelingCorner[m_bStt? 0 : 1][m_eTypeEnv][m_bUpper? 0 : 1][0]);
	}
}

void CCalcStmCorner::ResetLoadAtModel( CStmModelForArcbridge *pStmModel )
{
	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return;
	CARcBridgeDataStd* pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	
	CStmCorner *pStmCorner = new CStmCorner;

	pStmCorner->SetStmModel(pStmModel);
	SetCornerTemplate(pStmCorner);
	SetCornerLoad(pStmCorner);

	pStmCorner->MakeLoadOnly();
	
	delete pStmCorner;

	if(bLsd)
	{
		CStmObjLoadConditionCombo *pCombo = pStmModel->GetLoadConditionComboIndex(0);
		pCombo->SetName(pBri->m_strStmModelingCorner[m_bStt? 0 : 1][m_eTypeEnv][m_bUpper? 0 : 1][0]);
	}
}

void CCalcStmCorner::SetCornerTemplate( CStmCorner *pStmCorner )
{
	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return;

//	double dThickWall = pBri->GetThickWall(m_bStt? 0 : pBri->m_nQtyJigan, FALSE);

	if(m_bUpper)
		pStmCorner->sDim.eDir = m_bStt ? eStmCornerUpperLeft : eStmCornerUpperRight;
	else
		pStmCorner->sDim.eDir = m_bStt ? eStmCornerLowerLeft : eStmCornerLowerRight;

	pStmCorner->m_eMomentType	= m_bPlus? eStmMomentOpening1 : eStmMomentClose1;

	pStmCorner->sDim.H1		= m_bUpper? pBri->m_dTS : pBri->m_dTS_Lower ;
	pStmCorner->sDim.H2		= pBri->GetThickWall(m_bStt? 0 : pBri->m_nQtyJigan, FALSE);
	pStmCorner->sDim.Hh		= pBri->GetThickHunch(m_bStt? 0 : pBri->m_nQtyJigan, !m_bStt, FALSE, m_bUpper);
	pStmCorner->sDim.Bh		= pBri->GetWidthHunch(m_bStt? 0 : pBri->m_nQtyJigan, !m_bStt, m_bUpper);
	
	// 슬래브 상면 피복
	// Close일때와 Open일때 인장위치가 반대다.
	if(!m_bPlus)
	{
		pStmCorner->m_dPosEleSlabeOuter = GetDistRebarTie(TRUE);
		pStmCorner->m_dPosEleWallOuter = GetDistRebarTie(FALSE);
		pStmCorner->m_dPosEleSlabInner	= GetDistCompressStrut(TRUE);
		pStmCorner->m_dPosEleWallInner	= GetDistCompressStrut(FALSE);
	}
	else
	{
		pStmCorner->m_dPosEleSlabeOuter = GetDistCompressStrut(TRUE);
		pStmCorner->m_dPosEleWallOuter = GetDistCompressStrut(FALSE);
		pStmCorner->m_dPosEleSlabInner	= GetDistRebarTie(TRUE);
		pStmCorner->m_dPosEleWallInner	= GetDistRebarTie(FALSE);
	}

	pStmCorner->m_bUseMaxCouple = pBri->m_bCheckCorner;
}

void CCalcStmCorner::SetCornerLoad( CStmCorner *pStmCorner )
{
	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return;

	HGBOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if( !pData ) return;

	double dMoment(0);
	if(pBri->GetFemManage(0)->IsOutputLoaded())
		dMoment = pBri->GetMomentCorner(m_bStt, FALSE, !m_bPlus, m_bUpper, m_eTypeEnv);

	// 한계상태일때 하중은 단위폭(mm)당으로 구했지만 1m 폭에 재하되는 하중을 준다.
	double dWidth = bLsd? frM(UNIT_METER) : UNIT_METER;

	pStmCorner->m_strLoad	= _T("M");
	pStmCorner->m_dMoment	= dMoment * dWidth;
}

void CCalcStmCorner::SetCornerRebar( CStmModel *pStmModel, HGINT32 nType )
{
	// 철근은 2Cycle 철근을 넣어주기 위해서 Templete으로 모델이 만들어진 후에 Setting 해준다.
	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return;

	CStmDataDefine *pDefine = pStmModel->GetStmDataDefine();
	if(pDefine == NULL) return;

	CConcBasic *pConc = NULL;
	SStmDefineTieType *pTie = NULL;
	SStmDefineTieType *pTieVert = NULL;
	SStmDefineTieType *pTieHori = NULL;
	SStmDefineTieType *pTieSupp = NULL;
	HGINT32 iTie(0);
	HGINT32 nSpan = (m_bStt)? 0 : pBri->m_nQtyJigan-1;
	HGINT32 nJijum = m_bStt? 0 : pBri->m_nQtyJigan;

	double dCtc = pBri->GetCtcRebarMain(TRUE);
	CDPoint vAngleJijum = (pBri->m_nTypePlacingMainRebar_UpperSlab==0) ? pBri->GetAngleJijum(nJijum) : CDPoint(0,1);

	double dB = frM(UNIT_METER)/dCtc;
	double dB_Skew = Round(frM(UNIT_METER)/(dCtc/vAngleJijum.y),2);
	HGDOUBLE dCover(0);

	//수직 주철근
	iTie = pDefine->GetIndexTieTypeByName(STM_REBAR_STR_VERT_REBAR);
	pTieVert = pDefine->GetTieType(iTie);
	//수평 주철근
	iTie = pDefine->GetIndexTieTypeByName(STM_REBAR_STR_HORI_REBAR);
	pTieHori = pDefine->GetTieType(iTie);
	// 보강철근
	iTie = pDefine->GetIndexTieTypeByName(STM_REBAR_STR_SUPPORT_REBAR);
	pTieSupp = pDefine->GetTieType(iTie);

	if(pTieVert)
		ClearVectorPtr(pTieVert->main_rebar);
	if(pTieHori)
		ClearVectorPtr(pTieHori->main_rebar);
	if(pTieSupp)
		ClearVectorPtr(pTieSupp->support_rebar);

	for(HGINT32 nDan=0; nDan<MAX_REBAR_DAN; ++nDan)		// 단수
	{
		CDPoint xyInfo	= CDPoint(0, 0);

		// 수평 철근
		if(pTieHori)
		{
			if(m_bPlus)
				dCover = (m_bUpper)? pBri->m_dCoverLower_UpperSlab[nDan] : pBri->m_dCoverUpper_LowerSlab[nDan];
			else
				dCover = (m_bUpper)? pBri->m_dCoverUpper_UpperSlab[nDan] : pBri->m_dCoverLower_LowerSlab[nDan];
				

			SStmTieMainRebar *pMainRebar = new SStmTieMainRebar;
			pMainRebar->cover			= dCover;

			for(HGINT32 nCycle=0; nCycle<2; ++nCycle)
			{
				if(m_bUpper)
					xyInfo	= pBri->GetInfoMainRebar_UpperSlab(!m_bPlus, nSpan, nCycle, nDan, m_bStt? -1 : 1);
				else
					xyInfo	= pBri->GetInfoMainRebar_LowerSlab(!m_bPlus, nSpan, nCycle, nDan, m_bStt? -1 : 1);

				HGDOUBLE dDia = xyInfo.x;
				pMainRebar->dia[nCycle]		= dDia;
				pMainRebar->count[nCycle]	= (dDia > 0)? dB : 0;

				// 2Cycle 주철근 절곡처리 했을때 보강철근에 포함.
				// ARCBRIDGE-3719 : 외측절점부 철근 검토시에만 적용, 내측인장은 적용 X
				if((nType == eStmMomentClose1 || nType == eStmMomentClose2) && pBri->m_bBendingDanBuRebar && nCycle > 0 && dDia > 0)
				{
					SStmTieSupportRebar *pSupportRebar = new SStmTieSupportRebar;
					pSupportRebar->dia = dDia;
					pSupportRebar->count = dB_Skew;
					pSupportRebar->layer = 1;

					pTieSupp->support_rebar.push_back(pSupportRebar);
				}
			}
			pTieHori->main_rebar.push_back(pMainRebar);
		}

		// 수직 철근
		if(pTieVert)
		{
			dCover = (m_bPlus)? pBri->m_dCoverWallIn[nDan] : pBri->m_dCoverWallOut[nDan];

			SStmTieMainRebar *pMainRebar = new SStmTieMainRebar;
			pMainRebar->cover			= dCover;

			for(HGINT32 nCycle=0; nCycle<2; ++nCycle)
			{
				xyInfo	= pBri->GetInfoMainRebar_Wall(nJijum, nCycle, nDan, m_bUpper? 1 : -1, m_bPlus);
				HGDOUBLE dDia = xyInfo.x;

				pMainRebar->dia[nCycle]		= dDia;
				pMainRebar->count[nCycle]	= (dDia > 0)? dB_Skew : 0;
			}

			pTieVert->main_rebar.push_back(pMainRebar);
		}
	}


	if(nType == eStmMomentClose1)
	{
		HGINT32 nUD = m_bUpper ? iUPPER : iLOWER;
		HGDOUBLE dDia(0);
		// 단절점부 철근

		if(pTieSupp)
		{
			if(pBri->m_bIsDanbuRebar_Cycle1[nUD][m_bStt? 0:1])
			{
				long dan = 0; for(dan = 0; dan < 3; dan++)
				{
					dDia = pBri->m_dDia_DanbuRebar_Cycle1[nUD][m_bStt? 0:1][dan];
					if(dDia > 0)
					{
						SStmTieSupportRebar *pSupportRebar = new SStmTieSupportRebar;
						pSupportRebar->dia = dDia;
						pSupportRebar->count = dB_Skew;
						pSupportRebar->layer = 1;

						pTieSupp->support_rebar.push_back(pSupportRebar);
					}
				}
			}
			
			if(pBri->m_bIsDanbuRebar_Cycle2[nUD][m_bStt? 0:1])
			{
				long dan = 0; for(dan = 0; dan < 3; dan++)
				{
					dDia = pBri->m_dDia_DanbuRebar_Cycle2[nUD][m_bStt? 0:1][dan];
					if(dDia > 0)
					{
						SStmTieSupportRebar *pSupportRebar = new SStmTieSupportRebar;
						pSupportRebar->dia = dDia;
						pSupportRebar->count = dB_Skew;
						pSupportRebar->layer = 1;

						pTieSupp->support_rebar.push_back(pSupportRebar);
					}
				}
			}
		}
	}

	if(nType == eStmMomentOpening1 || nType == eStmMomentClose2)
	{
		// 주철근으로 들어가고 있는 철근을 전단철근 형식으로 바꿔줘야 한다.
		// 단절점부 스트럽 철근
		// (ARCBRIDGE-2647) 전단철근 -> 보강철근 으로 변경.
		iTie = pDefine->GetIndexTieTypeByName(STM_REBAR_STR_SUPPORT_REBAR);
		pTie = pDefine->GetTieType(iTie);

		if(pTie)
		{
			// ClearVectorPtr(pTie->shear_rebar);

			HGINT32 nUD = m_bUpper ? iUPPER : iLOWER;
			HGDOUBLE dDia	= pBri->m_dDiaStirrupAtDanBuRebar[nUD][m_bStt? 0:1];		// 단부 스터럽 철근 직경 (시종점)
			HGDOUBLE dCtc	= pBri->m_dCTC_MainRebar/2 * pBri->m_nCountStirrupCtcMainRebar[nUD][m_bStt? 0:1];
			HGDOUBLE dEa	= pBri->m_nCountStirrupAtDanBuRebar[nUD][m_bStt? 0:1];

			SStmTieSupportRebar *pSupportRebar = new SStmTieSupportRebar;
			pSupportRebar->dia = dDia;
			pSupportRebar->count = 1000 / dCtc;
			pSupportRebar->layer = dEa / 2.; // ARCBRIDGE-3164 : 두개구간에 적용되므로 / 2를 적용


			pTieSupp->support_rebar.push_back(pSupportRebar);
		}
	}

	if(nType == eStmMomentOpening2)
	{
		//슬래브 전단철근
		/*iTie = pDefine->GetIndexTieTypeByName(STM_REBAR_STR_SLAB_SHEAR);
		pTie = pDefine->GetTieType(iTie);

		pConc	= pBri->GetConcDataSlab(nSpan, m_bStt? CHECK_POS_LEFT:CHECK_POS_RIGHT);
		if(pTie && pConc)
		{
			ClearVectorPtr(pTie->shear_rebar);

			SStmTieShearRebar *pShearRebar = new SStmTieShearRebar;
			pShearRebar->dia		= pConc->m_SvDia;
			pShearRebar->count		= pConc->m_SvEa;
			pShearRebar->ctc		= pConc->m_SvCtc;
			pTie->type_shear		= eStmShearVert;
			pTie->shear_rebar.push_back(pShearRebar);
		}

		//벽체 전단철근
		iTie = pDefine->GetIndexTieTypeByName(STM_REBAR_STR_WALL_SHEAR);
		pTie = pDefine->GetTieType(iTie);
		pConc	= pBri->GetConcDataWall(nJijum, m_bUpper? CHECK_POS_TOP:CHECK_POS_BOTTOM, TRUE);
		if(pTie && pConc)
		{
			ClearVectorPtr(pTie->shear_rebar);

			SStmTieShearRebar *pShearRebar = new SStmTieShearRebar;
			pShearRebar->dia		= pConc->m_SvDia;
			pShearRebar->count		= pConc->m_SvEa;
			pShearRebar->ctc		= pConc->m_SvCtc;
			pTie->type_shear		= eStmShearVert;
			pTie->shear_rebar.push_back(pShearRebar);
		}*/
	}
}

HGDOUBLE CCalcStmCorner::GetDistDanbuRebar()
{
	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return 0;

	HGDOUBLE dAreaTot(0);
	HGDOUBLE dTotal(0);

	HGINT32 nStt = m_bStt? iSTT : iEND;
	HGINT32 nUpper = m_bUpper? iUPPER : iLOWER;
	for(HGINT32 nDan=0; nDan<MAX_REBAR_DAN; ++nDan)
	{
		for(HGINT32 nCycle=0; nCycle <2; ++nCycle)
		{
			if(nCycle == 0 && pBri->m_bIsDanbuRebar_Cycle1[nUpper][nStt] == FALSE) continue;
			if(nCycle == 1 && pBri->m_bIsDanbuRebar_Cycle2[nUpper][nStt] == FALSE) continue;

			HGDOUBLE dDia = (nCycle == 0)? pBri->m_dDia_DanbuRebar_Cycle1[nUpper][nStt][nDan] : pBri->m_dDia_DanbuRebar_Cycle2[nUpper][nStt][nDan];
			if(Compare(dDia, 0.0, _T("="), 0.1)) continue;

			HGDOUBLE dUnitArea = GetRebarUnitArea_Fix(dDia);
			HGDOUBLE dDistDanbu = (m_bUpper)? pBri->m_dDanCorner_UpperSlab[nDan] : pBri->m_dDanCorner_LowerSlab[nDan];

			dAreaTot += dUnitArea;
			dTotal += (dUnitArea * dDistDanbu);
		}
	}

	HGDOUBLE dDist = dTotal / dAreaTot;
	
	return dDist;
}

HGDOUBLE CCalcStmCorner::GetDistRebarTie(HGBOOL bSlab)
{
	HGDOUBLE dDist(0);

	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return dDist;

	CStmModelForArcbridge *pStmModel = dynamic_cast<CStmModelForArcbridge*>(pBri->GetStmModelByKey(m_ixStmKey));
	if(!pStmModel) return dDist;

	HGINT32 nJijum = m_bStt? 0 : pBri->m_nQtyJigan;
	HGINT32 nJigan = m_bStt? 0 : pBri->m_nQtyJigan-1;
	
	if(pStmModel->m_nTypePosRebarTie == 0)	// 1단철근위치
	{
		if(bSlab)
		{
			if(!m_bPlus)
				dDist = (m_bUpper)? pBri->m_dCoverUpper_UpperSlab[0] : pBri->m_dCoverLower_LowerSlab[0];
			else
				dDist = (m_bUpper)? pBri->m_dCoverLower_UpperSlab[0] : pBri->m_dCoverUpper_LowerSlab[0];
		}
		else
		{
			dDist = m_bPlus? pBri->m_dCoverWallIn[0] : pBri->m_dCoverWallOut[0];
		}
	}
	else if(pStmModel->m_nTypePosRebarTie == 1)	// 철근의도심
	{
		
		CConcBasic *pConc = NULL;

		if(bSlab)
			pConc = pBri->GetConcDataSlab(nJigan, m_bStt? CHECK_POS_LEFT:CHECK_POS_RIGHT, m_bUpper);
		else
			pConc = pBri->GetConcDataWall(nJijum, m_bUpper? CHECK_POS_TOP:CHECK_POS_BOTTOM, FALSE);

		if(pConc)
			dDist = (m_bPlus)? pConc->GetRebarDistCompression() : pConc->GetRebarDistTension();
	}	
	else if(pStmModel->m_nTypePosRebarTie == 2) // 사용자입력
	{
		dDist = pStmModel->m_dUserPosRebarTie;
	}
	
	return dDist;
}

HGDOUBLE CCalcStmCorner::GetDistCompressStrut(HGBOOL bSlab)
{
	HGDOUBLE dDist(0);

	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return dDist;

	CStmModelForArcbridge *pStmModel = dynamic_cast<CStmModelForArcbridge*>(pBri->GetStmModelByKey(m_ixStmKey));
	if(!pStmModel) return dDist;

	CStmDataConfig *pStmData = pStmModel->GetStmDataConfig();

	HGBOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	HGINT32 nJijum = m_bStt? 0 : pBri->m_nQtyJigan;
	HGINT32 nJigan = m_bStt? 0 : pBri->m_nQtyJigan-1;
	HGDOUBLE dRatePiT_PiS = pStmData->m_dUserRatioPiT_PiS;
	if(pStmData->m_ixRatioPiT_PiS == 0)
	{
		if(bLsd)
			dRatePiT_PiS = pStmData->m_dPi_S / pStmData->m_dPi_C;
		else
			dRatePiT_PiS = pStmData->m_dPiTie / pStmData->m_dPiStrut;
	}

	if(pStmModel->m_nTypePosCompressStrut == 0)	// 등가직사각형 응력블럭 깊이/2 x 강도감소계수비
	{
		CConcBasic *pConc = NULL;

		if(bSlab)
			pConc = pBri->GetConcDataSlab(nJigan, m_bStt? CHECK_POS_LEFT:CHECK_POS_RIGHT, m_bUpper);
		else
			pConc = pBri->GetConcDataWall(nJijum, m_bUpper? CHECK_POS_TOP:CHECK_POS_BOTTOM, TRUE);
		
		if(pConc)
		{
			EPartOfBridge ePart = bSlab? (m_bUpper? ePartUpperSlab : ePartLowerSlab) : (m_bStt? ePartOutWallStt : ePartOutWallEnd);

			double Fck = pBri->GetValueFck(bSlab? ePartUpperSlab : ePart);		// CGS 단위 그대로 사용
			double Fy = pBri->GetValueFy(bSlab? ePartUpperSlab : ePart, FALSE);

			double dc = (m_bPlus)? pConc->GetRebarDistTension() : pConc->GetRebarDistCompression();
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
		if(bSlab)
			dDist = m_bUpper? pBri->m_dCoverLower_UpperSlab[0] : pBri->m_dCoverUpper_LowerSlab[0];
		else
			dDist = m_bPlus? pBri->m_dCoverWallOut[0] : pBri->m_dCoverWallIn[0];

		if(pStmModel->m_nTypePosCompressStrut == 3)
		{
			dDist *= dRatePiT_PiS;
		}
	}
	else if(pStmModel->m_nTypePosCompressStrut == 2 || pStmModel->m_nTypePosCompressStrut == 4)	// 철근의도심 | 철근의도심x 강도감소계수비
	{
		CConcBasic *pConc = NULL;

		if(bSlab)
			pConc = pBri->GetConcDataSlab(nJigan, m_bStt? CHECK_POS_LEFT:CHECK_POS_RIGHT, m_bUpper);
		else
			pConc = pBri->GetConcDataWall(nJijum, m_bUpper? CHECK_POS_TOP:CHECK_POS_BOTTOM, FALSE);

		if(pConc)
			dDist = (m_bPlus)? pConc->GetRebarDistTension() : pConc->GetRebarDistCompression();
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