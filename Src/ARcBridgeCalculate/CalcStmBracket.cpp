#include "stdafx.h"
#include "ARcBridgeCalculate.h"


CCalcStmBracket::CCalcStmBracket(CARcBridgeCalcStm* pCalcStm, BOOL bStt)
{
	m_pCalcStm	= pCalcStm;
	m_bStt = bStt;
}


CCalcStmBracket::~CCalcStmBracket(void)
{
}

void CCalcStmBracket::SetBracketTemplate( CStmBracket *pStmBk )
{
	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return;

	double dThickWall = pBri->GetThickWall(m_bStt? 0 : pBri->m_nQtyJigan, FALSE);

	pStmBk->sDim.dA = GetDistBracketB3();
	pStmBk->sDim.stt = m_bStt ? eStmAbutStt : eStmAbutEnd;
	pStmBk->sDim.W = dThickWall;
	pStmBk->sDim.B = m_bStt ? pBri->m_dBHWS : pBri->m_dBHWE;
	pStmBk->sDim.H1 = m_bStt ? (pBri->m_dBHS - pBri->m_dBHHS) : (pBri->m_dBHE - pBri->m_dBHHE);
	pStmBk->sDim.H2 = m_bStt ? pBri->m_dBHHS : pBri->m_dBHHE;

	double dH1 = GetDistBracketH1();	// 브래킷 상단 이격거리
	double dB2 = GetDistBracketB2();	// 전면벽체 이격거리
	double dB1 = GetDistBracketB1();	// 배면벽체 이격거리

	pStmBk->SetOffsetDist(dB1, dB2, dH1);
}

double CCalcStmBracket::GetDistBracketH1()
{
	return GetDistRebarTie();
}

//압축스트럿
double CCalcStmBracket::GetDistBracketB1()
{
	return GetDistCompressStrut();		// 배면벽체 이격거리
}

//인장타이
double CCalcStmBracket::GetDistBracketB2()
{
	return GetDistRebarTie();	// 전면벽체 이격거리;
}

//집중하중의 거리
double CCalcStmBracket::GetDistBracketB3()
{
	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return 0;

	double dAW  = m_bStt? pBri->m_dBHWS : pBri->m_dBHWE;
	double dNuc = m_dLoadBracket[0];
	double dVu  = dNuc * BRACKET_HORI_FACTOR;
	double dH1 = GetDistBracketH1();		// 브래킷 상단 이격거리

	if(Compare(dVu, 0.0, _T("=")) || Compare(dNuc, 0.0, _T("=")))
	{
		return dAW / 2.;
	}

	return dAW / 2. + (dH1 * dVu / dNuc);
}

