#include "stdafx.h"
#include "ARcBridgeData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define ApplyLoadcaseDefine(i) \
	CLsdLoadCase *pLoadCase_##i = new CLsdLoadCase(Type);

#define ApplyLoadcaseDefineChild(i, j) \
	CLsdLoadCase *pLoadCase_##i = new CLsdLoadCase(Type, *pLoadCase_##j);

#define ApplyLoadcase(i, loadcase, unitload, bMaxMin, bSetValue) \
	ApplyLoadcaseDefine(i) \
	ApplyLoadcaseChildSet(i, loadcase, unitload, bMaxMin, bSetValue);

#define ApplyLoadcaseChild(i, ii, loadcase, unitload, bMaxMin, bSetValue) \
	ApplyLoadcaseDefineChild(i, ii) \
	ApplyLoadcaseChildSet(i, loadcase, unitload, bMaxMin, bSetValue);		

#define ApplyLoadcaseChildSet(i, loadcase, unitload, bMaxMin, bSetValue) \
	if(IsSafeValidLoad(loadcase)) \
	pLoadCase_##i->SetData(loadcase, bSetValue ? pLoad->GetUnitLoad(unitload)->GetValue(bMaxMin) : 0)

#define ApplyLoadcaseChildSetSideLoad(i, loadcase, unitload, bMaxMin, bSetValue, bLeft) \
	if(IsSafeValidLoad(loadcase) && GetDesignLoad(GetLoadSelectKindFromLoadCaseKind(loadcase))->IsLoadedSide(m_bForCaltilever ? eLSD_LOADCASE_DIR_PER : eLSD_LOADCASE_DIR_BRI, bLeft)) \
	pLoadCase_##i->SetData(loadcase, bSetValue ? pLoad->GetUnitLoad(unitload)->GetValue(bMaxMin) : 0)

#define CASE_S(i,c) \
	for(long i=0; i<c; ++i) {

#define CASE_E(i,c) \
	if(pLoadCase_##i!=NULL) delete pLoadCase_##i; \
	}


CLsdManagerBridge::CLsdManagerBridge(CRcBridgeRebar *pBri, BOOL bForCaltilever)
{
	m_pBri = pBri;
	m_bForCaltilever	 = bForCaltilever;
	ClearEnum();
	InitEnum();
}

CLsdManagerBridge::~CLsdManagerBridge(void)
{
}

void CLsdManagerBridge::SetDataDefaultSelectLoad()
{
	if(m_bForCaltilever)
		SetDataDefaultSelectLoadForCantilever();
	else
		SetDataDefaultSelectLoadForBridge();
}

void CLsdManagerBridge::InitEnum()
{
	if(m_bForCaltilever)
		InitEnumForCantilever();
	else
		InitEnumForBridge();
}

void CLsdManagerBridge::SetDataDefaultLimitCombineSet( ELSDLoadGroupKind Type, ELSDLoadLoadingDir Dir, BOOL bApplyWater)
{
	if(GetLsdDesignLoadArray().empty())
		return;

	// 교대같은 형태를 위한 예외 처리
	if(Dir == eLSD_LOAD_LOADING_DIR_BOTH && bApplyWater == FALSE)
		GetExtraColumn(Type, m_bForCaltilever ? eLSD_LOADCASE_DIR_PER : eLSD_LOADCASE_DIR_BRI).clear();

	CString szTitle[eLSD_COMBINATION_COUNT] = { NOTRANS("ST"), NOTRANS("EX"), NOTRANS("SE"), NOTRANS("FG") };
	CString szDir = (Dir == eLSD_LOAD_LOADING_DIR_BOTH)? _T("B") : ((Dir == eLSD_LOAD_LOADING_DIR_L)? _T("L") : _T("R"));
	CString szNW = bApplyWater? _T("W") : _T("N");

	if(m_bForCaltilever)
	{
		szDir = _T("");
		szNW = _T("");
	}

	long nIndexCombDetail = 0;
	long nCntSubsid = (m_pBri->m_bSelectEnvSubsid)? 1 : m_pBri->GetCountSubsidence();
	if(nCntSubsid == 0) nCntSubsid = 1;
	SetCountUseSubsidLoad(nCntSubsid);

	for(ELSDCombKind iCom=eLSD_COMBINE_LIMIT_I; iCom<eLSD_COMBINE_TOTAL_COUNT; NextStep(iCom))
 	{	
		if(GetLoadGroupKindByCombKind(iCom) != Type)
			continue;

		if(Dir == eLSD_LOAD_LOADING_DIR_BOTH && bApplyWater == FALSE)
			ClearVecLsdLoadCase(iCom, m_bForCaltilever ? eLSD_LOADCASE_DIR_PER : eLSD_LOADCASE_DIR_BRI);

		// 극단한계상태조합에서는 양방향 재하를 하지않는다.
		if(Dir == eLSD_LOAD_LOADING_DIR_BOTH && (iCom == eLSD_COMBINE_EXTREME_I || iCom == eLSD_COMBINE_EXTREME_II) && m_bForCaltilever == FALSE)
			continue;

		CLsdLoad *pLoad = GetLoad(static_cast<ELSDCombKind>(iCom));
		if(pLoad == NULL) continue;
		if(!pLoad->GetUse())
		{
			nIndexCombDetail++;
			continue;					
		}

		CString szPrefix, szNameCnv;
		szPrefix.Format(NOTRANS("%s%d%s%s"), szTitle[Type], 1 + nIndexCombDetail++, szNW, szDir);

		std::vector<CLsdUnitLoadGroup *> UnitGroupArray = pLoad->GetUnitGroupArray();
		if(UnitGroupArray.size() > 0)
		{	
			auto ite = UnitGroupArray.begin();
			long nMaxCase = ((*ite)->GetOptionType() == eFactor_rP) ? 2 : 1;
			long nIdx(1);

			for(long n=0; n<nMaxCase; n++)
			{		
				std::vector<std::pair<ELSDLoadCaseKind, ELSDUnitLoadKind>> LiveKind;

				long nCaseLive = 0;
				BOOL bLiveLoad = IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_LL1, eLSD_UNITLOAD_LL);
				if(TRUE)	// 하중조합을 돌리기 위해서는 무조건 한개는 들어가줘야 한다.
				{
					long nSeleOpt = GetDesignLoad(eLSD_LOADSELECT_LL1)->GetIndexSelectOfType(m_bForCaltilever ? eLSD_LOADCASE_DIR_PER : eLSD_LOADCASE_DIR_BRI);

					// 활하중 선택옵션이 활하중,탱크 및 중차량하중(극한2) 일경우
					if(nSeleOpt == 2 && iCom == eLSD_COMBINE_LIMIT_II)
					{
						++nCaseLive; LiveKind.push_back(std::make_pair(eLSD_LOADCASE_LL2, eLSD_UNITLOAD_LL));
					}
					else
					{
						++nCaseLive; LiveKind.push_back(std::make_pair(eLSD_LOADCASE_LL1, eLSD_UNITLOAD_LL));
						if(bLiveLoad && nSeleOpt == 1)
						{
							++nCaseLive; LiveKind.push_back(std::make_pair(eLSD_LOADCASE_LL2, eLSD_UNITLOAD_LL));
						}
					}
				}

				long nCaseDC = 1; // DC 는 무조건 있는 경우로 처리한다.
				long nCaseEH = ((*ite)->GetOptionType() == eFactor_rP) ? (IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_EH, eLSD_UNITLOAD_EH) ? 2 : 1) : 1;
				long nCaseEV = ((*ite)->GetOptionType() == eFactor_rP) ? (IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_EV, eLSD_UNITLOAD_EV) ? 2 : 1) : 1;
				long nCaseES = ((*ite)->GetOptionType() == eFactor_rP) ? (IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_ES, eLSD_UNITLOAD_ES) ? 2 : 1) : 1;
				long nCaseSH = ((*ite)->GetOptionType() == eFactor_rP) ? (IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_SH, eLSD_UNITLOAD_SH) ? 2 : 1) : 1;
				long nCaseTU = IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_TU, eLSD_UNITLOAD_TU) ? 2 : 1;
				long nCaseW = IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_WA, eLSD_UNITLOAD_WA)? 1 : 1;
				long nCaseLS = 1;
				long nCaseEQ = 1;
				long nCaseEtc = 1;
				long nCaseWS = IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_WS, eLSD_UNITLOAD_WS) ? 2 : 1;
				long nCaseCT = (m_bForCaltilever && IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_CT, eLSD_UNITLOAD_CT)) ? 2 : 1;
				
		
				long nCaseGamma = ((ite + 1) != UnitGroupArray.end() && (*(ite + 1))->GetOptionType() == eFactor_rEQ) && 
					(IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_LL1, eLSD_UNITLOAD_LL) ||
					IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_IM, eLSD_UNITLOAD_IM) ||
					IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_BR, eLSD_UNITLOAD_BR) ||
					IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_PL, eLSD_UNITLOAD_PL) ||
					IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_LS, eLSD_UNITLOAD_LS) ||
					IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_CF, eLSD_UNITLOAD_CF)) ? 2 : 1;

				CASE_S(iG, nCaseGamma)
					ApplyLoadcaseDefine(iG);
				CASE_S(i0,nCaseLive)							
					ApplyLoadcase(i0, LiveKind[i0].first, LiveKind[i0].second, n==0, iG == 0);
				CASE_S(i1,nCaseDC)			
					ApplyLoadcaseDefineChild(i1, i0);	
					ApplyLoadcaseChildSet(i1, eLSD_LOADCASE_DC1,eLSD_UNITLOAD_DC,n==0,i1==0);
					ApplyLoadcaseChildSet(i1, eLSD_LOADCASE_DW1,eLSD_UNITLOAD_DW,n==0,i1==0);
				
				CASE_S(i2,nCaseEH);
				// 수평하중 계열은 편재하일때 MAx/MIN을 쓰지않고 MAX만 사용한다.
				BOOL bMaxHorzentalLoad = (i2==0 || Dir != eLSD_LOAD_LOADING_DIR_BOTH)? TRUE : FALSE;
				ApplyLoadcaseDefineChild(i2, i1);	
				if(Dir == eLSD_LOAD_LOADING_DIR_BOTH)
				{
					ApplyLoadcaseChildSet(i2, bApplyWater? eLSD_LOADCASE_EHW1 : eLSD_LOADCASE_EH, eLSD_UNITLOAD_EH, bMaxHorzentalLoad, TRUE);
					ApplyLoadcaseChildSet(i2, bApplyWater? eLSD_LOADCASE_EHW2 : eLSD_LOADCASE_EH2, eLSD_UNITLOAD_EH, bMaxHorzentalLoad, TRUE);
				}
				else
				{
					ELSDLoadCaseKind eLoadCase = (Dir == eLSD_LOAD_LOADING_DIR_L)? (bApplyWater? eLSD_LOADCASE_EHW1 : eLSD_LOADCASE_EH) : (bApplyWater? eLSD_LOADCASE_EHW2 : eLSD_LOADCASE_EH2);
 					ApplyLoadcaseChildSet(i2, eLoadCase, eLSD_UNITLOAD_EH, bMaxHorzentalLoad, TRUE);
				}
				CASE_S(i3,nCaseES);
				ApplyLoadcaseDefineChild(i3, i2);
				if(Dir == eLSD_LOAD_LOADING_DIR_BOTH)
				{
					ApplyLoadcaseChildSet(i3, eLSD_LOADCASE_ES, eLSD_UNITLOAD_ES, bMaxHorzentalLoad, TRUE);
					ApplyLoadcaseChildSet(i3, eLSD_LOADCASE_ES2, eLSD_UNITLOAD_ES, bMaxHorzentalLoad, TRUE);
				}
				else
				{
					ApplyLoadcaseChildSet(i3, (Dir == eLSD_LOAD_LOADING_DIR_L)? eLSD_LOADCASE_ES : eLSD_LOADCASE_ES2, eLSD_UNITLOAD_ES, bMaxHorzentalLoad, TRUE);
				}
				CASE_S(i4,nCaseLS);
				ApplyLoadcaseDefineChild(i4, i3);
				if(Dir == eLSD_LOAD_LOADING_DIR_BOTH)
				{
					ApplyLoadcaseChildSet(i4, eLSD_LOADCASE_LS, eLSD_UNITLOAD_LS, bMaxHorzentalLoad, iG ==0);
					ApplyLoadcaseChildSet(i4, eLSD_LOADCASE_LS2, eLSD_UNITLOAD_LS, bMaxHorzentalLoad, iG ==0);
				}
				else
				{
					ApplyLoadcaseChildSet(i4, (Dir == eLSD_LOAD_LOADING_DIR_L)? eLSD_LOADCASE_LS : eLSD_LOADCASE_LS2, eLSD_UNITLOAD_LS, bMaxHorzentalLoad, iG==0);
				}
				
				CASE_S(i5,nCaseEV);
				ApplyLoadcaseDefineChild(i5, i4);
				if(bApplyWater)
				{
					ApplyLoadcaseChildSet(i5, eLSD_LOADCASE_EVW,eLSD_UNITLOAD_EV, n==0, TRUE);
					ApplyLoadcaseChildSet(i5, eLSD_LOADCASE_EVVW,eLSD_UNITLOAD_EV, n==0, TRUE);
				}
				else
				{
					ApplyLoadcaseChildSet(i5, eLSD_LOADCASE_EV,eLSD_UNITLOAD_EV, n==0, TRUE);
				}
				ApplyLoadcaseChildSet(i5, eLSD_LOADCASE_EVI,eLSD_UNITLOAD_EV,n==0, TRUE);
				
				CASE_S(i6,nCaseSH);
				ApplyLoadcaseChild(i6, i5, eLSD_LOADCASE_SH,eLSD_UNITLOAD_SH,i6==0,TRUE);

				CASE_S(i7, nCaseW);
				ApplyLoadcaseDefineChild(i7, i6);
				ApplyLoadcaseChildSet(i7, eLSD_LOADCASE_WA1,eLSD_UNITLOAD_WA,n==0,bApplyWater);
				ApplyLoadcaseChildSet(i7, eLSD_LOADCASE_WA21,eLSD_UNITLOAD_WA,n==0,TRUE);

				CASE_S(i8, nCaseEQ);
				ApplyLoadcaseDefineChild(i8, i7);
				if(m_pBri->IsBoxType())
				{
					// BOX 타입일때 등가정적 해석의 구체 관성력
					ApplyLoadcaseChildSetSideLoad(i8, (Dir == eLSD_LOAD_LOADING_DIR_L) ? eLSD_LOADCASE_EQ21 : eLSD_LOADCASE_EQ22, eLSD_UNITLOAD_EQ, TRUE, TRUE, i8==0);
				}
				else
				{
					ApplyLoadcaseChildSetSideLoad(i8, (Dir == eLSD_LOAD_LOADING_DIR_L) ? eLSD_LOADCASE_EQ11 : eLSD_LOADCASE_EQ12, eLSD_UNITLOAD_EQ, TRUE, TRUE, i8==0);
				}

				CASE_S(i9,nCaseTU);		
				ApplyLoadcaseDefineChild(i9, i8);	
				if(IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_TU, eLSD_UNITLOAD_TU))
				{
					ApplyLoadcaseChildSet(i9, (i9 == 0 ? eLSD_LOADCASE_TU21 : eLSD_LOADCASE_TU22),eLSD_UNITLOAD_TU,TRUE, TRUE);
				}
				if(IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_TG, eLSD_UNITLOAD_TG))
				{
					ApplyLoadcaseChildSet(i9, (i9 == 0 ? eLSD_LOADCASE_TGP : eLSD_LOADCASE_TGM),eLSD_UNITLOAD_TG, TRUE, TRUE);
				}
				
				ApplyLoadcaseChildSet(i9, eLSD_LOADCASE_PL,eLSD_UNITLOAD_PL, n==0, iG ==0);