void CCalcStmBracket::SetBracketLoad( CStmBracket *pStmBk )
{
	if(pStmBk->m_vStaticLoad.size() > 0)
		pStmBk->m_vStaticLoad.clear();
	
//	CARcBridgeCalcStd *pStd = m_pCalcStm->m_pStd;
	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return;

	HGBOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

//	double dSE = m_bStt ? -1.0 : 1.0;
	
	BOOL bApplySelfWeight = TRUE;
	long nCountLoad = bApplySelfWeight ? 3 : 2;
	pStmBk->m_vStaticLoad.resize(nCountLoad);

	long nIndexLoad = 0;
	double dFactorDead = 1.30;
	double dFactorLive = 2.15;

	if(bLsd)
	{
		CLsdManagerBridge *pLsdManager = pBri->m_plsdManager;
	
		double dEta(0);
		CString szCombo(_T(""));

		CLsdLoadCase *pLoadCase = pBri->m_plsdManager->GetLsdLoadCaseWingWallOrAps(dEta, szCombo, FALSE, FALSE, FALSE, m_bStt);
 		if(pLoadCase)
		{
			dFactorDead = pLoadCase->GetLoadFactor(eLSD_LOADCASE_DC1);
			dFactorLive =	pLoadCase->GetLoadFactor(eLSD_LOADCASE_LL1);
		}
	}

	// 브래킷 자중 : 옵션
	double dUC = GetValueUnitChange(1.0, UNIT_CGS_TONF, pData->m_szTonf);
	if(bApplySelfWeight)
	{
		double dB = toM(pStmBk->sDim.B);
		double dH1 = toM(pStmBk->sDim.H1);
		double dH2 = toM(pStmBk->sDim.H2);
		double dUnitWeight = toTon(pData->m_pBasicConcInfo->m_UWeightSteelConcrete);
		double dSelfWeight = (dB * dH1 + dB * dH2 / 2.) * dUnitWeight * dUC;
		pStmBk->m_vStaticLoad[nIndexLoad].name = _T("SW");
		pStmBk->m_vStaticLoad[nIndexLoad].vert = dSelfWeight;
		pStmBk->m_vStaticLoad[nIndexLoad].hori = dSelfWeight * BRACKET_HORI_FACTOR;
		pStmBk->m_vStaticLoad[nIndexLoad].factor = dFactorDead;
		pStmBk->m_vStaticLoad[nIndexLoad].description = _T("브라켓자중");
		nIndexLoad++;
	}

	// 접속슬래브 하중계산
	CARcBridgeCalcAps *pAps = (m_bStt)? m_pCalcStm->m_pStd->m_pApsStt[0] : m_pCalcStm->m_pStd->m_pApsEnd[0];
	if(!pAps) return;

	pAps->CalculateAll();

	m_dLoadBracket[0] = (pBri->IsAps(m_bStt, TRUE)) ? pAps->GetVForceCorbel(TRUE) : 0.0;
	m_dLoadBracket[1] = (pBri->IsAps(m_bStt, TRUE)) ? pAps->GetVForceCorbel(FALSE) : 0.0;

	if(pBri->IsAps(m_bStt, FALSE))
	{
		pAps = (m_bStt)? m_pCalcStm->m_pStd->m_pApsStt[1] : m_pCalcStm->m_pStd->m_pApsEnd[1];
		if(pAps)
		{
			pAps->CalculateAll();
			if(m_dLoadBracket[0] + m_dLoadBracket[1] < pAps->GetVForceCorbel(TRUE) + pAps->GetVForceCorbel(FALSE))
			{
				m_dLoadBracket[0] = pAps->GetVForceCorbel(TRUE);
				m_dLoadBracket[1] = pAps->GetVForceCorbel(FALSE);
			}
		}
	}

	// 접속슬래브 계산에서 산정되는 고정하중,활하중
	for(long nLoad = 0; nLoad < 2; nLoad++)
	{
		BOOL bDeadLoad = nLoad == 0 ? TRUE : FALSE;
		CString sNameLoad = bDeadLoad ? _T("D") : _T("L");
		CString sDescription = bDeadLoad ? _T("고정하중") : _T("활하중");
		
		HGDOUBLE dVert = pData->IsLsdDesign() ? frTon(m_dLoadBracket[nLoad]) : m_dLoadBracket[nLoad];
		
		pStmBk->m_vStaticLoad[nIndexLoad + nLoad].name = sNameLoad;
		pStmBk->m_vStaticLoad[nIndexLoad + nLoad].vert = dVert;
		pStmBk->m_vStaticLoad[nIndexLoad + nLoad].hori = dVert * BRACKET_HORI_FACTOR;
		pStmBk->m_vStaticLoad[nIndexLoad + nLoad].factor = (nLoad == 0)? dFactorDead : dFactorLive;		// 조합 계수.
		pStmBk->m_vStaticLoad[nIndexLoad + nLoad].description = sDescription;
	}
}

void CCalcStmBracket::SetPosStt( HGBOOL bStt )
{
	m_bStt = bStt;
}