//				ApplyLoadcaseChildSet(i9, eLSD_LOADCASE_SD,eLSD_UNITLOAD_SD, n==0, TRUE);
				ApplyLoadcaseChildSet(i9, eLSD_LOADCASE_BP,eLSD_UNITLOAD_BP, n==0, TRUE);
				ApplyLoadcaseChildSet(i9, eLSD_LOADCASE_BPI,eLSD_UNITLOAD_BP, n==0, TRUE);

				CASE_S(i10,nCaseEtc);
				ApplyLoadcaseDefineChild(i10, i9);
				if(IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_IM, eLSD_UNITLOAD_IM))
				{
					ApplyLoadcaseChildSet(i10, eLSD_LOADCASE_IM,eLSD_UNITLOAD_IM, TRUE, iG ==0);
				}
				if(IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_BR, eLSD_UNITLOAD_BR))
				{
					ApplyLoadcaseChildSet(i10, eLSD_LOADCASE_BR,eLSD_UNITLOAD_BR, TRUE, iG ==0);
				}
				if(IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_CF, eLSD_UNITLOAD_CF))
				{
					ApplyLoadcaseChildSet(i10, eLSD_LOADCASE_CF,eLSD_UNITLOAD_CF, TRUE, iG ==0);
				}
				if(IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_WP, eLSD_UNITLOAD_WP))
				{
					ApplyLoadcaseChildSet(i10, eLSD_LOADCASE_WP2,eLSD_UNITLOAD_WP, TRUE, TRUE);
				}
				if(IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_FR, eLSD_UNITLOAD_FR))
				{
					ApplyLoadcaseChildSet(i10, eLSD_LOADCASE_FR,eLSD_UNITLOAD_FR, TRUE, TRUE);
				}
				if(IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_GD, eLSD_UNITLOAD_GD))
				{
					ApplyLoadcaseChildSet(i10, eLSD_LOADCASE_GD,eLSD_UNITLOAD_GD, TRUE, TRUE);
				}
				if(IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_IC, eLSD_UNITLOAD_IC))
				{
					ApplyLoadcaseChildSet(i10, eLSD_LOADCASE_IC,eLSD_UNITLOAD_IC, TRUE, TRUE);
				}
				if(IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_CV, eLSD_UNITLOAD_CV))
				{
					ApplyLoadcaseChildSet(i10, eLSD_LOADCASE_CV1,eLSD_UNITLOAD_CV, TRUE, TRUE);
				}

				CASE_S(i11,nCaseWS);
				ApplyLoadcaseDefineChild(i11, i10);

				if(IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_WS, eLSD_UNITLOAD_WS))
				{
					ApplyLoadcaseChildSet(i11, (i11 == 0)? eLSD_LOADCASE_WS11 : eLSD_LOADCASE_WS12, eLSD_UNITLOAD_WS, TRUE, TRUE);
				}

				CASE_S(i12,nCaseCT);
				ApplyLoadcaseDefineChild(i12, i11);

				if(IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_CT, eLSD_UNITLOAD_CT))
				{
					ApplyLoadcaseChildSet(i12, (i12 == 0)? eLSD_LOADCASE_CT11 : eLSD_LOADCASE_CT12, eLSD_UNITLOAD_CT, TRUE, TRUE);
				}

				// 지점침하 갯수만큼 조합을 한다.

				CASE_S(i13, nCntSubsid);
				ApplyLoadcaseDefineChild(i13, i12);
				if(IsSafeValidSelectLoadAndUnitLoad(pLoad, eLSD_LOADSELECT_SD, eLSD_UNITLOAD_SD))
				{
					ApplyLoadcaseChildSet(i13, ELSDLoadCaseKind(eLSD_LOADCASE_SD1 + i13), eLSD_UNITLOAD_SD, TRUE, TRUE);
				}

				CLsdLoadCase *pLoadCase = new CLsdLoadCase(Type, *pLoadCase_i13);
				pLoadCase->SetOption(n==0 ? eLoadcaseOption_Max : eLoadcaseOption_Min);
					szNameCnv.Format(NOTRANS("%s%02d"), szPrefix, nIdx);
				pLoadCase->SetName(szNameCnv);
				pLoadCase->SetLsdLoadComboKind(iCom);
				pLoadCase->SetIndexCombo(nIdx - 1);
				if(LookupLoadcase(iCom,m_bForCaltilever ? eLSD_LOADCASE_DIR_PER : eLSD_LOADCASE_DIR_BRI,pLoadCase)) { ++nIdx; }

				CASE_E(i13, nCntSubsid);
				CASE_E(i12, nCaseCT);
				CASE_E(i11, nCaseWS);
				CASE_E(i10, nCaseEtc);
				CASE_E(i9, nCaseTU);
				CASE_E(i8, nCaseEQ);
				CASE_E(i7, nCaseW);
				CASE_E(i6, nCaseSH);
				CASE_E(i5, nCaseEV);
				CASE_E(i4, nCaseLS);
				CASE_E(i3, nCaseES);
				CASE_E(i2, nCaseEH);
				CASE_E(i1,nCaseDC);							
				CASE_E(i0,nCaseLive);
 				CASE_E(iG,nCaseGamma);
			}								
 		}				
 	}
}

BOOL CLsdManagerBridge::IsValidColumn( ELSDLoadGroupKind GroupKind, ELSDLoadCaseDir Dir, ELSDLoadCaseKind Case, BOOL bCheckUse )
{
	HGINT32 iSize = GetSizeLsdDesignLoad();

	if(!IsSafeValidLoad(Case))
		return FALSE;

	BOOL bResult = FALSE;
	for(int ix=0; ix<eLSD_COMBINE_TOTAL_COUNT; ++ix)
	{
		if(GetLoadGroupKindByCombKind(static_cast<ELSDCombKind>(ix)) == GroupKind)
		{					
			CLsdLoad *pLoad = GetLoad(static_cast<ELSDCombKind>(ix));
			if(IsSafeValidLoadGroup(pLoad, Case))
				bResult = TRUE;
		}
	}	
	if(!bResult)
		return FALSE;

	BOOL bValild = TRUE;
	BOOL bApplyGroundWaterLevel = m_pBri->m_bApplyGroundWaterLevel;

	switch(Case)
	{
	case eLSD_LOADCASE_EHW1:
	case eLSD_LOADCASE_EHW2:
	case eLSD_LOADCASE_EVW:
	case eLSD_LOADCASE_WA1:
	case eLSD_LOADCASE_EVVW:
		bValild = (bApplyGroundWaterLevel == FALSE)? FALSE : bValild;
		break;
	case eLSD_LOADCASE_EQ11:
	case eLSD_LOADCASE_EQ12:
		bValild = (m_pBri->IsBoxType() == TRUE)? FALSE : bValild;
		break;
	case eLSD_LOADCASE_EQ21:
	case eLSD_LOADCASE_EQ22:
		bValild = (m_pBri->IsBoxType() == FALSE)? FALSE : bValild;
		break;
	case eLSD_LOADCASE_EVI:
		bValild = (m_pBri->m_nSelectInteriorWaterPress != 0 || m_pBri->IsBoxType() == FALSE)? FALSE : bValild;
		break;
	case eLSD_LOADCASE_BPI:
		bValild = (m_pBri->m_nSelectInteriorWaterPress != 1 || m_pBri->IsBoxType() == FALSE)? FALSE : bValild;
		break;
	case eLSD_LOADCASE_WA21:
		bValild = (m_pBri->m_nSelectInteriorWaterPress != 2 || m_pBri->IsBoxType() == FALSE)? FALSE : bValild;
		break;
	case eLSD_LOADCASE_SD:
		bValild = FALSE;
		break;
	default:
		break;
	}
	
	if(Case >= eLSD_LOADCASE_SD1 && Case <= eLSD_LOADCASE_SD20)
	{
		if(m_pBri->m_bSelectEnvSubsid)
		{
			bValild = (Case > eLSD_LOADCASE_SD1)? FALSE : TRUE;
		}
		else
		{
			long nCnt = m_pBri->GetCountSubsidence();
			bValild = (Case - eLSD_LOADCASE_SD1 < nCnt)? TRUE : FALSE;
		}
	}

	// 하중조합에 실제로 사용 된하중인지 판단한다.
	if(bCheckUse && IsUseLoadCaseKindInCombination(GroupKind, Dir, Case) == FALSE)
		bValild = FALSE;

	return bValild;
}

BOOL CLsdManagerBridge::IsValidColumn( ELSDCombKind eKind, ELSDLoadCaseDir Dir, ELSDLoadCaseKind Case )
{
	ELSDLoadGroupKind CombKind = GetLoadGroupKindByCombKind(eKind);
	
	return IsValidColumn(CombKind, Dir, Case);
}

CString CLsdManagerBridge::GetStringSelectLoadLsd( ELSDLoadSelectKind eSelectKind, BOOL bMark/*=FALSE*/ )
{
	CString str(_T(""));
	switch (eSelectKind)
	{
	case eLSD_LOADSELECT_DC:
		str = bMark? _T("DC") : _T("고정하중+SW");	break;
	case eLSD_LOADSELECT_DW:
		str = bMark? _T("DW") : _T("고정하중");	break;
	case eLSD_LOADSELECT_EH:
		str = bMark? _T("EH") : _T("수평토압");	break;
	case eLSD_LOADSELECT_EV:
		str = bMark? _T("EV") : _T("연직토압");	break;
	case eLSD_LOADSELECT_ES:
		str = bMark? _T("ES") : _T("상재하중");	break;
	case eLSD_LOADSELECT_LS:
		str = bMark? _T("LS") : _T("상재활하중");	break;
	case eLSD_LOADSELECT_LL1:
		str = bMark? _T("LL") : _T("활하중");	break;
	case eLSD_LOADSELECT_LL2:
		str = bMark? _T("LLT") : _T("중차량하중");	break;
	case eLSD_LOADSELECT_LL3:
		str = bMark? _T("LLEQ") : _T("지진시 활하중");	break;
	case eLSD_LOADSELECT_PL:
		str = bMark? _T("PL") : _T("보도하중");	break;
	case eLSD_LOADSELECT_BP:
		str = bMark? _T("BP") : _T("부력");	break;
	case eLSD_LOADSELECT_WA:
		str = bMark? _T("WA") : _T("정수압");	break;
	case eLSD_LOADSELECT_SD:
		str = bMark? _T("SD") : _T("지점침하");	break;
	case eLSD_LOADSELECT_TU:
		str = bMark? _T("TU") : _T("온도하중");	break;
	case eLSD_LOADSELECT_TG:
		str = bMark? _T("TG") : _T("온도차하중");	break;
	case eLSD_LOADSELECT_SH:
		str = bMark? _T("SH") : _T("건조수축");	break;
	case eLSD_LOADSELECT_EQ:
		str = bMark? _T("EQ") : _T("지진하중");	break;
	case eLSD_LOADSELECT_DD:
		str = bMark? _T("DD") : _T("말뚝 부마찰력");	break;
	case eLSD_LOADSELECT_EL:
		str = bMark? _T("EL") : _T("시공중 발생하는 구속응력");	break;
	case eLSD_LOADSELECT_PS:
		str = bMark? _T("PS") : _T("프리스트레스힘");	break;
	case eLSD_LOADSELECT_CR:
		str = bMark? _T("CR") : _T("크리프");	break;
	case eLSD_LOADSELECT_IM:
		str = bMark? _T("IM") : _T("충격하중");	break;
	case eLSD_LOADSELECT_BR:
		str = bMark? _T("BR") : _T("제동하중");	break;
	case eLSD_LOADSELECT_CF:
		str = bMark? _T("CF") : _T("원심하중");	break;
	case eLSD_LOADSELECT_WP:
		str = bMark? _T("WP") : _T("파압");	break;
	case eLSD_LOADSELECT_WS:
		str = bMark? _T("WS") : _T("풍하중");	break;
	case eLSD_LOADSELECT_WL:
		str = bMark? _T("WL") : _T("활하중에 작용하는 풍하중");	break;
	case eLSD_LOADSELECT_FR:
		str = bMark? _T("FR") : _T("마찰력");	break;
	case eLSD_LOADSELECT_GD:
		str = bMark? _T("GD") : _T("지반변동");	break;
	case eLSD_LOADSELECT_IC:
		str = bMark? _T("IC") : _T("설하중");	break;
	case eLSD_LOADSELECT_CT:
		str = bMark? _T("CT") : _T("차량충돌하중");	break;
	case eLSD_LOADSELECT_CV:
		str = bMark? _T("CV") : _T("선박충돌하중");	break;
	default:
		break;
	}
	
	return str;
}

CString CLsdManagerBridge::GetStringLoadCaseLsd( ELSDLoadCaseKind eLoadCaseKind, BOOL bMark/*=FALSE*/ )
{
	CString str(_T(""));
	switch (eLoadCaseKind)
	{
	case eLSD_LOADCASE_DC1:
		str = bMark? _T("DC") : _T("고정하중+SW");	break;
	case eLSD_LOADCASE_DW1:
		str = bMark? _T("DW") : _T("고정하중");	break;
	case eLSD_LOADCASE_EH:
		str = bMark? _T("EHL") : _T("좌측수평토압(지하수미작용)");	break;
	case eLSD_LOADCASE_EH2:
		str = bMark? _T("EHR") : _T("우측수평토압(지하수미작용)");	break;
	case eLSD_LOADCASE_EHW1:
		str = bMark? _T("EHWL") : _T("좌측수평토압(지하수작용)");	break;
	case eLSD_LOADCASE_EHW2:
		str = bMark? _T("EHWR") : _T("우측수평토압(지하수작용)");	break;
	case eLSD_LOADCASE_EV:
		str = bMark? _T("EV") : _T("연직토압");	break;
	case eLSD_LOADCASE_EVW:
		str = bMark? _T("EVW") : _T("연직토압(지하수작용)");	break;
	case eLSD_LOADCASE_EVVW:
		str = bMark? _T("EVVW") : _T("연직수압");	break;
	case eLSD_LOADCASE_EVI:
		str = bMark? _T("EVi") : _T("내부연직수압");	break;
	case eLSD_LOADCASE_ES:
		str = bMark? _T("ESL") : _T("좌측상재하중, 수평하중");	break;
	case eLSD_LOADCASE_ES2:
		str = bMark? _T("ESR") : _T("우측상재하중, 수평하중");	break;
	case eLSD_LOADCASE_SH:
		str = bMark? _T("SH") : _T("건조수축");	break;
	case eLSD_LOADCASE_LL1:
		str = bMark? _T("LL") : _T("차량하중");	break;
	case eLSD_LOADCASE_LL2:
		str = bMark? _T("LLT") : _T("탱크 및 중차량 하중");	break;
	case eLSD_LOADCASE_PL:
		str = bMark? _T("PL") : _T("보도하중");	break;
	case eLSD_LOADCASE_LS:
		str = bMark? _T("LSL") : _T("좌측상재활하중, 수평하중");	break;
	case eLSD_LOADCASE_LS2:
		str = bMark? _T("LSR") : _T("우측상재활하중, 수평하중");	break;
	case eLSD_LOADCASE_WA1:
		str = bMark? _T("WA") : _T("정수압");	break;
	case eLSD_LOADCASE_WA21:
		str = bMark? _T("WAI") : _T("내부연직수압");	break;
	case eLSD_LOADCASE_BP:
		str = bMark? _T("BP") : _T("부력");	break;
	case eLSD_LOADCASE_BPI:
		str = bMark? _T("BPI") : _T("내부연직수압");	break;
	case eLSD_LOADCASE_TU21:
		str = bMark? _T("TUP") : _T("온도하중 (+)");	break;
	case eLSD_LOADCASE_TU22:
		str = bMark? _T("TUM") : _T("온도하중 (-)");	break;
	case eLSD_LOADCASE_TGP:
		str = bMark? _T("TGP") : _T("온도구배 (+)");	break;
	case eLSD_LOADCASE_TGM:
		str = bMark? _T("TGM") : _T("온도구배 (-)");	break;
	case eLSD_LOADCASE_EQ11:
		str = bMark? _T("EQL") : _T("좌측종방향 지진하중");	break;
	case eLSD_LOADCASE_EQ12:
		str = bMark? _T("EQR") : _T("우측종방향 지진하중");	break;
	case eLSD_LOADCASE_EQ21:
		str = bMark? _T("EQSWL") : _T("좌측종방향 지진하중");	break;
	case eLSD_LOADCASE_EQ22:
		str = bMark? _T("EQSWR") : _T("우측종방향 지진하중");	break;
	case eLSD_LOADCASE_IM:
		str = bMark? _T("IM") : _T("충격하중");	break;
	case eLSD_LOADCASE_CF:
		str = bMark? _T("CF") : _T("충돌하중");	break;
	case eLSD_LOADCASE_WS11:
		str = bMark? _T("WSL") : _T("좌측종방향 풍하중");	break;
	case eLSD_LOADCASE_WS12:
		str = bMark? _T("WSR") : _T("우측종방향 풍하중");	break;
	default:
		break;
	}

	if(eLoadCaseKind >= eLSD_LOADCASE_SD1 && eLoadCaseKind <= eLSD_LOADCASE_SD20)
	{
		str.Format(_T("%s%d"), (bMark? _T("SD") : _T("지점침하")), eLoadCaseKind - eLSD_LOADCASE_SD1 +1);
	}

	return str;
}