void CCalcStmBracket::MakeStmModelByTemplete( CStmModel *pStmModel, HGBOOL bStressFlow )
{
	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return;
	CARcBridgeDataStd* pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();

	m_pCalcStm->SetConcData(FALSE, TRUE, FALSE);

	CStmBracket *pStmBk = new CStmBracket;
	pStmModel->SetStmModifiedFlag(FALSE);
	pStmModel->ClearStm(TRUE);

	pStmModel->SetPlane(eStmPlaneXz);

	pStmModel->SetThickModel(pData->IsLsdDesign() ? frM(UNIT_METER) : UNIT_METER);
	pStmModel->SetLsdDesign(pData->IsLsdDesign());

	if(bLsd)
	{
		CStmDataConfig *pConfig = pStmModel->GetStmDataConfig();
		if(pConfig)
		{
			CLsdResistCoefMatCtl * pResistCoefCtl = &(pData->m_plsdEnv->GetLsdResistCoefMatCtl());

			ELSDCombKind eComboKind(eLSD_COMBINE_LIMIT_I);
			
			pConfig->m_dPi_C = pResistCoefCtl->GetCoefficient(eComboKind, eResistCoefMaterial_Concrete);
			pConfig->m_dPi_S = pResistCoefCtl->GetCoefficient(eComboKind, eResistCoefMaterial_Ferro);

			pConfig->m_dFck	= pBri->GetValueFck(ePartUpperSlab, TRUE);
			pConfig->m_dFy	= pBri->GetValueFy(ePartUpperSlab, FALSE, TRUE);
			pConfig->m_dFvy	= pBri->GetValueFy(ePartUpperSlab, TRUE, TRUE);
			pConfig->m_dFt	= GetValueUnitChange(pBri->GetValueStmFctk(m_bStt? eLsdStmBracketStt : eLsdStmBracketEnd), UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);
			pConfig->m_dEc	= pBri->GetValueEc(ePartUpperSlab, TRUE);
			pConfig->m_dEs	= GetValueUnitChange(pData->m_Es, UNIT_CGS_KGF_CM2, pData->m_szKgf_CM2);
		}
	}

	pStmBk->SetStmModel(pStmModel);
	SetBracketTemplate(pStmBk);
	SetBracketLoad(pStmBk);

	if(bStressFlow)
	{
		if(pStmModel->IsLsdDesign()) 
			pStmBk->SetMeshSize(TRUE, frM(pBri->m_dMeshSizeBracket));
		else
			pStmBk->SetMeshSize(TRUE, pBri->m_dMeshSizeBracket);
		pStmBk->MakeStressFlow();
	}
	else
	{
		pStmBk->MakeModeling(FALSE, TRUE);

		// MakeModel 하고 나서 철근을 넣어준다.
		SetBracketRebar(pStmModel);
	}
	pStmModel->SetStmModifiedFlag(TRUE);

	delete pStmBk;

	if(bLsd)
	{
		CStmObjLoadConditionCombo *pCombo = pStmModel->GetLoadConditionComboIndex(0);
		pCombo->SetName(pBri->m_strStmModelingBracket[m_bStt? 0 : 1]);
	}
}

void CCalcStmBracket::ResetLoadAtModel( CStmModel *pStmModel )
{
	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return;
	CARcBridgeDataStd* pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	CStmBracket *pStmBk = new CStmBracket;


	pStmBk->SetNodeBracketLoad(1);

	pStmBk->SetStmModel(pStmModel);
	SetBracketTemplate(pStmBk);
	SetBracketLoad(pStmBk);

	pStmBk->MakeLoadOnly();

	delete pStmBk;

	if(bLsd)
	{
		CStmObjLoadConditionCombo *pCombo = pStmModel->GetLoadConditionComboIndex(0);
		pCombo->SetName(pBri->m_strStmModelingBracket[m_bStt? 0 : 1]);
	}
}