void CLsdManagerBridge::SetLoadDirOption()
{
	map<ELSDLoadSelectKind, CLsdDesignLoad *> mapLoad = GetLsdDesignLoadArray();

	auto ite = mapLoad.find(eLSD_LOADSELECT_EH);
	if(ite == mapLoad.end())
		return;

	CString szOpt = (*ite).second->GetValueVer();

	ite = mapLoad.find(eLSD_LOADSELECT_ES);
	if(ite != mapLoad.end())
	{
		(*ite).second->SetValueVer(szOpt);
	}

	ite = mapLoad.find(eLSD_LOADSELECT_LS);
	if(ite != mapLoad.end())
	{
		(*ite).second->SetValueVer(szOpt);
	}
}

BOOL CLsdManagerBridge::IsSelectedLoadDesign( ELSDLoadSelectKind eKind )
{
	BOOL bSelected(FALSE);
	map<ELSDLoadSelectKind, CLsdDesignLoad *> mapDesignLoad = GetLsdDesignLoadArray();

	auto ite = mapDesignLoad.find(eKind);
	
	if( ite != mapDesignLoad.end() )
	{
		bSelected = ite->second->GetUse();
	}

	return bSelected;
}

BOOL CLsdManagerBridge::IsSafeValidLoad( ELSDLoadCaseKind Case )
{
	return CLsdManager::IsSafeValidLoad(Case);
}

ELSDLoadSelectKind CLsdManagerBridge::GetLoadSelectKindFromLoadCaseKind( ELSDLoadCaseKind casekind )
{
	return CLsdManager::GetLoadSelectKindFromLoadCaseKind(casekind);
}

BOOL CLsdManagerBridge::IsUsedLoadCaseAtComboination( ELSDLoadGroupKind eComboKind, ELSDLoadCaseKind Case )
{
	BOOL bUseLoadCase(FALSE);
	for(ELSDCombKind iCom=eLSD_COMBINE_LIMIT_I; iCom<eLSD_COMBINE_TOTAL_COUNT; NextStep(iCom))
	{	
		if(GetLoadGroupKindByCombKind(iCom) != eComboKind)
			continue;

		CLsdLoad *pLoad = GetLoad(static_cast<ELSDCombKind>(iCom));
		if(pLoad == NULL) continue;
		if(!pLoad->GetUse()) continue;

		std::vector<CLsdLoadCase *> vLoadCase = GetLsdLoadCaseArray(iCom, m_bForCaltilever ? eLSD_LOADCASE_DIR_PER : eLSD_LOADCASE_DIR_BRI);
		for(auto ite=vLoadCase.begin(); ite != vLoadCase.end(); ++ite)
		{
			double dFactor = (*ite)->GetLoadFactor(Case);
			if(dFactor != 0.0)
			{
				bUseLoadCase = TRUE;
				break;
			}
		}

		if(bUseLoadCase)
			break;
	}

	return bUseLoadCase;
}

double CLsdManagerBridge::GetValueLoadModifiedFactor( ELSDCombKind ekind, long ixPart, CLsdLoadCase *pLsdLoadCase/*=NULL*/ )
{
	double dFactor(0);
	long nSelect = (pLsdLoadCase)? pLsdLoadCase->GetOption() : 1;

	long nIndexModifiyed = GetindexLoadModifiedFactor(ixPart);
	
	switch(nSelect)
	{
	case 0:
		dFactor = GetLsdEtaiValueMaxLoadModified(nIndexModifiyed, ekind);	break;
	case 1:
		dFactor = GetLsdEtaiValueStandardLoadModified(nIndexModifiyed, ekind);	break;
	case 2:
		dFactor = GetLsdEtaiValueMinLoadModified(nIndexModifiyed, ekind); break;
	default:
		break;
	}

	return dFactor;
}

double CLsdManagerBridge::GetLoadFactor( long  nTypeCombo, CString sCombo, ELSDLoadCaseKind Case )
{
	double dFactor(0);

	ELSDCombKind eCombStt(eLSD_COMBINE_LIMIT_I);
	ELSDCombKind eCombEnd(eLSD_COMBINE_LIMIT_I);

	switch(nTypeCombo)
	{
	case eEnvLimit:
		eCombStt = eLSD_COMBINE_LIMIT_I;
		eCombEnd = eLSD_COMBINE_LIMIT_V;
		break;
	case eEnvExtremeI:
		eCombStt = eLSD_COMBINE_EXTREME_I;
		eCombEnd = eLSD_COMBINE_EXTREME_I;
		break;
	case eEnvExtremeII:
		eCombStt = eLSD_COMBINE_EXTREME_II;
		eCombEnd = eLSD_COMBINE_EXTREME_II;
		break;
	case eEnvService:
		eCombStt = eLSD_COMBINE_SERVICE_I;
		eCombEnd = eLSD_COMBINE_SERVICE_V;
		break;
	default:
		eCombStt = eLSD_COMBINE_LIMIT_I;
		eCombEnd = eLSD_COMBINE_SERVICE_V;
		break;
	}

	for(ELSDCombKind iCom=eCombStt; iCom<=eCombEnd; NextStep(iCom))
	{	
		CLsdLoad *pLoad = GetLoad(static_cast<ELSDCombKind>(iCom));
		if(pLoad == NULL) continue;
		if(!pLoad->GetUse()) continue;

		BOOL bFind(FALSE);
		std::vector<CLsdLoadCase *> vLoadCase = GetLsdLoadCaseArray(iCom, m_bForCaltilever ? eLSD_LOADCASE_DIR_PER : eLSD_LOADCASE_DIR_BRI);
		for(auto ite=vLoadCase.begin(); ite != vLoadCase.end(); ++ite)
		{
			if((*ite)->GetName() == sCombo)
			{
				dFactor = (*ite)->GetLoadFactor(Case);
				bFind = TRUE;
				break;
			}
			
		}

		if(bFind)
			break;
	}

	return dFactor;
}

void CLsdManagerBridge::GetStringArrayLsdLoadCaseKindForUserLoad( CStringArray &sArr )
{
	sArr.RemoveAll();

	CString strLoad;
	for(long ix=0; ix<eLSD_LOADCASE_SIZE; ++ix)
	{
		ELSDLoadCaseKind eCase = static_cast<ELSDLoadCaseKind>(ix);

		for(long nGroup = eLSD_COMBINATION_LIMIT; nGroup <= eLSD_COMBINATION_SERVICE; nGroup++)
		{
			if(IsValidColumn(static_cast<ELSDLoadGroupKind>(nGroup), m_bForCaltilever ? eLSD_LOADCASE_DIR_PER : eLSD_LOADCASE_DIR_BRI, eCase))
			{
				strLoad = GetDesignNameByIndex(eCase);
				sArr.Add(strLoad);
				break;
			}
		}
	}
}

long CLsdManagerBridge::GetIndexLsdLoadCaseKindForUserLoad( CString sLsdLoadCase )
{
	CStringArray sArr;
	GetStringArrayLsdLoadCaseKindForUserLoad(sArr);

	CString str;
	long nSize = sArr.GetSize();
	for(long i = 0; i < nSize; i++)
	{
		str = sArr.GetAt(i);
		if(str == sLsdLoadCase)
		{
			return i;
		}
	}

	return -1;
}

CLsdLoadCase * CLsdManagerBridge::GetLsdLoadCaseWingWallOrAps( HGDOUBLE &dEta, CString &strName, HGBOOL bUse, HGBOOL bJudge, BOOL bWingWall, BOOL bStt )
{
	long nComboKindStt = bUse ? eLSD_COMBINE_SERVICE_I : eLSD_COMBINE_LIMIT_I;
	long nComboKindEnd = bUse ? eLSD_COMBINE_SERVICE_V : eLSD_COMBINE_LIMIT_V;

	EPartOfBridge ePart;
	long nExtraCol(0);
	if(bWingWall)
	{
		ePart = bStt? ePartWingWallStt : ePartWingWallEnd;
		nExtraCol = bUse? (bJudge ? LSD_DESIGN_WINGWALL_SERVICE_JUDGE : LSD_DESIGN_WINGWALL_SERVICE_CHECK) : LSD_DESIGN_WINGWALL;
	}
	else
	{
		ePart = bStt? ePartApsStt : ePartApsEnd;
		nExtraCol = bUse? (bJudge ? LSD_DESIGN_APPROACH_SERVICE_JUDGE : LSD_DESIGN_APPROACH_SERVICE_CHECK) : LSD_DESIGN_APPROACH;
	}

	// 사용한계상태 날개벽 이나 접속슬래브 일때 추가된 항목이 초기화되지 않은경우 처리
	if(bUse)
	{
		for(long nComboKind = nComboKindStt; nComboKind <= nComboKindEnd; nComboKind++)
		{
			CLsdLoadCase *pLoadCase = GetLsdLoadCase((ELSDCombKind)nComboKind, m_bForCaltilever ? eLSD_LOADCASE_DIR_PER : eLSD_LOADCASE_DIR_BRI, 0);
			if(pLoadCase && pLoadCase->GetSizeExtraValueArray() <= LSD_DESIGN_APPROACH_SERVICE_CHECK)
			{
				if(nExtraCol == LSD_DESIGN_WINGWALL_SERVICE_JUDGE || nExtraCol == LSD_DESIGN_WINGWALL_SERVICE_CHECK) nExtraCol = LSD_DESIGN_WINGWALL;
				if(nExtraCol == LSD_DESIGN_APPROACH_SERVICE_JUDGE || nExtraCol == LSD_DESIGN_APPROACH_SERVICE_CHECK) nExtraCol = LSD_DESIGN_APPROACH;
				break;
			}
		}
	}

	// Radio Button의 선택 값은 해당 조합에 BOOL값으로 저장되는것이 아니라 마지막 데이터에 Index로 저장된다.
	// 마지막 LoadCase에서 Index를 찾는다.
	long nIndexRadioButton(-1);
	for(long nComboKind = nComboKindEnd; nComboKind >= nComboKindStt; nComboKind--)
	{
		std::vector<CLsdLoadCase *> vecLoadCase = GetLsdLoadCaseArray((ELSDCombKind)nComboKind, m_bForCaltilever ? eLSD_LOADCASE_DIR_PER : eLSD_LOADCASE_DIR_BRI);

		long nSizeCombo = vecLoadCase.size();
		for(long nCombo = nSizeCombo-1; nCombo >= 0; nCombo--)
		{
			CLsdLoadCase *pLoadCase = vecLoadCase[nCombo];

			if(pLoadCase->GetSizeExtraValueArray() <=  nExtraCol)	continue;

			nIndexRadioButton = pLoadCase->GetExtraValueArray()[nExtraCol];
			break;
		}
		if(nIndexRadioButton >= 0)
			break;
	}

	long nIndex(0);
	for(long nComboKind = nComboKindStt; nComboKind <= nComboKindEnd; nComboKind++)
	{
		std::vector<CLsdLoadCase *> vecLoadCase = GetLsdLoadCaseArray((ELSDCombKind)nComboKind, m_bForCaltilever ? eLSD_LOADCASE_DIR_PER : eLSD_LOADCASE_DIR_BRI);

		long nSizeCombo = vecLoadCase.size();
		for(long nCombo = 0; nCombo < nSizeCombo; nCombo++)
		{
			if(nIndex == nIndexRadioButton)
			{
				CLsdLoadCase *pLoadCase = vecLoadCase[nCombo];

				dEta = GetValueLoadModifiedFactor((ELSDCombKind)nComboKind, ePart, pLoadCase);
				strName = pLoadCase->GetName();
				return pLoadCase;
			}
			++nIndex;
		}
	}

	return NULL;
}