void CCalcStmBracket::SetBracketRebar( CStmModel *pStmModel )
{
	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return;

	CStmDataDefine *pDefine = pStmModel->GetStmDataDefine();
	if(pDefine == NULL) return;

	SStmDefineTieType *pTie = NULL;
	HGINT32 iTie(0);
	HGINT32 nJijum = m_bStt? 0 : pBri->m_nQtyJigan;
	double dCtc = pBri->GetCtcRebarMain(TRUE);
	CDPoint vAngleJijum = (pBri->m_nTypePlacingMainRebar_UpperSlab==0) ? pBri->GetAngleJijum(nJijum) : CDPoint(0,1);
	double dB = frM(UNIT_METER)/(dCtc/vAngleJijum.y);

	//외측 주철근(벽체 내측)
	iTie = pDefine->GetIndexTieTypeByName(STM_REBAR_STR_OUTTER_REBAR);
	pTie = pDefine->GetTieType(iTie);
	if(pTie)
	{
		ClearVectorPtr(pTie->main_rebar);

		for(HGINT32 nDan=0; nDan<MAX_REBAR_DAN; ++nDan)
		{
			HGDOUBLE dCover = pBri->m_dCoverWallIn[nDan];
			SStmTieMainRebar *pMainRebar = new SStmTieMainRebar;

			pMainRebar->cover			= dCover;

			for(HGINT32 nCycle=0; nCycle<2; ++nCycle)
			{
				HGDOUBLE dDia = pBri->GetInfoMainRebar_Wall(nJijum, nCycle, nDan, 1, TRUE).x;
				if(Compare(dDia, 0.0, _T("="), 0.01)) continue;

				pMainRebar->dia[nCycle]		= dDia;
				pMainRebar->count[nCycle]	= dB;				
			}

			pTie->main_rebar.push_back(pMainRebar);
		}
	}

	//내측 주철근(벽체 외측)
	iTie = pDefine->GetIndexTieTypeByName(STM_REBAR_STR_INNER_REBAR);
	pTie = pDefine->GetTieType(iTie);
	if(pTie)
	{
		ClearVectorPtr(pTie->main_rebar);

		for(HGINT32 nDan=0; nDan<MAX_REBAR_DAN; ++nDan)
		{
			HGDOUBLE dCover = pBri->m_dCoverWallOut[nDan];

			SStmTieMainRebar *pMainRebar = new SStmTieMainRebar;
			pMainRebar->cover			= dCover;

			for(HGINT32 nCycle=0; nCycle<2; ++nCycle)
			{
				HGDOUBLE dDia = pBri->GetInfoMainRebar_Wall(nJijum, nCycle, nDan, 1, FALSE).x;
				if(Compare(dDia, 0.0, _T("="), 0.01)) continue;

				pMainRebar->dia[nCycle]		= dDia;
				pMainRebar->count[nCycle]	= (dDia > 0)? dB : 0;
			}
			pTie->main_rebar.push_back(pMainRebar);
		}
	}
	
	//보강철근
	iTie = pDefine->GetIndexTieTypeByName(STM_REBAR_STR_MAIN_REBAR);
	pTie = pDefine->GetTieType(iTie);
	if(pTie)
	{
		ClearVectorPtr(pTie->main_rebar);

		HGDOUBLE dCover = pBri->m_dCoverWallOut[0];

		SStmTieMainRebar *pMainRebar = new SStmTieMainRebar;
		pMainRebar->cover			= dCover;

		for(HGINT32 nCycle=0; nCycle<2; ++nCycle)
		{
			HGBOOL bCheck = pBri->m_bIsBracketRebar[nCycle][m_bStt ? iSTT : iEND];
			HGDOUBLE dDia = (nCycle == 0)? pBri->m_dDiaRebarMain_Bracket_Cycle1[m_bStt ? iSTT : iEND] : pBri->m_dDiaRebarMain_Bracket_Cycle2[m_bStt ? iSTT : iEND];
			if(bCheck == FALSE || Compare(dDia, 0.0, _T("="), 0.01)) continue;

			pMainRebar->dia[nCycle]		= dDia;
			pMainRebar->count[nCycle]	= (dDia > 0)? dB : 0;
		}
		pTie->main_rebar.push_back(pMainRebar);
	}
	
	//전단철근
	iTie = pDefine->GetIndexTieTypeByName(STM_REBAR_STR_SHEAR_REBAR);
	pTie = pDefine->GetTieType(iTie);
	if(pTie)
	{
		ClearVectorPtr(pTie->shear_rebar);

		SStmTieShearRebar *pShearRebar = new SStmTieShearRebar;

		CRebarShearDetail *pShearDetail = &pBri->m_RebarShearDetail_Bracket[m_bStt ? iSTT : iEND];

		// 폐합스터럽 설정... 2/3d의 범위만
		CTwinVectorArray tvArr;
		pBri->GetTvRebarShear_VertSection_Bracket(tvArr, m_bStt);
		long nDanShear = tvArr.GetSize();
		pShearRebar->dia    = pShearDetail->m_dDia;
		pShearRebar->ctc     = pBri->m_dCTC_ShearRebarBracket;
		double dSvEa = 1000 / (dCtc * pBri->m_nCountWrapMainRebarForBracket[m_bStt ? iSTT : iEND]) * 2;
		pShearRebar->count    = (pShearDetail->m_nType == TYPE_SHEAR_DIGUT || pShearDetail->m_nType == TYPE_SHEAR_SINGLE_HOOK)? dSvEa/2 : dSvEa;
		pTie->type_shear		= eStmShearHori;
		pTie->shear_rebar.push_back(pShearRebar);
	}
}