void CLsdManagerBridge::InitEnumForBridge()
{
	SetEnum(NOTRANS("DC"),		eLSD_LOADCASE_DC1,		eLSD_LOADSELECT_DC,         eLSD_UNITLOAD_DC);
	SetEnum(NOTRANS("DW"),		eLSD_LOADCASE_DW1,		eLSD_LOADSELECT_DW,			eLSD_UNITLOAD_DW);
	SetEnum(NOTRANS("EHL"),		eLSD_LOADCASE_EH,		eLSD_LOADSELECT_EH,			eLSD_UNITLOAD_EH);
	SetEnum(NOTRANS("EHR"),		eLSD_LOADCASE_EH2,		eLSD_LOADSELECT_EH,			eLSD_UNITLOAD_EH);
	SetEnum(NOTRANS("EHWL"),	eLSD_LOADCASE_EHW1,		eLSD_LOADSELECT_EH,			eLSD_UNITLOAD_EH);
	SetEnum(NOTRANS("EHWR"),	eLSD_LOADCASE_EHW2,		eLSD_LOADSELECT_EH,			eLSD_UNITLOAD_EH);
	SetEnum(NOTRANS("EV"),		eLSD_LOADCASE_EV,		eLSD_LOADSELECT_EV,			eLSD_UNITLOAD_EV);
	SetEnum(NOTRANS("EVW"),		eLSD_LOADCASE_EVW,		eLSD_LOADSELECT_EV,			eLSD_UNITLOAD_EV);
	SetEnum(NOTRANS("EVVW"),	eLSD_LOADCASE_EVVW,		eLSD_LOADSELECT_EV,			eLSD_UNITLOAD_EV);
	SetEnum(NOTRANS("EVi"),		eLSD_LOADCASE_EVI,		eLSD_LOADSELECT_EV,			eLSD_UNITLOAD_EV);
	SetEnum(NOTRANS("ESL"),		eLSD_LOADCASE_ES,		eLSD_LOADSELECT_ES,			eLSD_UNITLOAD_ES);
	SetEnum(NOTRANS("ESR"),		eLSD_LOADCASE_ES2,		eLSD_LOADSELECT_ES,			eLSD_UNITLOAD_ES);
	SetEnum(NOTRANS("SH"),		eLSD_LOADCASE_SH,		eLSD_LOADSELECT_SH,			eLSD_UNITLOAD_SH);
	SetEnum(NOTRANS("LL"),		eLSD_LOADCASE_LL1,		eLSD_LOADSELECT_LL1,		eLSD_UNITLOAD_LL);
	SetEnum(NOTRANS("LLT"),		eLSD_LOADCASE_LL2,		eLSD_LOADSELECT_LL1,		eLSD_UNITLOAD_LL);
	SetEnum(NOTRANS("PL"),		eLSD_LOADCASE_PL,		eLSD_LOADSELECT_PL,			eLSD_UNITLOAD_PL);
	SetEnum(NOTRANS("LSL"),		eLSD_LOADCASE_LS,		eLSD_LOADSELECT_LS,			eLSD_UNITLOAD_LS);
	SetEnum(NOTRANS("LSR"),		eLSD_LOADCASE_LS2,		eLSD_LOADSELECT_LS,			eLSD_UNITLOAD_LS);
	SetEnum(NOTRANS("WA"),		eLSD_LOADCASE_WA1,		eLSD_LOADSELECT_WA,			eLSD_UNITLOAD_WA);
	SetEnum(NOTRANS("WAI"),		eLSD_LOADCASE_WA21,		eLSD_LOADSELECT_WA,			eLSD_UNITLOAD_WA);
	SetEnum(NOTRANS("BP"),		eLSD_LOADCASE_BP,		eLSD_LOADSELECT_BP,			eLSD_UNITLOAD_BP);
	SetEnum(NOTRANS("BPI"),		eLSD_LOADCASE_BPI,		eLSD_LOADSELECT_BP,			eLSD_UNITLOAD_BP);
	SetEnum(NOTRANS("TUP"),		eLSD_LOADCASE_TU21,		eLSD_LOADSELECT_TU,			eLSD_UNITLOAD_TU);
	SetEnum(NOTRANS("TUM"),		eLSD_LOADCASE_TU22,		eLSD_LOADSELECT_TU,			eLSD_UNITLOAD_TU);
	SetEnum(NOTRANS("TGP"),		eLSD_LOADCASE_TGP,		eLSD_LOADSELECT_TG,			eLSD_UNITLOAD_TG);
	SetEnum(NOTRANS("TGM"),		eLSD_LOADCASE_TGM,		eLSD_LOADSELECT_TG,			eLSD_UNITLOAD_TG);
	SetEnum(NOTRANS("SD"),		eLSD_LOADCASE_SD,		eLSD_LOADSELECT_SD,			eLSD_UNITLOAD_SD);					// 지점이동 SD
	SetEnum(NOTRANS("EQL"),		eLSD_LOADCASE_EQ11,		eLSD_LOADSELECT_EQ,			eLSD_UNITLOAD_EQ);
	SetEnum(NOTRANS("EQR"),		eLSD_LOADCASE_EQ12,		eLSD_LOADSELECT_EQ,			eLSD_UNITLOAD_EQ);
	SetEnum(NOTRANS("EQSWL"),	eLSD_LOADCASE_EQ21,		eLSD_LOADSELECT_EQ,			eLSD_UNITLOAD_EQ);
	SetEnum(NOTRANS("EQSWR"),	eLSD_LOADCASE_EQ22,		eLSD_LOADSELECT_EQ,			eLSD_UNITLOAD_EQ);
	SetEnum(NOTRANS("SD1"),		eLSD_LOADCASE_SD1,		eLSD_LOADSELECT_SD,			eLSD_UNITLOAD_SD);
	SetEnum(NOTRANS("SD2"),		eLSD_LOADCASE_SD2,		eLSD_LOADSELECT_SD,			eLSD_UNITLOAD_SD);
	SetEnum(NOTRANS("SD3"),		eLSD_LOADCASE_SD3,		eLSD_LOADSELECT_SD,			eLSD_UNITLOAD_SD);
	SetEnum(NOTRANS("SD4"),		eLSD_LOADCASE_SD4,		eLSD_LOADSELECT_SD,			eLSD_UNITLOAD_SD);
	SetEnum(NOTRANS("SD5"),		eLSD_LOADCASE_SD5,		eLSD_LOADSELECT_SD,			eLSD_UNITLOAD_SD);
	SetEnum(NOTRANS("SD6"),		eLSD_LOADCASE_SD6,		eLSD_LOADSELECT_SD,			eLSD_UNITLOAD_SD);
	SetEnum(NOTRANS("SD7"),		eLSD_LOADCASE_SD7,		eLSD_LOADSELECT_SD,			eLSD_UNITLOAD_SD);
	SetEnum(NOTRANS("SD8"),		eLSD_LOADCASE_SD8,		eLSD_LOADSELECT_SD,			eLSD_UNITLOAD_SD);
	SetEnum(NOTRANS("SD9"),		eLSD_LOADCASE_SD9,		eLSD_LOADSELECT_SD,			eLSD_UNITLOAD_SD);
	SetEnum(NOTRANS("SD10"),		eLSD_LOADCASE_SD20,		eLSD_LOADSELECT_SD,			eLSD_UNITLOAD_SD);
	SetEnum(NOTRANS("SD11"),		eLSD_LOADCASE_SD11,		eLSD_LOADSELECT_SD,			eLSD_UNITLOAD_SD);
	SetEnum(NOTRANS("SD12"),		eLSD_LOADCASE_SD12,		eLSD_LOADSELECT_SD,			eLSD_UNITLOAD_SD);
	SetEnum(NOTRANS("SD13"),		eLSD_LOADCASE_SD13,		eLSD_LOADSELECT_SD,			eLSD_UNITLOAD_SD);
	SetEnum(NOTRANS("SD14"),		eLSD_LOADCASE_SD14,		eLSD_LOADSELECT_SD,			eLSD_UNITLOAD_SD);
	SetEnum(NOTRANS("SD15"),		eLSD_LOADCASE_SD15,		eLSD_LOADSELECT_SD,			eLSD_UNITLOAD_SD);
	SetEnum(NOTRANS("SD16"),		eLSD_LOADCASE_SD16,		eLSD_LOADSELECT_SD,			eLSD_UNITLOAD_SD);
	SetEnum(NOTRANS("SD17"),		eLSD_LOADCASE_SD17,		eLSD_LOADSELECT_SD,			eLSD_UNITLOAD_SD);
	SetEnum(NOTRANS("SD18"),		eLSD_LOADCASE_SD18,		eLSD_LOADSELECT_SD,			eLSD_UNITLOAD_SD);
	SetEnum(NOTRANS("SD19"),		eLSD_LOADCASE_SD19,		eLSD_LOADSELECT_SD,			eLSD_UNITLOAD_SD);
	SetEnum(NOTRANS("SD20"),		eLSD_LOADCASE_SD20,		eLSD_LOADSELECT_SD,			eLSD_UNITLOAD_SD);
	// 사용자하중적용위한 하중들
	SetEnum(NOTRANS("IM"),		eLSD_LOADCASE_IM,		eLSD_LOADSELECT_IM,			eLSD_UNITLOAD_IM);
	SetEnum(NOTRANS("BR"),		eLSD_LOADCASE_BR,		eLSD_LOADSELECT_BR,			eLSD_UNITLOAD_BR);
	SetEnum(NOTRANS("CF"),		eLSD_LOADCASE_CF,		eLSD_LOADSELECT_CF,			eLSD_UNITLOAD_CF);
	SetEnum(NOTRANS("WP"),		eLSD_LOADCASE_WP2,		eLSD_LOADSELECT_WP,			eLSD_UNITLOAD_WP);
	SetEnum(NOTRANS("WS"),		eLSD_LOADCASE_WS11,		eLSD_LOADSELECT_WS,			eLSD_UNITLOAD_WS);
	SetEnum(NOTRANS("FR"),		eLSD_LOADCASE_FR,		eLSD_LOADSELECT_FR,			eLSD_UNITLOAD_FR);
	SetEnum(NOTRANS("GD"),		eLSD_LOADCASE_GD,		eLSD_LOADSELECT_GD,			eLSD_UNITLOAD_GD);
	SetEnum(NOTRANS("IC"),		eLSD_LOADCASE_IC,		eLSD_LOADSELECT_IC,			eLSD_UNITLOAD_IC);
	SetEnum(NOTRANS("CT"),		eLSD_LOADCASE_CT11,		eLSD_LOADSELECT_CT,			eLSD_UNITLOAD_CT);
	SetEnum(NOTRANS("CV"),		eLSD_LOADCASE_CV1,		eLSD_LOADSELECT_CV,			eLSD_UNITLOAD_CV);
}

void CLsdManagerBridge::InitEnumForCantilever()
{
	SetEnum(NOTRANS("DC"),		eLSD_LOADCASE_DC1,		eLSD_LOADSELECT_DC,         eLSD_UNITLOAD_DC);
	SetEnum(NOTRANS("DW"),		eLSD_LOADCASE_DW1,		eLSD_LOADSELECT_DW,			eLSD_UNITLOAD_DW);
	SetEnum(NOTRANS("LL"),		eLSD_LOADCASE_LL1,		eLSD_LOADSELECT_LL1,		eLSD_UNITLOAD_LL);
	SetEnum(NOTRANS("PL"),		eLSD_LOADCASE_PL,		eLSD_LOADSELECT_PL,			eLSD_UNITLOAD_PL);
	SetEnum(NOTRANS("IM"),		eLSD_LOADCASE_IM,		eLSD_LOADSELECT_IM,			eLSD_UNITLOAD_IM);
	SetEnum(NOTRANS("CF"),		eLSD_LOADCASE_CF,		eLSD_LOADSELECT_CF,			eLSD_UNITLOAD_CF);
	SetEnum(NOTRANS("WSL"),		eLSD_LOADCASE_WS11,		eLSD_LOADSELECT_WS,			eLSD_UNITLOAD_WS);
	SetEnum(NOTRANS("WSR"),		eLSD_LOADCASE_WS12,		eLSD_LOADSELECT_WS,			eLSD_UNITLOAD_WS);
	SetEnum(NOTRANS("CTL"),		eLSD_LOADCASE_CT11,		eLSD_LOADSELECT_CT,			eLSD_UNITLOAD_CT);
	SetEnum(NOTRANS("CTR"),		eLSD_LOADCASE_CT12,		eLSD_LOADSELECT_CT,			eLSD_UNITLOAD_CT);
}