HGDOUBLE CCalcStmBracket::GetDistRebarTie()
{
	HGDOUBLE dDist(0);

	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return dDist;

	CStmModelForArcbridge *pStmModel = dynamic_cast<CStmModelForArcbridge*>(pBri->GetStmModelBracket(m_bStt));
	if(!pStmModel) return dDist;

	HGINT32 nJijum = m_bStt? 0 : pBri->m_nQtyJigan;

	if(pStmModel->m_nTypePosRebarTie == 0)	// 1단철근위치
	{
		dDist = pBri->m_dCoverWallIn[0];
	}
	else if(pStmModel->m_nTypePosRebarTie == 2) // 사용자입력
	{
		dDist = pStmModel->m_dUserPosRebarTie;
	}
	else
	{
		BOOL bDeleteData = FALSE;
		CConcBasic *pConc = pBri->GetConcDataWall(nJijum, CHECK_POS_TOP, TRUE);
		if(pConc == NULL)
		{
			// 외측벽체를 벽체로 검토 할때는 벽체 concdata를 만들지 않는다. 새로 만들어준다.
			pConc = new CConcBasic;
			pBri->SetConcDataOutWall(pConc, eEnvLimit, CHECK_POS_TOP);
			bDeleteData = TRUE;
		}
		if(pConc)
		{
			if(pStmModel->m_nTypePosRebarTie == 1)	// 철근의도심
			{
				dDist = pConc->GetRebarDistCompression();
			}			
		}
		if(bDeleteData)
			SAFE_DELETE(pConc);
	}
	
	return dDist;
}

HGDOUBLE CCalcStmBracket::GetDistCompressStrut()
{
	HGDOUBLE dDist(0);

	CRcBridgeRebar *pBri = m_pCalcStm->m_pStd->m_pBri;
	if(!pBri) return dDist;

	CStmModelForArcbridge *pStmModel = dynamic_cast<CStmModelForArcbridge*>(pBri->GetStmModelBracket(m_bStt));
	if(!pStmModel) return dDist;

	HGBOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	HGINT32 nJijum = m_bStt? 0 : pBri->m_nQtyJigan;
	CStmDataConfig *pStmData = pStmModel->GetStmDataConfig();
	HGDOUBLE dRatePiT_PiS = pStmData->m_dUserRatioPiT_PiS;
	if(pStmData->m_ixRatioPiT_PiS == 0)
	{
		if(bLsd)
			dRatePiT_PiS = pStmData->m_dPi_S / pStmData->m_dPi_C;
		else
			dRatePiT_PiS = pStmData->m_dPiTie / pStmData->m_dPiStrut;
	}

	if(pStmModel->m_nTypePosCompressStrut == 1 || pStmModel->m_nTypePosCompressStrut == 3)	// 1단철근위치(피복) | 1단철근위치(피복)x강도감소계수비
	{
		dDist = pBri->m_dCoverWallOut[0];

		if(pStmModel->m_nTypePosCompressStrut == 3)
		{
			dDist *= dRatePiT_PiS;
		}
	}
	else if(pStmModel->m_nTypePosCompressStrut == 5) // 사용자입력
	{
		dDist = pStmModel->m_dUserPosCompressStrut;
	}
	else
	{
		BOOL bDeleteData = FALSE;
		CConcBasic *pConc = pBri->GetConcDataWall(nJijum, CHECK_POS_TOP, TRUE);
		if(pConc == NULL)
		{
			// 외측벽체를 벽체로 검토 할때는 벽체 concdata를 만들지 않는다. 새로 만들어준다.
			pConc = new CConcBasic;
			pBri->SetConcDataOutWall(pConc, eEnvLimit, CHECK_POS_TOP);
			bDeleteData = TRUE;
		}
		if(pConc)
		{
			if(pStmModel->m_nTypePosCompressStrut == 0)	// 등가직사각형 응력블럭 깊이/2 x 강도감소계수비
			{
				double Fck = pBri->GetValueFck(ePartUpperSlab);		// CGS 단위 그대로 사용
				double Fy = pBri->GetValueFy(ePartUpperSlab, FALSE);

				double dc = pConc->GetRebarDistTension();
				double dEs = pBri->m_pARcBridgeDataStd->m_Es;
				double dHeight = pConc->m_Ho;
				double dDist1 = dHeight - dc;
				double cDist = UltimateEs / (UltimateEs + Fy / dEs) * dDist1;
				double aDist = GetK1Value(Fck) * cDist;

				double dHeightBlock = aDist;

				dDist = dHeightBlock * dRatePiT_PiS / 2;
			}
			else if(pStmModel->m_nTypePosCompressStrut == 2 || pStmModel->m_nTypePosCompressStrut == 4)	// 철근의도심 | 철근의도심x 강도감소계수비
			{
				dDist = pConc->GetRebarDistTension();
				if(pStmModel->m_nTypePosCompressStrut == 4)
				{
					dDist *= dRatePiT_PiS;
				}
			}
		}
		if(bDeleteData)
			SAFE_DELETE(pConc);
	}

	return dDist;
}