void CLsdManagerBridge::SetDataDefaultSelectLoadForBridge()
{
	HGINT32 iDataSize(0);
	CString szData[eLSD_LOADSELECT_COUNT];
	ELSDLoadSelectKind eIndex[eLSD_LOADSELECT_COUNT];

	std::vector<ELSDLoadSelectKind> vSelectKind;
	std::vector<HGBOOL>	vSelect;

	vSelectKind.push_back(eLSD_LOADSELECT_DC);	vSelect.push_back(TRUE);
	vSelectKind.push_back(eLSD_LOADSELECT_DW);	vSelect.push_back(TRUE);

	BOOL bOutWall = m_pBri->IsOutsideWall();
	vSelectKind.push_back(eLSD_LOADSELECT_EH);	vSelect.push_back(bOutWall);
	vSelectKind.push_back(eLSD_LOADSELECT_ES);	vSelect.push_back(bOutWall);
	vSelectKind.push_back(eLSD_LOADSELECT_LS);	vSelect.push_back(bOutWall);

	BOOL bRcSlab = m_pBri->IsRcSlab();
	vSelectKind.push_back(eLSD_LOADSELECT_EV);	vSelect.push_back(!bRcSlab);
	vSelectKind.push_back(eLSD_LOADSELECT_EQ);	vSelect.push_back(!bRcSlab);

	vSelectKind.push_back(eLSD_LOADSELECT_LL1);	vSelect.push_back(TRUE);
	vSelectKind.push_back(eLSD_LOADSELECT_PL);	vSelect.push_back(TRUE);

	BOOL bBoxType = m_pBri->IsBoxType();
	vSelectKind.push_back(eLSD_LOADSELECT_BP);	vSelect.push_back(bBoxType);
	vSelectKind.push_back(eLSD_LOADSELECT_WA);	vSelect.push_back(bBoxType);
	if(bBoxType==FALSE)
	{
		vSelectKind.push_back(eLSD_LOADSELECT_SD);	vSelect.push_back(!bBoxType);
	}

	vSelectKind.push_back(eLSD_LOADSELECT_TU);	vSelect.push_back(TRUE);
	vSelectKind.push_back(eLSD_LOADSELECT_TG);	vSelect.push_back(TRUE);
	vSelectKind.push_back(eLSD_LOADSELECT_SH);	vSelect.push_back(TRUE);

	// 나머지 기본으로 사용하지 않는 하중들... 사용자하중에 적용하기 위해 넣어준다.
	vSelectKind.push_back(eLSD_LOADSELECT_DD);	vSelect.push_back(FALSE);
	vSelectKind.push_back(eLSD_LOADSELECT_EL);	vSelect.push_back(FALSE);
	vSelectKind.push_back(eLSD_LOADSELECT_PS);	vSelect.push_back(FALSE);
	vSelectKind.push_back(eLSD_LOADSELECT_CR);	vSelect.push_back(FALSE);
	vSelectKind.push_back(eLSD_LOADSELECT_IM);	vSelect.push_back(FALSE);
	vSelectKind.push_back(eLSD_LOADSELECT_BR);	vSelect.push_back(FALSE);
	vSelectKind.push_back(eLSD_LOADSELECT_CF);	vSelect.push_back(FALSE);
	vSelectKind.push_back(eLSD_LOADSELECT_WP);	vSelect.push_back(FALSE);
	vSelectKind.push_back(eLSD_LOADSELECT_WS);	vSelect.push_back(FALSE);
	vSelectKind.push_back(eLSD_LOADSELECT_WL);	vSelect.push_back(FALSE);
	vSelectKind.push_back(eLSD_LOADSELECT_FR);	vSelect.push_back(FALSE);
	vSelectKind.push_back(eLSD_LOADSELECT_GD);	vSelect.push_back(FALSE);
	vSelectKind.push_back(eLSD_LOADSELECT_IC);	vSelect.push_back(FALSE);
	vSelectKind.push_back(eLSD_LOADSELECT_CT);	vSelect.push_back(FALSE);
	vSelectKind.push_back(eLSD_LOADSELECT_CV);	vSelect.push_back(FALSE);

	CString strDir(_T(""));
	long icKind = vSelectKind.size();
	for(long ix=0; ix<icKind; ++ix)
	{
		ELSDLoadSelectKind eKind = vSelectKind[ix];

		if(eKind == eLSD_LOADSELECT_EH || eKind == eLSD_LOADSELECT_ES || eKind == eLSD_LOADSELECT_LS)
			strDir = _T("SideAndFull");
		else if(eKind == eLSD_LOADSELECT_LL1)
			strDir = _T("LiveLoad");
		else
			strDir = _T("");

		szData[iDataSize].Format(_T("%s,%s,%s,,%s"), GetStringSelectLoadLsd(eKind), GetStringSelectLoadLsd(eKind, TRUE), vSelect[ix]? _T("TRUE") : _T("FALSE"), strDir);
		eIndex[iDataSize] = eKind;
		++iDataSize;
	}

	CLsdManager::SetDataDefaultSelectLoad(szData, eIndex, iDataSize, FALSE);

	if(TRUE)
	{
		GetDesignLoad(eLSD_LOADSELECT_EH)->SetValueVer(hggettext_be("양방향 재하"));
		GetDesignLoad(eLSD_LOADSELECT_ES)->SetValueVer(hggettext_be("양방향 재하"));
		GetDesignLoad(eLSD_LOADSELECT_LS)->SetValueVer(hggettext_be("양방향 재하"));
		GetDesignLoad(eLSD_LOADSELECT_LL1)->SetValueVer(hggettext_be("차량하중"));
	}
}

void CLsdManagerBridge::SetDataDefaultSelectLoadForCantilever()
{
	HGINT32 iDataSize(0);
	CString szData[eLSD_LOADSELECT_COUNT];
	ELSDLoadSelectKind eIndex[eLSD_LOADSELECT_COUNT];

	std::vector<ELSDLoadSelectKind> vSelectKind;
	std::vector<HGBOOL>	vSelect;

	vSelectKind.push_back(eLSD_LOADSELECT_DC);	vSelect.push_back(TRUE);
	vSelectKind.push_back(eLSD_LOADSELECT_DW);	vSelect.push_back(TRUE);
	vSelectKind.push_back(eLSD_LOADSELECT_LL1);	vSelect.push_back(TRUE);
	vSelectKind.push_back(eLSD_LOADSELECT_PL);	vSelect.push_back(TRUE);
	vSelectKind.push_back(eLSD_LOADSELECT_IM);	vSelect.push_back(TRUE);
	vSelectKind.push_back(eLSD_LOADSELECT_CF);	vSelect.push_back(TRUE);
	vSelectKind.push_back(eLSD_LOADSELECT_WS);	vSelect.push_back(TRUE);
	vSelectKind.push_back(eLSD_LOADSELECT_CT);	vSelect.push_back(TRUE);
	
	CString strDir(_T(""));
	long icKind = vSelectKind.size();
	for(long ix=0; ix<icKind; ++ix)
	{
		ELSDLoadSelectKind eKind = vSelectKind[ix];

		if(eKind == eLSD_LOADSELECT_WS || eKind == eLSD_LOADSELECT_CT)
			strDir = _T("HorFull");
		else 
			strDir = _T("");

		szData[iDataSize].Format(_T("%s,%s,%s,,%s"), GetStringSelectLoadLsd(eKind), GetStringSelectLoadLsd(eKind, TRUE), vSelect[ix]? _T("TRUE") : _T("FALSE"), strDir);
		eIndex[iDataSize] = eKind;
		++iDataSize;
	}

	CLsdManager::SetDataDefaultSelectLoad(szData, eIndex, iDataSize, FALSE);

	if(TRUE)
	{
		GetDesignLoad(eLSD_LOADSELECT_WS)->SetValueVer(hggettext_be("양방향 재하"));
		GetDesignLoad(eLSD_LOADSELECT_CT)->SetValueVer(hggettext_be("양방향 재하"));
	}
}

long CLsdManagerBridge::GetindexLoadModifiedFactor( long ixPart )
{
	long nIndexModifiyed(0);

	// 위치별로 하중수정계수의 인덱스를 만들어 준다
	if(ixPart == ePartOutWallEnd)
		ixPart = ePartOutWallStt;
	else if(ixPart == ePartFootEnd)
		ixPart = ePartFootStt;
	else if(ixPart == ePartWingWallEnd)
		ixPart = ePartWingWallStt;
	else if(ixPart == ePartApsEnd)
		ixPart = ePartApsStt;

	for(long ix=0; ix<ePartCount; ix++)
	{
		if(m_pBri->IsUsePart((EPartOfBridge)ix, 1) == FALSE)
			continue;

		if(ix == ixPart)
		{
			break;
		}
		else
		{
			++nIndexModifiyed;
		}
	}

	return nIndexModifiyed;
}

double CLsdManagerBridge::GetEtaValue(/*EPartOfBridge*/ long nPart, CString szComboFirst)
{
	double dVal(0.);

	for(long nL=0; nL<eLSD_COMBINE_TOTAL_COUNT; nL++)
	{	
		ELSDCombKind eKind = static_cast<ELSDCombKind>(nL);

		std::vector<CLsdLoadCase *> LCArray = GetLsdLoadCaseArray(eKind,eLSD_LOADCASE_DIR_BRI);
		if(LCArray.size() > 0)
		{
			for (long nLC=0; nLC<(long)LCArray.size(); ++nLC)
			{
				if( szComboFirst == LCArray[nLC]->GetName() )
					dVal = GetValueLoadModifiedFactor(eKind, nPart, LCArray[0]);
			}
		}
	}

	return dVal;
}
